#include "frame_events_bus.h"
static int consume(struct sa_frame_events_bus *s){
 static const uint32_t event[5]={1u<<7,1u<<8,1u<<9,1u<<10,1u<<21};
 if(!s->enabled)return 0;
 if(s->frame->packet_count>SA_FRAME_COMMAND_CAPACITY||s->cursor>s->frame->packet_count)return -6;
 while(s->cursor<s->frame->packet_count){
  const struct sa_frame_command_packet *p=&s->frame->packets[s->cursor];
  uint32_t word=p->words[0],kind=word>>29;
  if(kind==4u||kind==5u){
   uint32_t list=(word>>24)&7u;
   if(list>4u){s->rejected++;return -2;}
   s->list=list;s->active=1;s->headers++;
  }else if(kind==7u){
   if(!s->active){s->rejected++;return -2;}
   s->vertices++;
  }else if(kind==0u){
   uint32_t nonzero=0;for(unsigned i=0;i<8;i++)nonzero|=p->words[i];
   if(nonzero||!s->active||s->list>4u){s->rejected++;return -2;}
   s->pending|=event[s->list];
   if(s->interrupt_pending)*s->interrupt_pending|=event[s->list];
   s->active=0;s->ends++;
  }else if(kind!=1u){s->rejected++;return -2;}
  s->cursor++;
 }
 return 0;
}
static int read(void *context,uint32_t a,unsigned n,uint32_t *value){
 struct sa_frame_events_bus *s=context;if(!s->parent.read)return -2;
 int status=s->parent.read(s->parent.context,a,n,value);
 if(!status&&n==4u&&(a&0x1fffffffu)==0x005f6900u)*value|=s->pending;
 return status;
}
static int write(void *context,uint32_t a,uint32_t value,unsigned n){
 struct sa_frame_events_bus *s=context;if(!s->parent.write)return -2;
 int status=s->parent.write(s->parent.context,a,value,n);if(status)return status;
 if(n==4u&&(a&0x1fffffffu)==0x005f6900u){
  s->pending&=~value;if(s->interrupt_pending)*s->interrupt_pending&=~value;
 }
 return consume(s);
}
static int cache(void *context,uint32_t a,unsigned op){
 struct sa_frame_events_bus *s=context;if(!s->parent.cache)return -2;
 int status=s->parent.cache(s->parent.context,a,op);return status?status:consume(s);
}
int sa_frame_events_bus_init(struct sa_frame_events_bus *s,struct sa_frame_command_memory *frame,const struct sa_vertex_bus *parent,struct sa_vertex_bus *out,uint32_t *pending,const uint32_t *seed,uint32_t enabled){
 if(!s||!frame||!parent||!out)return -6;
 s->parent=*parent;s->frame=frame;s->interrupt_pending=pending;
 uint32_t *fields=&s->cursor;for(unsigned i=0;i<9;i++)fields[i]=seed?seed[i]:0;
 s->enabled=!!enabled;
 if(pending)*pending|=s->pending;
 *out=(struct sa_vertex_bus){s,read,write,cache};return consume(s);
}
