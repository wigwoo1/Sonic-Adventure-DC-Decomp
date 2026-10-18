#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service external
int sa_angle_approach(ARGS);
int sa_effect_primary_angles(ARGS);
int sa_effect_secondary_angles(ARGS);
struct context {uint32_t *fpul,*macl;};
static int call(void *ctx,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=ctx;
 if(target!=0x8c055d20u)return -2;
 return sa_angle_approach(m,f,sr,r,pr,c->fpul,c->macl,0,0);
}
SA_STAGE_EXPORT int sa_effect_primary_angles_native(ARGS){
 (void)ctx;(void)external;struct context c={fpul,macl};
 return sa_effect_primary_angles(m,f,sr,r,pr,fpul,macl,&c,call);
}
SA_STAGE_EXPORT int sa_effect_secondary_angles_native(ARGS){
 (void)ctx;(void)external;struct context c={fpul,macl};
 return sa_effect_secondary_angles(m,f,sr,r,pr,fpul,macl,&c,call);
}
