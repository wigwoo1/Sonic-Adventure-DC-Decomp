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
/* Static C recovery of retail Tikal helper 8c09ce40..8c09ce7a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_interrupt_tail_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c09ce40;
L8c09ce40: r[2]=rd(m,0x8c09cec0u,4); /* mov.l 0x8c09cec0,r2 */
 goto L8c09ce42;
L8c09ce42: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c09ce44;
L8c09ce44: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c09ce46;
L8c09ce46: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c09ce48;
L8c09ce48: if((*sr&1u))goto L8c09ce5e; /* bt 0x8c09ce5e */
 goto L8c09ce4a;
L8c09ce4a: r[1]=rd(m,0x8c09cec4u,4); /* mov.l 0x8c09cec4,r1 */
 goto L8c09ce4c;
L8c09ce4c: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c09ce4e;
L8c09ce4e: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c09ce50;
L8c09ce50: if(!(*sr&1u))goto L8c09ce5e; /* bf 0x8c09ce5e */
 goto L8c09ce52;
L8c09ce52: r[3]=rd(m,0x8c09cec8u,4); /* mov.l 0x8c09cec8,r3 */
 goto L8c09ce54;
L8c09ce54: target=r[3];*pr=0x8c09ce58u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c09ce58;
L8c09ce58: r[2]=rd(m,0x8c09ceccu,4); /* mov.l 0x8c09cecc,r2 */
 goto L8c09ce5a;
L8c09ce5a: target=r[2];*pr=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c09ce5e: r[2]=rd(m,0x8c09ced0u,4); /* mov.l 0x8c09ced0,r2 */
 goto L8c09ce60;
L8c09ce60: target=r[2];*pr=0x8c09ce64u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c09ce64;
L8c09ce64: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c09ce66;
L8c09ce66: take=(*sr&1u);r[4]=r[0];if(take)goto L8c09ce6e;goto L8c09ce6a; /* bt/s 0x8c09ce6e */
 goto L8c09ce6a;
L8c09ce6a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c09ce6c;
L8c09ce6c: if(!(*sr&1u))goto L8c09ce74; /* bf 0x8c09ce74 */
 goto L8c09ce6e;
L8c09ce6e: r[3]=rd(m,0x8c09ced4u,4); /* mov.l 0x8c09ced4,r3 */
 goto L8c09ce70;
L8c09ce70: target=r[3];*pr=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
L8c09ce74: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c09ce76;
L8c09ce76: return 0; /* rts */
}
