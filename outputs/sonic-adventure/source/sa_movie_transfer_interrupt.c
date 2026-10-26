/* Original installed transfer interrupt handler; explicit machine/platform state.
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
 if(n==4u && ((a&0x1fffffffu)==0x00710000u || (a&0x1fffffffu)==0x00710004u))return 1;
 if((n==1u||n==2u||n==4u) && (a&0x1fffffffu)>=0x00702c00u && (a&0x1fffffffu)+n<=0x00702c04u)return 1;
 if(n==4u && (a==0xff00001cu||a==0xffa00040u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f6800u && (a&0x1fffffffu)<0x005f7d00u)return 1;
 if(n==4u && !(a&31u) && a>=0xf4000000u && a<0xf4004000u)return 1;
 if(n==2u && (a==0xffd00000u||a==0xffd00004u||a==0xffd00008u||a==0xffd0000cu||a==0xffd80010u))return 1;
 if(n==1u && (a==0xffd80000u||a==0xffd80004u))return 1;
 if(n==4u && (a==0xffd80008u||a==0xffd8000cu))return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_transfer_interrupt_8c10ebe8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ebe8;
L8c10ebe8: *stop=0x8c10ebe8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10ebea; /* mov.l r14,@-r15 */
L8c10ebea: *stop=0x8c10ebeau;r[14]=r[4];goto L8c10ebec; /* mov r4,r14 */
L8c10ebec: *stop=0x8c10ebecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ec38u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c10ebee; /* mov.l 0x8c10ec38,r4 */
L8c10ebee: *stop=0x8c10ebeeu;r[6]=0u;goto L8c10ebf0; /* mov #0x0,r6 */
L8c10ebf0: *stop=0x8c10ebf0u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ebf2; /* mov.l @r14,r3 */
L8c10ebf2: *stop=0x8c10ebf2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[5]=loaded;}goto L8c10ebf4; /* mov.l @r4,r5 */
L8c10ebf4: *stop=0x8c10ebf4u;r[3]<<=2;goto L8c10ebf6; /* shll2 r3 */
L8c10ebf6: *stop=0x8c10ebf6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10ec36u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c10ebf8; /* mov.w 0x8c10ec36,r2 */
L8c10ebf8: *stop=0x8c10ebf8u;r[3]<<=2;goto L8c10ebfa; /* shll2 r3 */
L8c10ebfa: *stop=0x8c10ebfau;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[5]=loaded;}goto L8c10ebfc; /* mov.l @r5,r5 */
L8c10ebfc: *stop=0x8c10ebfcu;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c10ebfe; /* shll r3 */
L8c10ebfe: *stop=0x8c10ebfeu;r[5]+=r[3];goto L8c10ec00; /* add r3,r5 */
L8c10ec00: *stop=0x8c10ec00u;if(!valid_access(r[5]+20u,4u))return -3;wr(m,bus,&io_status,r[5]+0x14u,r[6],4);if(io_status)return io_status;goto L8c10ec02; /* mov.l r6,@(0x14,r5) */
L8c10ec02: *stop=0x8c10ec02u;r[5]=1u;goto L8c10ec04; /* mov #0x1,r5 */
L8c10ec04: *stop=0x8c10ec04u;if(!valid_access(r[14]+44u,4u))return -3;wr(m,bus,&io_status,r[14]+0x2cu,r[6],4);if(io_status)return io_status;goto L8c10ec06; /* mov.l r6,@(0x2c,r14) */
L8c10ec06: *stop=0x8c10ec06u;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[5],4);if(io_status)return io_status;goto L8c10ec08; /* mov.l r5,@(0x30,r14) */
L8c10ec08: *stop=0x8c10ec08u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ec0a; /* mov.l @r14,r3 */
L8c10ec0a: *stop=0x8c10ec0au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10ec0c; /* mov.l @r4,r0 */
L8c10ec0c: *stop=0x8c10ec0cu;r[3]<<=2;goto L8c10ec0e; /* shll2 r3 */
L8c10ec0e: *stop=0x8c10ec0eu;r[2]+=r[0];goto L8c10ec10; /* add r0,r2 */
L8c10ec10: *stop=0x8c10ec10u;r[3]+=r[2];goto L8c10ec12; /* add r2,r3 */
L8c10ec12: *stop=0x8c10ec12u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10ec3cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10ec14; /* mov.l 0x8c10ec3c,r2 */
L8c10ec14: *stop=0x8c10ec14u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10ec16; /* mov.l @r3,r1 */
L8c10ec16: *stop=0x8c10ec16u;r[1]+=1u;goto L8c10ec18; /* add #0x1,r1 */
L8c10ec18: *stop=0x8c10ec18u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c10ec1a; /* mov.l r1,@r3 */
L8c10ec1a: *stop=0x8c10ec1au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10ec1c; /* mov.l @r14,r1 */
L8c10ec1c: *stop=0x8c10ec1cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ec1e; /* mov.l @r2,r3 */
L8c10ec1e: *stop=0x8c10ec1eu;r[1]+=15u;goto L8c10ec20; /* add #0xf,r1 */
L8c10ec20: *stop=0x8c10ec20u;{uint32_t shift=r[1];if(!(shift&0x80000000u))r[5]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[5]=n?(uint32_t)((int32_t)r[5]>>n):((r[5]&0x80000000u)?0xffffffffu:0u);}}goto L8c10ec22; /* shad r1,r5 */
L8c10ec22: *stop=0x8c10ec22u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c10ec24; /* mov.l r5,@r3 */
L8c10ec24: *stop=0x8c10ec24u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ec26; /* mov.l @(0x1c,r14),r3 */
L8c10ec26: *stop=0x8c10ec26u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c10ec28; /* tst r3,r3 */
L8c10ec28: if((*sr&1u))goto L8c10ec32;goto L8c10ec2a; /* bt 0x8c10ec32 */
L8c10ec2a: *stop=0x8c10ec2au;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c10ec2c; /* mov.l @(0x20,r14),r4 */
L8c10ec2c: *stop=0x8c10ec2cu;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10ec2e; /* mov.l @(0x1c,r14),r3 */
L8c10ec2e: target=r[3];*stop=0x8c10ec30u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c10ec32: *stop=0x8c10ec32u;if(*pr!=entry_pr)return -3;*stop=0x8c10ec34u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
