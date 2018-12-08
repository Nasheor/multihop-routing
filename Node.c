#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"

#include "dev/button-sensor.h"

#include "dev/leds.h"

#include <stdio.h>

/* This is the structure of broadcast messages. */
struct broadcast_message {
  int seqno; 
  int  hop;
};
/*---------------------------------------------------------------------------*/
PROCESS(broadcast_process, "Node Process");
AUTOSTART_PROCESSES(&broadcast_process);
/*---------------------------------------------------------------------------*/

static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
    struct broadcast_message *m;
    m = packetbuf_dataptr(); 

     printf("Broadcast: %d.%d\nHop Count: %d\nSequence Number: %d", from->u8[0], from->u8[1], m->hop, m->seqno);
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(broadcast_process, ev, data)
{
static struct etimer et;
static int seqno;
struct broadcast_message msg; 
PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

PROCESS_BEGIN();

broadcast_open(&broadcast, 140, &broadcast_call);

struct broadcast_message *message = calloc(sizeof(struct broadcast_message), 1);
while(1) {

/* Delay 5-10 seconds */
etimer_set(&et, CLOCK_SECOND * 5 + random_rand() % (CLOCK_SECOND * 4));

PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));


packetbuf_copyfrom(&msg, sizeOf(struct broadcast_message));
broadcast_send(&broadcast);
printf("Broadcast message sent\n");
broadcast_send(&broadcast)
}

PROCESS_END();
}
/*---------------------------------------------------------------------------*/