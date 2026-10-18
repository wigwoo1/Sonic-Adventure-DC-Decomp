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
int sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);
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
/* Static C recovery of retail sound backend field reader 8c66ba2c..8c66ba9c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_backend_field14_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66ba2c;
L8c66ba2c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66ba2e;
L8c66ba2e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66ba30;
L8c66ba30: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c66ba32;
L8c66ba32: r[3]=rd(m,0x8c66baa0u,4); /* mov.l 0x8c66baa0,r3 */
 goto L8c66ba34;
L8c66ba34: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c66ba36;
L8c66ba36: r[6]=rd(m,r[3],4); /* mov.l @r3,r6 */
 goto L8c66ba38;
L8c66ba38: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c66ba3a;
L8c66ba3a: take=!(*sr&1u);r[14]=0u;if(take)goto L8c66ba44;goto L8c66ba3e; /* bf/s 0x8c66ba44 */
 goto L8c66ba3e;
L8c66ba3e: r[14]=rd(m,0x8c66baa4u,4); /* mov.l 0x8c66baa4,r14 */
 goto L8c66ba40;
L8c66ba40: goto L8c66ba86; /* bra 0x8c66ba86 */
 goto L8c66ba44;
L8c66ba44: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66ba46;
L8c66ba46: if(!(*sr&1u))goto L8c66ba4e; /* bf 0x8c66ba4e */
 goto L8c66ba48;
L8c66ba48: r[14]=rd(m,0x8c66baa8u,4); /* mov.l 0x8c66baa8,r14 */
 goto L8c66ba4a;
L8c66ba4a: goto L8c66ba86; /* bra 0x8c66ba86 */
 goto L8c66ba4e;
L8c66ba4e: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c66ba50;
L8c66ba50: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c66ba52;
L8c66ba52: if(!(*sr&1u))goto L8c66ba5a; /* bf 0x8c66ba5a */
 goto L8c66ba54;
L8c66ba54: r[14]=rd(m,0x8c66baacu,4); /* mov.l 0x8c66baac,r14 */
 goto L8c66ba56;
L8c66ba56: goto L8c66ba86; /* bra 0x8c66ba86 */
 goto L8c66ba5a;
L8c66ba5a: r[0]=sx(m,0x8c66ba9cu,2); /* mov.w 0x8c66ba9c,r0 */
 goto L8c66ba5c;
L8c66ba5c: r[5]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r5 */
 goto L8c66ba5e;
L8c66ba5e: r[3]=sx(m,0x8c66ba9eu,2); /* mov.w 0x8c66ba9e,r3 */
 goto L8c66ba60;
L8c66ba60: r[5]<<=2; /* shll2 r5 */
 goto L8c66ba62;
L8c66ba62: r[2]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r2 */
 goto L8c66ba64;
L8c66ba64: r[5]<<=2; /* shll2 r5 */
 goto L8c66ba66;
L8c66ba66: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66ba68;
L8c66ba68: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1; /* shll r5 */
 goto L8c66ba6a;
L8c66ba6a: r[3]+=r[2]; /* add r2,r3 */
 goto L8c66ba6c;
L8c66ba6c: r[2]=rd(m,0x8c66bab0u,4); /* mov.l 0x8c66bab0,r2 */
 goto L8c66ba6e;
L8c66ba6e: r[0]<<=2; /* shll2 r0 */
 goto L8c66ba70;
L8c66ba70: r[0]<<=2; /* shll2 r0 */
 goto L8c66ba72;
L8c66ba72: r[0]<<=2; /* shll2 r0 */
 goto L8c66ba74;
L8c66ba74: r[0]+=r[3]; /* add r3,r0 */
 goto L8c66ba76;
L8c66ba76: r[0]+=r[5]; /* add r5,r0 */
 goto L8c66ba78;
L8c66ba78: r[4]=r[0]; /* mov r0,r4 */
 goto L8c66ba7a;
L8c66ba7a: r[4]+=28u; /* add #0x1c,r4 */
 goto L8c66ba7c;
L8c66ba7c: target=r[2];*pr=0x8c66ba80u;r[5]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66ba80;
L8c66ba80: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c66ba82;
L8c66ba82: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c66ba84;
L8c66ba84: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c66ba86;
L8c66ba86: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66ba88;
L8c66ba88: if((*sr&1u))goto L8c66ba92; /* bt 0x8c66ba92 */
 goto L8c66ba8a;
L8c66ba8a: r[2]=rd(m,0x8c66bab8u,4); /* mov.l 0x8c66bab8,r2 */
 goto L8c66ba8c;
L8c66ba8c: r[4]=rd(m,0x8c66bab4u,4); /* mov.l 0x8c66bab4,r4 */
 goto L8c66ba8e;
L8c66ba8e: target=r[2];*pr=0x8c66ba92u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66ba92;
L8c66ba92: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66ba94;
L8c66ba94: r[15]+=8u; /* add #0x8,r15 */
 goto L8c66ba96;
L8c66ba96: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66ba98;
L8c66ba98: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
