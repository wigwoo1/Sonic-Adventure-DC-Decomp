#include "tikal_resource_runtime.h"
#include "resource_release_complete_runtime.h"
uint32_t sa_tikal_cleanup_heap_connected(uint8_t *,uint32_t *,uint32_t,void *,sa_tikal_nested_invoke);
struct tikal_resource_runtime {uint8_t *ram;uint32_t caller_stack;void *context;sa_tikal_nested_invoke invoke;};
static uint32_t nested(void *ctx,uint32_t service,uint32_t argc,uint32_t a,uint32_t b,uint32_t c,uint32_t *sr)
{
 struct tikal_resource_runtime *r=ctx;
 if(service==0x8c64ddc0u)return sa_resource_release_complete(r->ram,sr,a,r->caller_stack-44u);
 return r->invoke(r->context,service,argc,a,b,c,sr);
}
SA_STAGE_EXPORT uint32_t sa_tikal_cleanup_with_heap_resources(uint8_t *ram,uint32_t *sr,uint32_t task,uint32_t caller_stack,void *ctx,sa_tikal_nested_invoke invoke)
{
 struct tikal_resource_runtime r={ram,caller_stack,ctx,invoke};
 return sa_tikal_cleanup_heap_connected(ram,sr,task,&r,nested);
}
