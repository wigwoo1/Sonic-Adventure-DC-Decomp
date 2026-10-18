#include "owner_helper_state0.h"
#include "helper_record_release.h"
#include "heap_free_ram.h"
int sa_owner_helper_dispatch_8c079476(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,void *,sa_owner_state0_service,uint32_t *);
struct cleanup_context{uint8_t *ram;void *context;sa_owner_state0_service fallback;};
static uint32_t invoke(void *opaque,uint32_t address,uint32_t argc,uint32_t a,uint32_t b,uint32_t r0,uint32_t *sr){
 struct cleanup_context *c=(struct cleanup_context *)opaque;
 if(address==0x8c07a49cu)return sa_helper_record_release_8c07a49c(c->ram,sr,a);
 if(address==0x8c098b42u)return sa_heap_free_ram_8c098b42(c->ram,sr,a,r0);
 return c->fallback(c->context,address,argc,a,b,r0,sr);
}
SA_STAGE_EXPORT int sa_owner_helper_with_native_cleanup(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,void *context,sa_owner_state0_service fallback,uint32_t *r0){
 struct cleanup_context c={ram,context,fallback};
 return sa_owner_helper_dispatch_8c079476(ram,sr,fpscr,fpul,config,&c,invoke,r0);
}
