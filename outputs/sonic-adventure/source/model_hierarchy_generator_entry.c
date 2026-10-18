#include "fpu_cleanup_transfer.h"
int sa_model_matrix_stack_push_8c639c70(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ram[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
/* Generator7bc0..7c14 entry, limit rejection through80de epilogue. next=0
 * denotes original return, otherwise7c14. Remaining channels/traversal pending. */
SA_STAGE_EXPORT int sa_model_hierarchy_generator_entry(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t pr,uint32_t *next){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;
 for(unsigned i=14;i>=8;i--){r[15]-=4u;wr(ram,r[15],r[i]);}
 r[15]-=4u;wr(ram,r[15],f->bank[bank][15]);r[15]-=4u;wr(ram,r[15],f->bank[bank][14]);r[15]-=4u;wr(ram,r[15],pr);r[15]-=24u;
 r[13]=r[4];r[4]=0x8c78b448u;r[14]=r[5];r[5]=0x8c78b44cu;r[3]=rd(ram,r[4])+1u;wr(ram,r[4],r[3]);r[2]=rd(ram,r[5]);*sr=(*sr&~1u)|(r[3]>r[2]);
 if(*sr&1u){
  r[1]=rd(ram,r[5]);wr(ram,r[4],r[1]);r[0]=0;r[15]+=28u;f->bank[bank][14]=rd(ram,r[15]);r[15]+=4u;f->bank[bank][15]=rd(ram,r[15]);r[15]+=4u;
  for(unsigned i=8;i<=14;i++){r[i]=rd(ram,r[15]);r[15]+=4u;}*next=0;return 0;
 }
 r[4]=rd(ram,r[14]+8u);r[5]=1;r[9]=0;*sr=(*sr&~1u)|(r[4]==0);r[6]=5;
 if(r[4])for(;;){r[2]=~r[5];r[9]++;r[5]<<=1;*sr=(*sr&~1u)|(r[9]>=r[6]);r[4]&=r[2];if(*sr&1u)break;*sr=(*sr&~1u)|(r[4]==0);if(*sr&1u)break;}
 r[3]=0x8c639c70u;r[4]=0;int status=sa_model_matrix_stack_push_8c639c70(ram,f,sr,r);*next=0x8c057c14u;return status;
}
