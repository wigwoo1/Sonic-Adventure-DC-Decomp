/* Static C generated from retail instruction exports by
 * recover_overlay_object_variants.py. Descriptive symbols are inferred.
 * No runtime opcode decoding. Guest RAM is 16 MiB; invalid pointers, hardware
 * exceptions and MMIO are not modeled. Native selector integration and other explicit callees use the bridge.
 * Host 0 means complete; the retail result is in R0. Missing call service
 * returns -2 atomically. A service error returns its status with partial state.
 * These routines use integer instructions and bank/width-aware FMOV only;
 * no floating-point arithmetic or PR=0 restriction is introduced.
 */
#include "fpu_cleanup_transfer.h"
typedef int (*sa_shared_service)(void *,uint32_t,uint8_t *,
    struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)value:(uint32_t)(int32_t)(int16_t)value;
}
static void wr(uint8_t *m,uint32_t a,uint32_t value,unsigned n){
 for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(value>>(8*i));
}
static void fload(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  f->bank[bank][reg]=rd(m,a,4);f->bank[bank][reg+1]=rd(m,a+4u,4);
 }else f->bank[bank][reg]=rd(m,a,4);
}
static void fstore(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  wr(m,a,f->bank[bank][reg],4);wr(m,a+4u,f->bank[bank][reg+1],4);
 }else wr(m,a,f->bank[bank][reg],4);
}
SA_STAGE_EXPORT int sa_overlay_variant_palette_0c904aea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)sr;(void)fload;(void)fstore;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc904aea;
Lc904aea: r[15]-=4;wr(m,r[15],r[14],4);goto Lc904aec; /* mov.l r14,@-r15 */
Lc904aec: r[15]-=4;wr(m,r[15],r[13],4);goto Lc904aee; /* mov.l r13,@-r15 */
Lc904aee: r[15]-=4;wr(m,r[15],r[12],4);goto Lc904af0; /* mov.l r12,@-r15 */
Lc904af0: r[15]-=4;wr(m,r[15],r[10],4);goto Lc904af2; /* mov.l r10,@-r15 */
Lc904af2: r[15]+=4294967232u;goto Lc904af4; /* add #-0x40,r15 */
Lc904af4: r[3]=rd(m,0x0c904c60u,4);goto Lc904af6; /* mov.l 0x0c904c60,r3 */
Lc904af6: r[2]=r[15];goto Lc904af8; /* mov r15,r2 */
Lc904af8: r[2]+=8u;goto Lc904afa; /* add #0x8,r2 */
Lc904afa: r[4]=r[15];goto Lc904afc; /* mov r15,r4 */
Lc904afc: r[1]=rd(m,r[3],4);goto Lc904afe; /* mov.l @r3,r1 */
Lc904afe: r[4]+=8u;goto Lc904b00; /* add #0x8,r4 */
Lc904b00: r[0]=rd(m,r[3]+0x4u,4);goto Lc904b02; /* mov.l @(0x4,r3),r0 */
Lc904b02: wr(m,r[2],r[1],4);goto Lc904b04; /* mov.l r1,@r2 */
Lc904b04: wr(m,r[2]+0x4u,r[0],4);goto Lc904b06; /* mov.l r0,@(0x4,r2) */
Lc904b06: r[1]=rd(m,r[3]+0x8u,4);goto Lc904b08; /* mov.l @(0x8,r3),r1 */
Lc904b08: r[0]=rd(m,r[3]+0xcu,4);goto Lc904b0a; /* mov.l @(0xc,r3),r0 */
Lc904b0a: wr(m,r[2]+0x8u,r[1],4);goto Lc904b0c; /* mov.l r1,@(0x8,r2) */
Lc904b0c: wr(m,r[2]+0xcu,r[0],4);goto Lc904b0e; /* mov.l r0,@(0xc,r2) */
Lc904b0e: r[1]=rd(m,r[3]+0x10u,4);goto Lc904b10; /* mov.l @(0x10,r3),r1 */
Lc904b10: r[0]=rd(m,r[3]+0x14u,4);goto Lc904b12; /* mov.l @(0x14,r3),r0 */
Lc904b12: wr(m,r[2]+0x10u,r[1],4);goto Lc904b14; /* mov.l r1,@(0x10,r2) */
Lc904b14: wr(m,r[2]+0x14u,r[0],4);goto Lc904b16; /* mov.l r0,@(0x14,r2) */
Lc904b16: r[1]=rd(m,r[3]+0x18u,4);goto Lc904b18; /* mov.l @(0x18,r3),r1 */
Lc904b18: r[0]=rd(m,r[3]+0x1cu,4);goto Lc904b1a; /* mov.l @(0x1c,r3),r0 */
Lc904b1a: wr(m,r[2]+0x18u,r[1],4);goto Lc904b1c; /* mov.l r1,@(0x18,r2) */
Lc904b1c: wr(m,r[2]+0x1cu,r[0],4);goto Lc904b1e; /* mov.l r0,@(0x1c,r2) */
Lc904b1e: r[1]=rd(m,r[3]+0x20u,4);goto Lc904b20; /* mov.l @(0x20,r3),r1 */
Lc904b20: r[0]=rd(m,r[3]+0x24u,4);goto Lc904b22; /* mov.l @(0x24,r3),r0 */
Lc904b22: wr(m,r[2]+0x20u,r[1],4);goto Lc904b24; /* mov.l r1,@(0x20,r2) */
Lc904b24: wr(m,r[2]+0x24u,r[0],4);goto Lc904b26; /* mov.l r0,@(0x24,r2) */
Lc904b26: r[1]=rd(m,r[3]+0x28u,4);goto Lc904b28; /* mov.l @(0x28,r3),r1 */
Lc904b28: r[0]=rd(m,r[3]+0x2cu,4);goto Lc904b2a; /* mov.l @(0x2c,r3),r0 */
Lc904b2a: wr(m,r[2]+0x28u,r[1],4);goto Lc904b2c; /* mov.l r1,@(0x28,r2) */
Lc904b2c: wr(m,r[2]+0x2cu,r[0],4);goto Lc904b2e; /* mov.l r0,@(0x2c,r2) */
Lc904b2e: r[1]=rd(m,r[3]+0x30u,4);goto Lc904b30; /* mov.l @(0x30,r3),r1 */
Lc904b30: r[0]=rd(m,r[3]+0x34u,4);goto Lc904b32; /* mov.l @(0x34,r3),r0 */
Lc904b32: wr(m,r[2]+0x30u,r[1],4);goto Lc904b34; /* mov.l r1,@(0x30,r2) */
Lc904b34: wr(m,r[2]+0x34u,r[0],4);goto Lc904b36; /* mov.l r0,@(0x34,r2) */
Lc904b36: r[1]=r[15];goto Lc904b38; /* mov r15,r1 */
Lc904b38: r[2]=rd(m,0x0c904c64u,4);goto Lc904b3a; /* mov.l 0x0c904c64,r2 */
Lc904b3a: r[6]=r[4];goto Lc904b3c; /* mov r4,r6 */
Lc904b3c: r[10]=2u;goto Lc904b3e; /* mov #0x2,r10 */
Lc904b3e: r[7]=r[15];goto Lc904b40; /* mov r15,r7 */
Lc904b40: r[0]=rd(m,r[2],4);goto Lc904b42; /* mov.l @r2,r0 */
Lc904b42: r[10]<<=8;goto Lc904b44; /* shll8 r10 */
Lc904b44: r[3]=rd(m,r[2]+0x4u,4);goto Lc904b46; /* mov.l @(0x4,r2),r3 */
Lc904b46: r[10]+=72u;goto Lc904b48; /* add #0x48,r10 */
Lc904b48: wr(m,r[1],r[0],4);goto Lc904b4a; /* mov.l r0,@r1 */
Lc904b4a: r[0]=4294967276u;goto Lc904b4c; /* mov #-0x14,r0 */
Lc904b4c: r[0]=r[0]&255u;goto Lc904b4e; /* extu.b r0,r0 */
Lc904b4e: wr(m,r[1]+0x4u,r[3],4);goto Lc904b50; /* mov.l r3,@(0x4,r1) */
Lc904b50: r[4]=rd(m,r[5]+r[0],4);goto Lc904b52; /* mov.l @(r0,r5),r4 */
Lc904b52: r[0]=2u;goto Lc904b54; /* mov #0x2,r0 */
Lc904b54: r[0]<<=8;goto Lc904b56; /* shll8 r0 */
Lc904b56: r[0]+=4294967186u;goto Lc904b58; /* add #-0x6e,r0 */
Lc904b58: r[4]=sx(m,r[4]+r[0],1);goto Lc904b5a; /* mov.b @(r0,r4),r4 */
Lc904b5a: r[1]=3u;goto Lc904b5c; /* mov #0x3,r1 */
Lc904b5c: r[10]+=r[5];goto Lc904b5e; /* add r5,r10 */
Lc904b5e: r[3]=r[4]&255u;goto Lc904b60; /* extu.b r4,r3 */
Lc904b60: *sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[1]);goto Lc904b62; /* cmp/gt r1,r3 */
Lc904b62: take=!(*sr&1u);r[14]=0u;if(take)goto Lc904b68;goto Lc904b66; /* bf/s 0x0c904b68 */
Lc904b66: r[4]=r[14];goto Lc904b68; /* mov r14,r4 */
Lc904b68: r[0]=rd(m,r[6],4);goto Lc904b6a; /* mov.l @r6,r0 */
Lc904b6a: r[13]=r[4]&255u;goto Lc904b6c; /* extu.b r4,r13 */
Lc904b6c: r[12]=rd(m,0x0c904c68u,4);goto Lc904b6e; /* mov.l 0x0c904c68,r12 */
Lc904b6e: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto Lc904b70; /* shll r13 */
Lc904b70: *sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc904b72; /* cmp/eq #-0x1,r0 */
Lc904b72: take=(*sr&1u);r[13]+=r[7];if(take)goto Lc904bb0;goto Lc904b76; /* bt/s 0x0c904bb0 */
Lc904b76: r[5]=rd(m,r[6],4);goto Lc904b78; /* mov.l @r6,r5 */
Lc904b78: r[3]=r[10];goto Lc904b7a; /* mov r10,r3 */
Lc904b7a: r[3]+=4u;goto Lc904b7c; /* add #0x4,r3 */
Lc904b7c: r[5]<<=2;goto Lc904b7e; /* shll2 r5 */
Lc904b7e: r[5]+=r[3];goto Lc904b80; /* add r3,r5 */
Lc904b80: r[5]=rd(m,r[5],4);goto Lc904b82; /* mov.l @r5,r5 */
Lc904b82: r[4]=rd(m,r[5]+0x4u,4);goto Lc904b84; /* mov.l @(0x4,r5),r4 */
Lc904b84: r[5]=r[14];goto Lc904b86; /* mov r14,r5 */
Lc904b86: r[0]=sx(m,r[4]+0x16u,2);goto Lc904b88; /* mov.w @(0x16,r4),r0 */
Lc904b88: r[1]=rd(m,r[4]+0x10u,4);goto Lc904b8a; /* mov.l @(0x10,r4),r1 */
Lc904b8a: r[4]=0u;goto Lc904b8c; /* mov #0x0,r4 */
Lc904b8c: r[7]=r[0]&65535u;goto Lc904b8e; /* extu.w r0,r7 */
Lc904b8e: *sr=(*sr&~1u)|((int32_t)r[7]>0);goto Lc904b90; /* cmp/pl r7 */
Lc904b90: take=!(*sr&1u);r[4]+=r[1];if(take)goto Lc904ba8;goto Lc904b94; /* bf/s 0x0c904ba8 */
Lc904b94: r[2]=sx(m,r[13],2);goto Lc904b96; /* mov.w @r13,r2 */
Lc904b96: r[5]+=1u;goto Lc904b98; /* add #0x1,r5 */
Lc904b98: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[7]);goto Lc904b9a; /* cmp/ge r7,r5 */
Lc904b9a: r[2]=r[2]&65535u;goto Lc904b9c; /* extu.w r2,r2 */
Lc904b9c: wr(m,r[4]+0xcu,r[2],4);goto Lc904b9e; /* mov.l r2,@(0xc,r4) */
Lc904b9e: r[3]=rd(m,r[4]+0x10u,4);goto Lc904ba0; /* mov.l @(0x10,r4),r3 */
Lc904ba0: r[3]|=r[12];goto Lc904ba2; /* or r12,r3 */
Lc904ba2: wr(m,r[4]+0x10u,r[3],4);goto Lc904ba4; /* mov.l r3,@(0x10,r4) */
Lc904ba4: take=!(*sr&1u);r[4]+=20u;if(take)goto Lc904b94;goto Lc904ba8; /* bf/s 0x0c904b94 */
Lc904ba8: r[6]+=4u;goto Lc904baa; /* add #0x4,r6 */
Lc904baa: r[0]=rd(m,r[6],4);goto Lc904bac; /* mov.l @r6,r0 */
Lc904bac: *sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc904bae; /* cmp/eq #-0x1,r0 */
Lc904bae: if(!(*sr&1u))goto Lc904b76;goto Lc904bb0; /* bf 0x0c904b76 */
Lc904bb0: r[15]+=64u;goto Lc904bb2; /* add #0x40,r15 */
Lc904bb2: r[10]=rd(m,r[15],4);r[15]+=4;goto Lc904bb4; /* mov.l @r15+,r10 */
Lc904bb4: r[12]=rd(m,r[15],4);r[15]+=4;goto Lc904bb6; /* mov.l @r15+,r12 */
Lc904bb6: r[13]=rd(m,r[15],4);r[15]+=4;goto Lc904bb8; /* mov.l @r15+,r13 */
Lc904bb8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_variant_material_158_0c904bbc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)sr;(void)fload;(void)fstore;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc904bbc;
Lc904bbc: r[15]-=4;wr(m,r[15],r[14],4);goto Lc904bbe; /* mov.l r14,@-r15 */
Lc904bbe: r[15]-=4;wr(m,r[15],r[13],4);goto Lc904bc0; /* mov.l r13,@-r15 */
Lc904bc0: r[15]-=4;wr(m,r[15],r[11],4);goto Lc904bc2; /* mov.l r11,@-r15 */
Lc904bc2: r[15]-=4;wr(m,r[15],r[10],4);goto Lc904bc4; /* mov.l r10,@-r15 */
Lc904bc4: r[15]+=4294967240u;goto Lc904bc6; /* add #-0x38,r15 */
Lc904bc6: r[3]=rd(m,0x0c904c6cu,4);goto Lc904bc8; /* mov.l 0x0c904c6c,r3 */
Lc904bc8: r[2]=r[15];goto Lc904bca; /* mov r15,r2 */
Lc904bca: r[10]=2u;goto Lc904bcc; /* mov #0x2,r10 */
Lc904bcc: r[4]=r[15];goto Lc904bce; /* mov r15,r4 */
Lc904bce: r[1]=rd(m,r[3],4);goto Lc904bd0; /* mov.l @r3,r1 */
Lc904bd0: r[0]=rd(m,r[3]+0x4u,4);goto Lc904bd2; /* mov.l @(0x4,r3),r0 */
Lc904bd2: wr(m,r[2],r[1],4);goto Lc904bd4; /* mov.l r1,@r2 */
Lc904bd4: wr(m,r[2]+0x4u,r[0],4);goto Lc904bd6; /* mov.l r0,@(0x4,r2) */
Lc904bd6: r[0]=rd(m,r[3]+0xcu,4);goto Lc904bd8; /* mov.l @(0xc,r3),r0 */
Lc904bd8: r[1]=rd(m,r[3]+0x8u,4);goto Lc904bda; /* mov.l @(0x8,r3),r1 */
Lc904bda: wr(m,r[2]+0x8u,r[1],4);goto Lc904bdc; /* mov.l r1,@(0x8,r2) */
Lc904bdc: wr(m,r[2]+0xcu,r[0],4);goto Lc904bde; /* mov.l r0,@(0xc,r2) */
Lc904bde: r[0]=rd(m,r[3]+0x14u,4);goto Lc904be0; /* mov.l @(0x14,r3),r0 */
Lc904be0: r[1]=rd(m,r[3]+0x10u,4);goto Lc904be2; /* mov.l @(0x10,r3),r1 */
Lc904be2: wr(m,r[2]+0x10u,r[1],4);goto Lc904be4; /* mov.l r1,@(0x10,r2) */
Lc904be4: wr(m,r[2]+0x14u,r[0],4);goto Lc904be6; /* mov.l r0,@(0x14,r2) */
Lc904be6: r[0]=rd(m,r[3]+0x1cu,4);goto Lc904be8; /* mov.l @(0x1c,r3),r0 */
Lc904be8: r[1]=rd(m,r[3]+0x18u,4);goto Lc904bea; /* mov.l @(0x18,r3),r1 */
Lc904bea: wr(m,r[2]+0x18u,r[1],4);goto Lc904bec; /* mov.l r1,@(0x18,r2) */
Lc904bec: wr(m,r[2]+0x1cu,r[0],4);goto Lc904bee; /* mov.l r0,@(0x1c,r2) */
Lc904bee: r[0]=rd(m,r[3]+0x24u,4);goto Lc904bf0; /* mov.l @(0x24,r3),r0 */
Lc904bf0: r[1]=rd(m,r[3]+0x20u,4);goto Lc904bf2; /* mov.l @(0x20,r3),r1 */
Lc904bf2: wr(m,r[2]+0x20u,r[1],4);goto Lc904bf4; /* mov.l r1,@(0x20,r2) */
Lc904bf4: wr(m,r[2]+0x24u,r[0],4);goto Lc904bf6; /* mov.l r0,@(0x24,r2) */
Lc904bf6: r[0]=rd(m,r[3]+0x2cu,4);goto Lc904bf8; /* mov.l @(0x2c,r3),r0 */
Lc904bf8: r[1]=rd(m,r[3]+0x28u,4);goto Lc904bfa; /* mov.l @(0x28,r3),r1 */
Lc904bfa: wr(m,r[2]+0x28u,r[1],4);goto Lc904bfc; /* mov.l r1,@(0x28,r2) */
Lc904bfc: wr(m,r[2]+0x2cu,r[0],4);goto Lc904bfe; /* mov.l r0,@(0x2c,r2) */
Lc904bfe: r[0]=rd(m,r[3]+0x34u,4);goto Lc904c00; /* mov.l @(0x34,r3),r0 */
Lc904c00: r[1]=rd(m,r[3]+0x30u,4);goto Lc904c02; /* mov.l @(0x30,r3),r1 */
Lc904c02: wr(m,r[2]+0x30u,r[1],4);goto Lc904c04; /* mov.l r1,@(0x30,r2) */
Lc904c04: wr(m,r[2]+0x34u,r[0],4);goto Lc904c06; /* mov.l r0,@(0x34,r2) */
Lc904c06: r[10]<<=8;goto Lc904c08; /* shll8 r10 */
Lc904c08: r[14]=rd(m,0x0c904c68u,4);goto Lc904c0a; /* mov.l 0x0c904c68,r14 */
Lc904c0a: r[10]+=72u;goto Lc904c0c; /* add #0x48,r10 */
Lc904c0c: r[10]+=r[5];goto Lc904c0e; /* add r5,r10 */
Lc904c0e: r[5]=r[4];goto Lc904c10; /* mov r4,r5 */
Lc904c10: r[0]=rd(m,r[5],4);goto Lc904c12; /* mov.l @r5,r0 */
Lc904c12: r[13]=4294967198u;goto Lc904c14; /* mov #-0x62,r13 */
Lc904c14: r[13]=r[13]&255u;goto Lc904c16; /* extu.b r13,r13 */
Lc904c16: *sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc904c18; /* cmp/eq #-0x1,r0 */
Lc904c18: take=(*sr&1u);r[11]=0u;if(take)goto Lc904c52;goto Lc904c1c; /* bt/s 0x0c904c52 */
Lc904c1c: r[6]=rd(m,r[5],4);goto Lc904c1e; /* mov.l @r5,r6 */
Lc904c1e: r[3]=r[10];goto Lc904c20; /* mov r10,r3 */
Lc904c20: r[3]+=4u;goto Lc904c22; /* add #0x4,r3 */
Lc904c22: r[6]<<=2;goto Lc904c24; /* shll2 r6 */
Lc904c24: r[6]+=r[3];goto Lc904c26; /* add r3,r6 */
Lc904c26: r[6]=rd(m,r[6],4);goto Lc904c28; /* mov.l @r6,r6 */
Lc904c28: r[4]=rd(m,r[6]+0x4u,4);goto Lc904c2a; /* mov.l @(0x4,r6),r4 */
Lc904c2a: r[6]=r[11];goto Lc904c2c; /* mov r11,r6 */
Lc904c2c: r[0]=sx(m,r[4]+0x16u,2);goto Lc904c2e; /* mov.w @(0x16,r4),r0 */
Lc904c2e: r[1]=rd(m,r[4]+0x10u,4);goto Lc904c30; /* mov.l @(0x10,r4),r1 */
Lc904c30: r[4]=0u;goto Lc904c32; /* mov #0x0,r4 */
Lc904c32: r[7]=r[0]&65535u;goto Lc904c34; /* extu.w r0,r7 */
Lc904c34: *sr=(*sr&~1u)|((int32_t)r[7]>0);goto Lc904c36; /* cmp/pl r7 */
Lc904c36: take=!(*sr&1u);r[4]+=r[1];if(take)goto Lc904c4a;goto Lc904c3a; /* bf/s 0x0c904c4a */
Lc904c3a: r[6]+=1u;goto Lc904c3c; /* add #0x1,r6 */
Lc904c3c: wr(m,r[4]+0xcu,r[13],4);goto Lc904c3e; /* mov.l r13,@(0xc,r4) */
Lc904c3e: r[2]=rd(m,r[4]+0x10u,4);goto Lc904c40; /* mov.l @(0x10,r4),r2 */
Lc904c40: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto Lc904c42; /* cmp/ge r7,r6 */
Lc904c42: r[2]|=r[14];goto Lc904c44; /* or r14,r2 */
Lc904c44: wr(m,r[4]+0x10u,r[2],4);goto Lc904c46; /* mov.l r2,@(0x10,r4) */
Lc904c46: take=!(*sr&1u);r[4]+=20u;if(take)goto Lc904c3a;goto Lc904c4a; /* bf/s 0x0c904c3a */
Lc904c4a: r[5]+=4u;goto Lc904c4c; /* add #0x4,r5 */
Lc904c4c: r[0]=rd(m,r[5],4);goto Lc904c4e; /* mov.l @r5,r0 */
Lc904c4e: *sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc904c50; /* cmp/eq #-0x1,r0 */
Lc904c50: if(!(*sr&1u))goto Lc904c1c;goto Lc904c52; /* bf 0x0c904c1c */
Lc904c52: r[15]+=56u;goto Lc904c54; /* add #0x38,r15 */
Lc904c54: r[10]=rd(m,r[15],4);r[15]+=4;goto Lc904c56; /* mov.l @r15+,r10 */
Lc904c56: r[11]=rd(m,r[15],4);r[15]+=4;goto Lc904c58; /* mov.l @r15+,r11 */
Lc904c58: r[13]=rd(m,r[15],4);r[15]+=4;goto Lc904c5a; /* mov.l @r15+,r13 */
Lc904c5a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_variant_material_161_0c904c80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)sr;(void)fload;(void)fstore;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc904c80;
Lc904c80: r[15]-=4;wr(m,r[15],r[14],4);goto Lc904c82; /* mov.l r14,@-r15 */
Lc904c82: r[15]-=4;wr(m,r[15],r[13],4);goto Lc904c84; /* mov.l r13,@-r15 */
Lc904c84: r[15]-=4;wr(m,r[15],r[11],4);goto Lc904c86; /* mov.l r11,@-r15 */
Lc904c86: r[15]-=4;wr(m,r[15],r[10],4);goto Lc904c88; /* mov.l r10,@-r15 */
Lc904c88: r[15]+=4294967240u;goto Lc904c8a; /* add #-0x38,r15 */
Lc904c8a: r[3]=rd(m,0x0c904e60u,4);goto Lc904c8c; /* mov.l 0x0c904e60,r3 */
Lc904c8c: r[2]=r[15];goto Lc904c8e; /* mov r15,r2 */
Lc904c8e: r[10]=2u;goto Lc904c90; /* mov #0x2,r10 */
Lc904c90: r[4]=r[15];goto Lc904c92; /* mov r15,r4 */
Lc904c92: r[1]=rd(m,r[3],4);goto Lc904c94; /* mov.l @r3,r1 */
Lc904c94: r[0]=rd(m,r[3]+0x4u,4);goto Lc904c96; /* mov.l @(0x4,r3),r0 */
Lc904c96: wr(m,r[2],r[1],4);goto Lc904c98; /* mov.l r1,@r2 */
Lc904c98: wr(m,r[2]+0x4u,r[0],4);goto Lc904c9a; /* mov.l r0,@(0x4,r2) */
Lc904c9a: r[0]=rd(m,r[3]+0xcu,4);goto Lc904c9c; /* mov.l @(0xc,r3),r0 */
Lc904c9c: r[1]=rd(m,r[3]+0x8u,4);goto Lc904c9e; /* mov.l @(0x8,r3),r1 */
Lc904c9e: wr(m,r[2]+0x8u,r[1],4);goto Lc904ca0; /* mov.l r1,@(0x8,r2) */
Lc904ca0: wr(m,r[2]+0xcu,r[0],4);goto Lc904ca2; /* mov.l r0,@(0xc,r2) */
Lc904ca2: r[0]=rd(m,r[3]+0x14u,4);goto Lc904ca4; /* mov.l @(0x14,r3),r0 */
Lc904ca4: r[1]=rd(m,r[3]+0x10u,4);goto Lc904ca6; /* mov.l @(0x10,r3),r1 */
Lc904ca6: wr(m,r[2]+0x10u,r[1],4);goto Lc904ca8; /* mov.l r1,@(0x10,r2) */
Lc904ca8: wr(m,r[2]+0x14u,r[0],4);goto Lc904caa; /* mov.l r0,@(0x14,r2) */
Lc904caa: r[0]=rd(m,r[3]+0x1cu,4);goto Lc904cac; /* mov.l @(0x1c,r3),r0 */
Lc904cac: r[1]=rd(m,r[3]+0x18u,4);goto Lc904cae; /* mov.l @(0x18,r3),r1 */
Lc904cae: wr(m,r[2]+0x18u,r[1],4);goto Lc904cb0; /* mov.l r1,@(0x18,r2) */
Lc904cb0: wr(m,r[2]+0x1cu,r[0],4);goto Lc904cb2; /* mov.l r0,@(0x1c,r2) */
Lc904cb2: r[0]=rd(m,r[3]+0x24u,4);goto Lc904cb4; /* mov.l @(0x24,r3),r0 */
Lc904cb4: r[1]=rd(m,r[3]+0x20u,4);goto Lc904cb6; /* mov.l @(0x20,r3),r1 */
Lc904cb6: wr(m,r[2]+0x20u,r[1],4);goto Lc904cb8; /* mov.l r1,@(0x20,r2) */
Lc904cb8: wr(m,r[2]+0x24u,r[0],4);goto Lc904cba; /* mov.l r0,@(0x24,r2) */
Lc904cba: r[0]=rd(m,r[3]+0x2cu,4);goto Lc904cbc; /* mov.l @(0x2c,r3),r0 */
Lc904cbc: r[1]=rd(m,r[3]+0x28u,4);goto Lc904cbe; /* mov.l @(0x28,r3),r1 */
Lc904cbe: wr(m,r[2]+0x28u,r[1],4);goto Lc904cc0; /* mov.l r1,@(0x28,r2) */
Lc904cc0: wr(m,r[2]+0x2cu,r[0],4);goto Lc904cc2; /* mov.l r0,@(0x2c,r2) */
Lc904cc2: r[0]=rd(m,r[3]+0x34u,4);goto Lc904cc4; /* mov.l @(0x34,r3),r0 */
Lc904cc4: r[1]=rd(m,r[3]+0x30u,4);goto Lc904cc6; /* mov.l @(0x30,r3),r1 */
Lc904cc6: wr(m,r[2]+0x30u,r[1],4);goto Lc904cc8; /* mov.l r1,@(0x30,r2) */
Lc904cc8: wr(m,r[2]+0x34u,r[0],4);goto Lc904cca; /* mov.l r0,@(0x34,r2) */
Lc904cca: r[10]<<=8;goto Lc904ccc; /* shll8 r10 */
Lc904ccc: r[14]=rd(m,0x0c904e64u,4);goto Lc904cce; /* mov.l 0x0c904e64,r14 */
Lc904cce: r[10]+=72u;goto Lc904cd0; /* add #0x48,r10 */
Lc904cd0: r[10]+=r[5];goto Lc904cd2; /* add r5,r10 */
Lc904cd2: r[5]=r[4];goto Lc904cd4; /* mov r4,r5 */
Lc904cd4: r[0]=rd(m,r[5],4);goto Lc904cd6; /* mov.l @r5,r0 */
Lc904cd6: r[13]=4294967201u;goto Lc904cd8; /* mov #-0x5f,r13 */
Lc904cd8: r[13]=r[13]&255u;goto Lc904cda; /* extu.b r13,r13 */
Lc904cda: *sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc904cdc; /* cmp/eq #-0x1,r0 */
Lc904cdc: take=(*sr&1u);r[11]=0u;if(take)goto Lc904d16;goto Lc904ce0; /* bt/s 0x0c904d16 */
Lc904ce0: r[6]=rd(m,r[5],4);goto Lc904ce2; /* mov.l @r5,r6 */
Lc904ce2: r[3]=r[10];goto Lc904ce4; /* mov r10,r3 */
Lc904ce4: r[3]+=4u;goto Lc904ce6; /* add #0x4,r3 */
Lc904ce6: r[6]<<=2;goto Lc904ce8; /* shll2 r6 */
Lc904ce8: r[6]+=r[3];goto Lc904cea; /* add r3,r6 */
Lc904cea: r[6]=rd(m,r[6],4);goto Lc904cec; /* mov.l @r6,r6 */
Lc904cec: r[4]=rd(m,r[6]+0x4u,4);goto Lc904cee; /* mov.l @(0x4,r6),r4 */
Lc904cee: r[6]=r[11];goto Lc904cf0; /* mov r11,r6 */
Lc904cf0: r[0]=sx(m,r[4]+0x16u,2);goto Lc904cf2; /* mov.w @(0x16,r4),r0 */
Lc904cf2: r[1]=rd(m,r[4]+0x10u,4);goto Lc904cf4; /* mov.l @(0x10,r4),r1 */
Lc904cf4: r[4]=0u;goto Lc904cf6; /* mov #0x0,r4 */
Lc904cf6: r[7]=r[0]&65535u;goto Lc904cf8; /* extu.w r0,r7 */
Lc904cf8: *sr=(*sr&~1u)|((int32_t)r[7]>0);goto Lc904cfa; /* cmp/pl r7 */
Lc904cfa: take=!(*sr&1u);r[4]+=r[1];if(take)goto Lc904d0e;goto Lc904cfe; /* bf/s 0x0c904d0e */
Lc904cfe: r[6]+=1u;goto Lc904d00; /* add #0x1,r6 */
Lc904d00: wr(m,r[4]+0xcu,r[13],4);goto Lc904d02; /* mov.l r13,@(0xc,r4) */
Lc904d02: r[2]=rd(m,r[4]+0x10u,4);goto Lc904d04; /* mov.l @(0x10,r4),r2 */
Lc904d04: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto Lc904d06; /* cmp/ge r7,r6 */
Lc904d06: r[2]|=r[14];goto Lc904d08; /* or r14,r2 */
Lc904d08: wr(m,r[4]+0x10u,r[2],4);goto Lc904d0a; /* mov.l r2,@(0x10,r4) */
Lc904d0a: take=!(*sr&1u);r[4]+=20u;if(take)goto Lc904cfe;goto Lc904d0e; /* bf/s 0x0c904cfe */
Lc904d0e: r[5]+=4u;goto Lc904d10; /* add #0x4,r5 */
Lc904d10: r[0]=rd(m,r[5],4);goto Lc904d12; /* mov.l @r5,r0 */
Lc904d12: *sr=(*sr&~1u)|(r[0]==0xffffffffu);goto Lc904d14; /* cmp/eq #-0x1,r0 */
Lc904d14: if(!(*sr&1u))goto Lc904ce0;goto Lc904d16; /* bf 0x0c904ce0 */
Lc904d16: r[15]+=56u;goto Lc904d18; /* add #0x38,r15 */
Lc904d18: r[10]=rd(m,r[15],4);r[15]+=4;goto Lc904d1a; /* mov.l @r15+,r10 */
Lc904d1a: r[11]=rd(m,r[15],4);r[15]+=4;goto Lc904d1c; /* mov.l @r15+,r11 */
Lc904d1c: r[13]=rd(m,r[15],4);r[15]+=4;goto Lc904d1e; /* mov.l @r15+,r13 */
Lc904d1e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
