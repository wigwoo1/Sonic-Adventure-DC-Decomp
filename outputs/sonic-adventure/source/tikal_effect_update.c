#include "fpu_cleanup_transfer.h"
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
/* Static C recovery of retail Tikal helper 0cb0101c..0cb013b4.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int tikal_effect_update(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto Lcb0101c;
Lcb0101c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto Lcb0101e;
Lcb0101e: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto Lcb01020;
Lcb01020: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto Lcb01022;
Lcb01022: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto Lcb01024;
Lcb01024: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto Lcb01026;
Lcb01026: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto Lcb01028;
Lcb01028: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto Lcb0102a;
Lcb0102a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto Lcb0102c;
Lcb0102c: r[15]+=4294967252u; /* add #-0x2c,r15 */
 goto Lcb0102e;
Lcb0102e: r[5]=rd(m,r[4]+0x20u,4); /* mov.l @(0x20,r4),r5 */
 goto Lcb01030;
Lcb01030: r[14]=rd(m,r[4]+0x24u,4); /* mov.l @(0x24,r4),r14 */
 goto Lcb01032;
Lcb01032: r[0]=sx(m,r[5]+0x8u,1); /* mov.b @(0x8,r5),r0 */
 goto Lcb01034;
Lcb01034: r[6]=r[0]&255u; /* extu.b r0,r6 */
 goto Lcb01036;
Lcb01036: r[0]=rd(m,0x0cb010d0u,4); /* mov.l 0x0cb010d0,r0 */
 goto Lcb01038;
Lcb01038: r[6]<<=2; /* shll2 r6 */
 goto Lcb0103a;
Lcb0103a: r[12]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r12 */
 goto Lcb0103c;
Lcb0103c: r[0]=rd(m,0x0cb010d4u,4); /* mov.l 0x0cb010d4,r0 */
 goto Lcb0103e;
Lcb0103e: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto Lcb01040;
Lcb01040: r[3]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r3 */
 goto Lcb01042;
Lcb01042: wr(m,r[15]+0x4u,r[3],4); /* mov.l r3,@(0x4,r15) */
 goto Lcb01044;
Lcb01044: take=!(*sr&1u);r[6]=1u;if(take)goto Lcb0104e;goto Lcb01048; /* bf/s 0x0cb0104e */
 goto Lcb01048;
Lcb01048: r[3]=4u; /* mov #0x4,r3 */
 goto Lcb0104a;
Lcb0104a: wr(m,r[5],r[3],1);goto Lcb01080; /* bra 0x0cb01080 */
 goto Lcb0104e;
Lcb0104e: r[1]=sx(m,r[5],1); /* mov.b @r5,r1 */
 goto Lcb01050;
Lcb01050: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto Lcb01052;
Lcb01052: if((*sr&1u))goto Lcb01080; /* bt 0x0cb01080 */
 goto Lcb01054;
Lcb01054: r[3]=rd(m,r[12]+0x3cu,4); /* mov.l @(0x3c,r12),r3 */
 goto Lcb01056;
Lcb01056: r[0]=sx(m,r[3]+0x4u,1); /* mov.b @(0x4,r3),r0 */
 goto Lcb01058;
Lcb01058: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto Lcb0105a;
Lcb0105a: if((*sr&1u))goto Lcb01062; /* bt 0x0cb01062 */
 goto Lcb0105c;
Lcb0105c: r[2]=3u; /* mov #0x3,r2 */
 goto Lcb0105e;
Lcb0105e: wr(m,r[5],r[2],1);goto Lcb01080; /* bra 0x0cb01080 */
 goto Lcb01062;
Lcb01062: r[0]=sx(m,r[5]+0x8u,1); /* mov.b @(0x8,r5),r0 */
 goto Lcb01064;
Lcb01064: r[3]=52u; /* mov #0x34,r3 */
 goto Lcb01066;
Lcb01066: r[1]=rd(m,0x0cb010d8u,4); /* mov.l 0x0cb010d8,r1 */
 goto Lcb01068;
Lcb01068: r[2]=1u; /* mov #0x1,r2 */
 goto Lcb0106a;
Lcb0106a: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb0106c;
Lcb0106c: *macl=(r[3]&65535u)*(r[0]&65535u); /* mulu.w r3,r0 */
 goto Lcb0106e;
Lcb0106e: r[2]<<=8; /* shll8 r2 */
 goto Lcb01070;
Lcb01070: r[0]=*macl; /* sts MACL,r0 */
 goto Lcb01072;
Lcb01072: r[1]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r1 */
 goto Lcb01074;
Lcb01074: *sr=(*sr&~1u)|!(r[2]&r[1]); /* tst r2,r1 */
 goto Lcb01076;
Lcb01076: if((*sr&1u))goto Lcb0107e; /* bt 0x0cb0107e */
 goto Lcb01078;
Lcb01078: r[0]=2u; /* mov #0x2,r0 */
 goto Lcb0107a;
Lcb0107a: wr(m,r[5],r[0],1);goto Lcb01080; /* bra 0x0cb01080 */
 goto Lcb0107e;
Lcb0107e: wr(m,r[5],r[6],1); /* mov.b r6,@r5 */
 goto Lcb01080;
Lcb01080: r[0]=sx(m,r[5],1); /* mov.b @r5,r0 */
 goto Lcb01082;
Lcb01082: r[13]=r[15]; /* mov r15,r13 */
 goto Lcb01084;
Lcb01084: r[8]=rd(m,0x0cb010dcu,4); /* mov.l 0x0cb010dc,r8 */
 goto Lcb01086;
Lcb01086: r[10]=0u; /* mov #0x0,r10 */
 goto Lcb01088;
Lcb01088: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto Lcb0108a;
Lcb0108a: fr[5]=0x3f800000u; /* fldi1 fr5 */
 goto Lcb0108c;
Lcb0108c: fr[4]=0; /* fldi0 fr4 */
 goto Lcb0108e;
Lcb0108e: take=(*sr&1u);r[13]+=32u;if(take)goto Lcb010e0;goto Lcb01092; /* bt/s 0x0cb010e0 */
 goto Lcb01092;
Lcb01092: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto Lcb01094;
Lcb01094: if(!(*sr&1u))goto Lcb0109a; /* bf 0x0cb0109a */
 goto Lcb01096;
Lcb01096: goto Lcb0118e; /* bra 0x0cb0118e */
 goto Lcb0109a;
Lcb0109a: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto Lcb0109c;
Lcb0109c: if(!(*sr&1u))goto Lcb010a2; /* bf 0x0cb010a2 */
 goto Lcb0109e;
Lcb0109e: goto Lcb011aa; /* bra 0x0cb011aa */
 goto Lcb010a2;
Lcb010a2: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto Lcb010a4;
Lcb010a4: if(!(*sr&1u))goto Lcb010aa; /* bf 0x0cb010aa */
 goto Lcb010a6;
Lcb010a6: goto Lcb01228; /* bra 0x0cb01228 */
 goto Lcb010aa;
Lcb010aa: goto Lcb0139a; /* bra 0x0cb0139a */
 goto Lcb010e0;
Lcb010e0: r[0]=88u; /* mov #0x58,r0 */
 goto Lcb010e2;
Lcb010e2: r[2]=r[14]; /* mov r14,r2 */
 goto Lcb010e4;
Lcb010e4: r[2]+=100u; /* add #0x64,r2 */
 goto Lcb010e6;
Lcb010e6: wr(m,r[5],r[6],1); /* mov.b r6,@r5 */
 goto Lcb010e8;
Lcb010e8: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto Lcb010ea;
Lcb010ea: r[1]=rd(m,r[3]+0x14u,4); /* mov.l @(0x14,r3),r1 */
 goto Lcb010ec;
Lcb010ec: wr(m,r[2],r[1],4); /* mov.l r1,@r2 */
 goto Lcb010ee;
Lcb010ee: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto Lcb010f0;
Lcb010f0: r[0]=104u; /* mov #0x68,r0 */
 goto Lcb010f2;
Lcb010f2: r[2]=rd(m,r[3]+0x18u,4); /* mov.l @(0x18,r3),r2 */
 goto Lcb010f4;
Lcb010f4: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto Lcb010f6;
Lcb010f6: r[0]=88u; /* mov #0x58,r0 */
 goto Lcb010f8;
Lcb010f8: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto Lcb010fa;
Lcb010fa: r[0]=108u; /* mov #0x6c,r0 */
 goto Lcb010fc;
Lcb010fc: r[2]=rd(m,r[3]+0x1cu,4); /* mov.l @(0x1c,r3),r2 */
 goto Lcb010fe;
Lcb010fe: r[3]=r[14]; /* mov r14,r3 */
 goto Lcb01100;
Lcb01100: r[3]+=112u; /* add #0x70,r3 */
 goto Lcb01102;
Lcb01102: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto Lcb01104;
Lcb01104: r[0]=92u; /* mov #0x5c,r0 */
 goto Lcb01106;
Lcb01106: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto Lcb01108;
Lcb01108: r[1]=rd(m,r[2]+0x14u,4); /* mov.l @(0x14,r2),r1 */
 goto Lcb0110a;
Lcb0110a: wr(m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto Lcb0110c;
Lcb0110c: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto Lcb0110e;
Lcb0110e: r[0]=116u; /* mov #0x74,r0 */
 goto Lcb01110;
Lcb01110: r[2]=rd(m,r[3]+0x18u,4); /* mov.l @(0x18,r3),r2 */
 goto Lcb01112;
Lcb01112: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto Lcb01114;
Lcb01114: r[0]=92u; /* mov #0x5c,r0 */
 goto Lcb01116;
Lcb01116: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto Lcb01118;
Lcb01118: r[0]=120u; /* mov #0x78,r0 */
 goto Lcb0111a;
Lcb0111a: r[2]=rd(m,r[3]+0x1cu,4); /* mov.l @(0x1c,r3),r2 */
 goto Lcb0111c;
Lcb0111c: r[3]=r[14]; /* mov r14,r3 */
 goto Lcb0111e;
Lcb0111e: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto Lcb01120;
Lcb01120: r[0]=96u; /* mov #0x60,r0 */
 goto Lcb01122;
Lcb01122: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto Lcb01124;
Lcb01124: r[3]+=124u; /* add #0x7c,r3 */
 goto Lcb01126;
Lcb01126: r[1]=rd(m,r[2]+0x14u,4); /* mov.l @(0x14,r2),r1 */
 goto Lcb01128;
Lcb01128: wr(m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto Lcb0112a;
Lcb0112a: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto Lcb0112c;
Lcb0112c: r[0]+=32u; /* add #0x20,r0 */
 goto Lcb0112e;
Lcb0112e: r[2]=rd(m,r[3]+0x18u,4); /* mov.l @(0x18,r3),r2 */
 goto Lcb01130;
Lcb01130: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto Lcb01132;
Lcb01132: r[0]=96u; /* mov #0x60,r0 */
 goto Lcb01134;
Lcb01134: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto Lcb01136;
Lcb01136: r[0]+=36u; /* add #0x24,r0 */
 goto Lcb01138;
Lcb01138: r[2]=rd(m,r[3]+0x1cu,4); /* mov.l @(0x1c,r3),r2 */
 goto Lcb0113a;
Lcb0113a: r[3]=32u; /* mov #0x20,r3 */
 goto Lcb0113c;
Lcb0113c: r[3]<<=8; /* shll8 r3 */
 goto Lcb0113e;
Lcb0113e: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto Lcb01140;
Lcb01140: r[2]=4294967264u; /* mov #-0x20,r2 */
 goto Lcb01142;
Lcb01142: r[2]<<=8; /* shll8 r2 */
 goto Lcb01144;
Lcb01144: wr(m,r[14]+0x28u,r[3],4); /* mov.l r3,@(0x28,r14) */
 goto Lcb01146;
Lcb01146: wr(m,r[14]+0x2cu,r[2],4); /* mov.l r2,@(0x2c,r14) */
 goto Lcb01148;
Lcb01148: r[2]=4294967272u; /* mov #-0x18,r2 */
 goto Lcb0114a;
Lcb0114a: r[3]=4u; /* mov #0x4,r3 */
 goto Lcb0114c;
Lcb0114c: r[2]<<=8; /* shll8 r2 */
 goto Lcb0114e;
Lcb0114e: r[3]<<=8; /* shll8 r3 */
 goto Lcb01150;
Lcb01150: wr(m,r[14]+0x30u,r[3],4); /* mov.l r3,@(0x30,r14) */
 goto Lcb01152;
Lcb01152: r[3]=2u; /* mov #0x2,r3 */
 goto Lcb01154;
Lcb01154: wr(m,r[14]+0x34u,r[2],4); /* mov.l r2,@(0x34,r14) */
 goto Lcb01156;
Lcb01156: r[2]=4294967269u; /* mov #-0x1b,r2 */
 goto Lcb01158;
Lcb01158: r[2]<<=8; /* shll8 r2 */
 goto Lcb0115a;
Lcb0115a: r[3]<<=8; /* shll8 r3 */
 goto Lcb0115c;
Lcb0115c: wr(m,r[14]+0x38u,r[3],4); /* mov.l r3,@(0x38,r14) */
 goto Lcb0115e;
Lcb0115e: wr(m,r[14]+0x3cu,r[2],4); /* mov.l r2,@(0x3c,r14) */
 goto Lcb01160;
Lcb01160: r[3]=27u; /* mov #0x1b,r3 */
 goto Lcb01162;
Lcb01162: r[0]=72u; /* mov #0x48,r0 */
 goto Lcb01164;
Lcb01164: r[3]<<=8; /* shll8 r3 */
 goto Lcb01166;
Lcb01166: r[2]=4294967294u; /* mov #-0x2,r2 */
 goto Lcb01168;
Lcb01168: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto Lcb0116a;
Lcb0116a: r[0]=76u; /* mov #0x4c,r0 */
 goto Lcb0116c;
Lcb0116c: r[3]=rd(m,0x0cb012a0u,4); /* mov.l 0x0cb012a0,r3 */
 goto Lcb0116e;
Lcb0116e: r[2]<<=8; /* shll8 r2 */
 goto Lcb01170;
Lcb01170: r[5]=8u; /* mov #0x8,r5 */
 goto Lcb01172;
Lcb01172: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto Lcb01174;
Lcb01174: r[0]=64u; /* mov #0x40,r0 */
 goto Lcb01176;
Lcb01176: r[5]<<=8; /* shll8 r5 */
 goto Lcb01178;
Lcb01178: r[6]=4294967283u; /* mov #-0xd,r6 */
 goto Lcb0117a;
Lcb0117a: wr(m,r[14]+r[0],r[5],4); /* mov.l r5,@(r0,r14) */
 goto Lcb0117c;
Lcb0117c: r[0]=68u; /* mov #0x44,r0 */
 goto Lcb0117e;
Lcb0117e: r[6]<<=8; /* shll8 r6 */
 goto Lcb01180;
Lcb01180: wr(m,r[14]+r[0],r[6],4); /* mov.l r6,@(r0,r14) */
 goto Lcb01182;
Lcb01182: r[0]=80u; /* mov #0x50,r0 */
 goto Lcb01184;
Lcb01184: wr(m,r[14]+r[0],r[5],4); /* mov.l r5,@(r0,r14) */
 goto Lcb01186;
Lcb01186: r[0]=84u; /* mov #0x54,r0 */
 goto Lcb01188;
Lcb01188: wr(m,r[14]+r[0],r[6],4); /* mov.l r6,@(r0,r14) */
 goto Lcb0118a;
Lcb0118a: wr(m,r[4]+0x18u,r[3],4);goto Lcb013a0; /* bra 0x0cb013a0 */
 goto Lcb0118e;
Lcb0118e: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto Lcb01190;
Lcb01190: r[3]=rd(m,0x0cb012a4u,4); /* mov.l 0x0cb012a4,r3 */
 goto Lcb01192;
Lcb01192: r[5]=rd(m,r[5]+0x20u,4); /* mov.l @(0x20,r5),r5 */
 goto Lcb01194;
Lcb01194: target=r[3];*pr=0xcb01198u;r[4]=rd(m,r[12]+0x18u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb01198;
Lcb01198: r[2]=rd(m,0x0cb012a8u,4); /* mov.l 0x0cb012a8,r2 */
 goto Lcb0119a;
Lcb0119a: r[0]=0u-r[0]; /* neg r0,r0 */
 goto Lcb0119c;
Lcb0119c: wr(m,r[14],r[0],4); /* mov.l r0,@r14 */
 goto Lcb0119e;
Lcb0119e: wr(m,r[14]+0x4u,r[10],4); /* mov.l r10,@(0x4,r14) */
 goto Lcb011a0;
Lcb011a0: target=r[2];*pr=0xcb011a4u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb011a4;
Lcb011a4: wr(m,r[14]+0x10u,r[10],4); /* mov.l r10,@(0x10,r14) */
 goto Lcb011a6;
Lcb011a6: wr(m,r[14]+0x14u,r[10],4);goto Lcb01392; /* bra 0x0cb01392 */
 goto Lcb011aa;
Lcb011aa: r[3]=rd(m,0x0cb012acu,4); /* mov.l 0x0cb012ac,r3 */
 goto Lcb011ac;
Lcb011ac: r[0]=16u; /* mov #0x10,r0 */
 goto Lcb011ae;
Lcb011ae: r[5]=r[15]; /* mov r15,r5 */
 goto Lcb011b0;
Lcb011b0: r[5]+=8u; /* add #0x8,r5 */
 goto Lcb011b2;
Lcb011b2: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto Lcb011b4;
Lcb011b4: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto Lcb011b6;
Lcb011b6: r[1]=rd(m,r[12]+0x38u,4); /* mov.l @(0x38,r12),r1 */
 goto Lcb011b8;
Lcb011b8: r[2]=rd(m,r[1]+0xcu,4); /* mov.l @(0xc,r1),r2 */
 goto Lcb011ba;
Lcb011ba: wr(m,r[15]+0x4u,r[2],4); /* mov.l r2,@(0x4,r15) */
 goto Lcb011bc;
Lcb011bc: wr(m,r[0]+r[15],fr[4],4); /* fmov fr4,@(r0,r15) */
 goto Lcb011be;
Lcb011be: r[0]=12u; /* mov #0xc,r0 */
 goto Lcb011c0;
Lcb011c0: wr(m,r[0]+r[15],fr[4],4); /* fmov fr4,@(r0,r15) */
 goto Lcb011c2;
Lcb011c2: r[0]=8u; /* mov #0x8,r0 */
 goto Lcb011c4;
Lcb011c4: r[2]=rd(m,0x0cb012b0u,4); /* mov.l 0x0cb012b0,r2 */
 goto Lcb011c6;
Lcb011c6: wr(m,r[0]+r[15],fr[5],4); /* fmov fr5,@(r0,r15) */
 goto Lcb011c8;
Lcb011c8: target=r[2];*pr=0xcb011ccu;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb011cc;
Lcb011cc: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto Lcb011ce;
Lcb011ce: r[2]=r[15]; /* mov r15,r2 */
 goto Lcb011d0;
Lcb011d0: r[2]+=20u; /* add #0x14,r2 */
 goto Lcb011d2;
Lcb011d2: r[4]=r[15]; /* mov r15,r4 */
 goto Lcb011d4;
Lcb011d4: r[1]=rd(m,r[3]+0x20u,4); /* mov.l @(0x20,r3),r1 */
 goto Lcb011d6;
Lcb011d6: r[0]=rd(m,r[3]+0x24u,4); /* mov.l @(0x24,r3),r0 */
 goto Lcb011d8;
Lcb011d8: wr(m,r[2],r[1],4); /* mov.l r1,@r2 */
 goto Lcb011da;
Lcb011da: wr(m,r[2]+0x4u,r[0],4); /* mov.l r0,@(0x4,r2) */
 goto Lcb011dc;
Lcb011dc: r[1]=rd(m,r[3]+0x28u,4); /* mov.l @(0x28,r3),r1 */
 goto Lcb011de;
Lcb011de: r[3]=rd(m,0x0cb012b4u,4); /* mov.l 0x0cb012b4,r3 */
 goto Lcb011e0;
Lcb011e0: wr(m,r[2]+0x8u,r[1],4); /* mov.l r1,@(0x8,r2) */
 goto Lcb011e2;
Lcb011e2: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto Lcb011e4;
Lcb011e4: r[5]+=8u; /* add #0x8,r5 */
 goto Lcb011e6;
Lcb011e6: target=r[3];*pr=0xcb011eau;r[4]+=20u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb011ea;
Lcb011ea: r[5]=r[15]; /* mov r15,r5 */
 goto Lcb011ec;
Lcb011ec: r[2]=rd(m,0x0cb012b8u,4); /* mov.l 0x0cb012b8,r2 */
 goto Lcb011ee;
Lcb011ee: r[4]=r[15]; /* mov r15,r4 */
 goto Lcb011f0;
Lcb011f0: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb011f2;
Lcb011f2: r[5]+=8u; /* add #0x8,r5 */
 goto Lcb011f4;
Lcb011f4: target=r[2];*pr=0xcb011f8u;r[4]+=20u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb011f8;
Lcb011f8: r[3]=r[13]; /* mov r13,r3 */
 goto Lcb011fa;
Lcb011fa: r[3]+=4u; /* add #0x4,r3 */
 goto Lcb011fc;
Lcb011fc: r[13]+=8u; /* add #0x8,r13 */
 goto Lcb011fe;
Lcb011fe: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto Lcb01200;
Lcb01200: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto Lcb01202;
Lcb01202: r[2]=0u-r[2]; /* neg r2,r2 */
 goto Lcb01204;
Lcb01204: wr(m,r[14],r[2],4); /* mov.l r2,@r14 */
 goto Lcb01206;
Lcb01206: r[2]=rd(m,0x0cb012a8u,4); /* mov.l 0x0cb012a8,r2 */
 goto Lcb01208;
Lcb01208: wr(m,r[15]+0x4u,r[13],4); /* mov.l r13,@(0x4,r15) */
 goto Lcb0120a;
Lcb0120a: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto Lcb0120c;
Lcb0120c: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto Lcb0120e;
Lcb0120e: target=r[2];*pr=0xcb01212u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb01212;
Lcb01212: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto Lcb01214;
Lcb01214: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto Lcb01216;
Lcb01216: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto Lcb01218;
Lcb01218: r[1]+=r[2]; /* add r2,r1 */
 goto Lcb0121a;
Lcb0121a: wr(m,r[14]+0x10u,r[1],4); /* mov.l r1,@(0x10,r14) */
 goto Lcb0121c;
Lcb0121c: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto Lcb0121e;
Lcb0121e: r[2]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r2 */
 goto Lcb01220;
Lcb01220: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto Lcb01222;
Lcb01222: r[1]-=r[2]; /* sub r2,r1 */
 goto Lcb01224;
Lcb01224: wr(m,r[14]+0x14u,r[1],4);goto Lcb01392; /* bra 0x0cb01392 */
 goto Lcb01228;
Lcb01228: r[11]=rd(m,r[12]+0x3cu,4); /* mov.l @(0x3c,r12),r11 */
 goto Lcb0122a;
Lcb0122a: r[4]=r[13]; /* mov r13,r4 */
 goto Lcb0122c;
Lcb0122c: r[2]=rd(m,r[12]+0x38u,4); /* mov.l @(0x38,r12),r2 */
 goto Lcb0122e;
Lcb0122e: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto Lcb01230;
Lcb01230: wr(m,r[15]+0x4u,r[3],4); /* mov.l r3,@(0x4,r15) */
 goto Lcb01232;
Lcb01232: r[0]=sx(m,r[11]+0x4u,1); /* mov.b @(0x4,r11),r0 */
 goto Lcb01234;
Lcb01234: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto Lcb01236;
Lcb01236: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto Lcb01238;
Lcb01238: take=(*sr&1u);r[4]+=4u;if(take)goto Lcb01248;goto Lcb0123c; /* bt/s 0x0cb01248 */
 goto Lcb0123c;
Lcb0123c: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto Lcb0123e;
Lcb0123e: if((*sr&1u))goto Lcb01252; /* bt 0x0cb01252 */
 goto Lcb01240;
Lcb01240: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto Lcb01242;
Lcb01242: if((*sr&1u))goto Lcb012c0; /* bt 0x0cb012c0 */
 goto Lcb01244;
Lcb01244: goto Lcb01360; /* bra 0x0cb01360 */
 goto Lcb01248;
Lcb01248: r[3]=rd(m,r[11]+0xcu,4); /* mov.l @(0xc,r11),r3 */
 goto Lcb0124a;
Lcb0124a: wr(m,r[4],r[3],4); /* mov.l r3,@r4 */
 goto Lcb0124c;
Lcb0124c: r[2]=rd(m,r[11]+0x10u,4); /* mov.l @(0x10,r11),r2 */
 goto Lcb0124e;
Lcb0124e: wr(m,r[13]+0x8u,r[2],4);goto Lcb01364; /* bra 0x0cb01364 */
 goto Lcb01252;
Lcb01252: r[0]=16u; /* mov #0x10,r0 */
 goto Lcb01254;
Lcb01254: r[5]=r[15]; /* mov r15,r5 */
 goto Lcb01256;
Lcb01256: wr(m,r[0]+r[15],fr[4],4); /* fmov fr4,@(r0,r15) */
 goto Lcb01258;
Lcb01258: r[0]=12u; /* mov #0xc,r0 */
 goto Lcb0125a;
Lcb0125a: wr(m,r[0]+r[15],fr[4],4); /* fmov fr4,@(r0,r15) */
 goto Lcb0125c;
Lcb0125c: r[0]=8u; /* mov #0x8,r0 */
 goto Lcb0125e;
Lcb0125e: r[3]=rd(m,0x0cb012b0u,4); /* mov.l 0x0cb012b0,r3 */
 goto Lcb01260;
Lcb01260: r[5]+=8u; /* add #0x8,r5 */
 goto Lcb01262;
Lcb01262: wr(m,r[0]+r[15],fr[5],4); /* fmov fr5,@(r0,r15) */
 goto Lcb01264;
Lcb01264: target=r[3];*pr=0xcb01268u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb01268;
Lcb01268: r[3]=rd(m,r[11]+0x14u,4); /* mov.l @(0x14,r11),r3 */
 goto Lcb0126a;
Lcb0126a: r[2]=r[15]; /* mov r15,r2 */
 goto Lcb0126c;
Lcb0126c: r[2]+=20u; /* add #0x14,r2 */
 goto Lcb0126e;
Lcb0126e: r[1]=rd(m,r[11]+0x18u,4); /* mov.l @(0x18,r11),r1 */
 goto Lcb01270;
Lcb01270: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto Lcb01272;
Lcb01272: r[4]=r[15]; /* mov r15,r4 */
 goto Lcb01274;
Lcb01274: wr(m,r[2]+0x4u,r[1],4); /* mov.l r1,@(0x4,r2) */
 goto Lcb01276;
Lcb01276: r[3]=rd(m,r[11]+0x1cu,4); /* mov.l @(0x1c,r11),r3 */
 goto Lcb01278;
Lcb01278: wr(m,r[2]+0x8u,r[3],4); /* mov.l r3,@(0x8,r2) */
 goto Lcb0127a;
Lcb0127a: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto Lcb0127c;
Lcb0127c: r[3]=rd(m,0x0cb012b4u,4); /* mov.l 0x0cb012b4,r3 */
 goto Lcb0127e;
Lcb0127e: r[5]+=8u; /* add #0x8,r5 */
 goto Lcb01280;
Lcb01280: target=r[3];*pr=0xcb01284u;r[4]+=20u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb01284;
Lcb01284: goto Lcb0134e; /* bra 0x0cb0134e */
 goto Lcb012c0;
Lcb012c0: r[2]=rd(m,r[11]+0x20u,4); /* mov.l @(0x20,r11),r2 */
 goto Lcb012c2;
Lcb012c2: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto Lcb012c4;
Lcb012c4: if((*sr&1u))goto Lcb012ce; /* bt 0x0cb012ce */
 goto Lcb012c6;
Lcb012c6: r[9]=rd(m,r[11]+0x20u,4); /* mov.l @(0x20,r11),r9 */
 goto Lcb012c8;
Lcb012c8: r[9]=rd(m,r[9]+0x20u,4); /* mov.l @(0x20,r9),r9 */
 goto Lcb012ca;
Lcb012ca: *sr=(*sr&~1u)|!(r[9]&r[9]); /* tst r9,r9 */
 goto Lcb012cc;
Lcb012cc: if(!(*sr&1u))goto Lcb012d4; /* bf 0x0cb012d4 */
 goto Lcb012ce;
Lcb012ce: r[0]=r[10]; /* mov r10,r0 */
 goto Lcb012d0;
Lcb012d0: wr(m,r[11]+0x4u,r[0],1);goto Lcb01364; /* bra 0x0cb01364 */
 goto Lcb012d4;
Lcb012d4: r[0]=16u; /* mov #0x10,r0 */
 goto Lcb012d6;
Lcb012d6: r[5]=r[15]; /* mov r15,r5 */
 goto Lcb012d8;
Lcb012d8: wr(m,r[0]+r[15],fr[4],4); /* fmov fr4,@(r0,r15) */
 goto Lcb012da;
Lcb012da: r[0]=12u; /* mov #0xc,r0 */
 goto Lcb012dc;
Lcb012dc: wr(m,r[0]+r[15],fr[4],4); /* fmov fr4,@(r0,r15) */
 goto Lcb012de;
Lcb012de: r[0]=8u; /* mov #0x8,r0 */
 goto Lcb012e0;
Lcb012e0: r[3]=rd(m,0x0cb013c0u,4); /* mov.l 0x0cb013c0,r3 */
 goto Lcb012e2;
Lcb012e2: r[5]+=8u; /* add #0x8,r5 */
 goto Lcb012e4;
Lcb012e4: wr(m,r[0]+r[15],fr[5],4); /* fmov fr5,@(r0,r15) */
 goto Lcb012e6;
Lcb012e6: target=r[3];*pr=0xcb012eau;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb012ea;
Lcb012ea: r[2]=rd(m,0x0cb013c8u,4); /* mov.l 0x0cb013c8,r2 */
 goto Lcb012ec;
Lcb012ec: r[4]=rd(m,0x0cb013c4u,4); /* mov.l 0x0cb013c4,r4 */
 goto Lcb012ee;
Lcb012ee: target=r[2];*pr=0xcb012f2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb012f2;
Lcb012f2: r[3]=rd(m,r[9]+0x1cu,4); /* mov.l @(0x1c,r9),r3 */
 goto Lcb012f4;
Lcb012f4: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto Lcb012f6;
Lcb012f6: take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto Lcb01302;goto Lcb012fa; /* bt/s 0x0cb01302 */
 goto Lcb012fa;
Lcb012fa: r[3]=rd(m,0x0cb013ccu,4); /* mov.l 0x0cb013cc,r3 */
 goto Lcb012fc;
Lcb012fc: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto Lcb012fe;
Lcb012fe: target=r[3];*pr=0xcb01302u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb01302;
Lcb01302: r[3]=rd(m,r[9]+0x14u,4); /* mov.l @(0x14,r9),r3 */
 goto Lcb01304;
Lcb01304: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto Lcb01306;
Lcb01306: take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto Lcb01312;goto Lcb0130a; /* bt/s 0x0cb01312 */
 goto Lcb0130a;
Lcb0130a: r[3]=rd(m,0x0cb013d0u,4); /* mov.l 0x0cb013d0,r3 */
 goto Lcb0130c;
Lcb0130c: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto Lcb0130e;
Lcb0130e: target=r[3];*pr=0xcb01312u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb01312;
Lcb01312: r[3]=rd(m,r[9]+0x18u,4); /* mov.l @(0x18,r9),r3 */
 goto Lcb01314;
Lcb01314: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto Lcb01316;
Lcb01316: take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto Lcb01322;goto Lcb0131a; /* bt/s 0x0cb01322 */
 goto Lcb0131a;
Lcb0131a: r[3]=rd(m,0x0cb013d4u,4); /* mov.l 0x0cb013d4,r3 */
 goto Lcb0131c;
Lcb0131c: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto Lcb0131e;
Lcb0131e: target=r[3];*pr=0xcb01322u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb01322;
Lcb01322: r[5]=r[11]; /* mov r11,r5 */
 goto Lcb01324;
Lcb01324: r[3]=rd(m,0x0cb013d8u,4); /* mov.l 0x0cb013d8,r3 */
 goto Lcb01326;
Lcb01326: r[6]=r[15]; /* mov r15,r6 */
 goto Lcb01328;
Lcb01328: r[5]+=20u; /* add #0x14,r5 */
 goto Lcb0132a;
Lcb0132a: r[6]+=20u; /* add #0x14,r6 */
 goto Lcb0132c;
Lcb0132c: target=r[3];*pr=0xcb01330u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb01330;
Lcb01330: r[2]=rd(m,0x0cb013dcu,4); /* mov.l 0x0cb013dc,r2 */
 goto Lcb01332;
Lcb01332: target=r[2];*pr=0xcb01336u;r[4]=1u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb01336;
Lcb01336: r[3]=rd(m,0x0cb013e0u,4); /* mov.l 0x0cb013e0,r3 */
 goto Lcb01338;
Lcb01338: r[5]=r[9]; /* mov r9,r5 */
 goto Lcb0133a;
Lcb0133a: r[4]=r[15]; /* mov r15,r4 */
 goto Lcb0133c;
Lcb0133c: r[5]+=32u; /* add #0x20,r5 */
 goto Lcb0133e;
Lcb0133e: target=r[3];*pr=0xcb01342u;r[4]+=20u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb01342;
Lcb01342: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto Lcb01344;
Lcb01344: r[4]=r[15]; /* mov r15,r4 */
 goto Lcb01346;
Lcb01346: r[3]=rd(m,0x0cb013e4u,4); /* mov.l 0x0cb013e4,r3 */
 goto Lcb01348;
Lcb01348: r[5]+=8u; /* add #0x8,r5 */
 goto Lcb0134a;
Lcb0134a: target=r[3];*pr=0xcb0134eu;r[4]+=20u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb0134e;
Lcb0134e: r[5]=r[15]; /* mov r15,r5 */
 goto Lcb01350;
Lcb01350: r[2]=rd(m,0x0cb013e8u,4); /* mov.l 0x0cb013e8,r2 */
 goto Lcb01352;
Lcb01352: r[4]=r[15]; /* mov r15,r4 */
 goto Lcb01354;
Lcb01354: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb01356;
Lcb01356: r[5]+=8u; /* add #0x8,r5 */
 goto Lcb01358;
Lcb01358: target=r[2];*pr=0xcb0135cu;r[4]+=20u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb0135c;
Lcb0135c: goto Lcb01364; /* bra 0x0cb01364 */
 goto Lcb01360;
Lcb01360: wr(m,r[4],r[10],4); /* mov.l r10,@r4 */
 goto Lcb01362;
Lcb01362: wr(m,r[13]+0x8u,r[10],4); /* mov.l r10,@(0x8,r13) */
 goto Lcb01364;
Lcb01364: r[3]=r[13]; /* mov r13,r3 */
 goto Lcb01366;
Lcb01366: r[3]+=4u; /* add #0x4,r3 */
 goto Lcb01368;
Lcb01368: r[13]+=8u; /* add #0x8,r13 */
 goto Lcb0136a;
Lcb0136a: wr(m,r[15]+0x4u,r[3],4); /* mov.l r3,@(0x4,r15) */
 goto Lcb0136c;
Lcb0136c: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto Lcb0136e;
Lcb0136e: r[2]=0u-r[2]; /* neg r2,r2 */
 goto Lcb01370;
Lcb01370: wr(m,r[14],r[2],4); /* mov.l r2,@r14 */
 goto Lcb01372;
Lcb01372: r[2]=rd(m,0x0cb013ecu,4); /* mov.l 0x0cb013ec,r2 */
 goto Lcb01374;
Lcb01374: wr(m,r[15],r[13],4); /* mov.l r13,@r15 */
 goto Lcb01376;
Lcb01376: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto Lcb01378;
Lcb01378: wr(m,r[14]+0x4u,r[3],4); /* mov.l r3,@(0x4,r14) */
 goto Lcb0137a;
Lcb0137a: target=r[2];*pr=0xcb0137eu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb0137e;
Lcb0137e: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto Lcb01380;
Lcb01380: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto Lcb01382;
Lcb01382: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto Lcb01384;
Lcb01384: r[1]+=r[2]; /* add r2,r1 */
 goto Lcb01386;
Lcb01386: wr(m,r[14]+0x10u,r[1],4); /* mov.l r1,@(0x10,r14) */
 goto Lcb01388;
Lcb01388: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto Lcb0138a;
Lcb0138a: r[2]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r2 */
 goto Lcb0138c;
Lcb0138c: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto Lcb0138e;
Lcb0138e: r[1]-=r[2]; /* sub r2,r1 */
 goto Lcb01390;
Lcb01390: wr(m,r[14]+0x14u,r[1],4); /* mov.l r1,@(0x14,r14) */
 goto Lcb01392;
Lcb01392: target=r[8];*pr=0xcb01396u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r8 */
 goto Lcb01396;
Lcb01396: goto Lcb013a0; /* bra 0x0cb013a0 */
 goto Lcb0139a;
Lcb0139a: r[2]=rd(m,0x0cb013f0u,4); /* mov.l 0x0cb013f0,r2 */
 goto Lcb0139c;
Lcb0139c: target=r[2];*pr=0xcb013a0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb013a0;
Lcb013a0: r[15]+=44u; /* add #0x2c,r15 */
 goto Lcb013a2;
Lcb013a2: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb013a4;
Lcb013a4: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto Lcb013a6;
Lcb013a6: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto Lcb013a8;
Lcb013a8: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto Lcb013aa;
Lcb013aa: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto Lcb013ac;
Lcb013ac: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto Lcb013ae;
Lcb013ae: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto Lcb013b0;
Lcb013b0: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
