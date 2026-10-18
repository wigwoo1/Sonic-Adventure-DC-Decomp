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
/* Static C recovery of retail Tikal helper 8c6461ae..8c6461f0.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_status_publish(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6461ae;
L8c6461ae: r[3]=rd(m,0x8c6462b4u,4); /* mov.l 0x8c6462b4,r3 */
 goto L8c6461b0;
L8c6461b0: r[6]=0u; /* mov #0x0,r6 */
 goto L8c6461b2;
L8c6461b2: r[5]=rd(m,0x8c6462b0u,4); /* mov.l 0x8c6462b0,r5 */
 goto L8c6461b4;
L8c6461b4: r[7]=24u; /* mov #0x18,r7 */
 goto L8c6461b6;
L8c6461b6: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c6461b8;
L8c6461b8: r[4]+=40u; /* add #0x28,r4 */
 goto L8c6461ba;
L8c6461ba: r[2]=sx(m,r[4],1); /* mov.b @r4,r2 */
 goto L8c6461bc;
L8c6461bc: r[6]+=1u; /* add #0x1,r6 */
 goto L8c6461be;
L8c6461be: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]); /* cmp/ge r7,r6 */
 goto L8c6461c0;
L8c6461c0: wr(m,r[5],r[2],1); /* mov.b r2,@r5 */
 goto L8c6461c2;
L8c6461c2: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c6461c4;
L8c6461c4: wr(m,r[5]+0x1u,r[0],1); /* mov.b r0,@(0x1,r5) */
 goto L8c6461c6;
L8c6461c6: r[0]=sx(m,r[4]+0x2u,1); /* mov.b @(0x2,r4),r0 */
 goto L8c6461c8;
L8c6461c8: wr(m,r[5]+0x2u,r[0],1); /* mov.b r0,@(0x2,r5) */
 goto L8c6461ca;
L8c6461ca: r[0]=sx(m,r[4]+0x3u,1); /* mov.b @(0x3,r4),r0 */
 goto L8c6461cc;
L8c6461cc: wr(m,r[5]+0x3u,r[0],1); /* mov.b r0,@(0x3,r5) */
 goto L8c6461ce;
L8c6461ce: r[3]=rd(m,r[4]+0x10u,4); /* mov.l @(0x10,r4),r3 */
 goto L8c6461d0;
L8c6461d0: wr(m,r[5]+0x8u,r[3],4); /* mov.l r3,@(0x8,r5) */
 goto L8c6461d2;
L8c6461d2: r[0]=sx(m,r[4]+0x8u,1); /* mov.b @(0x8,r4),r0 */
 goto L8c6461d4;
L8c6461d4: wr(m,r[5]+0x4u,r[0],1); /* mov.b r0,@(0x4,r5) */
 goto L8c6461d6;
L8c6461d6: r[0]=sx(m,r[4]+0x9u,1); /* mov.b @(0x9,r4),r0 */
 goto L8c6461d8;
L8c6461d8: wr(m,r[5]+0x5u,r[0],1); /* mov.b r0,@(0x5,r5) */
 goto L8c6461da;
L8c6461da: r[0]=sx(m,r[4]+0xau,1); /* mov.b @(0xa,r4),r0 */
 goto L8c6461dc;
L8c6461dc: wr(m,r[5]+0x6u,r[0],1); /* mov.b r0,@(0x6,r5) */
 goto L8c6461de;
L8c6461de: r[0]=sx(m,r[4]+0xbu,1); /* mov.b @(0xb,r4),r0 */
 goto L8c6461e0;
L8c6461e0: wr(m,r[5]+0x7u,r[0],1); /* mov.b r0,@(0x7,r5) */
 goto L8c6461e2;
L8c6461e2: r[3]=rd(m,r[4]+0x14u,4); /* mov.l @(0x14,r4),r3 */
 goto L8c6461e4;
L8c6461e4: wr(m,r[5]+0xcu,r[3],4); /* mov.l r3,@(0xc,r5) */
 goto L8c6461e6;
L8c6461e6: r[5]+=24u; /* add #0x18,r5 */
 goto L8c6461e8;
L8c6461e8: take=!(*sr&1u);r[4]+=44u;if(take)goto L8c6461ba;goto L8c6461ec; /* bf/s 0x8c6461ba */
 goto L8c6461ec;
L8c6461ec: return 0; /* rts */
}
