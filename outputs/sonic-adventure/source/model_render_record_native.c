#include "fpu_cleanup_transfer.h"
typedef uint32_t (*sa_vector_service)(void *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
int sa_model_render_record_setup(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t,uint32_t,uint32_t,void *,sa_vector_service,uint32_t *,uint32_t *);
int sa_model_matrix_inverse(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t);
struct context{uint8_t *ram;struct sa_fpu_banks *f;uint32_t *fpul;uint32_t record,frame;int error;};
static uint32_t invert(void *opaque,uint32_t addr,uint32_t argc,uint32_t a,uint32_t b,uint32_t c,uint32_t incoming,uint32_t *sr){
 struct context *ctx=opaque;(void)b;(void)c;if(ctx->error)return incoming;
 if(addr!=0x8c6390b0u||argc!=1u){ctx->error=-1;return incoming;}
 uint32_t r[16]={0};r[0]=incoming;r[4]=a;r[14]=ctx->record;r[15]=ctx->frame;
 ctx->error=sa_model_matrix_inverse(ctx->ram,ctx->f,sr,r,ctx->fpul,incoming==80u?0x8c07a866u:0x8c07a8acu);return r[0];
}
/* Original per-record setup connected without game-function services.
 * FPUL exposed separately from shared FPU prefix; supported finite scope. */
SA_STAGE_EXPORT int sa_model_render_record_native(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *sr,uint32_t config,uint32_t record,uint32_t frame,uint32_t *result,uint32_t *next){
 struct context ctx={ram,f,fpul,record,frame,0};int status=sa_model_render_record_setup(ram,f,sr,config,record,frame,&ctx,invert,result,next);return ctx.error?ctx.error:status;
}
