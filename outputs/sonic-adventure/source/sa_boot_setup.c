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
SA_STAGE_EXPORT int sa_boot_setup_8c107aaa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c107aaa;
L8c107aaa: *stop=0x8c107aaau;r[3]=rd(m,bus,&io_status,0x8c107b80u,4);if(io_status)return io_status;goto L8c107aac; /* mov.l 0x8c107b80,r3 */
L8c107aac: *stop=0x8c107aacu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c107aae; /* sts.l PR,@-r15 */
L8c107aae: *stop=0x8c107aaeu;if(!valid_access(r[3],4u))return -3;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c107ab0; /* mov.l @r3,r4 */
L8c107ab0: *stop=0x8c107ab0u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c107ab2; /* tst r4,r4 */
L8c107ab2: if((*sr&1u))goto L8c107ac0;goto L8c107ab4; /* bt 0x8c107ac0 */
L8c107ab4: *stop=0x8c107ab4u;r[1]=rd(m,bus,&io_status,0x8c107b84u,4);if(io_status)return io_status;goto L8c107ab6; /* mov.l 0x8c107b84,r1 */
L8c107ab6: target=r[1];*pr=0x8c107abau;*stop=0x8c107ab8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c107aba; /* jsr @r1 */
L8c107aba: *stop=0x8c107abau;r[2]=rd(m,bus,&io_status,0x8c107b80u,4);if(io_status)return io_status;goto L8c107abc; /* mov.l 0x8c107b80,r2 */
L8c107abc: *stop=0x8c107abcu;r[3]=0u;goto L8c107abe; /* mov #0x0,r3 */
L8c107abe: *stop=0x8c107abeu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c107ac0; /* mov.l r3,@r2 */
L8c107ac0: *stop=0x8c107ac0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c107ac2; /* lds.l @r15+,PR */
L8c107ac2: *stop=0x8c107ac2u;if(*pr!=entry_pr)return -3;*stop=0x8c107ac4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_setup_8c605c10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c605c10;
L8c605c10: *stop=0x8c605c10u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c605c12; /* sts.l PR,@-r15 */
L8c605c12: *stop=0x8c605c12u;r[15]+=4294967292u;goto L8c605c14; /* add #-0x4,r15 */
L8c605c14: *stop=0x8c605c14u;r[2]=r[15];goto L8c605c16; /* mov r15,r2 */
L8c605c16: *stop=0x8c605c16u;r[1]=rd(m,bus,&io_status,0x8c605c50u,4);if(io_status)return io_status;goto L8c605c18; /* mov.l 0x8c605c50,r1 */
L8c605c18: *stop=0x8c605c18u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c605c1a; /* mov.l r4,@r15 */
L8c605c1a: *stop=0x8c605c1au;if(!valid_access(r[2],4u))return -3;r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c605c1c; /* mov.l @r2,r3 */
L8c605c1c: target=r[1];*pr=0x8c605c20u;*stop=0x8c605c1eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c605c20; /* jsr @r1 */
L8c605c20: *stop=0x8c605c20u;r[15]+=8u;goto L8c605c22; /* add #0x8,r15 */
L8c605c22: *stop=0x8c605c22u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c605c24; /* lds.l @r15+,PR */
L8c605c24: *stop=0x8c605c24u;if(*pr!=entry_pr)return -3;*stop=0x8c605c26u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_setup_8c652f56(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus){
 int io_status=0;(void)io_status;(void)bus;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c652f56;
L8c652f56: *stop=0x8c652f56u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c652f58; /* sts.l PR,@-r15 */
L8c652f58: *stop=0x8c652f58u;r[3]=rd(m,bus,&io_status,0x8c653004u,4);if(io_status)return io_status;goto L8c652f5a; /* mov.l 0x8c653004,r3 */
L8c652f5a: *stop=0x8c652f5au;if(!valid_access(r[15]+4u,4u))return -3;r[5]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c652f5c; /* mov.l @(0x4,r15),r5 */
L8c652f5c: target=r[3];*pr=0x8c652f60u;*stop=0x8c652f5eu;r[4]=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c652f60; /* jsr @r3 */
L8c652f60: *stop=0x8c652f60u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c652f62; /* lds.l @r15+,PR */
L8c652f62: *stop=0x8c652f62u;if(*pr!=entry_pr)return -3;*stop=0x8c652f64u;r[0]=0u;return 0; /* rts */
}
