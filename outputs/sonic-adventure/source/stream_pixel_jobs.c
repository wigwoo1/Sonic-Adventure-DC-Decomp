#include "stream_pixel_jobs.h"
#include "banked_pixel_jobs.h"
SA_STAGE_EXPORT void sa_stream_pixel_job_dispose(struct sa_stream_pixel_job *s){if(s){sa_ta_stream_destroy(s->snapshot);s->snapshot=0;}}
static int read_bus(void *context,uint32_t a,unsigned n,uint32_t *out){struct sa_stream_pixel_binding *b=context;return b->parent.read?b->parent.read(b->parent.context,a,n,out):-2;}
static int write_bus(void *context,uint32_t a,uint32_t value,unsigned n){
 struct sa_stream_pixel_binding *b=context;struct sa_stream_pixel_job *s=b->state;
 uint32_t address=a&0x1fffffffu,count=sa_ta_stream_count(b->stream,0);
 int capture=address==0x5f8014u&&value==0xffffffffu&&b->render->enabled;
 struct sa_ta_stream *candidate=0;
 if(capture){
  if(n!=4u)return -3;if(s->ready)return -4;
  if(b->render->submitted==UINT64_MAX||s->cursor>count)return -6;
  int rc=sa_ta_stream_clone_range(b->stream,s->cursor,&candidate);if(rc)return rc;
 }
 if(!b->parent.write){sa_ta_stream_destroy(candidate);return -2;}
 int rc=b->parent.write(b->parent.context,a,value,n);
 if(rc){sa_ta_stream_destroy(candidate);return rc;}
 if(capture){
  /* Commit the independent owned copy only after the real bus write succeeds. */
  sa_ta_stream_destroy(s->snapshot);s->snapshot=candidate;s->count=count-s->cursor;
  s->job_id=b->render->submitted+1u;s->cursor=count;s->ready=1;
  rc=sa_ta_stream_set_cursor(b->stream,count);if(rc)return rc;
 }
 if(address==0x5f8144u&&n==4u&&(value&0x80000000u)){
  uint64_t life[8];rc=sa_ta_stream_lifecycle(b->stream,life);if(rc)return rc;if(life[7])s->cursor=0;
 }
 if(address==0x5f8008u&&n==4u&&(value&2u))s->ready=0;
 return 0;
}
static int cache_bus(void *context,uint32_t a,unsigned op){struct sa_stream_pixel_binding *b=context;return b->parent.cache?b->parent.cache(b->parent.context,a,op):-2;}
SA_STAGE_EXPORT int sa_stream_pixel_job_bind(struct sa_stream_pixel_binding *b,struct sa_stream_pixel_job *s,struct sa_render_completion *render,struct sa_ta_stream *stream,const struct sa_vertex_bus *parent,struct sa_vertex_bus *out){
 if(!b||!s||!render||!stream||!parent||!out)return -6;
 if(s->count!=sa_ta_stream_count(s->snapshot,0)||s->cursor>sa_ta_stream_count(stream,0))return -6;
 b->state=s;b->render=render;b->stream=stream;b->parent=*parent;*out=(struct sa_vertex_bus){b,read_bus,write_bus,cache_bus};return 0;
}
SA_STAGE_EXPORT int sa_stream_pixel_job_execute(struct sa_stream_pixel_job *s,struct sa_render_completion *r,const uint8_t *vram,uint32_t size,uint32_t *pixels,uint32_t *depth,uint64_t capacity,const struct sa_pixel_options *o){
 if(!s||!r)return -6;if(!s->ready||!r->enabled)return 0;
 if(!s->job_id||s->job_id!=r->active_id||s->count!=sa_ta_stream_count(s->snapshot,0)||s->processed==UINT64_MAX||r->completed==UINT64_MAX)return -6;
 if(!o||!pixels||!depth)return -6;
 if(!o->width||!o->height||o->width>8192u||o->height>8192u||o->textures>1u||o->alpha_ref>255u||capacity<(uint64_t)o->width*o->height||size>0x800000u)return -3;
 if((r->job[7]&0xffffu)||(r->job[8]&0xffffu)||o->input_width!=((r->job[7]>>16)&1023u)+1u||o->input_height!=((r->job[8]>>16)&1023u)+1u)return -2;
 int rc=0;uint32_t *words=0,count=0;
 if(s->count){
  rc=sa_ta_stream_flatten(s->snapshot,&words,&count);if(rc)return rc;
  rc=sa_pixel_draw(words,count,vram,size,pixels,depth,capacity,o,&s->stats);
  sa_ta_stream_release_flat(s->snapshot,words);
 }else{volatile uint8_t *p=(volatile uint8_t *)&s->stats;for(unsigned i=0;i<sizeof(s->stats);i++)p[i]=0;}
 if(rc)return rc;
 /* A completion event is emitted only after the complete native raster pass. */
 rc=sa_render_completion_finish(r,s->job_id);if(rc)return rc;
 s->processed++;s->last_job=s->job_id;s->ready=0;return 0;
}
SA_STAGE_EXPORT int sa_stream_banked_pixel_job_execute(struct sa_stream_pixel_job *s,struct sa_render_completion *r,const uint8_t *banks,uint32_t size,uint8_t *view,uint32_t capacity,uint32_t *pixels,uint32_t *depth,uint64_t pixel_capacity,const struct sa_pixel_options *o){
 if(!s||!r)return -6;if(!s->ready||!r->enabled)return 0;if(!o)return -6;if(o->textures>1u)return -3;
 if(!o->textures)return sa_stream_pixel_job_execute(s,r,0,0,pixels,depth,pixel_capacity,o);
 int rc=sa_pixel_vram64_view(banks,size,view,capacity);if(rc)return rc;
 return sa_stream_pixel_job_execute(s,r,view,size,pixels,depth,pixel_capacity,o);
}
