#include "allocation_list_remove.h"
static uint8_t *p(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *b=p(ram,a);return b[0]|((uint32_t)b[1]<<8)|((uint32_t)b[2]<<16)|((uint32_t)b[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)p(ram,a)[i]=(uint8_t)(v>>(8*i));}
uint32_t sa_allocation_list_remove_8c664b52(uint8_t *ram,uint32_t *sr,uint32_t head_address,uint32_t tail_address,uint32_t node,uint32_t incoming_r0)
{
 uint32_t previous=rd(ram,node+4u),saved_next=rd(ram,node+8u);
 if(previous==0u)wr(ram,head_address,rd(ram,node+8u));
 else wr(ram,previous+8u,rd(ram,node+8u));
 uint32_t next=rd(ram,node+8u);
 *sr=(*sr&~1u)|(next==0u);
 if(next==0u)wr(ram,tail_address,rd(ram,node+4u));
 else wr(ram,saved_next+4u,rd(ram,node+4u));
 return incoming_r0;
}
