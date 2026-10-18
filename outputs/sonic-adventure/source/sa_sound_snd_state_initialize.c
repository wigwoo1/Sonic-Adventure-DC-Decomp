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
/* Retail SND stream-state initializer, 8c669f2e..8c66a016.
 * Initializes format 4 and the parser-derived encoding selector.
 * Metadata, parser, quotient and fill resolve to recovered native functions. */
SA_STAGE_EXPORT int sa_sound_snd_state_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c669f2e;
L8c669f2e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c669f30;
L8c669f30: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c669f32;
L8c669f32: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c669f34;
L8c669f34: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c669f36;
L8c669f36: r[3]=r[15]; /* mov r15,r3 */
 goto L8c669f38;
L8c669f38: r[3]+=4u; /* add #0x4,r3 */
 goto L8c669f3a;
L8c669f3a: r[14]=r[4]; /* mov r4,r14 */
 goto L8c669f3c;
L8c669f3c: wr(m,r[15]+0x8u,r[5],4); /* mov.l r5,@(0x8,r15) */
 goto L8c669f3e;
L8c669f3e: wr(m,r[15]+0xcu,r[6],4); /* mov.l r6,@(0xc,r15) */
 goto L8c669f40;
L8c669f40: r[2]=r[14]; /* mov r14,r2 */
 goto L8c669f42;
L8c669f42: r[2]+=16u; /* add #0x10,r2 */
 goto L8c669f44;
L8c669f44: r[7]=r[14]; /* mov r14,r7 */
 goto L8c669f46;
L8c669f46: r[0]=1u; /* mov #0x1,r0 */
 goto L8c669f48;
L8c669f48: r[7]+=12u; /* add #0xc,r7 */
 goto L8c669f4a;
L8c669f4a: wr(m,r[14]+0x2u,r[0],2); /* mov.w r0,@(0x2,r14) */
 goto L8c669f4c;
L8c669f4c: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c669f4e;
L8c669f4e: r[3]=r[14]; /* mov r14,r3 */
 goto L8c669f50;
L8c669f50: r[3]+=24u; /* add #0x18,r3 */
 goto L8c669f52;
L8c669f52: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c669f54;
L8c669f54: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c669f56;
L8c669f56: r[2]=r[14]; /* mov r14,r2 */
 goto L8c669f58;
L8c669f58: r[3]=r[14]; /* mov r14,r3 */
 goto L8c669f5a;
L8c669f5a: r[2]+=20u; /* add #0x14,r2 */
 goto L8c669f5c;
L8c669f5c: r[3]+=14u; /* add #0xe,r3 */
 goto L8c669f5e;
L8c669f5e: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c669f60;
L8c669f60: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c669f62;
L8c669f62: r[2]=r[14]; /* mov r14,r2 */
 goto L8c669f64;
L8c669f64: r[2]+=15u; /* add #0xf,r2 */
 goto L8c669f66;
L8c669f66: r[3]=r[14]; /* mov r14,r3 */
 goto L8c669f68;
L8c669f68: r[3]+=13u; /* add #0xd,r3 */
 goto L8c669f6a;
L8c669f6a: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c669f6c;
L8c669f6c: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c669f6e;
L8c669f6e: r[6]=r[15]; /* mov r15,r6 */
 goto L8c669f70;
L8c669f70: r[5]=rd(m,r[15]+0x28u,4); /* mov.l @(0x28,r15),r5 */
 goto L8c669f72;
L8c669f72: r[6]+=28u; /* add #0x1c,r6 */
 goto L8c669f74;
L8c669f74: *pr=0x8c669f78u;r[4]=rd(m,r[15]+0x24u,4);status=call(ctx,0x8c669e6au,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c669e6a */
 goto L8c669f78;
L8c669f78: r[4]=r[0]; /* mov r0,r4 */
 goto L8c669f7a;
L8c669f7a: *sr=(*sr&~1u)|((int32_t)r[4]>=0); /* cmp/pz r4 */
 goto L8c669f7c;
L8c669f7c: take=(*sr&1u);r[15]+=28u;if(take)goto L8c669f84;goto L8c669f80; /* bt/s 0x8c669f84 */
 goto L8c669f80;
L8c669f80: r[0]=0u;goto L8c66a00c; /* bra 0x8c66a00c */
 goto L8c669f84;
L8c669f84: r[13]=0u; /* mov #0x0,r13 */
 goto L8c669f86;
L8c669f86: r[0]=r[13]; /* mov r13,r0 */
 goto L8c669f88;
L8c669f88: wr(m,r[14]+0x1cu,r[0],2); /* mov.w r0,@(0x1c,r14) */
 goto L8c669f8a;
L8c669f8a: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c669f8c;
L8c669f8c: wr(m,r[0]+r[14],r[13],2); /* mov.w r13,@(r0,r14) */
 goto L8c669f8e;
L8c669f8e: r[0]=72u; /* mov #0x48,r0 */
 goto L8c669f90;
L8c669f90: wr(m,r[0]+r[14],r[13],2); /* mov.w r13,@(r0,r14) */
 goto L8c669f92;
L8c669f92: r[0]=38u; /* mov #0x26,r0 */
 goto L8c669f94;
L8c669f94: wr(m,r[0]+r[14],r[13],2); /* mov.w r13,@(r0,r14) */
 goto L8c669f96;
L8c669f96: r[0]=36u; /* mov #0x24,r0 */
 goto L8c669f98;
L8c669f98: wr(m,r[0]+r[14],r[13],2); /* mov.w r13,@(r0,r14) */
 goto L8c669f9a;
L8c669f9a: r[1]=88u; /* mov #0x58,r1 */
 goto L8c669f9c;
L8c669f9c: r[1]+=r[14]; /* add r14,r1 */
 goto L8c669f9e;
L8c669f9e: wr(m,r[14]+0x34u,r[13],4); /* mov.l r13,@(0x34,r14) */
 goto L8c669fa0;
L8c669fa0: wr(m,r[14]+0x30u,r[13],4); /* mov.l r13,@(0x30,r14) */
 goto L8c669fa2;
L8c669fa2: wr(m,r[14]+0x2cu,r[13],4); /* mov.l r13,@(0x2c,r14) */
 goto L8c669fa4;
L8c669fa4: wr(m,r[14]+0x28u,r[13],4); /* mov.l r13,@(0x28,r14) */
 goto L8c669fa6;
L8c669fa6: wr(m,r[14]+0x20u,r[13],4); /* mov.l r13,@(0x20,r14) */
 goto L8c669fa8;
L8c669fa8: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c669faa;
L8c669faa: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c669fac;
L8c669fac: r[1]=92u; /* mov #0x5c,r1 */
 goto L8c669fae;
L8c669fae: r[0]=sx(m,r[14]+0xfu,1); /* mov.b @(0xf,r14),r0 */
 goto L8c669fb0;
L8c669fb0: r[1]+=r[14]; /* add r14,r1 */
 goto L8c669fb2;
L8c669fb2: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c669fb4;
L8c669fb4: r[0]=96u; /* mov #0x60,r0 */
 goto L8c669fb6;
L8c669fb6: r[3]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r3 */
 goto L8c669fb8;
L8c669fb8: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c669fba;
L8c669fba: r[0]=100u; /* mov #0x64,r0 */
 goto L8c669fbc;
L8c669fbc: r[2]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r2 */
 goto L8c669fbe;
L8c669fbe: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c669fc0;
L8c669fc0: r[0]=64u; /* mov #0x40,r0 */
 goto L8c669fc2;
L8c669fc2: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c669fc4;
L8c669fc4: r[0]=104u; /* mov #0x68,r0 */
 goto L8c669fc6;
L8c669fc6: r[5]=r[13]; /* mov r13,r5 */
 goto L8c669fc8;
L8c669fc8: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c669fca;
L8c669fca: r[0]=68u; /* mov #0x44,r0 */
 goto L8c669fcc;
L8c669fcc: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c669fce;
L8c669fce: r[0]=108u; /* mov #0x6c,r0 */
 goto L8c669fd0;
L8c669fd0: r[3]=r[14]; /* mov r14,r3 */
 goto L8c669fd2;
L8c669fd2: r[3]+=124u; /* add #0x7c,r3 */
 goto L8c669fd4;
L8c669fd4: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c669fd6;
L8c669fd6: r[0]=72u; /* mov #0x48,r0 */
 goto L8c669fd8;
L8c669fd8: r[2]=sx(m,r[0]+r[14],2); /* mov.w @(r0,r14),r2 */
 goto L8c669fda;
L8c669fda: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c669fdc;
L8c669fdc: r[4]=r[14]; /* mov r14,r4 */
 goto L8c669fde;
L8c669fde: wr(m,r[3],r[2],2); /* mov.w r2,@r3 */
 goto L8c669fe0;
L8c669fe0: r[2]=r[14]; /* mov r14,r2 */
 goto L8c669fe2;
L8c669fe2: r[3]=sx(m,r[0]+r[14],2); /* mov.w @(r0,r14),r3 */
 goto L8c669fe4;
L8c669fe4: r[0]=126u; /* mov #0x7e,r0 */
 goto L8c669fe6;
L8c669fe6: r[2]+=76u; /* add #0x4c,r2 */
 goto L8c669fe8;
L8c669fe8: wr(m,r[0]+r[14],r[3],2); /* mov.w r3,@(r0,r14) */
 goto L8c669fea;
L8c669fea: r[0]+=2u; /* add #0x2,r0 */
 goto L8c669fec;
L8c669fec: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c669fee;
L8c669fee: r[0]+=28u; /* add #0x1c,r0 */
 goto L8c669ff0;
L8c669ff0: wr(m,r[14]+r[0],r[13],4); /* mov.l r13,@(r0,r14) */
 goto L8c669ff2;
L8c669ff2: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c669ff4;
L8c669ff4: wr(m,r[14]+r[0],r[13],4); /* mov.l r13,@(r0,r14) */
 goto L8c669ff6;
L8c669ff6: r[0]+=16u; /* add #0x10,r0 */
 goto L8c669ff8;
L8c669ff8: r[3]=4u; /* mov #0x4,r3 */
 goto L8c669ffa;
L8c669ffa: wr(m,r[0]+r[14],r[3],2); /* mov.w r3,@(r0,r14) */
 goto L8c669ffc;
L8c669ffc: r[6]=r[3]; /* mov r3,r6 */
 goto L8c669ffe;
L8c669ffe: r[0]=sx(m,0x8c66a016u,2); /* mov.w 0x8c66a016,r0 */
 goto L8c66a000;
L8c66a000: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c66a002;
L8c66a002: wr(m,r[0]+r[14],r[2],2); /* mov.w r2,@(r0,r14) */
 goto L8c66a004;
L8c66a004: r[3]=rd(m,0x8c66a018u,4); /* mov.l 0x8c66a018,r3 */
 goto L8c66a006;
L8c66a006: target=r[3];*pr=0x8c66a00au;r[4]+=76u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66a00a;
L8c66a00a: r[0]=sx(m,r[15],2); /* mov.w @r15,r0 */
 goto L8c66a00c;
L8c66a00c: r[15]+=16u; /* add #0x10,r15 */
 goto L8c66a00e;
L8c66a00e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66a010;
L8c66a010: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66a012;
L8c66a012: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
