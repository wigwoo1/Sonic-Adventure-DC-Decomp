/* Original movement-state parameter update and output scalar selection.
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
SA_STAGE_EXPORT int sa_movement_parameters_8c075214(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c075214;
L8c075214: *stop=0x8c075214u;r[0]=92u;goto L8c075216; /* mov #0x5c,r0 */
L8c075216: *stop=0x8c075216u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c075218; /* mov.l r14,@-r15 */
L8c075218: *stop=0x8c075218u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0753b0u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c07521a; /* mov.l 0x8c0753b0,r14 */
L8c07521a: *stop=0x8c07521au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c07521c; /* mov.l r13,@-r15 */
L8c07521c: *stop=0x8c07521cu;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c07521e; /* mov.l @(r0,r6),r3 */
L8c07521e: *stop=0x8c07521eu;r[3]|=r[7];goto L8c075220; /* or r7,r3 */
L8c075220: *stop=0x8c075220u;if(!valid_access(r[0]+r[6],4u))return -3;wr(m,bus,&io_status,r[6]+r[0],r[3],4);if(io_status)return io_status;goto L8c075222; /* mov.l r3,@(r0,r6) */
L8c075222: *stop=0x8c075222u;r[0]=0x8c0753a4u;goto L8c075224; /* mova 0x8c0753a4,r0 */
L8c075224: *stop=0x8c075224u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;goto L8c075226; /* fmov.s @r0,fr6 */
L8c075226: *stop=0x8c075226u;r[0]=0x8c0753a8u;goto L8c075228; /* mova 0x8c0753a8,r0 */
L8c075228: *stop=0x8c075228u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c07522a; /* fmov.s @r0,fr5 */
L8c07522a: *stop=0x8c07522au;r[0]=0x8c0753acu;goto L8c07522c; /* mova 0x8c0753ac,r0 */
L8c07522c: *stop=0x8c07522cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c07522e; /* fmov.s @r0,fr4 */
L8c07522e: *stop=0x8c07522eu;r[0]=121u;goto L8c075230; /* mov #0x79,r0 */
L8c075230: *stop=0x8c075230u;r[0]<<=2;goto L8c075232; /* shll2 r0 */
L8c075232: *stop=0x8c075232u;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c075234; /* mov.l @(r0,r6),r0 */
L8c075234: *stop=0x8c075234u;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c075236; /* tst #0x2,r0 */
L8c075236: take=(*sr&1u);*stop=0x8c075238u;r[13]=r[14];if(take)goto L8c0752b8;goto L8c07523a; /* bt/s 0x8c0752b8 */
L8c07523a: *stop=0x8c07523au;if(!valid_access(r[4]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;goto L8c07523c; /* mov.b @(0x9,r4),r0 */
L8c07523c: *stop=0x8c07523cu;r[0]=r[0]&255u;goto L8c07523e; /* extu.b r0,r0 */
L8c07523e: *stop=0x8c07523eu;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c075240; /* cmp/eq #0x5,r0 */
L8c075240: if(!(*sr&1u))goto L8c07525c;goto L8c075242; /* bf 0x8c07525c */
L8c075242: *stop=0x8c075242u;if(!valid_access(r[4]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;goto L8c075244; /* mov.b @(0x9,r4),r0 */
L8c075244: *stop=0x8c075244u;r[0]=r[0]&255u;goto L8c075246; /* extu.b r0,r0 */
L8c075246: *stop=0x8c075246u;r[3]=r[0];goto L8c075248; /* mov r0,r3 */
L8c075248: *stop=0x8c075248u;r[0]<<=2;goto L8c07524a; /* shll2 r0 */
L8c07524a: *stop=0x8c07524au;r[0]<<=2;goto L8c07524c; /* shll2 r0 */
L8c07524c: *stop=0x8c07524cu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c07524e; /* shll r0 */
L8c07524e: *stop=0x8c07524eu;r[0]+=r[3];goto L8c075250; /* add r3,r0 */
L8c075250: *stop=0x8c075250u;r[0]<<=2;goto L8c075252; /* shll2 r0 */
L8c075252: *stop=0x8c075252u;r[13]+=r[0];goto L8c075254; /* add r0,r13 */
L8c075254: *stop=0x8c075254u;r[0]=120u;goto L8c075256; /* mov #0x78,r0 */
L8c075256: *stop=0x8c075256u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c075258; /* fmov.s @(r0,r13),fr3 */
L8c075258: *stop=0x8c07525au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c075270; /* bra 0x8c075270 */
L8c07525c: *stop=0x8c07525cu;r[3]=r[0];goto L8c07525e; /* mov r0,r3 */
L8c07525e: *stop=0x8c07525eu;r[0]<<=2;goto L8c075260; /* shll2 r0 */
L8c075260: *stop=0x8c075260u;r[0]<<=2;goto L8c075262; /* shll2 r0 */
L8c075262: *stop=0x8c075262u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c075264; /* shll r0 */
L8c075264: *stop=0x8c075264u;r[0]+=r[3];goto L8c075266; /* add r3,r0 */
L8c075266: *stop=0x8c075266u;r[0]<<=2;goto L8c075268; /* shll2 r0 */
L8c075268: *stop=0x8c075268u;r[13]+=r[0];goto L8c07526a; /* add r0,r13 */
L8c07526a: *stop=0x8c07526au;r[0]=120u;goto L8c07526c; /* mov #0x78,r0 */
L8c07526c: *stop=0x8c07526cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c07526e; /* fmov.s @(r0,r13),fr3 */
L8c07526e: *stop=0x8c07526eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c075270; /* fmul fr6,fr3 */
L8c075270: *stop=0x8c075270u;r[0]=69u;goto L8c075272; /* mov #0x45,r0 */
L8c075272: *stop=0x8c075272u;r[0]<<=2;goto L8c075274; /* shll2 r0 */
L8c075274: *stop=0x8c075274u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c075276; /* fmov fr3,@(r0,r6) */
L8c075276: *stop=0x8c075276u;r[1]=88u;goto L8c075278; /* mov #0x58,r1 */
L8c075278: *stop=0x8c075278u;if(!valid_access(r[4]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;goto L8c07527a; /* mov.b @(0x9,r4),r0 */
L8c07527a: *stop=0x8c07527au;r[0]=r[0]&255u;goto L8c07527c; /* extu.b r0,r0 */
L8c07527c: *stop=0x8c07527cu;r[3]=r[0];goto L8c07527e; /* mov r0,r3 */
L8c07527e: *stop=0x8c07527eu;r[0]<<=2;goto L8c075280; /* shll2 r0 */
L8c075280: *stop=0x8c075280u;r[0]<<=2;goto L8c075282; /* shll2 r0 */
L8c075282: *stop=0x8c075282u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c075284; /* shll r0 */
L8c075284: *stop=0x8c075284u;r[0]+=r[3];goto L8c075286; /* add r3,r0 */
L8c075286: *stop=0x8c075286u;r[0]<<=2;goto L8c075288; /* shll2 r0 */
L8c075288: *stop=0x8c075288u;r[0]+=r[14];goto L8c07528a; /* add r14,r0 */
L8c07528a: *stop=0x8c07528au;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c07528c; /* fmov.s @(r0,r1),fr3 */
L8c07528c: *stop=0x8c07528cu;r[0]=4294967284u;goto L8c07528e; /* mov #-0xc,r0 */
L8c07528e: *stop=0x8c07528eu;r[0]=r[0]&255u;goto L8c075290; /* extu.b r0,r0 */
L8c075290: *stop=0x8c075290u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c075292; /* fmul fr5,fr3 */
L8c075292: *stop=0x8c075292u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c075294; /* fmov fr3,@(r0,r6) */
L8c075294: *stop=0x8c075294u;if(!valid_access(r[4]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;goto L8c075296; /* mov.b @(0x9,r4),r0 */
L8c075296: *stop=0x8c075296u;r[0]=r[0]&255u;goto L8c075298; /* extu.b r0,r0 */
L8c075298: *stop=0x8c075298u;r[3]=r[0];goto L8c07529a; /* mov r0,r3 */
L8c07529a: *stop=0x8c07529au;r[0]<<=2;goto L8c07529c; /* shll2 r0 */
L8c07529c: *stop=0x8c07529cu;r[0]<<=2;goto L8c07529e; /* shll2 r0 */
L8c07529e: *stop=0x8c07529eu;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0752a0; /* shll r0 */
L8c0752a0: *stop=0x8c0752a0u;r[0]+=r[3];goto L8c0752a2; /* add r3,r0 */
L8c0752a2: *stop=0x8c0752a2u;r[0]<<=2;goto L8c0752a4; /* shll2 r0 */
L8c0752a4: *stop=0x8c0752a4u;r[14]+=r[0];goto L8c0752a6; /* add r0,r14 */
L8c0752a6: *stop=0x8c0752a6u;r[0]=0x8c0753b4u;goto L8c0752a8; /* mova 0x8c0753b4,r0 */
L8c0752a8: *stop=0x8c0752a8u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0752aa; /* fmov.s @r0,fr3 */
L8c0752aa: *stop=0x8c0752aau;r[0]=60u;goto L8c0752ac; /* mov #0x3c,r0 */
L8c0752ac: *stop=0x8c0752acu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0752ae; /* fmov.s @(r0,r14),fr2 */
L8c0752ae: *stop=0x8c0752aeu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0752b0; /* fmul fr3,fr2 */
L8c0752b0: *stop=0x8c0752b0u;r[0]=4294967256u;goto L8c0752b2; /* mov #-0x28,r0 */
L8c0752b2: *stop=0x8c0752b2u;r[0]=r[0]&255u;goto L8c0752b4; /* extu.b r0,r0 */
L8c0752b4: *stop=0x8c0752b6u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c075382; /* bra 0x8c075382 */
L8c0752b8: *stop=0x8c0752b8u;r[0]=121u;goto L8c0752ba; /* mov #0x79,r0 */
L8c0752ba: *stop=0x8c0752bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c0753b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0752bc; /* mov.l 0x8c0753b8,r3 */
L8c0752bc: *stop=0x8c0752bcu;r[0]<<=2;goto L8c0752be; /* shll2 r0 */
L8c0752be: *stop=0x8c0752beu;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0752c0; /* mov.l @(r0,r6),r2 */
L8c0752c0: *stop=0x8c0752c0u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c0752c2; /* tst r3,r2 */
L8c0752c2: if((*sr&1u))goto L8c075328;goto L8c0752c4; /* bt 0x8c075328 */
L8c0752c4: *stop=0x8c0752c4u;if(!valid_access(r[4]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;goto L8c0752c6; /* mov.b @(0x9,r4),r0 */
L8c0752c6: *stop=0x8c0752c6u;r[1]=88u;goto L8c0752c8; /* mov #0x58,r1 */
L8c0752c8: *stop=0x8c0752c8u;r[0]=r[0]&255u;goto L8c0752ca; /* extu.b r0,r0 */
L8c0752ca: *stop=0x8c0752cau;r[2]=r[0];goto L8c0752cc; /* mov r0,r2 */
L8c0752cc: *stop=0x8c0752ccu;r[0]<<=2;goto L8c0752ce; /* shll2 r0 */
L8c0752ce: *stop=0x8c0752ceu;r[0]<<=2;goto L8c0752d0; /* shll2 r0 */
L8c0752d0: *stop=0x8c0752d0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0752d2; /* shll r0 */
L8c0752d2: *stop=0x8c0752d2u;r[0]+=r[2];goto L8c0752d4; /* add r2,r0 */
L8c0752d4: *stop=0x8c0752d4u;r[0]<<=2;goto L8c0752d6; /* shll2 r0 */
L8c0752d6: *stop=0x8c0752d6u;r[13]+=r[0];goto L8c0752d8; /* add r0,r13 */
L8c0752d8: *stop=0x8c0752d8u;r[0]=0x8c0753bcu;goto L8c0752da; /* mova 0x8c0753bc,r0 */
L8c0752da: *stop=0x8c0752dau;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0752dc; /* fmov.s @r0,fr3 */
L8c0752dc: *stop=0x8c0752dcu;r[0]=120u;goto L8c0752de; /* mov #0x78,r0 */
L8c0752de: *stop=0x8c0752deu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c0752e0; /* fmov.s @(r0,r13),fr2 */
L8c0752e0: *stop=0x8c0752e0u;r[0]=69u;goto L8c0752e2; /* mov #0x45,r0 */
L8c0752e2: *stop=0x8c0752e2u;r[0]<<=2;goto L8c0752e4; /* shll2 r0 */
L8c0752e4: *stop=0x8c0752e4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0752e6; /* fmul fr3,fr2 */
L8c0752e6: *stop=0x8c0752e6u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c0752e8; /* fmov fr2,@(r0,r6) */
L8c0752e8: *stop=0x8c0752e8u;if(!valid_access(r[4]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;goto L8c0752ea; /* mov.b @(0x9,r4),r0 */
L8c0752ea: *stop=0x8c0752eau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0x3f800000u;goto L8c0752ec; /* fldi1 fr2 */
L8c0752ec: *stop=0x8c0752ecu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[2],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0752ee; /* fadd fr2,fr2 */
L8c0752ee: *stop=0x8c0752eeu;r[0]=r[0]&255u;goto L8c0752f0; /* extu.b r0,r0 */
L8c0752f0: *stop=0x8c0752f0u;r[2]=r[0];goto L8c0752f2; /* mov r0,r2 */
L8c0752f2: *stop=0x8c0752f2u;r[0]<<=2;goto L8c0752f4; /* shll2 r0 */
L8c0752f4: *stop=0x8c0752f4u;r[0]<<=2;goto L8c0752f6; /* shll2 r0 */
L8c0752f6: *stop=0x8c0752f6u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0752f8; /* shll r0 */
L8c0752f8: *stop=0x8c0752f8u;r[0]+=r[2];goto L8c0752fa; /* add r2,r0 */
L8c0752fa: *stop=0x8c0752fau;r[0]<<=2;goto L8c0752fc; /* shll2 r0 */
L8c0752fc: *stop=0x8c0752fcu;r[0]+=r[14];goto L8c0752fe; /* add r14,r0 */
L8c0752fe: *stop=0x8c0752feu;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[1]);if(io_status)return io_status;goto L8c075300; /* fmov.s @(r0,r1),fr1 */
L8c075300: *stop=0x8c075300u;r[0]=4294967284u;goto L8c075302; /* mov #-0xc,r0 */
L8c075302: *stop=0x8c075302u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c075304; /* fmul fr2,fr1 */
L8c075304: *stop=0x8c075304u;r[0]=r[0]&255u;goto L8c075306; /* extu.b r0,r0 */
L8c075306: *stop=0x8c075306u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[6]);if(io_status)return io_status;goto L8c075308; /* fmov fr1,@(r0,r6) */
L8c075308: *stop=0x8c075308u;if(!valid_access(r[4]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;goto L8c07530a; /* mov.b @(0x9,r4),r0 */
L8c07530a: *stop=0x8c07530au;r[0]=r[0]&255u;goto L8c07530c; /* extu.b r0,r0 */
L8c07530c: *stop=0x8c07530cu;r[2]=r[0];goto L8c07530e; /* mov r0,r2 */
L8c07530e: *stop=0x8c07530eu;r[0]<<=2;goto L8c075310; /* shll2 r0 */
L8c075310: *stop=0x8c075310u;r[0]<<=2;goto L8c075312; /* shll2 r0 */
L8c075312: *stop=0x8c075312u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c075314; /* shll r0 */
L8c075314: *stop=0x8c075314u;r[0]+=r[2];goto L8c075316; /* add r2,r0 */
L8c075316: *stop=0x8c075316u;r[0]<<=2;goto L8c075318; /* shll2 r0 */
L8c075318: *stop=0x8c075318u;r[14]+=r[0];goto L8c07531a; /* add r0,r14 */
L8c07531a: *stop=0x8c07531au;r[0]=60u;goto L8c07531c; /* mov #0x3c,r0 */
L8c07531c: *stop=0x8c07531cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c07531e; /* fmov.s @(r0,r14),fr1 */
L8c07531e: *stop=0x8c07531eu;r[0]=4294967256u;goto L8c075320; /* mov #-0x28,r0 */
L8c075320: *stop=0x8c075320u;r[0]=r[0]&255u;goto L8c075322; /* extu.b r0,r0 */
L8c075322: *stop=0x8c075322u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[6],&f->fpscr,&fr[1]);if(status)return status;}goto L8c075324; /* fmul fr6,fr1 */
L8c075324: *stop=0x8c075326u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[6]);if(io_status)return io_status;goto L8c075382; /* bra 0x8c075382 */
L8c075328: *stop=0x8c075328u;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c07532a; /* mov.l @(r0,r6),r0 */
L8c07532a: *stop=0x8c07532au;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c07532c; /* tst #0x2,r0 */
L8c07532c: if(!(*sr&1u))goto L8c075382;goto L8c07532e; /* bf 0x8c075382 */
L8c07532e: *stop=0x8c07532eu;if(!valid_access(r[4]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;goto L8c075330; /* mov.b @(0x9,r4),r0 */
L8c075330: *stop=0x8c075330u;r[1]=88u;goto L8c075332; /* mov #0x58,r1 */
L8c075332: *stop=0x8c075332u;r[0]=r[0]&255u;goto L8c075334; /* extu.b r0,r0 */
L8c075334: *stop=0x8c075334u;r[2]=r[0];goto L8c075336; /* mov r0,r2 */
L8c075336: *stop=0x8c075336u;r[0]<<=2;goto L8c075338; /* shll2 r0 */
L8c075338: *stop=0x8c075338u;r[0]<<=2;goto L8c07533a; /* shll2 r0 */
L8c07533a: *stop=0x8c07533au;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c07533c; /* shll r0 */
L8c07533c: *stop=0x8c07533cu;r[0]+=r[2];goto L8c07533e; /* add r2,r0 */
L8c07533e: *stop=0x8c07533eu;r[0]<<=2;goto L8c075340; /* shll2 r0 */
L8c075340: *stop=0x8c075340u;r[13]+=r[0];goto L8c075342; /* add r0,r13 */
L8c075342: *stop=0x8c075342u;r[0]=120u;goto L8c075344; /* mov #0x78,r0 */
L8c075344: *stop=0x8c075344u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c075346; /* fmov.s @(r0,r13),fr3 */
L8c075346: *stop=0x8c075346u;r[0]=69u;goto L8c075348; /* mov #0x45,r0 */
L8c075348: *stop=0x8c075348u;r[0]<<=2;goto L8c07534a; /* shll2 r0 */
L8c07534a: *stop=0x8c07534au;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c07534c; /* fmov fr3,@(r0,r6) */
L8c07534c: *stop=0x8c07534cu;if(!valid_access(r[4]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;goto L8c07534e; /* mov.b @(0x9,r4),r0 */
L8c07534e: *stop=0x8c07534eu;r[0]=r[0]&255u;goto L8c075350; /* extu.b r0,r0 */
L8c075350: *stop=0x8c075350u;r[2]=r[0];goto L8c075352; /* mov r0,r2 */
L8c075352: *stop=0x8c075352u;r[0]<<=2;goto L8c075354; /* shll2 r0 */
L8c075354: *stop=0x8c075354u;r[0]<<=2;goto L8c075356; /* shll2 r0 */
L8c075356: *stop=0x8c075356u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c075358; /* shll r0 */
L8c075358: *stop=0x8c075358u;r[0]+=r[2];goto L8c07535a; /* add r2,r0 */
L8c07535a: *stop=0x8c07535au;r[0]<<=2;goto L8c07535c; /* shll2 r0 */
L8c07535c: *stop=0x8c07535cu;r[0]+=r[14];goto L8c07535e; /* add r14,r0 */
L8c07535e: *stop=0x8c07535eu;if(!valid_access(r[0]+r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c075360; /* fmov.s @(r0,r1),fr3 */
L8c075360: *stop=0x8c075360u;r[0]=4294967284u;goto L8c075362; /* mov #-0xc,r0 */
L8c075362: *stop=0x8c075362u;r[0]=r[0]&255u;goto L8c075364; /* extu.b r0,r0 */
L8c075364: *stop=0x8c075364u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c075366; /* fmov fr3,@(r0,r6) */
L8c075366: *stop=0x8c075366u;if(!valid_access(r[4]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x9u,1);if(io_status)return io_status;goto L8c075368; /* mov.b @(0x9,r4),r0 */
L8c075368: *stop=0x8c075368u;r[0]=r[0]&255u;goto L8c07536a; /* extu.b r0,r0 */
L8c07536a: *stop=0x8c07536au;r[2]=r[0];goto L8c07536c; /* mov r0,r2 */
L8c07536c: *stop=0x8c07536cu;r[0]<<=2;goto L8c07536e; /* shll2 r0 */
L8c07536e: *stop=0x8c07536eu;r[0]<<=2;goto L8c075370; /* shll2 r0 */
L8c075370: *stop=0x8c075370u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c075372; /* shll r0 */
L8c075372: *stop=0x8c075372u;r[0]+=r[2];goto L8c075374; /* add r2,r0 */
L8c075374: *stop=0x8c075374u;r[0]<<=2;goto L8c075376; /* shll2 r0 */
L8c075376: *stop=0x8c075376u;r[14]+=r[0];goto L8c075378; /* add r0,r14 */
L8c075378: *stop=0x8c075378u;r[0]=60u;goto L8c07537a; /* mov #0x3c,r0 */
L8c07537a: *stop=0x8c07537au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c07537c; /* fmov.s @(r0,r14),fr3 */
L8c07537c: *stop=0x8c07537cu;r[0]=4294967256u;goto L8c07537e; /* mov #-0x28,r0 */
L8c07537e: *stop=0x8c07537eu;r[0]=r[0]&255u;goto L8c075380; /* extu.b r0,r0 */
L8c075380: *stop=0x8c075380u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c075382; /* fmov fr3,@(r0,r6) */
L8c075382: *stop=0x8c075382u;r[3]=8u;goto L8c075384; /* mov #0x8,r3 */
L8c075384: *stop=0x8c075384u;*sr=(*sr&~1u)|!(r[7]&r[3]);goto L8c075386; /* tst r7,r3 */
L8c075386: if((*sr&1u))goto L8c0753e0;goto L8c075388; /* bt 0x8c0753e0 */
L8c075388: *stop=0x8c075388u;r[0]=0x8c0753c0u;goto L8c07538a; /* mova 0x8c0753c0,r0 */
L8c07538a: *stop=0x8c07538cu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0753ea; /* bra 0x8c0753ea */
L8c0753e0: *stop=0x8c0753e0u;r[2]=4u;goto L8c0753e2; /* mov #0x4,r2 */
L8c0753e2: *stop=0x8c0753e2u;*sr=(*sr&~1u)|!(r[7]&r[2]);goto L8c0753e4; /* tst r7,r2 */
L8c0753e4: if((*sr&1u))goto L8c0753f0;goto L8c0753e6; /* bt 0x8c0753f0 */
L8c0753e6: *stop=0x8c0753e6u;r[0]=0x8c0754e0u;goto L8c0753e8; /* mova 0x8c0754e0,r0 */
L8c0753e8: *stop=0x8c0753e8u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0753ea; /* fmov.s @r0,fr3 */
L8c0753ea: *stop=0x8c0753eau;r[0]=60u;goto L8c0753ec; /* mov #0x3c,r0 */
L8c0753ec: *stop=0x8c0753eeu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c075410; /* bra 0x8c075410 */
L8c0753f0: *stop=0x8c0753f0u;r[2]=16u;goto L8c0753f2; /* mov #0x10,r2 */
L8c0753f2: *stop=0x8c0753f2u;*sr=(*sr&~1u)|!(r[7]&r[2]);goto L8c0753f4; /* tst r7,r2 */
L8c0753f4: if((*sr&1u))goto L8c0753fc;goto L8c0753f6; /* bt 0x8c0753fc */
L8c0753f6: *stop=0x8c0753f6u;r[0]=60u;goto L8c0753f8; /* mov #0x3c,r0 */
L8c0753f8: *stop=0x8c0753fau;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c075410; /* bra 0x8c075410 */
L8c0753fc: *stop=0x8c0753fcu;r[2]=4294967168u;goto L8c0753fe; /* mov #-0x80,r2 */
L8c0753fe: *stop=0x8c0753feu;r[2]=r[2]&255u;goto L8c075400; /* extu.b r2,r2 */
L8c075400: *stop=0x8c075400u;*sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c075402; /* tst r2,r7 */
L8c075402: if((*sr&1u))goto L8c07540a;goto L8c075404; /* bt 0x8c07540a */
L8c075404: *stop=0x8c075404u;r[0]=60u;goto L8c075406; /* mov #0x3c,r0 */
L8c075406: *stop=0x8c075408u;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c075410; /* bra 0x8c075410 */
L8c07540a: *stop=0x8c07540au;r[0]=60u;goto L8c07540c; /* mov #0x3c,r0 */
L8c07540c: *stop=0x8c07540cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c07540e; /* fldi1 fr3 */
L8c07540e: *stop=0x8c07540eu;if(!valid_access(r[0]+r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c075410; /* fmov fr3,@(r0,r5) */
L8c075410: *stop=0x8c075410u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c075412; /* mov.l @r15+,r13 */
L8c075412: *stop=0x8c075412u;if(*pr!=entry_pr)return -3;*stop=0x8c075414u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
