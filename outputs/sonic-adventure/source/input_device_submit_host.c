#include "input_io.h"
#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service call
int sa_input_device_submit_io(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,const struct sa_input_io *,void *,service);int sa_input_device_commands_prepare(ARGS);
int sa_input_device_error_report(ARGS);int sa_signed_division(ARGS);
int sa_input_queue_address(ARGS);int sa_input_queue_flush(ARGS);
int sa_input_device_command_one_native(ARGS);int sa_input_command_list_terminate(ARGS);
int sa_input_device_command_features(ARGS);int sa_input_device_command_two(ARGS);int sa_input_device_command_three(ARGS);int sa_input_device_command_four(ARGS);int sa_input_request_queue_native(ARGS);
struct submit_context {uint32_t *fpul,*macl;void *user;service external;const struct sa_input_io *io;};
static int dispatch(void *ctx,uint32_t t,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct submit_context *c=ctx;
 switch(t){
 #define ROUTE(address,name) case address: return name(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch)
 ROUTE(0x8c65eedau,sa_input_device_commands_prepare);
 ROUTE(0x8c65f348u,sa_input_device_error_report);
 ROUTE(0x8c10ca5cu,sa_signed_division);
 ROUTE(0x8c6607beu,sa_input_queue_address);
 ROUTE(0x8c65f7ccu,sa_input_queue_flush);
 ROUTE(0x8c6603b0u,sa_input_device_command_one_native);
 ROUTE(0x8c660794u,sa_input_command_list_terminate);
 ROUTE(0x8c66034eu,sa_input_device_command_features);
 ROUTE(0x8c6603f4u,sa_input_device_command_four);
 ROUTE(0x8c660438u,sa_input_device_command_three);
 ROUTE(0x8c66047cu,sa_input_device_command_two);
 ROUTE(0x8c65f810u,sa_input_request_queue_native);
 #undef ROUTE
 default:return c->external?c->external(c->user,t,m,f,sr,r,pr):-3;
 }
}
SA_STAGE_EXPORT int sa_input_device_submit_host(ARGS,const struct sa_input_io *io){if(!io||!io->read||!io->write)return -8;struct submit_context c={fpul,macl,ctx,call,io};return sa_input_device_submit_io(m,f,sr,r,pr,fpul,macl,io,&c,dispatch);}
