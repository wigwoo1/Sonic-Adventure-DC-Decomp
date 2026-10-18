#include "stage00_task.h"
#include "sh4_add_sub.h"
#include "sh4_float_i32.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static int gt(uint32_t a,uint32_t b){if((a&0x7fffffffu)==0u && (b&0x7fffffffu)==0u)return 0;if((a^b)&0x80000000u)return (a&0x80000000u)==0u;return (a&0x80000000u)!=0u?a<b:a>b;}
static void cmp(uint32_t *sr,uint32_t *fpscr,int condition){*fpscr&=~0x3f000u;*sr=(*sr&~1u)|(uint32_t)condition;}
static int convert(uint32_t raw,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t *out){
 *fpul=raw;*sr=(*sr&~1u)|(raw<0x80000000u);if(sa_sh4_float_i32(raw,fpscr,out)!=0)return -1;
 if(raw>=0x80000000u){uint32_t adjust=0x4f800000u;*fpul=adjust;if(sa_sh4_add_sub(out,&adjust,fpscr,out,0)!=0)return -1;}return 0;
}
/* 8c07a23e..8c07a2c4; output lower/upper indices, key address and next PC. */
SA_STAGE_EXPORT int sa_owner_model_third_select(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t previous,uint32_t frame,uint32_t *out){
 uint32_t keys=rd(ram,previous+8u),last=rd(ram,previous+20u)-1u,upper=0u,lower=0u,time;
 wr(ram,frame+4u,last);if(convert(rd(ram,keys),sr,fpscr,fpul,&time)!=0)return -1;
 cmp(sr,fpscr,gt(rd(ram,config+16u),time));
 if(*sr&1u){do{
  *sr=(*sr&~1u)|(upper>=rd(ram,frame+4u));
  if(*sr&1u){upper=0u;lower=rd(ram,frame+4u);break;}
  lower=upper;upper++;if(convert(rd(ram,keys+(upper<<4)),sr,fpscr,fpul,&time)!=0)return -1;
  cmp(sr,fpscr,gt(rd(ram,config+16u),time));
 }while(*sr&1u);}
 uint32_t key=keys+(lower<<4),buffer=rd(ram,frame+8u);*sr=(*sr&~1u)|(upper==lower);wr(ram,frame+4u,key);
 wr(ram,buffer+4u,rd(ram,key+4u));wr(ram,buffer+8u,rd(ram,key+8u));wr(ram,buffer+12u,rd(ram,key+12u));
 out[0]=lower;out[1]=upper;out[2]=key;out[3]=upper==lower?0x8c07a36cu:0x8c07a2c4u;return 0;
}
