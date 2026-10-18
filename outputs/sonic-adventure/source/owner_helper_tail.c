#include "owner_helper_tail.h"
#include "sh4_float_i32.h"
#include "sh4_add_sub.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint32_t half(uint8_t *p){return p[0]|((uint32_t)p[1]<<8);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
int sa_owner_helper_tail(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t *r0){
 uint32_t state=half(at(ram,config));
 if(state==6u || state==7u){
  uint32_t index=half(at(ram,config+6u)),model=rd(ram,rd(ram,config+32u)+(index<<4));
  uint32_t count=rd(ram,rd(ram,model+4u)+4u),value;
  *fpul=count;*sr=(*sr&~1u)|(count<0x80000000u);
  if(sa_sh4_float_i32(count,fpscr,&value)!=0)return -1;
  if(count>=0x80000000u){uint32_t adjust=0x4f800000u;if(sa_sh4_add_sub(&value,&adjust,fpscr,&value,0)!=0)return -1;}
  uint32_t minus_one=0xbf800000u;if(sa_sh4_add_sub(&value,&minus_one,fpscr,&value,0)!=0)return -1;
  wr(ram,config+16u,value);*r0=16u;return 0;
 }
 int copy_previous=state==3u || state==10u || state==9u || state==12u;
 *sr=(*sr&~1u)|(uint32_t)copy_previous;
 if(copy_previous){wr(ram,config+16u,rd(ram,config+20u));wr(ram,config+20u,0u);*r0=20u;}
 else{wr(ram,config+16u,0u);*r0=16u;}
 return 0;
}
