#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service external
int sa_task_display_children(ARGS);
int sa_task_display_list(ARGS);
int sa_task_display_all(ARGS);
struct context {uint32_t *fpul,*macl;void *user;service external;};
static int dispatch(void *opaque,uint32_t target,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct context *c=opaque;
 if(target==0x8c098842u)return sa_task_display_children(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(target==0x8c098800u)return sa_task_display_list(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 return c->external(c->user,target,m,f,sr,r,pr);
}
SA_STAGE_EXPORT int sa_task_display_list_native(ARGS){
 struct context c={fpul,macl,ctx,external};return sa_task_display_list(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_task_display_all_native(ARGS){
 struct context c={fpul,macl,ctx,external};return sa_task_display_all(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}
