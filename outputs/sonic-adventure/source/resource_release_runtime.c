#include "resource_release_runtime.h"
#include "resource_payload_release.h"
struct runtime {uint8_t *ram;void *context;sa_resource_release_invoke invoke;};
static uint32_t external(void *ctx,uint32_t service,uint32_t arg,uint32_t *sr)
{
 struct runtime *r=ctx;
 return r->invoke(r->context,service,arg,sr);
}
static uint32_t connected(void *ctx,uint32_t service,uint32_t arg,uint32_t *sr)
{
 struct runtime *r=ctx;
 if(service==0x8c653d68u)return sa_resource_payload_release_8c653d68(r->ram,sr,arg,r,external);
 return external(ctx,service,arg,sr);
}
uint32_t sa_resource_release_connected(uint8_t *ram,uint32_t *sr,uint32_t resource,void *ctx,sa_resource_release_invoke invoke)
{
 struct runtime r={ram,ctx,invoke};
 return sa_resource_release_8c64ddc0(ram,sr,resource,&r,connected);
}
