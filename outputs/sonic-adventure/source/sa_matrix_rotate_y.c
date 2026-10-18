#include "fpu_cleanup_transfer.h"
typedef int (*instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
static uint32_t rd(uint8_t *m,uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
static void move(struct sa_fpu_banks *f,unsigned n,unsigned s){unsigned b=(f->fpscr>>21)&1u;
 if(!(f->fpscr&0x100000u)){f->bank[b][n]=f->bank[b][s];return;}
 unsigned sb=b^(s&1u),nb=b^(n&1u);s&=~1u;n&=~1u;uint32_t a=f->bank[sb][s],v=f->bank[sb][s+1];f->bank[nb][n]=a;f->bank[nb][n+1]=v;
}
/* Complete retail body. PR=SZ=0 at entry. FSCA/FTRV numerical service explicit. */
SA_STAGE_EXPORT int sa_matrix_rotate_y(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,void *ctx,instruction vector){
 if(f->fpscr&0x180000u||!vector)return -1;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;(void)take;
 goto L8c639f5c;
L8c639f5c: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c639f5e;
L8c639f5e: take=!(*sr&1u);*fpul=r[5];if(take)goto L8c639f88; /* bf/s 0x8c639f88 */
 goto L8c639f62;
L8c639f62: fr[3]=0; /* fldi0 fr3 */
 goto L8c639f64;
L8c639f64: status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c639f66;
L8c639f66: fr[5]=0; /* fldi0 fr5 */
 goto L8c639f68;
L8c639f68: fr[7]=0; /* fldi0 fr7 */
 goto L8c639f6a;
L8c639f6a: move(f,2,0); /* fmov fr0,fr2 */
 goto L8c639f6c;
L8c639f6c: move(f,0,1); /* fmov fr1,fr0 */
 goto L8c639f6e;
L8c639f6e: fr[1]=0; /* fldi0 fr1 */
 goto L8c639f70;
L8c639f70: move(f,6,0); /* fmov fr0,fr6 */
 goto L8c639f72;
L8c639f72: move(f,4,2); /* fmov fr2,fr4 */
 goto L8c639f74;
L8c639f74: fr[2]^=0x80000000u; /* fneg fr2 */
 goto L8c639f76;
L8c639f76: status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c639f78;
L8c639f78: status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c639f7a;
L8c639f7a: f->fpscr^=0x100000u; /* fschg */
 goto L8c639f7c;
L8c639f7c: move(f,1,0); /* fmov fr0,fr1 */
 goto L8c639f7e;
L8c639f7e: move(f,3,2); /* fmov fr2,fr3 */
 goto L8c639f80;
L8c639f80: move(f,9,4); /* fmov fr4,fr9 */
 goto L8c639f82;
L8c639f82: move(f,11,6); /* fmov fr6,fr11 */
 goto L8c639f84;
L8c639f84: f->fpscr^=0x100000u;return 0; /* rts */
L8c639f88: fr[3]=0; /* fldi0 fr3 */
 goto L8c639f8a;
L8c639f8a: status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c639f8c;
L8c639f8c: fr[5]=0; /* fldi0 fr5 */
 goto L8c639f8e;
L8c639f8e: fr[7]=0; /* fldi0 fr7 */
 goto L8c639f90;
L8c639f90: move(f,2,0); /* fmov fr0,fr2 */
 goto L8c639f92;
L8c639f92: move(f,0,1); /* fmov fr1,fr0 */
 goto L8c639f94;
L8c639f94: fr[1]=0; /* fldi0 fr1 */
 goto L8c639f96;
L8c639f96: move(f,6,0); /* fmov fr0,fr6 */
 goto L8c639f98;
L8c639f98: move(f,4,2); /* fmov fr2,fr4 */
 goto L8c639f9a;
L8c639f9a: fr[2]^=0x80000000u; /* fneg fr2 */
 goto L8c639f9c;
L8c639f9c: r[15]-=4;wr(m,r[15],fr[12]); /* fmov.s fr12,@-r15 */
 goto L8c639f9e;
L8c639f9e: r[15]-=4;wr(m,r[15],fr[13]); /* fmov.s fr13,@-r15 */
 goto L8c639fa0;
L8c639fa0: r[15]-=4;wr(m,r[15],fr[14]); /* fmov.s fr14,@-r15 */
 goto L8c639fa2;
L8c639fa2: r[15]-=4;wr(m,r[15],fr[15]); /* fmov.s fr15,@-r15 */
 goto L8c639fa4;
L8c639fa4: f->fpscr^=0x100000u; /* fschg */
 goto L8c639fa6;
L8c639fa6: move(f,8,1); /* fmov fr1,fr8 */
 goto L8c639fa8;
L8c639fa8: move(f,10,3); /* fmov fr3,fr10 */
 goto L8c639faa;
L8c639faa: move(f,12,9); /* fmov fr9,fr12 */
 goto L8c639fac;
L8c639fac: move(f,14,11); /* fmov fr11,fr14 */
 goto L8c639fae;
L8c639fae: f->fpscr^=0x100000u; /* fschg */
 goto L8c639fb0;
L8c639fb0: f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c639fb2;
L8c639fb2: fr[0]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr0 */
 goto L8c639fb4;
L8c639fb4: fr[1]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr1 */
 goto L8c639fb6;
L8c639fb6: fr[2]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr2 */
 goto L8c639fb8;
L8c639fb8: fr[3]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr3 */
 goto L8c639fba;
L8c639fba: r[4]+=16u; /* add #0x10,r4 */
 goto L8c639fbc;
L8c639fbc: fr[8]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr8 */
 goto L8c639fbe;
L8c639fbe: fr[9]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr9 */
 goto L8c639fc0;
L8c639fc0: fr[10]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr10 */
 goto L8c639fc2;
L8c639fc2: fr[11]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr11 */
 goto L8c639fc4;
L8c639fc4: f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c639fc6;
L8c639fc6: status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c639fc8;
L8c639fc8: status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c639fca;
L8c639fca: r[4]-=4;wr(m,r[4],fr[7]); /* fmov.s fr7,@-r4 */
 goto L8c639fcc;
L8c639fcc: r[4]-=4;wr(m,r[4],fr[6]); /* fmov.s fr6,@-r4 */
 goto L8c639fce;
L8c639fce: r[4]-=4;wr(m,r[4],fr[5]); /* fmov.s fr5,@-r4 */
 goto L8c639fd0;
L8c639fd0: r[4]-=4;wr(m,r[4],fr[4]); /* fmov.s fr4,@-r4 */
 goto L8c639fd2;
L8c639fd2: r[4]+=4294967280u; /* add #-0x10,r4 */
 goto L8c639fd4;
L8c639fd4: r[4]-=4;wr(m,r[4],fr[3]); /* fmov.s fr3,@-r4 */
 goto L8c639fd6;
L8c639fd6: r[4]-=4;wr(m,r[4],fr[2]); /* fmov.s fr2,@-r4 */
 goto L8c639fd8;
L8c639fd8: r[4]-=4;wr(m,r[4],fr[1]); /* fmov.s fr1,@-r4 */
 goto L8c639fda;
L8c639fda: r[4]-=4;wr(m,r[4],fr[0]); /* fmov.s fr0,@-r4 */
 goto L8c639fdc;
L8c639fdc: f->fpscr^=0x100000u; /* fschg */
 goto L8c639fde;
L8c639fde: move(f,1,8); /* fmov fr8,fr1 */
 goto L8c639fe0;
L8c639fe0: move(f,3,10); /* fmov fr10,fr3 */
 goto L8c639fe2;
L8c639fe2: move(f,9,12); /* fmov fr12,fr9 */
 goto L8c639fe4;
L8c639fe4: move(f,11,14); /* fmov fr14,fr11 */
 goto L8c639fe6;
L8c639fe6: f->fpscr^=0x100000u; /* fschg */
 goto L8c639fe8;
L8c639fe8: fr[15]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr15 */
 goto L8c639fea;
L8c639fea: fr[14]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr14 */
 goto L8c639fec;
L8c639fec: fr[13]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr13 */
 goto L8c639fee;
L8c639fee: fr[12]=rd(m,r[15]);r[15]+=4;return 0; /* rts */
}
