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
/* Static C recovery of retail Tikal helper 8c647f10..8c647f4c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_system_type_set(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c647f10;
L8c647f10: r[0]=r[4]; /* mov r4,r0 */
 goto L8c647f12;
L8c647f12: r[3]=rd(m,0x8c647fc8u,4); /* mov.l 0x8c647fc8,r3 */
 goto L8c647f14;
L8c647f14: *sr=(*sr&~1u)|(r[0]==0xfu); /* cmp/eq #0xf,r0 */
 goto L8c647f16;
L8c647f16: take=(*sr&1u);r[5]=rd(m,r[3],4);if(take)goto L8c647f2a;goto L8c647f1a; /* bt/s 0x8c647f2a */
 goto L8c647f1a;
L8c647f1a: *sr=(*sr&~1u)|(r[0]==0x21u); /* cmp/eq #0x21,r0 */
 goto L8c647f1c;
L8c647f1c: if((*sr&1u))goto L8c647f30; /* bt 0x8c647f30 */
 goto L8c647f1e;
L8c647f1e: *sr=(*sr&~1u)|(r[0]==0x41u); /* cmp/eq #0x41,r0 */
 goto L8c647f20;
L8c647f20: if((*sr&1u))goto L8c647f34; /* bt 0x8c647f34 */
 goto L8c647f22;
L8c647f22: *sr=(*sr&~1u)|(r[0]==0x7au); /* cmp/eq #0x7a,r0 */
 goto L8c647f24;
L8c647f24: if((*sr&1u))goto L8c647f3a; /* bt 0x8c647f3a */
 goto L8c647f26;
L8c647f26: goto L8c647f40; /* bra 0x8c647f40 */
 goto L8c647f2a;
L8c647f2a: r[2]=0u; /* mov #0x0,r2 */
 goto L8c647f2c;
L8c647f2c: wr(m,r[5]+0x4u,r[2],4);goto L8c647f46; /* bra 0x8c647f46 */
 goto L8c647f30;
L8c647f30: r[0]=1u;goto L8c647f3c; /* bra 0x8c647f3c */
 goto L8c647f34;
L8c647f34: r[1]=2u; /* mov #0x2,r1 */
 goto L8c647f36;
L8c647f36: wr(m,r[5]+0x4u,r[1],4);goto L8c647f46; /* bra 0x8c647f46 */
 goto L8c647f3a;
L8c647f3a: r[0]=3u; /* mov #0x3,r0 */
 goto L8c647f3c;
L8c647f3c: wr(m,r[5]+0x4u,r[0],4);goto L8c647f46; /* bra 0x8c647f46 */
 goto L8c647f40;
L8c647f40: r[0]=sx(m,0x8c647fb2u,2); /* mov.w 0x8c647fb2,r0 */
 goto L8c647f42;
L8c647f42: return 0; /* rts */
L8c647f46: r[0]=0u; /* mov #0x0,r0 */
 goto L8c647f48;
L8c647f48: return 0; /* rts */
}
