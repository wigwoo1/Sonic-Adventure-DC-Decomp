/* Original complete owner dependencies;canonical flag/angle checks and collision response.
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
static int sa_player_movement_dependencies_run_8c0344dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c03454eu)goto L8c03454e;if(resume_pc==0x8c03458eu)goto L8c03458e;if(resume_pc==0x8c0345eeu)goto L8c0345ee;if(resume_pc==0x8c034644u)goto L8c034644;if(resume_pc==0x8c0346ccu)goto L8c0346cc;if(resume_pc==0x8c03473au)goto L8c03473a;if(resume_pc==0x8c03476au)goto L8c03476a;if(resume_pc==0x8c0347a6u)goto L8c0347a6;if(resume_pc==0x8c0347b8u)goto L8c0347b8;if(resume_pc==0x8c0347d0u)goto L8c0347d0;if(resume_pc==0x8c0347e2u)goto L8c0347e2;if(resume_pc==0x8c0347f4u)goto L8c0347f4;if(resume_pc==0x8c034804u)goto L8c034804;if(resume_pc==0x8c034810u)goto L8c034810;if(resume_pc==0x8c034816u)goto L8c034816;if(resume_pc==0x8c03486cu)goto L8c03486c;if(resume_pc==0x8c0348aau)goto L8c0348aa;if(resume_pc==0x8c0348f0u)goto L8c0348f0;goto L8c0344dc;
L8c0344dc: *stop=0x8c0344dcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0344de; /* mov.l r14,@-r15 */
L8c0344de: *stop=0x8c0344deu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0344e0; /* mov.l r13,@-r15 */
L8c0344e0: *stop=0x8c0344e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c0344e2; /* mov.l r12,@-r15 */
L8c0344e2: *stop=0x8c0344e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c0344e4; /* mov.l r11,@-r15 */
L8c0344e4: *stop=0x8c0344e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c0344e6; /* mov.l r10,@-r15 */
L8c0344e6: *stop=0x8c0344e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c0344e8; /* mov.l r9,@-r15 */
L8c0344e8: *stop=0x8c0344e8u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c0344ea; /* fmov.s fr15,@-r15 */
L8c0344ea: *stop=0x8c0344eau;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c0344ec; /* fmov.s fr14,@-r15 */
L8c0344ec: *stop=0x8c0344ecu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c0344ee; /* fmov.s fr13,@-r15 */
L8c0344ee: *stop=0x8c0344eeu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c0344f0; /* fmov.s fr12,@-r15 */
L8c0344f0: *stop=0x8c0344f0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0344f2; /* sts.l PR,@-r15 */
L8c0344f2: *stop=0x8c0344f2u;r[15]+=4294967252u;goto L8c0344f4; /* add #-0x2c,r15 */
L8c0344f4: *stop=0x8c0344f4u;r[14]=r[5];goto L8c0344f6; /* mov r5,r14 */
L8c0344f6: *stop=0x8c0344f6u;r[9]=r[4];goto L8c0344f8; /* mov r4,r9 */
L8c0344f8: *stop=0x8c0344f8u;if(!valid_access(r[9]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+0x38u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0344fa; /* mov.l @(0x38,r9),r5 */
L8c0344fa: *stop=0x8c0344fau;if(!valid_access(r[5]+22u,2u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x16u,2);if(io_status)return io_status;goto L8c0344fc; /* mov.w @(0x16,r5),r0 */
L8c0344fc: *stop=0x8c0344fcu;if(!valid_access(r[5]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x18u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0344fe; /* mov.l @(0x18,r5),r4 */
L8c0344fe: *stop=0x8c0344feu;r[6]=r[0];goto L8c034500; /* mov r0,r6 */
L8c034500: *stop=0x8c034500u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c034502; /* tst r4,r4 */
L8c034502: take=!(*sr&1u);*stop=0x8c034504u;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[5]=loaded;}if(take)goto L8c03450a;goto L8c034506; /* bf/s 0x8c03450a */
L8c034506: *stop=0x8c034508u;goto L8c0348ba; /* bra 0x8c0348ba */
L8c03450a: *stop=0x8c03450au;if(!valid_access(r[4]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x4u,2);if(io_status)return io_status;goto L8c03450c; /* mov.w @(0x4,r4),r0 */
L8c03450c: *stop=0x8c03450cu;r[3]=(uint32_t)(int32_t)(int16_t)r[6];goto L8c03450e; /* exts.w r6,r3 */
L8c03450e: *stop=0x8c03450eu;r[0]=r[0]&65535u;goto L8c034510; /* extu.w r0,r0 */
L8c034510: *stop=0x8c034510u;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[0]);goto L8c034512; /* cmp/gt r0,r3 */
L8c034512: if(!(*sr&1u))goto L8c034518;goto L8c034514; /* bf 0x8c034518 */
L8c034514: *stop=0x8c034516u;goto L8c0348ba; /* bra 0x8c0348ba */
L8c034518: *stop=0x8c034518u;r[13]=52u;goto L8c03451a; /* mov #0x34,r13 */
L8c03451a: *stop=0x8c03451au;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c03451c; /* mov.l @(0xc,r4),r3 */
L8c03451c: *stop=0x8c03451cu;*macl=(uint32_t)((int32_t)(int16_t)r[13]*(int32_t)(int16_t)r[6]);goto L8c03451e; /* muls.w r13,r6 */
L8c03451e: *stop=0x8c03451eu;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c034520; /* tst r5,r5 */
L8c034520: *stop=0x8c034520u;r[13]=*macl;goto L8c034522; /* sts MACL,r13 */
L8c034522: take=!(*sr&1u);*stop=0x8c034524u;r[13]+=r[3];if(take)goto L8c03452a;goto L8c034526; /* bf/s 0x8c03452a */
L8c034526: *stop=0x8c034528u;goto L8c0348ba; /* bra 0x8c0348ba */
L8c03452a: *stop=0x8c03452au;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c03452c; /* tst r13,r13 */
L8c03452c: if(!(*sr&1u))goto L8c034532;goto L8c03452e; /* bf 0x8c034532 */
L8c03452e: *stop=0x8c034530u;goto L8c0348ba; /* bra 0x8c0348ba */
L8c034532: *stop=0x8c034532u;r[0]=20u;goto L8c034534; /* mov #0x14,r0 */
L8c034534: *stop=0x8c034534u;r[6]=r[15];goto L8c034536; /* mov r15,r6 */
L8c034536: *stop=0x8c034536u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]+r[13]);if(io_status)return io_status;goto L8c034538; /* fmov.s @(r0,r13),fr12 */
L8c034538: *stop=0x8c034538u;r[6]+=32u;goto L8c03453a; /* add #0x20,r6 */
L8c03453a: *stop=0x8c03453au;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]+r[5]);if(io_status)return io_status;goto L8c03453c; /* fmov.s @(r0,r5),fr13 */
L8c03453c: *stop=0x8c03453cu;r[0]=28u;goto L8c03453e; /* mov #0x1c,r0 */
L8c03453e: *stop=0x8c03453eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c034540; /* fmov.s @(r0,r13),fr3 */
L8c034540: *stop=0x8c034540u;r[0]=4u;goto L8c034542; /* mov #0x4,r0 */
L8c034542: *stop=0x8c034542u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0346a4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c034544; /* mov.l 0x8c0346a4,r3 */
L8c034544: *stop=0x8c034544u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c034546; /* fmov fr3,@(r0,r15) */
L8c034546: *stop=0x8c034546u;if(!valid_access(r[4]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x10u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c034548; /* mov.l @(0x10,r4),r10 */
L8c034548: *stop=0x8c034548u;if(!valid_access(r[10]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x20u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c03454a; /* mov.l @(0x20,r10),r10 */
L8c03454a: target=r[3];*pr=0x8c03454eu;*stop=0x8c03454cu;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c03454e; /* jsr @r3 */
L8c03454e: *stop=0x8c03454eu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[0]=loaded;}goto L8c034550; /* mov.l @r13,r0 */
L8c034550: *stop=0x8c034550u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0346a8u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c034552; /* mov.l 0x8c0346a8,r11 */
L8c034552: *stop=0x8c034552u;r[0]&=0xfu;goto L8c034554; /* and #0xf,r0 */
L8c034554: *stop=0x8c034554u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0346acu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c034556; /* mov.l 0x8c0346ac,r12 */
L8c034556: *stop=0x8c034556u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c034558; /* cmp/eq #0x0,r0 */
L8c034558: *stop=0x8c034558u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][14]=0x3f800000u;goto L8c03455a; /* fldi1 fr14 */
L8c03455a: take=(*sr&1u);*stop=0x8c03455cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;if(take)goto L8c034582;goto L8c03455e; /* bt/s 0x8c034582 */
L8c03455e: *stop=0x8c03455eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c034560; /* cmp/eq #0x1,r0 */
L8c034560: if((*sr&1u))goto L8c0345e2;goto L8c034562; /* bt 0x8c0345e2 */
L8c034562: *stop=0x8c034562u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c034564; /* cmp/eq #0x2,r0 */
L8c034564: if((*sr&1u))goto L8c034638;goto L8c034566; /* bt 0x8c034638 */
L8c034566: *stop=0x8c034566u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c034568; /* cmp/eq #0x3,r0 */
L8c034568: if(!(*sr&1u))goto L8c03456e;goto L8c03456a; /* bf 0x8c03456e */
L8c03456a: *stop=0x8c03456cu;goto L8c0346c0; /* bra 0x8c0346c0 */
L8c03456e: *stop=0x8c03456eu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c034570; /* cmp/eq #0x4,r0 */
L8c034570: if(!(*sr&1u))goto L8c034576;goto L8c034572; /* bf 0x8c034576 */
L8c034572: *stop=0x8c034574u;goto L8c034716; /* bra 0x8c034716 */
L8c034576: *stop=0x8c034576u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c034578; /* cmp/eq #0x5,r0 */
L8c034578: if(!(*sr&1u))goto L8c03457e;goto L8c03457a; /* bf 0x8c03457e */
L8c03457a: *stop=0x8c03457cu;goto L8c034860; /* bra 0x8c034860 */
L8c03457e: *stop=0x8c034580u;goto L8c0348ba; /* bra 0x8c0348ba */
L8c034582: *stop=0x8c034582u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0346a4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c034584; /* mov.l 0x8c0346a4,r2 */
L8c034584: *stop=0x8c034584u;r[6]=r[15];goto L8c034586; /* mov r15,r6 */
L8c034586: *stop=0x8c034586u;r[5]=r[13];goto L8c034588; /* mov r13,r5 */
L8c034588: *stop=0x8c034588u;r[6]+=20u;goto L8c03458a; /* add #0x14,r6 */
L8c03458a: target=r[2];*pr=0x8c03458eu;*stop=0x8c03458cu;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c03458e; /* jsr @r2 */
L8c03458e: *stop=0x8c03458eu;r[0]=20u;goto L8c034590; /* mov #0x14,r0 */
L8c034590: *stop=0x8c034590u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c034592; /* fmov.s @(r0,r15),fr3 */
L8c034592: *stop=0x8c034592u;r[0]=32u;goto L8c034594; /* mov #0x20,r0 */
L8c034594: *stop=0x8c034594u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c034596; /* fmov.s @(r0,r15),fr2 */
L8c034596: *stop=0x8c034596u;r[0]=24u;goto L8c034598; /* mov #0x18,r0 */
L8c034598: *stop=0x8c034598u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c03459a; /* fsub fr3,fr2 */
L8c03459a: *stop=0x8c03459au;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c03459c; /* fmov.s fr2,@r14 */
L8c03459c: *stop=0x8c03459cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c03459e; /* fmov.s @(r0,r15),fr3 */
L8c03459e: *stop=0x8c03459eu;r[0]=36u;goto L8c0345a0; /* mov #0x24,r0 */
L8c0345a0: *stop=0x8c0345a0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0345a2; /* fmov.s @(r0,r15),fr2 */
L8c0345a2: *stop=0x8c0345a2u;r[0]=4u;goto L8c0345a4; /* mov #0x4,r0 */
L8c0345a4: *stop=0x8c0345a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0345a6; /* fsub fr3,fr2 */
L8c0345a6: *stop=0x8c0345a6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0345a8; /* fmov fr2,@(r0,r14) */
L8c0345a8: *stop=0x8c0345a8u;r[0]=28u;goto L8c0345aa; /* mov #0x1c,r0 */
L8c0345aa: *stop=0x8c0345aau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0345ac; /* fmov.s @(r0,r15),fr3 */
L8c0345ac: *stop=0x8c0345acu;r[0]=40u;goto L8c0345ae; /* mov #0x28,r0 */
L8c0345ae: *stop=0x8c0345aeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0345b0; /* fmov.s @(r0,r15),fr2 */
L8c0345b0: *stop=0x8c0345b0u;r[0]=8u;goto L8c0345b2; /* mov #0x8,r0 */
L8c0345b2: *stop=0x8c0345b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0345b4; /* fsub fr3,fr2 */
L8c0345b4: *stop=0x8c0345b4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0345b6; /* fmov fr2,@(r0,r14) */
L8c0345b6: *stop=0x8c0345b6u;r[0]=4u;goto L8c0345b8; /* mov #0x4,r0 */
L8c0345b8: *stop=0x8c0345b8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c0345ba; /* fmov.s @(r0,r14),fr4 */
L8c0345ba: *stop=0x8c0345bau;r[0]=8u;goto L8c0345bc; /* mov #0x8,r0 */
L8c0345bc: *stop=0x8c0345bcu;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c0345be; /* fmov.s @r14,fr5 */
L8c0345be: *stop=0x8c0345beu;fcopy(f,3u,4u);goto L8c0345c0; /* fmov fr4,fr3 */
L8c0345c0: *stop=0x8c0345c0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0345c2; /* fmul fr4,fr3 */
L8c0345c2: *stop=0x8c0345c2u;fcopy(f,0u,5u);goto L8c0345c4; /* fmov fr5,fr0 */
L8c0345c4: *stop=0x8c0345c4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[14]);if(io_status)return io_status;goto L8c0345c6; /* fmov.s @(r0,r14),fr6 */
L8c0345c6: *stop=0x8c0345c6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c0345c8; /* fldi0 fr2 */
L8c0345c8: *stop=0x8c0345c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[5],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0345ca; /* fmac fr0,fr5,fr3 */
L8c0345ca: *stop=0x8c0345cau;fcopy(f,0u,6u);goto L8c0345cc; /* fmov fr6,fr0 */
L8c0345cc: *stop=0x8c0345ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[6],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0345ce; /* fmac fr0,fr6,fr3 */
L8c0345ce: *stop=0x8c0345ceu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,0);if(status)return status;goto L8c0345d0; /* fcmp/eq fr2,fr3 */
L8c0345d0: if((*sr&1u))goto L8c0345d6;goto L8c0345d2; /* bt 0x8c0345d6 */
L8c0345d2: *stop=0x8c0345d4u;goto L8c0348a6; /* bra 0x8c0348a6 */
L8c0345d6: *stop=0x8c0345d6u;r[0]=4u;goto L8c0345d8; /* mov #0x4,r0 */
L8c0345d8: *stop=0x8c0345d8u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[14]);if(io_status)return io_status;goto L8c0345da; /* fmov.s fr15,@r14 */
L8c0345da: *stop=0x8c0345dau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c0345dc; /* fmov fr15,@(r0,r14) */
L8c0345dc: *stop=0x8c0345dcu;r[0]=8u;goto L8c0345de; /* mov #0x8,r0 */
L8c0345de: *stop=0x8c0345e0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[14]);if(io_status)return io_status;goto L8c0348be; /* bra 0x8c0348be */
L8c0345e2: *stop=0x8c0345e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0346a4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0345e4; /* mov.l 0x8c0346a4,r2 */
L8c0345e4: *stop=0x8c0345e4u;r[6]=r[15];goto L8c0345e6; /* mov r15,r6 */
L8c0345e6: *stop=0x8c0345e6u;r[5]=r[13];goto L8c0345e8; /* mov r13,r5 */
L8c0345e8: *stop=0x8c0345e8u;r[6]+=20u;goto L8c0345ea; /* add #0x14,r6 */
L8c0345ea: target=r[2];*pr=0x8c0345eeu;*stop=0x8c0345ecu;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0345ee; /* jsr @r2 */
L8c0345ee: *stop=0x8c0345eeu;r[0]=20u;goto L8c0345f0; /* mov #0x14,r0 */
L8c0345f0: *stop=0x8c0345f0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0345f2; /* fmov.s @(r0,r15),fr3 */
L8c0345f2: *stop=0x8c0345f2u;r[0]=32u;goto L8c0345f4; /* mov #0x20,r0 */
L8c0345f4: *stop=0x8c0345f4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0345f6; /* fmov.s @(r0,r15),fr2 */
L8c0345f6: *stop=0x8c0345f6u;r[0]=4u;goto L8c0345f8; /* mov #0x4,r0 */
L8c0345f8: *stop=0x8c0345f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0345fa; /* fsub fr3,fr2 */
L8c0345fa: *stop=0x8c0345fau;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c0345fc; /* fmov.s fr2,@r14 */
L8c0345fc: *stop=0x8c0345fcu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c0345fe; /* fmov fr15,@(r0,r14) */
L8c0345fe: *stop=0x8c0345feu;r[0]=28u;goto L8c034600; /* mov #0x1c,r0 */
L8c034600: *stop=0x8c034600u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c034602; /* fmov.s @(r0,r15),fr3 */
L8c034602: *stop=0x8c034602u;r[0]=40u;goto L8c034604; /* mov #0x28,r0 */
L8c034604: *stop=0x8c034604u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c034606; /* fmov.s @(r0,r15),fr2 */
L8c034606: *stop=0x8c034606u;r[0]=8u;goto L8c034608; /* mov #0x8,r0 */
L8c034608: *stop=0x8c034608u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c03460a; /* fsub fr3,fr2 */
L8c03460a: *stop=0x8c03460au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c03460c; /* fmov fr2,@(r0,r14) */
L8c03460c: *stop=0x8c03460cu;r[0]=4u;goto L8c03460e; /* mov #0x4,r0 */
L8c03460e: *stop=0x8c03460eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[14]);if(io_status)return io_status;goto L8c034610; /* fmov.s @(r0,r14),fr6 */
L8c034610: *stop=0x8c034610u;r[0]=8u;goto L8c034612; /* mov #0x8,r0 */
L8c034612: *stop=0x8c034612u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[14]);if(io_status)return io_status;goto L8c034614; /* fmov.s @r14,fr5 */
L8c034614: *stop=0x8c034614u;fcopy(f,3u,6u);goto L8c034616; /* fmov fr6,fr3 */
L8c034616: *stop=0x8c034616u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c034618; /* fmul fr6,fr3 */
L8c034618: *stop=0x8c034618u;fcopy(f,0u,5u);goto L8c03461a; /* fmov fr5,fr0 */
L8c03461a: *stop=0x8c03461au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c03461c; /* fmov.s @(r0,r14),fr4 */
L8c03461c: *stop=0x8c03461cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c03461e; /* fldi0 fr2 */
L8c03461e: *stop=0x8c03461eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[5],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c034620; /* fmac fr0,fr5,fr3 */
L8c034620: *stop=0x8c034620u;fcopy(f,0u,4u);goto L8c034622; /* fmov fr4,fr0 */
L8c034622: *stop=0x8c034622u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c034624; /* fmac fr0,fr4,fr3 */
L8c034624: *stop=0x8c034624u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,0);if(status)return status;goto L8c034626; /* fcmp/eq fr2,fr3 */
L8c034626: if((*sr&1u))goto L8c03462c;goto L8c034628; /* bt 0x8c03462c */
L8c034628: *stop=0x8c03462au;goto L8c0348a6; /* bra 0x8c0348a6 */
L8c03462c: *stop=0x8c03462cu;r[0]=4u;goto L8c03462e; /* mov #0x4,r0 */
L8c03462e: *stop=0x8c03462eu;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[14]);if(io_status)return io_status;goto L8c034630; /* fmov.s fr15,@r14 */
L8c034630: *stop=0x8c034630u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c034632; /* fmov fr15,@(r0,r14) */
L8c034632: *stop=0x8c034632u;r[0]=8u;goto L8c034634; /* mov #0x8,r0 */
L8c034634: *stop=0x8c034636u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[14]);if(io_status)return io_status;goto L8c0348be; /* bra 0x8c0348be */
L8c034638: *stop=0x8c034638u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0346b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c03463a; /* mov.l 0x8c0346b0,r2 */
L8c03463a: *stop=0x8c03463au;r[6]=r[15];goto L8c03463c; /* mov r15,r6 */
L8c03463c: *stop=0x8c03463cu;r[5]=r[13];goto L8c03463e; /* mov r13,r5 */
L8c03463e: *stop=0x8c03463eu;r[6]+=20u;goto L8c034640; /* add #0x14,r6 */
L8c034640: target=r[2];*pr=0x8c034644u;*stop=0x8c034642u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c034644; /* jsr @r2 */
L8c034644: *stop=0x8c034644u;r[0]=20u;goto L8c034646; /* mov #0x14,r0 */
L8c034646: *stop=0x8c034646u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c034648; /* fmov.s @(r0,r15),fr3 */
L8c034648: *stop=0x8c034648u;r[0]=32u;goto L8c03464a; /* mov #0x20,r0 */
L8c03464a: *stop=0x8c03464au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c03464c; /* fmov.s @(r0,r15),fr2 */
L8c03464c: *stop=0x8c03464cu;r[0]=4u;goto L8c03464e; /* mov #0x4,r0 */
L8c03464e: *stop=0x8c03464eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c034650; /* fsub fr3,fr2 */
L8c034650: *stop=0x8c034650u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c034652; /* fmov.s fr2,@r14 */
L8c034652: *stop=0x8c034652u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c034654; /* fmov fr15,@(r0,r14) */
L8c034654: *stop=0x8c034654u;r[0]=28u;goto L8c034656; /* mov #0x1c,r0 */
L8c034656: *stop=0x8c034656u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c034658; /* fmov.s @(r0,r15),fr3 */
L8c034658: *stop=0x8c034658u;r[0]=40u;goto L8c03465a; /* mov #0x28,r0 */
L8c03465a: *stop=0x8c03465au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c03465c; /* fmov.s @(r0,r15),fr2 */
L8c03465c: *stop=0x8c03465cu;r[0]=8u;goto L8c03465e; /* mov #0x8,r0 */
L8c03465e: *stop=0x8c03465eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c034660; /* fsub fr3,fr2 */
L8c034660: *stop=0x8c034660u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c034662; /* fmov fr2,@(r0,r14) */
L8c034662: *stop=0x8c034662u;r[0]=4u;goto L8c034664; /* mov #0x4,r0 */
L8c034664: *stop=0x8c034664u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c034666; /* fmov.s @(r0,r14),fr5 */
L8c034666: *stop=0x8c034666u;r[0]=8u;goto L8c034668; /* mov #0x8,r0 */
L8c034668: *stop=0x8c034668u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c03466a; /* fmov.s @r14,fr6 */
L8c03466a: *stop=0x8c03466au;fcopy(f,3u,5u);goto L8c03466c; /* fmov fr5,fr3 */
L8c03466c: *stop=0x8c03466cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c03466e; /* fmul fr5,fr3 */
L8c03466e: *stop=0x8c03466eu;fcopy(f,0u,6u);goto L8c034670; /* fmov fr6,fr0 */
L8c034670: *stop=0x8c034670u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c034672; /* fmov.s @(r0,r14),fr4 */
L8c034672: *stop=0x8c034672u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c034674; /* fldi0 fr2 */
L8c034674: *stop=0x8c034674u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[6],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c034676; /* fmac fr0,fr6,fr3 */
L8c034676: *stop=0x8c034676u;fcopy(f,0u,4u);goto L8c034678; /* fmov fr4,fr0 */
L8c034678: *stop=0x8c034678u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c03467a; /* fmac fr0,fr4,fr3 */
L8c03467a: *stop=0x8c03467au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,0);if(status)return status;goto L8c03467c; /* fcmp/eq fr2,fr3 */
L8c03467c: if((*sr&1u))goto L8c034682;goto L8c03467e; /* bt 0x8c034682 */
L8c03467e: *stop=0x8c034680u;goto L8c0348a6; /* bra 0x8c0348a6 */
L8c034682: *stop=0x8c034682u;r[0]=4u;goto L8c034684; /* mov #0x4,r0 */
L8c034684: *stop=0x8c034684u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[14]);if(io_status)return io_status;goto L8c034686; /* fmov.s fr15,@r14 */
L8c034686: *stop=0x8c034686u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c034688; /* fmov fr15,@(r0,r14) */
L8c034688: *stop=0x8c034688u;r[0]=8u;goto L8c03468a; /* mov #0x8,r0 */
L8c03468a: *stop=0x8c03468cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[14]);if(io_status)return io_status;goto L8c0348be; /* bra 0x8c0348be */
L8c0346c0: *stop=0x8c0346c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c034820u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0346c2; /* mov.l 0x8c034820,r2 */
L8c0346c2: *stop=0x8c0346c2u;r[6]=r[15];goto L8c0346c4; /* mov r15,r6 */
L8c0346c4: *stop=0x8c0346c4u;r[5]=r[13];goto L8c0346c6; /* mov r13,r5 */
L8c0346c6: *stop=0x8c0346c6u;r[6]+=20u;goto L8c0346c8; /* add #0x14,r6 */
L8c0346c8: target=r[2];*pr=0x8c0346ccu;*stop=0x8c0346cau;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0346cc; /* jsr @r2 */
L8c0346cc: *stop=0x8c0346ccu;r[0]=20u;goto L8c0346ce; /* mov #0x14,r0 */
L8c0346ce: *stop=0x8c0346ceu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0346d0; /* fmov.s @(r0,r15),fr3 */
L8c0346d0: *stop=0x8c0346d0u;r[0]=32u;goto L8c0346d2; /* mov #0x20,r0 */
L8c0346d2: *stop=0x8c0346d2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0346d4; /* fmov.s @(r0,r15),fr2 */
L8c0346d4: *stop=0x8c0346d4u;r[0]=4u;goto L8c0346d6; /* mov #0x4,r0 */
L8c0346d6: *stop=0x8c0346d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0346d8; /* fsub fr3,fr2 */
L8c0346d8: *stop=0x8c0346d8u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c0346da; /* fmov.s fr2,@r14 */
L8c0346da: *stop=0x8c0346dau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c0346dc; /* fmov fr15,@(r0,r14) */
L8c0346dc: *stop=0x8c0346dcu;r[0]=28u;goto L8c0346de; /* mov #0x1c,r0 */
L8c0346de: *stop=0x8c0346deu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0346e0; /* fmov.s @(r0,r15),fr3 */
L8c0346e0: *stop=0x8c0346e0u;r[0]=40u;goto L8c0346e2; /* mov #0x28,r0 */
L8c0346e2: *stop=0x8c0346e2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0346e4; /* fmov.s @(r0,r15),fr2 */
L8c0346e4: *stop=0x8c0346e4u;r[0]=8u;goto L8c0346e6; /* mov #0x8,r0 */
L8c0346e6: *stop=0x8c0346e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0346e8; /* fsub fr3,fr2 */
L8c0346e8: *stop=0x8c0346e8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0346ea; /* fmov fr2,@(r0,r14) */
L8c0346ea: *stop=0x8c0346eau;r[0]=4u;goto L8c0346ec; /* mov #0x4,r0 */
L8c0346ec: *stop=0x8c0346ecu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c0346ee; /* fmov.s @(r0,r14),fr5 */
L8c0346ee: *stop=0x8c0346eeu;r[0]=8u;goto L8c0346f0; /* mov #0x8,r0 */
L8c0346f0: *stop=0x8c0346f0u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[14]);if(io_status)return io_status;goto L8c0346f2; /* fmov.s @r14,fr6 */
L8c0346f2: *stop=0x8c0346f2u;fcopy(f,3u,5u);goto L8c0346f4; /* fmov fr5,fr3 */
L8c0346f4: *stop=0x8c0346f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0346f6; /* fmul fr5,fr3 */
L8c0346f6: *stop=0x8c0346f6u;fcopy(f,0u,6u);goto L8c0346f8; /* fmov fr6,fr0 */
L8c0346f8: *stop=0x8c0346f8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c0346fa; /* fmov.s @(r0,r14),fr4 */
L8c0346fa: *stop=0x8c0346fau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c0346fc; /* fldi0 fr2 */
L8c0346fc: *stop=0x8c0346fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[6],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0346fe; /* fmac fr0,fr6,fr3 */
L8c0346fe: *stop=0x8c0346feu;fcopy(f,0u,4u);goto L8c034700; /* fmov fr4,fr0 */
L8c034700: *stop=0x8c034700u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c034702; /* fmac fr0,fr4,fr3 */
L8c034702: *stop=0x8c034702u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,0);if(status)return status;goto L8c034704; /* fcmp/eq fr2,fr3 */
L8c034704: if((*sr&1u))goto L8c03470a;goto L8c034706; /* bt 0x8c03470a */
L8c034706: *stop=0x8c034708u;goto L8c0348a6; /* bra 0x8c0348a6 */
L8c03470a: *stop=0x8c03470au;r[0]=4u;goto L8c03470c; /* mov #0x4,r0 */
L8c03470c: *stop=0x8c03470cu;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[14]);if(io_status)return io_status;goto L8c03470e; /* fmov.s fr15,@r14 */
L8c03470e: *stop=0x8c03470eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c034710; /* fmov fr15,@(r0,r14) */
L8c034710: *stop=0x8c034710u;r[0]=8u;goto L8c034712; /* mov #0x8,r0 */
L8c034712: *stop=0x8c034714u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[14]);if(io_status)return io_status;goto L8c0348be; /* bra 0x8c0348be */
L8c034716: *stop=0x8c034716u;r[0]=32u;goto L8c034718; /* mov #0x20,r0 */
L8c034718: *stop=0x8c034718u;r[6]=r[15];goto L8c03471a; /* mov r15,r6 */
L8c03471a: *stop=0x8c03471au;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c03471c; /* fmov.s @(r0,r9),fr3 */
L8c03471c: *stop=0x8c03471cu;r[0]=32u;goto L8c03471e; /* mov #0x20,r0 */
L8c03471e: *stop=0x8c03471eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c034824u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c034720; /* mov.l 0x8c034824,r3 */
L8c034720: *stop=0x8c034720u;r[5]=r[13];goto L8c034722; /* mov r13,r5 */
L8c034722: *stop=0x8c034722u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c034724; /* fmov fr3,@(r0,r15) */
L8c034724: *stop=0x8c034724u;r[0]=36u;goto L8c034726; /* mov #0x24,r0 */
L8c034726: *stop=0x8c034726u;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c034728; /* fmov.s @(r0,r9),fr3 */
L8c034728: *stop=0x8c034728u;r[0]=36u;goto L8c03472a; /* mov #0x24,r0 */
L8c03472a: *stop=0x8c03472au;r[6]+=20u;goto L8c03472c; /* add #0x14,r6 */
L8c03472c: *stop=0x8c03472cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c03472e; /* fmov fr3,@(r0,r15) */
L8c03472e: *stop=0x8c03472eu;r[0]=40u;goto L8c034730; /* mov #0x28,r0 */
L8c034730: *stop=0x8c034730u;if(!valid_access(r[0]+r[9],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c034732; /* fmov.s @(r0,r9),fr3 */
L8c034732: *stop=0x8c034732u;r[0]=40u;goto L8c034734; /* mov #0x28,r0 */
L8c034734: *stop=0x8c034734u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c034736; /* fmov fr3,@(r0,r15) */
L8c034736: target=r[3];*pr=0x8c03473au;*stop=0x8c034738u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c03473a; /* jsr @r3 */
L8c03473a: *stop=0x8c03473au;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}goto L8c03473c; /* mov.l @r13,r4 */
L8c03473c: *stop=0x8c03473cu;r[2]=32u;goto L8c03473e; /* mov #0x20,r2 */
L8c03473e: *stop=0x8c03473eu;*sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c034740; /* tst r4,r2 */
L8c034740: if(!(*sr&1u))goto L8c03474a;goto L8c034742; /* bf 0x8c03474a */
L8c034742: *stop=0x8c034742u;r[1]=32u;goto L8c034744; /* mov #0x20,r1 */
L8c034744: *stop=0x8c034744u;r[1]<<=8;goto L8c034746; /* shll8 r1 */
L8c034746: *stop=0x8c034746u;*sr=(*sr&~1u)|!(r[4]&r[1]);goto L8c034748; /* tst r4,r1 */
L8c034748: if((*sr&1u))goto L8c034750;goto L8c03474a; /* bt 0x8c034750 */
L8c03474a: *stop=0x8c03474au;if(!valid_access(r[13]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x24u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c03474c; /* mov.l @(0x24,r13),r11 */
L8c03474c: *stop=0x8c03474eu;r[11]=0u-r[11];goto L8c034758; /* bra 0x8c034758 */
L8c034750: *stop=0x8c034750u;if(!valid_access(r[10]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x18u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c034752; /* mov.l @(0x18,r10),r11 */
L8c034752: *stop=0x8c034752u;if(!valid_access(r[13]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c034754; /* mov.l @(0x24,r13),r3 */
L8c034754: *stop=0x8c034754u;r[11]=0u-r[11];goto L8c034756; /* neg r11,r11 */
L8c034756: *stop=0x8c034756u;r[11]-=r[3];goto L8c034758; /* sub r3,r11 */
L8c034758: *stop=0x8c034758u;r[5]=r[15];goto L8c03475a; /* mov r15,r5 */
L8c03475a: *stop=0x8c03475au;{uint32_t loaded=rd(m,bus,&io_status,0x8c034828u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c03475c; /* mov.l 0x8c034828,r2 */
L8c03475c: *stop=0x8c03475cu;r[7]=r[15];goto L8c03475e; /* mov r15,r7 */
L8c03475e: *stop=0x8c03475eu;r[4]=r[15];goto L8c034760; /* mov r15,r4 */
L8c034760: *stop=0x8c034760u;r[5]+=20u;goto L8c034762; /* add #0x14,r5 */
L8c034762: *stop=0x8c034762u;r[6]=r[11];goto L8c034764; /* mov r11,r6 */
L8c034764: *stop=0x8c034764u;r[7]+=8u;goto L8c034766; /* add #0x8,r7 */
L8c034766: target=r[2];*pr=0x8c03476au;*stop=0x8c034768u;r[4]+=32u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c03476a; /* jsr @r2 */
L8c03476a: *stop=0x8c03476au;fcopy(f,3u,13u);goto L8c03476c; /* fmov fr13,fr3 */
L8c03476c: *stop=0x8c03476cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[12],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c03476e; /* fadd fr12,fr3 */
L8c03476e: *stop=0x8c03476eu;r[0]=16u;goto L8c034770; /* mov #0x10,r0 */
L8c034770: *stop=0x8c034770u;{uint32_t loaded=rd(m,bus,&io_status,0x8c03482cu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c034772; /* mov.l 0x8c03482c,r10 */
L8c034772: *stop=0x8c034772u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c034774; /* fmov.s @(r0,r15),fr2 */
L8c034774: *stop=0x8c034774u;r[0]=4u;goto L8c034776; /* mov #0x4,r0 */
L8c034776: *stop=0x8c034776u;{uint32_t loaded=rd(m,bus,&io_status,0x8c034830u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c034778; /* mov.l 0x8c034830,r13 */
L8c034778: *stop=0x8c034778u;fcopy(f,4u,3u);goto L8c03477a; /* fmov fr3,fr4 */
L8c03477a: *stop=0x8c03477au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c03477c; /* fmul fr2,fr4 */
L8c03477c: *stop=0x8c03477cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c03477e; /* fmov.s @(r0,r15),fr3 */
L8c03477e: *stop=0x8c03477eu;r[0]=8u;goto L8c034780; /* mov #0x8,r0 */
L8c034780: *stop=0x8c034780u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c034782; /* fmov.s @(r0,r15),fr2 */
L8c034782: *stop=0x8c034782u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[3],&f->fpscr,&fr[13],0);if(status)return status;}goto L8c034784; /* fadd fr3,fr13 */
L8c034784: *stop=0x8c034784u;fcopy(f,5u,4u);goto L8c034786; /* fmov fr4,fr5 */
L8c034786: *stop=0x8c034786u;fcopy(f,6u,13u);goto L8c034788; /* fmov fr13,fr6 */
L8c034788: *stop=0x8c034788u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[2],&f->fpscr,&fr[6]);if(status)return status;}goto L8c03478a; /* fmul fr2,fr6 */
L8c03478a: *stop=0x8c03478au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c03478c; /* fldi0 fr2 */
L8c03478c: *stop=0x8c03478cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[6],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c03478e; /* fsub fr6,fr5 */
L8c03478e: *stop=0x8c03478eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][5],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c034790; /* fcmp/gt fr2,fr5 */
L8c034790: take=!(*sr&1u);*stop=0x8c034792u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[6],&f->fpscr,&fr[4],0);if(status)return status;}if(take)goto L8c0347bc;goto L8c034794; /* bf/s 0x8c0347bc */
L8c034794: *stop=0x8c034794u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c034796; /* fcmp/gt fr2,fr4 */
L8c034796: if(!(*sr&1u))goto L8c0347aa;goto L8c034798; /* bf 0x8c0347aa */
L8c034798: *stop=0x8c034798u;if(!valid_access(r[12],1u))return -3;r[3]=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;goto L8c03479a; /* mov.b @r12,r3 */
L8c03479a: *stop=0x8c03479au;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c03479c; /* cmp/pz r3 */
L8c03479c: take=!(*sr&1u);*stop=0x8c03479eu;r[9]=0u;if(take)goto L8c0347e2;goto L8c0347a0; /* bf/s 0x8c0347e2 */
L8c0347a0: *stop=0x8c0347a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c034834u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0347a2; /* mov.l 0x8c034834,r5 */
L8c0347a2: target=r[10];*pr=0x8c0347a6u;*stop=0x8c0347a4u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0347a6; /* jsr @r10 */
L8c0347a6: *stop=0x8c0347a8u;goto L8c0347e2; /* bra 0x8c0347e2 */
L8c0347aa: *stop=0x8c0347aau;if(!valid_access(r[12],1u))return -3;r[2]=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;goto L8c0347ac; /* mov.b @r12,r2 */
L8c0347ac: *stop=0x8c0347acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c034838u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c0347ae; /* mov.l 0x8c034838,r9 */
L8c0347ae: *stop=0x8c0347aeu;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c0347b0; /* cmp/pz r2 */
L8c0347b0: if(!(*sr&1u))goto L8c0347e2;goto L8c0347b2; /* bf 0x8c0347e2 */
L8c0347b2: *stop=0x8c0347b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c03483cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0347b4; /* mov.l 0x8c03483c,r5 */
L8c0347b4: target=r[10];*pr=0x8c0347b8u;*stop=0x8c0347b6u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0347b8; /* jsr @r10 */
L8c0347b8: *stop=0x8c0347bau;goto L8c0347e2; /* bra 0x8c0347e2 */
L8c0347bc: *stop=0x8c0347bcu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c0347be; /* fcmp/gt fr2,fr4 */
L8c0347be: if(!(*sr&1u))goto L8c0347d4;goto L8c0347c0; /* bf 0x8c0347d4 */
L8c0347c0: *stop=0x8c0347c0u;if(!valid_access(r[12],1u))return -3;r[2]=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;goto L8c0347c2; /* mov.b @r12,r2 */
L8c0347c2: *stop=0x8c0347c2u;r[9]=64u;goto L8c0347c4; /* mov #0x40,r9 */
L8c0347c4: *stop=0x8c0347c4u;r[9]<<=8;goto L8c0347c6; /* shll8 r9 */
L8c0347c6: *stop=0x8c0347c6u;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c0347c8; /* cmp/pz r2 */
L8c0347c8: if(!(*sr&1u))goto L8c0347e2;goto L8c0347ca; /* bf 0x8c0347e2 */
L8c0347ca: *stop=0x8c0347cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c034840u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0347cc; /* mov.l 0x8c034840,r5 */
L8c0347cc: target=r[10];*pr=0x8c0347d0u;*stop=0x8c0347ceu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0347d0; /* jsr @r10 */
L8c0347d0: *stop=0x8c0347d2u;goto L8c0347e2; /* bra 0x8c0347e2 */
L8c0347d4: *stop=0x8c0347d4u;if(!valid_access(r[12],1u))return -3;r[3]=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;goto L8c0347d6; /* mov.b @r12,r3 */
L8c0347d6: *stop=0x8c0347d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c034844u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c0347d8; /* mov.l 0x8c034844,r9 */
L8c0347d8: *stop=0x8c0347d8u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c0347da; /* cmp/pz r3 */
L8c0347da: if(!(*sr&1u))goto L8c0347e2;goto L8c0347dc; /* bf 0x8c0347e2 */
L8c0347dc: *stop=0x8c0347dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c034848u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0347de; /* mov.l 0x8c034848,r5 */
L8c0347de: target=r[10];*pr=0x8c0347e2u;*stop=0x8c0347e0u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0347e2; /* jsr @r10 */
L8c0347e2: *stop=0x8c0347e2u;r[0]=8u;goto L8c0347e4; /* mov #0x8,r0 */
L8c0347e4: *stop=0x8c0347e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c03484cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0347e6; /* mov.l 0x8c03484c,r4 */
L8c0347e6: *stop=0x8c0347e6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c0347e8; /* fmov fr15,@(r0,r15) */
L8c0347e8: *stop=0x8c0347e8u;r[0]=12u;goto L8c0347ea; /* mov #0xc,r0 */
L8c0347ea: *stop=0x8c0347eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c034850u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0347ec; /* mov.l 0x8c034850,r3 */
L8c0347ec: *stop=0x8c0347ecu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c0347ee; /* fmov fr15,@(r0,r15) */
L8c0347ee: *stop=0x8c0347eeu;r[0]=16u;goto L8c0347f0; /* mov #0x10,r0 */
L8c0347f0: target=r[3];*pr=0x8c0347f4u;*stop=0x8c0347f2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[15]);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0347f4; /* jsr @r3 */
L8c0347f4: *stop=0x8c0347f4u;r[9]-=r[11];goto L8c0347f6; /* sub r11,r9 */
L8c0347f6: *stop=0x8c0347f6u;*sr=(*sr&~1u)|!(r[9]&r[9]);goto L8c0347f8; /* tst r9,r9 */
L8c0347f8: take=(*sr&1u);*stop=0x8c0347fau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;if(take)goto L8c034804;goto L8c0347fc; /* bt/s 0x8c034804 */
L8c0347fc: *stop=0x8c0347fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c034854u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0347fe; /* mov.l 0x8c034854,r3 */
L8c0347fe: *stop=0x8c0347feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c034800; /* mov.l @r15,r5 */
L8c034800: target=r[3];*pr=0x8c034804u;*stop=0x8c034802u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c034804; /* jsr @r3 */
L8c034804: *stop=0x8c034804u;{uint32_t loaded=rd(m,bus,&io_status,0x8c034858u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c034806; /* mov.l 0x8c034858,r3 */
L8c034806: *stop=0x8c034806u;r[5]=r[15];goto L8c034808; /* mov r15,r5 */
L8c034808: *stop=0x8c034808u;r[6]=r[14];goto L8c03480a; /* mov r14,r6 */
L8c03480a: *stop=0x8c03480au;r[5]+=8u;goto L8c03480c; /* add #0x8,r5 */
L8c03480c: target=r[3];*pr=0x8c034810u;*stop=0x8c03480eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c034810; /* jsr @r3 */
L8c034810: *stop=0x8c034810u;{uint32_t loaded=rd(m,bus,&io_status,0x8c03485cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c034812; /* mov.l 0x8c03485c,r2 */
L8c034812: target=r[2];*pr=0x8c034816u;*stop=0x8c034814u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c034816; /* jsr @r2 */
L8c034816: *stop=0x8c034818u;goto L8c0348be; /* bra 0x8c0348be */
L8c034860: *stop=0x8c034860u;{uint32_t loaded=rd(m,bus,&io_status,0x8c034920u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c034862; /* mov.l 0x8c034920,r2 */
L8c034862: *stop=0x8c034862u;r[6]=r[15];goto L8c034864; /* mov r15,r6 */
L8c034864: *stop=0x8c034864u;r[5]=r[13];goto L8c034866; /* mov r13,r5 */
L8c034866: *stop=0x8c034866u;r[6]+=20u;goto L8c034868; /* add #0x14,r6 */
L8c034868: target=r[2];*pr=0x8c03486cu;*stop=0x8c03486au;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c03486c; /* jsr @r2 */
L8c03486c: *stop=0x8c03486cu;r[0]=20u;goto L8c03486e; /* mov #0x14,r0 */
L8c03486e: *stop=0x8c03486eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c034870; /* fmov.s @(r0,r15),fr3 */
L8c034870: *stop=0x8c034870u;r[0]=32u;goto L8c034872; /* mov #0x20,r0 */
L8c034872: *stop=0x8c034872u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c034874; /* fmov.s @(r0,r15),fr2 */
L8c034874: *stop=0x8c034874u;r[0]=4u;goto L8c034876; /* mov #0x4,r0 */
L8c034876: *stop=0x8c034876u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c034878; /* fsub fr3,fr2 */
L8c034878: *stop=0x8c034878u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c03487a; /* fmov.s fr2,@r14 */
L8c03487a: *stop=0x8c03487au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c03487c; /* fmov fr15,@(r0,r14) */
L8c03487c: *stop=0x8c03487cu;r[0]=28u;goto L8c03487e; /* mov #0x1c,r0 */
L8c03487e: *stop=0x8c03487eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c034880; /* fmov.s @(r0,r15),fr3 */
L8c034880: *stop=0x8c034880u;r[0]=40u;goto L8c034882; /* mov #0x28,r0 */
L8c034882: *stop=0x8c034882u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c034884; /* fmov.s @(r0,r15),fr2 */
L8c034884: *stop=0x8c034884u;r[0]=8u;goto L8c034886; /* mov #0x8,r0 */
L8c034886: *stop=0x8c034886u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c034888; /* fsub fr3,fr2 */
L8c034888: *stop=0x8c034888u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c03488a; /* fmov fr2,@(r0,r14) */
L8c03488a: *stop=0x8c03488au;r[0]=4u;goto L8c03488c; /* mov #0x4,r0 */
L8c03488c: *stop=0x8c03488cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c03488e; /* fmov.s @(r0,r14),fr5 */
L8c03488e: *stop=0x8c03488eu;r[0]=8u;goto L8c034890; /* mov #0x8,r0 */
L8c034890: *stop=0x8c034890u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[14]);if(io_status)return io_status;goto L8c034892; /* fmov.s @r14,fr4 */
L8c034892: *stop=0x8c034892u;fcopy(f,3u,5u);goto L8c034894; /* fmov fr5,fr3 */
L8c034894: *stop=0x8c034894u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c034896; /* fmul fr5,fr3 */
L8c034896: *stop=0x8c034896u;fcopy(f,0u,4u);goto L8c034898; /* fmov fr4,fr0 */
L8c034898: *stop=0x8c034898u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]+r[14]);if(io_status)return io_status;goto L8c03489a; /* fmov.s @(r0,r14),fr6 */
L8c03489a: *stop=0x8c03489au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c03489c; /* fldi0 fr2 */
L8c03489c: *stop=0x8c03489cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c03489e; /* fmac fr0,fr4,fr3 */
L8c03489e: *stop=0x8c03489eu;fcopy(f,0u,6u);goto L8c0348a0; /* fmov fr6,fr0 */
L8c0348a0: *stop=0x8c0348a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[6],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0348a2; /* fmac fr0,fr6,fr3 */
L8c0348a2: *stop=0x8c0348a2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,0);if(status)return status;goto L8c0348a4; /* fcmp/eq fr2,fr3 */
L8c0348a4: if((*sr&1u))goto L8c0348ae;goto L8c0348a6; /* bt 0x8c0348ae */
L8c0348a6: target=r[11];*pr=0x8c0348aau;*stop=0x8c0348a8u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0348aa; /* jsr @r11 */
L8c0348aa: *stop=0x8c0348acu;goto L8c0348be; /* bra 0x8c0348be */
L8c0348ae: *stop=0x8c0348aeu;r[0]=4u;goto L8c0348b0; /* mov #0x4,r0 */
L8c0348b0: *stop=0x8c0348b0u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[14]);if(io_status)return io_status;goto L8c0348b2; /* fmov.s fr15,@r14 */
L8c0348b2: *stop=0x8c0348b2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c0348b4; /* fmov fr15,@(r0,r14) */
L8c0348b4: *stop=0x8c0348b4u;r[0]=8u;goto L8c0348b6; /* mov #0x8,r0 */
L8c0348b6: *stop=0x8c0348b8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,14u,r[0]+r[14]);if(io_status)return io_status;goto L8c0348be; /* bra 0x8c0348be */
L8c0348ba: *stop=0x8c0348bcu;r[0]=0u;goto L8c0348f2; /* bra 0x8c0348f2 */
L8c0348be: *stop=0x8c0348beu;if(!valid_access(r[12],1u))return -3;r[2]=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;goto L8c0348c0; /* mov.b @r12,r2 */
L8c0348c0: *stop=0x8c0348c0u;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c0348c2; /* cmp/pz r2 */
L8c0348c2: if(!(*sr&1u))goto L8c0348f0;goto L8c0348c4; /* bf 0x8c0348f0 */
L8c0348c4: *stop=0x8c0348c4u;r[0]=0x8c034924u;goto L8c0348c6; /* mova 0x8c034924,r0 */
L8c0348c6: *stop=0x8c0348c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c034928u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0348c8; /* mov.l 0x8c034928,r3 */
L8c0348c8: *stop=0x8c0348c8u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c0348ca; /* fmov.s @r0,fr14 */
L8c0348ca: *stop=0x8c0348cau;r[0]=28u;goto L8c0348cc; /* mov #0x1c,r0 */
L8c0348cc: *stop=0x8c0348ccu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c0348ce; /* fmov.s @(r0,r15),fr15 */
L8c0348ce: *stop=0x8c0348ceu;r[0]=8u;goto L8c0348d0; /* mov #0x8,r0 */
L8c0348d0: *stop=0x8c0348d0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0348d2; /* fmov.s @(r0,r14),fr3 */
L8c0348d2: *stop=0x8c0348d2u;r[0]=24u;goto L8c0348d4; /* mov #0x18,r0 */
L8c0348d4: *stop=0x8c0348d4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]+r[15]);if(io_status)return io_status;goto L8c0348d6; /* fmov.s @(r0,r15),fr12 */
L8c0348d6: *stop=0x8c0348d6u;r[0]=20u;goto L8c0348d8; /* mov #0x14,r0 */
L8c0348d8: *stop=0x8c0348d8u;fcopy(f,0u,14u);goto L8c0348da; /* fmov fr14,fr0 */
L8c0348da: *stop=0x8c0348dau;fcopy(f,9u,15u);goto L8c0348dc; /* fmov fr15,fr9 */
L8c0348dc: *stop=0x8c0348dcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[9],&f->fpscr,&fr[9]);if(status)return status;}goto L8c0348de; /* fmac fr0,fr3,fr9 */
L8c0348de: *stop=0x8c0348deu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]+r[15]);if(io_status)return io_status;goto L8c0348e0; /* fmov.s @(r0,r15),fr13 */
L8c0348e0: *stop=0x8c0348e0u;if(!valid_access(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c0348e2; /* fmov.s @r14,fr3 */
L8c0348e2: *stop=0x8c0348e2u;fcopy(f,7u,13u);goto L8c0348e4; /* fmov fr13,fr7 */
L8c0348e4: *stop=0x8c0348e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[7],&f->fpscr,&fr[7]);if(status)return status;}goto L8c0348e6; /* fmac fr0,fr3,fr7 */
L8c0348e6: *stop=0x8c0348e6u;fcopy(f,5u,12u);goto L8c0348e8; /* fmov fr12,fr5 */
L8c0348e8: *stop=0x8c0348e8u;fcopy(f,8u,12u);goto L8c0348ea; /* fmov fr12,fr8 */
L8c0348ea: *stop=0x8c0348eau;fcopy(f,6u,15u);goto L8c0348ec; /* fmov fr15,fr6 */
L8c0348ec: target=r[3];*pr=0x8c0348f0u;*stop=0x8c0348eeu;fcopy(f,4u,13u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0348f0; /* jsr @r3 */
L8c0348f0: *stop=0x8c0348f0u;r[0]=1u;goto L8c0348f2; /* mov #0x1,r0 */
L8c0348f2: *stop=0x8c0348f2u;r[15]+=44u;goto L8c0348f4; /* add #0x2c,r15 */
L8c0348f4: *stop=0x8c0348f4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0348f6; /* lds.l @r15+,PR */
L8c0348f6: *stop=0x8c0348f6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0348f8; /* fmov.s @r15+,fr12 */
L8c0348f8: *stop=0x8c0348f8u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0348fa; /* fmov.s @r15+,fr13 */
L8c0348fa: *stop=0x8c0348fau;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0348fc; /* fmov.s @r15+,fr14 */
L8c0348fc: *stop=0x8c0348fcu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0348fe; /* fmov.s @r15+,fr15 */
L8c0348fe: *stop=0x8c0348feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c034900; /* mov.l @r15+,r9 */
L8c034900: *stop=0x8c034900u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c034902; /* mov.l @r15+,r10 */
L8c034902: *stop=0x8c034902u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c034904; /* mov.l @r15+,r11 */
L8c034904: *stop=0x8c034904u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c034906; /* mov.l @r15+,r12 */
L8c034906: *stop=0x8c034906u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c034908; /* mov.l @r15+,r13 */
L8c034908: *stop=0x8c034908u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c03490au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_movement_dependencies_8c0344dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c03454e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c03454eu);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c03458e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c03458eu);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c0345ee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0345eeu);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c034644(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c034644u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c0346cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0346ccu);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c03473a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c03473au);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c03476a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c03476au);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c0347a6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0347a6u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c0347b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0347b8u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c0347d0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0347d0u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c0347e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0347e2u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c0347f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0347f4u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c034804(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c034804u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c034810(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c034810u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c034816(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c034816u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c03486c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c03486cu);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c0348aa(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0348aau);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c0348f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c0344dc(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0348f0u);}

static int sa_player_movement_dependencies_run_8c072946(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c07296eu)goto L8c07296e;if(resume_pc==0x8c0729a8u)goto L8c0729a8;if(resume_pc==0x8c0729c6u)goto L8c0729c6;if(resume_pc==0x8c072a06u)goto L8c072a06;if(resume_pc==0x8c072a5eu)goto L8c072a5e;if(resume_pc==0x8c072a74u)goto L8c072a74;if(resume_pc==0x8c072a80u)goto L8c072a80;if(resume_pc==0x8c072a8cu)goto L8c072a8c;if(resume_pc==0x8c072ab6u)goto L8c072ab6;if(resume_pc==0x8c072afeu)goto L8c072afe;if(resume_pc==0x8c072b02u)goto L8c072b02;if(resume_pc==0x8c072b10u)goto L8c072b10;if(resume_pc==0x8c072b1eu)goto L8c072b1e;goto L8c072946;
L8c072946: *stop=0x8c072946u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c072948; /* mov.l r14,@-r15 */
L8c072948: *stop=0x8c072948u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c07294a; /* mov.l r13,@-r15 */
L8c07294a: *stop=0x8c07294au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c07294c; /* mov.l r12,@-r15 */
L8c07294c: *stop=0x8c07294cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c07294e; /* mov.l r11,@-r15 */
L8c07294e: *stop=0x8c07294eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c072950; /* mov.l r10,@-r15 */
L8c072950: *stop=0x8c072950u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c072952; /* fmov.s fr15,@-r15 */
L8c072952: *stop=0x8c072952u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c072954; /* sts.l PR,@-r15 */
L8c072954: *stop=0x8c072954u;r[15]+=4294967188u;goto L8c072956; /* add #-0x6c,r15 */
L8c072956: *stop=0x8c072956u;r[13]=r[4];goto L8c072958; /* mov r4,r13 */
L8c072958: *stop=0x8c072958u;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c07295a; /* mov.w @(0x4,r13),r0 */
L8c07295a: *stop=0x8c07295au;r[14]=r[5];goto L8c07295c; /* mov r5,r14 */
L8c07295c: *stop=0x8c07295cu;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c07295e; /* tst #0x3,r0 */
L8c07295e: take=!(*sr&1u);*stop=0x8c072960u;r[12]=r[6];if(take)goto L8c072966;goto L8c072962; /* bf/s 0x8c072966 */
L8c072962: *stop=0x8c072964u;goto L8c072b32; /* bra 0x8c072b32 */
L8c072966: *stop=0x8c072966u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0729f0u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c072968; /* mov.l 0x8c0729f0,r11 */
L8c072968: *stop=0x8c072968u;r[4]=r[14];goto L8c07296a; /* mov r14,r4 */
L8c07296a: target=r[11];*pr=0x8c07296eu;*stop=0x8c07296cu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07296e; /* jsr @r11 */
L8c07296e: *stop=0x8c07296eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c072970; /* fldi1 fr3 */
L8c072970: *stop=0x8c072970u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c072972; /* fcmp/gt fr0,fr3 */
L8c072972: if(!(*sr&1u))goto L8c072978;goto L8c072974; /* bf 0x8c072978 */
L8c072974: *stop=0x8c072976u;goto L8c072b32; /* bra 0x8c072b32 */
L8c072978: *stop=0x8c072978u;if(!valid_access(r[13]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x38u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c07297a; /* mov.l @(0x38,r13),r4 */
L8c07297a: *stop=0x8c07297au;r[3]=4294967168u;goto L8c07297c; /* mov #-0x80,r3 */
L8c07297c: *stop=0x8c07297cu;r[3]=r[3]&255u;goto L8c07297e; /* extu.b r3,r3 */
L8c07297e: *stop=0x8c07297eu;if(!valid_access(r[4]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c072980; /* mov.w @(0x2,r4),r0 */
L8c072980: *stop=0x8c072980u;r[4]=r[0]&65535u;goto L8c072982; /* extu.w r0,r4 */
L8c072982: *stop=0x8c072982u;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c072984; /* tst r4,r3 */
L8c072984: if(!(*sr&1u))goto L8c07298a;goto L8c072986; /* bf 0x8c07298a */
L8c072986: *stop=0x8c072988u;goto L8c072b32; /* bra 0x8c072b32 */
L8c07298a: *stop=0x8c07298au;if(!valid_access(r[13]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c07298c; /* mov.w @(0x4,r13),r0 */
L8c07298c: *stop=0x8c07298cu;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c07298e; /* tst #0x4,r0 */
L8c07298e: if((*sr&1u))goto L8c072994;goto L8c072990; /* bt 0x8c072994 */
L8c072990: *stop=0x8c072992u;goto L8c072b32; /* bra 0x8c072b32 */
L8c072994: *stop=0x8c072994u;r[2]=8u;goto L8c072996; /* mov #0x8,r2 */
L8c072996: *stop=0x8c072996u;*sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c072998; /* tst r4,r2 */
L8c072998: if(!(*sr&1u))goto L8c07299e;goto L8c07299a; /* bf 0x8c07299e */
L8c07299a: *stop=0x8c07299cu;goto L8c072b32; /* bra 0x8c072b32 */
L8c07299e: *stop=0x8c07299eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0729f4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0729a0; /* mov.l 0x8c0729f4,r2 */
L8c0729a0: *stop=0x8c0729a0u;r[5]=r[15];goto L8c0729a2; /* mov r15,r5 */
L8c0729a2: *stop=0x8c0729a2u;r[5]+=96u;goto L8c0729a4; /* add #0x60,r5 */
L8c0729a4: target=r[2];*pr=0x8c0729a8u;*stop=0x8c0729a6u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0729a8; /* jsr @r2 */
L8c0729a8: *stop=0x8c0729a8u;r[3]=r[15];goto L8c0729aa; /* mov r15,r3 */
L8c0729aa: *stop=0x8c0729aau;r[3]+=96u;goto L8c0729ac; /* add #0x60,r3 */
L8c0729ac: *stop=0x8c0729acu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c0729ae; /* mov.l @r3,r1 */
L8c0729ae: *stop=0x8c0729aeu;r[2]=r[12];goto L8c0729b0; /* mov r12,r2 */
L8c0729b0: *stop=0x8c0729b0u;r[2]+=68u;goto L8c0729b2; /* add #0x44,r2 */
L8c0729b2: *stop=0x8c0729b2u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0729b4; /* mov.l @(0x4,r3),r0 */
L8c0729b4: *stop=0x8c0729b4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c0729b6; /* mov.l r1,@r2 */
L8c0729b6: *stop=0x8c0729b6u;r[4]=r[15];goto L8c0729b8; /* mov r15,r4 */
L8c0729b8: *stop=0x8c0729b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0729f8u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c0729ba; /* mov.l 0x8c0729f8,r10 */
L8c0729ba: *stop=0x8c0729bau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;goto L8c0729bc; /* fldi0 fr15 */
L8c0729bc: *stop=0x8c0729bcu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c0729be; /* mov.l r0,@(0x4,r2) */
L8c0729be: *stop=0x8c0729beu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0729c0; /* mov.l @(0x8,r3),r1 */
L8c0729c0: *stop=0x8c0729c0u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c0729c2; /* mov.l r1,@(0x8,r2) */
L8c0729c2: target=r[11];*pr=0x8c0729c6u;*stop=0x8c0729c4u;r[4]+=96u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0729c6; /* jsr @r11 */
L8c0729c6: *stop=0x8c0729c6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0729c8; /* fldi0 fr3 */
L8c0729c8: *stop=0x8c0729c8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c0729ca; /* fcmp/eq fr3,fr0 */
L8c0729ca: if(!(*sr&1u))goto L8c072a00;goto L8c0729cc; /* bf 0x8c072a00 */
L8c0729cc: *stop=0x8c0729ccu;r[0]=96u;goto L8c0729ce; /* mov #0x60,r0 */
L8c0729ce: *stop=0x8c0729ceu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c0729d0; /* fmov fr15,@(r0,r15) */
L8c0729d0: *stop=0x8c0729d0u;r[0]=100u;goto L8c0729d2; /* mov #0x64,r0 */
L8c0729d2: *stop=0x8c0729d2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c0729d4; /* fmov fr15,@(r0,r15) */
L8c0729d4: *stop=0x8c0729d4u;r[0]=104u;goto L8c0729d6; /* mov #0x68,r0 */
L8c0729d6: *stop=0x8c0729d8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a06; /* bra 0x8c072a06 */
L8c072a00: *stop=0x8c072a00u;r[4]=r[15];goto L8c072a02; /* mov r15,r4 */
L8c072a02: target=r[10];*pr=0x8c072a06u;*stop=0x8c072a04u;r[4]+=96u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072a06; /* jsr @r10 */
L8c072a06: *stop=0x8c072a06u;r[0]=72u;goto L8c072a08; /* mov #0x48,r0 */
L8c072a08: *stop=0x8c072a08u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a0a; /* fmov fr15,@(r0,r15) */
L8c072a0a: *stop=0x8c072a0au;r[0]=76u;goto L8c072a0c; /* mov #0x4c,r0 */
L8c072a0c: *stop=0x8c072a0cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a0e; /* fmov fr15,@(r0,r15) */
L8c072a0e: *stop=0x8c072a0eu;r[0]=80u;goto L8c072a10; /* mov #0x50,r0 */
L8c072a10: *stop=0x8c072a10u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a12; /* fmov fr15,@(r0,r15) */
L8c072a12: *stop=0x8c072a12u;r[0]=68u;goto L8c072a14; /* mov #0x44,r0 */
L8c072a14: *stop=0x8c072a14u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c072a16; /* fmov.s @(r0,r12),fr3 */
L8c072a16: *stop=0x8c072a16u;r[0]=84u;goto L8c072a18; /* mov #0x54,r0 */
L8c072a18: *stop=0x8c072a18u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a1a; /* fmov fr3,@(r0,r15) */
L8c072a1a: *stop=0x8c072a1au;r[0]=72u;goto L8c072a1c; /* mov #0x48,r0 */
L8c072a1c: *stop=0x8c072a1cu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c072a1e; /* fmov.s @(r0,r12),fr3 */
L8c072a1e: *stop=0x8c072a1eu;r[0]=88u;goto L8c072a20; /* mov #0x58,r0 */
L8c072a20: *stop=0x8c072a20u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a22; /* fmov fr3,@(r0,r15) */
L8c072a22: *stop=0x8c072a22u;r[0]=76u;goto L8c072a24; /* mov #0x4c,r0 */
L8c072a24: *stop=0x8c072a24u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c072a26; /* fmov.s @(r0,r12),fr3 */
L8c072a26: *stop=0x8c072a26u;r[0]=92u;goto L8c072a28; /* mov #0x5c,r0 */
L8c072a28: *stop=0x8c072a28u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a2a; /* fmov fr3,@(r0,r15) */
L8c072a2a: *stop=0x8c072a2au;r[0]=48u;goto L8c072a2c; /* mov #0x30,r0 */
L8c072a2c: *stop=0x8c072a2cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a2e; /* fmov fr15,@(r0,r15) */
L8c072a2e: *stop=0x8c072a2eu;r[0]=52u;goto L8c072a30; /* mov #0x34,r0 */
L8c072a30: *stop=0x8c072a30u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a32; /* fmov fr15,@(r0,r15) */
L8c072a32: *stop=0x8c072a32u;r[0]=56u;goto L8c072a34; /* mov #0x38,r0 */
L8c072a34: *stop=0x8c072a34u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a36; /* fmov fr15,@(r0,r15) */
L8c072a36: *stop=0x8c072a36u;r[0]=80u;goto L8c072a38; /* mov #0x50,r0 */
L8c072a38: *stop=0x8c072a38u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c072a3a; /* fmov.s @(r0,r12),fr3 */
L8c072a3a: *stop=0x8c072a3au;r[0]=60u;goto L8c072a3c; /* mov #0x3c,r0 */
L8c072a3c: *stop=0x8c072a3cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a3e; /* fmov fr3,@(r0,r15) */
L8c072a3e: *stop=0x8c072a3eu;r[0]=84u;goto L8c072a40; /* mov #0x54,r0 */
L8c072a40: *stop=0x8c072a40u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c072a42; /* fmov.s @(r0,r12),fr3 */
L8c072a42: *stop=0x8c072a42u;r[0]=64u;goto L8c072a44; /* mov #0x40,r0 */
L8c072a44: *stop=0x8c072a44u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a46; /* fmov fr3,@(r0,r15) */
L8c072a46: *stop=0x8c072a46u;r[0]=88u;goto L8c072a48; /* mov #0x58,r0 */
L8c072a48: *stop=0x8c072a48u;r[5]=r[15];goto L8c072a4a; /* mov r15,r5 */
L8c072a4a: *stop=0x8c072a4au;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c072a4c; /* fmov.s @(r0,r12),fr3 */
L8c072a4c: *stop=0x8c072a4cu;r[6]=r[15];goto L8c072a4e; /* mov r15,r6 */
L8c072a4e: *stop=0x8c072a4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c072c60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072a50; /* mov.l 0x8c072c60,r3 */
L8c072a50: *stop=0x8c072a50u;r[0]=68u;goto L8c072a52; /* mov #0x44,r0 */
L8c072a52: *stop=0x8c072a52u;r[4]=r[15];goto L8c072a54; /* mov r15,r4 */
L8c072a54: *stop=0x8c072a54u;r[5]+=48u;goto L8c072a56; /* add #0x30,r5 */
L8c072a56: *stop=0x8c072a56u;r[6]+=24u;goto L8c072a58; /* add #0x18,r6 */
L8c072a58: *stop=0x8c072a58u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a5a; /* fmov fr3,@(r0,r15) */
L8c072a5a: target=r[3];*pr=0x8c072a5eu;*stop=0x8c072a5cu;r[4]+=72u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072a5e; /* jsr @r3 */
L8c072a5e: *stop=0x8c072a5eu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072a60; /* mov.l @(0x4,r14),r3 */
L8c072a60: *stop=0x8c072a60u;r[2]=r[15];goto L8c072a62; /* mov r15,r2 */
L8c072a62: *stop=0x8c072a62u;r[2]+=12u;goto L8c072a64; /* add #0xc,r2 */
L8c072a64: *stop=0x8c072a64u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c072a66; /* mov.l @(0x8,r14),r1 */
L8c072a66: *stop=0x8c072a66u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c072a68; /* mov.l r3,@r2 */
L8c072a68: *stop=0x8c072a68u;r[4]=r[15];goto L8c072a6a; /* mov r15,r4 */
L8c072a6a: *stop=0x8c072a6au;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c072a6c; /* mov.l r1,@(0x4,r2) */
L8c072a6c: *stop=0x8c072a6cu;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072a6e; /* mov.l @(0xc,r14),r3 */
L8c072a6e: *stop=0x8c072a6eu;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c072a70; /* mov.l r3,@(0x8,r2) */
L8c072a70: target=r[11];*pr=0x8c072a74u;*stop=0x8c072a72u;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072a74; /* jsr @r11 */
L8c072a74: *stop=0x8c072a74u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c072a76; /* fldi0 fr3 */
L8c072a76: *stop=0x8c072a76u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c072a78; /* fcmp/eq fr3,fr0 */
L8c072a78: if((*sr&1u))goto L8c072a80;goto L8c072a7a; /* bt 0x8c072a80 */
L8c072a7a: *stop=0x8c072a7au;r[4]=r[15];goto L8c072a7c; /* mov r15,r4 */
L8c072a7c: target=r[10];*pr=0x8c072a80u;*stop=0x8c072a7eu;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072a80; /* jsr @r10 */
L8c072a80: *stop=0x8c072a80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072c64u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072a82; /* mov.l 0x8c072c64,r2 */
L8c072a82: *stop=0x8c072a82u;r[5]=r[15];goto L8c072a84; /* mov r15,r5 */
L8c072a84: *stop=0x8c072a84u;r[4]=r[15];goto L8c072a86; /* mov r15,r4 */
L8c072a86: *stop=0x8c072a86u;r[5]+=12u;goto L8c072a88; /* add #0xc,r5 */
L8c072a88: target=r[2];*pr=0x8c072a8cu;*stop=0x8c072a8au;r[4]+=36u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072a8c; /* jsr @r2 */
L8c072a8c: *stop=0x8c072a8cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c072a8e; /* fldi0 fr3 */
L8c072a8e: *stop=0x8c072a8eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c072a90; /* fcmp/gt fr0,fr3 */
L8c072a90: take=!(*sr&1u);*stop=0x8c072a92u;r[4]=r[14];if(take)goto L8c072ab2;goto L8c072a94; /* bf/s 0x8c072ab2 */
L8c072a94: *stop=0x8c072a94u;r[0]=36u;goto L8c072a96; /* mov #0x24,r0 */
L8c072a96: *stop=0x8c072a96u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a98; /* fmov.s @(r0,r15),fr2 */
L8c072a98: *stop=0x8c072a98u;r[0]=36u;goto L8c072a9a; /* mov #0x24,r0 */
L8c072a9a: *stop=0x8c072a9au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c072a9c; /* fneg fr2 */
L8c072a9c: *stop=0x8c072a9cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c072a9e; /* fmov fr2,@(r0,r15) */
L8c072a9e: *stop=0x8c072a9eu;r[0]=40u;goto L8c072aa0; /* mov #0x28,r0 */
L8c072aa0: *stop=0x8c072aa0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c072aa2; /* fmov.s @(r0,r15),fr2 */
L8c072aa2: *stop=0x8c072aa2u;r[0]=40u;goto L8c072aa4; /* mov #0x28,r0 */
L8c072aa4: *stop=0x8c072aa4u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c072aa6; /* fneg fr2 */
L8c072aa6: *stop=0x8c072aa6u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c072aa8; /* fmov fr2,@(r0,r15) */
L8c072aa8: *stop=0x8c072aa8u;r[0]=44u;goto L8c072aaa; /* mov #0x2c,r0 */
L8c072aaa: *stop=0x8c072aaau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c072aac; /* fmov.s @(r0,r15),fr2 */
L8c072aac: *stop=0x8c072aacu;r[0]=44u;goto L8c072aae; /* mov #0x2c,r0 */
L8c072aae: *stop=0x8c072aaeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c072ab0; /* fneg fr2 */
L8c072ab0: *stop=0x8c072ab0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c072ab2; /* fmov fr2,@(r0,r15) */
L8c072ab2: target=r[11];*pr=0x8c072ab6u;*stop=0x8c072ab4u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072ab6; /* jsr @r11 */
L8c072ab6: *stop=0x8c072ab6u;r[0]=36u;goto L8c072ab8; /* mov #0x24,r0 */
L8c072ab8: *stop=0x8c072ab8u;fcopy(f,4u,0u);goto L8c072aba; /* fmov fr0,fr4 */
L8c072aba: *stop=0x8c072abau;fcopy(f,2u,4u);goto L8c072abc; /* fmov fr4,fr2 */
L8c072abc: *stop=0x8c072abcu;r[3]=r[15];goto L8c072abe; /* mov r15,r3 */
L8c072abe: *stop=0x8c072abeu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072ac0; /* mov.l @(0x4,r14),r2 */
L8c072ac0: *stop=0x8c072ac0u;r[3]+=12u;goto L8c072ac2; /* add #0xc,r3 */
L8c072ac2: *stop=0x8c072ac2u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c072ac4; /* mov.l @(0x8,r14),r1 */
L8c072ac4: *stop=0x8c072ac4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c072ac6; /* mov.l r2,@r3 */
L8c072ac6: *stop=0x8c072ac6u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c072ac8; /* mov.l r1,@(0x4,r3) */
L8c072ac8: *stop=0x8c072ac8u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072aca; /* mov.l @(0xc,r14),r2 */
L8c072aca: *stop=0x8c072acau;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[2],4);if(io_status)return io_status;goto L8c072acc; /* mov.l r2,@(0x8,r3) */
L8c072acc: *stop=0x8c072accu;r[3]=r[15];goto L8c072ace; /* mov r15,r3 */
L8c072ace: *stop=0x8c072aceu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072ad0; /* fmov.s @(r0,r15),fr3 */
L8c072ad0: *stop=0x8c072ad0u;r[0]=4u;goto L8c072ad2; /* mov #0x4,r0 */
L8c072ad2: *stop=0x8c072ad2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c072ad4; /* fmul fr3,fr2 */
L8c072ad4: *stop=0x8c072ad4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c072ad6; /* fmov fr2,@(r0,r14) */
L8c072ad6: *stop=0x8c072ad6u;r[0]=40u;goto L8c072ad8; /* mov #0x28,r0 */
L8c072ad8: *stop=0x8c072ad8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072ada; /* fmov.s @(r0,r15),fr3 */
L8c072ada: *stop=0x8c072adau;r[0]=8u;goto L8c072adc; /* mov #0x8,r0 */
L8c072adc: *stop=0x8c072adcu;fcopy(f,2u,4u);goto L8c072ade; /* fmov fr4,fr2 */
L8c072ade: *stop=0x8c072adeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c072ae0; /* fmul fr3,fr2 */
L8c072ae0: *stop=0x8c072ae0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c072ae2; /* fmov fr2,@(r0,r14) */
L8c072ae2: *stop=0x8c072ae2u;r[0]=44u;goto L8c072ae4; /* mov #0x2c,r0 */
L8c072ae4: *stop=0x8c072ae4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c072ae6; /* fmov.s @(r0,r15),fr3 */
L8c072ae6: *stop=0x8c072ae6u;r[0]=12u;goto L8c072ae8; /* mov #0xc,r0 */
L8c072ae8: *stop=0x8c072ae8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c072aea; /* fmul fr3,fr4 */
L8c072aea: *stop=0x8c072aeau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c072aec; /* fmov fr4,@(r0,r14) */
L8c072aec: *stop=0x8c072aecu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072aee; /* mov.l @(0x4,r14),r2 */
L8c072aee: *stop=0x8c072aeeu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c072af0; /* mov.l @(0x8,r14),r1 */
L8c072af0: *stop=0x8c072af0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c072af2; /* mov.l r2,@r3 */
L8c072af2: *stop=0x8c072af2u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c072af4; /* mov.l r1,@(0x4,r3) */
L8c072af4: *stop=0x8c072af4u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072af6; /* mov.l @(0xc,r14),r2 */
L8c072af6: *stop=0x8c072af6u;r[4]=r[15];goto L8c072af8; /* mov r15,r4 */
L8c072af8: *stop=0x8c072af8u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[2],4);if(io_status)return io_status;goto L8c072afa; /* mov.l r2,@(0x8,r3) */
L8c072afa: target=r[10];*pr=0x8c072afeu;*stop=0x8c072afcu;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072afe; /* jsr @r10 */
L8c072afe: target=r[10];*pr=0x8c072b02u;*stop=0x8c072b00u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072b02; /* jsr @r10 */
L8c072b02: *stop=0x8c072b02u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072c68u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072b04; /* mov.l 0x8c072c68,r3 */
L8c072b04: *stop=0x8c072b04u;r[6]=r[12];goto L8c072b06; /* mov r12,r6 */
L8c072b06: *stop=0x8c072b06u;r[4]=r[15];goto L8c072b08; /* mov r15,r4 */
L8c072b08: *stop=0x8c072b08u;r[6]+=80u;goto L8c072b0a; /* add #0x50,r6 */
L8c072b0a: *stop=0x8c072b0au;r[5]=r[15];goto L8c072b0c; /* mov r15,r5 */
L8c072b0c: target=r[3];*pr=0x8c072b10u;*stop=0x8c072b0eu;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072b10; /* jsr @r3 */
L8c072b10: *stop=0x8c072b10u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072b12; /* mov.l @(0x18,r13),r2 */
L8c072b12: *stop=0x8c072b12u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072c6cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072b14; /* mov.l 0x8c072c6c,r3 */
L8c072b14: *stop=0x8c072b14u;r[2]-=r[0];goto L8c072b16; /* sub r0,r2 */
L8c072b16: *stop=0x8c072b16u;r[5]=r[2];goto L8c072b18; /* mov r2,r5 */
L8c072b18: *stop=0x8c072b18u;if(!valid_access(r[13]+24u,4u))return -3;wr(m,bus,&io_status,r[13]+0x18u,r[2],4);if(io_status)return io_status;goto L8c072b1a; /* mov.l r2,@(0x18,r13) */
L8c072b1a: target=r[3];*pr=0x8c072b1eu;*stop=0x8c072b1cu;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c072b1e; /* jsr @r3 */
L8c072b1e: *stop=0x8c072b1eu;r[2]=96u;goto L8c072b20; /* mov #0x60,r2 */
L8c072b20: *stop=0x8c072b20u;r[2]<<=8;goto L8c072b22; /* shll8 r2 */
L8c072b22: *stop=0x8c072b22u;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[2]);goto L8c072b24; /* cmp/gt r2,r0 */
L8c072b24: if(!(*sr&1u))goto L8c072b2e;goto L8c072b26; /* bf 0x8c072b2e */
L8c072b26: *stop=0x8c072b26u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c072b28; /* mov.l @(0x18,r13),r1 */
L8c072b28: *stop=0x8c072b28u;{uint32_t loaded=rd(m,bus,&io_status,0x8c072c70u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c072b2a; /* mov.l 0x8c072c70,r3 */
L8c072b2a: *stop=0x8c072b2au;r[1]+=r[3];goto L8c072b2c; /* add r3,r1 */
L8c072b2c: *stop=0x8c072b2cu;if(!valid_access(r[13]+24u,4u))return -3;wr(m,bus,&io_status,r[13]+0x18u,r[1],4);if(io_status)return io_status;goto L8c072b2e; /* mov.l r1,@(0x18,r13) */
L8c072b2e: *stop=0x8c072b2eu;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c072b30; /* mov.l @(0x18,r13),r2 */
L8c072b30: *stop=0x8c072b30u;if(!valid_access(r[14]+32u,4u))return -3;wr(m,bus,&io_status,r[14]+0x20u,r[2],4);if(io_status)return io_status;goto L8c072b32; /* mov.l r2,@(0x20,r14) */
L8c072b32: *stop=0x8c072b32u;r[15]+=108u;goto L8c072b34; /* add #0x6c,r15 */
L8c072b34: *stop=0x8c072b34u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c072b36; /* lds.l @r15+,PR */
L8c072b36: *stop=0x8c072b36u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c072b38; /* fmov.s @r15+,fr15 */
L8c072b38: *stop=0x8c072b38u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c072b3a; /* mov.l @r15+,r10 */
L8c072b3a: *stop=0x8c072b3au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c072b3c; /* mov.l @r15+,r11 */
L8c072b3c: *stop=0x8c072b3cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c072b3e; /* mov.l @r15+,r12 */
L8c072b3e: *stop=0x8c072b3eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c072b40; /* mov.l @r15+,r13 */
L8c072b40: *stop=0x8c072b40u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c072b42u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_player_movement_dependencies_8c072946(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c07296e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07296eu);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c0729a8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0729a8u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c0729c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0729c6u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c072a06(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072a06u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c072a5e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072a5eu);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c072a74(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072a74u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c072a80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072a80u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c072a8c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072a8cu);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c072ab6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072ab6u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c072afe(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072afeu);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c072b02(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072b02u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c072b10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072b10u);}
SA_STAGE_EXPORT int sa_player_movement_dependencies_resume_8c072b1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_player_movement_dependencies_run_8c072946(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c072b1eu);}

SA_STAGE_EXPORT int sa_player_movement_dependencies_8c055d5a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c055d5a;
L8c055d5a: *stop=0x8c055d5au;r[5]=r[5]&65535u;goto L8c055d5c; /* extu.w r5,r5 */
L8c055d5c: *stop=0x8c055d5cu;r[3]=r[5];goto L8c055d5e; /* mov r5,r3 */
L8c055d5e: *stop=0x8c055d5eu;r[4]=r[4]&65535u;goto L8c055d60; /* extu.w r4,r4 */
L8c055d60: *stop=0x8c055d60u;r[3]-=r[4];goto L8c055d62; /* sub r4,r3 */
L8c055d62: *stop=0x8c055d62u;r[4]=r[3];goto L8c055d64; /* mov r3,r4 */
L8c055d64: *stop=0x8c055d64u;r[2]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c055d66; /* exts.w r4,r2 */
L8c055d66: *stop=0x8c055d66u;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c055d68; /* cmp/pz r2 */
L8c055d68: if((*sr&1u))goto L8c055d6c;goto L8c055d6a; /* bt 0x8c055d6c */
L8c055d6a: *stop=0x8c055d6au;r[4]=0u-r[4];goto L8c055d6c; /* neg r4,r4 */
L8c055d6c: *stop=0x8c055d6cu;if(*pr!=entry_pr)return -3;*stop=0x8c055d6eu;r[0]=r[4]&65535u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_player_movement_dependencies_8c06c880(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c06c880;
L8c06c880: *stop=0x8c06c880u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c06c882; /* mov.l r14,@-r15 */
L8c06c882: *stop=0x8c06c882u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c06c884; /* mov.l r13,@-r15 */
L8c06c884: *stop=0x8c06c884u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c06c886; /* mov.l r12,@-r15 */
L8c06c886: *stop=0x8c06c886u;r[15]+=4294967288u;goto L8c06c888; /* add #-0x8,r15 */
L8c06c888: *stop=0x8c06c888u;if(!valid_access(r[4]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x4u,2);if(io_status)return io_status;goto L8c06c88a; /* mov.w @(0x4,r4),r0 */
L8c06c88a: *stop=0x8c06c88au;r[7]=8u;goto L8c06c88c; /* mov #0x8,r7 */
L8c06c88c: *stop=0x8c06c88cu;r[7]<<=8;goto L8c06c88e; /* shll8 r7 */
L8c06c88e: *stop=0x8c06c88eu;r[14]=2u;goto L8c06c890; /* mov #0x2,r14 */
L8c06c890: *stop=0x8c06c890u;r[13]=6u;goto L8c06c892; /* mov #0x6,r13 */
L8c06c892: *stop=0x8c06c892u;*sr=(*sr&~1u)|!(r[7]&r[0]);goto L8c06c894; /* tst r7,r0 */
L8c06c894: *stop=0x8c06c894u;r[12]=0u;goto L8c06c896; /* mov #0x0,r12 */
L8c06c896: *stop=0x8c06c896u;r[13]<<=8;goto L8c06c898; /* shll8 r13 */
L8c06c898: *stop=0x8c06c898u;r[14]<<=8;goto L8c06c89a; /* shll8 r14 */
L8c06c89a: take=(*sr&1u);*stop=0x8c06c89cu;r[5]=r[12];if(take)goto L8c06c944;goto L8c06c89e; /* bt/s 0x8c06c944 */
L8c06c89e: *stop=0x8c06c89eu;if(!valid_access(r[4]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x8u,1);if(io_status)return io_status;goto L8c06c8a0; /* mov.b @(0x8,r4),r0 */
L8c06c8a0: *stop=0x8c06c8a0u;r[6]=r[0]&255u;goto L8c06c8a2; /* extu.b r0,r6 */
L8c06c8a2: *stop=0x8c06c8a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c06c9c0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c06c8a4; /* mov.l 0x8c06c9c0,r0 */
L8c06c8a4: *stop=0x8c06c8a4u;r[6]<<=2;goto L8c06c8a6; /* shll2 r6 */
L8c06c8a6: *stop=0x8c06c8a6u;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c06c8a8; /* mov.l @(r0,r6),r6 */
L8c06c8a8: *stop=0x8c06c8a8u;r[0]=104u;goto L8c06c8aa; /* mov #0x68,r0 */
L8c06c8aa: *stop=0x8c06c8aau;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c06c8ac; /* mov.l r6,@(0x4,r15) */
L8c06c8ac: *stop=0x8c06c8acu;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c06c8ae; /* mov.l @(r0,r6),r6 */
L8c06c8ae: *stop=0x8c06c8aeu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c06c8b0; /* mov.l r6,@r15 */
L8c06c8b0: *stop=0x8c06c8b0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c06c8b2; /* mov.l @r15,r3 */
L8c06c8b2: *stop=0x8c06c8b2u;if(!valid_access(r[6]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x20u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c06c8b4; /* mov.l @(0x20,r6),r6 */
L8c06c8b4: *stop=0x8c06c8b4u;if(!valid_access(r[3]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c06c8b6; /* mov.l @(0x24,r3),r2 */
L8c06c8b6: *stop=0x8c06c8b6u;r[3]=r[2];goto L8c06c8b8; /* mov r2,r3 */
L8c06c8b8: *stop=0x8c06c8b8u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c06c8ba; /* tst r3,r3 */
L8c06c8ba: take=(*sr&1u);*stop=0x8c06c8bcu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c06c98e;goto L8c06c8be; /* bt/s 0x8c06c98e */
L8c06c8be: *stop=0x8c06c8beu;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c06c8c0; /* tst r6,r6 */
L8c06c8c0: if((*sr&1u))goto L8c06c98e;goto L8c06c8c2; /* bt 0x8c06c98e */
L8c06c8c2: *stop=0x8c06c8c2u;if(!valid_access(r[6]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x38u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c06c8c4; /* mov.l @(0x38,r6),r2 */
L8c06c8c4: *stop=0x8c06c8c4u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c06c8c6; /* tst r2,r2 */
L8c06c8c6: if((*sr&1u))goto L8c06c98e;goto L8c06c8c8; /* bt 0x8c06c98e */
L8c06c8c8: *stop=0x8c06c8c8u;if(!valid_access(r[6]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[6]+0x2u,1);if(io_status)return io_status;goto L8c06c8ca; /* mov.b @(0x2,r6),r0 */
L8c06c8ca: *stop=0x8c06c8cau;if(!valid_access(r[15],1u))return -3;wr(m,bus,&io_status,r[15],r[0],1);if(io_status)return io_status;goto L8c06c8cc; /* mov.b r0,@r15 */
L8c06c8cc: *stop=0x8c06c8ccu;if(!valid_access(r[4]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;goto L8c06c8ce; /* mov.b @(0x9,r4),r0 */
L8c06c8ce: *stop=0x8c06c8ceu;if(!valid_access(r[15],1u))return -3;r[6]=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;goto L8c06c8d0; /* mov.b @r15,r6 */
L8c06c8d0: *stop=0x8c06c8d0u;r[0]=r[0]&255u;goto L8c06c8d2; /* extu.b r0,r0 */
L8c06c8d2: *stop=0x8c06c8d2u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c06c8d4; /* cmp/eq #0x0,r0 */
L8c06c8d4: take=(*sr&1u);*stop=0x8c06c8d6u;r[6]=r[6]&255u;if(take)goto L8c06c8f0;goto L8c06c8d8; /* bt/s 0x8c06c8f0 */
L8c06c8d8: *stop=0x8c06c8d8u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c06c8da; /* cmp/eq #0x2,r0 */
L8c06c8da: if((*sr&1u))goto L8c06c8f0;goto L8c06c8dc; /* bt 0x8c06c8f0 */
L8c06c8dc: *stop=0x8c06c8dcu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c06c8de; /* cmp/eq #0x3,r0 */
L8c06c8de: if((*sr&1u))goto L8c06c8f0;goto L8c06c8e0; /* bt 0x8c06c8f0 */
L8c06c8e0: *stop=0x8c06c8e0u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c06c8e2; /* cmp/eq #0x5,r0 */
L8c06c8e2: if((*sr&1u))goto L8c06c8f0;goto L8c06c8e4; /* bt 0x8c06c8f0 */
L8c06c8e4: *stop=0x8c06c8e4u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c06c8e6; /* cmp/eq #0x6,r0 */
L8c06c8e6: if((*sr&1u))goto L8c06c8f0;goto L8c06c8e8; /* bt 0x8c06c8f0 */
L8c06c8e8: *stop=0x8c06c8e8u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c06c8ea; /* cmp/eq #0x7,r0 */
L8c06c8ea: if((*sr&1u))goto L8c06c91c;goto L8c06c8ec; /* bt 0x8c06c91c */
L8c06c8ec: *stop=0x8c06c8eeu;goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c8f0: *stop=0x8c06c8f0u;r[0]=r[6];goto L8c06c8f2; /* mov r6,r0 */
L8c06c8f2: *stop=0x8c06c8f2u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c06c8f4; /* cmp/eq #0x4,r0 */
L8c06c8f4: if(!(*sr&1u))goto L8c06c8fa;goto L8c06c8f6; /* bf 0x8c06c8fa */
L8c06c8f6: *stop=0x8c06c8f8u;goto L8c06ca0c; /* bra 0x8c06ca0c */
L8c06c8fa: *stop=0x8c06c8fau;r[0]=r[6];goto L8c06c8fc; /* mov r6,r0 */
L8c06c8fc: *stop=0x8c06c8fcu;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c06c8fe; /* cmp/eq #0x5,r0 */
L8c06c8fe: if(!(*sr&1u))goto L8c06c904;goto L8c06c900; /* bf 0x8c06c904 */
L8c06c900: *stop=0x8c06c902u;goto L8c06c9ec; /* bra 0x8c06c9ec */
L8c06c904: *stop=0x8c06c904u;r[0]=r[6];goto L8c06c906; /* mov r6,r0 */
L8c06c906: *stop=0x8c06c906u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c06c908; /* cmp/eq #0x6,r0 */
L8c06c908: if(!(*sr&1u))goto L8c06c90e;goto L8c06c90a; /* bf 0x8c06c90e */
L8c06c90a: *stop=0x8c06c90cu;goto L8c06c9f8; /* bra 0x8c06c9f8 */
L8c06c90e: *stop=0x8c06c90eu;r[0]=r[6];goto L8c06c910; /* mov r6,r0 */
L8c06c910: *stop=0x8c06c910u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c06c912; /* cmp/eq #0x7,r0 */
L8c06c912: if((*sr&1u))goto L8c06c918;goto L8c06c914; /* bt 0x8c06c918 */
L8c06c914: *stop=0x8c06c916u;goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c918: *stop=0x8c06c91au;r[5]=r[7];goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c91c: *stop=0x8c06c91cu;r[0]=r[6];goto L8c06c91e; /* mov r6,r0 */
L8c06c91e: *stop=0x8c06c91eu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c06c920; /* cmp/eq #0x4,r0 */
L8c06c920: if(!(*sr&1u))goto L8c06c926;goto L8c06c922; /* bf 0x8c06c926 */
L8c06c922: *stop=0x8c06c924u;goto L8c06ca0c; /* bra 0x8c06ca0c */
L8c06c926: *stop=0x8c06c926u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c06c928; /* cmp/eq #0x5,r0 */
L8c06c928: if(!(*sr&1u))goto L8c06c92e;goto L8c06c92a; /* bf 0x8c06c92e */
L8c06c92a: *stop=0x8c06c92cu;goto L8c06ca1a; /* bra 0x8c06ca1a */
L8c06c92e: *stop=0x8c06c92eu;r[0]=r[6];goto L8c06c930; /* mov r6,r0 */
L8c06c930: *stop=0x8c06c930u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c06c932; /* cmp/eq #0x6,r0 */
L8c06c932: if(!(*sr&1u))goto L8c06c938;goto L8c06c934; /* bf 0x8c06c938 */
L8c06c934: *stop=0x8c06c936u;goto L8c06ca1a; /* bra 0x8c06ca1a */
L8c06c938: *stop=0x8c06c938u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c06c93a; /* cmp/eq #0x7,r0 */
L8c06c93a: if((*sr&1u))goto L8c06c940;goto L8c06c93c; /* bt 0x8c06c940 */
L8c06c93c: *stop=0x8c06c93eu;goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c940: *stop=0x8c06c942u;r[5]=r[7];goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c944: *stop=0x8c06c944u;if(!valid_access(r[4]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x38u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c06c946; /* mov.l @(0x38,r4),r6 */
L8c06c946: *stop=0x8c06c946u;r[3]=2u;goto L8c06c948; /* mov #0x2,r3 */
L8c06c948: *stop=0x8c06c948u;if(!valid_access(r[6]+2u,2u))return -3;r[0]=sx(m,bus,&io_status,r[6]+0x2u,2);if(io_status)return io_status;goto L8c06c94a; /* mov.w @(0x2,r6),r0 */
L8c06c94a: *stop=0x8c06c94au;r[6]=r[0]&65535u;goto L8c06c94c; /* extu.w r0,r6 */
L8c06c94c: *stop=0x8c06c94cu;*sr=(*sr&~1u)|!(r[6]&r[3]);goto L8c06c94e; /* tst r6,r3 */
L8c06c94e: if(!(*sr&1u))goto L8c06c954;goto L8c06c950; /* bf 0x8c06c954 */
L8c06c950: *stop=0x8c06c952u;goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c954: *stop=0x8c06c954u;if(!valid_access(r[4]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x4u,2);if(io_status)return io_status;goto L8c06c956; /* mov.w @(0x4,r4),r0 */
L8c06c956: *stop=0x8c06c956u;*sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c06c958; /* tst #0x4,r0 */
L8c06c958: if((*sr&1u))goto L8c06c95e;goto L8c06c95a; /* bt 0x8c06c95e */
L8c06c95a: *stop=0x8c06c95cu;goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c95e: *stop=0x8c06c95eu;r[2]=8u;goto L8c06c960; /* mov #0x8,r2 */
L8c06c960: *stop=0x8c06c960u;*sr=(*sr&~1u)|!(r[6]&r[2]);goto L8c06c962; /* tst r6,r2 */
L8c06c962: if(!(*sr&1u))goto L8c06c968;goto L8c06c964; /* bf 0x8c06c968 */
L8c06c964: *stop=0x8c06c966u;goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c968: *stop=0x8c06c968u;if(!valid_access(r[4]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x38u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c06c96a; /* mov.l @(0x38,r4),r6 */
L8c06c96a: *stop=0x8c06c96au;if(!valid_access(r[6]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c06c96c; /* mov.l @(0x18,r6),r6 */
L8c06c96c: *stop=0x8c06c96cu;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c06c96e; /* tst r6,r6 */
L8c06c96e: if((*sr&1u))goto L8c06ca24;goto L8c06c970; /* bt 0x8c06ca24 */
L8c06c970: *stop=0x8c06c970u;if(!valid_access(r[6]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c06c972; /* mov.l @(0x10,r6),r0 */
L8c06c972: *stop=0x8c06c972u;if(!valid_access(r[0]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c06c974; /* mov.l @(0x20,r0),r0 */
L8c06c974: *stop=0x8c06c974u;if(!valid_access(r[0]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+0x4u,2);if(io_status)return io_status;goto L8c06c976; /* mov.w @(0x4,r0),r0 */
L8c06c976: *stop=0x8c06c976u;*sr=(*sr&~1u)|!(r[0]&0x40u);goto L8c06c978; /* tst #0x40,r0 */
L8c06c978: if((*sr&1u))goto L8c06ca24;goto L8c06c97a; /* bt 0x8c06ca24 */
L8c06c97a: *stop=0x8c06c97au;if(!valid_access(r[6]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c06c97c; /* mov.l @(0x10,r6),r2 */
L8c06c97c: *stop=0x8c06c97cu;r[3]=16u;goto L8c06c97e; /* mov #0x10,r3 */
L8c06c97e: *stop=0x8c06c97eu;r[3]<<=8;goto L8c06c980; /* shll8 r3 */
L8c06c980: *stop=0x8c06c980u;if(!valid_access(r[2]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x20u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c06c982; /* mov.l @(0x20,r2),r6 */
L8c06c982: *stop=0x8c06c982u;if(!valid_access(r[6]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[6]+0x2u,1);if(io_status)return io_status;goto L8c06c984; /* mov.b @(0x2,r6),r0 */
L8c06c984: *stop=0x8c06c984u;if(!valid_access(r[15],1u))return -3;wr(m,bus,&io_status,r[15],r[0],1);if(io_status)return io_status;goto L8c06c986; /* mov.b r0,@r15 */
L8c06c986: *stop=0x8c06c986u;if(!valid_access(r[6]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[6]+0x4u,2);if(io_status)return io_status;goto L8c06c988; /* mov.w @(0x4,r6),r0 */
L8c06c988: *stop=0x8c06c988u;r[6]=(uint32_t)(int32_t)(int16_t)r[0];goto L8c06c98a; /* exts.w r0,r6 */
L8c06c98a: *stop=0x8c06c98au;*sr=(*sr&~1u)|!(r[3]&r[6]);goto L8c06c98c; /* tst r3,r6 */
L8c06c98c: if((*sr&1u))goto L8c06c992;goto L8c06c98e; /* bt 0x8c06c992 */
L8c06c98e: *stop=0x8c06c990u;r[0]=0u;goto L8c06ca26; /* bra 0x8c06ca26 */
L8c06c992: *stop=0x8c06c992u;if(!valid_access(r[4]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;goto L8c06c994; /* mov.b @(0x9,r4),r0 */
L8c06c994: *stop=0x8c06c994u;if(!valid_access(r[15],1u))return -3;r[6]=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;goto L8c06c996; /* mov.b @r15,r6 */
L8c06c996: *stop=0x8c06c996u;r[0]=r[0]&255u;goto L8c06c998; /* extu.b r0,r0 */
L8c06c998: *stop=0x8c06c998u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c06c99a; /* cmp/eq #0x0,r0 */
L8c06c99a: take=(*sr&1u);*stop=0x8c06c99cu;r[6]=r[6]&255u;if(take)goto L8c06c9e0;goto L8c06c99e; /* bt/s 0x8c06c9e0 */
L8c06c99e: *stop=0x8c06c99eu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c06c9a0; /* cmp/eq #0x2,r0 */
L8c06c9a0: if((*sr&1u))goto L8c06c9e0;goto L8c06c9a2; /* bt 0x8c06c9e0 */
L8c06c9a2: *stop=0x8c06c9a2u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c06c9a4; /* cmp/eq #0x3,r0 */
L8c06c9a4: if((*sr&1u))goto L8c06c9e0;goto L8c06c9a6; /* bt 0x8c06c9e0 */
L8c06c9a6: *stop=0x8c06c9a6u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c06c9a8; /* cmp/eq #0x5,r0 */
L8c06c9a8: if((*sr&1u))goto L8c06c9e0;goto L8c06c9aa; /* bt 0x8c06c9e0 */
L8c06c9aa: *stop=0x8c06c9aau;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c06c9ac; /* cmp/eq #0x6,r0 */
L8c06c9ac: if((*sr&1u))goto L8c06c9e0;goto L8c06c9ae; /* bt 0x8c06c9e0 */
L8c06c9ae: *stop=0x8c06c9aeu;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c06c9b0; /* cmp/eq #0x7,r0 */
L8c06c9b0: if((*sr&1u))goto L8c06ca06;goto L8c06c9b2; /* bt 0x8c06ca06 */
L8c06c9b2: *stop=0x8c06c9b4u;goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c9e0: *stop=0x8c06c9e0u;r[0]=r[6];goto L8c06c9e2; /* mov r6,r0 */
L8c06c9e2: *stop=0x8c06c9e2u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c06c9e4; /* cmp/eq #0x4,r0 */
L8c06c9e4: if((*sr&1u))goto L8c06ca0c;goto L8c06c9e6; /* bt 0x8c06ca0c */
L8c06c9e6: *stop=0x8c06c9e6u;r[0]=r[6];goto L8c06c9e8; /* mov r6,r0 */
L8c06c9e8: *stop=0x8c06c9e8u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c06c9ea; /* cmp/eq #0x5,r0 */
L8c06c9ea: if(!(*sr&1u))goto L8c06c9f4;goto L8c06c9ec; /* bf 0x8c06c9f4 */
L8c06c9ec: *stop=0x8c06c9ecu;r[5]=4u;goto L8c06c9ee; /* mov #0x4,r5 */
L8c06c9ee: *stop=0x8c06c9eeu;r[5]<<=8;goto L8c06c9f0; /* shll8 r5 */
L8c06c9f0: *stop=0x8c06c9f2u;goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c9f4: *stop=0x8c06c9f4u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c06c9f6; /* cmp/eq #0x6,r0 */
L8c06c9f6: if(!(*sr&1u))goto L8c06c9fc;goto L8c06c9f8; /* bf 0x8c06c9fc */
L8c06c9f8: *stop=0x8c06c9fau;r[5]=r[12];goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c9fc: *stop=0x8c06c9fcu;r[0]=r[6];goto L8c06c9fe; /* mov r6,r0 */
L8c06c9fe: *stop=0x8c06c9feu;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c06ca00; /* cmp/eq #0x7,r0 */
L8c06ca00: if(!(*sr&1u))goto L8c06ca24;goto L8c06ca02; /* bf 0x8c06ca24 */
L8c06ca02: *stop=0x8c06ca04u;r[5]=r[7];goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06ca06: *stop=0x8c06ca06u;r[0]=r[6];goto L8c06ca08; /* mov r6,r0 */
L8c06ca08: *stop=0x8c06ca08u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c06ca0a; /* cmp/eq #0x4,r0 */
L8c06ca0a: if(!(*sr&1u))goto L8c06ca10;goto L8c06ca0c; /* bf 0x8c06ca10 */
L8c06ca0c: *stop=0x8c06ca0eu;r[5]=r[14];goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06ca10: *stop=0x8c06ca10u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c06ca12; /* cmp/eq #0x5,r0 */
L8c06ca12: if((*sr&1u))goto L8c06ca1a;goto L8c06ca14; /* bt 0x8c06ca1a */
L8c06ca14: *stop=0x8c06ca14u;r[0]=r[6];goto L8c06ca16; /* mov r6,r0 */
L8c06ca16: *stop=0x8c06ca16u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c06ca18; /* cmp/eq #0x6,r0 */
L8c06ca18: if(!(*sr&1u))goto L8c06ca1e;goto L8c06ca1a; /* bf 0x8c06ca1e */
L8c06ca1a: *stop=0x8c06ca1cu;r[5]=r[13];goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06ca1e: *stop=0x8c06ca1eu;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c06ca20; /* cmp/eq #0x7,r0 */
L8c06ca20: if(!(*sr&1u))goto L8c06ca24;goto L8c06ca22; /* bf 0x8c06ca24 */
L8c06ca22: *stop=0x8c06ca22u;r[5]=r[7];goto L8c06ca24; /* mov r7,r5 */
L8c06ca24: *stop=0x8c06ca24u;r[0]=r[5];goto L8c06ca26; /* mov r5,r0 */
L8c06ca26: *stop=0x8c06ca26u;r[15]+=8u;goto L8c06ca28; /* add #0x8,r15 */
L8c06ca28: *stop=0x8c06ca28u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c06ca2a; /* mov.l @r15+,r12 */
L8c06ca2a: *stop=0x8c06ca2au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c06ca2c; /* mov.l @r15+,r13 */
L8c06ca2c: *stop=0x8c06ca2cu;if(*pr!=entry_pr)return -3;*stop=0x8c06ca2eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
