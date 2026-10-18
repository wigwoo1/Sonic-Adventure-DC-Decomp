#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
typedef int (*vector)(void *,uint32_t,struct sa_fpu_banks *);
int tikal_effect_update(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
uint32_t sa_model_channel_difference_8c055d4e(uint32_t *);
int sa_model_matrix_stack_push_8c639c70(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_matrix_stack_pop_8c639b98(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_direction_transform(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,void *,vector);
int sa_effect_primary_angles_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
int sa_effect_secondary_angles_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
int sa_vector_add(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
int sa_vector_subtract(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
int sa_vector_to_angles(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
int sa_camera_position_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service,vector);
int sa_scalar_signed_sqrt(struct sa_fpu_banks *,uint32_t *,void *,vector);
int sa_atan2_full_native(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service);
struct context {void *user;service external;vector instruction;uint32_t *fpul,*macl;};
typedef int (*rotation_instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
int sa_matrix_rotate_x(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,void *,rotation_instruction);
int sa_matrix_rotate_y(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,void *,rotation_instruction);
int sa_matrix_rotate_z(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,void *,rotation_instruction);
static int rotation(void *opaque,uint32_t word,struct sa_fpu_banks *f,uint32_t fpul){
 (void)fpul;struct context *c=opaque;return c->instruction(c->user,word,f);
}
static int call(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;
 if(target==0x8c10d0f8u)return sa_atan2_full_native(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c63a9c4u)return sa_scalar_signed_sqrt(f,sr,c->user,c->instruction);
 if(target==0x8c078eb0u)return sa_camera_position_native(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external,c->instruction);
 if(target==0x8c056860u)return sa_vector_to_angles(m,f,sr,r,pr,c->fpul,c->macl,c,call);
 if(target==0x8c638eacu)return sa_vector_add(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c63a7e4u)return sa_vector_subtract(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c639ec8u)return sa_matrix_rotate_x(m,f,sr,r,c->fpul,c,rotation);
 if(target==0x8c639f5cu)return sa_matrix_rotate_y(m,f,sr,r,c->fpul,c,rotation);
 if(target==0x8c63a1ccu)return sa_matrix_rotate_z(m,f,sr,r,c->fpul,c,rotation);
 if(target==0x8c07aef8u)return sa_effect_primary_angles_native(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c07af8cu)return sa_effect_secondary_angles_native(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 if(target==0x8c055d4eu){sa_model_channel_difference_8c055d4e(r);return 0;}
 if(target==0x8c639c70u)return sa_model_matrix_stack_push_8c639c70(m,f,sr,r);
 if(target==0x8c639b98u)return sa_model_matrix_stack_pop_8c639b98(m,f,sr,r);
 if(target==0x8c638f28u)return sa_model_direction_transform(m,f,sr,r,c->user,c->instruction);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_tikal_effect_update_native(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,service external,vector instruction){
 struct context c={user,external,instruction,fpul,macl};return tikal_effect_update(m,f,sr,r,pr,fpul,macl,&c,call);
}
