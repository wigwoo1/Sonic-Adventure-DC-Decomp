#include "owner_helper_state0.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static void flag(uint32_t *sr,int v){*sr=(*sr&~1u)|(uint32_t)v;}
/* First-channel allocation gate, 8c079d26..8c079d66. Output: descriptor,
 * return register and next PC. Sampling and interpolation follow separately. */
SA_STAGE_EXPORT void sa_owner_model_channel_begin(uint8_t *ram,uint32_t *sr,uint32_t frame,uint32_t current,uint32_t previous,uint32_t destination,uint32_t incoming_r0,void *context,sa_owner_state0_service service,uint32_t *out){
 uint32_t index=rd(ram,frame+40u);flag(sr,rd(ram,current)==0u);out[0]=rd(ram,frame+52u+(index<<2));out[1]=incoming_r0;
 if((*sr&1u)!=0u){flag(sr,rd(ram,previous)==0u);if((*sr&1u)!=0u){out[2]=0x8c079f12u;return;}}
 uint32_t buffer=service(context,0x8c098a80u,1u,32u,0u,incoming_r0,sr);out[1]=buffer;flag(sr,buffer==0u);wr(ram,frame+12u,buffer);
 if(buffer==0u){out[2]=0x8c079f12u;return;}
 wr(ram,destination,buffer);wr(ram,buffer,0u);flag(sr,rd(ram,previous)==0u);out[2]=(*sr&1u)?0x8c079e9au:0x8c079d66u;
}
