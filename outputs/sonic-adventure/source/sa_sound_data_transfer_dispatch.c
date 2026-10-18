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
/* Retail data-chunk encoding path 8c66ab5e..8c66ab86.
 * Selector2 reconstructs stereo byte pairs(0,2)/(1,3);1 expands8-bit;0 copies16-bit.
 * Dispatcher skips unknown selectors. Native helpers connected by caller. */
SA_STAGE_EXPORT int sa_sound_data_transfer_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66ab5e;
L8c66ab5e: r[0]=sx(m,0x8c66ab8cu,2); /* mov.w 0x8c66ab8c,r0 */
 goto L8c66ab60;
L8c66ab60: r[0]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r0 */
 goto L8c66ab62;
L8c66ab62: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66ab64;
L8c66ab64: if(!(*sr&1u))goto L8c66ab6a; /* bf 0x8c66ab6a */
 goto L8c66ab66;
L8c66ab66: return call(ctx,0x8c66a9fau,m,f,sr,r,pr); /* bra 0x8c66a9fa */
 goto L8c66ab6a;
L8c66ab6a: r[0]=sx(m,0x8c66ab8cu,2); /* mov.w 0x8c66ab8c,r0 */
 goto L8c66ab6c;
L8c66ab6c: r[0]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r0 */
 goto L8c66ab6e;
L8c66ab6e: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66ab70;
L8c66ab70: if(!(*sr&1u))goto L8c66ab76; /* bf 0x8c66ab76 */
 goto L8c66ab72;
L8c66ab72: return call(ctx,0x8c66a8fau,m,f,sr,r,pr); /* bra 0x8c66a8fa */
 goto L8c66ab76;
L8c66ab76: r[0]=sx(m,0x8c66ab8cu,2); /* mov.w 0x8c66ab8c,r0 */
 goto L8c66ab78;
L8c66ab78: r[2]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r2 */
 goto L8c66ab7a;
L8c66ab7a: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c66ab7c;
L8c66ab7c: if(!(*sr&1u))goto L8c66ab82; /* bf 0x8c66ab82 */
 goto L8c66ab7e;
L8c66ab7e: return call(ctx,0x8c66a804u,m,f,sr,r,pr); /* bra 0x8c66a804 */
 goto L8c66ab82;
L8c66ab82: return 0; /* rts */
}
