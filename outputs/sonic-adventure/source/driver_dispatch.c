#include "driver_dispatch.h"
static uint32_t read_word(const uint8_t *ram,uint32_t address)
{
 const uint8_t *p=ram+((address&0x1fffffffu)-0x0c000000u);
 return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
uint32_t sa_driver_dispatch_8c6524d2(uint8_t *ram,uint32_t *sr,uint32_t index,
 void *context,uint32_t (*invoke)(void *,uint32_t,uint32_t,uint32_t *))
{
 *sr=(*sr&~1u)|((index>>29)&1u);
 uint32_t slot=0x8c8a2f84u+(index<<3);
 uint32_t callback=read_word(ram,slot),argument=read_word(ram,slot+4u);
 return invoke(context,callback,argument,sr);
}
