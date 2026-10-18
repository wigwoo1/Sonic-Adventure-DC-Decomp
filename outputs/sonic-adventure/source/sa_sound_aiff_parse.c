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
/* Static C recovery of retail Tikal helper 8c669528..8c669808.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_aiff_parse(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c669528;
L8c669528: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66952a;
L8c66952a: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66952c;
L8c66952c: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66952e;
L8c66952e: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c669530;
L8c669530: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c669532;
L8c669532: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c669534;
L8c669534: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c669536;
L8c669536: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c669538;
L8c669538: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66953a;
L8c66953a: r[9]=0u; /* mov #0x0,r9 */
 goto L8c66953c;
L8c66953c: r[2]=sx(m,r[4],1); /* mov.b @r4,r2 */
 goto L8c66953e;
L8c66953e: r[12]=r[9]; /* mov r9,r12 */
 goto L8c669540;
L8c669540: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669542;
L8c669542: r[10]=rd(m,r[15]+0x28u,4); /* mov.l @(0x28,r15),r10 */
 goto L8c669544;
L8c669544: r[0]<<=8; /* shll8 r0 */
 goto L8c669546;
L8c669546: r[13]=rd(m,0x8c669688u,4); /* mov.l 0x8c669688,r13 */
 goto L8c669548;
L8c669548: r[3]=r[0]; /* mov r0,r3 */
 goto L8c66954a;
L8c66954a: r[0]=sx(m,r[4]+0x2u,1); /* mov.b @(0x2,r4),r0 */
 goto L8c66954c;
L8c66954c: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c66954e;
L8c66954e: r[14]=rd(m,0x8c66968cu,4); /* mov.l 0x8c66968c,r14 */
 goto L8c669550;
L8c669550: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669552;
L8c669552: r[0]<<=16; /* shll16 r0 */
 goto L8c669554;
L8c669554: r[3]|=r[0]; /* or r0,r3 */
 goto L8c669556;
L8c669556: r[0]=sx(m,r[4]+0x3u,1); /* mov.b @(0x3,r4),r0 */
 goto L8c669558;
L8c669558: r[4]+=4u; /* add #0x4,r4 */
 goto L8c66955a;
L8c66955a: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66955c;
L8c66955c: r[0]<<=16; /* shll16 r0 */
 goto L8c66955e;
L8c66955e: r[0]<<=8; /* shll8 r0 */
 goto L8c669560;
L8c669560: r[3]|=r[0]; /* or r0,r3 */
 goto L8c669562;
L8c669562: r[3]|=r[2]; /* or r2,r3 */
 goto L8c669564;
L8c669564: wr(m,r[15]+0x8u,r[3],4); /* mov.l r3,@(0x8,r15) */
 goto L8c669566;
L8c669566: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c669568;
L8c669568: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66956a;
L8c66956a: r[0]<<=8; /* shll8 r0 */
 goto L8c66956c;
L8c66956c: r[11]=r[0]; /* mov r0,r11 */
 goto L8c66956e;
L8c66956e: r[0]=sx(m,r[4]+0x2u,1); /* mov.b @(0x2,r4),r0 */
 goto L8c669570;
L8c669570: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669572;
L8c669572: r[0]<<=16; /* shll16 r0 */
 goto L8c669574;
L8c669574: r[11]|=r[0]; /* or r0,r11 */
 goto L8c669576;
L8c669576: r[0]=sx(m,r[4]+0x3u,1); /* mov.b @(0x3,r4),r0 */
 goto L8c669578;
L8c669578: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66957a;
L8c66957a: r[2]=sx(m,r[4],1); /* mov.b @r4,r2 */
 goto L8c66957c;
L8c66957c: r[0]<<=16; /* shll16 r0 */
 goto L8c66957e;
L8c66957e: r[0]<<=8; /* shll8 r0 */
 goto L8c669580;
L8c669580: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c669582;
L8c669582: r[11]|=r[0]; /* or r0,r11 */
 goto L8c669584;
L8c669584: r[11]|=r[2]; /* or r2,r11 */
 goto L8c669586;
L8c669586: r[1]=r[11]; /* mov r11,r1 */
 goto L8c669588;
L8c669588: r[1]>>=16; /* shlr16 r1 */
 goto L8c66958a;
L8c66958a: r[0]=r[11]; /* mov r11,r0 */
 goto L8c66958c;
L8c66958c: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c66958e;
L8c66958e: r[1]>>=8; /* shlr8 r1 */
 goto L8c669590;
L8c669590: r[1]=(uint32_t)(int32_t)(int8_t)r[1]; /* exts.b r1,r1 */
 goto L8c669592;
L8c669592: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r0 */
 goto L8c669594;
L8c669594: r[2]=r[11]; /* mov r11,r2 */
 goto L8c669596;
L8c669596: r[0]&=r[14]; /* and r14,r0 */
 goto L8c669598;
L8c669598: r[1]=r[1]&255u; /* extu.b r1,r1 */
 goto L8c66959a;
L8c66959a: r[1]|=r[0]; /* or r0,r1 */
 goto L8c66959c;
L8c66959c: r[0]=r[11]; /* mov r11,r0 */
 goto L8c66959e;
L8c66959e: r[0]<<=8; /* shll8 r0 */
 goto L8c6695a0;
L8c6695a0: r[0]&=r[13]; /* and r13,r0 */
 goto L8c6695a2;
L8c6695a2: r[4]+=4u; /* add #0x4,r4 */
 goto L8c6695a4;
L8c6695a4: r[1]|=r[0]; /* or r0,r1 */
 goto L8c6695a6;
L8c6695a6: r[2]<<=16; /* shll16 r2 */
 goto L8c6695a8;
L8c6695a8: r[2]<<=8; /* shll8 r2 */
 goto L8c6695aa;
L8c6695aa: r[1]|=r[2]; /* or r2,r1 */
 goto L8c6695ac;
L8c6695ac: wr(m,r[15]+0x4u,r[1],4); /* mov.l r1,@(0x4,r15) */
 goto L8c6695ae;
L8c6695ae: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c6695b0;
L8c6695b0: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6695b2;
L8c6695b2: r[0]<<=8; /* shll8 r0 */
 goto L8c6695b4;
L8c6695b4: r[2]=r[0]; /* mov r0,r2 */
 goto L8c6695b6;
L8c6695b6: r[0]=sx(m,r[4]+0x2u,1); /* mov.b @(0x2,r4),r0 */
 goto L8c6695b8;
L8c6695b8: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6695ba;
L8c6695ba: r[3]=rd(m,0x8c669690u,4); /* mov.l 0x8c669690,r3 */
 goto L8c6695bc;
L8c6695bc: r[0]<<=16; /* shll16 r0 */
 goto L8c6695be;
L8c6695be: r[2]|=r[0]; /* or r0,r2 */
 goto L8c6695c0;
L8c6695c0: r[0]=sx(m,r[4]+0x3u,1); /* mov.b @(0x3,r4),r0 */
 goto L8c6695c2;
L8c6695c2: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6695c4;
L8c6695c4: r[0]<<=16; /* shll16 r0 */
 goto L8c6695c6;
L8c6695c6: r[0]<<=8; /* shll8 r0 */
 goto L8c6695c8;
L8c6695c8: r[2]|=r[0]; /* or r0,r2 */
 goto L8c6695ca;
L8c6695ca: r[0]=sx(m,r[4],1); /* mov.b @r4,r0 */
 goto L8c6695cc;
L8c6695cc: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6695ce;
L8c6695ce: r[2]|=r[0]; /* or r0,r2 */
 goto L8c6695d0;
L8c6695d0: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c6695d2;
L8c6695d2: r[0]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r0 */
 goto L8c6695d4;
L8c6695d4: *sr=(*sr&~1u)|(r[3]==r[0]); /* cmp/eq r3,r0 */
 goto L8c6695d6;
L8c6695d6: take=!(*sr&1u);r[4]+=4u;if(take)goto L8c669684;goto L8c6695da; /* bf/s 0x8c669684 */
 goto L8c6695da;
L8c6695da: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c6695dc;
L8c6695dc: r[3]=rd(m,0x8c669694u,4); /* mov.l 0x8c669694,r3 */
 goto L8c6695de;
L8c6695de: *sr=(*sr&~1u)|(r[3]==r[2]); /* cmp/eq r3,r2 */
 goto L8c6695e0;
L8c6695e0: if(!(*sr&1u))goto L8c669684; /* bf 0x8c669684 */
 goto L8c6695e2;
L8c6695e2: r[8]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r8 */
 goto L8c6695e4;
L8c6695e4: r[8]+=r[4]; /* add r4,r8 */
 goto L8c6695e6;
L8c6695e6: r[8]+=4294967292u; /* add #-0x4,r8 */
 goto L8c6695e8;
L8c6695e8: *sr=(*sr&~1u)|(r[4]>=r[8]); /* cmp/hs r8,r4 */
 goto L8c6695ea;
L8c6695ea: if(!(*sr&1u))goto L8c6695f0; /* bf 0x8c6695f0 */
 goto L8c6695ec;
L8c6695ec: goto L8c6697f4; /* bra 0x8c6697f4 */
 goto L8c6695f0;
L8c6695f0: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c6695f2;
L8c6695f2: r[2]=sx(m,r[4],1); /* mov.b @r4,r2 */
 goto L8c6695f4;
L8c6695f4: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6695f6;
L8c6695f6: r[0]<<=8; /* shll8 r0 */
 goto L8c6695f8;
L8c6695f8: r[3]=r[0]; /* mov r0,r3 */
 goto L8c6695fa;
L8c6695fa: r[0]=sx(m,r[4]+0x2u,1); /* mov.b @(0x2,r4),r0 */
 goto L8c6695fc;
L8c6695fc: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c6695fe;
L8c6695fe: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669600;
L8c669600: r[0]<<=16; /* shll16 r0 */
 goto L8c669602;
L8c669602: r[3]|=r[0]; /* or r0,r3 */
 goto L8c669604;
L8c669604: r[0]=sx(m,r[4]+0x3u,1); /* mov.b @(0x3,r4),r0 */
 goto L8c669606;
L8c669606: r[4]+=4u; /* add #0x4,r4 */
 goto L8c669608;
L8c669608: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66960a;
L8c66960a: r[0]<<=16; /* shll16 r0 */
 goto L8c66960c;
L8c66960c: r[0]<<=8; /* shll8 r0 */
 goto L8c66960e;
L8c66960e: r[3]|=r[0]; /* or r0,r3 */
 goto L8c669610;
L8c669610: r[3]|=r[2]; /* or r2,r3 */
 goto L8c669612;
L8c669612: wr(m,r[15]+0x4u,r[3],4); /* mov.l r3,@(0x4,r15) */
 goto L8c669614;
L8c669614: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c669616;
L8c669616: r[2]=sx(m,r[4],1); /* mov.b @r4,r2 */
 goto L8c669618;
L8c669618: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66961a;
L8c66961a: r[0]<<=8; /* shll8 r0 */
 goto L8c66961c;
L8c66961c: r[11]=r[0]; /* mov r0,r11 */
 goto L8c66961e;
L8c66961e: r[0]=sx(m,r[4]+0x2u,1); /* mov.b @(0x2,r4),r0 */
 goto L8c669620;
L8c669620: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669622;
L8c669622: r[0]<<=16; /* shll16 r0 */
 goto L8c669624;
L8c669624: r[11]|=r[0]; /* or r0,r11 */
 goto L8c669626;
L8c669626: r[0]=sx(m,r[4]+0x3u,1); /* mov.b @(0x3,r4),r0 */
 goto L8c669628;
L8c669628: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66962a;
L8c66962a: r[0]<<=16; /* shll16 r0 */
 goto L8c66962c;
L8c66962c: r[0]<<=8; /* shll8 r0 */
 goto L8c66962e;
L8c66962e: r[11]|=r[0]; /* or r0,r11 */
 goto L8c669630;
L8c669630: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c669632;
L8c669632: r[11]|=r[2]; /* or r2,r11 */
 goto L8c669634;
L8c669634: r[1]=r[11]; /* mov r11,r1 */
 goto L8c669636;
L8c669636: r[1]>>=16; /* shlr16 r1 */
 goto L8c669638;
L8c669638: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c66963a;
L8c66963a: r[0]=r[11]; /* mov r11,r0 */
 goto L8c66963c;
L8c66963c: r[1]>>=8; /* shlr8 r1 */
 goto L8c66963e;
L8c66963e: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r0 */
 goto L8c669640;
L8c669640: r[2]=r[11]; /* mov r11,r2 */
 goto L8c669642;
L8c669642: r[1]=(uint32_t)(int32_t)(int8_t)r[1]; /* exts.b r1,r1 */
 goto L8c669644;
L8c669644: r[0]&=r[14]; /* and r14,r0 */
 goto L8c669646;
L8c669646: r[1]=r[1]&255u; /* extu.b r1,r1 */
 goto L8c669648;
L8c669648: r[1]|=r[0]; /* or r0,r1 */
 goto L8c66964a;
L8c66964a: r[0]=r[11]; /* mov r11,r0 */
 goto L8c66964c;
L8c66964c: r[0]<<=8; /* shll8 r0 */
 goto L8c66964e;
L8c66964e: r[2]<<=16; /* shll16 r2 */
 goto L8c669650;
L8c669650: r[0]&=r[13]; /* and r13,r0 */
 goto L8c669652;
L8c669652: r[2]<<=8; /* shll8 r2 */
 goto L8c669654;
L8c669654: r[1]|=r[0]; /* or r0,r1 */
 goto L8c669656;
L8c669656: r[0]=r[3]; /* mov r3,r0 */
 goto L8c669658;
L8c669658: r[1]|=r[2]; /* or r2,r1 */
 goto L8c66965a;
L8c66965a: wr(m,r[15],r[1],4); /* mov.l r1,@r15 */
 goto L8c66965c;
L8c66965c: r[4]+=4u; /* add #0x4,r4 */
 goto L8c66965e;
L8c66965e: r[1]=rd(m,0x8c669698u,4); /* mov.l 0x8c669698,r1 */
 goto L8c669660;
L8c669660: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c669662;
L8c669662: take=(*sr&1u);r[11]=1u;if(take)goto L8c669674;goto L8c669666; /* bt/s 0x8c669674 */
 goto L8c669666;
L8c669666: r[1]=rd(m,0x8c66969cu,4); /* mov.l 0x8c66969c,r1 */
 goto L8c669668;
L8c669668: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c66966a;
L8c66966a: if(!(*sr&1u))goto L8c669670; /* bf 0x8c669670 */
 goto L8c66966c;
L8c66966c: goto L8c669788; /* bra 0x8c669788 */
 goto L8c669670;
L8c669670: goto L8c6697e2; /* bra 0x8c6697e2 */
 goto L8c669674;
L8c669674: *sr=(*sr&~1u)|!(r[9]&r[9]); /* tst r9,r9 */
 goto L8c669676;
L8c669676: if((*sr&1u))goto L8c66967c; /* bt 0x8c66967c */
 goto L8c669678;
L8c669678: goto L8c6697ec; /* bra 0x8c6697ec */
 goto L8c66967c;
L8c66967c: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c66967e;
L8c66967e: r[9]=18u; /* mov #0x12,r9 */
 goto L8c669680;
L8c669680: *sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[9]); /* cmp/ge r9,r1 */
 goto L8c669682;
L8c669682: if((*sr&1u))goto L8c6696a0; /* bt 0x8c6696a0 */
 goto L8c669684;
L8c669684: r[0]=0u;goto L8c6697f6; /* bra 0x8c6697f6 */
 goto L8c6696a0;
L8c6696a0: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c6696a2;
L8c6696a2: r[3]=sx(m,r[4],1); /* mov.b @r4,r3 */
 goto L8c6696a4;
L8c6696a4: r[4]+=2u; /* add #0x2,r4 */
 goto L8c6696a6;
L8c6696a6: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6696a8;
L8c6696a8: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c6696aa;
L8c6696aa: r[0]<<=8; /* shll8 r0 */
 goto L8c6696ac;
L8c6696ac: r[0]|=r[3]; /* or r3,r0 */
 goto L8c6696ae;
L8c6696ae: r[1]=r[0]; /* mov r0,r1 */
 goto L8c6696b0;
L8c6696b0: r[2]=r[0]; /* mov r0,r2 */
 goto L8c6696b2;
L8c6696b2: r[9]=r[0]; /* mov r0,r9 */
 goto L8c6696b4;
L8c6696b4: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c6696b6;
L8c6696b6: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r2 */
 goto L8c6696b8;
L8c6696b8: r[1]<<=8; /* shll8 r1 */
 goto L8c6696ba;
L8c6696ba: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c6696bc;
L8c6696bc: r[1]&=r[14]; /* and r14,r1 */
 goto L8c6696be;
L8c6696be: r[2]|=r[1]; /* or r1,r2 */
 goto L8c6696c0;
L8c6696c0: wr(m,r[6],r[2],4); /* mov.l r2,@r6 */
 goto L8c6696c2;
L8c6696c2: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c6696c4;
L8c6696c4: r[2]=sx(m,r[4],1); /* mov.b @r4,r2 */
 goto L8c6696c6;
L8c6696c6: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6696c8;
L8c6696c8: r[0]<<=8; /* shll8 r0 */
 goto L8c6696ca;
L8c6696ca: r[3]=r[0]; /* mov r0,r3 */
 goto L8c6696cc;
L8c6696cc: r[0]=sx(m,r[4]+0x2u,1); /* mov.b @(0x2,r4),r0 */
 goto L8c6696ce;
L8c6696ce: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c6696d0;
L8c6696d0: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6696d2;
L8c6696d2: r[0]<<=16; /* shll16 r0 */
 goto L8c6696d4;
L8c6696d4: r[3]|=r[0]; /* or r0,r3 */
 goto L8c6696d6;
L8c6696d6: r[0]=sx(m,r[4]+0x3u,1); /* mov.b @(0x3,r4),r0 */
 goto L8c6696d8;
L8c6696d8: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6696da;
L8c6696da: r[0]<<=16; /* shll16 r0 */
 goto L8c6696dc;
L8c6696dc: r[0]<<=8; /* shll8 r0 */
 goto L8c6696de;
L8c6696de: r[3]|=r[0]; /* or r0,r3 */
 goto L8c6696e0;
L8c6696e0: r[3]|=r[2]; /* or r2,r3 */
 goto L8c6696e2;
L8c6696e2: r[9]=r[3]; /* mov r3,r9 */
 goto L8c6696e4;
L8c6696e4: r[3]>>=16; /* shlr16 r3 */
 goto L8c6696e6;
L8c6696e6: r[1]=r[9]; /* mov r9,r1 */
 goto L8c6696e8;
L8c6696e8: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c6696ea;
L8c6696ea: r[3]>>=8; /* shlr8 r3 */
 goto L8c6696ec;
L8c6696ec: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r1 */
 goto L8c6696ee;
L8c6696ee: r[2]=r[9]; /* mov r9,r2 */
 goto L8c6696f0;
L8c6696f0: r[3]=(uint32_t)(int32_t)(int8_t)r[3]; /* exts.b r3,r3 */
 goto L8c6696f2;
L8c6696f2: r[1]&=r[14]; /* and r14,r1 */
 goto L8c6696f4;
L8c6696f4: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c6696f6;
L8c6696f6: r[3]|=r[1]; /* or r1,r3 */
 goto L8c6696f8;
L8c6696f8: r[1]=r[9]; /* mov r9,r1 */
 goto L8c6696fa;
L8c6696fa: r[1]<<=8; /* shll8 r1 */
 goto L8c6696fc;
L8c6696fc: r[2]<<=16; /* shll16 r2 */
 goto L8c6696fe;
L8c6696fe: r[1]&=r[13]; /* and r13,r1 */
 goto L8c669700;
L8c669700: r[2]<<=8; /* shll8 r2 */
 goto L8c669702;
L8c669702: r[3]|=r[1]; /* or r1,r3 */
 goto L8c669704;
L8c669704: r[4]+=4u; /* add #0x4,r4 */
 goto L8c669706;
L8c669706: r[3]|=r[2]; /* or r2,r3 */
 goto L8c669708;
L8c669708: wr(m,r[10],r[3],4); /* mov.l r3,@r10 */
 goto L8c66970a;
L8c66970a: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66970c;
L8c66970c: r[3]=sx(m,r[4],1); /* mov.b @r4,r3 */
 goto L8c66970e;
L8c66970e: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669710;
L8c669710: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c669712;
L8c669712: r[0]<<=8; /* shll8 r0 */
 goto L8c669714;
L8c669714: r[0]|=r[3]; /* or r3,r0 */
 goto L8c669716;
L8c669716: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c669718;
L8c669718: r[2]=r[0]; /* mov r0,r2 */
 goto L8c66971a;
L8c66971a: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r2 */
 goto L8c66971c;
L8c66971c: r[9]=r[0]; /* mov r0,r9 */
 goto L8c66971e;
L8c66971e: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c669720;
L8c669720: r[4]+=2u; /* add #0x2,r4 */
 goto L8c669722;
L8c669722: r[1]=r[0]; /* mov r0,r1 */
 goto L8c669724;
L8c669724: r[1]<<=8; /* shll8 r1 */
 goto L8c669726;
L8c669726: r[1]&=r[14]; /* and r14,r1 */
 goto L8c669728;
L8c669728: r[2]|=r[1]; /* or r1,r2 */
 goto L8c66972a;
L8c66972a: wr(m,r[7],r[2],4); /* mov.l r2,@r7 */
 goto L8c66972c;
L8c66972c: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c66972e;
L8c66972e: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c669730;
L8c669730: r[3]=sx(m,r[4],1); /* mov.b @r4,r3 */
 goto L8c669732;
L8c669732: r[4]+=2u; /* add #0x2,r4 */
 goto L8c669734;
L8c669734: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669736;
L8c669736: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c669738;
L8c669738: r[0]<<=8; /* shll8 r0 */
 goto L8c66973a;
L8c66973a: r[0]|=r[3]; /* or r3,r0 */
 goto L8c66973c;
L8c66973c: r[9]=r[0]&65535u; /* extu.w r0,r9 */
 goto L8c66973e;
L8c66973e: wr(m,r[15],r[0],2); /* mov.w r0,@r15 */
 goto L8c669740;
L8c669740: r[3]=r[9]; /* mov r9,r3 */
 goto L8c669742;
L8c669742: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c669744;
L8c669744: r[1]=r[9]; /* mov r9,r1 */
 goto L8c669746;
L8c669746: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c669748;
L8c669748: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66974a;
L8c66974a: r[9]=r[3]; /* mov r3,r9 */
 goto L8c66974c;
L8c66974c: r[3]=sx(m,r[4],1); /* mov.b @r4,r3 */
 goto L8c66974e;
L8c66974e: r[1]<<=8; /* shll8 r1 */
 goto L8c669750;
L8c669750: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669752;
L8c669752: r[0]<<=8; /* shll8 r0 */
 goto L8c669754;
L8c669754: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c669756;
L8c669756: r[0]|=r[3]; /* or r3,r0 */
 goto L8c669758;
L8c669758: wr(m,r[15]+0x4u,r[0],2); /* mov.w r0,@(0x4,r15) */
 goto L8c66975a;
L8c66975a: r[1]&=r[14]; /* and r14,r1 */
 goto L8c66975c;
L8c66975c: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c66975e;
L8c66975e: r[9]|=r[1]; /* or r1,r9 */
 goto L8c669760;
L8c669760: r[3]=r[2]; /* mov r2,r3 */
 goto L8c669762;
L8c669762: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c669764;
L8c669764: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c669766;
L8c669766: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c669768;
L8c669768: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66976a;
L8c66976a: r[2]<<=8; /* shll8 r2 */
 goto L8c66976c;
L8c66976c: r[2]&=r[14]; /* and r14,r2 */
 goto L8c66976e;
L8c66976e: r[0]|=r[2]; /* or r2,r0 */
 goto L8c669770;
L8c669770: r[2]=sx(m,0x8c669808u,2); /* mov.w 0x8c669808,r2 */
 goto L8c669772;
L8c669772: r[9]=r[9]&65535u; /* extu.w r9,r9 */
 goto L8c669774;
L8c669774: wr(m,r[15],r[0],2); /* mov.w r0,@r15 */
 goto L8c669776;
L8c669776: r[3]=sx(m,r[15],2); /* mov.w @r15,r3 */
 goto L8c669778;
L8c669778: r[2]-=r[9]; /* sub r9,r2 */
 goto L8c66977a;
L8c66977a: r[2]=0u-r[2]; /* neg r2,r2 */
 goto L8c66977c;
L8c66977c: r[3]=r[3]&65535u; /* extu.w r3,r3 */
 goto L8c66977e;
L8c66977e: r[3]=(r[2]&0x80000000u)?((r[2]&31u)?r[3]>>((0u-r[2])&31u):0u):(r[3]<<(r[2]&31u)); /* shld r2,r3 */
 goto L8c669780;
L8c669780: r[4]+=8u; /* add #0x8,r4 */
 goto L8c669782;
L8c669782: wr(m,r[5],r[3],4); /* mov.l r3,@r5 */
 goto L8c669784;
L8c669784: r[9]=r[11];goto L8c6697d6; /* bra 0x8c6697d6 */
 goto L8c669788;
L8c669788: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c66978a;
L8c66978a: if(!(*sr&1u))goto L8c6697ec; /* bf 0x8c6697ec */
 goto L8c66978c;
L8c66978c: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66978e;
L8c66978e: r[2]=sx(m,r[4],1); /* mov.b @r4,r2 */
 goto L8c669790;
L8c669790: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c669792;
L8c669792: r[0]<<=8; /* shll8 r0 */
 goto L8c669794;
L8c669794: r[12]=r[0]; /* mov r0,r12 */
 goto L8c669796;
L8c669796: r[0]=sx(m,r[4]+0x2u,1); /* mov.b @(0x2,r4),r0 */
 goto L8c669798;
L8c669798: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c66979a;
L8c66979a: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66979c;
L8c66979c: r[0]<<=16; /* shll16 r0 */
 goto L8c66979e;
L8c66979e: r[12]|=r[0]; /* or r0,r12 */
 goto L8c6697a0;
L8c6697a0: r[0]=sx(m,r[4]+0x3u,1); /* mov.b @(0x3,r4),r0 */
 goto L8c6697a2;
L8c6697a2: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6697a4;
L8c6697a4: r[0]<<=16; /* shll16 r0 */
 goto L8c6697a6;
L8c6697a6: r[0]<<=8; /* shll8 r0 */
 goto L8c6697a8;
L8c6697a8: r[12]|=r[0]; /* or r0,r12 */
 goto L8c6697aa;
L8c6697aa: r[12]|=r[2]; /* or r2,r12 */
 goto L8c6697ac;
L8c6697ac: r[1]=r[12]; /* mov r12,r1 */
 goto L8c6697ae;
L8c6697ae: r[0]=r[12]; /* mov r12,r0 */
 goto L8c6697b0;
L8c6697b0: r[1]>>=16; /* shlr16 r1 */
 goto L8c6697b2;
L8c6697b2: r[2]=r[12]; /* mov r12,r2 */
 goto L8c6697b4;
L8c6697b4: r[0]>>=8; /* shlr8 r0 */
 goto L8c6697b6;
L8c6697b6: r[1]>>=8; /* shlr8 r1 */
 goto L8c6697b8;
L8c6697b8: r[0]&=r[14]; /* and r14,r0 */
 goto L8c6697ba;
L8c6697ba: r[1]=r[1]&255u; /* extu.b r1,r1 */
 goto L8c6697bc;
L8c6697bc: r[1]|=r[0]; /* or r0,r1 */
 goto L8c6697be;
L8c6697be: r[0]=r[12]; /* mov r12,r0 */
 goto L8c6697c0;
L8c6697c0: r[0]<<=8; /* shll8 r0 */
 goto L8c6697c2;
L8c6697c2: r[0]&=r[13]; /* and r13,r0 */
 goto L8c6697c4;
L8c6697c4: r[2]<<=16; /* shll16 r2 */
 goto L8c6697c6;
L8c6697c6: r[1]|=r[0]; /* or r0,r1 */
 goto L8c6697c8;
L8c6697c8: r[12]=r[1]; /* mov r1,r12 */
 goto L8c6697ca;
L8c6697ca: r[2]<<=8; /* shll8 r2 */
 goto L8c6697cc;
L8c6697cc: r[4]+=4u; /* add #0x4,r4 */
 goto L8c6697ce;
L8c6697ce: r[12]|=r[2]; /* or r2,r12 */
 goto L8c6697d0;
L8c6697d0: r[12]+=r[4]; /* add r4,r12 */
 goto L8c6697d2;
L8c6697d2: wr(m,r[15]+0x8u,r[12],4); /* mov.l r12,@(0x8,r15) */
 goto L8c6697d4;
L8c6697d4: r[12]=r[11]; /* mov r11,r12 */
 goto L8c6697d6;
L8c6697d6: *sr=(*sr&~1u)|!(r[9]&r[9]); /* tst r9,r9 */
 goto L8c6697d8;
L8c6697d8: if((*sr&1u))goto L8c6697ec; /* bt 0x8c6697ec */
 goto L8c6697da;
L8c6697da: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c6697dc;
L8c6697dc: if((*sr&1u))goto L8c6697ec; /* bt 0x8c6697ec */
 goto L8c6697de;
L8c6697de: goto L8c6697f4; /* bra 0x8c6697f4 */
 goto L8c6697e2;
L8c6697e2: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c6697e4;
L8c6697e4: r[11]=4294967294u; /* mov #-0x2,r11 */
 goto L8c6697e6;
L8c6697e6: r[1]+=1u; /* add #0x1,r1 */
 goto L8c6697e8;
L8c6697e8: r[1]&=r[11]; /* and r11,r1 */
 goto L8c6697ea;
L8c6697ea: r[4]+=r[1]; /* add r1,r4 */
 goto L8c6697ec;
L8c6697ec: *sr=(*sr&~1u)|(r[4]>=r[8]); /* cmp/hs r8,r4 */
 goto L8c6697ee;
L8c6697ee: if((*sr&1u))goto L8c6697f4; /* bt 0x8c6697f4 */
 goto L8c6697f0;
L8c6697f0: goto L8c6695f0; /* bra 0x8c6695f0 */
 goto L8c6697f4;
L8c6697f4: r[0]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r0 */
 goto L8c6697f6;
L8c6697f6: r[15]+=12u; /* add #0xc,r15 */
 goto L8c6697f8;
L8c6697f8: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c6697fa;
L8c6697fa: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c6697fc;
L8c6697fc: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c6697fe;
L8c6697fe: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c669800;
L8c669800: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c669802;
L8c669802: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c669804;
L8c669804: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
