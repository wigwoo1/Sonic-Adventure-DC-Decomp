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
/* Static C recovery of retail Tikal helper 0cb008a0..0cb00994.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int tikal_display(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;
 goto Lcb008a0;
Lcb008a0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto Lcb008a2;
Lcb008a2: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto Lcb008a4;
Lcb008a4: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto Lcb008a6;
Lcb008a6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto Lcb008a8;
Lcb008a8: r[15]+=4294967276u; /* add #-0x14,r15 */
 goto Lcb008aa;
Lcb008aa: r[13]=rd(m,r[4]+0x20u,4); /* mov.l @(0x20,r4),r13 */
 goto Lcb008ac;
Lcb008ac: r[3]=rd(m,0x0cb009e0u,4); /* mov.l 0x0cb009e0,r3 */
 goto Lcb008ae;
Lcb008ae: r[4]=rd(m,r[4]+0x24u,4); /* mov.l @(0x24,r4),r4 */
 goto Lcb008b0;
Lcb008b0: target=r[3];*pr=0xcb008b4u;r[14]=rd(m,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb008b4;
Lcb008b4: r[2]=rd(m,0x0cb009e8u,4); /* mov.l 0x0cb009e8,r2 */
 goto Lcb008b6;
Lcb008b6: r[5]=rd(m,0x0cb009e4u,4); /* mov.l 0x0cb009e4,r5 */
 goto Lcb008b8;
Lcb008b8: target=r[2];*pr=0xcb008bcu;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb008bc;
Lcb008bc: r[3]=rd(m,0x0cb009ecu,4); /* mov.l 0x0cb009ec,r3 */
 goto Lcb008be;
Lcb008be: target=r[3];*pr=0xcb008c2u;r[4]=2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb008c2;
Lcb008c2: r[0]=73u; /* mov #0x49,r0 */
 goto Lcb008c4;
Lcb008c4: r[0]<<=2; /* shll2 r0 */
 goto Lcb008c6;
Lcb008c6: r[12]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r12 */
 goto Lcb008c8;
Lcb008c8: r[0]=72u; /* mov #0x48,r0 */
 goto Lcb008ca;
Lcb008ca: r[0]<<=2; /* shll2 r0 */
 goto Lcb008cc;
Lcb008cc: r[0]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r0 */
 goto Lcb008ce;
Lcb008ce: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto Lcb008d0;
Lcb008d0: take=!(*sr&1u);r[12]=r[12]&65535u;if(take)goto Lcb008de;goto Lcb008d4; /* bf/s 0x0cb008de */
 goto Lcb008d4;
Lcb008d4: r[0]=1u; /* mov #0x1,r0 */
 goto Lcb008d6;
Lcb008d6: r[0]<<=8; /* shll8 r0 */
 goto Lcb008d8;
Lcb008d8: r[0]+=38u; /* add #0x26,r0 */
 goto Lcb008da;
Lcb008da: r[12]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r12 */
 goto Lcb008dc;
Lcb008dc: r[12]=r[12]&65535u; /* extu.w r12,r12 */
 goto Lcb008de;
Lcb008de: r[0]=sx(m,r[13]+0x6u,2); /* mov.w @(0x6,r13),r0 */
 goto Lcb008e0;
Lcb008e0: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto Lcb008e2;
Lcb008e2: *sr=(*sr&~1u)|!(r[0]&0x1u); /* tst #0x1,r0 */
 goto Lcb008e4;
Lcb008e4: if(!(*sr&1u))goto Lcb00980; /* bf 0x0cb00980 */
 goto Lcb008e6;
Lcb008e6: r[2]=rd(m,0x0cb009f4u,4); /* mov.l 0x0cb009f4,r2 */
 goto Lcb008e8;
Lcb008e8: r[4]=rd(m,0x0cb009f0u,4); /* mov.l 0x0cb009f0,r4 */
 goto Lcb008ea;
Lcb008ea: target=r[2];*pr=0xcb008eeu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb008ee;
Lcb008ee: r[3]=rd(m,0x0cb009f8u,4); /* mov.l 0x0cb009f8,r3 */
 goto Lcb008f0;
Lcb008f0: target=r[3];*pr=0xcb008f4u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb008f4;
Lcb008f4: r[5]=rd(m,r[13]+0x38u,4); /* mov.l @(0x38,r13),r5 */
 goto Lcb008f6;
Lcb008f6: r[3]=rd(m,0x0cb009fcu,4); /* mov.l 0x0cb009fc,r3 */
 goto Lcb008f8;
Lcb008f8: r[5]=rd(m,r[5]+0xcu,4); /* mov.l @(0xc,r5),r5 */
 goto Lcb008fa;
Lcb008fa: r[5]+=8u; /* add #0x8,r5 */
 goto Lcb008fc;
Lcb008fc: target=r[3];*pr=0xcb00900u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00900;
Lcb00900: r[2]=rd(m,r[13]+0x1cu,4); /* mov.l @(0x1c,r13),r2 */
 goto Lcb00902;
Lcb00902: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto Lcb00904;
Lcb00904: take=(*sr&1u);wr(m,r[15],r[2],4);if(take)goto Lcb00910;goto Lcb00908; /* bt/s 0x0cb00910 */
 goto Lcb00908;
Lcb00908: r[3]=rd(m,0x0cb00a00u,4); /* mov.l 0x0cb00a00,r3 */
 goto Lcb0090a;
Lcb0090a: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto Lcb0090c;
Lcb0090c: target=r[3];*pr=0xcb00910u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00910;
Lcb00910: r[3]=rd(m,r[13]+0x14u,4); /* mov.l @(0x14,r13),r3 */
 goto Lcb00912;
Lcb00912: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto Lcb00914;
Lcb00914: take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto Lcb00920;goto Lcb00918; /* bt/s 0x0cb00920 */
 goto Lcb00918;
Lcb00918: r[3]=rd(m,0x0cb00a04u,4); /* mov.l 0x0cb00a04,r3 */
 goto Lcb0091a;
Lcb0091a: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto Lcb0091c;
Lcb0091c: target=r[3];*pr=0xcb00920u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00920;
Lcb00920: r[3]=rd(m,r[13]+0x18u,4); /* mov.l @(0x18,r13),r3 */
 goto Lcb00922;
Lcb00922: r[2]=rd(m,0x0cb00a08u,4); /* mov.l 0x0cb00a08,r2 */
 goto Lcb00924;
Lcb00924: r[3]=0u-r[3]; /* neg r3,r3 */
 goto Lcb00926;
Lcb00926: r[3]+=r[2]; /* add r2,r3 */
 goto Lcb00928;
Lcb00928: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto Lcb0092a;
Lcb0092a: take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto Lcb00936;goto Lcb0092e; /* bt/s 0x0cb00936 */
 goto Lcb0092e;
Lcb0092e: r[1]=rd(m,0x0cb00a0cu,4); /* mov.l 0x0cb00a0c,r1 */
 goto Lcb00930;
Lcb00930: r[5]=r[3]; /* mov r3,r5 */
 goto Lcb00932;
Lcb00932: target=r[1];*pr=0xcb00936u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto Lcb00936;
Lcb00936: r[2]=rd(m,r[13]+0x3cu,4); /* mov.l @(0x3c,r13),r2 */
 goto Lcb00938;
Lcb00938: r[0]=64u; /* mov #0x40,r0 */
 goto Lcb0093a;
Lcb0093a: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto Lcb0093c;
Lcb0093c: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto Lcb0093e;
Lcb0093e: if((*sr&1u))goto Lcb0094a; /* bt 0x0cb0094a */
 goto Lcb00940;
Lcb00940: r[3]=rd(m,0x0cb00a10u,4); /* mov.l 0x0cb00a10,r3 */
 goto Lcb00942;
Lcb00942: target=r[3];*pr=0xcb00946u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00946;
Lcb00946: goto Lcb0097a; /* bra 0x0cb0097a */
 goto Lcb0094a;
Lcb0094a: r[0]=72u; /* mov #0x48,r0 */
 goto Lcb0094c;
Lcb0094c: r[0]<<=2; /* shll2 r0 */
 goto Lcb0094e;
Lcb0094e: r[0]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r0 */
 goto Lcb00950;
Lcb00950: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto Lcb00952;
Lcb00952: if((*sr&1u))goto Lcb0096a; /* bt 0x0cb0096a */
 goto Lcb00954;
Lcb00954: r[0]=76u; /* mov #0x4c,r0 */
 goto Lcb00956;
Lcb00956: r[4]=r[12]; /* mov r12,r4 */
 goto Lcb00958;
Lcb00958: r[0]<<=2; /* shll2 r0 */
 goto Lcb0095a;
Lcb0095a: fr[4]=rd(m,r[0]+r[14],4); /* fmov.s @(r0,r14),fr4 */
 goto Lcb0095c;
Lcb0095c: r[0]+=16u; /* add #0x10,r0 */
 goto Lcb0095e;
Lcb0095e: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto Lcb00960;
Lcb00960: r[4]<<=2; /* shll2 r4 */
 goto Lcb00962;
Lcb00962: r[4]<<=2; /* shll2 r4 */
 goto Lcb00964;
Lcb00964: r[4]+=r[3]; /* add r3,r4 */
 goto Lcb00966;
Lcb00966: r[4]=rd(m,r[4],4);goto Lcb00974; /* bra 0x0cb00974 */
 goto Lcb0096a;
Lcb0096a: r[0]=76u; /* mov #0x4c,r0 */
 goto Lcb0096c;
Lcb0096c: r[0]<<=2; /* shll2 r0 */
 goto Lcb0096e;
Lcb0096e: fr[4]=rd(m,r[0]+r[14],4); /* fmov.s @(r0,r14),fr4 */
 goto Lcb00970;
Lcb00970: r[0]+=24u; /* add #0x18,r0 */
 goto Lcb00972;
Lcb00972: r[4]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r4 */
 goto Lcb00974;
Lcb00974: r[3]=rd(m,0x0cb00a14u,4); /* mov.l 0x0cb00a14,r3 */
 goto Lcb00976;
Lcb00976: target=r[3];*pr=0xcb0097au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb0097a;
Lcb0097a: r[2]=rd(m,0x0cb00a18u,4); /* mov.l 0x0cb00a18,r2 */
 goto Lcb0097c;
Lcb0097c: target=r[2];*pr=0xcb00980u;r[4]=1u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb00980;
Lcb00980: r[3]=rd(m,0x0cb009ecu,4); /* mov.l 0x0cb009ec,r3 */
 goto Lcb00982;
Lcb00982: target=r[3];*pr=0xcb00986u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00986;
Lcb00986: r[15]+=20u; /* add #0x14,r15 */
 goto Lcb00988;
Lcb00988: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb0098a;
Lcb0098a: r[2]=rd(m,0x0cb00a1cu,4); /* mov.l 0x0cb00a1c,r2 */
 goto Lcb0098c;
Lcb0098c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto Lcb0098e;
Lcb0098e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto Lcb00990;
Lcb00990: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
}
