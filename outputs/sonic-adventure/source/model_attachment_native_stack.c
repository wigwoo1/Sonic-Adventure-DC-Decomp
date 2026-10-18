#include "fpu_cleanup_transfer.h"
typedef uint32_t (*sa_vector_service)(void *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
int sa_model_attachment_route(uint32_t,uint32_t,uint32_t,uint32_t *,uint32_t,void *,sa_vector_service,uint32_t *);
int sa_model_matrix_stack_pop_8c639b98(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
struct route_context{uint8_t *ram;struct sa_fpu_banks *fpu;int error;void *opaque;sa_vector_service service;};
static uint32_t invoke(void *opaque,uint32_t address,uint32_t argc,uint32_t a,uint32_t b,uint32_t c,uint32_t incoming,uint32_t *sr){
 struct route_context *ctx=opaque;
 if(address==0x8c639b98u){uint32_t r[16]={0};r[0]=incoming;r[4]=a;ctx->error=sa_model_matrix_stack_pop_8c639b98(ctx->ram,ctx->fpu,sr,r);return r[0];}
 return ctx->service(ctx->opaque,address,argc,a,b,c,incoming,sr);
}
SA_STAGE_EXPORT int sa_model_attachment_with_native_stack(uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t type,uint32_t entity,uint32_t frame,uint32_t *sr,uint32_t incoming,void *opaque,sa_vector_service service,uint32_t *result){
 struct route_context ctx={ram,fpu,0,opaque,service};int status=sa_model_attachment_route(type,entity,frame,sr,incoming,&ctx,invoke,result);return ctx.error?ctx.error:status;
}
