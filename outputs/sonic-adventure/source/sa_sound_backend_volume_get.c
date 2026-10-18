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
/* Static C recovery of retail Tikal helper 8c66bdd4..8c66be38.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_backend_volume_get(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66bdd4;
L8c66bdd4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66bdd6;
L8c66bdd6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66bdd8;
L8c66bdd8: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c66bdda;
L8c66bdda: r[3]=rd(m,0x8c66be3cu,4); /* mov.l 0x8c66be3c,r3 */
 goto L8c66bddc;
L8c66bddc: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c66bdde;
L8c66bdde: r[6]=rd(m,r[3],4); /* mov.l @r3,r6 */
 goto L8c66bde0;
L8c66bde0: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c66bde2;
L8c66bde2: take=!(*sr&1u);r[14]=0u;if(take)goto L8c66bdec;goto L8c66bde6; /* bf/s 0x8c66bdec */
 goto L8c66bde6;
L8c66bde6: r[14]=rd(m,0x8c66be40u,4); /* mov.l 0x8c66be40,r14 */
 goto L8c66bde8;
L8c66bde8: goto L8c66be22; /* bra 0x8c66be22 */
 goto L8c66bdec;
L8c66bdec: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66bdee;
L8c66bdee: if(!(*sr&1u))goto L8c66bdf6; /* bf 0x8c66bdf6 */
 goto L8c66bdf0;
L8c66bdf0: r[14]=rd(m,0x8c66be44u,4); /* mov.l 0x8c66be44,r14 */
 goto L8c66bdf2;
L8c66bdf2: goto L8c66be22; /* bra 0x8c66be22 */
 goto L8c66bdf6;
L8c66bdf6: r[0]=sx(m,0x8c66be38u,2); /* mov.w 0x8c66be38,r0 */
 goto L8c66bdf8;
L8c66bdf8: r[5]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r5 */
 goto L8c66bdfa;
L8c66bdfa: r[3]=sx(m,0x8c66be3au,2); /* mov.w 0x8c66be3a,r3 */
 goto L8c66bdfc;
L8c66bdfc: r[5]<<=2; /* shll2 r5 */
 goto L8c66bdfe;
L8c66bdfe: r[2]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r2 */
 goto L8c66be00;
L8c66be00: r[5]<<=2; /* shll2 r5 */
 goto L8c66be02;
L8c66be02: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66be04;
L8c66be04: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1; /* shll r5 */
 goto L8c66be06;
L8c66be06: r[3]+=r[2]; /* add r2,r3 */
 goto L8c66be08;
L8c66be08: r[2]=rd(m,0x8c66be48u,4); /* mov.l 0x8c66be48,r2 */
 goto L8c66be0a;
L8c66be0a: r[0]<<=2; /* shll2 r0 */
 goto L8c66be0c;
L8c66be0c: r[0]<<=2; /* shll2 r0 */
 goto L8c66be0e;
L8c66be0e: r[0]<<=2; /* shll2 r0 */
 goto L8c66be10;
L8c66be10: r[0]+=r[3]; /* add r3,r0 */
 goto L8c66be12;
L8c66be12: r[0]+=r[5]; /* add r5,r0 */
 goto L8c66be14;
L8c66be14: r[4]=r[0]; /* mov r0,r4 */
 goto L8c66be16;
L8c66be16: r[4]+=1u; /* add #0x1,r4 */
 goto L8c66be18;
L8c66be18: target=r[2];*pr=0x8c66be1cu;r[5]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66be1c;
L8c66be1c: r[2]=sx(m,r[15],1); /* mov.b @r15,r2 */
 goto L8c66be1e;
L8c66be1e: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c66be20;
L8c66be20: wr(m,r[3],r[2],1); /* mov.b r2,@r3 */
 goto L8c66be22;
L8c66be22: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66be24;
L8c66be24: if((*sr&1u))goto L8c66be2e; /* bt 0x8c66be2e */
 goto L8c66be26;
L8c66be26: r[2]=rd(m,0x8c66be50u,4); /* mov.l 0x8c66be50,r2 */
 goto L8c66be28;
L8c66be28: r[4]=rd(m,0x8c66be4cu,4); /* mov.l 0x8c66be4c,r4 */
 goto L8c66be2a;
L8c66be2a: target=r[2];*pr=0x8c66be2eu;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66be2e;
L8c66be2e: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66be30;
L8c66be30: r[15]+=8u; /* add #0x8,r15 */
 goto L8c66be32;
L8c66be32: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66be34;
L8c66be34: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
