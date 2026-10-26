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
SA_STAGE_EXPORT int sa_movie_audio_mode_8c64387c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64387c;
L8c64387c: *stop=0x8c64387cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6438dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64387e; /* mov.l 0x8c6438dc,r2 */
L8c64387e: *stop=0x8c64387eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c643880; /* mov.l r14,@-r15 */
L8c643880: *stop=0x8c643880u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c643882; /* sts.l PR,@-r15 */
L8c643882: *stop=0x8c643882u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c643884; /* mov.l @r2,r3 */
L8c643884: *stop=0x8c643884u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c643886; /* tst r3,r3 */
L8c643886: if(!(*sr&1u))goto L8c64388e;goto L8c643888; /* bf 0x8c64388e */
L8c643888: *stop=0x8c643888u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6438e0u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c64388a; /* mov.l 0x8c6438e0,r14 */
L8c64388a: *stop=0x8c64388cu;goto L8c6438c4; /* bra 0x8c6438c4 */
L8c64388e: *stop=0x8c64388eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6438ecu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c643890; /* mov.l 0x8c6438ec,r1 */
L8c643890: *stop=0x8c643890u;r[0]=r[4];goto L8c643892; /* mov r4,r0 */
L8c643892: *stop=0x8c643892u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6438e8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c643894; /* mov.l 0x8c6438e8,r5 */
L8c643894: *stop=0x8c643894u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6438d8u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c643896; /* mov.w 0x8c6438d8,r2 */
L8c643896: *stop=0x8c643896u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c643898; /* cmp/eq r1,r0 */
L8c643898: *stop=0x8c643898u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6438e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64389a; /* mov.l 0x8c6438e4,r3 */
L8c64389a: take=(*sr&1u);*stop=0x8c64389cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;if(take)goto L8c6438a8;goto L8c64389e; /* bt/s 0x8c6438a8 */
L8c64389e: *stop=0x8c64389eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6438f0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6438a0; /* mov.l 0x8c6438f0,r1 */
L8c6438a0: *stop=0x8c6438a0u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c6438a2; /* cmp/eq r1,r0 */
L8c6438a2: if((*sr&1u))goto L8c6438ae;goto L8c6438a4; /* bt 0x8c6438ae */
L8c6438a4: *stop=0x8c6438a6u;goto L8c6438b4; /* bra 0x8c6438b4 */
L8c6438a8: *stop=0x8c6438a8u;{uint32_t loaded=sx(m,bus,&io_status,0x8c6438dau,2);if(io_status)return io_status;r[2]=loaded;}goto L8c6438aa; /* mov.w 0x8c6438da,r2 */
L8c6438aa: *stop=0x8c6438acu;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[2],1);if(io_status)return io_status;goto L8c6438ba; /* bra 0x8c6438ba */
L8c6438ae: *stop=0x8c6438aeu;r[0]=0u;goto L8c6438b0; /* mov #0x0,r0 */
L8c6438b0: *stop=0x8c6438b2u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[0],1);if(io_status)return io_status;goto L8c6438ba; /* bra 0x8c6438ba */
L8c6438b4: *stop=0x8c6438b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6438f4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6438b6; /* mov.l 0x8c6438f4,r14 */
L8c6438b6: *stop=0x8c6438b8u;goto L8c6438c4; /* bra 0x8c6438c4 */
L8c6438ba: *stop=0x8c6438bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6438f8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6438bc; /* mov.l 0x8c6438f8,r3 */
L8c6438bc: *stop=0x8c6438bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6438e4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6438be; /* mov.l 0x8c6438e4,r4 */
L8c6438be: target=r[3];*pr=0x8c6438c2u;*stop=0x8c6438c0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6438c2; /* jsr @r3 */
L8c6438c2: *stop=0x8c6438c2u;r[14]=r[0];goto L8c6438c4; /* mov r0,r14 */
L8c6438c4: *stop=0x8c6438c4u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6438c6; /* tst r14,r14 */
L8c6438c6: if((*sr&1u))goto L8c6438d0;goto L8c6438c8; /* bt 0x8c6438d0 */
L8c6438c8: *stop=0x8c6438c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c643900u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6438ca; /* mov.l 0x8c643900,r3 */
L8c6438ca: *stop=0x8c6438cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6438fcu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6438cc; /* mov.l 0x8c6438fc,r4 */
L8c6438cc: target=r[3];*pr=0x8c6438d0u;*stop=0x8c6438ceu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6438d0; /* jsr @r3 */
L8c6438d0: *stop=0x8c6438d0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6438d2; /* lds.l @r15+,PR */
L8c6438d2: *stop=0x8c6438d2u;r[0]=r[14];goto L8c6438d4; /* mov r14,r0 */
L8c6438d4: *stop=0x8c6438d4u;if(*pr!=entry_pr)return -3;*stop=0x8c6438d6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_audio_mode_8c65a464(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65a464;
L8c65a464: *stop=0x8c65a464u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65a466; /* mov.l r14,@-r15 */
L8c65a466: *stop=0x8c65a466u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65a468; /* sts.l PR,@-r15 */
L8c65a468: *stop=0x8c65a468u;r[15]+=4294967292u;goto L8c65a46a; /* add #-0x4,r15 */
L8c65a46a: *stop=0x8c65a46au;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c65a46c; /* stc SR,r0 */
L8c65a46c: *stop=0x8c65a46cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a4f8u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65a46e; /* mov.w 0x8c65a4f8,r3 */
L8c65a46e: *stop=0x8c65a46eu;r[14]=0u;goto L8c65a470; /* mov #0x0,r14 */
L8c65a470: *stop=0x8c65a470u;r[0]>>=2;goto L8c65a472; /* shlr2 r0 */
L8c65a472: *stop=0x8c65a472u;r[0]>>=2;goto L8c65a474; /* shlr2 r0 */
L8c65a474: *stop=0x8c65a474u;r[0]&=0xfu;goto L8c65a476; /* and #0xf,r0 */
L8c65a476: *stop=0x8c65a476u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c65a478; /* mov.l r0,@r15 */
L8c65a478: *stop=0x8c65a478u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c65a47a; /* stc SR,r0 */
L8c65a47a: *stop=0x8c65a47au;r[0]&=r[3];goto L8c65a47c; /* and r3,r0 */
L8c65a47c: *stop=0x8c65a47cu;r[0]|=0xf0u;goto L8c65a47e; /* or #0xf0,r0 */
L8c65a47e: *stop=0x8c65a47eu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65a480; /* ldc r0,SR */
L8c65a480: *stop=0x8c65a480u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a500u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a482; /* mov.l 0x8c65a500,r2 */
L8c65a482: *stop=0x8c65a482u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c65a484; /* mov.l @r2,r5 */
L8c65a484: *stop=0x8c65a484u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c65a486; /* tst r5,r5 */
L8c65a486: if(!(*sr&1u))goto L8c65a48e;goto L8c65a488; /* bf 0x8c65a48e */
L8c65a488: *stop=0x8c65a488u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a504u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a48a; /* mov.l 0x8c65a504,r14 */
L8c65a48a: *stop=0x8c65a48cu;goto L8c65a4d0; /* bra 0x8c65a4d0 */
L8c65a48e: *stop=0x8c65a48eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a4fau,2);if(io_status)return io_status;r[2]=loaded;}goto L8c65a490; /* mov.w 0x8c65a4fa,r2 */
L8c65a490: *stop=0x8c65a490u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a4fcu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c65a492; /* mov.w 0x8c65a4fc,r3 */
L8c65a492: *stop=0x8c65a492u;r[5]+=r[2];goto L8c65a494; /* add r2,r5 */
L8c65a494: *stop=0x8c65a494u;if(!valid_access(r[5]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x14u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c65a496; /* mov.l @(0x14,r5),r6 */
L8c65a496: *stop=0x8c65a496u;*sr=(*sr&~1u)|(r[3]==r[6]);goto L8c65a498; /* cmp/eq r3,r6 */
L8c65a498: take=!(*sr&1u);*stop=0x8c65a49au;if(!valid_access(r[5]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x10u,4);if(io_status)return io_status;r[7]=loaded;}if(take)goto L8c65a4a2;goto L8c65a49c; /* bf/s 0x8c65a4a2 */
L8c65a49c: *stop=0x8c65a49cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a508u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c65a49e; /* mov.l 0x8c65a508,r14 */
L8c65a49e: *stop=0x8c65a4a0u;goto L8c65a4d0; /* bra 0x8c65a4d0 */
L8c65a4a2: *stop=0x8c65a4a2u;if(!valid_access(r[5]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x18u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c65a4a4; /* mov.l @(0x18,r5),r6 */
L8c65a4a4: *stop=0x8c65a4a4u;r[3]=28u;goto L8c65a4a6; /* mov #0x1c,r3 */
L8c65a4a6: *stop=0x8c65a4a6u;*macl=r[3]*r[6];goto L8c65a4a8; /* mul.l r3,r6 */
L8c65a4a8: *stop=0x8c65a4a8u;r[6]+=1u;goto L8c65a4aa; /* add #0x1,r6 */
L8c65a4aa: *stop=0x8c65a4aau;if(!valid_access(r[5]+24u,4u))return -3;wr(m,bus,&io_status,r[5]+0x18u,r[6],4);if(io_status)return io_status;goto L8c65a4ac; /* mov.l r6,@(0x18,r5) */
L8c65a4ac: *stop=0x8c65a4acu;if(!valid_access(r[5]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65a4ae; /* mov.l @(0x18,r5),r1 */
L8c65a4ae: *stop=0x8c65a4aeu;r[6]=*macl;goto L8c65a4b0; /* sts MACL,r6 */
L8c65a4b0: *stop=0x8c65a4b0u;if(!valid_access(r[5]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a4b2; /* mov.l @(0xc,r5),r2 */
L8c65a4b2: *stop=0x8c65a4b2u;*sr=(*sr&~1u)|(r[7]==r[1]);goto L8c65a4b4; /* cmp/eq r7,r1 */
L8c65a4b4: *stop=0x8c65a4b4u;r[6]+=r[2];goto L8c65a4b6; /* add r2,r6 */
L8c65a4b6: take=!(*sr&1u);*stop=0x8c65a4b8u;r[3]=1u;if(take)goto L8c65a4be;goto L8c65a4ba; /* bf/s 0x8c65a4be */
L8c65a4ba: *stop=0x8c65a4bau;r[0]=0u;goto L8c65a4bc; /* mov #0x0,r0 */
L8c65a4bc: *stop=0x8c65a4bcu;if(!valid_access(r[5]+24u,4u))return -3;wr(m,bus,&io_status,r[5]+0x18u,r[0],4);if(io_status)return io_status;goto L8c65a4be; /* mov.l r0,@(0x18,r5) */
L8c65a4be: *stop=0x8c65a4beu;if(!valid_access(r[6]+20u,4u))return -3;wr(m,bus,&io_status,r[6]+0x14u,r[3],4);if(io_status)return io_status;goto L8c65a4c0; /* mov.l r3,@(0x14,r6) */
L8c65a4c0: *stop=0x8c65a4c0u;r[2]=r[4];goto L8c65a4c2; /* mov r4,r2 */
L8c65a4c2: *stop=0x8c65a4c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a50cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a4c4; /* mov.l 0x8c65a50c,r3 */
L8c65a4c4: *stop=0x8c65a4c4u;r[1]=r[6];goto L8c65a4c6; /* mov r6,r1 */
L8c65a4c6: target=r[3];*pr=0x8c65a4cau;*stop=0x8c65a4c8u;r[0]=20u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a4ca; /* jsr @r3 */
L8c65a4ca: *stop=0x8c65a4cau;if(!valid_access(r[5]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65a4cc; /* mov.l @(0x14,r5),r2 */
L8c65a4cc: *stop=0x8c65a4ccu;r[2]+=1u;goto L8c65a4ce; /* add #0x1,r2 */
L8c65a4ce: *stop=0x8c65a4ceu;if(!valid_access(r[5]+20u,4u))return -3;wr(m,bus,&io_status,r[5]+0x14u,r[2],4);if(io_status)return io_status;goto L8c65a4d0; /* mov.l r2,@(0x14,r5) */
L8c65a4d0: *stop=0x8c65a4d0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65a4d2; /* mov.l @r15,r0 */
L8c65a4d2: *stop=0x8c65a4d2u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c65a4d4; /* stc SR,r3 */
L8c65a4d4: *stop=0x8c65a4d4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c65a4f8u,2);if(io_status)return io_status;r[1]=loaded;}goto L8c65a4d6; /* mov.w 0x8c65a4f8,r1 */
L8c65a4d6: *stop=0x8c65a4d6u;r[0]&=0xfu;goto L8c65a4d8; /* and #0xf,r0 */
L8c65a4d8: *stop=0x8c65a4d8u;r[0]<<=2;goto L8c65a4da; /* shll2 r0 */
L8c65a4da: *stop=0x8c65a4dau;r[3]&=r[1];goto L8c65a4dc; /* and r1,r3 */
L8c65a4dc: *stop=0x8c65a4dcu;r[0]<<=2;goto L8c65a4de; /* shll2 r0 */
L8c65a4de: *stop=0x8c65a4deu;r[0]|=r[3];goto L8c65a4e0; /* or r3,r0 */
L8c65a4e0: *stop=0x8c65a4e0u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c65a4e2; /* ldc r0,SR */
L8c65a4e2: *stop=0x8c65a4e2u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c65a4e4; /* tst r14,r14 */
L8c65a4e4: if((*sr&1u))goto L8c65a4ee;goto L8c65a4e6; /* bt 0x8c65a4ee */
L8c65a4e6: *stop=0x8c65a4e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a514u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65a4e8; /* mov.l 0x8c65a514,r3 */
L8c65a4e8: *stop=0x8c65a4e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65a510u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c65a4ea; /* mov.l 0x8c65a510,r4 */
L8c65a4ea: target=r[3];*pr=0x8c65a4eeu;*stop=0x8c65a4ecu;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65a4ee; /* jsr @r3 */
L8c65a4ee: *stop=0x8c65a4eeu;r[0]=r[14];goto L8c65a4f0; /* mov r14,r0 */
L8c65a4f0: *stop=0x8c65a4f0u;r[15]+=4u;goto L8c65a4f2; /* add #0x4,r15 */
L8c65a4f2: *stop=0x8c65a4f2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65a4f4; /* lds.l @r15+,PR */
L8c65a4f4: *stop=0x8c65a4f4u;if(*pr!=entry_pr)return -3;*stop=0x8c65a4f6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_audio_mode_8c10cd82(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10cd82;
L8c10cd82: *stop=0x8c10cd82u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10cd84; /* mov.l @(0xc,r2),r0 */
L8c10cd84: *stop=0x8c10cd84u;if(!valid_access(r[1]+16u,4u))return -3;wr(m,bus,&io_status,r[1]+0x10u,r[3],4);if(io_status)return io_status;goto L8c10cd86; /* mov.l r3,@(0x10,r1) */
L8c10cd86: *stop=0x8c10cd86u;if(!valid_access(r[2]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c10cd88; /* mov.l @(0x8,r2),r3 */
L8c10cd88: *stop=0x8c10cd88u;if(!valid_access(r[1]+12u,4u))return -3;wr(m,bus,&io_status,r[1]+0xcu,r[0],4);if(io_status)return io_status;goto L8c10cd8a; /* mov.l r0,@(0xc,r1) */
L8c10cd8a: *stop=0x8c10cd8au;if(!valid_access(r[2]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10cd8c; /* mov.l @(0x4,r2),r0 */
L8c10cd8c: *stop=0x8c10cd8cu;if(!valid_access(r[1]+8u,4u))return -3;wr(m,bus,&io_status,r[1]+0x8u,r[3],4);if(io_status)return io_status;goto L8c10cd8e; /* mov.l r3,@(0x8,r1) */
L8c10cd8e: *stop=0x8c10cd8eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c10cd90; /* mov.l @r2,r3 */
L8c10cd90: *stop=0x8c10cd90u;if(!valid_access(r[1]+4u,4u))return -3;wr(m,bus,&io_status,r[1]+0x4u,r[0],4);if(io_status)return io_status;goto L8c10cd92; /* mov.l r0,@(0x4,r1) */
L8c10cd92: *stop=0x8c10cd92u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c10cd94; /* mov.l r3,@r1 */
L8c10cd94: *stop=0x8c10cd94u;if(*pr!=entry_pr)return -3;*stop=0x8c10cd96u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;return 0; /* rts */
}
