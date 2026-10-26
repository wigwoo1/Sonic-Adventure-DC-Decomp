/* Original graphics memory accounting; explicit complete state.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
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
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x05000000u && (a&0x1fffffffu)<0x05800000u)return 1;
 if((n==1u||n==2u||n==4u)&&!(a&(n-1u))&&(a&0x1fffffffu)>=0x00800000u&&(a&0x1fffffffu)<=0x00a00000u-n)return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_graphics_handoff_8c608d88(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c608d88;
L8c608d88: *stop=0x8c608d88u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c608d8a; /* sts.l PR,@-r15 */
L8c608d8a: *stop=0x8c608d8au;r[15]+=4294967284u;goto L8c608d8c; /* add #-0xc,r15 */
L8c608d8c: *stop=0x8c608d8cu;r[0]=r[4];goto L8c608d8e; /* mov r4,r0 */
L8c608d8e: *stop=0x8c608d8eu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c608d90; /* cmp/eq #0x2,r0 */
L8c608d90: *stop=0x8c608d90u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c608d92; /* mov.l r4,@(0x8,r15) */
L8c608d92: if(!(*sr&1u))goto L8c608da0;goto L8c608d94; /* bf 0x8c608da0 */
L8c608d94: *stop=0x8c608d94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608e58u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608d96; /* mov.l 0x8c608e58,r2 */
L8c608d96: *stop=0x8c608d96u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c608d98; /* mov.l @r2,r0 */
L8c608d98: *stop=0x8c608d98u;r[15]+=12u;goto L8c608d9a; /* add #0xc,r15 */
L8c608d9a: *stop=0x8c608d9au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c608d9c; /* lds.l @r15+,PR */
L8c608d9c: *stop=0x8c608d9cu;if(*pr!=entry_pr)return -3;*stop=0x8c608d9eu;return 0; /* rts */
L8c608da0: *stop=0x8c608da0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c608e5cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c608da2; /* mov.l 0x8c608e5c,r2 */
L8c608da2: *stop=0x8c608da2u;r[5]=r[15];goto L8c608da4; /* mov r15,r5 */
L8c608da4: *stop=0x8c608da4u;r[5]+=4u;goto L8c608da6; /* add #0x4,r5 */
L8c608da6: target=r[2];*pr=0x8c608daau;*stop=0x8c608da8u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608daa; /* jsr @r2 */
L8c608daa: *stop=0x8c608daau;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c608dac; /* mov.l @(0x8,r15),r3 */
L8c608dac: *stop=0x8c608dacu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c608dae; /* tst r3,r3 */
L8c608dae: if(!(*sr&1u))goto L8c608dba;goto L8c608db0; /* bf 0x8c608dba */
L8c608db0: *stop=0x8c608db0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c608db2; /* mov.l @r15,r0 */
L8c608db2: *stop=0x8c608db2u;r[15]+=12u;goto L8c608db4; /* add #0xc,r15 */
L8c608db4: *stop=0x8c608db4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c608db6; /* lds.l @r15+,PR */
L8c608db6: *stop=0x8c608db6u;if(*pr!=entry_pr)return -3;*stop=0x8c608db8u;return 0; /* rts */
L8c608dba: *stop=0x8c608dbau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c608dbc; /* mov.l @(0x4,r15),r0 */
L8c608dbc: *stop=0x8c608dbcu;r[15]+=12u;goto L8c608dbe; /* add #0xc,r15 */
L8c608dbe: *stop=0x8c608dbeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c608dc0; /* lds.l @r15+,PR */
L8c608dc0: *stop=0x8c608dc0u;if(*pr!=entry_pr)return -3;*stop=0x8c608dc2u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_handoff_8c653dc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c653dc0;
L8c653dc0: *stop=0x8c653dc0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c653dc2; /* sts.l PR,@-r15 */
L8c653dc2: *stop=0x8c653dc2u;r[15]+=4294967272u;goto L8c653dc4; /* add #-0x18,r15 */
L8c653dc4: *stop=0x8c653dc4u;r[6]=r[15];goto L8c653dc6; /* mov r15,r6 */
L8c653dc6: *stop=0x8c653dc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c653ea0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c653dc8; /* mov.l 0x8c653ea0,r3 */
L8c653dc8: *stop=0x8c653dc8u;r[7]=r[15];goto L8c653dca; /* mov r15,r7 */
L8c653dca: *stop=0x8c653dcau;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[4],4);if(io_status)return io_status;goto L8c653dcc; /* mov.l r4,@(0x14,r15) */
L8c653dcc: *stop=0x8c653dccu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[5],4);if(io_status)return io_status;goto L8c653dce; /* mov.l r5,@(0x10,r15) */
L8c653dce: *stop=0x8c653dceu;r[5]=r[15];goto L8c653dd0; /* mov r15,r5 */
L8c653dd0: *stop=0x8c653dd0u;r[5]+=4u;goto L8c653dd2; /* add #0x4,r5 */
L8c653dd2: *stop=0x8c653dd2u;r[6]+=8u;goto L8c653dd4; /* add #0x8,r6 */
L8c653dd4: *stop=0x8c653dd4u;r[7]+=12u;goto L8c653dd6; /* add #0xc,r7 */
L8c653dd6: target=r[3];*pr=0x8c653ddau;*stop=0x8c653dd8u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c653dda; /* jsr @r3 */
L8c653dda: *stop=0x8c653ddau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c653ddc; /* mov.l @(0x4,r15),r3 */
L8c653ddc: *stop=0x8c653ddcu;r[4]=r[0];goto L8c653dde; /* mov r0,r4 */
L8c653dde: *stop=0x8c653ddeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c653de0; /* mov.l @r15,r1 */
L8c653de0: *stop=0x8c653de0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c653de2; /* mov.l @(0x8,r15),r0 */
L8c653de2: *stop=0x8c653de2u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c653de4; /* mov.l @(0x14,r15),r2 */
L8c653de4: *stop=0x8c653de4u;r[1]+=r[3];goto L8c653de6; /* add r3,r1 */
L8c653de6: *stop=0x8c653de6u;r[1]+=r[0];goto L8c653de8; /* add r0,r1 */
L8c653de8: *stop=0x8c653de8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c653dea; /* mov.l r1,@r2 */
L8c653dea: *stop=0x8c653deau;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c653dec; /* mov.l @(0xc,r15),r2 */
L8c653dec: *stop=0x8c653decu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c653dee; /* mov.l @(0x10,r15),r3 */
L8c653dee: *stop=0x8c653deeu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c653df0; /* mov.l r2,@r3 */
L8c653df0: *stop=0x8c653df0u;r[15]+=24u;goto L8c653df2; /* add #0x18,r15 */
L8c653df2: *stop=0x8c653df2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c653df4; /* lds.l @r15+,PR */
L8c653df4: *stop=0x8c653df4u;if(*pr!=entry_pr)return -3;*stop=0x8c653df6u;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_graphics_handoff_8c663a60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c663a60;
L8c663a60: *stop=0x8c663a60u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c663a62; /* mov.l r14,@-r15 */
L8c663a62: *stop=0x8c663a62u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c663a64; /* mov.l r13,@-r15 */
L8c663a64: *stop=0x8c663a64u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c663a66; /* mov.l r12,@-r15 */
L8c663a66: *stop=0x8c663a66u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c663a68; /* mov.l r11,@-r15 */
L8c663a68: *stop=0x8c663a68u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c663a6a; /* mov.l r10,@-r15 */
L8c663a6a: *stop=0x8c663a6au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c663a6c; /* mov.l r9,@-r15 */
L8c663a6c: *stop=0x8c663a6cu;r[15]+=4294967264u;goto L8c663a6e; /* add #-0x20,r15 */
L8c663a6e: *stop=0x8c663a6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c663b68u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c663a70; /* mov.l 0x8c663b68,r13 */
L8c663a70: *stop=0x8c663a70u;r[11]=0u;goto L8c663a72; /* mov #0x0,r11 */
L8c663a72: *stop=0x8c663a72u;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[11],4);if(io_status)return io_status;goto L8c663a74; /* mov.l r11,@(0x1c,r15) */
L8c663a74: *stop=0x8c663a74u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[11],4);if(io_status)return io_status;goto L8c663a76; /* mov.l r11,@(0x10,r15) */
L8c663a76: *stop=0x8c663a76u;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[11],4);if(io_status)return io_status;goto L8c663a78; /* mov.l r11,@(0x18,r15) */
L8c663a78: *stop=0x8c663a78u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[11],4);if(io_status)return io_status;goto L8c663a7a; /* mov.l r11,@(0x14,r15) */
L8c663a7a: *stop=0x8c663a7au;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[11],4);if(io_status)return io_status;goto L8c663a7c; /* mov.l r11,@(0x8,r15) */
L8c663a7c: *stop=0x8c663a7cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[11],4);if(io_status)return io_status;goto L8c663a7e; /* mov.l r11,@(0x4,r15) */
L8c663a7e: *stop=0x8c663a7eu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[11],4);if(io_status)return io_status;goto L8c663a80; /* mov.l r11,@(0xc,r15) */
L8c663a80: *stop=0x8c663a80u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c663a82; /* mov.l @(0x8,r13),r14 */
L8c663a82: *stop=0x8c663a82u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c663a84; /* tst r14,r14 */
L8c663a84: if((*sr&1u))goto L8c663aa0;goto L8c663a86; /* bt 0x8c663aa0 */
L8c663a86: *stop=0x8c663a86u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663a88; /* mov.l @(0x10,r14),r3 */
L8c663a88: *stop=0x8c663a88u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663a8a; /* mov.l @(0x4,r15),r2 */
L8c663a8a: *stop=0x8c663a8au;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[2]);goto L8c663a8c; /* cmp/gt r2,r3 */
L8c663a8c: if(!(*sr&1u))goto L8c663a92;goto L8c663a8e; /* bf 0x8c663a92 */
L8c663a8e: *stop=0x8c663a8eu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663a90; /* mov.l @(0x10,r14),r3 */
L8c663a90: *stop=0x8c663a90u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c663a92; /* mov.l r3,@(0x4,r15) */
L8c663a92: *stop=0x8c663a92u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663a94; /* mov.l @(0x14,r15),r2 */
L8c663a94: *stop=0x8c663a94u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663a96; /* mov.l @(0x10,r14),r3 */
L8c663a96: *stop=0x8c663a96u;r[2]+=r[3];goto L8c663a98; /* add r3,r2 */
L8c663a98: *stop=0x8c663a98u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[2],4);if(io_status)return io_status;goto L8c663a9a; /* mov.l r2,@(0x14,r15) */
L8c663a9a: *stop=0x8c663a9au;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c663a9c; /* mov.l @(0x8,r14),r14 */
L8c663a9c: *stop=0x8c663a9cu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c663a9e; /* tst r14,r14 */
L8c663a9e: if(!(*sr&1u))goto L8c663a86;goto L8c663aa0; /* bf 0x8c663a86 */
L8c663aa0: *stop=0x8c663aa0u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[14]=loaded;}goto L8c663aa2; /* mov.l @r13,r14 */
L8c663aa2: *stop=0x8c663aa2u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c663aa4; /* tst r14,r14 */
L8c663aa4: take=(*sr&1u);*stop=0x8c663aa6u;r[9]=4u;if(take)goto L8c663aca;goto L8c663aa8; /* bt/s 0x8c663aca */
L8c663aa8: *stop=0x8c663aa8u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[3]=loaded;}goto L8c663aaa; /* mov.w @r14,r3 */
L8c663aaa: *stop=0x8c663aaau;r[3]=r[3]&65535u;goto L8c663aac; /* extu.w r3,r3 */
L8c663aac: *stop=0x8c663aacu;*sr=(*sr&~1u)|!(r[9]&r[3]);goto L8c663aae; /* tst r9,r3 */
L8c663aae: if(!(*sr&1u))goto L8c663ac4;goto L8c663ab0; /* bf 0x8c663ac4 */
L8c663ab0: *stop=0x8c663ab0u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663ab2; /* mov.l @(0x10,r14),r2 */
L8c663ab2: *stop=0x8c663ab2u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c663ab4; /* mov.l @(0x4,r15),r1 */
L8c663ab4: *stop=0x8c663ab4u;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[1]);goto L8c663ab6; /* cmp/gt r1,r2 */
L8c663ab6: if(!(*sr&1u))goto L8c663abc;goto L8c663ab8; /* bf 0x8c663abc */
L8c663ab8: *stop=0x8c663ab8u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663aba; /* mov.l @(0x10,r14),r3 */
L8c663aba: *stop=0x8c663abau;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c663abc; /* mov.l r3,@(0x4,r15) */
L8c663abc: *stop=0x8c663abcu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663abe; /* mov.l @(0x10,r15),r2 */
L8c663abe: *stop=0x8c663abeu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663ac0; /* mov.l @(0x10,r14),r3 */
L8c663ac0: *stop=0x8c663ac0u;r[2]+=r[3];goto L8c663ac2; /* add r3,r2 */
L8c663ac2: *stop=0x8c663ac2u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c663ac4; /* mov.l r2,@(0x10,r15) */
L8c663ac4: *stop=0x8c663ac4u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c663ac6; /* mov.l @(0x8,r14),r14 */
L8c663ac6: *stop=0x8c663ac6u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c663ac8; /* tst r14,r14 */
L8c663ac8: if(!(*sr&1u))goto L8c663aa8;goto L8c663aca; /* bf 0x8c663aa8 */
L8c663aca: *stop=0x8c663acau;r[14]=r[13];goto L8c663acc; /* mov r13,r14 */
L8c663acc: *stop=0x8c663accu;r[14]+=20u;goto L8c663ace; /* add #0x14,r14 */
L8c663ace: *stop=0x8c663aceu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c663ad0; /* mov.l r14,@r15 */
L8c663ad0: *stop=0x8c663ad0u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c663ad2; /* mov.l @(0x8,r14),r14 */
L8c663ad2: *stop=0x8c663ad2u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c663ad4; /* tst r14,r14 */
L8c663ad4: if((*sr&1u))goto L8c663af0;goto L8c663ad6; /* bt 0x8c663af0 */
L8c663ad6: *stop=0x8c663ad6u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663ad8; /* mov.l @(0x10,r14),r3 */
L8c663ad8: *stop=0x8c663ad8u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663ada; /* mov.l @(0x8,r15),r2 */
L8c663ada: *stop=0x8c663adau;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[2]);goto L8c663adc; /* cmp/gt r2,r3 */
L8c663adc: if(!(*sr&1u))goto L8c663ae2;goto L8c663ade; /* bf 0x8c663ae2 */
L8c663ade: *stop=0x8c663adeu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663ae0; /* mov.l @(0x10,r14),r3 */
L8c663ae0: *stop=0x8c663ae0u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c663ae2; /* mov.l r3,@(0x8,r15) */
L8c663ae2: *stop=0x8c663ae2u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663ae4; /* mov.l @(0x18,r15),r2 */
L8c663ae4: *stop=0x8c663ae4u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663ae6; /* mov.l @(0x10,r14),r3 */
L8c663ae6: *stop=0x8c663ae6u;r[2]+=r[3];goto L8c663ae8; /* add r3,r2 */
L8c663ae8: *stop=0x8c663ae8u;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[2],4);if(io_status)return io_status;goto L8c663aea; /* mov.l r2,@(0x18,r15) */
L8c663aea: *stop=0x8c663aeau;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c663aec; /* mov.l @(0x8,r14),r14 */
L8c663aec: *stop=0x8c663aecu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c663aee; /* tst r14,r14 */
L8c663aee: if(!(*sr&1u))goto L8c663ad6;goto L8c663af0; /* bf 0x8c663ad6 */
L8c663af0: *stop=0x8c663af0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}goto L8c663af2; /* mov.l @r15,r14 */
L8c663af2: *stop=0x8c663af2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[14]=loaded;}goto L8c663af4; /* mov.l @r14,r14 */
L8c663af4: *stop=0x8c663af4u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c663af6; /* tst r14,r14 */
L8c663af6: if((*sr&1u))goto L8c663b1a;goto L8c663af8; /* bt 0x8c663b1a */
L8c663af8: *stop=0x8c663af8u;if(!valid_access(r[14],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;r[2]=loaded;}goto L8c663afa; /* mov.w @r14,r2 */
L8c663afa: *stop=0x8c663afau;r[2]=r[2]&65535u;goto L8c663afc; /* extu.w r2,r2 */
L8c663afc: *stop=0x8c663afcu;*sr=(*sr&~1u)|!(r[9]&r[2]);goto L8c663afe; /* tst r9,r2 */
L8c663afe: if(!(*sr&1u))goto L8c663b14;goto L8c663b00; /* bf 0x8c663b14 */
L8c663b00: *stop=0x8c663b00u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663b02; /* mov.l @(0x10,r14),r3 */
L8c663b02: *stop=0x8c663b02u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c663b04; /* mov.l @(0x8,r15),r1 */
L8c663b04: *stop=0x8c663b04u;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[1]);goto L8c663b06; /* cmp/gt r1,r3 */
L8c663b06: if(!(*sr&1u))goto L8c663b0c;goto L8c663b08; /* bf 0x8c663b0c */
L8c663b08: *stop=0x8c663b08u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663b0a; /* mov.l @(0x10,r14),r3 */
L8c663b0a: *stop=0x8c663b0au;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c663b0c; /* mov.l r3,@(0x8,r15) */
L8c663b0c: *stop=0x8c663b0cu;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663b0e; /* mov.l @(0x1c,r15),r2 */
L8c663b0e: *stop=0x8c663b0eu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663b10; /* mov.l @(0x10,r14),r3 */
L8c663b10: *stop=0x8c663b10u;r[2]+=r[3];goto L8c663b12; /* add r3,r2 */
L8c663b12: *stop=0x8c663b12u;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c663b14; /* mov.l r2,@(0x1c,r15) */
L8c663b14: *stop=0x8c663b14u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c663b16; /* mov.l @(0x8,r14),r14 */
L8c663b16: *stop=0x8c663b16u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c663b18; /* tst r14,r14 */
L8c663b18: if(!(*sr&1u))goto L8c663af8;goto L8c663b1a; /* bf 0x8c663af8 */
L8c663b1a: *stop=0x8c663b1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c663b6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663b1c; /* mov.l 0x8c663b6c,r2 */
L8c663b1c: *stop=0x8c663b1cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[10]=loaded;}goto L8c663b1e; /* mov.l @r2,r10 */
L8c663b1e: *stop=0x8c663b1eu;*sr=(*sr&~1u)|!(r[10]&r[10]);goto L8c663b20; /* tst r10,r10 */
L8c663b20: if(!(*sr&1u))goto L8c663b26;goto L8c663b22; /* bf 0x8c663b26 */
L8c663b22: *stop=0x8c663b24u;goto L8c663c50; /* bra 0x8c663c50 */
L8c663b26: *stop=0x8c663b26u;r[12]=16u;goto L8c663b28; /* mov #0x10,r12 */
L8c663b28: *stop=0x8c663b28u;if(!valid_access(r[10],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[0]=loaded;}goto L8c663b2a; /* mov.w @r10,r0 */
L8c663b2a: *stop=0x8c663b2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c663b70u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663b2c; /* mov.l 0x8c663b70,r3 */
L8c663b2c: *stop=0x8c663b2cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c663b60u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c663b2e; /* mov.w 0x8c663b60,r1 */
L8c663b2e: *stop=0x8c663b2eu;r[0]=r[0]&65535u;goto L8c663b30; /* extu.w r0,r0 */
L8c663b30: *stop=0x8c663b30u;r[0]&=r[3];goto L8c663b32; /* and r3,r0 */
L8c663b32: *stop=0x8c663b32u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c663b34; /* cmp/eq r1,r0 */
L8c663b34: if((*sr&1u))goto L8c663b9e;goto L8c663b36; /* bt 0x8c663b9e */
L8c663b36: *stop=0x8c663b36u;{uint32_t loaded=sx(m,bus,&io_status,0x8c663b62u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c663b38; /* mov.w 0x8c663b62,r1 */
L8c663b38: *stop=0x8c663b38u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c663b3a; /* cmp/eq r1,r0 */
L8c663b3a: if((*sr&1u))goto L8c663ba4;goto L8c663b3c; /* bt 0x8c663ba4 */
L8c663b3c: *stop=0x8c663b3cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c663b64u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c663b3e; /* mov.w 0x8c663b64,r1 */
L8c663b3e: *stop=0x8c663b3eu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c663b40; /* cmp/eq r1,r0 */
L8c663b40: if((*sr&1u))goto L8c663baa;goto L8c663b42; /* bt 0x8c663baa */
L8c663b42: *stop=0x8c663b42u;{uint32_t loaded=sx(m,bus,&io_status,0x8c663b66u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c663b44; /* mov.w 0x8c663b66,r1 */
L8c663b44: *stop=0x8c663b44u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c663b46; /* cmp/eq r1,r0 */
L8c663b46: *stop=0x8c663b48u;goto L8c663b80; /* bra 0x8c663b80 */
L8c663b80: if((*sr&1u))goto L8c663bb0;goto L8c663b82; /* bt 0x8c663bb0 */
L8c663b82: *stop=0x8c663b82u;{uint32_t loaded=sx(m,bus,&io_status,0x8c663bc0u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c663b84; /* mov.w 0x8c663bc0,r1 */
L8c663b84: *stop=0x8c663b84u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c663b86; /* cmp/eq r1,r0 */
L8c663b86: if((*sr&1u))goto L8c663be0;goto L8c663b88; /* bt 0x8c663be0 */
L8c663b88: *stop=0x8c663b88u;{uint32_t loaded=sx(m,bus,&io_status,0x8c663bc2u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c663b8a; /* mov.w 0x8c663bc2,r1 */
L8c663b8a: *stop=0x8c663b8au;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c663b8c; /* cmp/eq r1,r0 */
L8c663b8c: if((*sr&1u))goto L8c663be6;goto L8c663b8e; /* bt 0x8c663be6 */
L8c663b8e: *stop=0x8c663b8eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c663bc4u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c663b90; /* mov.w 0x8c663bc4,r1 */
L8c663b90: *stop=0x8c663b90u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c663b92; /* cmp/eq r1,r0 */
L8c663b92: if((*sr&1u))goto L8c663bec;goto L8c663b94; /* bt 0x8c663bec */
L8c663b94: *stop=0x8c663b94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c663bd0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c663b96; /* mov.l 0x8c663bd0,r1 */
L8c663b96: *stop=0x8c663b96u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c663b98; /* cmp/eq r1,r0 */
L8c663b98: if((*sr&1u))goto L8c663bf2;goto L8c663b9a; /* bt 0x8c663bf2 */
L8c663b9a: *stop=0x8c663b9cu;goto L8c663c20; /* bra 0x8c663c20 */
L8c663b9e: *stop=0x8c663b9eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c663bc6u,2);if(io_status)return io_status;r[13]=loaded;}goto L8c663ba0; /* mov.w 0x8c663bc6,r13 */
L8c663ba0: *stop=0x8c663ba2u;goto L8c663bb2; /* bra 0x8c663bb2 */
L8c663ba4: *stop=0x8c663ba4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c663bc8u,2);if(io_status)return io_status;r[13]=loaded;}goto L8c663ba6; /* mov.w 0x8c663bc8,r13 */
L8c663ba6: *stop=0x8c663ba8u;goto L8c663bb2; /* bra 0x8c663bb2 */
L8c663baa: *stop=0x8c663baau;{uint32_t loaded=sx(m,bus,&io_status,0x8c663bcau,2);if(io_status)return io_status;r[13]=loaded;}goto L8c663bac; /* mov.w 0x8c663bca,r13 */
L8c663bac: *stop=0x8c663baeu;goto L8c663bb2; /* bra 0x8c663bb2 */
L8c663bb0: *stop=0x8c663bb0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c663bccu,2);if(io_status)return io_status;r[13]=loaded;}goto L8c663bb2; /* mov.w 0x8c663bcc,r13 */
L8c663bb2: *stop=0x8c663bb4u;r[14]=r[12];goto L8c663c24; /* bra 0x8c663c24 */
L8c663be0: *stop=0x8c663be0u;{uint32_t loaded=sx(m,bus,&io_status,0x8c663c00u,2);if(io_status)return io_status;r[13]=loaded;}goto L8c663be2; /* mov.w 0x8c663c00,r13 */
L8c663be2: *stop=0x8c663be4u;r[14]=8u;goto L8c663c24; /* bra 0x8c663c24 */
L8c663be6: *stop=0x8c663be6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c663c02u,2);if(io_status)return io_status;r[13]=loaded;}goto L8c663be8; /* mov.w 0x8c663c02,r13 */
L8c663be8: *stop=0x8c663beau;r[14]=r[9];goto L8c663c24; /* bra 0x8c663c24 */
L8c663bec: *stop=0x8c663becu;{uint32_t loaded=sx(m,bus,&io_status,0x8c663c04u,2);if(io_status)return io_status;r[13]=loaded;}goto L8c663bee; /* mov.w 0x8c663c04,r13 */
L8c663bee: *stop=0x8c663bf0u;r[14]=2u;goto L8c663c24; /* bra 0x8c663c24 */
L8c663bf2: *stop=0x8c663bf2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c663c06u,2);if(io_status)return io_status;r[13]=loaded;}goto L8c663bf4; /* mov.w 0x8c663c06,r13 */
L8c663bf4: *stop=0x8c663bf6u;r[14]=1u;goto L8c663c24; /* bra 0x8c663c24 */
L8c663c20: *stop=0x8c663c20u;r[14]=r[11];goto L8c663c22; /* mov r11,r14 */
L8c663c22: *stop=0x8c663c22u;r[13]=r[11];goto L8c663c24; /* mov r11,r13 */
L8c663c24: *stop=0x8c663c24u;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c663c26; /* cmp/pl r14 */
L8c663c26: *stop=0x8c663c26u;r[1]=r[10];goto L8c663c28; /* mov r10,r1 */
L8c663c28: *stop=0x8c663c28u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c663c2a; /* mov.l r11,@r15 */
L8c663c2a: take=!(*sr&1u);*stop=0x8c663c2cu;r[1]+=12u;if(take)goto L8c663c46;goto L8c663c2e; /* bf/s 0x8c663c46 */
L8c663c2e: *stop=0x8c663c2eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c663c30; /* mov.l @r1,r2 */
L8c663c30: *stop=0x8c663c30u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c663c32; /* tst r2,r2 */
L8c663c32: if(!(*sr&1u))goto L8c663c3a;goto L8c663c34; /* bf 0x8c663c3a */
L8c663c34: *stop=0x8c663c34u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663c36; /* mov.l @(0xc,r15),r3 */
L8c663c36: *stop=0x8c663c36u;r[3]+=r[13];goto L8c663c38; /* add r13,r3 */
L8c663c38: *stop=0x8c663c38u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c663c3a; /* mov.l r3,@(0xc,r15) */
L8c663c3a: *stop=0x8c663c3au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c663c3c; /* mov.l @r15,r2 */
L8c663c3c: *stop=0x8c663c3cu;r[2]+=1u;goto L8c663c3e; /* add #0x1,r2 */
L8c663c3e: *stop=0x8c663c3eu;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[14]);goto L8c663c40; /* cmp/ge r14,r2 */
L8c663c40: *stop=0x8c663c40u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c663c42; /* mov.l r2,@r15 */
L8c663c42: take=!(*sr&1u);*stop=0x8c663c44u;r[1]+=4u;if(take)goto L8c663c2e;goto L8c663c46; /* bf/s 0x8c663c2e */
L8c663c46: *stop=0x8c663c46u;if(!valid_access(r[10]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c663c48; /* mov.l @(0x4,r10),r10 */
L8c663c48: *stop=0x8c663c48u;*sr=(*sr&~1u)|!(r[10]&r[10]);goto L8c663c4a; /* tst r10,r10 */
L8c663c4a: if((*sr&1u))goto L8c663c50;goto L8c663c4c; /* bt 0x8c663c50 */
L8c663c4c: *stop=0x8c663c4eu;goto L8c663b28; /* bra 0x8c663b28 */
L8c663c50: *stop=0x8c663c50u;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663c52; /* mov.l @(0x14,r15),r2 */
L8c663c52: *stop=0x8c663c52u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663c54; /* mov.l @(0x10,r15),r3 */
L8c663c54: *stop=0x8c663c54u;r[2]+=r[3];goto L8c663c56; /* add r3,r2 */
L8c663c56: *stop=0x8c663c56u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c663c58; /* mov.l r2,@r4 */
L8c663c58: *stop=0x8c663c58u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c663c5a; /* mov.l @(0x18,r15),r1 */
L8c663c5a: *stop=0x8c663c5au;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663c5c; /* mov.l @(0x1c,r15),r3 */
L8c663c5c: *stop=0x8c663c5cu;r[1]+=r[3];goto L8c663c5e; /* add r3,r1 */
L8c663c5e: *stop=0x8c663c5eu;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c663c60; /* mov.l r1,@r5 */
L8c663c60: *stop=0x8c663c60u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c663c62; /* mov.l @(0xc,r15),r3 */
L8c663c62: *stop=0x8c663c62u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c663c64; /* mov.l r3,@r6 */
L8c663c64: *stop=0x8c663c64u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c663c66; /* mov.l @(0x8,r15),r1 */
L8c663c66: *stop=0x8c663c66u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663c68; /* mov.l @(0x4,r15),r2 */
L8c663c68: *stop=0x8c663c68u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[1]);goto L8c663c6a; /* cmp/ge r1,r2 */
L8c663c6a: if((*sr&1u))goto L8c663c70;goto L8c663c6c; /* bt 0x8c663c70 */
L8c663c6c: *stop=0x8c663c6eu;r[2]=r[1];goto L8c663c72; /* bra 0x8c663c72 */
L8c663c70: *stop=0x8c663c70u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c663c72; /* mov.l @(0x4,r15),r2 */
L8c663c72: *stop=0x8c663c72u;r[0]=0u;goto L8c663c74; /* mov #0x0,r0 */
L8c663c74: *stop=0x8c663c74u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c663c76; /* mov.l r2,@r7 */
L8c663c76: *stop=0x8c663c76u;r[15]+=32u;goto L8c663c78; /* add #0x20,r15 */
L8c663c78: *stop=0x8c663c78u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c663c7a; /* mov.l @r15+,r9 */
L8c663c7a: *stop=0x8c663c7au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c663c7c; /* mov.l @r15+,r10 */
L8c663c7c: *stop=0x8c663c7cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c663c7e; /* mov.l @r15+,r11 */
L8c663c7e: *stop=0x8c663c7eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c663c80; /* mov.l @r15+,r12 */
L8c663c80: *stop=0x8c663c80u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c663c82; /* mov.l @r15+,r13 */
L8c663c82: *stop=0x8c663c82u;if(*pr!=entry_pr)return -3;*stop=0x8c663c84u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
