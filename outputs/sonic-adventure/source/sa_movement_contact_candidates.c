/* Original contact candidate scans, motion clamp and selected-plane response.
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
static int sa_movement_contact_candidates_run_8c075686(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 if(resume_pc==0x8c075700u)goto L8c075700;if(resume_pc==0x8c075718u)goto L8c075718;if(resume_pc==0x8c07572cu)goto L8c07572c;if(resume_pc==0x8c075738u)goto L8c075738;if(resume_pc==0x8c075760u)goto L8c075760;if(resume_pc==0x8c07576eu)goto L8c07576e;if(resume_pc==0x8c075782u)goto L8c075782;if(resume_pc==0x8c0757c0u)goto L8c0757c0;if(resume_pc==0x8c0757d6u)goto L8c0757d6;if(resume_pc==0x8c0757f2u)goto L8c0757f2;goto L8c075686;
L8c075686: *stop=0x8c075686u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c075688; /* mov.l r14,@-r15 */
L8c075688: *stop=0x8c075688u;r[0]=4294967295u;goto L8c07568a; /* mov #-0x1,r0 */
L8c07568a: *stop=0x8c07568au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c07568c; /* mov.l r13,@-r15 */
L8c07568c: *stop=0x8c07568cu;r[0]<<=8;goto L8c07568e; /* shll8 r0 */
L8c07568e: *stop=0x8c07568eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c075690; /* mov.l r12,@-r15 */
L8c075690: *stop=0x8c075690u;r[0]+=108u;goto L8c075692; /* add #0x6c,r0 */
L8c075692: *stop=0x8c075692u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c075694; /* mov.l r11,@-r15 */
L8c075694: *stop=0x8c075694u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c075696; /* mov.l r10,@-r15 */
L8c075696: *stop=0x8c075696u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c075698; /* mov.l r9,@-r15 */
L8c075698: *stop=0x8c075698u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c07569a; /* mov.l r8,@-r15 */
L8c07569a: *stop=0x8c07569au;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c07569c; /* fmov.s fr15,@-r15 */
L8c07569c: *stop=0x8c07569cu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c07569e; /* fmov.s fr12,@-r15 */
L8c07569e: *stop=0x8c07569eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c0756a0; /* sts.l PR,@-r15 */
L8c0756a0: *stop=0x8c0756a0u;r[15]+=r[0];goto L8c0756a2; /* add r0,r15 */
L8c0756a2: *stop=0x8c0756a2u;r[0]=0x8c0756d4u;goto L8c0756a4; /* mova 0x8c0756d4,r0 */
L8c0756a4: *stop=0x8c0756a4u;r[9]=r[15];goto L8c0756a6; /* mov r15,r9 */
L8c0756a6: *stop=0x8c0756a6u;r[2]=4294967295u;goto L8c0756a8; /* mov #-0x1,r2 */
L8c0756a8: *stop=0x8c0756a8u;r[12]=r[15];goto L8c0756aa; /* mov r15,r12 */
L8c0756aa: *stop=0x8c0756aau;r[3]=0u;goto L8c0756ac; /* mov #0x0,r3 */
L8c0756ac: *stop=0x8c0756acu;r[14]=r[7];goto L8c0756ae; /* mov r7,r14 */
L8c0756ae: *stop=0x8c0756aeu;r[9]+=20u;goto L8c0756b0; /* add #0x14,r9 */
L8c0756b0: *stop=0x8c0756b0u;r[11]=r[3];goto L8c0756b2; /* mov r3,r11 */
L8c0756b2: *stop=0x8c0756b2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c0756b4; /* mov.l r3,@r15 */
L8c0756b4: *stop=0x8c0756b4u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c0756b6; /* mov.l r2,@(0x4,r15) */
L8c0756b6: *stop=0x8c0756b6u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c0756b8; /* fmov.s @r0,fr15 */
L8c0756b8: *stop=0x8c0756bau;r[12]+=84u;goto L8c0757a8; /* bra 0x8c0757a8 */
L8c0756e0: *stop=0x8c0756e0u;r[10]=88u;goto L8c0756e2; /* mov #0x58,r10 */
L8c0756e2: *stop=0x8c0756e2u;r[13]=r[11];goto L8c0756e4; /* mov r11,r13 */
L8c0756e4: *stop=0x8c0756e4u;*macl=r[10]*r[11];goto L8c0756e6; /* mul.l r10,r11 */
L8c0756e6: *stop=0x8c0756e6u;r[13]<<=2;goto L8c0756e8; /* shll2 r13 */
L8c0756e8: *stop=0x8c0756e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075840u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0756ea; /* mov.l 0x8c075840,r3 */
L8c0756ea: *stop=0x8c0756eau;r[2]=r[13];goto L8c0756ec; /* mov r13,r2 */
L8c0756ec: *stop=0x8c0756ecu;r[2]+=r[12];goto L8c0756ee; /* add r12,r2 */
L8c0756ee: *stop=0x8c0756eeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c0756f0; /* mov.l r2,@-r15 */
L8c0756f0: *stop=0x8c0756f0u;r[5]=r[14];goto L8c0756f2; /* mov r14,r5 */
L8c0756f2: *stop=0x8c0756f2u;r[10]=*macl;goto L8c0756f4; /* sts MACL,r10 */
L8c0756f4: *stop=0x8c0756f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075844u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0756f6; /* mov.l 0x8c075844,r2 */
L8c0756f6: *stop=0x8c0756f6u;r[5]+=4u;goto L8c0756f8; /* add #0x4,r5 */
L8c0756f8: *stop=0x8c0756f8u;r[10]+=r[3];goto L8c0756fa; /* add r3,r10 */
L8c0756fa: *stop=0x8c0756fau;r[4]=r[10];goto L8c0756fc; /* mov r10,r4 */
L8c0756fc: target=r[2];*pr=0x8c075700u;*stop=0x8c0756feu;r[4]+=44u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075700; /* jsr @r2 */
L8c075700: *stop=0x8c075700u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c075702; /* mov.l @r15+,r3 */
L8c075702: *stop=0x8c075702u;r[5]=r[10];goto L8c075704; /* mov r10,r5 */
L8c075704: *stop=0x8c075704u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075848u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075706; /* mov.l 0x8c075848,r2 */
L8c075706: *stop=0x8c075706u;r[0]=r[13];goto L8c075708; /* mov r13,r0 */
L8c075708: *stop=0x8c075708u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[3]);if(io_status)return io_status;goto L8c07570a; /* fmov.s fr0,@r3 */
L8c07570a: *stop=0x8c07570au;r[3]=0u;goto L8c07570c; /* mov #0x0,r3 */
L8c07570c: *stop=0x8c07570cu;r[4]=r[14];goto L8c07570e; /* mov r14,r4 */
L8c07570e: *stop=0x8c07570eu;r[5]+=20u;goto L8c075710; /* add #0x14,r5 */
L8c075710: *stop=0x8c075710u;if(!valid_access(r[0]+r[9],4u))return -3;wr(m,bus,&io_status,r[9]+r[0],r[3],4);if(io_status)return io_status;goto L8c075712; /* mov.l r3,@(r0,r9) */
L8c075712: *stop=0x8c075712u;r[6]=r[3];goto L8c075714; /* mov r3,r6 */
L8c075714: target=r[2];*pr=0x8c075718u;*stop=0x8c075716u;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075718; /* jsr @r2 */
L8c075718: *stop=0x8c075718u;r[3]=44u;goto L8c07571a; /* mov #0x2c,r3 */
L8c07571a: *stop=0x8c07571au;r[8]=r[0];goto L8c07571c; /* mov r0,r8 */
L8c07571c: *stop=0x8c07571cu;r[3]<<=8;goto L8c07571e; /* shll8 r3 */
L8c07571e: *stop=0x8c07571eu;*sr=(*sr&~1u)|((int32_t)r[8]>(int32_t)r[3]);goto L8c075720; /* cmp/gt r3,r8 */
L8c075720: if(!(*sr&1u))goto L8c07573c;goto L8c075722; /* bf 0x8c07573c */
L8c075722: *stop=0x8c075722u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075850u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c075724; /* mov.l 0x8c075850,r1 */
L8c075724: *stop=0x8c075724u;r[5]=r[14];goto L8c075726; /* mov r14,r5 */
L8c075726: *stop=0x8c075726u;{uint32_t loaded=rd(m,bus,&io_status,0x8c07584cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c075728; /* mov.l 0x8c07584c,r4 */
L8c075728: target=r[1];*pr=0x8c07572cu;*stop=0x8c07572au;r[5]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07572c; /* jsr @r1 */
L8c07572c: *stop=0x8c07572cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c07584cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c07572e; /* mov.l 0x8c07584c,r4 */
L8c07572e: *stop=0x8c07572eu;r[5]=r[10];goto L8c075730; /* mov r10,r5 */
L8c075730: *stop=0x8c075730u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075850u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075732; /* mov.l 0x8c075850,r3 */
L8c075732: *stop=0x8c075732u;fcopy(f,12u,0u);goto L8c075734; /* fmov fr0,fr12 */
L8c075734: target=r[3];*pr=0x8c075738u;*stop=0x8c075736u;r[5]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075738; /* jsr @r3 */
L8c075738: *stop=0x8c075738u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c07573a; /* fcmp/gt fr0,fr12 */
L8c07573a: if(!(*sr&1u))goto L8c075748;goto L8c07573c; /* bf 0x8c075748 */
L8c07573c: *stop=0x8c07573cu;r[0]=r[9];goto L8c07573e; /* mov r9,r0 */
L8c07573e: *stop=0x8c07573eu;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c075740; /* mov.l @(r0,r13),r2 */
L8c075740: *stop=0x8c075740u;r[3]=2u;goto L8c075742; /* mov #0x2,r3 */
L8c075742: *stop=0x8c075742u;r[2]|=r[3];goto L8c075744; /* or r3,r2 */
L8c075744: *stop=0x8c075746u;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[2],4);if(io_status)return io_status;goto L8c0757a6; /* bra 0x8c0757a6 */
L8c075748: *stop=0x8c075748u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07574a; /* mov.l @(0x10,r14),r1 */
L8c07574a: *stop=0x8c07574au;r[0]=r[15];goto L8c07574c; /* mov r15,r0 */
L8c07574c: *stop=0x8c07574cu;r[0]+=8u;goto L8c07574e; /* add #0x8,r0 */
L8c07574e: *stop=0x8c07574eu;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075750; /* mov.l @(0x14,r14),r3 */
L8c075750: *stop=0x8c075750u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c075752; /* mov.l r1,@r0 */
L8c075752: *stop=0x8c075752u;r[4]=r[15];goto L8c075754; /* mov r15,r4 */
L8c075754: *stop=0x8c075754u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075854u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075756; /* mov.l 0x8c075854,r2 */
L8c075756: *stop=0x8c075756u;if(!valid_access(r[0]+4u,4u))return -3;wr(m,bus,&io_status,r[0]+0x4u,r[3],4);if(io_status)return io_status;goto L8c075758; /* mov.l r3,@(0x4,r0) */
L8c075758: *stop=0x8c075758u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07575a; /* mov.l @(0x18,r14),r1 */
L8c07575a: *stop=0x8c07575au;if(!valid_access(r[0]+8u,4u))return -3;wr(m,bus,&io_status,r[0]+0x8u,r[1],4);if(io_status)return io_status;goto L8c07575c; /* mov.l r1,@(0x8,r0) */
L8c07575c: target=r[2];*pr=0x8c075760u;*stop=0x8c07575eu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075760; /* jsr @r2 */
L8c075760: *stop=0x8c075760u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c075762; /* fldi0 fr3 */
L8c075762: *stop=0x8c075762u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c075764; /* fcmp/eq fr3,fr0 */
L8c075764: if((*sr&1u))goto L8c07576e;goto L8c075766; /* bt 0x8c07576e */
L8c075766: *stop=0x8c075766u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075858u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075768; /* mov.l 0x8c075858,r2 */
L8c075768: *stop=0x8c075768u;r[4]=r[15];goto L8c07576a; /* mov r15,r4 */
L8c07576a: target=r[2];*pr=0x8c07576eu;*stop=0x8c07576cu;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07576e; /* jsr @r2 */
L8c07576e: *stop=0x8c07576eu;r[3]=112u;goto L8c075770; /* mov #0x70,r3 */
L8c075770: *stop=0x8c075770u;r[3]<<=8;goto L8c075772; /* shll8 r3 */
L8c075772: *stop=0x8c075772u;*sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[3]);goto L8c075774; /* cmp/ge r3,r8 */
L8c075774: if((*sr&1u))goto L8c0757a6;goto L8c075776; /* bt 0x8c0757a6 */
L8c075776: *stop=0x8c075776u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075850u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075778; /* mov.l 0x8c075850,r3 */
L8c075778: *stop=0x8c075778u;r[5]=r[10];goto L8c07577a; /* mov r10,r5 */
L8c07577a: *stop=0x8c07577au;r[4]=r[15];goto L8c07577c; /* mov r15,r4 */
L8c07577c: *stop=0x8c07577cu;r[5]+=20u;goto L8c07577e; /* add #0x14,r5 */
L8c07577e: target=r[3];*pr=0x8c075782u;*stop=0x8c075780u;r[4]+=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075782; /* jsr @r3 */
L8c075782: *stop=0x8c075782u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c075784; /* fldi0 fr3 */
L8c075784: *stop=0x8c075784u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c075786; /* fcmp/gt fr0,fr3 */
L8c075786: if(!(*sr&1u))goto L8c0757a6;goto L8c075788; /* bf 0x8c0757a6 */
L8c075788: *stop=0x8c075788u;r[0]=r[9];goto L8c07578a; /* mov r9,r0 */
L8c07578a: *stop=0x8c07578au;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c07578c; /* mov.l @(r0,r13),r2 */
L8c07578c: *stop=0x8c07578cu;r[3]=8u;goto L8c07578e; /* mov #0x8,r3 */
L8c07578e: *stop=0x8c07578eu;r[2]|=r[3];goto L8c075790; /* or r3,r2 */
L8c075790: *stop=0x8c075790u;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[2],4);if(io_status)return io_status;goto L8c075792; /* mov.l r2,@(r0,r13) */
L8c075792: *stop=0x8c075792u;r[0]=r[13];goto L8c075794; /* mov r13,r0 */
L8c075794: *stop=0x8c075794u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c075796; /* mov.l @r15,r1 */
L8c075796: *stop=0x8c075796u;r[1]+=1u;goto L8c075798; /* add #0x1,r1 */
L8c075798: *stop=0x8c075798u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c07579a; /* mov.l r1,@r15 */
L8c07579a: *stop=0x8c07579au;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c07579c; /* fmov.s @(r0,r12),fr2 */
L8c07579c: *stop=0x8c07579cu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c07579e; /* fcmp/gt fr2,fr15 */
L8c07579e: if(!(*sr&1u))goto L8c0757a6;goto L8c0757a0; /* bf 0x8c0757a6 */
L8c0757a0: *stop=0x8c0757a0u;r[0]=r[13];goto L8c0757a2; /* mov r13,r0 */
L8c0757a2: *stop=0x8c0757a2u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[12]);if(io_status)return io_status;goto L8c0757a4; /* fmov.s @(r0,r12),fr15 */
L8c0757a4: *stop=0x8c0757a4u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[11],4);if(io_status)return io_status;goto L8c0757a6; /* mov.l r11,@(0x4,r15) */
L8c0757a6: *stop=0x8c0757a6u;r[11]+=1u;goto L8c0757a8; /* add #0x1,r11 */
L8c0757a8: *stop=0x8c0757a8u;r[0]=40u;goto L8c0757aa; /* mov #0x28,r0 */
L8c0757aa: *stop=0x8c0757aau;if(!valid_access(r[0]+r[14],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c0757ac; /* mov.w @(r0,r14),r3 */
L8c0757ac: *stop=0x8c0757acu;r[3]=r[3]&65535u;goto L8c0757ae; /* extu.w r3,r3 */
L8c0757ae: *stop=0x8c0757aeu;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[3]);goto L8c0757b0; /* cmp/ge r3,r11 */
L8c0757b0: if(!(*sr&1u))goto L8c0756e0;goto L8c0757b2; /* bf 0x8c0756e0 */
L8c0757b2: *stop=0x8c0757b2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0757b4; /* mov.l @r15,r2 */
L8c0757b4: *stop=0x8c0757b4u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c0757b6; /* cmp/pl r2 */
L8c0757b6: if(!(*sr&1u))goto L8c075818;goto L8c0757b8; /* bf 0x8c075818 */
L8c0757b8: *stop=0x8c0757b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c075854u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0757ba; /* mov.l 0x8c075854,r2 */
L8c0757ba: *stop=0x8c0757bau;r[4]=r[14];goto L8c0757bc; /* mov r14,r4 */
L8c0757bc: target=r[2];*pr=0x8c0757c0u;*stop=0x8c0757beu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0757c0; /* jsr @r2 */
L8c0757c0: *stop=0x8c0757c0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c0757c2; /* mov.l @(0x4,r15),r13 */
L8c0757c2: *stop=0x8c0757c2u;fcopy(f,4u,0u);goto L8c0757c4; /* fmov fr0,fr4 */
L8c0757c4: *stop=0x8c0757c4u;r[13]<<=2;goto L8c0757c6; /* shll2 r13 */
L8c0757c6: *stop=0x8c0757c6u;r[0]=r[13];goto L8c0757c8; /* mov r13,r0 */
L8c0757c8: *stop=0x8c0757c8u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c0757ca; /* fmov.s @(r0,r12),fr3 */
L8c0757ca: *stop=0x8c0757cau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0757cc; /* fcmp/gt fr3,fr4 */
L8c0757cc: if(!(*sr&1u))goto L8c075818;goto L8c0757ce; /* bf 0x8c075818 */
L8c0757ce: *stop=0x8c0757ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c075854u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0757d0; /* mov.l 0x8c075854,r2 */
L8c0757d0: *stop=0x8c0757d0u;r[4]=r[14];goto L8c0757d2; /* mov r14,r4 */
L8c0757d2: target=r[2];*pr=0x8c0757d6u;*stop=0x8c0757d4u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0757d6; /* jsr @r2 */
L8c0757d6: *stop=0x8c0757d6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0757d8; /* fldi0 fr3 */
L8c0757d8: *stop=0x8c0757d8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c0757da; /* fcmp/eq fr3,fr0 */
L8c0757da: if(!(*sr&1u))goto L8c0757ea;goto L8c0757dc; /* bf 0x8c0757ea */
L8c0757dc: *stop=0x8c0757dcu;r[0]=24u;goto L8c0757de; /* mov #0x18,r0 */
L8c0757de: *stop=0x8c0757deu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0757e0; /* fmov fr3,@(r0,r14) */
L8c0757e0: *stop=0x8c0757e0u;r[0]=20u;goto L8c0757e2; /* mov #0x14,r0 */
L8c0757e2: *stop=0x8c0757e2u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0757e4; /* fmov fr3,@(r0,r14) */
L8c0757e4: *stop=0x8c0757e4u;r[0]=16u;goto L8c0757e6; /* mov #0x10,r0 */
L8c0757e6: *stop=0x8c0757e8u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c075814; /* bra 0x8c075814 */
L8c0757ea: *stop=0x8c0757eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c075858u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0757ec; /* mov.l 0x8c075858,r2 */
L8c0757ec: *stop=0x8c0757ecu;r[4]=r[14];goto L8c0757ee; /* mov r14,r4 */
L8c0757ee: target=r[2];*pr=0x8c0757f2u;*stop=0x8c0757f0u;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0757f2; /* jsr @r2 */
L8c0757f2: *stop=0x8c0757f2u;r[4]=r[13];goto L8c0757f4; /* mov r13,r4 */
L8c0757f4: *stop=0x8c0757f4u;r[4]+=r[12];goto L8c0757f6; /* add r12,r4 */
L8c0757f6: *stop=0x8c0757f6u;r[0]=16u;goto L8c0757f8; /* mov #0x10,r0 */
L8c0757f8: *stop=0x8c0757f8u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c0757fa; /* fmov.s @r4,fr3 */
L8c0757fa: *stop=0x8c0757fau;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c0757fc; /* fmov.s @(r0,r14),fr2 */
L8c0757fc: *stop=0x8c0757fcu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0757fe; /* fmul fr3,fr2 */
L8c0757fe: *stop=0x8c0757feu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c075800; /* fmov fr2,@(r0,r14) */
L8c075800: *stop=0x8c075800u;r[0]=20u;goto L8c075802; /* mov #0x14,r0 */
L8c075802: *stop=0x8c075802u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c075804; /* fmov.s @(r0,r14),fr2 */
L8c075804: *stop=0x8c075804u;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c075806; /* fmov.s @r4,fr3 */
L8c075806: *stop=0x8c075806u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c075808; /* fmul fr3,fr2 */
L8c075808: *stop=0x8c075808u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07580a; /* fmov fr2,@(r0,r14) */
L8c07580a: *stop=0x8c07580au;r[0]=24u;goto L8c07580c; /* mov #0x18,r0 */
L8c07580c: *stop=0x8c07580cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c07580e; /* fmov.s @(r0,r14),fr2 */
L8c07580e: *stop=0x8c07580eu;if(!valid_access(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[4]);if(io_status)return io_status;goto L8c075810; /* fmov.s @r4,fr3 */
L8c075810: *stop=0x8c075810u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c075812; /* fmul fr3,fr2 */
L8c075812: *stop=0x8c075812u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c075814; /* fmov fr2,@(r0,r14) */
L8c075814: *stop=0x8c075816u;r[0]=1u;goto L8c07581a; /* bra 0x8c07581a */
L8c075818: *stop=0x8c075818u;r[0]=0u;goto L8c07581a; /* mov #0x0,r0 */
L8c07581a: *stop=0x8c07581au;r[1]=4294967188u;goto L8c07581c; /* mov #-0x6c,r1 */
L8c07581c: *stop=0x8c07581cu;r[1]=r[1]&255u;goto L8c07581e; /* extu.b r1,r1 */
L8c07581e: *stop=0x8c07581eu;r[15]+=r[1];goto L8c075820; /* add r1,r15 */
L8c075820: *stop=0x8c075820u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c075822; /* lds.l @r15+,PR */
L8c075822: *stop=0x8c075822u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075824; /* fmov.s @r15+,fr12 */
L8c075824: *stop=0x8c075824u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c075826; /* fmov.s @r15+,fr15 */
L8c075826: *stop=0x8c075826u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c075828; /* mov.l @r15+,r8 */
L8c075828: *stop=0x8c075828u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c07582a; /* mov.l @r15+,r9 */
L8c07582a: *stop=0x8c07582au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c07582c; /* mov.l @r15+,r10 */
L8c07582c: *stop=0x8c07582cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c07582e; /* mov.l @r15+,r11 */
L8c07582e: *stop=0x8c07582eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c075830; /* mov.l @r15+,r12 */
L8c075830: *stop=0x8c075830u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c075832; /* mov.l @r15+,r13 */
L8c075832: *stop=0x8c075832u;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c075834u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_contact_candidates_8c075686(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075686(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c075700(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075686(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075700u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c075718(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075686(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075718u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c07572c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075686(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07572cu);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c075738(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075686(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075738u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c075760(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075686(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075760u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c07576e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075686(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07576eu);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c075782(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075686(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075782u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c0757c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075686(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0757c0u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c0757d6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075686(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0757d6u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c0757f2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075686(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0757f2u);}

static int sa_movement_contact_candidates_run_8c075f8c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr,uint32_t resume_pc){

 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;(void)valid_access;(void)r;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 if(resume_pc==0x8c075fe8u)goto L8c075fe8;if(resume_pc==0x8c076000u)goto L8c076000;if(resume_pc==0x8c076096u)goto L8c076096;if(resume_pc==0x8c0760a4u)goto L8c0760a4;if(resume_pc==0x8c0760bau)goto L8c0760ba;if(resume_pc==0x8c0760ceu)goto L8c0760ce;if(resume_pc==0x8c0760d8u)goto L8c0760d8;if(resume_pc==0x8c076120u)goto L8c076120;if(resume_pc==0x8c076140u)goto L8c076140;if(resume_pc==0x8c0761cau)goto L8c0761ca;if(resume_pc==0x8c0761e6u)goto L8c0761e6;if(resume_pc==0x8c0761eeu)goto L8c0761ee;if(resume_pc==0x8c07624au)goto L8c07624a;if(resume_pc==0x8c0762deu)goto L8c0762de;if(resume_pc==0x8c0762f8u)goto L8c0762f8;if(resume_pc==0x8c076306u)goto L8c076306;if(resume_pc==0x8c076314u)goto L8c076314;if(resume_pc==0x8c07634cu)goto L8c07634c;goto L8c075f8c;
L8c075f8c: *stop=0x8c075f8cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c075f8e; /* mov.l r14,@-r15 */
L8c075f8e: *stop=0x8c075f8eu;r[0]=4294967295u;goto L8c075f90; /* mov #-0x1,r0 */
L8c075f90: *stop=0x8c075f90u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c075f92; /* mov.l r13,@-r15 */
L8c075f92: *stop=0x8c075f92u;r[0]<<=8;goto L8c075f94; /* shll8 r0 */
L8c075f94: *stop=0x8c075f94u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c075f96; /* mov.l r12,@-r15 */
L8c075f96: *stop=0x8c075f96u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c075f98; /* mov.l r11,@-r15 */
L8c075f98: *stop=0x8c075f98u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c075f9a; /* mov.l r10,@-r15 */
L8c075f9a: *stop=0x8c075f9au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c075f9c; /* mov.l r9,@-r15 */
L8c075f9c: *stop=0x8c075f9cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c075f9e; /* mov.l r8,@-r15 */
L8c075f9e: *stop=0x8c075f9eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c075fa0; /* fmov.s fr15,@-r15 */
L8c075fa0: *stop=0x8c075fa0u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c075fa2; /* fmov.s fr14,@-r15 */
L8c075fa2: *stop=0x8c075fa2u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c075fa4; /* fmov.s fr13,@-r15 */
L8c075fa4: *stop=0x8c075fa4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c075fa6; /* sts.l PR,@-r15 */
L8c075fa6: *stop=0x8c075fa6u;r[15]+=r[0];goto L8c075fa8; /* add r0,r15 */
L8c075fa8: *stop=0x8c075fa8u;r[0]=0x8c076060u;goto L8c075faa; /* mova 0x8c076060,r0 */
L8c075faa: *stop=0x8c075faau;r[8]=r[15];goto L8c075fac; /* mov r15,r8 */
L8c075fac: *stop=0x8c075facu;r[2]=4294967295u;goto L8c075fae; /* mov #-0x1,r2 */
L8c075fae: *stop=0x8c075faeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076064u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c075fb0; /* mov.l 0x8c076064,r9 */
L8c075fb0: *stop=0x8c075fb0u;r[3]=0u;goto L8c075fb2; /* mov #0x0,r3 */
L8c075fb2: *stop=0x8c075fb2u;r[10]=r[15];goto L8c075fb4; /* mov r15,r10 */
L8c075fb4: *stop=0x8c075fb4u;r[8]+=32u;goto L8c075fb6; /* add #0x20,r8 */
L8c075fb6: *stop=0x8c075fb6u;r[11]=r[3];goto L8c075fb8; /* mov r3,r11 */
L8c075fb8: *stop=0x8c075fb8u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c075fba; /* mov.l r5,@r15 */
L8c075fba: *stop=0x8c075fbau;r[14]=r[7];goto L8c075fbc; /* mov r7,r14 */
L8c075fbc: *stop=0x8c075fbcu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c075fbe; /* mov.l r6,@(0x8,r15) */
L8c075fbe: *stop=0x8c075fbeu;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c075fc0; /* mov.l r3,@(0xc,r15) */
L8c075fc0: *stop=0x8c075fc0u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c075fc2; /* mov.l r2,@(0x10,r15) */
L8c075fc2: *stop=0x8c075fc2u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c075fc4; /* fmov.s @r0,fr15 */
L8c075fc4: *stop=0x8c075fc6u;r[10]+=96u;goto L8c076106; /* bra 0x8c076106 */
L8c075fc8: *stop=0x8c075fc8u;r[12]=88u;goto L8c075fca; /* mov #0x58,r12 */
L8c075fca: *stop=0x8c075fcau;r[13]=r[11];goto L8c075fcc; /* mov r11,r13 */
L8c075fcc: *stop=0x8c075fccu;*macl=r[12]*r[11];goto L8c075fce; /* mul.l r12,r11 */
L8c075fce: *stop=0x8c075fceu;r[13]<<=2;goto L8c075fd0; /* shll2 r13 */
L8c075fd0: *stop=0x8c075fd0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c076068u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c075fd2; /* mov.l 0x8c076068,r3 */
L8c075fd2: *stop=0x8c075fd2u;r[2]=r[13];goto L8c075fd4; /* mov r13,r2 */
L8c075fd4: *stop=0x8c075fd4u;r[2]+=r[10];goto L8c075fd6; /* add r10,r2 */
L8c075fd6: *stop=0x8c075fd6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c075fd8; /* mov.l r2,@-r15 */
L8c075fd8: *stop=0x8c075fd8u;r[5]=r[14];goto L8c075fda; /* mov r14,r5 */
L8c075fda: *stop=0x8c075fdau;r[12]=*macl;goto L8c075fdc; /* sts MACL,r12 */
L8c075fdc: *stop=0x8c075fdcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c07606cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075fde; /* mov.l 0x8c07606c,r2 */
L8c075fde: *stop=0x8c075fdeu;r[5]+=4u;goto L8c075fe0; /* add #0x4,r5 */
L8c075fe0: *stop=0x8c075fe0u;r[12]+=r[3];goto L8c075fe2; /* add r3,r12 */
L8c075fe2: *stop=0x8c075fe2u;r[4]=r[12];goto L8c075fe4; /* mov r12,r4 */
L8c075fe4: target=r[2];*pr=0x8c075fe8u;*stop=0x8c075fe6u;r[4]+=44u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c075fe8; /* jsr @r2 */
L8c075fe8: *stop=0x8c075fe8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c075fea; /* mov.l @r15+,r3 */
L8c075fea: *stop=0x8c075feau;r[5]=r[12];goto L8c075fec; /* mov r12,r5 */
L8c075fec: *stop=0x8c075fecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076070u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c075fee; /* mov.l 0x8c076070,r2 */
L8c075fee: *stop=0x8c075feeu;r[0]=r[13];goto L8c075ff0; /* mov r13,r0 */
L8c075ff0: *stop=0x8c075ff0u;if(!valid_access(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,0u,r[3]);if(io_status)return io_status;goto L8c075ff2; /* fmov.s fr0,@r3 */
L8c075ff2: *stop=0x8c075ff2u;r[3]=0u;goto L8c075ff4; /* mov #0x0,r3 */
L8c075ff4: *stop=0x8c075ff4u;r[4]=r[14];goto L8c075ff6; /* mov r14,r4 */
L8c075ff6: *stop=0x8c075ff6u;r[5]+=20u;goto L8c075ff8; /* add #0x14,r5 */
L8c075ff8: *stop=0x8c075ff8u;if(!valid_access(r[0]+r[8],4u))return -3;wr(m,bus,&io_status,r[8]+r[0],r[3],4);if(io_status)return io_status;goto L8c075ffa; /* mov.l r3,@(r0,r8) */
L8c075ffa: *stop=0x8c075ffau;r[6]=r[3];goto L8c075ffc; /* mov r3,r6 */
L8c075ffc: target=r[2];*pr=0x8c076000u;*stop=0x8c075ffeu;r[4]+=28u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c076000; /* jsr @r2 */
L8c076000: *stop=0x8c076000u;r[3]=44u;goto L8c076002; /* mov #0x2c,r3 */
L8c076002: *stop=0x8c076002u;r[3]<<=8;goto L8c076004; /* shll8 r3 */
L8c076004: *stop=0x8c076004u;*sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[3]);goto L8c076006; /* cmp/gt r3,r0 */
L8c076006: take=!(*sr&1u);*stop=0x8c076008u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],4);if(io_status)return io_status;if(take)goto L8c076016;goto L8c07600a; /* bf/s 0x8c076016 */
L8c07600a: *stop=0x8c07600au;r[0]=24u;goto L8c07600c; /* mov #0x18,r0 */
L8c07600c: *stop=0x8c07600cu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c07600e; /* fmov.s @(r0,r12),fr3 */
L8c07600e: *stop=0x8c07600eu;r[0]=32u;goto L8c076010; /* mov #0x20,r0 */
L8c076010: *stop=0x8c076010u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c076012; /* fmov.s @(r0,r14),fr2 */
L8c076012: *stop=0x8c076012u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c076014; /* fcmp/gt fr2,fr3 */
L8c076014: if(!(*sr&1u))goto L8c076080;goto L8c076016; /* bf 0x8c076080 */
L8c076016: *stop=0x8c076016u;r[0]=r[8];goto L8c076018; /* mov r8,r0 */
L8c076018: *stop=0x8c076018u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c07601a; /* mov.l @(r0,r13),r2 */
L8c07601a: *stop=0x8c07601au;r[3]=2u;goto L8c07601c; /* mov #0x2,r3 */
L8c07601c: *stop=0x8c07601cu;r[2]|=r[3];goto L8c07601e; /* or r3,r2 */
L8c07601e: *stop=0x8c076020u;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[2],4);if(io_status)return io_status;goto L8c076104; /* bra 0x8c076104 */
L8c076080: *stop=0x8c076080u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c076082; /* mov.l @(0x10,r14),r1 */
L8c076082: *stop=0x8c076082u;r[0]=r[15];goto L8c076084; /* mov r15,r0 */
L8c076084: *stop=0x8c076084u;r[0]+=20u;goto L8c076086; /* add #0x14,r0 */
L8c076086: *stop=0x8c076086u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c076088; /* mov.l @(0x14,r14),r3 */
L8c076088: *stop=0x8c076088u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[1],4);if(io_status)return io_status;goto L8c07608a; /* mov.l r1,@r0 */
L8c07608a: *stop=0x8c07608au;r[4]=r[15];goto L8c07608c; /* mov r15,r4 */
L8c07608c: *stop=0x8c07608cu;if(!valid_access(r[0]+4u,4u))return -3;wr(m,bus,&io_status,r[0]+0x4u,r[3],4);if(io_status)return io_status;goto L8c07608e; /* mov.l r3,@(0x4,r0) */
L8c07608e: *stop=0x8c07608eu;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c076090; /* mov.l @(0x18,r14),r1 */
L8c076090: *stop=0x8c076090u;if(!valid_access(r[0]+8u,4u))return -3;wr(m,bus,&io_status,r[0]+0x8u,r[1],4);if(io_status)return io_status;goto L8c076092; /* mov.l r1,@(0x8,r0) */
L8c076092: target=r[9];*pr=0x8c076096u;*stop=0x8c076094u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c076096; /* jsr @r9 */
L8c076096: *stop=0x8c076096u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c076098; /* fldi0 fr3 */
L8c076098: *stop=0x8c076098u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c07609a; /* fcmp/eq fr3,fr0 */
L8c07609a: if((*sr&1u))goto L8c0760a4;goto L8c07609c; /* bt 0x8c0760a4 */
L8c07609c: *stop=0x8c07609cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076220u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07609e; /* mov.l 0x8c076220,r2 */
L8c07609e: *stop=0x8c07609eu;r[4]=r[15];goto L8c0760a0; /* mov r15,r4 */
L8c0760a0: target=r[2];*pr=0x8c0760a4u;*stop=0x8c0760a2u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0760a4; /* jsr @r2 */
L8c0760a4: *stop=0x8c0760a4u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0760a6; /* mov.l @(0x4,r15),r2 */
L8c0760a6: *stop=0x8c0760a6u;r[3]=112u;goto L8c0760a8; /* mov #0x70,r3 */
L8c0760a8: *stop=0x8c0760a8u;r[3]<<=8;goto L8c0760aa; /* shll8 r3 */
L8c0760aa: *stop=0x8c0760aau;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);goto L8c0760ac; /* cmp/ge r3,r2 */
L8c0760ac: if((*sr&1u))goto L8c076104;goto L8c0760ae; /* bt 0x8c076104 */
L8c0760ae: *stop=0x8c0760aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076224u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0760b0; /* mov.l 0x8c076224,r3 */
L8c0760b0: *stop=0x8c0760b0u;r[5]=r[12];goto L8c0760b2; /* mov r12,r5 */
L8c0760b2: *stop=0x8c0760b2u;r[4]=r[15];goto L8c0760b4; /* mov r15,r4 */
L8c0760b4: *stop=0x8c0760b4u;r[5]+=20u;goto L8c0760b6; /* add #0x14,r5 */
L8c0760b6: target=r[3];*pr=0x8c0760bau;*stop=0x8c0760b8u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0760ba; /* jsr @r3 */
L8c0760ba: *stop=0x8c0760bau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0760bc; /* fldi0 fr3 */
L8c0760bc: *stop=0x8c0760bcu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c0760be; /* fcmp/gt fr0,fr3 */
L8c0760be: take=!(*sr&1u);*stop=0x8c0760c0u;fcopy(f,13u,0u);if(take)goto L8c076104;goto L8c0760c2; /* bf/s 0x8c076104 */
L8c0760c2: *stop=0x8c0760c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c076224u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0760c4; /* mov.l 0x8c076224,r2 */
L8c0760c4: *stop=0x8c0760c4u;r[5]=r[12];goto L8c0760c6; /* mov r12,r5 */
L8c0760c6: *stop=0x8c0760c6u;r[4]=r[14];goto L8c0760c8; /* mov r14,r4 */
L8c0760c8: *stop=0x8c0760c8u;r[5]+=20u;goto L8c0760ca; /* add #0x14,r5 */
L8c0760ca: target=r[2];*pr=0x8c0760ceu;*stop=0x8c0760ccu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0760ce; /* jsr @r2 */
L8c0760ce: *stop=0x8c0760ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076228u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0760d0; /* mov.l 0x8c076228,r3 */
L8c0760d0: *stop=0x8c0760d0u;r[4]=6u;goto L8c0760d2; /* mov #0x6,r4 */
L8c0760d2: *stop=0x8c0760d2u;r[4]<<=8;goto L8c0760d4; /* shll8 r4 */
L8c0760d4: target=r[3];*pr=0x8c0760d8u;*stop=0x8c0760d6u;fcopy(f,14u,0u);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0760d8; /* jsr @r3 */
L8c0760d8: *stop=0x8c0760d8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][0]^=0x80000000u;goto L8c0760da; /* fneg fr0 */
L8c0760da: *stop=0x8c0760dau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][13],&f->fpscr,sr,1);if(status)return status;goto L8c0760dc; /* fcmp/gt fr13,fr0 */
L8c0760dc: if((*sr&1u))goto L8c0760e6;goto L8c0760de; /* bt 0x8c0760e6 */
L8c0760de: *stop=0x8c0760deu;r[0]=0x8c07622cu;goto L8c0760e0; /* mova 0x8c07622c,r0 */
L8c0760e0: *stop=0x8c0760e0u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c0760e2; /* fmov.s @r0,fr3 */
L8c0760e2: *stop=0x8c0760e2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][14],&f->fpscr,sr,1);if(status)return status;goto L8c0760e4; /* fcmp/gt fr14,fr3 */
L8c0760e4: if(!(*sr&1u))goto L8c076104;goto L8c0760e6; /* bf 0x8c076104 */
L8c0760e6: *stop=0x8c0760e6u;r[0]=r[8];goto L8c0760e8; /* mov r8,r0 */
L8c0760e8: *stop=0x8c0760e8u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c0760ea; /* mov.l @(r0,r13),r2 */
L8c0760ea: *stop=0x8c0760eau;r[3]=8u;goto L8c0760ec; /* mov #0x8,r3 */
L8c0760ec: *stop=0x8c0760ecu;r[2]|=r[3];goto L8c0760ee; /* or r3,r2 */
L8c0760ee: *stop=0x8c0760eeu;if(!valid_access(r[0]+r[13],4u))return -3;wr(m,bus,&io_status,r[13]+r[0],r[2],4);if(io_status)return io_status;goto L8c0760f0; /* mov.l r2,@(r0,r13) */
L8c0760f0: *stop=0x8c0760f0u;r[0]=r[13];goto L8c0760f2; /* mov r13,r0 */
L8c0760f2: *stop=0x8c0760f2u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0760f4; /* mov.l @(0xc,r15),r1 */
L8c0760f4: *stop=0x8c0760f4u;r[1]+=1u;goto L8c0760f6; /* add #0x1,r1 */
L8c0760f6: *stop=0x8c0760f6u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c0760f8; /* mov.l r1,@(0xc,r15) */
L8c0760f8: *stop=0x8c0760f8u;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c0760fa; /* fmov.s @(r0,r10),fr3 */
L8c0760fa: *stop=0x8c0760fau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][15],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c0760fc; /* fcmp/gt fr3,fr15 */
L8c0760fc: if(!(*sr&1u))goto L8c076104;goto L8c0760fe; /* bf 0x8c076104 */
L8c0760fe: *stop=0x8c0760feu;r[0]=r[13];goto L8c076100; /* mov r13,r0 */
L8c076100: *stop=0x8c076100u;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]+r[10]);if(io_status)return io_status;goto L8c076102; /* fmov.s @(r0,r10),fr15 */
L8c076102: *stop=0x8c076102u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[11],4);if(io_status)return io_status;goto L8c076104; /* mov.l r11,@(0x10,r15) */
L8c076104: *stop=0x8c076104u;r[11]+=1u;goto L8c076106; /* add #0x1,r11 */
L8c076106: *stop=0x8c076106u;r[0]=40u;goto L8c076108; /* mov #0x28,r0 */
L8c076108: *stop=0x8c076108u;if(!valid_access(r[0]+r[14],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c07610a; /* mov.w @(r0,r14),r3 */
L8c07610a: *stop=0x8c07610au;r[3]=r[3]&65535u;goto L8c07610c; /* extu.w r3,r3 */
L8c07610c: *stop=0x8c07610cu;*sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[3]);goto L8c07610e; /* cmp/ge r3,r11 */
L8c07610e: if((*sr&1u))goto L8c076114;goto L8c076110; /* bt 0x8c076114 */
L8c076110: *stop=0x8c076112u;goto L8c075fc8; /* bra 0x8c075fc8 */
L8c076114: *stop=0x8c076114u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c076116; /* mov.l @(0xc,r15),r2 */
L8c076116: *stop=0x8c076116u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c076118; /* cmp/pl r2 */
L8c076118: if(!(*sr&1u))goto L8c0761d0;goto L8c07611a; /* bf 0x8c0761d0 */
L8c07611a: *stop=0x8c07611au;r[4]=r[14];goto L8c07611c; /* mov r14,r4 */
L8c07611c: target=r[9];*pr=0x8c076120u;*stop=0x8c07611eu;r[4]+=16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c076120; /* jsr @r9 */
L8c076120: *stop=0x8c076120u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c076122; /* mov.l @(0x10,r15),r0 */
L8c076122: *stop=0x8c076122u;fcopy(f,4u,0u);goto L8c076124; /* fmov fr0,fr4 */
L8c076124: *stop=0x8c076124u;r[0]<<=2;goto L8c076126; /* shll2 r0 */
L8c076126: *stop=0x8c076126u;if(!valid_access(r[0]+r[10],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[10]);if(io_status)return io_status;goto L8c076128; /* fmov.s @(r0,r10),fr3 */
L8c076128: *stop=0x8c076128u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c07612a; /* fcmp/gt fr3,fr4 */
L8c07612a: if(!(*sr&1u))goto L8c0761d0;goto L8c07612c; /* bf 0x8c0761d0 */
L8c07612c: *stop=0x8c07612cu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07612e; /* mov.l @(0x8,r15),r2 */
L8c07612e: *stop=0x8c07612eu;r[0]=96u;goto L8c076130; /* mov #0x60,r0 */
L8c076130: *stop=0x8c076130u;r[3]=64u;goto L8c076132; /* mov #0x40,r3 */
L8c076132: *stop=0x8c076132u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c076134; /* mov.l @(r0,r2),r1 */
L8c076134: *stop=0x8c076134u;r[3]<<=8;goto L8c076136; /* shll8 r3 */
L8c076136: *stop=0x8c076136u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c076138; /* tst r3,r1 */
L8c076138: if(!(*sr&1u))goto L8c0761d0;goto L8c07613a; /* bf 0x8c0761d0 */
L8c07613a: *stop=0x8c07613au;r[4]=r[14];goto L8c07613c; /* mov r14,r4 */
L8c07613c: target=r[9];*pr=0x8c076140u;*stop=0x8c07613eu;r[4]+=108u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c076140; /* jsr @r9 */
L8c076140: *stop=0x8c076140u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c076142; /* fldi0 fr3 */
L8c076142: *stop=0x8c076142u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c076144; /* fcmp/eq fr3,fr0 */
L8c076144: if(!(*sr&1u))goto L8c076154;goto L8c076146; /* bf 0x8c076154 */
L8c076146: *stop=0x8c076146u;r[0]=116u;goto L8c076148; /* mov #0x74,r0 */
L8c076148: *stop=0x8c076148u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c07614a; /* fmov fr3,@(r0,r14) */
L8c07614a: *stop=0x8c07614au;r[0]=112u;goto L8c07614c; /* mov #0x70,r0 */
L8c07614c: *stop=0x8c07614cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c07614e; /* fmov fr3,@(r0,r14) */
L8c07614e: *stop=0x8c07614eu;r[0]=108u;goto L8c076150; /* mov #0x6c,r0 */
L8c076150: *stop=0x8c076152u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c0761d0; /* bra 0x8c0761d0 */
L8c076154: *stop=0x8c076154u;r[1]=120u;goto L8c076156; /* mov #0x78,r1 */
L8c076156: *stop=0x8c076156u;r[1]+=r[14];goto L8c076158; /* add r14,r1 */
L8c076158: *stop=0x8c076158u;r[0]=108u;goto L8c07615a; /* mov #0x6c,r0 */
L8c07615a: *stop=0x8c07615au;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c07615c; /* fmov.s @r1,fr2 */
L8c07615c: *stop=0x8c07615cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c07615e; /* fmov.s @(r0,r14),fr1 */
L8c07615e: *stop=0x8c07615eu;r[1]=124u;goto L8c076160; /* mov #0x7c,r1 */
L8c076160: *stop=0x8c076160u;r[1]+=r[14];goto L8c076162; /* add r14,r1 */
L8c076162: *stop=0x8c076162u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c076164; /* fadd fr2,fr1 */
L8c076164: *stop=0x8c076164u;r[3]=88u;goto L8c076166; /* mov #0x58,r3 */
L8c076166: *stop=0x8c076166u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c076168; /* fmov fr1,@(r0,r14) */
L8c076168: *stop=0x8c076168u;r[0]=112u;goto L8c07616a; /* mov #0x70,r0 */
L8c07616a: *stop=0x8c07616au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c07616c; /* fmov.s @(r0,r14),fr1 */
L8c07616c: *stop=0x8c07616cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c07616e; /* fmov.s @r1,fr2 */
L8c07616e: *stop=0x8c07616eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c076170; /* fadd fr2,fr1 */
L8c076170: *stop=0x8c076170u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c076172; /* fmov fr1,@(r0,r14) */
L8c076172: *stop=0x8c076172u;r[0]=116u;goto L8c076174; /* mov #0x74,r0 */
L8c076174: *stop=0x8c076174u;r[1]=r[0];goto L8c076176; /* mov r0,r1 */
L8c076176: *stop=0x8c076176u;r[1]+=12u;goto L8c076178; /* add #0xc,r1 */
L8c076178: *stop=0x8c076178u;r[1]+=r[14];goto L8c07617a; /* add r14,r1 */
L8c07617a: *stop=0x8c07617au;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c07617c; /* fmov.s @(r0,r14),fr1 */
L8c07617c: *stop=0x8c07617cu;if(!valid_access(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c07617e; /* fmov.s @r1,fr2 */
L8c07617e: *stop=0x8c07617eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076230u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c076180; /* mov.l 0x8c076230,r1 */
L8c076180: *stop=0x8c076180u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c076182; /* fadd fr2,fr1 */
L8c076182: *stop=0x8c076182u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,1u,r[0]+r[14]);if(io_status)return io_status;goto L8c076184; /* fmov fr1,@(r0,r14) */
L8c076184: *stop=0x8c076184u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c076186; /* mov.l @(0x10,r15),r2 */
L8c076186: *stop=0x8c076186u;*macl=r[3]*r[2];goto L8c076188; /* mul.l r3,r2 */
L8c076188: *stop=0x8c076188u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c07618a; /* mov.l @(0x8,r15),r2 */
L8c07618a: *stop=0x8c07618au;r[2]+=68u;goto L8c07618c; /* add #0x44,r2 */
L8c07618c: *stop=0x8c07618cu;r[13]=*macl;goto L8c07618e; /* sts MACL,r13 */
L8c07618e: *stop=0x8c07618eu;r[13]+=r[1];goto L8c076190; /* add r1,r13 */
L8c076190: *stop=0x8c076190u;if(!valid_access(r[13]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x14u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c076192; /* mov.l @(0x14,r13),r0 */
L8c076192: *stop=0x8c076192u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c076194; /* mov.l @(0x18,r13),r3 */
L8c076194: *stop=0x8c076194u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c076196; /* mov.l r0,@r2 */
L8c076196: *stop=0x8c076196u;r[4]=4294967200u;goto L8c076198; /* mov #-0x60,r4 */
L8c076198: *stop=0x8c076198u;r[4]=r[4]&255u;goto L8c07619a; /* extu.b r4,r4 */
L8c07619a: *stop=0x8c07619au;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c07619c; /* mov.l r3,@(0x4,r2) */
L8c07619c: *stop=0x8c07619cu;if(!valid_access(r[13]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x1cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c07619e; /* mov.l @(0x1c,r13),r0 */
L8c07619e: *stop=0x8c07619eu;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[0],4);if(io_status)return io_status;goto L8c0761a0; /* mov.l r0,@(0x8,r2) */
L8c0761a0: *stop=0x8c0761a0u;r[0]=4u;goto L8c0761a2; /* mov #0x4,r0 */
L8c0761a2: *stop=0x8c0761a2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}goto L8c0761a4; /* mov.l @r15,r12 */
L8c0761a4: *stop=0x8c0761a4u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c0761a6; /* fmov.s @(r0,r12),fr3 */
L8c0761a6: *stop=0x8c0761a6u;r[0]=4294967200u;goto L8c0761a8; /* mov #-0x60,r0 */
L8c0761a8: *stop=0x8c0761a8u;r[0]=r[0]&255u;goto L8c0761aa; /* extu.b r0,r0 */
L8c0761aa: *stop=0x8c0761aau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c0761ac; /* fneg fr3 */
L8c0761ac: *stop=0x8c0761acu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0761ae; /* fmov fr3,@(r0,r15) */
L8c0761ae: *stop=0x8c0761aeu;r[0]=8u;goto L8c0761b0; /* mov #0x8,r0 */
L8c0761b0: *stop=0x8c0761b0u;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c0761b2; /* fmov.s @(r0,r12),fr3 */
L8c0761b2: *stop=0x8c0761b2u;r[0]=4294967204u;goto L8c0761b4; /* mov #-0x5c,r0 */
L8c0761b4: *stop=0x8c0761b4u;r[0]=r[0]&255u;goto L8c0761b6; /* extu.b r0,r0 */
L8c0761b6: *stop=0x8c0761b6u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c0761b8; /* fneg fr3 */
L8c0761b8: *stop=0x8c0761b8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0761ba; /* fmov fr3,@(r0,r15) */
L8c0761ba: *stop=0x8c0761bau;r[0]=12u;goto L8c0761bc; /* mov #0xc,r0 */
L8c0761bc: *stop=0x8c0761bcu;if(!valid_access(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c0761be; /* fmov.s @(r0,r12),fr3 */
L8c0761be: *stop=0x8c0761beu;r[0]=4294967208u;goto L8c0761c0; /* mov #-0x58,r0 */
L8c0761c0: *stop=0x8c0761c0u;r[0]=r[0]&255u;goto L8c0761c2; /* extu.b r0,r0 */
L8c0761c2: *stop=0x8c0761c2u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c0761c4; /* fneg fr3 */
L8c0761c4: *stop=0x8c0761c4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0761c6; /* fmov fr3,@(r0,r15) */
L8c0761c6: target=r[9];*pr=0x8c0761cau;*stop=0x8c0761c8u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0761ca; /* jsr @r9 */
L8c0761ca: *stop=0x8c0761cau;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0761cc; /* fldi0 fr3 */
L8c0761cc: *stop=0x8c0761ccu;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c0761ce; /* fcmp/eq fr3,fr0 */
L8c0761ce: if(!(*sr&1u))goto L8c0761d4;goto L8c0761d0; /* bf 0x8c0761d4 */
L8c0761d0: *stop=0x8c0761d2u;r[0]=0u;goto L8c076382; /* bra 0x8c076382 */
L8c0761d4: *stop=0x8c0761d4u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0761d6; /* mov.l @(0x8,r15),r5 */
L8c0761d6: *stop=0x8c0761d6u;r[6]=4294967212u;goto L8c0761d8; /* mov #-0x54,r6 */
L8c0761d8: *stop=0x8c0761d8u;r[6]=r[6]&255u;goto L8c0761da; /* extu.b r6,r6 */
L8c0761da: *stop=0x8c0761dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c076234u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0761dc; /* mov.l 0x8c076234,r3 */
L8c0761dc: *stop=0x8c0761dcu;r[4]=r[14];goto L8c0761de; /* mov r14,r4 */
L8c0761de: *stop=0x8c0761deu;r[6]+=r[15];goto L8c0761e0; /* add r15,r6 */
L8c0761e0: *stop=0x8c0761e0u;r[5]+=68u;goto L8c0761e2; /* add #0x44,r5 */
L8c0761e2: target=r[3];*pr=0x8c0761e6u;*stop=0x8c0761e4u;r[4]+=108u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0761e6; /* jsr @r3 */
L8c0761e6: *stop=0x8c0761e6u;r[4]=4294967212u;goto L8c0761e8; /* mov #-0x54,r4 */
L8c0761e8: *stop=0x8c0761e8u;r[4]=r[4]&255u;goto L8c0761ea; /* extu.b r4,r4 */
L8c0761ea: target=r[9];*pr=0x8c0761eeu;*stop=0x8c0761ecu;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0761ee; /* jsr @r9 */
L8c0761ee: *stop=0x8c0761eeu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0761f0; /* fldi0 fr3 */
L8c0761f0: *stop=0x8c0761f0u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c0761f2; /* fcmp/eq fr3,fr0 */
L8c0761f2: if(!(*sr&1u))goto L8c076240;goto L8c0761f4; /* bf 0x8c076240 */
L8c0761f4: *stop=0x8c0761f4u;r[2]=4294967212u;goto L8c0761f6; /* mov #-0x54,r2 */
L8c0761f6: *stop=0x8c0761f6u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0761f8; /* mov.l @(0x1c,r14),r3 */
L8c0761f8: *stop=0x8c0761f8u;r[2]=r[2]&255u;goto L8c0761fa; /* extu.b r2,r2 */
L8c0761fa: *stop=0x8c0761fau;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0761fc; /* mov.l @(0x20,r14),r1 */
L8c0761fc: *stop=0x8c0761fcu;r[2]+=r[15];goto L8c0761fe; /* add r15,r2 */
L8c0761fe: *stop=0x8c0761feu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c076200; /* mov.l r3,@r2 */
L8c076200: *stop=0x8c076200u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[1],4);if(io_status)return io_status;goto L8c076202; /* mov.l r1,@(0x4,r2) */
L8c076202: *stop=0x8c076202u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c076204; /* mov.l @(0x24,r14),r3 */
L8c076204: *stop=0x8c076206u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c07624a; /* bra 0x8c07624a */
L8c076240: *stop=0x8c076240u;{uint32_t loaded=rd(m,bus,&io_status,0x8c076420u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c076242; /* mov.l 0x8c076420,r3 */
L8c076242: *stop=0x8c076242u;r[4]=4294967212u;goto L8c076244; /* mov #-0x54,r4 */
L8c076244: *stop=0x8c076244u;r[4]=r[4]&255u;goto L8c076246; /* extu.b r4,r4 */
L8c076246: target=r[3];*pr=0x8c07624au;*stop=0x8c076248u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07624a; /* jsr @r3 */
L8c07624a: *stop=0x8c07624au;r[0]=32u;goto L8c07624c; /* mov #0x20,r0 */
L8c07624c: *stop=0x8c07624cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c07624e; /* fmov.s @(r0,r13),fr3 */
L8c07624e: *stop=0x8c07624eu;r[0]=4294967272u;goto L8c076250; /* mov #-0x18,r0 */
L8c076250: *stop=0x8c076250u;r[0]=r[0]&255u;goto L8c076252; /* extu.b r0,r0 */
L8c076252: *stop=0x8c076252u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076254; /* fmov fr3,@(r0,r15) */
L8c076254: *stop=0x8c076254u;r[0]=36u;goto L8c076256; /* mov #0x24,r0 */
L8c076256: *stop=0x8c076256u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c076258; /* fmov.s @(r0,r13),fr3 */
L8c076258: *stop=0x8c076258u;r[0]=4294967276u;goto L8c07625a; /* mov #-0x14,r0 */
L8c07625a: *stop=0x8c07625au;r[0]=r[0]&255u;goto L8c07625c; /* extu.b r0,r0 */
L8c07625c: *stop=0x8c07625cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07625e; /* fmov fr3,@(r0,r15) */
L8c07625e: *stop=0x8c07625eu;r[0]=40u;goto L8c076260; /* mov #0x28,r0 */
L8c076260: *stop=0x8c076260u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c076262; /* fmov.s @(r0,r13),fr3 */
L8c076262: *stop=0x8c076262u;r[0]=4294967280u;goto L8c076264; /* mov #-0x10,r0 */
L8c076264: *stop=0x8c076264u;r[0]=r[0]&255u;goto L8c076266; /* extu.b r0,r0 */
L8c076266: *stop=0x8c076266u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076268; /* fmov fr3,@(r0,r15) */
L8c076268: *stop=0x8c076268u;r[0]=20u;goto L8c07626a; /* mov #0x14,r0 */
L8c07626a: *stop=0x8c07626au;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c07626c; /* fmov.s @(r0,r13),fr3 */
L8c07626c: *stop=0x8c07626cu;r[0]=4294967284u;goto L8c07626e; /* mov #-0xc,r0 */
L8c07626e: *stop=0x8c07626eu;r[0]=r[0]&255u;goto L8c076270; /* extu.b r0,r0 */
L8c076270: *stop=0x8c076270u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076272; /* fmov fr3,@(r0,r15) */
L8c076272: *stop=0x8c076272u;r[0]=24u;goto L8c076274; /* mov #0x18,r0 */
L8c076274: *stop=0x8c076274u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c076276; /* fmov.s @(r0,r13),fr3 */
L8c076276: *stop=0x8c076276u;r[0]=4294967288u;goto L8c076278; /* mov #-0x8,r0 */
L8c076278: *stop=0x8c076278u;r[0]=r[0]&255u;goto L8c07627a; /* extu.b r0,r0 */
L8c07627a: *stop=0x8c07627au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07627c; /* fmov fr3,@(r0,r15) */
L8c07627c: *stop=0x8c07627cu;r[0]=28u;goto L8c07627e; /* mov #0x1c,r0 */
L8c07627e: *stop=0x8c07627eu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c076280; /* fmov.s @(r0,r13),fr3 */
L8c076280: *stop=0x8c076280u;r[0]=4294967292u;goto L8c076282; /* mov #-0x4,r0 */
L8c076282: *stop=0x8c076282u;r[0]=r[0]&255u;goto L8c076284; /* extu.b r0,r0 */
L8c076284: *stop=0x8c076284u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076286; /* fmov fr3,@(r0,r15) */
L8c076286: *stop=0x8c076286u;r[0]=96u;goto L8c076288; /* mov #0x60,r0 */
L8c076288: *stop=0x8c076288u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c07628a; /* fmov.s @(r0,r14),fr3 */
L8c07628a: *stop=0x8c07628au;r[0]=4294967248u;goto L8c07628c; /* mov #-0x30,r0 */
L8c07628c: *stop=0x8c07628cu;r[0]=r[0]&255u;goto L8c07628e; /* extu.b r0,r0 */
L8c07628e: *stop=0x8c07628eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076290; /* fmov fr3,@(r0,r15) */
L8c076290: *stop=0x8c076290u;r[0]=100u;goto L8c076292; /* mov #0x64,r0 */
L8c076292: *stop=0x8c076292u;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c076294; /* fmov.s @(r0,r14),fr3 */
L8c076294: *stop=0x8c076294u;r[0]=4294967252u;goto L8c076296; /* mov #-0x2c,r0 */
L8c076296: *stop=0x8c076296u;r[0]=r[0]&255u;goto L8c076298; /* extu.b r0,r0 */
L8c076298: *stop=0x8c076298u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c07629a; /* fmov fr3,@(r0,r15) */
L8c07629a: *stop=0x8c07629au;r[0]=104u;goto L8c07629c; /* mov #0x68,r0 */
L8c07629c: *stop=0x8c07629cu;if(!valid_access(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c07629e; /* fmov.s @(r0,r14),fr3 */
L8c07629e: *stop=0x8c07629eu;r[0]=4294967256u;goto L8c0762a0; /* mov #-0x28,r0 */
L8c0762a0: *stop=0x8c0762a0u;r[0]=r[0]&255u;goto L8c0762a2; /* extu.b r0,r0 */
L8c0762a2: *stop=0x8c0762a2u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0762a4; /* fmov fr3,@(r0,r15) */
L8c0762a4: *stop=0x8c0762a4u;r[0]=4294967212u;goto L8c0762a6; /* mov #-0x54,r0 */
L8c0762a6: *stop=0x8c0762a6u;r[0]=r[0]&255u;goto L8c0762a8; /* extu.b r0,r0 */
L8c0762a8: *stop=0x8c0762a8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0762aa; /* fmov.s @(r0,r15),fr3 */
L8c0762aa: *stop=0x8c0762aau;r[0]=4294967260u;goto L8c0762ac; /* mov #-0x24,r0 */
L8c0762ac: *stop=0x8c0762acu;r[0]=r[0]&255u;goto L8c0762ae; /* extu.b r0,r0 */
L8c0762ae: *stop=0x8c0762aeu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0762b0; /* fmov fr3,@(r0,r15) */
L8c0762b0: *stop=0x8c0762b0u;r[0]=4294967216u;goto L8c0762b2; /* mov #-0x50,r0 */
L8c0762b2: *stop=0x8c0762b2u;r[0]=r[0]&255u;goto L8c0762b4; /* extu.b r0,r0 */
L8c0762b4: *stop=0x8c0762b4u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0762b6; /* fmov.s @(r0,r15),fr3 */
L8c0762b6: *stop=0x8c0762b6u;r[0]=4294967264u;goto L8c0762b8; /* mov #-0x20,r0 */
L8c0762b8: *stop=0x8c0762b8u;r[0]=r[0]&255u;goto L8c0762ba; /* extu.b r0,r0 */
L8c0762ba: *stop=0x8c0762bau;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0762bc; /* fmov fr3,@(r0,r15) */
L8c0762bc: *stop=0x8c0762bcu;r[0]=4294967220u;goto L8c0762be; /* mov #-0x4c,r0 */
L8c0762be: *stop=0x8c0762beu;r[0]=r[0]&255u;goto L8c0762c0; /* extu.b r0,r0 */
L8c0762c0: *stop=0x8c0762c0u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0762c2; /* fmov.s @(r0,r15),fr3 */
L8c0762c2: *stop=0x8c0762c2u;r[0]=4294967268u;goto L8c0762c4; /* mov #-0x1c,r0 */
L8c0762c4: *stop=0x8c0762c4u;r[0]=r[0]&255u;goto L8c0762c6; /* extu.b r0,r0 */
L8c0762c6: *stop=0x8c0762c6u;r[6]=4294967224u;goto L8c0762c8; /* mov #-0x48,r6 */
L8c0762c8: *stop=0x8c0762c8u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c0762ca; /* fmov fr3,@(r0,r15) */
L8c0762ca: *stop=0x8c0762cau;r[5]=4294967248u;goto L8c0762cc; /* mov #-0x30,r5 */
L8c0762cc: *stop=0x8c0762ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076424u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0762ce; /* mov.l 0x8c076424,r3 */
L8c0762ce: *stop=0x8c0762ceu;r[4]=4294967272u;goto L8c0762d0; /* mov #-0x18,r4 */
L8c0762d0: *stop=0x8c0762d0u;r[5]=r[5]&255u;goto L8c0762d2; /* extu.b r5,r5 */
L8c0762d2: *stop=0x8c0762d2u;r[6]=r[6]&255u;goto L8c0762d4; /* extu.b r6,r6 */
L8c0762d4: *stop=0x8c0762d4u;r[4]=r[4]&255u;goto L8c0762d6; /* extu.b r4,r4 */
L8c0762d6: *stop=0x8c0762d6u;r[5]+=r[15];goto L8c0762d8; /* add r15,r5 */
L8c0762d8: *stop=0x8c0762d8u;r[6]+=r[15];goto L8c0762da; /* add r15,r6 */
L8c0762da: target=r[3];*pr=0x8c0762deu;*stop=0x8c0762dcu;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0762de; /* jsr @r3 */
L8c0762de: *stop=0x8c0762deu;r[2]=r[14];goto L8c0762e0; /* mov r14,r2 */
L8c0762e0: *stop=0x8c0762e0u;r[2]+=108u;goto L8c0762e2; /* add #0x6c,r2 */
L8c0762e2: *stop=0x8c0762e2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c0762e4; /* mov.l @r2,r1 */
L8c0762e4: *stop=0x8c0762e4u;r[3]=r[15];goto L8c0762e6; /* mov r15,r3 */
L8c0762e6: *stop=0x8c0762e6u;r[3]+=20u;goto L8c0762e8; /* add #0x14,r3 */
L8c0762e8: *stop=0x8c0762e8u;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c0762ea; /* mov.l @(0x4,r2),r0 */
L8c0762ea: *stop=0x8c0762eau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c0762ec; /* mov.l r1,@r3 */
L8c0762ec: *stop=0x8c0762ecu;r[4]=r[15];goto L8c0762ee; /* mov r15,r4 */
L8c0762ee: *stop=0x8c0762eeu;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[0],4);if(io_status)return io_status;goto L8c0762f0; /* mov.l r0,@(0x4,r3) */
L8c0762f0: *stop=0x8c0762f0u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0762f2; /* mov.l @(0x8,r2),r1 */
L8c0762f2: *stop=0x8c0762f2u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[1],4);if(io_status)return io_status;goto L8c0762f4; /* mov.l r1,@(0x8,r3) */
L8c0762f4: target=r[9];*pr=0x8c0762f8u;*stop=0x8c0762f6u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0762f8; /* jsr @r9 */
L8c0762f8: *stop=0x8c0762f8u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0762fa; /* fldi0 fr3 */
L8c0762fa: *stop=0x8c0762fau;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,0);if(status)return status;goto L8c0762fc; /* fcmp/eq fr3,fr0 */
L8c0762fc: if((*sr&1u))goto L8c076306;goto L8c0762fe; /* bt 0x8c076306 */
L8c0762fe: *stop=0x8c0762feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c076420u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c076300; /* mov.l 0x8c076420,r2 */
L8c076300: *stop=0x8c076300u;r[4]=r[15];goto L8c076302; /* mov r15,r4 */
L8c076302: target=r[2];*pr=0x8c076306u;*stop=0x8c076304u;r[4]+=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c076306; /* jsr @r2 */
L8c076306: *stop=0x8c076306u;r[4]=4294967236u;goto L8c076308; /* mov #-0x3c,r4 */
L8c076308: *stop=0x8c076308u;{uint32_t loaded=rd(m,bus,&io_status,0x8c076428u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c07630a; /* mov.l 0x8c076428,r3 */
L8c07630a: *stop=0x8c07630au;r[5]=r[15];goto L8c07630c; /* mov r15,r5 */
L8c07630c: *stop=0x8c07630cu;r[4]=r[4]&255u;goto L8c07630e; /* extu.b r4,r4 */
L8c07630e: *stop=0x8c07630eu;r[5]+=20u;goto L8c076310; /* add #0x14,r5 */
L8c076310: target=r[3];*pr=0x8c076314u;*stop=0x8c076312u;r[4]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c076314; /* jsr @r3 */
L8c076314: *stop=0x8c076314u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c076316; /* fldi0 fr3 */
L8c076316: *stop=0x8c076316u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c076318; /* fcmp/gt fr0,fr3 */
L8c076318: if(!(*sr&1u))goto L8c076344;goto L8c07631a; /* bf 0x8c076344 */
L8c07631a: *stop=0x8c07631au;r[0]=4294967236u;goto L8c07631c; /* mov #-0x3c,r0 */
L8c07631c: *stop=0x8c07631cu;r[0]=r[0]&255u;goto L8c07631e; /* extu.b r0,r0 */
L8c07631e: *stop=0x8c07631eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c076320; /* fmov.s @(r0,r15),fr2 */
L8c076320: *stop=0x8c076320u;r[0]=4294967236u;goto L8c076322; /* mov #-0x3c,r0 */
L8c076322: *stop=0x8c076322u;r[0]=r[0]&255u;goto L8c076324; /* extu.b r0,r0 */
L8c076324: *stop=0x8c076324u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c076326; /* fneg fr2 */
L8c076326: *stop=0x8c076326u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c076328; /* fmov fr2,@(r0,r15) */
L8c076328: *stop=0x8c076328u;r[0]=4294967240u;goto L8c07632a; /* mov #-0x38,r0 */
L8c07632a: *stop=0x8c07632au;r[0]=r[0]&255u;goto L8c07632c; /* extu.b r0,r0 */
L8c07632c: *stop=0x8c07632cu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c07632e; /* fmov.s @(r0,r15),fr2 */
L8c07632e: *stop=0x8c07632eu;r[0]=4294967240u;goto L8c076330; /* mov #-0x38,r0 */
L8c076330: *stop=0x8c076330u;r[0]=r[0]&255u;goto L8c076332; /* extu.b r0,r0 */
L8c076332: *stop=0x8c076332u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c076334; /* fneg fr2 */
L8c076334: *stop=0x8c076334u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c076336; /* fmov fr2,@(r0,r15) */
L8c076336: *stop=0x8c076336u;r[0]=4294967244u;goto L8c076338; /* mov #-0x34,r0 */
L8c076338: *stop=0x8c076338u;r[0]=r[0]&255u;goto L8c07633a; /* extu.b r0,r0 */
L8c07633a: *stop=0x8c07633au;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c07633c; /* fmov.s @(r0,r15),fr2 */
L8c07633c: *stop=0x8c07633cu;r[0]=4294967244u;goto L8c07633e; /* mov #-0x34,r0 */
L8c07633e: *stop=0x8c07633eu;r[0]=r[0]&255u;goto L8c076340; /* extu.b r0,r0 */
L8c076340: *stop=0x8c076340u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c076342; /* fneg fr2 */
L8c076342: *stop=0x8c076342u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c076344; /* fmov fr2,@(r0,r15) */
L8c076344: *stop=0x8c076344u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}goto L8c076346; /* mov.l @r15,r13 */
L8c076346: *stop=0x8c076346u;r[4]=r[13];goto L8c076348; /* mov r13,r4 */
L8c076348: target=r[9];*pr=0x8c07634cu;*stop=0x8c07634au;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c07634c; /* jsr @r9 */
L8c07634c: *stop=0x8c07634cu;r[0]=4294967236u;goto L8c07634e; /* mov #-0x3c,r0 */
L8c07634e: *stop=0x8c07634eu;fcopy(f,4u,0u);goto L8c076350; /* fmov fr0,fr4 */
L8c076350: *stop=0x8c076350u;r[0]=r[0]&255u;goto L8c076352; /* extu.b r0,r0 */
L8c076352: *stop=0x8c076352u;r[3]=r[14];goto L8c076354; /* mov r14,r3 */
L8c076354: *stop=0x8c076354u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076356; /* fmov.s @(r0,r15),fr3 */
L8c076356: *stop=0x8c076356u;r[0]=4u;goto L8c076358; /* mov #0x4,r0 */
L8c076358: *stop=0x8c076358u;r[3]+=108u;goto L8c07635a; /* add #0x6c,r3 */
L8c07635a: *stop=0x8c07635au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c07635c; /* fmul fr4,fr3 */
L8c07635c: *stop=0x8c07635cu;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c07635e; /* fmov fr3,@(r0,r13) */
L8c07635e: *stop=0x8c07635eu;r[0]=4294967240u;goto L8c076360; /* mov #-0x38,r0 */
L8c076360: *stop=0x8c076360u;r[0]=r[0]&255u;goto L8c076362; /* extu.b r0,r0 */
L8c076362: *stop=0x8c076362u;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076364; /* fmov.s @(r0,r15),fr3 */
L8c076364: *stop=0x8c076364u;r[0]=8u;goto L8c076366; /* mov #0x8,r0 */
L8c076366: *stop=0x8c076366u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c076368; /* fmul fr4,fr3 */
L8c076368: *stop=0x8c076368u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c07636a; /* fmov fr3,@(r0,r13) */
L8c07636a: *stop=0x8c07636au;r[0]=4294967244u;goto L8c07636c; /* mov #-0x34,r0 */
L8c07636c: *stop=0x8c07636cu;r[0]=r[0]&255u;goto L8c07636e; /* extu.b r0,r0 */
L8c07636e: *stop=0x8c07636eu;if(!valid_access(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c076370; /* fmov.s @(r0,r15),fr3 */
L8c076370: *stop=0x8c076370u;r[0]=12u;goto L8c076372; /* mov #0xc,r0 */
L8c076372: *stop=0x8c076372u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c076374; /* fmul fr4,fr3 */
L8c076374: *stop=0x8c076374u;if(!valid_access(r[0]+r[13],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c076376; /* fmov fr3,@(r0,r13) */
L8c076376: *stop=0x8c076376u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c076378; /* mov.l @(0x4,r13),r2 */
L8c076378: *stop=0x8c076378u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c07637a; /* mov.l @(0x8,r13),r1 */
L8c07637a: *stop=0x8c07637au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c07637c; /* mov.l r2,@r3 */
L8c07637c: *stop=0x8c07637cu;if(!valid_access(r[3]+4u,4u))return -3;wr(m,bus,&io_status,r[3]+0x4u,r[1],4);if(io_status)return io_status;goto L8c07637e; /* mov.l r1,@(0x4,r3) */
L8c07637e: *stop=0x8c07637eu;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c076380; /* mov.l @(0xc,r13),r2 */
L8c076380: *stop=0x8c076380u;if(!valid_access(r[3]+8u,4u))return -3;wr(m,bus,&io_status,r[3]+0x8u,r[2],4);if(io_status)return io_status;goto L8c076382; /* mov.l r2,@(0x8,r3) */
L8c076382: *stop=0x8c076382u;r[1]=1u;goto L8c076384; /* mov #0x1,r1 */
L8c076384: *stop=0x8c076384u;r[1]<<=8;goto L8c076386; /* shll8 r1 */
L8c076386: *stop=0x8c076386u;r[15]+=r[1];goto L8c076388; /* add r1,r15 */
L8c076388: *stop=0x8c076388u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c07638a; /* lds.l @r15+,PR */
L8c07638a: *stop=0x8c07638au;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c07638c; /* fmov.s @r15+,fr13 */
L8c07638c: *stop=0x8c07638cu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c07638e; /* fmov.s @r15+,fr14 */
L8c07638e: *stop=0x8c07638eu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c076390; /* fmov.s @r15+,fr15 */
L8c076390: *stop=0x8c076390u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c076392; /* mov.l @r15+,r8 */
L8c076392: *stop=0x8c076392u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c076394; /* mov.l @r15+,r9 */
L8c076394: *stop=0x8c076394u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c076396; /* mov.l @r15+,r10 */
L8c076396: *stop=0x8c076396u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c076398; /* mov.l @r15+,r11 */
L8c076398: *stop=0x8c076398u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c07639a; /* mov.l @r15+,r12 */
L8c07639a: *stop=0x8c07639au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c07639c; /* mov.l @r15+,r13 */
L8c07639c: *stop=0x8c07639cu;{uint32_t return_target=*pr;if(!resume_pc && return_target!=entry_pr)return -3;if(resume_pc && !sa_movie_channel_io_return_allowed(ctx,return_target))return -3;*stop=0x8c07639eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(resume_pc){if(!call)return -2;return call(ctx,return_target,m,f,sr,r,pr);}return 0;} /* rts */
}
SA_STAGE_EXPORT int sa_movement_contact_candidates_8c075f8c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c075fe8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c075fe8u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c076000(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c076000u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c076096(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c076096u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c0760a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0760a4u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c0760ba(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0760bau);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c0760ce(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0760ceu);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c0760d8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0760d8u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c076120(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c076120u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c076140(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c076140u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c0761ca(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0761cau);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c0761e6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0761e6u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c0761ee(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0761eeu);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c07624a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07624au);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c0762de(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0762deu);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c0762f8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c0762f8u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c076306(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c076306u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c076314(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c076314u);}
SA_STAGE_EXPORT int sa_movement_contact_candidates_resume_8c07634c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){return sa_movement_contact_candidates_run_8c075f8c(m,f,sr,r,pr,fpul,macl,ctx,call,stop,gbr,bus,integer,vbr,0x8c07634cu);}
