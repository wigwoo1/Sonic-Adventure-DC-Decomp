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
/* Static C recovery of retail engine mode initializer 8c65262c..8c6526bc.
 * Engine initialization, option selection and device setup remain external services;
 *  Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_mode_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65262c;
L8c65262c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65262e;
L8c65262e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c652630;
L8c652630: r[3]=rd(m,0x8c652694u,4); /* mov.l 0x8c652694,r3 */
 goto L8c652632;
L8c652632: target=r[3];*pr=0x8c652636u;r[14]=r[4];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c652636;
L8c652636: r[2]=2u; /* mov #0x2,r2 */
 goto L8c652638;
L8c652638: *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[2]); /* cmp/gt r2,r14 */
 goto L8c65263a;
L8c65263a: if((*sr&1u))goto L8c652640; /* bt 0x8c652640 */
 goto L8c65263c;
L8c65263c: *sr=(*sr&~1u)|((int32_t)r[14]>=0); /* cmp/pz r14 */
 goto L8c65263e;
L8c65263e: if((*sr&1u))goto L8c6526a0; /* bt 0x8c6526a0 */
 goto L8c652640;
L8c652640: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c652642;
L8c652642: r[0]=2u; /* mov #0x2,r0 */
 goto L8c652644;
L8c652644: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c6526a0: r[4]=rd(m,0x8c652744u,4); /* mov.l 0x8c652744,r4 */
 goto L8c6526a2;
L8c6526a2: r[6]=3u; /* mov #0x3,r6 */
 goto L8c6526a4;
L8c6526a4: r[3]=rd(m,0x8c652748u,4); /* mov.l 0x8c652748,r3 */
 goto L8c6526a6;
L8c6526a6: r[5]=rd(m,0x8c652740u,4); /* mov.l 0x8c652740,r5 */
 goto L8c6526a8;
L8c6526a8: target=r[3];*pr=0x8c6526acu;r[6]&=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6526ac;
L8c6526ac: r[3]=rd(m,0x8c652750u,4); /* mov.l 0x8c652750,r3 */
 goto L8c6526ae;
L8c6526ae: r[2]=rd(m,0x8c65274cu,4); /* mov.l 0x8c65274c,r2 */
 goto L8c6526b0;
L8c6526b0: target=r[3];*pr=0x8c6526b4u;wr(m,r[2],r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6526b4;
L8c6526b4: r[0]=0u; /* mov #0x0,r0 */
 goto L8c6526b6;
L8c6526b6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6526b8;
L8c6526b8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
