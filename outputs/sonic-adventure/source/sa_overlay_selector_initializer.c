/* Static C generated from retail instruction exports by
 * recover_overlay_selector_initializer.py. Descriptive symbols are inferred.
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
SA_STAGE_EXPORT int sa_overlay_selector_initializer_0c90fde4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)sr;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto Lc90fde4;
Lc90fde4: r[15]-=4;wr(m,r[15],r[14],4);goto Lc90fde6; /* mov.l r14,@-r15 */
Lc90fde6: r[15]-=4;wr(m,r[15],r[13],4);goto Lc90fde8; /* mov.l r13,@-r15 */
Lc90fde8: r[15]-=4;wr(m,r[15],r[12],4);goto Lc90fdea; /* mov.l r12,@-r15 */
Lc90fdea: r[12]=0u;goto Lc90fdec; /* mov #0x0,r12 */
Lc90fdec: r[0]=r[12];goto Lc90fdee; /* mov r12,r0 */
Lc90fdee: r[15]-=4;wr(m,r[15],*pr,4);goto Lc90fdf0; /* sts.l PR,@-r15 */
Lc90fdf0: r[13]=rd(m,r[4]+0x20u,4);goto Lc90fdf2; /* mov.l @(0x20,r4),r13 */
Lc90fdf2: r[14]=rd(m,r[4]+0x24u,4);goto Lc90fdf4; /* mov.l @(0x24,r4),r14 */
Lc90fdf4: wr(m,r[13],r[12],1);goto Lc90fdf6; /* mov.b r12,@r13 */
Lc90fdf6: wr(m,r[13]+0x1u,r[0],1);goto Lc90fdf8; /* mov.b r0,@(0x1,r13) */
Lc90fdf8: wr(m,r[13]+0x6u,r[0],2);goto Lc90fdfa; /* mov.w r0,@(0x6,r13) */
Lc90fdfa: wr(m,r[13]+0x14u,r[0],4);goto Lc90fdfc; /* mov.l r0,@(0x14,r13) */
Lc90fdfc: wr(m,r[13]+0x1cu,r[0],4);goto Lc90fdfe; /* mov.l r0,@(0x1c,r13) */
Lc90fdfe: r[0]=124u;goto Lc90fe00; /* mov #0x7c,r0 */
Lc90fe00: wr(m,r[14]+r[0],r[12],4);goto Lc90fe02; /* mov.l r12,@(r0,r14) */
Lc90fe02: r[0]+=4u;goto Lc90fe04; /* add #0x4,r0 */
Lc90fe04: wr(m,r[14]+r[0],r[12],4);goto Lc90fe06; /* mov.l r12,@(r0,r14) */
Lc90fe06: r[0]+=4u;goto Lc90fe08; /* add #0x4,r0 */
Lc90fe08: wr(m,r[14]+r[0],r[12],4);goto Lc90fe0a; /* mov.l r12,@(r0,r14) */
Lc90fe0a: r[0]+=104u;goto Lc90fe0c; /* add #0x68,r0 */
Lc90fe0c: r[3]=rd(m,r[14]+r[0],4);goto Lc90fe0e; /* mov.l @(r0,r14),r3 */
Lc90fe0e: r[0]=36u;goto Lc90fe10; /* mov #0x24,r0 */
Lc90fe10: fload(m,f,3u,r[0]+r[3]);goto Lc90fe12; /* fmov.s @(r0,r3),fr3 */
Lc90fe12: r[0]=4294967264u;goto Lc90fe14; /* mov #-0x20,r0 */
Lc90fe14: r[0]=r[0]&255u;goto Lc90fe16; /* extu.b r0,r0 */
Lc90fe16: fstore(m,f,3u,r[0]+r[14]);goto Lc90fe18; /* fmov fr3,@(r0,r14) */
Lc90fe18: r[0]+=12u;goto Lc90fe1a; /* add #0xc,r0 */
Lc90fe1a: r[3]=rd(m,r[14]+r[0],4);goto Lc90fe1c; /* mov.l @(r0,r14),r3 */
Lc90fe1c: r[0]=32u;goto Lc90fe1e; /* mov #0x20,r0 */
Lc90fe1e: fload(m,f,3u,r[0]+r[3]);goto Lc90fe20; /* fmov.s @(r0,r3),fr3 */
Lc90fe20: r[0]=4294967260u;goto Lc90fe22; /* mov #-0x24,r0 */
Lc90fe22: r[0]=r[0]&255u;goto Lc90fe24; /* extu.b r0,r0 */
Lc90fe24: fstore(m,f,3u,r[0]+r[14]);goto Lc90fe26; /* fmov fr3,@(r0,r14) */
Lc90fe26: r[0]+=16u;goto Lc90fe28; /* add #0x10,r0 */
Lc90fe28: r[3]=rd(m,r[14]+r[0],4);goto Lc90fe2a; /* mov.l @(r0,r14),r3 */
Lc90fe2a: r[0]=40u;goto Lc90fe2c; /* mov #0x28,r0 */
Lc90fe2c: r[5]=r[14];goto Lc90fe2e; /* mov r14,r5 */
Lc90fe2e: fload(m,f,3u,r[0]+r[3]);goto Lc90fe30; /* fmov.s @(r0,r3),fr3 */
Lc90fe30: r[0]=4294967268u;goto Lc90fe32; /* mov #-0x1c,r0 */
Lc90fe32: r[0]=r[0]&255u;goto Lc90fe34; /* extu.b r0,r0 */
Lc90fe34: r[3]=rd(m,0x0c90feccu,4);goto Lc90fe36; /* mov.l 0x0c90fecc,r3 */
Lc90fe36: fstore(m,f,3u,r[0]+r[14]);goto Lc90fe38; /* fmov fr3,@(r0,r14) */
Lc90fe38: r[0]=r[12];goto Lc90fe3a; /* mov r12,r0 */
Lc90fe3a: wr(m,r[13]+0x6u,r[0],2);goto Lc90fe3c; /* mov.w r0,@(0x6,r13) */
Lc90fe3c: target=r[3];*pr=0x0c90fe40u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90fe40; /* jsr @r3 */
Lc90fe40: r[2]=rd(m,0x0c90fed0u,4);goto Lc90fe42; /* mov.l 0x0c90fed0,r2 */
Lc90fe42: r[5]=r[14];goto Lc90fe44; /* mov r14,r5 */
Lc90fe44: target=r[2];*pr=0x0c90fe48u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90fe48; /* jsr @r2 */
Lc90fe48: r[3]=rd(m,0x0c90fed4u,4);goto Lc90fe4a; /* mov.l 0x0c90fed4,r3 */
Lc90fe4a: r[5]=r[14];goto Lc90fe4c; /* mov r14,r5 */
Lc90fe4c: target=r[3];*pr=0x0c90fe50u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90fe50; /* jsr @r3 */
Lc90fe50: r[2]=rd(m,0x0c90fed8u,4);goto Lc90fe52; /* mov.l 0x0c90fed8,r2 */
Lc90fe52: r[5]=r[14];goto Lc90fe54; /* mov r14,r5 */
Lc90fe54: target=r[2];*pr=0x0c90fe58u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90fe58; /* jsr @r2 */
Lc90fe58: r[3]=rd(m,0x0c90fedcu,4);goto Lc90fe5a; /* mov.l 0x0c90fedc,r3 */
Lc90fe5a: r[5]=r[14];goto Lc90fe5c; /* mov r14,r5 */
Lc90fe5c: target=r[3];*pr=0x0c90fe60u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90fe60; /* jsr @r3 */
Lc90fe60: r[2]=rd(m,0x0c90fee0u,4);goto Lc90fe62; /* mov.l 0x0c90fee0,r2 */
Lc90fe62: r[5]=r[14];goto Lc90fe64; /* mov r14,r5 */
Lc90fe64: target=r[2];*pr=0x0c90fe68u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90fe68; /* jsr @r2 */
Lc90fe68: r[3]=rd(m,0x0c90fee4u,4);goto Lc90fe6a; /* mov.l 0x0c90fee4,r3 */
Lc90fe6a: r[5]=r[14];goto Lc90fe6c; /* mov r14,r5 */
Lc90fe6c: target=r[3];*pr=0x0c90fe70u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90fe70; /* jsr @r3 */
Lc90fe70: r[2]=rd(m,0x0c90fee8u,4);goto Lc90fe72; /* mov.l 0x0c90fee8,r2 */
Lc90fe72: r[5]=r[14];goto Lc90fe74; /* mov r14,r5 */
Lc90fe74: target=r[2];*pr=0x0c90fe78u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90fe78; /* jsr @r2 */
Lc90fe78: r[0]=96u;goto Lc90fe7a; /* mov #0x60,r0 */
Lc90fe7a: r[1]=rd(m,0x0c90feecu,4);goto Lc90fe7c; /* mov.l 0x0c90feec,r1 */
Lc90fe7c: r[2]=rd(m,r[14]+r[0],4);goto Lc90fe7e; /* mov.l @(r0,r14),r2 */
Lc90fe7e: r[3]=4294967281u;goto Lc90fe80; /* mov #-0xf,r3 */
Lc90fe80: r[2]&=r[3];goto Lc90fe82; /* and r3,r2 */
Lc90fe82: wr(m,r[14]+r[0],r[2],4);goto Lc90fe84; /* mov.l r2,@(r0,r14) */
Lc90fe84: r[0]=4294967276u;goto Lc90fe86; /* mov #-0x14,r0 */
Lc90fe86: r[0]=r[0]&255u;goto Lc90fe88; /* extu.b r0,r0 */
Lc90fe88: r[2]=rd(m,r[14]+r[0],4);goto Lc90fe8a; /* mov.l @(r0,r14),r2 */
Lc90fe8a: r[0]=54u;goto Lc90fe8c; /* mov #0x36,r0 */
Lc90fe8c: r[3]=sx(m,r[0]+r[2],2);goto Lc90fe8e; /* mov.w @(r0,r2),r3 */
Lc90fe8e: r[3]&=r[1];goto Lc90fe90; /* and r1,r3 */
Lc90fe90: wr(m,r[0]+r[2],r[3],2);goto Lc90fe92; /* mov.w r3,@(r0,r2) */
Lc90fe92: r[0]=116u;goto Lc90fe94; /* mov #0x74,r0 */
Lc90fe94: wr(m,r[14]+r[0],r[12],4);goto Lc90fe96; /* mov.l r12,@(r0,r14) */
Lc90fe96: r[0]=120u;goto Lc90fe98; /* mov #0x78,r0 */
Lc90fe98: wr(m,r[0]+r[14],r[12],2);goto Lc90fe9a; /* mov.w r12,@(r0,r14) */
Lc90fe9a: r[0]=0x0c90fef0u;goto Lc90fe9c; /* mova 0x0c90fef0,r0 */
Lc90fe9c: fload(m,f,3u,r[0]);goto Lc90fe9e; /* fmov.s @r0,fr3 */
Lc90fe9e: r[0]=4294967256u;goto Lc90fea0; /* mov #-0x28,r0 */
Lc90fea0: r[0]=r[0]&255u;goto Lc90fea2; /* extu.b r0,r0 */
Lc90fea2: fstore(m,f,3u,r[0]+r[14]);goto Lc90fea4; /* fmov fr3,@(r0,r14) */
Lc90fea4: r[0]+=20u;goto Lc90fea6; /* add #0x14,r0 */
Lc90fea6: r[0]=rd(m,r[14]+r[0],4);goto Lc90fea8; /* mov.l @(r0,r14),r0 */
Lc90fea8: r[2]=54u;goto Lc90feaa; /* mov #0x36,r2 */
Lc90feaa: r[0]=sx(m,r[0]+r[2],2);goto Lc90feac; /* mov.w @(r0,r2),r0 */
Lc90feac: r[0]=r[0]&65535u;goto Lc90feae; /* extu.w r0,r0 */
Lc90feae: *sr=(*sr&~1u)|!(r[0]&0x40u);goto Lc90feb0; /* tst #0x40,r0 */
Lc90feb0: if((*sr&1u))goto Lc90ff00;goto Lc90feb2; /* bt 0x0c90ff00 */
Lc90feb2: *pr=rd(m,r[15],4);r[15]+=4;goto Lc90feb4; /* lds.l @r15+,PR */
Lc90feb4: r[4]=r[13];goto Lc90feb6; /* mov r13,r4 */
Lc90feb6: r[5]=r[14];goto Lc90feb8; /* mov r14,r5 */
Lc90feb8: r[12]=rd(m,r[15],4);r[15]+=4;goto Lc90feba; /* mov.l @r15+,r12 */
Lc90feba: r[13]=rd(m,r[15],4);r[15]+=4;goto Lc90febc; /* mov.l @r15+,r13 */
Lc90febc: r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0xc90fb82u,m,f,sr,r,pr); /* bra 0x0c90fb82 */
Lc90ff00: r[0]=4294967276u;goto Lc90ff02; /* mov #-0x14,r0 */
Lc90ff02: r[0]=r[0]&255u;goto Lc90ff04; /* extu.b r0,r0 */
Lc90ff04: r[0]=rd(m,r[14]+r[0],4);goto Lc90ff06; /* mov.l @(r0,r14),r0 */
Lc90ff06: r[1]=54u;goto Lc90ff08; /* mov #0x36,r1 */
Lc90ff08: r[0]=sx(m,r[0]+r[1],2);goto Lc90ff0a; /* mov.w @(r0,r1),r0 */
Lc90ff0a: r[0]=r[0]&65535u;goto Lc90ff0c; /* extu.w r0,r0 */
Lc90ff0c: *sr=(*sr&~1u)|!(r[0]&0x4u);goto Lc90ff0e; /* tst #0x4,r0 */
Lc90ff0e: if((*sr&1u))goto Lc90ff1e;goto Lc90ff10; /* bt 0x0c90ff1e */
Lc90ff10: *pr=rd(m,r[15],4);r[15]+=4;goto Lc90ff12; /* lds.l @r15+,PR */
Lc90ff12: r[4]=r[13];goto Lc90ff14; /* mov r13,r4 */
Lc90ff14: r[5]=r[14];goto Lc90ff16; /* mov r14,r5 */
Lc90ff16: r[12]=rd(m,r[15],4);r[15]+=4;goto Lc90ff18; /* mov.l @r15+,r12 */
Lc90ff18: r[13]=rd(m,r[15],4);r[15]+=4;goto Lc90ff1a; /* mov.l @r15+,r13 */
Lc90ff1a: r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0xc90fca0u,m,f,sr,r,pr); /* bra 0x0c90fca0 */
Lc90ff1e: r[0]=4294967276u;goto Lc90ff20; /* mov #-0x14,r0 */
Lc90ff20: r[0]=r[0]&255u;goto Lc90ff22; /* extu.b r0,r0 */
Lc90ff22: r[0]=rd(m,r[14]+r[0],4);goto Lc90ff24; /* mov.l @(r0,r14),r0 */
Lc90ff24: r[2]=54u;goto Lc90ff26; /* mov #0x36,r2 */
Lc90ff26: r[0]=sx(m,r[0]+r[2],2);goto Lc90ff28; /* mov.w @(r0,r2),r0 */
Lc90ff28: r[0]=r[0]&65535u;goto Lc90ff2a; /* extu.w r0,r0 */
Lc90ff2a: *sr=(*sr&~1u)|!(r[0]&0x8u);goto Lc90ff2c; /* tst #0x8,r0 */
Lc90ff2c: if((*sr&1u))goto Lc90ff3c;goto Lc90ff2e; /* bt 0x0c90ff3c */
Lc90ff2e: *pr=rd(m,r[15],4);r[15]+=4;goto Lc90ff30; /* lds.l @r15+,PR */
Lc90ff30: r[4]=r[13];goto Lc90ff32; /* mov r13,r4 */
Lc90ff32: r[5]=r[14];goto Lc90ff34; /* mov r14,r5 */
Lc90ff34: r[12]=rd(m,r[15],4);r[15]+=4;goto Lc90ff36; /* mov.l @r15+,r12 */
Lc90ff36: r[13]=rd(m,r[15],4);r[15]+=4;goto Lc90ff38; /* mov.l @r15+,r13 */
Lc90ff38: r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0xc90fd42u,m,f,sr,r,pr); /* bra 0x0c90fd42 */
Lc90ff3c: *pr=rd(m,r[15],4);r[15]+=4;goto Lc90ff3e; /* lds.l @r15+,PR */
Lc90ff3e: r[12]=rd(m,r[15],4);r[15]+=4;goto Lc90ff40; /* mov.l @r15+,r12 */
Lc90ff40: r[13]=rd(m,r[15],4);r[15]+=4;goto Lc90ff42; /* mov.l @r15+,r13 */
Lc90ff42: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
SA_STAGE_EXPORT int sa_overlay_entity_marker_reset_0c909d40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)sr;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto Lc909d40;
Lc909d40: r[4]=8u;goto Lc909d42; /* mov #0x8,r4 */
Lc909d42: r[4]<<=8;goto Lc909d44; /* shll8 r4 */
Lc909d44: r[4]+=16u;goto Lc909d46; /* add #0x10,r4 */
Lc909d46: r[0]=0u;goto Lc909d48; /* mov #0x0,r0 */
Lc909d48: r[4]+=r[5];goto Lc909d4a; /* add r5,r4 */
Lc909d4a: wr(m,r[4]+0x2u,r[0],2);goto Lc909d4c; /* mov.w r0,@(0x2,r4) */
Lc909d4c: r[0]=rd(m,0x0c909d6cu,4);goto Lc909d4e; /* mov.l 0x0c909d6c,r0 */
Lc909d4e: wr(m,r[4]+0x6u,r[0],2);return 0; /* rts */
}
