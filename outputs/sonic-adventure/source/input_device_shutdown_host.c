#include "input_io.h"
#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service call
int sa_input_device_elapsed_ticks(ARGS);
int sa_input_device_wait_clock_io(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,const struct sa_input_io *,void *,service);
int sa_input_device_shutdown_io(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,const struct sa_input_io *,void *,service);
struct shutdown_context {uint32_t *fpul,*macl;void *user;service external;const struct sa_input_io *io;};
static int dispatch(void *ctx,uint32_t t,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct shutdown_context *c=ctx;
 if(t==0x8c60260eu)return sa_input_device_wait_clock_io(m,f,sr,r,pr,c->fpul,c->macl,c->io,c,dispatch);
 if(t==0x8c602618u)return sa_input_device_elapsed_ticks(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 return c->external?c->external(c->user,t,m,f,sr,r,pr):-3;
}
SA_STAGE_EXPORT int sa_input_device_shutdown_host(ARGS,const struct sa_input_io *io){if(!io||!io->read||!io->write)return -8;struct shutdown_context c={fpul,macl,ctx,call,io};return sa_input_device_shutdown_io(m,f,sr,r,pr,fpul,macl,io,&c,dispatch);}
