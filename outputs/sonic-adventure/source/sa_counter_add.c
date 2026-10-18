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
/* Static C recovery of retail Tikal helper 8c08a198..8c08a1ea.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_counter_add(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c08a198;
L8c08a198: r[5]=rd(m,0x8c08a23cu,4); /* mov.l 0x8c08a23c,r5 */
 goto L8c08a19a;
L8c08a19a: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c08a19c;
L8c08a19c: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c08a19e;
L8c08a19e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c08a1a0;
L8c08a1a0: r[3]=sx(m,r[5],2); /* mov.w @r5,r3 */
 goto L8c08a1a2;
L8c08a1a2: r[6]=sx(m,r[5],2); /* mov.w @r5,r6 */
 goto L8c08a1a4;
L8c08a1a4: r[3]+=r[4]; /* add r4,r3 */
 goto L8c08a1a6;
L8c08a1a6: wr(m,r[5],r[3],2); /* mov.w r3,@r5 */
 goto L8c08a1a8;
L8c08a1a8: r[1]=(uint32_t)(int32_t)(int16_t)r[6]; /* exts.w r6,r1 */
 goto L8c08a1aa;
L8c08a1aa: r[3]=rd(m,0x8c08a240u,4); /* mov.l 0x8c08a240,r3 */
 goto L8c08a1ac;
L8c08a1ac: r[4]=100u; /* mov #0x64,r4 */
 goto L8c08a1ae;
L8c08a1ae: target=r[3];*pr=0x8c08a1b2u;r[0]=r[4];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c08a1b2;
L8c08a1b2: r[3]=rd(m,0x8c08a240u,4); /* mov.l 0x8c08a240,r3 */
 goto L8c08a1b4;
L8c08a1b4: r[14]=r[0]; /* mov r0,r14 */
 goto L8c08a1b6;
L8c08a1b6: r[1]=sx(m,r[5],2); /* mov.w @r5,r1 */
 goto L8c08a1b8;
L8c08a1b8: target=r[3];*pr=0x8c08a1bcu;r[0]=r[4];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c08a1bc;
L8c08a1bc: r[13]=r[0]; /* mov r0,r13 */
 goto L8c08a1be;
L8c08a1be: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[13]); /* cmp/ge r13,r14 */
 goto L8c08a1c0;
L8c08a1c0: if((*sr&1u))goto L8c08a1c8; /* bt 0x8c08a1c8 */
 goto L8c08a1c2;
L8c08a1c2: r[4]=r[13]; /* mov r13,r4 */
 goto L8c08a1c4;
L8c08a1c4: *pr=0x8c08a1c8u;r[4]-=r[14];status=call(ctx,0x8c08a120u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c08a120 */
 goto L8c08a1c8;
L8c08a1c8: r[2]=rd(m,0x8c08a228u,4); /* mov.l 0x8c08a228,r2 */
 goto L8c08a1ca;
L8c08a1ca: target=r[2];*pr=0x8c08a1ceu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c08a1ce;
L8c08a1ce: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c08a1d0;
L8c08a1d0: if(!(*sr&1u))goto L8c08a1e2; /* bf 0x8c08a1e2 */
 goto L8c08a1d2;
L8c08a1d2: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c08a1d4;
L8c08a1d4: r[6]=rd(m,0x8c08a244u,4); /* mov.l 0x8c08a244,r6 */
 goto L8c08a1d6;
L8c08a1d6: r[5]=6u; /* mov #0x6,r5 */
 goto L8c08a1d8;
L8c08a1d8: r[2]=rd(m,0x8c08a230u,4); /* mov.l 0x8c08a230,r2 */
 goto L8c08a1da;
L8c08a1da: r[4]=8u; /* mov #0x8,r4 */
 goto L8c08a1dc;
L8c08a1dc: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c08a1de;
L8c08a1de: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c08a1e2: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c08a1e4;
L8c08a1e4: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c08a1e6;
L8c08a1e6: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
