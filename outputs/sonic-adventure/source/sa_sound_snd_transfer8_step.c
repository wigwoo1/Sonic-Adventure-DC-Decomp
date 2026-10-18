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
/* Retail SND sample transfer step, 8c66a13c..8c66a236.
 * Pending getter resolves to native code; query/submission are caller callbacks. */
SA_STAGE_EXPORT int sa_sound_snd_transfer8_step(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66a13c;
L8c66a13c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66a13e;
L8c66a13e: r[14]=r[4]; /* mov r4,r14 */
 goto L8c66a140;
L8c66a140: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66a142;
L8c66a142: r[13]=r[4]; /* mov r4,r13 */
 goto L8c66a144;
L8c66a144: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66a146;
L8c66a146: r[13]+=80u; /* add #0x50,r13 */
 goto L8c66a148;
L8c66a148: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66a14a;
L8c66a14a: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c66a14c;
L8c66a14c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66a14e;
L8c66a14e: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66a150;
L8c66a150: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66a152;
L8c66a152: take=!(*sr&1u);r[12]=rd(m,r[13],4);if(take)goto L8c66a20c;goto L8c66a156; /* bf/s 0x8c66a20c */
 goto L8c66a156;
L8c66a156: r[2]=rd(m,0x8c66a240u,4); /* mov.l 0x8c66a240,r2 */
 goto L8c66a158;
L8c66a158: target=r[2];*pr=0x8c66a15cu;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66a15c;
L8c66a15c: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c66a15e;
L8c66a15e: if(!(*sr&1u))goto L8c66a20c; /* bf 0x8c66a20c */
 goto L8c66a160;
L8c66a160: r[0]=sx(m,0x8c66a236u,2); /* mov.w 0x8c66a236,r0 */
 goto L8c66a162;
L8c66a162: r[6]=r[13]; /* mov r13,r6 */
 goto L8c66a164;
L8c66a164: r[5]=r[13]; /* mov r13,r5 */
 goto L8c66a166;
L8c66a166: r[7]=r[13]; /* mov r13,r7 */
 goto L8c66a168;
L8c66a168: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c66a16a;
L8c66a16a: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66a16c;
L8c66a16c: r[5]+=32u; /* add #0x20,r5 */
 goto L8c66a16e;
L8c66a16e: r[7]+=40u; /* add #0x28,r7 */
 goto L8c66a170;
L8c66a170: r[6]+=36u; /* add #0x24,r6 */
 goto L8c66a172;
L8c66a172: target=r[2];*pr=0x8c66a176u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66a176;
L8c66a176: r[4]=rd(m,r[13]+0x18u,4); /* mov.l @(0x18,r13),r4 */
 goto L8c66a178;
L8c66a178: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66a17a;
L8c66a17a: r[2]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r2 */
 goto L8c66a17c;
L8c66a17c: r[4]-=r[3]; /* sub r3,r4 */
 goto L8c66a17e;
L8c66a17e: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66a180;
L8c66a180: take=!(*sr&1u);r[2]=rd(m,r[13]+0x4u,4);if(take)goto L8c66a186;goto L8c66a184; /* bf/s 0x8c66a186 */
 goto L8c66a184;
L8c66a184: r[4]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r4 */
 goto L8c66a186;
L8c66a186: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66a188;
L8c66a188: take=!(*sr&1u);r[5]=rd(m,r[13]+0x14u,4);if(take)goto L8c66a18e;goto L8c66a18c; /* bf/s 0x8c66a18e */
 goto L8c66a18c;
L8c66a18c: r[4]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r4 */
 goto L8c66a18e;
L8c66a18e: r[10]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r10 */
 goto L8c66a190;
L8c66a190: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66a192;
L8c66a192: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1; /* shll r10 */
 goto L8c66a194;
L8c66a194: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a196;
L8c66a196: r[10]+=r[5]; /* add r5,r10 */
 goto L8c66a198;
L8c66a198: take=!(*sr&1u);r[11]=0u;if(take)goto L8c66a1dc;goto L8c66a19c; /* bf/s 0x8c66a1dc */
 goto L8c66a19c;
L8c66a19c: r[1]=rd(m,r[13]+0x1cu,4); /* mov.l @(0x1c,r13),r1 */
 goto L8c66a19e;
L8c66a19e: r[7]=r[11]; /* mov r11,r7 */
 goto L8c66a1a0;
L8c66a1a0: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66a1a2;
L8c66a1a2: r[1]+=r[3]; /* add r3,r1 */
 goto L8c66a1a4;
L8c66a1a4: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c66a1a6;
L8c66a1a6: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66a1a8;
L8c66a1a8: r[1]+=r[5]; /* add r5,r1 */
 goto L8c66a1aa;
L8c66a1aa: r[5]=r[11]; /* mov r11,r5 */
 goto L8c66a1ac;
L8c66a1ac: take=!(*sr&1u);r[6]=r[11];if(take)goto L8c66a1fa;goto L8c66a1b0; /* bf/s 0x8c66a1fa */
 goto L8c66a1b0;
L8c66a1b0: r[0]=r[12]; /* mov r12,r0 */
 goto L8c66a1b2;
L8c66a1b2: r[2]=sx(m,r[5]+r[0],1); /* mov.b @(r0,r5),r2 */
 goto L8c66a1b4;
L8c66a1b4: r[3]=r[10]; /* mov r10,r3 */
 goto L8c66a1b6;
L8c66a1b6: r[3]+=r[6]; /* add r6,r3 */
 goto L8c66a1b8;
L8c66a1b8: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c66a1ba;
L8c66a1ba: r[0]=r[6]; /* mov r6,r0 */
 goto L8c66a1bc;
L8c66a1bc: r[2]<<=8; /* shll8 r2 */
 goto L8c66a1be;
L8c66a1be: wr(m,r[3],r[2],2); /* mov.w r2,@r3 */
 goto L8c66a1c0;
L8c66a1c0: r[3]=r[5]; /* mov r5,r3 */
 goto L8c66a1c2;
L8c66a1c2: r[3]+=1u; /* add #0x1,r3 */
 goto L8c66a1c4;
L8c66a1c4: r[3]+=r[12]; /* add r12,r3 */
 goto L8c66a1c6;
L8c66a1c6: r[3]=sx(m,r[3],1); /* mov.b @r3,r3 */
 goto L8c66a1c8;
L8c66a1c8: r[7]+=1u; /* add #0x1,r7 */
 goto L8c66a1ca;
L8c66a1ca: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[4]); /* cmp/ge r4,r7 */
 goto L8c66a1cc;
L8c66a1cc: r[5]+=2u; /* add #0x2,r5 */
 goto L8c66a1ce;
L8c66a1ce: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c66a1d0;
L8c66a1d0: r[3]<<=8; /* shll8 r3 */
 goto L8c66a1d2;
L8c66a1d2: wr(m,r[0]+r[1],r[3],2); /* mov.w r3,@(r0,r1) */
 goto L8c66a1d4;
L8c66a1d4: take=!(*sr&1u);r[6]+=2u;if(take)goto L8c66a1b0;goto L8c66a1d8; /* bf/s 0x8c66a1b0 */
 goto L8c66a1d8;
L8c66a1d8: goto L8c66a1fa; /* bra 0x8c66a1fa */
 goto L8c66a1dc;
L8c66a1dc: r[5]=0u; /* mov #0x0,r5 */
 goto L8c66a1de;
L8c66a1de: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66a1e0;
L8c66a1e0: r[6]=0u; /* mov #0x0,r6 */
 goto L8c66a1e2;
L8c66a1e2: r[7]=r[11]; /* mov r11,r7 */
 goto L8c66a1e4;
L8c66a1e4: r[5]+=r[12]; /* add r12,r5 */
 goto L8c66a1e6;
L8c66a1e6: take=!(*sr&1u);r[6]+=r[10];if(take)goto L8c66a1fa;goto L8c66a1ea; /* bf/s 0x8c66a1fa */
 goto L8c66a1ea;
L8c66a1ea: r[3]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r3 */
 goto L8c66a1ec;
L8c66a1ec: r[7]+=1u; /* add #0x1,r7 */
 goto L8c66a1ee;
L8c66a1ee: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[4]); /* cmp/ge r4,r7 */
 goto L8c66a1f0;
L8c66a1f0: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c66a1f2;
L8c66a1f2: r[3]<<=8; /* shll8 r3 */
 goto L8c66a1f4;
L8c66a1f4: wr(m,r[6],r[3],2); /* mov.w r3,@r6 */
 goto L8c66a1f6;
L8c66a1f6: take=!(*sr&1u);r[6]+=2u;if(take)goto L8c66a1ea;goto L8c66a1fa; /* bf/s 0x8c66a1ea */
 goto L8c66a1fa;
L8c66a1fa: r[0]=sx(m,0x8c66a238u,2); /* mov.w 0x8c66a238,r0 */
 goto L8c66a1fc;
L8c66a1fc: r[3]=2u; /* mov #0x2,r3 */
 goto L8c66a1fe;
L8c66a1fe: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66a200;
L8c66a200: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66a202;
L8c66a202: *macl=r[0]*r[4]; /* mul.l r0,r4 */
 goto L8c66a204;
L8c66a204: r[0]=sx(m,0x8c66a23au,2); /* mov.w 0x8c66a23a,r0 */
 goto L8c66a206;
L8c66a206: r[4]=*macl; /* sts MACL,r4 */
 goto L8c66a208;
L8c66a208: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66a20a;
L8c66a20a: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66a20c;
L8c66a20c: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66a20e;
L8c66a20e: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a210;
L8c66a210: if(!(*sr&1u))goto L8c66a228; /* bf 0x8c66a228 */
 goto L8c66a212;
L8c66a212: r[0]=sx(m,0x8c66a23cu,2); /* mov.w 0x8c66a23c,r0 */
 goto L8c66a214;
L8c66a214: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c66a216;
L8c66a216: r[0]+=16u; /* add #0x10,r0 */
 goto L8c66a218;
L8c66a218: r[6]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r6 */
 goto L8c66a21a;
L8c66a21a: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66a21c;
L8c66a21c: r[5]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r5 */
 goto L8c66a21e;
L8c66a21e: r[0]+=4294967280u; /* add #-0x10,r0 */
 goto L8c66a220;
L8c66a220: target=r[3];*pr=0x8c66a224u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66a224;
L8c66a224: r[3]=3u; /* mov #0x3,r3 */
 goto L8c66a226;
L8c66a226: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66a228;
L8c66a228: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66a22a;
L8c66a22a: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c66a22c;
L8c66a22c: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66a22e;
L8c66a22e: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66a230;
L8c66a230: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66a232;
L8c66a232: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
