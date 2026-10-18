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
/* Retail packed audio transfer, 8c66a524..8c66a61a.
 * Copies little-endian16 samples, separating stereo buffers.
 * Native pending getter; query/submission callbacks supplied by caller. */
SA_STAGE_EXPORT int sa_sound_packed_transfer_step(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66a524;
L8c66a524: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66a526;
L8c66a526: r[14]=r[4]; /* mov r4,r14 */
 goto L8c66a528;
L8c66a528: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66a52a;
L8c66a52a: r[13]=r[4]; /* mov r4,r13 */
 goto L8c66a52c;
L8c66a52c: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66a52e;
L8c66a52e: r[13]+=80u; /* add #0x50,r13 */
 goto L8c66a530;
L8c66a530: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66a532;
L8c66a532: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66a534;
L8c66a534: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66a536;
L8c66a536: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66a538;
L8c66a538: take=!(*sr&1u);r[12]=rd(m,r[13],4);if(take)goto L8c66a5f2;goto L8c66a53c; /* bf/s 0x8c66a5f2 */
 goto L8c66a53c;
L8c66a53c: r[2]=rd(m,0x8c66a5c4u,4); /* mov.l 0x8c66a5c4,r2 */
 goto L8c66a53e;
L8c66a53e: target=r[2];*pr=0x8c66a542u;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66a542;
L8c66a542: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c66a544;
L8c66a544: if(!(*sr&1u))goto L8c66a5f2; /* bf 0x8c66a5f2 */
 goto L8c66a546;
L8c66a546: r[0]=sx(m,0x8c66a5bcu,2); /* mov.w 0x8c66a5bc,r0 */
 goto L8c66a548;
L8c66a548: r[6]=r[13]; /* mov r13,r6 */
 goto L8c66a54a;
L8c66a54a: r[5]=r[13]; /* mov r13,r5 */
 goto L8c66a54c;
L8c66a54c: r[7]=r[13]; /* mov r13,r7 */
 goto L8c66a54e;
L8c66a54e: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c66a550;
L8c66a550: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66a552;
L8c66a552: r[5]+=32u; /* add #0x20,r5 */
 goto L8c66a554;
L8c66a554: r[7]+=40u; /* add #0x28,r7 */
 goto L8c66a556;
L8c66a556: r[6]+=36u; /* add #0x24,r6 */
 goto L8c66a558;
L8c66a558: target=r[2];*pr=0x8c66a55cu;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66a55c;
L8c66a55c: r[4]=rd(m,r[13]+0x18u,4); /* mov.l @(0x18,r13),r4 */
 goto L8c66a55e;
L8c66a55e: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66a560;
L8c66a560: r[2]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r2 */
 goto L8c66a562;
L8c66a562: r[4]-=r[3]; /* sub r3,r4 */
 goto L8c66a564;
L8c66a564: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66a566;
L8c66a566: take=!(*sr&1u);r[2]=rd(m,r[13]+0x4u,4);if(take)goto L8c66a56c;goto L8c66a56a; /* bf/s 0x8c66a56c */
 goto L8c66a56a;
L8c66a56a: r[4]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r4 */
 goto L8c66a56c;
L8c66a56c: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66a56e;
L8c66a56e: take=!(*sr&1u);r[5]=rd(m,r[13]+0x14u,4);if(take)goto L8c66a574;goto L8c66a572; /* bf/s 0x8c66a574 */
 goto L8c66a572;
L8c66a572: r[4]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r4 */
 goto L8c66a574;
L8c66a574: r[11]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r11 */
 goto L8c66a576;
L8c66a576: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66a578;
L8c66a578: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1; /* shll r11 */
 goto L8c66a57a;
L8c66a57a: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a57c;
L8c66a57c: r[11]+=r[5]; /* add r5,r11 */
 goto L8c66a57e;
L8c66a57e: take=!(*sr&1u);r[7]=0u;if(take)goto L8c66a5c8;goto L8c66a582; /* bf/s 0x8c66a5c8 */
 goto L8c66a582;
L8c66a582: r[1]=rd(m,r[13]+0x1cu,4); /* mov.l @(0x1c,r13),r1 */
 goto L8c66a584;
L8c66a584: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66a586;
L8c66a586: r[13]=r[7]; /* mov r7,r13 */
 goto L8c66a588;
L8c66a588: r[1]+=r[3]; /* add r3,r1 */
 goto L8c66a58a;
L8c66a58a: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c66a58c;
L8c66a58c: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66a58e;
L8c66a58e: r[1]+=r[5]; /* add r5,r1 */
 goto L8c66a590;
L8c66a590: r[5]=r[7]; /* mov r7,r5 */
 goto L8c66a592;
L8c66a592: take=!(*sr&1u);r[6]=r[7];if(take)goto L8c66a5de;goto L8c66a596; /* bf/s 0x8c66a5de */
 goto L8c66a596;
L8c66a596: r[3]=r[5]; /* mov r5,r3 */
 goto L8c66a598;
L8c66a598: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c66a59a;
L8c66a59a: r[3]+=r[12]; /* add r12,r3 */
 goto L8c66a59c;
L8c66a59c: r[0]=r[6]; /* mov r6,r0 */
 goto L8c66a59e;
L8c66a59e: r[3]=sx(m,r[3],2); /* mov.w @r3,r3 */
 goto L8c66a5a0;
L8c66a5a0: r[13]+=1u; /* add #0x1,r13 */
 goto L8c66a5a2;
L8c66a5a2: wr(m,r[0]+r[11],r[3],2); /* mov.w r3,@(r0,r11) */
 goto L8c66a5a4;
L8c66a5a4: r[3]=r[5]; /* mov r5,r3 */
 goto L8c66a5a6;
L8c66a5a6: r[3]+=1u; /* add #0x1,r3 */
 goto L8c66a5a8;
L8c66a5a8: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c66a5aa;
L8c66a5aa: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[4]); /* cmp/ge r4,r13 */
 goto L8c66a5ac;
L8c66a5ac: r[3]+=r[12]; /* add r12,r3 */
 goto L8c66a5ae;
L8c66a5ae: r[3]=sx(m,r[3],2); /* mov.w @r3,r3 */
 goto L8c66a5b0;
L8c66a5b0: r[5]+=2u; /* add #0x2,r5 */
 goto L8c66a5b2;
L8c66a5b2: wr(m,r[0]+r[1],r[3],2); /* mov.w r3,@(r0,r1) */
 goto L8c66a5b4;
L8c66a5b4: take=!(*sr&1u);r[6]+=2u;if(take)goto L8c66a596;goto L8c66a5b8; /* bf/s 0x8c66a596 */
 goto L8c66a5b8;
L8c66a5b8: goto L8c66a5de; /* bra 0x8c66a5de */
 goto L8c66a5c8;
L8c66a5c8: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66a5ca;
L8c66a5ca: r[6]=r[7]; /* mov r7,r6 */
 goto L8c66a5cc;
L8c66a5cc: take=!(*sr&1u);r[5]=r[7];if(take)goto L8c66a5de;goto L8c66a5d0; /* bf/s 0x8c66a5de */
 goto L8c66a5d0;
L8c66a5d0: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66a5d2;
L8c66a5d2: r[3]=sx(m,r[0]+r[12],2); /* mov.w @(r0,r12),r3 */
 goto L8c66a5d4;
L8c66a5d4: r[6]+=1u; /* add #0x1,r6 */
 goto L8c66a5d6;
L8c66a5d6: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[4]); /* cmp/ge r4,r6 */
 goto L8c66a5d8;
L8c66a5d8: wr(m,r[0]+r[11],r[3],2); /* mov.w r3,@(r0,r11) */
 goto L8c66a5da;
L8c66a5da: take=!(*sr&1u);r[5]+=2u;if(take)goto L8c66a5d0;goto L8c66a5de; /* bf/s 0x8c66a5d0 */
 goto L8c66a5de;
L8c66a5de: r[0]=sx(m,0x8c66a62eu,2); /* mov.w 0x8c66a62e,r0 */
 goto L8c66a5e0;
L8c66a5e0: r[3]=2u; /* mov #0x2,r3 */
 goto L8c66a5e2;
L8c66a5e2: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66a5e4;
L8c66a5e4: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66a5e6;
L8c66a5e6: *macl=r[0]*r[4]; /* mul.l r0,r4 */
 goto L8c66a5e8;
L8c66a5e8: r[0]=sx(m,0x8c66a630u,2); /* mov.w 0x8c66a630,r0 */
 goto L8c66a5ea;
L8c66a5ea: r[4]=*macl; /* sts MACL,r4 */
 goto L8c66a5ec;
L8c66a5ec: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c66a5ee;
L8c66a5ee: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66a5f0;
L8c66a5f0: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66a5f2;
L8c66a5f2: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66a5f4;
L8c66a5f4: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a5f6;
L8c66a5f6: if(!(*sr&1u))goto L8c66a60e; /* bf 0x8c66a60e */
 goto L8c66a5f8;
L8c66a5f8: r[0]=sx(m,0x8c66a632u,2); /* mov.w 0x8c66a632,r0 */
 goto L8c66a5fa;
L8c66a5fa: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c66a5fc;
L8c66a5fc: r[0]+=16u; /* add #0x10,r0 */
 goto L8c66a5fe;
L8c66a5fe: r[6]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r6 */
 goto L8c66a600;
L8c66a600: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66a602;
L8c66a602: r[5]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r5 */
 goto L8c66a604;
L8c66a604: r[0]+=4294967280u; /* add #-0x10,r0 */
 goto L8c66a606;
L8c66a606: target=r[3];*pr=0x8c66a60au;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66a60a;
L8c66a60a: r[3]=3u; /* mov #0x3,r3 */
 goto L8c66a60c;
L8c66a60c: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66a60e;
L8c66a60e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66a610;
L8c66a610: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66a612;
L8c66a612: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66a614;
L8c66a614: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66a616;
L8c66a616: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
