#include "fpu_cleanup_transfer.h"
static uint32_t get(uint8_t *m,uint32_t a){a&=0xffffffu;uint32_t v=0;for(unsigned i=0;i<4;i++)v|=(uint32_t)m[a+i]<<(8*i);return v;}
static void put(uint8_t *m,uint32_t a,uint32_t v){a&=0xffffffu;for(unsigned i=0;i<4;i++)m[a+i]=(uint8_t)(v>>(8*i));}
/* Retail 8c1108e0..8c110950. Returns the restored continuation in *pr.
 * Caller must transfer control to that continuation, not resume its own caller.
 * special contains MACH, MACL, GBR in that order; current FMOV SZ=0 required. */
SA_STAGE_EXPORT int sa_input_context_restore(uint8_t *m,struct sa_fpu_banks *f,uint32_t *r,uint32_t *sr,uint32_t *pr,uint32_t *fpul,uint32_t *special){
 if(f->fpscr&0x100000u)return -2;
 uint32_t a=r[4],value=r[5];put(m,0x8c7ac8c4u,value);
 unsigned bank=(f->fpscr>>21)&1;for(unsigned i=0;i<16;i++)f->bank[bank][i]=get(m,a+0x50+i*4);
 *fpul=get(m,a+0x90);f->fpscr=get(m,a+0x94);
 special[2]=get(m,a+0x4c);special[1]=get(m,a+0x48);special[0]=get(m,a+0x44);
 r[1]=get(m,a+0x3c);r[2]=get(m,a+0x38);r[3]=get(m,a+0x34);
 for(unsigned i=5;i<=13;i++)r[i]=get(m,a+0x2c-(i-5)*4);
 r[14]=get(m,a+4);r[4]=get(m,a+0x30);r[15]=get(m,a);*pr=get(m,a+8);
 r[0]=get(m,0x8c7ac8c4u);*sr=(*sr&~1u)|!r[0];if(!r[0])r[0]=1;return 0;
}
