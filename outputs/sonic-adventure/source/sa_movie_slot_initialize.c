/* Geometry projection, sampling and view proposals; explicit GBR/queue state.
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
SA_STAGE_EXPORT int sa_movie_slot_initialize_8c109cfe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c109cfe;
L8c109cfe: *stop=0x8c109cfeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c109d00; /* mov.l r14,@-r15 */
L8c109d00: *stop=0x8c109d00u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c109d02; /* mov.l r13,@-r15 */
L8c109d02: *stop=0x8c109d02u;r[13]=0u;goto L8c109d04; /* mov #0x0,r13 */
L8c109d04: *stop=0x8c109d04u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c109d06; /* mov.l r12,@-r15 */
L8c109d06: *stop=0x8c109d06u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c109d08; /* mov.l r11,@-r15 */
L8c109d08: *stop=0x8c109d08u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c109d0a; /* mov.l r10,@-r15 */
L8c109d0a: *stop=0x8c109d0au;r[10]=25u;goto L8c109d0c; /* mov #0x19,r10 */
L8c109d0c: *stop=0x8c109d0cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c109d0e; /* mov.l r9,@-r15 */
L8c109d0e: *stop=0x8c109d0eu;r[10]<<=8;goto L8c109d10; /* shll8 r10 */
L8c109d10: *stop=0x8c109d10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c109de8u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c109d12; /* mov.l 0x8c109de8,r9 */
L8c109d12: *stop=0x8c109d12u;r[10]+=4294967280u;goto L8c109d14; /* add #-0x10,r10 */
L8c109d14: *stop=0x8c109d14u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c109d16; /* mov.l r8,@-r15 */
L8c109d16: *stop=0x8c109d16u;r[14]=r[9];goto L8c109d18; /* mov r9,r14 */
L8c109d18: *stop=0x8c109d18u;r[14]+=16u;goto L8c109d1a; /* add #0x10,r14 */
L8c109d1a: *stop=0x8c109d1au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c109d1c; /* sts.l PR,@-r15 */
L8c109d1c: *stop=0x8c109d1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c109decu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c109d1e; /* mov.l 0x8c109dec,r12 */
L8c109d1e: *stop=0x8c109d1eu;r[10]+=r[14];goto L8c109d20; /* add r14,r10 */
L8c109d20: *stop=0x8c109d20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c109df0u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c109d22; /* mov.l 0x8c109df0,r8 */
L8c109d22: *stop=0x8c109d22u;*sr=(*sr&~1u)|(r[14]>=r[10]);goto L8c109d24; /* cmp/hs r10,r14 */
L8c109d24: take=(*sr&1u);*stop=0x8c109d26u;r[11]=r[13];if(take)goto L8c109d54;goto L8c109d28; /* bt/s 0x8c109d54 */
L8c109d28: target=r[8];*pr=0x8c109d2cu;*stop=0x8c109d2au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c109d2c; /* jsr @r8 */
L8c109d2c: *stop=0x8c109d2cu;r[0]=6u;goto L8c109d2e; /* mov #0x6,r0 */
L8c109d2e: *stop=0x8c109d2eu;r[0]<<=8;goto L8c109d30; /* shll8 r0 */
L8c109d30: *stop=0x8c109d30u;r[0]+=3u;goto L8c109d32; /* add #0x3,r0 */
L8c109d32: *stop=0x8c109d32u;if(!valid_access(r[0]+r[14],1u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[13],1);if(io_status)return io_status;goto L8c109d34; /* mov.b r13,@(r0,r14) */
L8c109d34: *stop=0x8c109d34u;r[0]+=45u;goto L8c109d36; /* add #0x2d,r0 */
L8c109d36: *stop=0x8c109d36u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}r[12]+=4;goto L8c109d38; /* mov.l @r12+,r3 */
L8c109d38: *stop=0x8c109d38u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[3],4);if(io_status)return io_status;goto L8c109d3a; /* mov.l r3,@(r0,r14) */
L8c109d3a: *stop=0x8c109d3au;r[0]+=4294967250u;goto L8c109d3c; /* add #-0x2e,r0 */
L8c109d3c: *stop=0x8c109d3cu;if(!valid_access(r[0]+r[14],1u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[11],1);if(io_status)return io_status;goto L8c109d3e; /* mov.b r11,@(r0,r14) */
L8c109d3e: *stop=0x8c109d3eu;r[0]+=50u;goto L8c109d40; /* add #0x32,r0 */
L8c109d40: *stop=0x8c109d40u;r[3]=6u;goto L8c109d42; /* mov #0x6,r3 */
L8c109d42: *stop=0x8c109d42u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[13],4);if(io_status)return io_status;goto L8c109d44; /* mov.l r13,@(r0,r14) */
L8c109d44: *stop=0x8c109d44u;r[3]<<=8;goto L8c109d46; /* shll8 r3 */
L8c109d46: *stop=0x8c109d46u;r[0]+=4u;goto L8c109d48; /* add #0x4,r0 */
L8c109d48: *stop=0x8c109d48u;r[3]+=60u;goto L8c109d4a; /* add #0x3c,r3 */
L8c109d4a: *stop=0x8c109d4au;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[13],4);if(io_status)return io_status;goto L8c109d4c; /* mov.l r13,@(r0,r14) */
L8c109d4c: *stop=0x8c109d4cu;r[14]+=r[3];goto L8c109d4e; /* add r3,r14 */
L8c109d4e: *stop=0x8c109d4eu;*sr=(*sr&~1u)|(r[14]>=r[10]);goto L8c109d50; /* cmp/hs r10,r14 */
L8c109d50: take=!(*sr&1u);*stop=0x8c109d52u;r[11]+=1u;if(take)goto L8c109d28;goto L8c109d54; /* bf/s 0x8c109d28 */
L8c109d54: *stop=0x8c109d54u;{uint32_t loaded=rd(m,bus,&io_status,0x8c109df4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c109d56; /* mov.l 0x8c109df4,r3 */
L8c109d56: target=r[3];*pr=0x8c109d5au;*stop=0x8c109d58u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c109d5a; /* jsr @r3 */
L8c109d5a: *stop=0x8c109d5au;if(!valid_access(r[9]+4u,4u))return -3;wr(m,bus,&io_status,r[9]+0x4u,r[13],4);if(io_status)return io_status;goto L8c109d5c; /* mov.l r13,@(0x4,r9) */
L8c109d5c: *stop=0x8c109d5cu;if(!valid_access(r[9]+8u,4u))return -3;wr(m,bus,&io_status,r[9]+0x8u,r[13],4);if(io_status)return io_status;goto L8c109d5e; /* mov.l r13,@(0x8,r9) */
L8c109d5e: *stop=0x8c109d5eu;if(!valid_access(r[9]+12u,4u))return -3;wr(m,bus,&io_status,r[9]+0xcu,r[13],4);if(io_status)return io_status;goto L8c109d60; /* mov.l r13,@(0xc,r9) */
L8c109d60: *stop=0x8c109d60u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c109d62; /* lds.l @r15+,PR */
L8c109d62: *stop=0x8c109d62u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c109d64; /* mov.l @r15+,r8 */
L8c109d64: *stop=0x8c109d64u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c109d66; /* mov.l @r15+,r9 */
L8c109d66: *stop=0x8c109d66u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c109d68; /* mov.l @r15+,r10 */
L8c109d68: *stop=0x8c109d68u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c109d6a; /* mov.l @r15+,r11 */
L8c109d6a: *stop=0x8c109d6au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c109d6c; /* mov.l @r15+,r12 */
L8c109d6c: *stop=0x8c109d6cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c109d6e; /* mov.l @r15+,r13 */
L8c109d6e: *stop=0x8c109d6eu;if(*pr!=entry_pr)return -3;*stop=0x8c109d70u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_slot_initialize_8c107ec0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c107ec0;
L8c107ec0: *stop=0x8c107ec0u;r[6]=6u;goto L8c107ec2; /* mov #0x6,r6 */
L8c107ec2: *stop=0x8c107ec2u;r[5]=r[4];goto L8c107ec4; /* mov r4,r5 */
L8c107ec4: *stop=0x8c107ec4u;r[6]<<=8;goto L8c107ec6; /* shll8 r6 */
L8c107ec6: *stop=0x8c107ec6u;r[6]+=r[5];goto L8c107ec8; /* add r5,r6 */
L8c107ec8: *stop=0x8c107ec8u;*sr=(*sr&~1u)|(r[5]>=r[6]);goto L8c107eca; /* cmp/hs r6,r5 */
L8c107eca: take=(*sr&1u);*stop=0x8c107eccu;r[4]=0u;if(take)goto L8c107ed6;goto L8c107ece; /* bt/s 0x8c107ed6 */
L8c107ece: *stop=0x8c107eceu;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[4],1);if(io_status)return io_status;goto L8c107ed0; /* mov.b r4,@r5 */
L8c107ed0: *stop=0x8c107ed0u;r[5]+=1u;goto L8c107ed2; /* add #0x1,r5 */
L8c107ed2: *stop=0x8c107ed2u;*sr=(*sr&~1u)|(r[5]>=r[6]);goto L8c107ed4; /* cmp/hs r6,r5 */
L8c107ed4: if(!(*sr&1u))goto L8c107ece;goto L8c107ed6; /* bf 0x8c107ece */
L8c107ed6: *stop=0x8c107ed6u;if(*pr!=entry_pr)return -3;*stop=0x8c107ed8u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_slot_initialize_8c108808(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c108808;
L8c108808: *stop=0x8c108808u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10880a; /* mov.l r14,@-r15 */
L8c10880a: *stop=0x8c10880au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10880c; /* mov.l r13,@-r15 */
L8c10880c: *stop=0x8c10880cu;r[13]=25u;goto L8c10880e; /* mov #0x19,r13 */
L8c10880e: *stop=0x8c10880eu;r[13]<<=8;goto L8c108810; /* shll8 r13 */
L8c108810: *stop=0x8c108810u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1088f4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c108812; /* mov.l 0x8c1088f4,r14 */
L8c108812: *stop=0x8c108812u;r[13]+=4294967280u;goto L8c108814; /* add #-0x10,r13 */
L8c108814: *stop=0x8c108814u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c108816; /* sts.l PR,@-r15 */
L8c108816: *stop=0x8c108816u;r[13]+=r[14];goto L8c108818; /* add r14,r13 */
L8c108818: *stop=0x8c108818u;*sr=(*sr&~1u)|(r[14]>=r[13]);goto L8c10881a; /* cmp/hs r13,r14 */
L8c10881a: if((*sr&1u))goto L8c10882c;goto L8c10881c; /* bt 0x8c10882c */
L8c10881c: target=0x8c1087d2u;*pr=0x8c108820u;*stop=0x8c10881eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c108820; /* bsr 0x8c1087d2 */
L8c108820: *stop=0x8c108820u;r[3]=6u;goto L8c108822; /* mov #0x6,r3 */
L8c108822: *stop=0x8c108822u;r[3]<<=8;goto L8c108824; /* shll8 r3 */
L8c108824: *stop=0x8c108824u;r[3]+=60u;goto L8c108826; /* add #0x3c,r3 */
L8c108826: *stop=0x8c108826u;r[14]+=r[3];goto L8c108828; /* add r3,r14 */
L8c108828: *stop=0x8c108828u;*sr=(*sr&~1u)|(r[14]>=r[13]);goto L8c10882a; /* cmp/hs r13,r14 */
L8c10882a: if(!(*sr&1u))goto L8c10881c;goto L8c10882c; /* bf 0x8c10881c */
L8c10882c: *stop=0x8c10882cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10882e; /* lds.l @r15+,PR */
L8c10882e: *stop=0x8c10882eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c108830; /* mov.l @r15+,r13 */
L8c108830: *stop=0x8c108830u;if(*pr!=entry_pr)return -3;*stop=0x8c108832u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_slot_initialize_8c1087d2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c1087d2;
L8c1087d2: *stop=0x8c1087d2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c1087d4; /* mov.l r14,@-r15 */
L8c1087d4: *stop=0x8c1087d4u;r[0]=6u;goto L8c1087d6; /* mov #0x6,r0 */
L8c1087d6: *stop=0x8c1087d6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c1087d8; /* sts.l PR,@-r15 */
L8c1087d8: *stop=0x8c1087d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1088ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c1087da; /* mov.l 0x8c1088ec,r3 */
L8c1087da: *stop=0x8c1087dau;r[0]<<=8;goto L8c1087dc; /* shll8 r0 */
L8c1087dc: *stop=0x8c1087dcu;r[14]=r[4];goto L8c1087de; /* mov r4,r14 */
L8c1087de: *stop=0x8c1087deu;r[0]+=48u;goto L8c1087e0; /* add #0x30,r0 */
L8c1087e0: target=r[3];*pr=0x8c1087e4u;*stop=0x8c1087e2u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c1087e4; /* jsr @r3 */
L8c1087e4: *stop=0x8c1087e4u;r[1]=6u;goto L8c1087e6; /* mov #0x6,r1 */
L8c1087e6: *stop=0x8c1087e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c1088f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c1087e8; /* mov.l 0x8c1088f0,r3 */
L8c1087e8: *stop=0x8c1087e8u;r[1]<<=8;goto L8c1087ea; /* shll8 r1 */
L8c1087ea: *stop=0x8c1087eau;r[1]+=r[14];goto L8c1087ec; /* add r14,r1 */
L8c1087ec: *stop=0x8c1087ecu;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[0],1);if(io_status)return io_status;goto L8c1087ee; /* mov.b r0,@r1 */
L8c1087ee: *stop=0x8c1087eeu;r[0]=6u;goto L8c1087f0; /* mov #0x6,r0 */
L8c1087f0: *stop=0x8c1087f0u;r[0]<<=8;goto L8c1087f2; /* shll8 r0 */
L8c1087f2: *stop=0x8c1087f2u;r[0]+=48u;goto L8c1087f4; /* add #0x30,r0 */
L8c1087f4: target=r[3];*pr=0x8c1087f8u;*stop=0x8c1087f6u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c1087f8; /* jsr @r3 */
L8c1087f8: *stop=0x8c1087f8u;r[1]=6u;goto L8c1087fa; /* mov #0x6,r1 */
L8c1087fa: *stop=0x8c1087fau;r[1]<<=8;goto L8c1087fc; /* shll8 r1 */
L8c1087fc: *stop=0x8c1087fcu;r[1]+=1u;goto L8c1087fe; /* add #0x1,r1 */
L8c1087fe: *stop=0x8c1087feu;r[1]+=r[14];goto L8c108800; /* add r14,r1 */
L8c108800: *stop=0x8c108800u;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[0],1);if(io_status)return io_status;goto L8c108802; /* mov.b r0,@r1 */
L8c108802: *stop=0x8c108802u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c108804; /* lds.l @r15+,PR */
L8c108804: *stop=0x8c108804u;if(*pr!=entry_pr)return -3;*stop=0x8c108806u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_slot_initialize_8c602ab4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602ab4;
L8c602ab4: *stop=0x8c602ab4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c602ab6; /* mov.l r14,@-r15 */
L8c602ab6: *stop=0x8c602ab6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c602ab8; /* sts.l PR,@-r15 */
L8c602ab8: *stop=0x8c602ab8u;r[15]+=4294967284u;goto L8c602aba; /* add #-0xc,r15 */
L8c602aba: *stop=0x8c602abau;{uint32_t loaded=rd(m,bus,&io_status,0x8c602b20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602abc; /* mov.l 0x8c602b20,r3 */
L8c602abc: target=r[3];*pr=0x8c602ac0u;*stop=0x8c602abeu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602ac0; /* jsr @r3 */
L8c602ac0: *stop=0x8c602ac0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602b24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602ac2; /* mov.l 0x8c602b24,r2 */
L8c602ac2: *stop=0x8c602ac2u;r[4]=r[15];goto L8c602ac4; /* mov r15,r4 */
L8c602ac4: *stop=0x8c602ac4u;r[14]=r[0];goto L8c602ac6; /* mov r0,r14 */
L8c602ac6: *stop=0x8c602ac6u;r[5]=r[0];goto L8c602ac8; /* mov r0,r5 */
L8c602ac8: target=r[2];*pr=0x8c602accu;*stop=0x8c602acau;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c602acc; /* jsr @r2 */
L8c602acc: *stop=0x8c602accu;r[4]=r[0];goto L8c602ace; /* mov r0,r4 */
L8c602ace: *stop=0x8c602aceu;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c602ad0; /* cmp/pz r4 */
L8c602ad0: if(!(*sr&1u))goto L8c602afe;goto L8c602ad2; /* bf 0x8c602afe */
L8c602ad2: *stop=0x8c602ad2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c602ad4; /* mov.l @r14,r0 */
L8c602ad4: *stop=0x8c602ad4u;*sr=(*sr&~1u)|(r[0]==0x0000000eu);goto L8c602ad6; /* cmp/eq #0xe,r0 */
L8c602ad6: if(!(*sr&1u))goto L8c602afe;goto L8c602ad8; /* bf 0x8c602afe */
L8c602ad8: *stop=0x8c602ad8u;if(!valid_access(r[15]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;goto L8c602ada; /* mov.w @(0x4,r15),r0 */
L8c602ada: *stop=0x8c602adau;r[0]=r[0]&65535u;goto L8c602adc; /* extu.w r0,r0 */
L8c602adc: *stop=0x8c602adcu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c602ade; /* cmp/eq #0x1,r0 */
L8c602ade: if(!(*sr&1u))goto L8c602afe;goto L8c602ae0; /* bf 0x8c602afe */
L8c602ae0: *stop=0x8c602ae0u;if(!valid_access(r[15]+6u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x6u,2);if(io_status)return io_status;goto L8c602ae2; /* mov.w @(0x6,r15),r0 */
L8c602ae2: *stop=0x8c602ae2u;r[3]=sx(m,bus,&io_status,0x8c602b12u,2);if(io_status)return io_status;goto L8c602ae4; /* mov.w 0x8c602b12,r3 */
L8c602ae4: *stop=0x8c602ae4u;r[0]=r[0]&65535u;goto L8c602ae6; /* extu.w r0,r0 */
L8c602ae6: *stop=0x8c602ae6u;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c602ae8; /* cmp/eq r3,r0 */
L8c602ae8: if(!(*sr&1u))goto L8c602afe;goto L8c602aea; /* bf 0x8c602afe */
L8c602aea: *stop=0x8c602aeau;if(!valid_access(r[15]+8u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x8u,2);if(io_status)return io_status;goto L8c602aec; /* mov.w @(0x8,r15),r0 */
L8c602aec: *stop=0x8c602aecu;r[0]=r[0]&65535u;goto L8c602aee; /* extu.w r0,r0 */
L8c602aee: *stop=0x8c602aeeu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c602af0; /* cmp/eq #0x1,r0 */
L8c602af0: if(!(*sr&1u))goto L8c602afe;goto L8c602af2; /* bf 0x8c602afe */
L8c602af2: *stop=0x8c602af2u;if(!valid_access(r[15]+10u,1u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0xau,1);if(io_status)return io_status;goto L8c602af4; /* mov.b @(0xa,r15),r0 */
L8c602af4: *stop=0x8c602af4u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c602af6; /* tst r0,r0 */
L8c602af6: if(!(*sr&1u))goto L8c602afe;goto L8c602af8; /* bf 0x8c602afe */
L8c602af8: *stop=0x8c602af8u;if(!valid_access(r[15]+11u,1u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0xbu,1);if(io_status)return io_status;goto L8c602afa; /* mov.b @(0xb,r15),r0 */
L8c602afa: *stop=0x8c602afau;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c602afc; /* tst r0,r0 */
L8c602afc: if((*sr&1u))goto L8c602b08;goto L8c602afe; /* bt 0x8c602b08 */
L8c602afe: *stop=0x8c602afeu;r[0]=0u;goto L8c602b00; /* mov #0x0,r0 */
L8c602b00: *stop=0x8c602b00u;r[15]+=12u;goto L8c602b02; /* add #0xc,r15 */
L8c602b02: *stop=0x8c602b02u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c602b04; /* lds.l @r15+,PR */
L8c602b04: *stop=0x8c602b04u;if(*pr!=entry_pr)return -3;*stop=0x8c602b06u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c602b08: *stop=0x8c602b08u;r[0]=1u;goto L8c602b0a; /* mov #0x1,r0 */
L8c602b0a: *stop=0x8c602b0au;r[15]+=12u;goto L8c602b0c; /* add #0xc,r15 */
L8c602b0c: *stop=0x8c602b0cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c602b0e; /* lds.l @r15+,PR */
L8c602b0e: *stop=0x8c602b0eu;if(*pr!=entry_pr)return -3;*stop=0x8c602b10u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_slot_initialize_8c602998(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602998;
L8c602998: *stop=0x8c602998u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c60299a; /* mov.l r14,@-r15 */
L8c60299a: *stop=0x8c60299au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c60299c; /* sts.l PR,@-r15 */
L8c60299c: *stop=0x8c60299cu;r[15]+=4294967292u;goto L8c60299e; /* add #-0x4,r15 */
L8c60299e: *stop=0x8c60299eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6029f8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6029a0; /* mov.l 0x8c6029f8,r0 */
L8c6029a0: *stop=0x8c6029a0u;r[14]=r[4];goto L8c6029a2; /* mov r4,r14 */
L8c6029a2: *stop=0x8c6029a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6029fcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6029a4; /* mov.l 0x8c6029fc,r3 */
L8c6029a4: *stop=0x8c6029a4u;if(!valid_access(r[0]+r[14],1u))return -3;r[4]=sx(m,bus,&io_status,r[14]+r[0],1);if(io_status)return io_status;goto L8c6029a6; /* mov.b @(r0,r14),r4 */
L8c6029a6: target=r[3];*pr=0x8c6029aau;*stop=0x8c6029a8u;r[4]=r[4]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6029aa; /* jsr @r3 */
L8c6029aa: *stop=0x8c6029aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6029fcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6029ac; /* mov.l 0x8c6029fc,r3 */
L8c6029ac: *stop=0x8c6029acu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c6029ae; /* mov.l r0,@r15 */
L8c6029ae: target=r[3];*pr=0x8c6029b2u;*stop=0x8c6029b0u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6029b2; /* jsr @r3 */
L8c6029b2: *stop=0x8c6029b2u;r[5]=r[0];goto L8c6029b4; /* mov r0,r5 */
L8c6029b4: *stop=0x8c6029b4u;r[0]=r[14];goto L8c6029b6; /* mov r14,r0 */
L8c6029b6: *stop=0x8c6029b6u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c6029b8; /* cmp/eq #0x2,r0 */
L8c6029b8: if((*sr&1u))goto L8c6029ca;goto L8c6029ba; /* bt 0x8c6029ca */
L8c6029ba: *stop=0x8c6029bau;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c6029bc; /* cmp/eq #0x8,r0 */
L8c6029bc: if((*sr&1u))goto L8c6029ca;goto L8c6029be; /* bt 0x8c6029ca */
L8c6029be: *stop=0x8c6029beu;*sr=(*sr&~1u)|(r[0]==0x0000000eu);goto L8c6029c0; /* cmp/eq #0xe,r0 */
L8c6029c0: if((*sr&1u))goto L8c6029ca;goto L8c6029c2; /* bt 0x8c6029ca */
L8c6029c2: *stop=0x8c6029c2u;*sr=(*sr&~1u)|(r[0]==0x00000014u);goto L8c6029c4; /* cmp/eq #0x14,r0 */
L8c6029c4: if((*sr&1u))goto L8c6029ca;goto L8c6029c6; /* bt 0x8c6029ca */
L8c6029c6: *stop=0x8c6029c8u;goto L8c6029ce; /* bra 0x8c6029ce */
L8c6029ca: *stop=0x8c6029ccu;r[4]=1u;goto L8c6029d0; /* bra 0x8c6029d0 */
L8c6029ce: *stop=0x8c6029ceu;r[4]=0u;goto L8c6029d0; /* mov #0x0,r4 */
L8c6029d0: *stop=0x8c6029d0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6029d2; /* mov.l @r15,r0 */
L8c6029d2: *stop=0x8c6029d2u;r[3]=r[5];goto L8c6029d4; /* mov r5,r3 */
L8c6029d4: *stop=0x8c6029d4u;r[3]+=18u;goto L8c6029d6; /* add #0x12,r3 */
L8c6029d6: *stop=0x8c6029d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602a00u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6029d8; /* mov.l 0x8c602a00,r1 */
L8c6029d8: *stop=0x8c6029d8u;r[0]+=18u;goto L8c6029da; /* add #0x12,r0 */
L8c6029da: *stop=0x8c6029dau;if(!valid_access(r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;goto L8c6029dc; /* mov.b @r3,r2 */
L8c6029dc: *stop=0x8c6029dcu;if(!valid_access(r[0]+r[4],1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+r[0],1);if(io_status)return io_status;goto L8c6029de; /* mov.b @(r0,r4),r0 */
L8c6029de: *stop=0x8c6029deu;r[2]=r[2]&255u;goto L8c6029e0; /* extu.b r2,r2 */
L8c6029e0: *stop=0x8c6029e0u;r[0]=r[0]&255u;goto L8c6029e2; /* extu.b r0,r0 */
L8c6029e2: *stop=0x8c6029e2u;r[0]<<=2;goto L8c6029e4; /* shll2 r0 */
L8c6029e4: *stop=0x8c6029e4u;r[0]+=r[2];goto L8c6029e6; /* add r2,r0 */
L8c6029e6: *stop=0x8c6029e6u;if(!valid_access(r[0]+r[1],1u))return -3;r[0]=sx(m,bus,&io_status,r[1]+r[0],1);if(io_status)return io_status;goto L8c6029e8; /* mov.b @(r0,r1),r0 */
L8c6029e8: *stop=0x8c6029e8u;r[0]=r[0]&255u;goto L8c6029ea; /* extu.b r0,r0 */
L8c6029ea: *stop=0x8c6029eau;r[15]+=4u;goto L8c6029ec; /* add #0x4,r15 */
L8c6029ec: *stop=0x8c6029ecu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6029ee; /* lds.l @r15+,PR */
L8c6029ee: *stop=0x8c6029eeu;if(*pr!=entry_pr)return -3;*stop=0x8c6029f0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_slot_initialize_8c646c8a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c646c8a;
L8c646c8a: *stop=0x8c646c8au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c646c8c; /* mov.l r14,@-r15 */
L8c646c8c: *stop=0x8c646c8cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c646c8e; /* sts.l PR,@-r15 */
L8c646c8e: *stop=0x8c646c8eu;r[15]+=4294967292u;goto L8c646c90; /* add #-0x4,r15 */
L8c646c90: *stop=0x8c646c90u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c646c92; /* mov.l r5,@r15 */
L8c646c92: *stop=0x8c646c92u;r[5]=4u;goto L8c646c94; /* mov #0x4,r5 */
L8c646c94: *stop=0x8c646c94u;r[14]=r[4];goto L8c646c96; /* mov r4,r14 */
L8c646c96: target=0x8c646b42u;*pr=0x8c646c9au;*stop=0x8c646c98u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646c9a; /* bsr 0x8c646b42 */
L8c646c9a: *stop=0x8c646c9au;r[4]=r[0];goto L8c646c9c; /* mov r0,r4 */
L8c646c9c: *stop=0x8c646c9cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c646c9e; /* tst r4,r4 */
L8c646c9e: if(!(*sr&1u))goto L8c646caa;goto L8c646ca0; /* bf 0x8c646caa */
L8c646ca0: *stop=0x8c646ca0u;r[0]=4294967295u;goto L8c646ca2; /* mov #-0x1,r0 */
L8c646ca2: *stop=0x8c646ca2u;r[15]+=4u;goto L8c646ca4; /* add #0x4,r15 */
L8c646ca4: *stop=0x8c646ca4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c646ca6; /* lds.l @r15+,PR */
L8c646ca6: *stop=0x8c646ca6u;if(*pr!=entry_pr)return -3;*stop=0x8c646ca8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c646caa: *stop=0x8c646caau;r[5]=r[4];goto L8c646cac; /* mov r4,r5 */
L8c646cac: *stop=0x8c646cacu;r[5]+=3u;goto L8c646cae; /* add #0x3,r5 */
L8c646cae: *stop=0x8c646caeu;if(!valid_access(r[4],1u))return -3;r[3]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;goto L8c646cb0; /* mov.b @r4,r3 */
L8c646cb0: *stop=0x8c646cb0u;r[3]=r[3]&255u;goto L8c646cb2; /* extu.b r3,r3 */
L8c646cb2: *stop=0x8c646cb2u;r[3]+=1u;goto L8c646cb4; /* add #0x1,r3 */
L8c646cb4: *stop=0x8c646cb4u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[3],2);if(io_status)return io_status;goto L8c646cb6; /* mov.w r3,@r14 */
L8c646cb6: *stop=0x8c646cb6u;if(!valid_access(r[4]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x1u,1);if(io_status)return io_status;goto L8c646cb8; /* mov.b @(0x1,r4),r0 */
L8c646cb8: *stop=0x8c646cb8u;r[0]=r[0]&255u;goto L8c646cba; /* extu.b r0,r0 */
L8c646cba: *stop=0x8c646cbau;r[0]+=1u;goto L8c646cbc; /* add #0x1,r0 */
L8c646cbc: *stop=0x8c646cbcu;r[0]<<=2;goto L8c646cbe; /* shll2 r0 */
L8c646cbe: *stop=0x8c646cbeu;r[0]<<=2;goto L8c646cc0; /* shll2 r0 */
L8c646cc0: *stop=0x8c646cc0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c646cc2; /* shll r0 */
L8c646cc2: *stop=0x8c646cc2u;if(!valid_access(r[14]+2u,2u))return -3;wr(m,bus,&io_status,r[14]+0x2u,r[0],2);if(io_status)return io_status;goto L8c646cc4; /* mov.w r0,@(0x2,r14) */
L8c646cc4: *stop=0x8c646cc4u;if(!valid_access(r[4]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x2u,1);if(io_status)return io_status;goto L8c646cc6; /* mov.b @(0x2,r4),r0 */
L8c646cc6: *stop=0x8c646cc6u;r[0]=r[0]&255u;goto L8c646cc8; /* extu.b r0,r0 */
L8c646cc8: *stop=0x8c646cc8u;r[0]>>=2;goto L8c646cca; /* shlr2 r0 */
L8c646cca: *stop=0x8c646ccau;r[0]>>=2;goto L8c646ccc; /* shlr2 r0 */
L8c646ccc: *stop=0x8c646cccu;r[0]&=0xfu;goto L8c646cce; /* and #0xf,r0 */
L8c646cce: *stop=0x8c646cceu;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c646cd0; /* mov.w r0,@(0x4,r14) */
L8c646cd0: *stop=0x8c646cd0u;if(!valid_access(r[5],1u))return -3;r[0]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c646cd2; /* mov.b @r5,r0 */
L8c646cd2: *stop=0x8c646cd2u;r[0]=r[0]&255u;goto L8c646cd4; /* extu.b r0,r0 */
L8c646cd4: *stop=0x8c646cd4u;r[0]>>=2;goto L8c646cd6; /* shlr2 r0 */
L8c646cd6: *stop=0x8c646cd6u;r[0]>>=2;goto L8c646cd8; /* shlr2 r0 */
L8c646cd8: *stop=0x8c646cd8u;r[0]>>=2;goto L8c646cda; /* shlr2 r0 */
L8c646cda: *stop=0x8c646cdau;r[0]&=0x3u;goto L8c646cdc; /* and #0x3,r0 */
L8c646cdc: *stop=0x8c646cdcu;if(!valid_access(r[14]+6u,1u))return -3;wr(m,bus,&io_status,r[14]+0x6u,r[0],1);if(io_status)return io_status;goto L8c646cde; /* mov.b r0,@(0x6,r14) */
L8c646cde: *stop=0x8c646cdeu;if(!valid_access(r[5],1u))return -3;r[0]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c646ce0; /* mov.b @r5,r0 */
L8c646ce0: *stop=0x8c646ce0u;r[0]=r[0]&255u;goto L8c646ce2; /* extu.b r0,r0 */
L8c646ce2: *stop=0x8c646ce2u;r[0]>>=2;goto L8c646ce4; /* shlr2 r0 */
L8c646ce4: *stop=0x8c646ce4u;r[0]>>=2;goto L8c646ce6; /* shlr2 r0 */
L8c646ce6: *stop=0x8c646ce6u;*sr=(*sr&~1u)|(r[0]&1u);r[0]>>=1;goto L8c646ce8; /* shlr r0 */
L8c646ce8: *stop=0x8c646ce8u;r[0]&=0x1u;goto L8c646cea; /* and #0x1,r0 */
L8c646cea: *stop=0x8c646ceau;if(!valid_access(r[14]+7u,1u))return -3;wr(m,bus,&io_status,r[14]+0x7u,r[0],1);if(io_status)return io_status;goto L8c646cec; /* mov.b r0,@(0x7,r14) */
L8c646cec: *stop=0x8c646cecu;r[0]=0u;goto L8c646cee; /* mov #0x0,r0 */
L8c646cee: *stop=0x8c646ceeu;r[15]+=4u;goto L8c646cf0; /* add #0x4,r15 */
L8c646cf0: *stop=0x8c646cf0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c646cf2; /* lds.l @r15+,PR */
L8c646cf2: *stop=0x8c646cf2u;if(*pr!=entry_pr)return -3;*stop=0x8c646cf4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
