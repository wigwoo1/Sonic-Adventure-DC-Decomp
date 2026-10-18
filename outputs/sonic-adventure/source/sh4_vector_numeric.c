#include "sh4_vector_numeric.h"
#include "../third_party/skmp-sh4-fpu/sh4_fpu_approx.h"

SA_STAGE_EXPORT int sa_sh4_vector_numeric(uint32_t word,struct sa_fpu_banks *f,uint32_t fpul){
 const int fsca=(word&0xf1ffu)==0xf0fdu;
 const int ftrv=(word&0xf3ffu)==0xf1fdu;
 const int fipr=(word&0xf0ffu)==0xf0edu;
 const int fsrra=(word&0xf0ffu)==0xf07du;
 if(word>0xffffu||(!fsca&&!ftrv&&!fipr&&!fsrra)||(f->fpscr&3u)>1u)return -1;
 const unsigned bank=(f->fpscr>>21)&1u;
 if(fsca){
  const unsigned n=(word>>8)&14u;
  return sh4_fsca_ex(fpul,&f->fpscr,&f->bank[bank][n],&f->bank[bank][n+1u]);
 }
 if(ftrv){
  const unsigned n=(word>>8)&12u;
  return sh4_ftrv_ex(f->bank[bank^1u],&f->bank[bank][n],&f->fpscr);
 }
 if(fipr){
  const unsigned n=(word>>8)&12u,m=((word>>8)&3u)*4u;
  return sh4_fipr_ex(&f->bank[bank][m],&f->bank[bank][n],&f->fpscr,&f->bank[bank][n+3u]);
 }
 const unsigned n=(word>>8)&15u;
 return sh4_fsrra_ex(f->bank[bank][n],&f->fpscr,&f->bank[bank][n]);
}
