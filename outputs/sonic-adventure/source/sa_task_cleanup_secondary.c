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
/* Static C recovery of retail Tikal helper 8c04b82e..8c04b868.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_task_cleanup_secondary(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c04b82e;
L8c04b82e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c04b830;
L8c04b830: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c04b832;
L8c04b832: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c04b834;
L8c04b834: r[13]=r[4]; /* mov r4,r13 */
 goto L8c04b836;
L8c04b836: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c04b838;
L8c04b838: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c04b83a;
L8c04b83a: r[4]=rd(m,r[13]+0x3cu,4); /* mov.l @(0x3c,r13),r4 */
 goto L8c04b83c;
L8c04b83c: r[12]=rd(m,0x8c04b888u,4); /* mov.l 0x8c04b888,r12 */
 goto L8c04b83e;
L8c04b83e: r[4]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r4 */
 goto L8c04b840;
L8c04b840: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c04b842;
L8c04b842: if((*sr&1u))goto L8c04b84e; /* bt 0x8c04b84e */
 goto L8c04b844;
L8c04b844: target=r[12];*pr=0x8c04b848u;r[14]=rd(m,r[4]+0xcu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c04b848;
L8c04b848: r[4]=r[14]; /* mov r14,r4 */
 goto L8c04b84a;
L8c04b84a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c04b84c;
L8c04b84c: if(!(*sr&1u))goto L8c04b844; /* bf 0x8c04b844 */
 goto L8c04b84e;
L8c04b84e: r[3]=rd(m,r[13]+0x3cu,4); /* mov.l @(0x3c,r13),r3 */
 goto L8c04b850;
L8c04b850: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c04b852;
L8c04b852: r[2]=0u; /* mov #0x0,r2 */
 goto L8c04b854;
L8c04b854: fr[3]=0; /* fldi0 fr3 */
 goto L8c04b856;
L8c04b856: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c04b858;
L8c04b858: r[0]=72u; /* mov #0x48,r0 */
 goto L8c04b85a;
L8c04b85a: r[3]=rd(m,r[13]+0x3cu,4); /* mov.l @(0x3c,r13),r3 */
 goto L8c04b85c;
L8c04b85c: wr(m,r[0]+r[3],fr[3],4); /* fmov fr3,@(r0,r3) */
 goto L8c04b85e;
L8c04b85e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c04b860;
L8c04b860: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c04b862;
L8c04b862: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c04b864;
L8c04b864: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
