#include "fpu_cleanup_transfer.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* 8c639b98..8c639bd4. Original unsigned depth arithmetic, PR0/SZ0. */
SA_STAGE_EXPORT int sa_model_matrix_stack_pop_8c639b98(uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r){
 if(fpu->fpscr&0x180000u)return -1;
 r[5]=0x8c88f69cu;r[2]=1u;r[3]=rd(ram,r[5])-r[4];*sr=(*sr&~1u)|(r[3]>=1u);
 if(r[3]<1u){r[0]=0u;return 0;}
 r[4]<<=6u;wr(ram,r[5],r[3]);r[5]=0x8c88f5f8u;r[1]=rd(ram,r[5])-r[4];wr(ram,r[5],r[1]);
 fpu->fpscr^=0x100000u;uint32_t alternate=((fpu->fpscr>>21)&1u)^1u;
 for(unsigned i=0;i<16;i++){fpu->bank[alternate][i]=rd(ram,r[1]);r[1]+=4u;}
 fpu->fpscr^=0x100000u;r[0]=1u;return 0;
}
