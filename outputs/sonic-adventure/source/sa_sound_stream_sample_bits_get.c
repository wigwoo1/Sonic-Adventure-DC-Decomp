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
int sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);
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
/* Static C recovery of retail Tikal helper 8c6676b2..8c6676ee.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_stream_sample_bits_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6676b2;
L8c6676b2: r[0]=sx(m,0x8c6677c0u,2); /* mov.w 0x8c6677c0,r0 */
 goto L8c6676b4;
L8c6676b4: r[3]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r3 */
 goto L8c6676b6;
L8c6676b6: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c6676b8;
L8c6676b8: if((*sr&1u))goto L8c6676e8; /* bt 0x8c6676e8 */
 goto L8c6676ba;
L8c6676ba: r[0]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r0 */
 goto L8c6676bc;
L8c6676bc: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c6676be;
L8c6676be: if(!(*sr&1u))goto L8c6676d4; /* bf 0x8c6676d4 */
 goto L8c6676c0;
L8c6676c0: r[0]=sx(m,0x8c6677c2u,2); /* mov.w 0x8c6677c2,r0 */
 goto L8c6676c2;
L8c6676c2: r[0]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r0 */
 goto L8c6676c4;
L8c6676c4: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c6676c6;
L8c6676c6: if((*sr&1u))goto L8c6676e4; /* bt 0x8c6676e4 */
 goto L8c6676c8;
L8c6676c8: r[0]=sx(m,0x8c6677c2u,2); /* mov.w 0x8c6677c2,r0 */
 goto L8c6676ca;
L8c6676ca: r[0]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r0 */
 goto L8c6676cc;
L8c6676cc: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c6676ce;
L8c6676ce: if(!(*sr&1u))goto L8c6676e8; /* bf 0x8c6676e8 */
 goto L8c6676d0;
L8c6676d0: r[0]=8u;return 0; /* rts */
L8c6676d4: r[0]=sx(m,0x8c6677c0u,2); /* mov.w 0x8c6677c0,r0 */
 goto L8c6676d6;
L8c6676d6: r[0]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r0 */
 goto L8c6676d8;
L8c6676d8: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c6676da;
L8c6676da: if(!(*sr&1u))goto L8c6676e8; /* bf 0x8c6676e8 */
 goto L8c6676dc;
L8c6676dc: r[0]=sx(m,0x8c6677c2u,2); /* mov.w 0x8c6677c2,r0 */
 goto L8c6676de;
L8c6676de: r[0]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r0 */
 goto L8c6676e0;
L8c6676e0: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c6676e2;
L8c6676e2: if(!(*sr&1u))goto L8c6676e8; /* bf 0x8c6676e8 */
 goto L8c6676e4;
L8c6676e4: r[0]=4u;return 0; /* rts */
L8c6676e8: r[0]=16u; /* mov #0x10,r0 */
 goto L8c6676ea;
L8c6676ea: return 0; /* rts */
}
