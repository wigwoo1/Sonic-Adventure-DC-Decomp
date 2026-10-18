#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_cache_invalidate(void *,uint32_t);
int sa_sh4_cache_purge(void *,uint32_t);
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
/* Static C recovery of retail Tikal helper 8c6685bc..8c6685fc.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_state_query(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6685bc;
L8c6685bc: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6685be;
L8c6685be: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6685c0;
L8c6685c0: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c6685c2;
L8c6685c2: r[0]=r[5]; /* mov r5,r0 */
 goto L8c6685c4;
L8c6685c4: wr(m,r[15]+0xcu,r[4],4); /* mov.l r4,@(0xc,r15) */
 goto L8c6685c6;
L8c6685c6: wr(m,r[15]+0x8u,r[0],1); /* mov.b r0,@(0x8,r15) */
 goto L8c6685c8;
L8c6685c8: r[5]=r[15]; /* mov r15,r5 */
 goto L8c6685ca;
L8c6685ca: r[2]=rd(m,0x8c6686f0u,4); /* mov.l 0x8c6686f0,r2 */
 goto L8c6685cc;
L8c6685cc: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c6685ce;
L8c6685ce: r[0]=sx(m,r[15]+0x8u,1); /* mov.b @(0x8,r15),r0 */
 goto L8c6685d0;
L8c6685d0: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c6685d2;
L8c6685d2: r[4]=r[0]; /* mov r0,r4 */
 goto L8c6685d4;
L8c6685d4: r[4]<<=2; /* shll2 r4 */
 goto L8c6685d6;
L8c6685d6: r[3]+=24u; /* add #0x18,r3 */
 goto L8c6685d8;
L8c6685d8: r[4]+=r[3]; /* add r3,r4 */
 goto L8c6685da;
L8c6685da: target=r[2];*pr=0x8c6685deu;r[4]=rd(m,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6685de;
L8c6685de: r[14]=r[0]; /* mov r0,r14 */
 goto L8c6685e0;
L8c6685e0: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c6685e2;
L8c6685e2: if((*sr&1u))goto L8c6685ec; /* bt 0x8c6685ec */
 goto L8c6685e4;
L8c6685e4: r[2]=rd(m,0x8c6686c0u,4); /* mov.l 0x8c6686c0,r2 */
 goto L8c6685e6;
L8c6685e6: r[4]=rd(m,0x8c6686f4u,4); /* mov.l 0x8c6686f4,r4 */
 goto L8c6685e8;
L8c6685e8: target=r[2];*pr=0x8c6685ecu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6685ec;
L8c6685ec: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c6685ee;
L8c6685ee: r[0]=r[14]; /* mov r14,r0 */
 goto L8c6685f0;
L8c6685f0: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c6685f2;
L8c6685f2: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c6685f4;
L8c6685f4: r[15]+=16u; /* add #0x10,r15 */
 goto L8c6685f6;
L8c6685f6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6685f8;
L8c6685f8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
