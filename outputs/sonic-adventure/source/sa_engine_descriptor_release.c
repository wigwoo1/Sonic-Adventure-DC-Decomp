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
/* Static C recovery of retail Tikal helper 8c664aec..8c664b12.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_descriptor_release(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c664aec;
L8c664aec: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c664aee;
L8c664aee: r[5]=rd(m,0x8c664b80u,4); /* mov.l 0x8c664b80,r5 */
 goto L8c664af0;
L8c664af0: r[3]=rd(m,0x8c664b84u,4); /* mov.l 0x8c664b84,r3 */
 goto L8c664af2;
L8c664af2: r[1]=r[5]; /* mov r5,r1 */
 goto L8c664af4;
L8c664af4: r[7]=rd(m,0x8c664b88u,4); /* mov.l 0x8c664b88,r7 */
 goto L8c664af6;
L8c664af6: r[1]+=r[3]; /* add r3,r1 */
 goto L8c664af8;
L8c664af8: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c664afa;
L8c664afa: r[6]=r[5]; /* mov r5,r6 */
 goto L8c664afc;
L8c664afc: *sr=(*sr&~1u)|(r[4]==r[5]); /* cmp/eq r4,r5 */
 goto L8c664afe;
L8c664afe: if(!(*sr&1u))goto L8c664b06; /* bf 0x8c664b06 */
 goto L8c664b00;
L8c664b00: r[2]=sx(m,r[6],2); /* mov.w @r6,r2 */
 goto L8c664b02;
L8c664b02: r[2]&=r[7]; /* and r7,r2 */
 goto L8c664b04;
L8c664b04: wr(m,r[6],r[2],2); /* mov.w r2,@r6 */
 goto L8c664b06;
L8c664b06: r[5]+=24u; /* add #0x18,r5 */
 goto L8c664b08;
L8c664b08: *sr=(*sr&~1u)|(r[5]>=r[1]); /* cmp/hs r1,r5 */
 goto L8c664b0a;
L8c664b0a: take=!(*sr&1u);r[6]+=24u;if(take)goto L8c664afc;goto L8c664b0e; /* bf/s 0x8c664afc */
 goto L8c664b0e;
L8c664b0e: r[15]+=4u;return 0; /* rts */
}
