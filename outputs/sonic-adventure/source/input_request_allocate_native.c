#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service call
int sa_input_request_queue_native(ARGS);int sa_input_request_allocate(ARGS);int sa_input_request_lock(ARGS);int sa_input_request_pack(ARGS);
struct allocation_context {uint32_t *fpul,*macl;void *user;service external;unsigned queue;};
static int dispatch(void *ctx,uint32_t t,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){struct allocation_context *c=ctx;if(t==0x8c6604e6u)return sa_input_request_lock(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);if(t==0x8c660518u)return sa_input_request_pack(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);if(c->queue&&t==0x8c65f810u)return sa_input_request_queue_native(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);return c->external?c->external(c->user,t,m,f,sr,r,pr):-3;}
SA_STAGE_EXPORT int sa_input_request_allocate_native(ARGS){struct allocation_context c={fpul,macl,ctx,call,0};return sa_input_request_allocate(m,f,sr,r,pr,fpul,macl,&c,dispatch);}

SA_STAGE_EXPORT int sa_input_request_allocate_queued_native(ARGS){struct allocation_context c={fpul,macl,ctx,call,1};return sa_input_request_allocate(m,f,sr,r,pr,fpul,macl,&c,dispatch);}
