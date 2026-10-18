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
/* Static C recovery of retail Tikal helper 8c660a7e..8c660bae.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
static int sa_input_backend_finalize_core(uint32_t entry,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 switch(entry){case 0:goto L8c660a7e;case 0x8c660aaeu:goto L8c660aae;case 0x8c660b18u:goto L8c660b18;case 0x8c660b2eu:goto L8c660b2e;default:return -4;}
L8c660a7e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c660a80;
L8c660a80: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c660a82;
L8c660a82: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c660a84;
L8c660a84: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c660a86;
L8c660a86: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c660a88;
L8c660a88: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c660a8a;
L8c660a8a: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c660a8c;
L8c660a8c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c660a8e;
L8c660a8e: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c660a90;
L8c660a90: r[11]=sx(m,0x8c660ad0u,2); /* mov.w 0x8c660ad0,r11 */
 goto L8c660a92;
L8c660a92: r[14]=r[4]; /* mov r4,r14 */
 goto L8c660a94;
L8c660a94: r[3]=rd(m,0x8c660ae4u,4); /* mov.l 0x8c660ae4,r3 */
 goto L8c660a96;
L8c660a96: *macl=r[11]*r[14]; /* mul.l r11,r14 */
 goto L8c660a98;
L8c660a98: r[13]=rd(m,0x8c660ae8u,4); /* mov.l 0x8c660ae8,r13 */
 goto L8c660a9a;
L8c660a9a: wr(m,r[15]+0x8u,r[5],4); /* mov.l r5,@(0x8,r15) */
 goto L8c660a9c;
L8c660a9c: wr(m,r[15],r[6],4); /* mov.l r6,@r15 */
 goto L8c660a9e;
L8c660a9e: r[11]=*macl; /* sts MACL,r11 */
 goto L8c660aa0;
L8c660aa0: r[11]+=r[3]; /* add r3,r11 */
 goto L8c660aa2;
L8c660aa2: *pr=0x8c660aa6u;r[4]=r[14];status=call(ctx,0x8c660decu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c660dec */
 goto L8c660aa6;
L8c660aa6: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c660aa8;
L8c660aa8: if(!(*sr&1u))goto L8c660ab2; /* bf 0x8c660ab2 */
 goto L8c660aaa;
L8c660aaa: target=r[13];*pr=0x8c660aaeu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c660aae;
L8c660aae: goto L8c660b68; /* bra 0x8c660b68 */
 goto L8c660ab2;
L8c660ab2: r[0]=rd(m,0x8c660aecu,4); /* mov.l 0x8c660aec,r0 */
 goto L8c660ab4;
L8c660ab4: r[14]<<=2; /* shll2 r14 */
 goto L8c660ab6;
L8c660ab6: r[3]=rd(m,0x8c660af0u,4); /* mov.l 0x8c660af0,r3 */
 goto L8c660ab8;
L8c660ab8: r[14]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r14 */
 goto L8c660aba;
L8c660aba: target=r[3];*pr=0x8c660abeu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660abe;
L8c660abe: r[4]=0u; /* mov #0x0,r4 */
 goto L8c660ac0;
L8c660ac0: r[8]=rd(m,r[15],4); /* mov.l @r15,r8 */
 goto L8c660ac2;
L8c660ac2: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c660ac4;
L8c660ac4: r[10]=r[4];goto L8c660b8e; /* bra 0x8c660b8e */
 goto L8c660af4;
L8c660af4: target=r[9];*pr=0x8c660af8u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto L8c660af8;
L8c660af8: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c660afa;
L8c660afa: if(!(*sr&1u))goto L8c660b42; /* bf 0x8c660b42 */
 goto L8c660afc;
L8c660afc: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c660afe;
L8c660afe: r[6]=0u; /* mov #0x0,r6 */
 goto L8c660b00;
L8c660b00: r[12]=rd(m,0x8c660bf4u,4); /* mov.l 0x8c660bf4,r12 */
 goto L8c660b02;
L8c660b02: r[5]=2u; /* mov #0x2,r5 */
 goto L8c660b04;
L8c660b04: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c660b06;
L8c660b06: r[7]=r[10]; /* mov r10,r7 */
 goto L8c660b08;
L8c660b08: target=r[12];*pr=0x8c660b0cu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c660b0c;
L8c660b0c: r[12]=r[0]; /* mov r0,r12 */
 goto L8c660b0e;
L8c660b0e: *sr=(*sr&~1u)|((int32_t)r[12]>=0); /* cmp/pz r12 */
 goto L8c660b10;
L8c660b10: take=(*sr&1u);r[15]+=4u;if(take)goto L8c660b1e;goto L8c660b14; /* bt/s 0x8c660b1e */
 goto L8c660b14;
L8c660b14: target=r[13];*pr=0x8c660b18u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c660b18;
L8c660b18: r[9]=rd(m,0x8c660bf8u,4); /* mov.l 0x8c660bf8,r9 */
 goto L8c660b1a;
L8c660b1a: goto L8c660af4; /* bra 0x8c660af4 */
 goto L8c660b1e;
L8c660b1e: r[2]=rd(m,0x8c660bfcu,4); /* mov.l 0x8c660bfc,r2 */
 goto L8c660b20;
L8c660b20: r[5]=r[12]; /* mov r12,r5 */
 goto L8c660b22;
L8c660b22: target=r[2];*pr=0x8c660b26u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c660b26;
L8c660b26: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c660b28;
L8c660b28: if(!(*sr&1u))goto L8c660b32; /* bf 0x8c660b32 */
 goto L8c660b2a;
L8c660b2a: target=r[13];*pr=0x8c660b2eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c660b2e;
L8c660b2e: goto L8c660b1e; /* bra 0x8c660b1e */
 goto L8c660b32;
L8c660b32: r[3]=rd(m,0x8c660c00u,4); /* mov.l 0x8c660c00,r3 */
 goto L8c660b34;
L8c660b34: target=r[3];*pr=0x8c660b38u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660b38;
L8c660b38: wr(m,r[15]+0x4u,r[0],4); /* mov.l r0,@(0x4,r15) */
 goto L8c660b3a;
L8c660b3a: target=r[9];*pr=0x8c660b3eu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r9 */
 goto L8c660b3e;
L8c660b3e: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c660b40;
L8c660b40: if((*sr&1u))goto L8c660b48; /* bt 0x8c660b48 */
 goto L8c660b42;
L8c660b42: r[0]=sx(m,0x8c660beau,2); /* mov.w 0x8c660bea,r0 */
 goto L8c660b44;
L8c660b44: goto L8c660b9a; /* bra 0x8c660b9a */
 goto L8c660b48;
L8c660b48: r[0]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c660b4a;
L8c660b4a: r[9]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r9 */
 goto L8c660b4c;
L8c660b4c: r[0]=sx(m,r[0]+0x4u,1); /* mov.b @(0x4,r0),r0 */
 goto L8c660b4e;
L8c660b4e: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c660b50;
L8c660b50: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto L8c660b52;
L8c660b52: take=(*sr&1u);r[9]=rd(m,r[9]+0x8u,4);if(take)goto L8c660b6e;goto L8c660b56; /* bt/s 0x8c660b6e */
 goto L8c660b56;
L8c660b56: r[1]=sx(m,0x8c660becu,2); /* mov.w 0x8c660bec,r1 */
 goto L8c660b58;
L8c660b58: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c660b5a;
L8c660b5a: if((*sr&1u))goto L8c660b18; /* bt 0x8c660b18 */
 goto L8c660b5c;
L8c660b5c: r[1]=sx(m,0x8c660beeu,2); /* mov.w 0x8c660bee,r1 */
 goto L8c660b5e;
L8c660b5e: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c660b60;
L8c660b60: if(!(*sr&1u))goto L8c660b68; /* bf 0x8c660b68 */
 goto L8c660b62;
L8c660b62: r[0]=rd(m,0x8c660c04u,4); /* mov.l 0x8c660c04,r0 */
 goto L8c660b64;
L8c660b64: goto L8c660b9a; /* bra 0x8c660b9a */
 goto L8c660b68;
L8c660b68: r[0]=sx(m,0x8c660bf0u,2); /* mov.w 0x8c660bf0,r0 */
 goto L8c660b6a;
L8c660b6a: goto L8c660b9a; /* bra 0x8c660b9a */
 goto L8c660b6e;
L8c660b6e: r[6]=rd(m,r[11]+0x1cu,4); /* mov.l @(0x1c,r11),r6 */
 goto L8c660b70;
L8c660b70: r[0]=68u; /* mov #0x44,r0 */
 goto L8c660b72;
L8c660b72: r[3]=rd(m,0x8c660c08u,4); /* mov.l 0x8c660c08,r3 */
 goto L8c660b74;
L8c660b74: r[5]=r[9]; /* mov r9,r5 */
 goto L8c660b76;
L8c660b76: r[6]=rd(m,r[6]+r[0],4); /* mov.l @(r0,r6),r6 */
 goto L8c660b78;
L8c660b78: r[5]+=12u; /* add #0xc,r5 */
 goto L8c660b7a;
L8c660b7a: target=r[3];*pr=0x8c660b7eu;r[4]=r[8];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c660b7e;
L8c660b7e: r[2]=rd(m,r[11]+0x1cu,4); /* mov.l @(0x1c,r11),r2 */
 goto L8c660b80;
L8c660b80: r[0]=68u; /* mov #0x44,r0 */
 goto L8c660b82;
L8c660b82: r[10]+=1u; /* add #0x1,r10 */
 goto L8c660b84;
L8c660b84: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c660b86;
L8c660b86: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c660b88;
L8c660b88: r[8]+=r[3]; /* add r3,r8 */
 goto L8c660b8a;
L8c660b8a: r[2]+=1u; /* add #0x1,r2 */
 goto L8c660b8c;
L8c660b8c: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c660b8e;
L8c660b8e: r[3]=rd(m,r[11]+0x1cu,4); /* mov.l @(0x1c,r11),r3 */
 goto L8c660b90;
L8c660b90: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c660b92;
L8c660b92: r[2]=rd(m,r[3]+0x3cu,4); /* mov.l @(0x3c,r3),r2 */
 goto L8c660b94;
L8c660b94: *sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[2]); /* cmp/ge r2,r1 */
 goto L8c660b96;
L8c660b96: if(!(*sr&1u))goto L8c660b18; /* bf 0x8c660b18 */
 goto L8c660b98;
L8c660b98: r[0]=0u; /* mov #0x0,r0 */
 goto L8c660b9a;
L8c660b9a: r[15]+=12u; /* add #0xc,r15 */
 goto L8c660b9c;
L8c660b9c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c660b9e;
L8c660b9e: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c660ba0;
L8c660ba0: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c660ba2;
L8c660ba2: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c660ba4;
L8c660ba4: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c660ba6;
L8c660ba6: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c660ba8;
L8c660ba8: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c660baa;
L8c660baa: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_input_backend_finalize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_backend_finalize_core(0,m,f,sr,r,pr,fpul,macl,ctx,call);}
SA_STAGE_EXPORT int sa_input_backend_finalize_resume(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_backend_finalize_core(*pr,m,f,sr,r,pr,fpul,macl,ctx,call);}
