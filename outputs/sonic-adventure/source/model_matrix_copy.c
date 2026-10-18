#include "fpu_cleanup_transfer.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* Original8c63a770..8c63a7e0: null destination loads alternate FPU matrix
 * bank; otherwise two buffered32-byte copies with original overlap behavior. */
SA_STAGE_EXPORT int sa_model_matrix_copy(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;*sr=(*sr&~1u)|(r[4]==0);
 if(!r[4]){for(unsigned i=0;i<16;i++){f->bank[bank^1u][i]=rd(ram,r[5]);r[5]+=4u;}return 0;}
 r[4]+=32u;
 for(unsigned half=0;half<2;half++){
  for(unsigned i=0;i<8;i++){f->bank[bank][i]=rd(ram,r[5]);r[5]+=4u;if(half&&i==1)r[4]+=64u;}
  for(unsigned i=8;i>0;i--){r[4]-=4u;wr(ram,r[4],f->bank[bank][i-1]);}
 }
 return 0;
}
