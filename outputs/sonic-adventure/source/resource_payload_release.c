#include "resource_payload_release.h"
static uint32_t rd(uint8_t *ram,uint32_t address)
{
 uint8_t *p=ram+((address&0x1fffffffu)-0x0c000000u);
 return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);
}
uint32_t sa_resource_payload_release_8c653d68(uint8_t *ram,uint32_t *sr,uint32_t payload,void *ctx,sa_resource_release_invoke invoke)
{
 uint32_t special=(rd(ram,payload)&0xffffu)==3u;
 *sr=(*sr&~1u)|special;
 if(special)return invoke(ctx,0x8c663950u,payload,sr);
 return invoke(ctx,0x8c66385au,rd(ram,payload+0x1cu),sr);
}
