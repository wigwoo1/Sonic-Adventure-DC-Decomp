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
int sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);
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
/* Retail mono block decoder65b120..65b222.
 * 32 samples per18-byte block; signed packed nibbles and predictor history.
 * Verified recurrence,scale4,three coefficient pairs; other scales unverified. */
SA_STAGE_EXPORT int sa_sound_decode_block_mono(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65b120;
L8c65b120: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65b122;
L8c65b122: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65b124;
L8c65b124: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65b126;
L8c65b126: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65b128;
L8c65b128: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65b12a;
L8c65b12a: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c65b12c;
L8c65b12c: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c65b12e;
L8c65b12e: r[3]=sx(m,r[7],2); /* mov.w @r7,r3 */
 goto L8c65b130;
L8c65b130: r[0]=0x8c65b228u; /* mova 0x8c65b228,r0 */
 goto L8c65b132;
L8c65b132: r[13]=r[4]; /* mov r4,r13 */
 goto L8c65b134;
L8c65b134: r[3]<<=16; /* shll16 r3 */
 goto L8c65b136;
L8c65b136: *fpul=r[3]; /* lds r3,FPUL */
 goto L8c65b138;
L8c65b138: r[3]=r[7]; /* mov r7,r3 */
 goto L8c65b13a;
L8c65b13a: r[3]+=2u; /* add #0x2,r3 */
 goto L8c65b13c;
L8c65b13c: wr(m,r[15]+0x4u,r[3],4); /* mov.l r3,@(0x4,r15) */
 goto L8c65b13e;
L8c65b13e: r[2]=sx(m,r[3],2); /* mov.w @r3,r2 */
 goto L8c65b140;
L8c65b140: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c65b142;
L8c65b142: fr[4]=rd(m,r[0],4); /* fmov.s @r0,fr4 */
 goto L8c65b144;
L8c65b144: r[0]=32u; /* mov #0x20,r0 */
 goto L8c65b146;
L8c65b146: r[2]<<=16; /* shll16 r2 */
 goto L8c65b148;
L8c65b148: *fpul=r[2]; /* lds r2,FPUL */
 goto L8c65b14a;
L8c65b14a: r[2]=sx(m,r[0]+r[15],2); /* mov.w @(r0,r15),r2 */
 goto L8c65b14c;
L8c65b14c: r[0]=36u; /* mov #0x24,r0 */
 goto L8c65b14e;
L8c65b14e: fr[6]=fr[3]; /* fmov fr3,fr6 */
 goto L8c65b150;
L8c65b150: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c65b152;
L8c65b152: r[2]<<=2; /* shll2 r2 */
 goto L8c65b154;
L8c65b154: r[2]<<=2; /* shll2 r2 */
 goto L8c65b156;
L8c65b156: *fpul=r[2]; /* lds r2,FPUL */
 goto L8c65b158;
L8c65b158: r[2]=sx(m,r[0]+r[15],2); /* mov.w @(r0,r15),r2 */
 goto L8c65b15a;
L8c65b15a: fr[7]=fr[3]; /* fmov fr3,fr7 */
 goto L8c65b15c;
L8c65b15c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c65b15e;
L8c65b15e: r[2]<<=2; /* shll2 r2 */
 goto L8c65b160;
L8c65b160: r[2]<<=2; /* shll2 r2 */
 goto L8c65b162;
L8c65b162: *fpul=r[2]; /* lds r2,FPUL */
 goto L8c65b164;
L8c65b164: fr[9]=fr[3]; /* fmov fr3,fr9 */
 goto L8c65b166;
L8c65b166: status=sa_sh4_divide(&fr[9],&fr[4],&f->fpscr,&fr[9]);if(status)return status; /* fdiv fr4,fr9 */
 goto L8c65b168;
L8c65b168: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c65b16a;
L8c65b16a: fr[8]=fr[3]; /* fmov fr3,fr8 */
 goto L8c65b16c;
L8c65b16c: status=sa_sh4_divide(&fr[8],&fr[4],&f->fpscr,&fr[8]);if(status)return status; /* fdiv fr4,fr8 */
 goto L8c65b16e;
L8c65b16e: r[9]=sx(m,0x8c65b224u,2); /* mov.w 0x8c65b224,r9 */
 goto L8c65b170;
L8c65b170: r[11]=32u; /* mov #0x20,r11 */
 goto L8c65b172;
L8c65b172: r[8]=sx(m,0x8c65b222u,2); /* mov.w 0x8c65b222,r8 */
 goto L8c65b174;
L8c65b174: r[10]=0u; /* mov #0x0,r10 */
 goto L8c65b176;
L8c65b176: r[12]=r[10];goto L8c65b1fa; /* bra 0x8c65b1fa */
 goto L8c65b17a;
L8c65b17a: r[4]=sx(m,r[13],2); /* mov.w @r13,r4 */
 goto L8c65b17c;
L8c65b17c: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c65b17e;
L8c65b17e: r[2]=r[4]; /* mov r4,r2 */
 goto L8c65b180;
L8c65b180: r[1]=r[4]; /* mov r4,r1 */
 goto L8c65b182;
L8c65b182: r[2]<<=8; /* shll8 r2 */
 goto L8c65b184;
L8c65b184: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r1 */
 goto L8c65b186;
L8c65b186: r[3]=rd(m,0x8c65b22cu,4); /* mov.l 0x8c65b22c,r3 */
 goto L8c65b188;
L8c65b188: r[2]&=r[8]; /* and r8,r2 */
 goto L8c65b18a;
L8c65b18a: r[1]=r[1]&255u; /* extu.b r1,r1 */
 goto L8c65b18c;
L8c65b18c: r[1]|=r[2]; /* or r2,r1 */
 goto L8c65b18e;
L8c65b18e: r[4]=(uint32_t)(int32_t)(int16_t)r[1]; /* exts.w r1,r4 */
 goto L8c65b190;
L8c65b190: *sr=(*sr&~1u)|!(r[4]&r[3]); /* tst r4,r3 */
 goto L8c65b192;
L8c65b192: if((*sr&1u))goto L8c65b198; /* bt 0x8c65b198 */
 goto L8c65b194;
L8c65b194: r[0]=r[12];goto L8c65b212; /* bra 0x8c65b212 */
 goto L8c65b198;
L8c65b198: r[4]&=r[9]; /* and r9,r4 */
 goto L8c65b19a;
L8c65b19a: wr(m,r[15],r[10],4); /* mov.l r10,@r15 */
 goto L8c65b19c;
L8c65b19c: r[4]+=1u; /* add #0x1,r4 */
 goto L8c65b19e;
L8c65b19e: *fpul=r[4]; /* lds r4,FPUL */
 goto L8c65b1a0;
L8c65b1a0: r[13]+=2u; /* add #0x2,r13 */
 goto L8c65b1a2;
L8c65b1a2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c65b1a4;
L8c65b1a4: fr[10]=fr[3]; /* fmov fr3,fr10 */
 goto L8c65b1a6;
L8c65b1a6: fr[3]=fr[8]; /* fmov fr8,fr3 */
 goto L8c65b1a8;
L8c65b1a8: status=sa_sh4_multiply(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status; /* fmul fr7,fr3 */
 goto L8c65b1aa;
L8c65b1aa: r[4]=sx(m,r[13],1);r[13]+=1; /* mov.b @r13+,r4 */
 goto L8c65b1ac;
L8c65b1ac: fr[0]=fr[9]; /* fmov fr9,fr0 */
 goto L8c65b1ae;
L8c65b1ae: r[0]=rd(m,0x8c65b230u,4); /* mov.l 0x8c65b230,r0 */
 goto L8c65b1b0;
L8c65b1b0: r[4]=r[4]&255u; /* extu.b r4,r4 */
 goto L8c65b1b2;
L8c65b1b2: status=sa_sh4_fmac(&fr[0],&fr[6],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr6,fr3 */
 goto L8c65b1b4;
L8c65b1b4: r[4]<<=2; /* shll2 r4 */
 goto L8c65b1b6;
L8c65b1b6: fr[4]=rd(m,r[0]+r[4],4); /* fmov.s @(r0,r4),fr4 */
 goto L8c65b1b8;
L8c65b1b8: r[1]=r[4]; /* mov r4,r1 */
 goto L8c65b1ba;
L8c65b1ba: fr[0]=fr[10]; /* fmov fr10,fr0 */
 goto L8c65b1bc;
L8c65b1bc: fr[5]=fr[6]; /* fmov fr6,fr5 */
 goto L8c65b1be;
L8c65b1be: status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr4,fr3 */
 goto L8c65b1c0;
L8c65b1c0: fr[0]=fr[9]; /* fmov fr9,fr0 */
 goto L8c65b1c2;
L8c65b1c2: r[0]=rd(m,0x8c65b234u,4); /* mov.l 0x8c65b234,r0 */
 goto L8c65b1c4;
L8c65b1c4: fr[1]=fr[3]; /* fmov fr3,fr1 */
 goto L8c65b1c6;
L8c65b1c6: status=sa_sh4_truncate_i32(fr[1],&f->fpscr,fpul);if(status)return status; /* ftrc fr1,FPUL */
 goto L8c65b1c8;
L8c65b1c8: fr[3]=fr[8]; /* fmov fr8,fr3 */
 goto L8c65b1ca;
L8c65b1ca: status=sa_sh4_multiply(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status; /* fmul fr5,fr3 */
 goto L8c65b1cc;
L8c65b1cc: fr[7]=fr[1]; /* fmov fr1,fr7 */
 goto L8c65b1ce;
L8c65b1ce: r[4]=*fpul; /* sts FPUL,r4 */
 goto L8c65b1d0;
L8c65b1d0: status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr1,fr3 */
 goto L8c65b1d2;
L8c65b1d2: fr[0]=fr[10]; /* fmov fr10,fr0 */
 goto L8c65b1d4;
L8c65b1d4: r[4]>>=16; /* shlr16 r4 */
 goto L8c65b1d6;
L8c65b1d6: wr(m,r[6],r[4],2); /* mov.w r4,@r6 */
 goto L8c65b1d8;
L8c65b1d8: r[6]+=2u; /* add #0x2,r6 */
 goto L8c65b1da;
L8c65b1da: fr[4]=rd(m,r[0]+r[1],4); /* fmov.s @(r0,r1),fr4 */
 goto L8c65b1dc;
L8c65b1dc: status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr4,fr3 */
 goto L8c65b1de;
L8c65b1de: fr[4]=fr[3]; /* fmov fr3,fr4 */
 goto L8c65b1e0;
L8c65b1e0: status=sa_sh4_truncate_i32(fr[4],&f->fpscr,fpul);if(status)return status; /* ftrc fr4,FPUL */
 goto L8c65b1e2;
L8c65b1e2: fr[6]=fr[3]; /* fmov fr3,fr6 */
 goto L8c65b1e4;
L8c65b1e4: r[4]=*fpul; /* sts FPUL,r4 */
 goto L8c65b1e6;
L8c65b1e6: r[4]>>=16; /* shlr16 r4 */
 goto L8c65b1e8;
L8c65b1e8: r[4]=(uint32_t)(int32_t)(int16_t)r[4]; /* exts.w r4,r4 */
 goto L8c65b1ea;
L8c65b1ea: wr(m,r[6],r[4],2); /* mov.w r4,@r6 */
 goto L8c65b1ec;
L8c65b1ec: r[6]+=2u; /* add #0x2,r6 */
 goto L8c65b1ee;
L8c65b1ee: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65b1f0;
L8c65b1f0: r[3]+=2u; /* add #0x2,r3 */
 goto L8c65b1f2;
L8c65b1f2: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[11]); /* cmp/ge r11,r3 */
 goto L8c65b1f4;
L8c65b1f4: take=!(*sr&1u);wr(m,r[15],r[3],4);if(take)goto L8c65b1a6;goto L8c65b1f8; /* bf/s 0x8c65b1a6 */
 goto L8c65b1f8;
L8c65b1f8: r[12]+=1u; /* add #0x1,r12 */
 goto L8c65b1fa;
L8c65b1fa: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[5]); /* cmp/ge r5,r12 */
 goto L8c65b1fc;
L8c65b1fc: if(!(*sr&1u))goto L8c65b17a; /* bf 0x8c65b17a */
 goto L8c65b1fe;
L8c65b1fe: status=sa_sh4_truncate_i32(fr[6],&f->fpscr,fpul);if(status)return status; /* ftrc fr6,FPUL */
 goto L8c65b200;
L8c65b200: r[0]=r[5]; /* mov r5,r0 */
 goto L8c65b202;
L8c65b202: r[2]=*fpul; /* sts FPUL,r2 */
 goto L8c65b204;
L8c65b204: status=sa_sh4_truncate_i32(fr[7],&f->fpscr,fpul);if(status)return status; /* ftrc fr7,FPUL */
 goto L8c65b206;
L8c65b206: r[2]>>=16; /* shlr16 r2 */
 goto L8c65b208;
L8c65b208: r[3]=*fpul; /* sts FPUL,r3 */
 goto L8c65b20a;
L8c65b20a: wr(m,r[7],r[2],2); /* mov.w r2,@r7 */
 goto L8c65b20c;
L8c65b20c: r[1]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r1 */
 goto L8c65b20e;
L8c65b20e: r[3]>>=16; /* shlr16 r3 */
 goto L8c65b210;
L8c65b210: wr(m,r[1],r[3],2); /* mov.w r3,@r1 */
 goto L8c65b212;
L8c65b212: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65b214;
L8c65b214: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c65b216;
L8c65b216: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65b218;
L8c65b218: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65b21a;
L8c65b21a: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65b21c;
L8c65b21c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65b21e;
L8c65b21e: r[13]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
