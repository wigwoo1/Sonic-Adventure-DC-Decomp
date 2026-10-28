/* Original actor orientation response, local-vector transform and circular short-angle approach.
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
static int sa_movement_orientation_response_run_8c074ee4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c074f06u)goto L8c074f06;if(resume_pc==0x8c074f12u)goto L8c074f12;if(resume_pc==0x8c074f9au)goto L8c074f9a;if(resume_pc==0x8c074faeu)goto L8c074fae;if(resume_pc==0x8c074fc0u)goto L8c074fc0;if(resume_pc==0x8c07504cu)goto L8c07504c;if(resume_pc==0x8c075058u)goto L8c075058;goto L8c074ee4;
L8c074ee4: *stop=0x8c074ee4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c074ee6; /* mov.l r14,@-r15 */
L8c074ee6: *stop=0x8c074ee6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c074ee8; /* mov.l r13,@-r15 */
L8c074ee8: *stop=0x8c074ee8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c074eea; /* mov.l r12,@-r15 */
L8c074eea: *stop=0x8c074eeau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c074eec; /* mov.l r11,@-r15 */
L8c074eec: *stop=0x8c074eecu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c074eee; /* fmov.s fr15,@-r15 */
L8c074eee: *stop=0x8c074eeeu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c074ef0; /* fmov.s fr14,@-r15 */
L8c074ef0: *stop=0x8c074ef0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c074ef2; /* fmov.s fr12,@-r15 */
L8c074ef2: *stop=0x8c074ef2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c074ef4; /* sts.l PR,@-r15 */
L8c074ef4: *stop=0x8c074ef4u;r[15]+=4294967284u;goto L8c074ef6; /* add #-0xc,r15 */
L8c074ef6: *stop=0x8c074ef6u;r[12]=r[4];goto L8c074ef8; /* mov r4,r12 */
L8c074ef8: *stop=0x8c074ef8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074f5cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c074efa; /* mov.l 0x8c074f5c,r4 */
L8c074efa: *stop=0x8c074efau;{uint32_t loaded=rd(m,bus,&io_status,0x8c074f60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074efc; /* mov.l 0x8c074f60,r3 */
L8c074efc: *stop=0x8c074efcu;r[13]=r[5];goto L8c074efe; /* mov r5,r13 */
L8c074efe: *stop=0x8c074efeu;r[11]=r[6];goto L8c074f00; /* mov r6,r11 */
L8c074f00: *stop=0x8c074f00u;r[5]=r[6];goto L8c074f02; /* mov r6,r5 */
L8c074f02: target=r[3];*pr=0x8c074f06u;*stop=0x8c074f04u;r[5]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074f06; /* jsr @r3 */
L8c074f06: *stop=0x8c074f06u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074f5cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c074f08; /* mov.l 0x8c074f5c,r4 */
L8c074f08: *stop=0x8c074f08u;r[5]=r[11];goto L8c074f0a; /* mov r11,r5 */
L8c074f0a: *stop=0x8c074f0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c074f60u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074f0c; /* mov.l 0x8c074f60,r2 */
L8c074f0c: *stop=0x8c074f0cu;fcopy(f,12u,0u);goto L8c074f0e; /* fmov fr0,fr12 */
L8c074f0e: target=r[2];*pr=0x8c074f12u;*stop=0x8c074f10u;r[5]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074f12; /* jsr @r2 */
L8c074f12: *stop=0x8c074f12u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c074f14; /* fcmp/gt fr0,fr12 */
L8c074f14: if((*sr&1u))goto L8c074f26;goto L8c074f16; /* bt 0x8c074f26 */
L8c074f16: *stop=0x8c074f16u;if(!valid_access(r[13]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x10u,2);if(io_status)return io_status;goto L8c074f18; /* mov.w @(0x10,r13),r0 */
L8c074f18: *stop=0x8c074f18u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c074f1a; /* tst #0x1,r0 */
L8c074f1a: if((*sr&1u))goto L8c074f26;goto L8c074f1c; /* bt 0x8c074f26 */
L8c074f1c: *stop=0x8c074f1cu;r[3]=32u;goto L8c074f1e; /* mov #0x20,r3 */
L8c074f1e: *stop=0x8c074f1eu;if(!valid_access(r[13]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x10u,2);if(io_status)return io_status;goto L8c074f20; /* mov.w @(0x10,r13),r0 */
L8c074f20: *stop=0x8c074f20u;r[3]<<=8;goto L8c074f22; /* shll8 r3 */
L8c074f22: *stop=0x8c074f22u;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c074f24; /* tst r3,r0 */
L8c074f24: if((*sr&1u))goto L8c074f80;goto L8c074f26; /* bt 0x8c074f80 */
L8c074f26: *stop=0x8c074f26u;if(!valid_access(r[11]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x34u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c074f28; /* mov.l @(0x34,r11),r1 */
L8c074f28: *stop=0x8c074f28u;if(!valid_access(r[12]+20u,4u))return -3;wr(m,bus,&io_status,r[12]+0x14u,r[1],4);if(io_status)return io_status;goto L8c074f2a; /* mov.l r1,@(0x14,r12) */
L8c074f2a: *stop=0x8c074f2au;if(!valid_access(r[11]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074f2c; /* mov.l @(0x38,r11),r3 */
L8c074f2c: *stop=0x8c074f2eu;if(!valid_access(r[12]+28u,4u))return -3;wr(m,bus,&io_status,r[12]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c07505a; /* bra 0x8c07505a */
L8c074f80: *stop=0x8c074f80u;r[2]=r[11];goto L8c074f82; /* mov r11,r2 */
L8c074f82: *stop=0x8c074f82u;r[2]+=72u;goto L8c074f84; /* add #0x48,r2 */
L8c074f84: *stop=0x8c074f84u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c074f86; /* mov.l @r2,r0 */
L8c074f86: *stop=0x8c074f86u;r[1]=r[15];goto L8c074f88; /* mov r15,r1 */
L8c074f88: *stop=0x8c074f88u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074f8a; /* mov.l @(0x4,r2),r3 */
L8c074f8a: *stop=0x8c074f8au;r[5]=r[15];goto L8c074f8c; /* mov r15,r5 */
L8c074f8c: *stop=0x8c074f8cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c074f8e; /* mov.l r0,@r1 */
L8c074f8e: *stop=0x8c074f8eu;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[3],4);if(io_status)return io_status;goto L8c074f90; /* mov.l r3,@(0x4,r1) */
L8c074f90: *stop=0x8c074f90u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c074f92; /* mov.l @(0x8,r2),r0 */
L8c074f92: *stop=0x8c074f92u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075180u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074f94; /* mov.l 0x8c075180,r2 */
L8c074f94: *stop=0x8c074f94u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[0],4);if(io_status)return io_status;goto L8c074f96; /* mov.l r0,@(0x8,r1) */
L8c074f96: target=r[2];*pr=0x8c074f9au;*stop=0x8c074f98u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074f9a; /* jsr @r2 */
L8c074f9a: *stop=0x8c074f9au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c074f9c; /* fmov.s @r15,fr15 */
L8c074f9c: *stop=0x8c074f9cu;r[0]=8u;goto L8c074f9e; /* mov #0x8,r0 */
L8c074f9e: *stop=0x8c074f9eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]+r[15]);if(io_status)return io_status;goto L8c074fa0; /* fmov.s @(r0,r15),fr14 */
L8c074fa0: *stop=0x8c074fa0u;fcopy(f,3u,15u);goto L8c074fa2; /* fmov fr15,fr3 */
L8c074fa2: *stop=0x8c074fa2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c074fa4; /* fmul fr15,fr3 */
L8c074fa4: *stop=0x8c074fa4u;fcopy(f,0u,14u);goto L8c074fa6; /* fmov fr14,fr0 */
L8c074fa6: *stop=0x8c074fa6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075184u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074fa8; /* mov.l 0x8c075184,r3 */
L8c074fa8: *stop=0x8c074fa8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[14],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c074faa; /* fmac fr0,fr14,fr3 */
L8c074faa: target=r[3];*pr=0x8c074faeu;*stop=0x8c074facu;fcopy(f,4u,3u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074fae; /* jsr @r3 */
L8c074fae: *stop=0x8c074faeu;r[0]=8u;goto L8c074fb0; /* mov #0x8,r0 */
L8c074fb0: *stop=0x8c074fb0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c074fb2; /* fldi0 fr3 */
L8c074fb2: *stop=0x8c074fb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075188u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074fb4; /* mov.l 0x8c075188,r3 */
L8c074fb4: *stop=0x8c074fb4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c074fb6; /* fmov.s fr0,@r15 */
L8c074fb6: *stop=0x8c074fb6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c074fb8; /* fmov fr3,@(r0,r15) */
L8c074fb8: *stop=0x8c074fb8u;r[0]=4u;goto L8c074fba; /* mov #0x4,r0 */
L8c074fba: *stop=0x8c074fbau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[15]);if(io_status)return io_status;goto L8c074fbc; /* fmov.s @(r0,r15),fr5 */
L8c074fbc: target=r[3];*pr=0x8c074fc0u;*stop=0x8c074fbeu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074fc0; /* jsr @r3 */
L8c074fc0: *stop=0x8c074fc0u;r[0]=0x8c07518cu;goto L8c074fc2; /* mova 0x8c07518c,r0 */
L8c074fc2: *stop=0x8c074fc2u;fcopy(f,2u,0u);goto L8c074fc4; /* fmov fr0,fr2 */
L8c074fc4: *stop=0x8c074fc4u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c074fc6; /* fmov.s @r0,fr3 */
L8c074fc6: *stop=0x8c074fc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075190u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074fc8; /* mov.l 0x8c075190,r3 */
L8c074fc8: *stop=0x8c074fc8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c074fca; /* fmul fr3,fr2 */
L8c074fca: *stop=0x8c074fcau;{uint32_t loaded=rd(m,bus,&io_status,0x8c075194u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074fcc; /* mov.l 0x8c075194,r2 */
L8c074fcc: *stop=0x8c074fccu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c074fce; /* ftrc fr2,FPUL */
L8c074fce: *stop=0x8c074fceu;r[14]=*fpul;goto L8c074fd0; /* sts FPUL,r14 */
L8c074fd0: *stop=0x8c074fd0u;r[14]&=r[3];goto L8c074fd2; /* and r3,r14 */
L8c074fd2: *stop=0x8c074fd2u;*sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[2]);goto L8c074fd4; /* cmp/gt r2,r14 */
L8c074fd4: if(!(*sr&1u))goto L8c074fdc;goto L8c074fd6; /* bf 0x8c074fdc */
L8c074fd6: *stop=0x8c074fd6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075198u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c074fd8; /* mov.l 0x8c075198,r0 */
L8c074fd8: *stop=0x8c074fd8u;r[0]-=r[14];goto L8c074fda; /* sub r14,r0 */
L8c074fda: *stop=0x8c074fdau;r[14]=r[0];goto L8c074fdc; /* mov r0,r14 */
L8c074fdc: *stop=0x8c074fdcu;if(!valid_access(r[12]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x4u,2);if(io_status)return io_status;goto L8c074fde; /* mov.w @(0x4,r12),r0 */
L8c074fde: *stop=0x8c074fdeu;r[4]=32u;goto L8c074fe0; /* mov #0x20,r4 */
L8c074fe0: *stop=0x8c074fe0u;r[4]<<=8;goto L8c074fe2; /* shll8 r4 */
L8c074fe2: *stop=0x8c074fe2u;*sr=(*sr&~1u)|!(r[4]&r[0]);goto L8c074fe4; /* tst r4,r0 */
L8c074fe4: if(!(*sr&1u))goto L8c075042;goto L8c074fe6; /* bf 0x8c075042 */
L8c074fe6: *stop=0x8c074fe6u;r[0]=4294967248u;goto L8c074fe8; /* mov #-0x30,r0 */
L8c074fe8: *stop=0x8c074fe8u;r[0]=r[0]&255u;goto L8c074fea; /* extu.b r0,r0 */
L8c074fea: *stop=0x8c074feau;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c074fec; /* fmov.s @(r0,r13),fr3 */
L8c074fec: *stop=0x8c074fecu;r[0]=56u;goto L8c074fee; /* mov #0x38,r0 */
L8c074fee: *stop=0x8c074feeu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074ff0; /* fmov.s @(r0,r13),fr2 */
L8c074ff0: *stop=0x8c074ff0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c074ff2; /* fcmp/gt fr2,fr3 */
L8c074ff2: if(!(*sr&1u))goto L8c075042;goto L8c074ff4; /* bf 0x8c075042 */
L8c074ff4: *stop=0x8c074ff4u;r[0]=4294967240u;goto L8c074ff6; /* mov #-0x38,r0 */
L8c074ff6: *stop=0x8c074ff6u;r[0]=r[0]&255u;goto L8c074ff8; /* extu.b r0,r0 */
L8c074ff8: *stop=0x8c074ff8u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c074ffa; /* fmov.s @(r0,r13),fr3 */
L8c074ffa: *stop=0x8c074ffau;r[0]=56u;goto L8c074ffc; /* mov #0x38,r0 */
L8c074ffc: *stop=0x8c074ffcu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074ffe; /* fmov.s @(r0,r13),fr2 */
L8c074ffe: *stop=0x8c074ffeu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c075000; /* fcmp/gt fr2,fr3 */
L8c075000: if((*sr&1u))goto L8c07500a;goto L8c075002; /* bt 0x8c07500a */
L8c075002: *stop=0x8c075002u;*sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[4]);goto L8c075004; /* cmp/gt r4,r14 */
L8c075004: if(!(*sr&1u))goto L8c075042;goto L8c075006; /* bf 0x8c075042 */
L8c075006: *stop=0x8c075008u;r[14]=r[4];goto L8c075042; /* bra 0x8c075042 */
L8c07500a: *stop=0x8c07500au;r[0]=4294967236u;goto L8c07500c; /* mov #-0x3c,r0 */
L8c07500c: *stop=0x8c07500cu;r[0]=r[0]&255u;goto L8c07500e; /* extu.b r0,r0 */
L8c07500e: *stop=0x8c07500eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c075010; /* fmov.s @(r0,r13),fr3 */
L8c075010: *stop=0x8c075010u;r[0]=56u;goto L8c075012; /* mov #0x38,r0 */
L8c075012: *stop=0x8c075012u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c075014; /* fmov.s @(r0,r13),fr2 */
L8c075014: *stop=0x8c075014u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c075016; /* fcmp/gt fr2,fr3 */
L8c075016: if((*sr&1u))goto L8c075024;goto L8c075018; /* bt 0x8c075024 */
L8c075018: *stop=0x8c075018u;r[4]=21u;goto L8c07501a; /* mov #0x15,r4 */
L8c07501a: *stop=0x8c07501au;r[4]<<=8;goto L8c07501c; /* shll8 r4 */
L8c07501c: *stop=0x8c07501cu;*sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[4]);goto L8c07501e; /* cmp/gt r4,r14 */
L8c07501e: if(!(*sr&1u))goto L8c075042;goto L8c075020; /* bf 0x8c075042 */
L8c075020: *stop=0x8c075022u;r[14]=r[4];goto L8c075042; /* bra 0x8c075042 */
L8c075024: *stop=0x8c075024u;r[0]=4294967232u;goto L8c075026; /* mov #-0x40,r0 */
L8c075026: *stop=0x8c075026u;r[0]=r[0]&255u;goto L8c075028; /* extu.b r0,r0 */
L8c075028: *stop=0x8c075028u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c07502a; /* fmov.s @(r0,r13),fr3 */
L8c07502a: *stop=0x8c07502au;r[0]=56u;goto L8c07502c; /* mov #0x38,r0 */
L8c07502c: *stop=0x8c07502cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07502e; /* fmov.s @(r0,r13),fr2 */
L8c07502e: *stop=0x8c07502eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c075030; /* fcmp/gt fr2,fr3 */
L8c075030: if((*sr&1u))goto L8c07503e;goto L8c075032; /* bt 0x8c07503e */
L8c075032: *stop=0x8c075032u;r[4]=16u;goto L8c075034; /* mov #0x10,r4 */
L8c075034: *stop=0x8c075034u;r[4]<<=8;goto L8c075036; /* shll8 r4 */
L8c075036: *stop=0x8c075036u;*sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[4]);goto L8c075038; /* cmp/gt r4,r14 */
L8c075038: if(!(*sr&1u))goto L8c075042;goto L8c07503a; /* bf 0x8c075042 */
L8c07503a: *stop=0x8c07503cu;r[14]=r[4];goto L8c075042; /* bra 0x8c075042 */
L8c07503e: *stop=0x8c07503eu;r[14]=4u;goto L8c075040; /* mov #0x4,r14 */
L8c075040: *stop=0x8c075040u;r[14]<<=8;goto L8c075042; /* shll8 r14 */
L8c075042: *stop=0x8c075042u;{uint32_t loaded=rd(m,bus,&io_status,0x8c07519cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075044; /* mov.l 0x8c07519c,r2 */
L8c075044: *stop=0x8c075044u;r[6]=r[14];goto L8c075046; /* mov r14,r6 */
L8c075046: *stop=0x8c075046u;if(!valid_access(r[11]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x34u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c075048; /* mov.l @(0x34,r11),r5 */
L8c075048: target=r[2];*pr=0x8c07504cu;*stop=0x8c07504au;if(!valid_access(r[12]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07504c; /* jsr @r2 */
L8c07504c: *stop=0x8c07504cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c07519cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07504e; /* mov.l 0x8c07519c,r3 */
L8c07504e: *stop=0x8c07504eu;r[6]=r[14];goto L8c075050; /* mov r14,r6 */
L8c075050: *stop=0x8c075050u;if(!valid_access(r[12]+20u,4u))return -3;wr(m,bus,&io_status,r[12]+0x14u,r[0],4);if(io_status)return io_status;goto L8c075052; /* mov.l r0,@(0x14,r12) */
L8c075052: *stop=0x8c075052u;if(!valid_access(r[11]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x38u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c075054; /* mov.l @(0x38,r11),r5 */
L8c075054: target=r[3];*pr=0x8c075058u;*stop=0x8c075056u;if(!valid_access(r[12]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x1cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075058; /* jsr @r3 */
L8c075058: *stop=0x8c075058u;if(!valid_access(r[12]+28u,4u))return -3;wr(m,bus,&io_status,r[12]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c07505a; /* mov.l r0,@(0x1c,r12) */
L8c07505a: *stop=0x8c07505au;r[15]+=12u;goto L8c07505c; /* add #0xc,r15 */
L8c07505c: *stop=0x8c07505cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c07505e; /* lds.l @r15+,PR */
L8c07505e: *stop=0x8c07505eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075060; /* fmov.s @r15+,fr12 */
L8c075060: *stop=0x8c075060u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075062; /* fmov.s @r15+,fr14 */
L8c075062: *stop=0x8c075062u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075064; /* fmov.s @r15+,fr15 */
L8c075064: *stop=0x8c075064u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c075066; /* mov.l @r15+,r11 */
L8c075066: *stop=0x8c075066u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c075068; /* mov.l @r15+,r12 */
L8c075068: *stop=0x8c075068u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c07506a; /* mov.l @r15+,r13 */
L8c07506a: *stop=0x8c07506au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c07506cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_orientation_response_8c074ee4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c074ee4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_orientation_response_resume_8c074f06(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c074ee4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074f06u);}
SA_STAGE_EXPORT int sa_movement_orientation_response_resume_8c074f12(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c074ee4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074f12u);}
SA_STAGE_EXPORT int sa_movement_orientation_response_resume_8c074f9a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c074ee4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074f9au);}
SA_STAGE_EXPORT int sa_movement_orientation_response_resume_8c074fae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c074ee4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074faeu);}
SA_STAGE_EXPORT int sa_movement_orientation_response_resume_8c074fc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c074ee4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074fc0u);}
SA_STAGE_EXPORT int sa_movement_orientation_response_resume_8c07504c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c074ee4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07504cu);}
SA_STAGE_EXPORT int sa_movement_orientation_response_resume_8c075058(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c074ee4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075058u);}

static int sa_movement_orientation_response_run_8c078e40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c078e62u)goto L8c078e62;if(resume_pc==0x8c078e72u)goto L8c078e72;if(resume_pc==0x8c078e84u)goto L8c078e84;if(resume_pc==0x8c078e96u)goto L8c078e96;if(resume_pc==0x8c078ea2u)goto L8c078ea2;if(resume_pc==0x8c078ea8u)goto L8c078ea8;goto L8c078e40;
L8c078e40: *stop=0x8c078e40u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c078e42; /* mov.l r14,@-r15 */
L8c078e42: *stop=0x8c078e42u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c078e44; /* sts.l PR,@-r15 */
L8c078e44: *stop=0x8c078e44u;r[15]+=4294967276u;goto L8c078e46; /* add #-0x14,r15 */
L8c078e46: *stop=0x8c078e46u;r[14]=r[4];goto L8c078e48; /* mov r4,r14 */
L8c078e48: *stop=0x8c078e48u;r[4]=r[5];goto L8c078e4a; /* mov r5,r4 */
L8c078e4a: *stop=0x8c078e4au;r[0]=8u;goto L8c078e4c; /* mov #0x8,r0 */
L8c078e4c: *stop=0x8c078e4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c078fc0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c078e4e; /* mov.l 0x8c078fc0,r3 */
L8c078e4e: *stop=0x8c078e4eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c078e50; /* mov.l r5,@(0x4,r15) */
L8c078e50: *stop=0x8c078e50u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c078e52; /* fmov.s @r4+,fr3 */
L8c078e52: *stop=0x8c078e52u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c078e54; /* fmov fr3,@(r0,r15) */
L8c078e54: *stop=0x8c078e54u;r[0]=12u;goto L8c078e56; /* mov #0xc,r0 */
L8c078e56: *stop=0x8c078e56u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c078e58; /* fmov.s @r4+,fr3 */
L8c078e58: *stop=0x8c078e58u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c078e5a; /* fmov fr3,@(r0,r15) */
L8c078e5a: *stop=0x8c078e5au;r[0]=16u;goto L8c078e5c; /* mov #0x10,r0 */
L8c078e5c: *stop=0x8c078e5cu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c078e5e; /* fmov.s @r4,fr3 */
L8c078e5e: target=r[3];*pr=0x8c078e62u;*stop=0x8c078e60u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078e62; /* jsr @r3 */
L8c078e62: *stop=0x8c078e62u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c078e64; /* mov.l @(0x18,r14),r2 */
L8c078e64: *stop=0x8c078e64u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c078e66; /* tst r2,r2 */
L8c078e66: take=(*sr&1u);*stop=0x8c078e68u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c078e72;goto L8c078e6a; /* bt/s 0x8c078e72 */
L8c078e6a: *stop=0x8c078e6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c078fc4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c078e6c; /* mov.l 0x8c078fc4,r3 */
L8c078e6c: *stop=0x8c078e6cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c078e6e; /* mov.l @r15,r5 */
L8c078e6e: target=r[3];*pr=0x8c078e72u;*stop=0x8c078e70u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078e72; /* jsr @r3 */
L8c078e72: *stop=0x8c078e72u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c078e74; /* mov.l @(0x14,r14),r3 */
L8c078e74: *stop=0x8c078e74u;r[3]=0u-r[3];goto L8c078e76; /* neg r3,r3 */
L8c078e76: *stop=0x8c078e76u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c078e78; /* tst r3,r3 */
L8c078e78: take=(*sr&1u);*stop=0x8c078e7au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c078e84;goto L8c078e7c; /* bt/s 0x8c078e84 */
L8c078e7c: *stop=0x8c078e7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c078fc8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c078e7e; /* mov.l 0x8c078fc8,r1 */
L8c078e7e: *stop=0x8c078e7eu;r[5]=r[3];goto L8c078e80; /* mov r3,r5 */
L8c078e80: target=r[1];*pr=0x8c078e84u;*stop=0x8c078e82u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078e84; /* jsr @r1 */
L8c078e84: *stop=0x8c078e84u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c078e86; /* mov.l @(0x1c,r14),r2 */
L8c078e86: *stop=0x8c078e86u;r[2]=0u-r[2];goto L8c078e88; /* neg r2,r2 */
L8c078e88: *stop=0x8c078e88u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c078e8a; /* tst r2,r2 */
L8c078e8a: take=(*sr&1u);*stop=0x8c078e8cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c078e96;goto L8c078e8e; /* bt/s 0x8c078e96 */
L8c078e8e: *stop=0x8c078e8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c078fccu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c078e90; /* mov.l 0x8c078fcc,r1 */
L8c078e90: *stop=0x8c078e90u;r[5]=r[2];goto L8c078e92; /* mov r2,r5 */
L8c078e92: target=r[1];*pr=0x8c078e96u;*stop=0x8c078e94u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078e96; /* jsr @r1 */
L8c078e96: *stop=0x8c078e96u;{uint32_t loaded=rd(m,bus,&io_status,0x8c078fd0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c078e98; /* mov.l 0x8c078fd0,r3 */
L8c078e98: *stop=0x8c078e98u;r[5]=r[15];goto L8c078e9a; /* mov r15,r5 */
L8c078e9a: *stop=0x8c078e9au;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c078e9c; /* mov.l @(0x4,r15),r6 */
L8c078e9c: *stop=0x8c078e9cu;r[5]+=8u;goto L8c078e9e; /* add #0x8,r5 */
L8c078e9e: target=r[3];*pr=0x8c078ea2u;*stop=0x8c078ea0u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078ea2; /* jsr @r3 */
L8c078ea2: *stop=0x8c078ea2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c078fd4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c078ea4; /* mov.l 0x8c078fd4,r2 */
L8c078ea4: target=r[2];*pr=0x8c078ea8u;*stop=0x8c078ea6u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c078ea8; /* jsr @r2 */
L8c078ea8: *stop=0x8c078ea8u;r[15]+=20u;goto L8c078eaa; /* add #0x14,r15 */
L8c078eaa: *stop=0x8c078eaau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c078eac; /* lds.l @r15+,PR */
L8c078eac: *stop=0x8c078eacu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c078eaeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_orientation_response_8c078e40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c078e40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_orientation_response_resume_8c078e62(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c078e40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078e62u);}
SA_STAGE_EXPORT int sa_movement_orientation_response_resume_8c078e72(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c078e40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078e72u);}
SA_STAGE_EXPORT int sa_movement_orientation_response_resume_8c078e84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c078e40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078e84u);}
SA_STAGE_EXPORT int sa_movement_orientation_response_resume_8c078e96(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c078e40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078e96u);}
SA_STAGE_EXPORT int sa_movement_orientation_response_resume_8c078ea2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c078e40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078ea2u);}
SA_STAGE_EXPORT int sa_movement_orientation_response_resume_8c078ea8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_orientation_response_run_8c078e40(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c078ea8u);}

SA_STAGE_EXPORT int sa_movement_orientation_response_8c055d20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c055d20;
L8c055d20: *stop=0x8c055d20u;r[5]=r[5]&65535u;goto L8c055d22; /* extu.w r5,r5 */
L8c055d22: *stop=0x8c055d22u;r[7]=r[5];goto L8c055d24; /* mov r5,r7 */
L8c055d24: *stop=0x8c055d24u;r[4]=r[4]&65535u;goto L8c055d26; /* extu.w r4,r4 */
L8c055d26: *stop=0x8c055d26u;r[7]-=r[4];goto L8c055d28; /* sub r4,r7 */
L8c055d28: *stop=0x8c055d28u;r[0]=(uint32_t)(int32_t)(int16_t)r[7];goto L8c055d2a; /* exts.w r7,r0 */
L8c055d2a: *stop=0x8c055d2au;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[6]);goto L8c055d2c; /* cmp/gt r6,r0 */
L8c055d2c: if((*sr&1u))goto L8c055d38;goto L8c055d2e; /* bt 0x8c055d38 */
L8c055d2e: *stop=0x8c055d2eu;r[2]=0u-r[6];goto L8c055d30; /* neg r6,r2 */
L8c055d30: *stop=0x8c055d30u;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[2]);goto L8c055d32; /* cmp/ge r2,r0 */
L8c055d32: if(!(*sr&1u))goto L8c055d38;goto L8c055d34; /* bf 0x8c055d38 */
L8c055d34: *stop=0x8c055d34u;if(*pr!=entry_pr)return -3;*stop=0x8c055d36u;r[0]=r[5];return 0; /* rts */
L8c055d38: *stop=0x8c055d38u;{uint32_t loaded=rd(m,bus,&io_status,0x8c055e20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c055d3a; /* mov.l 0x8c055e20,r2 */
L8c055d3a: *stop=0x8c055d3au;*sr=(*sr&~1u)|!(r[0]&r[2]);goto L8c055d3c; /* tst r0,r2 */
L8c055d3c: if((*sr&1u))goto L8c055d44;goto L8c055d3e; /* bt 0x8c055d44 */
L8c055d3e: *stop=0x8c055d3eu;r[5]=r[4];goto L8c055d40; /* mov r4,r5 */
L8c055d40: *stop=0x8c055d42u;r[5]-=r[6];goto L8c055d48; /* bra 0x8c055d48 */
L8c055d44: *stop=0x8c055d44u;r[5]=r[4];goto L8c055d46; /* mov r4,r5 */
L8c055d46: *stop=0x8c055d46u;r[5]+=r[6];goto L8c055d48; /* add r6,r5 */
L8c055d48: *stop=0x8c055d48u;r[0]=r[5]&65535u;goto L8c055d4a; /* extu.w r5,r0 */
L8c055d4a: *stop=0x8c055d4au;if(*pr!=entry_pr)return -3;*stop=0x8c055d4cu;return 0; /* rts */
}
