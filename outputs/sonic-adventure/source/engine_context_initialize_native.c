#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service call
int sa_engine_context_initialize(ARGS);
int sa_engine_option_select(ARGS);
int sa_engine_context_parameters_initialize(ARGS);
int sa_interrupt_defaults_initialize(ARGS);
int sa_memory_fill(ARGS);
int sa_string_compare_wrapper(ARGS);
int sa_string_compare(ARGS);
struct context {uint32_t *fpul,*macl;void *user;service external;};
static int dispatch(void *ctx,uint32_t t,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){struct context *c=ctx;
if(t==0x8c665a80u)return sa_engine_option_select(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
if(t==0x8c66120cu)return sa_engine_context_parameters_initialize(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
if(t==0x8c641160u)return sa_interrupt_defaults_initialize(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
if(t==0x8c10d31cu)return sa_memory_fill(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
if(t==0x8c10d3c8u)return sa_string_compare_wrapper(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
if(t==0x8c10db1cu)return sa_string_compare(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
if(t==0x8c64241au)return 0; /* Verified retail RTS/NOP leaf. */
return c->external?c->external(c->user,t,m,f,sr,r,pr):-3;
}
SA_STAGE_EXPORT int sa_engine_context_initialize_native(ARGS){struct context c={fpul,macl,ctx,call};return sa_engine_context_initialize(m,f,sr,r,pr,fpul,macl,&c,dispatch);}
