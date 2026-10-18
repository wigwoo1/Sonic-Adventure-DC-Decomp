#include "tikal_cleanup.h"
#include "owner_table_release.h"
#include "table_reference_release.h"
struct cleanup_context {
 uint8_t *ram;
 void *context;
 uint32_t (*invoke)(void *,uint32_t,uint32_t,uint32_t *);
};
static uint32_t release_entry(void *context,uint32_t argument,uint32_t *sr)
{
 struct cleanup_context *runtime=context;
 return runtime->invoke(runtime->context,0x8c6088bcu,argument,sr);
}
static uint32_t invoke(void *context,uint32_t service,uint32_t argument,uint32_t *sr)
{
 struct cleanup_context *runtime=context;
 if(service==0x8c0945d0u)return sa_owner_table_release_8c0945d0(runtime->ram,sr,argument);
 if(service==0x8c098ffcu)return sa_table_reference_release_8c098ffc(runtime->ram,sr,argument,runtime,release_entry);
 return runtime->invoke(runtime->context,service,argument,sr);
}
uint32_t sa_tikal_cleanup_with_owner_release(uint8_t *ram,uint32_t *sr,uint32_t task,
 void *context,uint32_t (*external)(void *,uint32_t,uint32_t,uint32_t *))
{
 struct cleanup_context runtime={ram,context,external};
 return sa_tikal_cleanup_0cb00780(ram,sr,task,&runtime,invoke);
}
