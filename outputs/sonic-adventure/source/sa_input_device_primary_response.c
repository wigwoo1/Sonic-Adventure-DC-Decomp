#include "fpu_cleanup_transfer.h"
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
/* Static C recovery of retail Tikal helper 8c65ffb8..8c66004c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_primary_response(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65ffb8;
L8c65ffb8: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65ffba;
L8c65ffba: r[14]=r[6]; /* mov r6,r14 */
 goto L8c65ffbc;
L8c65ffbc: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65ffbe;
L8c65ffbe: r[13]=r[7]; /* mov r7,r13 */
 goto L8c65ffc0;
L8c65ffc0: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65ffc2;
L8c65ffc2: r[12]=r[5]; /* mov r5,r12 */
 goto L8c65ffc4;
L8c65ffc4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65ffc6;
L8c65ffc6: r[5]=r[7]; /* mov r7,r5 */
 goto L8c65ffc8;
L8c65ffc8: *pr=0x8c65ffccu;r[4]=r[6];status=call(ctx,0x8c65ff5au,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65ff5a */
 goto L8c65ffcc;
L8c65ffcc: r[5]=r[13]; /* mov r13,r5 */
 goto L8c65ffce;
L8c65ffce: *pr=0x8c65ffd2u;r[4]=r[14];status=call(ctx,0x8c65ff12u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65ff12 */
 goto L8c65ffd2;
L8c65ffd2: r[0]=sx(m,r[14]+0x1u,1); /* mov.b @(0x1,r14),r0 */
 goto L8c65ffd4;
L8c65ffd4: r[4]=sx(m,r[14],1); /* mov.b @r14,r4 */
 goto L8c65ffd6;
L8c65ffd6: r[1]=sx(m,0x8c66004cu,2); /* mov.w 0x8c66004c,r1 */
 goto L8c65ffd8;
L8c65ffd8: r[0]^=r[4]; /* xor r4,r0 */
 goto L8c65ffda;
L8c65ffda: r[0]&=r[4]; /* and r4,r0 */
 goto L8c65ffdc;
L8c65ffdc: wr(m,r[14]+0x2u,r[0],1); /* mov.b r0,@(0x2,r14) */
 goto L8c65ffde;
L8c65ffde: r[4]=sx(m,r[14],1); /* mov.b @r14,r4 */
 goto L8c65ffe0;
L8c65ffe0: r[0]=~r[4]; /* not r4,r0 */
 goto L8c65ffe2;
L8c65ffe2: r[3]=r[0]; /* mov r0,r3 */
 goto L8c65ffe4;
L8c65ffe4: r[0]=sx(m,r[14]+0x1u,1); /* mov.b @(0x1,r14),r0 */
 goto L8c65ffe6;
L8c65ffe6: r[0]^=r[4]; /* xor r4,r0 */
 goto L8c65ffe8;
L8c65ffe8: r[0]&=r[3]; /* and r3,r0 */
 goto L8c65ffea;
L8c65ffea: r[3]=rd(m,0x8c660054u,4); /* mov.l 0x8c660054,r3 */
 goto L8c65ffec;
L8c65ffec: wr(m,r[14]+0x3u,r[0],1); /* mov.b r0,@(0x3,r14) */
 goto L8c65ffee;
L8c65ffee: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c65fff0;
L8c65fff0: r[0]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c65fff2;
L8c65fff2: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65fff4;
L8c65fff4: if(!(*sr&1u))goto L8c660010; /* bf 0x8c660010 */
 goto L8c65fff6;
L8c65fff6: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto L8c65fff8;
L8c65fff8: r[2]=sx(m,0x8c66004eu,2); /* mov.w 0x8c66004e,r2 */
 goto L8c65fffa;
L8c65fffa: r[4]=r[0]&255u; /* extu.b r0,r4 */
 goto L8c65fffc;
L8c65fffc: *sr=(*sr&~1u)|(r[2]==r[4]); /* cmp/eq r2,r4 */
 goto L8c65fffe;
L8c65fffe: if((*sr&1u))goto L8c660010; /* bt 0x8c660010 */
 goto L8c660000;
L8c660000: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c660002;
L8c660002: if((*sr&1u))goto L8c660010; /* bt 0x8c660010 */
 goto L8c660004;
L8c660004: r[5]=rd(m,r[13],4); /* mov.l @r13,r5 */
 goto L8c660006;
L8c660006: r[3]=rd(m,0x8c660058u,4); /* mov.l 0x8c660058,r3 */
 goto L8c660008;
L8c660008: r[5]=r[5]&255u; /* extu.b r5,r5 */
 goto L8c66000a;
L8c66000a: r[5]+=1u; /* add #0x1,r5 */
 goto L8c66000c;
L8c66000c: target=r[3];*pr=0x8c660010u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660010;
L8c660010: r[2]=sx(m,r[14],1); /* mov.b @r14,r2 */
 goto L8c660012;
L8c660012: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c660014;
L8c660014: take=(*sr&1u);r[4]=1u;if(take)goto L8c66001c;goto L8c660018; /* bt/s 0x8c66001c */
 goto L8c660018;
L8c660018: r[0]=9u;goto L8c66001e; /* bra 0x8c66001e */
 goto L8c66001c;
L8c66001c: r[0]=r[4]; /* mov r4,r0 */
 goto L8c66001e;
L8c66001e: wr(m,r[14]+0x4u,r[0],1); /* mov.b r0,@(0x4,r14) */
 goto L8c660020;
L8c660020: r[0]=sx(m,r[14]+0x3u,1); /* mov.b @(0x3,r14),r0 */
 goto L8c660022;
L8c660022: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c660024;
L8c660024: r[0]=sx(m,r[14]+0x2u,1); /* mov.b @(0x2,r14),r0 */
 goto L8c660026;
L8c660026: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c660028;
L8c660028: if((*sr&1u))goto L8c660036; /* bt 0x8c660036 */
 goto L8c66002a;
L8c66002a: r[0]=16u; /* mov #0x10,r0 */
 goto L8c66002c;
L8c66002c: r[5]=r[14]; /* mov r14,r5 */
 goto L8c66002e;
L8c66002e: r[6]=r[13]; /* mov r13,r6 */
 goto L8c660030;
L8c660030: wr(m,r[12]+r[0],r[4],1); /* mov.b r4,@(r0,r12) */
 goto L8c660032;
L8c660032: *pr=0x8c660036u;r[4]=r[12];status=call(ctx,0x8c66018eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c66018e */
 goto L8c660036;
L8c660036: r[0]=sx(m,r[14]+0x3u,1); /* mov.b @(0x3,r14),r0 */
 goto L8c660038;
L8c660038: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c66003a;
L8c66003a: if((*sr&1u))goto L8c660040; /* bt 0x8c660040 */
 goto L8c66003c;
L8c66003c: *pr=0x8c660040u;r[4]=r[12];status=call(ctx,0x8c6602d2u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6602d2 */
 goto L8c660040;
L8c660040: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c660042;
L8c660042: r[0]=0u; /* mov #0x0,r0 */
 goto L8c660044;
L8c660044: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c660046;
L8c660046: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c660048;
L8c660048: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
