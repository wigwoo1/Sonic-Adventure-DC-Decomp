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
/* Static C recovery of retail Tikal helper 8c6453e2..8c64541c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_name_equal_folded(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6453e2;
L8c6453e2: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6453e4;
L8c6453e4: r[14]=r[4]; /* mov r4,r14 */
 goto L8c6453e6;
L8c6453e6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6453e8;
L8c6453e8: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c6453ea;
L8c6453ea: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6453ec;
L8c6453ec: r[13]=r[5];goto L8c645400; /* bra 0x8c645400 */
 goto L8c6453f0;
L8c6453f0: *pr=0x8c6453f4u;r[4]=sx(m,r[14],1);r[14]+=1;status=call(ctx,0x8c645348u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c645348 */
 goto L8c6453f4;
L8c6453f4: r[8]=(uint32_t)(int32_t)(int8_t)r[0]; /* exts.b r0,r8 */
 goto L8c6453f6;
L8c6453f6: *pr=0x8c6453fau;r[4]=sx(m,r[13],1);r[13]+=1;status=call(ctx,0x8c645348u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c645348 */
 goto L8c6453fa;
L8c6453fa: r[0]=(uint32_t)(int32_t)(int8_t)r[0]; /* exts.b r0,r0 */
 goto L8c6453fc;
L8c6453fc: *sr=(*sr&~1u)|(r[0]==r[8]); /* cmp/eq r0,r8 */
 goto L8c6453fe;
L8c6453fe: if(!(*sr&1u))goto L8c64540c; /* bf 0x8c64540c */
 goto L8c645400;
L8c645400: r[3]=sx(m,r[14],1); /* mov.b @r14,r3 */
 goto L8c645402;
L8c645402: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c645404;
L8c645404: if(!(*sr&1u))goto L8c6453f0; /* bf 0x8c6453f0 */
 goto L8c645406;
L8c645406: r[1]=sx(m,r[13],1); /* mov.b @r13,r1 */
 goto L8c645408;
L8c645408: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c64540a;
L8c64540a: if((*sr&1u))goto L8c645410; /* bt 0x8c645410 */
 goto L8c64540c;
L8c64540c: r[0]=0u;goto L8c645412; /* bra 0x8c645412 */
 goto L8c645410;
L8c645410: r[0]=1u; /* mov #0x1,r0 */
 goto L8c645412;
L8c645412: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c645414;
L8c645414: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c645416;
L8c645416: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c645418;
L8c645418: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
