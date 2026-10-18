#include "sa_draw_motion_stop.h"
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
L8c09852e: sa_motion_publish_stop(ctx,0x8c09852eu); r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c098530;
L8c098530: sa_motion_publish_stop(ctx,0x8c098530u); r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c098532;
L8c098532: sa_motion_publish_stop(ctx,0x8c098532u); r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c098534;
L8c098534: sa_motion_publish_stop(ctx,0x8c098534u); r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c098536;
L8c098536: sa_motion_publish_stop(ctx,0x8c098536u); r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c098538;
L8c098538: sa_motion_publish_stop(ctx,0x8c098538u); wr(m,r[15],r[4],2); /* mov.w r4,@r15 */
 goto L8c09853a;
L8c09853a: sa_motion_publish_stop(ctx,0x8c09853au); wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c09853c;
L8c09853c: sa_motion_publish_stop(ctx,0x8c09853cu); r[5]=r[6]; /* mov r6,r5 */
 goto L8c09853e;
L8c09853e: sa_motion_publish_stop(ctx,0x8c09853eu); wr(m,r[15]+0x8u,r[6],4); /* mov.l r6,@(0x8,r15) */
 goto L8c098540;
L8c098540: sa_motion_publish_stop(ctx,0x8c098542u); *pr=0x8c098544u;r[4]=rd(m,r[15]+0x4u,4);status=call(ctx,0x8c0985ecu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c0985ec */
 goto L8c098544;
L8c098544: sa_motion_publish_stop(ctx,0x8c098544u); r[14]=r[0]; /* mov r0,r14 */
 goto L8c098546;
L8c098546: sa_motion_publish_stop(ctx,0x8c098546u); *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c098548;
L8c098548: sa_motion_publish_stop(ctx,0x8c098548u); if((*sr&1u))goto L8c0985d8; /* bt 0x8c0985d8 */
 goto L8c09854a;
L8c09854a: sa_motion_publish_stop(ctx,0x8c09854au); r[13]=sx(m,r[15],2); /* mov.w @r15,r13 */
 goto L8c09854c;
L8c09854c: sa_motion_publish_stop(ctx,0x8c09854cu); r[3]=2u; /* mov #0x2,r3 */
 goto L8c09854e;
L8c09854e: sa_motion_publish_stop(ctx,0x8c09854eu); r[13]=r[13]&65535u; /* extu.w r13,r13 */
 goto L8c098550;
L8c098550: sa_motion_publish_stop(ctx,0x8c098550u); *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c098552;
L8c098552: sa_motion_publish_stop(ctx,0x8c098552u); if((*sr&1u))goto L8c09856a; /* bt 0x8c09856a */
 goto L8c098554;
L8c098554: sa_motion_publish_stop(ctx,0x8c098554u); r[5]=64u; /* mov #0x40,r5 */
 goto L8c098556;
L8c098556: sa_motion_publish_stop(ctx,0x8c098558u); *pr=0x8c09855au;r[4]=1u;status=call(ctx,0x8c098b0cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b0c */
 goto L8c09855a;
L8c09855a: sa_motion_publish_stop(ctx,0x8c09855au); r[12]=r[0]; /* mov r0,r12 */
 goto L8c09855c;
L8c09855c: sa_motion_publish_stop(ctx,0x8c09855cu); *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c09855e;
L8c09855e: sa_motion_publish_stop(ctx,0x8c09855eu); if(!(*sr&1u))goto L8c098568; /* bf 0x8c098568 */
 goto L8c098560;
L8c098560: sa_motion_publish_stop(ctx,0x8c098562u); *pr=0x8c098564u;r[4]=r[14];status=call(ctx,0x8c09865cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c09865c */
 goto L8c098564;
L8c098564: sa_motion_publish_stop(ctx,0x8c098566u); goto L8c0985d8; /* bra 0x8c0985d8 */
 goto L8c098568;
L8c098568: sa_motion_publish_stop(ctx,0x8c098568u); wr(m,r[14]+0x20u,r[12],4); /* mov.l r12,@(0x20,r14) */
 goto L8c09856a;
L8c09856a: sa_motion_publish_stop(ctx,0x8c09856au); r[3]=4u; /* mov #0x4,r3 */
 goto L8c09856c;
L8c09856c: sa_motion_publish_stop(ctx,0x8c09856cu); *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c09856e;
L8c09856e: sa_motion_publish_stop(ctx,0x8c09856eu); if((*sr&1u))goto L8c098586; /* bt 0x8c098586 */
 goto L8c098570;
L8c098570: sa_motion_publish_stop(ctx,0x8c098570u); r[5]=28u; /* mov #0x1c,r5 */
 goto L8c098572;
L8c098572: sa_motion_publish_stop(ctx,0x8c098574u); *pr=0x8c098576u;r[4]=2u;status=call(ctx,0x8c098b0cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b0c */
 goto L8c098576;
L8c098576: sa_motion_publish_stop(ctx,0x8c098576u); r[12]=r[0]; /* mov r0,r12 */
 goto L8c098578;
L8c098578: sa_motion_publish_stop(ctx,0x8c098578u); *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c09857a;
L8c09857a: sa_motion_publish_stop(ctx,0x8c09857au); if(!(*sr&1u))goto L8c098584; /* bf 0x8c098584 */
 goto L8c09857c;
L8c09857c: sa_motion_publish_stop(ctx,0x8c09857eu); *pr=0x8c098580u;r[4]=r[14];status=call(ctx,0x8c09865cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c09865c */
 goto L8c098580;
L8c098580: sa_motion_publish_stop(ctx,0x8c098582u); goto L8c0985d8; /* bra 0x8c0985d8 */
 goto L8c098584;
L8c098584: sa_motion_publish_stop(ctx,0x8c098584u); wr(m,r[14]+0x28u,r[12],4); /* mov.l r12,@(0x28,r14) */
 goto L8c098586;
L8c098586: sa_motion_publish_stop(ctx,0x8c098586u); r[3]=1u; /* mov #0x1,r3 */
 goto L8c098588;
L8c098588: sa_motion_publish_stop(ctx,0x8c098588u); *sr=(*sr&~1u)|!(r[13]&r[3]); /* tst r13,r3 */
 goto L8c09858a;
L8c09858a: sa_motion_publish_stop(ctx,0x8c09858au); if((*sr&1u))goto L8c0985c2; /* bt 0x8c0985c2 */
 goto L8c09858c;
L8c09858c: sa_motion_publish_stop(ctx,0x8c09858cu); r[5]=64u; /* mov #0x40,r5 */
 goto L8c09858e;
L8c09858e: sa_motion_publish_stop(ctx,0x8c098590u); *pr=0x8c098592u;r[4]=1u;status=call(ctx,0x8c098b0cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b0c */
 goto L8c098592;
L8c098592: sa_motion_publish_stop(ctx,0x8c098592u); r[12]=r[0]; /* mov r0,r12 */
 goto L8c098594;
L8c098594: sa_motion_publish_stop(ctx,0x8c098594u); *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c098596;
L8c098596: sa_motion_publish_stop(ctx,0x8c098596u); if(!(*sr&1u))goto L8c0985c0; /* bf 0x8c0985c0 */
 goto L8c098598;
L8c098598: sa_motion_publish_stop(ctx,0x8c09859au); *pr=0x8c09859cu;r[4]=r[14];status=call(ctx,0x8c09865cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c09865c */
 goto L8c09859c;
L8c09859c: sa_motion_publish_stop(ctx,0x8c09859eu); goto L8c0985d8; /* bra 0x8c0985d8 */
 goto L8c0985c0;
L8c0985c0: sa_motion_publish_stop(ctx,0x8c0985c0u); wr(m,r[14]+0x24u,r[12],4); /* mov.l r12,@(0x24,r14) */
 goto L8c0985c2;
L8c0985c2: sa_motion_publish_stop(ctx,0x8c0985c2u); r[3]=8u; /* mov #0x8,r3 */
 goto L8c0985c4;
L8c0985c4: sa_motion_publish_stop(ctx,0x8c0985c4u); *sr=(*sr&~1u)|!(r[3]&r[13]); /* tst r3,r13 */
 goto L8c0985c6;
L8c0985c6: sa_motion_publish_stop(ctx,0x8c0985c6u); if((*sr&1u))goto L8c0985de; /* bt 0x8c0985de */
 goto L8c0985c8;
L8c0985c8: sa_motion_publish_stop(ctx,0x8c0985c8u); r[5]=16u; /* mov #0x10,r5 */
 goto L8c0985ca;
L8c0985ca: sa_motion_publish_stop(ctx,0x8c0985ccu); *pr=0x8c0985ceu;r[4]=1u;status=call(ctx,0x8c098b0cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b0c */
 goto L8c0985ce;
L8c0985ce: sa_motion_publish_stop(ctx,0x8c0985ceu); r[13]=r[0]; /* mov r0,r13 */
 goto L8c0985d0;
L8c0985d0: sa_motion_publish_stop(ctx,0x8c0985d0u); *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c0985d2;
L8c0985d2: sa_motion_publish_stop(ctx,0x8c0985d2u); if(!(*sr&1u))goto L8c0985dc; /* bf 0x8c0985dc */
 goto L8c0985d4;
L8c0985d4: sa_motion_publish_stop(ctx,0x8c0985d6u); *pr=0x8c0985d8u;r[4]=r[14];status=call(ctx,0x8c09865cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c09865c */
 goto L8c0985d8;
L8c0985d8: sa_motion_publish_stop(ctx,0x8c0985dau); r[0]=0u;goto L8c0985e0; /* bra 0x8c0985e0 */
 goto L8c0985dc;
L8c0985dc: sa_motion_publish_stop(ctx,0x8c0985dcu); wr(m,r[14]+0x2cu,r[13],4); /* mov.l r13,@(0x2c,r14) */
 goto L8c0985de;
L8c0985de: sa_motion_publish_stop(ctx,0x8c0985deu); r[0]=r[14]; /* mov r14,r0 */
 goto L8c0985e0;
L8c0985e0: sa_motion_publish_stop(ctx,0x8c0985e0u); r[15]+=12u; /* add #0xc,r15 */
 goto L8c0985e2;
L8c0985e2: sa_motion_publish_stop(ctx,0x8c0985e2u); *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c0985e4;
L8c0985e4: sa_motion_publish_stop(ctx,0x8c0985e4u); r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c0985e6;
L8c0985e6: sa_motion_publish_stop(ctx,0x8c0985e6u); r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c0985e8;
L8c0985e8: sa_motion_publish_stop(ctx,0x8c0985eau); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
