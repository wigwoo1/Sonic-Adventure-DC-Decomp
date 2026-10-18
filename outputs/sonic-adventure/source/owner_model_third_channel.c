#include "owner_helper_state0.h"
void sa_owner_model_third_begin(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,void *,sa_owner_state0_service,uint32_t *);
int sa_owner_model_third_select(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t *);
int sa_owner_model_third_interpolate(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
SA_STAGE_EXPORT int sa_owner_model_third_channel(uint8_t *ram,uint32_t *sr,uint32_t *fpscr,uint32_t *fpul,uint32_t config,uint32_t frame,uint32_t current,uint32_t previous,uint32_t destination,uint32_t descriptor,uint32_t incoming_r0,void *context,sa_owner_state0_service service,uint32_t *out){
 uint32_t start[2];(void)incoming_r0;sa_owner_model_third_begin(ram,sr,frame,current,previous,destination,context,service,start);out[0]=descriptor;out[1]=start[0];out[2]=0x8c07a414u;
 if(start[1]==0x8c07a414u)return 0;
 if(start[1]==0x8c07a40cu){wr(ram,rd(ram,frame+16u)+8u,0u);wr(ram,rd(ram,frame+16u)+20u,0u);return 0;}
 if(start[1]==0x8c07a23eu){uint32_t selected[4],old_previous=rd(ram,frame+4u),keys=rd(ram,old_previous+8u);
  if(sa_owner_model_third_select(ram,sr,fpscr,fpul,config,old_previous,frame,selected)!=0)return -1;
  if(selected[3]==0x8c07a2c4u && sa_owner_model_third_interpolate(ram,sr,fpscr,fpul,config,old_previous,frame,keys,selected[0],selected[1],&out[1])!=0)return -1;
 }else{for(unsigned c=4u;c<=12u;c+=4u)wr(ram,rd(ram,frame+8u)+c,rd(ram,descriptor+c+28u));}
 wr(ram,rd(ram,frame+8u)+16u,1u);uint32_t mode=rd(ram,frame+24u);*sr=(*sr&~1u)|(mode==3u);int has_current=0;
 if(*sr&1u){*sr=(*sr&~1u)|(rd(ram,rd(ram,frame+36u)+8u)==0u);has_current=(*sr&1u)==0u;}
 if(has_current){uint32_t source=rd(ram,rd(ram,frame+36u)+8u);for(unsigned c=4u;c<=12u;c+=4u)wr(ram,rd(ram,frame+8u)+16u+c,rd(ram,source+c));}
 else{for(unsigned c=4u;c<=12u;c+=4u)wr(ram,rd(ram,frame+8u)+16u+c,rd(ram,descriptor+c+28u));}
 wr(ram,rd(ram,frame+16u)+20u,2u);out[1]=12u;return 0;
}
