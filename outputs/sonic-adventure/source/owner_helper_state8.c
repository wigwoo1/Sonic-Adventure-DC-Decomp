#include "owner_helper_tail.h"
#include "owner_helper_state0.h"
int sa_owner_helper_state2_with_tail(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,void *,sa_owner_state0_service,uint32_t *);
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
static uint32_t half(uint8_t *p){return p[0]|((uint32_t)p[1]<<8);}
static void sh(uint8_t *ram,uint32_t a,uint32_t v){at(ram,a)[0]=(uint8_t)v;at(ram,a)[1]=(uint8_t)(v>>8);}
SA_STAGE_EXPORT int sa_owner_helper_state8(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t index,void *context,sa_owner_state0_service service,uint32_t *r0){
 uint32_t offset=(index&65535u)<<4,table=rd(ram,config+32u),entry=table+offset,model=rd(ram,entry);
 uint32_t value=rd(ram,config+16u),step=rd(ram,entry+12u),updated,span,minus_one=0xbf800000u;
 if(sa_sh4_add_sub(&value,&step,fpscr,&updated,0)!=0)return -1;wr(ram,config+16u,updated);
 if(period(ram,sr,fpscr,fpul,model,&span)!=0)return -1;
 if(sa_sh4_add_sub(&span,&minus_one,fpscr,&span,0)!=0)return -1;
 cmp(sr,fpscr,gt(span,updated));if((*sr&1u)!=0u){*r0=16u;return 0;}
 sh(ram,config+12u,0u);table=rd(ram,config+32u);entry=table+offset;step=rd(ram,entry+8u);
 if((*fpscr&0x80000u)!=0u || ((step&0x7f800000u)==0x7f800000u && (step&0x7fffffu)!=0u))return -1;
 if((step&0x7fffffffu)<0x800000u && (*fpscr&0x40000u)==0u && (step&0x7fffffffu)!=0u)return -1;
 cmp(sr,fpscr,step==0x3f800000u);value=0x3f800000u;
 if(step!=0x3f800000u){
  uint32_t candidate=half(at(ram,config+10u)),candidate_entry=table+(candidate<<4);
  uint32_t current_id=rd(ram,rd(ram,entry)),next_id=rd(ram,rd(ram,candidate_entry));
  *sr=(*sr&~1u)|(current_id==next_id);
  if(current_id==next_id){
   index=(uint32_t)(int32_t)(int16_t)candidate;sh(ram,config+4u,index&255u);
   (void)service(context,0x8c079c20u,1u,config,0u,index&255u,sr);value=0u;
  }
 }
 wr(ram,config+16u,value);sh(ram,config+6u,index);
 entry=rd(ram,config+32u)+((index&65535u)<<4);sh(ram,config+10u,at(ram,entry)[6]);sh(ram,config,2u);
 return sa_owner_helper_state2_with_tail(ram,sr,fpscr,fpul,config,index,context,service,r0);
}
