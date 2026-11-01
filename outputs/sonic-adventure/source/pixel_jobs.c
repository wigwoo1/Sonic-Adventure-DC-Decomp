#include "pixel_jobs.h"
SA_STAGE_EXPORT void sa_pixel_job_init(struct sa_pixel_job_state *s){if(s){volatile uint8_t *p=(volatile uint8_t *)s;for(unsigned i=0;i<sizeof(*s);i++)p[i]=0;}}
static int read(void *opaque,uint32_t a,unsigned n,uint32_t *out){struct sa_pixel_job_binding *b=opaque;return b->parent.read?b->parent.read(b->parent.context,a,n,out):-2;}
static int write(void *opaque,uint32_t a,uint32_t value,unsigned n){
 struct sa_pixel_job_binding *b=opaque;struct sa_pixel_job_state *s=b->state;
 int capture=(a&0x1fffffffu)==0x005f8014u&&value==0xffffffffu&&b->render->enabled;
 if(capture){
  if(n!=4u)return -3;if(s->ready)return -4;
  if(b->render->submitted==UINT64_MAX||b->frame->packet_count>64u||s->cursor>b->frame->packet_count)return -6;
 }
 if(!b->parent.write)return -2;int rc=b->parent.write(b->parent.context,a,value,n);if(rc)return rc;
 if(capture){
  s->count=b->frame->packet_count-s->cursor;s->job_id=b->render->submitted+1u;
  volatile uint32_t *words=s->words;for(unsigned i=0;i<512;i++)words[i]=0;
  for(unsigned i=0;i<s->count;i++)for(unsigned k=0;k<8;k++)s->words[i*8u+k]=b->frame->packets[s->cursor+i].words[k];
  s->cursor=b->frame->packet_count;s->ready=1;
 }
 if((a&0x1fffffffu)==0x005f8008u&&n==4u&&(value&2u))s->ready=0;
 return 0;
}
static int cache(void *opaque,uint32_t a,unsigned op){struct sa_pixel_job_binding *b=opaque;return b->parent.cache?b->parent.cache(b->parent.context,a,op):-2;}
SA_STAGE_EXPORT int sa_pixel_job_bind(struct sa_pixel_job_binding *b,struct sa_pixel_job_state *s,struct sa_render_completion *render,const struct sa_frame_command_memory *frame,const struct sa_vertex_bus *parent,struct sa_vertex_bus *out){
 if(!b||!s||!render||!frame||!parent||!out)return -6;
 b->state=s;b->render=render;b->frame=frame;b->parent=*parent;*out=(struct sa_vertex_bus){b,read,write,cache};return 0;
}
SA_STAGE_EXPORT int sa_pixel_job_execute(struct sa_pixel_job_state *s,struct sa_render_completion *r,const uint8_t *vram,uint32_t size,uint32_t *pixels,uint32_t *depth,uint64_t capacity,const struct sa_pixel_options *o){
 if(!s||!r)return -6;if(!s->ready||!r->enabled)return 0;
 if(!s->job_id||s->job_id!=r->active_id||s->count>64u||s->processed==UINT64_MAX||r->completed==UINT64_MAX)return -6;
 if(!o||!pixels||!depth)return -6;
 if(!o->width||!o->height||o->width>8192u||o->height>8192u||o->textures>1u||o->alpha_ref>255u||capacity<(uint64_t)o->width*o->height||size>0x800000u)return -3;
 /* Current host profile uses the original zero-origin viewport. */
 if((r->job[7]&0xffffu)||(r->job[8]&0xffffu)||o->input_width!=((r->job[7]>>16)&1023u)+1u||o->input_height!=((r->job[8]>>16)&1023u)+1u)return -2;
 int rc=0;
 if(s->count)rc=sa_pixel_draw(s->words,s->count,vram,size,pixels,depth,capacity,o,&s->stats);
 else{volatile uint8_t *p=(volatile uint8_t *)&s->stats;for(unsigned i=0;i<sizeof(s->stats);i++)p[i]=0;}
 if(rc)return rc;
 /* Nothing is acknowledged until native packet processing has succeeded. */
 rc=sa_render_completion_finish(r,s->job_id);if(rc)return rc;
 s->processed++;s->last_job=s->job_id;s->ready=0;return 0;
}
