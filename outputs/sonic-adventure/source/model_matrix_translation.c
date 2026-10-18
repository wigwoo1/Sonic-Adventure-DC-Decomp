#include "fpu_cleanup_transfer.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* Original 8c638fc0..8c638fe0, initial PR0/SZ0. */
SA_STAGE_EXPORT int sa_model_matrix_translation_8c638fc0(uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t *sr,uint32_t *r){
 if(fpu->fpscr&0x180000u)return -1;
 uint32_t active=(fpu->fpscr>>21)&1u;r[5]+=12u;*sr=(*sr&~1u)|(r[4]==0u);
 if(r[4]==0u){uint32_t other=active^1u;for(unsigned i=0;i<4;i++)fpu->bank[active][i]=fpu->bank[other][12u+i];}
 else{r[4]+=48u;for(unsigned i=0;i<3;i++){fpu->bank[active][i]=rd(ram,r[4]);r[4]+=4u;}}
 for(unsigned i=3;i>0;i--){r[5]-=4u;wr(ram,r[5],fpu->bank[active][i-1u]);}return 0;
}
