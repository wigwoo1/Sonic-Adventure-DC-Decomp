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
/* Static C recovery of retail Tikal helper 8c65f810..8c65fa7a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_request_queue(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f810;
L8c65f810: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65f812;
L8c65f812: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65f814;
L8c65f814: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65f816;
L8c65f816: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65f818;
L8c65f818: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65f81a;
L8c65f81a: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65f81c;
L8c65f81c: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c65f81e;
L8c65f81e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65f820;
L8c65f820: r[15]+=4294967268u; /* add #-0x1c,r15 */
 goto L8c65f822;
L8c65f822: r[10]=44u; /* mov #0x2c,r10 */
 goto L8c65f824;
L8c65f824: r[9]=r[4]; /* mov r4,r9 */
 goto L8c65f826;
L8c65f826: *macl=r[10]*r[9]; /* mul.l r10,r9 */
 goto L8c65f828;
L8c65f828: r[14]=rd(m,0x8c65f850u,4); /* mov.l 0x8c65f850,r14 */
 goto L8c65f82a;
L8c65f82a: r[0]=r[5]; /* mov r5,r0 */
 goto L8c65f82c;
L8c65f82c: wr(m,r[15]+0x4u,r[0],1); /* mov.b r0,@(0x4,r15) */
 goto L8c65f82e;
L8c65f82e: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65f830;
L8c65f830: wr(m,r[15]+0x18u,r[6],4); /* mov.l r6,@(0x18,r15) */
 goto L8c65f832;
L8c65f832: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c65f834;
L8c65f834: r[10]=*macl; /* sts MACL,r10 */
 goto L8c65f836;
L8c65f836: r[3]=r[4]; /* mov r4,r3 */
 goto L8c65f838;
L8c65f838: r[3]+=40u; /* add #0x28,r3 */
 goto L8c65f83a;
L8c65f83a: r[12]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r12 */
 goto L8c65f83c;
L8c65f83c: r[3]+=r[10]; /* add r10,r3 */
 goto L8c65f83e;
L8c65f83e: r[0]=sx(m,r[3]+0x6u,1); /* mov.b @(0x6,r3),r0 */
 goto L8c65f840;
L8c65f840: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c65f842;
L8c65f842: take=(*sr&1u);r[11]=r[7];if(take)goto L8c65f854;goto L8c65f846; /* bt/s 0x8c65f854 */
 goto L8c65f846;
L8c65f846: r[0]=4294967295u;goto L8c65fa66; /* bra 0x8c65fa66 */
 goto L8c65f854;
L8c65f854: r[0]=sx(m,r[15]+0x4u,1); /* mov.b @(0x4,r15),r0 */
 goto L8c65f856;
L8c65f856: r[2]=r[15]; /* mov r15,r2 */
 goto L8c65f858;
L8c65f858: r[5]=sx(m,0x8c65f950u,2); /* mov.w 0x8c65f950,r5 */
 goto L8c65f85a;
L8c65f85a: r[2]+=16u; /* add #0x10,r2 */
 goto L8c65f85c;
L8c65f85c: wr(m,r[3]+0x6u,r[0],1); /* mov.b r0,@(0x6,r3) */
 goto L8c65f85e;
L8c65f85e: r[8]=r[11]; /* mov r11,r8 */
 goto L8c65f860;
L8c65f860: r[0]=rd(m,0x8c65f960u,4); /* mov.l 0x8c65f960,r0 */
 goto L8c65f862;
L8c65f862: r[8]+=r[12]; /* add r12,r8 */
 goto L8c65f864;
L8c65f864: r[4]=r[8]; /* mov r8,r4 */
 goto L8c65f866;
L8c65f866: r[4]+=3u; /* add #0x3,r4 */
 goto L8c65f868;
L8c65f868: wr(m,r[15]+0x8u,r[4],4); /* mov.l r4,@(0x8,r15) */
 goto L8c65f86a;
L8c65f86a: r[4]=3u; /* mov #0x3,r4 */
 goto L8c65f86c;
L8c65f86c: r[3]=sx(m,r[9]+r[0],1); /* mov.b @(r0,r9),r3 */
 goto L8c65f86e;
L8c65f86e: r[0]=sx(m,0x8c65f952u,2); /* mov.w 0x8c65f952,r0 */
 goto L8c65f870;
L8c65f870: r[5]&=r[3]; /* and r3,r5 */
 goto L8c65f872;
L8c65f872: wr(m,r[2],r[3],1); /* mov.b r3,@r2 */
 goto L8c65f874;
L8c65f874: r[5]=r[5]&255u; /* extu.b r5,r5 */
 goto L8c65f876;
L8c65f876: wr(m,r[15]+0xcu,r[5],4); /* mov.l r5,@(0xc,r15) */
 goto L8c65f878;
L8c65f878: r[2]=4294967290u; /* mov #-0x6,r2 */
 goto L8c65f87a;
L8c65f87a: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[5]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[5]=n?(uint32_t)((int32_t)r[5]>>n):((r[5]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r5 */
 goto L8c65f87c;
L8c65f87c: r[5]&=r[4]; /* and r4,r5 */
 goto L8c65f87e;
L8c65f87e: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c65f880;
L8c65f880: r[13]=rd(m,r[14],4); /* mov.l @r14,r13 */
 goto L8c65f882;
L8c65f882: r[2]=r[13]; /* mov r13,r2 */
 goto L8c65f884;
L8c65f884: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c65f886;
L8c65f886: *sr=(*sr&~1u)|((int32_t)r[3]>0); /* cmp/pl r3 */
 goto L8c65f888;
L8c65f888: if(!(*sr&1u))goto L8c65f8aa; /* bf 0x8c65f8aa */
 goto L8c65f88a;
L8c65f88a: r[0]=sx(m,0x8c65f954u,2); /* mov.w 0x8c65f954,r0 */
 goto L8c65f88c;
L8c65f88c: r[1]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r1 */
 goto L8c65f88e;
L8c65f88e: *sr=(*sr&~1u)|((int32_t)r[1]>0); /* cmp/pl r1 */
 goto L8c65f890;
L8c65f890: if(!(*sr&1u))goto L8c65f8aa; /* bf 0x8c65f8aa */
 goto L8c65f892;
L8c65f892: r[0]=sx(m,0x8c65f956u,2); /* mov.w 0x8c65f956,r0 */
 goto L8c65f894;
L8c65f894: r[2]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r2 */
 goto L8c65f896;
L8c65f896: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c65f898;
L8c65f898: r[3]>>=16; /* shlr16 r3 */
 goto L8c65f89a;
L8c65f89a: r[3]=(uint32_t)(int32_t)(int16_t)r[3]; /* exts.w r3,r3 */
 goto L8c65f89c;
L8c65f89c: r[4]&=r[3]; /* and r3,r4 */
 goto L8c65f89e;
L8c65f89e: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65f8a0;
L8c65f8a0: *sr=(*sr&~1u)|(r[4]==r[3]); /* cmp/eq r4,r3 */
 goto L8c65f8a2;
L8c65f8a2: if(!(*sr&1u))goto L8c65f8aa; /* bf 0x8c65f8aa */
 goto L8c65f8a4;
L8c65f8a4: r[0]=sx(m,0x8c65f952u,2); /* mov.w 0x8c65f952,r0 */
 goto L8c65f8a6;
L8c65f8a6: *pr=0x8c65f8aau;r[4]=rd(m,r[13]+r[0],4);status=call(ctx,0x8c65f7ccu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65f7cc */
 goto L8c65f8aa;
L8c65f8aa: r[0]=sx(m,0x8c65f956u,2); /* mov.w 0x8c65f956,r0 */
 goto L8c65f8ac;
L8c65f8ac: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c65f8ae;
L8c65f8ae: r[1]=r[0]; /* mov r0,r1 */
 goto L8c65f8b0;
L8c65f8b0: r[1]+=12u; /* add #0xc,r1 */
 goto L8c65f8b2;
L8c65f8b2: r[3]=r[2]; /* mov r2,r3 */
 goto L8c65f8b4;
L8c65f8b4: r[1]+=r[3]; /* add r3,r1 */
 goto L8c65f8b6;
L8c65f8b6: r[1]=rd(m,r[1],4); /* mov.l @r1,r1 */
 goto L8c65f8b8;
L8c65f8b8: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c65f8ba;
L8c65f8ba: r[1]<<=2; /* shll2 r1 */
 goto L8c65f8bc;
L8c65f8bc: r[3]+=r[1]; /* add r1,r3 */
 goto L8c65f8be;
L8c65f8be: wr(m,r[2]+r[0],r[3],4); /* mov.l r3,@(r0,r2) */
 goto L8c65f8c0;
L8c65f8c0: r[1]=r[8]&255u; /* extu.b r8,r1 */
 goto L8c65f8c2;
L8c65f8c2: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c65f8c4;
L8c65f8c4: r[13]=rd(m,r[14],4); /* mov.l @r14,r13 */
 goto L8c65f8c6;
L8c65f8c6: r[2]<<=16; /* shll16 r2 */
 goto L8c65f8c8;
L8c65f8c8: r[13]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r13 */
 goto L8c65f8ca;
L8c65f8ca: r[2]|=r[1]; /* or r1,r2 */
 goto L8c65f8cc;
L8c65f8cc: wr(m,r[15]+0x14u,r[1],4); /* mov.l r1,@(0x14,r15) */
 goto L8c65f8ce;
L8c65f8ce: r[1]=sx(m,0x8c65f958u,2); /* mov.w 0x8c65f958,r1 */
 goto L8c65f8d0;
L8c65f8d0: wr(m,r[13],r[2],4); /* mov.l r2,@r13 */
 goto L8c65f8d2;
L8c65f8d2: r[13]+=4u; /* add #0x4,r13 */
 goto L8c65f8d4;
L8c65f8d4: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65f8d6;
L8c65f8d6: r[13]+=4u; /* add #0x4,r13 */
 goto L8c65f8d8;
L8c65f8d8: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65f8da;
L8c65f8da: r[3]=r[0]; /* mov r0,r3 */
 goto L8c65f8dc;
L8c65f8dc: r[1]+=r[3]; /* add r3,r1 */
 goto L8c65f8de;
L8c65f8de: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c65f8e0;
L8c65f8e0: r[2]=r[3]; /* mov r3,r2 */
 goto L8c65f8e2;
L8c65f8e2: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c65f8e4;
L8c65f8e4: r[3]+=r[2]; /* add r2,r3 */
 goto L8c65f8e6;
L8c65f8e6: r[3]<<=2; /* shll2 r3 */
 goto L8c65f8e8;
L8c65f8e8: r[3]<<=2; /* shll2 r3 */
 goto L8c65f8ea;
L8c65f8ea: r[2]=sx(m,0x8c65f95au,2); /* mov.w 0x8c65f95a,r2 */
 goto L8c65f8ec;
L8c65f8ec: r[4]=r[9]; /* mov r9,r4 */
 goto L8c65f8ee;
L8c65f8ee: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c65f8f0;
L8c65f8f0: r[0]+=r[2]; /* add r2,r0 */
 goto L8c65f8f2;
L8c65f8f2: r[4]<<=2; /* shll2 r4 */
 goto L8c65f8f4;
L8c65f8f4: r[0]+=r[3]; /* add r3,r0 */
 goto L8c65f8f6;
L8c65f8f6: *pr=0x8c65f8fau;r[4]=rd(m,r[4]+r[0],4);status=call(ctx,0x8c6607beu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6607be */
 goto L8c65f8fa;
L8c65f8fa: r[2]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r2 */
 goto L8c65f8fc;
L8c65f8fc: r[1]=sx(m,0x8c65f95cu,2); /* mov.w 0x8c65f95c,r1 */
 goto L8c65f8fe;
L8c65f8fe: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c65f900;
L8c65f900: r[0]=sx(m,r[15]+0x4u,1); /* mov.b @(0x4,r15),r0 */
 goto L8c65f902;
L8c65f902: r[6]=rd(m,0x8c65f964u,4); /* mov.l 0x8c65f964,r6 */
 goto L8c65f904;
L8c65f904: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c65f906;
L8c65f906: r[7]=rd(m,r[15]+0x18u,4); /* mov.l @(0x18,r15),r7 */
 goto L8c65f908;
L8c65f908: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c65f90a;
L8c65f90a: r[0]=16u; /* mov #0x10,r0 */
 goto L8c65f90c;
L8c65f90c: r[9]=sx(m,r[15]+r[0],1); /* mov.b @(r0,r15),r9 */
 goto L8c65f90e;
L8c65f90e: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65f910;
L8c65f910: r[4]=rd(m,0x8c65f96cu,4); /* mov.l 0x8c65f96c,r4 */
 goto L8c65f912;
L8c65f912: r[0]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c65f914;
L8c65f914: r[5]=rd(m,0x8c65f968u,4); /* mov.l 0x8c65f968,r5 */
 goto L8c65f916;
L8c65f916: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65f918;
L8c65f918: take=!(*sr&1u);r[9]=r[9]&255u;if(take)goto L8c65f970;goto L8c65f91c; /* bf/s 0x8c65f970 */
 goto L8c65f91c;
L8c65f91c: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65f91e;
L8c65f91e: r[9]<<=16; /* shll16 r9 */
 goto L8c65f920;
L8c65f920: r[0]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r0 */
 goto L8c65f922;
L8c65f922: r[9]&=r[4]; /* and r4,r9 */
 goto L8c65f924;
L8c65f924: r[3]<<=16; /* shll16 r3 */
 goto L8c65f926;
L8c65f926: *sr=(*sr&~1u)|((int32_t)r[11]>0); /* cmp/pl r11 */
 goto L8c65f928;
L8c65f928: r[3]<<=8; /* shll8 r3 */
 goto L8c65f92a;
L8c65f92a: r[0]<<=8; /* shll8 r0 */
 goto L8c65f92c;
L8c65f92c: r[3]&=r[5]; /* and r5,r3 */
 goto L8c65f92e;
L8c65f92e: r[0]&=r[6]; /* and r6,r0 */
 goto L8c65f930;
L8c65f930: r[3]|=r[9]; /* or r9,r3 */
 goto L8c65f932;
L8c65f932: r[3]|=r[0]; /* or r0,r3 */
 goto L8c65f934;
L8c65f934: r[0]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r0 */
 goto L8c65f936;
L8c65f936: r[3]|=r[0]; /* or r0,r3 */
 goto L8c65f938;
L8c65f938: wr(m,r[13],r[3],4); /* mov.l r3,@r13 */
 goto L8c65f93a;
L8c65f93a: r[13]+=4u; /* add #0x4,r13 */
 goto L8c65f93c;
L8c65f93c: take=!(*sr&1u);r[9]=0u;if(take)goto L8c65f9c4;goto L8c65f940; /* bf/s 0x8c65f9c4 */
 goto L8c65f940;
L8c65f940: r[3]=rd(m,r[7],4);r[7]+=4; /* mov.l @r7+,r3 */
 goto L8c65f942;
L8c65f942: r[9]+=1u; /* add #0x1,r9 */
 goto L8c65f944;
L8c65f944: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[11]); /* cmp/ge r11,r9 */
 goto L8c65f946;
L8c65f946: wr(m,r[13],r[3],4); /* mov.l r3,@r13 */
 goto L8c65f948;
L8c65f948: take=!(*sr&1u);r[13]+=4u;if(take)goto L8c65f940;goto L8c65f94c; /* bf/s 0x8c65f940 */
 goto L8c65f94c;
L8c65f94c: goto L8c65f9c4; /* bra 0x8c65f9c4 */
 goto L8c65f970;
L8c65f970: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c65f972;
L8c65f972: r[9]<<=8; /* shll8 r9 */
 goto L8c65f974;
L8c65f974: r[8]<<=16; /* shll16 r8 */
 goto L8c65f976;
L8c65f976: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c65f978;
L8c65f978: r[3]<<=16; /* shll16 r3 */
 goto L8c65f97a;
L8c65f97a: *sr=(*sr&~1u)|((int32_t)r[11]>0); /* cmp/pl r11 */
 goto L8c65f97c;
L8c65f97c: r[8]<<=8; /* shll8 r8 */
 goto L8c65f97e;
L8c65f97e: r[3]&=r[4]; /* and r4,r3 */
 goto L8c65f980;
L8c65f980: r[9]&=r[6]; /* and r6,r9 */
 goto L8c65f982;
L8c65f982: r[8]&=r[5]; /* and r5,r8 */
 goto L8c65f984;
L8c65f984: r[3]|=r[9]; /* or r9,r3 */
 goto L8c65f986;
L8c65f986: r[3]|=r[8]; /* or r8,r3 */
 goto L8c65f988;
L8c65f988: r[3]|=r[0]; /* or r0,r3 */
 goto L8c65f98a;
L8c65f98a: wr(m,r[13],r[3],4); /* mov.l r3,@r13 */
 goto L8c65f98c;
L8c65f98c: r[13]+=4u; /* add #0x4,r13 */
 goto L8c65f98e;
L8c65f98e: take=!(*sr&1u);r[8]=0u;if(take)goto L8c65f9c4;goto L8c65f992; /* bf/s 0x8c65f9c4 */
 goto L8c65f992;
L8c65f992: r[9]=rd(m,r[7],4);r[7]+=4; /* mov.l @r7+,r9 */
 goto L8c65f994;
L8c65f994: r[8]+=1u; /* add #0x1,r8 */
 goto L8c65f996;
L8c65f996: *sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[11]); /* cmp/ge r11,r8 */
 goto L8c65f998;
L8c65f998: r[2]=r[9]; /* mov r9,r2 */
 goto L8c65f99a;
L8c65f99a: r[3]=r[9]; /* mov r9,r3 */
 goto L8c65f99c;
L8c65f99c: r[2]<<=16; /* shll16 r2 */
 goto L8c65f99e;
L8c65f99e: r[1]=r[9]; /* mov r9,r1 */
 goto L8c65f9a0;
L8c65f9a0: r[3]<<=8; /* shll8 r3 */
 goto L8c65f9a2;
L8c65f9a2: r[2]<<=8; /* shll8 r2 */
 goto L8c65f9a4;
L8c65f9a4: r[3]&=r[4]; /* and r4,r3 */
 goto L8c65f9a6;
L8c65f9a6: r[2]&=r[5]; /* and r5,r2 */
 goto L8c65f9a8;
L8c65f9a8: r[2]|=r[3]; /* or r3,r2 */
 goto L8c65f9aa;
L8c65f9aa: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c65f9ac;
L8c65f9ac: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r1 */
 goto L8c65f9ae;
L8c65f9ae: r[1]&=r[6]; /* and r6,r1 */
 goto L8c65f9b0;
L8c65f9b0: r[2]|=r[1]; /* or r1,r2 */
 goto L8c65f9b2;
L8c65f9b2: r[1]=r[9]; /* mov r9,r1 */
 goto L8c65f9b4;
L8c65f9b4: r[1]>>=16; /* shlr16 r1 */
 goto L8c65f9b6;
L8c65f9b6: r[1]>>=8; /* shlr8 r1 */
 goto L8c65f9b8;
L8c65f9b8: r[1]=(uint32_t)(int32_t)(int8_t)r[1]; /* exts.b r1,r1 */
 goto L8c65f9ba;
L8c65f9ba: r[1]=r[1]&255u; /* extu.b r1,r1 */
 goto L8c65f9bc;
L8c65f9bc: r[2]|=r[1]; /* or r1,r2 */
 goto L8c65f9be;
L8c65f9be: wr(m,r[13],r[2],4); /* mov.l r2,@r13 */
 goto L8c65f9c0;
L8c65f9c0: take=!(*sr&1u);r[13]+=4u;if(take)goto L8c65f992;goto L8c65f9c4; /* bf/s 0x8c65f992 */
 goto L8c65f9c4;
L8c65f9c4: r[1]=sx(m,0x8c65fa7au,2); /* mov.w 0x8c65fa7a,r1 */
 goto L8c65f9c6;
L8c65f9c6: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65f9c8;
L8c65f9c8: r[0]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c65f9ca;
L8c65f9ca: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65f9cc;
L8c65f9cc: if(!(*sr&1u))goto L8c65fa06; /* bf 0x8c65fa06 */
 goto L8c65f9ce;
L8c65f9ce: r[7]=rd(m,r[15]+0x3cu,4); /* mov.l @(0x3c,r15),r7 */
 goto L8c65f9d0;
L8c65f9d0: *sr=(*sr&~1u)|((int32_t)r[12]>0); /* cmp/pl r12 */
 goto L8c65f9d2;
L8c65f9d2: take=!(*sr&1u);r[11]=0u;if(take)goto L8c65fa1a;goto L8c65f9d6; /* bf/s 0x8c65fa1a */
 goto L8c65f9d6;
L8c65f9d6: r[0]=sx(m,r[7]+0x1u,1); /* mov.b @(0x1,r7),r0 */
 goto L8c65f9d8;
L8c65f9d8: r[11]+=1u; /* add #0x1,r11 */
 goto L8c65f9da;
L8c65f9da: r[1]=sx(m,r[7],1); /* mov.b @r7,r1 */
 goto L8c65f9dc;
L8c65f9dc: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[12]); /* cmp/ge r12,r11 */
 goto L8c65f9de;
L8c65f9de: r[0]<<=16; /* shll16 r0 */
 goto L8c65f9e0;
L8c65f9e0: r[0]&=r[4]; /* and r4,r0 */
 goto L8c65f9e2;
L8c65f9e2: r[3]=r[0]; /* mov r0,r3 */
 goto L8c65f9e4;
L8c65f9e4: r[0]=sx(m,r[7]+0x2u,1); /* mov.b @(0x2,r7),r0 */
 goto L8c65f9e6;
L8c65f9e6: r[1]<<=16; /* shll16 r1 */
 goto L8c65f9e8;
L8c65f9e8: r[0]<<=8; /* shll8 r0 */
 goto L8c65f9ea;
L8c65f9ea: r[0]&=r[6]; /* and r6,r0 */
 goto L8c65f9ec;
L8c65f9ec: r[3]|=r[0]; /* or r0,r3 */
 goto L8c65f9ee;
L8c65f9ee: r[0]=sx(m,r[7]+0x3u,1); /* mov.b @(0x3,r7),r0 */
 goto L8c65f9f0;
L8c65f9f0: r[1]<<=8; /* shll8 r1 */
 goto L8c65f9f2;
L8c65f9f2: r[1]&=r[5]; /* and r5,r1 */
 goto L8c65f9f4;
L8c65f9f4: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c65f9f6;
L8c65f9f6: r[3]|=r[1]; /* or r1,r3 */
 goto L8c65f9f8;
L8c65f9f8: r[3]|=r[0]; /* or r0,r3 */
 goto L8c65f9fa;
L8c65f9fa: wr(m,r[13],r[3],4); /* mov.l r3,@r13 */
 goto L8c65f9fc;
L8c65f9fc: r[13]+=4u; /* add #0x4,r13 */
 goto L8c65f9fe;
L8c65f9fe: take=!(*sr&1u);r[7]+=4u;if(take)goto L8c65f9d6;goto L8c65fa02; /* bf/s 0x8c65f9d6 */
 goto L8c65fa02;
L8c65fa02: goto L8c65fa1a; /* bra 0x8c65fa1a */
 goto L8c65fa06;
L8c65fa06: r[5]=rd(m,r[15]+0x3cu,4); /* mov.l @(0x3c,r15),r5 */
 goto L8c65fa08;
L8c65fa08: *sr=(*sr&~1u)|((int32_t)r[12]>0); /* cmp/pl r12 */
 goto L8c65fa0a;
L8c65fa0a: take=!(*sr&1u);r[4]=0u;if(take)goto L8c65fa1a;goto L8c65fa0e; /* bf/s 0x8c65fa1a */
 goto L8c65fa0e;
L8c65fa0e: r[3]=rd(m,r[5],4);r[5]+=4; /* mov.l @r5+,r3 */
 goto L8c65fa10;
L8c65fa10: r[4]+=1u; /* add #0x1,r4 */
 goto L8c65fa12;
L8c65fa12: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[12]); /* cmp/ge r12,r4 */
 goto L8c65fa14;
L8c65fa14: wr(m,r[13],r[3],4); /* mov.l r3,@r13 */
 goto L8c65fa16;
L8c65fa16: take=!(*sr&1u);r[13]+=4u;if(take)goto L8c65fa0e;goto L8c65fa1a; /* bf/s 0x8c65fa0e */
 goto L8c65fa1a;
L8c65fa1a: r[0]=sx(m,0x8c65fa7cu,2); /* mov.w 0x8c65fa7c,r0 */
 goto L8c65fa1c;
L8c65fa1c: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c65fa1e;
L8c65fa1e: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65fa20;
L8c65fa20: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65fa22;
L8c65fa22: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65fa24;
L8c65fa24: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65fa26;
L8c65fa26: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c65fa28;
L8c65fa28: r[1]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r1 */
 goto L8c65fa2a;
L8c65fa2a: r[2]<<=2; /* shll2 r2 */
 goto L8c65fa2c;
L8c65fa2c: r[1]+=r[2]; /* add r2,r1 */
 goto L8c65fa2e;
L8c65fa2e: wr(m,r[3]+r[0],r[1],4); /* mov.l r1,@(r0,r3) */
 goto L8c65fa30;
L8c65fa30: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65fa32;
L8c65fa32: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65fa34;
L8c65fa34: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c65fa36;
L8c65fa36: r[1]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r1 */
 goto L8c65fa38;
L8c65fa38: r[2]+=40u; /* add #0x28,r2 */
 goto L8c65fa3a;
L8c65fa3a: r[1]+=1u; /* add #0x1,r1 */
 goto L8c65fa3c;
L8c65fa3c: wr(m,r[3]+r[0],r[1],4); /* mov.l r1,@(r0,r3) */
 goto L8c65fa3e;
L8c65fa3e: r[1]+=4294967295u; /* add #-0x1,r1 */
 goto L8c65fa40;
L8c65fa40: r[2]+=r[10]; /* add r10,r2 */
 goto L8c65fa42;
L8c65fa42: wr(m,r[2]+0x18u,r[1],4); /* mov.l r1,@(0x18,r2) */
 goto L8c65fa44;
L8c65fa44: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65fa46;
L8c65fa46: r[2]=rd(m,0x8c65fa80u,4); /* mov.l 0x8c65fa80,r2 */
 goto L8c65fa48;
L8c65fa48: r[1]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r1 */
 goto L8c65fa4a;
L8c65fa4a: r[1]&=r[2]; /* and r2,r1 */
 goto L8c65fa4c;
L8c65fa4c: wr(m,r[3]+r[0],r[1],4); /* mov.l r1,@(r0,r3) */
 goto L8c65fa4e;
L8c65fa4e: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65fa50;
L8c65fa50: r[1]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r1 */
 goto L8c65fa52;
L8c65fa52: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c65fa54;
L8c65fa54: if(!(*sr&1u))goto L8c65fa5e; /* bf 0x8c65fa5e */
 goto L8c65fa56;
L8c65fa56: r[0]=sx(m,0x8c65fa7eu,2); /* mov.w 0x8c65fa7e,r0 */
 goto L8c65fa58;
L8c65fa58: r[3]=1u; /* mov #0x1,r3 */
 goto L8c65fa5a;
L8c65fa5a: r[1]=rd(m,r[14],4); /* mov.l @r14,r1 */
 goto L8c65fa5c;
L8c65fa5c: wr(m,r[1]+r[0],r[3],4); /* mov.l r3,@(r0,r1) */
 goto L8c65fa5e;
L8c65fa5e: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c65fa60;
L8c65fa60: r[0]+=40u; /* add #0x28,r0 */
 goto L8c65fa62;
L8c65fa62: r[0]+=r[10]; /* add r10,r0 */
 goto L8c65fa64;
L8c65fa64: r[0]=rd(m,r[0]+0x18u,4); /* mov.l @(0x18,r0),r0 */
 goto L8c65fa66;
L8c65fa66: r[15]+=28u; /* add #0x1c,r15 */
 goto L8c65fa68;
L8c65fa68: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65fa6a;
L8c65fa6a: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c65fa6c;
L8c65fa6c: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65fa6e;
L8c65fa6e: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65fa70;
L8c65fa70: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65fa72;
L8c65fa72: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65fa74;
L8c65fa74: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65fa76;
L8c65fa76: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
