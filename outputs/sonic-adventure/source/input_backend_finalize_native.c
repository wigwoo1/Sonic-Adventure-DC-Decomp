#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service call
int sa_input_backend_finalize(ARGS);
int sa_input_backend_finalize_resume(ARGS);
int sa_input_packet_row_copy(ARGS);
struct row_context {uint32_t *fpul,*macl;void *user;service external;};
static int dispatch(void *ctx,uint32_t t,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){struct row_context *c=ctx;if(t==0x8c6607fcu)return sa_input_packet_row_copy(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);return c->external?c->external(c->user,t,m,f,sr,r,pr):-3;}
SA_STAGE_EXPORT int sa_input_backend_finalize_rows_native(ARGS){struct row_context c={fpul,macl,ctx,call};return sa_input_backend_finalize(m,f,sr,r,pr,fpul,macl,&c,dispatch);}

SA_STAGE_EXPORT int sa_input_backend_finalize_rows_resume_native(ARGS){struct row_context c={fpul,macl,ctx,call};return sa_input_backend_finalize_resume(m,f,sr,r,pr,fpul,macl,&c,dispatch);}
