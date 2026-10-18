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
/* Static C recovery of retail Tikal helper 8c10e50e..8c10e566.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_format_emit(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10e50e;
L8c10e50e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10e510;
L8c10e510: r[14]=r[6]; /* mov r6,r14 */
 goto L8c10e512;
L8c10e512: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c10e514;
L8c10e514: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c10e516;
L8c10e516: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c10e518;
L8c10e518: r[11]=r[4]; /* mov r4,r11 */
 goto L8c10e51a;
L8c10e51a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10e51c;
L8c10e51c: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c10e51e;
L8c10e51e: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c10e520;
L8c10e520: take=!(*sr&1u);r[13]=r[5];if(take)goto L8c10e53c;goto L8c10e524; /* bf/s 0x8c10e53c */
 goto L8c10e524;
L8c10e524: r[2]=rd(m,0x8c10e58cu,4); /* mov.l 0x8c10e58c,r2 */
 goto L8c10e526;
L8c10e526: r[5]=r[11]; /* mov r11,r5 */
 goto L8c10e528;
L8c10e528: r[6]=r[13]; /* mov r13,r6 */
 goto L8c10e52a;
L8c10e52a: target=r[2];*pr=0x8c10e52eu;r[4]=rd(m,r[14]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c10e52e;
L8c10e52e: r[3]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r3 */
 goto L8c10e530;
L8c10e530: r[3]+=r[13]; /* add r13,r3 */
 goto L8c10e532;
L8c10e532: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c10e534;
L8c10e534: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c10e536;
L8c10e536: r[2]+=r[13]; /* add r13,r2 */
 goto L8c10e538;
L8c10e538: wr(m,r[14]+0x8u,r[2],4);goto L8c10e55a; /* bra 0x8c10e55a */
 goto L8c10e53c;
L8c10e53c: *sr=(*sr&~1u)|((int32_t)r[13]>0); /* cmp/pl r13 */
 goto L8c10e53e;
L8c10e53e: take=!(*sr&1u);r[12]=0u;if(take)goto L8c10e55a;goto L8c10e542; /* bf/s 0x8c10e55a */
 goto L8c10e542;
L8c10e542: r[5]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r5 */
 goto L8c10e544;
L8c10e544: r[0]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r0 */
 goto L8c10e546;
L8c10e546: target=r[0];*pr=0x8c10e54au;r[4]=sx(m,r[11],1);r[11]+=1;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r0 */
 goto L8c10e54a;
L8c10e54a: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #0xff,r0 */
 goto L8c10e54c;
L8c10e54c: if((*sr&1u))goto L8c10e55a; /* bt 0x8c10e55a */
 goto L8c10e54e;
L8c10e54e: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c10e550;
L8c10e550: r[12]+=1u; /* add #0x1,r12 */
 goto L8c10e552;
L8c10e552: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[13]); /* cmp/ge r13,r12 */
 goto L8c10e554;
L8c10e554: r[2]+=1u; /* add #0x1,r2 */
 goto L8c10e556;
L8c10e556: take=!(*sr&1u);wr(m,r[14]+0x8u,r[2],4);if(take)goto L8c10e542;goto L8c10e55a; /* bf/s 0x8c10e542 */
 goto L8c10e55a;
L8c10e55a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10e55c;
L8c10e55c: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c10e55e;
L8c10e55e: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c10e560;
L8c10e560: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c10e562;
L8c10e562: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
