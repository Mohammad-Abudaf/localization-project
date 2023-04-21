#include "contiki.h"
#include "radio.h"
#include "sys/log.h"
#include "net/ipv6/simple-udp.h"
#include "net/routing/routing.h"
#include "net/netstack.h"

#define SOURCE_PORT 5678
#define DEST_PORT 8888

static struct simple_udp_connection udp_conn;

static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
    printf("broadcast message received from %d.%d: '%s'\n",
           from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
}

static struct broadcast_conn broadcast;

void mac_callback(void *dataPrt);

PROCESS(source_localization, "PROCESS: Source");
AUTOSTART_PROCESSES(&source_localization);

PROCESS_THREAD(source_localization, ev, data)
{

    static struct etimer periodic_time;

    PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
    PROCESS_BEGIN();
    broadcast_open(&broadcast, 129, &broadcast_recv);

    etimer_set(&periodic_time, CLOCK_SECOND * 3);
    while (1)
    {

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_time));
        packetbuf_copyfrom("Hello", 6);
        broadcast_send(&broadcast);
        
        printf("broadcast message sent\n");

        etimer_reset(&periodic_time);
    }
    PROCESS_END();
}