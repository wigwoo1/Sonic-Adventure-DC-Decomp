/* Cancellation and saved-context restore; descriptive names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * Integer instructions, no FPU-mode restriction. Valid RAM/table preconditions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
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
SA_STAGE_EXPORT int sa_main_dispatch_cancel_8c01ae60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c01ae60;
L8c01ae60: r[15]-=4;wr(m,r[15],r[14],4);goto L8c01ae62; /* mov.l r14,@-r15 */
L8c01ae62: r[14]=rd(m,0x8c01b000u,4);goto L8c01ae64; /* mov.l 0x8c01b000,r14 */
L8c01ae64: r[15]-=4;wr(m,r[15],r[13],4);goto L8c01ae66; /* mov.l r13,@-r15 */
L8c01ae66: r[15]-=4;wr(m,r[15],*pr,4);goto L8c01ae68; /* sts.l PR,@-r15 */
L8c01ae68: r[4]=rd(m,r[14],4);goto L8c01ae6a; /* mov.l @r14,r4 */
L8c01ae6a: r[0]=sx(m,r[4]+0x8u,1);goto L8c01ae6c; /* mov.b @(0x8,r4),r0 */
L8c01ae6c: *sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c01ae6e; /* cmp/eq #0x2,r0 */
L8c01ae6e: take=!(*sr&1u);r[13]=0u;if(take)goto L8c01aefa;goto L8c01ae72; /* bf/s 0x8c01aefa */
L8c01ae72: r[0]=52u;goto L8c01ae74; /* mov #0x34,r0 */
L8c01ae74: r[0]=sx(m,r[0]+r[4],2);goto L8c01ae76; /* mov.w @(r0,r4),r0 */
L8c01ae76: r[1]=62u;goto L8c01ae78; /* mov #0x3e,r1 */
L8c01ae78: r[2]=118u;goto L8c01ae7a; /* mov #0x76,r2 */
L8c01ae7a: wr(m,r[4]+0x4u,r[0],2);goto L8c01ae7c; /* mov.w r0,@(0x4,r4) */
L8c01ae7c: r[3]=rd(m,r[14],4);goto L8c01ae7e; /* mov.l @r14,r3 */
L8c01ae7e: r[0]=r[3];goto L8c01ae80; /* mov r3,r0 */
L8c01ae80: r[0]=sx(m,r[1]+r[0],1);goto L8c01ae82; /* mov.b @(r0,r1),r0 */
L8c01ae82: r[1]=76u;goto L8c01ae84; /* mov #0x4c,r1 */
L8c01ae84: wr(m,r[3]+0xbu,r[0],1);goto L8c01ae86; /* mov.b r0,@(0xb,r3) */
L8c01ae86: r[3]=rd(m,r[14],4);goto L8c01ae88; /* mov.l @r14,r3 */
L8c01ae88: r[0]=r[3];goto L8c01ae8a; /* mov r3,r0 */
L8c01ae8a: r[0]=sx(m,r[2]+r[0],1);goto L8c01ae8c; /* mov.b @(r0,r2),r0 */
L8c01ae8c: r[2]=124u;goto L8c01ae8e; /* mov #0x7c,r2 */
L8c01ae8e: wr(m,r[3]+0x6u,r[0],1);goto L8c01ae90; /* mov.b r0,@(0x6,r3) */
L8c01ae90: r[3]=rd(m,r[14],4);goto L8c01ae92; /* mov.l @r14,r3 */
L8c01ae92: r[0]=r[3];goto L8c01ae94; /* mov r3,r0 */
L8c01ae94: r[0]=rd(m,r[1]+r[0],4);goto L8c01ae96; /* mov.l @(r0,r1),r0 */
L8c01ae96: r[1]=100u;goto L8c01ae98; /* mov #0x64,r1 */
L8c01ae98: wr(m,r[3]+0xcu,r[0],4);goto L8c01ae9a; /* mov.l r0,@(0xc,r3) */
L8c01ae9a: r[3]=rd(m,r[14],4);goto L8c01ae9c; /* mov.l @r14,r3 */
L8c01ae9c: r[0]=r[3];goto L8c01ae9e; /* mov r3,r0 */
L8c01ae9e: r[0]=sx(m,r[2]+r[0],1);goto L8c01aea0; /* mov.b @(r0,r2),r0 */
L8c01aea0: wr(m,r[3]+0x9u,r[0],1);goto L8c01aea2; /* mov.b r0,@(0x9,r3) */
L8c01aea2: r[3]=rd(m,r[14],4);goto L8c01aea4; /* mov.l @r14,r3 */
L8c01aea4: r[0]=r[3];goto L8c01aea6; /* mov r3,r0 */
L8c01aea6: r[0]=rd(m,r[1]+r[0],4);goto L8c01aea8; /* mov.l @(r0,r1),r0 */
L8c01aea8: wr(m,r[3]+0x10u,r[0],4);goto L8c01aeaa; /* mov.l r0,@(0x10,r3) */
L8c01aeaa: r[3]=rd(m,r[14],4);goto L8c01aeac; /* mov.l @r14,r3 */
L8c01aeac: r[0]=r[3];goto L8c01aeae; /* mov r3,r0 */
L8c01aeae: r[0]=rd(m,r[0]+0x20u,4);goto L8c01aeb0; /* mov.l @(0x20,r0),r0 */
L8c01aeb0: wr(m,r[3]+0x8u,r[0],1);goto L8c01aeb2; /* mov.b r0,@(0x8,r3) */
L8c01aeb2: r[3]=rd(m,r[14],4);goto L8c01aeb4; /* mov.l @r14,r3 */
L8c01aeb4: r[5]=6u;goto L8c01aeb6; /* mov #0x6,r5 */
L8c01aeb6: r[1]=rd(m,0x8c01b004u,4);goto L8c01aeb8; /* mov.l 0x8c01b004,r1 */
L8c01aeb8: r[0]=r[3];goto L8c01aeba; /* mov r3,r0 */
L8c01aeba: r[0]=sx(m,r[0]+0x6u,1);goto L8c01aebc; /* mov.b @(0x6,r0),r0 */
L8c01aebc: r[2]=rd(m,r[1],4);goto L8c01aebe; /* mov.l @r1,r2 */
L8c01aebe: r[0]<<=2;goto L8c01aec0; /* shll2 r0 */
L8c01aec0: r[6]=rd(m,0x8c01b00cu,4);goto L8c01aec2; /* mov.l 0x8c01b00c,r6 */
L8c01aec2: r[0]<<=2;goto L8c01aec4; /* shll2 r0 */
L8c01aec4: r[0]+=r[2];goto L8c01aec6; /* add r2,r0 */
L8c01aec6: r[0]=sx(m,r[0]+0x5u,1);goto L8c01aec8; /* mov.b @(0x5,r0),r0 */
L8c01aec8: r[2]=1u;goto L8c01aeca; /* mov #0x1,r2 */
L8c01aeca: wr(m,r[3]+0x7u,r[0],1);goto L8c01aecc; /* mov.b r0,@(0x7,r3) */
L8c01aecc: r[3]=rd(m,r[14],4);goto L8c01aece; /* mov.l @r14,r3 */
L8c01aece: wr(m,r[3],r[2],4);goto L8c01aed0; /* mov.l r2,@r3 */
L8c01aed0: r[3]=rd(m,r[14],4);goto L8c01aed2; /* mov.l @r14,r3 */
L8c01aed2: wr(m,r[3]+0x14u,r[13],4);goto L8c01aed4; /* mov.l r13,@(0x14,r3) */
L8c01aed4: r[3]=rd(m,0x8c01b008u,4);goto L8c01aed6; /* mov.l 0x8c01b008,r3 */
L8c01aed6: r[2]=rd(m,r[3],4);goto L8c01aed8; /* mov.l @r3,r2 */
L8c01aed8: wr(m,r[2]+0x14u,r[13],4);goto L8c01aeda; /* mov.l r13,@(0x14,r2) */
L8c01aeda: r[2]=rd(m,0x8c01b010u,4);goto L8c01aedc; /* mov.l 0x8c01b010,r2 */
L8c01aedc: target=r[2];*pr=0x8c01aee0u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01aee0; /* jsr @r2 */
L8c01aee0: r[3]=rd(m,r[14],4);goto L8c01aee2; /* mov.l @r14,r3 */
L8c01aee2: r[0]=sx(m,r[3]+0x8u,1);goto L8c01aee4; /* mov.b @(0x8,r3),r0 */
L8c01aee4: *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c01aee6; /* tst r0,r0 */
L8c01aee6: if(!(*sr&1u))goto L8c01aefa;goto L8c01aee8; /* bf 0x8c01aefa */
L8c01aee8: r[2]=rd(m,0x8c01b014u,4);goto L8c01aeea; /* mov.l 0x8c01b014,r2 */
L8c01aeea: r[0]=rd(m,r[2],4);goto L8c01aeec; /* mov.l @r2,r0 */
L8c01aeec: *sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c01aeee; /* cmp/eq #-0x1,r0 */
L8c01aeee: if((*sr&1u))goto L8c01aefa;goto L8c01aef0; /* bt 0x8c01aefa */
L8c01aef0: r[1]=rd(m,0x8c01b018u,4);goto L8c01aef2; /* mov.l 0x8c01b018,r1 */
L8c01aef2: r[3]=rd(m,0x8c01b014u,4);goto L8c01aef4; /* mov.l 0x8c01b014,r3 */
L8c01aef4: r[5]=rd(m,r[1],4);goto L8c01aef6; /* mov.l @r1,r5 */
L8c01aef6: target=0x8c01aae0u;*pr=0x8c01aefau;r[4]=rd(m,r[3],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01aefa; /* bsr 0x8c01aae0 */
L8c01aefa: r[3]=rd(m,0x8c01b01cu,4);goto L8c01aefc; /* mov.l 0x8c01b01c,r3 */
L8c01aefc: wr(m,r[3],r[13],4);goto L8c01aefe; /* mov.l r13,@r3 */
L8c01aefe: *pr=rd(m,r[15],4);r[15]+=4;goto L8c01af00; /* lds.l @r15+,PR */
L8c01af00: r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01af02; /* mov.l @r15+,r13 */
L8c01af02: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_main_dispatch_context_restore_8c01aae0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01aae0;
L8c01aae0: r[6]=rd(m,0x8c01ac70u,4);goto L8c01aae2; /* mov.l 0x8c01ac70,r6 */
L8c01aae2: r[7]=rd(m,r[6],4);goto L8c01aae4; /* mov.l @r6,r7 */
L8c01aae4: r[0]=sx(m,r[7]+0x8u,1);goto L8c01aae6; /* mov.b @(0x8,r7),r0 */
L8c01aae6: *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c01aae8; /* tst r0,r0 */
L8c01aae8: if(!(*sr&1u))goto L8c01abd8;goto L8c01aaea; /* bf 0x8c01abd8 */
L8c01aaea: r[0]=sx(m,r[7]+0x8u,1);goto L8c01aaec; /* mov.b @(0x8,r7),r0 */
L8c01aaec: wr(m,r[7]+0x18u,r[0],4);goto L8c01aaee; /* mov.l r0,@(0x18,r7) */
L8c01aaee: r[3]=rd(m,r[6],4);goto L8c01aaf0; /* mov.l @r6,r3 */
L8c01aaf0: r[2]=rd(m,r[6],4);goto L8c01aaf2; /* mov.l @r6,r2 */
L8c01aaf2: r[0]=sx(m,r[3]+0x4u,2);goto L8c01aaf4; /* mov.w @(0x4,r3),r0 */
L8c01aaf4: r[2]+=48u;goto L8c01aaf6; /* add #0x30,r2 */
L8c01aaf6: wr(m,r[2],r[0],2);goto L8c01aaf8; /* mov.w r0,@r2 */
L8c01aaf8: r[3]=rd(m,r[6],4);goto L8c01aafa; /* mov.l @r6,r3 */
L8c01aafa: r[2]=rd(m,r[6],4);goto L8c01aafc; /* mov.l @r6,r2 */
L8c01aafc: r[0]=sx(m,r[3]+0xbu,1);goto L8c01aafe; /* mov.b @(0xb,r3),r0 */
L8c01aafe: r[2]+=60u;goto L8c01ab00; /* add #0x3c,r2 */
L8c01ab00: wr(m,r[2],r[0],1);goto L8c01ab02; /* mov.b r0,@r2 */
L8c01ab02: r[3]=rd(m,r[6],4);goto L8c01ab04; /* mov.l @r6,r3 */
L8c01ab04: r[2]=rd(m,r[6],4);goto L8c01ab06; /* mov.l @r6,r2 */
L8c01ab06: r[0]=sx(m,r[3]+0x6u,1);goto L8c01ab08; /* mov.b @(0x6,r3),r0 */
L8c01ab08: r[2]+=116u;goto L8c01ab0a; /* add #0x74,r2 */
L8c01ab0a: wr(m,r[2],r[0],1);goto L8c01ab0c; /* mov.b r0,@r2 */
L8c01ab0c: r[3]=rd(m,r[6],4);goto L8c01ab0e; /* mov.l @r6,r3 */
L8c01ab0e: r[2]=rd(m,r[6],4);goto L8c01ab10; /* mov.l @r6,r2 */
L8c01ab10: r[1]=rd(m,r[3]+0xcu,4);goto L8c01ab12; /* mov.l @(0xc,r3),r1 */
L8c01ab12: r[2]+=68u;goto L8c01ab14; /* add #0x44,r2 */
L8c01ab14: wr(m,r[2],r[1],4);goto L8c01ab16; /* mov.l r1,@r2 */
L8c01ab16: r[7]=rd(m,r[6],4);goto L8c01ab18; /* mov.l @r6,r7 */
L8c01ab18: r[0]=r[7];goto L8c01ab1a; /* mov r7,r0 */
L8c01ab1a: r[0]=sx(m,r[0]+0x4u,2);goto L8c01ab1c; /* mov.w @(0x4,r0),r0 */
L8c01ab1c: *sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c01ab1e; /* cmp/eq #-0x1,r0 */
L8c01ab1e: if((*sr&1u))goto L8c01ab38;goto L8c01ab20; /* bt 0x8c01ab38 */
L8c01ab20: r[0]=sx(m,r[7]+0x4u,2);goto L8c01ab22; /* mov.w @(0x4,r7),r0 */
L8c01ab22: r[2]=r[7];goto L8c01ab24; /* mov r7,r2 */
L8c01ab24: r[1]=rd(m,0x8c01ac74u,4);goto L8c01ab26; /* mov.l 0x8c01ac74,r1 */
L8c01ab26: r[2]+=122u;goto L8c01ab28; /* add #0x7a,r2 */
L8c01ab28: r[0]<<=2;goto L8c01ab2a; /* shll2 r0 */
L8c01ab2a: r[3]=rd(m,r[1],4);goto L8c01ab2c; /* mov.l @r1,r3 */
L8c01ab2c: r[0]<<=2;goto L8c01ab2e; /* shll2 r0 */
L8c01ab2e: r[0]<<=2;goto L8c01ab30; /* shll2 r0 */
L8c01ab30: r[0]+=r[3];goto L8c01ab32; /* add r3,r0 */
L8c01ab32: r[0]=sx(m,r[0]+0x2u,1);goto L8c01ab34; /* mov.b @(0x2,r0),r0 */
L8c01ab34: wr(m,r[2],r[0],1);goto L8c01ab40; /* bra 0x8c01ab40 */
L8c01ab38: r[2]=r[7];goto L8c01ab3a; /* mov r7,r2 */
L8c01ab3a: r[3]=21u;goto L8c01ab3c; /* mov #0x15,r3 */
L8c01ab3c: r[2]+=122u;goto L8c01ab3e; /* add #0x7a,r2 */
L8c01ab3e: wr(m,r[2],r[3],1);goto L8c01ab40; /* mov.b r3,@r2 */
L8c01ab40: r[1]=rd(m,r[6],4);goto L8c01ab42; /* mov.l @r6,r1 */
L8c01ab42: r[7]=rd(m,0x8c01ac78u,4);goto L8c01ab44; /* mov.l 0x8c01ac78,r7 */
L8c01ab44: r[1]+=122u;goto L8c01ab46; /* add #0x7a,r1 */
L8c01ab46: r[3]=rd(m,r[6],4);goto L8c01ab48; /* mov.l @r6,r3 */
L8c01ab48: r[2]=sx(m,r[1],1);goto L8c01ab4a; /* mov.b @r1,r2 */
L8c01ab4a: r[1]=rd(m,r[7],4);goto L8c01ab4c; /* mov.l @r7,r1 */
L8c01ab4c: r[3]+=92u;goto L8c01ab4e; /* add #0x5c,r3 */
L8c01ab4e: r[2]<<=2;goto L8c01ab50; /* shll2 r2 */
L8c01ab50: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c01ab52; /* shll r2 */
L8c01ab52: r[2]+=r[1];goto L8c01ab54; /* add r1,r2 */
L8c01ab54: r[1]=rd(m,0x8c01ac60u,4);goto L8c01ab56; /* mov.l 0x8c01ac60,r1 */
L8c01ab56: r[0]=rd(m,r[2]+0x4u,4);goto L8c01ab58; /* mov.l @(0x4,r2),r0 */
L8c01ab58: wr(m,r[3],r[0],4);goto L8c01ab5a; /* mov.l r0,@r3 */
L8c01ab5a: r[0]=4294967295u;goto L8c01ab5c; /* mov #-0x1,r0 */
L8c01ab5c: r[3]=rd(m,r[6],4);goto L8c01ab5e; /* mov.l @r6,r3 */
L8c01ab5e: wr(m,r[3]+0x4u,r[0],2);goto L8c01ab60; /* mov.w r0,@(0x4,r3) */
L8c01ab60: r[0]=0u;goto L8c01ab62; /* mov #0x0,r0 */
L8c01ab62: r[3]=rd(m,r[6],4);goto L8c01ab64; /* mov.l @r6,r3 */
L8c01ab64: wr(m,r[3]+0x8u,r[0],1);goto L8c01ab66; /* mov.b r0,@(0x8,r3) */
L8c01ab66: r[0]=r[4];goto L8c01ab68; /* mov r4,r0 */
L8c01ab68: r[4]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c01ab6a; /* exts.w r4,r4 */
L8c01ab6a: r[3]=rd(m,r[6],4);goto L8c01ab6c; /* mov.l @r6,r3 */
L8c01ab6c: r[4]<<=2;goto L8c01ab6e; /* shll2 r4 */
L8c01ab6e: r[4]<<=2;goto L8c01ab70; /* shll2 r4 */
L8c01ab70: wr(m,r[3]+0x6u,r[0],1);goto L8c01ab72; /* mov.b r0,@(0x6,r3) */
L8c01ab72: r[2]=rd(m,r[1],4);goto L8c01ab74; /* mov.l @r1,r2 */
L8c01ab74: r[3]=rd(m,r[6],4);goto L8c01ab76; /* mov.l @r6,r3 */
L8c01ab76: r[4]+=r[2];goto L8c01ab78; /* add r2,r4 */
L8c01ab78: r[0]=rd(m,r[4],4);goto L8c01ab7a; /* mov.l @r4,r0 */
L8c01ab7a: wr(m,r[3]+0xcu,r[0],4);goto L8c01ab7c; /* mov.l r0,@(0xc,r3) */
L8c01ab7c: r[0]=r[5];goto L8c01ab7e; /* mov r5,r0 */
L8c01ab7e: r[3]=rd(m,r[6],4);goto L8c01ab80; /* mov.l @r6,r3 */
L8c01ab80: r[5]=r[5]&255u;goto L8c01ab82; /* extu.b r5,r5 */
L8c01ab82: wr(m,r[3]+0x9u,r[0],1);goto L8c01ab84; /* mov.b r0,@(0x9,r3) */
L8c01ab84: r[5]<<=2;goto L8c01ab86; /* shll2 r5 */
L8c01ab86: r[2]=rd(m,r[7],4);goto L8c01ab88; /* mov.l @r7,r2 */
L8c01ab88: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c01ab8a; /* shll r5 */
L8c01ab8a: r[3]=rd(m,r[6],4);goto L8c01ab8c; /* mov.l @r6,r3 */
L8c01ab8c: r[5]+=r[2];goto L8c01ab8e; /* add r2,r5 */
L8c01ab8e: r[2]=rd(m,0x8c01ac64u,4);goto L8c01ab90; /* mov.l 0x8c01ac64,r2 */
L8c01ab90: r[0]=rd(m,r[5]+0x4u,4);goto L8c01ab92; /* mov.l @(0x4,r5),r0 */
L8c01ab92: wr(m,r[3]+0x10u,r[0],4);goto L8c01ab94; /* mov.l r0,@(0x10,r3) */
L8c01ab94: r[0]=0u;goto L8c01ab96; /* mov #0x0,r0 */
L8c01ab96: r[3]=rd(m,r[2],4);goto L8c01ab98; /* mov.l @r2,r3 */
L8c01ab98: wr(m,r[3]+0x14u,r[0],4);goto L8c01ab9a; /* mov.l r0,@(0x14,r3) */
L8c01ab9a: r[3]=rd(m,r[6],4);goto L8c01ab9c; /* mov.l @r6,r3 */
L8c01ab9c: r[0]=sx(m,r[3]+0x9u,1);goto L8c01ab9e; /* mov.b @(0x9,r3),r0 */
L8c01ab9e: r[3]=rd(m,r[7],4);goto L8c01aba0; /* mov.l @r7,r3 */
L8c01aba0: r[0]<<=2;goto L8c01aba2; /* shll2 r0 */
L8c01aba2: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c01aba4; /* shll r0 */
L8c01aba4: r[0]+=r[3];goto L8c01aba6; /* add r3,r0 */
L8c01aba6: r[3]=rd(m,r[0],4);goto L8c01aba8; /* mov.l @r0,r3 */
L8c01aba8: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c01abaa; /* tst r3,r3 */
L8c01abaa: if((*sr&1u))goto L8c01abb4;goto L8c01abac; /* bt 0x8c01abb4 */
L8c01abac: r[3]=rd(m,r[6],4);goto L8c01abae; /* mov.l @r6,r3 */
L8c01abae: r[0]=sx(m,r[3]+0xbu,1);goto L8c01abb0; /* mov.b @(0xb,r3),r0 */
L8c01abb0: r[0]|=0x1u;goto L8c01abb2; /* or #0x1,r0 */
L8c01abb2: wr(m,r[3]+0xbu,r[0],1);goto L8c01abb4; /* mov.b r0,@(0xb,r3) */
L8c01abb4: r[3]=rd(m,r[6],4);goto L8c01abb6; /* mov.l @r6,r3 */
L8c01abb6: r[5]=6u;goto L8c01abb8; /* mov #0x6,r5 */
L8c01abb8: r[1]=rd(m,0x8c01ac60u,4);goto L8c01abba; /* mov.l 0x8c01ac60,r1 */
L8c01abba: r[0]=r[3];goto L8c01abbc; /* mov r3,r0 */
L8c01abbc: r[0]=sx(m,r[0]+0x6u,1);goto L8c01abbe; /* mov.b @(0x6,r0),r0 */
L8c01abbe: r[2]=rd(m,r[1],4);goto L8c01abc0; /* mov.l @r1,r2 */
L8c01abc0: r[0]<<=2;goto L8c01abc2; /* shll2 r0 */
L8c01abc2: r[0]<<=2;goto L8c01abc4; /* shll2 r0 */
L8c01abc4: r[0]+=r[2];goto L8c01abc6; /* add r2,r0 */
L8c01abc6: r[0]=sx(m,r[0]+0x5u,1);goto L8c01abc8; /* mov.b @(0x5,r0),r0 */
L8c01abc8: r[2]=1u;goto L8c01abca; /* mov #0x1,r2 */
L8c01abca: wr(m,r[3]+0x7u,r[0],1);goto L8c01abcc; /* mov.b r0,@(0x7,r3) */
L8c01abcc: r[3]=rd(m,r[6],4);goto L8c01abce; /* mov.l @r6,r3 */
L8c01abce: r[6]=rd(m,0x8c01ac68u,4);goto L8c01abd0; /* mov.l 0x8c01ac68,r6 */
L8c01abd0: wr(m,r[3],r[2],4);goto L8c01abd2; /* mov.l r2,@r3 */
L8c01abd2: r[3]=rd(m,0x8c01ac6cu,4);goto L8c01abd4; /* mov.l 0x8c01ac6c,r3 */
L8c01abd4: target=r[3];r[4]=8u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c01abd8: return 0; /* rts */
}
