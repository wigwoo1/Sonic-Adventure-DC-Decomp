/* Original complete player child effect update,records,geometry and draw family.
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
static int sa_player_child_update_run_8c0b3b1a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c0b3b3au)goto L8c0b3b3a;if(resume_pc==0x8c0b3b48u)goto L8c0b3b48;if(resume_pc==0x8c0b3b7eu)goto L8c0b3b7e;if(resume_pc==0x8c0b3bbcu)goto L8c0b3bbc;if(resume_pc==0x8c0b3be6u)goto L8c0b3be6;if(resume_pc==0x8c0b3c2cu)goto L8c0b3c2c;if(resume_pc==0x8c0b3c56u)goto L8c0b3c56;if(resume_pc==0x8c0b3c62u)goto L8c0b3c62;if(resume_pc==0x8c0b3c7au)goto L8c0b3c7a;if(resume_pc==0x8c0b3c96u)goto L8c0b3c96;if(resume_pc==0x8c0b3ce4u)goto L8c0b3ce4;if(resume_pc==0x8c0b3e10u)goto L8c0b3e10;if(resume_pc==0x8c0b3e72u)goto L8c0b3e72;if(resume_pc==0x8c0b3eaeu)goto L8c0b3eae;if(resume_pc==0x8c0b3ec6u)goto L8c0b3ec6;if(resume_pc==0x8c0b3ee0u)goto L8c0b3ee0;goto L8c0b3b1a;
L8c0b3b1a: *stop=0x8c0b3b1au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0b3b1c; /* mov.l r14,@-r15 */
L8c0b3b1c: *stop=0x8c0b3b1cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0b3b1e; /* mov.l r13,@-r15 */
L8c0b3b1e: *stop=0x8c0b3b1eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c0b3b20; /* mov.l r12,@-r15 */
L8c0b3b20: *stop=0x8c0b3b20u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c0b3b22; /* mov.l r11,@-r15 */
L8c0b3b22: *stop=0x8c0b3b22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c0b3b24; /* mov.l r10,@-r15 */
L8c0b3b24: *stop=0x8c0b3b24u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c0b3b26; /* mov.l r9,@-r15 */
L8c0b3b26: *stop=0x8c0b3b26u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c0b3b28; /* fmov.s fr15,@-r15 */
L8c0b3b28: *stop=0x8c0b3b28u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c0b3b2a; /* fmov.s fr14,@-r15 */
L8c0b3b2a: *stop=0x8c0b3b2au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c0b3b2c; /* fmov.s fr13,@-r15 */
L8c0b3b2c: *stop=0x8c0b3b2cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c0b3b2e; /* fmov.s fr12,@-r15 */
L8c0b3b2e: *stop=0x8c0b3b2eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0b3b30; /* sts.l PR,@-r15 */
L8c0b3b30: *stop=0x8c0b3b30u;r[15]+=4294967200u;goto L8c0b3b32; /* add #-0x60,r15 */
L8c0b3b32: *stop=0x8c0b3b32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3c00u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3b34; /* mov.l 0x8c0b3c00,r3 */
L8c0b3b34: *stop=0x8c0b3b34u;r[14]=r[4];goto L8c0b3b36; /* mov r4,r14 */
L8c0b3b36: target=r[3];*pr=0x8c0b3b3au;*stop=0x8c0b3b38u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[13]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3b3a; /* jsr @r3 */
L8c0b3b3a: *stop=0x8c0b3b3au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0b3b3c; /* tst r0,r0 */
L8c0b3b3c: if((*sr&1u))goto L8c0b3b4c;goto L8c0b3b3e; /* bt 0x8c0b3b4c */
L8c0b3b3e: *stop=0x8c0b3b3eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3c0cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3b40; /* mov.l 0x8c0b3c0c,r3 */
L8c0b3b40: *stop=0x8c0b3b40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3c08u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0b3b42; /* mov.l 0x8c0b3c08,r4 */
L8c0b3b42: *stop=0x8c0b3b42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3c04u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0b3b44; /* mov.l 0x8c0b3c04,r5 */
L8c0b3b44: target=r[3];*pr=0x8c0b3b48u;*stop=0x8c0b3b46u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3b48; /* jsr @r3 */
L8c0b3b48: *stop=0x8c0b3b4au;goto L8c0b3f12; /* bra 0x8c0b3f12 */
L8c0b3b4c: *stop=0x8c0b3b4cu;r[0]=52u;goto L8c0b3b4e; /* mov #0x34,r0 */
L8c0b3b4e: *stop=0x8c0b3b4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3c10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0b3b50; /* mov.l 0x8c0b3c10,r4 */
L8c0b3b50: *stop=0x8c0b3b50u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0b3b52; /* fmov.s @(r0,r13),fr3 */
L8c0b3b52: *stop=0x8c0b3b52u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c0b3b54; /* ftrc fr3,FPUL */
L8c0b3b54: *stop=0x8c0b3b54u;r[0]=*fpul;goto L8c0b3b56; /* sts FPUL,r0 */
L8c0b3b56: *stop=0x8c0b3b56u;r[0]=(uint32_t)(int32_t)(int8_t)r[0];goto L8c0b3b58; /* exts.b r0,r0 */
L8c0b3b58: *stop=0x8c0b3b58u;r[0]<<=2;goto L8c0b3b5a; /* shll2 r0 */
L8c0b3b5a: *stop=0x8c0b3b5au;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3b5c; /* mov.l @(r0,r4),r3 */
L8c0b3b5c: *stop=0x8c0b3b5cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0b3b5e; /* tst r3,r3 */
L8c0b3b5e: if((*sr&1u))goto L8c0b3b78;goto L8c0b3b60; /* bt 0x8c0b3b78 */
L8c0b3b60: *stop=0x8c0b3b60u;if(!valid_access(r[13]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x3u,1);if(io_status)return io_status;goto L8c0b3b62; /* mov.b @(0x3,r13),r0 */
L8c0b3b62: *stop=0x8c0b3b62u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3b64; /* mov.l @(0x8,r13),r2 */
L8c0b3b64: *stop=0x8c0b3b64u;r[0]=r[0]&255u;goto L8c0b3b66; /* extu.b r0,r0 */
L8c0b3b66: *stop=0x8c0b3b66u;r[3]=r[0];goto L8c0b3b68; /* mov r0,r3 */
L8c0b3b68: *stop=0x8c0b3b68u;r[0]<<=2;goto L8c0b3b6a; /* shll2 r0 */
L8c0b3b6a: *stop=0x8c0b3b6au;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3b6c; /* shll r0 */
L8c0b3b6c: *stop=0x8c0b3b6cu;r[0]+=r[3];goto L8c0b3b6e; /* add r3,r0 */
L8c0b3b6e: *stop=0x8c0b3b6eu;r[0]<<=2;goto L8c0b3b70; /* shll2 r0 */
L8c0b3b70: *stop=0x8c0b3b70u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3b72; /* shll r0 */
L8c0b3b72: *stop=0x8c0b3b72u;r[0]+=r[2];goto L8c0b3b74; /* add r2,r0 */
L8c0b3b74: *stop=0x8c0b3b74u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0b3b76; /* tst r0,r0 */
L8c0b3b76: if(!(*sr&1u))goto L8c0b3b82;goto L8c0b3b78; /* bf 0x8c0b3b82 */
L8c0b3b78: *stop=0x8c0b3b78u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3c14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3b7a; /* mov.l 0x8c0b3c14,r2 */
L8c0b3b7a: target=r[2];*pr=0x8c0b3b7eu;*stop=0x8c0b3b7cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3b7e; /* jsr @r2 */
L8c0b3b7e: *stop=0x8c0b3b80u;goto L8c0b3f12; /* bra 0x8c0b3f12 */
L8c0b3b82: *stop=0x8c0b3b82u;r[0]=52u;goto L8c0b3b84; /* mov #0x34,r0 */
L8c0b3b84: *stop=0x8c0b3b84u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0b3b86; /* fmov.s @(r0,r13),fr3 */
L8c0b3b86: *stop=0x8c0b3b86u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c0b3b88; /* ftrc fr3,FPUL */
L8c0b3b88: *stop=0x8c0b3b88u;r[0]=*fpul;goto L8c0b3b8a; /* sts FPUL,r0 */
L8c0b3b8a: *stop=0x8c0b3b8au;r[0]=(uint32_t)(int32_t)(int8_t)r[0];goto L8c0b3b8c; /* exts.b r0,r0 */
L8c0b3b8c: *stop=0x8c0b3b8cu;r[0]<<=2;goto L8c0b3b8e; /* shll2 r0 */
L8c0b3b8e: *stop=0x8c0b3b8eu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c0b3b90; /* mov.l @(r0,r4),r0 */
L8c0b3b90: *stop=0x8c0b3b90u;if(!valid_access(r[0]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x1u,1);if(io_status)return io_status;goto L8c0b3b92; /* mov.b @(0x1,r0),r0 */
L8c0b3b92: *stop=0x8c0b3b92u;*sr=(*sr&~1u)|(r[0]==0x00000027u);goto L8c0b3b94; /* cmp/eq #0x27,r0 */
L8c0b3b94: if(!(*sr&1u))goto L8c0b3ba6;goto L8c0b3b96; /* bf 0x8c0b3ba6 */
L8c0b3b96: *stop=0x8c0b3b96u;if(!valid_access(r[13]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x1u,1);if(io_status)return io_status;goto L8c0b3b98; /* mov.b @(0x1,r13),r0 */
L8c0b3b98: *stop=0x8c0b3b98u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0b3b9a; /* tst r0,r0 */
L8c0b3b9a: if((*sr&1u))goto L8c0b3ba0;goto L8c0b3b9c; /* bt 0x8c0b3ba0 */
L8c0b3b9c: *stop=0x8c0b3b9eu;r[0]=1u;goto L8c0b3ba2; /* bra 0x8c0b3ba2 */
L8c0b3ba0: *stop=0x8c0b3ba0u;r[0]=2u;goto L8c0b3ba2; /* mov #0x2,r0 */
L8c0b3ba2: *stop=0x8c0b3ba4u;if(!valid_access(r[13]+3u,1u))return -3;wr(m,bus,&io_status,r[13]+0x3u,r[0],1);if(io_status)return io_status;goto L8c0b3baa; /* bra 0x8c0b3baa */
L8c0b3ba6: *stop=0x8c0b3ba6u;r[0]=0u;goto L8c0b3ba8; /* mov #0x0,r0 */
L8c0b3ba8: *stop=0x8c0b3ba8u;if(!valid_access(r[13]+3u,1u))return -3;wr(m,bus,&io_status,r[13]+0x3u,r[0],1);if(io_status)return io_status;goto L8c0b3baa; /* mov.b r0,@(0x3,r13) */
L8c0b3baa: *stop=0x8c0b3baau;if(!valid_access(r[13]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x1u,1);if(io_status)return io_status;goto L8c0b3bac; /* mov.b @(0x1,r13),r0 */
L8c0b3bac: *stop=0x8c0b3bacu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0b3bae; /* tst r0,r0 */
L8c0b3bae: if((*sr&1u))goto L8c0b3c20;goto L8c0b3bb0; /* bt 0x8c0b3c20 */
L8c0b3bb0: *stop=0x8c0b3bb0u;if(!valid_access(r[13]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x3u,1);if(io_status)return io_status;goto L8c0b3bb2; /* mov.b @(0x3,r13),r0 */
L8c0b3bb2: *stop=0x8c0b3bb2u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0b3bb4; /* tst r0,r0 */
L8c0b3bb4: if(!(*sr&1u))goto L8c0b3be2;goto L8c0b3bb6; /* bf 0x8c0b3be2 */
L8c0b3bb6: *stop=0x8c0b3bb6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3c18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3bb8; /* mov.l 0x8c0b3c18,r2 */
L8c0b3bb8: target=r[2];*pr=0x8c0b3bbcu;*stop=0x8c0b3bbau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3bbc; /* jsr @r2 */
L8c0b3bbc: *stop=0x8c0b3bbcu;*fpul=r[0];goto L8c0b3bbe; /* lds r0,FPUL */
L8c0b3bbe: *stop=0x8c0b3bbeu;r[0]=0x8c0b3c1cu;goto L8c0b3bc0; /* mova 0x8c0b3c1c,r0 */
L8c0b3bc0: *stop=0x8c0b3bc0u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c0b3bc2; /* fmov.s @r0,fr2 */
L8c0b3bc2: *stop=0x8c0b3bc2u;r[1]=52u;goto L8c0b3bc4; /* mov #0x34,r1 */
L8c0b3bc4: *stop=0x8c0b3bc4u;if(!valid_access(r[13]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x3u,1);if(io_status)return io_status;goto L8c0b3bc6; /* mov.b @(0x3,r13),r0 */
L8c0b3bc6: *stop=0x8c0b3bc6u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c0b3bc8; /* float FPUL,fr3 */
L8c0b3bc8: *stop=0x8c0b3bc8u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3bca; /* mov.l @(0x8,r13),r2 */
L8c0b3bca: *stop=0x8c0b3bcau;r[0]=r[0]&255u;goto L8c0b3bcc; /* extu.b r0,r0 */
L8c0b3bcc: *stop=0x8c0b3bccu;r[3]=r[0];goto L8c0b3bce; /* mov r0,r3 */
L8c0b3bce: *stop=0x8c0b3bceu;r[0]<<=2;goto L8c0b3bd0; /* shll2 r0 */
L8c0b3bd0: *stop=0x8c0b3bd0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3bd2; /* shll r0 */
L8c0b3bd2: *stop=0x8c0b3bd2u;r[0]+=r[3];goto L8c0b3bd4; /* add r3,r0 */
L8c0b3bd4: *stop=0x8c0b3bd4u;r[0]<<=2;goto L8c0b3bd6; /* shll2 r0 */
L8c0b3bd6: *stop=0x8c0b3bd6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0b3bd8; /* fmul fr2,fr3 */
L8c0b3bd8: *stop=0x8c0b3bd8u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3bda; /* shll r0 */
L8c0b3bda: *stop=0x8c0b3bdau;r[0]+=r[2];goto L8c0b3bdc; /* add r2,r0 */
L8c0b3bdc: *stop=0x8c0b3bdcu;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[1]);if(io_status)return io_status;goto L8c0b3bde; /* fmov.s @(r0,r1),fr1 */
L8c0b3bde: *stop=0x8c0b3bdeu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0b3be0; /* fcmp/gt fr3,fr1 */
L8c0b3be0: if(!(*sr&1u))goto L8c0b3be6;goto L8c0b3be2; /* bf 0x8c0b3be6 */
L8c0b3be2: target=0x8c0b37f2u;*pr=0x8c0b3be6u;*stop=0x8c0b3be4u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3be6; /* bsr 0x8c0b37f2 */
L8c0b3be6: *stop=0x8c0b3be6u;if(!valid_access(r[13]+6u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x6u,2);if(io_status)return io_status;goto L8c0b3be8; /* mov.w @(0x6,r13),r0 */
L8c0b3be8: *stop=0x8c0b3be8u;r[0]+=4294967295u;goto L8c0b3bea; /* add #-0x1,r0 */
L8c0b3bea: *stop=0x8c0b3beau;if(!valid_access(r[13]+6u,2u))return -3;wr(m,bus,&io_status,r[13]+0x6u,r[0],2);if(io_status)return io_status;goto L8c0b3bec; /* mov.w r0,@(0x6,r13) */
L8c0b3bec: *stop=0x8c0b3becu;r[0]+=1u;goto L8c0b3bee; /* add #0x1,r0 */
L8c0b3bee: *stop=0x8c0b3beeu;r[0]=r[0]&65535u;goto L8c0b3bf0; /* extu.w r0,r0 */
L8c0b3bf0: *stop=0x8c0b3bf0u;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c0b3bf2; /* cmp/pl r0 */
L8c0b3bf2: if((*sr&1u))goto L8c0b3c76;goto L8c0b3bf4; /* bt 0x8c0b3c76 */
L8c0b3bf4: *stop=0x8c0b3bf4u;r[0]=0u;goto L8c0b3bf6; /* mov #0x0,r0 */
L8c0b3bf6: *stop=0x8c0b3bf8u;if(!valid_access(r[13]+1u,1u))return -3;wr(m,bus,&io_status,r[13]+0x1u,r[0],1);if(io_status)return io_status;goto L8c0b3c76; /* bra 0x8c0b3c76 */
L8c0b3c20: *stop=0x8c0b3c20u;if(!valid_access(r[13]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x3u,1);if(io_status)return io_status;goto L8c0b3c22; /* mov.b @(0x3,r13),r0 */
L8c0b3c22: *stop=0x8c0b3c22u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0b3c24; /* tst r0,r0 */
L8c0b3c24: if((*sr&1u))goto L8c0b3c56;goto L8c0b3c26; /* bt 0x8c0b3c56 */
L8c0b3c26: *stop=0x8c0b3c26u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3e80u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3c28; /* mov.l 0x8c0b3e80,r2 */
L8c0b3c28: target=r[2];*pr=0x8c0b3c2cu;*stop=0x8c0b3c2au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3c2c; /* jsr @r2 */
L8c0b3c2c: *stop=0x8c0b3c2cu;*fpul=r[0];goto L8c0b3c2e; /* lds r0,FPUL */
L8c0b3c2e: *stop=0x8c0b3c2eu;r[0]=0x8c0b3e84u;goto L8c0b3c30; /* mova 0x8c0b3e84,r0 */
L8c0b3c30: *stop=0x8c0b3c30u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c0b3c32; /* fmov.s @r0,fr2 */
L8c0b3c32: *stop=0x8c0b3c32u;r[1]=52u;goto L8c0b3c34; /* mov #0x34,r1 */
L8c0b3c34: *stop=0x8c0b3c34u;if(!valid_access(r[13]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x3u,1);if(io_status)return io_status;goto L8c0b3c36; /* mov.b @(0x3,r13),r0 */
L8c0b3c36: *stop=0x8c0b3c36u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c0b3c38; /* float FPUL,fr3 */
L8c0b3c38: *stop=0x8c0b3c38u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3c3a; /* mov.l @(0x8,r13),r2 */
L8c0b3c3a: *stop=0x8c0b3c3au;r[0]=r[0]&255u;goto L8c0b3c3c; /* extu.b r0,r0 */
L8c0b3c3c: *stop=0x8c0b3c3cu;r[3]=r[0];goto L8c0b3c3e; /* mov r0,r3 */
L8c0b3c3e: *stop=0x8c0b3c3eu;r[0]<<=2;goto L8c0b3c40; /* shll2 r0 */
L8c0b3c40: *stop=0x8c0b3c40u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3c42; /* shll r0 */
L8c0b3c42: *stop=0x8c0b3c42u;r[0]+=r[3];goto L8c0b3c44; /* add r3,r0 */
L8c0b3c44: *stop=0x8c0b3c44u;r[0]<<=2;goto L8c0b3c46; /* shll2 r0 */
L8c0b3c46: *stop=0x8c0b3c46u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0b3c48; /* fmul fr2,fr3 */
L8c0b3c48: *stop=0x8c0b3c48u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3c4a; /* shll r0 */
L8c0b3c4a: *stop=0x8c0b3c4au;r[0]+=r[2];goto L8c0b3c4c; /* add r2,r0 */
L8c0b3c4c: *stop=0x8c0b3c4cu;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[1]);if(io_status)return io_status;goto L8c0b3c4e; /* fmov.s @(r0,r1),fr1 */
L8c0b3c4e: *stop=0x8c0b3c4eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0b3c50; /* fcmp/gt fr3,fr1 */
L8c0b3c50: if(!(*sr&1u))goto L8c0b3c56;goto L8c0b3c52; /* bf 0x8c0b3c56 */
L8c0b3c52: target=0x8c0b37f2u;*pr=0x8c0b3c56u;*stop=0x8c0b3c54u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3c56; /* bsr 0x8c0b37f2 */
L8c0b3c56: *stop=0x8c0b3c56u;r[0]=52u;goto L8c0b3c58; /* mov #0x34,r0 */
L8c0b3c58: *stop=0x8c0b3c58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3e88u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3c5a; /* mov.l 0x8c0b3e88,r3 */
L8c0b3c5a: *stop=0x8c0b3c5au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0b3c5c; /* fmov.s @(r0,r13),fr3 */
L8c0b3c5c: *stop=0x8c0b3c5cu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c0b3c5e; /* ftrc fr3,FPUL */
L8c0b3c5e: target=r[3];*pr=0x8c0b3c62u;*stop=0x8c0b3c60u;r[4]=*fpul;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3c62; /* jsr @r3 */
L8c0b3c62: *stop=0x8c0b3c62u;r[1]=76u;goto L8c0b3c64; /* mov #0x4c,r1 */
L8c0b3c64: *stop=0x8c0b3c64u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c0b3c66; /* fldi1 fr3 */
L8c0b3c66: *stop=0x8c0b3c66u;r[1]<<=2;goto L8c0b3c68; /* shll2 r1 */
L8c0b3c68: *stop=0x8c0b3c68u;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c0b3c6a; /* fmov.s @(r0,r1),fr2 */
L8c0b3c6a: *stop=0x8c0b3c6au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0b3c6c; /* fcmp/gt fr3,fr2 */
L8c0b3c6c: if((*sr&1u))goto L8c0b3c76;goto L8c0b3c6e; /* bt 0x8c0b3c76 */
L8c0b3c6e: *stop=0x8c0b3c6eu;r[0]=1u;goto L8c0b3c70; /* mov #0x1,r0 */
L8c0b3c70: *stop=0x8c0b3c70u;if(!valid_access(r[13]+1u,1u))return -3;wr(m,bus,&io_status,r[13]+0x1u,r[0],1);if(io_status)return io_status;goto L8c0b3c72; /* mov.b r0,@(0x1,r13) */
L8c0b3c72: *stop=0x8c0b3c72u;r[0]=40u;goto L8c0b3c74; /* mov #0x28,r0 */
L8c0b3c74: *stop=0x8c0b3c74u;if(!valid_access(r[13]+6u,2u))return -3;wr(m,bus,&io_status,r[13]+0x6u,r[0],2);if(io_status)return io_status;goto L8c0b3c76; /* mov.w r0,@(0x6,r13) */
L8c0b3c76: target=0x8c0b3f2cu;*pr=0x8c0b3c7au;*stop=0x8c0b3c78u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3c7a; /* bsr 0x8c0b3f2c */
L8c0b3c7a: *stop=0x8c0b3c7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3e8cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3c7c; /* mov.l 0x8c0b3e8c,r3 */
L8c0b3c7c: *stop=0x8c0b3c7cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3c7e; /* mov.l @r3,r2 */
L8c0b3c7e: *stop=0x8c0b3c7eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c0b3c80; /* tst r2,r2 */
L8c0b3c80: if((*sr&1u))goto L8c0b3c86;goto L8c0b3c82; /* bt 0x8c0b3c86 */
L8c0b3c82: *stop=0x8c0b3c84u;goto L8c0b3f12; /* bra 0x8c0b3f12 */
L8c0b3c86: *stop=0x8c0b3c86u;r[6]=r[15];goto L8c0b3c88; /* mov r15,r6 */
L8c0b3c88: *stop=0x8c0b3c88u;r[5]=r[15];goto L8c0b3c8a; /* mov r15,r5 */
L8c0b3c8a: *stop=0x8c0b3c8au;r[7]=r[15];goto L8c0b3c8c; /* mov r15,r7 */
L8c0b3c8c: *stop=0x8c0b3c8cu;r[5]+=84u;goto L8c0b3c8e; /* add #0x54,r5 */
L8c0b3c8e: *stop=0x8c0b3c8eu;r[7]+=60u;goto L8c0b3c90; /* add #0x3c,r7 */
L8c0b3c90: *stop=0x8c0b3c90u;r[6]+=72u;goto L8c0b3c92; /* add #0x48,r6 */
L8c0b3c92: target=0x8c0b3480u;*pr=0x8c0b3c96u;*stop=0x8c0b3c94u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3c96; /* bsr 0x8c0b3480 */
L8c0b3c96: *stop=0x8c0b3c96u;r[5]=r[15];goto L8c0b3c98; /* mov r15,r5 */
L8c0b3c98: *stop=0x8c0b3c98u;r[4]=r[15];goto L8c0b3c9a; /* mov r15,r4 */
L8c0b3c9a: *stop=0x8c0b3c9au;r[4]+=84u;goto L8c0b3c9c; /* add #0x54,r4 */
L8c0b3c9c: *stop=0x8c0b3c9cu;r[6]=r[15];goto L8c0b3c9e; /* mov r15,r6 */
L8c0b3c9e: *stop=0x8c0b3c9eu;r[5]+=60u;goto L8c0b3ca0; /* add #0x3c,r5 */
L8c0b3ca0: *stop=0x8c0b3ca0u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3ca2; /* fmov.s @r4+,fr3 */
L8c0b3ca2: *stop=0x8c0b3ca2u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3ca4; /* fmov.s @r5+,fr2 */
L8c0b3ca4: *stop=0x8c0b3ca4u;r[6]+=48u;goto L8c0b3ca6; /* add #0x30,r6 */
L8c0b3ca6: *stop=0x8c0b3ca6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c0b3ca8; /* fldi1 fr4 */
L8c0b3ca8: *stop=0x8c0b3ca8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[4],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c0b3caa; /* fadd fr4,fr4 */
L8c0b3caa: *stop=0x8c0b3caau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0b3cac; /* fsub fr3,fr2 */
L8c0b3cac: *stop=0x8c0b3cacu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c0b3cae; /* fmov.s fr2,@r6 */
L8c0b3cae: *stop=0x8c0b3caeu;r[6]+=4u;goto L8c0b3cb0; /* add #0x4,r6 */
L8c0b3cb0: *stop=0x8c0b3cb0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3cb2; /* fmov.s @r5+,fr2 */
L8c0b3cb2: *stop=0x8c0b3cb2u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3cb4; /* fmov.s @r4+,fr3 */
L8c0b3cb4: *stop=0x8c0b3cb4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0b3cb6; /* fsub fr3,fr2 */
L8c0b3cb6: *stop=0x8c0b3cb6u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c0b3cb8; /* fmov.s fr2,@r6 */
L8c0b3cb8: *stop=0x8c0b3cb8u;r[6]+=4u;goto L8c0b3cba; /* add #0x4,r6 */
L8c0b3cba: *stop=0x8c0b3cbau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3cbc; /* fmov.s @r5,fr2 */
L8c0b3cbc: *stop=0x8c0b3cbcu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c0b3cbe; /* fmov.s @r4,fr3 */
L8c0b3cbe: *stop=0x8c0b3cbeu;r[4]=r[15];goto L8c0b3cc0; /* mov r15,r4 */
L8c0b3cc0: *stop=0x8c0b3cc0u;r[4]+=48u;goto L8c0b3cc2; /* add #0x30,r4 */
L8c0b3cc2: *stop=0x8c0b3cc2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0b3cc4; /* fsub fr3,fr2 */
L8c0b3cc4: *stop=0x8c0b3cc4u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c0b3cc6; /* fmov.s fr2,@r6 */
L8c0b3cc6: *stop=0x8c0b3cc6u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c0b3cc8; /* fmov.s @r4,fr3 */
L8c0b3cc8: *stop=0x8c0b3cc8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0b3cca; /* fmul fr4,fr3 */
L8c0b3cca: *stop=0x8c0b3ccau;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c0b3ccc; /* fmov.s fr3,@r4 */
L8c0b3ccc: *stop=0x8c0b3cccu;r[4]+=4u;goto L8c0b3cce; /* add #0x4,r4 */
L8c0b3cce: *stop=0x8c0b3cceu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c0b3cd0; /* fmov.s @r4,fr3 */
L8c0b3cd0: *stop=0x8c0b3cd0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0b3cd2; /* fmul fr4,fr3 */
L8c0b3cd2: *stop=0x8c0b3cd2u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c0b3cd4; /* fmov.s fr3,@r4 */
L8c0b3cd4: *stop=0x8c0b3cd4u;r[4]+=4u;goto L8c0b3cd6; /* add #0x4,r4 */
L8c0b3cd6: *stop=0x8c0b3cd6u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c0b3cd8; /* fmov.s @r4,fr2 */
L8c0b3cd8: *stop=0x8c0b3cd8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3e90u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3cda; /* mov.l 0x8c0b3e90,r3 */
L8c0b3cda: *stop=0x8c0b3cdau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3cdc; /* fmul fr4,fr2 */
L8c0b3cdc: *stop=0x8c0b3cdcu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c0b3cde; /* fmov.s fr2,@r4 */
L8c0b3cde: *stop=0x8c0b3cdeu;r[4]=r[15];goto L8c0b3ce0; /* mov r15,r4 */
L8c0b3ce0: target=r[3];*pr=0x8c0b3ce4u;*stop=0x8c0b3ce2u;r[4]+=48u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3ce4; /* jsr @r3 */
L8c0b3ce4: *stop=0x8c0b3ce4u;r[5]=r[15];goto L8c0b3ce6; /* mov r15,r5 */
L8c0b3ce6: *stop=0x8c0b3ce6u;r[4]=r[15];goto L8c0b3ce8; /* mov r15,r4 */
L8c0b3ce8: *stop=0x8c0b3ce8u;r[4]+=84u;goto L8c0b3cea; /* add #0x54,r4 */
L8c0b3cea: *stop=0x8c0b3ceau;r[5]+=48u;goto L8c0b3cec; /* add #0x30,r5 */
L8c0b3cec: *stop=0x8c0b3cecu;r[0]=44u;goto L8c0b3cee; /* mov #0x2c,r0 */
L8c0b3cee: *stop=0x8c0b3ceeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3cf0; /* fmov fr0,@(r0,r15) */
L8c0b3cf0: *stop=0x8c0b3cf0u;r[0]=48u;goto L8c0b3cf2; /* mov #0x30,r0 */
L8c0b3cf2: *stop=0x8c0b3cf2u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3cf4; /* fmov.s @r4+,fr3 */
L8c0b3cf4: *stop=0x8c0b3cf4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3cf6; /* fmov.s @r5,fr2 */
L8c0b3cf6: *stop=0x8c0b3cf6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b3cf8; /* fadd fr3,fr2 */
L8c0b3cf8: *stop=0x8c0b3cf8u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3cfa; /* fmov.s fr2,@r5 */
L8c0b3cfa: *stop=0x8c0b3cfau;r[5]+=4u;goto L8c0b3cfc; /* add #0x4,r5 */
L8c0b3cfc: *stop=0x8c0b3cfcu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3cfe; /* fmov.s @r4+,fr3 */
L8c0b3cfe: *stop=0x8c0b3cfeu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3d00; /* fmov.s @r5,fr2 */
L8c0b3d00: *stop=0x8c0b3d00u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b3d02; /* fadd fr3,fr2 */
L8c0b3d02: *stop=0x8c0b3d02u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3d04; /* fmov.s fr2,@r5 */
L8c0b3d04: *stop=0x8c0b3d04u;r[5]+=4u;goto L8c0b3d06; /* add #0x4,r5 */
L8c0b3d06: *stop=0x8c0b3d06u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c0b3d08; /* fmov.s @r4,fr3 */
L8c0b3d08: *stop=0x8c0b3d08u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3d0a; /* fmov.s @r5,fr2 */
L8c0b3d0a: *stop=0x8c0b3d0au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b3d0c; /* fadd fr3,fr2 */
L8c0b3d0c: *stop=0x8c0b3d0cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3d0e; /* fmov.s fr2,@r5 */
L8c0b3d0e: *stop=0x8c0b3d0eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d10; /* fmov.s @(r0,r15),fr3 */
L8c0b3d10: *stop=0x8c0b3d10u;r[0]=16u;goto L8c0b3d12; /* mov #0x10,r0 */
L8c0b3d12: *stop=0x8c0b3d12u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d14; /* fmov fr3,@(r0,r15) */
L8c0b3d14: *stop=0x8c0b3d14u;r[0]=52u;goto L8c0b3d16; /* mov #0x34,r0 */
L8c0b3d16: *stop=0x8c0b3d16u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d18; /* fmov.s @(r0,r15),fr3 */
L8c0b3d18: *stop=0x8c0b3d18u;r[0]=20u;goto L8c0b3d1a; /* mov #0x14,r0 */
L8c0b3d1a: *stop=0x8c0b3d1au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d1c; /* fmov fr3,@(r0,r15) */
L8c0b3d1c: *stop=0x8c0b3d1cu;r[0]=56u;goto L8c0b3d1e; /* mov #0x38,r0 */
L8c0b3d1e: *stop=0x8c0b3d1eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d20; /* fmov.s @(r0,r15),fr3 */
L8c0b3d20: *stop=0x8c0b3d20u;r[0]=12u;goto L8c0b3d22; /* mov #0xc,r0 */
L8c0b3d22: *stop=0x8c0b3d22u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d24; /* fmov fr3,@(r0,r15) */
L8c0b3d24: *stop=0x8c0b3d24u;if(!valid_access(r[13]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x3u,1);if(io_status)return io_status;goto L8c0b3d26; /* mov.b @(0x3,r13),r0 */
L8c0b3d26: *stop=0x8c0b3d26u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0b3d28; /* mov.l @(0x8,r13),r4 */
L8c0b3d28: *stop=0x8c0b3d28u;r[0]=r[0]&255u;goto L8c0b3d2a; /* extu.b r0,r0 */
L8c0b3d2a: *stop=0x8c0b3d2au;r[3]=r[0];goto L8c0b3d2c; /* mov r0,r3 */
L8c0b3d2c: *stop=0x8c0b3d2cu;r[0]<<=2;goto L8c0b3d2e; /* shll2 r0 */
L8c0b3d2e: *stop=0x8c0b3d2eu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3d30; /* shll r0 */
L8c0b3d30: *stop=0x8c0b3d30u;r[0]+=r[3];goto L8c0b3d32; /* add r3,r0 */
L8c0b3d32: *stop=0x8c0b3d32u;r[0]<<=2;goto L8c0b3d34; /* shll2 r0 */
L8c0b3d34: *stop=0x8c0b3d34u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3d36; /* shll r0 */
L8c0b3d36: *stop=0x8c0b3d36u;r[4]+=r[0];goto L8c0b3d38; /* add r0,r4 */
L8c0b3d38: *stop=0x8c0b3d38u;r[0]=40u;goto L8c0b3d3a; /* mov #0x28,r0 */
L8c0b3d3a: *stop=0x8c0b3d3au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b3d3c; /* fmov.s @(r0,r4),fr3 */
L8c0b3d3c: *stop=0x8c0b3d3cu;r[0]=44u;goto L8c0b3d3e; /* mov #0x2c,r0 */
L8c0b3d3e: *stop=0x8c0b3d3eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c0b3d40; /* fmov.s fr3,@r15 */
L8c0b3d40: *stop=0x8c0b3d40u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b3d42; /* fmov.s @(r0,r4),fr3 */
L8c0b3d42: *stop=0x8c0b3d42u;r[0]=28u;goto L8c0b3d44; /* mov #0x1c,r0 */
L8c0b3d44: *stop=0x8c0b3d44u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d46; /* fmov fr3,@(r0,r15) */
L8c0b3d46: *stop=0x8c0b3d46u;r[0]=60u;goto L8c0b3d48; /* mov #0x3c,r0 */
L8c0b3d48: *stop=0x8c0b3d48u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b3d4a; /* fmov.s @(r0,r4),fr3 */
L8c0b3d4a: *stop=0x8c0b3d4au;r[0]=40u;goto L8c0b3d4c; /* mov #0x28,r0 */
L8c0b3d4c: *stop=0x8c0b3d4cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d4e; /* fmov fr3,@(r0,r15) */
L8c0b3d4e: *stop=0x8c0b3d4eu;r[0]=64u;goto L8c0b3d50; /* mov #0x40,r0 */
L8c0b3d50: *stop=0x8c0b3d50u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b3d52; /* fmov.s @(r0,r4),fr3 */
L8c0b3d52: *stop=0x8c0b3d52u;r[0]=24u;goto L8c0b3d54; /* mov #0x18,r0 */
L8c0b3d54: *stop=0x8c0b3d54u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d56; /* fmov fr3,@(r0,r15) */
L8c0b3d56: *stop=0x8c0b3d56u;r[0]=68u;goto L8c0b3d58; /* mov #0x44,r0 */
L8c0b3d58: *stop=0x8c0b3d58u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b3d5a; /* fmov.s @(r0,r4),fr3 */
L8c0b3d5a: *stop=0x8c0b3d5au;r[0]=8u;goto L8c0b3d5c; /* mov #0x8,r0 */
L8c0b3d5c: *stop=0x8c0b3d5cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d5e; /* fmov fr3,@(r0,r15) */
L8c0b3d5e: *stop=0x8c0b3d5eu;r[0]=16u;goto L8c0b3d60; /* mov #0x10,r0 */
L8c0b3d60: *stop=0x8c0b3d60u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d62; /* fmov.s @(r0,r15),fr3 */
L8c0b3d62: *stop=0x8c0b3d62u;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c0b3d64; /* mov.l @(0x10,r13),r14 */
L8c0b3d64: *stop=0x8c0b3d64u;r[0]=32u;goto L8c0b3d66; /* mov #0x20,r0 */
L8c0b3d66: *stop=0x8c0b3d66u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c0b3d68; /* fneg fr3 */
L8c0b3d68: *stop=0x8c0b3d68u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d6a; /* fmov fr3,@(r0,r15) */
L8c0b3d6a: *stop=0x8c0b3d6au;r[0]=20u;goto L8c0b3d6c; /* mov #0x14,r0 */
L8c0b3d6c: *stop=0x8c0b3d6cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d6e; /* fmov.s @(r0,r15),fr2 */
L8c0b3d6e: *stop=0x8c0b3d6eu;r[0]=36u;goto L8c0b3d70; /* mov #0x24,r0 */
L8c0b3d70: *stop=0x8c0b3d70u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3e80u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c0b3d72; /* mov.l 0x8c0b3e80,r12 */
L8c0b3d72: *stop=0x8c0b3d72u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c0b3d74; /* tst r14,r14 */
L8c0b3d74: *stop=0x8c0b3d74u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c0b3d76; /* fneg fr2 */
L8c0b3d76: *stop=0x8c0b3d76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3e94u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c0b3d78; /* mov.l 0x8c0b3e94,r9 */
L8c0b3d78: *stop=0x8c0b3d78u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d7a; /* fmov fr2,@(r0,r15) */
L8c0b3d7a: *stop=0x8c0b3d7au;r[0]=0x8c0b3e84u;goto L8c0b3d7c; /* mova 0x8c0b3e84,r0 */
L8c0b3d7c: take=!(*sr&1u);*stop=0x8c0b3d7eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;if(take)goto L8c0b3d84;goto L8c0b3d80; /* bf/s 0x8c0b3d84 */
L8c0b3d80: *stop=0x8c0b3d82u;goto L8c0b3f0c; /* bra 0x8c0b3f0c */
L8c0b3d84: *stop=0x8c0b3d84u;r[5]=r[14];goto L8c0b3d86; /* mov r14,r5 */
L8c0b3d86: *stop=0x8c0b3d86u;r[4]=r[14];goto L8c0b3d88; /* mov r14,r4 */
L8c0b3d88: *stop=0x8c0b3d88u;r[4]+=16u;goto L8c0b3d8a; /* add #0x10,r4 */
L8c0b3d8a: *stop=0x8c0b3d8au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c0b3d8c; /* fmov.s @r15,fr10 */
L8c0b3d8c: *stop=0x8c0b3d8cu;r[5]+=4u;goto L8c0b3d8e; /* add #0x4,r5 */
L8c0b3d8e: *stop=0x8c0b3d8eu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3d90; /* fmov.s @r4+,fr5 */
L8c0b3d90: *stop=0x8c0b3d90u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3d92; /* fmov.s @r5+,fr9 */
L8c0b3d92: *stop=0x8c0b3d92u;r[0]=28u;goto L8c0b3d94; /* mov #0x1c,r0 */
L8c0b3d94: *stop=0x8c0b3d94u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3d96; /* fmov.s @r4+,fr4 */
L8c0b3d96: *stop=0x8c0b3d96u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3d98; /* fmov.s @r5+,fr7 */
L8c0b3d98: *stop=0x8c0b3d98u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[5],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c0b3d9a; /* fadd fr5,fr9 */
L8c0b3d9a: *stop=0x8c0b3d9au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3d9c; /* fmov.s @(r0,r15),fr3 */
L8c0b3d9c: *stop=0x8c0b3d9cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[10],&f->fpscr,&fr[5]);if(status)return status;}goto L8c0b3d9e; /* fmul fr10,fr5 */
L8c0b3d9e: *stop=0x8c0b3d9eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[4],&f->fpscr,&fr[7],0);if(status)return status;}goto L8c0b3da0; /* fadd fr4,fr7 */
L8c0b3da0: *stop=0x8c0b3da0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;goto L8c0b3da2; /* fmov.s @r5,fr8 */
L8c0b3da2: *stop=0x8c0b3da2u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c0b3da4; /* fmov.s @r4,fr6 */
L8c0b3da4: *stop=0x8c0b3da4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c0b3da6; /* fadd fr3,fr4 */
L8c0b3da6: *stop=0x8c0b3da6u;r[0]=36u;goto L8c0b3da8; /* mov #0x24,r0 */
L8c0b3da8: *stop=0x8c0b3da8u;if(!valid_access(r[14]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x30u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c0b3daa; /* mov.l @(0x30,r14),r10 */
L8c0b3daa: *stop=0x8c0b3daau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[6],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c0b3dac; /* fadd fr6,fr8 */
L8c0b3dac: *stop=0x8c0b3dacu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[10],&f->fpscr,&fr[6]);if(status)return status;}goto L8c0b3dae; /* fmul fr10,fr6 */
L8c0b3dae: *stop=0x8c0b3daeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c0b3db0; /* fmul fr10,fr4 */
L8c0b3db0: *stop=0x8c0b3db0u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c0b3db2; /* fmov.s fr6,@r4 */
L8c0b3db2: *stop=0x8c0b3db2u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c0b3db4; /* fmov.s fr4,@-r4 */
L8c0b3db4: *stop=0x8c0b3db4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c0b3db6; /* fmov.s fr5,@-r4 */
L8c0b3db6: *stop=0x8c0b3db6u;r[4]=r[14];goto L8c0b3db8; /* mov r14,r4 */
L8c0b3db8: *stop=0x8c0b3db8u;r[4]+=4u;goto L8c0b3dba; /* add #0x4,r4 */
L8c0b3dba: *stop=0x8c0b3dbau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;goto L8c0b3dbc; /* fmov.s fr8,@r5 */
L8c0b3dbc: *stop=0x8c0b3dbcu;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;goto L8c0b3dbe; /* fmov.s fr7,@-r5 */
L8c0b3dbe: *stop=0x8c0b3dbeu;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;goto L8c0b3dc0; /* fmov.s fr9,@-r5 */
L8c0b3dc0: *stop=0x8c0b3dc0u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3dc2; /* fmov.s @r4+,fr3 */
L8c0b3dc2: *stop=0x8c0b3dc2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c0b3dc4; /* fmov.s @(r0,r14),fr5 */
L8c0b3dc4: *stop=0x8c0b3dc4u;r[0]=32u;goto L8c0b3dc6; /* mov #0x20,r0 */
L8c0b3dc6: *stop=0x8c0b3dc6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3dc8; /* fmov.s @(r0,r15),fr13 */
L8c0b3dc8: *stop=0x8c0b3dc8u;r[0]=36u;goto L8c0b3dca; /* mov #0x24,r0 */
L8c0b3dca: *stop=0x8c0b3dcau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3dcc; /* fmov.s @(r0,r15),fr14 */
L8c0b3dcc: *stop=0x8c0b3dccu;r[0]=12u;goto L8c0b3dce; /* mov #0xc,r0 */
L8c0b3dce: *stop=0x8c0b3dceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[3],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c0b3dd0; /* fadd fr3,fr13 */
L8c0b3dd0: *stop=0x8c0b3dd0u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3dd2; /* fmov.s @r4+,fr3 */
L8c0b3dd2: *stop=0x8c0b3dd2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c0b3dd4; /* fadd fr3,fr14 */
L8c0b3dd4: *stop=0x8c0b3dd4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3dd6; /* fmov.s @(r0,r15),fr3 */
L8c0b3dd6: *stop=0x8c0b3dd6u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[4]);if(io_status)return io_status;goto L8c0b3dd8; /* fmov.s @r4,fr12 */
L8c0b3dd8: *stop=0x8c0b3dd8u;r[0]=44u;goto L8c0b3dda; /* mov #0x2c,r0 */
L8c0b3dda: *stop=0x8c0b3ddau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3ddc; /* fmov.s @(r0,r15),fr1 */
L8c0b3ddc: *stop=0x8c0b3ddcu;r[0]=4u;goto L8c0b3dde; /* mov #0x4,r0 */
L8c0b3dde: *stop=0x8c0b3ddeu;fcopy(f,0u,13u);goto L8c0b3de0; /* fmov fr13,fr0 */
L8c0b3de0: *stop=0x8c0b3de0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[3],&f->fpscr,&fr[12],1);if(status)return status;}goto L8c0b3de2; /* fsub fr3,fr12 */
L8c0b3de2: *stop=0x8c0b3de2u;fcopy(f,2u,14u);goto L8c0b3de4; /* fmov fr14,fr2 */
L8c0b3de4: *stop=0x8c0b3de4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[14],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3de6; /* fmul fr14,fr2 */
L8c0b3de6: *stop=0x8c0b3de6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[5],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c0b3de8; /* fadd fr5,fr1 */
L8c0b3de8: *stop=0x8c0b3de8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[13],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3dea; /* fmac fr0,fr13,fr2 */
L8c0b3dea: *stop=0x8c0b3deau;fcopy(f,0u,12u);goto L8c0b3dec; /* fmov fr12,fr0 */
L8c0b3dec: *stop=0x8c0b3decu;fcopy(f,5u,1u);goto L8c0b3dee; /* fmov fr1,fr5 */
L8c0b3dee: *stop=0x8c0b3deeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[1],&f->fpscr,&fr[5]);if(status)return status;}goto L8c0b3df0; /* fmul fr1,fr5 */
L8c0b3df0: *stop=0x8c0b3df0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[12],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3df2; /* fmac fr0,fr12,fr2 */
L8c0b3df2: *stop=0x8c0b3df2u;fcopy(f,4u,2u);goto L8c0b3df4; /* fmov fr2,fr4 */
L8c0b3df4: *stop=0x8c0b3df4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c0b3df6; /* fcmp/gt fr4,fr5 */
L8c0b3df6: take=!(*sr&1u);*stop=0x8c0b3df8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;if(take)goto L8c0b3e46;goto L8c0b3dfa; /* bf/s 0x8c0b3e46 */
L8c0b3dfa: *stop=0x8c0b3dfau;r[0]=0x8c0b3e98u;goto L8c0b3dfc; /* mova 0x8c0b3e98,r0 */
L8c0b3dfc: *stop=0x8c0b3dfcu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c0b3dfe; /* fmov.s @r0,fr2 */
L8c0b3dfe: *stop=0x8c0b3dfeu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c0b3e00; /* fcmp/gt fr2,fr4 */
L8c0b3e00: if(!(*sr&1u))goto L8c0b3e46;goto L8c0b3e02; /* bf 0x8c0b3e46 */
L8c0b3e02: *stop=0x8c0b3e02u;r[0]=0x8c0b3e9cu;goto L8c0b3e04; /* mova 0x8c0b3e9c,r0 */
L8c0b3e04: *stop=0x8c0b3e04u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0b3e06; /* fmov.s @r0,fr3 */
L8c0b3e06: *stop=0x8c0b3e06u;r[0]=4u;goto L8c0b3e08; /* mov #0x4,r0 */
L8c0b3e08: *stop=0x8c0b3e08u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3e0a; /* fmov.s @(r0,r15),fr2 */
L8c0b3e0a: *stop=0x8c0b3e0au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b3e0c; /* fadd fr3,fr2 */
L8c0b3e0c: target=r[9];*pr=0x8c0b3e10u;*stop=0x8c0b3e0eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3e10; /* jsr @r9 */
L8c0b3e10: *stop=0x8c0b3e10u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3e12; /* fmov.s @r15+,fr4 */
L8c0b3e12: *stop=0x8c0b3e12u;r[0]=16u;goto L8c0b3e14; /* mov #0x10,r0 */
L8c0b3e14: *stop=0x8c0b3e14u;r[4]=r[14];goto L8c0b3e16; /* mov r14,r4 */
L8c0b3e16: *stop=0x8c0b3e16u;r[4]+=4u;goto L8c0b3e18; /* add #0x4,r4 */
L8c0b3e18: *stop=0x8c0b3e18u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[0],&f->fpscr,&fr[4]);if(status)return status;}goto L8c0b3e1a; /* fdiv fr0,fr4 */
L8c0b3e1a: *stop=0x8c0b3e1au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3e1c; /* fmov.s @(r0,r15),fr3 */
L8c0b3e1c: *stop=0x8c0b3e1cu;r[0]=12u;goto L8c0b3e1e; /* mov #0xc,r0 */
L8c0b3e1e: *stop=0x8c0b3e1eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[4],&f->fpscr,&fr[13]);if(status)return status;}goto L8c0b3e20; /* fmul fr4,fr13 */
L8c0b3e20: *stop=0x8c0b3e20u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[4],&f->fpscr,&fr[12]);if(status)return status;}goto L8c0b3e22; /* fmul fr4,fr12 */
L8c0b3e22: *stop=0x8c0b3e22u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[14],&fr[4],&f->fpscr,&fr[14]);if(status)return status;}goto L8c0b3e24; /* fmul fr4,fr14 */
L8c0b3e24: *stop=0x8c0b3e24u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[13],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c0b3e26; /* fadd fr13,fr3 */
L8c0b3e26: *stop=0x8c0b3e26u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c0b3e28; /* fmov.s fr3,@r4 */
L8c0b3e28: *stop=0x8c0b3e28u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3e2a; /* fmov.s @(r0,r15),fr4 */
L8c0b3e2a: *stop=0x8c0b3e2au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0b3e2c; /* fldi0 fr3 */
L8c0b3e2c: *stop=0x8c0b3e2cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][14],&f->fpscr,sr,1);if(status)return status;goto L8c0b3e2e; /* fcmp/gt fr14,fr3 */
L8c0b3e2e: *stop=0x8c0b3e2eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[12],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c0b3e30; /* fadd fr12,fr4 */
L8c0b3e30: take=(*sr&1u);*stop=0x8c0b3e32u;r[4]+=4u;if(take)goto L8c0b3e42;goto L8c0b3e34; /* bt/s 0x8c0b3e42 */
L8c0b3e34: *stop=0x8c0b3e34u;r[0]=20u;goto L8c0b3e36; /* mov #0x14,r0 */
L8c0b3e36: *stop=0x8c0b3e36u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3e38; /* fmov.s @(r0,r15),fr2 */
L8c0b3e38: *stop=0x8c0b3e38u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b3e3a; /* fadd fr14,fr2 */
L8c0b3e3a: *stop=0x8c0b3e3au;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c0b3e3c; /* fmov.s fr2,@r4 */
L8c0b3e3c: *stop=0x8c0b3e3cu;r[4]+=4u;goto L8c0b3e3e; /* add #0x4,r4 */
L8c0b3e3e: *stop=0x8c0b3e40u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c0b3e46; /* bra 0x8c0b3e46 */
L8c0b3e42: *stop=0x8c0b3e42u;r[0]=4u;goto L8c0b3e44; /* mov #0x4,r0 */
L8c0b3e44: *stop=0x8c0b3e44u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b3e46; /* fmov fr4,@(r0,r4) */
L8c0b3e46: *stop=0x8c0b3e46u;r[0]=8u;goto L8c0b3e48; /* mov #0x8,r0 */
L8c0b3e48: *stop=0x8c0b3e48u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[14]);if(io_status)return io_status;goto L8c0b3e4a; /* fmov.s @(r0,r14),fr6 */
L8c0b3e4a: *stop=0x8c0b3e4au;r[0]=40u;goto L8c0b3e4c; /* mov #0x28,r0 */
L8c0b3e4c: *stop=0x8c0b3e4cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c0b3e4e; /* fmov.s @(r0,r14),fr5 */
L8c0b3e4e: *stop=0x8c0b3e4eu;r[0]=40u;goto L8c0b3e50; /* mov #0x28,r0 */
L8c0b3e50: *stop=0x8c0b3e50u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3e52; /* fmov.s @(r0,r15),fr3 */
L8c0b3e52: *stop=0x8c0b3e52u;r[0]=36u;goto L8c0b3e54; /* mov #0x24,r0 */
L8c0b3e54: *stop=0x8c0b3e54u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c0b3e56; /* fmov.s @(r0,r14),fr4 */
L8c0b3e56: *stop=0x8c0b3e56u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c0b3e58; /* fadd fr3,fr4 */
L8c0b3e58: *stop=0x8c0b3e58u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c0b3e5a; /* fcmp/gt fr5,fr4 */
L8c0b3e5a: take=!(*sr&1u);*stop=0x8c0b3e5cu;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[14]);if(io_status)return io_status;if(take)goto L8c0b3e60;goto L8c0b3e5e; /* bf/s 0x8c0b3e60 */
L8c0b3e5e: *stop=0x8c0b3e5eu;fcopy(f,4u,5u);goto L8c0b3e60; /* fmov fr5,fr4 */
L8c0b3e60: *stop=0x8c0b3e60u;fcopy(f,3u,7u);goto L8c0b3e62; /* fmov fr7,fr3 */
L8c0b3e62: *stop=0x8c0b3e62u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c0b3e64; /* fsub fr4,fr3 */
L8c0b3e64: *stop=0x8c0b3e64u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][6],&f->fpscr,sr,1);if(status)return status;goto L8c0b3e66; /* fcmp/gt fr6,fr7 */
L8c0b3e66: *stop=0x8c0b3e66u;fcopy(f,5u,3u);goto L8c0b3e68; /* fmov fr3,fr5 */
L8c0b3e68: take=(*sr&1u);*stop=0x8c0b3e6au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[6],&f->fpscr,&fr[5],1);if(status)return status;}if(take)goto L8c0b3ea0;goto L8c0b3e6c; /* bt/s 0x8c0b3ea0 */
L8c0b3e6c: *stop=0x8c0b3e6cu;r[5]=r[14];goto L8c0b3e6e; /* mov r14,r5 */
L8c0b3e6e: target=0x8c0b33ceu;*pr=0x8c0b3e72u;*stop=0x8c0b3e70u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3e72; /* bsr 0x8c0b33ce */
L8c0b3e72: *stop=0x8c0b3e74u;goto L8c0b3eaa; /* bra 0x8c0b3eaa */
L8c0b3ea0: *stop=0x8c0b3ea0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0b3ea2; /* fldi0 fr3 */
L8c0b3ea2: *stop=0x8c0b3ea2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c0b3ea4; /* fcmp/gt fr5,fr3 */
L8c0b3ea4: if(!(*sr&1u))goto L8c0b3ea8;goto L8c0b3ea6; /* bf 0x8c0b3ea8 */
L8c0b3ea6: *stop=0x8c0b3ea6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[5],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c0b3ea8; /* fadd fr5,fr4 */
L8c0b3ea8: *stop=0x8c0b3ea8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c0b3eaa; /* fmov fr4,@(r0,r14) */
L8c0b3eaa: target=r[12];*pr=0x8c0b3eaeu;*stop=0x8c0b3eacu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3eae; /* jsr @r12 */
L8c0b3eae: *stop=0x8c0b3eaeu;*fpul=r[0];goto L8c0b3eb0; /* lds r0,FPUL */
L8c0b3eb0: *stop=0x8c0b3eb0u;r[0]=24u;goto L8c0b3eb2; /* mov #0x18,r0 */
L8c0b3eb2: *stop=0x8c0b3eb2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3eb4; /* fmov.s @(r0,r15),fr2 */
L8c0b3eb4: *stop=0x8c0b3eb4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c0b3eb6; /* float FPUL,fr3 */
L8c0b3eb6: *stop=0x8c0b3eb6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0b3eb8; /* fmul fr15,fr3 */
L8c0b3eb8: *stop=0x8c0b3eb8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0b3eba; /* fcmp/gt fr3,fr2 */
L8c0b3eba: if(!(*sr&1u))goto L8c0b3ef8;goto L8c0b3ebc; /* bf 0x8c0b3ef8 */
L8c0b3ebc: *stop=0x8c0b3ebcu;r[11]=r[14];goto L8c0b3ebe; /* mov r14,r11 */
L8c0b3ebe: *stop=0x8c0b3ebeu;r[11]+=16u;goto L8c0b3ec0; /* add #0x10,r11 */
L8c0b3ec0: *stop=0x8c0b3ec0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c0b3ec2; /* mov.l r11,@-r15 */
L8c0b3ec2: target=r[12];*pr=0x8c0b3ec6u;*stop=0x8c0b3ec4u;r[11]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3ec6; /* jsr @r12 */
L8c0b3ec6: *stop=0x8c0b3ec6u;*fpul=r[0];goto L8c0b3ec8; /* lds r0,FPUL */
L8c0b3ec8: *stop=0x8c0b3ec8u;r[0]=0x8c0b4000u;goto L8c0b3eca; /* mova 0x8c0b4000,r0 */
L8c0b3eca: *stop=0x8c0b3ecau;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c0b3ecc; /* fmov.s @r0,fr2 */
L8c0b3ecc: *stop=0x8c0b3eccu;r[0]=12u;goto L8c0b3ece; /* mov #0xc,r0 */
L8c0b3ece: *stop=0x8c0b3eceu;fcopy(f,0u,15u);goto L8c0b3ed0; /* fmov fr15,fr0 */
L8c0b3ed0: *stop=0x8c0b3ed0u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c0b3ed2; /* float FPUL,fr3 */
L8c0b3ed2: *stop=0x8c0b3ed2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3ed4; /* fmac fr0,fr3,fr2 */
L8c0b3ed4: *stop=0x8c0b3ed4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3ed6; /* fmov.s @(r0,r15),fr0 */
L8c0b3ed6: *stop=0x8c0b3ed6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c0b3ed8; /* mov.l @r15+,r3 */
L8c0b3ed8: *stop=0x8c0b3ed8u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c0b3eda; /* fmov.s @r3,fr3 */
L8c0b3eda: *stop=0x8c0b3edau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0b3edc; /* fmac fr0,fr2,fr3 */
L8c0b3edc: target=r[12];*pr=0x8c0b3ee0u;*stop=0x8c0b3edeu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3ee0; /* jsr @r12 */
L8c0b3ee0: *stop=0x8c0b3ee0u;*fpul=r[0];goto L8c0b3ee2; /* lds r0,FPUL */
L8c0b3ee2: *stop=0x8c0b3ee2u;r[0]=0x8c0b4000u;goto L8c0b3ee4; /* mova 0x8c0b4000,r0 */
L8c0b3ee4: *stop=0x8c0b3ee4u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0b3ee6; /* fmov.s @r0,fr3 */
L8c0b3ee6: *stop=0x8c0b3ee6u;r[0]=8u;goto L8c0b3ee8; /* mov #0x8,r0 */
L8c0b3ee8: *stop=0x8c0b3ee8u;fcopy(f,0u,15u);goto L8c0b3eea; /* fmov fr15,fr0 */
L8c0b3eea: *stop=0x8c0b3eeau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c0b3eec; /* float FPUL,fr2 */
L8c0b3eec: *stop=0x8c0b3eecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0b3eee; /* fmac fr0,fr2,fr3 */
L8c0b3eee: *stop=0x8c0b3eeeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3ef0; /* fmov.s @(r0,r15),fr0 */
L8c0b3ef0: *stop=0x8c0b3ef0u;r[0]=4u;goto L8c0b3ef2; /* mov #0x4,r0 */
L8c0b3ef2: *stop=0x8c0b3ef2u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c0b3ef4; /* fmov.s @(r0,r11),fr2 */
L8c0b3ef4: *stop=0x8c0b3ef4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3ef6; /* fmac fr0,fr3,fr2 */
L8c0b3ef6: *stop=0x8c0b3ef6u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c0b3ef8; /* fmov fr2,@(r0,r11) */
L8c0b3ef8: *stop=0x8c0b3ef8u;if(!valid_access(r[14]+30u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x1eu,2);if(io_status)return io_status;goto L8c0b3efa; /* mov.w @(0x1e,r14),r0 */
L8c0b3efa: *stop=0x8c0b3efau;r[3]=r[0];goto L8c0b3efc; /* mov r0,r3 */
L8c0b3efc: *stop=0x8c0b3efcu;if(!valid_access(r[14]+28u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x1cu,2);if(io_status)return io_status;goto L8c0b3efe; /* mov.w @(0x1c,r14),r0 */
L8c0b3efe: *stop=0x8c0b3efeu;r[0]+=r[3];goto L8c0b3f00; /* add r3,r0 */
L8c0b3f00: *stop=0x8c0b3f00u;if(!valid_access(r[14]+28u,2u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[0],2);if(io_status)return io_status;goto L8c0b3f02; /* mov.w r0,@(0x1c,r14) */
L8c0b3f02: *stop=0x8c0b3f02u;r[14]=r[10];goto L8c0b3f04; /* mov r10,r14 */
L8c0b3f04: *stop=0x8c0b3f04u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c0b3f06; /* tst r14,r14 */
L8c0b3f06: if((*sr&1u))goto L8c0b3f0c;goto L8c0b3f08; /* bt 0x8c0b3f0c */
L8c0b3f08: *stop=0x8c0b3f0au;goto L8c0b3d84; /* bra 0x8c0b3d84 */
L8c0b3f0c: *stop=0x8c0b3f0cu;if(!valid_access(r[13]+6u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x6u,2);if(io_status)return io_status;goto L8c0b3f0e; /* mov.w @(0x6,r13),r0 */
L8c0b3f0e: *stop=0x8c0b3f0eu;r[0]+=1u;goto L8c0b3f10; /* add #0x1,r0 */
L8c0b3f10: *stop=0x8c0b3f10u;if(!valid_access(r[13]+6u,2u))return -3;wr(m,bus,&io_status,r[13]+0x6u,r[0],2);if(io_status)return io_status;goto L8c0b3f12; /* mov.w r0,@(0x6,r13) */
L8c0b3f12: *stop=0x8c0b3f12u;r[15]+=96u;goto L8c0b3f14; /* add #0x60,r15 */
L8c0b3f14: *stop=0x8c0b3f14u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0b3f16; /* lds.l @r15+,PR */
L8c0b3f16: *stop=0x8c0b3f16u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3f18; /* fmov.s @r15+,fr12 */
L8c0b3f18: *stop=0x8c0b3f18u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3f1a; /* fmov.s @r15+,fr13 */
L8c0b3f1a: *stop=0x8c0b3f1au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3f1c; /* fmov.s @r15+,fr14 */
L8c0b3f1c: *stop=0x8c0b3f1cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3f1e; /* fmov.s @r15+,fr15 */
L8c0b3f1e: *stop=0x8c0b3f1eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c0b3f20; /* mov.l @r15+,r9 */
L8c0b3f20: *stop=0x8c0b3f20u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c0b3f22; /* mov.l @r15+,r10 */
L8c0b3f22: *stop=0x8c0b3f22u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c0b3f24; /* mov.l @r15+,r11 */
L8c0b3f24: *stop=0x8c0b3f24u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c0b3f26; /* mov.l @r15+,r12 */
L8c0b3f26: *stop=0x8c0b3f26u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0b3f28; /* mov.l @r15+,r13 */
L8c0b3f28: *stop=0x8c0b3f28u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c0b3f2au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_child_update_8c0b3b1a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3b3a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3b3au);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3b48(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3b48u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3b7e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3b7eu);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3bbc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3bbcu);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3be6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3be6u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3c2c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3c2cu);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3c56(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3c56u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3c62(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3c62u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3c7a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3c7au);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3c96(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3c96u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3ce4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3ce4u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3e10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3e10u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3e72(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3e72u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3eae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3eaeu);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3ec6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3ec6u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3ee0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3b1a(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3ee0u);}

static int sa_player_child_update_run_8c0b3afc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c0b3b0eu)goto L8c0b3b0e;goto L8c0b3afc;
L8c0b3afc: *stop=0x8c0b3afcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0b3afe; /* mov.l r14,@-r15 */
L8c0b3afe: *stop=0x8c0b3afeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0b3b00; /* sts.l PR,@-r15 */
L8c0b3b00: *stop=0x8c0b3b00u;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c0b3b02; /* mov.l @(0x20,r4),r14 */
L8c0b3b02: *stop=0x8c0b3b02u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3b04; /* mov.l @(0x10,r14),r3 */
L8c0b3b04: *stop=0x8c0b3b04u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0b3b06; /* tst r3,r3 */
L8c0b3b06: if((*sr&1u))goto L8c0b3b14;goto L8c0b3b08; /* bt 0x8c0b3b14 */
L8c0b3b08: *stop=0x8c0b3b08u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0b3b0a; /* mov.l @(0x10,r14),r5 */
L8c0b3b0a: target=0x8c0b33ceu;*pr=0x8c0b3b0eu;*stop=0x8c0b3b0cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3b0e; /* bsr 0x8c0b33ce */
L8c0b3b0e: *stop=0x8c0b3b0eu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3b10; /* mov.l @(0x10,r14),r2 */
L8c0b3b10: *stop=0x8c0b3b10u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c0b3b12; /* tst r2,r2 */
L8c0b3b12: if(!(*sr&1u))goto L8c0b3b08;goto L8c0b3b14; /* bf 0x8c0b3b08 */
L8c0b3b14: *stop=0x8c0b3b14u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0b3b16; /* lds.l @r15+,PR */
L8c0b3b16: *stop=0x8c0b3b16u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c0b3b18u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_child_update_8c0b3afc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3b0e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3afc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3b0eu);}

static int sa_player_child_update_run_8c0b3f2c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c0b3f50u)goto L8c0b3f50;if(resume_pc==0x8c0b3f58u)goto L8c0b3f58;if(resume_pc==0x8c0b3f96u)goto L8c0b3f96;goto L8c0b3f2c;
L8c0b3f2c: *stop=0x8c0b3f2cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0b3f2e; /* mov.l r14,@-r15 */
L8c0b3f2e: *stop=0x8c0b3f2eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0b3f30; /* mov.l r13,@-r15 */
L8c0b3f30: *stop=0x8c0b3f30u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0b3f32; /* sts.l PR,@-r15 */
L8c0b3f32: *stop=0x8c0b3f32u;r[15]+=4294967260u;goto L8c0b3f34; /* add #-0x24,r15 */
L8c0b3f34: *stop=0x8c0b3f34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b4004u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b3f36; /* mov.l 0x8c0b4004,r1 */
L8c0b3f36: *stop=0x8c0b3f36u;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3f38; /* mov.l @(0x20,r4),r3 */
L8c0b3f38: *stop=0x8c0b3f38u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0b3f3a; /* mov.l r3,@r15 */
L8c0b3f3a: *stop=0x8c0b3f3au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3f3c; /* mov.l @r1,r2 */
L8c0b3f3c: *stop=0x8c0b3f3cu;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c0b3f3e; /* cmp/pl r2 */
L8c0b3f3e: if((*sr&1u))goto L8c0b3f9c;goto L8c0b3f40; /* bt 0x8c0b3f9c */
L8c0b3f40: *stop=0x8c0b3f40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b400cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3f42; /* mov.l 0x8c0b400c,r2 */
L8c0b3f42: *stop=0x8c0b3f42u;r[5]=8u;goto L8c0b3f44; /* mov #0x8,r5 */
L8c0b3f44: *stop=0x8c0b3f44u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b4008u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0b3f46; /* mov.l 0x8c0b4008,r0 */
L8c0b3f46: *stop=0x8c0b3f46u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b4010u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3f48; /* mov.l 0x8c0b4010,r3 */
L8c0b3f48: *stop=0x8c0b3f48u;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c0b3f4a; /* mov.l r0,@(0x1c,r15) */
L8c0b3f4a: *stop=0x8c0b3f4au;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[2],4);if(io_status)return io_status;goto L8c0b3f4c; /* mov.l r2,@(0x20,r15) */
L8c0b3f4c: target=r[3];*pr=0x8c0b3f50u;*stop=0x8c0b3f4eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3f50; /* jsr @r3 */
L8c0b3f50: *stop=0x8c0b3f50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b4010u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3f52; /* mov.l 0x8c0b4010,r2 */
L8c0b3f52: *stop=0x8c0b3f52u;r[5]=10u;goto L8c0b3f54; /* mov #0xa,r5 */
L8c0b3f54: target=r[2];*pr=0x8c0b3f58u;*stop=0x8c0b3f56u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3f58; /* jsr @r2 */
L8c0b3f58: *stop=0x8c0b3f58u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}goto L8c0b3f5a; /* mov.l @r15,r14 */
L8c0b3f5a: *stop=0x8c0b3f5au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b4014u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c0b3f5c; /* mov.l 0x8c0b4014,r13 */
L8c0b3f5c: *stop=0x8c0b3f5cu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c0b3f5e; /* mov.l @(0x10,r14),r14 */
L8c0b3f5e: *stop=0x8c0b3f5eu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c0b3f60; /* tst r14,r14 */
L8c0b3f60: if((*sr&1u))goto L8c0b3f9c;goto L8c0b3f62; /* bt 0x8c0b3f9c */
L8c0b3f62: *stop=0x8c0b3f62u;if(!valid_access(r[14]+28u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x1cu,2);if(io_status)return io_status;goto L8c0b3f64; /* mov.w @(0x1c,r14),r0 */
L8c0b3f64: *stop=0x8c0b3f64u;r[3]=r[15];goto L8c0b3f66; /* mov r15,r3 */
L8c0b3f66: *stop=0x8c0b3f66u;r[3]+=4u;goto L8c0b3f68; /* add #0x4,r3 */
L8c0b3f68: *stop=0x8c0b3f68u;r[4]=r[15];goto L8c0b3f6a; /* mov r15,r4 */
L8c0b3f6a: *stop=0x8c0b3f6au;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[0],4);if(io_status)return io_status;goto L8c0b3f6c; /* mov.l r0,@(0x18,r15) */
L8c0b3f6c: *stop=0x8c0b3f6cu;r[0]=36u;goto L8c0b3f6e; /* mov #0x24,r0 */
L8c0b3f6e: *stop=0x8c0b3f6eu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3f70; /* mov.l @(0x4,r14),r2 */
L8c0b3f70: *stop=0x8c0b3f70u;r[6]=49u;goto L8c0b3f72; /* mov #0x31,r6 */
L8c0b3f72: *stop=0x8c0b3f72u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b3f74; /* mov.l @(0x8,r14),r1 */
L8c0b3f74: *stop=0x8c0b3f74u;r[5]=0u;goto L8c0b3f76; /* mov #0x0,r5 */
L8c0b3f76: *stop=0x8c0b3f76u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c0b3f78; /* mov.l r2,@r3 */
L8c0b3f78: *stop=0x8c0b3f78u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c0b3f7a; /* mov.l r1,@(0x4,r3) */
L8c0b3f7a: *stop=0x8c0b3f7au;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3f7c; /* mov.l @(0xc,r14),r2 */
L8c0b3f7c: *stop=0x8c0b3f7cu;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[2],4);if(io_status)return io_status;goto L8c0b3f7e; /* mov.l r2,@(0x8,r3) */
L8c0b3f7e: *stop=0x8c0b3f7eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0b3f80; /* fmov.s @(r0,r14),fr3 */
L8c0b3f80: *stop=0x8c0b3f80u;r[0]=16u;goto L8c0b3f82; /* mov #0x10,r0 */
L8c0b3f82: *stop=0x8c0b3f82u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3f84; /* fmov fr3,@(r0,r15) */
L8c0b3f84: *stop=0x8c0b3f84u;r[0]=32u;goto L8c0b3f86; /* mov #0x20,r0 */
L8c0b3f86: *stop=0x8c0b3f86u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0b3f88; /* fmov.s @(r0,r14),fr3 */
L8c0b3f88: *stop=0x8c0b3f88u;r[0]=36u;goto L8c0b3f8a; /* mov #0x24,r0 */
L8c0b3f8a: *stop=0x8c0b3f8au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0b3f8c; /* fmov.s @(r0,r14),fr2 */
L8c0b3f8c: *stop=0x8c0b3f8cu;r[0]=20u;goto L8c0b3f8e; /* mov #0x14,r0 */
L8c0b3f8e: *stop=0x8c0b3f8eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3f90; /* fmul fr3,fr2 */
L8c0b3f90: *stop=0x8c0b3f90u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3f92; /* fmov fr2,@(r0,r15) */
L8c0b3f92: target=r[13];*pr=0x8c0b3f96u;*stop=0x8c0b3f94u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3f96; /* jsr @r13 */
L8c0b3f96: *stop=0x8c0b3f96u;if(!valid_access(r[14]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x30u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c0b3f98; /* mov.l @(0x30,r14),r14 */
L8c0b3f98: *stop=0x8c0b3f98u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c0b3f9a; /* tst r14,r14 */
L8c0b3f9a: if(!(*sr&1u))goto L8c0b3f62;goto L8c0b3f9c; /* bf 0x8c0b3f62 */
L8c0b3f9c: *stop=0x8c0b3f9cu;r[15]+=36u;goto L8c0b3f9e; /* add #0x24,r15 */
L8c0b3f9e: *stop=0x8c0b3f9eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0b3fa0; /* lds.l @r15+,PR */
L8c0b3fa0: *stop=0x8c0b3fa0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0b3fa2; /* mov.l @r15+,r13 */
L8c0b3fa2: *stop=0x8c0b3fa2u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c0b3fa4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_child_update_8c0b3f2c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3f2c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3f50(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3f2c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3f50u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3f58(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3f2c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3f58u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3f96(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3f2c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3f96u);}

SA_STAGE_EXPORT int sa_player_child_update_8c0b33ce(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c0b33ce;
L8c0b33ce: *stop=0x8c0b33ceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0b33d0; /* mov.l r14,@-r15 */
L8c0b33d0: *stop=0x8c0b33d0u;r[14]=r[5];goto L8c0b33d2; /* mov r5,r14 */
L8c0b33d2: *stop=0x8c0b33d2u;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c0b33d4; /* mov.l @(0x2c,r14),r6 */
L8c0b33d4: *stop=0x8c0b33d4u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c0b33d6; /* tst r6,r6 */
L8c0b33d6: take=(*sr&1u);*stop=0x8c0b33d8u;if(!valid_access(r[14]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x30u,4);if(io_status)return io_status;r[5]=loaded;}if(take)goto L8c0b33de;goto L8c0b33da; /* bt/s 0x8c0b33de */
L8c0b33da: *stop=0x8c0b33dcu;if(!valid_access(r[6]+48u,4u))return -3;wr(m,bus,&io_status,r[6]+0x30u,r[5],4);if(io_status)return io_status;goto L8c0b33e0; /* bra 0x8c0b33e0 */
L8c0b33de: *stop=0x8c0b33deu;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[5],4);if(io_status)return io_status;goto L8c0b33e0; /* mov.l r5,@(0x10,r4) */
L8c0b33e0: *stop=0x8c0b33e0u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c0b33e2; /* tst r5,r5 */
L8c0b33e2: take=(*sr&1u);*stop=0x8c0b33e4u;r[4]=r[14];if(take)goto L8c0b33e8;goto L8c0b33e6; /* bt/s 0x8c0b33e8 */
L8c0b33e6: *stop=0x8c0b33e6u;if(!valid_access(r[5]+44u,4u))return -3;wr(m,bus,&io_status,r[5]+0x2cu,r[6],4);if(io_status)return io_status;goto L8c0b33e8; /* mov.l r6,@(0x2c,r5) */
L8c0b33e8: *stop=0x8c0b33e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b34e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b33ea; /* mov.l 0x8c0b34e4,r2 */
L8c0b33ea: target=r[2];*stop=0x8c0b33ecu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

static int sa_player_child_update_run_8c0b37f2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c0b3818u)goto L8c0b3818;if(resume_pc==0x8c0b383au)goto L8c0b383a;if(resume_pc==0x8c0b3860u)goto L8c0b3860;if(resume_pc==0x8c0b3898u)goto L8c0b3898;if(resume_pc==0x8c0b38c6u)goto L8c0b38c6;if(resume_pc==0x8c0b38f8u)goto L8c0b38f8;if(resume_pc==0x8c0b392au)goto L8c0b392a;if(resume_pc==0x8c0b3984u)goto L8c0b3984;if(resume_pc==0x8c0b399au)goto L8c0b399a;if(resume_pc==0x8c0b39dau)goto L8c0b39da;goto L8c0b37f2;
L8c0b37f2: *stop=0x8c0b37f2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0b37f4; /* mov.l r14,@-r15 */
L8c0b37f4: *stop=0x8c0b37f4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0b37f6; /* mov.l r13,@-r15 */
L8c0b37f6: *stop=0x8c0b37f6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c0b37f8; /* mov.l r12,@-r15 */
L8c0b37f8: *stop=0x8c0b37f8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c0b37fa; /* mov.l r8,@-r15 */
L8c0b37fa: *stop=0x8c0b37fau;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c0b37fc; /* fmov.s fr15,@-r15 */
L8c0b37fc: *stop=0x8c0b37fcu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c0b37fe; /* fmov.s fr14,@-r15 */
L8c0b37fe: *stop=0x8c0b37feu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c0b3800; /* fmov.s fr13,@-r15 */
L8c0b3800: *stop=0x8c0b3800u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c0b3802; /* fmov.s fr12,@-r15 */
L8c0b3802: *stop=0x8c0b3802u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0b3804; /* sts.l PR,@-r15 */
L8c0b3804: *stop=0x8c0b3804u;r[15]+=4294967264u;goto L8c0b3806; /* add #-0x20,r15 */
L8c0b3806: *stop=0x8c0b3806u;r[0]=52u;goto L8c0b3808; /* mov #0x34,r0 */
L8c0b3808: *stop=0x8c0b3808u;r[14]=r[4];goto L8c0b380a; /* mov r4,r14 */
L8c0b380a: *stop=0x8c0b380au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0b380c; /* fmov.s @(r0,r14),fr3 */
L8c0b380c: *stop=0x8c0b380cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3aa0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b380e; /* mov.l 0x8c0b3aa0,r3 */
L8c0b380e: *stop=0x8c0b380eu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c0b3810; /* ftrc fr3,FPUL */
L8c0b3810: *stop=0x8c0b3810u;r[0]=*fpul;goto L8c0b3812; /* sts FPUL,r0 */
L8c0b3812: *stop=0x8c0b3812u;if(!valid_access(r[15]+4u,1u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],1);if(io_status)return io_status;goto L8c0b3814; /* mov.b r0,@(0x4,r15) */
L8c0b3814: target=r[3];*pr=0x8c0b3818u;*stop=0x8c0b3816u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3818; /* jsr @r3 */
L8c0b3818: *stop=0x8c0b3818u;r[12]=r[0];goto L8c0b381a; /* mov r0,r12 */
L8c0b381a: *stop=0x8c0b381au;r[0]=125u;goto L8c0b381c; /* mov #0x7d,r0 */
L8c0b381c: *stop=0x8c0b381cu;r[0]<<=2;goto L8c0b381e; /* shll2 r0 */
L8c0b381e: *stop=0x8c0b381eu;r[2]=0u;goto L8c0b3820; /* mov #0x0,r2 */
L8c0b3820: *stop=0x8c0b3820u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c0b3822; /* mov.l r2,@r15 */
L8c0b3822: *stop=0x8c0b3822u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c0b3824; /* mov.l @(r0,r12),r0 */
L8c0b3824: *stop=0x8c0b3824u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c0b3826; /* tst #0x2,r0 */
L8c0b3826: if(!(*sr&1u))goto L8c0b382c;goto L8c0b3828; /* bf 0x8c0b382c */
L8c0b3828: *stop=0x8c0b382au;r[0]=0u;goto L8c0b3a18; /* bra 0x8c0b3a18 */
L8c0b382c: *stop=0x8c0b382cu;r[5]=r[15];goto L8c0b382e; /* mov r15,r5 */
L8c0b382e: *stop=0x8c0b382eu;r[6]=r[15];goto L8c0b3830; /* mov r15,r6 */
L8c0b3830: *stop=0x8c0b3830u;r[5]+=20u;goto L8c0b3832; /* add #0x14,r5 */
L8c0b3832: *stop=0x8c0b3832u;r[7]=0u;goto L8c0b3834; /* mov #0x0,r7 */
L8c0b3834: *stop=0x8c0b3834u;r[6]+=8u;goto L8c0b3836; /* add #0x8,r6 */
L8c0b3836: target=0x8c0b3480u;*pr=0x8c0b383au;*stop=0x8c0b3838u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b383a; /* bsr 0x8c0b3480 */
L8c0b383a: *stop=0x8c0b383au;r[0]=24u;goto L8c0b383c; /* mov #0x18,r0 */
L8c0b383c: *stop=0x8c0b383cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b383e; /* fmov.s @(r0,r15),fr14 */
L8c0b383e: *stop=0x8c0b383eu;r[0]=123u;goto L8c0b3840; /* mov #0x7b,r0 */
L8c0b3840: *stop=0x8c0b3840u;r[0]<<=2;goto L8c0b3842; /* shll2 r0 */
L8c0b3842: *stop=0x8c0b3842u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c0b3844; /* fmov.s @(r0,r12),fr15 */
L8c0b3844: *stop=0x8c0b3844u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][14],&f->fpscr,sr,1);if(status)return status;goto L8c0b3846; /* fcmp/gt fr14,fr15 */
L8c0b3846: if((*sr&1u))goto L8c0b384c;goto L8c0b3848; /* bt 0x8c0b384c */
L8c0b3848: *stop=0x8c0b384au;goto L8c0b3a16; /* bra 0x8c0b3a16 */
L8c0b384c: *stop=0x8c0b384cu;r[0]=0x8c0b3aa4u;goto L8c0b384e; /* mova 0x8c0b3aa4,r0 */
L8c0b384e: *stop=0x8c0b384eu;fcopy(f,2u,14u);goto L8c0b3850; /* fmov fr14,fr2 */
L8c0b3850: *stop=0x8c0b3850u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0b3852; /* fmov.s @r0,fr3 */
L8c0b3852: *stop=0x8c0b3852u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b3854; /* fadd fr3,fr2 */
L8c0b3854: *stop=0x8c0b3854u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][15],&f->fpscr,sr,1);if(status)return status;goto L8c0b3856; /* fcmp/gt fr15,fr2 */
L8c0b3856: if((*sr&1u))goto L8c0b385c;goto L8c0b3858; /* bt 0x8c0b385c */
L8c0b3858: *stop=0x8c0b385au;goto L8c0b3a16; /* bra 0x8c0b3a16 */
L8c0b385c: target=0x8c0b33a0u;*pr=0x8c0b3860u;*stop=0x8c0b385eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3860; /* bsr 0x8c0b33a0 */
L8c0b3860: *stop=0x8c0b3860u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0b3862; /* tst r0,r0 */
L8c0b3862: take=!(*sr&1u);*stop=0x8c0b3864u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;if(take)goto L8c0b386a;goto L8c0b3866; /* bf/s 0x8c0b386a */
L8c0b3866: *stop=0x8c0b3868u;goto L8c0b3a16; /* bra 0x8c0b3a16 */
L8c0b386a: *stop=0x8c0b386au;r[0]=0x8c0b3aa8u;goto L8c0b386c; /* mova 0x8c0b3aa8,r0 */
L8c0b386c: *stop=0x8c0b386cu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b386e; /* mov.l @(0x8,r14),r2 */
L8c0b386e: *stop=0x8c0b386eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c0b3870; /* fmov.s @r0,fr15 */
L8c0b3870: *stop=0x8c0b3870u;r[1]=32u;goto L8c0b3872; /* mov #0x20,r1 */
L8c0b3872: *stop=0x8c0b3872u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b3874; /* mov.b @(0x3,r14),r0 */
L8c0b3874: *stop=0x8c0b3874u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3aacu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c0b3876; /* mov.l 0x8c0b3aac,r13 */
L8c0b3876: *stop=0x8c0b3876u;r[0]=r[0]&255u;goto L8c0b3878; /* extu.b r0,r0 */
L8c0b3878: *stop=0x8c0b3878u;r[3]=r[0];goto L8c0b387a; /* mov r0,r3 */
L8c0b387a: *stop=0x8c0b387au;r[0]<<=2;goto L8c0b387c; /* shll2 r0 */
L8c0b387c: *stop=0x8c0b387cu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b387e; /* shll r0 */
L8c0b387e: *stop=0x8c0b387eu;r[0]+=r[3];goto L8c0b3880; /* add r3,r0 */
L8c0b3880: *stop=0x8c0b3880u;r[0]<<=2;goto L8c0b3882; /* shll2 r0 */
L8c0b3882: *stop=0x8c0b3882u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3884; /* shll r0 */
L8c0b3884: *stop=0x8c0b3884u;r[0]+=r[2];goto L8c0b3886; /* add r2,r0 */
L8c0b3886: *stop=0x8c0b3886u;r[3]=r[0];goto L8c0b3888; /* mov r0,r3 */
L8c0b3888: *stop=0x8c0b3888u;r[1]+=r[3];goto L8c0b388a; /* add r3,r1 */
L8c0b388a: *stop=0x8c0b388au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c0b388c; /* mov.l r0,@(0x4,r15) */
L8c0b388c: *stop=0x8c0b388cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;goto L8c0b388e; /* fmov.s @r1,fr0 */
L8c0b388e: *stop=0x8c0b388eu;r[1]=28u;goto L8c0b3890; /* mov #0x1c,r1 */
L8c0b3890: *stop=0x8c0b3890u;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c0b3892; /* fmov.s @(r0,r1),fr3 */
L8c0b3892: *stop=0x8c0b3892u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[0],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c0b3894; /* fsub fr0,fr3 */
L8c0b3894: target=r[13];*pr=0x8c0b3898u;*stop=0x8c0b3896u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3898; /* jsr @r13 */
L8c0b3898: *stop=0x8c0b3898u;*fpul=r[0];goto L8c0b389a; /* lds r0,FPUL */
L8c0b389a: *stop=0x8c0b389au;r[1]=32u;goto L8c0b389c; /* mov #0x20,r1 */
L8c0b389c: *stop=0x8c0b389cu;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b389e; /* mov.b @(0x3,r14),r0 */
L8c0b389e: *stop=0x8c0b389eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b38a0; /* fmov.s @r15+,fr2 */
L8c0b38a0: *stop=0x8c0b38a0u;r[0]=r[0]&255u;goto L8c0b38a2; /* extu.b r0,r0 */
L8c0b38a2: *stop=0x8c0b38a2u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c0b38a4; /* float FPUL,fr3 */
L8c0b38a4: *stop=0x8c0b38a4u;r[3]=r[0];goto L8c0b38a6; /* mov r0,r3 */
L8c0b38a6: *stop=0x8c0b38a6u;r[0]<<=2;goto L8c0b38a8; /* shll2 r0 */
L8c0b38a8: *stop=0x8c0b38a8u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b38aa; /* shll r0 */
L8c0b38aa: *stop=0x8c0b38aau;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b38ac; /* mov.l @(0x8,r14),r2 */
L8c0b38ac: *stop=0x8c0b38acu;r[0]+=r[3];goto L8c0b38ae; /* add r3,r0 */
L8c0b38ae: *stop=0x8c0b38aeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][13]=0x3f800000u;goto L8c0b38b0; /* fldi1 fr13 */
L8c0b38b0: *stop=0x8c0b38b0u;r[0]<<=2;goto L8c0b38b2; /* shll2 r0 */
L8c0b38b2: *stop=0x8c0b38b2u;fcopy(f,0u,15u);goto L8c0b38b4; /* fmov fr15,fr0 */
L8c0b38b4: *stop=0x8c0b38b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b38b6; /* fmul fr3,fr2 */
L8c0b38b6: *stop=0x8c0b38b6u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b38b8; /* shll r0 */
L8c0b38b8: *stop=0x8c0b38b8u;r[0]+=r[2];goto L8c0b38ba; /* add r2,r0 */
L8c0b38ba: *stop=0x8c0b38bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[13],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c0b38bc; /* fadd fr13,fr13 */
L8c0b38bc: *stop=0x8c0b38bcu;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]+r[1]);if(io_status)return io_status;goto L8c0b38be; /* fmov.s @(r0,r1),fr12 */
L8c0b38be: *stop=0x8c0b38beu;r[0]=0x8c0b3ab0u;goto L8c0b38c0; /* mova 0x8c0b3ab0,r0 */
L8c0b38c0: *stop=0x8c0b38c0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[12],&f->fpscr,&fr[12]);if(status)return status;}goto L8c0b38c2; /* fmac fr0,fr2,fr12 */
L8c0b38c2: target=r[13];*pr=0x8c0b38c6u;*stop=0x8c0b38c4u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b38c6; /* jsr @r13 */
L8c0b38c6: *stop=0x8c0b38c6u;*fpul=r[0];goto L8c0b38c8; /* lds r0,FPUL */
L8c0b38c8: *stop=0x8c0b38c8u;r[1]=48u;goto L8c0b38ca; /* mov #0x30,r1 */
L8c0b38ca: *stop=0x8c0b38cau;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b38cc; /* mov.b @(0x3,r14),r0 */
L8c0b38cc: *stop=0x8c0b38ccu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b38ce; /* mov.l @(0x8,r14),r2 */
L8c0b38ce: *stop=0x8c0b38ceu;r[0]=r[0]&255u;goto L8c0b38d0; /* extu.b r0,r0 */
L8c0b38d0: *stop=0x8c0b38d0u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c0b38d2; /* float FPUL,fr3 */
L8c0b38d2: *stop=0x8c0b38d2u;r[3]=r[0];goto L8c0b38d4; /* mov r0,r3 */
L8c0b38d4: *stop=0x8c0b38d4u;r[0]<<=2;goto L8c0b38d6; /* shll2 r0 */
L8c0b38d6: *stop=0x8c0b38d6u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b38d8; /* shll r0 */
L8c0b38d8: *stop=0x8c0b38d8u;fcopy(f,0u,15u);goto L8c0b38da; /* fmov fr15,fr0 */
L8c0b38da: *stop=0x8c0b38dau;r[0]+=r[3];goto L8c0b38dc; /* add r3,r0 */
L8c0b38dc: *stop=0x8c0b38dcu;fcopy(f,2u,14u);goto L8c0b38de; /* fmov fr14,fr2 */
L8c0b38de: *stop=0x8c0b38deu;r[0]<<=2;goto L8c0b38e0; /* shll2 r0 */
L8c0b38e0: *stop=0x8c0b38e0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b38e2; /* shll r0 */
L8c0b38e2: *stop=0x8c0b38e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b38e4; /* fmac fr0,fr3,fr2 */
L8c0b38e4: *stop=0x8c0b38e4u;r[0]+=r[2];goto L8c0b38e6; /* add r2,r0 */
L8c0b38e6: *stop=0x8c0b38e6u;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]+r[1]);if(io_status)return io_status;goto L8c0b38e8; /* fmov.s @(r0,r1),fr0 */
L8c0b38e8: *stop=0x8c0b38e8u;r[0]=8u;goto L8c0b38ea; /* mov #0x8,r0 */
L8c0b38ea: *stop=0x8c0b38eau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b38ec; /* fmov.s @(r0,r15),fr3 */
L8c0b38ec: *stop=0x8c0b38ecu;r[0]=8u;goto L8c0b38ee; /* mov #0x8,r0 */
L8c0b38ee: *stop=0x8c0b38eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b38f0; /* fmul fr0,fr2 */
L8c0b38f0: *stop=0x8c0b38f0u;fcopy(f,0u,13u);goto L8c0b38f2; /* fmov fr13,fr0 */
L8c0b38f2: *stop=0x8c0b38f2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0b38f4; /* fmac fr0,fr2,fr3 */
L8c0b38f4: target=r[13];*pr=0x8c0b38f8u;*stop=0x8c0b38f6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b38f8; /* jsr @r13 */
L8c0b38f8: *stop=0x8c0b38f8u;*fpul=r[0];goto L8c0b38fa; /* lds r0,FPUL */
L8c0b38fa: *stop=0x8c0b38fau;r[1]=48u;goto L8c0b38fc; /* mov #0x30,r1 */
L8c0b38fc: *stop=0x8c0b38fcu;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b38fe; /* mov.b @(0x3,r14),r0 */
L8c0b38fe: *stop=0x8c0b38feu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3900; /* mov.l @(0x8,r14),r2 */
L8c0b3900: *stop=0x8c0b3900u;r[0]=r[0]&255u;goto L8c0b3902; /* extu.b r0,r0 */
L8c0b3902: *stop=0x8c0b3902u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c0b3904; /* float FPUL,fr3 */
L8c0b3904: *stop=0x8c0b3904u;r[3]=r[0];goto L8c0b3906; /* mov r0,r3 */
L8c0b3906: *stop=0x8c0b3906u;r[0]<<=2;goto L8c0b3908; /* shll2 r0 */
L8c0b3908: *stop=0x8c0b3908u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b390a; /* shll r0 */
L8c0b390a: *stop=0x8c0b390au;fcopy(f,0u,15u);goto L8c0b390c; /* fmov fr15,fr0 */
L8c0b390c: *stop=0x8c0b390cu;r[0]+=r[3];goto L8c0b390e; /* add r3,r0 */
L8c0b390e: *stop=0x8c0b390eu;fcopy(f,2u,14u);goto L8c0b3910; /* fmov fr14,fr2 */
L8c0b3910: *stop=0x8c0b3910u;r[0]<<=2;goto L8c0b3912; /* shll2 r0 */
L8c0b3912: *stop=0x8c0b3912u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3914; /* shll r0 */
L8c0b3914: *stop=0x8c0b3914u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3916; /* fmac fr0,fr3,fr2 */
L8c0b3916: *stop=0x8c0b3916u;r[0]+=r[2];goto L8c0b3918; /* add r2,r0 */
L8c0b3918: *stop=0x8c0b3918u;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]+r[1]);if(io_status)return io_status;goto L8c0b391a; /* fmov.s @(r0,r1),fr0 */
L8c0b391a: *stop=0x8c0b391au;r[0]=12u;goto L8c0b391c; /* mov #0xc,r0 */
L8c0b391c: *stop=0x8c0b391cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b391e; /* fmov.s @(r0,r15),fr3 */
L8c0b391e: *stop=0x8c0b391eu;r[0]=12u;goto L8c0b3920; /* mov #0xc,r0 */
L8c0b3920: *stop=0x8c0b3920u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3922; /* fmul fr0,fr2 */
L8c0b3922: *stop=0x8c0b3922u;fcopy(f,0u,13u);goto L8c0b3924; /* fmov fr13,fr0 */
L8c0b3924: *stop=0x8c0b3924u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0b3926; /* fmac fr0,fr2,fr3 */
L8c0b3926: target=r[13];*pr=0x8c0b392au;*stop=0x8c0b3928u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b392a; /* jsr @r13 */
L8c0b392a: *stop=0x8c0b392au;*fpul=r[0];goto L8c0b392c; /* lds r0,FPUL */
L8c0b392c: *stop=0x8c0b392cu;r[1]=48u;goto L8c0b392e; /* mov #0x30,r1 */
L8c0b392e: *stop=0x8c0b392eu;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b3930; /* mov.b @(0x3,r14),r0 */
L8c0b3930: *stop=0x8c0b3930u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3932; /* mov.l @(0x8,r14),r2 */
L8c0b3932: *stop=0x8c0b3932u;r[0]=r[0]&255u;goto L8c0b3934; /* extu.b r0,r0 */
L8c0b3934: *stop=0x8c0b3934u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c0b3936; /* float FPUL,fr3 */
L8c0b3936: *stop=0x8c0b3936u;r[3]=r[0];goto L8c0b3938; /* mov r0,r3 */
L8c0b3938: *stop=0x8c0b3938u;r[0]<<=2;goto L8c0b393a; /* shll2 r0 */
L8c0b393a: *stop=0x8c0b393au;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b393c; /* shll r0 */
L8c0b393c: *stop=0x8c0b393cu;fcopy(f,2u,14u);goto L8c0b393e; /* fmov fr14,fr2 */
L8c0b393e: *stop=0x8c0b393eu;r[0]+=r[3];goto L8c0b3940; /* add r3,r0 */
L8c0b3940: *stop=0x8c0b3940u;fcopy(f,0u,15u);goto L8c0b3942; /* fmov fr15,fr0 */
L8c0b3942: *stop=0x8c0b3942u;r[0]<<=2;goto L8c0b3944; /* shll2 r0 */
L8c0b3944: *stop=0x8c0b3944u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3946; /* shll r0 */
L8c0b3946: *stop=0x8c0b3946u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3948; /* fmac fr0,fr3,fr2 */
L8c0b3948: *stop=0x8c0b3948u;r[0]+=r[2];goto L8c0b394a; /* add r2,r0 */
L8c0b394a: *stop=0x8c0b394au;r[2]=r[15];goto L8c0b394c; /* mov r15,r2 */
L8c0b394c: *stop=0x8c0b394cu;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]+r[1]);if(io_status)return io_status;goto L8c0b394e; /* fmov.s @(r0,r1),fr0 */
L8c0b394e: *stop=0x8c0b394eu;r[0]=16u;goto L8c0b3950; /* mov #0x10,r0 */
L8c0b3950: *stop=0x8c0b3950u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b3952; /* fmov.s @(r0,r15),fr3 */
L8c0b3952: *stop=0x8c0b3952u;r[2]+=20u;goto L8c0b3954; /* add #0x14,r2 */
L8c0b3954: *stop=0x8c0b3954u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3956; /* fmul fr0,fr2 */
L8c0b3956: *stop=0x8c0b3956u;fcopy(f,0u,13u);goto L8c0b3958; /* fmov fr13,fr0 */
L8c0b3958: *stop=0x8c0b3958u;r[0]=16u;goto L8c0b395a; /* mov #0x10,r0 */
L8c0b395a: *stop=0x8c0b395au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0b395c; /* fmac fr0,fr2,fr3 */
L8c0b395c: *stop=0x8c0b395cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0b395e; /* fmov fr3,@(r0,r15) */
L8c0b395e: *stop=0x8c0b395eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3960; /* mov.l @r15,r3 */
L8c0b3960: *stop=0x8c0b3960u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b3962; /* mov.l @r2,r1 */
L8c0b3962: *stop=0x8c0b3962u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0b3964; /* mov.l @(0x4,r2),r0 */
L8c0b3964: *stop=0x8c0b3964u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c0b3966; /* mov.l r1,@(0x4,r3) */
L8c0b3966: *stop=0x8c0b3966u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[0],4);if(io_status)return io_status;goto L8c0b3968; /* mov.l r0,@(0x8,r3) */
L8c0b3968: *stop=0x8c0b3968u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b396a; /* mov.l @(0x8,r2),r1 */
L8c0b396a: *stop=0x8c0b396au;if(!valid_access(r[3]+12u,4u))return -3;wr(m,bus,&io_status,r[3]+0xcu,r[1],4);if(io_status)return io_status;goto L8c0b396c; /* mov.l r1,@(0xc,r3) */
L8c0b396c: *stop=0x8c0b396cu;r[3]=r[15];goto L8c0b396e; /* mov r15,r3 */
L8c0b396e: *stop=0x8c0b396eu;r[3]+=8u;goto L8c0b3970; /* add #0x8,r3 */
L8c0b3970: *stop=0x8c0b3970u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3972; /* mov.l @r15,r2 */
L8c0b3972: *stop=0x8c0b3972u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b3974; /* mov.l @r3,r1 */
L8c0b3974: *stop=0x8c0b3974u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0b3976; /* mov.l @(0x4,r3),r0 */
L8c0b3976: *stop=0x8c0b3976u;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[1],4);if(io_status)return io_status;goto L8c0b3978; /* mov.l r1,@(0x10,r2) */
L8c0b3978: *stop=0x8c0b3978u;if(!valid_access(r[2]+20u,4u))return -3;wr(m,bus,&io_status,r[2]+0x14u,r[0],4);if(io_status)return io_status;goto L8c0b397a; /* mov.l r0,@(0x14,r2) */
L8c0b397a: *stop=0x8c0b397au;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b397c; /* mov.l @(0x8,r3),r1 */
L8c0b397c: *stop=0x8c0b397cu;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[1],4);if(io_status)return io_status;goto L8c0b397e; /* mov.l r1,@(0x18,r2) */
L8c0b397e: *stop=0x8c0b397eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}goto L8c0b3980; /* mov.l @r15,r8 */
L8c0b3980: target=r[13];*pr=0x8c0b3984u;*stop=0x8c0b3982u;r[8]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3984; /* jsr @r13 */
L8c0b3984: *stop=0x8c0b3984u;*fpul=r[0];goto L8c0b3986; /* lds r0,FPUL */
L8c0b3986: *stop=0x8c0b3986u;r[0]=0x8c0b3ab4u;goto L8c0b3988; /* mova 0x8c0b3ab4,r0 */
L8c0b3988: *stop=0x8c0b3988u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c0b398a; /* fmov.s @r0,fr2 */
L8c0b398a: *stop=0x8c0b398au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c0b398c; /* float FPUL,fr3 */
L8c0b398c: *stop=0x8c0b398cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0b398e; /* fmul fr2,fr3 */
L8c0b398e: *stop=0x8c0b398eu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c0b3990; /* ftrc fr3,FPUL */
L8c0b3990: *stop=0x8c0b3990u;r[0]=*fpul;goto L8c0b3992; /* sts FPUL,r0 */
L8c0b3992: *stop=0x8c0b3992u;if(!valid_access(r[8],2u))return -3;wr(m,bus,&io_status,r[8],r[0],2);if(io_status)return io_status;goto L8c0b3994; /* mov.w r0,@r8 */
L8c0b3994: *stop=0x8c0b3994u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}goto L8c0b3996; /* mov.l @r15,r8 */
L8c0b3996: target=r[13];*pr=0x8c0b399au;*stop=0x8c0b3998u;r[8]+=30u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b399a; /* jsr @r13 */
L8c0b399a: *stop=0x8c0b399au;*fpul=r[0];goto L8c0b399c; /* lds r0,FPUL */
L8c0b399c: *stop=0x8c0b399cu;r[1]=56u;goto L8c0b399e; /* mov #0x38,r1 */
L8c0b399e: *stop=0x8c0b399eu;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b39a0; /* mov.b @(0x3,r14),r0 */
L8c0b39a0: *stop=0x8c0b39a0u;fcopy(f,0u,15u);goto L8c0b39a2; /* fmov fr15,fr0 */
L8c0b39a2: *stop=0x8c0b39a2u;r[0]=r[0]&255u;goto L8c0b39a4; /* extu.b r0,r0 */
L8c0b39a4: *stop=0x8c0b39a4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c0b39a6; /* float FPUL,fr3 */
L8c0b39a6: *stop=0x8c0b39a6u;r[3]=r[0];goto L8c0b39a8; /* mov r0,r3 */
L8c0b39a8: *stop=0x8c0b39a8u;r[0]<<=2;goto L8c0b39aa; /* shll2 r0 */
L8c0b39aa: *stop=0x8c0b39aau;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b39ac; /* shll r0 */
L8c0b39ac: *stop=0x8c0b39acu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b39ae; /* mov.l @(0x8,r14),r2 */
L8c0b39ae: *stop=0x8c0b39aeu;r[0]+=r[3];goto L8c0b39b0; /* add r3,r0 */
L8c0b39b0: *stop=0x8c0b39b0u;r[0]<<=2;goto L8c0b39b2; /* shll2 r0 */
L8c0b39b2: *stop=0x8c0b39b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[14],&f->fpscr,&fr[14]);if(status)return status;}goto L8c0b39b4; /* fmac fr0,fr3,fr14 */
L8c0b39b4: *stop=0x8c0b39b4u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b39b6; /* shll r0 */
L8c0b39b6: *stop=0x8c0b39b6u;r[0]+=r[2];goto L8c0b39b8; /* add r2,r0 */
L8c0b39b8: *stop=0x8c0b39b8u;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c0b39ba; /* fmov.s @(r0,r1),fr3 */
L8c0b39ba: *stop=0x8c0b39bau;r[0]=0x8c0b3ab8u;goto L8c0b39bc; /* mova 0x8c0b3ab8,r0 */
L8c0b39bc: *stop=0x8c0b39bcu;fcopy(f,2u,14u);goto L8c0b39be; /* fmov fr14,fr2 */
L8c0b39be: *stop=0x8c0b39beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b39c0; /* fmul fr3,fr2 */
L8c0b39c0: *stop=0x8c0b39c0u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0b39c2; /* fmov.s @r0,fr3 */
L8c0b39c2: *stop=0x8c0b39c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b39c4; /* fmul fr3,fr2 */
L8c0b39c4: *stop=0x8c0b39c4u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c0b39c6; /* ftrc fr2,FPUL */
L8c0b39c6: *stop=0x8c0b39c6u;r[0]=*fpul;goto L8c0b39c8; /* sts FPUL,r0 */
L8c0b39c8: *stop=0x8c0b39c8u;if(!valid_access(r[8],2u))return -3;wr(m,bus,&io_status,r[8],r[0],2);if(io_status)return io_status;goto L8c0b39ca; /* mov.w r0,@r8 */
L8c0b39ca: *stop=0x8c0b39cau;r[0]=123u;goto L8c0b39cc; /* mov #0x7b,r0 */
L8c0b39cc: *stop=0x8c0b39ccu;r[0]<<=2;goto L8c0b39ce; /* shll2 r0 */
L8c0b39ce: *stop=0x8c0b39ceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b39d0; /* mov.l @r15,r3 */
L8c0b39d0: *stop=0x8c0b39d0u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c0b39d2; /* fmov.s @(r0,r12),fr2 */
L8c0b39d2: *stop=0x8c0b39d2u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c0b39d4; /* fmov.s fr2,@r3 */
L8c0b39d4: *stop=0x8c0b39d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}goto L8c0b39d6; /* mov.l @r15,r8 */
L8c0b39d6: target=r[13];*pr=0x8c0b39dau;*stop=0x8c0b39d8u;r[8]+=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b39da; /* jsr @r13 */
L8c0b39da: *stop=0x8c0b39dau;*fpul=r[0];goto L8c0b39dc; /* lds r0,FPUL */
L8c0b39dc: *stop=0x8c0b39dcu;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b39de; /* mov.b @(0x3,r14),r0 */
L8c0b39de: *stop=0x8c0b39deu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b39e0; /* mov.l @(0x8,r14),r1 */
L8c0b39e0: *stop=0x8c0b39e0u;r[0]=r[0]&255u;goto L8c0b39e2; /* extu.b r0,r0 */
L8c0b39e2: *stop=0x8c0b39e2u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c0b39e4; /* float FPUL,fr3 */
L8c0b39e4: *stop=0x8c0b39e4u;r[2]=r[0];goto L8c0b39e6; /* mov r0,r2 */
L8c0b39e6: *stop=0x8c0b39e6u;r[0]<<=2;goto L8c0b39e8; /* shll2 r0 */
L8c0b39e8: *stop=0x8c0b39e8u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b39ea; /* shll r0 */
L8c0b39ea: *stop=0x8c0b39eau;fcopy(f,1u,12u);goto L8c0b39ec; /* fmov fr12,fr1 */
L8c0b39ec: *stop=0x8c0b39ecu;r[0]+=r[2];goto L8c0b39ee; /* add r2,r0 */
L8c0b39ee: *stop=0x8c0b39eeu;r[0]<<=2;goto L8c0b39f0; /* shll2 r0 */
L8c0b39f0: *stop=0x8c0b39f0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b39f2; /* shll r0 */
L8c0b39f2: *stop=0x8c0b39f2u;r[2]=36u;goto L8c0b39f4; /* mov #0x24,r2 */
L8c0b39f4: *stop=0x8c0b39f4u;r[0]+=r[1];goto L8c0b39f6; /* add r1,r0 */
L8c0b39f6: *stop=0x8c0b39f6u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c0b39f8; /* fmov.s @(r0,r2),fr2 */
L8c0b39f8: *stop=0x8c0b39f8u;r[0]=0x8c0b3abcu;goto L8c0b39fa; /* mova 0x8c0b3abc,r0 */
L8c0b39fa: *stop=0x8c0b39fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0b39fc; /* fmul fr2,fr3 */
L8c0b39fc: *stop=0x8c0b39fcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c0b39fe; /* fldi1 fr2 */
L8c0b39fe: *stop=0x8c0b39feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0b3a00; /* fmul fr15,fr3 */
L8c0b3a00: *stop=0x8c0b3a00u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c0b3a02; /* fneg fr3 */
L8c0b3a02: *stop=0x8c0b3a02u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c0b3a04; /* fadd fr2,fr3 */
L8c0b3a04: *stop=0x8c0b3a04u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c0b3a06; /* fmov.s fr3,@r8 */
L8c0b3a06: *stop=0x8c0b3a06u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0b3a08; /* fmov.s @r0,fr3 */
L8c0b3a08: *stop=0x8c0b3a08u;r[0]=r[2];goto L8c0b3a0a; /* mov r2,r0 */
L8c0b3a0a: *stop=0x8c0b3a0au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3a0c; /* mov.l @r15,r3 */
L8c0b3a0c: *stop=0x8c0b3a0cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c0b3a0e; /* fmul fr3,fr1 */
L8c0b3a0e: *stop=0x8c0b3a0eu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[3]);if(io_status)return io_status;goto L8c0b3a10; /* fmov fr1,@(r0,r3) */
L8c0b3a10: *stop=0x8c0b3a10u;r[0]=40u;goto L8c0b3a12; /* mov #0x28,r0 */
L8c0b3a12: *stop=0x8c0b3a12u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3a14; /* mov.l @r15,r3 */
L8c0b3a14: *stop=0x8c0b3a14u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,12u,r[0]+r[3]);if(io_status)return io_status;goto L8c0b3a16; /* fmov fr12,@(r0,r3) */
L8c0b3a16: *stop=0x8c0b3a16u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c0b3a18; /* mov.l @r15,r0 */
L8c0b3a18: *stop=0x8c0b3a18u;r[15]+=32u;goto L8c0b3a1a; /* add #0x20,r15 */
L8c0b3a1a: *stop=0x8c0b3a1au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0b3a1c; /* lds.l @r15+,PR */
L8c0b3a1c: *stop=0x8c0b3a1cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3a1e; /* fmov.s @r15+,fr12 */
L8c0b3a1e: *stop=0x8c0b3a1eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3a20; /* fmov.s @r15+,fr13 */
L8c0b3a20: *stop=0x8c0b3a20u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3a22; /* fmov.s @r15+,fr14 */
L8c0b3a22: *stop=0x8c0b3a22u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0b3a24; /* fmov.s @r15+,fr15 */
L8c0b3a24: *stop=0x8c0b3a24u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c0b3a26; /* mov.l @r15+,r8 */
L8c0b3a26: *stop=0x8c0b3a26u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c0b3a28; /* mov.l @r15+,r12 */
L8c0b3a28: *stop=0x8c0b3a28u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0b3a2a; /* mov.l @r15+,r13 */
L8c0b3a2a: *stop=0x8c0b3a2au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c0b3a2cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_child_update_8c0b37f2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b37f2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3818(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b37f2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3818u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b383a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b37f2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b383au);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3860(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b37f2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3860u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3898(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b37f2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3898u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b38c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b37f2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b38c6u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b38f8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b37f2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b38f8u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b392a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b37f2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b392au);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3984(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b37f2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3984u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b399a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b37f2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b399au);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b39da(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b37f2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b39dau);}

static int sa_player_child_update_run_8c0b3480(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c0b3410u)goto L8c0b3410;if(resume_pc==0x8c0b341au)goto L8c0b341a;if(resume_pc==0x8c0b342au)goto L8c0b342a;if(resume_pc==0x8c0b343au)goto L8c0b343a;if(resume_pc==0x8c0b3450u)goto L8c0b3450;if(resume_pc==0x8c0b345au)goto L8c0b345a;if(resume_pc==0x8c0b3464u)goto L8c0b3464;if(resume_pc==0x8c0b3472u)goto L8c0b3472;if(resume_pc==0x8c0b349eu)goto L8c0b349e;goto L8c0b3480;
L8c0b33ee: *stop=0x8c0b33eeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0b33f0; /* mov.l r14,@-r15 */
L8c0b33f0: *stop=0x8c0b33f0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0b33f2; /* mov.l r13,@-r15 */
L8c0b33f2: *stop=0x8c0b33f2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0b33f4; /* sts.l PR,@-r15 */
L8c0b33f4: *stop=0x8c0b33f4u;r[15]+=4294967284u;goto L8c0b33f6; /* add #-0xc,r15 */
L8c0b33f6: *stop=0x8c0b33f6u;r[14]=r[4];goto L8c0b33f8; /* mov r4,r14 */
L8c0b33f8: *stop=0x8c0b33f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b34e8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0b33fa; /* mov.l 0x8c0b34e8,r0 */
L8c0b33fa: *stop=0x8c0b33fau;r[14]<<=2;goto L8c0b33fc; /* shll2 r14 */
L8c0b33fc: *stop=0x8c0b33fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b34ecu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0b33fe; /* mov.l 0x8c0b34ec,r4 */
L8c0b33fe: *stop=0x8c0b33feu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[5],4);if(io_status)return io_status;goto L8c0b3400; /* mov.l r5,@(0x8,r15) */
L8c0b3400: *stop=0x8c0b3400u;r[13]=r[7];goto L8c0b3402; /* mov r7,r13 */
L8c0b3402: *stop=0x8c0b3402u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c0b3404; /* mov.l r6,@(0x4,r15) */
L8c0b3404: *stop=0x8c0b3404u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[14]=loaded;}goto L8c0b3406; /* mov.l @(r0,r14),r14 */
L8c0b3406: *stop=0x8c0b3406u;if(!valid_access(r[14]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3408; /* mov.l @(0x38,r14),r3 */
L8c0b3408: *stop=0x8c0b3408u;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b340a; /* mov.l @(0xc,r3),r2 */
L8c0b340a: *stop=0x8c0b340au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b34f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b340c; /* mov.l 0x8c0b34f0,r3 */
L8c0b340c: target=r[3];*pr=0x8c0b3410u;*stop=0x8c0b340eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3410; /* jsr @r3 */
L8c0b3410: *stop=0x8c0b3410u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c0b3412; /* mov.l @r15,r5 */
L8c0b3412: *stop=0x8c0b3412u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b34f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3414; /* mov.l 0x8c0b34f4,r3 */
L8c0b3414: *stop=0x8c0b3414u;r[5]+=8u;goto L8c0b3416; /* add #0x8,r5 */
L8c0b3416: target=r[3];*pr=0x8c0b341au;*stop=0x8c0b3418u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b341a; /* jsr @r3 */
L8c0b341a: *stop=0x8c0b341au;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b341c; /* mov.l @(0x1c,r14),r2 */
L8c0b341c: *stop=0x8c0b341cu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c0b341e; /* tst r2,r2 */
L8c0b341e: take=(*sr&1u);*stop=0x8c0b3420u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c0b342a;goto L8c0b3422; /* bt/s 0x8c0b342a */
L8c0b3422: *stop=0x8c0b3422u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b34f8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3424; /* mov.l 0x8c0b34f8,r3 */
L8c0b3424: *stop=0x8c0b3424u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c0b3426; /* mov.l @r15,r5 */
L8c0b3426: target=r[3];*pr=0x8c0b342au;*stop=0x8c0b3428u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b342a; /* jsr @r3 */
L8c0b342a: *stop=0x8c0b342au;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b342c; /* mov.l @(0x14,r14),r3 */
L8c0b342c: *stop=0x8c0b342cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0b342e; /* tst r3,r3 */
L8c0b342e: take=(*sr&1u);*stop=0x8c0b3430u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c0b343a;goto L8c0b3432; /* bt/s 0x8c0b343a */
L8c0b3432: *stop=0x8c0b3432u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b34fcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3434; /* mov.l 0x8c0b34fc,r3 */
L8c0b3434: *stop=0x8c0b3434u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c0b3436; /* mov.l @r15,r5 */
L8c0b3436: target=r[3];*pr=0x8c0b343au;*stop=0x8c0b3438u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b343a; /* jsr @r3 */
L8c0b343a: *stop=0x8c0b343au;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b343c; /* mov.l @(0x18,r14),r3 */
L8c0b343c: *stop=0x8c0b343cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3500u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b343e; /* mov.l 0x8c0b3500,r2 */
L8c0b343e: *stop=0x8c0b343eu;r[3]=0u-r[3];goto L8c0b3440; /* neg r3,r3 */
L8c0b3440: *stop=0x8c0b3440u;r[3]+=r[2];goto L8c0b3442; /* add r2,r3 */
L8c0b3442: *stop=0x8c0b3442u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0b3444; /* tst r3,r3 */
L8c0b3444: take=(*sr&1u);*stop=0x8c0b3446u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c0b3450;goto L8c0b3448; /* bt/s 0x8c0b3450 */
L8c0b3448: *stop=0x8c0b3448u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3504u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b344a; /* mov.l 0x8c0b3504,r1 */
L8c0b344a: *stop=0x8c0b344au;r[5]=r[3];goto L8c0b344c; /* mov r3,r5 */
L8c0b344c: target=r[1];*pr=0x8c0b3450u;*stop=0x8c0b344eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3450; /* jsr @r1 */
L8c0b3450: *stop=0x8c0b3450u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c0b3452; /* mov.l @(0x8,r15),r6 */
L8c0b3452: *stop=0x8c0b3452u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3508u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3454; /* mov.l 0x8c0b3508,r3 */
L8c0b3454: *stop=0x8c0b3454u;r[5]=r[6];goto L8c0b3456; /* mov r6,r5 */
L8c0b3456: target=r[3];*pr=0x8c0b345au;*stop=0x8c0b3458u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b345a; /* jsr @r3 */
L8c0b345a: *stop=0x8c0b345au;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c0b345c; /* mov.l @(0x4,r15),r6 */
L8c0b345c: *stop=0x8c0b345cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b350cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b345e; /* mov.l 0x8c0b350c,r3 */
L8c0b345e: *stop=0x8c0b345eu;r[5]=r[6];goto L8c0b3460; /* mov r6,r5 */
L8c0b3460: target=r[3];*pr=0x8c0b3464u;*stop=0x8c0b3462u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3464; /* jsr @r3 */
L8c0b3464: *stop=0x8c0b3464u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c0b3466; /* tst r13,r13 */
L8c0b3466: if((*sr&1u))goto L8c0b3472;goto L8c0b3468; /* bt 0x8c0b3472 */
L8c0b3468: *stop=0x8c0b3468u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3508u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b346a; /* mov.l 0x8c0b3508,r3 */
L8c0b346a: *stop=0x8c0b346au;r[5]=r[13];goto L8c0b346c; /* mov r13,r5 */
L8c0b346c: *stop=0x8c0b346cu;r[6]=r[13];goto L8c0b346e; /* mov r13,r6 */
L8c0b346e: target=r[3];*pr=0x8c0b3472u;*stop=0x8c0b3470u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b3472; /* jsr @r3 */
L8c0b3472: *stop=0x8c0b3472u;r[4]=1u;goto L8c0b3474; /* mov #0x1,r4 */
L8c0b3474: *stop=0x8c0b3474u;r[15]+=12u;goto L8c0b3476; /* add #0xc,r15 */
L8c0b3476: *stop=0x8c0b3476u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0b3478; /* lds.l @r15+,PR */
L8c0b3478: *stop=0x8c0b3478u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3510u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b347a; /* mov.l 0x8c0b3510,r2 */
L8c0b347a: *stop=0x8c0b347au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0b347c; /* mov.l @r15+,r13 */
L8c0b347c: target=r[2];*stop=0x8c0b347eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c0b3480: *stop=0x8c0b3480u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0b3482; /* mov.l r14,@-r15 */
L8c0b3482: *stop=0x8c0b3482u;r[0]=52u;goto L8c0b3484; /* mov #0x34,r0 */
L8c0b3484: *stop=0x8c0b3484u;r[14]=r[4];goto L8c0b3486; /* mov r4,r14 */
L8c0b3486: *stop=0x8c0b3486u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0b3488; /* mov.l r13,@-r15 */
L8c0b3488: *stop=0x8c0b3488u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c0b348a; /* mov.l r12,@-r15 */
L8c0b348a: *stop=0x8c0b348au;r[13]=r[5];goto L8c0b348c; /* mov r5,r13 */
L8c0b348c: *stop=0x8c0b348cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c0b348e; /* mov.l r11,@-r15 */
L8c0b348e: *stop=0x8c0b348eu;r[11]=r[6];goto L8c0b3490; /* mov r6,r11 */
L8c0b3490: *stop=0x8c0b3490u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0b3492; /* sts.l PR,@-r15 */
L8c0b3492: *stop=0x8c0b3492u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0b3494; /* fmov.s @(r0,r14),fr3 */
L8c0b3494: *stop=0x8c0b3494u;r[12]=r[7];goto L8c0b3496; /* mov r7,r12 */
L8c0b3496: *stop=0x8c0b3496u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b3514u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3498; /* mov.l 0x8c0b3514,r3 */
L8c0b3498: *stop=0x8c0b3498u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c0b349a; /* ftrc fr3,FPUL */
L8c0b349a: target=r[3];*pr=0x8c0b349eu;*stop=0x8c0b349cu;r[4]=*fpul;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b349e; /* jsr @r3 */
L8c0b349e: *stop=0x8c0b349eu;r[4]=r[0];goto L8c0b34a0; /* mov r0,r4 */
L8c0b34a0: *stop=0x8c0b34a0u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b34a2; /* mov.b @(0x3,r14),r0 */
L8c0b34a2: *stop=0x8c0b34a2u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b34a4; /* mov.l @(0x8,r14),r2 */
L8c0b34a4: *stop=0x8c0b34a4u;r[0]=r[0]&255u;goto L8c0b34a6; /* extu.b r0,r0 */
L8c0b34a6: *stop=0x8c0b34a6u;r[3]=r[0];goto L8c0b34a8; /* mov r0,r3 */
L8c0b34a8: *stop=0x8c0b34a8u;r[0]<<=2;goto L8c0b34aa; /* shll2 r0 */
L8c0b34aa: *stop=0x8c0b34aau;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b34ac; /* shll r0 */
L8c0b34ac: *stop=0x8c0b34acu;r[0]+=r[3];goto L8c0b34ae; /* add r3,r0 */
L8c0b34ae: *stop=0x8c0b34aeu;r[0]<<=2;goto L8c0b34b0; /* shll2 r0 */
L8c0b34b0: *stop=0x8c0b34b0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b34b2; /* shll r0 */
L8c0b34b2: *stop=0x8c0b34b2u;r[0]+=r[2];goto L8c0b34b4; /* add r2,r0 */
L8c0b34b4: *stop=0x8c0b34b4u;if(!valid_access(r[0]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x18u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0b34b6; /* mov.l @(0x18,r0),r0 */
L8c0b34b6: *stop=0x8c0b34b6u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c0b34b8; /* cmp/eq #0x0,r0 */
L8c0b34b8: if((*sr&1u))goto L8c0b3520;goto L8c0b34ba; /* bt 0x8c0b3520 */
L8c0b34ba: *stop=0x8c0b34bau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c0b34bc; /* cmp/eq #0x1,r0 */
L8c0b34bc: if(!(*sr&1u))goto L8c0b34c2;goto L8c0b34be; /* bf 0x8c0b34c2 */
L8c0b34be: *stop=0x8c0b34c0u;goto L8c0b363c; /* bra 0x8c0b363c */
L8c0b34c2: *stop=0x8c0b34c4u;goto L8c0b3754; /* bra 0x8c0b3754 */
L8c0b3520: *stop=0x8c0b3520u;r[3]=113u;goto L8c0b3522; /* mov #0x71,r3 */
L8c0b3522: *stop=0x8c0b3522u;r[5]=r[11];goto L8c0b3524; /* mov r11,r5 */
L8c0b3524: *stop=0x8c0b3524u;r[3]<<=2;goto L8c0b3526; /* shll2 r3 */
L8c0b3526: *stop=0x8c0b3526u;r[3]+=r[4];goto L8c0b3528; /* add r4,r3 */
L8c0b3528: *stop=0x8c0b3528u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b352a; /* mov.l @r3,r2 */
L8c0b352a: *stop=0x8c0b352au;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b352c; /* mov.l @(0x4,r3),r1 */
L8c0b352c: *stop=0x8c0b352cu;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[2],4);if(io_status)return io_status;goto L8c0b352e; /* mov.l r2,@r11 */
L8c0b352e: *stop=0x8c0b352eu;if(!valid_access(r[11]+4u,4u))return -3;wr(m,bus,&io_status,r[11]+0x4u,r[1],4);if(io_status)return io_status;goto L8c0b3530; /* mov.l r1,@(0x4,r11) */
L8c0b3530: *stop=0x8c0b3530u;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3532; /* mov.l @(0x8,r3),r2 */
L8c0b3532: *stop=0x8c0b3532u;if(!valid_access(r[11]+8u,4u))return -3;wr(m,bus,&io_status,r[11]+0x8u,r[2],4);if(io_status)return io_status;goto L8c0b3534; /* mov.l r2,@(0x8,r11) */
L8c0b3534: *stop=0x8c0b3534u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b3536; /* mov.b @(0x3,r14),r0 */
L8c0b3536: *stop=0x8c0b3536u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b3538; /* mov.l @(0x8,r14),r1 */
L8c0b3538: *stop=0x8c0b3538u;r[0]=r[0]&255u;goto L8c0b353a; /* extu.b r0,r0 */
L8c0b353a: *stop=0x8c0b353au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b353c; /* fmov.s @r5,fr2 */
L8c0b353c: *stop=0x8c0b353cu;r[2]=r[0];goto L8c0b353e; /* mov r0,r2 */
L8c0b353e: *stop=0x8c0b353eu;r[0]<<=2;goto L8c0b3540; /* shll2 r0 */
L8c0b3540: *stop=0x8c0b3540u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3542; /* shll r0 */
L8c0b3542: *stop=0x8c0b3542u;r[0]+=r[2];goto L8c0b3544; /* add r2,r0 */
L8c0b3544: *stop=0x8c0b3544u;r[0]<<=2;goto L8c0b3546; /* shll2 r0 */
L8c0b3546: *stop=0x8c0b3546u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3548; /* shll r0 */
L8c0b3548: *stop=0x8c0b3548u;r[2]=20u;goto L8c0b354a; /* mov #0x14,r2 */
L8c0b354a: *stop=0x8c0b354au;r[0]+=r[1];goto L8c0b354c; /* add r1,r0 */
L8c0b354c: *stop=0x8c0b354cu;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c0b354e; /* fmov.s @(r0,r2),fr3 */
L8c0b354e: *stop=0x8c0b354eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3550; /* fmul fr3,fr2 */
L8c0b3550: *stop=0x8c0b3550u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3552; /* fmov.s fr2,@r5 */
L8c0b3552: *stop=0x8c0b3552u;r[5]+=4u;goto L8c0b3554; /* add #0x4,r5 */
L8c0b3554: *stop=0x8c0b3554u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b3556; /* mov.b @(0x3,r14),r0 */
L8c0b3556: *stop=0x8c0b3556u;r[0]=r[0]&255u;goto L8c0b3558; /* extu.b r0,r0 */
L8c0b3558: *stop=0x8c0b3558u;r[2]=r[0];goto L8c0b355a; /* mov r0,r2 */
L8c0b355a: *stop=0x8c0b355au;r[0]<<=2;goto L8c0b355c; /* shll2 r0 */
L8c0b355c: *stop=0x8c0b355cu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b355e; /* shll r0 */
L8c0b355e: *stop=0x8c0b355eu;r[0]+=r[2];goto L8c0b3560; /* add r2,r0 */
L8c0b3560: *stop=0x8c0b3560u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b3562; /* mov.l @(0x8,r14),r1 */
L8c0b3562: *stop=0x8c0b3562u;r[0]<<=2;goto L8c0b3564; /* shll2 r0 */
L8c0b3564: *stop=0x8c0b3564u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3566; /* shll r0 */
L8c0b3566: *stop=0x8c0b3566u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3568; /* fmov.s @r5,fr2 */
L8c0b3568: *stop=0x8c0b3568u;r[2]=20u;goto L8c0b356a; /* mov #0x14,r2 */
L8c0b356a: *stop=0x8c0b356au;r[0]+=r[1];goto L8c0b356c; /* add r1,r0 */
L8c0b356c: *stop=0x8c0b356cu;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c0b356e; /* fmov.s @(r0,r2),fr3 */
L8c0b356e: *stop=0x8c0b356eu;r[1]=20u;goto L8c0b3570; /* mov #0x14,r1 */
L8c0b3570: *stop=0x8c0b3570u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3572; /* fmul fr3,fr2 */
L8c0b3572: *stop=0x8c0b3572u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3574; /* fmov.s fr2,@r5 */
L8c0b3574: *stop=0x8c0b3574u;r[5]+=4u;goto L8c0b3576; /* add #0x4,r5 */
L8c0b3576: *stop=0x8c0b3576u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b3578; /* mov.b @(0x3,r14),r0 */
L8c0b3578: *stop=0x8c0b3578u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b357a; /* mov.l @(0x8,r14),r2 */
L8c0b357a: *stop=0x8c0b357au;r[0]=r[0]&255u;goto L8c0b357c; /* extu.b r0,r0 */
L8c0b357c: *stop=0x8c0b357cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b357e; /* fmov.s @r5,fr2 */
L8c0b357e: *stop=0x8c0b357eu;r[3]=r[0];goto L8c0b3580; /* mov r0,r3 */
L8c0b3580: *stop=0x8c0b3580u;r[0]<<=2;goto L8c0b3582; /* shll2 r0 */
L8c0b3582: *stop=0x8c0b3582u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3584; /* shll r0 */
L8c0b3584: *stop=0x8c0b3584u;r[0]+=r[3];goto L8c0b3586; /* add r3,r0 */
L8c0b3586: *stop=0x8c0b3586u;r[0]<<=2;goto L8c0b3588; /* shll2 r0 */
L8c0b3588: *stop=0x8c0b3588u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b358a; /* shll r0 */
L8c0b358a: *stop=0x8c0b358au;r[0]+=r[2];goto L8c0b358c; /* add r2,r0 */
L8c0b358c: *stop=0x8c0b358cu;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c0b358e; /* fmov.s @(r0,r1),fr3 */
L8c0b358e: *stop=0x8c0b358eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3590; /* fmul fr3,fr2 */
L8c0b3590: *stop=0x8c0b3590u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3592; /* fmov.s fr2,@r5 */
L8c0b3592: *stop=0x8c0b3592u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b3594; /* mov.b @(0x3,r14),r0 */
L8c0b3594: *stop=0x8c0b3594u;r[0]=r[0]&255u;goto L8c0b3596; /* extu.b r0,r0 */
L8c0b3596: *stop=0x8c0b3596u;r[3]=r[0];goto L8c0b3598; /* mov r0,r3 */
L8c0b3598: *stop=0x8c0b3598u;r[0]<<=2;goto L8c0b359a; /* shll2 r0 */
L8c0b359a: *stop=0x8c0b359au;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b359c; /* shll r0 */
L8c0b359c: *stop=0x8c0b359cu;r[0]+=r[3];goto L8c0b359e; /* add r3,r0 */
L8c0b359e: *stop=0x8c0b359eu;r[0]<<=2;goto L8c0b35a0; /* shll2 r0 */
L8c0b35a0: *stop=0x8c0b35a0u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0b35a2; /* mov.l @(0x8,r14),r5 */
L8c0b35a2: *stop=0x8c0b35a2u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b35a4; /* shll r0 */
L8c0b35a4: *stop=0x8c0b35a4u;r[5]+=r[0];goto L8c0b35a6; /* add r0,r5 */
L8c0b35a6: *stop=0x8c0b35a6u;r[0]=113u;goto L8c0b35a8; /* mov #0x71,r0 */
L8c0b35a8: *stop=0x8c0b35a8u;r[0]<<=2;goto L8c0b35aa; /* shll2 r0 */
L8c0b35aa: *stop=0x8c0b35aau;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b35ac; /* fmov.s @(r0,r4),fr0 */
L8c0b35ac: *stop=0x8c0b35acu;r[0]=16u;goto L8c0b35ae; /* mov #0x10,r0 */
L8c0b35ae: *stop=0x8c0b35aeu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c0b35b0; /* fmov.s @(r0,r5),fr3 */
L8c0b35b0: *stop=0x8c0b35b0u;r[0]=4u;goto L8c0b35b2; /* mov #0x4,r0 */
L8c0b35b2: *stop=0x8c0b35b2u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c0b35b4; /* fmov.s @(r0,r5),fr2 */
L8c0b35b4: *stop=0x8c0b35b4u;r[0]=95u;goto L8c0b35b6; /* mov #0x5f,r0 */
L8c0b35b6: *stop=0x8c0b35b6u;r[0]<<=2;goto L8c0b35b8; /* shll2 r0 */
L8c0b35b8: *stop=0x8c0b35b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b35ba; /* fmac fr0,fr3,fr2 */
L8c0b35ba: *stop=0x8c0b35bau;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b35bc; /* fmov.s @(r0,r4),fr3 */
L8c0b35bc: *stop=0x8c0b35bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b35be; /* fadd fr3,fr2 */
L8c0b35be: *stop=0x8c0b35beu;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c0b35c0; /* fmov.s fr2,@r13 */
L8c0b35c0: *stop=0x8c0b35c0u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b35c2; /* mov.b @(0x3,r14),r0 */
L8c0b35c2: *stop=0x8c0b35c2u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0b35c4; /* mov.l @(0x8,r14),r5 */
L8c0b35c4: *stop=0x8c0b35c4u;r[0]=r[0]&255u;goto L8c0b35c6; /* extu.b r0,r0 */
L8c0b35c6: *stop=0x8c0b35c6u;r[3]=r[0];goto L8c0b35c8; /* mov r0,r3 */
L8c0b35c8: *stop=0x8c0b35c8u;r[0]<<=2;goto L8c0b35ca; /* shll2 r0 */
L8c0b35ca: *stop=0x8c0b35cau;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b35cc; /* shll r0 */
L8c0b35cc: *stop=0x8c0b35ccu;r[0]+=r[3];goto L8c0b35ce; /* add r3,r0 */
L8c0b35ce: *stop=0x8c0b35ceu;r[0]<<=2;goto L8c0b35d0; /* shll2 r0 */
L8c0b35d0: *stop=0x8c0b35d0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b35d2; /* shll r0 */
L8c0b35d2: *stop=0x8c0b35d2u;r[5]+=r[0];goto L8c0b35d4; /* add r0,r5 */
L8c0b35d4: *stop=0x8c0b35d4u;r[0]=114u;goto L8c0b35d6; /* mov #0x72,r0 */
L8c0b35d6: *stop=0x8c0b35d6u;r[0]<<=2;goto L8c0b35d8; /* shll2 r0 */
L8c0b35d8: *stop=0x8c0b35d8u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b35da; /* fmov.s @(r0,r4),fr0 */
L8c0b35da: *stop=0x8c0b35dau;r[0]=16u;goto L8c0b35dc; /* mov #0x10,r0 */
L8c0b35dc: *stop=0x8c0b35dcu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c0b35de; /* fmov.s @(r0,r5),fr3 */
L8c0b35de: *stop=0x8c0b35deu;r[0]=8u;goto L8c0b35e0; /* mov #0x8,r0 */
L8c0b35e0: *stop=0x8c0b35e0u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c0b35e2; /* fmov.s @(r0,r5),fr2 */
L8c0b35e2: *stop=0x8c0b35e2u;r[0]=96u;goto L8c0b35e4; /* mov #0x60,r0 */
L8c0b35e4: *stop=0x8c0b35e4u;r[0]<<=2;goto L8c0b35e6; /* shll2 r0 */
L8c0b35e6: *stop=0x8c0b35e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b35e8; /* fmac fr0,fr3,fr2 */
L8c0b35e8: *stop=0x8c0b35e8u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b35ea; /* fmov.s @(r0,r4),fr3 */
L8c0b35ea: *stop=0x8c0b35eau;r[0]=4u;goto L8c0b35ec; /* mov #0x4,r0 */
L8c0b35ec: *stop=0x8c0b35ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b35ee; /* fadd fr3,fr2 */
L8c0b35ee: *stop=0x8c0b35eeu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0b35f0; /* fmov fr2,@(r0,r13) */
L8c0b35f0: *stop=0x8c0b35f0u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b35f2; /* mov.b @(0x3,r14),r0 */
L8c0b35f2: *stop=0x8c0b35f2u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0b35f4; /* mov.l @(0x8,r14),r5 */
L8c0b35f4: *stop=0x8c0b35f4u;r[0]=r[0]&255u;goto L8c0b35f6; /* extu.b r0,r0 */
L8c0b35f6: *stop=0x8c0b35f6u;r[3]=r[0];goto L8c0b35f8; /* mov r0,r3 */
L8c0b35f8: *stop=0x8c0b35f8u;r[0]<<=2;goto L8c0b35fa; /* shll2 r0 */
L8c0b35fa: *stop=0x8c0b35fau;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b35fc; /* shll r0 */
L8c0b35fc: *stop=0x8c0b35fcu;r[0]+=r[3];goto L8c0b35fe; /* add r3,r0 */
L8c0b35fe: *stop=0x8c0b35feu;r[0]<<=2;goto L8c0b3600; /* shll2 r0 */
L8c0b3600: *stop=0x8c0b3600u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3602; /* shll r0 */
L8c0b3602: *stop=0x8c0b3602u;r[5]+=r[0];goto L8c0b3604; /* add r0,r5 */
L8c0b3604: *stop=0x8c0b3604u;r[0]=115u;goto L8c0b3606; /* mov #0x73,r0 */
L8c0b3606: *stop=0x8c0b3606u;r[0]<<=2;goto L8c0b3608; /* shll2 r0 */
L8c0b3608: *stop=0x8c0b3608u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b360a; /* fmov.s @(r0,r4),fr0 */
L8c0b360a: *stop=0x8c0b360au;r[0]=16u;goto L8c0b360c; /* mov #0x10,r0 */
L8c0b360c: *stop=0x8c0b360cu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c0b360e; /* fmov.s @(r0,r5),fr3 */
L8c0b360e: *stop=0x8c0b360eu;r[0]=12u;goto L8c0b3610; /* mov #0xc,r0 */
L8c0b3610: *stop=0x8c0b3610u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c0b3612; /* fmov.s @(r0,r5),fr2 */
L8c0b3612: *stop=0x8c0b3612u;r[0]=97u;goto L8c0b3614; /* mov #0x61,r0 */
L8c0b3614: *stop=0x8c0b3614u;r[0]<<=2;goto L8c0b3616; /* shll2 r0 */
L8c0b3616: *stop=0x8c0b3616u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3618; /* fmac fr0,fr3,fr2 */
L8c0b3618: *stop=0x8c0b3618u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b361a; /* fmov.s @(r0,r4),fr3 */
L8c0b361a: *stop=0x8c0b361au;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c0b361c; /* tst r12,r12 */
L8c0b361c: *stop=0x8c0b361cu;r[0]=8u;goto L8c0b361e; /* mov #0x8,r0 */
L8c0b361e: *stop=0x8c0b361eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b3620; /* fadd fr3,fr2 */
L8c0b3620: take=!(*sr&1u);*stop=0x8c0b3622u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;if(take)goto L8c0b3628;goto L8c0b3624; /* bf/s 0x8c0b3628 */
L8c0b3624: *stop=0x8c0b3626u;goto L8c0b37d6; /* bra 0x8c0b37d6 */
L8c0b3628: *stop=0x8c0b3628u;r[2]=95u;goto L8c0b362a; /* mov #0x5f,r2 */
L8c0b362a: *stop=0x8c0b362au;r[2]<<=2;goto L8c0b362c; /* shll2 r2 */
L8c0b362c: *stop=0x8c0b362cu;r[2]+=r[4];goto L8c0b362e; /* add r4,r2 */
L8c0b362e: *stop=0x8c0b362eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3630; /* mov.l @r2,r3 */
L8c0b3630: *stop=0x8c0b3630u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b3632; /* mov.l @(0x4,r2),r1 */
L8c0b3632: *stop=0x8c0b3632u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[3],4);if(io_status)return io_status;goto L8c0b3634; /* mov.l r3,@r12 */
L8c0b3634: *stop=0x8c0b3634u;if(!valid_access(r[12]+4u,4u))return -3;wr(m,bus,&io_status,r[12]+0x4u,r[1],4);if(io_status)return io_status;goto L8c0b3636; /* mov.l r1,@(0x4,r12) */
L8c0b3636: *stop=0x8c0b3636u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3638; /* mov.l @(0x8,r2),r3 */
L8c0b3638: *stop=0x8c0b363au;if(!valid_access(r[12]+8u,4u))return -3;wr(m,bus,&io_status,r[12]+0x8u,r[3],4);if(io_status)return io_status;goto L8c0b37d6; /* bra 0x8c0b37d6 */
L8c0b363c: *stop=0x8c0b363cu;r[3]=116u;goto L8c0b363e; /* mov #0x74,r3 */
L8c0b363e: *stop=0x8c0b363eu;r[5]=r[11];goto L8c0b3640; /* mov r11,r5 */
L8c0b3640: *stop=0x8c0b3640u;r[3]<<=2;goto L8c0b3642; /* shll2 r3 */
L8c0b3642: *stop=0x8c0b3642u;r[3]+=r[4];goto L8c0b3644; /* add r4,r3 */
L8c0b3644: *stop=0x8c0b3644u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c0b3646; /* mov.l @r3,r0 */
L8c0b3646: *stop=0x8c0b3646u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3648; /* mov.l @(0x4,r3),r2 */
L8c0b3648: *stop=0x8c0b3648u;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[0],4);if(io_status)return io_status;goto L8c0b364a; /* mov.l r0,@r11 */
L8c0b364a: *stop=0x8c0b364au;if(!valid_access(r[11]+4u,4u))return -3;wr(m,bus,&io_status,r[11]+0x4u,r[2],4);if(io_status)return io_status;goto L8c0b364c; /* mov.l r2,@(0x4,r11) */
L8c0b364c: *stop=0x8c0b364cu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0b364e; /* mov.l @(0x8,r3),r0 */
L8c0b364e: *stop=0x8c0b364eu;if(!valid_access(r[11]+8u,4u))return -3;wr(m,bus,&io_status,r[11]+0x8u,r[0],4);if(io_status)return io_status;goto L8c0b3650; /* mov.l r0,@(0x8,r11) */
L8c0b3650: *stop=0x8c0b3650u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b3652; /* mov.b @(0x3,r14),r0 */
L8c0b3652: *stop=0x8c0b3652u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b3654; /* mov.l @(0x8,r14),r1 */
L8c0b3654: *stop=0x8c0b3654u;r[0]=r[0]&255u;goto L8c0b3656; /* extu.b r0,r0 */
L8c0b3656: *stop=0x8c0b3656u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3658; /* fmov.s @r5,fr2 */
L8c0b3658: *stop=0x8c0b3658u;r[2]=r[0];goto L8c0b365a; /* mov r0,r2 */
L8c0b365a: *stop=0x8c0b365au;r[0]<<=2;goto L8c0b365c; /* shll2 r0 */
L8c0b365c: *stop=0x8c0b365cu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b365e; /* shll r0 */
L8c0b365e: *stop=0x8c0b365eu;r[0]+=r[2];goto L8c0b3660; /* add r2,r0 */
L8c0b3660: *stop=0x8c0b3660u;r[0]<<=2;goto L8c0b3662; /* shll2 r0 */
L8c0b3662: *stop=0x8c0b3662u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3664; /* shll r0 */
L8c0b3664: *stop=0x8c0b3664u;r[2]=20u;goto L8c0b3666; /* mov #0x14,r2 */
L8c0b3666: *stop=0x8c0b3666u;r[0]+=r[1];goto L8c0b3668; /* add r1,r0 */
L8c0b3668: *stop=0x8c0b3668u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c0b366a; /* fmov.s @(r0,r2),fr3 */
L8c0b366a: *stop=0x8c0b366au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b366c; /* fmul fr3,fr2 */
L8c0b366c: *stop=0x8c0b366cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b366e; /* fmov.s fr2,@r5 */
L8c0b366e: *stop=0x8c0b366eu;r[5]+=4u;goto L8c0b3670; /* add #0x4,r5 */
L8c0b3670: *stop=0x8c0b3670u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b3672; /* mov.b @(0x3,r14),r0 */
L8c0b3672: *stop=0x8c0b3672u;r[0]=r[0]&255u;goto L8c0b3674; /* extu.b r0,r0 */
L8c0b3674: *stop=0x8c0b3674u;r[2]=r[0];goto L8c0b3676; /* mov r0,r2 */
L8c0b3676: *stop=0x8c0b3676u;r[0]<<=2;goto L8c0b3678; /* shll2 r0 */
L8c0b3678: *stop=0x8c0b3678u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b367a; /* shll r0 */
L8c0b367a: *stop=0x8c0b367au;r[0]+=r[2];goto L8c0b367c; /* add r2,r0 */
L8c0b367c: *stop=0x8c0b367cu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b367e; /* mov.l @(0x8,r14),r1 */
L8c0b367e: *stop=0x8c0b367eu;r[0]<<=2;goto L8c0b3680; /* shll2 r0 */
L8c0b3680: *stop=0x8c0b3680u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3682; /* shll r0 */
L8c0b3682: *stop=0x8c0b3682u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3684; /* fmov.s @r5,fr2 */
L8c0b3684: *stop=0x8c0b3684u;r[2]=20u;goto L8c0b3686; /* mov #0x14,r2 */
L8c0b3686: *stop=0x8c0b3686u;r[0]+=r[1];goto L8c0b3688; /* add r1,r0 */
L8c0b3688: *stop=0x8c0b3688u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c0b368a; /* fmov.s @(r0,r2),fr3 */
L8c0b368a: *stop=0x8c0b368au;r[1]=20u;goto L8c0b368c; /* mov #0x14,r1 */
L8c0b368c: *stop=0x8c0b368cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b368e; /* fmul fr3,fr2 */
L8c0b368e: *stop=0x8c0b368eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b3690; /* fmov.s fr2,@r5 */
L8c0b3690: *stop=0x8c0b3690u;r[5]+=4u;goto L8c0b3692; /* add #0x4,r5 */
L8c0b3692: *stop=0x8c0b3692u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b3694; /* mov.b @(0x3,r14),r0 */
L8c0b3694: *stop=0x8c0b3694u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3696; /* mov.l @(0x8,r14),r2 */
L8c0b3696: *stop=0x8c0b3696u;r[0]=r[0]&255u;goto L8c0b3698; /* extu.b r0,r0 */
L8c0b3698: *stop=0x8c0b3698u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b369a; /* fmov.s @r5,fr2 */
L8c0b369a: *stop=0x8c0b369au;r[3]=r[0];goto L8c0b369c; /* mov r0,r3 */
L8c0b369c: *stop=0x8c0b369cu;r[0]<<=2;goto L8c0b369e; /* shll2 r0 */
L8c0b369e: *stop=0x8c0b369eu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b36a0; /* shll r0 */
L8c0b36a0: *stop=0x8c0b36a0u;r[0]+=r[3];goto L8c0b36a2; /* add r3,r0 */
L8c0b36a2: *stop=0x8c0b36a2u;r[0]<<=2;goto L8c0b36a4; /* shll2 r0 */
L8c0b36a4: *stop=0x8c0b36a4u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b36a6; /* shll r0 */
L8c0b36a6: *stop=0x8c0b36a6u;r[0]+=r[2];goto L8c0b36a8; /* add r2,r0 */
L8c0b36a8: *stop=0x8c0b36a8u;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c0b36aa; /* fmov.s @(r0,r1),fr3 */
L8c0b36aa: *stop=0x8c0b36aau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b36ac; /* fmul fr3,fr2 */
L8c0b36ac: *stop=0x8c0b36acu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c0b36ae; /* fmov.s fr2,@r5 */
L8c0b36ae: *stop=0x8c0b36aeu;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b36b0; /* mov.b @(0x3,r14),r0 */
L8c0b36b0: *stop=0x8c0b36b0u;r[0]=r[0]&255u;goto L8c0b36b2; /* extu.b r0,r0 */
L8c0b36b2: *stop=0x8c0b36b2u;r[3]=r[0];goto L8c0b36b4; /* mov r0,r3 */
L8c0b36b4: *stop=0x8c0b36b4u;r[0]<<=2;goto L8c0b36b6; /* shll2 r0 */
L8c0b36b6: *stop=0x8c0b36b6u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b36b8; /* shll r0 */
L8c0b36b8: *stop=0x8c0b36b8u;r[0]+=r[3];goto L8c0b36ba; /* add r3,r0 */
L8c0b36ba: *stop=0x8c0b36bau;r[0]<<=2;goto L8c0b36bc; /* shll2 r0 */
L8c0b36bc: *stop=0x8c0b36bcu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0b36be; /* mov.l @(0x8,r14),r5 */
L8c0b36be: *stop=0x8c0b36beu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b36c0; /* shll r0 */
L8c0b36c0: *stop=0x8c0b36c0u;r[5]+=r[0];goto L8c0b36c2; /* add r0,r5 */
L8c0b36c2: *stop=0x8c0b36c2u;r[0]=116u;goto L8c0b36c4; /* mov #0x74,r0 */
L8c0b36c4: *stop=0x8c0b36c4u;r[0]<<=2;goto L8c0b36c6; /* shll2 r0 */
L8c0b36c6: *stop=0x8c0b36c6u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b36c8; /* fmov.s @(r0,r4),fr0 */
L8c0b36c8: *stop=0x8c0b36c8u;r[0]=16u;goto L8c0b36ca; /* mov #0x10,r0 */
L8c0b36ca: *stop=0x8c0b36cau;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c0b36cc; /* fmov.s @(r0,r5),fr3 */
L8c0b36cc: *stop=0x8c0b36ccu;r[0]=4u;goto L8c0b36ce; /* mov #0x4,r0 */
L8c0b36ce: *stop=0x8c0b36ceu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c0b36d0; /* fmov.s @(r0,r5),fr2 */
L8c0b36d0: *stop=0x8c0b36d0u;r[0]=98u;goto L8c0b36d2; /* mov #0x62,r0 */
L8c0b36d2: *stop=0x8c0b36d2u;r[0]<<=2;goto L8c0b36d4; /* shll2 r0 */
L8c0b36d4: *stop=0x8c0b36d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b36d6; /* fmac fr0,fr3,fr2 */
L8c0b36d6: *stop=0x8c0b36d6u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b36d8; /* fmov.s @(r0,r4),fr3 */
L8c0b36d8: *stop=0x8c0b36d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b36da; /* fadd fr3,fr2 */
L8c0b36da: *stop=0x8c0b36dau;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c0b36dc; /* fmov.s fr2,@r13 */
L8c0b36dc: *stop=0x8c0b36dcu;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b36de; /* mov.b @(0x3,r14),r0 */
L8c0b36de: *stop=0x8c0b36deu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0b36e0; /* mov.l @(0x8,r14),r5 */
L8c0b36e0: *stop=0x8c0b36e0u;r[0]=r[0]&255u;goto L8c0b36e2; /* extu.b r0,r0 */
L8c0b36e2: *stop=0x8c0b36e2u;r[3]=r[0];goto L8c0b36e4; /* mov r0,r3 */
L8c0b36e4: *stop=0x8c0b36e4u;r[0]<<=2;goto L8c0b36e6; /* shll2 r0 */
L8c0b36e6: *stop=0x8c0b36e6u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b36e8; /* shll r0 */
L8c0b36e8: *stop=0x8c0b36e8u;r[0]+=r[3];goto L8c0b36ea; /* add r3,r0 */
L8c0b36ea: *stop=0x8c0b36eau;r[0]<<=2;goto L8c0b36ec; /* shll2 r0 */
L8c0b36ec: *stop=0x8c0b36ecu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b36ee; /* shll r0 */
L8c0b36ee: *stop=0x8c0b36eeu;r[5]+=r[0];goto L8c0b36f0; /* add r0,r5 */
L8c0b36f0: *stop=0x8c0b36f0u;r[0]=117u;goto L8c0b36f2; /* mov #0x75,r0 */
L8c0b36f2: *stop=0x8c0b36f2u;r[0]<<=2;goto L8c0b36f4; /* shll2 r0 */
L8c0b36f4: *stop=0x8c0b36f4u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b36f6; /* fmov.s @(r0,r4),fr0 */
L8c0b36f6: *stop=0x8c0b36f6u;r[0]=16u;goto L8c0b36f8; /* mov #0x10,r0 */
L8c0b36f8: *stop=0x8c0b36f8u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c0b36fa; /* fmov.s @(r0,r5),fr3 */
L8c0b36fa: *stop=0x8c0b36fau;r[0]=8u;goto L8c0b36fc; /* mov #0x8,r0 */
L8c0b36fc: *stop=0x8c0b36fcu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c0b36fe; /* fmov.s @(r0,r5),fr2 */
L8c0b36fe: *stop=0x8c0b36feu;r[0]=99u;goto L8c0b3700; /* mov #0x63,r0 */
L8c0b3700: *stop=0x8c0b3700u;r[0]<<=2;goto L8c0b3702; /* shll2 r0 */
L8c0b3702: *stop=0x8c0b3702u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3704; /* fmac fr0,fr3,fr2 */
L8c0b3704: *stop=0x8c0b3704u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b3706; /* fmov.s @(r0,r4),fr3 */
L8c0b3706: *stop=0x8c0b3706u;r[0]=4u;goto L8c0b3708; /* mov #0x4,r0 */
L8c0b3708: *stop=0x8c0b3708u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b370a; /* fadd fr3,fr2 */
L8c0b370a: *stop=0x8c0b370au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0b370c; /* fmov fr2,@(r0,r13) */
L8c0b370c: *stop=0x8c0b370cu;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b370e; /* mov.b @(0x3,r14),r0 */
L8c0b370e: *stop=0x8c0b370eu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0b3710; /* mov.l @(0x8,r14),r5 */
L8c0b3710: *stop=0x8c0b3710u;r[0]=r[0]&255u;goto L8c0b3712; /* extu.b r0,r0 */
L8c0b3712: *stop=0x8c0b3712u;r[3]=r[0];goto L8c0b3714; /* mov r0,r3 */
L8c0b3714: *stop=0x8c0b3714u;r[0]<<=2;goto L8c0b3716; /* shll2 r0 */
L8c0b3716: *stop=0x8c0b3716u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3718; /* shll r0 */
L8c0b3718: *stop=0x8c0b3718u;r[0]+=r[3];goto L8c0b371a; /* add r3,r0 */
L8c0b371a: *stop=0x8c0b371au;r[0]<<=2;goto L8c0b371c; /* shll2 r0 */
L8c0b371c: *stop=0x8c0b371cu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b371e; /* shll r0 */
L8c0b371e: *stop=0x8c0b371eu;r[5]+=r[0];goto L8c0b3720; /* add r0,r5 */
L8c0b3720: *stop=0x8c0b3720u;r[0]=118u;goto L8c0b3722; /* mov #0x76,r0 */
L8c0b3722: *stop=0x8c0b3722u;r[0]<<=2;goto L8c0b3724; /* shll2 r0 */
L8c0b3724: *stop=0x8c0b3724u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b3726; /* fmov.s @(r0,r4),fr0 */
L8c0b3726: *stop=0x8c0b3726u;r[0]=16u;goto L8c0b3728; /* mov #0x10,r0 */
L8c0b3728: *stop=0x8c0b3728u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c0b372a; /* fmov.s @(r0,r5),fr3 */
L8c0b372a: *stop=0x8c0b372au;r[0]=12u;goto L8c0b372c; /* mov #0xc,r0 */
L8c0b372c: *stop=0x8c0b372cu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c0b372e; /* fmov.s @(r0,r5),fr2 */
L8c0b372e: *stop=0x8c0b372eu;r[0]=100u;goto L8c0b3730; /* mov #0x64,r0 */
L8c0b3730: *stop=0x8c0b3730u;r[0]<<=2;goto L8c0b3732; /* shll2 r0 */
L8c0b3732: *stop=0x8c0b3732u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0b3734; /* fmac fr0,fr3,fr2 */
L8c0b3734: *stop=0x8c0b3734u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c0b3736; /* fmov.s @(r0,r4),fr3 */
L8c0b3736: *stop=0x8c0b3736u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c0b3738; /* tst r12,r12 */
L8c0b3738: *stop=0x8c0b3738u;r[0]=8u;goto L8c0b373a; /* mov #0x8,r0 */
L8c0b373a: *stop=0x8c0b373au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b373c; /* fadd fr3,fr2 */
L8c0b373c: take=(*sr&1u);*stop=0x8c0b373eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;if(take)goto L8c0b37d6;goto L8c0b3740; /* bt/s 0x8c0b37d6 */
L8c0b3740: *stop=0x8c0b3740u;r[2]=98u;goto L8c0b3742; /* mov #0x62,r2 */
L8c0b3742: *stop=0x8c0b3742u;r[2]<<=2;goto L8c0b3744; /* shll2 r2 */
L8c0b3744: *stop=0x8c0b3744u;r[2]+=r[4];goto L8c0b3746; /* add r4,r2 */
L8c0b3746: *stop=0x8c0b3746u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3748; /* mov.l @r2,r3 */
L8c0b3748: *stop=0x8c0b3748u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b374a; /* mov.l @(0x4,r2),r1 */
L8c0b374a: *stop=0x8c0b374au;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[3],4);if(io_status)return io_status;goto L8c0b374c; /* mov.l r3,@r12 */
L8c0b374c: *stop=0x8c0b374cu;if(!valid_access(r[12]+4u,4u))return -3;wr(m,bus,&io_status,r[12]+0x4u,r[1],4);if(io_status)return io_status;goto L8c0b374e; /* mov.l r1,@(0x4,r12) */
L8c0b374e: *stop=0x8c0b374eu;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3750; /* mov.l @(0x8,r2),r3 */
L8c0b3750: *stop=0x8c0b3752u;if(!valid_access(r[12]+8u,4u))return -3;wr(m,bus,&io_status,r[12]+0x8u,r[3],4);if(io_status)return io_status;goto L8c0b37d6; /* bra 0x8c0b37d6 */
L8c0b3754: *stop=0x8c0b3754u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b3756; /* mov.b @(0x3,r14),r0 */
L8c0b3756: *stop=0x8c0b3756u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b3758; /* mov.l @(0x8,r14),r2 */
L8c0b3758: *stop=0x8c0b3758u;r[0]=r[0]&255u;goto L8c0b375a; /* extu.b r0,r0 */
L8c0b375a: *stop=0x8c0b375au;r[3]=r[0];goto L8c0b375c; /* mov r0,r3 */
L8c0b375c: *stop=0x8c0b375cu;r[0]<<=2;goto L8c0b375e; /* shll2 r0 */
L8c0b375e: *stop=0x8c0b375eu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3760; /* shll r0 */
L8c0b3760: *stop=0x8c0b3760u;r[0]+=r[3];goto L8c0b3762; /* add r3,r0 */
L8c0b3762: *stop=0x8c0b3762u;r[0]<<=2;goto L8c0b3764; /* shll2 r0 */
L8c0b3764: *stop=0x8c0b3764u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3766; /* shll r0 */
L8c0b3766: *stop=0x8c0b3766u;r[0]+=r[2];goto L8c0b3768; /* add r2,r0 */
L8c0b3768: *stop=0x8c0b3768u;if(!valid_access(r[0]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b376a; /* mov.l @(0x4,r0),r3 */
L8c0b376a: *stop=0x8c0b376au;if(!valid_access(r[0]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0b376c; /* mov.l @(0x8,r0),r1 */
L8c0b376c: *stop=0x8c0b376cu;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c0b376e; /* mov.l r3,@r13 */
L8c0b376e: *stop=0x8c0b376eu;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[1],4);if(io_status)return io_status;goto L8c0b3770; /* mov.l r1,@(0x4,r13) */
L8c0b3770: *stop=0x8c0b3770u;r[1]=16u;goto L8c0b3772; /* mov #0x10,r1 */
L8c0b3772: *stop=0x8c0b3772u;if(!valid_access(r[0]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b3774; /* mov.l @(0xc,r0),r3 */
L8c0b3774: *stop=0x8c0b3774u;if(!valid_access(r[13]+8u,4u))return -3;wr(m,bus,&io_status,r[13]+0x8u,r[3],4);if(io_status)return io_status;goto L8c0b3776; /* mov.l r3,@(0x8,r13) */
L8c0b3776: *stop=0x8c0b3776u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b3778; /* mov.b @(0x3,r14),r0 */
L8c0b3778: *stop=0x8c0b3778u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b377a; /* mov.l @(0x8,r14),r2 */
L8c0b377a: *stop=0x8c0b377au;r[0]=r[0]&255u;goto L8c0b377c; /* extu.b r0,r0 */
L8c0b377c: *stop=0x8c0b377cu;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c0b377e; /* fmov.s @r13,fr2 */
L8c0b377e: *stop=0x8c0b377eu;r[3]=r[0];goto L8c0b3780; /* mov r0,r3 */
L8c0b3780: *stop=0x8c0b3780u;r[0]<<=2;goto L8c0b3782; /* shll2 r0 */
L8c0b3782: *stop=0x8c0b3782u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b3784; /* shll r0 */
L8c0b3784: *stop=0x8c0b3784u;r[0]+=r[3];goto L8c0b3786; /* add r3,r0 */
L8c0b3786: *stop=0x8c0b3786u;r[0]<<=2;goto L8c0b3788; /* shll2 r0 */
L8c0b3788: *stop=0x8c0b3788u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b378a; /* shll r0 */
L8c0b378a: *stop=0x8c0b378au;r[0]+=r[2];goto L8c0b378c; /* add r2,r0 */
L8c0b378c: *stop=0x8c0b378cu;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c0b378e; /* fmov.s @(r0,r1),fr3 */
L8c0b378e: *stop=0x8c0b378eu;r[0]=4u;goto L8c0b3790; /* mov #0x4,r0 */
L8c0b3790: *stop=0x8c0b3790u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b3792; /* fadd fr3,fr2 */
L8c0b3792: *stop=0x8c0b3792u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c0b3794; /* fmov.s fr2,@r13 */
L8c0b3794: *stop=0x8c0b3794u;r[1]=70u;goto L8c0b3796; /* mov #0x46,r1 */
L8c0b3796: *stop=0x8c0b3796u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0b3798; /* fmov.s @(r0,r13),fr2 */
L8c0b3798: *stop=0x8c0b3798u;r[1]<<=2;goto L8c0b379a; /* shll2 r1 */
L8c0b379a: *stop=0x8c0b379au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c0b379c; /* fldi0 fr4 */
L8c0b379c: *stop=0x8c0b379cu;r[1]+=r[4];goto L8c0b379e; /* add r4,r1 */
L8c0b379e: *stop=0x8c0b379eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c0b37a0; /* fmov.s @r1,fr3 */
L8c0b37a0: *stop=0x8c0b37a0u;r[1]=20u;goto L8c0b37a2; /* mov #0x14,r1 */
L8c0b37a2: *stop=0x8c0b37a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0b37a4; /* fadd fr3,fr2 */
L8c0b37a4: *stop=0x8c0b37a4u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0b37a6; /* fmov fr2,@(r0,r13) */
L8c0b37a6: *stop=0x8c0b37a6u;if(!valid_access(r[14]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x3u,1);if(io_status)return io_status;goto L8c0b37a8; /* mov.b @(0x3,r14),r0 */
L8c0b37a8: *stop=0x8c0b37a8u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b37aa; /* mov.l @(0x8,r14),r2 */
L8c0b37aa: *stop=0x8c0b37aau;r[0]=r[0]&255u;goto L8c0b37ac; /* extu.b r0,r0 */
L8c0b37ac: *stop=0x8c0b37acu;r[3]=r[0];goto L8c0b37ae; /* mov r0,r3 */
L8c0b37ae: *stop=0x8c0b37aeu;r[0]<<=2;goto L8c0b37b0; /* shll2 r0 */
L8c0b37b0: *stop=0x8c0b37b0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b37b2; /* shll r0 */
L8c0b37b2: *stop=0x8c0b37b2u;r[0]+=r[3];goto L8c0b37b4; /* add r3,r0 */
L8c0b37b4: *stop=0x8c0b37b4u;r[0]<<=2;goto L8c0b37b6; /* shll2 r0 */
L8c0b37b6: *stop=0x8c0b37b6u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0b37b8; /* shll r0 */
L8c0b37b8: *stop=0x8c0b37b8u;r[0]+=r[2];goto L8c0b37ba; /* add r2,r0 */
L8c0b37ba: *stop=0x8c0b37bau;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c0b37bc; /* fmov.s @(r0,r1),fr3 */
L8c0b37bc: *stop=0x8c0b37bcu;r[0]=4u;goto L8c0b37be; /* mov #0x4,r0 */
L8c0b37be: *stop=0x8c0b37beu;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c0b37c0; /* tst r12,r12 */
L8c0b37c0: *stop=0x8c0b37c0u;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c0b37c2; /* fmov.s fr3,@r11 */
L8c0b37c2: *stop=0x8c0b37c2u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[11]);if(io_status)return io_status;goto L8c0b37c4; /* fmov fr4,@(r0,r11) */
L8c0b37c4: *stop=0x8c0b37c4u;r[0]=8u;goto L8c0b37c6; /* mov #0x8,r0 */
L8c0b37c6: take=(*sr&1u);*stop=0x8c0b37c8u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[11]);if(io_status)return io_status;if(take)goto L8c0b37d6;goto L8c0b37ca; /* bt/s 0x8c0b37d6 */
L8c0b37ca: *stop=0x8c0b37cau;r[0]=4u;goto L8c0b37cc; /* mov #0x4,r0 */
L8c0b37cc: *stop=0x8c0b37ccu;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[12]);if(io_status)return io_status;goto L8c0b37ce; /* fmov.s fr4,@r12 */
L8c0b37ce: *stop=0x8c0b37ceu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0b37d0; /* fmov.s @(r0,r13),fr3 */
L8c0b37d0: *stop=0x8c0b37d0u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c0b37d2; /* fmov fr3,@(r0,r12) */
L8c0b37d2: *stop=0x8c0b37d2u;r[0]=8u;goto L8c0b37d4; /* mov #0x8,r0 */
L8c0b37d4: *stop=0x8c0b37d4u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[12]);if(io_status)return io_status;goto L8c0b37d6; /* fmov fr4,@(r0,r12) */
L8c0b37d6: *stop=0x8c0b37d6u;r[0]=52u;goto L8c0b37d8; /* mov #0x34,r0 */
L8c0b37d8: *stop=0x8c0b37d8u;r[6]=r[11];goto L8c0b37da; /* mov r11,r6 */
L8c0b37da: *stop=0x8c0b37dau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0b37dc; /* fmov.s @(r0,r14),fr3 */
L8c0b37dc: *stop=0x8c0b37dcu;r[7]=r[12];goto L8c0b37de; /* mov r12,r7 */
L8c0b37de: *stop=0x8c0b37deu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0b37e0; /* lds.l @r15+,PR */
L8c0b37e0: *stop=0x8c0b37e0u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c0b37e2; /* ftrc fr3,FPUL */
L8c0b37e2: *stop=0x8c0b37e2u;r[5]=r[13];goto L8c0b37e4; /* mov r13,r5 */
L8c0b37e4: *stop=0x8c0b37e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c0b37e6; /* mov.l @r15+,r11 */
L8c0b37e6: *stop=0x8c0b37e6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c0b37e8; /* mov.l @r15+,r12 */
L8c0b37e8: *stop=0x8c0b37e8u;r[4]=*fpul;goto L8c0b37ea; /* sts FPUL,r4 */
L8c0b37ea: *stop=0x8c0b37eau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0b37ec; /* mov.l @r15+,r13 */
L8c0b37ec: *stop=0x8c0b37ecu;r[4]=(uint32_t)(int32_t)(int8_t)r[4];goto L8c0b37ee; /* exts.b r4,r4 */
L8c0b37ee: *stop=0x8c0b37f0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;goto L8c0b33ee; /* bra 0x8c0b33ee */
}
SA_STAGE_EXPORT int sa_player_child_update_8c0b3480(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3480(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3410(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3480(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3410u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b341a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3480(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b341au);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b342a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3480(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b342au);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b343a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3480(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b343au);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3450(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3480(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3450u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b345a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3480(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b345au);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3464(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3480(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3464u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b3472(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3480(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b3472u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b349e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b3480(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b349eu);}

static int sa_player_child_update_run_8c09e778(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c09e79cu)goto L8c09e79c;if(resume_pc==0x8c09e7d2u)goto L8c09e7d2;if(resume_pc==0x8c09e7deu)goto L8c09e7de;if(resume_pc==0x8c09e7f2u)goto L8c09e7f2;if(resume_pc==0x8c09e83au)goto L8c09e83a;if(resume_pc==0x8c09e84au)goto L8c09e84a;if(resume_pc==0x8c09e850u)goto L8c09e850;if(resume_pc==0x8c09e856u)goto L8c09e856;goto L8c09e778;
L8c09e778: *stop=0x8c09e778u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e784u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c09e77a; /* mov.l 0x8c09e784,r0 */
L8c09e77a: *stop=0x8c09e77au;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c09e77c; /* mov.l @r0,r0 */
L8c09e77c: *stop=0x8c09e77cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c09e77e; /* tst r0,r0 */
L8c09e77e: if((*sr&1u))goto L8c09e788;goto L8c09e780; /* bt 0x8c09e788 */
L8c09e780: *stop=0x8c09e780u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c09e782u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c09e788: *stop=0x8c09e788u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c09e78a; /* mov.l r10,@-r15 */
L8c09e78a: *stop=0x8c09e78au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c09e78c; /* mov.l r9,@-r15 */
L8c09e78c: *stop=0x8c09e78cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c09e78e; /* mov.l r8,@-r15 */
L8c09e78e: *stop=0x8c09e78eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c09e790; /* sts.l PR,@-r15 */
L8c09e790: *stop=0x8c09e790u;r[15]+=4294967292u;goto L8c09e792; /* add #-0x4,r15 */
L8c09e792: *stop=0x8c09e792u;r[8]=r[4];goto L8c09e794; /* mov r4,r8 */
L8c09e794: *stop=0x8c09e794u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e800u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09e796; /* mov.l 0x8c09e800,r3 */
L8c09e796: *stop=0x8c09e796u;r[9]=r[5];goto L8c09e798; /* mov r5,r9 */
L8c09e798: target=r[3];*pr=0x8c09e79cu;*stop=0x8c09e79au;r[10]=r[6];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09e79c; /* jsr @r3 */
L8c09e79c: *stop=0x8c09e79cu;r[0]=r[10];goto L8c09e79e; /* mov r10,r0 */
L8c09e79e: *stop=0x8c09e79eu;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c09e7a0; /* tst #0x20,r0 */
L8c09e7a0: *stop=0x8c09e7a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e804u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09e7a2; /* mov.l 0x8c09e804,r1 */
L8c09e7a2: if(!(*sr&1u))goto L8c09e7a6;goto L8c09e7a4; /* bf 0x8c09e7a6 */
L8c09e7a4: *stop=0x8c09e7a4u;r[1]+=4294967292u;goto L8c09e7a6; /* add #-0x4,r1 */
L8c09e7a6: *stop=0x8c09e7a6u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c09e7a8; /* mov.l @r1,r2 */
L8c09e7a8: *stop=0x8c09e7a8u;r[0]=r[2];goto L8c09e7aa; /* mov r2,r0 */
L8c09e7aa: *stop=0x8c09e7aau;r[0]>>=16;goto L8c09e7ac; /* shlr16 r0 */
L8c09e7ac: *stop=0x8c09e7acu;r[0]>>=8;goto L8c09e7ae; /* shlr8 r0 */
L8c09e7ae: *stop=0x8c09e7aeu;r[0]&=0x1cu;goto L8c09e7b0; /* and #0x1c,r0 */
L8c09e7b0: *stop=0x8c09e7b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e808u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09e7b2; /* mov.l 0x8c09e808,r1 */
L8c09e7b2: *stop=0x8c09e7b2u;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c09e7b4; /* mov.l r0,@-r1 */
L8c09e7b4: *stop=0x8c09e7b4u;if(!valid_access(r[1]-4u,4u))return -3;r[1]-=4;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c09e7b6; /* mov.l r0,@-r1 */
L8c09e7b6: *stop=0x8c09e7b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e80cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09e7b8; /* mov.l 0x8c09e80c,r1 */
L8c09e7b8: *stop=0x8c09e7b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e810u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c09e7ba; /* mov.l 0x8c09e810,r0 */
L8c09e7ba: *stop=0x8c09e7bau;r[2]&=r[1];goto L8c09e7bc; /* and r1,r2 */
L8c09e7bc: *stop=0x8c09e7bcu;r[2]|=r[0];goto L8c09e7be; /* or r0,r2 */
L8c09e7be: *stop=0x8c09e7beu;r[2]+=32u;goto L8c09e7c0; /* add #0x20,r2 */
L8c09e7c0: *stop=0x8c09e7c0u;r[1]=16u;goto L8c09e7c2; /* mov #0x10,r1 */
L8c09e7c2: *stop=0x8c09e7c2u;*sr=(*sr&~1u)|!(r[10]&r[1]);goto L8c09e7c4; /* tst r10,r1 */
L8c09e7c4: *stop=0x8c09e7c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e814u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09e7c6; /* mov.l 0x8c09e814,r3 */
L8c09e7c6: take=!(*sr&1u);*stop=0x8c09e7c8u;r[4]=r[8];if(take)goto L8c09e7cc;goto L8c09e7ca; /* bf/s 0x8c09e7cc */
L8c09e7ca: *stop=0x8c09e7cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e818u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09e7cc; /* mov.l 0x8c09e818,r3 */
L8c09e7cc: *stop=0x8c09e7ccu;r[5]=r[9];goto L8c09e7ce; /* mov r9,r5 */
L8c09e7ce: target=r[3];*pr=0x8c09e7d2u;*stop=0x8c09e7d0u;r[6]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09e7d2; /* jsr @r3 */
L8c09e7d2: *stop=0x8c09e7d2u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c09e7d4; /* tst r0,r0 */
L8c09e7d4: if((*sr&1u))goto L8c09e850;goto L8c09e7d6; /* bt 0x8c09e850 */
L8c09e7d6: *stop=0x8c09e7d6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c09e7d8; /* mov.l r2,@r15 */
L8c09e7d8: *stop=0x8c09e7d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e81cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09e7da; /* mov.l 0x8c09e81c,r3 */
L8c09e7da: target=r[3];*pr=0x8c09e7deu;*stop=0x8c09e7dcu;if(!valid_access(r[8]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8]+0x18u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09e7de; /* jsr @r3 */
L8c09e7de: *stop=0x8c09e7deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e820u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09e7e0; /* mov.l 0x8c09e820,r3 */
L8c09e7e0: *stop=0x8c09e7e0u;if(!valid_access(r[8]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09e7e2; /* mov.l @(0x1c,r8),r1 */
L8c09e7e2: *stop=0x8c09e7e2u;r[4]=r[9];goto L8c09e7e4; /* mov r9,r4 */
L8c09e7e4: *stop=0x8c09e7e4u;r[4]<<=2;goto L8c09e7e6; /* shll2 r4 */
L8c09e7e6: *stop=0x8c09e7e6u;r[4]+=r[9];goto L8c09e7e8; /* add r9,r4 */
L8c09e7e8: *stop=0x8c09e7e8u;r[4]<<=2;goto L8c09e7ea; /* shll2 r4 */
L8c09e7ea: *stop=0x8c09e7eau;r[1]+=r[4];goto L8c09e7ec; /* add r4,r1 */
L8c09e7ec: *stop=0x8c09e7ecu;if(!valid_access(r[1]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[1]+0x10u,2);if(io_status)return io_status;goto L8c09e7ee; /* mov.w @(0x10,r1),r0 */
L8c09e7ee: target=r[3];*pr=0x8c09e7f2u;*stop=0x8c09e7f0u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09e7f2; /* jsr @r3 */
L8c09e7f2: *stop=0x8c09e7f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e824u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09e7f4; /* mov.l 0x8c09e824,r1 */
L8c09e7f4: *stop=0x8c09e7f4u;*sr=(*sr&~1u)|!(r[10]&r[1]);goto L8c09e7f6; /* tst r10,r1 */
L8c09e7f6: if((*sr&1u))goto L8c09e830;goto L8c09e7f8; /* bt 0x8c09e830 */
L8c09e7f8: *stop=0x8c09e7f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e828u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c09e7fa; /* mov.l 0x8c09e828,r4 */
L8c09e7fa: *stop=0x8c09e7fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e82cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c09e7fc; /* mov.l 0x8c09e82c,r5 */
L8c09e7fc: *stop=0x8c09e7feu;goto L8c09e834; /* bra 0x8c09e834 */
L8c09e830: *stop=0x8c09e830u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e860u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c09e832; /* mov.l 0x8c09e860,r4 */
L8c09e832: *stop=0x8c09e832u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e864u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c09e834; /* mov.l 0x8c09e864,r5 */
L8c09e834: *stop=0x8c09e834u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e868u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09e836; /* mov.l 0x8c09e868,r3 */
L8c09e836: target=r[3];*pr=0x8c09e83au;*stop=0x8c09e838u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09e83a; /* jsr @r3 */
L8c09e83a: *stop=0x8c09e83au;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e86cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09e83c; /* mov.l 0x8c09e86c,r1 */
L8c09e83c: *stop=0x8c09e83cu;*sr=(*sr&~1u)|!(r[10]&r[1]);goto L8c09e83e; /* tst r10,r1 */
L8c09e83e: *stop=0x8c09e83eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e870u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c09e840; /* mov.l 0x8c09e870,r4 */
L8c09e840: take=!(*sr&1u);*stop=0x8c09e842u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;if(take)goto L8c09e846;goto L8c09e844; /* bf/s 0x8c09e846 */
L8c09e844: *stop=0x8c09e844u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e874u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c09e846; /* mov.l 0x8c09e874,r4 */
L8c09e846: target=r[3];*pr=0x8c09e84au;*stop=0x8c09e848u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09e84a; /* jsr @r3 */
L8c09e84a: *stop=0x8c09e84au;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e878u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09e84c; /* mov.l 0x8c09e878,r3 */
L8c09e84c: target=r[3];*pr=0x8c09e850u;*stop=0x8c09e84eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09e850; /* jsr @r3 */
L8c09e850: *stop=0x8c09e850u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e87cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09e852; /* mov.l 0x8c09e87c,r3 */
L8c09e852: target=r[3];*pr=0x8c09e856u;*stop=0x8c09e854u;r[15]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09e856; /* jsr @r3 */
L8c09e856: *stop=0x8c09e856u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c09e858; /* lds.l @r15+,PR */
L8c09e858: *stop=0x8c09e858u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c09e85a; /* mov.l @r15+,r8 */
L8c09e85a: *stop=0x8c09e85au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c09e85c; /* mov.l @r15+,r9 */
L8c09e85c: *stop=0x8c09e85cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c09e85eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_child_update_8c09e778(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c09e778(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c09e79c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c09e778(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09e79cu);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c09e7d2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c09e778(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09e7d2u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c09e7de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c09e778(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09e7deu);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c09e7f2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c09e778(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09e7f2u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c09e83a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c09e778(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09e83au);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c09e84a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c09e778(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09e84au);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c09e850(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c09e778(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09e850u);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c09e856(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c09e778(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09e856u);}

SA_STAGE_EXPORT int sa_player_child_update_8c098786(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c098786;
L8c098786: *stop=0x8c098786u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0987e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c098788; /* mov.l 0x8c0987e8,r3 */
L8c098788: *stop=0x8c098788u;if(*pr!=entry_pr)return -3;*stop=0x8c09878au;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[3],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_child_update_8c07cfde(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c07cfde;
L8c07cfde: *stop=0x8c07cfdeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c07d044u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07cfe0; /* mov.l 0x8c07d044,r1 */
L8c07cfe0: *stop=0x8c07cfe0u;r[0]=r[4]&255u;goto L8c07cfe2; /* extu.b r4,r0 */
L8c07cfe2: *stop=0x8c07cfe2u;r[0]<<=2;goto L8c07cfe4; /* shll2 r0 */
L8c07cfe4: *stop=0x8c07cfe4u;if(*pr!=entry_pr)return -3;*stop=0x8c07cfe6u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}return 0; /* rts */
}

static int sa_player_child_update_run_8c0b33a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c0b33acu)goto L8c0b33ac;goto L8c0b33a0;
L8c0b33a0: *stop=0x8c0b33a0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0b33a2; /* mov.l r14,@-r15 */
L8c0b33a2: *stop=0x8c0b33a2u;r[14]=r[4];goto L8c0b33a4; /* mov r4,r14 */
L8c0b33a4: *stop=0x8c0b33a4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0b33a6; /* sts.l PR,@-r15 */
L8c0b33a6: *stop=0x8c0b33a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0b34e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b33a8; /* mov.l 0x8c0b34e0,r3 */
L8c0b33a8: target=r[3];*pr=0x8c0b33acu;*stop=0x8c0b33aau;r[4]=52u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0b33ac; /* jsr @r3 */
L8c0b33ac: *stop=0x8c0b33acu;r[4]=r[0];goto L8c0b33ae; /* mov r0,r4 */
L8c0b33ae: *stop=0x8c0b33aeu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c0b33b0; /* tst r4,r4 */
L8c0b33b0: if((*sr&1u))goto L8c0b33c6;goto L8c0b33b2; /* bt 0x8c0b33c6 */
L8c0b33b2: *stop=0x8c0b33b2u;r[3]=0u;goto L8c0b33b4; /* mov #0x0,r3 */
L8c0b33b4: *stop=0x8c0b33b4u;if(!valid_access(r[4]+44u,4u))return -3;wr(m,bus,&io_status,r[4]+0x2cu,r[3],4);if(io_status)return io_status;goto L8c0b33b6; /* mov.l r3,@(0x2c,r4) */
L8c0b33b6: *stop=0x8c0b33b6u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0b33b8; /* mov.l @(0x10,r14),r2 */
L8c0b33b8: *stop=0x8c0b33b8u;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[2],4);if(io_status)return io_status;goto L8c0b33ba; /* mov.l r2,@(0x30,r4) */
L8c0b33ba: *stop=0x8c0b33bau;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b33bc; /* mov.l @(0x10,r14),r3 */
L8c0b33bc: *stop=0x8c0b33bcu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0b33be; /* tst r3,r3 */
L8c0b33be: if((*sr&1u))goto L8c0b33c4;goto L8c0b33c0; /* bt 0x8c0b33c4 */
L8c0b33c0: *stop=0x8c0b33c0u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0b33c2; /* mov.l @(0x10,r14),r3 */
L8c0b33c2: *stop=0x8c0b33c2u;if(!valid_access(r[3]+44u,4u))return -3;wr(m,bus,&io_status,r[3]+0x2cu,r[4],4);if(io_status)return io_status;goto L8c0b33c4; /* mov.l r4,@(0x2c,r3) */
L8c0b33c4: *stop=0x8c0b33c4u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[4],4);if(io_status)return io_status;goto L8c0b33c6; /* mov.l r4,@(0x10,r14) */
L8c0b33c6: *stop=0x8c0b33c6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0b33c8; /* lds.l @r15+,PR */
L8c0b33c8: *stop=0x8c0b33c8u;r[0]=r[4];goto L8c0b33ca; /* mov r4,r0 */
L8c0b33ca: *stop=0x8c0b33cau;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c0b33ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_child_update_8c0b33a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b33a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c0b33ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c0b33a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0b33acu);}

SA_STAGE_EXPORT int sa_player_child_update_8c09e880(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c09e880;
L8c09e880: *stop=0x8c09e880u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c09e882; /* fmov.s fr15,@-r15 */
L8c09e882: *stop=0x8c09e882u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c09e884; /* fmov.s fr14,@-r15 */
L8c09e884: *stop=0x8c09e884u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c09e886; /* fmov.s fr13,@-r15 */
L8c09e886: *stop=0x8c09e886u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c09e888; /* fmov.s fr12,@-r15 */
L8c09e888: *stop=0x8c09e888u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c09e88a; /* mov.l r9,@-r15 */
L8c09e88a: *stop=0x8c09e88au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c09e88c; /* mov.l r8,@-r15 */
L8c09e88c: *stop=0x8c09e88cu;r[9]=r[2];goto L8c09e88e; /* mov r2,r9 */
L8c09e88e: *stop=0x8c09e88eu;r[8]=r[15];goto L8c09e890; /* mov r15,r8 */
L8c09e890: *stop=0x8c09e890u;r[15]+=4294967264u;goto L8c09e892; /* add #-0x20,r15 */
L8c09e892: *stop=0x8c09e892u;r[1]=r[5];goto L8c09e894; /* mov r5,r1 */
L8c09e894: *stop=0x8c09e894u;r[5]<<=2;goto L8c09e896; /* shll2 r5 */
L8c09e896: *stop=0x8c09e896u;r[5]+=r[1];goto L8c09e898; /* add r1,r5 */
L8c09e898: *stop=0x8c09e898u;r[5]<<=2;goto L8c09e89a; /* shll2 r5 */
L8c09e89a: *stop=0x8c09e89au;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09e89c; /* mov.l @(0x1c,r4),r1 */
L8c09e89c: *stop=0x8c09e89cu;r[0]=0x8c09e948u;goto L8c09e89e; /* mova 0x8c09e948,r0 */
L8c09e89e: *stop=0x8c09e89eu;r[5]+=r[1];goto L8c09e8a0; /* add r1,r5 */
L8c09e8a0: *stop=0x8c09e8a0u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c09e8a2; /* fmov.s @r0,fr4 */
L8c09e8a2: *stop=0x8c09e8a2u;r[5]+=8u;goto L8c09e8a4; /* add #0x8,r5 */
L8c09e8a4: *stop=0x8c09e8a4u;if(!valid_access(r[5],2u))return -3;r[2]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09e8a6; /* mov.w @r5+,r2 */
L8c09e8a6: *stop=0x8c09e8a6u;*fpul=r[2];goto L8c09e8a8; /* lds r2,FPUL */
L8c09e8a8: *stop=0x8c09e8a8u;if(!valid_access(r[5],2u))return -3;r[1]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09e8aa; /* mov.w @r5+,r1 */
L8c09e8aa: *stop=0x8c09e8aau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][0]);if(status)return status;goto L8c09e8ac; /* float FPUL,fr0 */
L8c09e8ac: *stop=0x8c09e8acu;*fpul=r[1];goto L8c09e8ae; /* lds r1,FPUL */
L8c09e8ae: *stop=0x8c09e8aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c09e8b0; /* fmul fr4,fr0 */
L8c09e8b0: *stop=0x8c09e8b0u;if(!valid_access(r[5],2u))return -3;r[2]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09e8b2; /* mov.w @r5+,r2 */
L8c09e8b2: *stop=0x8c09e8b2u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c09e8b4; /* float FPUL,fr1 */
L8c09e8b4: *stop=0x8c09e8b4u;*fpul=r[2];goto L8c09e8b6; /* lds r2,FPUL */
L8c09e8b6: *stop=0x8c09e8b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c09e8b8; /* fmul fr4,fr1 */
L8c09e8b8: *stop=0x8c09e8b8u;if(!valid_access(r[5],2u))return -3;r[1]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09e8ba; /* mov.w @r5+,r1 */
L8c09e8ba: *stop=0x8c09e8bau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c09e8bc; /* float FPUL,fr2 */
L8c09e8bc: *stop=0x8c09e8bcu;*fpul=r[1];goto L8c09e8be; /* lds r1,FPUL */
L8c09e8be: *stop=0x8c09e8beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c09e8c0; /* fmul fr4,fr2 */
L8c09e8c0: *stop=0x8c09e8c0u;r[5]+=4294967280u;goto L8c09e8c2; /* add #-0x10,r5 */
L8c09e8c2: *stop=0x8c09e8c2u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c09e8c4; /* float FPUL,fr3 */
L8c09e8c4: *stop=0x8c09e8c4u;r[1]=4u;goto L8c09e8c6; /* mov #0x4,r1 */
L8c09e8c6: *stop=0x8c09e8c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c09e8c8; /* fmul fr4,fr3 */
L8c09e8c8: *stop=0x8c09e8c8u;*sr=(*sr&~1u)|!(r[6]&r[1]);goto L8c09e8ca; /* tst r6,r1 */
L8c09e8ca: if((*sr&1u))goto L8c09e8d2;goto L8c09e8cc; /* bt 0x8c09e8d2 */
L8c09e8cc: *stop=0x8c09e8ccu;fcopy(f,4u,0u);goto L8c09e8ce; /* fmov fr0,fr4 */
L8c09e8ce: *stop=0x8c09e8ceu;fcopy(f,0u,2u);goto L8c09e8d0; /* fmov fr2,fr0 */
L8c09e8d0: *stop=0x8c09e8d0u;fcopy(f,2u,4u);goto L8c09e8d2; /* fmov fr4,fr2 */
L8c09e8d2: *stop=0x8c09e8d2u;r[1]=8u;goto L8c09e8d4; /* mov #0x8,r1 */
L8c09e8d4: *stop=0x8c09e8d4u;*sr=(*sr&~1u)|!(r[6]&r[1]);goto L8c09e8d6; /* tst r6,r1 */
L8c09e8d6: if((*sr&1u))goto L8c09e8de;goto L8c09e8d8; /* bt 0x8c09e8de */
L8c09e8d8: *stop=0x8c09e8d8u;fcopy(f,4u,1u);goto L8c09e8da; /* fmov fr1,fr4 */
L8c09e8da: *stop=0x8c09e8dau;fcopy(f,1u,3u);goto L8c09e8dc; /* fmov fr3,fr1 */
L8c09e8dc: *stop=0x8c09e8dcu;fcopy(f,3u,4u);goto L8c09e8de; /* fmov fr4,fr3 */
L8c09e8de: *stop=0x8c09e8deu;if(!valid_access(r[5],2u))return -3;r[2]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09e8e0; /* mov.w @r5+,r2 */
L8c09e8e0: *stop=0x8c09e8e0u;if(!valid_access(r[5],2u))return -3;r[3]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09e8e2; /* mov.w @r5+,r3 */
L8c09e8e2: *stop=0x8c09e8e2u;if(!valid_access(r[5],2u))return -3;r[0]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09e8e4; /* mov.w @r5+,r0 */
L8c09e8e4: *stop=0x8c09e8e4u;if(!valid_access(r[5],2u))return -3;r[1]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09e8e6; /* mov.w @r5+,r1 */
L8c09e8e6: *stop=0x8c09e8e6u;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c09e8e8; /* fmov.s fr0,@-r8 */
L8c09e8e8: *stop=0x8c09e8e8u;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c09e8ea; /* fmov.s fr1,@-r8 */
L8c09e8ea: *stop=0x8c09e8eau;r[0]=0u-r[0];goto L8c09e8ec; /* neg r0,r0 */
L8c09e8ec: *stop=0x8c09e8ecu;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c09e8ee; /* fmov.s fr0,@-r8 */
L8c09e8ee: *stop=0x8c09e8eeu;r[1]=0u-r[1];goto L8c09e8f0; /* neg r1,r1 */
L8c09e8f0: *stop=0x8c09e8f0u;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c09e8f2; /* fmov.s fr3,@-r8 */
L8c09e8f2: *stop=0x8c09e8f2u;r[2]+=r[0];goto L8c09e8f4; /* add r0,r2 */
L8c09e8f4: *stop=0x8c09e8f4u;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c09e8f6; /* fmov.s fr2,@-r8 */
L8c09e8f6: *stop=0x8c09e8f6u;r[3]+=r[1];goto L8c09e8f8; /* add r1,r3 */
L8c09e8f8: *stop=0x8c09e8f8u;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c09e8fa; /* fmov.s fr1,@-r8 */
L8c09e8fa: *stop=0x8c09e8fau;*fpul=r[0];goto L8c09e8fc; /* lds r0,FPUL */
L8c09e8fc: *stop=0x8c09e8fcu;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c09e8fe; /* fmov.s fr2,@-r8 */
L8c09e8fe: *stop=0x8c09e8feu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][0]);if(status)return status;goto L8c09e900; /* float FPUL,fr0 */
L8c09e900: *stop=0x8c09e900u;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c09e902; /* fmov.s fr3,@-r8 */
L8c09e902: *stop=0x8c09e902u;*fpul=r[1];goto L8c09e904; /* lds r1,FPUL */
L8c09e904: *stop=0x8c09e904u;r[0]=12u;goto L8c09e906; /* mov #0xc,r0 */
L8c09e906: *stop=0x8c09e906u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c09e908; /* float FPUL,fr1 */
L8c09e908: *stop=0x8c09e908u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c09e90a; /* fmov.s @(r0,r4),fr4 */
L8c09e90a: *stop=0x8c09e90au;*fpul=r[2];goto L8c09e90c; /* lds r2,FPUL */
L8c09e90c: *stop=0x8c09e90cu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c09e90e; /* fmov.s @(r0,r4),fr5 */
L8c09e90e: *stop=0x8c09e90eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c09e910; /* float FPUL,fr2 */
L8c09e910: *stop=0x8c09e910u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c09e912; /* fmul fr4,fr0 */
L8c09e912: *stop=0x8c09e912u;*fpul=r[3];goto L8c09e914; /* lds r3,FPUL */
L8c09e914: *stop=0x8c09e914u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c09e916; /* fmul fr4,fr2 */
L8c09e916: *stop=0x8c09e916u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c09e918; /* float FPUL,fr3 */
L8c09e918: *stop=0x8c09e918u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[5],&f->fpscr,&fr[1]);if(status)return status;}goto L8c09e91a; /* fmul fr5,fr1 */
L8c09e91a: *stop=0x8c09e91au;r[0]=16u;goto L8c09e91c; /* mov #0x10,r0 */
L8c09e91c: *stop=0x8c09e91cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c09e91e; /* fmul fr5,fr3 */
L8c09e91e: *stop=0x8c09e91eu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09e920; /* fmov.s @r4+,fr4 */
L8c09e920: *stop=0x8c09e920u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09e922; /* fmov.s @r4+,fr5 */
L8c09e922: *stop=0x8c09e922u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09e924; /* fmov.s @r4+,fr6 */
L8c09e924: *stop=0x8c09e924u;r[4]+=4294967284u;goto L8c09e926; /* add #-0xc,r4 */
L8c09e926: *stop=0x8c09e926u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c09e928; /* fldi1 fr7 */
L8c09e928: *stop=0x8c09e928u;status=sa_sh4_vector_numeric(0xf5fdu,f,*fpul);if(status)return status;goto L8c09e92a; /* ftrv xmtrx,fv4 */
L8c09e92a: *stop=0x8c09e92au;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e94cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c09e92c; /* mov.l 0x8c09e94c,r0 */
L8c09e92c: *stop=0x8c09e92cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e950u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09e92e; /* mov.l 0x8c09e950,r1 */
L8c09e92e: *stop=0x8c09e92eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[0]);if(io_status)return io_status;goto L8c09e930; /* fmov.s @r0,fr8 */
L8c09e930: *stop=0x8c09e930u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[1]);if(io_status)return io_status;goto L8c09e932; /* fmov.s @r1,fr9 */
L8c09e932: *stop=0x8c09e932u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c09e934; /* fldi1 fr7 */
L8c09e934: *stop=0x8c09e934u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[7],&fr[6],&f->fpscr,&fr[7]);if(status)return status;}goto L8c09e936; /* fdiv fr6,fr7 */
L8c09e936: *stop=0x8c09e936u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e954u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c09e938; /* mov.l 0x8c09e954,r0 */
L8c09e938: *stop=0x8c09e938u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;goto L8c09e93a; /* fmov.s @r0,fr10 */
L8c09e93a: *stop=0x8c09e93au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[10],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c09e93c; /* fadd fr10,fr6 */
L8c09e93c: *stop=0x8c09e93cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][6],f->bank[(f->fpscr>>21)&1u][8],&f->fpscr,sr,1);if(status)return status;goto L8c09e93e; /* fcmp/gt fr8,fr6 */
L8c09e93e: if((*sr&1u))goto L8c09e958;goto L8c09e940; /* bt 0x8c09e958 */
L8c09e940: *stop=0x8c09e940u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][9],f->bank[(f->fpscr>>21)&1u][6],&f->fpscr,sr,1);if(status)return status;goto L8c09e942; /* fcmp/gt fr6,fr9 */
L8c09e942: if((*sr&1u))goto L8c09e958;goto L8c09e944; /* bt 0x8c09e958 */
L8c09e944: *stop=0x8c09e946u;goto L8c09e95c; /* bra 0x8c09e95c */
L8c09e958: *stop=0x8c09e95au;r[0]=0u;goto L8c09eaec; /* bra 0x8c09eaec */
L8c09e95c: *stop=0x8c09e95cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]=0x3f800000u;goto L8c09e95e; /* fldi1 fr10 */
L8c09e95e: *stop=0x8c09e95eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[10],&fr[6],&f->fpscr,&fr[10]);if(status)return status;}goto L8c09e960; /* fdiv fr6,fr10 */
L8c09e960: *stop=0x8c09e960u;fcopy(f,6u,10u);goto L8c09e962; /* fmov fr10,fr6 */
L8c09e962: *stop=0x8c09e962u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e9d4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c09e964; /* mov.l 0x8c09e9d4,r2 */
L8c09e964: *stop=0x8c09e964u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[2]);if(io_status)return io_status;goto L8c09e966; /* fmov.s @r2,fr10 */
L8c09e966: *stop=0x8c09e966u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e9d8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c09e968; /* mov.l 0x8c09e9d8,r0 */
L8c09e968: *stop=0x8c09e968u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[7],&f->fpscr,&fr[10]);if(status)return status;}goto L8c09e96a; /* fmul fr7,fr10 */
L8c09e96a: *stop=0x8c09e96au;{uint32_t loaded=rd(m,bus,&io_status,0x8c09e9dcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09e96c; /* mov.l 0x8c09e9dc,r1 */
L8c09e96c: *stop=0x8c09e96cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[10],&f->fpscr,&fr[0]);if(status)return status;}goto L8c09e96e; /* fmul fr10,fr0 */
L8c09e96e: *stop=0x8c09e96eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[0]);if(io_status)return io_status;goto L8c09e970; /* fmov.s @r0,fr8 */
L8c09e970: *stop=0x8c09e970u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[10],&f->fpscr,&fr[1]);if(status)return status;}goto L8c09e972; /* fmul fr10,fr1 */
L8c09e972: *stop=0x8c09e972u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[1]);if(io_status)return io_status;goto L8c09e974; /* fmov.s @r1,fr9 */
L8c09e974: *stop=0x8c09e974u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[10],&f->fpscr,&fr[2]);if(status)return status;}goto L8c09e976; /* fmul fr10,fr2 */
L8c09e976: *stop=0x8c09e976u;r[2]+=12u;goto L8c09e978; /* add #0xc,r2 */
L8c09e978: *stop=0x8c09e978u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[10],&f->fpscr,&fr[3]);if(status)return status;}goto L8c09e97a; /* fmul fr10,fr3 */
L8c09e97a: *stop=0x8c09e97au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[10],&f->fpscr,&fr[8]);if(status)return status;}goto L8c09e97c; /* fmul fr10,fr8 */
L8c09e97c: *stop=0x8c09e97cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[10],&f->fpscr,&fr[9]);if(status)return status;}goto L8c09e97e; /* fmul fr10,fr9 */
L8c09e97e: *stop=0x8c09e97eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[8],&f->fpscr,&fr[4]);if(status)return status;}goto L8c09e980; /* fmul fr8,fr4 */
L8c09e980: *stop=0x8c09e980u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09e982; /* fmov.s @r2+,fr10 */
L8c09e982: *stop=0x8c09e982u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[9],&f->fpscr,&fr[5]);if(status)return status;}goto L8c09e984; /* fmul fr9,fr5 */
L8c09e984: *stop=0x8c09e984u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09e986; /* fmov.s @r2+,fr11 */
L8c09e986: *stop=0x8c09e986u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[10],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c09e988; /* fadd fr10,fr4 */
L8c09e988: *stop=0x8c09e988u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c09e98a; /* fmov.s fr6,@-r15 */
L8c09e98a: *stop=0x8c09e98au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[11],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c09e98c; /* fadd fr11,fr5 */
L8c09e98c: *stop=0x8c09e98cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c09e98e; /* fmov.s fr5,@-r15 */
L8c09e98e: *stop=0x8c09e98eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c09e990; /* fmov.s fr4,@-r15 */
L8c09e990: *stop=0x8c09e990u;r[0]=1u;goto L8c09e992; /* mov #0x1,r0 */
L8c09e992: *stop=0x8c09e992u;*sr=(*sr&~1u)|!(r[0]&r[6]);goto L8c09e994; /* tst r0,r6 */
L8c09e994: if((*sr&1u))goto L8c09e9e0;goto L8c09e996; /* bt 0x8c09e9e0 */
L8c09e996: *stop=0x8c09e996u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09e998; /* mov.l @(0x14,r4),r1 */
L8c09e998: *stop=0x8c09e998u;r[1]=r[1]&65535u;goto L8c09e99a; /* extu.w r1,r1 */
L8c09e99a: *stop=0x8c09e99au;*fpul=r[1];goto L8c09e99c; /* lds r1,FPUL */
L8c09e99c: *stop=0x8c09e99cu;status=sa_sh4_vector_numeric(0xfafdu,f,*fpul);if(status)return status;goto L8c09e99e; /* fsca FPUL,dr10 */
L8c09e99e: *stop=0x8c09e99eu;fcopy(f,7u,1u);goto L8c09e9a0; /* fmov fr1,fr7 */
L8c09e9a0: *stop=0x8c09e9a0u;fcopy(f,1u,0u);goto L8c09e9a2; /* fmov fr0,fr1 */
L8c09e9a2: *stop=0x8c09e9a2u;fcopy(f,8u,2u);goto L8c09e9a4; /* fmov fr2,fr8 */
L8c09e9a4: *stop=0x8c09e9a4u;fcopy(f,9u,2u);goto L8c09e9a6; /* fmov fr2,fr9 */
L8c09e9a6: *stop=0x8c09e9a6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[11],&f->fpscr,&fr[0]);if(status)return status;}goto L8c09e9a8; /* fmul fr11,fr0 */
L8c09e9a8: *stop=0x8c09e9a8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[10],&f->fpscr,&fr[1]);if(status)return status;}goto L8c09e9aa; /* fmul fr10,fr1 */
L8c09e9aa: *stop=0x8c09e9aau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[11],&f->fpscr,&fr[8]);if(status)return status;}goto L8c09e9ac; /* fmul fr11,fr8 */
L8c09e9ac: *stop=0x8c09e9acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[10],&f->fpscr,&fr[9]);if(status)return status;}goto L8c09e9ae; /* fmul fr10,fr9 */
L8c09e9ae: *stop=0x8c09e9aeu;fcopy(f,14u,7u);goto L8c09e9b0; /* fmov fr7,fr14 */
L8c09e9b0: *stop=0x8c09e9b0u;fcopy(f,15u,3u);goto L8c09e9b2; /* fmov fr3,fr15 */
L8c09e9b2: *stop=0x8c09e9b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[10],&f->fpscr,&fr[7]);if(status)return status;}goto L8c09e9b4; /* fmul fr10,fr7 */
L8c09e9b4: *stop=0x8c09e9b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[14],&fr[11],&f->fpscr,&fr[14]);if(status)return status;}goto L8c09e9b6; /* fmul fr11,fr14 */
L8c09e9b6: *stop=0x8c09e9b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[10],&f->fpscr,&fr[3]);if(status)return status;}goto L8c09e9b8; /* fmul fr10,fr3 */
L8c09e9b8: *stop=0x8c09e9b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[11],&f->fpscr,&fr[15]);if(status)return status;}goto L8c09e9ba; /* fmul fr11,fr15 */
L8c09e9ba: *stop=0x8c09e9bau;fcopy(f,4u,0u);goto L8c09e9bc; /* fmov fr0,fr4 */
L8c09e9bc: *stop=0x8c09e9bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c09e9be; /* fsub fr3,fr4 */
L8c09e9be: *stop=0x8c09e9beu;fcopy(f,5u,1u);goto L8c09e9c0; /* fmov fr1,fr5 */
L8c09e9c0: *stop=0x8c09e9c0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[15],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c09e9c2; /* fadd fr15,fr5 */
L8c09e9c2: *stop=0x8c09e9c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[7],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c09e9c4; /* fsub fr7,fr0 */
L8c09e9c4: *stop=0x8c09e9c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c09e9c6; /* fadd fr14,fr1 */
L8c09e9c6: *stop=0x8c09e9c6u;fcopy(f,12u,8u);goto L8c09e9c8; /* fmov fr8,fr12 */
L8c09e9c8: *stop=0x8c09e9c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[3],&f->fpscr,&fr[12],1);if(status)return status;}goto L8c09e9ca; /* fsub fr3,fr12 */
L8c09e9ca: *stop=0x8c09e9cau;fcopy(f,13u,9u);goto L8c09e9cc; /* fmov fr9,fr13 */
L8c09e9cc: *stop=0x8c09e9ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[15],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c09e9ce; /* fadd fr15,fr13 */
L8c09e9ce: *stop=0x8c09e9ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[7],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c09e9d0; /* fsub fr7,fr8 */
L8c09e9d0: *stop=0x8c09e9d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[14],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c09e9ec; /* bra 0x8c09e9ec */
L8c09e9e0: *stop=0x8c09e9e0u;fcopy(f,4u,0u);goto L8c09e9e2; /* fmov fr0,fr4 */
L8c09e9e2: *stop=0x8c09e9e2u;fcopy(f,5u,3u);goto L8c09e9e4; /* fmov fr3,fr5 */
L8c09e9e4: *stop=0x8c09e9e4u;fcopy(f,8u,2u);goto L8c09e9e6; /* fmov fr2,fr8 */
L8c09e9e6: *stop=0x8c09e9e6u;fcopy(f,9u,1u);goto L8c09e9e8; /* fmov fr1,fr9 */
L8c09e9e8: *stop=0x8c09e9e8u;fcopy(f,12u,2u);goto L8c09e9ea; /* fmov fr2,fr12 */
L8c09e9ea: *stop=0x8c09e9eau;fcopy(f,13u,3u);goto L8c09e9ec; /* fmov fr3,fr13 */
L8c09e9ec: *stop=0x8c09e9ecu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09e9ee; /* fmov.s @r15+,fr3 */
L8c09e9ee: *stop=0x8c09e9eeu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09e9f0; /* fmov.s @r15+,fr7 */
L8c09e9f0: *stop=0x8c09e9f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c09e9f2; /* fadd fr3,fr0 */
L8c09e9f2: *stop=0x8c09e9f2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[7],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c09e9f4; /* fadd fr7,fr1 */
L8c09e9f4: *stop=0x8c09e9f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c09e9f6; /* fadd fr3,fr4 */
L8c09e9f6: *stop=0x8c09e9f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[7],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c09e9f8; /* fadd fr7,fr5 */
L8c09e9f8: *stop=0x8c09e9f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c09e9fa; /* fadd fr3,fr8 */
L8c09e9fa: *stop=0x8c09e9fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[7],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c09e9fc; /* fadd fr7,fr9 */
L8c09e9fc: *stop=0x8c09e9fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[3],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c09e9fe; /* fadd fr3,fr12 */
L8c09e9fe: *stop=0x8c09e9feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[7],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c09ea00; /* fadd fr7,fr13 */
L8c09ea00: *stop=0x8c09ea00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ea68u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ea02; /* mov.l 0x8c09ea68,r1 */
L8c09ea02: *stop=0x8c09ea02u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ea04; /* fmov.s @r1+,fr15 */
L8c09ea04: *stop=0x8c09ea04u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ea06; /* fmov.s @r1+,fr7 */
L8c09ea06: *stop=0x8c09ea06u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ea08; /* fmov.s @r1+,fr11 */
L8c09ea08: *stop=0x8c09ea08u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ea0a; /* fmov.s @r1+,fr3 */
L8c09ea0a: *stop=0x8c09ea0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ea6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c09ea0c; /* mov.l 0x8c09ea6c,r2 */
L8c09ea0c: *stop=0x8c09ea0cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ea70u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ea0e; /* mov.l 0x8c09ea70,r1 */
L8c09ea0e: *stop=0x8c09ea0eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c09ea10; /* fcmp/gt fr3,fr1 */
L8c09ea10: *stop=0x8c09ea10u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea12; /* rotcl r1 */
L8c09ea12: *stop=0x8c09ea12u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c09ea14; /* fcmp/gt fr1,fr7 */
L8c09ea14: *stop=0x8c09ea14u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea16; /* rotcl r1 */
L8c09ea16: *stop=0x8c09ea16u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,sr,1);if(status)return status;goto L8c09ea18; /* fcmp/gt fr11,fr0 */
L8c09ea18: *stop=0x8c09ea18u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea1a; /* rotcl r1 */
L8c09ea1a: *stop=0x8c09ea1au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c09ea1c; /* fcmp/gt fr0,fr15 */
L8c09ea1c: *stop=0x8c09ea1cu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea1e; /* rotcl r1 */
L8c09ea1e: *stop=0x8c09ea1eu;r[2]&=r[1];goto L8c09ea20; /* and r1,r2 */
L8c09ea20: *stop=0x8c09ea20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ea70u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ea22; /* mov.l 0x8c09ea70,r1 */
L8c09ea22: *stop=0x8c09ea22u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c09ea24; /* fcmp/gt fr3,fr5 */
L8c09ea24: *stop=0x8c09ea24u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea26; /* rotcl r1 */
L8c09ea26: *stop=0x8c09ea26u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c09ea28; /* fcmp/gt fr5,fr7 */
L8c09ea28: *stop=0x8c09ea28u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea2a; /* rotcl r1 */
L8c09ea2a: *stop=0x8c09ea2au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,sr,1);if(status)return status;goto L8c09ea2c; /* fcmp/gt fr11,fr4 */
L8c09ea2c: *stop=0x8c09ea2cu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea2e; /* rotcl r1 */
L8c09ea2e: *stop=0x8c09ea2eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c09ea30; /* fcmp/gt fr4,fr15 */
L8c09ea30: *stop=0x8c09ea30u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea32; /* rotcl r1 */
L8c09ea32: *stop=0x8c09ea32u;r[2]&=r[1];goto L8c09ea34; /* and r1,r2 */
L8c09ea34: *stop=0x8c09ea34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ea70u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ea36; /* mov.l 0x8c09ea70,r1 */
L8c09ea36: *stop=0x8c09ea36u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][9],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c09ea38; /* fcmp/gt fr3,fr9 */
L8c09ea38: *stop=0x8c09ea38u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea3a; /* rotcl r1 */
L8c09ea3a: *stop=0x8c09ea3au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][9],&f->fpscr,sr,1);if(status)return status;goto L8c09ea3c; /* fcmp/gt fr9,fr7 */
L8c09ea3c: *stop=0x8c09ea3cu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea3e; /* rotcl r1 */
L8c09ea3e: *stop=0x8c09ea3eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][8],f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,sr,1);if(status)return status;goto L8c09ea40; /* fcmp/gt fr11,fr8 */
L8c09ea40: *stop=0x8c09ea40u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea42; /* rotcl r1 */
L8c09ea42: *stop=0x8c09ea42u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][8],&f->fpscr,sr,1);if(status)return status;goto L8c09ea44; /* fcmp/gt fr8,fr15 */
L8c09ea44: *stop=0x8c09ea44u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea46; /* rotcl r1 */
L8c09ea46: *stop=0x8c09ea46u;r[2]&=r[1];goto L8c09ea48; /* and r1,r2 */
L8c09ea48: *stop=0x8c09ea48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ea70u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ea4a; /* mov.l 0x8c09ea70,r1 */
L8c09ea4a: *stop=0x8c09ea4au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][13],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c09ea4c; /* fcmp/gt fr3,fr13 */
L8c09ea4c: *stop=0x8c09ea4cu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea4e; /* rotcl r1 */
L8c09ea4e: *stop=0x8c09ea4eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][13],&f->fpscr,sr,1);if(status)return status;goto L8c09ea50; /* fcmp/gt fr13,fr7 */
L8c09ea50: *stop=0x8c09ea50u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea52; /* rotcl r1 */
L8c09ea52: *stop=0x8c09ea52u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,sr,1);if(status)return status;goto L8c09ea54; /* fcmp/gt fr11,fr12 */
L8c09ea54: *stop=0x8c09ea54u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea56; /* rotcl r1 */
L8c09ea56: *stop=0x8c09ea56u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][12],&f->fpscr,sr,1);if(status)return status;goto L8c09ea58; /* fcmp/gt fr12,fr15 */
L8c09ea58: *stop=0x8c09ea58u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ea5a; /* rotcl r1 */
L8c09ea5a: *stop=0x8c09ea5au;r[2]&=r[1];goto L8c09ea5c; /* and r1,r2 */
L8c09ea5c: *stop=0x8c09ea5cu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c09ea5e; /* tst r2,r2 */
L8c09ea5e: *stop=0x8c09ea5eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ea60; /* fmov.s @r15+,fr2 */
L8c09ea60: if((*sr&1u))goto L8c09ea74;goto L8c09ea62; /* bt 0x8c09ea74 */
L8c09ea62: *stop=0x8c09ea64u;r[0]=0u;goto L8c09eaec; /* bra 0x8c09eaec */
L8c09ea74: *stop=0x8c09ea74u;r[7]=r[9];goto L8c09ea76; /* mov r9,r7 */
L8c09ea76: *stop=0x8c09ea76u;r[9]+=64u;goto L8c09ea78; /* add #0x40,r9 */
L8c09ea78: *stop=0x8c09ea78u;r[9]+=64u;goto L8c09ea7a; /* add #0x40,r9 */
L8c09ea7a: *stop=0x8c09ea7au;r[7]+=28u;goto L8c09ea7c; /* add #0x1c,r7 */
L8c09ea7c: *stop=0x8c09ea7cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0x3f800000u;goto L8c09ea7e; /* fldi1 fr15 */
L8c09ea7e: *stop=0x8c09ea7eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09eb00u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ea80; /* mov.l 0x8c09eb00,r1 */
L8c09ea80: *stop=0x8c09ea80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09eb04u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c09ea82; /* mov.l 0x8c09eb04,r2 */
L8c09ea82: *stop=0x8c09ea82u;r[8]+=28u;goto L8c09ea84; /* add #0x1c,r8 */
L8c09ea84: *stop=0x8c09ea84u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c09ea86; /* fmov.s @r8,fr3 */
L8c09ea86: *stop=0x8c09ea86u;r[8]+=4294967292u;goto L8c09ea88; /* add #-0x4,r8 */
L8c09ea88: *stop=0x8c09ea88u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c09ea8a; /* fmov.s @r8,fr7 */
L8c09ea8a: *stop=0x8c09ea8au;r[8]+=4294967292u;goto L8c09ea8c; /* add #-0x4,r8 */
L8c09ea8c: *stop=0x8c09ea8cu;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[7]);if(io_status)return io_status;goto L8c09ea8e; /* fmov.s fr15,@-r7 */
L8c09ea8e: *stop=0x8c09ea8eu;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[7]);if(io_status)return io_status;goto L8c09ea90; /* fmov.s fr7,@-r7 */
L8c09ea90: *stop=0x8c09ea90u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c09ea92; /* fmov.s fr3,@-r7 */
L8c09ea92: *stop=0x8c09ea92u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c09ea94; /* fmov.s fr2,@-r7 */
L8c09ea94: *stop=0x8c09ea94u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[7]);if(io_status)return io_status;goto L8c09ea96; /* fmov.s fr1,@-r7 */
L8c09ea96: *stop=0x8c09ea96u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c09ea98; /* fmov.s fr0,@-r7 */
L8c09ea98: *stop=0x8c09ea98u;if(!valid_access(r[7]-4u,4u))return -3;r[7]-=4;wr(m,bus,&io_status,r[7],r[1],4);if(io_status)return io_status;goto L8c09ea9a; /* mov.l r1,@-r7 */
L8c09ea9a: *stop=0x8c09ea9au;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[7],1u);if(status)return status;goto L8c09ea9c; /* pref @r7 */
L8c09ea9c: *stop=0x8c09ea9cu;r[7]+=60u;goto L8c09ea9e; /* add #0x3c,r7 */
L8c09ea9e: *stop=0x8c09ea9eu;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c09eaa0; /* fmov.s @r8,fr3 */
L8c09eaa0: *stop=0x8c09eaa0u;r[8]+=4294967292u;goto L8c09eaa2; /* add #-0x4,r8 */
L8c09eaa2: *stop=0x8c09eaa2u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c09eaa4; /* fmov.s @r8,fr7 */
L8c09eaa4: *stop=0x8c09eaa4u;r[8]+=4294967292u;goto L8c09eaa6; /* add #-0x4,r8 */
L8c09eaa6: *stop=0x8c09eaa6u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[7]);if(io_status)return io_status;goto L8c09eaa8; /* fmov.s fr15,@-r7 */
L8c09eaa8: *stop=0x8c09eaa8u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[7]);if(io_status)return io_status;goto L8c09eaaa; /* fmov.s fr7,@-r7 */
L8c09eaaa: *stop=0x8c09eaaau;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c09eaac; /* fmov.s fr3,@-r7 */
L8c09eaac: *stop=0x8c09eaacu;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c09eaae; /* fmov.s fr2,@-r7 */
L8c09eaae: *stop=0x8c09eaaeu;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[7]);if(io_status)return io_status;goto L8c09eab0; /* fmov.s fr5,@-r7 */
L8c09eab0: *stop=0x8c09eab0u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[7]);if(io_status)return io_status;goto L8c09eab2; /* fmov.s fr4,@-r7 */
L8c09eab2: *stop=0x8c09eab2u;if(!valid_access(r[7]-4u,4u))return -3;r[7]-=4;wr(m,bus,&io_status,r[7],r[1],4);if(io_status)return io_status;goto L8c09eab4; /* mov.l r1,@-r7 */
L8c09eab4: *stop=0x8c09eab4u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[7],1u);if(status)return status;goto L8c09eab6; /* pref @r7 */
L8c09eab6: *stop=0x8c09eab6u;r[7]+=60u;goto L8c09eab8; /* add #0x3c,r7 */
L8c09eab8: *stop=0x8c09eab8u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c09eaba; /* fmov.s @r8,fr3 */
L8c09eaba: *stop=0x8c09eabau;r[8]+=4294967292u;goto L8c09eabc; /* add #-0x4,r8 */
L8c09eabc: *stop=0x8c09eabcu;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c09eabe; /* fmov.s @r8,fr7 */
L8c09eabe: *stop=0x8c09eabeu;r[8]+=4294967292u;goto L8c09eac0; /* add #-0x4,r8 */
L8c09eac0: *stop=0x8c09eac0u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[7]);if(io_status)return io_status;goto L8c09eac2; /* fmov.s fr15,@-r7 */
L8c09eac2: *stop=0x8c09eac2u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[7]);if(io_status)return io_status;goto L8c09eac4; /* fmov.s fr7,@-r7 */
L8c09eac4: *stop=0x8c09eac4u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c09eac6; /* fmov.s fr3,@-r7 */
L8c09eac6: *stop=0x8c09eac6u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c09eac8; /* fmov.s fr2,@-r7 */
L8c09eac8: *stop=0x8c09eac8u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[7]);if(io_status)return io_status;goto L8c09eaca; /* fmov.s fr9,@-r7 */
L8c09eaca: *stop=0x8c09eacau;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[7]);if(io_status)return io_status;goto L8c09eacc; /* fmov.s fr8,@-r7 */
L8c09eacc: *stop=0x8c09eaccu;if(!valid_access(r[7]-4u,4u))return -3;r[7]-=4;wr(m,bus,&io_status,r[7],r[1],4);if(io_status)return io_status;goto L8c09eace; /* mov.l r1,@-r7 */
L8c09eace: *stop=0x8c09eaceu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[7],1u);if(status)return status;goto L8c09ead0; /* pref @r7 */
L8c09ead0: *stop=0x8c09ead0u;r[7]+=60u;goto L8c09ead2; /* add #0x3c,r7 */
L8c09ead2: *stop=0x8c09ead2u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c09ead4; /* fmov.s @r8,fr3 */
L8c09ead4: *stop=0x8c09ead4u;r[8]+=4294967292u;goto L8c09ead6; /* add #-0x4,r8 */
L8c09ead6: *stop=0x8c09ead6u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c09ead8; /* fmov.s @r8,fr7 */
L8c09ead8: *stop=0x8c09ead8u;r[8]+=4294967292u;goto L8c09eada; /* add #-0x4,r8 */
L8c09eada: *stop=0x8c09eadau;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[7]);if(io_status)return io_status;goto L8c09eadc; /* fmov.s fr15,@-r7 */
L8c09eadc: *stop=0x8c09eadcu;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[7]);if(io_status)return io_status;goto L8c09eade; /* fmov.s fr7,@-r7 */
L8c09eade: *stop=0x8c09eadeu;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c09eae0; /* fmov.s fr3,@-r7 */
L8c09eae0: *stop=0x8c09eae0u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c09eae2; /* fmov.s fr2,@-r7 */
L8c09eae2: *stop=0x8c09eae2u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[7]);if(io_status)return io_status;goto L8c09eae4; /* fmov.s fr13,@-r7 */
L8c09eae4: *stop=0x8c09eae4u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[7]);if(io_status)return io_status;goto L8c09eae6; /* fmov.s fr12,@-r7 */
L8c09eae6: *stop=0x8c09eae6u;if(!valid_access(r[7]-4u,4u))return -3;r[7]-=4;wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c09eae8; /* mov.l r2,@-r7 */
L8c09eae8: *stop=0x8c09eae8u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[7],1u);if(status)return status;goto L8c09eaea; /* pref @r7 */
L8c09eaea: *stop=0x8c09eaeau;r[0]=1u;goto L8c09eaec; /* mov #0x1,r0 */
L8c09eaec: *stop=0x8c09eaecu;r[15]+=32u;goto L8c09eaee; /* add #0x20,r15 */
L8c09eaee: *stop=0x8c09eaeeu;r[2]=r[9];goto L8c09eaf0; /* mov r9,r2 */
L8c09eaf0: *stop=0x8c09eaf0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c09eaf2; /* mov.l @r15+,r8 */
L8c09eaf2: *stop=0x8c09eaf2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c09eaf4; /* mov.l @r15+,r9 */
L8c09eaf4: *stop=0x8c09eaf4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eaf6; /* fmov.s @r15+,fr12 */
L8c09eaf6: *stop=0x8c09eaf6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eaf8; /* fmov.s @r15+,fr13 */
L8c09eaf8: *stop=0x8c09eaf8u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eafa; /* fmov.s @r15+,fr14 */
L8c09eafa: *stop=0x8c09eafau;if(*pr!=entry_pr)return -3;*stop=0x8c09eafcu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;return 0; /* rts */
}

static int sa_player_child_update_run_8c09eb08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c09ee22u)goto L8c09ee22;goto L8c09eb08;
L8c09eb08: *stop=0x8c09eb08u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c09eb0a; /* fmov.s fr15,@-r15 */
L8c09eb0a: *stop=0x8c09eb0au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c09eb0c; /* fmov.s fr14,@-r15 */
L8c09eb0c: *stop=0x8c09eb0cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c09eb0e; /* fmov.s fr13,@-r15 */
L8c09eb0e: *stop=0x8c09eb0eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c09eb10; /* fmov.s fr12,@-r15 */
L8c09eb10: *stop=0x8c09eb10u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c09eb12; /* mov.l r9,@-r15 */
L8c09eb12: *stop=0x8c09eb12u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c09eb14; /* mov.l r8,@-r15 */
L8c09eb14: *stop=0x8c09eb14u;r[9]=r[2];goto L8c09eb16; /* mov r2,r9 */
L8c09eb16: *stop=0x8c09eb16u;r[8]=r[15];goto L8c09eb18; /* mov r15,r8 */
L8c09eb18: *stop=0x8c09eb18u;r[15]+=4294967264u;goto L8c09eb1a; /* add #-0x20,r15 */
L8c09eb1a: *stop=0x8c09eb1au;r[1]=r[5];goto L8c09eb1c; /* mov r5,r1 */
L8c09eb1c: *stop=0x8c09eb1cu;r[5]<<=2;goto L8c09eb1e; /* shll2 r5 */
L8c09eb1e: *stop=0x8c09eb1eu;r[5]+=r[1];goto L8c09eb20; /* add r1,r5 */
L8c09eb20: *stop=0x8c09eb20u;r[5]<<=2;goto L8c09eb22; /* shll2 r5 */
L8c09eb22: *stop=0x8c09eb22u;if(!valid_access(r[4]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09eb24; /* mov.l @(0x1c,r4),r1 */
L8c09eb24: *stop=0x8c09eb24u;r[0]=0x8c09ebecu;goto L8c09eb26; /* mova 0x8c09ebec,r0 */
L8c09eb26: *stop=0x8c09eb26u;r[5]+=r[1];goto L8c09eb28; /* add r1,r5 */
L8c09eb28: *stop=0x8c09eb28u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c09eb2a; /* fmov.s @r0,fr4 */
L8c09eb2a: *stop=0x8c09eb2au;r[5]+=8u;goto L8c09eb2c; /* add #0x8,r5 */
L8c09eb2c: *stop=0x8c09eb2cu;if(!valid_access(r[5],2u))return -3;r[2]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09eb2e; /* mov.w @r5+,r2 */
L8c09eb2e: *stop=0x8c09eb2eu;*fpul=r[2];goto L8c09eb30; /* lds r2,FPUL */
L8c09eb30: *stop=0x8c09eb30u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][0]);if(status)return status;goto L8c09eb32; /* float FPUL,fr0 */
L8c09eb32: *stop=0x8c09eb32u;if(!valid_access(r[5],2u))return -3;r[1]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09eb34; /* mov.w @r5+,r1 */
L8c09eb34: *stop=0x8c09eb34u;*fpul=r[1];goto L8c09eb36; /* lds r1,FPUL */
L8c09eb36: *stop=0x8c09eb36u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c09eb38; /* fmul fr4,fr0 */
L8c09eb38: *stop=0x8c09eb38u;if(!valid_access(r[5],2u))return -3;r[2]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09eb3a; /* mov.w @r5+,r2 */
L8c09eb3a: *stop=0x8c09eb3au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c09eb3c; /* float FPUL,fr1 */
L8c09eb3c: *stop=0x8c09eb3cu;*fpul=r[2];goto L8c09eb3e; /* lds r2,FPUL */
L8c09eb3e: *stop=0x8c09eb3eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c09eb40; /* fmul fr4,fr1 */
L8c09eb40: *stop=0x8c09eb40u;if(!valid_access(r[5],2u))return -3;r[1]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09eb42; /* mov.w @r5+,r1 */
L8c09eb42: *stop=0x8c09eb42u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c09eb44; /* float FPUL,fr2 */
L8c09eb44: *stop=0x8c09eb44u;*fpul=r[1];goto L8c09eb46; /* lds r1,FPUL */
L8c09eb46: *stop=0x8c09eb46u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c09eb48; /* fmul fr4,fr2 */
L8c09eb48: *stop=0x8c09eb48u;r[5]+=4294967280u;goto L8c09eb4a; /* add #-0x10,r5 */
L8c09eb4a: *stop=0x8c09eb4au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c09eb4c; /* float FPUL,fr3 */
L8c09eb4c: *stop=0x8c09eb4cu;r[1]=4u;goto L8c09eb4e; /* mov #0x4,r1 */
L8c09eb4e: *stop=0x8c09eb4eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c09eb50; /* fmul fr4,fr3 */
L8c09eb50: *stop=0x8c09eb50u;*sr=(*sr&~1u)|!(r[6]&r[1]);goto L8c09eb52; /* tst r6,r1 */
L8c09eb52: if((*sr&1u))goto L8c09eb5a;goto L8c09eb54; /* bt 0x8c09eb5a */
L8c09eb54: *stop=0x8c09eb54u;fcopy(f,4u,0u);goto L8c09eb56; /* fmov fr0,fr4 */
L8c09eb56: *stop=0x8c09eb56u;fcopy(f,0u,2u);goto L8c09eb58; /* fmov fr2,fr0 */
L8c09eb58: *stop=0x8c09eb58u;fcopy(f,2u,4u);goto L8c09eb5a; /* fmov fr4,fr2 */
L8c09eb5a: *stop=0x8c09eb5au;r[1]=8u;goto L8c09eb5c; /* mov #0x8,r1 */
L8c09eb5c: *stop=0x8c09eb5cu;*sr=(*sr&~1u)|!(r[6]&r[1]);goto L8c09eb5e; /* tst r6,r1 */
L8c09eb5e: if((*sr&1u))goto L8c09eb66;goto L8c09eb60; /* bt 0x8c09eb66 */
L8c09eb60: *stop=0x8c09eb60u;fcopy(f,4u,1u);goto L8c09eb62; /* fmov fr1,fr4 */
L8c09eb62: *stop=0x8c09eb62u;fcopy(f,1u,3u);goto L8c09eb64; /* fmov fr3,fr1 */
L8c09eb64: *stop=0x8c09eb64u;fcopy(f,3u,4u);goto L8c09eb66; /* fmov fr4,fr3 */
L8c09eb66: *stop=0x8c09eb66u;if(!valid_access(r[5],2u))return -3;r[2]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09eb68; /* mov.w @r5+,r2 */
L8c09eb68: *stop=0x8c09eb68u;if(!valid_access(r[5],2u))return -3;r[3]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09eb6a; /* mov.w @r5+,r3 */
L8c09eb6a: *stop=0x8c09eb6au;if(!valid_access(r[5],2u))return -3;r[0]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09eb6c; /* mov.w @r5+,r0 */
L8c09eb6c: *stop=0x8c09eb6cu;if(!valid_access(r[5],2u))return -3;r[1]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;r[5]+=2;goto L8c09eb6e; /* mov.w @r5+,r1 */
L8c09eb6e: *stop=0x8c09eb6eu;r[0]=0u-r[0];goto L8c09eb70; /* neg r0,r0 */
L8c09eb70: *stop=0x8c09eb70u;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c09eb72; /* fmov.s fr0,@-r8 */
L8c09eb72: *stop=0x8c09eb72u;r[1]=0u-r[1];goto L8c09eb74; /* neg r1,r1 */
L8c09eb74: *stop=0x8c09eb74u;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c09eb76; /* fmov.s fr1,@-r8 */
L8c09eb76: *stop=0x8c09eb76u;r[2]+=r[0];goto L8c09eb78; /* add r0,r2 */
L8c09eb78: *stop=0x8c09eb78u;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c09eb7a; /* fmov.s fr0,@-r8 */
L8c09eb7a: *stop=0x8c09eb7au;r[3]+=r[1];goto L8c09eb7c; /* add r1,r3 */
L8c09eb7c: *stop=0x8c09eb7cu;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c09eb7e; /* fmov.s fr3,@-r8 */
L8c09eb7e: *stop=0x8c09eb7eu;*fpul=r[0];goto L8c09eb80; /* lds r0,FPUL */
L8c09eb80: *stop=0x8c09eb80u;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c09eb82; /* fmov.s fr2,@-r8 */
L8c09eb82: *stop=0x8c09eb82u;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c09eb84; /* fmov.s fr1,@-r8 */
L8c09eb84: *stop=0x8c09eb84u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][0]);if(status)return status;goto L8c09eb86; /* float FPUL,fr0 */
L8c09eb86: *stop=0x8c09eb86u;*fpul=r[1];goto L8c09eb88; /* lds r1,FPUL */
L8c09eb88: *stop=0x8c09eb88u;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c09eb8a; /* fmov.s fr2,@-r8 */
L8c09eb8a: *stop=0x8c09eb8au;if(!valid_access(r[8]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c09eb8c; /* fmov.s fr3,@-r8 */
L8c09eb8c: *stop=0x8c09eb8cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c09eb8e; /* float FPUL,fr1 */
L8c09eb8e: *stop=0x8c09eb8eu;*fpul=r[2];goto L8c09eb90; /* lds r2,FPUL */
L8c09eb90: *stop=0x8c09eb90u;r[0]=12u;goto L8c09eb92; /* mov #0xc,r0 */
L8c09eb92: *stop=0x8c09eb92u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c09eb94; /* fmov.s @(r0,r4),fr4 */
L8c09eb94: *stop=0x8c09eb94u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c09eb96; /* float FPUL,fr2 */
L8c09eb96: *stop=0x8c09eb96u;*fpul=r[3];goto L8c09eb98; /* lds r3,FPUL */
L8c09eb98: *stop=0x8c09eb98u;r[0]=16u;goto L8c09eb9a; /* mov #0x10,r0 */
L8c09eb9a: *stop=0x8c09eb9au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c09eb9c; /* fmov.s @(r0,r4),fr5 */
L8c09eb9c: *stop=0x8c09eb9cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c09eb9e; /* float FPUL,fr3 */
L8c09eb9e: *stop=0x8c09eb9eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c09eba0; /* fmul fr4,fr0 */
L8c09eba0: *stop=0x8c09eba0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c09eba2; /* fmul fr4,fr2 */
L8c09eba2: *stop=0x8c09eba2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[5],&f->fpscr,&fr[1]);if(status)return status;}goto L8c09eba4; /* fmul fr5,fr1 */
L8c09eba4: *stop=0x8c09eba4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c09eba6; /* fmul fr5,fr3 */
L8c09eba6: *stop=0x8c09eba6u;r[0]=1u;goto L8c09eba8; /* mov #0x1,r0 */
L8c09eba8: *stop=0x8c09eba8u;*sr=(*sr&~1u)|!(r[0]&r[6]);goto L8c09ebaa; /* tst r0,r6 */
L8c09ebaa: if((*sr&1u))goto L8c09ebf0;goto L8c09ebac; /* bt 0x8c09ebf0 */
L8c09ebac: *stop=0x8c09ebacu;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ebae; /* mov.l @(0x14,r4),r1 */
L8c09ebae: *stop=0x8c09ebaeu;r[1]=r[1]&65535u;goto L8c09ebb0; /* extu.w r1,r1 */
L8c09ebb0: *stop=0x8c09ebb0u;*fpul=r[1];goto L8c09ebb2; /* lds r1,FPUL */
L8c09ebb2: *stop=0x8c09ebb2u;status=sa_sh4_vector_numeric(0xfafdu,f,*fpul);if(status)return status;goto L8c09ebb4; /* fsca FPUL,dr10 */
L8c09ebb4: *stop=0x8c09ebb4u;fcopy(f,7u,1u);goto L8c09ebb6; /* fmov fr1,fr7 */
L8c09ebb6: *stop=0x8c09ebb6u;fcopy(f,1u,0u);goto L8c09ebb8; /* fmov fr0,fr1 */
L8c09ebb8: *stop=0x8c09ebb8u;fcopy(f,8u,2u);goto L8c09ebba; /* fmov fr2,fr8 */
L8c09ebba: *stop=0x8c09ebbau;fcopy(f,9u,2u);goto L8c09ebbc; /* fmov fr2,fr9 */
L8c09ebbc: *stop=0x8c09ebbcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[11],&f->fpscr,&fr[0]);if(status)return status;}goto L8c09ebbe; /* fmul fr11,fr0 */
L8c09ebbe: *stop=0x8c09ebbeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[10],&f->fpscr,&fr[1]);if(status)return status;}goto L8c09ebc0; /* fmul fr10,fr1 */
L8c09ebc0: *stop=0x8c09ebc0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[11],&f->fpscr,&fr[8]);if(status)return status;}goto L8c09ebc2; /* fmul fr11,fr8 */
L8c09ebc2: *stop=0x8c09ebc2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[10],&f->fpscr,&fr[9]);if(status)return status;}goto L8c09ebc4; /* fmul fr10,fr9 */
L8c09ebc4: *stop=0x8c09ebc4u;fcopy(f,14u,7u);goto L8c09ebc6; /* fmov fr7,fr14 */
L8c09ebc6: *stop=0x8c09ebc6u;fcopy(f,15u,3u);goto L8c09ebc8; /* fmov fr3,fr15 */
L8c09ebc8: *stop=0x8c09ebc8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[10],&f->fpscr,&fr[7]);if(status)return status;}goto L8c09ebca; /* fmul fr10,fr7 */
L8c09ebca: *stop=0x8c09ebcau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[14],&fr[11],&f->fpscr,&fr[14]);if(status)return status;}goto L8c09ebcc; /* fmul fr11,fr14 */
L8c09ebcc: *stop=0x8c09ebccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[10],&f->fpscr,&fr[3]);if(status)return status;}goto L8c09ebce; /* fmul fr10,fr3 */
L8c09ebce: *stop=0x8c09ebceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[11],&f->fpscr,&fr[15]);if(status)return status;}goto L8c09ebd0; /* fmul fr11,fr15 */
L8c09ebd0: *stop=0x8c09ebd0u;fcopy(f,4u,0u);goto L8c09ebd2; /* fmov fr0,fr4 */
L8c09ebd2: *stop=0x8c09ebd2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c09ebd4; /* fsub fr3,fr4 */
L8c09ebd4: *stop=0x8c09ebd4u;fcopy(f,5u,1u);goto L8c09ebd6; /* fmov fr1,fr5 */
L8c09ebd6: *stop=0x8c09ebd6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[15],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c09ebd8; /* fadd fr15,fr5 */
L8c09ebd8: *stop=0x8c09ebd8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[7],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c09ebda; /* fsub fr7,fr0 */
L8c09ebda: *stop=0x8c09ebdau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c09ebdc; /* fadd fr14,fr1 */
L8c09ebdc: *stop=0x8c09ebdcu;fcopy(f,12u,8u);goto L8c09ebde; /* fmov fr8,fr12 */
L8c09ebde: *stop=0x8c09ebdeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[3],&f->fpscr,&fr[12],1);if(status)return status;}goto L8c09ebe0; /* fsub fr3,fr12 */
L8c09ebe0: *stop=0x8c09ebe0u;fcopy(f,13u,9u);goto L8c09ebe2; /* fmov fr9,fr13 */
L8c09ebe2: *stop=0x8c09ebe2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[15],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c09ebe4; /* fadd fr15,fr13 */
L8c09ebe4: *stop=0x8c09ebe4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[7],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c09ebe6; /* fsub fr7,fr8 */
L8c09ebe6: *stop=0x8c09ebe8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[14],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c09ebfc; /* bra 0x8c09ebfc */
L8c09ebf0: *stop=0x8c09ebf0u;fcopy(f,4u,0u);goto L8c09ebf2; /* fmov fr0,fr4 */
L8c09ebf2: *stop=0x8c09ebf2u;fcopy(f,5u,3u);goto L8c09ebf4; /* fmov fr3,fr5 */
L8c09ebf4: *stop=0x8c09ebf4u;fcopy(f,8u,2u);goto L8c09ebf6; /* fmov fr2,fr8 */
L8c09ebf6: *stop=0x8c09ebf6u;fcopy(f,9u,1u);goto L8c09ebf8; /* fmov fr1,fr9 */
L8c09ebf8: *stop=0x8c09ebf8u;fcopy(f,12u,2u);goto L8c09ebfa; /* fmov fr2,fr12 */
L8c09ebfa: *stop=0x8c09ebfau;fcopy(f,13u,3u);goto L8c09ebfc; /* fmov fr3,fr13 */
L8c09ebfc: *stop=0x8c09ebfcu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ebfe; /* fmov.s @r4+,fr3 */
L8c09ebfe: *stop=0x8c09ebfeu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ec00; /* fmov.s @r4+,fr7 */
L8c09ec00: *stop=0x8c09ec00u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c09ec02; /* fadd fr3,fr0 */
L8c09ec02: *stop=0x8c09ec02u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[7],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c09ec04; /* fadd fr7,fr1 */
L8c09ec04: *stop=0x8c09ec04u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ec06; /* fmov.s @r4+,fr2 */
L8c09ec06: *stop=0x8c09ec06u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c09ec08; /* fadd fr3,fr4 */
L8c09ec08: *stop=0x8c09ec08u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[7],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c09ec0a; /* fadd fr7,fr5 */
L8c09ec0a: *stop=0x8c09ec0au;fcopy(f,6u,2u);goto L8c09ec0c; /* fmov fr2,fr6 */
L8c09ec0c: *stop=0x8c09ec0cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c09ec0e; /* fadd fr3,fr8 */
L8c09ec0e: *stop=0x8c09ec0eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[7],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c09ec10; /* fadd fr7,fr9 */
L8c09ec10: *stop=0x8c09ec10u;fcopy(f,10u,2u);goto L8c09ec12; /* fmov fr2,fr10 */
L8c09ec12: *stop=0x8c09ec12u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[3],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c09ec14; /* fadd fr3,fr12 */
L8c09ec14: *stop=0x8c09ec14u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[7],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c09ec16; /* fadd fr7,fr13 */
L8c09ec16: *stop=0x8c09ec16u;fcopy(f,14u,2u);goto L8c09ec18; /* fmov fr2,fr14 */
L8c09ec18: *stop=0x8c09ec18u;r[2]=0u;goto L8c09ec1a; /* mov #0x0,r2 */
L8c09ec1a: *stop=0x8c09ec1au;r[3]=0u;goto L8c09ec1c; /* mov #0x0,r3 */
L8c09ec1c: *stop=0x8c09ec1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ec80u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c09ec1e; /* mov.l 0x8c09ec80,r0 */
L8c09ec1e: *stop=0x8c09ec1eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ec84u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ec20; /* mov.l 0x8c09ec84,r1 */
L8c09ec20: *stop=0x8c09ec20u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[0]);if(io_status)return io_status;goto L8c09ec22; /* fmov.s @r0,fr11 */
L8c09ec22: *stop=0x8c09ec22u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[1]);if(io_status)return io_status;goto L8c09ec24; /* fmov.s @r1,fr15 */
L8c09ec24: *stop=0x8c09ec24u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c09ec26; /* fldi1 fr3 */
L8c09ec26: *stop=0x8c09ec26u;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c09ec28; /* ftrv xmtrx,fv0 */
L8c09ec28: *stop=0x8c09ec28u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c09ec2a; /* fldi1 fr3 */
L8c09ec2a: *stop=0x8c09ec2au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c09ec2c; /* fdiv fr2,fr3 */
L8c09ec2c: *stop=0x8c09ec2cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,sr,1);if(status)return status;goto L8c09ec2e; /* fcmp/gt fr11,fr2 */
L8c09ec2e: *stop=0x8c09ec2eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[2]>>31);r[2]=(r[2]<<1)|incoming;}goto L8c09ec30; /* rotcl r2 */
L8c09ec30: *stop=0x8c09ec30u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c09ec32; /* fcmp/gt fr2,fr15 */
L8c09ec32: *stop=0x8c09ec32u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[3]>>31);r[3]=(r[3]<<1)|incoming;}goto L8c09ec34; /* rotcl r3 */
L8c09ec34: *stop=0x8c09ec34u;fcopy(f,2u,3u);goto L8c09ec36; /* fmov fr3,fr2 */
L8c09ec36: *stop=0x8c09ec36u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c09ec38; /* fldi1 fr7 */
L8c09ec38: *stop=0x8c09ec38u;status=sa_sh4_vector_numeric(0xf5fdu,f,*fpul);if(status)return status;goto L8c09ec3a; /* ftrv xmtrx,fv4 */
L8c09ec3a: *stop=0x8c09ec3au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c09ec3c; /* fldi1 fr7 */
L8c09ec3c: *stop=0x8c09ec3cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[7],&fr[6],&f->fpscr,&fr[7]);if(status)return status;}goto L8c09ec3e; /* fdiv fr6,fr7 */
L8c09ec3e: *stop=0x8c09ec3eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][6],f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,sr,1);if(status)return status;goto L8c09ec40; /* fcmp/gt fr11,fr6 */
L8c09ec40: *stop=0x8c09ec40u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[2]>>31);r[2]=(r[2]<<1)|incoming;}goto L8c09ec42; /* rotcl r2 */
L8c09ec42: *stop=0x8c09ec42u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][6],&f->fpscr,sr,1);if(status)return status;goto L8c09ec44; /* fcmp/gt fr6,fr15 */
L8c09ec44: *stop=0x8c09ec44u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[3]>>31);r[3]=(r[3]<<1)|incoming;}goto L8c09ec46; /* rotcl r3 */
L8c09ec46: *stop=0x8c09ec46u;fcopy(f,6u,7u);goto L8c09ec48; /* fmov fr7,fr6 */
L8c09ec48: *stop=0x8c09ec48u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c09ec4a; /* fmov.s @r0,fr3 */
L8c09ec4a: *stop=0x8c09ec4au;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[1]);if(io_status)return io_status;goto L8c09ec4c; /* fmov.s @r1,fr7 */
L8c09ec4c: *stop=0x8c09ec4cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][11]=0x3f800000u;goto L8c09ec4e; /* fldi1 fr11 */
L8c09ec4e: *stop=0x8c09ec4eu;status=sa_sh4_vector_numeric(0xf9fdu,f,*fpul);if(status)return status;goto L8c09ec50; /* ftrv xmtrx,fv8 */
L8c09ec50: *stop=0x8c09ec50u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][11]=0x3f800000u;goto L8c09ec52; /* fldi1 fr11 */
L8c09ec52: *stop=0x8c09ec52u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[11],&fr[10],&f->fpscr,&fr[11]);if(status)return status;}goto L8c09ec54; /* fdiv fr10,fr11 */
L8c09ec54: *stop=0x8c09ec54u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][10],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c09ec56; /* fcmp/gt fr3,fr10 */
L8c09ec56: *stop=0x8c09ec56u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[2]>>31);r[2]=(r[2]<<1)|incoming;}goto L8c09ec58; /* rotcl r2 */
L8c09ec58: *stop=0x8c09ec58u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][10],&f->fpscr,sr,1);if(status)return status;goto L8c09ec5a; /* fcmp/gt fr10,fr7 */
L8c09ec5a: *stop=0x8c09ec5au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[3]>>31);r[3]=(r[3]<<1)|incoming;}goto L8c09ec5c; /* rotcl r3 */
L8c09ec5c: *stop=0x8c09ec5cu;fcopy(f,10u,11u);goto L8c09ec5e; /* fmov fr11,fr10 */
L8c09ec5e: *stop=0x8c09ec5eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0x3f800000u;goto L8c09ec60; /* fldi1 fr15 */
L8c09ec60: *stop=0x8c09ec60u;status=sa_sh4_vector_numeric(0xfdfdu,f,*fpul);if(status)return status;goto L8c09ec62; /* ftrv xmtrx,fv12 */
L8c09ec62: *stop=0x8c09ec62u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0x3f800000u;goto L8c09ec64; /* fldi1 fr15 */
L8c09ec64: *stop=0x8c09ec64u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[15],&fr[14],&f->fpscr,&fr[15]);if(status)return status;}goto L8c09ec66; /* fdiv fr14,fr15 */
L8c09ec66: *stop=0x8c09ec66u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][14],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c09ec68; /* fcmp/gt fr3,fr14 */
L8c09ec68: *stop=0x8c09ec68u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[2]>>31);r[2]=(r[2]<<1)|incoming;}goto L8c09ec6a; /* rotcl r2 */
L8c09ec6a: *stop=0x8c09ec6au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][14],&f->fpscr,sr,1);if(status)return status;goto L8c09ec6c; /* fcmp/gt fr14,fr7 */
L8c09ec6c: *stop=0x8c09ec6cu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[3]>>31);r[3]=(r[3]<<1)|incoming;}goto L8c09ec6e; /* rotcl r3 */
L8c09ec6e: *stop=0x8c09ec6eu;fcopy(f,14u,15u);goto L8c09ec70; /* fmov fr15,fr14 */
L8c09ec70: *stop=0x8c09ec70u;r[1]=15u;goto L8c09ec72; /* mov #0xf,r1 */
L8c09ec72: *stop=0x8c09ec72u;*sr=(*sr&~1u)|(r[1]==r[2]);goto L8c09ec74; /* cmp/eq r1,r2 */
L8c09ec74: if((*sr&1u))goto L8c09ec7a;goto L8c09ec76; /* bt 0x8c09ec7a */
L8c09ec76: *stop=0x8c09ec76u;*sr=(*sr&~1u)|(r[1]==r[3]);goto L8c09ec78; /* cmp/eq r1,r3 */
L8c09ec78: if(!(*sr&1u))goto L8c09ec88;goto L8c09ec7a; /* bf 0x8c09ec88 */
L8c09ec7a: *stop=0x8c09ec7cu;r[0]=0u;goto L8c09ee2c; /* bra 0x8c09ee2c */
L8c09ec88: *stop=0x8c09ec88u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ed34u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c09ec8a; /* mov.l 0x8c09ed34,r2 */
L8c09ec8a: *stop=0x8c09ec8au;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c09ec8c; /* fmov.s @r2,fr3 */
L8c09ec8c: *stop=0x8c09ec8cu;fcopy(f,7u,3u);goto L8c09ec8e; /* fmov fr3,fr7 */
L8c09ec8e: *stop=0x8c09ec8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ed38u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c09ec90; /* mov.l 0x8c09ed38,r0 */
L8c09ec90: *stop=0x8c09ec90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ed3cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ec92; /* mov.l 0x8c09ed3c,r1 */
L8c09ec92: *stop=0x8c09ec92u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[0]);if(io_status)return io_status;goto L8c09ec94; /* fmov.s @r0,fr11 */
L8c09ec94: *stop=0x8c09ec94u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[1]);if(io_status)return io_status;goto L8c09ec96; /* fmov.s @r1,fr15 */
L8c09ec96: *stop=0x8c09ec96u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[11],&f->fpscr,&fr[3]);if(status)return status;}goto L8c09ec98; /* fmul fr11,fr3 */
L8c09ec98: *stop=0x8c09ec98u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[15],&f->fpscr,&fr[7]);if(status)return status;}goto L8c09ec9a; /* fmul fr15,fr7 */
L8c09ec9a: *stop=0x8c09ec9au;r[2]+=12u;goto L8c09ec9c; /* add #0xc,r2 */
L8c09ec9c: *stop=0x8c09ec9cu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ec9e; /* fmov.s @r2+,fr11 */
L8c09ec9e: *stop=0x8c09ec9eu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eca0; /* fmov.s @r2+,fr15 */
L8c09eca0: *stop=0x8c09eca0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[2],&f->fpscr,&fr[0]);if(status)return status;}goto L8c09eca2; /* fmul fr2,fr0 */
L8c09eca2: *stop=0x8c09eca2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c09eca4; /* fmul fr2,fr1 */
L8c09eca4: *stop=0x8c09eca4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c09eca6; /* fmul fr3,fr0 */
L8c09eca6: *stop=0x8c09eca6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c09eca8; /* fmul fr7,fr1 */
L8c09eca8: *stop=0x8c09eca8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[6],&f->fpscr,&fr[4]);if(status)return status;}goto L8c09ecaa; /* fmul fr6,fr4 */
L8c09ecaa: *stop=0x8c09ecaau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[11],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c09ecac; /* fadd fr11,fr0 */
L8c09ecac: *stop=0x8c09ecacu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[15],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c09ecae; /* fadd fr15,fr1 */
L8c09ecae: *stop=0x8c09ecaeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[6],&f->fpscr,&fr[5]);if(status)return status;}goto L8c09ecb0; /* fmul fr6,fr5 */
L8c09ecb0: *stop=0x8c09ecb0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c09ecb2; /* fmul fr3,fr4 */
L8c09ecb2: *stop=0x8c09ecb2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[7],&f->fpscr,&fr[5]);if(status)return status;}goto L8c09ecb4; /* fmul fr7,fr5 */
L8c09ecb4: *stop=0x8c09ecb4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[10],&f->fpscr,&fr[8]);if(status)return status;}goto L8c09ecb6; /* fmul fr10,fr8 */
L8c09ecb6: *stop=0x8c09ecb6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[11],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c09ecb8; /* fadd fr11,fr4 */
L8c09ecb8: *stop=0x8c09ecb8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[15],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c09ecba; /* fadd fr15,fr5 */
L8c09ecba: *stop=0x8c09ecbau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[10],&f->fpscr,&fr[9]);if(status)return status;}goto L8c09ecbc; /* fmul fr10,fr9 */
L8c09ecbc: *stop=0x8c09ecbcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[3],&f->fpscr,&fr[8]);if(status)return status;}goto L8c09ecbe; /* fmul fr3,fr8 */
L8c09ecbe: *stop=0x8c09ecbeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[7],&f->fpscr,&fr[9]);if(status)return status;}goto L8c09ecc0; /* fmul fr7,fr9 */
L8c09ecc0: *stop=0x8c09ecc0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[14],&f->fpscr,&fr[12]);if(status)return status;}goto L8c09ecc2; /* fmul fr14,fr12 */
L8c09ecc2: *stop=0x8c09ecc2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[11],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c09ecc4; /* fadd fr11,fr8 */
L8c09ecc4: *stop=0x8c09ecc4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[15],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c09ecc6; /* fadd fr15,fr9 */
L8c09ecc6: *stop=0x8c09ecc6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[14],&f->fpscr,&fr[13]);if(status)return status;}goto L8c09ecc8; /* fmul fr14,fr13 */
L8c09ecc8: *stop=0x8c09ecc8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[3],&f->fpscr,&fr[12]);if(status)return status;}goto L8c09ecca; /* fmul fr3,fr12 */
L8c09ecca: *stop=0x8c09eccau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[7],&f->fpscr,&fr[13]);if(status)return status;}goto L8c09eccc; /* fmul fr7,fr13 */
L8c09eccc: *stop=0x8c09ecccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[11],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c09ecce; /* fadd fr11,fr12 */
L8c09ecce: *stop=0x8c09ecceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[15],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c09ecd0; /* fadd fr15,fr13 */
L8c09ecd0: *stop=0x8c09ecd0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ed40u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ecd2; /* mov.l 0x8c09ed40,r1 */
L8c09ecd2: *stop=0x8c09ecd2u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ecd4; /* fmov.s @r1+,fr15 */
L8c09ecd4: *stop=0x8c09ecd4u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ecd6; /* fmov.s @r1+,fr7 */
L8c09ecd6: *stop=0x8c09ecd6u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ecd8; /* fmov.s @r1+,fr11 */
L8c09ecd8: *stop=0x8c09ecd8u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ecda; /* fmov.s @r1+,fr3 */
L8c09ecda: *stop=0x8c09ecdau;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ed44u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c09ecdc; /* mov.l 0x8c09ed44,r2 */
L8c09ecdc: *stop=0x8c09ecdcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ed48u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ecde; /* mov.l 0x8c09ed48,r1 */
L8c09ecde: *stop=0x8c09ecdeu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c09ece0; /* fcmp/gt fr3,fr1 */
L8c09ece0: *stop=0x8c09ece0u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ece2; /* rotcl r1 */
L8c09ece2: *stop=0x8c09ece2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c09ece4; /* fcmp/gt fr1,fr7 */
L8c09ece4: *stop=0x8c09ece4u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ece6; /* rotcl r1 */
L8c09ece6: *stop=0x8c09ece6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,sr,1);if(status)return status;goto L8c09ece8; /* fcmp/gt fr11,fr0 */
L8c09ece8: *stop=0x8c09ece8u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ecea; /* rotcl r1 */
L8c09ecea: *stop=0x8c09eceau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c09ecec; /* fcmp/gt fr0,fr15 */
L8c09ecec: *stop=0x8c09ececu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ecee; /* rotcl r1 */
L8c09ecee: *stop=0x8c09eceeu;r[2]&=r[1];goto L8c09ecf0; /* and r1,r2 */
L8c09ecf0: *stop=0x8c09ecf0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ed48u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ecf2; /* mov.l 0x8c09ed48,r1 */
L8c09ecf2: *stop=0x8c09ecf2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c09ecf4; /* fcmp/gt fr3,fr5 */
L8c09ecf4: *stop=0x8c09ecf4u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ecf6; /* rotcl r1 */
L8c09ecf6: *stop=0x8c09ecf6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c09ecf8; /* fcmp/gt fr5,fr7 */
L8c09ecf8: *stop=0x8c09ecf8u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ecfa; /* rotcl r1 */
L8c09ecfa: *stop=0x8c09ecfau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,sr,1);if(status)return status;goto L8c09ecfc; /* fcmp/gt fr11,fr4 */
L8c09ecfc: *stop=0x8c09ecfcu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ecfe; /* rotcl r1 */
L8c09ecfe: *stop=0x8c09ecfeu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c09ed00; /* fcmp/gt fr4,fr15 */
L8c09ed00: *stop=0x8c09ed00u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ed02; /* rotcl r1 */
L8c09ed02: *stop=0x8c09ed02u;r[2]&=r[1];goto L8c09ed04; /* and r1,r2 */
L8c09ed04: *stop=0x8c09ed04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ed48u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ed06; /* mov.l 0x8c09ed48,r1 */
L8c09ed06: *stop=0x8c09ed06u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][9],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c09ed08; /* fcmp/gt fr3,fr9 */
L8c09ed08: *stop=0x8c09ed08u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ed0a; /* rotcl r1 */
L8c09ed0a: *stop=0x8c09ed0au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][9],&f->fpscr,sr,1);if(status)return status;goto L8c09ed0c; /* fcmp/gt fr9,fr7 */
L8c09ed0c: *stop=0x8c09ed0cu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ed0e; /* rotcl r1 */
L8c09ed0e: *stop=0x8c09ed0eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][8],f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,sr,1);if(status)return status;goto L8c09ed10; /* fcmp/gt fr11,fr8 */
L8c09ed10: *stop=0x8c09ed10u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ed12; /* rotcl r1 */
L8c09ed12: *stop=0x8c09ed12u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][8],&f->fpscr,sr,1);if(status)return status;goto L8c09ed14; /* fcmp/gt fr8,fr15 */
L8c09ed14: *stop=0x8c09ed14u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ed16; /* rotcl r1 */
L8c09ed16: *stop=0x8c09ed16u;r[2]&=r[1];goto L8c09ed18; /* and r1,r2 */
L8c09ed18: *stop=0x8c09ed18u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ed48u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ed1a; /* mov.l 0x8c09ed48,r1 */
L8c09ed1a: *stop=0x8c09ed1au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][13],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c09ed1c; /* fcmp/gt fr3,fr13 */
L8c09ed1c: *stop=0x8c09ed1cu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ed1e; /* rotcl r1 */
L8c09ed1e: *stop=0x8c09ed1eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][7],f->bank[(f->fpscr>>21)&1u][13],&f->fpscr,sr,1);if(status)return status;goto L8c09ed20; /* fcmp/gt fr13,fr7 */
L8c09ed20: *stop=0x8c09ed20u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ed22; /* rotcl r1 */
L8c09ed22: *stop=0x8c09ed22u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,sr,1);if(status)return status;goto L8c09ed24; /* fcmp/gt fr11,fr12 */
L8c09ed24: *stop=0x8c09ed24u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ed26; /* rotcl r1 */
L8c09ed26: *stop=0x8c09ed26u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][12],&f->fpscr,sr,1);if(status)return status;goto L8c09ed28; /* fcmp/gt fr12,fr15 */
L8c09ed28: *stop=0x8c09ed28u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c09ed2a; /* rotcl r1 */
L8c09ed2a: *stop=0x8c09ed2au;r[2]&=r[1];goto L8c09ed2c; /* and r1,r2 */
L8c09ed2c: *stop=0x8c09ed2cu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c09ed2e; /* tst r2,r2 */
L8c09ed2e: if((*sr&1u))goto L8c09ed4c;goto L8c09ed30; /* bt 0x8c09ed4c */
L8c09ed30: *stop=0x8c09ed32u;r[0]=0u;goto L8c09ee2c; /* bra 0x8c09ee2c */
L8c09ed4c: *stop=0x8c09ed4cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c09ed4e; /* tst r3,r3 */
L8c09ed4e: if(!(*sr&1u))goto L8c09edd4;goto L8c09ed50; /* bf 0x8c09edd4 */
L8c09ed50: *stop=0x8c09ed50u;r[7]=r[9];goto L8c09ed52; /* mov r9,r7 */
L8c09ed52: *stop=0x8c09ed52u;r[9]+=64u;goto L8c09ed54; /* add #0x40,r9 */
L8c09ed54: *stop=0x8c09ed54u;r[9]+=64u;goto L8c09ed56; /* add #0x40,r9 */
L8c09ed56: *stop=0x8c09ed56u;r[7]+=28u;goto L8c09ed58; /* add #0x1c,r7 */
L8c09ed58: *stop=0x8c09ed58u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0x3f800000u;goto L8c09ed5a; /* fldi1 fr15 */
L8c09ed5a: *stop=0x8c09ed5au;{uint32_t loaded=rd(m,bus,&io_status,0x8c09edccu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ed5c; /* mov.l 0x8c09edcc,r1 */
L8c09ed5c: *stop=0x8c09ed5cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09edd0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c09ed5e; /* mov.l 0x8c09edd0,r2 */
L8c09ed5e: *stop=0x8c09ed5eu;r[8]+=28u;goto L8c09ed60; /* add #0x1c,r8 */
L8c09ed60: *stop=0x8c09ed60u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c09ed62; /* fmov.s @r8,fr3 */
L8c09ed62: *stop=0x8c09ed62u;r[8]+=4294967292u;goto L8c09ed64; /* add #-0x4,r8 */
L8c09ed64: *stop=0x8c09ed64u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c09ed66; /* fmov.s @r8,fr7 */
L8c09ed66: *stop=0x8c09ed66u;r[8]+=4294967292u;goto L8c09ed68; /* add #-0x4,r8 */
L8c09ed68: *stop=0x8c09ed68u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[7]);if(io_status)return io_status;goto L8c09ed6a; /* fmov.s fr15,@-r7 */
L8c09ed6a: *stop=0x8c09ed6au;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[7]);if(io_status)return io_status;goto L8c09ed6c; /* fmov.s fr7,@-r7 */
L8c09ed6c: *stop=0x8c09ed6cu;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c09ed6e; /* fmov.s fr3,@-r7 */
L8c09ed6e: *stop=0x8c09ed6eu;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c09ed70; /* fmov.s fr2,@-r7 */
L8c09ed70: *stop=0x8c09ed70u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[7]);if(io_status)return io_status;goto L8c09ed72; /* fmov.s fr1,@-r7 */
L8c09ed72: *stop=0x8c09ed72u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[7]);if(io_status)return io_status;goto L8c09ed74; /* fmov.s fr0,@-r7 */
L8c09ed74: *stop=0x8c09ed74u;if(!valid_access(r[7]-4u,4u))return -3;r[7]-=4;wr(m,bus,&io_status,r[7],r[1],4);if(io_status)return io_status;goto L8c09ed76; /* mov.l r1,@-r7 */
L8c09ed76: *stop=0x8c09ed76u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[7],1u);if(status)return status;goto L8c09ed78; /* pref @r7 */
L8c09ed78: *stop=0x8c09ed78u;r[7]+=60u;goto L8c09ed7a; /* add #0x3c,r7 */
L8c09ed7a: *stop=0x8c09ed7au;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c09ed7c; /* fmov.s @r8,fr3 */
L8c09ed7c: *stop=0x8c09ed7cu;r[8]+=4294967292u;goto L8c09ed7e; /* add #-0x4,r8 */
L8c09ed7e: *stop=0x8c09ed7eu;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c09ed80; /* fmov.s @r8,fr7 */
L8c09ed80: *stop=0x8c09ed80u;r[8]+=4294967292u;goto L8c09ed82; /* add #-0x4,r8 */
L8c09ed82: *stop=0x8c09ed82u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[7]);if(io_status)return io_status;goto L8c09ed84; /* fmov.s fr15,@-r7 */
L8c09ed84: *stop=0x8c09ed84u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[7]);if(io_status)return io_status;goto L8c09ed86; /* fmov.s fr7,@-r7 */
L8c09ed86: *stop=0x8c09ed86u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c09ed88; /* fmov.s fr3,@-r7 */
L8c09ed88: *stop=0x8c09ed88u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[7]);if(io_status)return io_status;goto L8c09ed8a; /* fmov.s fr6,@-r7 */
L8c09ed8a: *stop=0x8c09ed8au;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[7]);if(io_status)return io_status;goto L8c09ed8c; /* fmov.s fr5,@-r7 */
L8c09ed8c: *stop=0x8c09ed8cu;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[7]);if(io_status)return io_status;goto L8c09ed8e; /* fmov.s fr4,@-r7 */
L8c09ed8e: *stop=0x8c09ed8eu;if(!valid_access(r[7]-4u,4u))return -3;r[7]-=4;wr(m,bus,&io_status,r[7],r[1],4);if(io_status)return io_status;goto L8c09ed90; /* mov.l r1,@-r7 */
L8c09ed90: *stop=0x8c09ed90u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[7],1u);if(status)return status;goto L8c09ed92; /* pref @r7 */
L8c09ed92: *stop=0x8c09ed92u;r[7]+=60u;goto L8c09ed94; /* add #0x3c,r7 */
L8c09ed94: *stop=0x8c09ed94u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c09ed96; /* fmov.s @r8,fr3 */
L8c09ed96: *stop=0x8c09ed96u;r[8]+=4294967292u;goto L8c09ed98; /* add #-0x4,r8 */
L8c09ed98: *stop=0x8c09ed98u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c09ed9a; /* fmov.s @r8,fr7 */
L8c09ed9a: *stop=0x8c09ed9au;r[8]+=4294967292u;goto L8c09ed9c; /* add #-0x4,r8 */
L8c09ed9c: *stop=0x8c09ed9cu;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[7]);if(io_status)return io_status;goto L8c09ed9e; /* fmov.s fr15,@-r7 */
L8c09ed9e: *stop=0x8c09ed9eu;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[7]);if(io_status)return io_status;goto L8c09eda0; /* fmov.s fr7,@-r7 */
L8c09eda0: *stop=0x8c09eda0u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c09eda2; /* fmov.s fr3,@-r7 */
L8c09eda2: *stop=0x8c09eda2u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[7]);if(io_status)return io_status;goto L8c09eda4; /* fmov.s fr10,@-r7 */
L8c09eda4: *stop=0x8c09eda4u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[7]);if(io_status)return io_status;goto L8c09eda6; /* fmov.s fr9,@-r7 */
L8c09eda6: *stop=0x8c09eda6u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[7]);if(io_status)return io_status;goto L8c09eda8; /* fmov.s fr8,@-r7 */
L8c09eda8: *stop=0x8c09eda8u;if(!valid_access(r[7]-4u,4u))return -3;r[7]-=4;wr(m,bus,&io_status,r[7],r[1],4);if(io_status)return io_status;goto L8c09edaa; /* mov.l r1,@-r7 */
L8c09edaa: *stop=0x8c09edaau;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[7],1u);if(status)return status;goto L8c09edac; /* pref @r7 */
L8c09edac: *stop=0x8c09edacu;r[7]+=60u;goto L8c09edae; /* add #0x3c,r7 */
L8c09edae: *stop=0x8c09edaeu;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c09edb0; /* fmov.s @r8,fr3 */
L8c09edb0: *stop=0x8c09edb0u;r[8]+=4294967292u;goto L8c09edb2; /* add #-0x4,r8 */
L8c09edb2: *stop=0x8c09edb2u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;goto L8c09edb4; /* fmov.s @r8,fr7 */
L8c09edb4: *stop=0x8c09edb4u;r[8]+=4294967292u;goto L8c09edb6; /* add #-0x4,r8 */
L8c09edb6: *stop=0x8c09edb6u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[7]);if(io_status)return io_status;goto L8c09edb8; /* fmov.s fr15,@-r7 */
L8c09edb8: *stop=0x8c09edb8u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[7]);if(io_status)return io_status;goto L8c09edba; /* fmov.s fr7,@-r7 */
L8c09edba: *stop=0x8c09edbau;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[7]);if(io_status)return io_status;goto L8c09edbc; /* fmov.s fr3,@-r7 */
L8c09edbc: *stop=0x8c09edbcu;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[7]);if(io_status)return io_status;goto L8c09edbe; /* fmov.s fr14,@-r7 */
L8c09edbe: *stop=0x8c09edbeu;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[7]);if(io_status)return io_status;goto L8c09edc0; /* fmov.s fr13,@-r7 */
L8c09edc0: *stop=0x8c09edc0u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[7]);if(io_status)return io_status;goto L8c09edc2; /* fmov.s fr12,@-r7 */
L8c09edc2: *stop=0x8c09edc2u;if(!valid_access(r[7]-4u,4u))return -3;r[7]-=4;wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c09edc4; /* mov.l r2,@-r7 */
L8c09edc4: *stop=0x8c09edc4u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[7],1u);if(status)return status;goto L8c09edc6; /* pref @r7 */
L8c09edc6: *stop=0x8c09edc8u;r[0]=1u;goto L8c09ee2c; /* bra 0x8c09ee2c */
L8c09edd4: *stop=0x8c09edd4u;r[15]+=4294967288u;goto L8c09edd6; /* add #-0x8,r15 */
L8c09edd6: *stop=0x8c09edd6u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09edd8; /* fmov.s @r8+,fr7 */
L8c09edd8: *stop=0x8c09edd8u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09edda; /* fmov.s @r8+,fr3 */
L8c09edda: *stop=0x8c09eddau;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c09eddc; /* fmov.s fr7,@-r15 */
L8c09eddc: *stop=0x8c09eddcu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c09edde; /* fmov.s fr3,@-r15 */
L8c09edde: *stop=0x8c09eddeu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c09ede0; /* fmov.s fr14,@-r15 */
L8c09ede0: *stop=0x8c09ede0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c09ede2; /* fmov.s fr13,@-r15 */
L8c09ede2: *stop=0x8c09ede2u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c09ede4; /* fmov.s fr12,@-r15 */
L8c09ede4: *stop=0x8c09ede4u;r[15]+=4294967284u;goto L8c09ede6; /* add #-0xc,r15 */
L8c09ede6: *stop=0x8c09ede6u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ede8; /* fmov.s @r8+,fr7 */
L8c09ede8: *stop=0x8c09ede8u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09edea; /* fmov.s @r8+,fr3 */
L8c09edea: *stop=0x8c09edeau;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c09edec; /* fmov.s fr7,@-r15 */
L8c09edec: *stop=0x8c09edecu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c09edee; /* fmov.s fr3,@-r15 */
L8c09edee: *stop=0x8c09edeeu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c09edf0; /* fmov.s fr10,@-r15 */
L8c09edf0: *stop=0x8c09edf0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c09edf2; /* fmov.s fr9,@-r15 */
L8c09edf2: *stop=0x8c09edf2u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c09edf4; /* fmov.s fr8,@-r15 */
L8c09edf4: *stop=0x8c09edf4u;r[15]+=4294967284u;goto L8c09edf6; /* add #-0xc,r15 */
L8c09edf6: *stop=0x8c09edf6u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09edf8; /* fmov.s @r8+,fr7 */
L8c09edf8: *stop=0x8c09edf8u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09edfa; /* fmov.s @r8+,fr3 */
L8c09edfa: *stop=0x8c09edfau;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c09edfc; /* fmov.s fr7,@-r15 */
L8c09edfc: *stop=0x8c09edfcu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c09edfe; /* fmov.s fr3,@-r15 */
L8c09edfe: *stop=0x8c09edfeu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c09ee00; /* fmov.s fr6,@-r15 */
L8c09ee00: *stop=0x8c09ee00u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c09ee02; /* fmov.s fr5,@-r15 */
L8c09ee02: *stop=0x8c09ee02u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c09ee04; /* fmov.s fr4,@-r15 */
L8c09ee04: *stop=0x8c09ee04u;r[15]+=4294967284u;goto L8c09ee06; /* add #-0xc,r15 */
L8c09ee06: *stop=0x8c09ee06u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ee08; /* fmov.s @r8+,fr7 */
L8c09ee08: *stop=0x8c09ee08u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;r[8]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ee0a; /* fmov.s @r8+,fr3 */
L8c09ee0a: *stop=0x8c09ee0au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c09ee0c; /* fmov.s fr7,@-r15 */
L8c09ee0c: *stop=0x8c09ee0cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c09ee0e; /* fmov.s fr3,@-r15 */
L8c09ee0e: *stop=0x8c09ee0eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c09ee10; /* fmov.s fr2,@-r15 */
L8c09ee10: *stop=0x8c09ee10u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c09ee12; /* fmov.s fr1,@-r15 */
L8c09ee12: *stop=0x8c09ee12u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c09ee14; /* fmov.s fr0,@-r15 */
L8c09ee14: *stop=0x8c09ee14u;r[15]+=4294967292u;goto L8c09ee16; /* add #-0x4,r15 */
L8c09ee16: *stop=0x8c09ee16u;r[5]=r[15];goto L8c09ee18; /* mov r15,r5 */
L8c09ee18: *stop=0x8c09ee18u;r[2]=r[9];goto L8c09ee1a; /* mov r9,r2 */
L8c09ee1a: *stop=0x8c09ee1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ee40u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ee1c; /* mov.l 0x8c09ee40,r1 */
L8c09ee1c: *stop=0x8c09ee1cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c09ee1e; /* sts.l PR,@-r15 */
L8c09ee1e: target=r[1];*pr=0x8c09ee22u;*stop=0x8c09ee20u;r[4]=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09ee22; /* jsr @r1 */
L8c09ee22: *stop=0x8c09ee22u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c09ee24; /* lds.l @r15+,PR */
L8c09ee24: *stop=0x8c09ee24u;r[15]+=64u;goto L8c09ee26; /* add #0x40,r15 */
L8c09ee26: *stop=0x8c09ee26u;r[15]+=64u;goto L8c09ee28; /* add #0x40,r15 */
L8c09ee28: *stop=0x8c09ee28u;r[9]=r[2];goto L8c09ee2a; /* mov r2,r9 */
L8c09ee2a: *stop=0x8c09ee2au;r[0]=1u;goto L8c09ee2c; /* mov #0x1,r0 */
L8c09ee2c: *stop=0x8c09ee2cu;r[2]=r[9];goto L8c09ee2e; /* mov r9,r2 */
L8c09ee2e: *stop=0x8c09ee2eu;r[15]+=32u;goto L8c09ee30; /* add #0x20,r15 */
L8c09ee30: *stop=0x8c09ee30u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c09ee32; /* mov.l @r15+,r8 */
L8c09ee32: *stop=0x8c09ee32u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c09ee34; /* mov.l @r15+,r9 */
L8c09ee34: *stop=0x8c09ee34u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ee36; /* fmov.s @r15+,fr12 */
L8c09ee36: *stop=0x8c09ee36u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ee38; /* fmov.s @r15+,fr13 */
L8c09ee38: *stop=0x8c09ee38u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ee3a; /* fmov.s @r15+,fr14 */
L8c09ee3a: *stop=0x8c09ee3au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c09ee3cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_child_update_8c09eb08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c09eb08(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_child_update_resume_8c09ee22(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_child_update_run_8c09eb08(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c09ee22u);}

SA_STAGE_EXPORT int sa_player_child_update_8c606a88(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c606a88;
L8c606a88: *stop=0x8c606a88u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606b40u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c606a8a; /* mov.l 0x8c606b40,r2 */
L8c606a8a: *stop=0x8c606a8au;r[1]=r[4];goto L8c606a8c; /* mov r4,r1 */
L8c606a8c: *stop=0x8c606a8cu;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c606a8e; /* shll r4 */
L8c606a8e: *stop=0x8c606a8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c606b44u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c606a90; /* mov.l 0x8c606b44,r5 */
L8c606a90: *stop=0x8c606a90u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c606a92; /* mov.l @r2,r3 */
L8c606a92: *stop=0x8c606a92u;r[4]+=r[1];goto L8c606a94; /* add r1,r4 */
L8c606a94: *stop=0x8c606a94u;r[4]<<=2;goto L8c606a96; /* shll2 r4 */
L8c606a96: *stop=0x8c606a96u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[1]=loaded;}goto L8c606a98; /* mov.l @r5,r1 */
L8c606a98: *stop=0x8c606a98u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[3]=loaded;}goto L8c606a9a; /* mov.l @r3,r3 */
L8c606a9a: *stop=0x8c606a9au;r[4]+=r[3];goto L8c606a9c; /* add r3,r4 */
L8c606a9c: *stop=0x8c606a9cu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c606a9e; /* mov.l @(0x8,r4),r4 */
L8c606a9e: *stop=0x8c606a9eu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c606aa0; /* mov.l @r4,r3 */
L8c606aa0: *stop=0x8c606aa0u;*sr=(*sr&~1u)|(r[1]==r[3]);goto L8c606aa2; /* cmp/eq r1,r3 */
L8c606aa2: if(!(*sr&1u))goto L8c606aae;goto L8c606aa4; /* bf 0x8c606aae */
L8c606aa4: *stop=0x8c606aa4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606b48u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c606aa6; /* mov.l 0x8c606b48,r1 */
L8c606aa6: *stop=0x8c606aa6u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c606aa8; /* mov.l @(0x4,r4),r3 */
L8c606aa8: *stop=0x8c606aa8u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c606aaa; /* mov.l @r1,r0 */
L8c606aaa: *stop=0x8c606aaau;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c606aac; /* cmp/eq r3,r0 */
L8c606aac: if((*sr&1u))goto L8c606ada;goto L8c606aae; /* bt 0x8c606ada */
L8c606aae: *stop=0x8c606aaeu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c606ab0; /* mov.l @r4,r2 */
L8c606ab0: *stop=0x8c606ab0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606b48u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c606ab2; /* mov.l 0x8c606b48,r1 */
L8c606ab2: *stop=0x8c606ab2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606b20u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c606ab4; /* mov.l 0x8c606b20,r6 */
L8c606ab4: *stop=0x8c606ab4u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[2],4);if(io_status)return io_status;goto L8c606ab6; /* mov.l r2,@r5 */
L8c606ab6: *stop=0x8c606ab6u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c606ab8; /* mov.l @(0x4,r4),r3 */
L8c606ab8: *stop=0x8c606ab8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606b4cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c606aba; /* mov.l 0x8c606b4c,r2 */
L8c606aba: *stop=0x8c606abau;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c606abc; /* mov.l r3,@r1 */
L8c606abc: *stop=0x8c606abcu;r[3]=4294967232u;goto L8c606abe; /* mov #-0x40,r3 */
L8c606abe: *stop=0x8c606abeu;if(!valid_access(r[4]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c606ac0; /* mov.l @(0x8,r4),r0 */
L8c606ac0: *stop=0x8c606ac0u;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c606ac2; /* mov.l @(0x8,r6),r5 */
L8c606ac2: *stop=0x8c606ac2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606b50u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c606ac4; /* mov.l 0x8c606b50,r1 */
L8c606ac4: *stop=0x8c606ac4u;r[0]&=0x3fu;goto L8c606ac6; /* and #0x3f,r0 */
L8c606ac6: *stop=0x8c606ac6u;r[5]&=r[3];goto L8c606ac8; /* and r3,r5 */
L8c606ac8: *stop=0x8c606ac8u;r[5]|=r[0];goto L8c606aca; /* or r0,r5 */
L8c606aca: *stop=0x8c606acau;if(!valid_access(r[6]+8u,4u))return -3;wr(m,bus,&io_status,r[6]+0x8u,r[5],4);if(io_status)return io_status;goto L8c606acc; /* mov.l r5,@(0x8,r6) */
L8c606acc: *stop=0x8c606accu;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c606ace; /* mov.l @(0xc,r4),r4 */
L8c606ace: *stop=0x8c606aceu;r[2]&=r[4];goto L8c606ad0; /* and r4,r2 */
L8c606ad0: *stop=0x8c606ad0u;*sr=(*sr&~1u)|(r[2]>r[1]);goto L8c606ad2; /* cmp/hi r1,r2 */
L8c606ad2: if(!(*sr&1u))goto L8c606ad8;goto L8c606ad4; /* bf 0x8c606ad8 */
L8c606ad4: *stop=0x8c606ad4u;if(*pr!=entry_pr)return -3;*stop=0x8c606ad6u;r[0]=4294967295u;return 0; /* rts */
L8c606ad8: *stop=0x8c606ad8u;if(!valid_access(r[6]+12u,4u))return -3;wr(m,bus,&io_status,r[6]+0xcu,r[4],4);if(io_status)return io_status;goto L8c606ada; /* mov.l r4,@(0xc,r6) */
L8c606ada: *stop=0x8c606adau;r[0]=1u;goto L8c606adc; /* mov #0x1,r0 */
L8c606adc: *stop=0x8c606adcu;if(*pr!=entry_pr)return -3;*stop=0x8c606adeu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_child_update_8c6064b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6064b0;
L8c6064b0: *stop=0x8c6064b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606724u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6064b2; /* mov.l 0x8c606724,r7 */
L8c6064b2: *stop=0x8c6064b2u;r[6]=28u;goto L8c6064b4; /* mov #0x1c,r6 */
L8c6064b4: *stop=0x8c6064b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606728u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6064b6; /* mov.l 0x8c606728,r3 */
L8c6064b6: *stop=0x8c6064b6u;r[4]+=12u;goto L8c6064b8; /* add #0xc,r4 */
L8c6064b8: *stop=0x8c6064b8u;if(!valid_access(r[7]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6064ba; /* mov.l @(0x10,r7),r5 */
L8c6064ba: *stop=0x8c6064bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c60672cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6064bc; /* mov.l 0x8c60672c,r2 */
L8c6064bc: *stop=0x8c6064bcu;r[0]=r[5];goto L8c6064be; /* mov r5,r0 */
L8c6064be: *stop=0x8c6064beu;r[3]&=r[5];goto L8c6064c0; /* and r5,r3 */
L8c6064c0: *stop=0x8c6064c0u;r[0]>>=16;goto L8c6064c2; /* shlr16 r0 */
L8c6064c2: *stop=0x8c6064c2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6064c4; /* mov.l r3,@r2 */
L8c6064c4: *stop=0x8c6064c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606734u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6064c6; /* mov.l 0x8c606734,r3 */
L8c6064c6: *stop=0x8c6064c6u;r[0]>>=8;goto L8c6064c8; /* shlr8 r0 */
L8c6064c8: *stop=0x8c6064c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606738u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6064ca; /* mov.l 0x8c606738,r2 */
L8c6064ca: *stop=0x8c6064cau;r[6]&=r[0];goto L8c6064cc; /* and r0,r6 */
L8c6064cc: *stop=0x8c6064ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c60673cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6064ce; /* mov.l 0x8c60673c,r1 */
L8c6064ce: *stop=0x8c6064ceu;r[5]&=r[2];goto L8c6064d0; /* and r2,r5 */
L8c6064d0: *stop=0x8c6064d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606730u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6064d2; /* mov.l 0x8c606730,r0 */
L8c6064d2: *stop=0x8c6064d2u;r[5]|=r[1];goto L8c6064d4; /* or r1,r5 */
L8c6064d4: *stop=0x8c6064d4u;r[5]+=32u;goto L8c6064d6; /* add #0x20,r5 */
L8c6064d6: *stop=0x8c6064d6u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[6],4);if(io_status)return io_status;goto L8c6064d8; /* mov.l r6,@r0 */
L8c6064d8: *stop=0x8c6064d8u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[6],4);if(io_status)return io_status;goto L8c6064da; /* mov.l r6,@r3 */
L8c6064da: *stop=0x8c6064dau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6064dc; /* mov.l @r4,r3 */
L8c6064dc: *stop=0x8c6064dcu;r[4]+=4294967292u;goto L8c6064de; /* add #-0x4,r4 */
L8c6064de: *stop=0x8c6064deu;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064e0; /* mov.l r3,@-r5 */
L8c6064e0: *stop=0x8c6064e0u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6064e2; /* mov.l @r4,r3 */
L8c6064e2: *stop=0x8c6064e2u;r[4]+=4294967292u;goto L8c6064e4; /* add #-0x4,r4 */
L8c6064e4: *stop=0x8c6064e4u;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064e6; /* mov.l r3,@-r5 */
L8c6064e6: *stop=0x8c6064e6u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6064e8; /* mov.l @r4,r3 */
L8c6064e8: *stop=0x8c6064e8u;r[4]+=4294967292u;goto L8c6064ea; /* add #-0x4,r4 */
L8c6064ea: *stop=0x8c6064eau;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064ec; /* mov.l r3,@-r5 */
L8c6064ec: *stop=0x8c6064ecu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6064ee; /* mov.l @r4,r3 */
L8c6064ee: *stop=0x8c6064eeu;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064f0; /* mov.l r3,@-r5 */
L8c6064f0: *stop=0x8c6064f0u;if(!valid_access(r[7]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6064f2; /* mov.l @(0xc,r7),r3 */
L8c6064f2: *stop=0x8c6064f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606744u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6064f4; /* mov.l 0x8c606744,r2 */
L8c6064f4: *stop=0x8c6064f4u;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6064f6; /* mov.l r3,@-r5 */
L8c6064f6: *stop=0x8c6064f6u;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6064f8; /* mov.l @(0x8,r7),r0 */
L8c6064f8: *stop=0x8c6064f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606740u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6064fa; /* mov.l 0x8c606740,r3 */
L8c6064fa: *stop=0x8c6064fau;r[0]&=r[3];goto L8c6064fc; /* and r3,r0 */
L8c6064fc: *stop=0x8c6064fcu;r[0]|=r[2];goto L8c6064fe; /* or r2,r0 */
L8c6064fe: *stop=0x8c6064feu;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[0],4);if(io_status)return io_status;goto L8c606500; /* mov.l r0,@-r5 */
L8c606500: *stop=0x8c606500u;if(!valid_access(r[7]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c606502; /* mov.l @(0x4,r7),r1 */
L8c606502: *stop=0x8c606502u;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c606504; /* mov.l r1,@-r5 */
L8c606504: *stop=0x8c606504u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[1]=loaded;}goto L8c606506; /* mov.l @r7,r1 */
L8c606506: *stop=0x8c606506u;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c606508; /* mov.l r1,@-r5 */
L8c606508: *stop=0x8c606508u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[5],1u);if(status)return status;goto L8c60650a; /* pref @r5 */
L8c60650a: *stop=0x8c60650au;r[0]=r[7];goto L8c60650c; /* mov r7,r0 */
L8c60650c: *stop=0x8c60650cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c606748u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c60650e; /* mov.l 0x8c606748,r1 */
L8c60650e: *stop=0x8c60650eu;r[0]+=16u;goto L8c606510; /* add #0x10,r0 */
L8c606510: *stop=0x8c606510u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c606512; /* mov.l r0,@r1 */
L8c606512: *stop=0x8c606512u;r[5]+=32u;goto L8c606514; /* add #0x20,r5 */
L8c606514: *stop=0x8c606514u;if(*pr!=entry_pr)return -3;*stop=0x8c606516u;r[0]=r[5];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_child_update_8c6065b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6065b0;
L8c6065b0: *stop=0x8c6065b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606724u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6065b2; /* mov.l 0x8c606724,r7 */
L8c6065b2: *stop=0x8c6065b2u;r[6]=28u;goto L8c6065b4; /* mov #0x1c,r6 */
L8c6065b4: *stop=0x8c6065b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606728u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6065b6; /* mov.l 0x8c606728,r3 */
L8c6065b6: *stop=0x8c6065b6u;r[4]+=12u;goto L8c6065b8; /* add #0xc,r4 */
L8c6065b8: *stop=0x8c6065b8u;if(!valid_access(r[7]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x14u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6065ba; /* mov.l @(0x14,r7),r5 */
L8c6065ba: *stop=0x8c6065bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c60672cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6065bc; /* mov.l 0x8c60672c,r2 */
L8c6065bc: *stop=0x8c6065bcu;r[0]=r[5];goto L8c6065be; /* mov r5,r0 */
L8c6065be: *stop=0x8c6065beu;r[3]&=r[5];goto L8c6065c0; /* and r5,r3 */
L8c6065c0: *stop=0x8c6065c0u;r[0]>>=16;goto L8c6065c2; /* shlr16 r0 */
L8c6065c2: *stop=0x8c6065c2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6065c4; /* mov.l r3,@r2 */
L8c6065c4: *stop=0x8c6065c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606734u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6065c6; /* mov.l 0x8c606734,r3 */
L8c6065c6: *stop=0x8c6065c6u;r[0]>>=8;goto L8c6065c8; /* shlr8 r0 */
L8c6065c8: *stop=0x8c6065c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606738u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6065ca; /* mov.l 0x8c606738,r2 */
L8c6065ca: *stop=0x8c6065cau;r[6]&=r[0];goto L8c6065cc; /* and r0,r6 */
L8c6065cc: *stop=0x8c6065ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c60673cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6065ce; /* mov.l 0x8c60673c,r1 */
L8c6065ce: *stop=0x8c6065ceu;r[5]&=r[2];goto L8c6065d0; /* and r2,r5 */
L8c6065d0: *stop=0x8c6065d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606730u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6065d2; /* mov.l 0x8c606730,r0 */
L8c6065d2: *stop=0x8c6065d2u;r[5]|=r[1];goto L8c6065d4; /* or r1,r5 */
L8c6065d4: *stop=0x8c6065d4u;r[5]+=32u;goto L8c6065d6; /* add #0x20,r5 */
L8c6065d6: *stop=0x8c6065d6u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[6],4);if(io_status)return io_status;goto L8c6065d8; /* mov.l r6,@r0 */
L8c6065d8: *stop=0x8c6065d8u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[6],4);if(io_status)return io_status;goto L8c6065da; /* mov.l r6,@r3 */
L8c6065da: *stop=0x8c6065dau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6065dc; /* mov.l @r4,r3 */
L8c6065dc: *stop=0x8c6065dcu;r[4]+=4294967292u;goto L8c6065de; /* add #-0x4,r4 */
L8c6065de: *stop=0x8c6065deu;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065e0; /* mov.l r3,@-r5 */
L8c6065e0: *stop=0x8c6065e0u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6065e2; /* mov.l @r4,r3 */
L8c6065e2: *stop=0x8c6065e2u;r[4]+=4294967292u;goto L8c6065e4; /* add #-0x4,r4 */
L8c6065e4: *stop=0x8c6065e4u;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065e6; /* mov.l r3,@-r5 */
L8c6065e6: *stop=0x8c6065e6u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6065e8; /* mov.l @r4,r3 */
L8c6065e8: *stop=0x8c6065e8u;r[4]+=4294967292u;goto L8c6065ea; /* add #-0x4,r4 */
L8c6065ea: *stop=0x8c6065eau;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065ec; /* mov.l r3,@-r5 */
L8c6065ec: *stop=0x8c6065ecu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6065ee; /* mov.l @r4,r3 */
L8c6065ee: *stop=0x8c6065eeu;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065f0; /* mov.l r3,@-r5 */
L8c6065f0: *stop=0x8c6065f0u;if(!valid_access(r[7]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6065f2; /* mov.l @(0xc,r7),r3 */
L8c6065f2: *stop=0x8c6065f2u;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065f4; /* mov.l r3,@-r5 */
L8c6065f4: *stop=0x8c6065f4u;if(!valid_access(r[7]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6065f6; /* mov.l @(0x8,r7),r3 */
L8c6065f6: *stop=0x8c6065f6u;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065f8; /* mov.l r3,@-r5 */
L8c6065f8: *stop=0x8c6065f8u;if(!valid_access(r[7]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6065fa; /* mov.l @(0x4,r7),r3 */
L8c6065fa: *stop=0x8c6065fau;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6065fc; /* mov.l r3,@-r5 */
L8c6065fc: *stop=0x8c6065fcu;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6065fe; /* mov.l @r7,r3 */
L8c6065fe: *stop=0x8c6065feu;if(!valid_access(r[5]-4u,4u))return -3;r[5]-=4;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c606600; /* mov.l r3,@-r5 */
L8c606600: *stop=0x8c606600u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[5],1u);if(status)return status;goto L8c606602; /* pref @r5 */
L8c606602: *stop=0x8c606602u;r[0]=r[7];goto L8c606604; /* mov r7,r0 */
L8c606604: *stop=0x8c606604u;{uint32_t loaded=rd(m,bus,&io_status,0x8c606748u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c606606; /* mov.l 0x8c606748,r3 */
L8c606606: *stop=0x8c606606u;r[0]+=20u;goto L8c606608; /* add #0x14,r0 */
L8c606608: *stop=0x8c606608u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c60660a; /* mov.l r0,@r3 */
L8c60660a: *stop=0x8c60660au;r[5]+=32u;goto L8c60660c; /* add #0x20,r5 */
L8c60660c: *stop=0x8c60660cu;if(*pr!=entry_pr)return -3;*stop=0x8c60660eu;r[0]=r[5];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_child_update_8c09ee44(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c09ee44;
L8c09ee44: *stop=0x8c09ee44u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c09ee46; /* mov.l r14,@-r15 */
L8c09ee46: *stop=0x8c09ee46u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c09ee48; /* mov.l r13,@-r15 */
L8c09ee48: *stop=0x8c09ee48u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c09ee4a; /* mov.l r12,@-r15 */
L8c09ee4a: *stop=0x8c09ee4au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c09ee4c; /* mov.l r11,@-r15 */
L8c09ee4c: *stop=0x8c09ee4cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c09ee4e; /* mov.l r10,@-r15 */
L8c09ee4e: *stop=0x8c09ee4eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c09ee50; /* mov.l r9,@-r15 */
L8c09ee50: *stop=0x8c09ee50u;r[6]=r[15];goto L8c09ee52; /* mov r15,r6 */
L8c09ee52: *stop=0x8c09ee52u;r[15]+=4294967248u;goto L8c09ee54; /* add #-0x30,r15 */
L8c09ee54: *stop=0x8c09ee54u;r[15]+=4294967264u;goto L8c09ee56; /* add #-0x20,r15 */
L8c09ee56: *stop=0x8c09ee56u;r[7]=r[15];goto L8c09ee58; /* mov r15,r7 */
L8c09ee58: *stop=0x8c09ee58u;r[1]=r[15];goto L8c09ee5a; /* mov r15,r1 */
L8c09ee5a: *stop=0x8c09ee5au;r[15]+=4294967280u;goto L8c09ee5c; /* add #-0x10,r15 */
L8c09ee5c: *stop=0x8c09ee5cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ef14u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c09ee5e; /* mov.l 0x8c09ef14,r10 */
L8c09ee5e: *stop=0x8c09ee5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ef18u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c09ee60; /* mov.l 0x8c09ef18,r11 */
L8c09ee60: *stop=0x8c09ee60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ef1cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c09ee62; /* mov.l 0x8c09ef1c,r12 */
L8c09ee62: *stop=0x8c09ee62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ef20u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c09ee64; /* mov.l 0x8c09ef20,r13 */
L8c09ee64: *stop=0x8c09ee64u;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[11]);if(io_status)return io_status;goto L8c09ee66; /* fmov.s @r11,fr7 */
L8c09ee66: *stop=0x8c09ee66u;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[12]);if(io_status)return io_status;goto L8c09ee68; /* fmov.s @r12,fr8 */
L8c09ee68: *stop=0x8c09ee68u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[13]);if(io_status)return io_status;goto L8c09ee6a; /* fmov.s @r13,fr9 */
L8c09ee6a: *stop=0x8c09ee6au;r[11]+=12u;goto L8c09ee6c; /* add #0xc,r11 */
L8c09ee6c: *stop=0x8c09ee6cu;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[11]);if(io_status)return io_status;r[11]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ee6e; /* fmov.s @r11+,fr10 */
L8c09ee6e: *stop=0x8c09ee6eu;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[11]);if(io_status)return io_status;r[11]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ee70; /* fmov.s @r11+,fr11 */
L8c09ee70: *stop=0x8c09ee70u;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[10]);if(io_status)return io_status;goto L8c09ee72; /* fmov.s @r10,fr12 */
L8c09ee72: *stop=0x8c09ee72u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][13]=0x3f800000u;goto L8c09ee74; /* fldi1 fr13 */
L8c09ee74: *stop=0x8c09ee74u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[13],&fr[12],&f->fpscr,&fr[13]);if(status)return status;}goto L8c09ee76; /* fdiv fr12,fr13 */
L8c09ee76: *stop=0x8c09ee76u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[1]);if(io_status)return io_status;goto L8c09ee78; /* fmov.s fr11,@-r1 */
L8c09ee78: *stop=0x8c09ee78u;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[1]);if(io_status)return io_status;goto L8c09ee7a; /* fmov.s fr10,@-r1 */
L8c09ee7a: *stop=0x8c09ee7au;if(!valid_access(r[1]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[1]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[1]);if(io_status)return io_status;goto L8c09ee7c; /* fmov.s fr12,@-r1 */
L8c09ee7c: *stop=0x8c09ee7cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[7],&f->fpscr,&fr[8]);if(status)return status;}goto L8c09ee7e; /* fmul fr7,fr8 */
L8c09ee7e: *stop=0x8c09ee7eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[7],&f->fpscr,&fr[9]);if(status)return status;}goto L8c09ee80; /* fmul fr7,fr9 */
L8c09ee80: *stop=0x8c09ee80u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][14]=0x3f800000u;goto L8c09ee82; /* fldi1 fr14 */
L8c09ee82: *stop=0x8c09ee82u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0x3f800000u;goto L8c09ee84; /* fldi1 fr15 */
L8c09ee84: *stop=0x8c09ee84u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[14],&fr[8],&f->fpscr,&fr[14]);if(status)return status;}goto L8c09ee86; /* fdiv fr8,fr14 */
L8c09ee86: *stop=0x8c09ee86u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[15],&fr[9],&f->fpscr,&fr[15]);if(status)return status;}goto L8c09ee88; /* fdiv fr9,fr15 */
L8c09ee88: *stop=0x8c09ee88u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[13],&f->fpscr,&fr[8]);if(status)return status;}goto L8c09ee8a; /* fmul fr13,fr8 */
L8c09ee8a: *stop=0x8c09ee8au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[13],&f->fpscr,&fr[9]);if(status)return status;}goto L8c09ee8c; /* fmul fr13,fr9 */
L8c09ee8c: *stop=0x8c09ee8cu;r[5]+=100u;goto L8c09ee8e; /* add #0x64,r5 */
L8c09ee8e: *stop=0x8c09ee8eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ee90; /* fmov.s @r5+,fr0 */
L8c09ee90: *stop=0x8c09ee90u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ee92; /* fmov.s @r5+,fr1 */
L8c09ee92: *stop=0x8c09ee92u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ee94; /* fmov.s @r5+,fr3 */
L8c09ee94: *stop=0x8c09ee94u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c09ee96; /* fldi1 fr2 */
L8c09ee96: *stop=0x8c09ee96u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c09ee98; /* fdiv fr3,fr2 */
L8c09ee98: *stop=0x8c09ee98u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[10],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c09ee9a; /* fsub fr10,fr0 */
L8c09ee9a: *stop=0x8c09ee9au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c09ee9c; /* fsub fr11,fr1 */
L8c09ee9c: *stop=0x8c09ee9cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c09ee9e; /* fmul fr14,fr0 */
L8c09ee9e: *stop=0x8c09ee9eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[15],&f->fpscr,&fr[1]);if(status)return status;}goto L8c09eea0; /* fmul fr15,fr1 */
L8c09eea0: *stop=0x8c09eea0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[2],&f->fpscr,&fr[0]);if(status)return status;}goto L8c09eea2; /* fmul fr2,fr0 */
L8c09eea2: *stop=0x8c09eea2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c09eea4; /* fmul fr2,fr1 */
L8c09eea4: *stop=0x8c09eea4u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c09eea6; /* fmov.s fr2,@-r6 */
L8c09eea6: *stop=0x8c09eea6u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[6]);if(io_status)return io_status;goto L8c09eea8; /* fmov.s fr1,@-r6 */
L8c09eea8: *stop=0x8c09eea8u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c09eeaa; /* fmov.s fr0,@-r6 */
L8c09eeaa: *stop=0x8c09eeaau;r[5]+=4294967252u;goto L8c09eeac; /* add #-0x2c,r5 */
L8c09eeac: *stop=0x8c09eeacu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eeae; /* fmov.s @r5+,fr4 */
L8c09eeae: *stop=0x8c09eeaeu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eeb0; /* fmov.s @r5+,fr5 */
L8c09eeb0: *stop=0x8c09eeb0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eeb2; /* fmov.s @r5+,fr7 */
L8c09eeb2: *stop=0x8c09eeb2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c09eeb4; /* fldi1 fr6 */
L8c09eeb4: *stop=0x8c09eeb4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[7],&f->fpscr,&fr[6]);if(status)return status;}goto L8c09eeb6; /* fdiv fr7,fr6 */
L8c09eeb6: *stop=0x8c09eeb6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[10],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c09eeb8; /* fsub fr10,fr4 */
L8c09eeb8: *stop=0x8c09eeb8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[11],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c09eeba; /* fsub fr11,fr5 */
L8c09eeba: *stop=0x8c09eebau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[14],&f->fpscr,&fr[4]);if(status)return status;}goto L8c09eebc; /* fmul fr14,fr4 */
L8c09eebc: *stop=0x8c09eebcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[15],&f->fpscr,&fr[5]);if(status)return status;}goto L8c09eebe; /* fmul fr15,fr5 */
L8c09eebe: *stop=0x8c09eebeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[6],&f->fpscr,&fr[4]);if(status)return status;}goto L8c09eec0; /* fmul fr6,fr4 */
L8c09eec0: *stop=0x8c09eec0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[6],&f->fpscr,&fr[5]);if(status)return status;}goto L8c09eec2; /* fmul fr6,fr5 */
L8c09eec2: *stop=0x8c09eec2u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[6]);if(io_status)return io_status;goto L8c09eec4; /* fmov.s fr6,@-r6 */
L8c09eec4: *stop=0x8c09eec4u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[6]);if(io_status)return io_status;goto L8c09eec6; /* fmov.s fr5,@-r6 */
L8c09eec6: *stop=0x8c09eec6u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[6]);if(io_status)return io_status;goto L8c09eec8; /* fmov.s fr4,@-r6 */
L8c09eec8: *stop=0x8c09eec8u;r[5]+=4294967252u;goto L8c09eeca; /* add #-0x2c,r5 */
L8c09eeca: *stop=0x8c09eecau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eecc; /* fmov.s @r5+,fr0 */
L8c09eecc: *stop=0x8c09eeccu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eece; /* fmov.s @r5+,fr1 */
L8c09eece: *stop=0x8c09eeceu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eed0; /* fmov.s @r5+,fr3 */
L8c09eed0: *stop=0x8c09eed0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c09eed2; /* fldi1 fr2 */
L8c09eed2: *stop=0x8c09eed2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c09eed4; /* fdiv fr3,fr2 */
L8c09eed4: *stop=0x8c09eed4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[10],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c09eed6; /* fsub fr10,fr0 */
L8c09eed6: *stop=0x8c09eed6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c09eed8; /* fsub fr11,fr1 */
L8c09eed8: *stop=0x8c09eed8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[14],&f->fpscr,&fr[0]);if(status)return status;}goto L8c09eeda; /* fmul fr14,fr0 */
L8c09eeda: *stop=0x8c09eedau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[15],&f->fpscr,&fr[1]);if(status)return status;}goto L8c09eedc; /* fmul fr15,fr1 */
L8c09eedc: *stop=0x8c09eedcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[2],&f->fpscr,&fr[0]);if(status)return status;}goto L8c09eede; /* fmul fr2,fr0 */
L8c09eede: *stop=0x8c09eedeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c09eee0; /* fmul fr2,fr1 */
L8c09eee0: *stop=0x8c09eee0u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c09eee2; /* fmov.s fr2,@-r6 */
L8c09eee2: *stop=0x8c09eee2u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[6]);if(io_status)return io_status;goto L8c09eee4; /* fmov.s fr1,@-r6 */
L8c09eee4: *stop=0x8c09eee4u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c09eee6; /* fmov.s fr0,@-r6 */
L8c09eee6: *stop=0x8c09eee6u;r[5]+=4294967252u;goto L8c09eee8; /* add #-0x2c,r5 */
L8c09eee8: *stop=0x8c09eee8u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eeea; /* fmov.s @r5+,fr4 */
L8c09eeea: *stop=0x8c09eeeau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eeec; /* fmov.s @r5+,fr5 */
L8c09eeec: *stop=0x8c09eeecu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eeee; /* fmov.s @r5+,fr7 */
L8c09eeee: *stop=0x8c09eeeeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c09eef0; /* fldi1 fr6 */
L8c09eef0: *stop=0x8c09eef0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[7],&f->fpscr,&fr[6]);if(status)return status;}goto L8c09eef2; /* fdiv fr7,fr6 */
L8c09eef2: *stop=0x8c09eef2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[10],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c09eef4; /* fsub fr10,fr4 */
L8c09eef4: *stop=0x8c09eef4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[11],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c09eef6; /* fsub fr11,fr5 */
L8c09eef6: *stop=0x8c09eef6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[14],&f->fpscr,&fr[4]);if(status)return status;}goto L8c09eef8; /* fmul fr14,fr4 */
L8c09eef8: *stop=0x8c09eef8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[15],&f->fpscr,&fr[5]);if(status)return status;}goto L8c09eefa; /* fmul fr15,fr5 */
L8c09eefa: *stop=0x8c09eefau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[6],&f->fpscr,&fr[4]);if(status)return status;}goto L8c09eefc; /* fmul fr6,fr4 */
L8c09eefc: *stop=0x8c09eefcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[6],&f->fpscr,&fr[5]);if(status)return status;}goto L8c09eefe; /* fmul fr6,fr5 */
L8c09eefe: *stop=0x8c09eefeu;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[6]);if(io_status)return io_status;goto L8c09ef00; /* fmov.s fr6,@-r6 */
L8c09ef00: *stop=0x8c09ef00u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[6]);if(io_status)return io_status;goto L8c09ef02; /* fmov.s fr5,@-r6 */
L8c09ef02: *stop=0x8c09ef02u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[6]);if(io_status)return io_status;goto L8c09ef04; /* fmov.s fr4,@-r6 */
L8c09ef04: *stop=0x8c09ef04u;r[5]+=4294967280u;goto L8c09ef06; /* add #-0x10,r5 */
L8c09ef06: *stop=0x8c09ef06u;r[13]=r[5];goto L8c09ef08; /* mov r5,r13 */
L8c09ef08: *stop=0x8c09ef08u;r[11]=r[6];goto L8c09ef0a; /* mov r6,r11 */
L8c09ef0a: *stop=0x8c09ef0au;r[9]=r[4];goto L8c09ef0c; /* mov r4,r9 */
L8c09ef0c: *stop=0x8c09ef0cu;r[8]=2u;goto L8c09ef0e; /* mov #0x2,r8 */
L8c09ef0e: *stop=0x8c09ef10u;*sr=(*sr&~1u)|(r[9]&1u);r[9]>>=1;goto L8c09ef24; /* bra 0x8c09ef24 */
L8c09ef24: *stop=0x8c09ef24u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c09ef26; /* mov.l r8,@r15 */
L8c09ef26: *stop=0x8c09ef26u;r[10]=24u;goto L8c09ef28; /* mov #0x18,r10 */
L8c09ef28: *stop=0x8c09ef28u;r[10]+=r[11];goto L8c09ef2a; /* add r11,r10 */
L8c09ef2a: *stop=0x8c09ef2au;r[12]=64u;goto L8c09ef2c; /* mov #0x40,r12 */
L8c09ef2c: *stop=0x8c09ef2cu;r[12]+=r[13];goto L8c09ef2e; /* add r13,r12 */
L8c09ef2e: *stop=0x8c09ef2eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09efb0u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c09ef30; /* mov.l 0x8c09efb0,r8 */
L8c09ef30: *stop=0x8c09ef30u;r[9]+=r[8];goto L8c09ef32; /* add r8,r9 */
L8c09ef32: *stop=0x8c09ef32u;if(!valid_access(r[9],1u))return -3;r[9]=sx(m,bus,&io_status,r[9],1);if(io_status)return io_status;goto L8c09ef34; /* mov.b @r9,r9 */
L8c09ef34: *stop=0x8c09ef34u;r[9]=(uint32_t)(int32_t)(int8_t)r[9];goto L8c09ef36; /* exts.b r9,r9 */
L8c09ef36: *stop=0x8c09ef36u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09efb4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09ef38; /* mov.l 0x8c09efb4,r3 */
L8c09ef38: *stop=0x8c09ef38u;r[0]=0u;goto L8c09ef3a; /* mov #0x0,r0 */
L8c09ef3a: *stop=0x8c09ef3au;r[8]=3u;goto L8c09ef3c; /* mov #0x3,r8 */
L8c09ef3c: *stop=0x8c09ef3cu;*sr=(*sr&~1u)|(r[9]&1u);r[9]=(r[9]>>1)|(r[9]&0x80000000u);goto L8c09ef3e; /* shar r9 */
L8c09ef3e: if(!(*sr&1u))goto L8c09efd0;goto L8c09ef40; /* bf 0x8c09efd0 */
L8c09ef40: *stop=0x8c09ef40u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ef42; /* fmov.s @r1+,fr10 */
L8c09ef42: *stop=0x8c09ef42u;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[10]);if(io_status)return io_status;r[10]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ef44; /* fmov.s @r10+,fr0 */
L8c09ef44: *stop=0x8c09ef44u;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[10]);if(io_status)return io_status;r[10]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ef46; /* fmov.s @r10+,fr1 */
L8c09ef46: *stop=0x8c09ef46u;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[10]);if(io_status)return io_status;r[10]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ef48; /* fmov.s @r10+,fr2 */
L8c09ef48: *stop=0x8c09ef48u;r[10]+=4294967284u;goto L8c09ef4a; /* add #-0xc,r10 */
L8c09ef4a: *stop=0x8c09ef4au;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[11]);if(io_status)return io_status;r[11]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ef4c; /* fmov.s @r11+,fr4 */
L8c09ef4c: *stop=0x8c09ef4cu;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[11]);if(io_status)return io_status;r[11]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ef4e; /* fmov.s @r11+,fr5 */
L8c09ef4e: *stop=0x8c09ef4eu;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[11]);if(io_status)return io_status;r[11]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ef50; /* fmov.s @r11+,fr6 */
L8c09ef50: *stop=0x8c09ef50u;r[11]+=4294967284u;goto L8c09ef52; /* add #-0xc,r11 */
L8c09ef52: *stop=0x8c09ef52u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[10],&fr[2],&f->fpscr,&fr[10],1);if(status)return status;}goto L8c09ef54; /* fsub fr2,fr10 */
L8c09ef54: *stop=0x8c09ef54u;fcopy(f,11u,6u);goto L8c09ef56; /* fmov fr6,fr11 */
L8c09ef56: *stop=0x8c09ef56u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[2],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c09ef58; /* fsub fr2,fr11 */
L8c09ef58: *stop=0x8c09ef58u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[10],&fr[11],&f->fpscr,&fr[10]);if(status)return status;}goto L8c09ef5a; /* fdiv fr11,fr10 */
L8c09ef5a: *stop=0x8c09ef5au;fcopy(f,11u,4u);goto L8c09ef5c; /* fmov fr4,fr11 */
L8c09ef5c: *stop=0x8c09ef5cu;fcopy(f,12u,5u);goto L8c09ef5e; /* fmov fr5,fr12 */
L8c09ef5e: *stop=0x8c09ef5eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[0],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c09ef60; /* fsub fr0,fr11 */
L8c09ef60: *stop=0x8c09ef60u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[1],&f->fpscr,&fr[12],1);if(status)return status;}goto L8c09ef62; /* fsub fr1,fr12 */
L8c09ef62: *stop=0x8c09ef62u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[10],&f->fpscr,&fr[11]);if(status)return status;}goto L8c09ef64; /* fmul fr10,fr11 */
L8c09ef64: *stop=0x8c09ef64u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[10],&f->fpscr,&fr[12]);if(status)return status;}goto L8c09ef66; /* fmul fr10,fr12 */
L8c09ef66: *stop=0x8c09ef66u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[0],&f->fpscr,&fr[11],0);if(status)return status;}goto L8c09ef68; /* fadd fr0,fr11 */
L8c09ef68: *stop=0x8c09ef68u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[1],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c09ef6a; /* fadd fr1,fr12 */
L8c09ef6a: *stop=0x8c09ef6au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[8],&f->fpscr,&fr[11]);if(status)return status;}goto L8c09ef6c; /* fmul fr8,fr11 */
L8c09ef6c: *stop=0x8c09ef6cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[9],&f->fpscr,&fr[12]);if(status)return status;}goto L8c09ef6e; /* fmul fr9,fr12 */
L8c09ef6e: *stop=0x8c09ef6eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ef70; /* fmov.s @r1+,fr14 */
L8c09ef70: *stop=0x8c09ef70u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ef72; /* fmov.s @r1+,fr15 */
L8c09ef72: *stop=0x8c09ef72u;r[1]+=4294967284u;goto L8c09ef74; /* add #-0xc,r1 */
L8c09ef74: *stop=0x8c09ef74u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[14],&f->fpscr,&fr[11],0);if(status)return status;}goto L8c09ef76; /* fadd fr14,fr11 */
L8c09ef76: *stop=0x8c09ef76u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[15],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c09ef78; /* fadd fr15,fr12 */
L8c09ef78: *stop=0x8c09ef78u;r[12]+=16u;goto L8c09ef7a; /* add #0x10,r12 */
L8c09ef7a: *stop=0x8c09ef7au;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;r[12]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ef7c; /* fmov.s @r12+,fr3 */
L8c09ef7c: *stop=0x8c09ef7cu;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[12]);if(io_status)return io_status;r[12]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ef7e; /* fmov.s @r12+,fr7 */
L8c09ef7e: *stop=0x8c09ef7eu;r[12]+=4294967272u;goto L8c09ef80; /* add #-0x18,r12 */
L8c09ef80: *stop=0x8c09ef80u;r[13]+=16u;goto L8c09ef82; /* add #0x10,r13 */
L8c09ef82: *stop=0x8c09ef82u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[13]);if(io_status)return io_status;r[13]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ef84; /* fmov.s @r13+,fr14 */
L8c09ef84: *stop=0x8c09ef84u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[13]);if(io_status)return io_status;r[13]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09ef86; /* fmov.s @r13+,fr15 */
L8c09ef86: *stop=0x8c09ef86u;r[13]+=4294967272u;goto L8c09ef88; /* add #-0x18,r13 */
L8c09ef88: *stop=0x8c09ef88u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],1);if(status)return status;}goto L8c09ef8a; /* fsub fr3,fr14 */
L8c09ef8a: *stop=0x8c09ef8au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[7],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c09ef8c; /* fsub fr7,fr15 */
L8c09ef8c: *stop=0x8c09ef8cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[14],&fr[10],&f->fpscr,&fr[14]);if(status)return status;}goto L8c09ef8e; /* fmul fr10,fr14 */
L8c09ef8e: *stop=0x8c09ef8eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[10],&f->fpscr,&fr[15]);if(status)return status;}goto L8c09ef90; /* fmul fr10,fr15 */
L8c09ef90: *stop=0x8c09ef90u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c09ef92; /* fadd fr3,fr14 */
L8c09ef92: *stop=0x8c09ef92u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[7],&f->fpscr,&fr[15],0);if(status)return status;}goto L8c09ef94; /* fadd fr7,fr15 */
L8c09ef94: *stop=0x8c09ef94u;r[0]+=1u;goto L8c09ef96; /* add #0x1,r0 */
L8c09ef96: *stop=0x8c09ef96u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c09ef98; /* cmp/eq #0x3,r0 */
L8c09ef98: if(!(*sr&1u))goto L8c09efba;goto L8c09ef9a; /* bf 0x8c09efba */
L8c09ef9a: *stop=0x8c09ef9au;*sr=(*sr&~1u)|((int32_t)r[9]>=0);goto L8c09ef9c; /* cmp/pz r9 */
L8c09ef9c: if((*sr&1u))goto L8c09efb8;goto L8c09ef9e; /* bt 0x8c09efb8 */
L8c09ef9e: *stop=0x8c09ef9eu;r[7]+=24u;goto L8c09efa0; /* add #0x18,r7 */
L8c09efa0: *stop=0x8c09efa0u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[7]);if(io_status)return io_status;goto L8c09efa2; /* fmov.s fr15,@-r7 */
L8c09efa2: *stop=0x8c09efa2u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[7]);if(io_status)return io_status;goto L8c09efa4; /* fmov.s fr14,@-r7 */
L8c09efa4: *stop=0x8c09efa4u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[7]);if(io_status)return io_status;goto L8c09efa6; /* fmov.s fr13,@-r7 */
L8c09efa6: *stop=0x8c09efa6u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[7]);if(io_status)return io_status;goto L8c09efa8; /* fmov.s fr12,@-r7 */
L8c09efa8: *stop=0x8c09efa8u;if(!valid_access(r[7]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[7]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[7]);if(io_status)return io_status;goto L8c09efaa; /* fmov.s fr11,@-r7 */
L8c09efaa: *stop=0x8c09efaau;r[7]+=4294967292u;goto L8c09efac; /* add #-0x4,r7 */
L8c09efac: *stop=0x8c09efaeu;r[14]=r[7];goto L8c09efd0; /* bra 0x8c09efd0 */
L8c09efb8: *stop=0x8c09efb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09efe4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09efba; /* mov.l 0x8c09efe4,r3 */
L8c09efba: *stop=0x8c09efbau;r[2]+=28u;goto L8c09efbc; /* add #0x1c,r2 */
L8c09efbc: *stop=0x8c09efbcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]=0x3f800000u;goto L8c09efbe; /* fldi1 fr10 */
L8c09efbe: *stop=0x8c09efbeu;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[2]);if(io_status)return io_status;goto L8c09efc0; /* fmov.s fr10,@-r2 */
L8c09efc0: *stop=0x8c09efc0u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[2]);if(io_status)return io_status;goto L8c09efc2; /* fmov.s fr15,@-r2 */
L8c09efc2: *stop=0x8c09efc2u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[2]);if(io_status)return io_status;goto L8c09efc4; /* fmov.s fr14,@-r2 */
L8c09efc4: *stop=0x8c09efc4u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[2]);if(io_status)return io_status;goto L8c09efc6; /* fmov.s fr13,@-r2 */
L8c09efc6: *stop=0x8c09efc6u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[2]);if(io_status)return io_status;goto L8c09efc8; /* fmov.s fr12,@-r2 */
L8c09efc8: *stop=0x8c09efc8u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[2]);if(io_status)return io_status;goto L8c09efca; /* fmov.s fr11,@-r2 */
L8c09efca: *stop=0x8c09efcau;if(!valid_access(r[2]-4u,4u))return -3;r[2]-=4;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c09efcc; /* mov.l r3,@-r2 */
L8c09efcc: *stop=0x8c09efccu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c09efce; /* pref @r2 */
L8c09efce: *stop=0x8c09efceu;r[2]+=32u;goto L8c09efd0; /* add #0x20,r2 */
L8c09efd0: *stop=0x8c09efd0u;*sr=(*sr&~1u)|(r[9]&1u);r[9]=(r[9]>>1)|(r[9]&0x80000000u);goto L8c09efd2; /* shar r9 */
L8c09efd2: if(!(*sr&1u))goto L8c09f00e;goto L8c09efd4; /* bf 0x8c09f00e */
L8c09efd4: *stop=0x8c09efd4u;r[0]+=1u;goto L8c09efd6; /* add #0x1,r0 */
L8c09efd6: *stop=0x8c09efd6u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c09efd8; /* cmp/eq #0x3,r0 */
L8c09efd8: if(!(*sr&1u))goto L8c09efea;goto L8c09efda; /* bf 0x8c09efea */
L8c09efda: *stop=0x8c09efdau;*sr=(*sr&~1u)|((int32_t)r[9]>=0);goto L8c09efdc; /* cmp/pz r9 */
L8c09efdc: if((*sr&1u))goto L8c09efe8;goto L8c09efde; /* bt 0x8c09efe8 */
L8c09efde: *stop=0x8c09efe0u;r[14]=r[13];goto L8c09f00e; /* bra 0x8c09f00e */
L8c09efe8: *stop=0x8c09efe8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09f058u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09efea; /* mov.l 0x8c09f058,r3 */
L8c09efea: *stop=0x8c09efeau;r[13]+=4u;goto L8c09efec; /* add #0x4,r13 */
L8c09efec: *stop=0x8c09efecu;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[13]);if(io_status)return io_status;r[13]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09efee; /* fmov.s @r13+,fr11 */
L8c09efee: *stop=0x8c09efeeu;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[13]);if(io_status)return io_status;r[13]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eff0; /* fmov.s @r13+,fr12 */
L8c09eff0: *stop=0x8c09eff0u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[13]);if(io_status)return io_status;r[13]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eff2; /* fmov.s @r13+,fr7 */
L8c09eff2: *stop=0x8c09eff2u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[13]);if(io_status)return io_status;r[13]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eff4; /* fmov.s @r13+,fr14 */
L8c09eff4: *stop=0x8c09eff4u;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[13]);if(io_status)return io_status;r[13]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09eff6; /* fmov.s @r13+,fr15 */
L8c09eff6: *stop=0x8c09eff6u;r[2]+=28u;goto L8c09eff8; /* add #0x1c,r2 */
L8c09eff8: *stop=0x8c09eff8u;r[13]+=4294967272u;goto L8c09effa; /* add #-0x18,r13 */
L8c09effa: *stop=0x8c09effau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]=0x3f800000u;goto L8c09effc; /* fldi1 fr10 */
L8c09effc: *stop=0x8c09effcu;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[2]);if(io_status)return io_status;goto L8c09effe; /* fmov.s fr10,@-r2 */
L8c09effe: *stop=0x8c09effeu;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[2]);if(io_status)return io_status;goto L8c09f000; /* fmov.s fr15,@-r2 */
L8c09f000: *stop=0x8c09f000u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[2]);if(io_status)return io_status;goto L8c09f002; /* fmov.s fr14,@-r2 */
L8c09f002: *stop=0x8c09f002u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[2]);if(io_status)return io_status;goto L8c09f004; /* fmov.s fr7,@-r2 */
L8c09f004: *stop=0x8c09f004u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[2]);if(io_status)return io_status;goto L8c09f006; /* fmov.s fr12,@-r2 */
L8c09f006: *stop=0x8c09f006u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[2]);if(io_status)return io_status;goto L8c09f008; /* fmov.s fr11,@-r2 */
L8c09f008: *stop=0x8c09f008u;if(!valid_access(r[2]-4u,4u))return -3;r[2]-=4;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c09f00a; /* mov.l r3,@-r2 */
L8c09f00a: *stop=0x8c09f00au;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c09f00c; /* pref @r2 */
L8c09f00c: *stop=0x8c09f00cu;r[2]+=32u;goto L8c09f00e; /* add #0x20,r2 */
L8c09f00e: *stop=0x8c09f00eu;r[10]=r[11];goto L8c09f010; /* mov r11,r10 */
L8c09f010: *stop=0x8c09f010u;r[11]+=12u;goto L8c09f012; /* add #0xc,r11 */
L8c09f012: *stop=0x8c09f012u;r[8]--;*sr=(*sr&~1u)|(!r[8]);goto L8c09f014; /* dt r8 */
L8c09f014: *stop=0x8c09f014u;r[12]=r[13];goto L8c09f016; /* mov r13,r12 */
L8c09f016: take=!(*sr&1u);*stop=0x8c09f018u;r[13]+=32u;if(take)goto L8c09ef3c;goto L8c09f01a; /* bf/s 0x8c09ef3c */
L8c09f01a: *stop=0x8c09f01au;*sr=(*sr&~1u)|((int32_t)r[9]>=0);goto L8c09f01c; /* cmp/pz r9 */
L8c09f01c: if((*sr&1u))goto L8c09f042;goto L8c09f01e; /* bt 0x8c09f042 */
L8c09f01e: *stop=0x8c09f01eu;r[14]+=4u;goto L8c09f020; /* add #0x4,r14 */
L8c09f020: *stop=0x8c09f020u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[14]);if(io_status)return io_status;r[14]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09f022; /* fmov.s @r14+,fr11 */
L8c09f022: *stop=0x8c09f022u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[14]);if(io_status)return io_status;r[14]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09f024; /* fmov.s @r14+,fr12 */
L8c09f024: *stop=0x8c09f024u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[14]);if(io_status)return io_status;r[14]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09f026; /* fmov.s @r14+,fr13 */
L8c09f026: *stop=0x8c09f026u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[14]);if(io_status)return io_status;r[14]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09f028; /* fmov.s @r14+,fr14 */
L8c09f028: *stop=0x8c09f028u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[14]);if(io_status)return io_status;r[14]+=(f->fpscr&0x100000u)?8u:4u;goto L8c09f02a; /* fmov.s @r14+,fr15 */
L8c09f02a: *stop=0x8c09f02au;{uint32_t loaded=rd(m,bus,&io_status,0x8c09f058u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09f02c; /* mov.l 0x8c09f058,r3 */
L8c09f02c: *stop=0x8c09f02cu;r[2]+=28u;goto L8c09f02e; /* add #0x1c,r2 */
L8c09f02e: *stop=0x8c09f02eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]=0x3f800000u;goto L8c09f030; /* fldi1 fr10 */
L8c09f030: *stop=0x8c09f030u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[2]);if(io_status)return io_status;goto L8c09f032; /* fmov.s fr10,@-r2 */
L8c09f032: *stop=0x8c09f032u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[2]);if(io_status)return io_status;goto L8c09f034; /* fmov.s fr15,@-r2 */
L8c09f034: *stop=0x8c09f034u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[2]);if(io_status)return io_status;goto L8c09f036; /* fmov.s fr14,@-r2 */
L8c09f036: *stop=0x8c09f036u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[2]);if(io_status)return io_status;goto L8c09f038; /* fmov.s fr13,@-r2 */
L8c09f038: *stop=0x8c09f038u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[2]);if(io_status)return io_status;goto L8c09f03a; /* fmov.s fr12,@-r2 */
L8c09f03a: *stop=0x8c09f03au;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[2]);if(io_status)return io_status;goto L8c09f03c; /* fmov.s fr11,@-r2 */
L8c09f03c: *stop=0x8c09f03cu;if(!valid_access(r[2]-4u,4u))return -3;r[2]-=4;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c09f03e; /* mov.l r3,@-r2 */
L8c09f03e: *stop=0x8c09f03eu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c09f040; /* pref @r2 */
L8c09f040: *stop=0x8c09f040u;r[2]+=32u;goto L8c09f042; /* add #0x20,r2 */
L8c09f042: *stop=0x8c09f042u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}goto L8c09f044; /* mov.l @r15,r8 */
L8c09f044: *stop=0x8c09f044u;r[8]--;*sr=(*sr&~1u)|(!r[8]);goto L8c09f046; /* dt r8 */
L8c09f046: if((*sr&1u))goto L8c09f05c;goto L8c09f048; /* bt 0x8c09f05c */
L8c09f048: *stop=0x8c09f048u;r[13]=32u;goto L8c09f04a; /* mov #0x20,r13 */
L8c09f04a: *stop=0x8c09f04au;r[11]=12u;goto L8c09f04c; /* mov #0xc,r11 */
L8c09f04c: *stop=0x8c09f04cu;r[13]+=r[5];goto L8c09f04e; /* add r5,r13 */
L8c09f04e: *stop=0x8c09f04eu;r[11]+=r[6];goto L8c09f050; /* add r6,r11 */
L8c09f050: *stop=0x8c09f050u;r[9]=7u;goto L8c09f052; /* mov #0x7,r9 */
L8c09f052: *stop=0x8c09f054u;r[9]&=r[4];goto L8c09ef24; /* bra 0x8c09ef24 */
L8c09f05c: *stop=0x8c09f05cu;r[15]+=96u;goto L8c09f05e; /* add #0x60,r15 */
L8c09f05e: *stop=0x8c09f05eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c09f060; /* mov.l @r15+,r9 */
L8c09f060: *stop=0x8c09f060u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c09f062; /* mov.l @r15+,r10 */
L8c09f062: *stop=0x8c09f062u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c09f064; /* mov.l @r15+,r11 */
L8c09f064: *stop=0x8c09f064u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c09f066; /* mov.l @r15+,r12 */
L8c09f066: *stop=0x8c09f066u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c09f068; /* mov.l @r15+,r13 */
L8c09f068: *stop=0x8c09f068u;if(*pr!=entry_pr)return -3;*stop=0x8c09f06au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
