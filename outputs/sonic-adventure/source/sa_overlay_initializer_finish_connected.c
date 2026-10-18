#include "fpu_cleanup_transfer.h"
struct sa_selector_cpu {uint32_t r[16],sr,pr,pc;};
struct sa_cpu_state {uint32_t r[16],sr,pr,pc;};
typedef int (*service)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
typedef void (*observer)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
#define DECL(n) int n(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service,uint32_t*)
DECL(sa_overlay_animation_start_0c90bd40);
DECL(sa_overlay_appearance_select_0c90853e);
DECL(sa_overlay_accessory_install_0c9048ec);
DECL(sa_overlay_tail_palette_0c90fb82);
DECL(sa_overlay_tail_material_158_0c90fca0);
DECL(sa_overlay_tail_material_161_0c90fd42);
DECL(sa_overlay_appearance_mode_0c9084e6);
DECL(sa_overlay_appearance_pair_0c9084fc);
DECL(sa_overlay_appearance_links_0c908530);
void sa_random_u15_8c10d334(uint8_t*,struct sa_cpu_state*,uint32_t*);
struct context {void *user;observer observe;uint32_t *fpul,*macl,stop;};
static int dispatch(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *ctx=(struct context*)opaque;ctx->stop=target;
 if(ctx->observe)ctx->observe(ctx->user,target,m,f,sr,r,pr);
 switch(target){
#define ROUTE(a,n) case a:return n(m,f,sr,r,pr,ctx->fpul,ctx->macl,ctx,dispatch,&ctx->stop)
 ROUTE(0x0c90bd40u,sa_overlay_animation_start_0c90bd40);
 ROUTE(0x0c90853eu,sa_overlay_appearance_select_0c90853e);
 ROUTE(0x0c9048ecu,sa_overlay_accessory_install_0c9048ec);
 ROUTE(0x0c90fb82u,sa_overlay_tail_palette_0c90fb82);
 ROUTE(0x0c90fca0u,sa_overlay_tail_material_158_0c90fca0);
 ROUTE(0x0c90fd42u,sa_overlay_tail_material_161_0c90fd42);
 ROUTE(0x0c9084e6u,sa_overlay_appearance_mode_0c9084e6);
 ROUTE(0x0c9084fcu,sa_overlay_appearance_pair_0c9084fc);
 ROUTE(0x0c908530u,sa_overlay_appearance_links_0c908530);
#undef ROUTE
 case 0x8c10d334u:{
  struct sa_cpu_state s;for(unsigned i=0;i<16;i++)s.r[i]=r[i];s.sr=*sr;s.pr=*pr;s.pc=target;
  sa_random_u15_8c10d334(m,&s,ctx->macl);
  for(unsigned i=0;i<16;i++)r[i]=s.r[i];*sr=s.sr;*pr=s.pr;return 0;
 }
 default:return -2;
 }
}
SA_STAGE_EXPORT int sa_overlay_initializer_finish_connected(uint8_t *m,struct sa_selector_cpu *s,struct sa_fpu_banks *f,uint32_t *fpul,uint32_t *macl,uint32_t entry,void *user,observer observe){
 struct context ctx={user,observe,fpul,macl,entry};
 int status=dispatch(&ctx,entry,m,f,&s->sr,s->r,&s->pr);
 s->pc=status?ctx.stop:s->pr;return status;
}
