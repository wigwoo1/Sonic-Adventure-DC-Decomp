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
/* Retail indexed MIDI note-on wrapper and remembered-note update. */
SA_STAGE_EXPORT int sa_sound_note_on(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c093360;
L8c093360: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c093362;
L8c093362: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c093364;
L8c093364: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c093366;
L8c093366: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c093368;
L8c093368: r[14]=r[4]; /* mov r4,r14 */
 goto L8c09336a;
L8c09336a: r[4]=15u; /* mov #0xf,r4 */
 goto L8c09336c;
L8c09336c: *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[4]); /* cmp/gt r4,r14 */
 goto L8c09336e;
L8c09336e: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c093370;
L8c093370: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c093372;
L8c093372: take=!(*sr&1u);r[13]=0u;if(take)goto L8c093378;goto L8c093376; /* bf/s 0x8c093378 */
 goto L8c093376;
L8c093376: r[13]=1u; /* mov #0x1,r13 */
 goto L8c093378;
L8c093378: r[0]=rd(m,0x8c093440u,4); /* mov.l 0x8c093440,r0 */
 goto L8c09337a;
L8c09337a: r[2]=r[14]; /* mov r14,r2 */
 goto L8c09337c;
L8c09337c: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c09337e;
L8c09337e: r[2]<<=2; /* shll2 r2 */
 goto L8c093380;
L8c093380: r[14]&=r[4]; /* and r4,r14 */
 goto L8c093382;
L8c093382: r[4]=r[13]; /* mov r13,r4 */
 goto L8c093384;
L8c093384: wr(m,r[2]+r[0],r[3],4); /* mov.l r3,@(r0,r2) */
 goto L8c093386;
L8c093386: r[4]<<=2; /* shll2 r4 */
 goto L8c093388;
L8c093388: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c09338a;
L8c09338a: r[1]=rd(m,0x8c09344cu,4); /* mov.l 0x8c09344c,r1 */
 goto L8c09338c;
L8c09338c: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c09338e;
L8c09338e: r[3]=4294967184u; /* mov #-0x70,r3 */
 goto L8c093390;
L8c093390: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c093392;
L8c093392: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c093394;
L8c093394: r[14]|=r[3]; /* or r3,r14 */
 goto L8c093396;
L8c093396: r[5]=rd(m,0x8c093444u,4); /* mov.l 0x8c093444,r5 */
 goto L8c093398;
L8c093398: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c09339a;
L8c09339a: r[2]=rd(m,0x8c093448u,4); /* mov.l 0x8c093448,r2 */
 goto L8c09339c;
L8c09339c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c09339e;
L8c09339e: r[4]+=r[2]; /* add r2,r4 */
 goto L8c0933a0;
L8c0933a0: wr(m,r[15]+0x14u,r[4],4); /* mov.l r4,@(0x14,r15) */
 goto L8c0933a2;
L8c0933a2: target=r[1];*pr=0x8c0933a6u;r[4]=rd(m,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c0933a6;
L8c0933a6: r[4]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r4 */
 goto L8c0933a8;
L8c0933a8: r[6]=0u; /* mov #0x0,r6 */
 goto L8c0933aa;
L8c0933aa: r[5]=rd(m,0x8c093444u,4); /* mov.l 0x8c093444,r5 */
 goto L8c0933ac;
L8c0933ac: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c0933ae;
L8c0933ae: r[15]+=24u; /* add #0x18,r15 */
 goto L8c0933b0;
L8c0933b0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c0933b2;
L8c0933b2: r[3]=rd(m,0x8c093450u,4); /* mov.l 0x8c093450,r3 */
 goto L8c0933b4;
L8c0933b4: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c0933b6;
L8c0933b6: target=r[3];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
}
