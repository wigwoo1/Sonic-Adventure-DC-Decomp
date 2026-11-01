/* Original complete response debug-line submission body.
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
static int sa_player_response_draw_run_8c63ce24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c63ce96u)goto L8c63ce96;if(resume_pc==0x8c63ceaeu)goto L8c63ceae;if(resume_pc==0x8c63ceb4u)goto L8c63ceb4;if(resume_pc==0x8c63cec4u)goto L8c63cec4;if(resume_pc==0x8c63cecau)goto L8c63ceca;if(resume_pc==0x8c63cedau)goto L8c63ceda;if(resume_pc==0x8c63cee0u)goto L8c63cee0;if(resume_pc==0x8c63ceeau)goto L8c63ceea;if(resume_pc==0x8c63cef0u)goto L8c63cef0;if(resume_pc==0x8c63cfb0u)goto L8c63cfb0;if(resume_pc==0x8c63cfceu)goto L8c63cfce;if(resume_pc==0x8c63cfd8u)goto L8c63cfd8;if(resume_pc==0x8c63d04eu)goto L8c63d04e;if(resume_pc==0x8c63d248u)goto L8c63d248;if(resume_pc==0x8c63d266u)goto L8c63d266;if(resume_pc==0x8c63d270u)goto L8c63d270;if(resume_pc==0x8c63d2a8u)goto L8c63d2a8;if(resume_pc==0x8c63d3e6u)goto L8c63d3e6;if(resume_pc==0x8c63d3ecu)goto L8c63d3ec;goto L8c63ce24;
L8c63ce24: *stop=0x8c63ce24u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c63ce26; /* mov.l r14,@-r15 */
L8c63ce26: *stop=0x8c63ce26u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c63ce28; /* mov.l r13,@-r15 */
L8c63ce28: *stop=0x8c63ce28u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c63ce2a; /* mov.l r12,@-r15 */
L8c63ce2a: *stop=0x8c63ce2au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c63ce2c; /* mov.l r11,@-r15 */
L8c63ce2c: *stop=0x8c63ce2cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c63ce2e; /* mov.l r10,@-r15 */
L8c63ce2e: *stop=0x8c63ce2eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c63ce30; /* mov.l r9,@-r15 */
L8c63ce30: *stop=0x8c63ce30u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c63ce32; /* mov.l r8,@-r15 */
L8c63ce32: *stop=0x8c63ce32u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c63ce34; /* fmov.s fr15,@-r15 */
L8c63ce34: *stop=0x8c63ce34u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c63ce36; /* fmov.s fr14,@-r15 */
L8c63ce36: *stop=0x8c63ce36u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c63ce38; /* fmov.s fr13,@-r15 */
L8c63ce38: *stop=0x8c63ce38u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c63ce3a; /* fmov.s fr12,@-r15 */
L8c63ce3a: *stop=0x8c63ce3au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c63ce3c; /* sts.l PR,@-r15 */
L8c63ce3c: *stop=0x8c63ce3cu;r[0]=sx(m,bus,&io_status,0x8c63cefeu,2);if(io_status)return io_status;goto L8c63ce3e; /* mov.w 0x8c63cefe,r0 */
L8c63ce3e: *stop=0x8c63ce3eu;r[15]+=r[0];goto L8c63ce40; /* add r0,r15 */
L8c63ce40: *stop=0x8c63ce40u;r[0]=0x8c63cf10u;goto L8c63ce42; /* mova 0x8c63cf10,r0 */
L8c63ce42: *stop=0x8c63ce42u;r[1]=sx(m,bus,&io_status,0x8c63cf00u,2);if(io_status)return io_status;goto L8c63ce44; /* mov.w 0x8c63cf00,r1 */
L8c63ce44: *stop=0x8c63ce44u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c63ce46; /* fmov.s @r0,fr3 */
L8c63ce46: *stop=0x8c63ce46u;r[0]=20u;goto L8c63ce48; /* mov #0x14,r0 */
L8c63ce48: *stop=0x8c63ce48u;r[1]+=r[15];goto L8c63ce4a; /* add r15,r1 */
L8c63ce4a: *stop=0x8c63ce4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf14u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c63ce4c; /* mov.l 0x8c63cf14,r12 */
L8c63ce4c: *stop=0x8c63ce4cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63ce4e; /* fmov fr3,@(r0,r15) */
L8c63ce4e: *stop=0x8c63ce4eu;r[9]=r[5];goto L8c63ce50; /* mov r5,r9 */
L8c63ce50: *stop=0x8c63ce50u;r[0]=sx(m,bus,&io_status,0x8c63cf02u,2);if(io_status)return io_status;goto L8c63ce52; /* mov.w 0x8c63cf02,r0 */
L8c63ce52: *stop=0x8c63ce52u;r[13]=r[4];goto L8c63ce54; /* mov r4,r13 */
L8c63ce54: *stop=0x8c63ce54u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63ce56; /* mov.l 0x8c63cf18,r3 */
L8c63ce56: *stop=0x8c63ce56u;r[11]=r[6];goto L8c63ce58; /* mov r6,r11 */
L8c63ce58: *stop=0x8c63ce58u;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[12]);if(io_status)return io_status;goto L8c63ce5a; /* fmov.s @r12,fr4 */
L8c63ce5a: *stop=0x8c63ce5au;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c63ce5c; /* fmov.s @r3,fr3 */
L8c63ce5c: *stop=0x8c63ce5cu;fcopy(f,15u,4u);goto L8c63ce5e; /* fmov fr4,fr15 */
L8c63ce5e: *stop=0x8c63ce5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63ce60; /* mov.l 0x8c63cf1c,r2 */
L8c63ce60: *stop=0x8c63ce60u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[3],&f->fpscr,&fr[15]);if(status)return status;}goto L8c63ce62; /* fmul fr3,fr15 */
L8c63ce62: *stop=0x8c63ce62u;fcopy(f,13u,4u);goto L8c63ce64; /* fmov fr4,fr13 */
L8c63ce64: *stop=0x8c63ce64u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c63ce66; /* fmov.s @r2,fr2 */
L8c63ce66: *stop=0x8c63ce66u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;goto L8c63ce68; /* mov.l r1,@(r0,r15) */
L8c63ce68: *stop=0x8c63ce68u;r[1]=r[15];goto L8c63ce6a; /* mov r15,r1 */
L8c63ce6a: *stop=0x8c63ce6au;r[0]=sx(m,bus,&io_status,0x8c63cf04u,2);if(io_status)return io_status;goto L8c63ce6c; /* mov.w 0x8c63cf04,r0 */
L8c63ce6c: *stop=0x8c63ce6cu;r[1]+=96u;goto L8c63ce6e; /* add #0x60,r1 */
L8c63ce6e: *stop=0x8c63ce6eu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;goto L8c63ce70; /* mov.l r1,@(r0,r15) */
L8c63ce70: *stop=0x8c63ce70u;r[1]=r[15];goto L8c63ce72; /* mov r15,r1 */
L8c63ce72: *stop=0x8c63ce72u;r[0]=sx(m,bus,&io_status,0x8c63cf06u,2);if(io_status)return io_status;goto L8c63ce74; /* mov.w 0x8c63cf06,r0 */
L8c63ce74: *stop=0x8c63ce74u;r[1]+=88u;goto L8c63ce76; /* add #0x58,r1 */
L8c63ce76: *stop=0x8c63ce76u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;goto L8c63ce78; /* mov.l r1,@(r0,r15) */
L8c63ce78: *stop=0x8c63ce78u;r[1]=r[15];goto L8c63ce7a; /* mov r15,r1 */
L8c63ce7a: *stop=0x8c63ce7au;r[0]=sx(m,bus,&io_status,0x8c63cf08u,2);if(io_status)return io_status;goto L8c63ce7c; /* mov.w 0x8c63cf08,r0 */
L8c63ce7c: *stop=0x8c63ce7cu;r[1]+=104u;goto L8c63ce7e; /* add #0x68,r1 */
L8c63ce7e: *stop=0x8c63ce7eu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;goto L8c63ce80; /* mov.l r1,@(r0,r15) */
L8c63ce80: *stop=0x8c63ce80u;r[0]=sx(m,bus,&io_status,0x8c63cf0au,2);if(io_status)return io_status;goto L8c63ce82; /* mov.w 0x8c63cf0a,r0 */
L8c63ce82: *stop=0x8c63ce82u;r[1]=r[15];goto L8c63ce84; /* mov r15,r1 */
L8c63ce84: *stop=0x8c63ce84u;r[1]+=80u;goto L8c63ce86; /* add #0x50,r1 */
L8c63ce86: *stop=0x8c63ce86u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;goto L8c63ce88; /* mov.l r1,@(r0,r15) */
L8c63ce88: *stop=0x8c63ce88u;r[1]=r[15];goto L8c63ce8a; /* mov r15,r1 */
L8c63ce8a: *stop=0x8c63ce8au;r[0]=sx(m,bus,&io_status,0x8c63cf0cu,2);if(io_status)return io_status;goto L8c63ce8c; /* mov.w 0x8c63cf0c,r0 */
L8c63ce8c: *stop=0x8c63ce8cu;r[1]+=72u;goto L8c63ce8e; /* add #0x48,r1 */
L8c63ce8e: *stop=0x8c63ce8eu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;goto L8c63ce90; /* mov.l r1,@(r0,r15) */
L8c63ce90: *stop=0x8c63ce90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c63ce92; /* mov.l 0x8c63cf20,r1 */
L8c63ce92: target=r[1];*pr=0x8c63ce96u;*stop=0x8c63ce94u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[2],&f->fpscr,&fr[13]);if(status)return status;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63ce96; /* jsr @r1 */
L8c63ce96: *stop=0x8c63ce96u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63ce98; /* mov.l 0x8c63cf28,r3 */
L8c63ce98: *stop=0x8c63ce98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf24u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c63ce9a; /* mov.l 0x8c63cf24,r14 */
L8c63ce9a: *stop=0x8c63ce9au;r[3]&=r[11];goto L8c63ce9c; /* and r11,r3 */
L8c63ce9c: *stop=0x8c63ce9cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c63ce9e; /* tst r3,r3 */
L8c63ce9e: take=(*sr&1u);*stop=0x8c63cea0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c63ceb8;goto L8c63cea2; /* bt/s 0x8c63ceb8 */
L8c63cea2: *stop=0x8c63cea2u;r[1]=64u;goto L8c63cea4; /* mov #0x40,r1 */
L8c63cea4: *stop=0x8c63cea4u;*sr=(*sr&~1u)|!(r[11]&r[1]);goto L8c63cea6; /* tst r11,r1 */
L8c63cea6: if((*sr&1u))goto L8c63ceb8;goto L8c63cea8; /* bt 0x8c63ceb8 */
L8c63cea8: *stop=0x8c63cea8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf2cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c63ceaa; /* mov.l 0x8c63cf2c,r4 */
L8c63ceaa: target=r[14];*pr=0x8c63ceaeu;*stop=0x8c63ceacu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63ceae; /* jsr @r14 */
L8c63ceae: *stop=0x8c63ceaeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63ceb0; /* mov.l 0x8c63cf30,r3 */
L8c63ceb0: target=r[3];*pr=0x8c63ceb4u;*stop=0x8c63ceb2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63ceb4; /* jsr @r3 */
L8c63ceb4: *stop=0x8c63ceb6u;goto L8c63ceca; /* bra 0x8c63ceca */
L8c63ceb8: *stop=0x8c63ceb8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63ceba; /* mov.l @r15,r3 */
L8c63ceba: *stop=0x8c63cebau;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c63cebc; /* tst r3,r3 */
L8c63cebc: if((*sr&1u))goto L8c63cece;goto L8c63cebe; /* bt 0x8c63cece */
L8c63cebe: *stop=0x8c63cebeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf34u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c63cec0; /* mov.l 0x8c63cf34,r4 */
L8c63cec0: target=r[14];*pr=0x8c63cec4u;*stop=0x8c63cec2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63cec4; /* jsr @r14 */
L8c63cec4: *stop=0x8c63cec4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63cec6; /* mov.l 0x8c63cf38,r3 */
L8c63cec6: target=r[3];*pr=0x8c63cecau;*stop=0x8c63cec8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63ceca; /* jsr @r3 */
L8c63ceca: *stop=0x8c63ceccu;r[14]=r[0];goto L8c63cef2; /* bra 0x8c63cef2 */
L8c63cece: *stop=0x8c63ceceu;r[3]=64u;goto L8c63ced0; /* mov #0x40,r3 */
L8c63ced0: *stop=0x8c63ced0u;*sr=(*sr&~1u)|!(r[11]&r[3]);goto L8c63ced2; /* tst r11,r3 */
L8c63ced2: if((*sr&1u))goto L8c63cee4;goto L8c63ced4; /* bt 0x8c63cee4 */
L8c63ced4: *stop=0x8c63ced4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf3cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c63ced6; /* mov.l 0x8c63cf3c,r4 */
L8c63ced6: target=r[14];*pr=0x8c63cedau;*stop=0x8c63ced8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63ceda; /* jsr @r14 */
L8c63ceda: *stop=0x8c63cedau;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63cedc; /* mov.l 0x8c63cf40,r3 */
L8c63cedc: target=r[3];*pr=0x8c63cee0u;*stop=0x8c63cedeu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63cee0; /* jsr @r3 */
L8c63cee0: *stop=0x8c63cee2u;goto L8c63cef0; /* bra 0x8c63cef0 */
L8c63cee4: *stop=0x8c63cee4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf44u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c63cee6; /* mov.l 0x8c63cf44,r4 */
L8c63cee6: target=r[14];*pr=0x8c63ceeau;*stop=0x8c63cee8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63ceea; /* jsr @r14 */
L8c63ceea: *stop=0x8c63ceeau;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63ceec; /* mov.l 0x8c63cf48,r3 */
L8c63ceec: target=r[3];*pr=0x8c63cef0u;*stop=0x8c63ceeeu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63cef0; /* jsr @r3 */
L8c63cef0: *stop=0x8c63cef0u;r[14]=r[0];goto L8c63cef2; /* mov r0,r14 */
L8c63cef2: *stop=0x8c63cef2u;r[8]=15u;goto L8c63cef4; /* mov #0xf,r8 */
L8c63cef4: *stop=0x8c63cef4u;*sr=(*sr&~1u)|!(r[11]&r[8]);goto L8c63cef6; /* tst r11,r8 */
L8c63cef6: take=(*sr&1u);*stop=0x8c63cef8u;r[4]=0u;if(take)goto L8c63cf4c;goto L8c63cefa; /* bt/s 0x8c63cf4c */
L8c63cefa: *stop=0x8c63cefcu;r[8]=1u;goto L8c63cf4e; /* bra 0x8c63cf4e */
L8c63cf4c: *stop=0x8c63cf4cu;r[8]=2u;goto L8c63cf4e; /* mov #0x2,r8 */
L8c63cf4e: *stop=0x8c63cf4eu;r[3]=1u;goto L8c63cf50; /* mov #0x1,r3 */
L8c63cf50: *stop=0x8c63cf50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63cf90u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c63cf52; /* mov.l 0x8c63cf90,r10 */
L8c63cf52: *stop=0x8c63cf52u;r[0]=0x8c63cf8cu;goto L8c63cf54; /* mova 0x8c63cf8c,r0 */
L8c63cf54: *stop=0x8c63cf54u;r[3]&=r[11];goto L8c63cf56; /* and r11,r3 */
L8c63cf56: *stop=0x8c63cf56u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c63cf58; /* fmov.s @r0,fr12 */
L8c63cf58: *stop=0x8c63cf58u;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c63cf5a; /* mov.l r3,@(0x18,r15) */
L8c63cf5a: *stop=0x8c63cf5au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c63cf5c; /* mov.l @r15,r2 */
L8c63cf5c: *stop=0x8c63cf5cu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c63cf5e; /* tst r2,r2 */
L8c63cf5e: take=!(*sr&1u);*stop=0x8c63cf60u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][14]=0x3f800000u;if(take)goto L8c63cf66;goto L8c63cf62; /* bf/s 0x8c63cf66 */
L8c63cf62: *stop=0x8c63cf64u;goto L8c63d202; /* bra 0x8c63d202 */
L8c63cf66: *stop=0x8c63cf66u;r[5]=r[4];goto L8c63cf68; /* mov r4,r5 */
L8c63cf68: *stop=0x8c63cf68u;*sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c63cf6a; /* cmp/pl r9 */
L8c63cf6a: *stop=0x8c63cf6au;*macl=r[8]*r[5];goto L8c63cf6c; /* mul.l r8,r5 */
L8c63cf6c: *stop=0x8c63cf6cu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c63cf6e; /* mov.l r4,@(0x8,r15) */
L8c63cf6e: *stop=0x8c63cf6eu;r[5]=*macl;goto L8c63cf70; /* sts MACL,r5 */
L8c63cf70: *stop=0x8c63cf70u;r[6]=r[5];goto L8c63cf72; /* mov r5,r6 */
L8c63cf72: *stop=0x8c63cf72u;r[6]+=1u;goto L8c63cf74; /* add #0x1,r6 */
L8c63cf74: *stop=0x8c63cf74u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c63cf76; /* mov.l r6,@r15 */
L8c63cf76: *stop=0x8c63cf76u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[6],4);if(io_status)return io_status;goto L8c63cf78; /* mov.l r6,@(0x10,r15) */
L8c63cf78: *stop=0x8c63cf78u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[5],4);if(io_status)return io_status;goto L8c63cf7a; /* mov.l r5,@(0xc,r15) */
L8c63cf7a: *stop=0x8c63cf7au;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c63cf7c; /* mov.l r3,@(0x18,r15) */
L8c63cf7c: if((*sr&1u))goto L8c63cf82;goto L8c63cf7e; /* bt 0x8c63cf82 */
L8c63cf7e: *stop=0x8c63cf80u;goto L8c63d3c2; /* bra 0x8c63d3c2 */
L8c63cf82: *stop=0x8c63cf82u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63cf84; /* mov.l @(0x18,r15),r3 */
L8c63cf84: *stop=0x8c63cf84u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c63cf86; /* tst r3,r3 */
L8c63cf86: if((*sr&1u))goto L8c63cf94;goto L8c63cf88; /* bt 0x8c63cf94 */
L8c63cf88: *stop=0x8c63cf8au;r[2]=0u;goto L8c63cf96; /* bra 0x8c63cf96 */
L8c63cf94: *stop=0x8c63cf94u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63cf96; /* mov.l @(0xc,r15),r2 */
L8c63cf96: *stop=0x8c63cf96u;r[5]=r[2];goto L8c63cf98; /* mov r2,r5 */
L8c63cf98: *stop=0x8c63cf98u;r[3]=r[2];goto L8c63cf9a; /* mov r2,r3 */
L8c63cf9a: *stop=0x8c63cf9au;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c63cf9c; /* shll r5 */
L8c63cf9c: *stop=0x8c63cf9cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c63cf9e; /* mov.l r2,@(0x4,r15) */
L8c63cf9e: *stop=0x8c63cf9eu;r[0]=sx(m,bus,&io_status,0x8c63d142u,2);if(io_status)return io_status;goto L8c63cfa0; /* mov.w 0x8c63d142,r0 */
L8c63cfa0: *stop=0x8c63cfa0u;r[5]+=r[3];goto L8c63cfa2; /* add r3,r5 */
L8c63cfa2: *stop=0x8c63cfa2u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c63cfa4; /* mov.l @r13,r2 */
L8c63cfa4: *stop=0x8c63cfa4u;r[5]<<=2;goto L8c63cfa6; /* shll2 r5 */
L8c63cfa6: *stop=0x8c63cfa6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d150u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63cfa8; /* mov.l 0x8c63d150,r3 */
L8c63cfa8: *stop=0x8c63cfa8u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c63cfaa; /* mov.l @(r0,r15),r6 */
L8c63cfaa: *stop=0x8c63cfaau;r[5]+=r[2];goto L8c63cfac; /* add r2,r5 */
L8c63cfac: target=r[3];*pr=0x8c63cfb0u;*stop=0x8c63cfaeu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63cfb0; /* jsr @r3 */
L8c63cfb0: *stop=0x8c63cfb0u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63cfb2; /* mov.l @(0x10,r15),r2 */
L8c63cfb2: *stop=0x8c63cfb2u;r[0]=sx(m,bus,&io_status,0x8c63d142u,2);if(io_status)return io_status;goto L8c63cfb4; /* mov.w 0x8c63d142,r0 */
L8c63cfb4: *stop=0x8c63cfb4u;r[5]=r[2];goto L8c63cfb6; /* mov r2,r5 */
L8c63cfb6: *stop=0x8c63cfb6u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c63cfb8; /* shll r5 */
L8c63cfb8: *stop=0x8c63cfb8u;r[3]=r[2];goto L8c63cfba; /* mov r2,r3 */
L8c63cfba: *stop=0x8c63cfbau;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c63cfbc; /* mov.l r2,@(0x1c,r15) */
L8c63cfbc: *stop=0x8c63cfbcu;r[5]+=r[3];goto L8c63cfbe; /* add r3,r5 */
L8c63cfbe: *stop=0x8c63cfbeu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c63cfc0; /* mov.l @r13,r2 */
L8c63cfc0: *stop=0x8c63cfc0u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c63cfc2; /* mov.l @(r0,r15),r6 */
L8c63cfc2: *stop=0x8c63cfc2u;r[5]<<=2;goto L8c63cfc4; /* shll2 r5 */
L8c63cfc4: *stop=0x8c63cfc4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d150u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63cfc6; /* mov.l 0x8c63d150,r3 */
L8c63cfc6: *stop=0x8c63cfc6u;r[5]+=r[2];goto L8c63cfc8; /* add r2,r5 */
L8c63cfc8: *stop=0x8c63cfc8u;r[6]+=12u;goto L8c63cfca; /* add #0xc,r6 */
L8c63cfca: target=r[3];*pr=0x8c63cfceu;*stop=0x8c63cfccu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63cfce; /* jsr @r3 */
L8c63cfce: *stop=0x8c63cfceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d154u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63cfd0; /* mov.l 0x8c63d154,r2 */
L8c63cfd0: *stop=0x8c63cfd0u;r[5]=2u;goto L8c63cfd2; /* mov #0x2,r5 */
L8c63cfd2: *stop=0x8c63cfd2u;r[4]=sx(m,bus,&io_status,0x8c63d142u,2);if(io_status)return io_status;goto L8c63cfd4; /* mov.w 0x8c63d142,r4 */
L8c63cfd4: target=r[2];*pr=0x8c63cfd8u;*stop=0x8c63cfd6u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63cfd8; /* jsr @r2 */
L8c63cfd8: *stop=0x8c63cfd8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c63cfda; /* tst r0,r0 */
L8c63cfda: if(!(*sr&1u))goto L8c63cfe0;goto L8c63cfdc; /* bf 0x8c63cfe0 */
L8c63cfdc: *stop=0x8c63cfdeu;goto L8c63d1dc; /* bra 0x8c63d1dc */
L8c63cfe0: *stop=0x8c63cfe0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c63cfe2; /* mov.l @(0x4,r15),r4 */
L8c63cfe2: *stop=0x8c63cfe2u;r[0]=sx(m,bus,&io_status,0x8c63d144u,2);if(io_status)return io_status;goto L8c63cfe4; /* mov.w 0x8c63d144,r0 */
L8c63cfe4: *stop=0x8c63cfe4u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63cfe6; /* mov.l @(0x8,r13),r3 */
L8c63cfe6: *stop=0x8c63cfe6u;r[4]<<=2;goto L8c63cfe8; /* shll2 r4 */
L8c63cfe8: *stop=0x8c63cfe8u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c63cfea; /* mov.l @(r0,r15),r2 */
L8c63cfea: *stop=0x8c63cfeau;r[0]=sx(m,bus,&io_status,0x8c63d144u,2);if(io_status)return io_status;goto L8c63cfec; /* mov.w 0x8c63d144,r0 */
L8c63cfec: *stop=0x8c63cfecu;r[3]+=r[4];goto L8c63cfee; /* add r4,r3 */
L8c63cfee: *stop=0x8c63cfeeu;if(!valid_access(r[3],2u))return -3;r[1]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c63cff0; /* mov.w @r3,r1 */
L8c63cff0: *stop=0x8c63cff0u;if(!valid_access(r[2],2u))return -3;wr(m,bus,&io_status,r[2],r[1],2);if(io_status)return io_status;goto L8c63cff2; /* mov.w r1,@r2 */
L8c63cff2: *stop=0x8c63cff2u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63cff4; /* mov.l @(r0,r15),r3 */
L8c63cff4: *stop=0x8c63cff4u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c63cff6; /* mov.l @(0x8,r13),r0 */
L8c63cff6: *stop=0x8c63cff6u;r[0]+=r[4];goto L8c63cff8; /* add r4,r0 */
L8c63cff8: *stop=0x8c63cff8u;if(!valid_access(r[0]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c63cffa; /* mov.w @(0x2,r0),r0 */
L8c63cffa: *stop=0x8c63cffau;if(!valid_access(r[3]+2u,2u))return -3;wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c63cffc; /* mov.w r0,@(0x2,r3) */
L8c63cffc: *stop=0x8c63cffcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c63cffe; /* mov.l @r15,r5 */
L8c63cffe: *stop=0x8c63cffeu;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d000; /* mov.l @(0x8,r13),r2 */
L8c63d000: *stop=0x8c63d000u;r[4]=r[5];goto L8c63d002; /* mov r5,r4 */
L8c63d002: *stop=0x8c63d002u;r[0]=sx(m,bus,&io_status,0x8c63d144u,2);if(io_status)return io_status;goto L8c63d004; /* mov.w 0x8c63d144,r0 */
L8c63d004: *stop=0x8c63d004u;r[4]<<=2;goto L8c63d006; /* shll2 r4 */
L8c63d006: *stop=0x8c63d006u;r[2]+=r[4];goto L8c63d008; /* add r4,r2 */
L8c63d008: *stop=0x8c63d008u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d00a; /* mov.l @(r0,r15),r3 */
L8c63d00a: *stop=0x8c63d00au;if(!valid_access(r[2],2u))return -3;r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c63d00c; /* mov.w @r2,r1 */
L8c63d00c: *stop=0x8c63d00cu;r[0]=sx(m,bus,&io_status,0x8c63d144u,2);if(io_status)return io_status;goto L8c63d00e; /* mov.w 0x8c63d144,r0 */
L8c63d00e: *stop=0x8c63d00eu;r[3]+=4u;goto L8c63d010; /* add #0x4,r3 */
L8c63d010: *stop=0x8c63d010u;if(!valid_access(r[3],2u))return -3;wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c63d012; /* mov.w r1,@r3 */
L8c63d012: *stop=0x8c63d012u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d014; /* mov.l @(r0,r15),r3 */
L8c63d014: *stop=0x8c63d014u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c63d016; /* mov.l @(0x8,r13),r0 */
L8c63d016: *stop=0x8c63d016u;r[3]+=4u;goto L8c63d018; /* add #0x4,r3 */
L8c63d018: *stop=0x8c63d018u;r[0]+=r[4];goto L8c63d01a; /* add r4,r0 */
L8c63d01a: *stop=0x8c63d01au;if(!valid_access(r[0]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c63d01c; /* mov.w @(0x2,r0),r0 */
L8c63d01c: *stop=0x8c63d01cu;if(!valid_access(r[3]+2u,2u))return -3;wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c63d01e; /* mov.w r0,@(0x2,r3) */
L8c63d01e: *stop=0x8c63d01eu;r[0]=sx(m,bus,&io_status,0x8c63d146u,2);if(io_status)return io_status;goto L8c63d020; /* mov.w 0x8c63d146,r0 */
L8c63d020: *stop=0x8c63d020u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d022; /* mov.l @(0x4,r15),r2 */
L8c63d022: *stop=0x8c63d022u;r[6]=r[11];goto L8c63d024; /* mov r11,r6 */
L8c63d024: *stop=0x8c63d024u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c63d026; /* mov.l @(0x4,r13),r1 */
L8c63d026: *stop=0x8c63d026u;r[2]<<=2;goto L8c63d028; /* shll2 r2 */
L8c63d028: *stop=0x8c63d028u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d02a; /* mov.l @(r0,r15),r3 */
L8c63d02a: *stop=0x8c63d02au;r[2]+=r[1];goto L8c63d02c; /* add r1,r2 */
L8c63d02c: *stop=0x8c63d02cu;r[5]=sx(m,bus,&io_status,0x8c63d148u,2);if(io_status)return io_status;goto L8c63d02e; /* mov.w 0x8c63d148,r5 */
L8c63d02e: *stop=0x8c63d02eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c63d030; /* mov.l @r2,r0 */
L8c63d030: *stop=0x8c63d030u;r[5]+=r[15];goto L8c63d032; /* add r15,r5 */
L8c63d032: *stop=0x8c63d032u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c63d034; /* mov.l r0,@r3 */
L8c63d034: *stop=0x8c63d034u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c63d036; /* mov.l @r15,r4 */
L8c63d036: *stop=0x8c63d036u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d038; /* mov.l @(0x4,r13),r2 */
L8c63d038: *stop=0x8c63d038u;r[0]=sx(m,bus,&io_status,0x8c63d146u,2);if(io_status)return io_status;goto L8c63d03a; /* mov.w 0x8c63d146,r0 */
L8c63d03a: *stop=0x8c63d03au;r[4]<<=2;goto L8c63d03c; /* shll2 r4 */
L8c63d03c: *stop=0x8c63d03cu;r[4]+=r[2];goto L8c63d03e; /* add r2,r4 */
L8c63d03e: *stop=0x8c63d03eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d040; /* mov.l @(r0,r15),r3 */
L8c63d040: *stop=0x8c63d040u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c63d042; /* mov.l @r4,r1 */
L8c63d042: *stop=0x8c63d042u;r[3]+=4u;goto L8c63d044; /* add #0x4,r3 */
L8c63d044: *stop=0x8c63d044u;r[4]=sx(m,bus,&io_status,0x8c63d142u,2);if(io_status)return io_status;goto L8c63d046; /* mov.w 0x8c63d142,r4 */
L8c63d046: *stop=0x8c63d046u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c63d048; /* mov.l r1,@r3 */
L8c63d048: *stop=0x8c63d048u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d158u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d04a; /* mov.l 0x8c63d158,r3 */
L8c63d04a: target=r[3];*pr=0x8c63d04eu;*stop=0x8c63d04cu;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d04e; /* jsr @r3 */
L8c63d04e: *stop=0x8c63d04eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c63d050; /* tst r0,r0 */
L8c63d050: if(!(*sr&1u))goto L8c63d056;goto L8c63d052; /* bf 0x8c63d056 */
L8c63d052: *stop=0x8c63d054u;goto L8c63d1dc; /* bra 0x8c63d1dc */
L8c63d056: *stop=0x8c63d056u;r[0]=sx(m,bus,&io_status,0x8c63d148u,2);if(io_status)return io_status;goto L8c63d058; /* mov.w 0x8c63d148,r0 */
L8c63d058: *stop=0x8c63d058u;fcopy(f,4u,14u);goto L8c63d05a; /* fmov fr14,fr4 */
L8c63d05a: *stop=0x8c63d05au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c63d05c; /* mov.l @(r0,r15),r5 */
L8c63d05c: *stop=0x8c63d05cu;r[0]=8u;goto L8c63d05e; /* mov #0x8,r0 */
L8c63d05e: *stop=0x8c63d05eu;fcopy(f,5u,14u);goto L8c63d060; /* fmov fr14,fr5 */
L8c63d060: *stop=0x8c63d060u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c63d062; /* fmov.s @(r0,r5),fr3 */
L8c63d062: *stop=0x8c63d062u;r[4]=r[5];goto L8c63d064; /* mov r5,r4 */
L8c63d064: *stop=0x8c63d064u;r[4]+=12u;goto L8c63d066; /* add #0xc,r4 */
L8c63d066: *stop=0x8c63d066u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63d068; /* fdiv fr3,fr4 */
L8c63d068: *stop=0x8c63d068u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c63d06a; /* fmov.s @(r0,r4),fr3 */
L8c63d06a: *stop=0x8c63d06au;r[0]=12u;goto L8c63d06c; /* mov #0xc,r0 */
L8c63d06c: *stop=0x8c63d06cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c63d06e; /* fdiv fr3,fr5 */
L8c63d06e: *stop=0x8c63d06eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c63d070; /* fmov.s @r5,fr3 */
L8c63d070: *stop=0x8c63d070u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]+r[12]);if(io_status)return io_status;goto L8c63d072; /* fmov.s @(r0,r12),fr7 */
L8c63d072: *stop=0x8c63d072u;r[0]=4u;goto L8c63d074; /* mov #0x4,r0 */
L8c63d074: *stop=0x8c63d074u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d076; /* fmul fr15,fr3 */
L8c63d076: *stop=0x8c63d076u;fcopy(f,6u,7u);goto L8c63d078; /* fmov fr7,fr6 */
L8c63d078: *stop=0x8c63d078u;fcopy(f,0u,4u);goto L8c63d07a; /* fmov fr4,fr0 */
L8c63d07a: *stop=0x8c63d07au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[6],&f->fpscr,&fr[6]);if(status)return status;}goto L8c63d07c; /* fmac fr0,fr3,fr6 */
L8c63d07c: *stop=0x8c63d07cu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c63d07e; /* fmov.s @r4,fr3 */
L8c63d07e: *stop=0x8c63d07eu;fcopy(f,0u,5u);goto L8c63d080; /* fmov fr5,fr0 */
L8c63d080: *stop=0x8c63d080u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d082; /* fmul fr15,fr3 */
L8c63d082: *stop=0x8c63d082u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[7],&f->fpscr,&fr[7]);if(status)return status;}goto L8c63d084; /* fmac fr0,fr3,fr7 */
L8c63d084: *stop=0x8c63d084u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c63d086; /* fmov.s @(r0,r5),fr3 */
L8c63d086: *stop=0x8c63d086u;r[0]=16u;goto L8c63d088; /* mov #0x10,r0 */
L8c63d088: *stop=0x8c63d088u;fcopy(f,0u,4u);goto L8c63d08a; /* fmov fr4,fr0 */
L8c63d08a: *stop=0x8c63d08au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[13],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d08c; /* fmul fr13,fr3 */
L8c63d08c: *stop=0x8c63d08cu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]+r[12]);if(io_status)return io_status;goto L8c63d08e; /* fmov.s @(r0,r12),fr9 */
L8c63d08e: *stop=0x8c63d08eu;r[0]=4u;goto L8c63d090; /* mov #0x4,r0 */
L8c63d090: *stop=0x8c63d090u;fcopy(f,8u,9u);goto L8c63d092; /* fmov fr9,fr8 */
L8c63d092: *stop=0x8c63d092u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[8],&f->fpscr,&fr[8]);if(status)return status;}goto L8c63d094; /* fmac fr0,fr3,fr8 */
L8c63d094: *stop=0x8c63d094u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c63d096; /* fmov.s @(r0,r4),fr3 */
L8c63d096: *stop=0x8c63d096u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[13],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d098; /* fmul fr13,fr3 */
L8c63d098: *stop=0x8c63d098u;fcopy(f,0u,5u);goto L8c63d09a; /* fmov fr5,fr0 */
L8c63d09a: *stop=0x8c63d09au;fcopy(f,10u,8u);goto L8c63d09c; /* fmov fr8,fr10 */
L8c63d09c: *stop=0x8c63d09cu;r[0]=36u;goto L8c63d09e; /* mov #0x24,r0 */
L8c63d09e: *stop=0x8c63d09eu;fcopy(f,11u,6u);goto L8c63d0a0; /* fmov fr6,fr11 */
L8c63d0a0: *stop=0x8c63d0a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[7],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c63d0a2; /* fsub fr7,fr11 */
L8c63d0a2: *stop=0x8c63d0a2u;fcopy(f,2u,14u);goto L8c63d0a4; /* fmov fr14,fr2 */
L8c63d0a4: *stop=0x8c63d0a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[9],&f->fpscr,&fr[9]);if(status)return status;}goto L8c63d0a6; /* fmac fr0,fr3,fr9 */
L8c63d0a6: *stop=0x8c63d0a6u;fcopy(f,0u,11u);goto L8c63d0a8; /* fmov fr11,fr0 */
L8c63d0a8: *stop=0x8c63d0a8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[10],&fr[9],&f->fpscr,&fr[10],1);if(status)return status;}goto L8c63d0aa; /* fsub fr9,fr10 */
L8c63d0aa: *stop=0x8c63d0aau;fcopy(f,3u,10u);goto L8c63d0ac; /* fmov fr10,fr3 */
L8c63d0ac: *stop=0x8c63d0acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[10],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d0ae; /* fmul fr10,fr3 */
L8c63d0ae: *stop=0x8c63d0aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[11],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d0b0; /* fmac fr0,fr11,fr3 */
L8c63d0b0: *stop=0x8c63d0b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d0b2; /* fsqrt fr3 */
L8c63d0b2: *stop=0x8c63d0b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63d0b4; /* fdiv fr3,fr2 */
L8c63d0b4: *stop=0x8c63d0b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[2],&f->fpscr,&fr[10]);if(status)return status;}goto L8c63d0b6; /* fmul fr2,fr10 */
L8c63d0b6: *stop=0x8c63d0b6u;fcopy(f,3u,2u);goto L8c63d0b8; /* fmov fr2,fr3 */
L8c63d0b8: *stop=0x8c63d0b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[3],&f->fpscr,&fr[11]);if(status)return status;}goto L8c63d0ba; /* fmul fr3,fr11 */
L8c63d0ba: *stop=0x8c63d0bau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d0bc; /* fmov fr2,@(r0,r15) */
L8c63d0bc: *stop=0x8c63d0bcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c63d0be; /* fldi1 fr2 */
L8c63d0be: *stop=0x8c63d0beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[2],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c63d0c0; /* fadd fr2,fr2 */
L8c63d0c0: *stop=0x8c63d0c0u;r[0]=56u;goto L8c63d0c2; /* mov #0x38,r0 */
L8c63d0c2: *stop=0x8c63d0c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[12],&f->fpscr,&fr[10]);if(status)return status;}goto L8c63d0c4; /* fmul fr12,fr10 */
L8c63d0c4: *stop=0x8c63d0c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[12],&f->fpscr,&fr[11]);if(status)return status;}goto L8c63d0c6; /* fmul fr12,fr11 */
L8c63d0c6: *stop=0x8c63d0c6u;fcopy(f,1u,10u);goto L8c63d0c8; /* fmov fr10,fr1 */
L8c63d0c8: *stop=0x8c63d0c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63d0ca; /* fmul fr2,fr1 */
L8c63d0ca: *stop=0x8c63d0cau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][11]^=0x80000000u;goto L8c63d0cc; /* fneg fr11 */
L8c63d0cc: *stop=0x8c63d0ccu;fcopy(f,3u,11u);goto L8c63d0ce; /* fmov fr11,fr3 */
L8c63d0ce: *stop=0x8c63d0ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d0d0; /* fmul fr2,fr3 */
L8c63d0d0: *stop=0x8c63d0d0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d0d2; /* fmov fr1,@(r0,r15) */
L8c63d0d2: *stop=0x8c63d0d2u;r[0]=68u;goto L8c63d0d4; /* mov #0x44,r0 */
L8c63d0d4: *stop=0x8c63d0d4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d0d6; /* fmov fr3,@(r0,r15) */
L8c63d0d6: *stop=0x8c63d0d6u;r[0]=sx(m,bus,&io_status,0x8c63d14au,2);if(io_status)return io_status;goto L8c63d0d8; /* mov.w 0x8c63d14a,r0 */
L8c63d0d8: *stop=0x8c63d0d8u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c63d0da; /* mov.l @(r0,r15),r5 */
L8c63d0da: *stop=0x8c63d0dau;r[0]=20u;goto L8c63d0dc; /* mov #0x14,r0 */
L8c63d0dc: *stop=0x8c63d0dcu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d0de; /* fmov.s @(r0,r15),fr2 */
L8c63d0de: *stop=0x8c63d0deu;r[0]=60u;goto L8c63d0e0; /* mov #0x3c,r0 */
L8c63d0e0: *stop=0x8c63d0e0u;if(!valid_access(r[5],2u))return -3;r[3]=sx(m,bus,&io_status,r[5],2);if(io_status)return io_status;goto L8c63d0e2; /* mov.w @r5,r3 */
L8c63d0e2: *stop=0x8c63d0e2u;r[4]=r[5];goto L8c63d0e4; /* mov r5,r4 */
L8c63d0e4: *stop=0x8c63d0e4u;r[4]+=4u;goto L8c63d0e6; /* add #0x4,r4 */
L8c63d0e6: *stop=0x8c63d0e6u;*fpul=r[3];goto L8c63d0e8; /* lds r3,FPUL */
L8c63d0e8: *stop=0x8c63d0e8u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c63d0ea; /* float FPUL,fr3 */
L8c63d0ea: *stop=0x8c63d0eau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d0ec; /* fmul fr2,fr3 */
L8c63d0ec: *stop=0x8c63d0ecu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d0ee; /* fmov fr3,@(r0,r15) */
L8c63d0ee: *stop=0x8c63d0eeu;if(!valid_access(r[5]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x2u,2);if(io_status)return io_status;goto L8c63d0f0; /* mov.w @(0x2,r5),r0 */
L8c63d0f0: *stop=0x8c63d0f0u;r[3]=r[0];goto L8c63d0f2; /* mov r0,r3 */
L8c63d0f2: *stop=0x8c63d0f2u;*fpul=r[3];goto L8c63d0f4; /* lds r3,FPUL */
L8c63d0f4: *stop=0x8c63d0f4u;r[0]=20u;goto L8c63d0f6; /* mov #0x14,r0 */
L8c63d0f6: *stop=0x8c63d0f6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d0f8; /* fmov.s @(r0,r15),fr2 */
L8c63d0f8: *stop=0x8c63d0f8u;r[0]=64u;goto L8c63d0fa; /* mov #0x40,r0 */
L8c63d0fa: *stop=0x8c63d0fau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c63d0fc; /* float FPUL,fr3 */
L8c63d0fc: *stop=0x8c63d0fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d0fe; /* fmul fr2,fr3 */
L8c63d0fe: *stop=0x8c63d0feu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d100; /* fmov fr3,@(r0,r15) */
L8c63d100: *stop=0x8c63d100u;r[0]=20u;goto L8c63d102; /* mov #0x14,r0 */
L8c63d102: *stop=0x8c63d102u;if(!valid_access(r[4],2u))return -3;r[3]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c63d104; /* mov.w @r4,r3 */
L8c63d104: *stop=0x8c63d104u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d106; /* fmov.s @(r0,r15),fr2 */
L8c63d106: *stop=0x8c63d106u;r[0]=52u;goto L8c63d108; /* mov #0x34,r0 */
L8c63d108: *stop=0x8c63d108u;*fpul=r[3];goto L8c63d10a; /* lds r3,FPUL */
L8c63d10a: *stop=0x8c63d10au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c63d10c; /* float FPUL,fr3 */
L8c63d10c: *stop=0x8c63d10cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d10e; /* fmul fr2,fr3 */
L8c63d10e: *stop=0x8c63d10eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d110; /* fmov fr3,@(r0,r15) */
L8c63d110: *stop=0x8c63d110u;if(!valid_access(r[4]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c63d112; /* mov.w @(0x2,r4),r0 */
L8c63d112: *stop=0x8c63d112u;r[3]=r[0];goto L8c63d114; /* mov r0,r3 */
L8c63d114: *stop=0x8c63d114u;*fpul=r[3];goto L8c63d116; /* lds r3,FPUL */
L8c63d116: *stop=0x8c63d116u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c63d118; /* float FPUL,fr3 */
L8c63d118: *stop=0x8c63d118u;r[0]=20u;goto L8c63d11a; /* mov #0x14,r0 */
L8c63d11a: *stop=0x8c63d11au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d11c; /* fmov.s @(r0,r15),fr2 */
L8c63d11c: *stop=0x8c63d11cu;r[0]=48u;goto L8c63d11e; /* mov #0x30,r0 */
L8c63d11e: *stop=0x8c63d11eu;r[14]+=28u;goto L8c63d120; /* add #0x1c,r14 */
L8c63d120: *stop=0x8c63d120u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d122; /* fmul fr2,fr3 */
L8c63d122: *stop=0x8c63d122u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d124; /* fmov fr3,@(r0,r15) */
L8c63d124: *stop=0x8c63d124u;r[0]=sx(m,bus,&io_status,0x8c63d14cu,2);if(io_status)return io_status;goto L8c63d126; /* mov.w 0x8c63d14c,r0 */
L8c63d126: *stop=0x8c63d126u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c63d128; /* mov.l @(r0,r15),r5 */
L8c63d128: *stop=0x8c63d128u;r[0]=64u;goto L8c63d12a; /* mov #0x40,r0 */
L8c63d12a: *stop=0x8c63d12au;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[4]=loaded;}goto L8c63d12c; /* mov.l @r5,r4 */
L8c63d12c: *stop=0x8c63d12cu;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c63d12e; /* mov.l @(0x4,r5),r5 */
L8c63d12e: *stop=0x8c63d12eu;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c63d130; /* mov.l r4,@-r14 */
L8c63d130: *stop=0x8c63d130u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d132; /* fmov.s @(r0,r15),fr3 */
L8c63d132: *stop=0x8c63d132u;r[0]=60u;goto L8c63d134; /* mov #0x3c,r0 */
L8c63d134: *stop=0x8c63d134u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d136; /* fmov.s fr3,@-r14 */
L8c63d136: *stop=0x8c63d136u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d138; /* fmov.s @(r0,r15),fr3 */
L8c63d138: *stop=0x8c63d138u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d13a; /* fmov.s fr3,@-r14 */
L8c63d13a: *stop=0x8c63d13au;fcopy(f,3u,8u);goto L8c63d13c; /* fmov fr8,fr3 */
L8c63d13c: *stop=0x8c63d13cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c63d13e; /* fmov.s fr4,@-r14 */
L8c63d13e: *stop=0x8c63d140u;goto L8c63d15c; /* bra 0x8c63d15c */
L8c63d15c: *stop=0x8c63d15cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[11],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c63d15e; /* fsub fr11,fr3 */
L8c63d15e: *stop=0x8c63d15eu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d160; /* fmov.s fr3,@-r14 */
L8c63d160: *stop=0x8c63d160u;fcopy(f,3u,6u);goto L8c63d162; /* fmov fr6,fr3 */
L8c63d162: *stop=0x8c63d162u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[10],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c63d164; /* fsub fr10,fr3 */
L8c63d164: *stop=0x8c63d164u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d166; /* fmov.s fr3,@-r14 */
L8c63d166: *stop=0x8c63d166u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[10],4);if(io_status)return io_status;goto L8c63d168; /* mov.l r10,@-r14 */
L8c63d168: *stop=0x8c63d168u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c63d16a; /* pref @r14 */
L8c63d16a: *stop=0x8c63d16au;r[0]=48u;goto L8c63d16c; /* mov #0x30,r0 */
L8c63d16c: *stop=0x8c63d16cu;r[14]+=60u;goto L8c63d16e; /* add #0x3c,r14 */
L8c63d16e: *stop=0x8c63d16eu;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[5],4);if(io_status)return io_status;goto L8c63d170; /* mov.l r5,@-r14 */
L8c63d170: *stop=0x8c63d170u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d172; /* fmov.s @(r0,r15),fr3 */
L8c63d172: *stop=0x8c63d172u;r[0]=52u;goto L8c63d174; /* mov #0x34,r0 */
L8c63d174: *stop=0x8c63d174u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d176; /* fmov.s fr3,@-r14 */
L8c63d176: *stop=0x8c63d176u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d178; /* fmov.s @(r0,r15),fr3 */
L8c63d178: *stop=0x8c63d178u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d17a; /* fmov.s fr3,@-r14 */
L8c63d17a: *stop=0x8c63d17au;fcopy(f,3u,9u);goto L8c63d17c; /* fmov fr9,fr3 */
L8c63d17c: *stop=0x8c63d17cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[11],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c63d17e; /* fsub fr11,fr3 */
L8c63d17e: *stop=0x8c63d17eu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c63d180; /* fmov.s fr5,@-r14 */
L8c63d180: *stop=0x8c63d180u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d182; /* fmov.s fr3,@-r14 */
L8c63d182: *stop=0x8c63d182u;fcopy(f,3u,7u);goto L8c63d184; /* fmov fr7,fr3 */
L8c63d184: *stop=0x8c63d184u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[10],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c63d186; /* fsub fr10,fr3 */
L8c63d186: *stop=0x8c63d186u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d188; /* fmov.s fr3,@-r14 */
L8c63d188: *stop=0x8c63d188u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[10],4);if(io_status)return io_status;goto L8c63d18a; /* mov.l r10,@-r14 */
L8c63d18a: *stop=0x8c63d18au;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c63d18c; /* pref @r14 */
L8c63d18c: *stop=0x8c63d18cu;r[0]=64u;goto L8c63d18e; /* mov #0x40,r0 */
L8c63d18e: *stop=0x8c63d18eu;r[14]+=60u;goto L8c63d190; /* add #0x3c,r14 */
L8c63d190: *stop=0x8c63d190u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c63d192; /* mov.l r4,@-r14 */
L8c63d192: *stop=0x8c63d192u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d194; /* fmov.s @(r0,r15),fr3 */
L8c63d194: *stop=0x8c63d194u;r[0]=60u;goto L8c63d196; /* mov #0x3c,r0 */
L8c63d196: *stop=0x8c63d196u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d198; /* fmov.s fr3,@-r14 */
L8c63d198: *stop=0x8c63d198u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d19a; /* fmov.s @(r0,r15),fr3 */
L8c63d19a: *stop=0x8c63d19au;r[0]=68u;goto L8c63d19c; /* mov #0x44,r0 */
L8c63d19c: *stop=0x8c63d19cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d19e; /* fmov.s fr3,@-r14 */
L8c63d19e: *stop=0x8c63d19eu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c63d1a0; /* fmov.s fr4,@-r14 */
L8c63d1a0: *stop=0x8c63d1a0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d1a2; /* fmov.s @(r0,r15),fr3 */
L8c63d1a2: *stop=0x8c63d1a2u;r[0]=56u;goto L8c63d1a4; /* mov #0x38,r0 */
L8c63d1a4: *stop=0x8c63d1a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c63d1a6; /* fadd fr3,fr8 */
L8c63d1a6: *stop=0x8c63d1a6u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[14]);if(io_status)return io_status;goto L8c63d1a8; /* fmov.s fr8,@-r14 */
L8c63d1a8: *stop=0x8c63d1a8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d1aa; /* fmov.s @(r0,r15),fr3 */
L8c63d1aa: *stop=0x8c63d1aau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[3],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c63d1ac; /* fadd fr3,fr6 */
L8c63d1ac: *stop=0x8c63d1acu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c63d1ae; /* fmov.s fr6,@-r14 */
L8c63d1ae: *stop=0x8c63d1aeu;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[10],4);if(io_status)return io_status;goto L8c63d1b0; /* mov.l r10,@-r14 */
L8c63d1b0: *stop=0x8c63d1b0u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c63d1b2; /* pref @r14 */
L8c63d1b2: *stop=0x8c63d1b2u;r[0]=48u;goto L8c63d1b4; /* mov #0x30,r0 */
L8c63d1b4: *stop=0x8c63d1b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d228u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d1b6; /* mov.l 0x8c63d228,r3 */
L8c63d1b6: *stop=0x8c63d1b6u;r[14]+=60u;goto L8c63d1b8; /* add #0x3c,r14 */
L8c63d1b8: *stop=0x8c63d1b8u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[5],4);if(io_status)return io_status;goto L8c63d1ba; /* mov.l r5,@-r14 */
L8c63d1ba: *stop=0x8c63d1bau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d1bc; /* fmov.s @(r0,r15),fr3 */
L8c63d1bc: *stop=0x8c63d1bcu;r[0]=52u;goto L8c63d1be; /* mov #0x34,r0 */
L8c63d1be: *stop=0x8c63d1beu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d1c0; /* fmov.s fr3,@-r14 */
L8c63d1c0: *stop=0x8c63d1c0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d1c2; /* fmov.s @(r0,r15),fr3 */
L8c63d1c2: *stop=0x8c63d1c2u;r[0]=68u;goto L8c63d1c4; /* mov #0x44,r0 */
L8c63d1c4: *stop=0x8c63d1c4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d1c6; /* fmov.s fr3,@-r14 */
L8c63d1c6: *stop=0x8c63d1c6u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c63d1c8; /* fmov.s fr5,@-r14 */
L8c63d1c8: *stop=0x8c63d1c8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d1ca; /* fmov.s @(r0,r15),fr3 */
L8c63d1ca: *stop=0x8c63d1cau;r[0]=56u;goto L8c63d1cc; /* mov #0x38,r0 */
L8c63d1cc: *stop=0x8c63d1ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[3],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c63d1ce; /* fadd fr3,fr9 */
L8c63d1ce: *stop=0x8c63d1ceu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[14]);if(io_status)return io_status;goto L8c63d1d0; /* fmov.s fr9,@-r14 */
L8c63d1d0: *stop=0x8c63d1d0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d1d2; /* fmov.s @(r0,r15),fr3 */
L8c63d1d2: *stop=0x8c63d1d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[3],&f->fpscr,&fr[7],0);if(status)return status;}goto L8c63d1d4; /* fadd fr3,fr7 */
L8c63d1d4: *stop=0x8c63d1d4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[14]);if(io_status)return io_status;goto L8c63d1d6; /* fmov.s fr7,@-r14 */
L8c63d1d6: *stop=0x8c63d1d6u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c63d1d8; /* mov.l r3,@-r14 */
L8c63d1d8: *stop=0x8c63d1d8u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c63d1da; /* pref @r14 */
L8c63d1da: *stop=0x8c63d1dau;r[14]+=32u;goto L8c63d1dc; /* add #0x20,r14 */
L8c63d1dc: *stop=0x8c63d1dcu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d1de; /* mov.l @(0x8,r15),r2 */
L8c63d1de: *stop=0x8c63d1deu;r[2]+=1u;goto L8c63d1e0; /* add #0x1,r2 */
L8c63d1e0: *stop=0x8c63d1e0u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c63d1e2; /* mov.l r2,@(0x8,r15) */
L8c63d1e2: *stop=0x8c63d1e2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d1e4; /* mov.l @r15,r3 */
L8c63d1e4: *stop=0x8c63d1e4u;r[3]+=r[8];goto L8c63d1e6; /* add r8,r3 */
L8c63d1e6: *stop=0x8c63d1e6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c63d1e8; /* mov.l r3,@r15 */
L8c63d1e8: *stop=0x8c63d1e8u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c63d1ea; /* mov.l @(0x10,r15),r1 */
L8c63d1ea: *stop=0x8c63d1eau;r[1]+=r[8];goto L8c63d1ec; /* add r8,r1 */
L8c63d1ec: *stop=0x8c63d1ecu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c63d1ee; /* mov.l r1,@(0x10,r15) */
L8c63d1ee: *stop=0x8c63d1eeu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d1f0; /* mov.l @(0xc,r15),r3 */
L8c63d1f0: *stop=0x8c63d1f0u;r[3]+=r[8];goto L8c63d1f2; /* add r8,r3 */
L8c63d1f2: *stop=0x8c63d1f2u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c63d1f4; /* mov.l r3,@(0xc,r15) */
L8c63d1f4: *stop=0x8c63d1f4u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d1f6; /* mov.l @(0x8,r15),r2 */
L8c63d1f6: *stop=0x8c63d1f6u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[9]);goto L8c63d1f8; /* cmp/ge r9,r2 */
L8c63d1f8: if((*sr&1u))goto L8c63d1fe;goto L8c63d1fa; /* bt 0x8c63d1fe */
L8c63d1fa: *stop=0x8c63d1fcu;goto L8c63cf82; /* bra 0x8c63cf82 */
L8c63d1fe: *stop=0x8c63d200u;goto L8c63d3c2; /* bra 0x8c63d3c2 */
L8c63d202: *stop=0x8c63d202u;r[5]=r[4];goto L8c63d204; /* mov r4,r5 */
L8c63d204: *stop=0x8c63d204u;*sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c63d206; /* cmp/pl r9 */
L8c63d206: *stop=0x8c63d206u;*macl=r[8]*r[5];goto L8c63d208; /* mul.l r8,r5 */
L8c63d208: *stop=0x8c63d208u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c63d20a; /* mov.l r4,@(0x8,r15) */
L8c63d20a: *stop=0x8c63d20au;r[5]=*macl;goto L8c63d20c; /* sts MACL,r5 */
L8c63d20c: *stop=0x8c63d20cu;r[6]=r[5];goto L8c63d20e; /* mov r5,r6 */
L8c63d20e: *stop=0x8c63d20eu;r[6]+=1u;goto L8c63d210; /* add #0x1,r6 */
L8c63d210: *stop=0x8c63d210u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c63d212; /* mov.l r6,@r15 */
L8c63d212: *stop=0x8c63d212u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[6],4);if(io_status)return io_status;goto L8c63d214; /* mov.l r6,@(0x10,r15) */
L8c63d214: *stop=0x8c63d214u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[5],4);if(io_status)return io_status;goto L8c63d216; /* mov.l r5,@(0xc,r15) */
L8c63d216: *stop=0x8c63d216u;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c63d218; /* mov.l r3,@(0x18,r15) */
L8c63d218: if((*sr&1u))goto L8c63d21e;goto L8c63d21a; /* bt 0x8c63d21e */
L8c63d21a: *stop=0x8c63d21cu;goto L8c63d3c2; /* bra 0x8c63d3c2 */
L8c63d21e: *stop=0x8c63d21eu;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d220; /* mov.l @(0x18,r15),r3 */
L8c63d220: *stop=0x8c63d220u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c63d222; /* tst r3,r3 */
L8c63d222: if((*sr&1u))goto L8c63d22c;goto L8c63d224; /* bt 0x8c63d22c */
L8c63d224: *stop=0x8c63d226u;r[2]=0u;goto L8c63d22e; /* bra 0x8c63d22e */
L8c63d22c: *stop=0x8c63d22cu;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d22e; /* mov.l @(0xc,r15),r2 */
L8c63d22e: *stop=0x8c63d22eu;r[5]=r[2];goto L8c63d230; /* mov r2,r5 */
L8c63d230: *stop=0x8c63d230u;r[3]=r[2];goto L8c63d232; /* mov r2,r3 */
L8c63d232: *stop=0x8c63d232u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c63d234; /* shll r5 */
L8c63d234: *stop=0x8c63d234u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c63d236; /* mov.l r2,@(0x4,r15) */
L8c63d236: *stop=0x8c63d236u;r[0]=sx(m,bus,&io_status,0x8c63d3c6u,2);if(io_status)return io_status;goto L8c63d238; /* mov.w 0x8c63d3c6,r0 */
L8c63d238: *stop=0x8c63d238u;r[5]+=r[3];goto L8c63d23a; /* add r3,r5 */
L8c63d23a: *stop=0x8c63d23au;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d23c; /* mov.l @r13,r2 */
L8c63d23c: *stop=0x8c63d23cu;r[5]<<=2;goto L8c63d23e; /* shll2 r5 */
L8c63d23e: *stop=0x8c63d23eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d3d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d240; /* mov.l 0x8c63d3d0,r3 */
L8c63d240: *stop=0x8c63d240u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c63d242; /* mov.l @(r0,r15),r6 */
L8c63d242: *stop=0x8c63d242u;r[5]+=r[2];goto L8c63d244; /* add r2,r5 */
L8c63d244: target=r[3];*pr=0x8c63d248u;*stop=0x8c63d246u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d248; /* jsr @r3 */
L8c63d248: *stop=0x8c63d248u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d24a; /* mov.l @(0x10,r15),r2 */
L8c63d24a: *stop=0x8c63d24au;r[0]=sx(m,bus,&io_status,0x8c63d3c6u,2);if(io_status)return io_status;goto L8c63d24c; /* mov.w 0x8c63d3c6,r0 */
L8c63d24c: *stop=0x8c63d24cu;r[5]=r[2];goto L8c63d24e; /* mov r2,r5 */
L8c63d24e: *stop=0x8c63d24eu;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c63d250; /* shll r5 */
L8c63d250: *stop=0x8c63d250u;r[3]=r[2];goto L8c63d252; /* mov r2,r3 */
L8c63d252: *stop=0x8c63d252u;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c63d254; /* mov.l r2,@(0x1c,r15) */
L8c63d254: *stop=0x8c63d254u;r[5]+=r[3];goto L8c63d256; /* add r3,r5 */
L8c63d256: *stop=0x8c63d256u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d258; /* mov.l @r13,r2 */
L8c63d258: *stop=0x8c63d258u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c63d25a; /* mov.l @(r0,r15),r6 */
L8c63d25a: *stop=0x8c63d25au;r[5]<<=2;goto L8c63d25c; /* shll2 r5 */
L8c63d25c: *stop=0x8c63d25cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d3d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d25e; /* mov.l 0x8c63d3d0,r3 */
L8c63d25e: *stop=0x8c63d25eu;r[5]+=r[2];goto L8c63d260; /* add r2,r5 */
L8c63d260: *stop=0x8c63d260u;r[6]+=12u;goto L8c63d262; /* add #0xc,r6 */
L8c63d262: target=r[3];*pr=0x8c63d266u;*stop=0x8c63d264u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d266; /* jsr @r3 */
L8c63d266: *stop=0x8c63d266u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d3d4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d268; /* mov.l 0x8c63d3d4,r2 */
L8c63d268: *stop=0x8c63d268u;r[5]=2u;goto L8c63d26a; /* mov #0x2,r5 */
L8c63d26a: *stop=0x8c63d26au;r[4]=sx(m,bus,&io_status,0x8c63d3c6u,2);if(io_status)return io_status;goto L8c63d26c; /* mov.w 0x8c63d3c6,r4 */
L8c63d26c: target=r[2];*pr=0x8c63d270u;*stop=0x8c63d26eu;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d270; /* jsr @r2 */
L8c63d270: *stop=0x8c63d270u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c63d272; /* tst r0,r0 */
L8c63d272: if(!(*sr&1u))goto L8c63d278;goto L8c63d274; /* bf 0x8c63d278 */
L8c63d274: *stop=0x8c63d276u;goto L8c63d3a0; /* bra 0x8c63d3a0 */
L8c63d278: *stop=0x8c63d278u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d27a; /* mov.l @(0x4,r15),r3 */
L8c63d27a: *stop=0x8c63d27au;r[6]=r[11];goto L8c63d27c; /* mov r11,r6 */
L8c63d27c: *stop=0x8c63d27cu;r[0]=sx(m,bus,&io_status,0x8c63d3c8u,2);if(io_status)return io_status;goto L8c63d27e; /* mov.w 0x8c63d3c8,r0 */
L8c63d27e: *stop=0x8c63d27eu;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c63d280; /* mov.l @(0x4,r13),r1 */
L8c63d280: *stop=0x8c63d280u;r[3]<<=2;goto L8c63d282; /* shll2 r3 */
L8c63d282: *stop=0x8c63d282u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d284; /* mov.l @(r0,r15),r2 */
L8c63d284: *stop=0x8c63d284u;r[3]+=r[1];goto L8c63d286; /* add r1,r3 */
L8c63d286: *stop=0x8c63d286u;r[5]=sx(m,bus,&io_status,0x8c63d3cau,2);if(io_status)return io_status;goto L8c63d288; /* mov.w 0x8c63d3ca,r5 */
L8c63d288: *stop=0x8c63d288u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c63d28a; /* mov.l @r3,r0 */
L8c63d28a: *stop=0x8c63d28au;r[5]+=r[15];goto L8c63d28c; /* add r15,r5 */
L8c63d28c: *stop=0x8c63d28cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c63d28e; /* mov.l r0,@r2 */
L8c63d28e: *stop=0x8c63d28eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c63d290; /* mov.l @r15,r4 */
L8c63d290: *stop=0x8c63d290u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d292; /* mov.l @(0x4,r13),r2 */
L8c63d292: *stop=0x8c63d292u;r[0]=sx(m,bus,&io_status,0x8c63d3c8u,2);if(io_status)return io_status;goto L8c63d294; /* mov.w 0x8c63d3c8,r0 */
L8c63d294: *stop=0x8c63d294u;r[4]<<=2;goto L8c63d296; /* shll2 r4 */
L8c63d296: *stop=0x8c63d296u;r[4]+=r[2];goto L8c63d298; /* add r2,r4 */
L8c63d298: *stop=0x8c63d298u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d29a; /* mov.l @(r0,r15),r3 */
L8c63d29a: *stop=0x8c63d29au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c63d29c; /* mov.l @r4,r1 */
L8c63d29c: *stop=0x8c63d29cu;r[3]+=4u;goto L8c63d29e; /* add #0x4,r3 */
L8c63d29e: *stop=0x8c63d29eu;r[4]=sx(m,bus,&io_status,0x8c63d3c6u,2);if(io_status)return io_status;goto L8c63d2a0; /* mov.w 0x8c63d3c6,r4 */
L8c63d2a0: *stop=0x8c63d2a0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c63d2a2; /* mov.l r1,@r3 */
L8c63d2a2: *stop=0x8c63d2a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d3d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d2a4; /* mov.l 0x8c63d3d8,r3 */
L8c63d2a4: target=r[3];*pr=0x8c63d2a8u;*stop=0x8c63d2a6u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d2a8; /* jsr @r3 */
L8c63d2a8: *stop=0x8c63d2a8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c63d2aa; /* tst r0,r0 */
L8c63d2aa: if((*sr&1u))goto L8c63d3a0;goto L8c63d2ac; /* bt 0x8c63d3a0 */
L8c63d2ac: *stop=0x8c63d2acu;r[0]=sx(m,bus,&io_status,0x8c63d3cau,2);if(io_status)return io_status;goto L8c63d2ae; /* mov.w 0x8c63d3ca,r0 */
L8c63d2ae: *stop=0x8c63d2aeu;fcopy(f,4u,14u);goto L8c63d2b0; /* fmov fr14,fr4 */
L8c63d2b0: *stop=0x8c63d2b0u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c63d2b2; /* mov.l @(r0,r15),r5 */
L8c63d2b2: *stop=0x8c63d2b2u;r[0]=8u;goto L8c63d2b4; /* mov #0x8,r0 */
L8c63d2b4: *stop=0x8c63d2b4u;fcopy(f,5u,14u);goto L8c63d2b6; /* fmov fr14,fr5 */
L8c63d2b6: *stop=0x8c63d2b6u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c63d2b8; /* fmov.s @(r0,r5),fr3 */
L8c63d2b8: *stop=0x8c63d2b8u;r[4]=r[5];goto L8c63d2ba; /* mov r5,r4 */
L8c63d2ba: *stop=0x8c63d2bau;r[4]+=12u;goto L8c63d2bc; /* add #0xc,r4 */
L8c63d2bc: *stop=0x8c63d2bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63d2be; /* fdiv fr3,fr4 */
L8c63d2be: *stop=0x8c63d2beu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c63d2c0; /* fmov.s @(r0,r4),fr3 */
L8c63d2c0: *stop=0x8c63d2c0u;r[0]=12u;goto L8c63d2c2; /* mov #0xc,r0 */
L8c63d2c2: *stop=0x8c63d2c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c63d2c4; /* fdiv fr3,fr5 */
L8c63d2c4: *stop=0x8c63d2c4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c63d2c6; /* fmov.s @r5,fr3 */
L8c63d2c6: *stop=0x8c63d2c6u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]+r[12]);if(io_status)return io_status;goto L8c63d2c8; /* fmov.s @(r0,r12),fr7 */
L8c63d2c8: *stop=0x8c63d2c8u;r[0]=4u;goto L8c63d2ca; /* mov #0x4,r0 */
L8c63d2ca: *stop=0x8c63d2cau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d2cc; /* fmul fr15,fr3 */
L8c63d2cc: *stop=0x8c63d2ccu;fcopy(f,6u,7u);goto L8c63d2ce; /* fmov fr7,fr6 */
L8c63d2ce: *stop=0x8c63d2ceu;fcopy(f,0u,4u);goto L8c63d2d0; /* fmov fr4,fr0 */
L8c63d2d0: *stop=0x8c63d2d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[6],&f->fpscr,&fr[6]);if(status)return status;}goto L8c63d2d2; /* fmac fr0,fr3,fr6 */
L8c63d2d2: *stop=0x8c63d2d2u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c63d2d4; /* fmov.s @r4,fr3 */
L8c63d2d4: *stop=0x8c63d2d4u;fcopy(f,0u,5u);goto L8c63d2d6; /* fmov fr5,fr0 */
L8c63d2d6: *stop=0x8c63d2d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d2d8; /* fmul fr15,fr3 */
L8c63d2d8: *stop=0x8c63d2d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[7],&f->fpscr,&fr[7]);if(status)return status;}goto L8c63d2da; /* fmac fr0,fr3,fr7 */
L8c63d2da: *stop=0x8c63d2dau;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c63d2dc; /* fmov.s @(r0,r5),fr3 */
L8c63d2dc: *stop=0x8c63d2dcu;r[0]=16u;goto L8c63d2de; /* mov #0x10,r0 */
L8c63d2de: *stop=0x8c63d2deu;fcopy(f,0u,4u);goto L8c63d2e0; /* fmov fr4,fr0 */
L8c63d2e0: *stop=0x8c63d2e0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[13],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d2e2; /* fmul fr13,fr3 */
L8c63d2e2: *stop=0x8c63d2e2u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]+r[12]);if(io_status)return io_status;goto L8c63d2e4; /* fmov.s @(r0,r12),fr9 */
L8c63d2e4: *stop=0x8c63d2e4u;r[0]=4u;goto L8c63d2e6; /* mov #0x4,r0 */
L8c63d2e6: *stop=0x8c63d2e6u;fcopy(f,8u,9u);goto L8c63d2e8; /* fmov fr9,fr8 */
L8c63d2e8: *stop=0x8c63d2e8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[8],&f->fpscr,&fr[8]);if(status)return status;}goto L8c63d2ea; /* fmac fr0,fr3,fr8 */
L8c63d2ea: *stop=0x8c63d2eau;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c63d2ec; /* fmov.s @(r0,r4),fr3 */
L8c63d2ec: *stop=0x8c63d2ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[13],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d2ee; /* fmul fr13,fr3 */
L8c63d2ee: *stop=0x8c63d2eeu;fcopy(f,0u,5u);goto L8c63d2f0; /* fmov fr5,fr0 */
L8c63d2f0: *stop=0x8c63d2f0u;fcopy(f,10u,8u);goto L8c63d2f2; /* fmov fr8,fr10 */
L8c63d2f2: *stop=0x8c63d2f2u;r[0]=32u;goto L8c63d2f4; /* mov #0x20,r0 */
L8c63d2f4: *stop=0x8c63d2f4u;fcopy(f,11u,6u);goto L8c63d2f6; /* fmov fr6,fr11 */
L8c63d2f6: *stop=0x8c63d2f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[7],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c63d2f8; /* fsub fr7,fr11 */
L8c63d2f8: *stop=0x8c63d2f8u;fcopy(f,2u,14u);goto L8c63d2fa; /* fmov fr14,fr2 */
L8c63d2fa: *stop=0x8c63d2fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[9],&f->fpscr,&fr[9]);if(status)return status;}goto L8c63d2fc; /* fmac fr0,fr3,fr9 */
L8c63d2fc: *stop=0x8c63d2fcu;fcopy(f,0u,11u);goto L8c63d2fe; /* fmov fr11,fr0 */
L8c63d2fe: *stop=0x8c63d2feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[10],&fr[9],&f->fpscr,&fr[10],1);if(status)return status;}goto L8c63d300; /* fsub fr9,fr10 */
L8c63d300: *stop=0x8c63d300u;fcopy(f,3u,10u);goto L8c63d302; /* fmov fr10,fr3 */
L8c63d302: *stop=0x8c63d302u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[10],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d304; /* fmul fr10,fr3 */
L8c63d304: *stop=0x8c63d304u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[11],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d306; /* fmac fr0,fr11,fr3 */
L8c63d306: *stop=0x8c63d306u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d308; /* fsqrt fr3 */
L8c63d308: *stop=0x8c63d308u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63d30a; /* fdiv fr3,fr2 */
L8c63d30a: *stop=0x8c63d30au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[2],&f->fpscr,&fr[10]);if(status)return status;}goto L8c63d30c; /* fmul fr2,fr10 */
L8c63d30c: *stop=0x8c63d30cu;fcopy(f,3u,2u);goto L8c63d30e; /* fmov fr2,fr3 */
L8c63d30e: *stop=0x8c63d30eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[3],&f->fpscr,&fr[11]);if(status)return status;}goto L8c63d310; /* fmul fr3,fr11 */
L8c63d310: *stop=0x8c63d310u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d312; /* fmov fr2,@(r0,r15) */
L8c63d312: *stop=0x8c63d312u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c63d314; /* fldi1 fr2 */
L8c63d314: *stop=0x8c63d314u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[2],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c63d316; /* fadd fr2,fr2 */
L8c63d316: *stop=0x8c63d316u;r[0]=44u;goto L8c63d318; /* mov #0x2c,r0 */
L8c63d318: *stop=0x8c63d318u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[12],&f->fpscr,&fr[10]);if(status)return status;}goto L8c63d31a; /* fmul fr12,fr10 */
L8c63d31a: *stop=0x8c63d31au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[12],&f->fpscr,&fr[11]);if(status)return status;}goto L8c63d31c; /* fmul fr12,fr11 */
L8c63d31c: *stop=0x8c63d31cu;fcopy(f,1u,10u);goto L8c63d31e; /* fmov fr10,fr1 */
L8c63d31e: *stop=0x8c63d31eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63d320; /* fmul fr2,fr1 */
L8c63d320: *stop=0x8c63d320u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][11]^=0x80000000u;goto L8c63d322; /* fneg fr11 */
L8c63d322: *stop=0x8c63d322u;fcopy(f,3u,11u);goto L8c63d324; /* fmov fr11,fr3 */
L8c63d324: *stop=0x8c63d324u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63d326; /* fmul fr2,fr3 */
L8c63d326: *stop=0x8c63d326u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d328; /* fmov fr1,@(r0,r15) */
L8c63d328: *stop=0x8c63d328u;r[0]=40u;goto L8c63d32a; /* mov #0x28,r0 */
L8c63d32a: *stop=0x8c63d32au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d32c; /* fmov fr3,@(r0,r15) */
L8c63d32c: *stop=0x8c63d32cu;r[0]=sx(m,bus,&io_status,0x8c63d3ccu,2);if(io_status)return io_status;goto L8c63d32e; /* mov.w 0x8c63d3cc,r0 */
L8c63d32e: *stop=0x8c63d32eu;r[14]+=28u;goto L8c63d330; /* add #0x1c,r14 */
L8c63d330: *stop=0x8c63d330u;fcopy(f,3u,8u);goto L8c63d332; /* fmov fr8,fr3 */
L8c63d332: *stop=0x8c63d332u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[11],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c63d334; /* fsub fr11,fr3 */
L8c63d334: *stop=0x8c63d334u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c63d336; /* mov.l @(r0,r15),r5 */
L8c63d336: *stop=0x8c63d336u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[4]=loaded;}goto L8c63d338; /* mov.l @r5,r4 */
L8c63d338: *stop=0x8c63d338u;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c63d33a; /* mov.l @(0x4,r5),r5 */
L8c63d33a: *stop=0x8c63d33au;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c63d33c; /* mov.l r4,@-r14 */
L8c63d33c: *stop=0x8c63d33cu;r[14]+=4294967288u;goto L8c63d33e; /* add #-0x8,r14 */
L8c63d33e: *stop=0x8c63d33eu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c63d340; /* fmov.s fr4,@-r14 */
L8c63d340: *stop=0x8c63d340u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d342; /* fmov.s fr3,@-r14 */
L8c63d342: *stop=0x8c63d342u;fcopy(f,3u,6u);goto L8c63d344; /* fmov fr6,fr3 */
L8c63d344: *stop=0x8c63d344u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[10],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c63d346; /* fsub fr10,fr3 */
L8c63d346: *stop=0x8c63d346u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d348; /* fmov.s fr3,@-r14 */
L8c63d348: *stop=0x8c63d348u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[10],4);if(io_status)return io_status;goto L8c63d34a; /* mov.l r10,@-r14 */
L8c63d34a: *stop=0x8c63d34au;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c63d34c; /* pref @r14 */
L8c63d34c: *stop=0x8c63d34cu;fcopy(f,3u,9u);goto L8c63d34e; /* fmov fr9,fr3 */
L8c63d34e: *stop=0x8c63d34eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[11],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c63d350; /* fsub fr11,fr3 */
L8c63d350: *stop=0x8c63d350u;r[14]+=60u;goto L8c63d352; /* add #0x3c,r14 */
L8c63d352: *stop=0x8c63d352u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[5],4);if(io_status)return io_status;goto L8c63d354; /* mov.l r5,@-r14 */
L8c63d354: *stop=0x8c63d354u;r[14]+=4294967288u;goto L8c63d356; /* add #-0x8,r14 */
L8c63d356: *stop=0x8c63d356u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c63d358; /* fmov.s fr5,@-r14 */
L8c63d358: *stop=0x8c63d358u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d35a; /* fmov.s fr3,@-r14 */
L8c63d35a: *stop=0x8c63d35au;fcopy(f,3u,7u);goto L8c63d35c; /* fmov fr7,fr3 */
L8c63d35c: *stop=0x8c63d35cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[10],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c63d35e; /* fsub fr10,fr3 */
L8c63d35e: *stop=0x8c63d35eu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c63d360; /* fmov.s fr3,@-r14 */
L8c63d360: *stop=0x8c63d360u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[10],4);if(io_status)return io_status;goto L8c63d362; /* mov.l r10,@-r14 */
L8c63d362: *stop=0x8c63d362u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c63d364; /* pref @r14 */
L8c63d364: *stop=0x8c63d364u;r[0]=40u;goto L8c63d366; /* mov #0x28,r0 */
L8c63d366: *stop=0x8c63d366u;r[14]+=60u;goto L8c63d368; /* add #0x3c,r14 */
L8c63d368: *stop=0x8c63d368u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c63d36a; /* mov.l r4,@-r14 */
L8c63d36a: *stop=0x8c63d36au;r[14]+=4294967288u;goto L8c63d36c; /* add #-0x8,r14 */
L8c63d36c: *stop=0x8c63d36cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c63d36e; /* fmov.s fr4,@-r14 */
L8c63d36e: *stop=0x8c63d36eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d370; /* fmov.s @(r0,r15),fr3 */
L8c63d370: *stop=0x8c63d370u;r[0]=44u;goto L8c63d372; /* mov #0x2c,r0 */
L8c63d372: *stop=0x8c63d372u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c63d374; /* fadd fr3,fr8 */
L8c63d374: *stop=0x8c63d374u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[14]);if(io_status)return io_status;goto L8c63d376; /* fmov.s fr8,@-r14 */
L8c63d376: *stop=0x8c63d376u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d378; /* fmov.s @(r0,r15),fr3 */
L8c63d378: *stop=0x8c63d378u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[3],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c63d37a; /* fadd fr3,fr6 */
L8c63d37a: *stop=0x8c63d37au;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c63d37c; /* fmov.s fr6,@-r14 */
L8c63d37c: *stop=0x8c63d37cu;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[10],4);if(io_status)return io_status;goto L8c63d37e; /* mov.l r10,@-r14 */
L8c63d37e: *stop=0x8c63d37eu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c63d380; /* pref @r14 */
L8c63d380: *stop=0x8c63d380u;r[0]=40u;goto L8c63d382; /* mov #0x28,r0 */
L8c63d382: *stop=0x8c63d382u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d3dcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d384; /* mov.l 0x8c63d3dc,r3 */
L8c63d384: *stop=0x8c63d384u;r[14]+=60u;goto L8c63d386; /* add #0x3c,r14 */
L8c63d386: *stop=0x8c63d386u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[5],4);if(io_status)return io_status;goto L8c63d388; /* mov.l r5,@-r14 */
L8c63d388: *stop=0x8c63d388u;r[14]+=4294967288u;goto L8c63d38a; /* add #-0x8,r14 */
L8c63d38a: *stop=0x8c63d38au;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c63d38c; /* fmov.s fr5,@-r14 */
L8c63d38c: *stop=0x8c63d38cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d38e; /* fmov.s @(r0,r15),fr3 */
L8c63d38e: *stop=0x8c63d38eu;r[0]=44u;goto L8c63d390; /* mov #0x2c,r0 */
L8c63d390: *stop=0x8c63d390u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[3],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c63d392; /* fadd fr3,fr9 */
L8c63d392: *stop=0x8c63d392u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[14]);if(io_status)return io_status;goto L8c63d394; /* fmov.s fr9,@-r14 */
L8c63d394: *stop=0x8c63d394u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c63d396; /* fmov.s @(r0,r15),fr3 */
L8c63d396: *stop=0x8c63d396u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[3],&f->fpscr,&fr[7],0);if(status)return status;}goto L8c63d398; /* fadd fr3,fr7 */
L8c63d398: *stop=0x8c63d398u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[14]);if(io_status)return io_status;goto L8c63d39a; /* fmov.s fr7,@-r14 */
L8c63d39a: *stop=0x8c63d39au;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c63d39c; /* mov.l r3,@-r14 */
L8c63d39c: *stop=0x8c63d39cu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c63d39e; /* pref @r14 */
L8c63d39e: *stop=0x8c63d39eu;r[14]+=32u;goto L8c63d3a0; /* add #0x20,r14 */
L8c63d3a0: *stop=0x8c63d3a0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d3a2; /* mov.l @(0x8,r15),r2 */
L8c63d3a2: *stop=0x8c63d3a2u;r[2]+=1u;goto L8c63d3a4; /* add #0x1,r2 */
L8c63d3a4: *stop=0x8c63d3a4u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c63d3a6; /* mov.l r2,@(0x8,r15) */
L8c63d3a6: *stop=0x8c63d3a6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d3a8; /* mov.l @r15,r3 */
L8c63d3a8: *stop=0x8c63d3a8u;r[3]+=r[8];goto L8c63d3aa; /* add r8,r3 */
L8c63d3aa: *stop=0x8c63d3aau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c63d3ac; /* mov.l r3,@r15 */
L8c63d3ac: *stop=0x8c63d3acu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c63d3ae; /* mov.l @(0x10,r15),r1 */
L8c63d3ae: *stop=0x8c63d3aeu;r[1]+=r[8];goto L8c63d3b0; /* add r8,r1 */
L8c63d3b0: *stop=0x8c63d3b0u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c63d3b2; /* mov.l r1,@(0x10,r15) */
L8c63d3b2: *stop=0x8c63d3b2u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d3b4; /* mov.l @(0xc,r15),r3 */
L8c63d3b4: *stop=0x8c63d3b4u;r[3]+=r[8];goto L8c63d3b6; /* add r8,r3 */
L8c63d3b6: *stop=0x8c63d3b6u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c63d3b8; /* mov.l r3,@(0xc,r15) */
L8c63d3b8: *stop=0x8c63d3b8u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d3ba; /* mov.l @(0x8,r15),r2 */
L8c63d3ba: *stop=0x8c63d3bau;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[9]);goto L8c63d3bc; /* cmp/ge r9,r2 */
L8c63d3bc: if((*sr&1u))goto L8c63d3c2;goto L8c63d3be; /* bt 0x8c63d3c2 */
L8c63d3be: *stop=0x8c63d3c0u;goto L8c63d21e; /* bra 0x8c63d21e */
L8c63d3c2: *stop=0x8c63d3c4u;r[4]=r[14];goto L8c63d3e0; /* bra 0x8c63d3e0 */
L8c63d3e0: *stop=0x8c63d3e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d40cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c63d3e2; /* mov.l 0x8c63d40c,r3 */
L8c63d3e2: target=r[3];*pr=0x8c63d3e6u;*stop=0x8c63d3e4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d3e6; /* jsr @r3 */
L8c63d3e6: *stop=0x8c63d3e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c63d410u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c63d3e8; /* mov.l 0x8c63d410,r2 */
L8c63d3e8: target=r[2];*pr=0x8c63d3ecu;*stop=0x8c63d3eau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63d3ec; /* jsr @r2 */
L8c63d3ec: *stop=0x8c63d3ecu;r[1]=sx(m,bus,&io_status,0x8c63d40au,2);if(io_status)return io_status;goto L8c63d3ee; /* mov.w 0x8c63d40a,r1 */
L8c63d3ee: *stop=0x8c63d3eeu;r[15]+=r[1];goto L8c63d3f0; /* add r1,r15 */
L8c63d3f0: *stop=0x8c63d3f0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c63d3f2; /* lds.l @r15+,PR */
L8c63d3f2: *stop=0x8c63d3f2u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63d3f4; /* fmov.s @r15+,fr12 */
L8c63d3f4: *stop=0x8c63d3f4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63d3f6; /* fmov.s @r15+,fr13 */
L8c63d3f6: *stop=0x8c63d3f6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63d3f8; /* fmov.s @r15+,fr14 */
L8c63d3f8: *stop=0x8c63d3f8u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63d3fa; /* fmov.s @r15+,fr15 */
L8c63d3fa: *stop=0x8c63d3fau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c63d3fc; /* mov.l @r15+,r8 */
L8c63d3fc: *stop=0x8c63d3fcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c63d3fe; /* mov.l @r15+,r9 */
L8c63d3fe: *stop=0x8c63d3feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c63d400; /* mov.l @r15+,r10 */
L8c63d400: *stop=0x8c63d400u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c63d402; /* mov.l @r15+,r11 */
L8c63d402: *stop=0x8c63d402u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c63d404; /* mov.l @r15+,r12 */
L8c63d404: *stop=0x8c63d404u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c63d406; /* mov.l @r15+,r13 */
L8c63d406: *stop=0x8c63d406u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c63d408u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_response_draw_8c63ce24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63ce96(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63ce96u);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63ceae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63ceaeu);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63ceb4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63ceb4u);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63cec4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63cec4u);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63ceca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63cecau);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63ceda(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63cedau);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63cee0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63cee0u);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63ceea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63ceeau);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63cef0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63cef0u);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63cfb0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63cfb0u);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63cfce(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63cfceu);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63cfd8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63cfd8u);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63d04e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d04eu);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63d248(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d248u);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63d266(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d266u);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63d270(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d270u);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63d2a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d2a8u);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63d3e6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d3e6u);}
SA_STAGE_EXPORT int sa_player_response_draw_resume_8c63d3ec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_response_draw_run_8c63ce24(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c63d3ecu);}
