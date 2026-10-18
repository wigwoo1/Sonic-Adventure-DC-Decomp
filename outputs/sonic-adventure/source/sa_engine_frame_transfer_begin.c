/* Sonic Adventure USA Rev A frame transfer begin, 0x8c666c2c. */
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
/* Static C recovery of retail Tikal helper 8c666c2c..8c666ce2.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_frame_transfer_begin(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c666c2c;
L8c666c2c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c666c2e;
L8c666c2e: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c666c30;
L8c666c30: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c666c32;
L8c666c32: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c666c34;
L8c666c34: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c666c36;
L8c666c36: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c666c38;
L8c666c38: r[0]=*sr; /* stc SR,r0 */
 goto L8c666c3a;
L8c666c3a: r[3]=sx(m,0x8c666ce2u,2); /* mov.w 0x8c666ce2,r3 */
 goto L8c666c3c;
L8c666c3c: r[13]=r[4]; /* mov r4,r13 */
 goto L8c666c3e;
L8c666c3e: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c666c40;
L8c666c40: r[0]>>=2; /* shlr2 r0 */
 goto L8c666c42;
L8c666c42: r[0]>>=2; /* shlr2 r0 */
 goto L8c666c44;
L8c666c44: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c666c46;
L8c666c46: wr(m,r[15]+0x8u,r[0],4); /* mov.l r0,@(0x8,r15) */
 goto L8c666c48;
L8c666c48: r[0]=*sr; /* stc SR,r0 */
 goto L8c666c4a;
L8c666c4a: r[0]&=r[3]; /* and r3,r0 */
 goto L8c666c4c;
L8c666c4c: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c666c4e;
L8c666c4e: *sr=r[0]; /* ldc r0,SR */
 goto L8c666c50;
L8c666c50: r[11]=rd(m,0x8c666ce8u,4); /* mov.l 0x8c666ce8,r11 */
 goto L8c666c52;
L8c666c52: r[2]=rd(m,r[11],4); /* mov.l @r11,r2 */
 goto L8c666c54;
L8c666c54: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c666c56;
L8c666c56: if(!(*sr&1u))goto L8c666c5e; /* bf 0x8c666c5e */
 goto L8c666c58;
L8c666c58: r[14]=rd(m,0x8c666cecu,4); /* mov.l 0x8c666cec,r14 */
 goto L8c666c5a;
L8c666c5a: goto L8c666cb4; /* bra 0x8c666cb4 */
 goto L8c666c5e;
L8c666c5e: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c666c60;
L8c666c60: if(!(*sr&1u))goto L8c666c68; /* bf 0x8c666c68 */
 goto L8c666c62;
L8c666c62: r[14]=rd(m,0x8c666cf0u,4); /* mov.l 0x8c666cf0,r14 */
 goto L8c666c64;
L8c666c64: goto L8c666cb4; /* bra 0x8c666cb4 */
 goto L8c666c68;
L8c666c68: r[3]=rd(m,0x8c666cf4u,4); /* mov.l 0x8c666cf4,r3 */
 goto L8c666c6a;
L8c666c6a: r[5]=r[15]; /* mov r15,r5 */
 goto L8c666c6c;
L8c666c6c: target=r[3];*pr=0x8c666c70u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666c70;
L8c666c70: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c666c72;
L8c666c72: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c666c74;
L8c666c74: if(!(*sr&1u))goto L8c666c7c; /* bf 0x8c666c7c */
 goto L8c666c76;
L8c666c76: r[14]=rd(m,0x8c666cf8u,4); /* mov.l 0x8c666cf8,r14 */
 goto L8c666c78;
L8c666c78: goto L8c666cb4; /* bra 0x8c666cb4 */
 goto L8c666c7c;
L8c666c7c: r[3]=rd(m,0x8c666cfcu,4); /* mov.l 0x8c666cfc,r3 */
 goto L8c666c7e;
L8c666c7e: r[2]=0u; /* mov #0x0,r2 */
 goto L8c666c80;
L8c666c80: r[12]=r[13]; /* mov r13,r12 */
 goto L8c666c82;
L8c666c82: r[5]=2u; /* mov #0x2,r5 */
 goto L8c666c84;
L8c666c84: wr(m,r[12]+0x18u,r[2],4); /* mov.l r2,@(0x18,r12) */
 goto L8c666c86;
L8c666c86: target=r[3];*pr=0x8c666c8au;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666c8a;
L8c666c8a: r[14]=r[0]; /* mov r0,r14 */
 goto L8c666c8c;
L8c666c8c: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c666c8e;
L8c666c8e: if(!(*sr&1u))goto L8c666cb4; /* bf 0x8c666cb4 */
 goto L8c666c90;
L8c666c90: r[0]=sx(m,0x8c666ce4u,2); /* mov.w 0x8c666ce4,r0 */
 goto L8c666c92;
L8c666c92: r[2]=rd(m,r[11],4); /* mov.l @r11,r2 */
 goto L8c666c94;
L8c666c94: r[7]=rd(m,0x8c666d00u,4); /* mov.l 0x8c666d00,r7 */
 goto L8c666c96;
L8c666c96: wr(m,r[2]+r[0],r[13],4); /* mov.l r13,@(r0,r2) */
 goto L8c666c98;
L8c666c98: r[0]+=4294967288u; /* add #-0x8,r0 */
 goto L8c666c9a;
L8c666c9a: r[3]=rd(m,r[11],4); /* mov.l @r11,r3 */
 goto L8c666c9c;
L8c666c9c: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c666c9e;
L8c666c9e: r[2]+=1u; /* add #0x1,r2 */
 goto L8c666ca0;
L8c666ca0: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c666ca2;
L8c666ca2: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c666ca4;
L8c666ca4: r[5]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r5 */
 goto L8c666ca6;
L8c666ca6: r[3]=rd(m,0x8c666d04u,4); /* mov.l 0x8c666d04,r3 */
 goto L8c666ca8;
L8c666ca8: r[6]=rd(m,r[12]+0xcu,4); /* mov.l @(0xc,r12),r6 */
 goto L8c666caa;
L8c666caa: target=r[3];*pr=0x8c666caeu;r[4]=rd(m,r[12]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666cae;
L8c666cae: r[15]+=4u; /* add #0x4,r15 */
 goto L8c666cb0;
L8c666cb0: r[14]=r[0]; /* mov r0,r14 */
 goto L8c666cb2;
L8c666cb2: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c666cb4;
L8c666cb4: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c666cb6;
L8c666cb6: if((*sr&1u))goto L8c666cc0; /* bt 0x8c666cc0 */
 goto L8c666cb8;
L8c666cb8: r[3]=rd(m,0x8c666d0cu,4); /* mov.l 0x8c666d0c,r3 */
 goto L8c666cba;
L8c666cba: r[4]=rd(m,0x8c666d08u,4); /* mov.l 0x8c666d08,r4 */
 goto L8c666cbc;
L8c666cbc: target=r[3];*pr=0x8c666cc0u;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666cc0;
L8c666cc0: r[0]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r0 */
 goto L8c666cc2;
L8c666cc2: r[2]=*sr; /* stc SR,r2 */
 goto L8c666cc4;
L8c666cc4: r[3]=sx(m,0x8c666ce2u,2); /* mov.w 0x8c666ce2,r3 */
 goto L8c666cc6;
L8c666cc6: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c666cc8;
L8c666cc8: r[0]<<=2; /* shll2 r0 */
 goto L8c666cca;
L8c666cca: r[2]&=r[3]; /* and r3,r2 */
 goto L8c666ccc;
L8c666ccc: r[0]<<=2; /* shll2 r0 */
 goto L8c666cce;
L8c666cce: r[0]|=r[2]; /* or r2,r0 */
 goto L8c666cd0;
L8c666cd0: *sr=r[0]; /* ldc r0,SR */
 goto L8c666cd2;
L8c666cd2: r[0]=r[14]; /* mov r14,r0 */
 goto L8c666cd4;
L8c666cd4: r[15]+=12u; /* add #0xc,r15 */
 goto L8c666cd6;
L8c666cd6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c666cd8;
L8c666cd8: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c666cda;
L8c666cda: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c666cdc;
L8c666cdc: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c666cde;
L8c666cde: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
