#include "owner_helper_state0.h"
void sa_owner_model_channel_begin(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,void *,sa_owner_state0_service,uint32_t *);
int sa_owner_model_channel_select(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t *);
int sa_owner_model_channel_interpolate(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
SA_STAGE_EXPORT int sa_owner_model_first_channel(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t frame,uint32_t current,uint32_t previous,uint32_t destination,uint32_t incoming_r0,void *context,sa_owner_state0_service service,uint32_t *out){
 sa_owner_model_channel_begin(ram,sr,frame,current,previous,destination,incoming_r0,context,service,out);
 if(out[2]==0x8c079f12u){wr(ram,destination,0u);wr(ram,destination+8u,0u);out[2]=0x8c079f18u;return 0;}
 uint32_t descriptor=out[0];
 if(out[2]==0x8c079d66u){uint32_t selected[4];if(sa_owner_model_channel_select(ram,sr,fpscr,fpul,config,previous,frame,selected)!=0)return -1;
  out[1]=12u;if(selected[3]==0x8c079e10u && sa_owner_model_channel_interpolate(ram,sr,fpscr,fpul,config,previous,frame,rd(ram,previous),selected[0],selected[1],&out[1])!=0)return -1;
 }else{for(unsigned component=4u;component<=12u;component+=4u)wr(ram,rd(ram,frame+12u)+component,rd(ram,descriptor+component+4u));out[1]=12u;}
 wr(ram,rd(ram,frame+12u)+16u,1u);*sr=(*sr&~1u)|(rd(ram,current)==0u);
 if((*sr&1u)==0u){uint32_t source=rd(ram,current);for(unsigned component=4u;component<=12u;component+=4u)wr(ram,rd(ram,frame+12u)+16u+component,rd(ram,source+component));}
 else{for(unsigned component=4u;component<=12u;component+=4u)wr(ram,rd(ram,frame+12u)+16u+component,rd(ram,descriptor+component+4u));}
 wr(ram,destination+8u,2u);out[1]=12u;out[2]=0x8c079f18u;return 0;
}
