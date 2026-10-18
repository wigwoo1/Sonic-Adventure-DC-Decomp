#include "vblank_dispatch.h"
static uint32_t read_word(const uint8_t *ram,uint32_t address)
{
 const uint8_t *p=ram+((address&0x1fffffffu)-0x0c000000u);
 return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
void sa_vblank_dispatch_8c645b1e(uint8_t *ram,uint32_t *sr,void *context,
 void (*invoke)(void *,uint32_t,uint32_t,uint32_t *))
{
 *sr=(*sr&0xffffff0fu)|0x60u;
 uint32_t node=read_word(ram,0x8c893a80u);
 while(node!=0u) {
  *sr &= ~1u; /* TST of nonzero node clears the SH-4 T bit. */
  uint32_t callback=read_word(ram,node);
  uint32_t argument=read_word(ram,node+8u);
  invoke(context,callback,argument,sr);
  /* Retail reloads the current node's successor AFTER callback execution. */
  node=read_word(ram,node+4u);
 }
 *sr |= 1u; /* The terminating zero-node test sets T. */
}
