#include "owner_helper_tail.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void half(uint8_t *ram,uint32_t a,uint32_t v){at(ram,a)[0]=(uint8_t)v;at(ram,a)[1]=(uint8_t)(v>>8);}
/* Shared index transition at 8c079b56, followed by the original shared tail. */
SA_STAGE_EXPORT int sa_owner_helper_transition(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t index,uint32_t *r0){
 uint32_t table=rd(ram,config+32u);
 uint32_t next=at(ram,table+((index&65535u)<<4))[6];
 half(ram,config+4u,next);half(ram,config+6u,next);
 uint8_t *descriptor=at(ram,table+(next<<4));
 half(ram,config,(uint32_t)(int32_t)(int8_t)descriptor[5]);
 half(ram,config+10u,descriptor[6]);
 uint8_t *flags=at(ram,config+14u);half(ram,config+14u,(flags[0]|((uint32_t)flags[1]<<8))|1u);
 return sa_owner_helper_tail(ram,sr,fpscr,fpul,config,r0);
}
