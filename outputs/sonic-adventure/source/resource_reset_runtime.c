#include "resource_reset_runtime.h"
#include "resource_release_runtime.h"
#include "descriptor_reset.h"
struct reset_runtime {uint8_t *ram;void *context;sa_resource_release_invoke invoke;sa_resource_divide divide;};
static uint32_t divide(void *ctx,uint32_t numerator,uint32_t denominator,uint32_t *sr)
{
 struct reset_runtime *r=ctx;
 return r->divide(r->context,numerator,denominator,sr);
}
static uint32_t invoke(void *ctx,uint32_t service,uint32_t arg,uint32_t *sr)
{
 struct reset_runtime *r=ctx;
 if(service==0x8c64e720u)return sa_descriptor_reset_8c64e720(r->ram,sr,arg,r,divide);
 return r->invoke(r->context,service,arg,sr);
}
uint32_t sa_resource_release_with_reset(uint8_t *ram,uint32_t *sr,uint32_t resource,void *ctx,sa_resource_release_invoke external,sa_resource_divide division)
{
 struct reset_runtime r={ram,ctx,external,division};
 return sa_resource_release_connected(ram,sr,resource,&r,invoke);
}
