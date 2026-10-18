/* Unsigned-label integer-angle interpolation; explicit GBR/queue state.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
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
static uint32_t rd(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,unsigned n){
 if((a&0x1f000000u)!=0x0c000000u){uint32_t value=0;if(!bus||!bus->read){*io_status=-2;return 0;}*io_status=bus->read(bus->context,a,n,&value);return value;}
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
static uint32_t sx(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,unsigned n){
 uint32_t value=rd(m,bus,io_status,a,n);return n==1?(uint32_t)(int32_t)(int8_t)value:(uint32_t)(int32_t)(int16_t)value;
}
static void wr(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,uint32_t a,uint32_t value,unsigned n){
 if((a&0x1f000000u)!=0x0c000000u){if(!bus||!bus->write){*io_status=-2;return;}*io_status=bus->write(bus->context,a,value,n);return;}
 for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(value>>(8*i));
}
static void fload(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  f->bank[bank][reg]=rd(m,bus,io_status,a,4);f->bank[bank][reg+1]=rd(m,bus,io_status,a+4u,4);
 }else f->bank[bank][reg]=rd(m,bus,io_status,a,4);
}
static void fstore(uint8_t *m,const struct sa_vertex_bus *bus,int *io_status,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  wr(m,bus,io_status,a,f->bank[bank][reg],4);wr(m,bus,io_status,a+4u,f->bank[bank][reg+1],4);
 }else wr(m,bus,io_status,a,f->bank[bank][reg],4);
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
static int valid_access(uint32_t a,unsigned n){
 if(valid_ram(a,n))return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_angle_curve_8c609d68(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c609d68;
L8c609d68: *stop=0x8c609d68u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c609d6a; /* mov.l r14,@-r15 */
L8c609d6a: *stop=0x8c609d6au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c609d6c; /* mov.l r13,@-r15 */
L8c609d6c: *stop=0x8c609d6cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c609d6e; /* sts.l PR,@-r15 */
L8c609d6e: *stop=0x8c609d6eu;r[15]+=4294967288u;goto L8c609d70; /* add #-0x8,r15 */
L8c609d70: *stop=0x8c609d70u;r[2]=rd(m,bus,&io_status,0x8c609ec4u,4);if(io_status)return io_status;goto L8c609d72; /* mov.l 0x8c609ec4,r2 */
L8c609d72: *stop=0x8c609d72u;r[13]=r[15];goto L8c609d74; /* mov r15,r13 */
L8c609d74: *stop=0x8c609d74u;r[3]=rd(m,bus,&io_status,0x8c609eb0u,4);if(io_status)return io_status;goto L8c609d76; /* mov.l 0x8c609eb0,r3 */
L8c609d76: *stop=0x8c609d76u;r[1]=r[15];goto L8c609d78; /* mov r15,r1 */
L8c609d78: target=r[3];*pr=0x8c609d7cu;*stop=0x8c609d7au;r[0]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c609d7c; /* jsr @r3 */
L8c609d7c: *stop=0x8c609d7cu;r[14]=r[5];goto L8c609d7e; /* mov r5,r14 */
L8c609d7e: *stop=0x8c609d7eu;r[14]<<=2;goto L8c609d80; /* shll2 r14 */
L8c609d80: *stop=0x8c609d80u;r[1]=r[5];goto L8c609d82; /* mov r5,r1 */
L8c609d82: *stop=0x8c609d82u;r[3]=rd(m,bus,&io_status,0x8c609eb4u,4);if(io_status)return io_status;goto L8c609d84; /* mov.l 0x8c609eb4,r3 */
L8c609d84: *stop=0x8c609d84u;r[14]<<=2;goto L8c609d86; /* shll2 r14 */
L8c609d86: *stop=0x8c609d86u;r[1]+=1u;goto L8c609d88; /* add #0x1,r1 */
L8c609d88: *stop=0x8c609d88u;r[14]+=r[4];goto L8c609d8a; /* add r4,r14 */
L8c609d8a: target=r[3];*pr=0x8c609d8eu;*stop=0x8c609d8cu;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c609d8e; /* jsr @r3 */
L8c609d8e: *stop=0x8c609d8eu;r[0]<<=2;goto L8c609d90; /* shll2 r0 */
L8c609d90: *stop=0x8c609d90u;if(!valid_access(r[14],4u))return -3;r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c609d92; /* mov.l @r14,r3 */
L8c609d92: *stop=0x8c609d92u;r[0]<<=2;goto L8c609d94; /* shll2 r0 */
L8c609d94: *stop=0x8c609d94u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c609d96; /* fldi0 fr2 */
L8c609d96: *stop=0x8c609d96u;r[4]+=r[0];goto L8c609d98; /* add r0,r4 */
L8c609d98: *stop=0x8c609d98u;if(!valid_access(r[4],4u))return -3;r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c609d9a; /* mov.l @r4,r1 */
L8c609d9a: *stop=0x8c609d9au;r[1]-=r[3];goto L8c609d9c; /* sub r3,r1 */
L8c609d9c: *stop=0x8c609d9cu;*fpul=r[1];goto L8c609d9e; /* lds r1,FPUL */
L8c609d9e: *stop=0x8c609d9eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c609da0; /* float FPUL,fr3 */
L8c609da0: *stop=0x8c609da0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c609da2; /* fcmp/gt fr3,fr2 */
L8c609da2: take=!(*sr&1u);*stop=0x8c609da4u;fcopy(f,4u,3u);if(take)goto L8c609db0;goto L8c609da6; /* bf/s 0x8c609db0 */
L8c609da6: *stop=0x8c609da6u;r[0]=0x8c609eb8u;goto L8c609da8; /* mova 0x8c609eb8,r0 */
L8c609da8: *stop=0x8c609da8u;fcopy(f,3u,4u);goto L8c609daa; /* fmov fr4,fr3 */
L8c609daa: *stop=0x8c609daau;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;goto L8c609dac; /* fmov.s @r0,fr1 */
L8c609dac: *stop=0x8c609daeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c609db2; /* bra 0x8c609db2 */
L8c609db0: *stop=0x8c609db0u;fcopy(f,3u,4u);goto L8c609db2; /* fmov fr4,fr3 */
L8c609db2: *stop=0x8c609db2u;r[1]=rd(m,bus,&io_status,0x8c609ebcu,4);if(io_status)return io_status;goto L8c609db4; /* mov.l 0x8c609ebc,r1 */
L8c609db4: *stop=0x8c609db4u;r[3]=28u;goto L8c609db6; /* mov #0x1c,r3 */
L8c609db6: *stop=0x8c609db6u;r[0]=4u;goto L8c609db8; /* mov #0x4,r0 */
L8c609db8: *stop=0x8c609db8u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c609dba; /* fmov fr3,@(r0,r13) */
L8c609dba: *stop=0x8c609dbau;if(!valid_access(r[1],2u))return -3;r[2]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;goto L8c609dbc; /* mov.w @r1,r2 */
L8c609dbc: *stop=0x8c609dbcu;if(!valid_access(r[14],4u))return -3;r[5]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c609dbe; /* mov.l @r14,r5 */
L8c609dbe: *stop=0x8c609dbeu;*macl=(r[3]&65535u)*(r[2]&65535u);goto L8c609dc0; /* mulu.w r3,r2 */
L8c609dc0: *stop=0x8c609dc0u;*fpul=r[5];goto L8c609dc2; /* lds r5,FPUL */
L8c609dc2: *stop=0x8c609dc2u;*sr=(*sr&~1u)|((int32_t)r[5]>=0);goto L8c609dc4; /* cmp/pz r5 */
L8c609dc4: *stop=0x8c609dc4u;r[3]=*macl;goto L8c609dc6; /* sts MACL,r3 */
L8c609dc6: take=(*sr&1u);*stop=0x8c609dc8u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c609dd2;goto L8c609dca; /* bt/s 0x8c609dd2 */
L8c609dca: *stop=0x8c609dcau;r[2]=rd(m,bus,&io_status,0x8c609eb8u,4);if(io_status)return io_status;goto L8c609dcc; /* mov.l 0x8c609eb8,r2 */
L8c609dcc: *stop=0x8c609dccu;*fpul=r[2];goto L8c609dce; /* lds r2,FPUL */
L8c609dce: *stop=0x8c609dceu;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c609dd0; /* fsts FPUL,fr2 */
L8c609dd0: *stop=0x8c609dd0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c609dd2; /* fadd fr2,fr3 */
L8c609dd2: *stop=0x8c609dd2u;r[0]=rd(m,bus,&io_status,0x8c609ec0u,4);if(io_status)return io_status;goto L8c609dd4; /* mov.l 0x8c609ec0,r0 */
L8c609dd4: *stop=0x8c609dd4u;if(!valid_access(r[4]+4u,4u))return -3;r[2]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c609dd6; /* mov.l @(0x4,r4),r2 */
L8c609dd6: *stop=0x8c609dd6u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[3]);if(io_status)return io_status;goto L8c609dd8; /* fmov.s @(r0,r3),fr1 */
L8c609dd8: *stop=0x8c609dd8u;if(!valid_access(r[4],4u))return -3;r[0]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c609dda; /* mov.l @r4,r0 */
L8c609dda: *stop=0x8c609ddau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c609ddc; /* fsub fr3,fr1 */
L8c609ddc: *stop=0x8c609ddcu;if(!valid_access(r[14]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;goto L8c609dde; /* mov.l @(0x4,r14),r3 */
L8c609dde: *stop=0x8c609ddeu;r[0]-=r[5];goto L8c609de0; /* sub r5,r0 */
L8c609de0: *stop=0x8c609de0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c609de2; /* fldi1 fr5 */
L8c609de2: *stop=0x8c609de2u;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c609de4; /* cmp/pl r0 */
L8c609de4: *stop=0x8c609de4u;*fpul=r[3];goto L8c609de6; /* lds r3,FPUL */
L8c609de6: *stop=0x8c609de6u;r[0]=*sr&1u;goto L8c609de8; /* movt r0 */
L8c609de8: *stop=0x8c609de8u;r[0]<<=2;goto L8c609dea; /* shll2 r0 */
L8c609dea: *stop=0x8c609deau;fcopy(f,4u,1u);goto L8c609dec; /* fmov fr1,fr4 */
L8c609dec: *stop=0x8c609decu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c609dee; /* fmov.s @(r0,r13),fr3 */
L8c609dee: *stop=0x8c609deeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c609df0; /* fdiv fr3,fr4 */
L8c609df0: *stop=0x8c609df0u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c609df2; /* float FPUL,fr3 */
L8c609df2: *stop=0x8c609df2u;*fpul=r[2];goto L8c609df4; /* lds r2,FPUL */
L8c609df4: *stop=0x8c609df4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c609df6; /* float FPUL,fr2 */
L8c609df6: *stop=0x8c609df6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[4],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c609df8; /* fsub fr4,fr5 */
L8c609df8: *stop=0x8c609df8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c609dfa; /* fmul fr4,fr2 */
L8c609dfa: *stop=0x8c609dfau;fcopy(f,0u,5u);goto L8c609dfc; /* fmov fr5,fr0 */
L8c609dfc: *stop=0x8c609dfcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c609dfe; /* fmac fr0,fr3,fr2 */
L8c609dfe: *stop=0x8c609dfeu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c609e00; /* ftrc fr2,FPUL */
L8c609e00: *stop=0x8c609e00u;r[3]=*fpul;goto L8c609e02; /* sts FPUL,r3 */
L8c609e02: *stop=0x8c609e02u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[3],4);if(io_status)return io_status;goto L8c609e04; /* mov.l r3,@r7 */
L8c609e04: *stop=0x8c609e04u;if(!valid_access(r[14]+8u,4u))return -3;r[2]=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;goto L8c609e06; /* mov.l @(0x8,r14),r2 */
L8c609e06: *stop=0x8c609e06u;if(!valid_access(r[4]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;goto L8c609e08; /* mov.l @(0x8,r4),r3 */
L8c609e08: *stop=0x8c609e08u;*fpul=r[2];goto L8c609e0a; /* lds r2,FPUL */
L8c609e0a: *stop=0x8c609e0au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c609e0c; /* float FPUL,fr3 */
L8c609e0c: *stop=0x8c609e0cu;*fpul=r[3];goto L8c609e0e; /* lds r3,FPUL */
L8c609e0e: *stop=0x8c609e0eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c609e10; /* float FPUL,fr2 */
L8c609e10: *stop=0x8c609e10u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c609e12; /* fmul fr4,fr2 */
L8c609e12: *stop=0x8c609e12u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c609e14; /* fmac fr0,fr3,fr2 */
L8c609e14: *stop=0x8c609e14u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c609e16; /* ftrc fr2,FPUL */
L8c609e16: *stop=0x8c609e16u;r[3]=*fpul;goto L8c609e18; /* sts FPUL,r3 */
L8c609e18: *stop=0x8c609e18u;if(!valid_access(r[7]+4u,4u))return -3;wr(m,bus,&io_status,r[7]+0x4u,r[3],4);if(io_status)return io_status;goto L8c609e1a; /* mov.l r3,@(0x4,r7) */
L8c609e1a: *stop=0x8c609e1au;if(!valid_access(r[14]+12u,4u))return -3;r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c609e1c; /* mov.l @(0xc,r14),r2 */
L8c609e1c: *stop=0x8c609e1cu;if(!valid_access(r[4]+12u,4u))return -3;r[3]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c609e1e; /* mov.l @(0xc,r4),r3 */
L8c609e1e: *stop=0x8c609e1eu;*fpul=r[2];goto L8c609e20; /* lds r2,FPUL */
L8c609e20: *stop=0x8c609e20u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c609e22; /* float FPUL,fr3 */
L8c609e22: *stop=0x8c609e22u;*fpul=r[3];goto L8c609e24; /* lds r3,FPUL */
L8c609e24: *stop=0x8c609e24u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c609e26; /* float FPUL,fr2 */
L8c609e26: *stop=0x8c609e26u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c609e28; /* fmul fr4,fr2 */
L8c609e28: *stop=0x8c609e28u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c609e2a; /* fmac fr0,fr3,fr2 */
L8c609e2a: *stop=0x8c609e2au;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c609e2c; /* ftrc fr2,FPUL */
L8c609e2c: *stop=0x8c609e2cu;r[3]=*fpul;goto L8c609e2e; /* sts FPUL,r3 */
L8c609e2e: *stop=0x8c609e2eu;if(!valid_access(r[7]+8u,4u))return -3;wr(m,bus,&io_status,r[7]+0x8u,r[3],4);if(io_status)return io_status;goto L8c609e30; /* mov.l r3,@(0x8,r7) */
L8c609e30: *stop=0x8c609e30u;r[15]+=8u;goto L8c609e32; /* add #0x8,r15 */
L8c609e32: *stop=0x8c609e32u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c609e34; /* lds.l @r15+,PR */
L8c609e34: *stop=0x8c609e34u;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c609e36; /* mov.l @r15+,r13 */
L8c609e36: *stop=0x8c609e36u;if(*pr!=entry_pr)return -3;*stop=0x8c609e38u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
