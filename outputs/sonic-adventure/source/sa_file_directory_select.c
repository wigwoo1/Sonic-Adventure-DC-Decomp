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
/* Static C recovery of retail Tikal helper 8c60041a..8c600464.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_directory_select(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c60041a;
L8c60041a: r[3]=rd(m,0x8c600478u,4); /* mov.l 0x8c600478,r3 */
 goto L8c60041c;
L8c60041c: r[6]=0u; /* mov #0x0,r6 */
 goto L8c60041e;
L8c60041e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c600420;
L8c600420: r[14]=r[5]; /* mov r5,r14 */
 goto L8c600422;
L8c600422: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c600424;
L8c600424: r[5]=rd(m,r[3],4); /* mov.l @r3,r5 */
 goto L8c600426;
L8c600426: r[2]=rd(m,0x8c600480u,4); /* mov.l 0x8c600480,r2 */
 goto L8c600428;
L8c600428: target=r[2];*pr=0x8c60042cu;r[5]=rd(m,r[5]+0x10u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c60042c;
L8c60042c: r[5]=r[0]; /* mov r0,r5 */
 goto L8c60042e;
L8c60042e: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c600430;
L8c600430: if(!(*sr&1u))goto L8c60043a; /* bf 0x8c60043a */
 goto L8c600432;
L8c600432: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c600434;
L8c600434: r[0]=4294967288u; /* mov #-0x8,r0 */
 goto L8c600436;
L8c600436: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c60043a: r[2]=44u; /* mov #0x2c,r2 */
 goto L8c60043c;
L8c60043c: r[3]=rd(m,0x8c600478u,4); /* mov.l 0x8c600478,r3 */
 goto L8c60043e;
L8c60043e: r[5]+=4294967295u; /* add #-0x1,r5 */
 goto L8c600440;
L8c600440: r[1]=sx(m,0x8c600470u,2); /* mov.w 0x8c600470,r1 */
 goto L8c600442;
L8c600442: *macl=r[2]*r[5]; /* mul.l r2,r5 */
 goto L8c600444;
L8c600444: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c600446;
L8c600446: r[4]=rd(m,r[4]+0x10u,4); /* mov.l @(0x10,r4),r4 */
 goto L8c600448;
L8c600448: r[5]=*macl; /* sts MACL,r5 */
 goto L8c60044a;
L8c60044a: r[4]+=16u; /* add #0x10,r4 */
 goto L8c60044c;
L8c60044c: r[4]+=r[5]; /* add r5,r4 */
 goto L8c60044e;
L8c60044e: r[0]=rd(m,r[4],4); /* mov.l @r4,r0 */
 goto L8c600450;
L8c600450: r[0]+=r[1]; /* add r1,r0 */
 goto L8c600452;
L8c600452: wr(m,r[14],r[0],4); /* mov.l r0,@r14 */
 goto L8c600454;
L8c600454: r[3]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r3 */
 goto L8c600456;
L8c600456: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c600458;
L8c600458: r[0]=sx(m,r[4]+0x8u,1); /* mov.b @(0x8,r4),r0 */
 goto L8c60045a;
L8c60045a: wr(m,r[14]+0x8u,r[0],1); /* mov.b r0,@(0x8,r14) */
 goto L8c60045c;
L8c60045c: r[0]=0u; /* mov #0x0,r0 */
 goto L8c60045e;
L8c60045e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c600460;
L8c600460: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
