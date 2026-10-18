#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
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
/* Static C recovery of retail Unaligned file read helper 8c600fac..8c601084.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_read_unaligned(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c600fac;
L8c600fac: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c600fae;
L8c600fae: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c600fb0;
L8c600fb0: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c600fb2;
L8c600fb2: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c600fb4;
L8c600fb4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c600fb6;
L8c600fb6: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c600fb8;
L8c600fb8: r[14]=r[4]; /* mov r4,r14 */
 goto L8c600fba;
L8c600fba: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c600fbc;
L8c600fbc: take=(*sr&1u);r[13]=r[5];if(take)goto L8c600fc8;goto L8c600fc0; /* bt/s 0x8c600fc8 */
 goto L8c600fc0;
L8c600fc0: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c600fc2;
L8c600fc2: r[3]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r3 */
 goto L8c600fc4;
L8c600fc4: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c600fc6;
L8c600fc6: if(!(*sr&1u))goto L8c600fcc; /* bf 0x8c600fcc */
 goto L8c600fc8;
L8c600fc8: r[0]=4294967286u;goto L8c601076; /* bra 0x8c601076 */
 goto L8c600fcc;
L8c600fcc: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c600fce;
L8c600fce: r[2]=rd(m,r[3]+0x1cu,4); /* mov.l @(0x1c,r3),r2 */
 goto L8c600fd0;
L8c600fd0: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c600fd2;
L8c600fd2: if(!(*sr&1u))goto L8c601074; /* bf 0x8c601074 */
 goto L8c600fd4;
L8c600fd4: r[12]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r12 */
 goto L8c600fd6;
L8c600fd6: r[4]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r4 */
 goto L8c600fd8;
L8c600fd8: r[3]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r3 */
 goto L8c600fda;
L8c600fda: r[12]+=r[4]; /* add r4,r12 */
 goto L8c600fdc;
L8c600fdc: r[4]+=r[3]; /* add r3,r4 */
 goto L8c600fde;
L8c600fde: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[4]); /* cmp/ge r4,r12 */
 goto L8c600fe0;
L8c600fe0: take=(*sr&1u);r[11]=1u;if(take)goto L8c600ff4;goto L8c600fe4; /* bt/s 0x8c600ff4 */
 goto L8c600fe4;
L8c600fe4: r[3]=r[13]; /* mov r13,r3 */
 goto L8c600fe6;
L8c600fe6: r[3]+=r[12]; /* add r12,r3 */
 goto L8c600fe8;
L8c600fe8: *sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[4]); /* cmp/gt r4,r3 */
 goto L8c600fea;
L8c600fea: if(!(*sr&1u))goto L8c600ff0; /* bf 0x8c600ff0 */
 goto L8c600fec;
L8c600fec: r[13]=r[4]; /* mov r4,r13 */
 goto L8c600fee;
L8c600fee: r[13]-=r[12]; /* sub r12,r13 */
 goto L8c600ff0;
L8c600ff0: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c600ff2;
L8c600ff2: if(!(*sr&1u))goto L8c601004; /* bf 0x8c601004 */
 goto L8c600ff4;
L8c600ff4: r[2]=0u; /* mov #0x0,r2 */
 goto L8c600ff6;
L8c600ff6: r[0]=80u; /* mov #0x50,r0 */
 goto L8c600ff8;
L8c600ff8: wr(m,r[14]+0x18u,r[2],4); /* mov.l r2,@(0x18,r14) */
 goto L8c600ffa;
L8c600ffa: wr(m,r[14]+r[0],r[11],2);goto L8c601074; /* bra 0x8c601074 */
 goto L8c601004;
L8c601004: r[0]=*sr; /* stc SR,r0 */
 goto L8c601006;
L8c601006: r[3]=sx(m,0x8c6010eau,2); /* mov.w 0x8c6010ea,r3 */
 goto L8c601008;
L8c601008: r[0]>>=2; /* shlr2 r0 */
 goto L8c60100a;
L8c60100a: r[0]>>=2; /* shlr2 r0 */
 goto L8c60100c;
L8c60100c: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c60100e;
L8c60100e: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c601010;
L8c601010: r[0]=*sr; /* stc SR,r0 */
 goto L8c601012;
L8c601012: r[0]&=r[3]; /* and r3,r0 */
 goto L8c601014;
L8c601014: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c601016;
L8c601016: *sr=r[0]; /* ldc r0,SR */
 goto L8c601018;
L8c601018: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c60101a;
L8c60101a: r[5]=r[13]; /* mov r13,r5 */
 goto L8c60101c;
L8c60101c: r[7]=0u; /* mov #0x0,r7 */
 goto L8c60101e;
L8c60101e: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c601020;
L8c601020: r[1]=rd(m,r[3]+0x4u,4); /* mov.l @(0x4,r3),r1 */
 goto L8c601022;
L8c601022: target=r[1];*pr=0x8c601026u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c601026;
L8c601026: r[12]=r[0]; /* mov r0,r12 */
 goto L8c601028;
L8c601028: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c60102a;
L8c60102a: if((*sr&1u))goto L8c601056; /* bt 0x8c601056 */
 goto L8c60102c;
L8c60102c: r[2]=2u; /* mov #0x2,r2 */
 goto L8c60102e;
L8c60102e: wr(m,r[14]+0x3cu,r[12],4); /* mov.l r12,@(0x3c,r14) */
 goto L8c601030;
L8c601030: r[0]=80u; /* mov #0x50,r0 */
 goto L8c601032;
L8c601032: wr(m,r[14]+0x1cu,r[13],4); /* mov.l r13,@(0x1c,r14) */
 goto L8c601034;
L8c601034: wr(m,r[14]+r[0],r[2],2); /* mov.w r2,@(r0,r14) */
 goto L8c601036;
L8c601036: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c601038;
L8c601038: wr(m,r[14]+r[0],r[11],2); /* mov.w r11,@(r0,r14) */
 goto L8c60103a;
L8c60103a: r[3]=0u; /* mov #0x0,r3 */
 goto L8c60103c;
L8c60103c: r[0]=72u; /* mov #0x48,r0 */
 goto L8c60103e;
L8c60103e: wr(m,r[14]+r[0],r[11],2); /* mov.w r11,@(r0,r14) */
 goto L8c601040;
L8c601040: wr(m,r[14]+0x18u,r[3],4); /* mov.l r3,@(0x18,r14) */
 goto L8c601042;
L8c601042: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c601044;
L8c601044: wr(m,r[2]+0x1cu,r[14],4); /* mov.l r14,@(0x1c,r2) */
 goto L8c601046;
L8c601046: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c601048;
L8c601048: r[0]=64u; /* mov #0x40,r0 */
 goto L8c60104a;
L8c60104a: r[2]=rd(m,r[3]+0xcu,4); /* mov.l @(0xc,r3),r2 */
 goto L8c60104c;
L8c60104c: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c60104e;
L8c60104e: target=r[1];*pr=0x8c601052u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c601052;
L8c601052: goto L8c60105a; /* bra 0x8c60105a */
 goto L8c601056;
L8c601056: r[0]=80u; /* mov #0x50,r0 */
 goto L8c601058;
L8c601058: wr(m,r[14]+r[0],r[11],2); /* mov.w r11,@(r0,r14) */
 goto L8c60105a;
L8c60105a: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c60105c;
L8c60105c: r[3]=*sr; /* stc SR,r3 */
 goto L8c60105e;
L8c60105e: r[2]=sx(m,0x8c6010eau,2); /* mov.w 0x8c6010ea,r2 */
 goto L8c601060;
L8c601060: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c601062;
L8c601062: r[0]<<=2; /* shll2 r0 */
 goto L8c601064;
L8c601064: r[3]&=r[2]; /* and r2,r3 */
 goto L8c601066;
L8c601066: r[0]<<=2; /* shll2 r0 */
 goto L8c601068;
L8c601068: r[0]|=r[3]; /* or r3,r0 */
 goto L8c60106a;
L8c60106a: *sr=r[0]; /* ldc r0,SR */
 goto L8c60106c;
L8c60106c: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c60106e;
L8c60106e: if((*sr&1u))goto L8c601074; /* bt 0x8c601074 */
 goto L8c601070;
L8c601070: r[0]=r[13];goto L8c601076; /* bra 0x8c601076 */
 goto L8c601074;
L8c601074: r[0]=0u; /* mov #0x0,r0 */
 goto L8c601076;
L8c601076: r[15]+=4u; /* add #0x4,r15 */
 goto L8c601078;
L8c601078: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c60107a;
L8c60107a: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c60107c;
L8c60107c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c60107e;
L8c60107e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c601080;
L8c601080: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
