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
/* Static C recovery of retail Tikal helper 8c6604e6..8c660518.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_request_lock(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6604e6;
L8c6604e6: r[3]=rd(m,0x8c660610u,4); /* mov.l 0x8c660610,r3 */
 goto L8c6604e8;
L8c6604e8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6604ea;
L8c6604ea: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c6604ec;
L8c6604ec: {uint32_t value=rd(m,r[0],1);*sr=(*sr&~1u)|(value==0);wr(m,r[0],value|0x80u,1);} /* tas.b @r0 */
 goto L8c6604ee;
L8c6604ee: if((*sr&1u))goto L8c660500; /* bt 0x8c660500 */
 goto L8c6604f0;
L8c6604f0: r[2]=rd(m,0x8c660618u,4); /* mov.l 0x8c660618,r2 */
 goto L8c6604f2;
L8c6604f2: r[1]=rd(m,0x8c660614u,4); /* mov.l 0x8c660614,r1 */
 goto L8c6604f4;
L8c6604f4: target=r[2];*pr=0x8c6604f8u;r[15]-=4;wr(m,r[15],r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6604f8;
L8c6604f8: r[15]+=4u; /* add #0x4,r15 */
 goto L8c6604fa;
L8c6604fa: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6604fc;
L8c6604fc: r[0]=4294967294u;return 0; /* rts */
L8c660500: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c660502;
L8c660502: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c660504;
L8c660504: *pr=0x8c660508u;status=call(ctx,0x8c660518u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c660518 */
 goto L8c660508;
L8c660508: r[15]+=4u; /* add #0x4,r15 */
 goto L8c66050a;
L8c66050a: r[3]=rd(m,0x8c660610u,4); /* mov.l 0x8c660610,r3 */
 goto L8c66050c;
L8c66050c: r[1]=0u; /* mov #0x0,r1 */
 goto L8c66050e;
L8c66050e: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c660510;
L8c660510: wr(m,r[2],r[1],1); /* mov.b r1,@r2 */
 goto L8c660512;
L8c660512: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c660514;
L8c660514: return 0; /* rts */
}
