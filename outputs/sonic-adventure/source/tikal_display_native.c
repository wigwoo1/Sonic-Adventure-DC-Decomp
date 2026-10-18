#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
typedef int (*vector)(void *,uint32_t,struct sa_fpu_banks *);
int tikal_display(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
int sa_model_matrix_stack_push_8c639c70(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_matrix_stack_pop_8c639b98(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_matrix_translate(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,void *,vector);
struct context {void *user;service external;vector instruction;};
static int call(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;
 if(target==0x8c639c70u)return sa_model_matrix_stack_push_8c639c70(m,f,sr,r);
 if(target==0x8c639b98u)return sa_model_matrix_stack_pop_8c639b98(m,f,sr,r);
 if(target==0x8c63a804u)return sa_model_matrix_translate(m,f,sr,r,c->user,c->instruction);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_tikal_display_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,service external,vector instruction){
 struct context c={user,external,instruction};return tikal_display(m,f,sr,r,pr,fpul,macl,&c,call);
}
