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
/* Retail input provider65b88c..65b930. Queries each channel
 * through a virtual method and fills offset/capacity/remaining outputs.
 * Compilation alone does not establish recovered behavior. */
SA_STAGE_EXPORT int sa_sound_stream_input_provider(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65b88c;
L8c65b88c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65b88e;
L8c65b88e: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65b890;
L8c65b890: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65b892;
L8c65b892: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65b894;
L8c65b894: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65b896;
L8c65b896: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65b898;
L8c65b898: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c65b89a;
L8c65b89a: r[14]=r[4]; /* mov r4,r14 */
 goto L8c65b89c;
L8c65b89c: r[10]=rd(m,0x8c65b964u,4); /* mov.l 0x8c65b964,r10 */
 goto L8c65b89e;
L8c65b89e: r[11]=sx(m,0x8c65b95au,2); /* mov.w 0x8c65b95a,r11 */
 goto L8c65b8a0;
L8c65b8a0: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c65b8a2;
L8c65b8a2: wr(m,r[15]+0x8u,r[6],4); /* mov.l r6,@(0x8,r15) */
 goto L8c65b8a4;
L8c65b8a4: wr(m,r[15],r[7],4); /* mov.l r7,@r15 */
 goto L8c65b8a6;
L8c65b8a6: r[3]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r3 */
 goto L8c65b8a8;
L8c65b8a8: wr(m,r[15]+0xcu,r[3],4); /* mov.l r3,@(0xc,r15) */
 goto L8c65b8aa;
L8c65b8aa: r[13]=0u;goto L8c65b8d4; /* bra 0x8c65b8d4 */
 goto L8c65b8ae;
L8c65b8ae: r[3]=r[14]; /* mov r14,r3 */
 goto L8c65b8b0;
L8c65b8b0: r[12]=r[13]; /* mov r13,r12 */
 goto L8c65b8b2;
L8c65b8b2: r[3]+=12u; /* add #0xc,r3 */
 goto L8c65b8b4;
L8c65b8b4: r[7]=r[13]; /* mov r13,r7 */
 goto L8c65b8b6;
L8c65b8b6: r[12]<<=2; /* shll2 r12 */
 goto L8c65b8b8;
L8c65b8b8: r[6]=r[11]; /* mov r11,r6 */
 goto L8c65b8ba;
L8c65b8ba: r[12]+=r[3]; /* add r3,r12 */
 goto L8c65b8bc;
L8c65b8bc: r[12]=rd(m,r[12],4); /* mov.l @r12,r12 */
 goto L8c65b8be;
L8c65b8be: r[7]<<=2; /* shll2 r7 */
 goto L8c65b8c0;
L8c65b8c0: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1; /* shll r7 */
 goto L8c65b8c2;
L8c65b8c2: r[2]=rd(m,r[12],4); /* mov.l @r12,r2 */
 goto L8c65b8c4;
L8c65b8c4: r[5]=0u; /* mov #0x0,r5 */
 goto L8c65b8c6;
L8c65b8c6: r[3]=rd(m,r[2]+0x18u,4); /* mov.l @(0x18,r2),r3 */
 goto L8c65b8c8;
L8c65b8c8: r[2]=r[14]; /* mov r14,r2 */
 goto L8c65b8ca;
L8c65b8ca: r[2]+=28u; /* add #0x1c,r2 */
 goto L8c65b8cc;
L8c65b8cc: r[7]+=r[2]; /* add r2,r7 */
 goto L8c65b8ce;
L8c65b8ce: target=r[3];*pr=0x8c65b8d2u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65b8d2;
L8c65b8d2: r[13]+=1u; /* add #0x1,r13 */
 goto L8c65b8d4;
L8c65b8d4: target=r[10];*pr=0x8c65b8d8u;r[4]=rd(m,r[14]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto L8c65b8d8;
L8c65b8d8: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[0]); /* cmp/ge r0,r13 */
 goto L8c65b8da;
L8c65b8da: if(!(*sr&1u))goto L8c65b8ae; /* bf 0x8c65b8ae */
 goto L8c65b8dc;
L8c65b8dc: r[3]=rd(m,0x8c65b968u,4); /* mov.l 0x8c65b968,r3 */
 goto L8c65b8de;
L8c65b8de: target=r[3];*pr=0x8c65b8e2u;r[4]=rd(m,r[15]+0xcu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65b8e2;
L8c65b8e2: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65b8e4;
L8c65b8e4: r[4]=r[0]; /* mov r0,r4 */
 goto L8c65b8e6;
L8c65b8e6: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c65b8e8;
L8c65b8e8: r[3]-=r[4]; /* sub r4,r3 */
 goto L8c65b8ea;
L8c65b8ea: *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1; /* shlr r3 */
 goto L8c65b8ec;
L8c65b8ec: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c65b8ee;
L8c65b8ee: r[4]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r4 */
 goto L8c65b8f0;
L8c65b8f0: r[5]=rd(m,r[14]+0x38u,4); /* mov.l @(0x38,r14),r5 */
 goto L8c65b8f2;
L8c65b8f2: *sr=(*sr&~1u)|(r[4]&1u);r[4]>>=1; /* shlr r4 */
 goto L8c65b8f4;
L8c65b8f4: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]); /* cmp/ge r5,r4 */
 goto L8c65b8f6;
L8c65b8f6: take=(*sr&1u);r[2]=rd(m,r[15]+0x8u,4);if(take)goto L8c65b8fe;goto L8c65b8fa; /* bt/s 0x8c65b8fe */
 goto L8c65b8fa;
L8c65b8fa: r[3]=r[4];goto L8c65b900; /* bra 0x8c65b900 */
 goto L8c65b8fe;
L8c65b8fe: r[3]=r[5]; /* mov r5,r3 */
 goto L8c65b900;
L8c65b900: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c65b902;
L8c65b902: r[2]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r2 */
 goto L8c65b904;
L8c65b904: *sr=(*sr&~1u)|((int32_t)r[2]>=0); /* cmp/pz r2 */
 goto L8c65b906;
L8c65b906: if(!(*sr&1u))goto L8c65b916; /* bf 0x8c65b916 */
 goto L8c65b908;
L8c65b908: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65b90a;
L8c65b90a: r[2]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r2 */
 goto L8c65b90c;
L8c65b90c: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c65b90e;
L8c65b90e: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c65b910;
L8c65b910: r[2]-=r[3]; /* sub r3,r2 */
 goto L8c65b912;
L8c65b912: wr(m,r[1],r[2],4);goto L8c65b91c; /* bra 0x8c65b91c */
 goto L8c65b916;
L8c65b916: r[3]=rd(m,0x8c65b96cu,4); /* mov.l 0x8c65b96c,r3 */
 goto L8c65b918;
L8c65b918: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c65b91a;
L8c65b91a: wr(m,r[1],r[3],4); /* mov.l r3,@r1 */
 goto L8c65b91c;
L8c65b91c: r[4]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r4 */
 goto L8c65b91e;
L8c65b91e: r[15]+=16u; /* add #0x10,r15 */
 goto L8c65b920;
L8c65b920: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65b922;
L8c65b922: r[2]=rd(m,0x8c65b970u,4); /* mov.l 0x8c65b970,r2 */
 goto L8c65b924;
L8c65b924: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65b926;
L8c65b926: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65b928;
L8c65b928: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65b92a;
L8c65b92a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65b92c;
L8c65b92c: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
}
