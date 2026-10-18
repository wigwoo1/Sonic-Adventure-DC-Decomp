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
/* Static C recovery of retail Tikal helper 8c09852e..8c0985ec.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_task_create(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c09852e;
L8c09852e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c098530;
L8c098530: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c098532;
L8c098532: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c098534;
L8c098534: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c098536;
L8c098536: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c098538;
L8c098538: wr(m,r[15],r[4],2); /* mov.w r4,@r15 */
 goto L8c09853a;
L8c09853a: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c09853c;
L8c09853c: r[5]=r[6]; /* mov r6,r5 */
 goto L8c09853e;
L8c09853e: wr(m,r[15]+0x8u,r[6],4); /* mov.l r6,@(0x8,r15) */
 goto L8c098540;
L8c098540: *pr=0x8c098544u;r[4]=rd(m,r[15]+0x4u,4);status=call(ctx,0x8c0985ecu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c0985ec */
 goto L8c098544;
L8c098544: r[14]=r[0]; /* mov r0,r14 */
 goto L8c098546;
L8c098546: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c098548;
L8c098548: if((*sr&1u))goto L8c0985d8; /* bt 0x8c0985d8 */
 goto L8c09854a;
L8c09854a: r[13]=sx(m,r[15],2); /* mov.w @r15,r13 */
 goto L8c09854c;
L8c09854c: r[3]=2u; /* mov #0x2,r3 */
 goto L8c09854e;
L8c09854e: r[13]=r[13]&65535u; /* extu.w r13,r13 */
 goto L8c098550;
L8c098550: *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c098552;
L8c098552: if((*sr&1u))goto L8c09856a; /* bt 0x8c09856a */
 goto L8c098554;
L8c098554: r[5]=64u; /* mov #0x40,r5 */
 goto L8c098556;
L8c098556: *pr=0x8c09855au;r[4]=1u;status=call(ctx,0x8c098b0cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b0c */
 goto L8c09855a;
L8c09855a: r[12]=r[0]; /* mov r0,r12 */
 goto L8c09855c;
L8c09855c: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c09855e;
L8c09855e: if(!(*sr&1u))goto L8c098568; /* bf 0x8c098568 */
 goto L8c098560;
L8c098560: *pr=0x8c098564u;r[4]=r[14];status=call(ctx,0x8c09865cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c09865c */
 goto L8c098564;
L8c098564: goto L8c0985d8; /* bra 0x8c0985d8 */
 goto L8c098568;
L8c098568: wr(m,r[14]+0x20u,r[12],4); /* mov.l r12,@(0x20,r14) */
 goto L8c09856a;
L8c09856a: r[3]=4u; /* mov #0x4,r3 */
 goto L8c09856c;
L8c09856c: *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c09856e;
L8c09856e: if((*sr&1u))goto L8c098586; /* bt 0x8c098586 */
 goto L8c098570;
L8c098570: r[5]=28u; /* mov #0x1c,r5 */
 goto L8c098572;
L8c098572: *pr=0x8c098576u;r[4]=2u;status=call(ctx,0x8c098b0cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b0c */
 goto L8c098576;
L8c098576: r[12]=r[0]; /* mov r0,r12 */
 goto L8c098578;
L8c098578: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c09857a;
L8c09857a: if(!(*sr&1u))goto L8c098584; /* bf 0x8c098584 */
 goto L8c09857c;
L8c09857c: *pr=0x8c098580u;r[4]=r[14];status=call(ctx,0x8c09865cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c09865c */
 goto L8c098580;
L8c098580: goto L8c0985d8; /* bra 0x8c0985d8 */
 goto L8c098584;
L8c098584: wr(m,r[14]+0x28u,r[12],4); /* mov.l r12,@(0x28,r14) */
 goto L8c098586;
L8c098586: r[3]=1u; /* mov #0x1,r3 */
 goto L8c098588;
L8c098588: *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c09858a;
L8c09858a: if((*sr&1u))goto L8c0985c2; /* bt 0x8c0985c2 */
 goto L8c09858c;
L8c09858c: r[5]=64u; /* mov #0x40,r5 */
 goto L8c09858e;
L8c09858e: *pr=0x8c098592u;r[4]=1u;status=call(ctx,0x8c098b0cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b0c */
 goto L8c098592;
L8c098592: r[12]=r[0]; /* mov r0,r12 */
 goto L8c098594;
L8c098594: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c098596;
L8c098596: if(!(*sr&1u))goto L8c0985c0; /* bf 0x8c0985c0 */
 goto L8c098598;
L8c098598: *pr=0x8c09859cu;r[4]=r[14];status=call(ctx,0x8c09865cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c09865c */
 goto L8c09859c;
L8c09859c: goto L8c0985d8; /* bra 0x8c0985d8 */
 goto L8c0985c0;
L8c0985c0: wr(m,r[14]+0x24u,r[12],4); /* mov.l r12,@(0x24,r14) */
 goto L8c0985c2;
L8c0985c2: r[3]=8u; /* mov #0x8,r3 */
 goto L8c0985c4;
L8c0985c4: *sr=(*sr&~1u)|!(r[3]&r[13]); /* tst r3,r13 */
 goto L8c0985c6;
L8c0985c6: if((*sr&1u))goto L8c0985de; /* bt 0x8c0985de */
 goto L8c0985c8;
L8c0985c8: r[5]=16u; /* mov #0x10,r5 */
 goto L8c0985ca;
L8c0985ca: *pr=0x8c0985ceu;r[4]=1u;status=call(ctx,0x8c098b0cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b0c */
 goto L8c0985ce;
L8c0985ce: r[13]=r[0]; /* mov r0,r13 */
 goto L8c0985d0;
L8c0985d0: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c0985d2;
L8c0985d2: if(!(*sr&1u))goto L8c0985dc; /* bf 0x8c0985dc */
 goto L8c0985d4;
L8c0985d4: *pr=0x8c0985d8u;r[4]=r[14];status=call(ctx,0x8c09865cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c09865c */
 goto L8c0985d8;
L8c0985d8: r[0]=0u;goto L8c0985e0; /* bra 0x8c0985e0 */
 goto L8c0985dc;
L8c0985dc: wr(m,r[14]+0x2cu,r[13],4); /* mov.l r13,@(0x2c,r14) */
 goto L8c0985de;
L8c0985de: r[0]=r[14]; /* mov r14,r0 */
 goto L8c0985e0;
L8c0985e0: r[15]+=12u; /* add #0xc,r15 */
 goto L8c0985e2;
L8c0985e2: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c0985e4;
L8c0985e4: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c0985e6;
L8c0985e6: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c0985e8;
L8c0985e8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
