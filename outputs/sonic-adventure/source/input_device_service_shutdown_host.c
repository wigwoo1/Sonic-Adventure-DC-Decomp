#include "input_io.h"
#include "interrupt_io.h"
#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service call
int sa_input_device_service_shutdown_io(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,const struct sa_input_io *,void *,service);
int sa_interrupt_callback_remove(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,struct sa_interrupt_io *,void *,service);
int sa_input_device_shutdown_host(ARGS,const struct sa_input_io *);
int sa_memory_fill(ARGS);
int sa_input_service_initialize(ARGS);
int sa_input_service_start_explicit(ARGS);
int sa_input_service_start_contiguous(ARGS);
int sa_input_device_contiguous_initialize_host(ARGS,const struct sa_input_io *);
int sa_input_device_status_publish(ARGS);
int sa_input_device_backend_initialize_host(ARGS,const struct sa_input_io *);
int sa_interrupt_callback_create(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,struct sa_interrupt_io *,void *,service);
int sa_interrupt_handler_primary(ARGS);
int sa_interrupt_handler_set(ARGS);
struct shutdown_context {uint32_t *fpul,*macl;void *user;service external;const struct sa_input_io *io;struct sa_interrupt_io interrupt;};
static int read32(void *ctx,uint32_t a,uint32_t *v){struct shutdown_context *c=ctx;*v=c->io->read(c->io->user,a,4);return 0;}
static int write32(void *ctx,uint32_t a,uint32_t v){struct shutdown_context *c=ctx;c->io->write(c->io->user,a,v,4);return 0;}
static int dispatch(void *ctx,uint32_t t,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct shutdown_context *c=ctx;
 if(t==0x8c65ebb0u)return sa_input_device_contiguous_initialize_host(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,c->io);
 if(t==0x8c6461f0u)return sa_input_service_initialize(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c6461aeu)return sa_input_device_status_publish(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c65ecccu)return sa_input_device_backend_initialize_host(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,c->io);
 if(t==0x8c64594cu)return sa_interrupt_callback_create(m,f,sr,r,pr,c->fpul,c->macl,&c->interrupt,c,dispatch);
 if(t==0x8c6412c2u)return sa_interrupt_handler_set(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c658c20u||t==0x8c658c24u){r[0]=1;return 0;}
 if(t==0x8c64135au)return sa_interrupt_handler_primary(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 if(t==0x8c645c54u)return sa_interrupt_callback_remove(m,f,sr,r,pr,c->fpul,c->macl,&c->interrupt,c,dispatch);
 if(t==0x8c65f2a4u)return sa_input_device_shutdown_host(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch,c->io);
 if(t==0x8c10d31cu)return sa_memory_fill(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch);
 return c->external?c->external(c->user,t,m,f,sr,r,pr):-3;
}
SA_STAGE_EXPORT int sa_input_device_service_shutdown_host(ARGS,const struct sa_input_io *io,uint32_t vbr){
 if(!io||!io->read||!io->write)return -8;
 struct shutdown_context c={fpul,macl,ctx,call,io,{0}};
 c.interrupt.vbr=vbr;c.interrupt.context=&c;c.interrupt.read32=read32;c.interrupt.write32=write32;
 return sa_input_device_service_shutdown_io(m,f,sr,r,pr,fpul,macl,io,&c,dispatch);
}

SA_STAGE_EXPORT int sa_input_service_initialize_host(ARGS,const struct sa_input_io *io,uint32_t vbr){
 if(!io||!io->read||!io->write)return -8;
 struct shutdown_context c={fpul,macl,ctx,call,io,{0}};
 c.interrupt.vbr=vbr;c.interrupt.context=&c;c.interrupt.read32=read32;c.interrupt.write32=write32;
 return sa_input_service_initialize(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_input_service_start_explicit_host(ARGS,const struct sa_input_io *io,uint32_t vbr){
 if(!io||!io->read||!io->write)return -8;
 struct shutdown_context c={fpul,macl,ctx,call,io,{0}};
 c.interrupt.vbr=vbr;c.interrupt.context=&c;c.interrupt.read32=read32;c.interrupt.write32=write32;
 return sa_input_service_start_explicit(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}

SA_STAGE_EXPORT int sa_input_service_start_contiguous_host(ARGS,const struct sa_input_io *io,uint32_t vbr){
 if(!io||!io->read||!io->write)return -8;
 struct shutdown_context c={fpul,macl,ctx,call,io,{0}};
 c.interrupt.vbr=vbr;c.interrupt.context=&c;c.interrupt.read32=read32;c.interrupt.write32=write32;
 return sa_input_service_start_contiguous(m,f,sr,r,pr,fpul,macl,&c,dispatch);
}
