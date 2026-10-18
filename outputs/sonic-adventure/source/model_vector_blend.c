#include "fpu_cleanup_transfer.h"
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* Type1 blending blocks: 8c07a9ee..8c07aa2e averages point components;
 * 8c07aa94..8c07aad4 sums direction components before normalization.
 * FADD then separate FMUL preserved, with original FR2/FR3/FPSCR writes. */
SA_STAGE_EXPORT int sa_model_vector_blend(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *r,int midpoint){
 if(f->fpscr&0x180000u)return -1;
 unsigned bank=(f->fpscr>>21)&1u;
 if(!midpoint)r[3]=0x8c63a94cu;
 for(unsigned i=0;i<3;i++){
  r[0]=124u+i*4u;f->bank[bank][3]=rd(ram,r[15]+r[0]);
  r[0]=136u+i*4u;f->bank[bank][2]=rd(ram,r[15]+r[0]);
  int status=sa_sh4_add_sub(&f->bank[bank][2],&f->bank[bank][3],&f->fpscr,&f->bank[bank][2],0);if(status)return status;
  if(midpoint){status=sa_sh4_multiply(&f->bank[bank][2],&f->bank[bank][15],&f->fpscr,&f->bank[bank][2]);if(status)return status;}
  else if(i==0)r[4]=136u;
  wr(ram,r[15]+r[0],f->bank[bank][2]);
 }
 return 0;
}
