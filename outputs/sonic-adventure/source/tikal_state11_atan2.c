#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service external
int tikal_state11(ARGS);
int sa_atan2_full_native(ARGS);
typedef int (*instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
int sa_angle_trig(struct sa_fpu_banks *,uint32_t *,uint32_t *,void *,instruction,int);
typedef int (*vector)(void *,uint32_t,struct sa_fpu_banks *);
int sa_scalar_signed_sqrt(struct sa_fpu_banks *,uint32_t *,void *,vector);
int sa_stage_position_restore_saved(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
struct context {instruction execute;uint32_t *fpul,*macl;void *user;service external;};
static int scalar(void *opaque,uint32_t word,struct sa_fpu_banks *f){
 struct context *c=opaque;return c->execute(c->user,word,f,*c->fpul);
}
static int call(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;
 if(target==0x8c10d0f8u)return sa_atan2_full_native(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c63a970u||target==0x8c63a9b8u)return sa_angle_trig(f,r,c->fpul,c->user,c->execute,target==0x8c63a970u);
 if(target==0x8c63a9c4u)return sa_scalar_signed_sqrt(f,sr,c,scalar);
 if(target==0x8c04f8fau)return sa_stage_position_restore_saved(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_tikal_state11_atan2(ARGS,instruction execute){
 struct context c={execute,fpul,macl,ctx,external};return tikal_state11(m,f,sr,r,pr,fpul,macl,&c,call);
}
