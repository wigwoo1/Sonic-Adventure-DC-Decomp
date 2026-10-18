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
/* Retail sound bank load orchestration. File services and download remain external; behavioral verification pending. */
SA_STAGE_EXPORT int sa_sound_bank_load(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c0939b0;
L8c0939b0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c0939b2;
L8c0939b2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c0939b4;
L8c0939b4: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c0939b6;
L8c0939b6: r[3]=rd(m,0x8c093a28u,4); /* mov.l 0x8c093a28,r3 */
 goto L8c0939b8;
L8c0939b8: target=r[3];*pr=0x8c0939bcu;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c0939bc;
L8c0939bc: r[2]=rd(m,0x8c093a2cu,4); /* mov.l 0x8c093a2c,r2 */
 goto L8c0939be;
L8c0939be: r[5]=r[15]; /* mov r15,r5 */
 goto L8c0939c0;
L8c0939c0: r[14]=r[0]; /* mov r0,r14 */
 goto L8c0939c2;
L8c0939c2: target=r[2];*pr=0x8c0939c6u;r[4]=r[0];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c0939c6;
L8c0939c6: r[3]=8u; /* mov #0x8,r3 */
 goto L8c0939c8;
L8c0939c8: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c0939ca;
L8c0939ca: r[3]<<=8; /* shll8 r3 */
 goto L8c0939cc;
L8c0939cc: r[6]=rd(m,0x8c093a30u,4); /* mov.l 0x8c093a30,r6 */
 goto L8c0939ce;
L8c0939ce: r[3]+=4294967295u; /* add #-0x1,r3 */
 goto L8c0939d0;
L8c0939d0: r[1]=rd(m,0x8c093a34u,4); /* mov.l 0x8c093a34,r1 */
 goto L8c0939d2;
L8c0939d2: r[5]+=r[3]; /* add r3,r5 */
 goto L8c0939d4;
L8c0939d4: r[2]=4294967285u; /* mov #-0xb,r2 */
 goto L8c0939d6;
L8c0939d6: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[5]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[5]=n?(uint32_t)((int32_t)r[5]>>n):((r[5]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r5 */
 goto L8c0939d8;
L8c0939d8: target=r[1];*pr=0x8c0939dcu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c0939dc;
L8c0939dc: r[3]=rd(m,0x8c093a38u,4); /* mov.l 0x8c093a38,r3 */
 goto L8c0939de;
L8c0939de: target=r[3];*pr=0x8c0939e2u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c0939e2;
L8c0939e2: r[4]=rd(m,0x8c093a30u,4); /* mov.l 0x8c093a30,r4 */
 goto L8c0939e4;
L8c0939e4: *pr=0x8c0939e8u;status=call(ctx,0x8c093ce0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093ce0 */
 goto L8c0939e8;
L8c0939e8: r[0]=0u; /* mov #0x0,r0 */
 goto L8c0939ea;
L8c0939ea: r[15]+=4u; /* add #0x4,r15 */
 goto L8c0939ec;
L8c0939ec: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c0939ee;
L8c0939ee: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
