#include "fpu_cleanup_transfer.h"
struct sa_selector_cpu {uint32_t r[16],sr,pr,pc;};
typedef int (*service)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
typedef void (*observer)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
#define DECL(name) int name(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*)
DECL(sa_overlay_scale_spring_0c905618);
DECL(sa_overlay_parameter_update_0c905666);
DECL(sa_overlay_model_blend_0c904e80);
DECL(sa_word_copy_dispatch_8c10cddc);
struct context {void *user;observer observe;uint32_t *fpul,*macl,stop;};
static int dispatch(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *ctx=(struct context*)opaque;ctx->stop=target;
 if(ctx->observe)ctx->observe(ctx->user,target,m,f,sr,r,pr);
 if(target==0x0c905618u)return sa_overlay_scale_spring_0c905618(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch,&ctx->stop);
 if(target==0x0c905666u)return sa_overlay_parameter_update_0c905666(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch,&ctx->stop);
 if(target==0x0c904e80u)return sa_overlay_model_blend_0c904e80(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch,&ctx->stop);
 if(target==0x8c10cddcu)return sa_word_copy_dispatch_8c10cddc(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch,&ctx->stop);
 return -2;
}
SA_STAGE_EXPORT int sa_overlay_parameter_update_connected(uint8_t *m,struct sa_selector_cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer observe){
 struct context ctx={user,observe,fpul,macl,entry};
 int status=dispatch(&ctx,entry,m,f,&s->sr,s->r,&s->pr);
 s->pc=status?ctx.stop:s->pr;return status;
}
