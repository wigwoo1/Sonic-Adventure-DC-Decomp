#include "owner_helper_state2.h"
#include "sh4_add_sub.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
int sa_owner_helper_state2(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t config,uint32_t cached_index,void *context,sa_owner_state0_service service,uint32_t *r0){
 uint32_t offset=(cached_index&0xffffu)<<4;
 uint32_t phase=rd(ram,config+16u),step=rd(ram,rd(ram,config+32u)+offset+8u),updated;
 if(sa_sh4_add_sub(&phase,&step,fpscr,&updated,0)!=0)return -1;
 /* Compare FR15=1.0 with the updated finite/overflow result. */
 int below_one=(updated&0x80000000u)!=0u || updated<0x3f800000u;
 *fpscr&=~0x3f000u;*sr=(*sr&~1u)|(uint32_t)below_one;
 wr(ram,config+16u,updated);
 if(below_one){*r0=16u;return 0;}
 uint32_t descriptor=rd(ram,config+32u)+offset;
 uint32_t state=at(ram,descriptor)[5];if(state&0x80u)state|=0xffffff00u;
 at(ram,config)[0]=(uint8_t)state;at(ram,config)[1]=(uint8_t)(state>>8);
 *r0=service(context,0x8c07a49cu,1u,config,0u,state,sr);
 return 1;
}
