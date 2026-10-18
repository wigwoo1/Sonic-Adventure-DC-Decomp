#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service call
int sa_input_press_alternate(ARGS);
int sa_input_channel_state_get(ARGS);
int sa_input_channel_event_set(ARGS);
int sa_input_channel_request_set(ARGS);
struct context {uint32_t *fpul,*macl;void *user;service external;};
static int dispatch(void *ctx,uint32_t t,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){struct context *c=ctx;
if(t==0x8c647ca4u)return sa_input_channel_state_get(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
if(t==0x8c647cb4u)return sa_input_channel_event_set(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
if(t==0x8c647cc4u)return sa_input_channel_request_set(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
return c->external?c->external(c->user,t,m,f,sr,r,pr):-3;
}
SA_STAGE_EXPORT int sa_input_press_alternate_native(ARGS){struct context c={fpul,macl,ctx,call};return sa_input_press_alternate(m,f,sr,r,pr,fpul,macl,&c,dispatch);}
