#include "resource_division_runtime.h"
#include "resource_reset_runtime.h"
#include "unsigned_division.h"
struct native_runtime {uint8_t *ram;void *context;sa_resource_release_invoke invoke;};
static uint32_t external(void *ctx,uint32_t service,uint32_t arg,uint32_t *sr)
{
 struct native_runtime *r=ctx;
 return r->invoke(r->context,service,arg,sr);
}
static uint32_t division(void *ctx,uint32_t numerator,uint32_t denominator,uint32_t *sr)
{
 struct native_runtime *r=ctx;uint32_t r1;
 return sa_unsigned_division_8c10cb10(r->ram,sr,numerator,denominator,&r1);
}
uint32_t sa_resource_release_with_division(uint8_t *ram,uint32_t *sr,uint32_t resource,void *ctx,sa_resource_release_invoke invoke)
{
 struct native_runtime r={ram,ctx,invoke};
 return sa_resource_release_with_reset(ram,sr,resource,&r,external,division);
}
