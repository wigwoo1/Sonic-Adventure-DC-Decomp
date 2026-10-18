#include "stage00_task.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint32_t half(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static void copy(uint8_t *ram,uint32_t dst,uint32_t src){uint32_t a=rd(ram,src),b=rd(ram,src+4u);wr(ram,dst,a);wr(ram,dst+4u,b);wr(ram,dst+8u,rd(ram,src+8u));}
/* Internal restore loop 8c07ae16..8c07aecc, entered only for count > 0.
 * metadata arguments correspond to original r6 and r5. */
SA_STAGE_EXPORT uint32_t sa_model_vector_restore(uint8_t *ram,uint32_t *sr,uint32_t record,uint32_t first,uint32_t second,uint32_t initial_index){
 uint32_t index=initial_index,count;
 do{
  uint32_t i=index&65535u,map=rd(ram,record+20u),a=half(ram,map+i*4u),b=half(ram,map+i*4u+2u),src=rd(ram,record+16u)+i*48u;
  copy(ram,rd(ram,first)+a*12u,src);copy(ram,rd(ram,first+4u)+a*12u,rd(ram,record+16u)+i*48u+12u);
  copy(ram,rd(ram,second)+b*12u,rd(ram,record+16u)+i*48u+24u);copy(ram,rd(ram,second+4u)+b*12u,rd(ram,record+16u)+i*48u+36u);
  index++;count=at(ram,record)[12];*sr=(*sr&~1u)|((index&65535u)>=count);
 }while((*sr&1u)==0u);
 return count;
}
