/* Clipping proposals with integer banks and MACH; explicit GBR/queue state.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
#include "sh4_integer_state.h"
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
SA_STAGE_EXPORT int sa_clipping_state_8c037ee0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,uint32_t entry,uint32_t checkpoint){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)checkpoint;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 switch(entry){case 0x8c037ee0u:goto L8c037ee0;case 0x8c037ee4u:goto L8c037ee4;case 0x8c037fceu:goto L8c037fce;default:return -2;}
L8c037ee0: *stop=0x8c037ee0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c037ee2; /* mov.l r12,@-r15 */
L8c037ee2: *stop=0x8c037ee2u;r[12]=r[1];goto L8c037ee4; /* mov r1,r12 */
L8c037ee4: *stop=0x8c037ee4u;r[1]=32u;goto L8c037ee6; /* mov #0x20,r1 */
L8c037ee6: *stop=0x8c037ee6u;r[0]=24u;goto L8c037ee8; /* mov #0x18,r0 */
L8c037ee8: *stop=0x8c037ee8u;r[1]=(r[0]&0x80000000u)?((r[0]&31u)?r[1]>>((0u-r[0])&31u):0u):(r[1]<<(r[0]&31u));goto L8c037eea; /* shld r0,r1 */
L8c037eea: *stop=0x8c037eeau;r[1]=~r[1];goto L8c037eec; /* not r1,r1 */
L8c037eec: *stop=0x8c037eecu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c037eee; /* stc SR,r0 */
L8c037eee: *stop=0x8c037eeeu;r[0]&=r[1];goto L8c037ef0; /* and r1,r0 */
L8c037ef0: *stop=0x8c037ef0u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;if(checkpoint==0x8c037ef0u){*stop=0x8c037ef2u;return 2;}goto L8c037ef2; /* ldc r0,SR */
L8c037ef2: *stop=0x8c037ef2u;r[0]=rd(m,bus,&io_status,0x8c037fe8u,4);if(io_status)return io_status;goto L8c037ef4; /* mov.l 0x8c037fe8,r0 */
L8c037ef4: *stop=0x8c037ef4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][9]=0x3f800000u;goto L8c037ef6; /* fldi1 fr9 */
L8c037ef6: *stop=0x8c037ef6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]=0x3f800000u;goto L8c037ef8; /* fldi1 fr10 */
L8c037ef8: *stop=0x8c037ef8u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037efa; /* fmov.s @r0+,fr14 */
L8c037efa: *stop=0x8c037efau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[9],&fr[14],&f->fpscr,&fr[9]);if(status)return status;}goto L8c037efc; /* fdiv fr14,fr9 */
L8c037efc: *stop=0x8c037efcu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037efe; /* fmov.s @r0+,fr15 */
L8c037efe: *stop=0x8c037efeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[10],&fr[15],&f->fpscr,&fr[10]);if(status)return status;}goto L8c037f00; /* fdiv fr15,fr10 */
L8c037f00: *stop=0x8c037f00u;r[0]=rd(m,bus,&io_status,0x8c037fecu,4);if(io_status)return io_status;goto L8c037f02; /* mov.l 0x8c037fec,r0 */
L8c037f02: *stop=0x8c037f02u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037f04; /* fmov.s @r0+,fr12 */
L8c037f04: *stop=0x8c037f04u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037f06; /* fmov.s @r0+,fr13 */
L8c037f06: *stop=0x8c037f06u;r[1]=rd(m,bus,&io_status,0x8c037ff0u,4);if(io_status)return io_status;goto L8c037f08; /* mov.l 0x8c037ff0,r1 */
L8c037f08: *stop=0x8c037f08u;if(!valid_access(r[11],2u))return -3;r[0]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c037f0a; /* mov.w @r11+,r0 */
L8c037f0a: *stop=0x8c037f0au;*fpul=r[0];goto L8c037f0c; /* lds r0,FPUL */
L8c037f0c: *stop=0x8c037f0cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c037f0e; /* float FPUL,fr3 */
L8c037f0e: *stop=0x8c037f0eu;if(!valid_access(r[11],2u))return -3;r[0]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c037f10; /* mov.w @r11+,r0 */
L8c037f10: *stop=0x8c037f10u;*fpul=r[0];goto L8c037f12; /* lds r0,FPUL */
L8c037f12: *stop=0x8c037f12u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][4]);if(status)return status;goto L8c037f14; /* float FPUL,fr4 */
L8c037f14: *stop=0x8c037f14u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c037f16; /* fmov.s fr4,@r1 */
L8c037f16: *stop=0x8c037f16u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c037f18; /* fmov.s fr3,@-r1 */
L8c037f18: *stop=0x8c037f18u;if(!valid_ram(*gbr+0x48u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x48u,4);if(io_status)return io_status;goto L8c037f1a; /* mov.l @(0x48,GBR),r0 */
L8c037f1a: *stop=0x8c037f1au;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037f1c; /* mov.l r0,@-r1 */
L8c037f1c: *stop=0x8c037f1cu;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037f1e; /* fmov.s @r8+,fr3 */
L8c037f1e: *stop=0x8c037f1eu;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037f20; /* fmov.s @r8+,fr4 */
L8c037f20: *stop=0x8c037f20u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037f22; /* fmov.s @r8+,fr5 */
L8c037f22: *stop=0x8c037f22u;if(!valid_access(r[8],4u))return -3;r[0]=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[8]+=4;goto L8c037f24; /* mov.l @r8+,r0 */
L8c037f24: *stop=0x8c037f24u;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037f26; /* mov.l r0,@-r1 */
L8c037f26: *stop=0x8c037f26u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c037f28; /* fmov.s fr3,@-r1 */
L8c037f28: *stop=0x8c037f28u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c037f2a; /* fmov.s fr4,@-r1 */
L8c037f2a: *stop=0x8c037f2au;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;goto L8c037f2c; /* fmov.s fr5,@-r1 */
L8c037f2c: *stop=0x8c037f2cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c037f2e; /* fldi1 fr6 */
L8c037f2e: *stop=0x8c037f2eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[5],&f->fpscr,&fr[6]);if(status)return status;}goto L8c037f30; /* fdiv fr5,fr6 */
L8c037f30: *stop=0x8c037f30u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[12],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c037f32; /* fsub fr12,fr3 */
L8c037f32: *stop=0x8c037f32u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c037f34; /* fmul fr6,fr3 */
L8c037f34: *stop=0x8c037f34u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c037f36; /* fmul fr9,fr3 */
L8c037f36: *stop=0x8c037f36u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c037f38; /* fmov.s fr3,@-r1 */
L8c037f38: *stop=0x8c037f38u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[13],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c037f3a; /* fsub fr13,fr4 */
L8c037f3a: *stop=0x8c037f3au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[6],&f->fpscr,&fr[4]);if(status)return status;}goto L8c037f3c; /* fmul fr6,fr4 */
L8c037f3c: *stop=0x8c037f3cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c037f3e; /* fmul fr10,fr4 */
L8c037f3e: *stop=0x8c037f3eu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c037f40; /* fmov.s fr4,@-r1 */
L8c037f40: *stop=0x8c037f40u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[1]);if(io_status)return io_status;goto L8c037f42; /* fmov.s fr6,@-r1 */
L8c037f42: *stop=0x8c037f42u;r[8]+=4294967280u;goto L8c037f44; /* add #-0x10,r8 */
L8c037f44: *stop=0x8c037f44u;r[1]+=76u;goto L8c037f46; /* add #0x4c,r1 */
L8c037f46: *stop=0x8c037f46u;if(!valid_access(r[11],2u))return -3;r[0]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c037f48; /* mov.w @r11+,r0 */
L8c037f48: *stop=0x8c037f48u;*fpul=r[0];goto L8c037f4a; /* lds r0,FPUL */
L8c037f4a: *stop=0x8c037f4au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c037f4c; /* float FPUL,fr3 */
L8c037f4c: *stop=0x8c037f4cu;if(!valid_access(r[11],2u))return -3;r[0]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c037f4e; /* mov.w @r11+,r0 */
L8c037f4e: *stop=0x8c037f4eu;*fpul=r[0];goto L8c037f50; /* lds r0,FPUL */
L8c037f50: *stop=0x8c037f50u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][4]);if(status)return status;goto L8c037f52; /* float FPUL,fr4 */
L8c037f52: *stop=0x8c037f52u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c037f54; /* fmov.s fr4,@r1 */
L8c037f54: *stop=0x8c037f54u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c037f56; /* fmov.s fr3,@-r1 */
L8c037f56: *stop=0x8c037f56u;if(!valid_ram(*gbr+0x4cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x4cu,4);if(io_status)return io_status;goto L8c037f58; /* mov.l @(0x4c,GBR),r0 */
L8c037f58: *stop=0x8c037f58u;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037f5a; /* mov.l r0,@-r1 */
L8c037f5a: *stop=0x8c037f5au;if(!valid_access(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[9]);if(io_status)return io_status;r[9]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037f5c; /* fmov.s @r9+,fr3 */
L8c037f5c: *stop=0x8c037f5cu;if(!valid_access(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[9]);if(io_status)return io_status;r[9]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037f5e; /* fmov.s @r9+,fr4 */
L8c037f5e: *stop=0x8c037f5eu;if(!valid_access(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[9]);if(io_status)return io_status;r[9]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037f60; /* fmov.s @r9+,fr5 */
L8c037f60: *stop=0x8c037f60u;if(!valid_access(r[9],4u))return -3;r[0]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[9]+=4;goto L8c037f62; /* mov.l @r9+,r0 */
L8c037f62: *stop=0x8c037f62u;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037f64; /* mov.l r0,@-r1 */
L8c037f64: *stop=0x8c037f64u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c037f66; /* fmov.s fr3,@-r1 */
L8c037f66: *stop=0x8c037f66u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c037f68; /* fmov.s fr4,@-r1 */
L8c037f68: *stop=0x8c037f68u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;goto L8c037f6a; /* fmov.s fr5,@-r1 */
L8c037f6a: *stop=0x8c037f6au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c037f6c; /* fldi1 fr7 */
L8c037f6c: *stop=0x8c037f6cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[7],&fr[5],&f->fpscr,&fr[7]);if(status)return status;}goto L8c037f6e; /* fdiv fr5,fr7 */
L8c037f6e: *stop=0x8c037f6eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[12],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c037f70; /* fsub fr12,fr3 */
L8c037f70: *stop=0x8c037f70u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c037f72; /* fmul fr7,fr3 */
L8c037f72: *stop=0x8c037f72u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c037f74; /* fmul fr9,fr3 */
L8c037f74: *stop=0x8c037f74u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c037f76; /* fmov.s fr3,@-r1 */
L8c037f76: *stop=0x8c037f76u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[13],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c037f78; /* fsub fr13,fr4 */
L8c037f78: *stop=0x8c037f78u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[7],&f->fpscr,&fr[4]);if(status)return status;}goto L8c037f7a; /* fmul fr7,fr4 */
L8c037f7a: *stop=0x8c037f7au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c037f7c; /* fmul fr10,fr4 */
L8c037f7c: *stop=0x8c037f7cu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c037f7e; /* fmov.s fr4,@-r1 */
L8c037f7e: *stop=0x8c037f7eu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[1]);if(io_status)return io_status;goto L8c037f80; /* fmov.s fr7,@-r1 */
L8c037f80: *stop=0x8c037f80u;r[9]+=4294967280u;goto L8c037f82; /* add #-0x10,r9 */
L8c037f82: *stop=0x8c037f82u;r[1]+=76u;goto L8c037f84; /* add #0x4c,r1 */
L8c037f84: *stop=0x8c037f84u;if(!valid_access(r[11],2u))return -3;r[0]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c037f86; /* mov.w @r11+,r0 */
L8c037f86: *stop=0x8c037f86u;*fpul=r[0];goto L8c037f88; /* lds r0,FPUL */
L8c037f88: *stop=0x8c037f88u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c037f8a; /* float FPUL,fr3 */
L8c037f8a: *stop=0x8c037f8au;if(!valid_access(r[11],2u))return -3;r[0]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c037f8c; /* mov.w @r11+,r0 */
L8c037f8c: *stop=0x8c037f8cu;*fpul=r[0];goto L8c037f8e; /* lds r0,FPUL */
L8c037f8e: *stop=0x8c037f8eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][4]);if(status)return status;goto L8c037f90; /* float FPUL,fr4 */
L8c037f90: *stop=0x8c037f90u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c037f92; /* fmov.s fr4,@r1 */
L8c037f92: *stop=0x8c037f92u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c037f94; /* fmov.s fr3,@-r1 */
L8c037f94: *stop=0x8c037f94u;if(!valid_ram(*gbr+0x50u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x50u,4);if(io_status)return io_status;goto L8c037f96; /* mov.l @(0x50,GBR),r0 */
L8c037f96: *stop=0x8c037f96u;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037f98; /* mov.l r0,@-r1 */
L8c037f98: *stop=0x8c037f98u;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[10]);if(io_status)return io_status;r[10]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037f9a; /* fmov.s @r10+,fr3 */
L8c037f9a: *stop=0x8c037f9au;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[10]);if(io_status)return io_status;r[10]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037f9c; /* fmov.s @r10+,fr4 */
L8c037f9c: *stop=0x8c037f9cu;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[10]);if(io_status)return io_status;r[10]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037f9e; /* fmov.s @r10+,fr5 */
L8c037f9e: *stop=0x8c037f9eu;if(!valid_access(r[10],4u))return -3;r[0]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[10]+=4;goto L8c037fa0; /* mov.l @r10+,r0 */
L8c037fa0: *stop=0x8c037fa0u;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037fa2; /* mov.l r0,@-r1 */
L8c037fa2: *stop=0x8c037fa2u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c037fa4; /* fmov.s fr3,@-r1 */
L8c037fa4: *stop=0x8c037fa4u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c037fa6; /* fmov.s fr4,@-r1 */
L8c037fa6: *stop=0x8c037fa6u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;goto L8c037fa8; /* fmov.s fr5,@-r1 */
L8c037fa8: *stop=0x8c037fa8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][8]=0x3f800000u;goto L8c037faa; /* fldi1 fr8 */
L8c037faa: *stop=0x8c037faau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c037fac; /* fdiv fr5,fr8 */
L8c037fac: *stop=0x8c037facu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[12],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c037fae; /* fsub fr12,fr3 */
L8c037fae: *stop=0x8c037faeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c037fb0; /* fmul fr8,fr3 */
L8c037fb0: *stop=0x8c037fb0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c037fb2; /* fmul fr9,fr3 */
L8c037fb2: *stop=0x8c037fb2u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c037fb4; /* fmov.s fr3,@-r1 */
L8c037fb4: *stop=0x8c037fb4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[13],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c037fb6; /* fsub fr13,fr4 */
L8c037fb6: *stop=0x8c037fb6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[8],&f->fpscr,&fr[4]);if(status)return status;}goto L8c037fb8; /* fmul fr8,fr4 */
L8c037fb8: *stop=0x8c037fb8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c037fba; /* fmul fr10,fr4 */
L8c037fba: *stop=0x8c037fbau;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c037fbc; /* fmov.s fr4,@-r1 */
L8c037fbc: *stop=0x8c037fbcu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[1]);if(io_status)return io_status;goto L8c037fbe; /* fmov.s fr8,@-r1 */
L8c037fbe: *stop=0x8c037fbeu;r[10]+=4294967280u;goto L8c037fc0; /* add #-0x10,r10 */
L8c037fc0: *stop=0x8c037fc0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0x3f800000u;goto L8c037fc2; /* fldi1 fr0 */
L8c037fc2: *stop=0x8c037fc2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][6],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c037fc4; /* fcmp/gt fr0,fr6 */
L8c037fc4: if((*sr&1u)){if(!call)return -2;return call(ctx,0x8c037ff4u,m,f,sr,r,pr);}goto L8c037fc6; /* bt 0x8c037ff4 */
L8c037fc6: *stop=0x8c037fc6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c037fc8; /* fcmp/gt fr0,fr7 */
L8c037fc8: if((*sr&1u)){if(!call)return -2;return call(ctx,0x8c037ff4u,m,f,sr,r,pr);}goto L8c037fca; /* bt 0x8c037ff4 */
L8c037fca: *stop=0x8c037fcau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][8],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c037fcc; /* fcmp/gt fr0,fr8 */
L8c037fcc: if((*sr&1u)){if(!call)return -2;return call(ctx,0x8c037ff4u,m,f,sr,r,pr);}goto L8c037fce; /* bt 0x8c037ff4 */
L8c037fce: *stop=0x8c037fceu;r[1]=32u;goto L8c037fd0; /* mov #0x20,r1 */
L8c037fd0: *stop=0x8c037fd0u;r[0]=24u;goto L8c037fd2; /* mov #0x18,r0 */
L8c037fd2: *stop=0x8c037fd2u;r[1]=(r[0]&0x80000000u)?((r[0]&31u)?r[1]>>((0u-r[0])&31u):0u):(r[1]<<(r[0]&31u));goto L8c037fd4; /* shld r0,r1 */
L8c037fd4: *stop=0x8c037fd4u;r[1]=~r[1];goto L8c037fd6; /* not r1,r1 */
L8c037fd6: *stop=0x8c037fd6u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c037fd8; /* stc SR,r0 */
L8c037fd8: *stop=0x8c037fd8u;r[0]&=r[1];goto L8c037fda; /* and r1,r0 */
L8c037fda: *stop=0x8c037fdau;r[1]=~r[1];goto L8c037fdc; /* not r1,r1 */
L8c037fdc: *stop=0x8c037fdcu;r[0]|=r[1];goto L8c037fde; /* or r1,r0 */
L8c037fde: *stop=0x8c037fdeu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;if(checkpoint==0x8c037fdeu){*stop=0x8c037fe0u;return 2;}goto L8c037fe0; /* ldc r0,SR */
L8c037fe0: *stop=0x8c037fe0u;r[1]=r[12];goto L8c037fe2; /* mov r12,r1 */
L8c037fe2: *stop=0x8c037fe2u;if(*pr!=entry_pr)return -3;*stop=0x8c037fe4u;if(!valid_access(r[15],4u))return -3;r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_clipping_state_8c038802(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,uint32_t entry,uint32_t checkpoint){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)checkpoint;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 switch(entry){case 0x8c038802u:goto L8c038802;case 0x8c038804u:goto L8c038804;case 0x8c0388beu:goto L8c0388be;case 0x8c038962u:goto L8c038962;case 0x8c03897au:goto L8c03897a;case 0x8c038996u:goto L8c038996;case 0x8c0389b2u:goto L8c0389b2;case 0x8c0389d4u:goto L8c0389d4;case 0x8c0389ecu:goto L8c0389ec;case 0x8c038a08u:goto L8c038a08;case 0x8c038a24u:goto L8c038a24;case 0x8c038b14u:goto L8c038b14;default:return -2;}
L8c038802: *stop=0x8c038802u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c038804; /* mov.l r1,@-r15 */
L8c038804: *stop=0x8c038804u;r[1]=32u;goto L8c038806; /* mov #0x20,r1 */
L8c038806: *stop=0x8c038806u;r[0]=24u;goto L8c038808; /* mov #0x18,r0 */
L8c038808: *stop=0x8c038808u;r[1]=(r[0]&0x80000000u)?((r[0]&31u)?r[1]>>((0u-r[0])&31u):0u):(r[1]<<(r[0]&31u));goto L8c03880a; /* shld r0,r1 */
L8c03880a: *stop=0x8c03880au;r[1]=~r[1];goto L8c03880c; /* not r1,r1 */
L8c03880c: *stop=0x8c03880cu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c03880e; /* stc SR,r0 */
L8c03880e: *stop=0x8c03880eu;r[0]&=r[1];goto L8c038810; /* and r1,r0 */
L8c038810: *stop=0x8c038810u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;if(checkpoint==0x8c038810u){*stop=0x8c038812u;return 2;}goto L8c038812; /* ldc r0,SR */
L8c038812: *stop=0x8c038812u;r[0]=rd(m,bus,&io_status,0x8c0388d4u,4);if(io_status)return io_status;goto L8c038814; /* mov.l 0x8c0388d4,r0 */
L8c038814: *stop=0x8c038814u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][9]=0x3f800000u;goto L8c038816; /* fldi1 fr9 */
L8c038816: *stop=0x8c038816u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]=0x3f800000u;goto L8c038818; /* fldi1 fr10 */
L8c038818: *stop=0x8c038818u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03881a; /* fmov.s @r0+,fr14 */
L8c03881a: *stop=0x8c03881au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[9],&fr[14],&f->fpscr,&fr[9]);if(status)return status;}goto L8c03881c; /* fdiv fr14,fr9 */
L8c03881c: *stop=0x8c03881cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03881e; /* fmov.s @r0+,fr15 */
L8c03881e: *stop=0x8c03881eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[10],&fr[15],&f->fpscr,&fr[10]);if(status)return status;}goto L8c038820; /* fdiv fr15,fr10 */
L8c038820: *stop=0x8c038820u;r[0]=rd(m,bus,&io_status,0x8c0388d8u,4);if(io_status)return io_status;goto L8c038822; /* mov.l 0x8c0388d8,r0 */
L8c038822: *stop=0x8c038822u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038824; /* fmov.s @r0+,fr12 */
L8c038824: *stop=0x8c038824u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038826; /* fmov.s @r0+,fr13 */
L8c038826: *stop=0x8c038826u;r[1]=rd(m,bus,&io_status,0x8c0388dcu,4);if(io_status)return io_status;goto L8c038828; /* mov.l 0x8c0388dc,r1 */
L8c038828: *stop=0x8c038828u;if(!valid_ram(*gbr+0x48u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x48u,4);if(io_status)return io_status;goto L8c03882a; /* mov.l @(0x48,GBR),r0 */
L8c03882a: *stop=0x8c03882au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c03882c; /* mov.l r0,@r1 */
L8c03882c: *stop=0x8c03882cu;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03882e; /* fmov.s @r8+,fr3 */
L8c03882e: *stop=0x8c03882eu;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038830; /* fmov.s @r8+,fr4 */
L8c038830: *stop=0x8c038830u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038832; /* fmov.s @r8+,fr5 */
L8c038832: *stop=0x8c038832u;if(!valid_access(r[8],4u))return -3;r[0]=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[8]+=4;goto L8c038834; /* mov.l @r8+,r0 */
L8c038834: *stop=0x8c038834u;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c038836; /* mov.l r0,@-r1 */
L8c038836: *stop=0x8c038836u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c038838; /* fmov.s fr3,@-r1 */
L8c038838: *stop=0x8c038838u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c03883a; /* fmov.s fr4,@-r1 */
L8c03883a: *stop=0x8c03883au;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;goto L8c03883c; /* fmov.s fr5,@-r1 */
L8c03883c: *stop=0x8c03883cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c03883e; /* fldi1 fr6 */
L8c03883e: *stop=0x8c03883eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[5],&f->fpscr,&fr[6]);if(status)return status;}goto L8c038840; /* fdiv fr5,fr6 */
L8c038840: *stop=0x8c038840u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[12],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c038842; /* fsub fr12,fr3 */
L8c038842: *stop=0x8c038842u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c038844; /* fmul fr6,fr3 */
L8c038844: *stop=0x8c038844u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c038846; /* fmul fr9,fr3 */
L8c038846: *stop=0x8c038846u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c038848; /* fmov.s fr3,@-r1 */
L8c038848: *stop=0x8c038848u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[13],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c03884a; /* fsub fr13,fr4 */
L8c03884a: *stop=0x8c03884au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[6],&f->fpscr,&fr[4]);if(status)return status;}goto L8c03884c; /* fmul fr6,fr4 */
L8c03884c: *stop=0x8c03884cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c03884e; /* fmul fr10,fr4 */
L8c03884e: *stop=0x8c03884eu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c038850; /* fmov.s fr4,@-r1 */
L8c038850: *stop=0x8c038850u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[1]);if(io_status)return io_status;goto L8c038852; /* fmov.s fr6,@-r1 */
L8c038852: *stop=0x8c038852u;r[8]+=4294967280u;goto L8c038854; /* add #-0x10,r8 */
L8c038854: *stop=0x8c038854u;r[1]+=68u;goto L8c038856; /* add #0x44,r1 */
L8c038856: *stop=0x8c038856u;if(!valid_ram(*gbr+0x4cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x4cu,4);if(io_status)return io_status;goto L8c038858; /* mov.l @(0x4c,GBR),r0 */
L8c038858: *stop=0x8c038858u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c03885a; /* mov.l r0,@r1 */
L8c03885a: *stop=0x8c03885au;if(!valid_access(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[9]);if(io_status)return io_status;r[9]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03885c; /* fmov.s @r9+,fr3 */
L8c03885c: *stop=0x8c03885cu;if(!valid_access(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[9]);if(io_status)return io_status;r[9]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03885e; /* fmov.s @r9+,fr4 */
L8c03885e: *stop=0x8c03885eu;if(!valid_access(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[9]);if(io_status)return io_status;r[9]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038860; /* fmov.s @r9+,fr5 */
L8c038860: *stop=0x8c038860u;if(!valid_access(r[9],4u))return -3;r[0]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[9]+=4;goto L8c038862; /* mov.l @r9+,r0 */
L8c038862: *stop=0x8c038862u;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c038864; /* mov.l r0,@-r1 */
L8c038864: *stop=0x8c038864u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c038866; /* fmov.s fr3,@-r1 */
L8c038866: *stop=0x8c038866u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c038868; /* fmov.s fr4,@-r1 */
L8c038868: *stop=0x8c038868u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;goto L8c03886a; /* fmov.s fr5,@-r1 */
L8c03886a: *stop=0x8c03886au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c03886c; /* fldi1 fr7 */
L8c03886c: *stop=0x8c03886cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[7],&fr[5],&f->fpscr,&fr[7]);if(status)return status;}goto L8c03886e; /* fdiv fr5,fr7 */
L8c03886e: *stop=0x8c03886eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[12],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c038870; /* fsub fr12,fr3 */
L8c038870: *stop=0x8c038870u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c038872; /* fmul fr7,fr3 */
L8c038872: *stop=0x8c038872u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c038874; /* fmul fr9,fr3 */
L8c038874: *stop=0x8c038874u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c038876; /* fmov.s fr3,@-r1 */
L8c038876: *stop=0x8c038876u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[13],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c038878; /* fsub fr13,fr4 */
L8c038878: *stop=0x8c038878u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[7],&f->fpscr,&fr[4]);if(status)return status;}goto L8c03887a; /* fmul fr7,fr4 */
L8c03887a: *stop=0x8c03887au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c03887c; /* fmul fr10,fr4 */
L8c03887c: *stop=0x8c03887cu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c03887e; /* fmov.s fr4,@-r1 */
L8c03887e: *stop=0x8c03887eu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[1]);if(io_status)return io_status;goto L8c038880; /* fmov.s fr7,@-r1 */
L8c038880: *stop=0x8c038880u;r[9]+=4294967280u;goto L8c038882; /* add #-0x10,r9 */
L8c038882: *stop=0x8c038882u;r[1]+=68u;goto L8c038884; /* add #0x44,r1 */
L8c038884: *stop=0x8c038884u;if(!valid_ram(*gbr+0x50u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x50u,4);if(io_status)return io_status;goto L8c038886; /* mov.l @(0x50,GBR),r0 */
L8c038886: *stop=0x8c038886u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c038888; /* mov.l r0,@r1 */
L8c038888: *stop=0x8c038888u;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[10]);if(io_status)return io_status;r[10]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03888a; /* fmov.s @r10+,fr3 */
L8c03888a: *stop=0x8c03888au;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[10]);if(io_status)return io_status;r[10]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03888c; /* fmov.s @r10+,fr4 */
L8c03888c: *stop=0x8c03888cu;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[10]);if(io_status)return io_status;r[10]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03888e; /* fmov.s @r10+,fr5 */
L8c03888e: *stop=0x8c03888eu;if(!valid_access(r[10],4u))return -3;r[0]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[10]+=4;goto L8c038890; /* mov.l @r10+,r0 */
L8c038890: *stop=0x8c038890u;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c038892; /* mov.l r0,@-r1 */
L8c038892: *stop=0x8c038892u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c038894; /* fmov.s fr3,@-r1 */
L8c038894: *stop=0x8c038894u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c038896; /* fmov.s fr4,@-r1 */
L8c038896: *stop=0x8c038896u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;goto L8c038898; /* fmov.s fr5,@-r1 */
L8c038898: *stop=0x8c038898u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][8]=0x3f800000u;goto L8c03889a; /* fldi1 fr8 */
L8c03889a: *stop=0x8c03889au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c03889c; /* fdiv fr5,fr8 */
L8c03889c: *stop=0x8c03889cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[12],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c03889e; /* fsub fr12,fr3 */
L8c03889e: *stop=0x8c03889eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0388a0; /* fmul fr8,fr3 */
L8c0388a0: *stop=0x8c0388a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0388a2; /* fmul fr9,fr3 */
L8c0388a2: *stop=0x8c0388a2u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0388a4; /* fmov.s fr3,@-r1 */
L8c0388a4: *stop=0x8c0388a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[13],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c0388a6; /* fsub fr13,fr4 */
L8c0388a6: *stop=0x8c0388a6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[8],&f->fpscr,&fr[4]);if(status)return status;}goto L8c0388a8; /* fmul fr8,fr4 */
L8c0388a8: *stop=0x8c0388a8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c0388aa; /* fmul fr10,fr4 */
L8c0388aa: *stop=0x8c0388aau;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c0388ac; /* fmov.s fr4,@-r1 */
L8c0388ac: *stop=0x8c0388acu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[1]);if(io_status)return io_status;goto L8c0388ae; /* fmov.s fr8,@-r1 */
L8c0388ae: *stop=0x8c0388aeu;r[10]+=4294967280u;goto L8c0388b0; /* add #-0x10,r10 */
L8c0388b0: *stop=0x8c0388b0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0x3f800000u;goto L8c0388b2; /* fldi1 fr0 */
L8c0388b2: *stop=0x8c0388b2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][6],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c0388b4; /* fcmp/gt fr0,fr6 */
L8c0388b4: if((*sr&1u)){goto L8c0388e0;}goto L8c0388b6; /* bt 0x8c0388e0 */
L8c0388b6: *stop=0x8c0388b6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c0388b8; /* fcmp/gt fr0,fr7 */
L8c0388b8: if((*sr&1u)){goto L8c0388e0;}goto L8c0388ba; /* bt 0x8c0388e0 */
L8c0388ba: *stop=0x8c0388bau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][8],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c0388bc; /* fcmp/gt fr0,fr8 */
L8c0388bc: if((*sr&1u)){goto L8c0388e0;}goto L8c0388be; /* bt 0x8c0388e0 */
L8c0388be: *stop=0x8c0388beu;r[1]=32u;goto L8c0388c0; /* mov #0x20,r1 */
L8c0388c0: *stop=0x8c0388c0u;r[0]=24u;goto L8c0388c2; /* mov #0x18,r0 */
L8c0388c2: *stop=0x8c0388c2u;r[1]=(r[0]&0x80000000u)?((r[0]&31u)?r[1]>>((0u-r[0])&31u):0u):(r[1]<<(r[0]&31u));goto L8c0388c4; /* shld r0,r1 */
L8c0388c4: *stop=0x8c0388c4u;r[1]=~r[1];goto L8c0388c6; /* not r1,r1 */
L8c0388c6: *stop=0x8c0388c6u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c0388c8; /* stc SR,r0 */
L8c0388c8: *stop=0x8c0388c8u;r[0]&=r[1];goto L8c0388ca; /* and r1,r0 */
L8c0388ca: *stop=0x8c0388cau;r[1]=~r[1];goto L8c0388cc; /* not r1,r1 */
L8c0388cc: *stop=0x8c0388ccu;r[0]|=r[1];goto L8c0388ce; /* or r1,r0 */
L8c0388ce: *stop=0x8c0388ceu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;if(checkpoint==0x8c0388ceu){*stop=0x8c0388d0u;return 2;}goto L8c0388d0; /* ldc r0,SR */
L8c0388d0: *stop=0x8c0388d0u;if(*pr!=entry_pr)return -3;*stop=0x8c0388d2u;if(!valid_access(r[15],4u))return -3;r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
L8c0388e0: *stop=0x8c0388e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c0388e2; /* mov.l r11,@-r15 */
L8c0388e2: *stop=0x8c0388e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c0388e4; /* mov.l r12,@-r15 */
L8c0388e4: *stop=0x8c0388e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0388e6; /* mov.l r13,@-r15 */
L8c0388e6: *stop=0x8c0388e6u;r[7]=3u;goto L8c0388e8; /* mov #0x3,r7 */
L8c0388e8: *stop=0x8c0388e8u;r[6]=0u;goto L8c0388ea; /* mov #0x0,r6 */
L8c0388ea: *stop=0x8c0388eau;r[1]=rd(m,bus,&io_status,0x8c038908u,4);if(io_status)return io_status;goto L8c0388ec; /* mov.l 0x8c038908,r1 */
L8c0388ec: *stop=0x8c0388ecu;r[2]=rd(m,bus,&io_status,0x8c03890cu,4);if(io_status)return io_status;goto L8c0388ee; /* mov.l 0x8c03890c,r2 */
L8c0388ee: *stop=0x8c0388eeu;r[3]=rd(m,bus,&io_status,0x8c038910u,4);if(io_status)return io_status;goto L8c0388f0; /* mov.l 0x8c038910,r3 */
L8c0388f0: *stop=0x8c0388f0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0x3f800000u;goto L8c0388f2; /* fldi1 fr0 */
L8c0388f2: *stop=0x8c0388f2u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c0388f4; /* fmov.s @r2,fr1 */
L8c0388f4: *stop=0x8c0388f4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c0388f6; /* fcmp/gt fr1,fr0 */
L8c0388f6: *stop=0x8c0388f6u;r[0]=*sr&1u;goto L8c0388f8; /* movt r0 */
L8c0388f8: *stop=0x8c0388f8u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c0388fa; /* fmov.s @r1,fr2 */
L8c0388fa: *stop=0x8c0388fau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c0388fc; /* fcmp/gt fr2,fr0 */
L8c0388fc: *stop=0x8c0388fcu;r[4]=*sr&1u;goto L8c0388fe; /* movt r4 */
L8c0388fe: *stop=0x8c0388feu;r[0]^=r[4];goto L8c038900; /* xor r4,r0 */
L8c038900: *stop=0x8c038900u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c038902; /* tst r0,r0 */
L8c038902: if(!(*sr&1u)){goto L8c038914;}goto L8c038904; /* bf 0x8c038914 */
L8c038904: *stop=0x8c038906u;goto L8c038a54; /* bra 0x8c038a54 */
L8c038914: *stop=0x8c038914u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c038916; /* fsub fr1,fr0 */
L8c038916: *stop=0x8c038916u;r[1]+=4u;goto L8c038918; /* add #0x4,r1 */
L8c038918: *stop=0x8c038918u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[1],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c03891a; /* fsub fr1,fr2 */
L8c03891a: *stop=0x8c03891au;r[2]+=4u;goto L8c03891c; /* add #0x4,r2 */
L8c03891c: *stop=0x8c03891cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[0],&fr[2],&f->fpscr,&fr[0]);if(status)return status;}goto L8c03891e; /* fdiv fr2,fr0 */
L8c03891e: *stop=0x8c03891eu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038920; /* fmov.s @r2+,fr1 */
L8c038920: *stop=0x8c038920u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038922; /* fmov.s @r1+,fr2 */
L8c038922: *stop=0x8c038922u;fcopy(f,5u,2u);goto L8c038924; /* fmov fr2,fr5 */
L8c038924: *stop=0x8c038924u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[1],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c038926; /* fsub fr1,fr5 */
L8c038926: *stop=0x8c038926u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c038928; /* fmul fr0,fr5 */
L8c038928: *stop=0x8c038928u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[1],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c03892a; /* fadd fr1,fr5 */
L8c03892a: *stop=0x8c03892au;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03892c; /* fmov.s @r2+,fr1 */
L8c03892c: *stop=0x8c03892cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03892e; /* fmov.s @r1+,fr2 */
L8c03892e: *stop=0x8c03892eu;fcopy(f,4u,2u);goto L8c038930; /* fmov fr2,fr4 */
L8c038930: *stop=0x8c038930u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c038932; /* fsub fr1,fr4 */
L8c038932: *stop=0x8c038932u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[0],&f->fpscr,&fr[4]);if(status)return status;}goto L8c038934; /* fmul fr0,fr4 */
L8c038934: *stop=0x8c038934u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c038936; /* fadd fr1,fr4 */
L8c038936: *stop=0x8c038936u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[14],&f->fpscr,&fr[4]);if(status)return status;}goto L8c038938; /* fmul fr14,fr4 */
L8c038938: *stop=0x8c038938u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[12],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c03893a; /* fadd fr12,fr4 */
L8c03893a: *stop=0x8c03893au;r[1]+=12u;goto L8c03893c; /* add #0xc,r1 */
L8c03893c: *stop=0x8c03893cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[15],&f->fpscr,&fr[5]);if(status)return status;}goto L8c03893e; /* fmul fr15,fr5 */
L8c03893e: *stop=0x8c03893eu;r[2]+=12u;goto L8c038940; /* add #0xc,r2 */
L8c038940: *stop=0x8c038940u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[13],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c038942; /* fadd fr13,fr5 */
L8c038942: *stop=0x8c038942u;*fpul=r[14];goto L8c038944; /* lds r14,FPUL */
L8c038944: *stop=0x8c038944u;f->bank[(f->fpscr>>21)&1u][11]=*fpul;goto L8c038946; /* fsts FPUL,fr11 */
L8c038946: *stop=0x8c038946u;r[0]=rd(m,bus,&io_status,0x8c038a88u,4);if(io_status)return io_status;goto L8c038948; /* mov.l 0x8c038a88,r0 */
L8c038948: *stop=0x8c038948u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]);if(io_status)return io_status;goto L8c03894a; /* fmov.s @r0,fr9 */
L8c03894a: *stop=0x8c03894au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[0],&f->fpscr,&fr[9]);if(status)return status;}goto L8c03894c; /* fmul fr0,fr9 */
L8c03894c: *stop=0x8c03894cu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][9],&f->fpscr,fpul);if(status)return status;goto L8c03894e; /* ftrc fr9,FPUL */
L8c03894e: *stop=0x8c03894eu;r[14]=*fpul;goto L8c038950; /* sts FPUL,r14 */
L8c038950: *stop=0x8c038950u;r[0]=4294967295u;goto L8c038952; /* mov #-0x1,r0 */
L8c038952: *stop=0x8c038952u;r[0]=r[0]&255u;goto L8c038954; /* extu.b r0,r0 */
L8c038954: *stop=0x8c038954u;if(!valid_access(r[2],4u))return -3;r[4]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[2]+=4;goto L8c038956; /* mov.l @r2+,r4 */
L8c038956: *stop=0x8c038956u;if(!valid_access(r[1],4u))return -3;r[5]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]+=4;goto L8c038958; /* mov.l @r1+,r5 */
L8c038958: *stop=0x8c038958u;r[11]=r[4];goto L8c03895a; /* mov r4,r11 */
L8c03895a: *stop=0x8c03895au;r[11]&=r[0];goto L8c03895c; /* and r0,r11 */
L8c03895c: *stop=0x8c03895cu;r[13]=r[5];goto L8c03895e; /* mov r5,r13 */
L8c03895e: *stop=0x8c03895eu;r[13]&=r[0];goto L8c038960; /* and r0,r13 */
L8c038960: *stop=0x8c038960u;r[13]-=r[11];goto L8c038962; /* sub r11,r13 */
L8c038962: *stop=0x8c038962u;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c038964; /* dmuls.l r14,r13 */
L8c038964: *stop=0x8c038964u;r[13]=*macl;if(checkpoint==0x8c038964u){*stop=0x8c038966u;return 2;}goto L8c038966; /* sts MACL,r13 */
L8c038966: *stop=0x8c038966u;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c038968; /* swap.w r13,r13 */
L8c038968: *stop=0x8c038968u;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c03896a; /* exts.w r13,r13 */
L8c03896a: *stop=0x8c03896au;r[11]+=r[13];goto L8c03896c; /* add r13,r11 */
L8c03896c: *stop=0x8c03896cu;r[4]>>=8;goto L8c03896e; /* shlr8 r4 */
L8c03896e: *stop=0x8c03896eu;r[5]>>=8;goto L8c038970; /* shlr8 r5 */
L8c038970: *stop=0x8c038970u;r[12]=r[4];goto L8c038972; /* mov r4,r12 */
L8c038972: *stop=0x8c038972u;r[12]&=r[0];goto L8c038974; /* and r0,r12 */
L8c038974: *stop=0x8c038974u;r[13]=r[5];goto L8c038976; /* mov r5,r13 */
L8c038976: *stop=0x8c038976u;r[13]&=r[0];goto L8c038978; /* and r0,r13 */
L8c038978: *stop=0x8c038978u;r[13]-=r[12];goto L8c03897a; /* sub r12,r13 */
L8c03897a: *stop=0x8c03897au;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c03897c; /* dmuls.l r14,r13 */
L8c03897c: *stop=0x8c03897cu;r[13]=*macl;if(checkpoint==0x8c03897cu){*stop=0x8c03897eu;return 2;}goto L8c03897e; /* sts MACL,r13 */
L8c03897e: *stop=0x8c03897eu;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c038980; /* swap.w r13,r13 */
L8c038980: *stop=0x8c038980u;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c038982; /* exts.w r13,r13 */
L8c038982: *stop=0x8c038982u;r[12]+=r[13];goto L8c038984; /* add r13,r12 */
L8c038984: *stop=0x8c038984u;r[12]<<=8;goto L8c038986; /* shll8 r12 */
L8c038986: *stop=0x8c038986u;r[11]|=r[12];goto L8c038988; /* or r12,r11 */
L8c038988: *stop=0x8c038988u;r[4]>>=8;goto L8c03898a; /* shlr8 r4 */
L8c03898a: *stop=0x8c03898au;r[5]>>=8;goto L8c03898c; /* shlr8 r5 */
L8c03898c: *stop=0x8c03898cu;r[12]=r[4];goto L8c03898e; /* mov r4,r12 */
L8c03898e: *stop=0x8c03898eu;r[12]&=r[0];goto L8c038990; /* and r0,r12 */
L8c038990: *stop=0x8c038990u;r[13]=r[5];goto L8c038992; /* mov r5,r13 */
L8c038992: *stop=0x8c038992u;r[13]&=r[0];goto L8c038994; /* and r0,r13 */
L8c038994: *stop=0x8c038994u;r[13]-=r[12];goto L8c038996; /* sub r12,r13 */
L8c038996: *stop=0x8c038996u;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c038998; /* dmuls.l r14,r13 */
L8c038998: *stop=0x8c038998u;r[13]=*macl;if(checkpoint==0x8c038998u){*stop=0x8c03899au;return 2;}goto L8c03899a; /* sts MACL,r13 */
L8c03899a: *stop=0x8c03899au;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c03899c; /* swap.w r13,r13 */
L8c03899c: *stop=0x8c03899cu;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c03899e; /* exts.w r13,r13 */
L8c03899e: *stop=0x8c03899eu;r[12]+=r[13];goto L8c0389a0; /* add r13,r12 */
L8c0389a0: *stop=0x8c0389a0u;r[12]<<=16;goto L8c0389a2; /* shll16 r12 */
L8c0389a2: *stop=0x8c0389a2u;r[11]|=r[12];goto L8c0389a4; /* or r12,r11 */
L8c0389a4: *stop=0x8c0389a4u;r[4]>>=8;goto L8c0389a6; /* shlr8 r4 */
L8c0389a6: *stop=0x8c0389a6u;r[5]>>=8;goto L8c0389a8; /* shlr8 r5 */
L8c0389a8: *stop=0x8c0389a8u;r[12]=r[4];goto L8c0389aa; /* mov r4,r12 */
L8c0389aa: *stop=0x8c0389aau;r[12]&=r[0];goto L8c0389ac; /* and r0,r12 */
L8c0389ac: *stop=0x8c0389acu;r[13]=r[5];goto L8c0389ae; /* mov r5,r13 */
L8c0389ae: *stop=0x8c0389aeu;r[13]&=r[0];goto L8c0389b0; /* and r0,r13 */
L8c0389b0: *stop=0x8c0389b0u;r[13]-=r[12];goto L8c0389b2; /* sub r12,r13 */
L8c0389b2: *stop=0x8c0389b2u;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c0389b4; /* dmuls.l r14,r13 */
L8c0389b4: *stop=0x8c0389b4u;r[13]=*macl;if(checkpoint==0x8c0389b4u){*stop=0x8c0389b6u;return 2;}goto L8c0389b6; /* sts MACL,r13 */
L8c0389b6: *stop=0x8c0389b6u;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c0389b8; /* swap.w r13,r13 */
L8c0389b8: *stop=0x8c0389b8u;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c0389ba; /* exts.w r13,r13 */
L8c0389ba: *stop=0x8c0389bau;r[12]+=r[13];goto L8c0389bc; /* add r13,r12 */
L8c0389bc: *stop=0x8c0389bcu;r[12]<<=16;goto L8c0389be; /* shll16 r12 */
L8c0389be: *stop=0x8c0389beu;r[12]<<=8;goto L8c0389c0; /* shll8 r12 */
L8c0389c0: *stop=0x8c0389c0u;r[11]|=r[12];goto L8c0389c2; /* or r12,r11 */
L8c0389c2: *stop=0x8c0389c2u;*fpul=r[11];goto L8c0389c4; /* lds r11,FPUL */
L8c0389c4: *stop=0x8c0389c4u;f->bank[(f->fpscr>>21)&1u][9]=*fpul;goto L8c0389c6; /* fsts FPUL,fr9 */
L8c0389c6: *stop=0x8c0389c6u;if(!valid_access(r[2],4u))return -3;r[4]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[2]+=4;goto L8c0389c8; /* mov.l @r2+,r4 */
L8c0389c8: *stop=0x8c0389c8u;if(!valid_access(r[1],4u))return -3;r[5]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]+=4;goto L8c0389ca; /* mov.l @r1+,r5 */
L8c0389ca: *stop=0x8c0389cau;r[11]=r[4];goto L8c0389cc; /* mov r4,r11 */
L8c0389cc: *stop=0x8c0389ccu;r[11]&=r[0];goto L8c0389ce; /* and r0,r11 */
L8c0389ce: *stop=0x8c0389ceu;r[13]=r[5];goto L8c0389d0; /* mov r5,r13 */
L8c0389d0: *stop=0x8c0389d0u;r[13]&=r[0];goto L8c0389d2; /* and r0,r13 */
L8c0389d2: *stop=0x8c0389d2u;r[13]-=r[11];goto L8c0389d4; /* sub r11,r13 */
L8c0389d4: *stop=0x8c0389d4u;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c0389d6; /* dmuls.l r14,r13 */
L8c0389d6: *stop=0x8c0389d6u;r[13]=*macl;if(checkpoint==0x8c0389d6u){*stop=0x8c0389d8u;return 2;}goto L8c0389d8; /* sts MACL,r13 */
L8c0389d8: *stop=0x8c0389d8u;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c0389da; /* swap.w r13,r13 */
L8c0389da: *stop=0x8c0389dau;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c0389dc; /* exts.w r13,r13 */
L8c0389dc: *stop=0x8c0389dcu;r[11]+=r[13];goto L8c0389de; /* add r13,r11 */
L8c0389de: *stop=0x8c0389deu;r[4]>>=8;goto L8c0389e0; /* shlr8 r4 */
L8c0389e0: *stop=0x8c0389e0u;r[5]>>=8;goto L8c0389e2; /* shlr8 r5 */
L8c0389e2: *stop=0x8c0389e2u;r[12]=r[4];goto L8c0389e4; /* mov r4,r12 */
L8c0389e4: *stop=0x8c0389e4u;r[12]&=r[0];goto L8c0389e6; /* and r0,r12 */
L8c0389e6: *stop=0x8c0389e6u;r[13]=r[5];goto L8c0389e8; /* mov r5,r13 */
L8c0389e8: *stop=0x8c0389e8u;r[13]&=r[0];goto L8c0389ea; /* and r0,r13 */
L8c0389ea: *stop=0x8c0389eau;r[13]-=r[12];goto L8c0389ec; /* sub r12,r13 */
L8c0389ec: *stop=0x8c0389ecu;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c0389ee; /* dmuls.l r14,r13 */
L8c0389ee: *stop=0x8c0389eeu;r[13]=*macl;if(checkpoint==0x8c0389eeu){*stop=0x8c0389f0u;return 2;}goto L8c0389f0; /* sts MACL,r13 */
L8c0389f0: *stop=0x8c0389f0u;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c0389f2; /* swap.w r13,r13 */
L8c0389f2: *stop=0x8c0389f2u;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c0389f4; /* exts.w r13,r13 */
L8c0389f4: *stop=0x8c0389f4u;r[12]+=r[13];goto L8c0389f6; /* add r13,r12 */
L8c0389f6: *stop=0x8c0389f6u;r[12]<<=8;goto L8c0389f8; /* shll8 r12 */
L8c0389f8: *stop=0x8c0389f8u;r[11]|=r[12];goto L8c0389fa; /* or r12,r11 */
L8c0389fa: *stop=0x8c0389fau;r[4]>>=8;goto L8c0389fc; /* shlr8 r4 */
L8c0389fc: *stop=0x8c0389fcu;r[5]>>=8;goto L8c0389fe; /* shlr8 r5 */
L8c0389fe: *stop=0x8c0389feu;r[12]=r[4];goto L8c038a00; /* mov r4,r12 */
L8c038a00: *stop=0x8c038a00u;r[12]&=r[0];goto L8c038a02; /* and r0,r12 */
L8c038a02: *stop=0x8c038a02u;r[13]=r[5];goto L8c038a04; /* mov r5,r13 */
L8c038a04: *stop=0x8c038a04u;r[13]&=r[0];goto L8c038a06; /* and r0,r13 */
L8c038a06: *stop=0x8c038a06u;r[13]-=r[12];goto L8c038a08; /* sub r12,r13 */
L8c038a08: *stop=0x8c038a08u;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c038a0a; /* dmuls.l r14,r13 */
L8c038a0a: *stop=0x8c038a0au;r[13]=*macl;if(checkpoint==0x8c038a0au){*stop=0x8c038a0cu;return 2;}goto L8c038a0c; /* sts MACL,r13 */
L8c038a0c: *stop=0x8c038a0cu;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c038a0e; /* swap.w r13,r13 */
L8c038a0e: *stop=0x8c038a0eu;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c038a10; /* exts.w r13,r13 */
L8c038a10: *stop=0x8c038a10u;r[12]+=r[13];goto L8c038a12; /* add r13,r12 */
L8c038a12: *stop=0x8c038a12u;r[12]<<=16;goto L8c038a14; /* shll16 r12 */
L8c038a14: *stop=0x8c038a14u;r[11]|=r[12];goto L8c038a16; /* or r12,r11 */
L8c038a16: *stop=0x8c038a16u;r[4]>>=8;goto L8c038a18; /* shlr8 r4 */
L8c038a18: *stop=0x8c038a18u;r[5]>>=8;goto L8c038a1a; /* shlr8 r5 */
L8c038a1a: *stop=0x8c038a1au;r[12]=r[4];goto L8c038a1c; /* mov r4,r12 */
L8c038a1c: *stop=0x8c038a1cu;r[12]&=r[0];goto L8c038a1e; /* and r0,r12 */
L8c038a1e: *stop=0x8c038a1eu;r[13]=r[5];goto L8c038a20; /* mov r5,r13 */
L8c038a20: *stop=0x8c038a20u;r[13]&=r[0];goto L8c038a22; /* and r0,r13 */
L8c038a22: *stop=0x8c038a22u;r[13]-=r[12];goto L8c038a24; /* sub r12,r13 */
L8c038a24: *stop=0x8c038a24u;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c038a26; /* dmuls.l r14,r13 */
L8c038a26: *stop=0x8c038a26u;r[13]=*macl;if(checkpoint==0x8c038a26u){*stop=0x8c038a28u;return 2;}goto L8c038a28; /* sts MACL,r13 */
L8c038a28: *stop=0x8c038a28u;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c038a2a; /* swap.w r13,r13 */
L8c038a2a: *stop=0x8c038a2au;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c038a2c; /* exts.w r13,r13 */
L8c038a2c: *stop=0x8c038a2cu;r[12]+=r[13];goto L8c038a2e; /* add r13,r12 */
L8c038a2e: *stop=0x8c038a2eu;r[12]<<=16;goto L8c038a30; /* shll16 r12 */
L8c038a30: *stop=0x8c038a30u;r[12]<<=8;goto L8c038a32; /* shll8 r12 */
L8c038a32: *stop=0x8c038a32u;r[11]|=r[12];goto L8c038a34; /* or r12,r11 */
L8c038a34: *stop=0x8c038a34u;*fpul=r[11];goto L8c038a36; /* lds r11,FPUL */
L8c038a36: *stop=0x8c038a36u;f->bank[(f->fpscr>>21)&1u][10]=*fpul;goto L8c038a38; /* fsts FPUL,fr10 */
L8c038a38: *stop=0x8c038a38u;*fpul=f->bank[(f->fpscr>>21)&1u][11];goto L8c038a3a; /* flds fr11,FPUL */
L8c038a3a: *stop=0x8c038a3au;r[14]=*fpul;goto L8c038a3c; /* sts FPUL,r14 */
L8c038a3c: *stop=0x8c038a3cu;r[1]+=4294967264u;goto L8c038a3e; /* add #-0x20,r1 */
L8c038a3e: *stop=0x8c038a3eu;r[2]+=4294967264u;goto L8c038a40; /* add #-0x20,r2 */
L8c038a40: *stop=0x8c038a40u;r[0]=r[3];goto L8c038a42; /* mov r3,r0 */
L8c038a42: *stop=0x8c038a42u;r[0]+=28u;goto L8c038a44; /* add #0x1c,r0 */
L8c038a44: *stop=0x8c038a44u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;goto L8c038a46; /* fmov.s fr10,@r0 */
L8c038a46: *stop=0x8c038a46u;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[0]);if(io_status)return io_status;goto L8c038a48; /* fmov.s fr9,@-r0 */
L8c038a48: *stop=0x8c038a48u;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c038a4a; /* fmov.s fr4,@-r0 */
L8c038a4a: *stop=0x8c038a4au;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c038a4c; /* fmov.s fr5,@-r0 */
L8c038a4c: *stop=0x8c038a4cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c038a4e; /* fldi1 fr6 */
L8c038a4e: *stop=0x8c038a4eu;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c038a50; /* fmov.s fr6,@-r0 */
L8c038a50: *stop=0x8c038a50u;r[3]+=40u;goto L8c038a52; /* add #0x28,r3 */
L8c038a52: *stop=0x8c038a52u;r[6]+=1u;goto L8c038a54; /* add #0x1,r6 */
L8c038a54: *stop=0x8c038a54u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0x3f800000u;goto L8c038a56; /* fldi1 fr0 */
L8c038a56: *stop=0x8c038a56u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c038a58; /* fmov.s @r1,fr2 */
L8c038a58: *stop=0x8c038a58u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c038a5a; /* fcmp/gt fr0,fr2 */
L8c038a5a: if(!(*sr&1u)){goto L8c038a7c;}goto L8c038a5c; /* bf 0x8c038a7c */
L8c038a5c: *stop=0x8c038a5cu;r[0]=r[1];goto L8c038a5e; /* mov r1,r0 */
L8c038a5e: *stop=0x8c038a5eu;r[0]+=12u;goto L8c038a60; /* add #0xc,r0 */
L8c038a60: *stop=0x8c038a60u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038a62; /* fmov.s @r0+,fr6 */
L8c038a62: *stop=0x8c038a62u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038a64; /* fmov.s @r0+,fr5 */
L8c038a64: *stop=0x8c038a64u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038a66; /* fmov.s @r0+,fr4 */
L8c038a66: *stop=0x8c038a66u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038a68; /* fmov.s @r0+,fr9 */
L8c038a68: *stop=0x8c038a68u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038a6a; /* fmov.s @r0+,fr10 */
L8c038a6a: *stop=0x8c038a6au;r[0]=r[3];goto L8c038a6c; /* mov r3,r0 */
L8c038a6c: *stop=0x8c038a6cu;r[0]+=28u;goto L8c038a6e; /* add #0x1c,r0 */
L8c038a6e: *stop=0x8c038a6eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;goto L8c038a70; /* fmov.s fr10,@r0 */
L8c038a70: *stop=0x8c038a70u;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[0]);if(io_status)return io_status;goto L8c038a72; /* fmov.s fr9,@-r0 */
L8c038a72: *stop=0x8c038a72u;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c038a74; /* fmov.s fr4,@-r0 */
L8c038a74: *stop=0x8c038a74u;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c038a76; /* fmov.s fr5,@-r0 */
L8c038a76: *stop=0x8c038a76u;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c038a78; /* fmov.s fr6,@-r0 */
L8c038a78: *stop=0x8c038a78u;r[3]+=40u;goto L8c038a7a; /* add #0x28,r3 */
L8c038a7a: *stop=0x8c038a7au;r[6]+=1u;goto L8c038a7c; /* add #0x1,r6 */
L8c038a7c: *stop=0x8c038a7cu;r[2]=r[1];goto L8c038a7e; /* mov r1,r2 */
L8c038a7e: *stop=0x8c038a7eu;r[7]--;*sr=(*sr&~1u)|(!r[7]);goto L8c038a80; /* dt r7 */
L8c038a80: if((*sr&1u)){goto L8c038a8c;}goto L8c038a82; /* bt 0x8c038a8c */
L8c038a82: *stop=0x8c038a84u;r[1]+=40u;goto L8c0388f0; /* bra 0x8c0388f0 */
L8c038a8c: *stop=0x8c038a8cu;r[11]=rd(m,bus,&io_status,0x8c038b2cu,4);if(io_status)return io_status;goto L8c038a8e; /* mov.l 0x8c038b2c,r11 */
L8c038a8e: *stop=0x8c038a8eu;r[1]=rd(m,bus,&io_status,0x8c038b30u,4);if(io_status)return io_status;goto L8c038a90; /* mov.l 0x8c038b30,r1 */
L8c038a90: *stop=0x8c038a90u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038a92; /* fmov.s @r1+,fr2 */
L8c038a92: *stop=0x8c038a92u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038a94; /* fmov.s @r1+,fr1 */
L8c038a94: *stop=0x8c038a94u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038a96; /* fmov.s @r1+,fr0 */
L8c038a96: *stop=0x8c038a96u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038a98; /* fmov.s @r1+,fr3 */
L8c038a98: *stop=0x8c038a98u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038a9a; /* fmov.s @r1+,fr4 */
L8c038a9a: *stop=0x8c038a9au;r[14]+=28u;goto L8c038a9c; /* add #0x1c,r14 */
L8c038a9c: *stop=0x8c038a9cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c038a9e; /* fmov.s fr3,@-r14 */
L8c038a9e: *stop=0x8c038a9eu;r[14]+=4294967288u;goto L8c038aa0; /* add #-0x8,r14 */
L8c038aa0: *stop=0x8c038aa0u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c038aa2; /* fmov.s fr2,@-r14 */
L8c038aa2: *stop=0x8c038aa2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c038aa4; /* fmov.s fr1,@-r14 */
L8c038aa4: *stop=0x8c038aa4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c038aa6; /* fmov.s fr0,@-r14 */
L8c038aa6: *stop=0x8c038aa6u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c038aa8; /* mov.l r11,@-r14 */
L8c038aa8: *stop=0x8c038aa8u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c038aaa; /* pref @r14 */
L8c038aaa: *stop=0x8c038aaau;r[14]+=32u;goto L8c038aac; /* add #0x20,r14 */
L8c038aac: *stop=0x8c038aacu;r[1]=rd(m,bus,&io_status,0x8c038b34u,4);if(io_status)return io_status;goto L8c038aae; /* mov.l 0x8c038b34,r1 */
L8c038aae: *stop=0x8c038aaeu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038ab0; /* fmov.s @r1+,fr2 */
L8c038ab0: *stop=0x8c038ab0u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038ab2; /* fmov.s @r1+,fr1 */
L8c038ab2: *stop=0x8c038ab2u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038ab4; /* fmov.s @r1+,fr0 */
L8c038ab4: *stop=0x8c038ab4u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038ab6; /* fmov.s @r1+,fr3 */
L8c038ab6: *stop=0x8c038ab6u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038ab8; /* fmov.s @r1+,fr4 */
L8c038ab8: *stop=0x8c038ab8u;r[14]+=28u;goto L8c038aba; /* add #0x1c,r14 */
L8c038aba: *stop=0x8c038abau;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c038abc; /* fmov.s fr3,@-r14 */
L8c038abc: *stop=0x8c038abcu;r[14]+=4294967288u;goto L8c038abe; /* add #-0x8,r14 */
L8c038abe: *stop=0x8c038abeu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c038ac0; /* fmov.s fr2,@-r14 */
L8c038ac0: *stop=0x8c038ac0u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c038ac2; /* fmov.s fr1,@-r14 */
L8c038ac2: *stop=0x8c038ac2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c038ac4; /* fmov.s fr0,@-r14 */
L8c038ac4: *stop=0x8c038ac4u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c038ac6; /* mov.l r11,@-r14 */
L8c038ac6: *stop=0x8c038ac6u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c038ac8; /* pref @r14 */
L8c038ac8: *stop=0x8c038ac8u;r[14]+=32u;goto L8c038aca; /* add #0x20,r14 */
L8c038aca: *stop=0x8c038acau;r[0]=r[6];goto L8c038acc; /* mov r6,r0 */
L8c038acc: *stop=0x8c038accu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c038ace; /* cmp/eq #0x3,r0 */
L8c038ace: if((*sr&1u)){goto L8c038aee;}goto L8c038ad0; /* bt 0x8c038aee */
L8c038ad0: *stop=0x8c038ad0u;r[1]=rd(m,bus,&io_status,0x8c038b38u,4);if(io_status)return io_status;goto L8c038ad2; /* mov.l 0x8c038b38,r1 */
L8c038ad2: *stop=0x8c038ad2u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038ad4; /* fmov.s @r1+,fr2 */
L8c038ad4: *stop=0x8c038ad4u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038ad6; /* fmov.s @r1+,fr1 */
L8c038ad6: *stop=0x8c038ad6u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038ad8; /* fmov.s @r1+,fr0 */
L8c038ad8: *stop=0x8c038ad8u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038ada; /* fmov.s @r1+,fr3 */
L8c038ada: *stop=0x8c038adau;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038adc; /* fmov.s @r1+,fr4 */
L8c038adc: *stop=0x8c038adcu;r[14]+=28u;goto L8c038ade; /* add #0x1c,r14 */
L8c038ade: *stop=0x8c038adeu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c038ae0; /* fmov.s fr3,@-r14 */
L8c038ae0: *stop=0x8c038ae0u;r[14]+=4294967288u;goto L8c038ae2; /* add #-0x8,r14 */
L8c038ae2: *stop=0x8c038ae2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c038ae4; /* fmov.s fr2,@-r14 */
L8c038ae4: *stop=0x8c038ae4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c038ae6; /* fmov.s fr1,@-r14 */
L8c038ae6: *stop=0x8c038ae6u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c038ae8; /* fmov.s fr0,@-r14 */
L8c038ae8: *stop=0x8c038ae8u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c038aea; /* mov.l r11,@-r14 */
L8c038aea: *stop=0x8c038aeau;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c038aec; /* pref @r14 */
L8c038aec: *stop=0x8c038aecu;r[14]+=32u;goto L8c038aee; /* add #0x20,r14 */
L8c038aee: *stop=0x8c038aeeu;r[1]=rd(m,bus,&io_status,0x8c038b3cu,4);if(io_status)return io_status;goto L8c038af0; /* mov.l 0x8c038b3c,r1 */
L8c038af0: *stop=0x8c038af0u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038af2; /* fmov.s @r1+,fr2 */
L8c038af2: *stop=0x8c038af2u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038af4; /* fmov.s @r1+,fr1 */
L8c038af4: *stop=0x8c038af4u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038af6; /* fmov.s @r1+,fr0 */
L8c038af6: *stop=0x8c038af6u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038af8; /* fmov.s @r1+,fr3 */
L8c038af8: *stop=0x8c038af8u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038afa; /* fmov.s @r1+,fr4 */
L8c038afa: *stop=0x8c038afau;r[6]=rd(m,bus,&io_status,0x8c038b40u,4);if(io_status)return io_status;goto L8c038afc; /* mov.l 0x8c038b40,r6 */
L8c038afc: *stop=0x8c038afcu;r[14]+=28u;goto L8c038afe; /* add #0x1c,r14 */
L8c038afe: *stop=0x8c038afeu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c038b00; /* fmov.s fr3,@-r14 */
L8c038b00: *stop=0x8c038b00u;r[14]+=4294967288u;goto L8c038b02; /* add #-0x8,r14 */
L8c038b02: *stop=0x8c038b02u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c038b04; /* fmov.s fr2,@-r14 */
L8c038b04: *stop=0x8c038b04u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c038b06; /* fmov.s fr1,@-r14 */
L8c038b06: *stop=0x8c038b06u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c038b08; /* fmov.s fr0,@-r14 */
L8c038b08: *stop=0x8c038b08u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[6],4);if(io_status)return io_status;goto L8c038b0a; /* mov.l r6,@-r14 */
L8c038b0a: *stop=0x8c038b0au;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c038b0c; /* pref @r14 */
L8c038b0c: *stop=0x8c038b0cu;r[14]+=32u;goto L8c038b0e; /* add #0x20,r14 */
L8c038b0e: *stop=0x8c038b0eu;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c038b10; /* mov.l @r15+,r13 */
L8c038b10: *stop=0x8c038b10u;if(!valid_access(r[15],4u))return -3;r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c038b12; /* mov.l @r15+,r12 */
L8c038b12: *stop=0x8c038b12u;if(!valid_access(r[15],4u))return -3;r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c038b14; /* mov.l @r15+,r11 */
L8c038b14: *stop=0x8c038b14u;r[1]=32u;goto L8c038b16; /* mov #0x20,r1 */
L8c038b16: *stop=0x8c038b16u;r[0]=24u;goto L8c038b18; /* mov #0x18,r0 */
L8c038b18: *stop=0x8c038b18u;r[1]=(r[0]&0x80000000u)?((r[0]&31u)?r[1]>>((0u-r[0])&31u):0u):(r[1]<<(r[0]&31u));goto L8c038b1a; /* shld r0,r1 */
L8c038b1a: *stop=0x8c038b1au;r[1]=~r[1];goto L8c038b1c; /* not r1,r1 */
L8c038b1c: *stop=0x8c038b1cu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c038b1e; /* stc SR,r0 */
L8c038b1e: *stop=0x8c038b1eu;r[0]&=r[1];goto L8c038b20; /* and r1,r0 */
L8c038b20: *stop=0x8c038b20u;r[1]=~r[1];goto L8c038b22; /* not r1,r1 */
L8c038b22: *stop=0x8c038b22u;r[0]|=r[1];goto L8c038b24; /* or r1,r0 */
L8c038b24: *stop=0x8c038b24u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;if(checkpoint==0x8c038b24u){*stop=0x8c038b26u;return 2;}goto L8c038b26; /* ldc r0,SR */
L8c038b26: *stop=0x8c038b26u;if(*pr!=entry_pr)return -3;*stop=0x8c038b28u;if(!valid_access(r[15],4u))return -3;r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_clipping_state_8c037ff4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,uint32_t entry,uint32_t checkpoint){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)checkpoint;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 switch(entry){case 0x8c037ff4u:goto L8c037ff4;case 0x8c038076u:goto L8c038076;case 0x8c03808eu:goto L8c03808e;case 0x8c0380aau:goto L8c0380aa;case 0x8c0380c6u:goto L8c0380c6;case 0x8c0380e8u:goto L8c0380e8;case 0x8c038100u:goto L8c038100;case 0x8c03811cu:goto L8c03811c;case 0x8c038138u:goto L8c038138;case 0x8c03827cu:goto L8c03827c;default:return -2;}
L8c037ff4: *stop=0x8c037ff4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c037ff6; /* mov.l r11,@-r15 */
L8c037ff6: *stop=0x8c037ff6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c037ff8; /* mov.l r12,@-r15 */
L8c037ff8: *stop=0x8c037ff8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c037ffa; /* mov.l r13,@-r15 */
L8c037ffa: *stop=0x8c037ffau;r[7]=3u;goto L8c037ffc; /* mov #0x3,r7 */
L8c037ffc: *stop=0x8c037ffcu;r[6]=0u;goto L8c037ffe; /* mov #0x0,r6 */
L8c037ffe: *stop=0x8c037ffeu;r[1]=rd(m,bus,&io_status,0x8c03801cu,4);if(io_status)return io_status;goto L8c038000; /* mov.l 0x8c03801c,r1 */
L8c038000: *stop=0x8c038000u;r[2]=rd(m,bus,&io_status,0x8c038020u,4);if(io_status)return io_status;goto L8c038002; /* mov.l 0x8c038020,r2 */
L8c038002: *stop=0x8c038002u;r[3]=rd(m,bus,&io_status,0x8c038024u,4);if(io_status)return io_status;goto L8c038004; /* mov.l 0x8c038024,r3 */
L8c038004: *stop=0x8c038004u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0x3f800000u;goto L8c038006; /* fldi1 fr0 */
L8c038006: *stop=0x8c038006u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c038008; /* fmov.s @r2,fr1 */
L8c038008: *stop=0x8c038008u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c03800a; /* fcmp/gt fr1,fr0 */
L8c03800a: *stop=0x8c03800au;r[0]=*sr&1u;goto L8c03800c; /* movt r0 */
L8c03800c: *stop=0x8c03800cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c03800e; /* fmov.s @r1,fr2 */
L8c03800e: *stop=0x8c03800eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c038010; /* fcmp/gt fr2,fr0 */
L8c038010: *stop=0x8c038010u;r[4]=*sr&1u;goto L8c038012; /* movt r4 */
L8c038012: *stop=0x8c038012u;r[0]^=r[4];goto L8c038014; /* xor r4,r0 */
L8c038014: *stop=0x8c038014u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c038016; /* tst r0,r0 */
L8c038016: if(!(*sr&1u)){goto L8c038028;}goto L8c038018; /* bf 0x8c038028 */
L8c038018: *stop=0x8c03801au;goto L8c038184; /* bra 0x8c038184 */
L8c038028: *stop=0x8c038028u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c03802a; /* fsub fr1,fr0 */
L8c03802a: *stop=0x8c03802au;r[1]+=4u;goto L8c03802c; /* add #0x4,r1 */
L8c03802c: *stop=0x8c03802cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[1],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c03802e; /* fsub fr1,fr2 */
L8c03802e: *stop=0x8c03802eu;r[2]+=4u;goto L8c038030; /* add #0x4,r2 */
L8c038030: *stop=0x8c038030u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[0],&fr[2],&f->fpscr,&fr[0]);if(status)return status;}goto L8c038032; /* fdiv fr2,fr0 */
L8c038032: *stop=0x8c038032u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038034; /* fmov.s @r2+,fr1 */
L8c038034: *stop=0x8c038034u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038036; /* fmov.s @r1+,fr2 */
L8c038036: *stop=0x8c038036u;fcopy(f,5u,2u);goto L8c038038; /* fmov fr2,fr5 */
L8c038038: *stop=0x8c038038u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[1],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c03803a; /* fsub fr1,fr5 */
L8c03803a: *stop=0x8c03803au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c03803c; /* fmul fr0,fr5 */
L8c03803c: *stop=0x8c03803cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[1],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c03803e; /* fadd fr1,fr5 */
L8c03803e: *stop=0x8c03803eu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038040; /* fmov.s @r2+,fr1 */
L8c038040: *stop=0x8c038040u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038042; /* fmov.s @r1+,fr2 */
L8c038042: *stop=0x8c038042u;fcopy(f,4u,2u);goto L8c038044; /* fmov fr2,fr4 */
L8c038044: *stop=0x8c038044u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c038046; /* fsub fr1,fr4 */
L8c038046: *stop=0x8c038046u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[0],&f->fpscr,&fr[4]);if(status)return status;}goto L8c038048; /* fmul fr0,fr4 */
L8c038048: *stop=0x8c038048u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c03804a; /* fadd fr1,fr4 */
L8c03804a: *stop=0x8c03804au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[14],&f->fpscr,&fr[4]);if(status)return status;}goto L8c03804c; /* fmul fr14,fr4 */
L8c03804c: *stop=0x8c03804cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[12],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c03804e; /* fadd fr12,fr4 */
L8c03804e: *stop=0x8c03804eu;r[1]+=12u;goto L8c038050; /* add #0xc,r1 */
L8c038050: *stop=0x8c038050u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[15],&f->fpscr,&fr[5]);if(status)return status;}goto L8c038052; /* fmul fr15,fr5 */
L8c038052: *stop=0x8c038052u;r[2]+=12u;goto L8c038054; /* add #0xc,r2 */
L8c038054: *stop=0x8c038054u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[13],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c038056; /* fadd fr13,fr5 */
L8c038056: *stop=0x8c038056u;*fpul=r[14];goto L8c038058; /* lds r14,FPUL */
L8c038058: *stop=0x8c038058u;f->bank[(f->fpscr>>21)&1u][8]=*fpul;goto L8c03805a; /* fsts FPUL,fr8 */
L8c03805a: *stop=0x8c03805au;r[0]=rd(m,bus,&io_status,0x8c0381c0u,4);if(io_status)return io_status;goto L8c03805c; /* mov.l 0x8c0381c0,r0 */
L8c03805c: *stop=0x8c03805cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]);if(io_status)return io_status;goto L8c03805e; /* fmov.s @r0,fr9 */
L8c03805e: *stop=0x8c03805eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[0],&f->fpscr,&fr[9]);if(status)return status;}goto L8c038060; /* fmul fr0,fr9 */
L8c038060: *stop=0x8c038060u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][9],&f->fpscr,fpul);if(status)return status;goto L8c038062; /* ftrc fr9,FPUL */
L8c038062: *stop=0x8c038062u;r[14]=*fpul;goto L8c038064; /* sts FPUL,r14 */
L8c038064: *stop=0x8c038064u;r[0]=4294967295u;goto L8c038066; /* mov #-0x1,r0 */
L8c038066: *stop=0x8c038066u;r[0]=r[0]&255u;goto L8c038068; /* extu.b r0,r0 */
L8c038068: *stop=0x8c038068u;if(!valid_access(r[2],4u))return -3;r[4]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[2]+=4;goto L8c03806a; /* mov.l @r2+,r4 */
L8c03806a: *stop=0x8c03806au;if(!valid_access(r[1],4u))return -3;r[5]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]+=4;goto L8c03806c; /* mov.l @r1+,r5 */
L8c03806c: *stop=0x8c03806cu;r[11]=r[4];goto L8c03806e; /* mov r4,r11 */
L8c03806e: *stop=0x8c03806eu;r[11]&=r[0];goto L8c038070; /* and r0,r11 */
L8c038070: *stop=0x8c038070u;r[13]=r[5];goto L8c038072; /* mov r5,r13 */
L8c038072: *stop=0x8c038072u;r[13]&=r[0];goto L8c038074; /* and r0,r13 */
L8c038074: *stop=0x8c038074u;r[13]-=r[11];goto L8c038076; /* sub r11,r13 */
L8c038076: *stop=0x8c038076u;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c038078; /* dmuls.l r14,r13 */
L8c038078: *stop=0x8c038078u;r[13]=*macl;if(checkpoint==0x8c038078u){*stop=0x8c03807au;return 2;}goto L8c03807a; /* sts MACL,r13 */
L8c03807a: *stop=0x8c03807au;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c03807c; /* swap.w r13,r13 */
L8c03807c: *stop=0x8c03807cu;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c03807e; /* exts.w r13,r13 */
L8c03807e: *stop=0x8c03807eu;r[11]+=r[13];goto L8c038080; /* add r13,r11 */
L8c038080: *stop=0x8c038080u;r[4]>>=8;goto L8c038082; /* shlr8 r4 */
L8c038082: *stop=0x8c038082u;r[5]>>=8;goto L8c038084; /* shlr8 r5 */
L8c038084: *stop=0x8c038084u;r[12]=r[4];goto L8c038086; /* mov r4,r12 */
L8c038086: *stop=0x8c038086u;r[12]&=r[0];goto L8c038088; /* and r0,r12 */
L8c038088: *stop=0x8c038088u;r[13]=r[5];goto L8c03808a; /* mov r5,r13 */
L8c03808a: *stop=0x8c03808au;r[13]&=r[0];goto L8c03808c; /* and r0,r13 */
L8c03808c: *stop=0x8c03808cu;r[13]-=r[12];goto L8c03808e; /* sub r12,r13 */
L8c03808e: *stop=0x8c03808eu;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c038090; /* dmuls.l r14,r13 */
L8c038090: *stop=0x8c038090u;r[13]=*macl;if(checkpoint==0x8c038090u){*stop=0x8c038092u;return 2;}goto L8c038092; /* sts MACL,r13 */
L8c038092: *stop=0x8c038092u;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c038094; /* swap.w r13,r13 */
L8c038094: *stop=0x8c038094u;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c038096; /* exts.w r13,r13 */
L8c038096: *stop=0x8c038096u;r[12]+=r[13];goto L8c038098; /* add r13,r12 */
L8c038098: *stop=0x8c038098u;r[12]<<=8;goto L8c03809a; /* shll8 r12 */
L8c03809a: *stop=0x8c03809au;r[11]|=r[12];goto L8c03809c; /* or r12,r11 */
L8c03809c: *stop=0x8c03809cu;r[4]>>=8;goto L8c03809e; /* shlr8 r4 */
L8c03809e: *stop=0x8c03809eu;r[5]>>=8;goto L8c0380a0; /* shlr8 r5 */
L8c0380a0: *stop=0x8c0380a0u;r[12]=r[4];goto L8c0380a2; /* mov r4,r12 */
L8c0380a2: *stop=0x8c0380a2u;r[12]&=r[0];goto L8c0380a4; /* and r0,r12 */
L8c0380a4: *stop=0x8c0380a4u;r[13]=r[5];goto L8c0380a6; /* mov r5,r13 */
L8c0380a6: *stop=0x8c0380a6u;r[13]&=r[0];goto L8c0380a8; /* and r0,r13 */
L8c0380a8: *stop=0x8c0380a8u;r[13]-=r[12];goto L8c0380aa; /* sub r12,r13 */
L8c0380aa: *stop=0x8c0380aau;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c0380ac; /* dmuls.l r14,r13 */
L8c0380ac: *stop=0x8c0380acu;r[13]=*macl;if(checkpoint==0x8c0380acu){*stop=0x8c0380aeu;return 2;}goto L8c0380ae; /* sts MACL,r13 */
L8c0380ae: *stop=0x8c0380aeu;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c0380b0; /* swap.w r13,r13 */
L8c0380b0: *stop=0x8c0380b0u;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c0380b2; /* exts.w r13,r13 */
L8c0380b2: *stop=0x8c0380b2u;r[12]+=r[13];goto L8c0380b4; /* add r13,r12 */
L8c0380b4: *stop=0x8c0380b4u;r[12]<<=16;goto L8c0380b6; /* shll16 r12 */
L8c0380b6: *stop=0x8c0380b6u;r[11]|=r[12];goto L8c0380b8; /* or r12,r11 */
L8c0380b8: *stop=0x8c0380b8u;r[4]>>=8;goto L8c0380ba; /* shlr8 r4 */
L8c0380ba: *stop=0x8c0380bau;r[5]>>=8;goto L8c0380bc; /* shlr8 r5 */
L8c0380bc: *stop=0x8c0380bcu;r[12]=r[4];goto L8c0380be; /* mov r4,r12 */
L8c0380be: *stop=0x8c0380beu;r[12]&=r[0];goto L8c0380c0; /* and r0,r12 */
L8c0380c0: *stop=0x8c0380c0u;r[13]=r[5];goto L8c0380c2; /* mov r5,r13 */
L8c0380c2: *stop=0x8c0380c2u;r[13]&=r[0];goto L8c0380c4; /* and r0,r13 */
L8c0380c4: *stop=0x8c0380c4u;r[13]-=r[12];goto L8c0380c6; /* sub r12,r13 */
L8c0380c6: *stop=0x8c0380c6u;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c0380c8; /* dmuls.l r14,r13 */
L8c0380c8: *stop=0x8c0380c8u;r[13]=*macl;if(checkpoint==0x8c0380c8u){*stop=0x8c0380cau;return 2;}goto L8c0380ca; /* sts MACL,r13 */
L8c0380ca: *stop=0x8c0380cau;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c0380cc; /* swap.w r13,r13 */
L8c0380cc: *stop=0x8c0380ccu;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c0380ce; /* exts.w r13,r13 */
L8c0380ce: *stop=0x8c0380ceu;r[12]+=r[13];goto L8c0380d0; /* add r13,r12 */
L8c0380d0: *stop=0x8c0380d0u;r[12]<<=16;goto L8c0380d2; /* shll16 r12 */
L8c0380d2: *stop=0x8c0380d2u;r[12]<<=8;goto L8c0380d4; /* shll8 r12 */
L8c0380d4: *stop=0x8c0380d4u;r[11]|=r[12];goto L8c0380d6; /* or r12,r11 */
L8c0380d6: *stop=0x8c0380d6u;*fpul=r[11];goto L8c0380d8; /* lds r11,FPUL */
L8c0380d8: *stop=0x8c0380d8u;f->bank[(f->fpscr>>21)&1u][9]=*fpul;goto L8c0380da; /* fsts FPUL,fr9 */
L8c0380da: *stop=0x8c0380dau;if(!valid_access(r[2],4u))return -3;r[4]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[2]+=4;goto L8c0380dc; /* mov.l @r2+,r4 */
L8c0380dc: *stop=0x8c0380dcu;if(!valid_access(r[1],4u))return -3;r[5]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[1]+=4;goto L8c0380de; /* mov.l @r1+,r5 */
L8c0380de: *stop=0x8c0380deu;r[11]=r[4];goto L8c0380e0; /* mov r4,r11 */
L8c0380e0: *stop=0x8c0380e0u;r[11]&=r[0];goto L8c0380e2; /* and r0,r11 */
L8c0380e2: *stop=0x8c0380e2u;r[13]=r[5];goto L8c0380e4; /* mov r5,r13 */
L8c0380e4: *stop=0x8c0380e4u;r[13]&=r[0];goto L8c0380e6; /* and r0,r13 */
L8c0380e6: *stop=0x8c0380e6u;r[13]-=r[11];goto L8c0380e8; /* sub r11,r13 */
L8c0380e8: *stop=0x8c0380e8u;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c0380ea; /* dmuls.l r14,r13 */
L8c0380ea: *stop=0x8c0380eau;r[13]=*macl;if(checkpoint==0x8c0380eau){*stop=0x8c0380ecu;return 2;}goto L8c0380ec; /* sts MACL,r13 */
L8c0380ec: *stop=0x8c0380ecu;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c0380ee; /* swap.w r13,r13 */
L8c0380ee: *stop=0x8c0380eeu;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c0380f0; /* exts.w r13,r13 */
L8c0380f0: *stop=0x8c0380f0u;r[11]+=r[13];goto L8c0380f2; /* add r13,r11 */
L8c0380f2: *stop=0x8c0380f2u;r[4]>>=8;goto L8c0380f4; /* shlr8 r4 */
L8c0380f4: *stop=0x8c0380f4u;r[5]>>=8;goto L8c0380f6; /* shlr8 r5 */
L8c0380f6: *stop=0x8c0380f6u;r[12]=r[4];goto L8c0380f8; /* mov r4,r12 */
L8c0380f8: *stop=0x8c0380f8u;r[12]&=r[0];goto L8c0380fa; /* and r0,r12 */
L8c0380fa: *stop=0x8c0380fau;r[13]=r[5];goto L8c0380fc; /* mov r5,r13 */
L8c0380fc: *stop=0x8c0380fcu;r[13]&=r[0];goto L8c0380fe; /* and r0,r13 */
L8c0380fe: *stop=0x8c0380feu;r[13]-=r[12];goto L8c038100; /* sub r12,r13 */
L8c038100: *stop=0x8c038100u;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c038102; /* dmuls.l r14,r13 */
L8c038102: *stop=0x8c038102u;r[13]=*macl;if(checkpoint==0x8c038102u){*stop=0x8c038104u;return 2;}goto L8c038104; /* sts MACL,r13 */
L8c038104: *stop=0x8c038104u;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c038106; /* swap.w r13,r13 */
L8c038106: *stop=0x8c038106u;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c038108; /* exts.w r13,r13 */
L8c038108: *stop=0x8c038108u;r[12]+=r[13];goto L8c03810a; /* add r13,r12 */
L8c03810a: *stop=0x8c03810au;r[12]<<=8;goto L8c03810c; /* shll8 r12 */
L8c03810c: *stop=0x8c03810cu;r[11]|=r[12];goto L8c03810e; /* or r12,r11 */
L8c03810e: *stop=0x8c03810eu;r[4]>>=8;goto L8c038110; /* shlr8 r4 */
L8c038110: *stop=0x8c038110u;r[5]>>=8;goto L8c038112; /* shlr8 r5 */
L8c038112: *stop=0x8c038112u;r[12]=r[4];goto L8c038114; /* mov r4,r12 */
L8c038114: *stop=0x8c038114u;r[12]&=r[0];goto L8c038116; /* and r0,r12 */
L8c038116: *stop=0x8c038116u;r[13]=r[5];goto L8c038118; /* mov r5,r13 */
L8c038118: *stop=0x8c038118u;r[13]&=r[0];goto L8c03811a; /* and r0,r13 */
L8c03811a: *stop=0x8c03811au;r[13]-=r[12];goto L8c03811c; /* sub r12,r13 */
L8c03811c: *stop=0x8c03811cu;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c03811e; /* dmuls.l r14,r13 */
L8c03811e: *stop=0x8c03811eu;r[13]=*macl;if(checkpoint==0x8c03811eu){*stop=0x8c038120u;return 2;}goto L8c038120; /* sts MACL,r13 */
L8c038120: *stop=0x8c038120u;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c038122; /* swap.w r13,r13 */
L8c038122: *stop=0x8c038122u;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c038124; /* exts.w r13,r13 */
L8c038124: *stop=0x8c038124u;r[12]+=r[13];goto L8c038126; /* add r13,r12 */
L8c038126: *stop=0x8c038126u;r[12]<<=16;goto L8c038128; /* shll16 r12 */
L8c038128: *stop=0x8c038128u;r[11]|=r[12];goto L8c03812a; /* or r12,r11 */
L8c03812a: *stop=0x8c03812au;r[4]>>=8;goto L8c03812c; /* shlr8 r4 */
L8c03812c: *stop=0x8c03812cu;r[5]>>=8;goto L8c03812e; /* shlr8 r5 */
L8c03812e: *stop=0x8c03812eu;r[12]=r[4];goto L8c038130; /* mov r4,r12 */
L8c038130: *stop=0x8c038130u;r[12]&=r[0];goto L8c038132; /* and r0,r12 */
L8c038132: *stop=0x8c038132u;r[13]=r[5];goto L8c038134; /* mov r5,r13 */
L8c038134: *stop=0x8c038134u;r[13]&=r[0];goto L8c038136; /* and r0,r13 */
L8c038136: *stop=0x8c038136u;r[13]-=r[12];goto L8c038138; /* sub r12,r13 */
L8c038138: *stop=0x8c038138u;sa_sh4_dmuls_state(r[13],r[14],macl,integer);goto L8c03813a; /* dmuls.l r14,r13 */
L8c03813a: *stop=0x8c03813au;r[13]=*macl;if(checkpoint==0x8c03813au){*stop=0x8c03813cu;return 2;}goto L8c03813c; /* sts MACL,r13 */
L8c03813c: *stop=0x8c03813cu;r[13]=(r[13]<<16)|(r[13]>>16);goto L8c03813e; /* swap.w r13,r13 */
L8c03813e: *stop=0x8c03813eu;r[13]=(uint32_t)(int32_t)(int16_t)r[13];goto L8c038140; /* exts.w r13,r13 */
L8c038140: *stop=0x8c038140u;r[12]+=r[13];goto L8c038142; /* add r13,r12 */
L8c038142: *stop=0x8c038142u;r[12]<<=16;goto L8c038144; /* shll16 r12 */
L8c038144: *stop=0x8c038144u;r[12]<<=8;goto L8c038146; /* shll8 r12 */
L8c038146: *stop=0x8c038146u;r[11]|=r[12];goto L8c038148; /* or r12,r11 */
L8c038148: *stop=0x8c038148u;*fpul=r[11];goto L8c03814a; /* lds r11,FPUL */
L8c03814a: *stop=0x8c03814au;f->bank[(f->fpscr>>21)&1u][10]=*fpul;goto L8c03814c; /* fsts FPUL,fr10 */
L8c03814c: *stop=0x8c03814cu;*fpul=f->bank[(f->fpscr>>21)&1u][8];goto L8c03814e; /* flds fr8,FPUL */
L8c03814e: *stop=0x8c03814eu;r[14]=*fpul;goto L8c038150; /* sts FPUL,r14 */
L8c038150: *stop=0x8c038150u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038152; /* fmov.s @r2+,fr1 */
L8c038152: *stop=0x8c038152u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038154; /* fmov.s @r1+,fr2 */
L8c038154: *stop=0x8c038154u;fcopy(f,7u,2u);goto L8c038156; /* fmov fr2,fr7 */
L8c038156: *stop=0x8c038156u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[1],&f->fpscr,&fr[7],1);if(status)return status;}goto L8c038158; /* fsub fr1,fr7 */
L8c038158: *stop=0x8c038158u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c03815a; /* fmul fr0,fr7 */
L8c03815a: *stop=0x8c03815au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[1],&f->fpscr,&fr[7],0);if(status)return status;}goto L8c03815c; /* fadd fr1,fr7 */
L8c03815c: *stop=0x8c03815cu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03815e; /* fmov.s @r2+,fr1 */
L8c03815e: *stop=0x8c03815eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038160; /* fmov.s @r1+,fr2 */
L8c038160: *stop=0x8c038160u;fcopy(f,8u,2u);goto L8c038162; /* fmov fr2,fr8 */
L8c038162: *stop=0x8c038162u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[1],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c038164; /* fsub fr1,fr8 */
L8c038164: *stop=0x8c038164u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[0],&f->fpscr,&fr[8]);if(status)return status;}goto L8c038166; /* fmul fr0,fr8 */
L8c038166: *stop=0x8c038166u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[1],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c038168; /* fadd fr1,fr8 */
L8c038168: *stop=0x8c038168u;r[1]+=4294967256u;goto L8c03816a; /* add #-0x28,r1 */
L8c03816a: *stop=0x8c03816au;r[2]+=4294967256u;goto L8c03816c; /* add #-0x28,r2 */
L8c03816c: *stop=0x8c03816cu;r[0]=r[3];goto L8c03816e; /* mov r3,r0 */
L8c03816e: *stop=0x8c03816eu;r[0]+=36u;goto L8c038170; /* add #0x24,r0 */
L8c038170: *stop=0x8c038170u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,8u,r[0]);if(io_status)return io_status;goto L8c038172; /* fmov.s fr8,@r0 */
L8c038172: *stop=0x8c038172u;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;goto L8c038174; /* fmov.s fr7,@-r0 */
L8c038174: *stop=0x8c038174u;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;goto L8c038176; /* fmov.s fr10,@-r0 */
L8c038176: *stop=0x8c038176u;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[0]);if(io_status)return io_status;goto L8c038178; /* fmov.s fr9,@-r0 */
L8c038178: *stop=0x8c038178u;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c03817a; /* fmov.s fr4,@-r0 */
L8c03817a: *stop=0x8c03817au;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c03817c; /* fmov.s fr5,@-r0 */
L8c03817c: *stop=0x8c03817cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c03817e; /* fldi1 fr6 */
L8c03817e: *stop=0x8c03817eu;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c038180; /* fmov.s fr6,@-r0 */
L8c038180: *stop=0x8c038180u;r[3]+=40u;goto L8c038182; /* add #0x28,r3 */
L8c038182: *stop=0x8c038182u;r[6]+=1u;goto L8c038184; /* add #0x1,r6 */
L8c038184: *stop=0x8c038184u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0x3f800000u;goto L8c038186; /* fldi1 fr0 */
L8c038186: *stop=0x8c038186u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c038188; /* fmov.s @r1,fr2 */
L8c038188: *stop=0x8c038188u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c03818a; /* fcmp/gt fr0,fr2 */
L8c03818a: if(!(*sr&1u)){goto L8c0381b4;}goto L8c03818c; /* bf 0x8c0381b4 */
L8c03818c: *stop=0x8c03818cu;r[0]=r[1];goto L8c03818e; /* mov r1,r0 */
L8c03818e: *stop=0x8c03818eu;r[0]+=12u;goto L8c038190; /* add #0xc,r0 */
L8c038190: *stop=0x8c038190u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038192; /* fmov.s @r0+,fr6 */
L8c038192: *stop=0x8c038192u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038194; /* fmov.s @r0+,fr5 */
L8c038194: *stop=0x8c038194u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038196; /* fmov.s @r0+,fr4 */
L8c038196: *stop=0x8c038196u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038198; /* fmov.s @r0+,fr9 */
L8c038198: *stop=0x8c038198u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03819a; /* fmov.s @r0+,fr10 */
L8c03819a: *stop=0x8c03819au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03819c; /* fmov.s @r0+,fr7 */
L8c03819c: *stop=0x8c03819cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03819e; /* fmov.s @r0+,fr8 */
L8c03819e: *stop=0x8c03819eu;r[0]=r[3];goto L8c0381a0; /* mov r3,r0 */
L8c0381a0: *stop=0x8c0381a0u;r[0]+=36u;goto L8c0381a2; /* add #0x24,r0 */
L8c0381a2: *stop=0x8c0381a2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,8u,r[0]);if(io_status)return io_status;goto L8c0381a4; /* fmov.s fr8,@r0 */
L8c0381a4: *stop=0x8c0381a4u;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;goto L8c0381a6; /* fmov.s fr7,@-r0 */
L8c0381a6: *stop=0x8c0381a6u;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;goto L8c0381a8; /* fmov.s fr10,@-r0 */
L8c0381a8: *stop=0x8c0381a8u;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[0]);if(io_status)return io_status;goto L8c0381aa; /* fmov.s fr9,@-r0 */
L8c0381aa: *stop=0x8c0381aau;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c0381ac; /* fmov.s fr4,@-r0 */
L8c0381ac: *stop=0x8c0381acu;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c0381ae; /* fmov.s fr5,@-r0 */
L8c0381ae: *stop=0x8c0381aeu;if(!valid_access(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c0381b0; /* fmov.s fr6,@-r0 */
L8c0381b0: *stop=0x8c0381b0u;r[3]+=40u;goto L8c0381b2; /* add #0x28,r3 */
L8c0381b2: *stop=0x8c0381b2u;r[6]+=1u;goto L8c0381b4; /* add #0x1,r6 */
L8c0381b4: *stop=0x8c0381b4u;r[2]=r[1];goto L8c0381b6; /* mov r1,r2 */
L8c0381b6: *stop=0x8c0381b6u;r[7]--;*sr=(*sr&~1u)|(!r[7]);goto L8c0381b8; /* dt r7 */
L8c0381b8: if((*sr&1u)){goto L8c0381c4;}goto L8c0381ba; /* bt 0x8c0381c4 */
L8c0381ba: *stop=0x8c0381bcu;r[1]+=40u;goto L8c038004; /* bra 0x8c038004 */
L8c0381c4: *stop=0x8c0381c4u;r[2]=rd(m,bus,&io_status,0x8c038294u,4);if(io_status)return io_status;goto L8c0381c6; /* mov.l 0x8c038294,r2 */
L8c0381c6: *stop=0x8c0381c6u;r[1]=rd(m,bus,&io_status,0x8c038298u,4);if(io_status)return io_status;goto L8c0381c8; /* mov.l 0x8c038298,r1 */
L8c0381c8: *stop=0x8c0381c8u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0381ca; /* fmov.s @r1+,fr2 */
L8c0381ca: *stop=0x8c0381cau;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0381cc; /* fmov.s @r1+,fr1 */
L8c0381cc: *stop=0x8c0381ccu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0381ce; /* fmov.s @r1+,fr0 */
L8c0381ce: *stop=0x8c0381ceu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0381d0; /* fmov.s @r1+,fr3 */
L8c0381d0: *stop=0x8c0381d0u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0381d2; /* fmov.s @r1+,fr4 */
L8c0381d2: *stop=0x8c0381d2u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0381d4; /* fmov.s @r1+,fr5 */
L8c0381d4: *stop=0x8c0381d4u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0381d6; /* fmov.s @r1+,fr6 */
L8c0381d6: *stop=0x8c0381d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[11],&f->fpscr,&fr[5]);if(status)return status;}goto L8c0381d8; /* fmul fr11,fr5 */
L8c0381d8: *stop=0x8c0381d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[11],&f->fpscr,&fr[6]);if(status)return status;}goto L8c0381da; /* fmul fr11,fr6 */
L8c0381da: *stop=0x8c0381dau;r[14]+=32u;goto L8c0381dc; /* add #0x20,r14 */
L8c0381dc: *stop=0x8c0381dcu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c0381de; /* fmov.s fr4,@-r14 */
L8c0381de: *stop=0x8c0381deu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c0381e0; /* fmov.s fr3,@-r14 */
L8c0381e0: *stop=0x8c0381e0u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c0381e2; /* fmov.s fr6,@-r14 */
L8c0381e2: *stop=0x8c0381e2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c0381e4; /* fmov.s fr5,@-r14 */
L8c0381e4: *stop=0x8c0381e4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c0381e6; /* fmov.s fr2,@-r14 */
L8c0381e6: *stop=0x8c0381e6u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c0381e8; /* fmov.s fr1,@-r14 */
L8c0381e8: *stop=0x8c0381e8u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c0381ea; /* fmov.s fr0,@-r14 */
L8c0381ea: *stop=0x8c0381eau;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c0381ec; /* mov.l r2,@-r14 */
L8c0381ec: *stop=0x8c0381ecu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c0381ee; /* pref @r14 */
L8c0381ee: *stop=0x8c0381eeu;r[14]+=32u;goto L8c0381f0; /* add #0x20,r14 */
L8c0381f0: *stop=0x8c0381f0u;r[1]=rd(m,bus,&io_status,0x8c03829cu,4);if(io_status)return io_status;goto L8c0381f2; /* mov.l 0x8c03829c,r1 */
L8c0381f2: *stop=0x8c0381f2u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0381f4; /* fmov.s @r1+,fr2 */
L8c0381f4: *stop=0x8c0381f4u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0381f6; /* fmov.s @r1+,fr1 */
L8c0381f6: *stop=0x8c0381f6u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0381f8; /* fmov.s @r1+,fr0 */
L8c0381f8: *stop=0x8c0381f8u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0381fa; /* fmov.s @r1+,fr3 */
L8c0381fa: *stop=0x8c0381fau;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0381fc; /* fmov.s @r1+,fr4 */
L8c0381fc: *stop=0x8c0381fcu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0381fe; /* fmov.s @r1+,fr5 */
L8c0381fe: *stop=0x8c0381feu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038200; /* fmov.s @r1+,fr6 */
L8c038200: *stop=0x8c038200u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[11],&f->fpscr,&fr[5]);if(status)return status;}goto L8c038202; /* fmul fr11,fr5 */
L8c038202: *stop=0x8c038202u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[11],&f->fpscr,&fr[6]);if(status)return status;}goto L8c038204; /* fmul fr11,fr6 */
L8c038204: *stop=0x8c038204u;r[14]+=32u;goto L8c038206; /* add #0x20,r14 */
L8c038206: *stop=0x8c038206u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c038208; /* fmov.s fr4,@-r14 */
L8c038208: *stop=0x8c038208u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c03820a; /* fmov.s fr3,@-r14 */
L8c03820a: *stop=0x8c03820au;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c03820c; /* fmov.s fr6,@-r14 */
L8c03820c: *stop=0x8c03820cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c03820e; /* fmov.s fr5,@-r14 */
L8c03820e: *stop=0x8c03820eu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c038210; /* fmov.s fr2,@-r14 */
L8c038210: *stop=0x8c038210u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c038212; /* fmov.s fr1,@-r14 */
L8c038212: *stop=0x8c038212u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c038214; /* fmov.s fr0,@-r14 */
L8c038214: *stop=0x8c038214u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c038216; /* mov.l r2,@-r14 */
L8c038216: *stop=0x8c038216u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c038218; /* pref @r14 */
L8c038218: *stop=0x8c038218u;r[14]+=32u;goto L8c03821a; /* add #0x20,r14 */
L8c03821a: *stop=0x8c03821au;r[0]=r[6];goto L8c03821c; /* mov r6,r0 */
L8c03821c: *stop=0x8c03821cu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c03821e; /* cmp/eq #0x3,r0 */
L8c03821e: if((*sr&1u)){goto L8c03824a;}goto L8c038220; /* bt 0x8c03824a */
L8c038220: *stop=0x8c038220u;r[1]=rd(m,bus,&io_status,0x8c0382a0u,4);if(io_status)return io_status;goto L8c038222; /* mov.l 0x8c0382a0,r1 */
L8c038222: *stop=0x8c038222u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038224; /* fmov.s @r1+,fr2 */
L8c038224: *stop=0x8c038224u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038226; /* fmov.s @r1+,fr1 */
L8c038226: *stop=0x8c038226u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038228; /* fmov.s @r1+,fr0 */
L8c038228: *stop=0x8c038228u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03822a; /* fmov.s @r1+,fr3 */
L8c03822a: *stop=0x8c03822au;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03822c; /* fmov.s @r1+,fr4 */
L8c03822c: *stop=0x8c03822cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03822e; /* fmov.s @r1+,fr5 */
L8c03822e: *stop=0x8c03822eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038230; /* fmov.s @r1+,fr6 */
L8c038230: *stop=0x8c038230u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[11],&f->fpscr,&fr[5]);if(status)return status;}goto L8c038232; /* fmul fr11,fr5 */
L8c038232: *stop=0x8c038232u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[11],&f->fpscr,&fr[6]);if(status)return status;}goto L8c038234; /* fmul fr11,fr6 */
L8c038234: *stop=0x8c038234u;r[14]+=32u;goto L8c038236; /* add #0x20,r14 */
L8c038236: *stop=0x8c038236u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c038238; /* fmov.s fr4,@-r14 */
L8c038238: *stop=0x8c038238u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c03823a; /* fmov.s fr3,@-r14 */
L8c03823a: *stop=0x8c03823au;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c03823c; /* fmov.s fr6,@-r14 */
L8c03823c: *stop=0x8c03823cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c03823e; /* fmov.s fr5,@-r14 */
L8c03823e: *stop=0x8c03823eu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c038240; /* fmov.s fr2,@-r14 */
L8c038240: *stop=0x8c038240u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c038242; /* fmov.s fr1,@-r14 */
L8c038242: *stop=0x8c038242u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c038244; /* fmov.s fr0,@-r14 */
L8c038244: *stop=0x8c038244u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c038246; /* mov.l r2,@-r14 */
L8c038246: *stop=0x8c038246u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c038248; /* pref @r14 */
L8c038248: *stop=0x8c038248u;r[14]+=32u;goto L8c03824a; /* add #0x20,r14 */
L8c03824a: *stop=0x8c03824au;r[2]=rd(m,bus,&io_status,0x8c0382a4u,4);if(io_status)return io_status;goto L8c03824c; /* mov.l 0x8c0382a4,r2 */
L8c03824c: *stop=0x8c03824cu;r[1]=rd(m,bus,&io_status,0x8c0382a8u,4);if(io_status)return io_status;goto L8c03824e; /* mov.l 0x8c0382a8,r1 */
L8c03824e: *stop=0x8c03824eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038250; /* fmov.s @r1+,fr2 */
L8c038250: *stop=0x8c038250u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038252; /* fmov.s @r1+,fr1 */
L8c038252: *stop=0x8c038252u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038254; /* fmov.s @r1+,fr0 */
L8c038254: *stop=0x8c038254u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038256; /* fmov.s @r1+,fr3 */
L8c038256: *stop=0x8c038256u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038258; /* fmov.s @r1+,fr4 */
L8c038258: *stop=0x8c038258u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03825a; /* fmov.s @r1+,fr5 */
L8c03825a: *stop=0x8c03825au;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03825c; /* fmov.s @r1+,fr6 */
L8c03825c: *stop=0x8c03825cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[11],&f->fpscr,&fr[5]);if(status)return status;}goto L8c03825e; /* fmul fr11,fr5 */
L8c03825e: *stop=0x8c03825eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[11],&f->fpscr,&fr[6]);if(status)return status;}goto L8c038260; /* fmul fr11,fr6 */
L8c038260: *stop=0x8c038260u;r[14]+=32u;goto L8c038262; /* add #0x20,r14 */
L8c038262: *stop=0x8c038262u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c038264; /* fmov.s fr4,@-r14 */
L8c038264: *stop=0x8c038264u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c038266; /* fmov.s fr3,@-r14 */
L8c038266: *stop=0x8c038266u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c038268; /* fmov.s fr6,@-r14 */
L8c038268: *stop=0x8c038268u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c03826a; /* fmov.s fr5,@-r14 */
L8c03826a: *stop=0x8c03826au;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c03826c; /* fmov.s fr2,@-r14 */
L8c03826c: *stop=0x8c03826cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c03826e; /* fmov.s fr1,@-r14 */
L8c03826e: *stop=0x8c03826eu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c038270; /* fmov.s fr0,@-r14 */
L8c038270: *stop=0x8c038270u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c038272; /* mov.l r2,@-r14 */
L8c038272: *stop=0x8c038272u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c038274; /* pref @r14 */
L8c038274: *stop=0x8c038274u;r[14]+=32u;goto L8c038276; /* add #0x20,r14 */
L8c038276: *stop=0x8c038276u;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c038278; /* mov.l @r15+,r13 */
L8c038278: *stop=0x8c038278u;if(!valid_access(r[15],4u))return -3;r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c03827a; /* mov.l @r15+,r12 */
L8c03827a: *stop=0x8c03827au;if(!valid_access(r[15],4u))return -3;r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c03827c; /* mov.l @r15+,r11 */
L8c03827c: *stop=0x8c03827cu;r[1]=32u;goto L8c03827e; /* mov #0x20,r1 */
L8c03827e: *stop=0x8c03827eu;r[0]=24u;goto L8c038280; /* mov #0x18,r0 */
L8c038280: *stop=0x8c038280u;r[1]=(r[0]&0x80000000u)?((r[0]&31u)?r[1]>>((0u-r[0])&31u):0u):(r[1]<<(r[0]&31u));goto L8c038282; /* shld r0,r1 */
L8c038282: *stop=0x8c038282u;r[1]=~r[1];goto L8c038284; /* not r1,r1 */
L8c038284: *stop=0x8c038284u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c038286; /* stc SR,r0 */
L8c038286: *stop=0x8c038286u;r[0]&=r[1];goto L8c038288; /* and r1,r0 */
L8c038288: *stop=0x8c038288u;r[1]=~r[1];goto L8c03828a; /* not r1,r1 */
L8c03828a: *stop=0x8c03828au;r[0]|=r[1];goto L8c03828c; /* or r1,r0 */
L8c03828c: *stop=0x8c03828cu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;if(checkpoint==0x8c03828cu){*stop=0x8c03828eu;return 2;}goto L8c03828e; /* ldc r0,SR */
L8c03828e: *stop=0x8c03828eu;r[1]=r[12];goto L8c038290; /* mov r12,r1 */
L8c038290: *stop=0x8c038290u;if(*pr!=entry_pr)return -3;*stop=0x8c038292u;if(!valid_access(r[15],4u))return -3;r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_clipping_state_8c038516(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,uint32_t entry,uint32_t checkpoint){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)checkpoint;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 switch(entry){case 0x8c038516u:goto L8c038516;case 0x8c03851au:goto L8c03851a;case 0x8c03863au:goto L8c03863a;default:return -2;}
L8c038516: *stop=0x8c038516u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c038518; /* mov.l r12,@-r15 */
L8c038518: *stop=0x8c038518u;r[12]=r[1];goto L8c03851a; /* mov r1,r12 */
L8c03851a: *stop=0x8c03851au;r[1]=32u;goto L8c03851c; /* mov #0x20,r1 */
L8c03851c: *stop=0x8c03851cu;r[0]=24u;goto L8c03851e; /* mov #0x18,r0 */
L8c03851e: *stop=0x8c03851eu;r[1]=(r[0]&0x80000000u)?((r[0]&31u)?r[1]>>((0u-r[0])&31u):0u):(r[1]<<(r[0]&31u));goto L8c038520; /* shld r0,r1 */
L8c038520: *stop=0x8c038520u;r[1]=~r[1];goto L8c038522; /* not r1,r1 */
L8c038522: *stop=0x8c038522u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c038524; /* stc SR,r0 */
L8c038524: *stop=0x8c038524u;r[0]&=r[1];goto L8c038526; /* and r1,r0 */
L8c038526: *stop=0x8c038526u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;if(checkpoint==0x8c038526u){*stop=0x8c038528u;return 2;}goto L8c038528; /* ldc r0,SR */
L8c038528: *stop=0x8c038528u;r[0]=rd(m,bus,&io_status,0x8c038654u,4);if(io_status)return io_status;goto L8c03852a; /* mov.l 0x8c038654,r0 */
L8c03852a: *stop=0x8c03852au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][9]=0x3f800000u;goto L8c03852c; /* fldi1 fr9 */
L8c03852c: *stop=0x8c03852cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]=0x3f800000u;goto L8c03852e; /* fldi1 fr10 */
L8c03852e: *stop=0x8c03852eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038530; /* fmov.s @r0+,fr14 */
L8c038530: *stop=0x8c038530u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[9],&fr[14],&f->fpscr,&fr[9]);if(status)return status;}goto L8c038532; /* fdiv fr14,fr9 */
L8c038532: *stop=0x8c038532u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038534; /* fmov.s @r0+,fr15 */
L8c038534: *stop=0x8c038534u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[10],&fr[15],&f->fpscr,&fr[10]);if(status)return status;}goto L8c038536; /* fdiv fr15,fr10 */
L8c038536: *stop=0x8c038536u;r[0]=rd(m,bus,&io_status,0x8c038658u,4);if(io_status)return io_status;goto L8c038538; /* mov.l 0x8c038658,r0 */
L8c038538: *stop=0x8c038538u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03853a; /* fmov.s @r0+,fr12 */
L8c03853a: *stop=0x8c03853au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03853c; /* fmov.s @r0+,fr13 */
L8c03853c: *stop=0x8c03853cu;r[1]=rd(m,bus,&io_status,0x8c03865cu,4);if(io_status)return io_status;goto L8c03853e; /* mov.l 0x8c03865c,r1 */
L8c03853e: *stop=0x8c03853eu;if(!valid_access(r[13]+0u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x0u,2);if(io_status)return io_status;goto L8c038540; /* mov.w @(0x0,r13),r0 */
L8c038540: *stop=0x8c038540u;r[0]+=r[12];goto L8c038542; /* add r12,r0 */
L8c038542: *stop=0x8c038542u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038544; /* fmov.s @r0+,fr0 */
L8c038544: *stop=0x8c038544u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038546; /* fmov.s @r0+,fr1 */
L8c038546: *stop=0x8c038546u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038548; /* fmov.s @r0+,fr2 */
L8c038548: *stop=0x8c038548u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c03854a; /* fldi0 fr3 */
L8c03854a: *stop=0x8c03854au;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c03854c; /* ftrv xmtrx,fv0 */
L8c03854c: *stop=0x8c03854cu;r[0]=rd(m,bus,&io_status,0x8c038660u,4);if(io_status)return io_status;goto L8c03854e; /* mov.l 0x8c038660,r0 */
L8c03854e: *stop=0x8c03854eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c038550; /* fmov.s @r0,fr3 */
L8c038550: *stop=0x8c038550u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c038552; /* fmul fr3,fr0 */
L8c038552: *stop=0x8c038552u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c038554; /* fadd fr3,fr0 */
L8c038554: *stop=0x8c038554u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c038556; /* fmul fr3,fr1 */
L8c038556: *stop=0x8c038556u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c038558; /* fadd fr3,fr1 */
L8c038558: *stop=0x8c038558u;fcopy(f,3u,0u);goto L8c03855a; /* fmov fr0,fr3 */
L8c03855a: *stop=0x8c03855au;fcopy(f,4u,1u);goto L8c03855c; /* fmov fr1,fr4 */
L8c03855c: *stop=0x8c03855cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c03855e; /* fmov.s fr4,@r1 */
L8c03855e: *stop=0x8c03855eu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c038560; /* fmov.s fr3,@-r1 */
L8c038560: *stop=0x8c038560u;if(!valid_ram(*gbr+0x48u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x48u,4);if(io_status)return io_status;goto L8c038562; /* mov.l @(0x48,GBR),r0 */
L8c038562: *stop=0x8c038562u;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c038564; /* mov.l r0,@-r1 */
L8c038564: *stop=0x8c038564u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038566; /* fmov.s @r8+,fr3 */
L8c038566: *stop=0x8c038566u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038568; /* fmov.s @r8+,fr4 */
L8c038568: *stop=0x8c038568u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03856a; /* fmov.s @r8+,fr5 */
L8c03856a: *stop=0x8c03856au;if(!valid_access(r[8],4u))return -3;r[0]=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[8]+=4;goto L8c03856c; /* mov.l @r8+,r0 */
L8c03856c: *stop=0x8c03856cu;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c03856e; /* mov.l r0,@-r1 */
L8c03856e: *stop=0x8c03856eu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c038570; /* fmov.s fr3,@-r1 */
L8c038570: *stop=0x8c038570u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c038572; /* fmov.s fr4,@-r1 */
L8c038572: *stop=0x8c038572u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;goto L8c038574; /* fmov.s fr5,@-r1 */
L8c038574: *stop=0x8c038574u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c038576; /* fldi1 fr6 */
L8c038576: *stop=0x8c038576u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[5],&f->fpscr,&fr[6]);if(status)return status;}goto L8c038578; /* fdiv fr5,fr6 */
L8c038578: *stop=0x8c038578u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[12],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c03857a; /* fsub fr12,fr3 */
L8c03857a: *stop=0x8c03857au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c03857c; /* fmul fr6,fr3 */
L8c03857c: *stop=0x8c03857cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c03857e; /* fmul fr9,fr3 */
L8c03857e: *stop=0x8c03857eu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c038580; /* fmov.s fr3,@-r1 */
L8c038580: *stop=0x8c038580u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[13],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c038582; /* fsub fr13,fr4 */
L8c038582: *stop=0x8c038582u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[6],&f->fpscr,&fr[4]);if(status)return status;}goto L8c038584; /* fmul fr6,fr4 */
L8c038584: *stop=0x8c038584u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c038586; /* fmul fr10,fr4 */
L8c038586: *stop=0x8c038586u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c038588; /* fmov.s fr4,@-r1 */
L8c038588: *stop=0x8c038588u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[1]);if(io_status)return io_status;goto L8c03858a; /* fmov.s fr6,@-r1 */
L8c03858a: *stop=0x8c03858au;r[8]+=4294967280u;goto L8c03858c; /* add #-0x10,r8 */
L8c03858c: *stop=0x8c03858cu;r[1]+=76u;goto L8c03858e; /* add #0x4c,r1 */
L8c03858e: *stop=0x8c03858eu;if(!valid_access(r[13]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c038590; /* mov.w @(0x2,r13),r0 */
L8c038590: *stop=0x8c038590u;r[0]+=r[12];goto L8c038592; /* add r12,r0 */
L8c038592: *stop=0x8c038592u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038594; /* fmov.s @r0+,fr0 */
L8c038594: *stop=0x8c038594u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038596; /* fmov.s @r0+,fr1 */
L8c038596: *stop=0x8c038596u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038598; /* fmov.s @r0+,fr2 */
L8c038598: *stop=0x8c038598u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c03859a; /* fldi0 fr3 */
L8c03859a: *stop=0x8c03859au;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c03859c; /* ftrv xmtrx,fv0 */
L8c03859c: *stop=0x8c03859cu;r[0]=rd(m,bus,&io_status,0x8c038660u,4);if(io_status)return io_status;goto L8c03859e; /* mov.l 0x8c038660,r0 */
L8c03859e: *stop=0x8c03859eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0385a0; /* fmov.s @r0,fr3 */
L8c0385a0: *stop=0x8c0385a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c0385a2; /* fmul fr3,fr0 */
L8c0385a2: *stop=0x8c0385a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c0385a4; /* fadd fr3,fr0 */
L8c0385a4: *stop=0x8c0385a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c0385a6; /* fmul fr3,fr1 */
L8c0385a6: *stop=0x8c0385a6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c0385a8; /* fadd fr3,fr1 */
L8c0385a8: *stop=0x8c0385a8u;fcopy(f,3u,0u);goto L8c0385aa; /* fmov fr0,fr3 */
L8c0385aa: *stop=0x8c0385aau;fcopy(f,4u,1u);goto L8c0385ac; /* fmov fr1,fr4 */
L8c0385ac: *stop=0x8c0385acu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c0385ae; /* fmov.s fr4,@r1 */
L8c0385ae: *stop=0x8c0385aeu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0385b0; /* fmov.s fr3,@-r1 */
L8c0385b0: *stop=0x8c0385b0u;if(!valid_ram(*gbr+0x4cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x4cu,4);if(io_status)return io_status;goto L8c0385b2; /* mov.l @(0x4c,GBR),r0 */
L8c0385b2: *stop=0x8c0385b2u;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c0385b4; /* mov.l r0,@-r1 */
L8c0385b4: *stop=0x8c0385b4u;if(!valid_access(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[9]);if(io_status)return io_status;r[9]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0385b6; /* fmov.s @r9+,fr3 */
L8c0385b6: *stop=0x8c0385b6u;if(!valid_access(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[9]);if(io_status)return io_status;r[9]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0385b8; /* fmov.s @r9+,fr4 */
L8c0385b8: *stop=0x8c0385b8u;if(!valid_access(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[9]);if(io_status)return io_status;r[9]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0385ba; /* fmov.s @r9+,fr5 */
L8c0385ba: *stop=0x8c0385bau;if(!valid_access(r[9],4u))return -3;r[0]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[9]+=4;goto L8c0385bc; /* mov.l @r9+,r0 */
L8c0385bc: *stop=0x8c0385bcu;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c0385be; /* mov.l r0,@-r1 */
L8c0385be: *stop=0x8c0385beu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0385c0; /* fmov.s fr3,@-r1 */
L8c0385c0: *stop=0x8c0385c0u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c0385c2; /* fmov.s fr4,@-r1 */
L8c0385c2: *stop=0x8c0385c2u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;goto L8c0385c4; /* fmov.s fr5,@-r1 */
L8c0385c4: *stop=0x8c0385c4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c0385c6; /* fldi1 fr7 */
L8c0385c6: *stop=0x8c0385c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[7],&fr[5],&f->fpscr,&fr[7]);if(status)return status;}goto L8c0385c8; /* fdiv fr5,fr7 */
L8c0385c8: *stop=0x8c0385c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[12],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c0385ca; /* fsub fr12,fr3 */
L8c0385ca: *stop=0x8c0385cau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0385cc; /* fmul fr7,fr3 */
L8c0385cc: *stop=0x8c0385ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0385ce; /* fmul fr9,fr3 */
L8c0385ce: *stop=0x8c0385ceu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0385d0; /* fmov.s fr3,@-r1 */
L8c0385d0: *stop=0x8c0385d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[13],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c0385d2; /* fsub fr13,fr4 */
L8c0385d2: *stop=0x8c0385d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[7],&f->fpscr,&fr[4]);if(status)return status;}goto L8c0385d4; /* fmul fr7,fr4 */
L8c0385d4: *stop=0x8c0385d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c0385d6; /* fmul fr10,fr4 */
L8c0385d6: *stop=0x8c0385d6u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c0385d8; /* fmov.s fr4,@-r1 */
L8c0385d8: *stop=0x8c0385d8u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[1]);if(io_status)return io_status;goto L8c0385da; /* fmov.s fr7,@-r1 */
L8c0385da: *stop=0x8c0385dau;r[9]+=4294967280u;goto L8c0385dc; /* add #-0x10,r9 */
L8c0385dc: *stop=0x8c0385dcu;r[1]+=76u;goto L8c0385de; /* add #0x4c,r1 */
L8c0385de: *stop=0x8c0385deu;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c0385e0; /* mov.w @(0x4,r13),r0 */
L8c0385e0: *stop=0x8c0385e0u;r[0]+=r[12];goto L8c0385e2; /* add r12,r0 */
L8c0385e2: *stop=0x8c0385e2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0385e4; /* fmov.s @r0+,fr0 */
L8c0385e4: *stop=0x8c0385e4u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0385e6; /* fmov.s @r0+,fr1 */
L8c0385e6: *stop=0x8c0385e6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0385e8; /* fmov.s @r0+,fr2 */
L8c0385e8: *stop=0x8c0385e8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0385ea; /* fldi0 fr3 */
L8c0385ea: *stop=0x8c0385eau;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c0385ec; /* ftrv xmtrx,fv0 */
L8c0385ec: *stop=0x8c0385ecu;r[0]=rd(m,bus,&io_status,0x8c038660u,4);if(io_status)return io_status;goto L8c0385ee; /* mov.l 0x8c038660,r0 */
L8c0385ee: *stop=0x8c0385eeu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0385f0; /* fmov.s @r0,fr3 */
L8c0385f0: *stop=0x8c0385f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c0385f2; /* fmul fr3,fr0 */
L8c0385f2: *stop=0x8c0385f2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c0385f4; /* fadd fr3,fr0 */
L8c0385f4: *stop=0x8c0385f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c0385f6; /* fmul fr3,fr1 */
L8c0385f6: *stop=0x8c0385f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c0385f8; /* fadd fr3,fr1 */
L8c0385f8: *stop=0x8c0385f8u;fcopy(f,3u,0u);goto L8c0385fa; /* fmov fr0,fr3 */
L8c0385fa: *stop=0x8c0385fau;fcopy(f,4u,1u);goto L8c0385fc; /* fmov fr1,fr4 */
L8c0385fc: *stop=0x8c0385fcu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c0385fe; /* fmov.s fr4,@r1 */
L8c0385fe: *stop=0x8c0385feu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c038600; /* fmov.s fr3,@-r1 */
L8c038600: *stop=0x8c038600u;if(!valid_ram(*gbr+0x50u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x50u,4);if(io_status)return io_status;goto L8c038602; /* mov.l @(0x50,GBR),r0 */
L8c038602: *stop=0x8c038602u;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c038604; /* mov.l r0,@-r1 */
L8c038604: *stop=0x8c038604u;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[10]);if(io_status)return io_status;r[10]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038606; /* fmov.s @r10+,fr3 */
L8c038606: *stop=0x8c038606u;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[10]);if(io_status)return io_status;r[10]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038608; /* fmov.s @r10+,fr4 */
L8c038608: *stop=0x8c038608u;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[10]);if(io_status)return io_status;r[10]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03860a; /* fmov.s @r10+,fr5 */
L8c03860a: *stop=0x8c03860au;if(!valid_access(r[10],4u))return -3;r[0]=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[10]+=4;goto L8c03860c; /* mov.l @r10+,r0 */
L8c03860c: *stop=0x8c03860cu;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c03860e; /* mov.l r0,@-r1 */
L8c03860e: *stop=0x8c03860eu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c038610; /* fmov.s fr3,@-r1 */
L8c038610: *stop=0x8c038610u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c038612; /* fmov.s fr4,@-r1 */
L8c038612: *stop=0x8c038612u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;goto L8c038614; /* fmov.s fr5,@-r1 */
L8c038614: *stop=0x8c038614u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][8]=0x3f800000u;goto L8c038616; /* fldi1 fr8 */
L8c038616: *stop=0x8c038616u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c038618; /* fdiv fr5,fr8 */
L8c038618: *stop=0x8c038618u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[12],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c03861a; /* fsub fr12,fr3 */
L8c03861a: *stop=0x8c03861au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c03861c; /* fmul fr8,fr3 */
L8c03861c: *stop=0x8c03861cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c03861e; /* fmul fr9,fr3 */
L8c03861e: *stop=0x8c03861eu;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c038620; /* fmov.s fr3,@-r1 */
L8c038620: *stop=0x8c038620u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[13],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c038622; /* fsub fr13,fr4 */
L8c038622: *stop=0x8c038622u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[8],&f->fpscr,&fr[4]);if(status)return status;}goto L8c038624; /* fmul fr8,fr4 */
L8c038624: *stop=0x8c038624u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c038626; /* fmul fr10,fr4 */
L8c038626: *stop=0x8c038626u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;goto L8c038628; /* fmov.s fr4,@-r1 */
L8c038628: *stop=0x8c038628u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[1]);if(io_status)return io_status;goto L8c03862a; /* fmov.s fr8,@-r1 */
L8c03862a: *stop=0x8c03862au;r[10]+=4294967280u;goto L8c03862c; /* add #-0x10,r10 */
L8c03862c: *stop=0x8c03862cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0x3f800000u;goto L8c03862e; /* fldi1 fr0 */
L8c03862e: *stop=0x8c03862eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][6],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c038630; /* fcmp/gt fr0,fr6 */
L8c038630: if((*sr&1u)){goto L8c038664;}goto L8c038632; /* bt 0x8c038664 */
L8c038632: *stop=0x8c038632u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c038634; /* fcmp/gt fr0,fr7 */
L8c038634: if((*sr&1u)){goto L8c038664;}goto L8c038636; /* bt 0x8c038664 */
L8c038636: *stop=0x8c038636u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][8],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c038638; /* fcmp/gt fr0,fr8 */
L8c038638: if((*sr&1u)){goto L8c038664;}goto L8c03863a; /* bt 0x8c038664 */
L8c03863a: *stop=0x8c03863au;r[1]=32u;goto L8c03863c; /* mov #0x20,r1 */
L8c03863c: *stop=0x8c03863cu;r[0]=24u;goto L8c03863e; /* mov #0x18,r0 */
L8c03863e: *stop=0x8c03863eu;r[1]=(r[0]&0x80000000u)?((r[0]&31u)?r[1]>>((0u-r[0])&31u):0u):(r[1]<<(r[0]&31u));goto L8c038640; /* shld r0,r1 */
L8c038640: *stop=0x8c038640u;r[1]=~r[1];goto L8c038642; /* not r1,r1 */
L8c038642: *stop=0x8c038642u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c038644; /* stc SR,r0 */
L8c038644: *stop=0x8c038644u;r[0]&=r[1];goto L8c038646; /* and r1,r0 */
L8c038646: *stop=0x8c038646u;r[1]=~r[1];goto L8c038648; /* not r1,r1 */
L8c038648: *stop=0x8c038648u;r[0]|=r[1];goto L8c03864a; /* or r1,r0 */
L8c03864a: *stop=0x8c03864au;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;if(checkpoint==0x8c03864au){*stop=0x8c03864cu;return 2;}goto L8c03864c; /* ldc r0,SR */
L8c03864c: *stop=0x8c03864cu;r[1]=r[12];goto L8c03864e; /* mov r12,r1 */
L8c03864e: *stop=0x8c03864eu;if(*pr!=entry_pr)return -3;*stop=0x8c038650u;if(!valid_access(r[15],4u))return -3;r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
L8c038664: *stop=0x8c038666u;if(!call)return -2;return call(ctx,0x8c037ff4u,m,f,sr,r,pr); /* bra 0x8c037ff4 */
}
