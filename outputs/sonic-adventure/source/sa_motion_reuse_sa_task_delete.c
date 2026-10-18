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
/* Retail task deletion 8c09865c..8c098784. List unlinking, resource cleanup calls, and free-list insertion. */
SA_STAGE_EXPORT int sa_task_delete(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c09865c;
L8c09865c: sa_motion_publish_stop(ctx,0x8c09865cu); r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c09865e;
L8c09865e: sa_motion_publish_stop(ctx,0x8c09865eu); r[14]=r[4]; /* mov r4,r14 */
 goto L8c098660;
L8c098660: sa_motion_publish_stop(ctx,0x8c098660u); r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c098662;
L8c098662: sa_motion_publish_stop(ctx,0x8c098662u); r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c098664;
L8c098664: sa_motion_publish_stop(ctx,0x8c098664u); r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c098666;
L8c098666: sa_motion_publish_stop(ctx,0x8c098666u); r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c098668;
L8c098668: sa_motion_publish_stop(ctx,0x8c098668u); r[3]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r3 */
 goto L8c09866a;
L8c09866a: sa_motion_publish_stop(ctx,0x8c09866au); *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c09866c;
L8c09866c: sa_motion_publish_stop(ctx,0x8c09866cu); if((*sr&1u))goto L8c098674; /* bt 0x8c098674 */
 goto L8c09866e;
L8c09866e: sa_motion_publish_stop(ctx,0x8c09866eu); r[3]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r3 */
 goto L8c098670;
L8c098670: sa_motion_publish_stop(ctx,0x8c098672u); target=r[3];*pr=0x8c098674u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c098674;
L8c098674: sa_motion_publish_stop(ctx,0x8c098674u); r[13]=0u; /* mov #0x0,r13 */
 goto L8c098676;
L8c098676: sa_motion_publish_stop(ctx,0x8c098676u); wr(m,r[14]+0x10u,r[13],4); /* mov.l r13,@(0x10,r14) */
 goto L8c098678;
L8c098678: sa_motion_publish_stop(ctx,0x8c098678u); wr(m,r[14]+0x14u,r[13],4); /* mov.l r13,@(0x14,r14) */
 goto L8c09867a;
L8c09867a: sa_motion_publish_stop(ctx,0x8c09867au); wr(m,r[14]+0x18u,r[13],4); /* mov.l r13,@(0x18,r14) */
 goto L8c09867c;
L8c09867c: sa_motion_publish_stop(ctx,0x8c09867cu); r[2]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r2 */
 goto L8c09867e;
L8c09867e: sa_motion_publish_stop(ctx,0x8c09867eu); r[12]=rd(m,r[14],4); /* mov.l @r14,r12 */
 goto L8c098680;
L8c098680: sa_motion_publish_stop(ctx,0x8c098680u); *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098682;
L8c098682: sa_motion_publish_stop(ctx,0x8c098684u); take=(*sr&1u);r[11]=rd(m,r[14]+0x4u,4);if(take)goto L8c09868a;goto L8c098686; /* bt/s 0x8c09868a */
 goto L8c098686;
L8c098686: sa_motion_publish_stop(ctx,0x8c098688u); *pr=0x8c09868au;r[4]=r[14];status=call(ctx,0x8c098e60u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098e60 */
 goto L8c09868a;
L8c09868a: sa_motion_publish_stop(ctx,0x8c09868au); *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c09868c;
L8c09868c: sa_motion_publish_stop(ctx,0x8c09868cu); if((*sr&1u))goto L8c098698; /* bt 0x8c098698 */
 goto L8c09868e;
L8c09868e: sa_motion_publish_stop(ctx,0x8c09868eu); *sr=(*sr&~1u)|!(r[11]&r[11]); /* tst r11,r11 */
 goto L8c098690;
L8c098690: sa_motion_publish_stop(ctx,0x8c098690u); if((*sr&1u))goto L8c098698; /* bt 0x8c098698 */
 goto L8c098692;
L8c098692: sa_motion_publish_stop(ctx,0x8c098692u); wr(m,r[12]+0x4u,r[11],4); /* mov.l r11,@(0x4,r12) */
 goto L8c098694;
L8c098694: sa_motion_publish_stop(ctx,0x8c098696u); wr(m,r[11],r[12],4);goto L8c098718; /* bra 0x8c098718 */
 goto L8c098698;
L8c098698: sa_motion_publish_stop(ctx,0x8c098698u); *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c09869a;
L8c09869a: sa_motion_publish_stop(ctx,0x8c09869au); if(!(*sr&1u))goto L8c0986e0; /* bf 0x8c0986e0 */
 goto L8c09869c;
L8c09869c: sa_motion_publish_stop(ctx,0x8c09869cu); *sr=(*sr&~1u)|!(r[11]&r[11]); /* tst r11,r11 */
 goto L8c09869e;
L8c09869e: sa_motion_publish_stop(ctx,0x8c09869eu); if((*sr&1u))goto L8c0986e0; /* bt 0x8c0986e0 */
 goto L8c0986a0;
L8c0986a0: sa_motion_publish_stop(ctx,0x8c0986a2u); wr(m,r[11],r[13],4);goto L8c098718; /* bra 0x8c098718 */
 goto L8c0986e0;
L8c0986e0: sa_motion_publish_stop(ctx,0x8c0986e0u); r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c0986e2;
L8c0986e2: sa_motion_publish_stop(ctx,0x8c0986e2u); *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c0986e4;
L8c0986e4: sa_motion_publish_stop(ctx,0x8c0986e4u); if((*sr&1u))goto L8c0986f2; /* bt 0x8c0986f2 */
 goto L8c0986e6;
L8c0986e6: sa_motion_publish_stop(ctx,0x8c0986e6u); *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c0986e8;
L8c0986e8: sa_motion_publish_stop(ctx,0x8c0986e8u); if((*sr&1u))goto L8c0986ec; /* bt 0x8c0986ec */
 goto L8c0986ea;
L8c0986ea: sa_motion_publish_stop(ctx,0x8c0986eau); wr(m,r[12]+0x4u,r[13],4); /* mov.l r13,@(0x4,r12) */
 goto L8c0986ec;
L8c0986ec: sa_motion_publish_stop(ctx,0x8c0986ecu); r[2]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r2 */
 goto L8c0986ee;
L8c0986ee: sa_motion_publish_stop(ctx,0x8c0986f0u); wr(m,r[2]+0xcu,r[12],4);goto L8c098718; /* bra 0x8c098718 */
 goto L8c0986f2;
L8c0986f2: sa_motion_publish_stop(ctx,0x8c0986f2u); *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c0986f4;
L8c0986f4: sa_motion_publish_stop(ctx,0x8c0986f4u); if((*sr&1u))goto L8c0986f8; /* bt 0x8c0986f8 */
 goto L8c0986f6;
L8c0986f6: sa_motion_publish_stop(ctx,0x8c0986f6u); wr(m,r[12]+0x4u,r[13],4); /* mov.l r13,@(0x4,r12) */
 goto L8c0986f8;
L8c0986f8: sa_motion_publish_stop(ctx,0x8c0986f8u); r[6]=rd(m,0x8c0987e0u,4); /* mov.l 0x8c0987e0,r6 */
 goto L8c0986fa;
L8c0986fa: sa_motion_publish_stop(ctx,0x8c0986fau); r[7]=r[6]; /* mov r6,r7 */
 goto L8c0986fc;
L8c0986fc: sa_motion_publish_stop(ctx,0x8c0986fcu); r[7]+=32u; /* add #0x20,r7 */
 goto L8c0986fe;
L8c0986fe: sa_motion_publish_stop(ctx,0x8c0986feu); r[4]=r[6]; /* mov r6,r4 */
 goto L8c098700;
L8c098700: sa_motion_publish_stop(ctx,0x8c098700u); *sr=(*sr&~1u)|(r[4]>=r[7]); /* cmp/hs r7,r4 */
 goto L8c098702;
L8c098702: sa_motion_publish_stop(ctx,0x8c098704u); take=(*sr&1u);r[5]=r[6];if(take)goto L8c098718;goto L8c098706; /* bt/s 0x8c098718 */
 goto L8c098706;
L8c098706: sa_motion_publish_stop(ctx,0x8c098706u); r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c098708;
L8c098708: sa_motion_publish_stop(ctx,0x8c098708u); *sr=(*sr&~1u)|(r[2]==r[14]); /* cmp/eq r2,r14 */
 goto L8c09870a;
L8c09870a: sa_motion_publish_stop(ctx,0x8c09870au); if(!(*sr&1u))goto L8c098710; /* bf 0x8c098710 */
 goto L8c09870c;
L8c09870c: sa_motion_publish_stop(ctx,0x8c09870eu); wr(m,r[5],r[12],4);goto L8c098718; /* bra 0x8c098718 */
 goto L8c098710;
L8c098710: sa_motion_publish_stop(ctx,0x8c098710u); r[4]+=4u; /* add #0x4,r4 */
 goto L8c098712;
L8c098712: sa_motion_publish_stop(ctx,0x8c098712u); *sr=(*sr&~1u)|(r[4]>=r[7]); /* cmp/hs r7,r4 */
 goto L8c098714;
L8c098714: sa_motion_publish_stop(ctx,0x8c098716u); take=!(*sr&1u);r[5]+=4u;if(take)goto L8c098706;goto L8c098718; /* bf/s 0x8c098706 */
 goto L8c098718;
L8c098718: sa_motion_publish_stop(ctx,0x8c098718u); r[2]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r2 */
 goto L8c09871a;
L8c09871a: sa_motion_publish_stop(ctx,0x8c09871au); *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c09871c;
L8c09871c: sa_motion_publish_stop(ctx,0x8c09871cu); if((*sr&1u))goto L8c098722; /* bt 0x8c098722 */
 goto L8c09871e;
L8c09871e: sa_motion_publish_stop(ctx,0x8c098720u); *pr=0x8c098722u;r[4]=r[14];status=call(ctx,0x8c098998u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098998 */
 goto L8c098722;
L8c098722: sa_motion_publish_stop(ctx,0x8c098722u); r[2]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r2 */
 goto L8c098724;
L8c098724: sa_motion_publish_stop(ctx,0x8c098724u); *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098726;
L8c098726: sa_motion_publish_stop(ctx,0x8c098726u); if((*sr&1u))goto L8c098730; /* bt 0x8c098730 */
 goto L8c098728;
L8c098728: sa_motion_publish_stop(ctx,0x8c098728u); r[12]=r[14]; /* mov r14,r12 */
 goto L8c09872a;
L8c09872a: sa_motion_publish_stop(ctx,0x8c09872cu); *pr=0x8c09872eu;r[4]=rd(m,r[12]+0x28u,4);status=call(ctx,0x8c098b42u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b42 */
 goto L8c09872e;
L8c09872e: sa_motion_publish_stop(ctx,0x8c09872eu); wr(m,r[12]+0x28u,r[13],4); /* mov.l r13,@(0x28,r12) */
 goto L8c098730;
L8c098730: sa_motion_publish_stop(ctx,0x8c098730u); r[2]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r2 */
 goto L8c098732;
L8c098732: sa_motion_publish_stop(ctx,0x8c098732u); *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098734;
L8c098734: sa_motion_publish_stop(ctx,0x8c098734u); if((*sr&1u))goto L8c09873e; /* bt 0x8c09873e */
 goto L8c098736;
L8c098736: sa_motion_publish_stop(ctx,0x8c098736u); r[12]=r[14]; /* mov r14,r12 */
 goto L8c098738;
L8c098738: sa_motion_publish_stop(ctx,0x8c09873au); *pr=0x8c09873cu;r[4]=rd(m,r[12]+0x24u,4);status=call(ctx,0x8c098b42u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b42 */
 goto L8c09873c;
L8c09873c: sa_motion_publish_stop(ctx,0x8c09873cu); wr(m,r[12]+0x24u,r[13],4); /* mov.l r13,@(0x24,r12) */
 goto L8c09873e;
L8c09873e: sa_motion_publish_stop(ctx,0x8c09873eu); r[2]=rd(m,r[14]+0x2cu,4); /* mov.l @(0x2c,r14),r2 */
 goto L8c098740;
L8c098740: sa_motion_publish_stop(ctx,0x8c098740u); *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098742;
L8c098742: sa_motion_publish_stop(ctx,0x8c098742u); if((*sr&1u))goto L8c09874c; /* bt 0x8c09874c */
 goto L8c098744;
L8c098744: sa_motion_publish_stop(ctx,0x8c098744u); r[12]=r[14]; /* mov r14,r12 */
 goto L8c098746;
L8c098746: sa_motion_publish_stop(ctx,0x8c098748u); *pr=0x8c09874au;r[4]=rd(m,r[12]+0x2cu,4);status=call(ctx,0x8c098b42u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098b42 */
 goto L8c09874a;
L8c09874a: sa_motion_publish_stop(ctx,0x8c09874au); wr(m,r[12]+0x2cu,r[13],4); /* mov.l r13,@(0x2c,r12) */
 goto L8c09874c;
L8c09874c: sa_motion_publish_stop(ctx,0x8c09874cu); r[2]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r2 */
 goto L8c09874e;
L8c09874e: sa_motion_publish_stop(ctx,0x8c09874eu); *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c098750;
L8c098750: sa_motion_publish_stop(ctx,0x8c098750u); if((*sr&1u))goto L8c09875e; /* bt 0x8c09875e */
 goto L8c098752;
L8c098752: sa_motion_publish_stop(ctx,0x8c098752u); r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c098754;
L8c098754: sa_motion_publish_stop(ctx,0x8c098754u); r[2]=4294967294u; /* mov #-0x2,r2 */
 goto L8c098756;
L8c098756: sa_motion_publish_stop(ctx,0x8c098756u); r[0]=sx(m,r[3]+0x2u,2); /* mov.w @(0x2,r3),r0 */
 goto L8c098758;
L8c098758: sa_motion_publish_stop(ctx,0x8c098758u); r[0]&=r[2]; /* and r2,r0 */
 goto L8c09875a;
L8c09875a: sa_motion_publish_stop(ctx,0x8c09875au); wr(m,r[3]+0x2u,r[0],2); /* mov.w r0,@(0x2,r3) */
 goto L8c09875c;
L8c09875c: sa_motion_publish_stop(ctx,0x8c09875cu); wr(m,r[14]+0x1cu,r[13],4); /* mov.l r13,@(0x1c,r14) */
 goto L8c09875e;
L8c09875e: sa_motion_publish_stop(ctx,0x8c09875eu); r[4]=rd(m,0x8c0987e4u,4); /* mov.l 0x8c0987e4,r4 */
 goto L8c098760;
L8c098760: sa_motion_publish_stop(ctx,0x8c098760u); r[12]=rd(m,r[4],4); /* mov.l @r4,r12 */
 goto L8c098762;
L8c098762: sa_motion_publish_stop(ctx,0x8c098762u); *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c098764;
L8c098764: sa_motion_publish_stop(ctx,0x8c098764u); if(!(*sr&1u))goto L8c09876e; /* bf 0x8c09876e */
 goto L8c098766;
L8c098766: sa_motion_publish_stop(ctx,0x8c098766u); wr(m,r[4],r[14],4); /* mov.l r14,@r4 */
 goto L8c098768;
L8c098768: sa_motion_publish_stop(ctx,0x8c098768u); wr(m,r[14]+0x4u,r[13],4); /* mov.l r13,@(0x4,r14) */
 goto L8c09876a;
L8c09876a: sa_motion_publish_stop(ctx,0x8c09876cu); wr(m,r[14],r[13],4);goto L8c098776; /* bra 0x8c098776 */
 goto L8c09876e;
L8c09876e: sa_motion_publish_stop(ctx,0x8c09876eu); wr(m,r[12]+0x4u,r[14],4); /* mov.l r14,@(0x4,r12) */
 goto L8c098770;
L8c098770: sa_motion_publish_stop(ctx,0x8c098770u); wr(m,r[14],r[12],4); /* mov.l r12,@r14 */
 goto L8c098772;
L8c098772: sa_motion_publish_stop(ctx,0x8c098772u); wr(m,r[14]+0x4u,r[13],4); /* mov.l r13,@(0x4,r14) */
 goto L8c098774;
L8c098774: sa_motion_publish_stop(ctx,0x8c098774u); wr(m,r[4],r[14],4); /* mov.l r14,@r4 */
 goto L8c098776;
L8c098776: sa_motion_publish_stop(ctx,0x8c098776u); wr(m,r[14]+0xcu,r[13],4); /* mov.l r13,@(0xc,r14) */
 goto L8c098778;
L8c098778: sa_motion_publish_stop(ctx,0x8c098778u); wr(m,r[14]+0x8u,r[13],4); /* mov.l r13,@(0x8,r14) */
 goto L8c09877a;
L8c09877a: sa_motion_publish_stop(ctx,0x8c09877au); *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c09877c;
L8c09877c: sa_motion_publish_stop(ctx,0x8c09877cu); r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c09877e;
L8c09877e: sa_motion_publish_stop(ctx,0x8c09877eu); r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c098780;
L8c098780: sa_motion_publish_stop(ctx,0x8c098780u); r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c098782;
L8c098782: sa_motion_publish_stop(ctx,0x8c098784u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
