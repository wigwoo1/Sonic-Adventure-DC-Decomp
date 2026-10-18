/* Material/header/colour-cache helpers; explicit GBR/queue state.
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
SA_STAGE_EXPORT int sa_material_setup_8c037618(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c037618;
L8c037618: *stop=0x8c037618u;r[1]=rd(m,bus,&io_status,0x8c03762cu,4);if(io_status)return io_status;goto L8c03761a; /* mov.l 0x8c03762c,r1 */
L8c03761a: *stop=0x8c03761au;if(!valid_access(r[1]+8u,4u))return -3;r[8]=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;goto L8c03761c; /* mov.l @(0x8,r1),r8 */
L8c03761c: *stop=0x8c03761cu;r[9]=rd(m,bus,&io_status,0x8c037630u,4);if(io_status)return io_status;goto L8c03761e; /* mov.l 0x8c037630,r9 */
L8c03761e: *stop=0x8c03761eu;r[8]&=r[9];goto L8c037620; /* and r9,r8 */
L8c037620: *stop=0x8c037620u;r[9]=rd(m,bus,&io_status,0x8c037634u,4);if(io_status)return io_status;goto L8c037622; /* mov.l 0x8c037634,r9 */
L8c037622: *stop=0x8c037622u;r[9]&=r[0];goto L8c037624; /* and r0,r9 */
L8c037624: *stop=0x8c037624u;r[8]|=r[9];goto L8c037626; /* or r9,r8 */
L8c037626: *stop=0x8c037626u;if(*pr!=entry_pr)return -3;*stop=0x8c037628u;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[8],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_setup_8c037638(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c037638;
L8c037638: *stop=0x8c037638u;r[1]=rd(m,bus,&io_status,0x8c037670u,4);if(io_status)return io_status;goto L8c03763a; /* mov.l 0x8c037670,r1 */
L8c03763a: *stop=0x8c03763au;if(!valid_access(r[1],4u))return -3;r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c03763c; /* mov.l @r1,r1 */
L8c03763c: *stop=0x8c03763cu;if(!valid_access(r[1]+0u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0x0u,4);if(io_status)return io_status;goto L8c03763e; /* mov.l @(0x0,r1),r1 */
L8c03763e: *stop=0x8c03763eu;r[0]<<=2;goto L8c037640; /* shll2 r0 */
L8c037640: *stop=0x8c037640u;r[8]=r[0];goto L8c037642; /* mov r0,r8 */
L8c037642: *stop=0x8c037642u;*sr=(*sr&~1u)|(r[8]>>31);r[8]<<=1;goto L8c037644; /* shll r8 */
L8c037644: *stop=0x8c037644u;r[0]+=r[8];goto L8c037646; /* add r8,r0 */
L8c037646: *stop=0x8c037646u;r[1]+=r[0];goto L8c037648; /* add r0,r1 */
L8c037648: *stop=0x8c037648u;if(!valid_access(r[1]+8u,4u))return -3;r[1]=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;goto L8c03764a; /* mov.l @(0x8,r1),r1 */
L8c03764a: *stop=0x8c03764au;if(!valid_access(r[1]+0u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x0u,4);if(io_status)return io_status;goto L8c03764c; /* mov.l @(0x0,r1),r0 */
L8c03764c: *stop=0x8c03764cu;r[8]=rd(m,bus,&io_status,0x8c037674u,4);if(io_status)return io_status;goto L8c03764e; /* mov.l 0x8c037674,r8 */
L8c03764e: *stop=0x8c03764eu;if(!valid_access(r[8],4u))return -3;r[9]=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;goto L8c037650; /* mov.l @r8,r9 */
L8c037650: *stop=0x8c037650u;*sr=(*sr&~1u)|(r[9]==r[0]);goto L8c037652; /* cmp/eq r9,r0 */
L8c037652: if((*sr&1u))goto L8c03766a;goto L8c037654; /* bt 0x8c03766a */
L8c037654: *stop=0x8c037654u;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c037656; /* mov.l r0,@r8 */
L8c037656: *stop=0x8c037656u;r[9]=rd(m,bus,&io_status,0x8c037678u,4);if(io_status)return io_status;goto L8c037658; /* mov.l 0x8c037678,r9 */
L8c037658: *stop=0x8c037658u;if(!valid_access(r[9]+8u,4u))return -3;r[8]=rd(m,bus,&io_status,r[9]+0x8u,4);if(io_status)return io_status;goto L8c03765a; /* mov.l @(0x8,r9),r8 */
L8c03765a: *stop=0x8c03765au;r[0]=4294967232u;goto L8c03765c; /* mov #-0x40,r0 */
L8c03765c: *stop=0x8c03765cu;r[8]&=r[0];goto L8c03765e; /* and r0,r8 */
L8c03765e: *stop=0x8c03765eu;if(!valid_access(r[1]+8u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;goto L8c037660; /* mov.l @(0x8,r1),r0 */
L8c037660: *stop=0x8c037660u;r[0]&=0x3fu;goto L8c037662; /* and #0x3f,r0 */
L8c037662: *stop=0x8c037662u;r[8]|=r[0];goto L8c037664; /* or r0,r8 */
L8c037664: *stop=0x8c037664u;if(!valid_access(r[9]+8u,4u))return -3;wr(m,bus,&io_status,r[9]+0x8u,r[8],4);if(io_status)return io_status;goto L8c037666; /* mov.l r8,@(0x8,r9) */
L8c037666: *stop=0x8c037666u;if(!valid_access(r[1]+12u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c037668; /* mov.l @(0xc,r1),r0 */
L8c037668: *stop=0x8c037668u;if(!valid_access(r[9]+12u,4u))return -3;wr(m,bus,&io_status,r[9]+0xcu,r[0],4);if(io_status)return io_status;goto L8c03766a; /* mov.l r0,@(0xc,r9) */
L8c03766a: *stop=0x8c03766au;if(*pr!=entry_pr)return -3;*stop=0x8c03766cu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_setup_8c03767c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c03767c;
L8c03767c: *stop=0x8c03767cu;r[1]=rd(m,bus,&io_status,0x8c03769cu,4);if(io_status)return io_status;goto L8c03767e; /* mov.l 0x8c03769c,r1 */
L8c03767e: *stop=0x8c03767eu;if(!valid_access(r[1]+0u,4u))return -3;r[8]=rd(m,bus,&io_status,r[1]+0x0u,4);if(io_status)return io_status;goto L8c037680; /* mov.l @(0x0,r1),r8 */
L8c037680: *stop=0x8c037680u;if(!valid_access(r[1]+8u,4u))return -3;r[9]=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;goto L8c037682; /* mov.l @(0x8,r1),r9 */
L8c037682: *stop=0x8c037682u;r[10]=rd(m,bus,&io_status,0x8c0376a0u,4);if(io_status)return io_status;goto L8c037684; /* mov.l 0x8c0376a0,r10 */
L8c037684: *stop=0x8c037684u;r[8]&=r[10];goto L8c037686; /* and r10,r8 */
L8c037686: *stop=0x8c037686u;r[10]=rd(m,bus,&io_status,0x8c0376a4u,4);if(io_status)return io_status;goto L8c037688; /* mov.l 0x8c0376a4,r10 */
L8c037688: *stop=0x8c037688u;r[9]&=r[10];goto L8c03768a; /* and r10,r9 */
L8c03768a: *stop=0x8c03768au;r[10]=rd(m,bus,&io_status,0x8c0376a8u,4);if(io_status)return io_status;goto L8c03768c; /* mov.l 0x8c0376a8,r10 */
L8c03768c: *stop=0x8c03768cu;r[10]&=r[0];goto L8c03768e; /* and r0,r10 */
L8c03768e: *stop=0x8c03768eu;r[8]|=r[10];goto L8c037690; /* or r10,r8 */
L8c037690: *stop=0x8c037690u;r[11]=rd(m,bus,&io_status,0x8c0376acu,4);if(io_status)return io_status;goto L8c037692; /* mov.l 0x8c0376ac,r11 */
L8c037692: *stop=0x8c037692u;r[11]&=r[0];goto L8c037694; /* and r0,r11 */
L8c037694: *stop=0x8c037694u;r[9]|=r[11];goto L8c037696; /* or r11,r9 */
L8c037696: *stop=0x8c037696u;if(!valid_access(r[1]+0u,4u))return -3;wr(m,bus,&io_status,r[1]+0x0u,r[8],4);if(io_status)return io_status;goto L8c037698; /* mov.l r8,@(0x0,r1) */
L8c037698: *stop=0x8c037698u;if(*pr!=entry_pr)return -3;*stop=0x8c03769au;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[9],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_setup_8c0376b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0376b0;
L8c0376b0: *stop=0x8c0376b0u;r[1]=rd(m,bus,&io_status,0x8c0376c4u,4);if(io_status)return io_status;goto L8c0376b2; /* mov.l 0x8c0376c4,r1 */
L8c0376b2: *stop=0x8c0376b2u;r[0]=rd(m,bus,&io_status,0x8c0376c8u,4);if(io_status)return io_status;goto L8c0376b4; /* mov.l 0x8c0376c8,r0 */
L8c0376b4: *stop=0x8c0376b4u;r[0]&=r[14];goto L8c0376b6; /* and r14,r0 */
L8c0376b6: *stop=0x8c0376b6u;if(!valid_access(r[1],4u))return -3;r[8]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c0376b8; /* mov.l @r1,r8 */
L8c0376b8: *stop=0x8c0376b8u;r[0]|=r[8];goto L8c0376ba; /* or r8,r0 */
L8c0376ba: *stop=0x8c0376bau;r[1]=rd(m,bus,&io_status,0x8c0376ccu,4);if(io_status)return io_status;goto L8c0376bc; /* mov.l 0x8c0376cc,r1 */
L8c0376bc: *stop=0x8c0376bcu;if(!valid_access(r[1],4u))return -3;r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c0376be; /* mov.l @r1,r1 */
L8c0376be: *stop=0x8c0376beu;if(*pr!=entry_pr)return -3;*stop=0x8c0376c0u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_setup_8c037460(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c037460;
L8c037460: *stop=0x8c037460u;r[1]=rd(m,bus,&io_status,0x8c0374a8u,4);if(io_status)return io_status;goto L8c037462; /* mov.l 0x8c0374a8,r1 */
L8c037462: *stop=0x8c037462u;if(!valid_access(r[1],4u))return -3;r[8]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c037464; /* mov.l @r1,r8 */
L8c037464: *stop=0x8c037464u;r[0]=rd(m,bus,&io_status,0x8c0374acu,4);if(io_status)return io_status;goto L8c037466; /* mov.l 0x8c0374ac,r0 */
L8c037466: *stop=0x8c037466u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c037468; /* mov.l r1,@r0 */
L8c037468: *stop=0x8c037468u;r[0]=rd(m,bus,&io_status,0x8c0374b0u,4);if(io_status)return io_status;goto L8c03746a; /* mov.l 0x8c0374b0,r0 */
L8c03746a: *stop=0x8c03746au;r[0]&=r[8];goto L8c03746c; /* and r8,r0 */
L8c03746c: *stop=0x8c03746cu;r[1]=rd(m,bus,&io_status,0x8c0374b4u,4);if(io_status)return io_status;goto L8c03746e; /* mov.l 0x8c0374b4,r1 */
L8c03746e: *stop=0x8c03746eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037470; /* mov.l r0,@r1 */
L8c037470: *stop=0x8c037470u;r[0]=r[8];goto L8c037472; /* mov r8,r0 */
L8c037472: *stop=0x8c037472u;r[0]>>=16;goto L8c037474; /* shlr16 r0 */
L8c037474: *stop=0x8c037474u;r[0]>>=8;goto L8c037476; /* shlr8 r0 */
L8c037476: *stop=0x8c037476u;r[0]&=0x1cu;goto L8c037478; /* and #0x1c,r0 */
L8c037478: *stop=0x8c037478u;r[1]=rd(m,bus,&io_status,0x8c0374b8u,4);if(io_status)return io_status;goto L8c03747a; /* mov.l 0x8c0374b8,r1 */
L8c03747a: *stop=0x8c03747au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c03747c; /* mov.l r0,@r1 */
L8c03747c: *stop=0x8c03747cu;r[1]=rd(m,bus,&io_status,0x8c0374bcu,4);if(io_status)return io_status;goto L8c03747e; /* mov.l 0x8c0374bc,r1 */
L8c03747e: *stop=0x8c03747eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037480; /* mov.l r0,@r1 */
L8c037480: *stop=0x8c037480u;r[1]=rd(m,bus,&io_status,0x8c0374c0u,4);if(io_status)return io_status;goto L8c037482; /* mov.l 0x8c0374c0,r1 */
L8c037482: *stop=0x8c037482u;r[0]=rd(m,bus,&io_status,0x8c0374c4u,4);if(io_status)return io_status;goto L8c037484; /* mov.l 0x8c0374c4,r0 */
L8c037484: *stop=0x8c037484u;r[8]&=r[1];goto L8c037486; /* and r1,r8 */
L8c037486: *stop=0x8c037486u;r[8]|=r[0];goto L8c037488; /* or r0,r8 */
L8c037488: *stop=0x8c037488u;r[8]+=12u;goto L8c03748a; /* add #0xc,r8 */
L8c03748a: *stop=0x8c03748au;r[1]=rd(m,bus,&io_status,0x8c0374c8u,4);if(io_status)return io_status;goto L8c03748c; /* mov.l 0x8c0374c8,r1 */
L8c03748c: *stop=0x8c03748cu;if(!valid_access(r[1]+8u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;goto L8c03748e; /* mov.l @(0x8,r1),r0 */
L8c03748e: *stop=0x8c03748eu;r[9]=rd(m,bus,&io_status,0x8c0374ccu,4);if(io_status)return io_status;goto L8c037490; /* mov.l 0x8c0374cc,r9 */
L8c037490: *stop=0x8c037490u;r[0]&=r[9];goto L8c037492; /* and r9,r0 */
L8c037492: *stop=0x8c037492u;r[9]=rd(m,bus,&io_status,0x8c0374d0u,4);if(io_status)return io_status;goto L8c037494; /* mov.l 0x8c0374d0,r9 */
L8c037494: *stop=0x8c037494u;r[0]|=r[9];goto L8c037496; /* or r9,r0 */
L8c037496: *stop=0x8c037496u;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c037498; /* mov.l r0,@-r8 */
L8c037498: *stop=0x8c037498u;if(!valid_access(r[1]+4u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;goto L8c03749a; /* mov.l @(0x4,r1),r0 */
L8c03749a: *stop=0x8c03749au;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c03749c; /* mov.l r0,@-r8 */
L8c03749c: *stop=0x8c03749cu;if(!valid_access(r[1]+0u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x0u,4);if(io_status)return io_status;goto L8c03749e; /* mov.l @(0x0,r1),r0 */
L8c03749e: *stop=0x8c03749eu;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c0374a0; /* mov.l r0,@-r8 */
L8c0374a0: *stop=0x8c0374a0u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c0374a2; /* pref @r8 */
L8c0374a2: *stop=0x8c0374a2u;r[8]+=32u;goto L8c0374a4; /* add #0x20,r8 */
L8c0374a4: *stop=0x8c0374a4u;if(*pr!=entry_pr)return -3;*stop=0x8c0374a6u;r[0]=r[8];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_setup_8c0374d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0374d4;
L8c0374d4: *stop=0x8c0374d4u;r[1]=rd(m,bus,&io_status,0x8c037514u,4);if(io_status)return io_status;goto L8c0374d6; /* mov.l 0x8c037514,r1 */
L8c0374d6: *stop=0x8c0374d6u;if(!valid_access(r[1],4u))return -3;r[8]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c0374d8; /* mov.l @r1,r8 */
L8c0374d8: *stop=0x8c0374d8u;r[0]=rd(m,bus,&io_status,0x8c037518u,4);if(io_status)return io_status;goto L8c0374da; /* mov.l 0x8c037518,r0 */
L8c0374da: *stop=0x8c0374dau;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c0374dc; /* mov.l r1,@r0 */
L8c0374dc: *stop=0x8c0374dcu;r[0]=rd(m,bus,&io_status,0x8c03751cu,4);if(io_status)return io_status;goto L8c0374de; /* mov.l 0x8c03751c,r0 */
L8c0374de: *stop=0x8c0374deu;r[0]&=r[8];goto L8c0374e0; /* and r8,r0 */
L8c0374e0: *stop=0x8c0374e0u;r[1]=rd(m,bus,&io_status,0x8c037520u,4);if(io_status)return io_status;goto L8c0374e2; /* mov.l 0x8c037520,r1 */
L8c0374e2: *stop=0x8c0374e2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c0374e4; /* mov.l r0,@r1 */
L8c0374e4: *stop=0x8c0374e4u;r[0]=r[8];goto L8c0374e6; /* mov r8,r0 */
L8c0374e6: *stop=0x8c0374e6u;r[0]>>=16;goto L8c0374e8; /* shlr16 r0 */
L8c0374e8: *stop=0x8c0374e8u;r[0]>>=8;goto L8c0374ea; /* shlr8 r0 */
L8c0374ea: *stop=0x8c0374eau;r[0]&=0x1cu;goto L8c0374ec; /* and #0x1c,r0 */
L8c0374ec: *stop=0x8c0374ecu;r[1]=rd(m,bus,&io_status,0x8c037524u,4);if(io_status)return io_status;goto L8c0374ee; /* mov.l 0x8c037524,r1 */
L8c0374ee: *stop=0x8c0374eeu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c0374f0; /* mov.l r0,@r1 */
L8c0374f0: *stop=0x8c0374f0u;r[1]=rd(m,bus,&io_status,0x8c037528u,4);if(io_status)return io_status;goto L8c0374f2; /* mov.l 0x8c037528,r1 */
L8c0374f2: *stop=0x8c0374f2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c0374f4; /* mov.l r0,@r1 */
L8c0374f4: *stop=0x8c0374f4u;r[1]=rd(m,bus,&io_status,0x8c03752cu,4);if(io_status)return io_status;goto L8c0374f6; /* mov.l 0x8c03752c,r1 */
L8c0374f6: *stop=0x8c0374f6u;r[0]=rd(m,bus,&io_status,0x8c037530u,4);if(io_status)return io_status;goto L8c0374f8; /* mov.l 0x8c037530,r0 */
L8c0374f8: *stop=0x8c0374f8u;r[8]&=r[1];goto L8c0374fa; /* and r1,r8 */
L8c0374fa: *stop=0x8c0374fau;r[8]|=r[0];goto L8c0374fc; /* or r0,r8 */
L8c0374fc: *stop=0x8c0374fcu;r[8]+=12u;goto L8c0374fe; /* add #0xc,r8 */
L8c0374fe: *stop=0x8c0374feu;r[1]=rd(m,bus,&io_status,0x8c037534u,4);if(io_status)return io_status;goto L8c037500; /* mov.l 0x8c037534,r1 */
L8c037500: *stop=0x8c037500u;if(!valid_access(r[1]+8u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;goto L8c037502; /* mov.l @(0x8,r1),r0 */
L8c037502: *stop=0x8c037502u;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c037504; /* mov.l r0,@-r8 */
L8c037504: *stop=0x8c037504u;if(!valid_access(r[1]+4u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;goto L8c037506; /* mov.l @(0x4,r1),r0 */
L8c037506: *stop=0x8c037506u;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c037508; /* mov.l r0,@-r8 */
L8c037508: *stop=0x8c037508u;if(!valid_access(r[1]+0u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x0u,4);if(io_status)return io_status;goto L8c03750a; /* mov.l @(0x0,r1),r0 */
L8c03750a: *stop=0x8c03750au;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c03750c; /* mov.l r0,@-r8 */
L8c03750c: *stop=0x8c03750cu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c03750e; /* pref @r8 */
L8c03750e: *stop=0x8c03750eu;r[8]+=32u;goto L8c037510; /* add #0x20,r8 */
L8c037510: *stop=0x8c037510u;if(*pr!=entry_pr)return -3;*stop=0x8c037512u;r[0]=r[8];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_setup_8c037538(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c037538;
L8c037538: *stop=0x8c037538u;r[1]=rd(m,bus,&io_status,0x8c037584u,4);if(io_status)return io_status;goto L8c03753a; /* mov.l 0x8c037584,r1 */
L8c03753a: *stop=0x8c03753au;if(!valid_access(r[1],4u))return -3;r[8]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c03753c; /* mov.l @r1,r8 */
L8c03753c: *stop=0x8c03753cu;r[0]=rd(m,bus,&io_status,0x8c037588u,4);if(io_status)return io_status;goto L8c03753e; /* mov.l 0x8c037588,r0 */
L8c03753e: *stop=0x8c03753eu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c037540; /* mov.l r1,@r0 */
L8c037540: *stop=0x8c037540u;r[0]=rd(m,bus,&io_status,0x8c03758cu,4);if(io_status)return io_status;goto L8c037542; /* mov.l 0x8c03758c,r0 */
L8c037542: *stop=0x8c037542u;r[0]&=r[8];goto L8c037544; /* and r8,r0 */
L8c037544: *stop=0x8c037544u;r[1]=rd(m,bus,&io_status,0x8c037590u,4);if(io_status)return io_status;goto L8c037546; /* mov.l 0x8c037590,r1 */
L8c037546: *stop=0x8c037546u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037548; /* mov.l r0,@r1 */
L8c037548: *stop=0x8c037548u;r[0]=r[8];goto L8c03754a; /* mov r8,r0 */
L8c03754a: *stop=0x8c03754au;r[0]>>=16;goto L8c03754c; /* shlr16 r0 */
L8c03754c: *stop=0x8c03754cu;r[0]>>=8;goto L8c03754e; /* shlr8 r0 */
L8c03754e: *stop=0x8c03754eu;r[0]&=0x1cu;goto L8c037550; /* and #0x1c,r0 */
L8c037550: *stop=0x8c037550u;r[1]=rd(m,bus,&io_status,0x8c037594u,4);if(io_status)return io_status;goto L8c037552; /* mov.l 0x8c037594,r1 */
L8c037552: *stop=0x8c037552u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037554; /* mov.l r0,@r1 */
L8c037554: *stop=0x8c037554u;r[1]=rd(m,bus,&io_status,0x8c037598u,4);if(io_status)return io_status;goto L8c037556; /* mov.l 0x8c037598,r1 */
L8c037556: *stop=0x8c037556u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037558; /* mov.l r0,@r1 */
L8c037558: *stop=0x8c037558u;r[1]=rd(m,bus,&io_status,0x8c03759cu,4);if(io_status)return io_status;goto L8c03755a; /* mov.l 0x8c03759c,r1 */
L8c03755a: *stop=0x8c03755au;r[0]=rd(m,bus,&io_status,0x8c0375a0u,4);if(io_status)return io_status;goto L8c03755c; /* mov.l 0x8c0375a0,r0 */
L8c03755c: *stop=0x8c03755cu;r[8]&=r[1];goto L8c03755e; /* and r1,r8 */
L8c03755e: *stop=0x8c03755eu;r[8]|=r[0];goto L8c037560; /* or r0,r8 */
L8c037560: *stop=0x8c037560u;r[8]+=16u;goto L8c037562; /* add #0x10,r8 */
L8c037562: *stop=0x8c037562u;r[1]=rd(m,bus,&io_status,0x8c0375a4u,4);if(io_status)return io_status;goto L8c037564; /* mov.l 0x8c0375a4,r1 */
L8c037564: *stop=0x8c037564u;if(!valid_access(r[1]+12u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c037566; /* mov.l @(0xc,r1),r0 */
L8c037566: *stop=0x8c037566u;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c037568; /* mov.l r0,@-r8 */
L8c037568: *stop=0x8c037568u;if(!valid_access(r[1]+8u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;goto L8c03756a; /* mov.l @(0x8,r1),r0 */
L8c03756a: *stop=0x8c03756au;r[9]=rd(m,bus,&io_status,0x8c0375a8u,4);if(io_status)return io_status;goto L8c03756c; /* mov.l 0x8c0375a8,r9 */
L8c03756c: *stop=0x8c03756cu;r[0]&=r[9];goto L8c03756e; /* and r9,r0 */
L8c03756e: *stop=0x8c03756eu;r[9]=rd(m,bus,&io_status,0x8c0375acu,4);if(io_status)return io_status;goto L8c037570; /* mov.l 0x8c0375ac,r9 */
L8c037570: *stop=0x8c037570u;r[0]|=r[9];goto L8c037572; /* or r9,r0 */
L8c037572: *stop=0x8c037572u;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c037574; /* mov.l r0,@-r8 */
L8c037574: *stop=0x8c037574u;if(!valid_access(r[1]+4u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;goto L8c037576; /* mov.l @(0x4,r1),r0 */
L8c037576: *stop=0x8c037576u;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c037578; /* mov.l r0,@-r8 */
L8c037578: *stop=0x8c037578u;if(!valid_access(r[1]+0u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x0u,4);if(io_status)return io_status;goto L8c03757a; /* mov.l @(0x0,r1),r0 */
L8c03757a: *stop=0x8c03757au;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c03757c; /* mov.l r0,@-r8 */
L8c03757c: *stop=0x8c03757cu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c03757e; /* pref @r8 */
L8c03757e: *stop=0x8c03757eu;r[8]+=32u;goto L8c037580; /* add #0x20,r8 */
L8c037580: *stop=0x8c037580u;if(*pr!=entry_pr)return -3;*stop=0x8c037582u;r[0]=r[8];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_setup_8c0375b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0375b0;
L8c0375b0: *stop=0x8c0375b0u;r[1]=rd(m,bus,&io_status,0x8c0375f4u,4);if(io_status)return io_status;goto L8c0375b2; /* mov.l 0x8c0375f4,r1 */
L8c0375b2: *stop=0x8c0375b2u;if(!valid_access(r[1],4u))return -3;r[8]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c0375b4; /* mov.l @r1,r8 */
L8c0375b4: *stop=0x8c0375b4u;r[0]=rd(m,bus,&io_status,0x8c0375f8u,4);if(io_status)return io_status;goto L8c0375b6; /* mov.l 0x8c0375f8,r0 */
L8c0375b6: *stop=0x8c0375b6u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c0375b8; /* mov.l r1,@r0 */
L8c0375b8: *stop=0x8c0375b8u;r[0]=rd(m,bus,&io_status,0x8c0375fcu,4);if(io_status)return io_status;goto L8c0375ba; /* mov.l 0x8c0375fc,r0 */
L8c0375ba: *stop=0x8c0375bau;r[0]&=r[8];goto L8c0375bc; /* and r8,r0 */
L8c0375bc: *stop=0x8c0375bcu;r[1]=rd(m,bus,&io_status,0x8c037600u,4);if(io_status)return io_status;goto L8c0375be; /* mov.l 0x8c037600,r1 */
L8c0375be: *stop=0x8c0375beu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c0375c0; /* mov.l r0,@r1 */
L8c0375c0: *stop=0x8c0375c0u;r[0]=r[8];goto L8c0375c2; /* mov r8,r0 */
L8c0375c2: *stop=0x8c0375c2u;r[0]>>=16;goto L8c0375c4; /* shlr16 r0 */
L8c0375c4: *stop=0x8c0375c4u;r[0]>>=8;goto L8c0375c6; /* shlr8 r0 */
L8c0375c6: *stop=0x8c0375c6u;r[0]&=0x1cu;goto L8c0375c8; /* and #0x1c,r0 */
L8c0375c8: *stop=0x8c0375c8u;r[1]=rd(m,bus,&io_status,0x8c037604u,4);if(io_status)return io_status;goto L8c0375ca; /* mov.l 0x8c037604,r1 */
L8c0375ca: *stop=0x8c0375cau;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c0375cc; /* mov.l r0,@r1 */
L8c0375cc: *stop=0x8c0375ccu;r[1]=rd(m,bus,&io_status,0x8c037608u,4);if(io_status)return io_status;goto L8c0375ce; /* mov.l 0x8c037608,r1 */
L8c0375ce: *stop=0x8c0375ceu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c0375d0; /* mov.l r0,@r1 */
L8c0375d0: *stop=0x8c0375d0u;r[1]=rd(m,bus,&io_status,0x8c03760cu,4);if(io_status)return io_status;goto L8c0375d2; /* mov.l 0x8c03760c,r1 */
L8c0375d2: *stop=0x8c0375d2u;r[0]=rd(m,bus,&io_status,0x8c037610u,4);if(io_status)return io_status;goto L8c0375d4; /* mov.l 0x8c037610,r0 */
L8c0375d4: *stop=0x8c0375d4u;r[8]&=r[1];goto L8c0375d6; /* and r1,r8 */
L8c0375d6: *stop=0x8c0375d6u;r[8]|=r[0];goto L8c0375d8; /* or r0,r8 */
L8c0375d8: *stop=0x8c0375d8u;r[8]+=16u;goto L8c0375da; /* add #0x10,r8 */
L8c0375da: *stop=0x8c0375dau;r[1]=rd(m,bus,&io_status,0x8c037614u,4);if(io_status)return io_status;goto L8c0375dc; /* mov.l 0x8c037614,r1 */
L8c0375dc: *stop=0x8c0375dcu;if(!valid_access(r[1]+12u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c0375de; /* mov.l @(0xc,r1),r0 */
L8c0375de: *stop=0x8c0375deu;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c0375e0; /* mov.l r0,@-r8 */
L8c0375e0: *stop=0x8c0375e0u;if(!valid_access(r[1]+8u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;goto L8c0375e2; /* mov.l @(0x8,r1),r0 */
L8c0375e2: *stop=0x8c0375e2u;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c0375e4; /* mov.l r0,@-r8 */
L8c0375e4: *stop=0x8c0375e4u;if(!valid_access(r[1]+4u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x4u,4);if(io_status)return io_status;goto L8c0375e6; /* mov.l @(0x4,r1),r0 */
L8c0375e6: *stop=0x8c0375e6u;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c0375e8; /* mov.l r0,@-r8 */
L8c0375e8: *stop=0x8c0375e8u;if(!valid_access(r[1]+0u,4u))return -3;r[0]=rd(m,bus,&io_status,r[1]+0x0u,4);if(io_status)return io_status;goto L8c0375ea; /* mov.l @(0x0,r1),r0 */
L8c0375ea: *stop=0x8c0375eau;if(!valid_access(r[8]-4u,4u))return -3;r[8]-=4;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c0375ec; /* mov.l r0,@-r8 */
L8c0375ec: *stop=0x8c0375ecu;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c0375ee; /* pref @r8 */
L8c0375ee: *stop=0x8c0375eeu;r[8]+=32u;goto L8c0375f0; /* add #0x20,r8 */
L8c0375f0: *stop=0x8c0375f0u;if(*pr!=entry_pr)return -3;*stop=0x8c0375f2u;r[0]=r[8];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_material_setup_8c037872(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c037872;
L8c037872: *stop=0x8c037872u;if(!valid_ram(*gbr+0x2cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x2cu,4);if(io_status)return io_status;goto L8c037874; /* mov.l @(0x2c,GBR),r0 */
L8c037874: *stop=0x8c037874u;r[8]=rd(m,bus,&io_status,0x8c0378e4u,4);if(io_status)return io_status;goto L8c037876; /* mov.l 0x8c0378e4,r8 */
L8c037876: *stop=0x8c037876u;*sr=(*sr&~1u)|!(r[8]&r[0]);goto L8c037878; /* tst r8,r0 */
L8c037878: if(!(*sr&1u))goto L8c0378f4;goto L8c03787a; /* bf 0x8c0378f4 */
L8c03787a: *stop=0x8c03787au;if(!valid_access(r[5]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x3u,1);if(io_status)return io_status;goto L8c03787c; /* mov.b @(0x3,r5),r0 */
L8c03787c: *stop=0x8c03787cu;r[0]=r[0]&255u;goto L8c03787e; /* extu.b r0,r0 */
L8c03787e: *stop=0x8c03787eu;*fpul=r[0];goto L8c037880; /* lds r0,FPUL */
L8c037880: *stop=0x8c037880u;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][0]);if(status)return status;goto L8c037882; /* float FPUL,fr0 */
L8c037882: *stop=0x8c037882u;r[0]=rd(m,bus,&io_status,0x8c0378e8u,4);if(io_status)return io_status;goto L8c037884; /* mov.l 0x8c0378e8,r0 */
L8c037884: *stop=0x8c037884u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;goto L8c037886; /* fmov.s @r0,fr1 */
L8c037886: *stop=0x8c037886u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[1],&f->fpscr,&fr[0]);if(status)return status;}goto L8c037888; /* fmul fr1,fr0 */
L8c037888: *stop=0x8c037888u;if(!valid_ram(*gbr+0x28u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x28u,4);if(io_status)return io_status;goto L8c03788a; /* mov.l @(0x28,GBR),r0 */
L8c03788a: *stop=0x8c03788au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c03788c; /* tst r0,r0 */
L8c03788c: if((*sr&1u))goto L8c0378b0;goto L8c03788e; /* bt 0x8c0378b0 */
L8c03788e: *stop=0x8c03788eu;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c037890; /* cmp/pl r0 */
L8c037890: if(!(*sr&1u))goto L8c037896;goto L8c037892; /* bf 0x8c037896 */
L8c037892: *stop=0x8c037892u;r[8]=rd(m,bus,&io_status,0x8c0378ecu,4);if(io_status)return io_status;goto L8c037894; /* mov.l 0x8c0378ec,r8 */
L8c037894: *stop=0x8c037894u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c037896; /* fmov.s @r8,fr0 */
L8c037896: *stop=0x8c037896u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c037898; /* cmp/pz r0 */
L8c037898: if((*sr&1u))goto L8c0378b0;goto L8c03789a; /* bt 0x8c0378b0 */
L8c03789a: *stop=0x8c03789au;r[8]=rd(m,bus,&io_status,0x8c0378ecu,4);if(io_status)return io_status;goto L8c03789c; /* mov.l 0x8c0378ec,r8 */
L8c03789c: *stop=0x8c03789cu;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c03789e; /* fmov.s @r8,fr1 */
L8c03789e: *stop=0x8c03789eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c0378a0; /* fadd fr1,fr0 */
L8c0378a0: *stop=0x8c0378a0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0x3f800000u;goto L8c0378a2; /* fldi1 fr1 */
L8c0378a2: *stop=0x8c0378a2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c0378a4; /* fcmp/gt fr1,fr0 */
L8c0378a4: if(!(*sr&1u))goto L8c0378a8;goto L8c0378a6; /* bf 0x8c0378a8 */
L8c0378a6: *stop=0x8c0378a6u;fcopy(f,0u,1u);goto L8c0378a8; /* fmov fr1,fr0 */
L8c0378a8: *stop=0x8c0378a8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0;goto L8c0378aa; /* fldi0 fr1 */
L8c0378aa: *stop=0x8c0378aau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c0378ac; /* fcmp/gt fr1,fr0 */
L8c0378ac: if((*sr&1u))goto L8c0378b0;goto L8c0378ae; /* bt 0x8c0378b0 */
L8c0378ae: *stop=0x8c0378aeu;fcopy(f,0u,1u);goto L8c0378b0; /* fmov fr1,fr0 */
L8c0378b0: *stop=0x8c0378b0u;if(!valid_ram(*gbr+0x3cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x3cu,4);if(io_status)return io_status;goto L8c0378b2; /* mov.l @(0x3c,GBR),r0 */
L8c0378b2: *stop=0x8c0378b2u;r[1]=r[0];goto L8c0378b4; /* mov r0,r1 */
L8c0378b4: *stop=0x8c0378b4u;r[0]=rd(m,bus,&io_status,0x8c0378f0u,4);if(io_status)return io_status;goto L8c0378b6; /* mov.l 0x8c0378f0,r0 */
L8c0378b6: *stop=0x8c0378b6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;goto L8c0378b8; /* fmov.s @r0,fr1 */
L8c0378b8: *stop=0x8c0378b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[1],&f->fpscr,&fr[0]);if(status)return status;}goto L8c0378ba; /* fmul fr1,fr0 */
L8c0378ba: *stop=0x8c0378bau;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c0378bc; /* ftrc fr0,FPUL */
L8c0378bc: *stop=0x8c0378bcu;r[0]=*fpul;goto L8c0378be; /* sts FPUL,r0 */
L8c0378be: *stop=0x8c0378beu;if(!valid_access(r[4]+8u,4u))return -3;r[9]=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;goto L8c0378c0; /* mov.l @(0x8,r4),r9 */
L8c0378c0: *stop=0x8c0378c0u;r[1]+=15u;goto L8c0378c2; /* add #0xf,r1 */
L8c0378c2: *stop=0x8c0378c2u;if(!valid_access(r[1],1u))return -3;r[8]=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;goto L8c0378c4; /* mov.b @r1,r8 */
L8c0378c4: *stop=0x8c0378c4u;*sr=(*sr&~1u)|(r[8]==r[0]);goto L8c0378c6; /* cmp/eq r8,r0 */
L8c0378c6: if((*sr&1u))goto L8c0378de;goto L8c0378c8; /* bt 0x8c0378de */
L8c0378c8: *stop=0x8c0378c8u;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[0],1);if(io_status)return io_status;goto L8c0378ca; /* mov.b r0,@r1 */
L8c0378ca: *stop=0x8c0378cau;r[1]+=16u;goto L8c0378cc; /* add #0x10,r1 */
L8c0378cc: *stop=0x8c0378ccu;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[0],1);if(io_status)return io_status;goto L8c0378ce; /* mov.b r0,@r1 */
L8c0378ce: *stop=0x8c0378ceu;r[1]+=16u;goto L8c0378d0; /* add #0x10,r1 */
L8c0378d0: *stop=0x8c0378d0u;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[0],1);if(io_status)return io_status;goto L8c0378d2; /* mov.b r0,@r1 */
L8c0378d2: *stop=0x8c0378d2u;r[1]+=16u;goto L8c0378d4; /* add #0x10,r1 */
L8c0378d4: *stop=0x8c0378d4u;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[0],1);if(io_status)return io_status;goto L8c0378d6; /* mov.b r0,@r1 */
L8c0378d6: *stop=0x8c0378d6u;r[1]+=16u;goto L8c0378d8; /* add #0x10,r1 */
L8c0378d8: *stop=0x8c0378d8u;r[9]+=4294967292u;goto L8c0378da; /* add #-0x4,r9 */
L8c0378da: *stop=0x8c0378dau;*sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c0378dc; /* cmp/pl r9 */
L8c0378dc: if((*sr&1u))goto L8c0378c8;goto L8c0378de; /* bt 0x8c0378c8 */
L8c0378de: *stop=0x8c0378deu;if(*pr!=entry_pr)return -3;*stop=0x8c0378e0u;return 0; /* rts */
L8c0378f4: *stop=0x8c0378f4u;if(!valid_access(r[5]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x3u,1);if(io_status)return io_status;goto L8c0378f6; /* mov.b @(0x3,r5),r0 */
L8c0378f6: *stop=0x8c0378f6u;r[0]=r[0]&255u;goto L8c0378f8; /* extu.b r0,r0 */
L8c0378f8: *stop=0x8c0378f8u;*fpul=r[0];goto L8c0378fa; /* lds r0,FPUL */
L8c0378fa: *stop=0x8c0378fau;status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][0]);if(status)return status;goto L8c0378fc; /* float FPUL,fr0 */
L8c0378fc: *stop=0x8c0378fcu;r[0]=rd(m,bus,&io_status,0x8c037964u,4);if(io_status)return io_status;goto L8c0378fe; /* mov.l 0x8c037964,r0 */
L8c0378fe: *stop=0x8c0378feu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;goto L8c037900; /* fmov.s @r0,fr1 */
L8c037900: *stop=0x8c037900u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[1],&f->fpscr,&fr[0]);if(status)return status;}goto L8c037902; /* fmul fr1,fr0 */
L8c037902: *stop=0x8c037902u;if(!valid_ram(*gbr+0x28u,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x28u,4);if(io_status)return io_status;goto L8c037904; /* mov.l @(0x28,GBR),r0 */
L8c037904: *stop=0x8c037904u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c037906; /* tst r0,r0 */
L8c037906: if((*sr&1u))goto L8c03792a;goto L8c037908; /* bt 0x8c03792a */
L8c037908: *stop=0x8c037908u;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c03790a; /* cmp/pl r0 */
L8c03790a: if(!(*sr&1u))goto L8c037910;goto L8c03790c; /* bf 0x8c037910 */
L8c03790c: *stop=0x8c03790cu;r[8]=rd(m,bus,&io_status,0x8c037968u,4);if(io_status)return io_status;goto L8c03790e; /* mov.l 0x8c037968,r8 */
L8c03790e: *stop=0x8c03790eu;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[8]);if(io_status)return io_status;goto L8c037910; /* fmov.s @r8,fr0 */
L8c037910: *stop=0x8c037910u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c037912; /* cmp/pz r0 */
L8c037912: if((*sr&1u))goto L8c03792a;goto L8c037914; /* bt 0x8c03792a */
L8c037914: *stop=0x8c037914u;r[8]=rd(m,bus,&io_status,0x8c037968u,4);if(io_status)return io_status;goto L8c037916; /* mov.l 0x8c037968,r8 */
L8c037916: *stop=0x8c037916u;if(!valid_access(r[8],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c037918; /* fmov.s @r8,fr1 */
L8c037918: *stop=0x8c037918u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c03791a; /* fadd fr1,fr0 */
L8c03791a: *stop=0x8c03791au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0x3f800000u;goto L8c03791c; /* fldi1 fr1 */
L8c03791c: *stop=0x8c03791cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c03791e; /* fcmp/gt fr1,fr0 */
L8c03791e: if(!(*sr&1u))goto L8c037922;goto L8c037920; /* bf 0x8c037922 */
L8c037920: *stop=0x8c037920u;fcopy(f,0u,1u);goto L8c037922; /* fmov fr1,fr0 */
L8c037922: *stop=0x8c037922u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0;goto L8c037924; /* fldi0 fr1 */
L8c037924: *stop=0x8c037924u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c037926; /* fcmp/gt fr1,fr0 */
L8c037926: if((*sr&1u))goto L8c03792a;goto L8c037928; /* bt 0x8c03792a */
L8c037928: *stop=0x8c037928u;fcopy(f,0u,1u);goto L8c03792a; /* fmov fr1,fr0 */
L8c03792a: *stop=0x8c03792au;if(!valid_ram(*gbr+0x3cu,4u))return -3;r[0]=rd(m,bus,&io_status,*gbr+0x3cu,4);if(io_status)return io_status;goto L8c03792c; /* mov.l @(0x3c,GBR),r0 */
L8c03792c: *stop=0x8c03792cu;r[1]=r[0];goto L8c03792e; /* mov r0,r1 */
L8c03792e: *stop=0x8c03792eu;r[0]=rd(m,bus,&io_status,0x8c03796cu,4);if(io_status)return io_status;goto L8c037930; /* mov.l 0x8c03796c,r0 */
L8c037930: *stop=0x8c037930u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;goto L8c037932; /* fmov.s @r0,fr1 */
L8c037932: *stop=0x8c037932u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[1],&f->fpscr,&fr[0]);if(status)return status;}goto L8c037934; /* fmul fr1,fr0 */
L8c037934: *stop=0x8c037934u;status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,fpul);if(status)return status;goto L8c037936; /* ftrc fr0,FPUL */
L8c037936: *stop=0x8c037936u;r[0]=*fpul;goto L8c037938; /* sts FPUL,r0 */
L8c037938: *stop=0x8c037938u;r[0]<<=16;goto L8c03793a; /* shll16 r0 */
L8c03793a: *stop=0x8c03793au;r[0]<<=8;goto L8c03793c; /* shll8 r0 */
L8c03793c: *stop=0x8c03793cu;r[8]=rd(m,bus,&io_status,0x8c037970u,4);if(io_status)return io_status;goto L8c03793e; /* mov.l 0x8c037970,r8 */
L8c03793e: *stop=0x8c03793eu;r[0]|=r[8];goto L8c037940; /* or r8,r0 */
L8c037940: *stop=0x8c037940u;if(!valid_access(r[4]+8u,4u))return -3;r[9]=rd(m,bus,&io_status,r[4]+0x8u,4);if(io_status)return io_status;goto L8c037942; /* mov.l @(0x8,r4),r9 */
L8c037942: *stop=0x8c037942u;r[1]+=12u;goto L8c037944; /* add #0xc,r1 */
L8c037944: *stop=0x8c037944u;if(!valid_access(r[1],4u))return -3;r[8]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c037946; /* mov.l @r1,r8 */
L8c037946: *stop=0x8c037946u;*sr=(*sr&~1u)|(r[8]==r[0]);goto L8c037948; /* cmp/eq r8,r0 */
L8c037948: if((*sr&1u))goto L8c037960;goto L8c03794a; /* bt 0x8c037960 */
L8c03794a: *stop=0x8c03794au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c03794c; /* mov.l r0,@r1 */
L8c03794c: *stop=0x8c03794cu;r[1]+=16u;goto L8c03794e; /* add #0x10,r1 */
L8c03794e: *stop=0x8c03794eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037950; /* mov.l r0,@r1 */
L8c037950: *stop=0x8c037950u;r[1]+=16u;goto L8c037952; /* add #0x10,r1 */
L8c037952: *stop=0x8c037952u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037954; /* mov.l r0,@r1 */
L8c037954: *stop=0x8c037954u;r[1]+=16u;goto L8c037956; /* add #0x10,r1 */
L8c037956: *stop=0x8c037956u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c037958; /* mov.l r0,@r1 */
L8c037958: *stop=0x8c037958u;r[1]+=16u;goto L8c03795a; /* add #0x10,r1 */
L8c03795a: *stop=0x8c03795au;r[9]+=4294967292u;goto L8c03795c; /* add #-0x4,r9 */
L8c03795c: *stop=0x8c03795cu;*sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c03795e; /* cmp/pl r9 */
L8c03795e: if((*sr&1u))goto L8c03794a;goto L8c037960; /* bt 0x8c03794a */
L8c037960: *stop=0x8c037960u;if(*pr!=entry_pr)return -3;*stop=0x8c037962u;return 0; /* rts */
}
