/* Original USA world/land producer entries; names are source-informed.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_add_sub.h"
#include "sh4_vector_numeric.h"
#include "sh4_fmac.h"
#include "sh4_divide.h"
#include "sh4_multiply_extended.h"
#include "sh4_float_i32.h"
#include "sh4_compare_bits.h"
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
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
static void fcopy(struct sa_fpu_banks *f,unsigned n,unsigned s){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){unsigned nb=bank^(n&1u),sb=bank^(s&1u);n&=~1u;s&=~1u;
  uint32_t a=f->bank[sb][s],b=f->bank[sb][s+1];f->bank[nb][n]=a;f->bank[nb][n+1]=b;
 }else f->bank[bank][n]=f->bank[bank][s];
}
static int valid_ram(uint32_t a,unsigned n){
 return (a&0x1f000000u)==0x0c000000u && (a&0xffffffu)<=0x1000000u-n;
}
SA_STAGE_EXPORT int sa_world_draw_static_8c051e56(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c051e56;
L8c051e56: *stop=0x8c051e56u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c051e58; /* mov.l r14,@-r15 */
L8c051e58: *stop=0x8c051e58u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c051e5a; /* mov.l r13,@-r15 */
L8c051e5a: *stop=0x8c051e5au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c051e5c; /* mov.l r12,@-r15 */
L8c051e5c: *stop=0x8c051e5cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c051e5e; /* mov.l r11,@-r15 */
L8c051e5e: *stop=0x8c051e5eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c051e60; /* mov.l r10,@-r15 */
L8c051e60: *stop=0x8c051e60u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c051e62; /* mov.l r9,@-r15 */
L8c051e62: *stop=0x8c051e62u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c051e64; /* mov.l r8,@-r15 */
L8c051e64: *stop=0x8c051e64u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c051e66; /* sts.l PR,@-r15 */
L8c051e66: *stop=0x8c051e66u;r[15]+=4294967292u;goto L8c051e68; /* add #-0x4,r15 */
L8c051e68: *stop=0x8c051e68u;r[3]=rd(m,0x8c051fdcu,4);goto L8c051e6a; /* mov.l 0x8c051fdc,r3 */
L8c051e6a: target=r[3];*pr=0x8c051e6eu;*stop=0x8c051e6cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051e6e; /* jsr @r3 */
L8c051e6e: *stop=0x8c051e6eu;r[2]=rd(m,0x8c051fe0u,4);goto L8c051e70; /* mov.l 0x8c051fe0,r2 */
L8c051e70: target=r[2];*pr=0x8c051e74u;*stop=0x8c051e72u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051e74; /* jsr @r2 */
L8c051e74: *stop=0x8c051e74u;r[3]=rd(m,0x8c051ff0u,4);goto L8c051e76; /* mov.l 0x8c051ff0,r3 */
L8c051e76: *stop=0x8c051e76u;r[14]=2u;goto L8c051e78; /* mov #0x2,r14 */
L8c051e78: *stop=0x8c051e78u;r[14]<<=8;goto L8c051e7a; /* shll8 r14 */
L8c051e7a: *stop=0x8c051e7au;r[9]=rd(m,0x8c051fe8u,4);goto L8c051e7c; /* mov.l 0x8c051fe8,r9 */
L8c051e7c: *stop=0x8c051e7cu;if(!valid_ram(r[3],4u))return -3;r[13]=rd(m,r[3],4);goto L8c051e7e; /* mov.l @r3,r13 */
L8c051e7e: *stop=0x8c051e7eu;r[14]+=4294967189u;goto L8c051e80; /* add #-0x6b,r14 */
L8c051e80: *stop=0x8c051e80u;r[8]=rd(m,0x8c051fe4u,4);goto L8c051e82; /* mov.l 0x8c051fe4,r8 */
L8c051e82: *stop=0x8c051e82u;if(!valid_ram(r[13]+4u,2u))return -3;r[0]=sx(m,r[13]+0x4u,2);goto L8c051e84; /* mov.w @(0x4,r13),r0 */
L8c051e84: *stop=0x8c051e84u;r[10]=rd(m,0x8c051fecu,4);goto L8c051e86; /* mov.l 0x8c051fec,r10 */
L8c051e86: *stop=0x8c051e86u;*sr=(*sr&~1u)|!(r[0]&0xau);goto L8c051e88; /* tst #0xa,r0 */
L8c051e88: if((*sr&1u))goto L8c051eee;goto L8c051e8a; /* bt 0x8c051eee */
L8c051e8a: *stop=0x8c051e8au;r[1]=rd(m,0x8c051ff4u,4);goto L8c051e8c; /* mov.l 0x8c051ff4,r1 */
L8c051e8c: target=r[1];*pr=0x8c051e90u;*stop=0x8c051e8eu;if(!valid_ram(r[13]+24u,4u))return -3;r[4]=rd(m,r[13]+0x18u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051e90; /* jsr @r1 */
L8c051e90: *stop=0x8c051e90u;r[3]=rd(m,0x8c051ff8u,4);goto L8c051e92; /* mov.l 0x8c051ff8,r3 */
L8c051e92: *stop=0x8c051e92u;r[2]=rd(m,0x8c051ffcu,4);goto L8c051e94; /* mov.l 0x8c051ffc,r2 */
L8c051e94: *stop=0x8c051e94u;if(!valid_ram(r[3],2u))return -3;r[12]=sx(m,r[3],2);goto L8c051e96; /* mov.w @r3,r12 */
L8c051e96: *stop=0x8c051e96u;r[12]+=4294967295u;goto L8c051e98; /* add #-0x1,r12 */
L8c051e98: *stop=0x8c051e98u;r[1]=(uint32_t)(int32_t)(int16_t)r[12];goto L8c051e9a; /* exts.w r12,r1 */
L8c051e9a: *stop=0x8c051e9au;r[13]=(uint32_t)(int32_t)(int16_t)r[12];goto L8c051e9c; /* exts.w r12,r13 */
L8c051e9c: *stop=0x8c051e9cu;*sr=(*sr&~1u)|((int32_t)r[1]>=0);goto L8c051e9e; /* cmp/pz r1 */
L8c051e9e: *stop=0x8c051e9eu;r[13]<<=2;goto L8c051ea0; /* shll2 r13 */
L8c051ea0: take=!(*sr&1u);*stop=0x8c051ea2u;r[13]+=r[2];if(take)goto L8c051f48;goto L8c051ea4; /* bf/s 0x8c051f48 */
L8c051ea4: *stop=0x8c051ea4u;if(!valid_ram(r[13],4u))return -3;r[4]=rd(m,r[13],4);goto L8c051ea6; /* mov.l @r13,r4 */
L8c051ea6: *stop=0x8c051ea6u;r[0]=8u;goto L8c051ea8; /* mov #0x8,r0 */
L8c051ea8: *stop=0x8c051ea8u;if(!valid_ram(r[4]+24u,4u))return -3;r[11]=rd(m,r[4]+0x18u,4);goto L8c051eaa; /* mov.l @(0x18,r4),r11 */
L8c051eaa: *stop=0x8c051eaau;r[3]=r[11];goto L8c051eac; /* mov r11,r3 */
L8c051eac: *stop=0x8c051eacu;r[3]+=8u;goto L8c051eae; /* add #0x8,r3 */
L8c051eae: *stop=0x8c051eaeu;r[2]=r[3];goto L8c051eb0; /* mov r3,r2 */
L8c051eb0: *stop=0x8c051eb0u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto L8c051eb2; /* mov.l r3,@r15 */
L8c051eb2: *stop=0x8c051eb2u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[3]);goto L8c051eb4; /* fmov.s @(r0,r3),fr6 */
L8c051eb4: *stop=0x8c051eb4u;r[0]=4u;goto L8c051eb6; /* mov #0x4,r0 */
L8c051eb6: *stop=0x8c051eb6u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[2]);goto L8c051eb8; /* fmov.s @r2,fr4 */
L8c051eb8: *stop=0x8c051eb8u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[2]);goto L8c051eba; /* fmov.s @(r0,r2),fr5 */
L8c051eba: target=r[10];*pr=0x8c051ebeu;*stop=0x8c051ebcu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051ebe; /* jsr @r10 */
L8c051ebe: *stop=0x8c051ebeu;r[7]=r[11];goto L8c051ec0; /* mov r11,r7 */
L8c051ec0: *stop=0x8c051ec0u;r[7]+=20u;goto L8c051ec2; /* add #0x14,r7 */
L8c051ec2: *stop=0x8c051ec2u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[7],4);goto L8c051ec4; /* mov.l r7,@r15 */
L8c051ec4: *stop=0x8c051ec4u;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto L8c051ec6; /* mov.l @r15,r3 */
L8c051ec6: *stop=0x8c051ec6u;if(!valid_ram(r[7]+8u,4u))return -3;r[7]=rd(m,r[7]+0x8u,4);goto L8c051ec8; /* mov.l @(0x8,r7),r7 */
L8c051ec8: *stop=0x8c051ec8u;if(!valid_ram(r[3],4u))return -3;r[5]=rd(m,r[3],4);goto L8c051eca; /* mov.l @r3,r5 */
L8c051eca: *stop=0x8c051ecau;if(!valid_ram(r[3]+4u,4u))return -3;r[6]=rd(m,r[3]+0x4u,4);goto L8c051ecc; /* mov.l @(0x4,r3),r6 */
L8c051ecc: target=r[8];*pr=0x8c051ed0u;*stop=0x8c051eceu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051ed0; /* jsr @r8 */
L8c051ed0: *stop=0x8c051ed0u;if(!valid_ram(r[11]+4u,4u))return -3;r[4]=rd(m,r[11]+0x4u,4);goto L8c051ed2; /* mov.l @(0x4,r11),r4 */
L8c051ed2: *stop=0x8c051ed2u;*sr=(*sr&~1u)|(r[14]==r[4]);goto L8c051ed4; /* cmp/eq r14,r4 */
L8c051ed4: if((*sr&1u))goto L8c051edc;goto L8c051ed6; /* bt 0x8c051edc */
L8c051ed6: *stop=0x8c051ed6u;r[2]=rd(m,0x8c052000u,4);goto L8c051ed8; /* mov.l 0x8c052000,r2 */
L8c051ed8: target=r[2];*pr=0x8c051edcu;*stop=0x8c051edau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051edc; /* jsr @r2 */
L8c051edc: target=r[9];*pr=0x8c051ee0u;*stop=0x8c051edeu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051ee0; /* jsr @r9 */
L8c051ee0: *stop=0x8c051ee0u;r[12]+=4294967295u;goto L8c051ee2; /* add #-0x1,r12 */
L8c051ee2: *stop=0x8c051ee2u;r[3]=(uint32_t)(int32_t)(int16_t)r[12];goto L8c051ee4; /* exts.w r12,r3 */
L8c051ee4: *stop=0x8c051ee4u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c051ee6; /* cmp/pz r3 */
L8c051ee6: take=(*sr&1u);*stop=0x8c051ee8u;r[13]+=4294967292u;if(take)goto L8c051ea4;goto L8c051eea; /* bt/s 0x8c051ea4 */
L8c051eea: *stop=0x8c051eecu;goto L8c051f48; /* bra 0x8c051f48 */
L8c051eee: *stop=0x8c051eeeu;r[2]=rd(m,0x8c051ff8u,4);goto L8c051ef0; /* mov.l 0x8c051ff8,r2 */
L8c051ef0: *stop=0x8c051ef0u;r[1]=rd(m,0x8c051ffcu,4);goto L8c051ef2; /* mov.l 0x8c051ffc,r1 */
L8c051ef2: *stop=0x8c051ef2u;if(!valid_ram(r[2],2u))return -3;r[13]=sx(m,r[2],2);goto L8c051ef4; /* mov.w @r2,r13 */
L8c051ef4: *stop=0x8c051ef4u;r[13]+=4294967295u;goto L8c051ef6; /* add #-0x1,r13 */
L8c051ef6: *stop=0x8c051ef6u;r[3]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c051ef8; /* exts.w r13,r3 */
L8c051ef8: *stop=0x8c051ef8u;r[11]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c051efa; /* exts.w r13,r11 */
L8c051efa: *stop=0x8c051efau;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c051efc; /* cmp/pz r3 */
L8c051efc: *stop=0x8c051efcu;r[11]<<=2;goto L8c051efe; /* shll2 r11 */
L8c051efe: take=!(*sr&1u);*stop=0x8c051f00u;r[11]+=r[1];if(take)goto L8c051f48;goto L8c051f02; /* bf/s 0x8c051f48 */
L8c051f02: *stop=0x8c051f02u;if(!valid_ram(r[11],4u))return -3;r[4]=rd(m,r[11],4);goto L8c051f04; /* mov.l @r11,r4 */
L8c051f04: *stop=0x8c051f04u;r[0]=8u;goto L8c051f06; /* mov #0x8,r0 */
L8c051f06: *stop=0x8c051f06u;if(!valid_ram(r[4]+24u,4u))return -3;r[12]=rd(m,r[4]+0x18u,4);goto L8c051f08; /* mov.l @(0x18,r4),r12 */
L8c051f08: *stop=0x8c051f08u;r[3]=r[12];goto L8c051f0a; /* mov r12,r3 */
L8c051f0a: *stop=0x8c051f0au;r[3]+=8u;goto L8c051f0c; /* add #0x8,r3 */
L8c051f0c: *stop=0x8c051f0cu;r[2]=r[3];goto L8c051f0e; /* mov r3,r2 */
L8c051f0e: *stop=0x8c051f0eu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto L8c051f10; /* mov.l r3,@r15 */
L8c051f10: *stop=0x8c051f10u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[3]);goto L8c051f12; /* fmov.s @(r0,r3),fr6 */
L8c051f12: *stop=0x8c051f12u;r[0]=4u;goto L8c051f14; /* mov #0x4,r0 */
L8c051f14: *stop=0x8c051f14u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[2]);goto L8c051f16; /* fmov.s @r2,fr4 */
L8c051f16: *stop=0x8c051f16u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[2]);goto L8c051f18; /* fmov.s @(r0,r2),fr5 */
L8c051f18: target=r[10];*pr=0x8c051f1cu;*stop=0x8c051f1au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051f1c; /* jsr @r10 */
L8c051f1c: *stop=0x8c051f1cu;r[7]=r[12];goto L8c051f1e; /* mov r12,r7 */
L8c051f1e: *stop=0x8c051f1eu;r[7]+=20u;goto L8c051f20; /* add #0x14,r7 */
L8c051f20: *stop=0x8c051f20u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[7],4);goto L8c051f22; /* mov.l r7,@r15 */
L8c051f22: *stop=0x8c051f22u;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto L8c051f24; /* mov.l @r15,r3 */
L8c051f24: *stop=0x8c051f24u;if(!valid_ram(r[7]+8u,4u))return -3;r[7]=rd(m,r[7]+0x8u,4);goto L8c051f26; /* mov.l @(0x8,r7),r7 */
L8c051f26: *stop=0x8c051f26u;if(!valid_ram(r[3],4u))return -3;r[5]=rd(m,r[3],4);goto L8c051f28; /* mov.l @r3,r5 */
L8c051f28: *stop=0x8c051f28u;if(!valid_ram(r[3]+4u,4u))return -3;r[6]=rd(m,r[3]+0x4u,4);goto L8c051f2a; /* mov.l @(0x4,r3),r6 */
L8c051f2a: target=r[8];*pr=0x8c051f2eu;*stop=0x8c051f2cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051f2e; /* jsr @r8 */
L8c051f2e: *stop=0x8c051f2eu;if(!valid_ram(r[12]+4u,4u))return -3;r[4]=rd(m,r[12]+0x4u,4);goto L8c051f30; /* mov.l @(0x4,r12),r4 */
L8c051f30: *stop=0x8c051f30u;*sr=(*sr&~1u)|(r[14]==r[4]);goto L8c051f32; /* cmp/eq r14,r4 */
L8c051f32: if((*sr&1u))goto L8c051f3a;goto L8c051f34; /* bt 0x8c051f3a */
L8c051f34: *stop=0x8c051f34u;r[2]=rd(m,0x8c052000u,4);goto L8c051f36; /* mov.l 0x8c052000,r2 */
L8c051f36: target=r[2];*pr=0x8c051f3au;*stop=0x8c051f38u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051f3a; /* jsr @r2 */
L8c051f3a: target=r[9];*pr=0x8c051f3eu;*stop=0x8c051f3cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051f3e; /* jsr @r9 */
L8c051f3e: *stop=0x8c051f3eu;r[13]+=4294967295u;goto L8c051f40; /* add #-0x1,r13 */
L8c051f40: *stop=0x8c051f40u;r[3]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c051f42; /* exts.w r13,r3 */
L8c051f42: *stop=0x8c051f42u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c051f44; /* cmp/pz r3 */
L8c051f44: take=(*sr&1u);*stop=0x8c051f46u;r[11]+=4294967292u;if(take)goto L8c051f02;goto L8c051f48; /* bt/s 0x8c051f02 */
L8c051f48: *stop=0x8c051f48u;r[3]=rd(m,0x8c052004u,4);goto L8c051f4a; /* mov.l 0x8c052004,r3 */
L8c051f4a: target=r[3];*pr=0x8c051f4eu;*stop=0x8c051f4cu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051f4e; /* jsr @r3 */
L8c051f4e: *stop=0x8c051f4eu;r[15]+=4u;goto L8c051f50; /* add #0x4,r15 */
L8c051f50: *stop=0x8c051f50u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c051f52; /* lds.l @r15+,PR */
L8c051f52: *stop=0x8c051f52u;r[2]=rd(m,0x8c052008u,4);goto L8c051f54; /* mov.l 0x8c052008,r2 */
L8c051f54: *stop=0x8c051f54u;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c051f56; /* mov.l @r15+,r8 */
L8c051f56: *stop=0x8c051f56u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c051f58; /* mov.l @r15+,r9 */
L8c051f58: *stop=0x8c051f58u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c051f5a; /* mov.l @r15+,r10 */
L8c051f5a: *stop=0x8c051f5au;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c051f5c; /* mov.l @r15+,r11 */
L8c051f5c: *stop=0x8c051f5cu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c051f5e; /* mov.l @r15+,r12 */
L8c051f5e: *stop=0x8c051f5eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c051f60; /* mov.l @r15+,r13 */
L8c051f60: target=r[2];*stop=0x8c051f62u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_world_draw_motion_8c051f64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c051f64;
L8c051f64: *stop=0x8c051f64u;r[3]=rd(m,0x8c051ff0u,4);goto L8c051f66; /* mov.l 0x8c051ff0,r3 */
L8c051f66: *stop=0x8c051f66u;r[2]=1u;goto L8c051f68; /* mov #0x1,r2 */
L8c051f68: *stop=0x8c051f68u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c051f6a; /* mov.l r14,@-r15 */
L8c051f6a: *stop=0x8c051f6au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c051f6c; /* mov.l r13,@-r15 */
L8c051f6c: *stop=0x8c051f6cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c051f6e; /* mov.l r12,@-r15 */
L8c051f6e: *stop=0x8c051f6eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c051f70; /* mov.l r11,@-r15 */
L8c051f70: *stop=0x8c051f70u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c051f72; /* fmov.s fr15,@-r15 */
L8c051f72: *stop=0x8c051f72u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c051f74; /* sts.l PR,@-r15 */
L8c051f74: *stop=0x8c051f74u;if(!valid_ram(r[3],4u))return -3;r[5]=rd(m,r[3],4);goto L8c051f76; /* mov.l @r3,r5 */
L8c051f76: *stop=0x8c051f76u;if(!valid_ram(r[5]+4u,2u))return -3;r[0]=sx(m,r[5]+0x4u,2);goto L8c051f78; /* mov.w @(0x4,r5),r0 */
L8c051f78: *stop=0x8c051f78u;r[4]=r[0];goto L8c051f7a; /* mov r0,r4 */
L8c051f7a: *stop=0x8c051f7au;*sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c051f7c; /* tst r4,r2 */
L8c051f7c: if((*sr&1u))goto L8c05203a;goto L8c051f7e; /* bt 0x8c05203a */
L8c051f7e: *stop=0x8c051f7eu;r[12]=rd(m,0x8c051ff4u,4);goto L8c051f80; /* mov.l 0x8c051ff4,r12 */
L8c051f80: *stop=0x8c051f80u;r[3]=16u;goto L8c051f82; /* mov #0x10,r3 */
L8c051f82: *stop=0x8c051f82u;if(!valid_ram(r[5]+16u,4u))return -3;r[14]=rd(m,r[5]+0x10u,4);goto L8c051f84; /* mov.l @(0x10,r5),r14 */
L8c051f84: *stop=0x8c051f84u;*sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c051f86; /* tst r3,r4 */
L8c051f86: *stop=0x8c051f86u;if(!valid_ram(r[5]+2u,2u))return -3;r[0]=sx(m,r[5]+0x2u,2);goto L8c051f88; /* mov.w @(0x2,r5),r0 */
L8c051f88: take=(*sr&1u);*stop=0x8c051f8au;r[13]=r[0];if(take)goto L8c052020;goto L8c051f8c; /* bt/s 0x8c052020 */
L8c051f8c: *stop=0x8c051f8cu;r[11]=rd(m,0x8c05200cu,4);goto L8c051f8e; /* mov.l 0x8c05200c,r11 */
L8c051f8e: *stop=0x8c051f8eu;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c051f90; /* cmp/pl r13 */
L8c051f90: if(!(*sr&1u))goto L8c05203a;goto L8c051f92; /* bf 0x8c05203a */
L8c051f92: *stop=0x8c051f92u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[14]);goto L8c051f94; /* fmov.s @r14,fr15 */
L8c051f94: target=r[12];*pr=0x8c051f98u;*stop=0x8c051f96u;if(!valid_ram(r[14]+20u,4u))return -3;r[4]=rd(m,r[14]+0x14u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051f98; /* jsr @r12 */
L8c051f98: *stop=0x8c051f98u;if(!valid_ram(r[14]+16u,4u))return -3;r[5]=rd(m,r[14]+0x10u,4);goto L8c051f9a; /* mov.l @(0x10,r14),r5 */
L8c051f9a: *stop=0x8c051f9au;fcopy(f,4u,15u);goto L8c051f9c; /* fmov fr15,fr4 */
L8c051f9c: target=r[11];*pr=0x8c051fa0u;*stop=0x8c051f9eu;if(!valid_ram(r[14]+12u,4u))return -3;r[4]=rd(m,r[14]+0xcu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c051fa0; /* jsr @r11 */
L8c051fa0: *stop=0x8c051fa0u;r[13]+=4294967295u;goto L8c051fa2; /* add #-0x1,r13 */
L8c051fa2: *stop=0x8c051fa2u;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c051fa4; /* cmp/pl r13 */
L8c051fa4: take=(*sr&1u);*stop=0x8c051fa6u;r[14]+=24u;if(take)goto L8c051f92;goto L8c051fa8; /* bt/s 0x8c051f92 */
L8c051fa8: *stop=0x8c051faau;goto L8c05203a; /* bra 0x8c05203a */
L8c052020: *stop=0x8c052020u;r[11]=rd(m,0x8c052100u,4);goto L8c052022; /* mov.l 0x8c052100,r11 */
L8c052022: *stop=0x8c052022u;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c052024; /* cmp/pl r13 */
L8c052024: if(!(*sr&1u))goto L8c05203a;goto L8c052026; /* bf 0x8c05203a */
L8c052026: *stop=0x8c052026u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[14]);goto L8c052028; /* fmov.s @r14,fr15 */
L8c052028: target=r[12];*pr=0x8c05202cu;*stop=0x8c05202au;if(!valid_ram(r[14]+20u,4u))return -3;r[4]=rd(m,r[14]+0x14u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05202c; /* jsr @r12 */
L8c05202c: *stop=0x8c05202cu;fcopy(f,4u,15u);goto L8c05202e; /* fmov fr15,fr4 */
L8c05202e: target=r[11];*pr=0x8c052032u;*stop=0x8c052030u;if(!valid_ram(r[14]+16u,4u))return -3;r[4]=rd(m,r[14]+0x10u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052032; /* jsr @r11 */
L8c052032: *stop=0x8c052032u;r[13]+=4294967295u;goto L8c052034; /* add #-0x1,r13 */
L8c052034: *stop=0x8c052034u;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c052036; /* cmp/pl r13 */
L8c052036: take=(*sr&1u);*stop=0x8c052038u;r[14]+=24u;if(take)goto L8c052026;goto L8c05203a; /* bt/s 0x8c052026 */
L8c05203a: *stop=0x8c05203au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c05203c; /* lds.l @r15+,PR */
L8c05203c: *stop=0x8c05203cu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c05203e; /* fmov.s @r15+,fr15 */
L8c05203e: *stop=0x8c05203eu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c052040; /* mov.l @r15+,r11 */
L8c052040: *stop=0x8c052040u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c052042; /* mov.l @r15+,r12 */
L8c052042: *stop=0x8c052042u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c052044; /* mov.l @r15+,r13 */
L8c052044: *stop=0x8c052044u;if(*pr!=entry_pr)return -3;*stop=0x8c052046u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_world_draw_added_8c052048(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c052048;
L8c052048: *stop=0x8c052048u;r[3]=rd(m,0x8c052104u,4);goto L8c05204a; /* mov.l 0x8c052104,r3 */
L8c05204a: *stop=0x8c05204au;r[2]=1u;goto L8c05204c; /* mov #0x1,r2 */
L8c05204c: *stop=0x8c05204cu;if(!valid_ram(r[3],4u))return -3;r[5]=rd(m,r[3],4);goto L8c05204e; /* mov.l @r3,r5 */
L8c05204e: *stop=0x8c05204eu;if(!valid_ram(r[5]+4u,2u))return -3;r[0]=sx(m,r[5]+0x4u,2);goto L8c052050; /* mov.w @(0x4,r5),r0 */
L8c052050: *stop=0x8c052050u;r[6]=r[0];goto L8c052052; /* mov r0,r6 */
L8c052052: *stop=0x8c052052u;*sr=(*sr&~1u)|!(r[6]&r[2]);goto L8c052054; /* tst r6,r2 */
L8c052054: if((*sr&1u))goto L8c0520b4;goto L8c052056; /* bt 0x8c0520b4 */
L8c052056: *stop=0x8c052056u;if(!valid_ram(r[5]+2u,2u))return -3;r[0]=sx(m,r[5]+0x2u,2);goto L8c052058; /* mov.w @(0x2,r5),r0 */
L8c052058: *stop=0x8c052058u;r[3]=16u;goto L8c05205a; /* mov #0x10,r3 */
L8c05205a: *stop=0x8c05205au;if(!valid_ram(r[5]+16u,4u))return -3;r[4]=rd(m,r[5]+0x10u,4);goto L8c05205c; /* mov.l @(0x10,r5),r4 */
L8c05205c: *stop=0x8c05205cu;*sr=(*sr&~1u)|!(r[3]&r[6]);goto L8c05205e; /* tst r3,r6 */
L8c05205e: *stop=0x8c05205eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c052060; /* fldi1 fr5 */
L8c052060: *stop=0x8c052060u;r[5]=r[0];goto L8c052062; /* mov r0,r5 */
L8c052062: *stop=0x8c052062u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0;goto L8c052064; /* fldi0 fr6 */
L8c052064: take=(*sr&1u);*stop=0x8c052066u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]^=0x80000000u;if(take)goto L8c052090;goto L8c052068; /* bt/s 0x8c052090 */
L8c052068: *stop=0x8c052068u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c05206a; /* cmp/pl r5 */
L8c05206a: if(!(*sr&1u))goto L8c0520b4;goto L8c05206c; /* bf 0x8c0520b4 */
L8c05206c: *stop=0x8c05206cu;r[0]=4u;goto L8c05206e; /* mov #0x4,r0 */
L8c05206e: *stop=0x8c05206eu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);goto L8c052070; /* fmov.s @r4,fr4 */
L8c052070: *stop=0x8c052070u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[0]+r[4]);goto L8c052072; /* fmov.s @(r0,r4),fr7 */
L8c052072: *stop=0x8c052072u;r[0]=8u;goto L8c052074; /* mov #0x8,r0 */
L8c052074: *stop=0x8c052074u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c052076; /* fmov.s @(r0,r4),fr3 */
L8c052076: *stop=0x8c052076u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[7],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c052078; /* fadd fr7,fr4 */
L8c052078: *stop=0x8c052078u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c05207a; /* fsub fr7,fr3 */
L8c05207a: *stop=0x8c05207au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c05207c; /* fadd fr5,fr3 */
L8c05207c: *stop=0x8c05207cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c05207e; /* fcmp/gt fr4,fr3 */
L8c05207e: if((*sr&1u))goto L8c052082;goto L8c052080; /* bt 0x8c052082 */
L8c052080: *stop=0x8c052080u;fcopy(f,4u,6u);goto L8c052082; /* fmov fr6,fr4 */
L8c052082: *stop=0x8c052082u;r[5]+=4294967295u;goto L8c052084; /* add #-0x1,r5 */
L8c052084: *stop=0x8c052084u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[4]);goto L8c052086; /* fmov.s fr4,@r4 */
L8c052086: *stop=0x8c052086u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c052088; /* cmp/pl r5 */
L8c052088: take=(*sr&1u);*stop=0x8c05208au;r[4]+=24u;if(take)goto L8c05206c;goto L8c05208c; /* bt/s 0x8c05206c */
L8c05208c: *stop=0x8c05208cu;if(*pr!=entry_pr)return -3;*stop=0x8c05208eu;return 0; /* rts */
L8c052090: *stop=0x8c052090u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c052092; /* cmp/pl r5 */
L8c052092: if(!(*sr&1u))goto L8c0520b4;goto L8c052094; /* bf 0x8c0520b4 */
L8c052094: *stop=0x8c052094u;r[0]=4u;goto L8c052096; /* mov #0x4,r0 */
L8c052096: *stop=0x8c052096u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);goto L8c052098; /* fmov.s @r4,fr4 */
L8c052098: *stop=0x8c052098u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[0]+r[4]);goto L8c05209a; /* fmov.s @(r0,r4),fr7 */
L8c05209a: *stop=0x8c05209au;r[0]=8u;goto L8c05209c; /* mov #0x8,r0 */
L8c05209c: *stop=0x8c05209cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c05209e; /* fmov.s @(r0,r4),fr3 */
L8c05209e: *stop=0x8c05209eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[7],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c0520a0; /* fadd fr7,fr4 */
L8c0520a0: *stop=0x8c0520a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c0520a2; /* fsub fr7,fr3 */
L8c0520a2: *stop=0x8c0520a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c0520a4; /* fadd fr5,fr3 */
L8c0520a4: *stop=0x8c0520a4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c0520a6; /* fcmp/gt fr4,fr3 */
L8c0520a6: if((*sr&1u))goto L8c0520aa;goto L8c0520a8; /* bt 0x8c0520aa */
L8c0520a8: *stop=0x8c0520a8u;fcopy(f,4u,6u);goto L8c0520aa; /* fmov fr6,fr4 */
L8c0520aa: *stop=0x8c0520aau;r[5]+=4294967295u;goto L8c0520ac; /* add #-0x1,r5 */
L8c0520ac: *stop=0x8c0520acu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[4]);goto L8c0520ae; /* fmov.s fr4,@r4 */
L8c0520ae: *stop=0x8c0520aeu;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c0520b0; /* cmp/pl r5 */
L8c0520b0: take=(*sr&1u);*stop=0x8c0520b2u;r[4]+=24u;if(take)goto L8c052094;goto L8c0520b4; /* bt/s 0x8c052094 */
L8c0520b4: *stop=0x8c0520b4u;if(*pr!=entry_pr)return -3;*stop=0x8c0520b6u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_world_prepare_draw_8c0520c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c0520c8;
L8c0520c8: *stop=0x8c0520c8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0520ca; /* mov.l r14,@-r15 */
L8c0520ca: *stop=0x8c0520cau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c0520cc; /* mov.l r13,@-r15 */
L8c0520cc: *stop=0x8c0520ccu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c0520ce; /* mov.l r12,@-r15 */
L8c0520ce: *stop=0x8c0520ceu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c0520d0; /* mov.l r11,@-r15 */
L8c0520d0: *stop=0x8c0520d0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c0520d2; /* mov.l r10,@-r15 */
L8c0520d2: *stop=0x8c0520d2u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c0520d4; /* mov.l r9,@-r15 */
L8c0520d4: *stop=0x8c0520d4u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c0520d6; /* fmov.s fr15,@-r15 */
L8c0520d6: *stop=0x8c0520d6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c0520d8; /* sts.l PR,@-r15 */
L8c0520d8: *stop=0x8c0520d8u;r[15]+=4294967280u;goto L8c0520da; /* add #-0x10,r15 */
L8c0520da: *stop=0x8c0520dau;r[12]=rd(m,0x8c052104u,4);goto L8c0520dc; /* mov.l 0x8c052104,r12 */
L8c0520dc: *stop=0x8c0520dcu;r[3]=4294967295u;goto L8c0520de; /* mov #-0x1,r3 */
L8c0520de: *stop=0x8c0520deu;r[2]=rd(m,0x8c05210cu,4);goto L8c0520e0; /* mov.l 0x8c05210c,r2 */
L8c0520e0: *stop=0x8c0520e0u;if(!valid_ram(r[12],4u))return -3;r[14]=rd(m,r[12],4);goto L8c0520e2; /* mov.l @r12,r14 */
L8c0520e2: *stop=0x8c0520e2u;r[13]=rd(m,0x8c052110u,4);goto L8c0520e4; /* mov.l 0x8c052110,r13 */
L8c0520e4: *stop=0x8c0520e4u;if(!valid_ram(r[14]+12u,4u))return -3;r[14]=rd(m,r[14]+0xcu,4);goto L8c0520e6; /* mov.l @(0xc,r14),r14 */
L8c0520e6: *stop=0x8c0520e6u;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[3],2);goto L8c0520e8; /* mov.w r3,@r2 */
L8c0520e8: *stop=0x8c0520e8u;if(!valid_ram(r[12],4u))return -3;r[0]=rd(m,r[12],4);goto L8c0520ea; /* mov.l @r12,r0 */
L8c0520ea: *stop=0x8c0520eau;if(!valid_ram(r[0]+4u,2u))return -3;r[0]=sx(m,r[0]+0x4u,2);goto L8c0520ec; /* mov.w @(0x4,r0),r0 */
L8c0520ec: *stop=0x8c0520ecu;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c0520ee; /* tst #0x4,r0 */
L8c0520ee: take=(*sr&1u);*stop=0x8c0520f0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;if(take)goto L8c052120;goto L8c0520f2; /* bt/s 0x8c052120 */
L8c0520f2: *stop=0x8c0520f2u;if(!valid_ram(r[12],4u))return -3;r[1]=rd(m,r[12],4);goto L8c0520f4; /* mov.l @r12,r1 */
L8c0520f4: *stop=0x8c0520f4u;r[0]=8u;goto L8c0520f6; /* mov #0x8,r0 */
L8c0520f6: *stop=0x8c0520f8u;if(!valid_ram(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[0]+r[1]);goto L8c052122; /* bra 0x8c052122 */
L8c052120: *stop=0x8c052120u;if(!valid_ram(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[13]);goto L8c052122; /* fmov.s @r13,fr15 */
L8c052122: *stop=0x8c052122u;r[0]=8u;goto L8c052124; /* mov #0x8,r0 */
L8c052124: *stop=0x8c052124u;r[4]=rd(m,0x8c052284u,4);goto L8c052126; /* mov.l 0x8c052284,r4 */
L8c052126: *stop=0x8c052126u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c052128; /* fmov fr3,@(r0,r15) */
L8c052128: *stop=0x8c052128u;r[0]=4u;goto L8c05212a; /* mov #0x4,r0 */
L8c05212a: *stop=0x8c05212au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c05212c; /* fmov fr3,@(r0,r15) */
L8c05212c: *stop=0x8c05212cu;r[0]=0x8c052280u;goto L8c05212e; /* mova 0x8c052280,r0 */
L8c05212e: *stop=0x8c05212eu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);goto L8c052130; /* fmov.s @r0,fr4 */
L8c052130: *stop=0x8c052130u;r[0]=12u;goto L8c052132; /* mov #0xc,r0 */
L8c052132: *stop=0x8c052132u;fcopy(f,3u,15u);goto L8c052134; /* fmov fr15,fr3 */
L8c052134: *stop=0x8c052134u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c052136; /* fmul fr4,fr3 */
L8c052136: *stop=0x8c052136u;r[3]=rd(m,0x8c052288u,4);goto L8c052138; /* mov.l 0x8c052288,r3 */
L8c052138: *stop=0x8c052138u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c05213a; /* fneg fr3 */
L8c05213a: *stop=0x8c05213au;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c05213c; /* fmov fr3,@(r0,r15) */
L8c05213c: target=r[3];*pr=0x8c052140u;*stop=0x8c05213eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[4],&f->fpscr,&fr[15]);if(status)return status;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052140; /* jsr @r3 */
L8c052140: *stop=0x8c052140u;r[11]=rd(m,0x8c05228cu,4);goto L8c052142; /* mov.l 0x8c05228c,r11 */
L8c052142: *stop=0x8c052142u;if(!valid_ram(r[11],4u))return -3;r[2]=rd(m,r[11],4);goto L8c052144; /* mov.l @r11,r2 */
L8c052144: *stop=0x8c052144u;if(!valid_ram(r[2]+24u,4u))return -3;r[3]=rd(m,r[2]+0x18u,4);goto L8c052146; /* mov.l @(0x18,r2),r3 */
L8c052146: *stop=0x8c052146u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c052148; /* tst r3,r3 */
L8c052148: take=(*sr&1u);*stop=0x8c05214au;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);if(take)goto L8c052154;goto L8c05214c; /* bt/s 0x8c052154 */
L8c05214c: *stop=0x8c05214cu;r[3]=rd(m,0x8c052290u,4);goto L8c05214e; /* mov.l 0x8c052290,r3 */
L8c05214e: *stop=0x8c05214eu;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c052150; /* mov.l @r15,r5 */
L8c052150: target=r[3];*pr=0x8c052154u;*stop=0x8c052152u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052154; /* jsr @r3 */
L8c052154: *stop=0x8c052154u;if(!valid_ram(r[11],4u))return -3;r[3]=rd(m,r[11],4);goto L8c052156; /* mov.l @r11,r3 */
L8c052156: *stop=0x8c052156u;if(!valid_ram(r[3]+20u,4u))return -3;r[2]=rd(m,r[3]+0x14u,4);goto L8c052158; /* mov.l @(0x14,r3),r2 */
L8c052158: *stop=0x8c052158u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c05215a; /* tst r2,r2 */
L8c05215a: take=(*sr&1u);*stop=0x8c05215cu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[2],4);if(take)goto L8c052166;goto L8c05215e; /* bt/s 0x8c052166 */
L8c05215e: *stop=0x8c05215eu;r[3]=rd(m,0x8c052294u,4);goto L8c052160; /* mov.l 0x8c052294,r3 */
L8c052160: *stop=0x8c052160u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c052162; /* mov.l @r15,r5 */
L8c052162: target=r[3];*pr=0x8c052166u;*stop=0x8c052164u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052166; /* jsr @r3 */
L8c052166: *stop=0x8c052166u;r[5]=r[15];goto L8c052168; /* mov r15,r5 */
L8c052168: *stop=0x8c052168u;r[3]=rd(m,0x8c052298u,4);goto L8c05216a; /* mov.l 0x8c052298,r3 */
L8c05216a: *stop=0x8c05216au;r[6]=r[15];goto L8c05216c; /* mov r15,r6 */
L8c05216c: *stop=0x8c05216cu;r[5]+=4u;goto L8c05216e; /* add #0x4,r5 */
L8c05216e: *stop=0x8c05216eu;r[6]+=4u;goto L8c052170; /* add #0x4,r6 */
L8c052170: target=r[3];*pr=0x8c052174u;*stop=0x8c052172u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052174; /* jsr @r3 */
L8c052174: *stop=0x8c052174u;r[2]=rd(m,0x8c05229cu,4);goto L8c052176; /* mov.l 0x8c05229c,r2 */
L8c052176: target=r[2];*pr=0x8c05217au;*stop=0x8c052178u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05217a; /* jsr @r2 */
L8c05217a: *stop=0x8c05217au;r[0]=4u;goto L8c05217c; /* mov #0x4,r0 */
L8c05217c: *stop=0x8c05217cu;if(!valid_ram(r[11],4u))return -3;r[3]=rd(m,r[11],4);goto L8c05217e; /* mov.l @r11,r3 */
L8c05217e: *stop=0x8c05217eu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c052180; /* fmov.s @(r0,r15),fr3 */
L8c052180: *stop=0x8c052180u;r[0]=32u;goto L8c052182; /* mov #0x20,r0 */
L8c052182: *stop=0x8c052182u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[0]+r[3]);goto L8c052184; /* fmov.s @(r0,r3),fr8 */
L8c052184: *stop=0x8c052184u;r[0]=8u;goto L8c052186; /* mov #0x8,r0 */
L8c052186: *stop=0x8c052186u;if(!valid_ram(r[12],4u))return -3;r[5]=rd(m,r[12],4);goto L8c052188; /* mov.l @r12,r5 */
L8c052188: *stop=0x8c052188u;r[11]=4u;goto L8c05218a; /* mov #0x4,r11 */
L8c05218a: *stop=0x8c05218au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c05218c; /* fadd fr3,fr8 */
L8c05218c: *stop=0x8c05218cu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c05218e; /* fmov.s @(r0,r15),fr3 */
L8c05218e: *stop=0x8c05218eu;r[0]=36u;goto L8c052190; /* mov #0x24,r0 */
L8c052190: *stop=0x8c052190u;if(!valid_ram(r[5],2u))return -3;r[5]=sx(m,r[5],2);goto L8c052192; /* mov.w @r5,r5 */
L8c052192: *stop=0x8c052192u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,10u,r[0]+r[3]);goto L8c052194; /* fmov.s @(r0,r3),fr10 */
L8c052194: *stop=0x8c052194u;r[0]=12u;goto L8c052196; /* mov #0xc,r0 */
L8c052196: *stop=0x8c052196u;r[10]=rd(m,0x8c0522a4u,4);goto L8c052198; /* mov.l 0x8c0522a4,r10 */
L8c052198: *stop=0x8c052198u;r[11]<<=8;goto L8c05219a; /* shll8 r11 */
L8c05219a: *stop=0x8c05219au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[10],&fr[3],&f->fpscr,&fr[10],0);if(status)return status;}goto L8c05219c; /* fadd fr3,fr10 */
L8c05219c: *stop=0x8c05219cu;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c05219e; /* fmov.s @(r0,r15),fr3 */
L8c05219e: *stop=0x8c05219eu;r[0]=40u;goto L8c0521a0; /* mov #0x28,r0 */
L8c0521a0: *stop=0x8c0521a0u;r[9]=rd(m,0x8c0522a0u,4);goto L8c0521a2; /* mov.l 0x8c0522a0,r9 */
L8c0521a2: *stop=0x8c0521a2u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[0]+r[3]);goto L8c0521a4; /* fmov.s @(r0,r3),fr9 */
L8c0521a4: *stop=0x8c0521a4u;r[4]=0u;goto L8c0521a6; /* mov #0x0,r4 */
L8c0521a6: *stop=0x8c0521a6u;r[6]=rd(m,0x8c0522acu,4);goto L8c0521a8; /* mov.l 0x8c0522ac,r6 */
L8c0521a8: *stop=0x8c0521a8u;r[5]+=4294967295u;goto L8c0521aa; /* add #-0x1,r5 */
L8c0521aa: *stop=0x8c0521aau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[3],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c0521ac; /* fadd fr3,fr9 */
L8c0521ac: *stop=0x8c0521acu;r[12]=rd(m,0x8c0522a8u,4);goto L8c0521ae; /* mov.l 0x8c0522a8,r12 */
L8c0521ae: *stop=0x8c0521b0u;r[7]=32u;goto L8c052246; /* bra 0x8c052246 */
L8c0521b2: *stop=0x8c0521b2u;if(!valid_ram(r[14]+28u,4u))return -3;r[2]=rd(m,r[14]+0x1cu,4);goto L8c0521b4; /* mov.l @(0x1c,r14),r2 */
L8c0521b4: *stop=0x8c0521b4u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c0521b6; /* tst r2,r2 */
L8c0521b6: if((*sr&1u))goto L8c0521c0;goto L8c0521b8; /* bt 0x8c0521c0 */
L8c0521b8: *stop=0x8c0521b8u;if(!valid_ram(r[14]+28u,4u))return -3;r[2]=rd(m,r[14]+0x1cu,4);goto L8c0521ba; /* mov.l @(0x1c,r14),r2 */
L8c0521ba: *stop=0x8c0521bau;if(!valid_ram(r[9],4u))return -3;r[3]=rd(m,r[9],4);goto L8c0521bc; /* mov.l @r9,r3 */
L8c0521bc: *stop=0x8c0521bcu;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c0521be; /* tst r3,r2 */
L8c0521be: if((*sr&1u))goto L8c052242;goto L8c0521c0; /* bt 0x8c052242 */
L8c0521c0: *stop=0x8c0521c0u;if(!valid_ram(r[14]+32u,4u))return -3;r[2]=rd(m,r[14]+0x20u,4);goto L8c0521c2; /* mov.l @(0x20,r14),r2 */
L8c0521c2: *stop=0x8c0521c2u;*sr=(*sr&~1u)|!(r[12]&r[2]);goto L8c0521c4; /* tst r12,r2 */
L8c0521c4: if((*sr&1u))goto L8c052242;goto L8c0521c6; /* bt 0x8c052242 */
L8c0521c6: *stop=0x8c0521c6u;if(!valid_ram(r[14]+32u,4u))return -3;r[1]=rd(m,r[14]+0x20u,4);goto L8c0521c8; /* mov.l @(0x20,r14),r1 */
L8c0521c8: *stop=0x8c0521c8u;*sr=(*sr&~1u)|!(r[7]&r[1]);goto L8c0521ca; /* tst r7,r1 */
L8c0521ca: if(!(*sr&1u))goto L8c052202;goto L8c0521cc; /* bf 0x8c052202 */
L8c0521cc: *stop=0x8c0521ccu;r[0]=12u;goto L8c0521ce; /* mov #0xc,r0 */
L8c0521ce: *stop=0x8c0521ceu;fcopy(f,4u,15u);goto L8c0521d0; /* fmov fr15,fr4 */
L8c0521d0: *stop=0x8c0521d0u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c0521d2; /* fmov.s @(r0,r14),fr3 */
L8c0521d2: *stop=0x8c0521d2u;r[0]=4u;goto L8c0521d4; /* mov #0x4,r0 */
L8c0521d4: *stop=0x8c0521d4u;fcopy(f,6u,8u);goto L8c0521d6; /* fmov fr8,fr6 */
L8c0521d6: *stop=0x8c0521d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c0521d8; /* fadd fr3,fr4 */
L8c0521d8: *stop=0x8c0521d8u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[14]);goto L8c0521da; /* fmov.s @r14,fr3 */
L8c0521da: *stop=0x8c0521dau;fcopy(f,7u,10u);goto L8c0521dc; /* fmov fr10,fr7 */
L8c0521dc: *stop=0x8c0521dcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[3],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c0521de; /* fsub fr3,fr6 */
L8c0521de: *stop=0x8c0521deu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c0521e0; /* fmov.s @(r0,r14),fr3 */
L8c0521e0: *stop=0x8c0521e0u;r[0]=8u;goto L8c0521e2; /* mov #0x8,r0 */
L8c0521e2: *stop=0x8c0521e2u;fcopy(f,5u,9u);goto L8c0521e4; /* fmov fr9,fr5 */
L8c0521e4: *stop=0x8c0521e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[3],&f->fpscr,&fr[7],1);if(status)return status;}goto L8c0521e6; /* fsub fr3,fr7 */
L8c0521e6: *stop=0x8c0521e6u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c0521e8; /* fmov.s @(r0,r14),fr3 */
L8c0521e8: *stop=0x8c0521e8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c0521ea; /* fmul fr4,fr4 */
L8c0521ea: *stop=0x8c0521eau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c0521ec; /* fsub fr3,fr5 */
L8c0521ec: *stop=0x8c0521ecu;fcopy(f,0u,6u);goto L8c0521ee; /* fmov fr6,fr0 */
L8c0521ee: *stop=0x8c0521eeu;fcopy(f,3u,7u);goto L8c0521f0; /* fmov fr7,fr3 */
L8c0521f0: *stop=0x8c0521f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0521f2; /* fmul fr7,fr3 */
L8c0521f2: *stop=0x8c0521f2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[6],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0521f4; /* fmac fr0,fr6,fr3 */
L8c0521f4: *stop=0x8c0521f4u;fcopy(f,0u,5u);goto L8c0521f6; /* fmov fr5,fr0 */
L8c0521f6: *stop=0x8c0521f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[5],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0521f8; /* fmac fr0,fr5,fr3 */
L8c0521f8: *stop=0x8c0521f8u;fcopy(f,5u,3u);goto L8c0521fa; /* fmov fr3,fr5 */
L8c0521fa: *stop=0x8c0521fau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c0521fc; /* fcmp/gt fr4,fr5 */
L8c0521fc: if(!(*sr&1u))goto L8c052234;goto L8c0521fe; /* bf 0x8c052234 */
L8c0521fe: *stop=0x8c052200u;goto L8c052242; /* bra 0x8c052242 */
L8c052202: *stop=0x8c052202u;r[0]=12u;goto L8c052204; /* mov #0xc,r0 */
L8c052204: *stop=0x8c052204u;if(!valid_ram(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[13]);goto L8c052206; /* fmov.s @r13,fr4 */
L8c052206: *stop=0x8c052206u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c052208; /* fmov.s @(r0,r14),fr3 */
L8c052208: *stop=0x8c052208u;r[0]=4u;goto L8c05220a; /* mov #0x4,r0 */
L8c05220a: *stop=0x8c05220au;fcopy(f,7u,8u);goto L8c05220c; /* fmov fr8,fr7 */
L8c05220c: *stop=0x8c05220cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c05220e; /* fadd fr3,fr4 */
L8c05220e: *stop=0x8c05220eu;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[14]);goto L8c052210; /* fmov.s @r14,fr3 */
L8c052210: *stop=0x8c052210u;fcopy(f,6u,10u);goto L8c052212; /* fmov fr10,fr6 */
L8c052212: *stop=0x8c052212u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[3],&f->fpscr,&fr[7],1);if(status)return status;}goto L8c052214; /* fsub fr3,fr7 */
L8c052214: *stop=0x8c052214u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c052216; /* fmov.s @(r0,r14),fr3 */
L8c052216: *stop=0x8c052216u;r[0]=8u;goto L8c052218; /* mov #0x8,r0 */
L8c052218: *stop=0x8c052218u;fcopy(f,5u,9u);goto L8c05221a; /* fmov fr9,fr5 */
L8c05221a: *stop=0x8c05221au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[3],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c05221c; /* fsub fr3,fr6 */
L8c05221c: *stop=0x8c05221cu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[14]);goto L8c05221e; /* fmov.s @(r0,r14),fr3 */
L8c05221e: *stop=0x8c05221eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c052220; /* fmul fr4,fr4 */
L8c052220: *stop=0x8c052220u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c052222; /* fsub fr3,fr5 */
L8c052222: *stop=0x8c052222u;fcopy(f,0u,7u);goto L8c052224; /* fmov fr7,fr0 */
L8c052224: *stop=0x8c052224u;fcopy(f,3u,6u);goto L8c052226; /* fmov fr6,fr3 */
L8c052226: *stop=0x8c052226u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c052228; /* fmul fr6,fr3 */
L8c052228: *stop=0x8c052228u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[7],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c05222a; /* fmac fr0,fr7,fr3 */
L8c05222a: *stop=0x8c05222au;fcopy(f,0u,5u);goto L8c05222c; /* fmov fr5,fr0 */
L8c05222c: *stop=0x8c05222cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[5],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c05222e; /* fmac fr0,fr5,fr3 */
L8c05222e: *stop=0x8c05222eu;fcopy(f,5u,3u);goto L8c052230; /* fmov fr3,fr5 */
L8c052230: *stop=0x8c052230u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c052232; /* fcmp/gt fr4,fr5 */
L8c052232: if((*sr&1u))goto L8c052242;goto L8c052234; /* bt 0x8c052242 */
L8c052234: *stop=0x8c052234u;r[0]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c052236; /* exts.w r4,r0 */
L8c052236: *stop=0x8c052236u;r[4]+=1u;goto L8c052238; /* add #0x1,r4 */
L8c052238: *stop=0x8c052238u;r[3]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c05223a; /* exts.w r4,r3 */
L8c05223a: *stop=0x8c05223au;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[11]);goto L8c05223c; /* cmp/ge r11,r3 */
L8c05223c: *stop=0x8c05223cu;r[0]<<=2;goto L8c05223e; /* shll2 r0 */
L8c05223e: take=(*sr&1u);*stop=0x8c052240u;if(!valid_ram(r[0]+r[10],4u))return -3;wr(m,r[10]+r[0],r[14],4);if(take)goto L8c05224c;goto L8c052242; /* bt/s 0x8c05224c */
L8c052242: *stop=0x8c052242u;r[5]+=4294967295u;goto L8c052244; /* add #-0x1,r5 */
L8c052244: *stop=0x8c052244u;r[14]+=36u;goto L8c052246; /* add #0x24,r14 */
L8c052246: *stop=0x8c052246u;r[3]=(uint32_t)(int32_t)(int16_t)r[5];goto L8c052248; /* exts.w r5,r3 */
L8c052248: *stop=0x8c052248u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c05224a; /* cmp/pz r3 */
L8c05224a: if((*sr&1u))goto L8c0521b2;goto L8c05224c; /* bt 0x8c0521b2 */
L8c05224c: *stop=0x8c05224cu;if(!valid_ram(r[6],2u))return -3;wr(m,r[6],r[4],2);goto L8c05224e; /* mov.w r4,@r6 */
L8c05224e: *stop=0x8c05224eu;r[15]+=16u;goto L8c052250; /* add #0x10,r15 */
L8c052250: *stop=0x8c052250u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c052252; /* lds.l @r15+,PR */
L8c052252: *stop=0x8c052252u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c052254; /* fmov.s @r15+,fr15 */
L8c052254: *stop=0x8c052254u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c052256; /* mov.l @r15+,r9 */
L8c052256: *stop=0x8c052256u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c052258; /* mov.l @r15+,r10 */
L8c052258: *stop=0x8c052258u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c05225a; /* mov.l @r15+,r11 */
L8c05225a: *stop=0x8c05225au;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c05225c; /* mov.l @r15+,r12 */
L8c05225c: *stop=0x8c05225cu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c05225e; /* mov.l @r15+,r13 */
L8c05225e: *stop=0x8c05225eu;if(*pr!=entry_pr)return -3;*stop=0x8c052260u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_world_pool_clear_8c052a00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c052a00;
L8c052a00: *stop=0x8c052a00u;r[6]=rd(m,0x8c052b60u,4);goto L8c052a02; /* mov.l 0x8c052b60,r6 */
L8c052a02: *stop=0x8c052a02u;r[4]=4294967295u;goto L8c052a04; /* mov #-0x1,r4 */
L8c052a04: *stop=0x8c052a04u;r[4]=r[4]&255u;goto L8c052a06; /* extu.b r4,r4 */
L8c052a06: *stop=0x8c052a06u;r[6]+=r[4];goto L8c052a08; /* add r4,r6 */
L8c052a08: *stop=0x8c052a08u;r[5]=r[4];goto L8c052a0a; /* mov r4,r5 */
L8c052a0a: *stop=0x8c052a0au;r[4]=0u;goto L8c052a0c; /* mov #0x0,r4 */
L8c052a0c: *stop=0x8c052a0cu;r[6]+=1u;goto L8c052a0e; /* add #0x1,r6 */
L8c052a0e: *stop=0x8c052a0eu;r[5]+=4294967294u;goto L8c052a10; /* add #-0x2,r5 */
L8c052a10: *stop=0x8c052a10u;if(!valid_ram(r[6]-1u,1u))return -3;r[6]-=1;wr(m,r[6],r[4],1);goto L8c052a12; /* mov.b r4,@-r6 */
L8c052a12: *stop=0x8c052a12u;r[3]=(uint32_t)(int32_t)(int16_t)r[5];goto L8c052a14; /* exts.w r5,r3 */
L8c052a14: *stop=0x8c052a14u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c052a16; /* cmp/pz r3 */
L8c052a16: take=(*sr&1u);*stop=0x8c052a18u;if(!valid_ram(r[6]-1u,1u))return -3;r[6]-=1;wr(m,r[6],r[4],1);if(take)goto L8c052a0e;goto L8c052a1a; /* bt/s 0x8c052a0e */
L8c052a1a: *stop=0x8c052a1au;if(*pr!=entry_pr)return -3;*stop=0x8c052a1cu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_world_original_zero_hook_8c094dc4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c094dc4;
L8c094dc4: *stop=0x8c094dc4u;if(*pr!=entry_pr)return -3;*stop=0x8c094dc6u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_world_load_allocate_8c09d9c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c09d9c6;
L8c09d9c6: *stop=0x8c09d9c6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c09d9c8; /* mov.l r14,@-r15 */
L8c09d9c8: *stop=0x8c09d9c8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c09d9ca; /* sts.l PR,@-r15 */
L8c09d9ca: *stop=0x8c09d9cau;r[15]+=4294967288u;goto L8c09d9cc; /* add #-0x8,r15 */
L8c09d9cc: target=0x8c09da74u;*pr=0x8c09d9d0u;*stop=0x8c09d9ceu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[4],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d9d0; /* bsr 0x8c09da74 */
L8c09d9d0: *stop=0x8c09d9d0u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c09d9d2; /* cmp/eq #-0x1,r0 */
L8c09d9d2: take=(*sr&1u);*stop=0x8c09d9d4u;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[0],4);if(take)goto L8c09d9f4;goto L8c09d9d6; /* bt/s 0x8c09d9f4 */
L8c09d9d6: *stop=0x8c09d9d6u;r[3]=rd(m,0x8c09da38u,4);goto L8c09d9d8; /* mov.l 0x8c09da38,r3 */
L8c09d9d8: target=r[3];*pr=0x8c09d9dcu;*stop=0x8c09d9dau;if(!valid_ram(r[15]+4u,4u))return -3;r[4]=rd(m,r[15]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d9dc; /* jsr @r3 */
L8c09d9dc: *stop=0x8c09d9dcu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c09d9de; /* tst r0,r0 */
L8c09d9de: take=(*sr&1u);*stop=0x8c09d9e0u;r[14]=r[0];if(take)goto L8c09d9f4;goto L8c09d9e2; /* bt/s 0x8c09d9f4 */
L8c09d9e2: *stop=0x8c09d9e2u;if(!valid_ram(r[15],4u))return -3;r[4]=rd(m,r[15],4);goto L8c09d9e4; /* mov.l @r15,r4 */
L8c09d9e4: target=0x8c09cc80u;*pr=0x8c09d9e8u;*stop=0x8c09d9e6u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d9e8; /* bsr 0x8c09cc80 */
L8c09d9e8: *stop=0x8c09d9e8u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c09d9ea; /* cmp/eq #-0x1,r0 */
L8c09d9ea: take=!(*sr&1u);*stop=0x8c09d9ecu;r[5]=r[0];if(take)goto L8c09d9fe;goto L8c09d9ee; /* bf/s 0x8c09d9fe */
L8c09d9ee: *stop=0x8c09d9eeu;r[2]=rd(m,0x8c09da3cu,4);goto L8c09d9f0; /* mov.l 0x8c09da3c,r2 */
L8c09d9f0: target=r[2];*pr=0x8c09d9f4u;*stop=0x8c09d9f2u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09d9f4; /* jsr @r2 */
L8c09d9f4: *stop=0x8c09d9f4u;r[0]=0u;goto L8c09d9f6; /* mov #0x0,r0 */
L8c09d9f6: *stop=0x8c09d9f6u;r[15]+=8u;goto L8c09d9f8; /* add #0x8,r15 */
L8c09d9f8: *stop=0x8c09d9f8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c09d9fa; /* lds.l @r15+,PR */
L8c09d9fa: *stop=0x8c09d9fau;if(*pr!=entry_pr)return -3;*stop=0x8c09d9fcu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c09d9fe: *stop=0x8c09d9feu;r[0]=r[14];goto L8c09da00; /* mov r14,r0 */
L8c09da00: *stop=0x8c09da00u;r[15]+=8u;goto L8c09da02; /* add #0x8,r15 */
L8c09da02: *stop=0x8c09da02u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c09da04; /* lds.l @r15+,PR */
L8c09da04: *stop=0x8c09da04u;if(*pr!=entry_pr)return -3;*stop=0x8c09da06u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_world_relocate_8c09f3e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c09f3e0;
L8c09f3e0: *stop=0x8c09f3e0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c09f3e2; /* mov.l r14,@-r15 */
L8c09f3e2: *stop=0x8c09f3e2u;r[14]=r[4];goto L8c09f3e4; /* mov r4,r14 */
L8c09f3e4: *stop=0x8c09f3e4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c09f3e6; /* mov.l r13,@-r15 */
L8c09f3e6: *stop=0x8c09f3e6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c09f3e8; /* mov.l r12,@-r15 */
L8c09f3e8: *stop=0x8c09f3e8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c09f3ea; /* sts.l PR,@-r15 */
L8c09f3ea: *stop=0x8c09f3eau;if(!valid_ram(r[4]+3u,1u))return -3;r[0]=sx(m,r[4]+0x3u,1);goto L8c09f3ec; /* mov.b @(0x3,r4),r0 */
L8c09f3ec: *stop=0x8c09f3ecu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c09f3ee; /* cmp/eq #0x4,r0 */
L8c09f3ee: take=(*sr&1u);*stop=0x8c09f3f0u;r[14]+=8u;if(take)goto L8c09f406;goto L8c09f3f2; /* bt/s 0x8c09f406 */
L8c09f3f2: *stop=0x8c09f3f2u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c09f3f4; /* cmp/eq #0x3,r0 */
L8c09f3f4: if((*sr&1u))goto L8c09f402;goto L8c09f3f6; /* bt 0x8c09f402 */
L8c09f3f6: *stop=0x8c09f3f6u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c09f3f8; /* cmp/eq #0x2,r0 */
L8c09f3f8: if((*sr&1u))goto L8c09f402;goto L8c09f3fa; /* bt 0x8c09f402 */
L8c09f3fa: *stop=0x8c09f3fau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c09f3fc; /* cmp/eq #0x1,r0 */
L8c09f3fc: if((*sr&1u))goto L8c09f402;goto L8c09f3fe; /* bt 0x8c09f402 */
L8c09f3fe: *stop=0x8c09f400u;goto L8c09f402; /* bra 0x8c09f402 */
L8c09f402: *stop=0x8c09f404u;r[0]=0u;goto L8c09f454; /* bra 0x8c09f454 */
L8c09f406: *stop=0x8c09f406u;if(!valid_ram(r[14],2u))return -3;r[2]=sx(m,r[14],2);goto L8c09f408; /* mov.w @r14,r2 */
L8c09f408: *stop=0x8c09f408u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c09f40a; /* tst r2,r2 */
L8c09f40a: take=(*sr&1u);*stop=0x8c09f40cu;r[12]=4294967292u;if(take)goto L8c09f452;goto L8c09f40e; /* bt/s 0x8c09f452 */
L8c09f40e: *stop=0x8c09f40eu;if(!valid_ram(r[14],2u))return -3;r[0]=sx(m,r[14],2);goto L8c09f410; /* mov.w @r14,r0 */
L8c09f410: *stop=0x8c09f410u;if(!valid_ram(r[14]+4u,4u))return -3;r[13]=rd(m,r[14]+0x4u,4);goto L8c09f412; /* mov.l @(0x4,r14),r13 */
L8c09f412: *stop=0x8c09f412u;r[0]=r[0]&65535u;goto L8c09f414; /* extu.w r0,r0 */
L8c09f414: *stop=0x8c09f414u;r[13]+=r[14];goto L8c09f416; /* add r14,r13 */
L8c09f416: *stop=0x8c09f416u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c09f418; /* cmp/eq #0x1,r0 */
L8c09f418: *stop=0x8c09f418u;r[13]+=11u;goto L8c09f41a; /* add #0xb,r13 */
L8c09f41a: take=(*sr&1u);*stop=0x8c09f41cu;r[13]&=r[12];if(take)goto L8c09f42a;goto L8c09f41e; /* bt/s 0x8c09f42a */
L8c09f41e: *stop=0x8c09f41eu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c09f420; /* cmp/eq #0x3,r0 */
L8c09f420: if((*sr&1u))goto L8c09f436;goto L8c09f422; /* bt 0x8c09f436 */
L8c09f422: *stop=0x8c09f422u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c09f424; /* cmp/eq #0x4,r0 */
L8c09f424: if((*sr&1u))goto L8c09f442;goto L8c09f426; /* bt 0x8c09f442 */
L8c09f426: *stop=0x8c09f428u;goto L8c09f44a; /* bra 0x8c09f44a */
L8c09f42a: *stop=0x8c09f42au;r[4]=r[14];goto L8c09f42c; /* mov r14,r4 */
L8c09f42c: *stop=0x8c09f42cu;r[5]=r[13];goto L8c09f42e; /* mov r13,r5 */
L8c09f42e: target=0x8c09f5d6u;*pr=0x8c09f432u;*stop=0x8c09f430u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09f432; /* bsr 0x8c09f5d6 */
L8c09f432: *stop=0x8c09f434u;goto L8c09f44a; /* bra 0x8c09f44a */
L8c09f436: *stop=0x8c09f436u;r[4]=r[14];goto L8c09f438; /* mov r14,r4 */
L8c09f438: *stop=0x8c09f438u;r[5]=r[13];goto L8c09f43a; /* mov r13,r5 */
L8c09f43a: target=0x8c09f900u;*pr=0x8c09f43eu;*stop=0x8c09f43cu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09f43e; /* bsr 0x8c09f900 */
L8c09f43e: *stop=0x8c09f440u;goto L8c09f44a; /* bra 0x8c09f44a */
L8c09f442: *stop=0x8c09f442u;r[4]=r[14];goto L8c09f444; /* mov r14,r4 */
L8c09f444: *stop=0x8c09f444u;r[5]=r[13];goto L8c09f446; /* mov r13,r5 */
L8c09f446: target=0x8c09f966u;*pr=0x8c09f44au;*stop=0x8c09f448u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09f44a; /* bsr 0x8c09f966 */
L8c09f44a: *stop=0x8c09f44au;r[14]=r[13];goto L8c09f44c; /* mov r13,r14 */
L8c09f44c: *stop=0x8c09f44cu;if(!valid_ram(r[14],2u))return -3;r[3]=sx(m,r[14],2);goto L8c09f44e; /* mov.w @r14,r3 */
L8c09f44e: *stop=0x8c09f44eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c09f450; /* tst r3,r3 */
L8c09f450: if(!(*sr&1u))goto L8c09f40e;goto L8c09f452; /* bf 0x8c09f40e */
L8c09f452: *stop=0x8c09f452u;r[0]=1u;goto L8c09f454; /* mov #0x1,r0 */
L8c09f454: *stop=0x8c09f454u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c09f456; /* lds.l @r15+,PR */
L8c09f456: *stop=0x8c09f456u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c09f458; /* mov.l @r15+,r12 */
L8c09f458: *stop=0x8c09f458u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c09f45a; /* mov.l @r15+,r13 */
L8c09f45a: *stop=0x8c09f45au;if(*pr!=entry_pr)return -3;*stop=0x8c09f45cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
