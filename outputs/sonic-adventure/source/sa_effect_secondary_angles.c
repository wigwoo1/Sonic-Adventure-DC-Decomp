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
/* Static C recovery of retail Tikal helper 8c07af8c..8c07b04a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_effect_secondary_angles(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c07af8c;
L8c07af8c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c07af8e;
L8c07af8e: r[14]=r[4]; /* mov r4,r14 */
 goto L8c07af90;
L8c07af90: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c07af92;
L8c07af92: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c07af94;
L8c07af94: r[12]=8u; /* mov #0x8,r12 */
 goto L8c07af96;
L8c07af96: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c07af98;
L8c07af98: r[12]<<=8; /* shll8 r12 */
 goto L8c07af9a;
L8c07af9a: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c07af9c;
L8c07af9c: r[6]=r[12]; /* mov r12,r6 */
 goto L8c07af9e;
L8c07af9e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c07afa0;
L8c07afa0: r[5]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r5 */
 goto L8c07afa2;
L8c07afa2: r[10]=rd(m,0x8c07b060u,4); /* mov.l 0x8c07b060,r10 */
 goto L8c07afa4;
L8c07afa4: target=r[10];*pr=0x8c07afa8u;r[4]=rd(m,r[14]+0x18u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto L8c07afa8;
L8c07afa8: r[13]=rd(m,0x8c07b064u,4); /* mov.l 0x8c07b064,r13 */
 goto L8c07afaa;
L8c07afaa: r[3]=r[0]; /* mov r0,r3 */
 goto L8c07afac;
L8c07afac: r[11]=rd(m,0x8c07b068u,4); /* mov.l 0x8c07b068,r11 */
 goto L8c07afae;
L8c07afae: *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c07afb0;
L8c07afb0: take=(*sr&1u);r[4]=r[0];if(take)goto L8c07afb6;goto L8c07afb4; /* bt/s 0x8c07afb6 */
 goto L8c07afb4;
L8c07afb4: r[4]|=r[11]; /* or r11,r4 */
 goto L8c07afb6;
L8c07afb6: r[3]=rd(m,r[14]+0x38u,4); /* mov.l @(0x38,r14),r3 */
 goto L8c07afb8;
L8c07afb8: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[3]); /* cmp/gt r3,r4 */
 goto L8c07afba;
L8c07afba: if(!(*sr&1u))goto L8c07afc0; /* bf 0x8c07afc0 */
 goto L8c07afbc;
L8c07afbc: r[4]=rd(m,r[14]+0x38u,4);goto L8c07afc8; /* bra 0x8c07afc8 */
 goto L8c07afc0;
L8c07afc0: r[2]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r2 */
 goto L8c07afc2;
L8c07afc2: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]); /* cmp/ge r2,r4 */
 goto L8c07afc4;
L8c07afc4: if((*sr&1u))goto L8c07afc8; /* bt 0x8c07afc8 */
 goto L8c07afc6;
L8c07afc6: r[4]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r4 */
 goto L8c07afc8;
L8c07afc8: r[0]=92u; /* mov #0x5c,r0 */
 goto L8c07afca;
L8c07afca: r[6]=r[12]; /* mov r12,r6 */
 goto L8c07afcc;
L8c07afcc: wr(m,r[14]+0x18u,r[4],4); /* mov.l r4,@(0x18,r14) */
 goto L8c07afce;
L8c07afce: r[3]=r[4]; /* mov r4,r3 */
 goto L8c07afd0;
L8c07afd0: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c07afd2;
L8c07afd2: wr(m,r[2]+0x18u,r[4],4); /* mov.l r4,@(0x18,r2) */
 goto L8c07afd4;
L8c07afd4: r[5]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r5 */
 goto L8c07afd6;
L8c07afd6: target=r[10];*pr=0x8c07afdau;r[4]=rd(m,r[14]+0x20u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto L8c07afda;
L8c07afda: r[2]=r[0]; /* mov r0,r2 */
 goto L8c07afdc;
L8c07afdc: *sr=(*sr&~1u)|!(r[13]&r[2]); /* tst r13,r2 */
 goto L8c07afde;
L8c07afde: take=(*sr&1u);r[4]=r[0];if(take)goto L8c07afe4;goto L8c07afe2; /* bt/s 0x8c07afe4 */
 goto L8c07afe2;
L8c07afe2: r[4]|=r[11]; /* or r11,r4 */
 goto L8c07afe4;
L8c07afe4: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c07afe6;
L8c07afe6: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c07afe8;
L8c07afe8: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]); /* cmp/ge r2,r4 */
 goto L8c07afea;
L8c07afea: if((*sr&1u))goto L8c07aff0; /* bt 0x8c07aff0 */
 goto L8c07afec;
L8c07afec: r[4]=rd(m,r[14]+r[0],4);goto L8c07affa; /* bra 0x8c07affa */
 goto L8c07aff0;
L8c07aff0: r[0]=72u; /* mov #0x48,r0 */
 goto L8c07aff2;
L8c07aff2: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c07aff4;
L8c07aff4: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c07aff6;
L8c07aff6: if(!(*sr&1u))goto L8c07affa; /* bf 0x8c07affa */
 goto L8c07aff8;
L8c07aff8: r[4]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r4 */
 goto L8c07affa;
L8c07affa: r[0]=96u; /* mov #0x60,r0 */
 goto L8c07affc;
L8c07affc: r[6]=r[12]; /* mov r12,r6 */
 goto L8c07affe;
L8c07affe: wr(m,r[14]+0x20u,r[4],4); /* mov.l r4,@(0x20,r14) */
 goto L8c07b000;
L8c07b000: r[2]=r[4]; /* mov r4,r2 */
 goto L8c07b002;
L8c07b002: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c07b004;
L8c07b004: wr(m,r[3]+0x18u,r[4],4); /* mov.l r4,@(0x18,r3) */
 goto L8c07b006;
L8c07b006: r[5]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r5 */
 goto L8c07b008;
L8c07b008: target=r[10];*pr=0x8c07b00cu;r[4]=rd(m,r[14]+0x1cu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto L8c07b00c;
L8c07b00c: r[4]=r[0]; /* mov r0,r4 */
 goto L8c07b00e;
L8c07b00e: *sr=(*sr&~1u)|!(r[4]&r[13]); /* tst r4,r13 */
 goto L8c07b010;
L8c07b010: take=(*sr&1u);r[0]=84u;if(take)goto L8c07b016;goto L8c07b014; /* bt/s 0x8c07b016 */
 goto L8c07b014;
L8c07b014: r[4]|=r[11]; /* or r11,r4 */
 goto L8c07b016;
L8c07b016: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c07b018;
L8c07b018: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]); /* cmp/ge r2,r4 */
 goto L8c07b01a;
L8c07b01a: if((*sr&1u))goto L8c07b020; /* bt 0x8c07b020 */
 goto L8c07b01c;
L8c07b01c: r[4]=rd(m,r[14]+r[0],4);goto L8c07b02a; /* bra 0x8c07b02a */
 goto L8c07b020;
L8c07b020: r[0]=80u; /* mov #0x50,r0 */
 goto L8c07b022;
L8c07b022: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c07b024;
L8c07b024: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c07b026;
L8c07b026: if(!(*sr&1u))goto L8c07b02a; /* bf 0x8c07b02a */
 goto L8c07b028;
L8c07b028: r[4]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r4 */
 goto L8c07b02a;
L8c07b02a: r[0]=92u; /* mov #0x5c,r0 */
 goto L8c07b02c;
L8c07b02c: r[2]=r[4]; /* mov r4,r2 */
 goto L8c07b02e;
L8c07b02e: wr(m,r[14]+0x1cu,r[4],4); /* mov.l r4,@(0x1c,r14) */
 goto L8c07b030;
L8c07b030: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c07b032;
L8c07b032: r[0]=96u; /* mov #0x60,r0 */
 goto L8c07b034;
L8c07b034: wr(m,r[3]+0x1cu,r[4],4); /* mov.l r4,@(0x1c,r3) */
 goto L8c07b036;
L8c07b036: wr(m,r[14]+0x24u,r[2],4); /* mov.l r2,@(0x24,r14) */
 goto L8c07b038;
L8c07b038: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c07b03a;
L8c07b03a: wr(m,r[3]+0x1cu,r[2],4); /* mov.l r2,@(0x1c,r3) */
 goto L8c07b03c;
L8c07b03c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c07b03e;
L8c07b03e: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c07b040;
L8c07b040: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c07b042;
L8c07b042: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c07b044;
L8c07b044: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c07b046;
L8c07b046: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
