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
/* Static C recovery of retail Tikal helper 8c66b13c..8c66b180.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_descriptor_command2e(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66b13c;
L8c66b13c: r[2]=rd(m,0x8c66b180u,4); /* mov.l 0x8c66b180,r2 */
 goto L8c66b13e;
L8c66b13e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66b140;
L8c66b140: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66b142;
L8c66b142: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c66b144;
L8c66b144: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c66b146;
L8c66b146: if(!(*sr&1u))goto L8c66b14e; /* bf 0x8c66b14e */
 goto L8c66b148;
L8c66b148: r[14]=rd(m,0x8c66b184u,4); /* mov.l 0x8c66b184,r14 */
 goto L8c66b14a;
L8c66b14a: goto L8c66b16c; /* bra 0x8c66b16c */
 goto L8c66b14e;
L8c66b14e: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66b150;
L8c66b150: if(!(*sr&1u))goto L8c66b158; /* bf 0x8c66b158 */
 goto L8c66b152;
L8c66b152: r[14]=rd(m,0x8c66b188u,4); /* mov.l 0x8c66b188,r14 */
 goto L8c66b154;
L8c66b154: goto L8c66b16c; /* bra 0x8c66b16c */
 goto L8c66b158;
L8c66b158: r[2]=rd(m,0x8c66b18cu,4); /* mov.l 0x8c66b18c,r2 */
 goto L8c66b15a;
L8c66b15a: r[3]=46u; /* mov #0x2e,r3 */
 goto L8c66b15c;
L8c66b15c: r[1]=rd(m,0x8c66b190u,4); /* mov.l 0x8c66b190,r1 */
 goto L8c66b15e;
L8c66b15e: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c66b160;
L8c66b160: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66b162;
L8c66b162: r[3]=rd(m,0x8c66b194u,4); /* mov.l 0x8c66b194,r3 */
 goto L8c66b164;
L8c66b164: wr(m,r[1],r[0],1); /* mov.b r0,@r1 */
 goto L8c66b166;
L8c66b166: target=r[3];*pr=0x8c66b16au;r[4]=r[2];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66b16a;
L8c66b16a: r[14]=r[0]; /* mov r0,r14 */
 goto L8c66b16c;
L8c66b16c: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66b16e;
L8c66b16e: if((*sr&1u))goto L8c66b178; /* bt 0x8c66b178 */
 goto L8c66b170;
L8c66b170: r[3]=rd(m,0x8c66b19cu,4); /* mov.l 0x8c66b19c,r3 */
 goto L8c66b172;
L8c66b172: r[4]=rd(m,0x8c66b198u,4); /* mov.l 0x8c66b198,r4 */
 goto L8c66b174;
L8c66b174: target=r[3];*pr=0x8c66b178u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66b178;
L8c66b178: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66b17a;
L8c66b17a: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66b17c;
L8c66b17c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
