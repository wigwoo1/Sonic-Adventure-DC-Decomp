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
/* Static C recovery of retail Tikal helper 8c65f7cc..8c65f810.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_queue_flush(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f7cc;
L8c65f7cc: r[5]=rd(m,0x8c65f850u,4); /* mov.l 0x8c65f850,r5 */
 goto L8c65f7ce;
L8c65f7ce: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c65f7d0;
L8c65f7d0: r[0]=sx(m,0x8c65f84au,2); /* mov.w 0x8c65f84a,r0 */
 goto L8c65f7d2;
L8c65f7d2: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c65f7d4;
L8c65f7d4: r[1]=r[0]; /* mov r0,r1 */
 goto L8c65f7d6;
L8c65f7d6: r[1]+=12u; /* add #0xc,r1 */
 goto L8c65f7d8;
L8c65f7d8: r[2]=r[3]; /* mov r3,r2 */
 goto L8c65f7da;
L8c65f7da: r[1]+=r[2]; /* add r2,r1 */
 goto L8c65f7dc;
L8c65f7dc: r[1]=rd(m,r[1],4); /* mov.l @r1,r1 */
 goto L8c65f7de;
L8c65f7de: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f7e0;
L8c65f7e0: r[1]<<=2; /* shll2 r1 */
 goto L8c65f7e2;
L8c65f7e2: r[7]=sx(m,0x8c65f84cu,2); /* mov.w 0x8c65f84c,r7 */
 goto L8c65f7e4;
L8c65f7e4: r[2]+=r[1]; /* add r1,r2 */
 goto L8c65f7e6;
L8c65f7e6: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f7e8;
L8c65f7e8: r[1]=rd(m,r[5],4); /* mov.l @r5,r1 */
 goto L8c65f7ea;
L8c65f7ea: r[1]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r1 */
 goto L8c65f7ec;
L8c65f7ec: take=!(*sr&1u);r[6]=0u;if(take)goto L8c65f7fa;goto L8c65f7f0; /* bf/s 0x8c65f7fa */
 goto L8c65f7f0;
L8c65f7f0: r[6]+=1u; /* add #0x1,r6 */
 goto L8c65f7f2;
L8c65f7f2: wr(m,r[1],r[7],4); /* mov.l r7,@r1 */
 goto L8c65f7f4;
L8c65f7f4: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[4]); /* cmp/ge r4,r6 */
 goto L8c65f7f6;
L8c65f7f6: take=!(*sr&1u);r[1]+=4u;if(take)goto L8c65f7f0;goto L8c65f7fa; /* bf/s 0x8c65f7f0 */
 goto L8c65f7fa;
L8c65f7fa: r[0]=sx(m,0x8c65f84eu,2); /* mov.w 0x8c65f84e,r0 */
 goto L8c65f7fc;
L8c65f7fc: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c65f7fe;
L8c65f7fe: wr(m,r[2]+r[0],r[4],4); /* mov.l r4,@(r0,r2) */
 goto L8c65f800;
L8c65f800: r[0]+=4u; /* add #0x4,r0 */
 goto L8c65f802;
L8c65f802: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c65f804;
L8c65f804: r[4]<<=2; /* shll2 r4 */
 goto L8c65f806;
L8c65f806: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f808;
L8c65f808: r[2]+=r[4]; /* add r4,r2 */
 goto L8c65f80a;
L8c65f80a: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c65f80c;
L8c65f80c: r[0]=0u;return 0; /* rts */
}
