/* Original complete collision-contact caller.
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
static int sa_collision_contact_run_8c029b00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c029b3au)goto L8c029b3a;if(resume_pc==0x8c029c76u)goto L8c029c76;if(resume_pc==0x8c029c84u)goto L8c029c84;if(resume_pc==0x8c029c8cu)goto L8c029c8c;if(resume_pc==0x8c029d08u)goto L8c029d08;if(resume_pc==0x8c029d10u)goto L8c029d10;if(resume_pc==0x8c029d24u)goto L8c029d24;if(resume_pc==0x8c029d32u)goto L8c029d32;if(resume_pc==0x8c029d3eu)goto L8c029d3e;if(resume_pc==0x8c029d56u)goto L8c029d56;if(resume_pc==0x8c029d64u)goto L8c029d64;if(resume_pc==0x8c029d6cu)goto L8c029d6c;if(resume_pc==0x8c029d74u)goto L8c029d74;if(resume_pc==0x8c029d80u)goto L8c029d80;if(resume_pc==0x8c029d8cu)goto L8c029d8c;if(resume_pc==0x8c029dccu)goto L8c029dcc;if(resume_pc==0x8c029e48u)goto L8c029e48;if(resume_pc==0x8c02a0ccu)goto L8c02a0cc;if(resume_pc==0x8c02a0d2u)goto L8c02a0d2;if(resume_pc==0x8c02a0dcu)goto L8c02a0dc;if(resume_pc==0x8c02a0e2u)goto L8c02a0e2;if(resume_pc==0x8c02a19cu)goto L8c02a19c;if(resume_pc==0x8c02a1aeu)goto L8c02a1ae;if(resume_pc==0x8c02a1bcu)goto L8c02a1bc;if(resume_pc==0x8c02a1dcu)goto L8c02a1dc;if(resume_pc==0x8c02a21cu)goto L8c02a21c;if(resume_pc==0x8c02a2eeu)goto L8c02a2ee;if(resume_pc==0x8c02a31eu)goto L8c02a31e;if(resume_pc==0x8c02a384u)goto L8c02a384;if(resume_pc==0x8c02a3aau)goto L8c02a3aa;if(resume_pc==0x8c02a406u)goto L8c02a406;if(resume_pc==0x8c02a49cu)goto L8c02a49c;goto L8c029b00;
L8c029b00: *stop=0x8c029b00u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c029b02; /* mov.l r14,@-r15 */
L8c029b02: *stop=0x8c029b02u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c029b04; /* mov.l r13,@-r15 */
L8c029b04: *stop=0x8c029b04u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c029b06; /* mov.l r12,@-r15 */
L8c029b06: *stop=0x8c029b06u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c029b08; /* mov.l r11,@-r15 */
L8c029b08: *stop=0x8c029b08u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c029b0a; /* mov.l r10,@-r15 */
L8c029b0a: *stop=0x8c029b0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c029b0c; /* mov.l r9,@-r15 */
L8c029b0c: *stop=0x8c029b0cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c029b0e; /* mov.l r8,@-r15 */
L8c029b0e: *stop=0x8c029b0eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c029b10; /* fmov.s fr15,@-r15 */
L8c029b10: *stop=0x8c029b10u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c029b12; /* fmov.s fr14,@-r15 */
L8c029b12: *stop=0x8c029b12u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c029b14; /* fmov.s fr13,@-r15 */
L8c029b14: *stop=0x8c029b14u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c029b16; /* fmov.s fr12,@-r15 */
L8c029b16: *stop=0x8c029b16u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c029b18; /* sts.l PR,@-r15 */
L8c029b18: *stop=0x8c029b18u;r[0]=sx(m,bus,&io_status,0x8c029b80u,2);if(io_status)return io_status;goto L8c029b1a; /* mov.w 0x8c029b80,r0 */
L8c029b1a: *stop=0x8c029b1au;r[15]+=r[0];goto L8c029b1c; /* add r0,r15 */
L8c029b1c: *stop=0x8c029b1cu;r[13]=r[4];goto L8c029b1e; /* mov r4,r13 */
L8c029b1e: *stop=0x8c029b1eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c029b20; /* fldi0 fr3 */
L8c029b20: *stop=0x8c029b20u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c029b22; /* mov.l r5,@r15 */
L8c029b22: *stop=0x8c029b22u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c029b24; /* fmov.s @r13,fr2 */
L8c029b24: *stop=0x8c029b24u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c029b26; /* fcmp/gt fr3,fr2 */
L8c029b26: if((*sr&1u))goto L8c029b2c;goto L8c029b28; /* bt 0x8c029b2c */
L8c029b28: *stop=0x8c029b2au;goto L8c02a41e; /* bra 0x8c02a41e */
L8c029b2c: *stop=0x8c029b2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c029b84u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029b2e; /* mov.l 0x8c029b84,r3 */
L8c029b2e: *stop=0x8c029b2eu;r[2]=0u;goto L8c029b30; /* mov #0x0,r2 */
L8c029b30: *stop=0x8c029b30u;r[4]=r[13];goto L8c029b32; /* mov r13,r4 */
L8c029b32: *stop=0x8c029b32u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c029b34; /* mov.l r2,@r3 */
L8c029b34: *stop=0x8c029b34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c029b88u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c029b36; /* mov.l 0x8c029b88,r2 */
L8c029b36: target=r[2];*pr=0x8c029b3au;*stop=0x8c029b38u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029b3a; /* jsr @r2 */
L8c029b3a: *stop=0x8c029b3au;r[0]=28u;goto L8c029b3c; /* mov #0x1c,r0 */
L8c029b3c: *stop=0x8c029b3cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c029b90u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029b3e; /* mov.l 0x8c029b90,r3 */
L8c029b3e: *stop=0x8c029b3eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[0]+r[15]);if(io_status)return io_status;goto L8c029b40; /* fmov fr0,@(r0,r15) */
L8c029b40: *stop=0x8c029b40u;r[0]=0x8c029b8cu;goto L8c029b42; /* mova 0x8c029b8c,r0 */
L8c029b42: *stop=0x8c029b42u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c029b44; /* fmov.s @r0,fr3 */
L8c029b44: *stop=0x8c029b44u;r[5]=0u;goto L8c029b46; /* mov #0x0,r5 */
L8c029b46: *stop=0x8c029b46u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[13]);if(io_status)return io_status;goto L8c029b48; /* fmov.s @r13,fr13 */
L8c029b48: *stop=0x8c029b48u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;goto L8c029b4a; /* mov.l r3,@(0x10,r15) */
L8c029b4a: *stop=0x8c029b4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c029b94u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029b4c; /* mov.l 0x8c029b94,r3 */
L8c029b4c: *stop=0x8c029b4cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[3],&f->fpscr,&fr[13]);if(status)return status;}goto L8c029b4e; /* fmul fr3,fr13 */
L8c029b4e: *stop=0x8c029b4eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c029b50; /* mov.l @r15,r6 */
L8c029b50: *stop=0x8c029b50u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c029b52; /* mov.l @r3,r4 */
L8c029b52: *stop=0x8c029b52u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c029b54; /* tst r4,r4 */
L8c029b54: take=(*sr&1u);*stop=0x8c029b56u;r[10]=1u;if(take)goto L8c029ba6;goto L8c029b58; /* bt/s 0x8c029ba6 */
L8c029b58: *stop=0x8c029b58u;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029b5a; /* mov.l @(0x8,r4),r3 */
L8c029b5a: *stop=0x8c029b5au;*sr=(*sr&~1u)|(r[6]==r[3]);goto L8c029b5c; /* cmp/eq r6,r3 */
L8c029b5c: if(!(*sr&1u))goto L8c029ba0;goto L8c029b5e; /* bf 0x8c029ba0 */
L8c029b5e: *stop=0x8c029b60u;r[5]=r[10];goto L8c029ba6; /* bra 0x8c029ba6 */
L8c029ba0: *stop=0x8c029ba0u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c029ba2; /* mov.l @r4,r4 */
L8c029ba2: *stop=0x8c029ba2u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c029ba4; /* tst r4,r4 */
L8c029ba4: if(!(*sr&1u))goto L8c029b58;goto L8c029ba6; /* bf 0x8c029b58 */
L8c029ba6: *stop=0x8c029ba6u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c029ba8; /* tst r5,r5 */
L8c029ba8: if(!(*sr&1u))goto L8c029bae;goto L8c029baa; /* bf 0x8c029bae */
L8c029baa: *stop=0x8c029bacu;goto L8c02a41e; /* bra 0x8c02a41e */
L8c029bae: *stop=0x8c029baeu;if(!valid_access(r[4]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c029bb0; /* mov.l @(0x24,r4),r2 */
L8c029bb0: *stop=0x8c029bb0u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c029bb2; /* mov.l r2,@(0x14,r15) */
L8c029bb2: *stop=0x8c029bb2u;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c029bb4; /* mov.l @(0x1c,r4),r12 */
L8c029bb4: *stop=0x8c029bb4u;r[4]=sx(m,bus,&io_status,0x8c029cc0u,2);if(io_status)return io_status;goto L8c029bb6; /* mov.w 0x8c029cc0,r4 */
L8c029bb6: *stop=0x8c029bb6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c029cccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c029bb8; /* mov.l 0x8c029ccc,r2 */
L8c029bb8: *stop=0x8c029bb8u;r[4]+=r[15];goto L8c029bba; /* add r15,r4 */
L8c029bba: *stop=0x8c029bbau;r[3]=r[4];goto L8c029bbc; /* mov r4,r3 */
L8c029bbc: *stop=0x8c029bbcu;r[8]=r[4];goto L8c029bbe; /* mov r4,r8 */
L8c029bbe: *stop=0x8c029bbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c029cd0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c029bc0; /* mov.l 0x8c029cd0,r4 */
L8c029bc0: *stop=0x8c029bc0u;r[9]=r[3];goto L8c029bc2; /* mov r3,r9 */
L8c029bc2: *stop=0x8c029bc2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c029bc4; /* mov.l r3,@r15 */
L8c029bc4: *stop=0x8c029bc4u;r[8]+=12u;goto L8c029bc6; /* add #0xc,r8 */
L8c029bc6: *stop=0x8c029bc6u;if(!valid_access(r[15]+40u,4u))return -3;wr(m,bus,&io_status,r[15]+0x28u,r[2],4);if(io_status)return io_status;goto L8c029bc8; /* mov.l r2,@(0x28,r15) */
L8c029bc8: *stop=0x8c029bc8u;r[2]=r[4];goto L8c029bca; /* mov r4,r2 */
L8c029bca: *stop=0x8c029bcau;{uint32_t loaded=rd(m,bus,&io_status,0x8c029cd4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029bcc; /* mov.l 0x8c029cd4,r3 */
L8c029bcc: *stop=0x8c029bccu;r[1]=r[2];goto L8c029bce; /* mov r2,r1 */
L8c029bce: *stop=0x8c029bceu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c029bd0; /* mov.l r2,@(0xc,r15) */
L8c029bd0: *stop=0x8c029bd0u;r[4]+=24u;goto L8c029bd2; /* add #0x18,r4 */
L8c029bd2: *stop=0x8c029bd2u;r[2]=r[3];goto L8c029bd4; /* mov r3,r2 */
L8c029bd4: *stop=0x8c029bd4u;r[1]+=12u;goto L8c029bd6; /* add #0xc,r1 */
L8c029bd6: *stop=0x8c029bd6u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c029bd8; /* mov.l r1,@(0x4,r15) */
L8c029bd8: *stop=0x8c029bd8u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c029bda; /* mov.l r4,@(0x8,r15) */
L8c029bda: *stop=0x8c029bdau;if(!valid_access(r[15]+36u,4u))return -3;wr(m,bus,&io_status,r[15]+0x24u,r[3],4);if(io_status)return io_status;goto L8c029bdc; /* mov.l r3,@(0x24,r15) */
L8c029bdc: *stop=0x8c029bdcu;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[2],4);if(io_status)return io_status;goto L8c029bde; /* mov.l r2,@(0x20,r15) */
L8c029bde: *stop=0x8c029bdeu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029be0; /* mov.l @(0x14,r15),r3 */
L8c029be0: *stop=0x8c029be0u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c029be2; /* cmp/pl r3 */
L8c029be2: take=(*sr&1u);*stop=0x8c029be4u;r[9]+=24u;if(take)goto L8c029bea;goto L8c029be6; /* bt/s 0x8c029bea */
L8c029be6: *stop=0x8c029be8u;goto L8c02a416; /* bra 0x8c02a416 */
L8c029bea: *stop=0x8c029beau;r[0]=0x8c029cdcu;goto L8c029bec; /* mova 0x8c029cdc,r0 */
L8c029bec: *stop=0x8c029becu;r[14]=r[15];goto L8c029bee; /* mov r15,r14 */
L8c029bee: *stop=0x8c029beeu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c029bf0; /* fmov.s @r0,fr14 */
L8c029bf0: *stop=0x8c029bf0u;r[14]+=60u;goto L8c029bf2; /* add #0x3c,r14 */
L8c029bf2: *stop=0x8c029bf2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c029cd8u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c029bf4; /* mov.l 0x8c029cd8,r11 */
L8c029bf4: *stop=0x8c029bf4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;goto L8c029bf6; /* fldi0 fr15 */
L8c029bf6: *stop=0x8c029bf6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c029bf8; /* mov.l @r15,r3 */
L8c029bf8: *stop=0x8c029bf8u;r[0]=8u;goto L8c029bfa; /* mov #0x8,r0 */
L8c029bfa: *stop=0x8c029bfau;r[1]=r[3];goto L8c029bfc; /* mov r3,r1 */
L8c029bfc: *stop=0x8c029bfcu;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[1]);if(io_status)return io_status;goto L8c029bfe; /* fmov fr15,@(r0,r1) */
L8c029bfe: *stop=0x8c029bfeu;r[2]=r[3];goto L8c029c00; /* mov r3,r2 */
L8c029c00: *stop=0x8c029c00u;r[0]=4u;goto L8c029c02; /* mov #0x4,r0 */
L8c029c02: *stop=0x8c029c02u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[2]);if(io_status)return io_status;goto L8c029c04; /* fmov fr15,@(r0,r2) */
L8c029c04: *stop=0x8c029c04u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[3]);if(io_status)return io_status;goto L8c029c06; /* fmov.s fr15,@r3 */
L8c029c06: *stop=0x8c029c06u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[5]=loaded;}goto L8c029c08; /* mov.l @r12,r5 */
L8c029c08: *stop=0x8c029c08u;r[5]+=24u;goto L8c029c0a; /* add #0x18,r5 */
L8c029c0a: *stop=0x8c029c0au;r[4]=r[5];goto L8c029c0c; /* mov r5,r4 */
L8c029c0c: *stop=0x8c029c0cu;r[6]=r[5];goto L8c029c0e; /* mov r5,r6 */
L8c029c0e: *stop=0x8c029c0eu;r[4]+=12u;goto L8c029c10; /* add #0xc,r4 */
L8c029c10: *stop=0x8c029c10u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c029c12; /* fmov.s @r6+,fr6 */
L8c029c12: *stop=0x8c029c12u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c029c14; /* fmov.s @r4+,fr9 */
L8c029c14: *stop=0x8c029c14u;r[5]+=24u;goto L8c029c16; /* add #0x18,r5 */
L8c029c16: *stop=0x8c029c16u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c029c18; /* fmov.s @r6+,fr4 */
L8c029c18: *stop=0x8c029c18u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c029c1a; /* fmov.s @r4+,fr7 */
L8c029c1a: *stop=0x8c029c1au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[6],&f->fpscr,&fr[9],1);if(status)return status;}goto L8c029c1c; /* fsub fr6,fr9 */
L8c029c1c: *stop=0x8c029c1cu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[6]);if(io_status)return io_status;goto L8c029c1e; /* fmov.s @r6,fr5 */
L8c029c1e: *stop=0x8c029c1eu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;goto L8c029c20; /* fmov.s @r4,fr8 */
L8c029c20: *stop=0x8c029c20u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[4],&f->fpscr,&fr[7],1);if(status)return status;}goto L8c029c22; /* fsub fr4,fr7 */
L8c029c22: *stop=0x8c029c22u;r[4]=r[8];goto L8c029c24; /* mov r8,r4 */
L8c029c24: *stop=0x8c029c24u;r[4]+=12u;goto L8c029c26; /* add #0xc,r4 */
L8c029c26: *stop=0x8c029c26u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[5],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c029c28; /* fsub fr5,fr8 */
L8c029c28: *stop=0x8c029c28u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;goto L8c029c2a; /* fmov.s fr8,@-r4 */
L8c029c2a: *stop=0x8c029c2au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c029c2c; /* fmov.s fr7,@-r4 */
L8c029c2c: *stop=0x8c029c2cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c029c2e; /* fmov.s fr9,@-r4 */
L8c029c2e: *stop=0x8c029c2eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c029c30; /* fmov.s @r5+,fr8 */
L8c029c30: *stop=0x8c029c30u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[6],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c029c32; /* fsub fr6,fr8 */
L8c029c32: *stop=0x8c029c32u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c029c34; /* fmov.s @r5+,fr7 */
L8c029c34: *stop=0x8c029c34u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;goto L8c029c36; /* fmov.s @r5,fr9 */
L8c029c36: *stop=0x8c029c36u;r[3]=sx(m,bus,&io_status,0x8c029cc8u,2);if(io_status)return io_status;goto L8c029c38; /* mov.w 0x8c029cc8,r3 */
L8c029c38: *stop=0x8c029c38u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[5],&f->fpscr,&fr[9],1);if(status)return status;}goto L8c029c3a; /* fsub fr5,fr9 */
L8c029c3a: *stop=0x8c029c3au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[4],&f->fpscr,&fr[7],1);if(status)return status;}goto L8c029c3c; /* fsub fr4,fr7 */
L8c029c3c: *stop=0x8c029c3cu;r[4]=r[9];goto L8c029c3e; /* mov r9,r4 */
L8c029c3e: *stop=0x8c029c3eu;r[3]+=r[15];goto L8c029c40; /* add r15,r3 */
L8c029c40: *stop=0x8c029c40u;r[4]+=12u;goto L8c029c42; /* add #0xc,r4 */
L8c029c42: *stop=0x8c029c42u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c029c44; /* fmov.s fr9,@-r4 */
L8c029c44: *stop=0x8c029c44u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c029c46; /* fmov.s fr7,@-r4 */
L8c029c46: *stop=0x8c029c46u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;goto L8c029c48; /* fmov.s fr8,@-r4 */
L8c029c48: *stop=0x8c029c48u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c029c4a; /* fmov.s @(r0,r13),fr3 */
L8c029c4a: *stop=0x8c029c4au;r[0]=sx(m,bus,&io_status,0x8c029cc2u,2);if(io_status)return io_status;goto L8c029c4c; /* mov.w 0x8c029cc2,r0 */
L8c029c4c: *stop=0x8c029c4cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c029c4e; /* fsub fr6,fr3 */
L8c029c4e: *stop=0x8c029c4eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029c50; /* fmov fr3,@(r0,r15) */
L8c029c50: *stop=0x8c029c50u;r[0]=8u;goto L8c029c52; /* mov #0x8,r0 */
L8c029c52: *stop=0x8c029c52u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c029c54; /* fmov.s @(r0,r13),fr3 */
L8c029c54: *stop=0x8c029c54u;r[0]=sx(m,bus,&io_status,0x8c029cc4u,2);if(io_status)return io_status;goto L8c029c56; /* mov.w 0x8c029cc4,r0 */
L8c029c56: *stop=0x8c029c56u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c029c58; /* fsub fr4,fr3 */
L8c029c58: *stop=0x8c029c58u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029c5a; /* fmov fr3,@(r0,r15) */
L8c029c5a: *stop=0x8c029c5au;r[0]=12u;goto L8c029c5c; /* mov #0xc,r0 */
L8c029c5c: *stop=0x8c029c5cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c029c5e; /* fmov.s @(r0,r13),fr3 */
L8c029c5e: *stop=0x8c029c5eu;r[0]=sx(m,bus,&io_status,0x8c029cc6u,2);if(io_status)return io_status;goto L8c029c60; /* mov.w 0x8c029cc6,r0 */
L8c029c60: *stop=0x8c029c60u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c029c62; /* fsub fr5,fr3 */
L8c029c62: *stop=0x8c029c62u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029c64; /* fmov fr3,@(r0,r15) */
L8c029c64: *stop=0x8c029c64u;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c029c66; /* mov.l @(0x10,r13),r2 */
L8c029c66: *stop=0x8c029c66u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c029c68; /* mov.l r2,@r3 */
L8c029c68: *stop=0x8c029c68u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c029c6a; /* mov.l @(0x14,r13),r2 */
L8c029c6a: *stop=0x8c029c6au;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[2],4);if(io_status)return io_status;goto L8c029c6c; /* mov.l r2,@(0x4,r3) */
L8c029c6c: *stop=0x8c029c6cu;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c029c6e; /* mov.l @(0x18,r13),r2 */
L8c029c6e: *stop=0x8c029c6eu;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[2],4);if(io_status)return io_status;goto L8c029c70; /* mov.l r2,@(0x8,r3) */
L8c029c70: *stop=0x8c029c70u;{uint32_t loaded=rd(m,bus,&io_status,0x8c029ce0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029c72; /* mov.l 0x8c029ce0,r3 */
L8c029c72: target=r[3];*pr=0x8c029c76u;*stop=0x8c029c74u;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029c76; /* jsr @r3 */
L8c029c76: *stop=0x8c029c76u;r[7]=sx(m,bus,&io_status,0x8c029ccau,2);if(io_status)return io_status;goto L8c029c78; /* mov.w 0x8c029cca,r7 */
L8c029c78: *stop=0x8c029c78u;r[5]=r[8];goto L8c029c7a; /* mov r8,r5 */
L8c029c7a: *stop=0x8c029c7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c029ce4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029c7c; /* mov.l 0x8c029ce4,r3 */
L8c029c7c: *stop=0x8c029c7cu;r[6]=r[9];goto L8c029c7e; /* mov r9,r6 */
L8c029c7e: *stop=0x8c029c7eu;r[7]+=r[15];goto L8c029c80; /* add r15,r7 */
L8c029c80: target=r[3];*pr=0x8c029c84u;*stop=0x8c029c82u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029c84; /* jsr @r3 */
L8c029c84: *stop=0x8c029c84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c029ce8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c029c86; /* mov.l 0x8c029ce8,r2 */
L8c029c86: *stop=0x8c029c86u;r[4]=sx(m,bus,&io_status,0x8c029ccau,2);if(io_status)return io_status;goto L8c029c88; /* mov.w 0x8c029cca,r4 */
L8c029c88: target=r[2];*pr=0x8c029c8cu;*stop=0x8c029c8au;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029c8c; /* jsr @r2 */
L8c029c8c: *stop=0x8c029c8cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c029c8e; /* fldi0 fr3 */
L8c029c8e: *stop=0x8c029c8eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c029c90; /* fcmp/eq fr3,fr0 */
L8c029c90: if(!(*sr&1u))goto L8c029d00;goto L8c029c92; /* bf 0x8c029d00 */
L8c029c92: *stop=0x8c029c92u;r[3]=sx(m,bus,&io_status,0x8c029ccau,2);if(io_status)return io_status;goto L8c029c94; /* mov.w 0x8c029cca,r3 */
L8c029c94: *stop=0x8c029c94u;r[0]=4u;goto L8c029c96; /* mov #0x4,r0 */
L8c029c96: *stop=0x8c029c96u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c029c98; /* fldi1 fr2 */
L8c029c98: *stop=0x8c029c98u;r[3]+=r[15];goto L8c029c9a; /* add r15,r3 */
L8c029c9a: *stop=0x8c029c9au;r[2]=sx(m,bus,&io_status,0x8c029ccau,2);if(io_status)return io_status;goto L8c029c9c; /* mov.w 0x8c029cca,r2 */
L8c029c9c: *stop=0x8c029c9cu;r[2]+=r[15];goto L8c029c9e; /* add r15,r2 */
L8c029c9e: *stop=0x8c029c9eu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[2]);if(io_status)return io_status;goto L8c029ca0; /* fmov.s fr15,@r2 */
L8c029ca0: *stop=0x8c029ca0u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c029ca2; /* fmov fr2,@(r0,r3) */
L8c029ca2: *stop=0x8c029ca2u;r[0]=8u;goto L8c029ca4; /* mov #0x8,r0 */
L8c029ca4: *stop=0x8c029ca4u;r[3]=sx(m,bus,&io_status,0x8c029ccau,2);if(io_status)return io_status;goto L8c029ca6; /* mov.w 0x8c029cca,r3 */
L8c029ca6: *stop=0x8c029ca6u;r[3]+=r[15];goto L8c029ca8; /* add r15,r3 */
L8c029ca8: *stop=0x8c029ca8u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[3]);if(io_status)return io_status;goto L8c029caa; /* fmov fr15,@(r0,r3) */
L8c029caa: *stop=0x8c029cacu;goto L8c029d08; /* bra 0x8c029d08 */
L8c029d00: *stop=0x8c029d00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c029e10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c029d02; /* mov.l 0x8c029e10,r2 */
L8c029d02: *stop=0x8c029d02u;r[4]=sx(m,bus,&io_status,0x8c029e00u,2);if(io_status)return io_status;goto L8c029d04; /* mov.w 0x8c029e00,r4 */
L8c029d04: target=r[2];*pr=0x8c029d08u;*stop=0x8c029d06u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029d08; /* jsr @r2 */
L8c029d08: *stop=0x8c029d08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c029e14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029d0a; /* mov.l 0x8c029e14,r3 */
L8c029d0a: *stop=0x8c029d0au;r[0]=sx(m,bus,&io_status,0x8c029e02u,2);if(io_status)return io_status;goto L8c029d0c; /* mov.w 0x8c029e02,r0 */
L8c029d0c: target=r[3];*pr=0x8c029d10u;*stop=0x8c029d0eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029d10; /* jsr @r3 */
L8c029d10: *stop=0x8c029d10u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c029d12; /* fmul fr14,fr0 */
L8c029d12: *stop=0x8c029d12u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c029d14; /* ftrc fr0,FPUL */
L8c029d14: *stop=0x8c029d14u;r[2]=*fpul;goto L8c029d16; /* sts FPUL,r2 */
L8c029d16: *stop=0x8c029d16u;r[2]=0u-r[2];goto L8c029d18; /* neg r2,r2 */
L8c029d18: *stop=0x8c029d18u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c029d1a; /* tst r2,r2 */
L8c029d1a: if((*sr&1u))goto L8c029d32;goto L8c029d1c; /* bt 0x8c029d32 */
L8c029d1c: *stop=0x8c029d1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c029e14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029d1e; /* mov.l 0x8c029e14,r3 */
L8c029d1e: *stop=0x8c029d1eu;r[0]=sx(m,bus,&io_status,0x8c029e02u,2);if(io_status)return io_status;goto L8c029d20; /* mov.w 0x8c029e02,r0 */
L8c029d20: target=r[3];*pr=0x8c029d24u;*stop=0x8c029d22u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029d24; /* jsr @r3 */
L8c029d24: *stop=0x8c029d24u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c029d26; /* fmul fr14,fr0 */
L8c029d26: *stop=0x8c029d26u;{uint32_t loaded=rd(m,bus,&io_status,0x8c029e18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029d28; /* mov.l 0x8c029e18,r3 */
L8c029d28: *stop=0x8c029d28u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c029d2a; /* ftrc fr0,FPUL */
L8c029d2a: *stop=0x8c029d2au;r[2]=*fpul;goto L8c029d2c; /* sts FPUL,r2 */
L8c029d2c: *stop=0x8c029d2cu;r[5]=0u-r[2];goto L8c029d2e; /* neg r2,r5 */
L8c029d2e: target=r[3];*pr=0x8c029d32u;*stop=0x8c029d30u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029d32; /* jsr @r3 */
L8c029d32: *stop=0x8c029d32u;r[0]=sx(m,bus,&io_status,0x8c029e04u,2);if(io_status)return io_status;goto L8c029d34; /* mov.w 0x8c029e04,r0 */
L8c029d34: *stop=0x8c029d34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c029e1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029d36; /* mov.l 0x8c029e1c,r3 */
L8c029d36: *stop=0x8c029d36u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[15]);if(io_status)return io_status;goto L8c029d38; /* fmov.s @(r0,r15),fr5 */
L8c029d38: *stop=0x8c029d38u;r[0]=sx(m,bus,&io_status,0x8c029e00u,2);if(io_status)return io_status;goto L8c029d3a; /* mov.w 0x8c029e00,r0 */
L8c029d3a: target=r[3];*pr=0x8c029d3eu;*stop=0x8c029d3cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029d3e; /* jsr @r3 */
L8c029d3e: *stop=0x8c029d3eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c029d40; /* fmul fr14,fr0 */
L8c029d40: *stop=0x8c029d40u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c029d42; /* ftrc fr0,FPUL */
L8c029d42: *stop=0x8c029d42u;r[2]=*fpul;goto L8c029d44; /* sts FPUL,r2 */
L8c029d44: *stop=0x8c029d44u;r[2]=0u-r[2];goto L8c029d46; /* neg r2,r2 */
L8c029d46: *stop=0x8c029d46u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c029d48; /* tst r2,r2 */
L8c029d48: if((*sr&1u))goto L8c029d64;goto L8c029d4a; /* bt 0x8c029d64 */
L8c029d4a: *stop=0x8c029d4au;r[0]=sx(m,bus,&io_status,0x8c029e04u,2);if(io_status)return io_status;goto L8c029d4c; /* mov.w 0x8c029e04,r0 */
L8c029d4c: *stop=0x8c029d4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c029e1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029d4e; /* mov.l 0x8c029e1c,r3 */
L8c029d4e: *stop=0x8c029d4eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[15]);if(io_status)return io_status;goto L8c029d50; /* fmov.s @(r0,r15),fr5 */
L8c029d50: *stop=0x8c029d50u;r[0]=sx(m,bus,&io_status,0x8c029e00u,2);if(io_status)return io_status;goto L8c029d52; /* mov.w 0x8c029e00,r0 */
L8c029d52: target=r[3];*pr=0x8c029d56u;*stop=0x8c029d54u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029d56; /* jsr @r3 */
L8c029d56: *stop=0x8c029d56u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c029d58; /* fmul fr14,fr0 */
L8c029d58: *stop=0x8c029d58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c029e20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029d5a; /* mov.l 0x8c029e20,r3 */
L8c029d5a: *stop=0x8c029d5au;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c029d5c; /* ftrc fr0,FPUL */
L8c029d5c: *stop=0x8c029d5cu;r[2]=*fpul;goto L8c029d5e; /* sts FPUL,r2 */
L8c029d5e: *stop=0x8c029d5eu;r[5]=0u-r[2];goto L8c029d60; /* neg r2,r5 */
L8c029d60: target=r[3];*pr=0x8c029d64u;*stop=0x8c029d62u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029d64; /* jsr @r3 */
L8c029d64: *stop=0x8c029d64u;r[5]=r[8];goto L8c029d66; /* mov r8,r5 */
L8c029d66: *stop=0x8c029d66u;r[6]=r[8];goto L8c029d68; /* mov r8,r6 */
L8c029d68: target=r[11];*pr=0x8c029d6cu;*stop=0x8c029d6au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029d6c; /* jsr @r11 */
L8c029d6c: *stop=0x8c029d6cu;r[5]=r[9];goto L8c029d6e; /* mov r9,r5 */
L8c029d6e: *stop=0x8c029d6eu;r[6]=r[9];goto L8c029d70; /* mov r9,r6 */
L8c029d70: target=r[11];*pr=0x8c029d74u;*stop=0x8c029d72u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029d74; /* jsr @r11 */
L8c029d74: *stop=0x8c029d74u;r[5]=sx(m,bus,&io_status,0x8c029e06u,2);if(io_status)return io_status;goto L8c029d76; /* mov.w 0x8c029e06,r5 */
L8c029d76: *stop=0x8c029d76u;r[6]=sx(m,bus,&io_status,0x8c029e06u,2);if(io_status)return io_status;goto L8c029d78; /* mov.w 0x8c029e06,r6 */
L8c029d78: *stop=0x8c029d78u;r[5]+=r[15];goto L8c029d7a; /* add r15,r5 */
L8c029d7a: *stop=0x8c029d7au;r[6]+=r[15];goto L8c029d7c; /* add r15,r6 */
L8c029d7c: target=r[11];*pr=0x8c029d80u;*stop=0x8c029d7eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029d80; /* jsr @r11 */
L8c029d80: *stop=0x8c029d80u;r[5]=sx(m,bus,&io_status,0x8c029e08u,2);if(io_status)return io_status;goto L8c029d82; /* mov.w 0x8c029e08,r5 */
L8c029d82: *stop=0x8c029d82u;r[6]=sx(m,bus,&io_status,0x8c029e08u,2);if(io_status)return io_status;goto L8c029d84; /* mov.w 0x8c029e08,r6 */
L8c029d84: *stop=0x8c029d84u;r[5]+=r[15];goto L8c029d86; /* add r15,r5 */
L8c029d86: *stop=0x8c029d86u;r[6]+=r[15];goto L8c029d88; /* add r15,r6 */
L8c029d88: target=r[11];*pr=0x8c029d8cu;*stop=0x8c029d8au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029d8c; /* jsr @r11 */
L8c029d8c: *stop=0x8c029d8cu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c029d8e; /* mov.l @r12,r3 */
L8c029d8e: *stop=0x8c029d8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c029e24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c029d90; /* mov.l 0x8c029e24,r2 */
L8c029d90: *stop=0x8c029d90u;if(!valid_access(r[3]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x3cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c029d92; /* mov.l @(0x3c,r3),r1 */
L8c029d92: *stop=0x8c029d92u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[13]);if(io_status)return io_status;goto L8c029d94; /* fmov.s @r13,fr12 */
L8c029d94: *stop=0x8c029d94u;r[1]&=r[2];goto L8c029d96; /* and r2,r1 */
L8c029d96: *stop=0x8c029d96u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c029d98; /* tst r1,r1 */
L8c029d98: take=(*sr&1u);*stop=0x8c029d9au;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[1],4);if(io_status)return io_status;if(take)goto L8c029dac;goto L8c029d9c; /* bt/s 0x8c029dac */
L8c029d9c: *stop=0x8c029d9cu;r[0]=sx(m,bus,&io_status,0x8c029e0au,2);if(io_status)return io_status;goto L8c029d9e; /* mov.w 0x8c029e0a,r0 */
L8c029d9e: *stop=0x8c029d9eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c029da0; /* fldi0 fr3 */
L8c029da0: *stop=0x8c029da0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c029da2; /* fmov.s @(r0,r15),fr2 */
L8c029da2: *stop=0x8c029da2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c029da4; /* fcmp/gt fr2,fr3 */
L8c029da4: if(!(*sr&1u))goto L8c029dac;goto L8c029da6; /* bf 0x8c029dac */
L8c029da6: *stop=0x8c029da6u;r[0]=0x8c029e28u;goto L8c029da8; /* mova 0x8c029e28,r0 */
L8c029da8: *stop=0x8c029da8u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c029daa; /* fmov.s @r0,fr2 */
L8c029daa: *stop=0x8c029daau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[2],&f->fpscr,&fr[12]);if(status)return status;}goto L8c029dac; /* fmul fr2,fr12 */
L8c029dac: *stop=0x8c029dacu;r[3]=sx(m,bus,&io_status,0x8c029e08u,2);if(io_status)return io_status;goto L8c029dae; /* mov.w 0x8c029e08,r3 */
L8c029dae: *stop=0x8c029daeu;r[2]=0u;goto L8c029db0; /* mov #0x0,r2 */
L8c029db0: *stop=0x8c029db0u;r[1]=sx(m,bus,&io_status,0x8c029e0cu,2);if(io_status)return io_status;goto L8c029db2; /* mov.w 0x8c029e0c,r1 */
L8c029db2: *stop=0x8c029db2u;r[3]+=r[15];goto L8c029db4; /* add r15,r3 */
L8c029db4: *stop=0x8c029db4u;if(!valid_access(r[15]+44u,4u))return -3;wr(m,bus,&io_status,r[15]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c029db6; /* mov.l r2,@(0x2c,r15) */
L8c029db6: *stop=0x8c029db6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c029db8; /* mov.l @r3,r2 */
L8c029db8: *stop=0x8c029db8u;r[1]+=r[15];goto L8c029dba; /* add r15,r1 */
L8c029dba: *stop=0x8c029dbau;r[4]=sx(m,bus,&io_status,0x8c029e0cu,2);if(io_status)return io_status;goto L8c029dbc; /* mov.w 0x8c029e0c,r4 */
L8c029dbc: *stop=0x8c029dbcu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c029dbe; /* mov.l r2,@r1 */
L8c029dbe: *stop=0x8c029dbeu;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c029dc0; /* mov.l @(0x4,r3),r2 */
L8c029dc0: *stop=0x8c029dc0u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[2],4);if(io_status)return io_status;goto L8c029dc2; /* mov.l r2,@(0x4,r1) */
L8c029dc2: *stop=0x8c029dc2u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c029dc4; /* mov.l @(0x8,r3),r2 */
L8c029dc4: *stop=0x8c029dc4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c029e2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c029dc6; /* mov.l 0x8c029e2c,r3 */
L8c029dc6: *stop=0x8c029dc6u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[2],4);if(io_status)return io_status;goto L8c029dc8; /* mov.l r2,@(0x8,r1) */
L8c029dc8: target=r[3];*pr=0x8c029dccu;*stop=0x8c029dcau;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029dcc; /* jsr @r3 */
L8c029dcc: *stop=0x8c029dccu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c029dce; /* fldi0 fr3 */
L8c029dce: *stop=0x8c029dceu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c029dd0; /* fcmp/eq fr3,fr0 */
L8c029dd0: if(!(*sr&1u))goto L8c029e40;goto L8c029dd2; /* bf 0x8c029e40 */
L8c029dd2: *stop=0x8c029dd2u;r[1]=sx(m,bus,&io_status,0x8c029e0cu,2);if(io_status)return io_status;goto L8c029dd4; /* mov.w 0x8c029e0c,r1 */
L8c029dd4: *stop=0x8c029dd4u;r[0]=8u;goto L8c029dd6; /* mov #0x8,r0 */
L8c029dd6: *stop=0x8c029dd6u;r[3]=sx(m,bus,&io_status,0x8c029e0cu,2);if(io_status)return io_status;goto L8c029dd8; /* mov.w 0x8c029e0c,r3 */
L8c029dd8: *stop=0x8c029dd8u;r[1]+=r[15];goto L8c029dda; /* add r15,r1 */
L8c029dda: *stop=0x8c029ddau;r[2]=sx(m,bus,&io_status,0x8c029e0cu,2);if(io_status)return io_status;goto L8c029ddc; /* mov.w 0x8c029e0c,r2 */
L8c029ddc: *stop=0x8c029ddcu;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[1]);if(io_status)return io_status;goto L8c029dde; /* fmov fr15,@(r0,r1) */
L8c029dde: *stop=0x8c029ddeu;r[0]=4u;goto L8c029de0; /* mov #0x4,r0 */
L8c029de0: *stop=0x8c029de0u;r[3]+=r[15];goto L8c029de2; /* add r15,r3 */
L8c029de2: *stop=0x8c029de2u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[3]);if(io_status)return io_status;goto L8c029de4; /* fmov fr15,@(r0,r3) */
L8c029de4: *stop=0x8c029de4u;r[2]+=r[15];goto L8c029de6; /* add r15,r2 */
L8c029de6: *stop=0x8c029de8u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[2]);if(io_status)return io_status;goto L8c029e48; /* bra 0x8c029e48 */
L8c029e40: *stop=0x8c029e40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c029f98u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c029e42; /* mov.l 0x8c029f98,r2 */
L8c029e42: *stop=0x8c029e42u;r[4]=sx(m,bus,&io_status,0x8c029f80u,2);if(io_status)return io_status;goto L8c029e44; /* mov.w 0x8c029f80,r4 */
L8c029e44: target=r[2];*pr=0x8c029e48u;*stop=0x8c029e46u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c029e48; /* jsr @r2 */
L8c029e48: *stop=0x8c029e48u;r[0]=sx(m,bus,&io_status,0x8c029f80u,2);if(io_status)return io_status;goto L8c029e4a; /* mov.w 0x8c029f80,r0 */
L8c029e4a: *stop=0x8c029e4au;r[3]=sx(m,bus,&io_status,0x8c029f86u,2);if(io_status)return io_status;goto L8c029e4c; /* mov.w 0x8c029f86,r3 */
L8c029e4c: *stop=0x8c029e4cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029e4e; /* fmov.s @(r0,r15),fr3 */
L8c029e4e: *stop=0x8c029e4eu;r[0]=sx(m,bus,&io_status,0x8c029f80u,2);if(io_status)return io_status;goto L8c029e50; /* mov.w 0x8c029f80,r0 */
L8c029e50: *stop=0x8c029e50u;r[3]+=r[15];goto L8c029e52; /* add r15,r3 */
L8c029e52: *stop=0x8c029e52u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[13],&f->fpscr,&fr[3]);if(status)return status;}goto L8c029e54; /* fmul fr13,fr3 */
L8c029e54: *stop=0x8c029e54u;r[2]=sx(m,bus,&io_status,0x8c029f88u,2);if(io_status)return io_status;goto L8c029e56; /* mov.w 0x8c029f88,r2 */
L8c029e56: *stop=0x8c029e56u;fcopy(f,11u,15u);goto L8c029e58; /* fmov fr15,fr11 */
L8c029e58: *stop=0x8c029e58u;r[2]+=r[15];goto L8c029e5a; /* add r15,r2 */
L8c029e5a: *stop=0x8c029e5au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029e5c; /* fmov fr3,@(r0,r15) */
L8c029e5c: *stop=0x8c029e5cu;r[0]=sx(m,bus,&io_status,0x8c029f82u,2);if(io_status)return io_status;goto L8c029e5e; /* mov.w 0x8c029f82,r0 */
L8c029e5e: *stop=0x8c029e5eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029e60; /* fmov.s @(r0,r15),fr3 */
L8c029e60: *stop=0x8c029e60u;r[0]=sx(m,bus,&io_status,0x8c029f82u,2);if(io_status)return io_status;goto L8c029e62; /* mov.w 0x8c029f82,r0 */
L8c029e62: *stop=0x8c029e62u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[13],&f->fpscr,&fr[3]);if(status)return status;}goto L8c029e64; /* fmul fr13,fr3 */
L8c029e64: *stop=0x8c029e64u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029e66; /* fmov fr3,@(r0,r15) */
L8c029e66: *stop=0x8c029e66u;r[0]=sx(m,bus,&io_status,0x8c029f84u,2);if(io_status)return io_status;goto L8c029e68; /* mov.w 0x8c029f84,r0 */
L8c029e68: *stop=0x8c029e68u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029e6a; /* fmov.s @(r0,r15),fr3 */
L8c029e6a: *stop=0x8c029e6au;r[0]=sx(m,bus,&io_status,0x8c029f84u,2);if(io_status)return io_status;goto L8c029e6c; /* mov.w 0x8c029f84,r0 */
L8c029e6c: *stop=0x8c029e6cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[13],&f->fpscr,&fr[3]);if(status)return status;}goto L8c029e6e; /* fmul fr13,fr3 */
L8c029e6e: *stop=0x8c029e6eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029e70; /* fmov fr3,@(r0,r15) */
L8c029e70: *stop=0x8c029e70u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c029e72; /* mov.l @r3,r1 */
L8c029e72: *stop=0x8c029e72u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c029e74; /* mov.l r1,@r2 */
L8c029e74: *stop=0x8c029e74u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c029e76; /* mov.l @(0x4,r3),r1 */
L8c029e76: *stop=0x8c029e76u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c029e78; /* mov.l r1,@(0x4,r2) */
L8c029e78: *stop=0x8c029e78u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c029e7a; /* mov.l @(0x8,r3),r1 */
L8c029e7a: *stop=0x8c029e7au;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c029e7c; /* mov.l r1,@(0x8,r2) */
L8c029e7c: *stop=0x8c029e7cu;r[0]=28u;goto L8c029e7e; /* mov #0x1c,r0 */
L8c029e7e: *stop=0x8c029e7eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029e80; /* fmov.s @(r0,r15),fr3 */
L8c029e80: *stop=0x8c029e80u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,sr,1);if(status)return status;goto L8c029e82; /* fcmp/gt fr11,fr3 */
L8c029e82: take=(*sr&1u);*stop=0x8c029e84u;r[4]=r[10];if(take)goto L8c029ec6;goto L8c029e86; /* bt/s 0x8c029ec6 */
L8c029e86: *stop=0x8c029e86u;r[0]=sx(m,bus,&io_status,0x8c029f8au,2);if(io_status)return io_status;goto L8c029e88; /* mov.w 0x8c029f8a,r0 */
L8c029e88: *stop=0x8c029e88u;r[4]=0u;goto L8c029e8a; /* mov #0x0,r4 */
L8c029e8a: *stop=0x8c029e8au;r[2]=sx(m,bus,&io_status,0x8c029f86u,2);if(io_status)return io_status;goto L8c029e8c; /* mov.w 0x8c029f86,r2 */
L8c029e8c: *stop=0x8c029e8cu;r[3]=sx(m,bus,&io_status,0x8c029f88u,2);if(io_status)return io_status;goto L8c029e8e; /* mov.w 0x8c029f88,r3 */
L8c029e8e: *stop=0x8c029e8eu;r[2]+=r[15];goto L8c029e90; /* add r15,r2 */
L8c029e90: *stop=0x8c029e90u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c029e92; /* mov.l @r2,r1 */
L8c029e92: *stop=0x8c029e92u;r[3]+=r[15];goto L8c029e94; /* add r15,r3 */
L8c029e94: *stop=0x8c029e94u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c029e96; /* mov.l r1,@r3 */
L8c029e96: *stop=0x8c029e96u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c029e98; /* mov.l @(0x4,r2),r1 */
L8c029e98: *stop=0x8c029e98u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c029e9a; /* mov.l r1,@(0x4,r3) */
L8c029e9a: *stop=0x8c029e9au;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c029e9c; /* mov.l @(0x8,r2),r1 */
L8c029e9c: *stop=0x8c029e9cu;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c029e9e; /* mov.l r1,@(0x8,r3) */
L8c029e9e: *stop=0x8c029e9eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c029ea0; /* fmov.s @(r0,r15),fr2 */
L8c029ea0: *stop=0x8c029ea0u;r[0]=sx(m,bus,&io_status,0x8c029f86u,2);if(io_status)return io_status;goto L8c029ea2; /* mov.w 0x8c029f86,r0 */
L8c029ea2: *stop=0x8c029ea2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c029ea4; /* fmov.s @(r0,r15),fr1 */
L8c029ea4: *stop=0x8c029ea4u;r[0]=sx(m,bus,&io_status,0x8c029f88u,2);if(io_status)return io_status;goto L8c029ea6; /* mov.w 0x8c029f88,r0 */
L8c029ea6: *stop=0x8c029ea6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c029ea8; /* fadd fr2,fr1 */
L8c029ea8: *stop=0x8c029ea8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c029eaa; /* fmov fr1,@(r0,r15) */
L8c029eaa: *stop=0x8c029eaau;r[0]=sx(m,bus,&io_status,0x8c029f8cu,2);if(io_status)return io_status;goto L8c029eac; /* mov.w 0x8c029f8c,r0 */
L8c029eac: *stop=0x8c029eacu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c029eae; /* fmov.s @(r0,r15),fr2 */
L8c029eae: *stop=0x8c029eaeu;r[0]=sx(m,bus,&io_status,0x8c029f8eu,2);if(io_status)return io_status;goto L8c029eb0; /* mov.w 0x8c029f8e,r0 */
L8c029eb0: *stop=0x8c029eb0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c029eb2; /* fmov.s @(r0,r15),fr1 */
L8c029eb2: *stop=0x8c029eb2u;r[0]=sx(m,bus,&io_status,0x8c029f90u,2);if(io_status)return io_status;goto L8c029eb4; /* mov.w 0x8c029f90,r0 */
L8c029eb4: *stop=0x8c029eb4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c029eb6; /* fadd fr2,fr1 */
L8c029eb6: *stop=0x8c029eb6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c029eb8; /* fmov fr1,@(r0,r15) */
L8c029eb8: *stop=0x8c029eb8u;r[0]=sx(m,bus,&io_status,0x8c029f92u,2);if(io_status)return io_status;goto L8c029eba; /* mov.w 0x8c029f92,r0 */
L8c029eba: *stop=0x8c029ebau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c029ebc; /* fmov.s @(r0,r15),fr2 */
L8c029ebc: *stop=0x8c029ebcu;r[0]=sx(m,bus,&io_status,0x8c029f94u,2);if(io_status)return io_status;goto L8c029ebe; /* mov.w 0x8c029f94,r0 */
L8c029ebe: *stop=0x8c029ebeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c029ec0; /* fmov.s @(r0,r15),fr1 */
L8c029ec0: *stop=0x8c029ec0u;r[0]=sx(m,bus,&io_status,0x8c029f96u,2);if(io_status)return io_status;goto L8c029ec2; /* mov.w 0x8c029f96,r0 */
L8c029ec2: *stop=0x8c029ec2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c029ec4; /* fadd fr2,fr1 */
L8c029ec4: *stop=0x8c029ec4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c029ec6; /* fmov fr1,@(r0,r15) */
L8c029ec6: *stop=0x8c029ec6u;r[0]=sx(m,bus,&io_status,0x8c029f90u,2);if(io_status)return io_status;goto L8c029ec8; /* mov.w 0x8c029f90,r0 */
L8c029ec8: *stop=0x8c029ec8u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c029eca; /* fmov.s @r13,fr2 */
L8c029eca: *stop=0x8c029ecau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029ecc; /* fmov.s @(r0,r15),fr3 */
L8c029ecc: *stop=0x8c029eccu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]&=0x7fffffffu;goto L8c029ece; /* fabs fr3 */
L8c029ece: *stop=0x8c029eceu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c029ed0; /* fcmp/gt fr2,fr3 */
L8c029ed0: if((*sr&1u))goto L8c029fa0;goto L8c029ed2; /* bt 0x8c029fa0 */
L8c029ed2: *stop=0x8c029ed2u;r[0]=8u;goto L8c029ed4; /* mov #0x8,r0 */
L8c029ed4: *stop=0x8c029ed4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c029ed6; /* mov.l @r15,r3 */
L8c029ed6: *stop=0x8c029ed6u;if(!valid_access(r[0]+r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[8]);if(io_status)return io_status;goto L8c029ed8; /* fmov.s @(r0,r8),fr5 */
L8c029ed8: *stop=0x8c029ed8u;r[0]=8u;goto L8c029eda; /* mov #0x8,r0 */
L8c029eda: *stop=0x8c029edau;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]+r[3]);if(io_status)return io_status;goto L8c029edc; /* fmov.s @(r0,r3),fr7 */
L8c029edc: *stop=0x8c029edcu;r[0]=8u;goto L8c029ede; /* mov #0x8,r0 */
L8c029ede: *stop=0x8c029edeu;r[3]=sx(m,bus,&io_status,0x8c029f88u,2);if(io_status)return io_status;goto L8c029ee0; /* mov.w 0x8c029f88,r3 */
L8c029ee0: *stop=0x8c029ee0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c029ee2; /* mov.l @r15,r2 */
L8c029ee2: *stop=0x8c029ee2u;r[3]+=r[15];goto L8c029ee4; /* add r15,r3 */
L8c029ee4: *stop=0x8c029ee4u;fcopy(f,3u,5u);goto L8c029ee6; /* fmov fr5,fr3 */
L8c029ee6: *stop=0x8c029ee6u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[2]);if(io_status)return io_status;goto L8c029ee8; /* fmov.s @r2,fr6 */
L8c029ee8: *stop=0x8c029ee8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c029eea; /* fsub fr7,fr3 */
L8c029eea: *stop=0x8c029eeau;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c029eec; /* fmov.s @r3,fr2 */
L8c029eec: *stop=0x8c029eecu;r[3]=sx(m,bus,&io_status,0x8c029f88u,2);if(io_status)return io_status;goto L8c029eee; /* mov.w 0x8c029f88,r3 */
L8c029eee: *stop=0x8c029eeeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[6],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c029ef0; /* fsub fr6,fr2 */
L8c029ef0: *stop=0x8c029ef0u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[8]);if(io_status)return io_status;goto L8c029ef2; /* fmov.s @r8,fr9 */
L8c029ef2: *stop=0x8c029ef2u;r[3]+=r[15];goto L8c029ef4; /* add r15,r3 */
L8c029ef4: *stop=0x8c029ef4u;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[9]);if(io_status)return io_status;goto L8c029ef6; /* fmov.s @(r0,r9),fr4 */
L8c029ef6: *stop=0x8c029ef6u;fcopy(f,1u,9u);goto L8c029ef8; /* fmov fr9,fr1 */
L8c029ef8: *stop=0x8c029ef8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[6],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c029efa; /* fsub fr6,fr1 */
L8c029efa: *stop=0x8c029efau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c029efc; /* fmul fr2,fr3 */
L8c029efc: *stop=0x8c029efcu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c029efe; /* fmov.s @(r0,r3),fr2 */
L8c029efe: *stop=0x8c029efeu;r[3]=sx(m,bus,&io_status,0x8c029f88u,2);if(io_status)return io_status;goto L8c029f00; /* mov.w 0x8c029f88,r3 */
L8c029f00: *stop=0x8c029f00u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[7],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c029f02; /* fsub fr7,fr2 */
L8c029f02: *stop=0x8c029f02u;r[3]+=r[15];goto L8c029f04; /* add r15,r3 */
L8c029f04: *stop=0x8c029f04u;fcopy(f,8u,3u);goto L8c029f06; /* fmov fr3,fr8 */
L8c029f06: *stop=0x8c029f06u;fcopy(f,3u,4u);goto L8c029f08; /* fmov fr4,fr3 */
L8c029f08: *stop=0x8c029f08u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c029f0a; /* fsub fr5,fr3 */
L8c029f0a: *stop=0x8c029f0au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c029f0c; /* fmul fr2,fr1 */
L8c029f0c: *stop=0x8c029f0cu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c029f0e; /* fmov.s @r3,fr2 */
L8c029f0e: *stop=0x8c029f0eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[9],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c029f10; /* fsub fr9,fr2 */
L8c029f10: *stop=0x8c029f10u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[1],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c029f12; /* fsub fr1,fr8 */
L8c029f12: *stop=0x8c029f12u;r[3]=sx(m,bus,&io_status,0x8c029f88u,2);if(io_status)return io_status;goto L8c029f14; /* mov.w 0x8c029f88,r3 */
L8c029f14: *stop=0x8c029f14u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c029f16; /* fmul fr2,fr3 */
L8c029f16: *stop=0x8c029f16u;if(!valid_access(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[9]);if(io_status)return io_status;goto L8c029f18; /* fmov.s @r9,fr10 */
L8c029f18: *stop=0x8c029f18u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[4],&f->fpscr,&fr[7],1);if(status)return status;}goto L8c029f1a; /* fsub fr4,fr7 */
L8c029f1a: *stop=0x8c029f1au;r[3]+=r[15];goto L8c029f1c; /* add r15,r3 */
L8c029f1c: *stop=0x8c029f1cu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c029f1e; /* fmov.s @(r0,r3),fr2 */
L8c029f1e: *stop=0x8c029f1eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[10],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c029f20; /* fsub fr10,fr6 */
L8c029f20: *stop=0x8c029f20u;r[3]=sx(m,bus,&io_status,0x8c029f88u,2);if(io_status)return io_status;goto L8c029f22; /* mov.w 0x8c029f88,r3 */
L8c029f22: *stop=0x8c029f22u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c029f24; /* fsub fr5,fr2 */
L8c029f24: *stop=0x8c029f24u;fcopy(f,5u,3u);goto L8c029f26; /* fmov fr3,fr5 */
L8c029f26: *stop=0x8c029f26u;r[3]+=r[15];goto L8c029f28; /* add r15,r3 */
L8c029f28: *stop=0x8c029f28u;fcopy(f,1u,10u);goto L8c029f2a; /* fmov fr10,fr1 */
L8c029f2a: *stop=0x8c029f2au;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c029f2c; /* fmov.s @r3,fr3 */
L8c029f2c: *stop=0x8c029f2cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[9],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c029f2e; /* fsub fr9,fr1 */
L8c029f2e: *stop=0x8c029f2eu;r[3]=sx(m,bus,&io_status,0x8c029f88u,2);if(io_status)return io_status;goto L8c029f30; /* mov.w 0x8c029f88,r3 */
L8c029f30: *stop=0x8c029f30u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[10],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c029f32; /* fsub fr10,fr3 */
L8c029f32: *stop=0x8c029f32u;r[3]+=r[15];goto L8c029f34; /* add r15,r3 */
L8c029f34: *stop=0x8c029f34u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c029f36; /* fmul fr2,fr1 */
L8c029f36: *stop=0x8c029f36u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c029f38; /* fldi0 fr2 */
L8c029f38: *stop=0x8c029f38u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[3],&f->fpscr,&fr[7]);if(status)return status;}goto L8c029f3a; /* fmul fr3,fr7 */
L8c029f3a: *stop=0x8c029f3au;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c029f3c; /* fmov.s @(r0,r3),fr3 */
L8c029f3c: *stop=0x8c029f3cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c029f3e; /* fsub fr4,fr3 */
L8c029f3e: *stop=0x8c029f3eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[1],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c029f40; /* fsub fr1,fr5 */
L8c029f40: *stop=0x8c029f40u;fcopy(f,4u,7u);goto L8c029f42; /* fmov fr7,fr4 */
L8c029f42: *stop=0x8c029f42u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c029f44; /* fmul fr3,fr6 */
L8c029f44: *stop=0x8c029f44u;fcopy(f,3u,8u);goto L8c029f46; /* fmov fr8,fr3 */
L8c029f46: *stop=0x8c029f46u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c029f48; /* fmul fr5,fr3 */
L8c029f48: *stop=0x8c029f48u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c029f4a; /* fcmp/gt fr3,fr2 */
L8c029f4a: take=(*sr&1u);*stop=0x8c029f4cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[6],&f->fpscr,&fr[4],1);if(status)return status;}if(take)goto L8c029fa0;goto L8c029f4e; /* bt/s 0x8c029fa0 */
L8c029f4e: *stop=0x8c029f4eu;fcopy(f,3u,5u);goto L8c029f50; /* fmov fr5,fr3 */
L8c029f50: *stop=0x8c029f50u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c029f52; /* fmul fr4,fr3 */
L8c029f52: *stop=0x8c029f52u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c029f54; /* fcmp/gt fr3,fr2 */
L8c029f54: if((*sr&1u))goto L8c029fa0;goto L8c029f56; /* bt 0x8c029fa0 */
L8c029f56: *stop=0x8c029f56u;fcopy(f,3u,8u);goto L8c029f58; /* fmov fr8,fr3 */
L8c029f58: *stop=0x8c029f58u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c029f5a; /* fmul fr4,fr3 */
L8c029f5a: *stop=0x8c029f5au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c029f5c; /* fldi0 fr2 */
L8c029f5c: *stop=0x8c029f5cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c029f5e; /* fcmp/gt fr3,fr2 */
L8c029f5e: if((*sr&1u))goto L8c029fa0;goto L8c029f60; /* bt 0x8c029fa0 */
L8c029f60: *stop=0x8c029f62u;if(!valid_access(r[15]+44u,4u))return -3;wr(m,bus,&io_status,r[15]+0x2cu,r[10],4);if(io_status)return io_status;goto L8c02a000; /* bra 0x8c02a000 */
L8c029fa0: *stop=0x8c029fa0u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c029fa2; /* tst r4,r4 */
L8c029fa2: if((*sr&1u))goto L8c02a000;goto L8c029fa4; /* bt 0x8c02a000 */
L8c029fa4: *stop=0x8c029fa4u;r[0]=sx(m,bus,&io_status,0x8c029fe0u,2);if(io_status)return io_status;goto L8c029fa6; /* mov.w 0x8c029fe0,r0 */
L8c029fa6: *stop=0x8c029fa6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029fa8; /* fmov.s @(r0,r15),fr3 */
L8c029fa8: *stop=0x8c029fa8u;r[0]=sx(m,bus,&io_status,0x8c029fe2u,2);if(io_status)return io_status;goto L8c029faa; /* mov.w 0x8c029fe2,r0 */
L8c029faa: *stop=0x8c029faau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c029fac; /* fmov.s @(r0,r15),fr2 */
L8c029fac: *stop=0x8c029facu;r[0]=sx(m,bus,&io_status,0x8c029fe2u,2);if(io_status)return io_status;goto L8c029fae; /* mov.w 0x8c029fe2,r0 */
L8c029fae: *stop=0x8c029faeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c029fb0; /* fadd fr3,fr2 */
L8c029fb0: *stop=0x8c029fb0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c029fb2; /* fmov fr2,@(r0,r15) */
L8c029fb2: *stop=0x8c029fb2u;r[0]=sx(m,bus,&io_status,0x8c029fe4u,2);if(io_status)return io_status;goto L8c029fb4; /* mov.w 0x8c029fe4,r0 */
L8c029fb4: *stop=0x8c029fb4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029fb6; /* fmov.s @(r0,r15),fr3 */
L8c029fb6: *stop=0x8c029fb6u;r[0]=sx(m,bus,&io_status,0x8c029fe6u,2);if(io_status)return io_status;goto L8c029fb8; /* mov.w 0x8c029fe6,r0 */
L8c029fb8: *stop=0x8c029fb8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c029fba; /* fmov.s @(r0,r15),fr2 */
L8c029fba: *stop=0x8c029fbau;r[0]=sx(m,bus,&io_status,0x8c029fe6u,2);if(io_status)return io_status;goto L8c029fbc; /* mov.w 0x8c029fe6,r0 */
L8c029fbc: *stop=0x8c029fbcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c029fbe; /* fadd fr3,fr2 */
L8c029fbe: *stop=0x8c029fbeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c029fc0; /* fmov fr2,@(r0,r15) */
L8c029fc0: *stop=0x8c029fc0u;r[0]=sx(m,bus,&io_status,0x8c029fe8u,2);if(io_status)return io_status;goto L8c029fc2; /* mov.w 0x8c029fe8,r0 */
L8c029fc2: *stop=0x8c029fc2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c029fc4; /* fmov.s @(r0,r15),fr3 */
L8c029fc4: *stop=0x8c029fc4u;r[0]=sx(m,bus,&io_status,0x8c029feau,2);if(io_status)return io_status;goto L8c029fc6; /* mov.w 0x8c029fea,r0 */
L8c029fc6: *stop=0x8c029fc6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c029fc8; /* fmov.s @(r0,r15),fr2 */
L8c029fc8: *stop=0x8c029fc8u;r[0]=sx(m,bus,&io_status,0x8c029feau,2);if(io_status)return io_status;goto L8c029fca; /* mov.w 0x8c029fea,r0 */
L8c029fca: *stop=0x8c029fcau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c029fcc; /* fadd fr3,fr2 */
L8c029fcc: *stop=0x8c029fccu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c029fce; /* fmov fr2,@(r0,r15) */
L8c029fce: *stop=0x8c029fd0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[13],&f->fpscr,&fr[11],0);if(status)return status;}goto L8c029e7c; /* bra 0x8c029e7c */
L8c02a000: *stop=0x8c02a000u;if(!valid_access(r[15]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a002; /* mov.l @(0x2c,r15),r3 */
L8c02a002: *stop=0x8c02a002u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02a004; /* tst r3,r3 */
L8c02a004: if(!(*sr&1u))goto L8c02a00a;goto L8c02a006; /* bf 0x8c02a00a */
L8c02a006: *stop=0x8c02a008u;goto L8c02a400; /* bra 0x8c02a400 */
L8c02a00a: *stop=0x8c02a00au;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a14cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a00c; /* mov.l 0x8c02a14c,r2 */
L8c02a00c: *stop=0x8c02a00cu;if(!valid_access(r[2],1u))return -3;r[3]=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;goto L8c02a00e; /* mov.b @r2,r3 */
L8c02a00e: *stop=0x8c02a00eu;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c02a010; /* cmp/pz r3 */
L8c02a010: if(!(*sr&1u))goto L8c02a0e2;goto L8c02a012; /* bf 0x8c02a0e2 */
L8c02a012: *stop=0x8c02a012u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a014; /* mov.l @r12,r3 */
L8c02a014: *stop=0x8c02a014u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a016; /* mov.l @(0xc,r15),r1 */
L8c02a016: *stop=0x8c02a016u;if(!valid_access(r[3]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a018; /* mov.l @(0x18,r3),r0 */
L8c02a018: *stop=0x8c02a018u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c02a01a; /* mov.l r0,@r1 */
L8c02a01a: *stop=0x8c02a01au;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a01c; /* mov.l @(0x1c,r3),r0 */
L8c02a01c: *stop=0x8c02a01cu;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[0],4);if(io_status)return io_status;goto L8c02a01e; /* mov.l r0,@(0x4,r1) */
L8c02a01e: *stop=0x8c02a01eu;if(!valid_access(r[3]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a020; /* mov.l @(0x20,r3),r0 */
L8c02a020: *stop=0x8c02a020u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[0],4);if(io_status)return io_status;goto L8c02a022; /* mov.l r0,@(0x8,r1) */
L8c02a022: *stop=0x8c02a022u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a024; /* mov.l @r12,r1 */
L8c02a024: *stop=0x8c02a024u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a026; /* mov.l @(0x4,r15),r3 */
L8c02a026: *stop=0x8c02a026u;if(!valid_access(r[1]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x24u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a028; /* mov.l @(0x24,r1),r0 */
L8c02a028: *stop=0x8c02a028u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c02a02a; /* mov.l r0,@r3 */
L8c02a02a: *stop=0x8c02a02au;if(!valid_access(r[1]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x28u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a02c; /* mov.l @(0x28,r1),r0 */
L8c02a02c: *stop=0x8c02a02cu;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[0],4);if(io_status)return io_status;goto L8c02a02e; /* mov.l r0,@(0x4,r3) */
L8c02a02e: *stop=0x8c02a02eu;if(!valid_access(r[1]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x2cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a030; /* mov.l @(0x2c,r1),r0 */
L8c02a030: *stop=0x8c02a030u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[0],4);if(io_status)return io_status;goto L8c02a032; /* mov.l r0,@(0x8,r3) */
L8c02a032: *stop=0x8c02a032u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a034; /* mov.l @r12,r1 */
L8c02a034: *stop=0x8c02a034u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a036; /* mov.l @(0x8,r15),r3 */
L8c02a036: *stop=0x8c02a036u;if(!valid_access(r[1]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x30u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a038; /* mov.l @(0x30,r1),r0 */
L8c02a038: *stop=0x8c02a038u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c02a03a; /* mov.l r0,@r3 */
L8c02a03a: *stop=0x8c02a03au;if(!valid_access(r[1]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x34u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a03c; /* mov.l @(0x34,r1),r0 */
L8c02a03c: *stop=0x8c02a03cu;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[0],4);if(io_status)return io_status;goto L8c02a03e; /* mov.l r0,@(0x4,r3) */
L8c02a03e: *stop=0x8c02a03eu;if(!valid_access(r[1]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x38u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a040; /* mov.l @(0x38,r1),r0 */
L8c02a040: *stop=0x8c02a040u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[0],4);if(io_status)return io_status;goto L8c02a042; /* mov.l r0,@(0x8,r3) */
L8c02a042: *stop=0x8c02a042u;r[0]=sx(m,bus,&io_status,0x8c02a140u,2);if(io_status)return io_status;goto L8c02a044; /* mov.w 0x8c02a140,r0 */
L8c02a044: *stop=0x8c02a044u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a046; /* mov.l @(0xc,r15),r3 */
L8c02a046: *stop=0x8c02a046u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a048; /* fmov.s @(r0,r15),fr3 */
L8c02a048: *stop=0x8c02a048u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c02a04a; /* fmov.s @r3,fr2 */
L8c02a04a: *stop=0x8c02a04au;r[0]=sx(m,bus,&io_status,0x8c02a140u,2);if(io_status)return io_status;goto L8c02a04c; /* mov.w 0x8c02a140,r0 */
L8c02a04c: *stop=0x8c02a04cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a04e; /* fadd fr3,fr2 */
L8c02a04e: *stop=0x8c02a04eu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c02a050; /* fmov.s fr2,@r3 */
L8c02a050: *stop=0x8c02a050u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a052; /* mov.l @(0x4,r15),r3 */
L8c02a052: *stop=0x8c02a052u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c02a054; /* fmov.s @r3,fr2 */
L8c02a054: *stop=0x8c02a054u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a056; /* fmov.s @(r0,r15),fr3 */
L8c02a056: *stop=0x8c02a056u;r[0]=sx(m,bus,&io_status,0x8c02a140u,2);if(io_status)return io_status;goto L8c02a058; /* mov.w 0x8c02a140,r0 */
L8c02a058: *stop=0x8c02a058u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a05a; /* fadd fr3,fr2 */
L8c02a05a: *stop=0x8c02a05au;r[1]=sx(m,bus,&io_status,0x8c02a142u,2);if(io_status)return io_status;goto L8c02a05c; /* mov.w 0x8c02a142,r1 */
L8c02a05c: *stop=0x8c02a05cu;r[1]+=r[15];goto L8c02a05e; /* add r15,r1 */
L8c02a05e: *stop=0x8c02a05eu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c02a060; /* fmov.s fr2,@r3 */
L8c02a060: *stop=0x8c02a060u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a062; /* mov.l @(0x8,r15),r3 */
L8c02a062: *stop=0x8c02a062u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a064; /* fmov.s @(r0,r15),fr3 */
L8c02a064: *stop=0x8c02a064u;r[0]=4u;goto L8c02a066; /* mov #0x4,r0 */
L8c02a066: *stop=0x8c02a066u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c02a068; /* fmov.s @r3,fr2 */
L8c02a068: *stop=0x8c02a068u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a06a; /* fadd fr3,fr2 */
L8c02a06a: *stop=0x8c02a06au;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c02a06c; /* fmov.s fr2,@r3 */
L8c02a06c: *stop=0x8c02a06cu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a06e; /* mov.l @(0xc,r15),r3 */
L8c02a06e: *stop=0x8c02a06eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c02a070; /* fmov.s @r1,fr3 */
L8c02a070: *stop=0x8c02a070u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a072; /* fmov.s @(r0,r3),fr2 */
L8c02a072: *stop=0x8c02a072u;r[1]=sx(m,bus,&io_status,0x8c02a142u,2);if(io_status)return io_status;goto L8c02a074; /* mov.w 0x8c02a142,r1 */
L8c02a074: *stop=0x8c02a074u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a076; /* fadd fr3,fr2 */
L8c02a076: *stop=0x8c02a076u;r[1]+=r[15];goto L8c02a078; /* add r15,r1 */
L8c02a078: *stop=0x8c02a078u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a07a; /* fmov fr2,@(r0,r3) */
L8c02a07a: *stop=0x8c02a07au;r[0]=4u;goto L8c02a07c; /* mov #0x4,r0 */
L8c02a07c: *stop=0x8c02a07cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a07e; /* mov.l @(0x4,r15),r3 */
L8c02a07e: *stop=0x8c02a07eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c02a080; /* fmov.s @r1,fr3 */
L8c02a080: *stop=0x8c02a080u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a082; /* fmov.s @(r0,r3),fr2 */
L8c02a082: *stop=0x8c02a082u;r[1]=sx(m,bus,&io_status,0x8c02a142u,2);if(io_status)return io_status;goto L8c02a084; /* mov.w 0x8c02a142,r1 */
L8c02a084: *stop=0x8c02a084u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a086; /* fadd fr3,fr2 */
L8c02a086: *stop=0x8c02a086u;r[1]+=r[15];goto L8c02a088; /* add r15,r1 */
L8c02a088: *stop=0x8c02a088u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a08a; /* fmov fr2,@(r0,r3) */
L8c02a08a: *stop=0x8c02a08au;r[0]=4u;goto L8c02a08c; /* mov #0x4,r0 */
L8c02a08c: *stop=0x8c02a08cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a08e; /* mov.l @(0x8,r15),r3 */
L8c02a08e: *stop=0x8c02a08eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c02a090; /* fmov.s @r1,fr3 */
L8c02a090: *stop=0x8c02a090u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a092; /* fmov.s @(r0,r3),fr2 */
L8c02a092: *stop=0x8c02a092u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a094; /* fadd fr3,fr2 */
L8c02a094: *stop=0x8c02a094u;r[1]=sx(m,bus,&io_status,0x8c02a144u,2);if(io_status)return io_status;goto L8c02a096; /* mov.w 0x8c02a144,r1 */
L8c02a096: *stop=0x8c02a096u;r[1]+=r[15];goto L8c02a098; /* add r15,r1 */
L8c02a098: *stop=0x8c02a098u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a09a; /* fmov fr2,@(r0,r3) */
L8c02a09a: *stop=0x8c02a09au;r[0]=8u;goto L8c02a09c; /* mov #0x8,r0 */
L8c02a09c: *stop=0x8c02a09cu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a09e; /* mov.l @(0xc,r15),r3 */
L8c02a09e: *stop=0x8c02a09eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c02a0a0; /* fmov.s @r1,fr3 */
L8c02a0a0: *stop=0x8c02a0a0u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a0a2; /* fmov.s @(r0,r3),fr2 */
L8c02a0a2: *stop=0x8c02a0a2u;r[1]=sx(m,bus,&io_status,0x8c02a144u,2);if(io_status)return io_status;goto L8c02a0a4; /* mov.w 0x8c02a144,r1 */
L8c02a0a4: *stop=0x8c02a0a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a0a6; /* fadd fr3,fr2 */
L8c02a0a6: *stop=0x8c02a0a6u;r[1]+=r[15];goto L8c02a0a8; /* add r15,r1 */
L8c02a0a8: *stop=0x8c02a0a8u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a0aa; /* fmov fr2,@(r0,r3) */
L8c02a0aa: *stop=0x8c02a0aau;r[0]=8u;goto L8c02a0ac; /* mov #0x8,r0 */
L8c02a0ac: *stop=0x8c02a0acu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a0ae; /* mov.l @(0x4,r15),r3 */
L8c02a0ae: *stop=0x8c02a0aeu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c02a0b0; /* fmov.s @r1,fr3 */
L8c02a0b0: *stop=0x8c02a0b0u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a0b2; /* fmov.s @(r0,r3),fr2 */
L8c02a0b2: *stop=0x8c02a0b2u;r[1]=sx(m,bus,&io_status,0x8c02a144u,2);if(io_status)return io_status;goto L8c02a0b4; /* mov.w 0x8c02a144,r1 */
L8c02a0b4: *stop=0x8c02a0b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a0b6; /* fadd fr3,fr2 */
L8c02a0b6: *stop=0x8c02a0b6u;r[1]+=r[15];goto L8c02a0b8; /* add r15,r1 */
L8c02a0b8: *stop=0x8c02a0b8u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a0ba; /* fmov fr2,@(r0,r3) */
L8c02a0ba: *stop=0x8c02a0bau;r[0]=8u;goto L8c02a0bc; /* mov #0x8,r0 */
L8c02a0bc: *stop=0x8c02a0bcu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a0be; /* mov.l @(0x8,r15),r3 */
L8c02a0be: *stop=0x8c02a0beu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c02a0c0; /* fmov.s @r1,fr3 */
L8c02a0c0: *stop=0x8c02a0c0u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a0c2; /* fmov.s @(r0,r3),fr2 */
L8c02a0c2: *stop=0x8c02a0c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a0c4; /* fadd fr3,fr2 */
L8c02a0c4: *stop=0x8c02a0c4u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a0c6; /* fmov fr2,@(r0,r3) */
L8c02a0c6: *stop=0x8c02a0c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a150u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a0c8; /* mov.l 0x8c02a150,r3 */
L8c02a0c8: target=r[3];*pr=0x8c02a0ccu;*stop=0x8c02a0cau;if(!valid_access(r[15]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x24u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a0cc; /* jsr @r3 */
L8c02a0cc: *stop=0x8c02a0ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a154u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a0ce; /* mov.l 0x8c02a154,r2 */
L8c02a0ce: target=r[2];*pr=0x8c02a0d2u;*stop=0x8c02a0d0u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a0d2; /* jsr @r2 */
L8c02a0d2: *stop=0x8c02a0d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a15cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a0d4; /* mov.l 0x8c02a15c,r3 */
L8c02a0d4: *stop=0x8c02a0d4u;r[6]=32u;goto L8c02a0d6; /* mov #0x20,r6 */
L8c02a0d6: *stop=0x8c02a0d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a158u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c02a0d8; /* mov.l 0x8c02a158,r4 */
L8c02a0d8: target=r[3];*pr=0x8c02a0dcu;*stop=0x8c02a0dau;r[5]=3u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a0dc; /* jsr @r3 */
L8c02a0dc: *stop=0x8c02a0dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a160u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a0de; /* mov.l 0x8c02a160,r3 */
L8c02a0de: target=r[3];*pr=0x8c02a0e2u;*stop=0x8c02a0e0u;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a0e2; /* jsr @r3 */
L8c02a0e2: *stop=0x8c02a0e2u;r[0]=sx(m,bus,&io_status,0x8c02a146u,2);if(io_status)return io_status;goto L8c02a0e4; /* mov.w 0x8c02a146,r0 */
L8c02a0e4: *stop=0x8c02a0e4u;r[3]=sx(m,bus,&io_status,0x8c02a140u,2);if(io_status)return io_status;goto L8c02a0e6; /* mov.w 0x8c02a140,r3 */
L8c02a0e6: *stop=0x8c02a0e6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a0e8; /* fmov.s @(r0,r15),fr3 */
L8c02a0e8: *stop=0x8c02a0e8u;r[0]=80u;goto L8c02a0ea; /* mov #0x50,r0 */
L8c02a0ea: *stop=0x8c02a0eau;r[3]+=r[15];goto L8c02a0ec; /* add r15,r3 */
L8c02a0ec: *stop=0x8c02a0ecu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c02a0ee; /* fmov fr3,@(r0,r14) */
L8c02a0ee: *stop=0x8c02a0eeu;r[0]=sx(m,bus,&io_status,0x8c02a148u,2);if(io_status)return io_status;goto L8c02a0f0; /* mov.w 0x8c02a148,r0 */
L8c02a0f0: *stop=0x8c02a0f0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a0f2; /* fmov.s @(r0,r15),fr3 */
L8c02a0f2: *stop=0x8c02a0f2u;r[0]=sx(m,bus,&io_status,0x8c02a146u,2);if(io_status)return io_status;goto L8c02a0f4; /* mov.w 0x8c02a146,r0 */
L8c02a0f4: *stop=0x8c02a0f4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a0f6; /* fmov.s @(r0,r15),fr2 */
L8c02a0f6: *stop=0x8c02a0f6u;r[0]=84u;goto L8c02a0f8; /* mov #0x54,r0 */
L8c02a0f8: *stop=0x8c02a0f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a0fa; /* fadd fr3,fr2 */
L8c02a0fa: *stop=0x8c02a0fau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c02a0fc; /* fmov fr2,@(r0,r14) */
L8c02a0fc: *stop=0x8c02a0fcu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a0fe; /* mov.l @r3,r2 */
L8c02a0fe: *stop=0x8c02a0feu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;goto L8c02a100; /* mov.l r2,@(0x8,r14) */
L8c02a100: *stop=0x8c02a100u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a102; /* mov.l @(0x4,r3),r2 */
L8c02a102: *stop=0x8c02a102u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[2],4);if(io_status)return io_status;goto L8c02a104; /* mov.l r2,@(0xc,r14) */
L8c02a104: *stop=0x8c02a104u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a106; /* mov.l @(0x8,r3),r2 */
L8c02a106: *stop=0x8c02a106u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c02a108; /* mov.l r2,@(0x10,r14) */
L8c02a108: *stop=0x8c02a108u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a10a; /* mov.l @(0x18,r15),r1 */
L8c02a10a: *stop=0x8c02a10au;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c02a10c; /* tst r1,r1 */
L8c02a10c: if((*sr&1u))goto L8c02a180;goto L8c02a10e; /* bt 0x8c02a180 */
L8c02a10e: *stop=0x8c02a10eu;r[0]=sx(m,bus,&io_status,0x8c02a146u,2);if(io_status)return io_status;goto L8c02a110; /* mov.w 0x8c02a146,r0 */
L8c02a110: *stop=0x8c02a110u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c02a112; /* fldi0 fr3 */
L8c02a112: *stop=0x8c02a112u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a114; /* fmov.s @(r0,r15),fr2 */
L8c02a114: *stop=0x8c02a114u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c02a116; /* fcmp/gt fr2,fr3 */
L8c02a116: if(!(*sr&1u))goto L8c02a180;goto L8c02a118; /* bf 0x8c02a180 */
L8c02a118: *stop=0x8c02a118u;r[0]=sx(m,bus,&io_status,0x8c02a140u,2);if(io_status)return io_status;goto L8c02a11a; /* mov.w 0x8c02a140,r0 */
L8c02a11a: *stop=0x8c02a11au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a11c; /* fmov.s @(r0,r15),fr2 */
L8c02a11c: *stop=0x8c02a11cu;r[0]=20u;goto L8c02a11e; /* mov #0x14,r0 */
L8c02a11e: *stop=0x8c02a11eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c02a120; /* fneg fr2 */
L8c02a120: *stop=0x8c02a120u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c02a122; /* fmov fr2,@(r0,r14) */
L8c02a122: *stop=0x8c02a122u;r[0]=sx(m,bus,&io_status,0x8c02a142u,2);if(io_status)return io_status;goto L8c02a124; /* mov.w 0x8c02a142,r0 */
L8c02a124: *stop=0x8c02a124u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a126; /* fmov.s @(r0,r15),fr2 */
L8c02a126: *stop=0x8c02a126u;r[0]=24u;goto L8c02a128; /* mov #0x18,r0 */
L8c02a128: *stop=0x8c02a128u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c02a12a; /* fneg fr2 */
L8c02a12a: *stop=0x8c02a12au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c02a12c; /* fmov fr2,@(r0,r14) */
L8c02a12c: *stop=0x8c02a12cu;r[0]=sx(m,bus,&io_status,0x8c02a144u,2);if(io_status)return io_status;goto L8c02a12e; /* mov.w 0x8c02a144,r0 */
L8c02a12e: *stop=0x8c02a12eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a130; /* fmov.s @(r0,r15),fr2 */
L8c02a130: *stop=0x8c02a130u;r[0]=28u;goto L8c02a132; /* mov #0x1c,r0 */
L8c02a132: *stop=0x8c02a132u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c02a134; /* fneg fr2 */
L8c02a134: *stop=0x8c02a136u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c02a190; /* bra 0x8c02a190 */
L8c02a180: *stop=0x8c02a180u;r[2]=sx(m,bus,&io_status,0x8c02a280u,2);if(io_status)return io_status;goto L8c02a182; /* mov.w 0x8c02a280,r2 */
L8c02a182: *stop=0x8c02a182u;r[2]+=r[15];goto L8c02a184; /* add r15,r2 */
L8c02a184: *stop=0x8c02a184u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a186; /* mov.l @r2,r3 */
L8c02a186: *stop=0x8c02a186u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[3],4);if(io_status)return io_status;goto L8c02a188; /* mov.l r3,@(0x14,r14) */
L8c02a188: *stop=0x8c02a188u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a18a; /* mov.l @(0x4,r2),r3 */
L8c02a18a: *stop=0x8c02a18au;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[3],4);if(io_status)return io_status;goto L8c02a18c; /* mov.l r3,@(0x18,r14) */
L8c02a18c: *stop=0x8c02a18cu;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a18e; /* mov.l @(0x8,r2),r3 */
L8c02a18e: *stop=0x8c02a18eu;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c02a190; /* mov.l r3,@(0x1c,r14) */
L8c02a190: *stop=0x8c02a190u;r[0]=24u;goto L8c02a192; /* mov #0x18,r0 */
L8c02a192: *stop=0x8c02a192u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a298u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a194; /* mov.l 0x8c02a298,r3 */
L8c02a194: *stop=0x8c02a194u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c02a196; /* fmov.s @(r0,r14),fr5 */
L8c02a196: *stop=0x8c02a196u;r[0]=20u;goto L8c02a198; /* mov #0x14,r0 */
L8c02a198: target=r[3];*pr=0x8c02a19cu;*stop=0x8c02a19au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a19c; /* jsr @r3 */
L8c02a19c: *stop=0x8c02a19cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c02a19e; /* fmul fr14,fr0 */
L8c02a19e: *stop=0x8c02a19eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a29cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a1a0; /* mov.l 0x8c02a29c,r3 */
L8c02a1a0: *stop=0x8c02a1a0u;r[0]=28u;goto L8c02a1a2; /* mov #0x1c,r0 */
L8c02a1a2: *stop=0x8c02a1a2u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c02a1a4; /* ftrc fr0,FPUL */
L8c02a1a4: *stop=0x8c02a1a4u;r[2]=*fpul;goto L8c02a1a6; /* sts FPUL,r2 */
L8c02a1a6: *stop=0x8c02a1a6u;r[2]=0u-r[2];goto L8c02a1a8; /* neg r2,r2 */
L8c02a1a8: *stop=0x8c02a1a8u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[2],4);if(io_status)return io_status;goto L8c02a1aa; /* mov.l r2,@(0x4,r14) */
L8c02a1aa: target=r[3];*pr=0x8c02a1aeu;*stop=0x8c02a1acu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a1ae; /* jsr @r3 */
L8c02a1ae: *stop=0x8c02a1aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c02a1b0; /* fmul fr14,fr0 */
L8c02a1b0: *stop=0x8c02a1b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a2a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a1b2; /* mov.l 0x8c02a2a0,r3 */
L8c02a1b2: *stop=0x8c02a1b2u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c02a1b4; /* ftrc fr0,FPUL */
L8c02a1b4: *stop=0x8c02a1b4u;r[2]=*fpul;goto L8c02a1b6; /* sts FPUL,r2 */
L8c02a1b6: *stop=0x8c02a1b6u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c02a1b8; /* mov.l r2,@r14 */
L8c02a1b8: target=r[3];*pr=0x8c02a1bcu;*stop=0x8c02a1bau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a1bc; /* jsr @r3 */
L8c02a1bc: *stop=0x8c02a1bcu;r[0]=sx(m,bus,&io_status,0x8c02a282u,2);if(io_status)return io_status;goto L8c02a1be; /* mov.w 0x8c02a282,r0 */
L8c02a1be: *stop=0x8c02a1beu;r[6]=sx(m,bus,&io_status,0x8c02a28cu,2);if(io_status)return io_status;goto L8c02a1c0; /* mov.w 0x8c02a28c,r6 */
L8c02a1c0: *stop=0x8c02a1c0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a1c2; /* fmov.s @(r0,r15),fr3 */
L8c02a1c2: *stop=0x8c02a1c2u;r[0]=sx(m,bus,&io_status,0x8c02a284u,2);if(io_status)return io_status;goto L8c02a1c4; /* mov.w 0x8c02a284,r0 */
L8c02a1c4: *stop=0x8c02a1c4u;r[6]+=r[15];goto L8c02a1c6; /* add r15,r6 */
L8c02a1c6: *stop=0x8c02a1c6u;r[5]=sx(m,bus,&io_status,0x8c02a284u,2);if(io_status)return io_status;goto L8c02a1c8; /* mov.w 0x8c02a284,r5 */
L8c02a1c8: *stop=0x8c02a1c8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a1ca; /* fmov fr3,@(r0,r15) */
L8c02a1ca: *stop=0x8c02a1cau;r[0]=sx(m,bus,&io_status,0x8c02a286u,2);if(io_status)return io_status;goto L8c02a1cc; /* mov.w 0x8c02a286,r0 */
L8c02a1cc: *stop=0x8c02a1ccu;r[5]+=r[15];goto L8c02a1ce; /* add r15,r5 */
L8c02a1ce: *stop=0x8c02a1ceu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a1d0; /* fmov fr15,@(r0,r15) */
L8c02a1d0: *stop=0x8c02a1d0u;r[0]=sx(m,bus,&io_status,0x8c02a288u,2);if(io_status)return io_status;goto L8c02a1d2; /* mov.w 0x8c02a288,r0 */
L8c02a1d2: *stop=0x8c02a1d2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a1d4; /* fmov.s @(r0,r15),fr3 */
L8c02a1d4: *stop=0x8c02a1d4u;r[0]=sx(m,bus,&io_status,0x8c02a28au,2);if(io_status)return io_status;goto L8c02a1d6; /* mov.w 0x8c02a28a,r0 */
L8c02a1d6: *stop=0x8c02a1d6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a1d8; /* fmov fr3,@(r0,r15) */
L8c02a1d8: target=r[11];*pr=0x8c02a1dcu;*stop=0x8c02a1dau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a1dc; /* jsr @r11 */
L8c02a1dc: *stop=0x8c02a1dcu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a1de; /* mov.l @r12,r3 */
L8c02a1de: *stop=0x8c02a1deu;r[0]=24u;goto L8c02a1e0; /* mov #0x18,r0 */
L8c02a1e0: *stop=0x8c02a1e0u;r[6]=sx(m,bus,&io_status,0x8c02a28cu,2);if(io_status)return io_status;goto L8c02a1e2; /* mov.w 0x8c02a28c,r6 */
L8c02a1e2: *stop=0x8c02a1e2u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a1e4; /* fmov.s @(r0,r3),fr3 */
L8c02a1e4: *stop=0x8c02a1e4u;r[0]=sx(m,bus,&io_status,0x8c02a28cu,2);if(io_status)return io_status;goto L8c02a1e6; /* mov.w 0x8c02a28c,r0 */
L8c02a1e6: *stop=0x8c02a1e6u;r[6]+=r[15];goto L8c02a1e8; /* add r15,r6 */
L8c02a1e8: *stop=0x8c02a1e8u;r[5]=sx(m,bus,&io_status,0x8c02a284u,2);if(io_status)return io_status;goto L8c02a1ea; /* mov.w 0x8c02a284,r5 */
L8c02a1ea: *stop=0x8c02a1eau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a1ec; /* fmov.s @(r0,r15),fr2 */
L8c02a1ec: *stop=0x8c02a1ecu;r[0]=32u;goto L8c02a1ee; /* mov #0x20,r0 */
L8c02a1ee: *stop=0x8c02a1eeu;r[5]+=r[15];goto L8c02a1f0; /* add r15,r5 */
L8c02a1f0: *stop=0x8c02a1f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a1f2; /* fadd fr3,fr2 */
L8c02a1f2: *stop=0x8c02a1f2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c02a1f4; /* fmov fr2,@(r0,r14) */
L8c02a1f4: *stop=0x8c02a1f4u;r[0]=28u;goto L8c02a1f6; /* mov #0x1c,r0 */
L8c02a1f6: *stop=0x8c02a1f6u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a1f8; /* mov.l @r12,r3 */
L8c02a1f8: *stop=0x8c02a1f8u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a1fa; /* fmov.s @(r0,r3),fr3 */
L8c02a1fa: *stop=0x8c02a1fau;r[0]=sx(m,bus,&io_status,0x8c02a28eu,2);if(io_status)return io_status;goto L8c02a1fc; /* mov.w 0x8c02a28e,r0 */
L8c02a1fc: *stop=0x8c02a1fcu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a1fe; /* fmov.s @(r0,r15),fr2 */
L8c02a1fe: *stop=0x8c02a1feu;r[0]=36u;goto L8c02a200; /* mov #0x24,r0 */
L8c02a200: *stop=0x8c02a200u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a202; /* fadd fr3,fr2 */
L8c02a202: *stop=0x8c02a202u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c02a204; /* fmov fr2,@(r0,r14) */
L8c02a204: *stop=0x8c02a204u;r[0]=32u;goto L8c02a206; /* mov #0x20,r0 */
L8c02a206: *stop=0x8c02a206u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a208; /* mov.l @r12,r3 */
L8c02a208: *stop=0x8c02a208u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a20a; /* fmov.s @(r0,r3),fr3 */
L8c02a20a: *stop=0x8c02a20au;r[0]=sx(m,bus,&io_status,0x8c02a290u,2);if(io_status)return io_status;goto L8c02a20c; /* mov.w 0x8c02a290,r0 */
L8c02a20c: *stop=0x8c02a20cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a20e; /* fmov.s @(r0,r15),fr2 */
L8c02a20e: *stop=0x8c02a20eu;r[0]=40u;goto L8c02a210; /* mov #0x28,r0 */
L8c02a210: *stop=0x8c02a210u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a212; /* fadd fr3,fr2 */
L8c02a212: *stop=0x8c02a212u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c02a214; /* fmov fr2,@(r0,r14) */
L8c02a214: *stop=0x8c02a214u;r[0]=sx(m,bus,&io_status,0x8c02a286u,2);if(io_status)return io_status;goto L8c02a216; /* mov.w 0x8c02a286,r0 */
L8c02a216: *stop=0x8c02a216u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,12u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a218; /* fmov fr12,@(r0,r15) */
L8c02a218: target=r[11];*pr=0x8c02a21cu;*stop=0x8c02a21au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a21c; /* jsr @r11 */
L8c02a21c: *stop=0x8c02a21cu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a21e; /* mov.l @r12,r3 */
L8c02a21e: *stop=0x8c02a21eu;r[0]=24u;goto L8c02a220; /* mov #0x18,r0 */
L8c02a220: *stop=0x8c02a220u;r[2]=r[15];goto L8c02a222; /* mov r15,r2 */
L8c02a222: *stop=0x8c02a222u;r[2]+=48u;goto L8c02a224; /* add #0x30,r2 */
L8c02a224: *stop=0x8c02a224u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a226; /* fmov.s @(r0,r3),fr3 */
L8c02a226: *stop=0x8c02a226u;r[0]=sx(m,bus,&io_status,0x8c02a28cu,2);if(io_status)return io_status;goto L8c02a228; /* mov.w 0x8c02a28c,r0 */
L8c02a228: *stop=0x8c02a228u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a22a; /* fmov.s @(r0,r15),fr2 */
L8c02a22a: *stop=0x8c02a22au;r[0]=44u;goto L8c02a22c; /* mov #0x2c,r0 */
L8c02a22c: *stop=0x8c02a22cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a22e; /* fadd fr3,fr2 */
L8c02a22e: *stop=0x8c02a22eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c02a230; /* fmov fr2,@(r0,r14) */
L8c02a230: *stop=0x8c02a230u;r[0]=28u;goto L8c02a232; /* mov #0x1c,r0 */
L8c02a232: *stop=0x8c02a232u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a234; /* mov.l @r12,r3 */
L8c02a234: *stop=0x8c02a234u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a236; /* fmov.s @(r0,r3),fr3 */
L8c02a236: *stop=0x8c02a236u;r[0]=sx(m,bus,&io_status,0x8c02a28eu,2);if(io_status)return io_status;goto L8c02a238; /* mov.w 0x8c02a28e,r0 */
L8c02a238: *stop=0x8c02a238u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a23a; /* fmov.s @(r0,r15),fr2 */
L8c02a23a: *stop=0x8c02a23au;r[0]=48u;goto L8c02a23c; /* mov #0x30,r0 */
L8c02a23c: *stop=0x8c02a23cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a23e; /* fadd fr3,fr2 */
L8c02a23e: *stop=0x8c02a23eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c02a240; /* fmov fr2,@(r0,r14) */
L8c02a240: *stop=0x8c02a240u;r[0]=32u;goto L8c02a242; /* mov #0x20,r0 */
L8c02a242: *stop=0x8c02a242u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a244; /* mov.l @r12,r3 */
L8c02a244: *stop=0x8c02a244u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c02a246; /* fmov.s @(r0,r3),fr3 */
L8c02a246: *stop=0x8c02a246u;r[3]=sx(m,bus,&io_status,0x8c02a292u,2);if(io_status)return io_status;goto L8c02a248; /* mov.w 0x8c02a292,r3 */
L8c02a248: *stop=0x8c02a248u;r[0]=sx(m,bus,&io_status,0x8c02a290u,2);if(io_status)return io_status;goto L8c02a24a; /* mov.w 0x8c02a290,r0 */
L8c02a24a: *stop=0x8c02a24au;r[3]+=r[15];goto L8c02a24c; /* add r15,r3 */
L8c02a24c: *stop=0x8c02a24cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a24e; /* fmov.s @(r0,r15),fr2 */
L8c02a24e: *stop=0x8c02a24eu;r[0]=52u;goto L8c02a250; /* mov #0x34,r0 */
L8c02a250: *stop=0x8c02a250u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a252; /* fadd fr3,fr2 */
L8c02a252: *stop=0x8c02a252u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c02a254; /* fmov fr2,@(r0,r14) */
L8c02a254: *stop=0x8c02a254u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a256; /* mov.l @r3,r1 */
L8c02a256: *stop=0x8c02a256u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c02a258; /* mov.l r1,@r2 */
L8c02a258: *stop=0x8c02a258u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a25a; /* mov.l @(0x4,r3),r1 */
L8c02a25a: *stop=0x8c02a25au;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c02a25c; /* mov.l r1,@(0x4,r2) */
L8c02a25c: *stop=0x8c02a25cu;r[0]=sx(m,bus,&io_status,0x8c02a294u,2);if(io_status)return io_status;goto L8c02a25e; /* mov.w 0x8c02a294,r0 */
L8c02a25e: *stop=0x8c02a25eu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a260; /* mov.l @(0x8,r3),r1 */
L8c02a260: *stop=0x8c02a260u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c02a262; /* mov.l r1,@(0x8,r2) */
L8c02a262: *stop=0x8c02a262u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a264; /* mov.l @(0x18,r15),r3 */
L8c02a264: *stop=0x8c02a264u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a266; /* fmov.s @(r0,r15),fr4 */
L8c02a266: *stop=0x8c02a266u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c02a268; /* tst r3,r3 */
L8c02a268: if((*sr&1u))goto L8c02a2c0;goto L8c02a26a; /* bt 0x8c02a2c0 */
L8c02a26a: *stop=0x8c02a26au;r[0]=sx(m,bus,&io_status,0x8c02a296u,2);if(io_status)return io_status;goto L8c02a26c; /* mov.w 0x8c02a296,r0 */
L8c02a26c: *stop=0x8c02a26cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c02a26e; /* fldi0 fr2 */
L8c02a26e: *stop=0x8c02a26eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a270; /* fmov.s @(r0,r15),fr3 */
L8c02a270: *stop=0x8c02a270u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c02a272; /* fmul fr4,fr3 */
L8c02a272: *stop=0x8c02a272u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c02a274; /* fcmp/gt fr3,fr2 */
L8c02a274: if(!(*sr&1u))goto L8c02a2ce;goto L8c02a276; /* bf 0x8c02a2ce */
L8c02a276: *stop=0x8c02a276u;r[0]=52u;goto L8c02a278; /* mov #0x34,r0 */
L8c02a278: *stop=0x8c02a278u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c02a27a; /* fneg fr4 */
L8c02a27a: *stop=0x8c02a27cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a2ce; /* bra 0x8c02a2ce */
L8c02a2c0: *stop=0x8c02a2c0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[4],&f->fpscr,&fr[12]);if(status)return status;}goto L8c02a2c2; /* fmul fr4,fr12 */
L8c02a2c2: *stop=0x8c02a2c2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c02a2c4; /* fldi0 fr3 */
L8c02a2c4: *stop=0x8c02a2c4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][12],&f->fpscr,sr,1);if(status)return status;goto L8c02a2c6; /* fcmp/gt fr12,fr3 */
L8c02a2c6: if(!(*sr&1u))goto L8c02a2ce;goto L8c02a2c8; /* bf 0x8c02a2ce */
L8c02a2c8: *stop=0x8c02a2c8u;r[0]=52u;goto L8c02a2ca; /* mov #0x34,r0 */
L8c02a2ca: *stop=0x8c02a2cau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c02a2cc; /* fneg fr4 */
L8c02a2cc: *stop=0x8c02a2ccu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a2ce; /* fmov fr4,@(r0,r15) */
L8c02a2ce: *stop=0x8c02a2ceu;r[0]=sx(m,bus,&io_status,0x8c02a3c0u,2);if(io_status)return io_status;goto L8c02a2d0; /* mov.w 0x8c02a3c0,r0 */
L8c02a2d0: *stop=0x8c02a2d0u;r[5]=r[15];goto L8c02a2d2; /* mov r15,r5 */
L8c02a2d2: *stop=0x8c02a2d2u;r[6]=sx(m,bus,&io_status,0x8c02a3c4u,2);if(io_status)return io_status;goto L8c02a2d4; /* mov.w 0x8c02a3c4,r6 */
L8c02a2d4: *stop=0x8c02a2d4u;r[5]+=48u;goto L8c02a2d6; /* add #0x30,r5 */
L8c02a2d6: *stop=0x8c02a2d6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a2d8; /* fmov.s @(r0,r15),fr3 */
L8c02a2d8: *stop=0x8c02a2d8u;r[0]=48u;goto L8c02a2da; /* mov #0x30,r0 */
L8c02a2da: *stop=0x8c02a2dau;r[6]+=r[15];goto L8c02a2dc; /* add r15,r6 */
L8c02a2dc: *stop=0x8c02a2dcu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a2de; /* fmov fr3,@(r0,r15) */
L8c02a2de: *stop=0x8c02a2deu;r[0]=52u;goto L8c02a2e0; /* mov #0x34,r0 */
L8c02a2e0: *stop=0x8c02a2e0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a2e2; /* fmov fr15,@(r0,r15) */
L8c02a2e2: *stop=0x8c02a2e2u;r[0]=sx(m,bus,&io_status,0x8c02a3c2u,2);if(io_status)return io_status;goto L8c02a2e4; /* mov.w 0x8c02a3c2,r0 */
L8c02a2e4: *stop=0x8c02a2e4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a2e6; /* fmov.s @(r0,r15),fr3 */
L8c02a2e6: *stop=0x8c02a2e6u;r[0]=56u;goto L8c02a2e8; /* mov #0x38,r0 */
L8c02a2e8: *stop=0x8c02a2e8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a2ea; /* fmov fr3,@(r0,r15) */
L8c02a2ea: target=r[11];*pr=0x8c02a2eeu;*stop=0x8c02a2ecu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a2ee; /* jsr @r11 */
L8c02a2ee: *stop=0x8c02a2eeu;r[3]=sx(m,bus,&io_status,0x8c02a3c4u,2);if(io_status)return io_status;goto L8c02a2f0; /* mov.w 0x8c02a3c4,r3 */
L8c02a2f0: *stop=0x8c02a2f0u;r[0]=48u;goto L8c02a2f2; /* mov #0x30,r0 */
L8c02a2f2: *stop=0x8c02a2f2u;r[2]=r[14];goto L8c02a2f4; /* mov r14,r2 */
L8c02a2f4: *stop=0x8c02a2f4u;r[2]+=56u;goto L8c02a2f6; /* add #0x38,r2 */
L8c02a2f6: *stop=0x8c02a2f6u;r[3]+=r[15];goto L8c02a2f8; /* add r15,r3 */
L8c02a2f8: *stop=0x8c02a2f8u;r[6]=sx(m,bus,&io_status,0x8c02a3c4u,2);if(io_status)return io_status;goto L8c02a2fa; /* mov.w 0x8c02a3c4,r6 */
L8c02a2fa: *stop=0x8c02a2fau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a2fc; /* mov.l @r3,r1 */
L8c02a2fc: *stop=0x8c02a2fcu;r[5]=r[15];goto L8c02a2fe; /* mov r15,r5 */
L8c02a2fe: *stop=0x8c02a2feu;r[5]+=48u;goto L8c02a300; /* add #0x30,r5 */
L8c02a300: *stop=0x8c02a300u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c02a302; /* mov.l r1,@r2 */
L8c02a302: *stop=0x8c02a302u;r[6]+=r[15];goto L8c02a304; /* add r15,r6 */
L8c02a304: *stop=0x8c02a304u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a306; /* mov.l @(0x4,r3),r1 */
L8c02a306: *stop=0x8c02a306u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c02a308; /* mov.l r1,@(0x4,r2) */
L8c02a308: *stop=0x8c02a308u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a30a; /* mov.l @(0x8,r3),r1 */
L8c02a30a: *stop=0x8c02a30au;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c02a30c; /* mov.l r1,@(0x8,r2) */
L8c02a30c: *stop=0x8c02a30cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a30e; /* fmov fr15,@(r0,r15) */
L8c02a30e: *stop=0x8c02a30eu;r[0]=sx(m,bus,&io_status,0x8c02a3c6u,2);if(io_status)return io_status;goto L8c02a310; /* mov.w 0x8c02a3c6,r0 */
L8c02a310: *stop=0x8c02a310u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a312; /* fmov.s @(r0,r15),fr3 */
L8c02a312: *stop=0x8c02a312u;r[0]=52u;goto L8c02a314; /* mov #0x34,r0 */
L8c02a314: *stop=0x8c02a314u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a316; /* fmov fr3,@(r0,r15) */
L8c02a316: *stop=0x8c02a316u;r[0]=56u;goto L8c02a318; /* mov #0x38,r0 */
L8c02a318: *stop=0x8c02a318u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c02a31a; /* fmov fr15,@(r0,r15) */
L8c02a31a: target=r[11];*pr=0x8c02a31eu;*stop=0x8c02a31cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a31e; /* jsr @r11 */
L8c02a31e: *stop=0x8c02a31eu;r[3]=sx(m,bus,&io_status,0x8c02a3c4u,2);if(io_status)return io_status;goto L8c02a320; /* mov.w 0x8c02a3c4,r3 */
L8c02a320: *stop=0x8c02a320u;r[2]=r[14];goto L8c02a322; /* mov r14,r2 */
L8c02a322: *stop=0x8c02a322u;r[2]+=68u;goto L8c02a324; /* add #0x44,r2 */
L8c02a324: *stop=0x8c02a324u;r[3]+=r[15];goto L8c02a326; /* add r15,r3 */
L8c02a326: *stop=0x8c02a326u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a328; /* mov.l @r3,r1 */
L8c02a328: *stop=0x8c02a328u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c02a32a; /* mov.l r1,@r2 */
L8c02a32a: *stop=0x8c02a32au;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a32c; /* mov.l @(0x4,r3),r1 */
L8c02a32c: *stop=0x8c02a32cu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c02a32e; /* mov.l r1,@(0x4,r2) */
L8c02a32e: *stop=0x8c02a32eu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a330; /* mov.l @(0x8,r3),r1 */
L8c02a330: *stop=0x8c02a330u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c02a332; /* mov.l r1,@(0x8,r2) */
L8c02a332: *stop=0x8c02a332u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a334; /* mov.l @r12,r3 */
L8c02a334: *stop=0x8c02a334u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a336; /* mov.l @(0x10,r15),r0 */
L8c02a336: *stop=0x8c02a336u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a338; /* mov.l @r3,r2 */
L8c02a338: *stop=0x8c02a338u;r[0]+=4u;goto L8c02a33a; /* add #0x4,r0 */
L8c02a33a: *stop=0x8c02a33au;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c02a33c; /* mov.l r2,@r0 */
L8c02a33c: *stop=0x8c02a33cu;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a33e; /* mov.l @(0x4,r3),r2 */
L8c02a33e: *stop=0x8c02a33eu;if(!valid_access(r[0]+4u,4u))return -3;wr(m,bus,&io_status,r[0]+0x4u,r[2],4);if(io_status)return io_status;goto L8c02a340; /* mov.l r2,@(0x4,r0) */
L8c02a340: *stop=0x8c02a340u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a342; /* mov.l @(0x8,r3),r2 */
L8c02a342: *stop=0x8c02a342u;if(!valid_access(r[0]+8u,4u))return -3;wr(m,bus,&io_status,r[0]+0x8u,r[2],4);if(io_status)return io_status;goto L8c02a344; /* mov.l r2,@(0x8,r0) */
L8c02a344: *stop=0x8c02a344u;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a346; /* mov.l @(0xc,r3),r2 */
L8c02a346: *stop=0x8c02a346u;if(!valid_access(r[0]+12u,4u))return -3;wr(m,bus,&io_status,r[0]+0xcu,r[2],4);if(io_status)return io_status;goto L8c02a348; /* mov.l r2,@(0xc,r0) */
L8c02a348: *stop=0x8c02a348u;if(!valid_access(r[3]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a34a; /* mov.l @(0x10,r3),r2 */
L8c02a34a: *stop=0x8c02a34au;if(!valid_access(r[0]+16u,4u))return -3;wr(m,bus,&io_status,r[0]+0x10u,r[2],4);if(io_status)return io_status;goto L8c02a34c; /* mov.l r2,@(0x10,r0) */
L8c02a34c: *stop=0x8c02a34cu;if(!valid_access(r[3]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a34e; /* mov.l @(0x14,r3),r2 */
L8c02a34e: *stop=0x8c02a34eu;if(!valid_access(r[0]+20u,4u))return -3;wr(m,bus,&io_status,r[0]+0x14u,r[2],4);if(io_status)return io_status;goto L8c02a350; /* mov.l r2,@(0x14,r0) */
L8c02a350: *stop=0x8c02a350u;if(!valid_access(r[3]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a352; /* mov.l @(0x18,r3),r2 */
L8c02a352: *stop=0x8c02a352u;if(!valid_access(r[0]+24u,4u))return -3;wr(m,bus,&io_status,r[0]+0x18u,r[2],4);if(io_status)return io_status;goto L8c02a354; /* mov.l r2,@(0x18,r0) */
L8c02a354: *stop=0x8c02a354u;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a356; /* mov.l @(0x1c,r3),r2 */
L8c02a356: *stop=0x8c02a356u;if(!valid_access(r[0]+28u,4u))return -3;wr(m,bus,&io_status,r[0]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c02a358; /* mov.l r2,@(0x1c,r0) */
L8c02a358: *stop=0x8c02a358u;if(!valid_access(r[3]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a35a; /* mov.l @(0x20,r3),r2 */
L8c02a35a: *stop=0x8c02a35au;if(!valid_access(r[0]+32u,4u))return -3;wr(m,bus,&io_status,r[0]+0x20u,r[2],4);if(io_status)return io_status;goto L8c02a35c; /* mov.l r2,@(0x20,r0) */
L8c02a35c: *stop=0x8c02a35cu;if(!valid_access(r[3]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a35e; /* mov.l @(0x24,r3),r2 */
L8c02a35e: *stop=0x8c02a35eu;if(!valid_access(r[0]+36u,4u))return -3;wr(m,bus,&io_status,r[0]+0x24u,r[2],4);if(io_status)return io_status;goto L8c02a360; /* mov.l r2,@(0x24,r0) */
L8c02a360: *stop=0x8c02a360u;if(!valid_access(r[3]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a362; /* mov.l @(0x28,r3),r2 */
L8c02a362: *stop=0x8c02a362u;if(!valid_access(r[0]+40u,4u))return -3;wr(m,bus,&io_status,r[0]+0x28u,r[2],4);if(io_status)return io_status;goto L8c02a364; /* mov.l r2,@(0x28,r0) */
L8c02a364: *stop=0x8c02a364u;if(!valid_access(r[3]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x2cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a366; /* mov.l @(0x2c,r3),r2 */
L8c02a366: *stop=0x8c02a366u;if(!valid_access(r[0]+44u,4u))return -3;wr(m,bus,&io_status,r[0]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c02a368; /* mov.l r2,@(0x2c,r0) */
L8c02a368: *stop=0x8c02a368u;if(!valid_access(r[3]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x30u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a36a; /* mov.l @(0x30,r3),r2 */
L8c02a36a: *stop=0x8c02a36au;if(!valid_access(r[0]+48u,4u))return -3;wr(m,bus,&io_status,r[0]+0x30u,r[2],4);if(io_status)return io_status;goto L8c02a36c; /* mov.l r2,@(0x30,r0) */
L8c02a36c: *stop=0x8c02a36cu;if(!valid_access(r[3]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x34u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a36e; /* mov.l @(0x34,r3),r2 */
L8c02a36e: *stop=0x8c02a36eu;if(!valid_access(r[0]+52u,4u))return -3;wr(m,bus,&io_status,r[0]+0x34u,r[2],4);if(io_status)return io_status;goto L8c02a370; /* mov.l r2,@(0x34,r0) */
L8c02a370: *stop=0x8c02a370u;if(!valid_access(r[3]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a372; /* mov.l @(0x38,r3),r2 */
L8c02a372: *stop=0x8c02a372u;if(!valid_access(r[0]+56u,4u))return -3;wr(m,bus,&io_status,r[0]+0x38u,r[2],4);if(io_status)return io_status;goto L8c02a374; /* mov.l r2,@(0x38,r0) */
L8c02a374: *stop=0x8c02a374u;if(!valid_access(r[3]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x3cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a376; /* mov.l @(0x3c,r3),r2 */
L8c02a376: *stop=0x8c02a376u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a3ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a378; /* mov.l 0x8c02a3cc,r3 */
L8c02a378: *stop=0x8c02a378u;if(!valid_access(r[0]+60u,4u))return -3;wr(m,bus,&io_status,r[0]+0x3cu,r[2],4);if(io_status)return io_status;goto L8c02a37a; /* mov.l r2,@(0x3c,r0) */
L8c02a37a: *stop=0x8c02a37au;r[2]=r[14];goto L8c02a37c; /* mov r14,r2 */
L8c02a37c: *stop=0x8c02a37cu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a37e; /* mov.l @(0x10,r15),r1 */
L8c02a37e: *stop=0x8c02a37eu;r[1]+=68u;goto L8c02a380; /* add #0x44,r1 */
L8c02a380: target=r[3];*pr=0x8c02a384u;*stop=0x8c02a382u;r[0]=88u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a384; /* jsr @r3 */
L8c02a384: *stop=0x8c02a384u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a3d0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a386; /* mov.l 0x8c02a3d0,r1 */
L8c02a386: *stop=0x8c02a386u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a388; /* mov.l @(0x10,r15),r2 */
L8c02a388: *stop=0x8c02a388u;r[3]=sx(m,bus,&io_status,0x8c02a3c8u,2);if(io_status)return io_status;goto L8c02a38a; /* mov.w 0x8c02a3c8,r3 */
L8c02a38a: *stop=0x8c02a38au;r[2]+=r[3];goto L8c02a38c; /* add r3,r2 */
L8c02a38c: *stop=0x8c02a38cu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c02a38e; /* mov.l r2,@(0x10,r15) */
L8c02a38e: *stop=0x8c02a38eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a390; /* mov.l @r1,r0 */
L8c02a390: *stop=0x8c02a390u;r[0]+=1u;goto L8c02a392; /* add #0x1,r0 */
L8c02a392: *stop=0x8c02a392u;*sr=(*sr&~1u)|(r[0]==0x00000010u);goto L8c02a394; /* cmp/eq #0x10,r0 */
L8c02a394: take=!(*sr&1u);*stop=0x8c02a396u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;if(take)goto L8c02a400;goto L8c02a398; /* bf/s 0x8c02a400 */
L8c02a398: *stop=0x8c02a398u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a3d4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a39a; /* mov.l 0x8c02a3d4,r3 */
L8c02a39a: *stop=0x8c02a39au;if(!valid_access(r[3],1u))return -3;r[1]=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;goto L8c02a39c; /* mov.b @r3,r1 */
L8c02a39c: *stop=0x8c02a39cu;*sr=(*sr&~1u)|((int32_t)r[1]>=0);goto L8c02a39e; /* cmp/pz r1 */
L8c02a39e: if(!(*sr&1u))goto L8c02a416;goto L8c02a3a0; /* bf 0x8c02a416 */
L8c02a3a0: *stop=0x8c02a3a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a3e0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a3a2; /* mov.l 0x8c02a3e0,r1 */
L8c02a3a2: *stop=0x8c02a3a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a3dcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c02a3a4; /* mov.l 0x8c02a3dc,r4 */
L8c02a3a4: *stop=0x8c02a3a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a3d8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c02a3a6; /* mov.l 0x8c02a3d8,r5 */
L8c02a3a6: target=r[1];*pr=0x8c02a3aau;*stop=0x8c02a3a8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a3aa; /* jsr @r1 */
L8c02a3aa: *stop=0x8c02a3acu;goto L8c02a416; /* bra 0x8c02a416 */
L8c02a400: *stop=0x8c02a400u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a440u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a402; /* mov.l 0x8c02a440,r2 */
L8c02a402: target=r[2];*pr=0x8c02a406u;*stop=0x8c02a404u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a406; /* jsr @r2 */
L8c02a406: *stop=0x8c02a406u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a408; /* mov.l @(0x14,r15),r3 */
L8c02a408: *stop=0x8c02a408u;r[3]+=4294967295u;goto L8c02a40a; /* add #-0x1,r3 */
L8c02a40a: *stop=0x8c02a40au;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c02a40c; /* cmp/pl r3 */
L8c02a40c: *stop=0x8c02a40cu;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[3],4);if(io_status)return io_status;goto L8c02a40e; /* mov.l r3,@(0x14,r15) */
L8c02a40e: take=!(*sr&1u);*stop=0x8c02a410u;r[12]+=4u;if(take)goto L8c02a416;goto L8c02a412; /* bf/s 0x8c02a416 */
L8c02a412: *stop=0x8c02a414u;goto L8c029bf6; /* bra 0x8c029bf6 */
L8c02a416: *stop=0x8c02a416u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a444u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a418; /* mov.l 0x8c02a444,r3 */
L8c02a418: *stop=0x8c02a418u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02a41a; /* mov.l @r3,r5 */
L8c02a41a: *stop=0x8c02a41au;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[10]);goto L8c02a41c; /* cmp/ge r10,r5 */
L8c02a41c: if((*sr&1u))goto L8c02a460;goto L8c02a41e; /* bt 0x8c02a460 */
L8c02a41e: *stop=0x8c02a420u;r[0]=0u;goto L8c02a632; /* bra 0x8c02a632 */
L8c02a460: *stop=0x8c02a460u;r[0]=sx(m,bus,&io_status,0x8c02a540u,2);if(io_status)return io_status;goto L8c02a462; /* mov.w 0x8c02a540,r0 */
L8c02a462: *stop=0x8c02a462u;r[4]=r[10];goto L8c02a464; /* mov r10,r4 */
L8c02a464: *stop=0x8c02a464u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a544u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c02a466; /* mov.l 0x8c02a544,r12 */
L8c02a466: *stop=0x8c02a466u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]);goto L8c02a468; /* cmp/ge r5,r4 */
L8c02a468: *stop=0x8c02a468u;r[6]=sx(m,bus,&io_status,0x8c02a542u,2);if(io_status)return io_status;goto L8c02a46a; /* mov.w 0x8c02a542,r6 */
L8c02a46a: *stop=0x8c02a46au;r[7]=0u;goto L8c02a46c; /* mov #0x0,r7 */
L8c02a46c: *stop=0x8c02a46cu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[12]);if(io_status)return io_status;goto L8c02a46e; /* fmov.s @(r0,r12),fr4 */
L8c02a46e: take=(*sr&1u);*stop=0x8c02a470u;r[6]+=r[12];if(take)goto L8c02a486;goto L8c02a472; /* bt/s 0x8c02a486 */
L8c02a472: *stop=0x8c02a472u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c02a474; /* fmov.s @(r0,r6),fr3 */
L8c02a474: *stop=0x8c02a474u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c02a476; /* fcmp/gt fr3,fr4 */
L8c02a476: if(!(*sr&1u))goto L8c02a47c;goto L8c02a478; /* bf 0x8c02a47c */
L8c02a478: *stop=0x8c02a478u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c02a47a; /* fmov.s @(r0,r6),fr4 */
L8c02a47a: *stop=0x8c02a47au;r[7]=r[4];goto L8c02a47c; /* mov r4,r7 */
L8c02a47c: *stop=0x8c02a47cu;r[3]=sx(m,bus,&io_status,0x8c02a542u,2);if(io_status)return io_status;goto L8c02a47e; /* mov.w 0x8c02a542,r3 */
L8c02a47e: *stop=0x8c02a47eu;r[4]+=1u;goto L8c02a480; /* add #0x1,r4 */
L8c02a480: *stop=0x8c02a480u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]);goto L8c02a482; /* cmp/ge r5,r4 */
L8c02a482: take=!(*sr&1u);*stop=0x8c02a484u;r[6]+=r[3];if(take)goto L8c02a472;goto L8c02a486; /* bf/s 0x8c02a472 */
L8c02a486: *stop=0x8c02a486u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a548u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c02a488; /* mov.l 0x8c02a548,r6 */
L8c02a488: *stop=0x8c02a488u;r[4]=r[5];goto L8c02a48a; /* mov r5,r4 */
L8c02a48a: *stop=0x8c02a48au;*sr=(*sr&~1u)|((int32_t)r[4]>0);goto L8c02a48c; /* cmp/pl r4 */
L8c02a48c: take=!(*sr&1u);*stop=0x8c02a48eu;r[14]=r[12];if(take)goto L8c02a4a8;goto L8c02a490; /* bf/s 0x8c02a4a8 */
L8c02a490: *stop=0x8c02a490u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a54cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a492; /* mov.l 0x8c02a54c,r3 */
L8c02a492: *stop=0x8c02a492u;r[2]=r[14];goto L8c02a494; /* mov r14,r2 */
L8c02a494: *stop=0x8c02a494u;r[1]=r[6];goto L8c02a496; /* mov r6,r1 */
L8c02a496: *stop=0x8c02a496u;r[2]+=68u;goto L8c02a498; /* add #0x44,r2 */
L8c02a498: target=r[3];*pr=0x8c02a49cu;*stop=0x8c02a49au;r[0]=88u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02a49c; /* jsr @r3 */
L8c02a49c: *stop=0x8c02a49cu;r[2]=sx(m,bus,&io_status,0x8c02a542u,2);if(io_status)return io_status;goto L8c02a49e; /* mov.w 0x8c02a542,r2 */
L8c02a49e: *stop=0x8c02a49eu;r[4]+=4294967295u;goto L8c02a4a0; /* add #-0x1,r4 */
L8c02a4a0: *stop=0x8c02a4a0u;*sr=(*sr&~1u)|((int32_t)r[4]>0);goto L8c02a4a2; /* cmp/pl r4 */
L8c02a4a2: *stop=0x8c02a4a2u;r[14]+=r[2];goto L8c02a4a4; /* add r2,r14 */
L8c02a4a4: take=(*sr&1u);*stop=0x8c02a4a6u;r[6]+=88u;if(take)goto L8c02a490;goto L8c02a4a8; /* bt/s 0x8c02a490 */
L8c02a4a8: *stop=0x8c02a4a8u;r[4]=sx(m,bus,&io_status,0x8c02a542u,2);if(io_status)return io_status;goto L8c02a4aa; /* mov.w 0x8c02a542,r4 */
L8c02a4aa: *stop=0x8c02a4aau;r[0]=40u;goto L8c02a4ac; /* mov #0x28,r0 */
L8c02a4ac: *stop=0x8c02a4acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a550u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a4ae; /* mov.l 0x8c02a550,r3 */
L8c02a4ae: *stop=0x8c02a4aeu;*macl=(uint32_t)((int32_t)(int16_t)r[4]*(int32_t)(int16_t)r[7]);goto L8c02a4b0; /* muls.w r4,r7 */
L8c02a4b0: *stop=0x8c02a4b0u;if(!valid_access(r[3],2u))return -3;r[2]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c02a4b2; /* mov.w @r3,r2 */
L8c02a4b2: *stop=0x8c02a4b2u;if(!valid_access(r[0]+r[13],2u))return -3;wr(m,bus,&io_status,r[0]+r[13],r[2],2);if(io_status)return io_status;goto L8c02a4b4; /* mov.w r2,@(r0,r13) */
L8c02a4b4: *stop=0x8c02a4b4u;r[0]=42u;goto L8c02a4b6; /* mov #0x2a,r0 */
L8c02a4b6: *stop=0x8c02a4b6u;r[4]=*macl;goto L8c02a4b8; /* sts MACL,r4 */
L8c02a4b8: *stop=0x8c02a4b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a554u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a4ba; /* mov.l 0x8c02a554,r2 */
L8c02a4ba: *stop=0x8c02a4bau;if(!valid_access(r[0]+r[13],2u))return -3;wr(m,bus,&io_status,r[0]+r[13],r[7],2);if(io_status)return io_status;goto L8c02a4bc; /* mov.w r7,@(r0,r13) */
L8c02a4bc: *stop=0x8c02a4bcu;r[4]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c02a4be; /* exts.w r4,r4 */
L8c02a4be: *stop=0x8c02a4beu;r[4]+=r[2];goto L8c02a4c0; /* add r2,r4 */
L8c02a4c0: *stop=0x8c02a4c0u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a4c2; /* mov.l @r4,r3 */
L8c02a4c2: *stop=0x8c02a4c2u;if(!valid_access(r[13]+52u,4u))return -3;wr(m,bus,&io_status,r[13]+0x34u,r[3],4);if(io_status)return io_status;goto L8c02a4c4; /* mov.l r3,@(0x34,r13) */
L8c02a4c4: *stop=0x8c02a4c4u;r[3]=r[13];goto L8c02a4c6; /* mov r13,r3 */
L8c02a4c6: *stop=0x8c02a4c6u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a4c8; /* mov.l @(0x4,r4),r1 */
L8c02a4c8: *stop=0x8c02a4c8u;r[3]+=60u;goto L8c02a4ca; /* add #0x3c,r3 */
L8c02a4ca: *stop=0x8c02a4cau;if(!valid_access(r[13]+56u,4u))return -3;wr(m,bus,&io_status,r[13]+0x38u,r[1],4);if(io_status)return io_status;goto L8c02a4cc; /* mov.l r1,@(0x38,r13) */
L8c02a4cc: *stop=0x8c02a4ccu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a4ce; /* mov.l @(0x8,r4),r1 */
L8c02a4ce: *stop=0x8c02a4ceu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c02a4d0; /* mov.l r1,@r3 */
L8c02a4d0: *stop=0x8c02a4d0u;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a4d2; /* mov.l @(0xc,r4),r1 */
L8c02a4d2: *stop=0x8c02a4d2u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c02a4d4; /* mov.l r1,@(0x4,r3) */
L8c02a4d4: *stop=0x8c02a4d4u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a4d6; /* mov.l @(0x10,r4),r1 */
L8c02a4d6: *stop=0x8c02a4d6u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c02a4d8; /* mov.l r1,@(0x8,r3) */
L8c02a4d8: *stop=0x8c02a4d8u;r[3]=r[13];goto L8c02a4da; /* mov r13,r3 */
L8c02a4da: *stop=0x8c02a4dau;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a4dc; /* mov.l @(0x14,r4),r1 */
L8c02a4dc: *stop=0x8c02a4dcu;r[3]+=72u;goto L8c02a4de; /* add #0x48,r3 */
L8c02a4de: *stop=0x8c02a4deu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c02a4e0; /* mov.l r1,@r3 */
L8c02a4e0: *stop=0x8c02a4e0u;if(!valid_access(r[4]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a4e2; /* mov.l @(0x18,r4),r1 */
L8c02a4e2: *stop=0x8c02a4e2u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c02a4e4; /* mov.l r1,@(0x4,r3) */
L8c02a4e4: *stop=0x8c02a4e4u;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a4e6; /* mov.l @(0x1c,r4),r1 */
L8c02a4e6: *stop=0x8c02a4e6u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c02a4e8; /* mov.l r1,@(0x8,r3) */
L8c02a4e8: *stop=0x8c02a4e8u;r[3]=r[4];goto L8c02a4ea; /* mov r4,r3 */
L8c02a4ea: *stop=0x8c02a4eau;r[3]+=56u;goto L8c02a4ec; /* add #0x38,r3 */
L8c02a4ec: *stop=0x8c02a4ecu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a4ee; /* mov.l @r3,r0 */
L8c02a4ee: *stop=0x8c02a4eeu;r[1]=r[13];goto L8c02a4f0; /* mov r13,r1 */
L8c02a4f0: *stop=0x8c02a4f0u;r[1]+=108u;goto L8c02a4f2; /* add #0x6c,r1 */
L8c02a4f2: *stop=0x8c02a4f2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c02a4f4; /* mov.l r0,@r1 */
L8c02a4f4: *stop=0x8c02a4f4u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a4f6; /* mov.l @(0x4,r3),r0 */
L8c02a4f6: *stop=0x8c02a4f6u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[0],4);if(io_status)return io_status;goto L8c02a4f8; /* mov.l r0,@(0x4,r1) */
L8c02a4f8: *stop=0x8c02a4f8u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a4fa; /* mov.l @(0x8,r3),r0 */
L8c02a4fa: *stop=0x8c02a4fau;r[3]=r[4];goto L8c02a4fc; /* mov r4,r3 */
L8c02a4fc: *stop=0x8c02a4fcu;r[3]+=68u;goto L8c02a4fe; /* add #0x44,r3 */
L8c02a4fe: *stop=0x8c02a4feu;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[0],4);if(io_status)return io_status;goto L8c02a500; /* mov.l r0,@(0x8,r1) */
L8c02a500: *stop=0x8c02a500u;r[1]=r[13];goto L8c02a502; /* mov r13,r1 */
L8c02a502: *stop=0x8c02a502u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a504; /* mov.l @r3,r0 */
L8c02a504: *stop=0x8c02a504u;r[1]+=120u;goto L8c02a506; /* add #0x78,r1 */
L8c02a506: *stop=0x8c02a506u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c02a508; /* mov.l r0,@r1 */
L8c02a508: *stop=0x8c02a508u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a50a; /* mov.l @(0x4,r3),r0 */
L8c02a50a: *stop=0x8c02a50au;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[0],4);if(io_status)return io_status;goto L8c02a50c; /* mov.l r0,@(0x4,r1) */
L8c02a50c: *stop=0x8c02a50cu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a50e; /* mov.l @(0x8,r3),r0 */
L8c02a50e: *stop=0x8c02a50eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a550u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a510; /* mov.l 0x8c02a550,r3 */
L8c02a510: *stop=0x8c02a510u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[0],4);if(io_status)return io_status;goto L8c02a512; /* mov.l r0,@(0x8,r1) */
L8c02a512: *stop=0x8c02a512u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a514; /* mov.l @r3,r0 */
L8c02a514: *stop=0x8c02a514u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c02a516; /* cmp/eq #0x1,r0 */
L8c02a516: if(!(*sr&1u))goto L8c02a560;goto L8c02a518; /* bf 0x8c02a560 */
L8c02a518: *stop=0x8c02a518u;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a51a; /* mov.l @(0x20,r4),r1 */
L8c02a51a: *stop=0x8c02a51au;r[0]=r[13];goto L8c02a51c; /* mov r13,r0 */
L8c02a51c: *stop=0x8c02a51cu;r[0]+=84u;goto L8c02a51e; /* add #0x54,r0 */
L8c02a51e: *stop=0x8c02a51eu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c02a520; /* mov.l r1,@r0 */
L8c02a520: *stop=0x8c02a520u;if(!valid_access(r[4]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x24u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a522; /* mov.l @(0x24,r4),r1 */
L8c02a522: *stop=0x8c02a522u;if(!valid_access(r[0]+4u,4u))return -3;wr(m,bus,&io_status,r[0]+0x4u,r[1],4);if(io_status)return io_status;goto L8c02a524; /* mov.l r1,@(0x4,r0) */
L8c02a524: *stop=0x8c02a524u;if(!valid_access(r[4]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x28u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02a526; /* mov.l @(0x28,r4),r1 */
L8c02a526: *stop=0x8c02a526u;if(!valid_access(r[0]+8u,4u))return -3;wr(m,bus,&io_status,r[0]+0x8u,r[1],4);if(io_status)return io_status;goto L8c02a528; /* mov.l r1,@(0x8,r0) */
L8c02a528: *stop=0x8c02a528u;r[1]=r[13];goto L8c02a52a; /* mov r13,r1 */
L8c02a52a: *stop=0x8c02a52au;if(!valid_access(r[4]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x2cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a52c; /* mov.l @(0x2c,r4),r0 */
L8c02a52c: *stop=0x8c02a52cu;r[1]+=96u;goto L8c02a52e; /* add #0x60,r1 */
L8c02a52e: *stop=0x8c02a52eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c02a530; /* mov.l r0,@r1 */
L8c02a530: *stop=0x8c02a530u;if(!valid_access(r[4]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x30u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a532; /* mov.l @(0x30,r4),r0 */
L8c02a532: *stop=0x8c02a532u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[0],4);if(io_status)return io_status;goto L8c02a534; /* mov.l r0,@(0x4,r1) */
L8c02a534: *stop=0x8c02a534u;if(!valid_access(r[4]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x34u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a536; /* mov.l @(0x34,r4),r0 */
L8c02a536: *stop=0x8c02a536u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[0],4);if(io_status)return io_status;goto L8c02a538; /* mov.l r0,@(0x8,r1) */
L8c02a538: *stop=0x8c02a53au;goto L8c02a630; /* bra 0x8c02a630 */
L8c02a560: *stop=0x8c02a560u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a664u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c02a562; /* mov.l 0x8c02a664,r4 */
L8c02a562: *stop=0x8c02a562u;r[1]=r[13];goto L8c02a564; /* mov r13,r1 */
L8c02a564: *stop=0x8c02a564u;r[1]+=84u;goto L8c02a566; /* add #0x54,r1 */
L8c02a566: *stop=0x8c02a566u;r[0]=r[4];goto L8c02a568; /* mov r4,r0 */
L8c02a568: *stop=0x8c02a568u;r[0]+=100u;goto L8c02a56a; /* add #0x64,r0 */
L8c02a56a: *stop=0x8c02a56au;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a56c; /* mov.l @r0,r2 */
L8c02a56c: *stop=0x8c02a56cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c02a56e; /* mov.l r2,@r1 */
L8c02a56e: *stop=0x8c02a56eu;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a570; /* mov.l @(0x4,r0),r2 */
L8c02a570: *stop=0x8c02a570u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[2],4);if(io_status)return io_status;goto L8c02a572; /* mov.l r2,@(0x4,r1) */
L8c02a572: *stop=0x8c02a572u;if(!valid_access(r[0]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a574; /* mov.l @(0x8,r0),r2 */
L8c02a574: *stop=0x8c02a574u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[2],4);if(io_status)return io_status;goto L8c02a576; /* mov.l r2,@(0x8,r1) */
L8c02a576: *stop=0x8c02a576u;r[2]=r[4];goto L8c02a578; /* mov r4,r2 */
L8c02a578: *stop=0x8c02a578u;r[2]+=112u;goto L8c02a57a; /* add #0x70,r2 */
L8c02a57a: *stop=0x8c02a57au;r[1]=r[13];goto L8c02a57c; /* mov r13,r1 */
L8c02a57c: *stop=0x8c02a57cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a57e; /* mov.l @r2,r0 */
L8c02a57e: *stop=0x8c02a57eu;r[1]+=96u;goto L8c02a580; /* add #0x60,r1 */
L8c02a580: *stop=0x8c02a580u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c02a582; /* mov.l r0,@r1 */
L8c02a582: *stop=0x8c02a582u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a584; /* mov.l @(0x4,r2),r0 */
L8c02a584: *stop=0x8c02a584u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[0],4);if(io_status)return io_status;goto L8c02a586; /* mov.l r0,@(0x4,r1) */
L8c02a586: *stop=0x8c02a586u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c02a588; /* mov.l @(0x8,r2),r0 */
L8c02a588: *stop=0x8c02a588u;r[2]=sx(m,bus,&io_status,0x8c02a660u,2);if(io_status)return io_status;goto L8c02a58a; /* mov.w 0x8c02a660,r2 */
L8c02a58a: *stop=0x8c02a58au;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[0],4);if(io_status)return io_status;goto L8c02a58c; /* mov.l r0,@(0x8,r1) */
L8c02a58c: *stop=0x8c02a58cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c02a58e; /* mov.l @r3,r5 */
L8c02a58e: *stop=0x8c02a58eu;r[5]+=4294967295u;goto L8c02a590; /* add #-0x1,r5 */
L8c02a590: *stop=0x8c02a590u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c02a592; /* cmp/pl r5 */
L8c02a592: take=!(*sr&1u);*stop=0x8c02a594u;r[4]+=r[2];if(take)goto L8c02a5f4;goto L8c02a596; /* bf/s 0x8c02a5f4 */
L8c02a596: *stop=0x8c02a596u;r[1]=100u;goto L8c02a598; /* mov #0x64,r1 */
L8c02a598: *stop=0x8c02a598u;r[1]+=r[4];goto L8c02a59a; /* add r4,r1 */
L8c02a59a: *stop=0x8c02a59au;r[0]=84u;goto L8c02a59c; /* mov #0x54,r0 */
L8c02a59c: *stop=0x8c02a59cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c02a59e; /* fmov.s @r1,fr3 */
L8c02a59e: *stop=0x8c02a59eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a5a0; /* fmov.s @(r0,r13),fr2 */
L8c02a5a0: *stop=0x8c02a5a0u;r[1]=104u;goto L8c02a5a2; /* mov #0x68,r1 */
L8c02a5a2: *stop=0x8c02a5a2u;r[1]+=r[4];goto L8c02a5a4; /* add r4,r1 */
L8c02a5a4: *stop=0x8c02a5a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a5a6; /* fadd fr3,fr2 */
L8c02a5a6: *stop=0x8c02a5a6u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a5a8; /* fmov fr2,@(r0,r13) */
L8c02a5a8: *stop=0x8c02a5a8u;r[0]=88u;goto L8c02a5aa; /* mov #0x58,r0 */
L8c02a5aa: *stop=0x8c02a5aau;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a5ac; /* fmov.s @(r0,r13),fr2 */
L8c02a5ac: *stop=0x8c02a5acu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c02a5ae; /* fmov.s @r1,fr3 */
L8c02a5ae: *stop=0x8c02a5aeu;r[1]=108u;goto L8c02a5b0; /* mov #0x6c,r1 */
L8c02a5b0: *stop=0x8c02a5b0u;r[1]+=r[4];goto L8c02a5b2; /* add r4,r1 */
L8c02a5b2: *stop=0x8c02a5b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a5b4; /* fadd fr3,fr2 */
L8c02a5b4: *stop=0x8c02a5b4u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a5b6; /* fmov fr2,@(r0,r13) */
L8c02a5b6: *stop=0x8c02a5b6u;r[0]=92u;goto L8c02a5b8; /* mov #0x5c,r0 */
L8c02a5b8: *stop=0x8c02a5b8u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a5ba; /* fmov.s @(r0,r13),fr2 */
L8c02a5ba: *stop=0x8c02a5bau;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c02a5bc; /* fmov.s @r1,fr3 */
L8c02a5bc: *stop=0x8c02a5bcu;r[1]=112u;goto L8c02a5be; /* mov #0x70,r1 */
L8c02a5be: *stop=0x8c02a5beu;r[1]+=r[4];goto L8c02a5c0; /* add r4,r1 */
L8c02a5c0: *stop=0x8c02a5c0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a5c2; /* fadd fr3,fr2 */
L8c02a5c2: *stop=0x8c02a5c2u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a5c4; /* fmov fr2,@(r0,r13) */
L8c02a5c4: *stop=0x8c02a5c4u;r[0]=96u;goto L8c02a5c6; /* mov #0x60,r0 */
L8c02a5c6: *stop=0x8c02a5c6u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a5c8; /* fmov.s @(r0,r13),fr2 */
L8c02a5c8: *stop=0x8c02a5c8u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c02a5ca; /* fmov.s @r1,fr3 */
L8c02a5ca: *stop=0x8c02a5cau;r[1]=116u;goto L8c02a5cc; /* mov #0x74,r1 */
L8c02a5cc: *stop=0x8c02a5ccu;r[1]+=r[4];goto L8c02a5ce; /* add r4,r1 */
L8c02a5ce: *stop=0x8c02a5ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a5d0; /* fadd fr3,fr2 */
L8c02a5d0: *stop=0x8c02a5d0u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a5d2; /* fmov fr2,@(r0,r13) */
L8c02a5d2: *stop=0x8c02a5d2u;r[0]=100u;goto L8c02a5d4; /* mov #0x64,r0 */
L8c02a5d4: *stop=0x8c02a5d4u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c02a5d6; /* fmov.s @r1,fr3 */
L8c02a5d6: *stop=0x8c02a5d6u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a5d8; /* fmov.s @(r0,r13),fr2 */
L8c02a5d8: *stop=0x8c02a5d8u;r[1]=120u;goto L8c02a5da; /* mov #0x78,r1 */
L8c02a5da: *stop=0x8c02a5dau;r[1]+=r[4];goto L8c02a5dc; /* add r4,r1 */
L8c02a5dc: *stop=0x8c02a5dcu;r[3]=sx(m,bus,&io_status,0x8c02a660u,2);if(io_status)return io_status;goto L8c02a5de; /* mov.w 0x8c02a660,r3 */
L8c02a5de: *stop=0x8c02a5deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a5e0; /* fadd fr3,fr2 */
L8c02a5e0: *stop=0x8c02a5e0u;r[5]+=4294967295u;goto L8c02a5e2; /* add #-0x1,r5 */
L8c02a5e2: *stop=0x8c02a5e2u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c02a5e4; /* cmp/pl r5 */
L8c02a5e4: *stop=0x8c02a5e4u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a5e6; /* fmov fr2,@(r0,r13) */
L8c02a5e6: *stop=0x8c02a5e6u;r[0]=104u;goto L8c02a5e8; /* mov #0x68,r0 */
L8c02a5e8: *stop=0x8c02a5e8u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a5ea; /* fmov.s @(r0,r13),fr2 */
L8c02a5ea: *stop=0x8c02a5eau;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c02a5ec; /* fmov.s @r1,fr3 */
L8c02a5ec: *stop=0x8c02a5ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c02a5ee; /* fadd fr3,fr2 */
L8c02a5ee: *stop=0x8c02a5eeu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a5f0; /* fmov fr2,@(r0,r13) */
L8c02a5f0: take=(*sr&1u);*stop=0x8c02a5f2u;r[4]+=r[3];if(take)goto L8c02a596;goto L8c02a5f4; /* bt/s 0x8c02a596 */
L8c02a5f4: *stop=0x8c02a5f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02a668u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02a5f6; /* mov.l 0x8c02a668,r2 */
L8c02a5f6: *stop=0x8c02a5f6u;r[0]=84u;goto L8c02a5f8; /* mov #0x54,r0 */
L8c02a5f8: *stop=0x8c02a5f8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c02a5fa; /* fldi1 fr4 */
L8c02a5fa: *stop=0x8c02a5fau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c02a5fc; /* mov.l @r2,r3 */
L8c02a5fc: *stop=0x8c02a5fcu;*fpul=r[3];goto L8c02a5fe; /* lds r3,FPUL */
L8c02a5fe: *stop=0x8c02a5feu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c02a600; /* float FPUL,fr3 */
L8c02a600: *stop=0x8c02a600u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c02a602; /* fdiv fr3,fr4 */
L8c02a602: *stop=0x8c02a602u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a604; /* fmov.s @(r0,r13),fr3 */
L8c02a604: *stop=0x8c02a604u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c02a606; /* fmul fr4,fr3 */
L8c02a606: *stop=0x8c02a606u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a608; /* fmov fr3,@(r0,r13) */
L8c02a608: *stop=0x8c02a608u;r[0]=88u;goto L8c02a60a; /* mov #0x58,r0 */
L8c02a60a: *stop=0x8c02a60au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a60c; /* fmov.s @(r0,r13),fr2 */
L8c02a60c: *stop=0x8c02a60cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c02a60e; /* fmul fr4,fr2 */
L8c02a60e: *stop=0x8c02a60eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a610; /* fmov fr2,@(r0,r13) */
L8c02a610: *stop=0x8c02a610u;r[0]=92u;goto L8c02a612; /* mov #0x5c,r0 */
L8c02a612: *stop=0x8c02a612u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a614; /* fmov.s @(r0,r13),fr3 */
L8c02a614: *stop=0x8c02a614u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c02a616; /* fmul fr4,fr3 */
L8c02a616: *stop=0x8c02a616u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a618; /* fmov fr3,@(r0,r13) */
L8c02a618: *stop=0x8c02a618u;r[0]=96u;goto L8c02a61a; /* mov #0x60,r0 */
L8c02a61a: *stop=0x8c02a61au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a61c; /* fmov.s @(r0,r13),fr2 */
L8c02a61c: *stop=0x8c02a61cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c02a61e; /* fmul fr4,fr2 */
L8c02a61e: *stop=0x8c02a61eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a620; /* fmov fr2,@(r0,r13) */
L8c02a620: *stop=0x8c02a620u;r[0]=100u;goto L8c02a622; /* mov #0x64,r0 */
L8c02a622: *stop=0x8c02a622u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a624; /* fmov.s @(r0,r13),fr3 */
L8c02a624: *stop=0x8c02a624u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c02a626; /* fmul fr4,fr3 */
L8c02a626: *stop=0x8c02a626u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a628; /* fmov fr3,@(r0,r13) */
L8c02a628: *stop=0x8c02a628u;r[0]=104u;goto L8c02a62a; /* mov #0x68,r0 */
L8c02a62a: *stop=0x8c02a62au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a62c; /* fmov.s @(r0,r13),fr2 */
L8c02a62c: *stop=0x8c02a62cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c02a62e; /* fmul fr4,fr2 */
L8c02a62e: *stop=0x8c02a62eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c02a630; /* fmov fr2,@(r0,r13) */
L8c02a630: *stop=0x8c02a630u;r[0]=1u;goto L8c02a632; /* mov #0x1,r0 */
L8c02a632: *stop=0x8c02a632u;r[1]=sx(m,bus,&io_status,0x8c02a662u,2);if(io_status)return io_status;goto L8c02a634; /* mov.w 0x8c02a662,r1 */
L8c02a634: *stop=0x8c02a634u;r[15]+=r[1];goto L8c02a636; /* add r1,r15 */
L8c02a636: *stop=0x8c02a636u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c02a638; /* lds.l @r15+,PR */
L8c02a638: *stop=0x8c02a638u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02a63a; /* fmov.s @r15+,fr12 */
L8c02a63a: *stop=0x8c02a63au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02a63c; /* fmov.s @r15+,fr13 */
L8c02a63c: *stop=0x8c02a63cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02a63e; /* fmov.s @r15+,fr14 */
L8c02a63e: *stop=0x8c02a63eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02a640; /* fmov.s @r15+,fr15 */
L8c02a640: *stop=0x8c02a640u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c02a642; /* mov.l @r15+,r8 */
L8c02a642: *stop=0x8c02a642u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c02a644; /* mov.l @r15+,r9 */
L8c02a644: *stop=0x8c02a644u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c02a646; /* mov.l @r15+,r10 */
L8c02a646: *stop=0x8c02a646u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c02a648; /* mov.l @r15+,r11 */
L8c02a648: *stop=0x8c02a648u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c02a64a; /* mov.l @r15+,r12 */
L8c02a64a: *stop=0x8c02a64au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c02a64c; /* mov.l @r15+,r13 */
L8c02a64c: *stop=0x8c02a64cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c02a64eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_collision_contact_8c029b00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029b3a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029b3au);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029c76(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029c76u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029c84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029c84u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029c8c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029c8cu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029d08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029d08u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029d10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029d10u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029d24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029d24u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029d32(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029d32u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029d3e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029d3eu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029d56(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029d56u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029d64(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029d64u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029d6c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029d6cu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029d74(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029d74u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029d80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029d80u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029d8c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029d8cu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029dcc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029dccu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c029e48(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c029e48u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a0cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a0ccu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a0d2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a0d2u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a0dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a0dcu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a0e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a0e2u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a19c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a19cu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a1ae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a1aeu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a1bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a1bcu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a1dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a1dcu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a21c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a21cu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a2ee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a2eeu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a31e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a31eu);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a384(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a384u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a3aa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a3aau);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a406(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a406u);}
SA_STAGE_EXPORT int sa_collision_contact_resume_8c02a49c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_contact_run_8c029b00(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02a49cu);}
