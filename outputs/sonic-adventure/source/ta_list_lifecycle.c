#include "ta_list_lifecycle.h"
SA_STAGE_EXPORT void sa_ta_list_lifecycle_init(struct sa_ta_list_lifecycle *s,uint32_t enabled){
 if(!s)return;volatile uint8_t *p=(volatile uint8_t *)s;for(unsigned i=0;i<sizeof(*s);i++)p[i]=0;s->enabled=!!enabled;
}
static int read(void *context,uint32_t a,unsigned n,uint32_t *value){
 struct sa_ta_list_binding *b=context;return b->parent.read?b->parent.read(b->parent.context,a,n,value):-2;
}
static int write(void *context,uint32_t a,uint32_t value,unsigned n){
 struct sa_ta_list_binding *b=context;struct sa_ta_list_lifecycle *s=b->state;
 uint32_t address=a&0x1fffffffu,count=b->frame->packet_count,cursor=b->job?b->job->cursor:0;
 int init=s->enabled&&address==0x005f8144u&&(value&0x80000000u);
 if(s->enabled){
  if((address==0x005f8008u||address==0x005f8124u||address==0x005f8128u||address==0x005f8144u)&&n!=4u)return -3;
  if(address==0x005f8008u&&(value&1u)&&s->soft_resets==UINT64_MAX)return -6;
  if(init){
   if(count>SA_FRAME_COMMAND_CAPACITY||count>b->frame->packet_capacity||cursor>count||b->events->cursor>count)return -6;
   if(s->inits==UINT64_MAX||s->retired>UINT64_MAX-count||s->discarded>UINT64_MAX-(count-cursor))return -6;
  }
 }
 if(!b->parent.write)return -2;int rc=b->parent.write(b->parent.context,a,value,n);if(rc)return rc;
 if(!s->enabled)return 0;
 if(init){
  if(b->frame->packet_count!=count||(b->events->enabled&&b->events->cursor!=count))return -6;
  volatile uint32_t *last=(volatile uint32_t *)s->last;for(unsigned i=0;i<sizeof(s->last)/4u;i++)last[i]=0;
  for(unsigned i=0;i<count;i++)s->last[i]=b->frame->packets[i];
  s->last_count=count;s->inits++;s->retired+=count;s->discarded+=count-cursor;
  /* Current-list storage is distinct from the immutable submitted job. */
  volatile uint32_t *packets=(volatile uint32_t *)b->frame->packets;for(unsigned i=0;i<sizeof(b->frame->packets)/4u;i++)packets[i]=0;
  b->frame->packet_count=0;b->events->cursor=0;b->events->list=0;b->events->active=0;
  if(b->job)b->job->cursor=0;
 }
 if(address==0x005f8124u)s->opb_base=value;
 if(address==0x005f8128u)s->isp_base=value;
 if(address==0x005f8008u&&(value&1u))s->soft_resets++;
 return 0;
}
static int cache(void *context,uint32_t a,unsigned op){
 struct sa_ta_list_binding *b=context;return b->parent.cache?b->parent.cache(b->parent.context,a,op):-2;
}
SA_STAGE_EXPORT int sa_ta_list_lifecycle_bind(struct sa_ta_list_binding *b,struct sa_ta_list_lifecycle *s,struct sa_frame_command_memory *frame,struct sa_frame_events_bus *events,struct sa_pixel_job_state *job,const struct sa_vertex_bus *parent,struct sa_vertex_bus *out){
 if(!b||!s||!frame||!events||!parent||!out)return -6;
 b->state=s;b->frame=frame;b->events=events;b->job=job;b->parent=*parent;
 *out=(struct sa_vertex_bus){b,read,write,cache};return 0;
}
