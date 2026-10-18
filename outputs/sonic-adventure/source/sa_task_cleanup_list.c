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
/* Static C recovery of retail Tikal helper 8c04b48c..8c04b4d2.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_task_cleanup_list(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c04b48c;
L8c04b48c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c04b48e;
L8c04b48e: r[0]=64u; /* mov #0x40,r0 */
 goto L8c04b490;
L8c04b490: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c04b492;
L8c04b492: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c04b494;
L8c04b494: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c04b496;
L8c04b496: r[14]=rd(m,r[4]+0x3cu,4); /* mov.l @(0x3c,r4),r14 */
 goto L8c04b498;
L8c04b498: r[13]=rd(m,0x8c04b5e4u,4); /* mov.l 0x8c04b5e4,r13 */
 goto L8c04b49a;
L8c04b49a: r[4]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r4 */
 goto L8c04b49c;
L8c04b49c: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c04b49e;
L8c04b49e: if((*sr&1u))goto L8c04b4aa; /* bt 0x8c04b4aa */
 goto L8c04b4a0;
L8c04b4a0: target=r[13];*pr=0x8c04b4a4u;r[12]=rd(m,r[4]+0x10u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c04b4a4;
L8c04b4a4: r[4]=r[12]; /* mov r12,r4 */
 goto L8c04b4a6;
L8c04b4a6: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c04b4a8;
L8c04b4a8: if(!(*sr&1u))goto L8c04b4a0; /* bf 0x8c04b4a0 */
 goto L8c04b4aa;
L8c04b4aa: r[0]=64u; /* mov #0x40,r0 */
 goto L8c04b4ac;
L8c04b4ac: r[3]=0u; /* mov #0x0,r3 */
 goto L8c04b4ae;
L8c04b4ae: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c04b4b0;
L8c04b4b0: r[0]=68u; /* mov #0x44,r0 */
 goto L8c04b4b2;
L8c04b4b2: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c04b4b4;
L8c04b4b4: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c04b4b6;
L8c04b4b6: if((*sr&1u))goto L8c04b4c2; /* bt 0x8c04b4c2 */
 goto L8c04b4b8;
L8c04b4b8: target=r[13];*pr=0x8c04b4bcu;r[4]=rd(m,r[14]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c04b4bc;
L8c04b4bc: r[0]=68u; /* mov #0x44,r0 */
 goto L8c04b4be;
L8c04b4be: r[3]=0u; /* mov #0x0,r3 */
 goto L8c04b4c0;
L8c04b4c0: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c04b4c2;
L8c04b4c2: fr[3]=0; /* fldi0 fr3 */
 goto L8c04b4c4;
L8c04b4c4: r[0]=60u; /* mov #0x3c,r0 */
 goto L8c04b4c6;
L8c04b4c6: wr(m,r[0]+r[14],fr[3],4); /* fmov fr3,@(r0,r14) */
 goto L8c04b4c8;
L8c04b4c8: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c04b4ca;
L8c04b4ca: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c04b4cc;
L8c04b4cc: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c04b4ce;
L8c04b4ce: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
