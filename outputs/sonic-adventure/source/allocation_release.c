#include "allocation_release.h"
#include "allocation_list_insert.h"
#include "allocation_list_remove.h"
#include "allocation_pool_release.h"
static uint8_t *p(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *b=p(ram,a);return b[0]|((uint32_t)b[1]<<8)|((uint32_t)b[2]<<16)|((uint32_t)b[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)p(ram,a)[i]=(uint8_t)(v>>(8*i));}
static uint32_t adjacent(uint8_t *ram,uint32_t bank,uint32_t node)
{
 uint32_t other=rd(ram,bank+8u);
 while(other){
  uint32_t address=rd(ram,node+12u),size=rd(ram,node+16u);
  uint32_t other_address=rd(ram,other+12u);
  if(other_address==address+size||address==other_address+rd(ram,other+16u))return other;
  other=rd(ram,other+8u);
 }
 return 0u;
}
uint32_t sa_allocation_release_8c66497c(uint8_t *ram,uint32_t *sr,uint32_t bank_index,uint32_t node)
{
 p(ram,node)[0]&=0xfbu;
 uint32_t bank=0x8c67c2e0u+(uint32_t)(int32_t)(int16_t)bank_index*20u;
 (void)sa_allocation_list_remove_8c664b52(ram,sr,bank,bank+4u,node,0u);
 wr(ram,bank+16u,rd(ram,bank+16u)+rd(ram,node+16u));
 uint32_t other=adjacent(ram,bank,node);
 while(other){
  if(rd(ram,other+12u)==rd(ram,node+12u)+rd(ram,node+16u))wr(ram,other+12u,rd(ram,node+12u));
  wr(ram,other+16u,rd(ram,other+16u)+rd(ram,node+16u));
  (void)sa_allocation_pool_release_8c664aec(ram,sr,node,0u);
  (void)sa_allocation_list_remove_8c664b52(ram,sr,bank+8u,bank+12u,other,0u);
  node=other;
  other=adjacent(ram,bank,node);
 }
 (void)sa_allocation_list_insert_8c664b12(ram,sr,bank+8u,bank+12u,node,0u);
 return 0u;
}
