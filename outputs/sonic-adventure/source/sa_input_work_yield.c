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
/* Static C recovery of retail Tikal helper 8c668e1e..8c668e5c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
static int sa_input_work_yield_core(uint32_t entry,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 if(entry==0)goto L8c668e1e;else if(entry==0x8c668e48u)goto L8c668e48;else return -4;
L8c668e1e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c668e20;
L8c668e20: *sr=(*sr&~1u)|((int32_t)r[4]>=0); /* cmp/pz r4 */
 goto L8c668e22;
L8c668e22: r[14]=rd(m,0x8c668ea4u,4); /* mov.l 0x8c668ea4,r14 */
 goto L8c668e24;
L8c668e24: r[0]=sx(m,0x8c668ea0u,2); /* mov.w 0x8c668ea0,r0 */
 goto L8c668e26;
L8c668e26: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c668e28;
L8c668e28: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c668e2a;
L8c668e2a: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c668e2c;
L8c668e2c: if(!(*sr&1u))goto L8c668e38; /* bf 0x8c668e38 */
 goto L8c668e2e;
L8c668e2e: r[0]=sx(m,0x8c668e9eu,2); /* mov.w 0x8c668e9e,r0 */
 goto L8c668e30;
L8c668e30: r[1]=rd(m,r[14],4); /* mov.l @r14,r1 */
 goto L8c668e32;
L8c668e32: r[1]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r1 */
 goto L8c668e34;
L8c668e34: r[1]+=r[4];goto L8c668e3a; /* bra 0x8c668e3a */
 goto L8c668e38;
L8c668e38: r[1]=rd(m,0x8c668ea8u,4); /* mov.l 0x8c668ea8,r1 */
 goto L8c668e3a;
L8c668e3a: r[0]=sx(m,0x8c668ea0u,2); /* mov.w 0x8c668ea0,r0 */
 goto L8c668e3c;
L8c668e3c: r[3]=rd(m,0x8c668eacu,4); /* mov.l 0x8c668eac,r3 */
 goto L8c668e3e;
L8c668e3e: wr(m,r[2]+0x14u,r[1],4); /* mov.l r1,@(0x14,r2) */
 goto L8c668e40;
L8c668e40: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c668e42;
L8c668e42: r[4]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r4 */
 goto L8c668e44;
L8c668e44: target=r[3];*pr=0x8c668e48u;r[4]+=24u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668e48;
L8c668e48: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c668e4a;
L8c668e4a: if(!(*sr&1u))goto L8c668e56; /* bf 0x8c668e56 */
 goto L8c668e4c;
L8c668e4c: r[3]=rd(m,0x8c668eb0u,4); /* mov.l 0x8c668eb0,r3 */
 goto L8c668e4e;
L8c668e4e: r[5]=1u; /* mov #0x1,r5 */
 goto L8c668e50;
L8c668e50: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c668e52;
L8c668e52: target=r[3];*pr=0x8c668e56u;r[4]+=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668e56;
L8c668e56: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c668e58;
L8c668e58: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_input_work_yield(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_work_yield_core(0,m,f,sr,r,pr,fpul,macl,ctx,call);}
SA_STAGE_EXPORT int sa_input_work_yield_resume(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_work_yield_core(*pr,m,f,sr,r,pr,fpul,macl,ctx,call);}
