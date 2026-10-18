#include "fpu_cleanup_transfer.h"
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
/* Static C recovery of retail Tikal helper 8c668bee..8c668cca.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_work_create(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c668bee;
L8c668bee: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c668bf0;
L8c668bf0: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c668bf2;
L8c668bf2: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c668bf4;
L8c668bf4: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c668bf6;
L8c668bf6: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c668bf8;
L8c668bf8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c668bfa;
L8c668bfa: r[0]=sx(m,0x8c668c3cu,2); /* mov.w 0x8c668c3c,r0 */
 goto L8c668bfc;
L8c668bfc: r[15]+=r[0]; /* add r0,r15 */
 goto L8c668bfe;
L8c668bfe: r[10]=r[15]; /* mov r15,r10 */
 goto L8c668c00;
L8c668c00: r[3]=rd(m,0x8c668c48u,4); /* mov.l 0x8c668c48,r3 */
 goto L8c668c02;
L8c668c02: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c668c04;
L8c668c04: r[5]=0u; /* mov #0x0,r5 */
 goto L8c668c06;
L8c668c06: wr(m,r[15],r[6],4); /* mov.l r6,@r15 */
 goto L8c668c08;
L8c668c08: r[13]=r[4]; /* mov r4,r13 */
 goto L8c668c0a;
L8c668c0a: r[6]=sx(m,0x8c668c3eu,2); /* mov.w 0x8c668c3e,r6 */
 goto L8c668c0c;
L8c668c0c: r[10]+=8u; /* add #0x8,r10 */
 goto L8c668c0e;
L8c668c0e: r[11]=r[7]; /* mov r7,r11 */
 goto L8c668c10;
L8c668c10: target=r[3];*pr=0x8c668c14u;r[4]=r[10];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668c14;
L8c668c14: r[0]=sx(m,0x8c668c32u,2); /* mov.w 0x8c668c32,r0 */
 goto L8c668c16;
L8c668c16: r[2]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r2 */
 goto L8c668c18;
L8c668c18: r[0]+=4u; /* add #0x4,r0 */
 goto L8c668c1a;
L8c668c1a: r[3]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r3 */
 goto L8c668c1c;
L8c668c1c: *sr=(*sr&~1u)|(r[2]==r[3]); /* cmp/eq r2,r3 */
 goto L8c668c1e;
L8c668c1e: if(!(*sr&1u))goto L8c668c24; /* bf 0x8c668c24 */
 goto L8c668c20;
L8c668c20: r[0]=4294967295u;goto L8c668cb8; /* bra 0x8c668cb8 */
 goto L8c668c24;
L8c668c24: r[14]=sx(m,0x8c668c36u,2); /* mov.w 0x8c668c36,r14 */
 goto L8c668c26;
L8c668c26: r[4]=0u; /* mov #0x0,r4 */
 goto L8c668c28;
L8c668c28: r[12]=r[4]; /* mov r4,r12 */
 goto L8c668c2a;
L8c668c2a: r[14]+=r[13];goto L8c668c62; /* bra 0x8c668c62 */
 goto L8c668c5c;
L8c668c5c: r[2]=sx(m,0x8c668d66u,2); /* mov.w 0x8c668d66,r2 */
 goto L8c668c5e;
L8c668c5e: r[12]+=1u; /* add #0x1,r12 */
 goto L8c668c60;
L8c668c60: r[14]+=r[2]; /* add r2,r14 */
 goto L8c668c62;
L8c668c62: r[3]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r3 */
 goto L8c668c64;
L8c668c64: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c668c66;
L8c668c66: if(!(*sr&1u))goto L8c668c5c; /* bf 0x8c668c5c */
 goto L8c668c68;
L8c668c68: r[0]=sx(m,0x8c668d68u,2); /* mov.w 0x8c668d68,r0 */
 goto L8c668c6a;
L8c668c6a: r[2]=1u; /* mov #0x1,r2 */
 goto L8c668c6c;
L8c668c6c: r[6]=sx(m,0x8c668d6cu,2); /* mov.w 0x8c668d6c,r6 */
 goto L8c668c6e;
L8c668c6e: r[11]>>=2; /* shlr2 r11 */
 goto L8c668c70;
L8c668c70: wr(m,r[14]+0x8u,r[4],4); /* mov.l r4,@(0x8,r14) */
 goto L8c668c72;
L8c668c72: wr(m,r[14]+0xcu,r[2],4); /* mov.l r2,@(0xc,r14) */
 goto L8c668c74;
L8c668c74: r[5]=r[6]; /* mov r6,r5 */
 goto L8c668c76;
L8c668c76: r[3]=rd(m,r[13]+r[0],4); /* mov.l @(r0,r13),r3 */
 goto L8c668c78;
L8c668c78: r[5]+=4u; /* add #0x4,r5 */
 goto L8c668c7a;
L8c668c7a: r[0]=sx(m,0x8c668d6au,2); /* mov.w 0x8c668d6a,r0 */
 goto L8c668c7c;
L8c668c7c: r[5]+=r[13]; /* add r13,r5 */
 goto L8c668c7e;
L8c668c7e: wr(m,r[14]+0x14u,r[3],4); /* mov.l r3,@(0x14,r14) */
 goto L8c668c80;
L8c668c80: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c668c82;
L8c668c82: r[3]=rd(m,0x8c668d7cu,4); /* mov.l 0x8c668d7c,r3 */
 goto L8c668c84;
L8c668c84: wr(m,r[14],r[2],4); /* mov.l r2,@r14 */
 goto L8c668c86;
L8c668c86: wr(m,r[14]+0x4u,r[11],4); /* mov.l r11,@(0x4,r14) */
 goto L8c668c88;
L8c668c88: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c668c8a;
L8c668c8a: r[0]+=4u; /* add #0x4,r0 */
 goto L8c668c8c;
L8c668c8c: wr(m,r[14]+0x10u,r[4],4); /* mov.l r4,@(0x10,r14) */
 goto L8c668c8e;
L8c668c8e: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c668c90;
L8c668c90: r[4]=r[14]; /* mov r14,r4 */
 goto L8c668c92;
L8c668c92: target=r[3];*pr=0x8c668c96u;r[4]+=24u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668c96;
L8c668c96: r[2]=rd(m,0x8c668d80u,4); /* mov.l 0x8c668d80,r2 */
 goto L8c668c98;
L8c668c98: target=r[2];*pr=0x8c668c9cu;r[4]=r[10];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c668c9c;
L8c668c9c: r[3]=rd(m,0x8c668d7cu,4); /* mov.l 0x8c668d7c,r3 */
 goto L8c668c9e;
L8c668c9e: r[4]=r[14]; /* mov r14,r4 */
 goto L8c668ca0;
L8c668ca0: r[6]=sx(m,0x8c668d6cu,2); /* mov.w 0x8c668d6c,r6 */
 goto L8c668ca2;
L8c668ca2: r[5]=r[10]; /* mov r10,r5 */
 goto L8c668ca4;
L8c668ca4: target=r[3];*pr=0x8c668ca8u;r[4]+=24u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668ca8;
L8c668ca8: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c668caa;
L8c668caa: r[11]+=4294967295u; /* add #-0x1,r11 */
 goto L8c668cac;
L8c668cac: r[11]<<=2; /* shll2 r11 */
 goto L8c668cae;
L8c668cae: r[0]=r[12]; /* mov r12,r0 */
 goto L8c668cb0;
L8c668cb0: r[11]+=r[2]; /* add r2,r11 */
 goto L8c668cb2;
L8c668cb2: wr(m,r[14]+0x18u,r[11],4); /* mov.l r11,@(0x18,r14) */
 goto L8c668cb4;
L8c668cb4: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c668cb6;
L8c668cb6: wr(m,r[14]+0x20u,r[3],4); /* mov.l r3,@(0x20,r14) */
 goto L8c668cb8;
L8c668cb8: r[1]=sx(m,0x8c668d6eu,2); /* mov.w 0x8c668d6e,r1 */
 goto L8c668cba;
L8c668cba: r[15]+=r[1]; /* add r1,r15 */
 goto L8c668cbc;
L8c668cbc: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c668cbe;
L8c668cbe: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c668cc0;
L8c668cc0: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c668cc2;
L8c668cc2: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c668cc4;
L8c668cc4: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c668cc6;
L8c668cc6: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
