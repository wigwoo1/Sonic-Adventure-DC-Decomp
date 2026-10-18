#include "stage00_task.h"
#include "sh4_add_sub.h"
#include "sh4_float_i32.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static int convert(uint32_t raw,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t *out){
 *fpul=raw;*sr=(*sr&~1u)|(raw<0x80000000u);if(sa_sh4_float_i32(raw,fpscr,out)!=0)return -1;
 if(raw>=0x80000000u){uint32_t adjust=0x4f800000u;*fpul=adjust;if(sa_sh4_add_sub(out,&adjust,fpscr,out,0)!=0)return -1;}return 0;
}
#include "sh4_divide.h"
#include "sh4_fmac.h"
/* 8c079e10..8c079e9a, first-channel interpolation. */
SA_STAGE_EXPORT int sa_owner_model_channel_interpolate(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t previous,uint32_t frame,uint32_t keys,uint32_t lower,uint32_t upper,uint32_t *r0){
 uint32_t next=keys+(upper<<4),key=rd(ram,frame+4u);*sr=(*sr&~1u)|(upper>=lower);wr(ram,frame+16u,next);
 uint32_t duration=rd(ram,next)-rd(ram,key);if(upper<lower)duration+=rd(ram,rd(ram,frame+28u)+4u);
 uint32_t time;if(convert(rd(ram,key),sr,fpscr,fpul,&time)!=0)return -1;
 *fpul=duration;*sr=(*sr&~1u)|(duration<0x80000000u);
 uint32_t phase=rd(ram,config+16u),delta,denominator;
 if(sa_sh4_add_sub(&phase,&time,fpscr,&delta,1)!=0)return -1;
 if(sa_sh4_float_i32(duration,fpscr,&denominator)!=0)return -1;
 if(duration>=0x80000000u){uint32_t adjust=0x4f800000u;*fpul=adjust;if(sa_sh4_add_sub(&denominator,&adjust,fpscr,&denominator,0)!=0)return -1;}
 uint32_t factor;if(sa_sh4_divide(&delta,&denominator,fpscr,&factor)!=0)return -1;
 for(unsigned component=4u;component<=12u;component+=4u){
  next=rd(ram,frame+16u);uint32_t from=rd(ram,key+component),to=rd(ram,next+component),difference;
  if(sa_sh4_add_sub(&to,&from,fpscr,&difference,1)!=0)return -1;
  uint32_t buffer=rd(ram,frame+12u),base=rd(ram,buffer+component),result,rate=factor;
  if(sa_sh4_fmac(&rate,&difference,&base,fpscr,&result)!=0)return -1;wr(ram,buffer+component,result);
 }
 (void)previous;*r0=12u;return 0;
}
