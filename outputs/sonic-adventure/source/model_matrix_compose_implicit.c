#include "fpu_cleanup_transfer.h"
typedef int (*sa_vector_instruction)(void *,uint32_t,struct sa_fpu_banks *);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* 8c63997c..8c6399e2 implicit XMTRX composition path.
 * Explicit-matrix branch and FTRV arithmetic remain pending. */
SA_STAGE_EXPORT int sa_model_matrix_compose_implicit(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,void *ctx,sa_vector_instruction vector){
 if((f->fpscr&0x180000u)||r[4])return -1;
 uint32_t bank=(f->fpscr>>21)&1u;*sr|=1u;
 for(unsigned i=0;i<8;i++){f->bank[bank][i]=rd(ram,r[5]);r[5]+=4u;}
 if(vector(ctx,0xf1fdu,f)||vector(ctx,0xf5fdu,f))return -1;
 for(unsigned i=8;i>0;i--){r[15]-=4u;wr(ram,r[15],f->bank[bank][i-1]);}
 for(unsigned i=0;i<8;i++){f->bank[bank][i]=rd(ram,r[5]);r[5]+=4u;}
 if(vector(ctx,0xf1fdu,f)||vector(ctx,0xf5fdu,f))return -1;
 /* FSCHG plus odd destination FMOV pair transfers into alternate bank. */
 for(unsigned i=8;i>0;i-=2){f->bank[bank^1u][i+6]=f->bank[bank][i-2];f->bank[bank^1u][i+7]=f->bank[bank][i-1];}
 for(unsigned i=0;i<8;i++){f->bank[bank][i]=rd(ram,r[15]);r[15]+=4u;}
 for(unsigned i=8;i>0;i-=2){f->bank[bank^1u][i-2]=f->bank[bank][i-2];f->bank[bank^1u][i-1]=f->bank[bank][i-1];}
 return 0;
}
