#include "sa_draw_motion_stop.h"
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
L8c639f5c: sa_motion_publish_stop(ctx,0x8c639f5cu); *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c639f5e;
L8c639f5e: sa_motion_publish_stop(ctx,0x8c639f60u); take=!(*sr&1u);*fpul=r[5];if(take)goto L8c639f88; /* bf/s 0x8c639f88 */
 goto L8c639f62;
L8c639f62: sa_motion_publish_stop(ctx,0x8c639f62u); fr[3]=0; /* fldi0 fr3 */
 goto L8c639f64;
L8c639f64: sa_motion_publish_stop(ctx,0x8c639f64u); status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c639f66;
L8c639f66: sa_motion_publish_stop(ctx,0x8c639f66u); fr[5]=0; /* fldi0 fr5 */
 goto L8c639f68;
L8c639f68: sa_motion_publish_stop(ctx,0x8c639f68u); fr[7]=0; /* fldi0 fr7 */
 goto L8c639f6a;
L8c639f6a: sa_motion_publish_stop(ctx,0x8c639f6au); move(f,2,0); /* fmov fr0,fr2 */
 goto L8c639f6c;
L8c639f6c: sa_motion_publish_stop(ctx,0x8c639f6cu); move(f,0,1); /* fmov fr1,fr0 */
 goto L8c639f6e;
L8c639f6e: sa_motion_publish_stop(ctx,0x8c639f6eu); fr[1]=0; /* fldi0 fr1 */
 goto L8c639f70;
L8c639f70: sa_motion_publish_stop(ctx,0x8c639f70u); move(f,6,0); /* fmov fr0,fr6 */
 goto L8c639f72;
L8c639f72: sa_motion_publish_stop(ctx,0x8c639f72u); move(f,4,2); /* fmov fr2,fr4 */
 goto L8c639f74;
L8c639f74: sa_motion_publish_stop(ctx,0x8c639f74u); fr[2]^=0x80000000u; /* fneg fr2 */
 goto L8c639f76;
L8c639f76: sa_motion_publish_stop(ctx,0x8c639f76u); status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c639f78;
L8c639f78: sa_motion_publish_stop(ctx,0x8c639f78u); status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c639f7a;
L8c639f7a: sa_motion_publish_stop(ctx,0x8c639f7au); f->fpscr^=0x100000u; /* fschg */
 goto L8c639f7c;
L8c639f7c: sa_motion_publish_stop(ctx,0x8c639f7cu); move(f,1,0); /* fmov fr0,fr1 */
 goto L8c639f7e;
L8c639f7e: sa_motion_publish_stop(ctx,0x8c639f7eu); move(f,3,2); /* fmov fr2,fr3 */
 goto L8c639f80;
L8c639f80: sa_motion_publish_stop(ctx,0x8c639f80u); move(f,9,4); /* fmov fr4,fr9 */
 goto L8c639f82;
L8c639f82: sa_motion_publish_stop(ctx,0x8c639f82u); move(f,11,6); /* fmov fr6,fr11 */
 goto L8c639f84;
L8c639f84: sa_motion_publish_stop(ctx,0x8c639f86u); f->fpscr^=0x100000u;return 0; /* rts */
L8c639f88: sa_motion_publish_stop(ctx,0x8c639f88u); fr[3]=0; /* fldi0 fr3 */
 goto L8c639f8a;
L8c639f8a: sa_motion_publish_stop(ctx,0x8c639f8au); status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c639f8c;
L8c639f8c: sa_motion_publish_stop(ctx,0x8c639f8cu); fr[5]=0; /* fldi0 fr5 */
 goto L8c639f8e;
L8c639f8e: sa_motion_publish_stop(ctx,0x8c639f8eu); fr[7]=0; /* fldi0 fr7 */
 goto L8c639f90;
L8c639f90: sa_motion_publish_stop(ctx,0x8c639f90u); move(f,2,0); /* fmov fr0,fr2 */
 goto L8c639f92;
L8c639f92: sa_motion_publish_stop(ctx,0x8c639f92u); move(f,0,1); /* fmov fr1,fr0 */
 goto L8c639f94;
L8c639f94: sa_motion_publish_stop(ctx,0x8c639f94u); fr[1]=0; /* fldi0 fr1 */
 goto L8c639f96;
L8c639f96: sa_motion_publish_stop(ctx,0x8c639f96u); move(f,6,0); /* fmov fr0,fr6 */
 goto L8c639f98;
L8c639f98: sa_motion_publish_stop(ctx,0x8c639f98u); move(f,4,2); /* fmov fr2,fr4 */
 goto L8c639f9a;
L8c639f9a: sa_motion_publish_stop(ctx,0x8c639f9au); fr[2]^=0x80000000u; /* fneg fr2 */
 goto L8c639f9c;
L8c639f9c: sa_motion_publish_stop(ctx,0x8c639f9cu); r[15]-=4;wr(m,r[15],fr[12]); /* fmov.s fr12,@-r15 */
 goto L8c639f9e;
L8c639f9e: sa_motion_publish_stop(ctx,0x8c639f9eu); r[15]-=4;wr(m,r[15],fr[13]); /* fmov.s fr13,@-r15 */
 goto L8c639fa0;
L8c639fa0: sa_motion_publish_stop(ctx,0x8c639fa0u); r[15]-=4;wr(m,r[15],fr[14]); /* fmov.s fr14,@-r15 */
 goto L8c639fa2;
L8c639fa2: sa_motion_publish_stop(ctx,0x8c639fa2u); r[15]-=4;wr(m,r[15],fr[15]); /* fmov.s fr15,@-r15 */
 goto L8c639fa4;
L8c639fa4: sa_motion_publish_stop(ctx,0x8c639fa4u); f->fpscr^=0x100000u; /* fschg */
 goto L8c639fa6;
L8c639fa6: sa_motion_publish_stop(ctx,0x8c639fa6u); move(f,8,1); /* fmov fr1,fr8 */
 goto L8c639fa8;
L8c639fa8: sa_motion_publish_stop(ctx,0x8c639fa8u); move(f,10,3); /* fmov fr3,fr10 */
 goto L8c639faa;
L8c639faa: sa_motion_publish_stop(ctx,0x8c639faau); move(f,12,9); /* fmov fr9,fr12 */
 goto L8c639fac;
L8c639fac: sa_motion_publish_stop(ctx,0x8c639facu); move(f,14,11); /* fmov fr11,fr14 */
 goto L8c639fae;
L8c639fae: sa_motion_publish_stop(ctx,0x8c639faeu); f->fpscr^=0x100000u; /* fschg */
 goto L8c639fb0;
L8c639fb0: sa_motion_publish_stop(ctx,0x8c639fb0u); f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c639fb2;
L8c639fb2: sa_motion_publish_stop(ctx,0x8c639fb2u); fr[0]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr0 */
 goto L8c639fb4;
L8c639fb4: sa_motion_publish_stop(ctx,0x8c639fb4u); fr[1]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr1 */
 goto L8c639fb6;
L8c639fb6: sa_motion_publish_stop(ctx,0x8c639fb6u); fr[2]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr2 */
 goto L8c639fb8;
L8c639fb8: sa_motion_publish_stop(ctx,0x8c639fb8u); fr[3]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr3 */
 goto L8c639fba;
L8c639fba: sa_motion_publish_stop(ctx,0x8c639fbau); r[4]+=16u; /* add #0x10,r4 */
 goto L8c639fbc;
L8c639fbc: sa_motion_publish_stop(ctx,0x8c639fbcu); fr[8]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr8 */
 goto L8c639fbe;
L8c639fbe: sa_motion_publish_stop(ctx,0x8c639fbeu); fr[9]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr9 */
 goto L8c639fc0;
L8c639fc0: sa_motion_publish_stop(ctx,0x8c639fc0u); fr[10]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr10 */
 goto L8c639fc2;
L8c639fc2: sa_motion_publish_stop(ctx,0x8c639fc2u); fr[11]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr11 */
 goto L8c639fc4;
L8c639fc4: sa_motion_publish_stop(ctx,0x8c639fc4u); f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c639fc6;
L8c639fc6: sa_motion_publish_stop(ctx,0x8c639fc6u); status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c639fc8;
L8c639fc8: sa_motion_publish_stop(ctx,0x8c639fc8u); status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c639fca;
L8c639fca: sa_motion_publish_stop(ctx,0x8c639fcau); r[4]-=4;wr(m,r[4],fr[7]); /* fmov.s fr7,@-r4 */
 goto L8c639fcc;
L8c639fcc: sa_motion_publish_stop(ctx,0x8c639fccu); r[4]-=4;wr(m,r[4],fr[6]); /* fmov.s fr6,@-r4 */
 goto L8c639fce;
L8c639fce: sa_motion_publish_stop(ctx,0x8c639fceu); r[4]-=4;wr(m,r[4],fr[5]); /* fmov.s fr5,@-r4 */
 goto L8c639fd0;
L8c639fd0: sa_motion_publish_stop(ctx,0x8c639fd0u); r[4]-=4;wr(m,r[4],fr[4]); /* fmov.s fr4,@-r4 */
 goto L8c639fd2;
L8c639fd2: sa_motion_publish_stop(ctx,0x8c639fd2u); r[4]+=4294967280u; /* add #-0x10,r4 */
 goto L8c639fd4;
L8c639fd4: sa_motion_publish_stop(ctx,0x8c639fd4u); r[4]-=4;wr(m,r[4],fr[3]); /* fmov.s fr3,@-r4 */
 goto L8c639fd6;
L8c639fd6: sa_motion_publish_stop(ctx,0x8c639fd6u); r[4]-=4;wr(m,r[4],fr[2]); /* fmov.s fr2,@-r4 */
 goto L8c639fd8;
L8c639fd8: sa_motion_publish_stop(ctx,0x8c639fd8u); r[4]-=4;wr(m,r[4],fr[1]); /* fmov.s fr1,@-r4 */
 goto L8c639fda;
L8c639fda: sa_motion_publish_stop(ctx,0x8c639fdau); r[4]-=4;wr(m,r[4],fr[0]); /* fmov.s fr0,@-r4 */
 goto L8c639fdc;
L8c639fdc: sa_motion_publish_stop(ctx,0x8c639fdcu); f->fpscr^=0x100000u; /* fschg */
 goto L8c639fde;
L8c639fde: sa_motion_publish_stop(ctx,0x8c639fdeu); move(f,1,8); /* fmov fr8,fr1 */
 goto L8c639fe0;
L8c639fe0: sa_motion_publish_stop(ctx,0x8c639fe0u); move(f,3,10); /* fmov fr10,fr3 */
 goto L8c639fe2;
L8c639fe2: sa_motion_publish_stop(ctx,0x8c639fe2u); move(f,9,12); /* fmov fr12,fr9 */
 goto L8c639fe4;
L8c639fe4: sa_motion_publish_stop(ctx,0x8c639fe4u); move(f,11,14); /* fmov fr14,fr11 */
 goto L8c639fe6;
L8c639fe6: sa_motion_publish_stop(ctx,0x8c639fe6u); f->fpscr^=0x100000u; /* fschg */
 goto L8c639fe8;
L8c639fe8: sa_motion_publish_stop(ctx,0x8c639fe8u); fr[15]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr15 */
 goto L8c639fea;
L8c639fea: sa_motion_publish_stop(ctx,0x8c639feau); fr[14]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr14 */
 goto L8c639fec;
L8c639fec: sa_motion_publish_stop(ctx,0x8c639fecu); fr[13]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr13 */
 goto L8c639fee;
L8c639fee: sa_motion_publish_stop(ctx,0x8c639ff0u); fr[12]=rd(m,r[15]);r[15]+=4;return 0; /* rts */
}
