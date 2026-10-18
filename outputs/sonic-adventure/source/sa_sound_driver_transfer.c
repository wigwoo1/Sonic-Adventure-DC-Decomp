#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
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
/* Static C recovery of retail Tikal helper 8c643e6c..8c643f22.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_transfer(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c643e6c;
L8c643e6c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c643e6e;
L8c643e6e: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c643e70;
L8c643e70: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c643e72;
L8c643e72: r[12]=r[4]; /* mov r4,r12 */
 goto L8c643e74;
L8c643e74: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c643e76;
L8c643e76: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c643e78;
L8c643e78: take=!(*sr&1u);r[14]=0u;if(take)goto L8c643e82;goto L8c643e7c; /* bf/s 0x8c643e82 */
 goto L8c643e7c;
L8c643e7c: r[14]=rd(m,0x8c643f24u,4); /* mov.l 0x8c643f24,r14 */
 goto L8c643e7e;
L8c643e7e: goto L8c643efc; /* bra 0x8c643efc */
 goto L8c643e82;
L8c643e82: r[13]=rd(m,r[12]+0x8u,4); /* mov.l @(0x8,r12),r13 */
 goto L8c643e84;
L8c643e84: r[2]=rd(m,0x8c643f28u,4); /* mov.l 0x8c643f28,r2 */
 goto L8c643e86;
L8c643e86: r[3]=rd(m,r[13],4); /* mov.l @r13,r3 */
 goto L8c643e88;
L8c643e88: *sr=(*sr&~1u)|(r[2]==r[3]); /* cmp/eq r2,r3 */
 goto L8c643e8a;
L8c643e8a: if((*sr&1u))goto L8c643e92; /* bt 0x8c643e92 */
 goto L8c643e8c;
L8c643e8c: r[14]=rd(m,0x8c643f2cu,4); /* mov.l 0x8c643f2c,r14 */
 goto L8c643e8e;
L8c643e8e: goto L8c643efc; /* bra 0x8c643efc */
 goto L8c643e92;
L8c643e92: r[0]=sx(m,r[13]+0x4u,1); /* mov.b @(0x4,r13),r0 */
 goto L8c643e94;
L8c643e94: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c643e96;
L8c643e96: if(!(*sr&1u))goto L8c643ea0; /* bf 0x8c643ea0 */
 goto L8c643e98;
L8c643e98: r[0]=sx(m,r[13]+0x5u,1); /* mov.b @(0x5,r13),r0 */
 goto L8c643e9a;
L8c643e9a: r[3]=1u; /* mov #0x1,r3 */
 goto L8c643e9c;
L8c643e9c: *sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[3]); /* cmp/gt r3,r0 */
 goto L8c643e9e;
L8c643e9e: if(!(*sr&1u))goto L8c643ea6; /* bf 0x8c643ea6 */
 goto L8c643ea0;
L8c643ea0: r[14]=rd(m,0x8c643f30u,4); /* mov.l 0x8c643f30,r14 */
 goto L8c643ea2;
L8c643ea2: goto L8c643efc; /* bra 0x8c643efc */
 goto L8c643ea6;
L8c643ea6: r[2]=rd(m,0x8c643f34u,4); /* mov.l 0x8c643f34,r2 */
 goto L8c643ea8;
L8c643ea8: target=r[2];*pr=0x8c643eacu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c643eac;
L8c643eac: r[3]=rd(m,0x8c643f38u,4); /* mov.l 0x8c643f38,r3 */
 goto L8c643eae;
L8c643eae: target=r[3];*pr=0x8c643eb2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643eb2;
L8c643eb2: r[2]=rd(m,0x8c643f40u,4); /* mov.l 0x8c643f40,r2 */
 goto L8c643eb4;
L8c643eb4: r[4]=rd(m,0x8c643f3cu,4); /* mov.l 0x8c643f3c,r4 */
 goto L8c643eb6;
L8c643eb6: target=r[2];*pr=0x8c643ebau;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c643eba;
L8c643eba: r[5]=r[13]; /* mov r13,r5 */
 goto L8c643ebc;
L8c643ebc: r[2]=rd(m,0x8c643f3cu,4); /* mov.l 0x8c643f3c,r2 */
 goto L8c643ebe;
L8c643ebe: r[1]=rd(m,0x8c643f44u,4); /* mov.l 0x8c643f44,r1 */
 goto L8c643ec0;
L8c643ec0: r[3]=0u; /* mov #0x0,r3 */
 goto L8c643ec2;
L8c643ec2: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c643ec4;
L8c643ec4: r[7]=4294967295u; /* mov #-0x1,r7 */
 goto L8c643ec6;
L8c643ec6: r[6]=rd(m,r[13]+0x8u,4); /* mov.l @(0x8,r13),r6 */
 goto L8c643ec8;
L8c643ec8: r[5]+=32u; /* add #0x20,r5 */
 goto L8c643eca;
L8c643eca: target=r[1];*pr=0x8c643eceu;r[4]=rd(m,r[2],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c643ece;
L8c643ece: r[15]+=4u; /* add #0x4,r15 */
 goto L8c643ed0;
L8c643ed0: r[2]=rd(m,0x8c643f48u,4); /* mov.l 0x8c643f48,r2 */
 goto L8c643ed2;
L8c643ed2: r[5]=0u; /* mov #0x0,r5 */
 goto L8c643ed4;
L8c643ed4: r[3]=rd(m,0x8c643f3cu,4); /* mov.l 0x8c643f3c,r3 */
 goto L8c643ed6;
L8c643ed6: target=r[2];*pr=0x8c643edau;r[4]=rd(m,r[3],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c643eda;
L8c643eda: r[1]=rd(m,0x8c643f4cu,4); /* mov.l 0x8c643f4c,r1 */
 goto L8c643edc;
L8c643edc: target=r[1];*pr=0x8c643ee0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c643ee0;
L8c643ee0: r[4]=rd(m,0x8c643f50u,4); /* mov.l 0x8c643f50,r4 */
 goto L8c643ee2;
L8c643ee2: goto L8c643ee8; /* bra 0x8c643ee8 */
 goto L8c643ee6;
L8c643ee6: r[4]+=4294967295u; /* add #-0x1,r4 */
 goto L8c643ee8;
L8c643ee8: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c643eea;
L8c643eea: if(!(*sr&1u))goto L8c643ee6; /* bf 0x8c643ee6 */
 goto L8c643eec;
L8c643eec: r[1]=rd(m,0x8c643f58u,4); /* mov.l 0x8c643f58,r1 */
 goto L8c643eee;
L8c643eee: r[3]=1u; /* mov #0x1,r3 */
 goto L8c643ef0;
L8c643ef0: r[2]=rd(m,0x8c643f54u,4); /* mov.l 0x8c643f54,r2 */
 goto L8c643ef2;
L8c643ef2: target=r[1];*pr=0x8c643ef6u;wr(m,r[2],r[3],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c643ef6;
L8c643ef6: r[3]=rd(m,0x8c643f5cu,4); /* mov.l 0x8c643f5c,r3 */
 goto L8c643ef8;
L8c643ef8: target=r[3];*pr=0x8c643efcu;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643efc;
L8c643efc: r[3]=rd(m,0x8c643f3cu,4); /* mov.l 0x8c643f3c,r3 */
 goto L8c643efe;
L8c643efe: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c643f00;
L8c643f00: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c643f02;
L8c643f02: if((*sr&1u))goto L8c643f0a; /* bt 0x8c643f0a */
 goto L8c643f04;
L8c643f04: r[2]=rd(m,0x8c643f60u,4); /* mov.l 0x8c643f60,r2 */
 goto L8c643f06;
L8c643f06: target=r[2];*pr=0x8c643f0au;r[4]=rd(m,r[3],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c643f0a;
L8c643f0a: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c643f0c;
L8c643f0c: if((*sr&1u))goto L8c643f16; /* bt 0x8c643f16 */
 goto L8c643f0e;
L8c643f0e: r[3]=rd(m,0x8c643f68u,4); /* mov.l 0x8c643f68,r3 */
 goto L8c643f10;
L8c643f10: r[4]=rd(m,0x8c643f64u,4); /* mov.l 0x8c643f64,r4 */
 goto L8c643f12;
L8c643f12: target=r[3];*pr=0x8c643f16u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643f16;
L8c643f16: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c643f18;
L8c643f18: r[0]=r[14]; /* mov r14,r0 */
 goto L8c643f1a;
L8c643f1a: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c643f1c;
L8c643f1c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c643f1e;
L8c643f1e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
