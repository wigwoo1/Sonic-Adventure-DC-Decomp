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
/* Static C recovery of retail Tikal helper 8c64839a..8c648470.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
static int sa_input_press_service_core(uint32_t entry,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 switch(entry){case 0:goto L8c64839a;case 0x8c6483f6u:goto L8c6483f6;case 0x8c648400u:goto L8c648400;case 0x8c648412u:goto L8c648412;case 0x8c648426u:goto L8c648426;default:return -4;}
L8c64839a: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c64839c;
L8c64839c: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c64839e;
L8c64839e: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6483a0;
L8c6483a0: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6483a2;
L8c6483a2: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c6483a4;
L8c6483a4: r[14]=sx(m,0x8c648490u,2); /* mov.w 0x8c648490,r14 */
 goto L8c6483a6;
L8c6483a6: r[13]=r[4]; /* mov r4,r13 */
 goto L8c6483a8;
L8c6483a8: r[2]=76u; /* mov #0x4c,r2 */
 goto L8c6483aa;
L8c6483aa: r[1]=rd(m,0x8c648498u,4); /* mov.l 0x8c648498,r1 */
 goto L8c6483ac;
L8c6483ac: *macl=r[14]*r[13]; /* mul.l r14,r13 */
 goto L8c6483ae;
L8c6483ae: r[3]=rd(m,0x8c648494u,4); /* mov.l 0x8c648494,r3 */
 goto L8c6483b0;
L8c6483b0: r[0]=1u; /* mov #0x1,r0 */
 goto L8c6483b2;
L8c6483b2: r[14]=*macl; /* sts MACL,r14 */
 goto L8c6483b4;
L8c6483b4: *macl=r[2]*r[13]; /* mul.l r2,r13 */
 goto L8c6483b6;
L8c6483b6: r[14]+=r[3]; /* add r3,r14 */
 goto L8c6483b8;
L8c6483b8: r[3]=r[13]; /* mov r13,r3 */
 goto L8c6483ba;
L8c6483ba: r[3]<<=2; /* shll2 r3 */
 goto L8c6483bc;
L8c6483bc: r[2]=*macl; /* sts MACL,r2 */
 goto L8c6483be;
L8c6483be: r[3]<<=2; /* shll2 r3 */
 goto L8c6483c0;
L8c6483c0: r[3]<<=2; /* shll2 r3 */
 goto L8c6483c2;
L8c6483c2: r[2]+=r[1]; /* add r1,r2 */
 goto L8c6483c4;
L8c6483c4: wr(m,r[14]+0x1cu,r[2],4); /* mov.l r2,@(0x1c,r14) */
 goto L8c6483c6;
L8c6483c6: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c6483c8;
L8c6483c8: wr(m,r[14]+0x30u,r[0],4); /* mov.l r0,@(0x30,r14) */
 goto L8c6483ca;
L8c6483ca: r[2]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r2 */
 goto L8c6483cc;
L8c6483cc: r[0]=r[2]; /* mov r2,r0 */
 goto L8c6483ce;
L8c6483ce: r[2]<<=2; /* shll2 r2 */
 goto L8c6483d0;
L8c6483d0: r[2]+=r[0]; /* add r0,r2 */
 goto L8c6483d2;
L8c6483d2: r[0]=rd(m,0x8c64849cu,4); /* mov.l 0x8c64849c,r0 */
 goto L8c6483d4;
L8c6483d4: r[2]<<=2; /* shll2 r2 */
 goto L8c6483d6;
L8c6483d6: r[2]<<=2; /* shll2 r2 */
 goto L8c6483d8;
L8c6483d8: r[2]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r2 */
 goto L8c6483da;
L8c6483da: wr(m,r[14]+0x34u,r[2],4); /* mov.l r2,@(0x34,r14) */
 goto L8c6483dc;
L8c6483dc: r[2]=rd(m,0x8c6484a0u,4); /* mov.l 0x8c6484a0,r2 */
 goto L8c6483de;
L8c6483de: r[3]+=r[2]; /* add r2,r3 */
 goto L8c6483e0;
L8c6483e0: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c6483e2;
L8c6483e2: r[7]=rd(m,r[14]+0x34u,4); /* mov.l @(0x34,r14),r7 */
 goto L8c6483e4;
L8c6483e4: r[6]=rd(m,r[14]+0x30u,4); /* mov.l @(0x30,r14),r6 */
 goto L8c6483e6;
L8c6483e6: r[3]=rd(m,0x8c6484a4u,4); /* mov.l 0x8c6484a4,r3 */
 goto L8c6483e8;
L8c6483e8: r[6]+=1u; /* add #0x1,r6 */
 goto L8c6483ea;
L8c6483ea: wr(m,r[14]+0x30u,r[6],4); /* mov.l r6,@(0x30,r14) */
 goto L8c6483ec;
L8c6483ec: r[6]+=4294967295u; /* add #-0x1,r6 */
 goto L8c6483ee;
L8c6483ee: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c6483f0;
L8c6483f0: r[5]=rd(m,r[5]+0x8u,4); /* mov.l @(0x8,r5),r5 */
 goto L8c6483f2;
L8c6483f2: target=r[3];*pr=0x8c6483f6u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6483f6;
L8c6483f6: r[2]=rd(m,0x8c6484a8u,4); /* mov.l 0x8c6484a8,r2 */
 goto L8c6483f8;
L8c6483f8: r[12]=0u; /* mov #0x0,r12 */
 goto L8c6483fa;
L8c6483fa: r[5]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r5 */
 goto L8c6483fc;
L8c6483fc: target=r[2];*pr=0x8c648400u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c648400;
L8c648400: *sr=(*sr&~1u)|((int32_t)r[0]>=0); /* cmp/pz r0 */
 goto L8c648402;
L8c648402: if((*sr&1u))goto L8c64840c; /* bt 0x8c64840c */
 goto L8c648404;
L8c648404: r[0]=r[12]; /* mov r12,r0 */
 goto L8c648406;
L8c648406: wr(m,r[14]+0x9u,r[0],1); /* mov.b r0,@(0x9,r14) */
 goto L8c648408;
L8c648408: wr(m,r[14]+0x8u,r[0],1);goto L8c64842a; /* bra 0x8c64842a */
 goto L8c64840c;
L8c64840c: r[3]=rd(m,0x8c6484acu,4); /* mov.l 0x8c6484ac,r3 */
 goto L8c64840e;
L8c64840e: target=r[3];*pr=0x8c648412u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c648412;
L8c648412: *sr=(*sr&~1u)|((int32_t)r[0]>=0); /* cmp/pz r0 */
 goto L8c648414;
L8c648414: if(!(*sr&1u))goto L8c648462; /* bf 0x8c648462 */
 goto L8c648416;
L8c648416: r[4]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r4 */
 goto L8c648418;
L8c648418: r[5]=0u; /* mov #0x0,r5 */
 goto L8c64841a;
L8c64841a: r[6]=rd(m,0x8c6484b0u,4); /* mov.l 0x8c6484b0,r6 */
 goto L8c64841c;
L8c64841c: r[2]=rd(m,r[4]+0x1cu,4); /* mov.l @(0x1c,r4),r2 */
 goto L8c64841e;
L8c64841e: r[3]=rd(m,0x8c6484b4u,4); /* mov.l 0x8c6484b4,r3 */
 goto L8c648420;
L8c648420: wr(m,r[4]+0x30u,r[2],4); /* mov.l r2,@(0x30,r4) */
 goto L8c648422;
L8c648422: target=r[3];*pr=0x8c648426u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c648426;
L8c648426: *sr=(*sr&~1u)|((int32_t)r[0]>=0); /* cmp/pz r0 */
 goto L8c648428;
L8c648428: if((*sr&1u))goto L8c648430; /* bt 0x8c648430 */
 goto L8c64842a;
L8c64842a: r[0]=sx(m,0x8c648492u,2); /* mov.w 0x8c648492,r0 */
 goto L8c64842c;
L8c64842c: goto L8c648464; /* bra 0x8c648464 */
 goto L8c648430;
L8c648430: r[0]=1u; /* mov #0x1,r0 */
 goto L8c648432;
L8c648432: wr(m,r[14]+0x9u,r[0],1); /* mov.b r0,@(0x9,r14) */
 goto L8c648434;
L8c648434: r[0]=r[12]; /* mov r12,r0 */
 goto L8c648436;
L8c648436: wr(m,r[14]+0xau,r[0],1); /* mov.b r0,@(0xa,r14) */
 goto L8c648438;
L8c648438: wr(m,r[14]+0xbu,r[0],1); /* mov.b r0,@(0xb,r14) */
 goto L8c64843a;
L8c64843a: wr(m,r[14]+0xcu,r[0],1); /* mov.b r0,@(0xc,r14) */
 goto L8c64843c;
L8c64843c: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c64843e;
L8c64843e: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c648440;
L8c648440: if((*sr&1u))goto L8c648452; /* bt 0x8c648452 */
 goto L8c648442;
L8c648442: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c648444;
L8c648444: if((*sr&1u))goto L8c648456; /* bt 0x8c648456 */
 goto L8c648446;
L8c648446: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c648448;
L8c648448: if((*sr&1u))goto L8c64845a; /* bt 0x8c64845a */
 goto L8c64844a;
L8c64844a: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c64844c;
L8c64844c: if((*sr&1u))goto L8c64845e; /* bt 0x8c64845e */
 goto L8c64844e;
L8c64844e: goto L8c648462; /* bra 0x8c648462 */
 goto L8c648452;
L8c648452: r[0]=15u;goto L8c648464; /* bra 0x8c648464 */
 goto L8c648456;
L8c648456: r[0]=33u;goto L8c648464; /* bra 0x8c648464 */
 goto L8c64845a;
L8c64845a: r[0]=65u;goto L8c648464; /* bra 0x8c648464 */
 goto L8c64845e;
L8c64845e: r[0]=122u;goto L8c648464; /* bra 0x8c648464 */
 goto L8c648462;
L8c648462: r[0]=4294967171u; /* mov #-0x7d,r0 */
 goto L8c648464;
L8c648464: r[15]+=4u; /* add #0x4,r15 */
 goto L8c648466;
L8c648466: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c648468;
L8c648468: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c64846a;
L8c64846a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c64846c;
L8c64846c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_input_press_service(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_press_service_core(0,m,f,sr,r,pr,fpul,macl,ctx,call); }

SA_STAGE_EXPORT int sa_input_press_service_resume(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_press_service_core(*pr,m,f,sr,r,pr,fpul,macl,ctx,call); }
