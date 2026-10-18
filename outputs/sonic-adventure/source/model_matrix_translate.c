#include "fpu_cleanup_transfer.h"
typedef int (*sa_vector_instruction)(void *,uint32_t,struct sa_fpu_banks *);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static void wr(uint8_t *ram,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)ram[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
/* Original8c63a804..8c63a874. Vector instruction arithmetic explicit. */
SA_STAGE_EXPORT int sa_model_matrix_translate(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,void *ctx,sa_vector_instruction vector){
 if(f->fpscr&0x180000u||!vector)return -1;unsigned bank=(f->fpscr>>21)&1u;
 f->bank[bank][4]=rd(ram,r[5]);r[5]+=4u;*sr=(*sr&~1u)|(r[4]==0);f->bank[bank][5]=rd(ram,r[5]);r[5]+=4u;f->bank[bank][6]=rd(ram,r[5]);r[5]+=4u;f->bank[bank][7]=0x3f800000u;
 if(!r[4]){int status=vector(ctx,0xf5fdu,f);if(status)return status;unsigned alternate=bank^1u;f->bank[alternate][12]=f->bank[bank][4];f->bank[alternate][13]=f->bank[bank][5];f->bank[alternate][14]=f->bank[bank][6];f->bank[alternate][15]=f->bank[bank][7];return 0;}
 for(unsigned row=0;row<4;row++){if(!row)r[0]=16u;else r[0]=row;f->bank[bank][0]=rd(ram,r[4]+row*4u);for(unsigned col=1;col<4;col++){r[0]=row*4u+col*16u;f->bank[bank][col]=rd(ram,r[4]+r[0]);}int status=vector(ctx,0xf1edu,f);if(status)return status;r[0]=48u+row*4u;wr(ram,r[4]+r[0],f->bank[bank][3]);}return 0;
}
