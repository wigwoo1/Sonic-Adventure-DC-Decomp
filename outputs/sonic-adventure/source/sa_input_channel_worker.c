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
/* Static C recovery of retail Tikal helper 8c647980..8c647bb8.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
static int sa_input_channel_worker_core(uint32_t entry,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 if(entry==0x8c647980u)goto L8c647980;else if(entry==0x8c647982u)goto L8c647982;else if(entry==0x8c647b74u)goto L8c647b74;else if(entry==0x8c6479fau)goto L8c6479fa;else if(entry==0x8c647a08u)goto L8c647a08;else return -4;
L8c647980: r[4]=7u; /* mov #0x7,r4 */
 goto L8c647982;
L8c647982: r[14]=r[4]; /* mov r4,r14 */
 goto L8c647984;
L8c647984: r[14]<<=2; /* shll2 r14 */
 goto L8c647986;
L8c647986: r[14]<<=2; /* shll2 r14 */
 goto L8c647988;
L8c647988: r[3]=rd(m,0x8c6479bcu,4); /* mov.l 0x8c6479bc,r3 */
 goto L8c64798a;
L8c64798a: r[2]=rd(m,0x8c6479c4u,4); /* mov.l 0x8c6479c4,r2 */
 goto L8c64798c;
L8c64798c: r[9]=r[4]; /* mov r4,r9 */
 goto L8c64798e;
L8c64798e: r[14]<<=2; /* shll2 r14 */
 goto L8c647990;
L8c647990: r[10]=sx(m,0x8c6479b8u,2); /* mov.w 0x8c6479b8,r10 */
 goto L8c647992;
L8c647992: r[9]<<=2; /* shll2 r9 */
 goto L8c647994;
L8c647994: r[8]=rd(m,0x8c6479c0u,4); /* mov.l 0x8c6479c0,r8 */
 goto L8c647996;
L8c647996: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1; /* shll r14 */
 goto L8c647998;
L8c647998: r[11]=r[4]; /* mov r4,r11 */
 goto L8c64799a;
L8c64799a: r[9]+=r[2]; /* add r2,r9 */
 goto L8c64799c;
L8c64799c: r[14]+=r[3]; /* add r3,r14 */
 goto L8c64799e;
L8c64799e: r[13]=0u; /* mov #0x0,r13 */
 goto L8c6479a0;
L8c6479a0: r[0]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r0 */
 goto L8c6479a2;
L8c6479a2: r[1]=21u; /* mov #0x15,r1 */
 goto L8c6479a4;
L8c6479a4: *sr=(*sr&~1u)|(r[0]>=r[1]); /* cmp/hs r1,r0 */
 goto L8c6479a6;
L8c6479a6: if(!(*sr&1u))goto L8c6479ac; /* bf 0x8c6479ac */
 goto L8c6479a8;
L8c6479a8: goto L8c647b6e; /* bra 0x8c647b6e */
 goto L8c6479ac;
L8c6479ac: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c6479ae;
L8c6479ae: r[1]=r[0]; /* mov r0,r1 */
 goto L8c6479b0;
L8c6479b0: r[0]=0x8c6479c8u; /* mova 0x8c6479c8,r0 */
 goto L8c6479b2;
L8c6479b2: r[0]=sx(m,r[1]+r[0],2); /* mov.w @(r0,r1),r0 */
 goto L8c6479b4;
L8c6479b4: switch(0x8c6479b8u+r[0]){case 0x8c6479f2u:goto L8c6479f2;case 0x8c6479feu:goto L8c6479fe;case 0x8c647a0cu:goto L8c647a0c;case 0x8c647a28u:goto L8c647a28;case 0x8c647a3au:goto L8c647a3a;case 0x8c647a4eu:goto L8c647a4e;case 0x8c647a62u:goto L8c647a62;case 0x8c647a74u:goto L8c647a74;case 0x8c647a8au:goto L8c647a8a;case 0x8c647aa0u:goto L8c647aa0;case 0x8c647ab0u:goto L8c647ab0;case 0x8c647ac0u:goto L8c647ac0;case 0x8c647ad0u:goto L8c647ad0;case 0x8c647ae6u:goto L8c647ae6;case 0x8c647b06u:goto L8c647b06;case 0x8c647b26u:goto L8c647b26;case 0x8c647b40u:goto L8c647b40;case 0x8c647b52u:goto L8c647b52;case 0x8c647b6eu:goto L8c647b6e;default:return -2;} /* braf r0 */
L8c6479f2: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c6479f4;
L8c6479f4: wr(m,r[14]+0x10u,r[3],4); /* mov.l r3,@(0x10,r14) */
 goto L8c6479f6;
L8c6479f6: target=r[8];*pr=0x8c6479fau;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r8 */
 goto L8c6479fa;
L8c6479fa: goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c6479fe;
L8c6479fe: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c647a00;
L8c647a00: r[2]=rd(m,0x8c647c00u,4); /* mov.l 0x8c647c00,r2 */
 goto L8c647a02;
L8c647a02: wr(m,r[14]+0x10u,r[3],4); /* mov.l r3,@(0x10,r14) */
 goto L8c647a04;
L8c647a04: target=r[2];*pr=0x8c647a08u;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c647a08;
L8c647a08: goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647a0c;
L8c647a0c: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c647a0e;
L8c647a0e: r[5]=r[14]; /* mov r14,r5 */
 goto L8c647a10;
L8c647a10: r[7]=r[14]; /* mov r14,r7 */
 goto L8c647a12;
L8c647a12: r[5]+=76u; /* add #0x4c,r5 */
 goto L8c647a14;
L8c647a14: wr(m,r[14]+0x10u,r[2],4); /* mov.l r2,@(0x10,r14) */
 goto L8c647a16;
L8c647a16: r[7]+=108u; /* add #0x6c,r7 */
 goto L8c647a18;
L8c647a18: r[3]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r3 */
 goto L8c647a1a;
L8c647a1a: r[2]=rd(m,0x8c647c04u,4); /* mov.l 0x8c647c04,r2 */
 goto L8c647a1c;
L8c647a1c: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c647a1e;
L8c647a1e: r[6]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r6 */
 goto L8c647a20;
L8c647a20: target=r[2];*pr=0x8c647a24u;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c647a24;
L8c647a24: r[15]+=4u;goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647a28;
L8c647a28: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c647a2a;
L8c647a2a: r[3]=rd(m,r[2]+0x28u,4); /* mov.l @(0x28,r2),r3 */
 goto L8c647a2c;
L8c647a2c: r[2]=rd(m,0x8c647c08u,4); /* mov.l 0x8c647c08,r2 */
 goto L8c647a2e;
L8c647a2e: wr(m,r[14]+0x10u,r[3],4); /* mov.l r3,@(0x10,r14) */
 goto L8c647a30;
L8c647a30: r[5]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r5 */
 goto L8c647a32;
L8c647a32: target=r[2];*pr=0x8c647a36u;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c647a36;
L8c647a36: goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647a3a;
L8c647a3a: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c647a3c;
L8c647a3c: r[3]=rd(m,r[2]+0x28u,4); /* mov.l @(0x28,r2),r3 */
 goto L8c647a3e;
L8c647a3e: r[2]=rd(m,0x8c647c0cu,4); /* mov.l 0x8c647c0c,r2 */
 goto L8c647a40;
L8c647a40: wr(m,r[14]+0x10u,r[3],4); /* mov.l r3,@(0x10,r14) */
 goto L8c647a42;
L8c647a42: r[5]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r5 */
 goto L8c647a44;
L8c647a44: r[6]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r6 */
 goto L8c647a46;
L8c647a46: target=r[2];*pr=0x8c647a4au;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c647a4a;
L8c647a4a: goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647a4e;
L8c647a4e: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c647a50;
L8c647a50: r[3]=rd(m,r[2]+0x28u,4); /* mov.l @(0x28,r2),r3 */
 goto L8c647a52;
L8c647a52: r[2]=rd(m,0x8c647c10u,4); /* mov.l 0x8c647c10,r2 */
 goto L8c647a54;
L8c647a54: wr(m,r[14]+0x10u,r[3],4); /* mov.l r3,@(0x10,r14) */
 goto L8c647a56;
L8c647a56: r[5]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r5 */
 goto L8c647a58;
L8c647a58: r[6]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r6 */
 goto L8c647a5a;
L8c647a5a: target=r[2];*pr=0x8c647a5eu;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c647a5e;
L8c647a5e: goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647a62;
L8c647a62: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c647a64;
L8c647a64: r[5]=r[14]; /* mov r14,r5 */
 goto L8c647a66;
L8c647a66: r[3]=rd(m,0x8c647c14u,4); /* mov.l 0x8c647c14,r3 */
 goto L8c647a68;
L8c647a68: r[5]+=44u; /* add #0x2c,r5 */
 goto L8c647a6a;
L8c647a6a: wr(m,r[14]+0x10u,r[2],4); /* mov.l r2,@(0x10,r14) */
 goto L8c647a6c;
L8c647a6c: target=r[3];*pr=0x8c647a70u;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c647a70;
L8c647a70: goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647a74;
L8c647a74: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c647a76;
L8c647a76: r[5]=r[14]; /* mov r14,r5 */
 goto L8c647a78;
L8c647a78: r[6]=r[14]; /* mov r14,r6 */
 goto L8c647a7a;
L8c647a7a: r[2]=rd(m,0x8c647c18u,4); /* mov.l 0x8c647c18,r2 */
 goto L8c647a7c;
L8c647a7c: r[5]+=44u; /* add #0x2c,r5 */
 goto L8c647a7e;
L8c647a7e: wr(m,r[14]+0x10u,r[3],4); /* mov.l r3,@(0x10,r14) */
 goto L8c647a80;
L8c647a80: r[6]+=60u; /* add #0x3c,r6 */
 goto L8c647a82;
L8c647a82: target=r[2];*pr=0x8c647a86u;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c647a86;
L8c647a86: goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647a8a;
L8c647a8a: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c647a8c;
L8c647a8c: r[5]=r[14]; /* mov r14,r5 */
 goto L8c647a8e;
L8c647a8e: r[3]=rd(m,0x8c647c1cu,4); /* mov.l 0x8c647c1c,r3 */
 goto L8c647a90;
L8c647a90: r[5]+=44u; /* add #0x2c,r5 */
 goto L8c647a92;
L8c647a92: wr(m,r[14]+0x10u,r[2],4); /* mov.l r2,@(0x10,r14) */
 goto L8c647a94;
L8c647a94: r[6]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r6 */
 goto L8c647a96;
L8c647a96: r[7]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r7 */
 goto L8c647a98;
L8c647a98: target=r[3];*pr=0x8c647a9cu;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c647a9c;
L8c647a9c: goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647aa0;
L8c647aa0: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c647aa2;
L8c647aa2: r[2]=rd(m,0x8c647c20u,4); /* mov.l 0x8c647c20,r2 */
 goto L8c647aa4;
L8c647aa4: wr(m,r[14]+0x10u,r[3],4); /* mov.l r3,@(0x10,r14) */
 goto L8c647aa6;
L8c647aa6: r[5]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r5 */
 goto L8c647aa8;
L8c647aa8: target=r[2];*pr=0x8c647aacu;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c647aac;
L8c647aac: goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647ab0;
L8c647ab0: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c647ab2;
L8c647ab2: r[3]=rd(m,0x8c647c24u,4); /* mov.l 0x8c647c24,r3 */
 goto L8c647ab4;
L8c647ab4: wr(m,r[14]+0x10u,r[2],4); /* mov.l r2,@(0x10,r14) */
 goto L8c647ab6;
L8c647ab6: r[5]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r5 */
 goto L8c647ab8;
L8c647ab8: target=r[3];*pr=0x8c647abcu;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c647abc;
L8c647abc: goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647ac0;
L8c647ac0: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c647ac2;
L8c647ac2: r[2]=rd(m,0x8c647c28u,4); /* mov.l 0x8c647c28,r2 */
 goto L8c647ac4;
L8c647ac4: wr(m,r[14]+0x10u,r[3],4); /* mov.l r3,@(0x10,r14) */
 goto L8c647ac6;
L8c647ac6: r[5]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r5 */
 goto L8c647ac8;
L8c647ac8: target=r[2];*pr=0x8c647accu;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c647acc;
L8c647acc: goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647ad0;
L8c647ad0: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c647ad2;
L8c647ad2: r[5]=r[14]; /* mov r14,r5 */
 goto L8c647ad4;
L8c647ad4: r[3]=rd(m,0x8c647c2cu,4); /* mov.l 0x8c647c2c,r3 */
 goto L8c647ad6;
L8c647ad6: r[5]+=44u; /* add #0x2c,r5 */
 goto L8c647ad8;
L8c647ad8: wr(m,r[14]+0x10u,r[2],4); /* mov.l r2,@(0x10,r14) */
 goto L8c647ada;
L8c647ada: r[6]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r6 */
 goto L8c647adc;
L8c647adc: r[7]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r7 */
 goto L8c647ade;
L8c647ade: target=r[3];*pr=0x8c647ae2u;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c647ae2;
L8c647ae2: goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647ae6;
L8c647ae6: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c647ae8;
L8c647ae8: r[5]=r[14]; /* mov r14,r5 */
 goto L8c647aea;
L8c647aea: r[5]+=44u; /* add #0x2c,r5 */
 goto L8c647aec;
L8c647aec: wr(m,r[14]+0x10u,r[3],4); /* mov.l r3,@(0x10,r14) */
 goto L8c647aee;
L8c647aee: r[3]=r[14]; /* mov r14,r3 */
 goto L8c647af0;
L8c647af0: r[2]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r2 */
 goto L8c647af2;
L8c647af2: r[3]+=108u; /* add #0x6c,r3 */
 goto L8c647af4;
L8c647af4: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c647af6;
L8c647af6: r[2]=rd(m,0x8c647c30u,4); /* mov.l 0x8c647c30,r2 */
 goto L8c647af8;
L8c647af8: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c647afa;
L8c647afa: r[6]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r6 */
 goto L8c647afc;
L8c647afc: r[7]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r7 */
 goto L8c647afe;
L8c647afe: target=r[2];*pr=0x8c647b02u;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c647b02;
L8c647b02: r[15]+=8u;goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647b06;
L8c647b06: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c647b08;
L8c647b08: r[5]=r[14]; /* mov r14,r5 */
 goto L8c647b0a;
L8c647b0a: r[5]+=44u; /* add #0x2c,r5 */
 goto L8c647b0c;
L8c647b0c: wr(m,r[14]+0x10u,r[2],4); /* mov.l r2,@(0x10,r14) */
 goto L8c647b0e;
L8c647b0e: r[2]=r[14]; /* mov r14,r2 */
 goto L8c647b10;
L8c647b10: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c647b12;
L8c647b12: r[2]+=108u; /* add #0x6c,r2 */
 goto L8c647b14;
L8c647b14: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c647b16;
L8c647b16: r[3]=rd(m,0x8c647c34u,4); /* mov.l 0x8c647c34,r3 */
 goto L8c647b18;
L8c647b18: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c647b1a;
L8c647b1a: r[6]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r6 */
 goto L8c647b1c;
L8c647b1c: r[7]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r7 */
 goto L8c647b1e;
L8c647b1e: target=r[3];*pr=0x8c647b22u;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c647b22;
L8c647b22: r[15]+=8u;goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647b26;
L8c647b26: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c647b28;
L8c647b28: r[5]=r[14]; /* mov r14,r5 */
 goto L8c647b2a;
L8c647b2a: r[5]+=44u; /* add #0x2c,r5 */
 goto L8c647b2c;
L8c647b2c: wr(m,r[14]+0x10u,r[3],4); /* mov.l r3,@(0x10,r14) */
 goto L8c647b2e;
L8c647b2e: r[2]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r2 */
 goto L8c647b30;
L8c647b30: r[3]=rd(m,0x8c647c38u,4); /* mov.l 0x8c647c38,r3 */
 goto L8c647b32;
L8c647b32: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c647b34;
L8c647b34: r[6]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r6 */
 goto L8c647b36;
L8c647b36: r[7]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r7 */
 goto L8c647b38;
L8c647b38: target=r[3];*pr=0x8c647b3cu;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c647b3c;
L8c647b3c: r[15]+=4u;goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647b40;
L8c647b40: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c647b42;
L8c647b42: r[2]=rd(m,0x8c647c3cu,4); /* mov.l 0x8c647c3c,r2 */
 goto L8c647b44;
L8c647b44: wr(m,r[14]+0x10u,r[3],4); /* mov.l r3,@(0x10,r14) */
 goto L8c647b46;
L8c647b46: r[5]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r5 */
 goto L8c647b48;
L8c647b48: r[6]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r6 */
 goto L8c647b4a;
L8c647b4a: target=r[2];*pr=0x8c647b4eu;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c647b4e;
L8c647b4e: goto L8c647b6a; /* bra 0x8c647b6a */
 goto L8c647b52;
L8c647b52: r[2]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r2 */
 goto L8c647b54;
L8c647b54: r[5]=r[14]; /* mov r14,r5 */
 goto L8c647b56;
L8c647b56: r[5]+=44u; /* add #0x2c,r5 */
 goto L8c647b58;
L8c647b58: wr(m,r[14]+0x10u,r[2],4); /* mov.l r2,@(0x10,r14) */
 goto L8c647b5a;
L8c647b5a: r[3]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r3 */
 goto L8c647b5c;
L8c647b5c: r[2]=rd(m,0x8c647c40u,4); /* mov.l 0x8c647c40,r2 */
 goto L8c647b5e;
L8c647b5e: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c647b60;
L8c647b60: r[6]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r6 */
 goto L8c647b62;
L8c647b62: r[7]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r7 */
 goto L8c647b64;
L8c647b64: target=r[2];*pr=0x8c647b68u;r[4]=rd(m,r[14]+0x14u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c647b68;
L8c647b68: r[15]+=4u; /* add #0x4,r15 */
 goto L8c647b6a;
L8c647b6a: r[12]=r[0];goto L8c647b78; /* bra 0x8c647b78 */
 goto L8c647b6e;
L8c647b6e: r[2]=rd(m,0x8c647c44u,4); /* mov.l 0x8c647c44,r2 */
 goto L8c647b70;
L8c647b70: target=r[2];*pr=0x8c647b74u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c647b74;
L8c647b74: goto L8c6479a0; /* bra 0x8c6479a0 */
 goto L8c647b78;
L8c647b78: *sr=(*sr&~1u)|(r[10]==r[12]); /* cmp/eq r10,r12 */
 goto L8c647b7a;
L8c647b7a: if(!(*sr&1u))goto L8c647b82; /* bf 0x8c647b82 */
 goto L8c647b7c;
L8c647b7c: r[3]=rd(m,0x8c647c48u,4); /* mov.l 0x8c647c48,r3 */
 goto L8c647b7e;
L8c647b7e: target=r[3];*pr=0x8c647b82u;r[4]=rd(m,r[14]+0x10u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c647b82;
L8c647b82: r[0]=124u; /* mov #0x7c,r0 */
 goto L8c647b84;
L8c647b84: wr(m,r[9],r[12],4); /* mov.l r12,@r9 */
 goto L8c647b86;
L8c647b86: r[7]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r7 */
 goto L8c647b88;
L8c647b88: r[0]=116u; /* mov #0x74,r0 */
 goto L8c647b8a;
L8c647b8a: r[5]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r5 */
 goto L8c647b8c;
L8c647b8c: wr(m,r[14],r[13],4); /* mov.l r13,@r14 */
 goto L8c647b8e;
L8c647b8e: wr(m,r[14]+0x4u,r[13],4); /* mov.l r13,@(0x4,r14) */
 goto L8c647b90;
L8c647b90: wr(m,r[14]+0x8u,r[13],4); /* mov.l r13,@(0x8,r14) */
 goto L8c647b92;
L8c647b92: wr(m,r[14]+0xcu,r[13],4); /* mov.l r13,@(0xc,r14) */
 goto L8c647b94;
L8c647b94: wr(m,r[14]+0x10u,r[13],4); /* mov.l r13,@(0x10,r14) */
 goto L8c647b96;
L8c647b96: wr(m,r[14]+0x14u,r[13],4); /* mov.l r13,@(0x14,r14) */
 goto L8c647b98;
L8c647b98: wr(m,r[14]+0x18u,r[13],4); /* mov.l r13,@(0x18,r14) */
 goto L8c647b9a;
L8c647b9a: wr(m,r[14]+0x1cu,r[13],4); /* mov.l r13,@(0x1c,r14) */
 goto L8c647b9c;
L8c647b9c: wr(m,r[14]+0x20u,r[13],4); /* mov.l r13,@(0x20,r14) */
 goto L8c647b9e;
L8c647b9e: wr(m,r[14]+0x24u,r[13],4); /* mov.l r13,@(0x24,r14) */
 goto L8c647ba0;
L8c647ba0: wr(m,r[14]+0x28u,r[13],4); /* mov.l r13,@(0x28,r14) */
 goto L8c647ba2;
L8c647ba2: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c647ba4;
L8c647ba4: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c647ba6;
L8c647ba6: if(!(*sr&1u))goto L8c647bac; /* bf 0x8c647bac */
 goto L8c647ba8;
L8c647ba8: goto L8c6479a0; /* bra 0x8c6479a0 */
 goto L8c647bac;
L8c647bac: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c647bae;
L8c647bae: r[6]=r[12]; /* mov r12,r6 */
 goto L8c647bb0;
L8c647bb0: target=r[3];*pr=0x8c647bb4u;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c647bb4;
L8c647bb4: goto L8c6479a0; /* bra 0x8c6479a0 */
}

SA_STAGE_EXPORT int sa_input_channel_worker(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_channel_worker_core(0x8c647980u,m,f,sr,r,pr,fpul,macl,ctx,call);}
SA_STAGE_EXPORT int sa_input_channel_worker_entry(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_channel_worker_core(0x8c647982u,m,f,sr,r,pr,fpul,macl,ctx,call);}
SA_STAGE_EXPORT int sa_input_channel_worker_resume(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_channel_worker_core(*pr,m,f,sr,r,pr,fpul,macl,ctx,call);}
