#include "heap_free_ram.h"
static uint8_t *p(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *b=p(ram,a);return b[0]|((uint32_t)b[1]<<8)|((uint32_t)b[2]<<16)|((uint32_t)b[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)p(ram,a)[i]=(uint8_t)(v>>(8*i));}
static int zero(uint32_t *sr,uint32_t value){*sr=(*sr&~1u)|(value==0u);return value==0u;}
uint32_t sa_heap_free_ram_8c098b42(uint8_t *ram,uint32_t *sr,uint32_t pointer,uint32_t r0)
{
 uint32_t r2,r3,r4=pointer,r5,r6,r7;
 if(zero(sr,r4))goto done;
 r5=r4-16u;r2=rd(ram,r5);
 if(zero(sr,r2))goto header;
 r3=rd(ram,r5+4u);if(zero(sr,r3))goto header;
 for(;;){} /* Original already-free corruption path deliberately spins. */
 header:
 r6=rd(ram,r5+12u);r7=0x8c79f03cu;
 int previous_zero=zero(sr,r6);r4=rd(ram,r5+8u);
 if(previous_zero)goto right;
 r2=rd(ram,r6);if(!zero(sr,r2))goto both_check;
 r3=rd(ram,r6+4u);if(zero(sr,r3))goto left_check;
 both_check:
 r3=rd(ram,r4);if(!zero(sr,r3))goto both_next;
 r3=rd(ram,r4+4u);if(zero(sr,r3))goto left_check;
 both_next:
 r3=rd(ram,r4+8u);if(zero(sr,r3))goto left_check;
 wr(ram,r4+12u,r6);wr(ram,r6+8u,r4);
 r5=rd(ram,r4);int free_next_zero=zero(sr,r5);r0=rd(ram,r4+4u);
 if(!free_next_zero)wr(ram,r5+4u,r0);
 if(!zero(sr,r0))wr(ram,r0,r5);else wr(ram,r7,r5);
 r2=rd(ram,r4+8u);wr(ram,r2+12u,r6);
 r3=rd(ram,r4+8u);wr(ram,r6+8u,r3);goto done;
 left_check:
 if(zero(sr,r6))goto right;
 r2=rd(ram,r6);if(!zero(sr,r2))goto left_merge;
 r2=rd(ram,r6+4u);if(zero(sr,r2))goto right;
 left_merge:
 wr(ram,r4+12u,r6);wr(ram,r6+8u,r4);goto done;
 right:
 r2=rd(ram,r4);if(!zero(sr,r2))goto right_next;
 r3=rd(ram,r4+4u);if(zero(sr,r3))goto insert;
 right_next:
 r3=rd(ram,r4+8u);if(zero(sr,r3))goto insert;
 r6=rd(ram,r4);r0=rd(ram,r4+4u);
 int next_zero=zero(sr,r6);wr(ram,r5,r6);wr(ram,r5+4u,r0);
 if(!next_zero){r2=rd(ram,r4);wr(ram,r2+4u,r5);}
 if(!zero(sr,r0)){r2=rd(ram,r4+4u);wr(ram,r2,r5);}else wr(ram,r7,r5);
 r2=rd(ram,r4+8u);wr(ram,r5+8u,r2);
 r3=rd(ram,r4+8u);wr(ram,r3+12u,r5);goto done;
 insert:
 r4=rd(ram,r7);int head_zero=zero(sr,r4);wr(ram,r7,r5);
 if(!head_zero)wr(ram,r4+4u,r5);
 wr(ram,r5,r4);wr(ram,r5+4u,0u);
 done:return r0;
}
