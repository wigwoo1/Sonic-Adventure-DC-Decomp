#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
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
/* PARTIALLY VERIFIED: retail buffer setup 8c665fa8..8c6666c0.
 * 638 reachable instructions; startup zero-transfer path compared.
 * Nonzero tile path compared across 16 fixtures; alternative modes
 * and actual transfer hardware remain unverified. */
SA_STAGE_EXPORT int sa_engine_render_buffer_setup(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c665fa8;
L8c665fa8: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c665faa;
L8c665faa: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c665fac;
L8c665fac: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c665fae;
L8c665fae: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c665fb0;
L8c665fb0: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c665fb2;
L8c665fb2: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c665fb4;
L8c665fb4: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c665fb6;
L8c665fb6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c665fb8;
L8c665fb8: r[0]=sx(m,0x8c665fe4u,2); /* mov.w 0x8c665fe4,r0 */
 goto L8c665fba;
L8c665fba: r[15]+=r[0]; /* add r0,r15 */
 goto L8c665fbc;
L8c665fbc: r[8]=0u; /* mov #0x0,r8 */
 goto L8c665fbe;
L8c665fbe: r[3]=rd(m,0x8c66601cu,4); /* mov.l 0x8c66601c,r3 */
 goto L8c665fc0;
L8c665fc0: r[0]=80u; /* mov #0x50,r0 */
 goto L8c665fc2;
L8c665fc2: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c665fc4;
L8c665fc4: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c665fc6;
L8c665fc6: wr(m,r[15]+0x8u,r[6],4); /* mov.l r6,@(0x8,r15) */
 goto L8c665fc8;
L8c665fc8: wr(m,r[15]+r[0],r[8],4); /* mov.l r8,@(r0,r15) */
 goto L8c665fca;
L8c665fca: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c665fcc;
L8c665fcc: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c665fce;
L8c665fce: if(!(*sr&1u))goto L8c666020; /* bf 0x8c666020 */
 goto L8c665fd0;
L8c665fd0: r[12]=1u;goto L8c666022; /* bra 0x8c666022 */
 goto L8c666020;
L8c666020: r[12]=r[8]; /* mov r8,r12 */
 goto L8c666022;
L8c666022: r[2]=rd(m,0x8c6660c4u,4); /* mov.l 0x8c6660c4,r2 */
 goto L8c666024;
L8c666024: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c666026;
L8c666026: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c666028;
L8c666028: if(!(*sr&1u))goto L8c666034; /* bf 0x8c666034 */
 goto L8c66602a;
L8c66602a: r[0]=sx(m,0x8c6660c0u,2); /* mov.w 0x8c6660c0,r0 */
 goto L8c66602c;
L8c66602c: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c66602e;
L8c66602e: r[6]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r6 */
 goto L8c666030;
L8c666030: *pr=0x8c666034u;r[4]=rd(m,r[15],4);status=call(ctx,0x8c665e3cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c665e3c */
 goto L8c666034;
L8c666034: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c666036;
L8c666036: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c666038;
L8c666038: r[0]=sx(m,0x8c6660c0u,2); /* mov.w 0x8c6660c0,r0 */
 goto L8c66603a;
L8c66603a: *macl=r[3]*r[2]; /* mul.l r3,r2 */
 goto L8c66603c;
L8c66603c: r[3]=*macl; /* sts MACL,r3 */
 goto L8c66603e;
L8c66603e: r[14]=r[3]; /* mov r3,r14 */
 goto L8c666040;
L8c666040: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1; /* shll r14 */
 goto L8c666042;
L8c666042: r[1]=r[3]; /* mov r3,r1 */
 goto L8c666044;
L8c666044: r[14]+=r[1]; /* add r1,r14 */
 goto L8c666046;
L8c666046: wr(m,r[15]+0x4u,r[3],4); /* mov.l r3,@(0x4,r15) */
 goto L8c666048;
L8c666048: r[14]<<=2; /* shll2 r14 */
 goto L8c66604a;
L8c66604a: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c66604c;
L8c66604c: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1; /* shll r14 */
 goto L8c66604e;
L8c66604e: r[13]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r13 */
 goto L8c666050;
L8c666050: *macl=r[14]*r[3]; /* mul.l r14,r3 */
 goto L8c666052;
L8c666052: r[13]<<=2; /* shll2 r13 */
 goto L8c666054;
L8c666054: r[0]=rd(m,0x8c6660c8u,4); /* mov.l 0x8c6660c8,r0 */
 goto L8c666056;
L8c666056: r[13]<<=2; /* shll2 r13 */
 goto L8c666058;
L8c666058: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1; /* shll r13 */
 goto L8c66605a;
L8c66605a: r[3]=*macl; /* sts MACL,r3 */
 goto L8c66605c;
L8c66605c: r[2]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r2 */
 goto L8c66605e;
L8c66605e: r[3]+=r[2]; /* add r2,r3 */
 goto L8c666060;
L8c666060: r[2]=rd(m,0x8c6660ccu,4); /* mov.l 0x8c6660cc,r2 */
 goto L8c666062;
L8c666062: wr(m,r[15]+0xcu,r[3],4); /* mov.l r3,@(0xc,r15) */
 goto L8c666064;
L8c666064: r[0]=rd(m,r[2],4); /* mov.l @r2,r0 */
 goto L8c666066;
L8c666066: *sr=(*sr&~1u)|!(r[0]&0x20u); /* tst #0x20,r0 */
 goto L8c666068;
L8c666068: if((*sr&1u))goto L8c6660a2; /* bt 0x8c6660a2 */
 goto L8c66606a;
L8c66606a: r[0]=rd(m,0x8c6660d0u,4); /* mov.l 0x8c6660d0,r0 */
 goto L8c66606c;
L8c66606c: r[1]=rd(m,r[0],4); /* mov.l @r0,r1 */
 goto L8c66606e;
L8c66606e: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c666070;
L8c666070: if((*sr&1u))goto L8c6660a2; /* bt 0x8c6660a2 */
 goto L8c666072;
L8c666072: r[0]=rd(m,0x8c6660d4u,4); /* mov.l 0x8c6660d4,r0 */
 goto L8c666074;
L8c666074: r[5]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r5 */
 goto L8c666076;
L8c666076: r[4]=r[0]; /* mov r0,r4 */
 goto L8c666078;
L8c666078: r[14]<<=2; /* shll2 r14 */
 goto L8c66607a;
L8c66607a: r[4]+=r[13]; /* add r13,r4 */
 goto L8c66607c;
L8c66607c: r[4]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r4 */
 goto L8c66607e;
L8c66607e: *pr=0x8c666082u;r[4]+=r[14];status=call(ctx,0x8c665f42u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c665f42 */
 goto L8c666082;
L8c666082: r[2]=rd(m,0x8c6660d8u,4); /* mov.l 0x8c6660d8,r2 */
 goto L8c666084;
L8c666084: wr(m,r[15]+0x14u,r[0],4); /* mov.l r0,@(0x14,r15) */
 goto L8c666086;
L8c666086: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c666088;
L8c666088: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c66608a;
L8c66608a: if(!(*sr&1u))goto L8c66609c; /* bf 0x8c66609c */
 goto L8c66608c;
L8c66608c: r[1]=rd(m,0x8c6660dcu,4); /* mov.l 0x8c6660dc,r1 */
 goto L8c66608e;
L8c66608e: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c666090;
L8c666090: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c666092;
L8c666092: if((*sr&1u))goto L8c66609c; /* bt 0x8c66609c */
 goto L8c666094;
L8c666094: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c666096;
L8c666096: if((*sr&1u))goto L8c66609c; /* bt 0x8c66609c */
 goto L8c666098;
L8c666098: r[2]=1u;goto L8c66609e; /* bra 0x8c66609e */
 goto L8c66609c;
L8c66609c: r[2]=0u; /* mov #0x0,r2 */
 goto L8c66609e;
L8c66609e: r[0]=80u; /* mov #0x50,r0 */
 goto L8c6660a0;
L8c6660a0: wr(m,r[15]+r[0],r[2],4); /* mov.l r2,@(r0,r15) */
 goto L8c6660a2;
L8c6660a2: r[2]=rd(m,0x8c6660d8u,4); /* mov.l 0x8c6660d8,r2 */
 goto L8c6660a4;
L8c6660a4: r[14]=rd(m,0x8c6660e0u,4); /* mov.l 0x8c6660e0,r14 */
 goto L8c6660a6;
L8c6660a6: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c6660a8;
L8c6660a8: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c6660aa;
L8c6660aa: if(!(*sr&1u))goto L8c666100; /* bf 0x8c666100 */
 goto L8c6660ac;
L8c6660ac: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c6660ae;
L8c6660ae: wr(m,r[15]+r[0],r[14],4);goto L8c666104; /* bra 0x8c666104 */
 goto L8c666100;
L8c666100: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c666102;
L8c666102: wr(m,r[15]+r[0],r[8],4); /* mov.l r8,@(r0,r15) */
 goto L8c666104;
L8c666104: r[2]=rd(m,0x8c666160u,4); /* mov.l 0x8c666160,r2 */
 goto L8c666106;
L8c666106: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c666108;
L8c666108: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c66610a;
L8c66610a: if(!(*sr&1u))goto L8c666112; /* bf 0x8c666112 */
 goto L8c66610c;
L8c66610c: r[0]=92u; /* mov #0x5c,r0 */
 goto L8c66610e;
L8c66610e: wr(m,r[15]+r[0],r[14],4);goto L8c666116; /* bra 0x8c666116 */
 goto L8c666112;
L8c666112: r[0]=92u; /* mov #0x5c,r0 */
 goto L8c666114;
L8c666114: wr(m,r[15]+r[0],r[8],4); /* mov.l r8,@(r0,r15) */
 goto L8c666116;
L8c666116: r[2]=rd(m,0x8c666164u,4); /* mov.l 0x8c666164,r2 */
 goto L8c666118;
L8c666118: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c66611a;
L8c66611a: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c66611c;
L8c66611c: if(!(*sr&1u))goto L8c666124; /* bf 0x8c666124 */
 goto L8c66611e;
L8c66611e: r[0]=72u; /* mov #0x48,r0 */
 goto L8c666120;
L8c666120: wr(m,r[15]+r[0],r[14],4);goto L8c666128; /* bra 0x8c666128 */
 goto L8c666124;
L8c666124: r[0]=72u; /* mov #0x48,r0 */
 goto L8c666126;
L8c666126: wr(m,r[15]+r[0],r[8],4); /* mov.l r8,@(r0,r15) */
 goto L8c666128;
L8c666128: r[2]=rd(m,0x8c666168u,4); /* mov.l 0x8c666168,r2 */
 goto L8c66612a;
L8c66612a: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c66612c;
L8c66612c: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c66612e;
L8c66612e: if(!(*sr&1u))goto L8c666136; /* bf 0x8c666136 */
 goto L8c666130;
L8c666130: r[0]=84u; /* mov #0x54,r0 */
 goto L8c666132;
L8c666132: wr(m,r[15]+r[0],r[14],4);goto L8c66613a; /* bra 0x8c66613a */
 goto L8c666136;
L8c666136: r[0]=84u; /* mov #0x54,r0 */
 goto L8c666138;
L8c666138: wr(m,r[15]+r[0],r[8],4); /* mov.l r8,@(r0,r15) */
 goto L8c66613a;
L8c66613a: r[2]=rd(m,0x8c66616cu,4); /* mov.l 0x8c66616c,r2 */
 goto L8c66613c;
L8c66613c: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c66613e;
L8c66613e: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c666140;
L8c666140: if(!(*sr&1u))goto L8c666180; /* bf 0x8c666180 */
 goto L8c666142;
L8c666142: r[0]=88u; /* mov #0x58,r0 */
 goto L8c666144;
L8c666144: wr(m,r[15]+r[0],r[14],4);goto L8c666184; /* bra 0x8c666184 */
 goto L8c666180;
L8c666180: r[0]=88u; /* mov #0x58,r0 */
 goto L8c666182;
L8c666182: wr(m,r[15]+r[0],r[8],4); /* mov.l r8,@(r0,r15) */
 goto L8c666184;
L8c666184: r[3]=rd(m,0x8c666288u,4); /* mov.l 0x8c666288,r3 */
 goto L8c666186;
L8c666186: r[4]=rd(m,0x8c666284u,4); /* mov.l 0x8c666284,r4 */
 goto L8c666188;
L8c666188: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c66618a;
L8c66618a: r[0]<<=2; /* shll2 r0 */
 goto L8c66618c;
L8c66618c: r[1]=rd(m,0x8c66628cu,4); /* mov.l 0x8c66628c,r1 */
 goto L8c66618e;
L8c66618e: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto L8c666190;
L8c666190: r[2]<<=2; /* shll2 r2 */
 goto L8c666192;
L8c666192: wr(m,r[15]+0x38u,r[2],4); /* mov.l r2,@(0x38,r15) */
 goto L8c666194;
L8c666194: r[0]=rd(m,r[1],4); /* mov.l @r1,r0 */
 goto L8c666196;
L8c666196: r[0]<<=2; /* shll2 r0 */
 goto L8c666198;
L8c666198: r[2]=rd(m,0x8c666290u,4); /* mov.l 0x8c666290,r2 */
 goto L8c66619a;
L8c66619a: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c66619c;
L8c66619c: r[0]=64u; /* mov #0x40,r0 */
 goto L8c66619e;
L8c66619e: r[3]<<=2; /* shll2 r3 */
 goto L8c6661a0;
L8c6661a0: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c6661a2;
L8c6661a2: r[0]=rd(m,r[2],4); /* mov.l @r2,r0 */
 goto L8c6661a4;
L8c6661a4: r[0]<<=2; /* shll2 r0 */
 goto L8c6661a6;
L8c6661a6: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c6661a8;
L8c6661a8: r[3]<<=2; /* shll2 r3 */
 goto L8c6661aa;
L8c6661aa: wr(m,r[15]+0x3cu,r[3],4); /* mov.l r3,@(0x3c,r15) */
 goto L8c6661ac;
L8c6661ac: r[0]=rd(m,r[1]+0x8u,4); /* mov.l @(0x8,r1),r0 */
 goto L8c6661ae;
L8c6661ae: r[0]<<=2; /* shll2 r0 */
 goto L8c6661b0;
L8c6661b0: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c6661b2;
L8c6661b2: r[2]+=8u; /* add #0x8,r2 */
 goto L8c6661b4;
L8c6661b4: r[3]<<=2; /* shll2 r3 */
 goto L8c6661b6;
L8c6661b6: wr(m,r[15]+0x30u,r[3],4); /* mov.l r3,@(0x30,r15) */
 goto L8c6661b8;
L8c6661b8: r[0]=rd(m,r[2],4); /* mov.l @r2,r0 */
 goto L8c6661ba;
L8c6661ba: r[0]<<=2; /* shll2 r0 */
 goto L8c6661bc;
L8c6661bc: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c6661be;
L8c6661be: r[0]=68u; /* mov #0x44,r0 */
 goto L8c6661c0;
L8c6661c0: r[3]<<=2; /* shll2 r3 */
 goto L8c6661c2;
L8c6661c2: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c6661c4;
L8c6661c4: r[0]=rd(m,0x8c666294u,4); /* mov.l 0x8c666294,r0 */
 goto L8c6661c6;
L8c6661c6: r[1]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r1 */
 goto L8c6661c8;
L8c6661c8: r[3]=rd(m,0x8c666288u,4); /* mov.l 0x8c666288,r3 */
 goto L8c6661ca;
L8c6661ca: wr(m,r[15]+0x1cu,r[1],4); /* mov.l r1,@(0x1c,r15) */
 goto L8c6661cc;
L8c6661cc: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c6661ce;
L8c6661ce: r[0]<<=2; /* shll2 r0 */
 goto L8c6661d0;
L8c6661d0: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c6661d2;
L8c6661d2: r[1]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r1 */
 goto L8c6661d4;
L8c6661d4: r[2]+=4294967284u; /* add #-0xc,r2 */
 goto L8c6661d6;
L8c6661d6: r[0]=rd(m,r[15]+0x1cu,4); /* mov.l @(0x1c,r15),r0 */
 goto L8c6661d8;
L8c6661d8: *macl=r[5]*r[1]; /* mul.l r5,r1 */
 goto L8c6661da;
L8c6661da: r[1]=*macl; /* sts MACL,r1 */
 goto L8c6661dc;
L8c6661dc: r[1]<<=2; /* shll2 r1 */
 goto L8c6661de;
L8c6661de: r[1]+=r[0]; /* add r0,r1 */
 goto L8c6661e0;
L8c6661e0: wr(m,r[15]+0x18u,r[1],4); /* mov.l r1,@(0x18,r15) */
 goto L8c6661e2;
L8c6661e2: r[0]=rd(m,r[2],4); /* mov.l @r2,r0 */
 goto L8c6661e4;
L8c6661e4: r[0]<<=2; /* shll2 r0 */
 goto L8c6661e6;
L8c6661e6: r[2]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r2 */
 goto L8c6661e8;
L8c6661e8: *macl=r[5]*r[2]; /* mul.l r5,r2 */
 goto L8c6661ea;
L8c6661ea: r[2]=*macl; /* sts MACL,r2 */
 goto L8c6661ec;
L8c6661ec: r[2]<<=2; /* shll2 r2 */
 goto L8c6661ee;
L8c6661ee: r[1]+=r[2]; /* add r2,r1 */
 goto L8c6661f0;
L8c6661f0: r[2]=rd(m,0x8c666290u,4); /* mov.l 0x8c666290,r2 */
 goto L8c6661f2;
L8c6661f2: wr(m,r[15]+0x24u,r[1],4); /* mov.l r1,@(0x24,r15) */
 goto L8c6661f4;
L8c6661f4: r[0]=rd(m,r[2],4); /* mov.l @r2,r0 */
 goto L8c6661f6;
L8c6661f6: r[0]<<=2; /* shll2 r0 */
 goto L8c6661f8;
L8c6661f8: r[1]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r1 */
 goto L8c6661fa;
L8c6661fa: r[0]=rd(m,r[15]+0x24u,4); /* mov.l @(0x24,r15),r0 */
 goto L8c6661fc;
L8c6661fc: *macl=r[5]*r[1]; /* mul.l r5,r1 */
 goto L8c6661fe;
L8c6661fe: r[1]=*macl; /* sts MACL,r1 */
 goto L8c666200;
L8c666200: r[1]<<=2; /* shll2 r1 */
 goto L8c666202;
L8c666202: r[1]+=r[0]; /* add r0,r1 */
 goto L8c666204;
L8c666204: wr(m,r[15]+0x20u,r[1],4); /* mov.l r1,@(0x20,r15) */
 goto L8c666206;
L8c666206: r[0]=rd(m,r[3]+0xcu,4); /* mov.l @(0xc,r3),r0 */
 goto L8c666208;
L8c666208: r[0]<<=2; /* shll2 r0 */
 goto L8c66620a;
L8c66620a: r[13]=r[8]; /* mov r8,r13 */
 goto L8c66620c;
L8c66620c: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c66620e;
L8c66620e: r[2]=29u; /* mov #0x1d,r2 */
 goto L8c666210;
L8c666210: *macl=r[5]*r[3]; /* mul.l r5,r3 */
 goto L8c666212;
L8c666212: r[3]=*macl; /* sts MACL,r3 */
 goto L8c666214;
L8c666214: r[3]<<=2; /* shll2 r3 */
 goto L8c666216;
L8c666216: r[1]+=r[3]; /* add r3,r1 */
 goto L8c666218;
L8c666218: r[3]=r[5]; /* mov r5,r3 */
 goto L8c66621a;
L8c66621a: r[3]+=4294967295u; /* add #-0x1,r3 */
 goto L8c66621c;
L8c66621c: wr(m,r[15]+0x28u,r[1],4); /* mov.l r1,@(0x28,r15) */
 goto L8c66621e;
L8c66621e: r[1]=1u; /* mov #0x1,r1 */
 goto L8c666220;
L8c666220: wr(m,r[15]+0x2cu,r[3],4); /* mov.l r3,@(0x2c,r15) */
 goto L8c666222;
L8c666222: r[3]=0u; /* mov #0x0,r3 */
 goto L8c666224;
L8c666224: r[12]&=r[1]; /* and r1,r12 */
 goto L8c666226;
L8c666226: *sr=(*sr&~1u)|(r[3]>=r[5]); /* cmp/hs r5,r3 */
 goto L8c666228;
L8c666228: r[12]=(r[2]&0x80000000u)?((r[2]&31u)?r[12]>>((0u-r[2])&31u):0u):(r[12]<<(r[2]&31u)); /* shld r2,r12 */
 goto L8c66622a;
L8c66622a: take=!(*sr&1u);wr(m,r[15]+0x34u,r[12],4);if(take)goto L8c666232;goto L8c66622e; /* bf/s 0x8c666232 */
 goto L8c66622e;
L8c66622e: goto L8c66667c; /* bra 0x8c66667c */
 goto L8c666232;
L8c666232: r[11]=rd(m,0x8c6662a0u,4); /* mov.l 0x8c6662a0,r11 */
 goto L8c666234;
L8c666234: r[12]=rd(m,0x8c6662a4u,4); /* mov.l 0x8c6662a4,r12 */
 goto L8c666236;
L8c666236: r[9]=rd(m,0x8c666298u,4); /* mov.l 0x8c666298,r9 */
 goto L8c666238;
L8c666238: r[10]=rd(m,0x8c66629cu,4); /* mov.l 0x8c66629c,r10 */
 goto L8c66623a;
L8c66623a: r[0]=sx(m,0x8c666280u,2); /* mov.w 0x8c666280,r0 */
 goto L8c66623c;
L8c66623c: r[0]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r0 */
 goto L8c66623e;
L8c66623e: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c666240;
L8c666240: take=(*sr&1u);r[4]=r[8];if(take)goto L8c66624c;goto L8c666244; /* bt/s 0x8c66624c */
 goto L8c666244;
L8c666244: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c666246;
L8c666246: if((*sr&1u))goto L8c6662c0; /* bt 0x8c6662c0 */
 goto L8c666248;
L8c666248: goto L8c6662d8; /* bra 0x8c6662d8 */
 goto L8c66624c;
L8c66624c: r[3]=rd(m,0x8c6662acu,4); /* mov.l 0x8c6662ac,r3 */
 goto L8c66624e;
L8c66624e: r[1]=r[13]; /* mov r13,r1 */
 goto L8c666250;
L8c666250: r[2]=rd(m,0x8c6662a8u,4); /* mov.l 0x8c6662a8,r2 */
 goto L8c666252;
L8c666252: target=r[3];*pr=0x8c666256u;r[0]=rd(m,r[2],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666256;
L8c666256: r[3]=r[2]; /* mov r2,r3 */
 goto L8c666258;
L8c666258: wr(m,r[15]+0x10u,r[0],4); /* mov.l r0,@(0x10,r15) */
 goto L8c66625a;
L8c66625a: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c66625c;
L8c66625c: r[3]=rd(m,0x8c6662b0u,4); /* mov.l 0x8c6662b0,r3 */
 goto L8c66625e;
L8c66625e: target=r[3];*pr=0x8c666262u;r[1]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666262;
L8c666262: goto L8c6662d6; /* bra 0x8c6662d6 */
 goto L8c6662c0;
L8c6662c0: r[2]=rd(m,0x8c6663b0u,4); /* mov.l 0x8c6663b0,r2 */
 goto L8c6662c2;
L8c6662c2: r[1]=r[13]; /* mov r13,r1 */
 goto L8c6662c4;
L8c6662c4: r[3]=rd(m,0x8c6663acu,4); /* mov.l 0x8c6663ac,r3 */
 goto L8c6662c6;
L8c6662c6: target=r[2];*pr=0x8c6662cau;r[0]=rd(m,r[3],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6662ca;
L8c6662ca: r[2]=r[3]; /* mov r3,r2 */
 goto L8c6662cc;
L8c6662cc: wr(m,r[15]+0x10u,r[0],4); /* mov.l r0,@(0x10,r15) */
 goto L8c6662ce;
L8c6662ce: r[0]=rd(m,r[2],4); /* mov.l @r2,r0 */
 goto L8c6662d0;
L8c6662d0: r[2]=rd(m,0x8c6663b4u,4); /* mov.l 0x8c6663b4,r2 */
 goto L8c6662d2;
L8c6662d2: target=r[2];*pr=0x8c6662d6u;r[1]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6662d6;
L8c6662d6: wr(m,r[15]+0x8u,r[0],4); /* mov.l r0,@(0x8,r15) */
 goto L8c6662d8;
L8c6662d8: r[3]=rd(m,0x8c6663b8u,4); /* mov.l 0x8c6663b8,r3 */
 goto L8c6662da;
L8c6662da: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c6662dc;
L8c6662dc: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c6662de;
L8c6662de: if(!(*sr&1u))goto L8c6662f6; /* bf 0x8c6662f6 */
 goto L8c6662e0;
L8c6662e0: r[0]=rd(m,0x8c6663bcu,4); /* mov.l 0x8c6663bc,r0 */
 goto L8c6662e2;
L8c6662e2: r[1]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r1 */
 goto L8c6662e4;
L8c6662e4: r[2]=rd(m,r[0],4); /* mov.l @r0,r2 */
 goto L8c6662e6;
L8c6662e6: *sr=(*sr&~1u)|(r[2]>=r[1]); /* cmp/hs r1,r2 */
 goto L8c6662e8;
L8c6662e8: if(!(*sr&1u))goto L8c6662f4; /* bf 0x8c6662f4 */
 goto L8c6662ea;
L8c6662ea: r[0]=rd(m,0x8c6663c0u,4); /* mov.l 0x8c6663c0,r0 */
 goto L8c6662ec;
L8c6662ec: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c6662ee;
L8c6662ee: r[1]=rd(m,r[0],4); /* mov.l @r0,r1 */
 goto L8c6662f0;
L8c6662f0: *sr=(*sr&~1u)|(r[1]>=r[2]); /* cmp/hs r2,r1 */
 goto L8c6662f2;
L8c6662f2: if((*sr&1u))goto L8c6662f6; /* bt 0x8c6662f6 */
 goto L8c6662f4;
L8c6662f4: r[4]=r[14]; /* mov r14,r4 */
 goto L8c6662f6;
L8c6662f6: r[2]=rd(m,0x8c6663acu,4); /* mov.l 0x8c6663ac,r2 */
 goto L8c6662f8;
L8c6662f8: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c6662fa;
L8c6662fa: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c6662fc;
L8c6662fc: r[1]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r1 */
 goto L8c6662fe;
L8c6662fe: *macl=r[3]*r[2]; /* mul.l r3,r2 */
 goto L8c666300;
L8c666300: r[3]=rd(m,r[15]+0x2cu,4); /* mov.l @(0x2c,r15),r3 */
 goto L8c666302;
L8c666302: *sr=(*sr&~1u)|(r[3]==r[13]); /* cmp/eq r3,r13 */
 goto L8c666304;
L8c666304: r[0]=*sr&1u; /* movt r0 */
 goto L8c666306;
L8c666306: r[0]&=0x1u; /* and #0x1,r0 */
 goto L8c666308;
L8c666308: *sr=(*sr&~1u)|(r[0]&1u);r[0]=(r[0]>>1)|(r[0]<<31); /* rotr r0 */
 goto L8c66630a;
L8c66630a: r[3]=r[0]; /* mov r0,r3 */
 goto L8c66630c;
L8c66630c: r[0]=r[2]; /* mov r2,r0 */
 goto L8c66630e;
L8c66630e: r[0]&=0x3fu; /* and #0x3f,r0 */
 goto L8c666310;
L8c666310: r[2]=rd(m,r[15]+0x34u,4); /* mov.l @(0x34,r15),r2 */
 goto L8c666312;
L8c666312: r[5]=*macl; /* sts MACL,r5 */
 goto L8c666314;
L8c666314: r[0]<<=8; /* shll8 r0 */
 goto L8c666316;
L8c666316: r[3]|=r[0]; /* or r0,r3 */
 goto L8c666318;
L8c666318: r[0]=r[1]; /* mov r1,r0 */
 goto L8c66631a;
L8c66631a: r[5]+=r[1]; /* add r1,r5 */
 goto L8c66631c;
L8c66631c: r[0]&=0x3fu; /* and #0x3f,r0 */
 goto L8c66631e;
L8c66631e: r[0]<<=2; /* shll2 r0 */
 goto L8c666320;
L8c666320: r[3]|=r[0]; /* or r0,r3 */
 goto L8c666322;
L8c666322: r[0]=sx(m,0x8c6663a0u,2); /* mov.w 0x8c6663a0,r0 */
 goto L8c666324;
L8c666324: r[3]|=r[2]; /* or r2,r3 */
 goto L8c666326;
L8c666326: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c666328;
L8c666328: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c66632a;
L8c66632a: r[1]=rd(m,r[15]+0x38u,4); /* mov.l @(0x38,r15),r1 */
 goto L8c66632c;
L8c66632c: r[3]=rd(m,r[15]+0x1cu,4); /* mov.l @(0x1c,r15),r3 */
 goto L8c66632e;
L8c66632e: *macl=r[5]*r[1]; /* mul.l r5,r1 */
 goto L8c666330;
L8c666330: r[0]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r0 */
 goto L8c666332;
L8c666332: r[1]=*macl; /* sts MACL,r1 */
 goto L8c666334;
L8c666334: r[1]+=r[3]; /* add r3,r1 */
 goto L8c666336;
L8c666336: r[1]|=r[0]; /* or r0,r1 */
 goto L8c666338;
L8c666338: r[1]|=r[4]; /* or r4,r1 */
 goto L8c66633a;
L8c66633a: r[0]=sx(m,0x8c6663a2u,2); /* mov.w 0x8c6663a2,r0 */
 goto L8c66633c;
L8c66633c: wr(m,r[15]+r[0],r[1],4); /* mov.l r1,@(r0,r15) */
 goto L8c66633e;
L8c66633e: r[0]=64u; /* mov #0x40,r0 */
 goto L8c666340;
L8c666340: r[2]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r2 */
 goto L8c666342;
L8c666342: r[0]=92u; /* mov #0x5c,r0 */
 goto L8c666344;
L8c666344: r[1]=rd(m,r[15]+0x18u,4); /* mov.l @(0x18,r15),r1 */
 goto L8c666346;
L8c666346: *macl=r[5]*r[2]; /* mul.l r5,r2 */
 goto L8c666348;
L8c666348: r[0]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r0 */
 goto L8c66634a;
L8c66634a: r[2]=*macl; /* sts MACL,r2 */
 goto L8c66634c;
L8c66634c: r[2]+=r[1]; /* add r1,r2 */
 goto L8c66634e;
L8c66634e: r[2]|=r[0]; /* or r0,r2 */
 goto L8c666350;
L8c666350: r[0]=sx(m,0x8c6663a4u,2); /* mov.w 0x8c6663a4,r0 */
 goto L8c666352;
L8c666352: r[2]|=r[4]; /* or r4,r2 */
 goto L8c666354;
L8c666354: wr(m,r[15]+r[0],r[2],4); /* mov.l r2,@(r0,r15) */
 goto L8c666356;
L8c666356: r[0]=72u; /* mov #0x48,r0 */
 goto L8c666358;
L8c666358: r[3]=rd(m,r[15]+0x3cu,4); /* mov.l @(0x3c,r15),r3 */
 goto L8c66635a;
L8c66635a: r[2]=rd(m,r[15]+0x24u,4); /* mov.l @(0x24,r15),r2 */
 goto L8c66635c;
L8c66635c: *macl=r[5]*r[3]; /* mul.l r5,r3 */
 goto L8c66635e;
L8c66635e: r[0]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r0 */
 goto L8c666360;
L8c666360: r[3]=*macl; /* sts MACL,r3 */
 goto L8c666362;
L8c666362: r[3]+=r[2]; /* add r2,r3 */
 goto L8c666364;
L8c666364: r[3]|=r[0]; /* or r0,r3 */
 goto L8c666366;
L8c666366: r[0]=sx(m,0x8c6663a6u,2); /* mov.w 0x8c6663a6,r0 */
 goto L8c666368;
L8c666368: r[3]|=r[4]; /* or r4,r3 */
 goto L8c66636a;
L8c66636a: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c66636c;
L8c66636c: r[0]=84u; /* mov #0x54,r0 */
 goto L8c66636e;
L8c66636e: r[1]=rd(m,r[15]+0x30u,4); /* mov.l @(0x30,r15),r1 */
 goto L8c666370;
L8c666370: r[3]=rd(m,r[15]+0x20u,4); /* mov.l @(0x20,r15),r3 */
 goto L8c666372;
L8c666372: *macl=r[5]*r[1]; /* mul.l r5,r1 */
 goto L8c666374;
L8c666374: r[0]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r0 */
 goto L8c666376;
L8c666376: r[1]=*macl; /* sts MACL,r1 */
 goto L8c666378;
L8c666378: r[1]+=r[3]; /* add r3,r1 */
 goto L8c66637a;
L8c66637a: r[1]|=r[0]; /* or r0,r1 */
 goto L8c66637c;
L8c66637c: r[0]=sx(m,0x8c6663a8u,2); /* mov.w 0x8c6663a8,r0 */
 goto L8c66637e;
L8c66637e: r[1]|=r[4]; /* or r4,r1 */
 goto L8c666380;
L8c666380: wr(m,r[15]+r[0],r[1],4); /* mov.l r1,@(r0,r15) */
 goto L8c666382;
L8c666382: r[1]=rd(m,0x8c6663c4u,4); /* mov.l 0x8c6663c4,r1 */
 goto L8c666384;
L8c666384: r[2]=rd(m,r[1],4); /* mov.l @r1,r2 */
 goto L8c666386;
L8c666386: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c666388;
L8c666388: if((*sr&1u))goto L8c6663e0; /* bt 0x8c6663e0 */
 goto L8c66638a;
L8c66638a: r[0]=sx(m,0x8c6663a6u,2); /* mov.w 0x8c6663a6,r0 */
 goto L8c66638c;
L8c66638c: r[2]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r2 */
 goto L8c66638e;
L8c66638e: r[0]=sx(m,0x8c6663aau,2); /* mov.w 0x8c6663aa,r0 */
 goto L8c666390;
L8c666390: wr(m,r[15]+r[0],r[2],4); /* mov.l r2,@(r0,r15) */
 goto L8c666392;
L8c666392: goto L8c6663f8; /* bra 0x8c6663f8 */
 goto L8c6663e0;
L8c6663e0: r[0]=68u; /* mov #0x44,r0 */
 goto L8c6663e2;
L8c6663e2: r[3]=rd(m,r[15]+0x28u,4); /* mov.l @(0x28,r15),r3 */
 goto L8c6663e4;
L8c6663e4: r[2]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r2 */
 goto L8c6663e6;
L8c6663e6: r[0]=88u; /* mov #0x58,r0 */
 goto L8c6663e8;
L8c6663e8: r[1]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r1 */
 goto L8c6663ea;
L8c6663ea: *macl=r[5]*r[2]; /* mul.l r5,r2 */
 goto L8c6663ec;
L8c6663ec: r[0]=sx(m,0x8c666420u,2); /* mov.w 0x8c666420,r0 */
 goto L8c6663ee;
L8c6663ee: r[5]=*macl; /* sts MACL,r5 */
 goto L8c6663f0;
L8c6663f0: r[5]+=r[3]; /* add r3,r5 */
 goto L8c6663f2;
L8c6663f2: r[5]|=r[1]; /* or r1,r5 */
 goto L8c6663f4;
L8c6663f4: r[5]|=r[4]; /* or r4,r5 */
 goto L8c6663f6;
L8c6663f6: wr(m,r[15]+r[0],r[5],4); /* mov.l r5,@(r0,r15) */
 goto L8c6663f8;
L8c6663f8: r[2]=rd(m,0x8c666428u,4); /* mov.l 0x8c666428,r2 */
 goto L8c6663fa;
L8c6663fa: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c6663fc;
L8c6663fc: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c6663fe;
L8c6663fe: if((*sr&1u))goto L8c666448; /* bt 0x8c666448 */
 goto L8c666400;
L8c666400: r[3]=rd(m,0x8c66642cu,4); /* mov.l 0x8c66642c,r3 */
 goto L8c666402;
L8c666402: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c666404;
L8c666404: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c666406;
L8c666406: if(!(*sr&1u))goto L8c666440; /* bf 0x8c666440 */
 goto L8c666408;
L8c666408: r[0]=sx(m,0x8c666422u,2); /* mov.w 0x8c666422,r0 */
 goto L8c66640a;
L8c66640a: r[1]=sx(m,0x8c666424u,2); /* mov.w 0x8c666424,r1 */
 goto L8c66640c;
L8c66640c: r[0]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r0 */
 goto L8c66640e;
L8c66640e: r[1]+=r[15]; /* add r15,r1 */
 goto L8c666410;
L8c666410: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c666412;
L8c666412: goto L8c666448; /* bra 0x8c666448 */
 goto L8c666440;
L8c666440: r[0]=sx(m,0x8c6665a0u,2); /* mov.w 0x8c6665a0,r0 */
 goto L8c666442;
L8c666442: r[1]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r1 */
 goto L8c666444;
L8c666444: r[0]=sx(m,0x8c6665a2u,2); /* mov.w 0x8c6665a2,r0 */
 goto L8c666446;
L8c666446: wr(m,r[15]+r[0],r[1],4); /* mov.l r1,@(r0,r15) */
 goto L8c666448;
L8c666448: r[3]=rd(m,0x8c6665c0u,4); /* mov.l 0x8c6665c0,r3 */
 goto L8c66644a;
L8c66644a: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c66644c;
L8c66644c: *sr=(*sr&~1u)|!(r[0]&0x20u); /* tst #0x20,r0 */
 goto L8c66644e;
L8c66644e: if(!(*sr&1u))goto L8c666454; /* bf 0x8c666454 */
 goto L8c666450;
L8c666450: goto L8c666658; /* bra 0x8c666658 */
 goto L8c666454;
L8c666454: r[1]=rd(m,0x8c6665c4u,4); /* mov.l 0x8c6665c4,r1 */
 goto L8c666456;
L8c666456: r[0]=sx(m,0x8c6665a4u,2); /* mov.w 0x8c6665a4,r0 */
 goto L8c666458;
L8c666458: r[2]=rd(m,r[1],4); /* mov.l @r1,r2 */
 goto L8c66645a;
L8c66645a: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c66645c;
L8c66645c: take=!(*sr&1u);r[4]=rd(m,r[15]+r[0],4);if(take)goto L8c666464;goto L8c666460; /* bf/s 0x8c666464 */
 goto L8c666460;
L8c666460: goto L8c6665e0; /* bra 0x8c6665e0 */
 goto L8c666464;
L8c666464: r[0]=sx(m,0x8c6665a6u,2); /* mov.w 0x8c6665a6,r0 */
 goto L8c666466;
L8c666466: r[3]=sx(m,0x8c6665a4u,2); /* mov.w 0x8c6665a4,r3 */
 goto L8c666468;
L8c666468: wr(m,r[15]+r[0],r[4],4); /* mov.l r4,@(r0,r15) */
 goto L8c66646a;
L8c66646a: r[0]=120u; /* mov #0x78,r0 */
 goto L8c66646c;
L8c66646c: wr(m,r[15]+r[0],r[4],4); /* mov.l r4,@(r0,r15) */
 goto L8c66646e;
L8c66646e: r[0]=96u; /* mov #0x60,r0 */
 goto L8c666470;
L8c666470: wr(m,r[15]+r[0],r[4],4); /* mov.l r4,@(r0,r15) */
 goto L8c666472;
L8c666472: r[3]+=r[15]; /* add r15,r3 */
 goto L8c666474;
L8c666474: r[0]=sx(m,0x8c6665a4u,2); /* mov.w 0x8c6665a4,r0 */
 goto L8c666476;
L8c666476: r[2]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r2 */
 goto L8c666478;
L8c666478: r[0]=sx(m,0x8c6665a4u,2); /* mov.w 0x8c6665a4,r0 */
 goto L8c66647a;
L8c66647a: r[2]&=r[11]; /* and r11,r2 */
 goto L8c66647c;
L8c66647c: wr(m,r[15]+r[0],r[2],4); /* mov.l r2,@(r0,r15) */
 goto L8c66647e;
L8c66647e: r[0]=sx(m,0x8c6665a6u,2); /* mov.w 0x8c6665a6,r0 */
 goto L8c666480;
L8c666480: r[2]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r2 */
 goto L8c666482;
L8c666482: r[0]=sx(m,0x8c6665a6u,2); /* mov.w 0x8c6665a6,r0 */
 goto L8c666484;
L8c666484: r[2]&=r[11]; /* and r11,r2 */
 goto L8c666486;
L8c666486: wr(m,r[15]+r[0],r[2],4); /* mov.l r2,@(r0,r15) */
 goto L8c666488;
L8c666488: r[0]=120u; /* mov #0x78,r0 */
 goto L8c66648a;
L8c66648a: r[2]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r2 */
 goto L8c66648c;
L8c66648c: r[0]=120u; /* mov #0x78,r0 */
 goto L8c66648e;
L8c66648e: r[2]&=r[11]; /* and r11,r2 */
 goto L8c666490;
L8c666490: wr(m,r[15]+r[0],r[2],4); /* mov.l r2,@(r0,r15) */
 goto L8c666492;
L8c666492: r[0]=sx(m,0x8c6665a4u,2); /* mov.w 0x8c6665a4,r0 */
 goto L8c666494;
L8c666494: r[2]=rd(m,0x8c6665c8u,4); /* mov.l 0x8c6665c8,r2 */
 goto L8c666496;
L8c666496: r[0]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r0 */
 goto L8c666498;
L8c666498: r[0]&=r[2]; /* and r2,r0 */
 goto L8c66649a;
L8c66649a: wr(m,r[3],r[0],4); /* mov.l r0,@r3 */
 goto L8c66649c;
L8c66649c: r[0]=sx(m,0x8c6665a6u,2); /* mov.w 0x8c6665a6,r0 */
 goto L8c66649e;
L8c66649e: r[3]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r3 */
 goto L8c6664a0;
L8c6664a0: r[0]=sx(m,0x8c6665a6u,2); /* mov.w 0x8c6665a6,r0 */
 goto L8c6664a2;
L8c6664a2: r[3]|=r[10]; /* or r10,r3 */
 goto L8c6664a4;
L8c6664a4: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c6664a6;
L8c6664a6: r[0]=120u; /* mov #0x78,r0 */
 goto L8c6664a8;
L8c6664a8: r[3]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r3 */
 goto L8c6664aa;
L8c6664aa: r[0]=120u; /* mov #0x78,r0 */
 goto L8c6664ac;
L8c6664ac: r[1]=r[15]; /* mov r15,r1 */
 goto L8c6664ae;
L8c6664ae: r[1]+=96u; /* add #0x60,r1 */
 goto L8c6664b0;
L8c6664b0: r[3]|=r[10]; /* or r10,r3 */
 goto L8c6664b2;
L8c6664b2: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c6664b4;
L8c6664b4: r[0]=96u; /* mov #0x60,r0 */
 goto L8c6664b6;
L8c6664b6: r[3]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r3 */
 goto L8c6664b8;
L8c6664b8: r[0]=96u; /* mov #0x60,r0 */
 goto L8c6664ba;
L8c6664ba: r[3]|=r[10]; /* or r10,r3 */
 goto L8c6664bc;
L8c6664bc: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c6664be;
L8c6664be: r[0]=sx(m,0x8c6665a4u,2); /* mov.w 0x8c6665a4,r0 */
 goto L8c6664c0;
L8c6664c0: r[3]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r3 */
 goto L8c6664c2;
L8c6664c2: r[0]=sx(m,0x8c6665a4u,2); /* mov.w 0x8c6665a4,r0 */
 goto L8c6664c4;
L8c6664c4: r[3]|=r[9]; /* or r9,r3 */
 goto L8c6664c6;
L8c6664c6: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c6664c8;
L8c6664c8: r[0]=sx(m,0x8c6665a6u,2); /* mov.w 0x8c6665a6,r0 */
 goto L8c6664ca;
L8c6664ca: r[3]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r3 */
 goto L8c6664cc;
L8c6664cc: r[0]=sx(m,0x8c6665a6u,2); /* mov.w 0x8c6665a6,r0 */
 goto L8c6664ce;
L8c6664ce: r[3]|=r[9]; /* or r9,r3 */
 goto L8c6664d0;
L8c6664d0: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c6664d2;
L8c6664d2: r[0]=120u; /* mov #0x78,r0 */
 goto L8c6664d4;
L8c6664d4: r[3]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r3 */
 goto L8c6664d6;
L8c6664d6: r[0]=120u; /* mov #0x78,r0 */
 goto L8c6664d8;
L8c6664d8: r[3]|=r[9]; /* or r9,r3 */
 goto L8c6664da;
L8c6664da: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c6664dc;
L8c6664dc: r[0]=96u; /* mov #0x60,r0 */
 goto L8c6664de;
L8c6664de: r[0]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r0 */
 goto L8c6664e0;
L8c6664e0: r[3]=rd(m,0x8c6665ccu,4); /* mov.l 0x8c6665cc,r3 */
 goto L8c6664e2;
L8c6664e2: r[0]&=r[3]; /* and r3,r0 */
 goto L8c6664e4;
L8c6664e4: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c6664e6;
L8c6664e6: r[0]=sx(m,0x8c6665a6u,2); /* mov.w 0x8c6665a6,r0 */
 goto L8c6664e8;
L8c6664e8: r[2]=sx(m,0x8c6665a6u,2); /* mov.w 0x8c6665a6,r2 */
 goto L8c6664ea;
L8c6664ea: r[0]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r0 */
 goto L8c6664ec;
L8c6664ec: r[1]=rd(m,0x8c6665d0u,4); /* mov.l 0x8c6665d0,r1 */
 goto L8c6664ee;
L8c6664ee: r[2]+=r[15]; /* add r15,r2 */
 goto L8c6664f0;
L8c6664f0: r[0]&=r[1]; /* and r1,r0 */
 goto L8c6664f2;
L8c6664f2: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c6664f4;
L8c6664f4: r[0]=80u; /* mov #0x50,r0 */
 goto L8c6664f6;
L8c6664f6: r[2]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r2 */
 goto L8c6664f8;
L8c6664f8: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c6664fa;
L8c6664fa: if((*sr&1u))goto L8c666502; /* bt 0x8c666502 */
 goto L8c6664fc;
L8c6664fc: r[0]=sx(m,0x8c6665a8u,2); /* mov.w 0x8c6665a8,r0 */
 goto L8c6664fe;
L8c6664fe: r[2]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r2 */
 goto L8c666500;
L8c666500: wr(m,r[15]+r[0],r[2],4); /* mov.l r2,@(r0,r15) */
 goto L8c666502;
L8c666502: r[0]=sx(m,0x8c6665aau,2); /* mov.w 0x8c6665aa,r0 */
 goto L8c666504;
L8c666504: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c666506;
L8c666506: r[0]=sx(m,0x8c6665acu,2); /* mov.w 0x8c6665ac,r0 */
 goto L8c666508;
L8c666508: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c66650a;
L8c66650a: r[0]=sx(m,0x8c6665aeu,2); /* mov.w 0x8c6665ae,r0 */
 goto L8c66650c;
L8c66650c: r[3]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r3 */
 goto L8c66650e;
L8c66650e: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c666510;
L8c666510: r[0]=sx(m,0x8c6665b0u,2); /* mov.w 0x8c6665b0,r0 */
 goto L8c666512;
L8c666512: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c666514;
L8c666514: r[0]=sx(m,0x8c6665b2u,2); /* mov.w 0x8c6665b2,r0 */
 goto L8c666516;
L8c666516: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c666518;
L8c666518: r[0]=124u; /* mov #0x7c,r0 */
 goto L8c66651a;
L8c66651a: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c66651c;
L8c66651c: r[0]=sx(m,0x8c6665b4u,2); /* mov.w 0x8c6665b4,r0 */
 goto L8c66651e;
L8c66651e: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c666520;
L8c666520: r[0]=sx(m,0x8c6665b6u,2); /* mov.w 0x8c6665b6,r0 */
 goto L8c666522;
L8c666522: r[3]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r3 */
 goto L8c666524;
L8c666524: r[0]=sx(m,0x8c6665b8u,2); /* mov.w 0x8c6665b8,r0 */
 goto L8c666526;
L8c666526: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c666528;
L8c666528: r[0]=sx(m,0x8c6665a0u,2); /* mov.w 0x8c6665a0,r0 */
 goto L8c66652a;
L8c66652a: r[2]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r2 */
 goto L8c66652c;
L8c66652c: r[0]=sx(m,0x8c6665bau,2); /* mov.w 0x8c6665ba,r0 */
 goto L8c66652e;
L8c66652e: wr(m,r[15]+r[0],r[2],4); /* mov.l r2,@(r0,r15) */
 goto L8c666530;
L8c666530: r[0]=sx(m,0x8c6665bcu,2); /* mov.w 0x8c6665bc,r0 */
 goto L8c666532;
L8c666532: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c666534;
L8c666534: r[0]=sx(m,0x8c6665b6u,2); /* mov.w 0x8c6665b6,r0 */
 goto L8c666536;
L8c666536: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c666538;
L8c666538: r[0]=sx(m,0x8c6665a0u,2); /* mov.w 0x8c6665a0,r0 */
 goto L8c66653a;
L8c66653a: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c66653c;
L8c66653c: r[0]=100u; /* mov #0x64,r0 */
 goto L8c66653e;
L8c66653e: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c666540;
L8c666540: r[0]=104u; /* mov #0x68,r0 */
 goto L8c666542;
L8c666542: r[3]=r[13]; /* mov r13,r3 */
 goto L8c666544;
L8c666544: r[2]=r[13]; /* mov r13,r2 */
 goto L8c666546;
L8c666546: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c666548;
L8c666548: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c66654a;
L8c66654a: r[0]=108u; /* mov #0x6c,r0 */
 goto L8c66654c;
L8c66654c: r[5]=sx(m,0x8c6665a4u,2); /* mov.w 0x8c6665a4,r5 */
 goto L8c66654e;
L8c66654e: r[3]+=r[2]; /* add r2,r3 */
 goto L8c666550;
L8c666550: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c666552;
L8c666552: r[3]<<=2; /* shll2 r3 */
 goto L8c666554;
L8c666554: r[0]=112u; /* mov #0x70,r0 */
 goto L8c666556;
L8c666556: r[3]<<=2; /* shll2 r3 */
 goto L8c666558;
L8c666558: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c66655a;
L8c66655a: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c66655c;
L8c66655c: r[0]=116u; /* mov #0x74,r0 */
 goto L8c66655e;
L8c66655e: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c666560;
L8c666560: r[5]+=r[15]; /* add r15,r5 */
 goto L8c666562;
L8c666562: r[1]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r1 */
 goto L8c666564;
L8c666564: r[6]=24u; /* mov #0x18,r6 */
 goto L8c666566;
L8c666566: r[3]+=r[1]; /* add r1,r3 */
 goto L8c666568;
L8c666568: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c66656a;
L8c66656a: target=r[12];*pr=0x8c66656eu;r[4]=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c66656e;
L8c66656e: r[5]=sx(m,0x8c6665a6u,2); /* mov.w 0x8c6665a6,r5 */
 goto L8c666570;
L8c666570: r[6]=24u; /* mov #0x18,r6 */
 goto L8c666572;
L8c666572: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c666574;
L8c666574: r[5]+=r[15]; /* add r15,r5 */
 goto L8c666576;
L8c666576: target=r[12];*pr=0x8c66657au;r[4]+=24u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c66657a;
L8c66657a: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c66657c;
L8c66657c: r[5]=r[15]; /* mov r15,r5 */
 goto L8c66657e;
L8c66657e: r[6]=24u; /* mov #0x18,r6 */
 goto L8c666580;
L8c666580: r[5]+=120u; /* add #0x78,r5 */
 goto L8c666582;
L8c666582: target=r[12];*pr=0x8c666586u;r[4]+=48u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c666586;
L8c666586: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c666588;
L8c666588: r[5]=r[15]; /* mov r15,r5 */
 goto L8c66658a;
L8c66658a: r[6]=24u; /* mov #0x18,r6 */
 goto L8c66658c;
L8c66658c: r[5]+=96u; /* add #0x60,r5 */
 goto L8c66658e;
L8c66658e: r[4]+=72u;goto L8c666650; /* bra 0x8c666650 */
 goto L8c6665e0;
L8c6665e0: r[0]=sx(m,0x8c6666a0u,2); /* mov.w 0x8c6666a0,r0 */
 goto L8c6665e2;
L8c6665e2: r[1]=sx(m,0x8c6666a0u,2); /* mov.w 0x8c6666a0,r1 */
 goto L8c6665e4;
L8c6665e4: wr(m,r[15]+r[0],r[4],4); /* mov.l r4,@(r0,r15) */
 goto L8c6665e6;
L8c6665e6: r[0]=sx(m,0x8c6666a2u,2); /* mov.w 0x8c6666a2,r0 */
 goto L8c6665e8;
L8c6665e8: r[1]+=r[15]; /* add r15,r1 */
 goto L8c6665ea;
L8c6665ea: r[2]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r2 */
 goto L8c6665ec;
L8c6665ec: r[0]=sx(m,0x8c6666a2u,2); /* mov.w 0x8c6666a2,r0 */
 goto L8c6665ee;
L8c6665ee: r[2]&=r[11]; /* and r11,r2 */
 goto L8c6665f0;
L8c6665f0: wr(m,r[15]+r[0],r[2],4); /* mov.l r2,@(r0,r15) */
 goto L8c6665f2;
L8c6665f2: r[0]=sx(m,0x8c6666a2u,2); /* mov.w 0x8c6666a2,r0 */
 goto L8c6665f4;
L8c6665f4: r[2]=rd(m,0x8c6666b0u,4); /* mov.l 0x8c6666b0,r2 */
 goto L8c6665f6;
L8c6665f6: r[0]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r0 */
 goto L8c6665f8;
L8c6665f8: r[0]&=r[2]; /* and r2,r0 */
 goto L8c6665fa;
L8c6665fa: r[3]=r[0]; /* mov r0,r3 */
 goto L8c6665fc;
L8c6665fc: r[0]=sx(m,0x8c6666a2u,2); /* mov.w 0x8c6666a2,r0 */
 goto L8c6665fe;
L8c6665fe: r[3]|=r[9]; /* or r9,r3 */
 goto L8c666600;
L8c666600: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c666602;
L8c666602: r[0]=sx(m,0x8c6666a0u,2); /* mov.w 0x8c6666a0,r0 */
 goto L8c666604;
L8c666604: r[3]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r3 */
 goto L8c666606;
L8c666606: r[0]=sx(m,0x8c6666a0u,2); /* mov.w 0x8c6666a0,r0 */
 goto L8c666608;
L8c666608: r[3]|=r[10]; /* or r10,r3 */
 goto L8c66660a;
L8c66660a: wr(m,r[15]+r[0],r[3],4); /* mov.l r3,@(r0,r15) */
 goto L8c66660c;
L8c66660c: r[0]=sx(m,0x8c6666a0u,2); /* mov.w 0x8c6666a0,r0 */
 goto L8c66660e;
L8c66660e: r[3]=rd(m,0x8c6666b4u,4); /* mov.l 0x8c6666b4,r3 */
 goto L8c666610;
L8c666610: r[0]=rd(m,r[15]+r[0],4); /* mov.l @(r0,r15),r0 */
 goto L8c666612;
L8c666612: r[0]&=r[3]; /* and r3,r0 */
 goto L8c666614;
L8c666614: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c666616;
L8c666616: r[0]=sx(m,0x8c6666a4u,2); /* mov.w 0x8c6666a4,r0 */
 goto L8c666618;
L8c666618: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c66661a;
L8c66661a: r[0]=sx(m,0x8c6666a6u,2); /* mov.w 0x8c6666a6,r0 */
 goto L8c66661c;
L8c66661c: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c66661e;
L8c66661e: r[0]=sx(m,0x8c6666a8u,2); /* mov.w 0x8c6666a8,r0 */
 goto L8c666620;
L8c666620: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c666622;
L8c666622: r[1]=r[13]; /* mov r13,r1 */
 goto L8c666624;
L8c666624: r[0]=sx(m,0x8c6666aau,2); /* mov.w 0x8c6666aa,r0 */
 goto L8c666626;
L8c666626: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c666628;
L8c666628: r[5]=sx(m,0x8c6666a2u,2); /* mov.w 0x8c6666a2,r5 */
 goto L8c66662a;
L8c66662a: r[6]=24u; /* mov #0x18,r6 */
 goto L8c66662c;
L8c66662c: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c66662e;
L8c66662e: r[0]=sx(m,0x8c6666acu,2); /* mov.w 0x8c6666ac,r0 */
 goto L8c666630;
L8c666630: r[5]+=r[15]; /* add r15,r5 */
 goto L8c666632;
L8c666632: wr(m,r[15]+r[0],r[14],4); /* mov.l r14,@(r0,r15) */
 goto L8c666634;
L8c666634: r[0]=r[13]; /* mov r13,r0 */
 goto L8c666636;
L8c666636: r[1]+=r[0]; /* add r0,r1 */
 goto L8c666638;
L8c666638: r[0]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r0 */
 goto L8c66663a;
L8c66663a: r[1]<<=2; /* shll2 r1 */
 goto L8c66663c;
L8c66663c: r[1]<<=2; /* shll2 r1 */
 goto L8c66663e;
L8c66663e: r[1]+=r[0]; /* add r0,r1 */
 goto L8c666640;
L8c666640: wr(m,r[15],r[1],4); /* mov.l r1,@r15 */
 goto L8c666642;
L8c666642: target=r[12];*pr=0x8c666646u;r[4]=r[1];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c666646;
L8c666646: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c666648;
L8c666648: r[6]=24u; /* mov #0x18,r6 */
 goto L8c66664a;
L8c66664a: r[5]=sx(m,0x8c6666a0u,2); /* mov.w 0x8c6666a0,r5 */
 goto L8c66664c;
L8c66664c: r[4]+=24u; /* add #0x18,r4 */
 goto L8c66664e;
L8c66664e: r[5]+=r[15]; /* add r15,r5 */
 goto L8c666650;
L8c666650: target=r[12];*pr=0x8c666654u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c666654;
L8c666654: goto L8c666670; /* bra 0x8c666670 */
 goto L8c666658;
L8c666658: r[4]=r[13]; /* mov r13,r4 */
 goto L8c66665a;
L8c66665a: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c66665c;
L8c66665c: r[2]=r[13]; /* mov r13,r2 */
 goto L8c66665e;
L8c66665e: r[4]+=r[2]; /* add r2,r4 */
 goto L8c666660;
L8c666660: r[1]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r1 */
 goto L8c666662;
L8c666662: r[4]<<=2; /* shll2 r4 */
 goto L8c666664;
L8c666664: r[5]=sx(m,0x8c6666a2u,2); /* mov.w 0x8c6666a2,r5 */
 goto L8c666666;
L8c666666: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c666668;
L8c666668: r[6]=24u; /* mov #0x18,r6 */
 goto L8c66666a;
L8c66666a: r[4]+=r[1]; /* add r1,r4 */
 goto L8c66666c;
L8c66666c: target=r[12];*pr=0x8c666670u;r[5]+=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c666670;
L8c666670: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c666672;
L8c666672: r[13]+=1u; /* add #0x1,r13 */
 goto L8c666674;
L8c666674: *sr=(*sr&~1u)|(r[13]>=r[3]); /* cmp/hs r3,r13 */
 goto L8c666676;
L8c666676: if((*sr&1u))goto L8c66667c; /* bt 0x8c66667c */
 goto L8c666678;
L8c666678: goto L8c66623a; /* bra 0x8c66623a */
 goto L8c66667c;
L8c66667c: r[1]=sx(m,0x8c6666aeu,2); /* mov.w 0x8c6666ae,r1 */
 goto L8c66667e;
L8c66667e: r[15]+=r[1]; /* add r1,r15 */
 goto L8c666680;
L8c666680: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c666682;
L8c666682: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c666684;
L8c666684: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c666686;
L8c666686: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c666688;
L8c666688: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66668a;
L8c66668a: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66668c;
L8c66668c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66668e;
L8c66668e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
