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
/* Static C recovery of retail Tikal helper 8c09550e..8c0955c0.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_voice_reservation_release(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c09550e;
L8c09550e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c095510;
L8c095510: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c095512;
L8c095512: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c095514;
L8c095514: r[14]=52u; /* mov #0x34,r14 */
 goto L8c095516;
L8c095516: r[3]=rd(m,0x8c0955a8u,4); /* mov.l 0x8c0955a8,r3 */
 goto L8c095518;
L8c095518: *macl=r[14]*r[4]; /* mul.l r14,r4 */
 goto L8c09551a;
L8c09551a: r[5]=4294967295u; /* mov #-0x1,r5 */
 goto L8c09551c;
L8c09551c: r[1]=rd(m,0x8c0955acu,4); /* mov.l 0x8c0955ac,r1 */
 goto L8c09551e;
L8c09551e: r[6]=r[5]; /* mov r5,r6 */
 goto L8c095520;
L8c095520: r[7]=r[5]; /* mov r5,r7 */
 goto L8c095522;
L8c095522: r[13]=0u; /* mov #0x0,r13 */
 goto L8c095524;
L8c095524: r[14]=*macl; /* sts MACL,r14 */
 goto L8c095526;
L8c095526: r[4]=35u; /* mov #0x23,r4 */
 goto L8c095528;
L8c095528: r[5]=r[13]; /* mov r13,r5 */
 goto L8c09552a;
L8c09552a: r[14]+=r[3]; /* add r3,r14 */
 goto L8c09552c;
L8c09552c: r[0]=r[5]; /* mov r5,r0 */
 goto L8c09552e;
L8c09552e: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c095530;
L8c095530: r[2]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r2 */
 goto L8c095532;
L8c095532: r[3]=sx(m,r[0]+r[1],2); /* mov.w @(r0,r1),r3 */
 goto L8c095534;
L8c095534: *sr=(*sr&~1u)|(r[3]==r[2]); /* cmp/eq r3,r2 */
 goto L8c095536;
L8c095536: if(!(*sr&1u))goto L8c09553c; /* bf 0x8c09553c */
 goto L8c095538;
L8c095538: r[6]=r[5];goto L8c095542; /* bra 0x8c095542 */
 goto L8c09553c;
L8c09553c: r[5]+=1u; /* add #0x1,r5 */
 goto L8c09553e;
L8c09553e: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[4]); /* cmp/ge r4,r5 */
 goto L8c095540;
L8c095540: if(!(*sr&1u))goto L8c09552c; /* bf 0x8c09552c */
 goto L8c095542;
L8c095542: *sr=(*sr&~1u)|((int32_t)r[6]>=0); /* cmp/pz r6 */
 goto L8c095544;
L8c095544: if(!(*sr&1u))goto L8c095586; /* bf 0x8c095586 */
 goto L8c095546;
L8c095546: r[5]=r[6]; /* mov r6,r5 */
 goto L8c095548;
L8c095548: r[3]=rd(m,0x8c0955a0u,4); /* mov.l 0x8c0955a0,r3 */
 goto L8c09554a;
L8c09554a: r[5]<<=2; /* shll2 r5 */
 goto L8c09554c;
L8c09554c: r[2]=rd(m,0x8c0955a4u,4); /* mov.l 0x8c0955a4,r2 */
 goto L8c09554e;
L8c09554e: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1; /* shll r5 */
 goto L8c095550;
L8c095550: r[6]<<=2; /* shll2 r6 */
 goto L8c095552;
L8c095552: r[5]+=r[3]; /* add r3,r5 */
 goto L8c095554;
L8c095554: r[6]<<=2; /* shll2 r6 */
 goto L8c095556;
L8c095556: r[6]+=r[2]; /* add r2,r6 */
 goto L8c095558;
L8c095558: wr(m,r[15],r[6],4); /* mov.l r6,@r15 */
 goto L8c09555a;
L8c09555a: r[1]=sx(m,r[5],2); /* mov.w @r5,r1 */
 goto L8c09555c;
L8c09555c: *sr=(*sr&~1u)|((int32_t)r[1]>0); /* cmp/pl r1 */
 goto L8c09555e;
L8c09555e: take=!(*sr&1u);r[4]=r[13];if(take)goto L8c09557c;goto L8c095562; /* bf/s 0x8c09557c */
 goto L8c095562;
L8c095562: r[0]=r[4]; /* mov r4,r0 */
 goto L8c095564;
L8c095564: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c095566;
L8c095566: r[0]<<=2; /* shll2 r0 */
 goto L8c095568;
L8c095568: r[3]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r3 */
 goto L8c09556a;
L8c09556a: r[2]+=1u; /* add #0x1,r2 */
 goto L8c09556c;
L8c09556c: *sr=(*sr&~1u)|(r[2]==r[3]); /* cmp/eq r2,r3 */
 goto L8c09556e;
L8c09556e: if(!(*sr&1u))goto L8c095574; /* bf 0x8c095574 */
 goto L8c095570;
L8c095570: r[7]=r[4];goto L8c09557c; /* bra 0x8c09557c */
 goto L8c095574;
L8c095574: r[3]=sx(m,r[5],2); /* mov.w @r5,r3 */
 goto L8c095576;
L8c095576: r[4]+=1u; /* add #0x1,r4 */
 goto L8c095578;
L8c095578: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]); /* cmp/ge r3,r4 */
 goto L8c09557a;
L8c09557a: if(!(*sr&1u))goto L8c095562; /* bf 0x8c095562 */
 goto L8c09557c;
L8c09557c: *sr=(*sr&~1u)|((int32_t)r[7]>=0); /* cmp/pz r7 */
 goto L8c09557e;
L8c09557e: if(!(*sr&1u))goto L8c095586; /* bf 0x8c095586 */
 goto L8c095580;
L8c095580: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c095582;
L8c095582: r[7]<<=2; /* shll2 r7 */
 goto L8c095584;
L8c095584: wr(m,r[7]+r[0],r[13],4); /* mov.l r13,@(r0,r7) */
 goto L8c095586;
L8c095586: r[15]+=4u; /* add #0x4,r15 */
 goto L8c095588;
L8c095588: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c09558a;
L8c09558a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
