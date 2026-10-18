#include "paired_allocation_release.h"
#include "allocation_lookup.h"
#include "allocation_release.h"
static uint8_t *p(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *b=p(ram,a);return b[0]|((uint32_t)b[1]<<8)|((uint32_t)b[2]<<16)|((uint32_t)b[3]<<24);}
static uint32_t half(uint8_t *ram,uint32_t a){uint8_t *b=p(ram,a);return b[0]|((uint32_t)b[1]<<8);}
static void t(uint32_t *sr,int yes){*sr=(*sr&~1u)|(yes?1u:0u);}
uint32_t sa_paired_allocation_release_8c66385a(uint8_t *ram,uint32_t *sr,uint32_t address,uint32_t scratch)
{
 uint32_t first=sa_allocation_lookup_8c664940(ram,sr,address,scratch);
 t(sr,first==0u);if(first==0u)return 7u;
 uint32_t flags=half(ram,first);t(sr,(flags&2u)==0u);
 if((flags&2u)==0u)return sa_allocation_release_8c66497c(ram,sr,half(ram,scratch),first);
 uint32_t bank=half(ram,scratch);t(sr,bank==0u);
 uint32_t displacement=rd(ram,0x8c67c2d4u);
 uint32_t partner_address=bank==0u?address+displacement:address-displacement;
 uint32_t partner=sa_allocation_lookup_8c664940(ram,sr,partner_address,scratch+4u);
 t(sr,partner==0u);
 if(partner==0u){
  (void)sa_allocation_release_8c66497c(ram,sr,half(ram,scratch),first);return 0u;
 }
 flags=half(ram,partner);t(sr,(flags&4u)==0u);
 if(flags&4u){p(ram,first)[0]&=0xfbu;return 0u;}
 (void)sa_allocation_release_8c66497c(ram,sr,half(ram,scratch),first);
 return sa_allocation_release_8c66497c(ram,sr,half(ram,scratch+4u),partner);
}
