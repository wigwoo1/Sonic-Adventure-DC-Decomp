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
/* Static C recovery of retail Tikal helper 8c6687bc..8c6687f4.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_power_two_factor(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6687bc;
L8c6687bc: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6687be;
L8c6687be: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c6687c0;
L8c6687c0: if(!(*sr&1u))goto L8c6687ca; /* bf 0x8c6687ca */
 goto L8c6687c2;
L8c6687c2: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6687c4;
L8c6687c4: r[2]=0u; /* mov #0x0,r2 */
 goto L8c6687c6;
L8c6687c6: wr(m,r[6],r[2],4);return 0; /* rts */
L8c6687ca: r[2]=1u; /* mov #0x1,r2 */
 goto L8c6687cc;
L8c6687cc: wr(m,r[6],r[2],4); /* mov.l r2,@r6 */
 goto L8c6687ce;
L8c6687ce: r[7]=64u; /* mov #0x40,r7 */
 goto L8c6687d0;
L8c6687d0: r[4]=2u;goto L8c6687e8; /* bra 0x8c6687e8 */
 goto L8c6687d4;
L8c6687d4: r[2]=rd(m,0x8c668850u,4); /* mov.l 0x8c668850,r2 */
 goto L8c6687d6;
L8c6687d6: r[1]=r[5]; /* mov r5,r1 */
 goto L8c6687d8;
L8c6687d8: target=r[2];*pr=0x8c6687dcu;r[0]=r[4];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6687dc;
L8c6687dc: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c6687de;
L8c6687de: if(!(*sr&1u))goto L8c6687ee; /* bf 0x8c6687ee */
 goto L8c6687e0;
L8c6687e0: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c6687e2;
L8c6687e2: r[4]+=r[4]; /* add r4,r4 */
 goto L8c6687e4;
L8c6687e4: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c6687e6;
L8c6687e6: wr(m,r[6],r[3],4); /* mov.l r3,@r6 */
 goto L8c6687e8;
L8c6687e8: r[2]=rd(m,r[6],4); /* mov.l @r6,r2 */
 goto L8c6687ea;
L8c6687ea: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[7]); /* cmp/gt r7,r2 */
 goto L8c6687ec;
L8c6687ec: if(!(*sr&1u))goto L8c6687d4; /* bf 0x8c6687d4 */
 goto L8c6687ee;
L8c6687ee: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6687f0;
L8c6687f0: return 0; /* rts */
}
