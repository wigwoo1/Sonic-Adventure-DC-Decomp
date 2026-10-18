#include "owner_helper_terminal.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static void zero(uint32_t *sr,uint32_t v){*sr=(*sr&~1u)|(v==0u);}
uint32_t sa_owner_helper_terminal(uint8_t *ram,uint32_t *sr,uint32_t config,uint32_t incoming_r0,void *context,sa_owner_state0_service service){
 uint32_t result=service(context,0x8c07a49cu,1u,config,0u,incoming_r0,sr);
 uint32_t holder=rd(ram,config+40u);zero(sr,holder);
 if(holder==0u)return result;
 uint32_t inner=rd(ram,rd(ram,config+40u)+4u),payload=rd(ram,inner);zero(sr,payload);
 if(payload!=0u){
  inner=rd(ram,rd(ram,config+40u)+4u);
  result=service(context,0x8c098b42u,1u,rd(ram,inner),0u,result,sr);
  inner=rd(ram,rd(ram,config+40u)+4u);wr(ram,inner,0u);
 }
 inner=rd(ram,rd(ram,config+40u)+4u);zero(sr,inner);
 if(inner!=0u){
  result=service(context,0x8c098b42u,1u,rd(ram,rd(ram,config+40u)+4u),0u,result,sr);
  wr(ram,rd(ram,config+40u)+4u,0u);
 }
 result=service(context,0x8c098b42u,1u,rd(ram,config+40u),0u,result,sr);
 wr(ram,config+40u,0u);return result;
}
