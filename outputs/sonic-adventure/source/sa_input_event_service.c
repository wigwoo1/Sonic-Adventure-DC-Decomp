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
/* Static C recovery of retail Tikal helper 8c6482a0..8c64839a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_event_service(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6482a0;
L8c6482a0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6482a2;
L8c6482a2: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6482a4;
L8c6482a4: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6482a6;
L8c6482a6: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c6482a8;
L8c6482a8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6482aa;
L8c6482aa: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c6482ac;
L8c6482ac: r[14]=sx(m,0x8c648360u,2); /* mov.w 0x8c648360,r14 */
 goto L8c6482ae;
L8c6482ae: r[13]=r[4]; /* mov r4,r13 */
 goto L8c6482b0;
L8c6482b0: r[2]=rd(m,0x8c648370u,4); /* mov.l 0x8c648370,r2 */
 goto L8c6482b2;
L8c6482b2: *macl=r[14]*r[13]; /* mul.l r14,r13 */
 goto L8c6482b4;
L8c6482b4: r[0]=rd(m,r[2],4); /* mov.l @r2,r0 */
 goto L8c6482b6;
L8c6482b6: r[3]=rd(m,0x8c648368u,4); /* mov.l 0x8c648368,r3 */
 goto L8c6482b8;
L8c6482b8: r[0]=rd(m,r[0]+0x8u,4); /* mov.l @(0x8,r0),r0 */
 goto L8c6482ba;
L8c6482ba: r[14]=*macl; /* sts MACL,r14 */
 goto L8c6482bc;
L8c6482bc: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c6482be;
L8c6482be: take=!(*sr&1u);r[14]+=r[3];if(take)goto L8c6482d2;goto L8c6482c2; /* bf/s 0x8c6482d2 */
 goto L8c6482c2;
L8c6482c2: r[4]=r[13]; /* mov r13,r4 */
 goto L8c6482c4;
L8c6482c4: r[15]+=4u; /* add #0x4,r15 */
 goto L8c6482c6;
L8c6482c6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6482c8;
L8c6482c8: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c6482ca;
L8c6482ca: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6482cc;
L8c6482cc: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6482ce;
L8c6482ce: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c648470u,m,f,sr,r,pr); /* bra 0x8c648470 */
 goto L8c6482d2;
L8c6482d2: r[0]=sx(m,r[14]+0x2u,1); /* mov.b @(0x2,r14),r0 */
 goto L8c6482d4;
L8c6482d4: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c6482d6;
L8c6482d6: if((*sr&1u))goto L8c648326; /* bt 0x8c648326 */
 goto L8c6482d8;
L8c6482d8: r[0]=rd(m,0x8c648374u,4); /* mov.l 0x8c648374,r0 */
 goto L8c6482da;
L8c6482da: r[11]=1u; /* mov #0x1,r11 */
 goto L8c6482dc;
L8c6482dc: wr(m,r[14]+0x30u,r[11],4); /* mov.l r11,@(0x30,r14) */
 goto L8c6482de;
L8c6482de: r[2]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r2 */
 goto L8c6482e0;
L8c6482e0: r[3]=r[2]; /* mov r2,r3 */
 goto L8c6482e2;
L8c6482e2: r[2]<<=2; /* shll2 r2 */
 goto L8c6482e4;
L8c6482e4: r[2]+=r[3]; /* add r3,r2 */
 goto L8c6482e6;
L8c6482e6: r[3]=r[13]; /* mov r13,r3 */
 goto L8c6482e8;
L8c6482e8: r[2]<<=2; /* shll2 r2 */
 goto L8c6482ea;
L8c6482ea: r[3]<<=2; /* shll2 r3 */
 goto L8c6482ec;
L8c6482ec: r[2]<<=2; /* shll2 r2 */
 goto L8c6482ee;
L8c6482ee: r[3]<<=2; /* shll2 r3 */
 goto L8c6482f0;
L8c6482f0: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c6482f2;
L8c6482f2: r[3]<<=2; /* shll2 r3 */
 goto L8c6482f4;
L8c6482f4: r[2]=rd(m,0x8c648378u,4); /* mov.l 0x8c648378,r2 */
 goto L8c6482f6;
L8c6482f6: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c6482f8;
L8c6482f8: wr(m,r[14]+0x34u,r[1],4); /* mov.l r1,@(0x34,r14) */
 goto L8c6482fa;
L8c6482fa: r[3]+=r[2]; /* add r2,r3 */
 goto L8c6482fc;
L8c6482fc: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c6482fe;
L8c6482fe: r[6]=rd(m,r[14]+0x30u,4); /* mov.l @(0x30,r14),r6 */
 goto L8c648300;
L8c648300: r[3]=rd(m,0x8c64837cu,4); /* mov.l 0x8c64837c,r3 */
 goto L8c648302;
L8c648302: r[6]+=1u; /* add #0x1,r6 */
 goto L8c648304;
L8c648304: r[7]=rd(m,r[14]+0x34u,4); /* mov.l @(0x34,r14),r7 */
 goto L8c648306;
L8c648306: wr(m,r[14]+0x30u,r[6],4); /* mov.l r6,@(0x30,r14) */
 goto L8c648308;
L8c648308: r[6]+=4294967295u; /* add #-0x1,r6 */
 goto L8c64830a;
L8c64830a: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c64830c;
L8c64830c: r[5]=rd(m,r[5]+0x8u,4); /* mov.l @(0x8,r5),r5 */
 goto L8c64830e;
L8c64830e: target=r[3];*pr=0x8c648312u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c648312;
L8c648312: r[2]=rd(m,0x8c648380u,4); /* mov.l 0x8c648380,r2 */
 goto L8c648314;
L8c648314: r[12]=0u; /* mov #0x0,r12 */
 goto L8c648316;
L8c648316: r[5]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r5 */
 goto L8c648318;
L8c648318: target=r[2];*pr=0x8c64831cu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c64831c;
L8c64831c: *sr=(*sr&~1u)|((int32_t)r[0]>=0); /* cmp/pz r0 */
 goto L8c64831e;
L8c64831e: if((*sr&1u))goto L8c64832c; /* bt 0x8c64832c */
 goto L8c648320;
L8c648320: r[0]=r[12]; /* mov r12,r0 */
 goto L8c648322;
L8c648322: wr(m,r[14]+0x9u,r[0],1); /* mov.b r0,@(0x9,r14) */
 goto L8c648324;
L8c648324: wr(m,r[14]+0x8u,r[0],1); /* mov.b r0,@(0x8,r14) */
 goto L8c648326;
L8c648326: r[0]=sx(m,0x8c648362u,2); /* mov.w 0x8c648362,r0 */
 goto L8c648328;
L8c648328: goto L8c64838c; /* bra 0x8c64838c */
 goto L8c64832c;
L8c64832c: r[2]=rd(m,0x8c648384u,4); /* mov.l 0x8c648384,r2 */
 goto L8c64832e;
L8c64832e: target=r[2];*pr=0x8c648332u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c648332;
L8c648332: *sr=(*sr&~1u)|((int32_t)r[0]>=0); /* cmp/pz r0 */
 goto L8c648334;
L8c648334: if((*sr&1u))goto L8c64833a; /* bt 0x8c64833a */
 goto L8c648336;
L8c648336: r[0]=4294967171u;goto L8c64838c; /* bra 0x8c64838c */
 goto L8c64833a;
L8c64833a: r[4]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r4 */
 goto L8c64833c;
L8c64833c: r[0]=r[11]; /* mov r11,r0 */
 goto L8c64833e;
L8c64833e: r[2]=rd(m,r[4]+0x1cu,4); /* mov.l @(0x1c,r4),r2 */
 goto L8c648340;
L8c648340: wr(m,r[4]+0x30u,r[2],4); /* mov.l r2,@(0x30,r4) */
 goto L8c648342;
L8c648342: r[4]=r[13]; /* mov r13,r4 */
 goto L8c648344;
L8c648344: wr(m,r[14]+0x9u,r[0],1); /* mov.b r0,@(0x9,r14) */
 goto L8c648346;
L8c648346: wr(m,r[14]+0x8u,r[0],1); /* mov.b r0,@(0x8,r14) */
 goto L8c648348;
L8c648348: r[0]=r[12]; /* mov r12,r0 */
 goto L8c64834a;
L8c64834a: wr(m,r[14]+0xau,r[0],1); /* mov.b r0,@(0xa,r14) */
 goto L8c64834c;
L8c64834c: wr(m,r[14]+0xbu,r[0],1); /* mov.b r0,@(0xb,r14) */
 goto L8c64834e;
L8c64834e: wr(m,r[14]+0xcu,r[0],1); /* mov.b r0,@(0xc,r14) */
 goto L8c648350;
L8c648350: r[15]+=4u; /* add #0x4,r15 */
 goto L8c648352;
L8c648352: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c648354;
L8c648354: r[3]=rd(m,0x8c648388u,4); /* mov.l 0x8c648388,r3 */
 goto L8c648356;
L8c648356: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c648358;
L8c648358: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c64835a;
L8c64835a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c64835c;
L8c64835c: target=r[3];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
L8c64838c: r[15]+=4u; /* add #0x4,r15 */
 goto L8c64838e;
L8c64838e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c648390;
L8c648390: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c648392;
L8c648392: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c648394;
L8c648394: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c648396;
L8c648396: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
