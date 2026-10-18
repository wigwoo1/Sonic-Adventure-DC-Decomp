#include "fpu_cleanup_transfer.h"
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
/* Static C recovery of the USA Rev A main executable transfer busy getter. */
SA_STAGE_EXPORT int sa_engine_transfer_busy_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c1101f0;
L8c1101f0: r[2]=rd(m,0x8c110214u,4); /* mov.l 0x8c110214,r2 */
 goto L8c1101f2;
L8c1101f2: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c1101f4;
L8c1101f4: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c1101f6;
L8c1101f6: r[4]<<=2; /* shll2 r4 */
 goto L8c1101f8;
L8c1101f8: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto L8c1101fa;
L8c1101fa: r[4]<<=2; /* shll2 r4 */
 goto L8c1101fc;
L8c1101fc: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c1101fe;
L8c1101fe: r[3]=r[1]; /* mov r1,r3 */
 goto L8c110200;
L8c110200: r[4]+=r[3]; /* add r3,r4 */
 goto L8c110202;
L8c110202: r[0]=rd(m,r[4]+0x18u,4); /* mov.l @(0x18,r4),r0 */
 goto L8c110204;
L8c110204: *sr=(*sr&~1u)|!(r[0]&0x1u); /* tst #0x1,r0 */
 goto L8c110206;
L8c110206: if((*sr&1u))goto L8c11020c; /* bt 0x8c11020c */
 goto L8c110208;
L8c110208: r[1]=1u;goto L8c11020e; /* bra 0x8c11020e */
 goto L8c11020c;
L8c11020c: r[1]=0u; /* mov #0x0,r1 */
 goto L8c11020e;
L8c11020e: wr(m,r[5],r[1],4); /* mov.l r1,@r5 */
 goto L8c110210;
L8c110210: r[0]=0u;return 0; /* rts */
}
