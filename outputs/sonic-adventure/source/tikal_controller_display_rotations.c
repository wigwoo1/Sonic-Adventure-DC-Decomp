#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
typedef int (*vector)(void *,uint32_t,struct sa_fpu_banks *);
int sa_tikal_controller_position_rotations(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service,vector);
int sa_tikal_display_rotations(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,service,vector);
struct context {void *user;service external;vector instruction;uint32_t *fpul,*macl;};
static int call(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;if(target==0x0cb008a0u)return sa_tikal_display_rotations(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external,c->instruction);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_tikal_controller_display_rotations(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *user,service external,vector instruction){
 struct context c={user,external,instruction,fpul,macl};return sa_tikal_controller_position_rotations(m,f,sr,r,pr,fpul,macl,&c,call,instruction);
}
