#include "fpu_cleanup_transfer.h"
int sa_model_matrix_bank_save_8c638f94(uint8_t *,struct sa_fpu_banks *,uint32_t);
int sa_model_matrix_stack_pop_8c639b98(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
typedef int (*sa_generator_child)(void *,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ram[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
/* Original 8c0580a0..80de. Recursive child generator is an explicit service. */
SA_STAGE_EXPORT int sa_model_hierarchy_generator_tail(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,void *ctx,sa_generator_child child){
 if(f->fpscr&0x180000u)return -1;
 r[2]=0x8c638f94u;r[4]=rd(ram,r[14]+4u);int status=sa_model_matrix_bank_save_8c638f94(ram,f,r[4]);if(status)return status;
 r[3]=rd(ram,r[14]+4u)+64u;wr(ram,r[14]+4u,r[3]);r[12]=rd(ram,r[13]+44u);*sr=(*sr&~1u)|(r[12]==0);
 while(r[12]){r[5]=r[14];r[4]=r[12];if(!child)return -1;status=child(ctx,ram,f,sr,r,0x8c0580b8u);if(status)return status;r[12]=r[0];*sr=(*sr&~1u)|(r[12]==0);}
 r[3]=0x8c639b98u;r[4]=1;status=sa_model_matrix_stack_pop_8c639b98(ram,f,sr,r);if(status)return status;
 r[0]=rd(ram,r[13]+48u);r[15]+=28u;unsigned bank=(f->fpscr>>21)&1u;f->bank[bank][14]=rd(ram,r[15]);r[15]+=4u;f->bank[bank][15]=rd(ram,r[15]);r[15]+=4u;
 for(unsigned i=8;i<=14;i++){r[i]=rd(ram,r[15]);r[15]+=4u;}return 0;
}

