#include "stage00_task.h"
typedef uint32_t (*sa_vector_service)(void *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint32_t half(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static void wh(uint8_t *ram,uint32_t a,uint32_t v){at(ram,a)[0]=(uint8_t)v;at(ram,a)[1]=(uint8_t)(v>>8);}
/* Type3 branch8c07ac0e..8c07acde, shared matrix pop. Matrix/transform
 * dependencies remain explicit services; copy load/store ordering retained. */
SA_STAGE_EXPORT int sa_model_vector_type3(uint8_t *ram,uint32_t record,uint32_t frame,uint32_t *sr,void *ctx,sa_vector_service service,uint32_t *result){
 uint32_t r0=service(ctx,0x8c639c70u,1u,rd(ram,frame+108u),0,0,108u,sr);
 r0=service(ctx,0x8c63997cu,2u,0,rd(ram,frame+112u),0,112u,sr);
 wh(ram,frame,0);uint32_t count=at(ram,record)[12];r0=count;*sr=(*sr&~1u)|(count>0u);
 if(count)for(;;){
  uint32_t index=half(ram,frame),map=rd(ram,record+20u),a=half(ram,map+index*4u),b=half(ram,map+index*4u+2u);
  wr(ram,frame+4u,a);uint32_t secondoff=b*12u;wr(ram,frame+16u,secondoff);
  uint32_t slot=index*4u;wr(ram,frame+8u,slot);uint32_t payloadOff=(slot+2u)*12u;wr(ram,frame+36u,payloadOff);
  uint32_t dst=rd(ram,frame+28u)+secondoff,src=rd(ram,record+16u)+payloadOff;
  uint32_t x=rd(ram,src),y=rd(ram,src+4u);wr(ram,dst,x);wr(ram,dst+4u,y);wr(ram,dst+8u,rd(ram,src+8u));*sr&=~1u;
  uint32_t firstoff=a*12u;wr(ram,frame+12u,firstoff);
  r0=service(ctx,0x8c638eccu,3u,0,rd(ram,record+16u)+rd(ram,frame+36u),rd(ram,frame+32u)+firstoff,y,sr);
  uint32_t normaloff=(rd(ram,frame+8u)+3u)*12u;wr(ram,frame+4u,normaloff);
  dst=rd(ram,frame+24u)+rd(ram,frame+16u);src=rd(ram,record+16u)+normaloff;
  x=rd(ram,src);y=rd(ram,src+4u);wr(ram,dst,x);wr(ram,dst+4u,y);r0=rd(ram,src+8u);wr(ram,dst+8u,r0);*sr&=~1u;
  r0=service(ctx,0x8c638f28u,3u,0,rd(ram,record+16u)+rd(ram,frame+4u),rd(ram,frame+20u)+rd(ram,frame+12u),r0,sr);
  index=half(ram,frame)+1u;wh(ram,frame,index);index&=65535u;count=at(ram,record)[12];r0=count;*sr=(*sr&~1u)|(index>=count);if(index>=count)break;
 }
 *result=service(ctx,0x8c639b98u,1u,1u,0,0,r0,sr);return 0;
}
