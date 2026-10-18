#include "fpu_cleanup_transfer.h"
typedef int (*sa_vector_instruction)(void *,uint32_t,struct sa_fpu_banks *);
typedef int (*sa_render_vector_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t *);
typedef uint32_t (*sa_vector_service)(void *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
int sa_model_render_record_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t *,uint32_t *);
uint32_t sa_model_vector_render_select(uint32_t,uint32_t *,uint32_t *);
int sa_model_vector_type1_with_transforms(uint8_t *,struct sa_fpu_banks *,uint32_t,uint32_t,uint32_t *,void *,sa_vector_instruction,uint32_t *);
int sa_model_vector_type2_with_transforms(uint8_t *,struct sa_fpu_banks *,uint32_t,uint32_t,uint32_t *,void *,sa_vector_instruction,sa_vector_service,uint32_t *);
int sa_model_vector_type3_with_transforms(uint8_t *,struct sa_fpu_banks *,uint32_t,uint32_t,uint32_t *,void *,sa_vector_instruction,sa_vector_service,uint32_t *);
int sa_model_attachment_with_transforms(uint8_t *,struct sa_fpu_banks *,uint32_t,uint32_t,uint32_t,uint32_t *,uint32_t,void *,sa_vector_instruction,uint32_t *);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
struct vector_context{void *opaque;sa_render_vector_instruction vector;uint32_t *fpul;};
static int invoke_vector(void *opaque,uint32_t word,struct sa_fpu_banks *f){struct vector_context *ctx=opaque;return ctx->vector(ctx->opaque,word,f,ctx->fpul);}
/* Original record loop8c07a800..8c07adf6, entered with nonempty record list
 * and prepared frame. Native setup/branch bodies; vector numerical services
 * still explicit. Hierarchy preparation/full helper ABI remain pending. */
SA_STAGE_EXPORT int sa_model_render_record_loop(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *sr,uint32_t config,uint32_t record,uint32_t frame,uint32_t entity,void *ctx,sa_render_vector_instruction vector,uint32_t *result,uint32_t *end_record){
 struct vector_context vc={ctx,vector,fpul};
 for(;;){
  uint32_t next;int status=sa_model_render_record_native(ram,f,fpul,sr,config,record,frame,result,&next);if(status)return status;
  if(next==0x8c07a93cu){
   uint32_t type=ram[(record&0xffffffu)+13u];next=sa_model_vector_render_select(type,sr,result);
   if(next!=0x8c07ade6u){
    if(type==1u)status=sa_model_vector_type1_with_transforms(ram,f,record,frame,sr,&vc,invoke_vector,result);
    else if(type==2u)status=sa_model_vector_type2_with_transforms(ram,f,record,frame,sr,&vc,invoke_vector,0,result);
    else if(type==3u)status=sa_model_vector_type3_with_transforms(ram,f,record,frame,sr,&vc,invoke_vector,0,result);
    else status=sa_model_attachment_with_transforms(ram,f,type,entity,frame,sr,*result,&vc,invoke_vector,result);
    if(status)return status;
   }
  }
  record+=24u;*sr=(*sr&~1u)|(rd(ram,record)==0u);if(*sr&1u){*end_record=record;return 0;}
 }
}
