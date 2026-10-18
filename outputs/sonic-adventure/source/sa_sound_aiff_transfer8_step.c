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
/* Static C recovery of retail Tikal helper 8c669af2..8c669bfc.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_aiff_transfer8_step(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c669af2;
L8c669af2: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c669af4;
L8c669af4: r[14]=r[4]; /* mov r4,r14 */
 goto L8c669af6;
L8c669af6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c669af8;
L8c669af8: r[13]=r[4]; /* mov r4,r13 */
 goto L8c669afa;
L8c669afa: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c669afc;
L8c669afc: r[13]+=80u; /* add #0x50,r13 */
 goto L8c669afe;
L8c669afe: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c669b00;
L8c669b00: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c669b02;
L8c669b02: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c669b04;
L8c669b04: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c669b06;
L8c669b06: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c669b08;
L8c669b08: take=!(*sr&1u);r[12]=rd(m,r[13],4);if(take)goto L8c669bd2;goto L8c669b0c; /* bf/s 0x8c669bd2 */
 goto L8c669b0c;
L8c669b0c: r[2]=rd(m,0x8c669b9cu,4); /* mov.l 0x8c669b9c,r2 */
 goto L8c669b0e;
L8c669b0e: target=r[2];*pr=0x8c669b12u;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c669b12;
L8c669b12: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c669b14;
L8c669b14: if(!(*sr&1u))goto L8c669bd2; /* bf 0x8c669bd2 */
 goto L8c669b16;
L8c669b16: r[0]=sx(m,0x8c669b98u,2); /* mov.w 0x8c669b98,r0 */
 goto L8c669b18;
L8c669b18: r[6]=r[13]; /* mov r13,r6 */
 goto L8c669b1a;
L8c669b1a: r[5]=r[13]; /* mov r13,r5 */
 goto L8c669b1c;
L8c669b1c: r[7]=r[13]; /* mov r13,r7 */
 goto L8c669b1e;
L8c669b1e: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c669b20;
L8c669b20: r[0]+=4u; /* add #0x4,r0 */
 goto L8c669b22;
L8c669b22: r[5]+=32u; /* add #0x20,r5 */
 goto L8c669b24;
L8c669b24: r[7]+=40u; /* add #0x28,r7 */
 goto L8c669b26;
L8c669b26: r[6]+=36u; /* add #0x24,r6 */
 goto L8c669b28;
L8c669b28: target=r[2];*pr=0x8c669b2cu;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c669b2c;
L8c669b2c: r[4]=rd(m,r[13]+0x18u,4); /* mov.l @(0x18,r13),r4 */
 goto L8c669b2e;
L8c669b2e: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c669b30;
L8c669b30: r[2]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r2 */
 goto L8c669b32;
L8c669b32: r[4]-=r[3]; /* sub r3,r4 */
 goto L8c669b34;
L8c669b34: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c669b36;
L8c669b36: take=!(*sr&1u);r[2]=rd(m,r[13]+0x4u,4);if(take)goto L8c669b3c;goto L8c669b3a; /* bf/s 0x8c669b3c */
 goto L8c669b3a;
L8c669b3a: r[4]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r4 */
 goto L8c669b3c;
L8c669b3c: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c669b3e;
L8c669b3e: take=!(*sr&1u);r[5]=rd(m,r[13]+0x14u,4);if(take)goto L8c669b44;goto L8c669b42; /* bf/s 0x8c669b44 */
 goto L8c669b42;
L8c669b42: r[4]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r4 */
 goto L8c669b44;
L8c669b44: r[10]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r10 */
 goto L8c669b46;
L8c669b46: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c669b48;
L8c669b48: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1; /* shll r10 */
 goto L8c669b4a;
L8c669b4a: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c669b4c;
L8c669b4c: r[10]+=r[5]; /* add r5,r10 */
 goto L8c669b4e;
L8c669b4e: take=!(*sr&1u);r[11]=0u;if(take)goto L8c669ba0;goto L8c669b52; /* bf/s 0x8c669ba0 */
 goto L8c669b52;
L8c669b52: r[1]=rd(m,r[13]+0x1cu,4); /* mov.l @(0x1c,r13),r1 */
 goto L8c669b54;
L8c669b54: r[7]=r[11]; /* mov r11,r7 */
 goto L8c669b56;
L8c669b56: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c669b58;
L8c669b58: r[1]+=r[3]; /* add r3,r1 */
 goto L8c669b5a;
L8c669b5a: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c669b5c;
L8c669b5c: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c669b5e;
L8c669b5e: r[1]+=r[5]; /* add r5,r1 */
 goto L8c669b60;
L8c669b60: r[5]=r[11]; /* mov r11,r5 */
 goto L8c669b62;
L8c669b62: take=!(*sr&1u);r[6]=r[11];if(take)goto L8c669bbe;goto L8c669b66; /* bf/s 0x8c669bbe */
 goto L8c669b66;
L8c669b66: r[0]=r[12]; /* mov r12,r0 */
 goto L8c669b68;
L8c669b68: r[2]=sx(m,r[5]+r[0],1); /* mov.b @(r0,r5),r2 */
 goto L8c669b6a;
L8c669b6a: r[3]=r[10]; /* mov r10,r3 */
 goto L8c669b6c;
L8c669b6c: r[3]+=r[6]; /* add r6,r3 */
 goto L8c669b6e;
L8c669b6e: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c669b70;
L8c669b70: r[0]=r[6]; /* mov r6,r0 */
 goto L8c669b72;
L8c669b72: r[2]<<=8; /* shll8 r2 */
 goto L8c669b74;
L8c669b74: wr(m,r[3],r[2],2); /* mov.w r2,@r3 */
 goto L8c669b76;
L8c669b76: r[3]=r[5]; /* mov r5,r3 */
 goto L8c669b78;
L8c669b78: r[3]+=1u; /* add #0x1,r3 */
 goto L8c669b7a;
L8c669b7a: r[3]+=r[12]; /* add r12,r3 */
 goto L8c669b7c;
L8c669b7c: r[3]=sx(m,r[3],1); /* mov.b @r3,r3 */
 goto L8c669b7e;
L8c669b7e: r[7]+=1u; /* add #0x1,r7 */
 goto L8c669b80;
L8c669b80: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[4]); /* cmp/ge r4,r7 */
 goto L8c669b82;
L8c669b82: r[5]+=2u; /* add #0x2,r5 */
 goto L8c669b84;
L8c669b84: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c669b86;
L8c669b86: r[3]<<=8; /* shll8 r3 */
 goto L8c669b88;
L8c669b88: wr(m,r[0]+r[1],r[3],2); /* mov.w r3,@(r0,r1) */
 goto L8c669b8a;
L8c669b8a: take=!(*sr&1u);r[6]+=2u;if(take)goto L8c669b66;goto L8c669b8e; /* bf/s 0x8c669b66 */
 goto L8c669b8e;
L8c669b8e: goto L8c669bbe; /* bra 0x8c669bbe */
 goto L8c669ba0;
L8c669ba0: r[5]=0u; /* mov #0x0,r5 */
 goto L8c669ba2;
L8c669ba2: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c669ba4;
L8c669ba4: r[6]=0u; /* mov #0x0,r6 */
 goto L8c669ba6;
L8c669ba6: r[7]=r[11]; /* mov r11,r7 */
 goto L8c669ba8;
L8c669ba8: r[5]+=r[12]; /* add r12,r5 */
 goto L8c669baa;
L8c669baa: take=!(*sr&1u);r[6]+=r[10];if(take)goto L8c669bbe;goto L8c669bae; /* bf/s 0x8c669bbe */
 goto L8c669bae;
L8c669bae: r[3]=sx(m,r[5],1);r[5]+=1; /* mov.b @r5+,r3 */
 goto L8c669bb0;
L8c669bb0: r[7]+=1u; /* add #0x1,r7 */
 goto L8c669bb2;
L8c669bb2: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[4]); /* cmp/ge r4,r7 */
 goto L8c669bb4;
L8c669bb4: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c669bb6;
L8c669bb6: r[3]<<=8; /* shll8 r3 */
 goto L8c669bb8;
L8c669bb8: wr(m,r[6],r[3],2); /* mov.w r3,@r6 */
 goto L8c669bba;
L8c669bba: take=!(*sr&1u);r[6]+=2u;if(take)goto L8c669bae;goto L8c669bbe; /* bf/s 0x8c669bae */
 goto L8c669bbe;
L8c669bbe: r[0]=sx(m,0x8c669c0cu,2); /* mov.w 0x8c669c0c,r0 */
 goto L8c669bc0;
L8c669bc0: r[3]=2u; /* mov #0x2,r3 */
 goto L8c669bc2;
L8c669bc2: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c669bc4;
L8c669bc4: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c669bc6;
L8c669bc6: *macl=r[0]*r[4]; /* mul.l r0,r4 */
 goto L8c669bc8;
L8c669bc8: r[0]=sx(m,0x8c669c0eu,2); /* mov.w 0x8c669c0e,r0 */
 goto L8c669bca;
L8c669bca: r[4]=*macl; /* sts MACL,r4 */
 goto L8c669bcc;
L8c669bcc: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c669bce;
L8c669bce: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c669bd0;
L8c669bd0: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c669bd2;
L8c669bd2: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c669bd4;
L8c669bd4: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c669bd6;
L8c669bd6: if(!(*sr&1u))goto L8c669bee; /* bf 0x8c669bee */
 goto L8c669bd8;
L8c669bd8: r[0]=sx(m,0x8c669c10u,2); /* mov.w 0x8c669c10,r0 */
 goto L8c669bda;
L8c669bda: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c669bdc;
L8c669bdc: r[0]+=16u; /* add #0x10,r0 */
 goto L8c669bde;
L8c669bde: r[6]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r6 */
 goto L8c669be0;
L8c669be0: r[0]+=4u; /* add #0x4,r0 */
 goto L8c669be2;
L8c669be2: r[5]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r5 */
 goto L8c669be4;
L8c669be4: r[0]+=4294967280u; /* add #-0x10,r0 */
 goto L8c669be6;
L8c669be6: target=r[3];*pr=0x8c669beau;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c669bea;
L8c669bea: r[3]=3u; /* mov #0x3,r3 */
 goto L8c669bec;
L8c669bec: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c669bee;
L8c669bee: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c669bf0;
L8c669bf0: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c669bf2;
L8c669bf2: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c669bf4;
L8c669bf4: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c669bf6;
L8c669bf6: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c669bf8;
L8c669bf8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
