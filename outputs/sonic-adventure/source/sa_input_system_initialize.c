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
/* Static C recovery of retail Tikal helper 8c647e16..8c647f10.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_system_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c647e16;
L8c647e16: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c647e18;
L8c647e18: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c647e1a;
L8c647e1a: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c647e1c;
L8c647e1c: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c647e1e;
L8c647e1e: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c647e20;
L8c647e20: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c647e22;
L8c647e22: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c647e24;
L8c647e24: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c647e26;
L8c647e26: r[10]=r[6]; /* mov r6,r10 */
 goto L8c647e28;
L8c647e28: r[6]=rd(m,0x8c647eb4u,4); /* mov.l 0x8c647eb4,r6 */
 goto L8c647e2a;
L8c647e2a: r[3]=rd(m,0x8c647eb0u,4); /* mov.l 0x8c647eb0,r3 */
 goto L8c647e2c;
L8c647e2c: r[12]=r[5]; /* mov r5,r12 */
 goto L8c647e2e;
L8c647e2e: r[9]=rd(m,0x8c647e9cu,4); /* mov.l 0x8c647e9c,r9 */
 goto L8c647e30;
L8c647e30: r[5]=r[6]; /* mov r6,r5 */
 goto L8c647e32;
L8c647e32: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c647e34;
L8c647e34: r[5]+=32u; /* add #0x20,r5 */
 goto L8c647e36;
L8c647e36: r[14]=0u; /* mov #0x0,r14 */
 goto L8c647e38;
L8c647e38: wr(m,r[9],r[3],4); /* mov.l r3,@r9 */
 goto L8c647e3a;
L8c647e3a: r[4]=r[6]; /* mov r6,r4 */
 goto L8c647e3c;
L8c647e3c: wr(m,r[4],r[14],4); /* mov.l r14,@r4 */
 goto L8c647e3e;
L8c647e3e: r[4]+=4u; /* add #0x4,r4 */
 goto L8c647e40;
L8c647e40: *sr=(*sr&~1u)|(r[4]>=r[5]); /* cmp/hs r5,r4 */
 goto L8c647e42;
L8c647e42: if(!(*sr&1u))goto L8c647e3c; /* bf 0x8c647e3c */
 goto L8c647e44;
L8c647e44: r[4]=rd(m,0x8c647eb0u,4); /* mov.l 0x8c647eb0,r4 */
 goto L8c647e46;
L8c647e46: r[6]=20u; /* mov #0x14,r6 */
 goto L8c647e48;
L8c647e48: r[11]=rd(m,0x8c647eb8u,4); /* mov.l 0x8c647eb8,r11 */
 goto L8c647e4a;
L8c647e4a: target=r[11];*pr=0x8c647e4eu;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c647e4e;
L8c647e4e: r[4]=rd(m,0x8c647ebcu,4); /* mov.l 0x8c647ebc,r4 */
 goto L8c647e50;
L8c647e50: r[6]=sx(m,0x8c647e8cu,2); /* mov.w 0x8c647e8c,r6 */
 goto L8c647e52;
L8c647e52: target=r[11];*pr=0x8c647e56u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c647e56;
L8c647e56: *pr=0x8c647e5au;r[4]=0u;status=call(ctx,0x8c647de8u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c647de8 */
 goto L8c647e5a;
L8c647e5a: r[4]=rd(m,0x8c647ec0u,4); /* mov.l 0x8c647ec0,r4 */
 goto L8c647e5c;
L8c647e5c: r[2]=0u; /* mov #0x0,r2 */
 goto L8c647e5e;
L8c647e5e: r[5]=r[14]; /* mov r14,r5 */
 goto L8c647e60;
L8c647e60: r[6]=1u; /* mov #0x1,r6 */
 goto L8c647e62;
L8c647e62: wr(m,r[4],r[2],4); /* mov.l r2,@r4 */
 goto L8c647e64;
L8c647e64: r[13]=8u; /* mov #0x8,r13 */
 goto L8c647e66;
L8c647e66: r[3]=r[12]; /* mov r12,r3 */
 goto L8c647e68;
L8c647e68: *sr=(*sr&~1u)|!(r[6]&r[3]); /* tst r6,r3 */
 goto L8c647e6a;
L8c647e6a: take=(*sr&1u);r[5]+=1u;if(take)goto L8c647e74;goto L8c647e6e; /* bt/s 0x8c647e74 */
 goto L8c647e6e;
L8c647e6e: r[1]=rd(m,r[4],4); /* mov.l @r4,r1 */
 goto L8c647e70;
L8c647e70: r[1]+=1u; /* add #0x1,r1 */
 goto L8c647e72;
L8c647e72: wr(m,r[4],r[1],4); /* mov.l r1,@r4 */
 goto L8c647e74;
L8c647e74: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1; /* shll r6 */
 goto L8c647e76;
L8c647e76: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[13]); /* cmp/ge r13,r5 */
 goto L8c647e78;
L8c647e78: if(!(*sr&1u))goto L8c647e66; /* bf 0x8c647e66 */
 goto L8c647e7a;
L8c647e7a: *sr=(*sr&~1u)|!(r[10]&r[10]); /* tst r10,r10 */
 goto L8c647e7c;
L8c647e7c: if((*sr&1u))goto L8c647ec4; /* bt 0x8c647ec4 */
 goto L8c647e7e;
L8c647e7e: r[5]=r[12]; /* mov r12,r5 */
 goto L8c647e80;
L8c647e80: r[6]=r[10]; /* mov r10,r6 */
 goto L8c647e82;
L8c647e82: *pr=0x8c647e86u;r[4]=rd(m,r[15],4);status=call(ctx,0x8c647f4cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c647f4c */
 goto L8c647e86;
L8c647e86: r[2]=rd(m,r[9],4); /* mov.l @r9,r2 */
 goto L8c647e88;
L8c647e88: wr(m,r[2]+0x8u,r[14],4);goto L8c647ed2; /* bra 0x8c647ed2 */
 goto L8c647ec4;
L8c647ec4: r[5]=r[12]; /* mov r12,r5 */
 goto L8c647ec6;
L8c647ec6: r[6]=r[10]; /* mov r10,r6 */
 goto L8c647ec8;
L8c647ec8: *pr=0x8c647eccu;r[4]=rd(m,r[15],4);status=call(ctx,0x8c647f10u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c647f10 */
 goto L8c647ecc;
L8c647ecc: r[2]=rd(m,r[9],4); /* mov.l @r9,r2 */
 goto L8c647ece;
L8c647ece: r[3]=1u; /* mov #0x1,r3 */
 goto L8c647ed0;
L8c647ed0: wr(m,r[2]+0x8u,r[3],4); /* mov.l r3,@(0x8,r2) */
 goto L8c647ed2;
L8c647ed2: r[3]=rd(m,0x8c647fb8u,4); /* mov.l 0x8c647fb8,r3 */
 goto L8c647ed4;
L8c647ed4: r[2]=rd(m,0x8c647fb4u,4); /* mov.l 0x8c647fb4,r2 */
 goto L8c647ed6;
L8c647ed6: r[1]=rd(m,0x8c647fbcu,4); /* mov.l 0x8c647fbc,r1 */
 goto L8c647ed8;
L8c647ed8: wr(m,r[2],r[13],4); /* mov.l r13,@r2 */
 goto L8c647eda;
L8c647eda: wr(m,r[3],r[14],4); /* mov.l r14,@r3 */
 goto L8c647edc;
L8c647edc: target=r[1];*pr=0x8c647ee0u;r[4]=10u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c647ee0;
L8c647ee0: r[2]=rd(m,0x8c647fb4u,4); /* mov.l 0x8c647fb4,r2 */
 goto L8c647ee2;
L8c647ee2: r[4]=rd(m,0x8c647fc0u,4); /* mov.l 0x8c647fc0,r4 */
 goto L8c647ee4;
L8c647ee4: r[6]=rd(m,r[2],4); /* mov.l @r2,r6 */
 goto L8c647ee6;
L8c647ee6: r[3]=r[6]; /* mov r6,r3 */
 goto L8c647ee8;
L8c647ee8: r[6]<<=2; /* shll2 r6 */
 goto L8c647eea;
L8c647eea: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1; /* shll r6 */
 goto L8c647eec;
L8c647eec: r[6]+=r[3]; /* add r3,r6 */
 goto L8c647eee;
L8c647eee: r[6]<<=2; /* shll2 r6 */
 goto L8c647ef0;
L8c647ef0: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1; /* shll r6 */
 goto L8c647ef2;
L8c647ef2: target=r[11];*pr=0x8c647ef6u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c647ef6;
L8c647ef6: r[3]=rd(m,0x8c647fc4u,4); /* mov.l 0x8c647fc4,r3 */
 goto L8c647ef8;
L8c647ef8: target=r[3];*pr=0x8c647efcu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c647efc;
L8c647efc: r[0]=r[14]; /* mov r14,r0 */
 goto L8c647efe;
L8c647efe: r[15]+=4u; /* add #0x4,r15 */
 goto L8c647f00;
L8c647f00: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c647f02;
L8c647f02: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c647f04;
L8c647f04: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c647f06;
L8c647f06: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c647f08;
L8c647f08: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c647f0a;
L8c647f0a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c647f0c;
L8c647f0c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
