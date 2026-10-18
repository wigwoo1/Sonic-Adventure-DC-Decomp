#include "owner_helper_state0.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void word(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static void half(uint8_t *ram,uint32_t a,uint32_t v){at(ram,a)[0]=(uint8_t)v;at(ram,a)[1]=(uint8_t)(v>>8);}
uint32_t sa_owner_helper_state0(uint8_t *ram,uint32_t *sr,uint32_t config,uint32_t cached_index,void *context,sa_owner_state0_service service){
 word(ram,config+16u,0u);half(ram,config+6u,cached_index);
 half(ram,config,1u);word(ram,config+20u,0u);word(ram,config+16u,0u);
 uint32_t holder=service(context,0x8c098a80u,1u,8u,0u,16u,sr);
 word(ram,config+40u,holder);
 uint32_t descriptor=rd(ram,config+32u)+((cached_index&0xffffu)<<4);
 word(ram,holder,rd(ram,rd(ram,descriptor)));
 uint32_t cached_slot=rd(ram,config+40u)+4u;
 uint32_t header=service(context,0x8c098a80u,1u,12u,0u,holder,sr);
 word(ram,cached_slot,header);
 header=rd(ram,rd(ram,config+40u)+4u);word(ram,header+4u,2u);
 header=rd(ram,rd(ram,config+40u)+4u);half(ram,header+8u,3u);
 header=rd(ram,rd(ram,config+40u)+4u);half(ram,header+10u,2u);
 header=rd(ram,rd(ram,config+40u)+4u);
 uint32_t payload=service(context,0x8c098b0cu,2u,0x6fu,24u,2u,sr);
 word(ram,header,payload);return payload;
}
