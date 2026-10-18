/* Sonic Adventure USA Rev A frame context initializer, 0x8c65acc0. */
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
/* Static C recovery of retail Tikal helper 8c65acc0..8c65ad26.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_frame_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65acc0;
L8c65acc0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65acc2;
L8c65acc2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65acc4;
L8c65acc4: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c65acc6;
L8c65acc6: r[3]=rd(m,0x8c65ad30u,4); /* mov.l 0x8c65ad30,r3 */
 goto L8c65acc8;
L8c65acc8: r[4]=4294967264u; /* mov #-0x20,r4 */
 goto L8c65acca;
L8c65acca: r[14]=rd(m,0x8c65ad2cu,4); /* mov.l 0x8c65ad2c,r14 */
 goto L8c65accc;
L8c65accc: r[3]&=r[4]; /* and r4,r3 */
 goto L8c65acce;
L8c65acce: r[2]=sx(m,0x8c65ad26u,2); /* mov.w 0x8c65ad26,r2 */
 goto L8c65acd0;
L8c65acd0: wr(m,r[14],r[3],4); /* mov.l r3,@r14 */
 goto L8c65acd2;
L8c65acd2: r[3]=rd(m,0x8c65ad34u,4); /* mov.l 0x8c65ad34,r3 */
 goto L8c65acd4;
L8c65acd4: wr(m,r[15]+0x4u,r[2],4); /* mov.l r2,@(0x4,r15) */
 goto L8c65acd6;
L8c65acd6: r[4]&=r[3]; /* and r3,r4 */
 goto L8c65acd8;
L8c65acd8: r[2]=rd(m,0x8c65ad3cu,4); /* mov.l 0x8c65ad3c,r2 */
 goto L8c65acda;
L8c65acda: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c65acdc;
L8c65acdc: r[4]=rd(m,0x8c65ad38u,4); /* mov.l 0x8c65ad38,r4 */
 goto L8c65acde;
L8c65acde: target=r[2];*pr=0x8c65ace2u;r[5]=64u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65ace2;
L8c65ace2: r[3]=rd(m,0x8c65ad44u,4); /* mov.l 0x8c65ad44,r3 */
 goto L8c65ace4;
L8c65ace4: r[4]=rd(m,0x8c65ad40u,4); /* mov.l 0x8c65ad40,r4 */
 goto L8c65ace6;
L8c65ace6: target=r[3];*pr=0x8c65aceau;r[5]=64u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65acea;
L8c65acea: r[3]=rd(m,0x8c65ad48u,4); /* mov.l 0x8c65ad48,r3 */
 goto L8c65acec;
L8c65acec: target=r[3];*pr=0x8c65acf0u;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65acf0;
L8c65acf0: r[3]=rd(m,0x8c65ad50u,4); /* mov.l 0x8c65ad50,r3 */
 goto L8c65acf2;
L8c65acf2: r[4]=rd(m,0x8c65ad4cu,4); /* mov.l 0x8c65ad4c,r4 */
 goto L8c65acf4;
L8c65acf4: target=r[3];*pr=0x8c65acf8u;r[5]=rd(m,r[15]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65acf8;
L8c65acf8: r[2]=rd(m,0x8c65ad54u,4); /* mov.l 0x8c65ad54,r2 */
 goto L8c65acfa;
L8c65acfa: target=r[2];*pr=0x8c65acfeu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65acfe;
L8c65acfe: r[0]=sx(m,0x8c65ad28u,2); /* mov.w 0x8c65ad28,r0 */
 goto L8c65ad00;
L8c65ad00: r[2]=0u; /* mov #0x0,r2 */
 goto L8c65ad02;
L8c65ad02: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ad04;
L8c65ad04: r[5]=r[2]; /* mov r2,r5 */
 goto L8c65ad06;
L8c65ad06: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65ad08;
L8c65ad08: r[2]=rd(m,0x8c65ad58u,4); /* mov.l 0x8c65ad58,r2 */
 goto L8c65ad0a;
L8c65ad0a: r[3]=sx(m,0x8c65ad2au,2); /* mov.w 0x8c65ad2a,r3 */
 goto L8c65ad0c;
L8c65ad0c: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c65ad0e;
L8c65ad0e: target=r[2];*pr=0x8c65ad12u;r[4]+=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65ad12;
L8c65ad12: r[0]=sx(m,0x8c65ad2au,2); /* mov.w 0x8c65ad2a,r0 */
 goto L8c65ad14;
L8c65ad14: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65ad16;
L8c65ad16: r[1]=rd(m,0x8c65ad5cu,4); /* mov.l 0x8c65ad5c,r1 */
 goto L8c65ad18;
L8c65ad18: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65ad1a;
L8c65ad1a: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65ad1c;
L8c65ad1c: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c65ad1e;
L8c65ad1e: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65ad20;
L8c65ad20: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65ad22;
L8c65ad22: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
