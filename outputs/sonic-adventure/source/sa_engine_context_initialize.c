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
/* Static C recovery of retail Tikal helper 8c6510a4..8c651368.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_context_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6510a4;
L8c6510a4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6510a6;
L8c6510a6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6510a8;
L8c6510a8: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6510aa;
L8c6510aa: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c6510ac;
L8c6510ac: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c6510ae;
L8c6510ae: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c6510b0;
L8c6510b0: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c6510b2;
L8c6510b2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6510b4;
L8c6510b4: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c6510b6;
L8c6510b6: r[2]=rd(m,0x8c6511e4u,4); /* mov.l 0x8c6511e4,r2 */
 goto L8c6510b8;
L8c6510b8: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c6510ba;
L8c6510ba: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c6510bc;
L8c6510bc: if(!(*sr&1u))goto L8c6510ce; /* bf 0x8c6510ce */
 goto L8c6510be;
L8c6510be: r[1]=rd(m,0x8c6511e4u,4); /* mov.l 0x8c6511e4,r1 */
 goto L8c6510c0;
L8c6510c0: r[3]=rd(m,0x8c6511e8u,4); /* mov.l 0x8c6511e8,r3 */
 goto L8c6510c2;
L8c6510c2: wr(m,r[1],r[3],4); /* mov.l r3,@r1 */
 goto L8c6510c4;
L8c6510c4: r[3]=rd(m,0x8c6511ecu,4); /* mov.l 0x8c6511ec,r3 */
 goto L8c6510c6;
L8c6510c6: r[5]=0u; /* mov #0x0,r5 */
 goto L8c6510c8;
L8c6510c8: r[6]=sx(m,0x8c6511e0u,2); /* mov.w 0x8c6511e0,r6 */
 goto L8c6510ca;
L8c6510ca: target=r[3];*pr=0x8c6510ceu;r[4]=rd(m,r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6510ce;
L8c6510ce: r[4]=rd(m,0x8c6511f0u,4); /* mov.l 0x8c6511f0,r4 */
 goto L8c6510d0;
L8c6510d0: r[3]=3u; /* mov #0x3,r3 */
 goto L8c6510d2;
L8c6510d2: r[2]=6u; /* mov #0x6,r2 */
 goto L8c6510d4;
L8c6510d4: r[1]=rd(m,0x8c6511e8u,4); /* mov.l 0x8c6511e8,r1 */
 goto L8c6510d6;
L8c6510d6: wr(m,r[4],r[3],4); /* mov.l r3,@r4 */
 goto L8c6510d8;
L8c6510d8: r[5]=2u; /* mov #0x2,r5 */
 goto L8c6510da;
L8c6510da: r[3]=rd(m,0x8c6511f4u,4); /* mov.l 0x8c6511f4,r3 */
 goto L8c6510dc;
L8c6510dc: wr(m,r[4]+0x4u,r[5],4); /* mov.l r5,@(0x4,r4) */
 goto L8c6510de;
L8c6510de: wr(m,r[4]+0x8u,r[2],4); /* mov.l r2,@(0x8,r4) */
 goto L8c6510e0;
L8c6510e0: r[2]=r[4]; /* mov r4,r2 */
 goto L8c6510e2;
L8c6510e2: wr(m,r[4]+0xcu,r[5],4); /* mov.l r5,@(0xc,r4) */
 goto L8c6510e4;
L8c6510e4: wr(m,r[4]+0x10u,r[3],4); /* mov.l r3,@(0x10,r4) */
 goto L8c6510e6;
L8c6510e6: wr(m,r[1],r[4],4); /* mov.l r4,@r1 */
 goto L8c6510e8;
L8c6510e8: r[3]=rd(m,0x8c6511f8u,4); /* mov.l 0x8c6511f8,r3 */
 goto L8c6510ea;
L8c6510ea: r[14]=0u; /* mov #0x0,r14 */
 goto L8c6510ec;
L8c6510ec: wr(m,r[3],r[14],4); /* mov.l r14,@r3 */
 goto L8c6510ee;
L8c6510ee: r[2]=rd(m,0x8c6511fcu,4); /* mov.l 0x8c6511fc,r2 */
 goto L8c6510f0;
L8c6510f0: wr(m,r[2],r[14],4); /* mov.l r14,@r2 */
 goto L8c6510f2;
L8c6510f2: r[1]+=60u; /* add #0x3c,r1 */
 goto L8c6510f4;
L8c6510f4: wr(m,r[1],r[14],4); /* mov.l r14,@r1 */
 goto L8c6510f6;
L8c6510f6: r[3]+=28u; /* add #0x1c,r3 */
 goto L8c6510f8;
L8c6510f8: wr(m,r[3],r[14],4); /* mov.l r14,@r3 */
 goto L8c6510fa;
L8c6510fa: r[1]=rd(m,0x8c651200u,4); /* mov.l 0x8c651200,r1 */
 goto L8c6510fc;
L8c6510fc: r[2]+=28u; /* add #0x1c,r2 */
 goto L8c6510fe;
L8c6510fe: target=r[1];*pr=0x8c651102u;wr(m,r[2],r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c651102;
L8c651102: r[3]=rd(m,0x8c651204u,4); /* mov.l 0x8c651204,r3 */
 goto L8c651104;
L8c651104: wr(m,r[3],r[14],4); /* mov.l r14,@r3 */
 goto L8c651106;
L8c651106: r[2]=rd(m,0x8c651208u,4); /* mov.l 0x8c651208,r2 */
 goto L8c651108;
L8c651108: wr(m,r[2],r[14],4); /* mov.l r14,@r2 */
 goto L8c65110a;
L8c65110a: r[5]=rd(m,0x8c651214u,4); /* mov.l 0x8c651214,r5 */
 goto L8c65110c;
L8c65110c: r[6]=1u; /* mov #0x1,r6 */
 goto L8c65110e;
L8c65110e: r[13]=rd(m,0x8c651210u,4); /* mov.l 0x8c651210,r13 */
 goto L8c651110;
L8c651110: r[12]=rd(m,0x8c65120cu,4); /* mov.l 0x8c65120c,r12 */
 goto L8c651112;
L8c651112: target=r[13];*pr=0x8c651116u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c651116;
L8c651116: r[3]=rd(m,0x8c651218u,4); /* mov.l 0x8c651218,r3 */
 goto L8c651118;
L8c651118: r[0]+=4294967295u; /* add #-0x1,r0 */
 goto L8c65111a;
L8c65111a: wr(m,r[3],r[0],4); /* mov.l r0,@r3 */
 goto L8c65111c;
L8c65111c: r[1]=r[3]; /* mov r3,r1 */
 goto L8c65111e;
L8c65111e: r[2]=rd(m,r[1],4); /* mov.l @r1,r2 */
 goto L8c651120;
L8c651120: r[0]=rd(m,0x8c65121cu,4); /* mov.l 0x8c65121c,r0 */
 goto L8c651122;
L8c651122: wr(m,r[0],r[2],4); /* mov.l r2,@r0 */
 goto L8c651124;
L8c651124: r[2]=rd(m,0x8c651224u,4); /* mov.l 0x8c651224,r2 */
 goto L8c651126;
L8c651126: r[3]=rd(m,0x8c651220u,4); /* mov.l 0x8c651220,r3 */
 goto L8c651128;
L8c651128: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c65112a;
L8c65112a: r[3]=rd(m,0x8c65122cu,4); /* mov.l 0x8c65122c,r3 */
 goto L8c65112c;
L8c65112c: r[4]=rd(m,0x8c651228u,4); /* mov.l 0x8c651228,r4 */
 goto L8c65112e;
L8c65112e: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c651130;
L8c651130: r[1]=rd(m,0x8c651230u,4); /* mov.l 0x8c651230,r1 */
 goto L8c651132;
L8c651132: wr(m,r[1],r[4],4); /* mov.l r4,@r1 */
 goto L8c651134;
L8c651134: r[2]=rd(m,0x8c651234u,4); /* mov.l 0x8c651234,r2 */
 goto L8c651136;
L8c651136: wr(m,r[2],r[14],4); /* mov.l r14,@r2 */
 goto L8c651138;
L8c651138: r[3]=rd(m,0x8c651238u,4); /* mov.l 0x8c651238,r3 */
 goto L8c65113a;
L8c65113a: wr(m,r[3],r[14],4); /* mov.l r14,@r3 */
 goto L8c65113c;
L8c65113c: r[1]=rd(m,0x8c65123cu,4); /* mov.l 0x8c65123c,r1 */
 goto L8c65113e;
L8c65113e: wr(m,r[1],r[14],4); /* mov.l r14,@r1 */
 goto L8c651140;
L8c651140: r[0]=rd(m,0x8c651240u,4); /* mov.l 0x8c651240,r0 */
 goto L8c651142;
L8c651142: r[2]=3u; /* mov #0x3,r2 */
 goto L8c651144;
L8c651144: wr(m,r[0],r[2],4); /* mov.l r2,@r0 */
 goto L8c651146;
L8c651146: r[1]=rd(m,0x8c651248u,4); /* mov.l 0x8c651248,r1 */
 goto L8c651148;
L8c651148: r[3]=1u; /* mov #0x1,r3 */
 goto L8c65114a;
L8c65114a: r[2]=rd(m,0x8c651244u,4); /* mov.l 0x8c651244,r2 */
 goto L8c65114c;
L8c65114c: target=r[1];*pr=0x8c651150u;wr(m,r[2],r[3],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c651150;
L8c651150: r[5]=rd(m,0x8c65124cu,4); /* mov.l 0x8c65124c,r5 */
 goto L8c651152;
L8c651152: r[6]=sx(m,0x8c6511e2u,2); /* mov.w 0x8c6511e2,r6 */
 goto L8c651154;
L8c651154: target=r[13];*pr=0x8c651158u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c651158;
L8c651158: r[5]=rd(m,0x8c651254u,4); /* mov.l 0x8c651254,r5 */
 goto L8c65115a;
L8c65115a: r[6]=rd(m,0x8c651250u,4); /* mov.l 0x8c651250,r6 */
 goto L8c65115c;
L8c65115c: wr(m,r[15]+0x8u,r[0],4); /* mov.l r0,@(0x8,r15) */
 goto L8c65115e;
L8c65115e: target=r[13];*pr=0x8c651162u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c651162;
L8c651162: r[5]=rd(m,0x8c65125cu,4); /* mov.l 0x8c65125c,r5 */
 goto L8c651164;
L8c651164: r[6]=rd(m,0x8c651258u,4); /* mov.l 0x8c651258,r6 */
 goto L8c651166;
L8c651166: wr(m,r[15]+0x4u,r[0],4); /* mov.l r0,@(0x4,r15) */
 goto L8c651168;
L8c651168: target=r[13];*pr=0x8c65116cu;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c65116c;
L8c65116c: r[5]=rd(m,0x8c651260u,4); /* mov.l 0x8c651260,r5 */
 goto L8c65116e;
L8c65116e: r[6]=rd(m,0x8c651258u,4); /* mov.l 0x8c651258,r6 */
 goto L8c651170;
L8c651170: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c651172;
L8c651172: target=r[13];*pr=0x8c651176u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c651176;
L8c651176: r[6]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r6 */
 goto L8c651178;
L8c651178: r[5]=r[0]; /* mov r0,r5 */
 goto L8c65117a;
L8c65117a: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c65117c;
L8c65117c: r[5]>>=8; /* shlr8 r5 */
 goto L8c65117e;
L8c65117e: r[11]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r11 */
 goto L8c651180;
L8c651180: r[6]>>=8; /* shlr8 r6 */
 goto L8c651182;
L8c651182: r[4]>>=8; /* shlr8 r4 */
 goto L8c651184;
L8c651184: r[2]=rd(m,0x8c651264u,4); /* mov.l 0x8c651264,r2 */
 goto L8c651186;
L8c651186: r[11]>>=8; /* shlr8 r11 */
 goto L8c651188;
L8c651188: r[6]>>=2; /* shlr2 r6 */
 goto L8c65118a;
L8c65118a: r[11]>>=2; /* shlr2 r11 */
 goto L8c65118c;
L8c65118c: r[4]>>=2; /* shlr2 r4 */
 goto L8c65118e;
L8c65118e: r[5]>>=2; /* shlr2 r5 */
 goto L8c651190;
L8c651190: *sr=(*sr&~1u)|(r[11]&1u);r[11]>>=1; /* shlr r11 */
 goto L8c651192;
L8c651192: r[11]<<=8; /* shll8 r11 */
 goto L8c651194;
L8c651194: r[11]<<=2; /* shll2 r11 */
 goto L8c651196;
L8c651196: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1; /* shll r11 */
 goto L8c651198;
L8c651198: *sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1; /* shlr r6 */
 goto L8c65119a;
L8c65119a: r[6]<<=8; /* shll8 r6 */
 goto L8c65119c;
L8c65119c: r[6]<<=2; /* shll2 r6 */
 goto L8c65119e;
L8c65119e: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1; /* shll r6 */
 goto L8c6511a0;
L8c6511a0: *sr=(*sr&~1u)|(r[4]&1u);r[4]>>=1; /* shlr r4 */
 goto L8c6511a2;
L8c6511a2: r[4]<<=8; /* shll8 r4 */
 goto L8c6511a4;
L8c6511a4: r[4]<<=2; /* shll2 r4 */
 goto L8c6511a6;
L8c6511a6: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c6511a8;
L8c6511a8: *sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1; /* shlr r5 */
 goto L8c6511aa;
L8c6511aa: r[5]<<=8; /* shll8 r5 */
 goto L8c6511ac;
L8c6511ac: r[5]<<=2; /* shll2 r5 */
 goto L8c6511ae;
L8c6511ae: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1; /* shll r5 */
 goto L8c6511b0;
L8c6511b0: wr(m,r[2],r[4],4); /* mov.l r4,@r2 */
 goto L8c6511b2;
L8c6511b2: r[3]=rd(m,0x8c651268u,4); /* mov.l 0x8c651268,r3 */
 goto L8c6511b4;
L8c6511b4: wr(m,r[3],r[5],4); /* mov.l r5,@r3 */
 goto L8c6511b6;
L8c6511b6: r[7]=r[11]; /* mov r11,r7 */
 goto L8c6511b8;
L8c6511b8: r[7]+=r[6]; /* add r6,r7 */
 goto L8c6511ba;
L8c6511ba: r[4]+=r[7]; /* add r7,r4 */
 goto L8c6511bc;
L8c6511bc: r[8]=rd(m,0x8c65126cu,4); /* mov.l 0x8c65126c,r8 */
 goto L8c6511be;
L8c6511be: r[10]=rd(m,0x8c651270u,4); /* mov.l 0x8c651270,r10 */
 goto L8c6511c0;
L8c6511c0: r[6]=r[14]; /* mov r14,r6 */
 goto L8c6511c2;
L8c6511c2: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c6511c4;
L8c6511c4: r[4]=r[14]; /* mov r14,r4 */
 goto L8c6511c6;
L8c6511c6: r[5]=r[14]; /* mov r14,r5 */
 goto L8c6511c8;
L8c6511c8: r[9]=32u;goto L8c6512b0; /* bra 0x8c6512b0 */
 goto L8c651280;
L8c651280: r[0]=rd(m,0x8c651420u,4); /* mov.l 0x8c651420,r0 */
 goto L8c651282;
L8c651282: wr(m,r[4]+r[0],r[5],4); /* mov.l r5,@(r0,r4) */
 goto L8c651284;
L8c651284: r[2]=r[5]; /* mov r5,r2 */
 goto L8c651286;
L8c651286: r[3]=r[0]; /* mov r0,r3 */
 goto L8c651288;
L8c651288: r[2]+=r[11]; /* add r11,r2 */
 goto L8c65128a;
L8c65128a: r[3]+=r[4]; /* add r4,r3 */
 goto L8c65128c;
L8c65128c: wr(m,r[3]+0x4u,r[2],4); /* mov.l r2,@(0x4,r3) */
 goto L8c65128e;
L8c65128e: r[2]=r[5]; /* mov r5,r2 */
 goto L8c651290;
L8c651290: r[3]=r[0]; /* mov r0,r3 */
 goto L8c651292;
L8c651292: r[2]+=r[7]; /* add r7,r2 */
 goto L8c651294;
L8c651294: r[3]+=r[4]; /* add r4,r3 */
 goto L8c651296;
L8c651296: wr(m,r[3]+0x8u,r[2],4); /* mov.l r2,@(0x8,r3) */
 goto L8c651298;
L8c651298: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c65129a;
L8c65129a: r[3]=r[0]; /* mov r0,r3 */
 goto L8c65129c;
L8c65129c: r[3]+=r[4]; /* add r4,r3 */
 goto L8c65129e;
L8c65129e: r[2]+=r[5]; /* add r5,r2 */
 goto L8c6512a0;
L8c6512a0: wr(m,r[3]+0xcu,r[2],4); /* mov.l r2,@(0xc,r3) */
 goto L8c6512a2;
L8c6512a2: r[3]=r[8]; /* mov r8,r3 */
 goto L8c6512a4;
L8c6512a4: r[0]=rd(m,0x8c651424u,4); /* mov.l 0x8c651424,r0 */
 goto L8c6512a6;
L8c6512a6: r[3]-=r[5]; /* sub r5,r3 */
 goto L8c6512a8;
L8c6512a8: wr(m,r[6]+r[0],r[3],4); /* mov.l r3,@(r0,r6) */
 goto L8c6512aa;
L8c6512aa: r[4]+=16u; /* add #0x10,r4 */
 goto L8c6512ac;
L8c6512ac: r[5]+=r[10]; /* add r10,r5 */
 goto L8c6512ae;
L8c6512ae: r[6]+=4u; /* add #0x4,r6 */
 goto L8c6512b0;
L8c6512b0: *sr=(*sr&~1u)|(r[4]>=r[9]); /* cmp/hs r9,r4 */
 goto L8c6512b2;
L8c6512b2: if(!(*sr&1u))goto L8c651280; /* bf 0x8c651280 */
 goto L8c6512b4;
L8c6512b4: r[3]=rd(m,0x8c651428u,4); /* mov.l 0x8c651428,r3 */
 goto L8c6512b6;
L8c6512b6: wr(m,r[3],r[14],4); /* mov.l r14,@r3 */
 goto L8c6512b8;
L8c6512b8: r[2]=rd(m,0x8c65142cu,4); /* mov.l 0x8c65142c,r2 */
 goto L8c6512ba;
L8c6512ba: wr(m,r[2],r[14],4); /* mov.l r14,@r2 */
 goto L8c6512bc;
L8c6512bc: r[1]=rd(m,0x8c651430u,4); /* mov.l 0x8c651430,r1 */
 goto L8c6512be;
L8c6512be: wr(m,r[1],r[14],4); /* mov.l r14,@r1 */
 goto L8c6512c0;
L8c6512c0: r[3]+=12u; /* add #0xc,r3 */
 goto L8c6512c2;
L8c6512c2: r[4]=4294967295u; /* mov #-0x1,r4 */
 goto L8c6512c4;
L8c6512c4: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c6512c6;
L8c6512c6: r[2]=rd(m,0x8c651434u,4); /* mov.l 0x8c651434,r2 */
 goto L8c6512c8;
L8c6512c8: wr(m,r[2],r[4],4); /* mov.l r4,@r2 */
 goto L8c6512ca;
L8c6512ca: r[1]+=12u; /* add #0xc,r1 */
 goto L8c6512cc;
L8c6512cc: wr(m,r[1],r[4],4); /* mov.l r4,@r1 */
 goto L8c6512ce;
L8c6512ce: r[3]+=12u; /* add #0xc,r3 */
 goto L8c6512d0;
L8c6512d0: wr(m,r[3],r[14],4); /* mov.l r14,@r3 */
 goto L8c6512d2;
L8c6512d2: r[2]+=12u; /* add #0xc,r2 */
 goto L8c6512d4;
L8c6512d4: wr(m,r[2],r[14],4); /* mov.l r14,@r2 */
 goto L8c6512d6;
L8c6512d6: r[1]+=12u; /* add #0xc,r1 */
 goto L8c6512d8;
L8c6512d8: wr(m,r[1],r[14],4); /* mov.l r14,@r1 */
 goto L8c6512da;
L8c6512da: r[3]=rd(m,0x8c651438u,4); /* mov.l 0x8c651438,r3 */
 goto L8c6512dc;
L8c6512dc: wr(m,r[3],r[14],4); /* mov.l r14,@r3 */
 goto L8c6512de;
L8c6512de: r[2]=rd(m,0x8c65143cu,4); /* mov.l 0x8c65143c,r2 */
 goto L8c6512e0;
L8c6512e0: wr(m,r[2],r[14],4); /* mov.l r14,@r2 */
 goto L8c6512e2;
L8c6512e2: r[5]=rd(m,0x8c651448u,4); /* mov.l 0x8c651448,r5 */
 goto L8c6512e4;
L8c6512e4: r[1]=1u; /* mov #0x1,r1 */
 goto L8c6512e6;
L8c6512e6: r[4]=rd(m,0x8c65144cu,4); /* mov.l 0x8c65144c,r4 */
 goto L8c6512e8;
L8c6512e8: r[6]=rd(m,0x8c651444u,4); /* mov.l 0x8c651444,r6 */
 goto L8c6512ea;
L8c6512ea: r[0]=rd(m,0x8c651440u,4); /* mov.l 0x8c651440,r0 */
 goto L8c6512ec;
L8c6512ec: target=r[13];*pr=0x8c6512f0u;wr(m,r[0],r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c6512f0;
L8c6512f0: r[3]=rd(m,0x8c651450u,4); /* mov.l 0x8c651450,r3 */
 goto L8c6512f2;
L8c6512f2: wr(m,r[3],r[0],4); /* mov.l r0,@r3 */
 goto L8c6512f4;
L8c6512f4: r[4]=rd(m,0x8c65144cu,4); /* mov.l 0x8c65144c,r4 */
 goto L8c6512f6;
L8c6512f6: r[5]=rd(m,0x8c651454u,4); /* mov.l 0x8c651454,r5 */
 goto L8c6512f8;
L8c6512f8: target=r[13];*pr=0x8c6512fcu;r[6]=r[10];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c6512fc;
L8c6512fc: r[2]=rd(m,0x8c651458u,4); /* mov.l 0x8c651458,r2 */
 goto L8c6512fe;
L8c6512fe: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c651300;
L8c651300: r[5]=rd(m,0x8c65145cu,4); /* mov.l 0x8c65145c,r5 */
 goto L8c651302;
L8c651302: r[6]=0u; /* mov #0x0,r6 */
 goto L8c651304;
L8c651304: target=r[13];*pr=0x8c651308u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c651308;
L8c651308: r[3]=rd(m,0x8c651460u,4); /* mov.l 0x8c651460,r3 */
 goto L8c65130a;
L8c65130a: wr(m,r[3],r[0],4); /* mov.l r0,@r3 */
 goto L8c65130c;
L8c65130c: r[5]=rd(m,0x8c651464u,4); /* mov.l 0x8c651464,r5 */
 goto L8c65130e;
L8c65130e: r[6]=10u; /* mov #0xa,r6 */
 goto L8c651310;
L8c651310: target=r[13];*pr=0x8c651314u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c651314;
L8c651314: r[2]=rd(m,0x8c651468u,4); /* mov.l 0x8c651468,r2 */
 goto L8c651316;
L8c651316: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c651318;
L8c651318: r[5]=rd(m,0x8c65146cu,4); /* mov.l 0x8c65146c,r5 */
 goto L8c65131a;
L8c65131a: r[6]=10u; /* mov #0xa,r6 */
 goto L8c65131c;
L8c65131c: target=r[13];*pr=0x8c651320u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c651320;
L8c651320: r[3]=rd(m,0x8c651470u,4); /* mov.l 0x8c651470,r3 */
 goto L8c651322;
L8c651322: wr(m,r[3],r[0],4); /* mov.l r0,@r3 */
 goto L8c651324;
L8c651324: r[5]=rd(m,0x8c651474u,4); /* mov.l 0x8c651474,r5 */
 goto L8c651326;
L8c651326: r[6]=10u; /* mov #0xa,r6 */
 goto L8c651328;
L8c651328: target=r[13];*pr=0x8c65132cu;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c65132c;
L8c65132c: r[2]=rd(m,0x8c651478u,4); /* mov.l 0x8c651478,r2 */
 goto L8c65132e;
L8c65132e: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c651330;
L8c651330: r[3]=rd(m,0x8c65147cu,4); /* mov.l 0x8c65147c,r3 */
 goto L8c651332;
L8c651332: wr(m,r[3],r[14],4); /* mov.l r14,@r3 */
 goto L8c651334;
L8c651334: r[0]=rd(m,0x8c651480u,4); /* mov.l 0x8c651480,r0 */
 goto L8c651336;
L8c651336: r[1]=1u; /* mov #0x1,r1 */
 goto L8c651338;
L8c651338: wr(m,r[0],r[1],4); /* mov.l r1,@r0 */
 goto L8c65133a;
L8c65133a: r[1]=rd(m,0x8c651484u,4); /* mov.l 0x8c651484,r1 */
 goto L8c65133c;
L8c65133c: r[2]=15u; /* mov #0xf,r2 */
 goto L8c65133e;
L8c65133e: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c651340;
L8c651340: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c651342;
L8c651342: r[3]|=r[14]; /* or r14,r3 */
 goto L8c651344;
L8c651344: wr(m,r[1],r[3],4); /* mov.l r3,@r1 */
 goto L8c651346;
L8c651346: r[2]=rd(m,0x8c651488u,4); /* mov.l 0x8c651488,r2 */
 goto L8c651348;
L8c651348: wr(m,r[2],r[14],4); /* mov.l r14,@r2 */
 goto L8c65134a;
L8c65134a: r[3]=rd(m,0x8c65148cu,4); /* mov.l 0x8c65148c,r3 */
 goto L8c65134c;
L8c65134c: wr(m,r[3],r[14],4); /* mov.l r14,@r3 */
 goto L8c65134e;
L8c65134e: r[3]=rd(m,0x8c651490u,4); /* mov.l 0x8c651490,r3 */
 goto L8c651350;
L8c651350: target=r[3];*pr=0x8c651354u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c651354;
L8c651354: r[15]+=12u; /* add #0xc,r15 */
 goto L8c651356;
L8c651356: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c651358;
L8c651358: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c65135a;
L8c65135a: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65135c;
L8c65135c: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65135e;
L8c65135e: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c651360;
L8c651360: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c651362;
L8c651362: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c651364;
L8c651364: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
