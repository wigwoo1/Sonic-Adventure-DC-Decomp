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
/* Retail SND sample transfer step, 8c66a01c..8c66a12e.
 * Pending getter resolves to native code; query/submission are caller callbacks. */
SA_STAGE_EXPORT int sa_sound_snd_transfer16_step(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66a01c;
L8c66a01c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66a01e;
L8c66a01e: r[14]=r[4]; /* mov r4,r14 */
 goto L8c66a020;
L8c66a020: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66a022;
L8c66a022: r[13]=r[4]; /* mov r4,r13 */
 goto L8c66a024;
L8c66a024: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66a026;
L8c66a026: r[13]+=80u; /* add #0x50,r13 */
 goto L8c66a028;
L8c66a028: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66a02a;
L8c66a02a: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c66a02c;
L8c66a02c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66a02e;
L8c66a02e: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66a030;
L8c66a030: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66a032;
L8c66a032: take=!(*sr&1u);r[12]=rd(m,r[13],4);if(take)goto L8c66a104;goto L8c66a036; /* bf/s 0x8c66a104 */
 goto L8c66a036;
L8c66a036: r[2]=rd(m,0x8c66a138u,4); /* mov.l 0x8c66a138,r2 */
 goto L8c66a038;
L8c66a038: target=r[2];*pr=0x8c66a03cu;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66a03c;
L8c66a03c: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c66a03e;
L8c66a03e: if(!(*sr&1u))goto L8c66a104; /* bf 0x8c66a104 */
 goto L8c66a040;
L8c66a040: r[0]=sx(m,0x8c66a12eu,2); /* mov.w 0x8c66a12e,r0 */
 goto L8c66a042;
L8c66a042: r[6]=r[13]; /* mov r13,r6 */
 goto L8c66a044;
L8c66a044: r[5]=r[13]; /* mov r13,r5 */
 goto L8c66a046;
L8c66a046: r[7]=r[13]; /* mov r13,r7 */
 goto L8c66a048;
L8c66a048: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c66a04a;
L8c66a04a: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66a04c;
L8c66a04c: r[5]+=32u; /* add #0x20,r5 */
 goto L8c66a04e;
L8c66a04e: r[7]+=40u; /* add #0x28,r7 */
 goto L8c66a050;
L8c66a050: r[6]+=36u; /* add #0x24,r6 */
 goto L8c66a052;
L8c66a052: target=r[2];*pr=0x8c66a056u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66a056;
L8c66a056: r[4]=rd(m,r[13]+0x18u,4); /* mov.l @(0x18,r13),r4 */
 goto L8c66a058;
L8c66a058: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66a05a;
L8c66a05a: r[2]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r2 */
 goto L8c66a05c;
L8c66a05c: r[4]-=r[3]; /* sub r3,r4 */
 goto L8c66a05e;
L8c66a05e: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66a060;
L8c66a060: take=!(*sr&1u);r[2]=rd(m,r[13]+0x4u,4);if(take)goto L8c66a066;goto L8c66a064; /* bf/s 0x8c66a066 */
 goto L8c66a064;
L8c66a064: r[4]=rd(m,r[13]+0x24u,4); /* mov.l @(0x24,r13),r4 */
 goto L8c66a066;
L8c66a066: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c66a068;
L8c66a068: take=!(*sr&1u);r[5]=rd(m,r[13]+0x14u,4);if(take)goto L8c66a06e;goto L8c66a06c; /* bf/s 0x8c66a06e */
 goto L8c66a06c;
L8c66a06c: r[4]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r4 */
 goto L8c66a06e;
L8c66a06e: r[11]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r11 */
 goto L8c66a070;
L8c66a070: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66a072;
L8c66a072: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1; /* shll r11 */
 goto L8c66a074;
L8c66a074: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a076;
L8c66a076: r[11]+=r[5]; /* add r5,r11 */
 goto L8c66a078;
L8c66a078: take=!(*sr&1u);r[1]=0u;if(take)goto L8c66a0cc;goto L8c66a07c; /* bf/s 0x8c66a0cc */
 goto L8c66a07c;
L8c66a07c: r[9]=rd(m,r[13]+0x1cu,4); /* mov.l @(0x1c,r13),r9 */
 goto L8c66a07e;
L8c66a07e: r[7]=r[1]; /* mov r1,r7 */
 goto L8c66a080;
L8c66a080: r[3]=rd(m,r[13]+0x20u,4); /* mov.l @(0x20,r13),r3 */
 goto L8c66a082;
L8c66a082: r[9]+=r[3]; /* add r3,r9 */
 goto L8c66a084;
L8c66a084: *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1; /* shll r9 */
 goto L8c66a086;
L8c66a086: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66a088;
L8c66a088: r[9]+=r[5]; /* add r5,r9 */
 goto L8c66a08a;
L8c66a08a: r[5]=r[1]; /* mov r1,r5 */
 goto L8c66a08c;
L8c66a08c: take=!(*sr&1u);r[6]=r[1];if(take)goto L8c66a0f0;goto L8c66a090; /* bf/s 0x8c66a0f0 */
 goto L8c66a090;
L8c66a090: r[0]=r[6]; /* mov r6,r0 */
 goto L8c66a092;
L8c66a092: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c66a094;
L8c66a094: r[13]=sx(m,r[0]+r[12],2); /* mov.w @(r0,r12),r13 */
 goto L8c66a096;
L8c66a096: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c66a098;
L8c66a098: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66a09a;
L8c66a09a: r[7]+=1u; /* add #0x1,r7 */
 goto L8c66a09c;
L8c66a09c: r[13]=r[13]&65535u; /* extu.w r13,r13 */
 goto L8c66a09e;
L8c66a09e: r[3]=r[13]; /* mov r13,r3 */
 goto L8c66a0a0;
L8c66a0a0: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c66a0a2;
L8c66a0a2: r[13]<<=8; /* shll8 r13 */
 goto L8c66a0a4;
L8c66a0a4: r[13]|=r[3]; /* or r3,r13 */
 goto L8c66a0a6;
L8c66a0a6: wr(m,r[0]+r[11],r[13],2); /* mov.w r13,@(r0,r11) */
 goto L8c66a0a8;
L8c66a0a8: r[0]=r[6]; /* mov r6,r0 */
 goto L8c66a0aa;
L8c66a0aa: r[0]+=1u; /* add #0x1,r0 */
 goto L8c66a0ac;
L8c66a0ac: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c66a0ae;
L8c66a0ae: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[4]); /* cmp/ge r4,r7 */
 goto L8c66a0b0;
L8c66a0b0: r[13]=sx(m,r[0]+r[12],2); /* mov.w @(r0,r12),r13 */
 goto L8c66a0b2;
L8c66a0b2: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66a0b4;
L8c66a0b4: r[5]+=2u; /* add #0x2,r5 */
 goto L8c66a0b6;
L8c66a0b6: r[13]=r[13]&65535u; /* extu.w r13,r13 */
 goto L8c66a0b8;
L8c66a0b8: r[1]=r[13]; /* mov r13,r1 */
 goto L8c66a0ba;
L8c66a0ba: r[3]=r[1]; /* mov r1,r3 */
 goto L8c66a0bc;
L8c66a0bc: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c66a0be;
L8c66a0be: r[13]<<=8; /* shll8 r13 */
 goto L8c66a0c0;
L8c66a0c0: r[13]|=r[3]; /* or r3,r13 */
 goto L8c66a0c2;
L8c66a0c2: wr(m,r[0]+r[9],r[13],2); /* mov.w r13,@(r0,r9) */
 goto L8c66a0c4;
L8c66a0c4: take=!(*sr&1u);r[6]+=2u;if(take)goto L8c66a090;goto L8c66a0c8; /* bf/s 0x8c66a090 */
 goto L8c66a0c8;
L8c66a0c8: goto L8c66a0f0; /* bra 0x8c66a0f0 */
 goto L8c66a0cc;
L8c66a0cc: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c66a0ce;
L8c66a0ce: r[6]=r[1]; /* mov r1,r6 */
 goto L8c66a0d0;
L8c66a0d0: take=!(*sr&1u);r[5]=r[1];if(take)goto L8c66a0f0;goto L8c66a0d4; /* bf/s 0x8c66a0f0 */
 goto L8c66a0d4;
L8c66a0d4: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66a0d6;
L8c66a0d6: r[13]=sx(m,r[0]+r[12],2); /* mov.w @(r0,r12),r13 */
 goto L8c66a0d8;
L8c66a0d8: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c66a0da;
L8c66a0da: r[13]=r[13]&65535u; /* extu.w r13,r13 */
 goto L8c66a0dc;
L8c66a0dc: r[7]=r[13]; /* mov r13,r7 */
 goto L8c66a0de;
L8c66a0de: r[3]=r[7]; /* mov r7,r3 */
 goto L8c66a0e0;
L8c66a0e0: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c66a0e2;
L8c66a0e2: r[13]<<=8; /* shll8 r13 */
 goto L8c66a0e4;
L8c66a0e4: r[6]+=1u; /* add #0x1,r6 */
 goto L8c66a0e6;
L8c66a0e6: r[13]|=r[3]; /* or r3,r13 */
 goto L8c66a0e8;
L8c66a0e8: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[4]); /* cmp/ge r4,r6 */
 goto L8c66a0ea;
L8c66a0ea: wr(m,r[0]+r[11],r[13],2); /* mov.w r13,@(r0,r11) */
 goto L8c66a0ec;
L8c66a0ec: take=!(*sr&1u);r[5]+=2u;if(take)goto L8c66a0d4;goto L8c66a0f0; /* bf/s 0x8c66a0d4 */
 goto L8c66a0f0;
L8c66a0f0: r[0]=sx(m,0x8c66a130u,2); /* mov.w 0x8c66a130,r0 */
 goto L8c66a0f2;
L8c66a0f2: r[3]=2u; /* mov #0x2,r3 */
 goto L8c66a0f4;
L8c66a0f4: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66a0f6;
L8c66a0f6: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66a0f8;
L8c66a0f8: *macl=r[0]*r[4]; /* mul.l r0,r4 */
 goto L8c66a0fa;
L8c66a0fa: r[0]=sx(m,0x8c66a132u,2); /* mov.w 0x8c66a132,r0 */
 goto L8c66a0fc;
L8c66a0fc: r[4]=*macl; /* sts MACL,r4 */
 goto L8c66a0fe;
L8c66a0fe: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c66a100;
L8c66a100: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c66a102;
L8c66a102: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66a104;
L8c66a104: r[0]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r0 */
 goto L8c66a106;
L8c66a106: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a108;
L8c66a108: if(!(*sr&1u))goto L8c66a120; /* bf 0x8c66a120 */
 goto L8c66a10a;
L8c66a10a: r[0]=sx(m,0x8c66a134u,2); /* mov.w 0x8c66a134,r0 */
 goto L8c66a10c;
L8c66a10c: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c66a10e;
L8c66a10e: r[0]+=16u; /* add #0x10,r0 */
 goto L8c66a110;
L8c66a110: r[6]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r6 */
 goto L8c66a112;
L8c66a112: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66a114;
L8c66a114: r[5]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r5 */
 goto L8c66a116;
L8c66a116: r[0]+=4294967280u; /* add #-0x10,r0 */
 goto L8c66a118;
L8c66a118: target=r[3];*pr=0x8c66a11cu;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66a11c;
L8c66a11c: r[3]=3u; /* mov #0x3,r3 */
 goto L8c66a11e;
L8c66a11e: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto L8c66a120;
L8c66a120: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66a122;
L8c66a122: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c66a124;
L8c66a124: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66a126;
L8c66a126: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66a128;
L8c66a128: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66a12a;
L8c66a12a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
