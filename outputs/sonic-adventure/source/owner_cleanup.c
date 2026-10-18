#include "owner_cleanup.h"
static uint32_t offset(uint32_t address){return (address&0x1fffffffu)-0x0c000000u;}
static uint32_t read_word(const uint8_t *ram,uint32_t address)
{
 const uint8_t *p=ram+offset(address);
 return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static void write_word(uint8_t *ram,uint32_t address,uint32_t value)
{
 for(unsigned i=0;i<4;i++)ram[offset(address)+i]=(uint8_t)(value>>(i*8));
}
static void write_half(uint8_t *ram,uint32_t address,uint32_t value)
{
 ram[offset(address)]=(uint8_t)value;ram[offset(address)+1u]=(uint8_t)(value>>8);
}
static void test(uint32_t *sr,uint32_t value){*sr=(*sr&~1u)|(value==0u?1u:0u);}
uint32_t sa_owner_cleanup_8c06c5ca(uint8_t *ram,uint32_t *sr,uint32_t owner,
 const struct sa_owner_cleanup_services *services)
{
 uint32_t displacement=owner<<2;
 uint32_t first=read_word(ram,0x8c78c608u+displacement);
 uint32_t entity=read_word(ram,0x8c78c668u+displacement);
 uint32_t resource=read_word(ram,0x8c78c628u+displacement);
 write_word(ram,0x8c78c648u+displacement,0u);
 write_word(ram,0x8c78c608u+displacement,0u);
 write_word(ram,0x8c78c628u+displacement,0u);
 write_word(ram,0x8c78c668u+displacement,0u);
 uint32_t allocation=read_word(ram,entity+0x64u);
 test(sr,allocation);
 if(allocation!=0u){
  (void)services->free_8c098b42(services->context,allocation,sr);
  write_word(ram,entity+0x64u,0u);
 }
 write_half(ram,entity+0x120u,14u);
 uint32_t helper=read_word(ram,entity+0x148u);
 test(sr,helper);
 if(helper!=0u)(void)services->helper_8c079476(services->context,entity+0x120u,sr);
 write_half(ram,entity+0x122u,2u);
 uint32_t detached=read_word(ram,entity+0x144u);
 test(sr,detached);
 if(detached!=0u)(void)services->detach_8c07a55c(services->context,first,resource,entity,sr);
 uint32_t result=services->finish_8c07d6c0(services->context,entity,sr);
 allocation=read_word(ram,resource);
 test(sr,allocation);
 if(allocation!=0u){
  result=services->free_8c098b42(services->context,allocation,sr);
  write_word(ram,resource,0u);
 }
 return result;
}
