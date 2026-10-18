#include "payload_release_runtime.h"
#include "resource_payload_release.h"
#include "paired_allocation_release.h"
#include "linked_payload_release.h"
struct payload_runtime {uint8_t *ram;uint32_t caller_stack;};
static uint32_t invoke(void *ctx,uint32_t service,uint32_t arg,uint32_t *sr)
{
 struct payload_runtime *r=ctx;
 if(service==0x8c663950u)return sa_linked_payload_release_8c663950(r->ram,sr,arg,r->caller_stack-52u);
 return sa_paired_allocation_release_8c66385a(r->ram,sr,arg,r->caller_stack-28u);
}
uint32_t sa_payload_release_connected(uint8_t *ram,uint32_t *sr,uint32_t payload,uint32_t caller_stack)
{
 struct payload_runtime r={ram,caller_stack};
 return sa_resource_payload_release_8c653d68(ram,sr,payload,&r,invoke);
}
