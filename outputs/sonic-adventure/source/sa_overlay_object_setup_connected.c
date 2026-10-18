#include "fpu_cleanup_transfer.h"
#include "byte_copy_forward.h"
struct sa_selector_cpu {uint32_t r[16],sr,pr,pc;};
typedef int (*service)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
typedef void (*observer)(void*,uint32_t,uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*);
#define DECL(name) int name(uint8_t*,struct sa_fpu_banks*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,uint32_t*,void*,service)
DECL(sa_overlay_object_preorder_walk_0c9044a0);
DECL(sa_overlay_object_root_walk_0c9044e2);
DECL(sa_overlay_object_clone_0c90451c);
DECL(sa_overlay_object_setup_0c904d22);
DECL(sa_heap_allocate);
DECL(sa_heap_release);
struct context {void *user;service external;observer observe;uint32_t stop;};
static int dispatch(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *ctx=(struct context*)opaque;ctx->stop=target;
 if(ctx->observe)ctx->observe(ctx->user,target,m,f,sr,r,pr);
 uint32_t fpul=0,macl=0,source=r[4];int status;
 switch(target){
 case 0x0c9044a0u:
  status=sa_overlay_object_preorder_walk_0c9044a0(m,f,sr,r,pr,&fpul,&macl,ctx,dispatch);
  if(status==-3&&(source&0x1f000000u)!=0x0c000000u)ctx->stop=0x0c9044b4u;
  return status;
 case 0x0c9044e2u:
  status=sa_overlay_object_root_walk_0c9044e2(m,f,sr,r,pr,&fpul,&macl,ctx,dispatch);
  if(status==-3&&(source&0x1f000000u)!=0x0c000000u)ctx->stop=0x0c9044f2u;
  return status;
 case 0x0c90451cu:
  status=sa_overlay_object_clone_0c90451c(m,f,sr,r,pr,&fpul,&macl,ctx,dispatch);
  if(status==-3&&(source&0x1f000000u)!=0x0c000000u)ctx->stop=0x0c904534u;
  return status;
 case 0x0c904d22u:
  status=sa_overlay_object_setup_0c904d22(m,f,sr,r,pr,&fpul,&macl,ctx,dispatch);
  if(status==-1)ctx->stop=0x0c904d5eu;
  return status;
 case 0x8c098a80u:return sa_heap_allocate(m,f,sr,r,pr,&fpul,&macl,ctx,dispatch);
 case 0x8c098b42u:return sa_heap_release(m,f,sr,r,pr,&fpul,&macl,ctx,dispatch);
 case 0x8c604500u:{
  /* Reuse the verified forward copy; publish all seven original instructions'
   * register effects. Zero count is outside this bounded clone contract. */
  uint32_t length=r[6];if(!length)return -3;
  sa_byte_copy_forward_8c604500(m,r[4],r[5],length);
  r[0]=(uint32_t)(int32_t)(int8_t)m[(r[5]+length-1u)&0xffffffu];
  r[4]+=length;r[5]+=length;r[6]=0;*sr|=1u;return 0;
 }
 default:if(!ctx->external)return -2;return ctx->external(ctx->user,target,m,f,sr,r,pr);
 }
}
SA_STAGE_EXPORT int sa_overlay_object_setup_connected(uint8_t *m,struct sa_selector_cpu *s,struct sa_fpu_banks *f,uint32_t entry,void *user,service external,observer observe){
 struct context ctx={user,external,observe,entry};
 /* The observer includes the public entry and every nested call. */
 int status=dispatch(&ctx,entry,m,f,&s->sr,s->r,&s->pr);
 s->pc=status?ctx.stop:s->pr;return status;
}
