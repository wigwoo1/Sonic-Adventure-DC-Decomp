/* Polygon clipping/projection and header acquisition; names inferred.
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
int sa_sh4_sqrt(uint32_t,uint32_t*,uint32_t*);
static int valid_access(uint32_t a,unsigned n){
 if(valid_ram(a,n))return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_polygon_near_clip_8c64f840(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64f840;
L8c64f840: *stop=0x8c64f840u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64f842; /* mov.l r14,@-r15 */
L8c64f842: *stop=0x8c64f842u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64f844; /* mov.l r13,@-r15 */
L8c64f844: *stop=0x8c64f844u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64f846; /* mov.l r12,@-r15 */
L8c64f846: *stop=0x8c64f846u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64f848; /* mov.l r11,@-r15 */
L8c64f848: *stop=0x8c64f848u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64f84a; /* mov.l r10,@-r15 */
L8c64f84a: *stop=0x8c64f84au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64f84c; /* mov.l r9,@-r15 */
L8c64f84c: *stop=0x8c64f84cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64f84e; /* mov.l r8,@-r15 */
L8c64f84e: *stop=0x8c64f84eu;r[15]+=4294967260u;goto L8c64f850; /* add #-0x24,r15 */
L8c64f850: *stop=0x8c64f850u;r[12]=rd(m,bus,&io_status,0x8c64fae0u,4);if(io_status)return io_status;goto L8c64f852; /* mov.l 0x8c64fae0,r12 */
L8c64f852: *stop=0x8c64f852u;r[13]=0u;goto L8c64f854; /* mov #0x0,r13 */
L8c64f854: *stop=0x8c64f854u;r[9]=rd(m,bus,&io_status,0x8c64fae4u,4);if(io_status)return io_status;goto L8c64f856; /* mov.l 0x8c64fae4,r9 */
L8c64f856: *stop=0x8c64f856u;r[10]=r[5];goto L8c64f858; /* mov r5,r10 */
L8c64f858: *stop=0x8c64f858u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c64f85a; /* cmp/pl r5 */
L8c64f85a: *stop=0x8c64f85au;r[10]+=4294967295u;goto L8c64f85c; /* add #-0x1,r10 */
L8c64f85c: *stop=0x8c64f85cu;r[14]=r[13];goto L8c64f85e; /* mov r13,r14 */
L8c64f85e: take=(*sr&1u);*stop=0x8c64f860u;r[9]&=r[7];if(take)goto L8c64f866;goto L8c64f862; /* bt/s 0x8c64f866 */
L8c64f862: *stop=0x8c64f864u;goto L8c64facc; /* bra 0x8c64facc */
L8c64f866: *stop=0x8c64f866u;r[2]=r[10];goto L8c64f868; /* mov r10,r2 */
L8c64f868: *stop=0x8c64f868u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c64f86a; /* shll r2 */
L8c64f86a: *stop=0x8c64f86au;r[3]=r[10];goto L8c64f86c; /* mov r10,r3 */
L8c64f86c: *stop=0x8c64f86cu;r[2]+=r[3];goto L8c64f86e; /* add r3,r2 */
L8c64f86e: *stop=0x8c64f86eu;if(!valid_access(r[4],4u))return -3;r[7]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c64f870; /* mov.l @r4,r7 */
L8c64f870: *stop=0x8c64f870u;r[2]<<=2;goto L8c64f872; /* shll2 r2 */
L8c64f872: *stop=0x8c64f872u;r[0]=8u;goto L8c64f874; /* mov #0x8,r0 */
L8c64f874: *stop=0x8c64f874u;r[3]=r[14];goto L8c64f876; /* mov r14,r3 */
L8c64f876: *stop=0x8c64f876u;r[2]+=r[7];goto L8c64f878; /* add r7,r2 */
L8c64f878: *stop=0x8c64f878u;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c64f87a; /* fmov.s @r12,fr3 */
L8c64f87a: *stop=0x8c64f87au;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c64f87c; /* fmov.s @(r0,r2),fr2 */
L8c64f87c: *stop=0x8c64f87cu;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c64f87e; /* shll r3 */
L8c64f87e: *stop=0x8c64f87eu;r[2]=r[14];goto L8c64f880; /* mov r14,r2 */
L8c64f880: *stop=0x8c64f880u;r[3]+=r[2];goto L8c64f882; /* add r2,r3 */
L8c64f882: *stop=0x8c64f882u;r[3]<<=2;goto L8c64f884; /* shll2 r3 */
L8c64f884: *stop=0x8c64f884u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c64f886; /* fsub fr3,fr2 */
L8c64f886: *stop=0x8c64f886u;r[3]+=r[7];goto L8c64f888; /* add r7,r3 */
L8c64f888: *stop=0x8c64f888u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[3]);if(io_status)return io_status;goto L8c64f88a; /* fmov.s @(r0,r3),fr1 */
L8c64f88a: *stop=0x8c64f88au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c64f88c; /* fsub fr3,fr1 */
L8c64f88c: *stop=0x8c64f88cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c64f88e; /* fldi0 fr3 */
L8c64f88e: *stop=0x8c64f88eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64f890; /* fmul fr1,fr2 */
L8c64f890: *stop=0x8c64f890u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c64f892; /* fcmp/gt fr2,fr3 */
L8c64f892: if((*sr&1u))goto L8c64f898;goto L8c64f894; /* bt 0x8c64f898 */
L8c64f894: *stop=0x8c64f896u;goto L8c64fa44; /* bra 0x8c64fa44 */
L8c64f898: *stop=0x8c64f898u;r[8]=r[10];goto L8c64f89a; /* mov r10,r8 */
L8c64f89a: *stop=0x8c64f89au;*sr=(*sr&~1u)|(r[8]>>31);r[8]<<=1;goto L8c64f89c; /* shll r8 */
L8c64f89c: *stop=0x8c64f89cu;r[3]=r[10];goto L8c64f89e; /* mov r10,r3 */
L8c64f89e: *stop=0x8c64f89eu;if(!valid_access(r[4],4u))return -3;r[7]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c64f8a0; /* mov.l @r4,r7 */
L8c64f8a0: *stop=0x8c64f8a0u;r[8]+=r[3];goto L8c64f8a2; /* add r3,r8 */
L8c64f8a2: *stop=0x8c64f8a2u;r[11]=r[14];goto L8c64f8a4; /* mov r14,r11 */
L8c64f8a4: *stop=0x8c64f8a4u;r[2]=r[7];goto L8c64f8a6; /* mov r7,r2 */
L8c64f8a6: *stop=0x8c64f8a6u;r[8]<<=2;goto L8c64f8a8; /* shll2 r8 */
L8c64f8a8: *stop=0x8c64f8a8u;r[3]=r[14];goto L8c64f8aa; /* mov r14,r3 */
L8c64f8aa: *stop=0x8c64f8aau;*sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c64f8ac; /* shll r11 */
L8c64f8ac: *stop=0x8c64f8acu;r[0]=8u;goto L8c64f8ae; /* mov #0x8,r0 */
L8c64f8ae: *stop=0x8c64f8aeu;r[2]+=r[8];goto L8c64f8b0; /* add r8,r2 */
L8c64f8b0: *stop=0x8c64f8b0u;r[11]+=r[3];goto L8c64f8b2; /* add r3,r11 */
L8c64f8b2: *stop=0x8c64f8b2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64f8b4; /* mov.l r2,@r15 */
L8c64f8b4: *stop=0x8c64f8b4u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[2]);if(io_status)return io_status;goto L8c64f8b6; /* fmov.s @(r0,r2),fr4 */
L8c64f8b6: *stop=0x8c64f8b6u;r[11]<<=2;goto L8c64f8b8; /* shll2 r11 */
L8c64f8b8: *stop=0x8c64f8b8u;r[2]=r[7];goto L8c64f8ba; /* mov r7,r2 */
L8c64f8ba: *stop=0x8c64f8bau;r[2]+=r[11];goto L8c64f8bc; /* add r11,r2 */
L8c64f8bc: *stop=0x8c64f8bcu;r[0]=8u;goto L8c64f8be; /* mov #0x8,r0 */
L8c64f8be: *stop=0x8c64f8beu;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c64f8c0; /* fmov.s @r12,fr2 */
L8c64f8c0: *stop=0x8c64f8c0u;r[7]=r[13];goto L8c64f8c2; /* mov r13,r7 */
L8c64f8c2: *stop=0x8c64f8c2u;*sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c64f8c4; /* shll r7 */
L8c64f8c4: *stop=0x8c64f8c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c64f8c6; /* fsub fr4,fr2 */
L8c64f8c6: *stop=0x8c64f8c6u;r[3]=r[13];goto L8c64f8c8; /* mov r13,r3 */
L8c64f8c8: *stop=0x8c64f8c8u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[2],4);if(io_status)return io_status;goto L8c64f8ca; /* mov.l r2,@(0x20,r15) */
L8c64f8ca: *stop=0x8c64f8cau;r[7]+=r[3];goto L8c64f8cc; /* add r3,r7 */
L8c64f8cc: *stop=0x8c64f8ccu;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[2]);if(io_status)return io_status;goto L8c64f8ce; /* fmov.s @(r0,r2),fr1 */
L8c64f8ce: *stop=0x8c64f8ceu;r[7]<<=2;goto L8c64f8d0; /* shll2 r7 */
L8c64f8d0: *stop=0x8c64f8d0u;if(!valid_access(r[6],4u))return -3;r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c64f8d2; /* mov.l @r6,r2 */
L8c64f8d2: *stop=0x8c64f8d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[4],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c64f8d4; /* fsub fr4,fr1 */
L8c64f8d4: *stop=0x8c64f8d4u;fcopy(f,4u,2u);goto L8c64f8d6; /* fmov fr2,fr4 */
L8c64f8d6: *stop=0x8c64f8d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[1],&f->fpscr,&fr[4]);if(status)return status;}goto L8c64f8d8; /* fdiv fr1,fr4 */
L8c64f8d8: *stop=0x8c64f8d8u;if(!valid_access(r[15]+32u,4u))return -3;r[1]=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;goto L8c64f8da; /* mov.l @(0x20,r15),r1 */
L8c64f8da: *stop=0x8c64f8dau;r[2]+=r[7];goto L8c64f8dc; /* add r7,r2 */
L8c64f8dc: *stop=0x8c64f8dcu;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c64f8de; /* mov.l @r15,r3 */
L8c64f8de: *stop=0x8c64f8deu;r[0]=4u;goto L8c64f8e0; /* mov #0x4,r0 */
L8c64f8e0: *stop=0x8c64f8e0u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c64f8e2; /* fmov.s @r1,fr2 */
L8c64f8e2: *stop=0x8c64f8e2u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[3]);if(io_status)return io_status;goto L8c64f8e4; /* fmov.s @r3,fr5 */
L8c64f8e4: *stop=0x8c64f8e4u;fcopy(f,0u,4u);goto L8c64f8e6; /* fmov fr4,fr0 */
L8c64f8e6: *stop=0x8c64f8e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c64f8e8; /* fsub fr5,fr2 */
L8c64f8e8: *stop=0x8c64f8e8u;fcopy(f,1u,5u);goto L8c64f8ea; /* fmov fr5,fr1 */
L8c64f8ea: *stop=0x8c64f8eau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64f8ec; /* fmac fr0,fr2,fr1 */
L8c64f8ec: *stop=0x8c64f8ecu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c64f8ee; /* fmov.s fr1,@r2 */
L8c64f8ee: *stop=0x8c64f8eeu;if(!valid_access(r[4],4u))return -3;r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c64f8f0; /* mov.l @r4,r2 */
L8c64f8f0: *stop=0x8c64f8f0u;if(!valid_access(r[6],4u))return -3;r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c64f8f2; /* mov.l @r6,r3 */
L8c64f8f2: *stop=0x8c64f8f2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c64f8f4; /* mov.l r2,@r15 */
L8c64f8f4: *stop=0x8c64f8f4u;r[2]+=r[8];goto L8c64f8f6; /* add r8,r2 */
L8c64f8f6: *stop=0x8c64f8f6u;r[3]+=r[7];goto L8c64f8f8; /* add r7,r3 */
L8c64f8f8: *stop=0x8c64f8f8u;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[2],4);if(io_status)return io_status;goto L8c64f8fa; /* mov.l r2,@(0x1c,r15) */
L8c64f8fa: *stop=0x8c64f8fau;if(!valid_access(r[15],4u))return -3;r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c64f8fc; /* mov.l @r15,r1 */
L8c64f8fc: *stop=0x8c64f8fcu;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[2]);if(io_status)return io_status;goto L8c64f8fe; /* fmov.s @(r0,r2),fr5 */
L8c64f8fe: *stop=0x8c64f8feu;r[0]=4u;goto L8c64f900; /* mov #0x4,r0 */
L8c64f900: *stop=0x8c64f900u;r[11]+=r[1];goto L8c64f902; /* add r1,r11 */
L8c64f902: *stop=0x8c64f902u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c64f904; /* fmov.s @(r0,r11),fr2 */
L8c64f904: *stop=0x8c64f904u;r[11]=r[10];goto L8c64f906; /* mov r10,r11 */
L8c64f906: *stop=0x8c64f906u;fcopy(f,1u,5u);goto L8c64f908; /* fmov fr5,fr1 */
L8c64f908: *stop=0x8c64f908u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c64f90a; /* fsub fr5,fr2 */
L8c64f90a: *stop=0x8c64f90au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[1],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64f90c; /* fmac fr0,fr2,fr1 */
L8c64f90c: *stop=0x8c64f90cu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[3]);if(io_status)return io_status;goto L8c64f90e; /* fmov fr1,@(r0,r3) */
L8c64f90e: *stop=0x8c64f90eu;r[0]=8u;goto L8c64f910; /* mov #0x8,r0 */
L8c64f910: *stop=0x8c64f910u;if(!valid_access(r[6],4u))return -3;r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c64f912; /* mov.l @r6,r3 */
L8c64f912: *stop=0x8c64f912u;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c64f914; /* fmov.s @r12,fr2 */
L8c64f914: *stop=0x8c64f914u;r[7]+=r[3];goto L8c64f916; /* add r3,r7 */
L8c64f916: *stop=0x8c64f916u;if(!valid_access(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c64f918; /* fmov fr2,@(r0,r7) */
L8c64f918: *stop=0x8c64f918u;*sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c64f91a; /* tst r9,r9 */
L8c64f91a: take=(*sr&1u);*stop=0x8c64f91cu;r[11]<<=2;if(take)goto L8c64f97e;goto L8c64f91e; /* bt/s 0x8c64f97e */
L8c64f91e: *stop=0x8c64f91eu;if(!valid_access(r[4]+8u,4u))return -3;r[8]=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;goto L8c64f920; /* mov.l @(0x8,r4),r8 */
L8c64f920: *stop=0x8c64f920u;r[10]=r[14];goto L8c64f922; /* mov r14,r10 */
L8c64f922: *stop=0x8c64f922u;r[10]<<=2;goto L8c64f924; /* shll2 r10 */
L8c64f924: *stop=0x8c64f924u;fcopy(f,0u,4u);goto L8c64f926; /* fmov fr4,fr0 */
L8c64f926: *stop=0x8c64f926u;r[3]=r[8];goto L8c64f928; /* mov r8,r3 */
L8c64f928: *stop=0x8c64f928u;r[3]+=r[11];goto L8c64f92a; /* add r11,r3 */
L8c64f92a: *stop=0x8c64f92au;r[7]=r[13];goto L8c64f92c; /* mov r13,r7 */
L8c64f92c: *stop=0x8c64f92cu;if(!valid_access(r[6]+8u,4u))return -3;r[2]=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;goto L8c64f92e; /* mov.l @(0x8,r6),r2 */
L8c64f92e: *stop=0x8c64f92eu;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c64f930; /* mov.l r3,@(0x18,r15) */
L8c64f930: *stop=0x8c64f930u;r[7]<<=2;goto L8c64f932; /* shll2 r7 */
L8c64f932: *stop=0x8c64f932u;if(!valid_access(r[3],2u))return -3;r[1]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c64f934; /* mov.w @r3,r1 */
L8c64f934: *stop=0x8c64f934u;r[3]=r[8];goto L8c64f936; /* mov r8,r3 */
L8c64f936: *stop=0x8c64f936u;r[3]+=r[10];goto L8c64f938; /* add r10,r3 */
L8c64f938: *stop=0x8c64f938u;*fpul=r[1];goto L8c64f93a; /* lds r1,FPUL */
L8c64f93a: *stop=0x8c64f93au;r[2]+=r[7];goto L8c64f93c; /* add r7,r2 */
L8c64f93c: *stop=0x8c64f93cu;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[1],2);if(io_status)return io_status;goto L8c64f93e; /* mov.w r1,@r15 */
L8c64f93e: *stop=0x8c64f93eu;if(!valid_access(r[3],2u))return -3;r[1]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c64f940; /* mov.w @r3,r1 */
L8c64f940: *stop=0x8c64f940u;if(!valid_access(r[15],2u))return -3;r[3]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c64f942; /* mov.w @r15,r3 */
L8c64f942: *stop=0x8c64f942u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c64f944; /* float FPUL,fr2 */
L8c64f944: *stop=0x8c64f944u;r[1]-=r[3];goto L8c64f946; /* sub r3,r1 */
L8c64f946: *stop=0x8c64f946u;*fpul=r[1];goto L8c64f948; /* lds r1,FPUL */
L8c64f948: *stop=0x8c64f948u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c64f94a; /* float FPUL,fr1 */
L8c64f94a: *stop=0x8c64f94au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64f94c; /* fmac fr0,fr1,fr2 */
L8c64f94c: *stop=0x8c64f94cu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c64f94e; /* ftrc fr2,FPUL */
L8c64f94e: *stop=0x8c64f94eu;r[3]=*fpul;goto L8c64f950; /* sts FPUL,r3 */
L8c64f950: *stop=0x8c64f950u;if(!valid_access(r[2],2u))return -3;wr(m,bus,&io_status,r[2],r[3],2);if(io_status)return io_status;goto L8c64f952; /* mov.w r3,@r2 */
L8c64f952: *stop=0x8c64f952u;if(!valid_access(r[4]+8u,4u))return -3;r[8]=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;goto L8c64f954; /* mov.l @(0x8,r4),r8 */
L8c64f954: *stop=0x8c64f954u;if(!valid_access(r[6]+8u,4u))return -3;r[2]=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;goto L8c64f956; /* mov.l @(0x8,r6),r2 */
L8c64f956: *stop=0x8c64f956u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64f958; /* mov.l r8,@r15 */
L8c64f958: *stop=0x8c64f958u;r[8]+=r[11];goto L8c64f95a; /* add r11,r8 */
L8c64f95a: *stop=0x8c64f95au;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[8],4);if(io_status)return io_status;goto L8c64f95c; /* mov.l r8,@(0x14,r15) */
L8c64f95c: *stop=0x8c64f95cu;r[7]+=r[2];goto L8c64f95e; /* add r2,r7 */
L8c64f95e: *stop=0x8c64f95eu;if(!valid_access(r[8]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[8]+0x2u,2);if(io_status)return io_status;goto L8c64f960; /* mov.w @(0x2,r8),r0 */
L8c64f960: *stop=0x8c64f960u;if(!valid_access(r[15],4u))return -3;r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c64f962; /* mov.l @r15,r2 */
L8c64f962: *stop=0x8c64f962u;r[8]=r[0];goto L8c64f964; /* mov r0,r8 */
L8c64f964: *stop=0x8c64f964u;r[3]=(uint32_t)(int32_t)(int16_t)r[8];goto L8c64f966; /* exts.w r8,r3 */
L8c64f966: *stop=0x8c64f966u;r[10]+=r[2];goto L8c64f968; /* add r2,r10 */
L8c64f968: *stop=0x8c64f968u;*fpul=r[3];goto L8c64f96a; /* lds r3,FPUL */
L8c64f96a: *stop=0x8c64f96au;if(!valid_access(r[10]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c64f96c; /* mov.w @(0x2,r10),r0 */
L8c64f96c: *stop=0x8c64f96cu;r[3]=(uint32_t)(int32_t)(int16_t)r[8];goto L8c64f96e; /* exts.w r8,r3 */
L8c64f96e: *stop=0x8c64f96eu;r[0]-=r[3];goto L8c64f970; /* sub r3,r0 */
L8c64f970: *stop=0x8c64f970u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c64f972; /* float FPUL,fr2 */
L8c64f972: *stop=0x8c64f972u;*fpul=r[0];goto L8c64f974; /* lds r0,FPUL */
L8c64f974: *stop=0x8c64f974u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c64f976; /* float FPUL,fr1 */
L8c64f976: *stop=0x8c64f976u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[1],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64f978; /* fmac fr0,fr1,fr2 */
L8c64f978: *stop=0x8c64f978u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c64f97a; /* ftrc fr2,FPUL */
L8c64f97a: *stop=0x8c64f97au;r[0]=*fpul;goto L8c64f97c; /* sts FPUL,r0 */
L8c64f97c: *stop=0x8c64f97cu;if(!valid_access(r[7]+2u,2u))return -3;wr(m,bus,&io_status,r[7]+0x2u,r[0],2);if(io_status)return io_status;goto L8c64f97e; /* mov.w r0,@(0x2,r7) */
L8c64f97e: *stop=0x8c64f97eu;if(!valid_access(r[4]+4u,4u))return -3;r[8]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c64f980; /* mov.l @(0x4,r4),r8 */
L8c64f980: *stop=0x8c64f980u;r[10]=r[14];goto L8c64f982; /* mov r14,r10 */
L8c64f982: *stop=0x8c64f982u;r[10]<<=2;goto L8c64f984; /* shll2 r10 */
L8c64f984: *stop=0x8c64f984u;fcopy(f,0u,4u);goto L8c64f986; /* fmov fr4,fr0 */
L8c64f986: *stop=0x8c64f986u;if(!valid_access(r[6]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;goto L8c64f988; /* mov.l @(0x4,r6),r3 */
L8c64f988: *stop=0x8c64f988u;r[7]=r[13];goto L8c64f98a; /* mov r13,r7 */
L8c64f98a: *stop=0x8c64f98au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64f98c; /* mov.l r8,@r15 */
L8c64f98c: *stop=0x8c64f98cu;r[8]+=r[11];goto L8c64f98e; /* add r11,r8 */
L8c64f98e: *stop=0x8c64f98eu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[8],4);if(io_status)return io_status;goto L8c64f990; /* mov.l r8,@(0x10,r15) */
L8c64f990: *stop=0x8c64f990u;r[7]<<=2;goto L8c64f992; /* shll2 r7 */
L8c64f992: *stop=0x8c64f992u;if(!valid_access(r[8]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[8]+0x3u,1);if(io_status)return io_status;goto L8c64f994; /* mov.b @(0x3,r8),r0 */
L8c64f994: *stop=0x8c64f994u;r[3]+=r[7];goto L8c64f996; /* add r7,r3 */
L8c64f996: *stop=0x8c64f996u;r[8]=r[0];goto L8c64f998; /* mov r0,r8 */
L8c64f998: *stop=0x8c64f998u;r[2]=r[8]&255u;goto L8c64f99a; /* extu.b r8,r2 */
L8c64f99a: *stop=0x8c64f99au;*fpul=r[2];goto L8c64f99c; /* lds r2,FPUL */
L8c64f99c: *stop=0x8c64f99cu;if(!valid_access(r[15],4u))return -3;r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c64f99e; /* mov.l @r15,r2 */
L8c64f99e: *stop=0x8c64f99eu;r[2]+=r[10];goto L8c64f9a0; /* add r10,r2 */
L8c64f9a0: *stop=0x8c64f9a0u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64f9a2; /* float FPUL,fr3 */
L8c64f9a2: *stop=0x8c64f9a2u;if(!valid_access(r[2]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x3u,1);if(io_status)return io_status;goto L8c64f9a4; /* mov.b @(0x3,r2),r0 */
L8c64f9a4: *stop=0x8c64f9a4u;r[2]=r[8]&255u;goto L8c64f9a6; /* extu.b r8,r2 */
L8c64f9a6: *stop=0x8c64f9a6u;r[0]=r[0]&255u;goto L8c64f9a8; /* extu.b r0,r0 */
L8c64f9a8: *stop=0x8c64f9a8u;r[0]-=r[2];goto L8c64f9aa; /* sub r2,r0 */
L8c64f9aa: *stop=0x8c64f9aau;*fpul=r[0];goto L8c64f9ac; /* lds r0,FPUL */
L8c64f9ac: *stop=0x8c64f9acu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c64f9ae; /* float FPUL,fr2 */
L8c64f9ae: *stop=0x8c64f9aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f9b0; /* fmac fr0,fr2,fr3 */
L8c64f9b0: *stop=0x8c64f9b0u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c64f9b2; /* ftrc fr3,FPUL */
L8c64f9b2: *stop=0x8c64f9b2u;r[0]=*fpul;goto L8c64f9b4; /* sts FPUL,r0 */
L8c64f9b4: *stop=0x8c64f9b4u;if(!valid_access(r[3]+3u,1u))return -3;wr(m,bus,&io_status,r[3]+0x3u,r[0],1);if(io_status)return io_status;goto L8c64f9b6; /* mov.b r0,@(0x3,r3) */
L8c64f9b6: *stop=0x8c64f9b6u;if(!valid_access(r[4]+4u,4u))return -3;r[8]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c64f9b8; /* mov.l @(0x4,r4),r8 */
L8c64f9b8: *stop=0x8c64f9b8u;if(!valid_access(r[6]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;goto L8c64f9ba; /* mov.l @(0x4,r6),r3 */
L8c64f9ba: *stop=0x8c64f9bau;r[2]=r[8];goto L8c64f9bc; /* mov r8,r2 */
L8c64f9bc: *stop=0x8c64f9bcu;r[3]+=r[7];goto L8c64f9be; /* add r7,r3 */
L8c64f9be: *stop=0x8c64f9beu;r[2]+=r[11];goto L8c64f9c0; /* add r11,r2 */
L8c64f9c0: *stop=0x8c64f9c0u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c64f9c2; /* mov.l r2,@(0xc,r15) */
L8c64f9c2: *stop=0x8c64f9c2u;if(!valid_access(r[2]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x2u,1);if(io_status)return io_status;goto L8c64f9c4; /* mov.b @(0x2,r2),r0 */
L8c64f9c4: *stop=0x8c64f9c4u;r[2]=r[8];goto L8c64f9c6; /* mov r8,r2 */
L8c64f9c6: *stop=0x8c64f9c6u;r[2]+=r[10];goto L8c64f9c8; /* add r10,r2 */
L8c64f9c8: *stop=0x8c64f9c8u;if(!valid_access(r[15],1u))return -3;wr(m,bus,&io_status,r[15],r[0],1);if(io_status)return io_status;goto L8c64f9ca; /* mov.b r0,@r15 */
L8c64f9ca: *stop=0x8c64f9cau;r[0]=r[0]&255u;goto L8c64f9cc; /* extu.b r0,r0 */
L8c64f9cc: *stop=0x8c64f9ccu;*fpul=r[0];goto L8c64f9ce; /* lds r0,FPUL */
L8c64f9ce: *stop=0x8c64f9ceu;if(!valid_access(r[2]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x2u,1);if(io_status)return io_status;goto L8c64f9d0; /* mov.b @(0x2,r2),r0 */
L8c64f9d0: *stop=0x8c64f9d0u;if(!valid_access(r[15],1u))return -3;r[2]=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;goto L8c64f9d2; /* mov.b @r15,r2 */
L8c64f9d2: *stop=0x8c64f9d2u;r[0]=r[0]&255u;goto L8c64f9d4; /* extu.b r0,r0 */
L8c64f9d4: *stop=0x8c64f9d4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64f9d6; /* float FPUL,fr3 */
L8c64f9d6: *stop=0x8c64f9d6u;r[2]=r[2]&255u;goto L8c64f9d8; /* extu.b r2,r2 */
L8c64f9d8: *stop=0x8c64f9d8u;r[0]-=r[2];goto L8c64f9da; /* sub r2,r0 */
L8c64f9da: *stop=0x8c64f9dau;*fpul=r[0];goto L8c64f9dc; /* lds r0,FPUL */
L8c64f9dc: *stop=0x8c64f9dcu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c64f9de; /* float FPUL,fr2 */
L8c64f9de: *stop=0x8c64f9deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f9e0; /* fmac fr0,fr2,fr3 */
L8c64f9e0: *stop=0x8c64f9e0u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c64f9e2; /* ftrc fr3,FPUL */
L8c64f9e2: *stop=0x8c64f9e2u;r[0]=*fpul;goto L8c64f9e4; /* sts FPUL,r0 */
L8c64f9e4: *stop=0x8c64f9e4u;if(!valid_access(r[3]+2u,1u))return -3;wr(m,bus,&io_status,r[3]+0x2u,r[0],1);if(io_status)return io_status;goto L8c64f9e6; /* mov.b r0,@(0x2,r3) */
L8c64f9e6: *stop=0x8c64f9e6u;if(!valid_access(r[4]+4u,4u))return -3;r[8]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c64f9e8; /* mov.l @(0x4,r4),r8 */
L8c64f9e8: *stop=0x8c64f9e8u;if(!valid_access(r[6]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;goto L8c64f9ea; /* mov.l @(0x4,r6),r3 */
L8c64f9ea: *stop=0x8c64f9eau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64f9ec; /* mov.l r8,@r15 */
L8c64f9ec: *stop=0x8c64f9ecu;r[8]+=r[11];goto L8c64f9ee; /* add r11,r8 */
L8c64f9ee: *stop=0x8c64f9eeu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[8],4);if(io_status)return io_status;goto L8c64f9f0; /* mov.l r8,@(0x8,r15) */
L8c64f9f0: *stop=0x8c64f9f0u;r[3]+=r[7];goto L8c64f9f2; /* add r7,r3 */
L8c64f9f2: *stop=0x8c64f9f2u;if(!valid_access(r[8]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[8]+0x1u,1);if(io_status)return io_status;goto L8c64f9f4; /* mov.b @(0x1,r8),r0 */
L8c64f9f4: *stop=0x8c64f9f4u;if(!valid_access(r[15],4u))return -3;r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c64f9f6; /* mov.l @r15,r1 */
L8c64f9f6: *stop=0x8c64f9f6u;r[8]=r[0];goto L8c64f9f8; /* mov r0,r8 */
L8c64f9f8: *stop=0x8c64f9f8u;r[2]=r[8]&255u;goto L8c64f9fa; /* extu.b r8,r2 */
L8c64f9fa: *stop=0x8c64f9fau;*fpul=r[2];goto L8c64f9fc; /* lds r2,FPUL */
L8c64f9fc: *stop=0x8c64f9fcu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64f9fe; /* float FPUL,fr3 */
L8c64f9fe: *stop=0x8c64f9feu;r[1]+=r[10];goto L8c64fa00; /* add r10,r1 */
L8c64fa00: *stop=0x8c64fa00u;if(!valid_access(r[1]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[1]+0x1u,1);if(io_status)return io_status;goto L8c64fa02; /* mov.b @(0x1,r1),r0 */
L8c64fa02: *stop=0x8c64fa02u;r[2]=r[8]&255u;goto L8c64fa04; /* extu.b r8,r2 */
L8c64fa04: *stop=0x8c64fa04u;r[0]=r[0]&255u;goto L8c64fa06; /* extu.b r0,r0 */
L8c64fa06: *stop=0x8c64fa06u;r[0]-=r[2];goto L8c64fa08; /* sub r2,r0 */
L8c64fa08: *stop=0x8c64fa08u;*fpul=r[0];goto L8c64fa0a; /* lds r0,FPUL */
L8c64fa0a: *stop=0x8c64fa0au;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c64fa0c; /* float FPUL,fr2 */
L8c64fa0c: *stop=0x8c64fa0cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fa0e; /* fmac fr0,fr2,fr3 */
L8c64fa0e: *stop=0x8c64fa0eu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c64fa10; /* ftrc fr3,FPUL */
L8c64fa10: *stop=0x8c64fa10u;r[0]=*fpul;goto L8c64fa12; /* sts FPUL,r0 */
L8c64fa12: *stop=0x8c64fa12u;if(!valid_access(r[3]+1u,1u))return -3;wr(m,bus,&io_status,r[3]+0x1u,r[0],1);if(io_status)return io_status;goto L8c64fa14; /* mov.b r0,@(0x1,r3) */
L8c64fa14: *stop=0x8c64fa14u;if(!valid_access(r[4]+4u,4u))return -3;r[8]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c64fa16; /* mov.l @(0x4,r4),r8 */
L8c64fa16: *stop=0x8c64fa16u;if(!valid_access(r[6]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;goto L8c64fa18; /* mov.l @(0x4,r6),r3 */
L8c64fa18: *stop=0x8c64fa18u;r[2]=r[8];goto L8c64fa1a; /* mov r8,r2 */
L8c64fa1a: *stop=0x8c64fa1au;r[2]+=r[11];goto L8c64fa1c; /* add r11,r2 */
L8c64fa1c: *stop=0x8c64fa1cu;r[7]+=r[3];goto L8c64fa1e; /* add r3,r7 */
L8c64fa1e: *stop=0x8c64fa1eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c64fa20; /* mov.l r2,@(0x4,r15) */
L8c64fa20: *stop=0x8c64fa20u;if(!valid_access(r[2],1u))return -3;r[3]=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;goto L8c64fa22; /* mov.b @r2,r3 */
L8c64fa22: *stop=0x8c64fa22u;r[10]+=r[8];goto L8c64fa24; /* add r8,r10 */
L8c64fa24: *stop=0x8c64fa24u;if(!valid_access(r[15],1u))return -3;wr(m,bus,&io_status,r[15],r[3],1);if(io_status)return io_status;goto L8c64fa26; /* mov.b r3,@r15 */
L8c64fa26: *stop=0x8c64fa26u;r[3]=r[3]&255u;goto L8c64fa28; /* extu.b r3,r3 */
L8c64fa28: *stop=0x8c64fa28u;*fpul=r[3];goto L8c64fa2a; /* lds r3,FPUL */
L8c64fa2a: *stop=0x8c64fa2au;if(!valid_access(r[15],1u))return -3;r[2]=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;goto L8c64fa2c; /* mov.b @r15,r2 */
L8c64fa2c: *stop=0x8c64fa2cu;if(!valid_access(r[10],1u))return -3;r[3]=sx(m,bus,&io_status,r[10],1);if(io_status)return io_status;goto L8c64fa2e; /* mov.b @r10,r3 */
L8c64fa2e: *stop=0x8c64fa2eu;r[2]=r[2]&255u;goto L8c64fa30; /* extu.b r2,r2 */
L8c64fa30: *stop=0x8c64fa30u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64fa32; /* float FPUL,fr3 */
L8c64fa32: *stop=0x8c64fa32u;r[3]=r[3]&255u;goto L8c64fa34; /* extu.b r3,r3 */
L8c64fa34: *stop=0x8c64fa34u;r[3]-=r[2];goto L8c64fa36; /* sub r2,r3 */
L8c64fa36: *stop=0x8c64fa36u;*fpul=r[3];goto L8c64fa38; /* lds r3,FPUL */
L8c64fa38: *stop=0x8c64fa38u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c64fa3a; /* float FPUL,fr2 */
L8c64fa3a: *stop=0x8c64fa3au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fa3c; /* fmac fr0,fr2,fr3 */
L8c64fa3c: *stop=0x8c64fa3cu;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c64fa3e; /* ftrc fr3,FPUL */
L8c64fa3e: *stop=0x8c64fa3eu;r[3]=*fpul;goto L8c64fa40; /* sts FPUL,r3 */
L8c64fa40: *stop=0x8c64fa40u;r[13]+=1u;goto L8c64fa42; /* add #0x1,r13 */
L8c64fa42: *stop=0x8c64fa42u;if(!valid_access(r[7],1u))return -3;wr(m,bus,&io_status,r[7],r[3],1);if(io_status)return io_status;goto L8c64fa44; /* mov.b r3,@r7 */
L8c64fa44: *stop=0x8c64fa44u;r[11]=r[14];goto L8c64fa46; /* mov r14,r11 */
L8c64fa46: *stop=0x8c64fa46u;*sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c64fa48; /* shll r11 */
L8c64fa48: *stop=0x8c64fa48u;r[3]=r[14];goto L8c64fa4a; /* mov r14,r3 */
L8c64fa4a: *stop=0x8c64fa4au;if(!valid_access(r[4],4u))return -3;r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c64fa4c; /* mov.l @r4,r2 */
L8c64fa4c: *stop=0x8c64fa4cu;r[11]+=r[3];goto L8c64fa4e; /* add r3,r11 */
L8c64fa4e: *stop=0x8c64fa4eu;if(!valid_access(r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c64fa50; /* fmov.s @r12,fr3 */
L8c64fa50: *stop=0x8c64fa50u;r[11]<<=2;goto L8c64fa52; /* shll2 r11 */
L8c64fa52: *stop=0x8c64fa52u;r[0]=8u;goto L8c64fa54; /* mov #0x8,r0 */
L8c64fa54: *stop=0x8c64fa54u;r[2]+=r[11];goto L8c64fa56; /* add r11,r2 */
L8c64fa56: *stop=0x8c64fa56u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c64fa58; /* fmov.s @(r0,r2),fr2 */
L8c64fa58: *stop=0x8c64fa58u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c64fa5a; /* fcmp/gt fr2,fr3 */
L8c64fa5a: if((*sr&1u))goto L8c64fac0;goto L8c64fa5c; /* bt 0x8c64fac0 */
L8c64fa5c: *stop=0x8c64fa5cu;r[7]=r[13];goto L8c64fa5e; /* mov r13,r7 */
L8c64fa5e: *stop=0x8c64fa5eu;*sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c64fa60; /* shll r7 */
L8c64fa60: *stop=0x8c64fa60u;r[3]=r[13];goto L8c64fa62; /* mov r13,r3 */
L8c64fa62: *stop=0x8c64fa62u;r[7]+=r[3];goto L8c64fa64; /* add r3,r7 */
L8c64fa64: *stop=0x8c64fa64u;if(!valid_access(r[4],4u))return -3;r[3]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c64fa66; /* mov.l @r4,r3 */
L8c64fa66: *stop=0x8c64fa66u;r[7]<<=2;goto L8c64fa68; /* shll2 r7 */
L8c64fa68: *stop=0x8c64fa68u;if(!valid_access(r[6],4u))return -3;r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c64fa6a; /* mov.l @r6,r2 */
L8c64fa6a: *stop=0x8c64fa6au;r[0]=4u;goto L8c64fa6c; /* mov #0x4,r0 */
L8c64fa6c: *stop=0x8c64fa6cu;r[3]+=r[11];goto L8c64fa6e; /* add r11,r3 */
L8c64fa6e: *stop=0x8c64fa6eu;r[10]=r[14];goto L8c64fa70; /* mov r14,r10 */
L8c64fa70: *stop=0x8c64fa70u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c64fa72; /* fmov.s @r3,fr3 */
L8c64fa72: *stop=0x8c64fa72u;r[2]+=r[7];goto L8c64fa74; /* add r7,r2 */
L8c64fa74: *stop=0x8c64fa74u;*sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c64fa76; /* tst r9,r9 */
L8c64fa76: *stop=0x8c64fa76u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c64fa78; /* fmov.s fr3,@r2 */
L8c64fa78: *stop=0x8c64fa78u;if(!valid_access(r[4],4u))return -3;r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c64fa7a; /* mov.l @r4,r2 */
L8c64fa7a: *stop=0x8c64fa7au;if(!valid_access(r[6],4u))return -3;r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c64fa7c; /* mov.l @r6,r3 */
L8c64fa7c: *stop=0x8c64fa7cu;r[2]+=r[11];goto L8c64fa7e; /* add r11,r2 */
L8c64fa7e: *stop=0x8c64fa7eu;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c64fa80; /* fmov.s @(r0,r2),fr3 */
L8c64fa80: *stop=0x8c64fa80u;r[3]+=r[7];goto L8c64fa82; /* add r7,r3 */
L8c64fa82: *stop=0x8c64fa82u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c64fa84; /* fmov fr3,@(r0,r3) */
L8c64fa84: *stop=0x8c64fa84u;r[0]=8u;goto L8c64fa86; /* mov #0x8,r0 */
L8c64fa86: *stop=0x8c64fa86u;if(!valid_access(r[4],4u))return -3;r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c64fa88; /* mov.l @r4,r2 */
L8c64fa88: *stop=0x8c64fa88u;if(!valid_access(r[6],4u))return -3;r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c64fa8a; /* mov.l @r6,r3 */
L8c64fa8a: *stop=0x8c64fa8au;r[2]+=r[11];goto L8c64fa8c; /* add r11,r2 */
L8c64fa8c: *stop=0x8c64fa8cu;r[11]=r[13];goto L8c64fa8e; /* mov r13,r11 */
L8c64fa8e: *stop=0x8c64fa8eu;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c64fa90; /* fmov.s @(r0,r2),fr3 */
L8c64fa90: *stop=0x8c64fa90u;r[7]+=r[3];goto L8c64fa92; /* add r3,r7 */
L8c64fa92: *stop=0x8c64fa92u;r[11]<<=2;goto L8c64fa94; /* shll2 r11 */
L8c64fa94: *stop=0x8c64fa94u;if(!valid_access(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c64fa96; /* fmov fr3,@(r0,r7) */
L8c64fa96: take=(*sr&1u);*stop=0x8c64fa98u;r[10]<<=2;if(take)goto L8c64fab2;goto L8c64fa9a; /* bt/s 0x8c64fab2 */
L8c64fa9a: *stop=0x8c64fa9au;if(!valid_access(r[4]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;goto L8c64fa9c; /* mov.l @(0x8,r4),r3 */
L8c64fa9c: *stop=0x8c64fa9cu;if(!valid_access(r[6]+8u,4u))return -3;r[2]=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;goto L8c64fa9e; /* mov.l @(0x8,r6),r2 */
L8c64fa9e: *stop=0x8c64fa9eu;r[3]+=r[10];goto L8c64faa0; /* add r10,r3 */
L8c64faa0: *stop=0x8c64faa0u;if(!valid_access(r[3],2u))return -3;r[1]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c64faa2; /* mov.w @r3,r1 */
L8c64faa2: *stop=0x8c64faa2u;r[2]+=r[11];goto L8c64faa4; /* add r11,r2 */
L8c64faa4: *stop=0x8c64faa4u;if(!valid_access(r[2],2u))return -3;wr(m,bus,&io_status,r[2],r[1],2);if(io_status)return io_status;goto L8c64faa6; /* mov.w r1,@r2 */
L8c64faa6: *stop=0x8c64faa6u;if(!valid_access(r[4]+8u,4u))return -3;r[0]=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;goto L8c64faa8; /* mov.l @(0x8,r4),r0 */
L8c64faa8: *stop=0x8c64faa8u;if(!valid_access(r[6]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;goto L8c64faaa; /* mov.l @(0x8,r6),r3 */
L8c64faaa: *stop=0x8c64faaau;r[0]+=r[10];goto L8c64faac; /* add r10,r0 */
L8c64faac: *stop=0x8c64faacu;if(!valid_access(r[0]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c64faae; /* mov.w @(0x2,r0),r0 */
L8c64faae: *stop=0x8c64faaeu;r[3]+=r[11];goto L8c64fab0; /* add r11,r3 */
L8c64fab0: *stop=0x8c64fab0u;if(!valid_access(r[3]+2u,2u))return -3;wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c64fab2; /* mov.w r0,@(0x2,r3) */
L8c64fab2: *stop=0x8c64fab2u;if(!valid_access(r[4]+4u,4u))return -3;r[2]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c64fab4; /* mov.l @(0x4,r4),r2 */
L8c64fab4: *stop=0x8c64fab4u;r[13]+=1u;goto L8c64fab6; /* add #0x1,r13 */
L8c64fab6: *stop=0x8c64fab6u;if(!valid_access(r[6]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;goto L8c64fab8; /* mov.l @(0x4,r6),r3 */
L8c64fab8: *stop=0x8c64fab8u;r[2]+=r[10];goto L8c64faba; /* add r10,r2 */
L8c64faba: *stop=0x8c64fabau;if(!valid_access(r[2],4u))return -3;r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c64fabc; /* mov.l @r2,r1 */
L8c64fabc: *stop=0x8c64fabcu;r[3]+=r[11];goto L8c64fabe; /* add r11,r3 */
L8c64fabe: *stop=0x8c64fabeu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c64fac0; /* mov.l r1,@r3 */
L8c64fac0: *stop=0x8c64fac0u;r[10]=r[14];goto L8c64fac2; /* mov r14,r10 */
L8c64fac2: *stop=0x8c64fac2u;r[14]+=1u;goto L8c64fac4; /* add #0x1,r14 */
L8c64fac4: *stop=0x8c64fac4u;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[5]);goto L8c64fac6; /* cmp/ge r5,r14 */
L8c64fac6: if((*sr&1u))goto L8c64facc;goto L8c64fac8; /* bt 0x8c64facc */
L8c64fac8: *stop=0x8c64facau;goto L8c64f866; /* bra 0x8c64f866 */
L8c64facc: *stop=0x8c64faccu;r[0]=r[13];goto L8c64face; /* mov r13,r0 */
L8c64face: *stop=0x8c64faceu;r[15]+=36u;goto L8c64fad0; /* add #0x24,r15 */
L8c64fad0: *stop=0x8c64fad0u;if(!valid_access(r[15],4u))return -3;r[8]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c64fad2; /* mov.l @r15+,r8 */
L8c64fad2: *stop=0x8c64fad2u;if(!valid_access(r[15],4u))return -3;r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c64fad4; /* mov.l @r15+,r9 */
L8c64fad4: *stop=0x8c64fad4u;if(!valid_access(r[15],4u))return -3;r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c64fad6; /* mov.l @r15+,r10 */
L8c64fad6: *stop=0x8c64fad6u;if(!valid_access(r[15],4u))return -3;r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c64fad8; /* mov.l @r15+,r11 */
L8c64fad8: *stop=0x8c64fad8u;if(!valid_access(r[15],4u))return -3;r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c64fada; /* mov.l @r15+,r12 */
L8c64fada: *stop=0x8c64fadau;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c64fadc; /* mov.l @r15+,r13 */
L8c64fadc: *stop=0x8c64fadcu;if(*pr!=entry_pr)return -3;*stop=0x8c64fadeu;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_polygon_header_uv_8c60607a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60607a;
L8c60607a: *stop=0x8c60607au;r[6]=rd(m,bus,&io_status,0x8c606170u,4);if(io_status)return io_status;goto L8c60607c; /* mov.l 0x8c606170,r6 */
L8c60607c: *stop=0x8c60607cu;r[5]=28u;goto L8c60607e; /* mov #0x1c,r5 */
L8c60607e: *stop=0x8c60607eu;r[3]=rd(m,bus,&io_status,0x8c606174u,4);if(io_status)return io_status;goto L8c606080; /* mov.l 0x8c606174,r3 */
L8c606080: *stop=0x8c606080u;if(!valid_access(r[6]+20u,4u))return -3;r[4]=rd(m,bus,&io_status,r[6]+0x14u,4);if(io_status)return io_status;goto L8c606082; /* mov.l @(0x14,r6),r4 */
L8c606082: *stop=0x8c606082u;r[2]=rd(m,bus,&io_status,0x8c606178u,4);if(io_status)return io_status;goto L8c606084; /* mov.l 0x8c606178,r2 */
L8c606084: *stop=0x8c606084u;r[0]=r[4];goto L8c606086; /* mov r4,r0 */
L8c606086: *stop=0x8c606086u;r[3]&=r[4];goto L8c606088; /* and r4,r3 */
L8c606088: *stop=0x8c606088u;r[0]>>=16;goto L8c60608a; /* shlr16 r0 */
L8c60608a: *stop=0x8c60608au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c60608c; /* mov.l r3,@r2 */
L8c60608c: *stop=0x8c60608cu;r[3]=rd(m,bus,&io_status,0x8c606180u,4);if(io_status)return io_status;goto L8c60608e; /* mov.l 0x8c606180,r3 */
L8c60608e: *stop=0x8c60608eu;r[0]>>=8;goto L8c606090; /* shlr8 r0 */
L8c606090: *stop=0x8c606090u;r[2]=rd(m,bus,&io_status,0x8c606184u,4);if(io_status)return io_status;goto L8c606092; /* mov.l 0x8c606184,r2 */
L8c606092: *stop=0x8c606092u;r[5]&=r[0];goto L8c606094; /* and r0,r5 */
L8c606094: *stop=0x8c606094u;r[1]=rd(m,bus,&io_status,0x8c606188u,4);if(io_status)return io_status;goto L8c606096; /* mov.l 0x8c606188,r1 */
L8c606096: *stop=0x8c606096u;r[4]&=r[2];goto L8c606098; /* and r2,r4 */
L8c606098: *stop=0x8c606098u;r[0]=rd(m,bus,&io_status,0x8c60617cu,4);if(io_status)return io_status;goto L8c60609a; /* mov.l 0x8c60617c,r0 */
L8c60609a: *stop=0x8c60609au;r[4]|=r[1];goto L8c60609c; /* or r1,r4 */
L8c60609c: *stop=0x8c60609cu;r[4]+=16u;goto L8c60609e; /* add #0x10,r4 */
L8c60609e: *stop=0x8c60609eu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[5],4);if(io_status)return io_status;goto L8c6060a0; /* mov.l r5,@r0 */
L8c6060a0: *stop=0x8c6060a0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c6060a2; /* mov.l r5,@r3 */
L8c6060a2: *stop=0x8c6060a2u;if(!valid_access(r[6]+12u,4u))return -3;r[3]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c6060a4; /* mov.l @(0xc,r6),r3 */
L8c6060a4: *stop=0x8c6060a4u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6060a6; /* mov.l r3,@-r4 */
L8c6060a6: *stop=0x8c6060a6u;if(!valid_access(r[6]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;goto L8c6060a8; /* mov.l @(0x8,r6),r3 */
L8c6060a8: *stop=0x8c6060a8u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6060aa; /* mov.l r3,@-r4 */
L8c6060aa: *stop=0x8c6060aau;if(!valid_access(r[6]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;goto L8c6060ac; /* mov.l @(0x4,r6),r3 */
L8c6060ac: *stop=0x8c6060acu;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6060ae; /* mov.l r3,@-r4 */
L8c6060ae: *stop=0x8c6060aeu;if(!valid_access(r[6],4u))return -3;r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6060b0; /* mov.l @r6,r3 */
L8c6060b0: *stop=0x8c6060b0u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6060b2; /* mov.l r3,@-r4 */
L8c6060b2: *stop=0x8c6060b2u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c6060b4; /* pref @r4 */
L8c6060b4: *stop=0x8c6060b4u;r[0]=r[6];goto L8c6060b6; /* mov r6,r0 */
L8c6060b6: *stop=0x8c6060b6u;r[3]=rd(m,bus,&io_status,0x8c606194u,4);if(io_status)return io_status;goto L8c6060b8; /* mov.l 0x8c606194,r3 */
L8c6060b8: *stop=0x8c6060b8u;r[0]+=20u;goto L8c6060ba; /* add #0x14,r0 */
L8c6060ba: *stop=0x8c6060bau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c6060bc; /* mov.l r0,@r3 */
L8c6060bc: *stop=0x8c6060bcu;r[4]+=32u;goto L8c6060be; /* add #0x20,r4 */
L8c6060be: *stop=0x8c6060beu;if(*pr!=entry_pr)return -3;*stop=0x8c6060c0u;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_polygon_header_colour_8c605fca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c605fca;
L8c605fca: *stop=0x8c605fcau;r[6]=rd(m,bus,&io_status,0x8c606170u,4);if(io_status)return io_status;goto L8c605fcc; /* mov.l 0x8c606170,r6 */
L8c605fcc: *stop=0x8c605fccu;r[5]=28u;goto L8c605fce; /* mov #0x1c,r5 */
L8c605fce: *stop=0x8c605fceu;r[3]=rd(m,bus,&io_status,0x8c606174u,4);if(io_status)return io_status;goto L8c605fd0; /* mov.l 0x8c606174,r3 */
L8c605fd0: *stop=0x8c605fd0u;if(!valid_access(r[6]+16u,4u))return -3;r[4]=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;goto L8c605fd2; /* mov.l @(0x10,r6),r4 */
L8c605fd2: *stop=0x8c605fd2u;r[2]=rd(m,bus,&io_status,0x8c606178u,4);if(io_status)return io_status;goto L8c605fd4; /* mov.l 0x8c606178,r2 */
L8c605fd4: *stop=0x8c605fd4u;r[0]=r[4];goto L8c605fd6; /* mov r4,r0 */
L8c605fd6: *stop=0x8c605fd6u;r[3]&=r[4];goto L8c605fd8; /* and r4,r3 */
L8c605fd8: *stop=0x8c605fd8u;r[0]>>=16;goto L8c605fda; /* shlr16 r0 */
L8c605fda: *stop=0x8c605fdau;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c605fdc; /* mov.l r3,@r2 */
L8c605fdc: *stop=0x8c605fdcu;r[3]=rd(m,bus,&io_status,0x8c606180u,4);if(io_status)return io_status;goto L8c605fde; /* mov.l 0x8c606180,r3 */
L8c605fde: *stop=0x8c605fdeu;r[0]>>=8;goto L8c605fe0; /* shlr8 r0 */
L8c605fe0: *stop=0x8c605fe0u;r[2]=rd(m,bus,&io_status,0x8c606184u,4);if(io_status)return io_status;goto L8c605fe2; /* mov.l 0x8c606184,r2 */
L8c605fe2: *stop=0x8c605fe2u;r[5]&=r[0];goto L8c605fe4; /* and r0,r5 */
L8c605fe4: *stop=0x8c605fe4u;r[1]=rd(m,bus,&io_status,0x8c606188u,4);if(io_status)return io_status;goto L8c605fe6; /* mov.l 0x8c606188,r1 */
L8c605fe6: *stop=0x8c605fe6u;r[4]&=r[2];goto L8c605fe8; /* and r2,r4 */
L8c605fe8: *stop=0x8c605fe8u;r[0]=rd(m,bus,&io_status,0x8c60617cu,4);if(io_status)return io_status;goto L8c605fea; /* mov.l 0x8c60617c,r0 */
L8c605fea: *stop=0x8c605feau;r[4]|=r[1];goto L8c605fec; /* or r1,r4 */
L8c605fec: *stop=0x8c605fecu;r[2]=rd(m,bus,&io_status,0x8c606190u,4);if(io_status)return io_status;goto L8c605fee; /* mov.l 0x8c606190,r2 */
L8c605fee: *stop=0x8c605feeu;r[4]+=16u;goto L8c605ff0; /* add #0x10,r4 */
L8c605ff0: *stop=0x8c605ff0u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[5],4);if(io_status)return io_status;goto L8c605ff2; /* mov.l r5,@r0 */
L8c605ff2: *stop=0x8c605ff2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c605ff4; /* mov.l r5,@r3 */
L8c605ff4: *stop=0x8c605ff4u;if(!valid_access(r[6]+12u,4u))return -3;r[3]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c605ff6; /* mov.l @(0xc,r6),r3 */
L8c605ff6: *stop=0x8c605ff6u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c605ff8; /* mov.l r3,@-r4 */
L8c605ff8: *stop=0x8c605ff8u;if(!valid_access(r[6]+8u,4u))return -3;r[0]=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;goto L8c605ffa; /* mov.l @(0x8,r6),r0 */
L8c605ffa: *stop=0x8c605ffau;r[3]=rd(m,bus,&io_status,0x8c60618cu,4);if(io_status)return io_status;goto L8c605ffc; /* mov.l 0x8c60618c,r3 */
L8c605ffc: *stop=0x8c605ffcu;r[0]&=r[3];goto L8c605ffe; /* and r3,r0 */
L8c605ffe: *stop=0x8c605ffeu;r[0]|=r[2];goto L8c606000; /* or r2,r0 */
L8c606000: *stop=0x8c606000u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c606002; /* mov.l r0,@-r4 */
L8c606002: *stop=0x8c606002u;if(!valid_access(r[6]+4u,4u))return -3;r[1]=rd(m,bus,&io_status,r[6]+0x4u,4);if(io_status)return io_status;goto L8c606004; /* mov.l @(0x4,r6),r1 */
L8c606004: *stop=0x8c606004u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c606006; /* mov.l r1,@-r4 */
L8c606006: *stop=0x8c606006u;if(!valid_access(r[6],4u))return -3;r[1]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c606008; /* mov.l @r6,r1 */
L8c606008: *stop=0x8c606008u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c60600a; /* mov.l r1,@-r4 */
L8c60600a: *stop=0x8c60600au;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c60600c; /* pref @r4 */
L8c60600c: *stop=0x8c60600cu;r[0]=r[6];goto L8c60600e; /* mov r6,r0 */
L8c60600e: *stop=0x8c60600eu;r[1]=rd(m,bus,&io_status,0x8c606194u,4);if(io_status)return io_status;goto L8c606010; /* mov.l 0x8c606194,r1 */
L8c606010: *stop=0x8c606010u;r[0]+=16u;goto L8c606012; /* add #0x10,r0 */
L8c606012: *stop=0x8c606012u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c606014; /* mov.l r0,@r1 */
L8c606014: *stop=0x8c606014u;r[4]+=32u;goto L8c606016; /* add #0x20,r4 */
L8c606016: *stop=0x8c606016u;if(*pr!=entry_pr)return -3;*stop=0x8c606018u;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_polygon_emit_uv_8c64fc68(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64fc68;
L8c64fc68: *stop=0x8c64fc68u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64fc6a; /* mov.l r14,@-r15 */
L8c64fc6a: *stop=0x8c64fc6au;r[0]=0x8c64fdf4u;goto L8c64fc6c; /* mova 0x8c64fdf4,r0 */
L8c64fc6c: *stop=0x8c64fc6cu;r[14]=rd(m,bus,&io_status,0x8c64fdf8u,4);if(io_status)return io_status;goto L8c64fc6e; /* mov.l 0x8c64fdf8,r14 */
L8c64fc6e: *stop=0x8c64fc6eu;r[7]=0u;goto L8c64fc70; /* mov #0x0,r7 */
L8c64fc70: *stop=0x8c64fc70u;r[3]=rd(m,bus,&io_status,0x8c64fdfcu,4);if(io_status)return io_status;goto L8c64fc72; /* mov.l 0x8c64fdfc,r3 */
L8c64fc72: *stop=0x8c64fc72u;r[2]=rd(m,bus,&io_status,0x8c64fe00u,4);if(io_status)return io_status;goto L8c64fc74; /* mov.l 0x8c64fe00,r2 */
L8c64fc74: *stop=0x8c64fc74u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64fc76; /* mov.l r13,@-r15 */
L8c64fc76: *stop=0x8c64fc76u;r[13]=r[4];goto L8c64fc78; /* mov r4,r13 */
L8c64fc78: *stop=0x8c64fc78u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64fc7a; /* mov.l r12,@-r15 */
L8c64fc7a: *stop=0x8c64fc7au;r[13]+=4294967295u;goto L8c64fc7c; /* add #-0x1,r13 */
L8c64fc7c: *stop=0x8c64fc7cu;r[12]=rd(m,bus,&io_status,0x8c64fe04u,4);if(io_status)return io_status;goto L8c64fc7e; /* mov.l 0x8c64fe04,r12 */
L8c64fc7e: *stop=0x8c64fc7eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c64fc80; /* fldi1 fr7 */
L8c64fc80: *stop=0x8c64fc80u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64fc82; /* mov.l r10,@-r15 */
L8c64fc82: *stop=0x8c64fc82u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c64fc84; /* fmov.s @r14,fr4 */
L8c64fc84: *stop=0x8c64fc84u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c64fc86; /* fmov.s @r3,fr3 */
L8c64fc86: *stop=0x8c64fc86u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c64fc88; /* fmov.s @r2,fr2 */
L8c64fc88: *stop=0x8c64fc88u;fcopy(f,6u,4u);goto L8c64fc8a; /* fmov fr4,fr6 */
L8c64fc8a: *stop=0x8c64fc8au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c64fc8c; /* fmul fr3,fr6 */
L8c64fc8c: *stop=0x8c64fc8cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c64fc8e; /* fmul fr2,fr4 */
L8c64fc8e: *stop=0x8c64fc8eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c64fc90; /* fmov.s @r0,fr5 */
L8c64fc90: *stop=0x8c64fc90u;if(!valid_access(r[6],4u))return -3;r[4]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c64fc92; /* mov.l @r6,r4 */
L8c64fc92: *stop=0x8c64fc92u;r[10]=r[7];goto L8c64fc94; /* mov r7,r10 */
L8c64fc94: *stop=0x8c64fc94u;*sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c64fc96; /* shll r10 */
L8c64fc96: *stop=0x8c64fc96u;r[3]=r[7];goto L8c64fc98; /* mov r7,r3 */
L8c64fc98: *stop=0x8c64fc98u;if(!valid_access(r[5],4u))return -3;r[2]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fc9a; /* mov.l @r5,r2 */
L8c64fc9a: *stop=0x8c64fc9au;r[10]+=r[3];goto L8c64fc9c; /* add r3,r10 */
L8c64fc9c: *stop=0x8c64fc9cu;if(!valid_access(r[5]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c64fc9e; /* mov.l @(0x4,r5),r3 */
L8c64fc9e: *stop=0x8c64fc9eu;r[1]=r[7];goto L8c64fca0; /* mov r7,r1 */
L8c64fca0: *stop=0x8c64fca0u;r[10]<<=2;goto L8c64fca2; /* shll2 r10 */
L8c64fca2: *stop=0x8c64fca2u;r[1]<<=2;goto L8c64fca4; /* shll2 r1 */
L8c64fca4: *stop=0x8c64fca4u;fcopy(f,8u,7u);goto L8c64fca6; /* fmov fr7,fr8 */
L8c64fca6: *stop=0x8c64fca6u;r[2]+=r[10];goto L8c64fca8; /* add r10,r2 */
L8c64fca8: *stop=0x8c64fca8u;r[0]=8u;goto L8c64fcaa; /* mov #0x8,r0 */
L8c64fcaa: *stop=0x8c64fcaau;r[3]+=r[1];goto L8c64fcac; /* add r1,r3 */
L8c64fcac: *stop=0x8c64fcacu;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c64fcae; /* fmov.s @(r0,r2),fr3 */
L8c64fcae: *stop=0x8c64fcaeu;if(!valid_access(r[3],4u))return -3;r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c64fcb0; /* mov.l @r3,r2 */
L8c64fcb0: *stop=0x8c64fcb0u;r[4]+=28u;goto L8c64fcb2; /* add #0x1c,r4 */
L8c64fcb2: *stop=0x8c64fcb2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[8],&fr[3],&f->fpscr,&fr[8]);if(status)return status;}goto L8c64fcb4; /* fdiv fr3,fr8 */
L8c64fcb4: *stop=0x8c64fcb4u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c64fcb6; /* mov.l r2,@-r4 */
L8c64fcb6: *stop=0x8c64fcb6u;if(!valid_access(r[5]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;goto L8c64fcb8; /* mov.l @(0x8,r5),r3 */
L8c64fcb8: *stop=0x8c64fcb8u;r[3]+=r[1];goto L8c64fcba; /* add r1,r3 */
L8c64fcba: *stop=0x8c64fcbau;if(!valid_access(r[3]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c64fcbc; /* mov.w @(0x2,r3),r0 */
L8c64fcbc: *stop=0x8c64fcbcu;r[3]=r[0];goto L8c64fcbe; /* mov r0,r3 */
L8c64fcbe: *stop=0x8c64fcbeu;*fpul=r[3];goto L8c64fcc0; /* lds r3,FPUL */
L8c64fcc0: *stop=0x8c64fcc0u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64fcc2; /* float FPUL,fr3 */
L8c64fcc2: *stop=0x8c64fcc2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fcc4; /* fmul fr5,fr3 */
L8c64fcc4: *stop=0x8c64fcc4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c64fcc6; /* fmov.s fr3,@-r4 */
L8c64fcc6: *stop=0x8c64fcc6u;if(!valid_access(r[5]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;goto L8c64fcc8; /* mov.l @(0x8,r5),r3 */
L8c64fcc8: *stop=0x8c64fcc8u;r[1]+=r[3];goto L8c64fcca; /* add r3,r1 */
L8c64fcca: *stop=0x8c64fccau;if(!valid_access(r[1],2u))return -3;r[3]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;goto L8c64fccc; /* mov.w @r1,r3 */
L8c64fccc: *stop=0x8c64fcccu;*fpul=r[3];goto L8c64fcce; /* lds r3,FPUL */
L8c64fcce: *stop=0x8c64fcceu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64fcd0; /* float FPUL,fr3 */
L8c64fcd0: *stop=0x8c64fcd0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fcd2; /* fmul fr5,fr3 */
L8c64fcd2: *stop=0x8c64fcd2u;r[0]=4u;goto L8c64fcd4; /* mov #0x4,r0 */
L8c64fcd4: *stop=0x8c64fcd4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c64fcd6; /* fmov.s fr3,@-r4 */
L8c64fcd6: *stop=0x8c64fcd6u;fcopy(f,0u,8u);goto L8c64fcd8; /* fmov fr8,fr0 */
L8c64fcd8: *stop=0x8c64fcd8u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;goto L8c64fcda; /* fmov.s fr8,@-r4 */
L8c64fcda: *stop=0x8c64fcdau;if(!valid_access(r[5],4u))return -3;r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fcdc; /* mov.l @r5,r3 */
L8c64fcdc: *stop=0x8c64fcdcu;r[3]+=r[10];goto L8c64fcde; /* add r10,r3 */
L8c64fcde: *stop=0x8c64fcdeu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c64fce0; /* fmov.s @(r0,r3),fr3 */
L8c64fce0: *stop=0x8c64fce0u;r[0]=16u;goto L8c64fce2; /* mov #0x10,r0 */
L8c64fce2: *stop=0x8c64fce2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c64fce4; /* fmov.s @(r0,r14),fr2 */
L8c64fce4: *stop=0x8c64fce4u;r[0]=12u;goto L8c64fce6; /* mov #0xc,r0 */
L8c64fce6: *stop=0x8c64fce6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fce8; /* fmul fr4,fr3 */
L8c64fce8: *stop=0x8c64fce8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fcea; /* fmac fr0,fr3,fr2 */
L8c64fcea: *stop=0x8c64fceau;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64fcec; /* fmov.s fr2,@-r4 */
L8c64fcec: *stop=0x8c64fcecu;if(!valid_access(r[5],4u))return -3;r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fcee; /* mov.l @r5,r3 */
L8c64fcee: *stop=0x8c64fceeu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c64fcf0; /* fmov.s @(r0,r14),fr2 */
L8c64fcf0: *stop=0x8c64fcf0u;r[10]+=r[3];goto L8c64fcf2; /* add r3,r10 */
L8c64fcf2: *stop=0x8c64fcf2u;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[10]);if(io_status)return io_status;goto L8c64fcf4; /* fmov.s @r10,fr3 */
L8c64fcf4: *stop=0x8c64fcf4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fcf6; /* fmul fr6,fr3 */
L8c64fcf6: *stop=0x8c64fcf6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fcf8; /* fmac fr0,fr3,fr2 */
L8c64fcf8: *stop=0x8c64fcf8u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64fcfa; /* fmov.s fr2,@-r4 */
L8c64fcfa: *stop=0x8c64fcfau;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c64fcfc; /* mov.l r12,@-r4 */
L8c64fcfc: *stop=0x8c64fcfcu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c64fcfe; /* pref @r4 */
L8c64fcfe: *stop=0x8c64fcfeu;r[7]+=1u;goto L8c64fd00; /* add #0x1,r7 */
L8c64fd00: *stop=0x8c64fd00u;*sr=(*sr&~1u)|(r[13]==r[7]);goto L8c64fd02; /* cmp/eq r13,r7 */
L8c64fd02: take=(*sr&1u);*stop=0x8c64fd04u;r[4]+=32u;if(take)goto L8c64fd7a;goto L8c64fd06; /* bt/s 0x8c64fd7a */
L8c64fd06: *stop=0x8c64fd06u;r[10]=r[13];goto L8c64fd08; /* mov r13,r10 */
L8c64fd08: *stop=0x8c64fd08u;*sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c64fd0a; /* shll r10 */
L8c64fd0a: *stop=0x8c64fd0au;r[3]=r[13];goto L8c64fd0c; /* mov r13,r3 */
L8c64fd0c: *stop=0x8c64fd0cu;if(!valid_access(r[5],4u))return -3;r[2]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fd0e; /* mov.l @r5,r2 */
L8c64fd0e: *stop=0x8c64fd0eu;r[10]+=r[3];goto L8c64fd10; /* add r3,r10 */
L8c64fd10: *stop=0x8c64fd10u;if(!valid_access(r[5]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c64fd12; /* mov.l @(0x4,r5),r3 */
L8c64fd12: *stop=0x8c64fd12u;r[1]=r[13];goto L8c64fd14; /* mov r13,r1 */
L8c64fd14: *stop=0x8c64fd14u;r[10]<<=2;goto L8c64fd16; /* shll2 r10 */
L8c64fd16: *stop=0x8c64fd16u;r[1]<<=2;goto L8c64fd18; /* shll2 r1 */
L8c64fd18: *stop=0x8c64fd18u;fcopy(f,8u,7u);goto L8c64fd1a; /* fmov fr7,fr8 */
L8c64fd1a: *stop=0x8c64fd1au;r[2]+=r[10];goto L8c64fd1c; /* add r10,r2 */
L8c64fd1c: *stop=0x8c64fd1cu;r[0]=8u;goto L8c64fd1e; /* mov #0x8,r0 */
L8c64fd1e: *stop=0x8c64fd1eu;r[3]+=r[1];goto L8c64fd20; /* add r1,r3 */
L8c64fd20: *stop=0x8c64fd20u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c64fd22; /* fmov.s @(r0,r2),fr3 */
L8c64fd22: *stop=0x8c64fd22u;if(!valid_access(r[3],4u))return -3;r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c64fd24; /* mov.l @r3,r2 */
L8c64fd24: *stop=0x8c64fd24u;r[4]+=28u;goto L8c64fd26; /* add #0x1c,r4 */
L8c64fd26: *stop=0x8c64fd26u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[8],&fr[3],&f->fpscr,&fr[8]);if(status)return status;}goto L8c64fd28; /* fdiv fr3,fr8 */
L8c64fd28: *stop=0x8c64fd28u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c64fd2a; /* mov.l r2,@-r4 */
L8c64fd2a: *stop=0x8c64fd2au;if(!valid_access(r[5]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;goto L8c64fd2c; /* mov.l @(0x8,r5),r3 */
L8c64fd2c: *stop=0x8c64fd2cu;r[3]+=r[1];goto L8c64fd2e; /* add r1,r3 */
L8c64fd2e: *stop=0x8c64fd2eu;if(!valid_access(r[3]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c64fd30; /* mov.w @(0x2,r3),r0 */
L8c64fd30: *stop=0x8c64fd30u;r[3]=r[0];goto L8c64fd32; /* mov r0,r3 */
L8c64fd32: *stop=0x8c64fd32u;*fpul=r[3];goto L8c64fd34; /* lds r3,FPUL */
L8c64fd34: *stop=0x8c64fd34u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64fd36; /* float FPUL,fr3 */
L8c64fd36: *stop=0x8c64fd36u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fd38; /* fmul fr5,fr3 */
L8c64fd38: *stop=0x8c64fd38u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c64fd3a; /* fmov.s fr3,@-r4 */
L8c64fd3a: *stop=0x8c64fd3au;if(!valid_access(r[5]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;goto L8c64fd3c; /* mov.l @(0x8,r5),r3 */
L8c64fd3c: *stop=0x8c64fd3cu;r[1]+=r[3];goto L8c64fd3e; /* add r3,r1 */
L8c64fd3e: *stop=0x8c64fd3eu;if(!valid_access(r[1],2u))return -3;r[3]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;goto L8c64fd40; /* mov.w @r1,r3 */
L8c64fd40: *stop=0x8c64fd40u;*fpul=r[3];goto L8c64fd42; /* lds r3,FPUL */
L8c64fd42: *stop=0x8c64fd42u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64fd44; /* float FPUL,fr3 */
L8c64fd44: *stop=0x8c64fd44u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fd46; /* fmul fr5,fr3 */
L8c64fd46: *stop=0x8c64fd46u;r[0]=4u;goto L8c64fd48; /* mov #0x4,r0 */
L8c64fd48: *stop=0x8c64fd48u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c64fd4a; /* fmov.s fr3,@-r4 */
L8c64fd4a: *stop=0x8c64fd4au;fcopy(f,0u,8u);goto L8c64fd4c; /* fmov fr8,fr0 */
L8c64fd4c: *stop=0x8c64fd4cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;goto L8c64fd4e; /* fmov.s fr8,@-r4 */
L8c64fd4e: *stop=0x8c64fd4eu;if(!valid_access(r[5],4u))return -3;r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fd50; /* mov.l @r5,r3 */
L8c64fd50: *stop=0x8c64fd50u;r[3]+=r[10];goto L8c64fd52; /* add r10,r3 */
L8c64fd52: *stop=0x8c64fd52u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c64fd54; /* fmov.s @(r0,r3),fr3 */
L8c64fd54: *stop=0x8c64fd54u;r[0]=16u;goto L8c64fd56; /* mov #0x10,r0 */
L8c64fd56: *stop=0x8c64fd56u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c64fd58; /* fmov.s @(r0,r14),fr2 */
L8c64fd58: *stop=0x8c64fd58u;r[0]=12u;goto L8c64fd5a; /* mov #0xc,r0 */
L8c64fd5a: *stop=0x8c64fd5au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fd5c; /* fmul fr4,fr3 */
L8c64fd5c: *stop=0x8c64fd5cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fd5e; /* fmac fr0,fr3,fr2 */
L8c64fd5e: *stop=0x8c64fd5eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64fd60; /* fmov.s fr2,@-r4 */
L8c64fd60: *stop=0x8c64fd60u;if(!valid_access(r[5],4u))return -3;r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fd62; /* mov.l @r5,r3 */
L8c64fd62: *stop=0x8c64fd62u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c64fd64; /* fmov.s @(r0,r14),fr2 */
L8c64fd64: *stop=0x8c64fd64u;r[10]+=r[3];goto L8c64fd66; /* add r3,r10 */
L8c64fd66: *stop=0x8c64fd66u;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[10]);if(io_status)return io_status;goto L8c64fd68; /* fmov.s @r10,fr3 */
L8c64fd68: *stop=0x8c64fd68u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fd6a; /* fmul fr6,fr3 */
L8c64fd6a: *stop=0x8c64fd6au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fd6c; /* fmac fr0,fr3,fr2 */
L8c64fd6c: *stop=0x8c64fd6cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64fd6e; /* fmov.s fr2,@-r4 */
L8c64fd6e: *stop=0x8c64fd6eu;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c64fd70; /* mov.l r12,@-r4 */
L8c64fd70: *stop=0x8c64fd70u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c64fd72; /* pref @r4 */
L8c64fd72: *stop=0x8c64fd72u;r[13]+=4294967295u;goto L8c64fd74; /* add #-0x1,r13 */
L8c64fd74: *stop=0x8c64fd74u;*sr=(*sr&~1u)|(r[7]==r[13]);goto L8c64fd76; /* cmp/eq r7,r13 */
L8c64fd76: take=!(*sr&1u);*stop=0x8c64fd78u;r[4]+=32u;if(take)goto L8c64fc92;goto L8c64fd7a; /* bf/s 0x8c64fc92 */
L8c64fd7a: *stop=0x8c64fd7au;r[13]=r[7];goto L8c64fd7c; /* mov r7,r13 */
L8c64fd7c: *stop=0x8c64fd7cu;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c64fd7e; /* shll r13 */
L8c64fd7e: *stop=0x8c64fd7eu;r[3]=r[7];goto L8c64fd80; /* mov r7,r3 */
L8c64fd80: *stop=0x8c64fd80u;if(!valid_access(r[5],4u))return -3;r[2]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fd82; /* mov.l @r5,r2 */
L8c64fd82: *stop=0x8c64fd82u;r[13]+=r[3];goto L8c64fd84; /* add r3,r13 */
L8c64fd84: *stop=0x8c64fd84u;r[12]=r[7];goto L8c64fd86; /* mov r7,r12 */
L8c64fd86: *stop=0x8c64fd86u;if(!valid_access(r[5]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c64fd88; /* mov.l @(0x4,r5),r3 */
L8c64fd88: *stop=0x8c64fd88u;r[13]<<=2;goto L8c64fd8a; /* shll2 r13 */
L8c64fd8a: *stop=0x8c64fd8au;r[12]<<=2;goto L8c64fd8c; /* shll2 r12 */
L8c64fd8c: *stop=0x8c64fd8cu;r[0]=8u;goto L8c64fd8e; /* mov #0x8,r0 */
L8c64fd8e: *stop=0x8c64fd8eu;r[2]+=r[13];goto L8c64fd90; /* add r13,r2 */
L8c64fd90: *stop=0x8c64fd90u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c64fd92; /* fmov.s @(r0,r2),fr3 */
L8c64fd92: *stop=0x8c64fd92u;r[3]+=r[12];goto L8c64fd94; /* add r12,r3 */
L8c64fd94: *stop=0x8c64fd94u;if(!valid_access(r[3],4u))return -3;r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c64fd96; /* mov.l @r3,r2 */
L8c64fd96: *stop=0x8c64fd96u;r[4]+=28u;goto L8c64fd98; /* add #0x1c,r4 */
L8c64fd98: *stop=0x8c64fd98u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[7],&fr[3],&f->fpscr,&fr[7]);if(status)return status;}goto L8c64fd9a; /* fdiv fr3,fr7 */
L8c64fd9a: *stop=0x8c64fd9au;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c64fd9c; /* mov.l r2,@-r4 */
L8c64fd9c: *stop=0x8c64fd9cu;if(!valid_access(r[5]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;goto L8c64fd9e; /* mov.l @(0x8,r5),r3 */
L8c64fd9e: *stop=0x8c64fd9eu;r[3]+=r[12];goto L8c64fda0; /* add r12,r3 */
L8c64fda0: *stop=0x8c64fda0u;if(!valid_access(r[3]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c64fda2; /* mov.w @(0x2,r3),r0 */
L8c64fda2: *stop=0x8c64fda2u;r[3]=r[0];goto L8c64fda4; /* mov r0,r3 */
L8c64fda4: *stop=0x8c64fda4u;*fpul=r[3];goto L8c64fda6; /* lds r3,FPUL */
L8c64fda6: *stop=0x8c64fda6u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64fda8; /* float FPUL,fr3 */
L8c64fda8: *stop=0x8c64fda8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fdaa; /* fmul fr5,fr3 */
L8c64fdaa: *stop=0x8c64fdaau;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c64fdac; /* fmov.s fr3,@-r4 */
L8c64fdac: *stop=0x8c64fdacu;if(!valid_access(r[5]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;goto L8c64fdae; /* mov.l @(0x8,r5),r3 */
L8c64fdae: *stop=0x8c64fdaeu;r[12]+=r[3];goto L8c64fdb0; /* add r3,r12 */
L8c64fdb0: *stop=0x8c64fdb0u;if(!valid_access(r[12],2u))return -3;r[3]=sx(m,bus,&io_status,r[12],2);if(io_status)return io_status;goto L8c64fdb2; /* mov.w @r12,r3 */
L8c64fdb2: *stop=0x8c64fdb2u;*fpul=r[3];goto L8c64fdb4; /* lds r3,FPUL */
L8c64fdb4: *stop=0x8c64fdb4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c64fdb6; /* float FPUL,fr3 */
L8c64fdb6: *stop=0x8c64fdb6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fdb8; /* fmul fr5,fr3 */
L8c64fdb8: *stop=0x8c64fdb8u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c64fdba; /* fmov.s fr3,@-r4 */
L8c64fdba: *stop=0x8c64fdbau;r[0]=4u;goto L8c64fdbc; /* mov #0x4,r0 */
L8c64fdbc: *stop=0x8c64fdbcu;fcopy(f,0u,7u);goto L8c64fdbe; /* fmov fr7,fr0 */
L8c64fdbe: *stop=0x8c64fdbeu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c64fdc0; /* fmov.s fr7,@-r4 */
L8c64fdc0: *stop=0x8c64fdc0u;if(!valid_access(r[5],4u))return -3;r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fdc2; /* mov.l @r5,r3 */
L8c64fdc2: *stop=0x8c64fdc2u;r[3]+=r[13];goto L8c64fdc4; /* add r13,r3 */
L8c64fdc4: *stop=0x8c64fdc4u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c64fdc6; /* fmov.s @(r0,r3),fr3 */
L8c64fdc6: *stop=0x8c64fdc6u;r[0]=16u;goto L8c64fdc8; /* mov #0x10,r0 */
L8c64fdc8: *stop=0x8c64fdc8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c64fdca; /* fmov.s @(r0,r14),fr2 */
L8c64fdca: *stop=0x8c64fdcau;r[0]=12u;goto L8c64fdcc; /* mov #0xc,r0 */
L8c64fdcc: *stop=0x8c64fdccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fdce; /* fmul fr4,fr3 */
L8c64fdce: *stop=0x8c64fdceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fdd0; /* fmac fr0,fr3,fr2 */
L8c64fdd0: *stop=0x8c64fdd0u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64fdd2; /* fmov.s fr2,@-r4 */
L8c64fdd2: *stop=0x8c64fdd2u;if(!valid_access(r[5],4u))return -3;r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fdd4; /* mov.l @r5,r3 */
L8c64fdd4: *stop=0x8c64fdd4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c64fdd6; /* fmov.s @(r0,r14),fr2 */
L8c64fdd6: *stop=0x8c64fdd6u;r[13]+=r[3];goto L8c64fdd8; /* add r3,r13 */
L8c64fdd8: *stop=0x8c64fdd8u;r[3]=rd(m,bus,&io_status,0x8c64fe08u,4);if(io_status)return io_status;goto L8c64fdda; /* mov.l 0x8c64fe08,r3 */
L8c64fdda: *stop=0x8c64fddau;if(!valid_access(r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c64fddc; /* fmov.s @r13,fr3 */
L8c64fddc: *stop=0x8c64fddcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fdde; /* fmul fr6,fr3 */
L8c64fdde: *stop=0x8c64fddeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fde0; /* fmac fr0,fr3,fr2 */
L8c64fde0: *stop=0x8c64fde0u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64fde2; /* fmov.s fr2,@-r4 */
L8c64fde2: *stop=0x8c64fde2u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c64fde4; /* mov.l r3,@-r4 */
L8c64fde4: *stop=0x8c64fde4u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c64fde6; /* pref @r4 */
L8c64fde6: *stop=0x8c64fde6u;r[4]+=32u;goto L8c64fde8; /* add #0x20,r4 */
L8c64fde8: *stop=0x8c64fde8u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[4],4);if(io_status)return io_status;goto L8c64fdea; /* mov.l r4,@r6 */
L8c64fdea: *stop=0x8c64fdeau;if(!valid_access(r[15],4u))return -3;r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c64fdec; /* mov.l @r15+,r10 */
L8c64fdec: *stop=0x8c64fdecu;if(!valid_access(r[15],4u))return -3;r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c64fdee; /* mov.l @r15+,r12 */
L8c64fdee: *stop=0x8c64fdeeu;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c64fdf0; /* mov.l @r15+,r13 */
L8c64fdf0: *stop=0x8c64fdf0u;if(*pr!=entry_pr)return -3;*stop=0x8c64fdf2u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_polygon_emit_colour_8c64fae8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64fae8;
L8c64fae8: *stop=0x8c64fae8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64faea; /* mov.l r14,@-r15 */
L8c64faea: *stop=0x8c64faeau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64faec; /* mov.l r13,@-r15 */
L8c64faec: *stop=0x8c64faecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64faee; /* mov.l r12,@-r15 */
L8c64faee: *stop=0x8c64faeeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64faf0; /* mov.l r11,@-r15 */
L8c64faf0: *stop=0x8c64faf0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64faf2; /* mov.l r10,@-r15 */
L8c64faf2: *stop=0x8c64faf2u;r[15]+=4294967292u;goto L8c64faf4; /* add #-0x4,r15 */
L8c64faf4: *stop=0x8c64faf4u;r[14]=rd(m,bus,&io_status,0x8c64fc54u,4);if(io_status)return io_status;goto L8c64faf6; /* mov.l 0x8c64fc54,r14 */
L8c64faf6: *stop=0x8c64faf6u;r[12]=r[4];goto L8c64faf8; /* mov r4,r12 */
L8c64faf8: *stop=0x8c64faf8u;r[3]=rd(m,bus,&io_status,0x8c64fc58u,4);if(io_status)return io_status;goto L8c64fafa; /* mov.l 0x8c64fc58,r3 */
L8c64fafa: *stop=0x8c64fafau;r[13]=4294967295u;goto L8c64fafc; /* mov #-0x1,r13 */
L8c64fafc: *stop=0x8c64fafcu;r[2]=rd(m,bus,&io_status,0x8c64fc5cu,4);if(io_status)return io_status;goto L8c64fafe; /* mov.l 0x8c64fc5c,r2 */
L8c64fafe: *stop=0x8c64fafeu;r[12]+=4294967295u;goto L8c64fb00; /* add #-0x1,r12 */
L8c64fb00: *stop=0x8c64fb00u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c64fb02; /* fmov.s @r14,fr4 */
L8c64fb02: *stop=0x8c64fb02u;r[7]=0u;goto L8c64fb04; /* mov #0x0,r7 */
L8c64fb04: *stop=0x8c64fb04u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c64fb06; /* fmov.s @r3,fr3 */
L8c64fb06: *stop=0x8c64fb06u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c64fb08; /* fmov.s @r2,fr2 */
L8c64fb08: *stop=0x8c64fb08u;fcopy(f,5u,4u);goto L8c64fb0a; /* fmov fr4,fr5 */
L8c64fb0a: *stop=0x8c64fb0au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c64fb0c; /* fmul fr3,fr5 */
L8c64fb0c: *stop=0x8c64fb0cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c64fb0e; /* fmul fr2,fr4 */
L8c64fb0e: *stop=0x8c64fb0eu;if(!valid_access(r[6],4u))return -3;r[4]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c64fb10; /* mov.l @r6,r4 */
L8c64fb10: *stop=0x8c64fb10u;r[11]=rd(m,bus,&io_status,0x8c64fc60u,4);if(io_status)return io_status;goto L8c64fb12; /* mov.l 0x8c64fc60,r11 */
L8c64fb12: *stop=0x8c64fb12u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c64fb14; /* fldi1 fr6 */
L8c64fb14: *stop=0x8c64fb14u;r[3]=r[7];goto L8c64fb16; /* mov r7,r3 */
L8c64fb16: *stop=0x8c64fb16u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c64fb18; /* shll r3 */
L8c64fb18: *stop=0x8c64fb18u;r[2]=r[7];goto L8c64fb1a; /* mov r7,r2 */
L8c64fb1a: *stop=0x8c64fb1au;if(!valid_access(r[5],4u))return -3;r[1]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fb1c; /* mov.l @r5,r1 */
L8c64fb1c: *stop=0x8c64fb1cu;r[3]+=r[2];goto L8c64fb1e; /* add r2,r3 */
L8c64fb1e: *stop=0x8c64fb1eu;fcopy(f,7u,6u);goto L8c64fb20; /* fmov fr6,fr7 */
L8c64fb20: *stop=0x8c64fb20u;r[3]<<=2;goto L8c64fb22; /* shll2 r3 */
L8c64fb22: *stop=0x8c64fb22u;r[0]=8u;goto L8c64fb24; /* mov #0x8,r0 */
L8c64fb24: *stop=0x8c64fb24u;r[3]+=r[1];goto L8c64fb26; /* add r1,r3 */
L8c64fb26: *stop=0x8c64fb26u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c64fb28; /* fmov.s @(r0,r3),fr3 */
L8c64fb28: *stop=0x8c64fb28u;if(!valid_access(r[5]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c64fb2a; /* mov.l @(0x4,r5),r3 */
L8c64fb2a: *stop=0x8c64fb2au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[7],&fr[3],&f->fpscr,&fr[7]);if(status)return status;}goto L8c64fb2c; /* fdiv fr3,fr7 */
L8c64fb2c: *stop=0x8c64fb2cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64fb2e; /* tst r3,r3 */
L8c64fb2e: take=(*sr&1u);*stop=0x8c64fb30u;r[4]+=28u;if(take)goto L8c64fb40;goto L8c64fb32; /* bt/s 0x8c64fb40 */
L8c64fb32: *stop=0x8c64fb32u;if(!valid_access(r[5]+4u,4u))return -3;r[2]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c64fb34; /* mov.l @(0x4,r5),r2 */
L8c64fb34: *stop=0x8c64fb34u;r[3]=r[7];goto L8c64fb36; /* mov r7,r3 */
L8c64fb36: *stop=0x8c64fb36u;r[3]<<=2;goto L8c64fb38; /* shll2 r3 */
L8c64fb38: *stop=0x8c64fb38u;r[3]+=r[2];goto L8c64fb3a; /* add r2,r3 */
L8c64fb3a: *stop=0x8c64fb3au;if(!valid_access(r[3],4u))return -3;r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c64fb3c; /* mov.l @r3,r1 */
L8c64fb3c: *stop=0x8c64fb3eu;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c64fb42; /* bra 0x8c64fb42 */
L8c64fb40: *stop=0x8c64fb40u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[13],4);if(io_status)return io_status;goto L8c64fb42; /* mov.l r13,@-r4 */
L8c64fb42: *stop=0x8c64fb42u;r[10]=r[7];goto L8c64fb44; /* mov r7,r10 */
L8c64fb44: *stop=0x8c64fb44u;r[3]=r[7];goto L8c64fb46; /* mov r7,r3 */
L8c64fb46: *stop=0x8c64fb46u;*sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c64fb48; /* shll r10 */
L8c64fb48: *stop=0x8c64fb48u;fcopy(f,0u,7u);goto L8c64fb4a; /* fmov fr7,fr0 */
L8c64fb4a: *stop=0x8c64fb4au;r[10]+=r[3];goto L8c64fb4c; /* add r3,r10 */
L8c64fb4c: *stop=0x8c64fb4cu;r[10]<<=2;goto L8c64fb4e; /* shll2 r10 */
L8c64fb4e: *stop=0x8c64fb4eu;r[0]=4u;goto L8c64fb50; /* mov #0x4,r0 */
L8c64fb50: *stop=0x8c64fb50u;r[4]+=4294967288u;goto L8c64fb52; /* add #-0x8,r4 */
L8c64fb52: *stop=0x8c64fb52u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c64fb54; /* fmov.s fr7,@-r4 */
L8c64fb54: *stop=0x8c64fb54u;if(!valid_access(r[5],4u))return -3;r[2]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fb56; /* mov.l @r5,r2 */
L8c64fb56: *stop=0x8c64fb56u;r[2]+=r[10];goto L8c64fb58; /* add r10,r2 */
L8c64fb58: *stop=0x8c64fb58u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c64fb5a; /* fmov.s @(r0,r2),fr3 */
L8c64fb5a: *stop=0x8c64fb5au;r[0]=16u;goto L8c64fb5c; /* mov #0x10,r0 */
L8c64fb5c: *stop=0x8c64fb5cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c64fb5e; /* fmov.s @(r0,r14),fr2 */
L8c64fb5e: *stop=0x8c64fb5eu;r[0]=12u;goto L8c64fb60; /* mov #0xc,r0 */
L8c64fb60: *stop=0x8c64fb60u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fb62; /* fmul fr4,fr3 */
L8c64fb62: *stop=0x8c64fb62u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fb64; /* fmac fr0,fr3,fr2 */
L8c64fb64: *stop=0x8c64fb64u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64fb66; /* fmov.s fr2,@-r4 */
L8c64fb66: *stop=0x8c64fb66u;if(!valid_access(r[5],4u))return -3;r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fb68; /* mov.l @r5,r3 */
L8c64fb68: *stop=0x8c64fb68u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c64fb6a; /* fmov.s @(r0,r14),fr2 */
L8c64fb6a: *stop=0x8c64fb6au;r[10]+=r[3];goto L8c64fb6c; /* add r3,r10 */
L8c64fb6c: *stop=0x8c64fb6cu;if(!valid_access(r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[10]);if(io_status)return io_status;goto L8c64fb6e; /* fmov.s @r10,fr3 */
L8c64fb6e: *stop=0x8c64fb6eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fb70; /* fmul fr5,fr3 */
L8c64fb70: *stop=0x8c64fb70u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fb72; /* fmac fr0,fr3,fr2 */
L8c64fb72: *stop=0x8c64fb72u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64fb74; /* fmov.s fr2,@-r4 */
L8c64fb74: *stop=0x8c64fb74u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[11],4);if(io_status)return io_status;goto L8c64fb76; /* mov.l r11,@-r4 */
L8c64fb76: *stop=0x8c64fb76u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c64fb78; /* pref @r4 */
L8c64fb78: *stop=0x8c64fb78u;r[7]+=1u;goto L8c64fb7a; /* add #0x1,r7 */
L8c64fb7a: *stop=0x8c64fb7au;*sr=(*sr&~1u)|(r[12]==r[7]);goto L8c64fb7c; /* cmp/eq r12,r7 */
L8c64fb7c: take=(*sr&1u);*stop=0x8c64fb7eu;r[4]+=32u;if(take)goto L8c64fbe2;goto L8c64fb80; /* bt/s 0x8c64fbe2 */
L8c64fb80: *stop=0x8c64fb80u;r[10]=r[12];goto L8c64fb82; /* mov r12,r10 */
L8c64fb82: *stop=0x8c64fb82u;*sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c64fb84; /* shll r10 */
L8c64fb84: *stop=0x8c64fb84u;r[3]=r[12];goto L8c64fb86; /* mov r12,r3 */
L8c64fb86: *stop=0x8c64fb86u;if(!valid_access(r[5],4u))return -3;r[2]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fb88; /* mov.l @r5,r2 */
L8c64fb88: *stop=0x8c64fb88u;r[10]+=r[3];goto L8c64fb8a; /* add r3,r10 */
L8c64fb8a: *stop=0x8c64fb8au;fcopy(f,7u,6u);goto L8c64fb8c; /* fmov fr6,fr7 */
L8c64fb8c: *stop=0x8c64fb8cu;r[10]<<=2;goto L8c64fb8e; /* shll2 r10 */
L8c64fb8e: *stop=0x8c64fb8eu;if(!valid_access(r[5]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c64fb90; /* mov.l @(0x4,r5),r3 */
L8c64fb90: *stop=0x8c64fb90u;r[0]=8u;goto L8c64fb92; /* mov #0x8,r0 */
L8c64fb92: *stop=0x8c64fb92u;r[2]+=r[10];goto L8c64fb94; /* add r10,r2 */
L8c64fb94: *stop=0x8c64fb94u;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c64fb96; /* fmov.s @(r0,r2),fr3 */
L8c64fb96: *stop=0x8c64fb96u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64fb98; /* tst r3,r3 */
L8c64fb98: *stop=0x8c64fb98u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[7],&fr[3],&f->fpscr,&fr[7]);if(status)return status;}goto L8c64fb9a; /* fdiv fr3,fr7 */
L8c64fb9a: take=(*sr&1u);*stop=0x8c64fb9cu;r[4]+=28u;if(take)goto L8c64fbac;goto L8c64fb9e; /* bt/s 0x8c64fbac */
L8c64fb9e: *stop=0x8c64fb9eu;if(!valid_access(r[5]+4u,4u))return -3;r[2]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c64fba0; /* mov.l @(0x4,r5),r2 */
L8c64fba0: *stop=0x8c64fba0u;r[3]=r[12];goto L8c64fba2; /* mov r12,r3 */
L8c64fba2: *stop=0x8c64fba2u;r[3]<<=2;goto L8c64fba4; /* shll2 r3 */
L8c64fba4: *stop=0x8c64fba4u;r[3]+=r[2];goto L8c64fba6; /* add r2,r3 */
L8c64fba6: *stop=0x8c64fba6u;if(!valid_access(r[3],4u))return -3;r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c64fba8; /* mov.l @r3,r1 */
L8c64fba8: *stop=0x8c64fbaau;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c64fbae; /* bra 0x8c64fbae */
L8c64fbac: *stop=0x8c64fbacu;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[13],4);if(io_status)return io_status;goto L8c64fbae; /* mov.l r13,@-r4 */
L8c64fbae: *stop=0x8c64fbaeu;r[0]=4u;goto L8c64fbb0; /* mov #0x4,r0 */
L8c64fbb0: *stop=0x8c64fbb0u;fcopy(f,0u,7u);goto L8c64fbb2; /* fmov fr7,fr0 */
L8c64fbb2: *stop=0x8c64fbb2u;r[4]+=4294967288u;goto L8c64fbb4; /* add #-0x8,r4 */
L8c64fbb4: *stop=0x8c64fbb4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c64fbb6; /* fmov.s fr7,@-r4 */
L8c64fbb6: *stop=0x8c64fbb6u;if(!valid_access(r[5],4u))return -3;r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fbb8; /* mov.l @r5,r3 */
L8c64fbb8: *stop=0x8c64fbb8u;r[3]+=r[10];goto L8c64fbba; /* add r10,r3 */
L8c64fbba: *stop=0x8c64fbbau;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c64fbbc; /* fmov.s @(r0,r3),fr3 */
L8c64fbbc: *stop=0x8c64fbbcu;r[0]=16u;goto L8c64fbbe; /* mov #0x10,r0 */
L8c64fbbe: *stop=0x8c64fbbeu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c64fbc0; /* fmov.s @(r0,r14),fr2 */
L8c64fbc0: *stop=0x8c64fbc0u;r[0]=12u;goto L8c64fbc2; /* mov #0xc,r0 */
L8c64fbc2: *stop=0x8c64fbc2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fbc4; /* fmul fr4,fr3 */
L8c64fbc4: *stop=0x8c64fbc4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fbc6; /* fmac fr0,fr3,fr2 */
L8c64fbc6: *stop=0x8c64fbc6u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64fbc8; /* fmov.s fr2,@-r4 */
L8c64fbc8: *stop=0x8c64fbc8u;if(!valid_access(r[5],4u))return -3;r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fbca; /* mov.l @r5,r3 */
L8c64fbca: *stop=0x8c64fbcau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c64fbcc; /* fmov.s @(r0,r14),fr2 */
L8c64fbcc: *stop=0x8c64fbccu;r[3]+=r[10];goto L8c64fbce; /* add r10,r3 */
L8c64fbce: *stop=0x8c64fbceu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c64fbd0; /* fmov.s @r3,fr3 */
L8c64fbd0: *stop=0x8c64fbd0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fbd2; /* fmul fr5,fr3 */
L8c64fbd2: *stop=0x8c64fbd2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fbd4; /* fmac fr0,fr3,fr2 */
L8c64fbd4: *stop=0x8c64fbd4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64fbd6; /* fmov.s fr2,@-r4 */
L8c64fbd6: *stop=0x8c64fbd6u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[11],4);if(io_status)return io_status;goto L8c64fbd8; /* mov.l r11,@-r4 */
L8c64fbd8: *stop=0x8c64fbd8u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c64fbda; /* pref @r4 */
L8c64fbda: *stop=0x8c64fbdau;r[12]+=4294967295u;goto L8c64fbdc; /* add #-0x1,r12 */
L8c64fbdc: *stop=0x8c64fbdcu;*sr=(*sr&~1u)|(r[7]==r[12]);goto L8c64fbde; /* cmp/eq r7,r12 */
L8c64fbde: take=!(*sr&1u);*stop=0x8c64fbe0u;r[4]+=32u;if(take)goto L8c64fb14;goto L8c64fbe2; /* bf/s 0x8c64fb14 */
L8c64fbe2: *stop=0x8c64fbe2u;r[3]=r[7];goto L8c64fbe4; /* mov r7,r3 */
L8c64fbe4: *stop=0x8c64fbe4u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c64fbe6; /* shll r3 */
L8c64fbe6: *stop=0x8c64fbe6u;r[2]=r[7];goto L8c64fbe8; /* mov r7,r2 */
L8c64fbe8: *stop=0x8c64fbe8u;r[3]+=r[2];goto L8c64fbea; /* add r2,r3 */
L8c64fbea: *stop=0x8c64fbeau;r[3]<<=2;goto L8c64fbec; /* shll2 r3 */
L8c64fbec: *stop=0x8c64fbecu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64fbee; /* mov.l r3,@r15 */
L8c64fbee: *stop=0x8c64fbeeu;r[0]=8u;goto L8c64fbf0; /* mov #0x8,r0 */
L8c64fbf0: *stop=0x8c64fbf0u;if(!valid_access(r[5],4u))return -3;r[2]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fbf2; /* mov.l @r5,r2 */
L8c64fbf2: *stop=0x8c64fbf2u;r[3]+=r[2];goto L8c64fbf4; /* add r2,r3 */
L8c64fbf4: *stop=0x8c64fbf4u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c64fbf6; /* fmov.s @(r0,r3),fr3 */
L8c64fbf6: *stop=0x8c64fbf6u;if(!valid_access(r[5]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c64fbf8; /* mov.l @(0x4,r5),r3 */
L8c64fbf8: *stop=0x8c64fbf8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c64fbfa; /* fdiv fr3,fr6 */
L8c64fbfa: *stop=0x8c64fbfau;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c64fbfc; /* tst r3,r3 */
L8c64fbfc: take=(*sr&1u);*stop=0x8c64fbfeu;r[4]+=28u;if(take)goto L8c64fc0c;goto L8c64fc00; /* bt/s 0x8c64fc0c */
L8c64fc00: *stop=0x8c64fc00u;if(!valid_access(r[5]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c64fc02; /* mov.l @(0x4,r5),r3 */
L8c64fc02: *stop=0x8c64fc02u;r[7]<<=2;goto L8c64fc04; /* shll2 r7 */
L8c64fc04: *stop=0x8c64fc04u;r[7]+=r[3];goto L8c64fc06; /* add r3,r7 */
L8c64fc06: *stop=0x8c64fc06u;if(!valid_access(r[7],4u))return -3;r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c64fc08; /* mov.l @r7,r2 */
L8c64fc08: *stop=0x8c64fc0au;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c64fc0e; /* bra 0x8c64fc0e */
L8c64fc0c: *stop=0x8c64fc0cu;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[13],4);if(io_status)return io_status;goto L8c64fc0e; /* mov.l r13,@-r4 */
L8c64fc0e: *stop=0x8c64fc0eu;r[0]=4u;goto L8c64fc10; /* mov #0x4,r0 */
L8c64fc10: *stop=0x8c64fc10u;fcopy(f,0u,6u);goto L8c64fc12; /* fmov fr6,fr0 */
L8c64fc12: *stop=0x8c64fc12u;r[4]+=4294967288u;goto L8c64fc14; /* add #-0x8,r4 */
L8c64fc14: *stop=0x8c64fc14u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c64fc16; /* fmov.s fr6,@-r4 */
L8c64fc16: *stop=0x8c64fc16u;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c64fc18; /* mov.l @r15,r3 */
L8c64fc18: *stop=0x8c64fc18u;if(!valid_access(r[5],4u))return -3;r[2]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fc1a; /* mov.l @r5,r2 */
L8c64fc1a: *stop=0x8c64fc1au;r[2]+=r[3];goto L8c64fc1c; /* add r3,r2 */
L8c64fc1c: *stop=0x8c64fc1cu;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c64fc1e; /* fmov.s @(r0,r2),fr3 */
L8c64fc1e: *stop=0x8c64fc1eu;r[0]=16u;goto L8c64fc20; /* mov #0x10,r0 */
L8c64fc20: *stop=0x8c64fc20u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c64fc22; /* fmov.s @(r0,r14),fr2 */
L8c64fc22: *stop=0x8c64fc22u;r[0]=12u;goto L8c64fc24; /* mov #0xc,r0 */
L8c64fc24: *stop=0x8c64fc24u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fc26; /* fmul fr4,fr3 */
L8c64fc26: *stop=0x8c64fc26u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fc28; /* fmac fr0,fr3,fr2 */
L8c64fc28: *stop=0x8c64fc28u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64fc2a; /* fmov.s fr2,@-r4 */
L8c64fc2a: *stop=0x8c64fc2au;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c64fc2c; /* mov.l @r15,r3 */
L8c64fc2c: *stop=0x8c64fc2cu;if(!valid_access(r[5],4u))return -3;r[2]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fc2e; /* mov.l @r5,r2 */
L8c64fc2e: *stop=0x8c64fc2eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c64fc30; /* fmov.s @(r0,r14),fr2 */
L8c64fc30: *stop=0x8c64fc30u;r[2]+=r[3];goto L8c64fc32; /* add r3,r2 */
L8c64fc32: *stop=0x8c64fc32u;r[3]=rd(m,bus,&io_status,0x8c64fc64u,4);if(io_status)return io_status;goto L8c64fc34; /* mov.l 0x8c64fc64,r3 */
L8c64fc34: *stop=0x8c64fc34u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c64fc36; /* fmov.s @r2,fr3 */
L8c64fc36: *stop=0x8c64fc36u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fc38; /* fmul fr5,fr3 */
L8c64fc38: *stop=0x8c64fc38u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fc3a; /* fmac fr0,fr3,fr2 */
L8c64fc3a: *stop=0x8c64fc3au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64fc3c; /* fmov.s fr2,@-r4 */
L8c64fc3c: *stop=0x8c64fc3cu;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c64fc3e; /* mov.l r3,@-r4 */
L8c64fc3e: *stop=0x8c64fc3eu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c64fc40; /* pref @r4 */
L8c64fc40: *stop=0x8c64fc40u;r[4]+=32u;goto L8c64fc42; /* add #0x20,r4 */
L8c64fc42: *stop=0x8c64fc42u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[4],4);if(io_status)return io_status;goto L8c64fc44; /* mov.l r4,@r6 */
L8c64fc44: *stop=0x8c64fc44u;r[15]+=4u;goto L8c64fc46; /* add #0x4,r15 */
L8c64fc46: *stop=0x8c64fc46u;if(!valid_access(r[15],4u))return -3;r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c64fc48; /* mov.l @r15+,r10 */
L8c64fc48: *stop=0x8c64fc48u;if(!valid_access(r[15],4u))return -3;r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c64fc4a; /* mov.l @r15+,r11 */
L8c64fc4a: *stop=0x8c64fc4au;if(!valid_access(r[15],4u))return -3;r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c64fc4c; /* mov.l @r15+,r12 */
L8c64fc4c: *stop=0x8c64fc4cu;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c64fc4e; /* mov.l @r15+,r13 */
L8c64fc4e: *stop=0x8c64fc4eu;if(*pr!=entry_pr)return -3;*stop=0x8c64fc50u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_polygon_emit_projected_8c650144(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c650144;
L8c650144: *stop=0x8c650144u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c650146; /* mov.l r14,@-r15 */
L8c650146: *stop=0x8c650146u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c650148; /* mov.l r13,@-r15 */
L8c650148: *stop=0x8c650148u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65014a; /* mov.l r12,@-r15 */
L8c65014a: *stop=0x8c65014au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c65014c; /* mov.l r11,@-r15 */
L8c65014c: *stop=0x8c65014cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c65014e; /* mov.l r10,@-r15 */
L8c65014e: *stop=0x8c65014eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c650150; /* mov.l r9,@-r15 */
L8c650150: *stop=0x8c650150u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c650152; /* mov.l r8,@-r15 */
L8c650152: *stop=0x8c650152u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c650154; /* fmov.s fr15,@-r15 */
L8c650154: *stop=0x8c650154u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c650156; /* fmov.s fr14,@-r15 */
L8c650156: *stop=0x8c650156u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c650158; /* fmov.s fr13,@-r15 */
L8c650158: *stop=0x8c650158u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c65015a; /* fmov.s fr12,@-r15 */
L8c65015a: *stop=0x8c65015au;r[15]+=4294967196u;goto L8c65015c; /* add #-0x64,r15 */
L8c65015c: *stop=0x8c65015cu;r[14]=rd(m,bus,&io_status,0x8c6504d4u,4);if(io_status)return io_status;goto L8c65015e; /* mov.l 0x8c6504d4,r14 */
L8c65015e: *stop=0x8c65015eu;r[0]=0x8c6504d0u;goto L8c650160; /* mova 0x8c6504d0,r0 */
L8c650160: *stop=0x8c650160u;r[3]=rd(m,bus,&io_status,0x8c6504d8u,4);if(io_status)return io_status;goto L8c650162; /* mov.l 0x8c6504d8,r3 */
L8c650162: *stop=0x8c650162u;r[13]=r[4];goto L8c650164; /* mov r4,r13 */
L8c650164: *stop=0x8c650164u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c650166; /* fmov.s @r14,fr4 */
L8c650166: *stop=0x8c650166u;r[7]=0u;goto L8c650168; /* mov #0x0,r7 */
L8c650168: *stop=0x8c650168u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c65016a; /* fmov.s @r0,fr5 */
L8c65016a: *stop=0x8c65016au;r[0]=0x8c6504e8u;goto L8c65016c; /* mova 0x8c6504e8,r0 */
L8c65016c: *stop=0x8c65016cu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c65016e; /* fmov.s @r3,fr3 */
L8c65016e: *stop=0x8c65016eu;r[13]+=4294967295u;goto L8c650170; /* add #-0x1,r13 */
L8c650170: *stop=0x8c650170u;if(!valid_access(r[5],4u))return -3;r[1]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c650172; /* mov.l @r5,r1 */
L8c650172: *stop=0x8c650172u;r[2]=rd(m,bus,&io_status,0x8c6504dcu,4);if(io_status)return io_status;goto L8c650174; /* mov.l 0x8c6504dc,r2 */
L8c650174: *stop=0x8c650174u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;goto L8c650176; /* fmov.s @r0,fr7 */
L8c650176: *stop=0x8c650176u;r[0]=8u;goto L8c650178; /* mov #0x8,r0 */
L8c650178: *stop=0x8c650178u;r[11]=rd(m,bus,&io_status,0x8c6504e4u,4);if(io_status)return io_status;goto L8c65017a; /* mov.l 0x8c6504e4,r11 */
L8c65017a: *stop=0x8c65017au;fcopy(f,6u,4u);goto L8c65017c; /* fmov fr4,fr6 */
L8c65017c: *stop=0x8c65017cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c65017e; /* fmul fr3,fr6 */
L8c65017e: *stop=0x8c65017eu;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[11]);if(io_status)return io_status;goto L8c650180; /* fmov.s @r11,fr10 */
L8c650180: *stop=0x8c650180u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c650182; /* fmov.s @r2,fr2 */
L8c650182: *stop=0x8c650182u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][8]=0x3f800000u;goto L8c650184; /* fldi1 fr8 */
L8c650184: *stop=0x8c650184u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[8],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c650186; /* fadd fr8,fr8 */
L8c650186: *stop=0x8c650186u;fcopy(f,9u,4u);goto L8c650188; /* fmov fr4,fr9 */
L8c650188: *stop=0x8c650188u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[2],&f->fpscr,&fr[9]);if(status)return status;}goto L8c65018a; /* fmul fr2,fr9 */
L8c65018a: *stop=0x8c65018au;r[10]=rd(m,bus,&io_status,0x8c6504e0u,4);if(io_status)return io_status;goto L8c65018c; /* mov.l 0x8c6504e0,r10 */
L8c65018c: *stop=0x8c65018cu;r[12]=rd(m,bus,&io_status,0x8c6504ecu,4);if(io_status)return io_status;goto L8c65018e; /* mov.l 0x8c6504ec,r12 */
L8c65018e: *stop=0x8c65018eu;if(!valid_access(r[6],4u))return -3;r[4]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c650190; /* mov.l @r6,r4 */
L8c650190: *stop=0x8c650190u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c650192; /* mov.l r1,@(0x8,r15) */
L8c650192: *stop=0x8c650192u;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c650194; /* fmov.s @(r0,r1),fr3 */
L8c650194: *stop=0x8c650194u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][10],&f->fpscr,sr,0);if(status)return status;goto L8c650196; /* fcmp/eq fr10,fr3 */
L8c650196: take=!(*sr&1u);*stop=0x8c650198u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;if(take)goto L8c6501b4;goto L8c65019a; /* bf/s 0x8c6501b4 */
L8c65019a: *stop=0x8c65019au;r[1]=r[13];goto L8c65019c; /* mov r13,r1 */
L8c65019c: *stop=0x8c65019cu;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c65019e; /* shll r1 */
L8c65019e: *stop=0x8c65019eu;r[0]=r[13];goto L8c6501a0; /* mov r13,r0 */
L8c6501a0: *stop=0x8c6501a0u;r[1]+=r[0];goto L8c6501a2; /* add r0,r1 */
L8c6501a2: *stop=0x8c6501a2u;if(!valid_access(r[15]+8u,4u))return -3;r[0]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6501a4; /* mov.l @(0x8,r15),r0 */
L8c6501a4: *stop=0x8c6501a4u;r[1]<<=2;goto L8c6501a6; /* shll2 r1 */
L8c6501a6: *stop=0x8c6501a6u;r[1]+=r[0];goto L8c6501a8; /* add r0,r1 */
L8c6501a8: *stop=0x8c6501a8u;r[0]=8u;goto L8c6501aa; /* mov #0x8,r0 */
L8c6501aa: *stop=0x8c6501aau;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6501ac; /* fmov.s @(r0,r1),fr3 */
L8c6501ac: *stop=0x8c6501acu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][10],&f->fpscr,sr,0);if(status)return status;goto L8c6501ae; /* fcmp/eq fr10,fr3 */
L8c6501ae: if(!(*sr&1u))goto L8c6501b4;goto L8c6501b0; /* bf 0x8c6501b4 */
L8c6501b0: *stop=0x8c6501b2u;goto L8c650342; /* bra 0x8c650342 */
L8c6501b4: *stop=0x8c6501b4u;if(!valid_access(r[5],4u))return -3;r[8]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c6501b6; /* mov.l @r5,r8 */
L8c6501b6: *stop=0x8c6501b6u;r[0]=8u;goto L8c6501b8; /* mov #0x8,r0 */
L8c6501b8: *stop=0x8c6501b8u;fcopy(f,11u,4u);goto L8c6501ba; /* fmov fr4,fr11 */
L8c6501ba: *stop=0x8c6501bau;r[9]=r[13];goto L8c6501bc; /* mov r13,r9 */
L8c6501bc: *stop=0x8c6501bcu;if(!valid_access(r[0]+r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[8]);if(io_status)return io_status;goto L8c6501be; /* fmov.s @(r0,r8),fr3 */
L8c6501be: *stop=0x8c6501beu;*sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c6501c0; /* shll r9 */
L8c6501c0: *stop=0x8c6501c0u;r[3]=r[13];goto L8c6501c2; /* mov r13,r3 */
L8c6501c2: *stop=0x8c6501c2u;r[9]+=r[3];goto L8c6501c4; /* add r3,r9 */
L8c6501c4: *stop=0x8c6501c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[11],&fr[3],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6501c6; /* fdiv fr3,fr11 */
L8c6501c6: *stop=0x8c6501c6u;r[9]<<=2;goto L8c6501c8; /* shll2 r9 */
L8c6501c8: *stop=0x8c6501c8u;r[9]+=r[8];goto L8c6501ca; /* add r8,r9 */
L8c6501ca: *stop=0x8c6501cau;fcopy(f,10u,4u);goto L8c6501cc; /* fmov fr4,fr10 */
L8c6501cc: *stop=0x8c6501ccu;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6501ce; /* fmov.s @(r0,r9),fr3 */
L8c6501ce: *stop=0x8c6501ceu;r[0]=12u;goto L8c6501d0; /* mov #0xc,r0 */
L8c6501d0: *stop=0x8c6501d0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]+r[14]);if(io_status)return io_status;goto L8c6501d2; /* fmov.s @(r0,r14),fr14 */
L8c6501d2: *stop=0x8c6501d2u;r[0]=4u;goto L8c6501d4; /* mov #0x4,r0 */
L8c6501d4: *stop=0x8c6501d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[10],&fr[3],&f->fpscr,&fr[10]);if(status)return status;}goto L8c6501d6; /* fdiv fr3,fr10 */
L8c6501d6: *stop=0x8c6501d6u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6501d8; /* fmov.s @r8,fr3 */
L8c6501d8: *stop=0x8c6501d8u;fcopy(f,15u,14u);goto L8c6501da; /* fmov fr14,fr15 */
L8c6501da: *stop=0x8c6501dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6501dc; /* fmul fr6,fr3 */
L8c6501dc: *stop=0x8c6501dcu;fcopy(f,0u,11u);goto L8c6501de; /* fmov fr11,fr0 */
L8c6501de: *stop=0x8c6501deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[15],&f->fpscr,&fr[15]);if(status)return status;}goto L8c6501e0; /* fmac fr0,fr3,fr15 */
L8c6501e0: *stop=0x8c6501e0u;if(!valid_access(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[9]);if(io_status)return io_status;goto L8c6501e2; /* fmov.s @r9,fr3 */
L8c6501e2: *stop=0x8c6501e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6501e4; /* fmul fr6,fr3 */
L8c6501e4: *stop=0x8c6501e4u;fcopy(f,0u,10u);goto L8c6501e6; /* fmov fr10,fr0 */
L8c6501e6: *stop=0x8c6501e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[14],&f->fpscr,&fr[14]);if(status)return status;}goto L8c6501e8; /* fmac fr0,fr3,fr14 */
L8c6501e8: *stop=0x8c6501e8u;if(!valid_access(r[0]+r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[8]);if(io_status)return io_status;goto L8c6501ea; /* fmov.s @(r0,r8),fr3 */
L8c6501ea: *stop=0x8c6501eau;r[0]=16u;goto L8c6501ec; /* mov #0x10,r0 */
L8c6501ec: *stop=0x8c6501ecu;fcopy(f,0u,11u);goto L8c6501ee; /* fmov fr11,fr0 */
L8c6501ee: *stop=0x8c6501eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6501f0; /* fmul fr9,fr3 */
L8c6501f0: *stop=0x8c6501f0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]+r[14]);if(io_status)return io_status;goto L8c6501f2; /* fmov.s @(r0,r14),fr12 */
L8c6501f2: *stop=0x8c6501f2u;fcopy(f,13u,12u);goto L8c6501f4; /* fmov fr12,fr13 */
L8c6501f4: *stop=0x8c6501f4u;r[0]=4u;goto L8c6501f6; /* mov #0x4,r0 */
L8c6501f6: *stop=0x8c6501f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[13],&f->fpscr,&fr[13]);if(status)return status;}goto L8c6501f8; /* fmac fr0,fr3,fr13 */
L8c6501f8: *stop=0x8c6501f8u;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6501fa; /* fmov.s @(r0,r9),fr3 */
L8c6501fa: *stop=0x8c6501fau;r[0]=60u;goto L8c6501fc; /* mov #0x3c,r0 */
L8c6501fc: *stop=0x8c6501fcu;fcopy(f,0u,10u);goto L8c6501fe; /* fmov fr10,fr0 */
L8c6501fe: *stop=0x8c6501feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c650200; /* fmul fr9,fr3 */
L8c650200: *stop=0x8c650200u;fcopy(f,2u,13u);goto L8c650202; /* fmov fr13,fr2 */
L8c650202: *stop=0x8c650202u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[12],&f->fpscr,&fr[12]);if(status)return status;}goto L8c650204; /* fmac fr0,fr3,fr12 */
L8c650204: *stop=0x8c650204u;fcopy(f,3u,15u);goto L8c650206; /* fmov fr15,fr3 */
L8c650206: *stop=0x8c650206u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[14],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c650208; /* fsub fr14,fr3 */
L8c650208: *stop=0x8c650208u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[12],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c65020a; /* fsub fr12,fr2 */
L8c65020a: *stop=0x8c65020au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65020c; /* fmov fr3,@(r0,r15) */
L8c65020c: *stop=0x8c65020cu;r[0]=56u;goto L8c65020e; /* mov #0x38,r0 */
L8c65020e: *stop=0x8c65020eu;fcopy(f,0u,3u);goto L8c650210; /* fmov fr3,fr0 */
L8c650210: *stop=0x8c650210u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c650212; /* fmov fr2,@(r0,r15) */
L8c650212: *stop=0x8c650212u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c650214; /* fmul fr2,fr2 */
L8c650214: *stop=0x8c650214u;r[0]=72u;goto L8c650216; /* mov #0x48,r0 */
L8c650216: *stop=0x8c650216u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c650218; /* fmac fr0,fr3,fr2 */
L8c650218: *stop=0x8c650218u;fcopy(f,3u,4u);goto L8c65021a; /* fmov fr4,fr3 */
L8c65021a: *stop=0x8c65021au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c65021c; /* fsqrt fr2 */
L8c65021c: *stop=0x8c65021cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c65021e; /* fdiv fr2,fr3 */
L8c65021e: *stop=0x8c65021eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650220; /* fmov fr3,@(r0,r15) */
L8c650220: *stop=0x8c650220u;r[0]=56u;goto L8c650222; /* mov #0x38,r0 */
L8c650222: *stop=0x8c650222u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c650224; /* fmov.s @(r0,r15),fr2 */
L8c650224: *stop=0x8c650224u;r[0]=68u;goto L8c650226; /* mov #0x44,r0 */
L8c650226: *stop=0x8c650226u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c650228; /* fmul fr3,fr2 */
L8c650228: *stop=0x8c650228u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c65022a; /* fmul fr7,fr2 */
L8c65022a: *stop=0x8c65022au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c65022c; /* fmov fr2,@(r0,r15) */
L8c65022c: *stop=0x8c65022cu;r[0]=60u;goto L8c65022e; /* mov #0x3c,r0 */
L8c65022e: *stop=0x8c65022eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c650230; /* fmov.s @(r0,r15),fr1 */
L8c650230: *stop=0x8c650230u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c650232; /* fmul fr3,fr1 */
L8c650232: *stop=0x8c650232u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c650234; /* fmul fr7,fr1 */
L8c650234: *stop=0x8c650234u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c650236; /* fmul fr8,fr2 */
L8c650236: *stop=0x8c650236u;r[8]=r[13];goto L8c650238; /* mov r13,r8 */
L8c650238: *stop=0x8c650238u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto L8c65023a; /* fneg fr1 */
L8c65023a: *stop=0x8c65023au;r[0]=64u;goto L8c65023c; /* mov #0x40,r0 */
L8c65023c: *stop=0x8c65023cu;r[8]<<=2;goto L8c65023e; /* shll2 r8 */
L8c65023e: *stop=0x8c65023eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c650240; /* fmov fr1,@(r0,r15) */
L8c650240: *stop=0x8c650240u;r[0]=92u;goto L8c650242; /* mov #0x5c,r0 */
L8c650242: *stop=0x8c650242u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c650244; /* fmov fr2,@(r0,r15) */
L8c650244: *stop=0x8c650244u;r[0]=64u;goto L8c650246; /* mov #0x40,r0 */
L8c650246: *stop=0x8c650246u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650248; /* fmov.s @(r0,r15),fr3 */
L8c650248: *stop=0x8c650248u;r[0]=96u;goto L8c65024a; /* mov #0x60,r0 */
L8c65024a: *stop=0x8c65024au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c65024c; /* fmul fr8,fr3 */
L8c65024c: *stop=0x8c65024cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65024e; /* fmov fr3,@(r0,r15) */
L8c65024e: *stop=0x8c65024eu;r[0]=84u;goto L8c650250; /* mov #0x54,r0 */
L8c650250: *stop=0x8c650250u;if(!valid_access(r[5]+4u,4u))return -3;r[9]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c650252; /* mov.l @(0x4,r5),r9 */
L8c650252: *stop=0x8c650252u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[9],4);if(io_status)return io_status;goto L8c650254; /* mov.l r9,@(0x8,r15) */
L8c650254: *stop=0x8c650254u;if(!valid_access(r[9],4u))return -3;r[9]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c650256; /* mov.l @r9,r9 */
L8c650256: *stop=0x8c650256u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c650258; /* mov.l r8,@r15 */
L8c650258: *stop=0x8c650258u;if(!valid_access(r[15]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c65025a; /* mov.l @(0x8,r15),r3 */
L8c65025a: *stop=0x8c65025au;r[8]+=r[3];goto L8c65025c; /* add r3,r8 */
L8c65025c: *stop=0x8c65025cu;if(!valid_access(r[5]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;goto L8c65025e; /* mov.l @(0x8,r5),r3 */
L8c65025e: *stop=0x8c65025eu;if(!valid_access(r[8],4u))return -3;r[8]=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;goto L8c650260; /* mov.l @r8,r8 */
L8c650260: *stop=0x8c650260u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c650262; /* mov.l r3,@(0x4,r15) */
L8c650262: *stop=0x8c650262u;if(!valid_access(r[3],2u))return -3;r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c650264; /* mov.w @r3,r3 */
L8c650264: *stop=0x8c650264u;*fpul=r[3];goto L8c650266; /* lds r3,FPUL */
L8c650266: *stop=0x8c650266u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c650268; /* float FPUL,fr3 */
L8c650268: *stop=0x8c650268u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c65026a; /* fmul fr5,fr3 */
L8c65026a: *stop=0x8c65026au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65026c; /* fmov fr3,@(r0,r15) */
L8c65026c: *stop=0x8c65026cu;if(!valid_access(r[15]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c65026e; /* mov.l @(0x4,r15),r3 */
L8c65026e: *stop=0x8c65026eu;if(!valid_access(r[3]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c650270; /* mov.w @(0x2,r3),r0 */
L8c650270: *stop=0x8c650270u;r[3]=r[0];goto L8c650272; /* mov r0,r3 */
L8c650272: *stop=0x8c650272u;*fpul=r[3];goto L8c650274; /* lds r3,FPUL */
L8c650274: *stop=0x8c650274u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c650276; /* float FPUL,fr3 */
L8c650276: *stop=0x8c650276u;r[0]=88u;goto L8c650278; /* mov #0x58,r0 */
L8c650278: *stop=0x8c650278u;r[4]+=28u;goto L8c65027a; /* add #0x1c,r4 */
L8c65027a: *stop=0x8c65027au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c65027c; /* fmul fr5,fr3 */
L8c65027c: *stop=0x8c65027cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65027e; /* fmov fr3,@(r0,r15) */
L8c65027e: *stop=0x8c65027eu;r[0]=80u;goto L8c650280; /* mov #0x50,r0 */
L8c650280: *stop=0x8c650280u;if(!valid_access(r[15]+4u,4u))return -3;r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c650282; /* mov.l @(0x4,r15),r2 */
L8c650282: *stop=0x8c650282u;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c650284; /* mov.l @r15,r3 */
L8c650284: *stop=0x8c650284u;r[2]+=r[3];goto L8c650286; /* add r3,r2 */
L8c650286: *stop=0x8c650286u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c650288; /* mov.l r2,@(0x8,r15) */
L8c650288: *stop=0x8c650288u;if(!valid_access(r[2],2u))return -3;r[3]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c65028a; /* mov.w @r2,r3 */
L8c65028a: *stop=0x8c65028au;*fpul=r[3];goto L8c65028c; /* lds r3,FPUL */
L8c65028c: *stop=0x8c65028cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c65028e; /* float FPUL,fr3 */
L8c65028e: *stop=0x8c65028eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c650290; /* fmul fr5,fr3 */
L8c650290: *stop=0x8c650290u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650292; /* fmov fr3,@(r0,r15) */
L8c650292: *stop=0x8c650292u;if(!valid_access(r[15]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c650294; /* mov.l @(0x8,r15),r3 */
L8c650294: *stop=0x8c650294u;if(!valid_access(r[3]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c650296; /* mov.w @(0x2,r3),r0 */
L8c650296: *stop=0x8c650296u;r[3]=r[0];goto L8c650298; /* mov r0,r3 */
L8c650298: *stop=0x8c650298u;*fpul=r[3];goto L8c65029a; /* lds r3,FPUL */
L8c65029a: *stop=0x8c65029au;r[0]=76u;goto L8c65029c; /* mov #0x4c,r0 */
L8c65029c: *stop=0x8c65029cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c65029e; /* float FPUL,fr3 */
L8c65029e: *stop=0x8c65029eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6502a0; /* fmul fr5,fr3 */
L8c6502a0: *stop=0x8c6502a0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6502a2; /* fmov fr3,@(r0,r15) */
L8c6502a2: *stop=0x8c6502a2u;r[0]=88u;goto L8c6502a4; /* mov #0x58,r0 */
L8c6502a4: *stop=0x8c6502a4u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[9],4);if(io_status)return io_status;goto L8c6502a6; /* mov.l r9,@-r4 */
L8c6502a6: *stop=0x8c6502a6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6502a8; /* fmov.s @(r0,r15),fr3 */
L8c6502a8: *stop=0x8c6502a8u;r[0]=84u;goto L8c6502aa; /* mov #0x54,r0 */
L8c6502aa: *stop=0x8c6502aau;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c6502ac; /* fmov.s fr3,@-r4 */
L8c6502ac: *stop=0x8c6502acu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6502ae; /* fmov.s @(r0,r15),fr3 */
L8c6502ae: *stop=0x8c6502aeu;r[0]=64u;goto L8c6502b0; /* mov #0x40,r0 */
L8c6502b0: *stop=0x8c6502b0u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c6502b2; /* fmov.s fr3,@-r4 */
L8c6502b2: *stop=0x8c6502b2u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;goto L8c6502b4; /* fmov.s fr11,@-r4 */
L8c6502b4: *stop=0x8c6502b4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6502b6; /* fmov.s @(r0,r15),fr3 */
L8c6502b6: *stop=0x8c6502b6u;r[0]=68u;goto L8c6502b8; /* mov #0x44,r0 */
L8c6502b8: *stop=0x8c6502b8u;fcopy(f,2u,13u);goto L8c6502ba; /* fmov fr13,fr2 */
L8c6502ba: *stop=0x8c6502bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6502bc; /* fsub fr3,fr2 */
L8c6502bc: *stop=0x8c6502bcu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c6502be; /* fmov.s fr2,@-r4 */
L8c6502be: *stop=0x8c6502beu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6502c0; /* fmov.s @(r0,r15),fr3 */
L8c6502c0: *stop=0x8c6502c0u;fcopy(f,2u,15u);goto L8c6502c2; /* fmov fr15,fr2 */
L8c6502c2: *stop=0x8c6502c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6502c4; /* fsub fr3,fr2 */
L8c6502c4: *stop=0x8c6502c4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c6502c6; /* fmov.s fr2,@-r4 */
L8c6502c6: *stop=0x8c6502c6u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c6502c8; /* mov.l r12,@-r4 */
L8c6502c8: *stop=0x8c6502c8u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c6502ca; /* pref @r4 */
L8c6502ca: *stop=0x8c6502cau;r[0]=76u;goto L8c6502cc; /* mov #0x4c,r0 */
L8c6502cc: *stop=0x8c6502ccu;fcopy(f,2u,12u);goto L8c6502ce; /* fmov fr12,fr2 */
L8c6502ce: *stop=0x8c6502ceu;r[4]+=60u;goto L8c6502d0; /* add #0x3c,r4 */
L8c6502d0: *stop=0x8c6502d0u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[8],4);if(io_status)return io_status;goto L8c6502d2; /* mov.l r8,@-r4 */
L8c6502d2: *stop=0x8c6502d2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6502d4; /* fmov.s @(r0,r15),fr3 */
L8c6502d4: *stop=0x8c6502d4u;r[0]=80u;goto L8c6502d6; /* mov #0x50,r0 */
L8c6502d6: *stop=0x8c6502d6u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c6502d8; /* fmov.s fr3,@-r4 */
L8c6502d8: *stop=0x8c6502d8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6502da; /* fmov.s @(r0,r15),fr3 */
L8c6502da: *stop=0x8c6502dau;r[0]=64u;goto L8c6502dc; /* mov #0x40,r0 */
L8c6502dc: *stop=0x8c6502dcu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c6502de; /* fmov.s fr3,@-r4 */
L8c6502de: *stop=0x8c6502deu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;goto L8c6502e0; /* fmov.s fr10,@-r4 */
L8c6502e0: *stop=0x8c6502e0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6502e2; /* fmov.s @(r0,r15),fr3 */
L8c6502e2: *stop=0x8c6502e2u;r[0]=68u;goto L8c6502e4; /* mov #0x44,r0 */
L8c6502e4: *stop=0x8c6502e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6502e6; /* fsub fr3,fr2 */
L8c6502e6: *stop=0x8c6502e6u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c6502e8; /* fmov.s fr2,@-r4 */
L8c6502e8: *stop=0x8c6502e8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6502ea; /* fmov.s @(r0,r15),fr3 */
L8c6502ea: *stop=0x8c6502eau;fcopy(f,2u,14u);goto L8c6502ec; /* fmov fr14,fr2 */
L8c6502ec: *stop=0x8c6502ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6502ee; /* fsub fr3,fr2 */
L8c6502ee: *stop=0x8c6502eeu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c6502f0; /* fmov.s fr2,@-r4 */
L8c6502f0: *stop=0x8c6502f0u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c6502f2; /* mov.l r12,@-r4 */
L8c6502f2: *stop=0x8c6502f2u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c6502f4; /* pref @r4 */
L8c6502f4: *stop=0x8c6502f4u;r[0]=88u;goto L8c6502f6; /* mov #0x58,r0 */
L8c6502f6: *stop=0x8c6502f6u;r[4]+=60u;goto L8c6502f8; /* add #0x3c,r4 */
L8c6502f8: *stop=0x8c6502f8u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[9],4);if(io_status)return io_status;goto L8c6502fa; /* mov.l r9,@-r4 */
L8c6502fa: *stop=0x8c6502fau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6502fc; /* fmov.s @(r0,r15),fr3 */
L8c6502fc: *stop=0x8c6502fcu;r[0]=84u;goto L8c6502fe; /* mov #0x54,r0 */
L8c6502fe: *stop=0x8c6502feu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c650300; /* fmov.s fr3,@-r4 */
L8c650300: *stop=0x8c650300u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650302; /* fmov.s @(r0,r15),fr3 */
L8c650302: *stop=0x8c650302u;r[0]=96u;goto L8c650304; /* mov #0x60,r0 */
L8c650304: *stop=0x8c650304u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c650306; /* fmov.s fr3,@-r4 */
L8c650306: *stop=0x8c650306u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;goto L8c650308; /* fmov.s fr11,@-r4 */
L8c650308: *stop=0x8c650308u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65030a; /* fmov.s @(r0,r15),fr3 */
L8c65030a: *stop=0x8c65030au;r[0]=92u;goto L8c65030c; /* mov #0x5c,r0 */
L8c65030c: *stop=0x8c65030cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[3],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c65030e; /* fadd fr3,fr13 */
L8c65030e: *stop=0x8c65030eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[4]);if(io_status)return io_status;goto L8c650310; /* fmov.s fr13,@-r4 */
L8c650310: *stop=0x8c650310u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650312; /* fmov.s @(r0,r15),fr3 */
L8c650312: *stop=0x8c650312u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],0);if(status)return status;}goto L8c650314; /* fadd fr3,fr15 */
L8c650314: *stop=0x8c650314u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[4]);if(io_status)return io_status;goto L8c650316; /* fmov.s fr15,@-r4 */
L8c650316: *stop=0x8c650316u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c650318; /* mov.l r12,@-r4 */
L8c650318: *stop=0x8c650318u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c65031a; /* pref @r4 */
L8c65031a: *stop=0x8c65031au;r[0]=76u;goto L8c65031c; /* mov #0x4c,r0 */
L8c65031c: *stop=0x8c65031cu;r[4]+=60u;goto L8c65031e; /* add #0x3c,r4 */
L8c65031e: *stop=0x8c65031eu;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[8],4);if(io_status)return io_status;goto L8c650320; /* mov.l r8,@-r4 */
L8c650320: *stop=0x8c650320u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650322; /* fmov.s @(r0,r15),fr3 */
L8c650322: *stop=0x8c650322u;r[0]=80u;goto L8c650324; /* mov #0x50,r0 */
L8c650324: *stop=0x8c650324u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c650326; /* fmov.s fr3,@-r4 */
L8c650326: *stop=0x8c650326u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650328; /* fmov.s @(r0,r15),fr3 */
L8c650328: *stop=0x8c650328u;r[0]=96u;goto L8c65032a; /* mov #0x60,r0 */
L8c65032a: *stop=0x8c65032au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c65032c; /* fmov.s fr3,@-r4 */
L8c65032c: *stop=0x8c65032cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;goto L8c65032e; /* fmov.s fr10,@-r4 */
L8c65032e: *stop=0x8c65032eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650330; /* fmov.s @(r0,r15),fr3 */
L8c650330: *stop=0x8c650330u;r[0]=92u;goto L8c650332; /* mov #0x5c,r0 */
L8c650332: *stop=0x8c650332u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[3],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c650334; /* fadd fr3,fr12 */
L8c650334: *stop=0x8c650334u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[4]);if(io_status)return io_status;goto L8c650336; /* fmov.s fr12,@-r4 */
L8c650336: *stop=0x8c650336u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650338; /* fmov.s @(r0,r15),fr3 */
L8c650338: *stop=0x8c650338u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c65033a; /* fadd fr3,fr14 */
L8c65033a: *stop=0x8c65033au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[4]);if(io_status)return io_status;goto L8c65033c; /* fmov.s fr14,@-r4 */
L8c65033c: *stop=0x8c65033cu;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[10],4);if(io_status)return io_status;goto L8c65033e; /* mov.l r10,@-r4 */
L8c65033e: *stop=0x8c65033eu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c650340; /* pref @r4 */
L8c650340: *stop=0x8c650340u;r[4]+=32u;goto L8c650342; /* add #0x20,r4 */
L8c650342: *stop=0x8c650342u;r[7]+=1u;goto L8c650344; /* add #0x1,r7 */
L8c650344: *stop=0x8c650344u;r[3]=r[7];goto L8c650346; /* mov r7,r3 */
L8c650346: *stop=0x8c650346u;r[3]<<=2;goto L8c650348; /* shll2 r3 */
L8c650348: *stop=0x8c650348u;r[9]=r[7];goto L8c65034a; /* mov r7,r9 */
L8c65034a: *stop=0x8c65034au;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c65034c; /* mov.l r3,@(0x8,r15) */
L8c65034c: *stop=0x8c65034cu;r[3]=r[7];goto L8c65034e; /* mov r7,r3 */
L8c65034e: *stop=0x8c65034eu;*sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c650350; /* shll r9 */
L8c650350: *stop=0x8c650350u;r[8]=r[7];goto L8c650352; /* mov r7,r8 */
L8c650352: *stop=0x8c650352u;r[9]+=r[3];goto L8c650354; /* add r3,r9 */
L8c650354: *stop=0x8c650354u;if(!valid_access(r[5],4u))return -3;r[2]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c650356; /* mov.l @r5,r2 */
L8c650356: *stop=0x8c650356u;r[9]<<=2;goto L8c650358; /* shll2 r9 */
L8c650358: *stop=0x8c650358u;r[8]+=4294967295u;goto L8c65035a; /* add #-0x1,r8 */
L8c65035a: *stop=0x8c65035au;r[9]+=r[2];goto L8c65035c; /* add r2,r9 */
L8c65035c: *stop=0x8c65035cu;r[8]<<=2;goto L8c65035e; /* shll2 r8 */
L8c65035e: *stop=0x8c65035eu;r[2]=r[7];goto L8c650360; /* mov r7,r2 */
L8c650360: *stop=0x8c650360u;r[1]=r[7];goto L8c650362; /* mov r7,r1 */
L8c650362: *stop=0x8c650362u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c650364; /* shll r2 */
L8c650364: *stop=0x8c650364u;if(!valid_access(r[5],4u))return -3;r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c650366; /* mov.l @r5,r3 */
L8c650366: *stop=0x8c650366u;r[2]+=r[1];goto L8c650368; /* add r1,r2 */
L8c650368: *stop=0x8c650368u;r[2]<<=2;goto L8c65036a; /* shll2 r2 */
L8c65036a: *stop=0x8c65036au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c65036c; /* mov.l r3,@r15 */
L8c65036c: *stop=0x8c65036cu;r[0]=8u;goto L8c65036e; /* mov #0x8,r0 */
L8c65036e: *stop=0x8c65036eu;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c650370; /* fmov.s @r11,fr3 */
L8c650370: *stop=0x8c650370u;r[3]+=r[2];goto L8c650372; /* add r2,r3 */
L8c650372: *stop=0x8c650372u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c650374; /* fmov.s @(r0,r3),fr2 */
L8c650374: *stop=0x8c650374u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c650376; /* fcmp/eq fr3,fr2 */
L8c650376: if(!(*sr&1u))goto L8c650396;goto L8c650378; /* bf 0x8c650396 */
L8c650378: *stop=0x8c650378u;r[2]=r[7];goto L8c65037a; /* mov r7,r2 */
L8c65037a: *stop=0x8c65037au;r[2]+=4294967295u;goto L8c65037c; /* add #-0x1,r2 */
L8c65037c: *stop=0x8c65037cu;r[3]=r[2];goto L8c65037e; /* mov r2,r3 */
L8c65037e: *stop=0x8c65037eu;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c650380; /* shll r2 */
L8c650380: *stop=0x8c650380u;if(!valid_access(r[15],4u))return -3;r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c650382; /* mov.l @r15,r1 */
L8c650382: *stop=0x8c650382u;r[2]+=r[3];goto L8c650384; /* add r3,r2 */
L8c650384: *stop=0x8c650384u;r[2]<<=2;goto L8c650386; /* shll2 r2 */
L8c650386: *stop=0x8c650386u;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c650388; /* fmov.s @r11,fr3 */
L8c650388: *stop=0x8c650388u;r[0]=8u;goto L8c65038a; /* mov #0x8,r0 */
L8c65038a: *stop=0x8c65038au;r[2]+=r[1];goto L8c65038c; /* add r1,r2 */
L8c65038c: *stop=0x8c65038cu;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c65038e; /* fmov.s @(r0,r2),fr2 */
L8c65038e: *stop=0x8c65038eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c650390; /* fcmp/eq fr3,fr2 */
L8c650390: if(!(*sr&1u))goto L8c650396;goto L8c650392; /* bf 0x8c650396 */
L8c650392: *stop=0x8c650394u;goto L8c65055c; /* bra 0x8c65055c */
L8c650396: *stop=0x8c650396u;r[3]=r[7];goto L8c650398; /* mov r7,r3 */
L8c650398: *stop=0x8c650398u;r[3]+=4294967295u;goto L8c65039a; /* add #-0x1,r3 */
L8c65039a: *stop=0x8c65039au;r[2]=r[3];goto L8c65039c; /* mov r3,r2 */
L8c65039c: *stop=0x8c65039cu;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c65039e; /* shll r3 */
L8c65039e: *stop=0x8c65039eu;if(!valid_access(r[5],4u))return -3;r[1]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c6503a0; /* mov.l @r5,r1 */
L8c6503a0: *stop=0x8c6503a0u;r[3]+=r[2];goto L8c6503a2; /* add r2,r3 */
L8c6503a2: *stop=0x8c6503a2u;r[3]<<=2;goto L8c6503a4; /* shll2 r3 */
L8c6503a4: *stop=0x8c6503a4u;fcopy(f,10u,4u);goto L8c6503a6; /* fmov fr4,fr10 */
L8c6503a6: *stop=0x8c6503a6u;r[0]=8u;goto L8c6503a8; /* mov #0x8,r0 */
L8c6503a8: *stop=0x8c6503a8u;fcopy(f,11u,4u);goto L8c6503aa; /* fmov fr4,fr11 */
L8c6503aa: *stop=0x8c6503aau;r[3]+=r[1];goto L8c6503ac; /* add r1,r3 */
L8c6503ac: *stop=0x8c6503acu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6503ae; /* mov.l r3,@r15 */
L8c6503ae: *stop=0x8c6503aeu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6503b0; /* fmov.s @(r0,r3),fr3 */
L8c6503b0: *stop=0x8c6503b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[10],&fr[3],&f->fpscr,&fr[10]);if(status)return status;}goto L8c6503b2; /* fdiv fr3,fr10 */
L8c6503b2: *stop=0x8c6503b2u;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6503b4; /* fmov.s @(r0,r9),fr3 */
L8c6503b4: *stop=0x8c6503b4u;r[0]=12u;goto L8c6503b6; /* mov #0xc,r0 */
L8c6503b6: *stop=0x8c6503b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[11],&fr[3],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6503b8; /* fdiv fr3,fr11 */
L8c6503b8: *stop=0x8c6503b8u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6503ba; /* fmov.s @r3,fr3 */
L8c6503ba: *stop=0x8c6503bau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]+r[14]);if(io_status)return io_status;goto L8c6503bc; /* fmov.s @(r0,r14),fr14 */
L8c6503bc: *stop=0x8c6503bcu;r[0]=4u;goto L8c6503be; /* mov #0x4,r0 */
L8c6503be: *stop=0x8c6503beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6503c0; /* fmul fr6,fr3 */
L8c6503c0: *stop=0x8c6503c0u;fcopy(f,15u,14u);goto L8c6503c2; /* fmov fr14,fr15 */
L8c6503c2: *stop=0x8c6503c2u;fcopy(f,0u,10u);goto L8c6503c4; /* fmov fr10,fr0 */
L8c6503c4: *stop=0x8c6503c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[15],&f->fpscr,&fr[15]);if(status)return status;}goto L8c6503c6; /* fmac fr0,fr3,fr15 */
L8c6503c6: *stop=0x8c6503c6u;if(!valid_access(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[9]);if(io_status)return io_status;goto L8c6503c8; /* fmov.s @r9,fr3 */
L8c6503c8: *stop=0x8c6503c8u;fcopy(f,0u,11u);goto L8c6503ca; /* fmov fr11,fr0 */
L8c6503ca: *stop=0x8c6503cau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6503cc; /* fmul fr6,fr3 */
L8c6503cc: *stop=0x8c6503ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[14],&f->fpscr,&fr[14]);if(status)return status;}goto L8c6503ce; /* fmac fr0,fr3,fr14 */
L8c6503ce: *stop=0x8c6503ceu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6503d0; /* fmov.s @(r0,r3),fr3 */
L8c6503d0: *stop=0x8c6503d0u;r[0]=16u;goto L8c6503d2; /* mov #0x10,r0 */
L8c6503d2: *stop=0x8c6503d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6503d4; /* fmul fr9,fr3 */
L8c6503d4: *stop=0x8c6503d4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]+r[14]);if(io_status)return io_status;goto L8c6503d6; /* fmov.s @(r0,r14),fr12 */
L8c6503d6: *stop=0x8c6503d6u;fcopy(f,0u,10u);goto L8c6503d8; /* fmov fr10,fr0 */
L8c6503d8: *stop=0x8c6503d8u;r[0]=4u;goto L8c6503da; /* mov #0x4,r0 */
L8c6503da: *stop=0x8c6503dau;fcopy(f,13u,12u);goto L8c6503dc; /* fmov fr12,fr13 */
L8c6503dc: *stop=0x8c6503dcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[13],&f->fpscr,&fr[13]);if(status)return status;}goto L8c6503de; /* fmac fr0,fr3,fr13 */
L8c6503de: *stop=0x8c6503deu;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6503e0; /* fmov.s @(r0,r9),fr3 */
L8c6503e0: *stop=0x8c6503e0u;r[0]=16u;goto L8c6503e2; /* mov #0x10,r0 */
L8c6503e2: *stop=0x8c6503e2u;fcopy(f,0u,11u);goto L8c6503e4; /* fmov fr11,fr0 */
L8c6503e4: *stop=0x8c6503e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6503e6; /* fmul fr9,fr3 */
L8c6503e6: *stop=0x8c6503e6u;fcopy(f,2u,13u);goto L8c6503e8; /* fmov fr13,fr2 */
L8c6503e8: *stop=0x8c6503e8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[12],&f->fpscr,&fr[12]);if(status)return status;}goto L8c6503ea; /* fmac fr0,fr3,fr12 */
L8c6503ea: *stop=0x8c6503eau;fcopy(f,3u,15u);goto L8c6503ec; /* fmov fr15,fr3 */
L8c6503ec: *stop=0x8c6503ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[14],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6503ee; /* fsub fr14,fr3 */
L8c6503ee: *stop=0x8c6503eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[12],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6503f0; /* fsub fr12,fr2 */
L8c6503f0: *stop=0x8c6503f0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6503f2; /* fmov fr3,@(r0,r15) */
L8c6503f2: *stop=0x8c6503f2u;r[0]=12u;goto L8c6503f4; /* mov #0xc,r0 */
L8c6503f4: *stop=0x8c6503f4u;fcopy(f,0u,3u);goto L8c6503f6; /* fmov fr3,fr0 */
L8c6503f6: *stop=0x8c6503f6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c6503f8; /* fmov fr2,@(r0,r15) */
L8c6503f8: *stop=0x8c6503f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6503fa; /* fmul fr2,fr2 */
L8c6503fa: *stop=0x8c6503fau;r[0]=28u;goto L8c6503fc; /* mov #0x1c,r0 */
L8c6503fc: *stop=0x8c6503fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6503fe; /* fmac fr0,fr3,fr2 */
L8c6503fe: *stop=0x8c6503feu;fcopy(f,3u,4u);goto L8c650400; /* fmov fr4,fr3 */
L8c650400: *stop=0x8c650400u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c650402; /* fsqrt fr2 */
L8c650402: *stop=0x8c650402u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c650404; /* fdiv fr2,fr3 */
L8c650404: *stop=0x8c650404u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650406; /* fmov fr3,@(r0,r15) */
L8c650406: *stop=0x8c650406u;r[0]=12u;goto L8c650408; /* mov #0xc,r0 */
L8c650408: *stop=0x8c650408u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c65040a; /* fmov.s @(r0,r15),fr2 */
L8c65040a: *stop=0x8c65040au;r[0]=24u;goto L8c65040c; /* mov #0x18,r0 */
L8c65040c: *stop=0x8c65040cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c65040e; /* fmul fr3,fr2 */
L8c65040e: *stop=0x8c65040eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c650410; /* fmul fr7,fr2 */
L8c650410: *stop=0x8c650410u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c650412; /* fmov fr2,@(r0,r15) */
L8c650412: *stop=0x8c650412u;r[0]=16u;goto L8c650414; /* mov #0x10,r0 */
L8c650414: *stop=0x8c650414u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c650416; /* fmov.s @(r0,r15),fr1 */
L8c650416: *stop=0x8c650416u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c650418; /* fmul fr3,fr1 */
L8c650418: *stop=0x8c650418u;r[0]=20u;goto L8c65041a; /* mov #0x14,r0 */
L8c65041a: *stop=0x8c65041au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c65041c; /* fmul fr8,fr2 */
L8c65041c: *stop=0x8c65041cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c65041e; /* fmul fr7,fr1 */
L8c65041e: *stop=0x8c65041eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto L8c650420; /* fneg fr1 */
L8c650420: *stop=0x8c650420u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[15]);if(io_status)return io_status;goto L8c650422; /* fmov fr1,@(r0,r15) */
L8c650422: *stop=0x8c650422u;r[0]=48u;goto L8c650424; /* mov #0x30,r0 */
L8c650424: *stop=0x8c650424u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c650426; /* fmov fr2,@(r0,r15) */
L8c650426: *stop=0x8c650426u;r[0]=20u;goto L8c650428; /* mov #0x14,r0 */
L8c650428: *stop=0x8c650428u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65042a; /* fmov.s @(r0,r15),fr3 */
L8c65042a: *stop=0x8c65042au;r[0]=44u;goto L8c65042c; /* mov #0x2c,r0 */
L8c65042c: *stop=0x8c65042cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c65042e; /* fmul fr8,fr3 */
L8c65042e: *stop=0x8c65042eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650430; /* fmov fr3,@(r0,r15) */
L8c650430: *stop=0x8c650430u;r[0]=52u;goto L8c650432; /* mov #0x34,r0 */
L8c650432: *stop=0x8c650432u;if(!valid_access(r[5]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;goto L8c650434; /* mov.l @(0x8,r5),r3 */
L8c650434: *stop=0x8c650434u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c650436; /* mov.l r3,@r15 */
L8c650436: *stop=0x8c650436u;r[3]+=r[8];goto L8c650438; /* add r8,r3 */
L8c650438: *stop=0x8c650438u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c65043a; /* mov.l r3,@(0x4,r15) */
L8c65043a: *stop=0x8c65043au;if(!valid_access(r[3],2u))return -3;r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c65043c; /* mov.w @r3,r3 */
L8c65043c: *stop=0x8c65043cu;*fpul=r[3];goto L8c65043e; /* lds r3,FPUL */
L8c65043e: *stop=0x8c65043eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c650440; /* float FPUL,fr3 */
L8c650440: *stop=0x8c650440u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c650442; /* fmul fr5,fr3 */
L8c650442: *stop=0x8c650442u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650444; /* fmov fr3,@(r0,r15) */
L8c650444: *stop=0x8c650444u;if(!valid_access(r[15]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c650446; /* mov.l @(0x4,r15),r3 */
L8c650446: *stop=0x8c650446u;if(!valid_access(r[3]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c650448; /* mov.w @(0x2,r3),r0 */
L8c650448: *stop=0x8c650448u;r[3]=r[0];goto L8c65044a; /* mov r0,r3 */
L8c65044a: *stop=0x8c65044au;*fpul=r[3];goto L8c65044c; /* lds r3,FPUL */
L8c65044c: *stop=0x8c65044cu;r[0]=40u;goto L8c65044e; /* mov #0x28,r0 */
L8c65044e: *stop=0x8c65044eu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c650450; /* float FPUL,fr3 */
L8c650450: *stop=0x8c650450u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c650452; /* fmul fr5,fr3 */
L8c650452: *stop=0x8c650452u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650454; /* fmov fr3,@(r0,r15) */
L8c650454: *stop=0x8c650454u;if(!valid_access(r[15]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c650456; /* mov.l @(0x8,r15),r3 */
L8c650456: *stop=0x8c650456u;if(!valid_access(r[15],4u))return -3;r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c650458; /* mov.l @r15,r2 */
L8c650458: *stop=0x8c650458u;r[0]=36u;goto L8c65045a; /* mov #0x24,r0 */
L8c65045a: *stop=0x8c65045au;r[4]+=28u;goto L8c65045c; /* add #0x1c,r4 */
L8c65045c: *stop=0x8c65045cu;r[2]+=r[3];goto L8c65045e; /* add r3,r2 */
L8c65045e: *stop=0x8c65045eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c650460; /* mov.l r2,@(0x4,r15) */
L8c650460: *stop=0x8c650460u;if(!valid_access(r[2],2u))return -3;r[3]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c650462; /* mov.w @r2,r3 */
L8c650462: *stop=0x8c650462u;*fpul=r[3];goto L8c650464; /* lds r3,FPUL */
L8c650464: *stop=0x8c650464u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c650466; /* float FPUL,fr3 */
L8c650466: *stop=0x8c650466u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c650468; /* fmul fr5,fr3 */
L8c650468: *stop=0x8c650468u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65046a; /* fmov fr3,@(r0,r15) */
L8c65046a: *stop=0x8c65046au;if(!valid_access(r[15]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c65046c; /* mov.l @(0x4,r15),r3 */
L8c65046c: *stop=0x8c65046cu;if(!valid_access(r[3]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c65046e; /* mov.w @(0x2,r3),r0 */
L8c65046e: *stop=0x8c65046eu;r[3]=r[0];goto L8c650470; /* mov r0,r3 */
L8c650470: *stop=0x8c650470u;*fpul=r[3];goto L8c650472; /* lds r3,FPUL */
L8c650472: *stop=0x8c650472u;r[0]=32u;goto L8c650474; /* mov #0x20,r0 */
L8c650474: *stop=0x8c650474u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c650476; /* float FPUL,fr3 */
L8c650476: *stop=0x8c650476u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c650478; /* fmul fr5,fr3 */
L8c650478: *stop=0x8c650478u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65047a; /* fmov fr3,@(r0,r15) */
L8c65047a: *stop=0x8c65047au;r[0]=40u;goto L8c65047c; /* mov #0x28,r0 */
L8c65047c: *stop=0x8c65047cu;if(!valid_access(r[5]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c65047e; /* mov.l @(0x4,r5),r3 */
L8c65047e: *stop=0x8c65047eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c650480; /* mov.l r3,@(0x4,r15) */
L8c650480: *stop=0x8c650480u;r[3]+=r[8];goto L8c650482; /* add r8,r3 */
L8c650482: *stop=0x8c650482u;if(!valid_access(r[3],4u))return -3;r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c650484; /* mov.l @r3,r2 */
L8c650484: *stop=0x8c650484u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c650486; /* mov.l r2,@r15 */
L8c650486: *stop=0x8c650486u;if(!valid_access(r[15]+4u,4u))return -3;r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c650488; /* mov.l @(0x4,r15),r1 */
L8c650488: *stop=0x8c650488u;if(!valid_access(r[15]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c65048a; /* mov.l @(0x8,r15),r3 */
L8c65048a: *stop=0x8c65048au;r[1]+=r[3];goto L8c65048c; /* add r3,r1 */
L8c65048c: *stop=0x8c65048cu;if(!valid_access(r[1],4u))return -3;r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c65048e; /* mov.l @r1,r2 */
L8c65048e: *stop=0x8c65048eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c650490; /* mov.l r2,@(0x4,r15) */
L8c650490: *stop=0x8c650490u;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c650492; /* mov.l @r15,r3 */
L8c650492: *stop=0x8c650492u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c650494; /* mov.l r3,@-r4 */
L8c650494: *stop=0x8c650494u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650496; /* fmov.s @(r0,r15),fr3 */
L8c650496: *stop=0x8c650496u;r[0]=52u;goto L8c650498; /* mov #0x34,r0 */
L8c650498: *stop=0x8c650498u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c65049a; /* fmov.s fr3,@-r4 */
L8c65049a: *stop=0x8c65049au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65049c; /* fmov.s @(r0,r15),fr3 */
L8c65049c: *stop=0x8c65049cu;r[0]=20u;goto L8c65049e; /* mov #0x14,r0 */
L8c65049e: *stop=0x8c65049eu;fcopy(f,2u,13u);goto L8c6504a0; /* fmov fr13,fr2 */
L8c6504a0: *stop=0x8c6504a0u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c6504a2; /* fmov.s fr3,@-r4 */
L8c6504a2: *stop=0x8c6504a2u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;goto L8c6504a4; /* fmov.s fr10,@-r4 */
L8c6504a4: *stop=0x8c6504a4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6504a6; /* fmov.s @(r0,r15),fr3 */
L8c6504a6: *stop=0x8c6504a6u;r[0]=24u;goto L8c6504a8; /* mov #0x18,r0 */
L8c6504a8: *stop=0x8c6504a8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6504aa; /* fsub fr3,fr2 */
L8c6504aa: *stop=0x8c6504aau;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c6504ac; /* fmov.s fr2,@-r4 */
L8c6504ac: *stop=0x8c6504acu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6504ae; /* fmov.s @(r0,r15),fr3 */
L8c6504ae: *stop=0x8c6504aeu;fcopy(f,2u,15u);goto L8c6504b0; /* fmov fr15,fr2 */
L8c6504b0: *stop=0x8c6504b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6504b2; /* fsub fr3,fr2 */
L8c6504b2: *stop=0x8c6504b2u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c6504b4; /* fmov.s fr2,@-r4 */
L8c6504b4: *stop=0x8c6504b4u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c6504b6; /* mov.l r12,@-r4 */
L8c6504b6: *stop=0x8c6504b6u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c6504b8; /* pref @r4 */
L8c6504b8: *stop=0x8c6504b8u;r[0]=32u;goto L8c6504ba; /* mov #0x20,r0 */
L8c6504ba: *stop=0x8c6504bau;if(!valid_access(r[15]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6504bc; /* mov.l @(0x4,r15),r3 */
L8c6504bc: *stop=0x8c6504bcu;r[4]+=60u;goto L8c6504be; /* add #0x3c,r4 */
L8c6504be: *stop=0x8c6504beu;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6504c0; /* mov.l r3,@-r4 */
L8c6504c0: *stop=0x8c6504c0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6504c2; /* fmov.s @(r0,r15),fr3 */
L8c6504c2: *stop=0x8c6504c2u;r[0]=36u;goto L8c6504c4; /* mov #0x24,r0 */
L8c6504c4: *stop=0x8c6504c4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c6504c6; /* fmov.s fr3,@-r4 */
L8c6504c6: *stop=0x8c6504c6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6504c8; /* fmov.s @(r0,r15),fr3 */
L8c6504c8: *stop=0x8c6504c8u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c6504ca; /* fmov.s fr3,@-r4 */
L8c6504ca: *stop=0x8c6504ccu;goto L8c6504f0; /* bra 0x8c6504f0 */
L8c6504f0: *stop=0x8c6504f0u;r[0]=20u;goto L8c6504f2; /* mov #0x14,r0 */
L8c6504f2: *stop=0x8c6504f2u;fcopy(f,2u,12u);goto L8c6504f4; /* fmov fr12,fr2 */
L8c6504f4: *stop=0x8c6504f4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;goto L8c6504f6; /* fmov.s fr11,@-r4 */
L8c6504f6: *stop=0x8c6504f6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6504f8; /* fmov.s @(r0,r15),fr3 */
L8c6504f8: *stop=0x8c6504f8u;r[0]=24u;goto L8c6504fa; /* mov #0x18,r0 */
L8c6504fa: *stop=0x8c6504fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6504fc; /* fsub fr3,fr2 */
L8c6504fc: *stop=0x8c6504fcu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c6504fe; /* fmov.s fr2,@-r4 */
L8c6504fe: *stop=0x8c6504feu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650500; /* fmov.s @(r0,r15),fr3 */
L8c650500: *stop=0x8c650500u;fcopy(f,2u,14u);goto L8c650502; /* fmov fr14,fr2 */
L8c650502: *stop=0x8c650502u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c650504; /* fsub fr3,fr2 */
L8c650504: *stop=0x8c650504u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c650506; /* fmov.s fr2,@-r4 */
L8c650506: *stop=0x8c650506u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c650508; /* mov.l r12,@-r4 */
L8c650508: *stop=0x8c650508u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c65050a; /* pref @r4 */
L8c65050a: *stop=0x8c65050au;r[0]=40u;goto L8c65050c; /* mov #0x28,r0 */
L8c65050c: *stop=0x8c65050cu;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c65050e; /* mov.l @r15,r3 */
L8c65050e: *stop=0x8c65050eu;r[4]+=60u;goto L8c650510; /* add #0x3c,r4 */
L8c650510: *stop=0x8c650510u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c650512; /* mov.l r3,@-r4 */
L8c650512: *stop=0x8c650512u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650514; /* fmov.s @(r0,r15),fr3 */
L8c650514: *stop=0x8c650514u;r[0]=52u;goto L8c650516; /* mov #0x34,r0 */
L8c650516: *stop=0x8c650516u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c650518; /* fmov.s fr3,@-r4 */
L8c650518: *stop=0x8c650518u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65051a; /* fmov.s @(r0,r15),fr3 */
L8c65051a: *stop=0x8c65051au;r[0]=44u;goto L8c65051c; /* mov #0x2c,r0 */
L8c65051c: *stop=0x8c65051cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c65051e; /* fmov.s fr3,@-r4 */
L8c65051e: *stop=0x8c65051eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;goto L8c650520; /* fmov.s fr10,@-r4 */
L8c650520: *stop=0x8c650520u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650522; /* fmov.s @(r0,r15),fr3 */
L8c650522: *stop=0x8c650522u;r[0]=48u;goto L8c650524; /* mov #0x30,r0 */
L8c650524: *stop=0x8c650524u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[3],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c650526; /* fadd fr3,fr13 */
L8c650526: *stop=0x8c650526u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[4]);if(io_status)return io_status;goto L8c650528; /* fmov.s fr13,@-r4 */
L8c650528: *stop=0x8c650528u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65052a; /* fmov.s @(r0,r15),fr3 */
L8c65052a: *stop=0x8c65052au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],0);if(status)return status;}goto L8c65052c; /* fadd fr3,fr15 */
L8c65052c: *stop=0x8c65052cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[4]);if(io_status)return io_status;goto L8c65052e; /* fmov.s fr15,@-r4 */
L8c65052e: *stop=0x8c65052eu;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c650530; /* mov.l r12,@-r4 */
L8c650530: *stop=0x8c650530u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c650532; /* pref @r4 */
L8c650532: *stop=0x8c650532u;r[0]=32u;goto L8c650534; /* mov #0x20,r0 */
L8c650534: *stop=0x8c650534u;if(!valid_access(r[15]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c650536; /* mov.l @(0x4,r15),r3 */
L8c650536: *stop=0x8c650536u;r[4]+=60u;goto L8c650538; /* add #0x3c,r4 */
L8c650538: *stop=0x8c650538u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c65053a; /* mov.l r3,@-r4 */
L8c65053a: *stop=0x8c65053au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65053c; /* fmov.s @(r0,r15),fr3 */
L8c65053c: *stop=0x8c65053cu;r[0]=36u;goto L8c65053e; /* mov #0x24,r0 */
L8c65053e: *stop=0x8c65053eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c650540; /* fmov.s fr3,@-r4 */
L8c650540: *stop=0x8c650540u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650542; /* fmov.s @(r0,r15),fr3 */
L8c650542: *stop=0x8c650542u;r[0]=44u;goto L8c650544; /* mov #0x2c,r0 */
L8c650544: *stop=0x8c650544u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c650546; /* fmov.s fr3,@-r4 */
L8c650546: *stop=0x8c650546u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;goto L8c650548; /* fmov.s fr11,@-r4 */
L8c650548: *stop=0x8c650548u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65054a; /* fmov.s @(r0,r15),fr3 */
L8c65054a: *stop=0x8c65054au;r[0]=48u;goto L8c65054c; /* mov #0x30,r0 */
L8c65054c: *stop=0x8c65054cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[3],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c65054e; /* fadd fr3,fr12 */
L8c65054e: *stop=0x8c65054eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[4]);if(io_status)return io_status;goto L8c650550; /* fmov.s fr12,@-r4 */
L8c650550: *stop=0x8c650550u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650552; /* fmov.s @(r0,r15),fr3 */
L8c650552: *stop=0x8c650552u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c650554; /* fadd fr3,fr14 */
L8c650554: *stop=0x8c650554u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[4]);if(io_status)return io_status;goto L8c650556; /* fmov.s fr14,@-r4 */
L8c650556: *stop=0x8c650556u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[10],4);if(io_status)return io_status;goto L8c650558; /* mov.l r10,@-r4 */
L8c650558: *stop=0x8c650558u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c65055a; /* pref @r4 */
L8c65055a: *stop=0x8c65055au;r[4]+=32u;goto L8c65055c; /* add #0x20,r4 */
L8c65055c: *stop=0x8c65055cu;if(!valid_access(r[15]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c65055e; /* mov.l @(0x8,r15),r3 */
L8c65055e: *stop=0x8c65055eu;r[7]+=1u;goto L8c650560; /* add #0x1,r7 */
L8c650560: *stop=0x8c650560u;*sr=(*sr&~1u)|((int32_t)r[7]>(int32_t)r[13]);goto L8c650562; /* cmp/gt r13,r7 */
L8c650562: *stop=0x8c650562u;r[8]+=4u;goto L8c650564; /* add #0x4,r8 */
L8c650564: *stop=0x8c650564u;r[3]+=4u;goto L8c650566; /* add #0x4,r3 */
L8c650566: *stop=0x8c650566u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c650568; /* mov.l r3,@(0x8,r15) */
L8c650568: take=(*sr&1u);*stop=0x8c65056au;r[9]+=12u;if(take)goto L8c650570;goto L8c65056c; /* bt/s 0x8c650570 */
L8c65056c: *stop=0x8c65056eu;goto L8c65035e; /* bra 0x8c65035e */
L8c650570: *stop=0x8c650570u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[4],4);if(io_status)return io_status;goto L8c650572; /* mov.l r4,@r6 */
L8c650572: *stop=0x8c650572u;r[15]+=100u;goto L8c650574; /* add #0x64,r15 */
L8c650574: *stop=0x8c650574u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c650576; /* fmov.s @r15+,fr12 */
L8c650576: *stop=0x8c650576u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c650578; /* fmov.s @r15+,fr13 */
L8c650578: *stop=0x8c650578u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65057a; /* fmov.s @r15+,fr14 */
L8c65057a: *stop=0x8c65057au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c65057c; /* fmov.s @r15+,fr15 */
L8c65057c: *stop=0x8c65057cu;if(!valid_access(r[15],4u))return -3;r[8]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c65057e; /* mov.l @r15+,r8 */
L8c65057e: *stop=0x8c65057eu;if(!valid_access(r[15],4u))return -3;r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c650580; /* mov.l @r15+,r9 */
L8c650580: *stop=0x8c650580u;if(!valid_access(r[15],4u))return -3;r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c650582; /* mov.l @r15+,r10 */
L8c650582: *stop=0x8c650582u;if(!valid_access(r[15],4u))return -3;r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c650584; /* mov.l @r15+,r11 */
L8c650584: *stop=0x8c650584u;if(!valid_access(r[15],4u))return -3;r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c650586; /* mov.l @r15+,r12 */
L8c650586: *stop=0x8c650586u;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c650588; /* mov.l @r15+,r13 */
L8c650588: *stop=0x8c650588u;if(*pr!=entry_pr)return -3;*stop=0x8c65058au;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_polygon_emit_projected_uv_8c64fe0c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64fe0c;
L8c64fe0c: *stop=0x8c64fe0cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64fe0e; /* mov.l r14,@-r15 */
L8c64fe0e: *stop=0x8c64fe0eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64fe10; /* mov.l r13,@-r15 */
L8c64fe10: *stop=0x8c64fe10u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64fe12; /* mov.l r12,@-r15 */
L8c64fe12: *stop=0x8c64fe12u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64fe14; /* mov.l r11,@-r15 */
L8c64fe14: *stop=0x8c64fe14u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64fe16; /* mov.l r10,@-r15 */
L8c64fe16: *stop=0x8c64fe16u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64fe18; /* mov.l r9,@-r15 */
L8c64fe18: *stop=0x8c64fe18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64fe1a; /* mov.l r8,@-r15 */
L8c64fe1a: *stop=0x8c64fe1au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c64fe1c; /* fmov.s fr15,@-r15 */
L8c64fe1c: *stop=0x8c64fe1cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c64fe1e; /* fmov.s fr14,@-r15 */
L8c64fe1e: *stop=0x8c64fe1eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c64fe20; /* fmov.s fr13,@-r15 */
L8c64fe20: *stop=0x8c64fe20u;r[15]+=4294967244u;goto L8c64fe22; /* add #-0x34,r15 */
L8c64fe22: *stop=0x8c64fe22u;r[14]=rd(m,bus,&io_status,0x8c650128u,4);if(io_status)return io_status;goto L8c64fe24; /* mov.l 0x8c650128,r14 */
L8c64fe24: *stop=0x8c64fe24u;r[0]=0x8c65013cu;goto L8c64fe26; /* mova 0x8c65013c,r0 */
L8c64fe26: *stop=0x8c64fe26u;r[3]=rd(m,bus,&io_status,0x8c65012cu,4);if(io_status)return io_status;goto L8c64fe28; /* mov.l 0x8c65012c,r3 */
L8c64fe28: *stop=0x8c64fe28u;r[13]=r[4];goto L8c64fe2a; /* mov r4,r13 */
L8c64fe2a: *stop=0x8c64fe2au;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c64fe2c; /* fmov.s @r14,fr4 */
L8c64fe2c: *stop=0x8c64fe2cu;r[7]=0u;goto L8c64fe2e; /* mov #0x0,r7 */
L8c64fe2e: *stop=0x8c64fe2eu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c64fe30; /* fmov.s @r3,fr3 */
L8c64fe30: *stop=0x8c64fe30u;r[13]+=4294967295u;goto L8c64fe32; /* add #-0x1,r13 */
L8c64fe32: *stop=0x8c64fe32u;if(!valid_access(r[5],4u))return -3;r[1]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fe34; /* mov.l @r5,r1 */
L8c64fe34: *stop=0x8c64fe34u;r[2]=rd(m,bus,&io_status,0x8c650130u,4);if(io_status)return io_status;goto L8c64fe36; /* mov.l 0x8c650130,r2 */
L8c64fe36: *stop=0x8c64fe36u;r[11]=rd(m,bus,&io_status,0x8c650138u,4);if(io_status)return io_status;goto L8c64fe38; /* mov.l 0x8c650138,r11 */
L8c64fe38: *stop=0x8c64fe38u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c64fe3a; /* fmov.s @r0,fr6 */
L8c64fe3a: *stop=0x8c64fe3au;r[0]=8u;goto L8c64fe3c; /* mov #0x8,r0 */
L8c64fe3c: *stop=0x8c64fe3cu;fcopy(f,5u,4u);goto L8c64fe3e; /* fmov fr4,fr5 */
L8c64fe3e: *stop=0x8c64fe3eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c64fe40; /* fmul fr3,fr5 */
L8c64fe40: *stop=0x8c64fe40u;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[11]);if(io_status)return io_status;goto L8c64fe42; /* fmov.s @r11,fr9 */
L8c64fe42: *stop=0x8c64fe42u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c64fe44; /* fmov.s @r2,fr2 */
L8c64fe44: *stop=0x8c64fe44u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][7]=0x3f800000u;goto L8c64fe46; /* fldi1 fr7 */
L8c64fe46: *stop=0x8c64fe46u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[7],&f->fpscr,&fr[7],0);if(status)return status;}goto L8c64fe48; /* fadd fr7,fr7 */
L8c64fe48: *stop=0x8c64fe48u;fcopy(f,8u,4u);goto L8c64fe4a; /* fmov fr4,fr8 */
L8c64fe4a: *stop=0x8c64fe4au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[2],&f->fpscr,&fr[8]);if(status)return status;}goto L8c64fe4c; /* fmul fr2,fr8 */
L8c64fe4c: *stop=0x8c64fe4cu;if(!valid_access(r[6],4u))return -3;r[4]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c64fe4e; /* mov.l @r6,r4 */
L8c64fe4e: *stop=0x8c64fe4eu;r[10]=rd(m,bus,&io_status,0x8c650134u,4);if(io_status)return io_status;goto L8c64fe50; /* mov.l 0x8c650134,r10 */
L8c64fe50: *stop=0x8c64fe50u;r[12]=rd(m,bus,&io_status,0x8c650140u,4);if(io_status)return io_status;goto L8c64fe52; /* mov.l 0x8c650140,r12 */
L8c64fe52: *stop=0x8c64fe52u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c64fe54; /* mov.l r1,@(0x4,r15) */
L8c64fe54: *stop=0x8c64fe54u;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c64fe56; /* fmov.s @(r0,r1),fr3 */
L8c64fe56: *stop=0x8c64fe56u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][9],&f->fpscr,sr,0);if(status)return status;goto L8c64fe58; /* fcmp/eq fr9,fr3 */
L8c64fe58: take=!(*sr&1u);*stop=0x8c64fe5au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;if(take)goto L8c64fe76;goto L8c64fe5c; /* bf/s 0x8c64fe76 */
L8c64fe5c: *stop=0x8c64fe5cu;r[1]=r[13];goto L8c64fe5e; /* mov r13,r1 */
L8c64fe5e: *stop=0x8c64fe5eu;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c64fe60; /* shll r1 */
L8c64fe60: *stop=0x8c64fe60u;r[0]=r[13];goto L8c64fe62; /* mov r13,r0 */
L8c64fe62: *stop=0x8c64fe62u;r[1]+=r[0];goto L8c64fe64; /* add r0,r1 */
L8c64fe64: *stop=0x8c64fe64u;if(!valid_access(r[15]+4u,4u))return -3;r[0]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c64fe66; /* mov.l @(0x4,r15),r0 */
L8c64fe66: *stop=0x8c64fe66u;r[1]<<=2;goto L8c64fe68; /* shll2 r1 */
L8c64fe68: *stop=0x8c64fe68u;r[1]+=r[0];goto L8c64fe6a; /* add r0,r1 */
L8c64fe6a: *stop=0x8c64fe6au;r[0]=8u;goto L8c64fe6c; /* mov #0x8,r0 */
L8c64fe6c: *stop=0x8c64fe6cu;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c64fe6e; /* fmov.s @(r0,r1),fr3 */
L8c64fe6e: *stop=0x8c64fe6eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][9],&f->fpscr,sr,0);if(status)return status;goto L8c64fe70; /* fcmp/eq fr9,fr3 */
L8c64fe70: if(!(*sr&1u))goto L8c64fe76;goto L8c64fe72; /* bf 0x8c64fe76 */
L8c64fe72: *stop=0x8c64fe74u;goto L8c64ff86; /* bra 0x8c64ff86 */
L8c64fe76: *stop=0x8c64fe76u;if(!valid_access(r[5],4u))return -3;r[8]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64fe78; /* mov.l @r5,r8 */
L8c64fe78: *stop=0x8c64fe78u;r[0]=8u;goto L8c64fe7a; /* mov #0x8,r0 */
L8c64fe7a: *stop=0x8c64fe7au;fcopy(f,10u,4u);goto L8c64fe7c; /* fmov fr4,fr10 */
L8c64fe7c: *stop=0x8c64fe7cu;r[9]=r[13];goto L8c64fe7e; /* mov r13,r9 */
L8c64fe7e: *stop=0x8c64fe7eu;if(!valid_access(r[0]+r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[8]);if(io_status)return io_status;goto L8c64fe80; /* fmov.s @(r0,r8),fr3 */
L8c64fe80: *stop=0x8c64fe80u;*sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c64fe82; /* shll r9 */
L8c64fe82: *stop=0x8c64fe82u;r[3]=r[13];goto L8c64fe84; /* mov r13,r3 */
L8c64fe84: *stop=0x8c64fe84u;r[9]+=r[3];goto L8c64fe86; /* add r3,r9 */
L8c64fe86: *stop=0x8c64fe86u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[10],&fr[3],&f->fpscr,&fr[10]);if(status)return status;}goto L8c64fe88; /* fdiv fr3,fr10 */
L8c64fe88: *stop=0x8c64fe88u;r[9]<<=2;goto L8c64fe8a; /* shll2 r9 */
L8c64fe8a: *stop=0x8c64fe8au;r[9]+=r[8];goto L8c64fe8c; /* add r8,r9 */
L8c64fe8c: *stop=0x8c64fe8cu;fcopy(f,9u,4u);goto L8c64fe8e; /* fmov fr4,fr9 */
L8c64fe8e: *stop=0x8c64fe8eu;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c64fe90; /* fmov.s @(r0,r9),fr3 */
L8c64fe90: *stop=0x8c64fe90u;r[0]=12u;goto L8c64fe92; /* mov #0xc,r0 */
L8c64fe92: *stop=0x8c64fe92u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c64fe94; /* fmov.s @(r0,r14),fr15 */
L8c64fe94: *stop=0x8c64fe94u;r[0]=4u;goto L8c64fe96; /* mov #0x4,r0 */
L8c64fe96: *stop=0x8c64fe96u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[9],&fr[3],&f->fpscr,&fr[9]);if(status)return status;}goto L8c64fe98; /* fdiv fr3,fr9 */
L8c64fe98: *stop=0x8c64fe98u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c64fe9a; /* fmov.s @r8,fr3 */
L8c64fe9a: *stop=0x8c64fe9au;fcopy(f,11u,15u);goto L8c64fe9c; /* fmov fr15,fr11 */
L8c64fe9c: *stop=0x8c64fe9cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fe9e; /* fmul fr5,fr3 */
L8c64fe9e: *stop=0x8c64fe9eu;fcopy(f,0u,10u);goto L8c64fea0; /* fmov fr10,fr0 */
L8c64fea0: *stop=0x8c64fea0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[11],&f->fpscr,&fr[11]);if(status)return status;}goto L8c64fea2; /* fmac fr0,fr3,fr11 */
L8c64fea2: *stop=0x8c64fea2u;if(!valid_access(r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[9]);if(io_status)return io_status;goto L8c64fea4; /* fmov.s @r9,fr3 */
L8c64fea4: *stop=0x8c64fea4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fea6; /* fmul fr5,fr3 */
L8c64fea6: *stop=0x8c64fea6u;fcopy(f,0u,9u);goto L8c64fea8; /* fmov fr9,fr0 */
L8c64fea8: *stop=0x8c64fea8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[15],&f->fpscr,&fr[15]);if(status)return status;}goto L8c64feaa; /* fmac fr0,fr3,fr15 */
L8c64feaa: *stop=0x8c64feaau;if(!valid_access(r[0]+r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[8]);if(io_status)return io_status;goto L8c64feac; /* fmov.s @(r0,r8),fr3 */
L8c64feac: *stop=0x8c64feacu;r[0]=16u;goto L8c64feae; /* mov #0x10,r0 */
L8c64feae: *stop=0x8c64feaeu;fcopy(f,0u,10u);goto L8c64feb0; /* fmov fr10,fr0 */
L8c64feb0: *stop=0x8c64feb0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64feb2; /* fmul fr8,fr3 */
L8c64feb2: *stop=0x8c64feb2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]+r[14]);if(io_status)return io_status;goto L8c64feb4; /* fmov.s @(r0,r14),fr13 */
L8c64feb4: *stop=0x8c64feb4u;fcopy(f,14u,13u);goto L8c64feb6; /* fmov fr13,fr14 */
L8c64feb6: *stop=0x8c64feb6u;r[0]=4u;goto L8c64feb8; /* mov #0x4,r0 */
L8c64feb8: *stop=0x8c64feb8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[14],&f->fpscr,&fr[14]);if(status)return status;}goto L8c64feba; /* fmac fr0,fr3,fr14 */
L8c64feba: *stop=0x8c64febau;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c64febc; /* fmov.s @(r0,r9),fr3 */
L8c64febc: *stop=0x8c64febcu;r[0]=32u;goto L8c64febe; /* mov #0x20,r0 */
L8c64febe: *stop=0x8c64febeu;fcopy(f,0u,9u);goto L8c64fec0; /* fmov fr9,fr0 */
L8c64fec0: *stop=0x8c64fec0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fec2; /* fmul fr8,fr3 */
L8c64fec2: *stop=0x8c64fec2u;fcopy(f,1u,14u);goto L8c64fec4; /* fmov fr14,fr1 */
L8c64fec4: *stop=0x8c64fec4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[13],&f->fpscr,&fr[13]);if(status)return status;}goto L8c64fec6; /* fmac fr0,fr3,fr13 */
L8c64fec6: *stop=0x8c64fec6u;fcopy(f,3u,11u);goto L8c64fec8; /* fmov fr11,fr3 */
L8c64fec8: *stop=0x8c64fec8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[15],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c64feca; /* fsub fr15,fr3 */
L8c64feca: *stop=0x8c64fecau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c64fecc; /* fsub fr13,fr1 */
L8c64fecc: *stop=0x8c64feccu;fcopy(f,0u,3u);goto L8c64fece; /* fmov fr3,fr0 */
L8c64fece: *stop=0x8c64feceu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c64fed0; /* fmov fr3,@(r0,r15) */
L8c64fed0: *stop=0x8c64fed0u;r[0]=40u;goto L8c64fed2; /* mov #0x28,r0 */
L8c64fed2: *stop=0x8c64fed2u;fcopy(f,2u,1u);goto L8c64fed4; /* fmov fr1,fr2 */
L8c64fed4: *stop=0x8c64fed4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fed6; /* fmul fr1,fr2 */
L8c64fed6: *stop=0x8c64fed6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fed8; /* fmac fr0,fr3,fr2 */
L8c64fed8: *stop=0x8c64fed8u;fcopy(f,3u,4u);goto L8c64feda; /* fmov fr4,fr3 */
L8c64feda: *stop=0x8c64fedau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64fedc; /* fsqrt fr2 */
L8c64fedc: *stop=0x8c64fedcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fede; /* fdiv fr2,fr3 */
L8c64fede: *stop=0x8c64fedeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c64fee0; /* fmov fr3,@(r0,r15) */
L8c64fee0: *stop=0x8c64fee0u;r[0]=32u;goto L8c64fee2; /* mov #0x20,r0 */
L8c64fee2: *stop=0x8c64fee2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64fee4; /* fmul fr3,fr1 */
L8c64fee4: *stop=0x8c64fee4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c64fee6; /* fmov.s @(r0,r15),fr2 */
L8c64fee6: *stop=0x8c64fee6u;r[0]=36u;goto L8c64fee8; /* mov #0x24,r0 */
L8c64fee8: *stop=0x8c64fee8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64feea; /* fmul fr3,fr2 */
L8c64feea: *stop=0x8c64feeau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64feec; /* fmul fr6,fr1 */
L8c64feec: *stop=0x8c64feecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64feee; /* fmul fr6,fr2 */
L8c64feee: *stop=0x8c64feeeu;fcopy(f,3u,1u);goto L8c64fef0; /* fmov fr1,fr3 */
L8c64fef0: *stop=0x8c64fef0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64fef2; /* fmul fr7,fr3 */
L8c64fef2: *stop=0x8c64fef2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c64fef4; /* fneg fr2 */
L8c64fef4: *stop=0x8c64fef4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c64fef6; /* fmov fr2,@(r0,r15) */
L8c64fef6: *stop=0x8c64fef6u;r[0]=48u;goto L8c64fef8; /* mov #0x30,r0 */
L8c64fef8: *stop=0x8c64fef8u;r[8]=r[13];goto L8c64fefa; /* mov r13,r8 */
L8c64fefa: *stop=0x8c64fefau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c64fefc; /* fmov fr3,@(r0,r15) */
L8c64fefc: *stop=0x8c64fefcu;r[0]=36u;goto L8c64fefe; /* mov #0x24,r0 */
L8c64fefe: *stop=0x8c64fefeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c64ff00; /* fmov.s @(r0,r15),fr2 */
L8c64ff00: *stop=0x8c64ff00u;r[0]=44u;goto L8c64ff02; /* mov #0x2c,r0 */
L8c64ff02: *stop=0x8c64ff02u;r[4]+=28u;goto L8c64ff04; /* add #0x1c,r4 */
L8c64ff04: *stop=0x8c64ff04u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64ff06; /* fmul fr7,fr2 */
L8c64ff06: *stop=0x8c64ff06u;r[8]<<=2;goto L8c64ff08; /* shll2 r8 */
L8c64ff08: *stop=0x8c64ff08u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c64ff0a; /* fmov fr2,@(r0,r15) */
L8c64ff0a: *stop=0x8c64ff0au;r[0]=36u;goto L8c64ff0c; /* mov #0x24,r0 */
L8c64ff0c: *stop=0x8c64ff0cu;if(!valid_access(r[5]+4u,4u))return -3;r[9]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c64ff0e; /* mov.l @(0x4,r5),r9 */
L8c64ff0e: *stop=0x8c64ff0eu;fcopy(f,2u,14u);goto L8c64ff10; /* fmov fr14,fr2 */
L8c64ff10: *stop=0x8c64ff10u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[9],4);if(io_status)return io_status;goto L8c64ff12; /* mov.l r9,@(0x4,r15) */
L8c64ff12: *stop=0x8c64ff12u;if(!valid_access(r[9],4u))return -3;r[9]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c64ff14; /* mov.l @r9,r9 */
L8c64ff14: *stop=0x8c64ff14u;if(!valid_access(r[15]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c64ff16; /* mov.l @(0x4,r15),r3 */
L8c64ff16: *stop=0x8c64ff16u;r[8]+=r[3];goto L8c64ff18; /* add r3,r8 */
L8c64ff18: *stop=0x8c64ff18u;if(!valid_access(r[8],4u))return -3;r[8]=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;goto L8c64ff1a; /* mov.l @r8,r8 */
L8c64ff1a: *stop=0x8c64ff1au;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[9],4);if(io_status)return io_status;goto L8c64ff1c; /* mov.l r9,@-r4 */
L8c64ff1c: *stop=0x8c64ff1cu;r[4]+=4294967288u;goto L8c64ff1e; /* add #-0x8,r4 */
L8c64ff1e: *stop=0x8c64ff1eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;goto L8c64ff20; /* fmov.s fr10,@-r4 */
L8c64ff20: *stop=0x8c64ff20u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c64ff22; /* fmov.s @(r0,r15),fr3 */
L8c64ff22: *stop=0x8c64ff22u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c64ff24; /* fsub fr3,fr2 */
L8c64ff24: *stop=0x8c64ff24u;fcopy(f,3u,11u);goto L8c64ff26; /* fmov fr11,fr3 */
L8c64ff26: *stop=0x8c64ff26u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c64ff28; /* fsub fr1,fr3 */
L8c64ff28: *stop=0x8c64ff28u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64ff2a; /* fmov.s fr2,@-r4 */
L8c64ff2a: *stop=0x8c64ff2au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c64ff2c; /* fmov.s fr3,@-r4 */
L8c64ff2c: *stop=0x8c64ff2cu;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c64ff2e; /* mov.l r12,@-r4 */
L8c64ff2e: *stop=0x8c64ff2eu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c64ff30; /* pref @r4 */
L8c64ff30: *stop=0x8c64ff30u;r[0]=36u;goto L8c64ff32; /* mov #0x24,r0 */
L8c64ff32: *stop=0x8c64ff32u;fcopy(f,2u,13u);goto L8c64ff34; /* fmov fr13,fr2 */
L8c64ff34: *stop=0x8c64ff34u;r[4]+=60u;goto L8c64ff36; /* add #0x3c,r4 */
L8c64ff36: *stop=0x8c64ff36u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[8],4);if(io_status)return io_status;goto L8c64ff38; /* mov.l r8,@-r4 */
L8c64ff38: *stop=0x8c64ff38u;r[4]+=4294967288u;goto L8c64ff3a; /* add #-0x8,r4 */
L8c64ff3a: *stop=0x8c64ff3au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c64ff3c; /* fmov.s fr9,@-r4 */
L8c64ff3c: *stop=0x8c64ff3cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c64ff3e; /* fmov.s @(r0,r15),fr3 */
L8c64ff3e: *stop=0x8c64ff3eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c64ff40; /* fsub fr3,fr2 */
L8c64ff40: *stop=0x8c64ff40u;fcopy(f,3u,15u);goto L8c64ff42; /* fmov fr15,fr3 */
L8c64ff42: *stop=0x8c64ff42u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c64ff44; /* fsub fr1,fr3 */
L8c64ff44: *stop=0x8c64ff44u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c64ff46; /* fmov.s fr2,@-r4 */
L8c64ff46: *stop=0x8c64ff46u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c64ff48; /* fmov.s fr3,@-r4 */
L8c64ff48: *stop=0x8c64ff48u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c64ff4a; /* mov.l r12,@-r4 */
L8c64ff4a: *stop=0x8c64ff4au;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c64ff4c; /* pref @r4 */
L8c64ff4c: *stop=0x8c64ff4cu;r[0]=44u;goto L8c64ff4e; /* mov #0x2c,r0 */
L8c64ff4e: *stop=0x8c64ff4eu;r[4]+=60u;goto L8c64ff50; /* add #0x3c,r4 */
L8c64ff50: *stop=0x8c64ff50u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[9],4);if(io_status)return io_status;goto L8c64ff52; /* mov.l r9,@-r4 */
L8c64ff52: *stop=0x8c64ff52u;r[4]+=4294967288u;goto L8c64ff54; /* add #-0x8,r4 */
L8c64ff54: *stop=0x8c64ff54u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;goto L8c64ff56; /* fmov.s fr10,@-r4 */
L8c64ff56: *stop=0x8c64ff56u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c64ff58; /* fmov.s @(r0,r15),fr3 */
L8c64ff58: *stop=0x8c64ff58u;r[0]=48u;goto L8c64ff5a; /* mov #0x30,r0 */
L8c64ff5a: *stop=0x8c64ff5au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c64ff5c; /* fadd fr3,fr14 */
L8c64ff5c: *stop=0x8c64ff5cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[4]);if(io_status)return io_status;goto L8c64ff5e; /* fmov.s fr14,@-r4 */
L8c64ff5e: *stop=0x8c64ff5eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c64ff60; /* fmov.s @(r0,r15),fr3 */
L8c64ff60: *stop=0x8c64ff60u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],0);if(status)return status;}goto L8c64ff62; /* fadd fr3,fr11 */
L8c64ff62: *stop=0x8c64ff62u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;goto L8c64ff64; /* fmov.s fr11,@-r4 */
L8c64ff64: *stop=0x8c64ff64u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c64ff66; /* mov.l r12,@-r4 */
L8c64ff66: *stop=0x8c64ff66u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c64ff68; /* pref @r4 */
L8c64ff68: *stop=0x8c64ff68u;r[0]=44u;goto L8c64ff6a; /* mov #0x2c,r0 */
L8c64ff6a: *stop=0x8c64ff6au;r[4]+=60u;goto L8c64ff6c; /* add #0x3c,r4 */
L8c64ff6c: *stop=0x8c64ff6cu;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[8],4);if(io_status)return io_status;goto L8c64ff6e; /* mov.l r8,@-r4 */
L8c64ff6e: *stop=0x8c64ff6eu;r[4]+=4294967288u;goto L8c64ff70; /* add #-0x8,r4 */
L8c64ff70: *stop=0x8c64ff70u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c64ff72; /* fmov.s fr9,@-r4 */
L8c64ff72: *stop=0x8c64ff72u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c64ff74; /* fmov.s @(r0,r15),fr3 */
L8c64ff74: *stop=0x8c64ff74u;r[0]=48u;goto L8c64ff76; /* mov #0x30,r0 */
L8c64ff76: *stop=0x8c64ff76u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[3],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c64ff78; /* fadd fr3,fr13 */
L8c64ff78: *stop=0x8c64ff78u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[4]);if(io_status)return io_status;goto L8c64ff7a; /* fmov.s fr13,@-r4 */
L8c64ff7a: *stop=0x8c64ff7au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c64ff7c; /* fmov.s @(r0,r15),fr3 */
L8c64ff7c: *stop=0x8c64ff7cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],0);if(status)return status;}goto L8c64ff7e; /* fadd fr3,fr15 */
L8c64ff7e: *stop=0x8c64ff7eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[4]);if(io_status)return io_status;goto L8c64ff80; /* fmov.s fr15,@-r4 */
L8c64ff80: *stop=0x8c64ff80u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[10],4);if(io_status)return io_status;goto L8c64ff82; /* mov.l r10,@-r4 */
L8c64ff82: *stop=0x8c64ff82u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c64ff84; /* pref @r4 */
L8c64ff84: *stop=0x8c64ff84u;r[4]+=32u;goto L8c64ff86; /* add #0x20,r4 */
L8c64ff86: *stop=0x8c64ff86u;if(!valid_access(r[5]+4u,4u))return -3;r[9]=rd(m,bus,&io_status,r[5]+0x4u,4);if(io_status)return io_status;goto L8c64ff88; /* mov.l @(0x4,r5),r9 */
L8c64ff88: *stop=0x8c64ff88u;r[7]+=1u;goto L8c64ff8a; /* add #0x1,r7 */
L8c64ff8a: *stop=0x8c64ff8au;r[3]=r[7];goto L8c64ff8c; /* mov r7,r3 */
L8c64ff8c: *stop=0x8c64ff8cu;r[3]<<=2;goto L8c64ff8e; /* shll2 r3 */
L8c64ff8e: *stop=0x8c64ff8eu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[9],4);if(io_status)return io_status;goto L8c64ff90; /* mov.l r9,@(0x4,r15) */
L8c64ff90: *stop=0x8c64ff90u;r[2]=r[7];goto L8c64ff92; /* mov r7,r2 */
L8c64ff92: *stop=0x8c64ff92u;r[9]+=r[3];goto L8c64ff94; /* add r3,r9 */
L8c64ff94: *stop=0x8c64ff94u;if(!valid_access(r[15]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c64ff96; /* mov.l @(0x4,r15),r3 */
L8c64ff96: *stop=0x8c64ff96u;r[2]+=4294967295u;goto L8c64ff98; /* add #-0x1,r2 */
L8c64ff98: *stop=0x8c64ff98u;r[8]=r[7];goto L8c64ff9a; /* mov r7,r8 */
L8c64ff9a: *stop=0x8c64ff9au;r[2]<<=2;goto L8c64ff9c; /* shll2 r2 */
L8c64ff9c: *stop=0x8c64ff9cu;r[2]+=r[3];goto L8c64ff9e; /* add r3,r2 */
L8c64ff9e: *stop=0x8c64ff9eu;r[3]=r[7];goto L8c64ffa0; /* mov r7,r3 */
L8c64ffa0: *stop=0x8c64ffa0u;*sr=(*sr&~1u)|(r[8]>>31);r[8]<<=1;goto L8c64ffa2; /* shll r8 */
L8c64ffa2: *stop=0x8c64ffa2u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c64ffa4; /* mov.l r2,@(0x4,r15) */
L8c64ffa4: *stop=0x8c64ffa4u;if(!valid_access(r[5],4u))return -3;r[2]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64ffa6; /* mov.l @r5,r2 */
L8c64ffa6: *stop=0x8c64ffa6u;r[8]+=r[3];goto L8c64ffa8; /* add r3,r8 */
L8c64ffa8: *stop=0x8c64ffa8u;r[8]<<=2;goto L8c64ffaa; /* shll2 r8 */
L8c64ffaa: *stop=0x8c64ffaau;r[8]+=r[2];goto L8c64ffac; /* add r2,r8 */
L8c64ffac: *stop=0x8c64ffacu;r[2]=r[7];goto L8c64ffae; /* mov r7,r2 */
L8c64ffae: *stop=0x8c64ffaeu;r[1]=r[7];goto L8c64ffb0; /* mov r7,r1 */
L8c64ffb0: *stop=0x8c64ffb0u;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c64ffb2; /* shll r2 */
L8c64ffb2: *stop=0x8c64ffb2u;if(!valid_access(r[5],4u))return -3;r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64ffb4; /* mov.l @r5,r3 */
L8c64ffb4: *stop=0x8c64ffb4u;r[2]+=r[1];goto L8c64ffb6; /* add r1,r2 */
L8c64ffb6: *stop=0x8c64ffb6u;r[2]<<=2;goto L8c64ffb8; /* shll2 r2 */
L8c64ffb8: *stop=0x8c64ffb8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64ffba; /* mov.l r3,@r15 */
L8c64ffba: *stop=0x8c64ffbau;r[0]=8u;goto L8c64ffbc; /* mov #0x8,r0 */
L8c64ffbc: *stop=0x8c64ffbcu;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c64ffbe; /* fmov.s @r11,fr3 */
L8c64ffbe: *stop=0x8c64ffbeu;r[3]+=r[2];goto L8c64ffc0; /* add r2,r3 */
L8c64ffc0: *stop=0x8c64ffc0u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c64ffc2; /* fmov.s @(r0,r3),fr2 */
L8c64ffc2: *stop=0x8c64ffc2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c64ffc4; /* fcmp/eq fr3,fr2 */
L8c64ffc4: if(!(*sr&1u))goto L8c64ffe4;goto L8c64ffc6; /* bf 0x8c64ffe4 */
L8c64ffc6: *stop=0x8c64ffc6u;r[2]=r[7];goto L8c64ffc8; /* mov r7,r2 */
L8c64ffc8: *stop=0x8c64ffc8u;r[2]+=4294967295u;goto L8c64ffca; /* add #-0x1,r2 */
L8c64ffca: *stop=0x8c64ffcau;r[3]=r[2];goto L8c64ffcc; /* mov r2,r3 */
L8c64ffcc: *stop=0x8c64ffccu;*sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c64ffce; /* shll r2 */
L8c64ffce: *stop=0x8c64ffceu;if(!valid_access(r[15],4u))return -3;r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c64ffd0; /* mov.l @r15,r1 */
L8c64ffd0: *stop=0x8c64ffd0u;r[2]+=r[3];goto L8c64ffd2; /* add r3,r2 */
L8c64ffd2: *stop=0x8c64ffd2u;r[2]<<=2;goto L8c64ffd4; /* shll2 r2 */
L8c64ffd4: *stop=0x8c64ffd4u;if(!valid_access(r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[11]);if(io_status)return io_status;goto L8c64ffd6; /* fmov.s @r11,fr3 */
L8c64ffd6: *stop=0x8c64ffd6u;r[0]=8u;goto L8c64ffd8; /* mov #0x8,r0 */
L8c64ffd8: *stop=0x8c64ffd8u;r[2]+=r[1];goto L8c64ffda; /* add r1,r2 */
L8c64ffda: *stop=0x8c64ffdau;if(!valid_access(r[0]+r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c64ffdc; /* fmov.s @(r0,r2),fr2 */
L8c64ffdc: *stop=0x8c64ffdcu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c64ffde; /* fcmp/eq fr3,fr2 */
L8c64ffde: if(!(*sr&1u))goto L8c64ffe4;goto L8c64ffe0; /* bf 0x8c64ffe4 */
L8c64ffe0: *stop=0x8c64ffe2u;goto L8c6500fa; /* bra 0x8c6500fa */
L8c64ffe4: *stop=0x8c64ffe4u;r[3]=r[7];goto L8c64ffe6; /* mov r7,r3 */
L8c64ffe6: *stop=0x8c64ffe6u;r[3]+=4294967295u;goto L8c64ffe8; /* add #-0x1,r3 */
L8c64ffe8: *stop=0x8c64ffe8u;r[2]=r[3];goto L8c64ffea; /* mov r3,r2 */
L8c64ffea: *stop=0x8c64ffeau;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c64ffec; /* shll r3 */
L8c64ffec: *stop=0x8c64ffecu;if(!valid_access(r[5],4u))return -3;r[1]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c64ffee; /* mov.l @r5,r1 */
L8c64ffee: *stop=0x8c64ffeeu;r[3]+=r[2];goto L8c64fff0; /* add r2,r3 */
L8c64fff0: *stop=0x8c64fff0u;r[3]<<=2;goto L8c64fff2; /* shll2 r3 */
L8c64fff2: *stop=0x8c64fff2u;fcopy(f,10u,4u);goto L8c64fff4; /* fmov fr4,fr10 */
L8c64fff4: *stop=0x8c64fff4u;r[0]=8u;goto L8c64fff6; /* mov #0x8,r0 */
L8c64fff6: *stop=0x8c64fff6u;fcopy(f,9u,4u);goto L8c64fff8; /* fmov fr4,fr9 */
L8c64fff8: *stop=0x8c64fff8u;r[3]+=r[1];goto L8c64fffa; /* add r1,r3 */
L8c64fffa: *stop=0x8c64fffau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64fffc; /* mov.l r3,@r15 */
L8c64fffc: *stop=0x8c64fffcu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c64fffe; /* fmov.s @(r0,r3),fr3 */
L8c64fffe: *stop=0x8c64fffeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[10],&fr[3],&f->fpscr,&fr[10]);if(status)return status;}goto L8c650000; /* fdiv fr3,fr10 */
L8c650000: *stop=0x8c650000u;if(!valid_access(r[0]+r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[8]);if(io_status)return io_status;goto L8c650002; /* fmov.s @(r0,r8),fr3 */
L8c650002: *stop=0x8c650002u;r[0]=12u;goto L8c650004; /* mov #0xc,r0 */
L8c650004: *stop=0x8c650004u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[9],&fr[3],&f->fpscr,&fr[9]);if(status)return status;}goto L8c650006; /* fdiv fr3,fr9 */
L8c650006: *stop=0x8c650006u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c650008; /* fmov.s @r3,fr3 */
L8c650008: *stop=0x8c650008u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c65000a; /* fmov.s @(r0,r14),fr15 */
L8c65000a: *stop=0x8c65000au;r[0]=4u;goto L8c65000c; /* mov #0x4,r0 */
L8c65000c: *stop=0x8c65000cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c65000e; /* fmul fr5,fr3 */
L8c65000e: *stop=0x8c65000eu;fcopy(f,11u,15u);goto L8c650010; /* fmov fr15,fr11 */
L8c650010: *stop=0x8c650010u;fcopy(f,0u,10u);goto L8c650012; /* fmov fr10,fr0 */
L8c650012: *stop=0x8c650012u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[11],&f->fpscr,&fr[11]);if(status)return status;}goto L8c650014; /* fmac fr0,fr3,fr11 */
L8c650014: *stop=0x8c650014u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c650016; /* fmov.s @r8,fr3 */
L8c650016: *stop=0x8c650016u;fcopy(f,0u,9u);goto L8c650018; /* fmov fr9,fr0 */
L8c650018: *stop=0x8c650018u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c65001a; /* fmul fr5,fr3 */
L8c65001a: *stop=0x8c65001au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[15],&f->fpscr,&fr[15]);if(status)return status;}goto L8c65001c; /* fmac fr0,fr3,fr15 */
L8c65001c: *stop=0x8c65001cu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c65001e; /* fmov.s @(r0,r3),fr3 */
L8c65001e: *stop=0x8c65001eu;r[0]=16u;goto L8c650020; /* mov #0x10,r0 */
L8c650020: *stop=0x8c650020u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c650022; /* fmul fr8,fr3 */
L8c650022: *stop=0x8c650022u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]+r[14]);if(io_status)return io_status;goto L8c650024; /* fmov.s @(r0,r14),fr13 */
L8c650024: *stop=0x8c650024u;fcopy(f,0u,10u);goto L8c650026; /* fmov fr10,fr0 */
L8c650026: *stop=0x8c650026u;r[0]=4u;goto L8c650028; /* mov #0x4,r0 */
L8c650028: *stop=0x8c650028u;fcopy(f,14u,13u);goto L8c65002a; /* fmov fr13,fr14 */
L8c65002a: *stop=0x8c65002au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[14],&f->fpscr,&fr[14]);if(status)return status;}goto L8c65002c; /* fmac fr0,fr3,fr14 */
L8c65002c: *stop=0x8c65002cu;if(!valid_access(r[0]+r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[8]);if(io_status)return io_status;goto L8c65002e; /* fmov.s @(r0,r8),fr3 */
L8c65002e: *stop=0x8c65002eu;r[0]=8u;goto L8c650030; /* mov #0x8,r0 */
L8c650030: *stop=0x8c650030u;fcopy(f,0u,9u);goto L8c650032; /* fmov fr9,fr0 */
L8c650032: *stop=0x8c650032u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c650034; /* fmul fr8,fr3 */
L8c650034: *stop=0x8c650034u;fcopy(f,1u,14u);goto L8c650036; /* fmov fr14,fr1 */
L8c650036: *stop=0x8c650036u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[13],&f->fpscr,&fr[13]);if(status)return status;}goto L8c650038; /* fmac fr0,fr3,fr13 */
L8c650038: *stop=0x8c650038u;fcopy(f,3u,11u);goto L8c65003a; /* fmov fr11,fr3 */
L8c65003a: *stop=0x8c65003au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[15],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c65003c; /* fsub fr15,fr3 */
L8c65003c: *stop=0x8c65003cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c65003e; /* fsub fr13,fr1 */
L8c65003e: *stop=0x8c65003eu;fcopy(f,0u,3u);goto L8c650040; /* fmov fr3,fr0 */
L8c650040: *stop=0x8c650040u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650042; /* fmov fr3,@(r0,r15) */
L8c650042: *stop=0x8c650042u;r[0]=16u;goto L8c650044; /* mov #0x10,r0 */
L8c650044: *stop=0x8c650044u;fcopy(f,2u,1u);goto L8c650046; /* fmov fr1,fr2 */
L8c650046: *stop=0x8c650046u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c650048; /* fmul fr1,fr2 */
L8c650048: *stop=0x8c650048u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c65004a; /* fmac fr0,fr3,fr2 */
L8c65004a: *stop=0x8c65004au;fcopy(f,3u,4u);goto L8c65004c; /* fmov fr4,fr3 */
L8c65004c: *stop=0x8c65004cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c65004e; /* fsqrt fr2 */
L8c65004e: *stop=0x8c65004eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c650050; /* fdiv fr2,fr3 */
L8c650050: *stop=0x8c650050u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650052; /* fmov fr3,@(r0,r15) */
L8c650052: *stop=0x8c650052u;r[0]=8u;goto L8c650054; /* mov #0x8,r0 */
L8c650054: *stop=0x8c650054u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c650056; /* fmov.s @(r0,r15),fr2 */
L8c650056: *stop=0x8c650056u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c650058; /* fmul fr3,fr1 */
L8c650058: *stop=0x8c650058u;r[0]=12u;goto L8c65005a; /* mov #0xc,r0 */
L8c65005a: *stop=0x8c65005au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c65005c; /* fmul fr3,fr2 */
L8c65005c: *stop=0x8c65005cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c65005e; /* fmul fr6,fr1 */
L8c65005e: *stop=0x8c65005eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c650060; /* fmul fr6,fr2 */
L8c650060: *stop=0x8c650060u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c650062; /* fneg fr2 */
L8c650062: *stop=0x8c650062u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c650064; /* fmov fr2,@(r0,r15) */
L8c650064: *stop=0x8c650064u;fcopy(f,3u,1u);goto L8c650066; /* fmov fr1,fr3 */
L8c650066: *stop=0x8c650066u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c650068; /* fmul fr7,fr3 */
L8c650068: *stop=0x8c650068u;r[0]=28u;goto L8c65006a; /* mov #0x1c,r0 */
L8c65006a: *stop=0x8c65006au;r[4]+=28u;goto L8c65006c; /* add #0x1c,r4 */
L8c65006c: *stop=0x8c65006cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c65006e; /* fmov fr3,@(r0,r15) */
L8c65006e: *stop=0x8c65006eu;r[0]=12u;goto L8c650070; /* mov #0xc,r0 */
L8c650070: *stop=0x8c650070u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c650072; /* fmov.s @(r0,r15),fr2 */
L8c650072: *stop=0x8c650072u;r[0]=24u;goto L8c650074; /* mov #0x18,r0 */
L8c650074: *stop=0x8c650074u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c650076; /* fmul fr7,fr2 */
L8c650076: *stop=0x8c650076u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c650078; /* fmov fr2,@(r0,r15) */
L8c650078: *stop=0x8c650078u;r[0]=12u;goto L8c65007a; /* mov #0xc,r0 */
L8c65007a: *stop=0x8c65007au;if(!valid_access(r[15]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c65007c; /* mov.l @(0x4,r15),r3 */
L8c65007c: *stop=0x8c65007cu;fcopy(f,2u,14u);goto L8c65007e; /* fmov fr14,fr2 */
L8c65007e: *stop=0x8c65007eu;if(!valid_access(r[3],4u))return -3;r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c650080; /* mov.l @r3,r2 */
L8c650080: *stop=0x8c650080u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c650082; /* mov.l r2,@r15 */
L8c650082: *stop=0x8c650082u;if(!valid_access(r[9],4u))return -3;r[3]=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;goto L8c650084; /* mov.l @r9,r3 */
L8c650084: *stop=0x8c650084u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[3],4);if(io_status)return io_status;goto L8c650086; /* mov.l r3,@(0x14,r15) */
L8c650086: *stop=0x8c650086u;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c650088; /* mov.l @r15,r3 */
L8c650088: *stop=0x8c650088u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c65008a; /* mov.l r3,@-r4 */
L8c65008a: *stop=0x8c65008au;r[4]+=4294967288u;goto L8c65008c; /* add #-0x8,r4 */
L8c65008c: *stop=0x8c65008cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;goto L8c65008e; /* fmov.s fr10,@-r4 */
L8c65008e: *stop=0x8c65008eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c650090; /* fmov.s @(r0,r15),fr3 */
L8c650090: *stop=0x8c650090u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c650092; /* fsub fr3,fr2 */
L8c650092: *stop=0x8c650092u;fcopy(f,3u,11u);goto L8c650094; /* fmov fr11,fr3 */
L8c650094: *stop=0x8c650094u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c650096; /* fsub fr1,fr3 */
L8c650096: *stop=0x8c650096u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c650098; /* fmov.s fr2,@-r4 */
L8c650098: *stop=0x8c650098u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c65009a; /* fmov.s fr3,@-r4 */
L8c65009a: *stop=0x8c65009au;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c65009c; /* mov.l r12,@-r4 */
L8c65009c: *stop=0x8c65009cu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c65009e; /* pref @r4 */
L8c65009e: *stop=0x8c65009eu;r[0]=12u;goto L8c6500a0; /* mov #0xc,r0 */
L8c6500a0: *stop=0x8c6500a0u;fcopy(f,2u,13u);goto L8c6500a2; /* fmov fr13,fr2 */
L8c6500a2: *stop=0x8c6500a2u;if(!valid_access(r[15]+20u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c6500a4; /* mov.l @(0x14,r15),r3 */
L8c6500a4: *stop=0x8c6500a4u;r[4]+=60u;goto L8c6500a6; /* add #0x3c,r4 */
L8c6500a6: *stop=0x8c6500a6u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6500a8; /* mov.l r3,@-r4 */
L8c6500a8: *stop=0x8c6500a8u;r[4]+=4294967288u;goto L8c6500aa; /* add #-0x8,r4 */
L8c6500aa: *stop=0x8c6500aau;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c6500ac; /* fmov.s fr9,@-r4 */
L8c6500ac: *stop=0x8c6500acu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6500ae; /* fmov.s @(r0,r15),fr3 */
L8c6500ae: *stop=0x8c6500aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6500b0; /* fsub fr3,fr2 */
L8c6500b0: *stop=0x8c6500b0u;fcopy(f,3u,15u);goto L8c6500b2; /* fmov fr15,fr3 */
L8c6500b2: *stop=0x8c6500b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6500b4; /* fsub fr1,fr3 */
L8c6500b4: *stop=0x8c6500b4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c6500b6; /* fmov.s fr2,@-r4 */
L8c6500b6: *stop=0x8c6500b6u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c6500b8; /* fmov.s fr3,@-r4 */
L8c6500b8: *stop=0x8c6500b8u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c6500ba; /* mov.l r12,@-r4 */
L8c6500ba: *stop=0x8c6500bau;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c6500bc; /* pref @r4 */
L8c6500bc: *stop=0x8c6500bcu;r[0]=24u;goto L8c6500be; /* mov #0x18,r0 */
L8c6500be: *stop=0x8c6500beu;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6500c0; /* mov.l @r15,r3 */
L8c6500c0: *stop=0x8c6500c0u;r[4]+=60u;goto L8c6500c2; /* add #0x3c,r4 */
L8c6500c2: *stop=0x8c6500c2u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6500c4; /* mov.l r3,@-r4 */
L8c6500c4: *stop=0x8c6500c4u;r[4]+=4294967288u;goto L8c6500c6; /* add #-0x8,r4 */
L8c6500c6: *stop=0x8c6500c6u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;goto L8c6500c8; /* fmov.s fr10,@-r4 */
L8c6500c8: *stop=0x8c6500c8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6500ca; /* fmov.s @(r0,r15),fr3 */
L8c6500ca: *stop=0x8c6500cau;r[0]=28u;goto L8c6500cc; /* mov #0x1c,r0 */
L8c6500cc: *stop=0x8c6500ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c6500ce; /* fadd fr3,fr14 */
L8c6500ce: *stop=0x8c6500ceu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[4]);if(io_status)return io_status;goto L8c6500d0; /* fmov.s fr14,@-r4 */
L8c6500d0: *stop=0x8c6500d0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6500d2; /* fmov.s @(r0,r15),fr3 */
L8c6500d2: *stop=0x8c6500d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[3],&f->fpscr,&fr[11],0);if(status)return status;}goto L8c6500d4; /* fadd fr3,fr11 */
L8c6500d4: *stop=0x8c6500d4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;goto L8c6500d6; /* fmov.s fr11,@-r4 */
L8c6500d6: *stop=0x8c6500d6u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[12],4);if(io_status)return io_status;goto L8c6500d8; /* mov.l r12,@-r4 */
L8c6500d8: *stop=0x8c6500d8u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c6500da; /* pref @r4 */
L8c6500da: *stop=0x8c6500dau;r[0]=24u;goto L8c6500dc; /* mov #0x18,r0 */
L8c6500dc: *stop=0x8c6500dcu;if(!valid_access(r[15]+20u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c6500de; /* mov.l @(0x14,r15),r3 */
L8c6500de: *stop=0x8c6500deu;r[4]+=60u;goto L8c6500e0; /* add #0x3c,r4 */
L8c6500e0: *stop=0x8c6500e0u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6500e2; /* mov.l r3,@-r4 */
L8c6500e2: *stop=0x8c6500e2u;r[4]+=4294967288u;goto L8c6500e4; /* add #-0x8,r4 */
L8c6500e4: *stop=0x8c6500e4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c6500e6; /* fmov.s fr9,@-r4 */
L8c6500e6: *stop=0x8c6500e6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6500e8; /* fmov.s @(r0,r15),fr3 */
L8c6500e8: *stop=0x8c6500e8u;r[0]=28u;goto L8c6500ea; /* mov #0x1c,r0 */
L8c6500ea: *stop=0x8c6500eau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[3],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c6500ec; /* fadd fr3,fr13 */
L8c6500ec: *stop=0x8c6500ecu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[4]);if(io_status)return io_status;goto L8c6500ee; /* fmov.s fr13,@-r4 */
L8c6500ee: *stop=0x8c6500eeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c6500f0; /* fmov.s @(r0,r15),fr3 */
L8c6500f0: *stop=0x8c6500f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],0);if(status)return status;}goto L8c6500f2; /* fadd fr3,fr15 */
L8c6500f2: *stop=0x8c6500f2u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[4]);if(io_status)return io_status;goto L8c6500f4; /* fmov.s fr15,@-r4 */
L8c6500f4: *stop=0x8c6500f4u;if(!valid_access(r[4]-4u,4u))return -3;r[4]-=4;wr(m,bus,&io_status,r[4],r[10],4);if(io_status)return io_status;goto L8c6500f6; /* mov.l r10,@-r4 */
L8c6500f6: *stop=0x8c6500f6u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c6500f8; /* pref @r4 */
L8c6500f8: *stop=0x8c6500f8u;r[4]+=32u;goto L8c6500fa; /* add #0x20,r4 */
L8c6500fa: *stop=0x8c6500fau;if(!valid_access(r[15]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6500fc; /* mov.l @(0x4,r15),r3 */
L8c6500fc: *stop=0x8c6500fcu;r[7]+=1u;goto L8c6500fe; /* add #0x1,r7 */
L8c6500fe: *stop=0x8c6500feu;*sr=(*sr&~1u)|((int32_t)r[7]>(int32_t)r[13]);goto L8c650100; /* cmp/gt r13,r7 */
L8c650100: *stop=0x8c650100u;r[9]+=4u;goto L8c650102; /* add #0x4,r9 */
L8c650102: *stop=0x8c650102u;r[3]+=4u;goto L8c650104; /* add #0x4,r3 */
L8c650104: *stop=0x8c650104u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c650106; /* mov.l r3,@(0x4,r15) */
L8c650106: take=(*sr&1u);*stop=0x8c650108u;r[8]+=12u;if(take)goto L8c65010e;goto L8c65010a; /* bt/s 0x8c65010e */
L8c65010a: *stop=0x8c65010cu;goto L8c64ffac; /* bra 0x8c64ffac */
L8c65010e: *stop=0x8c65010eu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[4],4);if(io_status)return io_status;goto L8c650110; /* mov.l r4,@r6 */
L8c650110: *stop=0x8c650110u;r[15]+=52u;goto L8c650112; /* add #0x34,r15 */
L8c650112: *stop=0x8c650112u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c650114; /* fmov.s @r15+,fr13 */
L8c650114: *stop=0x8c650114u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c650116; /* fmov.s @r15+,fr14 */
L8c650116: *stop=0x8c650116u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c650118; /* fmov.s @r15+,fr15 */
L8c650118: *stop=0x8c650118u;if(!valid_access(r[15],4u))return -3;r[8]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c65011a; /* mov.l @r15+,r8 */
L8c65011a: *stop=0x8c65011au;if(!valid_access(r[15],4u))return -3;r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c65011c; /* mov.l @r15+,r9 */
L8c65011c: *stop=0x8c65011cu;if(!valid_access(r[15],4u))return -3;r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c65011e; /* mov.l @r15+,r10 */
L8c65011e: *stop=0x8c65011eu;if(!valid_access(r[15],4u))return -3;r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c650120; /* mov.l @r15+,r11 */
L8c650120: *stop=0x8c650120u;if(!valid_access(r[15],4u))return -3;r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c650122; /* mov.l @r15+,r12 */
L8c650122: *stop=0x8c650122u;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c650124; /* mov.l @r15+,r13 */
L8c650124: *stop=0x8c650124u;if(*pr!=entry_pr)return -3;*stop=0x8c650126u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
