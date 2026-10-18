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
/* Retail polynomial helper8c110238..8c110258.
 * Seven original coefficients evaluated with six fused multiply-add steps.
 * Native numerical helpers; finite PR/SZ0 and RM0/1 verification scope. */
SA_STAGE_EXPORT int sa_math_sine_polynomial(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c110238;
L8c110238: fr[6]=fr[4]; /* fmov fr4,fr6 */
 goto L8c11023a;
L8c11023a: status=sa_sh4_multiply(&fr[6],&fr[4],&f->fpscr,&fr[6]);if(status)return status; /* fmul fr4,fr6 */
 goto L8c11023c;
L8c11023c: r[5]=rd(m,0x8c110258u,4); /* mov.l 0x8c110258,r5 */
 goto L8c11023e;
L8c11023e: r[4]=6u; /* mov #0x6,r4 */
 goto L8c110240;
L8c110240: fr[5]=rd(m,r[5],4);r[5]+=4; /* fmov.s @r5+,fr5 */
 goto L8c110242;
L8c110242: fr[3]=rd(m,r[5],4);r[5]+=4; /* fmov.s @r5+,fr3 */
 goto L8c110244;
L8c110244: r[4]+=4294967295u; /* add #-0x1,r4 */
 goto L8c110246;
L8c110246: fr[0]=fr[6]; /* fmov fr6,fr0 */
 goto L8c110248;
L8c110248: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c11024a;
L8c11024a: status=sa_sh4_fmac(&fr[0],&fr[5],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr5,fr3 */
 goto L8c11024c;
L8c11024c: take=(*sr&1u);fr[5]=fr[3];if(take)goto L8c110242;goto L8c110250; /* bt/s 0x8c110242 */
 goto L8c110250;
L8c110250: fr[0]=fr[4]; /* fmov fr4,fr0 */
 goto L8c110252;
L8c110252: fr[6]=fr[5]; /* fmov fr5,fr6 */
 goto L8c110254;
L8c110254: status=sa_sh4_multiply(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;return 0; /* rts */
}
