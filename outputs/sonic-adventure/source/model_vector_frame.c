#include "fpu_cleanup_transfer.h"
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ram[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
/* Entry a55c..a5a0 and return aeda..aef8, original192-byte frame. */
SA_STAGE_EXPORT int sa_model_vector_frame_enter(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t pr,uint32_t *next){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;
 for(unsigned i=14;i>=8;i--){r[15]-=4u;wr(ram,r[15],r[i]);}r[0]=0xffffff6cu;
 for(unsigned i=15;i>=13;i--){r[15]-=4u;wr(ram,r[15],f->bank[bank][i]);}r[15]-=4u;wr(ram,r[15],pr);r[15]-=148u;
 r[8]=288u;r[10]=r[6];r[8]+=r[10];uint8_t *p=ram+((r[8]+2u)&0xffffffu);r[0]=(uint32_t)(int32_t)(int16_t)(p[0]|((uint16_t)p[1]<<8));r[14]=rd(ram,r[8]+36u);*sr=(*sr&~1u)|(r[0]==0);r[11]=0;if(!r[0]){*next=0x8c07a5a0u;return 0;}*sr=(*sr&~1u)|(r[0]==1);if(r[0]==1){*next=0x8c07a740u;return 0;}*sr=(*sr&~1u)|(r[0]==2);*next=0x8c07adf6u;return 0;
}
SA_STAGE_EXPORT int sa_model_vector_frame_leave(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *r){if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;r[1]=148u;r[15]+=152u;for(unsigned i=13;i<=15;i++){f->bank[bank][i]=rd(ram,r[15]);r[15]+=4u;}for(unsigned i=8;i<=14;i++){r[i]=rd(ram,r[15]);r[15]+=4u;}return 0;}
