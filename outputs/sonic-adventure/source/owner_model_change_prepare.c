#include "stage00_task.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint32_t half(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8);}
/* Original 8c079c20..8c079c76 setup. The following worker is unrecovered. */
SA_STAGE_EXPORT void sa_owner_model_change_prepare(uint8_t *ram,uint32_t *sr,uint32_t config,uint32_t stack,uint32_t *registers){
 uint32_t table=rd(ram,config+32u);
 uint32_t current_model=rd(ram,table+(half(ram,config+4u)<<4));
 uint32_t previous_model=rd(ram,table+(half(ram,config+6u)<<4));
 uint32_t current_metadata=rd(ram,current_model+4u),previous_metadata=rd(ram,previous_model+4u);
 uint8_t *saved=at(ram,stack-4u);for(unsigned i=0;i<4;i++)saved[i]=(uint8_t)(previous_metadata>>(8*i));
 uint32_t current_mode=(half(ram,current_metadata+8u)&4u)!=0u?3u:2u;
 uint32_t previous_mode=(half(ram,previous_metadata+8u)&4u)!=0u?3u:2u;
 registers[0]=current_mode==3u?current_mode:previous_mode;
 registers[2]=previous_model;registers[3]=previous_metadata;
 registers[5]=current_mode;registers[6]=previous_mode;registers[7]=current_metadata;
 *sr=(*sr&~1u)|(current_mode==3u || previous_mode==3u);
}
