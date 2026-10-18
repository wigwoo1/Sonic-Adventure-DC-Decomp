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
/* Static C recovery of retail sound backend field reader 8c66bd44..8c66bdb4.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_backend_fieldc_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66bd44;
L8c66bd44: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66bd46;
L8c66bd46: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66bd48;
L8c66bd48: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c66bd4a;
L8c66bd4a: r[3]=rd(m,0x8c66bdb8u,4); /* mov.l 0x8c66bdb8,r3 */
 goto L8c66bd4c;
L8c66bd4c: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c66bd4e;
L8c66bd4e: r[6]=rd(m,r[3],4); /* mov.l @r3,r6 */
 goto L8c66bd50;
L8c66bd50: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c66bd52;
L8c66bd52: take=!(*sr&1u);r[14]=0u;if(take)goto L8c66bd5c;goto L8c66bd56; /* bf/s 0x8c66bd5c */
 goto L8c66bd56;
L8c66bd56: r[14]=rd(m,0x8c66bdbcu,4); /* mov.l 0x8c66bdbc,r14 */
 goto L8c66bd58;
L8c66bd58: goto L8c66bd9e; /* bra 0x8c66bd9e */
 goto L8c66bd5c;
L8c66bd5c: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66bd5e;
L8c66bd5e: if(!(*sr&1u))goto L8c66bd66; /* bf 0x8c66bd66 */
 goto L8c66bd60;
L8c66bd60: r[14]=rd(m,0x8c66bdc0u,4); /* mov.l 0x8c66bdc0,r14 */
 goto L8c66bd62;
L8c66bd62: goto L8c66bd9e; /* bra 0x8c66bd9e */
 goto L8c66bd66;
L8c66bd66: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c66bd68;
L8c66bd68: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c66bd6a;
L8c66bd6a: if(!(*sr&1u))goto L8c66bd72; /* bf 0x8c66bd72 */
 goto L8c66bd6c;
L8c66bd6c: r[14]=rd(m,0x8c66bdc4u,4); /* mov.l 0x8c66bdc4,r14 */
 goto L8c66bd6e;
L8c66bd6e: goto L8c66bd9e; /* bra 0x8c66bd9e */
 goto L8c66bd72;
L8c66bd72: r[0]=sx(m,0x8c66bdb4u,2); /* mov.w 0x8c66bdb4,r0 */
 goto L8c66bd74;
L8c66bd74: r[5]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r5 */
 goto L8c66bd76;
L8c66bd76: r[3]=sx(m,0x8c66bdb6u,2); /* mov.w 0x8c66bdb6,r3 */
 goto L8c66bd78;
L8c66bd78: r[5]<<=2; /* shll2 r5 */
 goto L8c66bd7a;
L8c66bd7a: r[2]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r2 */
 goto L8c66bd7c;
L8c66bd7c: r[5]<<=2; /* shll2 r5 */
 goto L8c66bd7e;
L8c66bd7e: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66bd80;
L8c66bd80: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1; /* shll r5 */
 goto L8c66bd82;
L8c66bd82: r[3]+=r[2]; /* add r2,r3 */
 goto L8c66bd84;
L8c66bd84: r[2]=rd(m,0x8c66bdc8u,4); /* mov.l 0x8c66bdc8,r2 */
 goto L8c66bd86;
L8c66bd86: r[0]<<=2; /* shll2 r0 */
 goto L8c66bd88;
L8c66bd88: r[0]<<=2; /* shll2 r0 */
 goto L8c66bd8a;
L8c66bd8a: r[0]<<=2; /* shll2 r0 */
 goto L8c66bd8c;
L8c66bd8c: r[0]+=r[3]; /* add r3,r0 */
 goto L8c66bd8e;
L8c66bd8e: r[0]+=r[5]; /* add r5,r0 */
 goto L8c66bd90;
L8c66bd90: r[4]=r[0]; /* mov r0,r4 */
 goto L8c66bd92;
L8c66bd92: r[4]+=20u; /* add #0x14,r4 */
 goto L8c66bd94;
L8c66bd94: target=r[2];*pr=0x8c66bd98u;r[5]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66bd98;
L8c66bd98: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c66bd9a;
L8c66bd9a: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c66bd9c;
L8c66bd9c: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c66bd9e;
L8c66bd9e: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66bda0;
L8c66bda0: if((*sr&1u))goto L8c66bdaa; /* bt 0x8c66bdaa */
 goto L8c66bda2;
L8c66bda2: r[2]=rd(m,0x8c66bdd0u,4); /* mov.l 0x8c66bdd0,r2 */
 goto L8c66bda4;
L8c66bda4: r[4]=rd(m,0x8c66bdccu,4); /* mov.l 0x8c66bdcc,r4 */
 goto L8c66bda6;
L8c66bda6: target=r[2];*pr=0x8c66bdaau;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66bdaa;
L8c66bdaa: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66bdac;
L8c66bdac: r[15]+=8u; /* add #0x8,r15 */
 goto L8c66bdae;
L8c66bdae: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66bdb0;
L8c66bdb0: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
