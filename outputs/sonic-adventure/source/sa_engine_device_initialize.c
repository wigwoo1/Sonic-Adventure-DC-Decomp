#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
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
/* Static C recovery of retail engine device initializer 8c651384..8c6513ee.
 * Seven subordinate initialization services remain external;
 *  Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_device_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c651384;
L8c651384: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c651386;
L8c651386: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c651388;
L8c651388: r[1]=rd(m,0x8c65149cu,4); /* mov.l 0x8c65149c,r1 */
 goto L8c65138a;
L8c65138a: target=r[1];*pr=0x8c65138eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c65138e;
L8c65138e: r[3]=rd(m,0x8c6514a0u,4); /* mov.l 0x8c6514a0,r3 */
 goto L8c651390;
L8c651390: target=r[3];*pr=0x8c651394u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c651394;
L8c651394: r[1]=rd(m,0x8c6514a4u,4); /* mov.l 0x8c6514a4,r1 */
 goto L8c651396;
L8c651396: target=r[1];*pr=0x8c65139au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c65139a;
L8c65139a: r[3]=rd(m,0x8c6514a8u,4); /* mov.l 0x8c6514a8,r3 */
 goto L8c65139c;
L8c65139c: target=r[3];*pr=0x8c6513a0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6513a0;
L8c6513a0: r[1]=rd(m,0x8c6514acu,4); /* mov.l 0x8c6514ac,r1 */
 goto L8c6513a2;
L8c6513a2: target=r[1];*pr=0x8c6513a6u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c6513a6;
L8c6513a6: r[3]=rd(m,0x8c6514b0u,4); /* mov.l 0x8c6514b0,r3 */
 goto L8c6513a8;
L8c6513a8: target=r[3];*pr=0x8c6513acu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6513ac;
L8c6513ac: r[0]=rd(m,0x8c6514b8u,4); /* mov.l 0x8c6514b8,r0 */
 goto L8c6513ae;
L8c6513ae: r[1]=rd(m,0x8c6514b4u,4); /* mov.l 0x8c6514b4,r1 */
 goto L8c6513b0;
L8c6513b0: wr(m,r[0],r[1],4); /* mov.l r1,@r0 */
 goto L8c6513b2;
L8c6513b2: r[3]=rd(m,0x8c6514bcu,4); /* mov.l 0x8c6514bc,r3 */
 goto L8c6513b4;
L8c6513b4: r[14]=0u; /* mov #0x0,r14 */
 goto L8c6513b6;
L8c6513b6: wr(m,r[3],r[14],4); /* mov.l r14,@r3 */
 goto L8c6513b8;
L8c6513b8: r[2]=rd(m,0x8c6514c0u,4); /* mov.l 0x8c6514c0,r2 */
 goto L8c6513ba;
L8c6513ba: wr(m,r[2],r[14],4); /* mov.l r14,@r2 */
 goto L8c6513bc;
L8c6513bc: r[1]=rd(m,0x8c6514c4u,4); /* mov.l 0x8c6514c4,r1 */
 goto L8c6513be;
L8c6513be: wr(m,r[1],r[14],4); /* mov.l r14,@r1 */
 goto L8c6513c0;
L8c6513c0: r[0]=rd(m,0x8c651484u,4); /* mov.l 0x8c651484,r0 */
 goto L8c6513c2;
L8c6513c2: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6513c4;
L8c6513c4: r[3]=rd(m,r[0],4); /* mov.l @r0,r3 */
 goto L8c6513c6;
L8c6513c6: r[7]=rd(m,0x8c651420u,4); /* mov.l 0x8c651420,r7 */
 goto L8c6513c8;
L8c6513c8: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c6513ca;
L8c6513ca: r[7]=rd(m,r[7]+0xcu,4); /* mov.l @(0xc,r7),r7 */
 goto L8c6513cc;
L8c6513cc: r[6]=rd(m,0x8c651420u,4); /* mov.l 0x8c651420,r6 */
 goto L8c6513ce;
L8c6513ce: r[3]=4294967289u; /* mov #-0x7,r3 */
 goto L8c6513d0;
L8c6513d0: r[7]=(r[3]&0x80000000u)?((r[3]&31u)?r[7]>>((0u-r[3])&31u):0u):(r[7]<<(r[3]&31u)); /* shld r3,r7 */
 goto L8c6513d2;
L8c6513d2: r[6]=rd(m,r[6]+0x8u,4); /* mov.l @(0x8,r6),r6 */
 goto L8c6513d4;
L8c6513d4: r[5]=rd(m,0x8c651420u,4); /* mov.l 0x8c651420,r5 */
 goto L8c6513d6;
L8c6513d6: r[6]=(r[3]&0x80000000u)?((r[3]&31u)?r[6]>>((0u-r[3])&31u):0u):(r[6]<<(r[3]&31u)); /* shld r3,r6 */
 goto L8c6513d8;
L8c6513d8: r[5]=rd(m,r[5]+0x4u,4); /* mov.l @(0x4,r5),r5 */
 goto L8c6513da;
L8c6513da: r[4]=rd(m,0x8c651420u,4); /* mov.l 0x8c651420,r4 */
 goto L8c6513dc;
L8c6513dc: r[5]=(r[3]&0x80000000u)?((r[3]&31u)?r[5]>>((0u-r[3])&31u):0u):(r[5]<<(r[3]&31u)); /* shld r3,r5 */
 goto L8c6513de;
L8c6513de: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c6513e0;
L8c6513e0: r[2]=rd(m,0x8c6514c8u,4); /* mov.l 0x8c6514c8,r2 */
 goto L8c6513e2;
L8c6513e2: target=r[2];*pr=0x8c6513e6u;r[4]=(r[3]&0x80000000u)?((r[3]&31u)?r[4]>>((0u-r[3])&31u):0u):(r[4]<<(r[3]&31u));status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6513e6;
L8c6513e6: r[15]+=8u; /* add #0x8,r15 */
 goto L8c6513e8;
L8c6513e8: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6513ea;
L8c6513ea: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
