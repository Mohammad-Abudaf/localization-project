#include <stdio.h> // for printf

#include "contiki.h" //  contiki lib RTOS
#include "sys/log.h" //  for loggin ip

/*network libs*/
#include "net/ipv6/simple-udp.h"
#include "net/routing/routing.h"
#include "net/netstack.h"


/*Communication def*/
#define SOURCE_PORT 5555
#define DEST_PORT 7777

static struct simple_udp_connection udp_conn;


PROCESS(sink_process, "Sink Process");
AUTOSTART_PROCESSES(&sink_process);

PROCESS_THREAD(sink_process, ev, data)
{

    PROCESS_BEGIN();

    /* Initialize DAG root */
    NETSTACK_ROUTING.root_start();

    puts("Sink starting -- server udp");

    simple_udp_register(&udp_conn, DEST_PORT, NULL,
                        SOURCE_PORT, NULL);


    PROCESS_END();
}