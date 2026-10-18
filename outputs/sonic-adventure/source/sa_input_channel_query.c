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
/* Static C recovery of retail Tikal helper 8c660dac..8c660dec.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_channel_query(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c660dac;
L8c660dac: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c660dae;
L8c660dae: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c660db0;
L8c660db0: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c660db2;
L8c660db2: r[0]=rd(m,0x8c660e3cu,4); /* mov.l 0x8c660e3c,r0 */
 goto L8c660db4;
L8c660db4: r[14]=r[4]; /* mov r4,r14 */
 goto L8c660db6;
L8c660db6: r[14]<<=2; /* shll2 r14 */
 goto L8c660db8;
L8c660db8: r[3]=rd(m,0x8c660e40u,4); /* mov.l 0x8c660e40,r3 */
 goto L8c660dba;
L8c660dba: r[14]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r14 */
 goto L8c660dbc;
L8c660dbc: target=r[3];*pr=0x8c660dc0u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660dc0;
L8c660dc0: r[3]=rd(m,0x8c660e44u,4); /* mov.l 0x8c660e44,r3 */
 goto L8c660dc2;
L8c660dc2: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c660dc4;
L8c660dc4: target=r[3];*pr=0x8c660dc8u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660dc8;
L8c660dc8: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c660dca;
L8c660dca: r[4]=r[0]; /* mov r0,r4 */
 goto L8c660dcc;
L8c660dcc: r[3]=sx(m,r[2],1); /* mov.b @r2,r3 */
 goto L8c660dce;
L8c660dce: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c660dd0;
L8c660dd0: if((*sr&1u))goto L8c660dd8; /* bt 0x8c660dd8 */
 goto L8c660dd2;
L8c660dd2: r[0]=rd(m,r[4],4); /* mov.l @r4,r0 */
 goto L8c660dd4;
L8c660dd4: *sr=(*sr&~1u)|!(r[0]&0x2u); /* tst #0x2,r0 */
 goto L8c660dd6;
L8c660dd6: if(!(*sr&1u))goto L8c660de2; /* bf 0x8c660de2 */
 goto L8c660dd8;
L8c660dd8: r[0]=0u; /* mov #0x0,r0 */
 goto L8c660dda;
L8c660dda: r[15]+=4u; /* add #0x4,r15 */
 goto L8c660ddc;
L8c660ddc: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c660dde;
L8c660dde: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c660de2: r[0]=1u; /* mov #0x1,r0 */
 goto L8c660de4;
L8c660de4: r[15]+=4u; /* add #0x4,r15 */
 goto L8c660de6;
L8c660de6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c660de8;
L8c660de8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
