#include "fpu_cleanup_transfer.h"
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
typedef int (*sa_vector_instruction)(void *,uint32_t,struct sa_fpu_banks *);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* Original8c63a94c..8c63a96c: normalize in place, FR0 returns squared
 * magnitude times reciprocal root. FIPR/FSRRA numerical services pending. */
SA_STAGE_EXPORT int sa_model_vector_normalize(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *r,void *ctx,sa_vector_instruction vector){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;
 for(unsigned i=0;i<3;i++){f->bank[bank][i]=rd(ram,r[4]);r[4]+=4u;}f->bank[bank][3]=0;
 if(vector(ctx,0xf0edu,f))return -1;
 f->bank[bank][4]=f->bank[bank][0];f->bank[bank][0]=f->bank[bank][3];
 if(vector(ctx,0xf37du,f))return -1;
 const unsigned order[3]={2,1,4};
 for(unsigned i=0;i<3;i++){unsigned n=order[i];int status=sa_sh4_multiply(&f->bank[bank][n],&f->bank[bank][3],&f->fpscr,&f->bank[bank][n]);if(status)return status;}
 for(unsigned i=0;i<3;i++){r[4]-=4u;wr(ram,r[4],f->bank[bank][order[i]]);}
 return sa_sh4_multiply(&f->bank[bank][0],&f->bank[bank][3],&f->fpscr,&f->bank[bank][0]);
}
