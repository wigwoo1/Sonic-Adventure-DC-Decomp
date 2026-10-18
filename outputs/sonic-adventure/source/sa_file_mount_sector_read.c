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
/* Static C recovery of retail Tikal helper 8c5ffc98..8c5ffcd6.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_mount_sector_read(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ffc98;
L8c5ffc98: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c5ffc9a;
L8c5ffc9a: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c5ffc9c;
L8c5ffc9c: r[3]=rd(m,0x8c5ffd18u,4); /* mov.l 0x8c5ffd18,r3 */
 goto L8c5ffc9e;
L8c5ffc9e: wr(m,r[15]+0x8u,r[4],4); /* mov.l r4,@(0x8,r15) */
 goto L8c5ffca0;
L8c5ffca0: r[2]=rd(m,0x8c5ffd1cu,4); /* mov.l 0x8c5ffd1c,r2 */
 goto L8c5ffca2;
L8c5ffca2: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c5ffca4;
L8c5ffca4: r[5]=0u; /* mov #0x0,r5 */
 goto L8c5ffca6;
L8c5ffca6: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c5ffca8;
L8c5ffca8: r[7]=rd(m,r[15],4); /* mov.l @r15,r7 */
 goto L8c5ffcaa;
L8c5ffcaa: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c5ffcac;
L8c5ffcac: r[7]<<=8; /* shll8 r7 */
 goto L8c5ffcae;
L8c5ffcae: r[6]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r6 */
 goto L8c5ffcb0;
L8c5ffcb0: r[7]<<=2; /* shll2 r7 */
 goto L8c5ffcb2;
L8c5ffcb2: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1; /* shll r7 */
 goto L8c5ffcb4;
L8c5ffcb4: target=r[2];*pr=0x8c5ffcb8u;r[4]=rd(m,r[4]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c5ffcb8;
L8c5ffcb8: r[4]=r[0]; /* mov r0,r4 */
 goto L8c5ffcba;
L8c5ffcba: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c5ffcbc;
L8c5ffcbc: if(!(*sr&1u))goto L8c5ffcc6; /* bf 0x8c5ffcc6 */
 goto L8c5ffcbe;
L8c5ffcbe: r[15]+=12u; /* add #0xc,r15 */
 goto L8c5ffcc0;
L8c5ffcc0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c5ffcc2;
L8c5ffcc2: r[0]=4294967283u;return 0; /* rts */
L8c5ffcc6: r[3]=rd(m,0x8c5ffd20u,4); /* mov.l 0x8c5ffd20,r3 */
 goto L8c5ffcc8;
L8c5ffcc8: r[6]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r6 */
 goto L8c5ffcca;
L8c5ffcca: target=r[3];*pr=0x8c5ffcceu;r[5]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c5ffcce;
L8c5ffcce: r[15]+=12u; /* add #0xc,r15 */
 goto L8c5ffcd0;
L8c5ffcd0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c5ffcd2;
L8c5ffcd2: return 0; /* rts */
}
