#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
typedef int (*vector)(void *,uint32_t,struct sa_fpu_banks *);
int tikal_position(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
int sa_model_matrix_stack_push_8c639c70(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_matrix_stack_pop_8c639b98(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_direction_transform(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,void *,vector);
struct context {void *user;service external;vector instruction;uint32_t *fpul;};
typedef int (*rotation_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
int sa_matrix_rotate_x(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,void *,rotation_instruction);
int sa_matrix_rotate_y(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,void *,rotation_instruction);
int sa_matrix_rotate_z(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,void *,rotation_instruction);
static int rotation(void *opaque,uint32_t word,struct sa_fpu_banks *f,uint32_t fpul){
 (void)fpul;struct context *c=opaque;return c->instruction(c->user,word,f);
}

static int call(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;
 if(target==0x8c639ec8u)return sa_matrix_rotate_x(m,f,sr,r,c->fpul,c,rotation);
 if(target==0x8c639f5cu)return sa_matrix_rotate_y(m,f,sr,r,c->fpul,c,rotation);
 if(target==0x8c63a1ccu)return sa_matrix_rotate_z(m,f,sr,r,c->fpul,c,rotation);

 if(target==0x8c639c70u)return sa_model_matrix_stack_push_8c639c70(m,f,sr,r);
 if(target==0x8c639b98u)return sa_model_matrix_stack_pop_8c639b98(m,f,sr,r);
 if(target==0x8c638f28u)return sa_model_direction_transform(m,f,sr,r,c->user,c->instruction);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_tikal_position_rotations(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,service external,vector instruction){
 struct context c={user,external,instruction,fpul};return tikal_position(m,f,sr,r,pr,fpul,macl,&c,call);
}
