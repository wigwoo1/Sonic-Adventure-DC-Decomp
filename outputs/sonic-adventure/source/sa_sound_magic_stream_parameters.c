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
/* Retail magic8000 stream parameters65afca..65b11c.
 * Clears output flag first; requires signed size>=48,header offset>=44,flag1.
 * Copies BE16/BE32 fields with partial updates retained on rejection. */
SA_STAGE_EXPORT int sa_sound_magic_stream_parameters(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65afca;
L8c65afca: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c65afcc;
L8c65afcc: r[3]=0u; /* mov #0x0,r3 */
 goto L8c65afce;
L8c65afce: r[2]=48u; /* mov #0x30,r2 */
 goto L8c65afd0;
L8c65afd0: wr(m,r[7],r[3],2); /* mov.w r3,@r7 */
 goto L8c65afd2;
L8c65afd2: r[3]=r[2]; /* mov r2,r3 */
 goto L8c65afd4;
L8c65afd4: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[3]); /* cmp/ge r3,r5 */
 goto L8c65afd6;
L8c65afd6: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c65afd8;
L8c65afd8: if(!(*sr&1u))goto L8c65b014; /* bf 0x8c65b014 */
 goto L8c65afda;
L8c65afda: r[3]=sx(m,r[4],2); /* mov.w @r4,r3 */
 goto L8c65afdc;
L8c65afdc: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c65afde;
L8c65afde: r[5]=rd(m,0x8c65b050u,4); /* mov.l 0x8c65b050,r5 */
 goto L8c65afe0;
L8c65afe0: r[3]=r[3]&65535u; /* extu.w r3,r3 */
 goto L8c65afe2;
L8c65afe2: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c65afe4;
L8c65afe4: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c65afe6;
L8c65afe6: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c65afe8;
L8c65afe8: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c65afea;
L8c65afea: r[2]=rd(m,0x8c65b054u,4); /* mov.l 0x8c65b054,r2 */
 goto L8c65afec;
L8c65afec: r[1]<<=8; /* shll8 r1 */
 goto L8c65afee;
L8c65afee: r[1]&=r[5]; /* and r5,r1 */
 goto L8c65aff0;
L8c65aff0: r[3]|=r[1]; /* or r1,r3 */
 goto L8c65aff2;
L8c65aff2: wr(m,r[15],r[3],2); /* mov.w r3,@r15 */
 goto L8c65aff4;
L8c65aff4: r[3]=r[3]&65535u; /* extu.w r3,r3 */
 goto L8c65aff6;
L8c65aff6: *sr=(*sr&~1u)|(r[2]==r[3]); /* cmp/eq r2,r3 */
 goto L8c65aff8;
L8c65aff8: if(!(*sr&1u))goto L8c65b048; /* bf 0x8c65b048 */
 goto L8c65affa;
L8c65affa: r[0]=sx(m,r[4]+0x2u,2); /* mov.w @(0x2,r4),r0 */
 goto L8c65affc;
L8c65affc: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c65affe;
L8c65affe: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c65b000;
L8c65b000: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c65b002;
L8c65b002: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c65b004;
L8c65b004: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c65b006;
L8c65b006: r[3]=44u; /* mov #0x2c,r3 */
 goto L8c65b008;
L8c65b008: r[2]<<=8; /* shll8 r2 */
 goto L8c65b00a;
L8c65b00a: r[2]&=r[5]; /* and r5,r2 */
 goto L8c65b00c;
L8c65b00c: r[0]|=r[2]; /* or r2,r0 */
 goto L8c65b00e;
L8c65b00e: *sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[3]); /* cmp/ge r3,r0 */
 goto L8c65b010;
L8c65b010: take=(*sr&1u);wr(m,r[15],r[0],4);if(take)goto L8c65b01a;goto L8c65b014; /* bt/s 0x8c65b01a */
 goto L8c65b014;
L8c65b014: r[0]=4294967295u; /* mov #-0x1,r0 */
 goto L8c65b016;
L8c65b016: r[15]+=4u;return 0; /* rts */
L8c65b01a: r[0]=sx(m,r[4]+0x14u,2); /* mov.w @(0x14,r4),r0 */
 goto L8c65b01c;
L8c65b01c: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c65b01e;
L8c65b01e: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c65b020;
L8c65b020: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c65b022;
L8c65b022: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c65b024;
L8c65b024: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c65b026;
L8c65b026: r[2]<<=8; /* shll8 r2 */
 goto L8c65b028;
L8c65b028: r[2]&=r[5]; /* and r5,r2 */
 goto L8c65b02a;
L8c65b02a: r[0]|=r[2]; /* or r2,r0 */
 goto L8c65b02c;
L8c65b02c: wr(m,r[6],r[0],4); /* mov.l r0,@r6 */
 goto L8c65b02e;
L8c65b02e: r[0]=sx(m,r[4]+0x16u,2); /* mov.w @(0x16,r4),r0 */
 goto L8c65b030;
L8c65b030: r[6]=sx(m,0x8c65b04eu,2); /* mov.w 0x8c65b04e,r6 */
 goto L8c65b032;
L8c65b032: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c65b034;
L8c65b034: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c65b036;
L8c65b036: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c65b038;
L8c65b038: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c65b03a;
L8c65b03a: r[2]<<=8; /* shll8 r2 */
 goto L8c65b03c;
L8c65b03c: r[2]&=r[6]; /* and r6,r2 */
 goto L8c65b03e;
L8c65b03e: r[0]|=r[2]; /* or r2,r0 */
 goto L8c65b040;
L8c65b040: wr(m,r[7],r[0],2); /* mov.w r0,@r7 */
 goto L8c65b042;
L8c65b042: r[0]=sx(m,r[7],2); /* mov.w @r7,r0 */
 goto L8c65b044;
L8c65b044: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c65b046;
L8c65b046: if((*sr&1u))goto L8c65b058; /* bt 0x8c65b058 */
 goto L8c65b048;
L8c65b048: r[0]=4294967294u; /* mov #-0x2,r0 */
 goto L8c65b04a;
L8c65b04a: r[15]+=4u;return 0; /* rts */
L8c65b058: r[0]=sx(m,r[4]+0x1au,2); /* mov.w @(0x1a,r4),r0 */
 goto L8c65b05a;
L8c65b05a: r[1]=4294967288u; /* mov #-0x8,r1 */
 goto L8c65b05c;
L8c65b05c: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c65b05e;
L8c65b05e: r[3]=r[0]; /* mov r0,r3 */
 goto L8c65b060;
L8c65b060: r[7]=r[0]; /* mov r0,r7 */
 goto L8c65b062;
L8c65b062: {uint32_t shift=r[1];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r1,r3 */
 goto L8c65b064;
L8c65b064: r[0]<<=8; /* shll8 r0 */
 goto L8c65b066;
L8c65b066: r[0]&=r[6]; /* and r6,r0 */
 goto L8c65b068;
L8c65b068: r[6]=rd(m,0x8c65b11cu,4); /* mov.l 0x8c65b11c,r6 */
 goto L8c65b06a;
L8c65b06a: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c65b06c;
L8c65b06c: r[3]|=r[0]; /* or r0,r3 */
 goto L8c65b06e;
L8c65b06e: wr(m,r[2],r[3],2); /* mov.w r3,@r2 */
 goto L8c65b070;
L8c65b070: r[7]=rd(m,r[4]+0x1cu,4); /* mov.l @(0x1c,r4),r7 */
 goto L8c65b072;
L8c65b072: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c65b074;
L8c65b074: r[2]=r[7]; /* mov r7,r2 */
 goto L8c65b076;
L8c65b076: r[2]>>=16; /* shlr16 r2 */
 goto L8c65b078;
L8c65b078: r[2]>>=8; /* shlr8 r2 */
 goto L8c65b07a;
L8c65b07a: r[0]=r[7]; /* mov r7,r0 */
 goto L8c65b07c;
L8c65b07c: {uint32_t shift=r[1];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r1,r0 */
 goto L8c65b07e;
L8c65b07e: r[1]=r[7]; /* mov r7,r1 */
 goto L8c65b080;
L8c65b080: r[2]=(uint32_t)(int32_t)(int8_t)r[2]; /* exts.b r2,r2 */
 goto L8c65b082;
L8c65b082: r[0]&=r[5]; /* and r5,r0 */
 goto L8c65b084;
L8c65b084: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c65b086;
L8c65b086: r[2]|=r[0]; /* or r0,r2 */
 goto L8c65b088;
L8c65b088: r[0]=r[7]; /* mov r7,r0 */
 goto L8c65b08a;
L8c65b08a: r[0]<<=8; /* shll8 r0 */
 goto L8c65b08c;
L8c65b08c: r[1]<<=16; /* shll16 r1 */
 goto L8c65b08e;
L8c65b08e: r[0]&=r[6]; /* and r6,r0 */
 goto L8c65b090;
L8c65b090: r[1]<<=8; /* shll8 r1 */
 goto L8c65b092;
L8c65b092: r[2]|=r[0]; /* or r0,r2 */
 goto L8c65b094;
L8c65b094: r[2]|=r[1]; /* or r1,r2 */
 goto L8c65b096;
L8c65b096: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c65b098;
L8c65b098: r[7]=rd(m,r[4]+0x20u,4); /* mov.l @(0x20,r4),r7 */
 goto L8c65b09a;
L8c65b09a: r[1]=4294967288u; /* mov #-0x8,r1 */
 goto L8c65b09c;
L8c65b09c: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c65b09e;
L8c65b09e: r[2]=r[7]; /* mov r7,r2 */
 goto L8c65b0a0;
L8c65b0a0: r[2]>>=16; /* shlr16 r2 */
 goto L8c65b0a2;
L8c65b0a2: r[0]=r[7]; /* mov r7,r0 */
 goto L8c65b0a4;
L8c65b0a4: r[2]>>=8; /* shlr8 r2 */
 goto L8c65b0a6;
L8c65b0a6: {uint32_t shift=r[1];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r1,r0 */
 goto L8c65b0a8;
L8c65b0a8: r[1]=r[7]; /* mov r7,r1 */
 goto L8c65b0aa;
L8c65b0aa: r[2]=(uint32_t)(int32_t)(int8_t)r[2]; /* exts.b r2,r2 */
 goto L8c65b0ac;
L8c65b0ac: r[0]&=r[5]; /* and r5,r0 */
 goto L8c65b0ae;
L8c65b0ae: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c65b0b0;
L8c65b0b0: r[2]|=r[0]; /* or r0,r2 */
 goto L8c65b0b2;
L8c65b0b2: r[0]=r[7]; /* mov r7,r0 */
 goto L8c65b0b4;
L8c65b0b4: r[0]<<=8; /* shll8 r0 */
 goto L8c65b0b6;
L8c65b0b6: r[1]<<=16; /* shll16 r1 */
 goto L8c65b0b8;
L8c65b0b8: r[0]&=r[6]; /* and r6,r0 */
 goto L8c65b0ba;
L8c65b0ba: r[1]<<=8; /* shll8 r1 */
 goto L8c65b0bc;
L8c65b0bc: r[2]|=r[0]; /* or r0,r2 */
 goto L8c65b0be;
L8c65b0be: r[2]|=r[1]; /* or r1,r2 */
 goto L8c65b0c0;
L8c65b0c0: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c65b0c2;
L8c65b0c2: r[1]=4294967288u; /* mov #-0x8,r1 */
 goto L8c65b0c4;
L8c65b0c4: r[7]=rd(m,r[4]+0x24u,4); /* mov.l @(0x24,r4),r7 */
 goto L8c65b0c6;
L8c65b0c6: r[3]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r3 */
 goto L8c65b0c8;
L8c65b0c8: r[2]=r[7]; /* mov r7,r2 */
 goto L8c65b0ca;
L8c65b0ca: r[2]>>=16; /* shlr16 r2 */
 goto L8c65b0cc;
L8c65b0cc: r[0]=r[7]; /* mov r7,r0 */
 goto L8c65b0ce;
L8c65b0ce: r[2]>>=8; /* shlr8 r2 */
 goto L8c65b0d0;
L8c65b0d0: {uint32_t shift=r[1];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r1,r0 */
 goto L8c65b0d2;
L8c65b0d2: r[2]=(uint32_t)(int32_t)(int8_t)r[2]; /* exts.b r2,r2 */
 goto L8c65b0d4;
L8c65b0d4: r[0]&=r[5]; /* and r5,r0 */
 goto L8c65b0d6;
L8c65b0d6: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c65b0d8;
L8c65b0d8: r[2]|=r[0]; /* or r0,r2 */
 goto L8c65b0da;
L8c65b0da: r[0]=r[7]; /* mov r7,r0 */
 goto L8c65b0dc;
L8c65b0dc: r[1]=r[7]; /* mov r7,r1 */
 goto L8c65b0de;
L8c65b0de: r[0]<<=8; /* shll8 r0 */
 goto L8c65b0e0;
L8c65b0e0: r[1]<<=16; /* shll16 r1 */
 goto L8c65b0e2;
L8c65b0e2: r[0]&=r[6]; /* and r6,r0 */
 goto L8c65b0e4;
L8c65b0e4: r[1]<<=8; /* shll8 r1 */
 goto L8c65b0e6;
L8c65b0e6: r[2]|=r[0]; /* or r0,r2 */
 goto L8c65b0e8;
L8c65b0e8: r[2]|=r[1]; /* or r1,r2 */
 goto L8c65b0ea;
L8c65b0ea: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c65b0ec;
L8c65b0ec: r[1]=4294967288u; /* mov #-0x8,r1 */
 goto L8c65b0ee;
L8c65b0ee: r[7]=rd(m,r[4]+0x28u,4); /* mov.l @(0x28,r4),r7 */
 goto L8c65b0f0;
L8c65b0f0: r[3]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r3 */
 goto L8c65b0f2;
L8c65b0f2: r[2]=r[7]; /* mov r7,r2 */
 goto L8c65b0f4;
L8c65b0f4: r[2]>>=16; /* shlr16 r2 */
 goto L8c65b0f6;
L8c65b0f6: r[0]=r[7]; /* mov r7,r0 */
 goto L8c65b0f8;
L8c65b0f8: r[2]>>=8; /* shlr8 r2 */
 goto L8c65b0fa;
L8c65b0fa: {uint32_t shift=r[1];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r1,r0 */
 goto L8c65b0fc;
L8c65b0fc: r[1]=r[7]; /* mov r7,r1 */
 goto L8c65b0fe;
L8c65b0fe: r[2]=(uint32_t)(int32_t)(int8_t)r[2]; /* exts.b r2,r2 */
 goto L8c65b100;
L8c65b100: r[0]&=r[5]; /* and r5,r0 */
 goto L8c65b102;
L8c65b102: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c65b104;
L8c65b104: r[2]|=r[0]; /* or r0,r2 */
 goto L8c65b106;
L8c65b106: r[0]=r[7]; /* mov r7,r0 */
 goto L8c65b108;
L8c65b108: r[0]<<=8; /* shll8 r0 */
 goto L8c65b10a;
L8c65b10a: r[1]<<=16; /* shll16 r1 */
 goto L8c65b10c;
L8c65b10c: r[0]&=r[6]; /* and r6,r0 */
 goto L8c65b10e;
L8c65b10e: r[1]<<=8; /* shll8 r1 */
 goto L8c65b110;
L8c65b110: r[2]|=r[0]; /* or r0,r2 */
 goto L8c65b112;
L8c65b112: r[2]|=r[1]; /* or r1,r2 */
 goto L8c65b114;
L8c65b114: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65b116;
L8c65b116: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c65b118;
L8c65b118: r[15]+=4u;return 0; /* rts */
}
