#include "fpu_cleanup_transfer.h"
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
typedef int (*sa_vector_instruction)(void *,uint32_t,struct sa_fpu_banks *);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* 8c638f28..8c638f8c dataflow, with FIPR/FTRV/FSRRA instruction services.
 * Final FMULs use recovered native scalar arithmetic. */
SA_STAGE_EXPORT int sa_model_direction_transform(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,void *context,sa_vector_instruction vector){
 if(f->fpscr&0x180000u)return -1;uint32_t bank=(f->fpscr>>21)&1u;
 f->bank[bank][0]=rd(ram,r[5]);r[5]+=4u;*sr=(*sr&~1u)|(r[4]==0u);
 for(unsigned i=1;i<3;i++){f->bank[bank][i]=rd(ram,r[5]);r[5]+=4u;}f->bank[bank][3]=0u;
 if(*sr&1u){if(vector(context,0xf1fdu,f)!=0)return -1;}
 else{
  for(unsigned i=0;i<3;i++){r[0]=i*16u;f->bank[bank][4u+i]=rd(ram,r[4]+r[0]);}f->bank[bank][7]=0u;if(vector(context,0xf4edu,f)!=0)return -1;
  for(unsigned i=0;i<3;i++){r[0]=4u+i*16u;f->bank[bank][8u+i]=rd(ram,r[4]+r[0]);}f->bank[bank][11]=0u;if(vector(context,0xf8edu,f)!=0)return -1;f->bank[bank][10]=f->bank[bank][7];
  for(unsigned i=0;i<3;i++){r[0]=8u+i*16u;f->bank[bank][4u+i]=rd(ram,r[4]+r[0]);}f->bank[bank][7]=0u;if(vector(context,0xf4edu,f)!=0)return -1;
  f->bank[bank][0]=f->bank[bank][10];f->bank[bank][1]=f->bank[bank][11];f->bank[bank][2]=f->bank[bank][7];
 }
 f->bank[bank][3]=0u;r[3]=rd(ram,0x8c890070u);*sr=(*sr&~1u)|(r[3]==0u);r[6]+=12u;
 if(r[3]){
  if(vector(context,0xf0edu,f)!=0||vector(context,0xf37du,f)!=0)return -1;
  for(unsigned i=3;i>0;i--){uint32_t *value=&f->bank[bank][i-1u],*factor=&f->bank[bank][3];if(sa_sh4_multiply(value,factor,&f->fpscr,value)!=0)return -1;}
 }
 for(unsigned i=3;i>0;i--){r[6]-=4u;wr(ram,r[6],f->bank[bank][i-1u]);}return 0;
}
