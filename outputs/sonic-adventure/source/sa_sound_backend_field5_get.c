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
/* Static C recovery of retail sound backend byte reader 8c66b9b0..8c66ba14.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_backend_field5_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66b9b0;
L8c66b9b0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66b9b2;
L8c66b9b2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66b9b4;
L8c66b9b4: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c66b9b6;
L8c66b9b6: r[3]=rd(m,0x8c66ba18u,4); /* mov.l 0x8c66ba18,r3 */
 goto L8c66b9b8;
L8c66b9b8: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c66b9ba;
L8c66b9ba: r[6]=rd(m,r[3],4); /* mov.l @r3,r6 */
 goto L8c66b9bc;
L8c66b9bc: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c66b9be;
L8c66b9be: take=!(*sr&1u);r[14]=0u;if(take)goto L8c66b9c8;goto L8c66b9c2; /* bf/s 0x8c66b9c8 */
 goto L8c66b9c2;
L8c66b9c2: r[14]=rd(m,0x8c66ba1cu,4); /* mov.l 0x8c66ba1c,r14 */
 goto L8c66b9c4;
L8c66b9c4: goto L8c66ba0a; /* bra 0x8c66ba0a */
 goto L8c66b9c8;
L8c66b9c8: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66b9ca;
L8c66b9ca: if(!(*sr&1u))goto L8c66b9d2; /* bf 0x8c66b9d2 */
 goto L8c66b9cc;
L8c66b9cc: r[14]=rd(m,0x8c66ba20u,4); /* mov.l 0x8c66ba20,r14 */
 goto L8c66b9ce;
L8c66b9ce: goto L8c66ba0a; /* bra 0x8c66ba0a */
 goto L8c66b9d2;
L8c66b9d2: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c66b9d4;
L8c66b9d4: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c66b9d6;
L8c66b9d6: if(!(*sr&1u))goto L8c66b9de; /* bf 0x8c66b9de */
 goto L8c66b9d8;
L8c66b9d8: r[14]=rd(m,0x8c66ba24u,4); /* mov.l 0x8c66ba24,r14 */
 goto L8c66b9da;
L8c66b9da: goto L8c66ba0a; /* bra 0x8c66ba0a */
 goto L8c66b9de;
L8c66b9de: r[0]=sx(m,0x8c66ba14u,2); /* mov.w 0x8c66ba14,r0 */
 goto L8c66b9e0;
L8c66b9e0: r[5]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r5 */
 goto L8c66b9e2;
L8c66b9e2: r[3]=sx(m,0x8c66ba16u,2); /* mov.w 0x8c66ba16,r3 */
 goto L8c66b9e4;
L8c66b9e4: r[5]<<=2; /* shll2 r5 */
 goto L8c66b9e6;
L8c66b9e6: r[2]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r2 */
 goto L8c66b9e8;
L8c66b9e8: r[5]<<=2; /* shll2 r5 */
 goto L8c66b9ea;
L8c66b9ea: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66b9ec;
L8c66b9ec: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1; /* shll r5 */
 goto L8c66b9ee;
L8c66b9ee: r[3]+=r[2]; /* add r2,r3 */
 goto L8c66b9f0;
L8c66b9f0: r[2]=rd(m,0x8c66ba28u,4); /* mov.l 0x8c66ba28,r2 */
 goto L8c66b9f2;
L8c66b9f2: r[0]<<=2; /* shll2 r0 */
 goto L8c66b9f4;
L8c66b9f4: r[0]<<=2; /* shll2 r0 */
 goto L8c66b9f6;
L8c66b9f6: r[0]<<=2; /* shll2 r0 */
 goto L8c66b9f8;
L8c66b9f8: r[0]+=r[3]; /* add r3,r0 */
 goto L8c66b9fa;
L8c66b9fa: r[0]+=r[5]; /* add r5,r0 */
 goto L8c66b9fc;
L8c66b9fc: r[4]=r[0]; /* mov r0,r4 */
 goto L8c66b9fe;
L8c66b9fe: r[4]+=17u; /* add #0x11,r4 */
 goto L8c66ba00;
L8c66ba00: target=r[2];*pr=0x8c66ba04u;r[5]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66ba04;
L8c66ba04: r[2]=sx(m,r[15],1); /* mov.b @r15,r2 */
 goto L8c66ba06;
L8c66ba06: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c66ba08;
L8c66ba08: wr(m,r[3],r[2],1); /* mov.b r2,@r3 */
 goto L8c66ba0a;
L8c66ba0a: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66ba0c;
L8c66ba0c: r[15]+=8u; /* add #0x8,r15 */
 goto L8c66ba0e;
L8c66ba0e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66ba10;
L8c66ba10: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
