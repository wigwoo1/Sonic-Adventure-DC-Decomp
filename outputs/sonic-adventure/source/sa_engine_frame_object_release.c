/* Sonic Adventure USA Rev A recovered object configuration/release routine. */
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
/* Static C recovery of retail Tikal helper 8c6435bc..8c64360c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_frame_object_release(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6435bc;
L8c6435bc: r[2]=rd(m,0x8c643610u,4); /* mov.l 0x8c643610,r2 */
 goto L8c6435be;
L8c6435be: r[6]=0u; /* mov #0x0,r6 */
 goto L8c6435c0;
L8c6435c0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6435c2;
L8c6435c2: r[14]=r[6]; /* mov r6,r14 */
 goto L8c6435c4;
L8c6435c4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6435c6;
L8c6435c6: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c6435c8;
L8c6435c8: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c6435ca;
L8c6435ca: take=!(*sr&1u);r[5]=r[4];if(take)goto L8c6435d4;goto L8c6435ce; /* bf/s 0x8c6435d4 */
 goto L8c6435ce;
L8c6435ce: r[14]=rd(m,0x8c643614u,4); /* mov.l 0x8c643614,r14 */
 goto L8c6435d0;
L8c6435d0: goto L8c6435f8; /* bra 0x8c6435f8 */
 goto L8c6435d4;
L8c6435d4: r[7]=rd(m,0x8c643618u,4); /* mov.l 0x8c643618,r7 */
 goto L8c6435d6;
L8c6435d6: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c6435d8;
L8c6435d8: if(!(*sr&1u))goto L8c6435de; /* bf 0x8c6435de */
 goto L8c6435da;
L8c6435da: r[14]=r[7];goto L8c6435f8; /* bra 0x8c6435f8 */
 goto L8c6435de;
L8c6435de: r[0]=rd(m,r[5],4); /* mov.l @r5,r0 */
 goto L8c6435e0;
L8c6435e0: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c6435e2;
L8c6435e2: if(!(*sr&1u))goto L8c6435f6; /* bf 0x8c6435f6 */
 goto L8c6435e4;
L8c6435e4: r[3]=rd(m,0x8c643610u,4); /* mov.l 0x8c643610,r3 */
 goto L8c6435e6;
L8c6435e6: r[0]=sx(m,0x8c64360cu,2); /* mov.w 0x8c64360c,r0 */
 goto L8c6435e8;
L8c6435e8: wr(m,r[5],r[6],4); /* mov.l r6,@r5 */
 goto L8c6435ea;
L8c6435ea: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c6435ec;
L8c6435ec: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c6435ee;
L8c6435ee: r[1]+=4294967295u; /* add #-0x1,r1 */
 goto L8c6435f0;
L8c6435f0: wr(m,r[2]+r[0],r[1],4); /* mov.l r1,@(r0,r2) */
 goto L8c6435f2;
L8c6435f2: goto L8c6435f8; /* bra 0x8c6435f8 */
 goto L8c6435f6;
L8c6435f6: r[14]=r[7]; /* mov r7,r14 */
 goto L8c6435f8;
L8c6435f8: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c6435fa;
L8c6435fa: if((*sr&1u))goto L8c643604; /* bt 0x8c643604 */
 goto L8c6435fc;
L8c6435fc: r[2]=rd(m,0x8c643620u,4); /* mov.l 0x8c643620,r2 */
 goto L8c6435fe;
L8c6435fe: r[4]=rd(m,0x8c64361cu,4); /* mov.l 0x8c64361c,r4 */
 goto L8c643600;
L8c643600: target=r[2];*pr=0x8c643604u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c643604;
L8c643604: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c643606;
L8c643606: r[0]=r[14]; /* mov r14,r0 */
 goto L8c643608;
L8c643608: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
