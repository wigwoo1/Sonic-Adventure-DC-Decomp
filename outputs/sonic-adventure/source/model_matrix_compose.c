#include "fpu_cleanup_transfer.h"
typedef int (*sa_vector_instruction)(void *,uint32_t,struct sa_fpu_banks *);
int sa_model_matrix_compose_implicit(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,void *,sa_vector_instruction);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* Original 8c63997c..8c639b48: explicit matrix composition plus implicit
 * XMTRX path. FIPR/FTRV mathematics delegated to instruction service. */
SA_STAGE_EXPORT int sa_model_matrix_compose(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,void *ctx,sa_vector_instruction vector){
 if(f->fpscr&0x180000u)return -1;
 if(!r[4])return sa_model_matrix_compose_implicit(ram,f,sr,r,ctx,vector);
 *sr&=~1u;uint32_t bank=(f->fpscr>>21)&1u;r[6]=r[5];
 for(unsigned col=0;col<4;col++){
  if(col)r[5]=r[6];
  for(unsigned row=0;row<4;row++){if(col||row)r[0]=col*4u+row*16u;else r[0]=16u;f->bank[bank][8u+row]=rd(ram,r[4]+col*4u+row*16u);}
  for(unsigned pair=0;pair<2;pair++){
   for(unsigned i=0;i<8;i++){f->bank[bank][i]=rd(ram,r[5]);r[5]+=4u;}
   if(vector(ctx,0xf2edu,f)||vector(ctx,0xf6edu,f))return -1;
   r[15]-=4u;wr(ram,r[15],f->bank[bank][3]);r[15]-=4u;wr(ram,r[15],f->bank[bank][7]);
  }
 }
 const unsigned order[8]={1,3,5,7,0,2,4,6};
 for(unsigned group=0;group<2;group++){
  for(unsigned i=0;i<8;i++){f->bank[bank][order[i]]=rd(ram,r[15]);r[15]+=4u;}
  for(unsigned row=0;row<4;row++){
   r[0]=60u-group*8u-row*16u;wr(ram,r[4]+r[0],f->bank[bank][row*2u+1u]);
   uint32_t offset=56u-group*8u-row*16u;
   if(offset)r[0]=offset;
   wr(ram,r[4]+offset,f->bank[bank][row*2u]);
  }
 }
 return 0;
}
