#include "stage00_task.h"
uint32_t sa_model_angle_unwrap_8c055d70(uint32_t *,uint32_t *);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* 8c07a0fe..8c07a19e: normalize both angle vectors and unwrap the target. */
SA_STAGE_EXPORT uint32_t sa_owner_model_angle_finish(uint8_t *ram,uint32_t *sr,uint32_t frame,uint32_t destination,uint32_t mode){
 for(unsigned bank=0u;bank<2u;bank++)for(unsigned c=4u;c<=12u;c+=4u){uint32_t buffer=rd(ram,frame);wr(ram,buffer+bank*16u+c,(uint32_t)(int32_t)(int16_t)rd(ram,buffer+bank*16u+c));}
 uint32_t result=0u;
 for(unsigned c=4u;c<=12u;c+=4u){uint32_t buffer=rd(ram,frame),pointer=buffer+16u+c;wr(ram,frame-4u,pointer);uint32_t regs[16]={0};regs[4]=rd(ram,buffer+c);regs[5]=rd(ram,buffer+16u+c);result=sa_model_angle_unwrap_8c055d70(regs,sr);wr(ram,pointer,result);}
 wr(ram,destination+12u,mode);return result;
}
