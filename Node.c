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

struct broadcast_message status;

static linkaddr_t parent; 
static linkaddr_t sink;


/*---------------------------------------------------------------------------*/
PROCESS(broadcast_process, "Node Process");
AUTOSTART_PROCESSES(&broadcast_process);
/*---------------------------------------------------------------------------*/

static struct broadcast_conn broadcast;

static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{

    struct broadcast_message *m;
    m = packetbuf_dataptr(); 
    if(m->hop == 0){
      printf("Hop Count: %d\nSequence Number: %d\n", m->hop, m->seqno);
      sink.u8[0] = from-> u8[0];
      sink.u8[1] = from-> u8[0];
      status.hop = m->hop+1;
      status.seqno = m->seqno;
      packetbuf_copyfrom(&status, sizeof(struct broadcast_message));
      broadcast_send(&broadcast);
    }
    else if(m->seqno > status.seqno || (parent.u8[0] == from -> u8[0] && parent.u8[1] == from -> u8[1])){
      printf("Hop Count: %d\nSequence Number: %d\n", m->hop, m->seqno);
      status.hop = m->hop;
      status.seqno = m->seqno;
      parent.u8[0] = from->u8[0];
      parent.u8[1] = from->u8[1];
      status.hop = m -> hop + 1;
      packetbuf_copyfrom(&status, sizeof(struct broadcast_message));
      broadcast_send(&broadcast);
    }
    else{
      //printf("Waiting for an Updated Sequence Number");
    }
}

static const struct broadcast_callbacks broadcast_call = {broadcast_recv};

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(broadcast_process, ev, data)
{

PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

PROCESS_BEGIN();

broadcast_open(&broadcast, 140, &broadcast_call);

PROCESS_END();
}
/*---------------------------------------------------------------------------*/