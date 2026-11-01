#include "render_completion.h"
static int parameter(uint32_t a){
 static const uint32_t offsets[SA_RENDER_PARAMETER_COUNT]={0x20,0x2c,0x30,0x48,0x4c,0x60,0x64,0x68,0x6c,0x74,0x78,0x7c,0x80,0x84,0x88,0x8c};
 for(unsigned i=0;i<SA_RENDER_PARAMETER_COUNT;i++)if((a&0x1fffffffu)==0x005f8000u+offsets[i])return (int)i;
 return -1;
}
SA_STAGE_EXPORT void sa_render_completion_init(struct sa_render_completion *s,const uint32_t *parameters){
 if(!s)return;
 volatile uint8_t *p=(volatile uint8_t *)s;for(unsigned i=0;i<sizeof(*s);i++)p[i]=0;
 s->enabled=1;for(unsigned i=0;i<SA_RENDER_PARAMETER_COUNT;i++)s->parameters[i]=parameters?parameters[i]:0;
}
SA_STAGE_EXPORT int sa_render_completion_finish(struct sa_render_completion *s,uint64_t id){
 if(!s||!id||!s->enabled||s->active_id!=id||s->completed==UINT64_MAX)return -6;
 /* Completion is an explicit backend input. It never comes from a read,
  * VBlank, a timeout, or a fabricated game result. All three render stages
  * complete together in this declared host-job profile; physical order is open. */
 s->completed++;s->active_id=0;s->pending|=7u;return 0;
}
static int read(void *opaque,uint32_t a,unsigned n,uint32_t *out){
 struct sa_render_completion_binding *b=opaque;
 if((a&0x1fffffffu)==0x005f6900u&&(n!=4u||!out))return -3;
 if(!b->parent.read)return -2;
 int rc=b->parent.read(b->parent.context,a,n,out);if(rc)return rc;
 if((a&0x1fffffffu)==0x005f6900u)*out|=b->state->pending;
 return 0;
}
static int write(void *opaque,uint32_t a,uint32_t value,unsigned n){
 struct sa_render_completion_binding *b=opaque;struct sa_render_completion *s=b->state;
 uint32_t physical=a&0x1fffffffu;int index=parameter(a);
 int start=physical==0x005f8014u,reset=physical==0x005f8008u,ack=physical==0x005f6900u;
 if((start||reset||ack||index>=0)&&n!=4u)return -3;
 if(start&&value&&value!=0xffffffffu)return -2;
 if(s->enabled&&start&&value){
  /* One in-flight job. Research capacity errors are not hardware faults. */
  if(s->active_id||s->pending)return -4;
  if(s->submitted==UINT64_MAX)return -6;
 }
 if(reset&&(value&2u)&&s->active_id&&s->cancelled==UINT64_MAX)return -6;
 if(!b->parent.write)return -2;
 int rc=b->parent.write(b->parent.context,a,value,n);if(rc)return rc;
 if(index>=0)s->parameters[index]=value;
 if(ack)s->pending&=~value;
 if(reset&&(value&2u)&&s->active_id){s->active_id=0;s->cancelled++;}
 if(s->enabled&&start&&value){
  s->submitted++;s->active_id=s->submitted;
  for(unsigned i=0;i<SA_RENDER_PARAMETER_COUNT;i++)s->job[i]=s->parameters[i];
 }
 return 0;
}
static int cache(void *opaque,uint32_t a,unsigned op){struct sa_render_completion_binding *b=opaque;return b->parent.cache?b->parent.cache(b->parent.context,a,op):-2;}
SA_STAGE_EXPORT int sa_render_completion_bind(struct sa_render_completion_binding *b,struct sa_render_completion *s,const struct sa_vertex_bus *parent,struct sa_vertex_bus *out){
 if(!b||!s||!parent||!out)return -6;
 b->state=s;b->parent=*parent;*out=(struct sa_vertex_bus){b,read,write,cache};return 0;
}
