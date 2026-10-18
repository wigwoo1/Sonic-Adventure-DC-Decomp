#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service call
int sa_input_device_responses_process(ARGS);
int sa_input_device_port_response_mask(ARGS);
int sa_input_device_response_complete_native(ARGS);
struct response_context {uint32_t *fpul,*macl;void *user;service external;};
static int dispatch(void *ctx,uint32_t t,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct response_context *c=ctx;
 if(t==0x8c65fd4eu)return sa_input_device_port_response_mask(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c65fe14u)return sa_input_device_response_complete_native(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 return c->external?c->external(c->user,t,m,f,sr,r,pr):-3;
}
SA_STAGE_EXPORT int sa_input_device_responses_process_native(ARGS){struct response_context c={fpul,macl,ctx,call};return sa_input_device_responses_process(m,f,sr,r,pr,fpul,macl,&c,dispatch);}
