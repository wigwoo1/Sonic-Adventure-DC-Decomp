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
/* Retail stream format initializer667596..667650.
 * Magic8000 branch,then SPSD,WAVE,AIFF,SND detection and tail-call initialization.
 * Unknown header returns0. Detectors/initializers resolve through caller dispatch. */
SA_STAGE_EXPORT int sa_sound_stream_format_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c667596;
L8c667596: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c667598;
L8c667598: r[14]=r[5]; /* mov r5,r14 */
 goto L8c66759a;
L8c66759a: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66759c;
L8c66759c: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c66759e;
L8c66759e: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6675a0;
L8c6675a0: r[12]=r[4]; /* mov r4,r12 */
 goto L8c6675a2;
L8c6675a2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6675a4;
L8c6675a4: r[4]=sx(m,r[14],2); /* mov.w @r14,r4 */
 goto L8c6675a6;
L8c6675a6: r[4]=r[4]&65535u; /* extu.w r4,r4 */
 goto L8c6675a8;
L8c6675a8: r[3]=r[4]; /* mov r4,r3 */
 goto L8c6675aa;
L8c6675aa: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r3 */
 goto L8c6675ac;
L8c6675ac: r[1]=r[4]; /* mov r4,r1 */
 goto L8c6675ae;
L8c6675ae: r[2]=rd(m,0x8c667664u,4); /* mov.l 0x8c667664,r2 */
 goto L8c6675b0;
L8c6675b0: r[1]<<=8; /* shll8 r1 */
 goto L8c6675b2;
L8c6675b2: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c6675b4;
L8c6675b4: r[4]=r[3]; /* mov r3,r4 */
 goto L8c6675b6;
L8c6675b6: r[1]&=r[2]; /* and r2,r1 */
 goto L8c6675b8;
L8c6675b8: r[3]=rd(m,0x8c667668u,4); /* mov.l 0x8c667668,r3 */
 goto L8c6675ba;
L8c6675ba: r[4]|=r[1]; /* or r1,r4 */
 goto L8c6675bc;
L8c6675bc: r[4]=r[4]&65535u; /* extu.w r4,r4 */
 goto L8c6675be;
L8c6675be: *sr=(*sr&~1u)|(r[3]==r[4]); /* cmp/eq r3,r4 */
 goto L8c6675c0;
L8c6675c0: take=!(*sr&1u);r[13]=r[6];if(take)goto L8c6675d4;goto L8c6675c4; /* bf/s 0x8c6675d4 */
 goto L8c6675c4;
L8c6675c4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6675c6;
L8c6675c6: r[4]=r[12]; /* mov r12,r4 */
 goto L8c6675c8;
L8c6675c8: r[6]=r[13]; /* mov r13,r6 */
 goto L8c6675ca;
L8c6675ca: r[5]=r[14]; /* mov r14,r5 */
 goto L8c6675cc;
L8c6675cc: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6675ce;
L8c6675ce: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6675d0;
L8c6675d0: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c667488u,m,f,sr,r,pr); /* bra 0x8c667488 */
 goto L8c6675d4;
L8c6675d4: r[1]=rd(m,0x8c66766cu,4); /* mov.l 0x8c66766c,r1 */
 goto L8c6675d6;
L8c6675d6: target=r[1];*pr=0x8c6675dau;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c6675da;
L8c6675da: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c6675dc;
L8c6675dc: if((*sr&1u))goto L8c6675f0; /* bt 0x8c6675f0 */
 goto L8c6675de;
L8c6675de: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6675e0;
L8c6675e0: r[4]=r[12]; /* mov r12,r4 */
 goto L8c6675e2;
L8c6675e2: r[6]=r[13]; /* mov r13,r6 */
 goto L8c6675e4;
L8c6675e4: r[2]=rd(m,0x8c667670u,4); /* mov.l 0x8c667670,r2 */
 goto L8c6675e6;
L8c6675e6: r[5]=r[14]; /* mov r14,r5 */
 goto L8c6675e8;
L8c6675e8: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6675ea;
L8c6675ea: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6675ec;
L8c6675ec: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c6675f0: r[2]=rd(m,0x8c667674u,4); /* mov.l 0x8c667674,r2 */
 goto L8c6675f2;
L8c6675f2: target=r[2];*pr=0x8c6675f6u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6675f6;
L8c6675f6: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c6675f8;
L8c6675f8: if((*sr&1u))goto L8c66760c; /* bt 0x8c66760c */
 goto L8c6675fa;
L8c6675fa: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6675fc;
L8c6675fc: r[4]=r[12]; /* mov r12,r4 */
 goto L8c6675fe;
L8c6675fe: r[6]=r[13]; /* mov r13,r6 */
 goto L8c667600;
L8c667600: r[2]=rd(m,0x8c667678u,4); /* mov.l 0x8c667678,r2 */
 goto L8c667602;
L8c667602: r[5]=r[14]; /* mov r14,r5 */
 goto L8c667604;
L8c667604: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c667606;
L8c667606: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c667608;
L8c667608: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c66760c: r[2]=rd(m,0x8c66767cu,4); /* mov.l 0x8c66767c,r2 */
 goto L8c66760e;
L8c66760e: target=r[2];*pr=0x8c667612u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c667612;
L8c667612: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c667614;
L8c667614: if((*sr&1u))goto L8c667628; /* bt 0x8c667628 */
 goto L8c667616;
L8c667616: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c667618;
L8c667618: r[4]=r[12]; /* mov r12,r4 */
 goto L8c66761a;
L8c66761a: r[6]=r[13]; /* mov r13,r6 */
 goto L8c66761c;
L8c66761c: r[2]=rd(m,0x8c667680u,4); /* mov.l 0x8c667680,r2 */
 goto L8c66761e;
L8c66761e: r[5]=r[14]; /* mov r14,r5 */
 goto L8c667620;
L8c667620: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c667622;
L8c667622: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c667624;
L8c667624: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c667628: r[2]=rd(m,0x8c667684u,4); /* mov.l 0x8c667684,r2 */
 goto L8c66762a;
L8c66762a: target=r[2];*pr=0x8c66762eu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66762e;
L8c66762e: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c667630;
L8c667630: if((*sr&1u))goto L8c667644; /* bt 0x8c667644 */
 goto L8c667632;
L8c667632: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c667634;
L8c667634: r[4]=r[12]; /* mov r12,r4 */
 goto L8c667636;
L8c667636: r[6]=r[13]; /* mov r13,r6 */
 goto L8c667638;
L8c667638: r[2]=rd(m,0x8c667688u,4); /* mov.l 0x8c667688,r2 */
 goto L8c66763a;
L8c66763a: r[5]=r[14]; /* mov r14,r5 */
 goto L8c66763c;
L8c66763c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66763e;
L8c66763e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c667640;
L8c667640: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c667644: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c667646;
L8c667646: r[0]=0u; /* mov #0x0,r0 */
 goto L8c667648;
L8c667648: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66764a;
L8c66764a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66764c;
L8c66764c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
