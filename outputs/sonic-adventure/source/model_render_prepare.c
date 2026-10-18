#include "fpu_cleanup_transfer.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint32_t half(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* 8c07a740..8c07a782, prepares original hierarchy call parameters.
 * Stops before call; r8 is owner configuration, r15 working frame. */
SA_STAGE_EXPORT int sa_model_render_prepare(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){
 if(f->fpscr&0x180000u)return -1;
 uint32_t special=half(ram,r[8])==2u;*sr=(*sr&~1u)|special;
 r[0]=half(ram,r[8]+4u);r[7]=0;r[3]=rd(ram,r[8]+32u);r[6]=r[3]+r[0]*16u;
 if(special){r[6]=at(ram,r[6])[4];r[5]=rd(ram,r[8]+40u);}
 else{wr(ram,r[15],r[6]);r[0]=at(ram,r[6])[4];r[5]=rd(ram,r[15]);r[6]=r[0];r[5]=rd(ram,r[5]);}
 r[0]=16u;f->bank[(f->fpscr>>21)&1u][4]=rd(ram,r[8]+16u);r[3]=0x8c0583a0u;r[4]=0x8c67c640u;return 0;
}
