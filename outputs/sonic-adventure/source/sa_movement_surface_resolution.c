/* Original movement surface resolution, vector redirection and nullable velocity clearing.
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
static int sa_movement_surface_resolution_run_8c0747d2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c07485eu)goto L8c07485e;if(resume_pc==0x8c074870u)goto L8c074870;if(resume_pc==0x8c07487eu)goto L8c07487e;if(resume_pc==0x8c07489eu)goto L8c07489e;if(resume_pc==0x8c0748d8u)goto L8c0748d8;if(resume_pc==0x8c074938u)goto L8c074938;if(resume_pc==0x8c074962u)goto L8c074962;if(resume_pc==0x8c07498cu)goto L8c07498c;if(resume_pc==0x8c07499eu)goto L8c07499e;if(resume_pc==0x8c0749acu)goto L8c0749ac;if(resume_pc==0x8c0749bau)goto L8c0749ba;if(resume_pc==0x8c0749dcu)goto L8c0749dc;if(resume_pc==0x8c074a02u)goto L8c074a02;if(resume_pc==0x8c074a94u)goto L8c074a94;goto L8c0747d2;
L8c0747d2: *stop=0x8c0747d2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0747d4; /* mov.l r14,@-r15 */
L8c0747d4: *stop=0x8c0747d4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0747d6; /* mov.l r13,@-r15 */
L8c0747d6: *stop=0x8c0747d6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c0747d8; /* mov.l r12,@-r15 */
L8c0747d8: *stop=0x8c0747d8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c0747da; /* mov.l r11,@-r15 */
L8c0747da: *stop=0x8c0747dau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c0747dc; /* mov.l r10,@-r15 */
L8c0747dc: *stop=0x8c0747dcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c0747de; /* mov.l r9,@-r15 */
L8c0747de: *stop=0x8c0747deu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c0747e0; /* mov.l r8,@-r15 */
L8c0747e0: *stop=0x8c0747e0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c0747e2; /* fmov.s fr15,@-r15 */
L8c0747e2: *stop=0x8c0747e2u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c0747e4; /* fmov.s fr14,@-r15 */
L8c0747e4: *stop=0x8c0747e4u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c0747e6; /* fmov.s fr13,@-r15 */
L8c0747e6: *stop=0x8c0747e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0747e8; /* sts.l PR,@-r15 */
L8c0747e8: *stop=0x8c0747e8u;r[15]+=4294967276u;goto L8c0747ea; /* add #-0x14,r15 */
L8c0747ea: *stop=0x8c0747eau;r[0]=64u;goto L8c0747ec; /* mov #0x40,r0 */
L8c0747ec: *stop=0x8c0747ecu;r[11]=r[4];goto L8c0747ee; /* mov r4,r11 */
L8c0747ee: *stop=0x8c0747eeu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c0747f0; /* mov.l @(r0,r15),r4 */
L8c0747f0: *stop=0x8c0747f0u;r[0]=72u;goto L8c0747f2; /* mov #0x48,r0 */
L8c0747f2: *stop=0x8c0747f2u;r[3]=r[7];goto L8c0747f4; /* mov r7,r3 */
L8c0747f4: *stop=0x8c0747f4u;r[3]+=72u;goto L8c0747f6; /* add #0x48,r3 */
L8c0747f6: *stop=0x8c0747f6u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[12]=loaded;}goto L8c0747f8; /* mov.l @(r0,r15),r12 */
L8c0747f8: *stop=0x8c0747f8u;r[2]=r[6];goto L8c0747fa; /* mov r6,r2 */
L8c0747fa: *stop=0x8c0747fau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c0747fc; /* mov.l @r3,r1 */
L8c0747fc: *stop=0x8c0747fcu;r[2]+=68u;goto L8c0747fe; /* add #0x44,r2 */
L8c0747fe: *stop=0x8c0747feu;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c074800; /* mov.l @(0x4,r3),r0 */
L8c074800: *stop=0x8c074800u;r[10]=1u;goto L8c074802; /* mov #0x1,r10 */
L8c074802: *stop=0x8c074802u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c074804; /* mov.l r1,@r2 */
L8c074804: *stop=0x8c074804u;r[8]=64u;goto L8c074806; /* mov #0x40,r8 */
L8c074806: *stop=0x8c074806u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[0],4);if(io_status)return io_status;goto L8c074808; /* mov.l r0,@(0x4,r2) */
L8c074808: *stop=0x8c074808u;r[0]=0x8c0748f0u;goto L8c07480a; /* mova 0x8c0748f0,r0 */
L8c07480a: *stop=0x8c07480au;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07480c; /* mov.l @(0x8,r3),r1 */
L8c07480c: *stop=0x8c07480cu;r[13]=r[6];goto L8c07480e; /* mov r6,r13 */
L8c07480e: *stop=0x8c07480eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0748f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074810; /* mov.l 0x8c0748f4,r3 */
L8c074810: *stop=0x8c074810u;r[14]=r[7];goto L8c074812; /* mov r7,r14 */
L8c074812: *stop=0x8c074812u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[1],4);if(io_status)return io_status;goto L8c074814; /* mov.l r1,@(0x8,r2) */
L8c074814: *stop=0x8c074814u;r[8]<<=8;goto L8c074816; /* shll8 r8 */
L8c074816: *stop=0x8c074816u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]);if(io_status)return io_status;goto L8c074818; /* fmov.s @r0,fr13 */
L8c074818: *stop=0x8c074818u;r[0]=76u;goto L8c07481a; /* mov #0x4c,r0 */
L8c07481a: *stop=0x8c07481au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c07481c; /* mov.l @(r0,r15),r2 */
L8c07481c: *stop=0x8c07481cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]=0;goto L8c07481e; /* fldi0 fr15 */
L8c07481e: *stop=0x8c07481eu;r[2]&=r[3];goto L8c074820; /* and r3,r2 */
L8c074820: *stop=0x8c074820u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c074822; /* mov.l r2,@(0x4,r15) */
L8c074822: *stop=0x8c074822u;if(!valid_access(r[11]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[11]+0x4u,2);if(io_status)return io_status;goto L8c074824; /* mov.w @(0x4,r11),r0 */
L8c074824: *stop=0x8c074824u;*sr=(*sr&~1u)|!(r[10]&r[0]);goto L8c074826; /* tst r10,r0 */
L8c074826: take=!(*sr&1u);*stop=0x8c074828u;r[9]=2u;if(take)goto L8c07482e;goto L8c07482a; /* bf/s 0x8c07482e */
L8c07482a: *stop=0x8c07482cu;goto L8c07493c; /* bra 0x8c07493c */
L8c07482e: *stop=0x8c07482eu;r[0]=32u;goto L8c074830; /* mov #0x20,r0 */
L8c074830: *stop=0x8c074830u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c074832; /* fmov.s @r4,fr2 */
L8c074832: *stop=0x8c074832u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c074834; /* fmov.s @(r0,r11),fr3 */
L8c074834: *stop=0x8c074834u;r[0]=8u;goto L8c074836; /* mov #0x8,r0 */
L8c074836: *stop=0x8c074836u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0748f8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074838; /* mov.l 0x8c0748f8,r2 */
L8c074838: *stop=0x8c074838u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c07483a; /* fsub fr3,fr2 */
L8c07483a: *stop=0x8c07483au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c07483c; /* fmov fr2,@(r0,r15) */
L8c07483c: *stop=0x8c07483cu;r[0]=36u;goto L8c07483e; /* mov #0x24,r0 */
L8c07483e: *stop=0x8c07483eu;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c074840; /* fmov.s @(r0,r11),fr3 */
L8c074840: *stop=0x8c074840u;r[0]=4u;goto L8c074842; /* mov #0x4,r0 */
L8c074842: *stop=0x8c074842u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c074844; /* fmov.s @(r0,r4),fr2 */
L8c074844: *stop=0x8c074844u;r[0]=12u;goto L8c074846; /* mov #0xc,r0 */
L8c074846: *stop=0x8c074846u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074848; /* fsub fr3,fr2 */
L8c074848: *stop=0x8c074848u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c07484a; /* fmov fr2,@(r0,r15) */
L8c07484a: *stop=0x8c07484au;r[0]=40u;goto L8c07484c; /* mov #0x28,r0 */
L8c07484c: *stop=0x8c07484cu;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c07484e; /* fmov.s @(r0,r11),fr3 */
L8c07484e: *stop=0x8c07484eu;r[0]=8u;goto L8c074850; /* mov #0x8,r0 */
L8c074850: *stop=0x8c074850u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c074852; /* fmov.s @(r0,r4),fr2 */
L8c074852: *stop=0x8c074852u;r[0]=16u;goto L8c074854; /* mov #0x10,r0 */
L8c074854: *stop=0x8c074854u;r[4]=r[15];goto L8c074856; /* mov r15,r4 */
L8c074856: *stop=0x8c074856u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074858; /* fsub fr3,fr2 */
L8c074858: *stop=0x8c074858u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c07485a; /* fmov fr2,@(r0,r15) */
L8c07485a: target=r[2];*pr=0x8c07485eu;*stop=0x8c07485cu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07485e; /* jsr @r2 */
L8c07485e: *stop=0x8c07485eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c074860; /* fldi0 fr3 */
L8c074860: *stop=0x8c074860u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c074862; /* fcmp/eq fr3,fr0 */
L8c074862: if(!(*sr&1u))goto L8c074868;goto L8c074864; /* bf 0x8c074868 */
L8c074864: *stop=0x8c074866u;goto L8c074a94; /* bra 0x8c074a94 */
L8c074868: *stop=0x8c074868u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0748fcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07486a; /* mov.l 0x8c0748fc,r2 */
L8c07486a: *stop=0x8c07486au;r[4]=r[15];goto L8c07486c; /* mov r15,r4 */
L8c07486c: target=r[2];*pr=0x8c074870u;*stop=0x8c07486eu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074870; /* jsr @r2 */
L8c074870: *stop=0x8c074870u;r[5]=r[14];goto L8c074872; /* mov r14,r5 */
L8c074872: *stop=0x8c074872u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0748e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074874; /* mov.l 0x8c0748e0,r3 */
L8c074874: *stop=0x8c074874u;r[4]=r[15];goto L8c074876; /* mov r15,r4 */
L8c074876: *stop=0x8c074876u;r[6]=0u;goto L8c074878; /* mov #0x0,r6 */
L8c074878: *stop=0x8c074878u;r[5]+=72u;goto L8c07487a; /* add #0x48,r5 */
L8c07487a: target=r[3];*pr=0x8c07487eu;*stop=0x8c07487cu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07487e; /* jsr @r3 */
L8c07487e: *stop=0x8c07487eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074880; /* mov.l @(0x4,r15),r2 */
L8c074880: *stop=0x8c074880u;r[4]=r[0];goto L8c074882; /* mov r0,r4 */
L8c074882: *stop=0x8c074882u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c074884; /* tst r2,r2 */
L8c074884: if((*sr&1u))goto L8c0748a2;goto L8c074886; /* bt 0x8c0748a2 */
L8c074886: *stop=0x8c074886u;*sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[8]);goto L8c074888; /* cmp/gt r8,r4 */
L8c074888: if(!(*sr&1u))goto L8c07488e;goto L8c07488a; /* bf 0x8c07488e */
L8c07488a: *stop=0x8c07488cu;goto L8c074ae0; /* bra 0x8c074ae0 */
L8c07488e: *stop=0x8c07488eu;r[7]=r[15];goto L8c074890; /* mov r15,r7 */
L8c074890: *stop=0x8c074890u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074900u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074892; /* mov.l 0x8c074900,r2 */
L8c074892: *stop=0x8c074892u;r[5]=r[13];goto L8c074894; /* mov r13,r5 */
L8c074894: *stop=0x8c074894u;r[6]=r[14];goto L8c074896; /* mov r14,r6 */
L8c074896: *stop=0x8c074896u;r[12]=r[10];goto L8c074898; /* mov r10,r12 */
L8c074898: *stop=0x8c074898u;r[7]+=8u;goto L8c07489a; /* add #0x8,r7 */
L8c07489a: target=r[2];*pr=0x8c07489eu;*stop=0x8c07489cu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07489e; /* jsr @r2 */
L8c07489e: *stop=0x8c0748a0u;goto L8c074a94; /* bra 0x8c074a94 */
L8c0748a2: *stop=0x8c0748a2u;r[0]=56u;goto L8c0748a4; /* mov #0x38,r0 */
L8c0748a4: *stop=0x8c0748a4u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0748a6; /* fmov.s @(r0,r13),fr3 */
L8c0748a6: *stop=0x8c0748a6u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][13],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0748a8; /* fcmp/gt fr3,fr13 */
L8c0748a8: if((*sr&1u))goto L8c0748ae;goto L8c0748aa; /* bt 0x8c0748ae */
L8c0748aa: *stop=0x8c0748acu;goto L8c0749ec; /* bra 0x8c0749ec */
L8c0748ae: *stop=0x8c0748aeu;r[3]=16u;goto L8c0748b0; /* mov #0x10,r3 */
L8c0748b0: *stop=0x8c0748b0u;r[3]<<=8;goto L8c0748b2; /* shll8 r3 */
L8c0748b2: *stop=0x8c0748b2u;*sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[3]);goto L8c0748b4; /* cmp/gt r3,r4 */
L8c0748b4: if((*sr&1u))goto L8c074920;goto L8c0748b6; /* bt 0x8c074920 */
L8c0748b6: *stop=0x8c0748b6u;r[0]=4294967244u;goto L8c0748b8; /* mov #-0x34,r0 */
L8c0748b8: *stop=0x8c0748b8u;r[0]=r[0]&255u;goto L8c0748ba; /* extu.b r0,r0 */
L8c0748ba: *stop=0x8c0748bau;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0748bc; /* fmov.s @(r0,r13),fr3 */
L8c0748bc: *stop=0x8c0748bcu;r[0]=56u;goto L8c0748be; /* mov #0x38,r0 */
L8c0748be: *stop=0x8c0748beu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0748c0; /* fmov.s @(r0,r13),fr2 */
L8c0748c0: *stop=0x8c0748c0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c0748c2; /* fcmp/gt fr2,fr3 */
L8c0748c2: if((*sr&1u))goto L8c0748c8;goto L8c0748c4; /* bt 0x8c0748c8 */
L8c0748c4: *stop=0x8c0748c6u;goto L8c074a18; /* bra 0x8c074a18 */
L8c0748c8: *stop=0x8c0748c8u;r[7]=r[15];goto L8c0748ca; /* mov r15,r7 */
L8c0748ca: *stop=0x8c0748cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c074900u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0748cc; /* mov.l 0x8c074900,r2 */
L8c0748cc: *stop=0x8c0748ccu;r[5]=r[13];goto L8c0748ce; /* mov r13,r5 */
L8c0748ce: *stop=0x8c0748ceu;r[6]=r[14];goto L8c0748d0; /* mov r14,r6 */
L8c0748d0: *stop=0x8c0748d0u;r[12]=r[10];goto L8c0748d2; /* mov r10,r12 */
L8c0748d2: *stop=0x8c0748d2u;r[7]+=8u;goto L8c0748d4; /* add #0x8,r7 */
L8c0748d4: target=r[2];*pr=0x8c0748d8u;*stop=0x8c0748d6u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0748d8; /* jsr @r2 */
L8c0748d8: *stop=0x8c0748dau;goto L8c074a94; /* bra 0x8c074a94 */
L8c074920: *stop=0x8c074920u;*sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[8]);goto L8c074922; /* cmp/gt r8,r4 */
L8c074922: if(!(*sr&1u))goto L8c074928;goto L8c074924; /* bf 0x8c074928 */
L8c074924: *stop=0x8c074926u;goto L8c074a94; /* bra 0x8c074a94 */
L8c074928: *stop=0x8c074928u;r[7]=r[15];goto L8c07492a; /* mov r15,r7 */
L8c07492a: *stop=0x8c07492au;{uint32_t loaded=rd(m,bus,&io_status,0x8c074a40u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07492c; /* mov.l 0x8c074a40,r1 */
L8c07492c: *stop=0x8c07492cu;r[5]=r[13];goto L8c07492e; /* mov r13,r5 */
L8c07492e: *stop=0x8c07492eu;r[6]=r[14];goto L8c074930; /* mov r14,r6 */
L8c074930: *stop=0x8c074930u;r[12]=r[10];goto L8c074932; /* mov r10,r12 */
L8c074932: *stop=0x8c074932u;r[7]+=8u;goto L8c074934; /* add #0x8,r7 */
L8c074934: target=r[1];*pr=0x8c074938u;*stop=0x8c074936u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074938; /* jsr @r1 */
L8c074938: *stop=0x8c07493au;goto L8c074a94; /* bra 0x8c074a94 */
L8c07493c: *stop=0x8c07493cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074a48u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07493e; /* mov.l 0x8c074a48,r1 */
L8c07493e: *stop=0x8c07493eu;r[0]=0x8c074a44u;goto L8c074940; /* mova 0x8c074a44,r0 */
L8c074940: *stop=0x8c074940u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c074942; /* mov.l @r1,r2 */
L8c074942: *stop=0x8c074942u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c074944; /* tst r2,r2 */
L8c074944: take=(*sr&1u);*stop=0x8c074946u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;if(take)goto L8c074966;goto L8c074948; /* bt/s 0x8c074966 */
L8c074948: *stop=0x8c074948u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c07494a; /* fldi1 fr3 */
L8c07494a: *stop=0x8c07494au;r[0]=8u;goto L8c07494c; /* mov #0x8,r0 */
L8c07494c: *stop=0x8c07494cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c07494e; /* fneg fr3 */
L8c07494e: *stop=0x8c07494eu;r[5]=r[15];goto L8c074950; /* mov r15,r5 */
L8c074950: *stop=0x8c074950u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c074952; /* fmov fr3,@(r0,r15) */
L8c074952: *stop=0x8c074952u;r[0]=12u;goto L8c074954; /* mov #0xc,r0 */
L8c074954: *stop=0x8c074954u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c074956; /* fmov fr15,@(r0,r15) */
L8c074956: *stop=0x8c074956u;r[0]=16u;goto L8c074958; /* mov #0x10,r0 */
L8c074958: *stop=0x8c074958u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074a4cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07495a; /* mov.l 0x8c074a4c,r2 */
L8c07495a: *stop=0x8c07495au;r[5]+=8u;goto L8c07495c; /* add #0x8,r5 */
L8c07495c: *stop=0x8c07495cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[15]);if(io_status)return io_status;goto L8c07495e; /* fmov fr15,@(r0,r15) */
L8c07495e: target=r[2];*pr=0x8c074962u;*stop=0x8c074960u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074962; /* jsr @r2 */
L8c074962: *stop=0x8c074964u;goto L8c074984; /* bra 0x8c074984 */
L8c074966: *stop=0x8c074966u;r[0]=4u;goto L8c074968; /* mov #0x4,r0 */
L8c074968: *stop=0x8c074968u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c07496a; /* fmov.s @(r0,r5),fr3 */
L8c07496a: *stop=0x8c07496au;r[0]=8u;goto L8c07496c; /* mov #0x8,r0 */
L8c07496c: *stop=0x8c07496cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c07496e; /* fneg fr3 */
L8c07496e: *stop=0x8c07496eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c074970; /* fmov fr3,@(r0,r15) */
L8c074970: *stop=0x8c074970u;r[0]=8u;goto L8c074972; /* mov #0x8,r0 */
L8c074972: *stop=0x8c074972u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c074974; /* fmov.s @(r0,r5),fr3 */
L8c074974: *stop=0x8c074974u;r[0]=12u;goto L8c074976; /* mov #0xc,r0 */
L8c074976: *stop=0x8c074976u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c074978; /* fneg fr3 */
L8c074978: *stop=0x8c074978u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07497a; /* fmov fr3,@(r0,r15) */
L8c07497a: *stop=0x8c07497au;r[0]=12u;goto L8c07497c; /* mov #0xc,r0 */
L8c07497c: *stop=0x8c07497cu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c07497e; /* fmov.s @(r0,r5),fr3 */
L8c07497e: *stop=0x8c07497eu;r[0]=16u;goto L8c074980; /* mov #0x10,r0 */
L8c074980: *stop=0x8c074980u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c074982; /* fneg fr3 */
L8c074982: *stop=0x8c074982u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c074984; /* fmov fr3,@(r0,r15) */
L8c074984: *stop=0x8c074984u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074a50u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074986; /* mov.l 0x8c074a50,r3 */
L8c074986: *stop=0x8c074986u;r[4]=r[15];goto L8c074988; /* mov r15,r4 */
L8c074988: target=r[3];*pr=0x8c07498cu;*stop=0x8c07498au;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07498c; /* jsr @r3 */
L8c07498c: *stop=0x8c07498cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c07498e; /* fldi0 fr3 */
L8c07498e: *stop=0x8c07498eu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c074990; /* fcmp/eq fr3,fr0 */
L8c074990: if(!(*sr&1u))goto L8c074996;goto L8c074992; /* bf 0x8c074996 */
L8c074992: *stop=0x8c074994u;goto L8c074a94; /* bra 0x8c074a94 */
L8c074996: *stop=0x8c074996u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074a54u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074998; /* mov.l 0x8c074a54,r2 */
L8c074998: *stop=0x8c074998u;r[4]=r[15];goto L8c07499a; /* mov r15,r4 */
L8c07499a: target=r[2];*pr=0x8c07499eu;*stop=0x8c07499cu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07499e; /* jsr @r2 */
L8c07499e: *stop=0x8c07499eu;r[5]=r[14];goto L8c0749a0; /* mov r14,r5 */
L8c0749a0: *stop=0x8c0749a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074a58u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0749a2; /* mov.l 0x8c074a58,r3 */
L8c0749a2: *stop=0x8c0749a2u;r[4]=r[15];goto L8c0749a4; /* mov r15,r4 */
L8c0749a4: *stop=0x8c0749a4u;r[6]=0u;goto L8c0749a6; /* mov #0x0,r6 */
L8c0749a6: *stop=0x8c0749a6u;r[5]+=72u;goto L8c0749a8; /* add #0x48,r5 */
L8c0749a8: target=r[3];*pr=0x8c0749acu;*stop=0x8c0749aau;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0749ac; /* jsr @r3 */
L8c0749ac: *stop=0x8c0749acu;r[5]=r[14];goto L8c0749ae; /* mov r14,r5 */
L8c0749ae: *stop=0x8c0749aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074a5cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0749b0; /* mov.l 0x8c074a5c,r3 */
L8c0749b0: *stop=0x8c0749b0u;r[4]=r[15];goto L8c0749b2; /* mov r15,r4 */
L8c0749b2: *stop=0x8c0749b2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c0749b4; /* mov.l r0,@r15 */
L8c0749b4: *stop=0x8c0749b4u;r[5]+=72u;goto L8c0749b6; /* add #0x48,r5 */
L8c0749b6: target=r[3];*pr=0x8c0749bau;*stop=0x8c0749b8u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0749ba; /* jsr @r3 */
L8c0749ba: *stop=0x8c0749bau;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0749bc; /* mov.l @(0x4,r15),r2 */
L8c0749bc: *stop=0x8c0749bcu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c0749be; /* tst r2,r2 */
L8c0749be: take=(*sr&1u);*stop=0x8c0749c0u;fcopy(f,4u,0u);if(take)goto L8c0749e0;goto L8c0749c2; /* bt/s 0x8c0749e0 */
L8c0749c2: *stop=0x8c0749c2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0749c4; /* mov.l @r15,r3 */
L8c0749c4: *stop=0x8c0749c4u;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[8]);goto L8c0749c6; /* cmp/gt r8,r3 */
L8c0749c6: if(!(*sr&1u))goto L8c0749cc;goto L8c0749c8; /* bf 0x8c0749cc */
L8c0749c8: *stop=0x8c0749cau;goto L8c074ae0; /* bra 0x8c074ae0 */
L8c0749cc: *stop=0x8c0749ccu;r[7]=r[15];goto L8c0749ce; /* mov r15,r7 */
L8c0749ce: *stop=0x8c0749ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074a40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0749d0; /* mov.l 0x8c074a40,r3 */
L8c0749d0: *stop=0x8c0749d0u;r[5]=r[13];goto L8c0749d2; /* mov r13,r5 */
L8c0749d2: *stop=0x8c0749d2u;r[6]=r[14];goto L8c0749d4; /* mov r14,r6 */
L8c0749d4: *stop=0x8c0749d4u;r[12]=r[10];goto L8c0749d6; /* mov r10,r12 */
L8c0749d6: *stop=0x8c0749d6u;r[7]+=8u;goto L8c0749d8; /* add #0x8,r7 */
L8c0749d8: target=r[3];*pr=0x8c0749dcu;*stop=0x8c0749dau;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0749dc; /* jsr @r3 */
L8c0749dc: *stop=0x8c0749deu;goto L8c074a94; /* bra 0x8c074a94 */
L8c0749e0: *stop=0x8c0749e0u;r[0]=56u;goto L8c0749e2; /* mov #0x38,r0 */
L8c0749e2: *stop=0x8c0749e2u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0749e4; /* fmov.s @(r0,r13),fr3 */
L8c0749e4: *stop=0x8c0749e4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][13],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0749e6; /* fcmp/gt fr3,fr13 */
L8c0749e6: if((*sr&1u))goto L8c074a06;goto L8c0749e8; /* bt 0x8c074a06 */
L8c0749e8: *stop=0x8c0749e8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][14],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c0749ea; /* fcmp/gt fr4,fr14 */
L8c0749ea: if((*sr&1u))goto L8c074a20;goto L8c0749ec; /* bt 0x8c074a20 */
L8c0749ec: *stop=0x8c0749ecu;r[0]=116u;goto L8c0749ee; /* mov #0x74,r0 */
L8c0749ee: *stop=0x8c0749eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c074a60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0749f0; /* mov.l 0x8c074a60,r3 */
L8c0749f0: *stop=0x8c0749f0u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c0749f2; /* fmov fr15,@(r0,r14) */
L8c0749f2: *stop=0x8c0749f2u;r[0]=112u;goto L8c0749f4; /* mov #0x70,r0 */
L8c0749f4: *stop=0x8c0749f4u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c0749f6; /* fmov fr15,@(r0,r14) */
L8c0749f6: *stop=0x8c0749f6u;r[0]=108u;goto L8c0749f8; /* mov #0x6c,r0 */
L8c0749f8: *stop=0x8c0749f8u;r[5]=r[13];goto L8c0749fa; /* mov r13,r5 */
L8c0749fa: *stop=0x8c0749fau;r[12]=r[9];goto L8c0749fc; /* mov r9,r12 */
L8c0749fc: *stop=0x8c0749fcu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,15u,r[0]+r[14]);if(io_status)return io_status;goto L8c0749fe; /* fmov fr15,@(r0,r14) */
L8c0749fe: target=r[3];*pr=0x8c074a02u;*stop=0x8c074a00u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074a02; /* jsr @r3 */
L8c074a02: *stop=0x8c074a04u;goto L8c074a94; /* bra 0x8c074a94 */
L8c074a06: *stop=0x8c074a06u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][14],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c074a08; /* fcmp/gt fr4,fr14 */
L8c074a08: if((*sr&1u))goto L8c074a20;goto L8c074a0a; /* bt 0x8c074a20 */
L8c074a0a: *stop=0x8c074a0au;r[0]=4294967244u;goto L8c074a0c; /* mov #-0x34,r0 */
L8c074a0c: *stop=0x8c074a0cu;r[0]=r[0]&255u;goto L8c074a0e; /* extu.b r0,r0 */
L8c074a0e: *stop=0x8c074a0eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c074a10; /* fmov.s @(r0,r13),fr3 */
L8c074a10: *stop=0x8c074a10u;r[0]=56u;goto L8c074a12; /* mov #0x38,r0 */
L8c074a12: *stop=0x8c074a12u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c074a14; /* fmov.s @(r0,r13),fr2 */
L8c074a14: *stop=0x8c074a14u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c074a16; /* fcmp/gt fr2,fr3 */
L8c074a16: if((*sr&1u))goto L8c074a1c;goto L8c074a18; /* bt 0x8c074a1c */
L8c074a18: *stop=0x8c074a1au;r[12]=r[9];goto L8c074a94; /* bra 0x8c074a94 */
L8c074a1c: *stop=0x8c074a1eu;r[12]=r[10];goto L8c074a94; /* bra 0x8c074a94 */
L8c074a20: *stop=0x8c074a20u;if(!valid_access(r[11]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[11]+0x9u,1);if(io_status)return io_status;goto L8c074a22; /* mov.b @(0x9,r11),r0 */
L8c074a22: *stop=0x8c074a22u;r[0]=r[0]&255u;goto L8c074a24; /* extu.b r0,r0 */
L8c074a24: *stop=0x8c074a24u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c074a26; /* cmp/eq #0x3,r0 */
L8c074a26: if(!(*sr&1u))goto L8c074a80;goto L8c074a28; /* bf 0x8c074a80 */
L8c074a28: *stop=0x8c074a28u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c074a2a; /* mov.l @r15,r3 */
L8c074a2a: *stop=0x8c074a2au;r[2]=52u;goto L8c074a2c; /* mov #0x34,r2 */
L8c074a2c: *stop=0x8c074a2cu;r[2]<<=8;goto L8c074a2e; /* shll8 r2 */
L8c074a2e: *stop=0x8c074a2eu;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[2]);goto L8c074a30; /* cmp/gt r2,r3 */
L8c074a30: if((*sr&1u))goto L8c074a80;goto L8c074a32; /* bt 0x8c074a80 */
L8c074a32: *stop=0x8c074a32u;r[0]=76u;goto L8c074a34; /* mov #0x4c,r0 */
L8c074a34: *stop=0x8c074a34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074a64u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074a36; /* mov.l 0x8c074a64,r3 */
L8c074a36: *stop=0x8c074a36u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c074a38; /* mov.l @(r0,r15),r1 */
L8c074a38: *stop=0x8c074a38u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c074a3a; /* tst r3,r1 */
L8c074a3a: if(!(*sr&1u))goto L8c074a80;goto L8c074a3c; /* bf 0x8c074a80 */
L8c074a3c: *stop=0x8c074a3eu;r[12]=r[10];goto L8c074a94; /* bra 0x8c074a94 */
L8c074a80: *stop=0x8c074a80u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c074a82; /* mov.l @r15,r2 */
L8c074a82: *stop=0x8c074a82u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[8]);goto L8c074a84; /* cmp/ge r8,r2 */
L8c074a84: if((*sr&1u))goto L8c074ae0;goto L8c074a86; /* bt 0x8c074ae0 */
L8c074a86: *stop=0x8c074a86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c074c60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074a88; /* mov.l 0x8c074c60,r3 */
L8c074a88: *stop=0x8c074a88u;r[7]=r[15];goto L8c074a8a; /* mov r15,r7 */
L8c074a8a: *stop=0x8c074a8au;r[5]=r[13];goto L8c074a8c; /* mov r13,r5 */
L8c074a8c: *stop=0x8c074a8cu;r[7]+=8u;goto L8c074a8e; /* add #0x8,r7 */
L8c074a8e: *stop=0x8c074a8eu;r[6]=r[14];goto L8c074a90; /* mov r14,r6 */
L8c074a90: target=r[3];*pr=0x8c074a94u;*stop=0x8c074a92u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c074a94; /* jsr @r3 */
L8c074a94: *stop=0x8c074a94u;r[0]=68u;goto L8c074a96; /* mov #0x44,r0 */
L8c074a96: *stop=0x8c074a96u;r[2]=r[14];goto L8c074a98; /* mov r14,r2 */
L8c074a98: *stop=0x8c074a98u;r[2]+=96u;goto L8c074a9a; /* add #0x60,r2 */
L8c074a9a: *stop=0x8c074a9au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c074a9c; /* mov.l @r2,r3 */
L8c074a9c: *stop=0x8c074a9cu;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c074a9e; /* mov.l @(0x4,r2),r1 */
L8c074a9e: *stop=0x8c074a9eu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[3],4);if(io_status)return io_status;goto L8c074aa0; /* mov.l r3,@(0x4,r14) */
L8c074aa0: *stop=0x8c074aa0u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[1],4);if(io_status)return io_status;goto L8c074aa2; /* mov.l r1,@(0x8,r14) */
L8c074aa2: *stop=0x8c074aa2u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c074aa4; /* mov.l @(0x8,r2),r3 */
L8c074aa4: *stop=0x8c074aa4u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[3],4);if(io_status)return io_status;goto L8c074aa6; /* mov.l r3,@(0xc,r14) */
L8c074aa6: *stop=0x8c074aa6u;r[3]=r[14];goto L8c074aa8; /* mov r14,r3 */
L8c074aa8: *stop=0x8c074aa8u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c074aaa; /* mov.l @(r0,r15),r4 */
L8c074aaa: *stop=0x8c074aaau;r[0]=96u;goto L8c074aac; /* mov #0x60,r0 */
L8c074aac: *stop=0x8c074aacu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074aae; /* fmov.s @(r0,r14),fr2 */
L8c074aae: *stop=0x8c074aaeu;r[0]=32u;goto L8c074ab0; /* mov #0x20,r0 */
L8c074ab0: *stop=0x8c074ab0u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c074ab2; /* fmov.s @r4,fr3 */
L8c074ab2: *stop=0x8c074ab2u;r[3]+=108u;goto L8c074ab4; /* add #0x6c,r3 */
L8c074ab4: *stop=0x8c074ab4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074ab6; /* fsub fr3,fr2 */
L8c074ab6: *stop=0x8c074ab6u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c074ab8; /* fmov fr2,@(r0,r11) */
L8c074ab8: *stop=0x8c074ab8u;r[0]=4u;goto L8c074aba; /* mov #0x4,r0 */
L8c074aba: *stop=0x8c074abau;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c074abc; /* fmov.s @(r0,r4),fr3 */
L8c074abc: *stop=0x8c074abcu;r[0]=100u;goto L8c074abe; /* mov #0x64,r0 */
L8c074abe: *stop=0x8c074abeu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074ac0; /* fmov.s @(r0,r14),fr2 */
L8c074ac0: *stop=0x8c074ac0u;r[0]=36u;goto L8c074ac2; /* mov #0x24,r0 */
L8c074ac2: *stop=0x8c074ac2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074ac4; /* fsub fr3,fr2 */
L8c074ac4: *stop=0x8c074ac4u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c074ac6; /* fmov fr2,@(r0,r11) */
L8c074ac6: *stop=0x8c074ac6u;r[0]=8u;goto L8c074ac8; /* mov #0x8,r0 */
L8c074ac8: *stop=0x8c074ac8u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c074aca; /* fmov.s @(r0,r4),fr3 */
L8c074aca: *stop=0x8c074acau;r[0]=104u;goto L8c074acc; /* mov #0x68,r0 */
L8c074acc: *stop=0x8c074accu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c074ace; /* fmov.s @(r0,r14),fr2 */
L8c074ace: *stop=0x8c074aceu;r[0]=40u;goto L8c074ad0; /* mov #0x28,r0 */
L8c074ad0: *stop=0x8c074ad0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c074ad2; /* fsub fr3,fr2 */
L8c074ad2: *stop=0x8c074ad2u;if(!valid_access(r[0]+r[11],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c074ad4; /* fmov fr2,@(r0,r11) */
L8c074ad4: *stop=0x8c074ad4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c074ad6; /* mov.l @r3,r2 */
L8c074ad6: *stop=0x8c074ad6u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c074ad8; /* mov.l @(0x4,r3),r1 */
L8c074ad8: *stop=0x8c074ad8u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8c074ada; /* mov.l r2,@(0x10,r14) */
L8c074ada: *stop=0x8c074adau;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[1],4);if(io_status)return io_status;goto L8c074adc; /* mov.l r1,@(0x14,r14) */
L8c074adc: *stop=0x8c074adcu;if(!valid_access(r[3]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c074ade; /* mov.l @(0x8,r3),r2 */
L8c074ade: *stop=0x8c074adeu;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[2],4);if(io_status)return io_status;goto L8c074ae0; /* mov.l r2,@(0x18,r14) */
L8c074ae0: *stop=0x8c074ae0u;r[0]=r[12];goto L8c074ae2; /* mov r12,r0 */
L8c074ae2: *stop=0x8c074ae2u;r[15]+=20u;goto L8c074ae4; /* add #0x14,r15 */
L8c074ae4: *stop=0x8c074ae4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c074ae6; /* lds.l @r15+,PR */
L8c074ae6: *stop=0x8c074ae6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c074ae8; /* fmov.s @r15+,fr13 */
L8c074ae8: *stop=0x8c074ae8u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c074aea; /* fmov.s @r15+,fr14 */
L8c074aea: *stop=0x8c074aeau;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c074aec; /* fmov.s @r15+,fr15 */
L8c074aec: *stop=0x8c074aecu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c074aee; /* mov.l @r15+,r8 */
L8c074aee: *stop=0x8c074aeeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c074af0; /* mov.l @r15+,r9 */
L8c074af0: *stop=0x8c074af0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c074af2; /* mov.l @r15+,r10 */
L8c074af2: *stop=0x8c074af2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c074af4; /* mov.l @r15+,r11 */
L8c074af4: *stop=0x8c074af4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c074af6; /* mov.l @r15+,r12 */
L8c074af6: *stop=0x8c074af6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c074af8; /* mov.l @r15+,r13 */
L8c074af8: *stop=0x8c074af8u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c074afau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_surface_resolution_8c0747d2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c07485e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07485eu);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c074870(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074870u);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c07487e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07487eu);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c07489e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07489eu);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0748d8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0748d8u);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c074938(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074938u);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c074962(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074962u);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c07498c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07498cu);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c07499e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07499eu);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0749ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0749acu);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0749ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0749bau);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0749dc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0749dcu);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c074a02(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074a02u);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c074a94(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0747d2(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c074a94u);}

static int sa_movement_surface_resolution_run_8c0763a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c0763c6u)goto L8c0763c6;if(resume_pc==0x8c0763e2u)goto L8c0763e2;if(resume_pc==0x8c0763f6u)goto L8c0763f6;if(resume_pc==0x8c076446u)goto L8c076446;if(resume_pc==0x8c0764b6u)goto L8c0764b6;if(resume_pc==0x8c0764ceu)goto L8c0764ce;if(resume_pc==0x8c0764dau)goto L8c0764da;if(resume_pc==0x8c0764e6u)goto L8c0764e6;if(resume_pc==0x8c07651eu)goto L8c07651e;if(resume_pc==0x8c07655cu)goto L8c07655c;if(resume_pc==0x8c076596u)goto L8c076596;if(resume_pc==0x8c0765b8u)goto L8c0765b8;if(resume_pc==0x8c0765beu)goto L8c0765be;if(resume_pc==0x8c0765ceu)goto L8c0765ce;if(resume_pc==0x8c0765e4u)goto L8c0765e4;goto L8c0763a0;
L8c0763a0: *stop=0x8c0763a0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c0763a2; /* mov.l r14,@-r15 */
L8c0763a2: *stop=0x8c0763a2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c0763a4; /* mov.l r13,@-r15 */
L8c0763a4: *stop=0x8c0763a4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c0763a6; /* mov.l r12,@-r15 */
L8c0763a6: *stop=0x8c0763a6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c0763a8; /* mov.l r11,@-r15 */
L8c0763a8: *stop=0x8c0763a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c0763aa; /* mov.l r10,@-r15 */
L8c0763aa: *stop=0x8c0763aau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c0763ac; /* mov.l r9,@-r15 */
L8c0763ac: *stop=0x8c0763acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0763ae; /* sts.l PR,@-r15 */
L8c0763ae: *stop=0x8c0763aeu;r[15]+=4294967184u;goto L8c0763b0; /* add #-0x70,r15 */
L8c0763b0: *stop=0x8c0763b0u;r[13]=r[5];goto L8c0763b2; /* mov r5,r13 */
L8c0763b2: *stop=0x8c0763b2u;r[12]=r[4];goto L8c0763b4; /* mov r4,r12 */
L8c0763b4: *stop=0x8c0763b4u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[7],4);if(io_status)return io_status;goto L8c0763b6; /* mov.l r7,@r15 */
L8c0763b6: *stop=0x8c0763b6u;if(!valid_access(r[13]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x10u,2);if(io_status)return io_status;goto L8c0763b8; /* mov.w @(0x10,r13),r0 */
L8c0763b8: *stop=0x8c0763b8u;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c0763ba; /* tst #0x3,r0 */
L8c0763ba: take=!(*sr&1u);*stop=0x8c0763bcu;r[14]=r[6];if(take)goto L8c0763d4;goto L8c0763be; /* bf/s 0x8c0763d4 */
L8c0763be: *stop=0x8c0763beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c07642cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0763c0; /* mov.l 0x8c07642c,r2 */
L8c0763c0: *stop=0x8c0763c0u;r[4]=r[13];goto L8c0763c2; /* mov r13,r4 */
L8c0763c2: target=r[2];*pr=0x8c0763c6u;*stop=0x8c0763c4u;r[4]+=56u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0763c6; /* jsr @r2 */
L8c0763c6: *stop=0x8c0763c6u;r[0]=4294967232u;goto L8c0763c8; /* mov #-0x40,r0 */
L8c0763c8: *stop=0x8c0763c8u;r[0]=r[0]&255u;goto L8c0763ca; /* extu.b r0,r0 */
L8c0763ca: *stop=0x8c0763cau;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c0763cc; /* fmov.s @(r0,r13),fr3 */
L8c0763cc: *stop=0x8c0763ccu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0763ce; /* fcmp/gt fr3,fr0 */
L8c0763ce: if((*sr&1u))goto L8c0763d4;goto L8c0763d0; /* bt 0x8c0763d4 */
L8c0763d0: *stop=0x8c0763d2u;goto L8c076600; /* bra 0x8c076600 */
L8c0763d4: *stop=0x8c0763d4u;r[5]=r[14];goto L8c0763d6; /* mov r14,r5 */
L8c0763d6: *stop=0x8c0763d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c076430u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0763d8; /* mov.l 0x8c076430,r3 */
L8c0763d8: *stop=0x8c0763d8u;r[6]=r[15];goto L8c0763da; /* mov r15,r6 */
L8c0763da: *stop=0x8c0763dau;r[5]+=72u;goto L8c0763dc; /* add #0x48,r5 */
L8c0763dc: *stop=0x8c0763dcu;r[6]+=28u;goto L8c0763de; /* add #0x1c,r6 */
L8c0763de: target=r[3];*pr=0x8c0763e2u;*stop=0x8c0763e0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0763e2; /* jsr @r3 */
L8c0763e2: *stop=0x8c0763e2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0763e4; /* fldi0 fr3 */
L8c0763e4: *stop=0x8c0763e4u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c0763e6; /* fcmp/eq fr3,fr0 */
L8c0763e6: if(!(*sr&1u))goto L8c0763ec;goto L8c0763e8; /* bf 0x8c0763ec */
L8c0763e8: *stop=0x8c0763eau;goto L8c076600; /* bra 0x8c076600 */
L8c0763ec: *stop=0x8c0763ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c07642cu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c0763ee; /* mov.l 0x8c07642c,r11 */
L8c0763ee: *stop=0x8c0763eeu;r[4]=r[15];goto L8c0763f0; /* mov r15,r4 */
L8c0763f0: *stop=0x8c0763f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c076420u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c0763f2; /* mov.l 0x8c076420,r10 */
L8c0763f2: target=r[11];*pr=0x8c0763f6u;*stop=0x8c0763f4u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0763f6; /* jsr @r11 */
L8c0763f6: *stop=0x8c0763f6u;r[0]=0x8c076434u;goto L8c0763f8; /* mova 0x8c076434,r0 */
L8c0763f8: *stop=0x8c0763f8u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0763fa; /* fmov.s @r0,fr3 */
L8c0763fa: *stop=0x8c0763fau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0763fc; /* fcmp/gt fr3,fr0 */
L8c0763fc: if((*sr&1u))goto L8c076440;goto L8c0763fe; /* bt 0x8c076440 */
L8c0763fe: *stop=0x8c0763feu;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c076400; /* mov.l @(0x1c,r14),r3 */
L8c076400: *stop=0x8c076400u;r[2]=r[15];goto L8c076402; /* mov r15,r2 */
L8c076402: *stop=0x8c076402u;r[2]+=28u;goto L8c076404; /* add #0x1c,r2 */
L8c076404: *stop=0x8c076404u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c076406; /* mov.l @(0x20,r14),r1 */
L8c076406: *stop=0x8c076406u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c076408; /* mov.l r3,@r2 */
L8c076408: *stop=0x8c076408u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c07640a; /* mov.l r1,@(0x4,r2) */
L8c07640a: *stop=0x8c07640au;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07640c; /* mov.l @(0x24,r14),r3 */
L8c07640c: *stop=0x8c07640eu;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c076446; /* bra 0x8c076446 */
L8c076440: *stop=0x8c076440u;r[4]=r[15];goto L8c076442; /* mov r15,r4 */
L8c076442: target=r[10];*pr=0x8c076446u;*stop=0x8c076444u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c076446; /* jsr @r10 */
L8c076446: *stop=0x8c076446u;r[0]=84u;goto L8c076448; /* mov #0x54,r0 */
L8c076448: *stop=0x8c076448u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c07644a; /* fmov.s @(r0,r14),fr3 */
L8c07644a: *stop=0x8c07644au;r[0]=88u;goto L8c07644c; /* mov #0x58,r0 */
L8c07644c: *stop=0x8c07644cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07644e; /* fmov fr3,@(r0,r15) */
L8c07644e: *stop=0x8c07644eu;r[0]=88u;goto L8c076450; /* mov #0x58,r0 */
L8c076450: *stop=0x8c076450u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c076452; /* fmov.s @(r0,r14),fr3 */
L8c076452: *stop=0x8c076452u;r[0]=92u;goto L8c076454; /* mov #0x5c,r0 */
L8c076454: *stop=0x8c076454u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076456; /* fmov fr3,@(r0,r15) */
L8c076456: *stop=0x8c076456u;r[0]=92u;goto L8c076458; /* mov #0x5c,r0 */
L8c076458: *stop=0x8c076458u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c07645a; /* fmov.s @(r0,r14),fr3 */
L8c07645a: *stop=0x8c07645au;r[0]=96u;goto L8c07645c; /* mov #0x60,r0 */
L8c07645c: *stop=0x8c07645cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07645e; /* fmov fr3,@(r0,r15) */
L8c07645e: *stop=0x8c07645eu;r[0]=72u;goto L8c076460; /* mov #0x48,r0 */
L8c076460: *stop=0x8c076460u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c076462; /* fmov.s @(r0,r14),fr3 */
L8c076462: *stop=0x8c076462u;r[0]=100u;goto L8c076464; /* mov #0x64,r0 */
L8c076464: *stop=0x8c076464u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076466; /* fmov fr3,@(r0,r15) */
L8c076466: *stop=0x8c076466u;r[0]=76u;goto L8c076468; /* mov #0x4c,r0 */
L8c076468: *stop=0x8c076468u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c07646a; /* fmov.s @(r0,r14),fr3 */
L8c07646a: *stop=0x8c07646au;r[0]=104u;goto L8c07646c; /* mov #0x68,r0 */
L8c07646c: *stop=0x8c07646cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07646e; /* fmov fr3,@(r0,r15) */
L8c07646e: *stop=0x8c07646eu;r[0]=80u;goto L8c076470; /* mov #0x50,r0 */
L8c076470: *stop=0x8c076470u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c076472; /* fmov.s @(r0,r14),fr3 */
L8c076472: *stop=0x8c076472u;r[0]=108u;goto L8c076474; /* mov #0x6c,r0 */
L8c076474: *stop=0x8c076474u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076476; /* fmov fr3,@(r0,r15) */
L8c076476: *stop=0x8c076476u;r[0]=4u;goto L8c076478; /* mov #0x4,r0 */
L8c076478: *stop=0x8c076478u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c07647a; /* fmov.s @(r0,r14),fr3 */
L8c07647a: *stop=0x8c07647au;r[0]=64u;goto L8c07647c; /* mov #0x40,r0 */
L8c07647c: *stop=0x8c07647cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07647e; /* fmov fr3,@(r0,r15) */
L8c07647e: *stop=0x8c07647eu;r[0]=8u;goto L8c076480; /* mov #0x8,r0 */
L8c076480: *stop=0x8c076480u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c076482; /* fmov.s @(r0,r14),fr3 */
L8c076482: *stop=0x8c076482u;r[0]=68u;goto L8c076484; /* mov #0x44,r0 */
L8c076484: *stop=0x8c076484u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076486; /* fmov fr3,@(r0,r15) */
L8c076486: *stop=0x8c076486u;r[0]=12u;goto L8c076488; /* mov #0xc,r0 */
L8c076488: *stop=0x8c076488u;r[5]=r[15];goto L8c07648a; /* mov r15,r5 */
L8c07648a: *stop=0x8c07648au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c07648c; /* fmov.s @(r0,r14),fr3 */
L8c07648c: *stop=0x8c07648cu;r[0]=72u;goto L8c07648e; /* mov #0x48,r0 */
L8c07648e: *stop=0x8c07648eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076620u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c076490; /* mov.l 0x8c076620,r3 */
L8c076490: *stop=0x8c076490u;r[6]=r[15];goto L8c076492; /* mov r15,r6 */
L8c076492: *stop=0x8c076492u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076494; /* fmov fr3,@(r0,r15) */
L8c076494: *stop=0x8c076494u;r[0]=28u;goto L8c076496; /* mov #0x1c,r0 */
L8c076496: *stop=0x8c076496u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076498; /* fmov.s @(r0,r15),fr3 */
L8c076498: *stop=0x8c076498u;r[0]=76u;goto L8c07649a; /* mov #0x4c,r0 */
L8c07649a: *stop=0x8c07649au;r[4]=r[15];goto L8c07649c; /* mov r15,r4 */
L8c07649c: *stop=0x8c07649cu;r[5]+=64u;goto L8c07649e; /* add #0x40,r5 */
L8c07649e: *stop=0x8c07649eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0764a0; /* fmov fr3,@(r0,r15) */
L8c0764a0: *stop=0x8c0764a0u;r[0]=32u;goto L8c0764a2; /* mov #0x20,r0 */
L8c0764a2: *stop=0x8c0764a2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0764a4; /* fmov.s @(r0,r15),fr3 */
L8c0764a4: *stop=0x8c0764a4u;r[0]=80u;goto L8c0764a6; /* mov #0x50,r0 */
L8c0764a6: *stop=0x8c0764a6u;r[6]+=40u;goto L8c0764a8; /* add #0x28,r6 */
L8c0764a8: *stop=0x8c0764a8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0764aa; /* fmov fr3,@(r0,r15) */
L8c0764aa: *stop=0x8c0764aau;r[0]=36u;goto L8c0764ac; /* mov #0x24,r0 */
L8c0764ac: *stop=0x8c0764acu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0764ae; /* fmov.s @(r0,r15),fr3 */
L8c0764ae: *stop=0x8c0764aeu;r[0]=84u;goto L8c0764b0; /* mov #0x54,r0 */
L8c0764b0: *stop=0x8c0764b0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0764b2; /* fmov fr3,@(r0,r15) */
L8c0764b2: target=r[3];*pr=0x8c0764b6u;*stop=0x8c0764b4u;r[4]+=88u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0764b6; /* jsr @r3 */
L8c0764b6: *stop=0x8c0764b6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0764b8; /* mov.l @r15,r2 */
L8c0764b8: *stop=0x8c0764b8u;r[3]=r[15];goto L8c0764ba; /* mov r15,r3 */
L8c0764ba: *stop=0x8c0764bau;r[3]+=16u;goto L8c0764bc; /* add #0x10,r3 */
L8c0764bc: *stop=0x8c0764bcu;r[4]=r[15];goto L8c0764be; /* mov r15,r4 */
L8c0764be: *stop=0x8c0764beu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c0764c0; /* mov.l @r2,r1 */
L8c0764c0: *stop=0x8c0764c0u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0764c2; /* mov.l @(0x4,r2),r0 */
L8c0764c2: *stop=0x8c0764c2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c0764c4; /* mov.l r1,@r3 */
L8c0764c4: *stop=0x8c0764c4u;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[0],4);if(io_status)return io_status;goto L8c0764c6; /* mov.l r0,@(0x4,r3) */
L8c0764c6: *stop=0x8c0764c6u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0764c8; /* mov.l @(0x8,r2),r1 */
L8c0764c8: *stop=0x8c0764c8u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c0764ca; /* mov.l r1,@(0x8,r3) */
L8c0764ca: target=r[11];*pr=0x8c0764ceu;*stop=0x8c0764ccu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0764ce; /* jsr @r11 */
L8c0764ce: *stop=0x8c0764ceu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0764d0; /* fldi0 fr3 */
L8c0764d0: *stop=0x8c0764d0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c0764d2; /* fcmp/eq fr3,fr0 */
L8c0764d2: if((*sr&1u))goto L8c0764da;goto L8c0764d4; /* bt 0x8c0764da */
L8c0764d4: *stop=0x8c0764d4u;r[4]=r[15];goto L8c0764d6; /* mov r15,r4 */
L8c0764d6: target=r[10];*pr=0x8c0764dau;*stop=0x8c0764d8u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0764da; /* jsr @r10 */
L8c0764da: *stop=0x8c0764dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c076624u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0764dc; /* mov.l 0x8c076624,r2 */
L8c0764dc: *stop=0x8c0764dcu;r[5]=r[15];goto L8c0764de; /* mov r15,r5 */
L8c0764de: *stop=0x8c0764deu;r[4]=r[15];goto L8c0764e0; /* mov r15,r4 */
L8c0764e0: *stop=0x8c0764e0u;r[5]+=16u;goto L8c0764e2; /* add #0x10,r5 */
L8c0764e2: target=r[2];*pr=0x8c0764e6u;*stop=0x8c0764e4u;r[4]+=52u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0764e6; /* jsr @r2 */
L8c0764e6: *stop=0x8c0764e6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0764e8; /* fldi0 fr3 */
L8c0764e8: *stop=0x8c0764e8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0764ea; /* fcmp/gt fr3,fr0 */
L8c0764ea: if(!(*sr&1u))goto L8c07650a;goto L8c0764ec; /* bf 0x8c07650a */
L8c0764ec: *stop=0x8c0764ecu;r[0]=52u;goto L8c0764ee; /* mov #0x34,r0 */
L8c0764ee: *stop=0x8c0764eeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0764f0; /* fmov.s @(r0,r15),fr2 */
L8c0764f0: *stop=0x8c0764f0u;r[0]=52u;goto L8c0764f2; /* mov #0x34,r0 */
L8c0764f2: *stop=0x8c0764f2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c0764f4; /* fneg fr2 */
L8c0764f4: *stop=0x8c0764f4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0764f6; /* fmov fr2,@(r0,r15) */
L8c0764f6: *stop=0x8c0764f6u;r[0]=56u;goto L8c0764f8; /* mov #0x38,r0 */
L8c0764f8: *stop=0x8c0764f8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c0764fa; /* fmov.s @(r0,r15),fr2 */
L8c0764fa: *stop=0x8c0764fau;r[0]=56u;goto L8c0764fc; /* mov #0x38,r0 */
L8c0764fc: *stop=0x8c0764fcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c0764fe; /* fneg fr2 */
L8c0764fe: *stop=0x8c0764feu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c076500; /* fmov fr2,@(r0,r15) */
L8c076500: *stop=0x8c076500u;r[0]=60u;goto L8c076502; /* mov #0x3c,r0 */
L8c076502: *stop=0x8c076502u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c076504; /* fmov.s @(r0,r15),fr2 */
L8c076504: *stop=0x8c076504u;r[0]=60u;goto L8c076506; /* mov #0x3c,r0 */
L8c076506: *stop=0x8c076506u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c076508; /* fneg fr2 */
L8c076508: *stop=0x8c076508u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c07650a; /* fmov fr2,@(r0,r15) */
L8c07650a: *stop=0x8c07650au;if(!valid_access(r[13]+16u,2u))return -3;r[0]=sx(m,bus,&io_status,r[13]+0x10u,2);if(io_status)return io_status;goto L8c07650c; /* mov.w @(0x10,r13),r0 */
L8c07650c: *stop=0x8c07650cu;*sr=(*sr&~1u)|!(r[0]&0x3u);goto L8c07650e; /* tst #0x3,r0 */
L8c07650e: if(!(*sr&1u))goto L8c076516;goto L8c076510; /* bf 0x8c076516 */
L8c076510: *stop=0x8c076510u;r[4]=r[14];goto L8c076512; /* mov r14,r4 */
L8c076512: *stop=0x8c076514u;r[4]+=108u;goto L8c07651a; /* bra 0x8c07651a */
L8c076516: *stop=0x8c076516u;r[4]=r[14];goto L8c076518; /* mov r14,r4 */
L8c076518: *stop=0x8c076518u;r[4]+=16u;goto L8c07651a; /* add #0x10,r4 */
L8c07651a: target=r[11];*pr=0x8c07651eu;*stop=0x8c07651cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07651e; /* jsr @r11 */
L8c07651e: *stop=0x8c07651eu;r[0]=52u;goto L8c076520; /* mov #0x34,r0 */
L8c076520: *stop=0x8c076520u;fcopy(f,2u,0u);goto L8c076522; /* fmov fr0,fr2 */
L8c076522: *stop=0x8c076522u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076524; /* fmov.s @(r0,r15),fr3 */
L8c076524: *stop=0x8c076524u;r[0]=108u;goto L8c076526; /* mov #0x6c,r0 */
L8c076526: *stop=0x8c076526u;fcopy(f,4u,0u);goto L8c076528; /* fmov fr0,fr4 */
L8c076528: *stop=0x8c076528u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c07652a; /* fmul fr3,fr2 */
L8c07652a: *stop=0x8c07652au;{uint32_t loaded=rd(m,bus,&io_status,0x8c07662cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07652c; /* mov.l 0x8c07662c,r3 */
L8c07652c: *stop=0x8c07652cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076628u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c07652e; /* mov.l 0x8c076628,r9 */
L8c07652e: *stop=0x8c07652eu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c076530; /* fmov fr2,@(r0,r14) */
L8c076530: *stop=0x8c076530u;r[0]=56u;goto L8c076532; /* mov #0x38,r0 */
L8c076532: *stop=0x8c076532u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076534; /* fmov.s @(r0,r15),fr3 */
L8c076534: *stop=0x8c076534u;r[0]=112u;goto L8c076536; /* mov #0x70,r0 */
L8c076536: *stop=0x8c076536u;fcopy(f,2u,4u);goto L8c076538; /* fmov fr4,fr2 */
L8c076538: *stop=0x8c076538u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c07653a; /* fmul fr3,fr2 */
L8c07653a: *stop=0x8c07653au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07653c; /* fmov fr2,@(r0,r14) */
L8c07653c: *stop=0x8c07653cu;r[0]=60u;goto L8c07653e; /* mov #0x3c,r0 */
L8c07653e: *stop=0x8c07653eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076540; /* fmov.s @(r0,r15),fr3 */
L8c076540: *stop=0x8c076540u;r[0]=116u;goto L8c076542; /* mov #0x74,r0 */
L8c076542: *stop=0x8c076542u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c076544; /* fmul fr3,fr4 */
L8c076544: *stop=0x8c076544u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c076546; /* fmov fr4,@(r0,r14) */
L8c076546: *stop=0x8c076546u;r[0]=96u;goto L8c076548; /* mov #0x60,r0 */
L8c076548: *stop=0x8c076548u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c07654a; /* mov.l @(r0,r13),r2 */
L8c07654a: *stop=0x8c07654au;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c07654c; /* tst r3,r2 */
L8c07654c: if((*sr&1u))goto L8c076580;goto L8c07654e; /* bt 0x8c076580 */
L8c07654e: *stop=0x8c07654eu;if(!valid_access(r[12]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x8u,1);if(io_status)return io_status;goto L8c076550; /* mov.b @(0x8,r12),r0 */
L8c076550: *stop=0x8c076550u;r[0]=r[0]&255u;goto L8c076552; /* extu.b r0,r0 */
L8c076552: *stop=0x8c076552u;r[0]<<=2;goto L8c076554; /* shll2 r0 */
L8c076554: *stop=0x8c076554u;if(!valid_access(r[0]+r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;r[13]=loaded;}goto L8c076556; /* mov.l @(r0,r9),r13 */
L8c076556: *stop=0x8c076556u;r[4]=r[13];goto L8c076558; /* mov r13,r4 */
L8c076558: target=r[11];*pr=0x8c07655cu;*stop=0x8c07655au;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07655c; /* jsr @r11 */
L8c07655c: *stop=0x8c07655cu;r[0]=52u;goto L8c07655e; /* mov #0x34,r0 */
L8c07655e: *stop=0x8c07655eu;fcopy(f,2u,0u);goto L8c076560; /* fmov fr0,fr2 */
L8c076560: *stop=0x8c076560u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076562; /* fmov.s @(r0,r15),fr3 */
L8c076562: *stop=0x8c076562u;r[0]=4u;goto L8c076564; /* mov #0x4,r0 */
L8c076564: *stop=0x8c076564u;fcopy(f,4u,0u);goto L8c076566; /* fmov fr0,fr4 */
L8c076566: *stop=0x8c076566u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c076568; /* fmul fr3,fr2 */
L8c076568: *stop=0x8c076568u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c07656a; /* fmov fr2,@(r0,r13) */
L8c07656a: *stop=0x8c07656au;r[0]=56u;goto L8c07656c; /* mov #0x38,r0 */
L8c07656c: *stop=0x8c07656cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07656e; /* fmov.s @(r0,r15),fr3 */
L8c07656e: *stop=0x8c07656eu;r[0]=8u;goto L8c076570; /* mov #0x8,r0 */
L8c076570: *stop=0x8c076570u;fcopy(f,2u,4u);goto L8c076572; /* fmov fr4,fr2 */
L8c076572: *stop=0x8c076572u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c076574; /* fmul fr3,fr2 */
L8c076574: *stop=0x8c076574u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c076576; /* fmov fr2,@(r0,r13) */
L8c076576: *stop=0x8c076576u;r[0]=60u;goto L8c076578; /* mov #0x3c,r0 */
L8c076578: *stop=0x8c076578u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07657a; /* fmov.s @(r0,r15),fr3 */
L8c07657a: *stop=0x8c07657au;r[0]=12u;goto L8c07657c; /* mov #0xc,r0 */
L8c07657c: *stop=0x8c07657cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c07657e; /* fmul fr3,fr4 */
L8c07657e: *stop=0x8c07657eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c076580; /* fmov fr4,@(r0,r13) */
L8c076580: *stop=0x8c076580u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c076582; /* mov.l @(0x10,r14),r2 */
L8c076582: *stop=0x8c076582u;r[3]=r[15];goto L8c076584; /* mov r15,r3 */
L8c076584: *stop=0x8c076584u;r[3]+=16u;goto L8c076586; /* add #0x10,r3 */
L8c076586: *stop=0x8c076586u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c076588; /* mov.l @(0x14,r14),r1 */
L8c076588: *stop=0x8c076588u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c07658a; /* mov.l r2,@r3 */
L8c07658a: *stop=0x8c07658au;r[4]=r[15];goto L8c07658c; /* mov r15,r4 */
L8c07658c: *stop=0x8c07658cu;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c07658e; /* mov.l r1,@(0x4,r3) */
L8c07658e: *stop=0x8c07658eu;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c076590; /* mov.l @(0x18,r14),r2 */
L8c076590: *stop=0x8c076590u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[2],4);if(io_status)return io_status;goto L8c076592; /* mov.l r2,@(0x8,r3) */
L8c076592: target=r[11];*pr=0x8c076596u;*stop=0x8c076594u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c076596; /* jsr @r11 */
L8c076596: *stop=0x8c076596u;r[0]=0x8c076630u;goto L8c076598; /* mova 0x8c076630,r0 */
L8c076598: *stop=0x8c076598u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c07659a; /* fmov.s @r0,fr3 */
L8c07659a: *stop=0x8c07659au;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c07659c; /* fcmp/gt fr3,fr0 */
L8c07659c: if(!(*sr&1u))goto L8c0765d4;goto L8c07659e; /* bf 0x8c0765d4 */
L8c07659e: *stop=0x8c07659eu;r[2]=r[14];goto L8c0765a0; /* mov r14,r2 */
L8c0765a0: *stop=0x8c0765a0u;r[2]+=108u;goto L8c0765a2; /* add #0x6c,r2 */
L8c0765a2: *stop=0x8c0765a2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c0765a4; /* mov.l @r2,r1 */
L8c0765a4: *stop=0x8c0765a4u;r[3]=r[15];goto L8c0765a6; /* mov r15,r3 */
L8c0765a6: *stop=0x8c0765a6u;r[3]+=4u;goto L8c0765a8; /* add #0x4,r3 */
L8c0765a8: *stop=0x8c0765a8u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0765aa; /* mov.l @(0x4,r2),r0 */
L8c0765aa: *stop=0x8c0765aau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c0765ac; /* mov.l r1,@r3 */
L8c0765ac: *stop=0x8c0765acu;r[4]=r[15];goto L8c0765ae; /* mov r15,r4 */
L8c0765ae: *stop=0x8c0765aeu;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[0],4);if(io_status)return io_status;goto L8c0765b0; /* mov.l r0,@(0x4,r3) */
L8c0765b0: *stop=0x8c0765b0u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0765b2; /* mov.l @(0x8,r2),r1 */
L8c0765b2: *stop=0x8c0765b2u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c0765b4; /* mov.l r1,@(0x8,r3) */
L8c0765b4: target=r[10];*pr=0x8c0765b8u;*stop=0x8c0765b6u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0765b8; /* jsr @r10 */
L8c0765b8: *stop=0x8c0765b8u;r[4]=r[15];goto L8c0765ba; /* mov r15,r4 */
L8c0765ba: target=r[10];*pr=0x8c0765beu;*stop=0x8c0765bcu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0765be; /* jsr @r10 */
L8c0765be: *stop=0x8c0765beu;r[5]=r[15];goto L8c0765c0; /* mov r15,r5 */
L8c0765c0: *stop=0x8c0765c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c076634u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0765c2; /* mov.l 0x8c076634,r3 */
L8c0765c2: *stop=0x8c0765c2u;r[6]=r[14];goto L8c0765c4; /* mov r14,r6 */
L8c0765c4: *stop=0x8c0765c4u;r[4]=r[15];goto L8c0765c6; /* mov r15,r4 */
L8c0765c6: *stop=0x8c0765c6u;r[6]+=28u;goto L8c0765c8; /* add #0x1c,r6 */
L8c0765c8: *stop=0x8c0765c8u;r[5]+=4u;goto L8c0765ca; /* add #0x4,r5 */
L8c0765ca: target=r[3];*pr=0x8c0765ceu;*stop=0x8c0765ccu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0765ce; /* jsr @r3 */
L8c0765ce: *stop=0x8c0765ceu;if(!valid_access(r[12]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0765d0; /* mov.l @(0x18,r12),r2 */
L8c0765d0: *stop=0x8c0765d0u;r[2]-=r[0];goto L8c0765d2; /* sub r0,r2 */
L8c0765d2: *stop=0x8c0765d2u;if(!valid_access(r[12]+24u,4u))return -3;wr(m,bus,&io_status,r[12]+0x18u,r[2],4);if(io_status)return io_status;goto L8c0765d4; /* mov.l r2,@(0x18,r12) */
L8c0765d4: *stop=0x8c0765d4u;if(!valid_access(r[12]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x8u,1);if(io_status)return io_status;goto L8c0765d6; /* mov.b @(0x8,r12),r0 */
L8c0765d6: *stop=0x8c0765d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c076638u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0765d8; /* mov.l 0x8c076638,r3 */
L8c0765d8: *stop=0x8c0765d8u;r[0]=r[0]&255u;goto L8c0765da; /* extu.b r0,r0 */
L8c0765da: *stop=0x8c0765dau;if(!valid_access(r[12]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x18u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0765dc; /* mov.l @(0x18,r12),r5 */
L8c0765dc: *stop=0x8c0765dcu;r[0]<<=2;goto L8c0765de; /* shll2 r0 */
L8c0765de: *stop=0x8c0765deu;if(!valid_access(r[0]+r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c0765e0; /* mov.l @(r0,r9),r4 */
L8c0765e0: target=r[3];*pr=0x8c0765e4u;*stop=0x8c0765e2u;if(!valid_access(r[4]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x20u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0765e4; /* jsr @r3 */
L8c0765e4: *stop=0x8c0765e4u;r[2]=96u;goto L8c0765e6; /* mov #0x60,r2 */
L8c0765e6: *stop=0x8c0765e6u;r[2]<<=8;goto L8c0765e8; /* shll8 r2 */
L8c0765e8: *stop=0x8c0765e8u;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[2]);goto L8c0765ea; /* cmp/gt r2,r0 */
L8c0765ea: if(!(*sr&1u))goto L8c0765f4;goto L8c0765ec; /* bf 0x8c0765f4 */
L8c0765ec: *stop=0x8c0765ecu;if(!valid_access(r[12]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0765ee; /* mov.l @(0x18,r12),r1 */
L8c0765ee: *stop=0x8c0765eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c07663cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0765f0; /* mov.l 0x8c07663c,r3 */
L8c0765f0: *stop=0x8c0765f0u;r[1]+=r[3];goto L8c0765f2; /* add r3,r1 */
L8c0765f2: *stop=0x8c0765f2u;if(!valid_access(r[12]+24u,4u))return -3;wr(m,bus,&io_status,r[12]+0x18u,r[1],4);if(io_status)return io_status;goto L8c0765f4; /* mov.l r1,@(0x18,r12) */
L8c0765f4: *stop=0x8c0765f4u;if(!valid_access(r[12]+8u,1u))return -3;r[0]=sx(m,bus,&io_status,r[12]+0x8u,1);if(io_status)return io_status;goto L8c0765f6; /* mov.b @(0x8,r12),r0 */
L8c0765f6: *stop=0x8c0765f6u;if(!valid_access(r[12]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0765f8; /* mov.l @(0x18,r12),r2 */
L8c0765f8: *stop=0x8c0765f8u;r[0]=r[0]&255u;goto L8c0765fa; /* extu.b r0,r0 */
L8c0765fa: *stop=0x8c0765fau;r[0]<<=2;goto L8c0765fc; /* shll2 r0 */
L8c0765fc: *stop=0x8c0765fcu;if(!valid_access(r[0]+r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c0765fe; /* mov.l @(r0,r9),r3 */
L8c0765fe: *stop=0x8c0765feu;if(!valid_access(r[3]+32u,4u))return -3;wr(m,bus,&io_status,r[3]+0x20u,r[2],4);if(io_status)return io_status;goto L8c076600; /* mov.l r2,@(0x20,r3) */
L8c076600: *stop=0x8c076600u;r[15]+=112u;goto L8c076602; /* add #0x70,r15 */
L8c076602: *stop=0x8c076602u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c076604; /* lds.l @r15+,PR */
L8c076604: *stop=0x8c076604u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c076606; /* mov.l @r15+,r9 */
L8c076606: *stop=0x8c076606u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c076608; /* mov.l @r15+,r10 */
L8c076608: *stop=0x8c076608u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c07660a; /* mov.l @r15+,r11 */
L8c07660a: *stop=0x8c07660au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c07660c; /* mov.l @r15+,r12 */
L8c07660c: *stop=0x8c07660cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c07660e; /* mov.l @r15+,r13 */
L8c07660e: *stop=0x8c07660eu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c076610u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_surface_resolution_8c0763a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0763c6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0763c6u);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0763e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0763e2u);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0763f6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0763f6u);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c076446(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c076446u);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0764b6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0764b6u);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0764ce(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0764ceu);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0764da(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0764dau);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0764e6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0764e6u);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c07651e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07651eu);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c07655c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07655cu);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c076596(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c076596u);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0765b8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0765b8u);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0765be(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0765beu);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0765ce(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0765ceu);}
SA_STAGE_EXPORT int sa_movement_surface_resolution_resume_8c0765e4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_surface_resolution_run_8c0763a0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0765e4u);}

SA_STAGE_EXPORT int sa_movement_surface_resolution_8c07256a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c07256a;
L8c07256a: *stop=0x8c07256au;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c07256c; /* tst r5,r5 */
L8c07256c: take=(*sr&1u);*stop=0x8c07256eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;if(take)goto L8c07257c;goto L8c072570; /* bt/s 0x8c07257c */
L8c072570: *stop=0x8c072570u;r[0]=64u;goto L8c072572; /* mov #0x40,r0 */
L8c072572: *stop=0x8c072572u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c072574; /* fmov fr4,@(r0,r5) */
L8c072574: *stop=0x8c072574u;r[0]=60u;goto L8c072576; /* mov #0x3c,r0 */
L8c072576: *stop=0x8c072576u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c072578; /* fmov fr4,@(r0,r5) */
L8c072578: *stop=0x8c072578u;r[0]=56u;goto L8c07257a; /* mov #0x38,r0 */
L8c07257a: *stop=0x8c07257au;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c07257c; /* fmov fr4,@(r0,r5) */
L8c07257c: *stop=0x8c07257cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c07257e; /* tst r4,r4 */
L8c07257e: if((*sr&1u))goto L8c07258c;goto L8c072580; /* bt 0x8c07258c */
L8c072580: *stop=0x8c072580u;r[0]=12u;goto L8c072582; /* mov #0xc,r0 */
L8c072582: *stop=0x8c072582u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c072584; /* fmov fr4,@(r0,r4) */
L8c072584: *stop=0x8c072584u;r[0]=8u;goto L8c072586; /* mov #0x8,r0 */
L8c072586: *stop=0x8c072586u;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c072588; /* fmov fr4,@(r0,r4) */
L8c072588: *stop=0x8c072588u;r[0]=4u;goto L8c07258a; /* mov #0x4,r0 */
L8c07258a: *stop=0x8c07258au;if(!valid_access(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c07258c; /* fmov fr4,@(r0,r4) */
L8c07258c: *stop=0x8c07258cu;if(*pr!=entry_pr)return -3;*stop=0x8c07258eu;return 0; /* rts */
}
