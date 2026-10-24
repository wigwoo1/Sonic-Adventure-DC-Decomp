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
 if(n==4u && !(a&3u) && ((a&0x1fffffffu)==0x005f6920u || (a&0x1fffffffu)==0x005f6924u))return 1;
 if(n==4u && (a==0xffd8000cu || a==0xffa00020u || a==0xffa00024u || a==0xffa00028u || a==0xffa0002cu || a==0xa05f6800u || a==0xa05f6804u || a==0xa05f6808u))return 1;
 if(n==4u && !(a&3u) && (a&0x1fffffffu)>=0x005f8000u && (a&0x1fffffffu)<0x005fa000u)return 1;
 if(a&3u || (n!=4u&&n!=8u))return 0;
 return ((a&0xfc000000u)==0xe0000000u&&((a+n-1u)&0xfc000000u)==0xe0000000u) || (a==0xff000038u || (a==0xff00003cu&&n==4u));
}
SA_STAGE_EXPORT int sa_boot_event_8c605234(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c605234;
L8c605234: *stop=0x8c605234u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c605236; /* mov.l r14,@-r15 */
L8c605236: *stop=0x8c605236u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c605238; /* sts.l PR,@-r15 */
L8c605238: *stop=0x8c605238u;r[15]+=4294967292u;goto L8c60523a; /* add #-0x4,r15 */
L8c60523a: *stop=0x8c60523au;{uint32_t loaded=rd(m,bus,&io_status,0x8c605374u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c60523c; /* mov.l 0x8c605374,r14 */
L8c60523c: *stop=0x8c60523cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c60523e; /* mov.l r4,@r15 */
L8c60523e: *stop=0x8c60523eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}goto L8c605240; /* mov.l @r14,r4 */
L8c605240: *stop=0x8c605240u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c605242; /* tst r4,r4 */
L8c605242: if((*sr&1u))goto L8c60524e;goto L8c605244; /* bt 0x8c60524e */
L8c605244: *stop=0x8c605244u;{uint32_t loaded=rd(m,bus,&io_status,0x8c605378u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c605246; /* mov.l 0x8c605378,r2 */
L8c605246: target=r[2];*pr=0x8c60524au;*stop=0x8c605248u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60524a; /* jsr @r2 */
L8c60524a: *stop=0x8c60524au;r[3]=0u;goto L8c60524c; /* mov #0x0,r3 */
L8c60524c: *stop=0x8c60524cu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8c60524e; /* mov.l r3,@r14 */
L8c60524e: *stop=0x8c60524eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c605250; /* mov.l @r15,r2 */
L8c605250: *stop=0x8c605250u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c605252; /* tst r2,r2 */
L8c605252: if((*sr&1u))goto L8c605262;goto L8c605254; /* bt 0x8c605262 */
L8c605254: *stop=0x8c605254u;r[4]=sx(m,bus,&io_status,0x8c60536eu,2);if(io_status)return io_status;goto L8c605256; /* mov.w 0x8c60536e,r4 */
L8c605256: *stop=0x8c605256u;r[7]=0u;goto L8c605258; /* mov #0x0,r7 */
L8c605258: *stop=0x8c605258u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60537cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60525a; /* mov.l 0x8c60537c,r3 */
L8c60525a: *stop=0x8c60525au;r[6]=88u;goto L8c60525c; /* mov #0x58,r6 */
L8c60525c: target=r[3];*pr=0x8c605260u;*stop=0x8c60525eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c605260; /* jsr @r3 */
L8c605260: *stop=0x8c605260u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;goto L8c605262; /* mov.l r0,@r14 */
L8c605262: *stop=0x8c605262u;r[15]+=4u;goto L8c605264; /* add #0x4,r15 */
L8c605264: *stop=0x8c605264u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c605266; /* lds.l @r15+,PR */
L8c605266: *stop=0x8c605266u;if(*pr!=entry_pr)return -3;*stop=0x8c605268u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_event_8c64594c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c64594c;
L8c64594c: *stop=0x8c64594cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c64594e; /* mov.l r14,@-r15 */
L8c64594e: *stop=0x8c64594eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c645950; /* mov.l r13,@-r15 */
L8c645950: *stop=0x8c645950u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c645952; /* mov.l r12,@-r15 */
L8c645952: *stop=0x8c645952u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c645954; /* mov.l r11,@-r15 */
L8c645954: *stop=0x8c645954u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c645956; /* mov.l r10,@-r15 */
L8c645956: *stop=0x8c645956u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c645958; /* mov.l r9,@-r15 */
L8c645958: *stop=0x8c645958u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c64595a; /* mov.l r8,@-r15 */
L8c64595a: *stop=0x8c64595au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64595c; /* sts.l PR,@-r15 */
L8c64595c: *stop=0x8c64595cu;r[15]+=4294967284u;goto L8c64595e; /* add #-0xc,r15 */
L8c64595e: *stop=0x8c64595eu;r[0]=r[4];goto L8c645960; /* mov r4,r0 */
L8c645960: *stop=0x8c645960u;if(!valid_access(r[15]+4u,2u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[0],2);if(io_status)return io_status;goto L8c645962; /* mov.w r0,@(0x4,r15) */
L8c645962: *stop=0x8c645962u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c645964; /* stc SR,r0 */
L8c645964: *stop=0x8c645964u;r[3]=sx(m,bus,&io_status,0x8c6459fcu,2);if(io_status)return io_status;goto L8c645966; /* mov.w 0x8c6459fc,r3 */
L8c645966: *stop=0x8c645966u;r[8]=r[7];goto L8c645968; /* mov r7,r8 */
L8c645968: *stop=0x8c645968u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c64596a; /* mov.l r5,@r15 */
L8c64596a: *stop=0x8c64596au;r[0]>>=2;goto L8c64596c; /* shlr2 r0 */
L8c64596c: *stop=0x8c64596cu;r[0]>>=2;goto L8c64596e; /* shlr2 r0 */
L8c64596e: *stop=0x8c64596eu;r[0]&=0xfu;goto L8c645970; /* and #0xf,r0 */
L8c645970: *stop=0x8c645970u;if(!valid_access(r[15]+8u,2u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],2);if(io_status)return io_status;goto L8c645972; /* mov.w r0,@(0x8,r15) */
L8c645972: *stop=0x8c645972u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c645974; /* stc SR,r0 */
L8c645974: *stop=0x8c645974u;r[0]&=r[3];goto L8c645976; /* and r3,r0 */
L8c645976: *stop=0x8c645976u;r[0]|=0xf0u;goto L8c645978; /* or #0xf0,r0 */
L8c645978: *stop=0x8c645978u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c64597a; /* ldc r0,SR */
L8c64597a: *stop=0x8c64597au;if(!valid_access(r[15]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;goto L8c64597c; /* mov.w @(0x4,r15),r0 */
L8c64597c: *stop=0x8c64597cu;r[3]=4294967292u;goto L8c64597e; /* mov #-0x4,r3 */
L8c64597e: *stop=0x8c64597eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645a10u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c645980; /* mov.l 0x8c645a10,r9 */
L8c645980: *stop=0x8c645980u;r[11]=r[6];goto L8c645982; /* mov r6,r11 */
L8c645982: *stop=0x8c645982u;r[13]=r[0];goto L8c645984; /* mov r0,r13 */
L8c645984: *stop=0x8c645984u;r[0]=r[13];goto L8c645986; /* mov r13,r0 */
L8c645986: *stop=0x8c645986u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c645988; /* shad r3,r0 */
L8c645988: *stop=0x8c645988u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645a14u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c64598a; /* mov.l 0x8c645a14,r7 */
L8c64598a: *stop=0x8c64598au;r[0]<<=2;goto L8c64598c; /* shll2 r0 */
L8c64598c: *stop=0x8c64598cu;if(!valid_access(r[0]+r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;r[4]=loaded;}goto L8c64598e; /* mov.l @(r0,r9),r4 */
L8c64598e: *stop=0x8c64598eu;r[5]=r[4];goto L8c645990; /* mov r4,r5 */
L8c645990: *stop=0x8c645992u;r[11]|=r[7];goto L8c645998; /* bra 0x8c645998 */
L8c645994: *stop=0x8c645994u;r[5]=r[4];goto L8c645996; /* mov r4,r5 */
L8c645996: *stop=0x8c645996u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c645998; /* mov.l @(0x4,r4),r4 */
L8c645998: *stop=0x8c645998u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64599a; /* tst r4,r4 */
L8c64599a: if((*sr&1u))goto L8c6459a2;goto L8c64599c; /* bt 0x8c6459a2 */
L8c64599c: *stop=0x8c64599cu;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64599e; /* mov.l @(0xc,r4),r2 */
L8c64599e: *stop=0x8c64599eu;*sr=(*sr&~1u)|(r[2]>=r[11]);goto L8c6459a0; /* cmp/hs r11,r2 */
L8c6459a0: if(!(*sr&1u))goto L8c645994;goto L8c6459a2; /* bf 0x8c645994 */
L8c6459a2: *stop=0x8c6459a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645a0cu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c6459a4; /* mov.l 0x8c645a0c,r10 */
L8c6459a4: *stop=0x8c6459a4u;r[12]=1u;goto L8c6459a6; /* mov #0x1,r12 */
L8c6459a6: *stop=0x8c6459a6u;r[6]=sx(m,bus,&io_status,0x8c645a02u,2);if(io_status)return io_status;goto L8c6459a8; /* mov.w 0x8c645a02,r6 */
L8c6459a8: *stop=0x8c6459a8u;r[14]=r[12];goto L8c6459aa; /* mov r12,r14 */
L8c6459aa: *stop=0x8c6459aau;r[2]=r[14];goto L8c6459ac; /* mov r14,r2 */
L8c6459ac: *stop=0x8c6459acu;r[2]<<=2;goto L8c6459ae; /* shll2 r2 */
L8c6459ae: *stop=0x8c6459aeu;r[2]<<=2;goto L8c6459b0; /* shll2 r2 */
L8c6459b0: *stop=0x8c6459b0u;r[2]+=r[10];goto L8c6459b2; /* add r10,r2 */
L8c6459b2: *stop=0x8c6459b2u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6459b4; /* mov.l @(0xc,r2),r3 */
L8c6459b4: *stop=0x8c6459b4u;*sr=(*sr&~1u)|!(r[7]&r[3]);goto L8c6459b6; /* tst r7,r3 */
L8c6459b6: if((*sr&1u))goto L8c6459be;goto L8c6459b8; /* bt 0x8c6459be */
L8c6459b8: *stop=0x8c6459b8u;r[14]+=1u;goto L8c6459ba; /* add #0x1,r14 */
L8c6459ba: *stop=0x8c6459bau;*sr=(*sr&~1u)|(r[14]>=r[6]);goto L8c6459bc; /* cmp/hs r6,r14 */
L8c6459bc: if(!(*sr&1u))goto L8c6459aa;goto L8c6459be; /* bf 0x8c6459aa */
L8c6459be: *stop=0x8c6459beu;*sr=(*sr&~1u)|(r[14]>=r[6]);goto L8c6459c0; /* cmp/hs r6,r14 */
L8c6459c0: if(!(*sr&1u))goto L8c6459c6;goto L8c6459c2; /* bf 0x8c6459c6 */
L8c6459c2: *stop=0x8c6459c4u;goto L8c645ace; /* bra 0x8c645ace */
L8c6459c6: *stop=0x8c6459c6u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6459c8; /* tst r4,r4 */
L8c6459c8: if(!(*sr&1u))goto L8c645a9a;goto L8c6459ca; /* bf 0x8c645a9a */
L8c6459ca: *stop=0x8c6459cau;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c6459cc; /* tst r5,r5 */
L8c6459cc: if(!(*sr&1u))goto L8c645a9a;goto L8c6459ce; /* bf 0x8c645a9a */
L8c6459ce: *stop=0x8c6459ceu;r[4]=sx(m,bus,&io_status,0x8c645a04u,2);if(io_status)return io_status;goto L8c6459d0; /* mov.w 0x8c645a04,r4 */
L8c6459d0: *stop=0x8c6459d0u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[4]);goto L8c6459d2; /* cmp/ge r4,r13 */
L8c6459d2: if((*sr&1u))goto L8c6459ec;goto L8c6459d4; /* bt 0x8c6459ec */
L8c6459d4: *stop=0x8c6459d4u;r[3]=sx(m,bus,&io_status,0x8c645a06u,2);if(io_status)return io_status;goto L8c6459d6; /* mov.w 0x8c645a06,r3 */
L8c6459d6: *stop=0x8c6459d6u;status=sa_sh4_stc_vbr(*vbr,*sr,&r[2],0u);if(status)return status;goto L8c6459d8; /* stc VBR,r2 */
L8c6459d8: *stop=0x8c6459d8u;r[3]+=r[13];goto L8c6459da; /* add r13,r3 */
L8c6459da: *stop=0x8c6459dau;r[0]=sx(m,bus,&io_status,0x8c645a08u,2);if(io_status)return io_status;goto L8c6459dc; /* mov.w 0x8c645a08,r0 */
L8c6459dc: *stop=0x8c6459dcu;*sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u);goto L8c6459de; /* shar r3 */
L8c6459de: *stop=0x8c6459deu;*sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u);goto L8c6459e0; /* shar r3 */
L8c6459e0: *stop=0x8c6459e0u;*sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u);goto L8c6459e2; /* shar r3 */
L8c6459e2: *stop=0x8c6459e2u;r[2]+=r[3];goto L8c6459e4; /* add r3,r2 */
L8c6459e4: *stop=0x8c6459e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645a18u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6459e6; /* mov.l 0x8c645a18,r3 */
L8c6459e6: *stop=0x8c6459e6u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[3],4);if(io_status)return io_status;goto L8c6459e8; /* mov.l r3,@(r0,r2) */
L8c6459e8: *stop=0x8c6459eau;goto L8c645a3e; /* bra 0x8c645a3e */
L8c6459ec: *stop=0x8c6459ecu;*sr=(*sr&~1u)|(r[4]==r[13]);goto L8c6459ee; /* cmp/eq r4,r13 */
L8c6459ee: if(!(*sr&1u))goto L8c645a24;goto L8c6459f0; /* bf 0x8c645a24 */
L8c6459f0: *stop=0x8c6459f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645a20u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6459f2; /* mov.l 0x8c645a20,r2 */
L8c6459f2: *stop=0x8c6459f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645a1cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6459f4; /* mov.l 0x8c645a1c,r4 */
L8c6459f4: target=r[2];*pr=0x8c6459f8u;*stop=0x8c6459f6u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6459f8; /* jsr @r2 */
L8c6459f8: *stop=0x8c6459fau;goto L8c645a3e; /* bra 0x8c645a3e */
L8c645a24: *stop=0x8c645a24u;r[2]=sx(m,bus,&io_status,0x8c645b44u,2);if(io_status)return io_status;goto L8c645a26; /* mov.w 0x8c645b44,r2 */
L8c645a26: *stop=0x8c645a26u;*sr=(*sr&~1u)|(r[2]==r[13]);goto L8c645a28; /* cmp/eq r2,r13 */
L8c645a28: if(!(*sr&1u))goto L8c645a36;goto L8c645a2a; /* bf 0x8c645a36 */
L8c645a2a: *stop=0x8c645a2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c645b50u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c645a2c; /* mov.l 0x8c645b50,r1 */
L8c645a2c: *stop=0x8c645a2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645b4cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c645a2e; /* mov.l 0x8c645b4c,r4 */
L8c645a2e: target=r[1];*pr=0x8c645a32u;*stop=0x8c645a30u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645a32; /* jsr @r1 */
L8c645a32: *stop=0x8c645a34u;goto L8c645a3e; /* bra 0x8c645a3e */
L8c645a36: *stop=0x8c645a36u;status=sa_sh4_stc_vbr(*vbr,*sr,&r[1],0u);if(status)return status;goto L8c645a38; /* stc VBR,r1 */
L8c645a38: *stop=0x8c645a38u;r[0]=sx(m,bus,&io_status,0x8c645b46u,2);if(io_status)return io_status;goto L8c645a3a; /* mov.w 0x8c645b46,r0 */
L8c645a3a: *stop=0x8c645a3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c645b54u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645a3c; /* mov.l 0x8c645b54,r3 */
L8c645a3c: *stop=0x8c645a3cu;if(!valid_access(r[0]+r[1],4u))return -3;wr(m,bus,&io_status,r[1]+r[0],r[3],4);if(io_status)return io_status;goto L8c645a3e; /* mov.l r3,@(r0,r1) */
L8c645a3e: *stop=0x8c645a3eu;r[4]=r[14];goto L8c645a40; /* mov r14,r4 */
L8c645a40: *stop=0x8c645a40u;r[4]<<=2;goto L8c645a42; /* shll2 r4 */
L8c645a42: *stop=0x8c645a42u;r[0]=r[13];goto L8c645a44; /* mov r13,r0 */
L8c645a44: *stop=0x8c645a44u;r[3]=4294967292u;goto L8c645a46; /* mov #-0x4,r3 */
L8c645a46: *stop=0x8c645a46u;r[4]<<=2;goto L8c645a48; /* shll2 r4 */
L8c645a48: *stop=0x8c645a48u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c645a4a; /* shad r3,r0 */
L8c645a4a: *stop=0x8c645a4au;r[4]+=r[10];goto L8c645a4c; /* add r10,r4 */
L8c645a4c: *stop=0x8c645a4cu;r[0]<<=2;goto L8c645a4e; /* shll2 r0 */
L8c645a4e: *stop=0x8c645a4eu;if(!valid_access(r[0]+r[9],4u))return -3;wr(m,bus,&io_status,r[9]+r[0],r[4],4);if(io_status)return io_status;goto L8c645a50; /* mov.l r4,@(r0,r9) */
L8c645a50: *stop=0x8c645a50u;r[3]=0u;goto L8c645a52; /* mov #0x0,r3 */
L8c645a52: *stop=0x8c645a52u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[3],4);if(io_status)return io_status;goto L8c645a54; /* mov.l r3,@(0x4,r4) */
L8c645a54: *stop=0x8c645a54u;r[3]=4294967288u;goto L8c645a56; /* mov #-0x8,r3 */
L8c645a56: *stop=0x8c645a56u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c645a58; /* mov.l @r15,r2 */
L8c645a58: *stop=0x8c645a58u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c645a5a; /* mov.l r2,@r4 */
L8c645a5a: *stop=0x8c645a5au;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[8],4);if(io_status)return io_status;goto L8c645a5c; /* mov.l r8,@(0x8,r4) */
L8c645a5c: *stop=0x8c645a5cu;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[11],4);if(io_status)return io_status;goto L8c645a5e; /* mov.l r11,@(0xc,r4) */
L8c645a5e: *stop=0x8c645a5eu;if(!valid_access(r[15]+4u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x4u,2);if(io_status)return io_status;goto L8c645a60; /* mov.w @(0x4,r15),r0 */
L8c645a60: *stop=0x8c645a60u;r[4]=r[0];goto L8c645a62; /* mov r0,r4 */
L8c645a62: *stop=0x8c645a62u;r[4]=r[4]&255u;goto L8c645a64; /* extu.b r4,r4 */
L8c645a64: *stop=0x8c645a64u;r[0]=r[13];goto L8c645a66; /* mov r13,r0 */
L8c645a66: *stop=0x8c645a66u;r[4]>>=2;goto L8c645a68; /* shlr2 r4 */
L8c645a68: *stop=0x8c645a68u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c645a6a; /* shad r3,r0 */
L8c645a6a: *stop=0x8c645a6au;r[4]>>=2;goto L8c645a6c; /* shlr2 r4 */
L8c645a6c: *stop=0x8c645a6cu;*sr=(*sr&~1u)|(r[4]&1u);r[4]>>=1;goto L8c645a6e; /* shlr r4 */
L8c645a6e: *stop=0x8c645a6eu;*sr=(*sr&~1u)|(r[0]==0x0000000au);goto L8c645a70; /* cmp/eq #0xa,r0 */
L8c645a70: if(!(*sr&1u))goto L8c645a82;goto L8c645a72; /* bf 0x8c645a82 */
L8c645a72: *stop=0x8c645a72u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645b58u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645a74; /* mov.l 0x8c645b58,r3 */
L8c645a74: *stop=0x8c645a74u;r[4]+=12u;goto L8c645a76; /* add #0xc,r4 */
L8c645a76: *stop=0x8c645a76u;{uint32_t shift=r[4];if(!(shift&0x80000000u))r[12]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[12]=n?(uint32_t)((int32_t)r[12]>>n):((r[12]&0x80000000u)?0xffffffffu:0u);}}goto L8c645a78; /* shad r4,r12 */
L8c645a78: *stop=0x8c645a78u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c645a7a; /* mov.l @r3,r2 */
L8c645a7a: *stop=0x8c645a7au;r[2]|=r[12];goto L8c645a7c; /* or r12,r2 */
L8c645a7c: *stop=0x8c645a7cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c645a7e; /* mov.l r2,@r3 */
L8c645a7e: *stop=0x8c645a80u;goto L8c645aca; /* bra 0x8c645aca */
L8c645a82: *stop=0x8c645a82u;r[3]=4294967288u;goto L8c645a84; /* mov #-0x8,r3 */
L8c645a84: *stop=0x8c645a84u;r[0]=r[13];goto L8c645a86; /* mov r13,r0 */
L8c645a86: *stop=0x8c645a86u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c645a88; /* shad r3,r0 */
L8c645a88: *stop=0x8c645a88u;*sr=(*sr&~1u)|(r[0]==0x0000000bu);goto L8c645a8a; /* cmp/eq #0xb,r0 */
L8c645a8a: if(!(*sr&1u))goto L8c645aca;goto L8c645a8c; /* bf 0x8c645aca */
L8c645a8c: *stop=0x8c645a8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645b5cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645a8e; /* mov.l 0x8c645b5c,r3 */
L8c645a8e: *stop=0x8c645a8eu;{uint32_t shift=r[4];if(!(shift&0x80000000u))r[12]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[12]=n?(uint32_t)((int32_t)r[12]>>n):((r[12]&0x80000000u)?0xffffffffu:0u);}}goto L8c645a90; /* shad r4,r12 */
L8c645a90: *stop=0x8c645a90u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c645a92; /* mov.l @r3,r2 */
L8c645a92: *stop=0x8c645a92u;r[2]|=r[12];goto L8c645a94; /* or r12,r2 */
L8c645a94: *stop=0x8c645a94u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c645a96; /* mov.l r2,@r3 */
L8c645a96: *stop=0x8c645a98u;goto L8c645aca; /* bra 0x8c645aca */
L8c645a9a: *stop=0x8c645a9au;r[0]=r[13];goto L8c645a9c; /* mov r13,r0 */
L8c645a9c: *stop=0x8c645a9cu;r[3]=4294967292u;goto L8c645a9e; /* mov #-0x4,r3 */
L8c645a9e: *stop=0x8c645a9eu;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c645aa0; /* shad r3,r0 */
L8c645aa0: *stop=0x8c645aa0u;r[12]=r[14];goto L8c645aa2; /* mov r14,r12 */
L8c645aa2: *stop=0x8c645aa2u;r[0]<<=2;goto L8c645aa4; /* shll2 r0 */
L8c645aa4: *stop=0x8c645aa4u;if(!valid_access(r[0]+r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c645aa6; /* mov.l @(r0,r9),r2 */
L8c645aa6: *stop=0x8c645aa6u;r[12]<<=2;goto L8c645aa8; /* shll2 r12 */
L8c645aa8: *stop=0x8c645aa8u;r[12]<<=2;goto L8c645aaa; /* shll2 r12 */
L8c645aaa: *stop=0x8c645aaau;*sr=(*sr&~1u)|(r[2]==r[4]);goto L8c645aac; /* cmp/eq r2,r4 */
L8c645aac: take=!(*sr&1u);*stop=0x8c645aaeu;r[12]+=r[10];if(take)goto L8c645abe;goto L8c645ab0; /* bf/s 0x8c645abe */
L8c645ab0: *stop=0x8c645ab0u;r[3]=4294967292u;goto L8c645ab2; /* mov #-0x4,r3 */
L8c645ab2: *stop=0x8c645ab2u;r[0]=r[13];goto L8c645ab4; /* mov r13,r0 */
L8c645ab4: *stop=0x8c645ab4u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c645ab6; /* shad r3,r0 */
L8c645ab6: *stop=0x8c645ab6u;if(!valid_access(r[12]+4u,4u))return -3;wr(m,bus,&io_status,r[12]+0x4u,r[4],4);if(io_status)return io_status;goto L8c645ab8; /* mov.l r4,@(0x4,r12) */
L8c645ab8: *stop=0x8c645ab8u;r[0]<<=2;goto L8c645aba; /* shll2 r0 */
L8c645aba: *stop=0x8c645abcu;if(!valid_access(r[0]+r[9],4u))return -3;wr(m,bus,&io_status,r[9]+r[0],r[12],4);if(io_status)return io_status;goto L8c645ac2; /* bra 0x8c645ac2 */
L8c645abe: *stop=0x8c645abeu;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[12],4);if(io_status)return io_status;goto L8c645ac0; /* mov.l r12,@(0x4,r5) */
L8c645ac0: *stop=0x8c645ac0u;if(!valid_access(r[12]+4u,4u))return -3;wr(m,bus,&io_status,r[12]+0x4u,r[4],4);if(io_status)return io_status;goto L8c645ac2; /* mov.l r4,@(0x4,r12) */
L8c645ac2: *stop=0x8c645ac2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c645ac4; /* mov.l @r15,r2 */
L8c645ac4: *stop=0x8c645ac4u;if(!valid_access(r[12],4u))return -3;wr(m,bus,&io_status,r[12],r[2],4);if(io_status)return io_status;goto L8c645ac6; /* mov.l r2,@r12 */
L8c645ac6: *stop=0x8c645ac6u;if(!valid_access(r[12]+8u,4u))return -3;wr(m,bus,&io_status,r[12]+0x8u,r[8],4);if(io_status)return io_status;goto L8c645ac8; /* mov.l r8,@(0x8,r12) */
L8c645ac8: *stop=0x8c645ac8u;if(!valid_access(r[12]+12u,4u))return -3;wr(m,bus,&io_status,r[12]+0xcu,r[11],4);if(io_status)return io_status;goto L8c645aca; /* mov.l r11,@(0xc,r12) */
L8c645aca: *stop=0x8c645accu;r[4]=r[14];goto L8c645ad0; /* bra 0x8c645ad0 */
L8c645ace: *stop=0x8c645aceu;r[4]=0u;goto L8c645ad0; /* mov #0x0,r4 */
L8c645ad0: *stop=0x8c645ad0u;if(!valid_access(r[15]+8u,2u))return -3;r[0]=sx(m,bus,&io_status,r[15]+0x8u,2);if(io_status)return io_status;goto L8c645ad2; /* mov.w @(0x8,r15),r0 */
L8c645ad2: *stop=0x8c645ad2u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c645ad4; /* stc SR,r3 */
L8c645ad4: *stop=0x8c645ad4u;r[0]=r[0]&65535u;goto L8c645ad6; /* extu.w r0,r0 */
L8c645ad6: *stop=0x8c645ad6u;r[2]=sx(m,bus,&io_status,0x8c645b48u,2);if(io_status)return io_status;goto L8c645ad8; /* mov.w 0x8c645b48,r2 */
L8c645ad8: *stop=0x8c645ad8u;r[0]&=0xfu;goto L8c645ada; /* and #0xf,r0 */
L8c645ada: *stop=0x8c645adau;r[0]<<=2;goto L8c645adc; /* shll2 r0 */
L8c645adc: *stop=0x8c645adcu;r[3]&=r[2];goto L8c645ade; /* and r2,r3 */
L8c645ade: *stop=0x8c645adeu;r[0]<<=2;goto L8c645ae0; /* shll2 r0 */
L8c645ae0: *stop=0x8c645ae0u;r[0]|=r[3];goto L8c645ae2; /* or r3,r0 */
L8c645ae2: *stop=0x8c645ae2u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c645ae4; /* ldc r0,SR */
L8c645ae4: *stop=0x8c645ae4u;r[0]=r[4];goto L8c645ae6; /* mov r4,r0 */
L8c645ae6: *stop=0x8c645ae6u;r[15]+=12u;goto L8c645ae8; /* add #0xc,r15 */
L8c645ae8: *stop=0x8c645ae8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c645aea; /* lds.l @r15+,PR */
L8c645aea: *stop=0x8c645aeau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c645aec; /* mov.l @r15+,r8 */
L8c645aec: *stop=0x8c645aecu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c645aee; /* mov.l @r15+,r9 */
L8c645aee: *stop=0x8c645aeeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c645af0; /* mov.l @r15+,r10 */
L8c645af0: *stop=0x8c645af0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c645af2; /* mov.l @r15+,r11 */
L8c645af2: *stop=0x8c645af2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c645af4; /* mov.l @r15+,r12 */
L8c645af4: *stop=0x8c645af4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c645af6; /* mov.l @r15+,r13 */
L8c645af6: *stop=0x8c645af6u;if(*pr!=entry_pr)return -3;*stop=0x8c645af8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_event_8c645c54(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c645c54;
L8c645c54: *stop=0x8c645c54u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c645c56; /* mov.l r14,@-r15 */
L8c645c56: *stop=0x8c645c56u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c645c58; /* mov.l r13,@-r15 */
L8c645c58: *stop=0x8c645c58u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c645c5a; /* mov.l r12,@-r15 */
L8c645c5a: *stop=0x8c645c5au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c645c5c; /* mov.l r10,@-r15 */
L8c645c5c: *stop=0x8c645c5cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c645c5e; /* mov.l r9,@-r15 */
L8c645c5e: *stop=0x8c645c5eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c645c60; /* mov.l r8,@-r15 */
L8c645c60: *stop=0x8c645c60u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c645c62; /* sts.l PR,@-r15 */
L8c645c62: *stop=0x8c645c62u;r[15]+=4294967292u;goto L8c645c64; /* add #-0x4,r15 */
L8c645c64: *stop=0x8c645c64u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c645c66; /* stc SR,r0 */
L8c645c66: *stop=0x8c645c66u;r[3]=sx(m,bus,&io_status,0x8c645d60u,2);if(io_status)return io_status;goto L8c645c68; /* mov.w 0x8c645d60,r3 */
L8c645c68: *stop=0x8c645c68u;r[0]>>=2;goto L8c645c6a; /* shlr2 r0 */
L8c645c6a: *stop=0x8c645c6au;r[0]>>=2;goto L8c645c6c; /* shlr2 r0 */
L8c645c6c: *stop=0x8c645c6cu;r[0]&=0xfu;goto L8c645c6e; /* and #0xf,r0 */
L8c645c6e: *stop=0x8c645c6eu;if(!valid_access(r[15],2u))return -3;wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c645c70; /* mov.w r0,@r15 */
L8c645c70: *stop=0x8c645c70u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c645c72; /* stc SR,r0 */
L8c645c72: *stop=0x8c645c72u;r[0]&=r[3];goto L8c645c74; /* and r3,r0 */
L8c645c74: *stop=0x8c645c74u;r[0]|=0xf0u;goto L8c645c76; /* or #0xf0,r0 */
L8c645c76: *stop=0x8c645c76u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c645c78; /* ldc r0,SR */
L8c645c78: *stop=0x8c645c78u;r[12]=r[4];goto L8c645c7a; /* mov r4,r12 */
L8c645c7a: *stop=0x8c645c7au;r[12]<<=2;goto L8c645c7c; /* shll2 r12 */
L8c645c7c: *stop=0x8c645c7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645d70u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c645c7e; /* mov.l 0x8c645d70,r7 */
L8c645c7e: *stop=0x8c645c7eu;r[12]<<=2;goto L8c645c80; /* shll2 r12 */
L8c645c80: *stop=0x8c645c80u;r[13]=r[12];goto L8c645c82; /* mov r12,r13 */
L8c645c82: *stop=0x8c645c82u;r[4]=sx(m,bus,&io_status,0x8c645d62u,2);if(io_status)return io_status;goto L8c645c84; /* mov.w 0x8c645d62,r4 */
L8c645c84: *stop=0x8c645c84u;r[5]=1u;goto L8c645c86; /* mov #0x1,r5 */
L8c645c86: *stop=0x8c645c86u;r[6]=r[5];goto L8c645c88; /* mov r5,r6 */
L8c645c88: *stop=0x8c645c88u;r[13]+=r[7];goto L8c645c8a; /* add r7,r13 */
L8c645c8a: *stop=0x8c645c8au;r[14]=r[6];goto L8c645c8c; /* mov r6,r14 */
L8c645c8c: *stop=0x8c645c8cu;r[14]<<=2;goto L8c645c8e; /* shll2 r14 */
L8c645c8e: *stop=0x8c645c8eu;r[14]<<=2;goto L8c645c90; /* shll2 r14 */
L8c645c90: *stop=0x8c645c90u;r[14]+=r[7];goto L8c645c92; /* add r7,r14 */
L8c645c92: *stop=0x8c645c92u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645c94; /* mov.l @(0x4,r14),r3 */
L8c645c94: *stop=0x8c645c94u;*sr=(*sr&~1u)|(r[13]==r[3]);goto L8c645c96; /* cmp/eq r13,r3 */
L8c645c96: if(!(*sr&1u))goto L8c645ca0;goto L8c645c98; /* bf 0x8c645ca0 */
L8c645c98: *stop=0x8c645c98u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645c9a; /* mov.l @(0xc,r14),r2 */
L8c645c9a: *stop=0x8c645c9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c645d74u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645c9c; /* mov.l 0x8c645d74,r3 */
L8c645c9c: *stop=0x8c645c9cu;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c645c9e; /* tst r3,r2 */
L8c645c9e: if(!(*sr&1u))goto L8c645ca6;goto L8c645ca0; /* bf 0x8c645ca6 */
L8c645ca0: *stop=0x8c645ca0u;r[6]+=1u;goto L8c645ca2; /* add #0x1,r6 */
L8c645ca2: *stop=0x8c645ca2u;*sr=(*sr&~1u)|(r[6]>=r[4]);goto L8c645ca4; /* cmp/hs r4,r6 */
L8c645ca4: if(!(*sr&1u))goto L8c645c8a;goto L8c645ca6; /* bf 0x8c645c8a */
L8c645ca6: *stop=0x8c645ca6u;*sr=(*sr&~1u)|(r[4]==r[6]);goto L8c645ca8; /* cmp/eq r4,r6 */
L8c645ca8: take=(*sr&1u);*stop=0x8c645caau;r[1]=0u;if(take)goto L8c645cbe;goto L8c645cac; /* bt/s 0x8c645cbe */
L8c645cac: *stop=0x8c645cacu;r[4]=r[12];goto L8c645cae; /* mov r12,r4 */
L8c645cae: *stop=0x8c645caeu;r[6]<<=2;goto L8c645cb0; /* shll2 r6 */
L8c645cb0: *stop=0x8c645cb0u;r[4]+=r[7];goto L8c645cb2; /* add r7,r4 */
L8c645cb2: *stop=0x8c645cb2u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645cb4; /* mov.l @(0x4,r4),r3 */
L8c645cb4: *stop=0x8c645cb4u;r[6]<<=2;goto L8c645cb6; /* shll2 r6 */
L8c645cb6: *stop=0x8c645cb6u;r[6]+=r[7];goto L8c645cb8; /* add r7,r6 */
L8c645cb8: *stop=0x8c645cb8u;if(!valid_access(r[6]+4u,4u))return -3;wr(m,bus,&io_status,r[6]+0x4u,r[3],4);if(io_status)return io_status;goto L8c645cba; /* mov.l r3,@(0x4,r6) */
L8c645cba: *stop=0x8c645cbcu;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[1],4);if(io_status)return io_status;goto L8c645d98; /* bra 0x8c645d98 */
L8c645cbe: *stop=0x8c645cbeu;r[13]=r[12];goto L8c645cc0; /* mov r12,r13 */
L8c645cc0: *stop=0x8c645cc0u;r[13]+=r[7];goto L8c645cc2; /* add r7,r13 */
L8c645cc2: *stop=0x8c645cc2u;r[7]=sx(m,bus,&io_status,0x8c645d68u,2);if(io_status)return io_status;goto L8c645cc4; /* mov.w 0x8c645d68,r7 */
L8c645cc4: *stop=0x8c645cc4u;r[14]=r[5];goto L8c645cc6; /* mov r5,r14 */
L8c645cc6: *stop=0x8c645cc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645d7cu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c645cc8; /* mov.l 0x8c645d7c,r10 */
L8c645cc8: *stop=0x8c645cc8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645d80u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c645cca; /* mov.l 0x8c645d80,r4 */
L8c645cca: *stop=0x8c645ccau;{uint32_t loaded=rd(m,bus,&io_status,0x8c645d78u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c645ccc; /* mov.l 0x8c645d78,r9 */
L8c645ccc: *stop=0x8c645cccu;r[12]=sx(m,bus,&io_status,0x8c645d64u,2);if(io_status)return io_status;goto L8c645cce; /* mov.w 0x8c645d64,r12 */
L8c645cce: *stop=0x8c645cceu;r[8]=sx(m,bus,&io_status,0x8c645d66u,2);if(io_status)return io_status;goto L8c645cd0; /* mov.w 0x8c645d66,r8 */
L8c645cd0: *stop=0x8c645cd0u;r[0]=r[14];goto L8c645cd2; /* mov r14,r0 */
L8c645cd2: *stop=0x8c645cd2u;r[0]<<=2;goto L8c645cd4; /* shll2 r0 */
L8c645cd4: *stop=0x8c645cd4u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c645cd6; /* mov.l @(r0,r4),r3 */
L8c645cd6: *stop=0x8c645cd6u;*sr=(*sr&~1u)|(r[13]==r[3]);goto L8c645cd8; /* cmp/eq r13,r3 */
L8c645cd8: if(!(*sr&1u))goto L8c645d90;goto L8c645cda; /* bf 0x8c645d90 */
L8c645cda: *stop=0x8c645cdau;r[6]=r[14];goto L8c645cdc; /* mov r14,r6 */
L8c645cdc: *stop=0x8c645cdcu;r[6]<<=2;goto L8c645cde; /* shll2 r6 */
L8c645cde: *stop=0x8c645cdeu;r[6]+=r[4];goto L8c645ce0; /* add r4,r6 */
L8c645ce0: *stop=0x8c645ce0u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645ce2; /* mov.l @(0x4,r13),r3 */
L8c645ce2: *stop=0x8c645ce2u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c645ce4; /* mov.l r3,@r6 */
L8c645ce4: *stop=0x8c645ce4u;if(!valid_access(r[13]+12u,4u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[1],4);if(io_status)return io_status;goto L8c645ce6; /* mov.l r1,@(0xc,r13) */
L8c645ce6: *stop=0x8c645ce6u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c645ce8; /* mov.l @r6,r2 */
L8c645ce8: *stop=0x8c645ce8u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c645cea; /* tst r2,r2 */
L8c645cea: if(!(*sr&1u))goto L8c645d96;goto L8c645cec; /* bf 0x8c645d96 */
L8c645cec: *stop=0x8c645cecu;*sr=(*sr&~1u)|(r[7]==r[14]);goto L8c645cee; /* cmp/eq r7,r14 */
L8c645cee: if(!(*sr&1u))goto L8c645cfc;goto L8c645cf0; /* bf 0x8c645cfc */
L8c645cf0: *stop=0x8c645cf0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645d84u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645cf2; /* mov.l 0x8c645d84,r2 */
L8c645cf2: *stop=0x8c645cf2u;r[5]=0u;goto L8c645cf4; /* mov #0x0,r5 */
L8c645cf4: target=r[2];*pr=0x8c645cf8u;*stop=0x8c645cf6u;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645cf8; /* jsr @r2 */
L8c645cf8: *stop=0x8c645cfau;goto L8c645d96; /* bra 0x8c645d96 */
L8c645cfc: *stop=0x8c645cfcu;r[2]=sx(m,bus,&io_status,0x8c645d6au,2);if(io_status)return io_status;goto L8c645cfe; /* mov.w 0x8c645d6a,r2 */
L8c645cfe: *stop=0x8c645cfeu;*sr=(*sr&~1u)|(r[2]==r[14]);goto L8c645d00; /* cmp/eq r2,r14 */
L8c645d00: if(!(*sr&1u))goto L8c645d0e;goto L8c645d02; /* bf 0x8c645d0e */
L8c645d02: *stop=0x8c645d02u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645d88u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c645d04; /* mov.l 0x8c645d88,r1 */
L8c645d04: *stop=0x8c645d04u;r[5]=0u;goto L8c645d06; /* mov #0x0,r5 */
L8c645d06: target=r[1];*pr=0x8c645d0au;*stop=0x8c645d08u;r[4]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645d0a; /* jsr @r1 */
L8c645d0a: *stop=0x8c645d0cu;goto L8c645d96; /* bra 0x8c645d96 */
L8c645d0e: *stop=0x8c645d0eu;r[13]=r[14];goto L8c645d10; /* mov r14,r13 */
L8c645d10: *stop=0x8c645d10u;r[13]<<=2;goto L8c645d12; /* shll2 r13 */
L8c645d12: *stop=0x8c645d12u;r[13]<<=2;goto L8c645d14; /* shll2 r13 */
L8c645d14: *stop=0x8c645d14u;r[4]=r[13];goto L8c645d16; /* mov r13,r4 */
L8c645d16: *stop=0x8c645d16u;r[7]=r[4]&255u;goto L8c645d18; /* extu.b r4,r7 */
L8c645d18: *stop=0x8c645d18u;r[3]=4294967288u;goto L8c645d1a; /* mov #-0x8,r3 */
L8c645d1a: *stop=0x8c645d1au;r[6]=(uint32_t)(int32_t)(int16_t)r[4];goto L8c645d1c; /* exts.w r4,r6 */
L8c645d1c: *stop=0x8c645d1cu;r[7]>>=2;goto L8c645d1e; /* shlr2 r7 */
L8c645d1e: *stop=0x8c645d1eu;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[6]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[6]=n?(uint32_t)((int32_t)r[6]>>n):((r[6]&0x80000000u)?0xffffffffu:0u);}}goto L8c645d20; /* shad r3,r6 */
L8c645d20: *stop=0x8c645d20u;r[0]=r[6];goto L8c645d22; /* mov r6,r0 */
L8c645d22: *stop=0x8c645d22u;r[7]>>=2;goto L8c645d24; /* shlr2 r7 */
L8c645d24: *stop=0x8c645d24u;*sr=(*sr&~1u)|(r[7]&1u);r[7]>>=1;goto L8c645d26; /* shlr r7 */
L8c645d26: *stop=0x8c645d26u;*sr=(*sr&~1u)|(r[0]==0x0000000au);goto L8c645d28; /* cmp/eq #0xa,r0 */
L8c645d28: if(!(*sr&1u))goto L8c645d3a;goto L8c645d2a; /* bf 0x8c645d3a */
L8c645d2a: *stop=0x8c645d2au;r[3]=r[5];goto L8c645d2c; /* mov r5,r3 */
L8c645d2c: *stop=0x8c645d2cu;r[7]+=12u;goto L8c645d2e; /* add #0xc,r7 */
L8c645d2e: *stop=0x8c645d2eu;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[1]=loaded;}goto L8c645d30; /* mov.l @r10,r1 */
L8c645d30: *stop=0x8c645d30u;{uint32_t shift=r[7];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}}goto L8c645d32; /* shad r7,r3 */
L8c645d32: *stop=0x8c645d32u;r[3]=~r[3];goto L8c645d34; /* not r3,r3 */
L8c645d34: *stop=0x8c645d34u;r[1]&=r[3];goto L8c645d36; /* and r3,r1 */
L8c645d36: *stop=0x8c645d38u;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[1],4);if(io_status)return io_status;goto L8c645d4a; /* bra 0x8c645d4a */
L8c645d3a: *stop=0x8c645d3au;*sr=(*sr&~1u)|(r[0]==0x0000000bu);goto L8c645d3c; /* cmp/eq #0xb,r0 */
L8c645d3c: if(!(*sr&1u))goto L8c645d4a;goto L8c645d3e; /* bf 0x8c645d4a */
L8c645d3e: *stop=0x8c645d3eu;r[3]=r[5];goto L8c645d40; /* mov r5,r3 */
L8c645d40: *stop=0x8c645d40u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[1]=loaded;}goto L8c645d42; /* mov.l @r9,r1 */
L8c645d42: *stop=0x8c645d42u;{uint32_t shift=r[7];if(!(shift&0x80000000u))r[3]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[3]=n?(uint32_t)((int32_t)r[3]>>n):((r[3]&0x80000000u)?0xffffffffu:0u);}}goto L8c645d44; /* shad r7,r3 */
L8c645d44: *stop=0x8c645d44u;r[3]=~r[3];goto L8c645d46; /* not r3,r3 */
L8c645d46: *stop=0x8c645d46u;r[1]&=r[3];goto L8c645d48; /* and r3,r1 */
L8c645d48: *stop=0x8c645d48u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[1],4);if(io_status)return io_status;goto L8c645d4a; /* mov.l r1,@r9 */
L8c645d4a: *stop=0x8c645d4au;r[2]=sx(m,bus,&io_status,0x8c645d6cu,2);if(io_status)return io_status;goto L8c645d4c; /* mov.w 0x8c645d6c,r2 */
L8c645d4c: *stop=0x8c645d4cu;status=sa_sh4_stc_vbr(*vbr,*sr,&r[3],0u);if(status)return status;goto L8c645d4e; /* stc VBR,r3 */
L8c645d4e: *stop=0x8c645d4eu;r[13]+=r[2];goto L8c645d50; /* add r2,r13 */
L8c645d50: *stop=0x8c645d50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645d8cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c645d52; /* mov.l 0x8c645d8c,r0 */
L8c645d52: *stop=0x8c645d52u;r[13]>>=2;goto L8c645d54; /* shlr2 r13 */
L8c645d54: *stop=0x8c645d54u;*sr=(*sr&~1u)|(r[13]&1u);r[13]>>=1;goto L8c645d56; /* shlr r13 */
L8c645d56: *stop=0x8c645d56u;r[3]+=r[13];goto L8c645d58; /* add r13,r3 */
L8c645d58: *stop=0x8c645d58u;r[3]+=r[8];goto L8c645d5a; /* add r8,r3 */
L8c645d5a: *stop=0x8c645d5au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c645d5c; /* mov.l r0,@r3 */
L8c645d5c: *stop=0x8c645d5eu;goto L8c645d96; /* bra 0x8c645d96 */
L8c645d90: *stop=0x8c645d90u;r[14]+=1u;goto L8c645d92; /* add #0x1,r14 */
L8c645d92: *stop=0x8c645d92u;*sr=(*sr&~1u)|(r[14]>=r[12]);goto L8c645d94; /* cmp/hs r12,r14 */
L8c645d94: if(!(*sr&1u))goto L8c645cd0;goto L8c645d96; /* bf 0x8c645cd0 */
L8c645d96: *stop=0x8c645d96u;*sr=(*sr&~1u)|(r[12]==r[14]);goto L8c645d98; /* cmp/eq r12,r14 */
L8c645d98: *stop=0x8c645d98u;if(!valid_access(r[15],2u))return -3;r[0]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c645d9a; /* mov.w @r15,r0 */
L8c645d9a: *stop=0x8c645d9au;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c645d9c; /* stc SR,r3 */
L8c645d9c: *stop=0x8c645d9cu;r[0]=r[0]&65535u;goto L8c645d9e; /* extu.w r0,r0 */
L8c645d9e: *stop=0x8c645d9eu;r[2]=sx(m,bus,&io_status,0x8c645dbeu,2);if(io_status)return io_status;goto L8c645da0; /* mov.w 0x8c645dbe,r2 */
L8c645da0: *stop=0x8c645da0u;r[0]&=0xfu;goto L8c645da2; /* and #0xf,r0 */
L8c645da2: *stop=0x8c645da2u;r[0]<<=2;goto L8c645da4; /* shll2 r0 */
L8c645da4: *stop=0x8c645da4u;r[3]&=r[2];goto L8c645da6; /* and r2,r3 */
L8c645da6: *stop=0x8c645da6u;r[0]<<=2;goto L8c645da8; /* shll2 r0 */
L8c645da8: *stop=0x8c645da8u;r[0]|=r[3];goto L8c645daa; /* or r3,r0 */
L8c645daa: *stop=0x8c645daau;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c645dac; /* ldc r0,SR */
L8c645dac: *stop=0x8c645dacu;r[15]+=4u;goto L8c645dae; /* add #0x4,r15 */
L8c645dae: *stop=0x8c645daeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c645db0; /* lds.l @r15+,PR */
L8c645db0: *stop=0x8c645db0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c645db2; /* mov.l @r15+,r8 */
L8c645db2: *stop=0x8c645db2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c645db4; /* mov.l @r15+,r9 */
L8c645db4: *stop=0x8c645db4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c645db6; /* mov.l @r15+,r10 */
L8c645db6: *stop=0x8c645db6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c645db8; /* mov.l @r15+,r12 */
L8c645db8: *stop=0x8c645db8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c645dba; /* mov.l @r15+,r13 */
L8c645dba: *stop=0x8c645dbau;if(*pr!=entry_pr)return -3;*stop=0x8c645dbcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_event_8c64135a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64135a;
L8c64135a: *stop=0x8c64135au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64135c; /* sts.l PR,@-r15 */
L8c64135c: *stop=0x8c64135cu;r[15]+=4294967288u;goto L8c64135e; /* add #-0x8,r15 */
L8c64135e: *stop=0x8c64135eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6413c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641360; /* mov.l 0x8c6413c8,r3 */
L8c641360: *stop=0x8c641360u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c641362; /* mov.l r4,@r15 */
L8c641362: *stop=0x8c641362u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c641364; /* mov.l r5,@(0x4,r15) */
L8c641364: target=r[3];*pr=0x8c641368u;*stop=0x8c641366u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641368; /* jsr @r3 */
L8c641368: *stop=0x8c641368u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6413c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64136a; /* mov.l 0x8c6413c4,r3 */
L8c64136a: *stop=0x8c64136au;r[5]=1u;goto L8c64136c; /* mov #0x1,r5 */
L8c64136c: *stop=0x8c64136cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c64136e; /* mov.l @r15,r6 */
L8c64136e: *stop=0x8c64136eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c641370; /* mov.l @(0x4,r15),r7 */
L8c641370: target=0x8c6412c2u;*pr=0x8c641374u;*stop=0x8c641372u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641374; /* bsr 0x8c6412c2 */
L8c641374: *stop=0x8c641374u;r[15]+=8u;goto L8c641376; /* add #0x8,r15 */
L8c641376: *stop=0x8c641376u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c641378; /* lds.l @r15+,PR */
L8c641378: *stop=0x8c641378u;if(*pr!=entry_pr)return -3;*stop=0x8c64137au;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_event_8c64137c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64137c;
L8c64137c: *stop=0x8c64137cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64137e; /* sts.l PR,@-r15 */
L8c64137e: *stop=0x8c64137eu;r[15]+=4294967288u;goto L8c641380; /* add #-0x8,r15 */
L8c641380: *stop=0x8c641380u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c641382; /* mov.l r4,@r15 */
L8c641382: *stop=0x8c641382u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6413ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641384; /* mov.l 0x8c6413cc,r3 */
L8c641384: *stop=0x8c641384u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c641386; /* mov.l r5,@(0x4,r15) */
L8c641386: *stop=0x8c641386u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c641388; /* mov.l @r15,r4 */
L8c641388: *stop=0x8c641388u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c64138a; /* tst r4,r4 */
L8c64138a: *stop=0x8c64138au;r[4]=*sr&1u;goto L8c64138c; /* movt r4 */
L8c64138c: *stop=0x8c64138cu;r[4]+=4294967295u;goto L8c64138e; /* add #-0x1,r4 */
L8c64138e: target=r[3];*pr=0x8c641392u;*stop=0x8c641390u;r[4]=0u-r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641392; /* jsr @r3 */
L8c641392: *stop=0x8c641392u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6413c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641394; /* mov.l 0x8c6413c4,r3 */
L8c641394: *stop=0x8c641394u;r[5]=2u;goto L8c641396; /* mov #0x2,r5 */
L8c641396: *stop=0x8c641396u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[6]=loaded;}goto L8c641398; /* mov.l @r15,r6 */
L8c641398: *stop=0x8c641398u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c64139a; /* mov.l @(0x4,r15),r7 */
L8c64139a: target=0x8c6412c2u;*pr=0x8c64139eu;*stop=0x8c64139cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64139e; /* bsr 0x8c6412c2 */
L8c64139e: *stop=0x8c64139eu;r[15]+=8u;goto L8c6413a0; /* add #0x8,r15 */
L8c6413a0: *stop=0x8c6413a0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6413a2; /* lds.l @r15+,PR */
L8c6413a2: *stop=0x8c6413a2u;if(*pr!=entry_pr)return -3;*stop=0x8c6413a4u;r[0]=0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_event_8c658c20(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c658c20;
L8c658c20: *stop=0x8c658c20u;if(*pr!=entry_pr)return -3;*stop=0x8c658c22u;r[0]=1u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_event_8c658c24(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c658c24;
L8c658c24: *stop=0x8c658c24u;if(*pr!=entry_pr)return -3;*stop=0x8c658c26u;r[0]=1u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_event_8c6412c2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6412c2;
L8c6412c2: *stop=0x8c6412c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6412c4; /* sts.l PR,@-r15 */
L8c6412c4: *stop=0x8c6412c4u;r[15]+=4294967288u;goto L8c6412c6; /* add #-0x8,r15 */
L8c6412c6: *stop=0x8c6412c6u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c6412c8; /* stc SR,r0 */
L8c6412c8: *stop=0x8c6412c8u;r[3]=sx(m,bus,&io_status,0x8c641308u,2);if(io_status)return io_status;goto L8c6412ca; /* mov.w 0x8c641308,r3 */
L8c6412ca: *stop=0x8c6412cau;r[0]>>=2;goto L8c6412cc; /* shlr2 r0 */
L8c6412cc: *stop=0x8c6412ccu;r[0]>>=2;goto L8c6412ce; /* shlr2 r0 */
L8c6412ce: *stop=0x8c6412ceu;r[0]&=0xfu;goto L8c6412d0; /* and #0xf,r0 */
L8c6412d0: *stop=0x8c6412d0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c6412d2; /* mov.l r0,@r15 */
L8c6412d2: *stop=0x8c6412d2u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c6412d4; /* stc SR,r0 */
L8c6412d4: *stop=0x8c6412d4u;r[0]&=r[3];goto L8c6412d6; /* and r3,r0 */
L8c6412d6: *stop=0x8c6412d6u;r[0]|=0xf0u;goto L8c6412d8; /* or #0xf0,r0 */
L8c6412d8: *stop=0x8c6412d8u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c6412da; /* ldc r0,SR */
L8c6412da: *stop=0x8c6412dau;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c6412dc; /* tst r6,r6 */
L8c6412dc: if((*sr&1u))goto L8c641320;goto L8c6412de; /* bt 0x8c641320 */
L8c6412de: *stop=0x8c6412deu;r[3]=sx(m,bus,&io_status,0x8c64130au,2);if(io_status)return io_status;goto L8c6412e0; /* mov.w 0x8c64130a,r3 */
L8c6412e0: *stop=0x8c6412e0u;r[5]<<=2;goto L8c6412e2; /* shll2 r5 */
L8c6412e2: *stop=0x8c6412e2u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6412e4; /* shll r5 */
L8c6412e4: *stop=0x8c6412e4u;r[2]=sx(m,bus,&io_status,0x8c64130au,2);if(io_status)return io_status;goto L8c6412e6; /* mov.w 0x8c64130a,r2 */
L8c6412e6: *stop=0x8c6412e6u;r[3]+=r[4];goto L8c6412e8; /* add r4,r3 */
L8c6412e8: *stop=0x8c6412e8u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c6412ea; /* mov.l r5,@(0x4,r15) */
L8c6412ea: *stop=0x8c6412eau;r[5]+=r[3];goto L8c6412ec; /* add r3,r5 */
L8c6412ec: *stop=0x8c6412ecu;r[2]+=r[4];goto L8c6412ee; /* add r4,r2 */
L8c6412ee: *stop=0x8c6412eeu;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[6],4);if(io_status)return io_status;goto L8c6412f0; /* mov.l r6,@r5 */
L8c6412f0: *stop=0x8c6412f0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6412f2; /* mov.l @(0x4,r15),r3 */
L8c6412f2: *stop=0x8c6412f2u;r[2]+=r[3];goto L8c6412f4; /* add r3,r2 */
L8c6412f4: *stop=0x8c6412f6u;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[7],4);if(io_status)return io_status;goto L8c641324; /* bra 0x8c641324 */
L8c641320: target=0x8c6411b0u;*pr=0x8c641324u;*stop=0x8c641322u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641324; /* bsr 0x8c6411b0 */
L8c641324: *stop=0x8c641324u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c641326; /* mov.l @r15,r0 */
L8c641326: *stop=0x8c641326u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c641328; /* stc SR,r3 */
L8c641328: *stop=0x8c641328u;r[2]=sx(m,bus,&io_status,0x8c6413c0u,2);if(io_status)return io_status;goto L8c64132a; /* mov.w 0x8c6413c0,r2 */
L8c64132a: *stop=0x8c64132au;r[0]&=0xfu;goto L8c64132c; /* and #0xf,r0 */
L8c64132c: *stop=0x8c64132cu;r[0]<<=2;goto L8c64132e; /* shll2 r0 */
L8c64132e: *stop=0x8c64132eu;r[3]&=r[2];goto L8c641330; /* and r2,r3 */
L8c641330: *stop=0x8c641330u;r[0]<<=2;goto L8c641332; /* shll2 r0 */
L8c641332: *stop=0x8c641332u;r[0]|=r[3];goto L8c641334; /* or r3,r0 */
L8c641334: *stop=0x8c641334u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c641336; /* ldc r0,SR */
L8c641336: *stop=0x8c641336u;r[15]+=8u;goto L8c641338; /* add #0x8,r15 */
L8c641338: *stop=0x8c641338u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64133a; /* lds.l @r15+,PR */
L8c64133a: *stop=0x8c64133au;if(*pr!=entry_pr)return -3;*stop=0x8c64133cu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_event_8c6411b0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6411b0;
L8c6411b0: *stop=0x8c6411b0u;r[0]=r[5];goto L8c6411b2; /* mov r5,r0 */
L8c6411b2: *stop=0x8c6411b2u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c6411b4; /* cmp/eq #0x0,r0 */
L8c6411b4: if((*sr&1u))goto L8c641240;goto L8c6411b6; /* bt 0x8c641240 */
L8c6411b6: *stop=0x8c6411b6u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6411b8; /* cmp/eq #0x1,r0 */
L8c6411b8: if((*sr&1u))goto L8c641248;goto L8c6411ba; /* bt 0x8c641248 */
L8c6411ba: *stop=0x8c6411bau;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c6411bc; /* cmp/eq #0x2,r0 */
L8c6411bc: if((*sr&1u))goto L8c641250;goto L8c6411be; /* bt 0x8c641250 */
L8c6411be: *stop=0x8c6411beu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c6411c0; /* cmp/eq #0x3,r0 */
L8c6411c0: if((*sr&1u))goto L8c641258;goto L8c6411c2; /* bt 0x8c641258 */
L8c6411c2: *stop=0x8c6411c2u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c6411c4; /* cmp/eq #0x4,r0 */
L8c6411c4: if((*sr&1u))goto L8c641260;goto L8c6411c6; /* bt 0x8c641260 */
L8c6411c6: *stop=0x8c6411c6u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c6411c8; /* cmp/eq #0x5,r0 */
L8c6411c8: if(!(*sr&1u))goto L8c6411ce;goto L8c6411ca; /* bf 0x8c6411ce */
L8c6411ca: *stop=0x8c6411ccu;goto L8c6412a0; /* bra 0x8c6412a0 */
L8c6411ce: *stop=0x8c6411ceu;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c6411d0; /* cmp/eq #0x6,r0 */
L8c6411d0: if(!(*sr&1u))goto L8c6411d6;goto L8c6411d2; /* bf 0x8c6411d6 */
L8c6411d2: *stop=0x8c6411d4u;goto L8c6412a8; /* bra 0x8c6412a8 */
L8c6411d6: *stop=0x8c6411d6u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c6411d8; /* cmp/eq #0x7,r0 */
L8c6411d8: if(!(*sr&1u))goto L8c6411de;goto L8c6411da; /* bf 0x8c6411de */
L8c6411da: *stop=0x8c6411dcu;goto L8c6412b0; /* bra 0x8c6412b0 */
L8c6411de: *stop=0x8c6411deu;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c6411e0; /* cmp/eq #0x8,r0 */
L8c6411e0: if(!(*sr&1u))goto L8c6411e6;goto L8c6411e2; /* bf 0x8c6411e6 */
L8c6411e2: *stop=0x8c6411e4u;goto L8c6412b8; /* bra 0x8c6412b8 */
L8c6411e6: *stop=0x8c6411e8u;goto L8c6412be; /* bra 0x8c6412be */
L8c641240: *stop=0x8c641240u;r[0]=sx(m,bus,&io_status,0x8c641280u,2);if(io_status)return io_status;goto L8c641242; /* mov.w 0x8c641280,r0 */
L8c641242: *stop=0x8c641242u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64128cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641244; /* mov.l 0x8c64128c,r3 */
L8c641244: *stop=0x8c641246u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[3],4);if(io_status)return io_status;goto L8c6412be; /* bra 0x8c6412be */
L8c641248: *stop=0x8c641248u;r[0]=sx(m,bus,&io_status,0x8c641282u,2);if(io_status)return io_status;goto L8c64124a; /* mov.w 0x8c641282,r0 */
L8c64124a: *stop=0x8c64124au;{uint32_t loaded=rd(m,bus,&io_status,0x8c641290u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64124c; /* mov.l 0x8c641290,r1 */
L8c64124c: *stop=0x8c64124eu;goto L8c6412b4; /* bra 0x8c6412b4 */
L8c641250: *stop=0x8c641250u;r[0]=sx(m,bus,&io_status,0x8c641284u,2);if(io_status)return io_status;goto L8c641252; /* mov.w 0x8c641284,r0 */
L8c641252: *stop=0x8c641252u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641294u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641254; /* mov.l 0x8c641294,r2 */
L8c641254: *stop=0x8c641256u;goto L8c6412bc; /* bra 0x8c6412bc */
L8c641258: *stop=0x8c641258u;r[0]=sx(m,bus,&io_status,0x8c641286u,2);if(io_status)return io_status;goto L8c64125a; /* mov.w 0x8c641286,r0 */
L8c64125a: *stop=0x8c64125au;{uint32_t loaded=rd(m,bus,&io_status,0x8c641298u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64125c; /* mov.l 0x8c641298,r1 */
L8c64125c: *stop=0x8c64125eu;goto L8c6412b4; /* bra 0x8c6412b4 */
L8c641260: *stop=0x8c641260u;r[0]=sx(m,bus,&io_status,0x8c641288u,2);if(io_status)return io_status;goto L8c641262; /* mov.w 0x8c641288,r0 */
L8c641262: *stop=0x8c641262u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64129cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641264; /* mov.l 0x8c64129c,r2 */
L8c641264: *stop=0x8c641266u;goto L8c6412bc; /* bra 0x8c6412bc */
L8c6412a0: *stop=0x8c6412a0u;r[0]=sx(m,bus,&io_status,0x8c641300u,2);if(io_status)return io_status;goto L8c6412a2; /* mov.w 0x8c641300,r0 */
L8c6412a2: *stop=0x8c6412a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64130cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6412a4; /* mov.l 0x8c64130c,r1 */
L8c6412a4: *stop=0x8c6412a6u;goto L8c6412b4; /* bra 0x8c6412b4 */
L8c6412a8: *stop=0x8c6412a8u;r[0]=sx(m,bus,&io_status,0x8c641302u,2);if(io_status)return io_status;goto L8c6412aa; /* mov.w 0x8c641302,r0 */
L8c6412aa: *stop=0x8c6412aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c641310u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6412ac; /* mov.l 0x8c641310,r2 */
L8c6412ac: *stop=0x8c6412aeu;goto L8c6412bc; /* bra 0x8c6412bc */
L8c6412b0: *stop=0x8c6412b0u;r[0]=sx(m,bus,&io_status,0x8c641304u,2);if(io_status)return io_status;goto L8c6412b2; /* mov.w 0x8c641304,r0 */
L8c6412b2: *stop=0x8c6412b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641314u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6412b4; /* mov.l 0x8c641314,r1 */
L8c6412b4: *stop=0x8c6412b6u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[1],4);if(io_status)return io_status;goto L8c6412be; /* bra 0x8c6412be */
L8c6412b8: *stop=0x8c6412b8u;r[0]=sx(m,bus,&io_status,0x8c641306u,2);if(io_status)return io_status;goto L8c6412ba; /* mov.w 0x8c641306,r0 */
L8c6412ba: *stop=0x8c6412bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c641318u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6412bc; /* mov.l 0x8c641318,r2 */
L8c6412bc: *stop=0x8c6412bcu;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[2],4);if(io_status)return io_status;goto L8c6412be; /* mov.l r2,@(r0,r4) */
L8c6412be: *stop=0x8c6412beu;if(*pr!=entry_pr)return -3;*stop=0x8c6412c0u;return 0; /* rts */
}
