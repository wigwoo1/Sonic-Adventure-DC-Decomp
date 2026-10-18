/* AL_MAIN parameter update and scale smoothing; names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_add_sub.h"
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
SA_STAGE_EXPORT int sa_overlay_scale_spring_0c905618(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto Lc905618;
Lc905618: *stop=0xc905618u;r[0]=48u;goto Lc90561a; /* mov #0x30,r0 */
Lc90561a: *stop=0xc90561au;r[1]=rd(m,0x0c905744u,4);goto Lc90561c; /* mov.l 0x0c905744,r1 */
Lc90561c: *stop=0xc90561cu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[4]);goto Lc90561e; /* fmov.s @(r0,r4),fr4 */
Lc90561e: *stop=0xc90561eu;r[0]=4294967252u;goto Lc905620; /* mov #-0x2c,r0 */
Lc905620: *stop=0xc905620u;r[0]=r[0]&255u;goto Lc905622; /* extu.b r0,r0 */
Lc905622: *stop=0xc905622u;*fpul=r[1];goto Lc905624; /* lds r1,FPUL */
Lc905624: *stop=0xc905624u;r[1]=r[0];goto Lc905626; /* mov r0,r1 */
Lc905626: *stop=0xc905626u;r[1]+=r[5];goto Lc905628; /* add r5,r1 */
Lc905628: *stop=0xc905628u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);goto Lc90562a; /* fmov.s @r1,fr1 */
Lc90562a: *stop=0xc90562au;r[1]=rd(m,0x0c905748u,4);goto Lc90562c; /* mov.l 0x0c905748,r1 */
Lc90562c: *stop=0xc90562cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto Lc90562e; /* fldi1 fr3 */
Lc90562e: *stop=0xc90562eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto Lc905630; /* fneg fr3 */
Lc905630: *stop=0xc905630u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto Lc905632; /* fadd fr3,fr4 */
Lc905632: *stop=0xc905632u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto Lc905634; /* fsts FPUL,fr2 */
Lc905634: *stop=0xc905634u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto Lc905636; /* fmul fr2,fr1 */
Lc905636: *stop=0xc905636u;*fpul=r[1];goto Lc905638; /* lds r1,FPUL */
Lc905638: *stop=0xc905638u;r[1]=4294967252u;goto Lc90563a; /* mov #-0x2c,r1 */
Lc90563a: *stop=0xc90563au;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto Lc90563c; /* fmov.s @(r0,r5),fr3 */
Lc90563c: *stop=0xc90563cu;r[1]=r[1]&255u;goto Lc90563e; /* extu.b r1,r1 */
Lc90563e: *stop=0xc90563eu;r[1]+=r[5];goto Lc905640; /* add r5,r1 */
Lc905640: *stop=0xc905640u;f->bank[(f->fpscr>>21)&1u][0]=*fpul;goto Lc905642; /* fsts FPUL,fr0 */
Lc905642: *stop=0xc905642u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[0],&f->fpscr,&fr[4]);if(status)return status;}goto Lc905644; /* fmul fr0,fr4 */
Lc905644: *stop=0xc905644u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto Lc905646; /* fneg fr1 */
Lc905646: *stop=0xc905646u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[4],&f->fpscr,&fr[1],1);if(status)return status;}goto Lc905648; /* fsub fr4,fr1 */
Lc905648: *stop=0xc905648u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc90564a; /* fadd fr1,fr3 */
Lc90564a: *stop=0xc90564au;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto Lc90564c; /* fmov fr3,@(r0,r5) */
Lc90564c: *stop=0xc90564cu;r[0]=48u;goto Lc90564e; /* mov #0x30,r0 */
Lc90564e: *stop=0xc90564eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]+r[4]);goto Lc905650; /* fmov.s @(r0,r4),fr1 */
Lc905650: *stop=0xc905650u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);goto Lc905652; /* fmov.s @r1,fr3 */
Lc905652: *stop=0xc905652u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc905654; /* fadd fr3,fr1 */
Lc905654: *stop=0xc905654u;fcopy(f,3u,1u);goto Lc905656; /* fmov fr1,fr3 */
Lc905656: *stop=0xc905656u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,1u,r[0]+r[4]);goto Lc905658; /* fmov fr1,@(r0,r4) */
Lc905658: *stop=0xc905658u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0x3f800000u;goto Lc90565a; /* fldi1 fr1 */
Lc90565a: *stop=0xc90565au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[1],&f->fpscr,&fr[1],0);if(status)return status;}goto Lc90565c; /* fadd fr1,fr1 */
Lc90565c: *stop=0xc90565cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto Lc90565e; /* fneg fr3 */
Lc90565e: *stop=0xc90565eu;r[0]=44u;goto Lc905660; /* mov #0x2c,r0 */
Lc905660: *stop=0xc905660u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc905662; /* fadd fr1,fr3 */
Lc905662: *stop=0xc905662u;if(*pr!=entry_pr)return -3;*stop=0xc905664u;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_parameter_update_0c905666(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc905666;
Lc905666: *stop=0xc905666u;r[0]=4294967276u;goto Lc905668; /* mov #-0x14,r0 */
Lc905668: *stop=0xc905668u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto Lc90566a; /* mov.l r14,@-r15 */
Lc90566a: *stop=0xc90566au;r[0]=r[0]&255u;goto Lc90566c; /* extu.b r0,r0 */
Lc90566c: *stop=0xc90566cu;if(!valid_ram(r[0]+r[5],4u))return -3;r[6]=rd(m,r[5]+r[0],4);goto Lc90566e; /* mov.l @(r0,r5),r6 */
Lc90566e: *stop=0xc90566eu;r[0]=0x0c90574cu;goto Lc905670; /* mova 0x0c90574c,r0 */
Lc905670: *stop=0xc905670u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]);goto Lc905672; /* fmov.s @r0,fr4 */
Lc905672: *stop=0xc905672u;r[0]=32u;goto Lc905674; /* mov #0x20,r0 */
Lc905674: *stop=0xc905674u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[6]);goto Lc905676; /* fmov.s @(r0,r6),fr3 */
Lc905676: *stop=0xc905676u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto Lc905678; /* fcmp/gt fr4,fr3 */
Lc905678: take=!(*sr&1u);*stop=0xc90567au;r[7]=0u;if(take)goto Lc90567e;goto Lc90567c; /* bf/s 0x0c90567e */
Lc90567c: *stop=0xc90567cu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[6]);goto Lc90567e; /* fmov fr4,@(r0,r6) */
Lc90567e: *stop=0xc90567eu;r[0]=0x0c905750u;goto Lc905680; /* mova 0x0c905750,r0 */
Lc905680: *stop=0xc905680u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto Lc905682; /* fmov.s @r0,fr5 */
Lc905682: *stop=0xc905682u;r[0]=32u;goto Lc905684; /* mov #0x20,r0 */
Lc905684: *stop=0xc905684u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[6]);goto Lc905686; /* fmov.s @(r0,r6),fr3 */
Lc905686: *stop=0xc905686u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc905688; /* fcmp/gt fr3,fr5 */
Lc905688: if(!(*sr&1u))goto Lc90568c;goto Lc90568a; /* bf 0x0c90568c */
Lc90568a: *stop=0xc90568au;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[6]);goto Lc90568c; /* fmov fr5,@(r0,r6) */
Lc90568c: *stop=0xc90568cu;r[0]=36u;goto Lc90568e; /* mov #0x24,r0 */
Lc90568e: *stop=0xc90568eu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[6]);goto Lc905690; /* fmov.s @(r0,r6),fr3 */
Lc905690: *stop=0xc905690u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto Lc905692; /* fcmp/gt fr4,fr3 */
Lc905692: if(!(*sr&1u))goto Lc905696;goto Lc905694; /* bf 0x0c905696 */
Lc905694: *stop=0xc905694u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[6]);goto Lc905696; /* fmov fr4,@(r0,r6) */
Lc905696: *stop=0xc905696u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[6]);goto Lc905698; /* fmov.s @(r0,r6),fr3 */
Lc905698: *stop=0xc905698u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc90569a; /* fcmp/gt fr3,fr5 */
Lc90569a: if(!(*sr&1u))goto Lc90569e;goto Lc90569c; /* bf 0x0c90569e */
Lc90569c: *stop=0xc90569cu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,5u,r[0]+r[6]);goto Lc90569e; /* fmov fr5,@(r0,r6) */
Lc90569e: *stop=0xc90569eu;r[0]=0x0c905754u;goto Lc9056a0; /* mova 0x0c905754,r0 */
Lc9056a0: *stop=0xc9056a0u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]);goto Lc9056a2; /* fmov.s @r0,fr5 */
Lc9056a2: *stop=0xc9056a2u;r[0]=32u;goto Lc9056a4; /* mov #0x20,r0 */
Lc9056a4: *stop=0xc9056a4u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[6]);goto Lc9056a6; /* fmov.s @(r0,r6),fr3 */
Lc9056a6: *stop=0xc9056a6u;r[0]=4294967260u;goto Lc9056a8; /* mov #-0x24,r0 */
Lc9056a8: *stop=0xc9056a8u;r[0]=r[0]&255u;goto Lc9056aa; /* extu.b r0,r0 */
Lc9056aa: *stop=0xc9056aau;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[5]);goto Lc9056ac; /* fmov.s @(r0,r5),fr2 */
Lc9056ac: *stop=0xc9056acu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc9056ae; /* fcmp/gt fr3,fr2 */
Lc9056ae: take=!(*sr&1u);*stop=0xc9056b0u;r[14]=1u;if(take)goto Lc9056bc;goto Lc9056b2; /* bf/s 0x0c9056bc */
Lc9056b2: *stop=0xc9056b2u;r[0]=32u;goto Lc9056b4; /* mov #0x20,r0 */
Lc9056b4: *stop=0xc9056b4u;r[7]=r[14];goto Lc9056b6; /* mov r14,r7 */
Lc9056b6: *stop=0xc9056b6u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[6]);goto Lc9056b8; /* fmov.s @(r0,r6),fr3 */
Lc9056b8: *stop=0xc9056b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc9056ba; /* fadd fr5,fr3 */
Lc9056ba: *stop=0xc9056bau;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto Lc9056bc; /* fmov fr3,@(r0,r6) */
Lc9056bc: *stop=0xc9056bcu;r[0]=32u;goto Lc9056be; /* mov #0x20,r0 */
Lc9056be: *stop=0xc9056beu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[6]);goto Lc9056c0; /* fmov.s @(r0,r6),fr2 */
Lc9056c0: *stop=0xc9056c0u;r[0]=4294967260u;goto Lc9056c2; /* mov #-0x24,r0 */
Lc9056c2: *stop=0xc9056c2u;r[0]=r[0]&255u;goto Lc9056c4; /* extu.b r0,r0 */
Lc9056c4: *stop=0xc9056c4u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto Lc9056c6; /* fmov.s @(r0,r5),fr3 */
Lc9056c6: *stop=0xc9056c6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc9056c8; /* fcmp/gt fr3,fr2 */
Lc9056c8: if(!(*sr&1u))goto Lc9056d4;goto Lc9056ca; /* bf 0x0c9056d4 */
Lc9056ca: *stop=0xc9056cau;r[0]=32u;goto Lc9056cc; /* mov #0x20,r0 */
Lc9056cc: *stop=0xc9056ccu;r[7]=r[14];goto Lc9056ce; /* mov r14,r7 */
Lc9056ce: *stop=0xc9056ceu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[6]);goto Lc9056d0; /* fmov.s @(r0,r6),fr3 */
Lc9056d0: *stop=0xc9056d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc9056d2; /* fsub fr5,fr3 */
Lc9056d2: *stop=0xc9056d2u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto Lc9056d4; /* fmov fr3,@(r0,r6) */
Lc9056d4: *stop=0xc9056d4u;r[0]=36u;goto Lc9056d6; /* mov #0x24,r0 */
Lc9056d6: *stop=0xc9056d6u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[6]);goto Lc9056d8; /* fmov.s @(r0,r6),fr2 */
Lc9056d8: *stop=0xc9056d8u;r[0]=4294967264u;goto Lc9056da; /* mov #-0x20,r0 */
Lc9056da: *stop=0xc9056dau;r[0]=r[0]&255u;goto Lc9056dc; /* extu.b r0,r0 */
Lc9056dc: *stop=0xc9056dcu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto Lc9056de; /* fmov.s @(r0,r5),fr3 */
Lc9056de: *stop=0xc9056deu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto Lc9056e0; /* fcmp/gt fr2,fr3 */
Lc9056e0: if(!(*sr&1u))goto Lc9056ec;goto Lc9056e2; /* bf 0x0c9056ec */
Lc9056e2: *stop=0xc9056e2u;r[0]=36u;goto Lc9056e4; /* mov #0x24,r0 */
Lc9056e4: *stop=0xc9056e4u;r[7]=r[14];goto Lc9056e6; /* mov r14,r7 */
Lc9056e6: *stop=0xc9056e6u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[6]);goto Lc9056e8; /* fmov.s @(r0,r6),fr3 */
Lc9056e8: *stop=0xc9056e8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc9056ea; /* fadd fr5,fr3 */
Lc9056ea: *stop=0xc9056eau;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto Lc9056ec; /* fmov fr3,@(r0,r6) */
Lc9056ec: *stop=0xc9056ecu;r[0]=36u;goto Lc9056ee; /* mov #0x24,r0 */
Lc9056ee: *stop=0xc9056eeu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[6]);goto Lc9056f0; /* fmov.s @(r0,r6),fr2 */
Lc9056f0: *stop=0xc9056f0u;r[0]=4294967264u;goto Lc9056f2; /* mov #-0x20,r0 */
Lc9056f2: *stop=0xc9056f2u;r[0]=r[0]&255u;goto Lc9056f4; /* extu.b r0,r0 */
Lc9056f4: *stop=0xc9056f4u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto Lc9056f6; /* fmov.s @(r0,r5),fr3 */
Lc9056f6: *stop=0xc9056f6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc9056f8; /* fcmp/gt fr3,fr2 */
Lc9056f8: if(!(*sr&1u))goto Lc905704;goto Lc9056fa; /* bf 0x0c905704 */
Lc9056fa: *stop=0xc9056fau;r[0]=36u;goto Lc9056fc; /* mov #0x24,r0 */
Lc9056fc: *stop=0xc9056fcu;r[7]=r[14];goto Lc9056fe; /* mov r14,r7 */
Lc9056fe: *stop=0xc9056feu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[6]);goto Lc905700; /* fmov.s @(r0,r6),fr3 */
Lc905700: *stop=0xc905700u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc905702; /* fsub fr5,fr3 */
Lc905702: *stop=0xc905702u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto Lc905704; /* fmov fr3,@(r0,r6) */
Lc905704: *stop=0xc905704u;r[0]=54u;goto Lc905706; /* mov #0x36,r0 */
Lc905706: *stop=0xc905706u;if(!valid_ram(r[0]+r[6],2u))return -3;r[3]=sx(m,r[0]+r[6],2);goto Lc905708; /* mov.w @(r0,r6),r3 */
Lc905708: *stop=0xc905708u;r[3]=r[3]&65535u;goto Lc90570a; /* extu.w r3,r3 */
Lc90570a: *stop=0xc90570au;*sr=(*sr&~1u)|!(r[14]&r[3]);goto Lc90570c; /* tst r14,r3 */
Lc90570c: if(!(*sr&1u))goto Lc905712;goto Lc90570e; /* bf 0x0c905712 */
Lc90570e: *stop=0xc905710u;goto Lc90582c; /* bra 0x0c90582c */
Lc905712: *stop=0xc905712u;if(!valid_ram(r[0]+r[6],2u))return -3;r[2]=sx(m,r[0]+r[6],2);goto Lc905714; /* mov.w @(r0,r6),r2 */
Lc905714: *stop=0xc905714u;r[3]=rd(m,0x0c905758u,4);goto Lc905716; /* mov.l 0x0c905758,r3 */
Lc905716: *stop=0xc905716u;r[2]&=r[3];goto Lc905718; /* and r3,r2 */
Lc905718: *stop=0xc905718u;if(!valid_ram(r[0]+r[6],2u))return -3;wr(m,r[0]+r[6],r[2],2);goto Lc90571a; /* mov.w r2,@(r0,r6) */
Lc90571a: *stop=0xc90571au;if(!valid_ram(r[6],1u))return -3;r[1]=sx(m,r[6],1);goto Lc90571c; /* mov.b @r6,r1 */
Lc90571c: *stop=0xc90571cu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto Lc90571e; /* tst r1,r1 */
Lc90571e: if(!(*sr&1u))goto Lc905760;goto Lc905720; /* bf 0x0c905760 */
Lc905720: *stop=0xc905720u;r[1]=rd(m,0x0c90575cu,4);goto Lc905722; /* mov.l 0x0c90575c,r1 */
Lc905722: *stop=0xc905722u;r[0]=4294967268u;goto Lc905724; /* mov #-0x1c,r0 */
Lc905724: *stop=0xc905724u;r[0]=r[0]&255u;goto Lc905726; /* extu.b r0,r0 */
Lc905726: *stop=0xc905726u;*fpul=r[1];goto Lc905728; /* lds r1,FPUL */
Lc905728: *stop=0xc90572au;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc90576a; /* bra 0x0c90576a */
Lc905760: *stop=0xc905760u;r[1]=rd(m,0x0c905880u,4);goto Lc905762; /* mov.l 0x0c905880,r1 */
Lc905762: *stop=0xc905762u;r[0]=4294967268u;goto Lc905764; /* mov #-0x1c,r0 */
Lc905764: *stop=0xc905764u;r[0]=r[0]&255u;goto Lc905766; /* extu.b r0,r0 */
Lc905766: *stop=0xc905766u;*fpul=r[1];goto Lc905768; /* lds r1,FPUL */
Lc905768: *stop=0xc905768u;f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto Lc90576a; /* fsts FPUL,fr3 */
Lc90576a: *stop=0xc90576au;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[5]);goto Lc90576c; /* fmov.s @(r0,r5),fr2 */
Lc90576c: *stop=0xc90576cu;r[3]=84u;goto Lc90576e; /* mov #0x54,r3 */
Lc90576e: *stop=0xc90576eu;r[3]+=r[6];goto Lc905770; /* add r6,r3 */
Lc905770: *stop=0xc905770u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto Lc905772; /* fadd fr3,fr2 */
Lc905772: *stop=0xc905772u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,2u,r[0]+r[5]);goto Lc905774; /* fmov fr2,@(r0,r5) */
Lc905774: *stop=0xc905774u;r[0]=0x0c905884u;goto Lc905776; /* mova 0x0c905884,r0 */
Lc905776: *stop=0xc905776u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]);goto Lc905778; /* fmov.s @r0,fr6 */
Lc905778: *stop=0xc905778u;if(!valid_ram(r[3],2u))return -3;r[3]=sx(m,r[3],2);goto Lc90577a; /* mov.w @r3,r3 */
Lc90577a: *stop=0xc90577au;if(!valid_ram(r[6]+12u,2u))return -3;r[0]=sx(m,r[6]+0xcu,2);goto Lc90577c; /* mov.w @(0xc,r6),r0 */
Lc90577c: *stop=0xc90577cu;*sr=(*sr&~1u)|(r[0]>=r[3]);goto Lc90577e; /* cmp/hs r3,r0 */
Lc90577e: if((*sr&1u))goto Lc905790;goto Lc905780; /* bt 0x0c905790 */
Lc905780: *stop=0xc905780u;if(!valid_ram(r[6]+12u,2u))return -3;r[0]=sx(m,r[6]+0xcu,2);goto Lc905782; /* mov.w @(0xc,r6),r0 */
Lc905782: *stop=0xc905782u;r[0]+=10u;goto Lc905784; /* add #0xa,r0 */
Lc905784: *stop=0xc905784u;if(!valid_ram(r[6]+12u,2u))return -3;wr(m,r[6]+0xcu,r[0],2);goto Lc905786; /* mov.w r0,@(0xc,r6) */
Lc905786: *stop=0xc905786u;r[0]=4294967264u;goto Lc905788; /* mov #-0x20,r0 */
Lc905788: *stop=0xc905788u;r[0]=r[0]&255u;goto Lc90578a; /* extu.b r0,r0 */
Lc90578a: *stop=0xc90578au;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto Lc90578c; /* fmov.s @(r0,r5),fr3 */
Lc90578c: *stop=0xc90578cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc90578e; /* fsub fr6,fr3 */
Lc90578e: *stop=0xc90578eu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto Lc905790; /* fmov fr3,@(r0,r5) */
Lc905790: *stop=0xc905790u;r[3]=86u;goto Lc905792; /* mov #0x56,r3 */
Lc905792: *stop=0xc905792u;if(!valid_ram(r[6]+14u,2u))return -3;r[0]=sx(m,r[6]+0xeu,2);goto Lc905794; /* mov.w @(0xe,r6),r0 */
Lc905794: *stop=0xc905794u;r[3]+=r[6];goto Lc905796; /* add r6,r3 */
Lc905796: *stop=0xc905796u;if(!valid_ram(r[3],2u))return -3;r[3]=sx(m,r[3],2);goto Lc905798; /* mov.w @r3,r3 */
Lc905798: *stop=0xc905798u;*sr=(*sr&~1u)|(r[0]>=r[3]);goto Lc90579a; /* cmp/hs r3,r0 */
Lc90579a: if((*sr&1u))goto Lc9057ac;goto Lc90579c; /* bt 0x0c9057ac */
Lc90579c: *stop=0xc90579cu;if(!valid_ram(r[6]+14u,2u))return -3;r[0]=sx(m,r[6]+0xeu,2);goto Lc90579e; /* mov.w @(0xe,r6),r0 */
Lc90579e: *stop=0xc90579eu;r[0]+=10u;goto Lc9057a0; /* add #0xa,r0 */
Lc9057a0: *stop=0xc9057a0u;if(!valid_ram(r[6]+14u,2u))return -3;wr(m,r[6]+0xeu,r[0],2);goto Lc9057a2; /* mov.w r0,@(0xe,r6) */
Lc9057a2: *stop=0xc9057a2u;r[0]=4294967264u;goto Lc9057a4; /* mov #-0x20,r0 */
Lc9057a4: *stop=0xc9057a4u;r[0]=r[0]&255u;goto Lc9057a6; /* extu.b r0,r0 */
Lc9057a6: *stop=0xc9057a6u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto Lc9057a8; /* fmov.s @(r0,r5),fr3 */
Lc9057a8: *stop=0xc9057a8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc9057aa; /* fadd fr6,fr3 */
Lc9057aa: *stop=0xc9057aau;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto Lc9057ac; /* fmov fr3,@(r0,r5) */
Lc9057ac: *stop=0xc9057acu;r[3]=88u;goto Lc9057ae; /* mov #0x58,r3 */
Lc9057ae: *stop=0xc9057aeu;if(!valid_ram(r[6]+16u,2u))return -3;r[0]=sx(m,r[6]+0x10u,2);goto Lc9057b0; /* mov.w @(0x10,r6),r0 */
Lc9057b0: *stop=0xc9057b0u;r[3]+=r[6];goto Lc9057b2; /* add r6,r3 */
Lc9057b2: *stop=0xc9057b2u;if(!valid_ram(r[3],2u))return -3;r[3]=sx(m,r[3],2);goto Lc9057b4; /* mov.w @r3,r3 */
Lc9057b4: *stop=0xc9057b4u;*sr=(*sr&~1u)|(r[0]>=r[3]);goto Lc9057b6; /* cmp/hs r3,r0 */
Lc9057b6: if((*sr&1u))goto Lc9057c8;goto Lc9057b8; /* bt 0x0c9057c8 */
Lc9057b8: *stop=0xc9057b8u;if(!valid_ram(r[6]+16u,2u))return -3;r[0]=sx(m,r[6]+0x10u,2);goto Lc9057ba; /* mov.w @(0x10,r6),r0 */
Lc9057ba: *stop=0xc9057bau;r[0]+=10u;goto Lc9057bc; /* add #0xa,r0 */
Lc9057bc: *stop=0xc9057bcu;if(!valid_ram(r[6]+16u,2u))return -3;wr(m,r[6]+0x10u,r[0],2);goto Lc9057be; /* mov.w r0,@(0x10,r6) */
Lc9057be: *stop=0xc9057beu;r[0]=4294967260u;goto Lc9057c0; /* mov #-0x24,r0 */
Lc9057c0: *stop=0xc9057c0u;r[0]=r[0]&255u;goto Lc9057c2; /* extu.b r0,r0 */
Lc9057c2: *stop=0xc9057c2u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto Lc9057c4; /* fmov.s @(r0,r5),fr3 */
Lc9057c4: *stop=0xc9057c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc9057c6; /* fadd fr6,fr3 */
Lc9057c6: *stop=0xc9057c6u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto Lc9057c8; /* fmov fr3,@(r0,r5) */
Lc9057c8: *stop=0xc9057c8u;r[3]=90u;goto Lc9057ca; /* mov #0x5a,r3 */
Lc9057ca: *stop=0xc9057cau;if(!valid_ram(r[6]+18u,2u))return -3;r[0]=sx(m,r[6]+0x12u,2);goto Lc9057cc; /* mov.w @(0x12,r6),r0 */
Lc9057cc: *stop=0xc9057ccu;r[3]+=r[6];goto Lc9057ce; /* add r6,r3 */
Lc9057ce: *stop=0xc9057ceu;if(!valid_ram(r[3],2u))return -3;r[3]=sx(m,r[3],2);goto Lc9057d0; /* mov.w @r3,r3 */
Lc9057d0: *stop=0xc9057d0u;*sr=(*sr&~1u)|(r[0]>=r[3]);goto Lc9057d2; /* cmp/hs r3,r0 */
Lc9057d2: if((*sr&1u))goto Lc9057e4;goto Lc9057d4; /* bt 0x0c9057e4 */
Lc9057d4: *stop=0xc9057d4u;if(!valid_ram(r[6]+18u,2u))return -3;r[0]=sx(m,r[6]+0x12u,2);goto Lc9057d6; /* mov.w @(0x12,r6),r0 */
Lc9057d6: *stop=0xc9057d6u;r[0]+=10u;goto Lc9057d8; /* add #0xa,r0 */
Lc9057d8: *stop=0xc9057d8u;if(!valid_ram(r[6]+18u,2u))return -3;wr(m,r[6]+0x12u,r[0],2);goto Lc9057da; /* mov.w r0,@(0x12,r6) */
Lc9057da: *stop=0xc9057dau;r[0]=4294967260u;goto Lc9057dc; /* mov #-0x24,r0 */
Lc9057dc: *stop=0xc9057dcu;r[0]=r[0]&255u;goto Lc9057de; /* extu.b r0,r0 */
Lc9057de: *stop=0xc9057deu;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto Lc9057e0; /* fmov.s @(r0,r5),fr3 */
Lc9057e0: *stop=0xc9057e0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto Lc9057e2; /* fsub fr6,fr3 */
Lc9057e2: *stop=0xc9057e2u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[5]);goto Lc9057e4; /* fmov fr3,@(r0,r5) */
Lc9057e4: *stop=0xc9057e4u;r[0]=84u;goto Lc9057e6; /* mov #0x54,r0 */
Lc9057e6: *stop=0xc9057e6u;if(!valid_ram(r[0]+r[6],2u))return -3;r[3]=sx(m,r[0]+r[6],2);goto Lc9057e8; /* mov.w @(r0,r6),r3 */
Lc9057e8: *stop=0xc9057e8u;if(!valid_ram(r[6]+12u,2u))return -3;r[0]=sx(m,r[6]+0xcu,2);goto Lc9057ea; /* mov.w @(0xc,r6),r0 */
Lc9057ea: *stop=0xc9057eau;*sr=(*sr&~1u)|(r[3]>=r[0]);goto Lc9057ec; /* cmp/hs r0,r3 */
Lc9057ec: if((*sr&1u))goto Lc9057f6;goto Lc9057ee; /* bt 0x0c9057f6 */
Lc9057ee: *stop=0xc9057eeu;r[1]=84u;goto Lc9057f0; /* mov #0x54,r1 */
Lc9057f0: *stop=0xc9057f0u;if(!valid_ram(r[6]+12u,2u))return -3;r[0]=sx(m,r[6]+0xcu,2);goto Lc9057f2; /* mov.w @(0xc,r6),r0 */
Lc9057f2: *stop=0xc9057f2u;r[1]+=r[6];goto Lc9057f4; /* add r6,r1 */
Lc9057f4: *stop=0xc9057f4u;if(!valid_ram(r[1],2u))return -3;wr(m,r[1],r[0],2);goto Lc9057f6; /* mov.w r0,@r1 */
Lc9057f6: *stop=0xc9057f6u;r[0]=86u;goto Lc9057f8; /* mov #0x56,r0 */
Lc9057f8: *stop=0xc9057f8u;if(!valid_ram(r[0]+r[6],2u))return -3;r[3]=sx(m,r[0]+r[6],2);goto Lc9057fa; /* mov.w @(r0,r6),r3 */
Lc9057fa: *stop=0xc9057fau;if(!valid_ram(r[6]+14u,2u))return -3;r[0]=sx(m,r[6]+0xeu,2);goto Lc9057fc; /* mov.w @(0xe,r6),r0 */
Lc9057fc: *stop=0xc9057fcu;*sr=(*sr&~1u)|(r[3]>=r[0]);goto Lc9057fe; /* cmp/hs r0,r3 */
Lc9057fe: if((*sr&1u))goto Lc905808;goto Lc905800; /* bt 0x0c905808 */
Lc905800: *stop=0xc905800u;r[1]=86u;goto Lc905802; /* mov #0x56,r1 */
Lc905802: *stop=0xc905802u;if(!valid_ram(r[6]+14u,2u))return -3;r[0]=sx(m,r[6]+0xeu,2);goto Lc905804; /* mov.w @(0xe,r6),r0 */
Lc905804: *stop=0xc905804u;r[1]+=r[6];goto Lc905806; /* add r6,r1 */
Lc905806: *stop=0xc905806u;if(!valid_ram(r[1],2u))return -3;wr(m,r[1],r[0],2);goto Lc905808; /* mov.w r0,@r1 */
Lc905808: *stop=0xc905808u;r[0]=88u;goto Lc90580a; /* mov #0x58,r0 */
Lc90580a: *stop=0xc90580au;if(!valid_ram(r[0]+r[6],2u))return -3;r[3]=sx(m,r[0]+r[6],2);goto Lc90580c; /* mov.w @(r0,r6),r3 */
Lc90580c: *stop=0xc90580cu;if(!valid_ram(r[6]+16u,2u))return -3;r[0]=sx(m,r[6]+0x10u,2);goto Lc90580e; /* mov.w @(0x10,r6),r0 */
Lc90580e: *stop=0xc90580eu;*sr=(*sr&~1u)|(r[3]>=r[0]);goto Lc905810; /* cmp/hs r0,r3 */
Lc905810: if((*sr&1u))goto Lc90581a;goto Lc905812; /* bt 0x0c90581a */
Lc905812: *stop=0xc905812u;r[1]=88u;goto Lc905814; /* mov #0x58,r1 */
Lc905814: *stop=0xc905814u;if(!valid_ram(r[6]+16u,2u))return -3;r[0]=sx(m,r[6]+0x10u,2);goto Lc905816; /* mov.w @(0x10,r6),r0 */
Lc905816: *stop=0xc905816u;r[1]+=r[6];goto Lc905818; /* add r6,r1 */
Lc905818: *stop=0xc905818u;if(!valid_ram(r[1],2u))return -3;wr(m,r[1],r[0],2);goto Lc90581a; /* mov.w r0,@r1 */
Lc90581a: *stop=0xc90581au;r[0]=90u;goto Lc90581c; /* mov #0x5a,r0 */
Lc90581c: *stop=0xc90581cu;if(!valid_ram(r[0]+r[6],2u))return -3;r[3]=sx(m,r[0]+r[6],2);goto Lc90581e; /* mov.w @(r0,r6),r3 */
Lc90581e: *stop=0xc90581eu;if(!valid_ram(r[6]+18u,2u))return -3;r[0]=sx(m,r[6]+0x12u,2);goto Lc905820; /* mov.w @(0x12,r6),r0 */
Lc905820: *stop=0xc905820u;*sr=(*sr&~1u)|(r[3]>=r[0]);goto Lc905822; /* cmp/hs r0,r3 */
Lc905822: if((*sr&1u))goto Lc90582c;goto Lc905824; /* bt 0x0c90582c */
Lc905824: *stop=0xc905824u;r[1]=90u;goto Lc905826; /* mov #0x5a,r1 */
Lc905826: *stop=0xc905826u;if(!valid_ram(r[6]+18u,2u))return -3;r[0]=sx(m,r[6]+0x12u,2);goto Lc905828; /* mov.w @(0x12,r6),r0 */
Lc905828: *stop=0xc905828u;r[1]+=r[6];goto Lc90582a; /* add r6,r1 */
Lc90582a: *stop=0xc90582au;if(!valid_ram(r[1],2u))return -3;wr(m,r[1],r[0],2);goto Lc90582c; /* mov.w r0,@r1 */
Lc90582c: *stop=0xc90582cu;r[0]=4294967268u;goto Lc90582e; /* mov #-0x1c,r0 */
Lc90582e: *stop=0xc90582eu;r[0]=r[0]&255u;goto Lc905830; /* extu.b r0,r0 */
Lc905830: *stop=0xc905830u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto Lc905832; /* fmov.s @(r0,r5),fr3 */
Lc905832: *stop=0xc905832u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto Lc905834; /* fcmp/gt fr4,fr3 */
Lc905834: if(!(*sr&1u))goto Lc905838;goto Lc905836; /* bf 0x0c905838 */
Lc905836: *stop=0xc905836u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[5]);goto Lc905838; /* fmov fr4,@(r0,r5) */
Lc905838: *stop=0xc905838u;r[0]=40u;goto Lc90583a; /* mov #0x28,r0 */
Lc90583a: *stop=0xc90583au;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[6]);goto Lc90583c; /* fmov.s @(r0,r6),fr3 */
Lc90583c: *stop=0xc90583cu;r[0]=4294967268u;goto Lc90583e; /* mov #-0x1c,r0 */
Lc90583e: *stop=0xc90583eu;r[0]=r[0]&255u;goto Lc905840; /* extu.b r0,r0 */
Lc905840: *stop=0xc905840u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[5]);goto Lc905842; /* fmov.s @(r0,r5),fr2 */
Lc905842: *stop=0xc905842u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto Lc905844; /* fcmp/gt fr3,fr2 */
Lc905844: if(!(*sr&1u))goto Lc905852;goto Lc905846; /* bf 0x0c905852 */
Lc905846: *stop=0xc905846u;r[0]=40u;goto Lc905848; /* mov #0x28,r0 */
Lc905848: *stop=0xc905848u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[6]);goto Lc90584a; /* fmov.s @(r0,r6),fr3 */
Lc90584a: *stop=0xc90584au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],0);if(status)return status;}goto Lc90584c; /* fadd fr5,fr3 */
Lc90584c: *stop=0xc90584cu;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto Lc90584e; /* fmov fr3,@(r0,r6) */
Lc90584e: *stop=0xc905850u;r[7]=r[14];goto Lc905866; /* bra 0x0c905866 */
Lc905852: *stop=0xc905852u;r[0]=40u;goto Lc905854; /* mov #0x28,r0 */
Lc905854: *stop=0xc905854u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[6]);goto Lc905856; /* fmov.s @(r0,r6),fr3 */
Lc905856: *stop=0xc905856u;r[0]=4294967268u;goto Lc905858; /* mov #-0x1c,r0 */
Lc905858: *stop=0xc905858u;r[0]=r[0]&255u;goto Lc90585a; /* extu.b r0,r0 */
Lc90585a: *stop=0xc90585au;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[0]+r[5]);goto Lc90585c; /* fmov.s @(r0,r5),fr2 */
Lc90585c: *stop=0xc90585cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto Lc90585e; /* fcmp/gt fr2,fr3 */
Lc90585e: if(!(*sr&1u))goto Lc905866;goto Lc905860; /* bf 0x0c905866 */
Lc905860: *stop=0xc905860u;if(!valid_ram(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[5]);goto Lc905862; /* fmov.s @(r0,r5),fr3 */
Lc905862: *stop=0xc905862u;r[0]=40u;goto Lc905864; /* mov #0x28,r0 */
Lc905864: *stop=0xc905864u;if(!valid_ram(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[6]);goto Lc905866; /* fmov fr3,@(r0,r6) */
Lc905866: *stop=0xc905866u;*sr=(*sr&~1u)|!(r[7]&r[7]);goto Lc905868; /* tst r7,r7 */
Lc905868: if((*sr&1u))goto Lc90586e;goto Lc90586a; /* bt 0x0c90586e */
Lc90586a: *stop=0xc90586cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0xc904e80u,m,f,sr,r,pr); /* bra 0x0c904e80 */
Lc90586e: *stop=0xc90586eu;if(*pr!=entry_pr)return -3;*stop=0xc905870u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
