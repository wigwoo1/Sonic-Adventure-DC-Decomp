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
/* Static C recovery of retail Tikal helper 8c669014..8c66906e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_buffer_read(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c669014;
L8c669014: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c669016;
L8c669016: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c669018;
L8c669018: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c66901a;
L8c66901a: r[3]=3u; /* mov #0x3,r3 */
 goto L8c66901c;
L8c66901c: wr(m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c66901e;
L8c66901e: *sr=(*sr&~1u)|!(r[5]&r[3]); /* tst r5,r3 */
 goto L8c669020;
L8c669020: wr(m,r[15],r[6],4); /* mov.l r6,@r15 */
 goto L8c669022;
L8c669022: take=!(*sr&1u);r[14]=0u;if(take)goto L8c66902c;goto L8c669026; /* bf/s 0x8c66902c */
 goto L8c669026;
L8c669026: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c669028;
L8c669028: *sr=(*sr&~1u)|!(r[0]&0x3u); /* tst #0x3,r0 */
 goto L8c66902a;
L8c66902a: if((*sr&1u))goto L8c669032; /* bt 0x8c669032 */
 goto L8c66902c;
L8c66902c: r[14]=rd(m,0x8c669070u,4); /* mov.l 0x8c669070,r14 */
 goto L8c66902e;
L8c66902e: goto L8c669058; /* bra 0x8c669058 */
 goto L8c669032;
L8c669032: r[3]=1u; /* mov #0x1,r3 */
 goto L8c669034;
L8c669034: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c669036;
L8c669036: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c669038;
L8c669038: r[7]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r7 */
 goto L8c66903a;
L8c66903a: r[7]+=3u; /* add #0x3,r7 */
 goto L8c66903c;
L8c66903c: *sr=(*sr&~1u)|((int32_t)r[7]>=0); /* cmp/pz r7 */
 goto L8c66903e;
L8c66903e: if((*sr&1u))goto L8c669042; /* bt 0x8c669042 */
 goto L8c669040;
L8c669040: r[7]+=3u; /* add #0x3,r7 */
 goto L8c669042;
L8c669042: r[2]=rd(m,0x8c669074u,4); /* mov.l 0x8c669074,r2 */
 goto L8c669044;
L8c669044: *sr=(*sr&~1u)|(r[7]&1u);r[7]=(r[7]>>1)|(r[7]&0x80000000u); /* shar r7 */
 goto L8c669046;
L8c669046: r[4]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r4 */
 goto L8c669048;
L8c669048: *sr=(*sr&~1u)|(r[7]&1u);r[7]=(r[7]>>1)|(r[7]&0x80000000u); /* shar r7 */
 goto L8c66904a;
L8c66904a: r[4]+=r[2]; /* add r2,r4 */
 goto L8c66904c;
L8c66904c: r[2]=rd(m,0x8c669078u,4); /* mov.l 0x8c669078,r2 */
 goto L8c66904e;
L8c66904e: target=r[2];*pr=0x8c669052u;r[6]=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c669052;
L8c669052: r[15]+=8u; /* add #0x8,r15 */
 goto L8c669054;
L8c669054: r[3]=rd(m,0x8c66907cu,4); /* mov.l 0x8c66907c,r3 */
 goto L8c669056;
L8c669056: wr(m,r[3],r[0],4); /* mov.l r0,@r3 */
 goto L8c669058;
L8c669058: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66905a;
L8c66905a: if((*sr&1u))goto L8c669064; /* bt 0x8c669064 */
 goto L8c66905c;
L8c66905c: r[3]=rd(m,0x8c669084u,4); /* mov.l 0x8c669084,r3 */
 goto L8c66905e;
L8c66905e: r[4]=rd(m,0x8c669080u,4); /* mov.l 0x8c669080,r4 */
 goto L8c669060;
L8c669060: target=r[3];*pr=0x8c669064u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c669064;
L8c669064: r[0]=r[14]; /* mov r14,r0 */
 goto L8c669066;
L8c669066: r[15]+=8u; /* add #0x8,r15 */
 goto L8c669068;
L8c669068: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66906a;
L8c66906a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
