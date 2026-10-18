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
/* Static C recovery of retail Tikal helper 8c660494..8c6604c0.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_request_simple_lock(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c660494;
L8c660494: r[3]=rd(m,0x8c660610u,4); /* mov.l 0x8c660610,r3 */
 goto L8c660496;
L8c660496: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c660498;
L8c660498: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c66049a;
L8c66049a: {uint32_t value=rd(m,r[0],1);*sr=(*sr&~1u)|(value==0);wr(m,r[0],value|0x80u,1);} /* tas.b @r0 */
 goto L8c66049c;
L8c66049c: if((*sr&1u))goto L8c6604ae; /* bt 0x8c6604ae */
 goto L8c66049e;
L8c66049e: r[2]=rd(m,0x8c660618u,4); /* mov.l 0x8c660618,r2 */
 goto L8c6604a0;
L8c6604a0: r[1]=rd(m,0x8c660614u,4); /* mov.l 0x8c660614,r1 */
 goto L8c6604a2;
L8c6604a2: target=r[2];*pr=0x8c6604a6u;r[15]-=4;wr(m,r[15],r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6604a6;
L8c6604a6: r[15]+=4u; /* add #0x4,r15 */
 goto L8c6604a8;
L8c6604a8: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6604aa;
L8c6604aa: r[0]=4294967294u;return 0; /* rts */
L8c6604ae: *pr=0x8c6604b2u;status=call(ctx,0x8c6604c0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6604c0 */
 goto L8c6604b2;
L8c6604b2: r[3]=rd(m,0x8c660610u,4); /* mov.l 0x8c660610,r3 */
 goto L8c6604b4;
L8c6604b4: r[1]=0u; /* mov #0x0,r1 */
 goto L8c6604b6;
L8c6604b6: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c6604b8;
L8c6604b8: wr(m,r[2],r[1],1); /* mov.b r1,@r2 */
 goto L8c6604ba;
L8c6604ba: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6604bc;
L8c6604bc: return 0; /* rts */
}
