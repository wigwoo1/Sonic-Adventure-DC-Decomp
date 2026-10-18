#include "fpu_cleanup_transfer.h"
#include "sh4_float_i32.h"
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *ram,uint32_t a){uint8_t *p=ram+(a&0xffffffu);return p[0]|((uint32_t)p[1]<<8)|((uint32_t)p[2]<<16)|((uint32_t)p[3]<<24);}
static int convert(struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,uint32_t raw){unsigned bank=(f->fpscr>>21)&1u;*fpul=raw;*sr=(*sr&~1u)|(raw<0x80000000u);int status=sa_sh4_float_i32(raw,&f->fpscr,&f->bank[bank][3]);if(status)return status;if(raw&0x80000000u){r[2]=0x4f800000u;*fpul=r[2];f->bank[bank][2]=*fpul;return sa_sh4_add_sub(&f->bank[bank][3],&f->bank[bank][2],&f->fpscr,&f->bank[bank][3],0);}return 0;}
/* cf6/d16..d46 and f1a/f3a..f6a. Caller provides wrapping branch. */
SA_STAGE_EXPORT int sa_model_hierarchy_factor(uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,unsigned wrapping){
 if(f->fpscr&0x180000u)return -1;unsigned bank=(f->fpscr>>21)&1u;int status;
 if(wrapping){r[3]=rd(ram,r[4]);r[5]=r[8]+r[7];*fpul=r[3];*sr=(*sr&~1u)|(r[3]<0x80000000u);r[5]=rd(ram,r[5]);status=convert(f,sr,r,fpul,r[3]);if(status)return status;r[0]=16;f->bank[bank][15]=rd(ram,r[14]+r[0]);return sa_sh4_add_sub(&f->bank[bank][15],&f->bank[bank][3],&f->fpscr,&f->bank[bank][15],1);}
 r[6]=rd(ram,r[4]);status=convert(f,sr,r,fpul,r[6]);if(status)return status;r[0]=16;r[3]=rd(ram,r[5]);f->bank[bank][1]=rd(ram,r[14]+r[0]);r[3]-=r[6];status=sa_sh4_add_sub(&f->bank[bank][1],&f->bank[bank][3],&f->fpscr,&f->bank[bank][1],1);if(status)return status;status=convert(f,sr,r,fpul,r[3]);if(status)return status;f->bank[bank][15]=f->bank[bank][1];return sa_sh4_divide(&f->bank[bank][15],&f->bank[bank][3],&f->fpscr,&f->bank[bank][15]);
}
