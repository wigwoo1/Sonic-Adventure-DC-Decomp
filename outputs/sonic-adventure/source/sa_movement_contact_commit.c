/* Original contact position commit and character heading/velocity response.
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
static int sa_movement_contact_commit_run_8c074afc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c074b66u)goto L8c074b66;if(resume_pc==0x8c074b72u)goto L8c074b72;if(resume_pc==0x8c074b7eu)goto L8c074b7e;if(resume_pc==0x8c074baeu)goto L8c074bae;if(resume_pc==0x8c074bbeu)goto L8c074bbe;if(resume_pc==0x8c074bcau)goto L8c074bca;if(resume_pc==0x8c074be2u)goto L8c074be2;if(resume_pc==0x8c074c08u)goto L8c074c08;if(resume_pc==0x8c074c14u)goto L8c074c14;if(resume_pc==0x8c074c20u)goto L8c074c20;if(resume_pc==0x8c074c2cu)goto L8c074c2c;if(resume_pc==0x8c074c46u)goto L8c074c46;if(resume_pc==0x8c074c90u)goto L8c074c90;goto L8c074afc;
L8c074afc: *stop=0x8c074afcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c074afe; /* mov.l r14,@-r15 */
L8c074afe: *stop=0x8c074afeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c074b00; /* mov.l r13,@-r15 */
L8c074b00: *stop=0x8c074b00u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c074b02; /* mov.l r12,@-r15 */
L8c074b02: *stop=0x8c074b02u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c074b04; /* mov.l r11,@-r15 */
L8c074b04: *stop=0x8c074b04u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c074b06; /* mov.l r10,@-r15 */
L8c074b06: *stop=0x8c074b06u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c074b08; /* mov.l r9,@-r15 */
L8c074b08: *stop=0x8c074b08u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c074b0a; /* mov.l r8,@-r15 */
L8c074b0a: *stop=0x8c074b0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c074b0c; /* sts.l PR,@-r15 */
L8c074b0c: *stop=0x8c074b0cu;r[15]+=4294967284u;goto L8c074b0e; /* add #-0xc,r15 */
L8c074b0e: *stop=0x8c074b0eu;r[3]=r[7];goto L8c074b10; /* mov r7,r3 */
L8c074b10: *stop=0x8c074b10u;r[3]+=72u;goto L8c074b12; /* add #0x48,r3 */
L8c074b12: *stop=0x8c074b12u;r[2]=r[6];goto L8c074b14; /* mov r6,r2 */
L8c074b14: *stop=0x8c074b14u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c074b16; /* mov.l @(0x4,r3),r0 */
L8c074b16: *stop=0x8c074b16u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c074b18; /* mov.l @r3,r1 */
L8c074b18: *stop=0x8c074b18u;r[2]+=68u;goto L8c074b1a; /* add #0x44,r2 */
L8c074b1a: *stop=0x8c074b1au;r[13]=r[4];goto L8c074b1c; /* mov r4,r13 */
L8c074b1c: *stop=0x8c074b1cu;r[8]=64u;goto L8c074b1e; /* mov #0x40,r8 */
L8c074b1e: *stop=0x8c074b1eu;r[9]=1u;goto L8c074b20; /* mov #0x1,r9 */
L8c074b20: *stop=0x8c074b20u;if(!valid_access(r[15]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x2cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c074b22; /* mov.l @(0x2c,r15),r4 */
L8c074b22: *stop=0x8c074b22u;if(!valid_access(r[15]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x34u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c074b24; /* mov.l @(0x34,r15),r12 */
L8c074b24: *stop=0x8c074b24u;r[8]<<=8;goto L8c074b26; /* shll8 r8 */
L8c074b26: *stop=0x8c074b26u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c074b28; /* mov.l r1,@r2 */
L8c074b28: *stop=0x8c074b28u;r[11]=r[5];goto L8c074b2a; /* mov r5,r11 */
L8c074b2a: *stop=0x8c074b2au;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c074b2c; /* mov.l r0,@(0x4,r2) */
L8c074b2c: *stop=0x8c074b2cu;r[10]=r[6];goto L8c074b2e; /* mov r6,r10 */
L8c074b2e: *stop=0x8c074b2eu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c074b30; /* mov.l @(0x8,r3),r1 */
L8c074b30: *stop=0x8c074b30u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c074b32; /* mov.l r1,@(0x8,r2) */
L8c074b32: *stop=0x8c074b32u;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c074b34; /* mov.w @(0x4,r13),r0 */
L8c074b34: *stop=0x8c074b34u;*sr=(*sr&~1u)|!(r[9]&r[0]);goto L8c074b36; /* tst r9,r0 */
L8c074b36: take=(*sr&1u);*stop=0x8c074b38u;r[14]=r[7];if(take)goto L8c074be6;goto L8c074b3a; /* bt/s 0x8c074be6 */
L8c074b3a: *stop=0x8c074b3au;r[0]=32u;goto L8c074b3c; /* mov #0x20,r0 */
L8c074b3c: *stop=0x8c074b3cu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c074b3e; /* fmov.s @r4,fr2 */
L8c074b3e: *stop=0x8c074b3eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c074b40; /* fmov.s @(r0,r13),fr3 */
L8c074b40: *stop=0x8c074b40u;r[0]=36u;goto L8c074b42; /* mov #0x24,r0 */
L8c074b42: *stop=0x8c074b42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c64u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074b44; /* mov.l 0x8c074c64,r3 */
L8c074b44: *stop=0x8c074b44u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074b46; /* fsub fr3,fr2 */
L8c074b46: *stop=0x8c074b46u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c074b48; /* fmov.s fr2,@r15 */
L8c074b48: *stop=0x8c074b48u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c074b4a; /* fmov.s @(r0,r13),fr3 */
L8c074b4a: *stop=0x8c074b4au;r[0]=4u;goto L8c074b4c; /* mov #0x4,r0 */
L8c074b4c: *stop=0x8c074b4cu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c074b4e; /* fmov.s @(r0,r4),fr2 */
L8c074b4e: *stop=0x8c074b4eu;r[0]=4u;goto L8c074b50; /* mov #0x4,r0 */
L8c074b50: *stop=0x8c074b50u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074b52; /* fsub fr3,fr2 */
L8c074b52: *stop=0x8c074b52u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c074b54; /* fmov fr2,@(r0,r15) */
L8c074b54: *stop=0x8c074b54u;r[0]=40u;goto L8c074b56; /* mov #0x28,r0 */
L8c074b56: *stop=0x8c074b56u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c074b58; /* fmov.s @(r0,r13),fr3 */
L8c074b58: *stop=0x8c074b58u;r[0]=8u;goto L8c074b5a; /* mov #0x8,r0 */
L8c074b5a: *stop=0x8c074b5au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c074b5c; /* fmov.s @(r0,r4),fr2 */
L8c074b5c: *stop=0x8c074b5cu;r[0]=8u;goto L8c074b5e; /* mov #0x8,r0 */
L8c074b5e: *stop=0x8c074b5eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074b60; /* fsub fr3,fr2 */
L8c074b60: *stop=0x8c074b60u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c074b62; /* fmov fr2,@(r0,r15) */
L8c074b62: target=r[3];*pr=0x8c074b66u;*stop=0x8c074b64u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074b66; /* jsr @r3 */
L8c074b66: *stop=0x8c074b66u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c074b68; /* fldi0 fr3 */
L8c074b68: *stop=0x8c074b68u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c074b6a; /* fcmp/eq fr3,fr0 */
L8c074b6a: if((*sr&1u))goto L8c074b8c;goto L8c074b6c; /* bt 0x8c074b8c */
L8c074b6c: *stop=0x8c074b6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c68u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074b6e; /* mov.l 0x8c074c68,r2 */
L8c074b6e: target=r[2];*pr=0x8c074b72u;*stop=0x8c074b70u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074b72; /* jsr @r2 */
L8c074b72: *stop=0x8c074b72u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c6cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074b74; /* mov.l 0x8c074c6c,r3 */
L8c074b74: *stop=0x8c074b74u;r[5]=r[14];goto L8c074b76; /* mov r14,r5 */
L8c074b76: *stop=0x8c074b76u;r[6]=0u;goto L8c074b78; /* mov #0x0,r6 */
L8c074b78: *stop=0x8c074b78u;r[5]+=72u;goto L8c074b7a; /* add #0x48,r5 */
L8c074b7a: target=r[3];*pr=0x8c074b7eu;*stop=0x8c074b7cu;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074b7e; /* jsr @r3 */
L8c074b7e: *stop=0x8c074b7eu;r[4]=r[0];goto L8c074b80; /* mov r0,r4 */
L8c074b80: *stop=0x8c074b80u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[8]);goto L8c074b82; /* cmp/ge r8,r4 */
L8c074b82: if(!(*sr&1u))goto L8c074b88;goto L8c074b84; /* bf 0x8c074b88 */
L8c074b84: *stop=0x8c074b86u;goto L8c074cda; /* bra 0x8c074cda */
L8c074b88: *stop=0x8c074b8au;goto L8c074bd4; /* bra 0x8c074bd4 */
L8c074b8c: *stop=0x8c074b8cu;r[0]=4u;goto L8c074b8e; /* mov #0x4,r0 */
L8c074b8e: *stop=0x8c074b8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c64u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074b90; /* mov.l 0x8c074c64,r3 */
L8c074b90: *stop=0x8c074b90u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c074b92; /* fmov.s @(r0,r11),fr2 */
L8c074b92: *stop=0x8c074b92u;r[0]=8u;goto L8c074b94; /* mov #0x8,r0 */
L8c074b94: *stop=0x8c074b94u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c074b96; /* fneg fr2 */
L8c074b96: *stop=0x8c074b96u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c074b98; /* fmov.s fr2,@r15 */
L8c074b98: *stop=0x8c074b98u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c074b9a; /* fmov.s @(r0,r11),fr2 */
L8c074b9a: *stop=0x8c074b9au;r[0]=4u;goto L8c074b9c; /* mov #0x4,r0 */
L8c074b9c: *stop=0x8c074b9cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c074b9e; /* fneg fr2 */
L8c074b9e: *stop=0x8c074b9eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c074ba0; /* fmov fr2,@(r0,r15) */
L8c074ba0: *stop=0x8c074ba0u;r[0]=12u;goto L8c074ba2; /* mov #0xc,r0 */
L8c074ba2: *stop=0x8c074ba2u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c074ba4; /* fmov.s @(r0,r11),fr2 */
L8c074ba4: *stop=0x8c074ba4u;r[0]=8u;goto L8c074ba6; /* mov #0x8,r0 */
L8c074ba6: *stop=0x8c074ba6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c074ba8; /* fneg fr2 */
L8c074ba8: *stop=0x8c074ba8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c074baa; /* fmov fr2,@(r0,r15) */
L8c074baa: target=r[3];*pr=0x8c074baeu;*stop=0x8c074bacu;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074bae; /* jsr @r3 */
L8c074bae: *stop=0x8c074baeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c074bb0; /* fldi0 fr3 */
L8c074bb0: *stop=0x8c074bb0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c074bb2; /* fcmp/eq fr3,fr0 */
L8c074bb2: if(!(*sr&1u))goto L8c074bb8;goto L8c074bb4; /* bf 0x8c074bb8 */
L8c074bb4: *stop=0x8c074bb6u;goto L8c074cda; /* bra 0x8c074cda */
L8c074bb8: *stop=0x8c074bb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c68u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074bba; /* mov.l 0x8c074c68,r2 */
L8c074bba: target=r[2];*pr=0x8c074bbeu;*stop=0x8c074bbcu;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074bbe; /* jsr @r2 */
L8c074bbe: *stop=0x8c074bbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c6cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074bc0; /* mov.l 0x8c074c6c,r3 */
L8c074bc0: *stop=0x8c074bc0u;r[5]=r[14];goto L8c074bc2; /* mov r14,r5 */
L8c074bc2: *stop=0x8c074bc2u;r[6]=0u;goto L8c074bc4; /* mov #0x0,r6 */
L8c074bc4: *stop=0x8c074bc4u;r[5]+=72u;goto L8c074bc6; /* add #0x48,r5 */
L8c074bc6: target=r[3];*pr=0x8c074bcau;*stop=0x8c074bc8u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074bca; /* jsr @r3 */
L8c074bca: *stop=0x8c074bcau;r[4]=r[0];goto L8c074bcc; /* mov r0,r4 */
L8c074bcc: *stop=0x8c074bccu;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[8]);goto L8c074bce; /* cmp/ge r8,r4 */
L8c074bce: if(!(*sr&1u))goto L8c074bd4;goto L8c074bd0; /* bf 0x8c074bd4 */
L8c074bd0: *stop=0x8c074bd2u;goto L8c074cda; /* bra 0x8c074cda */
L8c074bd4: *stop=0x8c074bd4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c70u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074bd6; /* mov.l 0x8c074c70,r3 */
L8c074bd6: *stop=0x8c074bd6u;r[5]=r[10];goto L8c074bd8; /* mov r10,r5 */
L8c074bd8: *stop=0x8c074bd8u;r[6]=r[14];goto L8c074bda; /* mov r14,r6 */
L8c074bda: *stop=0x8c074bdau;r[12]=r[9];goto L8c074bdc; /* mov r9,r12 */
L8c074bdc: *stop=0x8c074bdcu;r[7]=r[15];goto L8c074bde; /* mov r15,r7 */
L8c074bde: target=r[3];*pr=0x8c074be2u;*stop=0x8c074be0u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074be2; /* jsr @r3 */
L8c074be2: *stop=0x8c074be4u;goto L8c074c90; /* bra 0x8c074c90 */
L8c074be6: *stop=0x8c074be6u;r[0]=4u;goto L8c074be8; /* mov #0x4,r0 */
L8c074be8: *stop=0x8c074be8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c64u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074bea; /* mov.l 0x8c074c64,r3 */
L8c074bea: *stop=0x8c074beau;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c074bec; /* fmov.s @(r0,r11),fr3 */
L8c074bec: *stop=0x8c074becu;r[0]=8u;goto L8c074bee; /* mov #0x8,r0 */
L8c074bee: *stop=0x8c074beeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c074bf0; /* fneg fr3 */
L8c074bf0: *stop=0x8c074bf0u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c074bf2; /* fmov.s fr3,@r15 */
L8c074bf2: *stop=0x8c074bf2u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c074bf4; /* fmov.s @(r0,r11),fr3 */
L8c074bf4: *stop=0x8c074bf4u;r[0]=4u;goto L8c074bf6; /* mov #0x4,r0 */
L8c074bf6: *stop=0x8c074bf6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c074bf8; /* fneg fr3 */
L8c074bf8: *stop=0x8c074bf8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c074bfa; /* fmov fr3,@(r0,r15) */
L8c074bfa: *stop=0x8c074bfau;r[0]=12u;goto L8c074bfc; /* mov #0xc,r0 */
L8c074bfc: *stop=0x8c074bfcu;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c074bfe; /* fmov.s @(r0,r11),fr3 */
L8c074bfe: *stop=0x8c074bfeu;r[0]=8u;goto L8c074c00; /* mov #0x8,r0 */
L8c074c00: *stop=0x8c074c00u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c074c02; /* fneg fr3 */
L8c074c02: *stop=0x8c074c02u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c074c04; /* fmov fr3,@(r0,r15) */
L8c074c04: target=r[3];*pr=0x8c074c08u;*stop=0x8c074c06u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074c08; /* jsr @r3 */
L8c074c08: *stop=0x8c074c08u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c074c0a; /* fldi0 fr3 */
L8c074c0a: *stop=0x8c074c0au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c074c0c; /* fcmp/eq fr3,fr0 */
L8c074c0c: if((*sr&1u))goto L8c074c90;goto L8c074c0e; /* bt 0x8c074c90 */
L8c074c0e: *stop=0x8c074c0eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c68u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074c10; /* mov.l 0x8c074c68,r2 */
L8c074c10: target=r[2];*pr=0x8c074c14u;*stop=0x8c074c12u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074c14; /* jsr @r2 */
L8c074c14: *stop=0x8c074c14u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c6cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074c16; /* mov.l 0x8c074c6c,r3 */
L8c074c16: *stop=0x8c074c16u;r[5]=r[14];goto L8c074c18; /* mov r14,r5 */
L8c074c18: *stop=0x8c074c18u;r[6]=0u;goto L8c074c1a; /* mov #0x0,r6 */
L8c074c1a: *stop=0x8c074c1au;r[5]+=72u;goto L8c074c1c; /* add #0x48,r5 */
L8c074c1c: target=r[3];*pr=0x8c074c20u;*stop=0x8c074c1eu;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074c20; /* jsr @r3 */
L8c074c20: *stop=0x8c074c20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c74u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074c22; /* mov.l 0x8c074c74,r2 */
L8c074c22: *stop=0x8c074c22u;r[5]=r[14];goto L8c074c24; /* mov r14,r5 */
L8c074c24: *stop=0x8c074c24u;r[11]=r[0];goto L8c074c26; /* mov r0,r11 */
L8c074c26: *stop=0x8c074c26u;r[5]+=72u;goto L8c074c28; /* add #0x48,r5 */
L8c074c28: target=r[2];*pr=0x8c074c2cu;*stop=0x8c074c2au;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074c2c; /* jsr @r2 */
L8c074c2c: *stop=0x8c074c2cu;if(!valid_access(r[15]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074c2e; /* mov.l @(0x38,r15),r2 */
L8c074c2e: *stop=0x8c074c2eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c78u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074c30; /* mov.l 0x8c074c78,r3 */
L8c074c30: *stop=0x8c074c30u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c074c32; /* tst r3,r2 */
L8c074c32: if((*sr&1u))goto L8c074c80;goto L8c074c34; /* bt 0x8c074c80 */
L8c074c34: *stop=0x8c074c34u;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[8]);goto L8c074c36; /* cmp/ge r8,r11 */
L8c074c36: if((*sr&1u))goto L8c074cda;goto L8c074c38; /* bt 0x8c074cda */
L8c074c38: *stop=0x8c074c38u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c70u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c074c3a; /* mov.l 0x8c074c70,r1 */
L8c074c3a: *stop=0x8c074c3au;r[5]=r[10];goto L8c074c3c; /* mov r10,r5 */
L8c074c3c: *stop=0x8c074c3cu;r[6]=r[14];goto L8c074c3e; /* mov r14,r6 */
L8c074c3e: *stop=0x8c074c3eu;r[12]=r[9];goto L8c074c40; /* mov r9,r12 */
L8c074c40: *stop=0x8c074c40u;r[7]=r[15];goto L8c074c42; /* mov r15,r7 */
L8c074c42: target=r[1];*pr=0x8c074c46u;*stop=0x8c074c44u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074c46; /* jsr @r1 */
L8c074c46: *stop=0x8c074c48u;goto L8c074c90; /* bra 0x8c074c90 */
L8c074c80: *stop=0x8c074c80u;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[8]);goto L8c074c82; /* cmp/ge r8,r11 */
L8c074c82: if((*sr&1u))goto L8c074cda;goto L8c074c84; /* bt 0x8c074cda */
L8c074c84: *stop=0x8c074c84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074de0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074c86; /* mov.l 0x8c074de0,r2 */
L8c074c86: *stop=0x8c074c86u;r[5]=r[10];goto L8c074c88; /* mov r10,r5 */
L8c074c88: *stop=0x8c074c88u;r[7]=r[15];goto L8c074c8a; /* mov r15,r7 */
L8c074c8a: *stop=0x8c074c8au;r[6]=r[14];goto L8c074c8c; /* mov r14,r6 */
L8c074c8c: target=r[2];*pr=0x8c074c90u;*stop=0x8c074c8eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074c90; /* jsr @r2 */
L8c074c90: *stop=0x8c074c90u;r[0]=96u;goto L8c074c92; /* mov #0x60,r0 */
L8c074c92: *stop=0x8c074c92u;r[3]=r[14];goto L8c074c94; /* mov r14,r3 */
L8c074c94: *stop=0x8c074c94u;r[3]+=96u;goto L8c074c96; /* add #0x60,r3 */
L8c074c96: *stop=0x8c074c96u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c074c98; /* mov.l @r3,r2 */
L8c074c98: *stop=0x8c074c98u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c074c9a; /* mov.l @(0x4,r3),r1 */
L8c074c9a: *stop=0x8c074c9au;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[2],4);if(io_status)return io_status;goto L8c074c9c; /* mov.l r2,@(0x4,r14) */
L8c074c9c: *stop=0x8c074c9cu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[1],4);if(io_status)return io_status;goto L8c074c9e; /* mov.l r1,@(0x8,r14) */
L8c074c9e: *stop=0x8c074c9eu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074ca0; /* mov.l @(0x8,r3),r2 */
L8c074ca0: *stop=0x8c074ca0u;r[3]=r[14];goto L8c074ca2; /* mov r14,r3 */
L8c074ca2: *stop=0x8c074ca2u;r[3]+=108u;goto L8c074ca4; /* add #0x6c,r3 */
L8c074ca4: *stop=0x8c074ca4u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[2],4);if(io_status)return io_status;goto L8c074ca6; /* mov.l r2,@(0xc,r14) */
L8c074ca6: *stop=0x8c074ca6u;if(!valid_access(r[15]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x30u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c074ca8; /* mov.l @(0x30,r15),r4 */
L8c074ca8: *stop=0x8c074ca8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074caa; /* fmov.s @(r0,r14),fr2 */
L8c074caa: *stop=0x8c074caau;r[0]=32u;goto L8c074cac; /* mov #0x20,r0 */
L8c074cac: *stop=0x8c074cacu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c074cae; /* fmov.s @r4,fr3 */
L8c074cae: *stop=0x8c074caeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074cb0; /* fsub fr3,fr2 */
L8c074cb0: *stop=0x8c074cb0u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074cb2; /* fmov fr2,@(r0,r13) */
L8c074cb2: *stop=0x8c074cb2u;r[0]=4u;goto L8c074cb4; /* mov #0x4,r0 */
L8c074cb4: *stop=0x8c074cb4u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c074cb6; /* fmov.s @(r0,r4),fr3 */
L8c074cb6: *stop=0x8c074cb6u;r[0]=100u;goto L8c074cb8; /* mov #0x64,r0 */
L8c074cb8: *stop=0x8c074cb8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074cba; /* fmov.s @(r0,r14),fr2 */
L8c074cba: *stop=0x8c074cbau;r[0]=36u;goto L8c074cbc; /* mov #0x24,r0 */
L8c074cbc: *stop=0x8c074cbcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074cbe; /* fsub fr3,fr2 */
L8c074cbe: *stop=0x8c074cbeu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074cc0; /* fmov fr2,@(r0,r13) */
L8c074cc0: *stop=0x8c074cc0u;r[0]=8u;goto L8c074cc2; /* mov #0x8,r0 */
L8c074cc2: *stop=0x8c074cc2u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c074cc4; /* fmov.s @(r0,r4),fr3 */
L8c074cc4: *stop=0x8c074cc4u;r[0]=104u;goto L8c074cc6; /* mov #0x68,r0 */
L8c074cc6: *stop=0x8c074cc6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074cc8; /* fmov.s @(r0,r14),fr2 */
L8c074cc8: *stop=0x8c074cc8u;r[0]=40u;goto L8c074cca; /* mov #0x28,r0 */
L8c074cca: *stop=0x8c074ccau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074ccc; /* fsub fr3,fr2 */
L8c074ccc: *stop=0x8c074cccu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074cce; /* fmov fr2,@(r0,r13) */
L8c074cce: *stop=0x8c074cceu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c074cd0; /* mov.l @r3,r2 */
L8c074cd0: *stop=0x8c074cd0u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c074cd2; /* mov.l @(0x4,r3),r1 */
L8c074cd2: *stop=0x8c074cd2u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c074cd4; /* mov.l r2,@(0x10,r14) */
L8c074cd4: *stop=0x8c074cd4u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[1],4);if(io_status)return io_status;goto L8c074cd6; /* mov.l r1,@(0x14,r14) */
L8c074cd6: *stop=0x8c074cd6u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074cd8; /* mov.l @(0x8,r3),r2 */
L8c074cd8: *stop=0x8c074cd8u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[2],4);if(io_status)return io_status;goto L8c074cda; /* mov.l r2,@(0x18,r14) */
L8c074cda: *stop=0x8c074cdau;r[0]=r[12];goto L8c074cdc; /* mov r12,r0 */
L8c074cdc: *stop=0x8c074cdcu;r[15]+=12u;goto L8c074cde; /* add #0xc,r15 */
L8c074cde: *stop=0x8c074cdeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c074ce0; /* lds.l @r15+,PR */
L8c074ce0: *stop=0x8c074ce0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c074ce2; /* mov.l @r15+,r8 */
L8c074ce2: *stop=0x8c074ce2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c074ce4; /* mov.l @r15+,r9 */
L8c074ce4: *stop=0x8c074ce4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c074ce6; /* mov.l @r15+,r10 */
L8c074ce6: *stop=0x8c074ce6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c074ce8; /* mov.l @r15+,r11 */
L8c074ce8: *stop=0x8c074ce8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c074cea; /* mov.l @r15+,r12 */
L8c074cea: *stop=0x8c074ceau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c074cec; /* mov.l @r15+,r13 */
L8c074cec: *stop=0x8c074cecu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c074ceeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_contact_commit_8c074afc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c074b66(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074b66u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c074b72(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074b72u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c074b7e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074b7eu);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c074bae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074baeu);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c074bbe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074bbeu);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c074bca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074bcau);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c074be2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074be2u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c074c08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074c08u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c074c14(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074c14u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c074c20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074c20u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c074c2c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074c2cu);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c074c46(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074c46u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c074c90(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c074afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074c90u);}

static int sa_movement_contact_commit_run_8c076640(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c076666u)goto L8c076666;if(resume_pc==0x8c076682u)goto L8c076682;if(resume_pc==0x8c0766b4u)goto L8c0766b4;if(resume_pc==0x8c0766c2u)goto L8c0766c2;if(resume_pc==0x8c0766c8u)goto L8c0766c8;if(resume_pc==0x8c0766d8u)goto L8c0766d8;if(resume_pc==0x8c0766f0u)goto L8c0766f0;if(resume_pc==0x8c076726u)goto L8c076726;if(resume_pc==0x8c07673cu)goto L8c07673c;if(resume_pc==0x8c076752u)goto L8c076752;goto L8c076640;
L8c076640: *stop=0x8c076640u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c076642; /* mov.l r14,@-r15 */
L8c076642: *stop=0x8c076642u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c076644; /* mov.l r13,@-r15 */
L8c076644: *stop=0x8c076644u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c076646; /* mov.l r12,@-r15 */
L8c076646: *stop=0x8c076646u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c076648; /* mov.l r11,@-r15 */
L8c076648: *stop=0x8c076648u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c07664a; /* mov.l r10,@-r15 */
L8c07664a: *stop=0x8c07664au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c07664c; /* fmov.s fr15,@-r15 */
L8c07664c: *stop=0x8c07664cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c07664e; /* sts.l PR,@-r15 */
L8c07664e: *stop=0x8c07664eu;r[15]+=4294967256u;goto L8c076650; /* add #-0x28,r15 */
L8c076650: *stop=0x8c076650u;r[12]=r[5];goto L8c076652; /* mov r5,r12 */
L8c076652: *stop=0x8c076652u;r[14]=r[4];goto L8c076654; /* mov r4,r14 */
L8c076654: *stop=0x8c076654u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c076656; /* mov.l r7,@r15 */
L8c076656: *stop=0x8c076656u;if(!valid_access(r[12]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x10u,2);if(io_status)return io_status;goto L8c076658; /* mov.w @(0x10,r12),r0 */
L8c076658: *stop=0x8c076658u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c07665a; /* tst #0x3,r0 */
L8c07665a: take=!(*sr&1u);*stop=0x8c07665cu;r[13]=r[6];if(take)goto L8c076674;goto L8c07665e; /* bf/s 0x8c076674 */
L8c07665e: *stop=0x8c07665eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076880u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c076660; /* mov.l 0x8c076880,r2 */
L8c076660: *stop=0x8c076660u;r[4]=r[12];goto L8c076662; /* mov r12,r4 */
L8c076662: target=r[2];*pr=0x8c076666u;*stop=0x8c076664u;r[4]+=56u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c076666; /* jsr @r2 */
L8c076666: *stop=0x8c076666u;r[0]=4294967232u;goto L8c076668; /* mov #-0x40,r0 */
L8c076668: *stop=0x8c076668u;r[0]=r[0]&255u;goto L8c07666a; /* extu.b r0,r0 */
L8c07666a: *stop=0x8c07666au;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c07666c; /* fmov.s @(r0,r12),fr3 */
L8c07666c: *stop=0x8c07666cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c07666e; /* fcmp/gt fr3,fr0 */
L8c07666e: if((*sr&1u))goto L8c076674;goto L8c076670; /* bt 0x8c076674 */
L8c076670: *stop=0x8c076672u;goto L8c076770; /* bra 0x8c076770 */
L8c076674: *stop=0x8c076674u;r[5]=r[13];goto L8c076676; /* mov r13,r5 */
L8c076676: *stop=0x8c076676u;{uint32_t loaded=rd(m,bus,&io_status,0x8c076884u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c076678; /* mov.l 0x8c076884,r3 */
L8c076678: *stop=0x8c076678u;r[6]=r[15];goto L8c07667a; /* mov r15,r6 */
L8c07667a: *stop=0x8c07667au;r[5]+=72u;goto L8c07667c; /* add #0x48,r5 */
L8c07667c: *stop=0x8c07667cu;r[6]+=28u;goto L8c07667e; /* add #0x1c,r6 */
L8c07667e: target=r[3];*pr=0x8c076682u;*stop=0x8c076680u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c076682; /* jsr @r3 */
L8c076682: *stop=0x8c076682u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c076684; /* fldi0 fr3 */
L8c076684: *stop=0x8c076684u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c076686; /* fcmp/eq fr3,fr0 */
L8c076686: if((*sr&1u))goto L8c076770;goto L8c076688; /* bt 0x8c076770 */
L8c076688: *stop=0x8c076688u;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07668a; /* mov.l @(0x10,r13),r3 */
L8c07668a: *stop=0x8c07668au;r[2]=r[15];goto L8c07668c; /* mov r15,r2 */
L8c07668c: *stop=0x8c07668cu;r[2]+=16u;goto L8c07668e; /* add #0x10,r2 */
L8c07668e: *stop=0x8c07668eu;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c076690; /* mov.l @(0x14,r13),r1 */
L8c076690: *stop=0x8c076690u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c076692; /* mov.l r3,@r2 */
L8c076692: *stop=0x8c076692u;r[0]=r[13];goto L8c076694; /* mov r13,r0 */
L8c076694: *stop=0x8c076694u;r[0]+=108u;goto L8c076696; /* add #0x6c,r0 */
L8c076696: *stop=0x8c076696u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c076698; /* mov.l r1,@(0x4,r2) */
L8c076698: *stop=0x8c076698u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07669a; /* mov.l @(0x18,r13),r3 */
L8c07669a: *stop=0x8c07669au;r[4]=r[15];goto L8c07669c; /* mov r15,r4 */
L8c07669c: *stop=0x8c07669cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076880u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c07669e; /* mov.l 0x8c076880,r11 */
L8c07669e: *stop=0x8c07669eu;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c0766a0; /* mov.l r3,@(0x8,r2) */
L8c0766a0: *stop=0x8c0766a0u;r[3]=r[15];goto L8c0766a2; /* mov r15,r3 */
L8c0766a2: *stop=0x8c0766a2u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0766a4; /* mov.l @r0,r2 */
L8c0766a4: *stop=0x8c0766a4u;r[3]+=4u;goto L8c0766a6; /* add #0x4,r3 */
L8c0766a6: *stop=0x8c0766a6u;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0766a8; /* mov.l @(0x4,r0),r1 */
L8c0766a8: *stop=0x8c0766a8u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c0766aa; /* mov.l r2,@r3 */
L8c0766aa: *stop=0x8c0766aau;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c0766ac; /* mov.l r1,@(0x4,r3) */
L8c0766ac: *stop=0x8c0766acu;if(!valid_access(r[0]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0766ae; /* mov.l @(0x8,r0),r2 */
L8c0766ae: *stop=0x8c0766aeu;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[2],4);if(io_status)return io_status;goto L8c0766b0; /* mov.l r2,@(0x8,r3) */
L8c0766b0: target=r[11];*pr=0x8c0766b4u;*stop=0x8c0766b2u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0766b4; /* jsr @r11 */
L8c0766b4: *stop=0x8c0766b4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0766b6; /* fldi0 fr3 */
L8c0766b6: *stop=0x8c0766b6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c0766b8; /* fcmp/eq fr3,fr0 */
L8c0766b8: if((*sr&1u))goto L8c076770;goto L8c0766ba; /* bt 0x8c076770 */
L8c0766ba: *stop=0x8c0766bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c076888u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c0766bc; /* mov.l 0x8c076888,r10 */
L8c0766bc: *stop=0x8c0766bcu;r[4]=r[15];goto L8c0766be; /* mov r15,r4 */
L8c0766be: target=r[10];*pr=0x8c0766c2u;*stop=0x8c0766c0u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0766c2; /* jsr @r10 */
L8c0766c2: *stop=0x8c0766c2u;r[4]=r[15];goto L8c0766c4; /* mov r15,r4 */
L8c0766c4: target=r[10];*pr=0x8c0766c8u;*stop=0x8c0766c6u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0766c8; /* jsr @r10 */
L8c0766c8: *stop=0x8c0766c8u;r[5]=r[15];goto L8c0766ca; /* mov r15,r5 */
L8c0766ca: *stop=0x8c0766cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c07688cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0766cc; /* mov.l 0x8c07688c,r2 */
L8c0766cc: *stop=0x8c0766ccu;r[6]=r[13];goto L8c0766ce; /* mov r13,r6 */
L8c0766ce: *stop=0x8c0766ceu;r[4]=r[15];goto L8c0766d0; /* mov r15,r4 */
L8c0766d0: *stop=0x8c0766d0u;r[6]+=28u;goto L8c0766d2; /* add #0x1c,r6 */
L8c0766d2: *stop=0x8c0766d2u;r[5]+=4u;goto L8c0766d4; /* add #0x4,r5 */
L8c0766d4: target=r[2];*pr=0x8c0766d8u;*stop=0x8c0766d6u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0766d8; /* jsr @r2 */
L8c0766d8: *stop=0x8c0766d8u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0766da; /* mov.l @(0x18,r14),r3 */
L8c0766da: *stop=0x8c0766dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c076890u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c0766dc; /* mov.l 0x8c076890,r12 */
L8c0766dc: *stop=0x8c0766dcu;r[3]-=r[0];goto L8c0766de; /* sub r0,r3 */
L8c0766de: *stop=0x8c0766deu;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[3],4);if(io_status)return io_status;goto L8c0766e0; /* mov.l r3,@(0x18,r14) */
L8c0766e0: *stop=0x8c0766e0u;r[5]=r[3];goto L8c0766e2; /* mov r3,r5 */
L8c0766e2: *stop=0x8c0766e2u;if(!valid_access(r[14]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x8u,1);if(io_status)return io_status;goto L8c0766e4; /* mov.b @(0x8,r14),r0 */
L8c0766e4: *stop=0x8c0766e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c076894u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0766e6; /* mov.l 0x8c076894,r3 */
L8c0766e6: *stop=0x8c0766e6u;r[0]=r[0]&255u;goto L8c0766e8; /* extu.b r0,r0 */
L8c0766e8: *stop=0x8c0766e8u;r[0]<<=2;goto L8c0766ea; /* shll2 r0 */
L8c0766ea: *stop=0x8c0766eau;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c0766ec; /* mov.l @(r0,r12),r4 */
L8c0766ec: target=r[3];*pr=0x8c0766f0u;*stop=0x8c0766eeu;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0766f0; /* jsr @r3 */
L8c0766f0: *stop=0x8c0766f0u;r[2]=96u;goto L8c0766f2; /* mov #0x60,r2 */
L8c0766f2: *stop=0x8c0766f2u;r[2]<<=8;goto L8c0766f4; /* shll8 r2 */
L8c0766f4: *stop=0x8c0766f4u;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[2]);goto L8c0766f6; /* cmp/gt r2,r0 */
L8c0766f6: if(!(*sr&1u))goto L8c076700;goto L8c0766f8; /* bf 0x8c076700 */
L8c0766f8: *stop=0x8c0766f8u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0766fa; /* mov.l @(0x18,r14),r1 */
L8c0766fa: *stop=0x8c0766fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c076898u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0766fc; /* mov.l 0x8c076898,r3 */
L8c0766fc: *stop=0x8c0766fcu;r[1]+=r[3];goto L8c0766fe; /* add r3,r1 */
L8c0766fe: *stop=0x8c0766feu;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[1],4);if(io_status)return io_status;goto L8c076700; /* mov.l r1,@(0x18,r14) */
L8c076700: *stop=0x8c076700u;if(!valid_access(r[14]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x8u,1);if(io_status)return io_status;goto L8c076702; /* mov.b @(0x8,r14),r0 */
L8c076702: *stop=0x8c076702u;r[1]=r[15];goto L8c076704; /* mov r15,r1 */
L8c076704: *stop=0x8c076704u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c076706; /* mov.l @(0x18,r14),r2 */
L8c076706: *stop=0x8c076706u;r[1]+=28u;goto L8c076708; /* add #0x1c,r1 */
L8c076708: *stop=0x8c076708u;r[0]=r[0]&255u;goto L8c07670a; /* extu.b r0,r0 */
L8c07670a: *stop=0x8c07670au;r[4]=r[15];goto L8c07670c; /* mov r15,r4 */
L8c07670c: *stop=0x8c07670cu;r[0]<<=2;goto L8c07670e; /* shll2 r0 */
L8c07670e: *stop=0x8c07670eu;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c076710; /* mov.l @(r0,r12),r3 */
L8c076710: *stop=0x8c076710u;if(!valid_access(r[3]+32u,4u))return -3;wr(m,bus,&io_status,r[3]+0x20u,r[2],4);if(io_status)return io_status;goto L8c076712; /* mov.l r2,@(0x20,r3) */
L8c076712: *stop=0x8c076712u;r[3]=r[13];goto L8c076714; /* mov r13,r3 */
L8c076714: *stop=0x8c076714u;r[3]+=108u;goto L8c076716; /* add #0x6c,r3 */
L8c076716: *stop=0x8c076716u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c076718; /* mov.l @r3,r0 */
L8c076718: *stop=0x8c076718u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07671a; /* mov.l @(0x4,r3),r2 */
L8c07671a: *stop=0x8c07671au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c07671c; /* mov.l r0,@r1 */
L8c07671c: *stop=0x8c07671cu;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[2],4);if(io_status)return io_status;goto L8c07671e; /* mov.l r2,@(0x4,r1) */
L8c07671e: *stop=0x8c07671eu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c076720; /* mov.l @(0x8,r3),r0 */
L8c076720: *stop=0x8c076720u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[0],4);if(io_status)return io_status;goto L8c076722; /* mov.l r0,@(0x8,r1) */
L8c076722: target=r[11];*pr=0x8c076726u;*stop=0x8c076724u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c076726; /* jsr @r11 */
L8c076726: *stop=0x8c076726u;r[0]=0x8c07689cu;goto L8c076728; /* mova 0x8c07689c,r0 */
L8c076728: *stop=0x8c076728u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c07672a; /* fmov.s @r0,fr3 */
L8c07672a: *stop=0x8c07672au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c07672c; /* fcmp/gt fr0,fr3 */
L8c07672c: if((*sr&1u))goto L8c076770;goto L8c07672e; /* bt 0x8c076770 */
L8c07672e: *stop=0x8c07672eu;if(!valid_access(r[14]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x8u,1);if(io_status)return io_status;goto L8c076730; /* mov.b @(0x8,r14),r0 */
L8c076730: *stop=0x8c076730u;r[0]=r[0]&255u;goto L8c076732; /* extu.b r0,r0 */
L8c076732: *stop=0x8c076732u;r[0]<<=2;goto L8c076734; /* shll2 r0 */
L8c076734: *stop=0x8c076734u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[14]=loaded;}goto L8c076736; /* mov.l @(r0,r12),r14 */
L8c076736: *stop=0x8c076736u;r[4]=r[14];goto L8c076738; /* mov r14,r4 */
L8c076738: target=r[11];*pr=0x8c07673cu;*stop=0x8c07673au;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07673c; /* jsr @r11 */
L8c07673c: *stop=0x8c07673cu;r[0]=0x8c0768a0u;goto L8c07673e; /* mova 0x8c0768a0,r0 */
L8c07673e: *stop=0x8c07673eu;fcopy(f,15u,0u);goto L8c076740; /* fmov fr0,fr15 */
L8c076740: *stop=0x8c076740u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c076742; /* fmov.s @r0,fr3 */
L8c076742: *stop=0x8c076742u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c076744; /* fcmp/gt fr3,fr15 */
L8c076744: if(!(*sr&1u))goto L8c07674c;goto L8c076746; /* bf 0x8c07674c */
L8c076746: *stop=0x8c076746u;r[0]=0x8c0768a4u;goto L8c076748; /* mova 0x8c0768a4,r0 */
L8c076748: *stop=0x8c076748u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c07674a; /* fmov.s @r0,fr2 */
L8c07674a: *stop=0x8c07674au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[2],&f->fpscr,&fr[15]);if(status)return status;}goto L8c07674c; /* fmul fr2,fr15 */
L8c07674c: *stop=0x8c07674cu;r[4]=r[15];goto L8c07674e; /* mov r15,r4 */
L8c07674e: target=r[10];*pr=0x8c076752u;*stop=0x8c076750u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c076752; /* jsr @r10 */
L8c076752: *stop=0x8c076752u;r[0]=28u;goto L8c076754; /* mov #0x1c,r0 */
L8c076754: *stop=0x8c076754u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076756; /* fmov.s @(r0,r15),fr3 */
L8c076756: *stop=0x8c076756u;r[0]=4u;goto L8c076758; /* mov #0x4,r0 */
L8c076758: *stop=0x8c076758u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c07675a; /* fmul fr15,fr3 */
L8c07675a: *stop=0x8c07675au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c07675c; /* fmov fr3,@(r0,r14) */
L8c07675c: *stop=0x8c07675cu;r[0]=32u;goto L8c07675e; /* mov #0x20,r0 */
L8c07675e: *stop=0x8c07675eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076760; /* fmov.s @(r0,r15),fr3 */
L8c076760: *stop=0x8c076760u;r[0]=8u;goto L8c076762; /* mov #0x8,r0 */
L8c076762: *stop=0x8c076762u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c076764; /* fmul fr15,fr3 */
L8c076764: *stop=0x8c076764u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c076766; /* fmov fr3,@(r0,r14) */
L8c076766: *stop=0x8c076766u;r[0]=36u;goto L8c076768; /* mov #0x24,r0 */
L8c076768: *stop=0x8c076768u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07676a; /* fmov.s @(r0,r15),fr3 */
L8c07676a: *stop=0x8c07676au;r[0]=12u;goto L8c07676c; /* mov #0xc,r0 */
L8c07676c: *stop=0x8c07676cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c07676e; /* fmul fr15,fr3 */
L8c07676e: *stop=0x8c07676eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c076770; /* fmov fr3,@(r0,r14) */
L8c076770: *stop=0x8c076770u;r[15]+=40u;goto L8c076772; /* add #0x28,r15 */
L8c076772: *stop=0x8c076772u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c076774; /* lds.l @r15+,PR */
L8c076774: *stop=0x8c076774u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c076776; /* fmov.s @r15+,fr15 */
L8c076776: *stop=0x8c076776u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c076778; /* mov.l @r15+,r10 */
L8c076778: *stop=0x8c076778u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c07677a; /* mov.l @r15+,r11 */
L8c07677a: *stop=0x8c07677au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c07677c; /* mov.l @r15+,r12 */
L8c07677c: *stop=0x8c07677cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c07677e; /* mov.l @r15+,r13 */
L8c07677e: *stop=0x8c07677eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c076780u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_contact_commit_8c076640(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c076640(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c076666(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c076640(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c076666u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c076682(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c076640(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c076682u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c0766b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c076640(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0766b4u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c0766c2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c076640(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0766c2u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c0766c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c076640(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0766c8u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c0766d8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c076640(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0766d8u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c0766f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c076640(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0766f0u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c076726(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c076640(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c076726u);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c07673c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c076640(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07673cu);}
SA_STAGE_EXPORT int sa_movement_contact_commit_resume_8c076752(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_commit_run_8c076640(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c076752u);}
