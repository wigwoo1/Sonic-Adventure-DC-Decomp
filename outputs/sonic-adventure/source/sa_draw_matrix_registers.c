/* Complete register-input matrix scale and translation; names inferred.
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
SA_STAGE_EXPORT int sa_draw_matrix_scale_registers_8c63a69c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63a69c;
L8c63a69c: *stop=0x8c63a69cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c63a69e; /* tst r4,r4 */
L8c63a69e: if(!(*sr&1u))goto L8c63a6e2;goto L8c63a6a0; /* bf 0x8c63a6e2 */
L8c63a6a0: *stop=0x8c63a6a0u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a6a2; /* fschg */
L8c63a6a2: *stop=0x8c63a6a2u;fcopy(f,0u,1u);goto L8c63a6a4; /* fmov fr1,fr0 */
L8c63a6a4: *stop=0x8c63a6a4u;fcopy(f,2u,3u);goto L8c63a6a6; /* fmov fr3,fr2 */
L8c63a6a6: *stop=0x8c63a6a6u;fcopy(f,8u,5u);goto L8c63a6a8; /* fmov fr5,fr8 */
L8c63a6a8: *stop=0x8c63a6a8u;fcopy(f,10u,7u);goto L8c63a6aa; /* fmov fr7,fr10 */
L8c63a6aa: *stop=0x8c63a6aau;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a6ac; /* fschg */
L8c63a6ac: *stop=0x8c63a6acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c63a6ae; /* fmul fr4,fr0 */
L8c63a6ae: *stop=0x8c63a6aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a6b0; /* fmul fr4,fr1 */
L8c63a6b0: *stop=0x8c63a6b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a6b2; /* fmul fr4,fr2 */
L8c63a6b2: *stop=0x8c63a6b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63a6b4; /* fmul fr4,fr3 */
L8c63a6b4: *stop=0x8c63a6b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c63a6b6; /* fmul fr5,fr8 */
L8c63a6b6: *stop=0x8c63a6b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[5],&f->fpscr,&fr[9]);if(status)return status;}goto L8c63a6b8; /* fmul fr5,fr9 */
L8c63a6b8: *stop=0x8c63a6b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[5],&f->fpscr,&fr[10]);if(status)return status;}goto L8c63a6ba; /* fmul fr5,fr10 */
L8c63a6ba: *stop=0x8c63a6bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[5],&f->fpscr,&fr[11]);if(status)return status;}goto L8c63a6bc; /* fmul fr5,fr11 */
L8c63a6bc: *stop=0x8c63a6bcu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a6be; /* fschg */
L8c63a6be: *stop=0x8c63a6beu;fcopy(f,1u,0u);goto L8c63a6c0; /* fmov fr0,fr1 */
L8c63a6c0: *stop=0x8c63a6c0u;fcopy(f,3u,2u);goto L8c63a6c2; /* fmov fr2,fr3 */
L8c63a6c2: *stop=0x8c63a6c2u;fcopy(f,5u,8u);goto L8c63a6c4; /* fmov fr8,fr5 */
L8c63a6c4: *stop=0x8c63a6c4u;fcopy(f,7u,10u);goto L8c63a6c6; /* fmov fr10,fr7 */
L8c63a6c6: *stop=0x8c63a6c6u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a6c8; /* fschg */
L8c63a6c8: *stop=0x8c63a6c8u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a6ca; /* fschg */
L8c63a6ca: *stop=0x8c63a6cau;fcopy(f,0u,9u);goto L8c63a6cc; /* fmov fr9,fr0 */
L8c63a6cc: *stop=0x8c63a6ccu;fcopy(f,2u,11u);goto L8c63a6ce; /* fmov fr11,fr2 */
L8c63a6ce: *stop=0x8c63a6ceu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a6d0; /* fschg */
L8c63a6d0: *stop=0x8c63a6d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;}goto L8c63a6d2; /* fmul fr6,fr0 */
L8c63a6d2: *stop=0x8c63a6d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a6d4; /* fmul fr6,fr1 */
L8c63a6d4: *stop=0x8c63a6d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a6d6; /* fmul fr6,fr2 */
L8c63a6d6: *stop=0x8c63a6d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63a6d8; /* fmul fr6,fr3 */
L8c63a6d8: *stop=0x8c63a6d8u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a6da; /* fschg */
L8c63a6da: *stop=0x8c63a6dau;fcopy(f,9u,0u);goto L8c63a6dc; /* fmov fr0,fr9 */
L8c63a6dc: *stop=0x8c63a6dcu;fcopy(f,11u,2u);goto L8c63a6de; /* fmov fr2,fr11 */
L8c63a6de: *stop=0x8c63a6deu;if(*pr!=entry_pr)return -3;*stop=0x8c63a6e0u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;return 0; /* rts */
L8c63a6e2: *stop=0x8c63a6e2u;r[5]=r[4];goto L8c63a6e4; /* mov r4,r5 */
L8c63a6e4: *stop=0x8c63a6e4u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a6e6; /* fmov.s @r4+,fr3 */
L8c63a6e6: *stop=0x8c63a6e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63a6e8; /* fmul fr4,fr3 */
L8c63a6e8: *stop=0x8c63a6e8u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a6ea; /* fmov.s @r4+,fr2 */
L8c63a6ea: *stop=0x8c63a6eau;r[0]=4u;goto L8c63a6ec; /* mov #0x4,r0 */
L8c63a6ec: *stop=0x8c63a6ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a6ee; /* fmul fr4,fr2 */
L8c63a6ee: *stop=0x8c63a6eeu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[5]);goto L8c63a6f0; /* fmov.s fr3,@r5 */
L8c63a6f0: *stop=0x8c63a6f0u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a6f2; /* fmov.s @r4+,fr1 */
L8c63a6f2: *stop=0x8c63a6f2u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[5]);goto L8c63a6f4; /* fmov fr2,@(r0,r5) */
L8c63a6f4: *stop=0x8c63a6f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a6f6; /* fmul fr4,fr1 */
L8c63a6f6: *stop=0x8c63a6f6u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a6f8; /* fmov.s @r4+,fr3 */
L8c63a6f8: *stop=0x8c63a6f8u;r[0]=8u;goto L8c63a6fa; /* mov #0x8,r0 */
L8c63a6fa: *stop=0x8c63a6fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63a6fc; /* fmul fr4,fr3 */
L8c63a6fc: *stop=0x8c63a6fcu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[5]);goto L8c63a6fe; /* fmov fr1,@(r0,r5) */
L8c63a6fe: *stop=0x8c63a6feu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a700; /* fmov.s @r4+,fr2 */
L8c63a700: *stop=0x8c63a700u;r[0]=12u;goto L8c63a702; /* mov #0xc,r0 */
L8c63a702: *stop=0x8c63a702u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a704; /* fmul fr5,fr2 */
L8c63a704: *stop=0x8c63a704u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto L8c63a706; /* fmov fr3,@(r0,r5) */
L8c63a706: *stop=0x8c63a706u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a708; /* fmov.s @r4+,fr1 */
L8c63a708: *stop=0x8c63a708u;r[0]=16u;goto L8c63a70a; /* mov #0x10,r0 */
L8c63a70a: *stop=0x8c63a70au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[5],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a70c; /* fmul fr5,fr1 */
L8c63a70c: *stop=0x8c63a70cu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[5]);goto L8c63a70e; /* fmov fr2,@(r0,r5) */
L8c63a70e: *stop=0x8c63a70eu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a710; /* fmov.s @r4+,fr3 */
L8c63a710: *stop=0x8c63a710u;r[0]=20u;goto L8c63a712; /* mov #0x14,r0 */
L8c63a712: *stop=0x8c63a712u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63a714; /* fmul fr5,fr3 */
L8c63a714: *stop=0x8c63a714u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[5]);goto L8c63a716; /* fmov fr1,@(r0,r5) */
L8c63a716: *stop=0x8c63a716u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a718; /* fmov.s @r4+,fr2 */
L8c63a718: *stop=0x8c63a718u;r[0]=24u;goto L8c63a71a; /* mov #0x18,r0 */
L8c63a71a: *stop=0x8c63a71au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a71c; /* fmul fr5,fr2 */
L8c63a71c: *stop=0x8c63a71cu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto L8c63a71e; /* fmov fr3,@(r0,r5) */
L8c63a71e: *stop=0x8c63a71eu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a720; /* fmov.s @r4+,fr1 */
L8c63a720: *stop=0x8c63a720u;r[0]=28u;goto L8c63a722; /* mov #0x1c,r0 */
L8c63a722: *stop=0x8c63a722u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a724; /* fmul fr6,fr1 */
L8c63a724: *stop=0x8c63a724u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[5]);goto L8c63a726; /* fmov fr2,@(r0,r5) */
L8c63a726: *stop=0x8c63a726u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a728; /* fmov.s @r4+,fr3 */
L8c63a728: *stop=0x8c63a728u;r[0]=32u;goto L8c63a72a; /* mov #0x20,r0 */
L8c63a72a: *stop=0x8c63a72au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63a72c; /* fmul fr6,fr3 */
L8c63a72c: *stop=0x8c63a72cu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[5]);goto L8c63a72e; /* fmov fr1,@(r0,r5) */
L8c63a72e: *stop=0x8c63a72eu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a730; /* fmov.s @r4+,fr2 */
L8c63a730: *stop=0x8c63a730u;r[0]=36u;goto L8c63a732; /* mov #0x24,r0 */
L8c63a732: *stop=0x8c63a732u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63a734; /* fmul fr6,fr2 */
L8c63a734: *stop=0x8c63a734u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto L8c63a736; /* fmov fr3,@(r0,r5) */
L8c63a736: *stop=0x8c63a736u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[4]);goto L8c63a738; /* fmov.s @r4,fr1 */
L8c63a738: *stop=0x8c63a738u;r[0]=40u;goto L8c63a73a; /* mov #0x28,r0 */
L8c63a73a: *stop=0x8c63a73au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63a73c; /* fmul fr6,fr1 */
L8c63a73c: *stop=0x8c63a73cu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[5]);goto L8c63a73e; /* fmov fr2,@(r0,r5) */
L8c63a73e: *stop=0x8c63a73eu;if(*pr!=entry_pr)return -3;*stop=0x8c63a740u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_draw_matrix_translate_registers_8c63a878(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c63a878;
L8c63a878: *stop=0x8c63a878u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c63a87a; /* tst r4,r4 */
L8c63a87a: take=!(*sr&1u);*stop=0x8c63a87cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;if(take)goto L8c63a88a;goto L8c63a87e; /* bf/s 0x8c63a88a */
L8c63a87e: *stop=0x8c63a87eu;status=sa_sh4_vector_numeric(0xf5fdu,f,*fpul);if(status)return status;goto L8c63a880; /* ftrv xmtrx,fv4 */
L8c63a880: *stop=0x8c63a880u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a882; /* fschg */
L8c63a882: *stop=0x8c63a882u;fcopy(f,13u,4u);goto L8c63a884; /* fmov fr4,fr13 */
L8c63a884: *stop=0x8c63a884u;fcopy(f,15u,6u);goto L8c63a886; /* fmov fr6,fr15 */
L8c63a886: *stop=0x8c63a886u;if(*pr!=entry_pr)return -3;*stop=0x8c63a888u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;return 0; /* rts */
L8c63a88a: *stop=0x8c63a88au;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[4]);goto L8c63a88c; /* fmov.s @r4,fr0 */
L8c63a88c: *stop=0x8c63a88cu;r[0]=16u;goto L8c63a88e; /* mov #0x10,r0 */
L8c63a88e: *stop=0x8c63a88eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto L8c63a890; /* fmov.s @(r0,r4),fr1 */
L8c63a890: *stop=0x8c63a890u;r[0]=32u;goto L8c63a892; /* mov #0x20,r0 */
L8c63a892: *stop=0x8c63a892u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c63a894; /* fmov.s @(r0,r4),fr2 */
L8c63a894: *stop=0x8c63a894u;r[0]=48u;goto L8c63a896; /* mov #0x30,r0 */
L8c63a896: *stop=0x8c63a896u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c63a898; /* fmov.s @(r0,r4),fr3 */
L8c63a898: *stop=0x8c63a898u;status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c63a89a; /* fipr fv4,fv0 */
L8c63a89a: *stop=0x8c63a89au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto L8c63a89c; /* fmov fr3,@(r0,r4) */
L8c63a89c: *stop=0x8c63a89cu;r[0]=4u;goto L8c63a89e; /* mov #0x4,r0 */
L8c63a89e: *stop=0x8c63a89eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto L8c63a8a0; /* fmov.s @(r0,r4),fr0 */
L8c63a8a0: *stop=0x8c63a8a0u;r[0]=20u;goto L8c63a8a2; /* mov #0x14,r0 */
L8c63a8a2: *stop=0x8c63a8a2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto L8c63a8a4; /* fmov.s @(r0,r4),fr1 */
L8c63a8a4: *stop=0x8c63a8a4u;r[0]=36u;goto L8c63a8a6; /* mov #0x24,r0 */
L8c63a8a6: *stop=0x8c63a8a6u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c63a8a8; /* fmov.s @(r0,r4),fr2 */
L8c63a8a8: *stop=0x8c63a8a8u;r[0]=52u;goto L8c63a8aa; /* mov #0x34,r0 */
L8c63a8aa: *stop=0x8c63a8aau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c63a8ac; /* fmov.s @(r0,r4),fr3 */
L8c63a8ac: *stop=0x8c63a8acu;status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c63a8ae; /* fipr fv4,fv0 */
L8c63a8ae: *stop=0x8c63a8aeu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto L8c63a8b0; /* fmov fr3,@(r0,r4) */
L8c63a8b0: *stop=0x8c63a8b0u;r[0]=8u;goto L8c63a8b2; /* mov #0x8,r0 */
L8c63a8b2: *stop=0x8c63a8b2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto L8c63a8b4; /* fmov.s @(r0,r4),fr0 */
L8c63a8b4: *stop=0x8c63a8b4u;r[0]=24u;goto L8c63a8b6; /* mov #0x18,r0 */
L8c63a8b6: *stop=0x8c63a8b6u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto L8c63a8b8; /* fmov.s @(r0,r4),fr1 */
L8c63a8b8: *stop=0x8c63a8b8u;r[0]=40u;goto L8c63a8ba; /* mov #0x28,r0 */
L8c63a8ba: *stop=0x8c63a8bau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c63a8bc; /* fmov.s @(r0,r4),fr2 */
L8c63a8bc: *stop=0x8c63a8bcu;r[0]=56u;goto L8c63a8be; /* mov #0x38,r0 */
L8c63a8be: *stop=0x8c63a8beu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c63a8c0; /* fmov.s @(r0,r4),fr3 */
L8c63a8c0: *stop=0x8c63a8c0u;status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c63a8c2; /* fipr fv4,fv0 */
L8c63a8c2: *stop=0x8c63a8c2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto L8c63a8c4; /* fmov fr3,@(r0,r4) */
L8c63a8c4: *stop=0x8c63a8c4u;r[0]=12u;goto L8c63a8c6; /* mov #0xc,r0 */
L8c63a8c6: *stop=0x8c63a8c6u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,0u,r[0]+r[4]);goto L8c63a8c8; /* fmov.s @(r0,r4),fr0 */
L8c63a8c8: *stop=0x8c63a8c8u;r[0]=28u;goto L8c63a8ca; /* mov #0x1c,r0 */
L8c63a8ca: *stop=0x8c63a8cau;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto L8c63a8cc; /* fmov.s @(r0,r4),fr1 */
L8c63a8cc: *stop=0x8c63a8ccu;r[0]=44u;goto L8c63a8ce; /* mov #0x2c,r0 */
L8c63a8ce: *stop=0x8c63a8ceu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[4]);goto L8c63a8d0; /* fmov.s @(r0,r4),fr2 */
L8c63a8d0: *stop=0x8c63a8d0u;r[0]=60u;goto L8c63a8d2; /* mov #0x3c,r0 */
L8c63a8d2: *stop=0x8c63a8d2u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[4]);goto L8c63a8d4; /* fmov.s @(r0,r4),fr3 */
L8c63a8d4: *stop=0x8c63a8d4u;status=sa_sh4_vector_numeric(0xf1edu,f,*fpul);if(status)return status;goto L8c63a8d6; /* fipr fv4,fv0 */
L8c63a8d6: *stop=0x8c63a8d6u;if(*pr!=entry_pr)return -3;*stop=0x8c63a8d8u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);return 0; /* rts */
}
