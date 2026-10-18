#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
typedef int (*vector)(void *,uint32_t,struct sa_fpu_banks *);
int sa_tikal_controller_display_rotations(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service,vector);
int tikal_effect_create(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
int sa_atan2_full_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
typedef int (*trig_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
int sa_angle_trig(struct sa_fpu_banks *,uint32_t *,uint32_t *,void *,trig_instruction,int);
int sa_scalar_signed_sqrt(struct sa_fpu_banks *,uint32_t *,void *,vector);
int sa_stage_position_restore_saved(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
struct context {vector instruction;void *user;service external;uint32_t *fpul,*macl;};
static int trig(void *opaque,uint32_t word,struct sa_fpu_banks *f,uint32_t fpul){
 (void)fpul;struct context *c=opaque;return c->instruction(c->user,word,f);
}
static int call(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;if(target==0x0cb00f60u)return tikal_effect_create(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c10d0f8u)return sa_atan2_full_native(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c63a970u||target==0x8c63a9b8u)return sa_angle_trig(f,r,c->fpul,c,trig,target==0x8c63a970u);
 if(target==0x8c63a9c4u)return sa_scalar_signed_sqrt(f,sr,c->user,c->instruction);
 if(target==0x8c04f8fau)return sa_stage_position_restore_saved(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_tikal_controller_rotations(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,service external,vector instruction){
 struct context c={instruction,user,external,fpul,macl};return sa_tikal_controller_display_rotations(m,f,sr,r,pr,fpul,macl,&c,call,instruction);
}
