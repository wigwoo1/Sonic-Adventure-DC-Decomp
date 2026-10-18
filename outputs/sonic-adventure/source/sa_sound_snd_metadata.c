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
/* Retail SND metadata wrapper, 8c669e6a..8c669f2e.
 * Input size and returned 16-bit offset use signed acceptance checks.
 * Parser and quotient resolve to recovered native functions in verification. */
SA_STAGE_EXPORT int sa_sound_snd_metadata(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c669e6a;
L8c669e6a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c669e6c;
L8c669e6c: r[15]+=4294967268u; /* add #-0x1c,r15 */
 goto L8c669e6e;
L8c669e6e: r[3]=8u; /* mov #0x8,r3 */
 goto L8c669e70;
L8c669e70: wr(m,r[15]+0x14u,r[4],4); /* mov.l r4,@(0x14,r15) */
 goto L8c669e72;
L8c669e72: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[3]); /* cmp/ge r3,r5 */
 goto L8c669e74;
L8c669e74: wr(m,r[15]+0x10u,r[6],4); /* mov.l r6,@(0x10,r15) */
 goto L8c669e76;
L8c669e76: take=(*sr&1u);wr(m,r[15]+0x18u,r[7],4);if(take)goto L8c669e84;goto L8c669e7a; /* bt/s 0x8c669e84 */
 goto L8c669e7a;
L8c669e7a: r[1]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r1 */
 goto L8c669e7c;
L8c669e7c: r[2]=0u; /* mov #0x0,r2 */
 goto L8c669e7e;
L8c669e7e: wr(m,r[1],r[2],2); /* mov.w r2,@r1 */
 goto L8c669e80;
L8c669e80: goto L8c669eb6; /* bra 0x8c669eb6 */
 goto L8c669e84;
L8c669e84: r[2]=rd(m,r[15]+0x38u,4); /* mov.l @(0x38,r15),r2 */
 goto L8c669e86;
L8c669e86: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c669e88;
L8c669e88: r[3]=r[15]; /* mov r15,r3 */
 goto L8c669e8a;
L8c669e8a: r[3]+=16u; /* add #0x10,r3 */
 goto L8c669e8c;
L8c669e8c: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c669e8e;
L8c669e8e: r[2]=r[15]; /* mov r15,r2 */
 goto L8c669e90;
L8c669e90: r[2]+=16u; /* add #0x10,r2 */
 goto L8c669e92;
L8c669e92: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c669e94;
L8c669e94: r[6]=r[15]; /* mov r15,r6 */
 goto L8c669e96;
L8c669e96: r[7]=r[15]; /* mov r15,r7 */
 goto L8c669e98;
L8c669e98: r[6]+=12u; /* add #0xc,r6 */
 goto L8c669e9a;
L8c669e9a: r[7]+=16u; /* add #0x10,r7 */
 goto L8c669e9c;
L8c669e9c: *pr=0x8c669ea0u;r[4]=rd(m,r[15]+0x20u,4);status=call(ctx,0x8c669c14u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c669c14 */
 goto L8c669ea0;
L8c669ea0: r[4]=r[0]; /* mov r0,r4 */
 goto L8c669ea2;
L8c669ea2: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c669ea4;
L8c669ea4: take=(*sr&1u);r[15]+=12u;if(take)goto L8c669eb6;goto L8c669ea8; /* bt/s 0x8c669eb6 */
 goto L8c669ea8;
L8c669ea8: r[3]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r3 */
 goto L8c669eaa;
L8c669eaa: r[2]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r2 */
 goto L8c669eac;
L8c669eac: r[4]-=r[3]; /* sub r3,r4 */
 goto L8c669eae;
L8c669eae: wr(m,r[2],r[4],2); /* mov.w r4,@r2 */
 goto L8c669eb0;
L8c669eb0: r[4]=(uint32_t)(int32_t)(int16_t)r[4]; /* exts.w r4,r4 */
 goto L8c669eb2;
L8c669eb2: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c669eb4;
L8c669eb4: if((*sr&1u))goto L8c669ee4; /* bt 0x8c669ee4 */
 goto L8c669eb6;
L8c669eb6: r[15]+=28u; /* add #0x1c,r15 */
 goto L8c669eb8;
L8c669eb8: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c669eba;
L8c669eba: r[0]=4294967295u;return 0; /* rts */
L8c669ee4: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c669ee6;
L8c669ee6: r[2]=rd(m,r[15]+0x2cu,4); /* mov.l @(0x2c,r15),r2 */
 goto L8c669ee8;
L8c669ee8: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c669eea;
L8c669eea: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c669eec;
L8c669eec: r[2]=rd(m,r[15]+0x28u,4); /* mov.l @(0x28,r15),r2 */
 goto L8c669eee;
L8c669eee: wr(m,r[2],r[3],1); /* mov.b r3,@r2 */
 goto L8c669ef0;
L8c669ef0: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c669ef2;
L8c669ef2: r[2]=rd(m,r[15]+0x20u,4); /* mov.l @(0x20,r15),r2 */
 goto L8c669ef4;
L8c669ef4: wr(m,r[2],r[3],1); /* mov.b r3,@r2 */
 goto L8c669ef6;
L8c669ef6: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c669ef8;
L8c669ef8: r[2]=rd(m,r[15]+0x30u,4); /* mov.l @(0x30,r15),r2 */
 goto L8c669efa;
L8c669efa: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c669efc;
L8c669efc: r[3]=4294967295u; /* mov #-0x1,r3 */
 goto L8c669efe;
L8c669efe: r[2]=rd(m,r[15]+0x18u,4); /* mov.l @(0x18,r15),r2 */
 goto L8c669f00;
L8c669f00: wr(m,r[2],r[3],1); /* mov.b r3,@r2 */
 goto L8c669f02;
L8c669f02: r[3]=rd(m,r[15]+0x28u,4); /* mov.l @(0x28,r15),r3 */
 goto L8c669f04;
L8c669f04: r[2]=rd(m,r[15]+0x24u,4); /* mov.l @(0x24,r15),r2 */
 goto L8c669f06;
L8c669f06: r[1]=sx(m,r[3],1); /* mov.b @r3,r1 */
 goto L8c669f08;
L8c669f08: r[3]=rd(m,r[15]+0x20u,4); /* mov.l @(0x20,r15),r3 */
 goto L8c669f0a;
L8c669f0a: r[3]=sx(m,r[3],1); /* mov.b @r3,r3 */
 goto L8c669f0c;
L8c669f0c: *macl=(uint32_t)((int32_t)(int16_t)r[3]*(int32_t)(int16_t)r[1]); /* muls.w r3,r1 */
 goto L8c669f0e;
L8c669f0e: r[1]=*macl; /* sts MACL,r1 */
 goto L8c669f10;
L8c669f10: *sr=(*sr&~1u)|((int32_t)r[1]>=0); /* cmp/pz r1 */
 goto L8c669f12;
L8c669f12: if((*sr&1u))goto L8c669f16; /* bt 0x8c669f16 */
 goto L8c669f14;
L8c669f14: r[1]+=7u; /* add #0x7,r1 */
 goto L8c669f16;
L8c669f16: *sr=(*sr&~1u)|(r[1]&1u);r[1]=(r[1]>>1)|(r[1]&0x80000000u); /* shar r1 */
 goto L8c669f18;
L8c669f18: *sr=(*sr&~1u)|(r[1]&1u);r[1]=(r[1]>>1)|(r[1]&0x80000000u); /* shar r1 */
 goto L8c669f1a;
L8c669f1a: *sr=(*sr&~1u)|(r[1]&1u);r[1]=(r[1]>>1)|(r[1]&0x80000000u); /* shar r1 */
 goto L8c669f1c;
L8c669f1c: wr(m,r[2],r[1],1); /* mov.b r1,@r2 */
 goto L8c669f1e;
L8c669f1e: r[3]=1u; /* mov #0x1,r3 */
 goto L8c669f20;
L8c669f20: r[2]=rd(m,r[15]+0x34u,4); /* mov.l @(0x34,r15),r2 */
 goto L8c669f22;
L8c669f22: r[0]=0u; /* mov #0x0,r0 */
 goto L8c669f24;
L8c669f24: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c669f26;
L8c669f26: r[15]+=28u; /* add #0x1c,r15 */
 goto L8c669f28;
L8c669f28: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c669f2a;
L8c669f2a: return 0; /* rts */
}
