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
/* Static C recovery of retail Tikal helper 8c07d5aa..8c07d5e8.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_position_task_create(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c07d5aa;
L8c07d5aa: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c07d5ac;
L8c07d5ac: r[14]=r[4]; /* mov r4,r14 */
 goto L8c07d5ae;
L8c07d5ae: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c07d5b0;
L8c07d5b0: r[3]=1u; /* mov #0x1,r3 */
 goto L8c07d5b2;
L8c07d5b2: r[4]=r[14]&255u; /* extu.b r14,r4 */
 goto L8c07d5b4;
L8c07d5b4: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]); /* cmp/ge r3,r4 */
 goto L8c07d5b6;
L8c07d5b6: if(!(*sr&1u))goto L8c07d5cc; /* bf 0x8c07d5cc */
 goto L8c07d5b8;
L8c07d5b8: r[1]=7u; /* mov #0x7,r1 */
 goto L8c07d5ba;
L8c07d5ba: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[1]); /* cmp/gt r1,r4 */
 goto L8c07d5bc;
L8c07d5bc: if(!(*sr&1u))goto L8c07d5cc; /* bf 0x8c07d5cc */
 goto L8c07d5be;
L8c07d5be: r[2]=rd(m,0x8c07d6acu,4); /* mov.l 0x8c07d6ac,r2 */
 goto L8c07d5c0;
L8c07d5c0: r[0]=r[14]&255u; /* extu.b r14,r0 */
 goto L8c07d5c2;
L8c07d5c2: r[0]<<=2; /* shll2 r0 */
 goto L8c07d5c4;
L8c07d5c4: r[2]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r2 */
 goto L8c07d5c6;
L8c07d5c6: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c07d5c8;
L8c07d5c8: if(!(*sr&1u))goto L8c07d5cc; /* bf 0x8c07d5cc */
 goto L8c07d5ca;
L8c07d5ca: r[14]=0u; /* mov #0x0,r14 */
 goto L8c07d5cc;
L8c07d5cc: r[3]=rd(m,0x8c07d6b4u,4); /* mov.l 0x8c07d6b4,r3 */
 goto L8c07d5ce;
L8c07d5ce: r[5]=0u; /* mov #0x0,r5 */
 goto L8c07d5d0;
L8c07d5d0: r[6]=rd(m,0x8c07d6b0u,4); /* mov.l 0x8c07d6b0,r6 */
 goto L8c07d5d2;
L8c07d5d2: target=r[3];*pr=0x8c07d5d6u;r[4]=2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c07d5d6;
L8c07d5d6: r[4]=r[0]; /* mov r0,r4 */
 goto L8c07d5d8;
L8c07d5d8: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c07d5da;
L8c07d5da: if((*sr&1u))goto L8c07d5e2; /* bt 0x8c07d5e2 */
 goto L8c07d5dc;
L8c07d5dc: r[3]=rd(m,r[4]+0x20u,4); /* mov.l @(0x20,r4),r3 */
 goto L8c07d5de;
L8c07d5de: r[0]=r[14]; /* mov r14,r0 */
 goto L8c07d5e0;
L8c07d5e0: wr(m,r[3]+0x8u,r[0],1); /* mov.b r0,@(0x8,r3) */
 goto L8c07d5e2;
L8c07d5e2: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c07d5e4;
L8c07d5e4: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
