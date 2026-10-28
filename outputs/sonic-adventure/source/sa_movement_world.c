/* Original movement world-query owner, nearby-object selection and identity matrix loader.
 * Exact original instruction order and existing numerical/bus ABI.
 * No upstream implementation code imported.
 */
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
#include "sh4_integer_state.h"
#include "sh4_vector_base.h"
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
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
int sa_sh4_sqrt(uint32_t,uint32_t*,uint32_t*);
int sa_movie_channel_io_return_allowed(void *,uint32_t);
static int sa_movement_world_run_8c028bfe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c028c24u)goto L8c028c24;if(resume_pc==0x8c028c3eu)goto L8c028c3e;if(resume_pc==0x8c028d38u)goto L8c028d38;if(resume_pc==0x8c028dfcu)goto L8c028dfc;goto L8c028bfe;
L8c028bfe: *stop=0x8c028bfeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c028c00; /* mov.l r14,@-r15 */
L8c028c00: *stop=0x8c028c00u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c028c02; /* mov.l r13,@-r15 */
L8c028c02: *stop=0x8c028c02u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c028c04; /* mov.l r12,@-r15 */
L8c028c04: *stop=0x8c028c04u;r[12]=r[4];goto L8c028c06; /* mov r4,r12 */
L8c028c06: *stop=0x8c028c06u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c028c08; /* mov.l r11,@-r15 */
L8c028c08: *stop=0x8c028c08u;r[11]=0u;goto L8c028c0a; /* mov #0x0,r11 */
L8c028c0a: *stop=0x8c028c0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c028c0c; /* mov.l r10,@-r15 */
L8c028c0c: *stop=0x8c028c0cu;r[4]=r[12];goto L8c028c0e; /* mov r12,r4 */
L8c028c0e: *stop=0x8c028c0eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c028c10; /* mov.l r9,@-r15 */
L8c028c10: *stop=0x8c028c10u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c028c12; /* mov.l r8,@-r15 */
L8c028c12: *stop=0x8c028c12u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c028c14; /* fmov.s fr15,@-r15 */
L8c028c14: *stop=0x8c028c14u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c028c16; /* fmov.s fr14,@-r15 */
L8c028c16: *stop=0x8c028c16u;{uint32_t loaded=rd(m,bus,&io_status,0x8c028d40u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c028c18; /* mov.l 0x8c028d40,r13 */
L8c028c18: *stop=0x8c028c18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c028c1a; /* sts.l PR,@-r15 */
L8c028c1a: *stop=0x8c028c1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c028d48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c028c1c; /* mov.l 0x8c028d48,r3 */
L8c028c1c: *stop=0x8c028c1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c028d44u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c028c1e; /* mov.l 0x8c028d44,r14 */
L8c028c1e: *stop=0x8c028c1eu;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[11],4);if(io_status)return io_status;goto L8c028c20; /* mov.l r11,@r13 */
L8c028c20: target=r[3];*pr=0x8c028c24u;*stop=0x8c028c22u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c028c24; /* jsr @r3 */
L8c028c24: *stop=0x8c028c24u;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[12]);if(io_status)return io_status;goto L8c028c26; /* fmov.s @r12,fr7 */
L8c028c26: *stop=0x8c028c26u;r[0]=12u;goto L8c028c28; /* mov #0xc,r0 */
L8c028c28: *stop=0x8c028c28u;fcopy(f,14u,0u);goto L8c028c2a; /* fmov fr0,fr14 */
L8c028c2a: *stop=0x8c028c2au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[7],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c028c2c; /* fadd fr7,fr14 */
L8c028c2c: *stop=0x8c028c2cu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[12]);if(io_status)return io_status;goto L8c028c2e; /* fmov.s @(r0,r12),fr6 */
L8c028c2e: *stop=0x8c028c2eu;r[0]=8u;goto L8c028c30; /* mov #0x8,r0 */
L8c028c30: *stop=0x8c028c30u;{uint32_t loaded=rd(m,bus,&io_status,0x8c028d4cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c028c32; /* mov.l 0x8c028d4c,r3 */
L8c028c32: *stop=0x8c028c32u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[12]);if(io_status)return io_status;goto L8c028c34; /* fmov.s @(r0,r12),fr5 */
L8c028c34: *stop=0x8c028c34u;r[0]=4u;goto L8c028c36; /* mov #0x4,r0 */
L8c028c36: *stop=0x8c028c36u;fcopy(f,15u,14u);goto L8c028c38; /* fmov fr14,fr15 */
L8c028c38: *stop=0x8c028c38u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[14],&f->fpscr,&fr[15]);if(status)return status;}goto L8c028c3a; /* fmul fr14,fr15 */
L8c028c3a: target=r[3];*pr=0x8c028c3eu;*stop=0x8c028c3cu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[12]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c028c3e; /* jsr @r3 */
L8c028c3e: *stop=0x8c028c3eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c028d50u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c028c40; /* mov.l 0x8c028d50,r2 */
L8c028c40: *stop=0x8c028c40u;r[0]=0x8c028d5cu;goto L8c028c42; /* mova 0x8c028d5c,r0 */
L8c028c42: *stop=0x8c028c42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c028d58u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c028c44; /* mov.l 0x8c028d58,r9 */
L8c028c44: *stop=0x8c028c44u;{uint32_t loaded=rd(m,bus,&io_status,0x8c028d60u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c028c46; /* mov.l 0x8c028d60,r10 */
L8c028c46: *stop=0x8c028c46u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c028c48; /* mov.l @r2,r5 */
L8c028c48: *stop=0x8c028c48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c028d54u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c028c4a; /* mov.l 0x8c028d54,r8 */
L8c028c4a: *stop=0x8c028c4cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c028e10; /* bra 0x8c028e10 */
L8c028c4e: *stop=0x8c028c4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c028d64u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c028c50; /* mov.l 0x8c028d64,r2 */
L8c028c50: *stop=0x8c028c50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c028d68u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c028c52; /* mov.l 0x8c028d68,r6 */
L8c028c52: *stop=0x8c028c52u;if(!valid_access(r[5]+28u,4u))return -3;wr(m,bus,&io_status,r[5]+0x1cu,r[11],4);if(io_status)return io_status;goto L8c028c54; /* mov.l r11,@(0x1c,r5) */
L8c028c54: *stop=0x8c028c54u;if(!valid_access(r[5]+36u,4u))return -3;wr(m,bus,&io_status,r[5]+0x24u,r[11],4);if(io_status)return io_status;goto L8c028c56; /* mov.l r11,@(0x24,r5) */
L8c028c56: *stop=0x8c028c56u;if(!valid_access(r[2],2u))return -3;r[4]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c028c58; /* mov.w @r2,r4 */
L8c028c58: *stop=0x8c028c58u;*sr=(*sr&~1u)|((int32_t)r[4]>0);goto L8c028c5a; /* cmp/pl r4 */
L8c028c5a: take=!(*sr&1u);*stop=0x8c028c5cu;r[7]=r[11];if(take)goto L8c028c72;goto L8c028c5e; /* bf/s 0x8c028c72 */
L8c028c5e: *stop=0x8c028c5eu;if(!valid_access(r[6]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c028c60; /* mov.l @(0x4,r6),r3 */
L8c028c60: *stop=0x8c028c60u;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c028c62; /* mov.l @(0x8,r5),r2 */
L8c028c62: *stop=0x8c028c62u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c028c64; /* cmp/eq r2,r3 */
L8c028c64: if(!(*sr&1u))goto L8c028c6a;goto L8c028c66; /* bf 0x8c028c6a */
L8c028c66: *stop=0x8c028c68u;r[7]=1u;goto L8c028c72; /* bra 0x8c028c72 */
L8c028c6a: *stop=0x8c028c6au;r[4]+=4294967295u;goto L8c028c6c; /* add #-0x1,r4 */
L8c028c6c: *stop=0x8c028c6cu;*sr=(*sr&~1u)|((int32_t)r[4]>0);goto L8c028c6e; /* cmp/pl r4 */
L8c028c6e: take=(*sr&1u);*stop=0x8c028c70u;r[6]+=12u;if(take)goto L8c028c5e;goto L8c028c72; /* bt/s 0x8c028c5e */
L8c028c72: *stop=0x8c028c72u;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c028c74; /* tst r7,r7 */
L8c028c74: if(!(*sr&1u))goto L8c028c7a;goto L8c028c76; /* bf 0x8c028c7a */
L8c028c76: *stop=0x8c028c78u;goto L8c028e0e; /* bra 0x8c028e0e */
L8c028c7a: *stop=0x8c028c7au;r[0]=12u;goto L8c028c7c; /* mov #0xc,r0 */
L8c028c7c: *stop=0x8c028c7cu;if(!valid_access(r[5]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c028c7e; /* mov.l @(0x10,r5),r4 */
L8c028c7e: *stop=0x8c028c7eu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[12]);if(io_status)return io_status;goto L8c028c80; /* fmov.s @(r0,r12),fr5 */
L8c028c80: *stop=0x8c028c80u;r[0]=40u;goto L8c028c82; /* mov #0x28,r0 */
L8c028c82: *stop=0x8c028c82u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]+r[5]);if(io_status)return io_status;goto L8c028c84; /* fmov.s @(r0,r5),fr7 */
L8c028c84: *stop=0x8c028c84u;r[0]=16u;goto L8c028c86; /* mov #0x10,r0 */
L8c028c86: *stop=0x8c028c86u;fcopy(f,3u,5u);goto L8c028c88; /* fmov fr5,fr3 */
L8c028c88: *stop=0x8c028c88u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c028c8a; /* fsub fr7,fr3 */
L8c028c8a: *stop=0x8c028c8au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c028c8c; /* fmov.s @(r0,r4),fr2 */
L8c028c8c: *stop=0x8c028c8cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[14],&f->fpscr,&fr[7],0);if(status)return status;}goto L8c028c8e; /* fadd fr14,fr7 */
L8c028c8e: *stop=0x8c028c8eu;if(!valid_access(r[5]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c028c90; /* mov.l @(0x14,r5),r6 */
L8c028c90: *stop=0x8c028c90u;fcopy(f,4u,3u);goto L8c028c92; /* fmov fr3,fr4 */
L8c028c92: *stop=0x8c028c92u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[14],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c028c94; /* fsub fr14,fr4 */
L8c028c94: *stop=0x8c028c94u;fcopy(f,8u,7u);goto L8c028c96; /* fmov fr7,fr8 */
L8c028c96: *stop=0x8c028c96u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c028c98; /* fcmp/gt fr2,fr4 */
L8c028c98: take=!(*sr&1u);*stop=0x8c028c9au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[5],&f->fpscr,&fr[8],0);if(status)return status;}if(take)goto L8c028cb8;goto L8c028c9c; /* bf/s 0x8c028cb8 */
L8c028c9c: *stop=0x8c028c9cu;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c028c9e; /* fmov.s @(r0,r6),fr3 */
L8c028c9e: *stop=0x8c028c9eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][8],&f->fpscr,sr,1);if(status)return status;goto L8c028ca0; /* fcmp/gt fr8,fr3 */
L8c028ca0: if(!(*sr&1u))goto L8c028cb8;goto L8c028ca2; /* bf 0x8c028cb8 */
L8c028ca2: *stop=0x8c028ca2u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c028ca4; /* fmov.s @(r0,r4),fr3 */
L8c028ca4: *stop=0x8c028ca4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c028ca6; /* fcmp/gt fr4,fr3 */
L8c028ca6: if((*sr&1u))goto L8c028cb8;goto L8c028ca8; /* bt 0x8c028cb8 */
L8c028ca8: *stop=0x8c028ca8u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c028caa; /* mov.l @r4,r4 */
L8c028caa: *stop=0x8c028caau;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c028cac; /* tst r4,r4 */
L8c028cac: if(!(*sr&1u))goto L8c028cb2;goto L8c028cae; /* bf 0x8c028cb2 */
L8c028cae: *stop=0x8c028cb0u;goto L8c028d92; /* bra 0x8c028d92 */
L8c028cb2: *stop=0x8c028cb2u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c028cb4; /* fmov.s @(r0,r4),fr3 */
L8c028cb4: *stop=0x8c028cb4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c028cb6; /* fcmp/gt fr4,fr3 */
L8c028cb6: if(!(*sr&1u))goto L8c028ca8;goto L8c028cb8; /* bf 0x8c028ca8 */
L8c028cb8: *stop=0x8c028cb8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c028cba; /* tst r4,r4 */
L8c028cba: if(!(*sr&1u))goto L8c028cc0;goto L8c028cbc; /* bf 0x8c028cc0 */
L8c028cbc: *stop=0x8c028cbeu;goto L8c028d92; /* bra 0x8c028d92 */
L8c028cc0: *stop=0x8c028cc0u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c028cc2; /* fmov.s @(r0,r4),fr3 */
L8c028cc2: *stop=0x8c028cc2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c028cc4; /* fcmp/gt fr4,fr3 */
L8c028cc4: if((*sr&1u))goto L8c028cca;goto L8c028cc6; /* bt 0x8c028cca */
L8c028cc6: *stop=0x8c028cc8u;goto L8c028d92; /* bra 0x8c028d92 */
L8c028cca: *stop=0x8c028cccu;goto L8c028d82; /* bra 0x8c028d82 */
L8c028cce: *stop=0x8c028cceu;r[0]=20u;goto L8c028cd0; /* mov #0x14,r0 */
L8c028cd0: *stop=0x8c028cd0u;r[6]=r[12];goto L8c028cd2; /* mov r12,r6 */
L8c028cd2: *stop=0x8c028cd2u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c028cd4; /* fmov.s @(r0,r4),fr3 */
L8c028cd4: *stop=0x8c028cd4u;r[7]=r[4];goto L8c028cd6; /* mov r4,r7 */
L8c028cd6: *stop=0x8c028cd6u;fcopy(f,2u,15u);goto L8c028cd8; /* fmov fr15,fr2 */
L8c028cd8: *stop=0x8c028cd8u;r[7]+=8u;goto L8c028cda; /* add #0x8,r7 */
L8c028cda: *stop=0x8c028cdau;r[6]+=4u;goto L8c028cdc; /* add #0x4,r6 */
L8c028cdc: *stop=0x8c028cdcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c028cde; /* fadd fr3,fr2 */
L8c028cde: *stop=0x8c028cdeu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c028ce0; /* fmov.s @r6+,fr3 */
L8c028ce0: *stop=0x8c028ce0u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[7]);if(io_status)return io_status;r[7]+=(f->fpscr&0x100000u)?8u:4u;goto L8c028ce2; /* fmov.s @r7+,fr5 */
L8c028ce2: *stop=0x8c028ce2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c028ce4; /* fsub fr3,fr5 */
L8c028ce4: *stop=0x8c028ce4u;fcopy(f,4u,2u);goto L8c028ce6; /* fmov fr2,fr4 */
L8c028ce6: *stop=0x8c028ce6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[6],&f->fpscr,&fr[4]);if(status)return status;}goto L8c028ce8; /* fmul fr6,fr4 */
L8c028ce8: *stop=0x8c028ce8u;fcopy(f,7u,5u);goto L8c028cea; /* fmov fr5,fr7 */
L8c028cea: *stop=0x8c028ceau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[5],&f->fpscr,&fr[7]);if(status)return status;}goto L8c028cec; /* fmul fr5,fr7 */
L8c028cec: *stop=0x8c028cecu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c028cee; /* fcmp/gt fr4,fr7 */
L8c028cee: if((*sr&1u))goto L8c028d80;goto L8c028cf0; /* bt 0x8c028d80 */
L8c028cf0: *stop=0x8c028cf0u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[7]);if(io_status)return io_status;r[7]+=(f->fpscr&0x100000u)?8u:4u;goto L8c028cf2; /* fmov.s @r7+,fr5 */
L8c028cf2: *stop=0x8c028cf2u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c028cf4; /* fmov.s @r6+,fr3 */
L8c028cf4: *stop=0x8c028cf4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c028cf6; /* fsub fr3,fr5 */
L8c028cf6: *stop=0x8c028cf6u;fcopy(f,0u,5u);goto L8c028cf8; /* fmov fr5,fr0 */
L8c028cf8: *stop=0x8c028cf8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[5],&fr[7],&f->fpscr,&fr[7]);if(status)return status;}goto L8c028cfa; /* fmac fr0,fr5,fr7 */
L8c028cfa: *stop=0x8c028cfau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c028cfc; /* fcmp/gt fr4,fr7 */
L8c028cfc: if((*sr&1u))goto L8c028d80;goto L8c028cfe; /* bt 0x8c028d80 */
L8c028cfe: *stop=0x8c028cfeu;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[7]);if(io_status)return io_status;goto L8c028d00; /* fmov.s @r7,fr5 */
L8c028d00: *stop=0x8c028d00u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c028d02; /* fmov.s @r6,fr3 */
L8c028d02: *stop=0x8c028d02u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c028d04; /* fsub fr3,fr5 */
L8c028d04: *stop=0x8c028d04u;fcopy(f,0u,5u);goto L8c028d06; /* fmov fr5,fr0 */
L8c028d06: *stop=0x8c028d06u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[5],&fr[7],&f->fpscr,&fr[7]);if(status)return status;}goto L8c028d08; /* fmac fr0,fr5,fr7 */
L8c028d08: *stop=0x8c028d08u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c028d0a; /* fcmp/gt fr4,fr7 */
L8c028d0a: if((*sr&1u))goto L8c028d80;goto L8c028d0c; /* bt 0x8c028d80 */
L8c028d0c: *stop=0x8c028d0cu;if(!valid_access(r[5]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c028d0e; /* mov.l @(0x1c,r5),r3 */
L8c028d0e: *stop=0x8c028d0eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c028d10; /* tst r3,r3 */
L8c028d10: if(!(*sr&1u))goto L8c028d14;goto L8c028d12; /* bf 0x8c028d14 */
L8c028d12: *stop=0x8c028d12u;if(!valid_access(r[5]+28u,4u))return -3;wr(m,bus,&io_status,r[5]+0x1cu,r[14],4);if(io_status)return io_status;goto L8c028d14; /* mov.l r14,@(0x1c,r5) */
L8c028d14: *stop=0x8c028d14u;if(!valid_access(r[5]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c028d16; /* mov.l @(0x24,r5),r3 */
L8c028d16: *stop=0x8c028d16u;r[3]+=1u;goto L8c028d18; /* add #0x1,r3 */
L8c028d18: *stop=0x8c028d18u;if(!valid_access(r[5]+36u,4u))return -3;wr(m,bus,&io_status,r[5]+0x24u,r[3],4);if(io_status)return io_status;goto L8c028d1a; /* mov.l r3,@(0x24,r5) */
L8c028d1a: *stop=0x8c028d1au;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c028d1c; /* mov.l r4,@r14 */
L8c028d1c: *stop=0x8c028d1cu;r[14]+=4u;goto L8c028d1e; /* add #0x4,r14 */
L8c028d1e: *stop=0x8c028d1eu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[0]=loaded;}goto L8c028d20; /* mov.l @r13,r0 */
L8c028d20: *stop=0x8c028d20u;r[0]+=1u;goto L8c028d22; /* add #0x1,r0 */
L8c028d22: *stop=0x8c028d22u;*sr=(*sr&~1u)|(r[0]==0x00000060u);goto L8c028d24; /* cmp/eq #0x60,r0 */
L8c028d24: take=!(*sr&1u);*stop=0x8c028d26u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[0],4);if(io_status)return io_status;if(take)goto L8c028d80;goto L8c028d28; /* bf/s 0x8c028d80 */
L8c028d28: *stop=0x8c028d28u;if(!valid_access(r[9],1u))return -3;r[2]=sx(m,bus,&io_status,r[9],1);if(io_status)return io_status;goto L8c028d2a; /* mov.b @r9,r2 */
L8c028d2a: *stop=0x8c028d2au;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c028d2c; /* cmp/pz r2 */
L8c028d2c: if((*sr&1u))goto L8c028d32;goto L8c028d2e; /* bt 0x8c028d32 */
L8c028d2e: *stop=0x8c028d30u;goto L8c028e18; /* bra 0x8c028e18 */
L8c028d32: *stop=0x8c028d32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c028d6cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c028d34; /* mov.l 0x8c028d6c,r5 */
L8c028d34: target=r[8];*pr=0x8c028d38u;*stop=0x8c028d36u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c028d38; /* jsr @r8 */
L8c028d38: *stop=0x8c028d3au;goto L8c028e18; /* bra 0x8c028e18 */
L8c028d80: *stop=0x8c028d80u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c028d82; /* mov.l @r4,r4 */
L8c028d82: *stop=0x8c028d82u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c028d84; /* tst r4,r4 */
L8c028d84: if((*sr&1u))goto L8c028e0e;goto L8c028d86; /* bt 0x8c028e0e */
L8c028d86: *stop=0x8c028d86u;r[0]=16u;goto L8c028d88; /* mov #0x10,r0 */
L8c028d88: *stop=0x8c028d88u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c028d8a; /* fmov.s @(r0,r4),fr3 */
L8c028d8a: *stop=0x8c028d8au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][8],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c028d8c; /* fcmp/gt fr3,fr8 */
L8c028d8c: if((*sr&1u))goto L8c028cce;goto L8c028d8e; /* bt 0x8c028cce */
L8c028d8e: *stop=0x8c028d90u;goto L8c028e0e; /* bra 0x8c028e0e */
L8c028d92: *stop=0x8c028d94u;r[4]=r[6];goto L8c028e02; /* bra 0x8c028e02 */
L8c028d96: *stop=0x8c028d96u;r[0]=20u;goto L8c028d98; /* mov #0x14,r0 */
L8c028d98: *stop=0x8c028d98u;r[6]=r[12];goto L8c028d9a; /* mov r12,r6 */
L8c028d9a: *stop=0x8c028d9au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c028d9c; /* fmov.s @(r0,r4),fr3 */
L8c028d9c: *stop=0x8c028d9cu;r[7]=r[4];goto L8c028d9e; /* mov r4,r7 */
L8c028d9e: *stop=0x8c028d9eu;fcopy(f,2u,15u);goto L8c028da0; /* fmov fr15,fr2 */
L8c028da0: *stop=0x8c028da0u;r[7]+=8u;goto L8c028da2; /* add #0x8,r7 */
L8c028da2: *stop=0x8c028da2u;r[6]+=4u;goto L8c028da4; /* add #0x4,r6 */
L8c028da4: *stop=0x8c028da4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c028da6; /* fadd fr3,fr2 */
L8c028da6: *stop=0x8c028da6u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c028da8; /* fmov.s @r6+,fr3 */
L8c028da8: *stop=0x8c028da8u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[7]);if(io_status)return io_status;r[7]+=(f->fpscr&0x100000u)?8u:4u;goto L8c028daa; /* fmov.s @r7+,fr8 */
L8c028daa: *stop=0x8c028daau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c028dac; /* fsub fr3,fr8 */
L8c028dac: *stop=0x8c028dacu;fcopy(f,5u,2u);goto L8c028dae; /* fmov fr2,fr5 */
L8c028dae: *stop=0x8c028daeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[6],&f->fpscr,&fr[5]);if(status)return status;}goto L8c028db0; /* fmul fr6,fr5 */
L8c028db0: *stop=0x8c028db0u;fcopy(f,7u,8u);goto L8c028db2; /* fmov fr8,fr7 */
L8c028db2: *stop=0x8c028db2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[8],&f->fpscr,&fr[7]);if(status)return status;}goto L8c028db4; /* fmul fr8,fr7 */
L8c028db4: *stop=0x8c028db4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c028db6; /* fcmp/gt fr5,fr7 */
L8c028db6: if((*sr&1u))goto L8c028e00;goto L8c028db8; /* bt 0x8c028e00 */
L8c028db8: *stop=0x8c028db8u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[7]);if(io_status)return io_status;r[7]+=(f->fpscr&0x100000u)?8u:4u;goto L8c028dba; /* fmov.s @r7+,fr8 */
L8c028dba: *stop=0x8c028dbau;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c028dbc; /* fmov.s @r6+,fr3 */
L8c028dbc: *stop=0x8c028dbcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c028dbe; /* fsub fr3,fr8 */
L8c028dbe: *stop=0x8c028dbeu;fcopy(f,0u,8u);goto L8c028dc0; /* fmov fr8,fr0 */
L8c028dc0: *stop=0x8c028dc0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[8],&fr[7],&f->fpscr,&fr[7]);if(status)return status;}goto L8c028dc2; /* fmac fr0,fr8,fr7 */
L8c028dc2: *stop=0x8c028dc2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c028dc4; /* fcmp/gt fr5,fr7 */
L8c028dc4: if((*sr&1u))goto L8c028e00;goto L8c028dc6; /* bt 0x8c028e00 */
L8c028dc6: *stop=0x8c028dc6u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[7]);if(io_status)return io_status;goto L8c028dc8; /* fmov.s @r7,fr8 */
L8c028dc8: *stop=0x8c028dc8u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c028dca; /* fmov.s @r6,fr3 */
L8c028dca: *stop=0x8c028dcau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c028dcc; /* fsub fr3,fr8 */
L8c028dcc: *stop=0x8c028dccu;fcopy(f,0u,8u);goto L8c028dce; /* fmov fr8,fr0 */
L8c028dce: *stop=0x8c028dceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[8],&fr[7],&f->fpscr,&fr[7]);if(status)return status;}goto L8c028dd0; /* fmac fr0,fr8,fr7 */
L8c028dd0: *stop=0x8c028dd0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c028dd2; /* fcmp/gt fr5,fr7 */
L8c028dd2: if((*sr&1u))goto L8c028e00;goto L8c028dd4; /* bt 0x8c028e00 */
L8c028dd4: *stop=0x8c028dd4u;if(!valid_access(r[5]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c028dd6; /* mov.l @(0x1c,r5),r3 */
L8c028dd6: *stop=0x8c028dd6u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c028dd8; /* tst r3,r3 */
L8c028dd8: if(!(*sr&1u))goto L8c028ddc;goto L8c028dda; /* bf 0x8c028ddc */
L8c028dda: *stop=0x8c028ddau;if(!valid_access(r[5]+28u,4u))return -3;wr(m,bus,&io_status,r[5]+0x1cu,r[14],4);if(io_status)return io_status;goto L8c028ddc; /* mov.l r14,@(0x1c,r5) */
L8c028ddc: *stop=0x8c028ddcu;if(!valid_access(r[5]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c028dde; /* mov.l @(0x24,r5),r3 */
L8c028dde: *stop=0x8c028ddeu;r[3]+=1u;goto L8c028de0; /* add #0x1,r3 */
L8c028de0: *stop=0x8c028de0u;if(!valid_access(r[5]+36u,4u))return -3;wr(m,bus,&io_status,r[5]+0x24u,r[3],4);if(io_status)return io_status;goto L8c028de2; /* mov.l r3,@(0x24,r5) */
L8c028de2: *stop=0x8c028de2u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c028de4; /* mov.l r4,@r14 */
L8c028de4: *stop=0x8c028de4u;r[14]+=4u;goto L8c028de6; /* add #0x4,r14 */
L8c028de6: *stop=0x8c028de6u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[0]=loaded;}goto L8c028de8; /* mov.l @r13,r0 */
L8c028de8: *stop=0x8c028de8u;r[0]+=1u;goto L8c028dea; /* add #0x1,r0 */
L8c028dea: *stop=0x8c028deau;*sr=(*sr&~1u)|(r[0]==0x00000060u);goto L8c028dec; /* cmp/eq #0x60,r0 */
L8c028dec: take=!(*sr&1u);*stop=0x8c028deeu;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[0],4);if(io_status)return io_status;if(take)goto L8c028e00;goto L8c028df0; /* bf/s 0x8c028e00 */
L8c028df0: *stop=0x8c028df0u;if(!valid_access(r[9],1u))return -3;r[2]=sx(m,bus,&io_status,r[9],1);if(io_status)return io_status;goto L8c028df2; /* mov.b @r9,r2 */
L8c028df2: *stop=0x8c028df2u;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c028df4; /* cmp/pz r2 */
L8c028df4: if(!(*sr&1u))goto L8c028e18;goto L8c028df6; /* bf 0x8c028e18 */
L8c028df6: *stop=0x8c028df6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c028e40u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c028df8; /* mov.l 0x8c028e40,r5 */
L8c028df8: target=r[8];*pr=0x8c028dfcu;*stop=0x8c028dfau;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c028dfc; /* jsr @r8 */
L8c028dfc: *stop=0x8c028dfeu;goto L8c028e18; /* bra 0x8c028e18 */
L8c028e00: *stop=0x8c028e00u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c028e02; /* mov.l @(0x4,r4),r4 */
L8c028e02: *stop=0x8c028e02u;r[0]=16u;goto L8c028e04; /* mov #0x10,r0 */
L8c028e04: *stop=0x8c028e04u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c028e06; /* fmov.s @(r0,r4),fr3 */
L8c028e06: *stop=0x8c028e06u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c028e08; /* fcmp/gt fr4,fr3 */
L8c028e08: if(!(*sr&1u))goto L8c028e0e;goto L8c028e0a; /* bf 0x8c028e0e */
L8c028e0a: *stop=0x8c028e0au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c028e0c; /* tst r4,r4 */
L8c028e0c: if(!(*sr&1u))goto L8c028d96;goto L8c028e0e; /* bf 0x8c028d96 */
L8c028e0e: *stop=0x8c028e0eu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[5]=loaded;}goto L8c028e10; /* mov.l @r5,r5 */
L8c028e10: *stop=0x8c028e10u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c028e12; /* tst r5,r5 */
L8c028e12: if((*sr&1u))goto L8c028e18;goto L8c028e14; /* bt 0x8c028e18 */
L8c028e14: *stop=0x8c028e16u;goto L8c028c4e; /* bra 0x8c028c4e */
L8c028e18: *stop=0x8c028e18u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c028e1a; /* lds.l @r15+,PR */
L8c028e1a: *stop=0x8c028e1au;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[0]=loaded;}goto L8c028e1c; /* mov.l @r13,r0 */
L8c028e1c: *stop=0x8c028e1cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c028e1e; /* fmov.s @r15+,fr14 */
L8c028e1e: *stop=0x8c028e1eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c028e20; /* fmov.s @r15+,fr15 */
L8c028e20: *stop=0x8c028e20u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c028e22; /* mov.l @r15+,r8 */
L8c028e22: *stop=0x8c028e22u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c028e24; /* mov.l @r15+,r9 */
L8c028e24: *stop=0x8c028e24u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c028e26; /* mov.l @r15+,r10 */
L8c028e26: *stop=0x8c028e26u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c028e28; /* mov.l @r15+,r11 */
L8c028e28: *stop=0x8c028e28u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c028e2a; /* mov.l @r15+,r12 */
L8c028e2a: *stop=0x8c028e2au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c028e2c; /* mov.l @r15+,r13 */
L8c028e2c: *stop=0x8c028e2cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c028e2eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_world_8c028bfe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c028bfe(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_world_resume_8c028c24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c028bfe(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c028c24u);}
SA_STAGE_EXPORT int sa_movement_world_resume_8c028c3e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c028bfe(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c028c3eu);}
SA_STAGE_EXPORT int sa_movement_world_resume_8c028d38(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c028bfe(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c028d38u);}
SA_STAGE_EXPORT int sa_movement_world_resume_8c028dfc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c028bfe(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c028dfcu);}

static int sa_movement_world_run_8c0522c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c0522f4u)goto L8c0522f4;if(resume_pc==0x8c05235au)goto L8c05235a;if(resume_pc==0x8c052368u)goto L8c052368;if(resume_pc==0x8c052376u)goto L8c052376;if(resume_pc==0x8c052384u)goto L8c052384;if(resume_pc==0x8c052392u)goto L8c052392;if(resume_pc==0x8c052408u)goto L8c052408;if(resume_pc==0x8c05241au)goto L8c05241a;if(resume_pc==0x8c0524cau)goto L8c0524ca;goto L8c0522c0;
L8c0522c0: *stop=0x8c0522c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0522c2; /* mov.l r14,@-r15 */
L8c0522c2: *stop=0x8c0522c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0522c4; /* mov.l r13,@-r15 */
L8c0522c4: *stop=0x8c0522c4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c0522c6; /* mov.l r12,@-r15 */
L8c0522c6: *stop=0x8c0522c6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c0522c8; /* mov.l r11,@-r15 */
L8c0522c8: *stop=0x8c0522c8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c0522ca; /* mov.l r10,@-r15 */
L8c0522ca: *stop=0x8c0522cau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c0522cc; /* mov.l r9,@-r15 */
L8c0522cc: *stop=0x8c0522ccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c0522ce; /* mov.l r8,@-r15 */
L8c0522ce: *stop=0x8c0522ceu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c0522d0; /* fmov.s fr15,@-r15 */
L8c0522d0: *stop=0x8c0522d0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c0522d2; /* fmov.s fr14,@-r15 */
L8c0522d2: *stop=0x8c0522d2u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c0522d4; /* fmov.s fr13,@-r15 */
L8c0522d4: *stop=0x8c0522d4u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c0522d6; /* fmov.s fr12,@-r15 */
L8c0522d6: *stop=0x8c0522d6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0522d8; /* sts.l PR,@-r15 */
L8c0522d8: *stop=0x8c0522d8u;r[15]+=4294967272u;goto L8c0522da; /* add #-0x18,r15 */
L8c0522da: *stop=0x8c0522dau;r[0]=4u;goto L8c0522dc; /* mov #0x4,r0 */
L8c0522dc: *stop=0x8c0522dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c052444u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c0522de; /* mov.l 0x8c052444,r13 */
L8c0522de: *stop=0x8c0522deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c052440u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c0522e0; /* mov.l 0x8c052440,r12 */
L8c0522e0: *stop=0x8c0522e0u;r[9]=0u;goto L8c0522e2; /* mov #0x0,r9 */
L8c0522e2: *stop=0x8c0522e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c052448u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0522e4; /* mov.l 0x8c052448,r3 */
L8c0522e4: *stop=0x8c0522e4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c0522e6; /* fmov.s fr4,@r15 */
L8c0522e6: *stop=0x8c0522e6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,6u,r[0]+r[15]);if(io_status)return io_status;goto L8c0522e8; /* fmov fr6,@(r0,r15) */
L8c0522e8: *stop=0x8c0522e8u;r[0]=8u;goto L8c0522ea; /* mov #0x8,r0 */
L8c0522ea: *stop=0x8c0522eau;fcopy(f,12u,5u);goto L8c0522ec; /* fmov fr5,fr12 */
L8c0522ec: *stop=0x8c0522ecu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,7u,r[0]+r[15]);if(io_status)return io_status;goto L8c0522ee; /* fmov fr7,@(r0,r15) */
L8c0522ee: *stop=0x8c0522eeu;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[9],2);if(io_status)return io_status;goto L8c0522f0; /* mov.w r9,@r13 */
L8c0522f0: target=r[3];*pr=0x8c0522f4u;*stop=0x8c0522f2u;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0522f4; /* jsr @r3 */
L8c0522f4: *stop=0x8c0522f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c05244cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0522f6; /* mov.l 0x8c05244c,r3 */
L8c0522f6: *stop=0x8c0522f6u;r[10]=4u;goto L8c0522f8; /* mov #0x4,r10 */
L8c0522f8: *stop=0x8c0522f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c052450u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0522fa; /* mov.l 0x8c052450,r4 */
L8c0522fa: *stop=0x8c0522fau;r[10]<<=8;goto L8c0522fc; /* shll8 r10 */
L8c0522fc: *stop=0x8c0522fcu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[11]=loaded;}goto L8c0522fe; /* mov.l @r3,r11 */
L8c0522fe: *stop=0x8c0522feu;r[0]=0x8c052454u;goto L8c052300; /* mova 0x8c052454,r0 */
L8c052300: *stop=0x8c052300u;r[11]+=4294967295u;goto L8c052302; /* add #-0x1,r11 */
L8c052302: *stop=0x8c052302u;r[2]=r[11];goto L8c052304; /* mov r11,r2 */
L8c052304: *stop=0x8c052304u;*sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c052306; /* shll r11 */
L8c052306: *stop=0x8c052306u;r[11]+=r[2];goto L8c052308; /* add r2,r11 */
L8c052308: *stop=0x8c052308u;r[11]<<=2;goto L8c05230a; /* shll2 r11 */
L8c05230a: *stop=0x8c05230au;r[11]+=r[4];goto L8c05230c; /* add r4,r11 */
L8c05230c: *stop=0x8c05230cu;*sr=(*sr&~1u)|(r[11]>=r[4]);goto L8c05230e; /* cmp/hs r4,r11 */
L8c05230e: take=(*sr&1u);*stop=0x8c052310u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;if(take)goto L8c052316;goto L8c052312; /* bt/s 0x8c052316 */
L8c052312: *stop=0x8c052314u;goto L8c052414; /* bra 0x8c052414 */
L8c052316: *stop=0x8c052316u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c052318; /* mov.l @r11,r2 */
L8c052318: *stop=0x8c052318u;{uint32_t loaded=rd(m,bus,&io_status,0x8c052458u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c05231a; /* mov.l 0x8c052458,r3 */
L8c05231a: *stop=0x8c05231au;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c05231c; /* tst r3,r2 */
L8c05231c: take=!(*sr&1u);*stop=0x8c05231eu;if(!valid_access(r[11]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;r[14]=loaded;}if(take)goto L8c052354;goto L8c052320; /* bf/s 0x8c052354 */
L8c052320: *stop=0x8c052320u;r[4]=r[14];goto L8c052322; /* mov r14,r4 */
L8c052322: *stop=0x8c052322u;r[4]+=8u;goto L8c052324; /* add #0x8,r4 */
L8c052324: *stop=0x8c052324u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c052326; /* fmov.s @r15,fr2 */
L8c052326: *stop=0x8c052326u;r[0]=24u;goto L8c052328; /* mov #0x18,r0 */
L8c052328: *stop=0x8c052328u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c05232a; /* fmov.s @r4,fr3 */
L8c05232a: *stop=0x8c05232au;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c05232c; /* mov.l @(0x4,r14),r5 */
L8c05232c: *stop=0x8c05232cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c05232e; /* fsub fr3,fr2 */
L8c05232e: *stop=0x8c05232eu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c052330; /* fmov.s @(r0,r5),fr3 */
L8c052330: *stop=0x8c052330u;r[0]=4u;goto L8c052332; /* mov #0x4,r0 */
L8c052332: *stop=0x8c052332u;fcopy(f,6u,2u);goto L8c052334; /* fmov fr2,fr6 */
L8c052334: *stop=0x8c052334u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[3],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c052336; /* fsub fr3,fr6 */
L8c052336: *stop=0x8c052336u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c052338; /* fmov.s @(r0,r4),fr3 */
L8c052338: *stop=0x8c052338u;r[0]=28u;goto L8c05233a; /* mov #0x1c,r0 */
L8c05233a: *stop=0x8c05233au;fcopy(f,2u,12u);goto L8c05233c; /* fmov fr12,fr2 */
L8c05233c: *stop=0x8c05233cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c05233e; /* fsub fr3,fr2 */
L8c05233e: *stop=0x8c05233eu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c052340; /* fmov.s @(r0,r5),fr3 */
L8c052340: *stop=0x8c052340u;r[0]=8u;goto L8c052342; /* mov #0x8,r0 */
L8c052342: *stop=0x8c052342u;fcopy(f,5u,2u);goto L8c052344; /* fmov fr2,fr5 */
L8c052344: *stop=0x8c052344u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c052346; /* fsub fr3,fr5 */
L8c052346: *stop=0x8c052346u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c052348; /* fmov.s @(r0,r4),fr3 */
L8c052348: *stop=0x8c052348u;r[0]=4u;goto L8c05234a; /* mov #0x4,r0 */
L8c05234a: *stop=0x8c05234au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c05234c; /* fmov.s @(r0,r15),fr2 */
L8c05234c: *stop=0x8c05234cu;r[0]=32u;goto L8c05234e; /* mov #0x20,r0 */
L8c05234e: *stop=0x8c05234eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c052350; /* fsub fr3,fr2 */
L8c052350: *stop=0x8c052352u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c0523c2; /* bra 0x8c0523c2 */
L8c052354: *stop=0x8c052354u;{uint32_t loaded=rd(m,bus,&io_status,0x8c05245cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c052356; /* mov.l 0x8c05245c,r1 */
L8c052356: target=r[1];*pr=0x8c05235au;*stop=0x8c052358u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05235a; /* jsr @r1 */
L8c05235a: *stop=0x8c05235au;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[8]=loaded;}goto L8c05235c; /* mov.l @(0x1c,r14),r8 */
L8c05235c: *stop=0x8c05235cu;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c05235e; /* tst r8,r8 */
L8c05235e: if((*sr&1u))goto L8c052368;goto L8c052360; /* bt 0x8c052368 */
L8c052360: *stop=0x8c052360u;{uint32_t loaded=rd(m,bus,&io_status,0x8c052460u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c052362; /* mov.l 0x8c052460,r2 */
L8c052362: *stop=0x8c052362u;r[5]=r[8];goto L8c052364; /* mov r8,r5 */
L8c052364: target=r[2];*pr=0x8c052368u;*stop=0x8c052366u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052368; /* jsr @r2 */
L8c052368: *stop=0x8c052368u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c05236a; /* mov.l @(0x14,r14),r8 */
L8c05236a: *stop=0x8c05236au;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c05236c; /* tst r8,r8 */
L8c05236c: if((*sr&1u))goto L8c052376;goto L8c05236e; /* bt 0x8c052376 */
L8c05236e: *stop=0x8c05236eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c052464u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c052370; /* mov.l 0x8c052464,r3 */
L8c052370: *stop=0x8c052370u;r[5]=r[8];goto L8c052372; /* mov r8,r5 */
L8c052372: target=r[3];*pr=0x8c052376u;*stop=0x8c052374u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052376; /* jsr @r3 */
L8c052376: *stop=0x8c052376u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c052378; /* mov.l @(0x18,r14),r8 */
L8c052378: *stop=0x8c052378u;*sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c05237a; /* tst r8,r8 */
L8c05237a: if((*sr&1u))goto L8c052384;goto L8c05237c; /* bt 0x8c052384 */
L8c05237c: *stop=0x8c05237cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c052468u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c05237e; /* mov.l 0x8c052468,r2 */
L8c05237e: *stop=0x8c05237eu;r[5]=r[8];goto L8c052380; /* mov r8,r5 */
L8c052380: target=r[2];*pr=0x8c052384u;*stop=0x8c052382u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052384; /* jsr @r2 */
L8c052384: *stop=0x8c052384u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c052386; /* mov.l @(0x4,r14),r5 */
L8c052386: *stop=0x8c052386u;r[6]=r[15];goto L8c052388; /* mov r15,r6 */
L8c052388: *stop=0x8c052388u;{uint32_t loaded=rd(m,bus,&io_status,0x8c05246cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c05238a; /* mov.l 0x8c05246c,r3 */
L8c05238a: *stop=0x8c05238au;r[6]+=12u;goto L8c05238c; /* add #0xc,r6 */
L8c05238c: *stop=0x8c05238cu;r[5]+=24u;goto L8c05238e; /* add #0x18,r5 */
L8c05238e: target=r[3];*pr=0x8c052392u;*stop=0x8c052390u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052392; /* jsr @r3 */
L8c052392: *stop=0x8c052392u;r[4]=r[14];goto L8c052394; /* mov r14,r4 */
L8c052394: *stop=0x8c052394u;r[4]+=8u;goto L8c052396; /* add #0x8,r4 */
L8c052396: *stop=0x8c052396u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c052398; /* fmov.s @r15,fr2 */
L8c052398: *stop=0x8c052398u;r[0]=12u;goto L8c05239a; /* mov #0xc,r0 */
L8c05239a: *stop=0x8c05239au;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c05239c; /* fmov.s @r4,fr3 */
L8c05239c: *stop=0x8c05239cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c05239e; /* fsub fr3,fr2 */
L8c05239e: *stop=0x8c05239eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0523a0; /* fmov.s @(r0,r15),fr3 */
L8c0523a0: *stop=0x8c0523a0u;r[0]=4u;goto L8c0523a2; /* mov #0x4,r0 */
L8c0523a2: *stop=0x8c0523a2u;fcopy(f,6u,2u);goto L8c0523a4; /* fmov fr2,fr6 */
L8c0523a4: *stop=0x8c0523a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[3],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c0523a6; /* fsub fr3,fr6 */
L8c0523a6: *stop=0x8c0523a6u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0523a8; /* fmov.s @(r0,r4),fr3 */
L8c0523a8: *stop=0x8c0523a8u;r[0]=16u;goto L8c0523aa; /* mov #0x10,r0 */
L8c0523aa: *stop=0x8c0523aau;fcopy(f,2u,12u);goto L8c0523ac; /* fmov fr12,fr2 */
L8c0523ac: *stop=0x8c0523acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0523ae; /* fsub fr3,fr2 */
L8c0523ae: *stop=0x8c0523aeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0523b0; /* fmov.s @(r0,r15),fr3 */
L8c0523b0: *stop=0x8c0523b0u;r[0]=8u;goto L8c0523b2; /* mov #0x8,r0 */
L8c0523b2: *stop=0x8c0523b2u;fcopy(f,5u,2u);goto L8c0523b4; /* fmov fr2,fr5 */
L8c0523b4: *stop=0x8c0523b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c0523b6; /* fsub fr3,fr5 */
L8c0523b6: *stop=0x8c0523b6u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0523b8; /* fmov.s @(r0,r4),fr3 */
L8c0523b8: *stop=0x8c0523b8u;r[0]=4u;goto L8c0523ba; /* mov #0x4,r0 */
L8c0523ba: *stop=0x8c0523bau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0523bc; /* fmov.s @(r0,r15),fr2 */
L8c0523bc: *stop=0x8c0523bcu;r[0]=20u;goto L8c0523be; /* mov #0x14,r0 */
L8c0523be: *stop=0x8c0523beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0523c0; /* fsub fr3,fr2 */
L8c0523c0: *stop=0x8c0523c0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0523c2; /* fmov.s @(r0,r15),fr3 */
L8c0523c2: *stop=0x8c0523c2u;fcopy(f,4u,2u);goto L8c0523c4; /* fmov fr2,fr4 */
L8c0523c4: *stop=0x8c0523c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c0523c6; /* fsub fr3,fr4 */
L8c0523c6: *stop=0x8c0523c6u;fcopy(f,3u,5u);goto L8c0523c8; /* fmov fr5,fr3 */
L8c0523c8: *stop=0x8c0523c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0523ca; /* fmul fr5,fr3 */
L8c0523ca: *stop=0x8c0523cau;fcopy(f,0u,6u);goto L8c0523cc; /* fmov fr6,fr0 */
L8c0523cc: *stop=0x8c0523ccu;r[0]=8u;goto L8c0523ce; /* mov #0x8,r0 */
L8c0523ce: *stop=0x8c0523ceu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0523d0; /* mov.l @(0x4,r14),r3 */
L8c0523d0: *stop=0x8c0523d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[6],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0523d2; /* fmac fr0,fr6,fr3 */
L8c0523d2: *stop=0x8c0523d2u;fcopy(f,0u,4u);goto L8c0523d4; /* fmov fr4,fr0 */
L8c0523d4: *stop=0x8c0523d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0523d6; /* fmac fr0,fr4,fr3 */
L8c0523d6: *stop=0x8c0523d6u;fcopy(f,5u,3u);goto L8c0523d8; /* fmov fr3,fr5 */
L8c0523d8: *stop=0x8c0523d8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0523da; /* fmov.s @(r0,r15),fr3 */
L8c0523da: *stop=0x8c0523dau;r[0]=36u;goto L8c0523dc; /* mov #0x24,r0 */
L8c0523dc: *stop=0x8c0523dcu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c0523de; /* fmov.s @(r0,r3),fr2 */
L8c0523de: *stop=0x8c0523deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0523e0; /* fadd fr3,fr2 */
L8c0523e0: *stop=0x8c0523e0u;fcopy(f,4u,2u);goto L8c0523e2; /* fmov fr2,fr4 */
L8c0523e2: *stop=0x8c0523e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[15],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c0523e4; /* fadd fr15,fr4 */
L8c0523e4: *stop=0x8c0523e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c0523e6; /* fmul fr4,fr4 */
L8c0523e6: *stop=0x8c0523e6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c0523e8; /* fcmp/gt fr5,fr4 */
L8c0523e8: if(!(*sr&1u))goto L8c05240c;goto L8c0523ea; /* bf 0x8c05240c */
L8c0523ea: *stop=0x8c0523eau;if(!valid_access(r[12]+4u,4u))return -3;wr(m,bus,&io_status,r[12]+0x4u,r[14],4);if(io_status)return io_status;goto L8c0523ec; /* mov.l r14,@(0x4,r12) */
L8c0523ec: *stop=0x8c0523ecu;if(!valid_access(r[11]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0523ee; /* mov.l @(0x8,r11),r2 */
L8c0523ee: *stop=0x8c0523eeu;if(!valid_access(r[12]+8u,4u))return -3;wr(m,bus,&io_status,r[12]+0x8u,r[2],4);if(io_status)return io_status;goto L8c0523f0; /* mov.l r2,@(0x8,r12) */
L8c0523f0: *stop=0x8c0523f0u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0523f2; /* mov.l @r11,r3 */
L8c0523f2: *stop=0x8c0523f2u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[3],4);if(io_status)return io_status;goto L8c0523f4; /* mov.l r3,@r12 */
L8c0523f4: *stop=0x8c0523f4u;if(!valid_access(r[13],2u))return -3;r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c0523f6; /* mov.w @r13,r2 */
L8c0523f6: *stop=0x8c0523f6u;r[2]+=1u;goto L8c0523f8; /* add #0x1,r2 */
L8c0523f8: *stop=0x8c0523f8u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[2],2);if(io_status)return io_status;goto L8c0523fa; /* mov.w r2,@r13 */
L8c0523fa: *stop=0x8c0523fau;if(!valid_access(r[13],2u))return -3;r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c0523fc; /* mov.w @r13,r3 */
L8c0523fc: *stop=0x8c0523fcu;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[10]);goto L8c0523fe; /* cmp/ge r10,r3 */
L8c0523fe: take=!(*sr&1u);*stop=0x8c052400u;r[12]+=12u;if(take)goto L8c05240c;goto L8c052402; /* bf/s 0x8c05240c */
L8c052402: *stop=0x8c052402u;{uint32_t loaded=rd(m,bus,&io_status,0x8c052470u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c052404; /* mov.l 0x8c052470,r3 */
L8c052404: target=r[3];*pr=0x8c052408u;*stop=0x8c052406u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052408; /* jsr @r3 */
L8c052408: *stop=0x8c05240au;goto L8c0524fc; /* bra 0x8c0524fc */
L8c05240c: *stop=0x8c05240cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c052450u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c05240e; /* mov.l 0x8c052450,r2 */
L8c05240e: *stop=0x8c05240eu;r[11]+=4294967284u;goto L8c052410; /* add #-0xc,r11 */
L8c052410: *stop=0x8c052410u;*sr=(*sr&~1u)|(r[11]>=r[2]);goto L8c052412; /* cmp/hs r2,r11 */
L8c052412: if((*sr&1u))goto L8c052316;goto L8c052414; /* bt 0x8c052316 */
L8c052414: *stop=0x8c052414u;{uint32_t loaded=rd(m,bus,&io_status,0x8c052470u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c052416; /* mov.l 0x8c052470,r2 */
L8c052416: target=r[2];*pr=0x8c05241au;*stop=0x8c052418u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c05241a; /* jsr @r2 */
L8c05241a: *stop=0x8c05241au;{uint32_t loaded=rd(m,bus,&io_status,0x8c052474u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c05241c; /* mov.l 0x8c052474,r3 */
L8c05241c: *stop=0x8c05241cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c05241e; /* mov.l @r3,r0 */
L8c05241e: *stop=0x8c05241eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c052420; /* cmp/eq #0x1,r0 */
L8c052420: if(!(*sr&1u))goto L8c0524fc;goto L8c052422; /* bf 0x8c0524fc */
L8c052422: *stop=0x8c052422u;{uint32_t loaded=rd(m,bus,&io_status,0x8c052478u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c052424; /* mov.l 0x8c052478,r2 */
L8c052424: *stop=0x8c052424u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c052426; /* mov.l @r2,r1 */
L8c052426: *stop=0x8c052426u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c052428; /* tst r1,r1 */
L8c052428: if((*sr&1u))goto L8c0524fc;goto L8c05242a; /* bt 0x8c0524fc */
L8c05242a: *stop=0x8c05242au;{uint32_t loaded=rd(m,bus,&io_status,0x8c052484u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c05242c; /* mov.l 0x8c052484,r8 */
L8c05242c: *stop=0x8c05242cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c052480u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c05242e; /* mov.l 0x8c052480,r3 */
L8c05242e: *stop=0x8c05242eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c05247cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c052430; /* mov.l 0x8c05247c,r14 */
L8c052430: *stop=0x8c052432u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[11]=loaded;}goto L8c0524f6; /* bra 0x8c0524f6 */
L8c0524a0: *stop=0x8c0524a0u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0524a2; /* mov.l @(0x20,r14),r3 */
L8c0524a2: *stop=0x8c0524a2u;*sr=(*sr&~1u)|!(r[8]&r[3]);goto L8c0524a4; /* tst r8,r3 */
L8c0524a4: if((*sr&1u))goto L8c0524f2;goto L8c0524a6; /* bt 0x8c0524f2 */
L8c0524a6: *stop=0x8c0524a6u;r[4]=r[14];goto L8c0524a8; /* mov r14,r4 */
L8c0524a8: *stop=0x8c0524a8u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0524aa; /* fmov.s @r4+,fr15 */
L8c0524aa: *stop=0x8c0524aau;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c0524ac; /* fmov.s @r15,fr3 */
L8c0524ac: *stop=0x8c0524acu;r[0]=4u;goto L8c0524ae; /* mov #0x4,r0 */
L8c0524ae: *stop=0x8c0524aeu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0524b0; /* fmov.s @r4+,fr13 */
L8c0524b0: *stop=0x8c0524b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c0524b2; /* fsub fr3,fr15 */
L8c0524b2: *stop=0x8c0524b2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0524b4; /* fmov.s @(r0,r15),fr2 */
L8c0524b4: *stop=0x8c0524b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[12],&f->fpscr,&fr[13],1);if(status)return status;}goto L8c0524b6; /* fsub fr12,fr13 */
L8c0524b6: *stop=0x8c0524b6u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[4]);if(io_status)return io_status;goto L8c0524b8; /* fmov.s @r4,fr14 */
L8c0524b8: *stop=0x8c0524b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c052600u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0524ba; /* mov.l 0x8c052600,r3 */
L8c0524ba: *stop=0x8c0524bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[2],&f->fpscr,&fr[14],1);if(status)return status;}goto L8c0524bc; /* fsub fr2,fr14 */
L8c0524bc: *stop=0x8c0524bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[15],&f->fpscr,&fr[15]);if(status)return status;}goto L8c0524be; /* fmul fr15,fr15 */
L8c0524be: *stop=0x8c0524beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[13],&f->fpscr,&fr[13]);if(status)return status;}goto L8c0524c0; /* fmul fr13,fr13 */
L8c0524c0: *stop=0x8c0524c0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[14],&fr[14],&f->fpscr,&fr[14]);if(status)return status;}goto L8c0524c2; /* fmul fr14,fr14 */
L8c0524c2: *stop=0x8c0524c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[13],&f->fpscr,&fr[15],0);if(status)return status;}goto L8c0524c4; /* fadd fr13,fr15 */
L8c0524c4: *stop=0x8c0524c4u;fcopy(f,4u,15u);goto L8c0524c6; /* fmov fr15,fr4 */
L8c0524c6: target=r[3];*pr=0x8c0524cau;*stop=0x8c0524c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[14],&f->fpscr,&fr[4],0);if(status)return status;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0524ca; /* jsr @r3 */
L8c0524ca: *stop=0x8c0524cau;r[0]=8u;goto L8c0524cc; /* mov #0x8,r0 */
L8c0524cc: *stop=0x8c0524ccu;fcopy(f,4u,0u);goto L8c0524ce; /* fmov fr0,fr4 */
L8c0524ce: *stop=0x8c0524ceu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0524d0; /* fmov.s @(r0,r15),fr3 */
L8c0524d0: *stop=0x8c0524d0u;r[0]=12u;goto L8c0524d2; /* mov #0xc,r0 */
L8c0524d2: *stop=0x8c0524d2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c0524d4; /* fmov.s @(r0,r14),fr5 */
L8c0524d4: *stop=0x8c0524d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c0524d6; /* fsub fr3,fr4 */
L8c0524d6: *stop=0x8c0524d6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c0524d8; /* fcmp/gt fr4,fr5 */
L8c0524d8: if(!(*sr&1u))goto L8c0524f2;goto L8c0524da; /* bf 0x8c0524f2 */
L8c0524da: *stop=0x8c0524dau;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0524dc; /* mov.l @(0x20,r14),r2 */
L8c0524dc: *stop=0x8c0524dcu;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[2],4);if(io_status)return io_status;goto L8c0524de; /* mov.l r2,@r12 */
L8c0524de: *stop=0x8c0524deu;if(!valid_access(r[12]+8u,4u))return -3;wr(m,bus,&io_status,r[12]+0x8u,r[9],4);if(io_status)return io_status;goto L8c0524e0; /* mov.l r9,@(0x8,r12) */
L8c0524e0: *stop=0x8c0524e0u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0524e2; /* mov.l @(0x18,r14),r3 */
L8c0524e2: *stop=0x8c0524e2u;if(!valid_access(r[12]+4u,4u))return -3;wr(m,bus,&io_status,r[12]+0x4u,r[3],4);if(io_status)return io_status;goto L8c0524e4; /* mov.l r3,@(0x4,r12) */
L8c0524e4: *stop=0x8c0524e4u;if(!valid_access(r[13],2u))return -3;r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c0524e6; /* mov.w @r13,r2 */
L8c0524e6: *stop=0x8c0524e6u;r[2]+=1u;goto L8c0524e8; /* add #0x1,r2 */
L8c0524e8: *stop=0x8c0524e8u;if(!valid_access(r[13],2u))return -3;wr(m,bus,&io_status,r[13],r[2],2);if(io_status)return io_status;goto L8c0524ea; /* mov.w r2,@r13 */
L8c0524ea: *stop=0x8c0524eau;if(!valid_access(r[13],2u))return -3;r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c0524ec; /* mov.w @r13,r3 */
L8c0524ec: *stop=0x8c0524ecu;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[10]);goto L8c0524ee; /* cmp/ge r10,r3 */
L8c0524ee: take=(*sr&1u);*stop=0x8c0524f0u;r[12]+=12u;if(take)goto L8c0524fc;goto L8c0524f2; /* bt/s 0x8c0524fc */
L8c0524f2: *stop=0x8c0524f2u;r[11]+=4294967295u;goto L8c0524f4; /* add #-0x1,r11 */
L8c0524f4: *stop=0x8c0524f4u;r[14]+=36u;goto L8c0524f6; /* add #0x24,r14 */
L8c0524f6: *stop=0x8c0524f6u;r[3]=(uint32_t)(int32_t)(int16_t)r[11];goto L8c0524f8; /* exts.w r11,r3 */
L8c0524f8: *stop=0x8c0524f8u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c0524fa; /* cmp/pl r3 */
L8c0524fa: if((*sr&1u))goto L8c0524a0;goto L8c0524fc; /* bt 0x8c0524a0 */
L8c0524fc: *stop=0x8c0524fcu;r[15]+=24u;goto L8c0524fe; /* add #0x18,r15 */
L8c0524fe: *stop=0x8c0524feu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c052500; /* lds.l @r15+,PR */
L8c052500: *stop=0x8c052500u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c052502; /* fmov.s @r15+,fr12 */
L8c052502: *stop=0x8c052502u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c052504; /* fmov.s @r15+,fr13 */
L8c052504: *stop=0x8c052504u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c052506; /* fmov.s @r15+,fr14 */
L8c052506: *stop=0x8c052506u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c052508; /* fmov.s @r15+,fr15 */
L8c052508: *stop=0x8c052508u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c05250a; /* mov.l @r15+,r8 */
L8c05250a: *stop=0x8c05250au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c05250c; /* mov.l @r15+,r9 */
L8c05250c: *stop=0x8c05250cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c05250e; /* mov.l @r15+,r10 */
L8c05250e: *stop=0x8c05250eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c052510; /* mov.l @r15+,r11 */
L8c052510: *stop=0x8c052510u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c052512; /* mov.l @r15+,r12 */
L8c052512: *stop=0x8c052512u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c052514; /* mov.l @r15+,r13 */
L8c052514: *stop=0x8c052514u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c052516u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_world_8c0522c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c0522c0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_world_resume_8c0522f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c0522c0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0522f4u);}
SA_STAGE_EXPORT int sa_movement_world_resume_8c05235a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c0522c0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c05235au);}
SA_STAGE_EXPORT int sa_movement_world_resume_8c052368(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c0522c0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c052368u);}
SA_STAGE_EXPORT int sa_movement_world_resume_8c052376(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c0522c0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c052376u);}
SA_STAGE_EXPORT int sa_movement_world_resume_8c052384(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c0522c0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c052384u);}
SA_STAGE_EXPORT int sa_movement_world_resume_8c052392(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c0522c0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c052392u);}
SA_STAGE_EXPORT int sa_movement_world_resume_8c052408(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c0522c0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c052408u);}
SA_STAGE_EXPORT int sa_movement_world_resume_8c05241a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c0522c0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c05241au);}
SA_STAGE_EXPORT int sa_movement_world_resume_8c0524ca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_world_run_8c0522c0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0524cau);}

SA_STAGE_EXPORT int sa_movement_world_8c63a8e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63a8e0;
L8c63a8e0: *stop=0x8c63a8e0u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c63a8e2; /* tst r4,r4 */
L8c63a8e2: *stop=0x8c63a8e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63a8fcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c63a8e4; /* mov.l 0x8c63a8fc,r5 */
L8c63a8e4: if(!(*sr&1u))goto L8c63a900;goto L8c63a8e6; /* bf 0x8c63a900 */
L8c63a8e6: *stop=0x8c63a8e6u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63a8e8; /* fschg */
L8c63a8e8: *stop=0x8c63a8e8u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a8ea; /* fmov.s @r5+,fr1 */
L8c63a8ea: *stop=0x8c63a8eau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a8ec; /* fmov.s @r5+,fr3 */
L8c63a8ec: *stop=0x8c63a8ecu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a8ee; /* fmov.s @r5+,fr5 */
L8c63a8ee: *stop=0x8c63a8eeu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a8f0; /* fmov.s @r5+,fr7 */
L8c63a8f0: *stop=0x8c63a8f0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a8f2; /* fmov.s @r5+,fr9 */
L8c63a8f2: *stop=0x8c63a8f2u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a8f4; /* fmov.s @r5+,fr11 */
L8c63a8f4: *stop=0x8c63a8f4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a8f6; /* fmov.s @r5+,fr13 */
L8c63a8f6: *stop=0x8c63a8f6u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a8f8; /* fmov.s @r5+,fr15 */
L8c63a8f8: *stop=0x8c63a8f8u;if(*pr!=entry_pr)return -3;*stop=0x8c63a8fau;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;return 0; /* rts */
L8c63a900: *stop=0x8c63a900u;r[4]+=32u;goto L8c63a902; /* add #0x20,r4 */
L8c63a902: *stop=0x8c63a902u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a904; /* fmov.s @r5+,fr0 */
L8c63a904: *stop=0x8c63a904u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a906; /* fmov.s @r5+,fr1 */
L8c63a906: *stop=0x8c63a906u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a908; /* fmov.s @r5+,fr2 */
L8c63a908: *stop=0x8c63a908u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a90a; /* fmov.s @r5+,fr3 */
L8c63a90a: *stop=0x8c63a90au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a90c; /* fmov.s @r5+,fr4 */
L8c63a90c: *stop=0x8c63a90cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a90e; /* fmov.s @r5+,fr5 */
L8c63a90e: *stop=0x8c63a90eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a910; /* fmov.s @r5+,fr6 */
L8c63a910: *stop=0x8c63a910u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a912; /* fmov.s @r5+,fr7 */
L8c63a912: *stop=0x8c63a912u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c63a914; /* fmov.s fr7,@-r4 */
L8c63a914: *stop=0x8c63a914u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c63a916; /* fmov.s fr6,@-r4 */
L8c63a916: *stop=0x8c63a916u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c63a918; /* fmov.s fr5,@-r4 */
L8c63a918: *stop=0x8c63a918u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c63a91a; /* fmov.s fr4,@-r4 */
L8c63a91a: *stop=0x8c63a91au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c63a91c; /* fmov.s fr3,@-r4 */
L8c63a91c: *stop=0x8c63a91cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c63a91e; /* fmov.s fr2,@-r4 */
L8c63a91e: *stop=0x8c63a91eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;goto L8c63a920; /* fmov.s fr1,@-r4 */
L8c63a920: *stop=0x8c63a920u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c63a922; /* fmov.s fr0,@-r4 */
L8c63a922: *stop=0x8c63a922u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a924; /* fmov.s @r5+,fr0 */
L8c63a924: *stop=0x8c63a924u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a926; /* fmov.s @r5+,fr1 */
L8c63a926: *stop=0x8c63a926u;r[4]+=64u;goto L8c63a928; /* add #0x40,r4 */
L8c63a928: *stop=0x8c63a928u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a92a; /* fmov.s @r5+,fr2 */
L8c63a92a: *stop=0x8c63a92au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a92c; /* fmov.s @r5+,fr3 */
L8c63a92c: *stop=0x8c63a92cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a92e; /* fmov.s @r5+,fr4 */
L8c63a92e: *stop=0x8c63a92eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a930; /* fmov.s @r5+,fr5 */
L8c63a930: *stop=0x8c63a930u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a932; /* fmov.s @r5+,fr6 */
L8c63a932: *stop=0x8c63a932u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63a934; /* fmov.s @r5+,fr7 */
L8c63a934: *stop=0x8c63a934u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c63a936; /* fmov.s fr7,@-r4 */
L8c63a936: *stop=0x8c63a936u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c63a938; /* fmov.s fr6,@-r4 */
L8c63a938: *stop=0x8c63a938u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c63a93a; /* fmov.s fr5,@-r4 */
L8c63a93a: *stop=0x8c63a93au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c63a93c; /* fmov.s fr4,@-r4 */
L8c63a93c: *stop=0x8c63a93cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c63a93e; /* fmov.s fr3,@-r4 */
L8c63a93e: *stop=0x8c63a93eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c63a940; /* fmov.s fr2,@-r4 */
L8c63a940: *stop=0x8c63a940u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;goto L8c63a942; /* fmov.s fr1,@-r4 */
L8c63a942: *stop=0x8c63a942u;if(*pr!=entry_pr)return -3;*stop=0x8c63a944u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;return 0; /* rts */
}
