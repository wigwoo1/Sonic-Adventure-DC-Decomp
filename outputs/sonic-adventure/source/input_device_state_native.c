#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service call
int sa_input_request_completion(ARGS);int sa_input_request_completion_impl(ARGS);int sa_input_device_record_pointer(ARGS);int sa_input_device_busy_read(ARGS);int sa_input_device_request_mark(ARGS);int sa_input_device_busy(ARGS);int sa_input_device_mark(ARGS);
struct state_context {uint32_t *fpul,*macl;};
static int dispatch(void *ctx,uint32_t t,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){struct state_context *c=ctx;if(t==0x8c65f3f4u)return sa_input_request_completion_impl(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);if(t==0x8c65f390u)return sa_input_device_record_pointer(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);if(t==0x8c65f40eu)return sa_input_device_busy_read(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);if(t==0x8c65f41eu)return sa_input_device_request_mark(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);return -3;}
SA_STAGE_EXPORT int sa_input_device_busy_native(ARGS){(void)ctx;(void)call;struct state_context c={fpul,macl};return sa_input_device_busy(m,f,sr,r,pr,fpul,macl,&c,dispatch);}
SA_STAGE_EXPORT int sa_input_device_mark_native(ARGS){(void)ctx;(void)call;struct state_context c={fpul,macl};return sa_input_device_mark(m,f,sr,r,pr,fpul,macl,&c,dispatch);}

SA_STAGE_EXPORT int sa_input_request_completion_native(ARGS){(void)ctx;(void)call;struct state_context c={fpul,macl};return sa_input_request_completion(m,f,sr,r,pr,fpul,macl,&c,dispatch);}
