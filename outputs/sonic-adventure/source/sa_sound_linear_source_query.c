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
/* Retail linear-source reservation query 8c65e1d0..8c65e242. */
SA_STAGE_EXPORT int sa_sound_linear_source_query(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e1d0;
L8c65e1d0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65e1d2;
L8c65e1d2: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65e1d4;
L8c65e1d4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65e1d6;
L8c65e1d6: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c65e1d8;
L8c65e1d8: r[13]=r[7]; /* mov r7,r13 */
 goto L8c65e1da;
L8c65e1da: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c65e1dc;
L8c65e1dc: wr(m,r[15],r[6],4); /* mov.l r6,@r15 */
 goto L8c65e1de;
L8c65e1de: *pr=0x8c65e1e2u;r[14]=r[4];status=call(ctx,0x8c65e08cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65e08c */
 goto L8c65e1e2;
L8c65e1e2: wr(m,r[15]+0x8u,r[0],4); /* mov.l r0,@(0x8,r15) */
 goto L8c65e1e4;
L8c65e1e4: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c65e1e6;
L8c65e1e6: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e1e8;
L8c65e1e8: take=!(*sr&1u);r[4]=0u;if(take)goto L8c65e1f2;goto L8c65e1ec; /* bf/s 0x8c65e1f2 */
 goto L8c65e1ec;
L8c65e1ec: wr(m,r[13]+0x4u,r[4],4); /* mov.l r4,@(0x4,r13) */
 goto L8c65e1ee;
L8c65e1ee: wr(m,r[13],r[4],4);goto L8c65e234; /* bra 0x8c65e234 */
 goto L8c65e1f2;
L8c65e1f2: r[0]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c65e1f4;
L8c65e1f4: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c65e1f6;
L8c65e1f6: if(!(*sr&1u))goto L8c65e222; /* bf 0x8c65e222 */
 goto L8c65e1f8;
L8c65e1f8: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65e1fa;
L8c65e1fa: r[4]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r4 */
 goto L8c65e1fc;
L8c65e1fc: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]); /* cmp/ge r3,r4 */
 goto L8c65e1fe;
L8c65e1fe: if((*sr&1u))goto L8c65e204; /* bt 0x8c65e204 */
 goto L8c65e200;
L8c65e200: r[2]=r[4];goto L8c65e206; /* bra 0x8c65e206 */
 goto L8c65e204;
L8c65e204: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c65e206;
L8c65e206: wr(m,r[13]+0x4u,r[2],4); /* mov.l r2,@(0x4,r13) */
 goto L8c65e208;
L8c65e208: r[1]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r1 */
 goto L8c65e20a;
L8c65e20a: r[3]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r3 */
 goto L8c65e20c;
L8c65e20c: r[1]+=r[3]; /* add r3,r1 */
 goto L8c65e20e;
L8c65e20e: wr(m,r[13],r[1],4); /* mov.l r1,@r13 */
 goto L8c65e210;
L8c65e210: r[2]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r2 */
 goto L8c65e212;
L8c65e212: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e214;
L8c65e214: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65e216;
L8c65e216: wr(m,r[14]+0x10u,r[2],4); /* mov.l r2,@(0x10,r14) */
 goto L8c65e218;
L8c65e218: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e21a;
L8c65e21a: r[1]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r1 */
 goto L8c65e21c;
L8c65e21c: r[1]-=r[3]; /* sub r3,r1 */
 goto L8c65e21e;
L8c65e21e: wr(m,r[14]+0xcu,r[1],4);goto L8c65e234; /* bra 0x8c65e234 */
 goto L8c65e222;
L8c65e222: wr(m,r[13]+0x4u,r[4],4); /* mov.l r4,@(0x4,r13) */
 goto L8c65e224;
L8c65e224: wr(m,r[13],r[4],4); /* mov.l r4,@r13 */
 goto L8c65e226;
L8c65e226: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e228;
L8c65e228: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e22a;
L8c65e22a: if((*sr&1u))goto L8c65e234; /* bt 0x8c65e234 */
 goto L8c65e22c;
L8c65e22c: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e22e;
L8c65e22e: r[5]=4294967293u; /* mov #-0x3,r5 */
 goto L8c65e230;
L8c65e230: target=r[3];*pr=0x8c65e234u;r[4]=rd(m,r[14]+0x20u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65e234;
L8c65e234: r[5]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r5 */
 goto L8c65e236;
L8c65e236: r[4]=r[14]; /* mov r14,r4 */
 goto L8c65e238;
L8c65e238: r[15]+=12u; /* add #0xc,r15 */
 goto L8c65e23a;
L8c65e23a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e23c;
L8c65e23c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65e23e;
L8c65e23e: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c65e0a4u,m,f,sr,r,pr); /* bra 0x8c65e0a4 */
}
