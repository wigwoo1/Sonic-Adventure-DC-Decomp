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
/* Retail indexed MIDI pitch bend, signed input biased by 8192. */
SA_STAGE_EXPORT int sa_sound_pitch_bend(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c0932e0;
L8c0932e0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c0932e2;
L8c0932e2: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c0932e4;
L8c0932e4: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c0932e6;
L8c0932e6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c0932e8;
L8c0932e8: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c0932ea;
L8c0932ea: r[14]=r[4]; /* mov r4,r14 */
 goto L8c0932ec;
L8c0932ec: r[4]=15u; /* mov #0xf,r4 */
 goto L8c0932ee;
L8c0932ee: *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[4]); /* cmp/gt r4,r14 */
 goto L8c0932f0;
L8c0932f0: r[13]=r[5]; /* mov r5,r13 */
 goto L8c0932f2;
L8c0932f2: take=!(*sr&1u);r[12]=0u;if(take)goto L8c0932f8;goto L8c0932f6; /* bf/s 0x8c0932f8 */
 goto L8c0932f6;
L8c0932f6: r[12]=1u; /* mov #0x1,r12 */
 goto L8c0932f8;
L8c0932f8: r[2]=32u; /* mov #0x20,r2 */
 goto L8c0932fa;
L8c0932fa: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c0932fc;
L8c0932fc: r[2]<<=8; /* shll8 r2 */
 goto L8c0932fe;
L8c0932fe: r[13]+=r[2]; /* add r2,r13 */
 goto L8c093300;
L8c093300: r[2]=rd(m,0x8c093340u,4); /* mov.l 0x8c093340,r2 */
 goto L8c093302;
L8c093302: r[1]=4294967289u; /* mov #-0x7,r1 */
 goto L8c093304;
L8c093304: r[3]=r[13]; /* mov r13,r3 */
 goto L8c093306;
L8c093306: {uint32_t shift=r[1];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r1,r3 */
 goto L8c093308;
L8c093308: r[1]=rd(m,0x8c093348u,4); /* mov.l 0x8c093348,r1 */
 goto L8c09330a;
L8c09330a: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c09330c;
L8c09330c: r[3]=4294967264u; /* mov #-0x20,r3 */
 goto L8c09330e;
L8c09330e: r[0]=127u; /* mov #0x7f,r0 */
 goto L8c093310;
L8c093310: r[14]&=r[4]; /* and r4,r14 */
 goto L8c093312;
L8c093312: r[4]=r[12]; /* mov r12,r4 */
 goto L8c093314;
L8c093314: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c093316;
L8c093316: r[0]&=r[13]; /* and r13,r0 */
 goto L8c093318;
L8c093318: r[4]<<=2; /* shll2 r4 */
 goto L8c09331a;
L8c09331a: r[15]-=4;wr(m,r[15],r[0],4); /* mov.l r0,@-r15 */
 goto L8c09331c;
L8c09331c: r[14]|=r[3]; /* or r3,r14 */
 goto L8c09331e;
L8c09331e: r[4]+=r[1]; /* add r1,r4 */
 goto L8c093320;
L8c093320: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c093322;
L8c093322: wr(m,r[15]+0xcu,r[4],4); /* mov.l r4,@(0xc,r15) */
 goto L8c093324;
L8c093324: target=r[2];*pr=0x8c093328u;r[4]=rd(m,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c093328;
L8c093328: r[4]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r4 */
 goto L8c09332a;
L8c09332a: r[6]=0u; /* mov #0x0,r6 */
 goto L8c09332c;
L8c09332c: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c09332e;
L8c09332e: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c093330;
L8c093330: r[15]+=16u; /* add #0x10,r15 */
 goto L8c093332;
L8c093332: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c093334;
L8c093334: r[3]=rd(m,0x8c09334cu,4); /* mov.l 0x8c09334c,r3 */
 goto L8c093336;
L8c093336: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c093338;
L8c093338: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c09333a;
L8c09333a: target=r[3];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
}
