/* Geometry projection, sampling and view proposals; explicit GBR/queue state.
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
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
int sa_sh4_sqrt(uint32_t,uint32_t*,uint32_t*);
SA_STAGE_EXPORT int sa_frame_setup_8c10d39c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d39c;
L8c10d39c: *stop=0x8c10d39cu;r[3]=rd(m,bus,&io_status,0x8c10d3a4u,4);if(io_status)return io_status;goto L8c10d39e; /* mov.l 0x8c10d3a4,r3 */
L8c10d39e: *stop=0x8c10d39eu;if(*pr!=entry_pr)return -3;*stop=0x8c10d3a0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c60526a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60526a;
L8c60526a: *stop=0x8c60526au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c60526c; /* mov.l r13,@-r15 */
L8c60526c: *stop=0x8c60526cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c60526e; /* sts.l PR,@-r15 */
L8c60526e: *stop=0x8c60526eu;r[15]+=4294967188u;goto L8c605270; /* add #-0x6c,r15 */
L8c605270: *stop=0x8c605270u;r[13]=r[15];goto L8c605272; /* mov r15,r13 */
L8c605272: *stop=0x8c605272u;r[13]+=12u;goto L8c605274; /* add #0xc,r13 */
L8c605274: *stop=0x8c605274u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c605276; /* fldi0 fr4 */
L8c605276: *stop=0x8c605276u;r[0]=4u;goto L8c605278; /* mov #0x4,r0 */
L8c605278: *stop=0x8c605278u;r[7]=r[13];goto L8c60527a; /* mov r13,r7 */
L8c60527a: *stop=0x8c60527au;r[2]=rd(m,bus,&io_status,0x8c605388u,4);if(io_status)return io_status;goto L8c60527c; /* mov.l 0x8c605388,r2 */
L8c60527c: *stop=0x8c60527cu;r[1]=r[7];goto L8c60527e; /* mov r7,r1 */
L8c60527e: *stop=0x8c60527eu;r[1]+=32u;goto L8c605280; /* add #0x20,r1 */
L8c605280: *stop=0x8c605280u;r[3]=rd(m,bus,&io_status,0x8c605380u,4);if(io_status)return io_status;goto L8c605282; /* mov.l 0x8c605380,r3 */
L8c605282: *stop=0x8c605282u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[3],4);if(io_status)return io_status;goto L8c605284; /* mov.l r3,@r7 */
L8c605284: *stop=0x8c605284u;if(!valid_access(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[7]);if(io_status)return io_status;goto L8c605286; /* fmov fr4,@(r0,r7) */
L8c605286: *stop=0x8c605286u;r[0]=8u;goto L8c605288; /* mov #0x8,r0 */
L8c605288: *stop=0x8c605288u;if(!valid_access(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[7]);if(io_status)return io_status;goto L8c60528a; /* fmov fr4,@(r0,r7) */
L8c60528a: *stop=0x8c60528au;r[0]=0x8c605384u;goto L8c60528c; /* mova 0x8c605384,r0 */
L8c60528c: *stop=0x8c60528cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c60528e; /* fmov.s @r0,fr5 */
L8c60528e: *stop=0x8c60528eu;r[0]=12u;goto L8c605290; /* mov #0xc,r0 */
L8c605290: *stop=0x8c605290u;if(!valid_access(r[0]+r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[7]);if(io_status)return io_status;goto L8c605292; /* fmov fr5,@(r0,r7) */
L8c605292: *stop=0x8c605292u;r[0]=4u;goto L8c605294; /* mov #0x4,r0 */
L8c605294: *stop=0x8c605294u;if(!valid_access(r[7]+24u,4u))return -3;wr(m,bus,&io_status,r[7]+0x18u,r[4],4);if(io_status)return io_status;goto L8c605296; /* mov.l r4,@(0x18,r7) */
L8c605296: *stop=0x8c605296u;r[4]=r[7];goto L8c605298; /* mov r7,r4 */
L8c605298: *stop=0x8c605298u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c60529a; /* mov.l r3,@r1 */
L8c60529a: *stop=0x8c60529au;r[4]+=64u;goto L8c60529c; /* add #0x40,r4 */
L8c60529c: *stop=0x8c60529cu;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c60529e; /* fmov.s @r2,fr3 */
L8c60529e: *stop=0x8c60529eu;r[3]=rd(m,bus,&io_status,0x8c60538cu,4);if(io_status)return io_status;goto L8c6052a0; /* mov.l 0x8c60538c,r3 */
L8c6052a0: *stop=0x8c6052a0u;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6052a2; /* fmov fr3,@(r0,r1) */
L8c6052a2: *stop=0x8c6052a2u;r[0]=8u;goto L8c6052a4; /* mov #0x8,r0 */
L8c6052a4: *stop=0x8c6052a4u;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[1]);if(io_status)return io_status;goto L8c6052a6; /* fmov fr4,@(r0,r1) */
L8c6052a6: *stop=0x8c6052a6u;r[0]=12u;goto L8c6052a8; /* mov #0xc,r0 */
L8c6052a8: *stop=0x8c6052a8u;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[1]);if(io_status)return io_status;goto L8c6052aa; /* fmov fr5,@(r0,r1) */
L8c6052aa: *stop=0x8c6052aau;r[0]=4u;goto L8c6052ac; /* mov #0x4,r0 */
L8c6052ac: *stop=0x8c6052acu;if(!valid_access(r[1]+24u,4u))return -3;wr(m,bus,&io_status,r[1]+0x18u,r[5],4);if(io_status)return io_status;goto L8c6052ae; /* mov.l r5,@(0x18,r1) */
L8c6052ae: *stop=0x8c6052aeu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c6052b0; /* mov.l r3,@r4 */
L8c6052b0: *stop=0x8c6052b0u;r[3]=rd(m,bus,&io_status,0x8c605390u,4);if(io_status)return io_status;goto L8c6052b2; /* mov.l 0x8c605390,r3 */
L8c6052b2: *stop=0x8c6052b2u;r[13]=r[15];goto L8c6052b4; /* mov r15,r13 */
L8c6052b4: *stop=0x8c6052b4u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c6052b6; /* fmov fr4,@(r0,r4) */
L8c6052b6: *stop=0x8c6052b6u;r[0]=8u;goto L8c6052b8; /* mov #0x8,r0 */
L8c6052b8: *stop=0x8c6052b8u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6052ba; /* fmov.s @r3,fr3 */
L8c6052ba: *stop=0x8c6052bau;r[5]=0u;goto L8c6052bc; /* mov #0x0,r5 */
L8c6052bc: *stop=0x8c6052bcu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6052be; /* fmov fr3,@(r0,r4) */
L8c6052be: *stop=0x8c6052beu;r[0]=12u;goto L8c6052c0; /* mov #0xc,r0 */
L8c6052c0: *stop=0x8c6052c0u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c6052c2; /* fmov fr5,@(r0,r4) */
L8c6052c2: *stop=0x8c6052c2u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[6],4);if(io_status)return io_status;goto L8c6052c4; /* mov.l r6,@(0x18,r4) */
L8c6052c4: *stop=0x8c6052c4u;r[6]=32u;goto L8c6052c6; /* mov #0x20,r6 */
L8c6052c6: *stop=0x8c6052c6u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[7],4);if(io_status)return io_status;goto L8c6052c8; /* mov.l r7,@r13 */
L8c6052c8: *stop=0x8c6052c8u;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[1],4);if(io_status)return io_status;goto L8c6052ca; /* mov.l r1,@(0x4,r13) */
L8c6052ca: *stop=0x8c6052cau;r[1]=rd(m,bus,&io_status,0x8c605394u,4);if(io_status)return io_status;goto L8c6052cc; /* mov.l 0x8c605394,r1 */
L8c6052cc: *stop=0x8c6052ccu;if(!valid_access(r[13]+8u,4u))return -3;wr(m,bus,&io_status,r[13]+0x8u,r[4],4);if(io_status)return io_status;goto L8c6052ce; /* mov.l r4,@(0x8,r13) */
L8c6052ce: target=r[1];*pr=0x8c6052d2u;*stop=0x8c6052d0u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6052d2; /* jsr @r1 */
L8c6052d2: *stop=0x8c6052d2u;r[15]+=108u;goto L8c6052d4; /* add #0x6c,r15 */
L8c6052d4: *stop=0x8c6052d4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6052d6; /* lds.l @r15+,PR */
L8c6052d6: *stop=0x8c6052d6u;if(*pr!=entry_pr)return -3;*stop=0x8c6052d8u;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c6054c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6054c6;
L8c6054c6: *stop=0x8c6054c6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6054c8; /* mov.l r14,@-r15 */
L8c6054c8: *stop=0x8c6054c8u;r[0]=r[4];goto L8c6054ca; /* mov r4,r0 */
L8c6054ca: *stop=0x8c6054cau;r[14]=rd(m,bus,&io_status,0x8c6054ecu,4);if(io_status)return io_status;goto L8c6054cc; /* mov.l 0x8c6054ec,r14 */
L8c6054cc: *stop=0x8c6054ccu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6054ce; /* cmp/eq #0x1,r0 */
L8c6054ce: *stop=0x8c6054ceu;r[2]=rd(m,bus,&io_status,0x8c6054fcu,4);if(io_status)return io_status;goto L8c6054d0; /* mov.l 0x8c6054fc,r2 */
L8c6054d0: *stop=0x8c6054d0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6054d2; /* sts.l PR,@-r15 */
L8c6054d2: *stop=0x8c6054d2u;if(!valid_access(r[14],4u))return -3;r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6054d4; /* mov.l @r14,r3 */
L8c6054d4: *stop=0x8c6054d4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6054d6; /* mov.l r2,@r3 */
L8c6054d6: take=(*sr&1u);*stop=0x8c6054d8u;if(!valid_access(r[14],4u))return -3;r[5]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;if(take)goto L8c605500;goto L8c6054da; /* bt/s 0x8c605500 */
L8c6054da: *stop=0x8c6054dau;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c6054dc; /* cmp/eq #0x0,r0 */
L8c6054dc: if((*sr&1u))goto L8c605508;goto L8c6054de; /* bt 0x8c605508 */
L8c6054de: *stop=0x8c6054e0u;goto L8c60550e; /* bra 0x8c60550e */
L8c605500: *stop=0x8c605500u;r[0]=88u;goto L8c605502; /* mov #0x58,r0 */
L8c605502: *stop=0x8c605502u;r[3]=1u;goto L8c605504; /* mov #0x1,r3 */
L8c605504: *stop=0x8c605506u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[3],4);if(io_status)return io_status;goto L8c60550e; /* bra 0x8c60550e */
L8c605508: *stop=0x8c605508u;r[0]=88u;goto L8c60550a; /* mov #0x58,r0 */
L8c60550a: *stop=0x8c60550au;r[1]=0u;goto L8c60550c; /* mov #0x0,r1 */
L8c60550c: *stop=0x8c60550cu;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[1],4);if(io_status)return io_status;goto L8c60550e; /* mov.l r1,@(r0,r5) */
L8c60550e: *stop=0x8c60550eu;r[2]=rd(m,bus,&io_status,0x8c6055c4u,4);if(io_status)return io_status;goto L8c605510; /* mov.l 0x8c6055c4,r2 */
L8c605510: target=r[2];*pr=0x8c605514u;*stop=0x8c605512u;if(!valid_access(r[14],4u))return -3;r[4]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c605514; /* jsr @r2 */
L8c605514: *stop=0x8c605514u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c605516; /* lds.l @r15+,PR */
L8c605516: *stop=0x8c605516u;r[3]=rd(m,bus,&io_status,0x8c6055c8u,4);if(io_status)return io_status;goto L8c605518; /* mov.l 0x8c6055c8,r3 */
L8c605518: *stop=0x8c605518u;if(!valid_access(r[14],4u))return -3;r[4]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c60551a; /* mov.l @r14,r4 */
L8c60551a: target=r[3];*stop=0x8c60551cu;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_frame_setup_8c60551e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60551e;
L8c60551e: *stop=0x8c60551eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c605520; /* mov.l r14,@-r15 */
L8c605520: *stop=0x8c605520u;r[0]=r[4];goto L8c605522; /* mov r4,r0 */
L8c605522: *stop=0x8c605522u;r[14]=rd(m,bus,&io_status,0x8c6055ccu,4);if(io_status)return io_status;goto L8c605524; /* mov.l 0x8c6055cc,r14 */
L8c605524: *stop=0x8c605524u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c605526; /* cmp/eq #0x0,r0 */
L8c605526: *stop=0x8c605526u;r[2]=sx(m,bus,&io_status,0x8c6055c0u,2);if(io_status)return io_status;goto L8c605528; /* mov.w 0x8c6055c0,r2 */
L8c605528: *stop=0x8c605528u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c60552a; /* sts.l PR,@-r15 */
L8c60552a: *stop=0x8c60552au;if(!valid_access(r[14],4u))return -3;r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c60552c; /* mov.l @r14,r3 */
L8c60552c: *stop=0x8c60552cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c60552e; /* mov.l r2,@r3 */
L8c60552e: take=(*sr&1u);*stop=0x8c605530u;if(!valid_access(r[14],4u))return -3;r[5]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;if(take)goto L8c605542;goto L8c605532; /* bt/s 0x8c605542 */
L8c605532: *stop=0x8c605532u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c605534; /* cmp/eq #0x1,r0 */
L8c605534: if((*sr&1u))goto L8c60554a;goto L8c605536; /* bt 0x8c60554a */
L8c605536: *stop=0x8c605536u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c605538; /* cmp/eq #0x2,r0 */
L8c605538: if((*sr&1u))goto L8c60554e;goto L8c60553a; /* bt 0x8c60554e */
L8c60553a: *stop=0x8c60553au;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c60553c; /* cmp/eq #0x3,r0 */
L8c60553c: if((*sr&1u))goto L8c605556;goto L8c60553e; /* bt 0x8c605556 */
L8c60553e: *stop=0x8c605540u;goto L8c60555c; /* bra 0x8c60555c */
L8c605542: *stop=0x8c605542u;r[0]=76u;goto L8c605544; /* mov #0x4c,r0 */
L8c605544: *stop=0x8c605544u;r[3]=0u;goto L8c605546; /* mov #0x0,r3 */
L8c605546: *stop=0x8c605548u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[3],4);if(io_status)return io_status;goto L8c60555c; /* bra 0x8c60555c */
L8c60554a: *stop=0x8c60554cu;r[1]=1u;goto L8c605558; /* bra 0x8c605558 */
L8c60554e: *stop=0x8c60554eu;r[0]=76u;goto L8c605550; /* mov #0x4c,r0 */
L8c605550: *stop=0x8c605550u;r[2]=2u;goto L8c605552; /* mov #0x2,r2 */
L8c605552: *stop=0x8c605554u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[2],4);if(io_status)return io_status;goto L8c60555c; /* bra 0x8c60555c */
L8c605556: *stop=0x8c605556u;r[1]=3u;goto L8c605558; /* mov #0x3,r1 */
L8c605558: *stop=0x8c605558u;r[0]=76u;goto L8c60555a; /* mov #0x4c,r0 */
L8c60555a: *stop=0x8c60555au;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[1],4);if(io_status)return io_status;goto L8c60555c; /* mov.l r1,@(r0,r5) */
L8c60555c: *stop=0x8c60555cu;r[2]=rd(m,bus,&io_status,0x8c6055c4u,4);if(io_status)return io_status;goto L8c60555e; /* mov.l 0x8c6055c4,r2 */
L8c60555e: target=r[2];*pr=0x8c605562u;*stop=0x8c605560u;if(!valid_access(r[14],4u))return -3;r[4]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c605562; /* jsr @r2 */
L8c605562: *stop=0x8c605562u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c605564; /* lds.l @r15+,PR */
L8c605564: *stop=0x8c605564u;r[3]=rd(m,bus,&io_status,0x8c6055c8u,4);if(io_status)return io_status;goto L8c605566; /* mov.l 0x8c6055c8,r3 */
L8c605566: *stop=0x8c605566u;if(!valid_access(r[14],4u))return -3;r[4]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c605568; /* mov.l @r14,r4 */
L8c605568: target=r[3];*stop=0x8c60556au;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_frame_setup_8c60556c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c60556c;
L8c60556c: *stop=0x8c60556cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c60556e; /* mov.l r14,@-r15 */
L8c60556e: *stop=0x8c60556eu;r[0]=r[4];goto L8c605570; /* mov r4,r0 */
L8c605570: *stop=0x8c605570u;r[14]=rd(m,bus,&io_status,0x8c6055ccu,4);if(io_status)return io_status;goto L8c605572; /* mov.l 0x8c6055cc,r14 */
L8c605572: *stop=0x8c605572u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c605574; /* cmp/eq #0x0,r0 */
L8c605574: *stop=0x8c605574u;r[2]=sx(m,bus,&io_status,0x8c6055c2u,2);if(io_status)return io_status;goto L8c605576; /* mov.w 0x8c6055c2,r2 */
L8c605576: *stop=0x8c605576u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c605578; /* sts.l PR,@-r15 */
L8c605578: *stop=0x8c605578u;if(!valid_access(r[14],4u))return -3;r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c60557a; /* mov.l @r14,r3 */
L8c60557a: *stop=0x8c60557au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c60557c; /* mov.l r2,@r3 */
L8c60557c: take=(*sr&1u);*stop=0x8c60557eu;if(!valid_access(r[14],4u))return -3;r[5]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;if(take)goto L8c605590;goto L8c605580; /* bt/s 0x8c605590 */
L8c605580: *stop=0x8c605580u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c605582; /* cmp/eq #0x1,r0 */
L8c605582: if((*sr&1u))goto L8c605598;goto L8c605584; /* bt 0x8c605598 */
L8c605584: *stop=0x8c605584u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c605586; /* cmp/eq #0x2,r0 */
L8c605586: if((*sr&1u))goto L8c60559c;goto L8c605588; /* bt 0x8c60559c */
L8c605588: *stop=0x8c605588u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c60558a; /* cmp/eq #0x3,r0 */
L8c60558a: if((*sr&1u))goto L8c6055a4;goto L8c60558c; /* bt 0x8c6055a4 */
L8c60558c: *stop=0x8c60558eu;goto L8c6055aa; /* bra 0x8c6055aa */
L8c605590: *stop=0x8c605590u;r[0]=80u;goto L8c605592; /* mov #0x50,r0 */
L8c605592: *stop=0x8c605592u;r[3]=0u;goto L8c605594; /* mov #0x0,r3 */
L8c605594: *stop=0x8c605596u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[3],4);if(io_status)return io_status;goto L8c6055aa; /* bra 0x8c6055aa */
L8c605598: *stop=0x8c60559au;r[1]=1u;goto L8c6055a6; /* bra 0x8c6055a6 */
L8c60559c: *stop=0x8c60559cu;r[0]=80u;goto L8c60559e; /* mov #0x50,r0 */
L8c60559e: *stop=0x8c60559eu;r[2]=2u;goto L8c6055a0; /* mov #0x2,r2 */
L8c6055a0: *stop=0x8c6055a2u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[2],4);if(io_status)return io_status;goto L8c6055aa; /* bra 0x8c6055aa */
L8c6055a4: *stop=0x8c6055a4u;r[1]=3u;goto L8c6055a6; /* mov #0x3,r1 */
L8c6055a6: *stop=0x8c6055a6u;r[0]=80u;goto L8c6055a8; /* mov #0x50,r0 */
L8c6055a8: *stop=0x8c6055a8u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[1],4);if(io_status)return io_status;goto L8c6055aa; /* mov.l r1,@(r0,r5) */
L8c6055aa: *stop=0x8c6055aau;r[2]=rd(m,bus,&io_status,0x8c6055c4u,4);if(io_status)return io_status;goto L8c6055ac; /* mov.l 0x8c6055c4,r2 */
L8c6055ac: target=r[2];*pr=0x8c6055b0u;*stop=0x8c6055aeu;if(!valid_access(r[14],4u))return -3;r[4]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6055b0; /* jsr @r2 */
L8c6055b0: *stop=0x8c6055b0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6055b2; /* lds.l @r15+,PR */
L8c6055b2: *stop=0x8c6055b2u;r[3]=rd(m,bus,&io_status,0x8c6055c8u,4);if(io_status)return io_status;goto L8c6055b4; /* mov.l 0x8c6055c8,r3 */
L8c6055b4: *stop=0x8c6055b4u;if(!valid_access(r[14],4u))return -3;r[4]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6055b6; /* mov.l @r14,r4 */
L8c6055b6: target=r[3];*stop=0x8c6055b8u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_frame_setup_8c6055ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6055ba;
L8c6055ba: *stop=0x8c6055bau;r[3]=rd(m,bus,&io_status,0x8c6055d0u,4);if(io_status)return io_status;goto L8c6055bc; /* mov.l 0x8c6055d0,r3 */
L8c6055bc: target=r[3];*stop=0x8c6055beu;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_frame_setup_8c6055d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6055d4;
L8c6055d4: *stop=0x8c6055d4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6055d6; /* mov.l r14,@-r15 */
L8c6055d6: *stop=0x8c6055d6u;r[0]=r[4];goto L8c6055d8; /* mov r4,r0 */
L8c6055d8: *stop=0x8c6055d8u;r[14]=rd(m,bus,&io_status,0x8c60561cu,4);if(io_status)return io_status;goto L8c6055da; /* mov.l 0x8c60561c,r14 */
L8c6055da: *stop=0x8c6055dau;r[6]=2u;goto L8c6055dc; /* mov #0x2,r6 */
L8c6055dc: *stop=0x8c6055dcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6055de; /* sts.l PR,@-r15 */
L8c6055de: *stop=0x8c6055deu;if(!valid_access(r[14],4u))return -3;r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c6055e0; /* mov.l @r14,r3 */
L8c6055e0: *stop=0x8c6055e0u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c6055e2; /* cmp/eq #0x0,r0 */
L8c6055e2: *stop=0x8c6055e2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[6],4);if(io_status)return io_status;goto L8c6055e4; /* mov.l r6,@r3 */
L8c6055e4: take=(*sr&1u);*stop=0x8c6055e6u;if(!valid_access(r[14],4u))return -3;r[5]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;if(take)goto L8c6055f8;goto L8c6055e8; /* bt/s 0x8c6055f8 */
L8c6055e8: *stop=0x8c6055e8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6055ea; /* cmp/eq #0x1,r0 */
L8c6055ea: if((*sr&1u))goto L8c6055fe;goto L8c6055ec; /* bt 0x8c6055fe */
L8c6055ec: *stop=0x8c6055ecu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c6055ee; /* cmp/eq #0x2,r0 */
L8c6055ee: if((*sr&1u))goto L8c605604;goto L8c6055f0; /* bt 0x8c605604 */
L8c6055f0: *stop=0x8c6055f0u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c6055f2; /* cmp/eq #0x3,r0 */
L8c6055f2: if((*sr&1u))goto L8c605608;goto L8c6055f4; /* bt 0x8c605608 */
L8c6055f4: *stop=0x8c6055f6u;goto L8c60560c; /* bra 0x8c60560c */
L8c6055f8: *stop=0x8c6055f8u;r[2]=0u;goto L8c6055fa; /* mov #0x0,r2 */
L8c6055fa: *stop=0x8c6055fcu;if(!valid_access(r[5]+24u,4u))return -3;wr(m,bus,&io_status,r[5]+0x18u,r[2],4);if(io_status)return io_status;goto L8c60560c; /* bra 0x8c60560c */
L8c6055fe: *stop=0x8c6055feu;r[1]=1u;goto L8c605600; /* mov #0x1,r1 */
L8c605600: *stop=0x8c605602u;if(!valid_access(r[5]+24u,4u))return -3;wr(m,bus,&io_status,r[5]+0x18u,r[1],4);if(io_status)return io_status;goto L8c60560c; /* bra 0x8c60560c */
L8c605604: *stop=0x8c605606u;if(!valid_access(r[5]+24u,4u))return -3;wr(m,bus,&io_status,r[5]+0x18u,r[6],4);if(io_status)return io_status;goto L8c60560c; /* bra 0x8c60560c */
L8c605608: *stop=0x8c605608u;r[3]=3u;goto L8c60560a; /* mov #0x3,r3 */
L8c60560a: *stop=0x8c60560au;if(!valid_access(r[5]+24u,4u))return -3;wr(m,bus,&io_status,r[5]+0x18u,r[3],4);if(io_status)return io_status;goto L8c60560c; /* mov.l r3,@(0x18,r5) */
L8c60560c: *stop=0x8c60560cu;r[3]=rd(m,bus,&io_status,0x8c605620u,4);if(io_status)return io_status;goto L8c60560e; /* mov.l 0x8c605620,r3 */
L8c60560e: target=r[3];*pr=0x8c605612u;*stop=0x8c605610u;if(!valid_access(r[14],4u))return -3;r[4]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c605612; /* jsr @r3 */
L8c605612: *stop=0x8c605612u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c605614; /* lds.l @r15+,PR */
L8c605614: *stop=0x8c605614u;r[2]=rd(m,bus,&io_status,0x8c605624u,4);if(io_status)return io_status;goto L8c605616; /* mov.l 0x8c605624,r2 */
L8c605616: *stop=0x8c605616u;if(!valid_access(r[14],4u))return -3;r[4]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c605618; /* mov.l @r14,r4 */
L8c605618: target=r[2];*stop=0x8c60561au;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_frame_setup_8c608c2a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c608c2a;
L8c608c2a: *stop=0x8c608c2au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c608c2c; /* sts.l PR,@-r15 */
L8c608c2c: *stop=0x8c608c2cu;r[3]=rd(m,bus,&io_status,0x8c608cecu,4);if(io_status)return io_status;goto L8c608c2e; /* mov.l 0x8c608cec,r3 */
L8c608c2e: target=r[3];*pr=0x8c608c32u;*stop=0x8c608c30u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c608c32; /* jsr @r3 */
L8c608c32: *stop=0x8c608c32u;r[1]=rd(m,bus,&io_status,0x8c608cf4u,4);if(io_status)return io_status;goto L8c608c34; /* mov.l 0x8c608cf4,r1 */
L8c608c34: *stop=0x8c608c34u;r[3]=2u;goto L8c608c36; /* mov #0x2,r3 */
L8c608c36: *stop=0x8c608c36u;r[2]=rd(m,bus,&io_status,0x8c608cf0u,4);if(io_status)return io_status;goto L8c608c38; /* mov.l 0x8c608cf0,r2 */
L8c608c38: *stop=0x8c608c38u;r[5]=0u;goto L8c608c3a; /* mov #0x0,r5 */
L8c608c3a: *stop=0x8c608c3au;r[4]=rd(m,bus,&io_status,0x8c608cfcu,4);if(io_status)return io_status;goto L8c608c3c; /* mov.l 0x8c608cfc,r4 */
L8c608c3c: *stop=0x8c608c3cu;r[0]=rd(m,bus,&io_status,0x8c608cf8u,4);if(io_status)return io_status;goto L8c608c3e; /* mov.l 0x8c608cf8,r0 */
L8c608c3e: *stop=0x8c608c3eu;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[5],1);if(io_status)return io_status;goto L8c608c40; /* mov.b r5,@r2 */
L8c608c40: *stop=0x8c608c40u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c608c42; /* mov.l r3,@r1 */
L8c608c42: *stop=0x8c608c42u;r[3]=r[5];goto L8c608c44; /* mov r5,r3 */
L8c608c44: *stop=0x8c608c44u;r[2]=r[3];goto L8c608c46; /* mov r3,r2 */
L8c608c46: *stop=0x8c608c46u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[5],4);if(io_status)return io_status;goto L8c608c48; /* mov.l r5,@r0 */
L8c608c48: *stop=0x8c608c48u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c608c4a; /* mov.l r5,@r4 */
L8c608c4a: *stop=0x8c608c4au;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[3],4);if(io_status)return io_status;goto L8c608c4c; /* mov.l r3,@(0x4,r4) */
L8c608c4c: *stop=0x8c608c4cu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[2],4);if(io_status)return io_status;goto L8c608c4e; /* mov.l r2,@(0x8,r4) */
L8c608c4e: *stop=0x8c608c4eu;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[2],4);if(io_status)return io_status;goto L8c608c50; /* mov.l r2,@(0xc,r4) */
L8c608c50: *stop=0x8c608c50u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[2],4);if(io_status)return io_status;goto L8c608c52; /* mov.l r2,@(0x10,r4) */
L8c608c52: *stop=0x8c608c52u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[2],4);if(io_status)return io_status;goto L8c608c54; /* mov.l r2,@(0x14,r4) */
L8c608c54: *stop=0x8c608c54u;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[2],4);if(io_status)return io_status;goto L8c608c56; /* mov.l r2,@(0x18,r4) */
L8c608c56: *stop=0x8c608c56u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c608c58; /* lds.l @r15+,PR */
L8c608c58: *stop=0x8c608c58u;if(*pr!=entry_pr)return -3;*stop=0x8c608c5au;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[2],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c60ede0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60ede0;
L8c60ede0: *stop=0x8c60ede0u;r[3]=rd(m,bus,&io_status,0x8c60ee34u,4);if(io_status)return io_status;goto L8c60ede2; /* mov.l 0x8c60ee34,r3 */
L8c60ede2: *stop=0x8c60ede2u;if(*pr!=entry_pr)return -3;*stop=0x8c60ede4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c60ede6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60ede6;
L8c60ede6: *stop=0x8c60ede6u;r[2]=rd(m,bus,&io_status,0x8c60ee38u,4);if(io_status)return io_status;goto L8c60ede8; /* mov.l 0x8c60ee38,r2 */
L8c60ede8: *stop=0x8c60ede8u;r[3]=rd(m,bus,&io_status,0x8c60ee3cu,4);if(io_status)return io_status;goto L8c60edea; /* mov.l 0x8c60ee3c,r3 */
L8c60edea: *stop=0x8c60edeau;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c60edec; /* mov.l r4,@r2 */
L8c60edec: *stop=0x8c60edecu;if(*pr!=entry_pr)return -3;*stop=0x8c60edeeu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c60ee0a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60ee0a;
L8c60ee0a: *stop=0x8c60ee0au;r[4]=rd(m,bus,&io_status,0x8c60ee48u,4);if(io_status)return io_status;goto L8c60ee0c; /* mov.l 0x8c60ee48,r4 */
L8c60ee0c: *stop=0x8c60ee0cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]^=0x80000000u;goto L8c60ee0e; /* fneg fr5 */
L8c60ee0e: *stop=0x8c60ee0eu;fcopy(f,2u,5u);goto L8c60ee10; /* fmov fr5,fr2 */
L8c60ee10: *stop=0x8c60ee10u;r[5]=rd(m,bus,&io_status,0x8c60ee4cu,4);if(io_status)return io_status;goto L8c60ee12; /* mov.l 0x8c60ee4c,r5 */
L8c60ee12: *stop=0x8c60ee12u;r[3]=rd(m,bus,&io_status,0x8c60ee50u,4);if(io_status)return io_status;goto L8c60ee14; /* mov.l 0x8c60ee50,r3 */
L8c60ee14: *stop=0x8c60ee14u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c60ee16; /* fneg fr4 */
L8c60ee16: *stop=0x8c60ee16u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c60ee18; /* fmov.s fr4,@r4 */
L8c60ee18: *stop=0x8c60ee18u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;goto L8c60ee1a; /* fmov.s fr5,@r5 */
L8c60ee1a: *stop=0x8c60ee1au;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c60ee1c; /* fmov.s @r4,fr3 */
L8c60ee1c: *stop=0x8c60ee1cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c60ee1e; /* fsub fr3,fr2 */
L8c60ee1e: *stop=0x8c60ee1eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c60ee20; /* fldi1 fr3 */
L8c60ee20: *stop=0x8c60ee20u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c60ee22; /* fdiv fr2,fr3 */
L8c60ee22: *stop=0x8c60ee22u;if(*pr!=entry_pr)return -3;*stop=0x8c60ee24u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c638fe4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c638fe4;
L8c638fe4: *stop=0x8c638fe4u;r[0]=rd(m,bus,&io_status,0x8c639074u,4);if(io_status)return io_status;goto L8c638fe6; /* mov.l 0x8c639074,r0 */
L8c638fe6: *stop=0x8c638fe6u;r[7]=1u;goto L8c638fe8; /* mov #0x1,r7 */
L8c638fe8: *stop=0x8c638fe8u;r[2]=rd(m,bus,&io_status,0x8c639078u,4);if(io_status)return io_status;goto L8c638fea; /* mov.l 0x8c639078,r2 */
L8c638fea: *stop=0x8c638feau;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[7],4);if(io_status)return io_status;goto L8c638fec; /* mov.l r7,@r0 */
L8c638fec: *stop=0x8c638fecu;r[1]=rd(m,bus,&io_status,0x8c63907cu,4);if(io_status)return io_status;goto L8c638fee; /* mov.l 0x8c63907c,r1 */
L8c638fee: *stop=0x8c638feeu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[6],4);if(io_status)return io_status;goto L8c638ff0; /* mov.l r6,@r2 */
L8c638ff0: *stop=0x8c638ff0u;r[0]=rd(m,bus,&io_status,0x8c639080u,4);if(io_status)return io_status;goto L8c638ff2; /* mov.l 0x8c639080,r0 */
L8c638ff2: *stop=0x8c638ff2u;r[3]=rd(m,bus,&io_status,0x8c639084u,4);if(io_status)return io_status;goto L8c638ff4; /* mov.l 0x8c639084,r3 */
L8c638ff4: *stop=0x8c638ff4u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[4],4);if(io_status)return io_status;goto L8c638ff6; /* mov.l r4,@r0 */
L8c638ff6: *stop=0x8c638ff6u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[5],4);if(io_status)return io_status;goto L8c638ff8; /* mov.l r5,@r1 */
L8c638ff8: *stop=0x8c638ff8u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c638ffa; /* mov.l r4,@r3 */
L8c638ffa: *stop=0x8c638ffau;r[1]=rd(m,bus,&io_status,0x8c639088u,4);if(io_status)return io_status;goto L8c638ffc; /* mov.l 0x8c639088,r1 */
L8c638ffc: *stop=0x8c638ffcu;r[4]+=64u;goto L8c638ffe; /* add #0x40,r4 */
L8c638ffe: *stop=0x8c638ffeu;r[1]+=32u;goto L8c639000; /* add #0x20,r1 */
L8c639000: *stop=0x8c639000u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639002; /* fmov.s @r1+,fr0 */
L8c639002: *stop=0x8c639002u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639004; /* fmov.s @r1+,fr1 */
L8c639004: *stop=0x8c639004u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639006; /* fmov.s @r1+,fr2 */
L8c639006: *stop=0x8c639006u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639008; /* fmov.s @r1+,fr3 */
L8c639008: *stop=0x8c639008u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63900a; /* fmov.s @r1+,fr4 */
L8c63900a: *stop=0x8c63900au;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63900c; /* fmov.s @r1+,fr5 */
L8c63900c: *stop=0x8c63900cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63900e; /* fmov.s @r1+,fr6 */
L8c63900e: *stop=0x8c63900eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639010; /* fmov.s @r1+,fr7 */
L8c639010: *stop=0x8c639010u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c639012; /* fmov.s fr7,@-r4 */
L8c639012: *stop=0x8c639012u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c639014; /* fmov.s fr6,@-r4 */
L8c639014: *stop=0x8c639014u;r[1]+=4294967232u;goto L8c639016; /* add #-0x40,r1 */
L8c639016: *stop=0x8c639016u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c639018; /* fmov.s fr5,@-r4 */
L8c639018: *stop=0x8c639018u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c63901a; /* fmov.s fr4,@-r4 */
L8c63901a: *stop=0x8c63901au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c63901c; /* fmov.s fr3,@-r4 */
L8c63901c: *stop=0x8c63901cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c63901e; /* fmov.s fr2,@-r4 */
L8c63901e: *stop=0x8c63901eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;goto L8c639020; /* fmov.s fr1,@-r4 */
L8c639020: *stop=0x8c639020u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c639022; /* fmov.s fr0,@-r4 */
L8c639022: *stop=0x8c639022u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639024; /* fmov.s @r1+,fr0 */
L8c639024: *stop=0x8c639024u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639026; /* fmov.s @r1+,fr1 */
L8c639026: *stop=0x8c639026u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639028; /* fmov.s @r1+,fr2 */
L8c639028: *stop=0x8c639028u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63902a; /* fmov.s @r1+,fr3 */
L8c63902a: *stop=0x8c63902au;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63902c; /* fmov.s @r1+,fr4 */
L8c63902c: *stop=0x8c63902cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63902e; /* fmov.s @r1+,fr5 */
L8c63902e: *stop=0x8c63902eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639030; /* fmov.s @r1+,fr6 */
L8c639030: *stop=0x8c639030u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639032; /* fmov.s @r1+,fr7 */
L8c639032: *stop=0x8c639032u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c639034; /* fmov.s fr7,@-r4 */
L8c639034: *stop=0x8c639034u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c639036; /* fmov.s fr6,@-r4 */
L8c639036: *stop=0x8c639036u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c639038; /* fmov.s fr5,@-r4 */
L8c639038: *stop=0x8c639038u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c63903a; /* fmov.s fr4,@-r4 */
L8c63903a: *stop=0x8c63903au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c63903c; /* fmov.s fr3,@-r4 */
L8c63903c: *stop=0x8c63903cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c63903e; /* fmov.s fr2,@-r4 */
L8c63903e: *stop=0x8c63903eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;goto L8c639040; /* fmov.s fr1,@-r4 */
L8c639040: *stop=0x8c639040u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c639042; /* fmov.s fr0,@-r4 */
L8c639042: *stop=0x8c639042u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;goto L8c639044; /* fneg fr0 */
L8c639044: *stop=0x8c639044u;r[0]=20u;goto L8c639046; /* mov #0x14,r0 */
L8c639046: *stop=0x8c639046u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]^=0x80000000u;goto L8c639048; /* fneg fr5 */
L8c639048: *stop=0x8c639048u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c63904a; /* fmov.s fr0,@r4 */
L8c63904a: *stop=0x8c63904au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c63904c; /* fmov fr5,@(r0,r4) */
L8c63904c: *stop=0x8c63904cu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c63904e; /* frchg */
L8c63904e: *stop=0x8c63904eu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639050; /* fmov.s @r4+,fr0 */
L8c639050: *stop=0x8c639050u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639052; /* fmov.s @r4+,fr1 */
L8c639052: *stop=0x8c639052u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639054; /* fmov.s @r4+,fr2 */
L8c639054: *stop=0x8c639054u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639056; /* fmov.s @r4+,fr3 */
L8c639056: *stop=0x8c639056u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639058; /* fmov.s @r4+,fr4 */
L8c639058: *stop=0x8c639058u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63905a; /* fmov.s @r4+,fr5 */
L8c63905a: *stop=0x8c63905au;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63905c; /* fmov.s @r4+,fr6 */
L8c63905c: *stop=0x8c63905cu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63905e; /* fmov.s @r4+,fr7 */
L8c63905e: *stop=0x8c63905eu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639060; /* fmov.s @r4+,fr8 */
L8c639060: *stop=0x8c639060u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639062; /* fmov.s @r4+,fr9 */
L8c639062: *stop=0x8c639062u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639064; /* fmov.s @r4+,fr10 */
L8c639064: *stop=0x8c639064u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639066; /* fmov.s @r4+,fr11 */
L8c639066: *stop=0x8c639066u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639068; /* fmov.s @r4+,fr12 */
L8c639068: *stop=0x8c639068u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63906a; /* fmov.s @r4+,fr13 */
L8c63906a: *stop=0x8c63906au;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63906c; /* fmov.s @r4+,fr14 */
L8c63906c: *stop=0x8c63906cu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63906e; /* fmov.s @r4+,fr15 */
L8c63906e: *stop=0x8c63906eu;if(*pr!=entry_pr)return -3;*stop=0x8c639070u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c63fb18(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63fb18;
L8c63fb18: *stop=0x8c63fb18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c63fb1a; /* mov.l r14,@-r15 */
L8c63fb1a: *stop=0x8c63fb1au;r[14]=rd(m,bus,&io_status,0x8c63fb3cu,4);if(io_status)return io_status;goto L8c63fb1c; /* mov.l 0x8c63fb3c,r14 */
L8c63fb1c: *stop=0x8c63fb1cu;r[2]=sx(m,bus,&io_status,0x8c63fb3au,2);if(io_status)return io_status;goto L8c63fb1e; /* mov.w 0x8c63fb3a,r2 */
L8c63fb1e: *stop=0x8c63fb1eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c63fb20; /* sts.l PR,@-r15 */
L8c63fb20: *stop=0x8c63fb20u;if(!valid_access(r[14],4u))return -3;r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c63fb22; /* mov.l @r14,r3 */
L8c63fb22: *stop=0x8c63fb22u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c63fb24; /* mov.l r2,@r3 */
L8c63fb24: *stop=0x8c63fb24u;r[2]=0u;goto L8c63fb26; /* mov #0x0,r2 */
L8c63fb26: *stop=0x8c63fb26u;if(!valid_access(r[14],4u))return -3;r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c63fb28; /* mov.l @r14,r3 */
L8c63fb28: *stop=0x8c63fb28u;if(!valid_access(r[3]+60u,4u))return -3;wr(m,bus,&io_status,r[3]+0x3cu,r[2],4);if(io_status)return io_status;goto L8c63fb2a; /* mov.l r2,@(0x3c,r3) */
L8c63fb2a: *stop=0x8c63fb2au;r[3]=rd(m,bus,&io_status,0x8c63fb40u,4);if(io_status)return io_status;goto L8c63fb2c; /* mov.l 0x8c63fb40,r3 */
L8c63fb2c: target=r[3];*pr=0x8c63fb30u;*stop=0x8c63fb2eu;if(!valid_access(r[14],4u))return -3;r[4]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63fb30; /* jsr @r3 */
L8c63fb30: *stop=0x8c63fb30u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c63fb32; /* lds.l @r15+,PR */
L8c63fb32: *stop=0x8c63fb32u;r[2]=rd(m,bus,&io_status,0x8c63fb44u,4);if(io_status)return io_status;goto L8c63fb34; /* mov.l 0x8c63fb44,r2 */
L8c63fb34: *stop=0x8c63fb34u;if(!valid_access(r[14],4u))return -3;r[4]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c63fb36; /* mov.l @r14,r4 */
L8c63fb36: target=r[2];*stop=0x8c63fb38u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_frame_setup_8c63fdc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c63fdc0;
L8c63fdc0: *stop=0x8c63fdc0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c63fdc2; /* mov.l r14,@-r15 */
L8c63fdc2: *stop=0x8c63fdc2u;r[0]=0x8c63fe00u;goto L8c63fdc4; /* mova 0x8c63fe00,r0 */
L8c63fdc4: *stop=0x8c63fdc4u;r[14]=1u;goto L8c63fdc6; /* mov #0x1,r14 */
L8c63fdc6: *stop=0x8c63fdc6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c63fdc8; /* fmov.s @r0,fr4 */
L8c63fdc8: *stop=0x8c63fdc8u;r[7]=r[14];goto L8c63fdca; /* mov r14,r7 */
L8c63fdca: *stop=0x8c63fdcau;r[6]=15u;goto L8c63fdcc; /* mov #0xf,r6 */
L8c63fdcc: *stop=0x8c63fdccu;r[5]=0u;goto L8c63fdce; /* mov #0x0,r5 */
L8c63fdce: *stop=0x8c63fdceu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c63fdd0; /* fldi1 fr5 */
L8c63fdd0: *stop=0x8c63fdd0u;r[7]+=127u;goto L8c63fdd2; /* add #0x7f,r7 */
L8c63fdd2: *stop=0x8c63fdd2u;r[3]=r[5];goto L8c63fdd4; /* mov r5,r3 */
L8c63fdd4: *stop=0x8c63fdd4u;r[2]=4294967292u;goto L8c63fdd6; /* mov #-0x4,r2 */
L8c63fdd6: *stop=0x8c63fdd6u;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}}goto L8c63fdd8; /* shad r2,r3 */
L8c63fdd8: *stop=0x8c63fdd8u;r[1]=r[14];goto L8c63fdda; /* mov r14,r1 */
L8c63fdda: *stop=0x8c63fddau;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}}goto L8c63fddc; /* shad r3,r1 */
L8c63fddc: *stop=0x8c63fddcu;r[3]=r[5];goto L8c63fdde; /* mov r5,r3 */
L8c63fdde: *stop=0x8c63fddeu;r[3]&=r[6];goto L8c63fde0; /* and r6,r3 */
L8c63fde0: *stop=0x8c63fde0u;fcopy(f,2u,5u);goto L8c63fde2; /* fmov fr5,fr2 */
L8c63fde2: *stop=0x8c63fde2u;r[3]+=16u;goto L8c63fde4; /* add #0x10,r3 */
L8c63fde4: *stop=0x8c63fde4u;r[0]=r[5];goto L8c63fde6; /* mov r5,r0 */
L8c63fde6: *stop=0x8c63fde6u;*macl=r[3]*r[1];goto L8c63fde8; /* mul.l r3,r1 */
L8c63fde8: *stop=0x8c63fde8u;r[5]+=1u;goto L8c63fdea; /* add #0x1,r5 */
L8c63fdea: *stop=0x8c63fdeau;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[7]);goto L8c63fdec; /* cmp/ge r7,r5 */
L8c63fdec: *stop=0x8c63fdecu;r[0]<<=2;goto L8c63fdee; /* shll2 r0 */
L8c63fdee: *stop=0x8c63fdeeu;r[1]=*macl;goto L8c63fdf0; /* sts MACL,r1 */
L8c63fdf0: *stop=0x8c63fdf0u;*fpul=r[1];goto L8c63fdf2; /* lds r1,FPUL */
L8c63fdf2: *stop=0x8c63fdf2u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c63fdf4; /* float FPUL,fr3 */
L8c63fdf4: *stop=0x8c63fdf4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63fdf6; /* fdiv fr4,fr3 */
L8c63fdf6: *stop=0x8c63fdf6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63fdf8; /* fdiv fr3,fr2 */
L8c63fdf8: take=!(*sr&1u);*stop=0x8c63fdfau;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;if(take)goto L8c63fdd2;goto L8c63fdfc; /* bf/s 0x8c63fdd2 */
L8c63fdfc: *stop=0x8c63fdfcu;if(*pr!=entry_pr)return -3;*stop=0x8c63fdfeu;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c63fe20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63fe20;
L8c63fe20: *stop=0x8c63fe20u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c63fe22; /* sts.l PR,@-r15 */
L8c63fe22: *stop=0x8c63fe22u;r[15]+=4294967292u;goto L8c63fe24; /* add #-0x4,r15 */
L8c63fe24: *stop=0x8c63fe24u;r[2]=rd(m,bus,&io_status,0x8c63fe38u,4);if(io_status)return io_status;goto L8c63fe26; /* mov.l 0x8c63fe38,r2 */
L8c63fe26: *stop=0x8c63fe26u;r[3]=r[4];goto L8c63fe28; /* mov r4,r3 */
L8c63fe28: *stop=0x8c63fe28u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c63fe2a; /* mov.l r4,@r15 */
L8c63fe2a: target=r[2];*pr=0x8c63fe2eu;*stop=0x8c63fe2cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c63fe2e; /* jsr @r2 */
L8c63fe2e: *stop=0x8c63fe2eu;r[15]+=8u;goto L8c63fe30; /* add #0x8,r15 */
L8c63fe30: *stop=0x8c63fe30u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c63fe32; /* lds.l @r15+,PR */
L8c63fe32: *stop=0x8c63fe32u;if(*pr!=entry_pr)return -3;*stop=0x8c63fe34u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c63fe3c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63fe3c;
L8c63fe3c: *stop=0x8c63fe3cu;r[3]=rd(m,bus,&io_status,0x8c63fe44u,4);if(io_status)return io_status;goto L8c63fe3e; /* mov.l 0x8c63fe44,r3 */
L8c63fe3e: target=r[3];*stop=0x8c63fe40u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_frame_setup_8c63fe48(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63fe48;
L8c63fe48: *stop=0x8c63fe48u;r[3]=rd(m,bus,&io_status,0x8c63fe50u,4);if(io_status)return io_status;goto L8c63fe4a; /* mov.l 0x8c63fe50,r3 */
L8c63fe4a: target=r[3];*stop=0x8c63fe4cu;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_frame_setup_8c64036c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64036c;
L8c64036c: *stop=0x8c64036cu;r[0]=0x8c64039cu;goto L8c64036e; /* mova 0x8c64039c,r0 */
L8c64036e: *stop=0x8c64036eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;goto L8c640370; /* fmov.s @r0,fr0 */
L8c640370: *stop=0x8c640370u;r[0]=0x8c6403a0u;goto L8c640372; /* mova 0x8c6403a0,r0 */
L8c640372: *stop=0x8c640372u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;goto L8c640374; /* fmov.s @r0,fr1 */
L8c640374: *stop=0x8c640374u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c640376; /* fcmp/gt fr0,fr4 */
L8c640376: take=!(*sr&1u);*stop=0x8c640378u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;if(take)goto L8c64037c;goto L8c64037a; /* bf/s 0x8c64037c */
L8c64037a: *stop=0x8c64037au;fcopy(f,4u,0u);goto L8c64037c; /* fmov fr0,fr4 */
L8c64037c: take=!(*sr&1u);*stop=0x8c64037eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;if(take)goto L8c640382;goto L8c640380; /* bf/s 0x8c640382 */
L8c640380: *stop=0x8c640380u;fcopy(f,4u,1u);goto L8c640382; /* fmov fr1,fr4 */
L8c640382: take=!(*sr&1u);*stop=0x8c640384u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;if(take)goto L8c640388;goto L8c640386; /* bf/s 0x8c640388 */
L8c640386: *stop=0x8c640386u;fcopy(f,5u,0u);goto L8c640388; /* fmov fr0,fr5 */
L8c640388: take=!(*sr&1u);*stop=0x8c64038au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;if(take)goto L8c64038e;goto L8c64038c; /* bf/s 0x8c64038e */
L8c64038c: *stop=0x8c64038cu;fcopy(f,5u,1u);goto L8c64038e; /* fmov fr1,fr5 */
L8c64038e: *stop=0x8c64038eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]^=0x80000000u;goto L8c640390; /* fneg fr5 */
L8c640390: *stop=0x8c640390u;r[2]=rd(m,bus,&io_status,0x8c6403a4u,4);if(io_status)return io_status;goto L8c640392; /* mov.l 0x8c6403a4,r2 */
L8c640392: *stop=0x8c640392u;r[3]=rd(m,bus,&io_status,0x8c6403a8u,4);if(io_status)return io_status;goto L8c640394; /* mov.l 0x8c6403a8,r3 */
L8c640394: *stop=0x8c640394u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[2]);if(io_status)return io_status;goto L8c640396; /* fmov.s fr4,@r2 */
L8c640396: *stop=0x8c640396u;if(*pr!=entry_pr)return -3;*stop=0x8c640398u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[3]);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c6403b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6403b0;
L8c6403b0: *stop=0x8c6403b0u;r[5]=rd(m,bus,&io_status,0x8c640414u,4);if(io_status)return io_status;goto L8c6403b2; /* mov.l 0x8c640414,r5 */
L8c6403b2: *stop=0x8c6403b2u;r[4]+=120u;goto L8c6403b4; /* add #0x78,r4 */
L8c6403b4: *stop=0x8c6403b4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0x3f800000u;goto L8c6403b6; /* fldi1 fr1 */
L8c6403b6: *stop=0x8c6403b6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]=0;goto L8c6403b8; /* fldi0 fr0 */
L8c6403b8: *stop=0x8c6403b8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto L8c6403ba; /* fneg fr1 */
L8c6403ba: *stop=0x8c6403bau;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6403bc; /* fmov.s fr0,@-r4 */
L8c6403bc: *stop=0x8c6403bcu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;goto L8c6403be; /* fmov.s fr1,@-r4 */
L8c6403be: *stop=0x8c6403beu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6403c0; /* fmov.s fr0,@-r4 */
L8c6403c0: *stop=0x8c6403c0u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6403c2; /* fmov.s fr0,@-r4 */
L8c6403c2: *stop=0x8c6403c2u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6403c4; /* fmov.s fr0,@-r4 */
L8c6403c4: *stop=0x8c6403c4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6403c6; /* fmov.s fr0,@-r4 */
L8c6403c6: *stop=0x8c6403c6u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6403c8; /* fmov.s fr0,@-r4 */
L8c6403c8: *stop=0x8c6403c8u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6403ca; /* fmov.s fr0,@-r4 */
L8c6403ca: *stop=0x8c6403cau;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;goto L8c6403cc; /* fmov.s fr1,@-r4 */
L8c6403cc: *stop=0x8c6403ccu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6403ce; /* fmov.s fr0,@-r4 */
L8c6403ce: *stop=0x8c6403ceu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6403d0; /* fmov.s fr0,@-r4 */
L8c6403d0: *stop=0x8c6403d0u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6403d2; /* fmov.s fr0,@-r4 */
L8c6403d2: *stop=0x8c6403d2u;r[6]=r[5];goto L8c6403d4; /* mov r5,r6 */
L8c6403d4: *stop=0x8c6403d4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6403d6; /* fmov.s fr0,@-r4 */
L8c6403d6: *stop=0x8c6403d6u;r[6]+=32u;goto L8c6403d8; /* add #0x20,r6 */
L8c6403d8: *stop=0x8c6403d8u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6403da; /* fmov.s fr0,@-r4 */
L8c6403da: *stop=0x8c6403dau;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6403dc; /* fschg */
L8c6403dc: *stop=0x8c6403dcu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6403de; /* fmov.s @r6+,fr0 */
L8c6403de: *stop=0x8c6403deu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6403e0; /* fmov.s @r6+,fr2 */
L8c6403e0: *stop=0x8c6403e0u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6403e2; /* fmov.s @r6+,fr4 */
L8c6403e2: *stop=0x8c6403e2u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6403e4; /* fmov.s @r6+,fr6 */
L8c6403e4: *stop=0x8c6403e4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6403e6; /* fmov.s @r5+,fr8 */
L8c6403e6: *stop=0x8c6403e6u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6403e8; /* fmov.s @r5+,fr10 */
L8c6403e8: *stop=0x8c6403e8u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6403ea; /* fschg */
L8c6403ea: *stop=0x8c6403eau;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c6403ec; /* fmov.s fr7,@-r4 */
L8c6403ec: *stop=0x8c6403ecu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6403ee; /* fmov.s fr6,@-r4 */
L8c6403ee: *stop=0x8c6403eeu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c6403f0; /* fmov.s fr5,@-r4 */
L8c6403f0: *stop=0x8c6403f0u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c6403f2; /* fmov.s fr4,@-r4 */
L8c6403f2: *stop=0x8c6403f2u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c6403f4; /* fmov.s fr3,@-r4 */
L8c6403f4: *stop=0x8c6403f4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c6403f6; /* fmov.s fr2,@-r4 */
L8c6403f6: *stop=0x8c6403f6u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;goto L8c6403f8; /* fmov.s fr1,@-r4 */
L8c6403f8: *stop=0x8c6403f8u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c6403fa; /* fmov.s fr0,@-r4 */
L8c6403fa: *stop=0x8c6403fau;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6403fc; /* fschg */
L8c6403fc: *stop=0x8c6403fcu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6403fe; /* fmov.s @r5+,fr6 */
L8c6403fe: *stop=0x8c6403feu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c640400; /* fmov.s @r5+,fr4 */
L8c640400: *stop=0x8c640400u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c640402; /* fschg */
L8c640402: *stop=0x8c640402u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c640404; /* fmov.s fr5,@-r4 */
L8c640404: *stop=0x8c640404u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c640406; /* fmov.s fr4,@-r4 */
L8c640406: *stop=0x8c640406u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c640408; /* fmov.s fr7,@-r4 */
L8c640408: *stop=0x8c640408u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c64040a; /* fmov.s fr6,@-r4 */
L8c64040a: *stop=0x8c64040au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;goto L8c64040c; /* fmov.s fr11,@-r4 */
L8c64040c: *stop=0x8c64040cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;goto L8c64040e; /* fmov.s fr10,@-r4 */
L8c64040e: *stop=0x8c64040eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c640410; /* fmov.s fr9,@-r4 */
L8c640410: *stop=0x8c640410u;if(*pr!=entry_pr)return -3;*stop=0x8c640412u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c640500(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c640500;
L8c640500: *stop=0x8c640500u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c640502; /* sts.l PR,@-r15 */
L8c640502: *stop=0x8c640502u;r[3]=rd(m,bus,&io_status,0x8c64051cu,4);if(io_status)return io_status;goto L8c640504; /* mov.l 0x8c64051c,r3 */
L8c640504: target=r[3];*pr=0x8c640508u;*stop=0x8c640506u;*sr=(*sr&~1u)|(r[4]&1u);r[4]=(r[4]>>1)|(r[4]&0x80000000u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c640508; /* jsr @r3 */
L8c640508: *stop=0x8c640508u;r[3]=rd(m,bus,&io_status,0x8c640520u,4);if(io_status)return io_status;goto L8c64050a; /* mov.l 0x8c640520,r3 */
L8c64050a: *stop=0x8c64050au;fcopy(f,3u,0u);goto L8c64050c; /* fmov fr0,fr3 */
L8c64050c: *stop=0x8c64050cu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c64050e; /* fmov.s @r3,fr2 */
L8c64050e: *stop=0x8c64050eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c640510; /* fadd fr3,fr0 */
L8c640510: *stop=0x8c640510u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c640512; /* fdiv fr0,fr2 */
L8c640512: *stop=0x8c640512u;r[3]+=4294967292u;goto L8c640514; /* add #-0x4,r3 */
L8c640514: *stop=0x8c640514u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c640516; /* lds.l @r15+,PR */
L8c640516: *stop=0x8c640516u;if(*pr!=entry_pr)return -3;*stop=0x8c640518u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c640528(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c640528;
L8c640528: *stop=0x8c640528u;r[0]=12u;goto L8c64052a; /* mov #0xc,r0 */
L8c64052a: *stop=0x8c64052au;r[5]=rd(m,bus,&io_status,0x8c64053cu,4);if(io_status)return io_status;goto L8c64052c; /* mov.l 0x8c64053c,r5 */
L8c64052c: *stop=0x8c64052cu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c64052e; /* fmov.s @r4,fr3 */
L8c64052e: *stop=0x8c64052eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c640530; /* fmov.s fr3,@r5 */
L8c640530: *stop=0x8c640530u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c640532; /* fmov.s @(r0,r4),fr2 */
L8c640532: *stop=0x8c640532u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c640534; /* fmov fr2,@(r0,r5) */
L8c640534: *stop=0x8c640534u;r[0]=16u;goto L8c640536; /* mov #0x10,r0 */
L8c640536: *stop=0x8c640536u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c640538; /* fmov.s @(r0,r4),fr3 */
L8c640538: *stop=0x8c640538u;if(*pr!=entry_pr)return -3;*stop=0x8c64053au;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c640540(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c640540;
L8c640540: *stop=0x8c640540u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c640542; /* mov.l r14,@-r15 */
L8c640542: *stop=0x8c640542u;r[5]=rd(m,bus,&io_status,0x8c640630u,4);if(io_status)return io_status;goto L8c640544; /* mov.l 0x8c640630,r5 */
L8c640544: *stop=0x8c640544u;r[14]=r[4];goto L8c640546; /* mov r4,r14 */
L8c640546: *stop=0x8c640546u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c640548; /* fmov.s fr15,@-r15 */
L8c640548: *stop=0x8c640548u;r[0]=88u;goto L8c64054a; /* mov #0x58,r0 */
L8c64054a: *stop=0x8c64054au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c64054c; /* fmov.s fr14,@-r15 */
L8c64054c: *stop=0x8c64054cu;if(!valid_access(r[0]+r[14],4u))return -3;r[2]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c64054e; /* mov.l @(r0,r14),r2 */
L8c64054e: *stop=0x8c64054eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c640550; /* fmov.s fr13,@-r15 */
L8c640550: *stop=0x8c640550u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c640552; /* fmov.s fr12,@-r15 */
L8c640552: *stop=0x8c640552u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c640554; /* sts.l PR,@-r15 */
L8c640554: *stop=0x8c640554u;r[6]=r[5];goto L8c640556; /* mov r5,r6 */
L8c640556: *stop=0x8c640556u;r[4]+=64u;goto L8c640558; /* add #0x40,r4 */
L8c640558: *stop=0x8c640558u;r[6]+=32u;goto L8c64055a; /* add #0x20,r6 */
L8c64055a: *stop=0x8c64055au;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64055c; /* fschg */
L8c64055c: *stop=0x8c64055cu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64055e; /* fmov.s @r6+,fr0 */
L8c64055e: *stop=0x8c64055eu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c640560; /* fmov.s @r6+,fr2 */
L8c640560: *stop=0x8c640560u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c640562; /* fmov.s @r6+,fr4 */
L8c640562: *stop=0x8c640562u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c640564; /* fmov.s @r6+,fr6 */
L8c640564: *stop=0x8c640564u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c640566; /* fmov.s @r5+,fr8 */
L8c640566: *stop=0x8c640566u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c640568; /* fmov.s @r5+,fr10 */
L8c640568: *stop=0x8c640568u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64056a; /* fschg */
L8c64056a: *stop=0x8c64056au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c64056c; /* fmov.s fr7,@-r4 */
L8c64056c: *stop=0x8c64056cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c64056e; /* fmov.s fr6,@-r4 */
L8c64056e: *stop=0x8c64056eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c640570; /* fmov.s fr5,@-r4 */
L8c640570: *stop=0x8c640570u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c640572; /* fmov.s fr4,@-r4 */
L8c640572: *stop=0x8c640572u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c640574; /* fmov.s fr3,@-r4 */
L8c640574: *stop=0x8c640574u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c640576; /* fmov.s fr2,@-r4 */
L8c640576: *stop=0x8c640576u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;goto L8c640578; /* fmov.s fr1,@-r4 */
L8c640578: *stop=0x8c640578u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c64057a; /* fmov.s fr0,@-r4 */
L8c64057a: *stop=0x8c64057au;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64057c; /* fschg */
L8c64057c: *stop=0x8c64057cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64057e; /* fmov.s @r5+,fr6 */
L8c64057e: *stop=0x8c64057eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c640580; /* fmov.s @r5+,fr4 */
L8c640580: *stop=0x8c640580u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c640582; /* fschg */
L8c640582: *stop=0x8c640582u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[4]);if(io_status)return io_status;goto L8c640584; /* fmov.s fr5,@-r4 */
L8c640584: *stop=0x8c640584u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c640586; /* fmov.s fr4,@-r4 */
L8c640586: *stop=0x8c640586u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;goto L8c640588; /* fmov.s fr7,@-r4 */
L8c640588: *stop=0x8c640588u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c64058a; /* fmov.s fr6,@-r4 */
L8c64058a: *stop=0x8c64058au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;goto L8c64058c; /* fmov.s fr11,@-r4 */
L8c64058c: *stop=0x8c64058cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;goto L8c64058e; /* fmov.s fr10,@-r4 */
L8c64058e: *stop=0x8c64058eu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c640590; /* fmov.s fr9,@-r4 */
L8c640590: *stop=0x8c640590u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;goto L8c640592; /* fmov.s fr8,@-r4 */
L8c640592: *stop=0x8c640592u;r[3]=rd(m,bus,&io_status,0x8c640634u,4);if(io_status)return io_status;goto L8c640594; /* mov.l 0x8c640634,r3 */
L8c640594: *stop=0x8c640594u;r[5]=0u-r[2];goto L8c640596; /* neg r2,r5 */
L8c640596: target=r[3];*pr=0x8c64059au;*stop=0x8c640598u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64059a; /* jsr @r3 */
L8c64059a: *stop=0x8c64059au;r[3]=r[14];goto L8c64059c; /* mov r14,r3 */
L8c64059c: *stop=0x8c64059cu;r[3]+=76u;goto L8c64059e; /* add #0x4c,r3 */
L8c64059e: *stop=0x8c64059eu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6405a0; /* fmov.s @r3+,fr12 */
L8c6405a0: *stop=0x8c6405a0u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6405a2; /* fmov.s @r3+,fr13 */
L8c6405a2: *stop=0x8c6405a2u;fcopy(f,15u,12u);goto L8c6405a4; /* fmov fr12,fr15 */
L8c6405a4: *stop=0x8c6405a4u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6405a6; /* fmov.s @r3+,fr14 */
L8c6405a6: *stop=0x8c6405a6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[12],&f->fpscr,&fr[15]);if(status)return status;}goto L8c6405a8; /* fmul fr12,fr15 */
L8c6405a8: *stop=0x8c6405a8u;fcopy(f,0u,14u);goto L8c6405aa; /* fmov fr14,fr0 */
L8c6405aa: *stop=0x8c6405aau;fcopy(f,11u,13u);goto L8c6405ac; /* fmov fr13,fr11 */
L8c6405ac: *stop=0x8c6405acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[14],&fr[15],&f->fpscr,&fr[15]);if(status)return status;}goto L8c6405ae; /* fmac fr0,fr14,fr15 */
L8c6405ae: *stop=0x8c6405aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[15],&f->fpscr,&fr[15]);if(status)return status;}goto L8c6405b0; /* fsqrt fr15 */
L8c6405b0: *stop=0x8c6405b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[13],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6405b2; /* fmul fr13,fr11 */
L8c6405b2: *stop=0x8c6405b2u;fcopy(f,0u,15u);goto L8c6405b4; /* fmov fr15,fr0 */
L8c6405b4: *stop=0x8c6405b4u;r[3]=rd(m,bus,&io_status,0x8c640638u,4);if(io_status)return io_status;goto L8c6405b6; /* mov.l 0x8c640638,r3 */
L8c6405b6: *stop=0x8c6405b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[15],&fr[11],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6405b8; /* fmac fr0,fr15,fr11 */
L8c6405b8: *stop=0x8c6405b8u;status=sa_sh4_vector_numeric(0xfb7du,f,*fpul);if(status)return status;goto L8c6405ba; /* fsrra fr11 */
L8c6405ba: *stop=0x8c6405bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[15],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6405bc; /* fmul fr15,fr11 */
L8c6405bc: target=r[3];*pr=0x8c6405c0u;*stop=0x8c6405beu;fcopy(f,4u,11u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6405c0; /* jsr @r3 */
L8c6405c0: *stop=0x8c6405c0u;r[0]=0x8c640654u;goto L8c6405c2; /* mova 0x8c640654,r0 */
L8c6405c2: *stop=0x8c6405c2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c6405c4; /* fmov.s @r0,fr3 */
L8c6405c4: *stop=0x8c6405c4u;r[2]=rd(m,bus,&io_status,0x8c64063cu,4);if(io_status)return io_status;goto L8c6405c6; /* mov.l 0x8c64063c,r2 */
L8c6405c6: *stop=0x8c6405c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6405c8; /* fmul fr0,fr3 */
L8c6405c8: *stop=0x8c6405c8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c6405ca; /* fldi0 fr2 */
L8c6405ca: *stop=0x8c6405cau;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6405cc; /* ftrc fr3,FPUL */
L8c6405cc: *stop=0x8c6405ccu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][13],&f->fpscr,sr,1);if(status)return status;goto L8c6405ce; /* fcmp/gt fr13,fr2 */
L8c6405ce: *stop=0x8c6405ceu;r[1]=*fpul;goto L8c6405d0; /* sts FPUL,r1 */
L8c6405d0: *stop=0x8c6405d0u;r[1]&=r[2];goto L8c6405d2; /* and r2,r1 */
L8c6405d2: if((*sr&1u))goto L8c6405d6;goto L8c6405d4; /* bt 0x8c6405d6 */
L8c6405d4: *stop=0x8c6405d4u;r[1]=0u-r[1];goto L8c6405d6; /* neg r1,r1 */
L8c6405d6: *stop=0x8c6405d6u;r[3]=rd(m,bus,&io_status,0x8c640640u,4);if(io_status)return io_status;goto L8c6405d8; /* mov.l 0x8c640640,r3 */
L8c6405d8: *stop=0x8c6405d8u;r[5]=0u-r[1];goto L8c6405da; /* neg r1,r5 */
L8c6405da: target=r[3];*pr=0x8c6405deu;*stop=0x8c6405dcu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6405de; /* jsr @r3 */
L8c6405de: *stop=0x8c6405deu;r[3]=rd(m,bus,&io_status,0x8c640638u,4);if(io_status)return io_status;goto L8c6405e0; /* mov.l 0x8c640638,r3 */
L8c6405e0: *stop=0x8c6405e0u;fcopy(f,4u,14u);goto L8c6405e2; /* fmov fr14,fr4 */
L8c6405e2: target=r[3];*pr=0x8c6405e6u;*stop=0x8c6405e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6405e6; /* jsr @r3 */
L8c6405e6: *stop=0x8c6405e6u;r[0]=0x8c640654u;goto L8c6405e8; /* mova 0x8c640654,r0 */
L8c6405e8: *stop=0x8c6405e8u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c6405ea; /* fmov.s @r0,fr3 */
L8c6405ea: *stop=0x8c6405eau;r[2]=rd(m,bus,&io_status,0x8c64063cu,4);if(io_status)return io_status;goto L8c6405ec; /* mov.l 0x8c64063c,r2 */
L8c6405ec: *stop=0x8c6405ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6405ee; /* fmul fr0,fr3 */
L8c6405ee: *stop=0x8c6405eeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c6405f0; /* fldi0 fr2 */
L8c6405f0: *stop=0x8c6405f0u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6405f2; /* ftrc fr3,FPUL */
L8c6405f2: *stop=0x8c6405f2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][12],&f->fpscr,sr,1);if(status)return status;goto L8c6405f4; /* fcmp/gt fr12,fr2 */
L8c6405f4: *stop=0x8c6405f4u;r[1]=*fpul;goto L8c6405f6; /* sts FPUL,r1 */
L8c6405f6: *stop=0x8c6405f6u;r[1]&=r[2];goto L8c6405f8; /* and r2,r1 */
L8c6405f8: if(!(*sr&1u))goto L8c6405fc;goto L8c6405fa; /* bf 0x8c6405fc */
L8c6405fa: *stop=0x8c6405fau;r[1]=0u-r[1];goto L8c6405fc; /* neg r1,r1 */
L8c6405fc: *stop=0x8c6405fcu;r[3]=rd(m,bus,&io_status,0x8c640644u,4);if(io_status)return io_status;goto L8c6405fe; /* mov.l 0x8c640644,r3 */
L8c6405fe: *stop=0x8c6405feu;r[5]=0u-r[1];goto L8c640600; /* neg r1,r5 */
L8c640600: target=r[3];*pr=0x8c640604u;*stop=0x8c640602u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c640604; /* jsr @r3 */
L8c640604: *stop=0x8c640604u;r[3]=rd(m,bus,&io_status,0x8c640648u,4);if(io_status)return io_status;goto L8c640606; /* mov.l 0x8c640648,r3 */
L8c640606: *stop=0x8c640606u;r[0]=72u;goto L8c640608; /* mov #0x48,r0 */
L8c640608: *stop=0x8c640608u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[14]);if(io_status)return io_status;goto L8c64060a; /* fmov.s @(r0,r14),fr6 */
L8c64060a: *stop=0x8c64060au;r[0]=68u;goto L8c64060c; /* mov #0x44,r0 */
L8c64060c: *stop=0x8c64060cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c64060e; /* fmov.s @(r0,r14),fr5 */
L8c64060e: *stop=0x8c64060eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]^=0x80000000u;goto L8c640610; /* fneg fr6 */
L8c640610: *stop=0x8c640610u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]^=0x80000000u;goto L8c640612; /* fneg fr5 */
L8c640612: *stop=0x8c640612u;r[0]=64u;goto L8c640614; /* mov #0x40,r0 */
L8c640614: *stop=0x8c640614u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c640616; /* fmov.s @(r0,r14),fr4 */
L8c640616: *stop=0x8c640616u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c640618; /* fneg fr4 */
L8c640618: target=r[3];*pr=0x8c64061cu;*stop=0x8c64061au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64061c; /* jsr @r3 */
L8c64061c: *stop=0x8c64061cu;r[2]=rd(m,bus,&io_status,0x8c64064cu,4);if(io_status)return io_status;goto L8c64061e; /* mov.l 0x8c64064c,r2 */
L8c64061e: *stop=0x8c64061eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c640620; /* mov.l r14,@r2 */
L8c640620: *stop=0x8c640620u;r[3]=rd(m,bus,&io_status,0x8c640650u,4);if(io_status)return io_status;goto L8c640622; /* mov.l 0x8c640650,r3 */
L8c640622: *stop=0x8c640622u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c640624; /* lds.l @r15+,PR */
L8c640624: *stop=0x8c640624u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c640626; /* fmov.s @r15+,fr12 */
L8c640626: *stop=0x8c640626u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c640628; /* fmov.s @r15+,fr13 */
L8c640628: *stop=0x8c640628u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64062a; /* fmov.s @r15+,fr14 */
L8c64062a: *stop=0x8c64062au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64062c; /* fmov.s @r15+,fr15 */
L8c64062c: target=r[3];*stop=0x8c64062eu;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_frame_setup_8c64075a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64075a;
L8c64075a: *stop=0x8c64075au;r[4]=r[4]&65535u;goto L8c64075c; /* extu.w r4,r4 */
L8c64075c: *stop=0x8c64075cu;r[3]=rd(m,bus,&io_status,0x8c64077cu,4);if(io_status)return io_status;goto L8c64075e; /* mov.l 0x8c64077c,r3 */
L8c64075e: *stop=0x8c64075eu;*fpul=r[4];goto L8c640760; /* lds r4,FPUL */
L8c640760: *stop=0x8c640760u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c640762; /* float FPUL,fr3 */
L8c640762: *stop=0x8c640762u;if(*pr!=entry_pr)return -3;*stop=0x8c640764u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c63a9f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63a9f4;
L8c63a9f4: *stop=0x8c63a9f4u;r[4]=r[4]&65535u;goto L8c63a9f6; /* extu.w r4,r4 */
L8c63a9f6: *stop=0x8c63a9f6u;*fpul=r[4];goto L8c63a9f8; /* lds r4,FPUL */
L8c63a9f8: *stop=0x8c63a9f8u;status=sa_sh4_vector_numeric(0xf0fdu,f,*fpul);if(status)return status;goto L8c63a9fa; /* fsca FPUL,dr0 */
L8c63a9fa: *stop=0x8c63a9fau;if(*pr!=entry_pr)return -3;*stop=0x8c63a9fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[0],&fr[1],&f->fpscr,&fr[0]);if(status)return status;}return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c10d008(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d008;
L8c10d008: *stop=0x8c10d008u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10d00a; /* sts.l PR,@-r15 */
L8c10d00a: *stop=0x8c10d00au;r[15]+=4294967292u;goto L8c10d00c; /* add #-0x4,r15 */
L8c10d00c: *stop=0x8c10d00cu;r[3]=rd(m,bus,&io_status,0x8c10d048u,4);if(io_status)return io_status;goto L8c10d00e; /* mov.l 0x8c10d048,r3 */
L8c10d00e: target=r[3];*pr=0x8c10d012u;*stop=0x8c10d010u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d012; /* jsr @r3 */
L8c10d012: *stop=0x8c10d012u;r[3]=r[15];goto L8c10d014; /* mov r15,r3 */
L8c10d014: *stop=0x8c10d014u;r[4]=rd(m,bus,&io_status,0x8c10d04cu,4);if(io_status)return io_status;goto L8c10d016; /* mov.l 0x8c10d04c,r4 */
L8c10d016: *stop=0x8c10d016u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c10d018; /* fmov.s fr0,@r15 */
L8c10d018: *stop=0x8c10d018u;if(!valid_access(r[3],4u))return -3;r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c10d01a; /* mov.l @r3,r2 */
L8c10d01a: *stop=0x8c10d01au;r[2]&=r[4];goto L8c10d01c; /* and r4,r2 */
L8c10d01c: *stop=0x8c10d01cu;*sr=(*sr&~1u)|(r[4]==r[2]);goto L8c10d01e; /* cmp/eq r4,r2 */
L8c10d01e: if(!(*sr&1u))goto L8c10d038;goto L8c10d020; /* bf 0x8c10d038 */
L8c10d020: *stop=0x8c10d020u;r[2]=r[15];goto L8c10d022; /* mov r15,r2 */
L8c10d022: *stop=0x8c10d022u;if(!valid_access(r[2],4u))return -3;r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c10d024; /* mov.l @r2,r1 */
L8c10d024: *stop=0x8c10d024u;r[3]=rd(m,bus,&io_status,0x8c10d050u,4);if(io_status)return io_status;goto L8c10d026; /* mov.l 0x8c10d050,r3 */
L8c10d026: *stop=0x8c10d026u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c10d028; /* tst r3,r1 */
L8c10d028: if((*sr&1u))goto L8c10d032;goto L8c10d02a; /* bt 0x8c10d032 */
L8c10d02a: *stop=0x8c10d02au;r[2]=rd(m,bus,&io_status,0x8c10d054u,4);if(io_status)return io_status;goto L8c10d02c; /* mov.l 0x8c10d054,r2 */
L8c10d02c: *stop=0x8c10d02cu;r[0]=sx(m,bus,&io_status,0x8c10d042u,2);if(io_status)return io_status;goto L8c10d02e; /* mov.w 0x8c10d042,r0 */
L8c10d02e: *stop=0x8c10d030u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c10d038; /* bra 0x8c10d038 */
L8c10d032: *stop=0x8c10d032u;r[1]=rd(m,bus,&io_status,0x8c10d054u,4);if(io_status)return io_status;goto L8c10d034; /* mov.l 0x8c10d054,r1 */
L8c10d034: *stop=0x8c10d034u;r[2]=sx(m,bus,&io_status,0x8c10d044u,2);if(io_status)return io_status;goto L8c10d036; /* mov.w 0x8c10d044,r2 */
L8c10d036: *stop=0x8c10d036u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c10d038; /* mov.l r2,@r1 */
L8c10d038: *stop=0x8c10d038u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c10d03a; /* fmov.s @r15,fr0 */
L8c10d03a: *stop=0x8c10d03au;r[15]+=4u;goto L8c10d03c; /* add #0x4,r15 */
L8c10d03c: *stop=0x8c10d03cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10d03e; /* lds.l @r15+,PR */
L8c10d03e: *stop=0x8c10d03eu;if(*pr!=entry_pr)return -3;*stop=0x8c10d040u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c64f334(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64f334;
L8c64f334: *stop=0x8c64f334u;r[1]=1u;goto L8c64f336; /* mov #0x1,r1 */
L8c64f336: *stop=0x8c64f336u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64f338; /* mov.l r14,@-r15 */
L8c64f338: *stop=0x8c64f338u;r[0]=rd(m,bus,&io_status,0x8c64f3c8u,4);if(io_status)return io_status;goto L8c64f33a; /* mov.l 0x8c64f3c8,r0 */
L8c64f33a: *stop=0x8c64f33au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64f33c; /* sts.l PR,@-r15 */
L8c64f33c: *stop=0x8c64f33cu;r[14]=rd(m,bus,&io_status,0x8c64f3ccu,4);if(io_status)return io_status;goto L8c64f33e; /* mov.l 0x8c64f3cc,r14 */
L8c64f33e: *stop=0x8c64f33eu;r[5]=rd(m,bus,&io_status,0x8c64f3d0u,4);if(io_status)return io_status;goto L8c64f340; /* mov.l 0x8c64f3d0,r5 */
L8c64f340: *stop=0x8c64f340u;if(!valid_access(r[14],4u))return -3;r[14]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c64f342; /* mov.l @r14,r14 */
L8c64f342: *stop=0x8c64f342u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c64f344; /* mov.l r1,@r0 */
L8c64f344: *stop=0x8c64f344u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f346; /* fschg */
L8c64f346: *stop=0x8c64f346u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[14]);if(io_status)return io_status;r[14]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f348; /* fmov.s @r14+,fr1 */
L8c64f348: *stop=0x8c64f348u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;r[14]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f34a; /* fmov.s @r14+,fr3 */
L8c64f34a: *stop=0x8c64f34au;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;r[14]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f34c; /* fmov.s @r14+,fr5 */
L8c64f34c: *stop=0x8c64f34cu;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[14]);if(io_status)return io_status;r[14]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f34e; /* fmov.s @r14+,fr7 */
L8c64f34e: *stop=0x8c64f34eu;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[14]);if(io_status)return io_status;r[14]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f350; /* fmov.s @r14+,fr9 */
L8c64f350: *stop=0x8c64f350u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[14]);if(io_status)return io_status;r[14]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f352; /* fmov.s @r14+,fr11 */
L8c64f352: *stop=0x8c64f352u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[14]);if(io_status)return io_status;r[14]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f354; /* fmov.s @r14+,fr13 */
L8c64f354: *stop=0x8c64f354u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[14]);if(io_status)return io_status;r[14]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f356; /* fmov.s @r14+,fr15 */
L8c64f356: *stop=0x8c64f356u;r[5]+=32u;goto L8c64f358; /* add #0x20,r5 */
L8c64f358: *stop=0x8c64f358u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f35a; /* fmov.s @r5+,fr0 */
L8c64f35a: *stop=0x8c64f35au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f35c; /* fmov.s @r5+,fr2 */
L8c64f35c: *stop=0x8c64f35cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f35e; /* fmov.s @r5+,fr4 */
L8c64f35e: *stop=0x8c64f35eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f360; /* fmov.s @r5+,fr6 */
L8c64f360: *stop=0x8c64f360u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c64f362; /* fmov.s fr6,@-r14 */
L8c64f362: *stop=0x8c64f362u;r[5]+=4294967232u;goto L8c64f364; /* add #-0x40,r5 */
L8c64f364: *stop=0x8c64f364u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c64f366; /* fmov.s fr4,@-r14 */
L8c64f366: *stop=0x8c64f366u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c64f368; /* fmov.s fr2,@-r14 */
L8c64f368: *stop=0x8c64f368u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c64f36a; /* fmov.s fr0,@-r14 */
L8c64f36a: *stop=0x8c64f36au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f36c; /* fmov.s @r5+,fr0 */
L8c64f36c: *stop=0x8c64f36cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f36e; /* fmov.s @r5+,fr2 */
L8c64f36e: *stop=0x8c64f36eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f370; /* fmov.s @r5+,fr4 */
L8c64f370: *stop=0x8c64f370u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f372; /* fmov.s @r5+,fr6 */
L8c64f372: *stop=0x8c64f372u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c64f374; /* fmov.s fr6,@-r14 */
L8c64f374: *stop=0x8c64f374u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c64f376; /* fmov.s fr4,@-r14 */
L8c64f376: *stop=0x8c64f376u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c64f378; /* fmov.s fr2,@-r14 */
L8c64f378: *stop=0x8c64f378u;if(!valid_access(r[14]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[14]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c64f37a; /* fmov.s fr0,@-r14 */
L8c64f37a: *stop=0x8c64f37au;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f37c; /* fschg */
L8c64f37c: *stop=0x8c64f37cu;r[2]=rd(m,bus,&io_status,0x8c64f3d4u,4);if(io_status)return io_status;goto L8c64f37e; /* mov.l 0x8c64f3d4,r2 */
L8c64f37e: *stop=0x8c64f37eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c64f380; /* mov.l r14,@r2 */
L8c64f380: *stop=0x8c64f380u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c64f382; /* fldi1 fr4 */
L8c64f382: *stop=0x8c64f382u;r[3]=rd(m,bus,&io_status,0x8c64f3d8u,4);if(io_status)return io_status;goto L8c64f384; /* mov.l 0x8c64f3d8,r3 */
L8c64f384: *stop=0x8c64f384u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c64f386; /* fneg fr4 */
L8c64f386: *stop=0x8c64f386u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c64f388; /* fldi1 fr6 */
L8c64f388: *stop=0x8c64f388u;fcopy(f,5u,4u);goto L8c64f38a; /* fmov fr4,fr5 */
L8c64f38a: target=r[3];*pr=0x8c64f38eu;*stop=0x8c64f38cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f38e; /* jsr @r3 */
L8c64f38e: *stop=0x8c64f38eu;r[3]=rd(m,bus,&io_status,0x8c64f3dcu,4);if(io_status)return io_status;goto L8c64f390; /* mov.l 0x8c64f3dc,r3 */
L8c64f390: *stop=0x8c64f390u;r[2]=rd(m,bus,&io_status,0x8c64f3e0u,4);if(io_status)return io_status;goto L8c64f392; /* mov.l 0x8c64f3e0,r2 */
L8c64f392: *stop=0x8c64f392u;if(!valid_access(r[3],4u))return -3;r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c64f394; /* mov.l @r3,r5 */
L8c64f394: target=r[2];*pr=0x8c64f398u;*stop=0x8c64f396u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64f398; /* jsr @r2 */
L8c64f398: *stop=0x8c64f398u;r[2]=rd(m,bus,&io_status,0x8c64f3e4u,4);if(io_status)return io_status;goto L8c64f39a; /* mov.l 0x8c64f3e4,r2 */
L8c64f39a: *stop=0x8c64f39au;r[5]=r[14];goto L8c64f39c; /* mov r14,r5 */
L8c64f39c: *stop=0x8c64f39cu;r[2]+=64u;goto L8c64f39e; /* add #0x40,r2 */
L8c64f39e: *stop=0x8c64f39eu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f3a0; /* fschg */
L8c64f3a0: *stop=0x8c64f3a0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f3a2; /* fmov.s @r5+,fr1 */
L8c64f3a2: *stop=0x8c64f3a2u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f3a4; /* fmov.s @r5+,fr3 */
L8c64f3a4: *stop=0x8c64f3a4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f3a6; /* fmov.s @r5+,fr5 */
L8c64f3a6: *stop=0x8c64f3a6u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f3a8; /* fmov.s @r5+,fr7 */
L8c64f3a8: *stop=0x8c64f3a8u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f3aa; /* fmov.s @r5+,fr9 */
L8c64f3aa: *stop=0x8c64f3aau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f3ac; /* fmov.s @r5+,fr11 */
L8c64f3ac: *stop=0x8c64f3acu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f3ae; /* fmov.s @r5+,fr13 */
L8c64f3ae: *stop=0x8c64f3aeu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f3b0; /* fmov.s @r5+,fr15 */
L8c64f3b0: *stop=0x8c64f3b0u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[2]);if(io_status)return io_status;goto L8c64f3b2; /* fmov.s fr15,@-r2 */
L8c64f3b2: *stop=0x8c64f3b2u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[2]);if(io_status)return io_status;goto L8c64f3b4; /* fmov.s fr13,@-r2 */
L8c64f3b4: *stop=0x8c64f3b4u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[2]);if(io_status)return io_status;goto L8c64f3b6; /* fmov.s fr11,@-r2 */
L8c64f3b6: *stop=0x8c64f3b6u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[2]);if(io_status)return io_status;goto L8c64f3b8; /* fmov.s fr9,@-r2 */
L8c64f3b8: *stop=0x8c64f3b8u;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[2]);if(io_status)return io_status;goto L8c64f3ba; /* fmov.s fr7,@-r2 */
L8c64f3ba: *stop=0x8c64f3bau;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[2]);if(io_status)return io_status;goto L8c64f3bc; /* fmov.s fr5,@-r2 */
L8c64f3bc: *stop=0x8c64f3bcu;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c64f3be; /* fmov.s fr3,@-r2 */
L8c64f3be: *stop=0x8c64f3beu;if(!valid_access(r[2]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c64f3c0; /* fmov.s fr1,@-r2 */
L8c64f3c0: *stop=0x8c64f3c0u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f3c2; /* fschg */
L8c64f3c2: *stop=0x8c64f3c2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64f3c4; /* lds.l @r15+,PR */
L8c64f3c4: *stop=0x8c64f3c4u;if(*pr!=entry_pr)return -3;*stop=0x8c64f3c6u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c64e5e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64e5e0;
L8c64e5e0: *stop=0x8c64e5e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64e5e2; /* mov.l r14,@-r15 */
L8c64e5e2: *stop=0x8c64e5e2u;r[7]=4294967295u;goto L8c64e5e4; /* mov #-0x1,r7 */
L8c64e5e4: *stop=0x8c64e5e4u;r[14]=rd(m,bus,&io_status,0x8c64e814u,4);if(io_status)return io_status;goto L8c64e5e6; /* mov.l 0x8c64e814,r14 */
L8c64e5e6: *stop=0x8c64e5e6u;r[6]=rd(m,bus,&io_status,0x8c64e810u,4);if(io_status)return io_status;goto L8c64e5e8; /* mov.l 0x8c64e810,r6 */
L8c64e5e8: *stop=0x8c64e5e8u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[4],4);if(io_status)return io_status;goto L8c64e5ea; /* mov.l r4,@r6 */
L8c64e5ea: *stop=0x8c64e5eau;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[5],4);if(io_status)return io_status;goto L8c64e5ec; /* mov.l r5,@r14 */
L8c64e5ec: *stop=0x8c64e5ecu;r[5]=0u;goto L8c64e5ee; /* mov #0x0,r5 */
L8c64e5ee: *stop=0x8c64e5eeu;r[1]=r[5];goto L8c64e5f0; /* mov r5,r1 */
L8c64e5f0: *stop=0x8c64e5f2u;r[4]=r[5];goto L8c64e60c; /* bra 0x8c64e60c */
L8c64e5f4: *stop=0x8c64e5f4u;if(!valid_access(r[6],4u))return -3;r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c64e5f6; /* mov.l @r6,r2 */
L8c64e5f6: *stop=0x8c64e5f6u;r[0]=64u;goto L8c64e5f8; /* mov #0x40,r0 */
L8c64e5f8: *stop=0x8c64e5f8u;r[1]+=1u;goto L8c64e5fa; /* add #0x1,r1 */
L8c64e5fa: *stop=0x8c64e5fau;r[2]+=r[4];goto L8c64e5fc; /* add r4,r2 */
L8c64e5fc: *stop=0x8c64e5fcu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[7],4);if(io_status)return io_status;goto L8c64e5fe; /* mov.l r7,@r2 */
L8c64e5fe: *stop=0x8c64e5feu;if(!valid_access(r[6],4u))return -3;r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c64e600; /* mov.l @r6,r3 */
L8c64e600: *stop=0x8c64e600u;r[3]+=r[4];goto L8c64e602; /* add r4,r3 */
L8c64e602: *stop=0x8c64e602u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[7],4);if(io_status)return io_status;goto L8c64e604; /* mov.l r7,@(0x4,r3) */
L8c64e604: *stop=0x8c64e604u;if(!valid_access(r[6],4u))return -3;r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c64e606; /* mov.l @r6,r2 */
L8c64e606: *stop=0x8c64e606u;r[2]+=r[4];goto L8c64e608; /* add r4,r2 */
L8c64e608: *stop=0x8c64e608u;r[4]+=68u;goto L8c64e60a; /* add #0x44,r4 */
L8c64e60a: *stop=0x8c64e60au;if(!valid_access(r[0]+r[2],2u))return -3;wr(m,bus,&io_status,r[0]+r[2],r[5],2);if(io_status)return io_status;goto L8c64e60c; /* mov.w r5,@(r0,r2) */
L8c64e60c: *stop=0x8c64e60cu;if(!valid_access(r[14],4u))return -3;r[3]=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;goto L8c64e60e; /* mov.l @r14,r3 */
L8c64e60e: *stop=0x8c64e60eu;*sr=(*sr&~1u)|(r[1]>=r[3]);goto L8c64e610; /* cmp/hs r3,r1 */
L8c64e610: if(!(*sr&1u))goto L8c64e5f4;goto L8c64e612; /* bf 0x8c64e5f4 */
L8c64e612: *stop=0x8c64e612u;r[3]=rd(m,bus,&io_status,0x8c64e81cu,4);if(io_status)return io_status;goto L8c64e614; /* mov.l 0x8c64e81c,r3 */
L8c64e614: *stop=0x8c64e614u;r[1]=rd(m,bus,&io_status,0x8c64e818u,4);if(io_status)return io_status;goto L8c64e616; /* mov.l 0x8c64e818,r1 */
L8c64e616: *stop=0x8c64e616u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[5],4);if(io_status)return io_status;goto L8c64e618; /* mov.l r5,@r1 */
L8c64e618: *stop=0x8c64e618u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c64e61a; /* mov.l r5,@r3 */
L8c64e61a: *stop=0x8c64e61au;if(*pr!=entry_pr)return -3;*stop=0x8c64e61cu;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c653020(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c653020;
L8c653020: *stop=0x8c653020u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c653022; /* sts.l PR,@-r15 */
L8c653022: *stop=0x8c653022u;r[15]+=4294967292u;goto L8c653024; /* add #-0x4,r15 */
L8c653024: *stop=0x8c653024u;r[3]=r[5];goto L8c653026; /* mov r5,r3 */
L8c653026: *stop=0x8c653026u;r[0]=rd(m,bus,&io_status,0x8c6530a0u,4);if(io_status)return io_status;goto L8c653028; /* mov.l 0x8c6530a0,r0 */
L8c653028: *stop=0x8c653028u;r[3]<<=2;goto L8c65302a; /* shll2 r3 */
L8c65302a: *stop=0x8c65302au;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c65302c; /* mov.l r5,@r15 */
L8c65302c: *stop=0x8c65302cu;if(!valid_access(r[0]+r[3],4u))return -3;r[3]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c65302e; /* mov.l @(r0,r3),r3 */
L8c65302e: target=r[3];*pr=0x8c653032u;*stop=0x8c653030u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c653032; /* jsr @r3 */
L8c653032: *stop=0x8c653032u;r[2]=rd(m,bus,&io_status,0x8c6530a8u,4);if(io_status)return io_status;goto L8c653034; /* mov.l 0x8c6530a8,r2 */
L8c653034: *stop=0x8c653034u;r[3]=rd(m,bus,&io_status,0x8c6530a4u,4);if(io_status)return io_status;goto L8c653036; /* mov.l 0x8c6530a4,r3 */
L8c653036: *stop=0x8c653036u;if(!valid_access(r[3],4u))return -3;r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c653038; /* mov.l @r3,r1 */
L8c653038: *stop=0x8c653038u;r[1]&=r[2];goto L8c65303a; /* and r2,r1 */
L8c65303a: *stop=0x8c65303au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c65303c; /* mov.l r1,@r3 */
L8c65303c: *stop=0x8c65303cu;r[15]+=4u;goto L8c65303e; /* add #0x4,r15 */
L8c65303e: *stop=0x8c65303eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c653040; /* lds.l @r15+,PR */
L8c653040: *stop=0x8c653040u;if(*pr!=entry_pr)return -3;*stop=0x8c653042u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c652d6e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c652d6e;
L8c652d6e: *stop=0x8c652d6eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c652d70; /* sts.l PR,@-r15 */
L8c652d70: *stop=0x8c652d70u;r[3]=rd(m,bus,&io_status,0x8c652d9cu,4);if(io_status)return io_status;goto L8c652d72; /* mov.l 0x8c652d9c,r3 */
L8c652d72: *stop=0x8c652d72u;r[4]=sx(m,bus,&io_status,0x8c652d86u,2);if(io_status)return io_status;goto L8c652d74; /* mov.w 0x8c652d86,r4 */
L8c652d74: target=r[3];*pr=0x8c652d78u;*stop=0x8c652d76u;if(!valid_access(r[15]+4u,4u))return -3;r[5]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652d78; /* jsr @r3 */
L8c652d78: *stop=0x8c652d78u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c652d7a; /* lds.l @r15+,PR */
L8c652d7a: *stop=0x8c652d7au;if(*pr!=entry_pr)return -3;*stop=0x8c652d7cu;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c652e38(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c652e38;
L8c652e38: *stop=0x8c652e38u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c652e3a; /* sts.l PR,@-r15 */
L8c652e3a: *stop=0x8c652e3au;r[15]+=4294967292u;goto L8c652e3c; /* add #-0x4,r15 */
L8c652e3c: *stop=0x8c652e3cu;r[5]=r[4];goto L8c652e3e; /* mov r4,r5 */
L8c652e3e: *stop=0x8c652e3eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c652e40; /* mov.l r4,@r15 */
L8c652e40: *stop=0x8c652e40u;r[3]=rd(m,bus,&io_status,0x8c652e6cu,4);if(io_status)return io_status;goto L8c652e42; /* mov.l 0x8c652e6c,r3 */
L8c652e42: *stop=0x8c652e42u;r[4]=sx(m,bus,&io_status,0x8c652e68u,2);if(io_status)return io_status;goto L8c652e44; /* mov.w 0x8c652e68,r4 */
L8c652e44: target=r[3];*pr=0x8c652e48u;*stop=0x8c652e46u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652e48; /* jsr @r3 */
L8c652e48: *stop=0x8c652e48u;r[15]+=4u;goto L8c652e4a; /* add #0x4,r15 */
L8c652e4a: *stop=0x8c652e4au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c652e4c; /* lds.l @r15+,PR */
L8c652e4c: *stop=0x8c652e4cu;if(*pr!=entry_pr)return -3;*stop=0x8c652e4eu;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c652dd0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c652dd0;
L8c652dd0: *stop=0x8c652dd0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c652dd2; /* mov.l r14,@-r15 */
L8c652dd2: *stop=0x8c652dd2u;r[0]=0x8c652e70u;goto L8c652dd4; /* mova 0x8c652e70,r0 */
L8c652dd4: *stop=0x8c652dd4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c652dd6; /* mov.l r13,@-r15 */
L8c652dd6: *stop=0x8c652dd6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c652dd8; /* mov.l r12,@-r15 */
L8c652dd8: *stop=0x8c652dd8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c652dda; /* mov.l r11,@-r15 */
L8c652dda: *stop=0x8c652ddau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c652ddc; /* mov.l r10,@-r15 */
L8c652ddc: *stop=0x8c652ddcu;r[10]=r[4];goto L8c652dde; /* mov r4,r10 */
L8c652dde: *stop=0x8c652ddeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c652de0; /* mov.l r9,@-r15 */
L8c652de0: *stop=0x8c652de0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c652de2; /* fmov.s fr15,@-r15 */
L8c652de2: *stop=0x8c652de2u;r[14]=sx(m,bus,&io_status,0x8c652e62u,2);if(io_status)return io_status;goto L8c652de4; /* mov.w 0x8c652e62,r14 */
L8c652de4: *stop=0x8c652de4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c652de6; /* sts.l PR,@-r15 */
L8c652de6: *stop=0x8c652de6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c652de8; /* fmov.s @r0,fr15 */
L8c652de8: *stop=0x8c652de8u;r[12]=r[14];goto L8c652dea; /* mov r14,r12 */
L8c652dea: *stop=0x8c652deau;r[0]=sx(m,bus,&io_status,0x8c652e64u,2);if(io_status)return io_status;goto L8c652dec; /* mov.w 0x8c652e64,r0 */
L8c652dec: *stop=0x8c652decu;r[12]+=127u;goto L8c652dee; /* add #0x7f,r12 */
L8c652dee: *stop=0x8c652deeu;r[9]=rd(m,bus,&io_status,0x8c652e6cu,4);if(io_status)return io_status;goto L8c652df0; /* mov.l 0x8c652e6c,r9 */
L8c652df0: *stop=0x8c652df0u;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c652df2; /* fmov.s @(r0,r10),fr3 */
L8c652df2: *stop=0x8c652df2u;r[11]=sx(m,bus,&io_status,0x8c652e66u,2);if(io_status)return io_status;goto L8c652df4; /* mov.w 0x8c652e66,r11 */
L8c652df4: *stop=0x8c652df4u;fcopy(f,2u,3u);goto L8c652df6; /* fmov fr3,fr2 */
L8c652df6: *stop=0x8c652df6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[15],&f->fpscr,&fr[2]);if(status)return status;}goto L8c652df8; /* fmul fr15,fr2 */
L8c652df8: *stop=0x8c652df8u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c652dfa; /* ftrc fr2,FPUL */
L8c652dfa: *stop=0x8c652dfau;r[13]=*fpul;goto L8c652dfc; /* sts FPUL,r13 */
L8c652dfc: *stop=0x8c652dfcu;r[13]&=r[12];goto L8c652dfe; /* and r12,r13 */
L8c652dfe: *stop=0x8c652dfeu;r[14]+=4294967295u;goto L8c652e00; /* add #-0x1,r14 */
L8c652e00: *stop=0x8c652e00u;r[4]=r[13];goto L8c652e02; /* mov r13,r4 */
L8c652e02: *stop=0x8c652e02u;r[0]=r[14];goto L8c652e04; /* mov r14,r0 */
L8c652e04: *stop=0x8c652e04u;r[0]<<=2;goto L8c652e06; /* shll2 r0 */
L8c652e06: *stop=0x8c652e06u;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[10]);if(io_status)return io_status;goto L8c652e08; /* fmov.s @(r0,r10),fr2 */
L8c652e08: *stop=0x8c652e08u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[15],&f->fpscr,&fr[2]);if(status)return status;}goto L8c652e0a; /* fmul fr15,fr2 */
L8c652e0a: *stop=0x8c652e0au;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c652e0c; /* ftrc fr2,FPUL */
L8c652e0c: *stop=0x8c652e0cu;r[13]=*fpul;goto L8c652e0e; /* sts FPUL,r13 */
L8c652e0e: *stop=0x8c652e0eu;r[13]&=r[12];goto L8c652e10; /* and r12,r13 */
L8c652e10: *stop=0x8c652e10u;r[5]=r[13];goto L8c652e12; /* mov r13,r5 */
L8c652e12: *stop=0x8c652e12u;r[5]<<=8;goto L8c652e14; /* shll8 r5 */
L8c652e14: *stop=0x8c652e14u;r[5]|=r[4];goto L8c652e16; /* or r4,r5 */
L8c652e16: *stop=0x8c652e16u;r[4]=r[14];goto L8c652e18; /* mov r14,r4 */
L8c652e18: *stop=0x8c652e18u;r[4]<<=2;goto L8c652e1a; /* shll2 r4 */
L8c652e1a: *stop=0x8c652e1au;r[5]=r[5]&65535u;goto L8c652e1c; /* extu.w r5,r5 */
L8c652e1c: target=r[9];*pr=0x8c652e20u;*stop=0x8c652e1eu;r[4]+=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652e20; /* jsr @r9 */
L8c652e20: *stop=0x8c652e20u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c652e22; /* tst r14,r14 */
L8c652e22: if(!(*sr&1u))goto L8c652dfe;goto L8c652e24; /* bf 0x8c652dfe */
L8c652e24: *stop=0x8c652e24u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c652e26; /* lds.l @r15+,PR */
L8c652e26: *stop=0x8c652e26u;r[0]=0u;goto L8c652e28; /* mov #0x0,r0 */
L8c652e28: *stop=0x8c652e28u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c652e2a; /* fmov.s @r15+,fr15 */
L8c652e2a: *stop=0x8c652e2au;if(!valid_access(r[15],4u))return -3;r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c652e2c; /* mov.l @r15+,r9 */
L8c652e2c: *stop=0x8c652e2cu;if(!valid_access(r[15],4u))return -3;r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c652e2e; /* mov.l @r15+,r10 */
L8c652e2e: *stop=0x8c652e2eu;if(!valid_access(r[15],4u))return -3;r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c652e30; /* mov.l @r15+,r11 */
L8c652e30: *stop=0x8c652e30u;if(!valid_access(r[15],4u))return -3;r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c652e32; /* mov.l @r15+,r12 */
L8c652e32: *stop=0x8c652e32u;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c652e34; /* mov.l @r15+,r13 */
L8c652e34: *stop=0x8c652e34u;if(*pr!=entry_pr)return -3;*stop=0x8c652e36u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c10e590(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10e590;
L8c10e590: *stop=0x8c10e590u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10e592; /* sts.l PR,@-r15 */
L8c10e592: *stop=0x8c10e592u;r[3]=rd(m,bus,&io_status,0x8c10e5a4u,4);if(io_status)return io_status;goto L8c10e594; /* mov.l 0x8c10e5a4,r3 */
L8c10e594: target=r[3];*pr=0x8c10e598u;*stop=0x8c10e596u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e598; /* jsr @r3 */
L8c10e598: *stop=0x8c10e598u;r[3]=rd(m,bus,&io_status,0x8c10e5a8u,4);if(io_status)return io_status;goto L8c10e59a; /* mov.l 0x8c10e5a8,r3 */
L8c10e59a: *stop=0x8c10e59au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10e59c; /* lds.l @r15+,PR */
L8c10e59c: *stop=0x8c10e59cu;fcopy(f,3u,0u);goto L8c10e59e; /* fmov fr0,fr3 */
L8c10e59e: *stop=0x8c10e59eu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[3]);if(io_status)return io_status;goto L8c10e5a0; /* fmov.s @r3,fr0 */
L8c10e5a0: *stop=0x8c10e5a0u;if(*pr!=entry_pr)return -3;*stop=0x8c10e5a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c63997c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c63997c;
L8c63997c: *stop=0x8c63997cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c63997e; /* tst r4,r4 */
L8c63997e: if(!(*sr&1u))goto L8c6399e2;goto L8c639980; /* bf 0x8c6399e2 */
L8c639980: *stop=0x8c639980u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639982; /* fmov.s @r5+,fr0 */
L8c639982: *stop=0x8c639982u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639984; /* fmov.s @r5+,fr1 */
L8c639984: *stop=0x8c639984u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639986; /* fmov.s @r5+,fr2 */
L8c639986: *stop=0x8c639986u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639988; /* fmov.s @r5+,fr3 */
L8c639988: *stop=0x8c639988u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63998a; /* fmov.s @r5+,fr4 */
L8c63998a: *stop=0x8c63998au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63998c; /* fmov.s @r5+,fr5 */
L8c63998c: *stop=0x8c63998cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63998e; /* fmov.s @r5+,fr6 */
L8c63998e: *stop=0x8c63998eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639990; /* fmov.s @r5+,fr7 */
L8c639990: *stop=0x8c639990u;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c639992; /* ftrv xmtrx,fv0 */
L8c639992: *stop=0x8c639992u;status=sa_sh4_vector_numeric(0xf5fdu,f,*fpul);if(status)return status;goto L8c639994; /* ftrv xmtrx,fv4 */
L8c639994: *stop=0x8c639994u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c639996; /* fmov.s fr7,@-r15 */
L8c639996: *stop=0x8c639996u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c639998; /* fmov.s fr6,@-r15 */
L8c639998: *stop=0x8c639998u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c63999a; /* fmov.s fr5,@-r15 */
L8c63999a: *stop=0x8c63999au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c63999c; /* fmov.s fr4,@-r15 */
L8c63999c: *stop=0x8c63999cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c63999e; /* fmov.s fr3,@-r15 */
L8c63999e: *stop=0x8c63999eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c6399a0; /* fmov.s fr2,@-r15 */
L8c6399a0: *stop=0x8c6399a0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c6399a2; /* fmov.s fr1,@-r15 */
L8c6399a2: *stop=0x8c6399a2u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c6399a4; /* fmov.s fr0,@-r15 */
L8c6399a4: *stop=0x8c6399a4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399a6; /* fmov.s @r5+,fr0 */
L8c6399a6: *stop=0x8c6399a6u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399a8; /* fmov.s @r5+,fr1 */
L8c6399a8: *stop=0x8c6399a8u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399aa; /* fmov.s @r5+,fr2 */
L8c6399aa: *stop=0x8c6399aau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399ac; /* fmov.s @r5+,fr3 */
L8c6399ac: *stop=0x8c6399acu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399ae; /* fmov.s @r5+,fr4 */
L8c6399ae: *stop=0x8c6399aeu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399b0; /* fmov.s @r5+,fr5 */
L8c6399b0: *stop=0x8c6399b0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399b2; /* fmov.s @r5+,fr6 */
L8c6399b2: *stop=0x8c6399b2u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399b4; /* fmov.s @r5+,fr7 */
L8c6399b4: *stop=0x8c6399b4u;status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6399b6; /* ftrv xmtrx,fv0 */
L8c6399b6: *stop=0x8c6399b6u;status=sa_sh4_vector_numeric(0xf5fdu,f,*fpul);if(status)return status;goto L8c6399b8; /* ftrv xmtrx,fv4 */
L8c6399b8: *stop=0x8c6399b8u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6399ba; /* fschg */
L8c6399ba: *stop=0x8c6399bau;fcopy(f,15u,6u);goto L8c6399bc; /* fmov fr6,fr15 */
L8c6399bc: *stop=0x8c6399bcu;fcopy(f,13u,4u);goto L8c6399be; /* fmov fr4,fr13 */
L8c6399be: *stop=0x8c6399beu;fcopy(f,11u,2u);goto L8c6399c0; /* fmov fr2,fr11 */
L8c6399c0: *stop=0x8c6399c0u;fcopy(f,9u,0u);goto L8c6399c2; /* fmov fr0,fr9 */
L8c6399c2: *stop=0x8c6399c2u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6399c4; /* fschg */
L8c6399c4: *stop=0x8c6399c4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399c6; /* fmov.s @r15+,fr0 */
L8c6399c6: *stop=0x8c6399c6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399c8; /* fmov.s @r15+,fr1 */
L8c6399c8: *stop=0x8c6399c8u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399ca; /* fmov.s @r15+,fr2 */
L8c6399ca: *stop=0x8c6399cau;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399cc; /* fmov.s @r15+,fr3 */
L8c6399cc: *stop=0x8c6399ccu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399ce; /* fmov.s @r15+,fr4 */
L8c6399ce: *stop=0x8c6399ceu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399d0; /* fmov.s @r15+,fr5 */
L8c6399d0: *stop=0x8c6399d0u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399d2; /* fmov.s @r15+,fr6 */
L8c6399d2: *stop=0x8c6399d2u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399d4; /* fmov.s @r15+,fr7 */
L8c6399d4: *stop=0x8c6399d4u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6399d6; /* fschg */
L8c6399d6: *stop=0x8c6399d6u;fcopy(f,7u,6u);goto L8c6399d8; /* fmov fr6,fr7 */
L8c6399d8: *stop=0x8c6399d8u;fcopy(f,5u,4u);goto L8c6399da; /* fmov fr4,fr5 */
L8c6399da: *stop=0x8c6399dau;fcopy(f,3u,2u);goto L8c6399dc; /* fmov fr2,fr3 */
L8c6399dc: *stop=0x8c6399dcu;fcopy(f,1u,0u);goto L8c6399de; /* fmov fr0,fr1 */
L8c6399de: *stop=0x8c6399deu;if(*pr!=entry_pr)return -3;*stop=0x8c6399e0u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;return 0; /* rts */
L8c6399e2: *stop=0x8c6399e2u;r[6]=r[5];goto L8c6399e4; /* mov r5,r6 */
L8c6399e4: *stop=0x8c6399e4u;r[0]=16u;goto L8c6399e6; /* mov #0x10,r0 */
L8c6399e6: *stop=0x8c6399e6u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;goto L8c6399e8; /* fmov.s @r4,fr8 */
L8c6399e8: *stop=0x8c6399e8u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]+r[4]);if(io_status)return io_status;goto L8c6399ea; /* fmov.s @(r0,r4),fr9 */
L8c6399ea: *stop=0x8c6399eau;r[0]+=16u;goto L8c6399ec; /* add #0x10,r0 */
L8c6399ec: *stop=0x8c6399ecu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[0]+r[4]);if(io_status)return io_status;goto L8c6399ee; /* fmov.s @(r0,r4),fr10 */
L8c6399ee: *stop=0x8c6399eeu;r[0]+=16u;goto L8c6399f0; /* add #0x10,r0 */
L8c6399f0: *stop=0x8c6399f0u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[0]+r[4]);if(io_status)return io_status;goto L8c6399f2; /* fmov.s @(r0,r4),fr11 */
L8c6399f2: *stop=0x8c6399f2u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399f4; /* fmov.s @r5+,fr0 */
L8c6399f4: *stop=0x8c6399f4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399f6; /* fmov.s @r5+,fr1 */
L8c6399f6: *stop=0x8c6399f6u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399f8; /* fmov.s @r5+,fr2 */
L8c6399f8: *stop=0x8c6399f8u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399fa; /* fmov.s @r5+,fr3 */
L8c6399fa: *stop=0x8c6399fau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399fc; /* fmov.s @r5+,fr4 */
L8c6399fc: *stop=0x8c6399fcu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6399fe; /* fmov.s @r5+,fr5 */
L8c6399fe: *stop=0x8c6399feu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a00; /* fmov.s @r5+,fr6 */
L8c639a00: *stop=0x8c639a00u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a02; /* fmov.s @r5+,fr7 */
L8c639a02: *stop=0x8c639a02u;status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c639a04; /* fipr fv8,fv0 */
L8c639a04: *stop=0x8c639a04u;status=sa_sh4_vector_numeric(0xf6edu,f,*fpul);if(status)return status;goto L8c639a06; /* fipr fv8,fv4 */
L8c639a06: *stop=0x8c639a06u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c639a08; /* fmov.s fr3,@-r15 */
L8c639a08: *stop=0x8c639a08u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c639a0a; /* fmov.s fr7,@-r15 */
L8c639a0a: *stop=0x8c639a0au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a0c; /* fmov.s @r5+,fr0 */
L8c639a0c: *stop=0x8c639a0cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a0e; /* fmov.s @r5+,fr1 */
L8c639a0e: *stop=0x8c639a0eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a10; /* fmov.s @r5+,fr2 */
L8c639a10: *stop=0x8c639a10u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a12; /* fmov.s @r5+,fr3 */
L8c639a12: *stop=0x8c639a12u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a14; /* fmov.s @r5+,fr4 */
L8c639a14: *stop=0x8c639a14u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a16; /* fmov.s @r5+,fr5 */
L8c639a16: *stop=0x8c639a16u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a18; /* fmov.s @r5+,fr6 */
L8c639a18: *stop=0x8c639a18u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a1a; /* fmov.s @r5+,fr7 */
L8c639a1a: *stop=0x8c639a1au;status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c639a1c; /* fipr fv8,fv0 */
L8c639a1c: *stop=0x8c639a1cu;status=sa_sh4_vector_numeric(0xf6edu,f,*fpul);if(status)return status;goto L8c639a1e; /* fipr fv8,fv4 */
L8c639a1e: *stop=0x8c639a1eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c639a20; /* fmov.s fr3,@-r15 */
L8c639a20: *stop=0x8c639a20u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c639a22; /* fmov.s fr7,@-r15 */
L8c639a22: *stop=0x8c639a22u;r[5]=r[6];goto L8c639a24; /* mov r6,r5 */
L8c639a24: *stop=0x8c639a24u;r[0]=4u;goto L8c639a26; /* mov #0x4,r0 */
L8c639a26: *stop=0x8c639a26u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[0]+r[4]);if(io_status)return io_status;goto L8c639a28; /* fmov.s @(r0,r4),fr8 */
L8c639a28: *stop=0x8c639a28u;r[0]+=16u;goto L8c639a2a; /* add #0x10,r0 */
L8c639a2a: *stop=0x8c639a2au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]+r[4]);if(io_status)return io_status;goto L8c639a2c; /* fmov.s @(r0,r4),fr9 */
L8c639a2c: *stop=0x8c639a2cu;r[0]+=16u;goto L8c639a2e; /* add #0x10,r0 */
L8c639a2e: *stop=0x8c639a2eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[0]+r[4]);if(io_status)return io_status;goto L8c639a30; /* fmov.s @(r0,r4),fr10 */
L8c639a30: *stop=0x8c639a30u;r[0]+=16u;goto L8c639a32; /* add #0x10,r0 */
L8c639a32: *stop=0x8c639a32u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[0]+r[4]);if(io_status)return io_status;goto L8c639a34; /* fmov.s @(r0,r4),fr11 */
L8c639a34: *stop=0x8c639a34u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a36; /* fmov.s @r5+,fr0 */
L8c639a36: *stop=0x8c639a36u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a38; /* fmov.s @r5+,fr1 */
L8c639a38: *stop=0x8c639a38u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a3a; /* fmov.s @r5+,fr2 */
L8c639a3a: *stop=0x8c639a3au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a3c; /* fmov.s @r5+,fr3 */
L8c639a3c: *stop=0x8c639a3cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a3e; /* fmov.s @r5+,fr4 */
L8c639a3e: *stop=0x8c639a3eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a40; /* fmov.s @r5+,fr5 */
L8c639a40: *stop=0x8c639a40u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a42; /* fmov.s @r5+,fr6 */
L8c639a42: *stop=0x8c639a42u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a44; /* fmov.s @r5+,fr7 */
L8c639a44: *stop=0x8c639a44u;status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c639a46; /* fipr fv8,fv0 */
L8c639a46: *stop=0x8c639a46u;status=sa_sh4_vector_numeric(0xf6edu,f,*fpul);if(status)return status;goto L8c639a48; /* fipr fv8,fv4 */
L8c639a48: *stop=0x8c639a48u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c639a4a; /* fmov.s fr3,@-r15 */
L8c639a4a: *stop=0x8c639a4au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c639a4c; /* fmov.s fr7,@-r15 */
L8c639a4c: *stop=0x8c639a4cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a4e; /* fmov.s @r5+,fr0 */
L8c639a4e: *stop=0x8c639a4eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a50; /* fmov.s @r5+,fr1 */
L8c639a50: *stop=0x8c639a50u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a52; /* fmov.s @r5+,fr2 */
L8c639a52: *stop=0x8c639a52u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a54; /* fmov.s @r5+,fr3 */
L8c639a54: *stop=0x8c639a54u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a56; /* fmov.s @r5+,fr4 */
L8c639a56: *stop=0x8c639a56u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a58; /* fmov.s @r5+,fr5 */
L8c639a58: *stop=0x8c639a58u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a5a; /* fmov.s @r5+,fr6 */
L8c639a5a: *stop=0x8c639a5au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a5c; /* fmov.s @r5+,fr7 */
L8c639a5c: *stop=0x8c639a5cu;status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c639a5e; /* fipr fv8,fv0 */
L8c639a5e: *stop=0x8c639a5eu;status=sa_sh4_vector_numeric(0xf6edu,f,*fpul);if(status)return status;goto L8c639a60; /* fipr fv8,fv4 */
L8c639a60: *stop=0x8c639a60u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c639a62; /* fmov.s fr3,@-r15 */
L8c639a62: *stop=0x8c639a62u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c639a64; /* fmov.s fr7,@-r15 */
L8c639a64: *stop=0x8c639a64u;r[5]=r[6];goto L8c639a66; /* mov r6,r5 */
L8c639a66: *stop=0x8c639a66u;r[0]=8u;goto L8c639a68; /* mov #0x8,r0 */
L8c639a68: *stop=0x8c639a68u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[0]+r[4]);if(io_status)return io_status;goto L8c639a6a; /* fmov.s @(r0,r4),fr8 */
L8c639a6a: *stop=0x8c639a6au;r[0]+=16u;goto L8c639a6c; /* add #0x10,r0 */
L8c639a6c: *stop=0x8c639a6cu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]+r[4]);if(io_status)return io_status;goto L8c639a6e; /* fmov.s @(r0,r4),fr9 */
L8c639a6e: *stop=0x8c639a6eu;r[0]+=16u;goto L8c639a70; /* add #0x10,r0 */
L8c639a70: *stop=0x8c639a70u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[0]+r[4]);if(io_status)return io_status;goto L8c639a72; /* fmov.s @(r0,r4),fr10 */
L8c639a72: *stop=0x8c639a72u;r[0]+=16u;goto L8c639a74; /* add #0x10,r0 */
L8c639a74: *stop=0x8c639a74u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[0]+r[4]);if(io_status)return io_status;goto L8c639a76; /* fmov.s @(r0,r4),fr11 */
L8c639a76: *stop=0x8c639a76u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a78; /* fmov.s @r5+,fr0 */
L8c639a78: *stop=0x8c639a78u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a7a; /* fmov.s @r5+,fr1 */
L8c639a7a: *stop=0x8c639a7au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a7c; /* fmov.s @r5+,fr2 */
L8c639a7c: *stop=0x8c639a7cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a7e; /* fmov.s @r5+,fr3 */
L8c639a7e: *stop=0x8c639a7eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a80; /* fmov.s @r5+,fr4 */
L8c639a80: *stop=0x8c639a80u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a82; /* fmov.s @r5+,fr5 */
L8c639a82: *stop=0x8c639a82u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a84; /* fmov.s @r5+,fr6 */
L8c639a84: *stop=0x8c639a84u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a86; /* fmov.s @r5+,fr7 */
L8c639a86: *stop=0x8c639a86u;status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c639a88; /* fipr fv8,fv0 */
L8c639a88: *stop=0x8c639a88u;status=sa_sh4_vector_numeric(0xf6edu,f,*fpul);if(status)return status;goto L8c639a8a; /* fipr fv8,fv4 */
L8c639a8a: *stop=0x8c639a8au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c639a8c; /* fmov.s fr3,@-r15 */
L8c639a8c: *stop=0x8c639a8cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c639a8e; /* fmov.s fr7,@-r15 */
L8c639a8e: *stop=0x8c639a8eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a90; /* fmov.s @r5+,fr0 */
L8c639a90: *stop=0x8c639a90u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a92; /* fmov.s @r5+,fr1 */
L8c639a92: *stop=0x8c639a92u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a94; /* fmov.s @r5+,fr2 */
L8c639a94: *stop=0x8c639a94u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a96; /* fmov.s @r5+,fr3 */
L8c639a96: *stop=0x8c639a96u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a98; /* fmov.s @r5+,fr4 */
L8c639a98: *stop=0x8c639a98u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a9a; /* fmov.s @r5+,fr5 */
L8c639a9a: *stop=0x8c639a9au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a9c; /* fmov.s @r5+,fr6 */
L8c639a9c: *stop=0x8c639a9cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639a9e; /* fmov.s @r5+,fr7 */
L8c639a9e: *stop=0x8c639a9eu;status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c639aa0; /* fipr fv8,fv0 */
L8c639aa0: *stop=0x8c639aa0u;status=sa_sh4_vector_numeric(0xf6edu,f,*fpul);if(status)return status;goto L8c639aa2; /* fipr fv8,fv4 */
L8c639aa2: *stop=0x8c639aa2u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c639aa4; /* fmov.s fr3,@-r15 */
L8c639aa4: *stop=0x8c639aa4u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c639aa6; /* fmov.s fr7,@-r15 */
L8c639aa6: *stop=0x8c639aa6u;r[5]=r[6];goto L8c639aa8; /* mov r6,r5 */
L8c639aa8: *stop=0x8c639aa8u;r[0]=12u;goto L8c639aaa; /* mov #0xc,r0 */
L8c639aaa: *stop=0x8c639aaau;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[0]+r[4]);if(io_status)return io_status;goto L8c639aac; /* fmov.s @(r0,r4),fr8 */
L8c639aac: *stop=0x8c639aacu;r[0]+=16u;goto L8c639aae; /* add #0x10,r0 */
L8c639aae: *stop=0x8c639aaeu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[0]+r[4]);if(io_status)return io_status;goto L8c639ab0; /* fmov.s @(r0,r4),fr9 */
L8c639ab0: *stop=0x8c639ab0u;r[0]+=16u;goto L8c639ab2; /* add #0x10,r0 */
L8c639ab2: *stop=0x8c639ab2u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[0]+r[4]);if(io_status)return io_status;goto L8c639ab4; /* fmov.s @(r0,r4),fr10 */
L8c639ab4: *stop=0x8c639ab4u;r[0]+=16u;goto L8c639ab6; /* add #0x10,r0 */
L8c639ab6: *stop=0x8c639ab6u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[0]+r[4]);if(io_status)return io_status;goto L8c639ab8; /* fmov.s @(r0,r4),fr11 */
L8c639ab8: *stop=0x8c639ab8u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639aba; /* fmov.s @r5+,fr0 */
L8c639aba: *stop=0x8c639abau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639abc; /* fmov.s @r5+,fr1 */
L8c639abc: *stop=0x8c639abcu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639abe; /* fmov.s @r5+,fr2 */
L8c639abe: *stop=0x8c639abeu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639ac0; /* fmov.s @r5+,fr3 */
L8c639ac0: *stop=0x8c639ac0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639ac2; /* fmov.s @r5+,fr4 */
L8c639ac2: *stop=0x8c639ac2u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639ac4; /* fmov.s @r5+,fr5 */
L8c639ac4: *stop=0x8c639ac4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639ac6; /* fmov.s @r5+,fr6 */
L8c639ac6: *stop=0x8c639ac6u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639ac8; /* fmov.s @r5+,fr7 */
L8c639ac8: *stop=0x8c639ac8u;status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c639aca; /* fipr fv8,fv0 */
L8c639aca: *stop=0x8c639acau;status=sa_sh4_vector_numeric(0xf6edu,f,*fpul);if(status)return status;goto L8c639acc; /* fipr fv8,fv4 */
L8c639acc: *stop=0x8c639accu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c639ace; /* fmov.s fr3,@-r15 */
L8c639ace: *stop=0x8c639aceu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c639ad0; /* fmov.s fr7,@-r15 */
L8c639ad0: *stop=0x8c639ad0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639ad2; /* fmov.s @r5+,fr0 */
L8c639ad2: *stop=0x8c639ad2u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639ad4; /* fmov.s @r5+,fr1 */
L8c639ad4: *stop=0x8c639ad4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639ad6; /* fmov.s @r5+,fr2 */
L8c639ad6: *stop=0x8c639ad6u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639ad8; /* fmov.s @r5+,fr3 */
L8c639ad8: *stop=0x8c639ad8u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639ada; /* fmov.s @r5+,fr4 */
L8c639ada: *stop=0x8c639adau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639adc; /* fmov.s @r5+,fr5 */
L8c639adc: *stop=0x8c639adcu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639ade; /* fmov.s @r5+,fr6 */
L8c639ade: *stop=0x8c639adeu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639ae0; /* fmov.s @r5+,fr7 */
L8c639ae0: *stop=0x8c639ae0u;status=sa_sh4_vector_numeric(0xf2edu,f,*fpul);if(status)return status;goto L8c639ae2; /* fipr fv8,fv0 */
L8c639ae2: *stop=0x8c639ae2u;status=sa_sh4_vector_numeric(0xf6edu,f,*fpul);if(status)return status;goto L8c639ae4; /* fipr fv8,fv4 */
L8c639ae4: *stop=0x8c639ae4u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c639ae6; /* fmov.s fr3,@-r15 */
L8c639ae6: *stop=0x8c639ae6u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c639ae8; /* fmov.s fr7,@-r15 */
L8c639ae8: *stop=0x8c639ae8u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639aea; /* fmov.s @r15+,fr1 */
L8c639aea: *stop=0x8c639aeau;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639aec; /* fmov.s @r15+,fr3 */
L8c639aec: *stop=0x8c639aecu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639aee; /* fmov.s @r15+,fr5 */
L8c639aee: *stop=0x8c639aeeu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639af0; /* fmov.s @r15+,fr7 */
L8c639af0: *stop=0x8c639af0u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639af2; /* fmov.s @r15+,fr0 */
L8c639af2: *stop=0x8c639af2u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639af4; /* fmov.s @r15+,fr2 */
L8c639af4: *stop=0x8c639af4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639af6; /* fmov.s @r15+,fr4 */
L8c639af6: *stop=0x8c639af6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639af8; /* fmov.s @r15+,fr6 */
L8c639af8: *stop=0x8c639af8u;r[0]=60u;goto L8c639afa; /* mov #0x3c,r0 */
L8c639afa: *stop=0x8c639afau;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c639afc; /* fmov fr1,@(r0,r4) */
L8c639afc: *stop=0x8c639afcu;r[0]=56u;goto L8c639afe; /* mov #0x38,r0 */
L8c639afe: *stop=0x8c639afeu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b00; /* fmov fr0,@(r0,r4) */
L8c639b00: *stop=0x8c639b00u;r[0]=44u;goto L8c639b02; /* mov #0x2c,r0 */
L8c639b02: *stop=0x8c639b02u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b04; /* fmov fr3,@(r0,r4) */
L8c639b04: *stop=0x8c639b04u;r[0]=40u;goto L8c639b06; /* mov #0x28,r0 */
L8c639b06: *stop=0x8c639b06u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b08; /* fmov fr2,@(r0,r4) */
L8c639b08: *stop=0x8c639b08u;r[0]=28u;goto L8c639b0a; /* mov #0x1c,r0 */
L8c639b0a: *stop=0x8c639b0au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b0c; /* fmov fr5,@(r0,r4) */
L8c639b0c: *stop=0x8c639b0cu;r[0]=24u;goto L8c639b0e; /* mov #0x18,r0 */
L8c639b0e: *stop=0x8c639b0eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b10; /* fmov fr4,@(r0,r4) */
L8c639b10: *stop=0x8c639b10u;r[0]=12u;goto L8c639b12; /* mov #0xc,r0 */
L8c639b12: *stop=0x8c639b12u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,7u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b14; /* fmov fr7,@(r0,r4) */
L8c639b14: *stop=0x8c639b14u;r[0]=8u;goto L8c639b16; /* mov #0x8,r0 */
L8c639b16: *stop=0x8c639b16u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,6u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b18; /* fmov fr6,@(r0,r4) */
L8c639b18: *stop=0x8c639b18u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639b1a; /* fmov.s @r15+,fr1 */
L8c639b1a: *stop=0x8c639b1au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639b1c; /* fmov.s @r15+,fr3 */
L8c639b1c: *stop=0x8c639b1cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639b1e; /* fmov.s @r15+,fr5 */
L8c639b1e: *stop=0x8c639b1eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639b20; /* fmov.s @r15+,fr7 */
L8c639b20: *stop=0x8c639b20u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639b22; /* fmov.s @r15+,fr0 */
L8c639b22: *stop=0x8c639b22u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639b24; /* fmov.s @r15+,fr2 */
L8c639b24: *stop=0x8c639b24u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639b26; /* fmov.s @r15+,fr4 */
L8c639b26: *stop=0x8c639b26u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639b28; /* fmov.s @r15+,fr6 */
L8c639b28: *stop=0x8c639b28u;r[0]=52u;goto L8c639b2a; /* mov #0x34,r0 */
L8c639b2a: *stop=0x8c639b2au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b2c; /* fmov fr1,@(r0,r4) */
L8c639b2c: *stop=0x8c639b2cu;r[0]=48u;goto L8c639b2e; /* mov #0x30,r0 */
L8c639b2e: *stop=0x8c639b2eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b30; /* fmov fr0,@(r0,r4) */
L8c639b30: *stop=0x8c639b30u;r[0]=36u;goto L8c639b32; /* mov #0x24,r0 */
L8c639b32: *stop=0x8c639b32u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b34; /* fmov fr3,@(r0,r4) */
L8c639b34: *stop=0x8c639b34u;r[0]=32u;goto L8c639b36; /* mov #0x20,r0 */
L8c639b36: *stop=0x8c639b36u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b38; /* fmov fr2,@(r0,r4) */
L8c639b38: *stop=0x8c639b38u;r[0]=20u;goto L8c639b3a; /* mov #0x14,r0 */
L8c639b3a: *stop=0x8c639b3au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b3c; /* fmov fr5,@(r0,r4) */
L8c639b3c: *stop=0x8c639b3cu;r[0]=16u;goto L8c639b3e; /* mov #0x10,r0 */
L8c639b3e: *stop=0x8c639b3eu;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b40; /* fmov fr4,@(r0,r4) */
L8c639b40: *stop=0x8c639b40u;r[0]=4u;goto L8c639b42; /* mov #0x4,r0 */
L8c639b42: *stop=0x8c639b42u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,7u,r[0]+r[4]);if(io_status)return io_status;goto L8c639b44; /* fmov fr7,@(r0,r4) */
L8c639b44: *stop=0x8c639b44u;if(*pr!=entry_pr)return -3;*stop=0x8c639b46u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c10e5ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10e5ac;
L8c10e5ac: *stop=0x8c10e5acu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c10e5ae; /* fmov.s fr15,@-r15 */
L8c10e5ae: *stop=0x8c10e5aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10e5b0; /* sts.l PR,@-r15 */
L8c10e5b0: *stop=0x8c10e5b0u;r[15]+=4294967288u;goto L8c10e5b2; /* add #-0x8,r15 */
L8c10e5b2: *stop=0x8c10e5b2u;r[0]=4u;goto L8c10e5b4; /* mov #0x4,r0 */
L8c10e5b4: *stop=0x8c10e5b4u;fcopy(f,5u,4u);goto L8c10e5b6; /* fmov fr4,fr5 */
L8c10e5b6: *stop=0x8c10e5b6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[15]);if(io_status)return io_status;goto L8c10e5b8; /* fmov fr5,@(r0,r15) */
L8c10e5b8: *stop=0x8c10e5b8u;if(!valid_access(r[15]+4u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c10e5ba; /* mov.l @(0x4,r15),r3 */
L8c10e5ba: *stop=0x8c10e5bau;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c10e5bc; /* cmp/pz r3 */
L8c10e5bc: take=(*sr&1u);*stop=0x8c10e5beu;r[4]=0u;if(take)goto L8c10e5c2;goto L8c10e5c0; /* bt/s 0x8c10e5c2 */
L8c10e5c0: *stop=0x8c10e5c0u;r[4]=8u;goto L8c10e5c2; /* mov #0x8,r4 */
L8c10e5c2: *stop=0x8c10e5c2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c10e5c4; /* fldi0 fr3 */
L8c10e5c4: *stop=0x8c10e5c4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c10e5c6; /* fcmp/eq fr3,fr5 */
L8c10e5c6: *stop=0x8c10e5c6u;r[7]=4u;goto L8c10e5c8; /* mov #0x4,r7 */
L8c10e5c8: take=!(*sr&1u);*stop=0x8c10e5cau;r[6]=1u;if(take)goto L8c10e5d0;goto L8c10e5cc; /* bf/s 0x8c10e5d0 */
L8c10e5cc: *stop=0x8c10e5ceu;r[4]|=r[7];goto L8c10e5e8; /* bra 0x8c10e5e8 */
L8c10e5d0: *stop=0x8c10e5d0u;if(!valid_access(r[15]+4u,4u))return -3;r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c10e5d2; /* mov.l @(0x4,r15),r2 */
L8c10e5d2: *stop=0x8c10e5d2u;r[3]=rd(m,bus,&io_status,0x8c10e67cu,4);if(io_status)return io_status;goto L8c10e5d4; /* mov.l 0x8c10e67c,r3 */
L8c10e5d4: *stop=0x8c10e5d4u;r[1]=rd(m,bus,&io_status,0x8c10e680u,4);if(io_status)return io_status;goto L8c10e5d6; /* mov.l 0x8c10e680,r1 */
L8c10e5d6: *stop=0x8c10e5d6u;r[2]&=r[3];goto L8c10e5d8; /* and r3,r2 */
L8c10e5d8: *stop=0x8c10e5d8u;*sr=(*sr&~1u)|(r[1]==r[2]);goto L8c10e5da; /* cmp/eq r1,r2 */
L8c10e5da: if(!(*sr&1u))goto L8c10e5e2;goto L8c10e5dc; /* bf 0x8c10e5e2 */
L8c10e5dc: *stop=0x8c10e5dcu;r[0]=2u;goto L8c10e5de; /* mov #0x2,r0 */
L8c10e5de: *stop=0x8c10e5e0u;r[4]|=r[0];goto L8c10e5e8; /* bra 0x8c10e5e8 */
L8c10e5e2: *stop=0x8c10e5e2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,0);if(status)return status;goto L8c10e5e4; /* fcmp/eq fr5,fr5 */
L8c10e5e4: if((*sr&1u))goto L8c10e5e8;goto L8c10e5e6; /* bt 0x8c10e5e8 */
L8c10e5e6: *stop=0x8c10e5e6u;r[4]|=r[6];goto L8c10e5e8; /* or r6,r4 */
L8c10e5e8: *stop=0x8c10e5e8u;r[5]=rd(m,bus,&io_status,0x8c10e684u,4);if(io_status)return io_status;goto L8c10e5ea; /* mov.l 0x8c10e684,r5 */
L8c10e5ea: *stop=0x8c10e5eau;*sr=(*sr&~1u)|!(r[4]&r[6]);goto L8c10e5ec; /* tst r4,r6 */
L8c10e5ec: if((*sr&1u))goto L8c10e5f8;goto L8c10e5ee; /* bt 0x8c10e5f8 */
L8c10e5ee: *stop=0x8c10e5eeu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c10e5f0; /* fmov.s @r5,fr0 */
L8c10e5f0: *stop=0x8c10e5f0u;r[15]+=8u;goto L8c10e5f2; /* add #0x8,r15 */
L8c10e5f2: *stop=0x8c10e5f2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10e5f4; /* lds.l @r15+,PR */
L8c10e5f4: *stop=0x8c10e5f4u;if(*pr!=entry_pr)return -3;*stop=0x8c10e5f6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;return 0; /* rts */
L8c10e5f8: *stop=0x8c10e5f8u;r[3]=r[15];goto L8c10e5fa; /* mov r15,r3 */
L8c10e5fa: *stop=0x8c10e5fau;r[2]=rd(m,bus,&io_status,0x8c10e688u,4);if(io_status)return io_status;goto L8c10e5fc; /* mov.l 0x8c10e688,r2 */
L8c10e5fc: *stop=0x8c10e5fcu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c10e5fe; /* fmov.s fr4,@r15 */
L8c10e5fe: *stop=0x8c10e5feu;if(!valid_access(r[3],4u))return -3;r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c10e600; /* mov.l @r3,r1 */
L8c10e600: *stop=0x8c10e600u;*sr=(*sr&~1u)|!(r[2]&r[1]);goto L8c10e602; /* tst r2,r1 */
L8c10e602: if((*sr&1u))goto L8c10e60a;goto L8c10e604; /* bt 0x8c10e60a */
L8c10e604: *stop=0x8c10e604u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c10e606; /* fmov.s @r15,fr3 */
L8c10e606: *stop=0x8c10e606u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c10e608; /* fneg fr3 */
L8c10e608: *stop=0x8c10e608u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c10e60a; /* fmov.s fr3,@r15 */
L8c10e60a: *stop=0x8c10e60au;r[3]=rd(m,bus,&io_status,0x8c10e68cu,4);if(io_status)return io_status;goto L8c10e60c; /* mov.l 0x8c10e68c,r3 */
L8c10e60c: *stop=0x8c10e60cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c10e60e; /* fmov.s @r15,fr3 */
L8c10e60e: *stop=0x8c10e60eu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[3]);if(io_status)return io_status;goto L8c10e610; /* fmov.s @r3,fr5 */
L8c10e610: *stop=0x8c10e610u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c10e612; /* fcmp/gt fr5,fr3 */
L8c10e612: if(!(*sr&1u))goto L8c10e618;goto L8c10e614; /* bf 0x8c10e618 */
L8c10e614: *stop=0x8c10e616u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;goto L8c10e672; /* bra 0x8c10e672 */
L8c10e618: *stop=0x8c10e618u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,0);if(status)return status;goto L8c10e61a; /* fcmp/eq fr5,fr3 */
L8c10e61a: if(!(*sr&1u))goto L8c10e62a;goto L8c10e61c; /* bf 0x8c10e62a */
L8c10e61c: *stop=0x8c10e61cu;r[2]=8u;goto L8c10e61e; /* mov #0x8,r2 */
L8c10e61e: *stop=0x8c10e61eu;r[1]=rd(m,bus,&io_status,0x8c10e690u,4);if(io_status)return io_status;goto L8c10e620; /* mov.l 0x8c10e690,r1 */
L8c10e620: *stop=0x8c10e620u;*sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c10e622; /* tst r2,r4 */
L8c10e622: take=(*sr&1u);*stop=0x8c10e624u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;if(take)goto L8c10e672;goto L8c10e626; /* bt/s 0x8c10e672 */
L8c10e626: *stop=0x8c10e628u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]^=0x80000000u;goto L8c10e672; /* bra 0x8c10e672 */
L8c10e62a: *stop=0x8c10e62au;*sr=(*sr&~1u)|!(r[7]&r[4]);goto L8c10e62c; /* tst r7,r4 */
L8c10e62c: if((*sr&1u))goto L8c10e632;goto L8c10e62e; /* bt 0x8c10e632 */
L8c10e62e: *stop=0x8c10e630u;fcopy(f,5u,4u);goto L8c10e672; /* bra 0x8c10e672 */
L8c10e632: *stop=0x8c10e632u;r[2]=rd(m,bus,&io_status,0x8c10e694u,4);if(io_status)return io_status;goto L8c10e634; /* mov.l 0x8c10e694,r2 */
L8c10e634: *stop=0x8c10e634u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c10e636; /* fmov.s @r15,fr3 */
L8c10e636: *stop=0x8c10e636u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c10e638; /* fmov.s @r2,fr2 */
L8c10e638: *stop=0x8c10e638u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c10e63a; /* fcmp/gt fr3,fr2 */
L8c10e63a: if(!(*sr&1u))goto L8c10e650;goto L8c10e63c; /* bf 0x8c10e650 */
L8c10e63c: *stop=0x8c10e63cu;fcopy(f,1u,4u);goto L8c10e63e; /* fmov fr4,fr1 */
L8c10e63e: *stop=0x8c10e63eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c10e640; /* fmul fr4,fr1 */
L8c10e640: *stop=0x8c10e640u;fcopy(f,0u,5u);goto L8c10e642; /* fmov fr5,fr0 */
L8c10e642: *stop=0x8c10e642u;fcopy(f,15u,4u);goto L8c10e644; /* fmov fr4,fr15 */
L8c10e644: *stop=0x8c10e644u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c10e646; /* fsub fr1,fr0 */
L8c10e646: *stop=0x8c10e646u;fcopy(f,1u,5u);goto L8c10e648; /* fmov fr5,fr1 */
L8c10e648: *stop=0x8c10e648u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[0],&f->fpscr,&fr[0]);if(status)return status;}goto L8c10e64a; /* fsqrt fr0 */
L8c10e64a: *stop=0x8c10e64au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[0],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c10e64c; /* fadd fr0,fr1 */
L8c10e64c: *stop=0x8c10e64eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[15],&fr[1],&f->fpscr,&fr[15]);if(status)return status;}goto L8c10e662; /* bra 0x8c10e662 */
L8c10e650: *stop=0x8c10e650u;fcopy(f,1u,4u);goto L8c10e652; /* fmov fr4,fr1 */
L8c10e652: *stop=0x8c10e652u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c10e654; /* fmul fr4,fr1 */
L8c10e654: *stop=0x8c10e654u;fcopy(f,0u,5u);goto L8c10e656; /* fmov fr5,fr0 */
L8c10e656: *stop=0x8c10e656u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c10e658; /* fsub fr1,fr0 */
L8c10e658: *stop=0x8c10e658u;fcopy(f,1u,5u);goto L8c10e65a; /* fmov fr5,fr1 */
L8c10e65a: *stop=0x8c10e65au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_sqrt(fr[0],&f->fpscr,&fr[0]);if(status)return status;}goto L8c10e65c; /* fsqrt fr0 */
L8c10e65c: *stop=0x8c10e65cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[0],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c10e65e; /* fsub fr0,fr1 */
L8c10e65e: *stop=0x8c10e65eu;fcopy(f,15u,1u);goto L8c10e660; /* fmov fr1,fr15 */
L8c10e660: *stop=0x8c10e660u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[15],&fr[4],&f->fpscr,&fr[15]);if(status)return status;}goto L8c10e662; /* fdiv fr4,fr15 */
L8c10e662: *stop=0x8c10e662u;r[3]=rd(m,bus,&io_status,0x8c10e698u,4);if(io_status)return io_status;goto L8c10e664; /* mov.l 0x8c10e698,r3 */
L8c10e664: target=r[3];*pr=0x8c10e668u;*stop=0x8c10e666u;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e668; /* jsr @r3 */
L8c10e668: *stop=0x8c10e668u;r[2]=rd(m,bus,&io_status,0x8c10e69cu,4);if(io_status)return io_status;goto L8c10e66a; /* mov.l 0x8c10e69c,r2 */
L8c10e66a: *stop=0x8c10e66au;fcopy(f,4u,0u);goto L8c10e66c; /* fmov fr0,fr4 */
L8c10e66c: target=r[2];*pr=0x8c10e670u;*stop=0x8c10e66eu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10e670; /* jsr @r2 */
L8c10e670: *stop=0x8c10e670u;fcopy(f,5u,0u);goto L8c10e672; /* fmov fr0,fr5 */
L8c10e672: *stop=0x8c10e672u;fcopy(f,0u,5u);goto L8c10e674; /* fmov fr5,fr0 */
L8c10e674: *stop=0x8c10e674u;r[15]+=8u;goto L8c10e676; /* add #0x8,r15 */
L8c10e676: *stop=0x8c10e676u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10e678; /* lds.l @r15+,PR */
L8c10e678: *stop=0x8c10e678u;if(*pr!=entry_pr)return -3;*stop=0x8c10e67au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c10ef84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10ef84;
L8c10ef84: *stop=0x8c10ef84u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10ef86; /* mov.l r14,@-r15 */
L8c10ef86: *stop=0x8c10ef86u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10ef88; /* mov.l r13,@-r15 */
L8c10ef88: *stop=0x8c10ef88u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c10ef8a; /* fmov.s fr15,@-r15 */
L8c10ef8a: *stop=0x8c10ef8au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c10ef8c; /* fmov.s fr14,@-r15 */
L8c10ef8c: *stop=0x8c10ef8cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c10ef8e; /* fmov.s fr13,@-r15 */
L8c10ef8e: *stop=0x8c10ef8eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c10ef90; /* fmov.s fr12,@-r15 */
L8c10ef90: *stop=0x8c10ef90u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10ef92; /* sts.l PR,@-r15 */
L8c10ef92: *stop=0x8c10ef92u;r[15]+=4294967284u;goto L8c10ef94; /* add #-0xc,r15 */
L8c10ef94: *stop=0x8c10ef94u;r[0]=8u;goto L8c10ef96; /* mov #0x8,r0 */
L8c10ef96: *stop=0x8c10ef96u;fcopy(f,5u,4u);goto L8c10ef98; /* fmov fr4,fr5 */
L8c10ef98: *stop=0x8c10ef98u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[15]);if(io_status)return io_status;goto L8c10ef9a; /* fmov fr5,@(r0,r15) */
L8c10ef9a: *stop=0x8c10ef9au;if(!valid_access(r[15]+8u,4u))return -3;r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c10ef9c; /* mov.l @(0x8,r15),r3 */
L8c10ef9c: *stop=0x8c10ef9cu;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c10ef9e; /* cmp/pz r3 */
L8c10ef9e: take=(*sr&1u);*stop=0x8c10efa0u;r[4]=0u;if(take)goto L8c10efa4;goto L8c10efa2; /* bt/s 0x8c10efa4 */
L8c10efa2: *stop=0x8c10efa2u;r[4]=8u;goto L8c10efa4; /* mov #0x8,r4 */
L8c10efa4: *stop=0x8c10efa4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c10efa6; /* fldi0 fr3 */
L8c10efa6: *stop=0x8c10efa6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c10efa8; /* fcmp/eq fr3,fr5 */
L8c10efa8: *stop=0x8c10efa8u;r[7]=4u;goto L8c10efaa; /* mov #0x4,r7 */
L8c10efaa: take=!(*sr&1u);*stop=0x8c10efacu;r[6]=1u;if(take)goto L8c10efb2;goto L8c10efae; /* bf/s 0x8c10efb2 */
L8c10efae: *stop=0x8c10efb0u;r[4]|=r[7];goto L8c10efca; /* bra 0x8c10efca */
L8c10efb2: *stop=0x8c10efb2u;if(!valid_access(r[15]+8u,4u))return -3;r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c10efb4; /* mov.l @(0x8,r15),r2 */
L8c10efb4: *stop=0x8c10efb4u;r[3]=rd(m,bus,&io_status,0x8c10f114u,4);if(io_status)return io_status;goto L8c10efb6; /* mov.l 0x8c10f114,r3 */
L8c10efb6: *stop=0x8c10efb6u;r[1]=rd(m,bus,&io_status,0x8c10f118u,4);if(io_status)return io_status;goto L8c10efb8; /* mov.l 0x8c10f118,r1 */
L8c10efb8: *stop=0x8c10efb8u;r[2]&=r[3];goto L8c10efba; /* and r3,r2 */
L8c10efba: *stop=0x8c10efbau;*sr=(*sr&~1u)|(r[1]==r[2]);goto L8c10efbc; /* cmp/eq r1,r2 */
L8c10efbc: if(!(*sr&1u))goto L8c10efc4;goto L8c10efbe; /* bf 0x8c10efc4 */
L8c10efbe: *stop=0x8c10efbeu;r[0]=2u;goto L8c10efc0; /* mov #0x2,r0 */
L8c10efc0: *stop=0x8c10efc2u;r[4]|=r[0];goto L8c10efca; /* bra 0x8c10efca */
L8c10efc4: *stop=0x8c10efc4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,0);if(status)return status;goto L8c10efc6; /* fcmp/eq fr5,fr5 */
L8c10efc6: if((*sr&1u))goto L8c10efca;goto L8c10efc8; /* bt 0x8c10efca */
L8c10efc8: *stop=0x8c10efc8u;r[4]|=r[6];goto L8c10efca; /* or r6,r4 */
L8c10efca: *stop=0x8c10efcau;r[3]=r[15];goto L8c10efcc; /* mov r15,r3 */
L8c10efcc: *stop=0x8c10efccu;r[2]=rd(m,bus,&io_status,0x8c10f11cu,4);if(io_status)return io_status;goto L8c10efce; /* mov.l 0x8c10f11c,r2 */
L8c10efce: *stop=0x8c10efceu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c10efd0; /* fmov.s fr4,@r15 */
L8c10efd0: *stop=0x8c10efd0u;if(!valid_access(r[3],4u))return -3;r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c10efd2; /* mov.l @r3,r1 */
L8c10efd2: *stop=0x8c10efd2u;*sr=(*sr&~1u)|!(r[2]&r[1]);goto L8c10efd4; /* tst r2,r1 */
L8c10efd4: take=(*sr&1u);*stop=0x8c10efd6u;r[5]=r[4];if(take)goto L8c10efde;goto L8c10efd8; /* bt/s 0x8c10efde */
L8c10efd8: *stop=0x8c10efd8u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c10efda; /* fmov.s @r15,fr3 */
L8c10efda: *stop=0x8c10efdau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c10efdc; /* fneg fr3 */
L8c10efdc: *stop=0x8c10efdcu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c10efde; /* fmov.s fr3,@r15 */
L8c10efde: *stop=0x8c10efdeu;*sr=(*sr&~1u)|!(r[4]&r[6]);goto L8c10efe0; /* tst r4,r6 */
L8c10efe0: if((*sr&1u))goto L8c10efe8;goto L8c10efe2; /* bt 0x8c10efe8 */
L8c10efe2: *stop=0x8c10efe2u;r[2]=rd(m,bus,&io_status,0x8c10f120u,4);if(io_status)return io_status;goto L8c10efe4; /* mov.l 0x8c10f120,r2 */
L8c10efe4: *stop=0x8c10efe6u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[2]);if(io_status)return io_status;goto L8c10f100; /* bra 0x8c10f100 */
L8c10efe8: *stop=0x8c10efe8u;r[2]=r[15];goto L8c10efea; /* mov r15,r2 */
L8c10efea: *stop=0x8c10efeau;if(!valid_access(r[2],4u))return -3;r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c10efec; /* mov.l @r2,r1 */
L8c10efec: *stop=0x8c10efecu;r[3]=rd(m,bus,&io_status,0x8c10f118u,4);if(io_status)return io_status;goto L8c10efee; /* mov.l 0x8c10f118,r3 */
L8c10efee: *stop=0x8c10efeeu;r[2]=4294967273u;goto L8c10eff0; /* mov #-0x17,r2 */
L8c10eff0: *stop=0x8c10eff0u;r[0]=32u;goto L8c10eff2; /* mov #0x20,r0 */
L8c10eff2: *stop=0x8c10eff2u;r[1]&=r[3];goto L8c10eff4; /* and r3,r1 */
L8c10eff4: *stop=0x8c10eff4u;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}}goto L8c10eff6; /* shad r2,r1 */
L8c10eff6: *stop=0x8c10eff6u;r[1]+=4294967169u;goto L8c10eff8; /* add #-0x7f,r1 */
L8c10eff8: *stop=0x8c10eff8u;*sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[0]);goto L8c10effa; /* cmp/ge r0,r1 */
L8c10effa: *stop=0x8c10effau;r[13]=8u;goto L8c10effc; /* mov #0x8,r13 */
L8c10effc: take=!(*sr&1u);*stop=0x8c10effeu;r[13]&=r[4];if(take)goto L8c10f010;goto L8c10f000; /* bf/s 0x8c10f010 */
L8c10f000: *stop=0x8c10f000u;r[1]=rd(m,bus,&io_status,0x8c10f124u,4);if(io_status)return io_status;goto L8c10f002; /* mov.l 0x8c10f124,r1 */
L8c10f002: *stop=0x8c10f002u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c10f004; /* tst r13,r13 */
L8c10f004: take=!(*sr&1u);*stop=0x8c10f006u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;if(take)goto L8c10f00c;goto L8c10f008; /* bf/s 0x8c10f00c */
L8c10f008: *stop=0x8c10f00au;goto L8c10f100; /* bra 0x8c10f100 */
L8c10f00c: *stop=0x8c10f00eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]^=0x80000000u;goto L8c10f100; /* bra 0x8c10f100 */
L8c10f010: *stop=0x8c10f010u;*sr=(*sr&~1u)|!(r[7]&r[4]);goto L8c10f012; /* tst r7,r4 */
L8c10f012: if((*sr&1u))goto L8c10f018;goto L8c10f014; /* bt 0x8c10f018 */
L8c10f014: *stop=0x8c10f016u;fcopy(f,5u,4u);goto L8c10f100; /* bra 0x8c10f100 */
L8c10f018: *stop=0x8c10f018u;r[1]=rd(m,bus,&io_status,0x8c10f128u,4);if(io_status)return io_status;goto L8c10f01a; /* mov.l 0x8c10f128,r1 */
L8c10f01a: *stop=0x8c10f01au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c10f01c; /* fmov.s @r15,fr3 */
L8c10f01c: *stop=0x8c10f01cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;goto L8c10f01e; /* fmov.s @r1,fr5 */
L8c10f01e: *stop=0x8c10f01eu;r[3]=rd(m,bus,&io_status,0x8c10f12cu,4);if(io_status)return io_status;goto L8c10f020; /* mov.l 0x8c10f12c,r3 */
L8c10f020: *stop=0x8c10f020u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c10f022; /* fcmp/gt fr3,fr5 */
L8c10f022: take=!(*sr&1u);*stop=0x8c10f024u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[3]);if(io_status)return io_status;if(take)goto L8c10f02e;goto L8c10f026; /* bf/s 0x8c10f02e */
L8c10f026: *stop=0x8c10f026u;r[2]=rd(m,bus,&io_status,0x8c10f130u,4);if(io_status)return io_status;goto L8c10f028; /* mov.l 0x8c10f130,r2 */
L8c10f028: *stop=0x8c10f028u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[2]);if(io_status)return io_status;goto L8c10f02a; /* fmov.s @r2,fr14 */
L8c10f02a: *stop=0x8c10f02cu;fcopy(f,15u,3u);goto L8c10f04c; /* bra 0x8c10f04c */
L8c10f02e: *stop=0x8c10f02eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c10f030; /* fcmp/eq fr3,fr5 */
L8c10f030: if(!(*sr&1u))goto L8c10f03c;goto L8c10f032; /* bf 0x8c10f03c */
L8c10f032: *stop=0x8c10f032u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c10f034; /* tst r13,r13 */
L8c10f034: if((*sr&1u))goto L8c10f038;goto L8c10f036; /* bt 0x8c10f038 */
L8c10f036: *stop=0x8c10f036u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c10f038; /* fneg fr4 */
L8c10f038: *stop=0x8c10f03au;fcopy(f,0u,4u);goto L8c10f102; /* bra 0x8c10f102 */
L8c10f03c: *stop=0x8c10f03cu;r[2]=rd(m,bus,&io_status,0x8c10f134u,4);if(io_status)return io_status;goto L8c10f03e; /* mov.l 0x8c10f134,r2 */
L8c10f03e: *stop=0x8c10f03eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c10f040; /* fmov.s @r15,fr3 */
L8c10f040: *stop=0x8c10f040u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c10f042; /* fmov.s @r2,fr2 */
L8c10f042: *stop=0x8c10f042u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c10f044; /* fadd fr3,fr5 */
L8c10f044: *stop=0x8c10f044u;fcopy(f,14u,4u);goto L8c10f046; /* fmov fr4,fr14 */
L8c10f046: *stop=0x8c10f046u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c10f048; /* fadd fr3,fr2 */
L8c10f048: *stop=0x8c10f048u;fcopy(f,15u,2u);goto L8c10f04a; /* fmov fr2,fr15 */
L8c10f04a: *stop=0x8c10f04au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[15],&fr[5],&f->fpscr,&fr[15]);if(status)return status;}goto L8c10f04c; /* fdiv fr5,fr15 */
L8c10f04c: *stop=0x8c10f04cu;r[3]=rd(m,bus,&io_status,0x8c10f138u,4);if(io_status)return io_status;goto L8c10f04e; /* mov.l 0x8c10f138,r3 */
L8c10f04e: *stop=0x8c10f04eu;r[4]=r[15];goto L8c10f050; /* mov r15,r4 */
L8c10f050: *stop=0x8c10f050u;r[2]=rd(m,bus,&io_status,0x8c10f13cu,4);if(io_status)return io_status;goto L8c10f052; /* mov.l 0x8c10f13c,r2 */
L8c10f052: *stop=0x8c10f052u;r[4]+=4u;goto L8c10f054; /* add #0x4,r4 */
L8c10f054: *stop=0x8c10f054u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[3]);if(io_status)return io_status;goto L8c10f056; /* fmov.s @r3,fr5 */
L8c10f056: target=r[2];*pr=0x8c10f05au;*stop=0x8c10f058u;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10f05a; /* jsr @r2 */
L8c10f05a: *stop=0x8c10f05au;if(!valid_access(r[15]+4u,4u))return -3;r[0]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c10f05c; /* mov.l @(0x4,r15),r0 */
L8c10f05c: *stop=0x8c10f05cu;r[3]=rd(m,bus,&io_status,0x8c10f128u,4);if(io_status)return io_status;goto L8c10f05e; /* mov.l 0x8c10f128,r3 */
L8c10f05e: *stop=0x8c10f05eu;r[14]=rd(m,bus,&io_status,0x8c10f140u,4);if(io_status)return io_status;goto L8c10f060; /* mov.l 0x8c10f140,r14 */
L8c10f060: *stop=0x8c10f060u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10f062; /* cmp/eq #0x0,r0 */
L8c10f062: take=(*sr&1u);*stop=0x8c10f064u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[3]);if(io_status)return io_status;if(take)goto L8c10f086;goto L8c10f066; /* bt/s 0x8c10f086 */
L8c10f066: *stop=0x8c10f066u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10f068; /* cmp/eq #0x1,r0 */
L8c10f068: if((*sr&1u))goto L8c10f08a;goto L8c10f06a; /* bt 0x8c10f08a */
L8c10f06a: *stop=0x8c10f06au;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10f06c; /* cmp/eq #0x2,r0 */
L8c10f06c: if((*sr&1u))goto L8c10f08a;goto L8c10f06e; /* bt 0x8c10f08a */
L8c10f06e: *stop=0x8c10f06eu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c10f070; /* cmp/eq #0x3,r0 */
L8c10f070: if((*sr&1u))goto L8c10f0a8;goto L8c10f072; /* bt 0x8c10f0a8 */
L8c10f072: *stop=0x8c10f072u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c10f074; /* cmp/eq #0x4,r0 */
L8c10f074: if((*sr&1u))goto L8c10f0a8;goto L8c10f076; /* bt 0x8c10f0a8 */
L8c10f076: *stop=0x8c10f076u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c10f078; /* cmp/eq #0x5,r0 */
L8c10f078: if((*sr&1u))goto L8c10f0c6;goto L8c10f07a; /* bt 0x8c10f0c6 */
L8c10f07a: *stop=0x8c10f07au;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c10f07c; /* cmp/eq #0x6,r0 */
L8c10f07c: if((*sr&1u))goto L8c10f0c6;goto L8c10f07e; /* bt 0x8c10f0c6 */
L8c10f07e: *stop=0x8c10f07eu;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c10f080; /* cmp/eq #0x7,r0 */
L8c10f080: if((*sr&1u))goto L8c10f0e0;goto L8c10f082; /* bt 0x8c10f0e0 */
L8c10f082: *stop=0x8c10f084u;goto L8c10f0fa; /* bra 0x8c10f0fa */
L8c10f086: *stop=0x8c10f088u;fcopy(f,4u,15u);goto L8c10f0f2; /* bra 0x8c10f0f2 */
L8c10f08a: *stop=0x8c10f08au;r[1]=rd(m,bus,&io_status,0x8c10f148u,4);if(io_status)return io_status;goto L8c10f08c; /* mov.l 0x8c10f148,r1 */
L8c10f08c: *stop=0x8c10f08cu;r[4]=4294967294u;goto L8c10f08e; /* mov #-0x2,r4 */
L8c10f08e: *stop=0x8c10f08eu;r[2]=rd(m,bus,&io_status,0x8c10f144u,4);if(io_status)return io_status;goto L8c10f090; /* mov.l 0x8c10f144,r2 */
L8c10f090: *stop=0x8c10f090u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c10f092; /* fmov.s @r1,fr2 */
L8c10f092: *stop=0x8c10f092u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c10f094; /* fmov.s @r2,fr3 */
L8c10f094: *stop=0x8c10f094u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[15],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c10f096; /* fadd fr15,fr2 */
L8c10f096: *stop=0x8c10f096u;r[3]=rd(m,bus,&io_status,0x8c10f14cu,4);if(io_status)return io_status;goto L8c10f098; /* mov.l 0x8c10f14c,r3 */
L8c10f098: *stop=0x8c10f098u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c10f09a; /* fadd fr3,fr14 */
L8c10f09a: *stop=0x8c10f09au;fcopy(f,12u,2u);goto L8c10f09c; /* fmov fr2,fr12 */
L8c10f09c: target=r[3];*pr=0x8c10f0a0u;*stop=0x8c10f09eu;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10f0a0; /* jsr @r3 */
L8c10f0a0: *stop=0x8c10f0a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[0],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c10f0a2; /* fadd fr0,fr13 */
L8c10f0a2: *stop=0x8c10f0a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[12],&fr[13],&f->fpscr,&fr[12]);if(status)return status;}goto L8c10f0a4; /* fdiv fr13,fr12 */
L8c10f0a4: *stop=0x8c10f0a6u;fcopy(f,4u,12u);goto L8c10f0f2; /* bra 0x8c10f0f2 */
L8c10f0a8: *stop=0x8c10f0a8u;r[1]=rd(m,bus,&io_status,0x8c10f154u,4);if(io_status)return io_status;goto L8c10f0aa; /* mov.l 0x8c10f154,r1 */
L8c10f0aa: *stop=0x8c10f0aau;r[4]=4294967295u;goto L8c10f0ac; /* mov #-0x1,r4 */
L8c10f0ac: *stop=0x8c10f0acu;r[2]=rd(m,bus,&io_status,0x8c10f150u,4);if(io_status)return io_status;goto L8c10f0ae; /* mov.l 0x8c10f150,r2 */
L8c10f0ae: *stop=0x8c10f0aeu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c10f0b0; /* fmov.s @r1,fr2 */
L8c10f0b0: *stop=0x8c10f0b0u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c10f0b2; /* fmov.s @r2,fr3 */
L8c10f0b2: *stop=0x8c10f0b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[15],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c10f0b4; /* fadd fr15,fr2 */
L8c10f0b4: *stop=0x8c10f0b4u;r[3]=rd(m,bus,&io_status,0x8c10f14cu,4);if(io_status)return io_status;goto L8c10f0b6; /* mov.l 0x8c10f14c,r3 */
L8c10f0b6: *stop=0x8c10f0b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c10f0b8; /* fadd fr3,fr14 */
L8c10f0b8: *stop=0x8c10f0b8u;fcopy(f,12u,2u);goto L8c10f0ba; /* fmov fr2,fr12 */
L8c10f0ba: target=r[3];*pr=0x8c10f0beu;*stop=0x8c10f0bcu;fcopy(f,4u,15u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10f0be; /* jsr @r3 */
L8c10f0be: *stop=0x8c10f0beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[0],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c10f0c0; /* fadd fr0,fr13 */
L8c10f0c0: *stop=0x8c10f0c0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[12],&fr[13],&f->fpscr,&fr[12]);if(status)return status;}goto L8c10f0c2; /* fdiv fr13,fr12 */
L8c10f0c2: *stop=0x8c10f0c4u;fcopy(f,4u,12u);goto L8c10f0f2; /* bra 0x8c10f0f2 */
L8c10f0c6: *stop=0x8c10f0c6u;r[1]=rd(m,bus,&io_status,0x8c10f15cu,4);if(io_status)return io_status;goto L8c10f0c8; /* mov.l 0x8c10f15c,r1 */
L8c10f0c8: *stop=0x8c10f0c8u;r[3]=rd(m,bus,&io_status,0x8c10f160u,4);if(io_status)return io_status;goto L8c10f0ca; /* mov.l 0x8c10f160,r3 */
L8c10f0ca: *stop=0x8c10f0cau;r[2]=rd(m,bus,&io_status,0x8c10f158u,4);if(io_status)return io_status;goto L8c10f0cc; /* mov.l 0x8c10f158,r2 */
L8c10f0cc: *stop=0x8c10f0ccu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c10f0ce; /* fmov.s @r1,fr2 */
L8c10f0ce: *stop=0x8c10f0ceu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[3]);if(io_status)return io_status;goto L8c10f0d0; /* fmov.s @r3,fr0 */
L8c10f0d0: *stop=0x8c10f0d0u;fcopy(f,1u,15u);goto L8c10f0d2; /* fmov fr15,fr1 */
L8c10f0d2: *stop=0x8c10f0d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c10f0d4; /* fadd fr2,fr1 */
L8c10f0d4: *stop=0x8c10f0d4u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c10f0d6; /* fmov.s @r2,fr3 */
L8c10f0d6: *stop=0x8c10f0d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[15],&fr[13],&f->fpscr,&fr[13]);if(status)return status;}goto L8c10f0d8; /* fmac fr0,fr15,fr13 */
L8c10f0d8: *stop=0x8c10f0d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c10f0da; /* fadd fr3,fr14 */
L8c10f0da: *stop=0x8c10f0dau;fcopy(f,4u,1u);goto L8c10f0dc; /* fmov fr1,fr4 */
L8c10f0dc: *stop=0x8c10f0deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[13],&f->fpscr,&fr[4]);if(status)return status;}goto L8c10f0f2; /* bra 0x8c10f0f2 */
L8c10f0e0: *stop=0x8c10f0e0u;r[1]=rd(m,bus,&io_status,0x8c10f134u,4);if(io_status)return io_status;goto L8c10f0e2; /* mov.l 0x8c10f134,r1 */
L8c10f0e2: *stop=0x8c10f0e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[15],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c10f0e4; /* fadd fr15,fr13 */
L8c10f0e4: *stop=0x8c10f0e4u;r[2]=rd(m,bus,&io_status,0x8c10f12cu,4);if(io_status)return io_status;goto L8c10f0e6; /* mov.l 0x8c10f12c,r2 */
L8c10f0e6: *stop=0x8c10f0e6u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c10f0e8; /* fmov.s @r1,fr2 */
L8c10f0e8: *stop=0x8c10f0e8u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c10f0ea; /* fmov.s @r2,fr3 */
L8c10f0ea: *stop=0x8c10f0eau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[15],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c10f0ec; /* fadd fr15,fr2 */
L8c10f0ec: *stop=0x8c10f0ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c10f0ee; /* fadd fr3,fr14 */
L8c10f0ee: *stop=0x8c10f0eeu;fcopy(f,4u,2u);goto L8c10f0f0; /* fmov fr2,fr4 */
L8c10f0f0: *stop=0x8c10f0f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[13],&f->fpscr,&fr[4]);if(status)return status;}goto L8c10f0f2; /* fdiv fr13,fr4 */
L8c10f0f2: target=r[14];*pr=0x8c10f0f6u;*stop=0x8c10f0f4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10f0f6; /* jsr @r14 */
L8c10f0f6: *stop=0x8c10f0f6u;fcopy(f,5u,14u);goto L8c10f0f8; /* fmov fr14,fr5 */
L8c10f0f8: *stop=0x8c10f0f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[0],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c10f0fa; /* fadd fr0,fr5 */
L8c10f0fa: *stop=0x8c10f0fau;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c10f0fc; /* tst r13,r13 */
L8c10f0fc: if((*sr&1u))goto L8c10f100;goto L8c10f0fe; /* bt 0x8c10f100 */
L8c10f0fe: *stop=0x8c10f0feu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]^=0x80000000u;goto L8c10f100; /* fneg fr5 */
L8c10f100: *stop=0x8c10f100u;fcopy(f,0u,5u);goto L8c10f102; /* fmov fr5,fr0 */
L8c10f102: *stop=0x8c10f102u;r[15]+=12u;goto L8c10f104; /* add #0xc,r15 */
L8c10f104: *stop=0x8c10f104u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10f106; /* lds.l @r15+,PR */
L8c10f106: *stop=0x8c10f106u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c10f108; /* fmov.s @r15+,fr12 */
L8c10f108: *stop=0x8c10f108u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c10f10a; /* fmov.s @r15+,fr13 */
L8c10f10a: *stop=0x8c10f10au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c10f10c; /* fmov.s @r15+,fr14 */
L8c10f10c: *stop=0x8c10f10cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c10f10e; /* fmov.s @r15+,fr15 */
L8c10f10e: *stop=0x8c10f10eu;if(!valid_access(r[15],4u))return -3;r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c10f110; /* mov.l @r15+,r13 */
L8c10f110: *stop=0x8c10f110u;if(*pr!=entry_pr)return -3;*stop=0x8c10f112u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_frame_setup_8c10e7b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10e7b8;
L8c10e7b8: *stop=0x8c10e7b8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10e7ba; /* mov.l r14,@-r15 */
L8c10e7ba: *stop=0x8c10e7bau;r[15]+=4294967292u;goto L8c10e7bc; /* add #-0x4,r15 */
L8c10e7bc: *stop=0x8c10e7bcu;r[6]=r[15];goto L8c10e7be; /* mov r15,r6 */
L8c10e7be: *stop=0x8c10e7beu;r[2]=rd(m,bus,&io_status,0x8c10e860u,4);if(io_status)return io_status;goto L8c10e7c0; /* mov.l 0x8c10e860,r2 */
L8c10e7c0: *stop=0x8c10e7c0u;r[3]=rd(m,bus,&io_status,0x8c10e858u,4);if(io_status)return io_status;goto L8c10e7c2; /* mov.l 0x8c10e858,r3 */
L8c10e7c2: *stop=0x8c10e7c2u;r[1]=rd(m,bus,&io_status,0x8c10e864u,4);if(io_status)return io_status;goto L8c10e7c4; /* mov.l 0x8c10e864,r1 */
L8c10e7c4: *stop=0x8c10e7c4u;r[5]=rd(m,bus,&io_status,0x8c10e85cu,4);if(io_status)return io_status;goto L8c10e7c6; /* mov.l 0x8c10e85c,r5 */
L8c10e7c6: *stop=0x8c10e7c6u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[6]);if(io_status)return io_status;goto L8c10e7c8; /* fmov.s fr4,@r6 */
L8c10e7c8: *stop=0x8c10e7c8u;if(!valid_access(r[6],4u))return -3;r[7]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c10e7ca; /* mov.l @r6,r7 */
L8c10e7ca: *stop=0x8c10e7cau;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[2]);if(io_status)return io_status;goto L8c10e7cc; /* fmov.s @r2,fr7 */
L8c10e7cc: *stop=0x8c10e7ccu;if(!valid_access(r[6],4u))return -3;r[14]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c10e7ce; /* mov.l @r6,r14 */
L8c10e7ce: *stop=0x8c10e7ceu;r[7]&=r[3];goto L8c10e7d0; /* and r3,r7 */
L8c10e7d0: *stop=0x8c10e7d0u;*sr=(*sr&~1u)|(r[3]==r[7]);goto L8c10e7d2; /* cmp/eq r3,r7 */
L8c10e7d2: take=!(*sr&1u);*stop=0x8c10e7d4u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;if(take)goto L8c10e7ee;goto L8c10e7d6; /* bf/s 0x8c10e7ee */
L8c10e7d6: *stop=0x8c10e7d6u;r[0]=rd(m,bus,&io_status,0x8c10e868u,4);if(io_status)return io_status;goto L8c10e7d8; /* mov.l 0x8c10e868,r0 */
L8c10e7d8: *stop=0x8c10e7d8u;*sr=(*sr&~1u)|!(r[14]&r[0]);goto L8c10e7da; /* tst r14,r0 */
L8c10e7da: if(!(*sr&1u))goto L8c10e7e4;goto L8c10e7dc; /* bf 0x8c10e7e4 */
L8c10e7dc: *stop=0x8c10e7dcu;*sr=(*sr&~1u)|!(r[14]&r[5]);goto L8c10e7de; /* tst r14,r5 */
L8c10e7de: if(!(*sr&1u))goto L8c10e822;goto L8c10e7e0; /* bf 0x8c10e822 */
L8c10e7e0: *stop=0x8c10e7e2u;goto L8c10e81a; /* bra 0x8c10e81a */
L8c10e7e4: *stop=0x8c10e7e4u;r[3]=rd(m,bus,&io_status,0x8c10e86cu,4);if(io_status)return io_status;goto L8c10e7e6; /* mov.l 0x8c10e86c,r3 */
L8c10e7e6: *stop=0x8c10e7e6u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[3]);if(io_status)return io_status;goto L8c10e7e8; /* fmov.s @r3,fr0 */
L8c10e7e8: *stop=0x8c10e7e8u;r[15]+=4u;goto L8c10e7ea; /* add #0x4,r15 */
L8c10e7ea: *stop=0x8c10e7eau;if(*pr!=entry_pr)return -3;*stop=0x8c10e7ecu;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
L8c10e7ee: *stop=0x8c10e7eeu;r[2]=rd(m,bus,&io_status,0x8c10e870u,4);if(io_status)return io_status;goto L8c10e7f0; /* mov.l 0x8c10e870,r2 */
L8c10e7f0: *stop=0x8c10e7f0u;*sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c10e7f2; /* cmp/pl r7 */
L8c10e7f2: *stop=0x8c10e7f2u;r[3]=rd(m,bus,&io_status,0x8c10e874u,4);if(io_status)return io_status;goto L8c10e7f4; /* mov.l 0x8c10e874,r3 */
L8c10e7f4: *stop=0x8c10e7f4u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[2]);if(io_status)return io_status;goto L8c10e7f6; /* fmov.s @r2,fr4 */
L8c10e7f6: take=(*sr&1u);*stop=0x8c10e7f8u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[3]);if(io_status)return io_status;if(take)goto L8c10e802;goto L8c10e7fa; /* bt/s 0x8c10e802 */
L8c10e7fa: *stop=0x8c10e7fau;*sr=(*sr&~1u)|!(r[14]&r[5]);goto L8c10e7fc; /* tst r14,r5 */
L8c10e7fc: if(!(*sr&1u))goto L8c10e836;goto L8c10e7fe; /* bf 0x8c10e836 */
L8c10e7fe: *stop=0x8c10e800u;goto L8c10e82e; /* bra 0x8c10e82e */
L8c10e802: *stop=0x8c10e802u;r[3]=4294967273u;goto L8c10e804; /* mov #-0x17,r3 */
L8c10e804: *stop=0x8c10e804u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[7]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[7]=n?(uint32_t)((int32_t)r[7]>>n):((r[7]&0x80000000u)?0xffffffffu:0u);}}goto L8c10e806; /* shad r3,r7 */
L8c10e806: *stop=0x8c10e806u;r[7]+=r[4];goto L8c10e808; /* add r4,r7 */
L8c10e808: *stop=0x8c10e808u;*sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c10e80a; /* cmp/pl r7 */
L8c10e80a: if(!(*sr&1u))goto L8c10e812;goto L8c10e80c; /* bf 0x8c10e812 */
L8c10e80c: *stop=0x8c10e80cu;r[3]=sx(m,bus,&io_status,0x8c10e856u,2);if(io_status)return io_status;goto L8c10e80e; /* mov.w 0x8c10e856,r3 */
L8c10e80e: *stop=0x8c10e80eu;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[3]);goto L8c10e810; /* cmp/ge r3,r7 */
L8c10e810: if(!(*sr&1u))goto L8c10e83e;goto L8c10e812; /* bf 0x8c10e83e */
L8c10e812: *stop=0x8c10e812u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c10e814; /* cmp/pz r4 */
L8c10e814: if(!(*sr&1u))goto L8c10e82a;goto L8c10e816; /* bf 0x8c10e82a */
L8c10e816: *stop=0x8c10e816u;*sr=(*sr&~1u)|!(r[14]&r[5]);goto L8c10e818; /* tst r14,r5 */
L8c10e818: if(!(*sr&1u))goto L8c10e822;goto L8c10e81a; /* bf 0x8c10e822 */
L8c10e81a: *stop=0x8c10e81au;fcopy(f,0u,7u);goto L8c10e81c; /* fmov fr7,fr0 */
L8c10e81c: *stop=0x8c10e81cu;r[15]+=4u;goto L8c10e81e; /* add #0x4,r15 */
L8c10e81e: *stop=0x8c10e81eu;if(*pr!=entry_pr)return -3;*stop=0x8c10e820u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
L8c10e822: *stop=0x8c10e822u;fcopy(f,0u,5u);goto L8c10e824; /* fmov fr5,fr0 */
L8c10e824: *stop=0x8c10e824u;r[15]+=4u;goto L8c10e826; /* add #0x4,r15 */
L8c10e826: *stop=0x8c10e826u;if(*pr!=entry_pr)return -3;*stop=0x8c10e828u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
L8c10e82a: *stop=0x8c10e82au;*sr=(*sr&~1u)|!(r[14]&r[5]);goto L8c10e82c; /* tst r14,r5 */
L8c10e82c: if(!(*sr&1u))goto L8c10e836;goto L8c10e82e; /* bf 0x8c10e836 */
L8c10e82e: *stop=0x8c10e82eu;fcopy(f,0u,4u);goto L8c10e830; /* fmov fr4,fr0 */
L8c10e830: *stop=0x8c10e830u;r[15]+=4u;goto L8c10e832; /* add #0x4,r15 */
L8c10e832: *stop=0x8c10e832u;if(*pr!=entry_pr)return -3;*stop=0x8c10e834u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
L8c10e836: *stop=0x8c10e836u;fcopy(f,0u,6u);goto L8c10e838; /* fmov fr6,fr0 */
L8c10e838: *stop=0x8c10e838u;r[15]+=4u;goto L8c10e83a; /* add #0x4,r15 */
L8c10e83a: *stop=0x8c10e83au;if(*pr!=entry_pr)return -3;*stop=0x8c10e83cu;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
L8c10e83e: *stop=0x8c10e83eu;r[2]=rd(m,bus,&io_status,0x8c10e868u,4);if(io_status)return io_status;goto L8c10e840; /* mov.l 0x8c10e868,r2 */
L8c10e840: *stop=0x8c10e840u;r[3]=23u;goto L8c10e842; /* mov #0x17,r3 */
L8c10e842: *stop=0x8c10e842u;r[5]&=r[14];goto L8c10e844; /* and r14,r5 */
L8c10e844: *stop=0x8c10e844u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[7]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[7]=n?(uint32_t)((int32_t)r[7]>>n):((r[7]&0x80000000u)?0xffffffffu:0u);}}goto L8c10e846; /* shad r3,r7 */
L8c10e846: *stop=0x8c10e846u;r[2]&=r[14];goto L8c10e848; /* and r14,r2 */
L8c10e848: *stop=0x8c10e848u;r[5]|=r[7];goto L8c10e84a; /* or r7,r5 */
L8c10e84a: *stop=0x8c10e84au;r[5]|=r[2];goto L8c10e84c; /* or r2,r5 */
L8c10e84c: *stop=0x8c10e84cu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[5],4);if(io_status)return io_status;goto L8c10e84e; /* mov.l r5,@r6 */
L8c10e84e: *stop=0x8c10e84eu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[6]);if(io_status)return io_status;goto L8c10e850; /* fmov.s @r6,fr0 */
L8c10e850: *stop=0x8c10e850u;r[15]+=4u;goto L8c10e852; /* add #0x4,r15 */
L8c10e852: *stop=0x8c10e852u;if(*pr!=entry_pr)return -3;*stop=0x8c10e854u;if(!valid_access(r[15],4u))return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
