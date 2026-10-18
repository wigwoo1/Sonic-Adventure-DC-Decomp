#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service external
int sa_task_remove_request(ARGS);
int sa_position_task_callback(ARGS);
struct context {uint32_t *fpul,*macl;void *user;service external;};
static int dispatch(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;
 if(target==0x8c098786u)return sa_task_remove_request(m,f,sr,r,pr,c->fpul,c->macl,c->user,c->external);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_position_task_callback_native(ARGS){
 struct context c={fpul,macl,ctx,external};return sa_position_task_callback(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}
