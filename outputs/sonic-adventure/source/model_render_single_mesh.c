#include "fpu_cleanup_transfer.h"
int sa_model_matrix_stack_push_8c639c70(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint8_t *at(uint8_t *ram,uint32_t a){return ram+(a&0xffffffu);}
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=at(ram,a);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)at(ram,a)[i]=(uint8_t)(v>>(8*i));}
/* Single-mesh fallback8c07a8b0..8c07a93c: default point/direction and
 * native matrix-stack push; next instruction selects rendering record type. */
SA_STAGE_EXPORT int sa_model_render_single_mesh(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;
 for(unsigned i=0;i<6;i++){r[0]=144u-i*4u;wr(ram,r[15]+r[0],f->bank[bank][13]);}
 uint32_t count=at(ram,r[14])[12];r[0]=(uint32_t)(int32_t)(int8_t)count;*sr=(*sr&~1u)|(count==0);
 if(!count)r[0]=124u;
 else{r[0]=count;*sr=(*sr&~1u)|(count==1);r[0]=count==1?128u:132u;}
 wr(ram,r[15]+r[0],f->bank[bank][14]);
 r[3]=0x8c639c70u;r[0]=120u;r[4]=rd(ram,r[15]+r[0]);
 return sa_model_matrix_stack_push_8c639c70(ram,f,sr,r);
}
