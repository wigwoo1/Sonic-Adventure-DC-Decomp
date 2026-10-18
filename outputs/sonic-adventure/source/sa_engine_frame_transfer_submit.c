/* Sonic Adventure USA Rev A main executable recovered transfer routine. */
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
/* Static C recovery of retail Tikal helper 8c669088..8c6690f4.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_frame_transfer_submit(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c669088;
L8c669088: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66908a;
L8c66908a: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66908c;
L8c66908c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66908e;
L8c66908e: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c669090;
L8c669090: r[3]=3u; /* mov #0x3,r3 */
 goto L8c669092;
L8c669092: r[13]=r[7]; /* mov r7,r13 */
 goto L8c669094;
L8c669094: *sr=(*sr&~1u)|!(r[4]&r[3]); /* tst r4,r3 */
 goto L8c669096;
L8c669096: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c669098;
L8c669098: wr(m,r[15],r[6],4); /* mov.l r6,@r15 */
 goto L8c66909a;
L8c66909a: take=!(*sr&1u);r[14]=0u;if(take)goto L8c6690a4;goto L8c66909e; /* bf/s 0x8c6690a4 */
 goto L8c66909e;
L8c66909e: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c6690a0;
L8c6690a0: *sr=(*sr&~1u)|!(r[0]&0x3u); /* tst #0x3,r0 */
 goto L8c6690a2;
L8c6690a2: if((*sr&1u))goto L8c6690aa; /* bt 0x8c6690aa */
 goto L8c6690a4;
L8c6690a4: r[14]=rd(m,0x8c6690f4u,4); /* mov.l 0x8c6690f4,r14 */
 goto L8c6690a6;
L8c6690a6: goto L8c6690dc; /* bra 0x8c6690dc */
 goto L8c6690aa;
L8c6690aa: r[3]=1u; /* mov #0x1,r3 */
 goto L8c6690ac;
L8c6690ac: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c6690ae;
L8c6690ae: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c6690b0;
L8c6690b0: r[7]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r7 */
 goto L8c6690b2;
L8c6690b2: *sr=(*sr&~1u)|((int32_t)r[7]>=0); /* cmp/pz r7 */
 goto L8c6690b4;
L8c6690b4: if((*sr&1u))goto L8c6690b8; /* bt 0x8c6690b8 */
 goto L8c6690b6;
L8c6690b6: r[7]+=3u; /* add #0x3,r7 */
 goto L8c6690b8;
L8c6690b8: r[2]=rd(m,0x8c6690f8u,4); /* mov.l 0x8c6690f8,r2 */
 goto L8c6690ba;
L8c6690ba: *sr=(*sr&~1u)|(r[7]&1u);r[7]=(r[7]>>1)|(r[7]&0x80000000u); /* shar r7 */
 goto L8c6690bc;
L8c6690bc: r[5]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r5 */
 goto L8c6690be;
L8c6690be: *sr=(*sr&~1u)|(r[7]&1u);r[7]=(r[7]>>1)|(r[7]&0x80000000u); /* shar r7 */
 goto L8c6690c0;
L8c6690c0: r[5]+=r[2]; /* add r2,r5 */
 goto L8c6690c2;
L8c6690c2: r[2]=rd(m,0x8c6690fcu,4); /* mov.l 0x8c6690fc,r2 */
 goto L8c6690c4;
L8c6690c4: target=r[2];*pr=0x8c6690c8u;r[6]=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6690c8;
L8c6690c8: r[15]+=8u; /* add #0x8,r15 */
 goto L8c6690ca;
L8c6690ca: r[3]=rd(m,0x8c669100u,4); /* mov.l 0x8c669100,r3 */
 goto L8c6690cc;
L8c6690cc: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c6690ce;
L8c6690ce: take=(*sr&1u);wr(m,r[3],r[0],4);if(take)goto L8c6690dc;goto L8c6690d2; /* bt/s 0x8c6690dc */
 goto L8c6690d2;
L8c6690d2: r[0]=r[13]; /* mov r13,r0 */
 goto L8c6690d4;
L8c6690d4: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #-0x1,r0 */
 goto L8c6690d6;
L8c6690d6: if((*sr&1u))goto L8c6690dc; /* bt 0x8c6690dc */
 goto L8c6690d8;
L8c6690d8: target=r[13];*pr=0x8c6690dcu;r[4]=rd(m,r[15]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c6690dc;
L8c6690dc: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c6690de;
L8c6690de: if((*sr&1u))goto L8c6690e8; /* bt 0x8c6690e8 */
 goto L8c6690e0;
L8c6690e0: r[2]=rd(m,0x8c669108u,4); /* mov.l 0x8c669108,r2 */
 goto L8c6690e2;
L8c6690e2: r[4]=rd(m,0x8c669104u,4); /* mov.l 0x8c669104,r4 */
 goto L8c6690e4;
L8c6690e4: target=r[2];*pr=0x8c6690e8u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6690e8;
L8c6690e8: r[0]=r[14]; /* mov r14,r0 */
 goto L8c6690ea;
L8c6690ea: r[15]+=8u; /* add #0x8,r15 */
 goto L8c6690ec;
L8c6690ec: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6690ee;
L8c6690ee: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6690f0;
L8c6690f0: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
