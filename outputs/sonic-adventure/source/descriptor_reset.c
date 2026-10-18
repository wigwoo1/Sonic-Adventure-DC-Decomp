#include "descriptor_reset.h"
static uint8_t *p(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *b=p(ram,a);return b[0]|((uint32_t)b[1]<<8)|((uint32_t)b[2]<<16)|((uint32_t)b[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)p(ram,a)[i]=(uint8_t)(v>>(i*8));}
static void t(uint32_t *sr,int yes){*sr=(*sr&~1u)|(yes?1u:0u);}
uint32_t sa_descriptor_reset_8c64e720(uint8_t *ram,uint32_t *sr,uint32_t descriptor,void *ctx,uint32_t (*divide)(void *,uint32_t,uint32_t,uint32_t *))
{
 wr(ram,descriptor,0xffffffffu);wr(ram,descriptor+4u,0xffffffffu);
 for(unsigned offset=0x14u;offset<=0x34u;offset+=4u)wr(ram,descriptor+offset,0u);
 wr(ram,descriptor+0x10u,0u);p(ram,descriptor+0x40u)[0]=0;p(ram,descriptor+0x40u)[1]=0;
 uint32_t result=divide(ctx,descriptor-rd(ram,0x8c673484u),0x44u,sr);
 uint32_t index=result,count=rd(ram,0x8c88f6a0u);
 t(sr,(int32_t)count>0);
 if((int32_t)count>0){
  result=count-1u;t(sr,index==result);
  if(index==result){
   for(;;){
    count=rd(ram,0x8c88f6a0u);t(sr,(int32_t)count>0);
    if((int32_t)count<=0)break;
    result=rd(ram,rd(ram,0x8c673484u)+(count-1u)*0x44u);t(sr,result==0xffffffffu);
    if(result!=0xffffffffu)break;
    wr(ram,0x8c88f6a0u,count-1u);
   }
  }
 }
 uint32_t last=rd(ram,0x8c88f6a0u)-1u;
 t(sr,rd(ram,0x8c673d18u)>last);
 wr(ram,0x8c673d18u,(*sr&1u)?last:0u);
 return result;
}
