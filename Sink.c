#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"

#include "dev/button-sensor.h"

#include "dev/leds.h"

#include <stdio.h>

typedef enum { false, true } bool;

/* This is the structure of broadcast messages. */
struct broadcast_message {
  int seqno;
  int  hop;
  bool wipe;
};

/*---------------------------------------------------------------------------*/
PROCESS(broadcast_process, "Broadcast example");
AUTOSTART_PROCESSES(&broadcast_process);
/*---------------------------------------------------------------------------*/

static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{

}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(broadcast_process, ev, data)
{
static struct etimer et;
static int seqno = 0;
struct broadcast_message msg; 
PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

PROCESS_BEGIN();

broadcast_open(&broadcast, 140, &broadcast_call);

//struct broadcast_message *message = calloc(sizeof(struct broadcast_message), 1);
while(1) {

  /* Delay 5-10 seconds */
  etimer_set(&et, CLOCK_SECOND * 8 + random_rand() % (CLOCK_SECOND * 8));

  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

  //Wipes the Tree every 16 hops, resetting the sequence number and setting wipe state to true
  if(seqno>16){
    msg.seqno = 0;
    msg.hop = 1;
    msg.wipe = true;
    packetbuf_copyfrom(&msg, sizeof(struct broadcast_message));
    broadcast_send(&broadcast);
    printf("Wipe Message Sent\n");
    seqno = 0;
  }
  //Increase the Sequence number sequentially every time the sink sends out a broadcast message 
  else{
      msg.seqno = seqno; 
      msg.hop = 0;
      msg.hop = false;
      packetbuf_copyfrom(&msg, sizeof(struct broadcast_message));
      printf("Broadcast message sent From Sink\n");
      broadcast_send(&broadcast);
      seqno++;
  }
}

PROCESS_END();
}
/*---------------------------------------------------------------------------*/