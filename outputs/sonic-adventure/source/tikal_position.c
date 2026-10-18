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
/* Static C recovery of retail Tikal helper 0cb007a0..0cb00846.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int tikal_position(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;
 goto Lcb007a0;
Lcb007a0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto Lcb007a2;
Lcb007a2: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto Lcb007a4;
Lcb007a4: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto Lcb007a6;
Lcb007a6: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto Lcb007a8;
Lcb007a8: r[14]=r[4]; /* mov r4,r14 */
 goto Lcb007aa;
Lcb007aa: r[4]=rd(m,0x0cb00870u,4); /* mov.l 0x0cb00870,r4 */
 goto Lcb007ac;
Lcb007ac: r[3]=rd(m,0x0cb00874u,4); /* mov.l 0x0cb00874,r3 */
 goto Lcb007ae;
Lcb007ae: wr(m,r[15],r[6],4); /* mov.l r6,@r15 */
 goto Lcb007b0;
Lcb007b0: r[13]=rd(m,r[14]+0x38u,4); /* mov.l @(0x38,r14),r13 */
 goto Lcb007b2;
Lcb007b2: target=r[3];*pr=0xcb007b6u;r[13]=rd(m,r[13]+0xcu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb007b6;
Lcb007b6: r[0]=12u; /* mov #0xc,r0 */
 goto Lcb007b8;
Lcb007b8: fr[3]=0; /* fldi0 fr3 */
 goto Lcb007ba;
Lcb007ba: wr(m,r[0]+r[15],fr[3],4); /* fmov fr3,@(r0,r15) */
 goto Lcb007bc;
Lcb007bc: r[0]=4u; /* mov #0x4,r0 */
 goto Lcb007be;
Lcb007be: wr(m,r[0]+r[15],fr[3],4); /* fmov fr3,@(r0,r15) */
 goto Lcb007c0;
Lcb007c0: r[0]=71u; /* mov #0x47,r0 */
 goto Lcb007c2;
Lcb007c2: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto Lcb007c4;
Lcb007c4: r[0]<<=2; /* shll2 r0 */
 goto Lcb007c6;
Lcb007c6: fr[3]=rd(m,r[0]+r[3],4); /* fmov.s @(r0,r3),fr3 */
 goto Lcb007c8;
Lcb007c8: r[0]=8u; /* mov #0x8,r0 */
 goto Lcb007ca;
Lcb007ca: wr(m,r[0]+r[15],fr[3],4); /* fmov fr3,@(r0,r15) */
 goto Lcb007cc;
Lcb007cc: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto Lcb007ce;
Lcb007ce: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto Lcb007d0;
Lcb007d0: take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto Lcb007dc;goto Lcb007d4; /* bt/s 0x0cb007dc */
 goto Lcb007d4;
Lcb007d4: r[3]=rd(m,0x0cb00878u,4); /* mov.l 0x0cb00878,r3 */
 goto Lcb007d6;
Lcb007d6: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto Lcb007d8;
Lcb007d8: target=r[3];*pr=0xcb007dcu;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb007dc;
Lcb007dc: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto Lcb007de;
Lcb007de: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto Lcb007e0;
Lcb007e0: take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto Lcb007ec;goto Lcb007e4; /* bt/s 0x0cb007ec */
 goto Lcb007e4;
Lcb007e4: r[3]=rd(m,0x0cb0087cu,4); /* mov.l 0x0cb0087c,r3 */
 goto Lcb007e6;
Lcb007e6: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto Lcb007e8;
Lcb007e8: target=r[3];*pr=0xcb007ecu;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb007ec;
Lcb007ec: r[3]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r3 */
 goto Lcb007ee;
Lcb007ee: r[3]=0u-r[3]; /* neg r3,r3 */
 goto Lcb007f0;
Lcb007f0: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto Lcb007f2;
Lcb007f2: take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto Lcb007fe;goto Lcb007f6; /* bt/s 0x0cb007fe */
 goto Lcb007f6;
Lcb007f6: r[1]=rd(m,0x0cb00880u,4); /* mov.l 0x0cb00880,r1 */
 goto Lcb007f8;
Lcb007f8: r[5]=r[3]; /* mov r3,r5 */
 goto Lcb007fa;
Lcb007fa: target=r[1];*pr=0xcb007feu;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto Lcb007fe;
Lcb007fe: r[5]=r[15]; /* mov r15,r5 */
 goto Lcb00800;
Lcb00800: r[2]=rd(m,0x0cb00884u,4); /* mov.l 0x0cb00884,r2 */
 goto Lcb00802;
Lcb00802: r[6]=r[13]; /* mov r13,r6 */
 goto Lcb00804;
Lcb00804: r[5]+=4u; /* add #0x4,r5 */
 goto Lcb00806;
Lcb00806: r[6]+=8u; /* add #0x8,r6 */
 goto Lcb00808;
Lcb00808: target=r[2];*pr=0xcb0080cu;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto Lcb0080c;
Lcb0080c: r[3]=rd(m,0x0cb00888u,4); /* mov.l 0x0cb00888,r3 */
 goto Lcb0080e;
Lcb0080e: target=r[3];*pr=0xcb00812u;r[4]=1u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto Lcb00812;
Lcb00812: r[1]=32u; /* mov #0x20,r1 */
 goto Lcb00814;
Lcb00814: r[1]+=r[14]; /* add r14,r1 */
 goto Lcb00816;
Lcb00816: r[0]=8u; /* mov #0x8,r0 */
 goto Lcb00818;
Lcb00818: fr[3]=rd(m,r[1],4); /* fmov.s @r1,fr3 */
 goto Lcb0081a;
Lcb0081a: fr[2]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr2 */
 goto Lcb0081c;
Lcb0081c: r[1]=36u; /* mov #0x24,r1 */
 goto Lcb0081e;
Lcb0081e: r[1]+=r[14]; /* add r14,r1 */
 goto Lcb00820;
Lcb00820: status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status; /* fadd fr3,fr2 */
 goto Lcb00822;
Lcb00822: wr(m,r[0]+r[13],fr[2],4); /* fmov fr2,@(r0,r13) */
 goto Lcb00824;
Lcb00824: r[0]=12u; /* mov #0xc,r0 */
 goto Lcb00826;
Lcb00826: fr[2]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr2 */
 goto Lcb00828;
Lcb00828: fr[3]=rd(m,r[1],4); /* fmov.s @r1,fr3 */
 goto Lcb0082a;
Lcb0082a: r[1]=40u; /* mov #0x28,r1 */
 goto Lcb0082c;
Lcb0082c: r[1]+=r[14]; /* add r14,r1 */
 goto Lcb0082e;
Lcb0082e: status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status; /* fadd fr3,fr2 */
 goto Lcb00830;
Lcb00830: wr(m,r[0]+r[13],fr[2],4); /* fmov fr2,@(r0,r13) */
 goto Lcb00832;
Lcb00832: r[0]=16u; /* mov #0x10,r0 */
 goto Lcb00834;
Lcb00834: fr[2]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr2 */
 goto Lcb00836;
Lcb00836: fr[3]=rd(m,r[1],4); /* fmov.s @r1,fr3 */
 goto Lcb00838;
Lcb00838: status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status; /* fadd fr3,fr2 */
 goto Lcb0083a;
Lcb0083a: wr(m,r[0]+r[13],fr[2],4); /* fmov fr2,@(r0,r13) */
 goto Lcb0083c;
Lcb0083c: r[15]+=16u; /* add #0x10,r15 */
 goto Lcb0083e;
Lcb0083e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto Lcb00840;
Lcb00840: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto Lcb00842;
Lcb00842: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
