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
/* Retail linear-source descriptor operation 8c65e282..8c65e302. */
SA_STAGE_EXPORT int sa_sound_linear_source_return(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e282;
L8c65e282: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65e284;
L8c65e284: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65e286;
L8c65e286: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65e288;
L8c65e288: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c65e28a;
L8c65e28a: r[13]=r[6]; /* mov r6,r13 */
 goto L8c65e28c;
L8c65e28c: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c65e28e;
L8c65e28e: *pr=0x8c65e292u;r[14]=r[4];status=call(ctx,0x8c65e08cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65e08c */
 goto L8c65e292;
L8c65e292: wr(m,r[15]+0x4u,r[0],4); /* mov.l r0,@(0x4,r15) */
 goto L8c65e294;
L8c65e294: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65e296;
L8c65e296: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e298;
L8c65e298: if(!(*sr&1u))goto L8c65e2a4; /* bf 0x8c65e2a4 */
 goto L8c65e29a;
L8c65e29a: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e29c;
L8c65e29c: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e29e;
L8c65e29e: if(!(*sr&1u))goto L8c65e2ec; /* bf 0x8c65e2ec */
 goto L8c65e2a0;
L8c65e2a0: goto L8c65e2f4; /* bra 0x8c65e2f4 */
 goto L8c65e2a4;
L8c65e2a4: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c65e2a6;
L8c65e2a6: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c65e2a8;
L8c65e2a8: if(!(*sr&1u))goto L8c65e2e0; /* bf 0x8c65e2e0 */
 goto L8c65e2aa;
L8c65e2aa: r[4]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r4 */
 goto L8c65e2ac;
L8c65e2ac: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e2ae;
L8c65e2ae: r[4]-=r[3]; /* sub r3,r4 */
 goto L8c65e2b0;
L8c65e2b0: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c65e2b2;
L8c65e2b2: if((*sr&1u))goto L8c65e2b6; /* bt 0x8c65e2b6 */
 goto L8c65e2b4;
L8c65e2b4: r[4]=0u; /* mov #0x0,r4 */
 goto L8c65e2b6;
L8c65e2b6: wr(m,r[14]+0x10u,r[4],4); /* mov.l r4,@(0x10,r14) */
 goto L8c65e2b8;
L8c65e2b8: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e2ba;
L8c65e2ba: r[6]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r6 */
 goto L8c65e2bc;
L8c65e2bc: r[5]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r5 */
 goto L8c65e2be;
L8c65e2be: r[6]+=r[3]; /* add r3,r6 */
 goto L8c65e2c0;
L8c65e2c0: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]); /* cmp/ge r6,r5 */
 goto L8c65e2c2;
L8c65e2c2: if((*sr&1u))goto L8c65e2c8; /* bt 0x8c65e2c8 */
 goto L8c65e2c4;
L8c65e2c4: r[2]=r[5];goto L8c65e2ca; /* bra 0x8c65e2ca */
 goto L8c65e2c8;
L8c65e2c8: r[2]=r[6]; /* mov r6,r2 */
 goto L8c65e2ca;
L8c65e2ca: wr(m,r[14]+0xcu,r[2],4); /* mov.l r2,@(0xc,r14) */
 goto L8c65e2cc;
L8c65e2cc: r[5]=rd(m,r[13],4); /* mov.l @r13,r5 */
 goto L8c65e2ce;
L8c65e2ce: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c65e2d0;
L8c65e2d0: r[5]-=r[3]; /* sub r3,r5 */
 goto L8c65e2d2;
L8c65e2d2: *sr=(*sr&~1u)|(r[5]==r[4]); /* cmp/eq r5,r4 */
 goto L8c65e2d4;
L8c65e2d4: if((*sr&1u))goto L8c65e2f4; /* bt 0x8c65e2f4 */
 goto L8c65e2d6;
L8c65e2d6: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e2d8;
L8c65e2d8: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e2da;
L8c65e2da: if((*sr&1u))goto L8c65e2f4; /* bt 0x8c65e2f4 */
 goto L8c65e2dc;
L8c65e2dc: goto L8c65e2ec; /* bra 0x8c65e2ec */
 goto L8c65e2e0;
L8c65e2e0: r[4]=0u; /* mov #0x0,r4 */
 goto L8c65e2e2;
L8c65e2e2: wr(m,r[13]+0x4u,r[4],4); /* mov.l r4,@(0x4,r13) */
 goto L8c65e2e4;
L8c65e2e4: wr(m,r[13],r[4],4); /* mov.l r4,@r13 */
 goto L8c65e2e6;
L8c65e2e6: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e2e8;
L8c65e2e8: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e2ea;
L8c65e2ea: if((*sr&1u))goto L8c65e2f4; /* bt 0x8c65e2f4 */
 goto L8c65e2ec;
L8c65e2ec: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e2ee;
L8c65e2ee: r[5]=4294967293u; /* mov #-0x3,r5 */
 goto L8c65e2f0;
L8c65e2f0: target=r[3];*pr=0x8c65e2f4u;r[4]=rd(m,r[14]+0x20u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65e2f4;
L8c65e2f4: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c65e2f6;
L8c65e2f6: r[4]=r[14]; /* mov r14,r4 */
 goto L8c65e2f8;
L8c65e2f8: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65e2fa;
L8c65e2fa: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e2fc;
L8c65e2fc: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65e2fe;
L8c65e2fe: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c65e0a4u,m,f,sr,r,pr); /* bra 0x8c65e0a4 */
}
