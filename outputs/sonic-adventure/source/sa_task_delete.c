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
/* Retail task deletion 8c09865c..8c098784. List unlinking, resource cleanup calls, and free-list insertion. */
SA_STAGE_EXPORT int sa_task_delete(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c09865c;
L8c09865c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c09865e;
L8c09865e: r[14]=r[4]; /* mov r4,r14 */
 goto L8c098660;
L8c098660: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c098662;
L8c098662: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c098664;
L8c098664: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c098666;
L8c098666: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c098668;
L8c098668: r[3]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r3 */
 goto L8c09866a;
L8c09866a: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c09866c;
L8c09866c: if((*sr&1u))goto L8c098674; /* bt 0x8c098674 */
 goto L8c09866e;
L8c09866e: r[3]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r3 */
 goto L8c098670;
L8c098670: target=r[3];*pr=0x8c098674u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c098674;
L8c098674: r[13]=0u; /* mov #0x0,r13 */
 goto L8c098676;
L8c098676: wr(m,r[14]+0x10u,r[13],4); /* mov.l r13,@(0x10,r14) */
 goto L8c098678;
L8c098678: wr(m,r[14]+0x14u,r[13],4); /* mov.l r13,@(0x14,r14) */
 goto L8c09867a;
L8c09867a: wr(m,r[14]+0x18u,r[13],4); /* mov.l r13,@(0x18,r14) */
 goto L8c09867c;
L8c09867c: r[2]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r2 */
 goto L8c09867e;
L8c09867e: r[12]=rd(m,r[14],4); /* mov.l @r14,r12 */
 goto L8c098680;
L8c098680: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098682;
L8c098682: take=(*sr&1u);r[11]=rd(m,r[14]+0x4u,4);if(take)goto L8c09868a;goto L8c098686; /* bt/s 0x8c09868a */
 goto L8c098686;
L8c098686: *pr=0x8c09868au;r[4]=r[14];status=call(ctx,0x8c098e60u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098e60 */
 goto L8c09868a;
L8c09868a: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c09868c;
L8c09868c: if((*sr&1u))goto L8c098698; /* bt 0x8c098698 */
 goto L8c09868e;
L8c09868e: *sr=(*sr&~1u)|!(r[11]&r[11]); /* tst r11,r11 */
 goto L8c098690;
L8c098690: if((*sr&1u))goto L8c098698; /* bt 0x8c098698 */
 goto L8c098692;
L8c098692: wr(m,r[12]+0x4u,r[11],4); /* mov.l r11,@(0x4,r12) */
 goto L8c098694;
L8c098694: wr(m,r[11],r[12],4);goto L8c098718; /* bra 0x8c098718 */
 goto L8c098698;
L8c098698: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c09869a;
L8c09869a: if(!(*sr&1u))goto L8c0986e0; /* bf 0x8c0986e0 */
 goto L8c09869c;
L8c09869c: *sr=(*sr&~1u)|!(r[11]&r[11]); /* tst r11,r11 */
 goto L8c09869e;
L8c09869e: if((*sr&1u))goto L8c0986e0; /* bt 0x8c0986e0 */
 goto L8c0986a0;
L8c0986a0: wr(m,r[11],r[13],4);goto L8c098718; /* bra 0x8c098718 */
 goto L8c0986e0;
L8c0986e0: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c0986e2;
L8c0986e2: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c0986e4;
L8c0986e4: if((*sr&1u))goto L8c0986f2; /* bt 0x8c0986f2 */
 goto L8c0986e6;
L8c0986e6: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c0986e8;
L8c0986e8: if((*sr&1u))goto L8c0986ec; /* bt 0x8c0986ec */
 goto L8c0986ea;
L8c0986ea: wr(m,r[12]+0x4u,r[13],4); /* mov.l r13,@(0x4,r12) */
 goto L8c0986ec;
L8c0986ec: r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c0986ee;
L8c0986ee: wr(m,r[2]+0xcu,r[12],4);goto L8c098718; /* bra 0x8c098718 */
 goto L8c0986f2;
L8c0986f2: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c0986f4;
L8c0986f4: if((*sr&1u))goto L8c0986f8; /* bt 0x8c0986f8 */
 goto L8c0986f6;
L8c0986f6: wr(m,r[12]+0x4u,r[13],4); /* mov.l r13,@(0x4,r12) */
 goto L8c0986f8;
L8c0986f8: r[6]=rd(m,0x8c0987e0u,4); /* mov.l 0x8c0987e0,r6 */
 goto L8c0986fa;
L8c0986fa: r[7]=r[6]; /* mov r6,r7 */
 goto L8c0986fc;
L8c0986fc: r[7]+=32u; /* add #0x20,r7 */
 goto L8c0986fe;
L8c0986fe: r[4]=r[6]; /* mov r6,r4 */
 goto L8c098700;
L8c098700: *sr=(*sr&~1u)|(r[4]>=r[7]); /* cmp/hs r7,r4 */
 goto L8c098702;
L8c098702: take=(*sr&1u);r[5]=r[6];if(take)goto L8c098718;goto L8c098706; /* bt/s 0x8c098718 */
 goto L8c098706;
L8c098706: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c098708;
L8c098708: *sr=(*sr&~1u)|(r[2]==r[14]); /* cmp/eq r2,r14 */
 goto L8c09870a;
L8c09870a: if(!(*sr&1u))goto L8c098710; /* bf 0x8c098710 */
 goto L8c09870c;
L8c09870c: wr(m,r[5],r[12],4);goto L8c098718; /* bra 0x8c098718 */
 goto L8c098710;
L8c098710: r[4]+=4u; /* add #0x4,r4 */
 goto L8c098712;
L8c098712: *sr=(*sr&~1u)|(r[4]>=r[7]); /* cmp/hs r7,r4 */
 goto L8c098714;
L8c098714: take=!(*sr&1u);r[5]+=4u;if(take)goto L8c098706;goto L8c098718; /* bf/s 0x8c098706 */
 goto L8c098718;
L8c098718: r[2]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r2 */
 goto L8c09871a;
L8c09871a: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c09871c;
L8c09871c: if((*sr&1u))goto L8c098722; /* bt 0x8c098722 */
 goto L8c09871e;
L8c09871e: *pr=0x8c098722u;r[4]=r[14];status=call(ctx,0x8c098998u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098998 */
 goto L8c098722;
L8c098722: r[2]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r2 */
 goto L8c098724;
L8c098724: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098726;
L8c098726: if((*sr&1u))goto L8c098730; /* bt 0x8c098730 */
 goto L8c098728;
L8c098728: r[12]=r[14]; /* mov r14,r12 */
 goto L8c09872a;
L8c09872a: *pr=0x8c09872eu;r[4]=rd(m,r[12]+0x28u,4);status=call(ctx,0x8c098b42u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b42 */
 goto L8c09872e;
L8c09872e: wr(m,r[12]+0x28u,r[13],4); /* mov.l r13,@(0x28,r12) */
 goto L8c098730;
L8c098730: r[2]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r2 */
 goto L8c098732;
L8c098732: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098734;
L8c098734: if((*sr&1u))goto L8c09873e; /* bt 0x8c09873e */
 goto L8c098736;
L8c098736: r[12]=r[14]; /* mov r14,r12 */
 goto L8c098738;
L8c098738: *pr=0x8c09873cu;r[4]=rd(m,r[12]+0x24u,4);status=call(ctx,0x8c098b42u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b42 */
 goto L8c09873c;
L8c09873c: wr(m,r[12]+0x24u,r[13],4); /* mov.l r13,@(0x24,r12) */
 goto L8c09873e;
L8c09873e: r[2]=rd(m,r[14]+0x2cu,4); /* mov.l @(0x2c,r14),r2 */
 goto L8c098740;
L8c098740: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098742;
L8c098742: if((*sr&1u))goto L8c09874c; /* bt 0x8c09874c */
 goto L8c098744;
L8c098744: r[12]=r[14]; /* mov r14,r12 */
 goto L8c098746;
L8c098746: *pr=0x8c09874au;r[4]=rd(m,r[12]+0x2cu,4);status=call(ctx,0x8c098b42u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b42 */
 goto L8c09874a;
L8c09874a: wr(m,r[12]+0x2cu,r[13],4); /* mov.l r13,@(0x2c,r12) */
 goto L8c09874c;
L8c09874c: r[2]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r2 */
 goto L8c09874e;
L8c09874e: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098750;
L8c098750: if((*sr&1u))goto L8c09875e; /* bt 0x8c09875e */
 goto L8c098752;
L8c098752: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c098754;
L8c098754: r[2]=4294967294u; /* mov #-0x2,r2 */
 goto L8c098756;
L8c098756: r[0]=sx(m,r[3]+0x2u,2); /* mov.w @(0x2,r3),r0 */
 goto L8c098758;
L8c098758: r[0]&=r[2]; /* and r2,r0 */
 goto L8c09875a;
L8c09875a: wr(m,r[3]+0x2u,r[0],2); /* mov.w r0,@(0x2,r3) */
 goto L8c09875c;
L8c09875c: wr(m,r[14]+0x1cu,r[13],4); /* mov.l r13,@(0x1c,r14) */
 goto L8c09875e;
L8c09875e: r[4]=rd(m,0x8c0987e4u,4); /* mov.l 0x8c0987e4,r4 */
 goto L8c098760;
L8c098760: r[12]=rd(m,r[4],4); /* mov.l @r4,r12 */
 goto L8c098762;
L8c098762: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c098764;
L8c098764: if(!(*sr&1u))goto L8c09876e; /* bf 0x8c09876e */
 goto L8c098766;
L8c098766: wr(m,r[4],r[14],4); /* mov.l r14,@r4 */
 goto L8c098768;
L8c098768: wr(m,r[14]+0x4u,r[13],4); /* mov.l r13,@(0x4,r14) */
 goto L8c09876a;
L8c09876a: wr(m,r[14],r[13],4);goto L8c098776; /* bra 0x8c098776 */
 goto L8c09876e;
L8c09876e: wr(m,r[12]+0x4u,r[14],4); /* mov.l r14,@(0x4,r12) */
 goto L8c098770;
L8c098770: wr(m,r[14],r[12],4); /* mov.l r12,@r14 */
 goto L8c098772;
L8c098772: wr(m,r[14]+0x4u,r[13],4); /* mov.l r13,@(0x4,r14) */
 goto L8c098774;
L8c098774: wr(m,r[4],r[14],4); /* mov.l r14,@r4 */
 goto L8c098776;
L8c098776: wr(m,r[14]+0xcu,r[13],4); /* mov.l r13,@(0xc,r14) */
 goto L8c098778;
L8c098778: wr(m,r[14]+0x8u,r[13],4); /* mov.l r13,@(0x8,r14) */
 goto L8c09877a;
L8c09877a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c09877c;
L8c09877c: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c09877e;
L8c09877e: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c098780;
L8c098780: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c098782;
L8c098782: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
