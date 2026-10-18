#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service external
int sa_atan_core(ARGS);
int sa_atan_reduced(ARGS);
int sa_atan_classifier(ARGS);
struct context {uint32_t *fpul,*macl;void *user;service external;};
static int call(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;
 if(target==0x8c10fbb8u)return sa_atan_classifier(m,f,sr,r,pr,c->fpul,c->macl,0,0);
 if(target==0x8c10fb94u)return sa_atan_reduced(m,f,sr,r,pr,c->fpul,c->macl,0,0);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_atan_core_classified(ARGS){
 struct context c={fpul,macl,ctx,external};return sa_atan_core(m,f,sr,r,pr,fpul,macl,&c,call);
}
