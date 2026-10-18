#include "fpu_cleanup_transfer.h"
typedef uint32_t (*sa_vector_service)(void *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
int sa_model_render_record_cache(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t *,uint32_t *);
int sa_model_hierarchy_matrix(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_matrix_copy(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_render_single_mesh(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
uint32_t sa_model_descriptor_lookup_8c056712(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* Per-record setup8c07a800..8c07a93c, meaningful RAM/FPU/r0/SR/branch.
 * Native cache/getter/copy/fallback;8c6390b0 remains explicit service. */
SA_STAGE_EXPORT int sa_model_render_record_setup(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t config,uint32_t record,uint32_t frame,void *ctx,sa_vector_service service,uint32_t *result,uint32_t *next){
 if(f->fpscr&0x180000u)return -1;
 int status=sa_model_render_record_cache(ram,sr,config,record,frame,result,next);if(status||*next!=0x8c07a832u)return status;
 uint32_t r[16]={0};r[4]=at(ram,record)[14];r[5]=rd(ram,frame+88u);r[15]=frame;
 status=sa_model_hierarchy_matrix(ram,f,sr,r);if(status)return status;
 uint32_t second=rd(ram,record+8u);*sr=(*sr&~1u)|(second==0);
 if(!second){r[14]=record;status=sa_model_render_single_mesh(ram,f,sr,r);*result=r[0];*next=0x8c07a93cu;return status;}
 uint32_t first=rd(ram,record+4u);wr(ram,frame+76u,first);uint32_t metadata=rd(ram,first+4u);wr(ram,frame+32u,rd(ram,metadata));wr(ram,frame+20u,rd(ram,metadata+4u));
 r[0]=104u;r[4]=0x8c798718u;r[5]=rd(ram,frame+104u);status=sa_model_matrix_copy(ram,f,sr,r);if(status)return status;
 *result=service(ctx,0x8c6390b0u,1u,rd(ram,frame+80u),0,0,80u,sr);
 uint32_t cached=at(ram,record)[15];*sr=(*sr&~1u)|(cached==255u);
 if(cached==255u){*result=sa_model_descriptor_lookup_8c056712(ram,sr,rd(ram,record),rd(ram,record+8u),frame);at(ram,record)[15]=(uint8_t)*result;*next=0x8c07ade6u;return 0;}
 r[4]=cached;r[5]=rd(ram,frame+116u);status=sa_model_hierarchy_matrix(ram,f,sr,r);if(status)return status;
 second=rd(ram,record+8u);wr(ram,frame+72u,second);metadata=rd(ram,second+4u);wr(ram,frame+28u,rd(ram,metadata));wr(ram,frame+24u,rd(ram,metadata+4u));
 r[0]=92u;r[4]=0x8c798758u;r[5]=rd(ram,frame+92u);status=sa_model_matrix_copy(ram,f,sr,r);if(status)return status;
 *result=service(ctx,0x8c6390b0u,1u,rd(ram,frame+96u),0,0,96u,sr);*next=0x8c07a93cu;return 0;
}
