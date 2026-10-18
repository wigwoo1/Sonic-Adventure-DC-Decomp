#include "fpu_cleanup_transfer.h"
typedef int (*sa_vector_instruction)(void *,uint32_t,struct sa_fpu_banks *);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* Original point transform 8c638ecc..8c638f24 dataflow.
 * FIPR/FTRV are explicit instruction services, not scalar substitutes. */
SA_STAGE_EXPORT int sa_model_point_transform(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,void *context,sa_vector_instruction vector){
 if(f->fpscr&0x180000u)return -1;
 uint32_t bank=(f->fpscr>>21)&1u;*sr=(*sr&~1u)|(r[4]==0u);
 for(unsigned i=0;i<3;i++){f->bank[bank][i]=rd(ram,r[5]);r[5]+=4u;}f->bank[bank][3]=0x3f800000u;
 if(*sr&1u){r[6]+=12u;if(vector(context,0xf1fdu,f)!=0)return -1;for(unsigned i=3;i>0;i--){r[6]-=4u;wr(ram,r[6],f->bank[bank][i-1]);}return 0;}
 r[0]=16u;f->bank[bank][4]=rd(ram,r[4]);for(unsigned i=1;i<4;i++){f->bank[bank][4u+i]=rd(ram,r[4]+r[0]);if(i<3)r[0]+=16u;}
 if(vector(context,0xf4edu,f)!=0)return -1;
 for(unsigned i=0;i<4;i++){r[0]=4u+i*16u;f->bank[bank][8u+i]=rd(ram,r[4]+r[0]);}wr(ram,r[6],f->bank[bank][7]);
 if(vector(context,0xf8edu,f)!=0)return -1;r[6]+=4u;
 for(unsigned i=0;i<4;i++){r[0]=8u+i*16u;f->bank[bank][4u+i]=rd(ram,r[4]+r[0]);}wr(ram,r[6],f->bank[bank][11]);
 if(vector(context,0xf4edu,f)!=0)return -1;r[6]+=4u;wr(ram,r[6],f->bank[bank][7]);return 0;
}
