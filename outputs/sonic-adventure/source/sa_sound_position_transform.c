#include "fpu_cleanup_transfer.h"
typedef int (*instruction)(void *,uint32_t,struct sa_fpu_banks *);
static uint32_t rd(uint8_t *m,uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
/* Retail8c02d528..8c02d53c,10 instructions. PR=SZ=0 only;
 * FTRV numerical instruction service remains explicit. */
SA_STAGE_EXPORT int sa_sound_position_transform(uint8_t *m,struct sa_fpu_banks *f,uint32_t *r,void *ctx,instruction vector){
 if((f->fpscr&0x180000u)||!vector)return -1;
 unsigned bank=(f->fpscr>>21)&1u;
 for(unsigned i=0;i<3;i++){f->bank[bank][i]=rd(m,r[4]);r[4]+=4;}
 f->bank[bank][3]=0x3f800000u;
 int result=vector(ctx,0xf1fdu,f);if(result)return result;
 if(f->fpscr&0x180000u)return -1;bank=(f->fpscr>>21)&1u;
 for(unsigned i=3;i>0;i--){r[4]-=4;wr(m,r[4],f->bank[bank][i-1]);}
 return 0;
}
