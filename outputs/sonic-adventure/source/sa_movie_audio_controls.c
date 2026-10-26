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
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_audio_controls_8c6436e8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6436e8;
L8c6436e8: *stop=0x8c6436e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643738u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6436ea; /* mov.l 0x8c643738,r2 */
L8c6436ea: *stop=0x8c6436eau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6436ec; /* mov.l r14,@-r15 */
L8c6436ec: *stop=0x8c6436ecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6436ee; /* sts.l PR,@-r15 */
L8c6436ee: *stop=0x8c6436eeu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6436f0; /* mov.l @r2,r3 */
L8c6436f0: *stop=0x8c6436f0u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6436f2; /* tst r3,r3 */
L8c6436f2: if(!(*sr&1u))goto L8c6436fa;goto L8c6436f4; /* bf 0x8c6436fa */
L8c6436f4: *stop=0x8c6436f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64373cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6436f6; /* mov.l 0x8c64373c,r14 */
L8c6436f6: *stop=0x8c6436f8u;goto L8c643720; /* bra 0x8c643720 */
L8c6436fa: *stop=0x8c6436fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c643740u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6436fc; /* mov.l 0x8c643740,r3 */
L8c6436fc: *stop=0x8c6436fcu;r[6]=7u;goto L8c6436fe; /* mov #0x7,r6 */
L8c6436fe: *stop=0x8c6436feu;{uint32_t loaded=sx(m,bus,&io_status,0x8c643734u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c643700; /* mov.w 0x8c643734,r2 */
L8c643700: *stop=0x8c643700u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c643702; /* mov.l r2,@r3 */
L8c643702: *stop=0x8c643704u;r[5]=0u;goto L8c643710; /* bra 0x8c643710 */
L8c643706: *stop=0x8c643706u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[2]=loaded;}r[4]+=1;goto L8c643708; /* mov.b @r4+,r2 */
L8c643708: *stop=0x8c643708u;r[3]=(uint32_t)(int32_t)(int8_t)r[5];goto L8c64370a; /* exts.b r5,r3 */
L8c64370a: *stop=0x8c64370au;{uint32_t loaded=rd(m,bus,&io_status,0x8c643744u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64370c; /* mov.l 0x8c643744,r0 */
L8c64370c: *stop=0x8c64370cu;r[5]+=1u;goto L8c64370e; /* add #0x1,r5 */
L8c64370e: *stop=0x8c64370eu;if(!valid_access(r[0]+r[3],1u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],1);if(io_status)return io_status;goto L8c643710; /* mov.b r2,@(r0,r3) */
L8c643710: *stop=0x8c643710u;r[3]=(uint32_t)(int32_t)(int8_t)r[5];goto L8c643712; /* exts.b r5,r3 */
L8c643712: *stop=0x8c643712u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[6]);goto L8c643714; /* cmp/ge r6,r3 */
L8c643714: if(!(*sr&1u))goto L8c643706;goto L8c643716; /* bf 0x8c643706 */
L8c643716: *stop=0x8c643716u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643748u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643718; /* mov.l 0x8c643748,r3 */
L8c643718: *stop=0x8c643718u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643740u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64371a; /* mov.l 0x8c643740,r4 */
L8c64371a: target=r[3];*pr=0x8c64371eu;*stop=0x8c64371cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64371e; /* jsr @r3 */
L8c64371e: *stop=0x8c64371eu;r[14]=r[0];goto L8c643720; /* mov r0,r14 */
L8c643720: *stop=0x8c643720u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c643722; /* tst r14,r14 */
L8c643722: if((*sr&1u))goto L8c64372c;goto L8c643724; /* bt 0x8c64372c */
L8c643724: *stop=0x8c643724u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643750u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643726; /* mov.l 0x8c643750,r3 */
L8c643726: *stop=0x8c643726u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64374cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c643728; /* mov.l 0x8c64374c,r4 */
L8c643728: target=r[3];*pr=0x8c64372cu;*stop=0x8c64372au;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64372c; /* jsr @r3 */
L8c64372c: *stop=0x8c64372cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64372e; /* lds.l @r15+,PR */
L8c64372e: *stop=0x8c64372eu;r[0]=r[14];goto L8c643730; /* mov r14,r0 */
L8c643730: *stop=0x8c643730u;if(*pr!=entry_pr)return -3;*stop=0x8c643732u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_audio_controls_8c643754(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c643754;
L8c643754: *stop=0x8c643754u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c643756; /* mov.l r14,@-r15 */
L8c643756: *stop=0x8c643756u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c643758; /* sts.l PR,@-r15 */
L8c643758: *stop=0x8c643758u;r[15]+=4294967292u;goto L8c64375a; /* add #-0x4,r15 */
L8c64375a: *stop=0x8c64375au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6437ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64375c; /* mov.l 0x8c6437cc,r3 */
L8c64375c: *stop=0x8c64375cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[7]=loaded;}goto L8c64375e; /* mov.l @r3,r7 */
L8c64375e: *stop=0x8c64375eu;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c643760; /* tst r7,r7 */
L8c643760: if(!(*sr&1u))goto L8c643768;goto L8c643762; /* bf 0x8c643768 */
L8c643762: *stop=0x8c643762u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6437d0u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c643764; /* mov.l 0x8c6437d0,r14 */
L8c643764: *stop=0x8c643766u;goto L8c6437b0; /* bra 0x8c6437b0 */
L8c643768: *stop=0x8c643768u;r[2]=(uint32_t)(int32_t)(int8_t)r[4];goto L8c64376a; /* exts.b r4,r2 */
L8c64376a: *stop=0x8c64376au;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c64376c; /* cmp/pz r2 */
L8c64376c: take=!(*sr&1u);*stop=0x8c64376eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;if(take)goto L8c643776;goto L8c643770; /* bf/s 0x8c643776 */
L8c643770: *stop=0x8c643770u;r[1]=15u;goto L8c643772; /* mov #0xf,r1 */
L8c643772: *stop=0x8c643772u;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[1]);goto L8c643774; /* cmp/gt r1,r2 */
L8c643774: if(!(*sr&1u))goto L8c64377c;goto L8c643776; /* bf 0x8c64377c */
L8c643776: *stop=0x8c643776u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6437d4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c643778; /* mov.l 0x8c6437d4,r14 */
L8c643778: *stop=0x8c64377au;goto L8c6437b0; /* bra 0x8c6437b0 */
L8c64377c: *stop=0x8c64377cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6437c6u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c64377e; /* mov.w 0x8c6437c6,r0 */
L8c64377e: *stop=0x8c64377eu;r[2]=39u;goto L8c643780; /* mov #0x27,r2 */
L8c643780: *stop=0x8c643780u;r[6]=(uint32_t)(int32_t)(int8_t)r[6];goto L8c643782; /* exts.b r6,r6 */
L8c643782: *stop=0x8c643782u;if(!valid_access(r[0]+r[7],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[7]+r[0],1);if(io_status)return io_status;r[3]=loaded;}goto L8c643784; /* mov.b @(r0,r7),r3 */
L8c643784: *stop=0x8c643784u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]);goto L8c643786; /* cmp/ge r2,r3 */
L8c643786: take=(*sr&1u);*stop=0x8c643788u;r[5]=(uint32_t)(int32_t)(int8_t)r[5];if(take)goto L8c643790;goto L8c64378a; /* bt/s 0x8c643790 */
L8c64378a: *stop=0x8c64378au;*sr=(*sr&~1u)|(r[6]&1u);r[6]=(r[6]>>1)|(r[6]&0x80000000u);goto L8c64378c; /* shar r6 */
L8c64378c: *stop=0x8c64378cu;*sr=(*sr&~1u)|(r[6]&1u);r[6]=(r[6]>>1)|(r[6]&0x80000000u);goto L8c64378e; /* shar r6 */
L8c64378e: *stop=0x8c64378eu;*sr=(*sr&~1u)|(r[6]&1u);r[6]=(r[6]>>1)|(r[6]&0x80000000u);goto L8c643790; /* shar r6 */
L8c643790: *stop=0x8c643790u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6437dcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c643792; /* mov.l 0x8c6437dc,r1 */
L8c643792: *stop=0x8c643792u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6437c8u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c643794; /* mov.w 0x8c6437c8,r3 */
L8c643794: *stop=0x8c643794u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6437d8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643796; /* mov.l 0x8c6437d8,r2 */
L8c643796: *stop=0x8c643796u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c643798; /* mov.l r3,@r2 */
L8c643798: *stop=0x8c643798u;r[2]+=6u;goto L8c64379a; /* add #0x6,r2 */
L8c64379a: *stop=0x8c64379au;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[4],1);if(io_status)return io_status;goto L8c64379c; /* mov.b r4,@r1 */
L8c64379c: *stop=0x8c64379cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c6437cau,2);if(io_status)return io_status;r[4]=loaded;}goto L8c64379e; /* mov.w 0x8c6437ca,r4 */
L8c64379e: *stop=0x8c64379eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6437e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6437a0; /* mov.l 0x8c6437e0,r3 */
L8c6437a0: *stop=0x8c6437a0u;r[5]+=r[4];goto L8c6437a2; /* add r4,r5 */
L8c6437a2: *stop=0x8c6437a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6437e4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6437a4; /* mov.l 0x8c6437e4,r1 */
L8c6437a4: *stop=0x8c6437a4u;r[6]+=r[4];goto L8c6437a6; /* add r4,r6 */
L8c6437a6: *stop=0x8c6437a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6437d8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6437a8; /* mov.l 0x8c6437d8,r4 */
L8c6437a8: *stop=0x8c6437a8u;if(!valid_access(r[3],1u))return -3;wr(m,bus,&io_status,r[3],r[6],1);if(io_status)return io_status;goto L8c6437aa; /* mov.b r6,@r3 */
L8c6437aa: target=r[1];*pr=0x8c6437aeu;*stop=0x8c6437acu;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[5],1);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6437ae; /* jsr @r1 */
L8c6437ae: *stop=0x8c6437aeu;r[14]=r[0];goto L8c6437b0; /* mov r0,r14 */
L8c6437b0: *stop=0x8c6437b0u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6437b2; /* tst r14,r14 */
L8c6437b2: if((*sr&1u))goto L8c6437bc;goto L8c6437b4; /* bt 0x8c6437bc */
L8c6437b4: *stop=0x8c6437b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6437ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6437b6; /* mov.l 0x8c6437ec,r2 */
L8c6437b6: *stop=0x8c6437b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6437e8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6437b8; /* mov.l 0x8c6437e8,r4 */
L8c6437b8: target=r[2];*pr=0x8c6437bcu;*stop=0x8c6437bau;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6437bc; /* jsr @r2 */
L8c6437bc: *stop=0x8c6437bcu;r[0]=r[14];goto L8c6437be; /* mov r14,r0 */
L8c6437be: *stop=0x8c6437beu;r[15]+=4u;goto L8c6437c0; /* add #0x4,r15 */
L8c6437c0: *stop=0x8c6437c0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6437c2; /* lds.l @r15+,PR */
L8c6437c2: *stop=0x8c6437c2u;if(*pr!=entry_pr)return -3;*stop=0x8c6437c4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_audio_controls_8c6437f0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6437f0;
L8c6437f0: *stop=0x8c6437f0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6437f2; /* mov.l r14,@-r15 */
L8c6437f2: *stop=0x8c6437f2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6437f4; /* sts.l PR,@-r15 */
L8c6437f4: *stop=0x8c6437f4u;r[15]+=4294967292u;goto L8c6437f6; /* add #-0x4,r15 */
L8c6437f6: *stop=0x8c6437f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64385cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6437f8; /* mov.l 0x8c64385c,r2 */
L8c6437f8: *stop=0x8c6437f8u;if(!valid_access(r[15],1u))return -3;wr(m,bus,&io_status,r[15],r[5],1);if(io_status)return io_status;goto L8c6437fa; /* mov.b r5,@r15 */
L8c6437fa: *stop=0x8c6437fau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6437fc; /* mov.l @r2,r3 */
L8c6437fc: *stop=0x8c6437fcu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6437fe; /* tst r3,r3 */
L8c6437fe: if(!(*sr&1u))goto L8c643806;goto L8c643800; /* bf 0x8c643806 */
L8c643800: *stop=0x8c643800u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643860u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c643802; /* mov.l 0x8c643860,r14 */
L8c643802: *stop=0x8c643804u;goto L8c643842; /* bra 0x8c643842 */
L8c643806: *stop=0x8c643806u;r[3]=(uint32_t)(int32_t)(int8_t)r[4];goto L8c643808; /* exts.b r4,r3 */
L8c643808: *stop=0x8c643808u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c64380a; /* cmp/pz r3 */
L8c64380a: if(!(*sr&1u))goto L8c643812;goto L8c64380c; /* bf 0x8c643812 */
L8c64380c: *stop=0x8c64380cu;if(!valid_access(r[15],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;r[3]=loaded;}goto L8c64380e; /* mov.b @r15,r3 */
L8c64380e: *stop=0x8c64380eu;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c643810; /* cmp/pz r3 */
L8c643810: if((*sr&1u))goto L8c643818;goto L8c643812; /* bt 0x8c643818 */
L8c643812: *stop=0x8c643812u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643864u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c643814; /* mov.l 0x8c643864,r14 */
L8c643814: *stop=0x8c643816u;goto L8c643842; /* bra 0x8c643842 */
L8c643818: *stop=0x8c643818u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643868u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64381a; /* mov.l 0x8c643868,r2 */
L8c64381a: *stop=0x8c64381au;{uint32_t loaded=sx(m,bus,&io_status,0x8c643858u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c64381c; /* mov.w 0x8c643858,r3 */
L8c64381c: *stop=0x8c64381cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c64386cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64381e; /* mov.l 0x8c64386c,r1 */
L8c64381e: *stop=0x8c64381eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c643820; /* mov.l r3,@r2 */
L8c643820: *stop=0x8c643820u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643870u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643822; /* mov.l 0x8c643870,r3 */
L8c643822: *stop=0x8c643822u;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[4],1);if(io_status)return io_status;goto L8c643824; /* mov.b r4,@r1 */
L8c643824: target=r[3];*pr=0x8c643828u;*stop=0x8c643826u;r[4]=r[2];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643828; /* jsr @r3 */
L8c643828: *stop=0x8c643828u;r[14]=r[0];goto L8c64382a; /* mov r0,r14 */
L8c64382a: *stop=0x8c64382au;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c64382c; /* tst r14,r14 */
L8c64382c: if(!(*sr&1u))goto L8c643842;goto L8c64382e; /* bf 0x8c643842 */
L8c64382e: *stop=0x8c64382eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c643868u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c643830; /* mov.l 0x8c643868,r3 */
L8c643830: *stop=0x8c643830u;{uint32_t loaded=sx(m,bus,&io_status,0x8c64385au,2);if(io_status)return io_status;r[2]=loaded;}goto L8c643832; /* mov.w 0x8c64385a,r2 */
L8c643832: *stop=0x8c643832u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64386cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c643834; /* mov.l 0x8c64386c,r1 */
L8c643834: *stop=0x8c643834u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c643836; /* mov.l r2,@r3 */
L8c643836: *stop=0x8c643836u;if(!valid_access(r[15],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15],1);if(io_status)return io_status;r[2]=loaded;}goto L8c643838; /* mov.b @r15,r2 */
L8c643838: *stop=0x8c643838u;if(!valid_access(r[1],1u))return -3;wr(m,bus,&io_status,r[1],r[2],1);if(io_status)return io_status;goto L8c64383a; /* mov.b r2,@r1 */
L8c64383a: *stop=0x8c64383au;{uint32_t loaded=rd(m,bus,&io_status,0x8c643870u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64383c; /* mov.l 0x8c643870,r2 */
L8c64383c: target=r[2];*pr=0x8c643840u;*stop=0x8c64383eu;r[4]=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c643840; /* jsr @r2 */
L8c643840: *stop=0x8c643840u;r[14]=r[0];goto L8c643842; /* mov r0,r14 */
L8c643842: *stop=0x8c643842u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c643844; /* tst r14,r14 */
L8c643844: if((*sr&1u))goto L8c64384e;goto L8c643846; /* bt 0x8c64384e */
L8c643846: *stop=0x8c643846u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643878u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c643848; /* mov.l 0x8c643878,r2 */
L8c643848: *stop=0x8c643848u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643874u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c64384a; /* mov.l 0x8c643874,r4 */
L8c64384a: target=r[2];*pr=0x8c64384eu;*stop=0x8c64384cu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64384e; /* jsr @r2 */
L8c64384e: *stop=0x8c64384eu;r[0]=r[14];goto L8c643850; /* mov r14,r0 */
L8c643850: *stop=0x8c643850u;r[15]+=4u;goto L8c643852; /* add #0x4,r15 */
L8c643852: *stop=0x8c643852u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c643854; /* lds.l @r15+,PR */
L8c643854: *stop=0x8c643854u;if(*pr!=entry_pr)return -3;*stop=0x8c643856u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
