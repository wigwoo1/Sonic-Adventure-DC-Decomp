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
/* Static C recovery of retail Tikal helper 8c643624..8c6436a4.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_frame_object_configure(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c643624;
L8c643624: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c643626;
L8c643626: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c643628;
L8c643628: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c64362a;
L8c64362a: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c64362c;
L8c64362c: r[2]=rd(m,0x8c6436a4u,4); /* mov.l 0x8c6436a4,r2 */
 goto L8c64362e;
L8c64362e: r[14]=r[4]; /* mov r4,r14 */
 goto L8c643630;
L8c643630: wr(m,r[15]+0xcu,r[5],4); /* mov.l r5,@(0xc,r15) */
 goto L8c643632;
L8c643632: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c643634;
L8c643634: wr(m,r[15]+0x8u,r[7],4); /* mov.l r7,@(0x8,r15) */
 goto L8c643636;
L8c643636: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c643638;
L8c643638: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c64363a;
L8c64363a: take=!(*sr&1u);r[13]=0u;if(take)goto L8c643644;goto L8c64363e; /* bf/s 0x8c643644 */
 goto L8c64363e;
L8c64363e: r[13]=rd(m,0x8c6436a8u,4); /* mov.l 0x8c6436a8,r13 */
 goto L8c643640;
L8c643640: goto L8c64368c; /* bra 0x8c64368c */
 goto L8c643644;
L8c643644: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c643646;
L8c643646: if(!(*sr&1u))goto L8c64364e; /* bf 0x8c64364e */
 goto L8c643648;
L8c643648: r[13]=rd(m,0x8c6436acu,4); /* mov.l 0x8c6436ac,r13 */
 goto L8c64364a;
L8c64364a: goto L8c64368c; /* bra 0x8c64368c */
 goto L8c64364e;
L8c64364e: r[3]=rd(m,0x8c6436b0u,4); /* mov.l 0x8c6436b0,r3 */
 goto L8c643650;
L8c643650: r[5]=r[15]; /* mov r15,r5 */
 goto L8c643652;
L8c643652: target=r[3];*pr=0x8c643656u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643656;
L8c643656: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c643658;
L8c643658: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c64365a;
L8c64365a: if(!(*sr&1u))goto L8c643662; /* bf 0x8c643662 */
 goto L8c64365c;
L8c64365c: r[13]=rd(m,0x8c6436b4u,4); /* mov.l 0x8c6436b4,r13 */
 goto L8c64365e;
L8c64365e: goto L8c64368c; /* bra 0x8c64368c */
 goto L8c643662;
L8c643662: r[3]=rd(m,0x8c6436b8u,4); /* mov.l 0x8c6436b8,r3 */
 goto L8c643664;
L8c643664: r[5]=0u; /* mov #0x0,r5 */
 goto L8c643666;
L8c643666: target=r[3];*pr=0x8c64366au;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c64366a;
L8c64366a: r[3]=rd(m,0x8c6436bcu,4); /* mov.l 0x8c6436bc,r3 */
 goto L8c64366c;
L8c64366c: r[5]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r5 */
 goto L8c64366e;
L8c64366e: target=r[3];*pr=0x8c643672u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643672;
L8c643672: r[3]=rd(m,0x8c6436c0u,4); /* mov.l 0x8c6436c0,r3 */
 goto L8c643674;
L8c643674: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c643676;
L8c643676: target=r[3];*pr=0x8c64367au;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c64367a;
L8c64367a: r[3]=rd(m,0x8c6436c4u,4); /* mov.l 0x8c6436c4,r3 */
 goto L8c64367c;
L8c64367c: r[5]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r5 */
 goto L8c64367e;
L8c64367e: r[6]=rd(m,r[15]+0x1cu,4); /* mov.l @(0x1c,r15),r6 */
 goto L8c643680;
L8c643680: target=r[3];*pr=0x8c643684u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643684;
L8c643684: r[2]=rd(m,0x8c6436b8u,4); /* mov.l 0x8c6436b8,r2 */
 goto L8c643686;
L8c643686: r[5]=0u; /* mov #0x0,r5 */
 goto L8c643688;
L8c643688: target=r[2];*pr=0x8c64368cu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c64368c;
L8c64368c: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c64368e;
L8c64368e: if((*sr&1u))goto L8c643698; /* bt 0x8c643698 */
 goto L8c643690;
L8c643690: r[2]=rd(m,0x8c6436ccu,4); /* mov.l 0x8c6436cc,r2 */
 goto L8c643692;
L8c643692: r[4]=rd(m,0x8c6436c8u,4); /* mov.l 0x8c6436c8,r4 */
 goto L8c643694;
L8c643694: target=r[2];*pr=0x8c643698u;r[5]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c643698;
L8c643698: r[0]=r[13]; /* mov r13,r0 */
 goto L8c64369a;
L8c64369a: r[15]+=16u; /* add #0x10,r15 */
 goto L8c64369c;
L8c64369c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c64369e;
L8c64369e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6436a0;
L8c6436a0: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
