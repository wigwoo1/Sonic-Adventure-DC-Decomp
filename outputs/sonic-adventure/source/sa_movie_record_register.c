/* Original record registration/callback/work-pool proposals; explicit complete state.
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
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_record_register_8c010b00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c010b00;
L8c010b00: *stop=0x8c010b00u;r[6]=2u;goto L8c010b02; /* mov #0x2,r6 */
L8c010b02: *stop=0x8c010b02u;{uint32_t loaded=rd(m,bus,&io_status,0x8c010c00u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c010b04; /* mov.l 0x8c010c00,r4 */
L8c010b04: *stop=0x8c010b04u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c010b06; /* sts.l PR,@-r15 */
L8c010b06: *stop=0x8c010b06u;{uint32_t loaded=rd(m,bus,&io_status,0x8c010c04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c010b08; /* mov.l 0x8c010c04,r3 */
L8c010b08: *stop=0x8c010b08u;r[6]<<=8;goto L8c010b0a; /* shll8 r6 */
L8c010b0a: *stop=0x8c010b0au;r[6]+=96u;goto L8c010b0c; /* add #0x60,r6 */
L8c010b0c: target=r[3];*pr=0x8c010b10u;*stop=0x8c010b0eu;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c010b10; /* jsr @r3 */
L8c010b10: *stop=0x8c010b10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c010c10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c010b12; /* mov.l 0x8c010c10,r2 */
L8c010b12: *stop=0x8c010b12u;r[5]=1u;goto L8c010b14; /* mov #0x1,r5 */
L8c010b14: *stop=0x8c010b14u;{uint32_t loaded=rd(m,bus,&io_status,0x8c010c14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c010b16; /* mov.l 0x8c010c14,r1 */
L8c010b16: *stop=0x8c010b16u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c010b18; /* mov.l @r2,r3 */
L8c010b18: *stop=0x8c010b18u;{uint32_t loaded=rd(m,bus,&io_status,0x8c010c0cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c010b1a; /* mov.l 0x8c010c0c,r6 */
L8c010b1a: *stop=0x8c010b1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c010c08u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c010b1c; /* mov.l 0x8c010c08,r7 */
L8c010b1c: *stop=0x8c010b1cu;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[5]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[5]=n?(uint32_t)((int32_t)r[5]>>n):((r[5]&0x80000000u)?0xffffffffu:0u);}}goto L8c010b1e; /* shad r3,r5 */
L8c010b1e: target=r[1];*pr=0x8c010b22u;*stop=0x8c010b20u;r[4]=15u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c010b22; /* jsr @r1 */
L8c010b22: *stop=0x8c010b22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c010c1cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c010b24; /* mov.l 0x8c010c1c,r0 */
L8c010b24: *stop=0x8c010b24u;r[3]=0u;goto L8c010b26; /* mov #0x0,r3 */
L8c010b26: *stop=0x8c010b26u;{uint32_t loaded=rd(m,bus,&io_status,0x8c010c18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c010b28; /* mov.l 0x8c010c18,r2 */
L8c010b28: *stop=0x8c010b28u;r[1]=r[3];goto L8c010b2a; /* mov r3,r1 */
L8c010b2a: *stop=0x8c010b2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c010c20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c010b2c; /* mov.l 0x8c010c20,r1 */
L8c010b2c: *stop=0x8c010b2cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c010b2e; /* mov.l r3,@r2 */
L8c010b2e: *stop=0x8c010b2eu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c010b30; /* mov.l r3,@r0 */
L8c010b30: *stop=0x8c010b30u;r[3]=1u;goto L8c010b32; /* mov #0x1,r3 */
L8c010b32: *stop=0x8c010b32u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c010b34; /* lds.l @r15+,PR */
L8c010b34: *stop=0x8c010b34u;if(*pr!=entry_pr)return -3;*stop=0x8c010b36u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c602b68(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602b68;
L8c602b68: *stop=0x8c602b68u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c602b6a; /* mov.l r14,@-r15 */
L8c602b6a: *stop=0x8c602b6au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c602b6c; /* sts.l PR,@-r15 */
L8c602b6c: *stop=0x8c602b6cu;r[15]+=4294967276u;goto L8c602b6e; /* add #-0x14,r15 */
L8c602b6e: *stop=0x8c602b6eu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c602b70; /* stc SR,r0 */
L8c602b70: *stop=0x8c602b70u;r[3]=sx(m,bus,&io_status,0x8c602c7eu,2);if(io_status)return io_status;goto L8c602b72; /* mov.w 0x8c602c7e,r3 */
L8c602b72: *stop=0x8c602b72u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[4],4);if(io_status)return io_status;goto L8c602b74; /* mov.l r4,@(0xc,r15) */
L8c602b74: *stop=0x8c602b74u;r[0]>>=2;goto L8c602b76; /* shlr2 r0 */
L8c602b76: *stop=0x8c602b76u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c602b78; /* mov.l r5,@(0x4,r15) */
L8c602b78: *stop=0x8c602b78u;r[0]>>=2;goto L8c602b7a; /* shlr2 r0 */
L8c602b7a: *stop=0x8c602b7au;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c602b7c; /* mov.l r6,@(0x8,r15) */
L8c602b7c: *stop=0x8c602b7cu;r[0]&=0xfu;goto L8c602b7e; /* and #0xf,r0 */
L8c602b7e: *stop=0x8c602b7eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c602b80; /* mov.l r7,@r15 */
L8c602b80: *stop=0x8c602b80u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[0],4);if(io_status)return io_status;goto L8c602b82; /* mov.l r0,@(0x10,r15) */
L8c602b82: *stop=0x8c602b82u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c602b84; /* stc SR,r0 */
L8c602b84: *stop=0x8c602b84u;r[0]&=r[3];goto L8c602b86; /* and r3,r0 */
L8c602b86: *stop=0x8c602b86u;r[0]|=0xf0u;goto L8c602b88; /* or #0xf0,r0 */
L8c602b88: *stop=0x8c602b88u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c602b8a; /* ldc r0,SR */
L8c602b8a: *stop=0x8c602b8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c602c80u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c602b8c; /* mov.l 0x8c602c80,r1 */
L8c602b8c: *stop=0x8c602b8cu;r[4]=0u;goto L8c602b8e; /* mov #0x0,r4 */
L8c602b8e: *stop=0x8c602b8eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602b90; /* mov.l @(0x8,r15),r2 */
L8c602b90: *stop=0x8c602b90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602c84u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602b92; /* mov.l 0x8c602c84,r3 */
L8c602b92: *stop=0x8c602b92u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c602b94; /* mov.l r2,@r1 */
L8c602b94: *stop=0x8c602b94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602c88u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602b96; /* mov.l 0x8c602c88,r2 */
L8c602b96: *stop=0x8c602b96u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c602b98; /* mov.l r4,@r3 */
L8c602b98: *stop=0x8c602b98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602c90u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602b9a; /* mov.l 0x8c602c90,r3 */
L8c602b9a: *stop=0x8c602b9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c602c8cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c602b9c; /* mov.l 0x8c602c8c,r1 */
L8c602b9c: *stop=0x8c602b9cu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c602b9e; /* mov.l r4,@r2 */
L8c602b9e: *stop=0x8c602b9eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c602c94u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602ba0; /* mov.l 0x8c602c94,r2 */
L8c602ba0: *stop=0x8c602ba0u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c602ba2; /* mov.l r4,@r1 */
L8c602ba2: *stop=0x8c602ba2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c602ba4; /* mov.l r4,@r3 */
L8c602ba4: target=r[2];*pr=0x8c602ba8u;*stop=0x8c602ba6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602ba8; /* jsr @r2 */
L8c602ba8: *stop=0x8c602ba8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602c98u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602baa; /* mov.l 0x8c602c98,r3 */
L8c602baa: *stop=0x8c602baau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c602bac; /* mov.l @(0x4,r15),r5 */
L8c602bac: *stop=0x8c602bacu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c602bae; /* mov.l @(0x8,r15),r6 */
L8c602bae: target=r[3];*pr=0x8c602bb2u;*stop=0x8c602bb0u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602bb2; /* jsr @r3 */
L8c602bb2: *stop=0x8c602bb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602ca0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602bb4; /* mov.l 0x8c602ca0,r2 */
L8c602bb4: *stop=0x8c602bb4u;r[14]=r[0];goto L8c602bb6; /* mov r0,r14 */
L8c602bb6: *stop=0x8c602bb6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602ca4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602bb8; /* mov.l 0x8c602ca4,r3 */
L8c602bb8: *stop=0x8c602bb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602c9cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c602bba; /* mov.l 0x8c602c9c,r5 */
L8c602bba: target=r[3];*pr=0x8c602bbeu;*stop=0x8c602bbcu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602bbe; /* jsr @r3 */
L8c602bbe: *stop=0x8c602bbeu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c602bc0; /* tst r14,r14 */
L8c602bc0: if(!(*sr&1u))goto L8c602bce;goto L8c602bc2; /* bf 0x8c602bce */
L8c602bc2: *stop=0x8c602bc2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c602bc4; /* mov.l @r15,r3 */
L8c602bc4: *stop=0x8c602bc4u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c602bc6; /* tst r3,r3 */
L8c602bc6: if((*sr&1u))goto L8c602bce;goto L8c602bc8; /* bt 0x8c602bce */
L8c602bc8: *stop=0x8c602bc8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c602bca; /* mov.l @r15,r3 */
L8c602bca: target=r[3];*pr=0x8c602bceu;*stop=0x8c602bccu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602bce; /* jsr @r3 */
L8c602bce: *stop=0x8c602bceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c602ca8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c602bd0; /* mov.l 0x8c602ca8,r1 */
L8c602bd0: *stop=0x8c602bd0u;r[2]=1u;goto L8c602bd2; /* mov #0x1,r2 */
L8c602bd2: *stop=0x8c602bd2u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c602bd4; /* stc SR,r3 */
L8c602bd4: *stop=0x8c602bd4u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c602bd6; /* mov.l r2,@r1 */
L8c602bd6: *stop=0x8c602bd6u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c602bd8; /* mov.l @(0x10,r15),r0 */
L8c602bd8: *stop=0x8c602bd8u;r[2]=sx(m,bus,&io_status,0x8c602c7eu,2);if(io_status)return io_status;goto L8c602bda; /* mov.w 0x8c602c7e,r2 */
L8c602bda: *stop=0x8c602bdau;r[0]&=0xfu;goto L8c602bdc; /* and #0xf,r0 */
L8c602bdc: *stop=0x8c602bdcu;r[0]<<=2;goto L8c602bde; /* shll2 r0 */
L8c602bde: *stop=0x8c602bdeu;r[3]&=r[2];goto L8c602be0; /* and r2,r3 */
L8c602be0: *stop=0x8c602be0u;r[0]<<=2;goto L8c602be2; /* shll2 r0 */
L8c602be2: *stop=0x8c602be2u;r[0]|=r[3];goto L8c602be4; /* or r3,r0 */
L8c602be4: *stop=0x8c602be4u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c602be6; /* ldc r0,SR */
L8c602be6: *stop=0x8c602be6u;r[0]=r[14];goto L8c602be8; /* mov r14,r0 */
L8c602be8: *stop=0x8c602be8u;r[15]+=20u;goto L8c602bea; /* add #0x14,r15 */
L8c602bea: *stop=0x8c602beau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c602bec; /* lds.l @r15+,PR */
L8c602bec: *stop=0x8c602becu;if(*pr!=entry_pr)return -3;*stop=0x8c602beeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c6477cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6477cc;
L8c6477cc: *stop=0x8c6477ccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6477ce; /* mov.l r14,@-r15 */
L8c6477ce: *stop=0x8c6477ceu;r[14]=0u;goto L8c6477d0; /* mov #0x0,r14 */
L8c6477d0: *stop=0x8c6477d0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6477d2; /* mov.l r13,@-r15 */
L8c6477d2: *stop=0x8c6477d2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6477d4; /* mov.l r12,@-r15 */
L8c6477d4: *stop=0x8c6477d4u;r[12]=r[4];goto L8c6477d6; /* mov r4,r12 */
L8c6477d6: *stop=0x8c6477d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647848u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6477d8; /* mov.l 0x8c647848,r4 */
L8c6477d8: *stop=0x8c6477d8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6477da; /* sts.l PR,@-r15 */
L8c6477da: *stop=0x8c6477dau;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[14],4);if(io_status)return io_status;goto L8c6477dc; /* mov.l r14,@r4 */
L8c6477dc: *stop=0x8c6477dcu;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[14],4);if(io_status)return io_status;goto L8c6477de; /* mov.l r14,@(0x4,r4) */
L8c6477de: *stop=0x8c6477deu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[14],4);if(io_status)return io_status;goto L8c6477e0; /* mov.l r14,@(0x8,r4) */
L8c6477e0: *stop=0x8c6477e0u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[14],4);if(io_status)return io_status;goto L8c6477e2; /* mov.l r14,@(0xc,r4) */
L8c6477e2: *stop=0x8c6477e2u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[14],4);if(io_status)return io_status;goto L8c6477e4; /* mov.l r14,@(0x10,r4) */
L8c6477e4: *stop=0x8c6477e4u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[14],4);if(io_status)return io_status;goto L8c6477e6; /* mov.l r14,@(0x14,r4) */
L8c6477e6: *stop=0x8c6477e6u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[14],4);if(io_status)return io_status;goto L8c6477e8; /* mov.l r14,@(0x18,r4) */
L8c6477e8: *stop=0x8c6477e8u;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[14],4);if(io_status)return io_status;goto L8c6477ea; /* mov.l r14,@(0x1c,r4) */
L8c6477ea: *stop=0x8c6477eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c647850u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6477ec; /* mov.l 0x8c647850,r4 */
L8c6477ec: *stop=0x8c6477ecu;r[6]=sx(m,bus,&io_status,0x8c647844u,2);if(io_status)return io_status;goto L8c6477ee; /* mov.w 0x8c647844,r6 */
L8c6477ee: *stop=0x8c6477eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64784cu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c6477f0; /* mov.l 0x8c64784c,r13 */
L8c6477f0: target=r[13];*pr=0x8c6477f4u;*stop=0x8c6477f2u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6477f4; /* jsr @r13 */
L8c6477f4: *stop=0x8c6477f4u;r[14]=sx(m,bus,&io_status,0x8c647846u,2);if(io_status)return io_status;goto L8c6477f6; /* mov.w 0x8c647846,r14 */
L8c6477f6: *stop=0x8c6477f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647854u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6477f8; /* mov.l 0x8c647854,r4 */
L8c6477f8: *stop=0x8c6477f8u;r[6]=r[14];goto L8c6477fa; /* mov r14,r6 */
L8c6477fa: target=r[13];*pr=0x8c6477feu;*stop=0x8c6477fcu;r[5]=48u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6477fe; /* jsr @r13 */
L8c6477fe: *stop=0x8c6477feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647858u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c647800; /* mov.l 0x8c647858,r4 */
L8c647800: *stop=0x8c647800u;r[6]=r[14];goto L8c647802; /* mov r14,r6 */
L8c647802: target=r[13];*pr=0x8c647806u;*stop=0x8c647804u;r[5]=49u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647806; /* jsr @r13 */
L8c647806: *stop=0x8c647806u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64785cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c647808; /* mov.l 0x8c64785c,r4 */
L8c647808: *stop=0x8c647808u;r[6]=r[14];goto L8c64780a; /* mov r14,r6 */
L8c64780a: target=r[13];*pr=0x8c64780eu;*stop=0x8c64780cu;r[5]=50u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64780e; /* jsr @r13 */
L8c64780e: *stop=0x8c64780eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647860u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c647810; /* mov.l 0x8c647860,r4 */
L8c647810: *stop=0x8c647810u;r[6]=r[14];goto L8c647812; /* mov r14,r6 */
L8c647812: target=r[13];*pr=0x8c647816u;*stop=0x8c647814u;r[5]=51u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647816; /* jsr @r13 */
L8c647816: *stop=0x8c647816u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647864u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c647818; /* mov.l 0x8c647864,r4 */
L8c647818: *stop=0x8c647818u;r[6]=r[14];goto L8c64781a; /* mov r14,r6 */
L8c64781a: target=r[13];*pr=0x8c64781eu;*stop=0x8c64781cu;r[5]=52u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64781e; /* jsr @r13 */
L8c64781e: *stop=0x8c64781eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647868u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c647820; /* mov.l 0x8c647868,r4 */
L8c647820: *stop=0x8c647820u;r[6]=r[14];goto L8c647822; /* mov r14,r6 */
L8c647822: target=r[13];*pr=0x8c647826u;*stop=0x8c647824u;r[5]=53u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647826; /* jsr @r13 */
L8c647826: *stop=0x8c647826u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64786cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c647828; /* mov.l 0x8c64786c,r4 */
L8c647828: *stop=0x8c647828u;r[6]=r[14];goto L8c64782a; /* mov r14,r6 */
L8c64782a: target=r[13];*pr=0x8c64782eu;*stop=0x8c64782cu;r[5]=54u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64782e; /* jsr @r13 */
L8c64782e: *stop=0x8c64782eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647870u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c647830; /* mov.l 0x8c647870,r4 */
L8c647830: *stop=0x8c647830u;r[6]=r[14];goto L8c647832; /* mov r14,r6 */
L8c647832: target=r[13];*pr=0x8c647836u;*stop=0x8c647834u;r[5]=55u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647836; /* jsr @r13 */
L8c647836: *stop=0x8c647836u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647874u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647838; /* mov.l 0x8c647874,r3 */
L8c647838: target=r[3];*pr=0x8c64783cu;*stop=0x8c64783au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64783c; /* jsr @r3 */
L8c64783c: *stop=0x8c64783cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64783e; /* tst r0,r0 */
L8c64783e: if(!(*sr&1u))goto L8c647878;goto L8c647840; /* bf 0x8c647878 */
L8c647840: *stop=0x8c647842u;r[0]=4294967295u;goto L8c6478ec; /* bra 0x8c6478ec */
L8c647878: *stop=0x8c647878u;r[3]=1u;goto L8c64787a; /* mov #0x1,r3 */
L8c64787a: *stop=0x8c64787au;{uint32_t loaded=rd(m,bus,&io_status,0x8c647930u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64787c; /* mov.l 0x8c647930,r13 */
L8c64787c: *stop=0x8c64787cu;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c64787e; /* tst r12,r3 */
L8c64787e: if((*sr&1u))goto L8c647888;goto L8c647880; /* bt 0x8c647888 */
L8c647880: *stop=0x8c647880u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647938u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c647882; /* mov.l 0x8c647938,r4 */
L8c647882: *stop=0x8c647882u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647934u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647884; /* mov.l 0x8c647934,r5 */
L8c647884: target=r[13];*pr=0x8c647888u;*stop=0x8c647886u;r[6]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647888; /* jsr @r13 */
L8c647888: *stop=0x8c647888u;r[3]=2u;goto L8c64788a; /* mov #0x2,r3 */
L8c64788a: *stop=0x8c64788au;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c64788c; /* tst r12,r3 */
L8c64788c: if((*sr&1u))goto L8c647896;goto L8c64788e; /* bt 0x8c647896 */
L8c64788e: *stop=0x8c64788eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647940u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c647890; /* mov.l 0x8c647940,r4 */
L8c647890: *stop=0x8c647890u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64793cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647892; /* mov.l 0x8c64793c,r5 */
L8c647892: target=r[13];*pr=0x8c647896u;*stop=0x8c647894u;r[6]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647896; /* jsr @r13 */
L8c647896: *stop=0x8c647896u;r[3]=4u;goto L8c647898; /* mov #0x4,r3 */
L8c647898: *stop=0x8c647898u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c64789a; /* tst r12,r3 */
L8c64789a: if((*sr&1u))goto L8c6478a4;goto L8c64789c; /* bt 0x8c6478a4 */
L8c64789c: *stop=0x8c64789cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647948u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64789e; /* mov.l 0x8c647948,r4 */
L8c64789e: *stop=0x8c64789eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647944u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6478a0; /* mov.l 0x8c647944,r5 */
L8c6478a0: target=r[13];*pr=0x8c6478a4u;*stop=0x8c6478a2u;r[6]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6478a4; /* jsr @r13 */
L8c6478a4: *stop=0x8c6478a4u;r[3]=8u;goto L8c6478a6; /* mov #0x8,r3 */
L8c6478a6: *stop=0x8c6478a6u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c6478a8; /* tst r12,r3 */
L8c6478a8: if((*sr&1u))goto L8c6478b2;goto L8c6478aa; /* bt 0x8c6478b2 */
L8c6478aa: *stop=0x8c6478aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c647950u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6478ac; /* mov.l 0x8c647950,r4 */
L8c6478ac: *stop=0x8c6478acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64794cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6478ae; /* mov.l 0x8c64794c,r5 */
L8c6478ae: target=r[13];*pr=0x8c6478b2u;*stop=0x8c6478b0u;r[6]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6478b2; /* jsr @r13 */
L8c6478b2: *stop=0x8c6478b2u;r[3]=16u;goto L8c6478b4; /* mov #0x10,r3 */
L8c6478b4: *stop=0x8c6478b4u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c6478b6; /* tst r12,r3 */
L8c6478b6: if((*sr&1u))goto L8c6478c0;goto L8c6478b8; /* bt 0x8c6478c0 */
L8c6478b8: *stop=0x8c6478b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647958u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6478ba; /* mov.l 0x8c647958,r4 */
L8c6478ba: *stop=0x8c6478bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c647954u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6478bc; /* mov.l 0x8c647954,r5 */
L8c6478bc: target=r[13];*pr=0x8c6478c0u;*stop=0x8c6478beu;r[6]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6478c0; /* jsr @r13 */
L8c6478c0: *stop=0x8c6478c0u;r[3]=32u;goto L8c6478c2; /* mov #0x20,r3 */
L8c6478c2: *stop=0x8c6478c2u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c6478c4; /* tst r12,r3 */
L8c6478c4: if((*sr&1u))goto L8c6478ce;goto L8c6478c6; /* bt 0x8c6478ce */
L8c6478c6: *stop=0x8c6478c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647960u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6478c8; /* mov.l 0x8c647960,r4 */
L8c6478c8: *stop=0x8c6478c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64795cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6478ca; /* mov.l 0x8c64795c,r5 */
L8c6478ca: target=r[13];*pr=0x8c6478ceu;*stop=0x8c6478ccu;r[6]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6478ce; /* jsr @r13 */
L8c6478ce: *stop=0x8c6478ceu;r[3]=64u;goto L8c6478d0; /* mov #0x40,r3 */
L8c6478d0: *stop=0x8c6478d0u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c6478d2; /* tst r12,r3 */
L8c6478d2: if((*sr&1u))goto L8c6478dc;goto L8c6478d4; /* bt 0x8c6478dc */
L8c6478d4: *stop=0x8c6478d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647968u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6478d6; /* mov.l 0x8c647968,r4 */
L8c6478d6: *stop=0x8c6478d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647964u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6478d8; /* mov.l 0x8c647964,r5 */
L8c6478d8: target=r[13];*pr=0x8c6478dcu;*stop=0x8c6478dau;r[6]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6478dc; /* jsr @r13 */
L8c6478dc: *stop=0x8c6478dcu;r[3]=sx(m,bus,&io_status,0x8c64792au,2);if(io_status)return io_status;goto L8c6478de; /* mov.w 0x8c64792a,r3 */
L8c6478de: *stop=0x8c6478deu;*sr=(*sr&~1u)|!(r[3]&r[12]);goto L8c6478e0; /* tst r3,r12 */
L8c6478e0: if((*sr&1u))goto L8c6478ea;goto L8c6478e2; /* bt 0x8c6478ea */
L8c6478e2: *stop=0x8c6478e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647970u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6478e4; /* mov.l 0x8c647970,r4 */
L8c6478e4: *stop=0x8c6478e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64796cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6478e6; /* mov.l 0x8c64796c,r5 */
L8c6478e6: target=r[13];*pr=0x8c6478eau;*stop=0x8c6478e8u;r[6]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6478ea; /* jsr @r13 */
L8c6478ea: *stop=0x8c6478eau;r[0]=0u;goto L8c6478ec; /* mov #0x0,r0 */
L8c6478ec: *stop=0x8c6478ecu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6478ee; /* lds.l @r15+,PR */
L8c6478ee: *stop=0x8c6478eeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6478f0; /* mov.l @r15+,r12 */
L8c6478f0: *stop=0x8c6478f0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6478f2; /* mov.l @r15+,r13 */
L8c6478f2: *stop=0x8c6478f2u;if(*pr!=entry_pr)return -3;*stop=0x8c6478f4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c647e16(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c647e16;
L8c647e16: *stop=0x8c647e16u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c647e18; /* mov.l r14,@-r15 */
L8c647e18: *stop=0x8c647e18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c647e1a; /* mov.l r13,@-r15 */
L8c647e1a: *stop=0x8c647e1au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c647e1c; /* mov.l r12,@-r15 */
L8c647e1c: *stop=0x8c647e1cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c647e1e; /* mov.l r11,@-r15 */
L8c647e1e: *stop=0x8c647e1eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c647e20; /* mov.l r10,@-r15 */
L8c647e20: *stop=0x8c647e20u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c647e22; /* mov.l r9,@-r15 */
L8c647e22: *stop=0x8c647e22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c647e24; /* sts.l PR,@-r15 */
L8c647e24: *stop=0x8c647e24u;r[15]+=4294967292u;goto L8c647e26; /* add #-0x4,r15 */
L8c647e26: *stop=0x8c647e26u;r[10]=r[6];goto L8c647e28; /* mov r6,r10 */
L8c647e28: *stop=0x8c647e28u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647eb4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c647e2a; /* mov.l 0x8c647eb4,r6 */
L8c647e2a: *stop=0x8c647e2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c647eb0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647e2c; /* mov.l 0x8c647eb0,r3 */
L8c647e2c: *stop=0x8c647e2cu;r[12]=r[5];goto L8c647e2e; /* mov r5,r12 */
L8c647e2e: *stop=0x8c647e2eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647e9cu,4);if(io_status)return io_status;r[9]=loaded;}goto L8c647e30; /* mov.l 0x8c647e9c,r9 */
L8c647e30: *stop=0x8c647e30u;r[5]=r[6];goto L8c647e32; /* mov r6,r5 */
L8c647e32: *stop=0x8c647e32u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c647e34; /* mov.l r4,@r15 */
L8c647e34: *stop=0x8c647e34u;r[5]+=32u;goto L8c647e36; /* add #0x20,r5 */
L8c647e36: *stop=0x8c647e36u;r[14]=0u;goto L8c647e38; /* mov #0x0,r14 */
L8c647e38: *stop=0x8c647e38u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[3],4);if(io_status)return io_status;goto L8c647e3a; /* mov.l r3,@r9 */
L8c647e3a: *stop=0x8c647e3au;r[4]=r[6];goto L8c647e3c; /* mov r6,r4 */
L8c647e3c: *stop=0x8c647e3cu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[14],4);if(io_status)return io_status;goto L8c647e3e; /* mov.l r14,@r4 */
L8c647e3e: *stop=0x8c647e3eu;r[4]+=4u;goto L8c647e40; /* add #0x4,r4 */
L8c647e40: *stop=0x8c647e40u;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c647e42; /* cmp/hs r5,r4 */
L8c647e42: if(!(*sr&1u))goto L8c647e3c;goto L8c647e44; /* bf 0x8c647e3c */
L8c647e44: *stop=0x8c647e44u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647eb0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c647e46; /* mov.l 0x8c647eb0,r4 */
L8c647e46: *stop=0x8c647e46u;r[6]=20u;goto L8c647e48; /* mov #0x14,r6 */
L8c647e48: *stop=0x8c647e48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647eb8u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c647e4a; /* mov.l 0x8c647eb8,r11 */
L8c647e4a: target=r[11];*pr=0x8c647e4eu;*stop=0x8c647e4cu;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647e4e; /* jsr @r11 */
L8c647e4e: *stop=0x8c647e4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647ebcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c647e50; /* mov.l 0x8c647ebc,r4 */
L8c647e50: *stop=0x8c647e50u;r[6]=sx(m,bus,&io_status,0x8c647e8cu,2);if(io_status)return io_status;goto L8c647e52; /* mov.w 0x8c647e8c,r6 */
L8c647e52: target=r[11];*pr=0x8c647e56u;*stop=0x8c647e54u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647e56; /* jsr @r11 */
L8c647e56: target=0x8c647de8u;*pr=0x8c647e5au;*stop=0x8c647e58u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647e5a; /* bsr 0x8c647de8 */
L8c647e5a: *stop=0x8c647e5au;{uint32_t loaded=rd(m,bus,&io_status,0x8c647ec0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c647e5c; /* mov.l 0x8c647ec0,r4 */
L8c647e5c: *stop=0x8c647e5cu;r[2]=0u;goto L8c647e5e; /* mov #0x0,r2 */
L8c647e5e: *stop=0x8c647e5eu;r[5]=r[14];goto L8c647e60; /* mov r14,r5 */
L8c647e60: *stop=0x8c647e60u;r[6]=1u;goto L8c647e62; /* mov #0x1,r6 */
L8c647e62: *stop=0x8c647e62u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c647e64; /* mov.l r2,@r4 */
L8c647e64: *stop=0x8c647e64u;r[13]=8u;goto L8c647e66; /* mov #0x8,r13 */
L8c647e66: *stop=0x8c647e66u;r[3]=r[12];goto L8c647e68; /* mov r12,r3 */
L8c647e68: *stop=0x8c647e68u;*sr=(*sr&~1u)|!(r[6]&r[3]);goto L8c647e6a; /* tst r6,r3 */
L8c647e6a: take=(*sr&1u);*stop=0x8c647e6cu;r[5]+=1u;if(take)goto L8c647e74;goto L8c647e6e; /* bt/s 0x8c647e74 */
L8c647e6e: *stop=0x8c647e6eu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[1]=loaded;}goto L8c647e70; /* mov.l @r4,r1 */
L8c647e70: *stop=0x8c647e70u;r[1]+=1u;goto L8c647e72; /* add #0x1,r1 */
L8c647e72: *stop=0x8c647e72u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c647e74; /* mov.l r1,@r4 */
L8c647e74: *stop=0x8c647e74u;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c647e76; /* shll r6 */
L8c647e76: *stop=0x8c647e76u;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[13]);goto L8c647e78; /* cmp/ge r13,r5 */
L8c647e78: if(!(*sr&1u))goto L8c647e66;goto L8c647e7a; /* bf 0x8c647e66 */
L8c647e7a: *stop=0x8c647e7au;*sr=(*sr&~1u)|!(r[10]&r[10]);goto L8c647e7c; /* tst r10,r10 */
L8c647e7c: if((*sr&1u))goto L8c647ec4;goto L8c647e7e; /* bt 0x8c647ec4 */
L8c647e7e: *stop=0x8c647e7eu;r[5]=r[12];goto L8c647e80; /* mov r12,r5 */
L8c647e80: *stop=0x8c647e80u;r[6]=r[10];goto L8c647e82; /* mov r10,r6 */
L8c647e82: target=0x8c647f4cu;*pr=0x8c647e86u;*stop=0x8c647e84u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647e86; /* bsr 0x8c647f4c */
L8c647e86: *stop=0x8c647e86u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8c647e88; /* mov.l @r9,r2 */
L8c647e88: *stop=0x8c647e8au;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[14],4);if(io_status)return io_status;goto L8c647ed2; /* bra 0x8c647ed2 */
L8c647ec4: *stop=0x8c647ec4u;r[5]=r[12];goto L8c647ec6; /* mov r12,r5 */
L8c647ec6: *stop=0x8c647ec6u;r[6]=r[10];goto L8c647ec8; /* mov r10,r6 */
L8c647ec8: target=0x8c647f10u;*pr=0x8c647eccu;*stop=0x8c647ecau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647ecc; /* bsr 0x8c647f10 */
L8c647ecc: *stop=0x8c647eccu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8c647ece; /* mov.l @r9,r2 */
L8c647ece: *stop=0x8c647eceu;r[3]=1u;goto L8c647ed0; /* mov #0x1,r3 */
L8c647ed0: *stop=0x8c647ed0u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c647ed2; /* mov.l r3,@(0x8,r2) */
L8c647ed2: *stop=0x8c647ed2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647fb8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647ed4; /* mov.l 0x8c647fb8,r3 */
L8c647ed4: *stop=0x8c647ed4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647fb4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647ed6; /* mov.l 0x8c647fb4,r2 */
L8c647ed6: *stop=0x8c647ed6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647fbcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c647ed8; /* mov.l 0x8c647fbc,r1 */
L8c647ed8: *stop=0x8c647ed8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c647eda; /* mov.l r13,@r2 */
L8c647eda: *stop=0x8c647edau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c647edc; /* mov.l r14,@r3 */
L8c647edc: target=r[1];*pr=0x8c647ee0u;*stop=0x8c647edeu;r[4]=10u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647ee0; /* jsr @r1 */
L8c647ee0: *stop=0x8c647ee0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647fb4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647ee2; /* mov.l 0x8c647fb4,r2 */
L8c647ee2: *stop=0x8c647ee2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647fc0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c647ee4; /* mov.l 0x8c647fc0,r4 */
L8c647ee4: *stop=0x8c647ee4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[6]=loaded;}goto L8c647ee6; /* mov.l @r2,r6 */
L8c647ee6: *stop=0x8c647ee6u;r[3]=r[6];goto L8c647ee8; /* mov r6,r3 */
L8c647ee8: *stop=0x8c647ee8u;r[6]<<=2;goto L8c647eea; /* shll2 r6 */
L8c647eea: *stop=0x8c647eeau;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c647eec; /* shll r6 */
L8c647eec: *stop=0x8c647eecu;r[6]+=r[3];goto L8c647eee; /* add r3,r6 */
L8c647eee: *stop=0x8c647eeeu;r[6]<<=2;goto L8c647ef0; /* shll2 r6 */
L8c647ef0: *stop=0x8c647ef0u;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c647ef2; /* shll r6 */
L8c647ef2: target=r[11];*pr=0x8c647ef6u;*stop=0x8c647ef4u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647ef6; /* jsr @r11 */
L8c647ef6: *stop=0x8c647ef6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647fc4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647ef8; /* mov.l 0x8c647fc4,r3 */
L8c647ef8: target=r[3];*pr=0x8c647efcu;*stop=0x8c647efau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c647efc; /* jsr @r3 */
L8c647efc: *stop=0x8c647efcu;r[0]=r[14];goto L8c647efe; /* mov r14,r0 */
L8c647efe: *stop=0x8c647efeu;r[15]+=4u;goto L8c647f00; /* add #0x4,r15 */
L8c647f00: *stop=0x8c647f00u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c647f02; /* lds.l @r15+,PR */
L8c647f02: *stop=0x8c647f02u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c647f04; /* mov.l @r15+,r9 */
L8c647f04: *stop=0x8c647f04u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c647f06; /* mov.l @r15+,r10 */
L8c647f06: *stop=0x8c647f06u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c647f08; /* mov.l @r15+,r11 */
L8c647f08: *stop=0x8c647f08u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c647f0a; /* mov.l @r15+,r12 */
L8c647f0a: *stop=0x8c647f0au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c647f0c; /* mov.l @r15+,r13 */
L8c647f0c: *stop=0x8c647f0cu;if(*pr!=entry_pr)return -3;*stop=0x8c647f0eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c646eee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c646eee;
L8c646eee: *stop=0x8c646eeeu;r[3]=31u;goto L8c646ef0; /* mov #0x1f,r3 */
L8c646ef0: *stop=0x8c646ef0u;*sr=(*sr&~1u)|(r[4]>r[3]);goto L8c646ef2; /* cmp/hi r3,r4 */
L8c646ef2: if((*sr&1u))goto L8c646efa;goto L8c646ef4; /* bt 0x8c646efa */
L8c646ef4: *stop=0x8c646ef4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646f64u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c646ef6; /* mov.l 0x8c646f64,r0 */
L8c646ef6: *stop=0x8c646ef6u;r[4]<<=2;goto L8c646ef8; /* shll2 r4 */
L8c646ef8: *stop=0x8c646ef8u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c646efa; /* mov.l r5,@(r0,r4) */
L8c646efa: *stop=0x8c646efau;if(*pr!=entry_pr)return -3;*stop=0x8c646efcu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c010f22(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c010f22;
L8c010f22: *stop=0x8c010f22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c010f24; /* sts.l PR,@-r15 */
L8c010f24: *stop=0x8c010f24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c010f54u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c010f26; /* mov.l 0x8c010f54,r3 */
L8c010f26: target=r[3];*pr=0x8c010f2au;*stop=0x8c010f28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c010f2a; /* jsr @r3 */
L8c010f2a: *stop=0x8c010f2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c010f5cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c010f2c; /* mov.l 0x8c010f5c,r2 */
L8c010f2c: *stop=0x8c010f2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c010f58u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c010f2e; /* mov.l 0x8c010f58,r4 */
L8c010f2e: target=r[2];*pr=0x8c010f32u;*stop=0x8c010f30u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c010f32; /* jsr @r2 */
L8c010f32: *stop=0x8c010f32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c010f64u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c010f34; /* mov.l 0x8c010f64,r3 */
L8c010f34: *stop=0x8c010f34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c010f60u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c010f36; /* mov.l 0x8c010f60,r4 */
L8c010f36: target=r[3];*stop=0x8c010f38u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(target==0x8c602de0u)goto L8c602de0;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c602de0: *stop=0x8c602de0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602de2; /* mov.l 0x8c602f30,r3 */
L8c602de2: target=r[3];*stop=0x8c602de4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c602bf0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602bf0;
L8c602bf0: *stop=0x8c602bf0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c602bf2; /* sts.l PR,@-r15 */
L8c602bf2: *stop=0x8c602bf2u;r[15]+=4294967292u;goto L8c602bf4; /* add #-0x4,r15 */
L8c602bf4: *stop=0x8c602bf4u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c602bf6; /* stc SR,r0 */
L8c602bf6: *stop=0x8c602bf6u;r[3]=sx(m,bus,&io_status,0x8c602c7eu,2);if(io_status)return io_status;goto L8c602bf8; /* mov.w 0x8c602c7e,r3 */
L8c602bf8: *stop=0x8c602bf8u;r[0]>>=2;goto L8c602bfa; /* shlr2 r0 */
L8c602bfa: *stop=0x8c602bfau;r[0]>>=2;goto L8c602bfc; /* shlr2 r0 */
L8c602bfc: *stop=0x8c602bfcu;r[0]&=0xfu;goto L8c602bfe; /* and #0xf,r0 */
L8c602bfe: *stop=0x8c602bfeu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c602c00; /* mov.l r0,@r15 */
L8c602c00: *stop=0x8c602c00u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c602c02; /* stc SR,r0 */
L8c602c02: *stop=0x8c602c02u;r[0]&=r[3];goto L8c602c04; /* and r3,r0 */
L8c602c04: *stop=0x8c602c04u;r[0]|=0xf0u;goto L8c602c06; /* or #0xf0,r0 */
L8c602c06: *stop=0x8c602c06u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c602c08; /* ldc r0,SR */
L8c602c08: *stop=0x8c602c08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602ca4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602c0a; /* mov.l 0x8c602ca4,r3 */
L8c602c0a: *stop=0x8c602c0au;r[5]=0u;goto L8c602c0c; /* mov #0x0,r5 */
L8c602c0c: *stop=0x8c602c0cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c602ca0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602c0e; /* mov.l 0x8c602ca0,r2 */
L8c602c0e: target=r[3];*pr=0x8c602c12u;*stop=0x8c602c10u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602c12; /* jsr @r3 */
L8c602c12: *stop=0x8c602c12u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c602c14; /* mov.l @r15,r0 */
L8c602c14: *stop=0x8c602c14u;status=sa_sh4_stc_sr(*sr,&r[1],0u);if(status)return status;goto L8c602c16; /* stc SR,r1 */
L8c602c16: *stop=0x8c602c16u;r[3]=sx(m,bus,&io_status,0x8c602c7eu,2);if(io_status)return io_status;goto L8c602c18; /* mov.w 0x8c602c7e,r3 */
L8c602c18: *stop=0x8c602c18u;r[0]&=0xfu;goto L8c602c1a; /* and #0xf,r0 */
L8c602c1a: *stop=0x8c602c1au;r[0]<<=2;goto L8c602c1c; /* shll2 r0 */
L8c602c1c: *stop=0x8c602c1cu;r[1]&=r[3];goto L8c602c1e; /* and r3,r1 */
L8c602c1e: *stop=0x8c602c1eu;r[0]<<=2;goto L8c602c20; /* shll2 r0 */
L8c602c20: *stop=0x8c602c20u;r[0]|=r[1];goto L8c602c22; /* or r1,r0 */
L8c602c22: *stop=0x8c602c22u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c602c24; /* ldc r0,SR */
L8c602c24: *stop=0x8c602c24u;r[15]+=4u;goto L8c602c26; /* add #0x4,r15 */
L8c602c26: *stop=0x8c602c26u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c602c28; /* lds.l @r15+,PR */
L8c602c28: *stop=0x8c602c28u;if(*pr!=entry_pr)return -3;*stop=0x8c602c2au;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c602dda(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602dda;
L8c602dda: *stop=0x8c602ddau;{uint32_t loaded=rd(m,bus,&io_status,0x8c602f2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602ddc; /* mov.l 0x8c602f2c,r3 */
L8c602ddc: target=r[3];*stop=0x8c602ddeu;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c647c6c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c647c6c;
L8c647c6c: *stop=0x8c647c6cu;r[15]+=4294967292u;goto L8c647c6e; /* add #-0x4,r15 */
L8c647c6e: *stop=0x8c647c6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647d10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647c70; /* mov.l 0x8c647d10,r5 */
L8c647c70: *stop=0x8c647c70u;r[3]=sx(m,bus,&io_status,0x8c647d0eu,2);if(io_status)return io_status;goto L8c647c72; /* mov.w 0x8c647d0e,r3 */
L8c647c72: *stop=0x8c647c72u;r[7]=r[5];goto L8c647c74; /* mov r5,r7 */
L8c647c74: *stop=0x8c647c74u;r[6]=sx(m,bus,&io_status,0x8c647d0cu,2);if(io_status)return io_status;goto L8c647c76; /* mov.w 0x8c647d0c,r6 */
L8c647c76: *stop=0x8c647c76u;r[7]+=r[3];goto L8c647c78; /* add r3,r7 */
L8c647c78: *stop=0x8c647c78u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c647c7a; /* mov.l r5,@r15 */
L8c647c7a: *stop=0x8c647c7au;r[0]=120u;goto L8c647c7c; /* mov #0x78,r0 */
L8c647c7c: *stop=0x8c647c7cu;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[4],4);if(io_status)return io_status;goto L8c647c7e; /* mov.l r4,@(r0,r5) */
L8c647c7e: *stop=0x8c647c7eu;r[5]+=r[6];goto L8c647c80; /* add r6,r5 */
L8c647c80: *stop=0x8c647c80u;*sr=(*sr&~1u)|(r[5]>=r[7]);goto L8c647c82; /* cmp/hs r7,r5 */
L8c647c82: if(!(*sr&1u))goto L8c647c7a;goto L8c647c84; /* bf 0x8c647c7a */
L8c647c84: *stop=0x8c647c84u;if(*pr!=entry_pr)return -3;*stop=0x8c647c86u;r[15]+=4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c647de8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c647de8;
L8c647de8: *stop=0x8c647de8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647e9cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647dea; /* mov.l 0x8c647e9c,r5 */
L8c647dea: *stop=0x8c647deau;r[3]=1u;goto L8c647dec; /* mov #0x1,r3 */
L8c647dec: *stop=0x8c647decu;r[4]&=r[3];goto L8c647dee; /* and r3,r4 */
L8c647dee: *stop=0x8c647deeu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c647df0; /* mov.l @r5,r2 */
L8c647df0: *stop=0x8c647df0u;r[0]=r[4];goto L8c647df2; /* mov r4,r0 */
L8c647df2: *stop=0x8c647df2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c647df4; /* cmp/eq #0x1,r0 */
L8c647df4: *stop=0x8c647df4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c647df6; /* mov.l r4,@r2 */
L8c647df6: take=!(*sr&1u);*stop=0x8c647df8u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[6]=loaded;}if(take)goto L8c647e08;goto L8c647dfa; /* bf/s 0x8c647e08 */
L8c647dfa: *stop=0x8c647dfau;{uint32_t loaded=rd(m,bus,&io_status,0x8c647ea0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647dfc; /* mov.l 0x8c647ea0,r2 */
L8c647dfc: *stop=0x8c647dfcu;if(!valid_access(r[6]+12u,4u))return -3;wr(m,bus,&io_status,r[6]+0xcu,r[2],4);if(io_status)return io_status;goto L8c647dfe; /* mov.l r2,@(0xc,r6) */
L8c647dfe: *stop=0x8c647dfeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647ea4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647e00; /* mov.l 0x8c647ea4,r2 */
L8c647e00: *stop=0x8c647e00u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[1]=loaded;}goto L8c647e02; /* mov.l @r5,r1 */
L8c647e02: *stop=0x8c647e02u;if(!valid_access(r[1]+16u,4u))return -3;wr(m,bus,&io_status,r[1]+0x10u,r[2],4);if(io_status)return io_status;goto L8c647e04; /* mov.l r2,@(0x10,r1) */
L8c647e04: *stop=0x8c647e06u;goto L8c647e12; /* bra 0x8c647e12 */
L8c647e08: *stop=0x8c647e08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647eacu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c647e0a; /* mov.l 0x8c647eac,r1 */
L8c647e0a: *stop=0x8c647e0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c647ea8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c647e0c; /* mov.l 0x8c647ea8,r0 */
L8c647e0c: *stop=0x8c647e0cu;if(!valid_access(r[6]+12u,4u))return -3;wr(m,bus,&io_status,r[6]+0xcu,r[0],4);if(io_status)return io_status;goto L8c647e0e; /* mov.l r0,@(0xc,r6) */
L8c647e0e: *stop=0x8c647e0eu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c647e10; /* mov.l @r5,r2 */
L8c647e10: *stop=0x8c647e10u;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[1],4);if(io_status)return io_status;goto L8c647e12; /* mov.l r1,@(0x10,r2) */
L8c647e12: *stop=0x8c647e12u;if(*pr!=entry_pr)return -3;*stop=0x8c647e14u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c647f10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c647f10;
L8c647f10: *stop=0x8c647f10u;r[0]=r[4];goto L8c647f12; /* mov r4,r0 */
L8c647f12: *stop=0x8c647f12u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647fc8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647f14; /* mov.l 0x8c647fc8,r3 */
L8c647f14: *stop=0x8c647f14u;*sr=(*sr&~1u)|(r[0]==0x0000000fu);goto L8c647f16; /* cmp/eq #0xf,r0 */
L8c647f16: take=(*sr&1u);*stop=0x8c647f18u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}if(take)goto L8c647f2a;goto L8c647f1a; /* bt/s 0x8c647f2a */
L8c647f1a: *stop=0x8c647f1au;*sr=(*sr&~1u)|(r[0]==0x00000021u);goto L8c647f1c; /* cmp/eq #0x21,r0 */
L8c647f1c: if((*sr&1u))goto L8c647f30;goto L8c647f1e; /* bt 0x8c647f30 */
L8c647f1e: *stop=0x8c647f1eu;*sr=(*sr&~1u)|(r[0]==0x00000041u);goto L8c647f20; /* cmp/eq #0x41,r0 */
L8c647f20: if((*sr&1u))goto L8c647f34;goto L8c647f22; /* bt 0x8c647f34 */
L8c647f22: *stop=0x8c647f22u;*sr=(*sr&~1u)|(r[0]==0x0000007au);goto L8c647f24; /* cmp/eq #0x7a,r0 */
L8c647f24: if((*sr&1u))goto L8c647f3a;goto L8c647f26; /* bt 0x8c647f3a */
L8c647f26: *stop=0x8c647f28u;goto L8c647f40; /* bra 0x8c647f40 */
L8c647f2a: *stop=0x8c647f2au;r[2]=0u;goto L8c647f2c; /* mov #0x0,r2 */
L8c647f2c: *stop=0x8c647f2eu;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[2],4);if(io_status)return io_status;goto L8c647f46; /* bra 0x8c647f46 */
L8c647f30: *stop=0x8c647f32u;r[0]=1u;goto L8c647f3c; /* bra 0x8c647f3c */
L8c647f34: *stop=0x8c647f34u;r[1]=2u;goto L8c647f36; /* mov #0x2,r1 */
L8c647f36: *stop=0x8c647f38u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[1],4);if(io_status)return io_status;goto L8c647f46; /* bra 0x8c647f46 */
L8c647f3a: *stop=0x8c647f3au;r[0]=3u;goto L8c647f3c; /* mov #0x3,r0 */
L8c647f3c: *stop=0x8c647f3eu;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[0],4);if(io_status)return io_status;goto L8c647f46; /* bra 0x8c647f46 */
L8c647f40: *stop=0x8c647f40u;r[0]=sx(m,bus,&io_status,0x8c647fb2u,2);if(io_status)return io_status;goto L8c647f42; /* mov.w 0x8c647fb2,r0 */
L8c647f42: *stop=0x8c647f42u;if(*pr!=entry_pr)return -3;*stop=0x8c647f44u;return 0; /* rts */
L8c647f46: *stop=0x8c647f46u;r[0]=0u;goto L8c647f48; /* mov #0x0,r0 */
L8c647f48: *stop=0x8c647f48u;if(*pr!=entry_pr)return -3;*stop=0x8c647f4au;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c647f4c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c647f4c;
L8c647f4c: *stop=0x8c647f4cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c647f4e; /* mov.l r14,@-r15 */
L8c647f4e: *stop=0x8c647f4eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c647f50; /* mov.l r13,@-r15 */
L8c647f50: *stop=0x8c647f50u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c647f52; /* mov.l r12,@-r15 */
L8c647f52: *stop=0x8c647f52u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c647f54; /* mov.l r11,@-r15 */
L8c647f54: *stop=0x8c647f54u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c647f56; /* mov.l r10,@-r15 */
L8c647f56: *stop=0x8c647f56u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c647f58; /* mov.l r9,@-r15 */
L8c647f58: *stop=0x8c647f58u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c647f5a; /* mov.l r8,@-r15 */
L8c647f5a: *stop=0x8c647f5au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c647f5c; /* sts.l PR,@-r15 */
L8c647f5c: *stop=0x8c647f5cu;r[15]+=4294967276u;goto L8c647f5e; /* add #-0x14,r15 */
L8c647f5e: *stop=0x8c647f5eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c647f60; /* mov.l r5,@(0x4,r15) */
L8c647f60: *stop=0x8c647f60u;r[0]=r[4];goto L8c647f62; /* mov r4,r0 */
L8c647f62: *stop=0x8c647f62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647fc8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647f64; /* mov.l 0x8c647fc8,r3 */
L8c647f64: *stop=0x8c647f64u;*sr=(*sr&~1u)|(r[0]==0x0000000fu);goto L8c647f66; /* cmp/eq #0xf,r0 */
L8c647f66: *stop=0x8c647f66u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647fd4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647f68; /* mov.l 0x8c647fd4,r5 */
L8c647f68: *stop=0x8c647f68u;r[10]=1u;goto L8c647f6a; /* mov #0x1,r10 */
L8c647f6a: *stop=0x8c647f6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c647fccu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c647f6c; /* mov.l 0x8c647fcc,r14 */
L8c647f6c: *stop=0x8c647f6cu;r[13]=0u;goto L8c647f6e; /* mov #0x0,r13 */
L8c647f6e: *stop=0x8c647f6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c647fd0u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c647f70; /* mov.l 0x8c647fd0,r12 */
L8c647f70: take=(*sr&1u);*stop=0x8c647f72u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[7]=loaded;}if(take)goto L8c647f84;goto L8c647f74; /* bt/s 0x8c647f84 */
L8c647f74: *stop=0x8c647f74u;*sr=(*sr&~1u)|(r[0]==0x00000021u);goto L8c647f76; /* cmp/eq #0x21,r0 */
L8c647f76: if((*sr&1u))goto L8c647f8c;goto L8c647f78; /* bt 0x8c647f8c */
L8c647f78: *stop=0x8c647f78u;*sr=(*sr&~1u)|(r[0]==0x00000041u);goto L8c647f7a; /* cmp/eq #0x41,r0 */
L8c647f7a: if((*sr&1u))goto L8c647f96;goto L8c647f7c; /* bt 0x8c647f96 */
L8c647f7c: *stop=0x8c647f7cu;*sr=(*sr&~1u)|(r[0]==0x0000007au);goto L8c647f7e; /* cmp/eq #0x7a,r0 */
L8c647f7e: if((*sr&1u))goto L8c647fa0;goto L8c647f80; /* bt 0x8c647fa0 */
L8c647f80: *stop=0x8c647f82u;goto L8c647fac; /* bra 0x8c647fac */
L8c647f84: *stop=0x8c647f84u;if(!valid_access(r[7]+4u,4u))return -3;wr(m,bus,&io_status,r[7]+0x4u,r[13],4);if(io_status)return io_status;goto L8c647f86; /* mov.l r13,@(0x4,r7) */
L8c647f86: *stop=0x8c647f86u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[8]=loaded;}goto L8c647f88; /* mov.l @r12,r8 */
L8c647f88: *stop=0x8c647f8au;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[2]=loaded;}goto L8c647f92; /* bra 0x8c647f92 */
L8c647f8c: *stop=0x8c647f8cu;if(!valid_access(r[7]+4u,4u))return -3;wr(m,bus,&io_status,r[7]+0x4u,r[10],4);if(io_status)return io_status;goto L8c647f8e; /* mov.l r10,@(0x4,r7) */
L8c647f8e: *stop=0x8c647f8eu;if(!valid_access(r[5]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647f90; /* mov.l @(0x4,r5),r2 */
L8c647f90: *stop=0x8c647f90u;if(!valid_access(r[12]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c647f92; /* mov.l @(0x4,r12),r8 */
L8c647f92: *stop=0x8c647f94u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c647fd8; /* bra 0x8c647fd8 */
L8c647f96: *stop=0x8c647f96u;r[2]=2u;goto L8c647f98; /* mov #0x2,r2 */
L8c647f98: *stop=0x8c647f98u;if(!valid_access(r[7]+4u,4u))return -3;wr(m,bus,&io_status,r[7]+0x4u,r[2],4);if(io_status)return io_status;goto L8c647f9a; /* mov.l r2,@(0x4,r7) */
L8c647f9a: *stop=0x8c647f9au;if(!valid_access(r[12]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x8u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c647f9c; /* mov.l @(0x8,r12),r8 */
L8c647f9c: *stop=0x8c647f9eu;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c647fa8; /* bra 0x8c647fa8 */
L8c647fa0: *stop=0x8c647fa0u;r[2]=3u;goto L8c647fa2; /* mov #0x3,r2 */
L8c647fa2: *stop=0x8c647fa2u;if(!valid_access(r[7]+4u,4u))return -3;wr(m,bus,&io_status,r[7]+0x4u,r[2],4);if(io_status)return io_status;goto L8c647fa4; /* mov.l r2,@(0x4,r7) */
L8c647fa4: *stop=0x8c647fa4u;if(!valid_access(r[12]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;r[8]=loaded;}goto L8c647fa6; /* mov.l @(0xc,r12),r8 */
L8c647fa6: *stop=0x8c647fa6u;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c647fa8; /* mov.l @(0xc,r5),r1 */
L8c647fa8: *stop=0x8c647faau;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c647fd8; /* bra 0x8c647fd8 */
L8c647fac: *stop=0x8c647facu;r[0]=sx(m,bus,&io_status,0x8c647fb2u,2);if(io_status)return io_status;goto L8c647fae; /* mov.w 0x8c647fb2,r0 */
L8c647fae: *stop=0x8c647fb0u;goto L8c64808e; /* bra 0x8c64808e */
L8c647fd8: *stop=0x8c647fd8u;r[3]=r[8];goto L8c647fda; /* mov r8,r3 */
L8c647fda: *stop=0x8c647fdau;r[4]<<=8;goto L8c647fdc; /* shll8 r4 */
L8c647fdc: *stop=0x8c647fdcu;r[3]+=1u;goto L8c647fde; /* add #0x1,r3 */
L8c647fde: *stop=0x8c647fdeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6480e0u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c647fe0; /* mov.l 0x8c6480e0,r12 */
L8c647fe0: *stop=0x8c647fe0u;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c647fe2; /* shll r4 */
L8c647fe2: *stop=0x8c647fe2u;r[11]=r[6];goto L8c647fe4; /* mov r6,r11 */
L8c647fe4: *stop=0x8c647fe4u;r[4]+=32u;goto L8c647fe6; /* add #0x20,r4 */
L8c647fe6: *stop=0x8c647fe6u;r[9]=r[13];goto L8c647fe8; /* mov r13,r9 */
L8c647fe8: *stop=0x8c647fe8u;r[3]<<=8;goto L8c647fea; /* shll8 r3 */
L8c647fea: *stop=0x8c647feau;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[4],4);if(io_status)return io_status;goto L8c647fec; /* mov.l r4,@(0x8,r15) */
L8c647fec: *stop=0x8c647fecu;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c647fee; /* shll r3 */
L8c647fee: *stop=0x8c647feeu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c647ff0; /* mov.l r10,@r15 */
L8c647ff0: *stop=0x8c647ff2u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c648086; /* bra 0x8c648086 */
L8c647ff4: *stop=0x8c647ff4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c647ff6; /* mov.l @(0x4,r15),r2 */
L8c647ff6: *stop=0x8c647ff6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c647ff8; /* mov.l @r15,r3 */
L8c647ff8: *stop=0x8c647ff8u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c647ffa; /* tst r3,r2 */
L8c647ffa: if((*sr&1u))goto L8c64807a;goto L8c647ffc; /* bt 0x8c64807a */
L8c647ffc: *stop=0x8c647ffcu;r[3]=76u;goto L8c647ffe; /* mov #0x4c,r3 */
L8c647ffe: *stop=0x8c647ffeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6480e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648000; /* mov.l 0x8c6480e4,r2 */
L8c648000: *stop=0x8c648000u;*macl=r[3]*r[9];goto L8c648002; /* mul.l r3,r9 */
L8c648002: *stop=0x8c648002u;r[0]=r[10];goto L8c648004; /* mov r10,r0 */
L8c648004: *stop=0x8c648004u;if(!valid_access(r[14]+2u,1u))return -3;wr(m,bus,&io_status,r[14]+0x2u,r[0],1);if(io_status)return io_status;goto L8c648006; /* mov.b r0,@(0x2,r14) */
L8c648006: *stop=0x8c648006u;r[0]=r[13];goto L8c648008; /* mov r13,r0 */
L8c648008: *stop=0x8c648008u;r[1]=sx(m,bus,&io_status,0x8c6480d8u,2);if(io_status)return io_status;goto L8c64800a; /* mov.w 0x8c6480d8,r1 */
L8c64800a: *stop=0x8c64800au;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[9],2);if(io_status)return io_status;goto L8c64800c; /* mov.w r9,@r14 */
L8c64800c: *stop=0x8c64800cu;r[3]=*macl;goto L8c64800e; /* sts MACL,r3 */
L8c64800e: *stop=0x8c64800eu;r[1]+=r[11];goto L8c648010; /* add r11,r1 */
L8c648010: *stop=0x8c648010u;r[3]+=r[2];goto L8c648012; /* add r2,r3 */
L8c648012: *stop=0x8c648012u;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[3],4);if(io_status)return io_status;goto L8c648014; /* mov.l r3,@(0x1c,r14) */
L8c648014: *stop=0x8c648014u;r[3]=r[11];goto L8c648016; /* mov r11,r3 */
L8c648016: *stop=0x8c648016u;if(!valid_access(r[14]+10u,1u))return -3;wr(m,bus,&io_status,r[14]+0xau,r[0],1);if(io_status)return io_status;goto L8c648018; /* mov.b r0,@(0xa,r14) */
L8c648018: *stop=0x8c648018u;r[3]+=32u;goto L8c64801a; /* add #0x20,r3 */
L8c64801a: *stop=0x8c64801au;if(!valid_access(r[14]+11u,1u))return -3;wr(m,bus,&io_status,r[14]+0xbu,r[0],1);if(io_status)return io_status;goto L8c64801c; /* mov.b r0,@(0xb,r14) */
L8c64801c: *stop=0x8c64801cu;if(!valid_access(r[14]+12u,1u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],1);if(io_status)return io_status;goto L8c64801e; /* mov.b r0,@(0xc,r14) */
L8c64801e: *stop=0x8c64801eu;r[0]=r[13];goto L8c648020; /* mov r13,r0 */
L8c648020: *stop=0x8c648020u;if(!valid_access(r[14]+44u,4u))return -3;wr(m,bus,&io_status,r[14]+0x2cu,r[11],4);if(io_status)return io_status;goto L8c648022; /* mov.l r11,@(0x2c,r14) */
L8c648022: *stop=0x8c648022u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[3],4);if(io_status)return io_status;goto L8c648024; /* mov.l r3,@(0x20,r14) */
L8c648024: *stop=0x8c648024u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[1],4);if(io_status)return io_status;goto L8c648026; /* mov.l r1,@(0x24,r14) */
L8c648026: *stop=0x8c648026u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648028; /* mov.l @(0xc,r15),r3 */
L8c648028: *stop=0x8c648028u;r[3]+=r[11];goto L8c64802a; /* add r11,r3 */
L8c64802a: *stop=0x8c64802au;r[3]+=32u;goto L8c64802c; /* add #0x20,r3 */
L8c64802c: *stop=0x8c64802cu;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[3],4);if(io_status)return io_status;goto L8c64802e; /* mov.l r3,@(0x28,r14) */
L8c64802e: *stop=0x8c64802eu;if(!valid_access(r[14]+8u,1u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],1);if(io_status)return io_status;goto L8c648030; /* mov.b r0,@(0x8,r14) */
L8c648030: *stop=0x8c648030u;if(!valid_access(r[14]+9u,1u))return -3;wr(m,bus,&io_status,r[14]+0x9u,r[0],1);if(io_status)return io_status;goto L8c648032; /* mov.b r0,@(0x9,r14) */
L8c648032: *stop=0x8c648032u;r[0]=r[10];goto L8c648034; /* mov r10,r0 */
L8c648034: *stop=0x8c648034u;if(!valid_access(r[14]+14u,2u))return -3;wr(m,bus,&io_status,r[14]+0xeu,r[0],2);if(io_status)return io_status;goto L8c648036; /* mov.w r0,@(0xe,r14) */
L8c648036: *stop=0x8c648036u;r[0]=r[8];goto L8c648038; /* mov r8,r0 */
L8c648038: *stop=0x8c648038u;if(!valid_access(r[14]+16u,2u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[0],2);if(io_status)return io_status;goto L8c64803a; /* mov.w r0,@(0x10,r14) */
L8c64803a: *stop=0x8c64803au;if(!valid_access(r[15]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x10u,2);if(io_status)return io_status;goto L8c64803c; /* mov.w @(0x10,r15),r0 */
L8c64803c: *stop=0x8c64803cu;if(!valid_access(r[14]+18u,2u))return -3;wr(m,bus,&io_status,r[14]+0x12u,r[0],2);if(io_status)return io_status;goto L8c64803e; /* mov.w r0,@(0x12,r14) */
L8c64803e: *stop=0x8c64803eu;r[0]=32u;goto L8c648040; /* mov #0x20,r0 */
L8c648040: *stop=0x8c648040u;r[5]=0u;goto L8c648042; /* mov #0x0,r5 */
L8c648042: *stop=0x8c648042u;r[4]=r[14];goto L8c648044; /* mov r14,r4 */
L8c648044: *stop=0x8c648044u;if(!valid_access(r[14]+20u,2u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[0],2);if(io_status)return io_status;goto L8c648046; /* mov.w r0,@(0x14,r14) */
L8c648046: *stop=0x8c648046u;r[6]=16u;goto L8c648048; /* mov #0x10,r6 */
L8c648048: target=r[12];*pr=0x8c64804cu;*stop=0x8c64804au;r[4]+=56u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64804c; /* jsr @r12 */
L8c64804c: *stop=0x8c64804cu;r[6]=sx(m,bus,&io_status,0x8c6480dau,2);if(io_status)return io_status;goto L8c64804e; /* mov.w 0x8c6480da,r6 */
L8c64804e: *stop=0x8c64804eu;r[4]=r[14];goto L8c648050; /* mov r14,r4 */
L8c648050: *stop=0x8c648050u;r[5]=0u;goto L8c648052; /* mov #0x0,r5 */
L8c648052: target=r[12];*pr=0x8c648056u;*stop=0x8c648054u;r[4]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648056; /* jsr @r12 */
L8c648056: *stop=0x8c648056u;r[5]=71u;goto L8c648058; /* mov #0x47,r5 */
L8c648058: *stop=0x8c648058u;r[6]=32u;goto L8c64805a; /* mov #0x20,r6 */
L8c64805a: target=r[12];*pr=0x8c64805eu;*stop=0x8c64805cu;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64805e; /* jsr @r12 */
L8c64805e: *stop=0x8c64805eu;r[5]=83u;goto L8c648060; /* mov #0x53,r5 */
L8c648060: *stop=0x8c648060u;r[6]=32u;goto L8c648062; /* mov #0x20,r6 */
L8c648062: target=r[12];*pr=0x8c648066u;*stop=0x8c648064u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648066; /* jsr @r12 */
L8c648066: *stop=0x8c648066u;r[5]=70u;goto L8c648068; /* mov #0x46,r5 */
L8c648068: *stop=0x8c648068u;r[6]=32u;goto L8c64806a; /* mov #0x20,r6 */
L8c64806a: target=r[12];*pr=0x8c64806eu;*stop=0x8c64806cu;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64806e; /* jsr @r12 */
L8c64806e: *stop=0x8c64806eu;r[5]=68u;goto L8c648070; /* mov #0x44,r5 */
L8c648070: *stop=0x8c648070u;r[6]=32u;goto L8c648072; /* mov #0x20,r6 */
L8c648072: target=r[12];*pr=0x8c648076u;*stop=0x8c648074u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648076; /* jsr @r12 */
L8c648076: *stop=0x8c648076u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648078; /* mov.l @(0x8,r15),r3 */
L8c648078: *stop=0x8c648078u;r[11]+=r[3];goto L8c64807a; /* add r3,r11 */
L8c64807a: *stop=0x8c64807au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64807c; /* mov.l @r15,r3 */
L8c64807c: *stop=0x8c64807cu;r[9]+=1u;goto L8c64807e; /* add #0x1,r9 */
L8c64807e: *stop=0x8c64807eu;r[2]=sx(m,bus,&io_status,0x8c6480dcu,2);if(io_status)return io_status;goto L8c648080; /* mov.w 0x8c6480dc,r2 */
L8c648080: *stop=0x8c648080u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c648082; /* shll r3 */
L8c648082: *stop=0x8c648082u;r[14]+=r[2];goto L8c648084; /* add r2,r14 */
L8c648084: *stop=0x8c648084u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c648086; /* mov.l r3,@r15 */
L8c648086: *stop=0x8c648086u;r[3]=8u;goto L8c648088; /* mov #0x8,r3 */
L8c648088: *stop=0x8c648088u;*sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[3]);goto L8c64808a; /* cmp/ge r3,r9 */
L8c64808a: if(!(*sr&1u))goto L8c647ff4;goto L8c64808c; /* bf 0x8c647ff4 */
L8c64808c: *stop=0x8c64808cu;r[0]=0u;goto L8c64808e; /* mov #0x0,r0 */
L8c64808e: *stop=0x8c64808eu;r[15]+=20u;goto L8c648090; /* add #0x14,r15 */
L8c648090: *stop=0x8c648090u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c648092; /* lds.l @r15+,PR */
L8c648092: *stop=0x8c648092u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c648094; /* mov.l @r15+,r8 */
L8c648094: *stop=0x8c648094u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c648096; /* mov.l @r15+,r9 */
L8c648096: *stop=0x8c648096u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c648098; /* mov.l @r15+,r10 */
L8c648098: *stop=0x8c648098u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64809a; /* mov.l @r15+,r11 */
L8c64809a: *stop=0x8c64809au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64809c; /* mov.l @r15+,r12 */
L8c64809c: *stop=0x8c64809cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64809e; /* mov.l @r15+,r13 */
L8c64809e: *stop=0x8c64809eu;if(*pr!=entry_pr)return -3;*stop=0x8c6480a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c647534(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c647534;
L8c647534: *stop=0x8c647534u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6475f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c647536; /* mov.l 0x8c6475f4,r3 */
L8c647536: *stop=0x8c647536u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c647538; /* mov.l r4,@r3 */
L8c647538: *stop=0x8c647538u;if(*pr!=entry_pr)return -3;*stop=0x8c64753au;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c660898(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c660898;
L8c660898: *stop=0x8c660898u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660954u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66089a; /* mov.l 0x8c660954,r2 */
L8c66089a: *stop=0x8c66089au;r[3]=0u;goto L8c66089c; /* mov #0x0,r3 */
L8c66089c: *stop=0x8c66089cu;if(*pr!=entry_pr)return -3;*stop=0x8c66089eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c647c50(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c647c50;
L8c647c50: *stop=0x8c647c50u;r[15]+=4294967292u;goto L8c647c52; /* add #-0x4,r15 */
L8c647c52: *stop=0x8c647c52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c647d10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c647c54; /* mov.l 0x8c647d10,r5 */
L8c647c54: *stop=0x8c647c54u;r[3]=sx(m,bus,&io_status,0x8c647d0eu,2);if(io_status)return io_status;goto L8c647c56; /* mov.w 0x8c647d0e,r3 */
L8c647c56: *stop=0x8c647c56u;r[7]=r[5];goto L8c647c58; /* mov r5,r7 */
L8c647c58: *stop=0x8c647c58u;r[6]=sx(m,bus,&io_status,0x8c647d0cu,2);if(io_status)return io_status;goto L8c647c5a; /* mov.w 0x8c647d0c,r6 */
L8c647c5a: *stop=0x8c647c5au;r[7]+=r[3];goto L8c647c5c; /* add r3,r7 */
L8c647c5c: *stop=0x8c647c5cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c647c5e; /* mov.l r5,@r15 */
L8c647c5e: *stop=0x8c647c5eu;r[0]=116u;goto L8c647c60; /* mov #0x74,r0 */
L8c647c60: *stop=0x8c647c60u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[4],4);if(io_status)return io_status;goto L8c647c62; /* mov.l r4,@(r0,r5) */
L8c647c62: *stop=0x8c647c62u;r[5]+=r[6];goto L8c647c64; /* add r6,r5 */
L8c647c64: *stop=0x8c647c64u;*sr=(*sr&~1u)|(r[5]>=r[7]);goto L8c647c66; /* cmp/hs r7,r5 */
L8c647c66: if(!(*sr&1u))goto L8c647c5e;goto L8c647c68; /* bf 0x8c647c5e */
L8c647c68: *stop=0x8c647c68u;if(*pr!=entry_pr)return -3;*stop=0x8c647c6au;r[15]+=4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c66082c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c66082c;
L8c66082c: *stop=0x8c66082cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c66082e; /* sts.l PR,@-r15 */
L8c66082e: *stop=0x8c66082eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c660878u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660830; /* mov.l 0x8c660878,r3 */
L8c660830: target=r[3];*pr=0x8c660834u;*stop=0x8c660832u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660834; /* jsr @r3 */
L8c660834: *stop=0x8c660834u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66087cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660836; /* mov.l 0x8c66087c,r2 */
L8c660836: target=r[2];*pr=0x8c66083au;*stop=0x8c660838u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66083a; /* jsr @r2 */
L8c66083a: *stop=0x8c66083au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c66083c; /* lds.l @r15+,PR */
L8c66083c: *stop=0x8c66083cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c660880u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66083e; /* mov.l 0x8c660880,r3 */
L8c66083e: *stop=0x8c66083eu;if(*pr!=entry_pr)return -3;*stop=0x8c660840u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c660842(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c660842;
L8c660842: *stop=0x8c660842u;r[15]+=4294967284u;goto L8c660844; /* add #-0xc,r15 */
L8c660844: *stop=0x8c660844u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660880u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660846; /* mov.l 0x8c660880,r3 */
L8c660846: *stop=0x8c660846u;r[7]=r[6];goto L8c660848; /* mov r6,r7 */
L8c660848: *stop=0x8c660848u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c66084a; /* mov.l r4,@r15 */
L8c66084a: *stop=0x8c66084au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c66084c; /* mov.l r5,@(0x4,r15) */
L8c66084c: *stop=0x8c66084cu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c66084e; /* mov.l r6,@(0x8,r15) */
L8c66084e: *stop=0x8c66084eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c660850; /* mov.l @r3,r4 */
L8c660850: *stop=0x8c660850u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660884u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660852; /* mov.l 0x8c660884,r2 */
L8c660852: *stop=0x8c660852u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c660854; /* mov.l @(0x4,r15),r6 */
L8c660854: *stop=0x8c660854u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c660856; /* mov.l @r15,r5 */
L8c660856: target=r[2];*stop=0x8c660858u;r[15]+=12u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c668b38(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c668b38;
L8c668b38: *stop=0x8c668b38u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c668b3a; /* sts.l PR,@-r15 */
L8c668b3a: *stop=0x8c668b3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c668c48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668b3c; /* mov.l 0x8c668c48,r3 */
L8c668b3c: *stop=0x8c668b3cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c668c44u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c668b3e; /* mov.l 0x8c668c44,r4 */
L8c668b3e: *stop=0x8c668b3eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c668c40u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c668b40; /* mov.l 0x8c668c40,r6 */
L8c668b40: target=r[3];*pr=0x8c668b44u;*stop=0x8c668b42u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668b44; /* jsr @r3 */
L8c668b44: *stop=0x8c668b44u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668c50u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668b46; /* mov.l 0x8c668c50,r3 */
L8c668b46: *stop=0x8c668b46u;r[4]=0u;goto L8c668b48; /* mov #0x0,r4 */
L8c668b48: *stop=0x8c668b48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668c4cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c668b4a; /* mov.l 0x8c668c4c,r2 */
L8c668b4a: *stop=0x8c668b4au;r[1]=r[4];goto L8c668b4c; /* mov r4,r1 */
L8c668b4c: *stop=0x8c668b4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c668c54u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c668b4e; /* mov.l 0x8c668c54,r0 */
L8c668b4e: *stop=0x8c668b4eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c668b50; /* mov.l r4,@r2 */
L8c668b50: *stop=0x8c668b50u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c668b52; /* mov.l r4,@r3 */
L8c668b52: *stop=0x8c668b52u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c668b54; /* lds.l @r15+,PR */
L8c668b54: *stop=0x8c668b54u;if(*pr!=entry_pr)return -3;*stop=0x8c668b56u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[4],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c668b58(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c668b58;
L8c668b58: *stop=0x8c668b58u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c668b5a; /* mov.l r14,@-r15 */
L8c668b5a: *stop=0x8c668b5au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c668b5c; /* mov.l r13,@-r15 */
L8c668b5c: *stop=0x8c668b5cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c668b5e; /* sts.l PR,@-r15 */
L8c668b5e: *stop=0x8c668b5eu;r[15]+=4294967292u;goto L8c668b60; /* add #-0x4,r15 */
L8c668b60: *stop=0x8c668b60u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c668b62; /* stc SR,r0 */
L8c668b62: *stop=0x8c668b62u;r[3]=sx(m,bus,&io_status,0x8c668c2eu,2);if(io_status)return io_status;goto L8c668b64; /* mov.w 0x8c668c2e,r3 */
L8c668b64: *stop=0x8c668b64u;r[0]>>=2;goto L8c668b66; /* shlr2 r0 */
L8c668b66: *stop=0x8c668b66u;r[0]>>=2;goto L8c668b68; /* shlr2 r0 */
L8c668b68: *stop=0x8c668b68u;r[0]&=0xfu;goto L8c668b6a; /* and #0xf,r0 */
L8c668b6a: *stop=0x8c668b6au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c668b6c; /* mov.l r0,@r15 */
L8c668b6c: *stop=0x8c668b6cu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c668b6e; /* stc SR,r0 */
L8c668b6e: *stop=0x8c668b6eu;r[0]&=r[3];goto L8c668b70; /* and r3,r0 */
L8c668b70: *stop=0x8c668b70u;r[0]|=0xf0u;goto L8c668b72; /* or #0xf0,r0 */
L8c668b72: *stop=0x8c668b72u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c668b74; /* ldc r0,SR */
L8c668b74: *stop=0x8c668b74u;r[13]=0u;goto L8c668b76; /* mov #0x0,r13 */
L8c668b76: *stop=0x8c668b76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668c44u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c668b78; /* mov.l 0x8c668c44,r14 */
L8c668b78: *stop=0x8c668b78u;r[6]=8u;goto L8c668b7a; /* mov #0x8,r6 */
L8c668b7a: *stop=0x8c668b7au;r[5]=r[13];goto L8c668b7c; /* mov r13,r5 */
L8c668b7c: *stop=0x8c668b7cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c668b7e; /* mov.l @r14,r2 */
L8c668b7e: *stop=0x8c668b7eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c668b80; /* tst r2,r2 */
L8c668b80: if((*sr&1u))goto L8c668b8c;goto L8c668b82; /* bt 0x8c668b8c */
L8c668b82: *stop=0x8c668b82u;r[2]=sx(m,bus,&io_status,0x8c668c30u,2);if(io_status)return io_status;goto L8c668b84; /* mov.w 0x8c668c30,r2 */
L8c668b84: *stop=0x8c668b84u;r[5]+=1u;goto L8c668b86; /* add #0x1,r5 */
L8c668b86: *stop=0x8c668b86u;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c668b88; /* cmp/ge r6,r5 */
L8c668b88: take=!(*sr&1u);*stop=0x8c668b8au;r[14]+=r[2];if(take)goto L8c668b7c;goto L8c668b8c; /* bf/s 0x8c668b7c */
L8c668b8c: *stop=0x8c668b8cu;r[0]=r[5];goto L8c668b8e; /* mov r5,r0 */
L8c668b8e: *stop=0x8c668b8eu;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c668b90; /* cmp/eq #0x8,r0 */
L8c668b90: if(!(*sr&1u))goto L8c668b96;goto L8c668b92; /* bf 0x8c668b96 */
L8c668b92: *stop=0x8c668b94u;r[14]=r[13];goto L8c668bd0; /* bra 0x8c668bd0 */
L8c668b96: *stop=0x8c668b96u;r[0]=sx(m,bus,&io_status,0x8c668c32u,2);if(io_status)return io_status;goto L8c668b98; /* mov.w 0x8c668c32,r0 */
L8c668b98: *stop=0x8c668b98u;r[5]=0u;goto L8c668b9a; /* mov #0x0,r5 */
L8c668b9a: *stop=0x8c668b9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c668c48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668b9c; /* mov.l 0x8c668c48,r3 */
L8c668b9c: *stop=0x8c668b9cu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c668b9e; /* mov.l r4,@(r0,r14) */
L8c668b9e: *stop=0x8c668b9eu;r[0]+=4u;goto L8c668ba0; /* add #0x4,r0 */
L8c668ba0: *stop=0x8c668ba0u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[13],4);if(io_status)return io_status;goto L8c668ba2; /* mov.l r13,@(r0,r14) */
L8c668ba2: *stop=0x8c668ba2u;r[0]+=4294967288u;goto L8c668ba4; /* add #-0x8,r0 */
L8c668ba4: *stop=0x8c668ba4u;r[4]=sx(m,bus,&io_status,0x8c668c36u,2);if(io_status)return io_status;goto L8c668ba6; /* mov.w 0x8c668c36,r4 */
L8c668ba6: *stop=0x8c668ba6u;r[6]=sx(m,bus,&io_status,0x8c668c34u,2);if(io_status)return io_status;goto L8c668ba8; /* mov.w 0x8c668c34,r6 */
L8c668ba8: *stop=0x8c668ba8u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[13],4);if(io_status)return io_status;goto L8c668baa; /* mov.l r13,@(r0,r14) */
L8c668baa: target=r[3];*pr=0x8c668baeu;*stop=0x8c668bacu;r[4]+=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668bae; /* jsr @r3 */
L8c668bae: *stop=0x8c668baeu;r[6]=sx(m,bus,&io_status,0x8c668c38u,2);if(io_status)return io_status;goto L8c668bb0; /* mov.w 0x8c668c38,r6 */
L8c668bb0: *stop=0x8c668bb0u;r[5]=0u;goto L8c668bb2; /* mov #0x0,r5 */
L8c668bb2: *stop=0x8c668bb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668c48u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c668bb4; /* mov.l 0x8c668c48,r2 */
L8c668bb4: *stop=0x8c668bb4u;r[4]=r[6];goto L8c668bb6; /* mov r6,r4 */
L8c668bb6: *stop=0x8c668bb6u;r[4]+=4u;goto L8c668bb8; /* add #0x4,r4 */
L8c668bb8: target=r[2];*pr=0x8c668bbcu;*stop=0x8c668bbau;r[4]+=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668bbc; /* jsr @r2 */
L8c668bbc: *stop=0x8c668bbcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c668c58u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668bbe; /* mov.l 0x8c668c58,r3 */
L8c668bbe: *stop=0x8c668bbeu;r[4]=sx(m,bus,&io_status,0x8c668c3au,2);if(io_status)return io_status;goto L8c668bc0; /* mov.w 0x8c668c3a,r4 */
L8c668bc0: target=r[3];*pr=0x8c668bc4u;*stop=0x8c668bc2u;r[4]+=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668bc4; /* jsr @r3 */
L8c668bc4: *stop=0x8c668bc4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668c54u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c668bc6; /* mov.l 0x8c668c54,r4 */
L8c668bc6: *stop=0x8c668bc6u;r[2]=1u;goto L8c668bc8; /* mov #0x1,r2 */
L8c668bc8: *stop=0x8c668bc8u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c668bca; /* mov.l r2,@r14 */
L8c668bca: *stop=0x8c668bcau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668bcc; /* mov.l @r4,r3 */
L8c668bcc: *stop=0x8c668bccu;r[3]+=1u;goto L8c668bce; /* add #0x1,r3 */
L8c668bce: *stop=0x8c668bceu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c668bd0; /* mov.l r3,@r4 */
L8c668bd0: *stop=0x8c668bd0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c668bd2; /* mov.l @r15,r0 */
L8c668bd2: *stop=0x8c668bd2u;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c668bd4; /* stc SR,r2 */
L8c668bd4: *stop=0x8c668bd4u;r[1]=sx(m,bus,&io_status,0x8c668c2eu,2);if(io_status)return io_status;goto L8c668bd6; /* mov.w 0x8c668c2e,r1 */
L8c668bd6: *stop=0x8c668bd6u;r[0]&=0xfu;goto L8c668bd8; /* and #0xf,r0 */
L8c668bd8: *stop=0x8c668bd8u;r[0]<<=2;goto L8c668bda; /* shll2 r0 */
L8c668bda: *stop=0x8c668bdau;r[2]&=r[1];goto L8c668bdc; /* and r1,r2 */
L8c668bdc: *stop=0x8c668bdcu;r[0]<<=2;goto L8c668bde; /* shll2 r0 */
L8c668bde: *stop=0x8c668bdeu;r[0]|=r[2];goto L8c668be0; /* or r2,r0 */
L8c668be0: *stop=0x8c668be0u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c668be2; /* ldc r0,SR */
L8c668be2: *stop=0x8c668be2u;r[0]=r[14];goto L8c668be4; /* mov r14,r0 */
L8c668be4: *stop=0x8c668be4u;r[15]+=4u;goto L8c668be6; /* add #0x4,r15 */
L8c668be6: *stop=0x8c668be6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c668be8; /* lds.l @r15+,PR */
L8c668be8: *stop=0x8c668be8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c668bea; /* mov.l @r15+,r13 */
L8c668bea: *stop=0x8c668beau;if(*pr!=entry_pr)return -3;*stop=0x8c668becu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c668bee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c668bee;
L8c668bee: *stop=0x8c668beeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c668bf0; /* mov.l r14,@-r15 */
L8c668bf0: *stop=0x8c668bf0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c668bf2; /* mov.l r13,@-r15 */
L8c668bf2: *stop=0x8c668bf2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c668bf4; /* mov.l r12,@-r15 */
L8c668bf4: *stop=0x8c668bf4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c668bf6; /* mov.l r11,@-r15 */
L8c668bf6: *stop=0x8c668bf6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c668bf8; /* mov.l r10,@-r15 */
L8c668bf8: *stop=0x8c668bf8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c668bfa; /* sts.l PR,@-r15 */
L8c668bfa: *stop=0x8c668bfau;r[0]=sx(m,bus,&io_status,0x8c668c3cu,2);if(io_status)return io_status;goto L8c668bfc; /* mov.w 0x8c668c3c,r0 */
L8c668bfc: *stop=0x8c668bfcu;r[15]+=r[0];goto L8c668bfe; /* add r0,r15 */
L8c668bfe: *stop=0x8c668bfeu;r[10]=r[15];goto L8c668c00; /* mov r15,r10 */
L8c668c00: *stop=0x8c668c00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668c48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668c02; /* mov.l 0x8c668c48,r3 */
L8c668c02: *stop=0x8c668c02u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c668c04; /* mov.l r5,@(0x4,r15) */
L8c668c04: *stop=0x8c668c04u;r[5]=0u;goto L8c668c06; /* mov #0x0,r5 */
L8c668c06: *stop=0x8c668c06u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c668c08; /* mov.l r6,@r15 */
L8c668c08: *stop=0x8c668c08u;r[13]=r[4];goto L8c668c0a; /* mov r4,r13 */
L8c668c0a: *stop=0x8c668c0au;r[6]=sx(m,bus,&io_status,0x8c668c3eu,2);if(io_status)return io_status;goto L8c668c0c; /* mov.w 0x8c668c3e,r6 */
L8c668c0c: *stop=0x8c668c0cu;r[10]+=8u;goto L8c668c0e; /* add #0x8,r10 */
L8c668c0e: *stop=0x8c668c0eu;r[11]=r[7];goto L8c668c10; /* mov r7,r11 */
L8c668c10: target=r[3];*pr=0x8c668c14u;*stop=0x8c668c12u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668c14; /* jsr @r3 */
L8c668c14: *stop=0x8c668c14u;r[0]=sx(m,bus,&io_status,0x8c668c32u,2);if(io_status)return io_status;goto L8c668c16; /* mov.w 0x8c668c32,r0 */
L8c668c16: *stop=0x8c668c16u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c668c18; /* mov.l @(r0,r13),r2 */
L8c668c18: *stop=0x8c668c18u;r[0]+=4u;goto L8c668c1a; /* add #0x4,r0 */
L8c668c1a: *stop=0x8c668c1au;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668c1c; /* mov.l @(r0,r13),r3 */
L8c668c1c: *stop=0x8c668c1cu;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c668c1e; /* cmp/eq r2,r3 */
L8c668c1e: if(!(*sr&1u))goto L8c668c24;goto L8c668c20; /* bf 0x8c668c24 */
L8c668c20: *stop=0x8c668c22u;r[0]=4294967295u;goto L8c668cb8; /* bra 0x8c668cb8 */
L8c668c24: *stop=0x8c668c24u;r[14]=sx(m,bus,&io_status,0x8c668c36u,2);if(io_status)return io_status;goto L8c668c26; /* mov.w 0x8c668c36,r14 */
L8c668c26: *stop=0x8c668c26u;r[4]=0u;goto L8c668c28; /* mov #0x0,r4 */
L8c668c28: *stop=0x8c668c28u;r[12]=r[4];goto L8c668c2a; /* mov r4,r12 */
L8c668c2a: *stop=0x8c668c2cu;r[14]+=r[13];goto L8c668c62; /* bra 0x8c668c62 */
L8c668c5c: *stop=0x8c668c5cu;r[2]=sx(m,bus,&io_status,0x8c668d66u,2);if(io_status)return io_status;goto L8c668c5e; /* mov.w 0x8c668d66,r2 */
L8c668c5e: *stop=0x8c668c5eu;r[12]+=1u;goto L8c668c60; /* add #0x1,r12 */
L8c668c60: *stop=0x8c668c60u;r[14]+=r[2];goto L8c668c62; /* add r2,r14 */
L8c668c62: *stop=0x8c668c62u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668c64; /* mov.l @(0xc,r14),r3 */
L8c668c64: *stop=0x8c668c64u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c668c66; /* tst r3,r3 */
L8c668c66: if(!(*sr&1u))goto L8c668c5c;goto L8c668c68; /* bf 0x8c668c5c */
L8c668c68: *stop=0x8c668c68u;r[0]=sx(m,bus,&io_status,0x8c668d68u,2);if(io_status)return io_status;goto L8c668c6a; /* mov.w 0x8c668d68,r0 */
L8c668c6a: *stop=0x8c668c6au;r[2]=1u;goto L8c668c6c; /* mov #0x1,r2 */
L8c668c6c: *stop=0x8c668c6cu;r[6]=sx(m,bus,&io_status,0x8c668d6cu,2);if(io_status)return io_status;goto L8c668c6e; /* mov.w 0x8c668d6c,r6 */
L8c668c6e: *stop=0x8c668c6eu;r[11]>>=2;goto L8c668c70; /* shlr2 r11 */
L8c668c70: *stop=0x8c668c70u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[4],4);if(io_status)return io_status;goto L8c668c72; /* mov.l r4,@(0x8,r14) */
L8c668c72: *stop=0x8c668c72u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[2],4);if(io_status)return io_status;goto L8c668c74; /* mov.l r2,@(0xc,r14) */
L8c668c74: *stop=0x8c668c74u;r[5]=r[6];goto L8c668c76; /* mov r6,r5 */
L8c668c76: *stop=0x8c668c76u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668c78; /* mov.l @(r0,r13),r3 */
L8c668c78: *stop=0x8c668c78u;r[5]+=4u;goto L8c668c7a; /* add #0x4,r5 */
L8c668c7a: *stop=0x8c668c7au;r[0]=sx(m,bus,&io_status,0x8c668d6au,2);if(io_status)return io_status;goto L8c668c7c; /* mov.w 0x8c668d6a,r0 */
L8c668c7c: *stop=0x8c668c7cu;r[5]+=r[13];goto L8c668c7e; /* add r13,r5 */
L8c668c7e: *stop=0x8c668c7eu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[3],4);if(io_status)return io_status;goto L8c668c80; /* mov.l r3,@(0x14,r14) */
L8c668c80: *stop=0x8c668c80u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c668c82; /* mov.l @r15,r2 */
L8c668c82: *stop=0x8c668c82u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668d7cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668c84; /* mov.l 0x8c668d7c,r3 */
L8c668c84: *stop=0x8c668c84u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c668c86; /* mov.l r2,@r14 */
L8c668c86: *stop=0x8c668c86u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[11],4);if(io_status)return io_status;goto L8c668c88; /* mov.l r11,@(0x4,r14) */
L8c668c88: *stop=0x8c668c88u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c668c8a; /* mov.l r4,@(r0,r14) */
L8c668c8a: *stop=0x8c668c8au;r[0]+=4u;goto L8c668c8c; /* add #0x4,r0 */
L8c668c8c: *stop=0x8c668c8cu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[4],4);if(io_status)return io_status;goto L8c668c8e; /* mov.l r4,@(0x10,r14) */
L8c668c8e: *stop=0x8c668c8eu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[4],4);if(io_status)return io_status;goto L8c668c90; /* mov.l r4,@(r0,r14) */
L8c668c90: *stop=0x8c668c90u;r[4]=r[14];goto L8c668c92; /* mov r14,r4 */
L8c668c92: target=r[3];*pr=0x8c668c96u;*stop=0x8c668c94u;r[4]+=24u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668c96; /* jsr @r3 */
L8c668c96: *stop=0x8c668c96u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668d80u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c668c98; /* mov.l 0x8c668d80,r2 */
L8c668c98: target=r[2];*pr=0x8c668c9cu;*stop=0x8c668c9au;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668c9c; /* jsr @r2 */
L8c668c9c: *stop=0x8c668c9cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c668d7cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668c9e; /* mov.l 0x8c668d7c,r3 */
L8c668c9e: *stop=0x8c668c9eu;r[4]=r[14];goto L8c668ca0; /* mov r14,r4 */
L8c668ca0: *stop=0x8c668ca0u;r[6]=sx(m,bus,&io_status,0x8c668d6cu,2);if(io_status)return io_status;goto L8c668ca2; /* mov.w 0x8c668d6c,r6 */
L8c668ca2: *stop=0x8c668ca2u;r[5]=r[10];goto L8c668ca4; /* mov r10,r5 */
L8c668ca4: target=r[3];*pr=0x8c668ca8u;*stop=0x8c668ca6u;r[4]+=24u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668ca8; /* jsr @r3 */
L8c668ca8: *stop=0x8c668ca8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c668caa; /* mov.l @r15,r2 */
L8c668caa: *stop=0x8c668caau;r[11]+=4294967295u;goto L8c668cac; /* add #-0x1,r11 */
L8c668cac: *stop=0x8c668cacu;r[11]<<=2;goto L8c668cae; /* shll2 r11 */
L8c668cae: *stop=0x8c668caeu;r[0]=r[12];goto L8c668cb0; /* mov r12,r0 */
L8c668cb0: *stop=0x8c668cb0u;r[11]+=r[2];goto L8c668cb2; /* add r2,r11 */
L8c668cb2: *stop=0x8c668cb2u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[11],4);if(io_status)return io_status;goto L8c668cb4; /* mov.l r11,@(0x18,r14) */
L8c668cb4: *stop=0x8c668cb4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668cb6; /* mov.l @(0x4,r15),r3 */
L8c668cb6: *stop=0x8c668cb6u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[3],4);if(io_status)return io_status;goto L8c668cb8; /* mov.l r3,@(0x20,r14) */
L8c668cb8: *stop=0x8c668cb8u;r[1]=sx(m,bus,&io_status,0x8c668d6eu,2);if(io_status)return io_status;goto L8c668cba; /* mov.w 0x8c668d6e,r1 */
L8c668cba: *stop=0x8c668cbau;r[15]+=r[1];goto L8c668cbc; /* add r1,r15 */
L8c668cbc: *stop=0x8c668cbcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c668cbe; /* lds.l @r15+,PR */
L8c668cbe: *stop=0x8c668cbeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c668cc0; /* mov.l @r15+,r10 */
L8c668cc0: *stop=0x8c668cc0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c668cc2; /* mov.l @r15+,r11 */
L8c668cc2: *stop=0x8c668cc2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c668cc4; /* mov.l @r15+,r12 */
L8c668cc4: *stop=0x8c668cc4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c668cc6; /* mov.l @r15+,r13 */
L8c668cc6: *stop=0x8c668cc6u;if(*pr!=entry_pr)return -3;*stop=0x8c668cc8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_record_register_8c110954(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c110954;
L8c110954: *stop=0x8c110954u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[15],4);if(io_status)return io_status;goto L8c110956; /* mov.l r15,@r4 */
L8c110956: *stop=0x8c110956u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[14],4);if(io_status)return io_status;goto L8c110958; /* mov.l r14,@(0x4,r4) */
L8c110958: *stop=0x8c110958u;r[14]=*pr;goto L8c11095a; /* sts PR,r14 */
L8c11095a: *stop=0x8c11095au;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[14],4);if(io_status)return io_status;goto L8c11095c; /* mov.l r14,@(0x8,r4) */
L8c11095c: *stop=0x8c11095cu;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c11095e; /* mov.l @(0x4,r4),r14 */
L8c11095e: *stop=0x8c11095eu;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[13],4);if(io_status)return io_status;goto L8c110960; /* mov.l r13,@(0xc,r4) */
L8c110960: *stop=0x8c110960u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[12],4);if(io_status)return io_status;goto L8c110962; /* mov.l r12,@(0x10,r4) */
L8c110962: *stop=0x8c110962u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[11],4);if(io_status)return io_status;goto L8c110964; /* mov.l r11,@(0x14,r4) */
L8c110964: *stop=0x8c110964u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[10],4);if(io_status)return io_status;goto L8c110966; /* mov.l r10,@(0x18,r4) */
L8c110966: *stop=0x8c110966u;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[9],4);if(io_status)return io_status;goto L8c110968; /* mov.l r9,@(0x1c,r4) */
L8c110968: *stop=0x8c110968u;if(!valid_access(r[4]+32u,4u))return -3;wr(m,bus,&io_status,r[4]+0x20u,r[8],4);if(io_status)return io_status;goto L8c11096a; /* mov.l r8,@(0x20,r4) */
L8c11096a: *stop=0x8c11096au;if(!valid_access(r[4]+36u,4u))return -3;wr(m,bus,&io_status,r[4]+0x24u,r[7],4);if(io_status)return io_status;goto L8c11096c; /* mov.l r7,@(0x24,r4) */
L8c11096c: *stop=0x8c11096cu;if(!valid_access(r[4]+40u,4u))return -3;wr(m,bus,&io_status,r[4]+0x28u,r[6],4);if(io_status)return io_status;goto L8c11096e; /* mov.l r6,@(0x28,r4) */
L8c11096e: *stop=0x8c11096eu;if(!valid_access(r[4]+44u,4u))return -3;wr(m,bus,&io_status,r[4]+0x2cu,r[5],4);if(io_status)return io_status;goto L8c110970; /* mov.l r5,@(0x2c,r4) */
L8c110970: *stop=0x8c110970u;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[4],4);if(io_status)return io_status;goto L8c110972; /* mov.l r4,@(0x30,r4) */
L8c110972: *stop=0x8c110972u;r[4]+=48u;goto L8c110974; /* add #0x30,r4 */
L8c110974: *stop=0x8c110974u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[3],4);if(io_status)return io_status;goto L8c110976; /* mov.l r3,@(0x4,r4) */
L8c110976: *stop=0x8c110976u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[2],4);if(io_status)return io_status;goto L8c110978; /* mov.l r2,@(0x8,r4) */
L8c110978: *stop=0x8c110978u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[1],4);if(io_status)return io_status;goto L8c11097a; /* mov.l r1,@(0xc,r4) */
L8c11097a: *stop=0x8c11097au;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[0],4);if(io_status)return io_status;goto L8c11097c; /* mov.l r0,@(0x10,r4) */
L8c11097c: *stop=0x8c11097cu;r[0]=integer->mach;goto L8c11097e; /* sts MACH,r0 */
L8c11097e: *stop=0x8c11097eu;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[0],4);if(io_status)return io_status;goto L8c110980; /* mov.l r0,@(0x14,r4) */
L8c110980: *stop=0x8c110980u;r[0]=*macl;goto L8c110982; /* sts MACL,r0 */
L8c110982: *stop=0x8c110982u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[0],4);if(io_status)return io_status;goto L8c110984; /* mov.l r0,@(0x18,r4) */
L8c110984: *stop=0x8c110984u;r[0]=*gbr;goto L8c110986; /* stc GBR,r0 */
L8c110986: *stop=0x8c110986u;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c110988; /* mov.l r0,@(0x1c,r4) */
L8c110988: *stop=0x8c110988u;r[4]+=92u;goto L8c11098a; /* add #0x5c,r4 */
L8c11098a: *stop=0x8c11098au;if(*sr&0x8000u)return -4;r[0]=f->fpscr;goto L8c11098c; /* sts FPSCR,r0 */
L8c11098c: *stop=0x8c11098cu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[0],4);if(io_status)return io_status;goto L8c11098e; /* mov.l r0,@(0x8,r4) */
L8c11098e: *stop=0x8c11098eu;if(*sr&0x8000u)return -4;r[0]=*fpul;goto L8c110990; /* sts FPUL,r0 */
L8c110990: *stop=0x8c110990u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[0],4);if(io_status)return io_status;goto L8c110992; /* mov.l r0,@(0x4,r4) */
L8c110992: *stop=0x8c110992u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[4]);if(io_status)return io_status;goto L8c110994; /* fmov.s fr15,@r4 */
L8c110994: *stop=0x8c110994u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[4]);if(io_status)return io_status;goto L8c110996; /* fmov.s fr14,@-r4 */
L8c110996: *stop=0x8c110996u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[4]);if(io_status)return io_status;goto L8c110998; /* fmov.s fr13,@-r4 */
L8c110998: *stop=0x8c110998u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[4]);if(io_status)return io_status;goto L8c11099a; /* fmov.s fr12,@-r4 */
L8c11099a: *stop=0x8c11099au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;goto L8c11099c; /* fmov.s fr11,@-r4 */
L8c11099c: *stop=0x8c11099cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;goto L8c11099e; /* fmov.s fr10,@-r4 */
L8c11099e: *stop=0x8c11099eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c1109a0; /* fmov.s fr9,@-r4 */
L8c1109a0: *stop=0x8c1109a0u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;goto L8c1109a2; /* fmov.s fr8,@-r4 */
L8c1109a2: *stop=0x8c1109a2u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c1109a4; /* fmov.s fr7,@-r4 */
L8c1109a4: *stop=0x8c1109a4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c1109a6; /* fmov.s fr6,@-r4 */
L8c1109a6: *stop=0x8c1109a6u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c1109a8; /* fmov.s fr5,@-r4 */
L8c1109a8: *stop=0x8c1109a8u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c1109aa; /* fmov.s fr4,@-r4 */
L8c1109aa: *stop=0x8c1109aau;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c1109ac; /* fmov.s fr3,@-r4 */
L8c1109ac: *stop=0x8c1109acu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c1109ae; /* fmov.s fr2,@-r4 */
L8c1109ae: *stop=0x8c1109aeu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;goto L8c1109b0; /* fmov.s fr1,@-r4 */
L8c1109b0: *stop=0x8c1109b0u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c1109b2; /* fmov.s fr0,@-r4 */
L8c1109b2: *stop=0x8c1109b2u;r[4]+=4294967264u;goto L8c1109b4; /* add #-0x20,r4 */
L8c1109b4: *stop=0x8c1109b4u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]=loaded;}goto L8c1109b6; /* mov.l @r4,r4 */
L8c1109b6: *stop=0x8c1109b6u;if(*pr!=entry_pr)return -3;*stop=0x8c1109b8u;r[0]=0u;return 0; /* rts */
}
