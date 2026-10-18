#include "owner_helper_state0.h"
#include "helper_record_release.h"
#include "heap_free_ram.h"
int sa_owner_helper_dispatch_8c079476(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,void *,sa_owner_state0_service,uint32_t *);
int sa_owner_model_change_with_native_heap(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,const uint32_t *,uint32_t,uint32_t,uint32_t *);
uint32_t sa_heap_allocate_ram_8c098a80(uint8_t *,uint32_t *,uint32_t);
uint32_t sa_heap_zero_allocate_ram_8c098b0c(uint8_t *,uint32_t *,uint32_t,uint32_t);
struct model_context{uint8_t *ram;uint32_t *fpscr,*fpul,stack;int error;void *opaque;sa_owner_state0_service fallback;};
static uint32_t invoke(void *opaque,uint32_t address,uint32_t argc,uint32_t a,uint32_t b,uint32_t incoming,uint32_t *sr){
 struct model_context *c=opaque;
 if(address==0x8c079c20u){uint32_t callee[7]={0},result=0;c->error=sa_owner_model_change_with_native_heap(c->ram,sr,c->fpscr,c->fpul,a,c->stack,callee,0u,0u,&result);return result;}
 if(address==0x8c098a80u)return sa_heap_allocate_ram_8c098a80(c->ram,sr,a);
 if(address==0x8c098b0cu)return sa_heap_zero_allocate_ram_8c098b0c(c->ram,sr,a,b);
 if(address==0x8c07a49cu)return sa_helper_record_release_8c07a49c(c->ram,sr,a);
 if(address==0x8c098b42u)return sa_heap_free_ram_8c098b42(c->ram,sr,a,incoming);
 return c->fallback(c->opaque,address,argc,a,b,incoming,sr);
}
/* Parent behavioral interface: native model/heap/record services. Original
 * nested callee-save scratch register values are outside this interface. */
SA_STAGE_EXPORT int sa_owner_helper_with_native_model(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t caller_stack,void *opaque,sa_owner_state0_service fallback,uint32_t *r0){
 struct model_context c={ram,fpscr,fpul,caller_stack-40u,0,opaque,fallback};
 int result=sa_owner_helper_dispatch_8c079476(ram,sr,fpscr,fpul,config,&c,invoke,r0);return c.error?c.error:result;
}
