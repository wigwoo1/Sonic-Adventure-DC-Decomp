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
/* Static C recovery of retail audio stream helper 8c6677e2..8c66780e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_stream_mono_request(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6677e2;
L8c6677e2: r[6]=r[4]; /* mov r4,r6 */
 goto L8c6677e4;
L8c6677e4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6677e6;
L8c6677e6: r[6]+=80u; /* add #0x50,r6 */
 goto L8c6677e8;
L8c6677e8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6677ea;
L8c6677ea: r[0]=80u; /* mov #0x50,r0 */
 goto L8c6677ec;
L8c6677ec: r[15]-=4;wr(m,r[15],r[5],4); /* mov.l r5,@-r15 */
 goto L8c6677ee;
L8c6677ee: r[3]=rd(m,r[6]+0x20u,4); /* mov.l @(0x20,r6),r3 */
 goto L8c6677f0;
L8c6677f0: r[7]=rd(m,r[6]+0x14u,4); /* mov.l @(0x14,r6),r7 */
 goto L8c6677f2;
L8c6677f2: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c6677f4;
L8c6677f4: r[6]=rd(m,r[15],4); /* mov.l @r15,r6 */
 goto L8c6677f6;
L8c6677f6: r[7]+=r[3]; /* add r3,r7 */
 goto L8c6677f8;
L8c6677f8: r[3]=rd(m,0x8c667918u,4); /* mov.l 0x8c667918,r3 */
 goto L8c6677fa;
L8c6677fa: r[5]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r5 */
 goto L8c6677fc;
L8c6677fc: r[14]=rd(m,r[4]+0x8u,4); /* mov.l @(0x8,r4),r14 */
 goto L8c6677fe;
L8c6677fe: target=r[3];*pr=0x8c667802u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c667802;
L8c667802: r[4]=r[14]; /* mov r14,r4 */
 goto L8c667804;
L8c667804: r[15]+=4u; /* add #0x4,r15 */
 goto L8c667806;
L8c667806: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c667808;
L8c667808: r[2]=rd(m,0x8c66791cu,4); /* mov.l 0x8c66791c,r2 */
 goto L8c66780a;
L8c66780a: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
}
