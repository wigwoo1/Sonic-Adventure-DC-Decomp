#include "fpu_cleanup_transfer.h"
int sa_model_matrix_copy(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* Original8c058434..8c05846a indexed matrix retrieval. Callee volatile
 * registers and canonical scratch modeled; saved PR bytes outside C API. */
SA_STAGE_EXPORT int sa_model_hierarchy_matrix(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){
 if(f->fpscr&0x180000u)return -1;
 r[1]=0x8c78b448u;wr(ram,r[15]-12u,r[4]);wr(ram,r[15]-8u,r[5]);r[3]=rd(ram,r[1]);r[2]=rd(ram,r[15]-12u);*sr=(*sr&~1u)|(r[2]>=r[3]);
 if(*sr&1u){r[0]=0;return 0;}
 r[5]=rd(ram,r[15]-12u);r[3]=0x8c789448u;r[2]=0x8c63a770u;r[5]=(r[5]<<6)+r[3];r[4]=rd(ram,r[15]-8u);
 int status=sa_model_matrix_copy(ram,f,sr,r);if(status)return status;r[0]=1;return 0;
}
