#include "fpu_cleanup_transfer.h"
typedef uint32_t (*sa_vector_service)(void *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
typedef int (*sa_vector_instruction)(void *,uint32_t,struct sa_fpu_banks *);
int sa_model_attachment_route(uint32_t,uint32_t,uint32_t,uint32_t *,uint32_t,void *,sa_vector_service,uint32_t *);
int sa_model_matrix_stack_pop_8c639b98(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_point_transform(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,void *,sa_vector_instruction);
int sa_model_direction_transform(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,void *,sa_vector_instruction);
struct transform_context{uint8_t *ram;struct sa_fpu_banks *fpu;int error;void *opaque;sa_vector_instruction vector;};
static uint32_t invoke(void *opaque,uint32_t address,uint32_t argc,uint32_t a,uint32_t b,uint32_t c,uint32_t incoming,uint32_t *sr){
 struct transform_context *ctx=opaque;uint32_t r[16]={0};(void)argc;r[0]=incoming;r[4]=a;r[5]=b;r[6]=c;
 /* Preserve the first unsupported instruction/trap and skip later services. */
 if(ctx->error)return incoming;
 if(address==0x8c639b98u)ctx->error=sa_model_matrix_stack_pop_8c639b98(ctx->ram,ctx->fpu,sr,r);
 else if(address==0x8c638eccu)ctx->error=sa_model_point_transform(ctx->ram,ctx->fpu,sr,r,ctx->opaque,ctx->vector);
 else if(address==0x8c638f28u)ctx->error=sa_model_direction_transform(ctx->ram,ctx->fpu,sr,r,ctx->opaque,ctx->vector);
 else ctx->error=-1;
 return r[0];
}
SA_STAGE_EXPORT int sa_model_attachment_with_transforms(uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t type,uint32_t entity,uint32_t frame,uint32_t *sr,uint32_t incoming,void *opaque,sa_vector_instruction vector,uint32_t *result){
 struct transform_context ctx={ram,fpu,0,opaque,vector};int status=sa_model_attachment_route(type,entity,frame,sr,incoming,&ctx,invoke,result);return ctx.error?ctx.error:status;
}
