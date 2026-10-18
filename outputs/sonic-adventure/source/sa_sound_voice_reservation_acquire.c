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
/* Static C recovery of retail Tikal helper 8c095402..8c09550e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_voice_reservation_acquire(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c095402;
L8c095402: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c095404;
L8c095404: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c095406;
L8c095406: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c095408;
L8c095408: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c09540a;
L8c09540a: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c09540c;
L8c09540c: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c09540e;
L8c09540e: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c095410;
L8c095410: r[13]=52u; /* mov #0x34,r13 */
 goto L8c095412;
L8c095412: r[3]=rd(m,0x8c095478u,4); /* mov.l 0x8c095478,r3 */
 goto L8c095414;
L8c095414: *macl=r[13]*r[4]; /* mul.l r13,r4 */
 goto L8c095416;
L8c095416: r[14]=4294967295u; /* mov #-0x1,r14 */
 goto L8c095418;
L8c095418: r[12]=rd(m,0x8c095470u,4); /* mov.l 0x8c095470,r12 */
 goto L8c09541a;
L8c09541a: r[4]=35u; /* mov #0x23,r4 */
 goto L8c09541c;
L8c09541c: r[7]=r[14]; /* mov r14,r7 */
 goto L8c09541e;
L8c09541e: r[5]=0u; /* mov #0x0,r5 */
 goto L8c095420;
L8c095420: r[6]=r[14]; /* mov r14,r6 */
 goto L8c095422;
L8c095422: r[13]=*macl; /* sts MACL,r13 */
 goto L8c095424;
L8c095424: r[13]+=r[3]; /* add r3,r13 */
 goto L8c095426;
L8c095426: r[0]=r[5]; /* mov r5,r0 */
 goto L8c095428;
L8c095428: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c09542a;
L8c09542a: r[2]=rd(m,r[13]+0x10u,4); /* mov.l @(0x10,r13),r2 */
 goto L8c09542c;
L8c09542c: r[3]=sx(m,r[0]+r[12],2); /* mov.w @(r0,r12),r3 */
 goto L8c09542e;
L8c09542e: *sr=(*sr&~1u)|(r[3]==r[2]); /* cmp/eq r3,r2 */
 goto L8c095430;
L8c095430: if(!(*sr&1u))goto L8c095480; /* bf 0x8c095480 */
 goto L8c095432;
L8c095432: r[7]=r[5];goto L8c095486; /* bra 0x8c095486 */
 goto L8c095480;
L8c095480: r[5]+=1u; /* add #0x1,r5 */
 goto L8c095482;
L8c095482: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[4]); /* cmp/ge r4,r5 */
 goto L8c095484;
L8c095484: if(!(*sr&1u))goto L8c095426; /* bf 0x8c095426 */
 goto L8c095486;
L8c095486: *sr=(*sr&~1u)|((int32_t)r[7]>=0); /* cmp/pz r7 */
 goto L8c095488;
L8c095488: if(!(*sr&1u))goto L8c0954c4; /* bf 0x8c0954c4 */
 goto L8c09548a;
L8c09548a: r[11]=r[7]; /* mov r7,r11 */
 goto L8c09548c;
L8c09548c: r[11]<<=2; /* shll2 r11 */
 goto L8c09548e;
L8c09548e: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1; /* shll r11 */
 goto L8c095490;
L8c095490: r[10]=rd(m,0x8c0955a0u,4); /* mov.l 0x8c0955a0,r10 */
 goto L8c095492;
L8c095492: r[9]=r[7]; /* mov r7,r9 */
 goto L8c095494;
L8c095494: r[5]=r[11]; /* mov r11,r5 */
 goto L8c095496;
L8c095496: r[3]=rd(m,0x8c0955a4u,4); /* mov.l 0x8c0955a4,r3 */
 goto L8c095498;
L8c095498: r[9]<<=2; /* shll2 r9 */
 goto L8c09549a;
L8c09549a: r[5]+=r[10]; /* add r10,r5 */
 goto L8c09549c;
L8c09549c: r[9]<<=2; /* shll2 r9 */
 goto L8c09549e;
L8c09549e: r[9]+=r[3]; /* add r3,r9 */
 goto L8c0954a0;
L8c0954a0: wr(m,r[15],r[9],4); /* mov.l r9,@r15 */
 goto L8c0954a2;
L8c0954a2: r[2]=sx(m,r[5],2); /* mov.w @r5,r2 */
 goto L8c0954a4;
L8c0954a4: *sr=(*sr&~1u)|((int32_t)r[2]>0); /* cmp/pl r2 */
 goto L8c0954a6;
L8c0954a6: take=!(*sr&1u);r[4]=0u;if(take)goto L8c0954c0;goto L8c0954aa; /* bf/s 0x8c0954c0 */
 goto L8c0954aa;
L8c0954aa: r[0]=r[4]; /* mov r4,r0 */
 goto L8c0954ac;
L8c0954ac: r[0]<<=2; /* shll2 r0 */
 goto L8c0954ae;
L8c0954ae: r[3]=rd(m,r[9]+r[0],4); /* mov.l @(r0,r9),r3 */
 goto L8c0954b0;
L8c0954b0: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c0954b2;
L8c0954b2: if(!(*sr&1u))goto L8c0954b8; /* bf 0x8c0954b8 */
 goto L8c0954b4;
L8c0954b4: r[6]=r[4];goto L8c0954c0; /* bra 0x8c0954c0 */
 goto L8c0954b8;
L8c0954b8: r[2]=sx(m,r[5],2); /* mov.w @r5,r2 */
 goto L8c0954ba;
L8c0954ba: r[4]+=1u; /* add #0x1,r4 */
 goto L8c0954bc;
L8c0954bc: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]); /* cmp/ge r2,r4 */
 goto L8c0954be;
L8c0954be: if(!(*sr&1u))goto L8c0954aa; /* bf 0x8c0954aa */
 goto L8c0954c0;
L8c0954c0: *sr=(*sr&~1u)|((int32_t)r[6]>=0); /* cmp/pz r6 */
 goto L8c0954c2;
L8c0954c2: if((*sr&1u))goto L8c0954ca; /* bt 0x8c0954ca */
 goto L8c0954c4;
L8c0954c4: wr(m,r[13]+0x30u,r[14],4); /* mov.l r14,@(0x30,r13) */
 goto L8c0954c6;
L8c0954c6: r[0]=r[14];goto L8c0954fe; /* bra 0x8c0954fe */
 goto L8c0954ca;
L8c0954ca: r[2]=rd(m,r[13]+0x8u,4); /* mov.l @(0x8,r13),r2 */
 goto L8c0954cc;
L8c0954cc: r[3]=r[6]; /* mov r6,r3 */
 goto L8c0954ce;
L8c0954ce: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c0954d0;
L8c0954d0: r[4]=r[11]; /* mov r11,r4 */
 goto L8c0954d2;
L8c0954d2: r[2]+=1u; /* add #0x1,r2 */
 goto L8c0954d4;
L8c0954d4: r[3]<<=2; /* shll2 r3 */
 goto L8c0954d6;
L8c0954d6: r[4]+=r[10]; /* add r10,r4 */
 goto L8c0954d8;
L8c0954d8: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c0954da;
L8c0954da: r[0]=sx(m,r[4]+0x2u,2); /* mov.w @(0x2,r4),r0 */
 goto L8c0954dc;
L8c0954dc: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c0954de;
L8c0954de: r[4]=r[0]; /* mov r0,r4 */
 goto L8c0954e0;
L8c0954e0: r[4]+=r[6]; /* add r6,r4 */
 goto L8c0954e2;
L8c0954e2: r[4]+=4294967295u; /* add #-0x1,r4 */
 goto L8c0954e4;
L8c0954e4: take=!(*sr&1u);wr(m,r[13]+0x30u,r[4],4);if(take)goto L8c0954ec;goto L8c0954e8; /* bf/s 0x8c0954ec */
 goto L8c0954e8;
L8c0954e8: r[0]=0u;goto L8c0954fe; /* bra 0x8c0954fe */
 goto L8c0954ec;
L8c0954ec: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1; /* shll r7 */
 goto L8c0954ee;
L8c0954ee: r[0]=r[11]; /* mov r11,r0 */
 goto L8c0954f0;
L8c0954f0: r[7]+=r[12]; /* add r12,r7 */
 goto L8c0954f2;
L8c0954f2: r[0]+=r[10]; /* add r10,r0 */
 goto L8c0954f4;
L8c0954f4: r[3]=sx(m,r[7],2); /* mov.w @r7,r3 */
 goto L8c0954f6;
L8c0954f6: r[0]=sx(m,r[0]+0x6u,2); /* mov.w @(0x6,r0),r0 */
 goto L8c0954f8;
L8c0954f8: r[0]-=r[3]; /* sub r3,r0 */
 goto L8c0954fa;
L8c0954fa: r[0]+=r[6]; /* add r6,r0 */
 goto L8c0954fc;
L8c0954fc: r[0]+=4294967295u; /* add #-0x1,r0 */
 goto L8c0954fe;
L8c0954fe: r[15]+=4u; /* add #0x4,r15 */
 goto L8c095500;
L8c095500: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c095502;
L8c095502: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c095504;
L8c095504: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c095506;
L8c095506: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c095508;
L8c095508: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c09550a;
L8c09550a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
