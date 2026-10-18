#include "descriptor_release.h"
static uint32_t read_word(const uint8_t *p)
{
 return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
static uint8_t *pointer(uint8_t *ram,uint32_t address)
{
 return ram+((address&0x1fffffffu)-0x0c000000u);
}
uint32_t sa_descriptor_release_8c6088bc(uint8_t *ram,uint32_t *sr,uint32_t descriptor,
 void *context,uint32_t (*release)(void *,uint32_t,uint32_t *))
{
 uint32_t result=1u,index=0u;
 uint8_t *list=pointer(ram,descriptor);
 /* Both count and data pointer are read again after every nested call. */
 while(index<read_word(list+4u)){
  *sr&=~1u; /* Unsigned loop comparison was false. */
  *sr=(*sr&~1u)|(index>>31); /* Original SHLL carry. */
  uint32_t base=read_word(list);
  uint32_t argument=read_word(pointer(ram,base+index*12u+8u));
  uint32_t value=release(context,argument,sr);
  *sr=(*sr&~1u)|(value<0x80000000u);
  if(value>=0x80000000u){
   result=0xffffffffu;
   for(unsigned byte=0;byte<4;byte++)ram[0x88f7a8u+byte]=(uint8_t)(index>>(byte*8));
  }
  index++;
 }
 *sr|=1u;
 return result;
}
