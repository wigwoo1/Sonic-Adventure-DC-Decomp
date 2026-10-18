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
/* Static C recovery of retail Tikal helper 8c646bbc..8c646c20.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_metadata(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c646bbc;
L8c646bbc: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c646bbe;
L8c646bbe: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c646bc0;
L8c646bc0: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c646bc2;
L8c646bc2: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c646bc4;
L8c646bc4: r[5]=2u; /* mov #0x2,r5 */
 goto L8c646bc6;
L8c646bc6: r[14]=r[4]; /* mov r4,r14 */
 goto L8c646bc8;
L8c646bc8: *pr=0x8c646bccu;r[4]=rd(m,r[15],4);status=call(ctx,0x8c646b42u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c646b42 */
 goto L8c646bcc;
L8c646bcc: r[4]=r[0]; /* mov r0,r4 */
 goto L8c646bce;
L8c646bce: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c646bd0;
L8c646bd0: if(!(*sr&1u))goto L8c646bdc; /* bf 0x8c646bdc */
 goto L8c646bd2;
L8c646bd2: r[0]=4294967295u; /* mov #-0x1,r0 */
 goto L8c646bd4;
L8c646bd4: r[15]+=4u; /* add #0x4,r15 */
 goto L8c646bd6;
L8c646bd6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c646bd8;
L8c646bd8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c646bdc: r[5]=r[4]; /* mov r4,r5 */
 goto L8c646bde;
L8c646bde: r[5]+=2u; /* add #0x2,r5 */
 goto L8c646be0;
L8c646be0: r[3]=sx(m,r[4],1); /* mov.b @r4,r3 */
 goto L8c646be2;
L8c646be2: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c646be4;
L8c646be4: r[3]+=1u; /* add #0x1,r3 */
 goto L8c646be6;
L8c646be6: wr(m,r[14],r[3],2); /* mov.w r3,@r14 */
 goto L8c646be8;
L8c646be8: r[3]=4294967289u; /* mov #-0x7,r3 */
 goto L8c646bea;
L8c646bea: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c646bec;
L8c646bec: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c646bee;
L8c646bee: r[0]+=1u; /* add #0x1,r0 */
 goto L8c646bf0;
L8c646bf0: r[0]<<=2; /* shll2 r0 */
 goto L8c646bf2;
L8c646bf2: r[0]<<=2; /* shll2 r0 */
 goto L8c646bf4;
L8c646bf4: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c646bf6;
L8c646bf6: wr(m,r[14]+0x2u,r[0],2); /* mov.w r0,@(0x2,r14) */
 goto L8c646bf8;
L8c646bf8: r[0]=sx(m,r[5],1); /* mov.b @r5,r0 */
 goto L8c646bfa;
L8c646bfa: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c646bfc;
L8c646bfc: r[0]>>=2; /* shlr2 r0 */
 goto L8c646bfe;
L8c646bfe: r[0]>>=2; /* shlr2 r0 */
 goto L8c646c00;
L8c646c00: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c646c02;
L8c646c02: wr(m,r[14]+0x4u,r[0],2); /* mov.w r0,@(0x4,r14) */
 goto L8c646c04;
L8c646c04: r[0]=sx(m,r[5],1); /* mov.b @r5,r0 */
 goto L8c646c06;
L8c646c06: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c646c08;
L8c646c08: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c646c0a;
L8c646c0a: wr(m,r[14]+0x6u,r[0],2); /* mov.w r0,@(0x6,r14) */
 goto L8c646c0c;
L8c646c0c: r[0]=sx(m,r[4]+0x3u,1); /* mov.b @(0x3,r4),r0 */
 goto L8c646c0e;
L8c646c0e: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c646c10;
L8c646c10: r[0]=(r[3]&0x80000000u)?((r[3]&31u)?r[0]>>((0u-r[3])&31u):0u):(r[0]<<(r[3]&31u)); /* shld r3,r0 */
 goto L8c646c12;
L8c646c12: r[0]&=0x1u; /* and #0x1,r0 */
 goto L8c646c14;
L8c646c14: wr(m,r[14]+0x8u,r[0],2); /* mov.w r0,@(0x8,r14) */
 goto L8c646c16;
L8c646c16: r[0]=0u; /* mov #0x0,r0 */
 goto L8c646c18;
L8c646c18: r[15]+=4u; /* add #0x4,r15 */
 goto L8c646c1a;
L8c646c1a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c646c1c;
L8c646c1c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
