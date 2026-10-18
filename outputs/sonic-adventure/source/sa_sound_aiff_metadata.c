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
/* Static C recovery of retail Tikal helper 8c66983e..8c6698d6.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_aiff_metadata(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66983e;
L8c66983e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c669840;
L8c669840: r[15]+=4294967268u; /* add #-0x1c,r15 */
 goto L8c669842;
L8c669842: r[3]=sx(m,0x8c66992au,2); /* mov.w 0x8c66992a,r3 */
 goto L8c669844;
L8c669844: wr(m,r[15]+0x10u,r[4],4); /* mov.l r4,@(0x10,r15) */
 goto L8c669846;
L8c669846: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[3]); /* cmp/ge r3,r5 */
 goto L8c669848;
L8c669848: wr(m,r[15]+0x14u,r[6],4); /* mov.l r6,@(0x14,r15) */
 goto L8c66984a;
L8c66984a: take=(*sr&1u);wr(m,r[15]+0x18u,r[7],4);if(take)goto L8c669858;goto L8c66984e; /* bt/s 0x8c669858 */
 goto L8c66984e;
L8c66984e: r[1]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r1 */
 goto L8c669850;
L8c669850: r[2]=0u; /* mov #0x0,r2 */
 goto L8c669852;
L8c669852: wr(m,r[1],r[2],2); /* mov.w r2,@r1 */
 goto L8c669854;
L8c669854: goto L8c669884; /* bra 0x8c669884 */
 goto L8c669858;
L8c669858: r[2]=r[15]; /* mov r15,r2 */
 goto L8c66985a;
L8c66985a: r[2]+=12u; /* add #0xc,r2 */
 goto L8c66985c;
L8c66985c: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c66985e;
L8c66985e: r[6]=r[15]; /* mov r15,r6 */
 goto L8c669860;
L8c669860: r[5]=r[15]; /* mov r15,r5 */
 goto L8c669862;
L8c669862: r[7]=r[15]; /* mov r15,r7 */
 goto L8c669864;
L8c669864: r[5]+=4u; /* add #0x4,r5 */
 goto L8c669866;
L8c669866: r[7]+=12u; /* add #0xc,r7 */
 goto L8c669868;
L8c669868: r[6]+=8u; /* add #0x8,r6 */
 goto L8c66986a;
L8c66986a: *pr=0x8c66986eu;r[4]=rd(m,r[15]+0x14u,4);status=call(ctx,0x8c669528u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c669528 */
 goto L8c66986e;
L8c66986e: r[4]=r[0]; /* mov r0,r4 */
 goto L8c669870;
L8c669870: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c669872;
L8c669872: take=(*sr&1u);r[15]+=4u;if(take)goto L8c669884;goto L8c669876; /* bt/s 0x8c669884 */
 goto L8c669876;
L8c669876: r[3]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r3 */
 goto L8c669878;
L8c669878: r[2]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r2 */
 goto L8c66987a;
L8c66987a: r[4]-=r[3]; /* sub r3,r4 */
 goto L8c66987c;
L8c66987c: wr(m,r[2],r[4],2); /* mov.w r4,@r2 */
 goto L8c66987e;
L8c66987e: r[4]=(uint32_t)(int32_t)(int16_t)r[4]; /* exts.w r4,r4 */
 goto L8c669880;
L8c669880: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c669882;
L8c669882: if((*sr&1u))goto L8c66988c; /* bt 0x8c66988c */
 goto L8c669884;
L8c669884: r[15]+=28u; /* add #0x1c,r15 */
 goto L8c669886;
L8c669886: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c669888;
L8c669888: r[0]=4294967295u;return 0; /* rts */
L8c66988c: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c66988e;
L8c66988e: r[2]=rd(m,r[15]+0x2cu,4); /* mov.l @(0x2c,r15),r2 */
 goto L8c669890;
L8c669890: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c669892;
L8c669892: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c669894;
L8c669894: r[2]=rd(m,r[15]+0x28u,4); /* mov.l @(0x28,r15),r2 */
 goto L8c669896;
L8c669896: wr(m,r[2],r[3],1); /* mov.b r3,@r2 */
 goto L8c669898;
L8c669898: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c66989a;
L8c66989a: r[2]=rd(m,r[15]+0x20u,4); /* mov.l @(0x20,r15),r2 */
 goto L8c66989c;
L8c66989c: wr(m,r[2],r[3],1); /* mov.b r3,@r2 */
 goto L8c66989e;
L8c66989e: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c6698a0;
L8c6698a0: r[2]=rd(m,r[15]+0x30u,4); /* mov.l @(0x30,r15),r2 */
 goto L8c6698a2;
L8c6698a2: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c6698a4;
L8c6698a4: r[3]=4294967295u; /* mov #-0x1,r3 */
 goto L8c6698a6;
L8c6698a6: r[2]=rd(m,r[15]+0x18u,4); /* mov.l @(0x18,r15),r2 */
 goto L8c6698a8;
L8c6698a8: wr(m,r[2],r[3],1); /* mov.b r3,@r2 */
 goto L8c6698aa;
L8c6698aa: r[3]=rd(m,r[15]+0x28u,4); /* mov.l @(0x28,r15),r3 */
 goto L8c6698ac;
L8c6698ac: r[2]=rd(m,r[15]+0x24u,4); /* mov.l @(0x24,r15),r2 */
 goto L8c6698ae;
L8c6698ae: r[1]=sx(m,r[3],1); /* mov.b @r3,r1 */
 goto L8c6698b0;
L8c6698b0: r[3]=rd(m,r[15]+0x20u,4); /* mov.l @(0x20,r15),r3 */
 goto L8c6698b2;
L8c6698b2: r[3]=sx(m,r[3],1); /* mov.b @r3,r3 */
 goto L8c6698b4;
L8c6698b4: *macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[1]); /* muls.w r3,r1 */
 goto L8c6698b6;
L8c6698b6: r[1]=*macl; /* sts MACL,r1 */
 goto L8c6698b8;
L8c6698b8: *sr=(*sr&~1u)|((int32_t)r[1]>=0); /* cmp/pz r1 */
 goto L8c6698ba;
L8c6698ba: if((*sr&1u))goto L8c6698be; /* bt 0x8c6698be */
 goto L8c6698bc;
L8c6698bc: r[1]+=7u; /* add #0x7,r1 */
 goto L8c6698be;
L8c6698be: *sr=(*sr&~1u)|(r[1]&1u);r[1]=(r[1]>>1)|(r[1]&0x80000000u); /* shar r1 */
 goto L8c6698c0;
L8c6698c0: *sr=(*sr&~1u)|(r[1]&1u);r[1]=(r[1]>>1)|(r[1]&0x80000000u); /* shar r1 */
 goto L8c6698c2;
L8c6698c2: *sr=(*sr&~1u)|(r[1]&1u);r[1]=(r[1]>>1)|(r[1]&0x80000000u); /* shar r1 */
 goto L8c6698c4;
L8c6698c4: wr(m,r[2],r[1],1); /* mov.b r1,@r2 */
 goto L8c6698c6;
L8c6698c6: r[3]=1u; /* mov #0x1,r3 */
 goto L8c6698c8;
L8c6698c8: r[2]=rd(m,r[15]+0x34u,4); /* mov.l @(0x34,r15),r2 */
 goto L8c6698ca;
L8c6698ca: r[0]=0u; /* mov #0x0,r0 */
 goto L8c6698cc;
L8c6698cc: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c6698ce;
L8c6698ce: r[15]+=28u; /* add #0x1c,r15 */
 goto L8c6698d0;
L8c6698d0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6698d2;
L8c6698d2: return 0; /* rts */
}
