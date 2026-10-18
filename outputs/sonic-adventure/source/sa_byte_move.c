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
/* Static C recovery of retail Tikal helper 8c10d25c..8c10d31c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_byte_move(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d25c;
L8c10d25c: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c10d25e;
L8c10d25e: *sr=(*sr&~1u)|(r[5]==r[4]); /* cmp/eq r5,r4 */
 goto L8c10d260;
L8c10d260: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c10d262;
L8c10d262: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c10d264;
L8c10d264: if((*sr&1u))goto L8c10d310; /* bt 0x8c10d310 */
 goto L8c10d266;
L8c10d266: r[2]=0u; /* mov #0x0,r2 */
 goto L8c10d268;
L8c10d268: *sr=(*sr&~1u)|(r[6]>r[2]); /* cmp/hi r2,r6 */
 goto L8c10d26a;
L8c10d26a: if(!(*sr&1u))goto L8c10d310; /* bf 0x8c10d310 */
 goto L8c10d26c;
L8c10d26c: r[7]=r[4]; /* mov r4,r7 */
 goto L8c10d26e;
L8c10d26e: r[11]=r[6]&65535u; /* extu.w r6,r11 */
 goto L8c10d270;
L8c10d270: r[7]|=r[5]; /* or r5,r7 */
 goto L8c10d272;
L8c10d272: r[12]=r[6]; /* mov r6,r12 */
 goto L8c10d274;
L8c10d274: *sr=(*sr&~1u)|(r[11]&1u);r[11]>>=1; /* shlr r11 */
 goto L8c10d276;
L8c10d276: *sr=(*sr&~1u)|(r[4]>=r[5]); /* cmp/hs r5,r4 */
 goto L8c10d278;
L8c10d278: r[7]|=r[6]; /* or r6,r7 */
 goto L8c10d27a;
L8c10d27a: take=(*sr&1u);r[12]>>=2;if(take)goto L8c10d2c6;goto L8c10d27e; /* bt/s 0x8c10d2c6 */
 goto L8c10d27e;
L8c10d27e: r[3]=1u; /* mov #0x1,r3 */
 goto L8c10d280;
L8c10d280: *sr=(*sr&~1u)|!(r[7]&r[3]); /* tst r7,r3 */
 goto L8c10d282;
L8c10d282: if(!(*sr&1u))goto L8c10d2b2; /* bf 0x8c10d2b2 */
 goto L8c10d284;
L8c10d284: r[1]=3u; /* mov #0x3,r1 */
 goto L8c10d286;
L8c10d286: *sr=(*sr&~1u)|!(r[1]&r[7]); /* tst r1,r7 */
 goto L8c10d288;
L8c10d288: if(!(*sr&1u))goto L8c10d29e; /* bf 0x8c10d29e */
 goto L8c10d28a;
L8c10d28a: r[6]=r[12]; /* mov r12,r6 */
 goto L8c10d28c;
L8c10d28c: r[7]=r[4]; /* mov r4,r7 */
 goto L8c10d28e;
L8c10d28e: r[3]=rd(m,r[5],4);r[5]+=4; /* mov.l @r5+,r3 */
 goto L8c10d290;
L8c10d290: r[6]+=4294967295u; /* add #-0x1,r6 */
 goto L8c10d292;
L8c10d292: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c10d294;
L8c10d294: wr(m,r[7],r[3],4); /* mov.l r3,@r7 */
 goto L8c10d296;
L8c10d296: take=!(*sr&1u);r[7]+=4u;if(take)goto L8c10d28e;goto L8c10d29a; /* bf/s 0x8c10d28e */
 goto L8c10d29a;
L8c10d29a: goto L8c10d310; /* bra 0x8c10d310 */
 goto L8c10d29e;
L8c10d29e: r[6]=r[11]; /* mov r11,r6 */
 goto L8c10d2a0;
L8c10d2a0: r[7]=r[4]; /* mov r4,r7 */
 goto L8c10d2a2;
L8c10d2a2: r[3]=sx(m,r[5],2);r[5]+=2; /* mov.w @r5+,r3 */
 goto L8c10d2a4;
L8c10d2a4: r[6]+=4294967295u; /* add #-0x1,r6 */
 goto L8c10d2a6;
L8c10d2a6: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c10d2a8;
L8c10d2a8: wr(m,r[7],r[3],2); /* mov.w r3,@r7 */
 goto L8c10d2aa;
L8c10d2aa: take=!(*sr&1u);r[7]+=2u;if(take)goto L8c10d2a2;goto L8c10d2ae; /* bf/s 0x8c10d2a2 */
 goto L8c10d2ae;
L8c10d2ae: goto L8c10d310; /* bra 0x8c10d310 */
 goto L8c10d2b2;
L8c10d2b2: r[0]=r[5]; /* mov r5,r0 */
 goto L8c10d2b4;
L8c10d2b4: r[7]=r[4]; /* mov r4,r7 */
 goto L8c10d2b6;
L8c10d2b6: r[3]=sx(m,r[0],1);r[0]+=1; /* mov.b @r0+,r3 */
 goto L8c10d2b8;
L8c10d2b8: r[6]+=4294967295u; /* add #-0x1,r6 */
 goto L8c10d2ba;
L8c10d2ba: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c10d2bc;
L8c10d2bc: wr(m,r[7],r[3],1); /* mov.b r3,@r7 */
 goto L8c10d2be;
L8c10d2be: take=!(*sr&1u);r[7]+=1u;if(take)goto L8c10d2b6;goto L8c10d2c2; /* bf/s 0x8c10d2b6 */
 goto L8c10d2c2;
L8c10d2c2: goto L8c10d310; /* bra 0x8c10d310 */
 goto L8c10d2c6;
L8c10d2c6: r[2]=1u; /* mov #0x1,r2 */
 goto L8c10d2c8;
L8c10d2c8: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10d2ca;
L8c10d2ca: *sr=(*sr&~1u)|!(r[7]&r[2]); /* tst r7,r2 */
 goto L8c10d2cc;
L8c10d2cc: r[13]=r[5]; /* mov r5,r13 */
 goto L8c10d2ce;
L8c10d2ce: r[0]+=r[6]; /* add r6,r0 */
 goto L8c10d2d0;
L8c10d2d0: take=!(*sr&1u);r[13]+=r[6];if(take)goto L8c10d302;goto L8c10d2d4; /* bf/s 0x8c10d302 */
 goto L8c10d2d4;
L8c10d2d4: r[1]=3u; /* mov #0x3,r1 */
 goto L8c10d2d6;
L8c10d2d6: *sr=(*sr&~1u)|!(r[1]&r[7]); /* tst r1,r7 */
 goto L8c10d2d8;
L8c10d2d8: if(!(*sr&1u))goto L8c10d2ee; /* bf 0x8c10d2ee */
 goto L8c10d2da;
L8c10d2da: r[6]=r[12]; /* mov r12,r6 */
 goto L8c10d2dc;
L8c10d2dc: r[7]=r[13]; /* mov r13,r7 */
 goto L8c10d2de;
L8c10d2de: r[5]=r[0]; /* mov r0,r5 */
 goto L8c10d2e0;
L8c10d2e0: r[7]+=4294967292u; /* add #-0x4,r7 */
 goto L8c10d2e2;
L8c10d2e2: r[3]=rd(m,r[7],4); /* mov.l @r7,r3 */
 goto L8c10d2e4;
L8c10d2e4: r[6]--;*sr=(*sr&~1u)|(!r[6]); /* dt r6 */
 goto L8c10d2e6;
L8c10d2e6: take=!(*sr&1u);r[5]-=4;wr(m,r[5],r[3],4);if(take)goto L8c10d2e0;goto L8c10d2ea; /* bf/s 0x8c10d2e0 */
 goto L8c10d2ea;
L8c10d2ea: goto L8c10d310; /* bra 0x8c10d310 */
 goto L8c10d2ee;
L8c10d2ee: r[6]=r[11]; /* mov r11,r6 */
 goto L8c10d2f0;
L8c10d2f0: r[5]=r[13]; /* mov r13,r5 */
 goto L8c10d2f2;
L8c10d2f2: r[7]=r[0]; /* mov r0,r7 */
 goto L8c10d2f4;
L8c10d2f4: r[5]+=4294967294u; /* add #-0x2,r5 */
 goto L8c10d2f6;
L8c10d2f6: r[3]=sx(m,r[5],2); /* mov.w @r5,r3 */
 goto L8c10d2f8;
L8c10d2f8: r[6]--;*sr=(*sr&~1u)|(!r[6]); /* dt r6 */
 goto L8c10d2fa;
L8c10d2fa: take=!(*sr&1u);r[7]-=2;wr(m,r[7],r[3],2);if(take)goto L8c10d2f4;goto L8c10d2fe; /* bf/s 0x8c10d2f4 */
 goto L8c10d2fe;
L8c10d2fe: goto L8c10d310; /* bra 0x8c10d310 */
 goto L8c10d302;
L8c10d302: r[7]=r[13]; /* mov r13,r7 */
 goto L8c10d304;
L8c10d304: r[5]=r[0]; /* mov r0,r5 */
 goto L8c10d306;
L8c10d306: r[7]+=4294967295u; /* add #-0x1,r7 */
 goto L8c10d308;
L8c10d308: r[3]=sx(m,r[7],1); /* mov.b @r7,r3 */
 goto L8c10d30a;
L8c10d30a: r[6]--;*sr=(*sr&~1u)|(!r[6]); /* dt r6 */
 goto L8c10d30c;
L8c10d30c: take=!(*sr&1u);r[5]-=1;wr(m,r[5],r[3],1);if(take)goto L8c10d306;goto L8c10d310; /* bf/s 0x8c10d306 */
 goto L8c10d310;
L8c10d310: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c10d312;
L8c10d312: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10d314;
L8c10d314: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c10d316;
L8c10d316: r[13]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
