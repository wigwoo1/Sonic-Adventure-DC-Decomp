#include "fpu_cleanup_transfer.h"
struct sa_selector_cpu {uint32_t r[16],sr,pr,pc;};
typedef int (*service)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
typedef void (*observer)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
int sa_overlay_selector_initializer_0c90fde4(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service);
int sa_overlay_selector_guard(uint8_t*,struct sa_selector_cpu*,struct sa_fpu_banks*);
struct sa_cpu_state {uint32_t r[16],sr,pr,pc;};
void sa_return_noop_0c90d210(uint8_t*,struct sa_cpu_state*);
int sa_overlay_entity_marker_reset_0c909d40(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service);
int sa_overlay_object_setup_connected(uint8_t*,struct sa_selector_cpu*,struct sa_fpu_banks*,uint32_t,void*,service,observer);
int sa_overlay_object_variant_connected(uint8_t*,struct sa_selector_cpu*,struct sa_fpu_banks*,uint32_t);
struct context {void *user;service external;observer observe;uint32_t stop;unsigned skip_object_entry;};
static int object_external(void *opaque,uint32_t target,uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *ctx=(struct context*)opaque;
 if(target==0x0c904aeau||target==0x0c904bbcu||target==0x0c904c80u){
  struct sa_selector_cpu state;for(unsigned i=0;i<16;i++)state.r[i]=r[i];state.sr=*sr;state.pr=*pr;state.pc=target;
  int status=sa_overlay_object_variant_connected(ram,&state,f,target);
  for(unsigned i=0;i<16;i++)r[i]=state.r[i];*sr=state.sr;*pr=state.pr;
  if(status)ctx->stop=state.pc;return status;
 }
 if(!ctx->external)return -2;
 return ctx->external(ctx->user,target,ram,f,sr,r,pr);
}
static void object_observe(void *opaque,uint32_t target,uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *ctx=(struct context*)opaque;
 /* The outer dispatcher already observed this one entry. Nested recursion,
  * heap/copy calls and remaining variants are observed exactly once here. */
 if(ctx->skip_object_entry){ctx->skip_object_entry=0;return;}
 if(ctx->observe)ctx->observe(ctx->user,target,ram,f,sr,r,pr);
}
static int dispatch(void *opaque,uint32_t target,uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *ctx=(struct context*)opaque;ctx->stop=target;
 if(ctx->observe)ctx->observe(ctx->user,target,ram,f,sr,r,pr);
 if(target==0x0c904d22u){
  struct sa_selector_cpu state;for(unsigned i=0;i<16;i++)state.r[i]=r[i];state.sr=*sr;state.pr=*pr;state.pc=target;
  ctx->skip_object_entry=1;
  int status=sa_overlay_object_setup_connected(ram,&state,f,target,ctx,object_external,object_observe);
  for(unsigned i=0;i<16;i++)r[i]=state.r[i];*sr=state.sr;*pr=state.pr;
  if(status)ctx->stop=state.pc;
  return status;
 }
 if(target==0x0c909d40u){uint32_t fpul=0,macl=0;return sa_overlay_entity_marker_reset_0c909d40(ram,f,sr,r,pr,&fpul,&macl,ctx,dispatch);}
 if(target==0x0c90d210u){
  struct sa_cpu_state state;for(unsigned i=0;i<16;i++)state.r[i]=r[i];state.sr=*sr;state.pr=*pr;state.pc=target;
  sa_return_noop_0c90d210(ram,&state);
  for(unsigned i=0;i<16;i++)r[i]=state.r[i];*sr=state.sr;*pr=state.pr;return 0;
 }
 if(target==0x0c905548u){
  struct sa_selector_cpu state;for(unsigned i=0;i<16;i++)state.r[i]=r[i];state.sr=*sr;state.pr=*pr;state.pc=target;
  int status=sa_overlay_selector_guard(ram,&state,f);
  for(unsigned i=0;i<16;i++)r[i]=state.r[i];*sr=state.sr;*pr=state.pr;
  if(status)ctx->stop=state.pc;return status;
 }
 if(!ctx->external)return -2;
 return ctx->external(ctx->user,target,ram,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_overlay_object_initializer_connected(uint8_t *ram,struct sa_selector_cpu *s,struct sa_fpu_banks *f,void *user,service external,observer observe){
 struct context ctx={user,external,observe,0,0};uint32_t fpul=0,macl=0;
 int status=sa_overlay_selector_initializer_0c90fde4(ram,f,&s->sr,s->r,&s->pr,&fpul,&macl,&ctx,dispatch);
 s->pc=status?ctx.stop:s->pr;return status;
}
