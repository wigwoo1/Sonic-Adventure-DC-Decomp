#include "stage00_task.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* Original loop advance 8c07a414..8c07a456. Output r9,r10,r11,r0,next PC. */
SA_STAGE_EXPORT void sa_owner_model_change_advance(uint8_t *ram,uint32_t *sr,uint32_t config,uint32_t frame,uint32_t current,uint32_t previous,uint32_t destination,uint32_t *out){
 uint32_t cm=rd(ram,frame+24u),pm=rd(ram,frame+20u);
 destination+=(cm==3u||pm==3u)?24u:16u;
 current+=cm==3u?24u:16u;previous+=pm==3u?24u:16u;
 uint32_t index=rd(ram,frame+40u)+1u;wr(ram,frame+40u,index);
 uint32_t count=at(ram,rd(ram,config+32u)+rd(ram,frame+44u))[4];
 *sr=(*sr&~1u)|(index>=count);
 out[0]=current;out[1]=previous;out[2]=destination;out[3]=count;out[4]=index>=count?0x8c07a456u:0x8c079d26u;
}
