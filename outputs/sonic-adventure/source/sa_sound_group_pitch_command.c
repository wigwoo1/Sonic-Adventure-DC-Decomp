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
/* Static C recovery of retail Tikal helper 8c66b030..8c66b10a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_group_pitch_command(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66b030;
L8c66b030: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66b032;
L8c66b032: r[3]=rd(m,0x8c66b110u,4); /* mov.l 0x8c66b110,r3 */
 goto L8c66b034;
L8c66b034: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66b036;
L8c66b036: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66b038;
L8c66b038: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66b03a;
L8c66b03a: r[14]=rd(m,r[3],4); /* mov.l @r3,r14 */
 goto L8c66b03c;
L8c66b03c: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66b03e;
L8c66b03e: if(!(*sr&1u))goto L8c66b046; /* bf 0x8c66b046 */
 goto L8c66b040;
L8c66b040: r[14]=rd(m,0x8c66b114u,4); /* mov.l 0x8c66b114,r14 */
 goto L8c66b042;
L8c66b042: goto L8c66b0f2; /* bra 0x8c66b0f2 */
 goto L8c66b046;
L8c66b046: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66b048;
L8c66b048: if(!(*sr&1u))goto L8c66b050; /* bf 0x8c66b050 */
 goto L8c66b04a;
L8c66b04a: r[14]=rd(m,0x8c66b118u,4); /* mov.l 0x8c66b118,r14 */
 goto L8c66b04c;
L8c66b04c: goto L8c66b0f2; /* bra 0x8c66b0f2 */
 goto L8c66b050;
L8c66b050: r[3]=4294967280u; /* mov #-0x10,r3 */
 goto L8c66b052;
L8c66b052: r[2]=(uint32_t)(int32_t)(int8_t)r[7]; /* exts.b r7,r2 */
 goto L8c66b054;
L8c66b054: *sr=(*sr&~1u)|!(r[3]&r[2]); /* tst r3,r2 */
 goto L8c66b056;
L8c66b056: if(!(*sr&1u))goto L8c66b0d6; /* bf 0x8c66b0d6 */
 goto L8c66b058;
L8c66b058: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66b05a;
L8c66b05a: r[3]=44u; /* mov #0x2c,r3 */
 goto L8c66b05c;
L8c66b05c: r[2]=sx(m,0x8c66b10au,2); /* mov.w 0x8c66b10a,r2 */
 goto L8c66b05e;
L8c66b05e: r[13]=r[0]; /* mov r0,r13 */
 goto L8c66b060;
L8c66b060: r[4]=(uint32_t)(int32_t)(int8_t)r[13]; /* exts.b r13,r4 */
 goto L8c66b062;
L8c66b062: *macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[4]); /* muls.w r3,r4 */
 goto L8c66b064;
L8c66b064: r[2]+=r[14]; /* add r14,r2 */
 goto L8c66b066;
L8c66b066: r[4]=*macl; /* sts MACL,r4 */
 goto L8c66b068;
L8c66b068: r[4]+=r[2]; /* add r2,r4 */
 goto L8c66b06a;
L8c66b06a: wr(m,r[4],r[5],4); /* mov.l r5,@r4 */
 goto L8c66b06c;
L8c66b06c: r[0]=sx(m,r[4]+0x5u,1); /* mov.b @(0x5,r4),r0 */
 goto L8c66b06e;
L8c66b06e: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66b070;
L8c66b070: take=(*sr&1u);r[14]=0u;if(take)goto L8c66b07c;goto L8c66b074; /* bt/s 0x8c66b07c */
 goto L8c66b074;
L8c66b074: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66b076;
L8c66b076: if((*sr&1u))goto L8c66b080; /* bt 0x8c66b080 */
 goto L8c66b078;
L8c66b078: goto L8c66b0d6; /* bra 0x8c66b0d6 */
 goto L8c66b07c;
L8c66b07c: r[12]=r[14];goto L8c66b082; /* bra 0x8c66b082 */
 goto L8c66b080;
L8c66b080: r[12]=sx(m,0x8c66b10cu,2); /* mov.w 0x8c66b10c,r12 */
 goto L8c66b082;
L8c66b082: r[0]=rd(m,r[4],4); /* mov.l @r4,r0 */
 goto L8c66b084;
L8c66b084: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66b086;
L8c66b086: if((*sr&1u))goto L8c66b094; /* bt 0x8c66b094 */
 goto L8c66b088;
L8c66b088: *sr=(*sr&~1u)|(r[0]==0x00000000u); /* cmp/eq #0x0,r0 */
 goto L8c66b08a;
L8c66b08a: if((*sr&1u))goto L8c66b0ce; /* bt 0x8c66b0ce */
 goto L8c66b08c;
L8c66b08c: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66b08e;
L8c66b08e: if((*sr&1u))goto L8c66b0d2; /* bt 0x8c66b0d2 */
 goto L8c66b090;
L8c66b090: goto L8c66b0d6; /* bra 0x8c66b0d6 */
 goto L8c66b094;
L8c66b094: r[5]=r[14]; /* mov r14,r5 */
 goto L8c66b096;
L8c66b096: r[3]=rd(m,0x8c66b11cu,4); /* mov.l 0x8c66b11c,r3 */
 goto L8c66b098;
L8c66b098: r[2]=33u; /* mov #0x21,r2 */
 goto L8c66b09a;
L8c66b09a: r[13]|=r[12]; /* or r12,r13 */
 goto L8c66b09c;
L8c66b09c: r[1]=rd(m,0x8c66b120u,4); /* mov.l 0x8c66b120,r1 */
 goto L8c66b09e;
L8c66b09e: r[13]|=r[5]; /* or r5,r13 */
 goto L8c66b0a0;
L8c66b0a0: wr(m,r[4]+0xcu,r[14],4); /* mov.l r14,@(0xc,r4) */
 goto L8c66b0a2;
L8c66b0a2: r[7]=(uint32_t)(int32_t)(int8_t)r[7]; /* exts.b r7,r7 */
 goto L8c66b0a4;
L8c66b0a4: wr(m,r[4]+0x10u,r[14],4); /* mov.l r14,@(0x10,r4) */
 goto L8c66b0a6;
L8c66b0a6: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c66b0a8;
L8c66b0a8: r[3]+=6u; /* add #0x6,r3 */
 goto L8c66b0aa;
L8c66b0aa: r[2]=rd(m,0x8c66b124u,4); /* mov.l 0x8c66b124,r2 */
 goto L8c66b0ac;
L8c66b0ac: r[7]<<=2; /* shll2 r7 */
 goto L8c66b0ae;
L8c66b0ae: wr(m,r[1],r[13],1); /* mov.b r13,@r1 */
 goto L8c66b0b0;
L8c66b0b0: r[1]+=3u; /* add #0x3,r1 */
 goto L8c66b0b2;
L8c66b0b2: r[0]=sx(m,r[4]+0x6u,1); /* mov.b @(0x6,r4),r0 */
 goto L8c66b0b4;
L8c66b0b4: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1; /* shll r7 */
 goto L8c66b0b6;
L8c66b0b6: wr(m,r[2],r[0],1); /* mov.b r0,@r2 */
 goto L8c66b0b8;
L8c66b0b8: wr(m,r[3],r[6],1); /* mov.b r6,@r3 */
 goto L8c66b0ba;
L8c66b0ba: r[6]=r[6]&65535u; /* extu.w r6,r6 */
 goto L8c66b0bc;
L8c66b0bc: r[6]>>=8; /* shlr8 r6 */
 goto L8c66b0be;
L8c66b0be: r[0]=rd(m,0x8c66b128u,4); /* mov.l 0x8c66b128,r0 */
 goto L8c66b0c0;
L8c66b0c0: wr(m,r[1],r[6],1); /* mov.b r6,@r1 */
 goto L8c66b0c2;
L8c66b0c2: r[6]=r[4]; /* mov r4,r6 */
 goto L8c66b0c4;
L8c66b0c4: r[4]=5u; /* mov #0x5,r4 */
 goto L8c66b0c6;
L8c66b0c6: wr(m,r[0],r[7],1); /* mov.b r7,@r0 */
 goto L8c66b0c8;
L8c66b0c8: r[6]+=7u; /* add #0x7,r6 */
 goto L8c66b0ca;
L8c66b0ca: r[5]=7u;goto L8c66b0e4; /* bra 0x8c66b0e4 */
 goto L8c66b0ce;
L8c66b0ce: r[5]=16u;goto L8c66b096; /* bra 0x8c66b096 */
 goto L8c66b0d2;
L8c66b0d2: r[5]=48u;goto L8c66b096; /* bra 0x8c66b096 */
 goto L8c66b0d6;
L8c66b0d6: r[14]=rd(m,0x8c66b12cu,4); /* mov.l 0x8c66b12c,r14 */
 goto L8c66b0d8;
L8c66b0d8: goto L8c66b0f2; /* bra 0x8c66b0f2 */
 goto L8c66b0dc;
L8c66b0dc: r[3]=sx(m,r[6],1);r[6]+=1; /* mov.b @r6+,r3 */
 goto L8c66b0de;
L8c66b0de: r[0]=rd(m,0x8c66b120u,4); /* mov.l 0x8c66b120,r0 */
 goto L8c66b0e0;
L8c66b0e0: wr(m,r[4]+r[0],r[3],1); /* mov.b r3,@(r0,r4) */
 goto L8c66b0e2;
L8c66b0e2: r[4]+=1u; /* add #0x1,r4 */
 goto L8c66b0e4;
L8c66b0e4: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]); /* cmp/ge r5,r4 */
 goto L8c66b0e6;
L8c66b0e6: if(!(*sr&1u))goto L8c66b0dc; /* bf 0x8c66b0dc */
 goto L8c66b0e8;
L8c66b0e8: r[3]=rd(m,0x8c66b130u,4); /* mov.l 0x8c66b130,r3 */
 goto L8c66b0ea;
L8c66b0ea: r[4]=rd(m,0x8c66b11cu,4); /* mov.l 0x8c66b11c,r4 */
 goto L8c66b0ec;
L8c66b0ec: target=r[3];*pr=0x8c66b0f0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66b0f0;
L8c66b0f0: r[14]=r[0]; /* mov r0,r14 */
 goto L8c66b0f2;
L8c66b0f2: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66b0f4;
L8c66b0f4: if((*sr&1u))goto L8c66b0fe; /* bt 0x8c66b0fe */
 goto L8c66b0f6;
L8c66b0f6: r[3]=rd(m,0x8c66b138u,4); /* mov.l 0x8c66b138,r3 */
 goto L8c66b0f8;
L8c66b0f8: r[4]=rd(m,0x8c66b134u,4); /* mov.l 0x8c66b134,r4 */
 goto L8c66b0fa;
L8c66b0fa: target=r[3];*pr=0x8c66b0feu;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66b0fe;
L8c66b0fe: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66b100;
L8c66b100: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66b102;
L8c66b102: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66b104;
L8c66b104: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66b106;
L8c66b106: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
