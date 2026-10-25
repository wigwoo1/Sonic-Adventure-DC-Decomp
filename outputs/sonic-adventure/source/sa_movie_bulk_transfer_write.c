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
 if((n==1u||n==2u||n==4u)&&!(a&(n-1u))&&(a&0x1fffffffu)>=0x00800000u&&(a&0x1fffffffu)<=0x00a00000u-n)return 1;
 if(n==4u && (a&0x1fffffffu)==0x005f688cu)return 1;
 if((n==1u||n==2u||n==4u) && !(a&(n-1u)) && (a&0x1fffffffu)>=0x00702c00u && (a&0x1fffffffu)+n<=0x00702c04u)return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_bulk_transfer_8c10d770(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c10d770;
L8c10d770: *stop=0x8c10d770u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c10d772; /* mov.l r14,@-r15 */
L8c10d772: *stop=0x8c10d772u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c10d774; /* mov.l r13,@-r15 */
L8c10d774: *stop=0x8c10d774u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c10d776; /* mov.l r12,@-r15 */
L8c10d776: *stop=0x8c10d776u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c10d778; /* mov.l r11,@-r15 */
L8c10d778: *stop=0x8c10d778u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c10d77a; /* mov.l r10,@-r15 */
L8c10d77a: *stop=0x8c10d77au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c10d77c; /* mov.l r9,@-r15 */
L8c10d77c: *stop=0x8c10d77cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10d77e; /* mov.l r8,@-r15 */
L8c10d77e: *stop=0x8c10d77eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c10d780; /* sts.l PR,@-r15 */
L8c10d780: *stop=0x8c10d780u;r[15]+=4294967280u;goto L8c10d782; /* add #-0x10,r15 */
L8c10d782: *stop=0x8c10d782u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c10d784; /* mov.l r5,@r15 */
L8c10d784: *stop=0x8c10d784u;r[5]=1u;goto L8c10d786; /* mov #0x1,r5 */
L8c10d786: *stop=0x8c10d786u;r[8]=r[4];goto L8c10d788; /* mov r4,r8 */
L8c10d788: *stop=0x8c10d788u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c10d78a; /* mov.l r6,@(0x4,r15) */
L8c10d78a: *stop=0x8c10d78au;if(!valid_access(r[15]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x30u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d78c; /* mov.l @(0x30,r15),r0 */
L8c10d78c: *stop=0x8c10d78cu;r[13]=r[7];goto L8c10d78e; /* mov r7,r13 */
L8c10d78e: *stop=0x8c10d78eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10d790; /* cmp/eq #0x0,r0 */
L8c10d790: take=(*sr&1u);*stop=0x8c10d792u;r[4]=4294967295u;if(take)goto L8c10d7a0;goto L8c10d794; /* bt/s 0x8c10d7a0 */
L8c10d794: *stop=0x8c10d794u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d796; /* cmp/eq #0x1,r0 */
L8c10d796: if((*sr&1u))goto L8c10d7a4;goto L8c10d798; /* bt 0x8c10d7a4 */
L8c10d798: *stop=0x8c10d798u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10d79a; /* cmp/eq #0x2,r0 */
L8c10d79a: if((*sr&1u))goto L8c10d7a8;goto L8c10d79c; /* bt 0x8c10d7a8 */
L8c10d79c: *stop=0x8c10d79eu;goto L8c10d7aa; /* bra 0x8c10d7aa */
L8c10d7a0: *stop=0x8c10d7a2u;r[10]=0u;goto L8c10d7aa; /* bra 0x8c10d7aa */
L8c10d7a4: *stop=0x8c10d7a6u;r[10]=r[5];goto L8c10d7aa; /* bra 0x8c10d7aa */
L8c10d7a8: *stop=0x8c10d7a8u;r[10]=r[4];goto L8c10d7aa; /* mov r4,r10 */
L8c10d7aa: *stop=0x8c10d7aau;if(!valid_access(r[15]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x34u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d7ac; /* mov.l @(0x34,r15),r0 */
L8c10d7ac: *stop=0x8c10d7acu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c10d7ae; /* cmp/eq #0x0,r0 */
L8c10d7ae: if((*sr&1u))goto L8c10d7bc;goto L8c10d7b0; /* bt 0x8c10d7bc */
L8c10d7b0: *stop=0x8c10d7b0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d7b2; /* cmp/eq #0x1,r0 */
L8c10d7b2: if((*sr&1u))goto L8c10d7c0;goto L8c10d7b4; /* bt 0x8c10d7c0 */
L8c10d7b4: *stop=0x8c10d7b4u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10d7b6; /* cmp/eq #0x2,r0 */
L8c10d7b6: if((*sr&1u))goto L8c10d7c4;goto L8c10d7b8; /* bt 0x8c10d7c4 */
L8c10d7b8: *stop=0x8c10d7bau;goto L8c10d7c6; /* bra 0x8c10d7c6 */
L8c10d7bc: *stop=0x8c10d7beu;r[9]=0u;goto L8c10d7c6; /* bra 0x8c10d7c6 */
L8c10d7c0: *stop=0x8c10d7c2u;r[9]=r[5];goto L8c10d7c6; /* bra 0x8c10d7c6 */
L8c10d7c4: *stop=0x8c10d7c4u;r[9]=r[4];goto L8c10d7c6; /* mov r4,r9 */
L8c10d7c6: *stop=0x8c10d7c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d844u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d7c8; /* mov.l 0x8c10d844,r3 */
L8c10d7c8: *stop=0x8c10d7c8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d840u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c10d7ca; /* mov.w 0x8c10d840,r1 */
L8c10d7ca: *stop=0x8c10d7cau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d7cc; /* mov.l @r3,r0 */
L8c10d7cc: *stop=0x8c10d7ccu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d7ce; /* mov.l @(r0,r1),r0 */
L8c10d7ce: *stop=0x8c10d7ceu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d7d0; /* cmp/eq #0x1,r0 */
L8c10d7d0: if(!(*sr&1u))goto L8c10d7e6;goto L8c10d7d2; /* bf 0x8c10d7e6 */
L8c10d7d2: *stop=0x8c10d7d2u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c10d7d4; /* stc SR,r0 */
L8c10d7d4: *stop=0x8c10d7d4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d842u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10d7d6; /* mov.w 0x8c10d842,r3 */
L8c10d7d6: *stop=0x8c10d7d6u;r[0]>>=2;goto L8c10d7d8; /* shlr2 r0 */
L8c10d7d8: *stop=0x8c10d7d8u;r[0]>>=2;goto L8c10d7da; /* shlr2 r0 */
L8c10d7da: *stop=0x8c10d7dau;r[0]&=0xfu;goto L8c10d7dc; /* and #0xf,r0 */
L8c10d7dc: *stop=0x8c10d7dcu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8c10d7de; /* mov.l r0,@(0xc,r15) */
L8c10d7de: *stop=0x8c10d7deu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c10d7e0; /* stc SR,r0 */
L8c10d7e0: *stop=0x8c10d7e0u;r[0]&=r[3];goto L8c10d7e2; /* and r3,r0 */
L8c10d7e2: *stop=0x8c10d7e2u;r[0]|=0xf0u;goto L8c10d7e4; /* or #0xf0,r0 */
L8c10d7e4: *stop=0x8c10d7e4u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c10d7e6; /* ldc r0,SR */
L8c10d7e6: *stop=0x8c10d7e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d848u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d7e8; /* mov.l 0x8c10d848,r2 */
L8c10d7e8: target=r[2];*pr=0x8c10d7ecu;*stop=0x8c10d7eau;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d7ec; /* jsr @r2 */
L8c10d7ec: *stop=0x8c10d7ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d84cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c10d7ee; /* mov.l 0x8c10d84c,r12 */
L8c10d7ee: target=r[12];*pr=0x8c10d7f2u;*stop=0x8c10d7f0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d7f2; /* jsr @r12 */
L8c10d7f2: *stop=0x8c10d7f2u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d7f4; /* mov.l @(0x4,r15),r0 */
L8c10d7f4: *stop=0x8c10d7f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d850u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c10d7f6; /* mov.l 0x8c10d850,r11 */
L8c10d7f6: *stop=0x8c10d7f6u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d7f8; /* cmp/eq #0x1,r0 */
L8c10d7f8: take=(*sr&1u);*stop=0x8c10d7fau;r[14]=7u;if(take)goto L8c10d808;goto L8c10d7fc; /* bt/s 0x8c10d808 */
L8c10d7fc: *stop=0x8c10d7fcu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c10d7fe; /* cmp/eq #0x2,r0 */
L8c10d7fe: if((*sr&1u))goto L8c10d854;goto L8c10d800; /* bt 0x8c10d854 */
L8c10d800: *stop=0x8c10d800u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c10d802; /* cmp/eq #0x4,r0 */
L8c10d802: if((*sr&1u))goto L8c10d890;goto L8c10d804; /* bt 0x8c10d890 */
L8c10d804: *stop=0x8c10d806u;goto L8c10d8ce; /* bra 0x8c10d8ce */
L8c10d808: *stop=0x8c10d808u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d80a; /* mov.l @r15,r3 */
L8c10d80a: *stop=0x8c10d80au;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c10d80c; /* cmp/pl r13 */
L8c10d80c: *stop=0x8c10d80cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c10d80e; /* mov.l r3,@(0x4,r15) */
L8c10d80e: *stop=0x8c10d80eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10d810; /* mov.l r8,@r15 */
L8c10d810: take=!(*sr&1u);*stop=0x8c10d812u;r[8]=0u;if(take)goto L8c10d8ce;goto L8c10d814; /* bf/s 0x8c10d8ce */
L8c10d814: *stop=0x8c10d814u;r[2]=r[8];goto L8c10d816; /* mov r8,r2 */
L8c10d816: *stop=0x8c10d816u;*sr=(*sr&~1u)|!(r[14]&r[2]);goto L8c10d818; /* tst r14,r2 */
L8c10d818: if(!(*sr&1u))goto L8c10d822;goto L8c10d81a; /* bf 0x8c10d822 */
L8c10d81a: target=r[12];*pr=0x8c10d81eu;*stop=0x8c10d81cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d81e; /* jsr @r12 */
L8c10d81e: target=r[11];*pr=0x8c10d822u;*stop=0x8c10d820u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d822; /* jsr @r11 */
L8c10d822: *stop=0x8c10d822u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d824; /* mov.l @r15,r3 */
L8c10d824: *stop=0x8c10d824u;r[8]+=1u;goto L8c10d826; /* add #0x1,r8 */
L8c10d826: *stop=0x8c10d826u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d828; /* mov.l @(0x4,r15),r1 */
L8c10d828: *stop=0x8c10d828u;*sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[13]);goto L8c10d82a; /* cmp/ge r13,r8 */
L8c10d82a: *stop=0x8c10d82au;if(!valid_access(r[3],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;r[2]=loaded;}goto L8c10d82c; /* mov.b @r3,r2 */
L8c10d82c: *stop=0x8c10d82cu;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[2],1);if(io_status)return io_status;goto L8c10d82e; /* mov.b r2,@r1 */
L8c10d82e: *stop=0x8c10d82eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d830; /* mov.l @r15,r3 */
L8c10d830: *stop=0x8c10d830u;r[3]+=r[10];goto L8c10d832; /* add r10,r3 */
L8c10d832: *stop=0x8c10d832u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10d834; /* mov.l r3,@r15 */
L8c10d834: *stop=0x8c10d834u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d836; /* mov.l @(0x4,r15),r2 */
L8c10d836: *stop=0x8c10d836u;r[2]+=r[9];goto L8c10d838; /* add r9,r2 */
L8c10d838: take=!(*sr&1u);*stop=0x8c10d83au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;if(take)goto L8c10d814;goto L8c10d83c; /* bf/s 0x8c10d814 */
L8c10d83c: *stop=0x8c10d83eu;goto L8c10d8ce; /* bra 0x8c10d8ce */
L8c10d854: *stop=0x8c10d854u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d856; /* mov.l @r15,r2 */
L8c10d856: *stop=0x8c10d856u;*sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c10d858; /* shll r10 */
L8c10d858: *stop=0x8c10d858u;*sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c10d85a; /* shll r9 */
L8c10d85a: *stop=0x8c10d85au;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c10d85c; /* cmp/pl r13 */
L8c10d85c: *stop=0x8c10d85cu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c10d85e; /* mov.l r2,@(0x4,r15) */
L8c10d85e: *stop=0x8c10d85eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10d860; /* mov.l r8,@r15 */
L8c10d860: take=!(*sr&1u);*stop=0x8c10d862u;r[8]=0u;if(take)goto L8c10d8ce;goto L8c10d864; /* bf/s 0x8c10d8ce */
L8c10d864: *stop=0x8c10d864u;r[2]=r[8];goto L8c10d866; /* mov r8,r2 */
L8c10d866: *stop=0x8c10d866u;*sr=(*sr&~1u)|!(r[14]&r[2]);goto L8c10d868; /* tst r14,r2 */
L8c10d868: if(!(*sr&1u))goto L8c10d872;goto L8c10d86a; /* bf 0x8c10d872 */
L8c10d86a: target=r[12];*pr=0x8c10d86eu;*stop=0x8c10d86cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d86e; /* jsr @r12 */
L8c10d86e: target=r[11];*pr=0x8c10d872u;*stop=0x8c10d870u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d872; /* jsr @r11 */
L8c10d872: *stop=0x8c10d872u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d874; /* mov.l @r15,r3 */
L8c10d874: *stop=0x8c10d874u;r[8]+=1u;goto L8c10d876; /* add #0x1,r8 */
L8c10d876: *stop=0x8c10d876u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d878; /* mov.l @(0x4,r15),r1 */
L8c10d878: *stop=0x8c10d878u;*sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[13]);goto L8c10d87a; /* cmp/ge r13,r8 */
L8c10d87a: *stop=0x8c10d87au;if(!valid_access(r[3],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;r[2]=loaded;}goto L8c10d87c; /* mov.w @r3,r2 */
L8c10d87c: *stop=0x8c10d87cu;if(!valid_access(r[1],2u))return -3;wr(m,bus,&io_status,r[1],r[2],2);if(io_status)return io_status;goto L8c10d87e; /* mov.w r2,@r1 */
L8c10d87e: *stop=0x8c10d87eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d880; /* mov.l @r15,r3 */
L8c10d880: *stop=0x8c10d880u;r[3]+=r[10];goto L8c10d882; /* add r10,r3 */
L8c10d882: *stop=0x8c10d882u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10d884; /* mov.l r3,@r15 */
L8c10d884: *stop=0x8c10d884u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d886; /* mov.l @(0x4,r15),r2 */
L8c10d886: *stop=0x8c10d886u;r[2]+=r[9];goto L8c10d888; /* add r9,r2 */
L8c10d888: take=!(*sr&1u);*stop=0x8c10d88au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;if(take)goto L8c10d864;goto L8c10d88c; /* bf/s 0x8c10d864 */
L8c10d88c: *stop=0x8c10d88eu;goto L8c10d8ce; /* bra 0x8c10d8ce */
L8c10d890: *stop=0x8c10d890u;r[10]<<=2;goto L8c10d892; /* shll2 r10 */
L8c10d892: *stop=0x8c10d892u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d894; /* mov.l @r15,r2 */
L8c10d894: *stop=0x8c10d894u;*sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c10d896; /* cmp/pl r13 */
L8c10d896: *stop=0x8c10d896u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c10d898; /* mov.l r2,@(0x4,r15) */
L8c10d898: *stop=0x8c10d898u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c10d89a; /* mov.l r8,@r15 */
L8c10d89a: *stop=0x8c10d89au;r[8]=0u;goto L8c10d89c; /* mov #0x0,r8 */
L8c10d89c: *stop=0x8c10d89cu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[10],4);if(io_status)return io_status;goto L8c10d89e; /* mov.l r10,@(0x8,r15) */
L8c10d89e: *stop=0x8c10d89eu;r[10]=r[9];goto L8c10d8a0; /* mov r9,r10 */
L8c10d8a0: take=!(*sr&1u);*stop=0x8c10d8a2u;r[10]<<=2;if(take)goto L8c10d8ce;goto L8c10d8a4; /* bf/s 0x8c10d8ce */
L8c10d8a4: *stop=0x8c10d8a4u;r[2]=r[8];goto L8c10d8a6; /* mov r8,r2 */
L8c10d8a6: *stop=0x8c10d8a6u;*sr=(*sr&~1u)|!(r[14]&r[2]);goto L8c10d8a8; /* tst r14,r2 */
L8c10d8a8: if(!(*sr&1u))goto L8c10d8b2;goto L8c10d8aa; /* bf 0x8c10d8b2 */
L8c10d8aa: target=r[12];*pr=0x8c10d8aeu;*stop=0x8c10d8acu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d8ae; /* jsr @r12 */
L8c10d8ae: target=r[11];*pr=0x8c10d8b2u;*stop=0x8c10d8b0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d8b2; /* jsr @r11 */
L8c10d8b2: *stop=0x8c10d8b2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d8b4; /* mov.l @r15,r3 */
L8c10d8b4: *stop=0x8c10d8b4u;r[8]+=1u;goto L8c10d8b6; /* add #0x1,r8 */
L8c10d8b6: *stop=0x8c10d8b6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d8b8; /* mov.l @(0x4,r15),r1 */
L8c10d8b8: *stop=0x8c10d8b8u;*sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[13]);goto L8c10d8ba; /* cmp/ge r13,r8 */
L8c10d8ba: *stop=0x8c10d8bau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d8bc; /* mov.l @r3,r2 */
L8c10d8bc: *stop=0x8c10d8bcu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c10d8be; /* mov.l r2,@r1 */
L8c10d8be: *stop=0x8c10d8beu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c10d8c0; /* mov.l @r15,r1 */
L8c10d8c0: *stop=0x8c10d8c0u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d8c2; /* mov.l @(0x8,r15),r3 */
L8c10d8c2: *stop=0x8c10d8c2u;r[1]+=r[3];goto L8c10d8c4; /* add r3,r1 */
L8c10d8c4: *stop=0x8c10d8c4u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c10d8c6; /* mov.l r1,@r15 */
L8c10d8c6: *stop=0x8c10d8c6u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10d8c8; /* mov.l @(0x4,r15),r2 */
L8c10d8c8: *stop=0x8c10d8c8u;r[2]+=r[10];goto L8c10d8ca; /* add r10,r2 */
L8c10d8ca: take=!(*sr&1u);*stop=0x8c10d8ccu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;if(take)goto L8c10d8a4;goto L8c10d8ce; /* bf/s 0x8c10d8a4 */
L8c10d8ce: *stop=0x8c10d8ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d90cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d8d0; /* mov.l 0x8c10d90c,r3 */
L8c10d8d0: target=r[3];*pr=0x8c10d8d4u;*stop=0x8c10d8d2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10d8d4; /* jsr @r3 */
L8c10d8d4: *stop=0x8c10d8d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d910u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10d8d6; /* mov.l 0x8c10d910,r3 */
L8c10d8d6: *stop=0x8c10d8d6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d908u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c10d8d8; /* mov.w 0x8c10d908,r1 */
L8c10d8d8: *stop=0x8c10d8d8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d8da; /* mov.l @r3,r0 */
L8c10d8da: *stop=0x8c10d8dau;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d8dc; /* mov.l @(r0,r1),r0 */
L8c10d8dc: *stop=0x8c10d8dcu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10d8de; /* cmp/eq #0x1,r0 */
L8c10d8de: if(!(*sr&1u))goto L8c10d8f2;goto L8c10d8e0; /* bf 0x8c10d8f2 */
L8c10d8e0: *stop=0x8c10d8e0u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d8e2; /* mov.l @(0xc,r15),r0 */
L8c10d8e2: *stop=0x8c10d8e2u;status=sa_sh4_stc_sr(*sr,&r[2],0u);if(status)return status;goto L8c10d8e4; /* stc SR,r2 */
L8c10d8e4: *stop=0x8c10d8e4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c10d90au,2);if(io_status)return io_status;r[3]=loaded;}goto L8c10d8e6; /* mov.w 0x8c10d90a,r3 */
L8c10d8e6: *stop=0x8c10d8e6u;r[0]&=0xfu;goto L8c10d8e8; /* and #0xf,r0 */
L8c10d8e8: *stop=0x8c10d8e8u;r[0]<<=2;goto L8c10d8ea; /* shll2 r0 */
L8c10d8ea: *stop=0x8c10d8eau;r[2]&=r[3];goto L8c10d8ec; /* and r3,r2 */
L8c10d8ec: *stop=0x8c10d8ecu;r[0]<<=2;goto L8c10d8ee; /* shll2 r0 */
L8c10d8ee: *stop=0x8c10d8eeu;r[0]|=r[2];goto L8c10d8f0; /* or r2,r0 */
L8c10d8f0: *stop=0x8c10d8f0u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c10d8f2; /* ldc r0,SR */
L8c10d8f2: *stop=0x8c10d8f2u;r[0]=0u;goto L8c10d8f4; /* mov #0x0,r0 */
L8c10d8f4: *stop=0x8c10d8f4u;r[15]+=16u;goto L8c10d8f6; /* add #0x10,r15 */
L8c10d8f6: *stop=0x8c10d8f6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c10d8f8; /* lds.l @r15+,PR */
L8c10d8f8: *stop=0x8c10d8f8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c10d8fa; /* mov.l @r15+,r8 */
L8c10d8fa: *stop=0x8c10d8fau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c10d8fc; /* mov.l @r15+,r9 */
L8c10d8fc: *stop=0x8c10d8fcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c10d8fe; /* mov.l @r15+,r10 */
L8c10d8fe: *stop=0x8c10d8feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c10d900; /* mov.l @r15+,r11 */
L8c10d900: *stop=0x8c10d900u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c10d902; /* mov.l @r15+,r12 */
L8c10d902: *stop=0x8c10d902u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c10d904; /* mov.l @r15+,r13 */
L8c10d904: *stop=0x8c10d904u;if(*pr!=entry_pr)return -3;*stop=0x8c10d906u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

