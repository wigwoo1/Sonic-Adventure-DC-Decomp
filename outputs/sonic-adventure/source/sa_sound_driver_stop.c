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
/* Static C recovery of retail Tikal helper 8c65a200..8c65a268.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_stop(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a200;
L8c65a200: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a202;
L8c65a202: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65a204;
L8c65a204: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c65a206;
L8c65a206: r[0]=*sr; /* stc SR,r0 */
 goto L8c65a208;
L8c65a208: r[3]=sx(m,0x8c65a268u,2); /* mov.w 0x8c65a268,r3 */
 goto L8c65a20a;
L8c65a20a: r[0]>>=2; /* shlr2 r0 */
 goto L8c65a20c;
L8c65a20c: r[0]>>=2; /* shlr2 r0 */
 goto L8c65a20e;
L8c65a20e: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c65a210;
L8c65a210: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c65a212;
L8c65a212: r[0]=*sr; /* stc SR,r0 */
 goto L8c65a214;
L8c65a214: r[0]&=r[3]; /* and r3,r0 */
 goto L8c65a216;
L8c65a216: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c65a218;
L8c65a218: *sr=r[0]; /* ldc r0,SR */
 goto L8c65a21a;
L8c65a21a: r[14]=1u; /* mov #0x1,r14 */
 goto L8c65a21c;
L8c65a21c: r[4]=rd(m,0x8c65a270u,4); /* mov.l 0x8c65a270,r4 */
 goto L8c65a21e;
L8c65a21e: r[5]=rd(m,0x8c65a26cu,4); /* mov.l 0x8c65a26c,r5 */
 goto L8c65a220;
L8c65a220: r[7]=r[14]; /* mov r14,r7 */
 goto L8c65a222;
L8c65a222: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a224;
L8c65a224: r[2]=rd(m,0x8c65a274u,4); /* mov.l 0x8c65a274,r2 */
 goto L8c65a226;
L8c65a226: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a228;
L8c65a228: target=r[2];*pr=0x8c65a22cu;r[6]=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65a22c;
L8c65a22c: r[3]=rd(m,0x8c65a26cu,4); /* mov.l 0x8c65a26c,r3 */
 goto L8c65a22e;
L8c65a22e: r[7]=r[14]; /* mov r14,r7 */
 goto L8c65a230;
L8c65a230: r[5]=rd(m,0x8c65a270u,4); /* mov.l 0x8c65a270,r5 */
 goto L8c65a232;
L8c65a232: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c65a234;
L8c65a234: r[4]=r[3]; /* mov r3,r4 */
 goto L8c65a236;
L8c65a236: r[2]|=r[14]; /* or r14,r2 */
 goto L8c65a238;
L8c65a238: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c65a23a;
L8c65a23a: r[3]=rd(m,0x8c65a278u,4); /* mov.l 0x8c65a278,r3 */
 goto L8c65a23c;
L8c65a23c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a23e;
L8c65a23e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65a240;
L8c65a240: target=r[3];*pr=0x8c65a244u;r[6]=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a244;
L8c65a244: r[15]+=16u; /* add #0x10,r15 */
 goto L8c65a246;
L8c65a246: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c65a248;
L8c65a248: r[2]=*sr; /* stc SR,r2 */
 goto L8c65a24a;
L8c65a24a: r[3]=sx(m,0x8c65a268u,2); /* mov.w 0x8c65a268,r3 */
 goto L8c65a24c;
L8c65a24c: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c65a24e;
L8c65a24e: r[0]<<=2; /* shll2 r0 */
 goto L8c65a250;
L8c65a250: r[2]&=r[3]; /* and r3,r2 */
 goto L8c65a252;
L8c65a252: r[0]<<=2; /* shll2 r0 */
 goto L8c65a254;
L8c65a254: r[0]|=r[2]; /* or r2,r0 */
 goto L8c65a256;
L8c65a256: *sr=r[0]; /* ldc r0,SR */
 goto L8c65a258;
L8c65a258: r[4]=sx(m,0x8c65a26au,2); /* mov.w 0x8c65a26a,r4 */
 goto L8c65a25a;
L8c65a25a: r[4]--;*sr=(*sr&~1u)|(!r[4]); /* dt r4 */
 goto L8c65a25c;
L8c65a25c: if(!(*sr&1u))goto L8c65a25a; /* bf 0x8c65a25a */
 goto L8c65a25e;
L8c65a25e: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65a260;
L8c65a260: r[15]+=4u; /* add #0x4,r15 */
 goto L8c65a262;
L8c65a262: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65a264;
L8c65a264: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
