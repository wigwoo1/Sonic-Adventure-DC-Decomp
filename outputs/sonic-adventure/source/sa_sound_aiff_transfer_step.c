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
/* Static C recovery of retail Tikal helper 8c6699d4..8c669af2.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_aiff_transfer_step(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6699d4;
L8c6699d4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6699d6;
L8c6699d6: r[14]=r[4]; /* mov r4,r14 */
 goto L8c6699d8;
L8c6699d8: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6699da;
L8c6699da: r[13]=r[4]; /* mov r4,r13 */
 goto L8c6699dc;
L8c6699dc: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6699de;
L8c6699de: r[13]+=80u; /* add #0x50,r13 */
 goto L8c6699e0;
L8c6699e0: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c6699e2;
L8c6699e2: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c6699e4;
L8c6699e4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6699e6;
L8c6699e6: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c6699e8;
L8c6699e8: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c6699ea;
L8c6699ea: take=!(*sr&1u);r[12]=rd(m,r[13],4);if(take)goto L8c669ac8;goto L8c6699ee; /* bf/s 0x8c669ac8 */
 goto L8c6699ee;
L8c6699ee: r[2]=rd(m,0x8c669a8cu,4); /* mov.l 0x8c669a8c,r2 */
 goto L8c6699f0;
L8c6699f0: target=r[2];*pr=0x8c6699f4u;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6699f4;
L8c6699f4: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c6699f6;
L8c6699f6: if(!(*sr&1u))goto L8c669ac8; /* bf 0x8c669ac8 */
 goto L8c6699f8;
L8c6699f8: r[0]=sx(m,0x8c669a86u,2); /* mov.w 0x8c669a86,r0 */
 goto L8c6699fa;
L8c6699fa: r[6]=r[13]; /* mov r13,r6 */
 goto L8c6699fc;
L8c6699fc: r[5]=r[13]; /* mov r13,r5 */
 goto L8c6699fe;
L8c6699fe: r[7]=r[13]; /* mov r13,r7 */
 goto L8c669a00;
L8c669a00: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c669a02;
L8c669a02: r[0]+=4u; /* add #0x4,r0 */
 goto L8c669a04;
L8c669a04: r[5]+=32u; /* add #0x20,r5 */
 goto L8c669a06;
L8c669a06: r[7]+=40u; /* add #0x28,r7 */
 goto L8c669a08;
L8c669a08: r[6]+=36u; /* add #0x24,r6 */
 goto L8c669a0a;
L8c669a0a: target=r[2];*pr=0x8c669a0eu;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c669a0e;
L8c669a0e: r[4]=rd(m,r[13]+0x18u,4); /* mov.l @(0x18,r13),r4 */
 goto L8c669a10;
L8c669a10: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c669a12;
L8c669a12: r[2]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r2 */
 goto L8c669a14;
L8c669a14: r[4]-=r[3]; /* sub r3,r4 */
 goto L8c669a16;
L8c669a16: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c669a18;
L8c669a18: take=!(*sr&1u);r[2]=rd(m,r[13]+0x4u,4);if(take)goto L8c669a1e;goto L8c669a1c; /* bf/s 0x8c669a1e */
 goto L8c669a1c;
L8c669a1c: r[4]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r4 */
 goto L8c669a1e;
L8c669a1e: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c669a20;
L8c669a20: take=!(*sr&1u);r[5]=rd(m,r[13]+0x14u,4);if(take)goto L8c669a26;goto L8c669a24; /* bf/s 0x8c669a26 */
 goto L8c669a24;
L8c669a24: r[4]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r4 */
 goto L8c669a26;
L8c669a26: r[11]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r11 */
 goto L8c669a28;
L8c669a28: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c669a2a;
L8c669a2a: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1; /* shll r11 */
 goto L8c669a2c;
L8c669a2c: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c669a2e;
L8c669a2e: r[11]+=r[5]; /* add r5,r11 */
 goto L8c669a30;
L8c669a30: take=!(*sr&1u);r[1]=0u;if(take)goto L8c669a90;goto L8c669a34; /* bf/s 0x8c669a90 */
 goto L8c669a34;
L8c669a34: r[9]=rd(m,r[13]+0x1cu,4); /* mov.l @(0x1c,r13),r9 */
 goto L8c669a36;
L8c669a36: r[7]=r[1]; /* mov r1,r7 */
 goto L8c669a38;
L8c669a38: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c669a3a;
L8c669a3a: r[9]+=r[3]; /* add r3,r9 */
 goto L8c669a3c;
L8c669a3c: *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1; /* shll r9 */
 goto L8c669a3e;
L8c669a3e: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c669a40;
L8c669a40: r[9]+=r[5]; /* add r5,r9 */
 goto L8c669a42;
L8c669a42: r[5]=r[1]; /* mov r1,r5 */
 goto L8c669a44;
L8c669a44: take=!(*sr&1u);r[6]=r[1];if(take)goto L8c669ab4;goto L8c669a48; /* bf/s 0x8c669ab4 */
 goto L8c669a48;
L8c669a48: r[0]=r[6]; /* mov r6,r0 */
 goto L8c669a4a;
L8c669a4a: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c669a4c;
L8c669a4c: r[13]=sx(m,r[0]+r[12],2); /* mov.w @(r0,r12),r13 */
 goto L8c669a4e;
L8c669a4e: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c669a50;
L8c669a50: r[0]=r[5]; /* mov r5,r0 */
 goto L8c669a52;
L8c669a52: r[7]+=1u; /* add #0x1,r7 */
 goto L8c669a54;
L8c669a54: r[13]=r[13]&65535u; /* extu.w r13,r13 */
 goto L8c669a56;
L8c669a56: r[3]=r[13]; /* mov r13,r3 */
 goto L8c669a58;
L8c669a58: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c669a5a;
L8c669a5a: r[13]<<=8; /* shll8 r13 */
 goto L8c669a5c;
L8c669a5c: r[13]|=r[3]; /* or r3,r13 */
 goto L8c669a5e;
L8c669a5e: wr(m,r[0]+r[11],r[13],2); /* mov.w r13,@(r0,r11) */
 goto L8c669a60;
L8c669a60: r[0]=r[6]; /* mov r6,r0 */
 goto L8c669a62;
L8c669a62: r[0]+=1u; /* add #0x1,r0 */
 goto L8c669a64;
L8c669a64: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c669a66;
L8c669a66: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[4]); /* cmp/ge r4,r7 */
 goto L8c669a68;
L8c669a68: r[13]=sx(m,r[0]+r[12],2); /* mov.w @(r0,r12),r13 */
 goto L8c669a6a;
L8c669a6a: r[0]=r[5]; /* mov r5,r0 */
 goto L8c669a6c;
L8c669a6c: r[5]+=2u; /* add #0x2,r5 */
 goto L8c669a6e;
L8c669a6e: r[13]=r[13]&65535u; /* extu.w r13,r13 */
 goto L8c669a70;
L8c669a70: r[1]=r[13]; /* mov r13,r1 */
 goto L8c669a72;
L8c669a72: r[3]=r[1]; /* mov r1,r3 */
 goto L8c669a74;
L8c669a74: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c669a76;
L8c669a76: r[13]<<=8; /* shll8 r13 */
 goto L8c669a78;
L8c669a78: r[13]|=r[3]; /* or r3,r13 */
 goto L8c669a7a;
L8c669a7a: wr(m,r[0]+r[9],r[13],2); /* mov.w r13,@(r0,r9) */
 goto L8c669a7c;
L8c669a7c: take=!(*sr&1u);r[6]+=2u;if(take)goto L8c669a48;goto L8c669a80; /* bf/s 0x8c669a48 */
 goto L8c669a80;
L8c669a80: goto L8c669ab4; /* bra 0x8c669ab4 */
 goto L8c669a90;
L8c669a90: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c669a92;
L8c669a92: r[6]=r[1]; /* mov r1,r6 */
 goto L8c669a94;
L8c669a94: take=!(*sr&1u);r[5]=r[1];if(take)goto L8c669ab4;goto L8c669a98; /* bf/s 0x8c669ab4 */
 goto L8c669a98;
L8c669a98: r[0]=r[5]; /* mov r5,r0 */
 goto L8c669a9a;
L8c669a9a: r[13]=sx(m,r[0]+r[12],2); /* mov.w @(r0,r12),r13 */
 goto L8c669a9c;
L8c669a9c: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c669a9e;
L8c669a9e: r[13]=r[13]&65535u; /* extu.w r13,r13 */
 goto L8c669aa0;
L8c669aa0: r[7]=r[13]; /* mov r13,r7 */
 goto L8c669aa2;
L8c669aa2: r[3]=r[7]; /* mov r7,r3 */
 goto L8c669aa4;
L8c669aa4: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c669aa6;
L8c669aa6: r[13]<<=8; /* shll8 r13 */
 goto L8c669aa8;
L8c669aa8: r[6]+=1u; /* add #0x1,r6 */
 goto L8c669aaa;
L8c669aaa: r[13]|=r[3]; /* or r3,r13 */
 goto L8c669aac;
L8c669aac: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[4]); /* cmp/ge r4,r6 */
 goto L8c669aae;
L8c669aae: wr(m,r[0]+r[11],r[13],2); /* mov.w r13,@(r0,r11) */
 goto L8c669ab0;
L8c669ab0: take=!(*sr&1u);r[5]+=2u;if(take)goto L8c669a98;goto L8c669ab4; /* bf/s 0x8c669a98 */
 goto L8c669ab4;
L8c669ab4: r[0]=sx(m,0x8c669b92u,2); /* mov.w 0x8c669b92,r0 */
 goto L8c669ab6;
L8c669ab6: r[3]=2u; /* mov #0x2,r3 */
 goto L8c669ab8;
L8c669ab8: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c669aba;
L8c669aba: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c669abc;
L8c669abc: *macl=r[0]*r[4]; /* mul.l r0,r4 */
 goto L8c669abe;
L8c669abe: r[0]=sx(m,0x8c669b94u,2); /* mov.w 0x8c669b94,r0 */
 goto L8c669ac0;
L8c669ac0: r[4]=*macl; /* sts MACL,r4 */
 goto L8c669ac2;
L8c669ac2: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c669ac4;
L8c669ac4: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c669ac6;
L8c669ac6: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c669ac8;
L8c669ac8: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c669aca;
L8c669aca: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c669acc;
L8c669acc: if(!(*sr&1u))goto L8c669ae4; /* bf 0x8c669ae4 */
 goto L8c669ace;
L8c669ace: r[0]=sx(m,0x8c669b96u,2); /* mov.w 0x8c669b96,r0 */
 goto L8c669ad0;
L8c669ad0: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c669ad2;
L8c669ad2: r[0]+=16u; /* add #0x10,r0 */
 goto L8c669ad4;
L8c669ad4: r[6]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r6 */
 goto L8c669ad6;
L8c669ad6: r[0]+=4u; /* add #0x4,r0 */
 goto L8c669ad8;
L8c669ad8: r[5]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r5 */
 goto L8c669ada;
L8c669ada: r[0]+=4294967280u; /* add #-0x10,r0 */
 goto L8c669adc;
L8c669adc: target=r[3];*pr=0x8c669ae0u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c669ae0;
L8c669ae0: r[3]=3u; /* mov #0x3,r3 */
 goto L8c669ae2;
L8c669ae2: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c669ae4;
L8c669ae4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c669ae6;
L8c669ae6: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c669ae8;
L8c669ae8: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c669aea;
L8c669aea: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c669aec;
L8c669aec: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c669aee;
L8c669aee: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
