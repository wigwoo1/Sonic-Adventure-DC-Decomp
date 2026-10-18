#include "stage00_task.h"
uint32_t sa_model_descriptor_collect_8c0567b0(uint8_t *,uint32_t *,uint32_t,uint32_t,uint32_t);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint32_t half(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static void sh(uint8_t *ram,uint32_t a,uint32_t v){at(ram,a)[0]=(uint8_t)v;at(ram,a)[1]=(uint8_t)(v>>8);}
/* 8c079c90..8c079d26; frame is the original worker's allocated stack base.
 * output: r8,r9,r10,r11,r0,next PC. Middle record loop remains unrecovered. */
SA_STAGE_EXPORT void sa_owner_model_change_initialize(uint8_t *ram,uint32_t *sr,uint32_t config,uint32_t frame,uint32_t *out){
 uint32_t offset=half(ram,config+4u)<<4;sh(ram,frame+4u,half(ram,config+6u));wr(ram,frame+44u,offset);
 uint32_t model=rd(ram,rd(ram,config+32u)+offset),root=rd(ram,model);wr(ram,rd(ram,config+40u),root);
 (void)sa_model_descriptor_collect_8c0567b0(ram,sr,root,frame+52u,frame);
 offset=rd(ram,frame+44u);uint32_t table=rd(ram,config+32u);
 uint32_t current=rd(ram,rd(ram,table+offset)+4u),previous=rd(ram,rd(ram,table+(half(ram,frame+4u)<<4))+4u);
 wr(ram,frame+28u,previous);sh(ram,frame+48u,(half(ram,current+8u)&4u)?3u:2u);
 uint32_t previous_mode=(half(ram,previous+8u)&4u)?3u:2u;
 out[0]=2u;out[1]=rd(ram,current);out[2]=rd(ram,previous);out[3]=rd(ram,rd(ram,rd(ram,config+40u)+4u));
 wr(ram,frame+40u,0u);wr(ram,frame+20u,previous_mode);wr(ram,frame+24u,half(ram,frame+48u));
 uint32_t count=at(ram,rd(ram,config+32u)+rd(ram,frame+44u))[4];*sr=(*sr&~1u)|(count==0u);out[4]=count;out[5]=count==0u?0x8c07a456u:0x8c079d26u;
}
