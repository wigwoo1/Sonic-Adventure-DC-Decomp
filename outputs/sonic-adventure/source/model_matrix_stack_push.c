#include "fpu_cleanup_transfer.h"
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+((a&0x1fffffffu)-0x0c000000u);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
static void save(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *r){wr(ram,r[5],r[0]);r[5]+=32u;wr(ram,r[5],r[0]);r[5]+=32u;f->fpscr^=0x100000u;uint32_t bank=((f->fpscr>>21)&1u)^1u;for(unsigned i=16;i>0;i-=2){r[5]-=8u;wr(ram,r[5],f->bank[bank][i-2]);wr(ram,r[5]+4u,f->bank[bank][i-1]);}f->fpscr^=0x100000u;}
/* Original 8c639c70..8c639cf0 matrix push and optional replacement, PR0/SZ0.
 * MOVCA writes preserved; host cache allocation effects not modeled. */
SA_STAGE_EXPORT int sa_model_matrix_stack_push_8c639c70(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){
 if(f->fpscr&0x180000u)return -1;
 r[1]=0x8c88f698u;r[6]=0x8c88f69cu;r[3]=rd(ram,r[1]);r[2]=rd(ram,r[6]);*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);
 if(*sr&1u){r[0]=0u;return 0;}
 r[2]++;r[1]=0x8c88f5f8u;wr(ram,r[6],r[2]);r[5]=rd(ram,r[1]);*sr=(*sr&~1u)|(r[4]==0u);
 save(ram,f,r);r[5]+=64u;wr(ram,r[1],r[5]);
 if(*sr&1u)save(ram,f,r);
 else{f->fpscr^=0x100000u;uint32_t bank=((f->fpscr>>21)&1u)^1u;for(unsigned i=0;i<16;i++){f->bank[bank][i]=rd(ram,r[4]);r[4]+=4u;}f->fpscr^=0x100000u;}
 r[0]=1u;return 0;
}
