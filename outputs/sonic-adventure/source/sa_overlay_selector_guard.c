/* Support guard around the existing selector, no new recovered-routine credit.
 * Valid destinations use the unchanged verified body. For an invalid node,
 * execute its original 21-instruction prefix and stop before FMOV at 0c905572.
 * Valid bounded entity/appearance records are required; no hardware fault is
 * delivered and a failed copy is not silently completed. */
#include "fpu_cleanup_transfer.h"
struct sa_selector_cpu {uint32_t r[16],sr,pr,pc;};
int sa_overlay_face_model_selector_0c905548(uint8_t*,struct sa_selector_cpu*,struct sa_fpu_banks*);
static uint32_t rd(const uint8_t *m,uint32_t a){
 a&=0xffffffu;return (uint32_t)m[a]|((uint32_t)m[a+1]<<8)|((uint32_t)m[a+2]<<16)|((uint32_t)m[a+3]<<24);
}
SA_STAGE_EXPORT int sa_overlay_selector_guard(uint8_t *m,struct sa_selector_cpu *s,struct sa_fpu_banks *f){
 if((rd(m,s->r[5]+0x2d4u)&0x1f000000u)==0x0c000000u)
  return sa_overlay_face_model_selector_0c905548(m,s,f);
 uint32_t *r=s->r;unsigned bank=(f->fpscr>>21)&1u;int wide=(f->fpscr&0x100000u)!=0;
 r[4]=2;r[0]=0xffffffecu;r[4]<<=8;r[0]&=255;r[4]+=0x48;
 r[7]=rd(m,r[0]+r[5]);r[4]+=r[5];r[5]=0xffffff88u;r[3]=r[4];r[5]&=255;
 r[0]=0x34;r[3]+=4;r[7]=(uint32_t)(int32_t)(int8_t)m[(r[0]+r[7])&0xffffffu];r[3]+=r[5];r[2]=rd(m,r[3]);
 r[0]=0x0c9055e8u;r[3]=r[4];f->bank[bank][4]=rd(m,r[0]);if(wide)f->bank[bank][5]=rd(m,r[0]+4u);
 r[3]+=4;r[0]=0x20;r[3]+=r[5];
 s->pc=0x0c905572u;return -3;
}
