/* Dispatcher prefix gate 8c0afb10, inferred name; static C generated from
 * checked retail bytes. R4=object and R6=work (the draft signature is unreliable).
 * Host 0 completes; guest result is R0. Missing/rejected service returns -2 at
 * its reached call boundary; service/scalar errors preserve partial state.
 * FMOV obeys FR/SZ. PR=1 is rejected only at reached FMUL/FNEG, not entry.
 * Valid original RAM/pointers required; MMIO/exception delivery excluded.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_multiply_extended.h"
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
SA_STAGE_EXPORT int sa_main_dispatch_prefix_gate_8c0afb10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;
 int status;uint32_t target;
 int take;
 goto L8c0afb10;
L8c0afb10: r[15]-=4;wr(m,r[15],r[14],4);goto L8c0afb12; /* mov.l r14,@-r15 */
L8c0afb12: r[14]=r[4];goto L8c0afb14; /* mov r4,r14 */
L8c0afb14: r[15]-=4;wr(m,r[15],r[13],4);goto L8c0afb16; /* mov.l r13,@-r15 */
L8c0afb16: r[15]-=4;wr(m,r[15],r[12],4);goto L8c0afb18; /* mov.l r12,@-r15 */
L8c0afb18: r[15]-=4;wr(m,r[15],*pr,4);goto L8c0afb1a; /* sts.l PR,@-r15 */
L8c0afb1a: r[0]=sx(m,r[14]+0x2u,1);goto L8c0afb1c; /* mov.b @(0x2,r14),r0 */
L8c0afb1c: r[0]=r[0]&255u;goto L8c0afb1e; /* extu.b r0,r0 */
L8c0afb1e: *sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c0afb20; /* cmp/eq #0x2,r0 */
L8c0afb20: take=!(*sr&1u);r[13]=r[6];if(take)goto L8c0afb28;goto L8c0afb24; /* bf/s 0x8c0afb28 */
L8c0afb24: goto L8c0afc8c; /* bra 0x8c0afc8c */
L8c0afb28: r[0]=sx(m,r[14]+0x4u,2);goto L8c0afb2a; /* mov.w @(0x4,r14),r0 */
L8c0afb2a: *sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c0afb2c; /* tst #0x4,r0 */
L8c0afb2c: if(!(*sr&1u))goto L8c0afb44;goto L8c0afb2e; /* bf 0x8c0afb44 */
L8c0afb2e: *sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c0afb30; /* tst r13,r13 */
L8c0afb30: if(!(*sr&1u))goto L8c0afb36;goto L8c0afb32; /* bf 0x8c0afb36 */
L8c0afb32: goto L8c0afc8c; /* bra 0x8c0afc8c */
L8c0afb36: r[0]=92u;goto L8c0afb38; /* mov #0x5c,r0 */
L8c0afb38: r[3]=rd(m,0x8c0afbd0u,4);goto L8c0afb3a; /* mov.l 0x8c0afbd0,r3 */
L8c0afb3a: r[2]=rd(m,r[13]+r[0],4);goto L8c0afb3c; /* mov.l @(r0,r13),r2 */
L8c0afb3c: *sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c0afb3e; /* tst r3,r2 */
L8c0afb3e: if(!(*sr&1u))goto L8c0afb44;goto L8c0afb40; /* bf 0x8c0afb44 */
L8c0afb40: goto L8c0afc8c; /* bra 0x8c0afc8c */
L8c0afb44: r[0]=120u;goto L8c0afb46; /* mov #0x78,r0 */
L8c0afb46: r[3]=rd(m,0x8c0afbd4u,4);goto L8c0afb48; /* mov.l 0x8c0afbd4,r3 */
L8c0afb48: wr(m,r[14]+0x6u,r[0],2);goto L8c0afb4a; /* mov.w r0,@(0x6,r14) */
L8c0afb4a: r[5]=7u;goto L8c0afb4c; /* mov #0x7,r5 */
L8c0afb4c: r[0]=sx(m,r[14]+0x8u,1);goto L8c0afb4e; /* mov.b @(0x8,r14),r0 */
L8c0afb4e: r[7]=6u;goto L8c0afb50; /* mov #0x6,r7 */
L8c0afb50: r[6]=59u;goto L8c0afb52; /* mov #0x3b,r6 */
L8c0afb52: target=r[3];*pr=0x8c0afb56u;r[4]=r[0]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afb56; /* jsr @r3 */
L8c0afb56: r[4]=sx(m,r[14],1);goto L8c0afb58; /* mov.b @r14,r4 */
L8c0afb58: r[2]=46u;goto L8c0afb5a; /* mov #0x2e,r2 */
L8c0afb5a: r[12]=rd(m,0x8c0afbd8u,4);goto L8c0afb5c; /* mov.l 0x8c0afbd8,r12 */
L8c0afb5c: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]);goto L8c0afb5e; /* cmp/ge r2,r4 */
L8c0afb5e: if(!(*sr&1u))goto L8c0afbe0;goto L8c0afb60; /* bf 0x8c0afbe0 */
L8c0afb60: r[1]=50u;goto L8c0afb62; /* mov #0x32,r1 */
L8c0afb62: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[1]);goto L8c0afb64; /* cmp/gt r1,r4 */
L8c0afb64: if((*sr&1u))goto L8c0afbe0;goto L8c0afb66; /* bt 0x8c0afbe0 */
L8c0afb66: r[4]=4u;goto L8c0afb68; /* mov #0x4,r4 */
L8c0afb68: r[4]<<=8;goto L8c0afb6a; /* shll8 r4 */
L8c0afb6a: r[7]=0u;goto L8c0afb6c; /* mov #0x0,r7 */
L8c0afb6c: r[4]+=88u;goto L8c0afb6e; /* add #0x58,r4 */
L8c0afb6e: r[6]=r[7];goto L8c0afb70; /* mov r7,r6 */
L8c0afb70: target=r[12];*pr=0x8c0afb74u;r[5]=r[7];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afb74; /* jsr @r12 */
L8c0afb74: r[0]=73u;goto L8c0afb76; /* mov #0x49,r0 */
L8c0afb76: r[0]<<=2;goto L8c0afb78; /* shll2 r0 */
L8c0afb78: r[3]=46u;goto L8c0afb7a; /* mov #0x2e,r3 */
L8c0afb7a: r[2]=50u;goto L8c0afb7c; /* mov #0x32,r2 */
L8c0afb7c: wr(m,r[14],r[2],1);goto L8c0afb7e; /* mov.b r2,@r14 */
L8c0afb7e: wr(m,r[0]+r[13],r[3],2);goto L8c0afb80; /* mov.w r3,@(r0,r13) */
L8c0afb80: r[3]=rd(m,0x8c0afbdcu,4);goto L8c0afb82; /* mov.l 0x8c0afbdc,r3 */
L8c0afb82: target=r[3];*pr=0x8c0afb86u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afb86; /* jsr @r3 */
L8c0afb86: goto L8c0afc88; /* bra 0x8c0afc88 */
L8c0afbe0: r[4]=sx(m,r[14],1);goto L8c0afbe2; /* mov.b @r14,r4 */
L8c0afbe2: r[3]=62u;goto L8c0afbe4; /* mov #0x3e,r3 */
L8c0afbe4: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]);goto L8c0afbe6; /* cmp/ge r3,r4 */
L8c0afbe6: if(!(*sr&1u))goto L8c0afc12;goto L8c0afbe8; /* bf 0x8c0afc12 */
L8c0afbe8: r[1]=68u;goto L8c0afbea; /* mov #0x44,r1 */
L8c0afbea: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[1]);goto L8c0afbec; /* cmp/gt r1,r4 */
L8c0afbec: if((*sr&1u))goto L8c0afc12;goto L8c0afbee; /* bt 0x8c0afc12 */
L8c0afbee: r[4]=4u;goto L8c0afbf0; /* mov #0x4,r4 */
L8c0afbf0: r[4]<<=8;goto L8c0afbf2; /* shll8 r4 */
L8c0afbf2: r[7]=0u;goto L8c0afbf4; /* mov #0x0,r7 */
L8c0afbf4: r[4]+=99u;goto L8c0afbf6; /* add #0x63,r4 */
L8c0afbf6: r[6]=r[7];goto L8c0afbf8; /* mov r7,r6 */
L8c0afbf8: target=r[12];*pr=0x8c0afbfcu;r[5]=r[7];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afbfc; /* jsr @r12 */
L8c0afbfc: r[0]=73u;goto L8c0afbfe; /* mov #0x49,r0 */
L8c0afbfe: r[0]<<=2;goto L8c0afc00; /* shll2 r0 */
L8c0afc00: r[2]=110u;goto L8c0afc02; /* mov #0x6e,r2 */
L8c0afc02: r[3]=68u;goto L8c0afc04; /* mov #0x44,r3 */
L8c0afc04: wr(m,r[14],r[3],1);goto L8c0afc06; /* mov.b r3,@r14 */
L8c0afc06: r[3]=4294967291u;goto L8c0afc08; /* mov #-0x5,r3 */
L8c0afc08: wr(m,r[0]+r[13],r[2],2);goto L8c0afc0a; /* mov.w r2,@(r0,r13) */
L8c0afc0a: r[0]=sx(m,r[14]+0x4u,2);goto L8c0afc0c; /* mov.w @(0x4,r14),r0 */
L8c0afc0c: r[0]&=r[3];goto L8c0afc0e; /* and r3,r0 */
L8c0afc0e: wr(m,r[14]+0x4u,r[0],2);goto L8c0afc8c; /* bra 0x8c0afc8c */
L8c0afc12: r[3]=rd(m,0x8c0afd60u,4);goto L8c0afc14; /* mov.l 0x8c0afd60,r3 */
L8c0afc14: r[0]=sx(m,r[13]+0x4u,2);goto L8c0afc16; /* mov.w @(0x4,r13),r0 */
L8c0afc16: *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c0afc18; /* tst r3,r0 */
L8c0afc18: if((*sr&1u))goto L8c0afc2a;goto L8c0afc1a; /* bt 0x8c0afc2a */
L8c0afc1a: r[1]=rd(m,0x8c0afd64u,4);goto L8c0afc1c; /* mov.l 0x8c0afd64,r1 */
L8c0afc1c: r[0]=56u;goto L8c0afc1e; /* mov #0x38,r0 */
L8c0afc1e: fload(m,f,2u,r[0]+r[13]);goto L8c0afc20; /* fmov.s @(r0,r13),fr2 */
L8c0afc20: *fpul=r[1];goto L8c0afc22; /* lds r1,FPUL */
L8c0afc22: f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto L8c0afc24; /* fsts FPUL,fr3 */
L8c0afc24: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0afc26; /* fmul fr3,fr2 */
L8c0afc26: fstore(m,f,2u,r[0]+r[13]);goto L8c0afc8c; /* bra 0x8c0afc8c */
L8c0afc2a: r[0]=sx(m,r[14],1);goto L8c0afc2c; /* mov.b @r14,r0 */
L8c0afc2c: *sr=(*sr&~1u)|(r[0]==0x0000002du);goto L8c0afc2e; /* cmp/eq #0x2d,r0 */
L8c0afc2e: if((*sr&1u))goto L8c0afc8c;goto L8c0afc30; /* bt 0x8c0afc8c */
L8c0afc30: r[0]=sx(m,r[14],1);goto L8c0afc32; /* mov.b @r14,r0 */
L8c0afc32: *sr=(*sr&~1u)|(r[0]==0x0000002cu);goto L8c0afc34; /* cmp/eq #0x2c,r0 */
L8c0afc34: if(!(*sr&1u))goto L8c0afc3c;goto L8c0afc36; /* bf 0x8c0afc3c */
L8c0afc36: r[1]=rd(m,0x8c0afd68u,4);goto L8c0afc38; /* mov.l 0x8c0afd68,r1 */
L8c0afc38: target=r[1];*pr=0x8c0afc3cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afc3c; /* jsr @r1 */
L8c0afc3c: r[4]=4u;goto L8c0afc3e; /* mov #0x4,r4 */
L8c0afc3e: r[4]<<=8;goto L8c0afc40; /* shll8 r4 */
L8c0afc40: r[7]=0u;goto L8c0afc42; /* mov #0x0,r7 */
L8c0afc42: r[4]+=99u;goto L8c0afc44; /* add #0x63,r4 */
L8c0afc44: r[6]=r[7];goto L8c0afc46; /* mov r7,r6 */
L8c0afc46: target=r[12];*pr=0x8c0afc4au;r[5]=r[7];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afc4a; /* jsr @r12 */
L8c0afc4a: r[0]=73u;goto L8c0afc4c; /* mov #0x49,r0 */
L8c0afc4c: r[2]=23u;goto L8c0afc4e; /* mov #0x17,r2 */
L8c0afc4e: r[0]<<=2;goto L8c0afc50; /* shll2 r0 */
L8c0afc50: r[3]=16u;goto L8c0afc52; /* mov #0x10,r3 */
L8c0afc52: wr(m,r[14],r[3],1);goto L8c0afc54; /* mov.b r3,@r14 */
L8c0afc54: r[3]=4294967291u;goto L8c0afc56; /* mov #-0x5,r3 */
L8c0afc56: wr(m,r[0]+r[13],r[2],2);goto L8c0afc58; /* mov.w r2,@(r0,r13) */
L8c0afc58: r[5]=8u;goto L8c0afc5a; /* mov #0x8,r5 */
L8c0afc5a: r[0]=sx(m,r[14]+0x4u,2);goto L8c0afc5c; /* mov.w @(0x4,r14),r0 */
L8c0afc5c: r[2]=rd(m,0x8c0afd6cu,4);goto L8c0afc5e; /* mov.l 0x8c0afd6c,r2 */
L8c0afc5e: r[0]&=r[3];goto L8c0afc60; /* and r3,r0 */
L8c0afc60: wr(m,r[14]+0x4u,r[0],2);goto L8c0afc62; /* mov.w r0,@(0x4,r14) */
L8c0afc62: target=r[2];*pr=0x8c0afc66u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afc66; /* jsr @r2 */
L8c0afc66: r[0]=4294967220u;goto L8c0afc68; /* mov #-0x4c,r0 */
L8c0afc68: r[0]=r[0]&255u;goto L8c0afc6a; /* extu.b r0,r0 */
L8c0afc6a: fload(m,f,3u,r[0]+r[13]);goto L8c0afc6c; /* fmov.s @(r0,r13),fr3 */
L8c0afc6c: r[0]=60u;goto L8c0afc6e; /* mov #0x3c,r0 */
L8c0afc6e: r[3]=4294967291u;goto L8c0afc70; /* mov #-0x5,r3 */
L8c0afc70: fstore(m,f,3u,r[0]+r[13]);goto L8c0afc72; /* fmov fr3,@(r0,r13) */
L8c0afc72: r[0]=4294967236u;goto L8c0afc74; /* mov #-0x3c,r0 */
L8c0afc74: r[0]=r[0]&255u;goto L8c0afc76; /* extu.b r0,r0 */
L8c0afc76: fload(m,f,3u,r[0]+r[13]);goto L8c0afc78; /* fmov.s @(r0,r13),fr3 */
L8c0afc78: r[0]=56u;goto L8c0afc7a; /* mov #0x38,r0 */
L8c0afc7a: r[3]<<=8;goto L8c0afc7c; /* shll8 r3 */
L8c0afc7c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c0afc7e; /* fneg fr3 */
L8c0afc7e: r[3]+=4294967295u;goto L8c0afc80; /* add #-0x1,r3 */
L8c0afc80: fstore(m,f,3u,r[0]+r[13]);goto L8c0afc82; /* fmov fr3,@(r0,r13) */
L8c0afc82: r[0]=sx(m,r[14]+0x4u,2);goto L8c0afc84; /* mov.w @(0x4,r14),r0 */
L8c0afc84: r[0]&=r[3];goto L8c0afc86; /* and r3,r0 */
L8c0afc86: wr(m,r[14]+0x4u,r[0],2);goto L8c0afc88; /* mov.w r0,@(0x4,r14) */
L8c0afc88: r[0]=1u;goto L8c0afc8e; /* bra 0x8c0afc8e */
L8c0afc8c: r[0]=0u;goto L8c0afc8e; /* mov #0x0,r0 */
L8c0afc8e: *pr=rd(m,r[15],4);r[15]+=4;goto L8c0afc90; /* lds.l @r15+,PR */
L8c0afc90: r[12]=rd(m,r[15],4);r[15]+=4;goto L8c0afc92; /* mov.l @r15+,r12 */
L8c0afc92: r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0afc94; /* mov.l @r15+,r13 */
L8c0afc94: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
