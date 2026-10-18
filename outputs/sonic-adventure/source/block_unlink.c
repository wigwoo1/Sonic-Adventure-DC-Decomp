#include "block_unlink.h"
static uint8_t *p(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *b=p(ram,a);return b[0]|((uint32_t)b[1]<<8)|((uint32_t)b[2]<<16)|((uint32_t)b[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)p(ram,a)[i]=(uint8_t)(v>>(8*i));}
uint32_t sa_block_unlink_8c665478(uint8_t *ram,uint32_t *sr,uint32_t head_address,uint32_t node)
{
 uint32_t current=rd(ram,head_address),predecessor=0u;
 while(current!=0u){
  uint32_t next=rd(ram,current+4u);
  if(next==node){predecessor=current;current=0u;}
  else current=next;
 }
 uint32_t is_head=rd(ram,head_address)==node;
 *sr=(*sr&~1u)|is_head;
 uint32_t next=rd(ram,node+4u);
 if(is_head)wr(ram,head_address,next);
 else wr(ram,predecessor+4u,next);
 /* The retail function assumes node membership; it leaves the removed node intact. */
 return predecessor;
}
