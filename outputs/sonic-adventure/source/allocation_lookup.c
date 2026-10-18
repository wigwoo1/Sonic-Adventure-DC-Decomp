#include "allocation_lookup.h"
static uint8_t *p(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *b=p(ram,a);return b[0]|((uint32_t)b[1]<<8)|((uint32_t)b[2]<<16)|((uint32_t)b[3]<<24);}
uint32_t sa_allocation_lookup_8c664940(uint8_t *ram,uint32_t *sr,uint32_t address,uint32_t bank_output)
{
 uint32_t bank=(int32_t)address>=(int32_t)rd(ram,0x8c67c2d8u);
 p(ram,bank_output)[0]=(uint8_t)bank;p(ram,bank_output)[1]=0;
 /* Read bank back after writing, as the original does. */
 bank=p(ram,bank_output)[0]|((uint32_t)p(ram,bank_output)[1]<<8);
 uint32_t current=rd(ram,0x8c67c2e0u+bank*20u),result=0u;
 while(current!=0u){
  if(rd(ram,current+12u)==address){result=current;current=0u;}
  else current=rd(ram,current+8u);
 }
 *sr|=1u;
 return result;
}
