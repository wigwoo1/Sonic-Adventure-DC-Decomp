/* Sonic Adventure USA Rev A recovered frame startup helper. */
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
/* Static C recovery of retail Tikal helper 8c65a2fc..8c65a33e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_frame_debug_log(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65a2fc;
L8c65a2fc: r[2]=rd(m,0x8c65a368u,4); /* mov.l 0x8c65a368,r2 */
 goto L8c65a2fe;
L8c65a2fe: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65a300;
L8c65a300: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c65a302;
L8c65a302: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65a304;
L8c65a304: if((*sr&1u))goto L8c65a338; /* bt 0x8c65a338 */
 goto L8c65a306;
L8c65a306: r[3]=rd(m,0x8c65a36cu,4); /* mov.l 0x8c65a36c,r3 */
 goto L8c65a308;
L8c65a308: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c65a30a;
L8c65a30a: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65a30c;
L8c65a30c: if(!(*sr&1u))goto L8c65a338; /* bf 0x8c65a338 */
 goto L8c65a30e;
L8c65a30e: r[0]=r[15]; /* mov r15,r0 */
 goto L8c65a310;
L8c65a310: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65a312;
L8c65a312: r[5]=3u; /* mov #0x3,r5 */
 goto L8c65a314;
L8c65a314: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65a316;
L8c65a316: *sr=(*sr&~1u)|!(r[0]&r[5]); /* tst r0,r5 */
 goto L8c65a318;
L8c65a318: if((*sr&1u))goto L8c65a322; /* bt 0x8c65a322 */
 goto L8c65a31a;
L8c65a31a: r[5]=r[15]; /* mov r15,r5 */
 goto L8c65a31c;
L8c65a31c: r[5]+=4u; /* add #0x4,r5 */
 goto L8c65a31e;
L8c65a31e: r[5]+=8u;goto L8c65a328; /* bra 0x8c65a328 */
 goto L8c65a322;
L8c65a322: r[5]=r[15]; /* mov r15,r5 */
 goto L8c65a324;
L8c65a324: r[5]+=4u; /* add #0x4,r5 */
 goto L8c65a326;
L8c65a326: r[5]+=4u; /* add #0x4,r5 */
 goto L8c65a328;
L8c65a328: r[3]=rd(m,0x8c65a374u,4); /* mov.l 0x8c65a374,r3 */
 goto L8c65a32a;
L8c65a32a: r[6]=r[5]; /* mov r5,r6 */
 goto L8c65a32c;
L8c65a32c: r[4]=rd(m,0x8c65a370u,4); /* mov.l 0x8c65a370,r4 */
 goto L8c65a32e;
L8c65a32e: target=r[3];*pr=0x8c65a332u;r[5]=rd(m,r[15]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65a332;
L8c65a332: r[4]=rd(m,0x8c65a370u,4); /* mov.l 0x8c65a370,r4 */
 goto L8c65a334;
L8c65a334: *pr=0x8c65a338u;status=call(ctx,0x8c65a33eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65a33e */
 goto L8c65a338;
L8c65a338: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65a33a;
L8c65a33a: r[0]=0u;return 0; /* rts */
}
