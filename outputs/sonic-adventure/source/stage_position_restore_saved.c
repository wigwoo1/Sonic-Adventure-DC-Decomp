#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service external
int sa_stage_position_restore(ARGS);
int sa_saved_position_available(ARGS);
int sa_stage_act_key(ARGS);
int sa_saved_position_reset(ARGS);
int sa_saved_position_load(ARGS);
int sa_saved_position_store(ARGS);
int sa_timer_set(ARGS);
int sa_timer_get(ARGS);
int sa_counter_get(ARGS);
int sa_counter_add(ARGS);
int sa_counter_award(ARGS);
int sa_signed_division(ARGS);
int sa_stage_mode_query(ARGS);
int sa_position_task_create(ARGS);
struct context {uint32_t *fpul,*macl;void *user;service external;};
static int call(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;
 if(target==0x8c064e92u)return sa_saved_position_available(m,f,sr,r,pr,c->fpul,c->macl,c,call);
 if(target==0x8c064eb2u)return sa_saved_position_load(m,f,sr,r,pr,c->fpul,c->macl,c,call);
 if(target==0x8c064ef6u)return sa_saved_position_store(m,f,sr,r,pr,c->fpul,c->macl,c,call);
 if(target==0x8c04f7e0u)return sa_stage_act_key(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c064e80u)return sa_saved_position_reset(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c08ab20u)return sa_timer_set(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c08ab2eu)return sa_timer_get(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c08a2bcu)return sa_counter_get(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c08a198u)return sa_counter_add(m,f,sr,r,pr,c->fpul,c->macl,c,call);
 if(target==0x8c04c960u)return sa_stage_mode_query(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c08a120u)return sa_counter_award(m,f,sr,r,pr,c->fpul,c->macl,c,call);
 if(target==0x8c10ca5cu)return sa_signed_division(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c07d5aau)return sa_position_task_create(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_stage_position_restore_saved(ARGS){
 struct context c={fpul,macl,ctx,external};return sa_stage_position_restore(m,f,sr,r,pr,fpul,macl,&c,call);
}

SA_STAGE_EXPORT int sa_counter_add_native(ARGS){
 struct context c={fpul,macl,ctx,external};return sa_counter_add(m,f,sr,r,pr,fpul,macl,&c,call);
}
