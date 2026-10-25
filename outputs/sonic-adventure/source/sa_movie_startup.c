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
 if(n==4u && a==0xff00001cu)return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f6800u && (a&0x1fffffffu)<0x005f7d00u)return 1;
 if(n==4u && !(a&31u) && a>=0xf4000000u && a<0xf4004000u)return 1;
 if(n==2u && (a==0xffd00000u||a==0xffd00004u||a==0xffd00008u||a==0xffd0000cu||a==0xffd80010u))return 1;
 if(n==1u && (a==0xffd80000u||a==0xffd80004u))return 1;
 if(n==4u && (a==0xffd80008u||a==0xffd8000cu))return 1;
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_movie_startup_8c010180(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c010180;
L8c010180: *stop=0x8c010180u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c010182; /* mov.l r14,@-r15 */
L8c010182: *stop=0x8c010182u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c010184; /* mov.l r13,@-r15 */
L8c010184: *stop=0x8c010184u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c010186; /* mov.l r12,@-r15 */
L8c010186: *stop=0x8c010186u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c010188; /* mov.l r11,@-r15 */
L8c010188: *stop=0x8c010188u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c01018a; /* mov.l r10,@-r15 */
L8c01018a: *stop=0x8c01018au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c01018c; /* mov.l r9,@-r15 */
L8c01018c: *stop=0x8c01018cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c01018e; /* sts.l PR,@-r15 */
L8c01018e: *stop=0x8c01018eu;r[15]+=4294967284u;goto L8c010190; /* add #-0xc,r15 */
L8c010190: *stop=0x8c010190u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c010192; /* stc SR,r0 */
L8c010192: *stop=0x8c010192u;r[3]=4294967295u;goto L8c010194; /* mov #-0x1,r3 */
L8c010194: *stop=0x8c010194u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[4],4);if(io_status)return io_status;goto L8c010196; /* mov.l r4,@(0x4,r15) */
L8c010196: *stop=0x8c010196u;r[3]<<=8;goto L8c010198; /* shll8 r3 */
L8c010198: *stop=0x8c010198u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c01019a; /* mov.l r5,@r15 */
L8c01019a: *stop=0x8c01019au;r[3]+=15u;goto L8c01019c; /* add #0xf,r3 */
L8c01019c: *stop=0x8c01019cu;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c01019e; /* mov.l r6,@(0x8,r15) */
L8c01019e: *stop=0x8c01019eu;r[0]&=r[3];goto L8c0101a0; /* and r3,r0 */
L8c0101a0: *stop=0x8c0101a0u;r[0]|=0xf0u;goto L8c0101a2; /* or #0xf0,r0 */
L8c0101a2: *stop=0x8c0101a2u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c0101a4; /* ldc r0,SR */
L8c0101a4: *stop=0x8c0101a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102a0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0101a6; /* mov.l 0x8c0102a0,r2 */
L8c0101a6: target=r[2];*pr=0x8c0101aau;*stop=0x8c0101a8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0101aa; /* jsr @r2 */
L8c0101aa: *stop=0x8c0101aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102a4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0101ac; /* mov.l 0x8c0102a4,r1 */
L8c0101ac: target=r[1];*pr=0x8c0101b0u;*stop=0x8c0101aeu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0101b0; /* jsr @r1 */
L8c0101b0: *stop=0x8c0101b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0101b2; /* mov.l 0x8c0102b0,r2 */
L8c0101b2: *stop=0x8c0101b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0101b4; /* mov.l 0x8c0102ac,r3 */
L8c0101b4: *stop=0x8c0101b4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}goto L8c0101b6; /* mov.l @r2,r4 */
L8c0101b6: *stop=0x8c0101b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102b4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c0101b8; /* mov.l 0x8c0102b4,r1 */
L8c0101b8: *stop=0x8c0101b8u;r[4]&=r[3];goto L8c0101ba; /* and r3,r4 */
L8c0101ba: *stop=0x8c0101bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102b8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0101bc; /* mov.l 0x8c0102b8,r2 */
L8c0101bc: *stop=0x8c0101bcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102bcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0101be; /* mov.l 0x8c0102bc,r3 */
L8c0101be: *stop=0x8c0101beu;r[4]|=r[1];goto L8c0101c0; /* or r1,r4 */
L8c0101c0: *stop=0x8c0101c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102a8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0101c2; /* mov.l 0x8c0102a8,r5 */
L8c0101c2: target=r[3];*pr=0x8c0101c6u;*stop=0x8c0101c4u;r[4]+=r[2];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0101c6; /* jsr @r3 */
L8c0101c6: *stop=0x8c0101c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0101c8; /* mov.l 0x8c0102c0,r3 */
L8c0101c8: *stop=0x8c0101c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c0101ca; /* mov.l @r15,r5 */
L8c0101ca: *stop=0x8c0101cau;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c0101cc; /* mov.l @(0x8,r15),r6 */
L8c0101cc: target=r[3];*pr=0x8c0101d0u;*stop=0x8c0101ceu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0101d0; /* jsr @r3 */
L8c0101d0: *stop=0x8c0101d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102c4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0101d2; /* mov.l 0x8c0102c4,r2 */
L8c0101d2: target=r[2];*pr=0x8c0101d6u;*stop=0x8c0101d4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0101d6; /* jsr @r2 */
L8c0101d6: *stop=0x8c0101d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102d0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c0101d8; /* mov.l 0x8c0102d0,r3 */
L8c0101d8: *stop=0x8c0101d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102ccu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c0101da; /* mov.l 0x8c0102cc,r5 */
L8c0101da: *stop=0x8c0101dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102c8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c0101dc; /* mov.l 0x8c0102c8,r6 */
L8c0101dc: target=r[3];*pr=0x8c0101e0u;*stop=0x8c0101deu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0101e0; /* jsr @r3 */
L8c0101e0: *stop=0x8c0101e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102d4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c0101e2; /* mov.l 0x8c0102d4,r2 */
L8c0101e2: target=r[2];*pr=0x8c0101e6u;*stop=0x8c0101e4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0101e6; /* jsr @r2 */
L8c0101e6: *stop=0x8c0101e6u;r[3]=4294967295u;goto L8c0101e8; /* mov #-0x1,r3 */
L8c0101e8: *stop=0x8c0101e8u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c0101ea; /* stc SR,r0 */
L8c0101ea: *stop=0x8c0101eau;r[3]<<=8;goto L8c0101ec; /* shll8 r3 */
L8c0101ec: *stop=0x8c0101ecu;r[3]+=15u;goto L8c0101ee; /* add #0xf,r3 */
L8c0101ee: *stop=0x8c0101eeu;r[0]&=r[3];goto L8c0101f0; /* and r3,r0 */
L8c0101f0: *stop=0x8c0101f0u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c0101f2; /* ldc r0,SR */
L8c0101f2: *stop=0x8c0101f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102dcu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c0101f4; /* mov.l 0x8c0102dc,r11 */
L8c0101f4: *stop=0x8c0101f4u;r[4]=4294967264u;goto L8c0101f6; /* mov #-0x20,r4 */
L8c0101f6: *stop=0x8c0101f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102d8u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c0101f8; /* mov.l 0x8c0102d8,r9 */
L8c0101f8: *stop=0x8c0101f8u;r[12]=8u;goto L8c0101fa; /* mov #0x8,r12 */
L8c0101fa: *stop=0x8c0101fau;r[12]<<=8;goto L8c0101fc; /* shll8 r12 */
L8c0101fc: *stop=0x8c0101fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102e0u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c0101fe; /* mov.l 0x8c0102e0,r10 */
L8c0101fe: *stop=0x8c0101feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102e4u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c010200; /* mov.l 0x8c0102e4,r13 */
L8c010200: *stop=0x8c010200u;r[9]&=r[4];goto L8c010202; /* and r4,r9 */
L8c010202: *stop=0x8c010202u;r[11]&=r[4];goto L8c010204; /* and r4,r11 */
L8c010204: *stop=0x8c010204u;r[12]+=52u;goto L8c010206; /* add #0x34,r12 */
L8c010206: *stop=0x8c010206u;r[9]+=32u;goto L8c010208; /* add #0x20,r9 */
L8c010208: *stop=0x8c010208u;r[11]+=32u;goto L8c01020a; /* add #0x20,r11 */
L8c01020a: *stop=0x8c01020au;r[14]=8u;goto L8c01020c; /* mov #0x8,r14 */
L8c01020c: *stop=0x8c01020cu;r[5]=r[9];goto L8c01020e; /* mov r9,r5 */
L8c01020e: *stop=0x8c01020eu;r[6]=r[12];goto L8c010210; /* mov r12,r6 */
L8c010210: *stop=0x8c010210u;r[7]=r[11];goto L8c010212; /* mov r11,r7 */
L8c010212: target=r[10];*pr=0x8c010216u;*stop=0x8c010214u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c010216; /* jsr @r10 */
L8c010216: *stop=0x8c010216u;*sr=(*sr&~1u)|(r[0]==0xffffffe9u);goto L8c010218; /* cmp/eq #-0x17,r0 */
L8c010218: take=(*sr&1u);*stop=0x8c01021au;r[4]=r[0];if(take)goto L8c010222;goto L8c01021c; /* bt/s 0x8c010222 */
L8c01021c: *stop=0x8c01021cu;r[0]=r[4];goto L8c01021e; /* mov r4,r0 */
L8c01021e: *stop=0x8c01021eu;*sr=(*sr&~1u)|(r[0]==0xffffffdfu);goto L8c010220; /* cmp/eq #-0x21,r0 */
L8c010220: if(!(*sr&1u))goto L8c01022e;goto L8c010222; /* bf 0x8c01022e */
L8c010222: target=0x8c01025au;*pr=0x8c010226u;*stop=0x8c010224u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c010226; /* bsr 0x8c01025a */
L8c010226: target=r[13];*pr=0x8c01022au;*stop=0x8c010228u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01022a; /* jsr @r13 */
L8c01022a: *stop=0x8c01022cu;goto L8c010232; /* bra 0x8c010232 */
L8c01022e: *stop=0x8c01022eu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c010230; /* tst r4,r4 */
L8c010230: if((*sr&1u))goto L8c010238;goto L8c010232; /* bt 0x8c010238 */
L8c010232: *stop=0x8c010232u;r[14]+=4294967295u;goto L8c010234; /* add #-0x1,r14 */
L8c010234: *stop=0x8c010234u;*sr=(*sr&~1u)|((int32_t)r[14]>0);goto L8c010236; /* cmp/pl r14 */
L8c010236: if((*sr&1u))goto L8c01020c;goto L8c010238; /* bt 0x8c01020c */
L8c010238: *stop=0x8c010238u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c01023a; /* tst r14,r14 */
L8c01023a: if(!(*sr&1u))goto L8c010244;goto L8c01023c; /* bf 0x8c010244 */
L8c01023c: target=0x8c01025au;*pr=0x8c010240u;*stop=0x8c01023eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c010240; /* bsr 0x8c01025a */
L8c010240: target=r[13];*pr=0x8c010244u;*stop=0x8c010242u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c010244; /* jsr @r13 */
L8c010244: *stop=0x8c010244u;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102e8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c010246; /* mov.l 0x8c0102e8,r4 */
L8c010246: *stop=0x8c010246u;r[15]+=12u;goto L8c010248; /* add #0xc,r15 */
L8c010248: *stop=0x8c010248u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c01024a; /* lds.l @r15+,PR */
L8c01024a: *stop=0x8c01024au;{uint32_t loaded=rd(m,bus,&io_status,0x8c0102ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c01024c; /* mov.l 0x8c0102ec,r2 */
L8c01024c: *stop=0x8c01024cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c01024e; /* mov.l @r15+,r9 */
L8c01024e: *stop=0x8c01024eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c010250; /* mov.l @r15+,r10 */
L8c010250: *stop=0x8c010250u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c010252; /* mov.l @r15+,r11 */
L8c010252: *stop=0x8c010252u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c010254; /* mov.l @r15+,r12 */
L8c010254: *stop=0x8c010254u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c010256; /* mov.l @r15+,r13 */
L8c010256: target=r[2];*stop=0x8c010258u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
}

SA_STAGE_EXPORT int sa_movie_startup_8c6024d0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6024d0;
L8c6024d0: *stop=0x8c6024d0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6024d2; /* sts.l PR,@-r15 */
L8c6024d2: target=0x8c602570u;*pr=0x8c6024d6u;*stop=0x8c6024d4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6024d6; /* bsr 0x8c602570 */
L8c6024d6: *stop=0x8c6024d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60259cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6024d8; /* mov.l 0x8c60259c,r2 */
L8c6024d8: *stop=0x8c6024d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6025a4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6024da; /* mov.l 0x8c6025a4,r1 */
L8c6024da: *stop=0x8c6024dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6025a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6024dc; /* mov.l 0x8c6025a0,r3 */
L8c6024dc: *stop=0x8c6024dcu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6024de; /* mov.l @r2,r5 */
L8c6024de: target=r[1];*pr=0x8c6024e2u;*stop=0x8c6024e0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6024e2; /* jsr @r1 */
L8c6024e2: *stop=0x8c6024e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6025a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6024e4; /* mov.l 0x8c6025a8,r2 */
L8c6024e4: target=r[2];*pr=0x8c6024e8u;*stop=0x8c6024e6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6024e8; /* jsr @r2 */
L8c6024e8: *stop=0x8c6024e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6025b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6024ea; /* mov.l 0x8c6025b0,r2 */
L8c6024ea: *stop=0x8c6024eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6025acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6024ec; /* mov.l 0x8c6025ac,r3 */
L8c6024ec: *stop=0x8c6024ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6025b4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6024ee; /* mov.l 0x8c6025b4,r1 */
L8c6024ee: *stop=0x8c6024eeu;r[2]|=r[3];goto L8c6024f0; /* or r3,r2 */
L8c6024f0: target=r[2];*pr=0x8c6024f4u;*stop=0x8c6024f2u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6024f4; /* jsr @r2 */
L8c6024f4: *stop=0x8c6024f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6025b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6024f6; /* mov.l 0x8c6025b8,r3 */
L8c6024f6: target=r[3];*stop=0x8c6024f8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_movie_startup_8c02e720(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c02e720;
L8c02e720: *stop=0x8c02e720u;if(*pr!=entry_pr)return -3;*stop=0x8c02e722u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_startup_8c602570(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602570;
L8c602570: *stop=0x8c602570u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6025d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c602572; /* mov.l 0x8c6025d8,r3 */
L8c602572: *stop=0x8c602572u;r[4]=0u;goto L8c602574; /* mov #0x0,r4 */
L8c602574: *stop=0x8c602574u;if(!valid_access(r[3],2u))return -3;wr(m,bus,&io_status,r[3],r[4],2);if(io_status)return io_status;goto L8c602576; /* mov.w r4,@r3 */
L8c602576: *stop=0x8c602576u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6025dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c602578; /* mov.l 0x8c6025dc,r2 */
L8c602578: *stop=0x8c602578u;if(!valid_access(r[2],2u))return -3;wr(m,bus,&io_status,r[2],r[4],2);if(io_status)return io_status;goto L8c60257a; /* mov.w r4,@r2 */
L8c60257a: *stop=0x8c60257au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6025e0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c60257c; /* mov.l 0x8c6025e0,r1 */
L8c60257c: *stop=0x8c60257cu;if(!valid_access(r[1],2u))return -3;wr(m,bus,&io_status,r[1],r[4],2);if(io_status)return io_status;goto L8c60257e; /* mov.w r4,@r1 */
L8c60257e: *stop=0x8c60257eu;r[3]+=12u;goto L8c602580; /* add #0xc,r3 */
L8c602580: *stop=0x8c602580u;if(!valid_access(r[3],2u))return -3;wr(m,bus,&io_status,r[3],r[4],2);if(io_status)return io_status;goto L8c602582; /* mov.w r4,@r3 */
L8c602582: *stop=0x8c602582u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6025e4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c602584; /* mov.l 0x8c6025e4,r4 */
L8c602584: *stop=0x8c602584u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c602586; /* mov.l @r4,r3 */
L8c602586: *stop=0x8c602586u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c602588; /* tst r3,r3 */
L8c602588: if((*sr&1u))goto L8c602594;goto L8c60258a; /* bt 0x8c602594 */
L8c60258a: *stop=0x8c60258au;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60258c; /* mov.l @(0x4,r4),r3 */
L8c60258c: *stop=0x8c60258cu;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60258e; /* mov.l @r4,r2 */
L8c60258e: *stop=0x8c60258eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c602590; /* mov.l r3,@r2 */
L8c602590: *stop=0x8c602592u;r[4]+=8u;goto L8c602584; /* bra 0x8c602584 */
L8c602594: *stop=0x8c602594u;if(*pr!=entry_pr)return -3;*stop=0x8c602596u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_startup_8c645de0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c645de0;
L8c645de0: *stop=0x8c645de0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c645de2; /* mov.l r14,@-r15 */
L8c645de2: *stop=0x8c645de2u;r[15]+=4294967292u;goto L8c645de4; /* add #-0x4,r15 */
L8c645de4: *stop=0x8c645de4u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c645de6; /* stc SR,r0 */
L8c645de6: *stop=0x8c645de6u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645e56u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c645de8; /* mov.w 0x8c645e56,r3 */
L8c645de8: *stop=0x8c645de8u;r[0]>>=2;goto L8c645dea; /* shlr2 r0 */
L8c645dea: *stop=0x8c645deau;r[0]>>=2;goto L8c645dec; /* shlr2 r0 */
L8c645dec: *stop=0x8c645decu;r[0]&=0xfu;goto L8c645dee; /* and #0xf,r0 */
L8c645dee: *stop=0x8c645deeu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c645df0; /* mov.l r0,@r15 */
L8c645df0: *stop=0x8c645df0u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c645df2; /* stc SR,r0 */
L8c645df2: *stop=0x8c645df2u;r[0]&=r[3];goto L8c645df4; /* and r3,r0 */
L8c645df4: *stop=0x8c645df4u;r[0]|=0xf0u;goto L8c645df6; /* or #0xf0,r0 */
L8c645df6: *stop=0x8c645df6u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c645df8; /* ldc r0,SR */
L8c645df8: *stop=0x8c645df8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645e60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645dfa; /* mov.l 0x8c645e60,r3 */
L8c645dfa: *stop=0x8c645dfau;{uint32_t loaded=sx(m,bus,&io_status,0x8c645e58u,2);if(io_status)return io_status;r[2]=loaded;}goto L8c645dfc; /* mov.w 0x8c645e58,r2 */
L8c645dfc: *stop=0x8c645dfcu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c645dfe; /* mov.l @r3,r1 */
L8c645dfe: *stop=0x8c645dfeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645e64u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c645e00; /* mov.l 0x8c645e64,r7 */
L8c645e00: *stop=0x8c645e00u;r[1]|=r[2];goto L8c645e02; /* or r2,r1 */
L8c645e02: *stop=0x8c645e02u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c645e04; /* mov.l r1,@r3 */
L8c645e04: *stop=0x8c645e04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645e68u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645e06; /* mov.l 0x8c645e68,r3 */
L8c645e06: *stop=0x8c645e06u;*sr=(*sr&~1u)|(r[3]==r[4]);goto L8c645e08; /* cmp/eq r3,r4 */
L8c645e08: take=!(*sr&1u);*stop=0x8c645e0au;r[6]=0u;if(take)goto L8c645e1e;goto L8c645e0c; /* bf/s 0x8c645e1e */
L8c645e0c: *stop=0x8c645e0cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645e74u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645e0e; /* mov.l 0x8c645e74,r2 */
L8c645e0e: *stop=0x8c645e0eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645e70u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c645e10; /* mov.l 0x8c645e70,r0 */
L8c645e10: *stop=0x8c645e10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645e6cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c645e12; /* mov.l 0x8c645e6c,r4 */
L8c645e12: *stop=0x8c645e12u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645e78u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645e14; /* mov.l 0x8c645e78,r3 */
L8c645e14: *stop=0x8c645e14u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[4],4);if(io_status)return io_status;goto L8c645e16; /* mov.l r4,@r0 */
L8c645e16: *stop=0x8c645e16u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[4],4);if(io_status)return io_status;goto L8c645e18; /* mov.l r4,@r7 */
L8c645e18: *stop=0x8c645e18u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[6],4);if(io_status)return io_status;goto L8c645e1a; /* mov.l r6,@r2 */
L8c645e1a: *stop=0x8c645e1cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c645e2e; /* bra 0x8c645e2e */
L8c645e1e: *stop=0x8c645e1eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645e74u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645e20; /* mov.l 0x8c645e74,r2 */
L8c645e20: *stop=0x8c645e20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645e78u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645e22; /* mov.l 0x8c645e78,r3 */
L8c645e22: *stop=0x8c645e22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645e7cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c645e24; /* mov.l 0x8c645e7c,r4 */
L8c645e24: *stop=0x8c645e24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645e70u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c645e26; /* mov.l 0x8c645e70,r0 */
L8c645e26: *stop=0x8c645e26u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[4],4);if(io_status)return io_status;goto L8c645e28; /* mov.l r4,@r0 */
L8c645e28: *stop=0x8c645e28u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[4],4);if(io_status)return io_status;goto L8c645e2a; /* mov.l r4,@r7 */
L8c645e2a: *stop=0x8c645e2au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[6],4);if(io_status)return io_status;goto L8c645e2c; /* mov.l r6,@r2 */
L8c645e2c: *stop=0x8c645e2cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[6],4);if(io_status)return io_status;goto L8c645e2e; /* mov.l r6,@r3 */
L8c645e2e: *stop=0x8c645e2eu;status=sa_sh4_stc_vbr(*vbr,*sr,&r[14],0u);if(status)return status;goto L8c645e30; /* stc VBR,r14 */
L8c645e30: *stop=0x8c645e30u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645e80u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c645e32; /* mov.l 0x8c645e80,r4 */
L8c645e32: *stop=0x8c645e32u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645e5au,2);if(io_status)return io_status;r[3]=loaded;}goto L8c645e34; /* mov.w 0x8c645e5a,r3 */
L8c645e34: *stop=0x8c645e36u;r[14]+=r[3];goto L8c645e3e; /* bra 0x8c645e3e */
L8c645e38: *stop=0x8c645e38u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}r[4]+=4;goto L8c645e3a; /* mov.l @r4+,r2 */
L8c645e3a: *stop=0x8c645e3au;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c645e3c; /* mov.l r2,@r14 */
L8c645e3c: *stop=0x8c645e3cu;r[14]+=4u;goto L8c645e3e; /* add #0x4,r14 */
L8c645e3e: *stop=0x8c645e3eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645e84u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645e40; /* mov.l 0x8c645e84,r3 */
L8c645e40: *stop=0x8c645e40u;*sr=(*sr&~1u)|(r[4]>r[3]);goto L8c645e42; /* cmp/hi r3,r4 */
L8c645e42: if(!(*sr&1u))goto L8c645e38;goto L8c645e44; /* bf 0x8c645e38 */
L8c645e44: *stop=0x8c645e44u;status=sa_sh4_stc_vbr(*vbr,*sr,&r[1],0u);if(status)return status;goto L8c645e46; /* stc VBR,r1 */
L8c645e46: *stop=0x8c645e46u;status=sa_sh4_stc_vbr(*vbr,*sr,&r[4],0u);if(status)return status;goto L8c645e48; /* stc VBR,r4 */
L8c645e48: *stop=0x8c645e48u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645e5eu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c645e4a; /* mov.w 0x8c645e5e,r3 */
L8c645e4a: *stop=0x8c645e4au;{uint32_t loaded=sx(m,bus,&io_status,0x8c645e5cu,2);if(io_status)return io_status;r[5]=loaded;}goto L8c645e4c; /* mov.w 0x8c645e5c,r5 */
L8c645e4c: *stop=0x8c645e4cu;r[14]-=r[1];goto L8c645e4e; /* sub r1,r14 */
L8c645e4e: *stop=0x8c645e4eu;r[4]+=r[3];goto L8c645e50; /* add r3,r4 */
L8c645e50: *stop=0x8c645e50u;r[5]+=r[14];goto L8c645e52; /* add r14,r5 */
L8c645e52: *stop=0x8c645e54u;goto L8c645e88; /* bra 0x8c645e88 */
L8c645e88: *stop=0x8c645e88u;r[5]>>=2;goto L8c645e8a; /* shlr2 r5 */
L8c645e8a: *stop=0x8c645e8au;r[5]>>=2;goto L8c645e8c; /* shlr2 r5 */
L8c645e8c: *stop=0x8c645e8cu;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c645e8e; /* shlr r5 */
L8c645e8e: *stop=0x8c645e8eu;r[5]+=1u;goto L8c645e90; /* add #0x1,r5 */
L8c645e90: *stop=0x8c645e90u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],3u);if(status)return status;goto L8c645e92; /* ocbp @r4 */
L8c645e92: *stop=0x8c645e92u;r[5]--;*sr=(*sr&~1u)|(!r[5]);goto L8c645e94; /* dt r5 */
L8c645e94: take=!(*sr&1u);*stop=0x8c645e96u;r[4]+=32u;if(take)goto L8c645e90;goto L8c645e98; /* bf/s 0x8c645e90 */
L8c645e98: *stop=0x8c645e98u;status=sa_sh4_stc_vbr(*vbr,*sr,&r[14],0u);if(status)return status;goto L8c645e9a; /* stc VBR,r14 */
L8c645e9a: *stop=0x8c645e9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c645ec8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c645e9c; /* mov.l 0x8c645ec8,r4 */
L8c645e9c: *stop=0x8c645e9cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c645ec0u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c645e9e; /* mov.w 0x8c645ec0,r3 */
L8c645e9e: *stop=0x8c645ea0u;r[14]+=r[3];goto L8c645ea8; /* bra 0x8c645ea8 */
L8c645ea2: *stop=0x8c645ea2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}r[4]+=4;goto L8c645ea4; /* mov.l @r4+,r2 */
L8c645ea4: *stop=0x8c645ea4u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c645ea6; /* mov.l r2,@r14 */
L8c645ea6: *stop=0x8c645ea6u;r[14]+=4u;goto L8c645ea8; /* add #0x4,r14 */
L8c645ea8: *stop=0x8c645ea8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645eccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645eaa; /* mov.l 0x8c645ecc,r3 */
L8c645eaa: *stop=0x8c645eaau;*sr=(*sr&~1u)|(r[4]>r[3]);goto L8c645eac; /* cmp/hi r3,r4 */
L8c645eac: if(!(*sr&1u))goto L8c645ea2;goto L8c645eae; /* bf 0x8c645ea2 */
L8c645eae: *stop=0x8c645eaeu;status=sa_sh4_stc_vbr(*vbr,*sr,&r[1],0u);if(status)return status;goto L8c645eb0; /* stc VBR,r1 */
L8c645eb0: *stop=0x8c645eb0u;status=sa_sh4_stc_vbr(*vbr,*sr,&r[4],0u);if(status)return status;goto L8c645eb2; /* stc VBR,r4 */
L8c645eb2: *stop=0x8c645eb2u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645ec4u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c645eb4; /* mov.w 0x8c645ec4,r3 */
L8c645eb4: *stop=0x8c645eb4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645ec2u,2);if(io_status)return io_status;r[5]=loaded;}goto L8c645eb6; /* mov.w 0x8c645ec2,r5 */
L8c645eb6: *stop=0x8c645eb6u;r[14]-=r[1];goto L8c645eb8; /* sub r1,r14 */
L8c645eb8: *stop=0x8c645eb8u;r[4]+=r[3];goto L8c645eba; /* add r3,r4 */
L8c645eba: *stop=0x8c645ebau;r[5]+=r[14];goto L8c645ebc; /* add r14,r5 */
L8c645ebc: *stop=0x8c645ebeu;goto L8c645ed0; /* bra 0x8c645ed0 */
L8c645ed0: *stop=0x8c645ed0u;r[5]>>=2;goto L8c645ed2; /* shlr2 r5 */
L8c645ed2: *stop=0x8c645ed2u;r[5]>>=2;goto L8c645ed4; /* shlr2 r5 */
L8c645ed4: *stop=0x8c645ed4u;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c645ed6; /* shlr r5 */
L8c645ed6: *stop=0x8c645ed6u;r[5]+=1u;goto L8c645ed8; /* add #0x1,r5 */
L8c645ed8: *stop=0x8c645ed8u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],3u);if(status)return status;goto L8c645eda; /* ocbp @r4 */
L8c645eda: *stop=0x8c645edau;r[5]--;*sr=(*sr&~1u)|(!r[5]);goto L8c645edc; /* dt r5 */
L8c645edc: take=!(*sr&1u);*stop=0x8c645edeu;r[4]+=32u;if(take)goto L8c645ed8;goto L8c645ee0; /* bf/s 0x8c645ed8 */
L8c645ee0: *stop=0x8c645ee0u;status=sa_sh4_stc_vbr(*vbr,*sr,&r[14],0u);if(status)return status;goto L8c645ee2; /* stc VBR,r14 */
L8c645ee2: *stop=0x8c645ee2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645f10u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c645ee4; /* mov.l 0x8c645f10,r4 */
L8c645ee4: *stop=0x8c645ee4u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645f08u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c645ee6; /* mov.w 0x8c645f08,r3 */
L8c645ee6: *stop=0x8c645ee8u;r[14]+=r[3];goto L8c645ef0; /* bra 0x8c645ef0 */
L8c645eea: *stop=0x8c645eeau;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}r[4]+=4;goto L8c645eec; /* mov.l @r4+,r2 */
L8c645eec: *stop=0x8c645eecu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c645eee; /* mov.l r2,@r14 */
L8c645eee: *stop=0x8c645eeeu;r[14]+=4u;goto L8c645ef0; /* add #0x4,r14 */
L8c645ef0: *stop=0x8c645ef0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645f14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645ef2; /* mov.l 0x8c645f14,r3 */
L8c645ef2: *stop=0x8c645ef2u;*sr=(*sr&~1u)|(r[4]>r[3]);goto L8c645ef4; /* cmp/hi r3,r4 */
L8c645ef4: if(!(*sr&1u))goto L8c645eea;goto L8c645ef6; /* bf 0x8c645eea */
L8c645ef6: *stop=0x8c645ef6u;status=sa_sh4_stc_vbr(*vbr,*sr,&r[1],0u);if(status)return status;goto L8c645ef8; /* stc VBR,r1 */
L8c645ef8: *stop=0x8c645ef8u;status=sa_sh4_stc_vbr(*vbr,*sr,&r[4],0u);if(status)return status;goto L8c645efa; /* stc VBR,r4 */
L8c645efa: *stop=0x8c645efau;{uint32_t loaded=sx(m,bus,&io_status,0x8c645f0cu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c645efc; /* mov.w 0x8c645f0c,r3 */
L8c645efc: *stop=0x8c645efcu;{uint32_t loaded=sx(m,bus,&io_status,0x8c645f0au,2);if(io_status)return io_status;r[5]=loaded;}goto L8c645efe; /* mov.w 0x8c645f0a,r5 */
L8c645efe: *stop=0x8c645efeu;r[14]-=r[1];goto L8c645f00; /* sub r1,r14 */
L8c645f00: *stop=0x8c645f00u;r[4]+=r[3];goto L8c645f02; /* add r3,r4 */
L8c645f02: *stop=0x8c645f02u;r[5]+=r[14];goto L8c645f04; /* add r14,r5 */
L8c645f04: *stop=0x8c645f06u;goto L8c645f18; /* bra 0x8c645f18 */
L8c645f18: *stop=0x8c645f18u;r[5]>>=2;goto L8c645f1a; /* shlr2 r5 */
L8c645f1a: *stop=0x8c645f1au;r[5]>>=2;goto L8c645f1c; /* shlr2 r5 */
L8c645f1c: *stop=0x8c645f1cu;*sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1;goto L8c645f1e; /* shlr r5 */
L8c645f1e: *stop=0x8c645f1eu;r[5]+=1u;goto L8c645f20; /* add #0x1,r5 */
L8c645f20: *stop=0x8c645f20u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],3u);if(status)return status;goto L8c645f22; /* ocbp @r4 */
L8c645f22: *stop=0x8c645f22u;r[5]--;*sr=(*sr&~1u)|(!r[5]);goto L8c645f24; /* dt r5 */
L8c645f24: take=!(*sr&1u);*stop=0x8c645f26u;r[4]+=32u;if(take)goto L8c645f20;goto L8c645f28; /* bf/s 0x8c645f20 */
L8c645f28: *stop=0x8c645f28u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c645f2a; /* mov.l @r15,r0 */
L8c645f2a: *stop=0x8c645f2au;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c645f2c; /* stc SR,r3 */
L8c645f2c: *stop=0x8c645f2cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c645f5cu,2);if(io_status)return io_status;r[2]=loaded;}goto L8c645f2e; /* mov.w 0x8c645f5c,r2 */
L8c645f2e: *stop=0x8c645f2eu;r[0]&=0xfu;goto L8c645f30; /* and #0xf,r0 */
L8c645f30: *stop=0x8c645f30u;r[0]<<=2;goto L8c645f32; /* shll2 r0 */
L8c645f32: *stop=0x8c645f32u;r[3]&=r[2];goto L8c645f34; /* and r2,r3 */
L8c645f34: *stop=0x8c645f34u;r[0]<<=2;goto L8c645f36; /* shll2 r0 */
L8c645f36: *stop=0x8c645f36u;r[0]|=r[3];goto L8c645f38; /* or r3,r0 */
L8c645f38: *stop=0x8c645f38u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c645f3a; /* ldc r0,SR */
L8c645f3a: *stop=0x8c645f3au;{uint32_t loaded=sx(m,bus,&io_status,0x8c645f60u,2);if(io_status)return io_status;r[5]=loaded;}goto L8c645f3c; /* mov.w 0x8c645f60,r5 */
L8c645f3c: *stop=0x8c645f3cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c645f62u,2);if(io_status)return io_status;r[4]=loaded;}goto L8c645f3e; /* mov.w 0x8c645f62,r4 */
L8c645f3e: *stop=0x8c645f3eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645f64u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c645f40; /* mov.l 0x8c645f64,r7 */
L8c645f40: *stop=0x8c645f40u;{uint32_t loaded=sx(m,bus,&io_status,0x8c645f5eu,2);if(io_status)return io_status;r[6]=loaded;}goto L8c645f42; /* mov.w 0x8c645f5e,r6 */
L8c645f42: *stop=0x8c645f42u;r[2]=r[4];goto L8c645f44; /* mov r4,r2 */
L8c645f44: *stop=0x8c645f44u;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u);goto L8c645f46; /* shar r2 */
L8c645f46: *stop=0x8c645f46u;status=sa_sh4_stc_vbr(*vbr,*sr,&r[3],0u);if(status)return status;goto L8c645f48; /* stc VBR,r3 */
L8c645f48: *stop=0x8c645f48u;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u);goto L8c645f4a; /* shar r2 */
L8c645f4a: *stop=0x8c645f4au;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u);goto L8c645f4c; /* shar r2 */
L8c645f4c: *stop=0x8c645f4cu;r[3]+=r[2];goto L8c645f4e; /* add r2,r3 */
L8c645f4e: *stop=0x8c645f4eu;r[4]+=32u;goto L8c645f50; /* add #0x20,r4 */
L8c645f50: *stop=0x8c645f50u;r[3]+=r[5];goto L8c645f52; /* add r5,r3 */
L8c645f52: *stop=0x8c645f52u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[6]);goto L8c645f54; /* cmp/ge r6,r4 */
L8c645f54: *stop=0x8c645f54u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[7],4);if(io_status)return io_status;goto L8c645f56; /* mov.l r7,@r3 */
L8c645f56: if(!(*sr&1u))goto L8c645f42;goto L8c645f58; /* bf 0x8c645f42 */
L8c645f58: *stop=0x8c645f5au;goto L8c645f68; /* bra 0x8c645f68 */
L8c645f68: *stop=0x8c645f68u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c645f6a; /* stc SR,r0 */
L8c645f6a: *stop=0x8c645f6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c645f74u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c645f6c; /* mov.l 0x8c645f74,r1 */
L8c645f6c: *stop=0x8c645f6cu;r[0]&=r[1];goto L8c645f6e; /* and r1,r0 */
L8c645f6e: *stop=0x8c645f6eu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c645f70; /* ldc r0,SR */
L8c645f70: *stop=0x8c645f72u;goto L8c645f78; /* bra 0x8c645f78 */
L8c645f78: *stop=0x8c645f78u;r[15]+=4u;goto L8c645f7a; /* add #0x4,r15 */
L8c645f7a: *stop=0x8c645f7au;if(*pr!=entry_pr)return -3;*stop=0x8c645f7cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_startup_8c645f80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c645f80;
L8c645f80: *stop=0x8c645f80u;if(*pr!=entry_pr)return -3;*stop=0x8c645f82u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_startup_8c6025e8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6025e8;
L8c6025e8: *stop=0x8c6025e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60270cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6025ea; /* mov.l 0x8c60270c,r2 */
L8c6025ea: *stop=0x8c6025eau;r[3]=0u;goto L8c6025ec; /* mov #0x0,r3 */
L8c6025ec: *stop=0x8c6025ecu;if(!valid_access(r[2],1u))return -3;wr(m,bus,&io_status,r[2],r[3],1);if(io_status)return io_status;goto L8c6025ee; /* mov.b r3,@r2 */
L8c6025ee: *stop=0x8c6025eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c602710u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6025f0; /* mov.l 0x8c602710,r4 */
L8c6025f0: *stop=0x8c6025f0u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[0]=loaded;}goto L8c6025f2; /* mov.b @r4,r0 */
L8c6025f2: *stop=0x8c6025f2u;r[0]&=0xfeu;goto L8c6025f4; /* and #0xfe,r0 */
L8c6025f4: *stop=0x8c6025f4u;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[0],1);if(io_status)return io_status;goto L8c6025f6; /* mov.b r0,@r4 */
L8c6025f6: *stop=0x8c6025f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c602714u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6025f8; /* mov.l 0x8c602714,r1 */
L8c6025f8: *stop=0x8c6025f8u;r[3]=2u;goto L8c6025fa; /* mov #0x2,r3 */
L8c6025fa: *stop=0x8c6025fau;if(!valid_access(r[1],2u))return -3;wr(m,bus,&io_status,r[1],r[3],2);if(io_status)return io_status;goto L8c6025fc; /* mov.w r3,@r1 */
L8c6025fc: *stop=0x8c6025fcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c602718u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6025fe; /* mov.l 0x8c602718,r3 */
L8c6025fe: *stop=0x8c6025feu;r[5]=4294967295u;goto L8c602600; /* mov #-0x1,r5 */
L8c602600: *stop=0x8c602600u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c602602; /* mov.l r5,@r3 */
L8c602602: *stop=0x8c602602u;r[2]+=12u;goto L8c602604; /* add #0xc,r2 */
L8c602604: *stop=0x8c602604u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[5],4);if(io_status)return io_status;goto L8c602606; /* mov.l r5,@r2 */
L8c602606: *stop=0x8c602606u;if(!valid_access(r[4],1u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[0]=loaded;}goto L8c602608; /* mov.b @r4,r0 */
L8c602608: *stop=0x8c602608u;r[0]|=0x1u;goto L8c60260a; /* or #0x1,r0 */
L8c60260a: *stop=0x8c60260au;if(*pr!=entry_pr)return -3;*stop=0x8c60260cu;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[0],1);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_startup_8c5fc81c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5fc81c;
L8c5fc81c: *stop=0x8c5fc81cu;r[15]+=4294967292u;goto L8c5fc81e; /* add #-0x4,r15 */
L8c5fc81e: *stop=0x8c5fc81eu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c5fc820; /* stc SR,r0 */
L8c5fc820: *stop=0x8c5fc820u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fc836u,2);if(io_status)return io_status;r[3]=loaded;}goto L8c5fc822; /* mov.w 0x8c5fc836,r3 */
L8c5fc822: *stop=0x8c5fc822u;r[0]>>=2;goto L8c5fc824; /* shlr2 r0 */
L8c5fc824: *stop=0x8c5fc824u;r[0]>>=2;goto L8c5fc826; /* shlr2 r0 */
L8c5fc826: *stop=0x8c5fc826u;r[0]&=0xfu;goto L8c5fc828; /* and #0xf,r0 */
L8c5fc828: *stop=0x8c5fc828u;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c5fc82a; /* mov.w r0,@r15 */
L8c5fc82a: *stop=0x8c5fc82au;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c5fc82c; /* stc SR,r0 */
L8c5fc82c: *stop=0x8c5fc82cu;r[0]&=r[3];goto L8c5fc82e; /* and r3,r0 */
L8c5fc82e: *stop=0x8c5fc82eu;r[0]|=0xf0u;goto L8c5fc830; /* or #0xf0,r0 */
L8c5fc830: *stop=0x8c5fc830u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c5fc832; /* ldc r0,SR */
L8c5fc832: *stop=0x8c5fc834u;goto L8c5fc838; /* bra 0x8c5fc838 */
L8c5fc838: *stop=0x8c5fc838u;r[0]=0x8c5fc848u;goto L8c5fc83a; /* mova 0x8c5fc848,r0 */
L8c5fc83a: *stop=0x8c5fc83au;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc844u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c5fc83c; /* mov.l 0x8c5fc844,r1 */
L8c5fc83c: *stop=0x8c5fc83cu;r[1]|=r[0];goto L8c5fc83e; /* or r0,r1 */
L8c5fc83e: target=r[1];*stop=0x8c5fc840u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r1 */
}

SA_STAGE_EXPORT int sa_movie_startup_8c5fc848(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c5fc848;
L8c5fc848: *stop=0x8c5fc848u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc8a0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fc84a; /* mov.l 0x8c5fc8a0,r2 */
L8c5fc84a: *stop=0x8c5fc84au;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc89cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c5fc84c; /* mov.l 0x8c5fc89c,r5 */
L8c5fc84c: *stop=0x8c5fc84cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c5fc84e; /* mov.l @r2,r0 */
L8c5fc84e: *stop=0x8c5fc84eu;r[0]>>=2;goto L8c5fc850; /* shlr2 r0 */
L8c5fc850: *stop=0x8c5fc850u;r[0]>>=2;goto L8c5fc852; /* shlr2 r0 */
L8c5fc852: *stop=0x8c5fc852u;*sr=(*sr&~1u)|(r[0]&1u);r[0]>>=1;goto L8c5fc854; /* shlr r0 */
L8c5fc854: *stop=0x8c5fc854u;r[0]&=0x1u;goto L8c5fc856; /* and #0x1,r0 */
L8c5fc856: *stop=0x8c5fc856u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c5fc858; /* cmp/eq #0x1,r0 */
L8c5fc858: take=!(*sr&1u);*stop=0x8c5fc85au;r[4]=0u;if(take)goto L8c5fc880;goto L8c5fc85c; /* bf/s 0x8c5fc880 */
L8c5fc85c: *stop=0x8c5fc85cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fc894u,2);if(io_status)return io_status;r[7]=loaded;}goto L8c5fc85e; /* mov.w 0x8c5fc894,r7 */
L8c5fc85e: *stop=0x8c5fc85eu;r[6]=r[4];goto L8c5fc860; /* mov r4,r6 */
L8c5fc860: *stop=0x8c5fc860u;r[2]=r[6];goto L8c5fc862; /* mov r6,r2 */
L8c5fc862: *stop=0x8c5fc862u;r[6]+=32u;goto L8c5fc864; /* add #0x20,r6 */
L8c5fc864: *stop=0x8c5fc864u;*sr=(*sr&~1u)|(r[6]>=r[7]);goto L8c5fc866; /* cmp/hs r7,r6 */
L8c5fc866: *stop=0x8c5fc866u;r[2]|=r[5];goto L8c5fc868; /* or r5,r2 */
L8c5fc868: take=!(*sr&1u);*stop=0x8c5fc86au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;if(take)goto L8c5fc860;goto L8c5fc86c; /* bf/s 0x8c5fc860 */
L8c5fc86c: *stop=0x8c5fc86cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fc898u,2);if(io_status)return io_status;r[6]=loaded;}goto L8c5fc86e; /* mov.w 0x8c5fc898,r6 */
L8c5fc86e: *stop=0x8c5fc86eu;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fc896u,2);if(io_status)return io_status;r[7]=loaded;}goto L8c5fc870; /* mov.w 0x8c5fc896,r7 */
L8c5fc870: *stop=0x8c5fc870u;r[2]=r[6];goto L8c5fc872; /* mov r6,r2 */
L8c5fc872: *stop=0x8c5fc872u;r[6]+=32u;goto L8c5fc874; /* add #0x20,r6 */
L8c5fc874: *stop=0x8c5fc874u;*sr=(*sr&~1u)|(r[6]>=r[7]);goto L8c5fc876; /* cmp/hs r7,r6 */
L8c5fc876: *stop=0x8c5fc876u;r[2]|=r[5];goto L8c5fc878; /* or r5,r2 */
L8c5fc878: take=!(*sr&1u);*stop=0x8c5fc87au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;if(take)goto L8c5fc870;goto L8c5fc87c; /* bf/s 0x8c5fc870 */
L8c5fc87c: *stop=0x8c5fc87eu;goto L8c5fc890; /* bra 0x8c5fc890 */
L8c5fc880: *stop=0x8c5fc880u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fc89au,2);if(io_status)return io_status;r[7]=loaded;}goto L8c5fc882; /* mov.w 0x8c5fc89a,r7 */
L8c5fc882: *stop=0x8c5fc882u;r[6]=r[4];goto L8c5fc884; /* mov r4,r6 */
L8c5fc884: *stop=0x8c5fc884u;r[3]=r[6];goto L8c5fc886; /* mov r6,r3 */
L8c5fc886: *stop=0x8c5fc886u;r[6]+=32u;goto L8c5fc888; /* add #0x20,r6 */
L8c5fc888: *stop=0x8c5fc888u;*sr=(*sr&~1u)|(r[6]>=r[7]);goto L8c5fc88a; /* cmp/hs r7,r6 */
L8c5fc88a: *stop=0x8c5fc88au;r[3]|=r[5];goto L8c5fc88c; /* or r5,r3 */
L8c5fc88c: take=!(*sr&1u);*stop=0x8c5fc88eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;if(take)goto L8c5fc884;goto L8c5fc890; /* bf/s 0x8c5fc884 */
L8c5fc890: *stop=0x8c5fc892u;goto L8c5fc8a4; /* bra 0x8c5fc8a4 */
L8c5fc8a4: *stop=0x8c5fc8a4u;goto L8c5fc8a6; /* nop */
L8c5fc8a6: *stop=0x8c5fc8a6u;goto L8c5fc8a8; /* nop */
L8c5fc8a8: *stop=0x8c5fc8a8u;goto L8c5fc8aa; /* nop */
L8c5fc8aa: *stop=0x8c5fc8aau;goto L8c5fc8ac; /* nop */
L8c5fc8ac: *stop=0x8c5fc8acu;goto L8c5fc8ae; /* nop */
L8c5fc8ae: *stop=0x8c5fc8aeu;goto L8c5fc8b0; /* nop */
L8c5fc8b0: *stop=0x8c5fc8b0u;goto L8c5fc8b2; /* nop */
L8c5fc8b2: *stop=0x8c5fc8b2u;goto L8c5fc8b4; /* nop */
L8c5fc8b4: *stop=0x8c5fc8b4u;if(!valid_access(r[15],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;r[0]=loaded;}goto L8c5fc8b6; /* mov.w @r15,r0 */
L8c5fc8b6: *stop=0x8c5fc8b6u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c5fc8b8; /* stc SR,r3 */
L8c5fc8b8: *stop=0x8c5fc8b8u;r[0]=r[0]&65535u;goto L8c5fc8ba; /* extu.w r0,r0 */
L8c5fc8ba: *stop=0x8c5fc8bau;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fc93au,2);if(io_status)return io_status;r[2]=loaded;}goto L8c5fc8bc; /* mov.w 0x8c5fc93a,r2 */
L8c5fc8bc: *stop=0x8c5fc8bcu;r[0]&=0xfu;goto L8c5fc8be; /* and #0xf,r0 */
L8c5fc8be: *stop=0x8c5fc8beu;r[0]<<=2;goto L8c5fc8c0; /* shll2 r0 */
L8c5fc8c0: *stop=0x8c5fc8c0u;r[3]&=r[2];goto L8c5fc8c2; /* and r2,r3 */
L8c5fc8c2: *stop=0x8c5fc8c2u;r[0]<<=2;goto L8c5fc8c4; /* shll2 r0 */
L8c5fc8c4: *stop=0x8c5fc8c4u;r[0]|=r[3];goto L8c5fc8c6; /* or r3,r0 */
L8c5fc8c6: *stop=0x8c5fc8c6u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c5fc8c8; /* ldc r0,SR */
L8c5fc8c8: *stop=0x8c5fc8c8u;if(*pr!=entry_pr)return -3;*stop=0x8c5fc8cau;r[15]+=4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_startup_8c5fc06c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5fc06c;
L8c5fc06c: *stop=0x8c5fc06cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c5fc06e; /* sts.l PR,@-r15 */
L8c5fc06e: *stop=0x8c5fc06eu;r[15]+=4294967288u;goto L8c5fc070; /* add #-0x8,r15 */
L8c5fc070: *stop=0x8c5fc070u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c5fc072; /* stc SR,r0 */
L8c5fc072: *stop=0x8c5fc072u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fc10au,2);if(io_status)return io_status;r[3]=loaded;}goto L8c5fc074; /* mov.w 0x8c5fc10a,r3 */
L8c5fc074: *stop=0x8c5fc074u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c5fc076; /* mov.l r4,@r15 */
L8c5fc076: *stop=0x8c5fc076u;r[0]>>=2;goto L8c5fc078; /* shlr2 r0 */
L8c5fc078: *stop=0x8c5fc078u;r[0]>>=2;goto L8c5fc07a; /* shlr2 r0 */
L8c5fc07a: *stop=0x8c5fc07au;r[0]&=0xfu;goto L8c5fc07c; /* and #0xf,r0 */
L8c5fc07c: *stop=0x8c5fc07cu;if(!valid_access(r[15]+4u,2u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],2);if(io_status)return io_status;goto L8c5fc07e; /* mov.w r0,@(0x4,r15) */
L8c5fc07e: *stop=0x8c5fc07eu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c5fc080; /* stc SR,r0 */
L8c5fc080: *stop=0x8c5fc080u;r[0]&=r[3];goto L8c5fc082; /* and r3,r0 */
L8c5fc082: *stop=0x8c5fc082u;r[0]|=0xf0u;goto L8c5fc084; /* or #0xf0,r0 */
L8c5fc084: *stop=0x8c5fc084u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c5fc086; /* ldc r0,SR */
L8c5fc086: *stop=0x8c5fc086u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc110u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fc088; /* mov.l 0x8c5fc110,r2 */
L8c5fc088: target=r[2];*pr=0x8c5fc08cu;*stop=0x8c5fc08au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5fc08c; /* jsr @r2 */
L8c5fc08c: *stop=0x8c5fc08cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5fc08e; /* mov.l @r15,r3 */
L8c5fc08e: *stop=0x8c5fc08eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5fc114u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c5fc090; /* mov.l 0x8c5fc114,r4 */
L8c5fc090: *stop=0x8c5fc090u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fc10eu,2);if(io_status)return io_status;r[1]=loaded;}goto L8c5fc092; /* mov.w 0x8c5fc10e,r1 */
L8c5fc092: *stop=0x8c5fc092u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c5fc094; /* mov.l r3,@r4 */
L8c5fc094: *stop=0x8c5fc094u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5fc096; /* mov.l @r4,r2 */
L8c5fc096: *stop=0x8c5fc096u;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fc10cu,2);if(io_status)return io_status;r[3]=loaded;}goto L8c5fc098; /* mov.w 0x8c5fc10c,r3 */
L8c5fc098: *stop=0x8c5fc098u;r[2]&=r[3];goto L8c5fc09a; /* and r3,r2 */
L8c5fc09a: *stop=0x8c5fc09au;r[2]|=r[1];goto L8c5fc09c; /* or r1,r2 */
L8c5fc09c: *stop=0x8c5fc09cu;{uint32_t loaded=sx(m,bus,&io_status,0x8c5fc10au,2);if(io_status)return io_status;r[1]=loaded;}goto L8c5fc09e; /* mov.w 0x8c5fc10a,r1 */
L8c5fc09e: *stop=0x8c5fc09eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c5fc0a0; /* mov.l r2,@r4 */
L8c5fc0a0: *stop=0x8c5fc0a0u;r[2]=4294967287u;goto L8c5fc0a2; /* mov #-0x9,r2 */
L8c5fc0a2: *stop=0x8c5fc0a2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c5fc0a4; /* mov.l @r4,r0 */
L8c5fc0a4: *stop=0x8c5fc0a4u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c5fc0a6; /* stc SR,r3 */
L8c5fc0a6: *stop=0x8c5fc0a6u;r[0]&=r[2];goto L8c5fc0a8; /* and r2,r0 */
L8c5fc0a8: *stop=0x8c5fc0a8u;r[0]|=0x8u;goto L8c5fc0aa; /* or #0x8,r0 */
L8c5fc0aa: *stop=0x8c5fc0aau;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c5fc0ac; /* mov.l r0,@r4 */
L8c5fc0ac: *stop=0x8c5fc0acu;r[3]&=r[1];goto L8c5fc0ae; /* and r1,r3 */
L8c5fc0ae: *stop=0x8c5fc0aeu;if(!valid_access(r[15]+4u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;r[0]=loaded;}goto L8c5fc0b0; /* mov.w @(0x4,r15),r0 */
L8c5fc0b0: *stop=0x8c5fc0b0u;r[0]&=0xfu;goto L8c5fc0b2; /* and #0xf,r0 */
L8c5fc0b2: *stop=0x8c5fc0b2u;r[0]<<=2;goto L8c5fc0b4; /* shll2 r0 */
L8c5fc0b4: *stop=0x8c5fc0b4u;r[0]<<=2;goto L8c5fc0b6; /* shll2 r0 */
L8c5fc0b6: *stop=0x8c5fc0b6u;r[0]|=r[3];goto L8c5fc0b8; /* or r3,r0 */
L8c5fc0b8: *stop=0x8c5fc0b8u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c5fc0ba; /* ldc r0,SR */
L8c5fc0ba: *stop=0x8c5fc0bau;r[15]+=8u;goto L8c5fc0bc; /* add #0x8,r15 */
L8c5fc0bc: *stop=0x8c5fc0bcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5fc0be; /* lds.l @r15+,PR */
L8c5fc0be: *stop=0x8c5fc0beu;if(*pr!=entry_pr)return -3;*stop=0x8c5fc0c0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_startup_ac5fc06c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto Lac5fc06c;
Lac5fc06c: *stop=0xac5fc06cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto Lac5fc06e; /* sts.l PR,@-r15 */
Lac5fc06e: *stop=0xac5fc06eu;r[15]+=4294967288u;goto Lac5fc070; /* add #-0x8,r15 */
Lac5fc070: *stop=0xac5fc070u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto Lac5fc072; /* stc SR,r0 */
Lac5fc072: *stop=0xac5fc072u;{uint32_t loaded=sx(m,bus,&io_status,0xac5fc10au,2);if(io_status)return io_status;r[3]=loaded;}goto Lac5fc074; /* mov.w 0x8c5fc10a,r3 */
Lac5fc074: *stop=0xac5fc074u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto Lac5fc076; /* mov.l r4,@r15 */
Lac5fc076: *stop=0xac5fc076u;r[0]>>=2;goto Lac5fc078; /* shlr2 r0 */
Lac5fc078: *stop=0xac5fc078u;r[0]>>=2;goto Lac5fc07a; /* shlr2 r0 */
Lac5fc07a: *stop=0xac5fc07au;r[0]&=0xfu;goto Lac5fc07c; /* and #0xf,r0 */
Lac5fc07c: *stop=0xac5fc07cu;if(!valid_access(r[15]+4u,2u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],2);if(io_status)return io_status;goto Lac5fc07e; /* mov.w r0,@(0x4,r15) */
Lac5fc07e: *stop=0xac5fc07eu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto Lac5fc080; /* stc SR,r0 */
Lac5fc080: *stop=0xac5fc080u;r[0]&=r[3];goto Lac5fc082; /* and r3,r0 */
Lac5fc082: *stop=0xac5fc082u;r[0]|=0xf0u;goto Lac5fc084; /* or #0xf0,r0 */
Lac5fc084: *stop=0xac5fc084u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto Lac5fc086; /* ldc r0,SR */
Lac5fc086: *stop=0xac5fc086u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc110u,4);if(io_status)return io_status;r[2]=loaded;}goto Lac5fc088; /* mov.l 0x8c5fc110,r2 */
Lac5fc088: target=r[2];*pr=0xac5fc08cu;*stop=0xac5fc08au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lac5fc08c; /* jsr @r2 */
Lac5fc08c: *stop=0xac5fc08cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto Lac5fc08e; /* mov.l @r15,r3 */
Lac5fc08e: *stop=0xac5fc08eu;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc114u,4);if(io_status)return io_status;r[4]=loaded;}goto Lac5fc090; /* mov.l 0x8c5fc114,r4 */
Lac5fc090: *stop=0xac5fc090u;{uint32_t loaded=sx(m,bus,&io_status,0xac5fc10eu,2);if(io_status)return io_status;r[1]=loaded;}goto Lac5fc092; /* mov.w 0x8c5fc10e,r1 */
Lac5fc092: *stop=0xac5fc092u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto Lac5fc094; /* mov.l r3,@r4 */
Lac5fc094: *stop=0xac5fc094u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[2]=loaded;}goto Lac5fc096; /* mov.l @r4,r2 */
Lac5fc096: *stop=0xac5fc096u;{uint32_t loaded=sx(m,bus,&io_status,0xac5fc10cu,2);if(io_status)return io_status;r[3]=loaded;}goto Lac5fc098; /* mov.w 0x8c5fc10c,r3 */
Lac5fc098: *stop=0xac5fc098u;r[2]&=r[3];goto Lac5fc09a; /* and r3,r2 */
Lac5fc09a: *stop=0xac5fc09au;r[2]|=r[1];goto Lac5fc09c; /* or r1,r2 */
Lac5fc09c: *stop=0xac5fc09cu;{uint32_t loaded=sx(m,bus,&io_status,0xac5fc10au,2);if(io_status)return io_status;r[1]=loaded;}goto Lac5fc09e; /* mov.w 0x8c5fc10a,r1 */
Lac5fc09e: *stop=0xac5fc09eu;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto Lac5fc0a0; /* mov.l r2,@r4 */
Lac5fc0a0: *stop=0xac5fc0a0u;r[2]=4294967287u;goto Lac5fc0a2; /* mov #-0x9,r2 */
Lac5fc0a2: *stop=0xac5fc0a2u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto Lac5fc0a4; /* mov.l @r4,r0 */
Lac5fc0a4: *stop=0xac5fc0a4u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto Lac5fc0a6; /* stc SR,r3 */
Lac5fc0a6: *stop=0xac5fc0a6u;r[0]&=r[2];goto Lac5fc0a8; /* and r2,r0 */
Lac5fc0a8: *stop=0xac5fc0a8u;r[0]|=0x8u;goto Lac5fc0aa; /* or #0x8,r0 */
Lac5fc0aa: *stop=0xac5fc0aau;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto Lac5fc0ac; /* mov.l r0,@r4 */
Lac5fc0ac: *stop=0xac5fc0acu;r[3]&=r[1];goto Lac5fc0ae; /* and r1,r3 */
Lac5fc0ae: *stop=0xac5fc0aeu;if(!valid_access(r[15]+4u,2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;r[0]=loaded;}goto Lac5fc0b0; /* mov.w @(0x4,r15),r0 */
Lac5fc0b0: *stop=0xac5fc0b0u;r[0]&=0xfu;goto Lac5fc0b2; /* and #0xf,r0 */
Lac5fc0b2: *stop=0xac5fc0b2u;r[0]<<=2;goto Lac5fc0b4; /* shll2 r0 */
Lac5fc0b4: *stop=0xac5fc0b4u;r[0]<<=2;goto Lac5fc0b6; /* shll2 r0 */
Lac5fc0b6: *stop=0xac5fc0b6u;r[0]|=r[3];goto Lac5fc0b8; /* or r3,r0 */
Lac5fc0b8: *stop=0xac5fc0b8u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto Lac5fc0ba; /* ldc r0,SR */
Lac5fc0ba: *stop=0xac5fc0bau;r[15]+=8u;goto Lac5fc0bc; /* add #0x8,r15 */
Lac5fc0bc: *stop=0xac5fc0bcu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto Lac5fc0be; /* lds.l @r15+,PR */
Lac5fc0be: *stop=0xac5fc0beu;if(*pr!=entry_pr)return -3;*stop=0xac5fc0c0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_movie_startup_ac5fc848(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)r;(void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lac5fc848;
Lac5fc848: *stop=0xac5fc848u;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc8a0u,4);if(io_status)return io_status;r[2]=loaded;}goto Lac5fc84a; /* mov.l 0x8c5fc8a0,r2 */
Lac5fc84a: *stop=0xac5fc84au;{uint32_t loaded=rd(m,bus,&io_status,0xac5fc89cu,4);if(io_status)return io_status;r[5]=loaded;}goto Lac5fc84c; /* mov.l 0x8c5fc89c,r5 */
Lac5fc84c: *stop=0xac5fc84cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto Lac5fc84e; /* mov.l @r2,r0 */
Lac5fc84e: *stop=0xac5fc84eu;r[0]>>=2;goto Lac5fc850; /* shlr2 r0 */
Lac5fc850: *stop=0xac5fc850u;r[0]>>=2;goto Lac5fc852; /* shlr2 r0 */
Lac5fc852: *stop=0xac5fc852u;*sr=(*sr&~1u)|(r[0]&1u);r[0]>>=1;goto Lac5fc854; /* shlr r0 */
Lac5fc854: *stop=0xac5fc854u;r[0]&=0x1u;goto Lac5fc856; /* and #0x1,r0 */
Lac5fc856: *stop=0xac5fc856u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto Lac5fc858; /* cmp/eq #0x1,r0 */
Lac5fc858: take=!(*sr&1u);*stop=0xac5fc85au;r[4]=0u;if(take)goto Lac5fc880;goto Lac5fc85c; /* bf/s 0x8c5fc880 */
Lac5fc85c: *stop=0xac5fc85cu;{uint32_t loaded=sx(m,bus,&io_status,0xac5fc894u,2);if(io_status)return io_status;r[7]=loaded;}goto Lac5fc85e; /* mov.w 0x8c5fc894,r7 */
Lac5fc85e: *stop=0xac5fc85eu;r[6]=r[4];goto Lac5fc860; /* mov r4,r6 */
Lac5fc860: *stop=0xac5fc860u;r[2]=r[6];goto Lac5fc862; /* mov r6,r2 */
Lac5fc862: *stop=0xac5fc862u;r[6]+=32u;goto Lac5fc864; /* add #0x20,r6 */
Lac5fc864: *stop=0xac5fc864u;*sr=(*sr&~1u)|(r[6]>=r[7]);goto Lac5fc866; /* cmp/hs r7,r6 */
Lac5fc866: *stop=0xac5fc866u;r[2]|=r[5];goto Lac5fc868; /* or r5,r2 */
Lac5fc868: take=!(*sr&1u);*stop=0xac5fc86au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;if(take)goto Lac5fc860;goto Lac5fc86c; /* bf/s 0x8c5fc860 */
Lac5fc86c: *stop=0xac5fc86cu;{uint32_t loaded=sx(m,bus,&io_status,0xac5fc898u,2);if(io_status)return io_status;r[6]=loaded;}goto Lac5fc86e; /* mov.w 0x8c5fc898,r6 */
Lac5fc86e: *stop=0xac5fc86eu;{uint32_t loaded=sx(m,bus,&io_status,0xac5fc896u,2);if(io_status)return io_status;r[7]=loaded;}goto Lac5fc870; /* mov.w 0x8c5fc896,r7 */
Lac5fc870: *stop=0xac5fc870u;r[2]=r[6];goto Lac5fc872; /* mov r6,r2 */
Lac5fc872: *stop=0xac5fc872u;r[6]+=32u;goto Lac5fc874; /* add #0x20,r6 */
Lac5fc874: *stop=0xac5fc874u;*sr=(*sr&~1u)|(r[6]>=r[7]);goto Lac5fc876; /* cmp/hs r7,r6 */
Lac5fc876: *stop=0xac5fc876u;r[2]|=r[5];goto Lac5fc878; /* or r5,r2 */
Lac5fc878: take=!(*sr&1u);*stop=0xac5fc87au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;if(take)goto Lac5fc870;goto Lac5fc87c; /* bf/s 0x8c5fc870 */
Lac5fc87c: *stop=0xac5fc87eu;goto Lac5fc890; /* bra 0x8c5fc890 */
Lac5fc880: *stop=0xac5fc880u;{uint32_t loaded=sx(m,bus,&io_status,0xac5fc89au,2);if(io_status)return io_status;r[7]=loaded;}goto Lac5fc882; /* mov.w 0x8c5fc89a,r7 */
Lac5fc882: *stop=0xac5fc882u;r[6]=r[4];goto Lac5fc884; /* mov r4,r6 */
Lac5fc884: *stop=0xac5fc884u;r[3]=r[6];goto Lac5fc886; /* mov r6,r3 */
Lac5fc886: *stop=0xac5fc886u;r[6]+=32u;goto Lac5fc888; /* add #0x20,r6 */
Lac5fc888: *stop=0xac5fc888u;*sr=(*sr&~1u)|(r[6]>=r[7]);goto Lac5fc88a; /* cmp/hs r7,r6 */
Lac5fc88a: *stop=0xac5fc88au;r[3]|=r[5];goto Lac5fc88c; /* or r5,r3 */
Lac5fc88c: take=!(*sr&1u);*stop=0xac5fc88eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;if(take)goto Lac5fc884;goto Lac5fc890; /* bf/s 0x8c5fc884 */
Lac5fc890: *stop=0xac5fc892u;goto Lac5fc8a4; /* bra 0x8c5fc8a4 */
Lac5fc8a4: *stop=0xac5fc8a4u;goto Lac5fc8a6; /* nop */
Lac5fc8a6: *stop=0xac5fc8a6u;goto Lac5fc8a8; /* nop */
Lac5fc8a8: *stop=0xac5fc8a8u;goto Lac5fc8aa; /* nop */
Lac5fc8aa: *stop=0xac5fc8aau;goto Lac5fc8ac; /* nop */
Lac5fc8ac: *stop=0xac5fc8acu;goto Lac5fc8ae; /* nop */
Lac5fc8ae: *stop=0xac5fc8aeu;goto Lac5fc8b0; /* nop */
Lac5fc8b0: *stop=0xac5fc8b0u;goto Lac5fc8b2; /* nop */
Lac5fc8b2: *stop=0xac5fc8b2u;goto Lac5fc8b4; /* nop */
Lac5fc8b4: *stop=0xac5fc8b4u;if(!valid_access(r[15],2u))return -3;{uint32_t loaded=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;r[0]=loaded;}goto Lac5fc8b6; /* mov.w @r15,r0 */
Lac5fc8b6: *stop=0xac5fc8b6u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto Lac5fc8b8; /* stc SR,r3 */
Lac5fc8b8: *stop=0xac5fc8b8u;r[0]=r[0]&65535u;goto Lac5fc8ba; /* extu.w r0,r0 */
Lac5fc8ba: *stop=0xac5fc8bau;{uint32_t loaded=sx(m,bus,&io_status,0xac5fc93au,2);if(io_status)return io_status;r[2]=loaded;}goto Lac5fc8bc; /* mov.w 0x8c5fc93a,r2 */
Lac5fc8bc: *stop=0xac5fc8bcu;r[0]&=0xfu;goto Lac5fc8be; /* and #0xf,r0 */
Lac5fc8be: *stop=0xac5fc8beu;r[0]<<=2;goto Lac5fc8c0; /* shll2 r0 */
Lac5fc8c0: *stop=0xac5fc8c0u;r[3]&=r[2];goto Lac5fc8c2; /* and r2,r3 */
Lac5fc8c2: *stop=0xac5fc8c2u;r[0]<<=2;goto Lac5fc8c4; /* shll2 r0 */
Lac5fc8c4: *stop=0xac5fc8c4u;r[0]|=r[3];goto Lac5fc8c6; /* or r3,r0 */
Lac5fc8c6: *stop=0xac5fc8c6u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto Lac5fc8c8; /* ldc r0,SR */
Lac5fc8c8: *stop=0xac5fc8c8u;if(*pr!=entry_pr)return -3;*stop=0xac5fc8cau;r[15]+=4u;return 0; /* rts */
}
