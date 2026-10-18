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
/* Retail higher-level stream completion65ba22..65bb4e, explicit SH4 ABI.
 * Caps published samples by signed remaining total; negative values preserved.
 * Getters,range splitting,virtual commits and acknowledgment remain services.
 * Verified scope: stream_work_complete_verification.json. */
SA_STAGE_EXPORT int sa_sound_stream_work_complete(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65ba22;
L8c65ba22: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65ba24;
L8c65ba24: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65ba26;
L8c65ba26: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65ba28;
L8c65ba28: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65ba2a;
L8c65ba2a: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65ba2c;
L8c65ba2c: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65ba2e;
L8c65ba2e: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c65ba30;
L8c65ba30: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65ba32;
L8c65ba32: r[15]+=4294967272u; /* add #-0x18,r15 */
 goto L8c65ba34;
L8c65ba34: r[14]=r[4]; /* mov r4,r14 */
 goto L8c65ba36;
L8c65ba36: r[3]=rd(m,0x8c65bbb4u,4); /* mov.l 0x8c65bbb4,r3 */
 goto L8c65ba38;
L8c65ba38: r[13]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r13 */
 goto L8c65ba3a;
L8c65ba3a: r[12]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r12 */
 goto L8c65ba3c;
L8c65ba3c: target=r[3];*pr=0x8c65ba40u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65ba40;
L8c65ba40: r[3]=rd(m,0x8c65bbb8u,4); /* mov.l 0x8c65bbb8,r3 */
 goto L8c65ba42;
L8c65ba42: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c65ba44;
L8c65ba44: target=r[3];*pr=0x8c65ba48u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65ba48;
L8c65ba48: r[2]=rd(m,0x8c65bbbcu,4); /* mov.l 0x8c65bbbc,r2 */
 goto L8c65ba4a;
L8c65ba4a: r[10]=r[0]; /* mov r0,r10 */
 goto L8c65ba4c;
L8c65ba4c: target=r[2];*pr=0x8c65ba50u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65ba50;
L8c65ba50: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c65ba52;
L8c65ba52: r[4]=r[0]; /* mov r0,r4 */
 goto L8c65ba54;
L8c65ba54: r[3]=rd(m,r[14]+0x34u,4); /* mov.l @(0x34,r14),r3 */
 goto L8c65ba56;
L8c65ba56: r[5]-=r[3]; /* sub r3,r5 */
 goto L8c65ba58;
L8c65ba58: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]); /* cmp/ge r5,r4 */
 goto L8c65ba5a;
L8c65ba5a: if((*sr&1u))goto L8c65ba60; /* bt 0x8c65ba60 */
 goto L8c65ba5c;
L8c65ba5c: r[11]=r[4];goto L8c65ba62; /* bra 0x8c65ba62 */
 goto L8c65ba60;
L8c65ba60: r[11]=r[5]; /* mov r5,r11 */
 goto L8c65ba62;
L8c65ba62: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65ba64;
L8c65ba64: r[7]=r[15]; /* mov r15,r7 */
 goto L8c65ba66;
L8c65ba66: r[9]=rd(m,0x8c65bbc0u,4); /* mov.l 0x8c65bbc0,r9 */
 goto L8c65ba68;
L8c65ba68: r[5]=r[10]; /* mov r10,r5 */
 goto L8c65ba6a;
L8c65ba6a: r[4]=r[14]; /* mov r14,r4 */
 goto L8c65ba6c;
L8c65ba6c: r[7]+=8u; /* add #0x8,r7 */
 goto L8c65ba6e;
L8c65ba6e: r[6]+=16u; /* add #0x10,r6 */
 goto L8c65ba70;
L8c65ba70: target=r[9];*pr=0x8c65ba74u;r[4]+=20u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto L8c65ba74;
L8c65ba74: r[2]=rd(m,r[13],4); /* mov.l @r13,r2 */
 goto L8c65ba76;
L8c65ba76: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65ba78;
L8c65ba78: r[5]=0u; /* mov #0x0,r5 */
 goto L8c65ba7a;
L8c65ba7a: r[3]=rd(m,r[2]+0x20u,4); /* mov.l @(0x20,r2),r3 */
 goto L8c65ba7c;
L8c65ba7c: r[6]+=16u; /* add #0x10,r6 */
 goto L8c65ba7e;
L8c65ba7e: target=r[3];*pr=0x8c65ba82u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65ba82;
L8c65ba82: r[2]=rd(m,r[13],4); /* mov.l @r13,r2 */
 goto L8c65ba84;
L8c65ba84: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65ba86;
L8c65ba86: r[5]=1u; /* mov #0x1,r5 */
 goto L8c65ba88;
L8c65ba88: r[3]=rd(m,r[2]+0x1cu,4); /* mov.l @(0x1c,r2),r3 */
 goto L8c65ba8a;
L8c65ba8a: r[6]+=8u; /* add #0x8,r6 */
 goto L8c65ba8c;
L8c65ba8c: target=r[3];*pr=0x8c65ba90u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65ba90;
L8c65ba90: r[2]=r[11]; /* mov r11,r2 */
 goto L8c65ba92;
L8c65ba92: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c65ba94;
L8c65ba94: r[13]=0u; /* mov #0x0,r13 */
 goto L8c65ba96;
L8c65ba96: wr(m,r[15]+0x4u,r[2],4);goto L8c65bae8; /* bra 0x8c65bae8 */
 goto L8c65ba9a;
L8c65ba9a: r[4]=r[13]; /* mov r13,r4 */
 goto L8c65ba9c;
L8c65ba9c: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c65ba9e;
L8c65ba9e: r[3]=r[14]; /* mov r14,r3 */
 goto L8c65baa0;
L8c65baa0: r[4]<<=2; /* shll2 r4 */
 goto L8c65baa2;
L8c65baa2: r[7]=r[15]; /* mov r15,r7 */
 goto L8c65baa4;
L8c65baa4: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65baa6;
L8c65baa6: r[3]+=28u; /* add #0x1c,r3 */
 goto L8c65baa8;
L8c65baa8: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c65baaa;
L8c65baaa: r[6]+=16u; /* add #0x10,r6 */
 goto L8c65baac;
L8c65baac: r[7]+=8u; /* add #0x8,r7 */
 goto L8c65baae;
L8c65baae: target=r[9];*pr=0x8c65bab2u;r[4]+=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto L8c65bab2;
L8c65bab2: r[8]=r[13]; /* mov r13,r8 */
 goto L8c65bab4;
L8c65bab4: r[3]=r[14]; /* mov r14,r3 */
 goto L8c65bab6;
L8c65bab6: r[8]<<=2; /* shll2 r8 */
 goto L8c65bab8;
L8c65bab8: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65baba;
L8c65baba: r[3]+=12u; /* add #0xc,r3 */
 goto L8c65babc;
L8c65babc: wr(m,r[15],r[8],4); /* mov.l r8,@r15 */
 goto L8c65babe;
L8c65babe: r[8]+=r[3]; /* add r3,r8 */
 goto L8c65bac0;
L8c65bac0: r[8]=rd(m,r[8],4); /* mov.l @r8,r8 */
 goto L8c65bac2;
L8c65bac2: r[5]=1u; /* mov #0x1,r5 */
 goto L8c65bac4;
L8c65bac4: r[6]+=16u; /* add #0x10,r6 */
 goto L8c65bac6;
L8c65bac6: r[2]=rd(m,r[8],4); /* mov.l @r8,r2 */
 goto L8c65bac8;
L8c65bac8: r[3]=rd(m,r[2]+0x20u,4); /* mov.l @(0x20,r2),r3 */
 goto L8c65baca;
L8c65baca: target=r[3];*pr=0x8c65baceu;r[4]=r[8];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65bace;
L8c65bace: r[8]=r[14]; /* mov r14,r8 */
 goto L8c65bad0;
L8c65bad0: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65bad2;
L8c65bad2: r[8]+=12u; /* add #0xc,r8 */
 goto L8c65bad4;
L8c65bad4: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65bad6;
L8c65bad6: r[8]+=r[3]; /* add r3,r8 */
 goto L8c65bad8;
L8c65bad8: r[8]=rd(m,r[8],4); /* mov.l @r8,r8 */
 goto L8c65bada;
L8c65bada: r[5]=0u; /* mov #0x0,r5 */
 goto L8c65badc;
L8c65badc: r[6]+=8u; /* add #0x8,r6 */
 goto L8c65bade;
L8c65bade: r[3]=rd(m,r[8],4); /* mov.l @r8,r3 */
 goto L8c65bae0;
L8c65bae0: r[2]=rd(m,r[3]+0x1cu,4); /* mov.l @(0x1c,r3),r2 */
 goto L8c65bae2;
L8c65bae2: target=r[2];*pr=0x8c65bae6u;r[4]=r[8];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65bae6;
L8c65bae6: r[13]+=1u; /* add #0x1,r13 */
 goto L8c65bae8;
L8c65bae8: r[3]=rd(m,0x8c65bbc4u,4); /* mov.l 0x8c65bbc4,r3 */
 goto L8c65baea;
L8c65baea: target=r[3];*pr=0x8c65baeeu;r[4]=rd(m,r[14]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65baee;
L8c65baee: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[0]); /* cmp/ge r0,r13 */
 goto L8c65baf0;
L8c65baf0: if(!(*sr&1u))goto L8c65ba9a; /* bf 0x8c65ba9a */
 goto L8c65baf2;
L8c65baf2: r[3]=rd(m,r[14]+0x2cu,4); /* mov.l @(0x2c,r14),r3 */
 goto L8c65baf4;
L8c65baf4: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65baf6;
L8c65baf6: r[3]+=r[11]; /* add r11,r3 */
 goto L8c65baf8;
L8c65baf8: wr(m,r[14]+0x2cu,r[3],4); /* mov.l r3,@(0x2c,r14) */
 goto L8c65bafa;
L8c65bafa: r[2]=rd(m,r[14]+0x30u,4); /* mov.l @(0x30,r14),r2 */
 goto L8c65bafc;
L8c65bafc: r[2]+=r[10]; /* add r10,r2 */
 goto L8c65bafe;
L8c65bafe: wr(m,r[14]+0x30u,r[2],4); /* mov.l r2,@(0x30,r14) */
 goto L8c65bb00;
L8c65bb00: r[3]=rd(m,r[14]+0x34u,4); /* mov.l @(0x34,r14),r3 */
 goto L8c65bb02;
L8c65bb02: r[3]+=r[11]; /* add r11,r3 */
 goto L8c65bb04;
L8c65bb04: wr(m,r[14]+0x34u,r[3],4); /* mov.l r3,@(0x34,r14) */
 goto L8c65bb06;
L8c65bb06: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c65bb08;
L8c65bb08: r[2]+=r[11]; /* add r11,r2 */
 goto L8c65bb0a;
L8c65bb0a: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c65bb0c;
L8c65bb0c: r[0]=68u; /* mov #0x44,r0 */
 goto L8c65bb0e;
L8c65bb0e: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c65bb10;
L8c65bb10: r[3]+=r[10]; /* add r10,r3 */
 goto L8c65bb12;
L8c65bb12: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c65bb14;
L8c65bb14: r[2]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r2 */
 goto L8c65bb16;
L8c65bb16: *sr=(*sr&~1u)|((int32_t)r[2]>=0); /* cmp/pz r2 */
 goto L8c65bb18;
L8c65bb18: if(!(*sr&1u))goto L8c65bb34; /* bf 0x8c65bb34 */
 goto L8c65bb1a;
L8c65bb1a: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65bb1c;
L8c65bb1c: r[1]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r1 */
 goto L8c65bb1e;
L8c65bb1e: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c65bb20;
L8c65bb20: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[1]); /* cmp/ge r1,r3 */
 goto L8c65bb22;
L8c65bb22: if(!(*sr&1u))goto L8c65bb34; /* bf 0x8c65bb34 */
 goto L8c65bb24;
L8c65bb24: r[0]=72u; /* mov #0x48,r0 */
 goto L8c65bb26;
L8c65bb26: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c65bb28;
L8c65bb28: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65bb2a;
L8c65bb2a: if((*sr&1u))goto L8c65bb34; /* bt 0x8c65bb34 */
 goto L8c65bb2c;
L8c65bb2c: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c65bb2e;
L8c65bb2e: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c65bb30;
L8c65bb30: target=r[2];*pr=0x8c65bb34u;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65bb34;
L8c65bb34: r[3]=rd(m,0x8c65bbc8u,4); /* mov.l 0x8c65bbc8,r3 */
 goto L8c65bb36;
L8c65bb36: target=r[3];*pr=0x8c65bb3au;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65bb3a;
L8c65bb3a: r[15]+=24u; /* add #0x18,r15 */
 goto L8c65bb3c;
L8c65bb3c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65bb3e;
L8c65bb3e: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c65bb40;
L8c65bb40: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65bb42;
L8c65bb42: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65bb44;
L8c65bb44: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65bb46;
L8c65bb46: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65bb48;
L8c65bb48: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65bb4a;
L8c65bb4a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
