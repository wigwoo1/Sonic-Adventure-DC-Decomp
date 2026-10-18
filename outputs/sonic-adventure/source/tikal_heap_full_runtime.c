#include "tikal_cleanup.h"
#include "owner_heap_runtime.h"
#include "entity_allocation_release.h"
#include "descriptor_release.h"
#include "owner_table_release.h"
#include "table_reference_release.h"
struct full_context {uint8_t *ram;void *context;sa_tikal_nested_invoke invoke;};
static uint32_t external_one(struct full_context *runtime,uint32_t service,uint32_t arg,uint32_t *sr)
{
 return runtime->invoke(runtime->context,service,1u,arg,0u,0u,sr);
}
static uint32_t free_allocation(void *context,uint32_t arg,uint32_t *sr)
{return external_one(context,0x8c098b42u,arg,sr);}
static uint32_t helper(void *context,uint32_t arg,uint32_t *sr)
{return external_one(context,0x8c079476u,arg,sr);}
static uint32_t finish(void *context,uint32_t arg,uint32_t *sr)
{
 struct full_context *runtime=context;
 return sa_entity_allocation_release_8c07d6c0(runtime->ram,sr,arg,runtime,free_allocation);
}
static uint32_t release_descriptor_item(void *context,uint32_t arg,uint32_t *sr)
{return external_one(context,0x8c64ddc0u,arg,sr);}
static uint32_t release_entry(void *context,uint32_t arg,uint32_t *sr)
{
 struct full_context *runtime=context;
 return sa_descriptor_release_8c6088bc(runtime->ram,sr,arg,runtime,release_descriptor_item);
}
static uint32_t detach(void *context,uint32_t first,uint32_t resource,uint32_t entity,uint32_t *sr)
{
 struct full_context *runtime=context;
 return runtime->invoke(runtime->context,0x8c07a55cu,3u,first,resource,entity,sr);
}
static uint32_t invoke(void *context,uint32_t service,uint32_t arg,uint32_t *sr)
{
 struct full_context *runtime=context;
 if(service==0x8c06c5cau){
  struct sa_owner_cleanup_services services={runtime,free_allocation,helper,detach,finish};
  return sa_owner_cleanup_with_heap(runtime->ram,sr,arg,&services);
 }
 if(service==0x8c0945d0u)return sa_owner_table_release_8c0945d0(runtime->ram,sr,arg);
 if(service==0x8c098ffcu)return sa_table_reference_release_8c098ffc(runtime->ram,sr,arg,runtime,release_entry);
 return external_one(runtime,service,arg,sr);
}
SA_STAGE_EXPORT uint32_t sa_tikal_cleanup_heap_connected(uint8_t *ram,uint32_t *sr,uint32_t task,
 void *context,sa_tikal_nested_invoke external)
{
 struct full_context runtime={ram,context,external};
 return sa_tikal_cleanup_0cb00780(ram,sr,task,&runtime,invoke);
}
