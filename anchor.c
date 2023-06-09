#include <stdio.h> // for printf

#include "contiki.h" //  contiki lib RTOS
#include "sys/log.h" //  for loggin ip

/*network libs*/
#include "net/ipv6/simple-udp.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "packetbuf.h"
#include "random.h"
#include "radio.h"

#define SEND_INTERVAL (60 * CLOCK_SECOND)

/*Communication def*/
#define SOURCE_PORT 5555
#define DEST_PORT 7777

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/*Communication def*/
#define WITH_SERVER_REPLY 0

static struct simple_udp_connection udp_conn;
static struct radio_driver radio;

static void
udp_rx_callback(struct simple_udp_connection *c,
                const uip_ipaddr_t *sender_addr,
                uint16_t sender_port,
                const uip_ipaddr_t *receiver_addr,
                uint16_t receiver_port,
                const uint8_t *data,
                uint16_t datalen)
{
    radio.on();
    LOG_INFO("Received data '%.*s' from ", datalen, (char *)data);
    LOG_INFO_6ADDR(sender_addr);
    LOG_INFO_("\n");
    // int rssi = (int16_t)packetbuf_attr(PACKETBUF_ATTR_RSSI);
    // // radio_value_t radio_rssi;
    // // NETSTACK_RADIO.get_value(RADIO_PARAM_LAST_RSSI, &radio_rssi);
    // printf("value is %d from call back", rssi);

#if WITH_SERVER_REPLY
    /* send back the same string to the client as an echo reply */
    LOG_INFO("Sending response.\n");
    simple_udp_sendto(c, data, datalen, sender_addr);
#endif /* WITH_SERVER_REPLY */
}

PROCESS(sink_process, "Sink Process");
AUTOSTART_PROCESSES(&sink_process);

PROCESS_THREAD(sink_process, ev, data)
{

    static struct etimer et; // event timer

    PROCESS_BEGIN();

    /* Initialize DAG root */
    NETSTACK_ROUTING.root_start();

    puts("Sink starting -- server udp");

    simple_udp_register(&udp_conn, SOURCE_PORT, NULL,
                        DEST_PORT, udp_rx_callback);

    printf("\ninit sensing process\n");

    etimer_set(&et, random_rand() % SEND_INTERVAL);
    while (1)
    {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        radio_value_t radio_rssi;
        NETSTACK_RADIO.get_value(RADIO_PARAM_LAST_RSSI, &radio_rssi);
        printf("%d", radio_rssi);
        etimer_set(&et, CLOCK_SECOND * 5);
    }

    PROCESS_END();
}