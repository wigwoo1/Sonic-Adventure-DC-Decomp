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
/* Static C recovery of retail Tikal helper 8c66bc24..8c66bc94.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_backend_field_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66bc24;
L8c66bc24: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66bc26;
L8c66bc26: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66bc28;
L8c66bc28: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c66bc2a;
L8c66bc2a: r[3]=rd(m,0x8c66bc98u,4); /* mov.l 0x8c66bc98,r3 */
 goto L8c66bc2c;
L8c66bc2c: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c66bc2e;
L8c66bc2e: r[6]=rd(m,r[3],4); /* mov.l @r3,r6 */
 goto L8c66bc30;
L8c66bc30: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c66bc32;
L8c66bc32: take=!(*sr&1u);r[14]=0u;if(take)goto L8c66bc3c;goto L8c66bc36; /* bf/s 0x8c66bc3c */
 goto L8c66bc36;
L8c66bc36: r[14]=rd(m,0x8c66bc9cu,4); /* mov.l 0x8c66bc9c,r14 */
 goto L8c66bc38;
L8c66bc38: goto L8c66bc7e; /* bra 0x8c66bc7e */
 goto L8c66bc3c;
L8c66bc3c: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66bc3e;
L8c66bc3e: if(!(*sr&1u))goto L8c66bc46; /* bf 0x8c66bc46 */
 goto L8c66bc40;
L8c66bc40: r[14]=rd(m,0x8c66bca0u,4); /* mov.l 0x8c66bca0,r14 */
 goto L8c66bc42;
L8c66bc42: goto L8c66bc7e; /* bra 0x8c66bc7e */
 goto L8c66bc46;
L8c66bc46: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c66bc48;
L8c66bc48: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c66bc4a;
L8c66bc4a: if(!(*sr&1u))goto L8c66bc52; /* bf 0x8c66bc52 */
 goto L8c66bc4c;
L8c66bc4c: r[14]=rd(m,0x8c66bca4u,4); /* mov.l 0x8c66bca4,r14 */
 goto L8c66bc4e;
L8c66bc4e: goto L8c66bc7e; /* bra 0x8c66bc7e */
 goto L8c66bc52;
L8c66bc52: r[0]=sx(m,0x8c66bc94u,2); /* mov.w 0x8c66bc94,r0 */
 goto L8c66bc54;
L8c66bc54: r[5]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r5 */
 goto L8c66bc56;
L8c66bc56: r[3]=sx(m,0x8c66bc96u,2); /* mov.w 0x8c66bc96,r3 */
 goto L8c66bc58;
L8c66bc58: r[5]<<=2; /* shll2 r5 */
 goto L8c66bc5a;
L8c66bc5a: r[2]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r2 */
 goto L8c66bc5c;
L8c66bc5c: r[5]<<=2; /* shll2 r5 */
 goto L8c66bc5e;
L8c66bc5e: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66bc60;
L8c66bc60: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1; /* shll r5 */
 goto L8c66bc62;
L8c66bc62: r[3]+=r[2]; /* add r2,r3 */
 goto L8c66bc64;
L8c66bc64: r[2]=rd(m,0x8c66bca8u,4); /* mov.l 0x8c66bca8,r2 */
 goto L8c66bc66;
L8c66bc66: r[0]<<=2; /* shll2 r0 */
 goto L8c66bc68;
L8c66bc68: r[0]<<=2; /* shll2 r0 */
 goto L8c66bc6a;
L8c66bc6a: r[0]<<=2; /* shll2 r0 */
 goto L8c66bc6c;
L8c66bc6c: r[0]+=r[3]; /* add r3,r0 */
 goto L8c66bc6e;
L8c66bc6e: r[0]+=r[5]; /* add r5,r0 */
 goto L8c66bc70;
L8c66bc70: r[4]=r[0]; /* mov r0,r4 */
 goto L8c66bc72;
L8c66bc72: r[4]+=8u; /* add #0x8,r4 */
 goto L8c66bc74;
L8c66bc74: target=r[2];*pr=0x8c66bc78u;r[5]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66bc78;
L8c66bc78: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c66bc7a;
L8c66bc7a: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c66bc7c;
L8c66bc7c: wr(m,r[3],r[2],2); /* mov.w r2,@r3 */
 goto L8c66bc7e;
L8c66bc7e: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66bc80;
L8c66bc80: if((*sr&1u))goto L8c66bc8a; /* bt 0x8c66bc8a */
 goto L8c66bc82;
L8c66bc82: r[2]=rd(m,0x8c66bcb0u,4); /* mov.l 0x8c66bcb0,r2 */
 goto L8c66bc84;
L8c66bc84: r[4]=rd(m,0x8c66bcacu,4); /* mov.l 0x8c66bcac,r4 */
 goto L8c66bc86;
L8c66bc86: target=r[2];*pr=0x8c66bc8au;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66bc8a;
L8c66bc8a: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66bc8c;
L8c66bc8c: r[15]+=8u; /* add #0x8,r15 */
 goto L8c66bc8e;
L8c66bc8e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66bc90;
L8c66bc90: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
