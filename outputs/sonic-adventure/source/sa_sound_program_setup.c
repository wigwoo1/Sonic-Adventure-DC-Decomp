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
/* Retail indexed five-message bank/program/controller setup. */
SA_STAGE_EXPORT int sa_sound_program_setup(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c093140;
L8c093140: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c093142;
L8c093142: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c093144;
L8c093144: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c093146;
L8c093146: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c093148;
L8c093148: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c09314a;
L8c09314a: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c09314c;
L8c09314c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c09314e;
L8c09314e: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c093150;
L8c093150: r[13]=r[4]; /* mov r4,r13 */
 goto L8c093152;
L8c093152: r[4]=15u; /* mov #0xf,r4 */
 goto L8c093154;
L8c093154: *sr=(*sr&~1u)|((int32_t)r[13]>(int32_t)r[4]); /* cmp/gt r4,r13 */
 goto L8c093156;
L8c093156: r[10]=0u; /* mov #0x0,r10 */
 goto L8c093158;
L8c093158: wr(m,r[15]+0x8u,r[5],4); /* mov.l r5,@(0x8,r15) */
 goto L8c09315a;
L8c09315a: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c09315c;
L8c09315c: take=!(*sr&1u);r[9]=64u;if(take)goto L8c093162;goto L8c093160; /* bf/s 0x8c093162 */
 goto L8c093160;
L8c093160: r[10]=1u; /* mov #0x1,r10 */
 goto L8c093162;
L8c093162: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c093164;
L8c093164: r[13]&=r[4]; /* and r4,r13 */
 goto L8c093166;
L8c093166: r[3]=32u; /* mov #0x20,r3 */
 goto L8c093168;
L8c093168: r[14]=r[10]; /* mov r10,r14 */
 goto L8c09316a;
L8c09316a: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c09316c;
L8c09316c: r[2]=4294967216u; /* mov #-0x50,r2 */
 goto L8c09316e;
L8c09316e: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c093170;
L8c093170: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c093172;
L8c093172: r[2]|=r[13]; /* or r13,r2 */
 goto L8c093174;
L8c093174: r[1]=rd(m,0x8c093348u,4); /* mov.l 0x8c093348,r1 */
 goto L8c093176;
L8c093176: r[0]=r[2]; /* mov r2,r0 */
 goto L8c093178;
L8c093178: r[14]<<=2; /* shll2 r14 */
 goto L8c09317a;
L8c09317a: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c09317c;
L8c09317c: r[14]+=r[1]; /* add r1,r14 */
 goto L8c09317e;
L8c09317e: r[11]=rd(m,0x8c093340u,4); /* mov.l 0x8c093340,r11 */
 goto L8c093180;
L8c093180: wr(m,r[15]+0x8u,r[0],1); /* mov.b r0,@(0x8,r15) */
 goto L8c093182;
L8c093182: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c093184;
L8c093184: target=r[11];*pr=0x8c093188u;r[4]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c093188;
L8c093188: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c09318a;
L8c09318a: r[6]=0u; /* mov #0x0,r6 */
 goto L8c09318c;
L8c09318c: r[12]=rd(m,0x8c09334cu,4); /* mov.l 0x8c09334c,r12 */
 goto L8c09318e;
L8c09318e: target=r[12];*pr=0x8c093192u;r[4]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c093192;
L8c093192: r[2]=4294967232u; /* mov #-0x40,r2 */
 goto L8c093194;
L8c093194: r[3]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r3 */
 goto L8c093196;
L8c093196: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c093198;
L8c093198: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c09319a;
L8c09319a: r[2]|=r[13]; /* or r13,r2 */
 goto L8c09319c;
L8c09319c: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c09319e;
L8c09319e: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c0931a0;
L8c0931a0: target=r[11];*pr=0x8c0931a4u;r[4]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c0931a4;
L8c0931a4: r[15]+=20u; /* add #0x14,r15 */
 goto L8c0931a6;
L8c0931a6: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c0931a8;
L8c0931a8: r[6]=0u; /* mov #0x0,r6 */
 goto L8c0931aa;
L8c0931aa: target=r[12];*pr=0x8c0931aeu;r[4]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c0931ae;
L8c0931ae: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c0931b0;
L8c0931b0: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c0931b2;
L8c0931b2: if(!(*sr&1u))goto L8c0931d2; /* bf 0x8c0931d2 */
 goto L8c0931b4;
L8c0931b4: r[0]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c0931b6;
L8c0931b6: *sr=(*sr&~1u)|(r[0]==0x7u); /* cmp/eq #0x7,r0 */
 goto L8c0931b8;
L8c0931b8: if(!(*sr&1u))goto L8c0931d2; /* bf 0x8c0931d2 */
 goto L8c0931ba;
L8c0931ba: r[4]=rd(m,0x8c093350u,4); /* mov.l 0x8c093350,r4 */
 goto L8c0931bc;
L8c0931bc: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c0931be;
L8c0931be: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c0931c0;
L8c0931c0: if((*sr&1u))goto L8c0931c6; /* bt 0x8c0931c6 */
 goto L8c0931c2;
L8c0931c2: r[9]=94u;goto L8c0931c8; /* bra 0x8c0931c8 */
 goto L8c0931c6;
L8c0931c6: r[9]=34u; /* mov #0x22,r9 */
 goto L8c0931c8;
L8c0931c8: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c0931ca;
L8c0931ca: r[2]+=1u; /* add #0x1,r2 */
 goto L8c0931cc;
L8c0931cc: r[0]=r[2]; /* mov r2,r0 */
 goto L8c0931ce;
L8c0931ce: r[0]&=0x1u; /* and #0x1,r0 */
 goto L8c0931d0;
L8c0931d0: wr(m,r[4],r[0],4); /* mov.l r0,@r4 */
 goto L8c0931d2;
L8c0931d2: r[3]=10u; /* mov #0xa,r3 */
 goto L8c0931d4;
L8c0931d4: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c0931d6;
L8c0931d6: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c0931d8;
L8c0931d8: r[14]=r[10]; /* mov r10,r14 */
 goto L8c0931da;
L8c0931da: r[0]=sx(m,r[15]+0x8u,1); /* mov.b @(0x8,r15),r0 */
 goto L8c0931dc;
L8c0931dc: r[14]<<=2; /* shll2 r14 */
 goto L8c0931de;
L8c0931de: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c0931e0;
L8c0931e0: r[2]=r[0]; /* mov r0,r2 */
 goto L8c0931e2;
L8c0931e2: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c0931e4;
L8c0931e4: r[2]=rd(m,0x8c093348u,4); /* mov.l 0x8c093348,r2 */
 goto L8c0931e6;
L8c0931e6: r[14]+=r[2]; /* add r2,r14 */
 goto L8c0931e8;
L8c0931e8: target=r[11];*pr=0x8c0931ecu;r[4]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c0931ec;
L8c0931ec: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c0931ee;
L8c0931ee: r[6]=0u; /* mov #0x0,r6 */
 goto L8c0931f0;
L8c0931f0: target=r[12];*pr=0x8c0931f4u;r[4]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c0931f4;
L8c0931f4: r[1]=4294967264u; /* mov #-0x20,r1 */
 goto L8c0931f6;
L8c0931f6: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c0931f8;
L8c0931f8: r[3]=64u; /* mov #0x40,r3 */
 goto L8c0931fa;
L8c0931fa: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c0931fc;
L8c0931fc: r[1]=r[1]&255u; /* extu.b r1,r1 */
 goto L8c0931fe;
L8c0931fe: r[2]=0u; /* mov #0x0,r2 */
 goto L8c093200;
L8c093200: r[13]|=r[1]; /* or r1,r13 */
 goto L8c093202;
L8c093202: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c093204;
L8c093204: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c093206;
L8c093206: target=r[11];*pr=0x8c09320au;r[4]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c09320a;
L8c09320a: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c09320c;
L8c09320c: r[6]=0u; /* mov #0x0,r6 */
 goto L8c09320e;
L8c09320e: target=r[12];*pr=0x8c093212u;r[4]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c093212;
L8c093212: r[3]=127u; /* mov #0x7f,r3 */
 goto L8c093214;
L8c093214: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c093216;
L8c093216: r[2]=1u; /* mov #0x1,r2 */
 goto L8c093218;
L8c093218: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c09321a;
L8c09321a: r[0]=32u; /* mov #0x20,r0 */
 goto L8c09321c;
L8c09321c: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c09321e;
L8c09321e: r[1]=sx(m,r[15]+r[0],1); /* mov.b @(r0,r15),r1 */
 goto L8c093220;
L8c093220: r[15]-=4;wr(m,r[15],r[1],4); /* mov.l r1,@-r15 */
 goto L8c093222;
L8c093222: target=r[11];*pr=0x8c093226u;r[4]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c093226;
L8c093226: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c093228;
L8c093228: r[6]=0u; /* mov #0x0,r6 */
 goto L8c09322a;
L8c09322a: target=r[12];*pr=0x8c09322eu;r[4]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c09322e;
L8c09322e: r[15]+=48u; /* add #0x30,r15 */
 goto L8c093230;
L8c093230: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c093232;
L8c093232: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c093234;
L8c093234: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c093236;
L8c093236: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c093238;
L8c093238: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c09323a;
L8c09323a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c09323c;
L8c09323c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
