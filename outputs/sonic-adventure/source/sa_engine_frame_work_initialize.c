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
/* Static C recovery of retail Tikal helper 8c666b34..8c666b84.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_frame_work_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c666b34;
L8c666b34: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c666b36;
L8c666b36: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c666b38;
L8c666b38: r[13]=rd(m,0x8c666b8cu,4); /* mov.l 0x8c666b8c,r13 */
 goto L8c666b3a;
L8c666b3a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c666b3c;
L8c666b3c: r[5]=rd(m,r[13],4); /* mov.l @r13,r5 */
 goto L8c666b3e;
L8c666b3e: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c666b40;
L8c666b40: if(!(*sr&1u))goto L8c666b48; /* bf 0x8c666b48 */
 goto L8c666b42;
L8c666b42: r[14]=rd(m,0x8c666b90u,4); /* mov.l 0x8c666b90,r14 */
 goto L8c666b44;
L8c666b44: goto L8c666b6e; /* bra 0x8c666b6e */
 goto L8c666b48;
L8c666b48: r[3]=rd(m,0x8c666b94u,4); /* mov.l 0x8c666b94,r3 */
 goto L8c666b4a;
L8c666b4a: r[0]=sx(m,0x8c666b84u,2); /* mov.w 0x8c666b84,r0 */
 goto L8c666b4c;
L8c666b4c: r[6]=sx(m,0x8c666b86u,2); /* mov.w 0x8c666b86,r6 */
 goto L8c666b4e;
L8c666b4e: r[2]=rd(m,0x8c666b98u,4); /* mov.l 0x8c666b98,r2 */
 goto L8c666b50;
L8c666b50: wr(m,r[5]+r[0],r[4],4); /* mov.l r4,@(r0,r5) */
 goto L8c666b52;
L8c666b52: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c666b54;
L8c666b54: target=r[2];*pr=0x8c666b58u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c666b58;
L8c666b58: r[0]=sx(m,0x8c666b88u,2); /* mov.w 0x8c666b88,r0 */
 goto L8c666b5a;
L8c666b5a: r[2]=0u; /* mov #0x0,r2 */
 goto L8c666b5c;
L8c666b5c: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c666b5e;
L8c666b5e: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c666b60;
L8c666b60: r[2]=rd(m,0x8c666b9cu,4); /* mov.l 0x8c666b9c,r2 */
 goto L8c666b62;
L8c666b62: r[3]=sx(m,0x8c666b8au,2); /* mov.w 0x8c666b8a,r3 */
 goto L8c666b64;
L8c666b64: r[4]=rd(m,r[13],4); /* mov.l @r13,r4 */
 goto L8c666b66;
L8c666b66: target=r[2];*pr=0x8c666b6au;r[4]+=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c666b6a;
L8c666b6a: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c666b6c;
L8c666b6c: r[14]=r[0]; /* mov r0,r14 */
 goto L8c666b6e;
L8c666b6e: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c666b70;
L8c666b70: if((*sr&1u))goto L8c666b7a; /* bt 0x8c666b7a */
 goto L8c666b72;
L8c666b72: r[2]=rd(m,0x8c666ba4u,4); /* mov.l 0x8c666ba4,r2 */
 goto L8c666b74;
L8c666b74: r[4]=rd(m,0x8c666ba0u,4); /* mov.l 0x8c666ba0,r4 */
 goto L8c666b76;
L8c666b76: target=r[2];*pr=0x8c666b7au;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c666b7a;
L8c666b7a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c666b7c;
L8c666b7c: r[0]=r[14]; /* mov r14,r0 */
 goto L8c666b7e;
L8c666b7e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c666b80;
L8c666b80: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
