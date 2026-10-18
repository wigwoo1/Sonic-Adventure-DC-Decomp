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
/* Retail stream completion667978..667a4a. Verified within the scope of
 * analysis/sound_stream_decode_complete_verification.json; result getter
 * and downstream compaction are modeled in that comparison. */
SA_STAGE_EXPORT int sa_sound_stream_decode_complete(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c667978;
L8c667978: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66797a;
L8c66797a: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66797c;
L8c66797c: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66797e;
L8c66797e: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c667980;
L8c667980: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c667982;
L8c667982: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c667984;
L8c667984: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c667986;
L8c667986: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c667988;
L8c667988: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c66798a;
L8c66798a: r[14]=r[4]; /* mov r4,r14 */
 goto L8c66798c;
L8c66798c: r[14]+=80u; /* add #0x50,r14 */
 goto L8c66798e;
L8c66798e: r[11]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r11 */
 goto L8c667990;
L8c667990: r[12]=r[4]; /* mov r4,r12 */
 goto L8c667992;
L8c667992: r[0]=64u; /* mov #0x40,r0 */
 goto L8c667994;
L8c667994: r[3]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r3 */
 goto L8c667996;
L8c667996: r[5]=r[11]; /* mov r11,r5 */
 goto L8c667998;
L8c667998: wr(m,r[15]+0x8u,r[3],4); /* mov.l r3,@(0x8,r15) */
 goto L8c66799a;
L8c66799a: r[4]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r4 */
 goto L8c66799c;
L8c66799c: r[10]=rd(m,r[12]+r[0],4); /* mov.l @(r0,r12),r10 */
 goto L8c66799e;
L8c66799e: r[0]=68u; /* mov #0x44,r0 */
 goto L8c6679a0;
L8c6679a0: r[5]+=r[4]; /* add r4,r5 */
 goto L8c6679a2;
L8c6679a2: r[3]=rd(m,0x8c667a50u,4); /* mov.l 0x8c667a50,r3 */
 goto L8c6679a4;
L8c6679a4: r[5]+=4294967295u; /* add #-0x1,r5 */
 goto L8c6679a6;
L8c6679a6: r[13]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r13 */
 goto L8c6679a8;
L8c6679a8: r[9]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r9 */
 goto L8c6679aa;
L8c6679aa: r[1]=r[5]; /* mov r5,r1 */
 goto L8c6679ac;
L8c6679ac: r[8]=rd(m,r[12]+r[0],4); /* mov.l @(r0,r12),r8 */
 goto L8c6679ae;
L8c6679ae: target=r[3];*pr=0x8c6679b2u;r[0]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6679b2;
L8c6679b2: r[3]=rd(m,0x8c667a54u,4); /* mov.l 0x8c667a54,r3 */
 goto L8c6679b4;
L8c6679b4: r[1]=r[5]; /* mov r5,r1 */
 goto L8c6679b6;
L8c6679b6: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c6679b8;
L8c6679b8: target=r[3];*pr=0x8c6679bcu;r[0]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6679bc;
L8c6679bc: r[2]=r[11]; /* mov r11,r2 */
 goto L8c6679be;
L8c6679be: r[2]-=r[0]; /* sub r0,r2 */
 goto L8c6679c0;
L8c6679c0: r[3]=rd(m,0x8c667a58u,4); /* mov.l 0x8c667a58,r3 */
 goto L8c6679c2;
L8c6679c2: r[2]+=4294967295u; /* add #-0x1,r2 */
 goto L8c6679c4;
L8c6679c4: wr(m,r[15]+0x4u,r[2],4); /* mov.l r2,@(0x4,r15) */
 goto L8c6679c6;
L8c6679c6: target=r[3];*pr=0x8c6679cau;r[4]=rd(m,r[12]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6679ca;
L8c6679ca: r[5]=r[0]; /* mov r0,r5 */
 goto L8c6679cc;
L8c6679cc: r[3]=rd(m,0x8c667a50u,4); /* mov.l 0x8c667a50,r3 */
 goto L8c6679ce;
L8c6679ce: *macl=r[5]*r[11]; /* mul.l r5,r11 */
 goto L8c6679d0;
L8c6679d0: r[1]=*macl; /* sts MACL,r1 */
 goto L8c6679d2;
L8c6679d2: target=r[3];*pr=0x8c6679d6u;r[0]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6679d6;
L8c6679d6: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c6679d8;
L8c6679d8: r[4]=r[0]; /* mov r0,r4 */
 goto L8c6679da;
L8c6679da: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[5]); /* cmp/gt r5,r2 */
 goto L8c6679dc;
L8c6679dc: if((*sr&1u))goto L8c6679e2; /* bt 0x8c6679e2 */
 goto L8c6679de;
L8c6679de: r[1]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r1 */
 goto L8c6679e0;
L8c6679e0: r[4]-=r[1]; /* sub r1,r4 */
 goto L8c6679e2;
L8c6679e2: r[0]=sx(m,0x8c667a4au,2); /* mov.w 0x8c667a4a,r0 */
 goto L8c6679e4;
L8c6679e4: r[13]+=r[4]; /* add r4,r13 */
 goto L8c6679e6;
L8c6679e6: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[10]); /* cmp/ge r10,r13 */
 goto L8c6679e8;
L8c6679e8: wr(m,r[12]+r[0],r[4],4); /* mov.l r4,@(r0,r12) */
 goto L8c6679ea;
L8c6679ea: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c6679ec;
L8c6679ec: r[0]=sx(m,0x8c667a4cu,2); /* mov.w 0x8c667a4c,r0 */
 goto L8c6679ee;
L8c6679ee: *macl=r[5]*r[3]; /* mul.l r5,r3 */
 goto L8c6679f0;
L8c6679f0: r[5]=*macl; /* sts MACL,r5 */
 goto L8c6679f2;
L8c6679f2: take=!(*sr&1u);wr(m,r[12]+r[0],r[5],4);if(take)goto L8c667a36;goto L8c6679f6; /* bf/s 0x8c667a36 */
 goto L8c6679f6;
L8c6679f6: r[0]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r0 */
 goto L8c6679f8;
L8c6679f8: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c6679fa;
L8c6679fa: take=!(*sr&1u);r[13]-=r[10];if(take)goto L8c667a1a;goto L8c6679fe; /* bf/s 0x8c667a1a */
 goto L8c6679fe;
L8c6679fe: r[5]=r[10]; /* mov r10,r5 */
 goto L8c667a00;
L8c667a00: r[4]=r[9]; /* mov r9,r4 */
 goto L8c667a02;
L8c667a02: r[7]=r[13]; /* mov r13,r7 */
 goto L8c667a04;
L8c667a04: r[6]=r[8]; /* mov r8,r6 */
 goto L8c667a06;
L8c667a06: r[15]+=12u; /* add #0xc,r15 */
 goto L8c667a08;
L8c667a08: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c667a0a;
L8c667a0a: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c667a0c;
L8c667a0c: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c667a0e;
L8c667a0e: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c667a10;
L8c667a10: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c667a12;
L8c667a12: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c667a14;
L8c667a14: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c667a16;
L8c667a16: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c66792cu,m,f,sr,r,pr); /* bra 0x8c66792c */
 goto L8c667a1a;
L8c667a1a: r[5]=r[10]; /* mov r10,r5 */
 goto L8c667a1c;
L8c667a1c: r[4]=r[9]; /* mov r9,r4 */
 goto L8c667a1e;
L8c667a1e: r[7]=r[13]; /* mov r13,r7 */
 goto L8c667a20;
L8c667a20: r[6]=r[8]; /* mov r8,r6 */
 goto L8c667a22;
L8c667a22: r[15]+=12u; /* add #0xc,r15 */
 goto L8c667a24;
L8c667a24: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c667a26;
L8c667a26: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c667a28;
L8c667a28: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c667a2a;
L8c667a2a: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c667a2c;
L8c667a2c: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c667a2e;
L8c667a2e: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c667a30;
L8c667a30: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c667a32;
L8c667a32: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c667960u,m,f,sr,r,pr); /* bra 0x8c667960 */
 goto L8c667a36;
L8c667a36: r[15]+=12u; /* add #0xc,r15 */
 goto L8c667a38;
L8c667a38: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c667a3a;
L8c667a3a: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c667a3c;
L8c667a3c: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c667a3e;
L8c667a3e: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c667a40;
L8c667a40: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c667a42;
L8c667a42: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c667a44;
L8c667a44: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c667a46;
L8c667a46: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
