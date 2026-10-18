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
/* Static C recovery of retail File sector read helper 8c600f1e..8c600fac.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_read_sectors(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c600f1e;
L8c600f1e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c600f20;
L8c600f20: r[3]=31u; /* mov #0x1f,r3 */
 goto L8c600f22;
L8c600f22: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c600f24;
L8c600f24: r[14]=r[4]; /* mov r4,r14 */
 goto L8c600f26;
L8c600f26: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c600f28;
L8c600f28: r[12]=r[5]; /* mov r5,r12 */
 goto L8c600f2a;
L8c600f2a: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c600f2c;
L8c600f2c: r[11]=r[6]; /* mov r6,r11 */
 goto L8c600f2e;
L8c600f2e: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c600f30;
L8c600f30: *sr=(*sr&~1u)|!(r[11]&r[3]); /* tst r11,r3 */
 goto L8c600f32;
L8c600f32: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c600f34;
L8c600f34: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c600f36;
L8c600f36: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c600f38;
L8c600f38: take=!(*sr&1u);r[4]=0u;if(take)goto L8c600f40;goto L8c600f3c; /* bf/s 0x8c600f40 */
 goto L8c600f3c;
L8c600f3c: r[9]=1u;goto L8c600f42; /* bra 0x8c600f42 */
 goto L8c600f40;
L8c600f40: r[9]=r[4]; /* mov r4,r9 */
 goto L8c600f42;
L8c600f42: r[10]=sx(m,0x8c600ffeu,2); /* mov.w 0x8c600ffe,r10 */
 goto L8c600f44;
L8c600f44: r[8]=rd(m,0x8c601000u,4); /* mov.l 0x8c601000,r8 */
 goto L8c600f46;
L8c600f46: r[13]=r[4];goto L8c600f82; /* bra 0x8c600f82 */
 goto L8c600f4a;
L8c600f4a: r[0]=r[9]; /* mov r9,r0 */
 goto L8c600f4c;
L8c600f4c: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c600f4e;
L8c600f4e: if(!(*sr&1u))goto L8c600f5c; /* bf 0x8c600f5c */
 goto L8c600f50;
L8c600f50: r[5]=r[12]; /* mov r12,r5 */
 goto L8c600f52;
L8c600f52: r[6]=r[11]; /* mov r11,r6 */
 goto L8c600f54;
L8c600f54: *pr=0x8c600f58u;r[4]=r[14];status=call(ctx,0x8c601084u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c601084 */
 goto L8c600f58;
L8c600f58: goto L8c600f64; /* bra 0x8c600f64 */
 goto L8c600f5c;
L8c600f5c: r[5]=r[12]; /* mov r12,r5 */
 goto L8c600f5e;
L8c600f5e: r[6]=r[11]; /* mov r11,r6 */
 goto L8c600f60;
L8c600f60: *pr=0x8c600f64u;r[4]=r[14];status=call(ctx,0x8c600facu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c600fac */
 goto L8c600f64;
L8c600f64: r[4]=r[0]; /* mov r0,r4 */
 goto L8c600f66;
L8c600f66: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c600f68;
L8c600f68: if(!(*sr&1u))goto L8c600f86; /* bf 0x8c600f86 */
 goto L8c600f6a;
L8c600f6a: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c600f6c;
L8c600f6c: if((*sr&1u))goto L8c600f86; /* bt 0x8c600f86 */
 goto L8c600f6e;
L8c600f6e: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c600f70;
L8c600f70: r[3]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r3 */
 goto L8c600f72;
L8c600f72: *sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]); /* cmp/ge r3,r2 */
 goto L8c600f74;
L8c600f74: if((*sr&1u))goto L8c600f86; /* bt 0x8c600f86 */
 goto L8c600f76;
L8c600f76: r[5]=r[8];goto L8c600f7c; /* bra 0x8c600f7c */
 goto L8c600f7a;
L8c600f7a: r[5]+=4294967295u; /* add #-0x1,r5 */
 goto L8c600f7c;
L8c600f7c: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c600f7e;
L8c600f7e: if(!(*sr&1u))goto L8c600f7a; /* bf 0x8c600f7a */
 goto L8c600f80;
L8c600f80: r[13]+=1u; /* add #0x1,r13 */
 goto L8c600f82;
L8c600f82: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[10]); /* cmp/ge r10,r13 */
 goto L8c600f84;
L8c600f84: if(!(*sr&1u))goto L8c600f4a; /* bf 0x8c600f4a */
 goto L8c600f86;
L8c600f86: *sr=(*sr&~1u)|(r[10]==r[13]); /* cmp/eq r10,r13 */
 goto L8c600f88;
L8c600f88: if(!(*sr&1u))goto L8c600f8e; /* bf 0x8c600f8e */
 goto L8c600f8a;
L8c600f8a: r[0]=4294967275u;goto L8c600f9a; /* bra 0x8c600f9a */
 goto L8c600f8e;
L8c600f8e: *sr=(*sr&~1u)|((int32_t)r[4]>=0); /* cmp/pz r4 */
 goto L8c600f90;
L8c600f90: if(!(*sr&1u))goto L8c600f98; /* bf 0x8c600f98 */
 goto L8c600f92;
L8c600f92: *pr=0x8c600f96u;r[4]=r[14];status=call(ctx,0x8c600cf0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c600cf0 */
 goto L8c600f96;
L8c600f96: r[4]=r[0]; /* mov r0,r4 */
 goto L8c600f98;
L8c600f98: r[0]=r[4]; /* mov r4,r0 */
 goto L8c600f9a;
L8c600f9a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c600f9c;
L8c600f9c: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c600f9e;
L8c600f9e: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c600fa0;
L8c600fa0: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c600fa2;
L8c600fa2: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c600fa4;
L8c600fa4: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c600fa6;
L8c600fa6: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c600fa8;
L8c600fa8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
