#include "owner_helper_state1.h"
int sa_owner_helper_state2_with_tail(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,void *,sa_owner_state0_service,uint32_t *);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint32_t half(uint8_t *p){return p[0]|((uint32_t)p[1]<<8);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static void sh(uint8_t *ram,uint32_t a,uint32_t v){at(ram,a)[0]=(uint8_t)v;at(ram,a)[1]=(uint8_t)(v>>8);}
int sa_owner_helper_state1(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t cached_index,void *context,sa_owner_state0_service service,uint32_t *r0){
 uint32_t offset=(cached_index&0xffffu)<<4,table=rd(ram,config+32u),entry=table+offset,step=rd(ram,entry+8u);
 if((*fpscr&0x80000u)!=0u || ((step&0x7f800000u)==0x7f800000u && (step&0x7fffffu)!=0u))return -1;
 if((step&0x7fffffffu)<0x800000u && (*fpscr&0x40000u)==0u && (step&0x7fffffffu)!=0u)return -1;
 *fpscr&=~0x3f000u;*sr=(*sr&~1u)|(step==0x3f800000u);
 uint32_t phase=0x3f800000u;
 if(step!=0x3f800000u){
  uint32_t previous=half(at(ram,config+6u)),previous_entry=table+(previous<<4);
  uint32_t current_id=rd(ram,rd(ram,entry)),previous_id=rd(ram,rd(ram,previous_entry));
  *sr=(*sr&~1u)|(current_id==previous_id);
  if(current_id==previous_id){(void)service(context,0x8c079c20u,1u,config,0u,previous_entry,sr);phase=0u;}
 }
 wr(ram,config+16u,phase);sh(ram,config+6u,cached_index);sh(ram,config,2u);
 entry=rd(ram,config+32u)+offset;sh(ram,config+10u,at(ram,entry)[6]);
 return sa_owner_helper_state2_with_tail(ram,sr,fpscr,fpul,config,cached_index,context,service,r0);
}
