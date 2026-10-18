/* Primitive emission and default scale buffer; explicit GBR/queue state.
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
static int valid_access(uint32_t a,unsigned n){
 if(valid_ram(a,n))return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_primitive_stream_8c037990(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c037990;
L8c037990: *stop=0x8c037990u;r[8]=rd(m,bus,&io_status,0x8c0379b8u,4);if(io_status)return io_status;goto L8c037992; /* mov.l 0x8c0379b8,r8 */
L8c037992: *stop=0x8c037992u;*sr=(*sr&~1u)|!(r[8]&r[6]);goto L8c037994; /* tst r8,r6 */
L8c037994: if(!(*sr&1u))goto L8c037a5c;goto L8c037996; /* bf 0x8c037a5c */
L8c037996: *stop=0x8c037996u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c037998; /* mov.l r3,@-r15 */
L8c037998: *stop=0x8c037998u;if(!valid_access(r[2]+4u,4u))return -3;r[8]=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;goto L8c03799a; /* mov.l @(0x4,r2),r8 */
L8c03799a: *stop=0x8c03799au;if(!valid_ram(*gbr+0x3cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x3cu,4);if(io_status)return io_status;goto L8c03799c; /* mov.l @(0x3c,GBR),r0 */
L8c03799c: *stop=0x8c03799cu;r[9]=r[0];goto L8c03799e; /* mov r0,r9 */
L8c03799e: *stop=0x8c03799eu;if(!valid_access(r[2]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c0379a0; /* mov.w @(0x2,r2),r0 */
L8c0379a0: *stop=0x8c0379a0u;if(!valid_ram(*gbr+0x44u,4u))return -3;wr(m,bus,&io_status,*gbr+0x44u,r[0],4);if(io_status)return io_status;goto L8c0379a2; /* mov.l r0,@(0x44,GBR) */
L8c0379a2: *stop=0x8c0379a2u;if(!valid_access(r[2]+20u,4u))return -3;r[1]=rd(m,bus,&io_status,r[2]+0x14u,4);if(io_status)return io_status;goto L8c0379a4; /* mov.l @(0x14,r2),r1 */
L8c0379a4: *stop=0x8c0379a4u;r[0]=rd(m,bus,&io_status,0x8c0379bcu,4);if(io_status)return io_status;goto L8c0379a6; /* mov.l 0x8c0379bc,r0 */
L8c0379a6: *stop=0x8c0379a6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[0]);if(io_status)return io_status;goto L8c0379a8; /* fmov.s @r0,fr11 */
L8c0379a8: *stop=0x8c0379a8u;if(!valid_ram(*gbr+0x38u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x38u,4);if(io_status)return io_status;goto L8c0379aa; /* mov.l @(0x38,GBR),r0 */
L8c0379aa: *stop=0x8c0379aau;r[3]=r[0];goto L8c0379ac; /* mov r0,r3 */
L8c0379ac: *stop=0x8c0379acu;if(!valid_ram(*gbr+0x54u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x54u,4);if(io_status)return io_status;goto L8c0379ae; /* mov.l @(0x54,GBR),r0 */
L8c0379ae: *stop=0x8c0379aeu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0379b0; /* tst r0,r0 */
L8c0379b0: if((*sr&1u))goto L8c0379c0;goto L8c0379b2; /* bt 0x8c0379c0 */
L8c0379b2: *stop=0x8c0379b4u;r[12]=r[0];goto L8c0379ca; /* bra 0x8c0379ca */
L8c0379c0: *stop=0x8c0379c0u;if(!valid_access(r[8],2u))return -3;r[12]=sx(m,bus,&io_status,r[8],2);if(io_status)return io_status;r[8]+=2;goto L8c0379c2; /* mov.w @r8+,r12 */
L8c0379c2: *stop=0x8c0379c2u;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c0379c4; /* cmp/pz r12 */
L8c0379c4: if((*sr&1u))goto L8c0379ca;goto L8c0379c6; /* bt 0x8c0379ca */
L8c0379c6: *stop=0x8c0379c6u;r[0]=sx(m,bus,&io_status,0x8c037a52u,2);if(io_status)return io_status;goto L8c0379c8; /* mov.w 0x8c037a52,r0 */
L8c0379c8: *stop=0x8c0379c8u;r[12]&=r[0];goto L8c0379ca; /* and r0,r12 */
L8c0379ca: *stop=0x8c0379cau;r[11]=rd(m,bus,&io_status,0x8c037a54u,4);if(io_status)return io_status;goto L8c0379cc; /* mov.l 0x8c037a54,r11 */
L8c0379cc: *stop=0x8c0379ccu;if(!valid_access(r[8],2u))return -3;r[0]=sx(m,bus,&io_status,r[8],2);if(io_status)return io_status;r[8]+=2;goto L8c0379ce; /* mov.w @r8+,r0 */
L8c0379ce: *stop=0x8c0379ceu;r[0]<<=2;goto L8c0379d0; /* shll2 r0 */
L8c0379d0: *stop=0x8c0379d0u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c0379d2; /* fmov.s @(r0,r3),fr4 */
L8c0379d2: *stop=0x8c0379d2u;r[0]<<=2;goto L8c0379d4; /* shll2 r0 */
L8c0379d4: *stop=0x8c0379d4u;r[0]+=r[9];goto L8c0379d6; /* add r9,r0 */
L8c0379d6: *stop=0x8c0379d6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0379d8; /* fmov.s @r0+,fr0 */
L8c0379d8: *stop=0x8c0379d8u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0379da; /* fmov.s @r0+,fr1 */
L8c0379da: *stop=0x8c0379dau;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0379dc; /* fmov.s @r0+,fr2 */
L8c0379dc: *stop=0x8c0379dcu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0379de; /* fmov.s @r0+,fr3 */
L8c0379de: *stop=0x8c0379deu;r[12]+=4294967295u;goto L8c0379e0; /* add #-0x1,r12 */
L8c0379e0: *stop=0x8c0379e0u;if(!valid_access(r[1],2u))return -3;r[0]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;r[1]+=2;goto L8c0379e2; /* mov.w @r1+,r0 */
L8c0379e2: *stop=0x8c0379e2u;*fpul=r[0];goto L8c0379e4; /* lds r0,FPUL */
L8c0379e4: *stop=0x8c0379e4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][5]);if(status)return status;goto L8c0379e6; /* float FPUL,fr5 */
L8c0379e6: *stop=0x8c0379e6u;if(!valid_access(r[1],2u))return -3;r[0]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;r[1]+=2;goto L8c0379e8; /* mov.w @r1+,r0 */
L8c0379e8: *stop=0x8c0379e8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[11],&f->fpscr,&fr[5]);if(status)return status;}goto L8c0379ea; /* fmul fr11,fr5 */
L8c0379ea: *stop=0x8c0379eau;*fpul=r[0];goto L8c0379ec; /* lds r0,FPUL */
L8c0379ec: *stop=0x8c0379ecu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][6]);if(status)return status;goto L8c0379ee; /* float FPUL,fr6 */
L8c0379ee: *stop=0x8c0379eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[11],&f->fpscr,&fr[6]);if(status)return status;}goto L8c0379f0; /* fmul fr11,fr6 */
L8c0379f0: *stop=0x8c0379f0u;r[14]+=32u;goto L8c0379f2; /* add #0x20,r14 */
L8c0379f2: *stop=0x8c0379f2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c0379f4; /* fmov.s fr4,@-r14 */
L8c0379f4: *stop=0x8c0379f4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c0379f6; /* fmov.s fr3,@-r14 */
L8c0379f6: *stop=0x8c0379f6u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c0379f8; /* fmov.s fr6,@-r14 */
L8c0379f8: *stop=0x8c0379f8u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c0379fa; /* fmov.s fr5,@-r14 */
L8c0379fa: *stop=0x8c0379fau;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c0379fc; /* fmov.s fr2,@-r14 */
L8c0379fc: *stop=0x8c0379fcu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c0379fe; /* fmov.s fr1,@-r14 */
L8c0379fe: *stop=0x8c0379feu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c037a00; /* fmov.s fr0,@-r14 */
L8c037a00: *stop=0x8c037a00u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c037a02; /* mov.l r11,@-r14 */
L8c037a02: *stop=0x8c037a02u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c037a04; /* pref @r14 */
L8c037a04: *stop=0x8c037a04u;r[14]+=32u;goto L8c037a06; /* add #0x20,r14 */
L8c037a06: *stop=0x8c037a06u;if(!valid_access(r[8],2u))return -3;r[0]=sx(m,bus,&io_status,r[8],2);if(io_status)return io_status;r[8]+=2;goto L8c037a08; /* mov.w @r8+,r0 */
L8c037a08: *stop=0x8c037a08u;r[0]<<=2;goto L8c037a0a; /* shll2 r0 */
L8c037a0a: *stop=0x8c037a0au;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c037a0c; /* fmov.s @(r0,r3),fr4 */
L8c037a0c: *stop=0x8c037a0cu;r[0]<<=2;goto L8c037a0e; /* shll2 r0 */
L8c037a0e: *stop=0x8c037a0eu;r[0]+=r[9];goto L8c037a10; /* add r9,r0 */
L8c037a10: *stop=0x8c037a10u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037a12; /* fmov.s @r0+,fr0 */
L8c037a12: *stop=0x8c037a12u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037a14; /* fmov.s @r0+,fr1 */
L8c037a14: *stop=0x8c037a14u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037a16; /* fmov.s @r0+,fr2 */
L8c037a16: *stop=0x8c037a16u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037a18; /* fmov.s @r0+,fr3 */
L8c037a18: *stop=0x8c037a18u;r[12]--;*sr=(*sr&~1u)|(!r[12]);goto L8c037a1a; /* dt r12 */
L8c037a1a: if(!(*sr&1u))goto L8c0379e0;goto L8c037a1c; /* bf 0x8c0379e0 */
L8c037a1c: *stop=0x8c037a1cu;if(!valid_access(r[1],2u))return -3;r[0]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;r[1]+=2;goto L8c037a1e; /* mov.w @r1+,r0 */
L8c037a1e: *stop=0x8c037a1eu;*fpul=r[0];goto L8c037a20; /* lds r0,FPUL */
L8c037a20: *stop=0x8c037a20u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][5]);if(status)return status;goto L8c037a22; /* float FPUL,fr5 */
L8c037a22: *stop=0x8c037a22u;if(!valid_access(r[1],2u))return -3;r[0]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;r[1]+=2;goto L8c037a24; /* mov.w @r1+,r0 */
L8c037a24: *stop=0x8c037a24u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[11],&f->fpscr,&fr[5]);if(status)return status;}goto L8c037a26; /* fmul fr11,fr5 */
L8c037a26: *stop=0x8c037a26u;*fpul=r[0];goto L8c037a28; /* lds r0,FPUL */
L8c037a28: *stop=0x8c037a28u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][6]);if(status)return status;goto L8c037a2a; /* float FPUL,fr6 */
L8c037a2a: *stop=0x8c037a2au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[11],&f->fpscr,&fr[6]);if(status)return status;}goto L8c037a2c; /* fmul fr11,fr6 */
L8c037a2c: *stop=0x8c037a2cu;r[11]=rd(m,bus,&io_status,0x8c037a58u,4);if(io_status)return io_status;goto L8c037a2e; /* mov.l 0x8c037a58,r11 */
L8c037a2e: *stop=0x8c037a2eu;r[14]+=32u;goto L8c037a30; /* add #0x20,r14 */
L8c037a30: *stop=0x8c037a30u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c037a32; /* fmov.s fr4,@-r14 */
L8c037a32: *stop=0x8c037a32u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c037a34; /* fmov.s fr3,@-r14 */
L8c037a34: *stop=0x8c037a34u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c037a36; /* fmov.s fr6,@-r14 */
L8c037a36: *stop=0x8c037a36u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c037a38; /* fmov.s fr5,@-r14 */
L8c037a38: *stop=0x8c037a38u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c037a3a; /* fmov.s fr2,@-r14 */
L8c037a3a: *stop=0x8c037a3au;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c037a3c; /* fmov.s fr1,@-r14 */
L8c037a3c: *stop=0x8c037a3cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c037a3e; /* fmov.s fr0,@-r14 */
L8c037a3e: *stop=0x8c037a3eu;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c037a40; /* mov.l r11,@-r14 */
L8c037a40: *stop=0x8c037a40u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c037a42; /* pref @r14 */
L8c037a42: *stop=0x8c037a42u;r[14]+=32u;goto L8c037a44; /* add #0x20,r14 */
L8c037a44: *stop=0x8c037a44u;if(!valid_ram(*gbr+0x44u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x44u,4);if(io_status)return io_status;goto L8c037a46; /* mov.l @(0x44,GBR),r0 */
L8c037a46: *stop=0x8c037a46u;r[0]--;*sr=(*sr&~1u)|(!r[0]);goto L8c037a48; /* dt r0 */
L8c037a48: *stop=0x8c037a48u;if(!valid_ram(*gbr+0x44u,4u))return -3;wr(m,bus,&io_status,*gbr+0x44u,r[0],4);if(io_status)return io_status;goto L8c037a4a; /* mov.l r0,@(0x44,GBR) */
L8c037a4a: if(!(*sr&1u))goto L8c0379ac;goto L8c037a4c; /* bf 0x8c0379ac */
L8c037a4c: *stop=0x8c037a4cu;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c037a4e; /* mov.l @r15+,r3 */
L8c037a4e: *stop=0x8c037a4eu;if(*pr!=entry_pr)return -3;*stop=0x8c037a50u;r[0]=r[14];return 0; /* rts */
L8c037a5c: *stop=0x8c037a5cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c037a5e; /* mov.l r3,@-r15 */
L8c037a5e: *stop=0x8c037a5eu;if(!valid_access(r[2]+4u,4u))return -3;r[8]=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;goto L8c037a60; /* mov.l @(0x4,r2),r8 */
L8c037a60: *stop=0x8c037a60u;if(!valid_ram(*gbr+0x3cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x3cu,4);if(io_status)return io_status;goto L8c037a62; /* mov.l @(0x3c,GBR),r0 */
L8c037a62: *stop=0x8c037a62u;r[9]=r[0];goto L8c037a64; /* mov r0,r9 */
L8c037a64: *stop=0x8c037a64u;if(!valid_access(r[2]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c037a66; /* mov.w @(0x2,r2),r0 */
L8c037a66: *stop=0x8c037a66u;if(!valid_ram(*gbr+0x44u,4u))return -3;wr(m,bus,&io_status,*gbr+0x44u,r[0],4);if(io_status)return io_status;goto L8c037a68; /* mov.l r0,@(0x44,GBR) */
L8c037a68: *stop=0x8c037a68u;if(!valid_access(r[4]+4u,4u))return -3;r[1]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c037a6a; /* mov.l @(0x4,r4),r1 */
L8c037a6a: *stop=0x8c037a6au;if(!valid_ram(*gbr+0x38u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x38u,4);if(io_status)return io_status;goto L8c037a6c; /* mov.l @(0x38,GBR),r0 */
L8c037a6c: *stop=0x8c037a6cu;r[3]=r[0];goto L8c037a6e; /* mov r0,r3 */
L8c037a6e: *stop=0x8c037a6eu;if(!valid_ram(*gbr+0x54u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x54u,4);if(io_status)return io_status;goto L8c037a70; /* mov.l @(0x54,GBR),r0 */
L8c037a70: *stop=0x8c037a70u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c037a72; /* tst r0,r0 */
L8c037a72: if((*sr&1u))goto L8c037a78;goto L8c037a74; /* bt 0x8c037a78 */
L8c037a74: *stop=0x8c037a76u;r[12]=r[0];goto L8c037a82; /* bra 0x8c037a82 */
L8c037a78: *stop=0x8c037a78u;if(!valid_access(r[8],2u))return -3;r[12]=sx(m,bus,&io_status,r[8],2);if(io_status)return io_status;r[8]+=2;goto L8c037a7a; /* mov.w @r8+,r12 */
L8c037a7a: *stop=0x8c037a7au;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c037a7c; /* cmp/pz r12 */
L8c037a7c: if((*sr&1u))goto L8c037a82;goto L8c037a7e; /* bt 0x8c037a82 */
L8c037a7e: *stop=0x8c037a7eu;r[0]=sx(m,bus,&io_status,0x8c037b2eu,2);if(io_status)return io_status;goto L8c037a80; /* mov.w 0x8c037b2e,r0 */
L8c037a80: *stop=0x8c037a80u;r[12]&=r[0];goto L8c037a82; /* and r0,r12 */
L8c037a82: *stop=0x8c037a82u;r[11]=rd(m,bus,&io_status,0x8c037b30u,4);if(io_status)return io_status;goto L8c037a84; /* mov.l 0x8c037b30,r11 */
L8c037a84: *stop=0x8c037a84u;if(!valid_access(r[8],2u))return -3;r[0]=sx(m,bus,&io_status,r[8],2);if(io_status)return io_status;r[8]+=2;goto L8c037a86; /* mov.w @r8+,r0 */
L8c037a86: *stop=0x8c037a86u;r[0]<<=2;goto L8c037a88; /* shll2 r0 */
L8c037a88: *stop=0x8c037a88u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c037a8a; /* fmov.s @(r0,r3),fr4 */
L8c037a8a: *stop=0x8c037a8au;r[6]=r[0];goto L8c037a8c; /* mov r0,r6 */
L8c037a8c: *stop=0x8c037a8cu;r[6]+=r[0];goto L8c037a8e; /* add r0,r6 */
L8c037a8e: *stop=0x8c037a8eu;r[6]+=r[0];goto L8c037a90; /* add r0,r6 */
L8c037a90: *stop=0x8c037a90u;r[0]<<=2;goto L8c037a92; /* shll2 r0 */
L8c037a92: *stop=0x8c037a92u;r[0]+=r[9];goto L8c037a94; /* add r9,r0 */
L8c037a94: *stop=0x8c037a94u;r[6]+=r[1];goto L8c037a96; /* add r1,r6 */
L8c037a96: *stop=0x8c037a96u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037a98; /* fmov.s @r6+,fr0 */
L8c037a98: *stop=0x8c037a98u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037a9a; /* fmov.s @r6+,fr1 */
L8c037a9a: *stop=0x8c037a9au;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037a9c; /* fmov.s @r6+,fr2 */
L8c037a9c: *stop=0x8c037a9cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c037a9e; /* fldi0 fr3 */
L8c037a9e: *stop=0x8c037a9eu;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c037aa0; /* ftrv xmtrx,fv0 */
L8c037aa0: *stop=0x8c037aa0u;r[6]=rd(m,bus,&io_status,0x8c037b34u,4);if(io_status)return io_status;goto L8c037aa2; /* mov.l 0x8c037b34,r6 */
L8c037aa2: *stop=0x8c037aa2u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c037aa4; /* fmov.s @r6,fr3 */
L8c037aa4: *stop=0x8c037aa4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c037aa6; /* fmul fr3,fr0 */
L8c037aa6: *stop=0x8c037aa6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c037aa8; /* fadd fr3,fr0 */
L8c037aa8: *stop=0x8c037aa8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c037aaa; /* fmul fr3,fr1 */
L8c037aaa: *stop=0x8c037aaau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c037aac; /* fadd fr3,fr1 */
L8c037aac: *stop=0x8c037aacu;fcopy(f,5u,0u);goto L8c037aae; /* fmov fr0,fr5 */
L8c037aae: *stop=0x8c037aaeu;fcopy(f,6u,1u);goto L8c037ab0; /* fmov fr1,fr6 */
L8c037ab0: *stop=0x8c037ab0u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037ab2; /* fmov.s @r0+,fr0 */
L8c037ab2: *stop=0x8c037ab2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037ab4; /* fmov.s @r0+,fr1 */
L8c037ab4: *stop=0x8c037ab4u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037ab6; /* fmov.s @r0+,fr2 */
L8c037ab6: *stop=0x8c037ab6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037ab8; /* fmov.s @r0+,fr3 */
L8c037ab8: *stop=0x8c037ab8u;r[12]+=4294967295u;goto L8c037aba; /* add #-0x1,r12 */
L8c037aba: *stop=0x8c037abau;r[14]+=32u;goto L8c037abc; /* add #0x20,r14 */
L8c037abc: *stop=0x8c037abcu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c037abe; /* fmov.s fr4,@-r14 */
L8c037abe: *stop=0x8c037abeu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c037ac0; /* fmov.s fr3,@-r14 */
L8c037ac0: *stop=0x8c037ac0u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c037ac2; /* fmov.s fr6,@-r14 */
L8c037ac2: *stop=0x8c037ac2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c037ac4; /* fmov.s fr5,@-r14 */
L8c037ac4: *stop=0x8c037ac4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c037ac6; /* fmov.s fr2,@-r14 */
L8c037ac6: *stop=0x8c037ac6u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c037ac8; /* fmov.s fr1,@-r14 */
L8c037ac8: *stop=0x8c037ac8u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c037aca; /* fmov.s fr0,@-r14 */
L8c037aca: *stop=0x8c037acau;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c037acc; /* mov.l r11,@-r14 */
L8c037acc: *stop=0x8c037accu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c037ace; /* pref @r14 */
L8c037ace: *stop=0x8c037aceu;r[14]+=32u;goto L8c037ad0; /* add #0x20,r14 */
L8c037ad0: *stop=0x8c037ad0u;if(!valid_access(r[8],2u))return -3;r[0]=sx(m,bus,&io_status,r[8],2);if(io_status)return io_status;r[8]+=2;goto L8c037ad2; /* mov.w @r8+,r0 */
L8c037ad2: *stop=0x8c037ad2u;r[0]<<=2;goto L8c037ad4; /* shll2 r0 */
L8c037ad4: *stop=0x8c037ad4u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c037ad6; /* fmov.s @(r0,r3),fr4 */
L8c037ad6: *stop=0x8c037ad6u;r[6]=r[0];goto L8c037ad8; /* mov r0,r6 */
L8c037ad8: *stop=0x8c037ad8u;r[6]+=r[0];goto L8c037ada; /* add r0,r6 */
L8c037ada: *stop=0x8c037adau;r[6]+=r[0];goto L8c037adc; /* add r0,r6 */
L8c037adc: *stop=0x8c037adcu;r[0]<<=2;goto L8c037ade; /* shll2 r0 */
L8c037ade: *stop=0x8c037adeu;r[0]+=r[9];goto L8c037ae0; /* add r9,r0 */
L8c037ae0: *stop=0x8c037ae0u;r[6]+=r[1];goto L8c037ae2; /* add r1,r6 */
L8c037ae2: *stop=0x8c037ae2u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037ae4; /* fmov.s @r6+,fr0 */
L8c037ae4: *stop=0x8c037ae4u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037ae6; /* fmov.s @r6+,fr1 */
L8c037ae6: *stop=0x8c037ae6u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037ae8; /* fmov.s @r6+,fr2 */
L8c037ae8: *stop=0x8c037ae8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c037aea; /* fldi0 fr3 */
L8c037aea: *stop=0x8c037aeau;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c037aec; /* ftrv xmtrx,fv0 */
L8c037aec: *stop=0x8c037aecu;r[6]=rd(m,bus,&io_status,0x8c037b34u,4);if(io_status)return io_status;goto L8c037aee; /* mov.l 0x8c037b34,r6 */
L8c037aee: *stop=0x8c037aeeu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c037af0; /* fmov.s @r6,fr3 */
L8c037af0: *stop=0x8c037af0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c037af2; /* fmul fr3,fr0 */
L8c037af2: *stop=0x8c037af2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c037af4; /* fadd fr3,fr0 */
L8c037af4: *stop=0x8c037af4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c037af6; /* fmul fr3,fr1 */
L8c037af6: *stop=0x8c037af6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c037af8; /* fadd fr3,fr1 */
L8c037af8: *stop=0x8c037af8u;fcopy(f,5u,0u);goto L8c037afa; /* fmov fr0,fr5 */
L8c037afa: *stop=0x8c037afau;fcopy(f,6u,1u);goto L8c037afc; /* fmov fr1,fr6 */
L8c037afc: *stop=0x8c037afcu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037afe; /* fmov.s @r0+,fr0 */
L8c037afe: *stop=0x8c037afeu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037b00; /* fmov.s @r0+,fr1 */
L8c037b00: *stop=0x8c037b00u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037b02; /* fmov.s @r0+,fr2 */
L8c037b02: *stop=0x8c037b02u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037b04; /* fmov.s @r0+,fr3 */
L8c037b04: *stop=0x8c037b04u;r[12]--;*sr=(*sr&~1u)|(!r[12]);goto L8c037b06; /* dt r12 */
L8c037b06: if(!(*sr&1u))goto L8c037aba;goto L8c037b08; /* bf 0x8c037aba */
L8c037b08: *stop=0x8c037b08u;r[11]=rd(m,bus,&io_status,0x8c037b38u,4);if(io_status)return io_status;goto L8c037b0a; /* mov.l 0x8c037b38,r11 */
L8c037b0a: *stop=0x8c037b0au;r[14]+=32u;goto L8c037b0c; /* add #0x20,r14 */
L8c037b0c: *stop=0x8c037b0cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c037b0e; /* fmov.s fr4,@-r14 */
L8c037b0e: *stop=0x8c037b0eu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c037b10; /* fmov.s fr3,@-r14 */
L8c037b10: *stop=0x8c037b10u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c037b12; /* fmov.s fr6,@-r14 */
L8c037b12: *stop=0x8c037b12u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c037b14; /* fmov.s fr5,@-r14 */
L8c037b14: *stop=0x8c037b14u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c037b16; /* fmov.s fr2,@-r14 */
L8c037b16: *stop=0x8c037b16u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c037b18; /* fmov.s fr1,@-r14 */
L8c037b18: *stop=0x8c037b18u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c037b1a; /* fmov.s fr0,@-r14 */
L8c037b1a: *stop=0x8c037b1au;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c037b1c; /* mov.l r11,@-r14 */
L8c037b1c: *stop=0x8c037b1cu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c037b1e; /* pref @r14 */
L8c037b1e: *stop=0x8c037b1eu;r[14]+=32u;goto L8c037b20; /* add #0x20,r14 */
L8c037b20: *stop=0x8c037b20u;if(!valid_ram(*gbr+0x44u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x44u,4);if(io_status)return io_status;goto L8c037b22; /* mov.l @(0x44,GBR),r0 */
L8c037b22: *stop=0x8c037b22u;r[0]--;*sr=(*sr&~1u)|(!r[0]);goto L8c037b24; /* dt r0 */
L8c037b24: *stop=0x8c037b24u;if(!valid_ram(*gbr+0x44u,4u))return -3;wr(m,bus,&io_status,*gbr+0x44u,r[0],4);if(io_status)return io_status;goto L8c037b26; /* mov.l r0,@(0x44,GBR) */
L8c037b26: if(!(*sr&1u))goto L8c037a6e;goto L8c037b28; /* bf 0x8c037a6e */
L8c037b28: *stop=0x8c037b28u;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c037b2a; /* mov.l @r15+,r3 */
L8c037b2a: *stop=0x8c037b2au;if(*pr!=entry_pr)return -3;*stop=0x8c037b2cu;r[0]=r[14];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_primitive_stream_8c037c3c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c037c3c;
L8c037c3c: *stop=0x8c037c3cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c037c3e; /* mov.l r3,@-r15 */
L8c037c3e: *stop=0x8c037c3eu;if(!valid_access(r[2]+4u,4u))return -3;r[8]=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;goto L8c037c40; /* mov.l @(0x4,r2),r8 */
L8c037c40: *stop=0x8c037c40u;if(!valid_ram(*gbr+0x3cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x3cu,4);if(io_status)return io_status;goto L8c037c42; /* mov.l @(0x3c,GBR),r0 */
L8c037c42: *stop=0x8c037c42u;r[9]=r[0];goto L8c037c44; /* mov r0,r9 */
L8c037c44: *stop=0x8c037c44u;if(!valid_access(r[2]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c037c46; /* mov.w @(0x2,r2),r0 */
L8c037c46: *stop=0x8c037c46u;if(!valid_ram(*gbr+0x44u,4u))return -3;wr(m,bus,&io_status,*gbr+0x44u,r[0],4);if(io_status)return io_status;goto L8c037c48; /* mov.l r0,@(0x44,GBR) */
L8c037c48: *stop=0x8c037c48u;if(!valid_ram(*gbr+0x38u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x38u,4);if(io_status)return io_status;goto L8c037c4a; /* mov.l @(0x38,GBR),r0 */
L8c037c4a: *stop=0x8c037c4au;r[3]=r[0];goto L8c037c4c; /* mov r0,r3 */
L8c037c4c: *stop=0x8c037c4cu;if(!valid_ram(*gbr+0x54u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x54u,4);if(io_status)return io_status;goto L8c037c4e; /* mov.l @(0x54,GBR),r0 */
L8c037c4e: *stop=0x8c037c4eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c037c50; /* tst r0,r0 */
L8c037c50: if((*sr&1u))goto L8c037c56;goto L8c037c52; /* bt 0x8c037c56 */
L8c037c52: *stop=0x8c037c54u;r[12]=r[0];goto L8c037c60; /* bra 0x8c037c60 */
L8c037c56: *stop=0x8c037c56u;if(!valid_access(r[8],2u))return -3;r[12]=sx(m,bus,&io_status,r[8],2);if(io_status)return io_status;r[8]+=2;goto L8c037c58; /* mov.w @r8+,r12 */
L8c037c58: *stop=0x8c037c58u;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c037c5a; /* cmp/pz r12 */
L8c037c5a: if((*sr&1u))goto L8c037c60;goto L8c037c5c; /* bt 0x8c037c60 */
L8c037c5c: *stop=0x8c037c5cu;r[0]=sx(m,bus,&io_status,0x8c037cc0u,2);if(io_status)return io_status;goto L8c037c5e; /* mov.w 0x8c037cc0,r0 */
L8c037c5e: *stop=0x8c037c5eu;r[12]&=r[0];goto L8c037c60; /* and r0,r12 */
L8c037c60: *stop=0x8c037c60u;r[11]=rd(m,bus,&io_status,0x8c037cc4u,4);if(io_status)return io_status;goto L8c037c62; /* mov.l 0x8c037cc4,r11 */
L8c037c62: *stop=0x8c037c62u;if(!valid_access(r[8],2u))return -3;r[0]=sx(m,bus,&io_status,r[8],2);if(io_status)return io_status;r[8]+=2;goto L8c037c64; /* mov.w @r8+,r0 */
L8c037c64: *stop=0x8c037c64u;r[0]<<=2;goto L8c037c66; /* shll2 r0 */
L8c037c66: *stop=0x8c037c66u;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c037c68; /* fmov.s @(r0,r3),fr4 */
L8c037c68: *stop=0x8c037c68u;r[0]<<=2;goto L8c037c6a; /* shll2 r0 */
L8c037c6a: *stop=0x8c037c6au;r[0]+=r[9];goto L8c037c6c; /* add r9,r0 */
L8c037c6c: *stop=0x8c037c6cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037c6e; /* fmov.s @r0+,fr0 */
L8c037c6e: *stop=0x8c037c6eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037c70; /* fmov.s @r0+,fr1 */
L8c037c70: *stop=0x8c037c70u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037c72; /* fmov.s @r0+,fr2 */
L8c037c72: *stop=0x8c037c72u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037c74; /* fmov.s @r0+,fr3 */
L8c037c74: *stop=0x8c037c74u;r[12]+=4294967295u;goto L8c037c76; /* add #-0x1,r12 */
L8c037c76: *stop=0x8c037c76u;r[14]+=28u;goto L8c037c78; /* add #0x1c,r14 */
L8c037c78: *stop=0x8c037c78u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c037c7a; /* fmov.s fr3,@-r14 */
L8c037c7a: *stop=0x8c037c7au;r[14]+=4294967288u;goto L8c037c7c; /* add #-0x8,r14 */
L8c037c7c: *stop=0x8c037c7cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c037c7e; /* fmov.s fr2,@-r14 */
L8c037c7e: *stop=0x8c037c7eu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c037c80; /* fmov.s fr1,@-r14 */
L8c037c80: *stop=0x8c037c80u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c037c82; /* fmov.s fr0,@-r14 */
L8c037c82: *stop=0x8c037c82u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c037c84; /* mov.l r11,@-r14 */
L8c037c84: *stop=0x8c037c84u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c037c86; /* pref @r14 */
L8c037c86: *stop=0x8c037c86u;r[14]+=32u;goto L8c037c88; /* add #0x20,r14 */
L8c037c88: *stop=0x8c037c88u;if(!valid_access(r[8],2u))return -3;r[0]=sx(m,bus,&io_status,r[8],2);if(io_status)return io_status;r[8]+=2;goto L8c037c8a; /* mov.w @r8+,r0 */
L8c037c8a: *stop=0x8c037c8au;r[0]<<=2;goto L8c037c8c; /* shll2 r0 */
L8c037c8c: *stop=0x8c037c8cu;if(!valid_access(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c037c8e; /* fmov.s @(r0,r3),fr4 */
L8c037c8e: *stop=0x8c037c8eu;r[0]<<=2;goto L8c037c90; /* shll2 r0 */
L8c037c90: *stop=0x8c037c90u;r[0]+=r[9];goto L8c037c92; /* add r9,r0 */
L8c037c92: *stop=0x8c037c92u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037c94; /* fmov.s @r0+,fr0 */
L8c037c94: *stop=0x8c037c94u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037c96; /* fmov.s @r0+,fr1 */
L8c037c96: *stop=0x8c037c96u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037c98; /* fmov.s @r0+,fr2 */
L8c037c98: *stop=0x8c037c98u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037c9a; /* fmov.s @r0+,fr3 */
L8c037c9a: *stop=0x8c037c9au;r[12]--;*sr=(*sr&~1u)|(!r[12]);goto L8c037c9c; /* dt r12 */
L8c037c9c: if(!(*sr&1u))goto L8c037c76;goto L8c037c9e; /* bf 0x8c037c76 */
L8c037c9e: *stop=0x8c037c9eu;r[11]=rd(m,bus,&io_status,0x8c037cc8u,4);if(io_status)return io_status;goto L8c037ca0; /* mov.l 0x8c037cc8,r11 */
L8c037ca0: *stop=0x8c037ca0u;r[14]+=28u;goto L8c037ca2; /* add #0x1c,r14 */
L8c037ca2: *stop=0x8c037ca2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c037ca4; /* fmov.s fr3,@-r14 */
L8c037ca4: *stop=0x8c037ca4u;r[14]+=4294967288u;goto L8c037ca6; /* add #-0x8,r14 */
L8c037ca6: *stop=0x8c037ca6u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c037ca8; /* fmov.s fr2,@-r14 */
L8c037ca8: *stop=0x8c037ca8u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c037caa; /* fmov.s fr1,@-r14 */
L8c037caa: *stop=0x8c037caau;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c037cac; /* fmov.s fr0,@-r14 */
L8c037cac: *stop=0x8c037cacu;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c037cae; /* mov.l r11,@-r14 */
L8c037cae: *stop=0x8c037caeu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c037cb0; /* pref @r14 */
L8c037cb0: *stop=0x8c037cb0u;r[14]+=32u;goto L8c037cb2; /* add #0x20,r14 */
L8c037cb2: *stop=0x8c037cb2u;if(!valid_ram(*gbr+0x44u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x44u,4);if(io_status)return io_status;goto L8c037cb4; /* mov.l @(0x44,GBR),r0 */
L8c037cb4: *stop=0x8c037cb4u;r[0]--;*sr=(*sr&~1u)|(!r[0]);goto L8c037cb6; /* dt r0 */
L8c037cb6: *stop=0x8c037cb6u;if(!valid_ram(*gbr+0x44u,4u))return -3;wr(m,bus,&io_status,*gbr+0x44u,r[0],4);if(io_status)return io_status;goto L8c037cb8; /* mov.l r0,@(0x44,GBR) */
L8c037cb8: if(!(*sr&1u))goto L8c037c4c;goto L8c037cba; /* bf 0x8c037c4c */
L8c037cba: *stop=0x8c037cbau;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c037cbc; /* mov.l @r15+,r3 */
L8c037cbc: *stop=0x8c037cbcu;if(*pr!=entry_pr)return -3;*stop=0x8c037cbeu;r[0]=r[14];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_primitive_stream_8c037ccc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c037ccc;
L8c037ccc: *stop=0x8c037cccu;r[8]=rd(m,bus,&io_status,0x8c037cd8u,4);if(io_status)return io_status;goto L8c037cce; /* mov.l 0x8c037cd8,r8 */
L8c037cce: *stop=0x8c037cceu;*sr=(*sr&~1u)|!(r[8]&r[6]);goto L8c037cd0; /* tst r8,r6 */
L8c037cd0: if((*sr&1u))goto L8c037cdc;goto L8c037cd2; /* bt 0x8c037cdc */
L8c037cd2: *stop=0x8c037cd4u;if(!call)return -2;return call(ctx,0x8c0382acu,m,f,sr,r,pr); /* bra 0x8c0382ac */
L8c037cdc: *stop=0x8c037cdcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c037cde; /* sts.l PR,@-r15 */
L8c037cde: *stop=0x8c037cdeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c037ce0; /* mov.l r13,@-r15 */
L8c037ce0: *stop=0x8c037ce0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c037ce2; /* mov.l r7,@-r15 */
L8c037ce2: *stop=0x8c037ce2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c037ce4; /* mov.l r4,@-r15 */
L8c037ce4: *stop=0x8c037ce4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c037ce6; /* mov.l r3,@-r15 */
L8c037ce6: *stop=0x8c037ce6u;if(!valid_access(r[2]+4u,4u))return -3;r[4]=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;goto L8c037ce8; /* mov.l @(0x4,r2),r4 */
L8c037ce8: *stop=0x8c037ce8u;if(!valid_ram(*gbr+0x3cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x3cu,4);if(io_status)return io_status;goto L8c037cea; /* mov.l @(0x3c,GBR),r0 */
L8c037cea: *stop=0x8c037ceau;r[5]=r[0];goto L8c037cec; /* mov r0,r5 */
L8c037cec: *stop=0x8c037cecu;if(!valid_access(r[2]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c037cee; /* mov.w @(0x2,r2),r0 */
L8c037cee: *stop=0x8c037ceeu;if(!valid_ram(*gbr+0x44u,4u))return -3;wr(m,bus,&io_status,*gbr+0x44u,r[0],4);if(io_status)return io_status;goto L8c037cf0; /* mov.l r0,@(0x44,GBR) */
L8c037cf0: *stop=0x8c037cf0u;if(!valid_access(r[2]+20u,4u))return -3;r[1]=rd(m,bus,&io_status,r[2]+0x14u,4);if(io_status)return io_status;goto L8c037cf2; /* mov.l @(0x14,r2),r1 */
L8c037cf2: *stop=0x8c037cf2u;r[0]=rd(m,bus,&io_status,0x8c037d04u,4);if(io_status)return io_status;goto L8c037cf4; /* mov.l 0x8c037d04,r0 */
L8c037cf4: *stop=0x8c037cf4u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[0]);if(io_status)return io_status;goto L8c037cf6; /* fmov.s @r0,fr11 */
L8c037cf6: *stop=0x8c037cf6u;if(!valid_ram(*gbr+0x38u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x38u,4);if(io_status)return io_status;goto L8c037cf8; /* mov.l @(0x38,GBR),r0 */
L8c037cf8: *stop=0x8c037cf8u;r[3]=r[0];goto L8c037cfa; /* mov r0,r3 */
L8c037cfa: *stop=0x8c037cfau;if(!valid_ram(*gbr+0x54u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x54u,4);if(io_status)return io_status;goto L8c037cfc; /* mov.l @(0x54,GBR),r0 */
L8c037cfc: *stop=0x8c037cfcu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c037cfe; /* tst r0,r0 */
L8c037cfe: if((*sr&1u))goto L8c037d08;goto L8c037d00; /* bt 0x8c037d08 */
L8c037d00: *stop=0x8c037d02u;r[12]=r[0];goto L8c037d12; /* bra 0x8c037d12 */
L8c037d08: *stop=0x8c037d08u;if(!valid_access(r[4],2u))return -3;r[12]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c037d0a; /* mov.w @r4+,r12 */
L8c037d0a: *stop=0x8c037d0au;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c037d0c; /* cmp/pz r12 */
L8c037d0c: if((*sr&1u))goto L8c037d12;goto L8c037d0e; /* bt 0x8c037d12 */
L8c037d0e: *stop=0x8c037d0eu;r[0]=sx(m,bus,&io_status,0x8c037dfau,2);if(io_status)return io_status;goto L8c037d10; /* mov.w 0x8c037dfa,r0 */
L8c037d10: *stop=0x8c037d10u;r[12]&=r[0];goto L8c037d12; /* and r0,r12 */
L8c037d12: *stop=0x8c037d12u;r[12]+=4294967294u;goto L8c037d14; /* add #-0x2,r12 */
L8c037d14: *stop=0x8c037d14u;r[11]=rd(m,bus,&io_status,0x8c037dfcu,4);if(io_status)return io_status;goto L8c037d16; /* mov.l 0x8c037dfc,r11 */
L8c037d16: *stop=0x8c037d16u;r[6]=0u;goto L8c037d18; /* mov #0x0,r6 */
L8c037d18: *stop=0x8c037d18u;if(!valid_access(r[4],2u))return -3;r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c037d1a; /* mov.w @r4+,r0 */
L8c037d1a: *stop=0x8c037d1au;r[0]<<=2;goto L8c037d1c; /* shll2 r0 */
L8c037d1c: *stop=0x8c037d1cu;r[8]=r[0];goto L8c037d1e; /* mov r0,r8 */
L8c037d1e: *stop=0x8c037d1eu;if(!valid_access(r[0]+r[3],4u))return -3;r[0]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c037d20; /* mov.l @(r0,r3),r0 */
L8c037d20: *stop=0x8c037d20u;if(!valid_ram(*gbr+0x48u,4u))return -3;wr(m,bus,&io_status,*gbr+0x48u,r[0],4);if(io_status)return io_status;goto L8c037d22; /* mov.l r0,@(0x48,GBR) */
L8c037d22: *stop=0x8c037d22u;r[8]<<=2;goto L8c037d24; /* shll2 r8 */
L8c037d24: *stop=0x8c037d24u;r[8]+=r[5];goto L8c037d26; /* add r5,r8 */
L8c037d26: *stop=0x8c037d26u;if(!valid_access(r[4],2u))return -3;r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c037d28; /* mov.w @r4+,r0 */
L8c037d28: *stop=0x8c037d28u;r[0]<<=2;goto L8c037d2a; /* shll2 r0 */
L8c037d2a: *stop=0x8c037d2au;r[9]=r[0];goto L8c037d2c; /* mov r0,r9 */
L8c037d2c: *stop=0x8c037d2cu;if(!valid_access(r[0]+r[3],4u))return -3;r[0]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c037d2e; /* mov.l @(r0,r3),r0 */
L8c037d2e: *stop=0x8c037d2eu;if(!valid_ram(*gbr+0x4cu,4u))return -3;wr(m,bus,&io_status,*gbr+0x4cu,r[0],4);if(io_status)return io_status;goto L8c037d30; /* mov.l r0,@(0x4c,GBR) */
L8c037d30: *stop=0x8c037d30u;r[9]<<=2;goto L8c037d32; /* shll2 r9 */
L8c037d32: *stop=0x8c037d32u;r[9]+=r[5];goto L8c037d34; /* add r5,r9 */
L8c037d34: *stop=0x8c037d34u;r[0]=8u;goto L8c037d36; /* mov #0x8,r0 */
L8c037d36: *stop=0x8c037d36u;if(!valid_access(r[0]+r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]+r[8]);if(io_status)return io_status;goto L8c037d38; /* fmov.s @(r0,r8),fr9 */
L8c037d38: *stop=0x8c037d38u;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]+r[9]);if(io_status)return io_status;goto L8c037d3a; /* fmov.s @(r0,r9),fr12 */
L8c037d3a: *stop=0x8c037d3au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0;goto L8c037d3c; /* fldi0 fr0 */
L8c037d3c: *stop=0x8c037d3cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0x3f800000u;goto L8c037d3e; /* fldi1 fr1 */
L8c037d3e: *stop=0x8c037d3eu;r[0]=0u;goto L8c037d40; /* mov #0x0,r0 */
L8c037d40: *stop=0x8c037d40u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][9],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c037d42; /* fcmp/gt fr0,fr9 */
L8c037d42: if((*sr&1u))goto L8c037d46;goto L8c037d44; /* bt 0x8c037d46 */
L8c037d44: *stop=0x8c037d44u;r[0]|=0x1u;goto L8c037d46; /* or #0x1,r0 */
L8c037d46: *stop=0x8c037d46u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][9],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c037d48; /* fcmp/gt fr1,fr9 */
L8c037d48: if(!(*sr&1u))goto L8c037d4c;goto L8c037d4a; /* bf 0x8c037d4c */
L8c037d4a: *stop=0x8c037d4au;r[0]|=0x10u;goto L8c037d4c; /* or #0x10,r0 */
L8c037d4c: *stop=0x8c037d4cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c037d4e; /* fcmp/gt fr0,fr12 */
L8c037d4e: if((*sr&1u))goto L8c037d52;goto L8c037d50; /* bt 0x8c037d52 */
L8c037d50: *stop=0x8c037d50u;r[0]|=0x2u;goto L8c037d52; /* or #0x2,r0 */
L8c037d52: *stop=0x8c037d52u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c037d54; /* fcmp/gt fr1,fr12 */
L8c037d54: if(!(*sr&1u))goto L8c037d58;goto L8c037d56; /* bf 0x8c037d58 */
L8c037d56: *stop=0x8c037d56u;r[0]|=0x20u;goto L8c037d58; /* or #0x20,r0 */
L8c037d58: *stop=0x8c037d58u;r[7]=r[0];goto L8c037d5a; /* mov r0,r7 */
L8c037d5a: *stop=0x8c037d5au;if(!valid_access(r[4],2u))return -3;r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c037d5c; /* mov.w @r4+,r0 */
L8c037d5c: *stop=0x8c037d5cu;r[0]<<=2;goto L8c037d5e; /* shll2 r0 */
L8c037d5e: *stop=0x8c037d5eu;r[10]=r[0];goto L8c037d60; /* mov r0,r10 */
L8c037d60: *stop=0x8c037d60u;if(!valid_access(r[0]+r[3],4u))return -3;r[0]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c037d62; /* mov.l @(r0,r3),r0 */
L8c037d62: *stop=0x8c037d62u;if(!valid_ram(*gbr+0x50u,4u))return -3;wr(m,bus,&io_status,*gbr+0x50u,r[0],4);if(io_status)return io_status;goto L8c037d64; /* mov.l r0,@(0x50,GBR) */
L8c037d64: *stop=0x8c037d64u;r[10]<<=2;goto L8c037d66; /* shll2 r10 */
L8c037d66: *stop=0x8c037d66u;r[10]+=r[5];goto L8c037d68; /* add r5,r10 */
L8c037d68: *stop=0x8c037d68u;r[0]=8u;goto L8c037d6a; /* mov #0x8,r0 */
L8c037d6a: *stop=0x8c037d6au;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[10]);if(io_status)return io_status;goto L8c037d6c; /* fmov.s @(r0,r10),fr15 */
L8c037d6c: *stop=0x8c037d6cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0;goto L8c037d6e; /* fldi0 fr0 */
L8c037d6e: *stop=0x8c037d6eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0x3f800000u;goto L8c037d70; /* fldi1 fr1 */
L8c037d70: *stop=0x8c037d70u;r[0]=r[7];goto L8c037d72; /* mov r7,r0 */
L8c037d72: *stop=0x8c037d72u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c037d74; /* fcmp/gt fr0,fr15 */
L8c037d74: if((*sr&1u))goto L8c037d78;goto L8c037d76; /* bt 0x8c037d78 */
L8c037d76: *stop=0x8c037d76u;r[0]|=0x4u;goto L8c037d78; /* or #0x4,r0 */
L8c037d78: *stop=0x8c037d78u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c037d7a; /* fcmp/gt fr1,fr15 */
L8c037d7a: if(!(*sr&1u))goto L8c037d7e;goto L8c037d7c; /* bf 0x8c037d7e */
L8c037d7c: *stop=0x8c037d7cu;r[0]|=0x40u;goto L8c037d7e; /* or #0x40,r0 */
L8c037d7e: *stop=0x8c037d7eu;r[7]=r[0];goto L8c037d80; /* mov r0,r7 */
L8c037d80: *stop=0x8c037d80u;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c037d82; /* tst r7,r7 */
L8c037d82: if(!(*sr&1u))goto L8c037e00;goto L8c037d84; /* bf 0x8c037e00 */
L8c037d84: *stop=0x8c037d84u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c037d86; /* tst r6,r6 */
L8c037d86: if(!(*sr&1u))goto L8c037dc0;goto L8c037d88; /* bf 0x8c037dc0 */
L8c037d88: *stop=0x8c037d88u;r[0]=r[8];goto L8c037d8a; /* mov r8,r0 */
L8c037d8a: *stop=0x8c037d8au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037d8c; /* fmov.s @r0+,fr0 */
L8c037d8c: *stop=0x8c037d8cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037d8e; /* fmov.s @r0+,fr1 */
L8c037d8e: *stop=0x8c037d8eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037d90; /* fmov.s @r0+,fr2 */
L8c037d90: *stop=0x8c037d90u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037d92; /* fmov.s @r0+,fr3 */
L8c037d92: *stop=0x8c037d92u;if(!valid_ram(*gbr+0x48u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x48u,4);if(io_status)return io_status;goto L8c037d94; /* mov.l @(0x48,GBR),r0 */
L8c037d94: *stop=0x8c037d94u;*fpul=r[0];goto L8c037d96; /* lds r0,FPUL */
L8c037d96: *stop=0x8c037d96u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c037d98; /* fsts FPUL,fr4 */
L8c037d98: *stop=0x8c037d98u;if(!valid_access(r[1],2u))return -3;r[0]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;r[1]+=2;goto L8c037d9a; /* mov.w @r1+,r0 */
L8c037d9a: *stop=0x8c037d9au;*fpul=r[0];goto L8c037d9c; /* lds r0,FPUL */
L8c037d9c: *stop=0x8c037d9cu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][5]);if(status)return status;goto L8c037d9e; /* float FPUL,fr5 */
L8c037d9e: *stop=0x8c037d9eu;if(!valid_access(r[1],2u))return -3;r[0]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;r[1]+=2;goto L8c037da0; /* mov.w @r1+,r0 */
L8c037da0: *stop=0x8c037da0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[11],&f->fpscr,&fr[5]);if(status)return status;}goto L8c037da2; /* fmul fr11,fr5 */
L8c037da2: *stop=0x8c037da2u;*fpul=r[0];goto L8c037da4; /* lds r0,FPUL */
L8c037da4: *stop=0x8c037da4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][6]);if(status)return status;goto L8c037da6; /* float FPUL,fr6 */
L8c037da6: *stop=0x8c037da6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[11],&f->fpscr,&fr[6]);if(status)return status;}goto L8c037da8; /* fmul fr11,fr6 */
L8c037da8: *stop=0x8c037da8u;r[14]+=32u;goto L8c037daa; /* add #0x20,r14 */
L8c037daa: *stop=0x8c037daau;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c037dac; /* fmov.s fr4,@-r14 */
L8c037dac: *stop=0x8c037dacu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c037dae; /* fmov.s fr3,@-r14 */
L8c037dae: *stop=0x8c037daeu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c037db0; /* fmov.s fr6,@-r14 */
L8c037db0: *stop=0x8c037db0u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c037db2; /* fmov.s fr5,@-r14 */
L8c037db2: *stop=0x8c037db2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c037db4; /* fmov.s fr2,@-r14 */
L8c037db4: *stop=0x8c037db4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c037db6; /* fmov.s fr1,@-r14 */
L8c037db6: *stop=0x8c037db6u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c037db8; /* fmov.s fr0,@-r14 */
L8c037db8: *stop=0x8c037db8u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c037dba; /* mov.l r11,@-r14 */
L8c037dba: *stop=0x8c037dbau;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c037dbc; /* pref @r14 */
L8c037dbc: *stop=0x8c037dbcu;r[14]+=32u;goto L8c037dbe; /* add #0x20,r14 */
L8c037dbe: *stop=0x8c037dbeu;r[6]+=1u;goto L8c037dc0; /* add #0x1,r6 */
L8c037dc0: *stop=0x8c037dc0u;r[0]=r[9];goto L8c037dc2; /* mov r9,r0 */
L8c037dc2: *stop=0x8c037dc2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037dc4; /* fmov.s @r0+,fr0 */
L8c037dc4: *stop=0x8c037dc4u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037dc6; /* fmov.s @r0+,fr1 */
L8c037dc6: *stop=0x8c037dc6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037dc8; /* fmov.s @r0+,fr2 */
L8c037dc8: *stop=0x8c037dc8u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037dca; /* fmov.s @r0+,fr3 */
L8c037dca: *stop=0x8c037dcau;if(!valid_ram(*gbr+0x4cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x4cu,4);if(io_status)return io_status;goto L8c037dcc; /* mov.l @(0x4c,GBR),r0 */
L8c037dcc: *stop=0x8c037dccu;*fpul=r[0];goto L8c037dce; /* lds r0,FPUL */
L8c037dce: *stop=0x8c037dceu;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c037dd0; /* fsts FPUL,fr4 */
L8c037dd0: *stop=0x8c037dd0u;if(!valid_access(r[1],2u))return -3;r[0]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;r[1]+=2;goto L8c037dd2; /* mov.w @r1+,r0 */
L8c037dd2: *stop=0x8c037dd2u;*fpul=r[0];goto L8c037dd4; /* lds r0,FPUL */
L8c037dd4: *stop=0x8c037dd4u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][5]);if(status)return status;goto L8c037dd6; /* float FPUL,fr5 */
L8c037dd6: *stop=0x8c037dd6u;if(!valid_access(r[1],2u))return -3;r[0]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;r[1]+=2;goto L8c037dd8; /* mov.w @r1+,r0 */
L8c037dd8: *stop=0x8c037dd8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[11],&f->fpscr,&fr[5]);if(status)return status;}goto L8c037dda; /* fmul fr11,fr5 */
L8c037dda: *stop=0x8c037ddau;*fpul=r[0];goto L8c037ddc; /* lds r0,FPUL */
L8c037ddc: *stop=0x8c037ddcu;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][6]);if(status)return status;goto L8c037dde; /* float FPUL,fr6 */
L8c037dde: *stop=0x8c037ddeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[11],&f->fpscr,&fr[6]);if(status)return status;}goto L8c037de0; /* fmul fr11,fr6 */
L8c037de0: *stop=0x8c037de0u;r[14]+=32u;goto L8c037de2; /* add #0x20,r14 */
L8c037de2: *stop=0x8c037de2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c037de4; /* fmov.s fr4,@-r14 */
L8c037de4: *stop=0x8c037de4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c037de6; /* fmov.s fr3,@-r14 */
L8c037de6: *stop=0x8c037de6u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c037de8; /* fmov.s fr6,@-r14 */
L8c037de8: *stop=0x8c037de8u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c037dea; /* fmov.s fr5,@-r14 */
L8c037dea: *stop=0x8c037deau;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c037dec; /* fmov.s fr2,@-r14 */
L8c037dec: *stop=0x8c037decu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c037dee; /* fmov.s fr1,@-r14 */
L8c037dee: *stop=0x8c037deeu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c037df0; /* fmov.s fr0,@-r14 */
L8c037df0: *stop=0x8c037df0u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c037df2; /* mov.l r11,@-r14 */
L8c037df2: *stop=0x8c037df2u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c037df4; /* pref @r14 */
L8c037df4: *stop=0x8c037df4u;r[14]+=32u;goto L8c037df6; /* add #0x20,r14 */
L8c037df6: *stop=0x8c037df8u;r[6]+=1u;goto L8c037e60; /* bra 0x8c037e60 */
L8c037e00: *stop=0x8c037e00u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c037e02; /* tst r6,r6 */
L8c037e02: if((*sr&1u))goto L8c037e48;goto L8c037e04; /* bt 0x8c037e48 */
L8c037e04: *stop=0x8c037e04u;r[0]=r[9];goto L8c037e06; /* mov r9,r0 */
L8c037e06: *stop=0x8c037e06u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037e08; /* fmov.s @r0+,fr0 */
L8c037e08: *stop=0x8c037e08u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037e0a; /* fmov.s @r0+,fr1 */
L8c037e0a: *stop=0x8c037e0au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037e0c; /* fmov.s @r0+,fr2 */
L8c037e0c: *stop=0x8c037e0cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037e0e; /* fmov.s @r0+,fr3 */
L8c037e0e: *stop=0x8c037e0eu;if(!valid_ram(*gbr+0x4cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x4cu,4);if(io_status)return io_status;goto L8c037e10; /* mov.l @(0x4c,GBR),r0 */
L8c037e10: *stop=0x8c037e10u;*fpul=r[0];goto L8c037e12; /* lds r0,FPUL */
L8c037e12: *stop=0x8c037e12u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c037e14; /* fsts FPUL,fr4 */
L8c037e14: *stop=0x8c037e14u;if(!valid_access(r[1],2u))return -3;r[0]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;r[1]+=2;goto L8c037e16; /* mov.w @r1+,r0 */
L8c037e16: *stop=0x8c037e16u;*fpul=r[0];goto L8c037e18; /* lds r0,FPUL */
L8c037e18: *stop=0x8c037e18u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][5]);if(status)return status;goto L8c037e1a; /* float FPUL,fr5 */
L8c037e1a: *stop=0x8c037e1au;if(!valid_access(r[1],2u))return -3;r[0]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;r[1]+=2;goto L8c037e1c; /* mov.w @r1+,r0 */
L8c037e1c: *stop=0x8c037e1cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[11],&f->fpscr,&fr[5]);if(status)return status;}goto L8c037e1e; /* fmul fr11,fr5 */
L8c037e1e: *stop=0x8c037e1eu;*fpul=r[0];goto L8c037e20; /* lds r0,FPUL */
L8c037e20: *stop=0x8c037e20u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][6]);if(status)return status;goto L8c037e22; /* float FPUL,fr6 */
L8c037e22: *stop=0x8c037e22u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[11],&f->fpscr,&fr[6]);if(status)return status;}goto L8c037e24; /* fmul fr11,fr6 */
L8c037e24: *stop=0x8c037e24u;r[1]+=4294967292u;goto L8c037e26; /* add #-0x4,r1 */
L8c037e26: *stop=0x8c037e26u;r[6]=rd(m,bus,&io_status,0x8c037e44u,4);if(io_status)return io_status;goto L8c037e28; /* mov.l 0x8c037e44,r6 */
L8c037e28: *stop=0x8c037e28u;r[14]+=32u;goto L8c037e2a; /* add #0x20,r14 */
L8c037e2a: *stop=0x8c037e2au;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c037e2c; /* fmov.s fr4,@-r14 */
L8c037e2c: *stop=0x8c037e2cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c037e2e; /* fmov.s fr3,@-r14 */
L8c037e2e: *stop=0x8c037e2eu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c037e30; /* fmov.s fr6,@-r14 */
L8c037e30: *stop=0x8c037e30u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c037e32; /* fmov.s fr5,@-r14 */
L8c037e32: *stop=0x8c037e32u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c037e34; /* fmov.s fr2,@-r14 */
L8c037e34: *stop=0x8c037e34u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c037e36; /* fmov.s fr1,@-r14 */
L8c037e36: *stop=0x8c037e36u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c037e38; /* fmov.s fr0,@-r14 */
L8c037e38: *stop=0x8c037e38u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[6],4);if(io_status)return io_status;goto L8c037e3a; /* mov.l r6,@-r14 */
L8c037e3a: *stop=0x8c037e3au;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c037e3c; /* pref @r14 */
L8c037e3c: *stop=0x8c037e3cu;r[14]+=32u;goto L8c037e3e; /* add #0x20,r14 */
L8c037e3e: *stop=0x8c037e40u;r[6]=0u;goto L8c037e4a; /* bra 0x8c037e4a */
L8c037e48: *stop=0x8c037e48u;r[1]+=4u;goto L8c037e4a; /* add #0x4,r1 */
L8c037e4a: *stop=0x8c037e4au;r[0]=r[7];goto L8c037e4c; /* mov r7,r0 */
L8c037e4c: *stop=0x8c037e4cu;*sr=(*sr&~1u)|!(r[0]&0x11u);goto L8c037e4e; /* tst #0x11,r0 */
L8c037e4e: if((*sr&1u))goto L8c037e58;goto L8c037e50; /* bt 0x8c037e58 */
L8c037e50: *stop=0x8c037e50u;*sr=(*sr&~1u)|!(r[0]&0x22u);goto L8c037e52; /* tst #0x22,r0 */
L8c037e52: if((*sr&1u))goto L8c037e58;goto L8c037e54; /* bt 0x8c037e58 */
L8c037e54: *stop=0x8c037e54u;*sr=(*sr&~1u)|!(r[0]&0x44u);goto L8c037e56; /* tst #0x44,r0 */
L8c037e56: if(!(*sr&1u))goto L8c037e60;goto L8c037e58; /* bf 0x8c037e60 */
L8c037e58: *stop=0x8c037e58u;r[11]=r[1];goto L8c037e5a; /* mov r1,r11 */
L8c037e5a: target=0x8c037ee0u;*pr=0x8c037e5eu;*stop=0x8c037e5cu;r[11]+=4294967292u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037e5e; /* bsr 0x8c037ee0 */
L8c037e5e: *stop=0x8c037e5eu;r[11]=rd(m,bus,&io_status,0x8c037e7cu,4);if(io_status)return io_status;goto L8c037e60; /* mov.l 0x8c037e7c,r11 */
L8c037e60: *stop=0x8c037e60u;r[8]=r[9];goto L8c037e62; /* mov r9,r8 */
L8c037e62: *stop=0x8c037e62u;r[9]=r[10];goto L8c037e64; /* mov r10,r9 */
L8c037e64: *stop=0x8c037e64u;if(!valid_ram(*gbr+0x4cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x4cu,4);if(io_status)return io_status;goto L8c037e66; /* mov.l @(0x4c,GBR),r0 */
L8c037e66: *stop=0x8c037e66u;if(!valid_ram(*gbr+0x48u,4u))return -3;wr(m,bus,&io_status,*gbr+0x48u,r[0],4);if(io_status)return io_status;goto L8c037e68; /* mov.l r0,@(0x48,GBR) */
L8c037e68: *stop=0x8c037e68u;if(!valid_ram(*gbr+0x50u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x50u,4);if(io_status)return io_status;goto L8c037e6a; /* mov.l @(0x50,GBR),r0 */
L8c037e6a: *stop=0x8c037e6au;if(!valid_ram(*gbr+0x4cu,4u))return -3;wr(m,bus,&io_status,*gbr+0x4cu,r[0],4);if(io_status)return io_status;goto L8c037e6c; /* mov.l r0,@(0x4c,GBR) */
L8c037e6c: *stop=0x8c037e6cu;*sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1;goto L8c037e6e; /* shlr r7 */
L8c037e6e: *stop=0x8c037e6eu;r[0]=51u;goto L8c037e70; /* mov #0x33,r0 */
L8c037e70: *stop=0x8c037e70u;r[7]&=r[0];goto L8c037e72; /* and r0,r7 */
L8c037e72: *stop=0x8c037e72u;r[12]--;*sr=(*sr&~1u)|(!r[12]);goto L8c037e74; /* dt r12 */
L8c037e74: if((*sr&1u))goto L8c037e80;goto L8c037e76; /* bt 0x8c037e80 */
L8c037e76: *stop=0x8c037e78u;goto L8c037d5a; /* bra 0x8c037d5a */
L8c037e80: *stop=0x8c037e80u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c037e82; /* tst r6,r6 */
L8c037e82: if((*sr&1u))goto L8c037ec4;goto L8c037e84; /* bt 0x8c037ec4 */
L8c037e84: *stop=0x8c037e84u;r[0]=r[10];goto L8c037e86; /* mov r10,r0 */
L8c037e86: *stop=0x8c037e86u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037e88; /* fmov.s @r0+,fr0 */
L8c037e88: *stop=0x8c037e88u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037e8a; /* fmov.s @r0+,fr1 */
L8c037e8a: *stop=0x8c037e8au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037e8c; /* fmov.s @r0+,fr2 */
L8c037e8c: *stop=0x8c037e8cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037e8e; /* fmov.s @r0+,fr3 */
L8c037e8e: *stop=0x8c037e8eu;if(!valid_ram(*gbr+0x50u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x50u,4);if(io_status)return io_status;goto L8c037e90; /* mov.l @(0x50,GBR),r0 */
L8c037e90: *stop=0x8c037e90u;*fpul=r[0];goto L8c037e92; /* lds r0,FPUL */
L8c037e92: *stop=0x8c037e92u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c037e94; /* fsts FPUL,fr4 */
L8c037e94: *stop=0x8c037e94u;if(!valid_access(r[1],2u))return -3;r[0]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;r[1]+=2;goto L8c037e96; /* mov.w @r1+,r0 */
L8c037e96: *stop=0x8c037e96u;*fpul=r[0];goto L8c037e98; /* lds r0,FPUL */
L8c037e98: *stop=0x8c037e98u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][5]);if(status)return status;goto L8c037e9a; /* float FPUL,fr5 */
L8c037e9a: *stop=0x8c037e9au;if(!valid_access(r[1],2u))return -3;r[0]=sx(m,bus,&io_status,r[1],2);if(io_status)return io_status;r[1]+=2;goto L8c037e9c; /* mov.w @r1+,r0 */
L8c037e9c: *stop=0x8c037e9cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[11],&f->fpscr,&fr[5]);if(status)return status;}goto L8c037e9e; /* fmul fr11,fr5 */
L8c037e9e: *stop=0x8c037e9eu;*fpul=r[0];goto L8c037ea0; /* lds r0,FPUL */
L8c037ea0: *stop=0x8c037ea0u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][6]);if(status)return status;goto L8c037ea2; /* float FPUL,fr6 */
L8c037ea2: *stop=0x8c037ea2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[11],&f->fpscr,&fr[6]);if(status)return status;}goto L8c037ea4; /* fmul fr11,fr6 */
L8c037ea4: *stop=0x8c037ea4u;r[11]=rd(m,bus,&io_status,0x8c037ec0u,4);if(io_status)return io_status;goto L8c037ea6; /* mov.l 0x8c037ec0,r11 */
L8c037ea6: *stop=0x8c037ea6u;r[14]+=32u;goto L8c037ea8; /* add #0x20,r14 */
L8c037ea8: *stop=0x8c037ea8u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c037eaa; /* fmov.s fr4,@-r14 */
L8c037eaa: *stop=0x8c037eaau;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c037eac; /* fmov.s fr3,@-r14 */
L8c037eac: *stop=0x8c037eacu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c037eae; /* fmov.s fr6,@-r14 */
L8c037eae: *stop=0x8c037eaeu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c037eb0; /* fmov.s fr5,@-r14 */
L8c037eb0: *stop=0x8c037eb0u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c037eb2; /* fmov.s fr2,@-r14 */
L8c037eb2: *stop=0x8c037eb2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c037eb4; /* fmov.s fr1,@-r14 */
L8c037eb4: *stop=0x8c037eb4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c037eb6; /* fmov.s fr0,@-r14 */
L8c037eb6: *stop=0x8c037eb6u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c037eb8; /* mov.l r11,@-r14 */
L8c037eb8: *stop=0x8c037eb8u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c037eba; /* pref @r14 */
L8c037eba: *stop=0x8c037ebau;r[14]+=32u;goto L8c037ebc; /* add #0x20,r14 */
L8c037ebc: *stop=0x8c037ebeu;goto L8c037ec6; /* bra 0x8c037ec6 */
L8c037ec4: *stop=0x8c037ec4u;r[1]+=8u;goto L8c037ec6; /* add #0x8,r1 */
L8c037ec6: *stop=0x8c037ec6u;if(!valid_ram(*gbr+0x44u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x44u,4);if(io_status)return io_status;goto L8c037ec8; /* mov.l @(0x44,GBR),r0 */
L8c037ec8: *stop=0x8c037ec8u;r[0]--;*sr=(*sr&~1u)|(!r[0]);goto L8c037eca; /* dt r0 */
L8c037eca: *stop=0x8c037ecau;if(!valid_ram(*gbr+0x44u,4u))return -3;wr(m,bus,&io_status,*gbr+0x44u,r[0],4);if(io_status)return io_status;goto L8c037ecc; /* mov.l r0,@(0x44,GBR) */
L8c037ecc: if((*sr&1u))goto L8c037ed2;goto L8c037ece; /* bt 0x8c037ed2 */
L8c037ece: *stop=0x8c037ed0u;goto L8c037cfa; /* bra 0x8c037cfa */
L8c037ed2: *stop=0x8c037ed2u;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c037ed4; /* mov.l @r15+,r3 */
L8c037ed4: *stop=0x8c037ed4u;if(!valid_access(r[15],4u))return -3;r[4]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c037ed6; /* mov.l @r15+,r4 */
L8c037ed6: *stop=0x8c037ed6u;if(!valid_access(r[15],4u))return -3;r[7]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c037ed8; /* mov.l @r15+,r7 */
L8c037ed8: *stop=0x8c037ed8u;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c037eda; /* mov.l @r15+,r13 */
L8c037eda: *stop=0x8c037edau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c037edc; /* lds.l @r15+,PR */
L8c037edc: *stop=0x8c037edcu;if(*pr!=entry_pr)return -3;*stop=0x8c037edeu;r[0]=r[14];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_primitive_stream_8c038668(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c038668;
L8c038668: *stop=0x8c038668u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c03866a; /* sts.l PR,@-r15 */
L8c03866a: *stop=0x8c03866au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c03866c; /* mov.l r13,@-r15 */
L8c03866c: *stop=0x8c03866cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c03866e; /* mov.l r7,@-r15 */
L8c03866e: *stop=0x8c03866eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c038670; /* mov.l r4,@-r15 */
L8c038670: *stop=0x8c038670u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c038672; /* mov.l r3,@-r15 */
L8c038672: *stop=0x8c038672u;if(!valid_access(r[2]+4u,4u))return -3;r[4]=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;goto L8c038674; /* mov.l @(0x4,r2),r4 */
L8c038674: *stop=0x8c038674u;if(!valid_ram(*gbr+0x3cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x3cu,4);if(io_status)return io_status;goto L8c038676; /* mov.l @(0x3c,GBR),r0 */
L8c038676: *stop=0x8c038676u;r[5]=r[0];goto L8c038678; /* mov r0,r5 */
L8c038678: *stop=0x8c038678u;if(!valid_access(r[2]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c03867a; /* mov.w @(0x2,r2),r0 */
L8c03867a: *stop=0x8c03867au;if(!valid_ram(*gbr+0x44u,4u))return -3;wr(m,bus,&io_status,*gbr+0x44u,r[0],4);if(io_status)return io_status;goto L8c03867c; /* mov.l r0,@(0x44,GBR) */
L8c03867c: *stop=0x8c03867cu;if(!valid_ram(*gbr+0x38u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x38u,4);if(io_status)return io_status;goto L8c03867e; /* mov.l @(0x38,GBR),r0 */
L8c03867e: *stop=0x8c03867eu;r[3]=r[0];goto L8c038680; /* mov r0,r3 */
L8c038680: *stop=0x8c038680u;if(!valid_ram(*gbr+0x54u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x54u,4);if(io_status)return io_status;goto L8c038682; /* mov.l @(0x54,GBR),r0 */
L8c038682: *stop=0x8c038682u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c038684; /* tst r0,r0 */
L8c038684: if((*sr&1u))goto L8c03868a;goto L8c038686; /* bt 0x8c03868a */
L8c038686: *stop=0x8c038688u;r[12]=r[0];goto L8c038694; /* bra 0x8c038694 */
L8c03868a: *stop=0x8c03868au;if(!valid_access(r[4],2u))return -3;r[12]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c03868c; /* mov.w @r4+,r12 */
L8c03868c: *stop=0x8c03868cu;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c03868e; /* cmp/pz r12 */
L8c03868e: if((*sr&1u))goto L8c038694;goto L8c038690; /* bt 0x8c038694 */
L8c038690: *stop=0x8c038690u;r[0]=sx(m,bus,&io_status,0x8c038754u,2);if(io_status)return io_status;goto L8c038692; /* mov.w 0x8c038754,r0 */
L8c038692: *stop=0x8c038692u;r[12]&=r[0];goto L8c038694; /* and r0,r12 */
L8c038694: *stop=0x8c038694u;r[12]+=4294967294u;goto L8c038696; /* add #-0x2,r12 */
L8c038696: *stop=0x8c038696u;r[11]=rd(m,bus,&io_status,0x8c038758u,4);if(io_status)return io_status;goto L8c038698; /* mov.l 0x8c038758,r11 */
L8c038698: *stop=0x8c038698u;r[6]=0u;goto L8c03869a; /* mov #0x0,r6 */
L8c03869a: *stop=0x8c03869au;if(!valid_access(r[4],2u))return -3;r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c03869c; /* mov.w @r4+,r0 */
L8c03869c: *stop=0x8c03869cu;r[0]<<=2;goto L8c03869e; /* shll2 r0 */
L8c03869e: *stop=0x8c03869eu;r[8]=r[0];goto L8c0386a0; /* mov r0,r8 */
L8c0386a0: *stop=0x8c0386a0u;if(!valid_access(r[0]+r[3],4u))return -3;r[0]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c0386a2; /* mov.l @(r0,r3),r0 */
L8c0386a2: *stop=0x8c0386a2u;if(!valid_ram(*gbr+0x48u,4u))return -3;wr(m,bus,&io_status,*gbr+0x48u,r[0],4);if(io_status)return io_status;goto L8c0386a4; /* mov.l r0,@(0x48,GBR) */
L8c0386a4: *stop=0x8c0386a4u;r[8]<<=2;goto L8c0386a6; /* shll2 r8 */
L8c0386a6: *stop=0x8c0386a6u;r[8]+=r[5];goto L8c0386a8; /* add r5,r8 */
L8c0386a8: *stop=0x8c0386a8u;if(!valid_access(r[4],2u))return -3;r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c0386aa; /* mov.w @r4+,r0 */
L8c0386aa: *stop=0x8c0386aau;r[0]<<=2;goto L8c0386ac; /* shll2 r0 */
L8c0386ac: *stop=0x8c0386acu;r[9]=r[0];goto L8c0386ae; /* mov r0,r9 */
L8c0386ae: *stop=0x8c0386aeu;if(!valid_access(r[0]+r[3],4u))return -3;r[0]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c0386b0; /* mov.l @(r0,r3),r0 */
L8c0386b0: *stop=0x8c0386b0u;if(!valid_ram(*gbr+0x4cu,4u))return -3;wr(m,bus,&io_status,*gbr+0x4cu,r[0],4);if(io_status)return io_status;goto L8c0386b2; /* mov.l r0,@(0x4c,GBR) */
L8c0386b2: *stop=0x8c0386b2u;r[9]<<=2;goto L8c0386b4; /* shll2 r9 */
L8c0386b4: *stop=0x8c0386b4u;r[9]+=r[5];goto L8c0386b6; /* add r5,r9 */
L8c0386b6: *stop=0x8c0386b6u;r[0]=8u;goto L8c0386b8; /* mov #0x8,r0 */
L8c0386b8: *stop=0x8c0386b8u;if(!valid_access(r[0]+r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]+r[8]);if(io_status)return io_status;goto L8c0386ba; /* fmov.s @(r0,r8),fr9 */
L8c0386ba: *stop=0x8c0386bau;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]+r[9]);if(io_status)return io_status;goto L8c0386bc; /* fmov.s @(r0,r9),fr12 */
L8c0386bc: *stop=0x8c0386bcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0;goto L8c0386be; /* fldi0 fr0 */
L8c0386be: *stop=0x8c0386beu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0x3f800000u;goto L8c0386c0; /* fldi1 fr1 */
L8c0386c0: *stop=0x8c0386c0u;r[0]=0u;goto L8c0386c2; /* mov #0x0,r0 */
L8c0386c2: *stop=0x8c0386c2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][9],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c0386c4; /* fcmp/gt fr0,fr9 */
L8c0386c4: if((*sr&1u))goto L8c0386c8;goto L8c0386c6; /* bt 0x8c0386c8 */
L8c0386c6: *stop=0x8c0386c6u;r[0]|=0x1u;goto L8c0386c8; /* or #0x1,r0 */
L8c0386c8: *stop=0x8c0386c8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][9],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c0386ca; /* fcmp/gt fr1,fr9 */
L8c0386ca: if(!(*sr&1u))goto L8c0386ce;goto L8c0386cc; /* bf 0x8c0386ce */
L8c0386cc: *stop=0x8c0386ccu;r[0]|=0x10u;goto L8c0386ce; /* or #0x10,r0 */
L8c0386ce: *stop=0x8c0386ceu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c0386d0; /* fcmp/gt fr0,fr12 */
L8c0386d0: if((*sr&1u))goto L8c0386d4;goto L8c0386d2; /* bt 0x8c0386d4 */
L8c0386d2: *stop=0x8c0386d2u;r[0]|=0x2u;goto L8c0386d4; /* or #0x2,r0 */
L8c0386d4: *stop=0x8c0386d4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c0386d6; /* fcmp/gt fr1,fr12 */
L8c0386d6: if(!(*sr&1u))goto L8c0386da;goto L8c0386d8; /* bf 0x8c0386da */
L8c0386d8: *stop=0x8c0386d8u;r[0]|=0x20u;goto L8c0386da; /* or #0x20,r0 */
L8c0386da: *stop=0x8c0386dau;r[7]=r[0];goto L8c0386dc; /* mov r0,r7 */
L8c0386dc: *stop=0x8c0386dcu;if(!valid_access(r[4],2u))return -3;r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c0386de; /* mov.w @r4+,r0 */
L8c0386de: *stop=0x8c0386deu;r[0]<<=2;goto L8c0386e0; /* shll2 r0 */
L8c0386e0: *stop=0x8c0386e0u;r[10]=r[0];goto L8c0386e2; /* mov r0,r10 */
L8c0386e2: *stop=0x8c0386e2u;if(!valid_access(r[0]+r[3],4u))return -3;r[0]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c0386e4; /* mov.l @(r0,r3),r0 */
L8c0386e4: *stop=0x8c0386e4u;if(!valid_ram(*gbr+0x50u,4u))return -3;wr(m,bus,&io_status,*gbr+0x50u,r[0],4);if(io_status)return io_status;goto L8c0386e6; /* mov.l r0,@(0x50,GBR) */
L8c0386e6: *stop=0x8c0386e6u;r[10]<<=2;goto L8c0386e8; /* shll2 r10 */
L8c0386e8: *stop=0x8c0386e8u;r[10]+=r[5];goto L8c0386ea; /* add r5,r10 */
L8c0386ea: *stop=0x8c0386eau;r[0]=8u;goto L8c0386ec; /* mov #0x8,r0 */
L8c0386ec: *stop=0x8c0386ecu;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[10]);if(io_status)return io_status;goto L8c0386ee; /* fmov.s @(r0,r10),fr15 */
L8c0386ee: *stop=0x8c0386eeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0;goto L8c0386f0; /* fldi0 fr0 */
L8c0386f0: *stop=0x8c0386f0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0x3f800000u;goto L8c0386f2; /* fldi1 fr1 */
L8c0386f2: *stop=0x8c0386f2u;r[0]=r[7];goto L8c0386f4; /* mov r7,r0 */
L8c0386f4: *stop=0x8c0386f4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c0386f6; /* fcmp/gt fr0,fr15 */
L8c0386f6: if((*sr&1u))goto L8c0386fa;goto L8c0386f8; /* bt 0x8c0386fa */
L8c0386f8: *stop=0x8c0386f8u;r[0]|=0x4u;goto L8c0386fa; /* or #0x4,r0 */
L8c0386fa: *stop=0x8c0386fau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c0386fc; /* fcmp/gt fr1,fr15 */
L8c0386fc: if(!(*sr&1u))goto L8c038700;goto L8c0386fe; /* bf 0x8c038700 */
L8c0386fe: *stop=0x8c0386feu;r[0]|=0x40u;goto L8c038700; /* or #0x40,r0 */
L8c038700: *stop=0x8c038700u;r[7]=r[0];goto L8c038702; /* mov r0,r7 */
L8c038702: *stop=0x8c038702u;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c038704; /* tst r7,r7 */
L8c038704: if(!(*sr&1u))goto L8c03875c;goto L8c038706; /* bf 0x8c03875c */
L8c038706: *stop=0x8c038706u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c038708; /* tst r6,r6 */
L8c038708: if(!(*sr&1u))goto L8c03872e;goto L8c03870a; /* bf 0x8c03872e */
L8c03870a: *stop=0x8c03870au;r[0]=r[8];goto L8c03870c; /* mov r8,r0 */
L8c03870c: *stop=0x8c03870cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03870e; /* fmov.s @r0+,fr0 */
L8c03870e: *stop=0x8c03870eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038710; /* fmov.s @r0+,fr1 */
L8c038710: *stop=0x8c038710u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038712; /* fmov.s @r0+,fr2 */
L8c038712: *stop=0x8c038712u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038714; /* fmov.s @r0+,fr3 */
L8c038714: *stop=0x8c038714u;if(!valid_ram(*gbr+0x48u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x48u,4);if(io_status)return io_status;goto L8c038716; /* mov.l @(0x48,GBR),r0 */
L8c038716: *stop=0x8c038716u;*fpul=r[0];goto L8c038718; /* lds r0,FPUL */
L8c038718: *stop=0x8c038718u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c03871a; /* fsts FPUL,fr4 */
L8c03871a: *stop=0x8c03871au;r[14]+=28u;goto L8c03871c; /* add #0x1c,r14 */
L8c03871c: *stop=0x8c03871cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c03871e; /* fmov.s fr3,@-r14 */
L8c03871e: *stop=0x8c03871eu;r[14]+=4294967288u;goto L8c038720; /* add #-0x8,r14 */
L8c038720: *stop=0x8c038720u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c038722; /* fmov.s fr2,@-r14 */
L8c038722: *stop=0x8c038722u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c038724; /* fmov.s fr1,@-r14 */
L8c038724: *stop=0x8c038724u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c038726; /* fmov.s fr0,@-r14 */
L8c038726: *stop=0x8c038726u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c038728; /* mov.l r11,@-r14 */
L8c038728: *stop=0x8c038728u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c03872a; /* pref @r14 */
L8c03872a: *stop=0x8c03872au;r[14]+=32u;goto L8c03872c; /* add #0x20,r14 */
L8c03872c: *stop=0x8c03872cu;r[6]+=1u;goto L8c03872e; /* add #0x1,r6 */
L8c03872e: *stop=0x8c03872eu;r[0]=r[9];goto L8c038730; /* mov r9,r0 */
L8c038730: *stop=0x8c038730u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038732; /* fmov.s @r0+,fr0 */
L8c038732: *stop=0x8c038732u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038734; /* fmov.s @r0+,fr1 */
L8c038734: *stop=0x8c038734u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038736; /* fmov.s @r0+,fr2 */
L8c038736: *stop=0x8c038736u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038738; /* fmov.s @r0+,fr3 */
L8c038738: *stop=0x8c038738u;if(!valid_ram(*gbr+0x4cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x4cu,4);if(io_status)return io_status;goto L8c03873a; /* mov.l @(0x4c,GBR),r0 */
L8c03873a: *stop=0x8c03873au;*fpul=r[0];goto L8c03873c; /* lds r0,FPUL */
L8c03873c: *stop=0x8c03873cu;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c03873e; /* fsts FPUL,fr4 */
L8c03873e: *stop=0x8c03873eu;r[14]+=28u;goto L8c038740; /* add #0x1c,r14 */
L8c038740: *stop=0x8c038740u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c038742; /* fmov.s fr3,@-r14 */
L8c038742: *stop=0x8c038742u;r[14]+=4294967288u;goto L8c038744; /* add #-0x8,r14 */
L8c038744: *stop=0x8c038744u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c038746; /* fmov.s fr2,@-r14 */
L8c038746: *stop=0x8c038746u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c038748; /* fmov.s fr1,@-r14 */
L8c038748: *stop=0x8c038748u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c03874a; /* fmov.s fr0,@-r14 */
L8c03874a: *stop=0x8c03874au;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c03874c; /* mov.l r11,@-r14 */
L8c03874c: *stop=0x8c03874cu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c03874e; /* pref @r14 */
L8c03874e: *stop=0x8c03874eu;r[14]+=32u;goto L8c038750; /* add #0x20,r14 */
L8c038750: *stop=0x8c038752u;r[6]+=1u;goto L8c03879e; /* bra 0x8c03879e */
L8c03875c: *stop=0x8c03875cu;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c03875e; /* tst r6,r6 */
L8c03875e: if((*sr&1u))goto L8c03878c;goto L8c038760; /* bt 0x8c03878c */
L8c038760: *stop=0x8c038760u;r[0]=r[9];goto L8c038762; /* mov r9,r0 */
L8c038762: *stop=0x8c038762u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038764; /* fmov.s @r0+,fr0 */
L8c038764: *stop=0x8c038764u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038766; /* fmov.s @r0+,fr1 */
L8c038766: *stop=0x8c038766u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038768; /* fmov.s @r0+,fr2 */
L8c038768: *stop=0x8c038768u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03876a; /* fmov.s @r0+,fr3 */
L8c03876a: *stop=0x8c03876au;if(!valid_ram(*gbr+0x4cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x4cu,4);if(io_status)return io_status;goto L8c03876c; /* mov.l @(0x4c,GBR),r0 */
L8c03876c: *stop=0x8c03876cu;*fpul=r[0];goto L8c03876e; /* lds r0,FPUL */
L8c03876e: *stop=0x8c03876eu;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c038770; /* fsts FPUL,fr4 */
L8c038770: *stop=0x8c038770u;r[6]=rd(m,bus,&io_status,0x8c038788u,4);if(io_status)return io_status;goto L8c038772; /* mov.l 0x8c038788,r6 */
L8c038772: *stop=0x8c038772u;r[14]+=28u;goto L8c038774; /* add #0x1c,r14 */
L8c038774: *stop=0x8c038774u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c038776; /* fmov.s fr3,@-r14 */
L8c038776: *stop=0x8c038776u;r[14]+=4294967288u;goto L8c038778; /* add #-0x8,r14 */
L8c038778: *stop=0x8c038778u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c03877a; /* fmov.s fr2,@-r14 */
L8c03877a: *stop=0x8c03877au;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c03877c; /* fmov.s fr1,@-r14 */
L8c03877c: *stop=0x8c03877cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c03877e; /* fmov.s fr0,@-r14 */
L8c03877e: *stop=0x8c03877eu;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[6],4);if(io_status)return io_status;goto L8c038780; /* mov.l r6,@-r14 */
L8c038780: *stop=0x8c038780u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c038782; /* pref @r14 */
L8c038782: *stop=0x8c038782u;r[14]+=32u;goto L8c038784; /* add #0x20,r14 */
L8c038784: *stop=0x8c038786u;r[6]=0u;goto L8c03878c; /* bra 0x8c03878c */
L8c03878c: *stop=0x8c03878cu;r[0]=r[7];goto L8c03878e; /* mov r7,r0 */
L8c03878e: *stop=0x8c03878eu;*sr=(*sr&~1u)|!(r[0]&0x11u);goto L8c038790; /* tst #0x11,r0 */
L8c038790: if((*sr&1u))goto L8c03879a;goto L8c038792; /* bt 0x8c03879a */
L8c038792: *stop=0x8c038792u;*sr=(*sr&~1u)|!(r[0]&0x22u);goto L8c038794; /* tst #0x22,r0 */
L8c038794: if((*sr&1u))goto L8c03879a;goto L8c038796; /* bt 0x8c03879a */
L8c038796: *stop=0x8c038796u;*sr=(*sr&~1u)|!(r[0]&0x44u);goto L8c038798; /* tst #0x44,r0 */
L8c038798: if(!(*sr&1u))goto L8c03879e;goto L8c03879a; /* bf 0x8c03879e */
L8c03879a: target=0x8c038802u;*pr=0x8c03879eu;*stop=0x8c03879cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c03879e; /* bsr 0x8c038802 */
L8c03879e: *stop=0x8c03879eu;r[8]=r[9];goto L8c0387a0; /* mov r9,r8 */
L8c0387a0: *stop=0x8c0387a0u;r[9]=r[10];goto L8c0387a2; /* mov r10,r9 */
L8c0387a2: *stop=0x8c0387a2u;if(!valid_ram(*gbr+0x4cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x4cu,4);if(io_status)return io_status;goto L8c0387a4; /* mov.l @(0x4c,GBR),r0 */
L8c0387a4: *stop=0x8c0387a4u;if(!valid_ram(*gbr+0x48u,4u))return -3;wr(m,bus,&io_status,*gbr+0x48u,r[0],4);if(io_status)return io_status;goto L8c0387a6; /* mov.l r0,@(0x48,GBR) */
L8c0387a6: *stop=0x8c0387a6u;if(!valid_ram(*gbr+0x50u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x50u,4);if(io_status)return io_status;goto L8c0387a8; /* mov.l @(0x50,GBR),r0 */
L8c0387a8: *stop=0x8c0387a8u;if(!valid_ram(*gbr+0x4cu,4u))return -3;wr(m,bus,&io_status,*gbr+0x4cu,r[0],4);if(io_status)return io_status;goto L8c0387aa; /* mov.l r0,@(0x4c,GBR) */
L8c0387aa: *stop=0x8c0387aau;*sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1;goto L8c0387ac; /* shlr r7 */
L8c0387ac: *stop=0x8c0387acu;r[0]=51u;goto L8c0387ae; /* mov #0x33,r0 */
L8c0387ae: *stop=0x8c0387aeu;r[7]&=r[0];goto L8c0387b0; /* and r0,r7 */
L8c0387b0: *stop=0x8c0387b0u;r[12]+=4294967295u;goto L8c0387b2; /* add #-0x1,r12 */
L8c0387b2: *stop=0x8c0387b2u;*sr=(*sr&~1u)|((int32_t)r[12]>0);goto L8c0387b4; /* cmp/pl r12 */
L8c0387b4: if((*sr&1u))goto L8c0386dc;goto L8c0387b6; /* bt 0x8c0386dc */
L8c0387b6: *stop=0x8c0387b6u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c0387b8; /* tst r6,r6 */
L8c0387b8: if((*sr&1u))goto L8c0387e8;goto L8c0387ba; /* bt 0x8c0387e8 */
L8c0387ba: *stop=0x8c0387bau;r[0]=r[10];goto L8c0387bc; /* mov r10,r0 */
L8c0387bc: *stop=0x8c0387bcu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0387be; /* fmov.s @r0+,fr0 */
L8c0387be: *stop=0x8c0387beu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0387c0; /* fmov.s @r0+,fr1 */
L8c0387c0: *stop=0x8c0387c0u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0387c2; /* fmov.s @r0+,fr2 */
L8c0387c2: *stop=0x8c0387c2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0387c4; /* fmov.s @r0+,fr3 */
L8c0387c4: *stop=0x8c0387c4u;if(!valid_ram(*gbr+0x50u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x50u,4);if(io_status)return io_status;goto L8c0387c6; /* mov.l @(0x50,GBR),r0 */
L8c0387c6: *stop=0x8c0387c6u;*fpul=r[0];goto L8c0387c8; /* lds r0,FPUL */
L8c0387c8: *stop=0x8c0387c8u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c0387ca; /* fsts FPUL,fr4 */
L8c0387ca: *stop=0x8c0387cau;r[11]=rd(m,bus,&io_status,0x8c0387e4u,4);if(io_status)return io_status;goto L8c0387cc; /* mov.l 0x8c0387e4,r11 */
L8c0387cc: *stop=0x8c0387ccu;r[14]+=28u;goto L8c0387ce; /* add #0x1c,r14 */
L8c0387ce: *stop=0x8c0387ceu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c0387d0; /* fmov.s fr3,@-r14 */
L8c0387d0: *stop=0x8c0387d0u;r[14]+=4294967288u;goto L8c0387d2; /* add #-0x8,r14 */
L8c0387d2: *stop=0x8c0387d2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c0387d4; /* fmov.s fr2,@-r14 */
L8c0387d4: *stop=0x8c0387d4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c0387d6; /* fmov.s fr1,@-r14 */
L8c0387d6: *stop=0x8c0387d6u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c0387d8; /* fmov.s fr0,@-r14 */
L8c0387d8: *stop=0x8c0387d8u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c0387da; /* mov.l r11,@-r14 */
L8c0387da: *stop=0x8c0387dau;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c0387dc; /* pref @r14 */
L8c0387dc: *stop=0x8c0387dcu;r[14]+=32u;goto L8c0387de; /* add #0x20,r14 */
L8c0387de: *stop=0x8c0387e0u;goto L8c0387e8; /* bra 0x8c0387e8 */
L8c0387e8: *stop=0x8c0387e8u;if(!valid_ram(*gbr+0x44u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x44u,4);if(io_status)return io_status;goto L8c0387ea; /* mov.l @(0x44,GBR),r0 */
L8c0387ea: *stop=0x8c0387eau;r[0]--;*sr=(*sr&~1u)|(!r[0]);goto L8c0387ec; /* dt r0 */
L8c0387ec: *stop=0x8c0387ecu;if(!valid_ram(*gbr+0x44u,4u))return -3;wr(m,bus,&io_status,*gbr+0x44u,r[0],4);if(io_status)return io_status;goto L8c0387ee; /* mov.l r0,@(0x44,GBR) */
L8c0387ee: if((*sr&1u))goto L8c0387f4;goto L8c0387f0; /* bt 0x8c0387f4 */
L8c0387f0: *stop=0x8c0387f2u;goto L8c038680; /* bra 0x8c038680 */
L8c0387f4: *stop=0x8c0387f4u;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c0387f6; /* mov.l @r15+,r3 */
L8c0387f6: *stop=0x8c0387f6u;if(!valid_access(r[15],4u))return -3;r[4]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c0387f8; /* mov.l @r15+,r4 */
L8c0387f8: *stop=0x8c0387f8u;if(!valid_access(r[15],4u))return -3;r[7]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c0387fa; /* mov.l @r15+,r7 */
L8c0387fa: *stop=0x8c0387fau;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c0387fc; /* mov.l @r15+,r13 */
L8c0387fc: *stop=0x8c0387fcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0387fe; /* lds.l @r15+,PR */
L8c0387fe: *stop=0x8c0387feu;if(*pr!=entry_pr)return -3;*stop=0x8c038800u;r[0]=r[14];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_primitive_stream_8c0382ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0382ac;
L8c0382ac: *stop=0x8c0382acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0382ae; /* sts.l PR,@-r15 */
L8c0382ae: *stop=0x8c0382aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0382b0; /* mov.l r13,@-r15 */
L8c0382b0: *stop=0x8c0382b0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c0382b2; /* mov.l r7,@-r15 */
L8c0382b2: *stop=0x8c0382b2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c0382b4; /* mov.l r4,@-r15 */
L8c0382b4: *stop=0x8c0382b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0382b6; /* mov.l r3,@-r15 */
L8c0382b6: *stop=0x8c0382b6u;if(!valid_ram(*gbr+0x3cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x3cu,4);if(io_status)return io_status;goto L8c0382b8; /* mov.l @(0x3c,GBR),r0 */
L8c0382b8: *stop=0x8c0382b8u;r[5]=r[0];goto L8c0382ba; /* mov r0,r5 */
L8c0382ba: *stop=0x8c0382bau;if(!valid_access(r[4]+4u,4u))return -3;r[1]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c0382bc; /* mov.l @(0x4,r4),r1 */
L8c0382bc: *stop=0x8c0382bcu;if(!valid_access(r[2]+4u,4u))return -3;r[4]=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;goto L8c0382be; /* mov.l @(0x4,r2),r4 */
L8c0382be: *stop=0x8c0382beu;if(!valid_access(r[2]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c0382c0; /* mov.w @(0x2,r2),r0 */
L8c0382c0: *stop=0x8c0382c0u;if(!valid_ram(*gbr+0x44u,4u))return -3;wr(m,bus,&io_status,*gbr+0x44u,r[0],4);if(io_status)return io_status;goto L8c0382c2; /* mov.l r0,@(0x44,GBR) */
L8c0382c2: *stop=0x8c0382c2u;r[13]=rd(m,bus,&io_status,0x8c0382d8u,4);if(io_status)return io_status;goto L8c0382c4; /* mov.l 0x8c0382d8,r13 */
L8c0382c4: *stop=0x8c0382c4u;r[0]=rd(m,bus,&io_status,0x8c0382dcu,4);if(io_status)return io_status;goto L8c0382c6; /* mov.l 0x8c0382dc,r0 */
L8c0382c6: *stop=0x8c0382c6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[0]);if(io_status)return io_status;goto L8c0382c8; /* fmov.s @r0,fr11 */
L8c0382c8: *stop=0x8c0382c8u;if(!valid_ram(*gbr+0x38u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x38u,4);if(io_status)return io_status;goto L8c0382ca; /* mov.l @(0x38,GBR),r0 */
L8c0382ca: *stop=0x8c0382cau;r[3]=r[0];goto L8c0382cc; /* mov r0,r3 */
L8c0382cc: *stop=0x8c0382ccu;if(!valid_ram(*gbr+0x54u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x54u,4);if(io_status)return io_status;goto L8c0382ce; /* mov.l @(0x54,GBR),r0 */
L8c0382ce: *stop=0x8c0382ceu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0382d0; /* tst r0,r0 */
L8c0382d0: if((*sr&1u))goto L8c0382e0;goto L8c0382d2; /* bt 0x8c0382e0 */
L8c0382d2: *stop=0x8c0382d4u;r[12]=r[0];goto L8c0382ea; /* bra 0x8c0382ea */
L8c0382e0: *stop=0x8c0382e0u;if(!valid_access(r[4],2u))return -3;r[12]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c0382e2; /* mov.w @r4+,r12 */
L8c0382e2: *stop=0x8c0382e2u;*sr=(*sr&~1u)|((int32_t)r[12]>=0);goto L8c0382e4; /* cmp/pz r12 */
L8c0382e4: if((*sr&1u))goto L8c0382ea;goto L8c0382e6; /* bt 0x8c0382ea */
L8c0382e6: *stop=0x8c0382e6u;r[0]=sx(m,bus,&io_status,0x8c038406u,2);if(io_status)return io_status;goto L8c0382e8; /* mov.w 0x8c038406,r0 */
L8c0382e8: *stop=0x8c0382e8u;r[12]&=r[0];goto L8c0382ea; /* and r0,r12 */
L8c0382ea: *stop=0x8c0382eau;r[12]+=4294967294u;goto L8c0382ec; /* add #-0x2,r12 */
L8c0382ec: *stop=0x8c0382ecu;r[11]=rd(m,bus,&io_status,0x8c038408u,4);if(io_status)return io_status;goto L8c0382ee; /* mov.l 0x8c038408,r11 */
L8c0382ee: *stop=0x8c0382eeu;r[6]=0u;goto L8c0382f0; /* mov #0x0,r6 */
L8c0382f0: *stop=0x8c0382f0u;if(!valid_access(r[4],2u))return -3;r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c0382f2; /* mov.w @r4+,r0 */
L8c0382f2: *stop=0x8c0382f2u;r[0]<<=2;goto L8c0382f4; /* shll2 r0 */
L8c0382f4: *stop=0x8c0382f4u;r[8]=r[0];goto L8c0382f6; /* mov r0,r8 */
L8c0382f6: *stop=0x8c0382f6u;if(!valid_access(r[0]+r[3],4u))return -3;r[0]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c0382f8; /* mov.l @(r0,r3),r0 */
L8c0382f8: *stop=0x8c0382f8u;if(!valid_ram(*gbr+0x48u,4u))return -3;wr(m,bus,&io_status,*gbr+0x48u,r[0],4);if(io_status)return io_status;goto L8c0382fa; /* mov.l r0,@(0x48,GBR) */
L8c0382fa: *stop=0x8c0382fau;r[0]=r[8];goto L8c0382fc; /* mov r8,r0 */
L8c0382fc: *stop=0x8c0382fcu;r[0]+=r[8];goto L8c0382fe; /* add r8,r0 */
L8c0382fe: *stop=0x8c0382feu;r[0]+=r[8];goto L8c038300; /* add r8,r0 */
L8c038300: *stop=0x8c038300u;r[8]<<=2;goto L8c038302; /* shll2 r8 */
L8c038302: *stop=0x8c038302u;r[8]+=r[5];goto L8c038304; /* add r5,r8 */
L8c038304: *stop=0x8c038304u;if(!valid_access(r[13]+0u,2u))return -3;wr(m,bus,&io_status,r[13]+0x0u,r[0],2);if(io_status)return io_status;goto L8c038306; /* mov.w r0,@(0x0,r13) */
L8c038306: *stop=0x8c038306u;if(!valid_access(r[4],2u))return -3;r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c038308; /* mov.w @r4+,r0 */
L8c038308: *stop=0x8c038308u;r[0]<<=2;goto L8c03830a; /* shll2 r0 */
L8c03830a: *stop=0x8c03830au;r[9]=r[0];goto L8c03830c; /* mov r0,r9 */
L8c03830c: *stop=0x8c03830cu;if(!valid_access(r[0]+r[3],4u))return -3;r[0]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c03830e; /* mov.l @(r0,r3),r0 */
L8c03830e: *stop=0x8c03830eu;if(!valid_ram(*gbr+0x4cu,4u))return -3;wr(m,bus,&io_status,*gbr+0x4cu,r[0],4);if(io_status)return io_status;goto L8c038310; /* mov.l r0,@(0x4c,GBR) */
L8c038310: *stop=0x8c038310u;r[0]=r[9];goto L8c038312; /* mov r9,r0 */
L8c038312: *stop=0x8c038312u;r[0]+=r[9];goto L8c038314; /* add r9,r0 */
L8c038314: *stop=0x8c038314u;r[0]+=r[9];goto L8c038316; /* add r9,r0 */
L8c038316: *stop=0x8c038316u;r[9]<<=2;goto L8c038318; /* shll2 r9 */
L8c038318: *stop=0x8c038318u;r[9]+=r[5];goto L8c03831a; /* add r5,r9 */
L8c03831a: *stop=0x8c03831au;if(!valid_access(r[13]+2u,2u))return -3;wr(m,bus,&io_status,r[13]+0x2u,r[0],2);if(io_status)return io_status;goto L8c03831c; /* mov.w r0,@(0x2,r13) */
L8c03831c: *stop=0x8c03831cu;r[0]=8u;goto L8c03831e; /* mov #0x8,r0 */
L8c03831e: *stop=0x8c03831eu;if(!valid_access(r[0]+r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]+r[8]);if(io_status)return io_status;goto L8c038320; /* fmov.s @(r0,r8),fr9 */
L8c038320: *stop=0x8c038320u;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]+r[9]);if(io_status)return io_status;goto L8c038322; /* fmov.s @(r0,r9),fr12 */
L8c038322: *stop=0x8c038322u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0;goto L8c038324; /* fldi0 fr0 */
L8c038324: *stop=0x8c038324u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0x3f800000u;goto L8c038326; /* fldi1 fr1 */
L8c038326: *stop=0x8c038326u;r[0]=0u;goto L8c038328; /* mov #0x0,r0 */
L8c038328: *stop=0x8c038328u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][9],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c03832a; /* fcmp/gt fr0,fr9 */
L8c03832a: if((*sr&1u))goto L8c03832e;goto L8c03832c; /* bt 0x8c03832e */
L8c03832c: *stop=0x8c03832cu;r[0]|=0x1u;goto L8c03832e; /* or #0x1,r0 */
L8c03832e: *stop=0x8c03832eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][9],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c038330; /* fcmp/gt fr1,fr9 */
L8c038330: if(!(*sr&1u))goto L8c038334;goto L8c038332; /* bf 0x8c038334 */
L8c038332: *stop=0x8c038332u;r[0]|=0x10u;goto L8c038334; /* or #0x10,r0 */
L8c038334: *stop=0x8c038334u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c038336; /* fcmp/gt fr0,fr12 */
L8c038336: if((*sr&1u))goto L8c03833a;goto L8c038338; /* bt 0x8c03833a */
L8c038338: *stop=0x8c038338u;r[0]|=0x2u;goto L8c03833a; /* or #0x2,r0 */
L8c03833a: *stop=0x8c03833au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c03833c; /* fcmp/gt fr1,fr12 */
L8c03833c: if(!(*sr&1u))goto L8c038340;goto L8c03833e; /* bf 0x8c038340 */
L8c03833e: *stop=0x8c03833eu;r[0]|=0x20u;goto L8c038340; /* or #0x20,r0 */
L8c038340: *stop=0x8c038340u;r[7]=r[0];goto L8c038342; /* mov r0,r7 */
L8c038342: *stop=0x8c038342u;if(!valid_access(r[4],2u))return -3;r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c038344; /* mov.w @r4+,r0 */
L8c038344: *stop=0x8c038344u;r[0]<<=2;goto L8c038346; /* shll2 r0 */
L8c038346: *stop=0x8c038346u;r[10]=r[0];goto L8c038348; /* mov r0,r10 */
L8c038348: *stop=0x8c038348u;if(!valid_access(r[0]+r[3],4u))return -3;r[0]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c03834a; /* mov.l @(r0,r3),r0 */
L8c03834a: *stop=0x8c03834au;if(!valid_ram(*gbr+0x50u,4u))return -3;wr(m,bus,&io_status,*gbr+0x50u,r[0],4);if(io_status)return io_status;goto L8c03834c; /* mov.l r0,@(0x50,GBR) */
L8c03834c: *stop=0x8c03834cu;r[0]=r[10];goto L8c03834e; /* mov r10,r0 */
L8c03834e: *stop=0x8c03834eu;r[0]+=r[10];goto L8c038350; /* add r10,r0 */
L8c038350: *stop=0x8c038350u;r[0]+=r[10];goto L8c038352; /* add r10,r0 */
L8c038352: *stop=0x8c038352u;r[10]<<=2;goto L8c038354; /* shll2 r10 */
L8c038354: *stop=0x8c038354u;r[10]+=r[5];goto L8c038356; /* add r5,r10 */
L8c038356: *stop=0x8c038356u;if(!valid_access(r[13]+4u,2u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[0],2);if(io_status)return io_status;goto L8c038358; /* mov.w r0,@(0x4,r13) */
L8c038358: *stop=0x8c038358u;r[0]=8u;goto L8c03835a; /* mov #0x8,r0 */
L8c03835a: *stop=0x8c03835au;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[10]);if(io_status)return io_status;goto L8c03835c; /* fmov.s @(r0,r10),fr15 */
L8c03835c: *stop=0x8c03835cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0;goto L8c03835e; /* fldi0 fr0 */
L8c03835e: *stop=0x8c03835eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0x3f800000u;goto L8c038360; /* fldi1 fr1 */
L8c038360: *stop=0x8c038360u;r[0]=r[7];goto L8c038362; /* mov r7,r0 */
L8c038362: *stop=0x8c038362u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c038364; /* fcmp/gt fr0,fr15 */
L8c038364: if((*sr&1u))goto L8c038368;goto L8c038366; /* bt 0x8c038368 */
L8c038366: *stop=0x8c038366u;r[0]|=0x4u;goto L8c038368; /* or #0x4,r0 */
L8c038368: *stop=0x8c038368u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c03836a; /* fcmp/gt fr1,fr15 */
L8c03836a: if(!(*sr&1u))goto L8c03836e;goto L8c03836c; /* bf 0x8c03836e */
L8c03836c: *stop=0x8c03836cu;r[0]|=0x40u;goto L8c03836e; /* or #0x40,r0 */
L8c03836e: *stop=0x8c03836eu;r[7]=r[0];goto L8c038370; /* mov r0,r7 */
L8c038370: *stop=0x8c038370u;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c038372; /* tst r7,r7 */
L8c038372: if(!(*sr&1u))goto L8c038410;goto L8c038374; /* bf 0x8c038410 */
L8c038374: *stop=0x8c038374u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c038376; /* tst r6,r6 */
L8c038376: if(!(*sr&1u))goto L8c0383be;goto L8c038378; /* bf 0x8c0383be */
L8c038378: *stop=0x8c038378u;if(!valid_access(r[13]+0u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x0u,2);if(io_status)return io_status;goto L8c03837a; /* mov.w @(0x0,r13),r0 */
L8c03837a: *stop=0x8c03837au;r[0]+=r[1];goto L8c03837c; /* add r1,r0 */
L8c03837c: *stop=0x8c03837cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03837e; /* fmov.s @r0+,fr0 */
L8c03837e: *stop=0x8c03837eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038380; /* fmov.s @r0+,fr1 */
L8c038380: *stop=0x8c038380u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038382; /* fmov.s @r0+,fr2 */
L8c038382: *stop=0x8c038382u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c038384; /* fldi0 fr3 */
L8c038384: *stop=0x8c038384u;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c038386; /* ftrv xmtrx,fv0 */
L8c038386: *stop=0x8c038386u;r[0]=rd(m,bus,&io_status,0x8c03840cu,4);if(io_status)return io_status;goto L8c038388; /* mov.l 0x8c03840c,r0 */
L8c038388: *stop=0x8c038388u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c03838a; /* fmov.s @r0,fr3 */
L8c03838a: *stop=0x8c03838au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c03838c; /* fmul fr3,fr0 */
L8c03838c: *stop=0x8c03838cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c03838e; /* fadd fr3,fr0 */
L8c03838e: *stop=0x8c03838eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c038390; /* fmul fr3,fr1 */
L8c038390: *stop=0x8c038390u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c038392; /* fadd fr3,fr1 */
L8c038392: *stop=0x8c038392u;fcopy(f,5u,0u);goto L8c038394; /* fmov fr0,fr5 */
L8c038394: *stop=0x8c038394u;fcopy(f,6u,1u);goto L8c038396; /* fmov fr1,fr6 */
L8c038396: *stop=0x8c038396u;r[0]=r[8];goto L8c038398; /* mov r8,r0 */
L8c038398: *stop=0x8c038398u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03839a; /* fmov.s @r0+,fr0 */
L8c03839a: *stop=0x8c03839au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03839c; /* fmov.s @r0+,fr1 */
L8c03839c: *stop=0x8c03839cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03839e; /* fmov.s @r0+,fr2 */
L8c03839e: *stop=0x8c03839eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0383a0; /* fmov.s @r0+,fr3 */
L8c0383a0: *stop=0x8c0383a0u;if(!valid_ram(*gbr+0x48u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x48u,4);if(io_status)return io_status;goto L8c0383a2; /* mov.l @(0x48,GBR),r0 */
L8c0383a2: *stop=0x8c0383a2u;*fpul=r[0];goto L8c0383a4; /* lds r0,FPUL */
L8c0383a4: *stop=0x8c0383a4u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c0383a6; /* fsts FPUL,fr4 */
L8c0383a6: *stop=0x8c0383a6u;r[14]+=32u;goto L8c0383a8; /* add #0x20,r14 */
L8c0383a8: *stop=0x8c0383a8u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c0383aa; /* fmov.s fr4,@-r14 */
L8c0383aa: *stop=0x8c0383aau;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c0383ac; /* fmov.s fr3,@-r14 */
L8c0383ac: *stop=0x8c0383acu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c0383ae; /* fmov.s fr6,@-r14 */
L8c0383ae: *stop=0x8c0383aeu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c0383b0; /* fmov.s fr5,@-r14 */
L8c0383b0: *stop=0x8c0383b0u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c0383b2; /* fmov.s fr2,@-r14 */
L8c0383b2: *stop=0x8c0383b2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c0383b4; /* fmov.s fr1,@-r14 */
L8c0383b4: *stop=0x8c0383b4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c0383b6; /* fmov.s fr0,@-r14 */
L8c0383b6: *stop=0x8c0383b6u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c0383b8; /* mov.l r11,@-r14 */
L8c0383b8: *stop=0x8c0383b8u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c0383ba; /* pref @r14 */
L8c0383ba: *stop=0x8c0383bau;r[14]+=32u;goto L8c0383bc; /* add #0x20,r14 */
L8c0383bc: *stop=0x8c0383bcu;r[6]+=1u;goto L8c0383be; /* add #0x1,r6 */
L8c0383be: *stop=0x8c0383beu;if(!valid_access(r[13]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c0383c0; /* mov.w @(0x2,r13),r0 */
L8c0383c0: *stop=0x8c0383c0u;r[0]+=r[1];goto L8c0383c2; /* add r1,r0 */
L8c0383c2: *stop=0x8c0383c2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0383c4; /* fmov.s @r0+,fr0 */
L8c0383c4: *stop=0x8c0383c4u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0383c6; /* fmov.s @r0+,fr1 */
L8c0383c6: *stop=0x8c0383c6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0383c8; /* fmov.s @r0+,fr2 */
L8c0383c8: *stop=0x8c0383c8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0383ca; /* fldi0 fr3 */
L8c0383ca: *stop=0x8c0383cau;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c0383cc; /* ftrv xmtrx,fv0 */
L8c0383cc: *stop=0x8c0383ccu;r[0]=rd(m,bus,&io_status,0x8c03840cu,4);if(io_status)return io_status;goto L8c0383ce; /* mov.l 0x8c03840c,r0 */
L8c0383ce: *stop=0x8c0383ceu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0383d0; /* fmov.s @r0,fr3 */
L8c0383d0: *stop=0x8c0383d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c0383d2; /* fmul fr3,fr0 */
L8c0383d2: *stop=0x8c0383d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c0383d4; /* fadd fr3,fr0 */
L8c0383d4: *stop=0x8c0383d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c0383d6; /* fmul fr3,fr1 */
L8c0383d6: *stop=0x8c0383d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c0383d8; /* fadd fr3,fr1 */
L8c0383d8: *stop=0x8c0383d8u;fcopy(f,5u,0u);goto L8c0383da; /* fmov fr0,fr5 */
L8c0383da: *stop=0x8c0383dau;fcopy(f,6u,1u);goto L8c0383dc; /* fmov fr1,fr6 */
L8c0383dc: *stop=0x8c0383dcu;r[0]=r[9];goto L8c0383de; /* mov r9,r0 */
L8c0383de: *stop=0x8c0383deu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0383e0; /* fmov.s @r0+,fr0 */
L8c0383e0: *stop=0x8c0383e0u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0383e2; /* fmov.s @r0+,fr1 */
L8c0383e2: *stop=0x8c0383e2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0383e4; /* fmov.s @r0+,fr2 */
L8c0383e4: *stop=0x8c0383e4u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0383e6; /* fmov.s @r0+,fr3 */
L8c0383e6: *stop=0x8c0383e6u;if(!valid_ram(*gbr+0x4cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x4cu,4);if(io_status)return io_status;goto L8c0383e8; /* mov.l @(0x4c,GBR),r0 */
L8c0383e8: *stop=0x8c0383e8u;*fpul=r[0];goto L8c0383ea; /* lds r0,FPUL */
L8c0383ea: *stop=0x8c0383eau;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c0383ec; /* fsts FPUL,fr4 */
L8c0383ec: *stop=0x8c0383ecu;r[14]+=32u;goto L8c0383ee; /* add #0x20,r14 */
L8c0383ee: *stop=0x8c0383eeu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c0383f0; /* fmov.s fr4,@-r14 */
L8c0383f0: *stop=0x8c0383f0u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c0383f2; /* fmov.s fr3,@-r14 */
L8c0383f2: *stop=0x8c0383f2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c0383f4; /* fmov.s fr6,@-r14 */
L8c0383f4: *stop=0x8c0383f4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c0383f6; /* fmov.s fr5,@-r14 */
L8c0383f6: *stop=0x8c0383f6u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c0383f8; /* fmov.s fr2,@-r14 */
L8c0383f8: *stop=0x8c0383f8u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c0383fa; /* fmov.s fr1,@-r14 */
L8c0383fa: *stop=0x8c0383fau;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c0383fc; /* fmov.s fr0,@-r14 */
L8c0383fc: *stop=0x8c0383fcu;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c0383fe; /* mov.l r11,@-r14 */
L8c0383fe: *stop=0x8c0383feu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c038400; /* pref @r14 */
L8c038400: *stop=0x8c038400u;r[14]+=32u;goto L8c038402; /* add #0x20,r14 */
L8c038402: *stop=0x8c038404u;r[6]+=1u;goto L8c03847c; /* bra 0x8c03847c */
L8c038410: *stop=0x8c038410u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c038412; /* tst r6,r6 */
L8c038412: if((*sr&1u))goto L8c038468;goto L8c038414; /* bt 0x8c038468 */
L8c038414: *stop=0x8c038414u;if(!valid_access(r[13]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c038416; /* mov.w @(0x2,r13),r0 */
L8c038416: *stop=0x8c038416u;r[0]+=r[1];goto L8c038418; /* add r1,r0 */
L8c038418: *stop=0x8c038418u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03841a; /* fmov.s @r0+,fr0 */
L8c03841a: *stop=0x8c03841au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03841c; /* fmov.s @r0+,fr1 */
L8c03841c: *stop=0x8c03841cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03841e; /* fmov.s @r0+,fr2 */
L8c03841e: *stop=0x8c03841eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c038420; /* fldi0 fr3 */
L8c038420: *stop=0x8c038420u;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c038422; /* ftrv xmtrx,fv0 */
L8c038422: *stop=0x8c038422u;r[0]=rd(m,bus,&io_status,0x8c038460u,4);if(io_status)return io_status;goto L8c038424; /* mov.l 0x8c038460,r0 */
L8c038424: *stop=0x8c038424u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c038426; /* fmov.s @r0,fr3 */
L8c038426: *stop=0x8c038426u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c038428; /* fmul fr3,fr0 */
L8c038428: *stop=0x8c038428u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c03842a; /* fadd fr3,fr0 */
L8c03842a: *stop=0x8c03842au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c03842c; /* fmul fr3,fr1 */
L8c03842c: *stop=0x8c03842cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c03842e; /* fadd fr3,fr1 */
L8c03842e: *stop=0x8c03842eu;fcopy(f,5u,0u);goto L8c038430; /* fmov fr0,fr5 */
L8c038430: *stop=0x8c038430u;fcopy(f,6u,1u);goto L8c038432; /* fmov fr1,fr6 */
L8c038432: *stop=0x8c038432u;r[0]=r[9];goto L8c038434; /* mov r9,r0 */
L8c038434: *stop=0x8c038434u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038436; /* fmov.s @r0+,fr0 */
L8c038436: *stop=0x8c038436u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c038438; /* fmov.s @r0+,fr1 */
L8c038438: *stop=0x8c038438u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03843a; /* fmov.s @r0+,fr2 */
L8c03843a: *stop=0x8c03843au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c03843c; /* fmov.s @r0+,fr3 */
L8c03843c: *stop=0x8c03843cu;if(!valid_ram(*gbr+0x4cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x4cu,4);if(io_status)return io_status;goto L8c03843e; /* mov.l @(0x4c,GBR),r0 */
L8c03843e: *stop=0x8c03843eu;*fpul=r[0];goto L8c038440; /* lds r0,FPUL */
L8c038440: *stop=0x8c038440u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c038442; /* fsts FPUL,fr4 */
L8c038442: *stop=0x8c038442u;r[6]=rd(m,bus,&io_status,0x8c038464u,4);if(io_status)return io_status;goto L8c038444; /* mov.l 0x8c038464,r6 */
L8c038444: *stop=0x8c038444u;r[14]+=32u;goto L8c038446; /* add #0x20,r14 */
L8c038446: *stop=0x8c038446u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c038448; /* fmov.s fr4,@-r14 */
L8c038448: *stop=0x8c038448u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c03844a; /* fmov.s fr3,@-r14 */
L8c03844a: *stop=0x8c03844au;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c03844c; /* fmov.s fr6,@-r14 */
L8c03844c: *stop=0x8c03844cu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c03844e; /* fmov.s fr5,@-r14 */
L8c03844e: *stop=0x8c03844eu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c038450; /* fmov.s fr2,@-r14 */
L8c038450: *stop=0x8c038450u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c038452; /* fmov.s fr1,@-r14 */
L8c038452: *stop=0x8c038452u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c038454; /* fmov.s fr0,@-r14 */
L8c038454: *stop=0x8c038454u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[6],4);if(io_status)return io_status;goto L8c038456; /* mov.l r6,@-r14 */
L8c038456: *stop=0x8c038456u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c038458; /* pref @r14 */
L8c038458: *stop=0x8c038458u;r[14]+=32u;goto L8c03845a; /* add #0x20,r14 */
L8c03845a: *stop=0x8c03845cu;r[6]=0u;goto L8c038468; /* bra 0x8c038468 */
L8c038468: *stop=0x8c038468u;r[0]=r[7];goto L8c03846a; /* mov r7,r0 */
L8c03846a: *stop=0x8c03846au;*sr=(*sr&~1u)|!(r[0]&0x11u);goto L8c03846c; /* tst #0x11,r0 */
L8c03846c: if((*sr&1u))goto L8c038476;goto L8c03846e; /* bt 0x8c038476 */
L8c03846e: *stop=0x8c03846eu;*sr=(*sr&~1u)|!(r[0]&0x22u);goto L8c038470; /* tst #0x22,r0 */
L8c038470: if((*sr&1u))goto L8c038476;goto L8c038472; /* bt 0x8c038476 */
L8c038472: *stop=0x8c038472u;*sr=(*sr&~1u)|!(r[0]&0x44u);goto L8c038474; /* tst #0x44,r0 */
L8c038474: if(!(*sr&1u))goto L8c03847c;goto L8c038476; /* bf 0x8c03847c */
L8c038476: target=0x8c038516u;*pr=0x8c03847au;*stop=0x8c038478u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c03847a; /* bsr 0x8c038516 */
L8c03847a: *stop=0x8c03847au;r[11]=rd(m,bus,&io_status,0x8c0384a0u,4);if(io_status)return io_status;goto L8c03847c; /* mov.l 0x8c0384a0,r11 */
L8c03847c: *stop=0x8c03847cu;r[8]=r[9];goto L8c03847e; /* mov r9,r8 */
L8c03847e: *stop=0x8c03847eu;r[9]=r[10];goto L8c038480; /* mov r10,r9 */
L8c038480: *stop=0x8c038480u;if(!valid_ram(*gbr+0x4cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x4cu,4);if(io_status)return io_status;goto L8c038482; /* mov.l @(0x4c,GBR),r0 */
L8c038482: *stop=0x8c038482u;if(!valid_ram(*gbr+0x48u,4u))return -3;wr(m,bus,&io_status,*gbr+0x48u,r[0],4);if(io_status)return io_status;goto L8c038484; /* mov.l r0,@(0x48,GBR) */
L8c038484: *stop=0x8c038484u;if(!valid_ram(*gbr+0x50u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x50u,4);if(io_status)return io_status;goto L8c038486; /* mov.l @(0x50,GBR),r0 */
L8c038486: *stop=0x8c038486u;if(!valid_ram(*gbr+0x4cu,4u))return -3;wr(m,bus,&io_status,*gbr+0x4cu,r[0],4);if(io_status)return io_status;goto L8c038488; /* mov.l r0,@(0x4c,GBR) */
L8c038488: *stop=0x8c038488u;if(!valid_access(r[13]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c03848a; /* mov.w @(0x2,r13),r0 */
L8c03848a: *stop=0x8c03848au;if(!valid_access(r[13]+0u,2u))return -3;wr(m,bus,&io_status,r[13]+0x0u,r[0],2);if(io_status)return io_status;goto L8c03848c; /* mov.w r0,@(0x0,r13) */
L8c03848c: *stop=0x8c03848cu;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c03848e; /* mov.w @(0x4,r13),r0 */
L8c03848e: *stop=0x8c03848eu;if(!valid_access(r[13]+2u,2u))return -3;wr(m,bus,&io_status,r[13]+0x2u,r[0],2);if(io_status)return io_status;goto L8c038490; /* mov.w r0,@(0x2,r13) */
L8c038490: *stop=0x8c038490u;*sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1;goto L8c038492; /* shlr r7 */
L8c038492: *stop=0x8c038492u;r[0]=51u;goto L8c038494; /* mov #0x33,r0 */
L8c038494: *stop=0x8c038494u;r[7]&=r[0];goto L8c038496; /* and r0,r7 */
L8c038496: *stop=0x8c038496u;r[12]--;*sr=(*sr&~1u)|(!r[12]);goto L8c038498; /* dt r12 */
L8c038498: if((*sr&1u))goto L8c0384a4;goto L8c03849a; /* bt 0x8c0384a4 */
L8c03849a: *stop=0x8c03849cu;goto L8c038342; /* bra 0x8c038342 */
L8c0384a4: *stop=0x8c0384a4u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c0384a6; /* tst r6,r6 */
L8c0384a6: if((*sr&1u))goto L8c0384fc;goto L8c0384a8; /* bt 0x8c0384fc */
L8c0384a8: *stop=0x8c0384a8u;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c0384aa; /* mov.w @(0x4,r13),r0 */
L8c0384aa: *stop=0x8c0384aau;r[0]+=r[1];goto L8c0384ac; /* add r1,r0 */
L8c0384ac: *stop=0x8c0384acu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0384ae; /* fmov.s @r0+,fr0 */
L8c0384ae: *stop=0x8c0384aeu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0384b0; /* fmov.s @r0+,fr1 */
L8c0384b0: *stop=0x8c0384b0u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0384b2; /* fmov.s @r0+,fr2 */
L8c0384b2: *stop=0x8c0384b2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0384b4; /* fldi0 fr3 */
L8c0384b4: *stop=0x8c0384b4u;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c0384b6; /* ftrv xmtrx,fv0 */
L8c0384b6: *stop=0x8c0384b6u;r[0]=rd(m,bus,&io_status,0x8c0384f4u,4);if(io_status)return io_status;goto L8c0384b8; /* mov.l 0x8c0384f4,r0 */
L8c0384b8: *stop=0x8c0384b8u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0384ba; /* fmov.s @r0,fr3 */
L8c0384ba: *stop=0x8c0384bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[3],&f->fpscr,&fr[0]);if(status)return status;}goto L8c0384bc; /* fmul fr3,fr0 */
L8c0384bc: *stop=0x8c0384bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c0384be; /* fadd fr3,fr0 */
L8c0384be: *stop=0x8c0384beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c0384c0; /* fmul fr3,fr1 */
L8c0384c0: *stop=0x8c0384c0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c0384c2; /* fadd fr3,fr1 */
L8c0384c2: *stop=0x8c0384c2u;fcopy(f,5u,0u);goto L8c0384c4; /* fmov fr0,fr5 */
L8c0384c4: *stop=0x8c0384c4u;fcopy(f,6u,1u);goto L8c0384c6; /* fmov fr1,fr6 */
L8c0384c6: *stop=0x8c0384c6u;r[0]=r[10];goto L8c0384c8; /* mov r10,r0 */
L8c0384c8: *stop=0x8c0384c8u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0384ca; /* fmov.s @r0+,fr0 */
L8c0384ca: *stop=0x8c0384cau;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0384cc; /* fmov.s @r0+,fr1 */
L8c0384cc: *stop=0x8c0384ccu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0384ce; /* fmov.s @r0+,fr2 */
L8c0384ce: *stop=0x8c0384ceu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0384d0; /* fmov.s @r0+,fr3 */
L8c0384d0: *stop=0x8c0384d0u;if(!valid_ram(*gbr+0x50u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x50u,4);if(io_status)return io_status;goto L8c0384d2; /* mov.l @(0x50,GBR),r0 */
L8c0384d2: *stop=0x8c0384d2u;*fpul=r[0];goto L8c0384d4; /* lds r0,FPUL */
L8c0384d4: *stop=0x8c0384d4u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c0384d6; /* fsts FPUL,fr4 */
L8c0384d6: *stop=0x8c0384d6u;r[11]=rd(m,bus,&io_status,0x8c0384f8u,4);if(io_status)return io_status;goto L8c0384d8; /* mov.l 0x8c0384f8,r11 */
L8c0384d8: *stop=0x8c0384d8u;r[14]+=32u;goto L8c0384da; /* add #0x20,r14 */
L8c0384da: *stop=0x8c0384dau;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c0384dc; /* fmov.s fr4,@-r14 */
L8c0384dc: *stop=0x8c0384dcu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c0384de; /* fmov.s fr3,@-r14 */
L8c0384de: *stop=0x8c0384deu;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c0384e0; /* fmov.s fr6,@-r14 */
L8c0384e0: *stop=0x8c0384e0u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c0384e2; /* fmov.s fr5,@-r14 */
L8c0384e2: *stop=0x8c0384e2u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c0384e4; /* fmov.s fr2,@-r14 */
L8c0384e4: *stop=0x8c0384e4u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;goto L8c0384e6; /* fmov.s fr1,@-r14 */
L8c0384e6: *stop=0x8c0384e6u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c0384e8; /* fmov.s fr0,@-r14 */
L8c0384e8: *stop=0x8c0384e8u;if(!valid_access(r[14]-4u,4u))return -3;r[14]-=4;wr(m,bus,&io_status,r[14],r[11],4);if(io_status)return io_status;goto L8c0384ea; /* mov.l r11,@-r14 */
L8c0384ea: *stop=0x8c0384eau;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[14],1u);if(status)return status;goto L8c0384ec; /* pref @r14 */
L8c0384ec: *stop=0x8c0384ecu;r[14]+=32u;goto L8c0384ee; /* add #0x20,r14 */
L8c0384ee: *stop=0x8c0384f0u;goto L8c0384fc; /* bra 0x8c0384fc */
L8c0384fc: *stop=0x8c0384fcu;if(!valid_ram(*gbr+0x44u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x44u,4);if(io_status)return io_status;goto L8c0384fe; /* mov.l @(0x44,GBR),r0 */
L8c0384fe: *stop=0x8c0384feu;r[0]+=4294967295u;goto L8c038500; /* add #-0x1,r0 */
L8c038500: *stop=0x8c038500u;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c038502; /* cmp/pl r0 */
L8c038502: if(!(*sr&1u))goto L8c038508;goto L8c038504; /* bf 0x8c038508 */
L8c038504: *stop=0x8c038506u;if(!valid_ram(*gbr+0x44u,4u))return -3;wr(m,bus,&io_status,*gbr+0x44u,r[0],4);if(io_status)return io_status;goto L8c0382cc; /* bra 0x8c0382cc */
L8c038508: *stop=0x8c038508u;if(!valid_access(r[15],4u))return -3;r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c03850a; /* mov.l @r15+,r3 */
L8c03850a: *stop=0x8c03850au;if(!valid_access(r[15],4u))return -3;r[4]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c03850c; /* mov.l @r15+,r4 */
L8c03850c: *stop=0x8c03850cu;if(!valid_access(r[15],4u))return -3;r[7]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c03850e; /* mov.l @r15+,r7 */
L8c03850e: *stop=0x8c03850eu;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c038510; /* mov.l @r15+,r13 */
L8c038510: *stop=0x8c038510u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c038512; /* lds.l @r15+,PR */
L8c038512: *stop=0x8c038512u;if(*pr!=entry_pr)return -3;*stop=0x8c038514u;r[0]=r[14];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_primitive_stream_8c60b462(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60b462;
L8c60b462: *stop=0x8c60b462u;r[6]=rd(m,bus,&io_status,0x8c60b4fcu,4);if(io_status)return io_status;goto L8c60b464; /* mov.l 0x8c60b4fc,r6 */
L8c60b464: *stop=0x8c60b464u;r[1]=32u;goto L8c60b466; /* mov #0x20,r1 */
L8c60b466: *stop=0x8c60b466u;r[1]+=r[4];goto L8c60b468; /* add r4,r1 */
L8c60b468: *stop=0x8c60b468u;r[5]=rd(m,bus,&io_status,0x8c60b508u,4);if(io_status)return io_status;goto L8c60b46a; /* mov.l 0x8c60b508,r5 */
L8c60b46a: *stop=0x8c60b46au;if(!valid_access(r[6],2u))return -3;r[0]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c60b46c; /* mov.w @r6,r0 */
L8c60b46c: *stop=0x8c60b46cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c60b46e; /* fmov.s @r1,fr3 */
L8c60b46e: *stop=0x8c60b46eu;r[1]=36u;goto L8c60b470; /* mov #0x24,r1 */
L8c60b470: *stop=0x8c60b470u;r[0]=r[0]&65535u;goto L8c60b472; /* extu.w r0,r0 */
L8c60b472: *stop=0x8c60b472u;r[3]=r[0];goto L8c60b474; /* mov r0,r3 */
L8c60b474: *stop=0x8c60b474u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b476; /* shll r0 */
L8c60b476: *stop=0x8c60b476u;r[0]+=r[3];goto L8c60b478; /* add r3,r0 */
L8c60b478: *stop=0x8c60b478u;r[0]<<=2;goto L8c60b47a; /* shll2 r0 */
L8c60b47a: *stop=0x8c60b47au;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c60b47c; /* fmov fr3,@(r0,r5) */
L8c60b47c: *stop=0x8c60b47cu;r[1]+=r[4];goto L8c60b47e; /* add r4,r1 */
L8c60b47e: *stop=0x8c60b47eu;if(!valid_access(r[6],2u))return -3;r[0]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c60b480; /* mov.w @r6,r0 */
L8c60b480: *stop=0x8c60b480u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c60b482; /* fmov.s @r1,fr3 */
L8c60b482: *stop=0x8c60b482u;r[1]=40u;goto L8c60b484; /* mov #0x28,r1 */
L8c60b484: *stop=0x8c60b484u;r[0]=r[0]&65535u;goto L8c60b486; /* extu.w r0,r0 */
L8c60b486: *stop=0x8c60b486u;r[3]=r[0];goto L8c60b488; /* mov r0,r3 */
L8c60b488: *stop=0x8c60b488u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b48a; /* shll r0 */
L8c60b48a: *stop=0x8c60b48au;r[0]+=r[3];goto L8c60b48c; /* add r3,r0 */
L8c60b48c: *stop=0x8c60b48cu;r[0]+=1u;goto L8c60b48e; /* add #0x1,r0 */
L8c60b48e: *stop=0x8c60b48eu;r[0]<<=2;goto L8c60b490; /* shll2 r0 */
L8c60b490: *stop=0x8c60b490u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c60b492; /* fmov fr3,@(r0,r5) */
L8c60b492: *stop=0x8c60b492u;r[1]+=r[4];goto L8c60b494; /* add r4,r1 */
L8c60b494: *stop=0x8c60b494u;if(!valid_access(r[6],2u))return -3;r[0]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c60b496; /* mov.w @r6,r0 */
L8c60b496: *stop=0x8c60b496u;r[0]=r[0]&65535u;goto L8c60b498; /* extu.w r0,r0 */
L8c60b498: *stop=0x8c60b498u;r[3]=r[0];goto L8c60b49a; /* mov r0,r3 */
L8c60b49a: *stop=0x8c60b49au;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c60b49c; /* shll r0 */
L8c60b49c: *stop=0x8c60b49cu;r[0]+=r[3];goto L8c60b49e; /* add r3,r0 */
L8c60b49e: *stop=0x8c60b49eu;r[0]+=2u;goto L8c60b4a0; /* add #0x2,r0 */
L8c60b4a0: *stop=0x8c60b4a0u;r[0]<<=2;goto L8c60b4a2; /* shll2 r0 */
L8c60b4a2: *stop=0x8c60b4a2u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c60b4a4; /* fmov.s @r1,fr3 */
L8c60b4a4: *stop=0x8c60b4a4u;if(*pr!=entry_pr)return -3;*stop=0x8c60b4a6u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;return 0; /* rts */
}
