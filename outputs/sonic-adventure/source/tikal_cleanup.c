#include "tikal_cleanup.h"
static uint32_t read_word(const uint8_t *ram,uint32_t address)
{
 const uint8_t *p=ram+((address&0x1fffffffu)-0x0c000000u);
 return (uint32_t)p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
uint32_t sa_tikal_cleanup_0cb00780(uint8_t *ram,uint32_t *sr,uint32_t task,
 void *context,uint32_t (*invoke)(void *,uint32_t,uint32_t,uint32_t *))
{
 uint32_t work=read_word(ram,task+32u);
 uint32_t owner=ram[((work+8u)&0x1fffffffu)-0x0c000000u];
 /* Retail reads task+36, then overwrites its register with the owner byte. */
 (void)read_word(ram,task+36u);
 (void)invoke(context,0x8c06c5cau,owner,sr);
 (void)invoke(context,0x8c0945d0u,work,sr);
 return invoke(context,0x8c098ffcu,4u,sr);
}
