#include "banked_pixel_jobs.h"
SA_STAGE_EXPORT int sa_pixel_vram64_view(const uint8_t *banks,uint32_t size,uint8_t *view,uint32_t capacity){
 const uint32_t bank_size=0x400000u,total=2u*bank_size;
 if(!banks||!view)return -6;
 if(size!=total||capacity<total)return -3;
 uintptr_t a=(uintptr_t)banks,b=(uintptr_t)view;
 if(a<=b?b-a<total:a-b<total)return -3;
 for(uint32_t i=0;i<bank_size;i+=4u)for(uint32_t lane=0;lane<4u;lane++){
  view[2u*i+lane]=banks[i+lane];
  view[2u*i+4u+lane]=banks[bank_size+i+lane];
 }
 return 0;
}
SA_STAGE_EXPORT int sa_banked_pixel_job_execute(struct sa_pixel_job_state *s,struct sa_render_completion *r,const uint8_t *banks,uint32_t size,uint8_t *view,uint32_t capacity,uint32_t *pixels,uint32_t *depth,uint64_t pixel_capacity,const struct sa_pixel_options *o){
 if(!s||!r)return -6;
 if(!s->ready||!r->enabled)return 0;
 if(!o)return -6;
 if(o->textures>1u)return -3;
 /* Off neither reads VRAM nor needs a conversion workspace. */
 if(!o->textures)return sa_pixel_job_execute(s,r,0,0,pixels,depth,pixel_capacity,o);
 int rc=sa_pixel_vram64_view(banks,size,view,capacity);if(rc)return rc;
 return sa_pixel_job_execute(s,r,view,size,pixels,depth,pixel_capacity,o);
}
