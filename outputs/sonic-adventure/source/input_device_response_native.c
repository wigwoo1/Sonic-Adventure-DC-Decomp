#include "fpu_cleanup_transfer.h"
typedef int (*service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
#define ARGS uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,service call
int sa_input_device_response_complete(ARGS);
int sa_input_device_presence_decode(ARGS);
int sa_input_device_response_header(ARGS);
int sa_input_device_primary_response(ARGS);
int sa_input_device_sub_response(ARGS);
int sa_input_device_info_decode(ARGS);
int sa_input_response_word_swap(ARGS);
int sa_input_device_info_initialize(ARGS);
int sa_input_word_fill(ARGS);
struct response_context {uint32_t *fpul,*macl;void *user;service external;};
static int dispatch(void *ctx,uint32_t t,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr){
 struct response_context *c=ctx;
 switch(t){
 #define ROUTE(address,name) case address: return name(m,f,sr,r,pr,c->fpul,c->macl,c,dispatch)
 ROUTE(0x8c65ff12u,sa_input_device_presence_decode);
 ROUTE(0x8c65ff5au,sa_input_device_response_header);
 ROUTE(0x8c65ffb8u,sa_input_device_primary_response);
 ROUTE(0x8c66005cu,sa_input_device_sub_response);
 ROUTE(0x8c66018eu,sa_input_device_info_decode);
 ROUTE(0x8c668b24u,sa_input_response_word_swap);
 ROUTE(0x8c6602d2u,sa_input_device_info_initialize);
 ROUTE(0x8c6607a8u,sa_input_word_fill);
 #undef ROUTE
 default:return c->external?c->external(c->user,t,m,f,sr,r,pr):-3;
 }
}
SA_STAGE_EXPORT int sa_input_device_response_complete_native(ARGS){struct response_context c={fpul,macl,ctx,call};return sa_input_device_response_complete(m,f,sr,r,pr,fpul,macl,&c,dispatch);}
