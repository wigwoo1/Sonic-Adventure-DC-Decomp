#include "resource_release_complete_runtime.h"
#include "resource_division_runtime.h"
#include "paired_allocation_release.h"
#include "linked_payload_release.h"
struct complete_runtime {uint8_t *ram;uint32_t caller_stack;};
static uint32_t invoke(void *ctx,uint32_t service,uint32_t arg,uint32_t *sr)
{
 struct complete_runtime *r=ctx;
 if(service==0x8c663950u)return sa_linked_payload_release_8c663950(r->ram,sr,arg,r->caller_stack-68u);
 return sa_paired_allocation_release_8c66385a(r->ram,sr,arg,r->caller_stack-44u);
}
uint32_t sa_resource_release_complete(uint8_t *ram,uint32_t *sr,uint32_t resource,uint32_t caller_stack)
{
 struct complete_runtime r={ram,caller_stack};
 return sa_resource_release_with_division(ram,sr,resource,&r,invoke);
}
