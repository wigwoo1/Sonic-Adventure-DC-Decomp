#include "fpu_cleanup_transfer.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* Post-determinant8c6390ba..8c639112 singular branch and nonzero routing.
 * Supported comparison: finite normal values and signed zeros, PR0/SZ0.
 * Nonzero inversion and determinant calculation remain unrecovered. */
SA_STAGE_EXPORT int sa_model_matrix_inverse_singular(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *next){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;uint32_t magnitude=f->bank[bank][0]&0x7fffffffu;
 if(magnitude>=0x7f800000u||(magnitude&&magnitude<0x800000u))return -1;
 f->bank[bank][1]=0;f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|(magnitude==0);
 if(magnitude){*next=0x8c639112u;return 0;}
 *sr=(*sr&~1u)|(r[14]==0);r[0]=0x8c6390e8u;r[4]=r[14];f->bank[bank][4]=rd(ram,r[0]);f->bank[bank][5]=f->bank[bank][4];
 if(!r[14]){for(unsigned i=0;i<16;i++)f->bank[bank^1u][i]=f->bank[bank][4u+(i&1u)];}
 else{r[4]+=64u;for(unsigned i=0;i<16;i++){r[4]-=4u;wr(ram,r[4],f->bank[bank][4]);}}
 r[0]=0;*next=0x8c6398a8u;return 0;
}
