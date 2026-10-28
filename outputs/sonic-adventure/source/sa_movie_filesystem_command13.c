/* Original command13 typeCC normal and readback-verified writes.
 * Explicit register state, original callbacks and nonlocal continuations.
 * No supplied parser results or saved task frames.
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
int sa_movie_channel_io_return_allowed(void *,uint32_t);
static int sa_movie_filesystem_command13_run_8c64b91c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64b93cu)goto L8c64b93c;if(resume_pc==0x8c64b996u)goto L8c64b996;if(resume_pc==0x8c64b9a6u)goto L8c64b9a6;if(resume_pc==0x8c64b9e8u)goto L8c64b9e8;if(resume_pc==0x8c64ba00u)goto L8c64ba00;goto L8c64b91c;
L8c64b91c: *stop=0x8c64b91cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64b91e; /* mov.l r14,@-r15 */
L8c64b91e: *stop=0x8c64b91eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64b920; /* mov.l r13,@-r15 */
L8c64b920: *stop=0x8c64b920u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64b922; /* mov.l r12,@-r15 */
L8c64b922: *stop=0x8c64b922u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64b924; /* mov.l r11,@-r15 */
L8c64b924: *stop=0x8c64b924u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64b926; /* mov.l r10,@-r15 */
L8c64b926: *stop=0x8c64b926u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64b928; /* sts.l PR,@-r15 */
L8c64b928: *stop=0x8c64b928u;r[15]+=4294967284u;goto L8c64b92a; /* add #-0xc,r15 */
L8c64b92a: *stop=0x8c64b92au;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[5],4);if(io_status)return io_status;goto L8c64b92c; /* mov.l r5,@(0x8,r15) */
L8c64b92c: *stop=0x8c64b92cu;r[5]=1u;goto L8c64b92e; /* mov #0x1,r5 */
L8c64b92e: *stop=0x8c64b92eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64b970u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b930; /* mov.l 0x8c64b970,r3 */
L8c64b930: *stop=0x8c64b930u;r[11]=r[4];goto L8c64b932; /* mov r4,r11 */
L8c64b932: *stop=0x8c64b932u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c64b934; /* mov.l r6,@(0x4,r15) */
L8c64b934: *stop=0x8c64b934u;r[13]=r[7];goto L8c64b936; /* mov r7,r13 */
L8c64b936: *stop=0x8c64b936u;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64b938; /* mov.l @(0x28,r15),r12 */
L8c64b938: target=r[3];*pr=0x8c64b93cu;*stop=0x8c64b93au;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b93c; /* jsr @r3 */
L8c64b93c: *stop=0x8c64b93cu;r[4]=r[0];goto L8c64b93e; /* mov r0,r4 */
L8c64b93e: *stop=0x8c64b93eu;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c64b940; /* cmp/pz r4 */
L8c64b940: if((*sr&1u))goto L8c64b974;goto L8c64b942; /* bt 0x8c64b974 */
L8c64b942: *stop=0x8c64b944u;r[0]=r[4];goto L8c64ba02; /* bra 0x8c64ba02 */
L8c64b974: *stop=0x8c64b974u;*sr=(*sr&~1u)|(r[4]>=r[13]);goto L8c64b976; /* cmp/hs r13,r4 */
L8c64b976: if((*sr&1u))goto L8c64b97e;goto L8c64b978; /* bt 0x8c64b97e */
L8c64b978: *stop=0x8c64b978u;r[0]=sx(m,bus,&io_status,0x8c64ba12u,2);if(io_status)return io_status;goto L8c64b97a; /* mov.w 0x8c64ba12,r0 */
L8c64b97a: *stop=0x8c64b97cu;goto L8c64ba02; /* bra 0x8c64ba02 */
L8c64b97e: *stop=0x8c64b97eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ba18u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64b980; /* mov.l 0x8c64ba18,r4 */
L8c64b980: *stop=0x8c64b980u;r[4]&=r[12];goto L8c64b982; /* and r12,r4 */
L8c64b982: *stop=0x8c64b982u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64b984; /* tst r4,r4 */
L8c64b984: if((*sr&1u))goto L8c64b99a;goto L8c64b986; /* bt 0x8c64b99a */
L8c64b986: *stop=0x8c64b986u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64b988; /* mov.l r12,@-r15 */
L8c64b988: *stop=0x8c64b988u;r[7]=r[13];goto L8c64b98a; /* mov r13,r7 */
L8c64b98a: *stop=0x8c64b98au;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b98c; /* mov.l @(0x28,r15),r3 */
L8c64b98c: *stop=0x8c64b98cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c64b98e; /* mov.l r3,@-r15 */
L8c64b98e: *stop=0x8c64b98eu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b990; /* mov.l @(0x10,r15),r5 */
L8c64b990: *stop=0x8c64b990u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64b992; /* mov.l @(0xc,r15),r6 */
L8c64b992: target=0x8c64ba30u;*pr=0x8c64b996u;*stop=0x8c64b994u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b996; /* bsr 0x8c64ba30 */
L8c64b996: *stop=0x8c64b998u;r[15]+=8u;goto L8c64ba02; /* bra 0x8c64ba02 */
L8c64b99a: *stop=0x8c64b99au;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64b99c; /* mov.l @(0x8,r15),r5 */
L8c64b99c: *stop=0x8c64b99cu;r[7]=r[15];goto L8c64b99e; /* mov r15,r7 */
L8c64b99e: *stop=0x8c64b99eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ba1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b9a0; /* mov.l 0x8c64ba1c,r3 */
L8c64b9a0: *stop=0x8c64b9a0u;if(!valid_access(r[15]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x24u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64b9a2; /* mov.l @(0x24,r15),r6 */
L8c64b9a2: target=r[3];*pr=0x8c64b9a6u;*stop=0x8c64b9a4u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b9a6; /* jsr @r3 */
L8c64b9a6: *stop=0x8c64b9a6u;r[14]=r[0];goto L8c64b9a8; /* mov r0,r14 */
L8c64b9a8: *stop=0x8c64b9a8u;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b9aa; /* cmp/pz r14 */
L8c64b9aa: if(!(*sr&1u))goto L8c64ba00;goto L8c64b9ac; /* bf 0x8c64ba00 */
L8c64b9ac: *stop=0x8c64b9acu;r[4]=sx(m,bus,&io_status,0x8c64ba14u,2);if(io_status)return io_status;goto L8c64b9ae; /* mov.w 0x8c64ba14,r4 */
L8c64b9ae: *stop=0x8c64b9aeu;r[0]=r[12];goto L8c64b9b0; /* mov r12,r0 */
L8c64b9b0: *stop=0x8c64b9b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ba20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b9b2; /* mov.l 0x8c64ba20,r3 */
L8c64b9b2: *stop=0x8c64b9b2u;*macl=r[4]*r[11];goto L8c64b9b4; /* mul.l r4,r11 */
L8c64b9b4: *stop=0x8c64b9b4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64b9b6; /* mov.l @r15,r2 */
L8c64b9b6: *stop=0x8c64b9b6u;r[11]=0u;goto L8c64b9b8; /* mov #0x0,r11 */
L8c64b9b8: *stop=0x8c64b9b8u;if(!valid_access(r[2]+1u,1u))return -3;wr(m,bus,&io_status,r[2]+0x1u,r[0],1);if(io_status)return io_status;goto L8c64b9ba; /* mov.b r0,@(0x1,r2) */
L8c64b9ba: *stop=0x8c64b9bau;r[2]=1u;goto L8c64b9bc; /* mov #0x1,r2 */
L8c64b9bc: *stop=0x8c64b9bcu;r[4]=*macl;goto L8c64b9be; /* sts MACL,r4 */
L8c64b9be: *stop=0x8c64b9beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ba24u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64b9c0; /* mov.l 0x8c64ba24,r0 */
L8c64b9c0: *stop=0x8c64b9c0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c64b9c2; /* mov.l @(0x4,r15),r12 */
L8c64b9c2: *stop=0x8c64b9c2u;r[4]+=r[3];goto L8c64b9c4; /* add r3,r4 */
L8c64b9c4: *stop=0x8c64b9c4u;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[2],4);if(io_status)return io_status;goto L8c64b9c6; /* mov.l r2,@(0x30,r4) */
L8c64b9c6: *stop=0x8c64b9c6u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64b9c8; /* mov.l @(0x4,r4),r1 */
L8c64b9c8: *stop=0x8c64b9c8u;r[2]=r[1];goto L8c64b9ca; /* mov r1,r2 */
L8c64b9ca: *stop=0x8c64b9cau;r[1]<<=2;goto L8c64b9cc; /* shll2 r1 */
L8c64b9cc: *stop=0x8c64b9ccu;r[1]+=r[2];goto L8c64b9ce; /* add r2,r1 */
L8c64b9ce: *stop=0x8c64b9ceu;r[2]=r[11];goto L8c64b9d0; /* mov r11,r2 */
L8c64b9d0: *stop=0x8c64b9d0u;r[1]<<=2;goto L8c64b9d2; /* shll2 r1 */
L8c64b9d2: *stop=0x8c64b9d2u;*sr=(*sr&~1u)|(r[2]>=r[13]);goto L8c64b9d4; /* cmp/hs r13,r2 */
L8c64b9d4: *stop=0x8c64b9d4u;r[1]<<=2;goto L8c64b9d6; /* shll2 r1 */
L8c64b9d6: *stop=0x8c64b9d6u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64b9d8; /* mov.l @(r0,r1),r0 */
L8c64b9d8: *stop=0x8c64b9d8u;r[0]+=r[13];goto L8c64b9da; /* add r13,r0 */
L8c64b9da: take=(*sr&1u);*stop=0x8c64b9dcu;if(!valid_access(r[4]+52u,4u))return -3;wr(m,bus,&io_status,r[4]+0x34u,r[0],4);if(io_status)return io_status;if(take)goto L8c64b9f6;goto L8c64b9de; /* bt/s 0x8c64b9f6 */
L8c64b9de: *stop=0x8c64b9deu;r[10]=sx(m,bus,&io_status,0x8c64ba16u,2);if(io_status)return io_status;goto L8c64b9e0; /* mov.w 0x8c64ba16,r10 */
L8c64b9e0: *stop=0x8c64b9e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ba28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b9e2; /* mov.l 0x8c64ba28,r3 */
L8c64b9e2: *stop=0x8c64b9e2u;r[5]=r[12];goto L8c64b9e4; /* mov r12,r5 */
L8c64b9e4: target=r[3];*pr=0x8c64b9e8u;*stop=0x8c64b9e6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64b9e8; /* jsr @r3 */
L8c64b9e8: *stop=0x8c64b9e8u;r[14]=r[0];goto L8c64b9ea; /* mov r0,r14 */
L8c64b9ea: *stop=0x8c64b9eau;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64b9ec; /* cmp/pz r14 */
L8c64b9ec: if(!(*sr&1u))goto L8c64b9f8;goto L8c64b9ee; /* bf 0x8c64b9f8 */
L8c64b9ee: *stop=0x8c64b9eeu;r[11]+=1u;goto L8c64b9f0; /* add #0x1,r11 */
L8c64b9f0: *stop=0x8c64b9f0u;*sr=(*sr&~1u)|(r[11]>=r[13]);goto L8c64b9f2; /* cmp/hs r13,r11 */
L8c64b9f2: take=!(*sr&1u);*stop=0x8c64b9f4u;r[12]+=r[10];if(take)goto L8c64b9e0;goto L8c64b9f6; /* bf/s 0x8c64b9e0 */
L8c64b9f6: *stop=0x8c64b9f6u;r[14]=0u;goto L8c64b9f8; /* mov #0x0,r14 */
L8c64b9f8: *stop=0x8c64b9f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64ba2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64b9fa; /* mov.l 0x8c64ba2c,r3 */
L8c64b9fa: *stop=0x8c64b9fau;r[5]=0u;goto L8c64b9fc; /* mov #0x0,r5 */
L8c64b9fc: target=r[3];*pr=0x8c64ba00u;*stop=0x8c64b9feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ba00; /* jsr @r3 */
L8c64ba00: *stop=0x8c64ba00u;r[0]=r[14];goto L8c64ba02; /* mov r14,r0 */
L8c64ba02: *stop=0x8c64ba02u;r[15]+=12u;goto L8c64ba04; /* add #0xc,r15 */
L8c64ba04: *stop=0x8c64ba04u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64ba06; /* lds.l @r15+,PR */
L8c64ba06: *stop=0x8c64ba06u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64ba08; /* mov.l @r15+,r10 */
L8c64ba08: *stop=0x8c64ba08u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64ba0a; /* mov.l @r15+,r11 */
L8c64ba0a: *stop=0x8c64ba0au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64ba0c; /* mov.l @r15+,r12 */
L8c64ba0c: *stop=0x8c64ba0cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64ba0e; /* mov.l @r15+,r13 */
L8c64ba0e: *stop=0x8c64ba0eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64ba10u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_8c64b91c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64b91c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64b93c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64b91c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b93cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64b996(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64b91c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b996u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64b9a6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64b91c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b9a6u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64b9e8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64b91c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64b9e8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64ba00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64b91c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ba00u);}

static int sa_movie_filesystem_command13_run_8c648e36(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c648e6eu)goto L8c648e6e;if(resume_pc==0x8c648e76u)goto L8c648e76;if(resume_pc==0x8c648e80u)goto L8c648e80;if(resume_pc==0x8c648e9cu)goto L8c648e9c;if(resume_pc==0x8c648eb8u)goto L8c648eb8;if(resume_pc==0x8c648ec8u)goto L8c648ec8;if(resume_pc==0x8c648f06u)goto L8c648f06;if(resume_pc==0x8c648f18u)goto L8c648f18;if(resume_pc==0x8c648f26u)goto L8c648f26;if(resume_pc==0x8c648f3au)goto L8c648f3a;if(resume_pc==0x8c648f40u)goto L8c648f40;if(resume_pc==0x8c648f84u)goto L8c648f84;goto L8c648e36;
L8c648e36: *stop=0x8c648e36u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c648e38; /* mov.l r14,@-r15 */
L8c648e38: *stop=0x8c648e38u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c648e3a; /* mov.l r13,@-r15 */
L8c648e3a: *stop=0x8c648e3au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c648e3c; /* mov.l r12,@-r15 */
L8c648e3c: *stop=0x8c648e3cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c648e3e; /* mov.l r11,@-r15 */
L8c648e3e: *stop=0x8c648e3eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c648e40; /* mov.l r10,@-r15 */
L8c648e40: *stop=0x8c648e40u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c648e42; /* sts.l PR,@-r15 */
L8c648e42: *stop=0x8c648e42u;r[15]+=4294967236u;goto L8c648e44; /* add #-0x3c,r15 */
L8c648e44: *stop=0x8c648e44u;r[3]=sx(m,bus,&io_status,0x8c648ed4u,2);if(io_status)return io_status;goto L8c648e46; /* mov.w 0x8c648ed4,r3 */
L8c648e46: *stop=0x8c648e46u;r[14]=r[4];goto L8c648e48; /* mov r4,r14 */
L8c648e48: *stop=0x8c648e48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648ee8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648e4a; /* mov.l 0x8c648ee8,r2 */
L8c648e4a: *stop=0x8c648e4au;r[11]=0u;goto L8c648e4c; /* mov #0x0,r11 */
L8c648e4c: *stop=0x8c648e4cu;*macl=r[3]*r[14];goto L8c648e4e; /* mul.l r3,r14 */
L8c648e4e: *stop=0x8c648e4eu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c648e50; /* mov.l r6,@(0x8,r15) */
L8c648e50: *stop=0x8c648e50u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[7],4);if(io_status)return io_status;goto L8c648e52; /* mov.l r7,@(0x4,r15) */
L8c648e52: *stop=0x8c648e52u;r[3]=*macl;goto L8c648e54; /* sts MACL,r3 */
L8c648e54: *stop=0x8c648e54u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c648e56; /* mov.l r3,@(0xc,r15) */
L8c648e56: *stop=0x8c648e56u;r[3]+=r[2];goto L8c648e58; /* add r2,r3 */
L8c648e58: *stop=0x8c648e58u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c648e5a; /* mov.l r3,@r15 */
L8c648e5a: *stop=0x8c648e5au;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c648e5c; /* mov.l @(0x4,r15),r1 */
L8c648e5c: *stop=0x8c648e5cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[11],4);if(io_status)return io_status;goto L8c648e5e; /* mov.l r11,@r1 */
L8c648e5e: *stop=0x8c648e5eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c648e60; /* mov.l @r15,r3 */
L8c648e60: *stop=0x8c648e60u;if(!valid_access(r[3]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[3]+0x2u,1);if(io_status)return io_status;goto L8c648e62; /* mov.b @(0x2,r3),r0 */
L8c648e62: *stop=0x8c648e62u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648e64; /* tst r0,r0 */
L8c648e64: take=(*sr&1u);*stop=0x8c648e66u;r[12]=r[5];if(take)goto L8c648e90;goto L8c648e68; /* bt/s 0x8c648e90 */
L8c648e68: *stop=0x8c648e68u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648eecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648e6a; /* mov.l 0x8c648eec,r3 */
L8c648e6a: target=r[3];*pr=0x8c648e6eu;*stop=0x8c648e6cu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648e6e; /* jsr @r3 */
L8c648e6e: *stop=0x8c648e6eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648e70; /* tst r0,r0 */
L8c648e70: if(!(*sr&1u))goto L8c648e7a;goto L8c648e72; /* bf 0x8c648e7a */
L8c648e72: target=0x8c64850au;*pr=0x8c648e76u;*stop=0x8c648e74u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648e76; /* bsr 0x8c64850a */
L8c648e76: *stop=0x8c648e78u;goto L8c648e90; /* bra 0x8c648e90 */
L8c648e7a: *stop=0x8c648e7au;{uint32_t loaded=rd(m,bus,&io_status,0x8c648ef0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648e7c; /* mov.l 0x8c648ef0,r2 */
L8c648e7c: target=r[2];*pr=0x8c648e80u;*stop=0x8c648e7eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648e80; /* jsr @r2 */
L8c648e80: *stop=0x8c648e80u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648e82; /* tst r0,r0 */
L8c648e82: if((*sr&1u))goto L8c648e90;goto L8c648e84; /* bt 0x8c648e90 */
L8c648e84: *stop=0x8c648e84u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648e86; /* mov.l @(0xc,r15),r2 */
L8c648e86: *stop=0x8c648e86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648ee8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648e88; /* mov.l 0x8c648ee8,r3 */
L8c648e88: *stop=0x8c648e88u;r[2]+=r[3];goto L8c648e8a; /* add r3,r2 */
L8c648e8a: *stop=0x8c648e8au;if(!valid_access(r[2]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[2]+0x8u,1);if(io_status)return io_status;goto L8c648e8c; /* mov.b @(0x8,r2),r0 */
L8c648e8c: *stop=0x8c648e8cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648e8e; /* tst r0,r0 */
L8c648e8e: if(!(*sr&1u))goto L8c648e96;goto L8c648e90; /* bf 0x8c648e96 */
L8c648e90: *stop=0x8c648e90u;r[0]=sx(m,bus,&io_status,0x8c648ed6u,2);if(io_status)return io_status;goto L8c648e92; /* mov.w 0x8c648ed6,r0 */
L8c648e92: *stop=0x8c648e94u;goto L8c648f8a; /* bra 0x8c648f8a */
L8c648e96: *stop=0x8c648e96u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648ef4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648e98; /* mov.l 0x8c648ef4,r3 */
L8c648e98: target=r[3];*pr=0x8c648e9cu;*stop=0x8c648e9au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648e9c; /* jsr @r3 */
L8c648e9c: *stop=0x8c648e9cu;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648e9e; /* cmp/pz r0 */
L8c648e9e: if((*sr&1u))goto L8c648ea6;goto L8c648ea0; /* bt 0x8c648ea6 */
L8c648ea0: *stop=0x8c648ea0u;r[0]=sx(m,bus,&io_status,0x8c648ed8u,2);if(io_status)return io_status;goto L8c648ea2; /* mov.w 0x8c648ed8,r0 */
L8c648ea2: *stop=0x8c648ea4u;goto L8c648f8a; /* bra 0x8c648f8a */
L8c648ea6: *stop=0x8c648ea6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c648ea8; /* mov.l @r15,r3 */
L8c648ea8: *stop=0x8c648ea8u;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648eaa; /* mov.l @(0x1c,r3),r2 */
L8c648eaa: *stop=0x8c648eaau;if(!valid_access(r[2]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x38u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c648eac; /* mov.l @(0x38,r2),r1 */
L8c648eac: *stop=0x8c648eacu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c648eae; /* tst r1,r1 */
L8c648eae: if((*sr&1u))goto L8c648f2c;goto L8c648eb0; /* bt 0x8c648f2c */
L8c648eb0: *stop=0x8c648eb0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c648ef8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648eb2; /* mov.l 0x8c648ef8,r3 */
L8c648eb2: *stop=0x8c648eb2u;r[5]=r[12];goto L8c648eb4; /* mov r12,r5 */
L8c648eb4: target=r[3];*pr=0x8c648eb8u;*stop=0x8c648eb6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648eb8; /* jsr @r3 */
L8c648eb8: *stop=0x8c648eb8u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648eba; /* cmp/pz r0 */
L8c648eba: if(!(*sr&1u))goto L8c648f2c;goto L8c648ebc; /* bf 0x8c648f2c */
L8c648ebc: *stop=0x8c648ebcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c648edcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648ebe; /* mov.l 0x8c648edc,r3 */
L8c648ebe: *stop=0x8c648ebeu;r[6]=r[15];goto L8c648ec0; /* mov r15,r6 */
L8c648ec0: *stop=0x8c648ec0u;r[5]=r[12];goto L8c648ec2; /* mov r12,r5 */
L8c648ec2: *stop=0x8c648ec2u;r[6]+=16u;goto L8c648ec4; /* add #0x10,r6 */
L8c648ec4: target=r[3];*pr=0x8c648ec8u;*stop=0x8c648ec6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648ec8; /* jsr @r3 */
L8c648ec8: *stop=0x8c648ec8u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648eca; /* cmp/pz r0 */
L8c648eca: if(!(*sr&1u))goto L8c648efc;goto L8c648ecc; /* bf 0x8c648efc */
L8c648ecc: *stop=0x8c648eccu;r[0]=sx(m,bus,&io_status,0x8c648edau,2);if(io_status)return io_status;goto L8c648ece; /* mov.w 0x8c648eda,r0 */
L8c648ece: *stop=0x8c648ed0u;goto L8c648f8a; /* bra 0x8c648f8a */
L8c648efc: *stop=0x8c648efcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649000u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648efe; /* mov.l 0x8c649000,r3 */
L8c648efe: *stop=0x8c648efeu;r[5]=r[15];goto L8c648f00; /* mov r15,r5 */
L8c648f00: *stop=0x8c648f00u;r[5]+=16u;goto L8c648f02; /* add #0x10,r5 */
L8c648f02: target=r[3];*pr=0x8c648f06u;*stop=0x8c648f04u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648f06; /* jsr @r3 */
L8c648f06: *stop=0x8c648f06u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c648f08; /* cmp/pz r0 */
L8c648f08: if(!(*sr&1u))goto L8c648f10;goto L8c648f0a; /* bf 0x8c648f10 */
L8c648f0a: *stop=0x8c648f0au;r[0]=sx(m,bus,&io_status,0x8c648ff4u,2);if(io_status)return io_status;goto L8c648f0c; /* mov.w 0x8c648ff4,r0 */
L8c648f0c: *stop=0x8c648f0eu;goto L8c648f8a; /* bra 0x8c648f8a */
L8c648f10: *stop=0x8c648f10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649004u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648f12; /* mov.l 0x8c649004,r3 */
L8c648f12: *stop=0x8c648f12u;r[5]=0u;goto L8c648f14; /* mov #0x0,r5 */
L8c648f14: target=r[3];*pr=0x8c648f18u;*stop=0x8c648f16u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648f18; /* jsr @r3 */
L8c648f18: *stop=0x8c648f18u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649008u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648f1a; /* mov.l 0x8c649008,r2 */
L8c648f1a: *stop=0x8c648f1au;r[0]=r[0]&65535u;goto L8c648f1c; /* extu.w r0,r0 */
L8c648f1c: *stop=0x8c648f1cu;*sr=(*sr&~1u)|(r[2]==r[0]);goto L8c648f1e; /* cmp/eq r2,r0 */
L8c648f1e: if(!(*sr&1u))goto L8c648f2c;goto L8c648f20; /* bf 0x8c648f2c */
L8c648f20: *stop=0x8c648f20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64900cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648f22; /* mov.l 0x8c64900c,r3 */
L8c648f22: target=r[3];*pr=0x8c648f26u;*stop=0x8c648f24u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648f26; /* jsr @r3 */
L8c648f26: *stop=0x8c648f26u;r[10]=r[0];goto L8c648f28; /* mov r0,r10 */
L8c648f28: *stop=0x8c648f28u;*sr=(*sr&~1u)|((int32_t)r[10]>=0);goto L8c648f2a; /* cmp/pz r10 */
L8c648f2a: if((*sr&1u))goto L8c648f32;goto L8c648f2c; /* bt 0x8c648f32 */
L8c648f2c: *stop=0x8c648f2cu;r[0]=sx(m,bus,&io_status,0x8c648ff6u,2);if(io_status)return io_status;goto L8c648f2e; /* mov.w 0x8c648ff6,r0 */
L8c648f2e: *stop=0x8c648f30u;goto L8c648f8a; /* bra 0x8c648f8a */
L8c648f32: *stop=0x8c648f32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649010u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648f34; /* mov.l 0x8c649010,r3 */
L8c648f34: *stop=0x8c648f34u;r[5]=r[10];goto L8c648f36; /* mov r10,r5 */
L8c648f36: target=r[3];*pr=0x8c648f3au;*stop=0x8c648f38u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648f3a; /* jsr @r3 */
L8c648f3a: *stop=0x8c648f3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c649014u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648f3c; /* mov.l 0x8c649014,r2 */
L8c648f3c: target=r[2];*pr=0x8c648f40u;*stop=0x8c648f3eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648f40; /* jsr @r2 */
L8c648f40: *stop=0x8c648f40u;r[13]=r[0];goto L8c648f42; /* mov r0,r13 */
L8c648f42: *stop=0x8c648f42u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c648f44; /* tst r13,r13 */
L8c648f44: if(!(*sr&1u))goto L8c648f4c;goto L8c648f46; /* bf 0x8c648f4c */
L8c648f46: *stop=0x8c648f46u;r[0]=sx(m,bus,&io_status,0x8c648ff8u,2);if(io_status)return io_status;goto L8c648f48; /* mov.w 0x8c648ff8,r0 */
L8c648f48: *stop=0x8c648f4au;goto L8c648f8a; /* bra 0x8c648f8a */
L8c648f4c: *stop=0x8c648f4cu;r[4]=1u;goto L8c648f4e; /* mov #0x1,r4 */
L8c648f4e: *stop=0x8c648f4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c649018u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648f50; /* mov.l 0x8c649018,r3 */
L8c648f50: *stop=0x8c648f50u;r[0]=r[4];goto L8c648f52; /* mov r4,r0 */
L8c648f52: *stop=0x8c648f52u;r[2]=2u;goto L8c648f54; /* mov #0x2,r2 */
L8c648f54: *stop=0x8c648f54u;if(!valid_access(r[13]+8u,4u))return -3;wr(m,bus,&io_status,r[13]+0x8u,r[4],4);if(io_status)return io_status;goto L8c648f56; /* mov.l r4,@(0x8,r13) */
L8c648f56: *stop=0x8c648f56u;r[4]=r[13];goto L8c648f58; /* mov r13,r4 */
L8c648f58: *stop=0x8c648f58u;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[2],4);if(io_status)return io_status;goto L8c648f5a; /* mov.l r2,@(0x4,r13) */
L8c648f5a: *stop=0x8c648f5au;if(!valid_access(r[13]+44u,4u))return -3;wr(m,bus,&io_status,r[13]+0x2cu,r[10],4);if(io_status)return io_status;goto L8c648f5c; /* mov.l r10,@(0x2c,r13) */
L8c648f5c: *stop=0x8c648f5cu;if(!valid_access(r[13]+56u,4u))return -3;wr(m,bus,&io_status,r[13]+0x38u,r[3],4);if(io_status)return io_status;goto L8c648f5e; /* mov.l r3,@(0x38,r13) */
L8c648f5e: *stop=0x8c648f5eu;if(!valid_access(r[13]+48u,4u))return -3;wr(m,bus,&io_status,r[13]+0x30u,r[11],4);if(io_status)return io_status;goto L8c648f60; /* mov.l r11,@(0x30,r13) */
L8c648f60: *stop=0x8c648f60u;if(!valid_access(r[13]+40u,4u))return -3;wr(m,bus,&io_status,r[13]+0x28u,r[14],4);if(io_status)return io_status;goto L8c648f62; /* mov.l r14,@(0x28,r13) */
L8c648f62: *stop=0x8c648f62u;if(!valid_access(r[13]+2u,2u))return -3;wr(m,bus,&io_status,r[13]+0x2u,r[0],2);if(io_status)return io_status;goto L8c648f64; /* mov.w r0,@(0x2,r13) */
L8c648f64: *stop=0x8c648f64u;r[0]=r[11];goto L8c648f66; /* mov r11,r0 */
L8c648f66: *stop=0x8c648f66u;if(!valid_access(r[13]+1u,1u))return -3;wr(m,bus,&io_status,r[13]+0x1u,r[0],1);if(io_status)return io_status;goto L8c648f68; /* mov.b r0,@(0x1,r13) */
L8c648f68: *stop=0x8c648f6au;r[4]+=16u;goto L8c648f72; /* bra 0x8c648f72 */
L8c648f6c: *stop=0x8c648f6cu;if(!valid_access(r[12],1u))return -3;r[3]=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;r[12]+=1;goto L8c648f6e; /* mov.b @r12+,r3 */
L8c648f6e: *stop=0x8c648f6eu;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[3],1);if(io_status)return io_status;goto L8c648f70; /* mov.b r3,@r4 */
L8c648f70: *stop=0x8c648f70u;r[4]+=1u;goto L8c648f72; /* add #0x1,r4 */
L8c648f72: *stop=0x8c648f72u;if(!valid_access(r[12],1u))return -3;r[2]=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;goto L8c648f74; /* mov.b @r12,r2 */
L8c648f74: *stop=0x8c648f74u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c648f76; /* tst r2,r2 */
L8c648f76: if(!(*sr&1u))goto L8c648f6c;goto L8c648f78; /* bf 0x8c648f6c */
L8c648f78: *stop=0x8c648f78u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64901cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c648f7a; /* mov.l 0x8c64901c,r3 */
L8c648f7a: *stop=0x8c648f7au;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[11],1);if(io_status)return io_status;goto L8c648f7c; /* mov.b r11,@r4 */
L8c648f7c: *stop=0x8c648f7cu;r[4]=r[13];goto L8c648f7e; /* mov r13,r4 */
L8c648f7e: *stop=0x8c648f7eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c648f80; /* mov.l @(0x8,r15),r5 */
L8c648f80: target=r[3];*pr=0x8c648f84u;*stop=0x8c648f82u;r[4]+=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648f84; /* jsr @r3 */
L8c648f84: *stop=0x8c648f84u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c648f86; /* mov.l @(0x4,r15),r2 */
L8c648f86: *stop=0x8c648f86u;r[0]=0u;goto L8c648f88; /* mov #0x0,r0 */
L8c648f88: *stop=0x8c648f88u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c648f8a; /* mov.l r13,@r2 */
L8c648f8a: *stop=0x8c648f8au;r[15]+=60u;goto L8c648f8c; /* add #0x3c,r15 */
L8c648f8c: *stop=0x8c648f8cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c648f8e; /* lds.l @r15+,PR */
L8c648f8e: *stop=0x8c648f8eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c648f90; /* mov.l @r15+,r10 */
L8c648f90: *stop=0x8c648f90u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c648f92; /* mov.l @r15+,r11 */
L8c648f92: *stop=0x8c648f92u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c648f94; /* mov.l @r15+,r12 */
L8c648f94: *stop=0x8c648f94u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c648f96; /* mov.l @r15+,r13 */
L8c648f96: *stop=0x8c648f96u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c648f98u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_8c648e36(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c648e36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c648e6e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c648e36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648e6eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c648e76(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c648e36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648e76u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c648e80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c648e36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648e80u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c648e9c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c648e36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648e9cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c648eb8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c648e36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648eb8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c648ec8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c648e36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648ec8u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c648f06(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c648e36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648f06u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c648f18(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c648e36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648f18u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c648f26(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c648e36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648f26u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c648f3a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c648e36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648f3au);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c648f40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c648e36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648f40u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c648f84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c648e36(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c648f84u);}

static int sa_movie_filesystem_command13_run_8c6494e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c649504u)goto L8c649504;if(resume_pc==0x8c64950eu)goto L8c64950e;if(resume_pc==0x8c649518u)goto L8c649518;if(resume_pc==0x8c649538u)goto L8c649538;if(resume_pc==0x8c64954eu)goto L8c64954e;if(resume_pc==0x8c649572u)goto L8c649572;if(resume_pc==0x8c6495bau)goto L8c6495ba;if(resume_pc==0x8c6495d2u)goto L8c6495d2;goto L8c6494e2;
L8c6494e2: *stop=0x8c6494e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6494e4; /* mov.l r14,@-r15 */
L8c6494e4: *stop=0x8c6494e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6494e6; /* mov.l r13,@-r15 */
L8c6494e6: *stop=0x8c6494e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6494e8; /* mov.l r12,@-r15 */
L8c6494e8: *stop=0x8c6494e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6494ea; /* mov.l r11,@-r15 */
L8c6494ea: *stop=0x8c6494eau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6494ec; /* sts.l PR,@-r15 */
L8c6494ec: *stop=0x8c6494ecu;r[15]+=4294967288u;goto L8c6494ee; /* add #-0x8,r15 */
L8c6494ee: *stop=0x8c6494eeu;r[14]=r[4];goto L8c6494f0; /* mov r4,r14 */
L8c6494f0: *stop=0x8c6494f0u;r[3]=sx(m,bus,&io_status,0x8c64957cu,2);if(io_status)return io_status;goto L8c6494f2; /* mov.w 0x8c64957c,r3 */
L8c6494f2: *stop=0x8c6494f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64958cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6494f4; /* mov.l 0x8c64958c,r2 */
L8c6494f4: *stop=0x8c6494f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649590u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6494f6; /* mov.l 0x8c649590,r1 */
L8c6494f6: *stop=0x8c6494f6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c6494f8; /* mov.l r5,@r15 */
L8c6494f8: *stop=0x8c6494f8u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6494fa; /* mov.l @(0x28,r14),r4 */
L8c6494fa: *stop=0x8c6494fau;*macl=r[3]*r[4];goto L8c6494fc; /* mul.l r3,r4 */
L8c6494fc: *stop=0x8c6494fcu;r[3]=*macl;goto L8c6494fe; /* sts MACL,r3 */
L8c6494fe: *stop=0x8c6494feu;r[3]+=r[2];goto L8c649500; /* add r2,r3 */
L8c649500: target=r[1];*pr=0x8c649504u;*stop=0x8c649502u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649504; /* jsr @r1 */
L8c649504: *stop=0x8c649504u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c649506; /* tst r0,r0 */
L8c649506: if(!(*sr&1u))goto L8c649512;goto L8c649508; /* bf 0x8c649512 */
L8c649508: *stop=0x8c649508u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649594u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64950a; /* mov.l 0x8c649594,r2 */
L8c64950a: target=r[2];*pr=0x8c64950eu;*stop=0x8c64950cu;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64950e; /* jsr @r2 */
L8c64950e: *stop=0x8c649510u;goto L8c64952c; /* bra 0x8c64952c */
L8c649512: *stop=0x8c649512u;{uint32_t loaded=rd(m,bus,&io_status,0x8c649598u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649514; /* mov.l 0x8c649598,r3 */
L8c649514: target=r[3];*pr=0x8c649518u;*stop=0x8c649516u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649518; /* jsr @r3 */
L8c649518: *stop=0x8c649518u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64951a; /* tst r0,r0 */
L8c64951a: if((*sr&1u))goto L8c64952c;goto L8c64951c; /* bt 0x8c64952c */
L8c64951c: *stop=0x8c64951cu;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64951e; /* mov.l @(0x28,r14),r2 */
L8c64951e: *stop=0x8c64951eu;r[3]=sx(m,bus,&io_status,0x8c64957cu,2);if(io_status)return io_status;goto L8c649520; /* mov.w 0x8c64957c,r3 */
L8c649520: *stop=0x8c649520u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64959cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c649522; /* mov.l 0x8c64959c,r0 */
L8c649522: *stop=0x8c649522u;*macl=r[3]*r[2];goto L8c649524; /* mul.l r3,r2 */
L8c649524: *stop=0x8c649524u;r[3]=*macl;goto L8c649526; /* sts MACL,r3 */
L8c649526: *stop=0x8c649526u;if(!valid_access(r[0]+r[3],1u))return -3;r[1]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8c649528; /* mov.b @(r0,r3),r1 */
L8c649528: *stop=0x8c649528u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c64952a; /* tst r1,r1 */
L8c64952a: if(!(*sr&1u))goto L8c649532;goto L8c64952c; /* bf 0x8c649532 */
L8c64952c: *stop=0x8c64952cu;r[0]=sx(m,bus,&io_status,0x8c64957eu,2);if(io_status)return io_status;goto L8c64952e; /* mov.w 0x8c64957e,r0 */
L8c64952e: *stop=0x8c649530u;goto L8c6495dc; /* bra 0x8c6495dc */
L8c649532: *stop=0x8c649532u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6495a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649534; /* mov.l 0x8c6495a0,r3 */
L8c649534: target=r[3];*pr=0x8c649538u;*stop=0x8c649536u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649538; /* jsr @r3 */
L8c649538: *stop=0x8c649538u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c64953a; /* cmp/pz r0 */
L8c64953a: if((*sr&1u))goto L8c649542;goto L8c64953c; /* bt 0x8c649542 */
L8c64953c: *stop=0x8c64953cu;r[0]=sx(m,bus,&io_status,0x8c649580u,2);if(io_status)return io_status;goto L8c64953e; /* mov.w 0x8c649580,r0 */
L8c64953e: *stop=0x8c649540u;goto L8c6495dc; /* bra 0x8c6495dc */
L8c649542: *stop=0x8c649542u;if(!valid_access(r[14]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x30u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c649544; /* mov.l @(0x30,r14),r13 */
L8c649544: *stop=0x8c649544u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6495a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649546; /* mov.l 0x8c6495a4,r3 */
L8c649546: *stop=0x8c649546u;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c649548; /* mov.l @(0x28,r14),r12 */
L8c649548: *stop=0x8c649548u;r[5]=r[13];goto L8c64954a; /* mov r13,r5 */
L8c64954a: target=r[3];*pr=0x8c64954eu;*stop=0x8c64954cu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64954e; /* jsr @r3 */
L8c64954e: *stop=0x8c64954eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6495a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c649550; /* mov.l 0x8c6495a8,r2 */
L8c649550: *stop=0x8c649550u;r[0]=r[0]&65535u;goto L8c649552; /* extu.w r0,r0 */
L8c649552: *stop=0x8c649552u;*sr=(*sr&~1u)|(r[2]==r[0]);goto L8c649554; /* cmp/eq r2,r0 */
L8c649554: if(!(*sr&1u))goto L8c649562;goto L8c649556; /* bf 0x8c649562 */
L8c649556: *stop=0x8c649556u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c649558; /* mov.l @(0x4,r15),r3 */
L8c649558: *stop=0x8c649558u;if(!valid_access(r[14]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x30u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64955a; /* mov.l @(0x30,r14),r0 */
L8c64955a: *stop=0x8c64955au;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64955c; /* mov.l @(0x1c,r3),r2 */
L8c64955c: *stop=0x8c64955cu;if(!valid_access(r[2]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x38u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64955e; /* mov.l @(0x38,r2),r1 */
L8c64955e: *stop=0x8c64955eu;*sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[1]);goto L8c649560; /* cmp/ge r1,r0 */
L8c649560: if(!(*sr&1u))goto L8c649568;goto L8c649562; /* bf 0x8c649568 */
L8c649562: *stop=0x8c649562u;r[0]=sx(m,bus,&io_status,0x8c649582u,2);if(io_status)return io_status;goto L8c649564; /* mov.w 0x8c649582,r0 */
L8c649564: *stop=0x8c649566u;goto L8c6495dc; /* bra 0x8c6495dc */
L8c649568: *stop=0x8c649568u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6495acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64956a; /* mov.l 0x8c6495ac,r3 */
L8c64956a: *stop=0x8c64956au;r[5]=r[13];goto L8c64956c; /* mov r13,r5 */
L8c64956c: *stop=0x8c64956cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64956e; /* mov.l @r15,r6 */
L8c64956e: target=r[3];*pr=0x8c649572u;*stop=0x8c649570u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c649572; /* jsr @r3 */
L8c649572: *stop=0x8c649572u;r[4]=r[0];goto L8c649574; /* mov r0,r4 */
L8c649574: *stop=0x8c649574u;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c649576; /* cmp/pz r4 */
L8c649576: if((*sr&1u))goto L8c6495b0;goto L8c649578; /* bt 0x8c6495b0 */
L8c649578: *stop=0x8c64957au;r[0]=r[4];goto L8c6495dc; /* bra 0x8c6495dc */
L8c6495b0: *stop=0x8c6495b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6496acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6495b2; /* mov.l 0x8c6496ac,r3 */
L8c6495b2: *stop=0x8c6495b2u;r[5]=r[13];goto L8c6495b4; /* mov r13,r5 */
L8c6495b4: *stop=0x8c6495b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6496a8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c6495b6; /* mov.l 0x8c6496a8,r6 */
L8c6495b6: target=r[3];*pr=0x8c6495bau;*stop=0x8c6495b8u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6495ba; /* jsr @r3 */
L8c6495ba: *stop=0x8c6495bau;if(!valid_access(r[14]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6495bc; /* mov.l @(0x38,r14),r3 */
L8c6495bc: *stop=0x8c6495bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6496b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6495be; /* mov.l 0x8c6496b0,r2 */
L8c6495be: *stop=0x8c6495beu;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c6495c0; /* cmp/eq r2,r3 */
L8c6495c0: take=!(*sr&1u);*stop=0x8c6495c2u;r[11]=(uint32_t)(int32_t)(int16_t)r[13];if(take)goto L8c6495c8;goto L8c6495c4; /* bf/s 0x8c6495c8 */
L8c6495c4: *stop=0x8c6495c6u;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[11],4);if(io_status)return io_status;goto L8c6495d2; /* bra 0x8c6495d2 */
L8c6495c8: *stop=0x8c6495c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6496acu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6495ca; /* mov.l 0x8c6496ac,r1 */
L8c6495ca: *stop=0x8c6495cau;r[6]=r[13];goto L8c6495cc; /* mov r13,r6 */
L8c6495cc: *stop=0x8c6495ccu;if(!valid_access(r[14]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x3cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6495ce; /* mov.l @(0x3c,r14),r5 */
L8c6495ce: target=r[1];*pr=0x8c6495d2u;*stop=0x8c6495d0u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6495d2; /* jsr @r1 */
L8c6495d2: *stop=0x8c6495d2u;r[0]=0u;goto L8c6495d4; /* mov #0x0,r0 */
L8c6495d4: *stop=0x8c6495d4u;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[11],4);if(io_status)return io_status;goto L8c6495d6; /* mov.l r11,@(0x3c,r14) */
L8c6495d6: *stop=0x8c6495d6u;if(!valid_access(r[14]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6495d8; /* mov.l @(0x30,r14),r3 */
L8c6495d8: *stop=0x8c6495d8u;r[3]+=1u;goto L8c6495da; /* add #0x1,r3 */
L8c6495da: *stop=0x8c6495dau;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[3],4);if(io_status)return io_status;goto L8c6495dc; /* mov.l r3,@(0x30,r14) */
L8c6495dc: *stop=0x8c6495dcu;r[15]+=8u;goto L8c6495de; /* add #0x8,r15 */
L8c6495de: *stop=0x8c6495deu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6495e0; /* lds.l @r15+,PR */
L8c6495e0: *stop=0x8c6495e0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6495e2; /* mov.l @r15+,r11 */
L8c6495e2: *stop=0x8c6495e2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6495e4; /* mov.l @r15+,r12 */
L8c6495e4: *stop=0x8c6495e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6495e6; /* mov.l @r15+,r13 */
L8c6495e6: *stop=0x8c6495e6u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6495e8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_8c6494e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c6494e2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c649504(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c6494e2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649504u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64950e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c6494e2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64950eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c649518(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c6494e2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649518u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c649538(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c6494e2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649538u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64954e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c6494e2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64954eu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c649572(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c6494e2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c649572u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c6495ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c6494e2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6495bau);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c6495d2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c6494e2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6495d2u);}

static int sa_movie_filesystem_command13_run_8c64ba30(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c64ba56u)goto L8c64ba56;if(resume_pc==0x8c64ba9cu)goto L8c64ba9c;if(resume_pc==0x8c64bab2u)goto L8c64bab2;if(resume_pc==0x8c64babcu)goto L8c64babc;if(resume_pc==0x8c64bae2u)goto L8c64bae2;if(resume_pc==0x8c64baf0u)goto L8c64baf0;if(resume_pc==0x8c64bb0au)goto L8c64bb0a;goto L8c64ba30;
L8c64ba30: *stop=0x8c64ba30u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64ba32; /* mov.l r14,@-r15 */
L8c64ba32: *stop=0x8c64ba32u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c64ba34; /* mov.l r13,@-r15 */
L8c64ba34: *stop=0x8c64ba34u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c64ba36; /* mov.l r12,@-r15 */
L8c64ba36: *stop=0x8c64ba36u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c64ba38; /* mov.l r11,@-r15 */
L8c64ba38: *stop=0x8c64ba38u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c64ba3a; /* mov.l r10,@-r15 */
L8c64ba3a: *stop=0x8c64ba3au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c64ba3c; /* mov.l r9,@-r15 */
L8c64ba3c: *stop=0x8c64ba3cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64ba3e; /* mov.l r8,@-r15 */
L8c64ba3e: *stop=0x8c64ba3eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64ba40; /* sts.l PR,@-r15 */
L8c64ba40: *stop=0x8c64ba40u;r[15]+=4294967284u;goto L8c64ba42; /* add #-0xc,r15 */
L8c64ba42: *stop=0x8c64ba42u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c64ba44; /* mov.l r5,@(0x4,r15) */
L8c64ba44: *stop=0x8c64ba44u;r[12]=r[7];goto L8c64ba46; /* mov r7,r12 */
L8c64ba46: *stop=0x8c64ba46u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c64ba48; /* mov.l r6,@(0x8,r15) */
L8c64ba48: *stop=0x8c64ba48u;r[9]=r[4];goto L8c64ba4a; /* mov r4,r9 */
L8c64ba4a: *stop=0x8c64ba4au;if(!valid_access(r[15]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x2cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64ba4c; /* mov.l @(0x2c,r15),r6 */
L8c64ba4c: *stop=0x8c64ba4cu;r[7]=r[15];goto L8c64ba4e; /* mov r15,r7 */
L8c64ba4e: *stop=0x8c64ba4eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64ba50; /* mov.l @(0x4,r15),r5 */
L8c64ba50: *stop=0x8c64ba50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bb28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ba52; /* mov.l 0x8c64bb28,r3 */
L8c64ba52: target=r[3];*pr=0x8c64ba56u;*stop=0x8c64ba54u;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ba56; /* jsr @r3 */
L8c64ba56: *stop=0x8c64ba56u;r[14]=r[0];goto L8c64ba58; /* mov r0,r14 */
L8c64ba58: *stop=0x8c64ba58u;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64ba5a; /* cmp/pz r14 */
L8c64ba5a: if(!(*sr&1u))goto L8c64bb0a;goto L8c64ba5c; /* bf 0x8c64bb0a */
L8c64ba5c: *stop=0x8c64ba5cu;r[4]=sx(m,bus,&io_status,0x8c64bb20u,2);if(io_status)return io_status;goto L8c64ba5e; /* mov.w 0x8c64bb20,r4 */
L8c64ba5e: *stop=0x8c64ba5eu;r[2]=1u;goto L8c64ba60; /* mov #0x1,r2 */
L8c64ba60: *stop=0x8c64ba60u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ba62; /* mov.l @r15,r3 */
L8c64ba62: *stop=0x8c64ba62u;r[1]=r[12];goto L8c64ba64; /* mov r12,r1 */
L8c64ba64: *stop=0x8c64ba64u;*macl=r[4]*r[9];goto L8c64ba66; /* mul.l r4,r9 */
L8c64ba66: *stop=0x8c64ba66u;if(!valid_access(r[15]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x30u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ba68; /* mov.l @(0x30,r15),r0 */
L8c64ba68: *stop=0x8c64ba68u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c64ba6a; /* shll r1 */
L8c64ba6a: *stop=0x8c64ba6au;r[10]=0u;goto L8c64ba6c; /* mov #0x0,r10 */
L8c64ba6c: *stop=0x8c64ba6cu;r[13]=sx(m,bus,&io_status,0x8c64bb22u,2);if(io_status)return io_status;goto L8c64ba6e; /* mov.w 0x8c64bb22,r13 */
L8c64ba6e: *stop=0x8c64ba6eu;if(!valid_access(r[3]+1u,1u))return -3;wr(m,bus,&io_status,r[3]+0x1u,r[0],1);if(io_status)return io_status;goto L8c64ba70; /* mov.b r0,@(0x1,r3) */
L8c64ba70: *stop=0x8c64ba70u;r[4]=*macl;goto L8c64ba72; /* sts MACL,r4 */
L8c64ba72: *stop=0x8c64ba72u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bb2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ba74; /* mov.l 0x8c64bb2c,r3 */
L8c64ba74: *stop=0x8c64ba74u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c64ba76; /* mov.l @(0x8,r15),r11 */
L8c64ba76: *stop=0x8c64ba76u;r[4]+=r[3];goto L8c64ba78; /* add r3,r4 */
L8c64ba78: *stop=0x8c64ba78u;if(!valid_access(r[4]+48u,4u))return -3;wr(m,bus,&io_status,r[4]+0x30u,r[2],4);if(io_status)return io_status;goto L8c64ba7a; /* mov.l r2,@(0x30,r4) */
L8c64ba7a: *stop=0x8c64ba7au;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ba7c; /* mov.l @(0x4,r4),r2 */
L8c64ba7c: *stop=0x8c64ba7cu;r[0]=r[2];goto L8c64ba7e; /* mov r2,r0 */
L8c64ba7e: *stop=0x8c64ba7eu;r[2]<<=2;goto L8c64ba80; /* shll2 r2 */
L8c64ba80: *stop=0x8c64ba80u;r[2]+=r[0];goto L8c64ba82; /* add r0,r2 */
L8c64ba82: *stop=0x8c64ba82u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bb30u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64ba84; /* mov.l 0x8c64bb30,r0 */
L8c64ba84: *stop=0x8c64ba84u;r[2]<<=2;goto L8c64ba86; /* shll2 r2 */
L8c64ba86: *stop=0x8c64ba86u;r[2]<<=2;goto L8c64ba88; /* shll2 r2 */
L8c64ba88: *stop=0x8c64ba88u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64ba8a; /* mov.l @(r0,r2),r2 */
L8c64ba8a: *stop=0x8c64ba8au;r[1]+=r[2];goto L8c64ba8c; /* add r2,r1 */
L8c64ba8c: *stop=0x8c64ba8cu;r[2]=r[10];goto L8c64ba8e; /* mov r10,r2 */
L8c64ba8e: *stop=0x8c64ba8eu;*sr=(*sr&~1u)|(r[2]>=r[12]);goto L8c64ba90; /* cmp/hs r12,r2 */
L8c64ba90: take=(*sr&1u);*stop=0x8c64ba92u;if(!valid_access(r[4]+52u,4u))return -3;wr(m,bus,&io_status,r[4]+0x34u,r[1],4);if(io_status)return io_status;if(take)goto L8c64baaa;goto L8c64ba94; /* bt/s 0x8c64baaa */
L8c64ba94: *stop=0x8c64ba94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bb34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64ba96; /* mov.l 0x8c64bb34,r3 */
L8c64ba96: *stop=0x8c64ba96u;r[5]=r[11];goto L8c64ba98; /* mov r11,r5 */
L8c64ba98: target=r[3];*pr=0x8c64ba9cu;*stop=0x8c64ba9au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64ba9c; /* jsr @r3 */
L8c64ba9c: *stop=0x8c64ba9cu;r[14]=r[0];goto L8c64ba9e; /* mov r0,r14 */
L8c64ba9e: *stop=0x8c64ba9eu;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64baa0; /* cmp/pz r14 */
L8c64baa0: if(!(*sr&1u))goto L8c64bb02;goto L8c64baa2; /* bf 0x8c64bb02 */
L8c64baa2: *stop=0x8c64baa2u;r[10]+=1u;goto L8c64baa4; /* add #0x1,r10 */
L8c64baa4: *stop=0x8c64baa4u;*sr=(*sr&~1u)|(r[10]>=r[12]);goto L8c64baa6; /* cmp/hs r12,r10 */
L8c64baa6: take=!(*sr&1u);*stop=0x8c64baa8u;r[11]+=r[13];if(take)goto L8c64ba94;goto L8c64baaa; /* bf/s 0x8c64ba94 */
L8c64baaa: *stop=0x8c64baaau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bb38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64baac; /* mov.l 0x8c64bb38,r3 */
L8c64baac: *stop=0x8c64baacu;r[5]=0u;goto L8c64baae; /* mov #0x0,r5 */
L8c64baae: target=r[3];*pr=0x8c64bab2u;*stop=0x8c64bab0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bab2; /* jsr @r3 */
L8c64bab2: *stop=0x8c64bab2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bb3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bab4; /* mov.l 0x8c64bb3c,r3 */
L8c64bab4: *stop=0x8c64bab4u;r[6]=r[15];goto L8c64bab6; /* mov r15,r6 */
L8c64bab6: *stop=0x8c64bab6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c64bab8; /* mov.l @(0x4,r15),r5 */
L8c64bab8: target=r[3];*pr=0x8c64babcu;*stop=0x8c64babau;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64babc; /* jsr @r3 */
L8c64babc: *stop=0x8c64babcu;r[14]=r[0];goto L8c64babe; /* mov r0,r14 */
L8c64babe: *stop=0x8c64babeu;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64bac0; /* cmp/pz r14 */
L8c64bac0: if(!(*sr&1u))goto L8c64bb0a;goto L8c64bac2; /* bf 0x8c64bb0a */
L8c64bac2: *stop=0x8c64bac2u;r[8]=r[9];goto L8c64bac4; /* mov r9,r8 */
L8c64bac4: *stop=0x8c64bac4u;r[8]<<=8;goto L8c64bac6; /* shll8 r8 */
L8c64bac6: *stop=0x8c64bac6u;r[11]=0u;goto L8c64bac8; /* mov #0x0,r11 */
L8c64bac8: *stop=0x8c64bac8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bb40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64baca; /* mov.l 0x8c64bb40,r3 */
L8c64baca: *stop=0x8c64bacau;r[2]=r[11];goto L8c64bacc; /* mov r11,r2 */
L8c64bacc: *stop=0x8c64baccu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c64bace; /* mov.l @(0x8,r15),r10 */
L8c64bace: *stop=0x8c64baceu;r[8]<<=2;goto L8c64bad0; /* shll2 r8 */
L8c64bad0: *stop=0x8c64bad0u;*sr=(*sr&~1u)|(r[8]>>31);r[8]<<=1;goto L8c64bad2; /* shll r8 */
L8c64bad2: *stop=0x8c64bad2u;*sr=(*sr&~1u)|(r[2]>=r[12]);goto L8c64bad4; /* cmp/hs r12,r2 */
L8c64bad4: take=(*sr&1u);*stop=0x8c64bad6u;r[8]+=r[3];if(take)goto L8c64bb00;goto L8c64bad8; /* bt/s 0x8c64bb00 */
L8c64bad8: *stop=0x8c64bad8u;r[9]=sx(m,bus,&io_status,0x8c64bb24u,2);if(io_status)return io_status;goto L8c64bada; /* mov.w 0x8c64bb24,r9 */
L8c64bada: *stop=0x8c64badau;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bb44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64badc; /* mov.l 0x8c64bb44,r3 */
L8c64badc: *stop=0x8c64badcu;r[5]=r[8];goto L8c64bade; /* mov r8,r5 */
L8c64bade: target=r[3];*pr=0x8c64bae2u;*stop=0x8c64bae0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bae2; /* jsr @r3 */
L8c64bae2: *stop=0x8c64bae2u;r[14]=r[0];goto L8c64bae4; /* mov r0,r14 */
L8c64bae4: *stop=0x8c64bae4u;*sr=(*sr&~1u)|((int32_t)r[14]>=0);goto L8c64bae6; /* cmp/pz r14 */
L8c64bae6: if(!(*sr&1u))goto L8c64bb02;goto L8c64bae8; /* bf 0x8c64bb02 */
L8c64bae8: *stop=0x8c64bae8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bb48u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64baea; /* mov.l 0x8c64bb48,r2 */
L8c64baea: *stop=0x8c64baeau;r[5]=r[10];goto L8c64baec; /* mov r10,r5 */
L8c64baec: target=r[2];*pr=0x8c64baf0u;*stop=0x8c64baeeu;r[4]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64baf0; /* jsr @r2 */
L8c64baf0: *stop=0x8c64baf0u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64baf2; /* tst r0,r0 */
L8c64baf2: if((*sr&1u))goto L8c64baf8;goto L8c64baf4; /* bt 0x8c64baf8 */
L8c64baf4: *stop=0x8c64baf6u;r[14]=r[9];goto L8c64bb02; /* bra 0x8c64bb02 */
L8c64baf8: *stop=0x8c64baf8u;r[11]+=1u;goto L8c64bafa; /* add #0x1,r11 */
L8c64bafa: *stop=0x8c64bafau;*sr=(*sr&~1u)|(r[11]>=r[12]);goto L8c64bafc; /* cmp/hs r12,r11 */
L8c64bafc: take=!(*sr&1u);*stop=0x8c64bafeu;r[10]+=r[13];if(take)goto L8c64bada;goto L8c64bb00; /* bf/s 0x8c64bada */
L8c64bb00: *stop=0x8c64bb00u;r[14]=0u;goto L8c64bb02; /* mov #0x0,r14 */
L8c64bb02: *stop=0x8c64bb02u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64bb38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64bb04; /* mov.l 0x8c64bb38,r3 */
L8c64bb04: *stop=0x8c64bb04u;r[5]=0u;goto L8c64bb06; /* mov #0x0,r5 */
L8c64bb06: target=r[3];*pr=0x8c64bb0au;*stop=0x8c64bb08u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bb0a; /* jsr @r3 */
L8c64bb0a: *stop=0x8c64bb0au;r[0]=r[14];goto L8c64bb0c; /* mov r14,r0 */
L8c64bb0c: *stop=0x8c64bb0cu;r[15]+=12u;goto L8c64bb0e; /* add #0xc,r15 */
L8c64bb0e: *stop=0x8c64bb0eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64bb10; /* lds.l @r15+,PR */
L8c64bb10: *stop=0x8c64bb10u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c64bb12; /* mov.l @r15+,r8 */
L8c64bb12: *stop=0x8c64bb12u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c64bb14; /* mov.l @r15+,r9 */
L8c64bb14: *stop=0x8c64bb14u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c64bb16; /* mov.l @r15+,r10 */
L8c64bb16: *stop=0x8c64bb16u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c64bb18; /* mov.l @r15+,r11 */
L8c64bb18: *stop=0x8c64bb18u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c64bb1a; /* mov.l @r15+,r12 */
L8c64bb1a: *stop=0x8c64bb1au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64bb1c; /* mov.l @r15+,r13 */
L8c64bb1c: *stop=0x8c64bb1cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c64bb1eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_8c64ba30(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64ba30(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64ba56(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64ba30(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ba56u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64ba9c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64ba30(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64ba9cu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64bab2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64ba30(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64bab2u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64babc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64ba30(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64babcu);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64bae2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64ba30(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64bae2u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64baf0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64ba30(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64baf0u);}
SA_STAGE_EXPORT int sa_movie_filesystem_command13_resume_8c64bb0a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_filesystem_command13_run_8c64ba30(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64bb0au);}
