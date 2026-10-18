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
/* Static C recovery of retail Tikal helper 8c6477cc..8c6478f6.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_channels_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6477cc;
L8c6477cc: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6477ce;
L8c6477ce: r[14]=0u; /* mov #0x0,r14 */
 goto L8c6477d0;
L8c6477d0: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6477d2;
L8c6477d2: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6477d4;
L8c6477d4: r[12]=r[4]; /* mov r4,r12 */
 goto L8c6477d6;
L8c6477d6: r[4]=rd(m,0x8c647848u,4); /* mov.l 0x8c647848,r4 */
 goto L8c6477d8;
L8c6477d8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6477da;
L8c6477da: wr(m,r[4],r[14],4); /* mov.l r14,@r4 */
 goto L8c6477dc;
L8c6477dc: wr(m,r[4]+0x4u,r[14],4); /* mov.l r14,@(0x4,r4) */
 goto L8c6477de;
L8c6477de: wr(m,r[4]+0x8u,r[14],4); /* mov.l r14,@(0x8,r4) */
 goto L8c6477e0;
L8c6477e0: wr(m,r[4]+0xcu,r[14],4); /* mov.l r14,@(0xc,r4) */
 goto L8c6477e2;
L8c6477e2: wr(m,r[4]+0x10u,r[14],4); /* mov.l r14,@(0x10,r4) */
 goto L8c6477e4;
L8c6477e4: wr(m,r[4]+0x14u,r[14],4); /* mov.l r14,@(0x14,r4) */
 goto L8c6477e6;
L8c6477e6: wr(m,r[4]+0x18u,r[14],4); /* mov.l r14,@(0x18,r4) */
 goto L8c6477e8;
L8c6477e8: wr(m,r[4]+0x1cu,r[14],4); /* mov.l r14,@(0x1c,r4) */
 goto L8c6477ea;
L8c6477ea: r[4]=rd(m,0x8c647850u,4); /* mov.l 0x8c647850,r4 */
 goto L8c6477ec;
L8c6477ec: r[6]=sx(m,0x8c647844u,2); /* mov.w 0x8c647844,r6 */
 goto L8c6477ee;
L8c6477ee: r[13]=rd(m,0x8c64784cu,4); /* mov.l 0x8c64784c,r13 */
 goto L8c6477f0;
L8c6477f0: target=r[13];*pr=0x8c6477f4u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c6477f4;
L8c6477f4: r[14]=sx(m,0x8c647846u,2); /* mov.w 0x8c647846,r14 */
 goto L8c6477f6;
L8c6477f6: r[4]=rd(m,0x8c647854u,4); /* mov.l 0x8c647854,r4 */
 goto L8c6477f8;
L8c6477f8: r[6]=r[14]; /* mov r14,r6 */
 goto L8c6477fa;
L8c6477fa: target=r[13];*pr=0x8c6477feu;r[5]=48u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c6477fe;
L8c6477fe: r[4]=rd(m,0x8c647858u,4); /* mov.l 0x8c647858,r4 */
 goto L8c647800;
L8c647800: r[6]=r[14]; /* mov r14,r6 */
 goto L8c647802;
L8c647802: target=r[13];*pr=0x8c647806u;r[5]=49u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c647806;
L8c647806: r[4]=rd(m,0x8c64785cu,4); /* mov.l 0x8c64785c,r4 */
 goto L8c647808;
L8c647808: r[6]=r[14]; /* mov r14,r6 */
 goto L8c64780a;
L8c64780a: target=r[13];*pr=0x8c64780eu;r[5]=50u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c64780e;
L8c64780e: r[4]=rd(m,0x8c647860u,4); /* mov.l 0x8c647860,r4 */
 goto L8c647810;
L8c647810: r[6]=r[14]; /* mov r14,r6 */
 goto L8c647812;
L8c647812: target=r[13];*pr=0x8c647816u;r[5]=51u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c647816;
L8c647816: r[4]=rd(m,0x8c647864u,4); /* mov.l 0x8c647864,r4 */
 goto L8c647818;
L8c647818: r[6]=r[14]; /* mov r14,r6 */
 goto L8c64781a;
L8c64781a: target=r[13];*pr=0x8c64781eu;r[5]=52u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c64781e;
L8c64781e: r[4]=rd(m,0x8c647868u,4); /* mov.l 0x8c647868,r4 */
 goto L8c647820;
L8c647820: r[6]=r[14]; /* mov r14,r6 */
 goto L8c647822;
L8c647822: target=r[13];*pr=0x8c647826u;r[5]=53u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c647826;
L8c647826: r[4]=rd(m,0x8c64786cu,4); /* mov.l 0x8c64786c,r4 */
 goto L8c647828;
L8c647828: r[6]=r[14]; /* mov r14,r6 */
 goto L8c64782a;
L8c64782a: target=r[13];*pr=0x8c64782eu;r[5]=54u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c64782e;
L8c64782e: r[4]=rd(m,0x8c647870u,4); /* mov.l 0x8c647870,r4 */
 goto L8c647830;
L8c647830: r[6]=r[14]; /* mov r14,r6 */
 goto L8c647832;
L8c647832: target=r[13];*pr=0x8c647836u;r[5]=55u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c647836;
L8c647836: r[3]=rd(m,0x8c647874u,4); /* mov.l 0x8c647874,r3 */
 goto L8c647838;
L8c647838: target=r[3];*pr=0x8c64783cu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c64783c;
L8c64783c: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c64783e;
L8c64783e: if(!(*sr&1u))goto L8c647878; /* bf 0x8c647878 */
 goto L8c647840;
L8c647840: r[0]=4294967295u;goto L8c6478ec; /* bra 0x8c6478ec */
 goto L8c647878;
L8c647878: r[3]=1u; /* mov #0x1,r3 */
 goto L8c64787a;
L8c64787a: r[13]=rd(m,0x8c647930u,4); /* mov.l 0x8c647930,r13 */
 goto L8c64787c;
L8c64787c: *sr=(*sr&~1u)|!(r[12]&r[3]); /* tst r12,r3 */
 goto L8c64787e;
L8c64787e: if((*sr&1u))goto L8c647888; /* bt 0x8c647888 */
 goto L8c647880;
L8c647880: r[4]=rd(m,0x8c647938u,4); /* mov.l 0x8c647938,r4 */
 goto L8c647882;
L8c647882: r[5]=rd(m,0x8c647934u,4); /* mov.l 0x8c647934,r5 */
 goto L8c647884;
L8c647884: target=r[13];*pr=0x8c647888u;r[6]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c647888;
L8c647888: r[3]=2u; /* mov #0x2,r3 */
 goto L8c64788a;
L8c64788a: *sr=(*sr&~1u)|!(r[12]&r[3]); /* tst r12,r3 */
 goto L8c64788c;
L8c64788c: if((*sr&1u))goto L8c647896; /* bt 0x8c647896 */
 goto L8c64788e;
L8c64788e: r[4]=rd(m,0x8c647940u,4); /* mov.l 0x8c647940,r4 */
 goto L8c647890;
L8c647890: r[5]=rd(m,0x8c64793cu,4); /* mov.l 0x8c64793c,r5 */
 goto L8c647892;
L8c647892: target=r[13];*pr=0x8c647896u;r[6]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c647896;
L8c647896: r[3]=4u; /* mov #0x4,r3 */
 goto L8c647898;
L8c647898: *sr=(*sr&~1u)|!(r[12]&r[3]); /* tst r12,r3 */
 goto L8c64789a;
L8c64789a: if((*sr&1u))goto L8c6478a4; /* bt 0x8c6478a4 */
 goto L8c64789c;
L8c64789c: r[4]=rd(m,0x8c647948u,4); /* mov.l 0x8c647948,r4 */
 goto L8c64789e;
L8c64789e: r[5]=rd(m,0x8c647944u,4); /* mov.l 0x8c647944,r5 */
 goto L8c6478a0;
L8c6478a0: target=r[13];*pr=0x8c6478a4u;r[6]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c6478a4;
L8c6478a4: r[3]=8u; /* mov #0x8,r3 */
 goto L8c6478a6;
L8c6478a6: *sr=(*sr&~1u)|!(r[12]&r[3]); /* tst r12,r3 */
 goto L8c6478a8;
L8c6478a8: if((*sr&1u))goto L8c6478b2; /* bt 0x8c6478b2 */
 goto L8c6478aa;
L8c6478aa: r[4]=rd(m,0x8c647950u,4); /* mov.l 0x8c647950,r4 */
 goto L8c6478ac;
L8c6478ac: r[5]=rd(m,0x8c64794cu,4); /* mov.l 0x8c64794c,r5 */
 goto L8c6478ae;
L8c6478ae: target=r[13];*pr=0x8c6478b2u;r[6]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c6478b2;
L8c6478b2: r[3]=16u; /* mov #0x10,r3 */
 goto L8c6478b4;
L8c6478b4: *sr=(*sr&~1u)|!(r[12]&r[3]); /* tst r12,r3 */
 goto L8c6478b6;
L8c6478b6: if((*sr&1u))goto L8c6478c0; /* bt 0x8c6478c0 */
 goto L8c6478b8;
L8c6478b8: r[4]=rd(m,0x8c647958u,4); /* mov.l 0x8c647958,r4 */
 goto L8c6478ba;
L8c6478ba: r[5]=rd(m,0x8c647954u,4); /* mov.l 0x8c647954,r5 */
 goto L8c6478bc;
L8c6478bc: target=r[13];*pr=0x8c6478c0u;r[6]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c6478c0;
L8c6478c0: r[3]=32u; /* mov #0x20,r3 */
 goto L8c6478c2;
L8c6478c2: *sr=(*sr&~1u)|!(r[12]&r[3]); /* tst r12,r3 */
 goto L8c6478c4;
L8c6478c4: if((*sr&1u))goto L8c6478ce; /* bt 0x8c6478ce */
 goto L8c6478c6;
L8c6478c6: r[4]=rd(m,0x8c647960u,4); /* mov.l 0x8c647960,r4 */
 goto L8c6478c8;
L8c6478c8: r[5]=rd(m,0x8c64795cu,4); /* mov.l 0x8c64795c,r5 */
 goto L8c6478ca;
L8c6478ca: target=r[13];*pr=0x8c6478ceu;r[6]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c6478ce;
L8c6478ce: r[3]=64u; /* mov #0x40,r3 */
 goto L8c6478d0;
L8c6478d0: *sr=(*sr&~1u)|!(r[12]&r[3]); /* tst r12,r3 */
 goto L8c6478d2;
L8c6478d2: if((*sr&1u))goto L8c6478dc; /* bt 0x8c6478dc */
 goto L8c6478d4;
L8c6478d4: r[4]=rd(m,0x8c647968u,4); /* mov.l 0x8c647968,r4 */
 goto L8c6478d6;
L8c6478d6: r[5]=rd(m,0x8c647964u,4); /* mov.l 0x8c647964,r5 */
 goto L8c6478d8;
L8c6478d8: target=r[13];*pr=0x8c6478dcu;r[6]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c6478dc;
L8c6478dc: r[3]=sx(m,0x8c64792au,2); /* mov.w 0x8c64792a,r3 */
 goto L8c6478de;
L8c6478de: *sr=(*sr&~1u)|!(r[3]&r[12]); /* tst r3,r12 */
 goto L8c6478e0;
L8c6478e0: if((*sr&1u))goto L8c6478ea; /* bt 0x8c6478ea */
 goto L8c6478e2;
L8c6478e2: r[4]=rd(m,0x8c647970u,4); /* mov.l 0x8c647970,r4 */
 goto L8c6478e4;
L8c6478e4: r[5]=rd(m,0x8c64796cu,4); /* mov.l 0x8c64796c,r5 */
 goto L8c6478e6;
L8c6478e6: target=r[13];*pr=0x8c6478eau;r[6]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c6478ea;
L8c6478ea: r[0]=0u; /* mov #0x0,r0 */
 goto L8c6478ec;
L8c6478ec: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6478ee;
L8c6478ee: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6478f0;
L8c6478f0: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6478f2;
L8c6478f2: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
