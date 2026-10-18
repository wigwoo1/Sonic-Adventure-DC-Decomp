#include "fpu_cleanup_transfer.h"
typedef uint32_t (*sa_vector_service)(void *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
int sa_model_vector_blend(uint8_t *,struct sa_fpu_banks *,uint32_t *,int);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint32_t half(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static void wh(uint8_t *ram,uint32_t a,uint32_t v){at(ram,a)[0]=(uint8_t)v;at(ram,a)[1]=(uint8_t)(v>>8);}
/* Type1 branch8c07a986..8c07ab10. Exact midpoint/sum arithmetic;
 * matrix point/direction transforms and normalization remain services. */
SA_STAGE_EXPORT int sa_model_vector_type1(uint8_t *ram,struct sa_fpu_banks *f,uint32_t record,uint32_t frame,uint32_t *sr,void *ctx,sa_vector_service service,uint32_t *result){
 if(f->fpscr&0x180000u)return -1;
 wh(ram,frame,0);uint32_t count=at(ram,record)[12],r0=count;*sr=(*sr&~1u)|(count>0u);
 if(count)for(;;){
  uint32_t index=half(ram,frame),map=rd(ram,record+20u);wr(ram,frame+8u,half(ram,map+index*4u));wr(ram,frame+16u,half(ram,map+index*4u+2u));
  uint32_t slot=index*4u;wr(ram,frame+4u,slot);uint32_t payload=rd(ram,record+16u);wr(ram,frame+12u,payload+(slot+2u)*12u);*sr&=~1u;
  r0=service(ctx,0x8c638eccu,3u,rd(ram,frame+68u),payload+slot*12u,frame+136u,68u,sr);
  r0=service(ctx,0x8c638eccu,3u,rd(ram,frame+64u),rd(ram,frame+12u),frame+124u,64u,sr);
  uint32_t regs[16]={0};regs[15]=frame;int status=sa_model_vector_blend(ram,f,regs,1);if(status)return status;r0=regs[0];
  uint32_t off=rd(ram,frame+8u)*12u;wr(ram,frame+12u,off);*sr&=~1u;
  r0=service(ctx,0x8c638eccu,3u,rd(ram,frame+60u),frame+136u,rd(ram,frame+32u)+off,r0,sr);
  off=rd(ram,frame+16u)*12u;wr(ram,frame+8u,off);*sr&=~1u;
  r0=service(ctx,0x8c638eccu,3u,rd(ram,frame+56u),frame+136u,rd(ram,frame+28u)+off,r0,sr);
  *sr&=~1u;r0=service(ctx,0x8c638f28u,3u,rd(ram,frame+52u),rd(ram,record+16u)+(rd(ram,frame+4u)+1u)*12u,frame+136u,r0,sr);
  *sr&=~1u;r0=service(ctx,0x8c638f28u,3u,rd(ram,frame+48u),rd(ram,record+16u)+(rd(ram,frame+4u)+3u)*12u,frame+124u,r0,sr);
  status=sa_model_vector_blend(ram,f,regs,0);if(status)return status;r0=regs[0];
  r0=service(ctx,0x8c63a94cu,1u,frame+136u,0,0,r0,sr);
  r0=service(ctx,0x8c638f28u,3u,rd(ram,frame+44u),frame+136u,rd(ram,frame+20u)+rd(ram,frame+12u),r0,sr);
  r0=service(ctx,0x8c638f28u,3u,rd(ram,frame+40u),frame+136u,rd(ram,frame+24u)+rd(ram,frame+8u),r0,sr);
  index=half(ram,frame)+1u;wh(ram,frame,index);index&=65535u;count=at(ram,record)[12];r0=count;*sr=(*sr&~1u)|(index>=count);if(index>=count)break;
 }
 *result=r0;return 0;
}
