#include "owner_helper_repeat.h"
#include "sh4_add_sub.h"
#include "sh4_fmac.h"
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
int sa_owner_helper_repeat(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t index,uint32_t state,uint32_t *r0){
 if(state!=3u && state!=9u && state!=10u && state!=12u)return -1;
 uint32_t descriptor=rd(ram,config+32u)+((index&0xffffu)<<4),model=rd(ram,descriptor);
 uint32_t value=rd(ram,config+16u),step=rd(ram,descriptor+12u),updated;
 if(state==12u){updated=value;}
 else if(state==3u){if(sa_sh4_add_sub(&value,&step,fpscr,&updated,0)!=0)return -1;}
 else{uint32_t rate=rd(ram,rd(ram,config+(state==9u?24u:28u)));if(sa_sh4_fmac(&step,&rate,&value,fpscr,&updated)!=0)return -1;}
 wr(ram,config+16u,updated);
 uint32_t span;if(period(ram,sr,fpscr,fpul,model,&span)!=0)return -1;
 cmp(sr,fpscr,gt(span,rd(ram,config+16u)));
 while((*sr&1u)==0u){
  if(period(ram,sr,fpscr,fpul,model,&span)!=0)return -1;
  value=rd(ram,config+16u);if(sa_sh4_add_sub(&value,&span,fpscr,&updated,1)!=0)return -1;wr(ram,config+16u,updated);
  if(period(ram,sr,fpscr,fpul,model,&span)!=0)return -1;
  cmp(sr,fpscr,gt(span,rd(ram,config+16u)));
 }
 cmp(sr,fpscr,gt(0u,rd(ram,config+16u)));
 while((*sr&1u)!=0u){
  if(period(ram,sr,fpscr,fpul,model,&span)!=0)return -1;
  value=rd(ram,config+16u);if(sa_sh4_add_sub(&value,&span,fpscr,&updated,0)!=0)return -1;wr(ram,config+16u,updated);
  cmp(sr,fpscr,gt(0u,rd(ram,config+16u)));
 }
 *r0=16u;return 0;
}
