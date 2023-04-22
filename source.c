#include <math.h>
#include <stdio.h> // for printf();
#include <random.h>
#include <inttypes.h>

#include "contiki.h"   // RTOS


// for logging
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/*networking libs*/
#include "net/ipv6/simple-udp.h"
#include "net/routing/routing.h"
#include "net/netstack.h"

/*Communication def*/
#define SOURCE_PORT 5555
#define DEST_PORT 7777



#define SEND_INTERVAL (60 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;

PROCESS(sensing_process, "PROCESS: sensing");

AUTOSTART_PROCESSES(&sensing_process);

PROCESS_THREAD(sensing_process, ev, data)
{
    
    static struct etimer et; // event timer

    uip_ipaddr_t dest_ip_address;
    static char payload[32]; // msg payload

    PROCESS_BEGIN(); // starting of the process

    simple_udp_register(&udp_conn,
                        DEST_PORT,
                        NULL,
                        SOURCE_PORT,
                        NULL);

    printf("\ninit sensing process\n");

    etimer_set(&et, random_rand() % SEND_INTERVAL);
    while (1)
    {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if (NETSTACK_ROUTING.node_is_reachable() &&
            NETSTACK_ROUTING.get_root_ipaddr(&dest_ip_address))
        {

            
            LOG_INFO_6ADDR(&dest_ip_address);
            LOG_INFO_("\n");

            // fill the payload!
            snprintf(payload, sizeof(payload), "Hello");

            // send the packet
            simple_udp_sendto(&udp_conn, payload,
                              strlen(payload), &dest_ip_address);
        }
        /* Add some jitter */
        etimer_set(&et, SEND_INTERVAL - CLOCK_SECOND + (random_rand() % (2 * CLOCK_SECOND)));
    }

    PROCESS_END(); // end of the process
}
