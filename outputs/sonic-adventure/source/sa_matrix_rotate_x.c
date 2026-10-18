#include "fpu_cleanup_transfer.h"
typedef int (*instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
static uint32_t rd(uint8_t *m,uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
static void move(struct sa_fpu_banks *f,unsigned n,unsigned s){unsigned b=(f->fpscr>>21)&1u;
 if(!(f->fpscr&0x100000u)){f->bank[b][n]=f->bank[b][s];return;}
 unsigned sb=b^(s&1u),nb=b^(n&1u);s&=~1u;n&=~1u;uint32_t a=f->bank[sb][s],v=f->bank[sb][s+1];f->bank[nb][n]=a;f->bank[nb][n+1]=v;
}
/* Complete retail body. PR=SZ=0 at entry. FSCA/FTRV numerical service explicit. */
SA_STAGE_EXPORT int sa_matrix_rotate_x(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,void *ctx,instruction vector){
 if(f->fpscr&0x180000u||!vector)return -1;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;(void)take;
 goto L8c639ec8;
L8c639ec8: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c639eca;
L8c639eca: take=!(*sr&1u);*fpul=r[5];if(take)goto L8c639ef2; /* bf/s 0x8c639ef2 */
 goto L8c639ece;
L8c639ece: fr[3]=0; /* fldi0 fr3 */
 goto L8c639ed0;
L8c639ed0: status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c639ed2;
L8c639ed2: fr[4]=0; /* fldi0 fr4 */
 goto L8c639ed4;
L8c639ed4: fr[7]=0; /* fldi0 fr7 */
 goto L8c639ed6;
L8c639ed6: move(f,2,0); /* fmov fr0,fr2 */
 goto L8c639ed8;
L8c639ed8: fr[0]=0; /* fldi0 fr0 */
 goto L8c639eda;
L8c639eda: move(f,5,2); /* fmov fr2,fr5 */
 goto L8c639edc;
L8c639edc: move(f,6,1); /* fmov fr1,fr6 */
 goto L8c639ede;
L8c639ede: fr[5]^=0x80000000u; /* fneg fr5 */
 goto L8c639ee0;
L8c639ee0: status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c639ee2;
L8c639ee2: status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c639ee4;
L8c639ee4: f->fpscr^=0x100000u; /* fschg */
 goto L8c639ee6;
L8c639ee6: move(f,5,0); /* fmov fr0,fr5 */
 goto L8c639ee8;
L8c639ee8: move(f,7,2); /* fmov fr2,fr7 */
 goto L8c639eea;
L8c639eea: move(f,9,4); /* fmov fr4,fr9 */
 goto L8c639eec;
L8c639eec: move(f,11,6); /* fmov fr6,fr11 */
 goto L8c639eee;
L8c639eee: f->fpscr^=0x100000u;return 0; /* rts */
L8c639ef2: fr[3]=0; /* fldi0 fr3 */
 goto L8c639ef4;
L8c639ef4: status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c639ef6;
L8c639ef6: fr[4]=0; /* fldi0 fr4 */
 goto L8c639ef8;
L8c639ef8: fr[7]=0; /* fldi0 fr7 */
 goto L8c639efa;
L8c639efa: move(f,2,0); /* fmov fr0,fr2 */
 goto L8c639efc;
L8c639efc: fr[0]=0; /* fldi0 fr0 */
 goto L8c639efe;
L8c639efe: move(f,5,2); /* fmov fr2,fr5 */
 goto L8c639f00;
L8c639f00: move(f,6,1); /* fmov fr1,fr6 */
 goto L8c639f02;
L8c639f02: fr[5]^=0x80000000u; /* fneg fr5 */
 goto L8c639f04;
L8c639f04: r[15]-=4;wr(m,r[15],fr[12]); /* fmov.s fr12,@-r15 */
 goto L8c639f06;
L8c639f06: r[15]-=4;wr(m,r[15],fr[13]); /* fmov.s fr13,@-r15 */
 goto L8c639f08;
L8c639f08: r[15]-=4;wr(m,r[15],fr[14]); /* fmov.s fr14,@-r15 */
 goto L8c639f0a;
L8c639f0a: r[15]-=4;wr(m,r[15],fr[15]); /* fmov.s fr15,@-r15 */
 goto L8c639f0c;
L8c639f0c: f->fpscr^=0x100000u; /* fschg */
 goto L8c639f0e;
L8c639f0e: move(f,8,5); /* fmov fr5,fr8 */
 goto L8c639f10;
L8c639f10: move(f,10,7); /* fmov fr7,fr10 */
 goto L8c639f12;
L8c639f12: move(f,12,9); /* fmov fr9,fr12 */
 goto L8c639f14;
L8c639f14: move(f,14,11); /* fmov fr11,fr14 */
 goto L8c639f16;
L8c639f16: f->fpscr^=0x100000u; /* fschg */
 goto L8c639f18;
L8c639f18: f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c639f1a;
L8c639f1a: r[4]+=16u; /* add #0x10,r4 */
 goto L8c639f1c;
L8c639f1c: fr[4]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr4 */
 goto L8c639f1e;
L8c639f1e: fr[5]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr5 */
 goto L8c639f20;
L8c639f20: fr[6]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr6 */
 goto L8c639f22;
L8c639f22: fr[7]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr7 */
 goto L8c639f24;
L8c639f24: fr[8]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr8 */
 goto L8c639f26;
L8c639f26: fr[9]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr9 */
 goto L8c639f28;
L8c639f28: fr[10]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr10 */
 goto L8c639f2a;
L8c639f2a: fr[11]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr11 */
 goto L8c639f2c;
L8c639f2c: f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c639f2e;
L8c639f2e: status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c639f30;
L8c639f30: status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c639f32;
L8c639f32: r[4]-=4;wr(m,r[4],fr[7]); /* fmov.s fr7,@-r4 */
 goto L8c639f34;
L8c639f34: r[4]-=4;wr(m,r[4],fr[6]); /* fmov.s fr6,@-r4 */
 goto L8c639f36;
L8c639f36: r[4]-=4;wr(m,r[4],fr[5]); /* fmov.s fr5,@-r4 */
 goto L8c639f38;
L8c639f38: r[4]-=4;wr(m,r[4],fr[4]); /* fmov.s fr4,@-r4 */
 goto L8c639f3a;
L8c639f3a: r[4]-=4;wr(m,r[4],fr[3]); /* fmov.s fr3,@-r4 */
 goto L8c639f3c;
L8c639f3c: r[4]-=4;wr(m,r[4],fr[2]); /* fmov.s fr2,@-r4 */
 goto L8c639f3e;
L8c639f3e: r[4]-=4;wr(m,r[4],fr[1]); /* fmov.s fr1,@-r4 */
 goto L8c639f40;
L8c639f40: r[4]-=4;wr(m,r[4],fr[0]); /* fmov.s fr0,@-r4 */
 goto L8c639f42;
L8c639f42: f->fpscr^=0x100000u; /* fschg */
 goto L8c639f44;
L8c639f44: move(f,5,8); /* fmov fr8,fr5 */
 goto L8c639f46;
L8c639f46: move(f,7,10); /* fmov fr10,fr7 */
 goto L8c639f48;
L8c639f48: move(f,9,12); /* fmov fr12,fr9 */
 goto L8c639f4a;
L8c639f4a: move(f,11,14); /* fmov fr14,fr11 */
 goto L8c639f4c;
L8c639f4c: f->fpscr^=0x100000u; /* fschg */
 goto L8c639f4e;
L8c639f4e: fr[15]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr15 */
 goto L8c639f50;
L8c639f50: fr[14]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr14 */
 goto L8c639f52;
L8c639f52: fr[13]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr13 */
 goto L8c639f54;
L8c639f54: fr[12]=rd(m,r[15]);r[15]+=4;return 0; /* rts */
}
