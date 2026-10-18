#include "payload_metadata.h"
#include "unsigned_division.h"
static uint8_t *p(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *b=p(ram,a);return b[0]|((uint32_t)b[1]<<8)|((uint32_t)b[2]<<16)|((uint32_t)b[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)p(ram,a)[i]=(uint8_t)(v>>(8*i));}
uint32_t sa_payload_metadata_8c6652c0(uint8_t *ram,uint32_t *sr,uint32_t output,uint32_t config)
{
 static const uint32_t values[4][2][4]={
  {{0x1000u,0x80u,0x20u,0xf90u},{0x2000u,0x80u,0x20u,0xfa0u}},
  {{0x3000u,0x80u,0x40u,0xfc0u},{0x4000u,0x80u,0x60u,0xfe0u}},
  {{0x5000u,0x100u,0x100u,0x1000u},{0x6000u,0x200u,0x160u,0xf60u}},
  {{0x7000u,0x400u,0x400u,0x1000u},{0x8000u,0x800u,0x560u,0xd60u}}
 };
 uint32_t size=rd(ram,config+0xcu),index;
 switch(size){case 8:index=0;break;case 16:index=1;break;case 32:index=2;break;case 64:index=3;break;
 default:*sr&=~1u;return 0u;}
 uint32_t flag=rd(ram,config+0x18u)&1u;
 *sr=(*sr&~1u)|(flag==0u);
 for(unsigned i=0;i<4;i++)wr(ram,output+i*4u,values[index][flag][i]);
 uint32_t r1;
 uint32_t count=sa_unsigned_division_8c10cb10(ram,sr,0x800u,rd(ram,output+4u),&r1);
 wr(ram,output+0x10u,count);
 return 1u;
}
