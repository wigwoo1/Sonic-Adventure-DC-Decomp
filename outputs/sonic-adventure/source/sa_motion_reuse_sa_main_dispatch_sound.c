#include "sa_draw_motion_stop.h"
/* Retail sound-request/slot-selection control flow, generated from checked
 * bytes. Names are inferred. No runtime opcode interpreter or audio output.
 * Valid original 16 MiB RAM/pointer preconditions; no MMIO/CPU exceptions.
 * FLDI0 is guarded only when reached with PR=1; FMOV stores obey FR/SZ.
 * Host 0 completes (retail result in R0), -1 unsupported reached FLDI0,
 * -2 missing service at its call boundary. Errors retain partial state.
 */
#include "fpu_cleanup_transfer.h"
typedef int (*sa_shared_service)(void *,uint32_t,uint8_t *,
    struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
static void wr(uint8_t *m,uint32_t a,uint32_t value,unsigned n){
 for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(value>>(8*i));
}
static void fstore(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  wr(m,a,f->bank[bank][reg],4);wr(m,a+4u,f->bank[bank][reg+1],4);
 }else wr(m,a,f->bank[bank][reg],4);
}
SA_STAGE_EXPORT int sa_sound_slot_select_8c093f02(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c093f02;
L8c093f02: sa_motion_publish_stop(ctx,0x8c093f02u); r[15]-=4;wr(m,r[15],r[14],4);goto L8c093f04; /* mov.l r14,@-r15 */
L8c093f04: sa_motion_publish_stop(ctx,0x8c093f04u); *sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c093f06; /* tst r5,r5 */
L8c093f06: sa_motion_publish_stop(ctx,0x8c093f06u); r[15]-=4;wr(m,r[15],r[13],4);goto L8c093f08; /* mov.l r13,@-r15 */
L8c093f08: sa_motion_publish_stop(ctx,0x8c093f08u); r[13]=32u;goto L8c093f0a; /* mov #0x20,r13 */
L8c093f0a: sa_motion_publish_stop(ctx,0x8c093f0au); r[15]-=4;wr(m,r[15],r[12],4);goto L8c093f0c; /* mov.l r12,@-r15 */
L8c093f0c: sa_motion_publish_stop(ctx,0x8c093f0cu); r[7]=4294967295u;goto L8c093f0e; /* mov #-0x1,r7 */
L8c093f0e: sa_motion_publish_stop(ctx,0x8c093f0eu); r[15]-=4;wr(m,r[15],r[10],4);goto L8c093f10; /* mov.l r10,@-r15 */
L8c093f10: sa_motion_publish_stop(ctx,0x8c093f10u); r[14]=rd(m,0x8c094078u,4);goto L8c093f12; /* mov.l 0x8c094078,r14 */
L8c093f12: sa_motion_publish_stop(ctx,0x8c093f12u); r[15]-=4;wr(m,r[15],r[9],4);goto L8c093f14; /* mov.l r9,@-r15 */
L8c093f14: sa_motion_publish_stop(ctx,0x8c093f14u); r[9]=1u;goto L8c093f16; /* mov #0x1,r9 */
L8c093f16: sa_motion_publish_stop(ctx,0x8c093f18u); take=(*sr&1u);r[6]=52u;if(take)goto L8c093fda;goto L8c093f1a; /* bt/s 0x8c093fda */
L8c093f1a: sa_motion_publish_stop(ctx,0x8c093f1au); r[0]=r[5];goto L8c093f1c; /* mov r5,r0 */
L8c093f1c: sa_motion_publish_stop(ctx,0x8c093f1cu); *sr=(*sr&~1u)|(r[0]==0x00000042u);goto L8c093f1e; /* cmp/eq #0x42,r0 */
L8c093f1e: sa_motion_publish_stop(ctx,0x8c093f1eu); if((*sr&1u))goto L8c093fda;goto L8c093f20; /* bt 0x8c093fda */
L8c093f20: sa_motion_publish_stop(ctx,0x8c093f20u); r[0]=r[5];goto L8c093f22; /* mov r5,r0 */
L8c093f22: sa_motion_publish_stop(ctx,0x8c093f22u); *sr=(*sr&~1u)|(r[0]==0x00000043u);goto L8c093f24; /* cmp/eq #0x43,r0 */
L8c093f24: sa_motion_publish_stop(ctx,0x8c093f24u); if((*sr&1u))goto L8c093fda;goto L8c093f26; /* bt 0x8c093fda */
L8c093f26: sa_motion_publish_stop(ctx,0x8c093f26u); r[0]=r[5];goto L8c093f28; /* mov r5,r0 */
L8c093f28: sa_motion_publish_stop(ctx,0x8c093f28u); *sr=(*sr&~1u)|(r[0]==0x0000007cu);goto L8c093f2a; /* cmp/eq #0x7c,r0 */
L8c093f2a: sa_motion_publish_stop(ctx,0x8c093f2au); if((*sr&1u))goto L8c093fda;goto L8c093f2c; /* bt 0x8c093fda */
L8c093f2c: sa_motion_publish_stop(ctx,0x8c093f2cu); r[3]=4294967201u;goto L8c093f2e; /* mov #-0x5f,r3 */
L8c093f2e: sa_motion_publish_stop(ctx,0x8c093f2eu); r[3]=r[3]&255u;goto L8c093f30; /* extu.b r3,r3 */
L8c093f30: sa_motion_publish_stop(ctx,0x8c093f30u); *sr=(*sr&~1u)|(r[3]==r[5]);goto L8c093f32; /* cmp/eq r3,r5 */
L8c093f32: sa_motion_publish_stop(ctx,0x8c093f32u); if((*sr&1u))goto L8c093fda;goto L8c093f34; /* bt 0x8c093fda */
L8c093f34: sa_motion_publish_stop(ctx,0x8c093f34u); r[1]=4294967200u;goto L8c093f36; /* mov #-0x60,r1 */
L8c093f36: sa_motion_publish_stop(ctx,0x8c093f36u); r[1]=r[1]&255u;goto L8c093f38; /* extu.b r1,r1 */
L8c093f38: sa_motion_publish_stop(ctx,0x8c093f38u); *sr=(*sr&~1u)|(r[1]==r[5]);goto L8c093f3a; /* cmp/eq r1,r5 */
L8c093f3a: sa_motion_publish_stop(ctx,0x8c093f3au); if((*sr&1u))goto L8c093fda;goto L8c093f3c; /* bt 0x8c093fda */
L8c093f3c: sa_motion_publish_stop(ctx,0x8c093f3cu); r[0]=4294967279u;goto L8c093f3e; /* mov #-0x11,r0 */
L8c093f3e: sa_motion_publish_stop(ctx,0x8c093f3eu); r[0]=r[0]&255u;goto L8c093f40; /* extu.b r0,r0 */
L8c093f40: sa_motion_publish_stop(ctx,0x8c093f40u); *sr=(*sr&~1u)|(r[0]==r[5]);goto L8c093f42; /* cmp/eq r0,r5 */
L8c093f42: sa_motion_publish_stop(ctx,0x8c093f42u); if((*sr&1u))goto L8c093fda;goto L8c093f44; /* bt 0x8c093fda */
L8c093f44: sa_motion_publish_stop(ctx,0x8c093f44u); r[2]=2u;goto L8c093f46; /* mov #0x2,r2 */
L8c093f46: sa_motion_publish_stop(ctx,0x8c093f46u); r[2]<<=8;goto L8c093f48; /* shll8 r2 */
L8c093f48: sa_motion_publish_stop(ctx,0x8c093f48u); r[2]+=41u;goto L8c093f4a; /* add #0x29,r2 */
L8c093f4a: sa_motion_publish_stop(ctx,0x8c093f4au); *sr=(*sr&~1u)|(r[2]==r[5]);goto L8c093f4c; /* cmp/eq r2,r5 */
L8c093f4c: sa_motion_publish_stop(ctx,0x8c093f4cu); if((*sr&1u))goto L8c093fda;goto L8c093f4e; /* bt 0x8c093fda */
L8c093f4e: sa_motion_publish_stop(ctx,0x8c093f4eu); r[1]=2u;goto L8c093f50; /* mov #0x2,r1 */
L8c093f50: sa_motion_publish_stop(ctx,0x8c093f50u); r[1]<<=8;goto L8c093f52; /* shll8 r1 */
L8c093f52: sa_motion_publish_stop(ctx,0x8c093f52u); r[1]+=42u;goto L8c093f54; /* add #0x2a,r1 */
L8c093f54: sa_motion_publish_stop(ctx,0x8c093f54u); *sr=(*sr&~1u)|(r[1]==r[5]);goto L8c093f56; /* cmp/eq r1,r5 */
L8c093f56: sa_motion_publish_stop(ctx,0x8c093f56u); if((*sr&1u))goto L8c093fda;goto L8c093f58; /* bt 0x8c093fda */
L8c093f58: sa_motion_publish_stop(ctx,0x8c093f58u); r[2]=4u;goto L8c093f5a; /* mov #0x4,r2 */
L8c093f5a: sa_motion_publish_stop(ctx,0x8c093f5au); r[2]<<=8;goto L8c093f5c; /* shll8 r2 */
L8c093f5c: sa_motion_publish_stop(ctx,0x8c093f5cu); r[2]+=35u;goto L8c093f5e; /* add #0x23,r2 */
L8c093f5e: sa_motion_publish_stop(ctx,0x8c093f5eu); *sr=(*sr&~1u)|(r[2]==r[5]);goto L8c093f60; /* cmp/eq r2,r5 */
L8c093f60: sa_motion_publish_stop(ctx,0x8c093f60u); if((*sr&1u))goto L8c093fda;goto L8c093f62; /* bt 0x8c093fda */
L8c093f62: sa_motion_publish_stop(ctx,0x8c093f62u); r[1]=4u;goto L8c093f64; /* mov #0x4,r1 */
L8c093f64: sa_motion_publish_stop(ctx,0x8c093f64u); r[1]<<=8;goto L8c093f66; /* shll8 r1 */
L8c093f66: sa_motion_publish_stop(ctx,0x8c093f66u); r[1]+=15u;goto L8c093f68; /* add #0xf,r1 */
L8c093f68: sa_motion_publish_stop(ctx,0x8c093f68u); *sr=(*sr&~1u)|(r[1]==r[5]);goto L8c093f6a; /* cmp/eq r1,r5 */
L8c093f6a: sa_motion_publish_stop(ctx,0x8c093f6au); if((*sr&1u))goto L8c093fda;goto L8c093f6c; /* bt 0x8c093fda */
L8c093f6c: sa_motion_publish_stop(ctx,0x8c093f6cu); r[2]=4u;goto L8c093f6e; /* mov #0x4,r2 */
L8c093f6e: sa_motion_publish_stop(ctx,0x8c093f6eu); r[2]<<=8;goto L8c093f70; /* shll8 r2 */
L8c093f70: sa_motion_publish_stop(ctx,0x8c093f70u); r[2]+=56u;goto L8c093f72; /* add #0x38,r2 */
L8c093f72: sa_motion_publish_stop(ctx,0x8c093f72u); *sr=(*sr&~1u)|(r[2]==r[5]);goto L8c093f74; /* cmp/eq r2,r5 */
L8c093f74: sa_motion_publish_stop(ctx,0x8c093f74u); if((*sr&1u))goto L8c093fda;goto L8c093f76; /* bt 0x8c093fda */
L8c093f76: sa_motion_publish_stop(ctx,0x8c093f76u); r[1]=3u;goto L8c093f78; /* mov #0x3,r1 */
L8c093f78: sa_motion_publish_stop(ctx,0x8c093f78u); r[1]<<=8;goto L8c093f7a; /* shll8 r1 */
L8c093f7a: sa_motion_publish_stop(ctx,0x8c093f7au); r[1]+=4294967195u;goto L8c093f7c; /* add #-0x65,r1 */
L8c093f7c: sa_motion_publish_stop(ctx,0x8c093f7cu); *sr=(*sr&~1u)|(r[1]==r[5]);goto L8c093f7e; /* cmp/eq r1,r5 */
L8c093f7e: sa_motion_publish_stop(ctx,0x8c093f7eu); if((*sr&1u))goto L8c093fda;goto L8c093f80; /* bt 0x8c093fda */
L8c093f80: sa_motion_publish_stop(ctx,0x8c093f80u); r[2]=4u;goto L8c093f82; /* mov #0x4,r2 */
L8c093f82: sa_motion_publish_stop(ctx,0x8c093f82u); r[2]<<=8;goto L8c093f84; /* shll8 r2 */
L8c093f84: sa_motion_publish_stop(ctx,0x8c093f84u); r[2]+=3u;goto L8c093f86; /* add #0x3,r2 */
L8c093f86: sa_motion_publish_stop(ctx,0x8c093f86u); *sr=(*sr&~1u)|(r[2]==r[5]);goto L8c093f88; /* cmp/eq r2,r5 */
L8c093f88: sa_motion_publish_stop(ctx,0x8c093f88u); if((*sr&1u))goto L8c093fda;goto L8c093f8a; /* bt 0x8c093fda */
L8c093f8a: sa_motion_publish_stop(ctx,0x8c093f8au); r[1]=4u;goto L8c093f8c; /* mov #0x4,r1 */
L8c093f8c: sa_motion_publish_stop(ctx,0x8c093f8cu); r[1]<<=8;goto L8c093f8e; /* shll8 r1 */
L8c093f8e: sa_motion_publish_stop(ctx,0x8c093f8eu); r[1]+=5u;goto L8c093f90; /* add #0x5,r1 */
L8c093f90: sa_motion_publish_stop(ctx,0x8c093f90u); *sr=(*sr&~1u)|(r[1]==r[5]);goto L8c093f92; /* cmp/eq r1,r5 */
L8c093f92: sa_motion_publish_stop(ctx,0x8c093f92u); if((*sr&1u))goto L8c093fda;goto L8c093f94; /* bt 0x8c093fda */
L8c093f94: sa_motion_publish_stop(ctx,0x8c093f94u); r[2]=3u;goto L8c093f96; /* mov #0x3,r2 */
L8c093f96: sa_motion_publish_stop(ctx,0x8c093f96u); r[2]<<=8;goto L8c093f98; /* shll8 r2 */
L8c093f98: sa_motion_publish_stop(ctx,0x8c093f98u); r[2]+=9u;goto L8c093f9a; /* add #0x9,r2 */
L8c093f9a: sa_motion_publish_stop(ctx,0x8c093f9au); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[2]);goto L8c093f9c; /* cmp/ge r2,r5 */
L8c093f9c: sa_motion_publish_stop(ctx,0x8c093f9cu); if(!(*sr&1u))goto L8c093fa8;goto L8c093f9e; /* bf 0x8c093fa8 */
L8c093f9e: sa_motion_publish_stop(ctx,0x8c093f9eu); r[1]=3u;goto L8c093fa0; /* mov #0x3,r1 */
L8c093fa0: sa_motion_publish_stop(ctx,0x8c093fa0u); r[1]<<=8;goto L8c093fa2; /* shll8 r1 */
L8c093fa2: sa_motion_publish_stop(ctx,0x8c093fa2u); r[1]+=13u;goto L8c093fa4; /* add #0xd,r1 */
L8c093fa4: sa_motion_publish_stop(ctx,0x8c093fa4u); *sr=(*sr&~1u)|((int32_t)r[5]>(int32_t)r[1]);goto L8c093fa6; /* cmp/gt r1,r5 */
L8c093fa6: sa_motion_publish_stop(ctx,0x8c093fa6u); if(!(*sr&1u))goto L8c093fda;goto L8c093fa8; /* bf 0x8c093fda */
L8c093fa8: sa_motion_publish_stop(ctx,0x8c093fa8u); r[2]=5u;goto L8c093faa; /* mov #0x5,r2 */
L8c093faa: sa_motion_publish_stop(ctx,0x8c093faau); r[2]<<=8;goto L8c093fac; /* shll8 r2 */
L8c093fac: sa_motion_publish_stop(ctx,0x8c093facu); r[2]+=4294967231u;goto L8c093fae; /* add #-0x41,r2 */
L8c093fae: sa_motion_publish_stop(ctx,0x8c093faeu); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[2]);goto L8c093fb0; /* cmp/ge r2,r5 */
L8c093fb0: sa_motion_publish_stop(ctx,0x8c093fb0u); if(!(*sr&1u))goto L8c093fbc;goto L8c093fb2; /* bf 0x8c093fbc */
L8c093fb2: sa_motion_publish_stop(ctx,0x8c093fb2u); r[1]=5u;goto L8c093fb4; /* mov #0x5,r1 */
L8c093fb4: sa_motion_publish_stop(ctx,0x8c093fb4u); r[1]<<=8;goto L8c093fb6; /* shll8 r1 */
L8c093fb6: sa_motion_publish_stop(ctx,0x8c093fb6u); r[1]+=4294967238u;goto L8c093fb8; /* add #-0x3a,r1 */
L8c093fb8: sa_motion_publish_stop(ctx,0x8c093fb8u); *sr=(*sr&~1u)|((int32_t)r[5]>(int32_t)r[1]);goto L8c093fba; /* cmp/gt r1,r5 */
L8c093fba: sa_motion_publish_stop(ctx,0x8c093fbau); if(!(*sr&1u))goto L8c093fda;goto L8c093fbc; /* bf 0x8c093fda */
L8c093fbc: sa_motion_publish_stop(ctx,0x8c093fbcu); r[2]=5u;goto L8c093fbe; /* mov #0x5,r2 */
L8c093fbe: sa_motion_publish_stop(ctx,0x8c093fbeu); r[2]<<=8;goto L8c093fc0; /* shll8 r2 */
L8c093fc0: sa_motion_publish_stop(ctx,0x8c093fc0u); r[2]+=4294967280u;goto L8c093fc2; /* add #-0x10,r2 */
L8c093fc2: sa_motion_publish_stop(ctx,0x8c093fc2u); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[2]);goto L8c093fc4; /* cmp/ge r2,r5 */
L8c093fc4: sa_motion_publish_stop(ctx,0x8c093fc4u); if(!(*sr&1u))goto L8c093fd0;goto L8c093fc6; /* bf 0x8c093fd0 */
L8c093fc6: sa_motion_publish_stop(ctx,0x8c093fc6u); r[1]=5u;goto L8c093fc8; /* mov #0x5,r1 */
L8c093fc8: sa_motion_publish_stop(ctx,0x8c093fc8u); r[1]<<=8;goto L8c093fca; /* shll8 r1 */
L8c093fca: sa_motion_publish_stop(ctx,0x8c093fcau); r[1]+=4294967287u;goto L8c093fcc; /* add #-0x9,r1 */
L8c093fcc: sa_motion_publish_stop(ctx,0x8c093fccu); *sr=(*sr&~1u)|((int32_t)r[5]>(int32_t)r[1]);goto L8c093fce; /* cmp/gt r1,r5 */
L8c093fce: sa_motion_publish_stop(ctx,0x8c093fceu); if(!(*sr&1u))goto L8c093fda;goto L8c093fd0; /* bf 0x8c093fda */
L8c093fd0: sa_motion_publish_stop(ctx,0x8c093fd0u); r[2]=5u;goto L8c093fd2; /* mov #0x5,r2 */
L8c093fd2: sa_motion_publish_stop(ctx,0x8c093fd2u); r[2]<<=8;goto L8c093fd4; /* shll8 r2 */
L8c093fd4: sa_motion_publish_stop(ctx,0x8c093fd4u); r[2]+=4294967289u;goto L8c093fd6; /* add #-0x7,r2 */
L8c093fd6: sa_motion_publish_stop(ctx,0x8c093fd6u); *sr=(*sr&~1u)|(r[2]==r[5]);goto L8c093fd8; /* cmp/eq r2,r5 */
L8c093fd8: sa_motion_publish_stop(ctx,0x8c093fd8u); if(!(*sr&1u))goto L8c094080;goto L8c093fda; /* bf 0x8c094080 */
L8c093fda: sa_motion_publish_stop(ctx,0x8c093fdau); r[10]=rd(m,0x8c09407cu,4);goto L8c093fdc; /* mov.l 0x8c09407c,r10 */
L8c093fdc: sa_motion_publish_stop(ctx,0x8c093fdcu); *sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c093fde; /* tst r9,r9 */
L8c093fde: sa_motion_publish_stop(ctx,0x8c093fdeu); r[12]=37u;goto L8c093fe0; /* mov #0x25,r12 */
L8c093fe0: sa_motion_publish_stop(ctx,0x8c093fe0u); r[5]=rd(m,r[10],4);goto L8c093fe2; /* mov.l @r10,r5 */
L8c093fe2: sa_motion_publish_stop(ctx,0x8c093fe4u); take=(*sr&1u);r[0]=r[12];if(take)goto L8c09401a;goto L8c093fe6; /* bt/s 0x8c09401a */
L8c093fe6: sa_motion_publish_stop(ctx,0x8c093fe6u); *sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c093fe8; /* tst r9,r9 */
L8c093fe8: sa_motion_publish_stop(ctx,0x8c093fe8u); if((*sr&1u))goto L8c09401a;goto L8c093fea; /* bt 0x8c09401a */
L8c093fea: sa_motion_publish_stop(ctx,0x8c093feau); *macl=r[6]*r[5];goto L8c093fec; /* mul.l r6,r5 */
L8c093fec: sa_motion_publish_stop(ctx,0x8c093fecu); r[2]=*macl;goto L8c093fee; /* sts MACL,r2 */
L8c093fee: sa_motion_publish_stop(ctx,0x8c093feeu); r[2]+=r[14];goto L8c093ff0; /* add r14,r2 */
L8c093ff0: sa_motion_publish_stop(ctx,0x8c093ff0u); r[3]=rd(m,r[2]+0x4u,4);goto L8c093ff2; /* mov.l @(0x4,r2),r3 */
L8c093ff2: sa_motion_publish_stop(ctx,0x8c093ff2u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c093ff4; /* tst r3,r3 */
L8c093ff4: sa_motion_publish_stop(ctx,0x8c093ff6u); take=!(*sr&1u);*macl=r[6]*r[5];if(take)goto L8c093ffa;goto L8c093ff8; /* bf/s 0x8c093ffa */
L8c093ff8: sa_motion_publish_stop(ctx,0x8c093ff8u); r[7]=r[5];goto L8c093ffa; /* mov r5,r7 */
L8c093ffa: sa_motion_publish_stop(ctx,0x8c093ffau); r[3]=*macl;goto L8c093ffc; /* sts MACL,r3 */
L8c093ffc: sa_motion_publish_stop(ctx,0x8c093ffcu); r[3]+=r[14];goto L8c093ffe; /* add r14,r3 */
L8c093ffe: sa_motion_publish_stop(ctx,0x8c093ffeu); r[2]=rd(m,r[3],4);goto L8c094000; /* mov.l @r3,r2 */
L8c094000: sa_motion_publish_stop(ctx,0x8c094000u); *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[4]);goto L8c094002; /* cmp/gt r4,r2 */
L8c094002: sa_motion_publish_stop(ctx,0x8c094002u); if((*sr&1u))goto L8c094006;goto L8c094004; /* bt 0x8c094006 */
L8c094004: sa_motion_publish_stop(ctx,0x8c094004u); r[7]=r[5];goto L8c094006; /* mov r5,r7 */
L8c094006: sa_motion_publish_stop(ctx,0x8c094006u); r[5]+=1u;goto L8c094008; /* add #0x1,r5 */
L8c094008: sa_motion_publish_stop(ctx,0x8c094008u); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[12]);goto L8c09400a; /* cmp/ge r12,r5 */
L8c09400a: sa_motion_publish_stop(ctx,0x8c09400cu); take=!(*sr&1u);r[0]+=4294967295u;if(take)goto L8c094010;goto L8c09400e; /* bf/s 0x8c094010 */
L8c09400e: sa_motion_publish_stop(ctx,0x8c09400eu); r[5]=r[13];goto L8c094010; /* mov r13,r5 */
L8c094010: sa_motion_publish_stop(ctx,0x8c094010u); *sr=(*sr&~1u)|((int32_t)r[7]>=0);goto L8c094012; /* cmp/pz r7 */
L8c094012: sa_motion_publish_stop(ctx,0x8c094012u); if((*sr&1u))goto L8c09401a;goto L8c094014; /* bt 0x8c09401a */
L8c094014: sa_motion_publish_stop(ctx,0x8c094014u); r[2]=32u;goto L8c094016; /* mov #0x20,r2 */
L8c094016: sa_motion_publish_stop(ctx,0x8c094016u); *sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[2]);goto L8c094018; /* cmp/gt r2,r0 */
L8c094018: sa_motion_publish_stop(ctx,0x8c094018u); if((*sr&1u))goto L8c093fea;goto L8c09401a; /* bt 0x8c093fea */
L8c09401a: sa_motion_publish_stop(ctx,0x8c09401au); r[1]=r[7];goto L8c09401c; /* mov r7,r1 */
L8c09401c: sa_motion_publish_stop(ctx,0x8c09401cu); r[1]+=1u;goto L8c09401e; /* add #0x1,r1 */
L8c09401e: sa_motion_publish_stop(ctx,0x8c09401eu); r[3]=r[1];goto L8c094020; /* mov r1,r3 */
L8c094020: sa_motion_publish_stop(ctx,0x8c094020u); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[12]);goto L8c094022; /* cmp/ge r12,r3 */
L8c094022: sa_motion_publish_stop(ctx,0x8c094024u); take=!(*sr&1u);wr(m,r[10],r[1],4);if(take)goto L8c0940cc;goto L8c094026; /* bf/s 0x8c0940cc */
L8c094026: sa_motion_publish_stop(ctx,0x8c094028u); wr(m,r[10],r[13],4);goto L8c0940cc; /* bra 0x8c0940cc */
L8c094080: sa_motion_publish_stop(ctx,0x8c094080u); r[10]=rd(m,0x8c094220u,4);goto L8c094082; /* mov.l 0x8c094220,r10 */
L8c094082: sa_motion_publish_stop(ctx,0x8c094082u); *sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c094084; /* tst r9,r9 */
L8c094084: sa_motion_publish_stop(ctx,0x8c094084u); r[0]=r[13];goto L8c094086; /* mov r13,r0 */
L8c094086: sa_motion_publish_stop(ctx,0x8c094086u); r[5]=rd(m,r[10],4);goto L8c094088; /* mov.l @r10,r5 */
L8c094088: sa_motion_publish_stop(ctx,0x8c09408au); take=(*sr&1u);r[12]=0u;if(take)goto L8c0940be;goto L8c09408c; /* bt/s 0x8c0940be */
L8c09408c: sa_motion_publish_stop(ctx,0x8c09408cu); *sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c09408e; /* tst r9,r9 */
L8c09408e: sa_motion_publish_stop(ctx,0x8c09408eu); if((*sr&1u))goto L8c0940be;goto L8c094090; /* bt 0x8c0940be */
L8c094090: sa_motion_publish_stop(ctx,0x8c094090u); *macl=r[6]*r[5];goto L8c094092; /* mul.l r6,r5 */
L8c094092: sa_motion_publish_stop(ctx,0x8c094092u); r[2]=*macl;goto L8c094094; /* sts MACL,r2 */
L8c094094: sa_motion_publish_stop(ctx,0x8c094094u); r[2]+=r[14];goto L8c094096; /* add r14,r2 */
L8c094096: sa_motion_publish_stop(ctx,0x8c094096u); r[3]=rd(m,r[2]+0x4u,4);goto L8c094098; /* mov.l @(0x4,r2),r3 */
L8c094098: sa_motion_publish_stop(ctx,0x8c094098u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c09409a; /* tst r3,r3 */
L8c09409a: sa_motion_publish_stop(ctx,0x8c09409cu); take=!(*sr&1u);*macl=r[6]*r[5];if(take)goto L8c0940a0;goto L8c09409e; /* bf/s 0x8c0940a0 */
L8c09409e: sa_motion_publish_stop(ctx,0x8c09409eu); r[7]=r[5];goto L8c0940a0; /* mov r5,r7 */
L8c0940a0: sa_motion_publish_stop(ctx,0x8c0940a0u); r[3]=*macl;goto L8c0940a2; /* sts MACL,r3 */
L8c0940a2: sa_motion_publish_stop(ctx,0x8c0940a2u); r[3]+=r[14];goto L8c0940a4; /* add r14,r3 */
L8c0940a4: sa_motion_publish_stop(ctx,0x8c0940a4u); r[2]=rd(m,r[3],4);goto L8c0940a6; /* mov.l @r3,r2 */
L8c0940a6: sa_motion_publish_stop(ctx,0x8c0940a6u); *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[4]);goto L8c0940a8; /* cmp/gt r4,r2 */
L8c0940a8: sa_motion_publish_stop(ctx,0x8c0940a8u); if((*sr&1u))goto L8c0940ac;goto L8c0940aa; /* bt 0x8c0940ac */
L8c0940aa: sa_motion_publish_stop(ctx,0x8c0940aau); r[7]=r[5];goto L8c0940ac; /* mov r5,r7 */
L8c0940ac: sa_motion_publish_stop(ctx,0x8c0940acu); r[5]+=1u;goto L8c0940ae; /* add #0x1,r5 */
L8c0940ae: sa_motion_publish_stop(ctx,0x8c0940aeu); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[13]);goto L8c0940b0; /* cmp/ge r13,r5 */
L8c0940b0: sa_motion_publish_stop(ctx,0x8c0940b2u); take=!(*sr&1u);r[0]+=4294967295u;if(take)goto L8c0940b6;goto L8c0940b4; /* bf/s 0x8c0940b6 */
L8c0940b4: sa_motion_publish_stop(ctx,0x8c0940b4u); r[5]=r[12];goto L8c0940b6; /* mov r12,r5 */
L8c0940b6: sa_motion_publish_stop(ctx,0x8c0940b6u); *sr=(*sr&~1u)|((int32_t)r[7]>=0);goto L8c0940b8; /* cmp/pz r7 */
L8c0940b8: sa_motion_publish_stop(ctx,0x8c0940b8u); if((*sr&1u))goto L8c0940be;goto L8c0940ba; /* bt 0x8c0940be */
L8c0940ba: sa_motion_publish_stop(ctx,0x8c0940bau); *sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c0940bc; /* cmp/pl r0 */
L8c0940bc: sa_motion_publish_stop(ctx,0x8c0940bcu); if((*sr&1u))goto L8c094090;goto L8c0940be; /* bt 0x8c094090 */
L8c0940be: sa_motion_publish_stop(ctx,0x8c0940beu); r[3]=r[7];goto L8c0940c0; /* mov r7,r3 */
L8c0940c0: sa_motion_publish_stop(ctx,0x8c0940c0u); r[3]+=1u;goto L8c0940c2; /* add #0x1,r3 */
L8c0940c2: sa_motion_publish_stop(ctx,0x8c0940c2u); r[2]=r[3];goto L8c0940c4; /* mov r3,r2 */
L8c0940c4: sa_motion_publish_stop(ctx,0x8c0940c4u); *sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[13]);goto L8c0940c6; /* cmp/ge r13,r2 */
L8c0940c6: sa_motion_publish_stop(ctx,0x8c0940c6u); wr(m,r[10],r[3],4);goto L8c0940c8; /* mov.l r3,@r10 */
L8c0940c8: sa_motion_publish_stop(ctx,0x8c0940c8u); if(!(*sr&1u))goto L8c0940cc;goto L8c0940ca; /* bf 0x8c0940cc */
L8c0940ca: sa_motion_publish_stop(ctx,0x8c0940cau); wr(m,r[10],r[12],4);goto L8c0940cc; /* mov.l r12,@r10 */
L8c0940cc: sa_motion_publish_stop(ctx,0x8c0940ccu); r[9]=rd(m,r[15],4);r[15]+=4;goto L8c0940ce; /* mov.l @r15+,r9 */
L8c0940ce: sa_motion_publish_stop(ctx,0x8c0940ceu); r[0]=r[7];goto L8c0940d0; /* mov r7,r0 */
L8c0940d0: sa_motion_publish_stop(ctx,0x8c0940d0u); r[10]=rd(m,r[15],4);r[15]+=4;goto L8c0940d2; /* mov.l @r15+,r10 */
L8c0940d2: sa_motion_publish_stop(ctx,0x8c0940d2u); r[12]=rd(m,r[15],4);r[15]+=4;goto L8c0940d4; /* mov.l @r15+,r12 */
L8c0940d4: sa_motion_publish_stop(ctx,0x8c0940d4u); r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0940d6; /* mov.l @r15+,r13 */
L8c0940d6: sa_motion_publish_stop(ctx,0x8c0940d8u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_sound_request_8c0940da(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0940da;
L8c0940da: sa_motion_publish_stop(ctx,0x8c0940dau); r[15]-=4;wr(m,r[15],*pr,4);goto L8c0940dc; /* sts.l PR,@-r15 */
L8c0940dc: sa_motion_publish_stop(ctx,0x8c0940dcu); r[15]+=4294967280u;goto L8c0940de; /* add #-0x10,r15 */
L8c0940de: sa_motion_publish_stop(ctx,0x8c0940deu); wr(m,r[15],r[4],4);goto L8c0940e0; /* mov.l r4,@r15 */
L8c0940e0: sa_motion_publish_stop(ctx,0x8c0940e0u); wr(m,r[15]+0x8u,r[5],4);goto L8c0940e2; /* mov.l r5,@(0x8,r15) */
L8c0940e2: sa_motion_publish_stop(ctx,0x8c0940e2u); wr(m,r[15]+0xcu,r[6],4);goto L8c0940e4; /* mov.l r6,@(0xc,r15) */
L8c0940e4: sa_motion_publish_stop(ctx,0x8c0940e4u); wr(m,r[15]+0x4u,r[7],4);goto L8c0940e6; /* mov.l r7,@(0x4,r15) */
L8c0940e6: sa_motion_publish_stop(ctx,0x8c0940e6u); r[5]=rd(m,r[15],4);goto L8c0940e8; /* mov.l @r15,r5 */
L8c0940e8: sa_motion_publish_stop(ctx,0x8c0940eau); target=0x8c093f02u;*pr=0x8c0940ecu;r[4]=rd(m,r[15]+0xcu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0940ec; /* bsr 0x8c093f02 */
L8c0940ec: sa_motion_publish_stop(ctx,0x8c0940ecu); r[5]=r[0];goto L8c0940ee; /* mov r0,r5 */
L8c0940ee: sa_motion_publish_stop(ctx,0x8c0940eeu); *sr=(*sr&~1u)|((int32_t)r[5]>=0);goto L8c0940f0; /* cmp/pz r5 */
L8c0940f0: sa_motion_publish_stop(ctx,0x8c0940f0u); if((*sr&1u))goto L8c0940fa;goto L8c0940f2; /* bt 0x8c0940fa */
L8c0940f2: sa_motion_publish_stop(ctx,0x8c0940f2u); r[15]+=16u;goto L8c0940f4; /* add #0x10,r15 */
L8c0940f4: sa_motion_publish_stop(ctx,0x8c0940f4u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c0940f6; /* lds.l @r15+,PR */
L8c0940f6: sa_motion_publish_stop(ctx,0x8c0940f8u); r[0]=4294967295u;return 0; /* rts */
L8c0940fa: sa_motion_publish_stop(ctx,0x8c0940fau); r[4]=52u;goto L8c0940fc; /* mov #0x34,r4 */
L8c0940fc: sa_motion_publish_stop(ctx,0x8c0940fcu); r[3]=rd(m,0x8c094224u,4);goto L8c0940fe; /* mov.l 0x8c094224,r3 */
L8c0940fe: sa_motion_publish_stop(ctx,0x8c0940feu); *macl=r[4]*r[5];goto L8c094100; /* mul.l r4,r5 */
L8c094100: sa_motion_publish_stop(ctx,0x8c094100u); r[2]=120u;goto L8c094102; /* mov #0x78,r2 */
L8c094102: sa_motion_publish_stop(ctx,0x8c094102u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c094104; /* fldi0 fr4 */
L8c094104: sa_motion_publish_stop(ctx,0x8c094104u); r[0]=36u;goto L8c094106; /* mov #0x24,r0 */
L8c094106: sa_motion_publish_stop(ctx,0x8c094106u); r[1]=4294967295u;goto L8c094108; /* mov #-0x1,r1 */
L8c094108: sa_motion_publish_stop(ctx,0x8c094108u); r[4]=*macl;goto L8c09410a; /* sts MACL,r4 */
L8c09410a: sa_motion_publish_stop(ctx,0x8c09410au); r[5]=0u;goto L8c09410c; /* mov #0x0,r5 */
L8c09410c: sa_motion_publish_stop(ctx,0x8c09410cu); r[4]+=r[3];goto L8c09410e; /* add r3,r4 */
L8c09410e: sa_motion_publish_stop(ctx,0x8c09410eu); wr(m,r[4]+0x4u,r[2],4);goto L8c094110; /* mov.l r2,@(0x4,r4) */
L8c094110: sa_motion_publish_stop(ctx,0x8c094110u); wr(m,r[4],r[1],4);goto L8c094112; /* mov.l r1,@r4 */
L8c094112: sa_motion_publish_stop(ctx,0x8c094112u); r[1]=17u;goto L8c094114; /* mov #0x11,r1 */
L8c094114: sa_motion_publish_stop(ctx,0x8c094114u); r[2]=rd(m,r[15]+0x8u,4);goto L8c094116; /* mov.l @(0x8,r15),r2 */
L8c094116: sa_motion_publish_stop(ctx,0x8c094116u); wr(m,r[4]+0x8u,r[2],4);goto L8c094118; /* mov.l r2,@(0x8,r4) */
L8c094118: sa_motion_publish_stop(ctx,0x8c094118u); wr(m,r[4]+0xcu,r[1],4);goto L8c09411a; /* mov.l r1,@(0xc,r4) */
L8c09411a: sa_motion_publish_stop(ctx,0x8c09411au); r[2]=rd(m,r[15],4);goto L8c09411c; /* mov.l @r15,r2 */
L8c09411c: sa_motion_publish_stop(ctx,0x8c09411cu); wr(m,r[4]+0x10u,r[2],4);goto L8c09411e; /* mov.l r2,@(0x10,r4) */
L8c09411e: sa_motion_publish_stop(ctx,0x8c09411eu); wr(m,r[4]+0x14u,r[5],4);goto L8c094120; /* mov.l r5,@(0x14,r4) */
L8c094120: sa_motion_publish_stop(ctx,0x8c094120u); r[2]=rd(m,r[15]+0x4u,4);goto L8c094122; /* mov.l @(0x4,r15),r2 */
L8c094122: sa_motion_publish_stop(ctx,0x8c094122u); wr(m,r[4]+0x18u,r[2],4);goto L8c094124; /* mov.l r2,@(0x18,r4) */
L8c094124: sa_motion_publish_stop(ctx,0x8c094124u); wr(m,r[4]+0x1cu,r[5],4);goto L8c094126; /* mov.l r5,@(0x1c,r4) */
L8c094126: sa_motion_publish_stop(ctx,0x8c094126u); wr(m,r[4]+0x20u,r[5],4);goto L8c094128; /* mov.l r5,@(0x20,r4) */
L8c094128: sa_motion_publish_stop(ctx,0x8c094128u); fstore(m,f,4u,r[0]+r[4]);goto L8c09412a; /* fmov fr4,@(r0,r4) */
L8c09412a: sa_motion_publish_stop(ctx,0x8c09412au); r[0]=40u;goto L8c09412c; /* mov #0x28,r0 */
L8c09412c: sa_motion_publish_stop(ctx,0x8c09412cu); fstore(m,f,4u,r[0]+r[4]);goto L8c09412e; /* fmov fr4,@(r0,r4) */
L8c09412e: sa_motion_publish_stop(ctx,0x8c09412eu); r[0]=44u;goto L8c094130; /* mov #0x2c,r0 */
L8c094130: sa_motion_publish_stop(ctx,0x8c094130u); fstore(m,f,4u,r[0]+r[4]);goto L8c094132; /* fmov fr4,@(r0,r4) */
L8c094132: sa_motion_publish_stop(ctx,0x8c094132u); r[15]+=16u;goto L8c094134; /* add #0x10,r15 */
L8c094134: sa_motion_publish_stop(ctx,0x8c094134u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c094136; /* lds.l @r15+,PR */
L8c094136: sa_motion_publish_stop(ctx,0x8c094138u); return 0; /* rts */
}
