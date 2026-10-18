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
/* Static C recovery of retail File release wait helper 8c600cf0..8c600d82.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_release_wait(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c600cf0;
L8c600cf0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c600cf2;
L8c600cf2: r[14]=r[4]; /* mov r4,r14 */
 goto L8c600cf4;
L8c600cf4: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c600cf6;
L8c600cf6: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c600cf8;
L8c600cf8: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c600cfa;
L8c600cfa: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c600cfc;
L8c600cfc: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c600cfe;
L8c600cfe: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c600d00;
L8c600d00: r[13]=rd(m,0x8c600de4u,4); /* mov.l 0x8c600de4,r13 */
 goto L8c600d02;
L8c600d02: target=r[13];*pr=0x8c600d06u;r[11]=rd(m,r[14]+0x18u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c600d06;
L8c600d06: r[10]=rd(m,0x8c600decu,4); /* mov.l 0x8c600dec,r10 */
 goto L8c600d08;
L8c600d08: r[12]=r[0]; /* mov r0,r12 */
 goto L8c600d0a;
L8c600d0a: r[9]=rd(m,0x8c600de8u,4); /* mov.l 0x8c600de8,r9 */
 goto L8c600d0c;
L8c600d0c: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c600d0e;
L8c600d0e: r[0]=72u; /* mov #0x48,r0 */
 goto L8c600d10;
L8c600d10: r[3]=rd(m,r[2]+0x3cu,4); /* mov.l @(0x3c,r2),r3 */
 goto L8c600d12;
L8c600d12: r[1]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r1 */
 goto L8c600d14;
L8c600d14: target=r[1];*pr=0x8c600d18u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c600d18;
L8c600d18: *pr=0x8c600d1cu;r[4]=r[14];status=call(ctx,0x8c6013a4u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6013a4 */
 goto L8c600d1c;
L8c600d1c: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c600d1e;
L8c600d1e: take=(*sr&1u);r[4]=r[0];if(take)goto L8c600d3e;goto L8c600d22; /* bt/s 0x8c600d3e */
 goto L8c600d22;
L8c600d22: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c600d24;
L8c600d24: if((*sr&1u))goto L8c600d42; /* bt 0x8c600d42 */
 goto L8c600d26;
L8c600d26: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c600d28;
L8c600d28: if((*sr&1u))goto L8c600d46; /* bt 0x8c600d46 */
 goto L8c600d2a;
L8c600d2a: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c600d2c;
L8c600d2c: if((*sr&1u))goto L8c600d46; /* bt 0x8c600d46 */
 goto L8c600d2e;
L8c600d2e: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c600d30;
L8c600d30: if((*sr&1u))goto L8c600d46; /* bt 0x8c600d46 */
 goto L8c600d32;
L8c600d32: *sr=(*sr&~1u)|(r[0]==0x5u); /* cmp/eq #0x5,r0 */
 goto L8c600d34;
L8c600d34: if((*sr&1u))goto L8c600d56; /* bt 0x8c600d56 */
 goto L8c600d36;
L8c600d36: *sr=(*sr&~1u)|(r[0]==0x40u); /* cmp/eq #0x40,r0 */
 goto L8c600d38;
L8c600d38: if((*sr&1u))goto L8c600d56; /* bt 0x8c600d56 */
 goto L8c600d3a;
L8c600d3a: goto L8c600d5c; /* bra 0x8c600d5c */
 goto L8c600d3e;
L8c600d3e: r[0]=4294967264u;goto L8c600d72; /* bra 0x8c600d72 */
 goto L8c600d42;
L8c600d42: r[0]=0u;goto L8c600d72; /* bra 0x8c600d72 */
 goto L8c600d46;
L8c600d46: r[2]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r2 */
 goto L8c600d48;
L8c600d48: *sr=(*sr&~1u)|(r[2]==r[11]); /* cmp/eq r2,r11 */
 goto L8c600d4a;
L8c600d4a: if((*sr&1u))goto L8c600d60; /* bt 0x8c600d60 */
 goto L8c600d4c;
L8c600d4c: target=r[13];*pr=0x8c600d50u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c600d50;
L8c600d50: r[12]=r[0]; /* mov r0,r12 */
 goto L8c600d52;
L8c600d52: r[11]=rd(m,r[14]+0x18u,4);goto L8c600d60; /* bra 0x8c600d60 */
 goto L8c600d56;
L8c600d56: r[0]=82u; /* mov #0x52,r0 */
 goto L8c600d58;
L8c600d58: r[0]=sx(m,r[14]+r[0],2);goto L8c600d72; /* bra 0x8c600d72 */
 goto L8c600d5c;
L8c600d5c: r[0]=4294967271u;goto L8c600d72; /* bra 0x8c600d72 */
 goto L8c600d60;
L8c600d60: target=r[13];*pr=0x8c600d64u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c600d64;
L8c600d64: r[5]=r[0]; /* mov r0,r5 */
 goto L8c600d66;
L8c600d66: target=r[9];*pr=0x8c600d6au;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto L8c600d6a;
L8c600d6a: r[4]=r[0]; /* mov r0,r4 */
 goto L8c600d6c;
L8c600d6c: *sr=(*sr&~1u)|(r[4]>r[10]); /* cmp/hi r10,r4 */
 goto L8c600d6e;
L8c600d6e: if(!(*sr&1u))goto L8c600d0c; /* bf 0x8c600d0c */
 goto L8c600d70;
L8c600d70: r[0]=4294967275u; /* mov #-0x15,r0 */
 goto L8c600d72;
L8c600d72: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c600d74;
L8c600d74: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c600d76;
L8c600d76: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c600d78;
L8c600d78: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c600d7a;
L8c600d7a: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c600d7c;
L8c600d7c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c600d7e;
L8c600d7e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
