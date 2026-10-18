#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_cache_invalidate(void *,uint32_t);
int sa_sh4_cache_purge(void *,uint32_t);
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
/* Static C recovery of retail Tikal helper 8c66b28c..8c66b304.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_control_setup_request(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66b28c;
L8c66b28c: r[2]=rd(m,0x8c66b2e0u,4); /* mov.l 0x8c66b2e0,r2 */
 goto L8c66b28e;
L8c66b28e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66b290;
L8c66b290: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66b292;
L8c66b292: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c66b294;
L8c66b294: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c66b296;
L8c66b296: if(!(*sr&1u))goto L8c66b29e; /* bf 0x8c66b29e */
 goto L8c66b298;
L8c66b298: r[14]=rd(m,0x8c66b2e4u,4); /* mov.l 0x8c66b2e4,r14 */
 goto L8c66b29a;
L8c66b29a: goto L8c66b2ca; /* bra 0x8c66b2ca */
 goto L8c66b29e;
L8c66b29e: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66b2a0;
L8c66b2a0: if(!(*sr&1u))goto L8c66b2a8; /* bf 0x8c66b2a8 */
 goto L8c66b2a2;
L8c66b2a2: r[14]=rd(m,0x8c66b2e8u,4); /* mov.l 0x8c66b2e8,r14 */
 goto L8c66b2a4;
L8c66b2a4: goto L8c66b2ca; /* bra 0x8c66b2ca */
 goto L8c66b2a8;
L8c66b2a8: r[3]=sx(m,0x8c66b2deu,2); /* mov.w 0x8c66b2de,r3 */
 goto L8c66b2aa;
L8c66b2aa: r[6]=(uint32_t)(int32_t)(int8_t)r[6]; /* exts.b r6,r6 */
 goto L8c66b2ac;
L8c66b2ac: r[1]=rd(m,0x8c66b2ecu,4); /* mov.l 0x8c66b2ec,r1 */
 goto L8c66b2ae;
L8c66b2ae: r[2]=41u; /* mov #0x29,r2 */
 goto L8c66b2b0;
L8c66b2b0: r[6]+=r[3]; /* add r3,r6 */
 goto L8c66b2b2;
L8c66b2b2: r[3]=rd(m,0x8c66b2f0u,4); /* mov.l 0x8c66b2f0,r3 */
 goto L8c66b2b4;
L8c66b2b4: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c66b2b6;
L8c66b2b6: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66b2b8;
L8c66b2b8: r[4]=r[1]; /* mov r1,r4 */
 goto L8c66b2ba;
L8c66b2ba: r[1]=rd(m,0x8c66b2f8u,4); /* mov.l 0x8c66b2f8,r1 */
 goto L8c66b2bc;
L8c66b2bc: wr(m,r[3],r[0],1); /* mov.b r0,@r3 */
 goto L8c66b2be;
L8c66b2be: r[3]+=1u; /* add #0x1,r3 */
 goto L8c66b2c0;
L8c66b2c0: r[2]=rd(m,0x8c66b2f4u,4); /* mov.l 0x8c66b2f4,r2 */
 goto L8c66b2c2;
L8c66b2c2: wr(m,r[3],r[5],1); /* mov.b r5,@r3 */
 goto L8c66b2c4;
L8c66b2c4: target=r[1];*pr=0x8c66b2c8u;wr(m,r[2],r[6],1);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c66b2c8;
L8c66b2c8: r[14]=r[0]; /* mov r0,r14 */
 goto L8c66b2ca;
L8c66b2ca: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66b2cc;
L8c66b2cc: if((*sr&1u))goto L8c66b2d6; /* bt 0x8c66b2d6 */
 goto L8c66b2ce;
L8c66b2ce: r[2]=rd(m,0x8c66b300u,4); /* mov.l 0x8c66b300,r2 */
 goto L8c66b2d0;
L8c66b2d0: r[4]=rd(m,0x8c66b2fcu,4); /* mov.l 0x8c66b2fc,r4 */
 goto L8c66b2d2;
L8c66b2d2: target=r[2];*pr=0x8c66b2d6u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66b2d6;
L8c66b2d6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66b2d8;
L8c66b2d8: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66b2da;
L8c66b2da: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
