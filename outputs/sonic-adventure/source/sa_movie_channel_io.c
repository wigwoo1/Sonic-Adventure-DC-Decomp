/* Original header/readiness and canonical record backend; resumptions explicit.
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
int sa_movie_channel_io_return_allowed(void *,uint32_t);
static int sa_movie_channel_io_run_8c6608a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c6608c6u)goto L8c6608c6;if(resume_pc==0x8c6608ceu)goto L8c6608ce;if(resume_pc==0x8c6608eeu)goto L8c6608ee;if(resume_pc==0x8c6608f4u)goto L8c6608f4;if(resume_pc==0x8c6608fcu)goto L8c6608fc;if(resume_pc==0x8c660906u)goto L8c660906;if(resume_pc==0x8c660912u)goto L8c660912;if(resume_pc==0x8c66091au)goto L8c66091a;if(resume_pc==0x8c660922u)goto L8c660922;if(resume_pc==0x8c66092au)goto L8c66092a;if(resume_pc==0x8c660932u)goto L8c660932;if(resume_pc==0x8c660984u)goto L8c660984;if(resume_pc==0x8c660992u)goto L8c660992;if(resume_pc==0x8c6609a0u)goto L8c6609a0;if(resume_pc==0x8c6609aeu)goto L8c6609ae;if(resume_pc==0x8c6609bcu)goto L8c6609bc;if(resume_pc==0x8c6609cau)goto L8c6609ca;if(resume_pc==0x8c6609d8u)goto L8c6609d8;if(resume_pc==0x8c6609e6u)goto L8c6609e6;if(resume_pc==0x8c6609f4u)goto L8c6609f4;if(resume_pc==0x8c660a02u)goto L8c660a02;if(resume_pc==0x8c660a20u)goto L8c660a20;if(resume_pc==0x8c660a2au)goto L8c660a2a;if(resume_pc==0x8c660a4au)goto L8c660a4a;if(resume_pc==0x8c660a5au)goto L8c660a5a;if(resume_pc==0x8c660a68u)goto L8c660a68;goto L8c6608a4;
L8c6608a4: *stop=0x8c6608a4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6608a6; /* mov.l r14,@-r15 */
L8c6608a6: *stop=0x8c6608a6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6608a8; /* mov.l r13,@-r15 */
L8c6608a8: *stop=0x8c6608a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6608aa; /* mov.l r12,@-r15 */
L8c6608aa: *stop=0x8c6608aau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6608ac; /* mov.l r11,@-r15 */
L8c6608ac: *stop=0x8c6608acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6608ae; /* mov.l r10,@-r15 */
L8c6608ae: *stop=0x8c6608aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6608b0; /* mov.l r9,@-r15 */
L8c6608b0: *stop=0x8c6608b0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6608b2; /* mov.l r8,@-r15 */
L8c6608b2: *stop=0x8c6608b2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6608b4; /* sts.l PR,@-r15 */
L8c6608b4: *stop=0x8c6608b4u;r[15]+=4294967280u;goto L8c6608b6; /* add #-0x10,r15 */
L8c6608b6: *stop=0x8c6608b6u;r[13]=r[4];goto L8c6608b8; /* mov r4,r13 */
L8c6608b8: *stop=0x8c6608b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660958u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6608ba; /* mov.l 0x8c660958,r0 */
L8c6608ba: *stop=0x8c6608bau;r[13]<<=2;goto L8c6608bc; /* shll2 r13 */
L8c6608bc: *stop=0x8c6608bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66095cu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c6608be; /* mov.l 0x8c66095c,r10 */
L8c6608be: *stop=0x8c6608beu;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[13]=loaded;}goto L8c6608c0; /* mov.l @(r0,r13),r13 */
L8c6608c0: *stop=0x8c6608c0u;r[14]=r[5];goto L8c6608c2; /* mov r5,r14 */
L8c6608c2: target=r[10];*pr=0x8c6608c6u;*stop=0x8c6608c4u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6608c6; /* jsr @r10 */
L8c6608c6: *stop=0x8c6608c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660960u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6608c8; /* mov.l 0x8c660960,r3 */
L8c6608c8: *stop=0x8c6608c8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c6608ca; /* mov.l r0,@r15 */
L8c6608ca: target=r[3];*pr=0x8c6608ceu;*stop=0x8c6608ccu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6608ce; /* jsr @r3 */
L8c6608ce: *stop=0x8c6608ceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6608d0; /* mov.l @r15,r2 */
L8c6608d0: *stop=0x8c6608d0u;r[4]=r[0];goto L8c6608d2; /* mov r0,r4 */
L8c6608d2: *stop=0x8c6608d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660964u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c6608d4; /* mov.l 0x8c660964,r11 */
L8c6608d4: *stop=0x8c6608d4u;if(!valid_access(r[2],1u))return -3;r[3]=sx(m,bus,&io_status,r[2],1);if(io_status)return io_status;goto L8c6608d6; /* mov.b @r2,r3 */
L8c6608d6: *stop=0x8c6608d6u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6608d8; /* tst r3,r3 */
L8c6608d8: if(!(*sr&1u))goto L8c6608de;goto L8c6608da; /* bf 0x8c6608de */
L8c6608da: *stop=0x8c6608dcu;goto L8c660a64; /* bra 0x8c660a64 */
L8c6608de: *stop=0x8c6608deu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6608e0; /* mov.l @r4,r0 */
L8c6608e0: *stop=0x8c6608e0u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c6608e2; /* tst #0x2,r0 */
L8c6608e2: if(!(*sr&1u))goto L8c6608e8;goto L8c6608e4; /* bf 0x8c6608e8 */
L8c6608e4: *stop=0x8c6608e6u;goto L8c660a64; /* bra 0x8c660a64 */
L8c6608e8: *stop=0x8c6608e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660968u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6608ea; /* mov.l 0x8c660968,r2 */
L8c6608ea: target=r[2];*pr=0x8c6608eeu;*stop=0x8c6608ecu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6608ee; /* jsr @r2 */
L8c6608ee: *stop=0x8c6608eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66096cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c6608f0; /* mov.l 0x8c66096c,r12 */
L8c6608f0: target=r[10];*pr=0x8c6608f4u;*stop=0x8c6608f2u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6608f4; /* jsr @r10 */
L8c6608f4: *stop=0x8c6608f4u;r[5]=2u;goto L8c6608f6; /* mov #0x2,r5 */
L8c6608f6: *stop=0x8c6608f6u;r[6]=0u;goto L8c6608f8; /* mov #0x0,r6 */
L8c6608f8: target=r[12];*pr=0x8c6608fcu;*stop=0x8c6608fau;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6608fc; /* jsr @r12 */
L8c6608fc: *stop=0x8c6608fcu;r[9]=r[0];goto L8c6608fe; /* mov r0,r9 */
L8c6608fe: *stop=0x8c6608feu;*sr=(*sr&~1u)|((int32_t)r[9]>=0);goto L8c660900; /* cmp/pz r9 */
L8c660900: if((*sr&1u))goto L8c66090a;goto L8c660902; /* bt 0x8c66090a */
L8c660902: target=r[11];*pr=0x8c660906u;*stop=0x8c660904u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660906; /* jsr @r11 */
L8c660906: *stop=0x8c660908u;goto L8c6608f0; /* bra 0x8c6608f0 */
L8c66090a: *stop=0x8c66090au;{uint32_t loaded=rd(m,bus,&io_status,0x8c660970u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c66090c; /* mov.l 0x8c660970,r12 */
L8c66090c: *stop=0x8c66090cu;r[5]=r[9];goto L8c66090e; /* mov r9,r5 */
L8c66090e: target=r[12];*pr=0x8c660912u;*stop=0x8c660910u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660912; /* jsr @r12 */
L8c660912: *stop=0x8c660912u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660914; /* tst r0,r0 */
L8c660914: if(!(*sr&1u))goto L8c66091e;goto L8c660916; /* bf 0x8c66091e */
L8c660916: target=r[11];*pr=0x8c66091au;*stop=0x8c660918u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66091a; /* jsr @r11 */
L8c66091a: *stop=0x8c66091cu;goto L8c66090c; /* bra 0x8c66090c */
L8c66091e: target=r[10];*pr=0x8c660922u;*stop=0x8c660920u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660922; /* jsr @r10 */
L8c660922: *stop=0x8c660922u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660974u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660924; /* mov.l 0x8c660974,r3 */
L8c660924: *stop=0x8c660924u;r[8]=r[0];goto L8c660926; /* mov r0,r8 */
L8c660926: target=r[3];*pr=0x8c66092au;*stop=0x8c660928u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66092a; /* jsr @r3 */
L8c66092a: *stop=0x8c66092au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c66092c; /* tst r0,r0 */
L8c66092c: if((*sr&1u))goto L8c660938;goto L8c66092e; /* bt 0x8c660938 */
L8c66092e: target=r[11];*pr=0x8c660932u;*stop=0x8c660930u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660932; /* jsr @r11 */
L8c660932: *stop=0x8c660932u;r[0]=sx(m,bus,&io_status,0x8c66094eu,2);if(io_status)return io_status;goto L8c660934; /* mov.w 0x8c66094e,r0 */
L8c660934: *stop=0x8c660936u;goto L8c660a6a; /* bra 0x8c660a6a */
L8c660938: *stop=0x8c660938u;if(!valid_access(r[8]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[8]+0x4u,1);if(io_status)return io_status;goto L8c66093a; /* mov.b @(0x4,r8),r0 */
L8c66093a: *stop=0x8c66093au;if(!valid_access(r[8]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8]+0x8u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c66093c; /* mov.l @(0x8,r8),r12 */
L8c66093c: *stop=0x8c66093cu;r[0]=r[0]&255u;goto L8c66093e; /* extu.b r0,r0 */
L8c66093e: *stop=0x8c66093eu;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c660940; /* cmp/eq #0x8,r0 */
L8c660940: take=(*sr&1u);*stop=0x8c660942u;r[12]+=8u;if(take)goto L8c660978;goto L8c660944; /* bt/s 0x8c660978 */
L8c660944: *stop=0x8c660944u;r[1]=sx(m,bus,&io_status,0x8c660950u,2);if(io_status)return io_status;goto L8c660946; /* mov.w 0x8c660950,r1 */
L8c660946: *stop=0x8c660946u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c660948; /* cmp/eq r1,r0 */
L8c660948: if((*sr&1u))goto L8c6608ee;goto L8c66094a; /* bt 0x8c6608ee */
L8c66094a: *stop=0x8c66094cu;goto L8c660a68; /* bra 0x8c660a68 */
L8c660978: *stop=0x8c660978u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660ad4u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c66097a; /* mov.l 0x8c660ad4,r10 */
L8c66097a: *stop=0x8c66097au;r[5]=0u;goto L8c66097c; /* mov #0x0,r5 */
L8c66097c: *stop=0x8c66097cu;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66097e; /* mov.l @r10,r2 */
L8c66097e: *stop=0x8c66097eu;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660980; /* mov.l @(0xc,r2),r3 */
L8c660980: target=r[3];*pr=0x8c660984u;*stop=0x8c660982u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660984; /* jsr @r3 */
L8c660984: *stop=0x8c660984u;r[0]=r[0]&65535u;goto L8c660986; /* extu.w r0,r0 */
L8c660986: *stop=0x8c660986u;r[5]=2u;goto L8c660988; /* mov #0x2,r5 */
L8c660988: *stop=0x8c660988u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;goto L8c66098a; /* mov.l r0,@r14 */
L8c66098a: *stop=0x8c66098au;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66098c; /* mov.l @r10,r2 */
L8c66098c: *stop=0x8c66098cu;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66098e; /* mov.l @(0xc,r2),r3 */
L8c66098e: target=r[3];*pr=0x8c660992u;*stop=0x8c660990u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660992; /* jsr @r3 */
L8c660992: *stop=0x8c660992u;r[0]=r[0]&65535u;goto L8c660994; /* extu.w r0,r0 */
L8c660994: *stop=0x8c660994u;r[5]=4u;goto L8c660996; /* mov #0x4,r5 */
L8c660996: *stop=0x8c660996u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],4);if(io_status)return io_status;goto L8c660998; /* mov.l r0,@(0x4,r14) */
L8c660998: *stop=0x8c660998u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66099a; /* mov.l @r10,r2 */
L8c66099a: *stop=0x8c66099au;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66099c; /* mov.l @(0xc,r2),r3 */
L8c66099c: target=r[3];*pr=0x8c6609a0u;*stop=0x8c66099eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6609a0; /* jsr @r3 */
L8c6609a0: *stop=0x8c6609a0u;r[0]=r[0]&65535u;goto L8c6609a2; /* extu.w r0,r0 */
L8c6609a2: *stop=0x8c6609a2u;r[5]=6u;goto L8c6609a4; /* mov #0x6,r5 */
L8c6609a4: *stop=0x8c6609a4u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],4);if(io_status)return io_status;goto L8c6609a6; /* mov.l r0,@(0x8,r14) */
L8c6609a6: *stop=0x8c6609a6u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6609a8; /* mov.l @r10,r2 */
L8c6609a8: *stop=0x8c6609a8u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6609aa; /* mov.l @(0xc,r2),r3 */
L8c6609aa: target=r[3];*pr=0x8c6609aeu;*stop=0x8c6609acu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6609ae; /* jsr @r3 */
L8c6609ae: *stop=0x8c6609aeu;r[0]=r[0]&65535u;goto L8c6609b0; /* extu.w r0,r0 */
L8c6609b0: *stop=0x8c6609b0u;r[5]=8u;goto L8c6609b2; /* mov #0x8,r5 */
L8c6609b2: *stop=0x8c6609b2u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[0],4);if(io_status)return io_status;goto L8c6609b4; /* mov.l r0,@(0xc,r14) */
L8c6609b4: *stop=0x8c6609b4u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6609b6; /* mov.l @r10,r2 */
L8c6609b6: *stop=0x8c6609b6u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6609b8; /* mov.l @(0xc,r2),r3 */
L8c6609b8: target=r[3];*pr=0x8c6609bcu;*stop=0x8c6609bau;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6609bc; /* jsr @r3 */
L8c6609bc: *stop=0x8c6609bcu;r[0]=r[0]&65535u;goto L8c6609be; /* extu.w r0,r0 */
L8c6609be: *stop=0x8c6609beu;r[5]=10u;goto L8c6609c0; /* mov #0xa,r5 */
L8c6609c0: *stop=0x8c6609c0u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[0],4);if(io_status)return io_status;goto L8c6609c2; /* mov.l r0,@(0x10,r14) */
L8c6609c2: *stop=0x8c6609c2u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6609c4; /* mov.l @r10,r2 */
L8c6609c4: *stop=0x8c6609c4u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6609c6; /* mov.l @(0xc,r2),r3 */
L8c6609c6: target=r[3];*pr=0x8c6609cau;*stop=0x8c6609c8u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6609ca; /* jsr @r3 */
L8c6609ca: *stop=0x8c6609cau;r[0]=r[0]&65535u;goto L8c6609cc; /* extu.w r0,r0 */
L8c6609cc: *stop=0x8c6609ccu;r[5]=12u;goto L8c6609ce; /* mov #0xc,r5 */
L8c6609ce: *stop=0x8c6609ceu;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[0],4);if(io_status)return io_status;goto L8c6609d0; /* mov.l r0,@(0x14,r14) */
L8c6609d0: *stop=0x8c6609d0u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6609d2; /* mov.l @r10,r2 */
L8c6609d2: *stop=0x8c6609d2u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6609d4; /* mov.l @(0xc,r2),r3 */
L8c6609d4: target=r[3];*pr=0x8c6609d8u;*stop=0x8c6609d6u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6609d8; /* jsr @r3 */
L8c6609d8: *stop=0x8c6609d8u;r[0]=r[0]&65535u;goto L8c6609da; /* extu.w r0,r0 */
L8c6609da: *stop=0x8c6609dau;r[5]=16u;goto L8c6609dc; /* mov #0x10,r5 */
L8c6609dc: *stop=0x8c6609dcu;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[0],4);if(io_status)return io_status;goto L8c6609de; /* mov.l r0,@(0x18,r14) */
L8c6609de: *stop=0x8c6609deu;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6609e0; /* mov.l @r10,r2 */
L8c6609e0: *stop=0x8c6609e0u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6609e2; /* mov.l @(0xc,r2),r3 */
L8c6609e2: target=r[3];*pr=0x8c6609e6u;*stop=0x8c6609e4u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6609e6; /* jsr @r3 */
L8c6609e6: *stop=0x8c6609e6u;r[0]=r[0]&65535u;goto L8c6609e8; /* extu.w r0,r0 */
L8c6609e8: *stop=0x8c6609e8u;r[5]=18u;goto L8c6609ea; /* mov #0x12,r5 */
L8c6609ea: *stop=0x8c6609eau;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c6609ec; /* mov.l r0,@(0x1c,r14) */
L8c6609ec: *stop=0x8c6609ecu;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6609ee; /* mov.l @r10,r2 */
L8c6609ee: *stop=0x8c6609eeu;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6609f0; /* mov.l @(0xc,r2),r3 */
L8c6609f0: target=r[3];*pr=0x8c6609f4u;*stop=0x8c6609f2u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6609f4; /* jsr @r3 */
L8c6609f4: *stop=0x8c6609f4u;r[0]=r[0]&65535u;goto L8c6609f6; /* extu.w r0,r0 */
L8c6609f6: *stop=0x8c6609f6u;r[5]=20u;goto L8c6609f8; /* mov #0x14,r5 */
L8c6609f8: *stop=0x8c6609f8u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[0],4);if(io_status)return io_status;goto L8c6609fa; /* mov.l r0,@(0x20,r14) */
L8c6609fa: *stop=0x8c6609fau;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6609fc; /* mov.l @r10,r2 */
L8c6609fc: *stop=0x8c6609fcu;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6609fe; /* mov.l @(0xc,r2),r3 */
L8c6609fe: target=r[3];*pr=0x8c660a02u;*stop=0x8c660a00u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660a02; /* jsr @r3 */
L8c660a02: *stop=0x8c660a02u;r[0]=r[0]&65535u;goto L8c660a04; /* extu.w r0,r0 */
L8c660a04: *stop=0x8c660a04u;r[2]=sx(m,bus,&io_status,0x8c660ac8u,2);if(io_status)return io_status;goto L8c660a06; /* mov.w 0x8c660ac8,r2 */
L8c660a06: *stop=0x8c660a06u;if(!valid_access(r[14]+52u,4u))return -3;wr(m,bus,&io_status,r[14]+0x34u,r[0],4);if(io_status)return io_status;goto L8c660a08; /* mov.l r0,@(0x34,r14) */
L8c660a08: *stop=0x8c660a08u;r[3]=sx(m,bus,&io_status,0x8c660acau,2);if(io_status)return io_status;goto L8c660a0a; /* mov.w 0x8c660aca,r3 */
L8c660a0a: *stop=0x8c660a0au;if(!valid_access(r[14]+56u,4u))return -3;wr(m,bus,&io_status,r[14]+0x38u,r[2],4);if(io_status)return io_status;goto L8c660a0c; /* mov.l r2,@(0x38,r14) */
L8c660a0c: *stop=0x8c660a0cu;if(!valid_access(r[12]+14u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0xeu,1);if(io_status)return io_status;goto L8c660a0e; /* mov.b @(0xe,r12),r0 */
L8c660a0e: *stop=0x8c660a0eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c660ad8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c660a10; /* mov.l 0x8c660ad8,r2 */
L8c660a10: *stop=0x8c660a10u;r[0]=r[0]&255u;goto L8c660a12; /* extu.b r0,r0 */
L8c660a12: *stop=0x8c660a12u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[0],4);if(io_status)return io_status;goto L8c660a14; /* mov.l r0,@(0x24,r14) */
L8c660a14: *stop=0x8c660a14u;if(!valid_access(r[12]+15u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0xfu,1);if(io_status)return io_status;goto L8c660a16; /* mov.b @(0xf,r12),r0 */
L8c660a16: *stop=0x8c660a16u;r[0]=r[0]&255u;goto L8c660a18; /* extu.b r0,r0 */
L8c660a18: *stop=0x8c660a18u;if(!valid_access(r[14]+40u,4u))return -3;wr(m,bus,&io_status,r[14]+0x28u,r[0],4);if(io_status)return io_status;goto L8c660a1a; /* mov.l r0,@(0x28,r14) */
L8c660a1a: *stop=0x8c660a1au;if(!valid_access(r[14]+44u,4u))return -3;wr(m,bus,&io_status,r[14]+0x2cu,r[3],4);if(io_status)return io_status;goto L8c660a1c; /* mov.l r3,@(0x2c,r14) */
L8c660a1c: target=r[2];*pr=0x8c660a20u;*stop=0x8c660a1eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660a20; /* jsr @r2 */
L8c660a20: *stop=0x8c660a20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660adcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660a22; /* mov.l 0x8c660adc,r3 */
L8c660a22: *stop=0x8c660a22u;r[4]=r[15];goto L8c660a24; /* mov r15,r4 */
L8c660a24: *stop=0x8c660a24u;r[5]=r[0];goto L8c660a26; /* mov r0,r5 */
L8c660a26: target=r[3];*pr=0x8c660a2au;*stop=0x8c660a28u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660a2a; /* jsr @r3 */
L8c660a2a: *stop=0x8c660a2au;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c660a2c; /* cmp/pz r0 */
L8c660a2c: if(!(*sr&1u))goto L8c660a64;goto L8c660a2e; /* bf 0x8c660a64 */
L8c660a2e: *stop=0x8c660a2eu;if(!valid_access(r[15]+10u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0xau,2);if(io_status)return io_status;goto L8c660a30; /* mov.w @(0xa,r15),r0 */
L8c660a30: *stop=0x8c660a30u;r[1]=64u;goto L8c660a32; /* mov #0x40,r1 */
L8c660a32: *stop=0x8c660a32u;r[1]+=r[14];goto L8c660a34; /* add r14,r1 */
L8c660a34: *stop=0x8c660a34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660ae0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660a36; /* mov.l 0x8c660ae0,r3 */
L8c660a36: *stop=0x8c660a36u;r[0]=r[0]&65535u;goto L8c660a38; /* extu.w r0,r0 */
L8c660a38: *stop=0x8c660a38u;r[4]=sx(m,bus,&io_status,0x8c660accu,2);if(io_status)return io_status;goto L8c660a3a; /* mov.w 0x8c660acc,r4 */
L8c660a3a: *stop=0x8c660a3au;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[0],4);if(io_status)return io_status;goto L8c660a3c; /* mov.l r0,@(0x3c,r14) */
L8c660a3c: *stop=0x8c660a3cu;if(!valid_access(r[15]+8u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x8u,2);if(io_status)return io_status;goto L8c660a3e; /* mov.w @(0x8,r15),r0 */
L8c660a3e: *stop=0x8c660a3eu;r[0]=r[0]&65535u;goto L8c660a40; /* extu.w r0,r0 */
L8c660a40: *stop=0x8c660a40u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c660a42; /* mov.l r0,@r1 */
L8c660a42: *stop=0x8c660a42u;if(!valid_access(r[15]+10u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0xau,2);if(io_status)return io_status;goto L8c660a44; /* mov.w @(0xa,r15),r0 */
L8c660a44: *stop=0x8c660a44u;r[0]=r[0]&65535u;goto L8c660a46; /* extu.w r0,r0 */
L8c660a46: target=r[3];*pr=0x8c660a4au;*stop=0x8c660a48u;r[1]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660a4a; /* jsr @r3 */
L8c660a4a: *stop=0x8c660a4au;r[1]=68u;goto L8c660a4c; /* mov #0x44,r1 */
L8c660a4c: *stop=0x8c660a4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c660ae0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660a4e; /* mov.l 0x8c660ae0,r3 */
L8c660a4e: *stop=0x8c660a4eu;r[1]+=r[14];goto L8c660a50; /* add r14,r1 */
L8c660a50: *stop=0x8c660a50u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c660a52; /* mov.l r0,@r1 */
L8c660a52: *stop=0x8c660a52u;if(!valid_access(r[15]+8u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x8u,2);if(io_status)return io_status;goto L8c660a54; /* mov.w @(0x8,r15),r0 */
L8c660a54: *stop=0x8c660a54u;r[0]=r[0]&65535u;goto L8c660a56; /* extu.w r0,r0 */
L8c660a56: target=r[3];*pr=0x8c660a5au;*stop=0x8c660a58u;r[1]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660a5a; /* jsr @r3 */
L8c660a5a: *stop=0x8c660a5au;r[1]=72u;goto L8c660a5c; /* mov #0x48,r1 */
L8c660a5c: *stop=0x8c660a5cu;r[1]+=r[14];goto L8c660a5e; /* add r14,r1 */
L8c660a5e: *stop=0x8c660a5eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c660a60; /* mov.l r0,@r1 */
L8c660a60: *stop=0x8c660a62u;r[0]=0u;goto L8c660a6a; /* bra 0x8c660a6a */
L8c660a64: target=r[11];*pr=0x8c660a68u;*stop=0x8c660a66u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660a68; /* jsr @r11 */
L8c660a68: *stop=0x8c660a68u;r[0]=sx(m,bus,&io_status,0x8c660aceu,2);if(io_status)return io_status;goto L8c660a6a; /* mov.w 0x8c660ace,r0 */
L8c660a6a: *stop=0x8c660a6au;r[15]+=16u;goto L8c660a6c; /* add #0x10,r15 */
L8c660a6c: *stop=0x8c660a6cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660a6e; /* lds.l @r15+,PR */
L8c660a6e: *stop=0x8c660a6eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c660a70; /* mov.l @r15+,r8 */
L8c660a70: *stop=0x8c660a70u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c660a72; /* mov.l @r15+,r9 */
L8c660a72: *stop=0x8c660a72u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c660a74; /* mov.l @r15+,r10 */
L8c660a74: *stop=0x8c660a74u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c660a76; /* mov.l @r15+,r11 */
L8c660a76: *stop=0x8c660a76u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c660a78; /* mov.l @r15+,r12 */
L8c660a78: *stop=0x8c660a78u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c660a7a; /* mov.l @r15+,r13 */
L8c660a7a: *stop=0x8c660a7au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c660a7cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_io_8c6608a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6608c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6608c6u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6608ce(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6608ceu);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6608ee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6608eeu);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6608f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6608f4u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6608fc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6608fcu);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660906(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660906u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660912(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660912u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c66091a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c66091au);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660922(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660922u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c66092a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c66092au);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660932(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660932u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660984(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660984u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660992(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660992u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6609a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6609a0u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6609ae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6609aeu);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6609bc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6609bcu);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6609ca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6609cau);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6609d8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6609d8u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6609e6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6609e6u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6609f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6609f4u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660a02(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660a02u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660a20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660a20u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660a2a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660a2au);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660a4a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660a4au);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660a5a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660a5au);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660a68(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6608a4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660a68u);}

static int sa_movie_channel_io_run_8c660dec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c660e00u)goto L8c660e00;if(resume_pc==0x8c660e08u)goto L8c660e08;if(resume_pc==0x8c660e18u)goto L8c660e18;goto L8c660dec;
L8c660dec: *stop=0x8c660decu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c660dee; /* mov.l r14,@-r15 */
L8c660dee: *stop=0x8c660deeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c660df0; /* sts.l PR,@-r15 */
L8c660df0: *stop=0x8c660df0u;r[15]+=4294967288u;goto L8c660df2; /* add #-0x8,r15 */
L8c660df2: *stop=0x8c660df2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660e3cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c660df4; /* mov.l 0x8c660e3c,r0 */
L8c660df4: *stop=0x8c660df4u;r[14]=r[4];goto L8c660df6; /* mov r4,r14 */
L8c660df6: *stop=0x8c660df6u;r[14]<<=2;goto L8c660df8; /* shll2 r14 */
L8c660df8: *stop=0x8c660df8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660e40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660dfa; /* mov.l 0x8c660e40,r3 */
L8c660dfa: *stop=0x8c660dfau;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[14]=loaded;}goto L8c660dfc; /* mov.l @(r0,r14),r14 */
L8c660dfc: target=r[3];*pr=0x8c660e00u;*stop=0x8c660dfeu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660e00; /* jsr @r3 */
L8c660e00: *stop=0x8c660e00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660e44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660e02; /* mov.l 0x8c660e44,r3 */
L8c660e02: *stop=0x8c660e02u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;goto L8c660e04; /* mov.l r0,@(0x4,r15) */
L8c660e04: target=r[3];*pr=0x8c660e08u;*stop=0x8c660e06u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660e08; /* jsr @r3 */
L8c660e08: *stop=0x8c660e08u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c660e0a; /* mov.l r0,@r15 */
L8c660e0a: *stop=0x8c660e0au;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660e0c; /* mov.l @(0x4,r15),r3 */
L8c660e0c: *stop=0x8c660e0cu;if(!valid_access(r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;goto L8c660e0e; /* mov.b @r3,r2 */
L8c660e0e: *stop=0x8c660e0eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c660e10; /* tst r2,r2 */
L8c660e10: if((*sr&1u))goto L8c660e24;goto L8c660e12; /* bt 0x8c660e24 */
L8c660e12: *stop=0x8c660e12u;{uint32_t loaded=rd(m,bus,&io_status,0x8c660e48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c660e14; /* mov.l 0x8c660e48,r3 */
L8c660e14: target=r[3];*pr=0x8c660e18u;*stop=0x8c660e16u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c660e18; /* jsr @r3 */
L8c660e18: *stop=0x8c660e18u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c660e1a; /* tst r0,r0 */
L8c660e1a: if(!(*sr&1u))goto L8c660e24;goto L8c660e1c; /* bf 0x8c660e24 */
L8c660e1c: *stop=0x8c660e1cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c660e1e; /* mov.l @r15,r0 */
L8c660e1e: *stop=0x8c660e1eu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c660e20; /* mov.l @r0,r0 */
L8c660e20: *stop=0x8c660e20u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c660e22; /* tst #0x2,r0 */
L8c660e22: if(!(*sr&1u))goto L8c660e4c;goto L8c660e24; /* bf 0x8c660e4c */
L8c660e24: *stop=0x8c660e24u;r[0]=0u;goto L8c660e26; /* mov #0x0,r0 */
L8c660e26: *stop=0x8c660e26u;r[15]+=8u;goto L8c660e28; /* add #0x8,r15 */
L8c660e28: *stop=0x8c660e28u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660e2a; /* lds.l @r15+,PR */
L8c660e2a: *stop=0x8c660e2au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c660e2cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c660e4c: *stop=0x8c660e4cu;r[0]=1u;goto L8c660e4e; /* mov #0x1,r0 */
L8c660e4e: *stop=0x8c660e4eu;r[15]+=8u;goto L8c660e50; /* add #0x8,r15 */
L8c660e50: *stop=0x8c660e50u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c660e52; /* lds.l @r15+,PR */
L8c660e52: *stop=0x8c660e52u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c660e54u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_io_8c660dec(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c660dec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660e00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c660dec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660e00u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660e08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c660dec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660e08u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c660e18(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c660dec(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c660e18u);}

SA_STAGE_EXPORT int sa_movie_channel_io_8c646884(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c646884;
L8c646884: *stop=0x8c646884u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6468c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646886; /* mov.l 0x8c6468c0,r3 */
L8c646886: target=r[3];*stop=0x8c646888u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_channel_io_8c646b36(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c646b36;
L8c646b36: *stop=0x8c646b36u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646c34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646b38; /* mov.l 0x8c646c34,r3 */
L8c646b38: target=r[3];*stop=0x8c646b3au;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_channel_io_8c646b3c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c646b3c;
L8c646b3c: *stop=0x8c646b3cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c646c38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646b3e; /* mov.l 0x8c646c38,r3 */
L8c646b3e: target=r[3];*stop=0x8c646b40u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

static int sa_movie_channel_io_run_8c646bbc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c646bccu)goto L8c646bcc;goto L8c646bbc;
L8c646bbc: *stop=0x8c646bbcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c646bbe; /* mov.l r14,@-r15 */
L8c646bbe: *stop=0x8c646bbeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c646bc0; /* sts.l PR,@-r15 */
L8c646bc0: *stop=0x8c646bc0u;r[15]+=4294967292u;goto L8c646bc2; /* add #-0x4,r15 */
L8c646bc2: *stop=0x8c646bc2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c646bc4; /* mov.l r5,@r15 */
L8c646bc4: *stop=0x8c646bc4u;r[5]=2u;goto L8c646bc6; /* mov #0x2,r5 */
L8c646bc6: *stop=0x8c646bc6u;r[14]=r[4];goto L8c646bc8; /* mov r4,r14 */
L8c646bc8: target=0x8c646b42u;*pr=0x8c646bccu;*stop=0x8c646bcau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646bcc; /* bsr 0x8c646b42 */
L8c646bcc: *stop=0x8c646bccu;r[4]=r[0];goto L8c646bce; /* mov r0,r4 */
L8c646bce: *stop=0x8c646bceu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c646bd0; /* tst r4,r4 */
L8c646bd0: if(!(*sr&1u))goto L8c646bdc;goto L8c646bd2; /* bf 0x8c646bdc */
L8c646bd2: *stop=0x8c646bd2u;r[0]=4294967295u;goto L8c646bd4; /* mov #-0x1,r0 */
L8c646bd4: *stop=0x8c646bd4u;r[15]+=4u;goto L8c646bd6; /* add #0x4,r15 */
L8c646bd6: *stop=0x8c646bd6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c646bd8; /* lds.l @r15+,PR */
L8c646bd8: *stop=0x8c646bd8u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c646bdau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
L8c646bdc: *stop=0x8c646bdcu;r[5]=r[4];goto L8c646bde; /* mov r4,r5 */
L8c646bde: *stop=0x8c646bdeu;r[5]+=2u;goto L8c646be0; /* add #0x2,r5 */
L8c646be0: *stop=0x8c646be0u;if(!valid_access(r[4],1u))return -3;r[3]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;goto L8c646be2; /* mov.b @r4,r3 */
L8c646be2: *stop=0x8c646be2u;r[3]=r[3]&255u;goto L8c646be4; /* extu.b r3,r3 */
L8c646be4: *stop=0x8c646be4u;r[3]+=1u;goto L8c646be6; /* add #0x1,r3 */
L8c646be6: *stop=0x8c646be6u;if(!valid_access(r[14],2u))return -3;wr(m,bus,&io_status,r[14],r[3],2);if(io_status)return io_status;goto L8c646be8; /* mov.w r3,@r14 */
L8c646be8: *stop=0x8c646be8u;r[3]=4294967289u;goto L8c646bea; /* mov #-0x7,r3 */
L8c646bea: *stop=0x8c646beau;if(!valid_access(r[4]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x1u,1);if(io_status)return io_status;goto L8c646bec; /* mov.b @(0x1,r4),r0 */
L8c646bec: *stop=0x8c646becu;r[0]=r[0]&255u;goto L8c646bee; /* extu.b r0,r0 */
L8c646bee: *stop=0x8c646beeu;r[0]+=1u;goto L8c646bf0; /* add #0x1,r0 */
L8c646bf0: *stop=0x8c646bf0u;r[0]<<=2;goto L8c646bf2; /* shll2 r0 */
L8c646bf2: *stop=0x8c646bf2u;r[0]<<=2;goto L8c646bf4; /* shll2 r0 */
L8c646bf4: *stop=0x8c646bf4u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c646bf6; /* shll r0 */
L8c646bf6: *stop=0x8c646bf6u;if(!valid_access(r[14]+2u,2u))return -3;wr(m,bus,&io_status,r[14]+0x2u,r[0],2);if(io_status)return io_status;goto L8c646bf8; /* mov.w r0,@(0x2,r14) */
L8c646bf8: *stop=0x8c646bf8u;if(!valid_access(r[5],1u))return -3;r[0]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c646bfa; /* mov.b @r5,r0 */
L8c646bfa: *stop=0x8c646bfau;r[0]=r[0]&255u;goto L8c646bfc; /* extu.b r0,r0 */
L8c646bfc: *stop=0x8c646bfcu;r[0]>>=2;goto L8c646bfe; /* shlr2 r0 */
L8c646bfe: *stop=0x8c646bfeu;r[0]>>=2;goto L8c646c00; /* shlr2 r0 */
L8c646c00: *stop=0x8c646c00u;r[0]&=0xfu;goto L8c646c02; /* and #0xf,r0 */
L8c646c02: *stop=0x8c646c02u;if(!valid_access(r[14]+4u,2u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],2);if(io_status)return io_status;goto L8c646c04; /* mov.w r0,@(0x4,r14) */
L8c646c04: *stop=0x8c646c04u;if(!valid_access(r[5],1u))return -3;r[0]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c646c06; /* mov.b @r5,r0 */
L8c646c06: *stop=0x8c646c06u;r[0]=r[0]&255u;goto L8c646c08; /* extu.b r0,r0 */
L8c646c08: *stop=0x8c646c08u;r[0]&=0xfu;goto L8c646c0a; /* and #0xf,r0 */
L8c646c0a: *stop=0x8c646c0au;if(!valid_access(r[14]+6u,2u))return -3;wr(m,bus,&io_status,r[14]+0x6u,r[0],2);if(io_status)return io_status;goto L8c646c0c; /* mov.w r0,@(0x6,r14) */
L8c646c0c: *stop=0x8c646c0cu;if(!valid_access(r[4]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x3u,1);if(io_status)return io_status;goto L8c646c0e; /* mov.b @(0x3,r4),r0 */
L8c646c0e: *stop=0x8c646c0eu;r[0]=r[0]&255u;goto L8c646c10; /* extu.b r0,r0 */
L8c646c10: *stop=0x8c646c10u;r[0]=(r[3]&0x80000000u)?((r[3]&31u)?r[0]>>((0u-r[3])&31u):0u):(r[0]<<(r[3]&31u));goto L8c646c12; /* shld r3,r0 */
L8c646c12: *stop=0x8c646c12u;r[0]&=0x1u;goto L8c646c14; /* and #0x1,r0 */
L8c646c14: *stop=0x8c646c14u;if(!valid_access(r[14]+8u,2u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[0],2);if(io_status)return io_status;goto L8c646c16; /* mov.w r0,@(0x8,r14) */
L8c646c16: *stop=0x8c646c16u;r[0]=0u;goto L8c646c18; /* mov #0x0,r0 */
L8c646c18: *stop=0x8c646c18u;r[15]+=4u;goto L8c646c1a; /* add #0x4,r15 */
L8c646c1a: *stop=0x8c646c1au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c646c1c; /* lds.l @r15+,PR */
L8c646c1c: *stop=0x8c646c1cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c646c1eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_io_8c646bbc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c646bbc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c646bcc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c646bbc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c646bccu);}

static int sa_movie_channel_io_run_8c646982(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c64698cu)goto L8c64698c;if(resume_pc==0x8c64699eu)goto L8c64699e;goto L8c646982;
L8c646982: *stop=0x8c646982u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c646984; /* sts.l PR,@-r15 */
L8c646984: *stop=0x8c646984u;r[15]+=4294967292u;goto L8c646986; /* add #-0x4,r15 */
L8c646986: *stop=0x8c646986u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6469c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646988; /* mov.l 0x8c6469c4,r3 */
L8c646988: target=r[3];*pr=0x8c64698cu;*stop=0x8c64698au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64698c; /* jsr @r3 */
L8c64698c: *stop=0x8c64698cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6469b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64698e; /* mov.l 0x8c6469b0,r2 */
L8c64698e: *stop=0x8c64698eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c646990; /* mov.l r0,@r15 */
L8c646990: *stop=0x8c646990u;r[0]=sx(m,bus,&io_status,0x8c6469a8u,2);if(io_status)return io_status;goto L8c646992; /* mov.w 0x8c6469a8,r0 */
L8c646992: *stop=0x8c646992u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c646994; /* mov.l @r2,r3 */
L8c646994: *stop=0x8c646994u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c646996; /* mov.l @(r0,r3),r1 */
L8c646996: *stop=0x8c646996u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c646998; /* tst r1,r1 */
L8c646998: if((*sr&1u))goto L8c64699e;goto L8c64699a; /* bt 0x8c64699e */
L8c64699a: target=0x8c646654u;*pr=0x8c64699eu;*stop=0x8c64699cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64699e; /* bsr 0x8c646654 */
L8c64699e: *stop=0x8c64699eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6469a0; /* mov.l @r15,r0 */
L8c6469a0: *stop=0x8c6469a0u;r[15]+=4u;goto L8c6469a2; /* add #0x4,r15 */
L8c6469a2: *stop=0x8c6469a2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6469a4; /* lds.l @r15+,PR */
L8c6469a4: *stop=0x8c6469a4u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6469a6u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_io_8c646982(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c646982(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c64698c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c646982(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64698cu);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c64699e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c646982(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c64699eu);}

static int sa_movie_channel_io_run_8c6469c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c6469d6u)goto L8c6469d6;if(resume_pc==0x8c6469eau)goto L8c6469ea;goto L8c6469c8;
L8c6469c8: *stop=0x8c6469c8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6469ca; /* sts.l PR,@-r15 */
L8c6469ca: *stop=0x8c6469cau;r[15]+=4294967292u;goto L8c6469cc; /* add #-0x4,r15 */
L8c6469cc: *stop=0x8c6469ccu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6469ce; /* mov.l @(0x8,r15),r3 */
L8c6469ce: *stop=0x8c6469ceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6469d0; /* mov.l r3,@-r15 */
L8c6469d0: *stop=0x8c6469d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646ac8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6469d2; /* mov.l 0x8c646ac8,r3 */
L8c6469d2: target=r[3];*pr=0x8c6469d6u;*stop=0x8c6469d4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6469d6; /* jsr @r3 */
L8c6469d6: *stop=0x8c6469d6u;r[15]+=4u;goto L8c6469d8; /* add #0x4,r15 */
L8c6469d8: *stop=0x8c6469d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646accu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6469da; /* mov.l 0x8c646acc,r2 */
L8c6469da: *stop=0x8c6469dau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c6469dc; /* mov.l r0,@r15 */
L8c6469dc: *stop=0x8c6469dcu;r[0]=sx(m,bus,&io_status,0x8c646ac6u,2);if(io_status)return io_status;goto L8c6469de; /* mov.w 0x8c646ac6,r0 */
L8c6469de: *stop=0x8c6469deu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6469e0; /* mov.l @r2,r3 */
L8c6469e0: *stop=0x8c6469e0u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6469e2; /* mov.l @(r0,r3),r1 */
L8c6469e2: *stop=0x8c6469e2u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c6469e4; /* tst r1,r1 */
L8c6469e4: if((*sr&1u))goto L8c6469ea;goto L8c6469e6; /* bt 0x8c6469ea */
L8c6469e6: target=0x8c646654u;*pr=0x8c6469eau;*stop=0x8c6469e8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6469ea; /* bsr 0x8c646654 */
L8c6469ea: *stop=0x8c6469eau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6469ec; /* mov.l @r15,r0 */
L8c6469ec: *stop=0x8c6469ecu;r[15]+=4u;goto L8c6469ee; /* add #0x4,r15 */
L8c6469ee: *stop=0x8c6469eeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6469f0; /* lds.l @r15+,PR */
L8c6469f0: *stop=0x8c6469f0u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6469f2u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_io_8c6469c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6469c8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6469d6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6469c8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6469d6u);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c6469ea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c6469c8(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6469eau);}

static int sa_movie_channel_io_run_8c65f3f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c65f3fcu)goto L8c65f3fc;goto L8c65f3f4;
L8c65f3f4: *stop=0x8c65f3f4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65f3f6; /* sts.l PR,@-r15 */
L8c65f3f6: *stop=0x8c65f3f6u;r[15]+=4294967292u;goto L8c65f3f8; /* add #-0x4,r15 */
L8c65f3f8: target=0x8c65f390u;*pr=0x8c65f3fcu;*stop=0x8c65f3fau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f3fc; /* bsr 0x8c65f390 */
L8c65f3fc: *stop=0x8c65f3fcu;r[4]=r[0];goto L8c65f3fe; /* mov r0,r4 */
L8c65f3fe: *stop=0x8c65f3feu;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f400; /* mov.l @(0x20,r4),r3 */
L8c65f400: *stop=0x8c65f400u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65f402; /* mov.l @r15,r2 */
L8c65f402: *stop=0x8c65f402u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c65f404; /* cmp/eq r2,r3 */
L8c65f404: *stop=0x8c65f404u;r[0]=*sr&1u;goto L8c65f406; /* movt r0 */
L8c65f406: *stop=0x8c65f406u;r[15]+=4u;goto L8c65f408; /* add #0x4,r15 */
L8c65f408: *stop=0x8c65f408u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f40a; /* lds.l @r15+,PR */
L8c65f40a: *stop=0x8c65f40au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c65f40cu;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_io_8c65f3f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c65f3f4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c65f3fc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c65f3f4(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c65f3fcu);}

static int sa_movie_channel_io_run_8c65f40e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c65f414u)goto L8c65f414;goto L8c65f40e;
L8c65f40e: *stop=0x8c65f40eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65f410; /* sts.l PR,@-r15 */
L8c65f410: target=0x8c65f390u;*pr=0x8c65f414u;*stop=0x8c65f412u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f414; /* bsr 0x8c65f390 */
L8c65f414: *stop=0x8c65f414u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f416; /* lds.l @r15+,PR */
L8c65f416: *stop=0x8c65f416u;r[4]=r[0];goto L8c65f418; /* mov r0,r4 */
L8c65f418: *stop=0x8c65f418u;if(!valid_access(r[4]+12u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0xcu,1);if(io_status)return io_status;goto L8c65f41a; /* mov.b @(0xc,r4),r0 */
L8c65f41a: *stop=0x8c65f41au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c65f41cu;r[0]=r[0]&255u;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_io_8c65f40e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c65f40e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c65f414(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c65f40e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c65f414u);}

static int sa_movie_channel_io_run_8c65f41e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c65f424u)goto L8c65f424;goto L8c65f41e;
L8c65f41e: *stop=0x8c65f41eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65f420; /* sts.l PR,@-r15 */
L8c65f420: target=0x8c65f390u;*pr=0x8c65f424u;*stop=0x8c65f422u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65f424; /* bsr 0x8c65f390 */
L8c65f424: *stop=0x8c65f424u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65f426; /* lds.l @r15+,PR */
L8c65f426: *stop=0x8c65f426u;r[4]=r[0];goto L8c65f428; /* mov r0,r4 */
L8c65f428: *stop=0x8c65f428u;r[0]=1u;goto L8c65f42a; /* mov #0x1,r0 */
L8c65f42a: *stop=0x8c65f42au;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c65f42cu;if(!valid_access(r[4]+13u,1u))return -3;wr(m,bus,&io_status,r[4]+0xdu,r[0],1);if(io_status)return io_status;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movie_channel_io_8c65f41e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c65f41e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movie_channel_io_resume_8c65f424(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movie_channel_io_run_8c65f41e(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c65f424u);}

SA_STAGE_EXPORT int sa_movie_channel_io_8c65f390(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f390;
L8c65f390: *stop=0x8c65f390u;r[2]=44u;goto L8c65f392; /* mov #0x2c,r2 */
L8c65f392: *stop=0x8c65f392u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f3d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f394; /* mov.l 0x8c65f3d8,r3 */
L8c65f394: *stop=0x8c65f394u;*macl=r[2]*r[4];goto L8c65f396; /* mul.l r2,r4 */
L8c65f396: *stop=0x8c65f396u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f398; /* mov.l @r3,r0 */
L8c65f398: *stop=0x8c65f398u;r[0]+=40u;goto L8c65f39a; /* add #0x28,r0 */
L8c65f39a: *stop=0x8c65f39au;r[4]=*macl;goto L8c65f39c; /* sts MACL,r4 */
L8c65f39c: *stop=0x8c65f39cu;if(*pr!=entry_pr)return -3;*stop=0x8c65f39eu;r[0]+=r[4];return 0; /* rts */
}
