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
/* Retail buffer register configuration, 8c665e3c..8c665f42.
 * Register writes are delegated to the native graphics writer. */
SA_STAGE_EXPORT int sa_engine_render_buffer_registers(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c665e3c;
L8c665e3c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c665e3e;
L8c665e3e: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c665e40;
L8c665e40: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c665e42;
L8c665e42: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c665e44;
L8c665e44: r[13]=r[6]; /* mov r6,r13 */
 goto L8c665e46;
L8c665e46: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c665e48;
L8c665e48: r[13]<<=2; /* shll2 r13 */
 goto L8c665e4a;
L8c665e4a: r[0]=rd(m,0x8c665efcu,4); /* mov.l 0x8c665efc,r0 */
 goto L8c665e4c;
L8c665e4c: r[13]<<=2; /* shll2 r13 */
 goto L8c665e4e;
L8c665e4e: r[4]=sx(m,0x8c665ee0u,2); /* mov.w 0x8c665ee0,r4 */
 goto L8c665e50;
L8c665e50: r[14]=rd(m,0x8c665ef8u,4); /* mov.l 0x8c665ef8,r14 */
 goto L8c665e52;
L8c665e52: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1; /* shll r13 */
 goto L8c665e54;
L8c665e54: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c665e56;
L8c665e56: wr(m,r[15]+0x8u,r[6],4); /* mov.l r6,@(0x8,r15) */
 goto L8c665e58;
L8c665e58: target=r[14];*pr=0x8c665e5cu;r[5]=rd(m,r[13]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665e5c;
L8c665e5c: r[4]=sx(m,0x8c665ee2u,2); /* mov.w 0x8c665ee2,r4 */
 goto L8c665e5e;
L8c665e5e: r[0]=rd(m,0x8c665f00u,4); /* mov.l 0x8c665f00,r0 */
 goto L8c665e60;
L8c665e60: target=r[14];*pr=0x8c665e64u;r[5]=rd(m,r[13]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665e64;
L8c665e64: r[4]=sx(m,0x8c665ee4u,2); /* mov.w 0x8c665ee4,r4 */
 goto L8c665e66;
L8c665e66: r[0]=rd(m,0x8c665f04u,4); /* mov.l 0x8c665f04,r0 */
 goto L8c665e68;
L8c665e68: target=r[14];*pr=0x8c665e6cu;r[5]=rd(m,r[13]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665e6c;
L8c665e6c: r[4]=sx(m,0x8c665ee6u,2); /* mov.w 0x8c665ee6,r4 */
 goto L8c665e6e;
L8c665e6e: r[0]=rd(m,0x8c665f08u,4); /* mov.l 0x8c665f08,r0 */
 goto L8c665e70;
L8c665e70: target=r[14];*pr=0x8c665e74u;r[5]=rd(m,r[13]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665e74;
L8c665e74: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c665e76;
L8c665e76: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c665e78;
L8c665e78: r[5]+=4294967295u; /* add #-0x1,r5 */
 goto L8c665e7a;
L8c665e7a: r[4]=sx(m,0x8c665ee8u,2); /* mov.w 0x8c665ee8,r4 */
 goto L8c665e7c;
L8c665e7c: r[5]<<=16; /* shll16 r5 */
 goto L8c665e7e;
L8c665e7e: r[5]+=r[3]; /* add r3,r5 */
 goto L8c665e80;
L8c665e80: target=r[14];*pr=0x8c665e84u;r[5]+=4294967295u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665e84;
L8c665e84: r[3]=rd(m,0x8c665ef0u,4); /* mov.l 0x8c665ef0,r3 */
 goto L8c665e86;
L8c665e86: r[4]=3u; /* mov #0x3,r4 */
 goto L8c665e88;
L8c665e88: r[5]=rd(m,r[3],4); /* mov.l @r3,r5 */
 goto L8c665e8a;
L8c665e8a: r[1]=rd(m,0x8c665ef4u,4); /* mov.l 0x8c665ef4,r1 */
 goto L8c665e8c;
L8c665e8c: r[5]&=r[4]; /* and r4,r5 */
 goto L8c665e8e;
L8c665e8e: r[2]=rd(m,r[1],4); /* mov.l @r1,r2 */
 goto L8c665e90;
L8c665e90: r[2]&=r[4]; /* and r4,r2 */
 goto L8c665e92;
L8c665e92: r[0]=rd(m,0x8c665f0cu,4); /* mov.l 0x8c665f0c,r0 */
 goto L8c665e94;
L8c665e94: r[2]<<=2; /* shll2 r2 */
 goto L8c665e96;
L8c665e96: r[2]<<=2; /* shll2 r2 */
 goto L8c665e98;
L8c665e98: r[5]|=r[2]; /* or r2,r5 */
 goto L8c665e9a;
L8c665e9a: r[2]=rd(m,r[0],4); /* mov.l @r0,r2 */
 goto L8c665e9c;
L8c665e9c: r[2]&=r[4]; /* and r4,r2 */
 goto L8c665e9e;
L8c665e9e: r[2]<<=8; /* shll8 r2 */
 goto L8c665ea0;
L8c665ea0: r[5]|=r[2]; /* or r2,r5 */
 goto L8c665ea2;
L8c665ea2: r[2]=rd(m,0x8c665f10u,4); /* mov.l 0x8c665f10,r2 */
 goto L8c665ea4;
L8c665ea4: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c665ea6;
L8c665ea6: r[3]&=r[4]; /* and r4,r3 */
 goto L8c665ea8;
L8c665ea8: r[3]<<=8; /* shll8 r3 */
 goto L8c665eaa;
L8c665eaa: r[3]<<=2; /* shll2 r3 */
 goto L8c665eac;
L8c665eac: r[3]<<=2; /* shll2 r3 */
 goto L8c665eae;
L8c665eae: r[5]|=r[3]; /* or r3,r5 */
 goto L8c665eb0;
L8c665eb0: r[3]=rd(m,0x8c665f14u,4); /* mov.l 0x8c665f14,r3 */
 goto L8c665eb2;
L8c665eb2: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c665eb4;
L8c665eb4: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c665eb6;
L8c665eb6: take=!(*sr&1u);r[6]=1u;if(take)goto L8c665f20;goto L8c665eba; /* bf/s 0x8c665f20 */
 goto L8c665eba;
L8c665eba: r[1]=rd(m,0x8c665f18u,4); /* mov.l 0x8c665f18,r1 */
 goto L8c665ebc;
L8c665ebc: r[0]=rd(m,r[1],4); /* mov.l @r1,r0 */
 goto L8c665ebe;
L8c665ebe: r[6]&=r[0]; /* and r0,r6 */
 goto L8c665ec0;
L8c665ec0: r[0]=rd(m,0x8c665f1cu,4); /* mov.l 0x8c665f1c,r0 */
 goto L8c665ec2;
L8c665ec2: r[6]<<=16; /* shll16 r6 */
 goto L8c665ec4;
L8c665ec4: r[6]<<=2; /* shll2 r6 */
 goto L8c665ec6;
L8c665ec6: r[6]<<=2; /* shll2 r6 */
 goto L8c665ec8;
L8c665ec8: r[5]|=r[6]; /* or r6,r5 */
 goto L8c665eca;
L8c665eca: r[2]=rd(m,r[0],4); /* mov.l @r0,r2 */
 goto L8c665ecc;
L8c665ecc: r[4]&=r[2]; /* and r2,r4 */
 goto L8c665ece;
L8c665ece: r[4]<<=16; /* shll16 r4 */
 goto L8c665ed0;
L8c665ed0: r[5]|=r[4];goto L8c665f2a; /* bra 0x8c665f2a */
 goto L8c665f20;
L8c665f20: r[0]=rd(m,0x8c665fe8u,4); /* mov.l 0x8c665fe8,r0 */
 goto L8c665f22;
L8c665f22: r[1]=rd(m,r[0],4); /* mov.l @r0,r1 */
 goto L8c665f24;
L8c665f24: r[6]&=r[1]; /* and r1,r6 */
 goto L8c665f26;
L8c665f26: r[6]<<=16; /* shll16 r6 */
 goto L8c665f28;
L8c665f28: r[5]|=r[6]; /* or r6,r5 */
 goto L8c665f2a;
L8c665f2a: r[4]=sx(m,0x8c665fe0u,2); /* mov.w 0x8c665fe0,r4 */
 goto L8c665f2c;
L8c665f2c: target=r[14];*pr=0x8c665f30u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665f30;
L8c665f30: r[4]=sx(m,0x8c665fe2u,2); /* mov.w 0x8c665fe2,r4 */
 goto L8c665f32;
L8c665f32: r[5]=rd(m,0x8c665fecu,4); /* mov.l 0x8c665fec,r5 */
 goto L8c665f34;
L8c665f34: target=r[14];*pr=0x8c665f38u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665f38;
L8c665f38: r[15]+=12u; /* add #0xc,r15 */
 goto L8c665f3a;
L8c665f3a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c665f3c;
L8c665f3c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c665f3e;
L8c665f3e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
