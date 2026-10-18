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
/* Static C recovery of retail Tikal helper 8c65f348..8c65f390.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_error_report(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f348;
L8c65f348: r[2]=rd(m,0x8c65f3d8u,4); /* mov.l 0x8c65f3d8,r2 */
 goto L8c65f34a;
L8c65f34a: r[0]=sx(m,0x8c65f3c8u,2); /* mov.w 0x8c65f3c8,r0 */
 goto L8c65f34c;
L8c65f34c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65f34e;
L8c65f34e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65f350;
L8c65f350: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c65f352;
L8c65f352: r[1]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r1 */
 goto L8c65f354;
L8c65f354: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c65f356;
L8c65f356: if((*sr&1u))goto L8c65f38a; /* bt 0x8c65f38a */
 goto L8c65f358;
L8c65f358: r[0]=r[15]; /* mov r15,r0 */
 goto L8c65f35a;
L8c65f35a: r[0]+=8u; /* add #0x8,r0 */
 goto L8c65f35c;
L8c65f35c: r[5]=3u; /* mov #0x3,r5 */
 goto L8c65f35e;
L8c65f35e: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65f360;
L8c65f360: *sr=(*sr&~1u)|!(r[0]&r[5]); /* tst r0,r5 */
 goto L8c65f362;
L8c65f362: if((*sr&1u))goto L8c65f36c; /* bt 0x8c65f36c */
 goto L8c65f364;
L8c65f364: r[5]=r[15]; /* mov r15,r5 */
 goto L8c65f366;
L8c65f366: r[5]+=8u; /* add #0x8,r5 */
 goto L8c65f368;
L8c65f368: r[5]+=8u;goto L8c65f372; /* bra 0x8c65f372 */
 goto L8c65f36c;
L8c65f36c: r[5]=r[15]; /* mov r15,r5 */
 goto L8c65f36e;
L8c65f36e: r[5]+=8u; /* add #0x8,r5 */
 goto L8c65f370;
L8c65f370: r[5]+=4u; /* add #0x4,r5 */
 goto L8c65f372;
L8c65f372: r[3]=rd(m,0x8c65f3ecu,4); /* mov.l 0x8c65f3ec,r3 */
 goto L8c65f374;
L8c65f374: r[6]=r[5]; /* mov r5,r6 */
 goto L8c65f376;
L8c65f376: r[4]=rd(m,0x8c65f3e8u,4); /* mov.l 0x8c65f3e8,r4 */
 goto L8c65f378;
L8c65f378: target=r[3];*pr=0x8c65f37cu;r[5]=rd(m,r[15]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65f37c;
L8c65f37c: r[2]=rd(m,0x8c65f3f0u,4); /* mov.l 0x8c65f3f0,r2 */
 goto L8c65f37e;
L8c65f37e: r[14]=rd(m,r[2],4); /* mov.l @r2,r14 */
 goto L8c65f380;
L8c65f380: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c65f382;
L8c65f382: if((*sr&1u))goto L8c65f38a; /* bt 0x8c65f38a */
 goto L8c65f384;
L8c65f384: r[4]=rd(m,0x8c65f3e8u,4); /* mov.l 0x8c65f3e8,r4 */
 goto L8c65f386;
L8c65f386: target=r[14];*pr=0x8c65f38au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c65f38a;
L8c65f38a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65f38c;
L8c65f38c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
