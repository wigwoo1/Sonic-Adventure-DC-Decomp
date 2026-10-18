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
/* Static C recovery of retail Tikal helper 8c643f98..8c643ff0.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_interrupt_service_643f98(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c643f98;
L8c643f98: r[3]=rd(m,0x8c643ff4u,4); /* mov.l 0x8c643ff4,r3 */
 goto L8c643f9a;
L8c643f9a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c643f9c;
L8c643f9c: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c643f9e;
L8c643f9e: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c643fa0;
L8c643fa0: if((*sr&1u))goto L8c643fea; /* bt 0x8c643fea */
 goto L8c643fa2;
L8c643fa2: r[1]=rd(m,0x8c643ff8u,4); /* mov.l 0x8c643ff8,r1 */
 goto L8c643fa4;
L8c643fa4: r[0]=rd(m,r[1],4); /* mov.l @r1,r0 */
 goto L8c643fa6;
L8c643fa6: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c643fa8;
L8c643fa8: if(!(*sr&1u))goto L8c643fea; /* bf 0x8c643fea */
 goto L8c643faa;
L8c643faa: r[0]=sx(m,0x8c643ff0u,2); /* mov.w 0x8c643ff0,r0 */
 goto L8c643fac;
L8c643fac: r[3]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r3 */
 goto L8c643fae;
L8c643fae: *sr=(*sr&~1u)|((int32_t)r[3]>0); /* cmp/pl r3 */
 goto L8c643fb0;
L8c643fb0: if(!(*sr&1u))goto L8c643fb8; /* bf 0x8c643fb8 */
 goto L8c643fb2;
L8c643fb2: r[1]=rd(m,0x8c643ffcu,4); /* mov.l 0x8c643ffc,r1 */
 goto L8c643fb4;
L8c643fb4: target=r[1];*pr=0x8c643fb8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c643fb8;
L8c643fb8: r[1]=rd(m,0x8c643ff4u,4); /* mov.l 0x8c643ff4,r1 */
 goto L8c643fba;
L8c643fba: r[0]=sx(m,0x8c643ff2u,2); /* mov.w 0x8c643ff2,r0 */
 goto L8c643fbc;
L8c643fbc: r[2]=rd(m,r[1],4); /* mov.l @r1,r2 */
 goto L8c643fbe;
L8c643fbe: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c643fc0;
L8c643fc0: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c643fc2;
L8c643fc2: if((*sr&1u))goto L8c643fea; /* bt 0x8c643fea */
 goto L8c643fc4;
L8c643fc4: r[3]=rd(m,0x8c644004u,4); /* mov.l 0x8c644004,r3 */
 goto L8c643fc6;
L8c643fc6: r[5]=rd(m,0x8c644000u,4); /* mov.l 0x8c644000,r5 */
 goto L8c643fc8;
L8c643fc8: target=r[3];*pr=0x8c643fccu;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643fcc;
L8c643fcc: r[2]=rd(m,0x8c64400cu,4); /* mov.l 0x8c64400c,r2 */
 goto L8c643fce;
L8c643fce: r[5]=rd(m,0x8c644008u,4); /* mov.l 0x8c644008,r5 */
 goto L8c643fd0;
L8c643fd0: target=r[2];*pr=0x8c643fd4u;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c643fd4;
L8c643fd4: r[0]=rd(m,0x8c644008u,4); /* mov.l 0x8c644008,r0 */
 goto L8c643fd6;
L8c643fd6: r[1]=rd(m,0x8c644000u,4); /* mov.l 0x8c644000,r1 */
 goto L8c643fd8;
L8c643fd8: r[2]=rd(m,r[0],4); /* mov.l @r0,r2 */
 goto L8c643fda;
L8c643fda: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c643fdc;
L8c643fdc: *sr=(*sr&~1u)|(r[2]>=r[3]); /* cmp/hs r3,r2 */
 goto L8c643fde;
L8c643fde: if((*sr&1u))goto L8c643fea; /* bt 0x8c643fea */
 goto L8c643fe0;
L8c643fe0: r[2]=rd(m,0x8c644018u,4); /* mov.l 0x8c644018,r2 */
 goto L8c643fe2;
L8c643fe2: r[4]=rd(m,0x8c644014u,4); /* mov.l 0x8c644014,r4 */
 goto L8c643fe4;
L8c643fe4: r[5]=rd(m,0x8c644010u,4); /* mov.l 0x8c644010,r5 */
 goto L8c643fe6;
L8c643fe6: target=r[2];*pr=0x8c643feau;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c643fea;
L8c643fea: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c643fec;
L8c643fec: r[0]=0u;return 0; /* rts */
}
