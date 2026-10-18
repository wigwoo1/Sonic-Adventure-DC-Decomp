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
SA_STAGE_EXPORT int sa_matrix_rotate_yxz(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,void *ctx,instruction vector){
 if(f->fpscr&0x180000u||!vector)return -1;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;(void)take;
 goto L8c639ff8;
L8c639ff8: sa_motion_publish_stop(ctx,0x8c639ff8u); *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c639ffa;
L8c639ffa: sa_motion_publish_stop(ctx,0x8c639ffau); if(!(*sr&1u))goto L8c63a07e; /* bf 0x8c63a07e */
 goto L8c639ffc;
L8c639ffc: sa_motion_publish_stop(ctx,0x8c639ffcu); *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c639ffe;
L8c639ffe: sa_motion_publish_stop(ctx,0x8c63a000u); take=(*sr&1u);*fpul=r[6];if(take)goto L8c63a026; /* bt/s 0x8c63a026 */
 goto L8c63a002;
L8c63a002: sa_motion_publish_stop(ctx,0x8c63a002u); fr[3]=0; /* fldi0 fr3 */
 goto L8c63a004;
L8c63a004: sa_motion_publish_stop(ctx,0x8c63a004u); status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a006;
L8c63a006: sa_motion_publish_stop(ctx,0x8c63a006u); fr[5]=0; /* fldi0 fr5 */
 goto L8c63a008;
L8c63a008: sa_motion_publish_stop(ctx,0x8c63a008u); fr[7]=0; /* fldi0 fr7 */
 goto L8c63a00a;
L8c63a00a: sa_motion_publish_stop(ctx,0x8c63a00au); move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a00c;
L8c63a00c: sa_motion_publish_stop(ctx,0x8c63a00cu); move(f,0,1); /* fmov fr1,fr0 */
 goto L8c63a00e;
L8c63a00e: sa_motion_publish_stop(ctx,0x8c63a00eu); fr[1]=0; /* fldi0 fr1 */
 goto L8c63a010;
L8c63a010: sa_motion_publish_stop(ctx,0x8c63a010u); move(f,6,0); /* fmov fr0,fr6 */
 goto L8c63a012;
L8c63a012: sa_motion_publish_stop(ctx,0x8c63a012u); move(f,4,2); /* fmov fr2,fr4 */
 goto L8c63a014;
L8c63a014: sa_motion_publish_stop(ctx,0x8c63a014u); fr[2]^=0x80000000u; /* fneg fr2 */
 goto L8c63a016;
L8c63a016: sa_motion_publish_stop(ctx,0x8c63a016u); status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a018;
L8c63a018: sa_motion_publish_stop(ctx,0x8c63a018u); status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a01a;
L8c63a01a: sa_motion_publish_stop(ctx,0x8c63a01au); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a01c;
L8c63a01c: sa_motion_publish_stop(ctx,0x8c63a01cu); move(f,1,0); /* fmov fr0,fr1 */
 goto L8c63a01e;
L8c63a01e: sa_motion_publish_stop(ctx,0x8c63a01eu); move(f,3,2); /* fmov fr2,fr3 */
 goto L8c63a020;
L8c63a020: sa_motion_publish_stop(ctx,0x8c63a020u); move(f,9,4); /* fmov fr4,fr9 */
 goto L8c63a022;
L8c63a022: sa_motion_publish_stop(ctx,0x8c63a022u); move(f,11,6); /* fmov fr6,fr11 */
 goto L8c63a024;
L8c63a024: sa_motion_publish_stop(ctx,0x8c63a024u); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a026;
L8c63a026: sa_motion_publish_stop(ctx,0x8c63a026u); *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c63a028;
L8c63a028: sa_motion_publish_stop(ctx,0x8c63a02au); take=(*sr&1u);*fpul=r[5];if(take)goto L8c63a04e; /* bt/s 0x8c63a04e */
 goto L8c63a02c;
L8c63a02c: sa_motion_publish_stop(ctx,0x8c63a02cu); fr[3]=0; /* fldi0 fr3 */
 goto L8c63a02e;
L8c63a02e: sa_motion_publish_stop(ctx,0x8c63a02eu); status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a030;
L8c63a030: sa_motion_publish_stop(ctx,0x8c63a030u); fr[4]=0; /* fldi0 fr4 */
 goto L8c63a032;
L8c63a032: sa_motion_publish_stop(ctx,0x8c63a032u); fr[7]=0; /* fldi0 fr7 */
 goto L8c63a034;
L8c63a034: sa_motion_publish_stop(ctx,0x8c63a034u); move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a036;
L8c63a036: sa_motion_publish_stop(ctx,0x8c63a036u); fr[0]=0; /* fldi0 fr0 */
 goto L8c63a038;
L8c63a038: sa_motion_publish_stop(ctx,0x8c63a038u); move(f,5,2); /* fmov fr2,fr5 */
 goto L8c63a03a;
L8c63a03a: sa_motion_publish_stop(ctx,0x8c63a03au); move(f,6,1); /* fmov fr1,fr6 */
 goto L8c63a03c;
L8c63a03c: sa_motion_publish_stop(ctx,0x8c63a03cu); fr[5]^=0x80000000u; /* fneg fr5 */
 goto L8c63a03e;
L8c63a03e: sa_motion_publish_stop(ctx,0x8c63a03eu); status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a040;
L8c63a040: sa_motion_publish_stop(ctx,0x8c63a040u); status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a042;
L8c63a042: sa_motion_publish_stop(ctx,0x8c63a042u); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a044;
L8c63a044: sa_motion_publish_stop(ctx,0x8c63a044u); move(f,5,0); /* fmov fr0,fr5 */
 goto L8c63a046;
L8c63a046: sa_motion_publish_stop(ctx,0x8c63a046u); move(f,7,2); /* fmov fr2,fr7 */
 goto L8c63a048;
L8c63a048: sa_motion_publish_stop(ctx,0x8c63a048u); move(f,9,4); /* fmov fr4,fr9 */
 goto L8c63a04a;
L8c63a04a: sa_motion_publish_stop(ctx,0x8c63a04au); move(f,11,6); /* fmov fr6,fr11 */
 goto L8c63a04c;
L8c63a04c: sa_motion_publish_stop(ctx,0x8c63a04cu); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a04e;
L8c63a04e: sa_motion_publish_stop(ctx,0x8c63a04eu); *sr=(*sr&~1u)|!(r[7]&r[7]); /* tst r7,r7 */
 goto L8c63a050;
L8c63a050: sa_motion_publish_stop(ctx,0x8c63a052u); take=(*sr&1u);*fpul=r[7];if(take)goto L8c63a07a; /* bt/s 0x8c63a07a */
 goto L8c63a054;
L8c63a054: sa_motion_publish_stop(ctx,0x8c63a054u); fr[3]=0; /* fldi0 fr3 */
 goto L8c63a056;
L8c63a056: sa_motion_publish_stop(ctx,0x8c63a056u); status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a058;
L8c63a058: sa_motion_publish_stop(ctx,0x8c63a058u); fr[6]=0; /* fldi0 fr6 */
 goto L8c63a05a;
L8c63a05a: sa_motion_publish_stop(ctx,0x8c63a05au); fr[7]=0; /* fldi0 fr7 */
 goto L8c63a05c;
L8c63a05c: sa_motion_publish_stop(ctx,0x8c63a05cu); move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a05e;
L8c63a05e: sa_motion_publish_stop(ctx,0x8c63a05eu); move(f,0,1); /* fmov fr1,fr0 */
 goto L8c63a060;
L8c63a060: sa_motion_publish_stop(ctx,0x8c63a060u); move(f,1,2); /* fmov fr2,fr1 */
 goto L8c63a062;
L8c63a062: sa_motion_publish_stop(ctx,0x8c63a062u); fr[2]=0; /* fldi0 fr2 */
 goto L8c63a064;
L8c63a064: sa_motion_publish_stop(ctx,0x8c63a064u); move(f,5,0); /* fmov fr0,fr5 */
 goto L8c63a066;
L8c63a066: sa_motion_publish_stop(ctx,0x8c63a066u); move(f,4,1); /* fmov fr1,fr4 */
 goto L8c63a068;
L8c63a068: sa_motion_publish_stop(ctx,0x8c63a068u); fr[4]^=0x80000000u; /* fneg fr4 */
 goto L8c63a06a;
L8c63a06a: sa_motion_publish_stop(ctx,0x8c63a06au); status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a06c;
L8c63a06c: sa_motion_publish_stop(ctx,0x8c63a06cu); status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a06e;
L8c63a06e: sa_motion_publish_stop(ctx,0x8c63a06eu); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a070;
L8c63a070: sa_motion_publish_stop(ctx,0x8c63a070u); move(f,1,0); /* fmov fr0,fr1 */
 goto L8c63a072;
L8c63a072: sa_motion_publish_stop(ctx,0x8c63a072u); move(f,3,2); /* fmov fr2,fr3 */
 goto L8c63a074;
L8c63a074: sa_motion_publish_stop(ctx,0x8c63a074u); move(f,5,4); /* fmov fr4,fr5 */
 goto L8c63a076;
L8c63a076: sa_motion_publish_stop(ctx,0x8c63a076u); move(f,7,6); /* fmov fr6,fr7 */
 goto L8c63a078;
L8c63a078: sa_motion_publish_stop(ctx,0x8c63a078u); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a07a;
L8c63a07a: sa_motion_publish_stop(ctx,0x8c63a07cu); return 0; /* rts */
L8c63a07e: sa_motion_publish_stop(ctx,0x8c63a07eu); *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c63a080;
L8c63a080: sa_motion_publish_stop(ctx,0x8c63a082u); take=(*sr&1u);*fpul=r[6];if(take)goto L8c63a0ec; /* bt/s 0x8c63a0ec */
 goto L8c63a084;
L8c63a084: sa_motion_publish_stop(ctx,0x8c63a084u); fr[3]=0; /* fldi0 fr3 */
 goto L8c63a086;
L8c63a086: sa_motion_publish_stop(ctx,0x8c63a086u); status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a088;
L8c63a088: sa_motion_publish_stop(ctx,0x8c63a088u); fr[5]=0; /* fldi0 fr5 */
 goto L8c63a08a;
L8c63a08a: sa_motion_publish_stop(ctx,0x8c63a08au); fr[7]=0; /* fldi0 fr7 */
 goto L8c63a08c;
L8c63a08c: sa_motion_publish_stop(ctx,0x8c63a08cu); move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a08e;
L8c63a08e: sa_motion_publish_stop(ctx,0x8c63a08eu); move(f,0,1); /* fmov fr1,fr0 */
 goto L8c63a090;
L8c63a090: sa_motion_publish_stop(ctx,0x8c63a090u); fr[1]=0; /* fldi0 fr1 */
 goto L8c63a092;
L8c63a092: sa_motion_publish_stop(ctx,0x8c63a092u); move(f,6,0); /* fmov fr0,fr6 */
 goto L8c63a094;
L8c63a094: sa_motion_publish_stop(ctx,0x8c63a094u); move(f,4,2); /* fmov fr2,fr4 */
 goto L8c63a096;
L8c63a096: sa_motion_publish_stop(ctx,0x8c63a096u); fr[2]^=0x80000000u; /* fneg fr2 */
 goto L8c63a098;
L8c63a098: sa_motion_publish_stop(ctx,0x8c63a098u); r[15]-=4;wr(m,r[15],fr[12]); /* fmov.s fr12,@-r15 */
 goto L8c63a09a;
L8c63a09a: sa_motion_publish_stop(ctx,0x8c63a09au); r[15]-=4;wr(m,r[15],fr[13]); /* fmov.s fr13,@-r15 */
 goto L8c63a09c;
L8c63a09c: sa_motion_publish_stop(ctx,0x8c63a09cu); r[15]-=4;wr(m,r[15],fr[14]); /* fmov.s fr14,@-r15 */
 goto L8c63a09e;
L8c63a09e: sa_motion_publish_stop(ctx,0x8c63a09eu); r[15]-=4;wr(m,r[15],fr[15]); /* fmov.s fr15,@-r15 */
 goto L8c63a0a0;
L8c63a0a0: sa_motion_publish_stop(ctx,0x8c63a0a0u); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a0a2;
L8c63a0a2: sa_motion_publish_stop(ctx,0x8c63a0a2u); move(f,8,1); /* fmov fr1,fr8 */
 goto L8c63a0a4;
L8c63a0a4: sa_motion_publish_stop(ctx,0x8c63a0a4u); move(f,10,3); /* fmov fr3,fr10 */
 goto L8c63a0a6;
L8c63a0a6: sa_motion_publish_stop(ctx,0x8c63a0a6u); move(f,12,9); /* fmov fr9,fr12 */
 goto L8c63a0a8;
L8c63a0a8: sa_motion_publish_stop(ctx,0x8c63a0a8u); move(f,14,11); /* fmov fr11,fr14 */
 goto L8c63a0aa;
L8c63a0aa: sa_motion_publish_stop(ctx,0x8c63a0aau); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a0ac;
L8c63a0ac: sa_motion_publish_stop(ctx,0x8c63a0acu); f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a0ae;
L8c63a0ae: sa_motion_publish_stop(ctx,0x8c63a0aeu); fr[0]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr0 */
 goto L8c63a0b0;
L8c63a0b0: sa_motion_publish_stop(ctx,0x8c63a0b0u); fr[1]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr1 */
 goto L8c63a0b2;
L8c63a0b2: sa_motion_publish_stop(ctx,0x8c63a0b2u); fr[2]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr2 */
 goto L8c63a0b4;
L8c63a0b4: sa_motion_publish_stop(ctx,0x8c63a0b4u); fr[3]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr3 */
 goto L8c63a0b6;
L8c63a0b6: sa_motion_publish_stop(ctx,0x8c63a0b6u); r[4]+=16u; /* add #0x10,r4 */
 goto L8c63a0b8;
L8c63a0b8: sa_motion_publish_stop(ctx,0x8c63a0b8u); fr[8]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr8 */
 goto L8c63a0ba;
L8c63a0ba: sa_motion_publish_stop(ctx,0x8c63a0bau); fr[9]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr9 */
 goto L8c63a0bc;
L8c63a0bc: sa_motion_publish_stop(ctx,0x8c63a0bcu); fr[10]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr10 */
 goto L8c63a0be;
L8c63a0be: sa_motion_publish_stop(ctx,0x8c63a0beu); fr[11]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr11 */
 goto L8c63a0c0;
L8c63a0c0: sa_motion_publish_stop(ctx,0x8c63a0c0u); f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a0c2;
L8c63a0c2: sa_motion_publish_stop(ctx,0x8c63a0c2u); status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a0c4;
L8c63a0c4: sa_motion_publish_stop(ctx,0x8c63a0c4u); status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a0c6;
L8c63a0c6: sa_motion_publish_stop(ctx,0x8c63a0c6u); r[4]-=4;wr(m,r[4],fr[7]); /* fmov.s fr7,@-r4 */
 goto L8c63a0c8;
L8c63a0c8: sa_motion_publish_stop(ctx,0x8c63a0c8u); r[4]-=4;wr(m,r[4],fr[6]); /* fmov.s fr6,@-r4 */
 goto L8c63a0ca;
L8c63a0ca: sa_motion_publish_stop(ctx,0x8c63a0cau); r[4]-=4;wr(m,r[4],fr[5]); /* fmov.s fr5,@-r4 */
 goto L8c63a0cc;
L8c63a0cc: sa_motion_publish_stop(ctx,0x8c63a0ccu); r[4]-=4;wr(m,r[4],fr[4]); /* fmov.s fr4,@-r4 */
 goto L8c63a0ce;
L8c63a0ce: sa_motion_publish_stop(ctx,0x8c63a0ceu); r[4]+=4294967280u; /* add #-0x10,r4 */
 goto L8c63a0d0;
L8c63a0d0: sa_motion_publish_stop(ctx,0x8c63a0d0u); r[4]-=4;wr(m,r[4],fr[3]); /* fmov.s fr3,@-r4 */
 goto L8c63a0d2;
L8c63a0d2: sa_motion_publish_stop(ctx,0x8c63a0d2u); r[4]-=4;wr(m,r[4],fr[2]); /* fmov.s fr2,@-r4 */
 goto L8c63a0d4;
L8c63a0d4: sa_motion_publish_stop(ctx,0x8c63a0d4u); r[4]-=4;wr(m,r[4],fr[1]); /* fmov.s fr1,@-r4 */
 goto L8c63a0d6;
L8c63a0d6: sa_motion_publish_stop(ctx,0x8c63a0d6u); r[4]-=4;wr(m,r[4],fr[0]); /* fmov.s fr0,@-r4 */
 goto L8c63a0d8;
L8c63a0d8: sa_motion_publish_stop(ctx,0x8c63a0d8u); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a0da;
L8c63a0da: sa_motion_publish_stop(ctx,0x8c63a0dau); move(f,1,8); /* fmov fr8,fr1 */
 goto L8c63a0dc;
L8c63a0dc: sa_motion_publish_stop(ctx,0x8c63a0dcu); move(f,3,10); /* fmov fr10,fr3 */
 goto L8c63a0de;
L8c63a0de: sa_motion_publish_stop(ctx,0x8c63a0deu); move(f,9,12); /* fmov fr12,fr9 */
 goto L8c63a0e0;
L8c63a0e0: sa_motion_publish_stop(ctx,0x8c63a0e0u); move(f,11,14); /* fmov fr14,fr11 */
 goto L8c63a0e2;
L8c63a0e2: sa_motion_publish_stop(ctx,0x8c63a0e2u); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a0e4;
L8c63a0e4: sa_motion_publish_stop(ctx,0x8c63a0e4u); fr[15]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr15 */
 goto L8c63a0e6;
L8c63a0e6: sa_motion_publish_stop(ctx,0x8c63a0e6u); fr[14]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr14 */
 goto L8c63a0e8;
L8c63a0e8: sa_motion_publish_stop(ctx,0x8c63a0e8u); fr[13]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr13 */
 goto L8c63a0ea;
L8c63a0ea: sa_motion_publish_stop(ctx,0x8c63a0eau); fr[12]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr12 */
 goto L8c63a0ec;
L8c63a0ec: sa_motion_publish_stop(ctx,0x8c63a0ecu); *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c63a0ee;
L8c63a0ee: sa_motion_publish_stop(ctx,0x8c63a0f0u); take=(*sr&1u);*fpul=r[5];if(take)goto L8c63a158; /* bt/s 0x8c63a158 */
 goto L8c63a0f2;
L8c63a0f2: sa_motion_publish_stop(ctx,0x8c63a0f2u); fr[3]=0; /* fldi0 fr3 */
 goto L8c63a0f4;
L8c63a0f4: sa_motion_publish_stop(ctx,0x8c63a0f4u); status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a0f6;
L8c63a0f6: sa_motion_publish_stop(ctx,0x8c63a0f6u); fr[4]=0; /* fldi0 fr4 */
 goto L8c63a0f8;
L8c63a0f8: sa_motion_publish_stop(ctx,0x8c63a0f8u); fr[7]=0; /* fldi0 fr7 */
 goto L8c63a0fa;
L8c63a0fa: sa_motion_publish_stop(ctx,0x8c63a0fau); move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a0fc;
L8c63a0fc: sa_motion_publish_stop(ctx,0x8c63a0fcu); fr[0]=0; /* fldi0 fr0 */
 goto L8c63a0fe;
L8c63a0fe: sa_motion_publish_stop(ctx,0x8c63a0feu); move(f,5,2); /* fmov fr2,fr5 */
 goto L8c63a100;
L8c63a100: sa_motion_publish_stop(ctx,0x8c63a100u); move(f,6,1); /* fmov fr1,fr6 */
 goto L8c63a102;
L8c63a102: sa_motion_publish_stop(ctx,0x8c63a102u); fr[5]^=0x80000000u; /* fneg fr5 */
 goto L8c63a104;
L8c63a104: sa_motion_publish_stop(ctx,0x8c63a104u); r[15]-=4;wr(m,r[15],fr[12]); /* fmov.s fr12,@-r15 */
 goto L8c63a106;
L8c63a106: sa_motion_publish_stop(ctx,0x8c63a106u); r[15]-=4;wr(m,r[15],fr[13]); /* fmov.s fr13,@-r15 */
 goto L8c63a108;
L8c63a108: sa_motion_publish_stop(ctx,0x8c63a108u); r[15]-=4;wr(m,r[15],fr[14]); /* fmov.s fr14,@-r15 */
 goto L8c63a10a;
L8c63a10a: sa_motion_publish_stop(ctx,0x8c63a10au); r[15]-=4;wr(m,r[15],fr[15]); /* fmov.s fr15,@-r15 */
 goto L8c63a10c;
L8c63a10c: sa_motion_publish_stop(ctx,0x8c63a10cu); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a10e;
L8c63a10e: sa_motion_publish_stop(ctx,0x8c63a10eu); move(f,8,5); /* fmov fr5,fr8 */
 goto L8c63a110;
L8c63a110: sa_motion_publish_stop(ctx,0x8c63a110u); move(f,10,7); /* fmov fr7,fr10 */
 goto L8c63a112;
L8c63a112: sa_motion_publish_stop(ctx,0x8c63a112u); move(f,12,9); /* fmov fr9,fr12 */
 goto L8c63a114;
L8c63a114: sa_motion_publish_stop(ctx,0x8c63a114u); move(f,14,11); /* fmov fr11,fr14 */
 goto L8c63a116;
L8c63a116: sa_motion_publish_stop(ctx,0x8c63a116u); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a118;
L8c63a118: sa_motion_publish_stop(ctx,0x8c63a118u); f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a11a;
L8c63a11a: sa_motion_publish_stop(ctx,0x8c63a11au); r[4]+=16u; /* add #0x10,r4 */
 goto L8c63a11c;
L8c63a11c: sa_motion_publish_stop(ctx,0x8c63a11cu); fr[4]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr4 */
 goto L8c63a11e;
L8c63a11e: sa_motion_publish_stop(ctx,0x8c63a11eu); fr[5]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr5 */
 goto L8c63a120;
L8c63a120: sa_motion_publish_stop(ctx,0x8c63a120u); fr[6]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr6 */
 goto L8c63a122;
L8c63a122: sa_motion_publish_stop(ctx,0x8c63a122u); fr[7]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr7 */
 goto L8c63a124;
L8c63a124: sa_motion_publish_stop(ctx,0x8c63a124u); fr[8]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr8 */
 goto L8c63a126;
L8c63a126: sa_motion_publish_stop(ctx,0x8c63a126u); fr[9]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr9 */
 goto L8c63a128;
L8c63a128: sa_motion_publish_stop(ctx,0x8c63a128u); fr[10]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr10 */
 goto L8c63a12a;
L8c63a12a: sa_motion_publish_stop(ctx,0x8c63a12au); fr[11]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr11 */
 goto L8c63a12c;
L8c63a12c: sa_motion_publish_stop(ctx,0x8c63a12cu); f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a12e;
L8c63a12e: sa_motion_publish_stop(ctx,0x8c63a12eu); status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a130;
L8c63a130: sa_motion_publish_stop(ctx,0x8c63a130u); status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a132;
L8c63a132: sa_motion_publish_stop(ctx,0x8c63a132u); r[4]-=4;wr(m,r[4],fr[7]); /* fmov.s fr7,@-r4 */
 goto L8c63a134;
L8c63a134: sa_motion_publish_stop(ctx,0x8c63a134u); r[4]-=4;wr(m,r[4],fr[6]); /* fmov.s fr6,@-r4 */
 goto L8c63a136;
L8c63a136: sa_motion_publish_stop(ctx,0x8c63a136u); r[4]-=4;wr(m,r[4],fr[5]); /* fmov.s fr5,@-r4 */
 goto L8c63a138;
L8c63a138: sa_motion_publish_stop(ctx,0x8c63a138u); r[4]-=4;wr(m,r[4],fr[4]); /* fmov.s fr4,@-r4 */
 goto L8c63a13a;
L8c63a13a: sa_motion_publish_stop(ctx,0x8c63a13au); r[4]-=4;wr(m,r[4],fr[3]); /* fmov.s fr3,@-r4 */
 goto L8c63a13c;
L8c63a13c: sa_motion_publish_stop(ctx,0x8c63a13cu); r[4]-=4;wr(m,r[4],fr[2]); /* fmov.s fr2,@-r4 */
 goto L8c63a13e;
L8c63a13e: sa_motion_publish_stop(ctx,0x8c63a13eu); r[4]-=4;wr(m,r[4],fr[1]); /* fmov.s fr1,@-r4 */
 goto L8c63a140;
L8c63a140: sa_motion_publish_stop(ctx,0x8c63a140u); r[4]-=4;wr(m,r[4],fr[0]); /* fmov.s fr0,@-r4 */
 goto L8c63a142;
L8c63a142: sa_motion_publish_stop(ctx,0x8c63a142u); r[4]+=4294967280u; /* add #-0x10,r4 */
 goto L8c63a144;
L8c63a144: sa_motion_publish_stop(ctx,0x8c63a144u); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a146;
L8c63a146: sa_motion_publish_stop(ctx,0x8c63a146u); move(f,5,8); /* fmov fr8,fr5 */
 goto L8c63a148;
L8c63a148: sa_motion_publish_stop(ctx,0x8c63a148u); move(f,7,10); /* fmov fr10,fr7 */
 goto L8c63a14a;
L8c63a14a: sa_motion_publish_stop(ctx,0x8c63a14au); move(f,9,12); /* fmov fr12,fr9 */
 goto L8c63a14c;
L8c63a14c: sa_motion_publish_stop(ctx,0x8c63a14cu); move(f,11,14); /* fmov fr14,fr11 */
 goto L8c63a14e;
L8c63a14e: sa_motion_publish_stop(ctx,0x8c63a14eu); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a150;
L8c63a150: sa_motion_publish_stop(ctx,0x8c63a150u); fr[15]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr15 */
 goto L8c63a152;
L8c63a152: sa_motion_publish_stop(ctx,0x8c63a152u); fr[14]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr14 */
 goto L8c63a154;
L8c63a154: sa_motion_publish_stop(ctx,0x8c63a154u); fr[13]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr13 */
 goto L8c63a156;
L8c63a156: sa_motion_publish_stop(ctx,0x8c63a156u); fr[12]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr12 */
 goto L8c63a158;
L8c63a158: sa_motion_publish_stop(ctx,0x8c63a158u); *sr=(*sr&~1u)|!(r[7]&r[7]); /* tst r7,r7 */
 goto L8c63a15a;
L8c63a15a: sa_motion_publish_stop(ctx,0x8c63a15cu); take=(*sr&1u);*fpul=r[7];if(take)goto L8c63a1c4; /* bt/s 0x8c63a1c4 */
 goto L8c63a15e;
L8c63a15e: sa_motion_publish_stop(ctx,0x8c63a15eu); fr[3]=0; /* fldi0 fr3 */
 goto L8c63a160;
L8c63a160: sa_motion_publish_stop(ctx,0x8c63a160u); status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a162;
L8c63a162: sa_motion_publish_stop(ctx,0x8c63a162u); fr[6]=0; /* fldi0 fr6 */
 goto L8c63a164;
L8c63a164: sa_motion_publish_stop(ctx,0x8c63a164u); fr[7]=0; /* fldi0 fr7 */
 goto L8c63a166;
L8c63a166: sa_motion_publish_stop(ctx,0x8c63a166u); move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a168;
L8c63a168: sa_motion_publish_stop(ctx,0x8c63a168u); move(f,0,1); /* fmov fr1,fr0 */
 goto L8c63a16a;
L8c63a16a: sa_motion_publish_stop(ctx,0x8c63a16au); move(f,1,2); /* fmov fr2,fr1 */
 goto L8c63a16c;
L8c63a16c: sa_motion_publish_stop(ctx,0x8c63a16cu); fr[2]=0; /* fldi0 fr2 */
 goto L8c63a16e;
L8c63a16e: sa_motion_publish_stop(ctx,0x8c63a16eu); move(f,5,0); /* fmov fr0,fr5 */
 goto L8c63a170;
L8c63a170: sa_motion_publish_stop(ctx,0x8c63a170u); move(f,4,1); /* fmov fr1,fr4 */
 goto L8c63a172;
L8c63a172: sa_motion_publish_stop(ctx,0x8c63a172u); fr[4]^=0x80000000u; /* fneg fr4 */
 goto L8c63a174;
L8c63a174: sa_motion_publish_stop(ctx,0x8c63a174u); r[15]-=4;wr(m,r[15],fr[12]); /* fmov.s fr12,@-r15 */
 goto L8c63a176;
L8c63a176: sa_motion_publish_stop(ctx,0x8c63a176u); r[15]-=4;wr(m,r[15],fr[13]); /* fmov.s fr13,@-r15 */
 goto L8c63a178;
L8c63a178: sa_motion_publish_stop(ctx,0x8c63a178u); r[15]-=4;wr(m,r[15],fr[14]); /* fmov.s fr14,@-r15 */
 goto L8c63a17a;
L8c63a17a: sa_motion_publish_stop(ctx,0x8c63a17au); r[15]-=4;wr(m,r[15],fr[15]); /* fmov.s fr15,@-r15 */
 goto L8c63a17c;
L8c63a17c: sa_motion_publish_stop(ctx,0x8c63a17cu); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a17e;
L8c63a17e: sa_motion_publish_stop(ctx,0x8c63a17eu); move(f,8,1); /* fmov fr1,fr8 */
 goto L8c63a180;
L8c63a180: sa_motion_publish_stop(ctx,0x8c63a180u); move(f,10,3); /* fmov fr3,fr10 */
 goto L8c63a182;
L8c63a182: sa_motion_publish_stop(ctx,0x8c63a182u); move(f,12,5); /* fmov fr5,fr12 */
 goto L8c63a184;
L8c63a184: sa_motion_publish_stop(ctx,0x8c63a184u); move(f,14,7); /* fmov fr7,fr14 */
 goto L8c63a186;
L8c63a186: sa_motion_publish_stop(ctx,0x8c63a186u); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a188;
L8c63a188: sa_motion_publish_stop(ctx,0x8c63a188u); f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a18a;
L8c63a18a: sa_motion_publish_stop(ctx,0x8c63a18au); fr[0]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr0 */
 goto L8c63a18c;
L8c63a18c: sa_motion_publish_stop(ctx,0x8c63a18cu); fr[1]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr1 */
 goto L8c63a18e;
L8c63a18e: sa_motion_publish_stop(ctx,0x8c63a18eu); fr[2]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr2 */
 goto L8c63a190;
L8c63a190: sa_motion_publish_stop(ctx,0x8c63a190u); fr[3]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr3 */
 goto L8c63a192;
L8c63a192: sa_motion_publish_stop(ctx,0x8c63a192u); fr[4]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr4 */
 goto L8c63a194;
L8c63a194: sa_motion_publish_stop(ctx,0x8c63a194u); fr[5]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr5 */
 goto L8c63a196;
L8c63a196: sa_motion_publish_stop(ctx,0x8c63a196u); fr[6]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr6 */
 goto L8c63a198;
L8c63a198: sa_motion_publish_stop(ctx,0x8c63a198u); fr[7]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr7 */
 goto L8c63a19a;
L8c63a19a: sa_motion_publish_stop(ctx,0x8c63a19au); f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a19c;
L8c63a19c: sa_motion_publish_stop(ctx,0x8c63a19cu); status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a19e;
L8c63a19e: sa_motion_publish_stop(ctx,0x8c63a19eu); status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a1a0;
L8c63a1a0: sa_motion_publish_stop(ctx,0x8c63a1a0u); r[4]-=4;wr(m,r[4],fr[7]); /* fmov.s fr7,@-r4 */
 goto L8c63a1a2;
L8c63a1a2: sa_motion_publish_stop(ctx,0x8c63a1a2u); r[4]-=4;wr(m,r[4],fr[6]); /* fmov.s fr6,@-r4 */
 goto L8c63a1a4;
L8c63a1a4: sa_motion_publish_stop(ctx,0x8c63a1a4u); r[4]-=4;wr(m,r[4],fr[5]); /* fmov.s fr5,@-r4 */
 goto L8c63a1a6;
L8c63a1a6: sa_motion_publish_stop(ctx,0x8c63a1a6u); r[4]-=4;wr(m,r[4],fr[4]); /* fmov.s fr4,@-r4 */
 goto L8c63a1a8;
L8c63a1a8: sa_motion_publish_stop(ctx,0x8c63a1a8u); r[4]-=4;wr(m,r[4],fr[3]); /* fmov.s fr3,@-r4 */
 goto L8c63a1aa;
L8c63a1aa: sa_motion_publish_stop(ctx,0x8c63a1aau); r[4]-=4;wr(m,r[4],fr[2]); /* fmov.s fr2,@-r4 */
 goto L8c63a1ac;
L8c63a1ac: sa_motion_publish_stop(ctx,0x8c63a1acu); r[4]-=4;wr(m,r[4],fr[1]); /* fmov.s fr1,@-r4 */
 goto L8c63a1ae;
L8c63a1ae: sa_motion_publish_stop(ctx,0x8c63a1aeu); r[4]-=4;wr(m,r[4],fr[0]); /* fmov.s fr0,@-r4 */
 goto L8c63a1b0;
L8c63a1b0: sa_motion_publish_stop(ctx,0x8c63a1b0u); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a1b2;
L8c63a1b2: sa_motion_publish_stop(ctx,0x8c63a1b2u); move(f,1,8); /* fmov fr8,fr1 */
 goto L8c63a1b4;
L8c63a1b4: sa_motion_publish_stop(ctx,0x8c63a1b4u); move(f,3,10); /* fmov fr10,fr3 */
 goto L8c63a1b6;
L8c63a1b6: sa_motion_publish_stop(ctx,0x8c63a1b6u); move(f,5,12); /* fmov fr12,fr5 */
 goto L8c63a1b8;
L8c63a1b8: sa_motion_publish_stop(ctx,0x8c63a1b8u); move(f,7,14); /* fmov fr14,fr7 */
 goto L8c63a1ba;
L8c63a1ba: sa_motion_publish_stop(ctx,0x8c63a1bau); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a1bc;
L8c63a1bc: sa_motion_publish_stop(ctx,0x8c63a1bcu); fr[15]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr15 */
 goto L8c63a1be;
L8c63a1be: sa_motion_publish_stop(ctx,0x8c63a1beu); fr[14]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr14 */
 goto L8c63a1c0;
L8c63a1c0: sa_motion_publish_stop(ctx,0x8c63a1c0u); fr[13]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr13 */
 goto L8c63a1c2;
L8c63a1c2: sa_motion_publish_stop(ctx,0x8c63a1c2u); fr[12]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr12 */
 goto L8c63a1c4;
L8c63a1c4: sa_motion_publish_stop(ctx,0x8c63a1c6u); return 0; /* rts */
}
