/* Authored native transfer/event service, not recovered game code.
 * CPU-triggered, untimed transfers advance at declared guest service boundaries.
 * No game completion flag or game-helper return is supplied by this platform. */
#include "transfer_runtime.h"
static int channel_word(uint32_t a){
 uint32_t p=a&0x1fffffffu;
 return p>=0x005f7800u&&p<0x005f7880u?(int)((p-0x005f7800u)/4u):-1;
}
static int read(void *opaque,uint32_t a,unsigned n,uint32_t *value){
 struct sa_transfer_runtime_binding *b=opaque;
 if(!b->parent.read)return -2;
 int status=b->parent.read(b->parent.context,a,n,value);if(status)return status;
 if((a&0x1fffffffu)==0x005f6900u&&n==4u)*value|=b->state->pending;
 return 0;
}
static int write(void *opaque,uint32_t a,uint32_t value,unsigned n){
 struct sa_transfer_runtime_binding *b=opaque;struct sa_transfer_runtime *s=b->state;
 if(!b->parent.write)return -2;
 int status=b->parent.write(b->parent.context,a,value,n);if(status)return status;
 if((a&0x1fffffffu)==0x005f6900u&&n==4u)s->pending&=~value;
 int word=channel_word(a);
 if(word>=0&&n==4u&&!(a&3u)){
  unsigned ch=(unsigned)word/8u,index=(unsigned)word&7u;s->registers[word]=value;
  if((index==5u||index==6u)&&!(value&1u)){s->active[ch]=0;s->progress[ch]=0;}
  if(index==6u&&(value&1u)&&(s->registers[ch*8u+5u]&1u)&&!(s->registers[ch*8u+4u]&2u)){
   int captured=sa_g2_transfer_capture(ch,s->registers+ch*8u,&s->jobs[ch]);
   if(captured)return captured;
   s->progress[ch]=0;s->active[ch]=1;
  }
 }
 return 0;
}
static int cache(void *opaque,uint32_t a,unsigned op){
 struct sa_transfer_runtime_binding *b=opaque;
 return b->parent.cache?b->parent.cache(b->parent.context,a,op):-2;
}
static int range(uint32_t a,uint32_t n,uint32_t first,uint32_t size){
 return a>=first&&a-first<=size&&n<=size-(a-first);
}
static int data_read(void *opaque,uint32_t a,uint8_t *out,uint32_t n){
 struct sa_transfer_runtime_binding *b=opaque;
 if(range(a,n,0x0c000000u,0x1000000u)){for(uint32_t i=0;i<n;i++)out[i]=b->ram[a-0x0c000000u+i];return 0;}
 if(!range(a,n,0x00800000u,0x200000u)||!b->parent.read)return -2;
 for(uint32_t i=0;i<n;i+=4){uint32_t value;int status=b->parent.read(b->parent.context,a+i,4,&value);if(status)return status;for(unsigned j=0;j<4;j++)out[i+j]=(uint8_t)(value>>(j*8));}
 return 0;
}
static int data_write(void *opaque,uint32_t a,const uint8_t *in,uint32_t n){
 struct sa_transfer_runtime_binding *b=opaque;
 if(range(a,n,0x0c000000u,0x1000000u)){for(uint32_t i=0;i<n;i++)b->ram[a-0x0c000000u+i]=in[i];return 0;}
 if(!range(a,n,0x00800000u,0x200000u)||!b->parent.write)return -2;
 for(uint32_t i=0;i<n;i+=4){uint32_t value=0;for(unsigned j=0;j<4;j++)value|=(uint32_t)in[i+j]<<(j*8);int status=b->parent.write(b->parent.context,a+i,value,4);if(status)return status;}
 return 0;
}
SA_STAGE_EXPORT int sa_transfer_runtime_init(struct sa_transfer_runtime *s,uint32_t enabled){
 if(!s)return -6;
 volatile uint8_t *bytes=(volatile uint8_t *)s;for(unsigned i=0;i<sizeof(*s);i++)bytes[i]=0;
 s->enabled=!!enabled;return 0;
}
SA_STAGE_EXPORT int sa_transfer_runtime_bind(struct sa_transfer_runtime_binding *b,struct sa_transfer_runtime *s,uint8_t *ram,const struct sa_vertex_bus *parent,struct sa_vertex_bus *out){
 if(!b||!s||!ram||!parent||!out)return -6;
 b->state=s;b->parent=*parent;b->ram=ram;*out=(struct sa_vertex_bus){b,read,write,cache};return 0;
}
SA_STAGE_EXPORT int sa_transfer_runtime_tick(struct sa_transfer_runtime_binding *b){
 if(!b||!b->state)return -6;
 struct sa_transfer_runtime *s=b->state;if(!s->enabled||s->in_irq)return 0;s->polls++;
 for(unsigned ch=0;ch<4;ch++)if(s->active[ch]){
  struct sa_g2_transfer job=s->jobs[ch];uint32_t length=job.length&0x7fffffffu;
  uint32_t previous=s->progress[ch];if(previous>length)return -6;
  job.root_address+=previous;job.g2_address+=previous;job.length=length-previous;
  struct sa_g2_bus bus={b,data_read,data_write};uint32_t completed,event;
  int status=sa_g2_transfer_execute(&job,&bus,&completed,&event);
  s->progress[ch]+=completed;s->bytes+=completed;if(status)return status;
  uint32_t a=0xa05f7800u+ch*32u;
  if(!b->parent.write)return -2;
  status=b->parent.write(b->parent.context,a+24u,0,4);if(status)return status;s->registers[ch*8u+6u]=0;
  if(s->jobs[ch].length&0x80000000u){status=b->parent.write(b->parent.context,a+20u,0,4);if(status)return status;s->registers[ch*8u+5u]=0;}
  s->active[ch]=0;s->pending|=1u<<(15u+ch);s->transfers++;
  (void)event; /* Pending is derived from the captured channel after real bytes. */
 }
 return 0;
}
SA_STAGE_EXPORT int sa_transfer_runtime_irq_ready(struct sa_transfer_runtime_binding *b,uint32_t *ready){
 if(!b||!b->state||!ready)return -6;
 *ready=0;if(!b->state->enabled||!b->state->pending)return 0;
 if(!b->parent.read)return -2;
 uint32_t mask;int status=b->parent.read(b->parent.context,0xa05f6920u,4,&mask);if(status)return status;
 *ready=!!(mask&b->state->pending);return 0;
}
