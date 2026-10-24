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
SA_STAGE_EXPORT int sa_movie_entry_8ceb0000(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8ceb0000;
L8ceb0000: *stop=0x8ceb0002u;if(!call)return -2;return call(ctx,0x8ceb0020u,m,f,sr,r,pr); /* bra 0x8ceb0020 */
}

SA_STAGE_EXPORT int sa_movie_entry_8ceb0020(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8ceb0020;
L8ceb0020: *stop=0x8ceb0020u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8ceb0022; /* mov.l r14,@-r15 */
L8ceb0022: *stop=0x8ceb0022u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8ceb0024; /* mov.l r13,@-r15 */
L8ceb0024: *stop=0x8ceb0024u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8ceb0026; /* mov.l r12,@-r15 */
L8ceb0026: *stop=0x8ceb0026u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8ceb0028; /* mov.l r11,@-r15 */
L8ceb0028: *stop=0x8ceb0028u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8ceb002a; /* mov.l r10,@-r15 */
L8ceb002a: *stop=0x8ceb002au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8ceb002c; /* mov.l r9,@-r15 */
L8ceb002c: *stop=0x8ceb002cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8ceb002e; /* mov.l r8,@-r15 */
L8ceb002e: *stop=0x8ceb002eu;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8ceb0030; /* fmov.s fr15,@-r15 */
L8ceb0030: *stop=0x8ceb0030u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8ceb0032; /* fmov.s fr14,@-r15 */
L8ceb0032: *stop=0x8ceb0032u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8ceb0034; /* fmov.s fr13,@-r15 */
L8ceb0034: *stop=0x8ceb0034u;if(!valid_access(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8ceb0036; /* fmov.s fr12,@-r15 */
L8ceb0036: *stop=0x8ceb0036u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8ceb0038; /* sts.l PR,@-r15 */
L8ceb0038: *stop=0x8ceb0038u;r[15]+=4294967280u;goto L8ceb003a; /* add #-0x10,r15 */
L8ceb003a: *stop=0x8ceb003au;r[11]=r[4];goto L8ceb003c; /* mov r4,r11 */
L8ceb003c: *stop=0x8ceb003cu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0180u,4);if(io_status)return io_status;r[9]=loaded;}goto L8ceb003e; /* mov.l 0x8ceb0180,r9 */
L8ceb003e: *stop=0x8ceb003eu;r[0]=r[11];goto L8ceb0040; /* mov r11,r0 */
L8ceb0040: *stop=0x8ceb0040u;r[12]=0u;goto L8ceb0042; /* mov #0x0,r12 */
L8ceb0042: *stop=0x8ceb0042u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8ceb0044; /* cmp/eq #0x7,r0 */
L8ceb0044: *stop=0x8ceb0044u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[12],4);if(io_status)return io_status;goto L8ceb0046; /* mov.l r12,@(0x4,r15) */
L8ceb0046: take=(*sr&1u);*stop=0x8ceb0048u;r[10]=1u;if(take)goto L8ceb0050;goto L8ceb004a; /* bt/s 0x8ceb0050 */
L8ceb004a: *stop=0x8ceb004au;r[2]=5u;goto L8ceb004c; /* mov #0x5,r2 */
L8ceb004c: *stop=0x8ceb004eu;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[2],4);if(io_status)return io_status;goto L8ceb0052; /* bra 0x8ceb0052 */
L8ceb0050: *stop=0x8ceb0050u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[12],4);if(io_status)return io_status;goto L8ceb0052; /* mov.l r12,@r9 */
L8ceb0052: *stop=0x8ceb0052u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0190u,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb0054; /* mov.l 0x8ceb0190,r2 */
L8ceb0054: *stop=0x8ceb0054u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb018cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8ceb0056; /* mov.l 0x8ceb018c,r14 */
L8ceb0056: *stop=0x8ceb0056u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8ceb0058; /* mov.l @r2,r0 */
L8ceb0058: *stop=0x8ceb0058u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0184u,4);if(io_status)return io_status;r[4]=loaded;}goto L8ceb005a; /* mov.l 0x8ceb0184,r4 */
L8ceb005a: *stop=0x8ceb005au;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0188u,4);if(io_status)return io_status;r[13]=loaded;}goto L8ceb005c; /* mov.l 0x8ceb0188,r13 */
L8ceb005c: *stop=0x8ceb005cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8ceb005e; /* cmp/eq #0x1,r0 */
L8ceb005e: if(!(*sr&1u))goto L8ceb006c;goto L8ceb0060; /* bf 0x8ceb006c */
L8ceb0060: *stop=0x8ceb0060u;r[1]=1u;goto L8ceb0062; /* mov #0x1,r1 */
L8ceb0062: *stop=0x8ceb0062u;r[3]=2u;goto L8ceb0064; /* mov #0x2,r3 */
L8ceb0064: *stop=0x8ceb0064u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8ceb0066; /* mov.l r1,@r4 */
L8ceb0066: *stop=0x8ceb0066u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[1],4);if(io_status)return io_status;goto L8ceb0068; /* mov.l r1,@r14 */
L8ceb0068: *stop=0x8ceb006au;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8ceb0088; /* bra 0x8ceb0088 */
L8ceb006c: *stop=0x8ceb006cu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0194u,4);if(io_status)return io_status;r[1]=loaded;}goto L8ceb006e; /* mov.l 0x8ceb0194,r1 */
L8ceb006e: *stop=0x8ceb006eu;r[0]=1u;goto L8ceb0070; /* mov #0x1,r0 */
L8ceb0070: *stop=0x8ceb0070u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[0],4);if(io_status)return io_status;goto L8ceb0072; /* mov.l r0,@r13 */
L8ceb0072: *stop=0x8ceb0072u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb0074; /* mov.l @r1,r3 */
L8ceb0074: *stop=0x8ceb0074u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8ceb0076; /* tst r3,r3 */
L8ceb0076: if(!(*sr&1u))goto L8ceb0082;goto L8ceb0078; /* bf 0x8ceb0082 */
L8ceb0078: *stop=0x8ceb0078u;r[3]=52u;goto L8ceb007a; /* mov #0x34,r3 */
L8ceb007a: *stop=0x8ceb007au;r[0]=56u;goto L8ceb007c; /* mov #0x38,r0 */
L8ceb007c: *stop=0x8ceb007cu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8ceb007e; /* mov.l r3,@r14 */
L8ceb007e: *stop=0x8ceb0080u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8ceb0088; /* bra 0x8ceb0088 */
L8ceb0082: *stop=0x8ceb0082u;r[5]=58u;goto L8ceb0084; /* mov #0x3a,r5 */
L8ceb0084: *stop=0x8ceb0084u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[5],4);if(io_status)return io_status;goto L8ceb0086; /* mov.l r5,@r14 */
L8ceb0086: *stop=0x8ceb0086u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8ceb0088; /* mov.l r5,@r4 */
L8ceb0088: *stop=0x8ceb0088u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb019cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb008a; /* mov.l 0x8ceb019c,r2 */
L8ceb008a: *stop=0x8ceb008au;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0198u,4);if(io_status)return io_status;r[4]=loaded;}goto L8ceb008c; /* mov.l 0x8ceb0198,r4 */
L8ceb008c: target=r[2];*pr=0x8ceb0090u;*stop=0x8ceb008eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb0090; /* jsr @r2 */
L8ceb0090: *stop=0x8ceb0090u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01a0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb0092; /* mov.l 0x8ceb01a0,r3 */
L8ceb0092: target=r[3];*pr=0x8ceb0096u;*stop=0x8ceb0094u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb0096; /* jsr @r3 */
L8ceb0096: *stop=0x8ceb0096u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01a4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb0098; /* mov.l 0x8ceb01a4,r2 */
L8ceb0098: target=r[2];*pr=0x8ceb009cu;*stop=0x8ceb009au;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb009c; /* jsr @r2 */
L8ceb009c: *stop=0x8ceb009cu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01a8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb009e; /* mov.l 0x8ceb01a8,r3 */
L8ceb009e: target=r[3];*pr=0x8ceb00a2u;*stop=0x8ceb00a0u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb00a2; /* jsr @r3 */
L8ceb00a2: *stop=0x8ceb00a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01acu,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb00a4; /* mov.l 0x8ceb01ac,r2 */
L8ceb00a4: target=r[2];*pr=0x8ceb00a8u;*stop=0x8ceb00a6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb00a8; /* jsr @r2 */
L8ceb00a8: *stop=0x8ceb00a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb00aa; /* mov.l 0x8ceb01b0,r3 */
L8ceb00aa: target=r[3];*pr=0x8ceb00aeu;*stop=0x8ceb00acu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb00ae; /* jsr @r3 */
L8ceb00ae: *stop=0x8ceb00aeu;r[0]=r[11];goto L8ceb00b0; /* mov r11,r0 */
L8ceb00b0: *stop=0x8ceb00b0u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8ceb00b2; /* cmp/eq #0x7,r0 */
L8ceb00b2: if(!(*sr&1u))goto L8ceb00ba;goto L8ceb00b4; /* bf 0x8ceb00ba */
L8ceb00b4: *stop=0x8ceb00b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01b4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb00b6; /* mov.l 0x8ceb01b4,r3 */
L8ceb00b6: target=r[3];*pr=0x8ceb00bau;*stop=0x8ceb00b8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb00ba; /* jsr @r3 */
L8ceb00ba: *stop=0x8ceb00bau;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01bcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb00bc; /* mov.l 0x8ceb01bc,r2 */
L8ceb00bc: *stop=0x8ceb00bcu;r[6]=32u;goto L8ceb00be; /* mov #0x20,r6 */
L8ceb00be: *stop=0x8ceb00beu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01b8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8ceb00c0; /* mov.l 0x8ceb01b8,r4 */
L8ceb00c0: target=r[2];*pr=0x8ceb00c4u;*stop=0x8ceb00c2u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb00c4; /* jsr @r2 */
L8ceb00c4: *stop=0x8ceb00c4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb00c6; /* mov.l @r14,r3 */
L8ceb00c6: *stop=0x8ceb00c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01b8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8ceb00c8; /* mov.l 0x8ceb01b8,r4 */
L8ceb00c8: *stop=0x8ceb00c8u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8ceb00ca; /* mov.l r3,@r4 */
L8ceb00ca: *stop=0x8ceb00cau;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb00cc; /* mov.l @r9,r2 */
L8ceb00cc: *stop=0x8ceb00ccu;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[2],4);if(io_status)return io_status;goto L8ceb00ce; /* mov.l r2,@(0x4,r4) */
L8ceb00ce: *stop=0x8ceb00ceu;r[2]=2u;goto L8ceb00d0; /* mov #0x2,r2 */
L8ceb00d0: *stop=0x8ceb00d0u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb00d2; /* mov.l @r13,r3 */
L8ceb00d2: *stop=0x8ceb00d2u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[3],4);if(io_status)return io_status;goto L8ceb00d4; /* mov.l r3,@(0x8,r4) */
L8ceb00d4: *stop=0x8ceb00d4u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb00d6; /* mov.l 0x8ceb01c0,r3 */
L8ceb00d6: target=r[3];*pr=0x8ceb00dau;*stop=0x8ceb00d8u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[2],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb00da; /* jsr @r3 */
L8ceb00da: *stop=0x8ceb00dau;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01c4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8ceb00dc; /* mov.l 0x8ceb01c4,r0 */
L8ceb00dc: *stop=0x8ceb00dcu;r[2]=r[11];goto L8ceb00de; /* mov r11,r2 */
L8ceb00de: *stop=0x8ceb00deu;r[2]<<=2;goto L8ceb00e0; /* shll2 r2 */
L8ceb00e0: *stop=0x8ceb00e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01d8u,4);if(io_status)return io_status;r[8]=loaded;}goto L8ceb00e2; /* mov.l 0x8ceb01d8,r8 */
L8ceb00e2: *stop=0x8ceb00e2u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8ceb00e4; /* tst r11,r11 */
L8ceb00e4: *stop=0x8ceb00e4u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb00e6; /* mov.l @(r0,r2),r3 */
L8ceb00e6: *stop=0x8ceb00e6u;r[0]=*sr&1u;goto L8ceb00e8; /* movt r0 */
L8ceb00e8: *stop=0x8ceb00e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb00ea; /* mov.l 0x8ceb01c8,r2 */
L8ceb00ea: *stop=0x8ceb00eau;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01e4u,4);if(io_status)return io_status;r[14]=loaded;}goto L8ceb00ec; /* mov.l 0x8ceb01e4,r14 */
L8ceb00ec: *stop=0x8ceb00ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01e0u,4);if(io_status)return io_status;r[13]=loaded;}goto L8ceb00ee; /* mov.l 0x8ceb01e0,r13 */
L8ceb00ee: *stop=0x8ceb00eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01dcu,4);if(io_status)return io_status;r[9]=loaded;}goto L8ceb00f0; /* mov.l 0x8ceb01dc,r9 */
L8ceb00f0: *stop=0x8ceb00f0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8ceb00f2; /* mov.l r3,@r15 */
L8ceb00f2: *stop=0x8ceb00f2u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8ceb00f4; /* mov.l r2,@(0x8,r15) */
L8ceb00f4: *stop=0x8ceb00f4u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[0],4);if(io_status)return io_status;goto L8ceb00f6; /* mov.l r0,@(0xc,r15) */
L8ceb00f6: *stop=0x8ceb00f6u;r[0]=0x8ceb01ccu;goto L8ceb00f8; /* mova 0x8ceb01cc,r0 */
L8ceb00f8: *stop=0x8ceb00f8u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8ceb00fa; /* fmov.s @r0,fr12 */
L8ceb00fa: *stop=0x8ceb00fau;r[0]=0x8ceb01d0u;goto L8ceb00fc; /* mova 0x8ceb01d0,r0 */
L8ceb00fc: *stop=0x8ceb00fcu;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[0]);if(io_status)return io_status;goto L8ceb00fe; /* fmov.s @r0,fr13 */
L8ceb00fe: *stop=0x8ceb00feu;r[0]=0x8ceb01d4u;goto L8ceb0100; /* mova 0x8ceb01d4,r0 */
L8ceb0100: *stop=0x8ceb0100u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8ceb0102; /* fmov.s @r0,fr15 */
L8ceb0102: *stop=0x8ceb0102u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][14]=0;goto L8ceb0104; /* fldi0 fr14 */
L8ceb0104: *stop=0x8ceb0104u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb0106; /* mov.l @(0x4,r15),r3 */
L8ceb0106: *stop=0x8ceb0106u;r[3]+=1u;goto L8ceb0108; /* add #0x1,r3 */
L8ceb0108: *stop=0x8ceb0108u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8ceb010a; /* mov.l r3,@(0x4,r15) */
L8ceb010a: *stop=0x8ceb010au;r[3]+=4294967295u;goto L8ceb010c; /* add #-0x1,r3 */
L8ceb010c: *stop=0x8ceb010cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8ceb010e; /* mov.l @r15,r0 */
L8ceb010e: *stop=0x8ceb010eu;if(!valid_access(r[0]+r[3],1u))return -3;r[3]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8ceb0110; /* mov.b @(r0,r3),r3 */
L8ceb0110: *stop=0x8ceb0110u;r[0]=r[3];goto L8ceb0112; /* mov r3,r0 */
L8ceb0112: *stop=0x8ceb0112u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8ceb0114; /* cmp/eq #-0x1,r0 */
L8ceb0114: take=!(*sr&1u);*stop=0x8ceb0116u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[3],4);if(io_status)return io_status;if(take)goto L8ceb011c;goto L8ceb0118; /* bf/s 0x8ceb011c */
L8ceb0118: *stop=0x8ceb011au;goto L8ceb03a2; /* bra 0x8ceb03a2 */
L8ceb011c: *stop=0x8ceb011cu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[0]=loaded;}goto L8ceb011e; /* mov.l @r9,r0 */
L8ceb011e: *stop=0x8ceb011eu;*sr=(*sr&~1u)|(r[0]==0xfffffffeu);goto L8ceb0120; /* cmp/eq #-0x2,r0 */
L8ceb0120: take=!(*sr&1u);*stop=0x8ceb0122u;r[6]=32u;if(take)goto L8ceb0132;goto L8ceb0124; /* bf/s 0x8ceb0132 */
L8ceb0124: *stop=0x8ceb0124u;r[3]=r[12];goto L8ceb0126; /* mov r12,r3 */
L8ceb0126: *stop=0x8ceb0126u;r[3]+=1u;goto L8ceb0128; /* add #0x1,r3 */
L8ceb0128: *stop=0x8ceb0128u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8ceb012a; /* mov.l r3,@(0x4,r15) */
L8ceb012a: *stop=0x8ceb012au;r[3]+=4294967295u;goto L8ceb012c; /* add #-0x1,r3 */
L8ceb012c: *stop=0x8ceb012cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8ceb012e; /* mov.l @r15,r0 */
L8ceb012e: *stop=0x8ceb012eu;if(!valid_access(r[0]+r[3],1u))return -3;r[3]=sx(m,bus,&io_status,r[3]+r[0],1);if(io_status)return io_status;goto L8ceb0130; /* mov.b @(r0,r3),r3 */
L8ceb0130: *stop=0x8ceb0130u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[3],4);if(io_status)return io_status;goto L8ceb0132; /* mov.l r3,@r9 */
L8ceb0132: *stop=0x8ceb0132u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01bcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb0134; /* mov.l 0x8ceb01bc,r2 */
L8ceb0134: *stop=0x8ceb0134u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01e4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8ceb0136; /* mov.l 0x8ceb01e4,r4 */
L8ceb0136: target=r[2];*pr=0x8ceb013au;*stop=0x8ceb0138u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb013a; /* jsr @r2 */
L8ceb013a: *stop=0x8ceb013au;r[3]=1u;goto L8ceb013c; /* mov #0x1,r3 */
L8ceb013c: *stop=0x8ceb013cu;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[3],4);if(io_status)return io_status;goto L8ceb013e; /* mov.l r3,@r14 */
L8ceb013e: *stop=0x8ceb013eu;r[2]=0u;goto L8ceb0140; /* mov #0x0,r2 */
L8ceb0140: *stop=0x8ceb0140u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb0142; /* mov.l 0x8ceb01e8,r3 */
L8ceb0142: *stop=0x8ceb0142u;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[2],4);if(io_status)return io_status;goto L8ceb0144; /* mov.l r2,@(0x1c,r14) */
L8ceb0144: *stop=0x8ceb0144u;r[2]=80u;goto L8ceb0146; /* mov #0x50,r2 */
L8ceb0146: *stop=0x8ceb0146u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[3],4);if(io_status)return io_status;goto L8ceb0148; /* mov.l r3,@(0x4,r14) */
L8ceb0148: *stop=0x8ceb0148u;r[3]=120u;goto L8ceb014a; /* mov #0x78,r3 */
L8ceb014a: *stop=0x8ceb014au;r[2]<<=2;goto L8ceb014c; /* shll2 r2 */
L8ceb014c: *stop=0x8ceb014cu;r[3]<<=2;goto L8ceb014e; /* shll2 r3 */
L8ceb014e: *stop=0x8ceb014eu;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[2],4);if(io_status)return io_status;goto L8ceb0150; /* mov.l r2,@(0x8,r14) */
L8ceb0150: *stop=0x8ceb0150u;r[2]=6u;goto L8ceb0152; /* mov #0x6,r2 */
L8ceb0152: *stop=0x8ceb0152u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[3],4);if(io_status)return io_status;goto L8ceb0154; /* mov.l r3,@(0xc,r14) */
L8ceb0154: *stop=0x8ceb0154u;r[3]=r[2];goto L8ceb0156; /* mov r2,r3 */
L8ceb0156: *stop=0x8ceb0156u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[2],4);if(io_status)return io_status;goto L8ceb0158; /* mov.l r2,@(0x10,r14) */
L8ceb0158: *stop=0x8ceb0158u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8ceb015a; /* mov.l r2,@-r15 */
L8ceb015a: *stop=0x8ceb015au;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8ceb015c; /* mov.l @(0x4,r14),r5 */
L8ceb015c: *stop=0x8ceb015cu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb015e; /* mov.l 0x8ceb01ec,r2 */
L8ceb015e: *stop=0x8ceb015eu;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[7]=loaded;}goto L8ceb0160; /* mov.l @(0xc,r14),r7 */
L8ceb0160: *stop=0x8ceb0160u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8ceb0162; /* mov.l @(0x8,r14),r6 */
L8ceb0162: target=r[2];*pr=0x8ceb0166u;*stop=0x8ceb0164u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb0166; /* jsr @r2 */
L8ceb0166: *stop=0x8ceb0166u;r[15]+=4u;goto L8ceb0168; /* add #0x4,r15 */
L8ceb0168: *stop=0x8ceb0168u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01e4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8ceb016a; /* mov.l 0x8ceb01e4,r4 */
L8ceb016a: *stop=0x8ceb016au;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01f4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb016c; /* mov.l 0x8ceb01f4,r2 */
L8ceb016c: *stop=0x8ceb016cu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb01f0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb016e; /* mov.l 0x8ceb01f0,r3 */
L8ceb016e: *stop=0x8ceb016eu;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[0],4);if(io_status)return io_status;goto L8ceb0170; /* mov.l r0,@(0x18,r14) */
L8ceb0170: target=r[2];*pr=0x8ceb0174u;*stop=0x8ceb0172u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb0174; /* jsr @r2 */
L8ceb0174: *stop=0x8ceb0174u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8ceb0176; /* tst r0,r0 */
L8ceb0176: take=!(*sr&1u);*stop=0x8ceb0178u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[0],4);if(io_status)return io_status;if(take)goto L8ceb0200;goto L8ceb017a; /* bf/s 0x8ceb0200 */
L8ceb017a: *stop=0x8ceb017cu;r[10]=r[12];goto L8ceb03a2; /* bra 0x8ceb03a2 */
L8ceb0200: *stop=0x8ceb0200u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb0202; /* mov.l @(0xc,r15),r3 */
L8ceb0202: *stop=0x8ceb0202u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8ceb0204; /* tst r3,r3 */
L8ceb0204: if(!(*sr&1u))goto L8ceb020c;goto L8ceb0206; /* bf 0x8ceb020c */
L8ceb0206: *stop=0x8ceb0206u;r[0]=r[11];goto L8ceb0208; /* mov r11,r0 */
L8ceb0208: *stop=0x8ceb0208u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8ceb020a; /* cmp/eq #0x7,r0 */
L8ceb020a: if(!(*sr&1u))goto L8ceb0214;goto L8ceb020c; /* bf 0x8ceb0214 */
L8ceb020c: *stop=0x8ceb020cu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0320u,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb020e; /* mov.l 0x8ceb0320,r2 */
L8ceb020e: *stop=0x8ceb020eu;r[5]=1u;goto L8ceb0210; /* mov #0x1,r5 */
L8ceb0210: target=r[2];*pr=0x8ceb0214u;*stop=0x8ceb0212u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb0214; /* jsr @r2 */
L8ceb0214: *stop=0x8ceb0214u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0324u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb0216; /* mov.l 0x8ceb0324,r3 */
L8ceb0216: *stop=0x8ceb0216u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8ceb0218; /* mov.l @r3,r0 */
L8ceb0218: *stop=0x8ceb0218u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8ceb021a; /* cmp/eq #0x1,r0 */
L8ceb021a: if((*sr&1u))goto L8ceb0238;goto L8ceb021c; /* bt 0x8ceb0238 */
L8ceb021c: *stop=0x8ceb021cu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0328u,4);if(io_status)return io_status;r[1]=loaded;}goto L8ceb021e; /* mov.l 0x8ceb0328,r1 */
L8ceb021e: *stop=0x8ceb021eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8ceb0220; /* mov.l @r1,r0 */
L8ceb0220: *stop=0x8ceb0220u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8ceb0222; /* cmp/eq #0x1,r0 */
L8ceb0222: if(!(*sr&1u))goto L8ceb0238;goto L8ceb0224; /* bf 0x8ceb0238 */
L8ceb0224: *stop=0x8ceb0224u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb032cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb0226; /* mov.l 0x8ceb032c,r3 */
L8ceb0226: *stop=0x8ceb0226u;fcopy(f,4u,14u);goto L8ceb0228; /* fmov fr14,fr4 */
L8ceb0228: *stop=0x8ceb0228u;fcopy(f,5u,15u);goto L8ceb022a; /* fmov fr15,fr5 */
L8ceb022a: target=r[3];*pr=0x8ceb022eu;*stop=0x8ceb022cu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb022e; /* jsr @r3 */
L8ceb022e: *stop=0x8ceb022eu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0330u,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb0230; /* mov.l 0x8ceb0330,r2 */
L8ceb0230: *stop=0x8ceb0230u;fcopy(f,4u,12u);goto L8ceb0232; /* fmov fr12,fr4 */
L8ceb0232: *stop=0x8ceb0232u;fcopy(f,5u,13u);goto L8ceb0234; /* fmov fr13,fr5 */
L8ceb0234: target=r[2];*pr=0x8ceb0238u;*stop=0x8ceb0236u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb0238; /* jsr @r2 */
L8ceb0238: target=0x8ceb03f2u;*pr=0x8ceb023cu;*stop=0x8ceb023au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb023c; /* bsr 0x8ceb03f2 */
L8ceb023c: *stop=0x8ceb023cu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb023e; /* mov.l @r13,r2 */
L8ceb023e: *stop=0x8ceb023eu;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[5]=loaded;}goto L8ceb0240; /* mov.l @r9,r5 */
L8ceb0240: *stop=0x8ceb0240u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb0242; /* mov.l @r2,r3 */
L8ceb0242: *stop=0x8ceb0242u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0334u,4);if(io_status)return io_status;r[0]=loaded;}goto L8ceb0244; /* mov.l 0x8ceb0334,r0 */
L8ceb0244: *stop=0x8ceb0244u;if(!valid_access(r[3]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8ceb0246; /* mov.l @(0x18,r3),r1 */
L8ceb0246: *stop=0x8ceb0246u;r[3]=r[5];goto L8ceb0248; /* mov r5,r3 */
L8ceb0248: *stop=0x8ceb0248u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8ceb024a; /* shll r5 */
L8ceb024a: *stop=0x8ceb024au;r[5]+=r[3];goto L8ceb024c; /* add r3,r5 */
L8ceb024c: *stop=0x8ceb024cu;r[5]<<=2;goto L8ceb024e; /* shll2 r5 */
L8ceb024e: *stop=0x8ceb024eu;if(!valid_access(r[0]+r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8ceb0250; /* mov.l @(r0,r5),r5 */
L8ceb0250: target=r[1];*pr=0x8ceb0254u;*stop=0x8ceb0252u;r[4]=r[2];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb0254; /* jsr @r1 */
L8ceb0254: *stop=0x8ceb0254u;r[3]=4294967295u;goto L8ceb0256; /* mov #-0x1,r3 */
L8ceb0256: *stop=0x8ceb0256u;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8ceb0258; /* mov.l r3,@r8 */
L8ceb0258: *stop=0x8ceb0258u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0338u,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb025a; /* mov.l 0x8ceb0338,r2 */
L8ceb025a: target=r[2];*pr=0x8ceb025eu;*stop=0x8ceb025cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb025e; /* jsr @r2 */
L8ceb025e: *stop=0x8ceb025eu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb033cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8ceb0260; /* mov.l 0x8ceb033c,r1 */
L8ceb0260: target=r[1];*pr=0x8ceb0264u;*stop=0x8ceb0262u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb0264; /* jsr @r1 */
L8ceb0264: *stop=0x8ceb0264u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0340u,4);if(io_status)return io_status;r[1]=loaded;}goto L8ceb0266; /* mov.l 0x8ceb0340,r1 */
L8ceb0266: *stop=0x8ceb0266u;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb0268; /* mov.l @r8,r2 */
L8ceb0268: *stop=0x8ceb0268u;r[2]+=1u;goto L8ceb026a; /* add #0x1,r2 */
L8ceb026a: *stop=0x8ceb026au;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8ceb026c; /* mov.l r2,@r8 */
L8ceb026c: *stop=0x8ceb026cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb026e; /* mov.l @r1,r3 */
L8ceb026e: *stop=0x8ceb026eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8ceb0270; /* tst r3,r3 */
L8ceb0270: if((*sr&1u))goto L8ceb027a;goto L8ceb0272; /* bt 0x8ceb027a */
L8ceb0272: *stop=0x8ceb0272u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0340u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb0274; /* mov.l 0x8ceb0340,r3 */
L8ceb0274: *stop=0x8ceb0274u;r[10]=r[12];goto L8ceb0276; /* mov r12,r10 */
L8ceb0276: *stop=0x8ceb0278u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[12],4);if(io_status)return io_status;goto L8ceb0380; /* bra 0x8ceb0380 */
L8ceb027a: *stop=0x8ceb027au;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0344u,4);if(io_status)return io_status;r[1]=loaded;}goto L8ceb027c; /* mov.l 0x8ceb0344,r1 */
L8ceb027c: target=r[1];*pr=0x8ceb0280u;*stop=0x8ceb027eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb0280; /* jsr @r1 */
L8ceb0280: *stop=0x8ceb0280u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0348u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb0282; /* mov.l 0x8ceb0348,r3 */
L8ceb0282: target=r[3];*pr=0x8ceb0286u;*stop=0x8ceb0284u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb0286; /* jsr @r3 */
L8ceb0286: *stop=0x8ceb0286u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8ceb0288; /* tst r0,r0 */
L8ceb0288: if((*sr&1u))goto L8ceb02a2;goto L8ceb028a; /* bt 0x8ceb02a2 */
L8ceb028a: *stop=0x8ceb028au;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8ceb028c; /* tst r11,r11 */
L8ceb028c: if(!(*sr&1u))goto L8ceb029e;goto L8ceb028e; /* bf 0x8ceb029e */
L8ceb028e: *stop=0x8ceb028eu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb034cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb0290; /* mov.l 0x8ceb034c,r2 */
L8ceb0290: target=r[2];*pr=0x8ceb0294u;*stop=0x8ceb0292u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb0294; /* jsr @r2 */
L8ceb0294: *stop=0x8ceb0294u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0350u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb0296; /* mov.l 0x8ceb0350,r3 */
L8ceb0296: target=r[3];*pr=0x8ceb029au;*stop=0x8ceb0298u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb029a; /* jsr @r3 */
L8ceb029a: *stop=0x8ceb029cu;goto L8ceb02a2; /* bra 0x8ceb02a2 */
L8ceb029e: *stop=0x8ceb02a0u;r[10]=4294967295u;goto L8ceb0380; /* bra 0x8ceb0380 */
L8ceb02a2: *stop=0x8ceb02a2u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8ceb02a4; /* tst r11,r11 */
L8ceb02a4: if(!(*sr&1u))goto L8ceb02d4;goto L8ceb02a6; /* bf 0x8ceb02d4 */
L8ceb02a6: *stop=0x8ceb02a6u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8ceb02a8; /* mov.l @(0x8,r15),r0 */
L8ceb02a8: *stop=0x8ceb02a8u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8ceb02aa; /* mov.l @r0,r0 */
L8ceb02aa: *stop=0x8ceb02aau;if(!valid_access(r[0]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0]+0x10u,4);if(io_status)return io_status;r[0]=loaded;}goto L8ceb02ac; /* mov.l @(0x10,r0),r0 */
L8ceb02ac: *stop=0x8ceb02acu;*sr=(*sr&~1u)|!(r[0]&0x8u);goto L8ceb02ae; /* tst #0x8,r0 */
L8ceb02ae: if((*sr&1u))goto L8ceb02d4;goto L8ceb02b0; /* bt 0x8ceb02d4 */
L8ceb02b0: *stop=0x8ceb02b0u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0354u,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb02b2; /* mov.l 0x8ceb0354,r2 */
L8ceb02b2: *stop=0x8ceb02b2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}goto L8ceb02b4; /* mov.l @r2,r4 */
L8ceb02b4: *stop=0x8ceb02b4u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8ceb02b6; /* tst r4,r4 */
L8ceb02b6: take=!(*sr&1u);*stop=0x8ceb02b8u;r[10]=r[12];if(take)goto L8ceb02c2;goto L8ceb02ba; /* bf/s 0x8ceb02c2 */
L8ceb02ba: *stop=0x8ceb02bau;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0358u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb02bc; /* mov.l 0x8ceb0358,r3 */
L8ceb02bc: *stop=0x8ceb02bcu;r[1]=1u;goto L8ceb02be; /* mov #0x1,r1 */
L8ceb02be: *stop=0x8ceb02c0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8ceb02d4; /* bra 0x8ceb02d4 */
L8ceb02c2: *stop=0x8ceb02c2u;r[0]=r[4];goto L8ceb02c4; /* mov r4,r0 */
L8ceb02c4: *stop=0x8ceb02c4u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8ceb02c6; /* cmp/eq #0x3,r0 */
L8ceb02c6: if((*sr&1u))goto L8ceb0380;goto L8ceb02c8; /* bt 0x8ceb0380 */
L8ceb02c8: *stop=0x8ceb02c8u;r[0]=r[4];goto L8ceb02ca; /* mov r4,r0 */
L8ceb02ca: *stop=0x8ceb02cau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8ceb02cc; /* cmp/eq #0x1,r0 */
L8ceb02cc: if(!(*sr&1u))goto L8ceb02d4;goto L8ceb02ce; /* bf 0x8ceb02d4 */
L8ceb02ce: *stop=0x8ceb02ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0354u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb02d0; /* mov.l 0x8ceb0354,r3 */
L8ceb02d0: *stop=0x8ceb02d0u;r[2]=4u;goto L8ceb02d2; /* mov #0x4,r2 */
L8ceb02d2: *stop=0x8ceb02d2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8ceb02d4; /* mov.l r2,@r3 */
L8ceb02d4: target=0x8ceb0430u;*pr=0x8ceb02d8u;*stop=0x8ceb02d6u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb02d8; /* bsr 0x8ceb0430 */
L8ceb02d8: *stop=0x8ceb02d8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8ceb02da; /* tst r0,r0 */
L8ceb02da: if(!(*sr&1u))goto L8ceb0380;goto L8ceb02dc; /* bf 0x8ceb0380 */
L8ceb02dc: *stop=0x8ceb02dcu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb02de; /* mov.l @r13,r3 */
L8ceb02de: *stop=0x8ceb02deu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb02e0; /* mov.l @r3,r2 */
L8ceb02e0: *stop=0x8ceb02e0u;if(!valid_access(r[2]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x20u,4);if(io_status)return io_status;r[1]=loaded;}goto L8ceb02e2; /* mov.l @(0x20,r2),r1 */
L8ceb02e2: target=r[1];*pr=0x8ceb02e6u;*stop=0x8ceb02e4u;r[4]=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb02e6; /* jsr @r1 */
L8ceb02e6: *stop=0x8ceb02e6u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb035cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb02e8; /* mov.l 0x8ceb035c,r3 */
L8ceb02e8: *stop=0x8ceb02e8u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8ceb02ea; /* cmp/eq #0x3,r0 */
L8ceb02ea: take=(*sr&1u);*stop=0x8ceb02ecu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;if(take)goto L8ceb0380;goto L8ceb02ee; /* bt/s 0x8ceb0380 */
L8ceb02ee: *stop=0x8ceb02eeu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8ceb02f0; /* cmp/eq #0x4,r0 */
L8ceb02f0: if((*sr&1u))goto L8ceb0380;goto L8ceb02f2; /* bt 0x8ceb0380 */
L8ceb02f2: *stop=0x8ceb02f2u;r[0]=r[11];goto L8ceb02f4; /* mov r11,r0 */
L8ceb02f4: *stop=0x8ceb02f4u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8ceb02f6; /* cmp/eq #0x7,r0 */
L8ceb02f6: if(!(*sr&1u))goto L8ceb02fe;goto L8ceb02f8; /* bf 0x8ceb02fe */
L8ceb02f8: *stop=0x8ceb02f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0360u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb02fa; /* mov.l 0x8ceb0360,r3 */
L8ceb02fa: target=r[3];*pr=0x8ceb02feu;*stop=0x8ceb02fcu;if(!valid_access(r[8],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[8],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb02fe; /* jsr @r3 */
L8ceb02fe: *stop=0x8ceb02feu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0364u,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb0300; /* mov.l 0x8ceb0364,r2 */
L8ceb0300: target=r[2];*pr=0x8ceb0304u;*stop=0x8ceb0302u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb0304; /* jsr @r2 */
L8ceb0304: *stop=0x8ceb0306u;goto L8ceb0258; /* bra 0x8ceb0258 */
L8ceb0380: *stop=0x8ceb0380u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb0382; /* mov.l @r13,r3 */
L8ceb0382: *stop=0x8ceb0382u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb0384; /* mov.l @r3,r2 */
L8ceb0384: *stop=0x8ceb0384u;if(!valid_access(r[2]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8ceb0386; /* mov.l @(0x1c,r2),r1 */
L8ceb0386: target=r[1];*pr=0x8ceb038au;*stop=0x8ceb0388u;r[4]=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb038a; /* jsr @r1 */
L8ceb038a: *stop=0x8ceb038au;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0500u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb038c; /* mov.l 0x8ceb0500,r3 */
L8ceb038c: target=r[3];*pr=0x8ceb0390u;*stop=0x8ceb038eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb0390; /* jsr @r3 */
L8ceb0390: *stop=0x8ceb0390u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb0392; /* mov.l @r13,r2 */
L8ceb0392: *stop=0x8ceb0392u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb0394; /* mov.l @r2,r3 */
L8ceb0394: *stop=0x8ceb0394u;if(!valid_access(r[3]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8ceb0396; /* mov.l @(0x14,r3),r1 */
L8ceb0396: target=r[1];*pr=0x8ceb039au;*stop=0x8ceb0398u;r[4]=r[2];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb039a; /* jsr @r1 */
L8ceb039a: *stop=0x8ceb039au;*sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8ceb039c; /* cmp/pl r10 */
L8ceb039c: if(!(*sr&1u))goto L8ceb03a2;goto L8ceb039e; /* bf 0x8ceb03a2 */
L8ceb039e: *stop=0x8ceb03a0u;goto L8ceb0104; /* bra 0x8ceb0104 */
L8ceb03a2: *stop=0x8ceb03a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0504u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb03a4; /* mov.l 0x8ceb0504,r3 */
L8ceb03a4: target=r[3];*pr=0x8ceb03a8u;*stop=0x8ceb03a6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb03a8; /* jsr @r3 */
L8ceb03a8: *stop=0x8ceb03a8u;r[0]=r[11];goto L8ceb03aa; /* mov r11,r0 */
L8ceb03aa: *stop=0x8ceb03aau;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8ceb03ac; /* cmp/eq #0x7,r0 */
L8ceb03ac: if(!(*sr&1u))goto L8ceb03b4;goto L8ceb03ae; /* bf 0x8ceb03b4 */
L8ceb03ae: *stop=0x8ceb03aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0508u,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb03b0; /* mov.l 0x8ceb0508,r3 */
L8ceb03b0: target=r[3];*pr=0x8ceb03b4u;*stop=0x8ceb03b2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb03b4; /* jsr @r3 */
L8ceb03b4: *stop=0x8ceb03b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0500u,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb03b6; /* mov.l 0x8ceb0500,r2 */
L8ceb03b6: target=r[2];*pr=0x8ceb03bau;*stop=0x8ceb03b8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb03ba; /* jsr @r2 */
L8ceb03ba: *stop=0x8ceb03bau;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb050cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8ceb03bc; /* mov.l 0x8ceb050c,r3 */
L8ceb03bc: target=r[3];*pr=0x8ceb03c0u;*stop=0x8ceb03beu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb03c0; /* jsr @r3 */
L8ceb03c0: *stop=0x8ceb03c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8ceb0500u,4);if(io_status)return io_status;r[2]=loaded;}goto L8ceb03c2; /* mov.l 0x8ceb0500,r2 */
L8ceb03c2: target=r[2];*pr=0x8ceb03c6u;*stop=0x8ceb03c4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8ceb03c6; /* jsr @r2 */
L8ceb03c6: *stop=0x8ceb03c6u;*sr=(*sr&~1u)|((int32_t)r[10]>=0);goto L8ceb03c8; /* cmp/pz r10 */
L8ceb03c8: if(!(*sr&1u))goto L8ceb03d4;goto L8ceb03ca; /* bf 0x8ceb03d4 */
L8ceb03ca: *stop=0x8ceb03cau;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8ceb03cc; /* tst r11,r11 */
L8ceb03cc: if(!(*sr&1u))goto L8ceb03d2;goto L8ceb03ce; /* bf 0x8ceb03d2 */
L8ceb03ce: *stop=0x8ceb03d0u;r[10]=1u;goto L8ceb03d4; /* bra 0x8ceb03d4 */
L8ceb03d2: *stop=0x8ceb03d2u;r[10]=r[12];goto L8ceb03d4; /* mov r12,r10 */
L8ceb03d4: *stop=0x8ceb03d4u;r[0]=r[10];goto L8ceb03d6; /* mov r10,r0 */
L8ceb03d6: *stop=0x8ceb03d6u;r[15]+=16u;goto L8ceb03d8; /* add #0x10,r15 */
L8ceb03d8: *stop=0x8ceb03d8u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8ceb03da; /* lds.l @r15+,PR */
L8ceb03da: *stop=0x8ceb03dau;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8ceb03dc; /* fmov.s @r15+,fr12 */
L8ceb03dc: *stop=0x8ceb03dcu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8ceb03de; /* fmov.s @r15+,fr13 */
L8ceb03de: *stop=0x8ceb03deu;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8ceb03e0; /* fmov.s @r15+,fr14 */
L8ceb03e0: *stop=0x8ceb03e0u;if(!valid_access(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8ceb03e2; /* fmov.s @r15+,fr15 */
L8ceb03e2: *stop=0x8ceb03e2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8ceb03e4; /* mov.l @r15+,r8 */
L8ceb03e4: *stop=0x8ceb03e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8ceb03e6; /* mov.l @r15+,r9 */
L8ceb03e6: *stop=0x8ceb03e6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8ceb03e8; /* mov.l @r15+,r10 */
L8ceb03e8: *stop=0x8ceb03e8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8ceb03ea; /* mov.l @r15+,r11 */
L8ceb03ea: *stop=0x8ceb03eau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8ceb03ec; /* mov.l @r15+,r12 */
L8ceb03ec: *stop=0x8ceb03ecu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8ceb03ee; /* mov.l @r15+,r13 */
L8ceb03ee: *stop=0x8ceb03eeu;if(*pr!=entry_pr)return -3;*stop=0x8ceb03f0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
