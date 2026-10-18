#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_cache_invalidate(void *,uint32_t);
int sa_sh4_cache_purge(void *,uint32_t);
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);
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
/* Static C recovery of retail sound backend byte reader 8c66babc..8c66bb2c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_backend_field4_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66babc;
L8c66babc: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66babe;
L8c66babe: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66bac0;
L8c66bac0: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c66bac2;
L8c66bac2: r[3]=rd(m,0x8c66bb30u,4); /* mov.l 0x8c66bb30,r3 */
 goto L8c66bac4;
L8c66bac4: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c66bac6;
L8c66bac6: r[6]=rd(m,r[3],4); /* mov.l @r3,r6 */
 goto L8c66bac8;
L8c66bac8: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c66baca;
L8c66baca: take=!(*sr&1u);r[14]=0u;if(take)goto L8c66bad4;goto L8c66bace; /* bf/s 0x8c66bad4 */
 goto L8c66bace;
L8c66bace: r[14]=rd(m,0x8c66bb34u,4); /* mov.l 0x8c66bb34,r14 */
 goto L8c66bad0;
L8c66bad0: goto L8c66bb16; /* bra 0x8c66bb16 */
 goto L8c66bad4;
L8c66bad4: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66bad6;
L8c66bad6: if(!(*sr&1u))goto L8c66bade; /* bf 0x8c66bade */
 goto L8c66bad8;
L8c66bad8: r[14]=rd(m,0x8c66bb38u,4); /* mov.l 0x8c66bb38,r14 */
 goto L8c66bada;
L8c66bada: goto L8c66bb16; /* bra 0x8c66bb16 */
 goto L8c66bade;
L8c66bade: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c66bae0;
L8c66bae0: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c66bae2;
L8c66bae2: if(!(*sr&1u))goto L8c66baea; /* bf 0x8c66baea */
 goto L8c66bae4;
L8c66bae4: r[14]=rd(m,0x8c66bb3cu,4); /* mov.l 0x8c66bb3c,r14 */
 goto L8c66bae6;
L8c66bae6: goto L8c66bb16; /* bra 0x8c66bb16 */
 goto L8c66baea;
L8c66baea: r[0]=sx(m,0x8c66bb2cu,2); /* mov.w 0x8c66bb2c,r0 */
 goto L8c66baec;
L8c66baec: r[5]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r5 */
 goto L8c66baee;
L8c66baee: r[3]=sx(m,0x8c66bb2eu,2); /* mov.w 0x8c66bb2e,r3 */
 goto L8c66baf0;
L8c66baf0: r[5]<<=2; /* shll2 r5 */
 goto L8c66baf2;
L8c66baf2: r[2]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r2 */
 goto L8c66baf4;
L8c66baf4: r[5]<<=2; /* shll2 r5 */
 goto L8c66baf6;
L8c66baf6: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66baf8;
L8c66baf8: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1; /* shll r5 */
 goto L8c66bafa;
L8c66bafa: r[3]+=r[2]; /* add r2,r3 */
 goto L8c66bafc;
L8c66bafc: r[2]=rd(m,0x8c66bb40u,4); /* mov.l 0x8c66bb40,r2 */
 goto L8c66bafe;
L8c66bafe: r[0]<<=2; /* shll2 r0 */
 goto L8c66bb00;
L8c66bb00: r[0]<<=2; /* shll2 r0 */
 goto L8c66bb02;
L8c66bb02: r[0]<<=2; /* shll2 r0 */
 goto L8c66bb04;
L8c66bb04: r[0]+=r[3]; /* add r3,r0 */
 goto L8c66bb06;
L8c66bb06: r[0]+=r[5]; /* add r5,r0 */
 goto L8c66bb08;
L8c66bb08: r[4]=r[0]; /* mov r0,r4 */
 goto L8c66bb0a;
L8c66bb0a: r[4]+=16u; /* add #0x10,r4 */
 goto L8c66bb0c;
L8c66bb0c: target=r[2];*pr=0x8c66bb10u;r[5]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66bb10;
L8c66bb10: r[2]=sx(m,r[15],1); /* mov.b @r15,r2 */
 goto L8c66bb12;
L8c66bb12: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c66bb14;
L8c66bb14: wr(m,r[3],r[2],1); /* mov.b r2,@r3 */
 goto L8c66bb16;
L8c66bb16: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66bb18;
L8c66bb18: if((*sr&1u))goto L8c66bb22; /* bt 0x8c66bb22 */
 goto L8c66bb1a;
L8c66bb1a: r[2]=rd(m,0x8c66bb48u,4); /* mov.l 0x8c66bb48,r2 */
 goto L8c66bb1c;
L8c66bb1c: r[4]=rd(m,0x8c66bb44u,4); /* mov.l 0x8c66bb44,r4 */
 goto L8c66bb1e;
L8c66bb1e: target=r[2];*pr=0x8c66bb22u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66bb22;
L8c66bb22: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66bb24;
L8c66bb24: r[15]+=8u; /* add #0x8,r15 */
 goto L8c66bb26;
L8c66bb26: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66bb28;
L8c66bb28: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
