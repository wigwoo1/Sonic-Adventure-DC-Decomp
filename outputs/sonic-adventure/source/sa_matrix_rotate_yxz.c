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
L8c639ff8: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c639ffa;
L8c639ffa: if(!(*sr&1u))goto L8c63a07e; /* bf 0x8c63a07e */
 goto L8c639ffc;
L8c639ffc: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c639ffe;
L8c639ffe: take=(*sr&1u);*fpul=r[6];if(take)goto L8c63a026; /* bt/s 0x8c63a026 */
 goto L8c63a002;
L8c63a002: fr[3]=0; /* fldi0 fr3 */
 goto L8c63a004;
L8c63a004: status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a006;
L8c63a006: fr[5]=0; /* fldi0 fr5 */
 goto L8c63a008;
L8c63a008: fr[7]=0; /* fldi0 fr7 */
 goto L8c63a00a;
L8c63a00a: move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a00c;
L8c63a00c: move(f,0,1); /* fmov fr1,fr0 */
 goto L8c63a00e;
L8c63a00e: fr[1]=0; /* fldi0 fr1 */
 goto L8c63a010;
L8c63a010: move(f,6,0); /* fmov fr0,fr6 */
 goto L8c63a012;
L8c63a012: move(f,4,2); /* fmov fr2,fr4 */
 goto L8c63a014;
L8c63a014: fr[2]^=0x80000000u; /* fneg fr2 */
 goto L8c63a016;
L8c63a016: status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a018;
L8c63a018: status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a01a;
L8c63a01a: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a01c;
L8c63a01c: move(f,1,0); /* fmov fr0,fr1 */
 goto L8c63a01e;
L8c63a01e: move(f,3,2); /* fmov fr2,fr3 */
 goto L8c63a020;
L8c63a020: move(f,9,4); /* fmov fr4,fr9 */
 goto L8c63a022;
L8c63a022: move(f,11,6); /* fmov fr6,fr11 */
 goto L8c63a024;
L8c63a024: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a026;
L8c63a026: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c63a028;
L8c63a028: take=(*sr&1u);*fpul=r[5];if(take)goto L8c63a04e; /* bt/s 0x8c63a04e */
 goto L8c63a02c;
L8c63a02c: fr[3]=0; /* fldi0 fr3 */
 goto L8c63a02e;
L8c63a02e: status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a030;
L8c63a030: fr[4]=0; /* fldi0 fr4 */
 goto L8c63a032;
L8c63a032: fr[7]=0; /* fldi0 fr7 */
 goto L8c63a034;
L8c63a034: move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a036;
L8c63a036: fr[0]=0; /* fldi0 fr0 */
 goto L8c63a038;
L8c63a038: move(f,5,2); /* fmov fr2,fr5 */
 goto L8c63a03a;
L8c63a03a: move(f,6,1); /* fmov fr1,fr6 */
 goto L8c63a03c;
L8c63a03c: fr[5]^=0x80000000u; /* fneg fr5 */
 goto L8c63a03e;
L8c63a03e: status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a040;
L8c63a040: status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a042;
L8c63a042: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a044;
L8c63a044: move(f,5,0); /* fmov fr0,fr5 */
 goto L8c63a046;
L8c63a046: move(f,7,2); /* fmov fr2,fr7 */
 goto L8c63a048;
L8c63a048: move(f,9,4); /* fmov fr4,fr9 */
 goto L8c63a04a;
L8c63a04a: move(f,11,6); /* fmov fr6,fr11 */
 goto L8c63a04c;
L8c63a04c: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a04e;
L8c63a04e: *sr=(*sr&~1u)|!(r[7]&r[7]); /* tst r7,r7 */
 goto L8c63a050;
L8c63a050: take=(*sr&1u);*fpul=r[7];if(take)goto L8c63a07a; /* bt/s 0x8c63a07a */
 goto L8c63a054;
L8c63a054: fr[3]=0; /* fldi0 fr3 */
 goto L8c63a056;
L8c63a056: status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a058;
L8c63a058: fr[6]=0; /* fldi0 fr6 */
 goto L8c63a05a;
L8c63a05a: fr[7]=0; /* fldi0 fr7 */
 goto L8c63a05c;
L8c63a05c: move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a05e;
L8c63a05e: move(f,0,1); /* fmov fr1,fr0 */
 goto L8c63a060;
L8c63a060: move(f,1,2); /* fmov fr2,fr1 */
 goto L8c63a062;
L8c63a062: fr[2]=0; /* fldi0 fr2 */
 goto L8c63a064;
L8c63a064: move(f,5,0); /* fmov fr0,fr5 */
 goto L8c63a066;
L8c63a066: move(f,4,1); /* fmov fr1,fr4 */
 goto L8c63a068;
L8c63a068: fr[4]^=0x80000000u; /* fneg fr4 */
 goto L8c63a06a;
L8c63a06a: status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a06c;
L8c63a06c: status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a06e;
L8c63a06e: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a070;
L8c63a070: move(f,1,0); /* fmov fr0,fr1 */
 goto L8c63a072;
L8c63a072: move(f,3,2); /* fmov fr2,fr3 */
 goto L8c63a074;
L8c63a074: move(f,5,4); /* fmov fr4,fr5 */
 goto L8c63a076;
L8c63a076: move(f,7,6); /* fmov fr6,fr7 */
 goto L8c63a078;
L8c63a078: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a07a;
L8c63a07a: return 0; /* rts */
L8c63a07e: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c63a080;
L8c63a080: take=(*sr&1u);*fpul=r[6];if(take)goto L8c63a0ec; /* bt/s 0x8c63a0ec */
 goto L8c63a084;
L8c63a084: fr[3]=0; /* fldi0 fr3 */
 goto L8c63a086;
L8c63a086: status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a088;
L8c63a088: fr[5]=0; /* fldi0 fr5 */
 goto L8c63a08a;
L8c63a08a: fr[7]=0; /* fldi0 fr7 */
 goto L8c63a08c;
L8c63a08c: move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a08e;
L8c63a08e: move(f,0,1); /* fmov fr1,fr0 */
 goto L8c63a090;
L8c63a090: fr[1]=0; /* fldi0 fr1 */
 goto L8c63a092;
L8c63a092: move(f,6,0); /* fmov fr0,fr6 */
 goto L8c63a094;
L8c63a094: move(f,4,2); /* fmov fr2,fr4 */
 goto L8c63a096;
L8c63a096: fr[2]^=0x80000000u; /* fneg fr2 */
 goto L8c63a098;
L8c63a098: r[15]-=4;wr(m,r[15],fr[12]); /* fmov.s fr12,@-r15 */
 goto L8c63a09a;
L8c63a09a: r[15]-=4;wr(m,r[15],fr[13]); /* fmov.s fr13,@-r15 */
 goto L8c63a09c;
L8c63a09c: r[15]-=4;wr(m,r[15],fr[14]); /* fmov.s fr14,@-r15 */
 goto L8c63a09e;
L8c63a09e: r[15]-=4;wr(m,r[15],fr[15]); /* fmov.s fr15,@-r15 */
 goto L8c63a0a0;
L8c63a0a0: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a0a2;
L8c63a0a2: move(f,8,1); /* fmov fr1,fr8 */
 goto L8c63a0a4;
L8c63a0a4: move(f,10,3); /* fmov fr3,fr10 */
 goto L8c63a0a6;
L8c63a0a6: move(f,12,9); /* fmov fr9,fr12 */
 goto L8c63a0a8;
L8c63a0a8: move(f,14,11); /* fmov fr11,fr14 */
 goto L8c63a0aa;
L8c63a0aa: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a0ac;
L8c63a0ac: f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a0ae;
L8c63a0ae: fr[0]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr0 */
 goto L8c63a0b0;
L8c63a0b0: fr[1]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr1 */
 goto L8c63a0b2;
L8c63a0b2: fr[2]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr2 */
 goto L8c63a0b4;
L8c63a0b4: fr[3]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr3 */
 goto L8c63a0b6;
L8c63a0b6: r[4]+=16u; /* add #0x10,r4 */
 goto L8c63a0b8;
L8c63a0b8: fr[8]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr8 */
 goto L8c63a0ba;
L8c63a0ba: fr[9]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr9 */
 goto L8c63a0bc;
L8c63a0bc: fr[10]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr10 */
 goto L8c63a0be;
L8c63a0be: fr[11]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr11 */
 goto L8c63a0c0;
L8c63a0c0: f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a0c2;
L8c63a0c2: status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a0c4;
L8c63a0c4: status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a0c6;
L8c63a0c6: r[4]-=4;wr(m,r[4],fr[7]); /* fmov.s fr7,@-r4 */
 goto L8c63a0c8;
L8c63a0c8: r[4]-=4;wr(m,r[4],fr[6]); /* fmov.s fr6,@-r4 */
 goto L8c63a0ca;
L8c63a0ca: r[4]-=4;wr(m,r[4],fr[5]); /* fmov.s fr5,@-r4 */
 goto L8c63a0cc;
L8c63a0cc: r[4]-=4;wr(m,r[4],fr[4]); /* fmov.s fr4,@-r4 */
 goto L8c63a0ce;
L8c63a0ce: r[4]+=4294967280u; /* add #-0x10,r4 */
 goto L8c63a0d0;
L8c63a0d0: r[4]-=4;wr(m,r[4],fr[3]); /* fmov.s fr3,@-r4 */
 goto L8c63a0d2;
L8c63a0d2: r[4]-=4;wr(m,r[4],fr[2]); /* fmov.s fr2,@-r4 */
 goto L8c63a0d4;
L8c63a0d4: r[4]-=4;wr(m,r[4],fr[1]); /* fmov.s fr1,@-r4 */
 goto L8c63a0d6;
L8c63a0d6: r[4]-=4;wr(m,r[4],fr[0]); /* fmov.s fr0,@-r4 */
 goto L8c63a0d8;
L8c63a0d8: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a0da;
L8c63a0da: move(f,1,8); /* fmov fr8,fr1 */
 goto L8c63a0dc;
L8c63a0dc: move(f,3,10); /* fmov fr10,fr3 */
 goto L8c63a0de;
L8c63a0de: move(f,9,12); /* fmov fr12,fr9 */
 goto L8c63a0e0;
L8c63a0e0: move(f,11,14); /* fmov fr14,fr11 */
 goto L8c63a0e2;
L8c63a0e2: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a0e4;
L8c63a0e4: fr[15]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr15 */
 goto L8c63a0e6;
L8c63a0e6: fr[14]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr14 */
 goto L8c63a0e8;
L8c63a0e8: fr[13]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr13 */
 goto L8c63a0ea;
L8c63a0ea: fr[12]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr12 */
 goto L8c63a0ec;
L8c63a0ec: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c63a0ee;
L8c63a0ee: take=(*sr&1u);*fpul=r[5];if(take)goto L8c63a158; /* bt/s 0x8c63a158 */
 goto L8c63a0f2;
L8c63a0f2: fr[3]=0; /* fldi0 fr3 */
 goto L8c63a0f4;
L8c63a0f4: status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a0f6;
L8c63a0f6: fr[4]=0; /* fldi0 fr4 */
 goto L8c63a0f8;
L8c63a0f8: fr[7]=0; /* fldi0 fr7 */
 goto L8c63a0fa;
L8c63a0fa: move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a0fc;
L8c63a0fc: fr[0]=0; /* fldi0 fr0 */
 goto L8c63a0fe;
L8c63a0fe: move(f,5,2); /* fmov fr2,fr5 */
 goto L8c63a100;
L8c63a100: move(f,6,1); /* fmov fr1,fr6 */
 goto L8c63a102;
L8c63a102: fr[5]^=0x80000000u; /* fneg fr5 */
 goto L8c63a104;
L8c63a104: r[15]-=4;wr(m,r[15],fr[12]); /* fmov.s fr12,@-r15 */
 goto L8c63a106;
L8c63a106: r[15]-=4;wr(m,r[15],fr[13]); /* fmov.s fr13,@-r15 */
 goto L8c63a108;
L8c63a108: r[15]-=4;wr(m,r[15],fr[14]); /* fmov.s fr14,@-r15 */
 goto L8c63a10a;
L8c63a10a: r[15]-=4;wr(m,r[15],fr[15]); /* fmov.s fr15,@-r15 */
 goto L8c63a10c;
L8c63a10c: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a10e;
L8c63a10e: move(f,8,5); /* fmov fr5,fr8 */
 goto L8c63a110;
L8c63a110: move(f,10,7); /* fmov fr7,fr10 */
 goto L8c63a112;
L8c63a112: move(f,12,9); /* fmov fr9,fr12 */
 goto L8c63a114;
L8c63a114: move(f,14,11); /* fmov fr11,fr14 */
 goto L8c63a116;
L8c63a116: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a118;
L8c63a118: f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a11a;
L8c63a11a: r[4]+=16u; /* add #0x10,r4 */
 goto L8c63a11c;
L8c63a11c: fr[4]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr4 */
 goto L8c63a11e;
L8c63a11e: fr[5]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr5 */
 goto L8c63a120;
L8c63a120: fr[6]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr6 */
 goto L8c63a122;
L8c63a122: fr[7]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr7 */
 goto L8c63a124;
L8c63a124: fr[8]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr8 */
 goto L8c63a126;
L8c63a126: fr[9]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr9 */
 goto L8c63a128;
L8c63a128: fr[10]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr10 */
 goto L8c63a12a;
L8c63a12a: fr[11]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr11 */
 goto L8c63a12c;
L8c63a12c: f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a12e;
L8c63a12e: status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a130;
L8c63a130: status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a132;
L8c63a132: r[4]-=4;wr(m,r[4],fr[7]); /* fmov.s fr7,@-r4 */
 goto L8c63a134;
L8c63a134: r[4]-=4;wr(m,r[4],fr[6]); /* fmov.s fr6,@-r4 */
 goto L8c63a136;
L8c63a136: r[4]-=4;wr(m,r[4],fr[5]); /* fmov.s fr5,@-r4 */
 goto L8c63a138;
L8c63a138: r[4]-=4;wr(m,r[4],fr[4]); /* fmov.s fr4,@-r4 */
 goto L8c63a13a;
L8c63a13a: r[4]-=4;wr(m,r[4],fr[3]); /* fmov.s fr3,@-r4 */
 goto L8c63a13c;
L8c63a13c: r[4]-=4;wr(m,r[4],fr[2]); /* fmov.s fr2,@-r4 */
 goto L8c63a13e;
L8c63a13e: r[4]-=4;wr(m,r[4],fr[1]); /* fmov.s fr1,@-r4 */
 goto L8c63a140;
L8c63a140: r[4]-=4;wr(m,r[4],fr[0]); /* fmov.s fr0,@-r4 */
 goto L8c63a142;
L8c63a142: r[4]+=4294967280u; /* add #-0x10,r4 */
 goto L8c63a144;
L8c63a144: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a146;
L8c63a146: move(f,5,8); /* fmov fr8,fr5 */
 goto L8c63a148;
L8c63a148: move(f,7,10); /* fmov fr10,fr7 */
 goto L8c63a14a;
L8c63a14a: move(f,9,12); /* fmov fr12,fr9 */
 goto L8c63a14c;
L8c63a14c: move(f,11,14); /* fmov fr14,fr11 */
 goto L8c63a14e;
L8c63a14e: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a150;
L8c63a150: fr[15]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr15 */
 goto L8c63a152;
L8c63a152: fr[14]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr14 */
 goto L8c63a154;
L8c63a154: fr[13]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr13 */
 goto L8c63a156;
L8c63a156: fr[12]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr12 */
 goto L8c63a158;
L8c63a158: *sr=(*sr&~1u)|!(r[7]&r[7]); /* tst r7,r7 */
 goto L8c63a15a;
L8c63a15a: take=(*sr&1u);*fpul=r[7];if(take)goto L8c63a1c4; /* bt/s 0x8c63a1c4 */
 goto L8c63a15e;
L8c63a15e: fr[3]=0; /* fldi0 fr3 */
 goto L8c63a160;
L8c63a160: status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a162;
L8c63a162: fr[6]=0; /* fldi0 fr6 */
 goto L8c63a164;
L8c63a164: fr[7]=0; /* fldi0 fr7 */
 goto L8c63a166;
L8c63a166: move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a168;
L8c63a168: move(f,0,1); /* fmov fr1,fr0 */
 goto L8c63a16a;
L8c63a16a: move(f,1,2); /* fmov fr2,fr1 */
 goto L8c63a16c;
L8c63a16c: fr[2]=0; /* fldi0 fr2 */
 goto L8c63a16e;
L8c63a16e: move(f,5,0); /* fmov fr0,fr5 */
 goto L8c63a170;
L8c63a170: move(f,4,1); /* fmov fr1,fr4 */
 goto L8c63a172;
L8c63a172: fr[4]^=0x80000000u; /* fneg fr4 */
 goto L8c63a174;
L8c63a174: r[15]-=4;wr(m,r[15],fr[12]); /* fmov.s fr12,@-r15 */
 goto L8c63a176;
L8c63a176: r[15]-=4;wr(m,r[15],fr[13]); /* fmov.s fr13,@-r15 */
 goto L8c63a178;
L8c63a178: r[15]-=4;wr(m,r[15],fr[14]); /* fmov.s fr14,@-r15 */
 goto L8c63a17a;
L8c63a17a: r[15]-=4;wr(m,r[15],fr[15]); /* fmov.s fr15,@-r15 */
 goto L8c63a17c;
L8c63a17c: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a17e;
L8c63a17e: move(f,8,1); /* fmov fr1,fr8 */
 goto L8c63a180;
L8c63a180: move(f,10,3); /* fmov fr3,fr10 */
 goto L8c63a182;
L8c63a182: move(f,12,5); /* fmov fr5,fr12 */
 goto L8c63a184;
L8c63a184: move(f,14,7); /* fmov fr7,fr14 */
 goto L8c63a186;
L8c63a186: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a188;
L8c63a188: f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a18a;
L8c63a18a: fr[0]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr0 */
 goto L8c63a18c;
L8c63a18c: fr[1]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr1 */
 goto L8c63a18e;
L8c63a18e: fr[2]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr2 */
 goto L8c63a190;
L8c63a190: fr[3]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr3 */
 goto L8c63a192;
L8c63a192: fr[4]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr4 */
 goto L8c63a194;
L8c63a194: fr[5]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr5 */
 goto L8c63a196;
L8c63a196: fr[6]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr6 */
 goto L8c63a198;
L8c63a198: fr[7]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr7 */
 goto L8c63a19a;
L8c63a19a: f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a19c;
L8c63a19c: status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a19e;
L8c63a19e: status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a1a0;
L8c63a1a0: r[4]-=4;wr(m,r[4],fr[7]); /* fmov.s fr7,@-r4 */
 goto L8c63a1a2;
L8c63a1a2: r[4]-=4;wr(m,r[4],fr[6]); /* fmov.s fr6,@-r4 */
 goto L8c63a1a4;
L8c63a1a4: r[4]-=4;wr(m,r[4],fr[5]); /* fmov.s fr5,@-r4 */
 goto L8c63a1a6;
L8c63a1a6: r[4]-=4;wr(m,r[4],fr[4]); /* fmov.s fr4,@-r4 */
 goto L8c63a1a8;
L8c63a1a8: r[4]-=4;wr(m,r[4],fr[3]); /* fmov.s fr3,@-r4 */
 goto L8c63a1aa;
L8c63a1aa: r[4]-=4;wr(m,r[4],fr[2]); /* fmov.s fr2,@-r4 */
 goto L8c63a1ac;
L8c63a1ac: r[4]-=4;wr(m,r[4],fr[1]); /* fmov.s fr1,@-r4 */
 goto L8c63a1ae;
L8c63a1ae: r[4]-=4;wr(m,r[4],fr[0]); /* fmov.s fr0,@-r4 */
 goto L8c63a1b0;
L8c63a1b0: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a1b2;
L8c63a1b2: move(f,1,8); /* fmov fr8,fr1 */
 goto L8c63a1b4;
L8c63a1b4: move(f,3,10); /* fmov fr10,fr3 */
 goto L8c63a1b6;
L8c63a1b6: move(f,5,12); /* fmov fr12,fr5 */
 goto L8c63a1b8;
L8c63a1b8: move(f,7,14); /* fmov fr14,fr7 */
 goto L8c63a1ba;
L8c63a1ba: f->fpscr^=0x100000u; /* fschg */
 goto L8c63a1bc;
L8c63a1bc: fr[15]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr15 */
 goto L8c63a1be;
L8c63a1be: fr[14]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr14 */
 goto L8c63a1c0;
L8c63a1c0: fr[13]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr13 */
 goto L8c63a1c2;
L8c63a1c2: fr[12]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr12 */
 goto L8c63a1c4;
L8c63a1c4: return 0; /* rts */
}
