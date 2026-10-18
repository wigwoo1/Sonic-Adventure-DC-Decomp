#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service external
int sa_atan2_entry(ARGS);
int sa_atan2_quadrants(ARGS);
struct context {uint32_t *fpul,*macl;void *user;service external;};
static int call(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;
 if(target==0x8c10e6a0u)return sa_atan2_quadrants(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_atan2_entry_native(ARGS){
 struct context c={fpul,macl,ctx,external};return sa_atan2_entry(m,f,sr,r,pr,fpul,macl,&c,call);
}
