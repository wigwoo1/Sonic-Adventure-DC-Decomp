#include "fpu_cleanup_transfer.h"
static void put(uint8_t *m,uint32_t a,uint32_t v){a&=0xffffffu;for(unsigned i=0;i<4;i++)m[a+i]=(uint8_t)(v>>(8*i));}
/* Retail 0x8c110954..0x8c1109ba. The saved PR is the continuation.
 * MACH, MACL and GBR are explicit inputs; current FMOV mode must be SZ=0. */
SA_STAGE_EXPORT int sa_input_context_save(uint8_t *m,struct sa_fpu_banks *f,uint32_t *r,uint32_t pr,uint32_t fpul,uint32_t mach,uint32_t macl,uint32_t gbr){
 if(f->fpscr&0x100000u)return -2;
 uint32_t a=r[4];put(m,a,r[15]);put(m,a+4,r[14]);r[14]=pr;put(m,a+8,r[14]);r[14]=((uint32_t)m[((a+4)&0xffffffu)])|((uint32_t)m[((a+5)&0xffffffu)]<<8)|((uint32_t)m[((a+6)&0xffffffu)]<<16)|((uint32_t)m[((a+7)&0xffffffu)]<<24);
 for(unsigned i=0;i<10;i++)put(m,a+0xc+i*4,r[13-i]);
 r[4]+=0x30;for(unsigned i=0;i<4;i++)put(m,r[4]+4+i*4,r[3-i]);
 r[0]=mach;put(m,r[4]+0x14,r[0]);r[0]=macl;put(m,r[4]+0x18,r[0]);r[0]=gbr;put(m,r[4]+0x1c,r[0]);
 r[4]+=0x5c;r[0]=f->fpscr;put(m,r[4]+8,r[0]);r[0]=fpul;put(m,r[4]+4,r[0]);
 unsigned bank=(f->fpscr>>21)&1;put(m,r[4],f->bank[bank][15]);
 for(int i=14;i>=0;i--){r[4]-=4;put(m,r[4],f->bank[bank][i]);}
 r[4]-=0x20;r[4]=a;r[0]=0;return 0;
}

