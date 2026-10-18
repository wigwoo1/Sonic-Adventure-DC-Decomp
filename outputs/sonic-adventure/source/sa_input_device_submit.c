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
/* Static C recovery of retail Tikal helper 8c65efda..8c65f1aa.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_submit(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65efda;
L8c65efda: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65efdc;
L8c65efdc: r[14]=rd(m,0x8c65f024u,4); /* mov.l 0x8c65f024,r14 */
 goto L8c65efde;
L8c65efde: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65efe0;
L8c65efe0: r[1]=sx(m,0x8c65f00cu,2); /* mov.w 0x8c65f00c,r1 */
 goto L8c65efe2;
L8c65efe2: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65efe4;
L8c65efe4: r[12]=1u; /* mov #0x1,r12 */
 goto L8c65efe6;
L8c65efe6: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c65efe8;
L8c65efe8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65efea;
L8c65efea: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65efec;
L8c65efec: r[0]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c65efee;
L8c65efee: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65eff0;
L8c65eff0: take=(*sr&1u);r[13]=0u;if(take)goto L8c65eff8;goto L8c65eff4; /* bt/s 0x8c65eff8 */
 goto L8c65eff4;
L8c65eff4: r[0]=4294967293u;goto L8c65f19e; /* bra 0x8c65f19e */
 goto L8c65eff8;
L8c65eff8: r[0]=sx(m,0x8c65f00eu,2); /* mov.w 0x8c65f00e,r0 */
 goto L8c65effa;
L8c65effa: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c65effc;
L8c65effc: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c65effe;
L8c65effe: r[1]=rd(m,r[3]+0x18u,4); /* mov.l @(0x18,r3),r1 */
 goto L8c65f000;
L8c65f000: *sr=(*sr&~1u)|!(r[12]&r[1]); /* tst r12,r1 */
 goto L8c65f002;
L8c65f002: if((*sr&1u))goto L8c65f028; /* bt 0x8c65f028 */
 goto L8c65f004;
L8c65f004: r[0]=4294967295u;goto L8c65f19e; /* bra 0x8c65f19e */
 goto L8c65f028;
L8c65f028: r[3]=rd(m,0x8c65f154u,4); /* mov.l 0x8c65f154,r3 */
 goto L8c65f02a;
L8c65f02a: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c65f02c;
L8c65f02c: {uint32_t value=rd(m,r[0],1);*sr=(*sr&~1u)|(value==0);wr(m,r[0],value|0x80u,1);} /* tas.b @r0 */
 goto L8c65f02e;
L8c65f02e: if((*sr&1u))goto L8c65f03c; /* bt 0x8c65f03c */
 goto L8c65f030;
L8c65f030: r[1]=rd(m,0x8c65f158u,4); /* mov.l 0x8c65f158,r1 */
 goto L8c65f032;
L8c65f032: *pr=0x8c65f036u;r[15]-=4;wr(m,r[15],r[1],4);status=call(ctx,0x8c65f348u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65f348 */
 goto L8c65f036;
L8c65f036: r[15]+=4u; /* add #0x4,r15 */
 goto L8c65f038;
L8c65f038: r[0]=4294967294u;goto L8c65f19e; /* bra 0x8c65f19e */
 goto L8c65f03c;
L8c65f03c: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f03e;
L8c65f03e: r[0]=sx(m,0x8c65f13au,2); /* mov.w 0x8c65f13a,r0 */
 goto L8c65f040;
L8c65f040: r[2]=r[3]; /* mov r3,r2 */
 goto L8c65f042;
L8c65f042: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c65f044;
L8c65f044: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65f046;
L8c65f046: r[3]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r3 */
 goto L8c65f048;
L8c65f048: *sr=(*sr&~1u)|(r[1]==r[3]); /* cmp/eq r1,r3 */
 goto L8c65f04a;
L8c65f04a: if((*sr&1u))goto L8c65f0aa; /* bt 0x8c65f0aa */
 goto L8c65f04c;
L8c65f04c: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f04e;
L8c65f04e: r[0]=sx(m,0x8c65f13au,2); /* mov.w 0x8c65f13a,r0 */
 goto L8c65f050;
L8c65f050: r[2]=r[3]; /* mov r3,r2 */
 goto L8c65f052;
L8c65f052: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c65f054;
L8c65f054: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65f056;
L8c65f056: wr(m,r[3]+r[0],r[1],4); /* mov.l r1,@(r0,r3) */
 goto L8c65f058;
L8c65f058: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c65f05a;
L8c65f05a: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f05c;
L8c65f05c: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f05e;
L8c65f05e: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c65f060;
L8c65f060: if((*sr&1u))goto L8c65f07e; /* bt 0x8c65f07e */
 goto L8c65f062;
L8c65f062: r[1]=rd(m,0x8c65f15cu,4); /* mov.l 0x8c65f15c,r1 */
 goto L8c65f064;
L8c65f064: r[3]=sx(m,0x8c65f13cu,2); /* mov.w 0x8c65f13c,r3 */
 goto L8c65f066;
L8c65f066: r[0]=rd(m,r[1],4); /* mov.l @r1,r0 */
 goto L8c65f068;
L8c65f068: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c65f06a;
L8c65f06a: *macl=r[3]*r[0]; /* mul.l r3,r0 */
 goto L8c65f06c;
L8c65f06c: r[3]=rd(m,0x8c65f160u,4); /* mov.l 0x8c65f160,r3 */
 goto L8c65f06e;
L8c65f06e: r[1]=*macl; /* sts MACL,r1 */
 goto L8c65f070;
L8c65f070: target=r[3];*pr=0x8c65f074u;r[0]=20u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65f074;
L8c65f074: r[1]=sx(m,0x8c65f13eu,2); /* mov.w 0x8c65f13e,r1 */
 goto L8c65f076;
L8c65f076: r[0]<<=16; /* shll16 r0 */
 goto L8c65f078;
L8c65f078: r[1]+=r[2]; /* add r2,r1 */
 goto L8c65f07a;
L8c65f07a: wr(m,r[1],r[0],4);goto L8c65f086; /* bra 0x8c65f086 */
 goto L8c65f07e;
L8c65f07e: r[0]=sx(m,0x8c65f13eu,2); /* mov.w 0x8c65f13e,r0 */
 goto L8c65f080;
L8c65f080: r[2]=rd(m,0x8c65f164u,4); /* mov.l 0x8c65f164,r2 */
 goto L8c65f082;
L8c65f082: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f084;
L8c65f084: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f086;
L8c65f086: r[0]=sx(m,0x8c65f140u,2); /* mov.w 0x8c65f140,r0 */
 goto L8c65f088;
L8c65f088: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f08a;
L8c65f08a: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f08c;
L8c65f08c: wr(m,r[2]+0x14u,r[13],4); /* mov.l r13,@(0x14,r2) */
 goto L8c65f08e;
L8c65f08e: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f090;
L8c65f090: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f092;
L8c65f092: r[0]+=4294967260u; /* add #-0x24,r0 */
 goto L8c65f094;
L8c65f094: r[1]=r[3]; /* mov r3,r1 */
 goto L8c65f096;
L8c65f096: r[3]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r3 */
 goto L8c65f098;
L8c65f098: r[0]+=4294967280u; /* add #-0x10,r0 */
 goto L8c65f09a;
L8c65f09a: r[1]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r1 */
 goto L8c65f09c;
L8c65f09c: r[0]=sx(m,0x8c65f142u,2); /* mov.w 0x8c65f142,r0 */
 goto L8c65f09e;
L8c65f09e: r[1]|=r[3]; /* or r3,r1 */
 goto L8c65f0a0;
L8c65f0a0: wr(m,r[2]+r[0],r[1],4); /* mov.l r1,@(r0,r2) */
 goto L8c65f0a2;
L8c65f0a2: r[0]=sx(m,0x8c65f140u,2); /* mov.w 0x8c65f140,r0 */
 goto L8c65f0a4;
L8c65f0a4: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f0a6;
L8c65f0a6: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f0a8;
L8c65f0a8: wr(m,r[2]+0x14u,r[12],4); /* mov.l r12,@(0x14,r2) */
 goto L8c65f0aa;
L8c65f0aa: *pr=0x8c65f0aeu;status=call(ctx,0x8c65eedau,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65eeda */
 goto L8c65f0ae;
L8c65f0ae: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c65f0b0;
L8c65f0b0: r[1]=sx(m,0x8c65f144u,2); /* mov.w 0x8c65f144,r1 */
 goto L8c65f0b2;
L8c65f0b2: r[3]=r[2]; /* mov r2,r3 */
 goto L8c65f0b4;
L8c65f0b4: r[0]=r[2]; /* mov r2,r0 */
 goto L8c65f0b6;
L8c65f0b6: r[1]+=r[3]; /* add r3,r1 */
 goto L8c65f0b8;
L8c65f0b8: r[3]=sx(m,0x8c65f146u,2); /* mov.w 0x8c65f146,r3 */
 goto L8c65f0ba;
L8c65f0ba: r[1]=rd(m,r[1],4); /* mov.l @r1,r1 */
 goto L8c65f0bc;
L8c65f0bc: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65f0be;
L8c65f0be: r[1]<<=2; /* shll2 r1 */
 goto L8c65f0c0;
L8c65f0c0: r[1]+=r[3]; /* add r3,r1 */
 goto L8c65f0c2;
L8c65f0c2: r[0]=rd(m,r[1],4); /* mov.l @r1,r0 */
 goto L8c65f0c4;
L8c65f0c4: r[1]=sx(m,0x8c65f148u,2); /* mov.w 0x8c65f148,r1 */
 goto L8c65f0c6;
L8c65f0c6: r[1]+=r[2]; /* add r2,r1 */
 goto L8c65f0c8;
L8c65f0c8: r[2]=rd(m,0x8c65f168u,4); /* mov.l 0x8c65f168,r2 */
 goto L8c65f0ca;
L8c65f0ca: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c65f0cc;
L8c65f0cc: r[0]=sx(m,0x8c65f140u,2); /* mov.w 0x8c65f140,r0 */
 goto L8c65f0ce;
L8c65f0ce: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f0d0;
L8c65f0d0: r[1]=sx(m,0x8c65f144u,2); /* mov.w 0x8c65f144,r1 */
 goto L8c65f0d2;
L8c65f0d2: r[4]=r[3]; /* mov r3,r4 */
 goto L8c65f0d4;
L8c65f0d4: r[8]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r8 */
 goto L8c65f0d6;
L8c65f0d6: r[1]+=r[4]; /* add r4,r1 */
 goto L8c65f0d8;
L8c65f0d8: r[0]=r[3]; /* mov r3,r0 */
 goto L8c65f0da;
L8c65f0da: r[3]=sx(m,0x8c65f146u,2); /* mov.w 0x8c65f146,r3 */
 goto L8c65f0dc;
L8c65f0dc: r[8]+=4u; /* add #0x4,r8 */
 goto L8c65f0de;
L8c65f0de: r[4]=rd(m,r[1],4); /* mov.l @r1,r4 */
 goto L8c65f0e0;
L8c65f0e0: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65f0e2;
L8c65f0e2: r[4]<<=2; /* shll2 r4 */
 goto L8c65f0e4;
L8c65f0e4: r[4]+=r[3]; /* add r3,r4 */
 goto L8c65f0e6;
L8c65f0e6: target=r[2];*pr=0x8c65f0eau;r[4]=rd(m,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65f0ea;
L8c65f0ea: wr(m,r[8],r[0],4); /* mov.l r0,@r8 */
 goto L8c65f0ec;
L8c65f0ec: r[0]=sx(m,0x8c65f140u,2); /* mov.w 0x8c65f140,r0 */
 goto L8c65f0ee;
L8c65f0ee: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f0f0;
L8c65f0f0: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f0f2;
L8c65f0f2: wr(m,r[2]+0x18u,r[12],4); /* mov.l r12,@(0x18,r2) */
 goto L8c65f0f4;
L8c65f0f4: r[0]=sx(m,0x8c65f14au,2); /* mov.w 0x8c65f14a,r0 */
 goto L8c65f0f6;
L8c65f0f6: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f0f8;
L8c65f0f8: r[1]=sx(m,0x8c65f144u,2); /* mov.w 0x8c65f144,r1 */
 goto L8c65f0fa;
L8c65f0fa: wr(m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f0fc;
L8c65f0fc: r[0]=sx(m,0x8c65f144u,2); /* mov.w 0x8c65f144,r0 */
 goto L8c65f0fe;
L8c65f0fe: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f100;
L8c65f100: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f102;
L8c65f102: r[2]^=r[12]; /* xor r12,r2 */
 goto L8c65f104;
L8c65f104: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f106;
L8c65f106: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f108;
L8c65f108: r[2]=r[3]; /* mov r3,r2 */
 goto L8c65f10a;
L8c65f10a: r[1]+=r[2]; /* add r2,r1 */
 goto L8c65f10c;
L8c65f10c: r[2]=sx(m,0x8c65f146u,2); /* mov.w 0x8c65f146,r2 */
 goto L8c65f10e;
L8c65f10e: r[0]=r[3]; /* mov r3,r0 */
 goto L8c65f110;
L8c65f110: r[1]=rd(m,r[1],4); /* mov.l @r1,r1 */
 goto L8c65f112;
L8c65f112: r[2]+=r[0]; /* add r0,r2 */
 goto L8c65f114;
L8c65f114: r[1]<<=2; /* shll2 r1 */
 goto L8c65f116;
L8c65f116: r[1]+=r[2]; /* add r2,r1 */
 goto L8c65f118;
L8c65f118: r[0]=rd(m,r[1],4); /* mov.l @r1,r0 */
 goto L8c65f11a;
L8c65f11a: r[1]=sx(m,0x8c65f14cu,2); /* mov.w 0x8c65f14c,r1 */
 goto L8c65f11c;
L8c65f11c: r[1]+=r[3]; /* add r3,r1 */
 goto L8c65f11e;
L8c65f11e: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c65f120;
L8c65f120: r[0]=sx(m,0x8c65f14eu,2); /* mov.w 0x8c65f14e,r0 */
 goto L8c65f122;
L8c65f122: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f124;
L8c65f124: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f126;
L8c65f126: r[2]^=r[12]; /* xor r12,r2 */
 goto L8c65f128;
L8c65f128: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f12a;
L8c65f12a: r[0]=sx(m,0x8c65f150u,2); /* mov.w 0x8c65f150,r0 */
 goto L8c65f12c;
L8c65f12c: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f12e;
L8c65f12e: wr(m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f130;
L8c65f130: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c65f132;
L8c65f132: r[4]+=40u; /* add #0x28,r4 */
 goto L8c65f134;
L8c65f134: r[5]=r[13]; /* mov r13,r5 */
 goto L8c65f136;
L8c65f136: r[6]=24u;goto L8c65f18c; /* bra 0x8c65f18c */
 goto L8c65f16c;
L8c65f16c: r[3]=sx(m,0x8c65f284u,2); /* mov.w 0x8c65f284,r3 */
 goto L8c65f16e;
L8c65f16e: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65f170;
L8c65f170: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65f172;
L8c65f172: r[3]+=r[5]; /* add r5,r3 */
 goto L8c65f174;
L8c65f174: r[2]=sx(m,r[3],1); /* mov.b @r3,r2 */
 goto L8c65f176;
L8c65f176: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c65f178;
L8c65f178: take=(*sr&1u);r[5]+=1u;if(take)goto L8c65f18a;goto L8c65f17c; /* bt/s 0x8c65f18a */
 goto L8c65f17c;
L8c65f17c: r[0]=sx(m,r[4]+0x6u,1); /* mov.b @(0x6,r4),r0 */
 goto L8c65f17e;
L8c65f17e: wr(m,r[4]+0x7u,r[0],1); /* mov.b r0,@(0x7,r4) */
 goto L8c65f180;
L8c65f180: r[0]=r[13]; /* mov r13,r0 */
 goto L8c65f182;
L8c65f182: wr(m,r[4]+0x6u,r[0],1); /* mov.b r0,@(0x6,r4) */
 goto L8c65f184;
L8c65f184: r[3]=rd(m,r[4]+0x18u,4); /* mov.l @(0x18,r4),r3 */
 goto L8c65f186;
L8c65f186: wr(m,r[4]+0x1cu,r[3],4); /* mov.l r3,@(0x1c,r4) */
 goto L8c65f188;
L8c65f188: wr(m,r[4]+0x18u,r[0],4); /* mov.l r0,@(0x18,r4) */
 goto L8c65f18a;
L8c65f18a: r[4]+=44u; /* add #0x2c,r4 */
 goto L8c65f18c;
L8c65f18c: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]); /* cmp/ge r6,r5 */
 goto L8c65f18e;
L8c65f18e: if(!(*sr&1u))goto L8c65f16c; /* bf 0x8c65f16c */
 goto L8c65f190;
L8c65f190: r[0]=sx(m,0x8c65f286u,2); /* mov.w 0x8c65f286,r0 */
 goto L8c65f192;
L8c65f192: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65f194;
L8c65f194: r[2]=rd(m,0x8c65f28cu,4); /* mov.l 0x8c65f28c,r2 */
 goto L8c65f196;
L8c65f196: wr(m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c65f198;
L8c65f198: r[0]=r[13]; /* mov r13,r0 */
 goto L8c65f19a;
L8c65f19a: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c65f19c;
L8c65f19c: wr(m,r[3],r[13],1); /* mov.b r13,@r3 */
 goto L8c65f19e;
L8c65f19e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65f1a0;
L8c65f1a0: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c65f1a2;
L8c65f1a2: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65f1a4;
L8c65f1a4: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65f1a6;
L8c65f1a6: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
