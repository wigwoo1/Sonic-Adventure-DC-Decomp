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
/* Retail indexed MIDI controller-10 pan wrapper; input offset +64. */
SA_STAGE_EXPORT int sa_sound_pan_control(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c09328e;
L8c09328e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c093290;
L8c093290: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c093292;
L8c093292: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c093294;
L8c093294: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c093296;
L8c093296: r[14]=r[4]; /* mov r4,r14 */
 goto L8c093298;
L8c093298: r[4]=15u; /* mov #0xf,r4 */
 goto L8c09329a;
L8c09329a: *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[4]); /* cmp/gt r4,r14 */
 goto L8c09329c;
L8c09329c: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c09329e;
L8c09329e: take=!(*sr&1u);r[13]=0u;if(take)goto L8c0932a4;goto L8c0932a2; /* bf/s 0x8c0932a4 */
 goto L8c0932a2;
L8c0932a2: r[13]=1u; /* mov #0x1,r13 */
 goto L8c0932a4;
L8c0932a4: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c0932a6;
L8c0932a6: r[3]=10u; /* mov #0xa,r3 */
 goto L8c0932a8;
L8c0932a8: r[14]&=r[4]; /* and r4,r14 */
 goto L8c0932aa;
L8c0932aa: r[4]=r[13]; /* mov r13,r4 */
 goto L8c0932ac;
L8c0932ac: r[2]+=64u; /* add #0x40,r2 */
 goto L8c0932ae;
L8c0932ae: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c0932b0;
L8c0932b0: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c0932b2;
L8c0932b2: r[1]=4294967216u; /* mov #-0x50,r1 */
 goto L8c0932b4;
L8c0932b4: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c0932b6;
L8c0932b6: r[4]<<=2; /* shll2 r4 */
 goto L8c0932b8;
L8c0932b8: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c0932ba;
L8c0932ba: r[1]=r[1]&255u; /* extu.b r1,r1 */
 goto L8c0932bc;
L8c0932bc: r[3]=rd(m,0x8c093348u,4); /* mov.l 0x8c093348,r3 */
 goto L8c0932be;
L8c0932be: r[14]|=r[1]; /* or r1,r14 */
 goto L8c0932c0;
L8c0932c0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c0932c2;
L8c0932c2: r[4]+=r[3]; /* add r3,r4 */
 goto L8c0932c4;
L8c0932c4: r[3]=rd(m,0x8c093340u,4); /* mov.l 0x8c093340,r3 */
 goto L8c0932c6;
L8c0932c6: wr(m,r[15]+0x10u,r[4],4); /* mov.l r4,@(0x10,r15) */
 goto L8c0932c8;
L8c0932c8: target=r[3];*pr=0x8c0932ccu;r[4]=rd(m,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c0932cc;
L8c0932cc: r[4]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r4 */
 goto L8c0932ce;
L8c0932ce: r[6]=0u; /* mov #0x0,r6 */
 goto L8c0932d0;
L8c0932d0: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c0932d2;
L8c0932d2: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c0932d4;
L8c0932d4: r[15]+=20u; /* add #0x14,r15 */
 goto L8c0932d6;
L8c0932d6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c0932d8;
L8c0932d8: r[3]=rd(m,0x8c09334cu,4); /* mov.l 0x8c09334c,r3 */
 goto L8c0932da;
L8c0932da: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c0932dc;
L8c0932dc: target=r[3];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
}
