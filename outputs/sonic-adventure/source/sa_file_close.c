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
/* Static C recovery of retail File handle metadata helper 8c600df4..8c600e2c.
 * Retail handle metadata routine. Close dispatches pending-operation service
 * 8c6013d2 when handle state is2/3/4; size getter has no subordinate calls.
 * Integer instructions do not depend on FPU mode bits. */
SA_STAGE_EXPORT int sa_file_close(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c600df4;
L8c600df4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c600df6;
L8c600df6: r[14]=r[4]; /* mov r4,r14 */
 goto L8c600df8;
L8c600df8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c600dfa;
L8c600dfa: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c600dfc;
L8c600dfc: if((*sr&1u))goto L8c600e26; /* bt 0x8c600e26 */
 goto L8c600dfe;
L8c600dfe: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c600e00;
L8c600e00: r[2]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r2 */
 goto L8c600e02;
L8c600e02: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c600e04;
L8c600e04: if((*sr&1u))goto L8c600e26; /* bt 0x8c600e26 */
 goto L8c600e06;
L8c600e06: r[0]=80u; /* mov #0x50,r0 */
 goto L8c600e08;
L8c600e08: r[0]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r0 */
 goto L8c600e0a;
L8c600e0a: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c600e0c;
L8c600e0c: take=(*sr&1u);r[4]=r[0];if(take)goto L8c600e1c;goto L8c600e10; /* bt/s 0x8c600e1c */
 goto L8c600e10;
L8c600e10: r[0]=r[4]; /* mov r4,r0 */
 goto L8c600e12;
L8c600e12: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c600e14;
L8c600e14: if((*sr&1u))goto L8c600e1c; /* bt 0x8c600e1c */
 goto L8c600e16;
L8c600e16: r[0]=r[4]; /* mov r4,r0 */
 goto L8c600e18;
L8c600e18: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c600e1a;
L8c600e1a: if(!(*sr&1u))goto L8c600e20; /* bf 0x8c600e20 */
 goto L8c600e1c;
L8c600e1c: *pr=0x8c600e20u;r[4]=r[14];status=call(ctx,0x8c6013d2u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6013d2 */
 goto L8c600e20;
L8c600e20: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c600e22;
L8c600e22: r[3]=0u; /* mov #0x0,r3 */
 goto L8c600e24;
L8c600e24: wr(m,r[14]+r[0],r[3],2); /* mov.w r3,@(r0,r14) */
 goto L8c600e26;
L8c600e26: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c600e28;
L8c600e28: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
