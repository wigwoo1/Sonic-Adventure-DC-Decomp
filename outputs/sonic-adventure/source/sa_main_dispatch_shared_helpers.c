/* Static C generated from retail instruction exports by
 * recover_main_dispatch_shared_helpers.py. Descriptive symbols are inferred.
 * No runtime opcode decoding. Guest RAM is 16 MiB; invalid pointers, hardware
 * exceptions and MMIO are not modeled. All callees remain explicit services.
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
SA_STAGE_EXPORT int sa_main_dispatch_status_transition_8c0afc98(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;
 if(!call)return -2;
 int status;uint32_t target;
 goto L8c0afc98;
L8c0afc98: r[15]-=4;wr(m,r[15],r[14],4);goto L8c0afc9a; /* mov.l r14,@-r15 */
L8c0afc9a: r[15]-=4;wr(m,r[15],r[13],4);goto L8c0afc9c; /* mov.l r13,@-r15 */
L8c0afc9c: r[15]-=4;wr(m,r[15],r[12],4);goto L8c0afc9e; /* mov.l r12,@-r15 */
L8c0afc9e: r[15]-=4;wr(m,r[15],r[11],4);goto L8c0afca0; /* mov.l r11,@-r15 */
L8c0afca0: r[15]-=4;wr(m,r[15],r[10],4);goto L8c0afca2; /* mov.l r10,@-r15 */
L8c0afca2: r[15]-=4;wr(m,r[15],r[9],4);goto L8c0afca4; /* mov.l r9,@-r15 */
L8c0afca4: r[15]-=4;wr(m,r[15],*pr,4);goto L8c0afca6; /* sts.l PR,@-r15 */
L8c0afca6: r[15]+=4294967292u;goto L8c0afca8; /* add #-0x4,r15 */
L8c0afca8: r[10]=rd(m,0x8c0afd70u,4);goto L8c0afcaa; /* mov.l 0x8c0afd70,r10 */
L8c0afcaa: r[13]=r[4];goto L8c0afcac; /* mov r4,r13 */
L8c0afcac: r[3]=rd(m,0x8c0afd74u,4);goto L8c0afcae; /* mov.l 0x8c0afd74,r3 */
L8c0afcae: r[12]=0u;goto L8c0afcb0; /* mov #0x0,r12 */
L8c0afcb0: wr(m,r[15],r[5],4);goto L8c0afcb2; /* mov.l r5,@r15 */
L8c0afcb2: r[5]=r[6];goto L8c0afcb4; /* mov r6,r5 */
L8c0afcb4: r[14]=r[6];goto L8c0afcb6; /* mov r6,r14 */
L8c0afcb6: r[11]=4294967293u;goto L8c0afcb8; /* mov #-0x3,r11 */
L8c0afcb8: target=r[3];*pr=0x8c0afcbcu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afcbc; /* jsr @r3 */
L8c0afcbc: *sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c0afcbe; /* cmp/eq #0x2,r0 */
L8c0afcbe: if(!(*sr&1u))goto L8c0afcf8;goto L8c0afcc0; /* bf 0x8c0afcf8 */
L8c0afcc0: r[3]=8u;goto L8c0afcc2; /* mov #0x8,r3 */
L8c0afcc2: r[0]=sx(m,r[13]+0x4u,2);goto L8c0afcc4; /* mov.w @(0x4,r13),r0 */
L8c0afcc4: r[3]<<=8;goto L8c0afcc6; /* shll8 r3 */
L8c0afcc6: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c0afcc8; /* tst r3,r0 */
L8c0afcc8: if((*sr&1u))goto L8c0afcd4;goto L8c0afcca; /* bt 0x8c0afcd4 */
L8c0afcca: r[2]=rd(m,0x8c0afd78u,4);goto L8c0afccc; /* mov.l 0x8c0afd78,r2 */
L8c0afccc: r[6]=r[14];goto L8c0afcce; /* mov r14,r6 */
L8c0afcce: r[5]=rd(m,r[15],4);goto L8c0afcd0; /* mov.l @r15,r5 */
L8c0afcd0: target=r[2];*pr=0x8c0afcd4u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afcd4; /* jsr @r2 */
L8c0afcd4: r[3]=rd(m,0x8c0afd7cu,4);goto L8c0afcd6; /* mov.l 0x8c0afd7c,r3 */
L8c0afcd6: target=r[3];*pr=0x8c0afcdau;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afcda; /* jsr @r3 */
L8c0afcda: r[0]=73u;goto L8c0afcdc; /* mov #0x49,r0 */
L8c0afcdc: r[0]<<=2;goto L8c0afcde; /* shll2 r0 */
L8c0afcde: r[3]=73u;goto L8c0afce0; /* mov #0x49,r3 */
L8c0afce0: r[7]=0u;goto L8c0afce2; /* mov #0x0,r7 */
L8c0afce2: r[2]=40u;goto L8c0afce4; /* mov #0x28,r2 */
L8c0afce4: r[6]=r[7];goto L8c0afce6; /* mov r7,r6 */
L8c0afce6: wr(m,r[13],r[2],1);goto L8c0afce8; /* mov.b r2,@r13 */
L8c0afce8: wr(m,r[0]+r[14],r[3],2);goto L8c0afcea; /* mov.w r3,@(r0,r14) */
L8c0afcea: r[0]=sx(m,r[13]+0x4u,2);goto L8c0afcec; /* mov.w @(0x4,r13),r0 */
L8c0afcec: r[0]&=r[11];goto L8c0afcee; /* and r11,r0 */
L8c0afcee: wr(m,r[13]+0x4u,r[0],2);goto L8c0afcf0; /* mov.w r0,@(0x4,r13) */
L8c0afcf0: r[0]=r[12];goto L8c0afcf2; /* mov r12,r0 */
L8c0afcf2: wr(m,r[14]+0x8u,r[0],2);goto L8c0afcf4; /* mov.w r0,@(0x8,r14) */
L8c0afcf4: r[5]=r[7];goto L8c0afe04; /* bra 0x8c0afe04 */
L8c0afcf8: r[2]=rd(m,0x8c0afd74u,4);goto L8c0afcfa; /* mov.l 0x8c0afd74,r2 */
L8c0afcfa: r[5]=r[14];goto L8c0afcfc; /* mov r14,r5 */
L8c0afcfc: target=r[2];*pr=0x8c0afd00u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afd00; /* jsr @r2 */
L8c0afd00: *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0afd02; /* tst r0,r0 */
L8c0afd02: if(!(*sr&1u))goto L8c0afd08;goto L8c0afd04; /* bf 0x8c0afd08 */
L8c0afd04: goto L8c0afe0c; /* bra 0x8c0afe0c */
L8c0afd08: r[3]=8u;goto L8c0afd0a; /* mov #0x8,r3 */
L8c0afd0a: r[0]=sx(m,r[13]+0x4u,2);goto L8c0afd0c; /* mov.w @(0x4,r13),r0 */
L8c0afd0c: r[3]<<=8;goto L8c0afd0e; /* shll8 r3 */
L8c0afd0e: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c0afd10; /* tst r3,r0 */
L8c0afd10: if((*sr&1u))goto L8c0afdaa;goto L8c0afd12; /* bt 0x8c0afdaa */
L8c0afd12: r[1]=rd(m,0x8c0afd80u,4);goto L8c0afd14; /* mov.l 0x8c0afd80,r1 */
L8c0afd14: target=r[1];*pr=0x8c0afd18u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afd18; /* jsr @r1 */
L8c0afd18: r[3]=2u;goto L8c0afd1a; /* mov #0x2,r3 */
L8c0afd1a: r[4]=r[0];goto L8c0afd1c; /* mov r0,r4 */
L8c0afd1c: r[3]<<=8;goto L8c0afd1e; /* shll8 r3 */
L8c0afd1e: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c0afd20; /* tst r3,r4 */
L8c0afd20: if((*sr&1u))goto L8c0afda0;goto L8c0afd22; /* bt 0x8c0afda0 */
L8c0afd22: r[0]=4294967220u;goto L8c0afd24; /* mov #-0x4c,r0 */
L8c0afd24: r[0]=r[0]&255u;goto L8c0afd26; /* extu.b r0,r0 */
L8c0afd26: r[2]=55u;goto L8c0afd28; /* mov #0x37,r2 */
L8c0afd28: r[7]=0u;goto L8c0afd2a; /* mov #0x0,r7 */
L8c0afd2a: r[1]=29u;goto L8c0afd2c; /* mov #0x1d,r1 */
L8c0afd2c: r[6]=r[7];goto L8c0afd2e; /* mov r7,r6 */
L8c0afd2e: wr(m,r[13],r[1],1);goto L8c0afd30; /* mov.b r1,@r13 */
L8c0afd30: fload(m,f,3u,r[0]+r[14]);goto L8c0afd32; /* fmov.s @(r0,r14),fr3 */
L8c0afd32: r[0]=60u;goto L8c0afd34; /* mov #0x3c,r0 */
L8c0afd34: fstore(m,f,3u,r[0]+r[14]);goto L8c0afd36; /* fmov fr3,@(r0,r14) */
L8c0afd36: r[0]=73u;goto L8c0afd38; /* mov #0x49,r0 */
L8c0afd38: r[0]<<=2;goto L8c0afd3a; /* shll2 r0 */
L8c0afd3a: wr(m,r[0]+r[14],r[2],2);goto L8c0afd3c; /* mov.w r2,@(r0,r14) */
L8c0afd3c: r[0]=sx(m,r[13]+0x4u,2);goto L8c0afd3e; /* mov.w @(0x4,r13),r0 */
L8c0afd3e: r[0]&=r[11];goto L8c0afd40; /* and r11,r0 */
L8c0afd40: wr(m,r[13]+0x4u,r[0],2);goto L8c0afd42; /* mov.w r0,@(0x4,r13) */
L8c0afd42: r[0]=r[12];goto L8c0afd44; /* mov r12,r0 */
L8c0afd44: wr(m,r[14]+0x8u,r[0],2);goto L8c0afd46; /* mov.w r0,@(0x8,r14) */
L8c0afd46: r[5]=r[7];goto L8c0afe04; /* bra 0x8c0afe04 */
L8c0afda0: r[2]=rd(m,0x8c0afee0u,4);goto L8c0afda2; /* mov.l 0x8c0afee0,r2 */
L8c0afda2: r[6]=r[14];goto L8c0afda4; /* mov r14,r6 */
L8c0afda4: r[5]=rd(m,r[15],4);goto L8c0afda6; /* mov.l @r15,r5 */
L8c0afda6: target=r[2];*pr=0x8c0afdaau;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afdaa; /* jsr @r2 */
L8c0afdaa: r[0]=4294967220u;goto L8c0afdac; /* mov #-0x4c,r0 */
L8c0afdac: r[0]=r[0]&255u;goto L8c0afdae; /* extu.b r0,r0 */
L8c0afdae: r[3]=8u;goto L8c0afdb0; /* mov #0x8,r3 */
L8c0afdb0: wr(m,r[13],r[3],1);goto L8c0afdb2; /* mov.b r3,@r13 */
L8c0afdb2: fload(m,f,3u,r[0]+r[14]);goto L8c0afdb4; /* fmov.s @(r0,r14),fr3 */
L8c0afdb4: r[0]=60u;goto L8c0afdb6; /* mov #0x3c,r0 */
L8c0afdb6: fstore(m,f,3u,r[0]+r[14]);goto L8c0afdb8; /* fmov fr3,@(r0,r14) */
L8c0afdb8: r[0]=108u;goto L8c0afdba; /* mov #0x6c,r0 */
L8c0afdba: r[3]=rd(m,r[14]+r[0],4);goto L8c0afdbc; /* mov.l @(r0,r14),r3 */
L8c0afdbc: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0afdbe; /* tst r3,r3 */
L8c0afdbe: if((*sr&1u))goto L8c0afddc;goto L8c0afdc0; /* bt 0x8c0afddc */
L8c0afdc0: r[9]=rd(m,r[14]+r[0],4);goto L8c0afdc2; /* mov.l @(r0,r14),r9 */
L8c0afdc2: r[9]=rd(m,r[9]+0x28u,4);goto L8c0afdc4; /* mov.l @(0x28,r9),r9 */
L8c0afdc4: *sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c0afdc6; /* tst r9,r9 */
L8c0afdc6: if((*sr&1u))goto L8c0afddc;goto L8c0afdc8; /* bt 0x8c0afddc */
L8c0afdc8: r[0]=24u;goto L8c0afdca; /* mov #0x18,r0 */
L8c0afdca: r[3]=rd(m,0x8c0afee4u,4);goto L8c0afdcc; /* mov.l 0x8c0afee4,r3 */
L8c0afdcc: fload(m,f,6u,r[0]+r[9]);goto L8c0afdce; /* fmov.s @(r0,r9),fr6 */
L8c0afdce: r[0]=20u;goto L8c0afdd0; /* mov #0x14,r0 */
L8c0afdd0: fload(m,f,5u,r[0]+r[9]);goto L8c0afdd2; /* fmov.s @(r0,r9),fr5 */
L8c0afdd2: r[0]=16u;goto L8c0afdd4; /* mov #0x10,r0 */
L8c0afdd4: fload(m,f,4u,r[0]+r[9]);goto L8c0afdd6; /* fmov.s @(r0,r9),fr4 */
L8c0afdd6: r[0]=sx(m,r[13]+0x8u,1);goto L8c0afdd8; /* mov.b @(0x8,r13),r0 */
L8c0afdd8: target=r[3];*pr=0x8c0afddcu;r[4]=r[0];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afddc; /* jsr @r3 */
L8c0afddc: r[0]=73u;goto L8c0afdde; /* mov #0x49,r0 */
L8c0afdde: r[0]<<=2;goto L8c0afde0; /* shll2 r0 */
L8c0afde0: r[2]=14u;goto L8c0afde2; /* mov #0xe,r2 */
L8c0afde2: wr(m,r[0]+r[14],r[2],2);goto L8c0afde4; /* mov.w r2,@(r0,r14) */
L8c0afde4: r[0]=0x8c0afee8u;goto L8c0afde6; /* mova 0x8c0afee8,r0 */
L8c0afde6: fload(m,f,3u,r[0]);goto L8c0afde8; /* fmov.s @r0,fr3 */
L8c0afde8: r[3]=5u;goto L8c0afdea; /* mov #0x5,r3 */
L8c0afdea: r[3]<<=8;goto L8c0afdec; /* shll8 r3 */
L8c0afdec: r[7]=0u;goto L8c0afdee; /* mov #0x0,r7 */
L8c0afdee: fstore(m,f,3u,r[14]);goto L8c0afdf0; /* fmov.s fr3,@r14 */
L8c0afdf0: r[0]=sx(m,r[13]+0x4u,2);goto L8c0afdf2; /* mov.w @(0x4,r13),r0 */
L8c0afdf2: r[5]=0u;goto L8c0afdf4; /* mov #0x0,r5 */
L8c0afdf4: r[6]=r[7];goto L8c0afdf6; /* mov r7,r6 */
L8c0afdf6: r[0]&=r[11];goto L8c0afdf8; /* and r11,r0 */
L8c0afdf8: wr(m,r[13]+0x4u,r[0],2);goto L8c0afdfa; /* mov.w r0,@(0x4,r13) */
L8c0afdfa: r[0]=sx(m,r[13]+0x4u,2);goto L8c0afdfc; /* mov.w @(0x4,r13),r0 */
L8c0afdfc: r[0]|=r[3];goto L8c0afdfe; /* or r3,r0 */
L8c0afdfe: wr(m,r[13]+0x4u,r[0],2);goto L8c0afe00; /* mov.w r0,@(0x4,r13) */
L8c0afe00: r[0]=r[12];goto L8c0afe02; /* mov r12,r0 */
L8c0afe02: wr(m,r[14]+0x8u,r[0],2);goto L8c0afe04; /* mov.w r0,@(0x8,r14) */
L8c0afe04: target=r[10];*pr=0x8c0afe08u;r[4]=17u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afe08; /* jsr @r10 */
L8c0afe08: r[0]=1u;goto L8c0afe0e; /* bra 0x8c0afe0e */
L8c0afe0c: r[0]=0u;goto L8c0afe0e; /* mov #0x0,r0 */
L8c0afe0e: r[15]+=4u;goto L8c0afe10; /* add #0x4,r15 */
L8c0afe10: *pr=rd(m,r[15],4);r[15]+=4;goto L8c0afe12; /* lds.l @r15+,PR */
L8c0afe12: r[9]=rd(m,r[15],4);r[15]+=4;goto L8c0afe14; /* mov.l @r15+,r9 */
L8c0afe14: r[10]=rd(m,r[15],4);r[15]+=4;goto L8c0afe16; /* mov.l @r15+,r10 */
L8c0afe16: r[11]=rd(m,r[15],4);r[15]+=4;goto L8c0afe18; /* mov.l @r15+,r11 */
L8c0afe18: r[12]=rd(m,r[15],4);r[15]+=4;goto L8c0afe1a; /* mov.l @r15+,r12 */
L8c0afe1a: r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0afe1c; /* mov.l @r15+,r13 */
L8c0afe1c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_main_dispatch_flag_transition_8c0b11c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;
 if(!call)return -2;
 int status;uint32_t target;
 int take;
 goto L8c0b11c0;
L8c0b11c0: r[15]-=4;wr(m,r[15],r[14],4);goto L8c0b11c2; /* mov.l r14,@-r15 */
L8c0b11c2: r[15]-=4;wr(m,r[15],r[13],4);goto L8c0b11c4; /* mov.l r13,@-r15 */
L8c0b11c4: r[15]-=4;wr(m,r[15],*pr,4);goto L8c0b11c6; /* sts.l PR,@-r15 */
L8c0b11c6: r[15]+=4294967292u;goto L8c0b11c8; /* add #-0x4,r15 */
L8c0b11c8: r[14]=r[4];goto L8c0b11ca; /* mov r4,r14 */
L8c0b11ca: wr(m,r[15],r[5],4);goto L8c0b11cc; /* mov.l r5,@r15 */
L8c0b11cc: r[0]=sx(m,r[14]+0x4u,2);goto L8c0b11ce; /* mov.w @(0x4,r14),r0 */
L8c0b11ce: *sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c0b11d0; /* tst #0x3,r0 */
L8c0b11d0: take=!(*sr&1u);r[13]=r[6];if(take)goto L8c0b1222;goto L8c0b11d4; /* bf/s 0x8c0b1222 */
L8c0b11d4: r[3]=1u;goto L8c0b11d6; /* mov #0x1,r3 */
L8c0b11d6: r[0]=sx(m,r[14]+0x4u,2);goto L8c0b11d8; /* mov.w @(0x4,r14),r0 */
L8c0b11d8: r[3]<<=8;goto L8c0b11da; /* shll8 r3 */
L8c0b11da: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c0b11dc; /* tst r3,r0 */
L8c0b11dc: if((*sr&1u))goto L8c0b11e6;goto L8c0b11de; /* bt 0x8c0b11e6 */
L8c0b11de: r[1]=8u;goto L8c0b11e0; /* mov #0x8,r1 */
L8c0b11e0: wr(m,r[14],r[1],1);goto L8c0b11e2; /* mov.b r1,@r14 */
L8c0b11e2: goto L8c0b121e; /* bra 0x8c0b121e */
L8c0b11e6: r[2]=8u;goto L8c0b11e8; /* mov #0x8,r2 */
L8c0b11e8: r[2]<<=8;goto L8c0b11ea; /* shll8 r2 */
L8c0b11ea: *sr=(*sr&~1u)|!(r[2]&r[0]);goto L8c0b11ec; /* tst r2,r0 */
L8c0b11ec: if((*sr&1u))goto L8c0b1212;goto L8c0b11ee; /* bt 0x8c0b1212 */
L8c0b11ee: r[1]=rd(m,0x8c0b12c0u,4);goto L8c0b11f0; /* mov.l 0x8c0b12c0,r1 */
L8c0b11f0: target=r[1];*pr=0x8c0b11f4u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b11f4; /* jsr @r1 */
L8c0b11f4: r[3]=2u;goto L8c0b11f6; /* mov #0x2,r3 */
L8c0b11f6: r[3]<<=8;goto L8c0b11f8; /* shll8 r3 */
L8c0b11f8: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c0b11fa; /* tst r3,r0 */
L8c0b11fa: if((*sr&1u))goto L8c0b120a;goto L8c0b11fc; /* bt 0x8c0b120a */
L8c0b11fc: r[0]=73u;goto L8c0b11fe; /* mov #0x49,r0 */
L8c0b11fe: r[1]=28u;goto L8c0b1200; /* mov #0x1c,r1 */
L8c0b1200: r[0]<<=2;goto L8c0b1202; /* shll2 r0 */
L8c0b1202: wr(m,r[14],r[1],1);goto L8c0b1204; /* mov.b r1,@r14 */
L8c0b1204: r[2]=57u;goto L8c0b1206; /* mov #0x39,r2 */
L8c0b1206: wr(m,r[0]+r[13],r[2],2);goto L8c0b121e; /* bra 0x8c0b121e */
L8c0b120a: r[5]=rd(m,r[15],4);goto L8c0b120c; /* mov.l @r15,r5 */
L8c0b120c: r[6]=r[13];goto L8c0b120e; /* mov r13,r6 */
L8c0b120e: target=0x8c0b1d60u;*pr=0x8c0b1212u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b1212; /* bsr 0x8c0b1d60 */
L8c0b1212: r[0]=73u;goto L8c0b1214; /* mov #0x49,r0 */
L8c0b1214: r[0]<<=2;goto L8c0b1216; /* shll2 r0 */
L8c0b1216: r[3]=18u;goto L8c0b1218; /* mov #0x12,r3 */
L8c0b1218: r[2]=12u;goto L8c0b121a; /* mov #0xc,r2 */
L8c0b121a: wr(m,r[14],r[2],1);goto L8c0b121c; /* mov.b r2,@r14 */
L8c0b121c: wr(m,r[0]+r[13],r[3],2);goto L8c0b121e; /* mov.w r3,@(r0,r13) */
L8c0b121e: r[0]=1u;goto L8c0b1224; /* bra 0x8c0b1224 */
L8c0b1222: r[0]=0u;goto L8c0b1224; /* mov #0x0,r0 */
L8c0b1224: r[15]+=4u;goto L8c0b1226; /* add #0x4,r15 */
L8c0b1226: *pr=rd(m,r[15],4);r[15]+=4;goto L8c0b1228; /* lds.l @r15+,PR */
L8c0b1228: r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0b122a; /* mov.l @r15+,r13 */
L8c0b122a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
