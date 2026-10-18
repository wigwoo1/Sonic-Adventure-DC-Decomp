#include "input_io.h"
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
/* Static C recovery of retail Tikal helper 8c64594c..8c645afa.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_callback_register_io(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,uint32_t vbr,const struct sa_input_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c64594c;
L8c64594c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c64594e;
L8c64594e: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c645950;
L8c645950: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c645952;
L8c645952: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c645954;
L8c645954: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c645956;
L8c645956: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c645958;
L8c645958: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c64595a;
L8c64595a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c64595c;
L8c64595c: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c64595e;
L8c64595e: r[0]=r[4]; /* mov r4,r0 */
 goto L8c645960;
L8c645960: wr(m,r[15]+0x4u,r[0],2); /* mov.w r0,@(0x4,r15) */
 goto L8c645962;
L8c645962: r[0]=*sr; /* stc SR,r0 */
 goto L8c645964;
L8c645964: r[3]=sx(m,0x8c6459fcu,2); /* mov.w 0x8c6459fc,r3 */
 goto L8c645966;
L8c645966: r[8]=r[7]; /* mov r7,r8 */
 goto L8c645968;
L8c645968: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c64596a;
L8c64596a: r[0]>>=2; /* shlr2 r0 */
 goto L8c64596c;
L8c64596c: r[0]>>=2; /* shlr2 r0 */
 goto L8c64596e;
L8c64596e: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c645970;
L8c645970: wr(m,r[15]+0x8u,r[0],2); /* mov.w r0,@(0x8,r15) */
 goto L8c645972;
L8c645972: r[0]=*sr; /* stc SR,r0 */
 goto L8c645974;
L8c645974: r[0]&=r[3]; /* and r3,r0 */
 goto L8c645976;
L8c645976: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c645978;
L8c645978: *sr=r[0]; /* ldc r0,SR */
 goto L8c64597a;
L8c64597a: r[0]=sx(m,r[15]+0x4u,2); /* mov.w @(0x4,r15),r0 */
 goto L8c64597c;
L8c64597c: r[3]=4294967292u; /* mov #-0x4,r3 */
 goto L8c64597e;
L8c64597e: r[9]=rd(m,0x8c645a10u,4); /* mov.l 0x8c645a10,r9 */
 goto L8c645980;
L8c645980: r[11]=r[6]; /* mov r6,r11 */
 goto L8c645982;
L8c645982: r[13]=r[0]; /* mov r0,r13 */
 goto L8c645984;
L8c645984: r[0]=r[13]; /* mov r13,r0 */
 goto L8c645986;
L8c645986: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c645988;
L8c645988: r[7]=rd(m,0x8c645a14u,4); /* mov.l 0x8c645a14,r7 */
 goto L8c64598a;
L8c64598a: r[0]<<=2; /* shll2 r0 */
 goto L8c64598c;
L8c64598c: r[4]=rd(m,r[9]+r[0],4); /* mov.l @(r0,r9),r4 */
 goto L8c64598e;
L8c64598e: r[5]=r[4]; /* mov r4,r5 */
 goto L8c645990;
L8c645990: r[11]|=r[7];goto L8c645998; /* bra 0x8c645998 */
 goto L8c645994;
L8c645994: r[5]=r[4]; /* mov r4,r5 */
 goto L8c645996;
L8c645996: r[4]=rd(m,r[4]+0x4u,4); /* mov.l @(0x4,r4),r4 */
 goto L8c645998;
L8c645998: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c64599a;
L8c64599a: if((*sr&1u))goto L8c6459a2; /* bt 0x8c6459a2 */
 goto L8c64599c;
L8c64599c: r[2]=rd(m,r[4]+0xcu,4); /* mov.l @(0xc,r4),r2 */
 goto L8c64599e;
L8c64599e: *sr=(*sr&~1u)|(r[2]>=r[11]); /* cmp/hs r11,r2 */
 goto L8c6459a0;
L8c6459a0: if(!(*sr&1u))goto L8c645994; /* bf 0x8c645994 */
 goto L8c6459a2;
L8c6459a2: r[10]=rd(m,0x8c645a0cu,4); /* mov.l 0x8c645a0c,r10 */
 goto L8c6459a4;
L8c6459a4: r[12]=1u; /* mov #0x1,r12 */
 goto L8c6459a6;
L8c6459a6: r[6]=sx(m,0x8c645a02u,2); /* mov.w 0x8c645a02,r6 */
 goto L8c6459a8;
L8c6459a8: r[14]=r[12]; /* mov r12,r14 */
 goto L8c6459aa;
L8c6459aa: r[2]=r[14]; /* mov r14,r2 */
 goto L8c6459ac;
L8c6459ac: r[2]<<=2; /* shll2 r2 */
 goto L8c6459ae;
L8c6459ae: r[2]<<=2; /* shll2 r2 */
 goto L8c6459b0;
L8c6459b0: r[2]+=r[10]; /* add r10,r2 */
 goto L8c6459b2;
L8c6459b2: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c6459b4;
L8c6459b4: *sr=(*sr&~1u)|!(r[7]&r[3]); /* tst r7,r3 */
 goto L8c6459b6;
L8c6459b6: if((*sr&1u))goto L8c6459be; /* bt 0x8c6459be */
 goto L8c6459b8;
L8c6459b8: r[14]+=1u; /* add #0x1,r14 */
 goto L8c6459ba;
L8c6459ba: *sr=(*sr&~1u)|(r[14]>=r[6]); /* cmp/hs r6,r14 */
 goto L8c6459bc;
L8c6459bc: if(!(*sr&1u))goto L8c6459aa; /* bf 0x8c6459aa */
 goto L8c6459be;
L8c6459be: *sr=(*sr&~1u)|(r[14]>=r[6]); /* cmp/hs r6,r14 */
 goto L8c6459c0;
L8c6459c0: if(!(*sr&1u))goto L8c6459c6; /* bf 0x8c6459c6 */
 goto L8c6459c2;
L8c6459c2: goto L8c645ace; /* bra 0x8c645ace */
 goto L8c6459c6;
L8c6459c6: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c6459c8;
L8c6459c8: if(!(*sr&1u))goto L8c645a9a; /* bf 0x8c645a9a */
 goto L8c6459ca;
L8c6459ca: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c6459cc;
L8c6459cc: if(!(*sr&1u))goto L8c645a9a; /* bf 0x8c645a9a */
 goto L8c6459ce;
L8c6459ce: r[4]=sx(m,0x8c645a04u,2); /* mov.w 0x8c645a04,r4 */
 goto L8c6459d0;
L8c6459d0: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[4]); /* cmp/ge r4,r13 */
 goto L8c6459d2;
L8c6459d2: if((*sr&1u))goto L8c6459ec; /* bt 0x8c6459ec */
 goto L8c6459d4;
L8c6459d4: r[3]=sx(m,0x8c645a06u,2); /* mov.w 0x8c645a06,r3 */
 goto L8c6459d6;
L8c6459d6: r[2]=vbr; /* stc VBR,r2 */
 goto L8c6459d8;
L8c6459d8: r[3]+=r[13]; /* add r13,r3 */
 goto L8c6459da;
L8c6459da: r[0]=sx(m,0x8c645a08u,2); /* mov.w 0x8c645a08,r0 */
 goto L8c6459dc;
L8c6459dc: *sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u); /* shar r3 */
 goto L8c6459de;
L8c6459de: *sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u); /* shar r3 */
 goto L8c6459e0;
L8c6459e0: *sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u); /* shar r3 */
 goto L8c6459e2;
L8c6459e2: r[2]+=r[3]; /* add r3,r2 */
 goto L8c6459e4;
L8c6459e4: r[3]=rd(m,0x8c645a18u,4); /* mov.l 0x8c645a18,r3 */
 goto L8c6459e6;
L8c6459e6: wr(m,r[2]+r[0],r[3],4); /* mov.l r3,@(r0,r2) */
 goto L8c6459e8;
L8c6459e8: goto L8c645a3e; /* bra 0x8c645a3e */
 goto L8c6459ec;
L8c6459ec: *sr=(*sr&~1u)|(r[4]==r[13]); /* cmp/eq r4,r13 */
 goto L8c6459ee;
L8c6459ee: if(!(*sr&1u))goto L8c645a24; /* bf 0x8c645a24 */
 goto L8c6459f0;
L8c6459f0: r[2]=rd(m,0x8c645a20u,4); /* mov.l 0x8c645a20,r2 */
 goto L8c6459f2;
L8c6459f2: r[4]=rd(m,0x8c645a1cu,4); /* mov.l 0x8c645a1c,r4 */
 goto L8c6459f4;
L8c6459f4: target=r[2];*pr=0x8c6459f8u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6459f8;
L8c6459f8: goto L8c645a3e; /* bra 0x8c645a3e */
 goto L8c645a24;
L8c645a24: r[2]=sx(m,0x8c645b44u,2); /* mov.w 0x8c645b44,r2 */
 goto L8c645a26;
L8c645a26: *sr=(*sr&~1u)|(r[2]==r[13]); /* cmp/eq r2,r13 */
 goto L8c645a28;
L8c645a28: if(!(*sr&1u))goto L8c645a36; /* bf 0x8c645a36 */
 goto L8c645a2a;
L8c645a2a: r[1]=rd(m,0x8c645b50u,4); /* mov.l 0x8c645b50,r1 */
 goto L8c645a2c;
L8c645a2c: r[4]=rd(m,0x8c645b4cu,4); /* mov.l 0x8c645b4c,r4 */
 goto L8c645a2e;
L8c645a2e: target=r[1];*pr=0x8c645a32u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c645a32;
L8c645a32: goto L8c645a3e; /* bra 0x8c645a3e */
 goto L8c645a36;
L8c645a36: r[1]=vbr; /* stc VBR,r1 */
 goto L8c645a38;
L8c645a38: r[0]=sx(m,0x8c645b46u,2); /* mov.w 0x8c645b46,r0 */
 goto L8c645a3a;
L8c645a3a: r[3]=rd(m,0x8c645b54u,4); /* mov.l 0x8c645b54,r3 */
 goto L8c645a3c;
L8c645a3c: wr(m,r[1]+r[0],r[3],4); /* mov.l r3,@(r0,r1) */
 goto L8c645a3e;
L8c645a3e: r[4]=r[14]; /* mov r14,r4 */
 goto L8c645a40;
L8c645a40: r[4]<<=2; /* shll2 r4 */
 goto L8c645a42;
L8c645a42: r[0]=r[13]; /* mov r13,r0 */
 goto L8c645a44;
L8c645a44: r[3]=4294967292u; /* mov #-0x4,r3 */
 goto L8c645a46;
L8c645a46: r[4]<<=2; /* shll2 r4 */
 goto L8c645a48;
L8c645a48: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c645a4a;
L8c645a4a: r[4]+=r[10]; /* add r10,r4 */
 goto L8c645a4c;
L8c645a4c: r[0]<<=2; /* shll2 r0 */
 goto L8c645a4e;
L8c645a4e: wr(m,r[9]+r[0],r[4],4); /* mov.l r4,@(r0,r9) */
 goto L8c645a50;
L8c645a50: r[3]=0u; /* mov #0x0,r3 */
 goto L8c645a52;
L8c645a52: wr(m,r[4]+0x4u,r[3],4); /* mov.l r3,@(0x4,r4) */
 goto L8c645a54;
L8c645a54: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c645a56;
L8c645a56: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c645a58;
L8c645a58: wr(m,r[4],r[2],4); /* mov.l r2,@r4 */
 goto L8c645a5a;
L8c645a5a: wr(m,r[4]+0x8u,r[8],4); /* mov.l r8,@(0x8,r4) */
 goto L8c645a5c;
L8c645a5c: wr(m,r[4]+0xcu,r[11],4); /* mov.l r11,@(0xc,r4) */
 goto L8c645a5e;
L8c645a5e: r[0]=sx(m,r[15]+0x4u,2); /* mov.w @(0x4,r15),r0 */
 goto L8c645a60;
L8c645a60: r[4]=r[0]; /* mov r0,r4 */
 goto L8c645a62;
L8c645a62: r[4]=r[4]&255u; /* extu.b r4,r4 */
 goto L8c645a64;
L8c645a64: r[0]=r[13]; /* mov r13,r0 */
 goto L8c645a66;
L8c645a66: r[4]>>=2; /* shlr2 r4 */
 goto L8c645a68;
L8c645a68: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c645a6a;
L8c645a6a: r[4]>>=2; /* shlr2 r4 */
 goto L8c645a6c;
L8c645a6c: *sr=(*sr&~1u)|(r[4]&1u);r[4]>>=1; /* shlr r4 */
 goto L8c645a6e;
L8c645a6e: *sr=(*sr&~1u)|(r[0]==0xau); /* cmp/eq #0xa,r0 */
 goto L8c645a70;
L8c645a70: if(!(*sr&1u))goto L8c645a82; /* bf 0x8c645a82 */
 goto L8c645a72;
L8c645a72: r[3]=rd(m,0x8c645b58u,4); /* mov.l 0x8c645b58,r3 */
 goto L8c645a74;
L8c645a74: r[4]+=12u; /* add #0xc,r4 */
 goto L8c645a76;
L8c645a76: {uint32_t shift=r[4];if(!(shift&0x80000000u))r[12]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[12]=n?(uint32_t)((int32_t)r[12]>>n):((r[12]&0x80000000u)?0xffffffffu:0u);}} /* shad r4,r12 */
 goto L8c645a78;
L8c645a78: r[2]=io->read(io->user,r[3],4); /* mov.l @r3,r2 */
 goto L8c645a7a;
L8c645a7a: r[2]|=r[12]; /* or r12,r2 */
 goto L8c645a7c;
L8c645a7c: io->write(io->user,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c645a7e;
L8c645a7e: goto L8c645aca; /* bra 0x8c645aca */
 goto L8c645a82;
L8c645a82: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c645a84;
L8c645a84: r[0]=r[13]; /* mov r13,r0 */
 goto L8c645a86;
L8c645a86: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c645a88;
L8c645a88: *sr=(*sr&~1u)|(r[0]==0xbu); /* cmp/eq #0xb,r0 */
 goto L8c645a8a;
L8c645a8a: if(!(*sr&1u))goto L8c645aca; /* bf 0x8c645aca */
 goto L8c645a8c;
L8c645a8c: r[3]=rd(m,0x8c645b5cu,4); /* mov.l 0x8c645b5c,r3 */
 goto L8c645a8e;
L8c645a8e: {uint32_t shift=r[4];if(!(shift&0x80000000u))r[12]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[12]=n?(uint32_t)((int32_t)r[12]>>n):((r[12]&0x80000000u)?0xffffffffu:0u);}} /* shad r4,r12 */
 goto L8c645a90;
L8c645a90: r[2]=io->read(io->user,r[3],4); /* mov.l @r3,r2 */
 goto L8c645a92;
L8c645a92: r[2]|=r[12]; /* or r12,r2 */
 goto L8c645a94;
L8c645a94: io->write(io->user,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c645a96;
L8c645a96: goto L8c645aca; /* bra 0x8c645aca */
 goto L8c645a9a;
L8c645a9a: r[0]=r[13]; /* mov r13,r0 */
 goto L8c645a9c;
L8c645a9c: r[3]=4294967292u; /* mov #-0x4,r3 */
 goto L8c645a9e;
L8c645a9e: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c645aa0;
L8c645aa0: r[12]=r[14]; /* mov r14,r12 */
 goto L8c645aa2;
L8c645aa2: r[0]<<=2; /* shll2 r0 */
 goto L8c645aa4;
L8c645aa4: r[2]=rd(m,r[9]+r[0],4); /* mov.l @(r0,r9),r2 */
 goto L8c645aa6;
L8c645aa6: r[12]<<=2; /* shll2 r12 */
 goto L8c645aa8;
L8c645aa8: r[12]<<=2; /* shll2 r12 */
 goto L8c645aaa;
L8c645aaa: *sr=(*sr&~1u)|(r[2]==r[4]); /* cmp/eq r2,r4 */
 goto L8c645aac;
L8c645aac: take=!(*sr&1u);r[12]+=r[10];if(take)goto L8c645abe;goto L8c645ab0; /* bf/s 0x8c645abe */
 goto L8c645ab0;
L8c645ab0: r[3]=4294967292u; /* mov #-0x4,r3 */
 goto L8c645ab2;
L8c645ab2: r[0]=r[13]; /* mov r13,r0 */
 goto L8c645ab4;
L8c645ab4: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r0 */
 goto L8c645ab6;
L8c645ab6: wr(m,r[12]+0x4u,r[4],4); /* mov.l r4,@(0x4,r12) */
 goto L8c645ab8;
L8c645ab8: r[0]<<=2; /* shll2 r0 */
 goto L8c645aba;
L8c645aba: wr(m,r[9]+r[0],r[12],4);goto L8c645ac2; /* bra 0x8c645ac2 */
 goto L8c645abe;
L8c645abe: wr(m,r[5]+0x4u,r[12],4); /* mov.l r12,@(0x4,r5) */
 goto L8c645ac0;
L8c645ac0: wr(m,r[12]+0x4u,r[4],4); /* mov.l r4,@(0x4,r12) */
 goto L8c645ac2;
L8c645ac2: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c645ac4;
L8c645ac4: wr(m,r[12],r[2],4); /* mov.l r2,@r12 */
 goto L8c645ac6;
L8c645ac6: wr(m,r[12]+0x8u,r[8],4); /* mov.l r8,@(0x8,r12) */
 goto L8c645ac8;
L8c645ac8: wr(m,r[12]+0xcu,r[11],4); /* mov.l r11,@(0xc,r12) */
 goto L8c645aca;
L8c645aca: r[4]=r[14];goto L8c645ad0; /* bra 0x8c645ad0 */
 goto L8c645ace;
L8c645ace: r[4]=0u; /* mov #0x0,r4 */
 goto L8c645ad0;
L8c645ad0: r[0]=sx(m,r[15]+0x8u,2); /* mov.w @(0x8,r15),r0 */
 goto L8c645ad2;
L8c645ad2: r[3]=*sr; /* stc SR,r3 */
 goto L8c645ad4;
L8c645ad4: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c645ad6;
L8c645ad6: r[2]=sx(m,0x8c645b48u,2); /* mov.w 0x8c645b48,r2 */
 goto L8c645ad8;
L8c645ad8: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c645ada;
L8c645ada: r[0]<<=2; /* shll2 r0 */
 goto L8c645adc;
L8c645adc: r[3]&=r[2]; /* and r2,r3 */
 goto L8c645ade;
L8c645ade: r[0]<<=2; /* shll2 r0 */
 goto L8c645ae0;
L8c645ae0: r[0]|=r[3]; /* or r3,r0 */
 goto L8c645ae2;
L8c645ae2: *sr=r[0]; /* ldc r0,SR */
 goto L8c645ae4;
L8c645ae4: r[0]=r[4]; /* mov r4,r0 */
 goto L8c645ae6;
L8c645ae6: r[15]+=12u; /* add #0xc,r15 */
 goto L8c645ae8;
L8c645ae8: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c645aea;
L8c645aea: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c645aec;
L8c645aec: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c645aee;
L8c645aee: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c645af0;
L8c645af0: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c645af2;
L8c645af2: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c645af4;
L8c645af4: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c645af6;
L8c645af6: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
