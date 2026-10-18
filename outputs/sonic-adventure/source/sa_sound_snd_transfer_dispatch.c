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
/* Retail SND encoding path, 8c66a348..8c66a364.
 * Selector 2 uses retail table67c3ec; selector1 expands raw bytes; otherwise BE16.
 * Native transfer/pending calls connected; query/submission callbacks supplied by caller. */
SA_STAGE_EXPORT int sa_sound_snd_transfer_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66a348;
L8c66a348: r[0]=sx(m,0x8c66a36cu,2); /* mov.w 0x8c66a36c,r0 */
 goto L8c66a34a;
L8c66a34a: r[0]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r0 */
 goto L8c66a34c;
L8c66a34c: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a34e;
L8c66a34e: if(!(*sr&1u))goto L8c66a354; /* bf 0x8c66a354 */
 goto L8c66a350;
L8c66a350: return call(ctx,0x8c66a244u,m,f,sr,r,pr); /* bra 0x8c66a244 */
 goto L8c66a354;
L8c66a354: r[0]=sx(m,0x8c66a36cu,2); /* mov.w 0x8c66a36c,r0 */
 goto L8c66a356;
L8c66a356: r[0]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r0 */
 goto L8c66a358;
L8c66a358: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66a35a;
L8c66a35a: if(!(*sr&1u))goto L8c66a360; /* bf 0x8c66a360 */
 goto L8c66a35c;
L8c66a35c: return call(ctx,0x8c66a13cu,m,f,sr,r,pr); /* bra 0x8c66a13c */
 goto L8c66a360;
L8c66a360: return call(ctx,0x8c66a01cu,m,f,sr,r,pr); /* bra 0x8c66a01c */
}
