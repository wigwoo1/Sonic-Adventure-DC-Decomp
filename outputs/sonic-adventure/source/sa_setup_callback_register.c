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
/* Static C recovery of retail Tikal helper 8c605234..8c60526a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_setup_callback_register(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c605234;
L8c605234: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c605236;
L8c605236: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c605238;
L8c605238: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c60523a;
L8c60523a: r[14]=rd(m,0x8c605374u,4); /* mov.l 0x8c605374,r14 */
 goto L8c60523c;
L8c60523c: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c60523e;
L8c60523e: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c605240;
L8c605240: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c605242;
L8c605242: if((*sr&1u))goto L8c60524e; /* bt 0x8c60524e */
 goto L8c605244;
L8c605244: r[2]=rd(m,0x8c605378u,4); /* mov.l 0x8c605378,r2 */
 goto L8c605246;
L8c605246: target=r[2];*pr=0x8c60524au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c60524a;
L8c60524a: r[3]=0u; /* mov #0x0,r3 */
 goto L8c60524c;
L8c60524c: wr(m,r[14],r[3],4); /* mov.l r3,@r14 */
 goto L8c60524e;
L8c60524e: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c605250;
L8c605250: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c605252;
L8c605252: if((*sr&1u))goto L8c605262; /* bt 0x8c605262 */
 goto L8c605254;
L8c605254: r[4]=sx(m,0x8c60536eu,2); /* mov.w 0x8c60536e,r4 */
 goto L8c605256;
L8c605256: r[7]=0u; /* mov #0x0,r7 */
 goto L8c605258;
L8c605258: r[3]=rd(m,0x8c60537cu,4); /* mov.l 0x8c60537c,r3 */
 goto L8c60525a;
L8c60525a: r[6]=88u; /* mov #0x58,r6 */
 goto L8c60525c;
L8c60525c: target=r[3];*pr=0x8c605260u;r[5]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c605260;
L8c605260: wr(m,r[14],r[0],4); /* mov.l r0,@r14 */
 goto L8c605262;
L8c605262: r[15]+=4u; /* add #0x4,r15 */
 goto L8c605264;
L8c605264: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c605266;
L8c605266: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
