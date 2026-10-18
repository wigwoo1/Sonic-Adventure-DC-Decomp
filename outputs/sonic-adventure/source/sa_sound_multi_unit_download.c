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
/* Static C recovery of retail Multi-unit sound bank download helper 8c643970..8c643a7a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_multi_unit_download(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c643970;
L8c643970: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c643972;
L8c643972: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c643974;
L8c643974: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c643976;
L8c643976: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c643978;
L8c643978: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c64397a;
L8c64397a: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c64397c;
L8c64397c: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c64397e;
L8c64397e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c643980;
L8c643980: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c643982;
L8c643982: r[3]=rd(m,0x8c643a80u,4); /* mov.l 0x8c643a80,r3 */
 goto L8c643984;
L8c643984: r[12]=0u; /* mov #0x0,r12 */
 goto L8c643986;
L8c643986: r[8]=rd(m,0x8c643a7cu,4); /* mov.l 0x8c643a7c,r8 */
 goto L8c643988;
L8c643988: r[13]=r[12]; /* mov r12,r13 */
 goto L8c64398a;
L8c64398a: r[2]=rd(m,0x8c643a84u,4); /* mov.l 0x8c643a84,r2 */
 goto L8c64398c;
L8c64398c: wr(m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c64398e;
L8c64398e: wr(m,r[15],r[12],4); /* mov.l r12,@r15 */
 goto L8c643990;
L8c643990: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c643992;
L8c643992: target=r[8];*pr=0x8c643996u;r[15]-=4;wr(m,r[15],r[2],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r8 */
 goto L8c643996;
L8c643996: r[1]=rd(m,0x8c643a88u,4); /* mov.l 0x8c643a88,r1 */
 goto L8c643998;
L8c643998: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c64399a;
L8c64399a: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c64399c;
L8c64399c: take=!(*sr&1u);r[15]+=8u;if(take)goto L8c6439a6;goto L8c6439a0; /* bf/s 0x8c6439a6 */
 goto L8c6439a0;
L8c6439a0: r[13]=rd(m,0x8c643a8cu,4); /* mov.l 0x8c643a8c,r13 */
 goto L8c6439a2;
L8c6439a2: goto L8c643a4a; /* bra 0x8c643a4a */
 goto L8c6439a6;
L8c6439a6: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c6439a8;
L8c6439a8: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c6439aa;
L8c6439aa: if(!(*sr&1u))goto L8c6439b2; /* bf 0x8c6439b2 */
 goto L8c6439ac;
L8c6439ac: r[13]=rd(m,0x8c643a90u,4); /* mov.l 0x8c643a90,r13 */
 goto L8c6439ae;
L8c6439ae: goto L8c643a4a; /* bra 0x8c643a4a */
 goto L8c6439b2;
L8c6439b2: r[11]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r11 */
 goto L8c6439b4;
L8c6439b4: r[3]=rd(m,0x8c643a94u,4); /* mov.l 0x8c643a94,r3 */
 goto L8c6439b6;
L8c6439b6: r[11]=rd(m,r[11]+0x8u,4); /* mov.l @(0x8,r11),r11 */
 goto L8c6439b8;
L8c6439b8: r[2]=rd(m,r[11],4); /* mov.l @r11,r2 */
 goto L8c6439ba;
L8c6439ba: *sr=(*sr&~1u)|(r[3]==r[2]); /* cmp/eq r3,r2 */
 goto L8c6439bc;
L8c6439bc: if((*sr&1u))goto L8c6439c4; /* bt 0x8c6439c4 */
 goto L8c6439be;
L8c6439be: r[13]=rd(m,0x8c643a98u,4); /* mov.l 0x8c643a98,r13 */
 goto L8c6439c0;
L8c6439c0: goto L8c643a4a; /* bra 0x8c643a4a */
 goto L8c6439c4;
L8c6439c4: r[2]=rd(m,0x8c643a9cu,4); /* mov.l 0x8c643a9c,r2 */
 goto L8c6439c6;
L8c6439c6: target=r[2];*pr=0x8c6439cau;r[4]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6439ca;
L8c6439ca: r[0]=sx(m,r[11]+0x4u,1); /* mov.b @(0x4,r11),r0 */
 goto L8c6439cc;
L8c6439cc: r[3]=1u; /* mov #0x1,r3 */
 goto L8c6439ce;
L8c6439ce: r[10]=rd(m,r[11]+0x8u,4); /* mov.l @(0x8,r11),r10 */
 goto L8c6439d0;
L8c6439d0: r[14]=r[11]; /* mov r11,r14 */
 goto L8c6439d2;
L8c6439d2: *sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[3]); /* cmp/ge r3,r0 */
 goto L8c6439d4;
L8c6439d4: take=(*sr&1u);r[14]+=32u;if(take)goto L8c6439ea;goto L8c6439d8; /* bt/s 0x8c6439ea */
 goto L8c6439d8;
L8c6439d8: r[1]=rd(m,0x8c643aa0u,4); /* mov.l 0x8c643aa0,r1 */
 goto L8c6439da;
L8c6439da: target=r[8];*pr=0x8c6439deu;r[15]-=4;wr(m,r[15],r[1],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r8 */
 goto L8c6439de;
L8c6439de: r[0]=rd(m,r[11]+0x10u,4); /* mov.l @(0x10,r11),r0 */
 goto L8c6439e0;
L8c6439e0: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #-0x1,r0 */
 goto L8c6439e2;
L8c6439e2: take=(*sr&1u);r[15]+=4u;if(take)goto L8c6439e8;goto L8c6439e6; /* bt/s 0x8c6439e8 */
 goto L8c6439e6;
L8c6439e6: r[14]+=4294967280u; /* add #-0x10,r14 */
 goto L8c6439e8;
L8c6439e8: r[13]=rd(m,0x8c643aa4u,4); /* mov.l 0x8c643aa4,r13 */
 goto L8c6439ea;
L8c6439ea: r[3]=rd(m,0x8c643aa8u,4); /* mov.l 0x8c643aa8,r3 */
 goto L8c6439ec;
L8c6439ec: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6439ee;
L8c6439ee: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c6439f0;
L8c6439f0: target=r[8];*pr=0x8c6439f4u;r[15]-=4;wr(m,r[15],r[3],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r8 */
 goto L8c6439f4;
L8c6439f4: r[15]+=12u; /* add #0xc,r15 */
 goto L8c6439f6;
L8c6439f6: *sr=(*sr&~1u)|((int32_t)r[10]>0); /* cmp/pl r10 */
 goto L8c6439f8;
L8c6439f8: take=!(*sr&1u);r[9]=r[12];if(take)goto L8c643a4a;goto L8c6439fc; /* bf/s 0x8c643a4a */
 goto L8c6439fc;
L8c6439fc: r[3]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r3 */
 goto L8c6439fe;
L8c6439fe: *sr=(*sr&~1u)|((int32_t)r[3]>=0); /* cmp/pz r3 */
 goto L8c643a00;
L8c643a00: if(!(*sr&1u))goto L8c643a14; /* bf 0x8c643a14 */
 goto L8c643a02;
L8c643a02: r[5]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r5 */
 goto L8c643a04;
L8c643a04: r[3]=rd(m,0x8c643aacu,4); /* mov.l 0x8c643aac,r3 */
 goto L8c643a06;
L8c643a06: r[7]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r7 */
 goto L8c643a08;
L8c643a08: r[6]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r6 */
 goto L8c643a0a;
L8c643a0a: target=r[3];*pr=0x8c643a0eu;r[4]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643a0e;
L8c643a0e: r[13]=r[0]; /* mov r0,r13 */
 goto L8c643a10;
L8c643a10: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c643a12;
L8c643a12: if(!(*sr&1u))goto L8c643a4a; /* bf 0x8c643a4a */
 goto L8c643a14;
L8c643a14: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c643a16;
L8c643a16: *sr=(*sr&~1u)|((int32_t)r[2]>0); /* cmp/pl r2 */
 goto L8c643a18;
L8c643a18: if(!(*sr&1u))goto L8c643a42; /* bf 0x8c643a42 */
 goto L8c643a1a;
L8c643a1a: r[5]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r5 */
 goto L8c643a1c;
L8c643a1c: r[7]=4294967295u; /* mov #-0x1,r7 */
 goto L8c643a1e;
L8c643a1e: r[3]=rd(m,0x8c643ab0u,4); /* mov.l 0x8c643ab0,r3 */
 goto L8c643a20;
L8c643a20: r[6]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r6 */
 goto L8c643a22;
L8c643a22: r[5]+=r[11]; /* add r11,r5 */
 goto L8c643a24;
L8c643a24: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c643a26;
L8c643a26: target=r[3];*pr=0x8c643a2au;r[4]=rd(m,r[15]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643a2a;
L8c643a2a: r[13]=r[0]; /* mov r0,r13 */
 goto L8c643a2c;
L8c643a2c: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c643a2e;
L8c643a2e: take=!(*sr&1u);r[15]+=4u;if(take)goto L8c643a4a;goto L8c643a32; /* bf/s 0x8c643a4a */
 goto L8c643a32;
L8c643a32: r[3]=rd(m,0x8c643ab4u,4); /* mov.l 0x8c643ab4,r3 */
 goto L8c643a34;
L8c643a34: r[5]=rd(m,r[14],4); /* mov.l @r14,r5 */
 goto L8c643a36;
L8c643a36: r[6]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r6 */
 goto L8c643a38;
L8c643a38: target=r[3];*pr=0x8c643a3cu;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643a3c;
L8c643a3c: r[13]=r[0]; /* mov r0,r13 */
 goto L8c643a3e;
L8c643a3e: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c643a40;
L8c643a40: if(!(*sr&1u))goto L8c643a4a; /* bf 0x8c643a4a */
 goto L8c643a42;
L8c643a42: r[9]+=1u; /* add #0x1,r9 */
 goto L8c643a44;
L8c643a44: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[10]); /* cmp/ge r10,r9 */
 goto L8c643a46;
L8c643a46: take=!(*sr&1u);r[14]+=32u;if(take)goto L8c6439fc;goto L8c643a4a; /* bf/s 0x8c6439fc */
 goto L8c643a4a;
L8c643a4a: r[3]=rd(m,0x8c643a80u,4); /* mov.l 0x8c643a80,r3 */
 goto L8c643a4c;
L8c643a4c: r[2]=rd(m,0x8c643ab8u,4); /* mov.l 0x8c643ab8,r2 */
 goto L8c643a4e;
L8c643a4e: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c643a50;
L8c643a50: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c643a52;
L8c643a52: target=r[8];*pr=0x8c643a56u;r[15]-=4;wr(m,r[15],r[2],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r8 */
 goto L8c643a56;
L8c643a56: r[15]+=12u; /* add #0xc,r15 */
 goto L8c643a58;
L8c643a58: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c643a5a;
L8c643a5a: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c643a5c;
L8c643a5c: if((*sr&1u))goto L8c643a64; /* bt 0x8c643a64 */
 goto L8c643a5e;
L8c643a5e: r[3]=rd(m,0x8c643abcu,4); /* mov.l 0x8c643abc,r3 */
 goto L8c643a60;
L8c643a60: target=r[3];*pr=0x8c643a64u;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643a64;
L8c643a64: r[0]=r[13]; /* mov r13,r0 */
 goto L8c643a66;
L8c643a66: r[15]+=8u; /* add #0x8,r15 */
 goto L8c643a68;
L8c643a68: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c643a6a;
L8c643a6a: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c643a6c;
L8c643a6c: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c643a6e;
L8c643a6e: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c643a70;
L8c643a70: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c643a72;
L8c643a72: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c643a74;
L8c643a74: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c643a76;
L8c643a76: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
