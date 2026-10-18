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
/* Retail stereo block decoder65b238..65b428.
 * 32 samples per18-byte block; signed packed nibbles and predictor history.
 * Verified recurrence,scale4,three coefficient pairs; other scales unverified. */
SA_STAGE_EXPORT int sa_sound_decode_block_stereo(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65b238;
L8c65b238: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65b23a;
L8c65b23a: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65b23c;
L8c65b23c: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65b23e;
L8c65b23e: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65b240;
L8c65b240: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65b242;
L8c65b242: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c65b244;
L8c65b244: r[15]+=4294967272u; /* add #-0x18,r15 */
 goto L8c65b246;
L8c65b246: r[3]=sx(m,r[7],2); /* mov.w @r7,r3 */
 goto L8c65b248;
L8c65b248: r[0]=0x8c65b364u; /* mova 0x8c65b364,r0 */
 goto L8c65b24a;
L8c65b24a: r[11]=rd(m,r[15]+0x34u,4); /* mov.l @(0x34,r15),r11 */
 goto L8c65b24c;
L8c65b24c: r[10]=r[4]; /* mov r4,r10 */
 goto L8c65b24e;
L8c65b24e: r[3]<<=16; /* shll16 r3 */
 goto L8c65b250;
L8c65b250: r[12]=rd(m,r[15]+0x30u,4); /* mov.l @(0x30,r15),r12 */
 goto L8c65b252;
L8c65b252: *fpul=r[3]; /* lds r3,FPUL */
 goto L8c65b254;
L8c65b254: r[3]=r[7]; /* mov r7,r3 */
 goto L8c65b256;
L8c65b256: r[3]+=2u; /* add #0x2,r3 */
 goto L8c65b258;
L8c65b258: wr(m,r[15]+0x10u,r[3],4); /* mov.l r3,@(0x10,r15) */
 goto L8c65b25a;
L8c65b25a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c65b25c;
L8c65b25c: r[2]=sx(m,r[3],2); /* mov.w @r3,r2 */
 goto L8c65b25e;
L8c65b25e: r[2]<<=16; /* shll16 r2 */
 goto L8c65b260;
L8c65b260: *fpul=r[2]; /* lds r2,FPUL */
 goto L8c65b262;
L8c65b262: r[2]=sx(m,r[11],2); /* mov.w @r11,r2 */
 goto L8c65b264;
L8c65b264: fr[9]=fr[3]; /* fmov fr3,fr9 */
 goto L8c65b266;
L8c65b266: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c65b268;
L8c65b268: r[2]<<=16; /* shll16 r2 */
 goto L8c65b26a;
L8c65b26a: *fpul=r[2]; /* lds r2,FPUL */
 goto L8c65b26c;
L8c65b26c: r[2]=r[11]; /* mov r11,r2 */
 goto L8c65b26e;
L8c65b26e: r[2]+=2u; /* add #0x2,r2 */
 goto L8c65b270;
L8c65b270: wr(m,r[15]+0x14u,r[2],4); /* mov.l r2,@(0x14,r15) */
 goto L8c65b272;
L8c65b272: r[1]=sx(m,r[2],2); /* mov.w @r2,r1 */
 goto L8c65b274;
L8c65b274: fr[2]=fr[3]; /* fmov fr3,fr2 */
 goto L8c65b276;
L8c65b276: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c65b278;
L8c65b278: r[1]<<=16; /* shll16 r1 */
 goto L8c65b27a;
L8c65b27a: fr[4]=rd(m,r[0],4); /* fmov.s @r0,fr4 */
 goto L8c65b27c;
L8c65b27c: *fpul=r[1]; /* lds r1,FPUL */
 goto L8c65b27e;
L8c65b27e: r[0]=56u; /* mov #0x38,r0 */
 goto L8c65b280;
L8c65b280: fr[10]=fr[3]; /* fmov fr3,fr10 */
 goto L8c65b282;
L8c65b282: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c65b284;
L8c65b284: fr[11]=fr[3]; /* fmov fr3,fr11 */
 goto L8c65b286;
L8c65b286: r[1]=sx(m,r[0]+r[15],2); /* mov.w @(r0,r15),r1 */
 goto L8c65b288;
L8c65b288: r[0]=60u; /* mov #0x3c,r0 */
 goto L8c65b28a;
L8c65b28a: r[8]=0u; /* mov #0x0,r8 */
 goto L8c65b28c;
L8c65b28c: r[1]<<=2; /* shll2 r1 */
 goto L8c65b28e;
L8c65b28e: r[1]<<=2; /* shll2 r1 */
 goto L8c65b290;
L8c65b290: *fpul=r[1]; /* lds r1,FPUL */
 goto L8c65b292;
L8c65b292: r[9]=32u; /* mov #0x20,r9 */
 goto L8c65b294;
L8c65b294: r[1]=sx(m,r[0]+r[15],2); /* mov.w @(r0,r15),r1 */
 goto L8c65b296;
L8c65b296: r[0]=r[8]; /* mov r8,r0 */
 goto L8c65b298;
L8c65b298: wr(m,r[15],r[8],4); /* mov.l r8,@r15 */
 goto L8c65b29a;
L8c65b29a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c65b29c;
L8c65b29c: r[1]<<=2; /* shll2 r1 */
 goto L8c65b29e;
L8c65b29e: r[1]<<=2; /* shll2 r1 */
 goto L8c65b2a0;
L8c65b2a0: *fpul=r[1]; /* lds r1,FPUL */
 goto L8c65b2a2;
L8c65b2a2: r[1]=1u; /* mov #0x1,r1 */
 goto L8c65b2a4;
L8c65b2a4: wr(m,r[15]+0x8u,r[1],4); /* mov.l r1,@(0x8,r15) */
 goto L8c65b2a6;
L8c65b2a6: r[1]=r[5]; /* mov r5,r1 */
 goto L8c65b2a8;
L8c65b2a8: *sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[1]); /* cmp/gt r1,r0 */
 goto L8c65b2aa;
L8c65b2aa: wr(m,r[15]+0x4u,r[8],4); /* mov.l r8,@(0x4,r15) */
 goto L8c65b2ac;
L8c65b2ac: fr[7]=fr[3]; /* fmov fr3,fr7 */
 goto L8c65b2ae;
L8c65b2ae: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c65b2b0;
L8c65b2b0: status=sa_sh4_divide(&fr[7],&fr[4],&f->fpscr,&fr[7]);if(status)return status; /* fdiv fr4,fr7 */
 goto L8c65b2b2;
L8c65b2b2: {uint64_t sum=(uint64_t)r[1]+r[0]+(*sr&1u);r[1]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);} /* addc r0,r1 */
 goto L8c65b2b4;
L8c65b2b4: *sr=(*sr&~1u)|(r[1]&1u);r[1]=(r[1]>>1)|(r[1]&0x80000000u); /* shar r1 */
 goto L8c65b2b6;
L8c65b2b6: fr[8]=fr[3]; /* fmov fr3,fr8 */
 goto L8c65b2b8;
L8c65b2b8: status=sa_sh4_divide(&fr[8],&fr[4],&f->fpscr,&fr[8]);if(status)return status; /* fdiv fr4,fr8 */
 goto L8c65b2ba;
L8c65b2ba: wr(m,r[15]+0xcu,r[1],4);goto L8c65b3e4; /* bra 0x8c65b3e4 */
 goto L8c65b2be;
L8c65b2be: r[4]=sx(m,r[10],2); /* mov.w @r10,r4 */
 goto L8c65b2c0;
L8c65b2c0: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c65b2c2;
L8c65b2c2: r[2]=r[4]; /* mov r4,r2 */
 goto L8c65b2c4;
L8c65b2c4: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r2 */
 goto L8c65b2c6;
L8c65b2c6: r[1]=r[4]; /* mov r4,r1 */
 goto L8c65b2c8;
L8c65b2c8: r[3]=sx(m,0x8c65b360u,2); /* mov.w 0x8c65b360,r3 */
 goto L8c65b2ca;
L8c65b2ca: r[1]<<=8; /* shll8 r1 */
 goto L8c65b2cc;
L8c65b2cc: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c65b2ce;
L8c65b2ce: r[4]=r[2]; /* mov r2,r4 */
 goto L8c65b2d0;
L8c65b2d0: r[1]&=r[3]; /* and r3,r1 */
 goto L8c65b2d2;
L8c65b2d2: r[4]|=r[1]; /* or r1,r4 */
 goto L8c65b2d4;
L8c65b2d4: r[1]=rd(m,0x8c65b368u,4); /* mov.l 0x8c65b368,r1 */
 goto L8c65b2d6;
L8c65b2d6: r[2]=(uint32_t)(int32_t)(int16_t)r[4]; /* exts.w r4,r2 */
 goto L8c65b2d8;
L8c65b2d8: *sr=(*sr&~1u)|!(r[1]&r[2]); /* tst r1,r2 */
 goto L8c65b2da;
L8c65b2da: if((*sr&1u))goto L8c65b2e0; /* bt 0x8c65b2e0 */
 goto L8c65b2dc;
L8c65b2dc: r[0]=rd(m,r[15]+0x4u,4);goto L8c65b418; /* bra 0x8c65b418 */
 goto L8c65b2e0;
L8c65b2e0: r[3]=sx(m,0x8c65b362u,2); /* mov.w 0x8c65b362,r3 */
 goto L8c65b2e2;
L8c65b2e2: r[4]=(uint32_t)(int32_t)(int16_t)r[4]; /* exts.w r4,r4 */
 goto L8c65b2e4;
L8c65b2e4: r[1]=r[8]; /* mov r8,r1 */
 goto L8c65b2e6;
L8c65b2e6: r[10]+=2u; /* add #0x2,r10 */
 goto L8c65b2e8;
L8c65b2e8: r[4]&=r[3]; /* and r3,r4 */
 goto L8c65b2ea;
L8c65b2ea: r[4]+=1u; /* add #0x1,r4 */
 goto L8c65b2ec;
L8c65b2ec: *fpul=r[4]; /* lds r4,FPUL */
 goto L8c65b2ee;
L8c65b2ee: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c65b2f0;
L8c65b2f0: fr[6]=fr[3]; /* fmov fr3,fr6 */
 goto L8c65b2f2;
L8c65b2f2: fr[3]=fr[8]; /* fmov fr8,fr3 */
 goto L8c65b2f4;
L8c65b2f4: status=sa_sh4_multiply(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status; /* fmul fr2,fr3 */
 goto L8c65b2f6;
L8c65b2f6: r[4]=sx(m,r[10],1);r[10]+=1; /* mov.b @r10+,r4 */
 goto L8c65b2f8;
L8c65b2f8: fr[0]=fr[7]; /* fmov fr7,fr0 */
 goto L8c65b2fa;
L8c65b2fa: r[0]=rd(m,0x8c65b36cu,4); /* mov.l 0x8c65b36c,r0 */
 goto L8c65b2fc;
L8c65b2fc: r[4]=r[4]&255u; /* extu.b r4,r4 */
 goto L8c65b2fe;
L8c65b2fe: status=sa_sh4_fmac(&fr[0],&fr[9],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr9,fr3 */
 goto L8c65b300;
L8c65b300: r[4]<<=2; /* shll2 r4 */
 goto L8c65b302;
L8c65b302: fr[4]=rd(m,r[0]+r[4],4); /* fmov.s @(r0,r4),fr4 */
 goto L8c65b304;
L8c65b304: r[14]=r[4]; /* mov r4,r14 */
 goto L8c65b306;
L8c65b306: fr[0]=fr[6]; /* fmov fr6,fr0 */
 goto L8c65b308;
L8c65b308: fr[5]=fr[9]; /* fmov fr9,fr5 */
 goto L8c65b30a;
L8c65b30a: status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr4,fr3 */
 goto L8c65b30c;
L8c65b30c: fr[0]=fr[7]; /* fmov fr7,fr0 */
 goto L8c65b30e;
L8c65b30e: r[0]=rd(m,0x8c65b370u,4); /* mov.l 0x8c65b370,r0 */
 goto L8c65b310;
L8c65b310: fr[1]=fr[3]; /* fmov fr3,fr1 */
 goto L8c65b312;
L8c65b312: status=sa_sh4_truncate_i32(fr[1],&f->fpscr,fpul);if(status)return status; /* ftrc fr1,FPUL */
 goto L8c65b314;
L8c65b314: fr[3]=fr[8]; /* fmov fr8,fr3 */
 goto L8c65b316;
L8c65b316: status=sa_sh4_multiply(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status; /* fmul fr5,fr3 */
 goto L8c65b318;
L8c65b318: fr[2]=fr[1]; /* fmov fr1,fr2 */
 goto L8c65b31a;
L8c65b31a: r[4]=*fpul; /* sts FPUL,r4 */
 goto L8c65b31c;
L8c65b31c: status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr1,fr3 */
 goto L8c65b31e;
L8c65b31e: fr[0]=fr[6]; /* fmov fr6,fr0 */
 goto L8c65b320;
L8c65b320: r[4]>>=16; /* shlr16 r4 */
 goto L8c65b322;
L8c65b322: wr(m,r[6],r[4],2); /* mov.w r4,@r6 */
 goto L8c65b324;
L8c65b324: r[6]+=2u; /* add #0x2,r6 */
 goto L8c65b326;
L8c65b326: fr[4]=rd(m,r[0]+r[14],4); /* fmov.s @(r0,r14),fr4 */
 goto L8c65b328;
L8c65b328: status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr4,fr3 */
 goto L8c65b32a;
L8c65b32a: fr[4]=fr[3]; /* fmov fr3,fr4 */
 goto L8c65b32c;
L8c65b32c: status=sa_sh4_truncate_i32(fr[4],&f->fpscr,fpul);if(status)return status; /* ftrc fr4,FPUL */
 goto L8c65b32e;
L8c65b32e: fr[9]=fr[3]; /* fmov fr3,fr9 */
 goto L8c65b330;
L8c65b330: r[4]=*fpul; /* sts FPUL,r4 */
 goto L8c65b332;
L8c65b332: r[4]>>=16; /* shlr16 r4 */
 goto L8c65b334;
L8c65b334: r[1]+=2u; /* add #0x2,r1 */
 goto L8c65b336;
L8c65b336: r[4]=(uint32_t)(int32_t)(int16_t)r[4]; /* exts.w r4,r4 */
 goto L8c65b338;
L8c65b338: *sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[9]); /* cmp/ge r9,r1 */
 goto L8c65b33a;
L8c65b33a: wr(m,r[6],r[4],2); /* mov.w r4,@r6 */
 goto L8c65b33c;
L8c65b33c: take=!(*sr&1u);r[6]+=2u;if(take)goto L8c65b2f2;goto L8c65b340; /* bf/s 0x8c65b2f2 */
 goto L8c65b340;
L8c65b340: r[4]=sx(m,r[10],2); /* mov.w @r10,r4 */
 goto L8c65b342;
L8c65b342: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c65b344;
L8c65b344: r[14]=r[4]; /* mov r4,r14 */
 goto L8c65b346;
L8c65b346: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[14]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[14]=n?(uint32_t)((int32_t)r[14]>>n):((r[14]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r14 */
 goto L8c65b348;
L8c65b348: r[2]=r[4]; /* mov r4,r2 */
 goto L8c65b34a;
L8c65b34a: r[3]=sx(m,0x8c65b360u,2); /* mov.w 0x8c65b360,r3 */
 goto L8c65b34c;
L8c65b34c: r[2]<<=8; /* shll8 r2 */
 goto L8c65b34e;
L8c65b34e: r[2]&=r[3]; /* and r3,r2 */
 goto L8c65b350;
L8c65b350: r[14]=r[14]&255u; /* extu.b r14,r14 */
 goto L8c65b352;
L8c65b352: r[14]|=r[2]; /* or r2,r14 */
 goto L8c65b354;
L8c65b354: r[2]=rd(m,0x8c65b368u,4); /* mov.l 0x8c65b368,r2 */
 goto L8c65b356;
L8c65b356: r[4]=(uint32_t)(int32_t)(int16_t)r[14]; /* exts.w r14,r4 */
 goto L8c65b358;
L8c65b358: *sr=(*sr&~1u)|!(r[4]&r[2]); /* tst r4,r2 */
 goto L8c65b35a;
L8c65b35a: if((*sr&1u))goto L8c65b374; /* bt 0x8c65b374 */
 goto L8c65b35c;
L8c65b35c: r[0]=rd(m,r[15]+0x8u,4);goto L8c65b418; /* bra 0x8c65b418 */
 goto L8c65b374;
L8c65b374: r[3]=sx(m,0x8c65b428u,2); /* mov.w 0x8c65b428,r3 */
 goto L8c65b376;
L8c65b376: r[1]=r[8]; /* mov r8,r1 */
 goto L8c65b378;
L8c65b378: r[10]+=2u; /* add #0x2,r10 */
 goto L8c65b37a;
L8c65b37a: r[4]&=r[3]; /* and r3,r4 */
 goto L8c65b37c;
L8c65b37c: r[4]+=1u; /* add #0x1,r4 */
 goto L8c65b37e;
L8c65b37e: *fpul=r[4]; /* lds r4,FPUL */
 goto L8c65b380;
L8c65b380: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[3]);if(status)return status; /* float FPUL,fr3 */
 goto L8c65b382;
L8c65b382: fr[6]=fr[3]; /* fmov fr3,fr6 */
 goto L8c65b384;
L8c65b384: fr[3]=fr[8]; /* fmov fr8,fr3 */
 goto L8c65b386;
L8c65b386: status=sa_sh4_multiply(&fr[3],&fr[11],&f->fpscr,&fr[3]);if(status)return status; /* fmul fr11,fr3 */
 goto L8c65b388;
L8c65b388: r[4]=sx(m,r[10],1);r[10]+=1; /* mov.b @r10+,r4 */
 goto L8c65b38a;
L8c65b38a: fr[0]=fr[7]; /* fmov fr7,fr0 */
 goto L8c65b38c;
L8c65b38c: r[0]=rd(m,0x8c65b42cu,4); /* mov.l 0x8c65b42c,r0 */
 goto L8c65b38e;
L8c65b38e: r[4]=r[4]&255u; /* extu.b r4,r4 */
 goto L8c65b390;
L8c65b390: status=sa_sh4_fmac(&fr[0],&fr[10],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr10,fr3 */
 goto L8c65b392;
L8c65b392: r[4]<<=2; /* shll2 r4 */
 goto L8c65b394;
L8c65b394: fr[4]=rd(m,r[0]+r[4],4); /* fmov.s @(r0,r4),fr4 */
 goto L8c65b396;
L8c65b396: r[14]=r[4]; /* mov r4,r14 */
 goto L8c65b398;
L8c65b398: fr[0]=fr[6]; /* fmov fr6,fr0 */
 goto L8c65b39a;
L8c65b39a: fr[5]=fr[10]; /* fmov fr10,fr5 */
 goto L8c65b39c;
L8c65b39c: status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr4,fr3 */
 goto L8c65b39e;
L8c65b39e: fr[0]=fr[7]; /* fmov fr7,fr0 */
 goto L8c65b3a0;
L8c65b3a0: r[0]=rd(m,0x8c65b430u,4); /* mov.l 0x8c65b430,r0 */
 goto L8c65b3a2;
L8c65b3a2: fr[1]=fr[3]; /* fmov fr3,fr1 */
 goto L8c65b3a4;
L8c65b3a4: status=sa_sh4_truncate_i32(fr[1],&f->fpscr,fpul);if(status)return status; /* ftrc fr1,FPUL */
 goto L8c65b3a6;
L8c65b3a6: fr[3]=fr[8]; /* fmov fr8,fr3 */
 goto L8c65b3a8;
L8c65b3a8: status=sa_sh4_multiply(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status; /* fmul fr5,fr3 */
 goto L8c65b3aa;
L8c65b3aa: fr[11]=fr[1]; /* fmov fr1,fr11 */
 goto L8c65b3ac;
L8c65b3ac: r[4]=*fpul; /* sts FPUL,r4 */
 goto L8c65b3ae;
L8c65b3ae: status=sa_sh4_fmac(&fr[0],&fr[1],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr1,fr3 */
 goto L8c65b3b0;
L8c65b3b0: fr[0]=fr[6]; /* fmov fr6,fr0 */
 goto L8c65b3b2;
L8c65b3b2: r[4]>>=16; /* shlr16 r4 */
 goto L8c65b3b4;
L8c65b3b4: wr(m,r[12],r[4],2); /* mov.w r4,@r12 */
 goto L8c65b3b6;
L8c65b3b6: r[12]+=2u; /* add #0x2,r12 */
 goto L8c65b3b8;
L8c65b3b8: fr[4]=rd(m,r[0]+r[14],4); /* fmov.s @(r0,r14),fr4 */
 goto L8c65b3ba;
L8c65b3ba: status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr4,fr3 */
 goto L8c65b3bc;
L8c65b3bc: fr[4]=fr[3]; /* fmov fr3,fr4 */
 goto L8c65b3be;
L8c65b3be: status=sa_sh4_truncate_i32(fr[4],&f->fpscr,fpul);if(status)return status; /* ftrc fr4,FPUL */
 goto L8c65b3c0;
L8c65b3c0: fr[10]=fr[3]; /* fmov fr3,fr10 */
 goto L8c65b3c2;
L8c65b3c2: r[4]=*fpul; /* sts FPUL,r4 */
 goto L8c65b3c4;
L8c65b3c4: r[4]>>=16; /* shlr16 r4 */
 goto L8c65b3c6;
L8c65b3c6: r[1]+=2u; /* add #0x2,r1 */
 goto L8c65b3c8;
L8c65b3c8: r[4]=(uint32_t)(int32_t)(int16_t)r[4]; /* exts.w r4,r4 */
 goto L8c65b3ca;
L8c65b3ca: *sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[9]); /* cmp/ge r9,r1 */
 goto L8c65b3cc;
L8c65b3cc: wr(m,r[12],r[4],2); /* mov.w r4,@r12 */
 goto L8c65b3ce;
L8c65b3ce: take=!(*sr&1u);r[12]+=2u;if(take)goto L8c65b384;goto L8c65b3d2; /* bf/s 0x8c65b384 */
 goto L8c65b3d2;
L8c65b3d2: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c65b3d4;
L8c65b3d4: r[2]+=1u; /* add #0x1,r2 */
 goto L8c65b3d6;
L8c65b3d6: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c65b3d8;
L8c65b3d8: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c65b3da;
L8c65b3da: r[3]+=2u; /* add #0x2,r3 */
 goto L8c65b3dc;
L8c65b3dc: wr(m,r[15]+0x8u,r[3],4); /* mov.l r3,@(0x8,r15) */
 goto L8c65b3de;
L8c65b3de: r[1]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r1 */
 goto L8c65b3e0;
L8c65b3e0: r[1]+=2u; /* add #0x2,r1 */
 goto L8c65b3e2;
L8c65b3e2: wr(m,r[15]+0x4u,r[1],4); /* mov.l r1,@(0x4,r15) */
 goto L8c65b3e4;
L8c65b3e4: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c65b3e6;
L8c65b3e6: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c65b3e8;
L8c65b3e8: *sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]); /* cmp/ge r3,r2 */
 goto L8c65b3ea;
L8c65b3ea: if((*sr&1u))goto L8c65b3f0; /* bt 0x8c65b3f0 */
 goto L8c65b3ec;
L8c65b3ec: goto L8c65b2be; /* bra 0x8c65b2be */
 goto L8c65b3f0;
L8c65b3f0: status=sa_sh4_truncate_i32(fr[9],&f->fpscr,fpul);if(status)return status; /* ftrc fr9,FPUL */
 goto L8c65b3f2;
L8c65b3f2: r[0]=r[5]; /* mov r5,r0 */
 goto L8c65b3f4;
L8c65b3f4: r[3]=*fpul; /* sts FPUL,r3 */
 goto L8c65b3f6;
L8c65b3f6: status=sa_sh4_truncate_i32(fr[2],&f->fpscr,fpul);if(status)return status; /* ftrc fr2,FPUL */
 goto L8c65b3f8;
L8c65b3f8: r[3]>>=16; /* shlr16 r3 */
 goto L8c65b3fa;
L8c65b3fa: wr(m,r[7],r[3],2); /* mov.w r3,@r7 */
 goto L8c65b3fc;
L8c65b3fc: r[3]=*fpul; /* sts FPUL,r3 */
 goto L8c65b3fe;
L8c65b3fe: status=sa_sh4_truncate_i32(fr[10],&f->fpscr,fpul);if(status)return status; /* ftrc fr10,FPUL */
 goto L8c65b400;
L8c65b400: r[1]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r1 */
 goto L8c65b402;
L8c65b402: r[3]>>=16; /* shlr16 r3 */
 goto L8c65b404;
L8c65b404: wr(m,r[1],r[3],2); /* mov.w r3,@r1 */
 goto L8c65b406;
L8c65b406: r[1]=*fpul; /* sts FPUL,r1 */
 goto L8c65b408;
L8c65b408: status=sa_sh4_truncate_i32(fr[11],&f->fpscr,fpul);if(status)return status; /* ftrc fr11,FPUL */
 goto L8c65b40a;
L8c65b40a: r[1]>>=16; /* shlr16 r1 */
 goto L8c65b40c;
L8c65b40c: r[3]=*fpul; /* sts FPUL,r3 */
 goto L8c65b40e;
L8c65b40e: r[1]=(uint32_t)(int32_t)(int16_t)r[1]; /* exts.w r1,r1 */
 goto L8c65b410;
L8c65b410: wr(m,r[11],r[1],2); /* mov.w r1,@r11 */
 goto L8c65b412;
L8c65b412: r[2]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r2 */
 goto L8c65b414;
L8c65b414: r[3]>>=16; /* shlr16 r3 */
 goto L8c65b416;
L8c65b416: wr(m,r[2],r[3],2); /* mov.w r3,@r2 */
 goto L8c65b418;
L8c65b418: r[15]+=24u; /* add #0x18,r15 */
 goto L8c65b41a;
L8c65b41a: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c65b41c;
L8c65b41c: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65b41e;
L8c65b41e: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65b420;
L8c65b420: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65b422;
L8c65b422: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65b424;
L8c65b424: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
