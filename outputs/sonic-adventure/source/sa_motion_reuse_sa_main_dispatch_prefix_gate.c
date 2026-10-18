#include "sa_draw_motion_stop.h"
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
L8c0afb10: sa_motion_publish_stop(ctx,0x8c0afb10u); r[15]-=4;wr(m,r[15],r[14],4);goto L8c0afb12; /* mov.l r14,@-r15 */
L8c0afb12: sa_motion_publish_stop(ctx,0x8c0afb12u); r[14]=r[4];goto L8c0afb14; /* mov r4,r14 */
L8c0afb14: sa_motion_publish_stop(ctx,0x8c0afb14u); r[15]-=4;wr(m,r[15],r[13],4);goto L8c0afb16; /* mov.l r13,@-r15 */
L8c0afb16: sa_motion_publish_stop(ctx,0x8c0afb16u); r[15]-=4;wr(m,r[15],r[12],4);goto L8c0afb18; /* mov.l r12,@-r15 */
L8c0afb18: sa_motion_publish_stop(ctx,0x8c0afb18u); r[15]-=4;wr(m,r[15],*pr,4);goto L8c0afb1a; /* sts.l PR,@-r15 */
L8c0afb1a: sa_motion_publish_stop(ctx,0x8c0afb1au); r[0]=sx(m,r[14]+0x2u,1);goto L8c0afb1c; /* mov.b @(0x2,r14),r0 */
L8c0afb1c: sa_motion_publish_stop(ctx,0x8c0afb1cu); r[0]=r[0]&255u;goto L8c0afb1e; /* extu.b r0,r0 */
L8c0afb1e: sa_motion_publish_stop(ctx,0x8c0afb1eu); *sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c0afb20; /* cmp/eq #0x2,r0 */
L8c0afb20: sa_motion_publish_stop(ctx,0x8c0afb22u); take=!(*sr&1u);r[13]=r[6];if(take)goto L8c0afb28;goto L8c0afb24; /* bf/s 0x8c0afb28 */
L8c0afb24: sa_motion_publish_stop(ctx,0x8c0afb26u); goto L8c0afc8c; /* bra 0x8c0afc8c */
L8c0afb28: sa_motion_publish_stop(ctx,0x8c0afb28u); r[0]=sx(m,r[14]+0x4u,2);goto L8c0afb2a; /* mov.w @(0x4,r14),r0 */
L8c0afb2a: sa_motion_publish_stop(ctx,0x8c0afb2au); *sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c0afb2c; /* tst #0x4,r0 */
L8c0afb2c: sa_motion_publish_stop(ctx,0x8c0afb2cu); if(!(*sr&1u))goto L8c0afb44;goto L8c0afb2e; /* bf 0x8c0afb44 */
L8c0afb2e: sa_motion_publish_stop(ctx,0x8c0afb2eu); *sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c0afb30; /* tst r13,r13 */
L8c0afb30: sa_motion_publish_stop(ctx,0x8c0afb30u); if(!(*sr&1u))goto L8c0afb36;goto L8c0afb32; /* bf 0x8c0afb36 */
L8c0afb32: sa_motion_publish_stop(ctx,0x8c0afb34u); goto L8c0afc8c; /* bra 0x8c0afc8c */
L8c0afb36: sa_motion_publish_stop(ctx,0x8c0afb36u); r[0]=92u;goto L8c0afb38; /* mov #0x5c,r0 */
L8c0afb38: sa_motion_publish_stop(ctx,0x8c0afb38u); r[3]=rd(m,0x8c0afbd0u,4);goto L8c0afb3a; /* mov.l 0x8c0afbd0,r3 */
L8c0afb3a: sa_motion_publish_stop(ctx,0x8c0afb3au); r[2]=rd(m,r[13]+r[0],4);goto L8c0afb3c; /* mov.l @(r0,r13),r2 */
L8c0afb3c: sa_motion_publish_stop(ctx,0x8c0afb3cu); *sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c0afb3e; /* tst r3,r2 */
L8c0afb3e: sa_motion_publish_stop(ctx,0x8c0afb3eu); if(!(*sr&1u))goto L8c0afb44;goto L8c0afb40; /* bf 0x8c0afb44 */
L8c0afb40: sa_motion_publish_stop(ctx,0x8c0afb42u); goto L8c0afc8c; /* bra 0x8c0afc8c */
L8c0afb44: sa_motion_publish_stop(ctx,0x8c0afb44u); r[0]=120u;goto L8c0afb46; /* mov #0x78,r0 */
L8c0afb46: sa_motion_publish_stop(ctx,0x8c0afb46u); r[3]=rd(m,0x8c0afbd4u,4);goto L8c0afb48; /* mov.l 0x8c0afbd4,r3 */
L8c0afb48: sa_motion_publish_stop(ctx,0x8c0afb48u); wr(m,r[14]+0x6u,r[0],2);goto L8c0afb4a; /* mov.w r0,@(0x6,r14) */
L8c0afb4a: sa_motion_publish_stop(ctx,0x8c0afb4au); r[5]=7u;goto L8c0afb4c; /* mov #0x7,r5 */
L8c0afb4c: sa_motion_publish_stop(ctx,0x8c0afb4cu); r[0]=sx(m,r[14]+0x8u,1);goto L8c0afb4e; /* mov.b @(0x8,r14),r0 */
L8c0afb4e: sa_motion_publish_stop(ctx,0x8c0afb4eu); r[7]=6u;goto L8c0afb50; /* mov #0x6,r7 */
L8c0afb50: sa_motion_publish_stop(ctx,0x8c0afb50u); r[6]=59u;goto L8c0afb52; /* mov #0x3b,r6 */
L8c0afb52: sa_motion_publish_stop(ctx,0x8c0afb54u); target=r[3];*pr=0x8c0afb56u;r[4]=r[0]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afb56; /* jsr @r3 */
L8c0afb56: sa_motion_publish_stop(ctx,0x8c0afb56u); r[4]=sx(m,r[14],1);goto L8c0afb58; /* mov.b @r14,r4 */
L8c0afb58: sa_motion_publish_stop(ctx,0x8c0afb58u); r[2]=46u;goto L8c0afb5a; /* mov #0x2e,r2 */
L8c0afb5a: sa_motion_publish_stop(ctx,0x8c0afb5au); r[12]=rd(m,0x8c0afbd8u,4);goto L8c0afb5c; /* mov.l 0x8c0afbd8,r12 */
L8c0afb5c: sa_motion_publish_stop(ctx,0x8c0afb5cu); *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]);goto L8c0afb5e; /* cmp/ge r2,r4 */
L8c0afb5e: sa_motion_publish_stop(ctx,0x8c0afb5eu); if(!(*sr&1u))goto L8c0afbe0;goto L8c0afb60; /* bf 0x8c0afbe0 */
L8c0afb60: sa_motion_publish_stop(ctx,0x8c0afb60u); r[1]=50u;goto L8c0afb62; /* mov #0x32,r1 */
L8c0afb62: sa_motion_publish_stop(ctx,0x8c0afb62u); *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[1]);goto L8c0afb64; /* cmp/gt r1,r4 */
L8c0afb64: sa_motion_publish_stop(ctx,0x8c0afb64u); if((*sr&1u))goto L8c0afbe0;goto L8c0afb66; /* bt 0x8c0afbe0 */
L8c0afb66: sa_motion_publish_stop(ctx,0x8c0afb66u); r[4]=4u;goto L8c0afb68; /* mov #0x4,r4 */
L8c0afb68: sa_motion_publish_stop(ctx,0x8c0afb68u); r[4]<<=8;goto L8c0afb6a; /* shll8 r4 */
L8c0afb6a: sa_motion_publish_stop(ctx,0x8c0afb6au); r[7]=0u;goto L8c0afb6c; /* mov #0x0,r7 */
L8c0afb6c: sa_motion_publish_stop(ctx,0x8c0afb6cu); r[4]+=88u;goto L8c0afb6e; /* add #0x58,r4 */
L8c0afb6e: sa_motion_publish_stop(ctx,0x8c0afb6eu); r[6]=r[7];goto L8c0afb70; /* mov r7,r6 */
L8c0afb70: sa_motion_publish_stop(ctx,0x8c0afb72u); target=r[12];*pr=0x8c0afb74u;r[5]=r[7];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afb74; /* jsr @r12 */
L8c0afb74: sa_motion_publish_stop(ctx,0x8c0afb74u); r[0]=73u;goto L8c0afb76; /* mov #0x49,r0 */
L8c0afb76: sa_motion_publish_stop(ctx,0x8c0afb76u); r[0]<<=2;goto L8c0afb78; /* shll2 r0 */
L8c0afb78: sa_motion_publish_stop(ctx,0x8c0afb78u); r[3]=46u;goto L8c0afb7a; /* mov #0x2e,r3 */
L8c0afb7a: sa_motion_publish_stop(ctx,0x8c0afb7au); r[2]=50u;goto L8c0afb7c; /* mov #0x32,r2 */
L8c0afb7c: sa_motion_publish_stop(ctx,0x8c0afb7cu); wr(m,r[14],r[2],1);goto L8c0afb7e; /* mov.b r2,@r14 */
L8c0afb7e: sa_motion_publish_stop(ctx,0x8c0afb7eu); wr(m,r[0]+r[13],r[3],2);goto L8c0afb80; /* mov.w r3,@(r0,r13) */
L8c0afb80: sa_motion_publish_stop(ctx,0x8c0afb80u); r[3]=rd(m,0x8c0afbdcu,4);goto L8c0afb82; /* mov.l 0x8c0afbdc,r3 */
L8c0afb82: sa_motion_publish_stop(ctx,0x8c0afb84u); target=r[3];*pr=0x8c0afb86u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afb86; /* jsr @r3 */
L8c0afb86: sa_motion_publish_stop(ctx,0x8c0afb88u); goto L8c0afc88; /* bra 0x8c0afc88 */
L8c0afbe0: sa_motion_publish_stop(ctx,0x8c0afbe0u); r[4]=sx(m,r[14],1);goto L8c0afbe2; /* mov.b @r14,r4 */
L8c0afbe2: sa_motion_publish_stop(ctx,0x8c0afbe2u); r[3]=62u;goto L8c0afbe4; /* mov #0x3e,r3 */
L8c0afbe4: sa_motion_publish_stop(ctx,0x8c0afbe4u); *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]);goto L8c0afbe6; /* cmp/ge r3,r4 */
L8c0afbe6: sa_motion_publish_stop(ctx,0x8c0afbe6u); if(!(*sr&1u))goto L8c0afc12;goto L8c0afbe8; /* bf 0x8c0afc12 */
L8c0afbe8: sa_motion_publish_stop(ctx,0x8c0afbe8u); r[1]=68u;goto L8c0afbea; /* mov #0x44,r1 */
L8c0afbea: sa_motion_publish_stop(ctx,0x8c0afbeau); *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[1]);goto L8c0afbec; /* cmp/gt r1,r4 */
L8c0afbec: sa_motion_publish_stop(ctx,0x8c0afbecu); if((*sr&1u))goto L8c0afc12;goto L8c0afbee; /* bt 0x8c0afc12 */
L8c0afbee: sa_motion_publish_stop(ctx,0x8c0afbeeu); r[4]=4u;goto L8c0afbf0; /* mov #0x4,r4 */
L8c0afbf0: sa_motion_publish_stop(ctx,0x8c0afbf0u); r[4]<<=8;goto L8c0afbf2; /* shll8 r4 */
L8c0afbf2: sa_motion_publish_stop(ctx,0x8c0afbf2u); r[7]=0u;goto L8c0afbf4; /* mov #0x0,r7 */
L8c0afbf4: sa_motion_publish_stop(ctx,0x8c0afbf4u); r[4]+=99u;goto L8c0afbf6; /* add #0x63,r4 */
L8c0afbf6: sa_motion_publish_stop(ctx,0x8c0afbf6u); r[6]=r[7];goto L8c0afbf8; /* mov r7,r6 */
L8c0afbf8: sa_motion_publish_stop(ctx,0x8c0afbfau); target=r[12];*pr=0x8c0afbfcu;r[5]=r[7];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afbfc; /* jsr @r12 */
L8c0afbfc: sa_motion_publish_stop(ctx,0x8c0afbfcu); r[0]=73u;goto L8c0afbfe; /* mov #0x49,r0 */
L8c0afbfe: sa_motion_publish_stop(ctx,0x8c0afbfeu); r[0]<<=2;goto L8c0afc00; /* shll2 r0 */
L8c0afc00: sa_motion_publish_stop(ctx,0x8c0afc00u); r[2]=110u;goto L8c0afc02; /* mov #0x6e,r2 */
L8c0afc02: sa_motion_publish_stop(ctx,0x8c0afc02u); r[3]=68u;goto L8c0afc04; /* mov #0x44,r3 */
L8c0afc04: sa_motion_publish_stop(ctx,0x8c0afc04u); wr(m,r[14],r[3],1);goto L8c0afc06; /* mov.b r3,@r14 */
L8c0afc06: sa_motion_publish_stop(ctx,0x8c0afc06u); r[3]=4294967291u;goto L8c0afc08; /* mov #-0x5,r3 */
L8c0afc08: sa_motion_publish_stop(ctx,0x8c0afc08u); wr(m,r[0]+r[13],r[2],2);goto L8c0afc0a; /* mov.w r2,@(r0,r13) */
L8c0afc0a: sa_motion_publish_stop(ctx,0x8c0afc0au); r[0]=sx(m,r[14]+0x4u,2);goto L8c0afc0c; /* mov.w @(0x4,r14),r0 */
L8c0afc0c: sa_motion_publish_stop(ctx,0x8c0afc0cu); r[0]&=r[3];goto L8c0afc0e; /* and r3,r0 */
L8c0afc0e: sa_motion_publish_stop(ctx,0x8c0afc10u); wr(m,r[14]+0x4u,r[0],2);goto L8c0afc8c; /* bra 0x8c0afc8c */
L8c0afc12: sa_motion_publish_stop(ctx,0x8c0afc12u); r[3]=rd(m,0x8c0afd60u,4);goto L8c0afc14; /* mov.l 0x8c0afd60,r3 */
L8c0afc14: sa_motion_publish_stop(ctx,0x8c0afc14u); r[0]=sx(m,r[13]+0x4u,2);goto L8c0afc16; /* mov.w @(0x4,r13),r0 */
L8c0afc16: sa_motion_publish_stop(ctx,0x8c0afc16u); *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c0afc18; /* tst r3,r0 */
L8c0afc18: sa_motion_publish_stop(ctx,0x8c0afc18u); if((*sr&1u))goto L8c0afc2a;goto L8c0afc1a; /* bt 0x8c0afc2a */
L8c0afc1a: sa_motion_publish_stop(ctx,0x8c0afc1au); r[1]=rd(m,0x8c0afd64u,4);goto L8c0afc1c; /* mov.l 0x8c0afd64,r1 */
L8c0afc1c: sa_motion_publish_stop(ctx,0x8c0afc1cu); r[0]=56u;goto L8c0afc1e; /* mov #0x38,r0 */
L8c0afc1e: sa_motion_publish_stop(ctx,0x8c0afc1eu); fload(m,f,2u,r[0]+r[13]);goto L8c0afc20; /* fmov.s @(r0,r13),fr2 */
L8c0afc20: sa_motion_publish_stop(ctx,0x8c0afc20u); *fpul=r[1];goto L8c0afc22; /* lds r1,FPUL */
L8c0afc22: sa_motion_publish_stop(ctx,0x8c0afc22u); f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto L8c0afc24; /* fsts FPUL,fr3 */
L8c0afc24: sa_motion_publish_stop(ctx,0x8c0afc24u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0afc26; /* fmul fr3,fr2 */
L8c0afc26: sa_motion_publish_stop(ctx,0x8c0afc28u); fstore(m,f,2u,r[0]+r[13]);goto L8c0afc8c; /* bra 0x8c0afc8c */
L8c0afc2a: sa_motion_publish_stop(ctx,0x8c0afc2au); r[0]=sx(m,r[14],1);goto L8c0afc2c; /* mov.b @r14,r0 */
L8c0afc2c: sa_motion_publish_stop(ctx,0x8c0afc2cu); *sr=(*sr&~1u)|(r[0]==0x0000002du);goto L8c0afc2e; /* cmp/eq #0x2d,r0 */
L8c0afc2e: sa_motion_publish_stop(ctx,0x8c0afc2eu); if((*sr&1u))goto L8c0afc8c;goto L8c0afc30; /* bt 0x8c0afc8c */
L8c0afc30: sa_motion_publish_stop(ctx,0x8c0afc30u); r[0]=sx(m,r[14],1);goto L8c0afc32; /* mov.b @r14,r0 */
L8c0afc32: sa_motion_publish_stop(ctx,0x8c0afc32u); *sr=(*sr&~1u)|(r[0]==0x0000002cu);goto L8c0afc34; /* cmp/eq #0x2c,r0 */
L8c0afc34: sa_motion_publish_stop(ctx,0x8c0afc34u); if(!(*sr&1u))goto L8c0afc3c;goto L8c0afc36; /* bf 0x8c0afc3c */
L8c0afc36: sa_motion_publish_stop(ctx,0x8c0afc36u); r[1]=rd(m,0x8c0afd68u,4);goto L8c0afc38; /* mov.l 0x8c0afd68,r1 */
L8c0afc38: sa_motion_publish_stop(ctx,0x8c0afc3au); target=r[1];*pr=0x8c0afc3cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afc3c; /* jsr @r1 */
L8c0afc3c: sa_motion_publish_stop(ctx,0x8c0afc3cu); r[4]=4u;goto L8c0afc3e; /* mov #0x4,r4 */
L8c0afc3e: sa_motion_publish_stop(ctx,0x8c0afc3eu); r[4]<<=8;goto L8c0afc40; /* shll8 r4 */
L8c0afc40: sa_motion_publish_stop(ctx,0x8c0afc40u); r[7]=0u;goto L8c0afc42; /* mov #0x0,r7 */
L8c0afc42: sa_motion_publish_stop(ctx,0x8c0afc42u); r[4]+=99u;goto L8c0afc44; /* add #0x63,r4 */
L8c0afc44: sa_motion_publish_stop(ctx,0x8c0afc44u); r[6]=r[7];goto L8c0afc46; /* mov r7,r6 */
L8c0afc46: sa_motion_publish_stop(ctx,0x8c0afc48u); target=r[12];*pr=0x8c0afc4au;r[5]=r[7];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afc4a; /* jsr @r12 */
L8c0afc4a: sa_motion_publish_stop(ctx,0x8c0afc4au); r[0]=73u;goto L8c0afc4c; /* mov #0x49,r0 */
L8c0afc4c: sa_motion_publish_stop(ctx,0x8c0afc4cu); r[2]=23u;goto L8c0afc4e; /* mov #0x17,r2 */
L8c0afc4e: sa_motion_publish_stop(ctx,0x8c0afc4eu); r[0]<<=2;goto L8c0afc50; /* shll2 r0 */
L8c0afc50: sa_motion_publish_stop(ctx,0x8c0afc50u); r[3]=16u;goto L8c0afc52; /* mov #0x10,r3 */
L8c0afc52: sa_motion_publish_stop(ctx,0x8c0afc52u); wr(m,r[14],r[3],1);goto L8c0afc54; /* mov.b r3,@r14 */
L8c0afc54: sa_motion_publish_stop(ctx,0x8c0afc54u); r[3]=4294967291u;goto L8c0afc56; /* mov #-0x5,r3 */
L8c0afc56: sa_motion_publish_stop(ctx,0x8c0afc56u); wr(m,r[0]+r[13],r[2],2);goto L8c0afc58; /* mov.w r2,@(r0,r13) */
L8c0afc58: sa_motion_publish_stop(ctx,0x8c0afc58u); r[5]=8u;goto L8c0afc5a; /* mov #0x8,r5 */
L8c0afc5a: sa_motion_publish_stop(ctx,0x8c0afc5au); r[0]=sx(m,r[14]+0x4u,2);goto L8c0afc5c; /* mov.w @(0x4,r14),r0 */
L8c0afc5c: sa_motion_publish_stop(ctx,0x8c0afc5cu); r[2]=rd(m,0x8c0afd6cu,4);goto L8c0afc5e; /* mov.l 0x8c0afd6c,r2 */
L8c0afc5e: sa_motion_publish_stop(ctx,0x8c0afc5eu); r[0]&=r[3];goto L8c0afc60; /* and r3,r0 */
L8c0afc60: sa_motion_publish_stop(ctx,0x8c0afc60u); wr(m,r[14]+0x4u,r[0],2);goto L8c0afc62; /* mov.w r0,@(0x4,r14) */
L8c0afc62: sa_motion_publish_stop(ctx,0x8c0afc64u); target=r[2];*pr=0x8c0afc66u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0afc66; /* jsr @r2 */
L8c0afc66: sa_motion_publish_stop(ctx,0x8c0afc66u); r[0]=4294967220u;goto L8c0afc68; /* mov #-0x4c,r0 */
L8c0afc68: sa_motion_publish_stop(ctx,0x8c0afc68u); r[0]=r[0]&255u;goto L8c0afc6a; /* extu.b r0,r0 */
L8c0afc6a: sa_motion_publish_stop(ctx,0x8c0afc6au); fload(m,f,3u,r[0]+r[13]);goto L8c0afc6c; /* fmov.s @(r0,r13),fr3 */
L8c0afc6c: sa_motion_publish_stop(ctx,0x8c0afc6cu); r[0]=60u;goto L8c0afc6e; /* mov #0x3c,r0 */
L8c0afc6e: sa_motion_publish_stop(ctx,0x8c0afc6eu); r[3]=4294967291u;goto L8c0afc70; /* mov #-0x5,r3 */
L8c0afc70: sa_motion_publish_stop(ctx,0x8c0afc70u); fstore(m,f,3u,r[0]+r[13]);goto L8c0afc72; /* fmov fr3,@(r0,r13) */
L8c0afc72: sa_motion_publish_stop(ctx,0x8c0afc72u); r[0]=4294967236u;goto L8c0afc74; /* mov #-0x3c,r0 */
L8c0afc74: sa_motion_publish_stop(ctx,0x8c0afc74u); r[0]=r[0]&255u;goto L8c0afc76; /* extu.b r0,r0 */
L8c0afc76: sa_motion_publish_stop(ctx,0x8c0afc76u); fload(m,f,3u,r[0]+r[13]);goto L8c0afc78; /* fmov.s @(r0,r13),fr3 */
L8c0afc78: sa_motion_publish_stop(ctx,0x8c0afc78u); r[0]=56u;goto L8c0afc7a; /* mov #0x38,r0 */
L8c0afc7a: sa_motion_publish_stop(ctx,0x8c0afc7au); r[3]<<=8;goto L8c0afc7c; /* shll8 r3 */
L8c0afc7c: sa_motion_publish_stop(ctx,0x8c0afc7cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c0afc7e; /* fneg fr3 */
L8c0afc7e: sa_motion_publish_stop(ctx,0x8c0afc7eu); r[3]+=4294967295u;goto L8c0afc80; /* add #-0x1,r3 */
L8c0afc80: sa_motion_publish_stop(ctx,0x8c0afc80u); fstore(m,f,3u,r[0]+r[13]);goto L8c0afc82; /* fmov fr3,@(r0,r13) */
L8c0afc82: sa_motion_publish_stop(ctx,0x8c0afc82u); r[0]=sx(m,r[14]+0x4u,2);goto L8c0afc84; /* mov.w @(0x4,r14),r0 */
L8c0afc84: sa_motion_publish_stop(ctx,0x8c0afc84u); r[0]&=r[3];goto L8c0afc86; /* and r3,r0 */
L8c0afc86: sa_motion_publish_stop(ctx,0x8c0afc86u); wr(m,r[14]+0x4u,r[0],2);goto L8c0afc88; /* mov.w r0,@(0x4,r14) */
L8c0afc88: sa_motion_publish_stop(ctx,0x8c0afc8au); r[0]=1u;goto L8c0afc8e; /* bra 0x8c0afc8e */
L8c0afc8c: sa_motion_publish_stop(ctx,0x8c0afc8cu); r[0]=0u;goto L8c0afc8e; /* mov #0x0,r0 */
L8c0afc8e: sa_motion_publish_stop(ctx,0x8c0afc8eu); *pr=rd(m,r[15],4);r[15]+=4;goto L8c0afc90; /* lds.l @r15+,PR */
L8c0afc90: sa_motion_publish_stop(ctx,0x8c0afc90u); r[12]=rd(m,r[15],4);r[15]+=4;goto L8c0afc92; /* mov.l @r15+,r12 */
L8c0afc92: sa_motion_publish_stop(ctx,0x8c0afc92u); r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0afc94; /* mov.l @r15+,r13 */
L8c0afc94: sa_motion_publish_stop(ctx,0x8c0afc96u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
