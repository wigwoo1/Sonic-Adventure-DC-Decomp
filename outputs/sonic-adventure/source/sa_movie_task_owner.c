/* Original owner and pool teardown; bookkeeping behavior remains explicit.
 * Static C from byte-checked SH-4 instructions; complete explicit state.
 * Missing callees remain reached boundaries; no callback result is substituted.
 * Context and interrupt entry remain explicit platform state.
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
SA_STAGE_EXPORT int sa_movie_task_owner_8c668cec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c668cec;
L8c668cec: *stop=0x8c668cecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c668cee; /* mov.l r14,@-r15 */
L8c668cee: *stop=0x8c668ceeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c668cf0; /* mov.l r13,@-r15 */
L8c668cf0: *stop=0x8c668cf0u;r[13]=0u;goto L8c668cf2; /* mov #0x0,r13 */
L8c668cf2: *stop=0x8c668cf2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c668cf4; /* mov.l r12,@-r15 */
L8c668cf4: *stop=0x8c668cf4u;r[12]=8u;goto L8c668cf6; /* mov #0x8,r12 */
L8c668cf6: *stop=0x8c668cf6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c668cf8; /* sts.l PR,@-r15 */
L8c668cf8: *stop=0x8c668cf8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668d84u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c668cfa; /* mov.l 0x8c668d84,r14 */
L8c668cfa: *stop=0x8c668cfau;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668cfc; /* mov.l @r14,r3 */
L8c668cfc: *stop=0x8c668cfcu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c668cfe; /* tst r3,r3 */
L8c668cfe: if((*sr&1u))goto L8c668d04;goto L8c668d00; /* bt 0x8c668d04 */
L8c668d00: target=0x8c668d18u;*pr=0x8c668d04u;*stop=0x8c668d02u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668d04; /* bsr 0x8c668d18 */
L8c668d04: *stop=0x8c668d04u;r[2]=sx(m,bus,&io_status,0x8c668d72u,2);if(io_status)return io_status;goto L8c668d06; /* mov.w 0x8c668d72,r2 */
L8c668d06: *stop=0x8c668d06u;r[13]+=1u;goto L8c668d08; /* add #0x1,r13 */
L8c668d08: *stop=0x8c668d08u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[12]);goto L8c668d0a; /* cmp/ge r12,r13 */
L8c668d0a: take=!(*sr&1u);*stop=0x8c668d0cu;r[14]+=r[2];if(take)goto L8c668cfa;goto L8c668d0e; /* bf/s 0x8c668cfa */
L8c668d0e: *stop=0x8c668d0eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c668d10; /* lds.l @r15+,PR */
L8c668d10: *stop=0x8c668d10u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c668d12; /* mov.l @r15+,r12 */
L8c668d12: *stop=0x8c668d12u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c668d14; /* mov.l @r15+,r13 */
L8c668d14: *stop=0x8c668d14u;if(*pr!=entry_pr)return -3;*stop=0x8c668d16u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_task_owner_8c668d18(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c668d18;
L8c668d18: *stop=0x8c668d18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c668d1a; /* mov.l r14,@-r15 */
L8c668d1a: *stop=0x8c668d1au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c668d1c; /* mov.l r13,@-r15 */
L8c668d1c: *stop=0x8c668d1cu;r[13]=r[4];goto L8c668d1e; /* mov r4,r13 */
L8c668d1e: *stop=0x8c668d1eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c668d20; /* mov.l r12,@-r15 */
L8c668d20: *stop=0x8c668d20u;r[12]=0u;goto L8c668d22; /* mov #0x0,r12 */
L8c668d22: *stop=0x8c668d22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c668d24; /* sts.l PR,@-r15 */
L8c668d24: *stop=0x8c668d24u;r[14]=sx(m,bus,&io_status,0x8c668d74u,2);if(io_status)return io_status;goto L8c668d26; /* mov.w 0x8c668d74,r14 */
L8c668d26: *stop=0x8c668d28u;r[14]+=r[13];goto L8c668d3c; /* bra 0x8c668d3c */
L8c668d2a: *stop=0x8c668d2au;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c668d2c; /* mov.l @(0xc,r14),r2 */
L8c668d2c: *stop=0x8c668d2cu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c668d2e; /* tst r2,r2 */
L8c668d2e: if((*sr&1u))goto L8c668d36;goto L8c668d30; /* bt 0x8c668d36 */
L8c668d30: *stop=0x8c668d30u;r[5]=r[14];goto L8c668d32; /* mov r14,r5 */
L8c668d32: target=0x8c668ccau;*pr=0x8c668d36u;*stop=0x8c668d34u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c668d36; /* bsr 0x8c668cca */
L8c668d36: *stop=0x8c668d36u;r[2]=sx(m,bus,&io_status,0x8c668d66u,2);if(io_status)return io_status;goto L8c668d38; /* mov.w 0x8c668d66,r2 */
L8c668d38: *stop=0x8c668d38u;r[12]+=1u;goto L8c668d3a; /* add #0x1,r12 */
L8c668d3a: *stop=0x8c668d3au;r[14]+=r[2];goto L8c668d3c; /* add r2,r14 */
L8c668d3c: *stop=0x8c668d3cu;r[0]=sx(m,bus,&io_status,0x8c668d76u,2);if(io_status)return io_status;goto L8c668d3e; /* mov.w 0x8c668d76,r0 */
L8c668d3e: *stop=0x8c668d3eu;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c668d40; /* mov.l @(r0,r13),r3 */
L8c668d40: *stop=0x8c668d40u;*sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[3]);goto L8c668d42; /* cmp/ge r3,r12 */
L8c668d42: if(!(*sr&1u))goto L8c668d2a;goto L8c668d44; /* bf 0x8c668d2a */
L8c668d44: *stop=0x8c668d44u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c668d46; /* lds.l @r15+,PR */
L8c668d46: *stop=0x8c668d46u;r[4]=r[13];goto L8c668d48; /* mov r13,r4 */
L8c668d48: *stop=0x8c668d48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c668d88u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c668d4a; /* mov.l 0x8c668d88,r3 */
L8c668d4a: *stop=0x8c668d4au;r[6]=sx(m,bus,&io_status,0x8c668d72u,2);if(io_status)return io_status;goto L8c668d4c; /* mov.w 0x8c668d72,r6 */
L8c668d4c: *stop=0x8c668d4cu;r[5]=0u;goto L8c668d4e; /* mov #0x0,r5 */
L8c668d4e: *stop=0x8c668d4eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c668d50; /* mov.l @r15+,r12 */
L8c668d50: *stop=0x8c668d50u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c668d52; /* mov.l @r15+,r13 */
L8c668d52: target=r[3];*stop=0x8c668d54u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}
