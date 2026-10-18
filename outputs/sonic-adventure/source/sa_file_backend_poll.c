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
/* Static C recovery of retail filesystem backend poll 8c6005c2..8c6005e6.
 * Device pumping and completion processing are explicit service boundaries.
 * Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_backend_poll(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6005c2;
L8c6005c2: r[2]=rd(m,0x8c6006b4u,4); /* mov.l 0x8c6006b4,r2 */
 goto L8c6005c4;
L8c6005c4: r[0]=64u; /* mov #0x40,r0 */
 goto L8c6005c6;
L8c6005c6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6005c8;
L8c6005c8: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c6005ca;
L8c6005ca: r[1]=rd(m,r[3]+0xcu,4); /* mov.l @(0xc,r3),r1 */
 goto L8c6005cc;
L8c6005cc: r[0]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c6005ce;
L8c6005ce: target=r[0];*pr=0x8c6005d2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r0 */
 goto L8c6005d2;
L8c6005d2: r[2]=rd(m,0x8c6006b4u,4); /* mov.l 0x8c6006b4,r2 */
 goto L8c6005d4;
L8c6005d4: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c6005d6;
L8c6005d6: r[1]=rd(m,r[3]+0x1cu,4); /* mov.l @(0x1c,r3),r1 */
 goto L8c6005d8;
L8c6005d8: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c6005da;
L8c6005da: if((*sr&1u))goto L8c6005e0; /* bt 0x8c6005e0 */
 goto L8c6005dc;
L8c6005dc: *pr=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c5ffa12u,m,f,sr,r,pr); /* bra 0x8c5ffa12 */
 goto L8c6005e0;
L8c6005e0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6005e2;
L8c6005e2: return 0; /* rts */
}
