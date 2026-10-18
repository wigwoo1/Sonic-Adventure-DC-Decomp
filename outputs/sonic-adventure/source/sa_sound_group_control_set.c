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
/* Static C recovery of retail Tikal helper 8c66b1a0..8c66b204.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_group_control_set(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66b1a0;
L8c66b1a0: r[3]=rd(m,0x8c66b20cu,4); /* mov.l 0x8c66b20c,r3 */
 goto L8c66b1a2;
L8c66b1a2: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66b1a4;
L8c66b1a4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66b1a6;
L8c66b1a6: r[6]=rd(m,r[3],4); /* mov.l @r3,r6 */
 goto L8c66b1a8;
L8c66b1a8: r[7]=rd(m,0x8c66b208u,4); /* mov.l 0x8c66b208,r7 */
 goto L8c66b1aa;
L8c66b1aa: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c66b1ac;
L8c66b1ac: take=!(*sr&1u);r[14]=0u;if(take)goto L8c66b1b4;goto L8c66b1b0; /* bf/s 0x8c66b1b4 */
 goto L8c66b1b0;
L8c66b1b0: r[14]=r[7];goto L8c66b1f0; /* bra 0x8c66b1f0 */
 goto L8c66b1b4;
L8c66b1b4: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66b1b6;
L8c66b1b6: if(!(*sr&1u))goto L8c66b1be; /* bf 0x8c66b1be */
 goto L8c66b1b8;
L8c66b1b8: r[14]=rd(m,0x8c66b210u,4); /* mov.l 0x8c66b210,r14 */
 goto L8c66b1ba;
L8c66b1ba: goto L8c66b1f0; /* bra 0x8c66b1f0 */
 goto L8c66b1be;
L8c66b1be: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c66b1c0;
L8c66b1c0: if((*sr&1u))goto L8c66b1ee; /* bt 0x8c66b1ee */
 goto L8c66b1c2;
L8c66b1c2: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66b1c4;
L8c66b1c4: r[3]=44u; /* mov #0x2c,r3 */
 goto L8c66b1c6;
L8c66b1c6: r[7]=r[0]; /* mov r0,r7 */
 goto L8c66b1c8;
L8c66b1c8: r[4]=(uint32_t)(int32_t)(int8_t)r[7]; /* exts.b r7,r4 */
 goto L8c66b1ca;
L8c66b1ca: *macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[4]); /* muls.w r3,r4 */
 goto L8c66b1cc;
L8c66b1cc: r[0]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r0 */
 goto L8c66b1ce;
L8c66b1ce: *sr=(*sr&~1u)|(r[0]==0x0000007fu); /* cmp/eq #0x7f,r0 */
 goto L8c66b1d0;
L8c66b1d0: take=!(*sr&1u);r[4]=*macl;if(take)goto L8c66b1de;goto L8c66b1d4; /* bf/s 0x8c66b1de */
 goto L8c66b1d4;
L8c66b1d4: r[1]=sx(m,0x8c66b204u,2); /* mov.w 0x8c66b204,r1 */
 goto L8c66b1d6;
L8c66b1d6: r[1]+=r[6]; /* add r6,r1 */
 goto L8c66b1d8;
L8c66b1d8: r[4]+=r[1]; /* add r1,r4 */
 goto L8c66b1da;
L8c66b1da: r[0]=0u;goto L8c66b1ea; /* bra 0x8c66b1ea */
 goto L8c66b1de;
L8c66b1de: r[1]=sx(m,0x8c66b204u,2); /* mov.w 0x8c66b204,r1 */
 goto L8c66b1e0;
L8c66b1e0: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1; /* shll r5 */
 goto L8c66b1e2;
L8c66b1e2: r[0]=4294967295u; /* mov #-0x1,r0 */
 goto L8c66b1e4;
L8c66b1e4: r[1]+=r[6]; /* add r6,r1 */
 goto L8c66b1e6;
L8c66b1e6: r[0]-=r[5]; /* sub r5,r0 */
 goto L8c66b1e8;
L8c66b1e8: r[4]+=r[1]; /* add r1,r4 */
 goto L8c66b1ea;
L8c66b1ea: wr(m,r[4]+0x6u,r[0],1);goto L8c66b1f0; /* bra 0x8c66b1f0 */
 goto L8c66b1ee;
L8c66b1ee: r[14]=r[7]; /* mov r7,r14 */
 goto L8c66b1f0;
L8c66b1f0: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66b1f2;
L8c66b1f2: if((*sr&1u))goto L8c66b1fc; /* bt 0x8c66b1fc */
 goto L8c66b1f4;
L8c66b1f4: r[3]=rd(m,0x8c66b218u,4); /* mov.l 0x8c66b218,r3 */
 goto L8c66b1f6;
L8c66b1f6: r[4]=rd(m,0x8c66b214u,4); /* mov.l 0x8c66b214,r4 */
 goto L8c66b1f8;
L8c66b1f8: target=r[3];*pr=0x8c66b1fcu;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66b1fc;
L8c66b1fc: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66b1fe;
L8c66b1fe: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66b200;
L8c66b200: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
