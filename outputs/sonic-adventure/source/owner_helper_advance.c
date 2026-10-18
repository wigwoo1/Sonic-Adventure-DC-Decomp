#include "owner_helper_tail.h"
SA_STAGE_EXPORT int sa_owner_helper_transition(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t *);
#include "sh4_add_sub.h"
#include "sh4_float_i32.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static int gt(uint32_t a,uint32_t b){if((a&0x7fffffffu)==0u && (b&0x7fffffffu)==0u)return 0;if((a^b)&0x80000000u)return (a&0x80000000u)==0u;return (a&0x80000000u)!=0u?a<b:a>b;}
static void cmp(uint32_t *sr,uint32_t *fpscr,int condition){*fpscr&=~0x3f000u;*sr=(*sr&~1u)|(uint32_t)condition;}
static int period(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t model,uint32_t *out){
 uint32_t count=rd(ram,rd(ram,model+4u)+4u);*fpul=count;*sr=(*sr&~1u)|(count<0x80000000u);
 if(sa_sh4_float_i32(count,fpscr,out)!=0)return -1;
 if(count>=0x80000000u){uint32_t adjust=0x4f800000u;*fpul=adjust;if(sa_sh4_add_sub(out,&adjust,fpscr,out,0)!=0)return -1;}
 return 0;
}
SA_STAGE_EXPORT int sa_owner_helper_advance(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t index,uint32_t state,uint32_t *r0){
 if(state!=4u && state!=6u && state!=11u)return -1;
 uint32_t descriptor=rd(ram,config+32u)+((index&65535u)<<4),model=rd(ram,descriptor);
 uint32_t value=rd(ram,config+16u),step=state==11u?rd(ram,rd(ram,config+28u)):rd(ram,descriptor+12u),updated;
 if(sa_sh4_add_sub(&value,&step,fpscr,&updated,state==6u)!=0)return -1;
 wr(ram,config+16u,updated);
 if(state==6u){cmp(sr,fpscr,gt(0u,updated));if((*sr&1u)==0u){*r0=16u;return 0;}}
 else{
  uint32_t span,minus_one=0xbf800000u;
  if(period(ram,sr,fpscr,fpul,model,&span)!=0)return -1;
  if(sa_sh4_add_sub(&span,&minus_one,fpscr,&span,0)!=0)return -1;
  cmp(sr,fpscr,gt(span,updated));if((*sr&1u)!=0u){*r0=16u;return 0;}
 }
 return sa_owner_helper_transition(ram,sr,fpscr,fpul,config,index,r0);
}
