/* Original complete actor collision allocation,attachment and extent family.
 * Exact original instruction order and existing numerical/bus ABI.
 * No upstream implementation code imported.
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
int sa_sh4_sqrt(uint32_t,uint32_t*,uint32_t*);
int sa_movie_channel_io_return_allowed(void *,uint32_t);
static int sa_player_collision_attach_run_8c02f4b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c02f4d0u)goto L8c02f4d0;if(resume_pc==0x8c02f4eau)goto L8c02f4ea;if(resume_pc==0x8c02f4f6u)goto L8c02f4f6;if(resume_pc==0x8c02f50cu)goto L8c02f50c;goto L8c02f4b0;
L8c02f4b0: *stop=0x8c02f4b0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c02f4b2; /* mov.l r14,@-r15 */
L8c02f4b2: *stop=0x8c02f4b2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c02f4b4; /* mov.l r13,@-r15 */
L8c02f4b4: *stop=0x8c02f4b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c02f4b6; /* mov.l r12,@-r15 */
L8c02f4b6: *stop=0x8c02f4b6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c02f4b8; /* mov.l r11,@-r15 */
L8c02f4b8: *stop=0x8c02f4b8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c02f4ba; /* sts.l PR,@-r15 */
L8c02f4ba: *stop=0x8c02f4bau;r[15]+=4294967284u;goto L8c02f4bc; /* add #-0xc,r15 */
L8c02f4bc: *stop=0x8c02f4bcu;r[0]=r[6];goto L8c02f4be; /* mov r6,r0 */
L8c02f4be: *stop=0x8c02f4beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02f5b4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02f4c0; /* mov.l 0x8c02f5b4,r3 */
L8c02f4c0: *stop=0x8c02f4c0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c02f4c2; /* mov.l r4,@r15 */
L8c02f4c2: *stop=0x8c02f4c2u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[5],4);if(io_status)return io_status;goto L8c02f4c4; /* mov.l r5,@(0x8,r15) */
L8c02f4c4: *stop=0x8c02f4c4u;r[5]=28u;goto L8c02f4c6; /* mov #0x1c,r5 */
L8c02f4c6: *stop=0x8c02f4c6u;if(!valid_access(r[15]+4u,1u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],1);if(io_status)return io_status;goto L8c02f4c8; /* mov.b r0,@(0x4,r15) */
L8c02f4c8: *stop=0x8c02f4c8u;r[0]=32u;goto L8c02f4ca; /* mov #0x20,r0 */
L8c02f4ca: *stop=0x8c02f4cau;if(!valid_access(r[0]+r[15],1u))return -3;r[13]=sx(m,bus,&io_status,r[15]+r[0],1);if(io_status)return io_status;goto L8c02f4cc; /* mov.b @(r0,r15),r13 */
L8c02f4cc: target=r[3];*pr=0x8c02f4d0u;*stop=0x8c02f4ceu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02f4d0; /* jsr @r3 */
L8c02f4d0: *stop=0x8c02f4d0u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c02f4d2; /* tst r0,r0 */
L8c02f4d2: take=!(*sr&1u);*stop=0x8c02f4d4u;r[14]=r[0];if(take)goto L8c02f4ee;goto L8c02f4d6; /* bf/s 0x8c02f4ee */
L8c02f4d6: *stop=0x8c02f4d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02f5c0u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c02f4d8; /* mov.l 0x8c02f5c0,r13 */
L8c02f4d8: *stop=0x8c02f4d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02f5c4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c02f4da; /* mov.l 0x8c02f5c4,r14 */
L8c02f4da: *stop=0x8c02f4dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c02f5b8u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c02f4dc; /* mov.l 0x8c02f5b8,r11 */
L8c02f4dc: *stop=0x8c02f4dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02f5bcu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c02f4de; /* mov.l 0x8c02f5bc,r12 */
L8c02f4de: *stop=0x8c02f4deu;if(!valid_access(r[13],1u))return -3;r[3]=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;goto L8c02f4e0; /* mov.b @r13,r3 */
L8c02f4e0: *stop=0x8c02f4e0u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c02f4e2; /* cmp/pz r3 */
L8c02f4e2: if(!(*sr&1u))goto L8c02f4de;goto L8c02f4e4; /* bf 0x8c02f4de */
L8c02f4e4: *stop=0x8c02f4e4u;r[5]=r[12];goto L8c02f4e6; /* mov r12,r5 */
L8c02f4e6: target=r[11];*pr=0x8c02f4eau;*stop=0x8c02f4e8u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02f4ea; /* jsr @r11 */
L8c02f4ea: *stop=0x8c02f4ecu;goto L8c02f4de; /* bra 0x8c02f4de */
L8c02f4ee: *stop=0x8c02f4eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02f5b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02f4f0; /* mov.l 0x8c02f5b4,r2 */
L8c02f4f0: *stop=0x8c02f4f0u;r[5]=52u;goto L8c02f4f2; /* mov #0x34,r5 */
L8c02f4f2: target=r[2];*pr=0x8c02f4f6u;*stop=0x8c02f4f4u;r[4]=r[13]&255u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02f4f6; /* jsr @r2 */
L8c02f4f6: *stop=0x8c02f4f6u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c02f4f8; /* tst r0,r0 */
L8c02f4f8: take=(*sr&1u);*stop=0x8c02f4fau;r[12]=r[0];if(take)goto L8c02f576;goto L8c02f4fc; /* bt/s 0x8c02f576 */
L8c02f4fc: *stop=0x8c02f4fcu;r[3]=52u;goto L8c02f4fe; /* mov #0x34,r3 */
L8c02f4fe: *stop=0x8c02f4feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c02f5c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c02f500; /* mov.l 0x8c02f5c8,r2 */
L8c02f500: *stop=0x8c02f500u;r[6]=r[13]&255u;goto L8c02f502; /* extu.b r13,r6 */
L8c02f502: *stop=0x8c02f502u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c02f504; /* mov.l @(0x8,r15),r5 */
L8c02f504: *stop=0x8c02f504u;*macl=(r[3]&65535u)*(r[6]&65535u);goto L8c02f506; /* mulu.w r3,r6 */
L8c02f506: *stop=0x8c02f506u;r[6]=*macl;goto L8c02f508; /* sts MACL,r6 */
L8c02f508: target=r[2];*pr=0x8c02f50cu;*stop=0x8c02f50au;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02f50c; /* jsr @r2 */
L8c02f50c: *stop=0x8c02f50cu;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[12],4);if(io_status)return io_status;goto L8c02f50e; /* mov.l r12,@(0xc,r14) */
L8c02f50e: *stop=0x8c02f50eu;if(!valid_access(r[14]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c02f510; /* mov.w @(0x2,r14),r0 */
L8c02f510: *stop=0x8c02f510u;r[0]|=0x20u;goto L8c02f512; /* or #0x20,r0 */
L8c02f512: *stop=0x8c02f512u;if(!valid_access(r[14]+2u,2u))return -3;wr(m,bus,&io_status,r[14]+0x2u,r[0],2);if(io_status)return io_status;goto L8c02f514; /* mov.w r0,@(0x2,r14) */
L8c02f514: *stop=0x8c02f514u;if(!valid_access(r[15]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x4u,1);if(io_status)return io_status;goto L8c02f516; /* mov.b @(0x4,r15),r0 */
L8c02f516: *stop=0x8c02f516u;r[0]=r[0]&255u;goto L8c02f518; /* extu.b r0,r0 */
L8c02f518: *stop=0x8c02f518u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[0],2);if(io_status)return io_status;goto L8c02f51a; /* mov.w r0,@r14 */
L8c02f51a: *stop=0x8c02f51au;if(!valid_access(r[15]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x4u,1);if(io_status)return io_status;goto L8c02f51c; /* mov.b @(0x4,r15),r0 */
L8c02f51c: *stop=0x8c02f51cu;r[12]=r[0];goto L8c02f51e; /* mov r0,r12 */
L8c02f51e: *stop=0x8c02f51eu;r[12]=r[12]&255u;goto L8c02f520; /* extu.b r12,r12 */
L8c02f520: *stop=0x8c02f520u;r[0]=r[12];goto L8c02f522; /* mov r12,r0 */
L8c02f522: *stop=0x8c02f522u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c02f524; /* cmp/eq #0x2,r0 */
L8c02f524: if((*sr&1u))goto L8c02f532;goto L8c02f526; /* bt 0x8c02f532 */
L8c02f526: *stop=0x8c02f526u;r[0]=r[12];goto L8c02f528; /* mov r12,r0 */
L8c02f528: *stop=0x8c02f528u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c02f52a; /* cmp/eq #0x3,r0 */
L8c02f52a: if((*sr&1u))goto L8c02f532;goto L8c02f52c; /* bt 0x8c02f532 */
L8c02f52c: *stop=0x8c02f52cu;r[0]=r[12];goto L8c02f52e; /* mov r12,r0 */
L8c02f52e: *stop=0x8c02f52eu;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c02f530; /* cmp/eq #0x7,r0 */
L8c02f530: if(!(*sr&1u))goto L8c02f53c;goto L8c02f532; /* bf 0x8c02f53c */
L8c02f532: *stop=0x8c02f532u;if(!valid_access(r[14]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c02f534; /* mov.w @(0x2,r14),r0 */
L8c02f534: *stop=0x8c02f534u;r[3]=8u;goto L8c02f536; /* mov #0x8,r3 */
L8c02f536: *stop=0x8c02f536u;r[3]<<=8;goto L8c02f538; /* shll8 r3 */
L8c02f538: *stop=0x8c02f538u;r[0]|=r[3];goto L8c02f53a; /* or r3,r0 */
L8c02f53a: *stop=0x8c02f53au;if(!valid_access(r[14]+2u,2u))return -3;wr(m,bus,&io_status,r[14]+0x2u,r[0],2);if(io_status)return io_status;goto L8c02f53c; /* mov.w r0,@(0x2,r14) */
L8c02f53c: *stop=0x8c02f53cu;r[0]=r[13]&255u;goto L8c02f53e; /* extu.b r13,r0 */
L8c02f53e: *stop=0x8c02f53eu;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c02f540; /* mov.w r0,@(0x4,r14) */
L8c02f540: *stop=0x8c02f540u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c02f542; /* tst r12,r12 */
L8c02f542: *stop=0x8c02f542u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c02f544; /* mov.l @r15,r3 */
L8c02f544: *stop=0x8c02f544u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[3],4);if(io_status)return io_status;goto L8c02f546; /* mov.l r3,@(0x10,r14) */
L8c02f546: *stop=0x8c02f546u;if(!valid_access(r[14]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c02f548; /* mov.w @(0x2,r14),r0 */
L8c02f548: *stop=0x8c02f548u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02f5ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c02f54a; /* mov.l 0x8c02f5cc,r3 */
L8c02f54a: *stop=0x8c02f54au;r[0]&=r[3];goto L8c02f54c; /* and r3,r0 */
L8c02f54c: *stop=0x8c02f54cu;if(!valid_access(r[14]+2u,2u))return -3;wr(m,bus,&io_status,r[14]+0x2u,r[0],2);if(io_status)return io_status;goto L8c02f54e; /* mov.w r0,@(0x2,r14) */
L8c02f54e: *stop=0x8c02f54eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c02f550; /* mov.l @r15,r2 */
L8c02f550: *stop=0x8c02f550u;if(!valid_access(r[2]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c02f552; /* mov.l @(0x20,r2),r1 */
L8c02f552: take=(*sr&1u);*stop=0x8c02f554u;if(!valid_access(r[1]+56u,4u))return -3;wr(m,bus,&io_status,r[1]+0x38u,r[14],4);if(io_status)return io_status;if(take)goto L8c02f55c;goto L8c02f556; /* bt/s 0x8c02f55c */
L8c02f556: *stop=0x8c02f556u;r[0]=r[12];goto L8c02f558; /* mov r12,r0 */
L8c02f558: *stop=0x8c02f558u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c02f55a; /* cmp/eq #0x1,r0 */
L8c02f55a: if(!(*sr&1u))goto L8c02f566;goto L8c02f55c; /* bf 0x8c02f566 */
L8c02f55c: *stop=0x8c02f55cu;r[0]=0x8c02f5b0u;goto L8c02f55e; /* mova 0x8c02f5b0,r0 */
L8c02f55e: *stop=0x8c02f55eu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c02f560; /* fmov.s @r0,fr3 */
L8c02f560: *stop=0x8c02f560u;r[0]=8u;goto L8c02f562; /* mov #0x8,r0 */
L8c02f562: *stop=0x8c02f564u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c02f576; /* bra 0x8c02f576 */
L8c02f566: *stop=0x8c02f566u;r[4]=r[14];goto L8c02f568; /* mov r14,r4 */
L8c02f568: *stop=0x8c02f568u;r[15]+=12u;goto L8c02f56a; /* add #0xc,r15 */
L8c02f56a: *stop=0x8c02f56au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c02f56c; /* lds.l @r15+,PR */
L8c02f56c: *stop=0x8c02f56cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c02f56e; /* mov.l @r15+,r11 */
L8c02f56e: *stop=0x8c02f56eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c02f570; /* mov.l @r15+,r12 */
L8c02f570: *stop=0x8c02f570u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c02f572; /* mov.l @r15+,r13 */
L8c02f572: *stop=0x8c02f574u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,0x8c02f400u,m,f,sr,r,pr); /* bra 0x8c02f400 */
L8c02f576: *stop=0x8c02f576u;r[15]+=12u;goto L8c02f578; /* add #0xc,r15 */
L8c02f578: *stop=0x8c02f578u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c02f57a; /* lds.l @r15+,PR */
L8c02f57a: *stop=0x8c02f57au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c02f57c; /* mov.l @r15+,r11 */
L8c02f57c: *stop=0x8c02f57cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c02f57e; /* mov.l @r15+,r12 */
L8c02f57e: *stop=0x8c02f57eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c02f580; /* mov.l @r15+,r13 */
L8c02f580: *stop=0x8c02f580u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c02f582u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_collision_attach_8c02f4b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_collision_attach_run_8c02f4b0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_collision_attach_resume_8c02f4d0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_collision_attach_run_8c02f4b0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02f4d0u);}
SA_STAGE_EXPORT int sa_player_collision_attach_resume_8c02f4ea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_collision_attach_run_8c02f4b0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02f4eau);}
SA_STAGE_EXPORT int sa_player_collision_attach_resume_8c02f4f6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_collision_attach_run_8c02f4b0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02f4f6u);}
SA_STAGE_EXPORT int sa_player_collision_attach_resume_8c02f50c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_collision_attach_run_8c02f4b0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02f50cu);}

static int sa_player_collision_attach_run_8c02f400(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c02f44cu)goto L8c02f44c;if(resume_pc==0x8c02f45au)goto L8c02f45a;if(resume_pc==0x8c02f46cu)goto L8c02f46c;goto L8c02f400;
L8c02f400: *stop=0x8c02f400u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c02f402; /* mov.l r14,@-r15 */
L8c02f402: *stop=0x8c02f402u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c02f404; /* mov.l r13,@-r15 */
L8c02f404: *stop=0x8c02f404u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c02f406; /* mov.l r12,@-r15 */
L8c02f406: *stop=0x8c02f406u;r[12]=r[4];goto L8c02f408; /* mov r4,r12 */
L8c02f408: *stop=0x8c02f408u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c02f40a; /* mov.l r11,@-r15 */
L8c02f40a: *stop=0x8c02f40au;r[11]=52u;goto L8c02f40c; /* mov #0x34,r11 */
L8c02f40c: *stop=0x8c02f40cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c02f40e; /* mov.l r10,@-r15 */
L8c02f40e: *stop=0x8c02f40eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c02f410; /* mov.l r9,@-r15 */
L8c02f410: *stop=0x8c02f410u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c02f412; /* fmov.s fr15,@-r15 */
L8c02f412: *stop=0x8c02f412u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c02f414; /* fmov.s fr14,@-r15 */
L8c02f414: *stop=0x8c02f414u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c02f416; /* fmov.s fr13,@-r15 */
L8c02f416: *stop=0x8c02f416u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c02f418; /* fmov.s fr12,@-r15 */
L8c02f418: *stop=0x8c02f418u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c02f41a; /* sts.l PR,@-r15 */
L8c02f41a: *stop=0x8c02f41au;if(!valid_access(r[12]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x4u,2);if(io_status)return io_status;goto L8c02f41c; /* mov.w @(0x4,r12),r0 */
L8c02f41c: *stop=0x8c02f41cu;if(!valid_access(r[12]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c02f41e; /* mov.l @(0xc,r12),r4 */
L8c02f41e: *stop=0x8c02f41eu;*macl=(r[11]&65535u)*(r[0]&65535u);goto L8c02f420; /* mulu.w r11,r0 */
L8c02f420: *stop=0x8c02f420u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02f5a0u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c02f422; /* mov.l 0x8c02f5a0,r9 */
L8c02f422: *stop=0x8c02f422u;r[14]=r[4];goto L8c02f424; /* mov r4,r14 */
L8c02f424: *stop=0x8c02f424u;{uint32_t loaded=rd(m,bus,&io_status,0x8c02f5a4u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c02f426; /* mov.l 0x8c02f5a4,r13 */
L8c02f426: *stop=0x8c02f426u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][13]=0;goto L8c02f428; /* fldi0 fr13 */
L8c02f428: *stop=0x8c02f428u;r[11]=*macl;goto L8c02f42a; /* sts MACL,r11 */
L8c02f42a: *stop=0x8c02f42au;r[11]+=r[4];goto L8c02f42c; /* add r4,r11 */
L8c02f42c: *stop=0x8c02f42cu;*sr=(*sr&~1u)|(r[14]>=r[11]);goto L8c02f42e; /* cmp/hs r11,r14 */
L8c02f42e: take=(*sr&1u);*stop=0x8c02f430u;r[10]=15u;if(take)goto L8c02f47c;goto L8c02f432; /* bt/s 0x8c02f47c */
L8c02f432: *stop=0x8c02f432u;r[0]=20u;goto L8c02f434; /* mov #0x14,r0 */
L8c02f434: *stop=0x8c02f434u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]+r[14]);if(io_status)return io_status;goto L8c02f436; /* fmov.s @(r0,r14),fr14 */
L8c02f436: *stop=0x8c02f436u;r[0]=24u;goto L8c02f438; /* mov #0x18,r0 */
L8c02f438: *stop=0x8c02f438u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c02f43a; /* fmov.s @(r0,r14),fr15 */
L8c02f43a: *stop=0x8c02f43au;r[0]=28u;goto L8c02f43c; /* mov #0x1c,r0 */
L8c02f43c: *stop=0x8c02f43cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]+r[14]);if(io_status)return io_status;goto L8c02f43e; /* fmov.s @(r0,r14),fr12 */
L8c02f43e: *stop=0x8c02f43eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[0]=loaded;}goto L8c02f440; /* mov.l @r14,r0 */
L8c02f440: *stop=0x8c02f440u;r[0]&=r[10];goto L8c02f442; /* and r10,r0 */
L8c02f442: *stop=0x8c02f442u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c02f444; /* cmp/eq #0x3,r0 */
L8c02f444: if(!(*sr&1u))goto L8c02f454;goto L8c02f446; /* bf 0x8c02f454 */
L8c02f446: *stop=0x8c02f446u;r[4]=r[14];goto L8c02f448; /* mov r14,r4 */
L8c02f448: target=r[13];*pr=0x8c02f44cu;*stop=0x8c02f44au;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02f44c; /* jsr @r13 */
L8c02f44c: *stop=0x8c02f44cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[14],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c02f44e; /* fadd fr14,fr0 */
L8c02f44e: *stop=0x8c02f44eu;fcopy(f,4u,0u);goto L8c02f450; /* fmov fr0,fr4 */
L8c02f450: *stop=0x8c02f452u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[15],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c02f470; /* bra 0x8c02f470 */
L8c02f454: *stop=0x8c02f454u;r[4]=r[14];goto L8c02f456; /* mov r14,r4 */
L8c02f456: target=r[13];*pr=0x8c02f45au;*stop=0x8c02f458u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02f45a; /* jsr @r13 */
L8c02f45a: *stop=0x8c02f45au;fcopy(f,3u,15u);goto L8c02f45c; /* fmov fr15,fr3 */
L8c02f45c: *stop=0x8c02f45cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c02f45e; /* fmul fr15,fr3 */
L8c02f45e: *stop=0x8c02f45eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c02f460; /* fmov.s fr0,@-r15 */
L8c02f460: *stop=0x8c02f460u;fcopy(f,0u,14u);goto L8c02f462; /* fmov fr14,fr0 */
L8c02f462: *stop=0x8c02f462u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[14],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c02f464; /* fmac fr0,fr14,fr3 */
L8c02f464: *stop=0x8c02f464u;fcopy(f,0u,12u);goto L8c02f466; /* fmov fr12,fr0 */
L8c02f466: *stop=0x8c02f466u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[12],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c02f468; /* fmac fr0,fr12,fr3 */
L8c02f468: target=r[9];*pr=0x8c02f46cu;*stop=0x8c02f46au;fcopy(f,4u,3u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c02f46c; /* jsr @r9 */
L8c02f46c: *stop=0x8c02f46cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02f46e; /* fmov.s @r15+,fr4 */
L8c02f46e: *stop=0x8c02f46eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[0],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c02f470; /* fadd fr0,fr4 */
L8c02f470: *stop=0x8c02f470u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][13],&f->fpscr,sr,1);if(status)return status;goto L8c02f472; /* fcmp/gt fr13,fr4 */
L8c02f472: take=!(*sr&1u);*stop=0x8c02f474u;r[14]+=52u;if(take)goto L8c02f478;goto L8c02f476; /* bf/s 0x8c02f478 */
L8c02f476: *stop=0x8c02f476u;fcopy(f,13u,4u);goto L8c02f478; /* fmov fr4,fr13 */
L8c02f478: *stop=0x8c02f478u;*sr=(*sr&~1u)|(r[14]>=r[11]);goto L8c02f47a; /* cmp/hs r11,r14 */
L8c02f47a: if(!(*sr&1u))goto L8c02f432;goto L8c02f47c; /* bf 0x8c02f432 */
L8c02f47c: *stop=0x8c02f47cu;r[0]=0x8c02f5a8u;goto L8c02f47e; /* mova 0x8c02f5a8,r0 */
L8c02f47e: *stop=0x8c02f47eu;fcopy(f,2u,13u);goto L8c02f480; /* fmov fr13,fr2 */
L8c02f480: *stop=0x8c02f480u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c02f482; /* fmov.s @r0,fr3 */
L8c02f482: *stop=0x8c02f482u;r[0]=8u;goto L8c02f484; /* mov #0x8,r0 */
L8c02f484: *stop=0x8c02f484u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c02f486; /* fmul fr3,fr2 */
L8c02f486: *stop=0x8c02f486u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c02f488; /* fmov fr2,@(r0,r12) */
L8c02f488: *stop=0x8c02f488u;r[0]=0x8c02f5acu;goto L8c02f48a; /* mova 0x8c02f5ac,r0 */
L8c02f48a: *stop=0x8c02f48au;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;goto L8c02f48c; /* fmov.s @r0,fr2 */
L8c02f48c: *stop=0x8c02f48cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][13],&f->fpscr,sr,1);if(status)return status;goto L8c02f48e; /* fcmp/gt fr13,fr2 */
L8c02f48e: if(!(*sr&1u))goto L8c02f498;goto L8c02f490; /* bf 0x8c02f498 */
L8c02f490: *stop=0x8c02f490u;r[0]=0x8c02f5b0u;goto L8c02f492; /* mova 0x8c02f5b0,r0 */
L8c02f492: *stop=0x8c02f492u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;goto L8c02f494; /* fmov.s @r0,fr1 */
L8c02f494: *stop=0x8c02f494u;r[0]=8u;goto L8c02f496; /* mov #0x8,r0 */
L8c02f496: *stop=0x8c02f496u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c02f498; /* fmov fr1,@(r0,r12) */
L8c02f498: *stop=0x8c02f498u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c02f49a; /* lds.l @r15+,PR */
L8c02f49a: *stop=0x8c02f49au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02f49c; /* fmov.s @r15+,fr12 */
L8c02f49c: *stop=0x8c02f49cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02f49e; /* fmov.s @r15+,fr13 */
L8c02f49e: *stop=0x8c02f49eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02f4a0; /* fmov.s @r15+,fr14 */
L8c02f4a0: *stop=0x8c02f4a0u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02f4a2; /* fmov.s @r15+,fr15 */
L8c02f4a2: *stop=0x8c02f4a2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c02f4a4; /* mov.l @r15+,r9 */
L8c02f4a4: *stop=0x8c02f4a4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c02f4a6; /* mov.l @r15+,r10 */
L8c02f4a6: *stop=0x8c02f4a6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c02f4a8; /* mov.l @r15+,r11 */
L8c02f4a8: *stop=0x8c02f4a8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c02f4aa; /* mov.l @r15+,r12 */
L8c02f4aa: *stop=0x8c02f4aau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c02f4ac; /* mov.l @r15+,r13 */
L8c02f4ac: *stop=0x8c02f4acu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c02f4aeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_collision_attach_8c02f400(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_collision_attach_run_8c02f400(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_collision_attach_resume_8c02f44c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_collision_attach_run_8c02f400(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02f44cu);}
SA_STAGE_EXPORT int sa_player_collision_attach_resume_8c02f45a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_collision_attach_run_8c02f400(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02f45au);}
SA_STAGE_EXPORT int sa_player_collision_attach_resume_8c02f46c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_collision_attach_run_8c02f400(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c02f46cu);}

SA_STAGE_EXPORT int sa_player_collision_attach_8c604500(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c604500;
L8c604500: *stop=0x8c604500u;if(!valid_access(r[5],1u))return -3;r[0]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[5]+=1;goto L8c604502; /* mov.b @r5+,r0 */
L8c604502: *stop=0x8c604502u;r[6]--;*sr=(*sr&~1u)|(!r[6]);goto L8c604504; /* dt r6 */
L8c604504: *stop=0x8c604504u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[0],1);if(io_status)return io_status;goto L8c604506; /* mov.b r0,@r4 */
L8c604506: take=!(*sr&1u);*stop=0x8c604508u;r[4]+=1u;if(take)goto L8c604500;goto L8c60450a; /* bf/s 0x8c604500 */
L8c60450a: *stop=0x8c60450au;if(*pr!=entry_pr)return -3;*stop=0x8c60450cu;return 0; /* rts */
}
