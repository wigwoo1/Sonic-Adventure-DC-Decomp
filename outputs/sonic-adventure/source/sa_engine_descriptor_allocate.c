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
/* Static C recovery of retail Tikal helper 8c664ac0..8c664aec.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_descriptor_allocate(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c664ac0;
L8c664ac0: r[7]=rd(m,0x8c664b84u,4); /* mov.l 0x8c664b84,r7 */
 goto L8c664ac2;
L8c664ac2: r[6]=rd(m,0x8c664b80u,4); /* mov.l 0x8c664b80,r6 */
 goto L8c664ac4;
L8c664ac4: r[7]+=r[6]; /* add r6,r7 */
 goto L8c664ac6;
L8c664ac6: r[4]=r[6]; /* mov r6,r4 */
 goto L8c664ac8;
L8c664ac8: *sr=(*sr&~1u)|(r[4]>=r[7]); /* cmp/hs r7,r4 */
 goto L8c664aca;
L8c664aca: r[5]=r[6]; /* mov r6,r5 */
 goto L8c664acc;
L8c664acc: take=(*sr&1u);r[6]=1u;if(take)goto L8c664ae6;goto L8c664ad0; /* bt/s 0x8c664ae6 */
 goto L8c664ad0;
L8c664ad0: r[2]=sx(m,r[4],2); /* mov.w @r4,r2 */
 goto L8c664ad2;
L8c664ad2: r[2]=r[2]&65535u; /* extu.w r2,r2 */
 goto L8c664ad4;
L8c664ad4: *sr=(*sr&~1u)|!(r[6]&r[2]); /* tst r6,r2 */
 goto L8c664ad6;
L8c664ad6: if(!(*sr&1u))goto L8c664ade; /* bf 0x8c664ade */
 goto L8c664ad8;
L8c664ad8: wr(m,r[5],r[6],2); /* mov.w r6,@r5 */
 goto L8c664ada;
L8c664ada: r[0]=r[5];return 0; /* rts */
L8c664ade: r[4]+=24u; /* add #0x18,r4 */
 goto L8c664ae0;
L8c664ae0: *sr=(*sr&~1u)|(r[4]>=r[7]); /* cmp/hs r7,r4 */
 goto L8c664ae2;
L8c664ae2: take=!(*sr&1u);r[5]+=24u;if(take)goto L8c664ad0;goto L8c664ae6; /* bf/s 0x8c664ad0 */
 goto L8c664ae6;
L8c664ae6: r[0]=0u; /* mov #0x0,r0 */
 goto L8c664ae8;
L8c664ae8: return 0; /* rts */
}
