#include "owner_helper_state0.h"
uint32_t sa_owner_model_angle_finish(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t);
int sa_owner_model_angle_select(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t *);
int sa_owner_model_angle_interpolate(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
SA_STAGE_EXPORT int sa_owner_model_angle_channel(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t frame,uint32_t current,uint32_t previous,uint32_t destination,uint32_t descriptor,uint32_t incoming_r0,void *context,sa_owner_state0_service service,uint32_t *out){
 out[0]=descriptor;out[1]=incoming_r0;out[2]=0x8c07a1c4u;
 *sr=(*sr&~1u)|(rd(ram,current+4u)==0u);int allocate=(*sr&1u)==0u;
 if(!allocate){*sr=(*sr&~1u)|(rd(ram,previous+4u)==0u);allocate=(*sr&1u)==0u;}
 if(!allocate){wr(ram,destination+4u,0u);wr(ram,destination+12u,0u);return 0;}
 uint32_t buffer=service(context,0x8c098a80u,1u,32u,0u,incoming_r0,sr);out[1]=buffer;*sr=(*sr&~1u)|(buffer==0u);wr(ram,frame,buffer);
 if(buffer==0u){wr(ram,destination+4u,0u);wr(ram,destination+12u,0u);return 0;}
 wr(ram,destination+4u,buffer);wr(ram,buffer,0u);*sr=(*sr&~1u)|(rd(ram,previous+4u)==0u);
 if((*sr&1u)==0u){uint32_t selected[4];if(sa_owner_model_angle_select(ram,sr,fpscr,fpul,config,previous,frame,selected)!=0)return -1;
  if(selected[3]==0x8c079feau && sa_owner_model_angle_interpolate(ram,sr,fpscr,fpul,config,previous,frame,rd(ram,previous+4u),selected[0],selected[1],&out[1])!=0)return -1;
 }else{for(unsigned c=4u;c<=12u;c+=4u)wr(ram,rd(ram,frame)+c,rd(ram,descriptor+c+16u));}
 wr(ram,rd(ram,frame)+16u,1u);*sr=(*sr&~1u)|(rd(ram,current+4u)==0u);
 if((*sr&1u)==0u){uint32_t source=rd(ram,current+4u);for(unsigned c=4u;c<=12u;c+=4u)wr(ram,rd(ram,frame)+16u+c,rd(ram,source+c));}
 else{for(unsigned c=4u;c<=12u;c+=4u)wr(ram,rd(ram,frame)+16u+c,rd(ram,descriptor+c+16u));}
 out[1]=sa_owner_model_angle_finish(ram,sr,frame,destination,2u);return 0;
}
