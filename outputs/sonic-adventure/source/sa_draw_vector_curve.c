/* Unsigned-label vector interpolation; explicit GBR/queue state.
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
SA_STAGE_EXPORT int sa_vector_curve_8c609cb0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c609cb0;
L8c609cb0: *stop=0x8c609cb0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c609cb2; /* mov.l r14,@-r15 */
L8c609cb2: *stop=0x8c609cb2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c609cb4; /* mov.l r13,@-r15 */
L8c609cb4: *stop=0x8c609cb4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c609cb6; /* sts.l PR,@-r15 */
L8c609cb6: *stop=0x8c609cb6u;r[15]+=4294967288u;goto L8c609cb8; /* add #-0x8,r15 */
L8c609cb8: *stop=0x8c609cb8u;r[2]=rd(m,bus,&io_status,0x8c609eacu,4);if(io_status)return io_status;goto L8c609cba; /* mov.l 0x8c609eac,r2 */
L8c609cba: *stop=0x8c609cbau;r[13]=r[15];goto L8c609cbc; /* mov r15,r13 */
L8c609cbc: *stop=0x8c609cbcu;r[3]=rd(m,bus,&io_status,0x8c609eb0u,4);if(io_status)return io_status;goto L8c609cbe; /* mov.l 0x8c609eb0,r3 */
L8c609cbe: *stop=0x8c609cbeu;r[1]=r[15];goto L8c609cc0; /* mov r15,r1 */
L8c609cc0: target=r[3];*pr=0x8c609cc4u;*stop=0x8c609cc2u;r[0]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c609cc4; /* jsr @r3 */
L8c609cc4: *stop=0x8c609cc4u;r[14]=r[5];goto L8c609cc6; /* mov r5,r14 */
L8c609cc6: *stop=0x8c609cc6u;r[14]<<=2;goto L8c609cc8; /* shll2 r14 */
L8c609cc8: *stop=0x8c609cc8u;r[1]=r[5];goto L8c609cca; /* mov r5,r1 */
L8c609cca: *stop=0x8c609ccau;r[3]=rd(m,bus,&io_status,0x8c609eb4u,4);if(io_status)return io_status;goto L8c609ccc; /* mov.l 0x8c609eb4,r3 */
L8c609ccc: *stop=0x8c609cccu;r[14]<<=2;goto L8c609cce; /* shll2 r14 */
L8c609cce: *stop=0x8c609cceu;r[1]+=1u;goto L8c609cd0; /* add #0x1,r1 */
L8c609cd0: *stop=0x8c609cd0u;r[14]+=r[4];goto L8c609cd2; /* add r4,r14 */
L8c609cd2: target=r[3];*pr=0x8c609cd6u;*stop=0x8c609cd4u;r[0]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c609cd6; /* jsr @r3 */
L8c609cd6: *stop=0x8c609cd6u;r[0]<<=2;goto L8c609cd8; /* shll2 r0 */
L8c609cd8: *stop=0x8c609cd8u;if(!valid_access(r[14],4u))return -3;r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c609cda; /* mov.l @r14,r3 */
L8c609cda: *stop=0x8c609cdau;r[0]<<=2;goto L8c609cdc; /* shll2 r0 */
L8c609cdc: *stop=0x8c609cdcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c609cde; /* fldi0 fr2 */
L8c609cde: *stop=0x8c609cdeu;r[4]+=r[0];goto L8c609ce0; /* add r0,r4 */
L8c609ce0: *stop=0x8c609ce0u;if(!valid_access(r[4],4u))return -3;r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c609ce2; /* mov.l @r4,r1 */
L8c609ce2: *stop=0x8c609ce2u;r[1]-=r[3];goto L8c609ce4; /* sub r3,r1 */
L8c609ce4: *stop=0x8c609ce4u;*fpul=r[1];goto L8c609ce6; /* lds r1,FPUL */
L8c609ce6: *stop=0x8c609ce6u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c609ce8; /* float FPUL,fr3 */
L8c609ce8: *stop=0x8c609ce8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c609cea; /* fcmp/gt fr3,fr2 */
L8c609cea: take=!(*sr&1u);*stop=0x8c609cecu;fcopy(f,4u,3u);if(take)goto L8c609cf8;goto L8c609cee; /* bf/s 0x8c609cf8 */
L8c609cee: *stop=0x8c609ceeu;r[0]=0x8c609eb8u;goto L8c609cf0; /* mova 0x8c609eb8,r0 */
L8c609cf0: *stop=0x8c609cf0u;fcopy(f,3u,4u);goto L8c609cf2; /* fmov fr4,fr3 */
L8c609cf2: *stop=0x8c609cf2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;goto L8c609cf4; /* fmov.s @r0,fr1 */
L8c609cf4: *stop=0x8c609cf6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c609cfa; /* bra 0x8c609cfa */
L8c609cf8: *stop=0x8c609cf8u;fcopy(f,3u,4u);goto L8c609cfa; /* fmov fr4,fr3 */
L8c609cfa: *stop=0x8c609cfau;r[1]=rd(m,bus,&io_status,0x8c609ebcu,4);if(io_status)return io_status;goto L8c609cfc; /* mov.l 0x8c609ebc,r1 */
L8c609cfc: *stop=0x8c609cfcu;r[3]=28u;goto L8c609cfe; /* mov #0x1c,r3 */
L8c609cfe: *stop=0x8c609cfeu;r[0]=4u;goto L8c609d00; /* mov #0x4,r0 */
L8c609d00: *stop=0x8c609d00u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c609d02; /* fmov fr3,@(r0,r13) */
L8c609d02: *stop=0x8c609d02u;if(!valid_access(r[1],2u))return -3;r[2]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;goto L8c609d04; /* mov.w @r1,r2 */
L8c609d04: *stop=0x8c609d04u;if(!valid_access(r[14],4u))return -3;r[5]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c609d06; /* mov.l @r14,r5 */
L8c609d06: *stop=0x8c609d06u;*macl=(r[3]&65535u)*(r[2]&65535u);goto L8c609d08; /* mulu.w r3,r2 */
L8c609d08: *stop=0x8c609d08u;*fpul=r[5];goto L8c609d0a; /* lds r5,FPUL */
L8c609d0a: *stop=0x8c609d0au;*sr=(*sr&~1u)|((int32_t)r[5]>=0);goto L8c609d0c; /* cmp/pz r5 */
L8c609d0c: *stop=0x8c609d0cu;r[3]=*macl;goto L8c609d0e; /* sts MACL,r3 */
L8c609d0e: take=(*sr&1u);*stop=0x8c609d10u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;if(take)goto L8c609d1a;goto L8c609d12; /* bt/s 0x8c609d1a */
L8c609d12: *stop=0x8c609d12u;r[2]=rd(m,bus,&io_status,0x8c609eb8u,4);if(io_status)return io_status;goto L8c609d14; /* mov.l 0x8c609eb8,r2 */
L8c609d14: *stop=0x8c609d14u;*fpul=r[2];goto L8c609d16; /* lds r2,FPUL */
L8c609d16: *stop=0x8c609d16u;f->bank[(f->fpscr>>21)&1u][2]=*fpul;goto L8c609d18; /* fsts FPUL,fr2 */
L8c609d18: *stop=0x8c609d18u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c609d1a; /* fadd fr2,fr3 */
L8c609d1a: *stop=0x8c609d1au;r[0]=rd(m,bus,&io_status,0x8c609ec0u,4);if(io_status)return io_status;goto L8c609d1c; /* mov.l 0x8c609ec0,r0 */
L8c609d1c: *stop=0x8c609d1cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c609d1e; /* fldi1 fr5 */
L8c609d1e: *stop=0x8c609d1eu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[3]);if(io_status)return io_status;goto L8c609d20; /* fmov.s @(r0,r3),fr1 */
L8c609d20: *stop=0x8c609d20u;if(!valid_access(r[4],4u))return -3;r[0]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c609d22; /* mov.l @r4,r0 */
L8c609d22: *stop=0x8c609d22u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c609d24; /* fsub fr3,fr1 */
L8c609d24: *stop=0x8c609d24u;r[0]-=r[5];goto L8c609d26; /* sub r5,r0 */
L8c609d26: *stop=0x8c609d26u;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c609d28; /* cmp/pl r0 */
L8c609d28: *stop=0x8c609d28u;r[0]=*sr&1u;goto L8c609d2a; /* movt r0 */
L8c609d2a: *stop=0x8c609d2au;r[0]<<=2;goto L8c609d2c; /* shll2 r0 */
L8c609d2c: *stop=0x8c609d2cu;fcopy(f,4u,1u);goto L8c609d2e; /* fmov fr1,fr4 */
L8c609d2e: *stop=0x8c609d2eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c609d30; /* fmov.s @(r0,r13),fr3 */
L8c609d30: *stop=0x8c609d30u;r[0]=4u;goto L8c609d32; /* mov #0x4,r0 */
L8c609d32: *stop=0x8c609d32u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c609d34; /* fmov.s @(r0,r14),fr2 */
L8c609d34: *stop=0x8c609d34u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c609d36; /* fdiv fr3,fr4 */
L8c609d36: *stop=0x8c609d36u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c609d38; /* fmov.s @(r0,r4),fr3 */
L8c609d38: *stop=0x8c609d38u;r[0]=8u;goto L8c609d3a; /* mov #0x8,r0 */
L8c609d3a: *stop=0x8c609d3au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[4],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c609d3c; /* fsub fr4,fr5 */
L8c609d3c: *stop=0x8c609d3cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c609d3e; /* fmul fr4,fr3 */
L8c609d3e: *stop=0x8c609d3eu;fcopy(f,0u,5u);goto L8c609d40; /* fmov fr5,fr0 */
L8c609d40: *stop=0x8c609d40u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c609d42; /* fmac fr0,fr2,fr3 */
L8c609d42: *stop=0x8c609d42u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c609d44; /* fmov.s fr3,@r7 */
L8c609d44: *stop=0x8c609d44u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c609d46; /* fmov.s @(r0,r4),fr3 */
L8c609d46: *stop=0x8c609d46u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c609d48; /* fmov.s @(r0,r14),fr2 */
L8c609d48: *stop=0x8c609d48u;r[0]=4u;goto L8c609d4a; /* mov #0x4,r0 */
L8c609d4a: *stop=0x8c609d4au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c609d4c; /* fmul fr4,fr3 */
L8c609d4c: *stop=0x8c609d4cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c609d4e; /* fmac fr0,fr2,fr3 */
L8c609d4e: *stop=0x8c609d4eu;if(!valid_access(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c609d50; /* fmov fr3,@(r0,r7) */
L8c609d50: *stop=0x8c609d50u;r[0]=12u;goto L8c609d52; /* mov #0xc,r0 */
L8c609d52: *stop=0x8c609d52u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c609d54; /* fmov.s @(r0,r4),fr3 */
L8c609d54: *stop=0x8c609d54u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c609d56; /* fmov.s @(r0,r14),fr2 */
L8c609d56: *stop=0x8c609d56u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c609d58; /* fmul fr4,fr3 */
L8c609d58: *stop=0x8c609d58u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c609d5a; /* fmac fr0,fr2,fr3 */
L8c609d5a: *stop=0x8c609d5au;r[0]=8u;goto L8c609d5c; /* mov #0x8,r0 */
L8c609d5c: *stop=0x8c609d5cu;if(!valid_access(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c609d5e; /* fmov fr3,@(r0,r7) */
L8c609d5e: *stop=0x8c609d5eu;r[15]+=8u;goto L8c609d60; /* add #0x8,r15 */
L8c609d60: *stop=0x8c609d60u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c609d62; /* lds.l @r15+,PR */
L8c609d62: *stop=0x8c609d62u;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c609d64; /* mov.l @r15+,r13 */
L8c609d64: *stop=0x8c609d64u;if(*pr!=entry_pr)return -3;*stop=0x8c609d66u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
