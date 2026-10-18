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
/* Retail sound low-index/MIDI path. Packer varargs follow the recovered SH-4 ABI. */
SA_STAGE_EXPORT int sa_sound_low_index(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c0933ba;
L8c0933ba: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c0933bc;
L8c0933bc: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c0933be;
L8c0933be: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c0933c0;
L8c0933c0: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c0933c2;
L8c0933c2: r[14]=r[4]; /* mov r4,r14 */
 goto L8c0933c4;
L8c0933c4: r[4]=15u; /* mov #0xf,r4 */
 goto L8c0933c6;
L8c0933c6: *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[4]); /* cmp/gt r4,r14 */
 goto L8c0933c8;
L8c0933c8: take=!(*sr&1u);r[13]=0u;if(take)goto L8c0933ce;goto L8c0933cc; /* bf/s 0x8c0933ce */
 goto L8c0933cc;
L8c0933cc: r[13]=1u; /* mov #0x1,r13 */
 goto L8c0933ce;
L8c0933ce: r[0]=rd(m,0x8c093440u,4); /* mov.l 0x8c093440,r0 */
 goto L8c0933d0;
L8c0933d0: r[7]=r[14]; /* mov r14,r7 */
 goto L8c0933d2;
L8c0933d2: r[7]<<=2; /* shll2 r7 */
 goto L8c0933d4;
L8c0933d4: r[1]=rd(m,0x8c093448u,4); /* mov.l 0x8c093448,r1 */
 goto L8c0933d6;
L8c0933d6: r[3]=0u; /* mov #0x0,r3 */
 goto L8c0933d8;
L8c0933d8: r[7]=rd(m,r[7]+r[0],4); /* mov.l @(r0,r7),r7 */
 goto L8c0933da;
L8c0933da: r[2]=4294967168u; /* mov #-0x80,r2 */
 goto L8c0933dc;
L8c0933dc: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c0933de;
L8c0933de: r[14]&=r[4]; /* and r4,r14 */
 goto L8c0933e0;
L8c0933e0: r[4]=r[13]; /* mov r13,r4 */
 goto L8c0933e2;
L8c0933e2: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c0933e4;
L8c0933e4: r[15]-=4;wr(m,r[15],r[7],4); /* mov.l r7,@-r15 */
 goto L8c0933e6;
L8c0933e6: r[4]<<=2; /* shll2 r4 */
 goto L8c0933e8;
L8c0933e8: r[3]=rd(m,0x8c09344cu,4); /* mov.l 0x8c09344c,r3 */
 goto L8c0933ea;
L8c0933ea: r[14]|=r[2]; /* or r2,r14 */
 goto L8c0933ec;
L8c0933ec: r[5]=rd(m,0x8c093444u,4); /* mov.l 0x8c093444,r5 */
 goto L8c0933ee;
L8c0933ee: r[4]+=r[1]; /* add r1,r4 */
 goto L8c0933f0;
L8c0933f0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c0933f2;
L8c0933f2: wr(m,r[15]+0xcu,r[4],4); /* mov.l r4,@(0xc,r15) */
 goto L8c0933f4;
L8c0933f4: target=r[3];*pr=0x8c0933f8u;r[4]=rd(m,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c0933f8;
L8c0933f8: r[4]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r4 */
 goto L8c0933fa;
L8c0933fa: r[6]=0u; /* mov #0x0,r6 */
 goto L8c0933fc;
L8c0933fc: r[5]=rd(m,0x8c093444u,4); /* mov.l 0x8c093444,r5 */
 goto L8c0933fe;
L8c0933fe: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c093400;
L8c093400: r[15]+=16u; /* add #0x10,r15 */
 goto L8c093402;
L8c093402: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c093404;
L8c093404: r[3]=rd(m,0x8c093450u,4); /* mov.l 0x8c093450,r3 */
 goto L8c093406;
L8c093406: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c093408;
L8c093408: target=r[3];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
}
