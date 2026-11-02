/* Original camera follow-up and shared vector services; names inferred.
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
SA_STAGE_EXPORT int sa_camera_final_8c63ae0c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63ae0c;
L8c63ae0c: *stop=0x8c63ae0cu;r[0]=4u;goto L8c63ae0e; /* mov #0x4,r0 */
L8c63ae0e: *stop=0x8c63ae0eu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[5]);goto L8c63ae10; /* fmov.s @r5,fr3 */
L8c63ae10: *stop=0x8c63ae10u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63ae12; /* fmul fr4,fr3 */
L8c63ae12: *stop=0x8c63ae12u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[4]);goto L8c63ae14; /* fmov.s fr3,@r4 */
L8c63ae14: *stop=0x8c63ae14u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[5]);goto L8c63ae16; /* fmov.s @(r0,r5),fr2 */
L8c63ae16: *stop=0x8c63ae16u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63ae18; /* fmul fr4,fr2 */
L8c63ae18: *stop=0x8c63ae18u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto L8c63ae1a; /* fmov fr2,@(r0,r4) */
L8c63ae1a: *stop=0x8c63ae1au;r[0]=8u;goto L8c63ae1c; /* mov #0x8,r0 */
L8c63ae1c: *stop=0x8c63ae1cu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto L8c63ae1e; /* fmov.s @(r0,r5),fr3 */
L8c63ae1e: *stop=0x8c63ae1eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63ae20; /* fmul fr4,fr3 */
L8c63ae20: *stop=0x8c63ae20u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto L8c63ae22; /* fmov fr3,@(r0,r4) */
L8c63ae22: *stop=0x8c63ae22u;r[0]=12u;goto L8c63ae24; /* mov #0xc,r0 */
L8c63ae24: *stop=0x8c63ae24u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto L8c63ae26; /* fmov.s @(r0,r5),fr3 */
L8c63ae26: *stop=0x8c63ae26u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63ae28; /* fmul fr4,fr3 */
L8c63ae28: *stop=0x8c63ae28u;if(*pr!=entry_pr)return -3;*stop=0x8c63ae2au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_final_8c63aec8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63aec8;
L8c63aec8: *stop=0x8c63aec8u;r[0]=4u;goto L8c63aeca; /* mov #0x4,r0 */
L8c63aeca: *stop=0x8c63aecau;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[5]);goto L8c63aecc; /* fmov.s @r5,fr3 */
L8c63aecc: *stop=0x8c63aeccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63aece; /* fmul fr4,fr3 */
L8c63aece: *stop=0x8c63aeceu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[4]);goto L8c63aed0; /* fmov.s fr3,@r4 */
L8c63aed0: *stop=0x8c63aed0u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[5]);goto L8c63aed2; /* fmov.s @(r0,r5),fr2 */
L8c63aed2: *stop=0x8c63aed2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63aed4; /* fmul fr4,fr2 */
L8c63aed4: *stop=0x8c63aed4u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[4]);goto L8c63aed6; /* fmov fr2,@(r0,r4) */
L8c63aed6: *stop=0x8c63aed6u;r[0]=8u;goto L8c63aed8; /* mov #0x8,r0 */
L8c63aed8: *stop=0x8c63aed8u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto L8c63aeda; /* fmov.s @(r0,r5),fr3 */
L8c63aeda: *stop=0x8c63aedau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63aedc; /* fmul fr4,fr3 */
L8c63aedc: *stop=0x8c63aedcu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto L8c63aede; /* fmov fr3,@(r0,r4) */
L8c63aede: *stop=0x8c63aedeu;r[0]=12u;goto L8c63aee0; /* mov #0xc,r0 */
L8c63aee0: *stop=0x8c63aee0u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto L8c63aee2; /* fmov.s @(r0,r5),fr3 */
L8c63aee2: *stop=0x8c63aee2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63aee4; /* fmul fr4,fr3 */
L8c63aee4: *stop=0x8c63aee4u;if(*pr!=entry_pr)return -3;*stop=0x8c63aee6u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_final_8c01a718(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01a718;
L8c01a718: *stop=0x8c01a718u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c01a71a; /* mov.l r14,@-r15 */
L8c01a71a: *stop=0x8c01a71au;r[0]=8u;goto L8c01a71c; /* mov #0x8,r0 */
L8c01a71c: *stop=0x8c01a71cu;r[14]=rd(m,0x8c01a8a0u,4);goto L8c01a71e; /* mov.l 0x8c01a8a0,r14 */
L8c01a71e: *stop=0x8c01a71eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c01a720; /* mov.l r13,@-r15 */
L8c01a720: *stop=0x8c01a720u;r[13]=r[4];goto L8c01a722; /* mov r4,r13 */
L8c01a722: *stop=0x8c01a722u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c01a724; /* fmov.s fr15,@-r15 */
L8c01a724: *stop=0x8c01a724u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[15]);goto L8c01a726; /* fmov.s fr13,@-r15 */
L8c01a726: *stop=0x8c01a726u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[15]);goto L8c01a728; /* fmov.s fr12,@-r15 */
L8c01a728: *stop=0x8c01a728u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01a72a; /* sts.l PR,@-r15 */
L8c01a72a: *stop=0x8c01a72au;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a72c; /* mov.l @r14,r3 */
L8c01a72c: *stop=0x8c01a72cu;r[4]=r[3];goto L8c01a72e; /* mov r3,r4 */
L8c01a72e: *stop=0x8c01a72eu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]+r[3]);goto L8c01a730; /* fmov.s @(r0,r3),fr12 */
L8c01a730: *stop=0x8c01a730u;r[3]=rd(m,0x8c01a8b0u,4);goto L8c01a732; /* mov.l 0x8c01a8b0,r3 */
L8c01a732: target=r[3];*pr=0x8c01a736u;*stop=0x8c01a734u;if(!valid_ram(r[4]+36u,4u))return -3;r[4]=rd(m,r[4]+0x24u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a736; /* jsr @r3 */
L8c01a736: *stop=0x8c01a736u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[0],&f->fpscr,&fr[12]);if(status)return status;}goto L8c01a738; /* fmul fr0,fr12 */
L8c01a738: *stop=0x8c01a738u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a73a; /* mov.l @r14,r3 */
L8c01a73a: *stop=0x8c01a73au;r[0]=28u;goto L8c01a73c; /* mov #0x1c,r0 */
L8c01a73c: *stop=0x8c01a73cu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[0]+r[3]);goto L8c01a73e; /* fmov.s @(r0,r3),fr13 */
L8c01a73e: *stop=0x8c01a73eu;r[0]=8u;goto L8c01a740; /* mov #0x8,r0 */
L8c01a740: *stop=0x8c01a740u;r[4]=r[3];goto L8c01a742; /* mov r3,r4 */
L8c01a742: *stop=0x8c01a742u;r[2]=r[3];goto L8c01a744; /* mov r3,r2 */
L8c01a744: *stop=0x8c01a744u;fcopy(f,15u,12u);goto L8c01a746; /* fmov fr12,fr15 */
L8c01a746: *stop=0x8c01a746u;r[3]=rd(m,0x8c01a8b4u,4);goto L8c01a748; /* mov.l 0x8c01a8b4,r3 */
L8c01a748: *stop=0x8c01a748u;if(!valid_ram(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]+r[2]);goto L8c01a74a; /* fmov.s @(r0,r2),fr12 */
L8c01a74a: target=r[3];*pr=0x8c01a74eu;*stop=0x8c01a74cu;if(!valid_ram(r[4]+36u,4u))return -3;r[4]=rd(m,r[4]+0x24u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a74e; /* jsr @r3 */
L8c01a74e: *stop=0x8c01a74eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[0],&f->fpscr,&fr[12]);if(status)return status;}goto L8c01a750; /* fmul fr0,fr12 */
L8c01a750: *stop=0x8c01a750u;r[0]=36u;goto L8c01a752; /* mov #0x24,r0 */
L8c01a752: *stop=0x8c01a752u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[12],&f->fpscr,&fr[13],1);if(status)return status;}goto L8c01a754; /* fsub fr12,fr13 */
L8c01a754: *stop=0x8c01a754u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,13u,r[0]+r[13]);goto L8c01a756; /* fmov fr13,@(r0,r13) */
L8c01a756: *stop=0x8c01a756u;r[0]=24u;goto L8c01a758; /* mov #0x18,r0 */
L8c01a758: *stop=0x8c01a758u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a75a; /* mov.l @r14,r3 */
L8c01a75a: *stop=0x8c01a75au;r[4]=r[3];goto L8c01a75c; /* mov r3,r4 */
L8c01a75c: *stop=0x8c01a75cu;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]+r[3]);goto L8c01a75e; /* fmov.s @(r0,r3),fr12 */
L8c01a75e: *stop=0x8c01a75eu;r[3]=rd(m,0x8c01a8b4u,4);goto L8c01a760; /* mov.l 0x8c01a8b4,r3 */
L8c01a760: target=r[3];*pr=0x8c01a764u;*stop=0x8c01a762u;if(!valid_ram(r[4]+40u,4u))return -3;r[4]=rd(m,r[4]+0x28u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a764; /* jsr @r3 */
L8c01a764: *stop=0x8c01a764u;fcopy(f,3u,0u);goto L8c01a766; /* fmov fr0,fr3 */
L8c01a766: *stop=0x8c01a766u;r[0]=32u;goto L8c01a768; /* mov #0x20,r0 */
L8c01a768: *stop=0x8c01a768u;fcopy(f,0u,15u);goto L8c01a76a; /* fmov fr15,fr0 */
L8c01a76a: *stop=0x8c01a76au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[12],&f->fpscr,&fr[12]);if(status)return status;}goto L8c01a76c; /* fmac fr0,fr3,fr12 */
L8c01a76c: *stop=0x8c01a76cu;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,12u,r[0]+r[13]);goto L8c01a76e; /* fmov fr12,@(r0,r13) */
L8c01a76e: *stop=0x8c01a76eu;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a770; /* mov.l @r14,r3 */
L8c01a770: *stop=0x8c01a770u;r[4]=r[3];goto L8c01a772; /* mov r3,r4 */
L8c01a772: *stop=0x8c01a772u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]+r[3]);goto L8c01a774; /* fmov.s @(r0,r3),fr12 */
L8c01a774: *stop=0x8c01a774u;r[3]=rd(m,0x8c01a8b0u,4);goto L8c01a776; /* mov.l 0x8c01a8b0,r3 */
L8c01a776: target=r[3];*pr=0x8c01a77au;*stop=0x8c01a778u;if(!valid_ram(r[4]+40u,4u))return -3;r[4]=rd(m,r[4]+0x28u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01a77a; /* jsr @r3 */
L8c01a77a: *stop=0x8c01a77au;fcopy(f,3u,0u);goto L8c01a77c; /* fmov fr0,fr3 */
L8c01a77c: *stop=0x8c01a77cu;r[0]=40u;goto L8c01a77e; /* mov #0x28,r0 */
L8c01a77e: *stop=0x8c01a77eu;fcopy(f,0u,15u);goto L8c01a780; /* fmov fr15,fr0 */
L8c01a780: *stop=0x8c01a780u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[12],&f->fpscr,&fr[12]);if(status)return status;}goto L8c01a782; /* fmac fr0,fr3,fr12 */
L8c01a782: *stop=0x8c01a782u;if(!valid_ram(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,12u,r[0]+r[13]);goto L8c01a784; /* fmov fr12,@(r0,r13) */
L8c01a784: *stop=0x8c01a784u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a786; /* mov.l @r14,r3 */
L8c01a786: *stop=0x8c01a786u;if(!valid_ram(r[3]+36u,4u))return -3;r[2]=rd(m,r[3]+0x24u,4);goto L8c01a788; /* mov.l @(0x24,r3),r2 */
L8c01a788: *stop=0x8c01a788u;if(!valid_ram(r[13]+20u,4u))return -3;wr(m,r[13]+0x14u,r[2],4);goto L8c01a78a; /* mov.l r2,@(0x14,r13) */
L8c01a78a: *stop=0x8c01a78au;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a78c; /* mov.l @r14,r3 */
L8c01a78c: *stop=0x8c01a78cu;if(!valid_ram(r[3]+40u,4u))return -3;r[2]=rd(m,r[3]+0x28u,4);goto L8c01a78e; /* mov.l @(0x28,r3),r2 */
L8c01a78e: *stop=0x8c01a78eu;if(!valid_ram(r[13]+24u,4u))return -3;wr(m,r[13]+0x18u,r[2],4);goto L8c01a790; /* mov.l r2,@(0x18,r13) */
L8c01a790: *stop=0x8c01a790u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c01a792; /* mov.l @r14,r3 */
L8c01a792: *stop=0x8c01a792u;if(!valid_ram(r[3]+44u,4u))return -3;r[2]=rd(m,r[3]+0x2cu,4);goto L8c01a794; /* mov.l @(0x2c,r3),r2 */
L8c01a794: *stop=0x8c01a794u;if(!valid_ram(r[13]+28u,4u))return -3;wr(m,r[13]+0x1cu,r[2],4);goto L8c01a796; /* mov.l r2,@(0x1c,r13) */
L8c01a796: *stop=0x8c01a796u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01a798; /* lds.l @r15+,PR */
L8c01a798: *stop=0x8c01a798u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c01a79a; /* fmov.s @r15+,fr12 */
L8c01a79a: *stop=0x8c01a79au;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c01a79c; /* fmov.s @r15+,fr13 */
L8c01a79c: *stop=0x8c01a79cu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c01a79e; /* fmov.s @r15+,fr15 */
L8c01a79e: *stop=0x8c01a79eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01a7a0; /* mov.l @r15+,r13 */
L8c01a7a0: *stop=0x8c01a7a0u;if(*pr!=entry_pr)return -3;*stop=0x8c01a7a2u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_final_8c01c296(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01c296;
L8c01c296: *stop=0x8c01c298u;if(!call)return -2;return call(ctx,0x8c01bd9eu,m,f,sr,r,pr); /* bra 0x8c01bd9e */
}

SA_STAGE_EXPORT int sa_camera_final_8c01c29a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c01c29a;
L8c01c29a: *stop=0x8c01c29au;r[6]=2u;goto L8c01c29c; /* mov #0x2,r6 */
L8c01c29c: *stop=0x8c01c29cu;r[5]=rd(m,0x8c01c340u,4);goto L8c01c29e; /* mov.l 0x8c01c340,r5 */
L8c01c29e: *stop=0x8c01c29eu;r[6]<<=8;goto L8c01c2a0; /* shll8 r6 */
L8c01c2a0: *stop=0x8c01c2a0u;r[6]+=88u;goto L8c01c2a2; /* add #0x58,r6 */
L8c01c2a2: *stop=0x8c01c2a2u;r[4]=r[5];goto L8c01c2a4; /* mov r5,r4 */
L8c01c2a4: *stop=0x8c01c2a4u;r[6]+=r[5];goto L8c01c2a6; /* add r5,r6 */
L8c01c2a6: *stop=0x8c01c2a6u;r[5]=rd(m,0x8c01c344u,4);goto L8c01c2a8; /* mov.l 0x8c01c344,r5 */
L8c01c2a8: *stop=0x8c01c2a8u;*sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c01c2aa; /* cmp/hs r6,r4 */
L8c01c2aa: if((*sr&1u))goto L8c01c2d0;goto L8c01c2ac; /* bt 0x8c01c2d0 */
L8c01c2ac: *stop=0x8c01c2acu;if(!valid_ram(r[5],4u))return -3;r[2]=rd(m,r[5],4);goto L8c01c2ae; /* mov.l @r5,r2 */
L8c01c2ae: *stop=0x8c01c2aeu;if(!valid_ram(r[2]+32u,4u))return -3;r[3]=rd(m,r[2]+0x20u,4);goto L8c01c2b0; /* mov.l @(0x20,r2),r3 */
L8c01c2b0: *stop=0x8c01c2b0u;if(!valid_ram(r[2]+36u,4u))return -3;r[1]=rd(m,r[2]+0x24u,4);goto L8c01c2b2; /* mov.l @(0x24,r2),r1 */
L8c01c2b2: *stop=0x8c01c2b2u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[3],4);goto L8c01c2b4; /* mov.l r3,@r4 */
L8c01c2b4: *stop=0x8c01c2b4u;if(!valid_ram(r[4]+4u,4u))return -3;wr(m,r[4]+0x4u,r[1],4);goto L8c01c2b6; /* mov.l r1,@(0x4,r4) */
L8c01c2b6: *stop=0x8c01c2b6u;if(!valid_ram(r[2]+40u,4u))return -3;r[3]=rd(m,r[2]+0x28u,4);goto L8c01c2b8; /* mov.l @(0x28,r2),r3 */
L8c01c2b8: *stop=0x8c01c2b8u;if(!valid_ram(r[4]+8u,4u))return -3;wr(m,r[4]+0x8u,r[3],4);goto L8c01c2ba; /* mov.l r3,@(0x8,r4) */
L8c01c2ba: *stop=0x8c01c2bau;r[4]+=12u;goto L8c01c2bc; /* add #0xc,r4 */
L8c01c2bc: *stop=0x8c01c2bcu;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c01c2be; /* mov.l @r5,r3 */
L8c01c2be: *stop=0x8c01c2beu;if(!valid_ram(r[3]+32u,4u))return -3;r[2]=rd(m,r[3]+0x20u,4);goto L8c01c2c0; /* mov.l @(0x20,r3),r2 */
L8c01c2c0: *stop=0x8c01c2c0u;if(!valid_ram(r[3]+36u,4u))return -3;r[1]=rd(m,r[3]+0x24u,4);goto L8c01c2c2; /* mov.l @(0x24,r3),r1 */
L8c01c2c2: *stop=0x8c01c2c2u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[2],4);goto L8c01c2c4; /* mov.l r2,@r4 */
L8c01c2c4: *stop=0x8c01c2c4u;if(!valid_ram(r[4]+4u,4u))return -3;wr(m,r[4]+0x4u,r[1],4);goto L8c01c2c6; /* mov.l r1,@(0x4,r4) */
L8c01c2c6: *stop=0x8c01c2c6u;if(!valid_ram(r[3]+40u,4u))return -3;r[2]=rd(m,r[3]+0x28u,4);goto L8c01c2c8; /* mov.l @(0x28,r3),r2 */
L8c01c2c8: *stop=0x8c01c2c8u;if(!valid_ram(r[4]+8u,4u))return -3;wr(m,r[4]+0x8u,r[2],4);goto L8c01c2ca; /* mov.l r2,@(0x8,r4) */
L8c01c2ca: *stop=0x8c01c2cau;r[4]+=12u;goto L8c01c2cc; /* add #0xc,r4 */
L8c01c2cc: *stop=0x8c01c2ccu;*sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c01c2ce; /* cmp/hs r6,r4 */
L8c01c2ce: if(!(*sr&1u))goto L8c01c2ac;goto L8c01c2d0; /* bf 0x8c01c2ac */
L8c01c2d0: *stop=0x8c01c2d0u;r[3]=rd(m,0x8c01c348u,4);goto L8c01c2d2; /* mov.l 0x8c01c348,r3 */
L8c01c2d2: *stop=0x8c01c2d2u;r[2]=0u;goto L8c01c2d4; /* mov #0x0,r2 */
L8c01c2d4: *stop=0x8c01c2d4u;if(*pr!=entry_pr)return -3;*stop=0x8c01c2d6u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[2],4);return 0; /* rts */
}
