#include "fpu_cleanup_transfer.h"
typedef int (*sa_hierarchy_walk)(void *,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t);
int sa_model_matrix_stack_push_8c639c70(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_matrix_stack_pop_8c639b98(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
int sa_model_matrix_copy(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static uint32_t half(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ram[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
/* Original8c0583a0..8c05842e hierarchy preparation wrapper. Native stack
 * and copy; recursive8c057bc0 generator remains an explicit service. */
SA_STAGE_EXPORT int sa_model_hierarchy_prepare(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t incoming_pr,void *ctx,sa_hierarchy_walk walk){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;int status;
 r[15]-=4u;wr(ram,r[15],r[14]);r[15]-=4u;wr(ram,r[15],incoming_pr);r[15]-=36u;
 r[2]=0x8c78b448u;r[3]=0;r[14]=r[5];wr(ram,r[15]+4u,r[4]);r[4]=128u;r[5]=0x8c78b44cu;r[0]=12u;wr(ram,r[15]+12u,f->bank[bank][4]);wr(ram,r[15]+8u,r[6]);wr(ram,r[15],r[7]);wr(ram,r[2],r[3]);r[3]=rd(ram,r[15]+8u)+1u;wr(ram,r[5],r[3]);*sr=(*sr&~1u)|(r[3]>r[4]);if(*sr&1u)wr(ram,r[5],r[4]);
 r[2]=0x8c639c70u;r[4]=0;status=sa_model_matrix_stack_push_8c639c70(ram,f,sr,r);if(status)return status;
 r[3]=rd(ram,r[15]+4u);*sr=(*sr&~1u)|(r[3]==0);
 if(r[3]){r[3]=0x8c63a770u;r[5]=rd(ram,r[15]+4u);r[4]=0;status=sa_model_matrix_copy(ram,f,sr,r);if(status)return status;}
 r[2]=rd(ram,r[14]+4u);r[5]=r[15]+16u;r[3]=rd(ram,r[2]);r[2]=0x8c789448u;wr(ram,r[15]+16u,r[3]);wr(ram,r[15]+20u,r[2]);r[3]=rd(ram,r[14]+4u);r[0]=half(ram,r[3]+8u);wr(ram,r[15]+24u,r[0]);r[0]=12u;r[3]=rd(ram,r[14]+4u);r[2]=rd(ram,r[3]+4u);wr(ram,r[15]+28u,r[2]);f->bank[bank][3]=rd(ram,r[15]+r[0]);r[0]=32u;wr(ram,r[15]+r[0],f->bank[bank][3]);r[4]=rd(ram,r[14]);
 status=walk(ctx,ram,f,sr,r,0x8c058406u);if(status)return status;
 r[2]=rd(ram,r[15]);*sr=(*sr&~1u)|(r[2]==0);
 if(r[2]){r[5]=rd(ram,r[15]+8u);r[3]=0x8c789448u;r[2]=0x8c63a770u;r[5]=(r[5]<<6)+r[3];r[4]=rd(ram,r[15]);status=sa_model_matrix_copy(ram,f,sr,r);if(status)return status;}
 r[3]=0x8c639b98u;r[4]=1u;status=sa_model_matrix_stack_pop_8c639b98(ram,f,sr,r);if(status)return status;
 r[0]=1u;r[15]+=40u;r[14]=rd(ram,r[15]);r[15]+=4u;return 0;
}
