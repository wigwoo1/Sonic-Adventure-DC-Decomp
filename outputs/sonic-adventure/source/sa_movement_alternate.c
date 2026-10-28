/* Original alternate alternate-response owner with actual retained math callees.
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
static int sa_movement_alternate_run_8c075860(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c0758c6u)goto L8c0758c6;if(resume_pc==0x8c0758dcu)goto L8c0758dc;if(resume_pc==0x8c0758f0u)goto L8c0758f0;if(resume_pc==0x8c0758fcu)goto L8c0758fc;if(resume_pc==0x8c07593cu)goto L8c07593c;if(resume_pc==0x8c07594au)goto L8c07594a;if(resume_pc==0x8c075956u)goto L8c075956;if(resume_pc==0x8c07597eu)goto L8c07597e;if(resume_pc==0x8c075992u)goto L8c075992;if(resume_pc==0x8c0759aeu)goto L8c0759ae;goto L8c075860;
L8c075860: *stop=0x8c075860u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c075862; /* mov.l r14,@-r15 */
L8c075862: *stop=0x8c075862u;r[0]=4294967295u;goto L8c075864; /* mov #-0x1,r0 */
L8c075864: *stop=0x8c075864u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c075866; /* mov.l r13,@-r15 */
L8c075866: *stop=0x8c075866u;r[0]<<=8;goto L8c075868; /* shll8 r0 */
L8c075868: *stop=0x8c075868u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c07586a; /* mov.l r12,@-r15 */
L8c07586a: *stop=0x8c07586au;r[0]+=104u;goto L8c07586c; /* add #0x68,r0 */
L8c07586c: *stop=0x8c07586cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c07586e; /* mov.l r11,@-r15 */
L8c07586e: *stop=0x8c07586eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c075870; /* mov.l r10,@-r15 */
L8c075870: *stop=0x8c075870u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c075872; /* mov.l r9,@-r15 */
L8c075872: *stop=0x8c075872u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c075874; /* mov.l r8,@-r15 */
L8c075874: *stop=0x8c075874u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c075876; /* fmov.s fr15,@-r15 */
L8c075876: *stop=0x8c075876u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c075878; /* fmov.s fr14,@-r15 */
L8c075878: *stop=0x8c075878u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c07587a; /* fmov.s fr12,@-r15 */
L8c07587a: *stop=0x8c07587au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c07587c; /* sts.l PR,@-r15 */
L8c07587c: *stop=0x8c07587cu;r[15]+=r[0];goto L8c07587e; /* add r0,r15 */
L8c07587e: *stop=0x8c07587eu;r[0]=0x8c075a00u;goto L8c075880; /* mova 0x8c075a00,r0 */
L8c075880: *stop=0x8c075880u;r[14]=r[7];goto L8c075882; /* mov r7,r14 */
L8c075882: *stop=0x8c075882u;r[3]=4294967295u;goto L8c075884; /* mov #-0x1,r3 */
L8c075884: *stop=0x8c075884u;r[10]=r[15];goto L8c075886; /* mov r15,r10 */
L8c075886: *stop=0x8c075886u;r[9]=0u;goto L8c075888; /* mov #0x0,r9 */
L8c075888: *stop=0x8c075888u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075a08u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c07588a; /* mov.l 0x8c075a08,r8 */
L8c07588a: *stop=0x8c07588au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[9],4);if(io_status)return io_status;goto L8c07588c; /* mov.l r9,@(0x4,r15) */
L8c07588c: *stop=0x8c07588cu;r[10]+=24u;goto L8c07588e; /* add #0x18,r10 */
L8c07588e: *stop=0x8c07588eu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c075890; /* mov.l r3,@(0x8,r15) */
L8c075890: *stop=0x8c075890u;r[12]=r[15];goto L8c075892; /* mov r15,r12 */
L8c075892: *stop=0x8c075892u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c075894; /* fmov.s @r0,fr15 */
L8c075894: *stop=0x8c075894u;r[0]=0x8c075a04u;goto L8c075896; /* mova 0x8c075a04,r0 */
L8c075896: *stop=0x8c075896u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c075898; /* fmov.s @r0,fr14 */
L8c075898: *stop=0x8c075898u;r[0]=40u;goto L8c07589a; /* mov #0x28,r0 */
L8c07589a: *stop=0x8c07589au;if(!valid_access(r[0]+r[14],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c07589c; /* mov.w @(r0,r14),r3 */
L8c07589c: *stop=0x8c07589cu;r[13]=r[9];goto L8c07589e; /* mov r9,r13 */
L8c07589e: *stop=0x8c07589eu;r[3]=r[3]&65535u;goto L8c0758a0; /* extu.w r3,r3 */
L8c0758a0: *stop=0x8c0758a0u;*sr=(*sr&~1u)|((int32_t)r[3]>0);goto L8c0758a2; /* cmp/pl r3 */
L8c0758a2: take=!(*sr&1u);*stop=0x8c0758a4u;r[12]+=88u;if(take)goto L8c075972;goto L8c0758a6; /* bf/s 0x8c075972 */
L8c0758a6: *stop=0x8c0758a6u;r[11]=88u;goto L8c0758a8; /* mov #0x58,r11 */
L8c0758a8: *stop=0x8c0758a8u;r[2]=r[13];goto L8c0758aa; /* mov r13,r2 */
L8c0758aa: *stop=0x8c0758aau;*macl=r[11]*r[13];goto L8c0758ac; /* mul.l r11,r13 */
L8c0758ac: *stop=0x8c0758acu;r[2]<<=2;goto L8c0758ae; /* shll2 r2 */
L8c0758ae: *stop=0x8c0758aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c075a0cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0758b0; /* mov.l 0x8c075a0c,r3 */
L8c0758b0: *stop=0x8c0758b0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c0758b2; /* mov.l r2,@r15 */
L8c0758b2: *stop=0x8c0758b2u;r[2]+=r[12];goto L8c0758b4; /* add r12,r2 */
L8c0758b4: *stop=0x8c0758b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c0758b6; /* mov.l r2,@-r15 */
L8c0758b6: *stop=0x8c0758b6u;r[5]=r[14];goto L8c0758b8; /* mov r14,r5 */
L8c0758b8: *stop=0x8c0758b8u;r[11]=*macl;goto L8c0758ba; /* sts MACL,r11 */
L8c0758ba: *stop=0x8c0758bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c075a10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0758bc; /* mov.l 0x8c075a10,r2 */
L8c0758bc: *stop=0x8c0758bcu;r[5]+=4u;goto L8c0758be; /* add #0x4,r5 */
L8c0758be: *stop=0x8c0758beu;r[11]+=r[3];goto L8c0758c0; /* add r3,r11 */
L8c0758c0: *stop=0x8c0758c0u;r[4]=r[11];goto L8c0758c2; /* mov r11,r4 */
L8c0758c2: target=r[2];*pr=0x8c0758c6u;*stop=0x8c0758c4u;r[4]+=44u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0758c6; /* jsr @r2 */
L8c0758c6: *stop=0x8c0758c6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c0758c8; /* mov.l @r15+,r3 */
L8c0758c8: *stop=0x8c0758c8u;r[5]=r[11];goto L8c0758ca; /* mov r11,r5 */
L8c0758ca: *stop=0x8c0758cau;r[4]=r[14];goto L8c0758cc; /* mov r14,r4 */
L8c0758cc: *stop=0x8c0758ccu;r[5]+=20u;goto L8c0758ce; /* add #0x14,r5 */
L8c0758ce: *stop=0x8c0758ceu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[3]);if(io_status)return io_status;goto L8c0758d0; /* fmov.s fr0,@r3 */
L8c0758d0: *stop=0x8c0758d0u;r[6]=0u;goto L8c0758d2; /* mov #0x0,r6 */
L8c0758d2: *stop=0x8c0758d2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c0758d4; /* mov.l @r15,r0 */
L8c0758d4: *stop=0x8c0758d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075a14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0758d6; /* mov.l 0x8c075a14,r3 */
L8c0758d6: *stop=0x8c0758d6u;if(!valid_access(r[0]+r[10],4u))return -3;wr(m,bus,&io_status,r[10]+r[0],r[9],4);if(io_status)return io_status;goto L8c0758d8; /* mov.l r9,@(r0,r10) */
L8c0758d8: target=r[3];*pr=0x8c0758dcu;*stop=0x8c0758dau;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0758dc; /* jsr @r3 */
L8c0758dc: *stop=0x8c0758dcu;r[3]=44u;goto L8c0758de; /* mov #0x2c,r3 */
L8c0758de: *stop=0x8c0758deu;r[3]<<=8;goto L8c0758e0; /* shll8 r3 */
L8c0758e0: *stop=0x8c0758e0u;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[3]);goto L8c0758e2; /* cmp/gt r3,r0 */
L8c0758e2: take=!(*sr&1u);*stop=0x8c0758e4u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;if(take)goto L8c075900;goto L8c0758e6; /* bf/s 0x8c075900 */
L8c0758e6: *stop=0x8c0758e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075a1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0758e8; /* mov.l 0x8c075a1c,r3 */
L8c0758e8: *stop=0x8c0758e8u;r[5]=r[14];goto L8c0758ea; /* mov r14,r5 */
L8c0758ea: *stop=0x8c0758eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c075a18u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0758ec; /* mov.l 0x8c075a18,r4 */
L8c0758ec: target=r[3];*pr=0x8c0758f0u;*stop=0x8c0758eeu;r[5]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0758f0; /* jsr @r3 */
L8c0758f0: *stop=0x8c0758f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075a18u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c0758f2; /* mov.l 0x8c075a18,r4 */
L8c0758f2: *stop=0x8c0758f2u;r[5]=r[11];goto L8c0758f4; /* mov r11,r5 */
L8c0758f4: *stop=0x8c0758f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075a1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0758f6; /* mov.l 0x8c075a1c,r2 */
L8c0758f6: *stop=0x8c0758f6u;fcopy(f,12u,0u);goto L8c0758f8; /* fmov fr0,fr12 */
L8c0758f8: target=r[2];*pr=0x8c0758fcu;*stop=0x8c0758fau;r[5]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0758fc; /* jsr @r2 */
L8c0758fc: *stop=0x8c0758fcu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c0758fe; /* fcmp/gt fr0,fr12 */
L8c0758fe: if(!(*sr&1u))goto L8c075926;goto L8c075900; /* bf 0x8c075926 */
L8c075900: *stop=0x8c075900u;r[4]=r[13];goto L8c075902; /* mov r13,r4 */
L8c075902: *stop=0x8c075902u;r[0]=r[10];goto L8c075904; /* mov r10,r0 */
L8c075904: *stop=0x8c075904u;r[4]<<=2;goto L8c075906; /* shll2 r4 */
L8c075906: *stop=0x8c075906u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c075908; /* mov.l @(r0,r4),r2 */
L8c075908: *stop=0x8c075908u;r[3]=2u;goto L8c07590a; /* mov #0x2,r3 */
L8c07590a: *stop=0x8c07590au;r[2]|=r[3];goto L8c07590c; /* or r3,r2 */
L8c07590c: *stop=0x8c07590cu;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[2],4);if(io_status)return io_status;goto L8c07590e; /* mov.l r2,@(r0,r4) */
L8c07590e: *stop=0x8c07590eu;r[0]=r[4];goto L8c075910; /* mov r4,r0 */
L8c075910: *stop=0x8c075910u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c075912; /* mov.l @(0x4,r15),r1 */
L8c075912: *stop=0x8c075912u;r[1]+=1u;goto L8c075914; /* add #0x1,r1 */
L8c075914: *stop=0x8c075914u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c075916; /* mov.l r1,@(0x4,r15) */
L8c075916: *stop=0x8c075916u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c075918; /* fmov.s @(r0,r12),fr3 */
L8c075918: *stop=0x8c075918u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c07591a; /* fcmp/gt fr3,fr15 */
L8c07591a: if(!(*sr&1u))goto L8c075966;goto L8c07591c; /* bf 0x8c075966 */
L8c07591c: *stop=0x8c07591cu;r[0]=r[13];goto L8c07591e; /* mov r13,r0 */
L8c07591e: *stop=0x8c07591eu;r[0]<<=2;goto L8c075920; /* shll2 r0 */
L8c075920: *stop=0x8c075920u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c075922; /* fmov.s @(r0,r12),fr15 */
L8c075922: *stop=0x8c075924u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[13],4);if(io_status)return io_status;goto L8c075966; /* bra 0x8c075966 */
L8c075926: *stop=0x8c075926u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075928; /* mov.l @(0x10,r14),r3 */
L8c075928: *stop=0x8c075928u;r[2]=r[15];goto L8c07592a; /* mov r15,r2 */
L8c07592a: *stop=0x8c07592au;r[2]+=12u;goto L8c07592c; /* add #0xc,r2 */
L8c07592c: *stop=0x8c07592cu;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07592e; /* mov.l @(0x14,r14),r1 */
L8c07592e: *stop=0x8c07592eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c075930; /* mov.l r3,@r2 */
L8c075930: *stop=0x8c075930u;r[4]=r[15];goto L8c075932; /* mov r15,r4 */
L8c075932: *stop=0x8c075932u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c075934; /* mov.l r1,@(0x4,r2) */
L8c075934: *stop=0x8c075934u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075936; /* mov.l @(0x18,r14),r3 */
L8c075936: *stop=0x8c075936u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c075938; /* mov.l r3,@(0x8,r2) */
L8c075938: target=r[8];*pr=0x8c07593cu;*stop=0x8c07593au;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07593c; /* jsr @r8 */
L8c07593c: *stop=0x8c07593cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c07593e; /* fldi0 fr3 */
L8c07593e: *stop=0x8c07593eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c075940; /* fcmp/eq fr3,fr0 */
L8c075940: if((*sr&1u))goto L8c07594a;goto L8c075942; /* bt 0x8c07594a */
L8c075942: *stop=0x8c075942u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075a20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075944; /* mov.l 0x8c075a20,r2 */
L8c075944: *stop=0x8c075944u;r[4]=r[15];goto L8c075946; /* mov r15,r4 */
L8c075946: target=r[2];*pr=0x8c07594au;*stop=0x8c075948u;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07594a; /* jsr @r2 */
L8c07594a: *stop=0x8c07594au;{uint32_t loaded=rd(m,bus,&io_status,0x8c075a1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07594c; /* mov.l 0x8c075a1c,r3 */
L8c07594c: *stop=0x8c07594cu;r[5]=r[11];goto L8c07594e; /* mov r11,r5 */
L8c07594e: *stop=0x8c07594eu;r[4]=r[15];goto L8c075950; /* mov r15,r4 */
L8c075950: *stop=0x8c075950u;r[5]+=20u;goto L8c075952; /* add #0x14,r5 */
L8c075952: target=r[3];*pr=0x8c075956u;*stop=0x8c075954u;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075956; /* jsr @r3 */
L8c075956: *stop=0x8c075956u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][14],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c075958; /* fcmp/gt fr0,fr14 */
L8c075958: if(!(*sr&1u))goto L8c075966;goto L8c07595a; /* bf 0x8c075966 */
L8c07595a: *stop=0x8c07595au;r[0]=r[13];goto L8c07595c; /* mov r13,r0 */
L8c07595c: *stop=0x8c07595cu;r[0]<<=2;goto L8c07595e; /* shll2 r0 */
L8c07595e: *stop=0x8c07595eu;if(!valid_access(r[0]+r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c075960; /* mov.l @(r0,r10),r2 */
L8c075960: *stop=0x8c075960u;r[3]=8u;goto L8c075962; /* mov #0x8,r3 */
L8c075962: *stop=0x8c075962u;r[2]|=r[3];goto L8c075964; /* or r3,r2 */
L8c075964: *stop=0x8c075964u;if(!valid_access(r[0]+r[10],4u))return -3;wr(m,bus,&io_status,r[10]+r[0],r[2],4);if(io_status)return io_status;goto L8c075966; /* mov.l r2,@(r0,r10) */
L8c075966: *stop=0x8c075966u;r[0]=40u;goto L8c075968; /* mov #0x28,r0 */
L8c075968: *stop=0x8c075968u;if(!valid_access(r[0]+r[14],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c07596a; /* mov.w @(r0,r14),r3 */
L8c07596a: *stop=0x8c07596au;r[13]+=1u;goto L8c07596c; /* add #0x1,r13 */
L8c07596c: *stop=0x8c07596cu;r[3]=r[3]&65535u;goto L8c07596e; /* extu.w r3,r3 */
L8c07596e: *stop=0x8c07596eu;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[3]);goto L8c075970; /* cmp/ge r3,r13 */
L8c075970: if(!(*sr&1u))goto L8c0758a6;goto L8c075972; /* bf 0x8c0758a6 */
L8c075972: *stop=0x8c075972u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075974; /* mov.l @(0x4,r15),r2 */
L8c075974: *stop=0x8c075974u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c075976; /* cmp/pl r2 */
L8c075976: if(!(*sr&1u))goto L8c0759d4;goto L8c075978; /* bf 0x8c0759d4 */
L8c075978: *stop=0x8c075978u;r[4]=r[14];goto L8c07597a; /* mov r14,r4 */
L8c07597a: target=r[8];*pr=0x8c07597eu;*stop=0x8c07597cu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07597e; /* jsr @r8 */
L8c07597e: *stop=0x8c07597eu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c075980; /* mov.l @(0x8,r15),r13 */
L8c075980: *stop=0x8c075980u;fcopy(f,4u,0u);goto L8c075982; /* fmov fr0,fr4 */
L8c075982: *stop=0x8c075982u;r[13]<<=2;goto L8c075984; /* shll2 r13 */
L8c075984: *stop=0x8c075984u;r[0]=r[13];goto L8c075986; /* mov r13,r0 */
L8c075986: *stop=0x8c075986u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c075988; /* fmov.s @(r0,r12),fr3 */
L8c075988: *stop=0x8c075988u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c07598a; /* fcmp/gt fr3,fr4 */
L8c07598a: if(!(*sr&1u))goto L8c0759d4;goto L8c07598c; /* bf 0x8c0759d4 */
L8c07598c: *stop=0x8c07598cu;r[4]=r[14];goto L8c07598e; /* mov r14,r4 */
L8c07598e: target=r[8];*pr=0x8c075992u;*stop=0x8c075990u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075992; /* jsr @r8 */
L8c075992: *stop=0x8c075992u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c075994; /* fldi0 fr3 */
L8c075994: *stop=0x8c075994u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c075996; /* fcmp/eq fr3,fr0 */
L8c075996: if(!(*sr&1u))goto L8c0759a6;goto L8c075998; /* bf 0x8c0759a6 */
L8c075998: *stop=0x8c075998u;r[0]=24u;goto L8c07599a; /* mov #0x18,r0 */
L8c07599a: *stop=0x8c07599au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c07599c; /* fmov fr3,@(r0,r14) */
L8c07599c: *stop=0x8c07599cu;r[0]=20u;goto L8c07599e; /* mov #0x14,r0 */
L8c07599e: *stop=0x8c07599eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0759a0; /* fmov fr3,@(r0,r14) */
L8c0759a0: *stop=0x8c0759a0u;r[0]=16u;goto L8c0759a2; /* mov #0x10,r0 */
L8c0759a2: *stop=0x8c0759a4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0759d0; /* bra 0x8c0759d0 */
L8c0759a6: *stop=0x8c0759a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075a20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0759a8; /* mov.l 0x8c075a20,r2 */
L8c0759a8: *stop=0x8c0759a8u;r[4]=r[14];goto L8c0759aa; /* mov r14,r4 */
L8c0759aa: target=r[2];*pr=0x8c0759aeu;*stop=0x8c0759acu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0759ae; /* jsr @r2 */
L8c0759ae: *stop=0x8c0759aeu;r[4]=r[13];goto L8c0759b0; /* mov r13,r4 */
L8c0759b0: *stop=0x8c0759b0u;r[4]+=r[12];goto L8c0759b2; /* add r12,r4 */
L8c0759b2: *stop=0x8c0759b2u;r[0]=16u;goto L8c0759b4; /* mov #0x10,r0 */
L8c0759b4: *stop=0x8c0759b4u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c0759b6; /* fmov.s @r4,fr3 */
L8c0759b6: *stop=0x8c0759b6u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0759b8; /* fmov.s @(r0,r14),fr2 */
L8c0759b8: *stop=0x8c0759b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0759ba; /* fmul fr3,fr2 */
L8c0759ba: *stop=0x8c0759bau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0759bc; /* fmov fr2,@(r0,r14) */
L8c0759bc: *stop=0x8c0759bcu;r[0]=20u;goto L8c0759be; /* mov #0x14,r0 */
L8c0759be: *stop=0x8c0759beu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0759c0; /* fmov.s @(r0,r14),fr2 */
L8c0759c0: *stop=0x8c0759c0u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c0759c2; /* fmov.s @r4,fr3 */
L8c0759c2: *stop=0x8c0759c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0759c4; /* fmul fr3,fr2 */
L8c0759c4: *stop=0x8c0759c4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0759c6; /* fmov fr2,@(r0,r14) */
L8c0759c6: *stop=0x8c0759c6u;r[0]=24u;goto L8c0759c8; /* mov #0x18,r0 */
L8c0759c8: *stop=0x8c0759c8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0759ca; /* fmov.s @(r0,r14),fr2 */
L8c0759ca: *stop=0x8c0759cau;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c0759cc; /* fmov.s @r4,fr3 */
L8c0759cc: *stop=0x8c0759ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0759ce; /* fmul fr3,fr2 */
L8c0759ce: *stop=0x8c0759ceu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0759d0; /* fmov fr2,@(r0,r14) */
L8c0759d0: *stop=0x8c0759d2u;r[0]=1u;goto L8c0759d6; /* bra 0x8c0759d6 */
L8c0759d4: *stop=0x8c0759d4u;r[0]=0u;goto L8c0759d6; /* mov #0x0,r0 */
L8c0759d6: *stop=0x8c0759d6u;r[1]=4294967192u;goto L8c0759d8; /* mov #-0x68,r1 */
L8c0759d8: *stop=0x8c0759d8u;r[1]=r[1]&255u;goto L8c0759da; /* extu.b r1,r1 */
L8c0759da: *stop=0x8c0759dau;r[15]+=r[1];goto L8c0759dc; /* add r1,r15 */
L8c0759dc: *stop=0x8c0759dcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c0759de; /* lds.l @r15+,PR */
L8c0759de: *stop=0x8c0759deu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0759e0; /* fmov.s @r15+,fr12 */
L8c0759e0: *stop=0x8c0759e0u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0759e2; /* fmov.s @r15+,fr14 */
L8c0759e2: *stop=0x8c0759e2u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0759e4; /* fmov.s @r15+,fr15 */
L8c0759e4: *stop=0x8c0759e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c0759e6; /* mov.l @r15+,r8 */
L8c0759e6: *stop=0x8c0759e6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c0759e8; /* mov.l @r15+,r9 */
L8c0759e8: *stop=0x8c0759e8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c0759ea; /* mov.l @r15+,r10 */
L8c0759ea: *stop=0x8c0759eau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c0759ec; /* mov.l @r15+,r11 */
L8c0759ec: *stop=0x8c0759ecu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c0759ee; /* mov.l @r15+,r12 */
L8c0759ee: *stop=0x8c0759eeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c0759f0; /* mov.l @r15+,r13 */
L8c0759f0: *stop=0x8c0759f0u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c0759f2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_alternate_8c075860(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_alternate_run_8c075860(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_alternate_resume_8c0758c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_alternate_run_8c075860(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0758c6u);}
SA_STAGE_EXPORT int sa_movement_alternate_resume_8c0758dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_alternate_run_8c075860(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0758dcu);}
SA_STAGE_EXPORT int sa_movement_alternate_resume_8c0758f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_alternate_run_8c075860(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0758f0u);}
SA_STAGE_EXPORT int sa_movement_alternate_resume_8c0758fc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_alternate_run_8c075860(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0758fcu);}
SA_STAGE_EXPORT int sa_movement_alternate_resume_8c07593c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_alternate_run_8c075860(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07593cu);}
SA_STAGE_EXPORT int sa_movement_alternate_resume_8c07594a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_alternate_run_8c075860(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07594au);}
SA_STAGE_EXPORT int sa_movement_alternate_resume_8c075956(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_alternate_run_8c075860(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075956u);}
SA_STAGE_EXPORT int sa_movement_alternate_resume_8c07597e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_alternate_run_8c075860(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07597eu);}
SA_STAGE_EXPORT int sa_movement_alternate_resume_8c075992(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_alternate_run_8c075860(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075992u);}
SA_STAGE_EXPORT int sa_movement_alternate_resume_8c0759ae(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_alternate_run_8c075860(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0759aeu);}
