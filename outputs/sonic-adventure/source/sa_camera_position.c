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
/* Static C recovery of retail Tikal helper 8c078eb0..8c078f1e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_camera_position(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c078eb0;
L8c078eb0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c078eb2;
L8c078eb2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c078eb4;
L8c078eb4: r[15]+=4294967276u; /* add #-0x14,r15 */
 goto L8c078eb6;
L8c078eb6: r[14]=r[4]; /* mov r4,r14 */
 goto L8c078eb8;
L8c078eb8: r[4]=r[5]; /* mov r5,r4 */
 goto L8c078eba;
L8c078eba: r[0]=8u; /* mov #0x8,r0 */
 goto L8c078ebc;
L8c078ebc: r[3]=rd(m,0x8c078fc0u,4); /* mov.l 0x8c078fc0,r3 */
 goto L8c078ebe;
L8c078ebe: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c078ec0;
L8c078ec0: fr[3]=rd(m,r[4],4);r[4]+=4; /* fmov.s @r4+,fr3 */
 goto L8c078ec2;
L8c078ec2: wr(m,r[0]+r[15],fr[3],4); /* fmov fr3,@(r0,r15) */
 goto L8c078ec4;
L8c078ec4: r[0]=12u; /* mov #0xc,r0 */
 goto L8c078ec6;
L8c078ec6: fr[3]=rd(m,r[4],4);r[4]+=4; /* fmov.s @r4+,fr3 */
 goto L8c078ec8;
L8c078ec8: wr(m,r[0]+r[15],fr[3],4); /* fmov fr3,@(r0,r15) */
 goto L8c078eca;
L8c078eca: r[0]=16u; /* mov #0x10,r0 */
 goto L8c078ecc;
L8c078ecc: fr[3]=rd(m,r[4],4); /* fmov.s @r4,fr3 */
 goto L8c078ece;
L8c078ece: target=r[3];*pr=0x8c078ed2u;wr(m,r[0]+r[15],fr[3],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c078ed2;
L8c078ed2: r[2]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r2 */
 goto L8c078ed4;
L8c078ed4: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c078ed6;
L8c078ed6: take=(*sr&1u);wr(m,r[15],r[2],4);if(take)goto L8c078ee2;goto L8c078eda; /* bt/s 0x8c078ee2 */
 goto L8c078eda;
L8c078eda: r[3]=rd(m,0x8c078fccu,4); /* mov.l 0x8c078fcc,r3 */
 goto L8c078edc;
L8c078edc: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c078ede;
L8c078ede: target=r[3];*pr=0x8c078ee2u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c078ee2;
L8c078ee2: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c078ee4;
L8c078ee4: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c078ee6;
L8c078ee6: take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto L8c078ef2;goto L8c078eea; /* bt/s 0x8c078ef2 */
 goto L8c078eea;
L8c078eea: r[3]=rd(m,0x8c078fc8u,4); /* mov.l 0x8c078fc8,r3 */
 goto L8c078eec;
L8c078eec: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c078eee;
L8c078eee: target=r[3];*pr=0x8c078ef2u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c078ef2;
L8c078ef2: r[3]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r3 */
 goto L8c078ef4;
L8c078ef4: r[3]=0u-r[3]; /* neg r3,r3 */
 goto L8c078ef6;
L8c078ef6: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c078ef8;
L8c078ef8: take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto L8c078f04;goto L8c078efc; /* bt/s 0x8c078f04 */
 goto L8c078efc;
L8c078efc: r[1]=rd(m,0x8c078fc4u,4); /* mov.l 0x8c078fc4,r1 */
 goto L8c078efe;
L8c078efe: r[5]=r[3]; /* mov r3,r5 */
 goto L8c078f00;
L8c078f00: target=r[1];*pr=0x8c078f04u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c078f04;
L8c078f04: r[3]=rd(m,0x8c078fd0u,4); /* mov.l 0x8c078fd0,r3 */
 goto L8c078f06;
L8c078f06: r[5]=r[15]; /* mov r15,r5 */
 goto L8c078f08;
L8c078f08: r[6]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r6 */
 goto L8c078f0a;
L8c078f0a: r[5]+=8u; /* add #0x8,r5 */
 goto L8c078f0c;
L8c078f0c: target=r[3];*pr=0x8c078f10u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c078f10;
L8c078f10: r[2]=rd(m,0x8c078fd4u,4); /* mov.l 0x8c078fd4,r2 */
 goto L8c078f12;
L8c078f12: target=r[2];*pr=0x8c078f16u;r[4]=1u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c078f16;
L8c078f16: r[15]+=20u; /* add #0x14,r15 */
 goto L8c078f18;
L8c078f18: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c078f1a;
L8c078f1a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
