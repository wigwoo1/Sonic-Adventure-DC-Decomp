/* Canonical matrix inverse and determinant through the current runtime ABI.
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
static int sa_collision_inverse_run_8c6390b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c6390bau)goto L8c6390ba;goto L8c6390b0;
L8c6390b0: *stop=0x8c6390b0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6390b2; /* mov.l r14,@-r15 */
L8c6390b2: *stop=0x8c6390b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6390e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6390b4; /* mov.l 0x8c6390e4,r3 */
L8c6390b4: *stop=0x8c6390b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6390b6; /* sts.l PR,@-r15 */
L8c6390b6: target=r[3];*pr=0x8c6390bau;*stop=0x8c6390b8u;r[14]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6390ba; /* jsr @r3 */
L8c6390ba: *stop=0x8c6390bau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0;goto L8c6390bc; /* fldi0 fr1 */
L8c6390bc: *stop=0x8c6390bcu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,0);if(status)return status;goto L8c6390be; /* fcmp/eq fr0,fr1 */
L8c6390be: if(!(*sr&1u))goto L8c639112;goto L8c6390c0; /* bf 0x8c639112 */
L8c6390c0: *stop=0x8c6390c0u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6390c2; /* tst r14,r14 */
L8c6390c2: *stop=0x8c6390c2u;r[0]=0x8c6390e8u;goto L8c6390c4; /* mova 0x8c6390e8,r0 */
L8c6390c4: *stop=0x8c6390c4u;r[4]=r[14];goto L8c6390c6; /* mov r14,r4 */
L8c6390c6: *stop=0x8c6390c6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c6390c8; /* fmov.s @r0,fr4 */
L8c6390c8: take=!(*sr&1u);*stop=0x8c6390cau;fcopy(f,5u,4u);if(take)goto L8c6390ec;goto L8c6390cc; /* bf/s 0x8c6390ec */
L8c6390cc: *stop=0x8c6390ccu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6390ce; /* fschg */
L8c6390ce: *stop=0x8c6390ceu;fcopy(f,1u,4u);goto L8c6390d0; /* fmov fr4,fr1 */
L8c6390d0: *stop=0x8c6390d0u;fcopy(f,3u,4u);goto L8c6390d2; /* fmov fr4,fr3 */
L8c6390d2: *stop=0x8c6390d2u;fcopy(f,5u,4u);goto L8c6390d4; /* fmov fr4,fr5 */
L8c6390d4: *stop=0x8c6390d4u;fcopy(f,7u,4u);goto L8c6390d6; /* fmov fr4,fr7 */
L8c6390d6: *stop=0x8c6390d6u;fcopy(f,9u,4u);goto L8c6390d8; /* fmov fr4,fr9 */
L8c6390d8: *stop=0x8c6390d8u;fcopy(f,11u,4u);goto L8c6390da; /* fmov fr4,fr11 */
L8c6390da: *stop=0x8c6390dau;fcopy(f,13u,4u);goto L8c6390dc; /* fmov fr4,fr13 */
L8c6390dc: *stop=0x8c6390dcu;fcopy(f,15u,4u);goto L8c6390de; /* fmov fr4,fr15 */
L8c6390de: *stop=0x8c6390deu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6390e0; /* fschg */
L8c6390e0: *stop=0x8c6390e2u;r[0]=0u;goto L8c6398a8; /* bra 0x8c6398a8 */
L8c6390ec: *stop=0x8c6390ecu;r[4]+=64u;goto L8c6390ee; /* add #0x40,r4 */
L8c6390ee: *stop=0x8c6390eeu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c6390f0; /* fmov.s fr4,@-r4 */
L8c6390f0: *stop=0x8c6390f0u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c6390f2; /* fmov.s fr4,@-r4 */
L8c6390f2: *stop=0x8c6390f2u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c6390f4; /* fmov.s fr4,@-r4 */
L8c6390f4: *stop=0x8c6390f4u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c6390f6; /* fmov.s fr4,@-r4 */
L8c6390f6: *stop=0x8c6390f6u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c6390f8; /* fmov.s fr4,@-r4 */
L8c6390f8: *stop=0x8c6390f8u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c6390fa; /* fmov.s fr4,@-r4 */
L8c6390fa: *stop=0x8c6390fau;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c6390fc; /* fmov.s fr4,@-r4 */
L8c6390fc: *stop=0x8c6390fcu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c6390fe; /* fmov.s fr4,@-r4 */
L8c6390fe: *stop=0x8c6390feu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c639100; /* fmov.s fr4,@-r4 */
L8c639100: *stop=0x8c639100u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c639102; /* fmov.s fr4,@-r4 */
L8c639102: *stop=0x8c639102u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c639104; /* fmov.s fr4,@-r4 */
L8c639104: *stop=0x8c639104u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c639106; /* fmov.s fr4,@-r4 */
L8c639106: *stop=0x8c639106u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c639108; /* fmov.s fr4,@-r4 */
L8c639108: *stop=0x8c639108u;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c63910a; /* fmov.s fr4,@-r4 */
L8c63910a: *stop=0x8c63910au;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c63910c; /* fmov.s fr4,@-r4 */
L8c63910c: *stop=0x8c63910cu;if(!valid_access(r[4]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[4]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;goto L8c63910e; /* fmov.s fr4,@-r4 */
L8c63910e: *stop=0x8c639110u;r[0]=0u;goto L8c6398a8; /* bra 0x8c6398a8 */
L8c639112: *stop=0x8c639112u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]=0x3f800000u;goto L8c639114; /* fldi1 fr1 */
L8c639114: *stop=0x8c639114u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c639116; /* fdiv fr0,fr1 */
L8c639116: *stop=0x8c639116u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c639118; /* fmov.s fr12,@-r15 */
L8c639118: *stop=0x8c639118u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c63911a; /* fmov.s fr13,@-r15 */
L8c63911a: *stop=0x8c63911au;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c63911c; /* tst r14,r14 */
L8c63911c: *stop=0x8c63911cu;*fpul=f->bank[(f->fpscr>>21)&1u][1];goto L8c63911e; /* flds fr1,FPUL */
L8c63911e: take=(*sr&1u);*stop=0x8c639120u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;if(take)goto L8c639126;goto L8c639122; /* bt/s 0x8c639126 */
L8c639122: *stop=0x8c639124u;goto L8c63949a; /* bra 0x8c63949a */
L8c639126: *stop=0x8c639126u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639128; /* fschg */
L8c639128: *stop=0x8c639128u;fcopy(f,0u,9u);goto L8c63912a; /* fmov fr9,fr0 */
L8c63912a: *stop=0x8c63912au;fcopy(f,2u,11u);goto L8c63912c; /* fmov fr11,fr2 */
L8c63912c: *stop=0x8c63912cu;fcopy(f,4u,13u);goto L8c63912e; /* fmov fr13,fr4 */
L8c63912e: *stop=0x8c63912eu;fcopy(f,6u,15u);goto L8c639130; /* fmov fr15,fr6 */
L8c639130: *stop=0x8c639130u;fcopy(f,8u,5u);goto L8c639132; /* fmov fr5,fr8 */
L8c639132: *stop=0x8c639132u;fcopy(f,10u,7u);goto L8c639134; /* fmov fr7,fr10 */
L8c639134: *stop=0x8c639134u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639136; /* fschg */
L8c639136: *stop=0x8c639136u;fcopy(f,0u,1u);goto L8c639138; /* fmov fr1,fr0 */
L8c639138: *stop=0x8c639138u;fcopy(f,4u,2u);goto L8c63913a; /* fmov fr2,fr4 */
L8c63913a: *stop=0x8c63913au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63913c; /* fmul fr5,fr2 */
L8c63913c: *stop=0x8c63913cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[7],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63913e; /* fmul fr7,fr4 */
L8c63913e: *stop=0x8c63913eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c639140; /* fmul fr3,fr5 */
L8c639140: *stop=0x8c639140u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c639142; /* fmul fr6,fr1 */
L8c639142: *stop=0x8c639142u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[7],&f->fpscr,&fr[0]);if(status)return status;}goto L8c639144; /* fmul fr7,fr0 */
L8c639144: *stop=0x8c639144u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c639146; /* fmul fr6,fr3 */
L8c639146: *stop=0x8c639146u;fcopy(f,6u,9u);goto L8c639148; /* fmov fr9,fr6 */
L8c639148: *stop=0x8c639148u;fcopy(f,7u,10u);goto L8c63914a; /* fmov fr10,fr7 */
L8c63914a: *stop=0x8c63914au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[4],&f->fpscr,&fr[6]);if(status)return status;}goto L8c63914c; /* fmul fr4,fr6 */
L8c63914c: *stop=0x8c63914cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[5],&f->fpscr,&fr[7]);if(status)return status;}goto L8c63914e; /* fmul fr5,fr7 */
L8c63914e: *stop=0x8c63914eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[0],&f->fpscr,&fr[10]);if(status)return status;}goto L8c639150; /* fmul fr0,fr10 */
L8c639150: *stop=0x8c639150u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c639152; /* fadd fr7,fr6 */
L8c639152: *stop=0x8c639152u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[3],&f->fpscr,&fr[9]);if(status)return status;}goto L8c639154; /* fmul fr3,fr9 */
L8c639154: *stop=0x8c639154u;fcopy(f,7u,11u);goto L8c639156; /* fmov fr11,fr7 */
L8c639156: *stop=0x8c639156u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[1],&f->fpscr,&fr[11]);if(status)return status;}goto L8c639158; /* fmul fr1,fr11 */
L8c639158: *stop=0x8c639158u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[2],&f->fpscr,&fr[7]);if(status)return status;}goto L8c63915a; /* fmul fr2,fr7 */
L8c63915a: *stop=0x8c63915au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[11],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c63915c; /* fadd fr11,fr6 */
L8c63915c: *stop=0x8c63915cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c63915e; /* fsub fr7,fr6 */
L8c63915e: *stop=0x8c63915eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[10],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c639160; /* fsub fr10,fr6 */
L8c639160: *stop=0x8c639160u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[9],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c639162; /* fsub fr9,fr6 */
L8c639162: *stop=0x8c639162u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639164; /* fschg */
L8c639164: *stop=0x8c639164u;fcopy(f,8u,1u);goto L8c639166; /* fmov fr1,fr8 */
L8c639166: *stop=0x8c639166u;fcopy(f,10u,3u);goto L8c639168; /* fmov fr3,fr10 */
L8c639168: *stop=0x8c639168u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63916a; /* fschg */
L8c63916a: *stop=0x8c63916au;fcopy(f,8u,9u);goto L8c63916c; /* fmov fr9,fr8 */
L8c63916c: *stop=0x8c63916cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[10],&f->fpscr,&fr[5]);if(status)return status;}goto L8c63916e; /* fmul fr10,fr5 */
L8c63916e: *stop=0x8c63916eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[4],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639170; /* fmul fr4,fr8 */
L8c639170: *stop=0x8c639170u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[11],&f->fpscr,&fr[1]);if(status)return status;}goto L8c639172; /* fmul fr11,fr1 */
L8c639172: *stop=0x8c639172u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[10],&f->fpscr,&fr[0]);if(status)return status;}goto L8c639174; /* fmul fr10,fr0 */
L8c639174: *stop=0x8c639174u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[11],&f->fpscr,&fr[2]);if(status)return status;}goto L8c639176; /* fmul fr11,fr2 */
L8c639176: *stop=0x8c639176u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[3],&f->fpscr,&fr[9]);if(status)return status;}goto L8c639178; /* fmul fr3,fr9 */
L8c639178: *stop=0x8c639178u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[8],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c63917a; /* fadd fr8,fr5 */
L8c63917a: *stop=0x8c63917au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[5],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c63917c; /* fadd fr5,fr1 */
L8c63917c: *stop=0x8c63917cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c63917e; /* fsub fr2,fr1 */
L8c63917e: *stop=0x8c63917eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[0],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c639180; /* fsub fr0,fr1 */
L8c639180: *stop=0x8c639180u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[9],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c639182; /* fsub fr9,fr1 */
L8c639182: *stop=0x8c639182u;fcopy(f,0u,6u);goto L8c639184; /* fmov fr6,fr0 */
L8c639184: *stop=0x8c639184u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto L8c639186; /* fneg fr1 */
L8c639186: *stop=0x8c639186u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639188; /* fschg */
L8c639188: *stop=0x8c639188u;fcopy(f,6u,9u);goto L8c63918a; /* fmov fr9,fr6 */
L8c63918a: *stop=0x8c63918au;fcopy(f,8u,11u);goto L8c63918c; /* fmov fr11,fr8 */
L8c63918c: *stop=0x8c63918cu;fcopy(f,10u,13u);goto L8c63918e; /* fmov fr13,fr10 */
L8c63918e: *stop=0x8c63918eu;fcopy(f,12u,15u);goto L8c639190; /* fmov fr15,fr12 */
L8c639190: *stop=0x8c639190u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639192; /* fschg */
L8c639192: *stop=0x8c639192u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[10],&f->fpscr,&fr[9]);if(status)return status;}goto L8c639194; /* fmul fr10,fr9 */
L8c639194: *stop=0x8c639194u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[6],&f->fpscr,&fr[12]);if(status)return status;}goto L8c639196; /* fmul fr6,fr12 */
L8c639196: *stop=0x8c639196u;fcopy(f,2u,9u);goto L8c639198; /* fmov fr9,fr2 */
L8c639198: *stop=0x8c639198u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[10],&f->fpscr,&fr[8]);if(status)return status;}goto L8c63919a; /* fmul fr10,fr8 */
L8c63919a: *stop=0x8c63919au;fcopy(f,5u,12u);goto L8c63919c; /* fmov fr12,fr5 */
L8c63919c: *stop=0x8c63919cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[13],&f->fpscr,&fr[6]);if(status)return status;}goto L8c63919e; /* fmul fr13,fr6 */
L8c63919e: *stop=0x8c63919eu;fcopy(f,7u,8u);goto L8c6391a0; /* fmov fr8,fr7 */
L8c6391a0: *stop=0x8c6391a0u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6391a2; /* fschg */
L8c6391a2: *stop=0x8c6391a2u;fcopy(f,8u,5u);goto L8c6391a4; /* fmov fr5,fr8 */
L8c6391a4: *stop=0x8c6391a4u;fcopy(f,10u,7u);goto L8c6391a6; /* fmov fr7,fr10 */
L8c6391a6: *stop=0x8c6391a6u;fcopy(f,12u,7u);goto L8c6391a8; /* fmov fr7,fr12 */
L8c6391a8: *stop=0x8c6391a8u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6391aa; /* fschg */
L8c6391aa: *stop=0x8c6391aau;fcopy(f,9u,8u);goto L8c6391ac; /* fmov fr8,fr9 */
L8c6391ac: *stop=0x8c6391acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[2],&f->fpscr,&fr[12]);if(status)return status;}goto L8c6391ae; /* fmul fr2,fr12 */
L8c6391ae: *stop=0x8c6391aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[4],&f->fpscr,&fr[9]);if(status)return status;}goto L8c6391b0; /* fmul fr4,fr9 */
L8c6391b0: *stop=0x8c6391b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[5],&f->fpscr,&fr[13]);if(status)return status;}goto L8c6391b2; /* fmul fr5,fr13 */
L8c6391b2: *stop=0x8c6391b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[7],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6391b4; /* fmul fr7,fr11 */
L8c6391b4: *stop=0x8c6391b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[6],&f->fpscr,&fr[10]);if(status)return status;}goto L8c6391b6; /* fmul fr6,fr10 */
L8c6391b6: *stop=0x8c6391b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[3],&f->fpscr,&fr[8]);if(status)return status;}goto L8c6391b8; /* fmul fr3,fr8 */
L8c6391b8: *stop=0x8c6391b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[12],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c6391ba; /* fadd fr12,fr9 */
L8c6391ba: *stop=0x8c6391bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[13],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c6391bc; /* fadd fr13,fr9 */
L8c6391bc: *stop=0x8c6391bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[11],&f->fpscr,&fr[9],1);if(status)return status;}goto L8c6391be; /* fsub fr11,fr9 */
L8c6391be: *stop=0x8c6391beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[10],&f->fpscr,&fr[9],1);if(status)return status;}goto L8c6391c0; /* fsub fr10,fr9 */
L8c6391c0: *stop=0x8c6391c0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[8],&f->fpscr,&fr[9],1);if(status)return status;}goto L8c6391c2; /* fsub fr8,fr9 */
L8c6391c2: *stop=0x8c6391c2u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6391c4; /* fschg */
L8c6391c4: *stop=0x8c6391c4u;fcopy(f,10u,1u);goto L8c6391c6; /* fmov fr1,fr10 */
L8c6391c6: *stop=0x8c6391c6u;fcopy(f,12u,3u);goto L8c6391c8; /* fmov fr3,fr12 */
L8c6391c8: *stop=0x8c6391c8u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6391ca; /* fschg */
L8c6391ca: *stop=0x8c6391cau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][9]^=0x80000000u;goto L8c6391cc; /* fneg fr9 */
L8c6391cc: *stop=0x8c6391ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6391ce; /* fmul fr10,fr4 */
L8c6391ce: *stop=0x8c6391ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[12],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6391d0; /* fmul fr12,fr2 */
L8c6391d0: *stop=0x8c6391d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[13],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6391d2; /* fmul fr13,fr5 */
L8c6391d2: *stop=0x8c6391d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[12],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6391d4; /* fmul fr12,fr6 */
L8c6391d4: *stop=0x8c6391d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[13],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6391d6; /* fmul fr13,fr7 */
L8c6391d6: *stop=0x8c6391d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[10],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6391d8; /* fmul fr10,fr3 */
L8c6391d8: *stop=0x8c6391d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[2],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c6391da; /* fadd fr2,fr4 */
L8c6391da: *stop=0x8c6391dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[5],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c6391dc; /* fadd fr5,fr4 */
L8c6391dc: *stop=0x8c6391dcu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c6391de; /* fneg fr3 */
L8c6391de: *stop=0x8c6391deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[7],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c6391e0; /* fsub fr7,fr4 */
L8c6391e0: *stop=0x8c6391e0u;fcopy(f,2u,9u);goto L8c6391e2; /* fmov fr9,fr2 */
L8c6391e2: *stop=0x8c6391e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[6],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c6391e4; /* fsub fr6,fr4 */
L8c6391e4: *stop=0x8c6391e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c6391e6; /* fadd fr4,fr3 */
L8c6391e6: *stop=0x8c6391e6u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c6391e8; /* fsts FPUL,fr4 */
L8c6391e8: *stop=0x8c6391e8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6391ea; /* fmul fr4,fr0 */
L8c6391ea: *stop=0x8c6391eau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6391ec; /* fmul fr4,fr1 */
L8c6391ec: *stop=0x8c6391ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6391ee; /* fmul fr4,fr2 */
L8c6391ee: *stop=0x8c6391eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6391f0; /* fmul fr4,fr3 */
L8c6391f0: *stop=0x8c6391f0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c6391f2; /* fmov.s fr0,@-r15 */
L8c6391f2: *stop=0x8c6391f2u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c6391f4; /* fmov.s fr1,@-r15 */
L8c6391f4: *stop=0x8c6391f4u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c6391f6; /* fmov.s fr2,@-r15 */
L8c6391f6: *stop=0x8c6391f6u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c6391f8; /* fmov.s fr3,@-r15 */
L8c6391f8: *stop=0x8c6391f8u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6391fa; /* fschg */
L8c6391fa: *stop=0x8c6391fau;fcopy(f,0u,9u);goto L8c6391fc; /* fmov fr9,fr0 */
L8c6391fc: *stop=0x8c6391fcu;fcopy(f,2u,11u);goto L8c6391fe; /* fmov fr11,fr2 */
L8c6391fe: *stop=0x8c6391feu;fcopy(f,4u,13u);goto L8c639200; /* fmov fr13,fr4 */
L8c639200: *stop=0x8c639200u;fcopy(f,6u,15u);goto L8c639202; /* fmov fr15,fr6 */
L8c639202: *stop=0x8c639202u;fcopy(f,8u,5u);goto L8c639204; /* fmov fr5,fr8 */
L8c639204: *stop=0x8c639204u;fcopy(f,10u,7u);goto L8c639206; /* fmov fr7,fr10 */
L8c639206: *stop=0x8c639206u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639208; /* fschg */
L8c639208: *stop=0x8c639208u;fcopy(f,2u,1u);goto L8c63920a; /* fmov fr1,fr2 */
L8c63920a: *stop=0x8c63920au;fcopy(f,6u,5u);goto L8c63920c; /* fmov fr5,fr6 */
L8c63920c: *stop=0x8c63920cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63920e; /* fmul fr7,fr1 */
L8c63920e: *stop=0x8c63920eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c639210; /* fmul fr0,fr7 */
L8c639210: *stop=0x8c639210u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c639212; /* fmul fr5,fr0 */
L8c639212: *stop=0x8c639212u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c639214; /* fmul fr3,fr5 */
L8c639214: *stop=0x8c639214u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c639216; /* fmul fr4,fr3 */
L8c639216: *stop=0x8c639216u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c639218; /* fmul fr4,fr2 */
L8c639218: *stop=0x8c639218u;fcopy(f,4u,7u);goto L8c63921a; /* fmov fr7,fr4 */
L8c63921a: *stop=0x8c63921au;fcopy(f,12u,11u);goto L8c63921c; /* fmov fr11,fr12 */
L8c63921c: *stop=0x8c63921cu;fcopy(f,6u,8u);goto L8c63921e; /* fmov fr8,fr6 */
L8c63921e: *stop=0x8c63921eu;fcopy(f,7u,9u);goto L8c639220; /* fmov fr9,fr7 */
L8c639220: *stop=0x8c639220u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[1],&f->fpscr,&fr[6]);if(status)return status;}goto L8c639222; /* fmul fr1,fr6 */
L8c639222: *stop=0x8c639222u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[3],&f->fpscr,&fr[7]);if(status)return status;}goto L8c639224; /* fmul fr3,fr7 */
L8c639224: *stop=0x8c639224u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[0],&f->fpscr,&fr[12]);if(status)return status;}goto L8c639226; /* fmul fr0,fr12 */
L8c639226: *stop=0x8c639226u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c639228; /* fadd fr7,fr6 */
L8c639228: *stop=0x8c639228u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[2],&f->fpscr,&fr[11]);if(status)return status;}goto L8c63922a; /* fmul fr2,fr11 */
L8c63922a: *stop=0x8c63922au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[12],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c63922c; /* fadd fr12,fr6 */
L8c63922c: *stop=0x8c63922cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[4],&f->fpscr,&fr[9]);if(status)return status;}goto L8c63922e; /* fmul fr4,fr9 */
L8c63922e: *stop=0x8c63922eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[11],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c639230; /* fsub fr11,fr6 */
L8c639230: *stop=0x8c639230u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639232; /* fmul fr5,fr8 */
L8c639232: *stop=0x8c639232u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[9],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c639234; /* fsub fr9,fr6 */
L8c639234: *stop=0x8c639234u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[8],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c639236; /* fsub fr8,fr6 */
L8c639236: *stop=0x8c639236u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639238; /* fschg */
L8c639238: *stop=0x8c639238u;fcopy(f,8u,1u);goto L8c63923a; /* fmov fr1,fr8 */
L8c63923a: *stop=0x8c63923au;fcopy(f,10u,3u);goto L8c63923c; /* fmov fr3,fr10 */
L8c63923c: *stop=0x8c63923cu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63923e; /* fschg */
L8c63923e: *stop=0x8c63923eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c639240; /* fmul fr8,fr1 */
L8c639240: *stop=0x8c639240u;fcopy(f,10u,11u);goto L8c639242; /* fmov fr11,fr10 */
L8c639242: *stop=0x8c639242u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c639244; /* fmul fr9,fr3 */
L8c639244: *stop=0x8c639244u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[0],&f->fpscr,&fr[10]);if(status)return status;}goto L8c639246; /* fmul fr0,fr10 */
L8c639246: *stop=0x8c639246u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c639248; /* fadd fr3,fr1 */
L8c639248: *stop=0x8c639248u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[2],&f->fpscr,&fr[11]);if(status)return status;}goto L8c63924a; /* fmul fr2,fr11 */
L8c63924a: *stop=0x8c63924au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[10],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c63924c; /* fadd fr10,fr1 */
L8c63924c: *stop=0x8c63924cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[9],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63924e; /* fmul fr9,fr4 */
L8c63924e: *stop=0x8c63924eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c639250; /* fsub fr11,fr1 */
L8c639250: *stop=0x8c639250u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[8],&f->fpscr,&fr[5]);if(status)return status;}goto L8c639252; /* fmul fr8,fr5 */
L8c639252: *stop=0x8c639252u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[4],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c639254; /* fsub fr4,fr1 */
L8c639254: *stop=0x8c639254u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[5],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c639256; /* fsub fr5,fr1 */
L8c639256: *stop=0x8c639256u;fcopy(f,3u,6u);goto L8c639258; /* fmov fr6,fr3 */
L8c639258: *stop=0x8c639258u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto L8c63925a; /* fneg fr1 */
L8c63925a: *stop=0x8c63925au;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63925c; /* fschg */
L8c63925c: *stop=0x8c63925cu;fcopy(f,4u,9u);goto L8c63925e; /* fmov fr9,fr4 */
L8c63925e: *stop=0x8c63925eu;fcopy(f,6u,11u);goto L8c639260; /* fmov fr11,fr6 */
L8c639260: *stop=0x8c639260u;fcopy(f,8u,13u);goto L8c639262; /* fmov fr13,fr8 */
L8c639262: *stop=0x8c639262u;fcopy(f,10u,15u);goto L8c639264; /* fmov fr15,fr10 */
L8c639264: *stop=0x8c639264u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639266; /* fschg */
L8c639266: *stop=0x8c639266u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[10],&f->fpscr,&fr[5]);if(status)return status;}goto L8c639268; /* fmul fr10,fr5 */
L8c639268: *stop=0x8c639268u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[6],&f->fpscr,&fr[8]);if(status)return status;}goto L8c63926a; /* fmul fr6,fr8 */
L8c63926a: *stop=0x8c63926au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63926c; /* fmul fr10,fr4 */
L8c63926c: *stop=0x8c63926cu;fcopy(f,7u,8u);goto L8c63926e; /* fmov fr8,fr7 */
L8c63926e: *stop=0x8c63926eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[9],&f->fpscr,&fr[6]);if(status)return status;}goto L8c639270; /* fmul fr9,fr6 */
L8c639270: *stop=0x8c639270u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639272; /* fschg */
L8c639272: *stop=0x8c639272u;fcopy(f,8u,5u);goto L8c639274; /* fmov fr5,fr8 */
L8c639274: *stop=0x8c639274u;fcopy(f,10u,7u);goto L8c639276; /* fmov fr7,fr10 */
L8c639276: *stop=0x8c639276u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639278; /* fschg */
L8c639278: *stop=0x8c639278u;fcopy(f,11u,8u);goto L8c63927a; /* fmov fr8,fr11 */
L8c63927a: *stop=0x8c63927au;fcopy(f,12u,9u);goto L8c63927c; /* fmov fr9,fr12 */
L8c63927c: *stop=0x8c63927cu;fcopy(f,13u,10u);goto L8c63927e; /* fmov fr10,fr13 */
L8c63927e: *stop=0x8c63927eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[5],&f->fpscr,&fr[11]);if(status)return status;}goto L8c639280; /* fmul fr5,fr11 */
L8c639280: *stop=0x8c639280u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[7],&f->fpscr,&fr[12]);if(status)return status;}goto L8c639282; /* fmul fr7,fr12 */
L8c639282: *stop=0x8c639282u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[0],&f->fpscr,&fr[13]);if(status)return status;}goto L8c639284; /* fmul fr0,fr13 */
L8c639284: *stop=0x8c639284u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[11],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c639286; /* fadd fr11,fr12 */
L8c639286: *stop=0x8c639286u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[2],&f->fpscr,&fr[10]);if(status)return status;}goto L8c639288; /* fmul fr2,fr10 */
L8c639288: *stop=0x8c639288u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[13],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c63928a; /* fadd fr13,fr12 */
L8c63928a: *stop=0x8c63928au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[4],&f->fpscr,&fr[9]);if(status)return status;}goto L8c63928c; /* fmul fr4,fr9 */
L8c63928c: *stop=0x8c63928cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[10],&f->fpscr,&fr[12],1);if(status)return status;}goto L8c63928e; /* fsub fr10,fr12 */
L8c63928e: *stop=0x8c63928eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[6],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639290; /* fmul fr6,fr8 */
L8c639290: *stop=0x8c639290u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[9],&f->fpscr,&fr[12],1);if(status)return status;}goto L8c639292; /* fsub fr9,fr12 */
L8c639292: *stop=0x8c639292u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[8],&f->fpscr,&fr[12],1);if(status)return status;}goto L8c639294; /* fsub fr8,fr12 */
L8c639294: *stop=0x8c639294u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]^=0x80000000u;goto L8c639296; /* fneg fr12 */
L8c639296: *stop=0x8c639296u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639298; /* fschg */
L8c639298: *stop=0x8c639298u;fcopy(f,8u,1u);goto L8c63929a; /* fmov fr1,fr8 */
L8c63929a: *stop=0x8c63929au;fcopy(f,10u,3u);goto L8c63929c; /* fmov fr3,fr10 */
L8c63929c: *stop=0x8c63929cu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63929e; /* fschg */
L8c63929e: *stop=0x8c63929eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[8],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6392a0; /* fmul fr8,fr5 */
L8c6392a0: *stop=0x8c6392a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[9],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6392a2; /* fmul fr9,fr7 */
L8c6392a2: *stop=0x8c6392a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[10],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6392a4; /* fmul fr10,fr0 */
L8c6392a4: *stop=0x8c6392a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[7],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c6392a6; /* fadd fr7,fr5 */
L8c6392a6: *stop=0x8c6392a6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[10],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6392a8; /* fmul fr10,fr2 */
L8c6392a8: *stop=0x8c6392a8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[5],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c6392aa; /* fadd fr5,fr0 */
L8c6392aa: *stop=0x8c6392aau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[9],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6392ac; /* fmul fr9,fr4 */
L8c6392ac: *stop=0x8c6392acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[2],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6392ae; /* fsub fr2,fr0 */
L8c6392ae: *stop=0x8c6392aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[8],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6392b0; /* fmul fr8,fr6 */
L8c6392b0: *stop=0x8c6392b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[4],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6392b2; /* fsub fr4,fr0 */
L8c6392b2: *stop=0x8c6392b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[6],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6392b4; /* fsub fr6,fr0 */
L8c6392b4: *stop=0x8c6392b4u;f->bank[(f->fpscr>>21)&1u][8]=*fpul;goto L8c6392b6; /* fsts FPUL,fr8 */
L8c6392b6: *stop=0x8c6392b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6392b8; /* fmul fr8,fr3 */
L8c6392b8: *stop=0x8c6392b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6392ba; /* fmul fr8,fr1 */
L8c6392ba: *stop=0x8c6392bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[8],&f->fpscr,&fr[12]);if(status)return status;}goto L8c6392bc; /* fmul fr8,fr12 */
L8c6392bc: *stop=0x8c6392bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[8],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6392be; /* fmul fr8,fr0 */
L8c6392be: *stop=0x8c6392beu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c6392c0; /* fmov.s fr3,@-r15 */
L8c6392c0: *stop=0x8c6392c0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c6392c2; /* fmov.s fr1,@-r15 */
L8c6392c2: *stop=0x8c6392c2u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c6392c4; /* fmov.s fr12,@-r15 */
L8c6392c4: *stop=0x8c6392c4u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c6392c6; /* fmov.s fr0,@-r15 */
L8c6392c6: *stop=0x8c6392c6u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6392c8; /* fschg */
L8c6392c8: *stop=0x8c6392c8u;fcopy(f,0u,1u);goto L8c6392ca; /* fmov fr1,fr0 */
L8c6392ca: *stop=0x8c6392cau;fcopy(f,2u,3u);goto L8c6392cc; /* fmov fr3,fr2 */
L8c6392cc: *stop=0x8c6392ccu;fcopy(f,4u,5u);goto L8c6392ce; /* fmov fr5,fr4 */
L8c6392ce: *stop=0x8c6392ceu;fcopy(f,6u,7u);goto L8c6392d0; /* fmov fr7,fr6 */
L8c6392d0: *stop=0x8c6392d0u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6392d2; /* fschg */
L8c6392d2: *stop=0x8c6392d2u;fcopy(f,0u,1u);goto L8c6392d4; /* fmov fr1,fr0 */
L8c6392d4: *stop=0x8c6392d4u;fcopy(f,4u,7u);goto L8c6392d6; /* fmov fr7,fr4 */
L8c6392d6: *stop=0x8c6392d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6392d8; /* fmul fr6,fr0 */
L8c6392d8: *stop=0x8c6392d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6392da; /* fmul fr2,fr4 */
L8c6392da: *stop=0x8c6392dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6392dc; /* fmul fr7,fr1 */
L8c6392dc: *stop=0x8c6392dcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6392de; /* fmul fr5,fr2 */
L8c6392de: *stop=0x8c6392deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6392e0; /* fmul fr3,fr5 */
L8c6392e0: *stop=0x8c6392e0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6392e2; /* fmul fr6,fr3 */
L8c6392e2: *stop=0x8c6392e2u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6392e4; /* fschg */
L8c6392e4: *stop=0x8c6392e4u;fcopy(f,6u,13u);goto L8c6392e6; /* fmov fr13,fr6 */
L8c6392e6: *stop=0x8c6392e6u;fcopy(f,8u,15u);goto L8c6392e8; /* fmov fr15,fr8 */
L8c6392e8: *stop=0x8c6392e8u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6392ea; /* fschg */
L8c6392ea: *stop=0x8c6392eau;fcopy(f,6u,9u);goto L8c6392ec; /* fmov fr9,fr6 */
L8c6392ec: *stop=0x8c6392ecu;fcopy(f,10u,7u);goto L8c6392ee; /* fmov fr7,fr10 */
L8c6392ee: *stop=0x8c6392eeu;fcopy(f,11u,8u);goto L8c6392f0; /* fmov fr8,fr11 */
L8c6392f0: *stop=0x8c6392f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6392f2; /* fmul fr0,fr6 */
L8c6392f2: *stop=0x8c6392f2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[4],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6392f4; /* fmul fr4,fr7 */
L8c6392f4: *stop=0x8c6392f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c6392f6; /* fmul fr5,fr8 */
L8c6392f6: *stop=0x8c6392f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c6392f8; /* fadd fr7,fr6 */
L8c6392f8: *stop=0x8c6392f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[3],&f->fpscr,&fr[10]);if(status)return status;}goto L8c6392fa; /* fmul fr3,fr10 */
L8c6392fa: *stop=0x8c6392fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[8],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c6392fc; /* fadd fr8,fr6 */
L8c6392fc: *stop=0x8c6392fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[2],&f->fpscr,&fr[9]);if(status)return status;}goto L8c6392fe; /* fmul fr2,fr9 */
L8c6392fe: *stop=0x8c6392feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[10],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c639300; /* fsub fr10,fr6 */
L8c639300: *stop=0x8c639300u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[1],&f->fpscr,&fr[11]);if(status)return status;}goto L8c639302; /* fmul fr1,fr11 */
L8c639302: *stop=0x8c639302u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[9],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c639304; /* fsub fr9,fr6 */
L8c639304: *stop=0x8c639304u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[11],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c639306; /* fsub fr11,fr6 */
L8c639306: *stop=0x8c639306u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639308; /* fschg */
L8c639308: *stop=0x8c639308u;fcopy(f,8u,9u);goto L8c63930a; /* fmov fr9,fr8 */
L8c63930a: *stop=0x8c63930au;fcopy(f,10u,11u);goto L8c63930c; /* fmov fr11,fr10 */
L8c63930c: *stop=0x8c63930cu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63930e; /* fschg */
L8c63930e: *stop=0x8c63930eu;fcopy(f,8u,9u);goto L8c639310; /* fmov fr9,fr8 */
L8c639310: *stop=0x8c639310u;fcopy(f,12u,9u);goto L8c639312; /* fmov fr9,fr12 */
L8c639312: *stop=0x8c639312u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[11],&f->fpscr,&fr[0]);if(status)return status;}goto L8c639314; /* fmul fr11,fr0 */
L8c639314: *stop=0x8c639314u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[4],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639316; /* fmul fr4,fr8 */
L8c639316: *stop=0x8c639316u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[10],&f->fpscr,&fr[5]);if(status)return status;}goto L8c639318; /* fmul fr10,fr5 */
L8c639318: *stop=0x8c639318u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[8],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c63931a; /* fadd fr8,fr0 */
L8c63931a: *stop=0x8c63931au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[3],&f->fpscr,&fr[9]);if(status)return status;}goto L8c63931c; /* fmul fr3,fr9 */
L8c63931c: *stop=0x8c63931cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[5],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c63931e; /* fadd fr5,fr0 */
L8c63931e: *stop=0x8c63931eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[11],&f->fpscr,&fr[2]);if(status)return status;}goto L8c639320; /* fmul fr11,fr2 */
L8c639320: *stop=0x8c639320u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[9],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c639322; /* fsub fr9,fr0 */
L8c639322: *stop=0x8c639322u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[10],&f->fpscr,&fr[1]);if(status)return status;}goto L8c639324; /* fmul fr10,fr1 */
L8c639324: *stop=0x8c639324u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[2],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c639326; /* fsub fr2,fr0 */
L8c639326: *stop=0x8c639326u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c639328; /* fsub fr1,fr0 */
L8c639328: *stop=0x8c639328u;fcopy(f,2u,4u);goto L8c63932a; /* fmov fr4,fr2 */
L8c63932a: *stop=0x8c63932au;fcopy(f,1u,6u);goto L8c63932c; /* fmov fr6,fr1 */
L8c63932c: *stop=0x8c63932cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;goto L8c63932e; /* fneg fr0 */
L8c63932e: *stop=0x8c63932eu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639330; /* fschg */
L8c639330: *stop=0x8c639330u;fcopy(f,4u,1u);goto L8c639332; /* fmov fr1,fr4 */
L8c639332: *stop=0x8c639332u;fcopy(f,6u,3u);goto L8c639334; /* fmov fr3,fr6 */
L8c639334: *stop=0x8c639334u;fcopy(f,8u,5u);goto L8c639336; /* fmov fr5,fr8 */
L8c639336: *stop=0x8c639336u;fcopy(f,10u,7u);goto L8c639338; /* fmov fr7,fr10 */
L8c639338: *stop=0x8c639338u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63933a; /* fschg */
L8c63933a: *stop=0x8c63933au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[8],&f->fpscr,&fr[7]);if(status)return status;}goto L8c63933c; /* fmul fr8,fr7 */
L8c63933c: *stop=0x8c63933cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[8],&f->fpscr,&fr[6]);if(status)return status;}goto L8c63933e; /* fmul fr8,fr6 */
L8c63933e: *stop=0x8c63933eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[4],&f->fpscr,&fr[10]);if(status)return status;}goto L8c639340; /* fmul fr4,fr10 */
L8c639340: *stop=0x8c639340u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[11],&f->fpscr,&fr[4]);if(status)return status;}goto L8c639342; /* fmul fr11,fr4 */
L8c639342: *stop=0x8c639342u;fcopy(f,5u,10u);goto L8c639344; /* fmov fr10,fr5 */
L8c639344: *stop=0x8c639344u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639346; /* fschg */
L8c639346: *stop=0x8c639346u;fcopy(f,8u,13u);goto L8c639348; /* fmov fr13,fr8 */
L8c639348: *stop=0x8c639348u;fcopy(f,10u,15u);goto L8c63934a; /* fmov fr15,fr10 */
L8c63934a: *stop=0x8c63934au;fcopy(f,12u,15u);goto L8c63934c; /* fmov fr15,fr12 */
L8c63934c: *stop=0x8c63934cu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63934e; /* fschg */
L8c63934e: *stop=0x8c63934eu;fcopy(f,9u,8u);goto L8c639350; /* fmov fr8,fr9 */
L8c639350: *stop=0x8c639350u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[5],&f->fpscr,&fr[11]);if(status)return status;}goto L8c639352; /* fmul fr5,fr11 */
L8c639352: *stop=0x8c639352u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[2],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639354; /* fmul fr2,fr8 */
L8c639354: *stop=0x8c639354u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[7],&f->fpscr,&fr[10]);if(status)return status;}goto L8c639356; /* fmul fr7,fr10 */
L8c639356: *stop=0x8c639356u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[11],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c639358; /* fadd fr11,fr8 */
L8c639358: *stop=0x8c639358u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[3],&f->fpscr,&fr[9]);if(status)return status;}goto L8c63935a; /* fmul fr3,fr9 */
L8c63935a: *stop=0x8c63935au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[10],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c63935c; /* fadd fr10,fr8 */
L8c63935c: *stop=0x8c63935cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[6],&f->fpscr,&fr[13]);if(status)return status;}goto L8c63935e; /* fmul fr6,fr13 */
L8c63935e: *stop=0x8c63935eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[9],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c639360; /* fsub fr9,fr8 */
L8c639360: *stop=0x8c639360u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[4],&f->fpscr,&fr[12]);if(status)return status;}goto L8c639362; /* fmul fr4,fr12 */
L8c639362: *stop=0x8c639362u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[13],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c639364; /* fsub fr13,fr8 */
L8c639364: *stop=0x8c639364u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[12],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c639366; /* fsub fr12,fr8 */
L8c639366: *stop=0x8c639366u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639368; /* fschg */
L8c639368: *stop=0x8c639368u;fcopy(f,10u,9u);goto L8c63936a; /* fmov fr9,fr10 */
L8c63936a: *stop=0x8c63936au;fcopy(f,12u,11u);goto L8c63936c; /* fmov fr11,fr12 */
L8c63936c: *stop=0x8c63936cu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63936e; /* fschg */
L8c63936e: *stop=0x8c63936eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][8]^=0x80000000u;goto L8c639370; /* fneg fr8 */
L8c639370: *stop=0x8c639370u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[13],&f->fpscr,&fr[5]);if(status)return status;}goto L8c639372; /* fmul fr13,fr5 */
L8c639372: *stop=0x8c639372u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[10],&f->fpscr,&fr[2]);if(status)return status;}goto L8c639374; /* fmul fr10,fr2 */
L8c639374: *stop=0x8c639374u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[12],&f->fpscr,&fr[7]);if(status)return status;}goto L8c639376; /* fmul fr12,fr7 */
L8c639376: *stop=0x8c639376u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c639378; /* fadd fr5,fr2 */
L8c639378: *stop=0x8c639378u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[10],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63937a; /* fmul fr10,fr3 */
L8c63937a: *stop=0x8c63937au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[7],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c63937c; /* fadd fr7,fr2 */
L8c63937c: *stop=0x8c63937cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[13],&f->fpscr,&fr[6]);if(status)return status;}goto L8c63937e; /* fmul fr13,fr6 */
L8c63937e: *stop=0x8c63937eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c639380; /* fsub fr3,fr2 */
L8c639380: *stop=0x8c639380u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c639382; /* fmul fr12,fr4 */
L8c639382: *stop=0x8c639382u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[6],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c639384; /* fsub fr6,fr2 */
L8c639384: *stop=0x8c639384u;fcopy(f,3u,8u);goto L8c639386; /* fmov fr8,fr3 */
L8c639386: *stop=0x8c639386u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c639388; /* fsub fr4,fr2 */
L8c639388: *stop=0x8c639388u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c63938a; /* fsts FPUL,fr4 */
L8c63938a: *stop=0x8c63938au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63938c; /* fmul fr4,fr1 */
L8c63938c: *stop=0x8c63938cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c63938e; /* fmul fr4,fr0 */
L8c63938e: *stop=0x8c63938eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c639390; /* fmul fr4,fr3 */
L8c639390: *stop=0x8c639390u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c639392; /* fmul fr4,fr2 */
L8c639392: *stop=0x8c639392u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c639394; /* fmov.s fr1,@-r15 */
L8c639394: *stop=0x8c639394u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c639396; /* fmov.s fr0,@-r15 */
L8c639396: *stop=0x8c639396u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c639398; /* fmov.s fr3,@-r15 */
L8c639398: *stop=0x8c639398u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c63939a; /* fmov.s fr2,@-r15 */
L8c63939a: *stop=0x8c63939au;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63939c; /* fschg */
L8c63939c: *stop=0x8c63939cu;fcopy(f,0u,1u);goto L8c63939e; /* fmov fr1,fr0 */
L8c63939e: *stop=0x8c63939eu;fcopy(f,2u,3u);goto L8c6393a0; /* fmov fr3,fr2 */
L8c6393a0: *stop=0x8c6393a0u;fcopy(f,4u,5u);goto L8c6393a2; /* fmov fr5,fr4 */
L8c6393a2: *stop=0x8c6393a2u;fcopy(f,6u,7u);goto L8c6393a4; /* fmov fr7,fr6 */
L8c6393a4: *stop=0x8c6393a4u;fcopy(f,8u,1u);goto L8c6393a6; /* fmov fr1,fr8 */
L8c6393a6: *stop=0x8c6393a6u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6393a8; /* fschg */
L8c6393a8: *stop=0x8c6393a8u;fcopy(f,2u,3u);goto L8c6393aa; /* fmov fr3,fr2 */
L8c6393aa: *stop=0x8c6393aau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[7],&f->fpscr,&fr[8]);if(status)return status;}goto L8c6393ac; /* fmul fr7,fr8 */
L8c6393ac: *stop=0x8c6393acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6393ae; /* fmul fr5,fr0 */
L8c6393ae: *stop=0x8c6393aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6393b0; /* fmul fr7,fr1 */
L8c6393b0: *stop=0x8c6393b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6393b2; /* fmul fr4,fr2 */
L8c6393b2: *stop=0x8c6393b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6393b4; /* fmul fr5,fr3 */
L8c6393b4: *stop=0x8c6393b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[9],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6393b6; /* fmul fr9,fr4 */
L8c6393b6: *stop=0x8c6393b6u;fcopy(f,5u,8u);goto L8c6393b8; /* fmov fr8,fr5 */
L8c6393b8: *stop=0x8c6393b8u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6393ba; /* fschg */
L8c6393ba: *stop=0x8c6393bau;fcopy(f,6u,13u);goto L8c6393bc; /* fmov fr13,fr6 */
L8c6393bc: *stop=0x8c6393bcu;fcopy(f,8u,15u);goto L8c6393be; /* fmov fr15,fr8 */
L8c6393be: *stop=0x8c6393beu;fcopy(f,10u,13u);goto L8c6393c0; /* fmov fr13,fr10 */
L8c6393c0: *stop=0x8c6393c0u;fcopy(f,12u,15u);goto L8c6393c2; /* fmov fr15,fr12 */
L8c6393c2: *stop=0x8c6393c2u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6393c4; /* fschg */
L8c6393c4: *stop=0x8c6393c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[0],&f->fpscr,&fr[9]);if(status)return status;}goto L8c6393c6; /* fmul fr0,fr9 */
L8c6393c6: *stop=0x8c6393c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[1],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6393c8; /* fmul fr1,fr6 */
L8c6393c8: *stop=0x8c6393c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[2],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6393ca; /* fmul fr2,fr7 */
L8c6393ca: *stop=0x8c6393cau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[9],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c6393cc; /* fadd fr9,fr6 */
L8c6393cc: *stop=0x8c6393ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[3],&f->fpscr,&fr[10]);if(status)return status;}goto L8c6393ce; /* fmul fr3,fr10 */
L8c6393ce: *stop=0x8c6393ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c6393d0; /* fadd fr7,fr6 */
L8c6393d0: *stop=0x8c6393d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[4],&f->fpscr,&fr[13]);if(status)return status;}goto L8c6393d2; /* fmul fr4,fr13 */
L8c6393d2: *stop=0x8c6393d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[10],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6393d4; /* fsub fr10,fr6 */
L8c6393d4: *stop=0x8c6393d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[5],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6393d6; /* fmul fr5,fr11 */
L8c6393d6: *stop=0x8c6393d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[13],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6393d8; /* fsub fr13,fr6 */
L8c6393d8: *stop=0x8c6393d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[11],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6393da; /* fsub fr11,fr6 */
L8c6393da: *stop=0x8c6393dau;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6393dc; /* fschg */
L8c6393dc: *stop=0x8c6393dcu;fcopy(f,8u,9u);goto L8c6393de; /* fmov fr9,fr8 */
L8c6393de: *stop=0x8c6393deu;fcopy(f,10u,11u);goto L8c6393e0; /* fmov fr11,fr10 */
L8c6393e0: *stop=0x8c6393e0u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6393e2; /* fschg */
L8c6393e2: *stop=0x8c6393e2u;fcopy(f,10u,11u);goto L8c6393e4; /* fmov fr11,fr10 */
L8c6393e4: *stop=0x8c6393e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6393e6; /* fmul fr8,fr1 */
L8c6393e6: *stop=0x8c6393e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[0],&f->fpscr,&fr[10]);if(status)return status;}goto L8c6393e8; /* fmul fr0,fr10 */
L8c6393e8: *stop=0x8c6393e8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[9],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6393ea; /* fmul fr9,fr2 */
L8c6393ea: *stop=0x8c6393eau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[10],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c6393ec; /* fadd fr10,fr1 */
L8c6393ec: *stop=0x8c6393ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[3],&f->fpscr,&fr[8]);if(status)return status;}goto L8c6393ee; /* fmul fr3,fr8 */
L8c6393ee: *stop=0x8c6393eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c6393f0; /* fadd fr2,fr1 */
L8c6393f0: *stop=0x8c6393f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[4],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6393f2; /* fmul fr4,fr11 */
L8c6393f2: *stop=0x8c6393f2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[8],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6393f4; /* fsub fr8,fr1 */
L8c6393f4: *stop=0x8c6393f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[9],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6393f6; /* fmul fr9,fr5 */
L8c6393f6: *stop=0x8c6393f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6393f8; /* fsub fr11,fr1 */
L8c6393f8: *stop=0x8c6393f8u;fcopy(f,3u,4u);goto L8c6393fa; /* fmov fr4,fr3 */
L8c6393fa: *stop=0x8c6393fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[5],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6393fc; /* fsub fr5,fr1 */
L8c6393fc: *stop=0x8c6393fcu;fcopy(f,2u,6u);goto L8c6393fe; /* fmov fr6,fr2 */
L8c6393fe: *stop=0x8c6393feu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto L8c639400; /* fneg fr1 */
L8c639400: *stop=0x8c639400u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639402; /* fschg */
L8c639402: *stop=0x8c639402u;fcopy(f,4u,1u);goto L8c639404; /* fmov fr1,fr4 */
L8c639404: *stop=0x8c639404u;fcopy(f,6u,3u);goto L8c639406; /* fmov fr3,fr6 */
L8c639406: *stop=0x8c639406u;fcopy(f,8u,5u);goto L8c639408; /* fmov fr5,fr8 */
L8c639408: *stop=0x8c639408u;fcopy(f,10u,7u);goto L8c63940a; /* fmov fr7,fr10 */
L8c63940a: *stop=0x8c63940au;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63940c; /* fschg */
L8c63940c: *stop=0x8c63940cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[10],&f->fpscr,&fr[5]);if(status)return status;}goto L8c63940e; /* fmul fr10,fr5 */
L8c63940e: *stop=0x8c63940eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[6],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639410; /* fmul fr6,fr8 */
L8c639410: *stop=0x8c639410u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c639412; /* fmul fr10,fr4 */
L8c639412: *stop=0x8c639412u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[9],&f->fpscr,&fr[6]);if(status)return status;}goto L8c639414; /* fmul fr9,fr6 */
L8c639414: *stop=0x8c639414u;fcopy(f,7u,8u);goto L8c639416; /* fmov fr8,fr7 */
L8c639416: *stop=0x8c639416u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639418; /* fschg */
L8c639418: *stop=0x8c639418u;fcopy(f,8u,13u);goto L8c63941a; /* fmov fr13,fr8 */
L8c63941a: *stop=0x8c63941au;fcopy(f,10u,15u);goto L8c63941c; /* fmov fr15,fr10 */
L8c63941c: *stop=0x8c63941cu;fcopy(f,12u,13u);goto L8c63941e; /* fmov fr13,fr12 */
L8c63941e: *stop=0x8c63941eu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639420; /* fschg */
L8c639420: *stop=0x8c639420u;fcopy(f,11u,10u);goto L8c639422; /* fmov fr10,fr11 */
L8c639422: *stop=0x8c639422u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639424; /* fmul fr5,fr8 */
L8c639424: *stop=0x8c639424u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[0],&f->fpscr,&fr[10]);if(status)return status;}goto L8c639426; /* fmul fr0,fr10 */
L8c639426: *stop=0x8c639426u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[7],&f->fpscr,&fr[9]);if(status)return status;}goto L8c639428; /* fmul fr7,fr9 */
L8c639428: *stop=0x8c639428u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[10],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c63942a; /* fadd fr10,fr8 */
L8c63942a: *stop=0x8c63942au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[6],&f->fpscr,&fr[12]);if(status)return status;}goto L8c63942c; /* fmul fr6,fr12 */
L8c63942c: *stop=0x8c63942cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[9],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c63942e; /* fadd fr9,fr8 */
L8c63942e: *stop=0x8c63942eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[3],&f->fpscr,&fr[11]);if(status)return status;}goto L8c639430; /* fmul fr3,fr11 */
L8c639430: *stop=0x8c639430u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[12],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c639432; /* fsub fr12,fr8 */
L8c639432: *stop=0x8c639432u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[4],&f->fpscr,&fr[13]);if(status)return status;}goto L8c639434; /* fmul fr4,fr13 */
L8c639434: *stop=0x8c639434u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[11],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c639436; /* fsub fr11,fr8 */
L8c639436: *stop=0x8c639436u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[13],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c639438; /* fsub fr13,fr8 */
L8c639438: *stop=0x8c639438u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][8]^=0x80000000u;goto L8c63943a; /* fneg fr8 */
L8c63943a: *stop=0x8c63943au;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63943c; /* fschg */
L8c63943c: *stop=0x8c63943cu;fcopy(f,10u,9u);goto L8c63943e; /* fmov fr9,fr10 */
L8c63943e: *stop=0x8c63943eu;fcopy(f,12u,11u);goto L8c639440; /* fmov fr11,fr12 */
L8c639440: *stop=0x8c639440u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639442; /* fschg */
L8c639442: *stop=0x8c639442u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c639444; /* fmul fr12,fr0 */
L8c639444: *stop=0x8c639444u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[10],&f->fpscr,&fr[5]);if(status)return status;}goto L8c639446; /* fmul fr10,fr5 */
L8c639446: *stop=0x8c639446u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[11],&f->fpscr,&fr[7]);if(status)return status;}goto L8c639448; /* fmul fr11,fr7 */
L8c639448: *stop=0x8c639448u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[5],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c63944a; /* fadd fr5,fr0 */
L8c63944a: *stop=0x8c63944au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[10],&f->fpscr,&fr[6]);if(status)return status;}goto L8c63944c; /* fmul fr10,fr6 */
L8c63944c: *stop=0x8c63944cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[7],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c63944e; /* fadd fr7,fr0 */
L8c63944e: *stop=0x8c63944eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[12],&f->fpscr,&fr[3]);if(status)return status;}goto L8c639450; /* fmul fr12,fr3 */
L8c639450: *stop=0x8c639450u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[6],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c639452; /* fsub fr6,fr0 */
L8c639452: *stop=0x8c639452u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[11],&f->fpscr,&fr[4]);if(status)return status;}goto L8c639454; /* fmul fr11,fr4 */
L8c639454: *stop=0x8c639454u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c639456; /* fsub fr3,fr0 */
L8c639456: *stop=0x8c639456u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[4],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c639458; /* fsub fr4,fr0 */
L8c639458: *stop=0x8c639458u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c63945a; /* fsts FPUL,fr4 */
L8c63945a: *stop=0x8c63945au;fcopy(f,3u,1u);goto L8c63945c; /* fmov fr1,fr3 */
L8c63945c: *stop=0x8c63945cu;fcopy(f,9u,0u);goto L8c63945e; /* fmov fr0,fr9 */
L8c63945e: *stop=0x8c63945eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c639460; /* fmul fr4,fr2 */
L8c639460: *stop=0x8c639460u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c639462; /* fmul fr4,fr3 */
L8c639462: *stop=0x8c639462u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[4],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639464; /* fmul fr4,fr8 */
L8c639464: *stop=0x8c639464u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[4],&f->fpscr,&fr[9]);if(status)return status;}goto L8c639466; /* fmul fr4,fr9 */
L8c639466: *stop=0x8c639466u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639468; /* fschg */
L8c639468: *stop=0x8c639468u;fcopy(f,11u,2u);goto L8c63946a; /* fmov fr2,fr11 */
L8c63946a: *stop=0x8c63946au;fcopy(f,15u,8u);goto L8c63946c; /* fmov fr8,fr15 */
L8c63946c: *stop=0x8c63946cu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c63946e; /* fschg */
L8c63946e: *stop=0x8c63946eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639470; /* fmov.s @r15+,fr1 */
L8c639470: *stop=0x8c639470u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639472; /* fmov.s @r15+,fr0 */
L8c639472: *stop=0x8c639472u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639474; /* fmov.s @r15+,fr3 */
L8c639474: *stop=0x8c639474u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639476; /* fmov.s @r15+,fr2 */
L8c639476: *stop=0x8c639476u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639478; /* fmov.s @r15+,fr5 */
L8c639478: *stop=0x8c639478u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63947a; /* fmov.s @r15+,fr4 */
L8c63947a: *stop=0x8c63947au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63947c; /* fmov.s @r15+,fr7 */
L8c63947c: *stop=0x8c63947cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63947e; /* fmov.s @r15+,fr6 */
L8c63947e: *stop=0x8c63947eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639480; /* fmov.s @r15+,fr9 */
L8c639480: *stop=0x8c639480u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639482; /* fmov.s @r15+,fr8 */
L8c639482: *stop=0x8c639482u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639484; /* fmov.s @r15+,fr11 */
L8c639484: *stop=0x8c639484u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639486; /* fmov.s @r15+,fr10 */
L8c639486: *stop=0x8c639486u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639488; /* fschg */
L8c639488: *stop=0x8c639488u;fcopy(f,7u,0u);goto L8c63948a; /* fmov fr0,fr7 */
L8c63948a: *stop=0x8c63948au;fcopy(f,3u,2u);goto L8c63948c; /* fmov fr2,fr3 */
L8c63948c: *stop=0x8c63948cu;fcopy(f,13u,4u);goto L8c63948e; /* fmov fr4,fr13 */
L8c63948e: *stop=0x8c63948eu;fcopy(f,9u,6u);goto L8c639490; /* fmov fr6,fr9 */
L8c639490: *stop=0x8c639490u;fcopy(f,5u,8u);goto L8c639492; /* fmov fr8,fr5 */
L8c639492: *stop=0x8c639492u;fcopy(f,1u,10u);goto L8c639494; /* fmov fr10,fr1 */
L8c639494: *stop=0x8c639494u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c639496; /* fschg */
L8c639496: *stop=0x8c639498u;r[0]=1u;goto L8c6398a2; /* bra 0x8c6398a2 */
L8c63949a: *stop=0x8c63949au;r[3]=r[14];goto L8c63949c; /* mov r14,r3 */
L8c63949c: *stop=0x8c63949cu;r[2]=r[14];goto L8c63949e; /* mov r14,r2 */
L8c63949e: *stop=0x8c63949eu;r[3]+=32u;goto L8c6394a0; /* add #0x20,r3 */
L8c6394a0: *stop=0x8c6394a0u;r[2]+=16u;goto L8c6394a2; /* add #0x10,r2 */
L8c6394a2: *stop=0x8c6394a2u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394a4; /* fmov.s @r3+,fr0 */
L8c6394a4: *stop=0x8c6394a4u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394a6; /* fmov.s @r3+,fr1 */
L8c6394a6: *stop=0x8c6394a6u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394a8; /* fmov.s @r3+,fr2 */
L8c6394a8: *stop=0x8c6394a8u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394aa; /* fmov.s @r3+,fr3 */
L8c6394aa: *stop=0x8c6394aau;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394ac; /* fmov.s @r3+,fr4 */
L8c6394ac: *stop=0x8c6394acu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394ae; /* fmov.s @r3+,fr5 */
L8c6394ae: *stop=0x8c6394aeu;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394b0; /* fmov.s @r3+,fr6 */
L8c6394b0: *stop=0x8c6394b0u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394b2; /* fmov.s @r3+,fr7 */
L8c6394b2: *stop=0x8c6394b2u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394b4; /* fmov.s @r2+,fr8 */
L8c6394b4: *stop=0x8c6394b4u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394b6; /* fmov.s @r2+,fr9 */
L8c6394b6: *stop=0x8c6394b6u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394b8; /* fmov.s @r2+,fr10 */
L8c6394b8: *stop=0x8c6394b8u;if(!valid_access(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394ba; /* fmov.s @r2+,fr11 */
L8c6394ba: *stop=0x8c6394bau;fcopy(f,0u,1u);goto L8c6394bc; /* fmov fr1,fr0 */
L8c6394bc: *stop=0x8c6394bcu;fcopy(f,4u,2u);goto L8c6394be; /* fmov fr2,fr4 */
L8c6394be: *stop=0x8c6394beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6394c0; /* fmul fr5,fr2 */
L8c6394c0: *stop=0x8c6394c0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[7],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6394c2; /* fmul fr7,fr4 */
L8c6394c2: *stop=0x8c6394c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6394c4; /* fmul fr3,fr5 */
L8c6394c4: *stop=0x8c6394c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6394c6; /* fmul fr6,fr1 */
L8c6394c6: *stop=0x8c6394c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[7],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6394c8; /* fmul fr7,fr0 */
L8c6394c8: *stop=0x8c6394c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6394ca; /* fmul fr6,fr3 */
L8c6394ca: *stop=0x8c6394cau;fcopy(f,6u,9u);goto L8c6394cc; /* fmov fr9,fr6 */
L8c6394cc: *stop=0x8c6394ccu;fcopy(f,7u,10u);goto L8c6394ce; /* fmov fr10,fr7 */
L8c6394ce: *stop=0x8c6394ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[4],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6394d0; /* fmul fr4,fr6 */
L8c6394d0: *stop=0x8c6394d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[5],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6394d2; /* fmul fr5,fr7 */
L8c6394d2: *stop=0x8c6394d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[0],&f->fpscr,&fr[10]);if(status)return status;}goto L8c6394d4; /* fmul fr0,fr10 */
L8c6394d4: *stop=0x8c6394d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c6394d6; /* fadd fr7,fr6 */
L8c6394d6: *stop=0x8c6394d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[3],&f->fpscr,&fr[9]);if(status)return status;}goto L8c6394d8; /* fmul fr3,fr9 */
L8c6394d8: *stop=0x8c6394d8u;fcopy(f,7u,11u);goto L8c6394da; /* fmov fr11,fr7 */
L8c6394da: *stop=0x8c6394dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[1],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6394dc; /* fmul fr1,fr11 */
L8c6394dc: *stop=0x8c6394dcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[2],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6394de; /* fmul fr2,fr7 */
L8c6394de: *stop=0x8c6394deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[11],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c6394e0; /* fadd fr11,fr6 */
L8c6394e0: *stop=0x8c6394e0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6394e2; /* fsub fr7,fr6 */
L8c6394e2: *stop=0x8c6394e2u;r[5]=r[14];goto L8c6394e4; /* mov r14,r5 */
L8c6394e4: *stop=0x8c6394e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[10],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6394e6; /* fsub fr10,fr6 */
L8c6394e6: *stop=0x8c6394e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[9],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6394e8; /* fsub fr9,fr6 */
L8c6394e8: *stop=0x8c6394e8u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394ea; /* fmov.s @r5+,fr8 */
L8c6394ea: *stop=0x8c6394eau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394ec; /* fmov.s @r5+,fr9 */
L8c6394ec: *stop=0x8c6394ecu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394ee; /* fmov.s @r5+,fr10 */
L8c6394ee: *stop=0x8c6394eeu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6394f0; /* fmov.s @r5+,fr11 */
L8c6394f0: *stop=0x8c6394f0u;fcopy(f,8u,9u);goto L8c6394f2; /* fmov fr9,fr8 */
L8c6394f2: *stop=0x8c6394f2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[10],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6394f4; /* fmul fr10,fr5 */
L8c6394f4: *stop=0x8c6394f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[4],&f->fpscr,&fr[8]);if(status)return status;}goto L8c6394f6; /* fmul fr4,fr8 */
L8c6394f6: *stop=0x8c6394f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[11],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6394f8; /* fmul fr11,fr1 */
L8c6394f8: *stop=0x8c6394f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[8],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c6394fa; /* fadd fr8,fr5 */
L8c6394fa: *stop=0x8c6394fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[10],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6394fc; /* fmul fr10,fr0 */
L8c6394fc: *stop=0x8c6394fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[5],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c6394fe; /* fadd fr5,fr1 */
L8c6394fe: *stop=0x8c6394feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[11],&f->fpscr,&fr[2]);if(status)return status;}goto L8c639500; /* fmul fr11,fr2 */
L8c639500: *stop=0x8c639500u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[3],&f->fpscr,&fr[9]);if(status)return status;}goto L8c639502; /* fmul fr3,fr9 */
L8c639502: *stop=0x8c639502u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c639504; /* fsub fr2,fr1 */
L8c639504: *stop=0x8c639504u;r[7]=r[14];goto L8c639506; /* mov r14,r7 */
L8c639506: *stop=0x8c639506u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[0],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c639508; /* fsub fr0,fr1 */
L8c639508: *stop=0x8c639508u;r[7]+=16u;goto L8c63950a; /* add #0x10,r7 */
L8c63950a: *stop=0x8c63950au;r[6]=r[14];goto L8c63950c; /* mov r14,r6 */
L8c63950c: *stop=0x8c63950cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[9],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c63950e; /* fsub fr9,fr1 */
L8c63950e: *stop=0x8c63950eu;fcopy(f,0u,6u);goto L8c639510; /* fmov fr6,fr0 */
L8c639510: *stop=0x8c639510u;r[6]+=32u;goto L8c639512; /* add #0x20,r6 */
L8c639512: *stop=0x8c639512u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto L8c639514; /* fneg fr1 */
L8c639514: *stop=0x8c639514u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639516; /* fmov.s @r6+,fr6 */
L8c639516: *stop=0x8c639516u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639518; /* fmov.s @r6+,fr7 */
L8c639518: *stop=0x8c639518u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63951a; /* fmov.s @r6+,fr8 */
L8c63951a: *stop=0x8c63951au;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63951c; /* fmov.s @r6+,fr9 */
L8c63951c: *stop=0x8c63951cu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63951e; /* fmov.s @r6+,fr10 */
L8c63951e: *stop=0x8c63951eu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639520; /* fmov.s @r6+,fr11 */
L8c639520: *stop=0x8c639520u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639522; /* fmov.s @r6+,fr12 */
L8c639522: *stop=0x8c639522u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639524; /* fmov.s @r6+,fr13 */
L8c639524: *stop=0x8c639524u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[10],&f->fpscr,&fr[9]);if(status)return status;}goto L8c639526; /* fmul fr10,fr9 */
L8c639526: *stop=0x8c639526u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[6],&f->fpscr,&fr[12]);if(status)return status;}goto L8c639528; /* fmul fr6,fr12 */
L8c639528: *stop=0x8c639528u;fcopy(f,2u,9u);goto L8c63952a; /* fmov fr9,fr2 */
L8c63952a: *stop=0x8c63952au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[10],&f->fpscr,&fr[8]);if(status)return status;}goto L8c63952c; /* fmul fr10,fr8 */
L8c63952c: *stop=0x8c63952cu;fcopy(f,5u,12u);goto L8c63952e; /* fmov fr12,fr5 */
L8c63952e: *stop=0x8c63952eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[13],&f->fpscr,&fr[6]);if(status)return status;}goto L8c639530; /* fmul fr13,fr6 */
L8c639530: *stop=0x8c639530u;fcopy(f,7u,8u);goto L8c639532; /* fmov fr8,fr7 */
L8c639532: *stop=0x8c639532u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[7]);if(io_status)return io_status;r[7]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639534; /* fmov.s @r7+,fr8 */
L8c639534: *stop=0x8c639534u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[7]);if(io_status)return io_status;r[7]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639536; /* fmov.s @r7+,fr9 */
L8c639536: *stop=0x8c639536u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[7]);if(io_status)return io_status;goto L8c639538; /* fmov.s @r7,fr10 */
L8c639538: *stop=0x8c639538u;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[7]);if(io_status)return io_status;r[7]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63953a; /* fmov.s @r7+,fr12 */
L8c63953a: *stop=0x8c63953au;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[7]);if(io_status)return io_status;goto L8c63953c; /* fmov.s @r7,fr11 */
L8c63953c: *stop=0x8c63953cu;if(!valid_access(r[7],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[7]);if(io_status)return io_status;goto L8c63953e; /* fmov.s @r7,fr13 */
L8c63953e: *stop=0x8c63953eu;fcopy(f,9u,8u);goto L8c639540; /* fmov fr8,fr9 */
L8c639540: *stop=0x8c639540u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[2],&f->fpscr,&fr[12]);if(status)return status;}goto L8c639542; /* fmul fr2,fr12 */
L8c639542: *stop=0x8c639542u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[4],&f->fpscr,&fr[9]);if(status)return status;}goto L8c639544; /* fmul fr4,fr9 */
L8c639544: *stop=0x8c639544u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[5],&f->fpscr,&fr[13]);if(status)return status;}goto L8c639546; /* fmul fr5,fr13 */
L8c639546: *stop=0x8c639546u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[7],&f->fpscr,&fr[11]);if(status)return status;}goto L8c639548; /* fmul fr7,fr11 */
L8c639548: *stop=0x8c639548u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[6],&f->fpscr,&fr[10]);if(status)return status;}goto L8c63954a; /* fmul fr6,fr10 */
L8c63954a: *stop=0x8c63954au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[3],&f->fpscr,&fr[8]);if(status)return status;}goto L8c63954c; /* fmul fr3,fr8 */
L8c63954c: *stop=0x8c63954cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[12],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c63954e; /* fadd fr12,fr9 */
L8c63954e: *stop=0x8c63954eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[13],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c639550; /* fadd fr13,fr9 */
L8c639550: *stop=0x8c639550u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[11],&f->fpscr,&fr[9],1);if(status)return status;}goto L8c639552; /* fsub fr11,fr9 */
L8c639552: *stop=0x8c639552u;r[6]=r[14];goto L8c639554; /* mov r14,r6 */
L8c639554: *stop=0x8c639554u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[10],&f->fpscr,&fr[9],1);if(status)return status;}goto L8c639556; /* fsub fr10,fr9 */
L8c639556: *stop=0x8c639556u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[8],&f->fpscr,&fr[9],1);if(status)return status;}goto L8c639558; /* fsub fr8,fr9 */
L8c639558: *stop=0x8c639558u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63955a; /* fmov.s @r6+,fr10 */
L8c63955a: *stop=0x8c63955au;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63955c; /* fmov.s @r6+,fr11 */
L8c63955c: *stop=0x8c63955cu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63955e; /* fmov.s @r6+,fr12 */
L8c63955e: *stop=0x8c63955eu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639560; /* fmov.s @r6+,fr13 */
L8c639560: *stop=0x8c639560u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][9]^=0x80000000u;goto L8c639562; /* fneg fr9 */
L8c639562: *stop=0x8c639562u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c639564; /* fmul fr10,fr4 */
L8c639564: *stop=0x8c639564u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[12],&f->fpscr,&fr[2]);if(status)return status;}goto L8c639566; /* fmul fr12,fr2 */
L8c639566: *stop=0x8c639566u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[13],&f->fpscr,&fr[5]);if(status)return status;}goto L8c639568; /* fmul fr13,fr5 */
L8c639568: *stop=0x8c639568u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[12],&f->fpscr,&fr[6]);if(status)return status;}goto L8c63956a; /* fmul fr12,fr6 */
L8c63956a: *stop=0x8c63956au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[13],&f->fpscr,&fr[7]);if(status)return status;}goto L8c63956c; /* fmul fr13,fr7 */
L8c63956c: *stop=0x8c63956cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[10],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63956e; /* fmul fr10,fr3 */
L8c63956e: *stop=0x8c63956eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[2],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c639570; /* fadd fr2,fr4 */
L8c639570: *stop=0x8c639570u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[5],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c639572; /* fadd fr5,fr4 */
L8c639572: *stop=0x8c639572u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c639574; /* fneg fr3 */
L8c639574: *stop=0x8c639574u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[7],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c639576; /* fsub fr7,fr4 */
L8c639576: *stop=0x8c639576u;fcopy(f,2u,9u);goto L8c639578; /* fmov fr9,fr2 */
L8c639578: *stop=0x8c639578u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[6],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c63957a; /* fsub fr6,fr4 */
L8c63957a: *stop=0x8c63957au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c63957c; /* fadd fr4,fr3 */
L8c63957c: *stop=0x8c63957cu;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c63957e; /* fsts FPUL,fr4 */
L8c63957e: *stop=0x8c63957eu;r[5]=r[14];goto L8c639580; /* mov r14,r5 */
L8c639580: *stop=0x8c639580u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c639582; /* fmul fr4,fr0 */
L8c639582: *stop=0x8c639582u;r[6]=r[14];goto L8c639584; /* mov r14,r6 */
L8c639584: *stop=0x8c639584u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c639586; /* fmul fr4,fr1 */
L8c639586: *stop=0x8c639586u;r[5]+=32u;goto L8c639588; /* add #0x20,r5 */
L8c639588: *stop=0x8c639588u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63958a; /* fmul fr4,fr2 */
L8c63958a: *stop=0x8c63958au;r[6]+=16u;goto L8c63958c; /* add #0x10,r6 */
L8c63958c: *stop=0x8c63958cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63958e; /* fmul fr4,fr3 */
L8c63958e: *stop=0x8c63958eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c639590; /* fmov.s fr0,@-r15 */
L8c639590: *stop=0x8c639590u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c639592; /* fmov.s fr1,@-r15 */
L8c639592: *stop=0x8c639592u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c639594; /* fmov.s fr2,@-r15 */
L8c639594: *stop=0x8c639594u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c639596; /* fmov.s fr3,@-r15 */
L8c639596: *stop=0x8c639596u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639598; /* fmov.s @r5+,fr0 */
L8c639598: *stop=0x8c639598u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63959a; /* fmov.s @r5+,fr1 */
L8c63959a: *stop=0x8c63959au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63959c; /* fmov.s @r5+,fr2 */
L8c63959c: *stop=0x8c63959cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63959e; /* fmov.s @r5+,fr3 */
L8c63959e: *stop=0x8c63959eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6395a0; /* fmov.s @r5+,fr4 */
L8c6395a0: *stop=0x8c6395a0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6395a2; /* fmov.s @r5+,fr5 */
L8c6395a2: *stop=0x8c6395a2u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6395a4; /* fmov.s @r5+,fr6 */
L8c6395a4: *stop=0x8c6395a4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6395a6; /* fmov.s @r5+,fr7 */
L8c6395a6: *stop=0x8c6395a6u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6395a8; /* fmov.s @r6+,fr8 */
L8c6395a8: *stop=0x8c6395a8u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6395aa; /* fmov.s @r6+,fr9 */
L8c6395aa: *stop=0x8c6395aau;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6395ac; /* fmov.s @r6+,fr10 */
L8c6395ac: *stop=0x8c6395acu;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6395ae; /* fmov.s @r6+,fr11 */
L8c6395ae: *stop=0x8c6395aeu;fcopy(f,2u,1u);goto L8c6395b0; /* fmov fr1,fr2 */
L8c6395b0: *stop=0x8c6395b0u;fcopy(f,6u,5u);goto L8c6395b2; /* fmov fr5,fr6 */
L8c6395b2: *stop=0x8c6395b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6395b4; /* fmul fr7,fr1 */
L8c6395b4: *stop=0x8c6395b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[0],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6395b6; /* fmul fr0,fr7 */
L8c6395b6: *stop=0x8c6395b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6395b8; /* fmul fr5,fr0 */
L8c6395b8: *stop=0x8c6395b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6395ba; /* fmul fr3,fr5 */
L8c6395ba: *stop=0x8c6395bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6395bc; /* fmul fr4,fr3 */
L8c6395bc: *stop=0x8c6395bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6395be; /* fmul fr4,fr2 */
L8c6395be: *stop=0x8c6395beu;fcopy(f,4u,7u);goto L8c6395c0; /* fmov fr7,fr4 */
L8c6395c0: *stop=0x8c6395c0u;fcopy(f,12u,11u);goto L8c6395c2; /* fmov fr11,fr12 */
L8c6395c2: *stop=0x8c6395c2u;fcopy(f,6u,8u);goto L8c6395c4; /* fmov fr8,fr6 */
L8c6395c4: *stop=0x8c6395c4u;fcopy(f,7u,9u);goto L8c6395c6; /* fmov fr9,fr7 */
L8c6395c6: *stop=0x8c6395c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[1],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6395c8; /* fmul fr1,fr6 */
L8c6395c8: *stop=0x8c6395c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[3],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6395ca; /* fmul fr3,fr7 */
L8c6395ca: *stop=0x8c6395cau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[0],&f->fpscr,&fr[12]);if(status)return status;}goto L8c6395cc; /* fmul fr0,fr12 */
L8c6395cc: *stop=0x8c6395ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c6395ce; /* fadd fr7,fr6 */
L8c6395ce: *stop=0x8c6395ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[2],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6395d0; /* fmul fr2,fr11 */
L8c6395d0: *stop=0x8c6395d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[12],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c6395d2; /* fadd fr12,fr6 */
L8c6395d2: *stop=0x8c6395d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[4],&f->fpscr,&fr[9]);if(status)return status;}goto L8c6395d4; /* fmul fr4,fr9 */
L8c6395d4: *stop=0x8c6395d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[11],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6395d6; /* fsub fr11,fr6 */
L8c6395d6: *stop=0x8c6395d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c6395d8; /* fmul fr5,fr8 */
L8c6395d8: *stop=0x8c6395d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[9],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6395da; /* fsub fr9,fr6 */
L8c6395da: *stop=0x8c6395dau;r[5]=r[14];goto L8c6395dc; /* mov r14,r5 */
L8c6395dc: *stop=0x8c6395dcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[8],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6395de; /* fsub fr8,fr6 */
L8c6395de: *stop=0x8c6395deu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6395e0; /* fmov.s @r5+,fr8 */
L8c6395e0: *stop=0x8c6395e0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6395e2; /* fmov.s @r5+,fr9 */
L8c6395e2: *stop=0x8c6395e2u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6395e4; /* fmov.s @r5+,fr10 */
L8c6395e4: *stop=0x8c6395e4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6395e6; /* fmov.s @r5+,fr11 */
L8c6395e6: *stop=0x8c6395e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6395e8; /* fmul fr8,fr1 */
L8c6395e8: *stop=0x8c6395e8u;fcopy(f,10u,11u);goto L8c6395ea; /* fmov fr11,fr10 */
L8c6395ea: *stop=0x8c6395eau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6395ec; /* fmul fr9,fr3 */
L8c6395ec: *stop=0x8c6395ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[0],&f->fpscr,&fr[10]);if(status)return status;}goto L8c6395ee; /* fmul fr0,fr10 */
L8c6395ee: *stop=0x8c6395eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c6395f0; /* fadd fr3,fr1 */
L8c6395f0: *stop=0x8c6395f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[2],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6395f2; /* fmul fr2,fr11 */
L8c6395f2: *stop=0x8c6395f2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[10],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c6395f4; /* fadd fr10,fr1 */
L8c6395f4: *stop=0x8c6395f4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[9],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6395f6; /* fmul fr9,fr4 */
L8c6395f6: *stop=0x8c6395f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6395f8; /* fsub fr11,fr1 */
L8c6395f8: *stop=0x8c6395f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[8],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6395fa; /* fmul fr8,fr5 */
L8c6395fa: *stop=0x8c6395fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[4],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6395fc; /* fsub fr4,fr1 */
L8c6395fc: *stop=0x8c6395fcu;r[5]=r[14];goto L8c6395fe; /* mov r14,r5 */
L8c6395fe: *stop=0x8c6395feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[5],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c639600; /* fsub fr5,fr1 */
L8c639600: *stop=0x8c639600u;r[5]+=32u;goto L8c639602; /* add #0x20,r5 */
L8c639602: *stop=0x8c639602u;fcopy(f,3u,6u);goto L8c639604; /* fmov fr6,fr3 */
L8c639604: *stop=0x8c639604u;r[6]=r[14];goto L8c639606; /* mov r14,r6 */
L8c639606: *stop=0x8c639606u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto L8c639608; /* fneg fr1 */
L8c639608: *stop=0x8c639608u;r[6]+=16u;goto L8c63960a; /* add #0x10,r6 */
L8c63960a: *stop=0x8c63960au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63960c; /* fmov.s @r5+,fr4 */
L8c63960c: *stop=0x8c63960cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63960e; /* fmov.s @r5+,fr5 */
L8c63960e: *stop=0x8c63960eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639610; /* fmov.s @r5+,fr6 */
L8c639610: *stop=0x8c639610u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639612; /* fmov.s @r5+,fr7 */
L8c639612: *stop=0x8c639612u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639614; /* fmov.s @r5+,fr8 */
L8c639614: *stop=0x8c639614u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639616; /* fmov.s @r5+,fr9 */
L8c639616: *stop=0x8c639616u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639618; /* fmov.s @r5+,fr10 */
L8c639618: *stop=0x8c639618u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63961a; /* fmov.s @r5+,fr11 */
L8c63961a: *stop=0x8c63961au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[10],&f->fpscr,&fr[5]);if(status)return status;}goto L8c63961c; /* fmul fr10,fr5 */
L8c63961c: *stop=0x8c63961cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[6],&f->fpscr,&fr[8]);if(status)return status;}goto L8c63961e; /* fmul fr6,fr8 */
L8c63961e: *stop=0x8c63961eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c639620; /* fmul fr10,fr4 */
L8c639620: *stop=0x8c639620u;fcopy(f,7u,8u);goto L8c639622; /* fmov fr8,fr7 */
L8c639622: *stop=0x8c639622u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[9],&f->fpscr,&fr[6]);if(status)return status;}goto L8c639624; /* fmul fr9,fr6 */
L8c639624: *stop=0x8c639624u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639626; /* fmov.s @r6+,fr8 */
L8c639626: *stop=0x8c639626u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639628; /* fmov.s @r6+,fr9 */
L8c639628: *stop=0x8c639628u;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63962a; /* fmov.s @r6+,fr10 */
L8c63962a: *stop=0x8c63962au;if(!valid_access(r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[6]);if(io_status)return io_status;r[6]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63962c; /* fmov.s @r6+,fr11 */
L8c63962c: *stop=0x8c63962cu;fcopy(f,11u,8u);goto L8c63962e; /* fmov fr8,fr11 */
L8c63962e: *stop=0x8c63962eu;fcopy(f,12u,9u);goto L8c639630; /* fmov fr9,fr12 */
L8c639630: *stop=0x8c639630u;fcopy(f,13u,10u);goto L8c639632; /* fmov fr10,fr13 */
L8c639632: *stop=0x8c639632u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[5],&f->fpscr,&fr[11]);if(status)return status;}goto L8c639634; /* fmul fr5,fr11 */
L8c639634: *stop=0x8c639634u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[7],&f->fpscr,&fr[12]);if(status)return status;}goto L8c639636; /* fmul fr7,fr12 */
L8c639636: *stop=0x8c639636u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[0],&f->fpscr,&fr[13]);if(status)return status;}goto L8c639638; /* fmul fr0,fr13 */
L8c639638: *stop=0x8c639638u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[11],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c63963a; /* fadd fr11,fr12 */
L8c63963a: *stop=0x8c63963au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[2],&f->fpscr,&fr[10]);if(status)return status;}goto L8c63963c; /* fmul fr2,fr10 */
L8c63963c: *stop=0x8c63963cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[13],&f->fpscr,&fr[12],0);if(status)return status;}goto L8c63963e; /* fadd fr13,fr12 */
L8c63963e: *stop=0x8c63963eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[4],&f->fpscr,&fr[9]);if(status)return status;}goto L8c639640; /* fmul fr4,fr9 */
L8c639640: *stop=0x8c639640u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[10],&f->fpscr,&fr[12],1);if(status)return status;}goto L8c639642; /* fsub fr10,fr12 */
L8c639642: *stop=0x8c639642u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[6],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639644; /* fmul fr6,fr8 */
L8c639644: *stop=0x8c639644u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[9],&f->fpscr,&fr[12],1);if(status)return status;}goto L8c639646; /* fsub fr9,fr12 */
L8c639646: *stop=0x8c639646u;r[5]=r[14];goto L8c639648; /* mov r14,r5 */
L8c639648: *stop=0x8c639648u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[12],&fr[8],&f->fpscr,&fr[12],1);if(status)return status;}goto L8c63964a; /* fsub fr8,fr12 */
L8c63964a: *stop=0x8c63964au;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]^=0x80000000u;goto L8c63964c; /* fneg fr12 */
L8c63964c: *stop=0x8c63964cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63964e; /* fmov.s @r5+,fr8 */
L8c63964e: *stop=0x8c63964eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639650; /* fmov.s @r5+,fr9 */
L8c639650: *stop=0x8c639650u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639652; /* fmov.s @r5+,fr10 */
L8c639652: *stop=0x8c639652u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639654; /* fmov.s @r5+,fr11 */
L8c639654: *stop=0x8c639654u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[8],&f->fpscr,&fr[5]);if(status)return status;}goto L8c639656; /* fmul fr8,fr5 */
L8c639656: *stop=0x8c639656u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[9],&f->fpscr,&fr[7]);if(status)return status;}goto L8c639658; /* fmul fr9,fr7 */
L8c639658: *stop=0x8c639658u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[10],&f->fpscr,&fr[0]);if(status)return status;}goto L8c63965a; /* fmul fr10,fr0 */
L8c63965a: *stop=0x8c63965au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[7],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c63965c; /* fadd fr7,fr5 */
L8c63965c: *stop=0x8c63965cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[10],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63965e; /* fmul fr10,fr2 */
L8c63965e: *stop=0x8c63965eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[5],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c639660; /* fadd fr5,fr0 */
L8c639660: *stop=0x8c639660u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[9],&f->fpscr,&fr[4]);if(status)return status;}goto L8c639662; /* fmul fr9,fr4 */
L8c639662: *stop=0x8c639662u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[2],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c639664; /* fsub fr2,fr0 */
L8c639664: *stop=0x8c639664u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[8],&f->fpscr,&fr[6]);if(status)return status;}goto L8c639666; /* fmul fr8,fr6 */
L8c639666: *stop=0x8c639666u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[4],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c639668; /* fsub fr4,fr0 */
L8c639668: *stop=0x8c639668u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[6],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c63966a; /* fsub fr6,fr0 */
L8c63966a: *stop=0x8c63966au;f->bank[(f->fpscr>>21)&1u][8]=*fpul;goto L8c63966c; /* fsts FPUL,fr8 */
L8c63966c: *stop=0x8c63966cu;r[5]=r[14];goto L8c63966e; /* mov r14,r5 */
L8c63966e: *stop=0x8c63966eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[8],&f->fpscr,&fr[3]);if(status)return status;}goto L8c639670; /* fmul fr8,fr3 */
L8c639670: *stop=0x8c639670u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c639672; /* fmul fr8,fr1 */
L8c639672: *stop=0x8c639672u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[8],&f->fpscr,&fr[12]);if(status)return status;}goto L8c639674; /* fmul fr8,fr12 */
L8c639674: *stop=0x8c639674u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[8],&f->fpscr,&fr[0]);if(status)return status;}goto L8c639676; /* fmul fr8,fr0 */
L8c639676: *stop=0x8c639676u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c639678; /* fmov.s fr3,@-r15 */
L8c639678: *stop=0x8c639678u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c63967a; /* fmov.s fr1,@-r15 */
L8c63967a: *stop=0x8c63967au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c63967c; /* fmov.s fr12,@-r15 */
L8c63967c: *stop=0x8c63967cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c63967e; /* fmov.s fr0,@-r15 */
L8c63967e: *stop=0x8c63967eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639680; /* fmov.s @r5+,fr0 */
L8c639680: *stop=0x8c639680u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639682; /* fmov.s @r5+,fr1 */
L8c639682: *stop=0x8c639682u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639684; /* fmov.s @r5+,fr2 */
L8c639684: *stop=0x8c639684u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639686; /* fmov.s @r5+,fr3 */
L8c639686: *stop=0x8c639686u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639688; /* fmov.s @r5+,fr4 */
L8c639688: *stop=0x8c639688u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63968a; /* fmov.s @r5+,fr5 */
L8c63968a: *stop=0x8c63968au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63968c; /* fmov.s @r5+,fr6 */
L8c63968c: *stop=0x8c63968cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63968e; /* fmov.s @r5+,fr7 */
L8c63968e: *stop=0x8c63968eu;fcopy(f,0u,1u);goto L8c639690; /* fmov fr1,fr0 */
L8c639690: *stop=0x8c639690u;fcopy(f,4u,7u);goto L8c639692; /* fmov fr7,fr4 */
L8c639692: *stop=0x8c639692u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;}goto L8c639694; /* fmul fr6,fr0 */
L8c639694: *stop=0x8c639694u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c639696; /* fmul fr2,fr4 */
L8c639696: *stop=0x8c639696u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c639698; /* fmul fr7,fr1 */
L8c639698: *stop=0x8c639698u;r[5]+=16u;goto L8c63969a; /* add #0x10,r5 */
L8c63969a: *stop=0x8c63969au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63969c; /* fmul fr5,fr2 */
L8c63969c: *stop=0x8c63969cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c63969e; /* fmul fr3,fr5 */
L8c63969e: *stop=0x8c63969eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6396a0; /* fmul fr6,fr3 */
L8c6396a0: *stop=0x8c6396a0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396a2; /* fmov.s @r5+,fr6 */
L8c6396a2: *stop=0x8c6396a2u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396a4; /* fmov.s @r5+,fr7 */
L8c6396a4: *stop=0x8c6396a4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396a6; /* fmov.s @r5+,fr8 */
L8c6396a6: *stop=0x8c6396a6u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396a8; /* fmov.s @r5+,fr9 */
L8c6396a8: *stop=0x8c6396a8u;fcopy(f,6u,9u);goto L8c6396aa; /* fmov fr9,fr6 */
L8c6396aa: *stop=0x8c6396aau;fcopy(f,10u,7u);goto L8c6396ac; /* fmov fr7,fr10 */
L8c6396ac: *stop=0x8c6396acu;fcopy(f,11u,8u);goto L8c6396ae; /* fmov fr8,fr11 */
L8c6396ae: *stop=0x8c6396aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6396b0; /* fmul fr0,fr6 */
L8c6396b0: *stop=0x8c6396b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[4],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6396b2; /* fmul fr4,fr7 */
L8c6396b2: *stop=0x8c6396b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c6396b4; /* fmul fr5,fr8 */
L8c6396b4: *stop=0x8c6396b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c6396b6; /* fadd fr7,fr6 */
L8c6396b6: *stop=0x8c6396b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[3],&f->fpscr,&fr[10]);if(status)return status;}goto L8c6396b8; /* fmul fr3,fr10 */
L8c6396b8: *stop=0x8c6396b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[8],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c6396ba; /* fadd fr8,fr6 */
L8c6396ba: *stop=0x8c6396bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[2],&f->fpscr,&fr[9]);if(status)return status;}goto L8c6396bc; /* fmul fr2,fr9 */
L8c6396bc: *stop=0x8c6396bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[10],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6396be; /* fsub fr10,fr6 */
L8c6396be: *stop=0x8c6396beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[1],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6396c0; /* fmul fr1,fr11 */
L8c6396c0: *stop=0x8c6396c0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[9],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6396c2; /* fsub fr9,fr6 */
L8c6396c2: *stop=0x8c6396c2u;r[5]=r[14];goto L8c6396c4; /* mov r14,r5 */
L8c6396c4: *stop=0x8c6396c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[11],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6396c6; /* fsub fr11,fr6 */
L8c6396c6: *stop=0x8c6396c6u;r[5]+=32u;goto L8c6396c8; /* add #0x20,r5 */
L8c6396c8: *stop=0x8c6396c8u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396ca; /* fmov.s @r5+,fr8 */
L8c6396ca: *stop=0x8c6396cau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396cc; /* fmov.s @r5+,fr9 */
L8c6396cc: *stop=0x8c6396ccu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396ce; /* fmov.s @r5+,fr10 */
L8c6396ce: *stop=0x8c6396ceu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396d0; /* fmov.s @r5+,fr11 */
L8c6396d0: *stop=0x8c6396d0u;fcopy(f,8u,9u);goto L8c6396d2; /* fmov fr9,fr8 */
L8c6396d2: *stop=0x8c6396d2u;fcopy(f,12u,9u);goto L8c6396d4; /* fmov fr9,fr12 */
L8c6396d4: *stop=0x8c6396d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[11],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6396d6; /* fmul fr11,fr0 */
L8c6396d6: *stop=0x8c6396d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[4],&f->fpscr,&fr[8]);if(status)return status;}goto L8c6396d8; /* fmul fr4,fr8 */
L8c6396d8: *stop=0x8c6396d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[10],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6396da; /* fmul fr10,fr5 */
L8c6396da: *stop=0x8c6396dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[8],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c6396dc; /* fadd fr8,fr0 */
L8c6396dc: *stop=0x8c6396dcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[3],&f->fpscr,&fr[9]);if(status)return status;}goto L8c6396de; /* fmul fr3,fr9 */
L8c6396de: *stop=0x8c6396deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[5],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c6396e0; /* fadd fr5,fr0 */
L8c6396e0: *stop=0x8c6396e0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[11],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6396e2; /* fmul fr11,fr2 */
L8c6396e2: *stop=0x8c6396e2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[9],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6396e4; /* fsub fr9,fr0 */
L8c6396e4: *stop=0x8c6396e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[10],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6396e6; /* fmul fr10,fr1 */
L8c6396e6: *stop=0x8c6396e6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[2],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6396e8; /* fsub fr2,fr0 */
L8c6396e8: *stop=0x8c6396e8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[1],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6396ea; /* fsub fr1,fr0 */
L8c6396ea: *stop=0x8c6396eau;r[5]=r[14];goto L8c6396ec; /* mov r14,r5 */
L8c6396ec: *stop=0x8c6396ecu;fcopy(f,2u,4u);goto L8c6396ee; /* fmov fr4,fr2 */
L8c6396ee: *stop=0x8c6396eeu;fcopy(f,1u,6u);goto L8c6396f0; /* fmov fr6,fr1 */
L8c6396f0: *stop=0x8c6396f0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;goto L8c6396f2; /* fneg fr0 */
L8c6396f2: *stop=0x8c6396f2u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396f4; /* fmov.s @r5+,fr4 */
L8c6396f4: *stop=0x8c6396f4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396f6; /* fmov.s @r5+,fr5 */
L8c6396f6: *stop=0x8c6396f6u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396f8; /* fmov.s @r5+,fr6 */
L8c6396f8: *stop=0x8c6396f8u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396fa; /* fmov.s @r5+,fr7 */
L8c6396fa: *stop=0x8c6396fau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396fc; /* fmov.s @r5+,fr8 */
L8c6396fc: *stop=0x8c6396fcu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6396fe; /* fmov.s @r5+,fr9 */
L8c6396fe: *stop=0x8c6396feu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639700; /* fmov.s @r5+,fr10 */
L8c639700: *stop=0x8c639700u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639702; /* fmov.s @r5+,fr11 */
L8c639702: *stop=0x8c639702u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[8],&f->fpscr,&fr[7]);if(status)return status;}goto L8c639704; /* fmul fr8,fr7 */
L8c639704: *stop=0x8c639704u;r[5]+=16u;goto L8c639706; /* add #0x10,r5 */
L8c639706: *stop=0x8c639706u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[8],&f->fpscr,&fr[6]);if(status)return status;}goto L8c639708; /* fmul fr8,fr6 */
L8c639708: *stop=0x8c639708u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[4],&f->fpscr,&fr[10]);if(status)return status;}goto L8c63970a; /* fmul fr4,fr10 */
L8c63970a: *stop=0x8c63970au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[11],&f->fpscr,&fr[4]);if(status)return status;}goto L8c63970c; /* fmul fr11,fr4 */
L8c63970c: *stop=0x8c63970cu;fcopy(f,5u,10u);goto L8c63970e; /* fmov fr10,fr5 */
L8c63970e: *stop=0x8c63970eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639710; /* fmov.s @r5+,fr8 */
L8c639710: *stop=0x8c639710u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639712; /* fmov.s @r5+,fr9 */
L8c639712: *stop=0x8c639712u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;goto L8c639714; /* fmov.s @r5,fr10 */
L8c639714: *stop=0x8c639714u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639716; /* fmov.s @r5+,fr12 */
L8c639716: *stop=0x8c639716u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;goto L8c639718; /* fmov.s @r5,fr11 */
L8c639718: *stop=0x8c639718u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[5]);if(io_status)return io_status;goto L8c63971a; /* fmov.s @r5,fr13 */
L8c63971a: *stop=0x8c63971au;fcopy(f,9u,8u);goto L8c63971c; /* fmov fr8,fr9 */
L8c63971c: *stop=0x8c63971cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[5],&f->fpscr,&fr[11]);if(status)return status;}goto L8c63971e; /* fmul fr5,fr11 */
L8c63971e: *stop=0x8c63971eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[2],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639720; /* fmul fr2,fr8 */
L8c639720: *stop=0x8c639720u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[7],&f->fpscr,&fr[10]);if(status)return status;}goto L8c639722; /* fmul fr7,fr10 */
L8c639722: *stop=0x8c639722u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[11],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c639724; /* fadd fr11,fr8 */
L8c639724: *stop=0x8c639724u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[3],&f->fpscr,&fr[9]);if(status)return status;}goto L8c639726; /* fmul fr3,fr9 */
L8c639726: *stop=0x8c639726u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[10],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c639728; /* fadd fr10,fr8 */
L8c639728: *stop=0x8c639728u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[6],&f->fpscr,&fr[13]);if(status)return status;}goto L8c63972a; /* fmul fr6,fr13 */
L8c63972a: *stop=0x8c63972au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[9],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c63972c; /* fsub fr9,fr8 */
L8c63972c: *stop=0x8c63972cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[4],&f->fpscr,&fr[12]);if(status)return status;}goto L8c63972e; /* fmul fr4,fr12 */
L8c63972e: *stop=0x8c63972eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[13],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c639730; /* fsub fr13,fr8 */
L8c639730: *stop=0x8c639730u;r[5]=r[14];goto L8c639732; /* mov r14,r5 */
L8c639732: *stop=0x8c639732u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[12],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c639734; /* fsub fr12,fr8 */
L8c639734: *stop=0x8c639734u;r[5]+=32u;goto L8c639736; /* add #0x20,r5 */
L8c639736: *stop=0x8c639736u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639738; /* fmov.s @r5+,fr10 */
L8c639738: *stop=0x8c639738u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63973a; /* fmov.s @r5+,fr11 */
L8c63973a: *stop=0x8c63973au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63973c; /* fmov.s @r5+,fr12 */
L8c63973c: *stop=0x8c63973cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63973e; /* fmov.s @r5+,fr13 */
L8c63973e: *stop=0x8c63973eu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][8]^=0x80000000u;goto L8c639740; /* fneg fr8 */
L8c639740: *stop=0x8c639740u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[13],&f->fpscr,&fr[5]);if(status)return status;}goto L8c639742; /* fmul fr13,fr5 */
L8c639742: *stop=0x8c639742u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[10],&f->fpscr,&fr[2]);if(status)return status;}goto L8c639744; /* fmul fr10,fr2 */
L8c639744: *stop=0x8c639744u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[12],&f->fpscr,&fr[7]);if(status)return status;}goto L8c639746; /* fmul fr12,fr7 */
L8c639746: *stop=0x8c639746u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c639748; /* fadd fr5,fr2 */
L8c639748: *stop=0x8c639748u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[10],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63974a; /* fmul fr10,fr3 */
L8c63974a: *stop=0x8c63974au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[7],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c63974c; /* fadd fr7,fr2 */
L8c63974c: *stop=0x8c63974cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[13],&f->fpscr,&fr[6]);if(status)return status;}goto L8c63974e; /* fmul fr13,fr6 */
L8c63974e: *stop=0x8c63974eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c639750; /* fsub fr3,fr2 */
L8c639750: *stop=0x8c639750u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c639752; /* fmul fr12,fr4 */
L8c639752: *stop=0x8c639752u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[6],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c639754; /* fsub fr6,fr2 */
L8c639754: *stop=0x8c639754u;fcopy(f,3u,8u);goto L8c639756; /* fmov fr8,fr3 */
L8c639756: *stop=0x8c639756u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c639758; /* fsub fr4,fr2 */
L8c639758: *stop=0x8c639758u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c63975a; /* fsts FPUL,fr4 */
L8c63975a: *stop=0x8c63975au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c63975c; /* fmul fr4,fr1 */
L8c63975c: *stop=0x8c63975cu;r[5]=r[14];goto L8c63975e; /* mov r14,r5 */
L8c63975e: *stop=0x8c63975eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c639760; /* fmul fr4,fr0 */
L8c639760: *stop=0x8c639760u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c639762; /* fmul fr4,fr3 */
L8c639762: *stop=0x8c639762u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c639764; /* fmul fr4,fr2 */
L8c639764: *stop=0x8c639764u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c639766; /* fmov.s fr1,@-r15 */
L8c639766: *stop=0x8c639766u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c639768; /* fmov.s fr0,@-r15 */
L8c639768: *stop=0x8c639768u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c63976a; /* fmov.s fr3,@-r15 */
L8c63976a: *stop=0x8c63976au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c63976c; /* fmov.s fr2,@-r15 */
L8c63976c: *stop=0x8c63976cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;goto L8c63976e; /* fmov.s @r5,fr8 */
L8c63976e: *stop=0x8c63976eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639770; /* fmov.s @r5+,fr0 */
L8c639770: *stop=0x8c639770u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;goto L8c639772; /* fmov.s @r5,fr9 */
L8c639772: *stop=0x8c639772u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639774; /* fmov.s @r5+,fr1 */
L8c639774: *stop=0x8c639774u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639776; /* fmov.s @r5+,fr2 */
L8c639776: *stop=0x8c639776u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639778; /* fmov.s @r5+,fr3 */
L8c639778: *stop=0x8c639778u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63977a; /* fmov.s @r5+,fr4 */
L8c63977a: *stop=0x8c63977au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63977c; /* fmov.s @r5+,fr5 */
L8c63977c: *stop=0x8c63977cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63977e; /* fmov.s @r5+,fr6 */
L8c63977e: *stop=0x8c63977eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639780; /* fmov.s @r5+,fr7 */
L8c639780: *stop=0x8c639780u;fcopy(f,2u,3u);goto L8c639782; /* fmov fr3,fr2 */
L8c639782: *stop=0x8c639782u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[7],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639784; /* fmul fr7,fr8 */
L8c639784: *stop=0x8c639784u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c639786; /* fmul fr5,fr0 */
L8c639786: *stop=0x8c639786u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c639788; /* fmul fr7,fr1 */
L8c639788: *stop=0x8c639788u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c63978a; /* fmul fr4,fr2 */
L8c63978a: *stop=0x8c63978au;r[5]+=16u;goto L8c63978c; /* add #0x10,r5 */
L8c63978c: *stop=0x8c63978cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63978e; /* fmul fr5,fr3 */
L8c63978e: *stop=0x8c63978eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[9],&f->fpscr,&fr[4]);if(status)return status;}goto L8c639790; /* fmul fr9,fr4 */
L8c639790: *stop=0x8c639790u;fcopy(f,5u,8u);goto L8c639792; /* fmov fr8,fr5 */
L8c639792: *stop=0x8c639792u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;goto L8c639794; /* fmov.s @r5,fr6 */
L8c639794: *stop=0x8c639794u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639796; /* fmov.s @r5+,fr10 */
L8c639796: *stop=0x8c639796u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;goto L8c639798; /* fmov.s @r5,fr7 */
L8c639798: *stop=0x8c639798u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63979a; /* fmov.s @r5+,fr11 */
L8c63979a: *stop=0x8c63979au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;goto L8c63979c; /* fmov.s @r5,fr8 */
L8c63979c: *stop=0x8c63979cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63979e; /* fmov.s @r5+,fr12 */
L8c63979e: *stop=0x8c63979eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;goto L8c6397a0; /* fmov.s @r5,fr9 */
L8c6397a0: *stop=0x8c6397a0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6397a2; /* fmov.s @r5+,fr13 */
L8c6397a2: *stop=0x8c6397a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[0],&f->fpscr,&fr[9]);if(status)return status;}goto L8c6397a4; /* fmul fr0,fr9 */
L8c6397a4: *stop=0x8c6397a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[1],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6397a6; /* fmul fr1,fr6 */
L8c6397a6: *stop=0x8c6397a6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[2],&f->fpscr,&fr[7]);if(status)return status;}goto L8c6397a8; /* fmul fr2,fr7 */
L8c6397a8: *stop=0x8c6397a8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[9],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c6397aa; /* fadd fr9,fr6 */
L8c6397aa: *stop=0x8c6397aau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[3],&f->fpscr,&fr[10]);if(status)return status;}goto L8c6397ac; /* fmul fr3,fr10 */
L8c6397ac: *stop=0x8c6397acu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[7],&f->fpscr,&fr[6],0);if(status)return status;}goto L8c6397ae; /* fadd fr7,fr6 */
L8c6397ae: *stop=0x8c6397aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[4],&f->fpscr,&fr[13]);if(status)return status;}goto L8c6397b0; /* fmul fr4,fr13 */
L8c6397b0: *stop=0x8c6397b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[10],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6397b2; /* fsub fr10,fr6 */
L8c6397b2: *stop=0x8c6397b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[5],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6397b4; /* fmul fr5,fr11 */
L8c6397b4: *stop=0x8c6397b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[13],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6397b6; /* fsub fr13,fr6 */
L8c6397b6: *stop=0x8c6397b6u;r[5]+=4294967264u;goto L8c6397b8; /* add #-0x20,r5 */
L8c6397b8: *stop=0x8c6397b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[11],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c6397ba; /* fsub fr11,fr6 */
L8c6397ba: *stop=0x8c6397bau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6397bc; /* fmov.s @r5+,fr8 */
L8c6397bc: *stop=0x8c6397bcu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6397be; /* fmov.s @r5+,fr9 */
L8c6397be: *stop=0x8c6397beu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6397c0; /* fmov.s @r5+,fr10 */
L8c6397c0: *stop=0x8c6397c0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6397c2; /* fmov.s @r5+,fr11 */
L8c6397c2: *stop=0x8c6397c2u;fcopy(f,10u,11u);goto L8c6397c4; /* fmov fr11,fr10 */
L8c6397c4: *stop=0x8c6397c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6397c6; /* fmul fr8,fr1 */
L8c6397c6: *stop=0x8c6397c6u;r[5]=r[14];goto L8c6397c8; /* mov r14,r5 */
L8c6397c8: *stop=0x8c6397c8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[0],&f->fpscr,&fr[10]);if(status)return status;}goto L8c6397ca; /* fmul fr0,fr10 */
L8c6397ca: *stop=0x8c6397cau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[9],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6397cc; /* fmul fr9,fr2 */
L8c6397cc: *stop=0x8c6397ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[10],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c6397ce; /* fadd fr10,fr1 */
L8c6397ce: *stop=0x8c6397ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[3],&f->fpscr,&fr[8]);if(status)return status;}goto L8c6397d0; /* fmul fr3,fr8 */
L8c6397d0: *stop=0x8c6397d0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c6397d2; /* fadd fr2,fr1 */
L8c6397d2: *stop=0x8c6397d2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[4],&f->fpscr,&fr[11]);if(status)return status;}goto L8c6397d4; /* fmul fr4,fr11 */
L8c6397d4: *stop=0x8c6397d4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[8],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6397d6; /* fsub fr8,fr1 */
L8c6397d6: *stop=0x8c6397d6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[9],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6397d8; /* fmul fr9,fr5 */
L8c6397d8: *stop=0x8c6397d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6397da; /* fsub fr11,fr1 */
L8c6397da: *stop=0x8c6397dau;fcopy(f,3u,4u);goto L8c6397dc; /* fmov fr4,fr3 */
L8c6397dc: *stop=0x8c6397dcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[5],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6397de; /* fsub fr5,fr1 */
L8c6397de: *stop=0x8c6397deu;fcopy(f,2u,6u);goto L8c6397e0; /* fmov fr6,fr2 */
L8c6397e0: *stop=0x8c6397e0u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][1]^=0x80000000u;goto L8c6397e2; /* fneg fr1 */
L8c6397e2: *stop=0x8c6397e2u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6397e4; /* fmov.s @r5+,fr4 */
L8c6397e4: *stop=0x8c6397e4u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6397e6; /* fmov.s @r5+,fr5 */
L8c6397e6: *stop=0x8c6397e6u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6397e8; /* fmov.s @r5+,fr6 */
L8c6397e8: *stop=0x8c6397e8u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6397ea; /* fmov.s @r5+,fr7 */
L8c6397ea: *stop=0x8c6397eau;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6397ec; /* fmov.s @r5+,fr8 */
L8c6397ec: *stop=0x8c6397ecu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6397ee; /* fmov.s @r5+,fr9 */
L8c6397ee: *stop=0x8c6397eeu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6397f0; /* fmov.s @r5+,fr10 */
L8c6397f0: *stop=0x8c6397f0u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6397f2; /* fmov.s @r5+,fr11 */
L8c6397f2: *stop=0x8c6397f2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[10],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6397f4; /* fmul fr10,fr5 */
L8c6397f4: *stop=0x8c6397f4u;r[5]+=16u;goto L8c6397f6; /* add #0x10,r5 */
L8c6397f6: *stop=0x8c6397f6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[6],&f->fpscr,&fr[8]);if(status)return status;}goto L8c6397f8; /* fmul fr6,fr8 */
L8c6397f8: *stop=0x8c6397f8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[10],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6397fa; /* fmul fr10,fr4 */
L8c6397fa: *stop=0x8c6397fau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[9],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6397fc; /* fmul fr9,fr6 */
L8c6397fc: *stop=0x8c6397fcu;fcopy(f,7u,8u);goto L8c6397fe; /* fmov fr8,fr7 */
L8c6397fe: *stop=0x8c6397feu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;goto L8c639800; /* fmov.s @r5,fr8 */
L8c639800: *stop=0x8c639800u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639802; /* fmov.s @r5+,fr12 */
L8c639802: *stop=0x8c639802u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;goto L8c639804; /* fmov.s @r5,fr9 */
L8c639804: *stop=0x8c639804u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639806; /* fmov.s @r5+,fr13 */
L8c639806: *stop=0x8c639806u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639808; /* fmov.s @r5+,fr10 */
L8c639808: *stop=0x8c639808u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;goto L8c63980a; /* fmov.s @r5,fr11 */
L8c63980a: *stop=0x8c63980au;fcopy(f,11u,10u);goto L8c63980c; /* fmov fr10,fr11 */
L8c63980c: *stop=0x8c63980cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c63980e; /* fmul fr5,fr8 */
L8c63980e: *stop=0x8c63980eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[0],&f->fpscr,&fr[10]);if(status)return status;}goto L8c639810; /* fmul fr0,fr10 */
L8c639810: *stop=0x8c639810u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[7],&f->fpscr,&fr[9]);if(status)return status;}goto L8c639812; /* fmul fr7,fr9 */
L8c639812: *stop=0x8c639812u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[10],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c639814; /* fadd fr10,fr8 */
L8c639814: *stop=0x8c639814u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[6],&f->fpscr,&fr[12]);if(status)return status;}goto L8c639816; /* fmul fr6,fr12 */
L8c639816: *stop=0x8c639816u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[9],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c639818; /* fadd fr9,fr8 */
L8c639818: *stop=0x8c639818u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[3],&f->fpscr,&fr[11]);if(status)return status;}goto L8c63981a; /* fmul fr3,fr11 */
L8c63981a: *stop=0x8c63981au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[12],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c63981c; /* fsub fr12,fr8 */
L8c63981c: *stop=0x8c63981cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[4],&f->fpscr,&fr[13]);if(status)return status;}goto L8c63981e; /* fmul fr4,fr13 */
L8c63981e: *stop=0x8c63981eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[11],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c639820; /* fsub fr11,fr8 */
L8c639820: *stop=0x8c639820u;r[5]=r[14];goto L8c639822; /* mov r14,r5 */
L8c639822: *stop=0x8c639822u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[13],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c639824; /* fsub fr13,fr8 */
L8c639824: *stop=0x8c639824u;r[5]+=32u;goto L8c639826; /* add #0x20,r5 */
L8c639826: *stop=0x8c639826u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][8]^=0x80000000u;goto L8c639828; /* fneg fr8 */
L8c639828: *stop=0x8c639828u;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63982a; /* fmov.s @r5+,fr10 */
L8c63982a: *stop=0x8c63982au;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63982c; /* fmov.s @r5+,fr11 */
L8c63982c: *stop=0x8c63982cu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63982e; /* fmov.s @r5+,fr12 */
L8c63982e: *stop=0x8c63982eu;if(!valid_access(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639830; /* fmov.s @r5+,fr13 */
L8c639830: *stop=0x8c639830u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c639832; /* fmul fr12,fr0 */
L8c639832: *stop=0x8c639832u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[10],&f->fpscr,&fr[5]);if(status)return status;}goto L8c639834; /* fmul fr10,fr5 */
L8c639834: *stop=0x8c639834u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[11],&f->fpscr,&fr[7]);if(status)return status;}goto L8c639836; /* fmul fr11,fr7 */
L8c639836: *stop=0x8c639836u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[5],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c639838; /* fadd fr5,fr0 */
L8c639838: *stop=0x8c639838u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[10],&f->fpscr,&fr[6]);if(status)return status;}goto L8c63983a; /* fmul fr10,fr6 */
L8c63983a: *stop=0x8c63983au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[7],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c63983c; /* fadd fr7,fr0 */
L8c63983c: *stop=0x8c63983cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[12],&f->fpscr,&fr[3]);if(status)return status;}goto L8c63983e; /* fmul fr12,fr3 */
L8c63983e: *stop=0x8c63983eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[6],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c639840; /* fsub fr6,fr0 */
L8c639840: *stop=0x8c639840u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[11],&f->fpscr,&fr[4]);if(status)return status;}goto L8c639842; /* fmul fr11,fr4 */
L8c639842: *stop=0x8c639842u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c639844; /* fsub fr3,fr0 */
L8c639844: *stop=0x8c639844u;r[5]=r[14];goto L8c639846; /* mov r14,r5 */
L8c639846: *stop=0x8c639846u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[4],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c639848; /* fsub fr4,fr0 */
L8c639848: *stop=0x8c639848u;f->bank[(f->fpscr>>21)&1u][4]=*fpul;goto L8c63984a; /* fsts FPUL,fr4 */
L8c63984a: *stop=0x8c63984au;fcopy(f,3u,1u);goto L8c63984c; /* fmov fr1,fr3 */
L8c63984c: *stop=0x8c63984cu;fcopy(f,9u,0u);goto L8c63984e; /* fmov fr0,fr9 */
L8c63984e: *stop=0x8c63984eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c639850; /* fmul fr4,fr2 */
L8c639850: *stop=0x8c639850u;r[0]=44u;goto L8c639852; /* mov #0x2c,r0 */
L8c639852: *stop=0x8c639852u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c639854; /* fmul fr4,fr3 */
L8c639854: *stop=0x8c639854u;r[6]=r[14];goto L8c639856; /* mov r14,r6 */
L8c639856: *stop=0x8c639856u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[4],&f->fpscr,&fr[8]);if(status)return status;}goto L8c639858; /* fmul fr4,fr8 */
L8c639858: *stop=0x8c639858u;r[6]+=64u;goto L8c63985a; /* add #0x40,r6 */
L8c63985a: *stop=0x8c63985au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[4],&f->fpscr,&fr[9]);if(status)return status;}goto L8c63985c; /* fmul fr4,fr9 */
L8c63985c: *stop=0x8c63985cu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c63985e; /* fmov fr3,@(r0,r5) */
L8c63985e: *stop=0x8c63985eu;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[6]);if(io_status)return io_status;goto L8c639860; /* fmov.s fr9,@-r6 */
L8c639860: *stop=0x8c639860u;r[0]=40u;goto L8c639862; /* mov #0x28,r0 */
L8c639862: *stop=0x8c639862u;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[6]);if(io_status)return io_status;goto L8c639864; /* fmov.s fr8,@-r6 */
L8c639864: *stop=0x8c639864u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c639866; /* fmov fr2,@(r0,r5) */
L8c639866: *stop=0x8c639866u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639868; /* fmov.s @r15+,fr1 */
L8c639868: *stop=0x8c639868u;r[5]=r[14];goto L8c63986a; /* mov r14,r5 */
L8c63986a: *stop=0x8c63986au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63986c; /* fmov.s @r15+,fr0 */
L8c63986c: *stop=0x8c63986cu;r[6]=r[14];goto L8c63986e; /* mov r14,r6 */
L8c63986e: *stop=0x8c63986eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639870; /* fmov.s @r15+,fr3 */
L8c639870: *stop=0x8c639870u;r[6]+=56u;goto L8c639872; /* add #0x38,r6 */
L8c639872: *stop=0x8c639872u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639874; /* fmov.s @r15+,fr2 */
L8c639874: *stop=0x8c639874u;r[5]+=40u;goto L8c639876; /* add #0x28,r5 */
L8c639876: *stop=0x8c639876u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639878; /* fmov.s @r15+,fr5 */
L8c639878: *stop=0x8c639878u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63987a; /* fmov.s @r15+,fr4 */
L8c63987a: *stop=0x8c63987au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63987c; /* fmov.s @r15+,fr7 */
L8c63987c: *stop=0x8c63987cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c63987e; /* fmov.s @r15+,fr6 */
L8c63987e: *stop=0x8c63987eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639880; /* fmov.s @r15+,fr9 */
L8c639880: *stop=0x8c639880u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639882; /* fmov.s @r15+,fr8 */
L8c639882: *stop=0x8c639882u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639884; /* fmov.s @r15+,fr11 */
L8c639884: *stop=0x8c639884u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c639886; /* fmov.s @r15+,fr10 */
L8c639886: *stop=0x8c639886u;r[7]=r[14];goto L8c639888; /* mov r14,r7 */
L8c639888: *stop=0x8c639888u;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[5]);if(io_status)return io_status;goto L8c63988a; /* fmov.s fr7,@-r5 */
L8c63988a: *stop=0x8c63988au;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[5]);if(io_status)return io_status;goto L8c63988c; /* fmov.s fr6,@-r5 */
L8c63988c: *stop=0x8c63988cu;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;goto L8c63988e; /* fmov.s fr1,@-r5 */
L8c63988e: *stop=0x8c63988eu;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;goto L8c639890; /* fmov.s fr0,@-r5 */
L8c639890: *stop=0x8c639890u;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[5]);if(io_status)return io_status;goto L8c639892; /* fmov.s fr9,@-r5 */
L8c639892: *stop=0x8c639892u;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;goto L8c639894; /* fmov.s fr8,@-r5 */
L8c639894: *stop=0x8c639894u;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c639896; /* fmov.s fr3,@-r5 */
L8c639896: *stop=0x8c639896u;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c639898; /* fmov.s fr2,@-r5 */
L8c639898: *stop=0x8c639898u;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;goto L8c63989a; /* fmov.s fr11,@-r5 */
L8c63989a: *stop=0x8c63989au;if(!valid_access(r[5]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[5]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;goto L8c63989c; /* fmov.s fr10,@-r5 */
L8c63989c: *stop=0x8c63989cu;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[6]);if(io_status)return io_status;goto L8c63989e; /* fmov.s fr5,@-r6 */
L8c63989e: *stop=0x8c63989eu;if(!valid_access(r[6]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[6]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[6]);if(io_status)return io_status;goto L8c6398a0; /* fmov.s fr4,@-r6 */
L8c6398a0: *stop=0x8c6398a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6398b0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6398a2; /* mov.l 0x8c6398b0,r0 */
L8c6398a2: *stop=0x8c6398a2u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6398a4; /* fmov.s @r15+,fr14 */
L8c6398a4: *stop=0x8c6398a4u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6398a6; /* fmov.s @r15+,fr13 */
L8c6398a6: *stop=0x8c6398a6u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6398a8; /* fmov.s @r15+,fr12 */
L8c6398a8: *stop=0x8c6398a8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6398aa; /* lds.l @r15+,PR */
L8c6398aa: *stop=0x8c6398aau;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c6398acu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_model_matrix_inverse(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_inverse_run_8c6390b0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_collision_inverse_resume_8c6390ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_collision_inverse_run_8c6390b0(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c6390bau);}

SA_STAGE_EXPORT int sa_model_matrix_determinant(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64f3ec;
L8c64f3ec: *stop=0x8c64f3ecu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64f3ee; /* tst r4,r4 */
L8c64f3ee: if(!(*sr&1u))goto L8c64f494;goto L8c64f3f0; /* bf 0x8c64f494 */
L8c64f3f0: *stop=0x8c64f3f0u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f3f2; /* fschg */
L8c64f3f2: *stop=0x8c64f3f2u;fcopy(f,0u,9u);goto L8c64f3f4; /* fmov fr9,fr0 */
L8c64f3f4: *stop=0x8c64f3f4u;fcopy(f,2u,11u);goto L8c64f3f6; /* fmov fr11,fr2 */
L8c64f3f6: *stop=0x8c64f3f6u;fcopy(f,4u,13u);goto L8c64f3f8; /* fmov fr13,fr4 */
L8c64f3f8: *stop=0x8c64f3f8u;fcopy(f,6u,15u);goto L8c64f3fa; /* fmov fr15,fr6 */
L8c64f3fa: *stop=0x8c64f3fau;fcopy(f,8u,9u);goto L8c64f3fc; /* fmov fr9,fr8 */
L8c64f3fc: *stop=0x8c64f3fcu;fcopy(f,10u,11u);goto L8c64f3fe; /* fmov fr11,fr10 */
L8c64f3fe: *stop=0x8c64f3feu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f400; /* fschg */
L8c64f400: *stop=0x8c64f400u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[7],&f->fpscr,&fr[10]);if(status)return status;}goto L8c64f402; /* fmul fr7,fr10 */
L8c64f402: *stop=0x8c64f402u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f404; /* fmul fr6,fr3 */
L8c64f404: *stop=0x8c64f404u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[10],&fr[3],&f->fpscr,&fr[10],1);if(status)return status;}goto L8c64f406; /* fsub fr3,fr10 */
L8c64f406: *stop=0x8c64f406u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[6],&f->fpscr,&fr[9]);if(status)return status;}goto L8c64f408; /* fmul fr6,fr9 */
L8c64f408: *stop=0x8c64f408u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64f40a; /* fmul fr5,fr2 */
L8c64f40a: *stop=0x8c64f40au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[2],&f->fpscr,&fr[9],1);if(status)return status;}goto L8c64f40c; /* fsub fr2,fr9 */
L8c64f40c: *stop=0x8c64f40cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[4],&f->fpscr,&fr[11]);if(status)return status;}goto L8c64f40e; /* fmul fr4,fr11 */
L8c64f40e: *stop=0x8c64f40eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[7],&f->fpscr,&fr[0]);if(status)return status;}goto L8c64f410; /* fmul fr7,fr0 */
L8c64f410: *stop=0x8c64f410u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[0],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c64f412; /* fsub fr0,fr11 */
L8c64f412: *stop=0x8c64f412u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c64f414; /* fmul fr5,fr8 */
L8c64f414: *stop=0x8c64f414u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64f416; /* fmul fr4,fr1 */
L8c64f416: *stop=0x8c64f416u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[1],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c64f418; /* fsub fr1,fr8 */
L8c64f418: *stop=0x8c64f418u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f41a; /* fschg */
L8c64f41a: *stop=0x8c64f41au;fcopy(f,0u,9u);goto L8c64f41c; /* fmov fr9,fr0 */
L8c64f41c: *stop=0x8c64f41cu;fcopy(f,2u,11u);goto L8c64f41e; /* fmov fr11,fr2 */
L8c64f41e: *stop=0x8c64f41eu;fcopy(f,4u,13u);goto L8c64f420; /* fmov fr13,fr4 */
L8c64f420: *stop=0x8c64f420u;fcopy(f,6u,15u);goto L8c64f422; /* fmov fr15,fr6 */
L8c64f422: *stop=0x8c64f422u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f424; /* fschg */
L8c64f424: *stop=0x8c64f424u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c64f426; /* fmul fr3,fr5 */
L8c64f426: *stop=0x8c64f426u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c64f428; /* fmul fr0,fr6 */
L8c64f428: *stop=0x8c64f428u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[1],&f->fpscr,&fr[7]);if(status)return status;}goto L8c64f42a; /* fmul fr1,fr7 */
L8c64f42a: *stop=0x8c64f42au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c64f42c; /* fmul fr2,fr4 */
L8c64f42c: *stop=0x8c64f42cu;fcopy(f,0u,5u);goto L8c64f42e; /* fmov fr5,fr0 */
L8c64f42e: *stop=0x8c64f42eu;fcopy(f,1u,6u);goto L8c64f430; /* fmov fr6,fr1 */
L8c64f430: *stop=0x8c64f430u;fcopy(f,2u,7u);goto L8c64f432; /* fmov fr7,fr2 */
L8c64f432: *stop=0x8c64f432u;fcopy(f,3u,4u);goto L8c64f434; /* fmov fr4,fr3 */
L8c64f434: *stop=0x8c64f434u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[2],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c64f436; /* fsub fr2,fr5 */
L8c64f436: *stop=0x8c64f436u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[3],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c64f438; /* fsub fr3,fr6 */
L8c64f438: *stop=0x8c64f438u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[0],&f->fpscr,&fr[7],1);if(status)return status;}goto L8c64f43a; /* fsub fr0,fr7 */
L8c64f43a: *stop=0x8c64f43au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c64f43c; /* fsub fr1,fr4 */
L8c64f43c: *stop=0x8c64f43cu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f43e; /* fschg */
L8c64f43e: *stop=0x8c64f43eu;fcopy(f,0u,5u);goto L8c64f440; /* fmov fr5,fr0 */
L8c64f440: *stop=0x8c64f440u;fcopy(f,2u,5u);goto L8c64f442; /* fmov fr5,fr2 */
L8c64f442: *stop=0x8c64f442u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f444; /* fschg */
L8c64f444: *stop=0x8c64f444u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[2],&f->fpscr,&fr[7]);if(status)return status;}goto L8c64f446; /* fmul fr2,fr7 */
L8c64f446: *stop=0x8c64f446u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[10],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64f448; /* fmul fr10,fr2 */
L8c64f448: *stop=0x8c64f448u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[9],&f->fpscr,&fr[0]);if(status)return status;}goto L8c64f44a; /* fmul fr9,fr0 */
L8c64f44a: *stop=0x8c64f44au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[3],&f->fpscr,&fr[10]);if(status)return status;}goto L8c64f44c; /* fmul fr3,fr10 */
L8c64f44c: *stop=0x8c64f44cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[11],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64f44e; /* fmul fr11,fr1 */
L8c64f44e: *stop=0x8c64f44eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f450; /* fmul fr4,fr3 */
L8c64f450: *stop=0x8c64f450u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[1],&f->fpscr,&fr[7],0);if(status)return status;}goto L8c64f452; /* fadd fr1,fr7 */
L8c64f452: *stop=0x8c64f452u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[0],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c64f454; /* fadd fr0,fr3 */
L8c64f454: *stop=0x8c64f454u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f456; /* fschg */
L8c64f456: *stop=0x8c64f456u;fcopy(f,0u,7u);goto L8c64f458; /* fmov fr7,fr0 */
L8c64f458: *stop=0x8c64f458u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f45a; /* fschg */
L8c64f45a: *stop=0x8c64f45au;fcopy(f,1u,0u);goto L8c64f45c; /* fmov fr0,fr1 */
L8c64f45c: *stop=0x8c64f45cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[1],&f->fpscr,&fr[11]);if(status)return status;}goto L8c64f45e; /* fmul fr1,fr11 */
L8c64f45e: *stop=0x8c64f45eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c64f460; /* fmul fr0,fr5 */
L8c64f460: *stop=0x8c64f460u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64f462; /* fmul fr8,fr1 */
L8c64f462: *stop=0x8c64f462u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[10],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c64f464; /* fadd fr10,fr5 */
L8c64f464: *stop=0x8c64f464u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[11],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c64f466; /* fadd fr11,fr2 */
L8c64f466: *stop=0x8c64f466u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c64f468; /* fadd fr1,fr3 */
L8c64f468: *stop=0x8c64f468u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f46a; /* fschg */
L8c64f46a: *stop=0x8c64f46au;fcopy(f,0u,7u);goto L8c64f46c; /* fmov fr7,fr0 */
L8c64f46c: *stop=0x8c64f46cu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f46e; /* fschg */
L8c64f46e: *stop=0x8c64f46eu;fcopy(f,0u,1u);goto L8c64f470; /* fmov fr1,fr0 */
L8c64f470: *stop=0x8c64f470u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[0],&f->fpscr,&fr[9]);if(status)return status;}goto L8c64f472; /* fmul fr0,fr9 */
L8c64f472: *stop=0x8c64f472u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[1],&f->fpscr,&fr[6]);if(status)return status;}goto L8c64f474; /* fmul fr1,fr6 */
L8c64f474: *stop=0x8c64f474u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[8],&f->fpscr,&fr[0]);if(status)return status;}goto L8c64f476; /* fmul fr8,fr0 */
L8c64f476: *stop=0x8c64f476u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[9],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c64f478; /* fadd fr9,fr5 */
L8c64f478: *stop=0x8c64f478u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[6],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c64f47a; /* fadd fr6,fr2 */
L8c64f47a: *stop=0x8c64f47au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[7],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c64f47c; /* fadd fr7,fr0 */
L8c64f47c: *stop=0x8c64f47cu;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f47e; /* fschg */
L8c64f47e: *stop=0x8c64f47eu;fcopy(f,6u,1u);goto L8c64f480; /* fmov fr1,fr6 */
L8c64f480: *stop=0x8c64f480u;fcopy(f,8u,3u);goto L8c64f482; /* fmov fr3,fr8 */
L8c64f482: *stop=0x8c64f482u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c64f484; /* fschg */
L8c64f484: *stop=0x8c64f484u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[6],&f->fpscr,&fr[5]);if(status)return status;}goto L8c64f486; /* fmul fr6,fr5 */
L8c64f486: *stop=0x8c64f486u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64f488; /* fmul fr7,fr2 */
L8c64f488: *stop=0x8c64f488u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[8],&f->fpscr,&fr[0]);if(status)return status;}goto L8c64f48a; /* fmul fr8,fr0 */
L8c64f48a: *stop=0x8c64f48au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f48c; /* fmul fr9,fr3 */
L8c64f48c: *stop=0x8c64f48cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[2],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c64f48e; /* fsub fr2,fr5 */
L8c64f48e: *stop=0x8c64f48eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c64f490; /* fsub fr3,fr0 */
L8c64f490: *stop=0x8c64f490u;if(*pr!=entry_pr)return -3;*stop=0x8c64f492u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[5],&f->fpscr,&fr[0],0);if(status)return status;}return 0; /* rts */
L8c64f494: *stop=0x8c64f494u;r[1]=r[4];goto L8c64f496; /* mov r4,r1 */
L8c64f496: *stop=0x8c64f496u;r[1]+=32u;goto L8c64f498; /* add #0x20,r1 */
L8c64f498: *stop=0x8c64f498u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f49a; /* fmov.s @r1+,fr0 */
L8c64f49a: *stop=0x8c64f49au;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f49c; /* fmov.s @r1+,fr1 */
L8c64f49c: *stop=0x8c64f49cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f49e; /* fmov.s @r1+,fr2 */
L8c64f49e: *stop=0x8c64f49eu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4a0; /* fmov.s @r1+,fr3 */
L8c64f4a0: *stop=0x8c64f4a0u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4a2; /* fmov.s @r1+,fr4 */
L8c64f4a2: *stop=0x8c64f4a2u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4a4; /* fmov.s @r1+,fr5 */
L8c64f4a4: *stop=0x8c64f4a4u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4a6; /* fmov.s @r1+,fr6 */
L8c64f4a6: *stop=0x8c64f4a6u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4a8; /* fmov.s @r1+,fr7 */
L8c64f4a8: *stop=0x8c64f4a8u;fcopy(f,8u,0u);goto L8c64f4aa; /* fmov fr0,fr8 */
L8c64f4aa: *stop=0x8c64f4aau;fcopy(f,9u,1u);goto L8c64f4ac; /* fmov fr1,fr9 */
L8c64f4ac: *stop=0x8c64f4acu;fcopy(f,10u,2u);goto L8c64f4ae; /* fmov fr2,fr10 */
L8c64f4ae: *stop=0x8c64f4aeu;fcopy(f,11u,3u);goto L8c64f4b0; /* fmov fr3,fr11 */
L8c64f4b0: *stop=0x8c64f4b0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[7],&f->fpscr,&fr[10]);if(status)return status;}goto L8c64f4b2; /* fmul fr7,fr10 */
L8c64f4b2: *stop=0x8c64f4b2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f4b4; /* fmul fr6,fr3 */
L8c64f4b4: *stop=0x8c64f4b4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[10],&fr[3],&f->fpscr,&fr[10],1);if(status)return status;}goto L8c64f4b6; /* fsub fr3,fr10 */
L8c64f4b6: *stop=0x8c64f4b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[6],&f->fpscr,&fr[9]);if(status)return status;}goto L8c64f4b8; /* fmul fr6,fr9 */
L8c64f4b8: *stop=0x8c64f4b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64f4ba; /* fmul fr5,fr2 */
L8c64f4ba: *stop=0x8c64f4bau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[2],&f->fpscr,&fr[9],1);if(status)return status;}goto L8c64f4bc; /* fsub fr2,fr9 */
L8c64f4bc: *stop=0x8c64f4bcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[4],&f->fpscr,&fr[11]);if(status)return status;}goto L8c64f4be; /* fmul fr4,fr11 */
L8c64f4be: *stop=0x8c64f4beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[7],&f->fpscr,&fr[0]);if(status)return status;}goto L8c64f4c0; /* fmul fr7,fr0 */
L8c64f4c0: *stop=0x8c64f4c0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[11],&fr[0],&f->fpscr,&fr[11],1);if(status)return status;}goto L8c64f4c2; /* fsub fr0,fr11 */
L8c64f4c2: *stop=0x8c64f4c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[5],&f->fpscr,&fr[8]);if(status)return status;}goto L8c64f4c4; /* fmul fr5,fr8 */
L8c64f4c4: *stop=0x8c64f4c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64f4c6; /* fmul fr4,fr1 */
L8c64f4c6: *stop=0x8c64f4c6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[1],&f->fpscr,&fr[8],1);if(status)return status;}goto L8c64f4c8; /* fsub fr1,fr8 */
L8c64f4c8: *stop=0x8c64f4c8u;r[1]+=4294967264u;goto L8c64f4ca; /* add #-0x20,r1 */
L8c64f4ca: *stop=0x8c64f4cau;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4cc; /* fmov.s @r1+,fr0 */
L8c64f4cc: *stop=0x8c64f4ccu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4ce; /* fmov.s @r1+,fr1 */
L8c64f4ce: *stop=0x8c64f4ceu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4d0; /* fmov.s @r1+,fr2 */
L8c64f4d0: *stop=0x8c64f4d0u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4d2; /* fmov.s @r1+,fr3 */
L8c64f4d2: *stop=0x8c64f4d2u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4d4; /* fmov.s @r1+,fr4 */
L8c64f4d4: *stop=0x8c64f4d4u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4d6; /* fmov.s @r1+,fr5 */
L8c64f4d6: *stop=0x8c64f4d6u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4d8; /* fmov.s @r1+,fr6 */
L8c64f4d8: *stop=0x8c64f4d8u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4da; /* fmov.s @r1+,fr7 */
L8c64f4da: *stop=0x8c64f4dau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c64f4dc; /* fmul fr3,fr5 */
L8c64f4dc: *stop=0x8c64f4dcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[0],&f->fpscr,&fr[6]);if(status)return status;}goto L8c64f4de; /* fmul fr0,fr6 */
L8c64f4de: *stop=0x8c64f4deu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[1],&f->fpscr,&fr[7]);if(status)return status;}goto L8c64f4e0; /* fmul fr1,fr7 */
L8c64f4e0: *stop=0x8c64f4e0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c64f4e2; /* fmul fr2,fr4 */
L8c64f4e2: *stop=0x8c64f4e2u;fcopy(f,0u,5u);goto L8c64f4e4; /* fmov fr5,fr0 */
L8c64f4e4: *stop=0x8c64f4e4u;fcopy(f,1u,6u);goto L8c64f4e6; /* fmov fr6,fr1 */
L8c64f4e6: *stop=0x8c64f4e6u;fcopy(f,2u,7u);goto L8c64f4e8; /* fmov fr7,fr2 */
L8c64f4e8: *stop=0x8c64f4e8u;fcopy(f,3u,4u);goto L8c64f4ea; /* fmov fr4,fr3 */
L8c64f4ea: *stop=0x8c64f4eau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[2],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c64f4ec; /* fsub fr2,fr5 */
L8c64f4ec: *stop=0x8c64f4ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[3],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c64f4ee; /* fsub fr3,fr6 */
L8c64f4ee: *stop=0x8c64f4eeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[0],&f->fpscr,&fr[7],1);if(status)return status;}goto L8c64f4f0; /* fsub fr0,fr7 */
L8c64f4f0: *stop=0x8c64f4f0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c64f4f2; /* fsub fr1,fr4 */
L8c64f4f2: *stop=0x8c64f4f2u;r[1]+=4294967248u;goto L8c64f4f4; /* add #-0x30,r1 */
L8c64f4f4: *stop=0x8c64f4f4u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4f6; /* fmov.s @r1+,fr0 */
L8c64f4f6: *stop=0x8c64f4f6u;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f4f8; /* fmov.s @r1+,fr1 */
L8c64f4f8: *stop=0x8c64f4f8u;fcopy(f,2u,0u);goto L8c64f4fa; /* fmov fr0,fr2 */
L8c64f4fa: *stop=0x8c64f4fau;fcopy(f,3u,1u);goto L8c64f4fc; /* fmov fr1,fr3 */
L8c64f4fc: *stop=0x8c64f4fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[7],&fr[2],&f->fpscr,&fr[7]);if(status)return status;}goto L8c64f4fe; /* fmul fr2,fr7 */
L8c64f4fe: *stop=0x8c64f4feu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[10],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64f500; /* fmul fr10,fr2 */
L8c64f500: *stop=0x8c64f500u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[9],&f->fpscr,&fr[0]);if(status)return status;}goto L8c64f502; /* fmul fr9,fr0 */
L8c64f502: *stop=0x8c64f502u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[10],&fr[3],&f->fpscr,&fr[10]);if(status)return status;}goto L8c64f504; /* fmul fr3,fr10 */
L8c64f504: *stop=0x8c64f504u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[11],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64f506; /* fmul fr11,fr1 */
L8c64f506: *stop=0x8c64f506u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f508; /* fmul fr4,fr3 */
L8c64f508: *stop=0x8c64f508u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[7],&fr[1],&f->fpscr,&fr[7],0);if(status)return status;}goto L8c64f50a; /* fadd fr1,fr7 */
L8c64f50a: *stop=0x8c64f50au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[0],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c64f50c; /* fadd fr0,fr3 */
L8c64f50c: *stop=0x8c64f50cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f50e; /* fmov.s @r1+,fr0 */
L8c64f50e: *stop=0x8c64f50eu;fcopy(f,1u,0u);goto L8c64f510; /* fmov fr0,fr1 */
L8c64f510: *stop=0x8c64f510u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[1],&f->fpscr,&fr[11]);if(status)return status;}goto L8c64f512; /* fmul fr1,fr11 */
L8c64f512: *stop=0x8c64f512u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[0],&f->fpscr,&fr[5]);if(status)return status;}goto L8c64f514; /* fmul fr0,fr5 */
L8c64f514: *stop=0x8c64f514u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[8],&f->fpscr,&fr[1]);if(status)return status;}goto L8c64f516; /* fmul fr8,fr1 */
L8c64f516: *stop=0x8c64f516u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[10],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c64f518; /* fadd fr10,fr5 */
L8c64f518: *stop=0x8c64f518u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[11],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c64f51a; /* fadd fr11,fr2 */
L8c64f51a: *stop=0x8c64f51au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c64f51c; /* fadd fr1,fr3 */
L8c64f51c: *stop=0x8c64f51cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,0u,r[1]);if(io_status)return io_status;goto L8c64f51e; /* fmov.s @r1,fr0 */
L8c64f51e: *stop=0x8c64f51eu;fcopy(f,1u,0u);goto L8c64f520; /* fmov fr0,fr1 */
L8c64f520: *stop=0x8c64f520u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[0],&f->fpscr,&fr[9]);if(status)return status;}goto L8c64f522; /* fmul fr0,fr9 */
L8c64f522: *stop=0x8c64f522u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[6],&fr[1],&f->fpscr,&fr[6]);if(status)return status;}goto L8c64f524; /* fmul fr1,fr6 */
L8c64f524: *stop=0x8c64f524u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[8],&f->fpscr,&fr[0]);if(status)return status;}goto L8c64f526; /* fmul fr8,fr0 */
L8c64f526: *stop=0x8c64f526u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[9],&f->fpscr,&fr[5],0);if(status)return status;}goto L8c64f528; /* fadd fr9,fr5 */
L8c64f528: *stop=0x8c64f528u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[6],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c64f52a; /* fadd fr6,fr2 */
L8c64f52a: *stop=0x8c64f52au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[7],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c64f52c; /* fadd fr7,fr0 */
L8c64f52c: *stop=0x8c64f52cu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f52e; /* fmov.s @r4+,fr6 */
L8c64f52e: *stop=0x8c64f52eu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f530; /* fmov.s @r4+,fr7 */
L8c64f530: *stop=0x8c64f530u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f532; /* fmov.s @r4+,fr8 */
L8c64f532: *stop=0x8c64f532u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c64f534; /* fmov.s @r4+,fr9 */
L8c64f534: *stop=0x8c64f534u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[6],&f->fpscr,&fr[5]);if(status)return status;}goto L8c64f536; /* fmul fr6,fr5 */
L8c64f536: *stop=0x8c64f536u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c64f538; /* fmul fr7,fr2 */
L8c64f538: *stop=0x8c64f538u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[8],&f->fpscr,&fr[0]);if(status)return status;}goto L8c64f53a; /* fmul fr8,fr0 */
L8c64f53a: *stop=0x8c64f53au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c64f53c; /* fmul fr9,fr3 */
L8c64f53c: *stop=0x8c64f53cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[2],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c64f53e; /* fsub fr2,fr5 */
L8c64f53e: *stop=0x8c64f53eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c64f540; /* fsub fr3,fr0 */
L8c64f540: *stop=0x8c64f540u;if(*pr!=entry_pr)return -3;*stop=0x8c64f542u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[5],&f->fpscr,&fr[0],0);if(status)return status;}return 0; /* rts */
}
