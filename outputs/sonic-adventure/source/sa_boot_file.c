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

static void div_step(uint32_t divisor, uint32_t *remainder, uint32_t *sr)
{
    uint32_t q = *remainder >> 31;
    uint32_t m = (*sr >> 9) & 1u;
    uint32_t old_q = (*sr >> 8) & 1u;
    uint32_t before = (*remainder << 1) | (*sr & 1u);
    uint32_t result;
    uint32_t carry;

    if (old_q == m) {
        result = before - divisor;
        carry = result > before;
    } else {
        result = before + divisor;
        carry = result < before;
    }
    q = m ? (q ? carry : !carry) : (q ? !carry : carry);
    *remainder = result;
    *sr = (*sr & ~0x101u) | (q << 8) | (uint32_t)(q == m);
}

SA_STAGE_EXPORT int sa_boot_file_8c09da40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c09da40;
L8c09da40: *stop=0x8c09da42u;if(!call)return -2;return call(ctx,0x8c09cc80u,m,f,sr,r,pr); /* bra 0x8c09cc80 */
}

SA_STAGE_EXPORT int sa_boot_file_8c09cc80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c09cc80;
L8c09cc80: *stop=0x8c09cc80u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c09cc82; /* mov.l r14,@-r15 */
L8c09cc82: *stop=0x8c09cc82u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c09cc84; /* sts.l PR,@-r15 */
L8c09cc84: *stop=0x8c09cc84u;r[15]+=4294967288u;goto L8c09cc86; /* add #-0x8,r15 */
L8c09cc86: *stop=0x8c09cc86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ce04u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09cc88; /* mov.l 0x8c09ce04,r3 */
L8c09cc88: *stop=0x8c09cc88u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ce00u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c09cc8a; /* mov.l 0x8c09ce00,r14 */
L8c09cc8a: *stop=0x8c09cc8au;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c09cc8c; /* mov.l r5,@(0x4,r15) */
L8c09cc8c: target=r[3];*pr=0x8c09cc90u;*stop=0x8c09cc8eu;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cc90; /* jsr @r3 */
L8c09cc90: *stop=0x8c09cc90u;r[4]=r[0];goto L8c09cc92; /* mov r0,r4 */
L8c09cc92: *stop=0x8c09cc92u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c09cc94; /* tst r4,r4 */
L8c09cc94: take=!(*sr&1u);*stop=0x8c09cc96u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[0],4);if(io_status)return io_status;if(take)goto L8c09cca2;goto L8c09cc98; /* bf/s 0x8c09cca2 */
L8c09cc98: *stop=0x8c09cc98u;r[0]=4294967295u;goto L8c09cc9a; /* mov #-0x1,r0 */
L8c09cc9a: *stop=0x8c09cc9au;r[15]+=8u;goto L8c09cc9c; /* add #0x8,r15 */
L8c09cc9c: *stop=0x8c09cc9cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c09cc9e; /* lds.l @r15+,PR */
L8c09cc9e: *stop=0x8c09cc9eu;if(*pr!=entry_pr)return -3;*stop=0x8c09cca0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c09cca2: *stop=0x8c09cca2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ce08u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09cca4; /* mov.l 0x8c09ce08,r3 */
L8c09cca4: target=r[3];*pr=0x8c09cca8u;*stop=0x8c09cca6u;r[5]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09cca8; /* jsr @r3 */
L8c09cca8: *stop=0x8c09cca8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ccaa; /* mov.l @r15,r1 */
L8c09ccaa: *stop=0x8c09ccaau;r[5]=8u;goto L8c09ccac; /* mov #0x8,r5 */
L8c09ccac: *stop=0x8c09ccacu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ce0cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c09ccae; /* mov.l 0x8c09ce0c,r2 */
L8c09ccae: *stop=0x8c09ccaeu;r[5]<<=8;goto L8c09ccb0; /* shll8 r5 */
L8c09ccb0: target=r[2];*pr=0x8c09ccb4u;*stop=0x8c09ccb2u;r[0]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09ccb4; /* jsr @r2 */
L8c09ccb4: *stop=0x8c09ccb4u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c09ccb6; /* tst r0,r0 */
L8c09ccb6: if((*sr&1u))goto L8c09ccbc;goto L8c09ccb8; /* bt 0x8c09ccbc */
L8c09ccb8: *stop=0x8c09ccbau;r[4]=1u;goto L8c09ccbe; /* bra 0x8c09ccbe */
L8c09ccbc: *stop=0x8c09ccbcu;r[4]=0u;goto L8c09ccbe; /* mov #0x0,r4 */
L8c09ccbe: *stop=0x8c09ccbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ce10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09ccc0; /* mov.l 0x8c09ce10,r3 */
L8c09ccc0: *stop=0x8c09ccc0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ccc2; /* mov.l @r15,r1 */
L8c09ccc2: target=r[3];*pr=0x8c09ccc6u;*stop=0x8c09ccc4u;r[0]=r[5];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09ccc6; /* jsr @r3 */
L8c09ccc6: *stop=0x8c09ccc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ce14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09ccc8; /* mov.l 0x8c09ce14,r3 */
L8c09ccc8: *stop=0x8c09ccc8u;r[0]+=r[4];goto L8c09ccca; /* add r4,r0 */
L8c09ccca: *stop=0x8c09cccau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c09cccc; /* mov.l r0,@r15 */
L8c09cccc: *stop=0x8c09ccccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c09ccce; /* mov.l @r15,r5 */
L8c09ccce: *stop=0x8c09ccceu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c09ccd0; /* mov.l @(0x4,r15),r6 */
L8c09ccd0: target=r[3];*pr=0x8c09ccd4u;*stop=0x8c09ccd2u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09ccd4; /* jsr @r3 */
L8c09ccd4: *stop=0x8c09ccd4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ce18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c09ccd6; /* mov.l 0x8c09ce18,r2 */
L8c09ccd6: target=r[2];*pr=0x8c09ccdau;*stop=0x8c09ccd8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09ccda; /* jsr @r2 */
L8c09ccda: *stop=0x8c09ccdau;r[0]=0u;goto L8c09ccdc; /* mov #0x0,r0 */
L8c09ccdc: *stop=0x8c09ccdcu;r[15]+=8u;goto L8c09ccde; /* add #0x8,r15 */
L8c09ccde: *stop=0x8c09ccdeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c09cce0; /* lds.l @r15+,PR */
L8c09cce0: *stop=0x8c09cce0u;if(*pr!=entry_pr)return -3;*stop=0x8c09cce2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c09ce40(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c09ce40;
L8c09ce40: *stop=0x8c09ce40u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09cec0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c09ce42; /* mov.l 0x8c09cec0,r2 */
L8c09ce42: *stop=0x8c09ce42u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c09ce44; /* sts.l PR,@-r15 */
L8c09ce44: *stop=0x8c09ce44u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c09ce46; /* mov.l @r2,r3 */
L8c09ce46: *stop=0x8c09ce46u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c09ce48; /* tst r3,r3 */
L8c09ce48: if((*sr&1u))goto L8c09ce5e;goto L8c09ce4a; /* bt 0x8c09ce5e */
L8c09ce4a: *stop=0x8c09ce4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c09cec4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c09ce4c; /* mov.l 0x8c09cec4,r1 */
L8c09ce4c: *stop=0x8c09ce4cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c09ce4e; /* mov.l @r1,r3 */
L8c09ce4e: *stop=0x8c09ce4eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c09ce50; /* tst r3,r3 */
L8c09ce50: if(!(*sr&1u))goto L8c09ce5e;goto L8c09ce52; /* bf 0x8c09ce5e */
L8c09ce52: *stop=0x8c09ce52u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09cec8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09ce54; /* mov.l 0x8c09cec8,r3 */
L8c09ce54: target=r[3];*pr=0x8c09ce58u;*stop=0x8c09ce56u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09ce58; /* jsr @r3 */
L8c09ce58: *stop=0x8c09ce58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ceccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c09ce5a; /* mov.l 0x8c09cecc,r2 */
L8c09ce5a: target=r[2];*stop=0x8c09ce5cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c09ce5e: *stop=0x8c09ce5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ced0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c09ce60; /* mov.l 0x8c09ced0,r2 */
L8c09ce60: target=r[2];*pr=0x8c09ce64u;*stop=0x8c09ce62u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09ce64; /* jsr @r2 */
L8c09ce64: *stop=0x8c09ce64u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c09ce66; /* cmp/eq #0x6,r0 */
L8c09ce66: take=(*sr&1u);*stop=0x8c09ce68u;r[4]=r[0];if(take)goto L8c09ce6e;goto L8c09ce6a; /* bt/s 0x8c09ce6e */
L8c09ce6a: *stop=0x8c09ce6au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c09ce6c; /* tst r4,r4 */
L8c09ce6c: if(!(*sr&1u))goto L8c09ce74;goto L8c09ce6e; /* bf 0x8c09ce74 */
L8c09ce6e: *stop=0x8c09ce6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c09ced4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c09ce70; /* mov.l 0x8c09ced4,r3 */
L8c09ce70: target=r[3];*stop=0x8c09ce72u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c09ce74: *stop=0x8c09ce74u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c09ce76; /* lds.l @r15+,PR */
L8c09ce76: *stop=0x8c09ce76u;if(*pr!=entry_pr)return -3;*stop=0x8c09ce78u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6016d6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6016d6;
L8c6016d6: *stop=0x8c6016d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601818u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6016d8; /* mov.l 0x8c601818,r3 */
L8c6016d8: *stop=0x8c6016d8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6016da; /* mov.l r14,@-r15 */
L8c6016da: *stop=0x8c6016dau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[14]=loaded;}goto L8c6016dc; /* mov.l @r3,r14 */
L8c6016dc: *stop=0x8c6016dcu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6016de; /* tst r14,r14 */
L8c6016de: if((*sr&1u))goto L8c6016e6;goto L8c6016e0; /* bt 0x8c6016e6 */
L8c6016e0: *stop=0x8c6016e0u;if(!valid_access(r[14]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x3cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6016e2; /* mov.l @(0x3c,r14),r0 */
L8c6016e2: target=r[0];*stop=0x8c6016e4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
L8c6016e6: *stop=0x8c6016e6u;r[0]=0u;goto L8c6016e8; /* mov #0x0,r0 */
L8c6016e8: *stop=0x8c6016e8u;if(*pr!=entry_pr)return -3;*stop=0x8c6016eau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c600e2c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c600e2c;
L8c600e2c: *stop=0x8c600e2cu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c600e2e; /* tst r4,r4 */
L8c600e2e: if((*sr&1u))goto L8c600e38;goto L8c600e30; /* bt 0x8c600e38 */
L8c600e30: *stop=0x8c600e30u;r[0]=76u;goto L8c600e32; /* mov #0x4c,r0 */
L8c600e32: *stop=0x8c600e32u;if(!valid_access(r[0]+r[4],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[4],2);if(io_status)return io_status;goto L8c600e34; /* mov.w @(r0,r4),r3 */
L8c600e34: *stop=0x8c600e34u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c600e36; /* tst r3,r3 */
L8c600e36: if(!(*sr&1u))goto L8c600e3c;goto L8c600e38; /* bf 0x8c600e3c */
L8c600e38: *stop=0x8c600e38u;if(*pr!=entry_pr)return -3;*stop=0x8c600e3au;r[0]=0u;return 0; /* rts */
L8c600e3c: *stop=0x8c600e3cu;if(!valid_access(r[4]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c600e3e; /* mov.l @(0xc,r4),r3 */
L8c600e3e: *stop=0x8c600e3eu;r[0]=1u;goto L8c600e40; /* mov #0x1,r0 */
L8c600e40: *stop=0x8c600e40u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c600e42; /* mov.l r3,@r5 */
L8c600e42: *stop=0x8c600e42u;if(*pr!=entry_pr)return -3;*stop=0x8c600e44u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c10ca5c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ca5c;
L8c10ca5c: *stop=0x8c10ca5cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c10ca5e; /* tst r0,r0 */
L8c10ca5e: *stop=0x8c10ca5eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c10ca60; /* mov.l r2,@-r15 */
L8c10ca60: if((*sr&1u))goto L8c10cafa;goto L8c10ca62; /* bt 0x8c10cafa */
L8c10ca62: *stop=0x8c10ca62u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10ca64; /* mov.l r3,@-r15 */
L8c10ca64: *stop=0x8c10ca64u;r[2]=0u;goto L8c10ca66; /* mov #0x0,r2 */
L8c10ca66: *stop=0x8c10ca66u;*sr=(*sr&~0x301u)|((r[1]>>31)<<8)|((r[2]>>31)<<9)|((r[1]^r[2])>>31);goto L8c10ca68; /* div0s r2,r1 */
L8c10ca68: *stop=0x8c10ca68u;{uint64_t lhs=r[3],rhs=(uint64_t)r[3]+(*sr&1u);r[3]=(uint32_t)(lhs-rhs);*sr=(*sr&~1u)|(lhs<rhs);}goto L8c10ca6a; /* subc r3,r3 */
L8c10ca6a: *stop=0x8c10ca6au;{uint64_t lhs=r[1],rhs=(uint64_t)r[2]+(*sr&1u);r[1]=(uint32_t)(lhs-rhs);*sr=(*sr&~1u)|(lhs<rhs);}goto L8c10ca6c; /* subc r2,r1 */
L8c10ca6c: *stop=0x8c10ca6cu;*sr=(*sr&~0x301u)|((r[3]>>31)<<8)|((r[0]>>31)<<9)|((r[3]^r[0])>>31);goto L8c10ca6e; /* div0s r0,r3 */
L8c10ca6e: *stop=0x8c10ca6eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ca70; /* rotcl r1 */
L8c10ca70: *stop=0x8c10ca70u;div_step(r[0],&r[3],sr);goto L8c10ca72; /* div1 r0,r3 */
L8c10ca72: *stop=0x8c10ca72u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ca74; /* rotcl r1 */
L8c10ca74: *stop=0x8c10ca74u;div_step(r[0],&r[3],sr);goto L8c10ca76; /* div1 r0,r3 */
L8c10ca76: *stop=0x8c10ca76u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ca78; /* rotcl r1 */
L8c10ca78: *stop=0x8c10ca78u;div_step(r[0],&r[3],sr);goto L8c10ca7a; /* div1 r0,r3 */
L8c10ca7a: *stop=0x8c10ca7au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ca7c; /* rotcl r1 */
L8c10ca7c: *stop=0x8c10ca7cu;div_step(r[0],&r[3],sr);goto L8c10ca7e; /* div1 r0,r3 */
L8c10ca7e: *stop=0x8c10ca7eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ca80; /* rotcl r1 */
L8c10ca80: *stop=0x8c10ca80u;div_step(r[0],&r[3],sr);goto L8c10ca82; /* div1 r0,r3 */
L8c10ca82: *stop=0x8c10ca82u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ca84; /* rotcl r1 */
L8c10ca84: *stop=0x8c10ca84u;div_step(r[0],&r[3],sr);goto L8c10ca86; /* div1 r0,r3 */
L8c10ca86: *stop=0x8c10ca86u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ca88; /* rotcl r1 */
L8c10ca88: *stop=0x8c10ca88u;div_step(r[0],&r[3],sr);goto L8c10ca8a; /* div1 r0,r3 */
L8c10ca8a: *stop=0x8c10ca8au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ca8c; /* rotcl r1 */
L8c10ca8c: *stop=0x8c10ca8cu;div_step(r[0],&r[3],sr);goto L8c10ca8e; /* div1 r0,r3 */
L8c10ca8e: *stop=0x8c10ca8eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ca90; /* rotcl r1 */
L8c10ca90: *stop=0x8c10ca90u;div_step(r[0],&r[3],sr);goto L8c10ca92; /* div1 r0,r3 */
L8c10ca92: *stop=0x8c10ca92u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ca94; /* rotcl r1 */
L8c10ca94: *stop=0x8c10ca94u;div_step(r[0],&r[3],sr);goto L8c10ca96; /* div1 r0,r3 */
L8c10ca96: *stop=0x8c10ca96u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ca98; /* rotcl r1 */
L8c10ca98: *stop=0x8c10ca98u;div_step(r[0],&r[3],sr);goto L8c10ca9a; /* div1 r0,r3 */
L8c10ca9a: *stop=0x8c10ca9au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ca9c; /* rotcl r1 */
L8c10ca9c: *stop=0x8c10ca9cu;div_step(r[0],&r[3],sr);goto L8c10ca9e; /* div1 r0,r3 */
L8c10ca9e: *stop=0x8c10ca9eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10caa0; /* rotcl r1 */
L8c10caa0: *stop=0x8c10caa0u;div_step(r[0],&r[3],sr);goto L8c10caa2; /* div1 r0,r3 */
L8c10caa2: *stop=0x8c10caa2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10caa4; /* rotcl r1 */
L8c10caa4: *stop=0x8c10caa4u;div_step(r[0],&r[3],sr);goto L8c10caa6; /* div1 r0,r3 */
L8c10caa6: *stop=0x8c10caa6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10caa8; /* rotcl r1 */
L8c10caa8: *stop=0x8c10caa8u;div_step(r[0],&r[3],sr);goto L8c10caaa; /* div1 r0,r3 */
L8c10caaa: *stop=0x8c10caaau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10caac; /* rotcl r1 */
L8c10caac: *stop=0x8c10caacu;div_step(r[0],&r[3],sr);goto L8c10caae; /* div1 r0,r3 */
L8c10caae: *stop=0x8c10caaeu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cab0; /* rotcl r1 */
L8c10cab0: *stop=0x8c10cab0u;div_step(r[0],&r[3],sr);goto L8c10cab2; /* div1 r0,r3 */
L8c10cab2: *stop=0x8c10cab2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cab4; /* rotcl r1 */
L8c10cab4: *stop=0x8c10cab4u;div_step(r[0],&r[3],sr);goto L8c10cab6; /* div1 r0,r3 */
L8c10cab6: *stop=0x8c10cab6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cab8; /* rotcl r1 */
L8c10cab8: *stop=0x8c10cab8u;div_step(r[0],&r[3],sr);goto L8c10caba; /* div1 r0,r3 */
L8c10caba: *stop=0x8c10cabau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cabc; /* rotcl r1 */
L8c10cabc: *stop=0x8c10cabcu;div_step(r[0],&r[3],sr);goto L8c10cabe; /* div1 r0,r3 */
L8c10cabe: *stop=0x8c10cabeu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cac0; /* rotcl r1 */
L8c10cac0: *stop=0x8c10cac0u;div_step(r[0],&r[3],sr);goto L8c10cac2; /* div1 r0,r3 */
L8c10cac2: *stop=0x8c10cac2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cac4; /* rotcl r1 */
L8c10cac4: *stop=0x8c10cac4u;div_step(r[0],&r[3],sr);goto L8c10cac6; /* div1 r0,r3 */
L8c10cac6: *stop=0x8c10cac6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cac8; /* rotcl r1 */
L8c10cac8: *stop=0x8c10cac8u;div_step(r[0],&r[3],sr);goto L8c10caca; /* div1 r0,r3 */
L8c10caca: *stop=0x8c10cacau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cacc; /* rotcl r1 */
L8c10cacc: *stop=0x8c10caccu;div_step(r[0],&r[3],sr);goto L8c10cace; /* div1 r0,r3 */
L8c10cace: *stop=0x8c10caceu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cad0; /* rotcl r1 */
L8c10cad0: *stop=0x8c10cad0u;div_step(r[0],&r[3],sr);goto L8c10cad2; /* div1 r0,r3 */
L8c10cad2: *stop=0x8c10cad2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cad4; /* rotcl r1 */
L8c10cad4: *stop=0x8c10cad4u;div_step(r[0],&r[3],sr);goto L8c10cad6; /* div1 r0,r3 */
L8c10cad6: *stop=0x8c10cad6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cad8; /* rotcl r1 */
L8c10cad8: *stop=0x8c10cad8u;div_step(r[0],&r[3],sr);goto L8c10cada; /* div1 r0,r3 */
L8c10cada: *stop=0x8c10cadau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cadc; /* rotcl r1 */
L8c10cadc: *stop=0x8c10cadcu;div_step(r[0],&r[3],sr);goto L8c10cade; /* div1 r0,r3 */
L8c10cade: *stop=0x8c10cadeu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cae0; /* rotcl r1 */
L8c10cae0: *stop=0x8c10cae0u;div_step(r[0],&r[3],sr);goto L8c10cae2; /* div1 r0,r3 */
L8c10cae2: *stop=0x8c10cae2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cae4; /* rotcl r1 */
L8c10cae4: *stop=0x8c10cae4u;div_step(r[0],&r[3],sr);goto L8c10cae6; /* div1 r0,r3 */
L8c10cae6: *stop=0x8c10cae6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cae8; /* rotcl r1 */
L8c10cae8: *stop=0x8c10cae8u;div_step(r[0],&r[3],sr);goto L8c10caea; /* div1 r0,r3 */
L8c10caea: *stop=0x8c10caeau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10caec; /* rotcl r1 */
L8c10caec: *stop=0x8c10caecu;div_step(r[0],&r[3],sr);goto L8c10caee; /* div1 r0,r3 */
L8c10caee: *stop=0x8c10caeeu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10caf0; /* rotcl r1 */
L8c10caf0: *stop=0x8c10caf0u;{uint64_t sum=(uint64_t)r[1]+r[2]+(*sr&1u);r[1]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);}goto L8c10caf2; /* addc r2,r1 */
L8c10caf2: *stop=0x8c10caf2u;r[0]=r[1];goto L8c10caf4; /* mov r1,r0 */
L8c10caf4: *stop=0x8c10caf4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c10caf6; /* mov.l @r15+,r3 */
L8c10caf6: *stop=0x8c10caf6u;if(*pr!=entry_pr)return -3;*stop=0x8c10caf8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;return 0; /* rts */
L8c10cafa: *stop=0x8c10cafau;{uint32_t loaded=rd(m,bus,&io_status,0x8c10cb08u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10cafc; /* mov.l 0x8c10cb08,r1 */
L8c10cafc: *stop=0x8c10cafcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10cb0cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10cafe; /* mov.l 0x8c10cb0c,r2 */
L8c10cafe: *stop=0x8c10cafeu;r[0]=0u;goto L8c10cb00; /* mov #0x0,r0 */
L8c10cb00: *stop=0x8c10cb00u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c10cb02; /* mov.l r2,@r1 */
L8c10cb02: *stop=0x8c10cb02u;if(*pr!=entry_pr)return -3;*stop=0x8c10cb04u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c600f1e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 unsigned delay_visits=0;(void)delay_visits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c600f1e;
L8c600f1e: *stop=0x8c600f1eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c600f20; /* mov.l r14,@-r15 */
L8c600f20: *stop=0x8c600f20u;r[3]=31u;goto L8c600f22; /* mov #0x1f,r3 */
L8c600f22: *stop=0x8c600f22u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c600f24; /* mov.l r13,@-r15 */
L8c600f24: *stop=0x8c600f24u;r[14]=r[4];goto L8c600f26; /* mov r4,r14 */
L8c600f26: *stop=0x8c600f26u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c600f28; /* mov.l r12,@-r15 */
L8c600f28: *stop=0x8c600f28u;r[12]=r[5];goto L8c600f2a; /* mov r5,r12 */
L8c600f2a: *stop=0x8c600f2au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c600f2c; /* mov.l r11,@-r15 */
L8c600f2c: *stop=0x8c600f2cu;r[11]=r[6];goto L8c600f2e; /* mov r6,r11 */
L8c600f2e: *stop=0x8c600f2eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c600f30; /* mov.l r10,@-r15 */
L8c600f30: *stop=0x8c600f30u;*sr=(*sr&~1u)|!(r[11]&r[3]);goto L8c600f32; /* tst r11,r3 */
L8c600f32: *stop=0x8c600f32u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c600f34; /* mov.l r9,@-r15 */
L8c600f34: *stop=0x8c600f34u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c600f36; /* mov.l r8,@-r15 */
L8c600f36: *stop=0x8c600f36u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c600f38; /* sts.l PR,@-r15 */
L8c600f38: take=!(*sr&1u);*stop=0x8c600f3au;r[4]=0u;if(take)goto L8c600f40;goto L8c600f3c; /* bf/s 0x8c600f40 */
L8c600f3c: *stop=0x8c600f3eu;r[9]=1u;goto L8c600f42; /* bra 0x8c600f42 */
L8c600f40: *stop=0x8c600f40u;r[9]=r[4];goto L8c600f42; /* mov r4,r9 */
L8c600f42: *stop=0x8c600f42u;r[10]=sx(m,bus,&io_status,0x8c600ffeu,2);if(io_status)return io_status;goto L8c600f44; /* mov.w 0x8c600ffe,r10 */
L8c600f44: *stop=0x8c600f44u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601000u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c600f46; /* mov.l 0x8c601000,r8 */
L8c600f46: *stop=0x8c600f48u;r[13]=r[4];goto L8c600f82; /* bra 0x8c600f82 */
L8c600f4a: *stop=0x8c600f4au;r[0]=r[9];goto L8c600f4c; /* mov r9,r0 */
L8c600f4c: *stop=0x8c600f4cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c600f4e; /* cmp/eq #0x1,r0 */
L8c600f4e: if(!(*sr&1u))goto L8c600f5c;goto L8c600f50; /* bf 0x8c600f5c */
L8c600f50: *stop=0x8c600f50u;r[5]=r[12];goto L8c600f52; /* mov r12,r5 */
L8c600f52: *stop=0x8c600f52u;r[6]=r[11];goto L8c600f54; /* mov r11,r6 */
L8c600f54: target=0x8c601084u;*pr=0x8c600f58u;*stop=0x8c600f56u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600f58; /* bsr 0x8c601084 */
L8c600f58: *stop=0x8c600f5au;goto L8c600f64; /* bra 0x8c600f64 */
L8c600f5c: *stop=0x8c600f5cu;r[5]=r[12];goto L8c600f5e; /* mov r12,r5 */
L8c600f5e: *stop=0x8c600f5eu;r[6]=r[11];goto L8c600f60; /* mov r11,r6 */
L8c600f60: target=0x8c600facu;*pr=0x8c600f64u;*stop=0x8c600f62u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600f64; /* bsr 0x8c600fac */
L8c600f64: *stop=0x8c600f64u;r[4]=r[0];goto L8c600f66; /* mov r0,r4 */
L8c600f66: *stop=0x8c600f66u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c600f68; /* tst r4,r4 */
L8c600f68: if(!(*sr&1u))goto L8c600f86;goto L8c600f6a; /* bf 0x8c600f86 */
L8c600f6a: *stop=0x8c600f6au;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c600f6c; /* tst r12,r12 */
L8c600f6c: if((*sr&1u))goto L8c600f86;goto L8c600f6e; /* bt 0x8c600f86 */
L8c600f6e: *stop=0x8c600f6eu;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c600f70; /* mov.l @(0x14,r14),r2 */
L8c600f70: *stop=0x8c600f70u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c600f72; /* mov.l @(0x10,r14),r3 */
L8c600f72: *stop=0x8c600f72u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);goto L8c600f74; /* cmp/ge r3,r2 */
L8c600f74: if((*sr&1u))goto L8c600f86;goto L8c600f76; /* bt 0x8c600f86 */
L8c600f76: *stop=0x8c600f78u;r[5]=r[8];goto L8c600f7c; /* bra 0x8c600f7c */
L8c600f7a: *stop=0x8c600f7au;r[5]+=4294967295u;goto L8c600f7c; /* add #-0x1,r5 */
L8c600f7c: *stop=0x8c600f7cu;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c600f7e; /* tst r5,r5 */
L8c600f7e:
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c600f7eu;if(!(*sr&1u) && ++delay_visits>=2u)return -4;
#endif
 if(!(*sr&1u))goto L8c600f7a;goto L8c600f80; /* bf 0x8c600f7a */
L8c600f80: *stop=0x8c600f80u;r[13]+=1u;goto L8c600f82; /* add #0x1,r13 */
L8c600f82: *stop=0x8c600f82u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[10]);goto L8c600f84; /* cmp/ge r10,r13 */
L8c600f84: if(!(*sr&1u))goto L8c600f4a;goto L8c600f86; /* bf 0x8c600f4a */
L8c600f86: *stop=0x8c600f86u;*sr=(*sr&~1u)|(r[10]==r[13]);goto L8c600f88; /* cmp/eq r10,r13 */
L8c600f88: if(!(*sr&1u))goto L8c600f8e;goto L8c600f8a; /* bf 0x8c600f8e */
L8c600f8a: *stop=0x8c600f8cu;r[0]=4294967275u;goto L8c600f9a; /* bra 0x8c600f9a */
L8c600f8e: *stop=0x8c600f8eu;*sr=(*sr&~1u)|((int32_t)r[4]>=0);goto L8c600f90; /* cmp/pz r4 */
L8c600f90: if(!(*sr&1u))goto L8c600f98;goto L8c600f92; /* bf 0x8c600f98 */
L8c600f92: target=0x8c600cf0u;*pr=0x8c600f96u;*stop=0x8c600f94u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600f96; /* bsr 0x8c600cf0 */
L8c600f96: *stop=0x8c600f96u;r[4]=r[0];goto L8c600f98; /* mov r0,r4 */
L8c600f98: *stop=0x8c600f98u;r[0]=r[4];goto L8c600f9a; /* mov r4,r0 */
L8c600f9a: *stop=0x8c600f9au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c600f9c; /* lds.l @r15+,PR */
L8c600f9c: *stop=0x8c600f9cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c600f9e; /* mov.l @r15+,r8 */
L8c600f9e: *stop=0x8c600f9eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c600fa0; /* mov.l @r15+,r9 */
L8c600fa0: *stop=0x8c600fa0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c600fa2; /* mov.l @r15+,r10 */
L8c600fa2: *stop=0x8c600fa2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c600fa4; /* mov.l @r15+,r11 */
L8c600fa4: *stop=0x8c600fa4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c600fa6; /* mov.l @r15+,r12 */
L8c600fa6: *stop=0x8c600fa6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c600fa8; /* mov.l @r15+,r13 */
L8c600fa8: *stop=0x8c600fa8u;if(*pr!=entry_pr)return -3;*stop=0x8c600faau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c600df4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c600df4;
L8c600df4: *stop=0x8c600df4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c600df6; /* mov.l r14,@-r15 */
L8c600df6: *stop=0x8c600df6u;r[14]=r[4];goto L8c600df8; /* mov r4,r14 */
L8c600df8: *stop=0x8c600df8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c600dfa; /* sts.l PR,@-r15 */
L8c600dfa: *stop=0x8c600dfau;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c600dfc; /* tst r14,r14 */
L8c600dfc: if((*sr&1u))goto L8c600e26;goto L8c600dfe; /* bt 0x8c600e26 */
L8c600dfe: *stop=0x8c600dfeu;r[0]=76u;goto L8c600e00; /* mov #0x4c,r0 */
L8c600e00: *stop=0x8c600e00u;if(!valid_access(r[0]+r[14],2u))return -3;r[2]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c600e02; /* mov.w @(r0,r14),r2 */
L8c600e02: *stop=0x8c600e02u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c600e04; /* tst r2,r2 */
L8c600e04: if((*sr&1u))goto L8c600e26;goto L8c600e06; /* bt 0x8c600e26 */
L8c600e06: *stop=0x8c600e06u;r[0]=80u;goto L8c600e08; /* mov #0x50,r0 */
L8c600e08: *stop=0x8c600e08u;if(!valid_access(r[0]+r[14],2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c600e0a; /* mov.w @(r0,r14),r0 */
L8c600e0a: *stop=0x8c600e0au;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c600e0c; /* cmp/eq #0x2,r0 */
L8c600e0c: take=(*sr&1u);*stop=0x8c600e0eu;r[4]=r[0];if(take)goto L8c600e1c;goto L8c600e10; /* bt/s 0x8c600e1c */
L8c600e10: *stop=0x8c600e10u;r[0]=r[4];goto L8c600e12; /* mov r4,r0 */
L8c600e12: *stop=0x8c600e12u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c600e14; /* cmp/eq #0x3,r0 */
L8c600e14: if((*sr&1u))goto L8c600e1c;goto L8c600e16; /* bt 0x8c600e1c */
L8c600e16: *stop=0x8c600e16u;r[0]=r[4];goto L8c600e18; /* mov r4,r0 */
L8c600e18: *stop=0x8c600e18u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c600e1a; /* cmp/eq #0x4,r0 */
L8c600e1a: if(!(*sr&1u))goto L8c600e20;goto L8c600e1c; /* bf 0x8c600e20 */
L8c600e1c: target=0x8c6013d2u;*pr=0x8c600e20u;*stop=0x8c600e1eu;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600e20; /* bsr 0x8c6013d2 */
L8c600e20: *stop=0x8c600e20u;r[0]=76u;goto L8c600e22; /* mov #0x4c,r0 */
L8c600e22: *stop=0x8c600e22u;r[3]=0u;goto L8c600e24; /* mov #0x0,r3 */
L8c600e24: *stop=0x8c600e24u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[3],2);if(io_status)return io_status;goto L8c600e26; /* mov.w r3,@(r0,r14) */
L8c600e26: *stop=0x8c600e26u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c600e28; /* lds.l @r15+,PR */
L8c600e28: *stop=0x8c600e28u;if(*pr!=entry_pr)return -3;*stop=0x8c600e2au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c60434c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60434c;
L8c60434c: *stop=0x8c60434cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c604368u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60434e; /* mov.l 0x8c604368,r2 */
L8c60434e: *stop=0x8c60434eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c604350; /* mov.l @r2,r3 */
L8c604350: target=r[3];*stop=0x8c604352u;r[4]=1u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_boot_file_8c60178c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c60178c;
L8c60178c: *stop=0x8c60178cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601818u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60178e; /* mov.l 0x8c601818,r3 */
L8c60178e: *stop=0x8c60178eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c601790; /* mov.l r14,@-r15 */
L8c601790: *stop=0x8c601790u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[14]=loaded;}goto L8c601792; /* mov.l @r3,r14 */
L8c601792: *stop=0x8c601792u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c601794; /* tst r14,r14 */
L8c601794: if((*sr&1u))goto L8c60179e;goto L8c601796; /* bt 0x8c60179e */
L8c601796: *stop=0x8c601796u;r[0]=84u;goto L8c601798; /* mov #0x54,r0 */
L8c601798: *stop=0x8c601798u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c60179a; /* mov.l @(r0,r14),r0 */
L8c60179a: target=r[0];*stop=0x8c60179cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
L8c60179e: *stop=0x8c60179eu;r[0]=0u;goto L8c6017a0; /* mov #0x0,r0 */
L8c6017a0: *stop=0x8c6017a0u;if(*pr!=entry_pr)return -3;*stop=0x8c6017a2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6017a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6017a4;
L8c6017a4: *stop=0x8c6017a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601818u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6017a6; /* mov.l 0x8c601818,r3 */
L8c6017a6: *stop=0x8c6017a6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6017a8; /* mov.l r14,@-r15 */
L8c6017a8: *stop=0x8c6017a8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[14]=loaded;}goto L8c6017aa; /* mov.l @r3,r14 */
L8c6017aa: *stop=0x8c6017aau;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6017ac; /* tst r14,r14 */
L8c6017ac: if((*sr&1u))goto L8c6017b6;goto L8c6017ae; /* bt 0x8c6017b6 */
L8c6017ae: *stop=0x8c6017aeu;r[0]=88u;goto L8c6017b0; /* mov #0x58,r0 */
L8c6017b0: *stop=0x8c6017b0u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6017b2; /* mov.l @(r0,r14),r0 */
L8c6017b2: target=r[0];*stop=0x8c6017b4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
L8c6017b6: *stop=0x8c6017b6u;r[0]=0u;goto L8c6017b8; /* mov #0x0,r0 */
L8c6017b8: *stop=0x8c6017b8u;if(*pr!=entry_pr)return -3;*stop=0x8c6017bau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c600240(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c600240;
L8c600240: *stop=0x8c600240u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c600242; /* mov.l r14,@-r15 */
L8c600242: *stop=0x8c600242u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c600244; /* mov.l r13,@-r15 */
L8c600244: *stop=0x8c600244u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c600246; /* mov.l r12,@-r15 */
L8c600246: *stop=0x8c600246u;{uint32_t loaded=rd(m,bus,&io_status,0x8c600344u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c600248; /* mov.l 0x8c600344,r1 */
L8c600248: *stop=0x8c600248u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c60024a; /* mov.l r11,@-r15 */
L8c60024a: *stop=0x8c60024au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c60024c; /* mov.l r10,@-r15 */
L8c60024c: *stop=0x8c60024cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c60024e; /* sts.l PR,@-r15 */
L8c60024e: *stop=0x8c60024eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}goto L8c600250; /* mov.l @r1,r4 */
L8c600250: *stop=0x8c600250u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c600252; /* tst r4,r4 */
L8c600252: if((*sr&1u))goto L8c60025a;goto L8c600254; /* bt 0x8c60025a */
L8c600254: *stop=0x8c600254u;{uint32_t loaded=rd(m,bus,&io_status,0x8c600348u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c600256; /* mov.l 0x8c600348,r2 */
L8c600256: target=r[2];*pr=0x8c60025au;*stop=0x8c600258u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60025a; /* jsr @r2 */
L8c60025a: *stop=0x8c60025au;{uint32_t loaded=rd(m,bus,&io_status,0x8c60034cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60025c; /* mov.l 0x8c60034c,r3 */
L8c60025c: *stop=0x8c60025cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[4]=loaded;}goto L8c60025e; /* mov.l @r3,r4 */
L8c60025e: *stop=0x8c60025eu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c600260; /* tst r4,r4 */
L8c600260: if((*sr&1u))goto L8c600268;goto L8c600262; /* bt 0x8c600268 */
L8c600262: *stop=0x8c600262u;{uint32_t loaded=rd(m,bus,&io_status,0x8c600348u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c600264; /* mov.l 0x8c600348,r1 */
L8c600264: target=r[1];*pr=0x8c600268u;*stop=0x8c600266u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600268; /* jsr @r1 */
L8c600268: *stop=0x8c600268u;{uint32_t loaded=rd(m,bus,&io_status,0x8c600354u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c60026a; /* mov.l 0x8c600354,r11 */
L8c60026a: *stop=0x8c60026au;r[12]=0u;goto L8c60026c; /* mov #0x0,r12 */
L8c60026c: *stop=0x8c60026cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c60034cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60026e; /* mov.l 0x8c60034c,r2 */
L8c60026e: *stop=0x8c60026eu;r[14]=r[12];goto L8c600270; /* mov r12,r14 */
L8c600270: *stop=0x8c600270u;{uint32_t loaded=rd(m,bus,&io_status,0x8c600344u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c600272; /* mov.l 0x8c600344,r3 */
L8c600272: *stop=0x8c600272u;{uint32_t loaded=rd(m,bus,&io_status,0x8c600350u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c600274; /* mov.l 0x8c600350,r10 */
L8c600274: *stop=0x8c600274u;r[1]=sx(m,bus,&io_status,0x8c60033eu,2);if(io_status)return io_status;goto L8c600276; /* mov.w 0x8c60033e,r1 */
L8c600276: *stop=0x8c600276u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[12],4);if(io_status)return io_status;goto L8c600278; /* mov.l r12,@r3 */
L8c600278: *stop=0x8c600278u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[12],4);if(io_status)return io_status;goto L8c60027a; /* mov.l r12,@r2 */
L8c60027a: *stop=0x8c60027au;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[13]=loaded;}goto L8c60027c; /* mov.l @r11,r13 */
L8c60027c: *stop=0x8c60027eu;r[13]+=r[1];goto L8c600288; /* bra 0x8c600288 */
L8c600280: target=r[10];*pr=0x8c600284u;*stop=0x8c600282u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600284; /* jsr @r10 */
L8c600284: *stop=0x8c600284u;r[14]+=1u;goto L8c600286; /* add #0x1,r14 */
L8c600286: *stop=0x8c600286u;r[13]+=84u;goto L8c600288; /* add #0x54,r13 */
L8c600288: *stop=0x8c600288u;if(!valid_access(r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60028a; /* mov.l @r11,r2 */
L8c60028a: *stop=0x8c60028au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60028c; /* mov.l @r2,r3 */
L8c60028c: *stop=0x8c60028cu;r[3]+=1u;goto L8c60028e; /* add #0x1,r3 */
L8c60028e: *stop=0x8c60028eu;*sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[3]);goto L8c600290; /* cmp/ge r3,r14 */
L8c600290: if(!(*sr&1u))goto L8c600280;goto L8c600292; /* bf 0x8c600280 */
L8c600292: *stop=0x8c600292u;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[12],4);if(io_status)return io_status;goto L8c600294; /* mov.l r12,@r11 */
L8c600294: *stop=0x8c600294u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c600296; /* lds.l @r15+,PR */
L8c600296: *stop=0x8c600296u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c600298; /* mov.l @r15+,r10 */
L8c600298: *stop=0x8c600298u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c60029a; /* mov.l @r15+,r11 */
L8c60029a: *stop=0x8c60029au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c60029c; /* mov.l @r15+,r12 */
L8c60029c: *stop=0x8c60029cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c60029e; /* mov.l @r15+,r13 */
L8c60029e: *stop=0x8c60029eu;if(*pr!=entry_pr)return -3;*stop=0x8c6002a0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6004d4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6004d4;
L8c6004d4: *stop=0x8c6004d4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6004d6; /* mov.l r14,@-r15 */
L8c6004d6: *stop=0x8c6004d6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6004d8; /* mov.l r13,@-r15 */
L8c6004d8: *stop=0x8c6004d8u;r[13]=r[5];goto L8c6004da; /* mov r5,r13 */
L8c6004da: *stop=0x8c6004dau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6004dc; /* mov.l r12,@-r15 */
L8c6004dc: *stop=0x8c6004dcu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c6004de; /* tst r13,r13 */
L8c6004de: *stop=0x8c6004deu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6004e0; /* sts.l PR,@-r15 */
L8c6004e0: take=!(*sr&1u);*stop=0x8c6004e2u;r[6]=0u;if(take)goto L8c6004ea;goto L8c6004e4; /* bf/s 0x8c6004ea */
L8c6004e4: *stop=0x8c6004e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60059cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6004e6; /* mov.l 0x8c60059c,r3 */
L8c6004e6: *stop=0x8c6004e6u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[13]=loaded;}goto L8c6004e8; /* mov.l @r3,r13 */
L8c6004e8: *stop=0x8c6004e8u;if(!valid_access(r[13]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x10u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c6004ea; /* mov.l @(0x10,r13),r13 */
L8c6004ea: *stop=0x8c6004eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6005a0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6004ec; /* mov.l 0x8c6005a0,r2 */
L8c6004ec: target=r[2];*pr=0x8c6004f0u;*stop=0x8c6004eeu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6004f0; /* jsr @r2 */
L8c6004f0: *stop=0x8c6004f0u;r[5]=r[0];goto L8c6004f2; /* mov r0,r5 */
L8c6004f2: *stop=0x8c6004f2u;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c6004f4; /* tst r5,r5 */
L8c6004f4: if(!(*sr&1u))goto L8c6004fe;goto L8c6004f6; /* bf 0x8c6004fe */
L8c6004f6: target=0x8c5ffa08u;*pr=0x8c6004fau;*stop=0x8c6004f8u;r[4]=4294967288u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6004fa; /* bsr 0x8c5ffa08 */
L8c6004fa: *stop=0x8c6004fcu;goto L8c600540; /* bra 0x8c600540 */
L8c6004fe: *stop=0x8c6004feu;r[7]=r[5];goto L8c600500; /* mov r5,r7 */
L8c600500: *stop=0x8c600500u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60059cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c600502; /* mov.l 0x8c60059c,r2 */
L8c600502: *stop=0x8c600502u;r[7]+=4294967295u;goto L8c600504; /* add #-0x1,r7 */
L8c600504: *stop=0x8c600504u;r[12]=sx(m,bus,&io_status,0x8c600596u,2);if(io_status)return io_status;goto L8c600506; /* mov.w 0x8c600596,r12 */
L8c600506: *stop=0x8c600506u;r[3]=44u;goto L8c600508; /* mov #0x2c,r3 */
L8c600508: *stop=0x8c600508u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[6]=loaded;}goto L8c60050a; /* mov.l @r2,r6 */
L8c60050a: *stop=0x8c60050au;*macl=r[3]*r[7];goto L8c60050c; /* mul.l r3,r7 */
L8c60050c: *stop=0x8c60050cu;if(!valid_access(r[6]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x18u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c60050e; /* mov.l @(0x18,r6),r14 */
L8c60050e: *stop=0x8c60050eu;r[4]=0u;goto L8c600510; /* mov #0x0,r4 */
L8c600510: *stop=0x8c600512u;r[7]=*macl;goto L8c600536; /* bra 0x8c600536 */
L8c600514: *stop=0x8c600514u;r[0]=76u;goto L8c600516; /* mov #0x4c,r0 */
L8c600516: *stop=0x8c600516u;if(!valid_access(r[0]+r[14],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c600518; /* mov.w @(r0,r14),r3 */
L8c600518: *stop=0x8c600518u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c60051a; /* tst r3,r3 */
L8c60051a: if(!(*sr&1u))goto L8c600532;goto L8c60051c; /* bf 0x8c600532 */
L8c60051c: *stop=0x8c60051cu;r[13]+=16u;goto L8c60051e; /* add #0x10,r13 */
L8c60051e: *stop=0x8c60051eu;r[13]+=r[7];goto L8c600520; /* add r7,r13 */
L8c600520: *stop=0x8c600520u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[6]=loaded;}goto L8c600522; /* mov.l @r13,r6 */
L8c600522: *stop=0x8c600522u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c600524; /* mov.l @(0x4,r13),r7 */
L8c600524: *stop=0x8c600524u;r[6]+=r[12];goto L8c600526; /* add r12,r6 */
L8c600526: target=0x8c600464u;*pr=0x8c60052au;*stop=0x8c600528u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60052a; /* bsr 0x8c600464 */
L8c60052a: target=0x8c5ffa08u;*pr=0x8c60052eu;*stop=0x8c60052cu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60052e; /* bsr 0x8c5ffa08 */
L8c60052e: *stop=0x8c600530u;r[0]=r[14];goto L8c600542; /* bra 0x8c600542 */
L8c600532: *stop=0x8c600532u;r[14]+=84u;goto L8c600534; /* add #0x54,r14 */
L8c600534: *stop=0x8c600534u;r[4]+=1u;goto L8c600536; /* add #0x1,r4 */
L8c600536: *stop=0x8c600536u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[3]=loaded;}goto L8c600538; /* mov.l @r6,r3 */
L8c600538: *stop=0x8c600538u;*sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]);goto L8c60053a; /* cmp/ge r3,r4 */
L8c60053a: if(!(*sr&1u))goto L8c600514;goto L8c60053c; /* bf 0x8c600514 */
L8c60053c: target=0x8c5ffa08u;*pr=0x8c600540u;*stop=0x8c60053eu;r[4]=4294967287u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600540; /* bsr 0x8c5ffa08 */
L8c600540: *stop=0x8c600540u;r[0]=0u;goto L8c600542; /* mov #0x0,r0 */
L8c600542: *stop=0x8c600542u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c600544; /* lds.l @r15+,PR */
L8c600544: *stop=0x8c600544u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c600546; /* mov.l @r15+,r12 */
L8c600546: *stop=0x8c600546u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c600548; /* mov.l @r15+,r13 */
L8c600548: *stop=0x8c600548u;if(*pr!=entry_pr)return -3;*stop=0x8c60054au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c600688(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c600688;
L8c600688: *stop=0x8c600688u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c60068a; /* sts.l PR,@-r15 */
L8c60068a: *stop=0x8c60068au;r[15]+=4294967288u;goto L8c60068c; /* add #-0x8,r15 */
L8c60068c: *stop=0x8c60068cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6006b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60068e; /* mov.l 0x8c6006b4,r2 */
L8c60068e: *stop=0x8c60068eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c600690; /* mov.l @r2,r3 */
L8c600690: *stop=0x8c600690u;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c600692; /* mov.l @(0xc,r3),r1 */
L8c600692: *stop=0x8c600692u;if(!valid_access(r[1]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x20u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c600694; /* mov.l @(0x20,r1),r0 */
L8c600694: target=r[0];*pr=0x8c600698u;*stop=0x8c600696u;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600698; /* jsr @r0 */
L8c600698: *stop=0x8c600698u;r[4]=r[0];goto L8c60069a; /* mov r0,r4 */
L8c60069a: *stop=0x8c60069au;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c60069c; /* tst r4,r4 */
L8c60069c: if(!(*sr&1u))goto L8c6006a6;goto L8c60069e; /* bf 0x8c6006a6 */
L8c60069e: *stop=0x8c60069eu;r[15]+=8u;goto L8c6006a0; /* add #0x8,r15 */
L8c6006a0: *stop=0x8c6006a0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6006a2; /* lds.l @r15+,PR */
L8c6006a2: *stop=0x8c6006a2u;if(*pr!=entry_pr)return -3;*stop=0x8c6006a4u;r[0]=4294967295u;return 0; /* rts */
L8c6006a6: *stop=0x8c6006a6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6006a8; /* mov.l @r15,r0 */
L8c6006a8: *stop=0x8c6006a8u;r[15]+=8u;goto L8c6006aa; /* add #0x8,r15 */
L8c6006aa: *stop=0x8c6006aau;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6006ac; /* lds.l @r15+,PR */
L8c6006ac: *stop=0x8c6006acu;if(*pr!=entry_pr)return -3;*stop=0x8c6006aeu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6006c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6006c0;
L8c6006c0: *stop=0x8c6006c0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6006c2; /* mov.l r14,@-r15 */
L8c6006c2: *stop=0x8c6006c2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6006c4; /* mov.l r13,@-r15 */
L8c6006c4: *stop=0x8c6006c4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6006c6; /* mov.l r12,@-r15 */
L8c6006c6: *stop=0x8c6006c6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6006c8; /* sts.l PR,@-r15 */
L8c6006c8: *stop=0x8c6006c8u;r[15]+=4294967292u;goto L8c6006ca; /* add #-0x4,r15 */
L8c6006ca: *stop=0x8c6006cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c600788u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c6006cc; /* mov.l 0x8c600788,r12 */
L8c6006cc: *stop=0x8c6006ccu;r[7]=0u;goto L8c6006ce; /* mov #0x0,r7 */
L8c6006ce: *stop=0x8c6006ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c60078cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6006d0; /* mov.l 0x8c60078c,r3 */
L8c6006d0: *stop=0x8c6006d0u;r[6]=r[7];goto L8c6006d2; /* mov r7,r6 */
L8c6006d2: *stop=0x8c6006d2u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[4]=loaded;}goto L8c6006d4; /* mov.l @r12,r4 */
L8c6006d4: *stop=0x8c6006d4u;r[5]=r[7];goto L8c6006d6; /* mov r7,r5 */
L8c6006d6: target=r[3];*pr=0x8c6006dau;*stop=0x8c6006d8u;if(!valid_access(r[4]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x14u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6006da; /* jsr @r3 */
L8c6006da: *stop=0x8c6006dau;r[14]=r[0];goto L8c6006dc; /* mov r0,r14 */
L8c6006dc: *stop=0x8c6006dcu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6006de; /* tst r14,r14 */
L8c6006de: if((*sr&1u))goto L8c60073e;goto L8c6006e0; /* bt 0x8c60073e */
L8c6006e0: *stop=0x8c6006e0u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c6006e2; /* stc SR,r0 */
L8c6006e2: *stop=0x8c6006e2u;r[3]=sx(m,bus,&io_status,0x8c600784u,2);if(io_status)return io_status;goto L8c6006e4; /* mov.w 0x8c600784,r3 */
L8c6006e4: *stop=0x8c6006e4u;r[0]>>=2;goto L8c6006e6; /* shlr2 r0 */
L8c6006e6: *stop=0x8c6006e6u;r[0]>>=2;goto L8c6006e8; /* shlr2 r0 */
L8c6006e8: *stop=0x8c6006e8u;r[0]&=0xfu;goto L8c6006ea; /* and #0xf,r0 */
L8c6006ea: *stop=0x8c6006eau;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c6006ec; /* mov.l r0,@r15 */
L8c6006ec: *stop=0x8c6006ecu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c6006ee; /* stc SR,r0 */
L8c6006ee: *stop=0x8c6006eeu;r[0]&=r[3];goto L8c6006f0; /* and r3,r0 */
L8c6006f0: *stop=0x8c6006f0u;r[0]|=0xf0u;goto L8c6006f2; /* or #0xf0,r0 */
L8c6006f2: *stop=0x8c6006f2u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c6006f4; /* ldc r0,SR */
L8c6006f4: *stop=0x8c6006f4u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[13]=loaded;}goto L8c6006f6; /* mov.l @r12,r13 */
L8c6006f6: *stop=0x8c6006f6u;r[5]=14u;goto L8c6006f8; /* mov #0xe,r5 */
L8c6006f8: *stop=0x8c6006f8u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6006fa; /* mov.l @r12,r6 */
L8c6006fa: *stop=0x8c6006fau;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c6006fc; /* mov.l @(0xc,r13),r13 */
L8c6006fc: *stop=0x8c6006fcu;r[6]+=64u;goto L8c6006fe; /* add #0x40,r6 */
L8c6006fe: *stop=0x8c6006feu;if(!valid_access(r[13]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x3cu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c600700; /* mov.l @(0x3c,r13),r13 */
L8c600700: target=r[13];*pr=0x8c600704u;*stop=0x8c600702u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600704; /* jsr @r13 */
L8c600704: *stop=0x8c600704u;r[13]=r[0];goto L8c600706; /* mov r0,r13 */
L8c600706: *stop=0x8c600706u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c600708; /* tst r13,r13 */
L8c600708: if((*sr&1u))goto L8c600728;goto L8c60070a; /* bt 0x8c600728 */
L8c60070a: *stop=0x8c60070au;r[0]=80u;goto L8c60070c; /* mov #0x50,r0 */
L8c60070c: *stop=0x8c60070cu;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[13],4);if(io_status)return io_status;goto L8c60070e; /* mov.l r13,@(0x3c,r14) */
L8c60070e: *stop=0x8c60070eu;r[2]=4u;goto L8c600710; /* mov #0x4,r2 */
L8c600710: *stop=0x8c600710u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[2],2);if(io_status)return io_status;goto L8c600712; /* mov.w r2,@(r0,r14) */
L8c600712: *stop=0x8c600712u;r[0]=72u;goto L8c600714; /* mov #0x48,r0 */
L8c600714: *stop=0x8c600714u;r[3]=10u;goto L8c600716; /* mov #0xa,r3 */
L8c600716: *stop=0x8c600716u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[3],2);if(io_status)return io_status;goto L8c600718; /* mov.w r3,@(r0,r14) */
L8c600718: *stop=0x8c600718u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60071a; /* mov.l @r14,r2 */
L8c60071a: *stop=0x8c60071au;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[14],4);if(io_status)return io_status;goto L8c60071c; /* mov.l r14,@(0x1c,r2) */
L8c60071c: *stop=0x8c60071cu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60071e; /* mov.l @r12,r3 */
L8c60071e: *stop=0x8c60071eu;r[0]=64u;goto L8c600720; /* mov #0x40,r0 */
L8c600720: *stop=0x8c600720u;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c600722; /* mov.l @(0xc,r3),r2 */
L8c600722: *stop=0x8c600722u;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c600724; /* mov.l @(r0,r2),r1 */
L8c600724: target=r[1];*pr=0x8c600728u;*stop=0x8c600726u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600728; /* jsr @r1 */
L8c600728: *stop=0x8c600728u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c60072a; /* mov.l @r15,r0 */
L8c60072a: *stop=0x8c60072au;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c60072c; /* stc SR,r3 */
L8c60072c: *stop=0x8c60072cu;r[2]=sx(m,bus,&io_status,0x8c600784u,2);if(io_status)return io_status;goto L8c60072e; /* mov.w 0x8c600784,r2 */
L8c60072e: *stop=0x8c60072eu;r[0]&=0xfu;goto L8c600730; /* and #0xf,r0 */
L8c600730: *stop=0x8c600730u;r[0]<<=2;goto L8c600732; /* shll2 r0 */
L8c600732: *stop=0x8c600732u;r[3]&=r[2];goto L8c600734; /* and r2,r3 */
L8c600734: *stop=0x8c600734u;r[0]<<=2;goto L8c600736; /* shll2 r0 */
L8c600736: *stop=0x8c600736u;r[0]|=r[3];goto L8c600738; /* or r3,r0 */
L8c600738: *stop=0x8c600738u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c60073a; /* ldc r0,SR */
L8c60073a: *stop=0x8c60073au;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c60073c; /* tst r13,r13 */
L8c60073c: if(!(*sr&1u))goto L8c600742;goto L8c60073e; /* bf 0x8c600742 */
L8c60073e: *stop=0x8c600740u;r[0]=4294967283u;goto L8c600744; /* bra 0x8c600744 */
L8c600742: *stop=0x8c600742u;r[0]=0u;goto L8c600744; /* mov #0x0,r0 */
L8c600744: *stop=0x8c600744u;r[15]+=4u;goto L8c600746; /* add #0x4,r15 */
L8c600746: *stop=0x8c600746u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c600748; /* lds.l @r15+,PR */
L8c600748: *stop=0x8c600748u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c60074a; /* mov.l @r15+,r12 */
L8c60074a: *stop=0x8c60074au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c60074c; /* mov.l @r15+,r13 */
L8c60074c: *stop=0x8c60074cu;if(*pr!=entry_pr)return -3;*stop=0x8c60074eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c601084(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c601084;
L8c601084: *stop=0x8c601084u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c601086; /* mov.l r14,@-r15 */
L8c601086: *stop=0x8c601086u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c601088; /* mov.l r13,@-r15 */
L8c601088: *stop=0x8c601088u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c60108a; /* mov.l r12,@-r15 */
L8c60108a: *stop=0x8c60108au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c60108c; /* mov.l r11,@-r15 */
L8c60108c: *stop=0x8c60108cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c60108e; /* mov.l r10,@-r15 */
L8c60108e: *stop=0x8c60108eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c601090; /* sts.l PR,@-r15 */
L8c601090: *stop=0x8c601090u;r[15]+=4294967292u;goto L8c601092; /* add #-0x4,r15 */
L8c601092: *stop=0x8c601092u;r[14]=r[4];goto L8c601094; /* mov r4,r14 */
L8c601094: *stop=0x8c601094u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c601096; /* tst r14,r14 */
L8c601096: take=(*sr&1u);*stop=0x8c601098u;r[13]=r[5];if(take)goto L8c6010a2;goto L8c60109a; /* bt/s 0x8c6010a2 */
L8c60109a: *stop=0x8c60109au;r[0]=76u;goto L8c60109c; /* mov #0x4c,r0 */
L8c60109c: *stop=0x8c60109cu;if(!valid_access(r[0]+r[14],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c60109e; /* mov.w @(r0,r14),r3 */
L8c60109e: *stop=0x8c60109eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6010a0; /* tst r3,r3 */
L8c6010a0: if(!(*sr&1u))goto L8c6010a6;goto L8c6010a2; /* bf 0x8c6010a6 */
L8c6010a2: *stop=0x8c6010a4u;r[0]=4294967286u;goto L8c60115e; /* bra 0x8c60115e */
L8c6010a6: *stop=0x8c6010a6u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6010a8; /* mov.l @r14,r3 */
L8c6010a8: *stop=0x8c6010a8u;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6010aa; /* mov.l @(0x1c,r3),r2 */
L8c6010aa: *stop=0x8c6010aau;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6010ac; /* tst r2,r2 */
L8c6010ac: if(!(*sr&1u))goto L8c60115c;goto L8c6010ae; /* bf 0x8c60115c */
L8c6010ae: *stop=0x8c6010aeu;r[3]=31u;goto L8c6010b0; /* mov #0x1f,r3 */
L8c6010b0: *stop=0x8c6010b0u;*sr=(*sr&~1u)|!(r[6]&r[3]);goto L8c6010b2; /* tst r6,r3 */
L8c6010b2: if((*sr&1u))goto L8c6010b8;goto L8c6010b4; /* bt 0x8c6010b8 */
L8c6010b4: *stop=0x8c6010b6u;r[0]=4294967282u;goto L8c60115e; /* bra 0x8c60115e */
L8c6010b8: *stop=0x8c6010b8u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c6010ba; /* mov.l @(0x14,r14),r12 */
L8c6010ba: *stop=0x8c6010bau;r[10]=0u;goto L8c6010bc; /* mov #0x0,r10 */
L8c6010bc: *stop=0x8c6010bcu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c6010be; /* mov.l @(0x8,r14),r4 */
L8c6010be: *stop=0x8c6010beu;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6010c0; /* mov.l @(0x10,r14),r3 */
L8c6010c0: *stop=0x8c6010c0u;r[12]+=r[4];goto L8c6010c2; /* add r4,r12 */
L8c6010c2: *stop=0x8c6010c2u;r[4]+=r[3];goto L8c6010c4; /* add r3,r4 */
L8c6010c4: *stop=0x8c6010c4u;*sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[4]);goto L8c6010c6; /* cmp/ge r4,r12 */
L8c6010c6: take=!(*sr&1u);*stop=0x8c6010c8u;r[11]=1u;if(take)goto L8c6010d2;goto L8c6010ca; /* bf/s 0x8c6010d2 */
L8c6010ca: *stop=0x8c6010cau;r[0]=80u;goto L8c6010cc; /* mov #0x50,r0 */
L8c6010cc: *stop=0x8c6010ccu;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[10],4);if(io_status)return io_status;goto L8c6010ce; /* mov.l r10,@(0x18,r14) */
L8c6010ce: *stop=0x8c6010d0u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[11],2);if(io_status)return io_status;goto L8c60115c; /* bra 0x8c60115c */
L8c6010d2: *stop=0x8c6010d2u;r[2]=r[13];goto L8c6010d4; /* mov r13,r2 */
L8c6010d4: *stop=0x8c6010d4u;r[2]+=r[12];goto L8c6010d6; /* add r12,r2 */
L8c6010d6: *stop=0x8c6010d6u;*sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[4]);goto L8c6010d8; /* cmp/gt r4,r2 */
L8c6010d8: if(!(*sr&1u))goto L8c6010de;goto L8c6010da; /* bf 0x8c6010de */
L8c6010da: *stop=0x8c6010dau;r[13]=r[4];goto L8c6010dc; /* mov r4,r13 */
L8c6010dc: *stop=0x8c6010dcu;r[13]-=r[12];goto L8c6010de; /* sub r12,r13 */
L8c6010de: *stop=0x8c6010deu;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c6010e0; /* tst r13,r13 */
L8c6010e0: if(!(*sr&1u))goto L8c6010ec;goto L8c6010e2; /* bf 0x8c6010ec */
L8c6010e2: *stop=0x8c6010e2u;r[0]=80u;goto L8c6010e4; /* mov #0x50,r0 */
L8c6010e4: *stop=0x8c6010e4u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[10],4);if(io_status)return io_status;goto L8c6010e6; /* mov.l r10,@(0x18,r14) */
L8c6010e6: *stop=0x8c6010e8u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[11],2);if(io_status)return io_status;goto L8c601158; /* bra 0x8c601158 */
L8c6010ec: *stop=0x8c6010ecu;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c6010ee; /* stc SR,r0 */
L8c6010ee: *stop=0x8c6010eeu;r[3]=sx(m,bus,&io_status,0x8c60121au,2);if(io_status)return io_status;goto L8c6010f0; /* mov.w 0x8c60121a,r3 */
L8c6010f0: *stop=0x8c6010f0u;r[0]>>=2;goto L8c6010f2; /* shlr2 r0 */
L8c6010f2: *stop=0x8c6010f2u;r[0]>>=2;goto L8c6010f4; /* shlr2 r0 */
L8c6010f4: *stop=0x8c6010f4u;r[0]&=0xfu;goto L8c6010f6; /* and #0xf,r0 */
L8c6010f6: *stop=0x8c6010f6u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c6010f8; /* mov.l r0,@r15 */
L8c6010f8: *stop=0x8c6010f8u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c6010fa; /* stc SR,r0 */
L8c6010fa: *stop=0x8c6010fau;r[0]&=r[3];goto L8c6010fc; /* and r3,r0 */
L8c6010fc: *stop=0x8c6010fcu;r[0]|=0xf0u;goto L8c6010fe; /* or #0xf0,r0 */
L8c6010fe: *stop=0x8c6010feu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c601100; /* ldc r0,SR */
L8c601100: *stop=0x8c601100u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c601102; /* mov.l @r14,r2 */
L8c601102: *stop=0x8c601102u;r[0]=78u;goto L8c601104; /* mov #0x4e,r0 */
L8c601104: *stop=0x8c601104u;if(!valid_access(r[0]+r[14],2u))return -3;r[7]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c601106; /* mov.w @(r0,r14),r7 */
L8c601106: *stop=0x8c601106u;r[5]=r[13];goto L8c601108; /* mov r13,r5 */
L8c601108: *stop=0x8c601108u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c60110a; /* mov.l @(0xc,r2),r3 */
L8c60110a: *stop=0x8c60110au;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c60110c; /* mov.l @(0x4,r3),r1 */
L8c60110c: target=r[1];*pr=0x8c601110u;*stop=0x8c60110eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601110; /* jsr @r1 */
L8c601110: *stop=0x8c601110u;r[12]=r[0];goto L8c601112; /* mov r0,r12 */
L8c601112: *stop=0x8c601112u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c601114; /* tst r12,r12 */
L8c601114: if((*sr&1u))goto L8c60113e;goto L8c601116; /* bt 0x8c60113e */
L8c601116: *stop=0x8c601116u;r[2]=2u;goto L8c601118; /* mov #0x2,r2 */
L8c601118: *stop=0x8c601118u;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[12],4);if(io_status)return io_status;goto L8c60111a; /* mov.l r12,@(0x3c,r14) */
L8c60111a: *stop=0x8c60111au;r[0]=80u;goto L8c60111c; /* mov #0x50,r0 */
L8c60111c: *stop=0x8c60111cu;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[13],4);if(io_status)return io_status;goto L8c60111e; /* mov.l r13,@(0x1c,r14) */
L8c60111e: *stop=0x8c60111eu;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[2],2);if(io_status)return io_status;goto L8c601120; /* mov.w r2,@(r0,r14) */
L8c601120: *stop=0x8c601120u;r[0]=74u;goto L8c601122; /* mov #0x4a,r0 */
L8c601122: *stop=0x8c601122u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[11],2);if(io_status)return io_status;goto L8c601124; /* mov.w r11,@(r0,r14) */
L8c601124: *stop=0x8c601124u;r[0]=72u;goto L8c601126; /* mov #0x48,r0 */
L8c601126: *stop=0x8c601126u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[11],2);if(io_status)return io_status;goto L8c601128; /* mov.w r11,@(r0,r14) */
L8c601128: *stop=0x8c601128u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[10],4);if(io_status)return io_status;goto L8c60112a; /* mov.l r10,@(0x18,r14) */
L8c60112a: *stop=0x8c60112au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c60112c; /* mov.l @r14,r3 */
L8c60112c: *stop=0x8c60112cu;if(!valid_access(r[3]+28u,4u))return -3;wr(m,bus,&io_status,r[3]+0x1cu,r[14],4);if(io_status)return io_status;goto L8c60112e; /* mov.l r14,@(0x1c,r3) */
L8c60112e: *stop=0x8c60112eu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c601130; /* mov.l @r14,r2 */
L8c601130: *stop=0x8c601130u;r[0]=64u;goto L8c601132; /* mov #0x40,r0 */
L8c601132: *stop=0x8c601132u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601134; /* mov.l @(0xc,r2),r3 */
L8c601134: *stop=0x8c601134u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c601136; /* mov.l @(r0,r3),r1 */
L8c601136: target=r[1];*pr=0x8c60113au;*stop=0x8c601138u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60113a; /* jsr @r1 */
L8c60113a: *stop=0x8c60113cu;goto L8c601142; /* bra 0x8c601142 */
L8c60113e: *stop=0x8c60113eu;r[0]=80u;goto L8c601140; /* mov #0x50,r0 */
L8c601140: *stop=0x8c601140u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[11],2);if(io_status)return io_status;goto L8c601142; /* mov.w r11,@(r0,r14) */
L8c601142: *stop=0x8c601142u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c601144; /* mov.l @r15,r0 */
L8c601144: *stop=0x8c601144u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c601146; /* stc SR,r3 */
L8c601146: *stop=0x8c601146u;r[2]=sx(m,bus,&io_status,0x8c60121au,2);if(io_status)return io_status;goto L8c601148; /* mov.w 0x8c60121a,r2 */
L8c601148: *stop=0x8c601148u;r[0]&=0xfu;goto L8c60114a; /* and #0xf,r0 */
L8c60114a: *stop=0x8c60114au;r[0]<<=2;goto L8c60114c; /* shll2 r0 */
L8c60114c: *stop=0x8c60114cu;r[3]&=r[2];goto L8c60114e; /* and r2,r3 */
L8c60114e: *stop=0x8c60114eu;r[0]<<=2;goto L8c601150; /* shll2 r0 */
L8c601150: *stop=0x8c601150u;r[0]|=r[3];goto L8c601152; /* or r3,r0 */
L8c601152: *stop=0x8c601152u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c601154; /* ldc r0,SR */
L8c601154: *stop=0x8c601154u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c601156; /* tst r12,r12 */
L8c601156: if((*sr&1u))goto L8c60115c;goto L8c601158; /* bt 0x8c60115c */
L8c601158: *stop=0x8c60115au;r[0]=r[13];goto L8c60115e; /* bra 0x8c60115e */
L8c60115c: *stop=0x8c60115cu;r[0]=0u;goto L8c60115e; /* mov #0x0,r0 */
L8c60115e: *stop=0x8c60115eu;r[15]+=4u;goto L8c601160; /* add #0x4,r15 */
L8c601160: *stop=0x8c601160u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c601162; /* lds.l @r15+,PR */
L8c601162: *stop=0x8c601162u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c601164; /* mov.l @r15+,r10 */
L8c601164: *stop=0x8c601164u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c601166; /* mov.l @r15+,r11 */
L8c601166: *stop=0x8c601166u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c601168; /* mov.l @r15+,r12 */
L8c601168: *stop=0x8c601168u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c60116a; /* mov.l @r15+,r13 */
L8c60116a: *stop=0x8c60116au;if(*pr!=entry_pr)return -3;*stop=0x8c60116cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c600fac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c600fac;
L8c600fac: *stop=0x8c600facu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c600fae; /* mov.l r14,@-r15 */
L8c600fae: *stop=0x8c600faeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c600fb0; /* mov.l r13,@-r15 */
L8c600fb0: *stop=0x8c600fb0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c600fb2; /* mov.l r12,@-r15 */
L8c600fb2: *stop=0x8c600fb2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c600fb4; /* mov.l r11,@-r15 */
L8c600fb4: *stop=0x8c600fb4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c600fb6; /* sts.l PR,@-r15 */
L8c600fb6: *stop=0x8c600fb6u;r[15]+=4294967292u;goto L8c600fb8; /* add #-0x4,r15 */
L8c600fb8: *stop=0x8c600fb8u;r[14]=r[4];goto L8c600fba; /* mov r4,r14 */
L8c600fba: *stop=0x8c600fbau;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c600fbc; /* tst r14,r14 */
L8c600fbc: take=(*sr&1u);*stop=0x8c600fbeu;r[13]=r[5];if(take)goto L8c600fc8;goto L8c600fc0; /* bt/s 0x8c600fc8 */
L8c600fc0: *stop=0x8c600fc0u;r[0]=76u;goto L8c600fc2; /* mov #0x4c,r0 */
L8c600fc2: *stop=0x8c600fc2u;if(!valid_access(r[0]+r[14],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c600fc4; /* mov.w @(r0,r14),r3 */
L8c600fc4: *stop=0x8c600fc4u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c600fc6; /* tst r3,r3 */
L8c600fc6: if(!(*sr&1u))goto L8c600fcc;goto L8c600fc8; /* bf 0x8c600fcc */
L8c600fc8: *stop=0x8c600fcau;r[0]=4294967286u;goto L8c601076; /* bra 0x8c601076 */
L8c600fcc: *stop=0x8c600fccu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c600fce; /* mov.l @r14,r3 */
L8c600fce: *stop=0x8c600fceu;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c600fd0; /* mov.l @(0x1c,r3),r2 */
L8c600fd0: *stop=0x8c600fd0u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c600fd2; /* tst r2,r2 */
L8c600fd2: if(!(*sr&1u))goto L8c601074;goto L8c600fd4; /* bf 0x8c601074 */
L8c600fd4: *stop=0x8c600fd4u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c600fd6; /* mov.l @(0x14,r14),r12 */
L8c600fd6: *stop=0x8c600fd6u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c600fd8; /* mov.l @(0x8,r14),r4 */
L8c600fd8: *stop=0x8c600fd8u;if(!valid_access(r[14]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c600fda; /* mov.l @(0x10,r14),r3 */
L8c600fda: *stop=0x8c600fdau;r[12]+=r[4];goto L8c600fdc; /* add r4,r12 */
L8c600fdc: *stop=0x8c600fdcu;r[4]+=r[3];goto L8c600fde; /* add r3,r4 */
L8c600fde: *stop=0x8c600fdeu;*sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[4]);goto L8c600fe0; /* cmp/ge r4,r12 */
L8c600fe0: take=(*sr&1u);*stop=0x8c600fe2u;r[11]=1u;if(take)goto L8c600ff4;goto L8c600fe4; /* bt/s 0x8c600ff4 */
L8c600fe4: *stop=0x8c600fe4u;r[3]=r[13];goto L8c600fe6; /* mov r13,r3 */
L8c600fe6: *stop=0x8c600fe6u;r[3]+=r[12];goto L8c600fe8; /* add r12,r3 */
L8c600fe8: *stop=0x8c600fe8u;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[4]);goto L8c600fea; /* cmp/gt r4,r3 */
L8c600fea: if(!(*sr&1u))goto L8c600ff0;goto L8c600fec; /* bf 0x8c600ff0 */
L8c600fec: *stop=0x8c600fecu;r[13]=r[4];goto L8c600fee; /* mov r4,r13 */
L8c600fee: *stop=0x8c600feeu;r[13]-=r[12];goto L8c600ff0; /* sub r12,r13 */
L8c600ff0: *stop=0x8c600ff0u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c600ff2; /* tst r13,r13 */
L8c600ff2: if(!(*sr&1u))goto L8c601004;goto L8c600ff4; /* bf 0x8c601004 */
L8c600ff4: *stop=0x8c600ff4u;r[2]=0u;goto L8c600ff6; /* mov #0x0,r2 */
L8c600ff6: *stop=0x8c600ff6u;r[0]=80u;goto L8c600ff8; /* mov #0x50,r0 */
L8c600ff8: *stop=0x8c600ff8u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[2],4);if(io_status)return io_status;goto L8c600ffa; /* mov.l r2,@(0x18,r14) */
L8c600ffa: *stop=0x8c600ffcu;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[11],2);if(io_status)return io_status;goto L8c601074; /* bra 0x8c601074 */
L8c601004: *stop=0x8c601004u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c601006; /* stc SR,r0 */
L8c601006: *stop=0x8c601006u;r[3]=sx(m,bus,&io_status,0x8c6010eau,2);if(io_status)return io_status;goto L8c601008; /* mov.w 0x8c6010ea,r3 */
L8c601008: *stop=0x8c601008u;r[0]>>=2;goto L8c60100a; /* shlr2 r0 */
L8c60100a: *stop=0x8c60100au;r[0]>>=2;goto L8c60100c; /* shlr2 r0 */
L8c60100c: *stop=0x8c60100cu;r[0]&=0xfu;goto L8c60100e; /* and #0xf,r0 */
L8c60100e: *stop=0x8c60100eu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c601010; /* mov.l r0,@r15 */
L8c601010: *stop=0x8c601010u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c601012; /* stc SR,r0 */
L8c601012: *stop=0x8c601012u;r[0]&=r[3];goto L8c601014; /* and r3,r0 */
L8c601014: *stop=0x8c601014u;r[0]|=0xf0u;goto L8c601016; /* or #0xf0,r0 */
L8c601016: *stop=0x8c601016u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c601018; /* ldc r0,SR */
L8c601018: *stop=0x8c601018u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60101a; /* mov.l @r14,r2 */
L8c60101a: *stop=0x8c60101au;r[5]=r[13];goto L8c60101c; /* mov r13,r5 */
L8c60101c: *stop=0x8c60101cu;r[7]=0u;goto L8c60101e; /* mov #0x0,r7 */
L8c60101e: *stop=0x8c60101eu;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601020; /* mov.l @(0xc,r2),r3 */
L8c601020: *stop=0x8c601020u;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c601022; /* mov.l @(0x4,r3),r1 */
L8c601022: target=r[1];*pr=0x8c601026u;*stop=0x8c601024u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601026; /* jsr @r1 */
L8c601026: *stop=0x8c601026u;r[12]=r[0];goto L8c601028; /* mov r0,r12 */
L8c601028: *stop=0x8c601028u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c60102a; /* tst r12,r12 */
L8c60102a: if((*sr&1u))goto L8c601056;goto L8c60102c; /* bt 0x8c601056 */
L8c60102c: *stop=0x8c60102cu;r[2]=2u;goto L8c60102e; /* mov #0x2,r2 */
L8c60102e: *stop=0x8c60102eu;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[12],4);if(io_status)return io_status;goto L8c601030; /* mov.l r12,@(0x3c,r14) */
L8c601030: *stop=0x8c601030u;r[0]=80u;goto L8c601032; /* mov #0x50,r0 */
L8c601032: *stop=0x8c601032u;if(!valid_access(r[14]+28u,4u))return -3;wr(m,bus,&io_status,r[14]+0x1cu,r[13],4);if(io_status)return io_status;goto L8c601034; /* mov.l r13,@(0x1c,r14) */
L8c601034: *stop=0x8c601034u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[2],2);if(io_status)return io_status;goto L8c601036; /* mov.w r2,@(r0,r14) */
L8c601036: *stop=0x8c601036u;r[0]=74u;goto L8c601038; /* mov #0x4a,r0 */
L8c601038: *stop=0x8c601038u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[11],2);if(io_status)return io_status;goto L8c60103a; /* mov.w r11,@(r0,r14) */
L8c60103a: *stop=0x8c60103au;r[3]=0u;goto L8c60103c; /* mov #0x0,r3 */
L8c60103c: *stop=0x8c60103cu;r[0]=72u;goto L8c60103e; /* mov #0x48,r0 */
L8c60103e: *stop=0x8c60103eu;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[11],2);if(io_status)return io_status;goto L8c601040; /* mov.w r11,@(r0,r14) */
L8c601040: *stop=0x8c601040u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[3],4);if(io_status)return io_status;goto L8c601042; /* mov.l r3,@(0x18,r14) */
L8c601042: *stop=0x8c601042u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c601044; /* mov.l @r14,r2 */
L8c601044: *stop=0x8c601044u;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[14],4);if(io_status)return io_status;goto L8c601046; /* mov.l r14,@(0x1c,r2) */
L8c601046: *stop=0x8c601046u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[3]=loaded;}goto L8c601048; /* mov.l @r14,r3 */
L8c601048: *stop=0x8c601048u;r[0]=64u;goto L8c60104a; /* mov #0x40,r0 */
L8c60104a: *stop=0x8c60104au;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c60104c; /* mov.l @(0xc,r3),r2 */
L8c60104c: *stop=0x8c60104cu;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c60104e; /* mov.l @(r0,r2),r1 */
L8c60104e: target=r[1];*pr=0x8c601052u;*stop=0x8c601050u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601052; /* jsr @r1 */
L8c601052: *stop=0x8c601054u;goto L8c60105a; /* bra 0x8c60105a */
L8c601056: *stop=0x8c601056u;r[0]=80u;goto L8c601058; /* mov #0x50,r0 */
L8c601058: *stop=0x8c601058u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[11],2);if(io_status)return io_status;goto L8c60105a; /* mov.w r11,@(r0,r14) */
L8c60105a: *stop=0x8c60105au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c60105c; /* mov.l @r15,r0 */
L8c60105c: *stop=0x8c60105cu;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c60105e; /* stc SR,r3 */
L8c60105e: *stop=0x8c60105eu;r[2]=sx(m,bus,&io_status,0x8c6010eau,2);if(io_status)return io_status;goto L8c601060; /* mov.w 0x8c6010ea,r2 */
L8c601060: *stop=0x8c601060u;r[0]&=0xfu;goto L8c601062; /* and #0xf,r0 */
L8c601062: *stop=0x8c601062u;r[0]<<=2;goto L8c601064; /* shll2 r0 */
L8c601064: *stop=0x8c601064u;r[3]&=r[2];goto L8c601066; /* and r2,r3 */
L8c601066: *stop=0x8c601066u;r[0]<<=2;goto L8c601068; /* shll2 r0 */
L8c601068: *stop=0x8c601068u;r[0]|=r[3];goto L8c60106a; /* or r3,r0 */
L8c60106a: *stop=0x8c60106au;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c60106c; /* ldc r0,SR */
L8c60106c: *stop=0x8c60106cu;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c60106e; /* tst r12,r12 */
L8c60106e: if((*sr&1u))goto L8c601074;goto L8c601070; /* bt 0x8c601074 */
L8c601070: *stop=0x8c601072u;r[0]=r[13];goto L8c601076; /* bra 0x8c601076 */
L8c601074: *stop=0x8c601074u;r[0]=0u;goto L8c601076; /* mov #0x0,r0 */
L8c601076: *stop=0x8c601076u;r[15]+=4u;goto L8c601078; /* add #0x4,r15 */
L8c601078: *stop=0x8c601078u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c60107a; /* lds.l @r15+,PR */
L8c60107a: *stop=0x8c60107au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c60107c; /* mov.l @r15+,r11 */
L8c60107c: *stop=0x8c60107cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c60107e; /* mov.l @r15+,r12 */
L8c60107e: *stop=0x8c60107eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c601080; /* mov.l @r15+,r13 */
L8c601080: *stop=0x8c601080u;if(*pr!=entry_pr)return -3;*stop=0x8c601082u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c600cf0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c600cf0;
L8c600cf0: *stop=0x8c600cf0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c600cf2; /* mov.l r14,@-r15 */
L8c600cf2: *stop=0x8c600cf2u;r[14]=r[4];goto L8c600cf4; /* mov r4,r14 */
L8c600cf4: *stop=0x8c600cf4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c600cf6; /* mov.l r13,@-r15 */
L8c600cf6: *stop=0x8c600cf6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c600cf8; /* mov.l r12,@-r15 */
L8c600cf8: *stop=0x8c600cf8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c600cfa; /* mov.l r11,@-r15 */
L8c600cfa: *stop=0x8c600cfau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c600cfc; /* mov.l r10,@-r15 */
L8c600cfc: *stop=0x8c600cfcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c600cfe; /* mov.l r9,@-r15 */
L8c600cfe: *stop=0x8c600cfeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c600d00; /* sts.l PR,@-r15 */
L8c600d00: *stop=0x8c600d00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c600de4u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c600d02; /* mov.l 0x8c600de4,r13 */
L8c600d02: target=r[13];*pr=0x8c600d06u;*stop=0x8c600d04u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[11]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600d06; /* jsr @r13 */
L8c600d06: *stop=0x8c600d06u;{uint32_t loaded=rd(m,bus,&io_status,0x8c600decu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c600d08; /* mov.l 0x8c600dec,r10 */
L8c600d08: *stop=0x8c600d08u;r[12]=r[0];goto L8c600d0a; /* mov r0,r12 */
L8c600d0a: *stop=0x8c600d0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c600de8u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c600d0c; /* mov.l 0x8c600de8,r9 */
L8c600d0c: *stop=0x8c600d0cu;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c600d0e; /* mov.l @r14,r2 */
L8c600d0e: *stop=0x8c600d0eu;r[0]=72u;goto L8c600d10; /* mov #0x48,r0 */
L8c600d10: *stop=0x8c600d10u;if(!valid_access(r[2]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c600d12; /* mov.l @(0x3c,r2),r3 */
L8c600d12: *stop=0x8c600d12u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c600d14; /* mov.l @(r0,r3),r1 */
L8c600d14: target=r[1];*pr=0x8c600d18u;*stop=0x8c600d16u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600d18; /* jsr @r1 */
L8c600d18: target=0x8c6013a4u;*pr=0x8c600d1cu;*stop=0x8c600d1au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600d1c; /* bsr 0x8c6013a4 */
L8c600d1c: *stop=0x8c600d1cu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c600d1e; /* cmp/eq #0x0,r0 */
L8c600d1e: take=(*sr&1u);*stop=0x8c600d20u;r[4]=r[0];if(take)goto L8c600d3e;goto L8c600d22; /* bt/s 0x8c600d3e */
L8c600d22: *stop=0x8c600d22u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c600d24; /* cmp/eq #0x1,r0 */
L8c600d24: if((*sr&1u))goto L8c600d42;goto L8c600d26; /* bt 0x8c600d42 */
L8c600d26: *stop=0x8c600d26u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c600d28; /* cmp/eq #0x2,r0 */
L8c600d28: if((*sr&1u))goto L8c600d46;goto L8c600d2a; /* bt 0x8c600d46 */
L8c600d2a: *stop=0x8c600d2au;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c600d2c; /* cmp/eq #0x3,r0 */
L8c600d2c: if((*sr&1u))goto L8c600d46;goto L8c600d2e; /* bt 0x8c600d46 */
L8c600d2e: *stop=0x8c600d2eu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c600d30; /* cmp/eq #0x4,r0 */
L8c600d30: if((*sr&1u))goto L8c600d46;goto L8c600d32; /* bt 0x8c600d46 */
L8c600d32: *stop=0x8c600d32u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c600d34; /* cmp/eq #0x5,r0 */
L8c600d34: if((*sr&1u))goto L8c600d56;goto L8c600d36; /* bt 0x8c600d56 */
L8c600d36: *stop=0x8c600d36u;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c600d38; /* cmp/eq #0x40,r0 */
L8c600d38: if((*sr&1u))goto L8c600d56;goto L8c600d3a; /* bt 0x8c600d56 */
L8c600d3a: *stop=0x8c600d3cu;goto L8c600d5c; /* bra 0x8c600d5c */
L8c600d3e: *stop=0x8c600d40u;r[0]=4294967264u;goto L8c600d72; /* bra 0x8c600d72 */
L8c600d42: *stop=0x8c600d44u;r[0]=0u;goto L8c600d72; /* bra 0x8c600d72 */
L8c600d46: *stop=0x8c600d46u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c600d48; /* mov.l @(0x18,r14),r2 */
L8c600d48: *stop=0x8c600d48u;*sr=(*sr&~1u)|(r[2]==r[11]);goto L8c600d4a; /* cmp/eq r2,r11 */
L8c600d4a: if((*sr&1u))goto L8c600d60;goto L8c600d4c; /* bt 0x8c600d60 */
L8c600d4c: target=r[13];*pr=0x8c600d50u;*stop=0x8c600d4eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600d50; /* jsr @r13 */
L8c600d50: *stop=0x8c600d50u;r[12]=r[0];goto L8c600d52; /* mov r0,r12 */
L8c600d52: *stop=0x8c600d54u;if(!valid_access(r[14]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x18u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c600d60; /* bra 0x8c600d60 */
L8c600d56: *stop=0x8c600d56u;r[0]=82u;goto L8c600d58; /* mov #0x52,r0 */
L8c600d58: *stop=0x8c600d5au;if(!valid_access(r[0]+r[14],2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c600d72; /* bra 0x8c600d72 */
L8c600d5c: *stop=0x8c600d5eu;r[0]=4294967271u;goto L8c600d72; /* bra 0x8c600d72 */
L8c600d60: target=r[13];*pr=0x8c600d64u;*stop=0x8c600d62u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600d64; /* jsr @r13 */
L8c600d64: *stop=0x8c600d64u;r[5]=r[0];goto L8c600d66; /* mov r0,r5 */
L8c600d66: target=r[9];*pr=0x8c600d6au;*stop=0x8c600d68u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c600d6a; /* jsr @r9 */
L8c600d6a: *stop=0x8c600d6au;r[4]=r[0];goto L8c600d6c; /* mov r0,r4 */
L8c600d6c: *stop=0x8c600d6cu;*sr=(*sr&~1u)|(r[4]>r[10]);goto L8c600d6e; /* cmp/hi r10,r4 */
L8c600d6e: if(!(*sr&1u))goto L8c600d0c;goto L8c600d70; /* bf 0x8c600d0c */
L8c600d70: *stop=0x8c600d70u;r[0]=4294967275u;goto L8c600d72; /* mov #-0x15,r0 */
L8c600d72: *stop=0x8c600d72u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c600d74; /* lds.l @r15+,PR */
L8c600d74: *stop=0x8c600d74u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c600d76; /* mov.l @r15+,r9 */
L8c600d76: *stop=0x8c600d76u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c600d78; /* mov.l @r15+,r10 */
L8c600d78: *stop=0x8c600d78u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c600d7a; /* mov.l @r15+,r11 */
L8c600d7a: *stop=0x8c600d7au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c600d7c; /* mov.l @r15+,r12 */
L8c600d7c: *stop=0x8c600d7cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c600d7e; /* mov.l @r15+,r13 */
L8c600d7e: *stop=0x8c600d7eu;if(*pr!=entry_pr)return -3;*stop=0x8c600d80u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6013d2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6013d2;
L8c6013d2: *stop=0x8c6013d2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6013d4; /* mov.l r14,@-r15 */
L8c6013d4: *stop=0x8c6013d4u;r[14]=r[4];goto L8c6013d6; /* mov r4,r14 */
L8c6013d6: *stop=0x8c6013d6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6013d8; /* mov.l r13,@-r15 */
L8c6013d8: *stop=0x8c6013d8u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c6013da; /* tst r14,r14 */
L8c6013da: *stop=0x8c6013dau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6013dc; /* mov.l r12,@-r15 */
L8c6013dc: *stop=0x8c6013dcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6013de; /* mov.l r11,@-r15 */
L8c6013de: *stop=0x8c6013deu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6013e0; /* mov.l r10,@-r15 */
L8c6013e0: *stop=0x8c6013e0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6013e2; /* mov.l r9,@-r15 */
L8c6013e2: *stop=0x8c6013e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6013e4; /* sts.l PR,@-r15 */
L8c6013e4: take=(*sr&1u);*stop=0x8c6013e6u;r[13]=0u;if(take)goto L8c6013f0;goto L8c6013e8; /* bt/s 0x8c6013f0 */
L8c6013e8: *stop=0x8c6013e8u;r[0]=76u;goto L8c6013ea; /* mov #0x4c,r0 */
L8c6013ea: *stop=0x8c6013eau;if(!valid_access(r[0]+r[14],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c6013ec; /* mov.w @(r0,r14),r3 */
L8c6013ec: *stop=0x8c6013ecu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6013ee; /* tst r3,r3 */
L8c6013ee: if(!(*sr&1u))goto L8c6013f4;goto L8c6013f0; /* bf 0x8c6013f4 */
L8c6013f0: *stop=0x8c6013f2u;r[0]=4294967286u;goto L8c6014f0; /* bra 0x8c6014f0 */
L8c6013f4: *stop=0x8c6013f4u;r[0]=80u;goto L8c6013f6; /* mov #0x50,r0 */
L8c6013f6: *stop=0x8c6013f6u;if(!valid_access(r[0]+r[14],2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c6013f8; /* mov.w @(r0,r14),r0 */
L8c6013f8: *stop=0x8c6013f8u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c6013fa; /* cmp/eq #0x2,r0 */
L8c6013fa: take=(*sr&1u);*stop=0x8c6013fcu;r[4]=r[0];if(take)goto L8c601404;goto L8c6013fe; /* bt/s 0x8c601404 */
L8c6013fe: *stop=0x8c6013feu;r[0]=r[4];goto L8c601400; /* mov r4,r0 */
L8c601400: *stop=0x8c601400u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c601402; /* cmp/eq #0x4,r0 */
L8c601402: if(!(*sr&1u))goto L8c60142c;goto L8c601404; /* bf 0x8c60142c */
L8c601404: *stop=0x8c601404u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c601406; /* mov.l @r14,r2 */
L8c601406: *stop=0x8c601406u;if(!valid_access(r[2]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601408; /* mov.l @(0x1c,r2),r3 */
L8c601408: *stop=0x8c601408u;*sr=(*sr&~1u)|(r[14]==r[3]);goto L8c60140a; /* cmp/eq r14,r3 */
L8c60140a: if((*sr&1u))goto L8c601412;goto L8c60140c; /* bt 0x8c601412 */
L8c60140c: *stop=0x8c60140eu;r[0]=4294967283u;goto L8c6014f0; /* bra 0x8c6014f0 */
L8c601412: *stop=0x8c601412u;{uint32_t loaded=rd(m,bus,&io_status,0x8c60151cu,4);if(io_status)return io_status;r[12]=loaded;}goto L8c601414; /* mov.l 0x8c60151c,r12 */
L8c601414: target=r[12];*pr=0x8c601418u;*stop=0x8c601416u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601418; /* jsr @r12 */
L8c601418: *stop=0x8c60141au;r[10]=r[0];goto L8c601456; /* bra 0x8c601456 */
L8c60141c: *stop=0x8c60141cu;r[0]=80u;goto L8c60141e; /* mov #0x50,r0 */
L8c60141e: *stop=0x8c60141eu;if(!valid_access(r[0]+r[14],2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c601420; /* mov.w @(r0,r14),r0 */
L8c601420: *stop=0x8c601420u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c601422; /* cmp/eq #0x2,r0 */
L8c601422: take=(*sr&1u);*stop=0x8c601424u;r[4]=r[0];if(take)goto L8c601434;goto L8c601426; /* bt/s 0x8c601434 */
L8c601426: *stop=0x8c601426u;r[0]=r[4];goto L8c601428; /* mov r4,r0 */
L8c601428: *stop=0x8c601428u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c60142a; /* cmp/eq #0x4,r0 */
L8c60142a: if((*sr&1u))goto L8c601434;goto L8c60142c; /* bt 0x8c601434 */
L8c60142c: *stop=0x8c60142cu;r[0]=80u;goto L8c60142e; /* mov #0x50,r0 */
L8c60142e: *stop=0x8c60142eu;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[13],2);if(io_status)return io_status;goto L8c601430; /* mov.w r13,@(r0,r14) */
L8c601430: *stop=0x8c601432u;r[0]=0u;goto L8c6014f0; /* bra 0x8c6014f0 */
L8c601434: target=r[12];*pr=0x8c601438u;*stop=0x8c601436u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601438; /* jsr @r12 */
L8c601438: *stop=0x8c601438u;r[5]=r[0];goto L8c60143a; /* mov r0,r5 */
L8c60143a: target=r[11];*pr=0x8c60143eu;*stop=0x8c60143cu;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60143e; /* jsr @r11 */
L8c60143e: *stop=0x8c60143eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601520u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c601440; /* mov.l 0x8c601520,r5 */
L8c601440: *stop=0x8c601440u;r[4]=r[0];goto L8c601442; /* mov r0,r4 */
L8c601442: *stop=0x8c601442u;*sr=(*sr&~1u)|(r[4]>r[5]);goto L8c601444; /* cmp/hi r5,r4 */
L8c601444: if(!(*sr&1u))goto L8c60144a;goto L8c601446; /* bf 0x8c60144a */
L8c601446: *stop=0x8c601448u;r[0]=4294967275u;goto L8c6014f0; /* bra 0x8c6014f0 */
L8c60144a: *stop=0x8c60144au;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c60144c; /* mov.l @r14,r2 */
L8c60144c: *stop=0x8c60144cu;r[0]=72u;goto L8c60144e; /* mov #0x48,r0 */
L8c60144e: *stop=0x8c60144eu;if(!valid_access(r[2]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601450; /* mov.l @(0x3c,r2),r3 */
L8c601450: *stop=0x8c601450u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c601452; /* mov.l @(r0,r3),r1 */
L8c601452: target=r[1];*pr=0x8c601456u;*stop=0x8c601454u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601456; /* jsr @r1 */
L8c601456: *stop=0x8c601456u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601524u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c601458; /* mov.l 0x8c601524,r11 */
L8c601458: *stop=0x8c601458u;r[0]=64u;goto L8c60145a; /* mov #0x40,r0 */
L8c60145a: *stop=0x8c60145au;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c60145c; /* mov.l @(r0,r14),r0 */
L8c60145c: *stop=0x8c60145cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c60145e; /* cmp/eq #0x1,r0 */
L8c60145e: if(!(*sr&1u))goto L8c60141c;goto L8c601460; /* bf 0x8c60141c */
L8c601460: target=r[12];*pr=0x8c601464u;*stop=0x8c601462u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601464; /* jsr @r12 */
L8c601464: *stop=0x8c601464u;r[10]=sx(m,bus,&io_status,0x8c601518u,2);if(io_status)return io_status;goto L8c601466; /* mov.w 0x8c601518,r10 */
L8c601466: *stop=0x8c601466u;r[9]=r[0];goto L8c601468; /* mov r0,r9 */
L8c601468: target=r[12];*pr=0x8c60146cu;*stop=0x8c60146au;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c60146c; /* jsr @r12 */
L8c60146c: *stop=0x8c60146cu;r[5]=r[0];goto L8c60146e; /* mov r0,r5 */
L8c60146e: target=r[11];*pr=0x8c601472u;*stop=0x8c601470u;r[4]=r[9];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601472; /* jsr @r11 */
L8c601472: *stop=0x8c601472u;r[4]=r[0];goto L8c601474; /* mov r0,r4 */
L8c601474: *stop=0x8c601474u;*sr=(*sr&~1u)|(r[4]>=r[10]);goto L8c601476; /* cmp/hs r10,r4 */
L8c601476: if(!(*sr&1u))goto L8c601468;goto L8c601478; /* bf 0x8c601468 */
L8c601478: *stop=0x8c601478u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c60147a; /* stc SR,r0 */
L8c60147a: *stop=0x8c60147au;r[3]=sx(m,bus,&io_status,0x8c60151au,2);if(io_status)return io_status;goto L8c60147c; /* mov.w 0x8c60151a,r3 */
L8c60147c: *stop=0x8c60147cu;r[0]>>=2;goto L8c60147e; /* shlr2 r0 */
L8c60147e: *stop=0x8c60147eu;r[0]>>=2;goto L8c601480; /* shlr2 r0 */
L8c601480: *stop=0x8c601480u;r[0]&=0xfu;goto L8c601482; /* and #0xf,r0 */
L8c601482: *stop=0x8c601482u;r[12]=r[0];goto L8c601484; /* mov r0,r12 */
L8c601484: *stop=0x8c601484u;status=sa_sh4_stc_sr(*sr,&r[0],0u);if(status)return status;goto L8c601486; /* stc SR,r0 */
L8c601486: *stop=0x8c601486u;r[0]&=r[3];goto L8c601488; /* and r3,r0 */
L8c601488: *stop=0x8c601488u;r[0]|=0xf0u;goto L8c60148a; /* or #0xf0,r0 */
L8c60148a: *stop=0x8c60148au;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c60148c; /* ldc r0,SR */
L8c60148c: *stop=0x8c60148cu;r[0]=80u;goto L8c60148e; /* mov #0x50,r0 */
L8c60148e: *stop=0x8c60148eu;if(!valid_access(r[0]+r[14],2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c601490; /* mov.w @(r0,r14),r0 */
L8c601490: *stop=0x8c601490u;r[5]=4294967283u;goto L8c601492; /* mov #-0xd,r5 */
L8c601492: *stop=0x8c601492u;r[11]=3u;goto L8c601494; /* mov #0x3,r11 */
L8c601494: *stop=0x8c601494u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c601496; /* cmp/eq #0x2,r0 */
L8c601496: take=(*sr&1u);*stop=0x8c601498u;r[4]=r[0];if(take)goto L8c6014a8;goto L8c60149a; /* bt/s 0x8c6014a8 */
L8c60149a: *stop=0x8c60149au;r[0]=r[4];goto L8c60149c; /* mov r4,r0 */
L8c60149c: *stop=0x8c60149cu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c60149e; /* cmp/eq #0x4,r0 */
L8c60149e: if((*sr&1u))goto L8c6014a8;goto L8c6014a0; /* bt 0x8c6014a8 */
L8c6014a0: *stop=0x8c6014a0u;r[0]=80u;goto L8c6014a2; /* mov #0x50,r0 */
L8c6014a2: *stop=0x8c6014a2u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[13],2);if(io_status)return io_status;goto L8c6014a4; /* mov.w r13,@(r0,r14) */
L8c6014a4: *stop=0x8c6014a6u;r[4]=r[13];goto L8c6014dc; /* bra 0x8c6014dc */
L8c6014a8: *stop=0x8c6014a8u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6014aa; /* mov.l @r14,r2 */
L8c6014aa: *stop=0x8c6014aau;if(!valid_access(r[2]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6014ac; /* mov.l @(0x1c,r2),r3 */
L8c6014ac: *stop=0x8c6014acu;*sr=(*sr&~1u)|(r[14]==r[3]);goto L8c6014ae; /* cmp/eq r14,r3 */
L8c6014ae: if((*sr&1u))goto L8c6014b4;goto L8c6014b0; /* bt 0x8c6014b4 */
L8c6014b0: *stop=0x8c6014b2u;r[4]=r[5];goto L8c6014dc; /* bra 0x8c6014dc */
L8c6014b4: *stop=0x8c6014b4u;if(!valid_access(r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6014b6; /* mov.l @r14,r2 */
L8c6014b6: *stop=0x8c6014b6u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6014b8; /* mov.l @(0xc,r2),r3 */
L8c6014b8: *stop=0x8c6014b8u;if(!valid_access(r[3]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6014ba; /* mov.l @(0x14,r3),r1 */
L8c6014ba: target=r[1];*pr=0x8c6014beu;*stop=0x8c6014bcu;if(!valid_access(r[14]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x3cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6014be; /* jsr @r1 */
L8c6014be: *stop=0x8c6014beu;r[0]=72u;goto L8c6014c0; /* mov #0x48,r0 */
L8c6014c0: *stop=0x8c6014c0u;r[2]=sx(m,bus,&io_status,0x8c60151au,2);if(io_status)return io_status;goto L8c6014c2; /* mov.w 0x8c60151a,r2 */
L8c6014c2: *stop=0x8c6014c2u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[11],2);if(io_status)return io_status;goto L8c6014c4; /* mov.w r11,@(r0,r14) */
L8c6014c4: *stop=0x8c6014c4u;r[0]=r[12];goto L8c6014c6; /* mov r12,r0 */
L8c6014c6: *stop=0x8c6014c6u;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c6014c8; /* stc SR,r3 */
L8c6014c8: *stop=0x8c6014c8u;r[0]&=0xfu;goto L8c6014ca; /* and #0xf,r0 */
L8c6014ca: *stop=0x8c6014cau;r[0]<<=2;goto L8c6014cc; /* shll2 r0 */
L8c6014cc: *stop=0x8c6014ccu;r[3]&=r[2];goto L8c6014ce; /* and r2,r3 */
L8c6014ce: *stop=0x8c6014ceu;r[0]<<=2;goto L8c6014d0; /* shll2 r0 */
L8c6014d0: *stop=0x8c6014d0u;r[0]|=r[3];goto L8c6014d2; /* or r3,r0 */
L8c6014d2: *stop=0x8c6014d2u;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c6014d4; /* ldc r0,SR */
L8c6014d4: target=0x8c600cf0u;*pr=0x8c6014d8u;*stop=0x8c6014d6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6014d8; /* bsr 0x8c600cf0 */
L8c6014d8: *stop=0x8c6014dau;r[4]=r[0];goto L8c6014ee; /* bra 0x8c6014ee */
L8c6014dc: *stop=0x8c6014dcu;status=sa_sh4_stc_sr(*sr,&r[3],0u);if(status)return status;goto L8c6014de; /* stc SR,r3 */
L8c6014de: *stop=0x8c6014deu;r[0]=r[12];goto L8c6014e0; /* mov r12,r0 */
L8c6014e0: *stop=0x8c6014e0u;r[2]=sx(m,bus,&io_status,0x8c60151au,2);if(io_status)return io_status;goto L8c6014e2; /* mov.w 0x8c60151a,r2 */
L8c6014e2: *stop=0x8c6014e2u;r[0]&=0xfu;goto L8c6014e4; /* and #0xf,r0 */
L8c6014e4: *stop=0x8c6014e4u;r[0]<<=2;goto L8c6014e6; /* shll2 r0 */
L8c6014e6: *stop=0x8c6014e6u;r[3]&=r[2];goto L8c6014e8; /* and r2,r3 */
L8c6014e8: *stop=0x8c6014e8u;r[0]<<=2;goto L8c6014ea; /* shll2 r0 */
L8c6014ea: *stop=0x8c6014eau;r[0]|=r[3];goto L8c6014ec; /* or r3,r0 */
L8c6014ec: *stop=0x8c6014ecu;status=sa_sh4_ldc_sr(r[0],sr,r,integer,0u);if(status)return status;goto L8c6014ee; /* ldc r0,SR */
L8c6014ee: *stop=0x8c6014eeu;r[0]=r[4];goto L8c6014f0; /* mov r4,r0 */
L8c6014f0: *stop=0x8c6014f0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6014f2; /* lds.l @r15+,PR */
L8c6014f2: *stop=0x8c6014f2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6014f4; /* mov.l @r15+,r9 */
L8c6014f4: *stop=0x8c6014f4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6014f6; /* mov.l @r15+,r10 */
L8c6014f6: *stop=0x8c6014f6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6014f8; /* mov.l @r15+,r11 */
L8c6014f8: *stop=0x8c6014f8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6014fa; /* mov.l @r15+,r12 */
L8c6014fa: *stop=0x8c6014fau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6014fc; /* mov.l @r15+,r13 */
L8c6014fc: *stop=0x8c6014fcu;if(*pr!=entry_pr)return -3;*stop=0x8c6014feu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c644fe8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c644fe8;
L8c644fe8: *stop=0x8c644fe8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c644fea; /* sts.l PR,@-r15 */
L8c644fea: *stop=0x8c644feau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6451d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c644fec; /* mov.l 0x8c6451d8,r3 */
L8c644fec: target=r[3];*pr=0x8c644ff0u;*stop=0x8c644feeu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c644ff0; /* jsr @r3 */
L8c644ff0: *stop=0x8c644ff0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c644ff2; /* lds.l @r15+,PR */
L8c644ff2: *stop=0x8c644ff2u;if(*pr!=entry_pr)return -3;*stop=0x8c644ff4u;r[0]=1u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c644ff6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c644ff6;
L8c644ff6: *stop=0x8c644ff6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c644ff8; /* mov.l r14,@-r15 */
L8c644ff8: *stop=0x8c644ff8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c644ffa; /* mov.l r13,@-r15 */
L8c644ffa: *stop=0x8c644ffau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c644ffc; /* sts.l PR,@-r15 */
L8c644ffc: *stop=0x8c644ffcu;r[15]+=4294967276u;goto L8c644ffe; /* add #-0x14,r15 */
L8c644ffe: *stop=0x8c644ffeu;r[14]=r[15];goto L8c645000; /* mov r15,r14 */
L8c645000: *stop=0x8c645000u;r[14]+=4u;goto L8c645002; /* add #0x4,r14 */
L8c645002: *stop=0x8c645002u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c645004; /* mov.l r5,@r15 */
L8c645004: *stop=0x8c645004u;r[13]=r[6];goto L8c645006; /* mov r6,r13 */
L8c645006: *stop=0x8c645006u;r[2]=0u;goto L8c645008; /* mov #0x0,r2 */
L8c645008: *stop=0x8c645008u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6451dcu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64500a; /* mov.l 0x8c6451dc,r6 */
L8c64500a: *stop=0x8c64500au;r[5]=r[14];goto L8c64500c; /* mov r14,r5 */
L8c64500c: *stop=0x8c64500cu;r[0]=r[7];goto L8c64500e; /* mov r7,r0 */
L8c64500e: *stop=0x8c64500eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c645010; /* cmp/eq #0x1,r0 */
L8c645010: *stop=0x8c645010u;r[5]+=8u;goto L8c645012; /* add #0x8,r5 */
L8c645012: *stop=0x8c645012u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c645014; /* mov.l r4,@r14 */
L8c645014: *stop=0x8c645014u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c645016; /* mov.l @r15,r3 */
L8c645016: *stop=0x8c645016u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[3],4);if(io_status)return io_status;goto L8c645018; /* mov.l r3,@(0x4,r14) */
L8c645018: *stop=0x8c645018u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[2],4);if(io_status)return io_status;goto L8c64501a; /* mov.l r2,@(0xc,r14) */
L8c64501a: take=!(*sr&1u);*stop=0x8c64501cu;r[6]&=r[13];if(take)goto L8c645022;goto L8c64501e; /* bf/s 0x8c645022 */
L8c64501e: *stop=0x8c645020u;r[4]=17u;goto L8c64502c; /* bra 0x8c64502c */
L8c645022: *stop=0x8c645022u;*sr=(*sr&~1u)|!(r[7]&r[7]);goto L8c645024; /* tst r7,r7 */
L8c645024: if(!(*sr&1u))goto L8c645030;goto L8c645026; /* bf 0x8c645030 */
L8c645026: *stop=0x8c645026u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6451e0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645028; /* mov.l 0x8c6451e0,r2 */
L8c645028: *stop=0x8c645028u;r[4]=16u;goto L8c64502a; /* mov #0x10,r4 */
L8c64502a: *stop=0x8c64502au;r[6]|=r[2];goto L8c64502c; /* or r2,r6 */
L8c64502c: *stop=0x8c64502eu;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[6],4);if(io_status)return io_status;goto L8c645034; /* bra 0x8c645034 */
L8c645030: *stop=0x8c645032u;r[0]=4294967278u;goto L8c64506c; /* bra 0x8c64506c */
L8c645034: *stop=0x8c645034u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6451e4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c645036; /* mov.l 0x8c6451e4,r2 */
L8c645036: target=r[2];*pr=0x8c64503au;*stop=0x8c645038u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64503a; /* jsr @r2 */
L8c64503a: *stop=0x8c64503au;r[14]=r[0];goto L8c64503c; /* mov r0,r14 */
L8c64503c: *stop=0x8c64503cu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c64503e; /* tst r14,r14 */
L8c64503e: if((*sr&1u))goto L8c64506a;goto L8c645040; /* bt 0x8c64506a */
L8c645040: *stop=0x8c645040u;r[3]=4294967268u;goto L8c645042; /* mov #-0x1c,r3 */
L8c645042: *stop=0x8c645042u;r[0]=r[13];goto L8c645044; /* mov r13,r0 */
L8c645044: *stop=0x8c645044u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c645046; /* shad r3,r0 */
L8c645046: *stop=0x8c645046u;r[0]&=0xeu;goto L8c645048; /* and #0xe,r0 */
L8c645048: *stop=0x8c645048u;*sr=(*sr&~1u)|(r[0]==0x0000000au);goto L8c64504a; /* cmp/eq #0xa,r0 */
L8c64504a: if((*sr&1u))goto L8c64506a;goto L8c64504c; /* bt 0x8c64506a */
L8c64504c: *stop=0x8c64504cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[5]=loaded;}goto L8c64504e; /* mov.l @r15,r5 */
L8c64504e: *stop=0x8c64504eu;r[3]=31u;goto L8c645050; /* mov #0x1f,r3 */
L8c645050: *stop=0x8c645050u;r[5]<<=8;goto L8c645052; /* shll8 r5 */
L8c645052: *stop=0x8c645052u;r[5]<<=2;goto L8c645054; /* shll2 r5 */
L8c645054: *stop=0x8c645054u;*sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c645056; /* shll r5 */
L8c645056: *stop=0x8c645056u;*sr=(*sr&~1u)|!(r[13]&r[3]);goto L8c645058; /* tst r13,r3 */
L8c645058: if(!(*sr&1u))goto L8c645064;goto L8c64505a; /* bf 0x8c645064 */
L8c64505a: *stop=0x8c64505au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6451e8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64505c; /* mov.l 0x8c6451e8,r3 */
L8c64505c: target=r[3];*pr=0x8c645060u;*stop=0x8c64505eu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645060; /* jsr @r3 */
L8c645060: *stop=0x8c645062u;goto L8c64506a; /* bra 0x8c64506a */
L8c645064: *stop=0x8c645064u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6451ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645066; /* mov.l 0x8c6451ec,r3 */
L8c645066: target=r[3];*pr=0x8c64506au;*stop=0x8c645068u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64506a; /* jsr @r3 */
L8c64506a: *stop=0x8c64506au;r[0]=r[14];goto L8c64506c; /* mov r14,r0 */
L8c64506c: *stop=0x8c64506cu;r[15]+=20u;goto L8c64506e; /* add #0x14,r15 */
L8c64506e: *stop=0x8c64506eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c645070; /* lds.l @r15+,PR */
L8c645070: *stop=0x8c645070u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c645072; /* mov.l @r15+,r13 */
L8c645072: *stop=0x8c645072u;if(*pr!=entry_pr)return -3;*stop=0x8c645074u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6451a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6451a4;
L8c6451a4: *stop=0x8c6451a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6451f8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6451a6; /* mov.l 0x8c6451f8,r3 */
L8c6451a6: target=r[3];*stop=0x8c6451a8u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_boot_file_8c6451c8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6451c8;
L8c6451c8: *stop=0x8c6451c8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6451ca; /* sts.l PR,@-r15 */
L8c6451ca: *stop=0x8c6451cau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6451e4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6451cc; /* mov.l 0x8c6451e4,r3 */
L8c6451cc: *stop=0x8c6451ccu;r[5]=0u;goto L8c6451ce; /* mov #0x0,r5 */
L8c6451ce: target=r[3];*pr=0x8c6451d2u;*stop=0x8c6451d0u;r[4]=24u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6451d2; /* jsr @r3 */
L8c6451d2: *stop=0x8c6451d2u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6451d4; /* lds.l @r15+,PR */
L8c6451d4: *stop=0x8c6451d4u;if(*pr!=entry_pr)return -3;*stop=0x8c6451d6u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6451fc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6451fc;
L8c6451fc: *stop=0x8c6451fcu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6451fe; /* sts.l PR,@-r15 */
L8c6451fe: *stop=0x8c6451feu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645330u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c645200; /* mov.l 0x8c645330,r3 */
L8c645200: target=r[3];*pr=0x8c645204u;*stop=0x8c645202u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645204; /* jsr @r3 */
L8c645204: *stop=0x8c645204u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c645206; /* cmp/eq #0x1,r0 */
L8c645206: take=!(*sr&1u);*stop=0x8c645208u;r[4]=r[0];if(take)goto L8c645210;goto L8c64520a; /* bf/s 0x8c645210 */
L8c64520a: *stop=0x8c64520au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64520c; /* lds.l @r15+,PR */
L8c64520c: *stop=0x8c64520cu;if(*pr!=entry_pr)return -3;*stop=0x8c64520eu;r[0]=0u;return 0; /* rts */
L8c645210: *stop=0x8c645210u;r[0]=1u;goto L8c645212; /* mov #0x1,r0 */
L8c645212: *stop=0x8c645212u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c645214; /* lds.l @r15+,PR */
L8c645214: *stop=0x8c645214u;if(*pr!=entry_pr)return -3;*stop=0x8c645216u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6452b4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6452b4;
L8c6452b4: *stop=0x8c6452b4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6452b6; /* mov.l r14,@-r15 */
L8c6452b6: *stop=0x8c6452b6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6452b8; /* sts.l PR,@-r15 */
L8c6452b8: *stop=0x8c6452b8u;r[15]+=4294967280u;goto L8c6452ba; /* add #-0x10,r15 */
L8c6452ba: *stop=0x8c6452bau;r[14]=r[15];goto L8c6452bc; /* mov r15,r14 */
L8c6452bc: *stop=0x8c6452bcu;r[3]=0u;goto L8c6452be; /* mov #0x0,r3 */
L8c6452be: *stop=0x8c6452beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c645338u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6452c0; /* mov.l 0x8c645338,r2 */
L8c6452c0: *stop=0x8c6452c0u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c6452c2; /* mov.l r4,@r14 */
L8c6452c2: *stop=0x8c6452c2u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[5],4);if(io_status)return io_status;goto L8c6452c4; /* mov.l r5,@(0x4,r14) */
L8c6452c4: *stop=0x8c6452c4u;r[5]=r[14];goto L8c6452c6; /* mov r14,r5 */
L8c6452c6: *stop=0x8c6452c6u;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[6],4);if(io_status)return io_status;goto L8c6452c8; /* mov.l r6,@(0x8,r14) */
L8c6452c8: *stop=0x8c6452c8u;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[3],4);if(io_status)return io_status;goto L8c6452ca; /* mov.l r3,@(0xc,r14) */
L8c6452ca: target=r[2];*pr=0x8c6452ceu;*stop=0x8c6452ccu;r[4]=34u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6452ce; /* jsr @r2 */
L8c6452ce: *stop=0x8c6452ceu;r[15]+=16u;goto L8c6452d0; /* add #0x10,r15 */
L8c6452d0: *stop=0x8c6452d0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6452d2; /* lds.l @r15+,PR */
L8c6452d2: *stop=0x8c6452d2u;if(*pr!=entry_pr)return -3;*stop=0x8c6452d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6452d6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6452d6;
L8c6452d6: *stop=0x8c6452d6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6452d8; /* sts.l PR,@-r15 */
L8c6452d8: *stop=0x8c6452d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64533cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6452da; /* mov.l 0x8c64533c,r3 */
L8c6452da: target=r[3];*pr=0x8c6452deu;*stop=0x8c6452dcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6452de; /* jsr @r3 */
L8c6452de: *stop=0x8c6452deu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6452e0; /* lds.l @r15+,PR */
L8c6452e0: *stop=0x8c6452e0u;if(*pr!=entry_pr)return -3;*stop=0x8c6452e2u;r[0]=1u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6452ea(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6452ea;
L8c6452ea: *stop=0x8c6452eau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6452ec; /* mov.l r14,@-r15 */
L8c6452ec: *stop=0x8c6452ecu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6452ee; /* sts.l PR,@-r15 */
L8c6452ee: *stop=0x8c6452eeu;r[15]+=4294967280u;goto L8c6452f0; /* add #-0x10,r15 */
L8c6452f0: *stop=0x8c6452f0u;r[14]=r[15];goto L8c6452f2; /* mov r15,r14 */
L8c6452f2: *stop=0x8c6452f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645338u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6452f4; /* mov.l 0x8c645338,r3 */
L8c6452f4: *stop=0x8c6452f4u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c6452f6; /* mov.l r4,@r14 */
L8c6452f6: *stop=0x8c6452f6u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[5],4);if(io_status)return io_status;goto L8c6452f8; /* mov.l r5,@(0x4,r14) */
L8c6452f8: *stop=0x8c6452f8u;r[5]=r[14];goto L8c6452fa; /* mov r14,r5 */
L8c6452fa: *stop=0x8c6452fau;if(!valid_access(r[14]+8u,4u))return -3;wr(m,bus,&io_status,r[14]+0x8u,r[6],4);if(io_status)return io_status;goto L8c6452fc; /* mov.l r6,@(0x8,r14) */
L8c6452fc: *stop=0x8c6452fcu;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[7],4);if(io_status)return io_status;goto L8c6452fe; /* mov.l r7,@(0xc,r14) */
L8c6452fe: target=r[3];*pr=0x8c645302u;*stop=0x8c645300u;r[4]=36u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645302; /* jsr @r3 */
L8c645302: *stop=0x8c645302u;r[15]+=16u;goto L8c645304; /* add #0x10,r15 */
L8c645304: *stop=0x8c645304u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c645306; /* lds.l @r15+,PR */
L8c645306: *stop=0x8c645306u;if(*pr!=entry_pr)return -3;*stop=0x8c645308u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c645328(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c645328;
L8c645328: *stop=0x8c645328u;{uint32_t loaded=rd(m,bus,&io_status,0x8c645344u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64532a; /* mov.l 0x8c645344,r0 */
L8c64532a: *stop=0x8c64532au;if(*pr!=entry_pr)return -3;*stop=0x8c64532cu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6455ce(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6455ce;
L8c6455ce: *stop=0x8c6455ceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6455d0; /* mov.l r14,@-r15 */
L8c6455d0: *stop=0x8c6455d0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6455d2; /* mov.l r13,@-r15 */
L8c6455d2: *stop=0x8c6455d2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6455d4; /* mov.l r12,@-r15 */
L8c6455d4: *stop=0x8c6455d4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6455d6; /* mov.l r11,@-r15 */
L8c6455d6: *stop=0x8c6455d6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6455d8; /* mov.l r10,@-r15 */
L8c6455d8: *stop=0x8c6455d8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6455da; /* sts.l PR,@-r15 */
L8c6455da: *stop=0x8c6455dau;r[15]+=4294967292u;goto L8c6455dc; /* add #-0x4,r15 */
L8c6455dc: *stop=0x8c6455dcu;r[14]=r[5];goto L8c6455de; /* mov r5,r14 */
L8c6455de: *stop=0x8c6455deu;r[14]+=16u;goto L8c6455e0; /* add #0x10,r14 */
L8c6455e0: *stop=0x8c6455e0u;r[12]=r[4];goto L8c6455e2; /* mov r4,r12 */
L8c6455e2: *stop=0x8c6455e2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[6],4);if(io_status)return io_status;goto L8c6455e4; /* mov.l r6,@r15 */
L8c6455e4: *stop=0x8c6455e4u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[11]=loaded;}goto L8c6455e6; /* mov.l @r5,r11 */
L8c6455e6: target=0x8c645442u;*pr=0x8c6455eau;*stop=0x8c6455e8u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6455ea; /* bsr 0x8c645442 */
L8c6455ea: *stop=0x8c6455eau;*sr=(*sr&~1u)|((int32_t)r[11]>0);goto L8c6455ec; /* cmp/pl r11 */
L8c6455ec: *stop=0x8c6455ecu;r[10]=r[0];goto L8c6455ee; /* mov r0,r10 */
L8c6455ee: take=!(*sr&1u);*stop=0x8c6455f0u;r[13]=0u;if(take)goto L8c64562c;goto L8c6455f2; /* bf/s 0x8c64562c */
L8c6455f2: *stop=0x8c6455f2u;if(!valid_access(r[14]+9u,1u))return -3;r[0]=sx(m,bus,&io_status,r[14]+0x9u,1);if(io_status)return io_status;goto L8c6455f4; /* mov.b @(0x9,r14),r0 */
L8c6455f4: *stop=0x8c6455f4u;r[3]=r[10]&255u;goto L8c6455f6; /* extu.b r10,r3 */
L8c6455f6: *stop=0x8c6455f6u;r[0]=r[0]&255u;goto L8c6455f8; /* extu.b r0,r0 */
L8c6455f8: *stop=0x8c6455f8u;*sr=(*sr&~1u)|(r[3]==r[0]);goto L8c6455fa; /* cmp/eq r3,r0 */
L8c6455fa: if(!(*sr&1u))goto L8c645624;goto L8c6455fc; /* bf 0x8c645624 */
L8c6455fc: *stop=0x8c6455fcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6455fe; /* mov.l @r15,r0 */
L8c6455fe: *stop=0x8c6455feu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c645600; /* cmp/eq #0x1,r0 */
L8c645600: if(!(*sr&1u))goto L8c645612;goto L8c645602; /* bf 0x8c645612 */
L8c645602: *stop=0x8c645602u;r[5]=r[14];goto L8c645604; /* mov r14,r5 */
L8c645604: *stop=0x8c645604u;r[5]+=10u;goto L8c645606; /* add #0xa,r5 */
L8c645606: target=0x8c6453c0u;*pr=0x8c64560au;*stop=0x8c645608u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64560a; /* bsr 0x8c6453c0 */
L8c64560a: *stop=0x8c64560au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64560c; /* cmp/eq #0x1,r0 */
L8c64560c: if((*sr&1u))goto L8c64561e;goto L8c64560e; /* bt 0x8c64561e */
L8c64560e: *stop=0x8c645610u;goto L8c645624; /* bra 0x8c645624 */
L8c645612: *stop=0x8c645612u;r[5]=r[14];goto L8c645614; /* mov r14,r5 */
L8c645614: *stop=0x8c645614u;r[5]+=10u;goto L8c645616; /* add #0xa,r5 */
L8c645616: target=0x8c6453e2u;*pr=0x8c64561au;*stop=0x8c645618u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64561a; /* bsr 0x8c6453e2 */
L8c64561a: *stop=0x8c64561au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64561c; /* cmp/eq #0x1,r0 */
L8c64561c: if(!(*sr&1u))goto L8c645624;goto L8c64561e; /* bf 0x8c645624 */
L8c64561e: *stop=0x8c64561eu;r[0]=r[13];goto L8c645620; /* mov r13,r0 */
L8c645620: *stop=0x8c645622u;r[0]+=1u;goto L8c64562e; /* bra 0x8c64562e */
L8c645624: *stop=0x8c645624u;r[13]+=1u;goto L8c645626; /* add #0x1,r13 */
L8c645626: *stop=0x8c645626u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[11]);goto L8c645628; /* cmp/ge r11,r13 */
L8c645628: take=!(*sr&1u);*stop=0x8c64562au;r[14]+=44u;if(take)goto L8c6455f2;goto L8c64562c; /* bf/s 0x8c6455f2 */
L8c64562c: *stop=0x8c64562cu;r[0]=0u;goto L8c64562e; /* mov #0x0,r0 */
L8c64562e: *stop=0x8c64562eu;r[15]+=4u;goto L8c645630; /* add #0x4,r15 */
L8c645630: *stop=0x8c645630u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c645632; /* lds.l @r15+,PR */
L8c645632: *stop=0x8c645632u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c645634; /* mov.l @r15+,r10 */
L8c645634: *stop=0x8c645634u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c645636; /* mov.l @r15+,r11 */
L8c645636: *stop=0x8c645636u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c645638; /* mov.l @r15+,r12 */
L8c645638: *stop=0x8c645638u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c64563a; /* mov.l @r15+,r13 */
L8c64563a: *stop=0x8c64563au;if(*pr!=entry_pr)return -3;*stop=0x8c64563cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c600464(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c600464;
L8c600464: *stop=0x8c600464u;r[0]=76u;goto L8c600466; /* mov #0x4c,r0 */
L8c600466: *stop=0x8c600466u;if(!valid_access(r[0]+r[4],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[4],2);if(io_status)return io_status;goto L8c600468; /* mov.w @(r0,r4),r3 */
L8c600468: *stop=0x8c600468u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c60046a; /* tst r3,r3 */
L8c60046a: if((*sr&1u))goto L8c600484;goto L8c60046c; /* bt 0x8c600484 */
L8c60046c: *stop=0x8c60046cu;if(*pr!=entry_pr)return -3;*stop=0x8c60046eu;r[0]=0u;return 0; /* rts */
L8c600484: *stop=0x8c600484u;r[3]=sx(m,bus,&io_status,0x8c600594u,2);if(io_status)return io_status;goto L8c600486; /* mov.w 0x8c600594,r3 */
L8c600486: *stop=0x8c600486u;r[2]=r[7];goto L8c600488; /* mov r7,r2 */
L8c600488: *stop=0x8c600488u;r[1]=4294967285u;goto L8c60048a; /* mov #-0xb,r1 */
L8c60048a: *stop=0x8c60048au;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c60048c; /* mov.l r5,@(0x4,r4) */
L8c60048c: *stop=0x8c60048cu;r[2]+=r[3];goto L8c60048e; /* add r3,r2 */
L8c60048e: *stop=0x8c60048eu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[6],4);if(io_status)return io_status;goto L8c600490; /* mov.l r6,@(0x8,r4) */
L8c600490: *stop=0x8c600490u;r[5]=0u;goto L8c600492; /* mov #0x0,r5 */
L8c600492: *stop=0x8c600492u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[7],4);if(io_status)return io_status;goto L8c600494; /* mov.l r7,@(0xc,r4) */
L8c600494: *stop=0x8c600494u;{uint32_t shift=r[1];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}}goto L8c600496; /* shad r1,r2 */
L8c600496: *stop=0x8c600496u;r[0]=64u;goto L8c600498; /* mov #0x40,r0 */
L8c600498: *stop=0x8c600498u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[2],4);if(io_status)return io_status;goto L8c60049a; /* mov.l r2,@(0x10,r4) */
L8c60049a: *stop=0x8c60049au;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[5],4);if(io_status)return io_status;goto L8c60049c; /* mov.l r5,@(0x14,r4) */
L8c60049c: *stop=0x8c60049cu;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[5],4);if(io_status)return io_status;goto L8c60049e; /* mov.l r5,@(0x18,r4) */
L8c60049e: *stop=0x8c60049eu;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[5],4);if(io_status)return io_status;goto L8c6004a0; /* mov.l r5,@(0x1c,r4) */
L8c6004a0: *stop=0x8c6004a0u;if(!valid_access(r[4]+32u,4u))return -3;wr(m,bus,&io_status,r[4]+0x20u,r[5],4);if(io_status)return io_status;goto L8c6004a2; /* mov.l r5,@(0x20,r4) */
L8c6004a2: *stop=0x8c6004a2u;if(!valid_access(r[4]+36u,4u))return -3;wr(m,bus,&io_status,r[4]+0x24u,r[5],4);if(io_status)return io_status;goto L8c6004a4; /* mov.l r5,@(0x24,r4) */
L8c6004a4: *stop=0x8c6004a4u;if(!valid_access(r[4]+44u,4u))return -3;wr(m,bus,&io_status,r[4]+0x2cu,r[5],4);if(io_status)return io_status;goto L8c6004a6; /* mov.l r5,@(0x2c,r4) */
L8c6004a6: *stop=0x8c6004a6u;if(!valid_access(r[4]+52u,4u))return -3;wr(m,bus,&io_status,r[4]+0x34u,r[5],4);if(io_status)return io_status;goto L8c6004a8; /* mov.l r5,@(0x34,r4) */
L8c6004a8: *stop=0x8c6004a8u;if(!valid_access(r[4]+60u,4u))return -3;wr(m,bus,&io_status,r[4]+0x3cu,r[5],4);if(io_status)return io_status;goto L8c6004aa; /* mov.l r5,@(0x3c,r4) */
L8c6004aa: *stop=0x8c6004aau;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c6004ac; /* mov.l r5,@(r0,r4) */
L8c6004ac: *stop=0x8c6004acu;r[0]=68u;goto L8c6004ae; /* mov #0x44,r0 */
L8c6004ae: *stop=0x8c6004aeu;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c6004b0; /* mov.l r5,@(r0,r4) */
L8c6004b0: *stop=0x8c6004b0u;r[0]=72u;goto L8c6004b2; /* mov #0x48,r0 */
L8c6004b2: *stop=0x8c6004b2u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[5],2);if(io_status)return io_status;goto L8c6004b4; /* mov.w r5,@(r0,r4) */
L8c6004b4: *stop=0x8c6004b4u;r[0]=74u;goto L8c6004b6; /* mov #0x4a,r0 */
L8c6004b6: *stop=0x8c6004b6u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[5],2);if(io_status)return io_status;goto L8c6004b8; /* mov.w r5,@(r0,r4) */
L8c6004b8: *stop=0x8c6004b8u;r[0]=76u;goto L8c6004ba; /* mov #0x4c,r0 */
L8c6004ba: *stop=0x8c6004bau;r[2]=1u;goto L8c6004bc; /* mov #0x1,r2 */
L8c6004bc: *stop=0x8c6004bcu;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[2],2);if(io_status)return io_status;goto L8c6004be; /* mov.w r2,@(r0,r4) */
L8c6004be: *stop=0x8c6004beu;r[0]=78u;goto L8c6004c0; /* mov #0x4e,r0 */
L8c6004c0: *stop=0x8c6004c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c600598u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6004c2; /* mov.l 0x8c600598,r2 */
L8c6004c2: *stop=0x8c6004c2u;if(!valid_access(r[2],2u))return -3;r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c6004c4; /* mov.w @r2,r1 */
L8c6004c4: *stop=0x8c6004c4u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[1],2);if(io_status)return io_status;goto L8c6004c6; /* mov.w r1,@(r0,r4) */
L8c6004c6: *stop=0x8c6004c6u;r[0]=80u;goto L8c6004c8; /* mov #0x50,r0 */
L8c6004c8: *stop=0x8c6004c8u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[5],2);if(io_status)return io_status;goto L8c6004ca; /* mov.w r5,@(r0,r4) */
L8c6004ca: *stop=0x8c6004cau;r[0]=82u;goto L8c6004cc; /* mov #0x52,r0 */
L8c6004cc: *stop=0x8c6004ccu;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[5],2);if(io_status)return io_status;goto L8c6004ce; /* mov.w r5,@(r0,r4) */
L8c6004ce: *stop=0x8c6004ceu;r[0]=r[4];goto L8c6004d0; /* mov r4,r0 */
L8c6004d0: *stop=0x8c6004d0u;if(*pr!=entry_pr)return -3;*stop=0x8c6004d2u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c5ffa08(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c5ffa08;
L8c5ffa08: *stop=0x8c5ffa08u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffc0cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffa0a; /* mov.l 0x8c5ffc0c,r2 */
L8c5ffa0a: *stop=0x8c5ffa0au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffa0c; /* mov.l @r2,r3 */
L8c5ffa0c: *stop=0x8c5ffa0cu;if(!valid_access(r[3]+48u,4u))return -3;wr(m,bus,&io_status,r[3]+0x30u,r[4],4);if(io_status)return io_status;goto L8c5ffa0e; /* mov.l r4,@(0x30,r3) */
L8c5ffa0e: *stop=0x8c5ffa0eu;if(*pr!=entry_pr)return -3;*stop=0x8c5ffa10u;r[0]=1u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c600d82(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c600d82;
L8c600d82: *stop=0x8c600d82u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c600d84; /* tst r4,r4 */
L8c600d84: if((*sr&1u))goto L8c600d8e;goto L8c600d86; /* bt 0x8c600d8e */
L8c600d86: *stop=0x8c600d86u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[3]=loaded;}goto L8c600d88; /* mov.l @r4,r3 */
L8c600d88: *stop=0x8c600d88u;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c600d8a; /* mov.l @(0x1c,r3),r2 */
L8c600d8a: *stop=0x8c600d8au;*sr=(*sr&~1u)|(r[4]==r[2]);goto L8c600d8c; /* cmp/eq r4,r2 */
L8c600d8c: if(!(*sr&1u))goto L8c600d92;goto L8c600d8e; /* bf 0x8c600d92 */
L8c600d8e: *stop=0x8c600d8eu;if(*pr!=entry_pr)return -3;*stop=0x8c600d90u;r[0]=0u;return 0; /* rts */
L8c600d92: *stop=0x8c600d92u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c600d94; /* mov.l r5,@(0x4,r4) */
L8c600d94: *stop=0x8c600d94u;r[5]=0u;goto L8c600d96; /* mov #0x0,r5 */
L8c600d96: *stop=0x8c600d96u;r[0]=68u;goto L8c600d98; /* mov #0x44,r0 */
L8c600d98: *stop=0x8c600d98u;r[3]=sx(m,bus,&io_status,0x8c600de2u,2);if(io_status)return io_status;goto L8c600d9a; /* mov.w 0x8c600de2,r3 */
L8c600d9a: *stop=0x8c600d9au;r[2]=r[7];goto L8c600d9c; /* mov r7,r2 */
L8c600d9c: *stop=0x8c600d9cu;r[1]=4294967285u;goto L8c600d9e; /* mov #-0xb,r1 */
L8c600d9e: *stop=0x8c600d9eu;r[2]+=r[3];goto L8c600da0; /* add r3,r2 */
L8c600da0: *stop=0x8c600da0u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[6],4);if(io_status)return io_status;goto L8c600da2; /* mov.l r6,@(0x8,r4) */
L8c600da2: *stop=0x8c600da2u;{uint32_t shift=r[1];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}}goto L8c600da4; /* shad r1,r2 */
L8c600da4: *stop=0x8c600da4u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[7],4);if(io_status)return io_status;goto L8c600da6; /* mov.l r7,@(0xc,r4) */
L8c600da6: *stop=0x8c600da6u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[2],4);if(io_status)return io_status;goto L8c600da8; /* mov.l r2,@(0x10,r4) */
L8c600da8: *stop=0x8c600da8u;r[2]=1u;goto L8c600daa; /* mov #0x1,r2 */
L8c600daa: *stop=0x8c600daau;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[5],4);if(io_status)return io_status;goto L8c600dac; /* mov.l r5,@(0x14,r4) */
L8c600dac: *stop=0x8c600dacu;if(!valid_access(r[4]+24u,4u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[5],4);if(io_status)return io_status;goto L8c600dae; /* mov.l r5,@(0x18,r4) */
L8c600dae: *stop=0x8c600daeu;if(!valid_access(r[4]+28u,4u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[5],4);if(io_status)return io_status;goto L8c600db0; /* mov.l r5,@(0x1c,r4) */
L8c600db0: *stop=0x8c600db0u;if(!valid_access(r[4]+32u,4u))return -3;wr(m,bus,&io_status,r[4]+0x20u,r[5],4);if(io_status)return io_status;goto L8c600db2; /* mov.l r5,@(0x20,r4) */
L8c600db2: *stop=0x8c600db2u;if(!valid_access(r[4]+60u,4u))return -3;wr(m,bus,&io_status,r[4]+0x3cu,r[5],4);if(io_status)return io_status;goto L8c600db4; /* mov.l r5,@(0x3c,r4) */
L8c600db4: *stop=0x8c600db4u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[5],4);if(io_status)return io_status;goto L8c600db6; /* mov.l r5,@(r0,r4) */
L8c600db6: *stop=0x8c600db6u;r[0]=72u;goto L8c600db8; /* mov #0x48,r0 */
L8c600db8: *stop=0x8c600db8u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[5],2);if(io_status)return io_status;goto L8c600dba; /* mov.w r5,@(r0,r4) */
L8c600dba: *stop=0x8c600dbau;r[0]=74u;goto L8c600dbc; /* mov #0x4a,r0 */
L8c600dbc: *stop=0x8c600dbcu;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[5],2);if(io_status)return io_status;goto L8c600dbe; /* mov.w r5,@(r0,r4) */
L8c600dbe: *stop=0x8c600dbeu;r[0]=76u;goto L8c600dc0; /* mov #0x4c,r0 */
L8c600dc0: *stop=0x8c600dc0u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[2],2);if(io_status)return io_status;goto L8c600dc2; /* mov.w r2,@(r0,r4) */
L8c600dc2: *stop=0x8c600dc2u;r[0]=80u;goto L8c600dc4; /* mov #0x50,r0 */
L8c600dc4: *stop=0x8c600dc4u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[5],2);if(io_status)return io_status;goto L8c600dc6; /* mov.w r5,@(r0,r4) */
L8c600dc6: *stop=0x8c600dc6u;r[0]=82u;goto L8c600dc8; /* mov #0x52,r0 */
L8c600dc8: *stop=0x8c600dc8u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[5],2);if(io_status)return io_status;goto L8c600dca; /* mov.w r5,@(r0,r4) */
L8c600dca: *stop=0x8c600dcau;r[0]=r[4];goto L8c600dcc; /* mov r4,r0 */
L8c600dcc: *stop=0x8c600dccu;if(*pr!=entry_pr)return -3;*stop=0x8c600dceu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6013a4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6013a4;
L8c6013a4: *stop=0x8c6013a4u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6013a6; /* tst r4,r4 */
L8c6013a6: if((*sr&1u))goto L8c6013b0;goto L8c6013a8; /* bt 0x8c6013b0 */
L8c6013a8: *stop=0x8c6013a8u;r[0]=76u;goto L8c6013aa; /* mov #0x4c,r0 */
L8c6013aa: *stop=0x8c6013aau;if(!valid_access(r[0]+r[4],2u))return -3;r[3]=sx(m,bus,&io_status,r[0]+r[4],2);if(io_status)return io_status;goto L8c6013ac; /* mov.w @(r0,r4),r3 */
L8c6013ac: *stop=0x8c6013acu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6013ae; /* tst r3,r3 */
L8c6013ae: if(!(*sr&1u))goto L8c6013b4;goto L8c6013b0; /* bf 0x8c6013b4 */
L8c6013b0: *stop=0x8c6013b0u;if(*pr!=entry_pr)return -3;*stop=0x8c6013b2u;r[0]=4294967286u;return 0; /* rts */
L8c6013b4: *stop=0x8c6013b4u;r[0]=80u;goto L8c6013b6; /* mov #0x50,r0 */
L8c6013b6: *stop=0x8c6013b6u;if(!valid_access(r[0]+r[4],2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+r[4],2);if(io_status)return io_status;goto L8c6013b8; /* mov.w @(r0,r4),r0 */
L8c6013b8: *stop=0x8c6013b8u;if(*pr!=entry_pr)return -3;*stop=0x8c6013bau;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c645442(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c645442;
L8c645442: *stop=0x8c645442u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c645444; /* mov.l r14,@-r15 */
L8c645444: *stop=0x8c645444u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c645446; /* mov.l r13,@-r15 */
L8c645446: *stop=0x8c645446u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c645448; /* sts.l PR,@-r15 */
L8c645448: *stop=0x8c645448u;r[13]=sx(m,bus,&io_status,0x8c64554eu,2);if(io_status)return io_status;goto L8c64544a; /* mov.w 0x8c64554e,r13 */
L8c64544a: *stop=0x8c64544cu;r[14]=r[4];goto L8c645456; /* bra 0x8c645456 */
L8c64544e: target=0x8c645348u;*pr=0x8c645452u;*stop=0x8c645450u;if(!valid_access(r[14],1u))return -3;r[4]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[14]+=1;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c645452; /* bsr 0x8c645348 */
L8c645452: *stop=0x8c645452u;r[0]=r[0]&255u;goto L8c645454; /* extu.b r0,r0 */
L8c645454: *stop=0x8c645454u;r[13]+=r[0];goto L8c645456; /* add r0,r13 */
L8c645456: *stop=0x8c645456u;if(!valid_access(r[14],1u))return -3;r[2]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c645458; /* mov.b @r14,r2 */
L8c645458: *stop=0x8c645458u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64545a; /* tst r2,r2 */
L8c64545a: if(!(*sr&1u))goto L8c64544e;goto L8c64545c; /* bf 0x8c64544e */
L8c64545c: *stop=0x8c64545cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c64545e; /* lds.l @r15+,PR */
L8c64545e: *stop=0x8c64545eu;r[13]>>=2;goto L8c645460; /* shlr2 r13 */
L8c645460: *stop=0x8c645460u;r[0]=r[13]&255u;goto L8c645462; /* extu.b r13,r0 */
L8c645462: *stop=0x8c645462u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c645464; /* mov.l @r15+,r13 */
L8c645464: *stop=0x8c645464u;if(*pr!=entry_pr)return -3;*stop=0x8c645466u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6453e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6453e2;
L8c6453e2: *stop=0x8c6453e2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6453e4; /* mov.l r14,@-r15 */
L8c6453e4: *stop=0x8c6453e4u;r[14]=r[4];goto L8c6453e6; /* mov r4,r14 */
L8c6453e6: *stop=0x8c6453e6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6453e8; /* mov.l r13,@-r15 */
L8c6453e8: *stop=0x8c6453e8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6453ea; /* mov.l r8,@-r15 */
L8c6453ea: *stop=0x8c6453eau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6453ec; /* sts.l PR,@-r15 */
L8c6453ec: *stop=0x8c6453eeu;r[13]=r[5];goto L8c645400; /* bra 0x8c645400 */
L8c6453f0: target=0x8c645348u;*pr=0x8c6453f4u;*stop=0x8c6453f2u;if(!valid_access(r[14],1u))return -3;r[4]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;r[14]+=1;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6453f4; /* bsr 0x8c645348 */
L8c6453f4: *stop=0x8c6453f4u;r[8]=(uint32_t)(int32_t)(int8_t)r[0];goto L8c6453f6; /* exts.b r0,r8 */
L8c6453f6: target=0x8c645348u;*pr=0x8c6453fau;*stop=0x8c6453f8u;if(!valid_access(r[13],1u))return -3;r[4]=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;r[13]+=1;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6453fa; /* bsr 0x8c645348 */
L8c6453fa: *stop=0x8c6453fau;r[0]=(uint32_t)(int32_t)(int8_t)r[0];goto L8c6453fc; /* exts.b r0,r0 */
L8c6453fc: *stop=0x8c6453fcu;*sr=(*sr&~1u)|(r[0]==r[8]);goto L8c6453fe; /* cmp/eq r0,r8 */
L8c6453fe: if(!(*sr&1u))goto L8c64540c;goto L8c645400; /* bf 0x8c64540c */
L8c645400: *stop=0x8c645400u;if(!valid_access(r[14],1u))return -3;r[3]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c645402; /* mov.b @r14,r3 */
L8c645402: *stop=0x8c645402u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c645404; /* tst r3,r3 */
L8c645404: if(!(*sr&1u))goto L8c6453f0;goto L8c645406; /* bf 0x8c6453f0 */
L8c645406: *stop=0x8c645406u;if(!valid_access(r[13],1u))return -3;r[1]=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;goto L8c645408; /* mov.b @r13,r1 */
L8c645408: *stop=0x8c645408u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c64540a; /* tst r1,r1 */
L8c64540a: if((*sr&1u))goto L8c645410;goto L8c64540c; /* bt 0x8c645410 */
L8c64540c: *stop=0x8c64540eu;r[0]=0u;goto L8c645412; /* bra 0x8c645412 */
L8c645410: *stop=0x8c645410u;r[0]=1u;goto L8c645412; /* mov #0x1,r0 */
L8c645412: *stop=0x8c645412u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c645414; /* lds.l @r15+,PR */
L8c645414: *stop=0x8c645414u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c645416; /* mov.l @r15+,r8 */
L8c645416: *stop=0x8c645416u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c645418; /* mov.l @r15+,r13 */
L8c645418: *stop=0x8c645418u;if(*pr!=entry_pr)return -3;*stop=0x8c64541au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c6453c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6453c0;
L8c6453c0: *stop=0x8c6453c2u;if(!call)return -2;return call(ctx,0x8c6453ccu,m,f,sr,r,pr); /* bra 0x8c6453cc */
}

SA_STAGE_EXPORT int sa_boot_file_8c6453cc(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c6453cc;
L8c6453c4: *stop=0x8c6453c4u;if(!valid_access(r[5],1u))return -3;r[2]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;r[5]+=1;goto L8c6453c6; /* mov.b @r5+,r2 */
L8c6453c6: *stop=0x8c6453c6u;if(!valid_access(r[4],1u))return -3;r[3]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;r[4]+=1;goto L8c6453c8; /* mov.b @r4+,r3 */
L8c6453c8: *stop=0x8c6453c8u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c6453ca; /* cmp/eq r2,r3 */
L8c6453ca: if(!(*sr&1u))goto L8c6453d8;goto L8c6453cc; /* bf 0x8c6453d8 */
L8c6453cc: *stop=0x8c6453ccu;if(!valid_access(r[4],1u))return -3;r[3]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;goto L8c6453ce; /* mov.b @r4,r3 */
L8c6453ce: *stop=0x8c6453ceu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6453d0; /* tst r3,r3 */
L8c6453d0: if(!(*sr&1u))goto L8c6453c4;goto L8c6453d2; /* bf 0x8c6453c4 */
L8c6453d2: *stop=0x8c6453d2u;if(!valid_access(r[5],1u))return -3;r[1]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c6453d4; /* mov.b @r5,r1 */
L8c6453d4: *stop=0x8c6453d4u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c6453d6; /* tst r1,r1 */
L8c6453d6: if((*sr&1u))goto L8c6453dc;goto L8c6453d8; /* bt 0x8c6453dc */
L8c6453d8: *stop=0x8c6453d8u;if(*pr!=entry_pr)return -3;*stop=0x8c6453dau;r[0]=0u;return 0; /* rts */
L8c6453dc: *stop=0x8c6453dcu;r[0]=1u;goto L8c6453de; /* mov #0x1,r0 */
L8c6453de: *stop=0x8c6453deu;if(*pr!=entry_pr)return -3;*stop=0x8c6453e0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c10d938(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d938;
L8c10d938: *stop=0x8c10d938u;r[6]=0u;goto L8c10d93a; /* mov #0x0,r6 */
L8c10d93a: *stop=0x8c10d93au;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d944u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10d93c; /* mov.l 0x8c10d944,r7 */
L8c10d93c: *stop=0x8c10d93cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d948u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d93e; /* mov.l 0x8c10d948,r0 */
L8c10d93e: *stop=0x8c10d93eu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d940; /* mov.l @r0,r0 */
L8c10d940: target=r[0];*stop=0x8c10d942u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}

SA_STAGE_EXPORT int sa_boot_file_8c10d960(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d960;
L8c10d960: *stop=0x8c10d960u;r[6]=0u;goto L8c10d962; /* mov #0x0,r6 */
L8c10d962: *stop=0x8c10d962u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d96cu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10d964; /* mov.l 0x8c10d96c,r7 */
L8c10d964: *stop=0x8c10d964u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d970u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d966; /* mov.l 0x8c10d970,r0 */
L8c10d966: *stop=0x8c10d966u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d968; /* mov.l @r0,r0 */
L8c10d968: target=r[0];*stop=0x8c10d96au;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}

SA_STAGE_EXPORT int sa_boot_file_8c10d988(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d988;
L8c10d988: *stop=0x8c10d988u;r[6]=0u;goto L8c10d98a; /* mov #0x0,r6 */
L8c10d98a: *stop=0x8c10d98au;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d994u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c10d98c; /* mov.l 0x8c10d994,r7 */
L8c10d98c: *stop=0x8c10d98cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10d998u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d98e; /* mov.l 0x8c10d998,r0 */
L8c10d98e: *stop=0x8c10d98eu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c10d990; /* mov.l @r0,r0 */
L8c10d990: target=r[0];*stop=0x8c10d992u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r0 */
}

SA_STAGE_EXPORT int sa_boot_file_8c6005c2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6005c2;
L8c5ffa12: *stop=0x8c5ffa12u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c5ffa14; /* mov.l r14,@-r15 */
L8c5ffa14: *stop=0x8c5ffa14u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c5ffa16; /* mov.l r13,@-r15 */
L8c5ffa16: *stop=0x8c5ffa16u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c5ffa18; /* mov.l r12,@-r15 */
L8c5ffa18: *stop=0x8c5ffa18u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c5ffa1a; /* mov.l r11,@-r15 */
L8c5ffa1a: *stop=0x8c5ffa1au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c5ffa1c; /* mov.l r10,@-r15 */
L8c5ffa1c: *stop=0x8c5ffa1cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c5ffa1e; /* mov.l r9,@-r15 */
L8c5ffa1e: *stop=0x8c5ffa1eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c5ffa20; /* sts.l PR,@-r15 */
L8c5ffa20: *stop=0x8c5ffa20u;r[15]+=4294967256u;goto L8c5ffa22; /* add #-0x28,r15 */
L8c5ffa22: *stop=0x8c5ffa22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffc0cu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c5ffa24; /* mov.l 0x8c5ffc0c,r13 */
L8c5ffa24: *stop=0x8c5ffa24u;r[2]=1u;goto L8c5ffa26; /* mov #0x1,r2 */
L8c5ffa26: *stop=0x8c5ffa26u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffa28; /* mov.l @r13,r3 */
L8c5ffa28: *stop=0x8c5ffa28u;if(!valid_access(r[3]+32u,4u))return -3;wr(m,bus,&io_status,r[3]+0x20u,r[2],4);if(io_status)return io_status;goto L8c5ffa2a; /* mov.l r2,@(0x20,r3) */
L8c5ffa2a: *stop=0x8c5ffa2au;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[14]=loaded;}goto L8c5ffa2c; /* mov.l @r13,r14 */
L8c5ffa2c: *stop=0x8c5ffa2cu;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[14]=loaded;}goto L8c5ffa2e; /* mov.l @(0x1c,r14),r14 */
L8c5ffa2e: *stop=0x8c5ffa2eu;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c5ffa30; /* tst r14,r14 */
L8c5ffa30: take=!(*sr&1u);*stop=0x8c5ffa32u;r[12]=0u;if(take)goto L8c5ffa38;goto L8c5ffa34; /* bf/s 0x8c5ffa38 */
L8c5ffa34: *stop=0x8c5ffa36u;r[4]=0u;goto L8c5ffa3a; /* bra 0x8c5ffa3a */
L8c5ffa38: *stop=0x8c5ffa38u;if(!valid_access(r[14]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x3cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c5ffa3a; /* mov.l @(0x3c,r14),r4 */
L8c5ffa3a: *stop=0x8c5ffa3au;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[11]=loaded;}goto L8c5ffa3c; /* mov.l @r13,r11 */
L8c5ffa3c: *stop=0x8c5ffa3cu;r[10]=r[15];goto L8c5ffa3e; /* mov r15,r10 */
L8c5ffa3e: *stop=0x8c5ffa3eu;r[10]+=28u;goto L8c5ffa40; /* add #0x1c,r10 */
L8c5ffa40: *stop=0x8c5ffa40u;if(!valid_access(r[11]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c5ffa42; /* mov.l @(0xc,r11),r11 */
L8c5ffa42: *stop=0x8c5ffa42u;if(!valid_access(r[11]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+0x10u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c5ffa44; /* mov.l @(0x10,r11),r11 */
L8c5ffa44: target=r[11];*pr=0x8c5ffa48u;*stop=0x8c5ffa46u;r[5]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffa48; /* jsr @r11 */
L8c5ffa48: *stop=0x8c5ffa48u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c5ffa4a; /* tst r14,r14 */
L8c5ffa4a: take=!(*sr&1u);*stop=0x8c5ffa4cu;r[11]=r[0];if(take)goto L8c5ffa52;goto L8c5ffa4e; /* bf/s 0x8c5ffa52 */
L8c5ffa4e: *stop=0x8c5ffa50u;goto L8c5ffc80; /* bra 0x8c5ffc80 */
L8c5ffa52: *stop=0x8c5ffa52u;r[0]=r[11];goto L8c5ffa54; /* mov r11,r0 */
L8c5ffa54: *stop=0x8c5ffa54u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c5ffa56; /* cmp/eq #0x0,r0 */
L8c5ffa56: take=(*sr&1u);*stop=0x8c5ffa58u;r[9]=2u;if(take)goto L8c5ffa8a;goto L8c5ffa5a; /* bt/s 0x8c5ffa8a */
L8c5ffa5a: *stop=0x8c5ffa5au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c5ffa5c; /* cmp/eq #0x1,r0 */
L8c5ffa5c: if((*sr&1u))goto L8c5ffa8a;goto L8c5ffa5e; /* bt 0x8c5ffa8a */
L8c5ffa5e: *stop=0x8c5ffa5eu;*sr=(*sr&~1u)|(r[0]==0x00000041u);goto L8c5ffa60; /* cmp/eq #0x41,r0 */
L8c5ffa60: if(!(*sr&1u))goto L8c5ffa66;goto L8c5ffa62; /* bf 0x8c5ffa66 */
L8c5ffa62: *stop=0x8c5ffa64u;goto L8c5ffb84; /* bra 0x8c5ffb84 */
L8c5ffa66: *stop=0x8c5ffa66u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c5ffa68; /* cmp/eq #0x4,r0 */
L8c5ffa68: if(!(*sr&1u))goto L8c5ffa6e;goto L8c5ffa6a; /* bf 0x8c5ffa6e */
L8c5ffa6a: *stop=0x8c5ffa6cu;goto L8c5ffbb8; /* bra 0x8c5ffbb8 */
L8c5ffa6e: *stop=0x8c5ffa6eu;*sr=(*sr&~1u)|(r[0]==0x00000043u);goto L8c5ffa70; /* cmp/eq #0x43,r0 */
L8c5ffa70: if(!(*sr&1u))goto L8c5ffa76;goto L8c5ffa72; /* bf 0x8c5ffa76 */
L8c5ffa72: *stop=0x8c5ffa74u;goto L8c5ffc80; /* bra 0x8c5ffc80 */
L8c5ffa76: *stop=0x8c5ffa76u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c5ffa78; /* cmp/eq #0x5,r0 */
L8c5ffa78: if(!(*sr&1u))goto L8c5ffa7e;goto L8c5ffa7a; /* bf 0x8c5ffa7e */
L8c5ffa7a: *stop=0x8c5ffa7cu;goto L8c5ffbcc; /* bra 0x8c5ffbcc */
L8c5ffa7e: *stop=0x8c5ffa7eu;*sr=(*sr&~1u)|(r[0]==0x00000040u);goto L8c5ffa80; /* cmp/eq #0x40,r0 */
L8c5ffa80: if(!(*sr&1u))goto L8c5ffa86;goto L8c5ffa82; /* bf 0x8c5ffa86 */
L8c5ffa82: *stop=0x8c5ffa84u;goto L8c5ffbcc; /* bra 0x8c5ffbcc */
L8c5ffa86: *stop=0x8c5ffa88u;goto L8c5ffc74; /* bra 0x8c5ffc74 */
L8c5ffa8a: *stop=0x8c5ffa8au;r[0]=72u;goto L8c5ffa8c; /* mov #0x48,r0 */
L8c5ffa8c: *stop=0x8c5ffa8cu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffa8e; /* mov.l @r13,r3 */
L8c5ffa8e: *stop=0x8c5ffa8eu;if(!valid_access(r[0]+r[14],2u))return -3;r[4]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c5ffa90; /* mov.w @(r0,r14),r4 */
L8c5ffa90: *stop=0x8c5ffa90u;if(!valid_access(r[3]+28u,4u))return -3;wr(m,bus,&io_status,r[3]+0x1cu,r[12],4);if(io_status)return io_status;goto L8c5ffa92; /* mov.l r12,@(0x1c,r3) */
L8c5ffa92: *stop=0x8c5ffa92u;r[0]=80u;goto L8c5ffa94; /* mov #0x50,r0 */
L8c5ffa94: *stop=0x8c5ffa94u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[11],2);if(io_status)return io_status;goto L8c5ffa96; /* mov.w r11,@(r0,r14) */
L8c5ffa96: *stop=0x8c5ffa96u;r[0]=72u;goto L8c5ffa98; /* mov #0x48,r0 */
L8c5ffa98: *stop=0x8c5ffa98u;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[12],4);if(io_status)return io_status;goto L8c5ffa9a; /* mov.l r12,@(0x3c,r14) */
L8c5ffa9a: *stop=0x8c5ffa9au;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[12],2);if(io_status)return io_status;goto L8c5ffa9c; /* mov.w r12,@(r0,r14) */
L8c5ffa9c: *stop=0x8c5ffa9cu;r[0]=r[11];goto L8c5ffa9e; /* mov r11,r0 */
L8c5ffa9e: *stop=0x8c5ffa9eu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c5ffaa0; /* cmp/eq #0x1,r0 */
L8c5ffaa0: if((*sr&1u))goto L8c5ffaa6;goto L8c5ffaa2; /* bt 0x8c5ffaa6 */
L8c5ffaa2: *stop=0x8c5ffaa4u;goto L8c5ffc80; /* bra 0x8c5ffc80 */
L8c5ffaa6: *stop=0x8c5ffaa6u;r[0]=r[4];goto L8c5ffaa8; /* mov r4,r0 */
L8c5ffaa8: *stop=0x8c5ffaa8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c5ffaaa; /* cmp/eq #0x1,r0 */
L8c5ffaaa: if(!(*sr&1u))goto L8c5ffad0;goto L8c5ffaac; /* bf 0x8c5ffad0 */
L8c5ffaac: *stop=0x8c5ffaacu;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffaae; /* mov.l @r10,r3 */
L8c5ffaae: *stop=0x8c5ffaaeu;r[0]=74u;goto L8c5ffab0; /* mov #0x4a,r0 */
L8c5ffab0: *stop=0x8c5ffab0u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[3],4);if(io_status)return io_status;goto L8c5ffab2; /* mov.l r3,@(0x18,r14) */
L8c5ffab2: *stop=0x8c5ffab2u;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffab4; /* mov.l @(0x14,r14),r2 */
L8c5ffab4: *stop=0x8c5ffab4u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffab6; /* mov.l @(0x1c,r14),r3 */
L8c5ffab6: *stop=0x8c5ffab6u;r[2]+=r[3];goto L8c5ffab8; /* add r3,r2 */
L8c5ffab8: *stop=0x8c5ffab8u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[2],4);if(io_status)return io_status;goto L8c5ffaba; /* mov.l r2,@(0x14,r14) */
L8c5ffaba: *stop=0x8c5ffabau;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[9],2);if(io_status)return io_status;goto L8c5ffabc; /* mov.w r9,@(r0,r14) */
L8c5ffabc: *stop=0x8c5ffabcu;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffabe; /* mov.l @(0x24,r14),r3 */
L8c5ffabe: *stop=0x8c5ffabeu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c5ffac0; /* tst r3,r3 */
L8c5ffac0: if(!(*sr&1u))goto L8c5ffac6;goto L8c5ffac2; /* bf 0x8c5ffac6 */
L8c5ffac2: *stop=0x8c5ffac4u;goto L8c5ffc80; /* bra 0x8c5ffc80 */
L8c5ffac6: *stop=0x8c5ffac6u;if(!valid_access(r[14]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffac8; /* mov.l @(0x24,r14),r3 */
L8c5ffac8: target=r[3];*pr=0x8c5ffaccu;*stop=0x8c5ffacau;if(!valid_access(r[14]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffacc; /* jsr @r3 */
L8c5ffacc: *stop=0x8c5ffaceu;goto L8c5ffc80; /* bra 0x8c5ffc80 */
L8c5ffad0: *stop=0x8c5ffad0u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c5ffad2; /* cmp/eq #0x3,r0 */
L8c5ffad2: if((*sr&1u))goto L8c5ffae6;goto L8c5ffad4; /* bt 0x8c5ffae6 */
L8c5ffad4: *stop=0x8c5ffad4u;r[0]=r[4];goto L8c5ffad6; /* mov r4,r0 */
L8c5ffad6: *stop=0x8c5ffad6u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c5ffad8; /* cmp/eq #0x4,r0 */
L8c5ffad8: if(!(*sr&1u))goto L8c5ffaec;goto L8c5ffada; /* bf 0x8c5ffaec */
L8c5ffada: *stop=0x8c5ffadau;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffadc; /* mov.l @(0x20,r14),r3 */
L8c5ffadc: *stop=0x8c5ffadcu;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[3],4);if(io_status)return io_status;goto L8c5ffade; /* mov.l r3,@(0x18,r14) */
L8c5ffade: *stop=0x8c5ffadeu;if(!valid_access(r[14]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffae0; /* mov.l @(0x14,r14),r2 */
L8c5ffae0: *stop=0x8c5ffae0u;if(!valid_access(r[14]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffae2; /* mov.l @(0x1c,r14),r3 */
L8c5ffae2: *stop=0x8c5ffae2u;r[2]+=r[3];goto L8c5ffae4; /* add r3,r2 */
L8c5ffae4: *stop=0x8c5ffae4u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[2],4);if(io_status)return io_status;goto L8c5ffae6; /* mov.l r2,@(0x14,r14) */
L8c5ffae6: *stop=0x8c5ffae6u;r[0]=74u;goto L8c5ffae8; /* mov #0x4a,r0 */
L8c5ffae8: *stop=0x8c5ffaeau;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[9],2);if(io_status)return io_status;goto L8c5ffc80; /* bra 0x8c5ffc80 */
L8c5ffaec: *stop=0x8c5ffaecu;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c5ffaee; /* cmp/eq #0x7,r0 */
L8c5ffaee: if(!(*sr&1u))goto L8c5ffb38;goto L8c5ffaf0; /* bf 0x8c5ffb38 */
L8c5ffaf0: *stop=0x8c5ffaf0u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[4]=loaded;}goto L8c5ffaf2; /* mov.l @r13,r4 */
L8c5ffaf2: *stop=0x8c5ffaf2u;r[5]=r[15];goto L8c5ffaf4; /* mov r15,r5 */
L8c5ffaf4: *stop=0x8c5ffaf4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffc10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffaf6; /* mov.l 0x8c5ffc10,r3 */
L8c5ffaf6: *stop=0x8c5ffaf6u;r[5]+=4u;goto L8c5ffaf8; /* add #0x4,r5 */
L8c5ffaf8: target=r[3];*pr=0x8c5ffafcu;*stop=0x8c5ffafau;r[4]+=64u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffafc; /* jsr @r3 */
L8c5ffafc: *stop=0x8c5ffafcu;r[4]=r[0];goto L8c5ffafe; /* mov r0,r4 */
L8c5ffafe: *stop=0x8c5ffafeu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ffb00; /* tst r4,r4 */
L8c5ffb00: if((*sr&1u))goto L8c5ffb14;goto L8c5ffb02; /* bt 0x8c5ffb14 */
L8c5ffb02: *stop=0x8c5ffb02u;r[0]=80u;goto L8c5ffb04; /* mov #0x50,r0 */
L8c5ffb04: *stop=0x8c5ffb04u;r[3]=5u;goto L8c5ffb06; /* mov #0x5,r3 */
L8c5ffb06: *stop=0x8c5ffb06u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[3],2);if(io_status)return io_status;goto L8c5ffb08; /* mov.w r3,@(r0,r14) */
L8c5ffb08: *stop=0x8c5ffb08u;r[0]=82u;goto L8c5ffb0a; /* mov #0x52,r0 */
L8c5ffb0a: *stop=0x8c5ffb0au;r[2]=4294967277u;goto L8c5ffb0c; /* mov #-0x13,r2 */
L8c5ffb0c: *stop=0x8c5ffb0cu;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[2],2);if(io_status)return io_status;goto L8c5ffb0e; /* mov.w r2,@(r0,r14) */
L8c5ffb0e: *stop=0x8c5ffb0eu;r[0]=68u;goto L8c5ffb10; /* mov #0x44,r0 */
L8c5ffb10: *stop=0x8c5ffb12u;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[12],4);if(io_status)return io_status;goto L8c5ffc34; /* bra 0x8c5ffc34 */
L8c5ffb14: *stop=0x8c5ffb14u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffb16; /* mov.l @(0x4,r15),r3 */
L8c5ffb16: *stop=0x8c5ffb16u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffb18; /* mov.l @(0x8,r14),r2 */
L8c5ffb18: *stop=0x8c5ffb18u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c5ffb1a; /* mov.l r3,@r2 */
L8c5ffb1a: *stop=0x8c5ffb1au;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffb1c; /* mov.l @(0x8,r15),r3 */
L8c5ffb1c: *stop=0x8c5ffb1cu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffb1e; /* mov.l @(0x8,r14),r2 */
L8c5ffb1e: *stop=0x8c5ffb1eu;if(!valid_access(r[2]+4u,4u))return -3;wr(m,bus,&io_status,r[2]+0x4u,r[3],4);if(io_status)return io_status;goto L8c5ffb20; /* mov.l r3,@(0x4,r2) */
L8c5ffb20: *stop=0x8c5ffb20u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffb22; /* mov.l @(0xc,r15),r3 */
L8c5ffb22: *stop=0x8c5ffb22u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffb24; /* mov.l @(0x8,r14),r2 */
L8c5ffb24: *stop=0x8c5ffb24u;if(!valid_access(r[2]+8u,4u))return -3;wr(m,bus,&io_status,r[2]+0x8u,r[3],4);if(io_status)return io_status;goto L8c5ffb26; /* mov.l r3,@(0x8,r2) */
L8c5ffb26: *stop=0x8c5ffb26u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffb28; /* mov.l @(0x10,r15),r3 */
L8c5ffb28: *stop=0x8c5ffb28u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffb2a; /* mov.l @(0x8,r14),r2 */
L8c5ffb2a: *stop=0x8c5ffb2au;if(!valid_access(r[2]+12u,4u))return -3;wr(m,bus,&io_status,r[2]+0xcu,r[3],4);if(io_status)return io_status;goto L8c5ffb2c; /* mov.l r3,@(0xc,r2) */
L8c5ffb2c: *stop=0x8c5ffb2cu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c5ffb2e; /* mov.l @(0x14,r15),r1 */
L8c5ffb2e: *stop=0x8c5ffb2eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffc14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffb30; /* mov.l 0x8c5ffc14,r3 */
L8c5ffb30: *stop=0x8c5ffb30u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffb32; /* mov.l @(0x8,r14),r2 */
L8c5ffb32: *stop=0x8c5ffb32u;r[1]&=r[3];goto L8c5ffb34; /* and r3,r1 */
L8c5ffb34: *stop=0x8c5ffb36u;if(!valid_access(r[2]+16u,4u))return -3;wr(m,bus,&io_status,r[2]+0x10u,r[1],4);if(io_status)return io_status;goto L8c5ffc80; /* bra 0x8c5ffc80 */
L8c5ffb38: *stop=0x8c5ffb38u;r[0]=r[4];goto L8c5ffb3a; /* mov r4,r0 */
L8c5ffb3a: *stop=0x8c5ffb3au;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c5ffb3c; /* cmp/eq #0x8,r0 */
L8c5ffb3c: if((*sr&1u))goto L8c5ffb42;goto L8c5ffb3e; /* bt 0x8c5ffb42 */
L8c5ffb3e: *stop=0x8c5ffb40u;goto L8c5ffc80; /* bra 0x8c5ffc80 */
L8c5ffb42: *stop=0x8c5ffb42u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffb44; /* mov.l @r13,r3 */
L8c5ffb44: *stop=0x8c5ffb44u;r[0]=72u;goto L8c5ffb46; /* mov #0x48,r0 */
L8c5ffb46: *stop=0x8c5ffb46u;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffc1cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c5ffb48; /* mov.l 0x8c5ffc1c,r5 */
L8c5ffb48: *stop=0x8c5ffb48u;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffb4a; /* mov.l @(0xc,r3),r2 */
L8c5ffb4a: *stop=0x8c5ffb4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffc20u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c5ffb4c; /* mov.l 0x8c5ffc20,r4 */
L8c5ffb4c: *stop=0x8c5ffb4cu;if(!valid_access(r[0]+r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c5ffb4e; /* mov.l @(r0,r2),r1 */
L8c5ffb4e: *stop=0x8c5ffb4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c5ffc18u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c5ffb50; /* mov.l 0x8c5ffc18,r7 */
L8c5ffb50: target=r[1];*pr=0x8c5ffb54u;*stop=0x8c5ffb52u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffb54; /* jsr @r1 */
L8c5ffb54: *stop=0x8c5ffb54u;r[4]=r[0];goto L8c5ffb56; /* mov r0,r4 */
L8c5ffb56: *stop=0x8c5ffb56u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ffb58; /* tst r4,r4 */
L8c5ffb58: if((*sr&1u))goto L8c5ffb6e;goto L8c5ffb5a; /* bt 0x8c5ffb6e */
L8c5ffb5a: *stop=0x8c5ffb5au;r[0]=80u;goto L8c5ffb5c; /* mov #0x50,r0 */
L8c5ffb5c: *stop=0x8c5ffb5cu;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[4],4);if(io_status)return io_status;goto L8c5ffb5e; /* mov.l r4,@(0x3c,r14) */
L8c5ffb5e: *stop=0x8c5ffb5eu;r[2]=4u;goto L8c5ffb60; /* mov #0x4,r2 */
L8c5ffb60: *stop=0x8c5ffb60u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[2],2);if(io_status)return io_status;goto L8c5ffb62; /* mov.w r2,@(r0,r14) */
L8c5ffb62: *stop=0x8c5ffb62u;r[0]=72u;goto L8c5ffb64; /* mov #0x48,r0 */
L8c5ffb64: *stop=0x8c5ffb64u;r[3]=6u;goto L8c5ffb66; /* mov #0x6,r3 */
L8c5ffb66: *stop=0x8c5ffb66u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[3],2);if(io_status)return io_status;goto L8c5ffb68; /* mov.w r3,@(r0,r14) */
L8c5ffb68: *stop=0x8c5ffb68u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffb6a; /* mov.l @r13,r2 */
L8c5ffb6a: *stop=0x8c5ffb6cu;if(!valid_access(r[2]+28u,4u))return -3;wr(m,bus,&io_status,r[2]+0x1cu,r[14],4);if(io_status)return io_status;goto L8c5ffc80; /* bra 0x8c5ffc80 */
L8c5ffb6e: *stop=0x8c5ffb6eu;r[0]=80u;goto L8c5ffb70; /* mov #0x50,r0 */
L8c5ffb70: *stop=0x8c5ffb70u;r[2]=5u;goto L8c5ffb72; /* mov #0x5,r2 */
L8c5ffb72: *stop=0x8c5ffb72u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[2],2);if(io_status)return io_status;goto L8c5ffb74; /* mov.w r2,@(r0,r14) */
L8c5ffb74: *stop=0x8c5ffb74u;r[0]=82u;goto L8c5ffb76; /* mov #0x52,r0 */
L8c5ffb76: *stop=0x8c5ffb76u;r[3]=4294967268u;goto L8c5ffb78; /* mov #-0x1c,r3 */
L8c5ffb78: *stop=0x8c5ffb78u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[3],2);if(io_status)return io_status;goto L8c5ffb7a; /* mov.w r3,@(r0,r14) */
L8c5ffb7a: *stop=0x8c5ffb7au;r[0]=68u;goto L8c5ffb7c; /* mov #0x44,r0 */
L8c5ffb7c: *stop=0x8c5ffb7cu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[12],4);if(io_status)return io_status;goto L8c5ffb7e; /* mov.l r12,@(r0,r14) */
L8c5ffb7e: *stop=0x8c5ffb7eu;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffb80; /* mov.l @(0x4,r14),r3 */
L8c5ffb80: *stop=0x8c5ffb82u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[12],4);if(io_status)return io_status;goto L8c5ffc34; /* bra 0x8c5ffc34 */
L8c5ffb84: *stop=0x8c5ffb84u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffb86; /* mov.l @r13,r2 */
L8c5ffb86: *stop=0x8c5ffb86u;r[5]=r[15];goto L8c5ffb88; /* mov r15,r5 */
L8c5ffb88: *stop=0x8c5ffb88u;if(!valid_access(r[2]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffb8a; /* mov.l @(0xc,r2),r3 */
L8c5ffb8a: *stop=0x8c5ffb8au;if(!valid_access(r[3]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x24u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c5ffb8c; /* mov.l @(0x24,r3),r1 */
L8c5ffb8c: target=r[1];*pr=0x8c5ffb90u;*stop=0x8c5ffb8eu;if(!valid_access(r[14]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x3cu,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffb90; /* jsr @r1 */
L8c5ffb90: *stop=0x8c5ffb90u;r[4]=r[0];goto L8c5ffb92; /* mov r0,r4 */
L8c5ffb92: *stop=0x8c5ffb92u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ffb94; /* tst r4,r4 */
L8c5ffb94: if(!(*sr&1u))goto L8c5ffbac;goto L8c5ffb96; /* bf 0x8c5ffbac */
L8c5ffb96: *stop=0x8c5ffb96u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffb98; /* mov.l @r15,r2 */
L8c5ffb98: *stop=0x8c5ffb98u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c5ffb9a; /* cmp/pl r2 */
L8c5ffb9a: if(!(*sr&1u))goto L8c5ffba2;goto L8c5ffb9c; /* bf 0x8c5ffba2 */
L8c5ffb9c: *stop=0x8c5ffb9cu;r[0]=74u;goto L8c5ffb9e; /* mov #0x4a,r0 */
L8c5ffb9e: *stop=0x8c5ffba0u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[12],2);if(io_status)return io_status;goto L8c5ffba6; /* bra 0x8c5ffba6 */
L8c5ffba2: *stop=0x8c5ffba2u;r[0]=74u;goto L8c5ffba4; /* mov #0x4a,r0 */
L8c5ffba4: *stop=0x8c5ffba4u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[9],2);if(io_status)return io_status;goto L8c5ffba6; /* mov.w r9,@(r0,r14) */
L8c5ffba6: *stop=0x8c5ffba6u;if(!valid_access(r[14]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffba8; /* mov.l @(0x20,r14),r3 */
L8c5ffba8: *stop=0x8c5ffbaau;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[3],4);if(io_status)return io_status;goto L8c5ffc80; /* bra 0x8c5ffc80 */
L8c5ffbac: *stop=0x8c5ffbacu;r[0]=r[4];goto L8c5ffbae; /* mov r4,r0 */
L8c5ffbae: *stop=0x8c5ffbaeu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c5ffbb0; /* cmp/eq #0x1,r0 */
L8c5ffbb0: if(!(*sr&1u))goto L8c5ffc80;goto L8c5ffbb2; /* bf 0x8c5ffc80 */
L8c5ffbb2: *stop=0x8c5ffbb2u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffbb4; /* mov.l @r15,r2 */
L8c5ffbb4: *stop=0x8c5ffbb6u;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[2],4);if(io_status)return io_status;goto L8c5ffc80; /* bra 0x8c5ffc80 */
L8c5ffbb8: *stop=0x8c5ffbb8u;if(!valid_access(r[10]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c5ffbba; /* mov.l @(0x8,r10),r1 */
L8c5ffbba: *stop=0x8c5ffbbau;r[0]=64u;goto L8c5ffbbc; /* mov #0x40,r0 */
L8c5ffbbc: *stop=0x8c5ffbbcu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[1],4);if(io_status)return io_status;goto L8c5ffbbe; /* mov.l r1,@(r0,r14) */
L8c5ffbbe: *stop=0x8c5ffbbeu;r[0]=72u;goto L8c5ffbc0; /* mov #0x48,r0 */
L8c5ffbc0: *stop=0x8c5ffbc0u;if(!valid_access(r[0]+r[14],2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c5ffbc2; /* mov.w @(r0,r14),r0 */
L8c5ffbc2: *stop=0x8c5ffbc2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c5ffbc4; /* cmp/eq #0x1,r0 */
L8c5ffbc4: if(!(*sr&1u))goto L8c5ffc80;goto L8c5ffbc6; /* bf 0x8c5ffc80 */
L8c5ffbc6: *stop=0x8c5ffbc6u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffbc8; /* mov.l @r10,r2 */
L8c5ffbc8: *stop=0x8c5ffbcau;if(!valid_access(r[14]+24u,4u))return -3;wr(m,bus,&io_status,r[14]+0x18u,r[2],4);if(io_status)return io_status;goto L8c5ffc80; /* bra 0x8c5ffc80 */
L8c5ffbcc: *stop=0x8c5ffbccu;r[0]=80u;goto L8c5ffbce; /* mov #0x50,r0 */
L8c5ffbce: *stop=0x8c5ffbceu;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[11],2);if(io_status)return io_status;goto L8c5ffbd0; /* mov.w r11,@(r0,r14) */
L8c5ffbd0: *stop=0x8c5ffbd0u;r[0]=82u;goto L8c5ffbd2; /* mov #0x52,r0 */
L8c5ffbd2: *stop=0x8c5ffbd2u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffbd4; /* mov.l @r10,r3 */
L8c5ffbd4: *stop=0x8c5ffbd4u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[3],2);if(io_status)return io_status;goto L8c5ffbd6; /* mov.w r3,@(r0,r14) */
L8c5ffbd6: *stop=0x8c5ffbd6u;r[0]=68u;goto L8c5ffbd8; /* mov #0x44,r0 */
L8c5ffbd8: *stop=0x8c5ffbd8u;if(!valid_access(r[10]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffbda; /* mov.l @(0x4,r10),r2 */
L8c5ffbda: *stop=0x8c5ffbdau;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[2],4);if(io_status)return io_status;goto L8c5ffbdc; /* mov.l r2,@(r0,r14) */
L8c5ffbdc: *stop=0x8c5ffbdcu;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[12],4);if(io_status)return io_status;goto L8c5ffbde; /* mov.l r12,@(0x3c,r14) */
L8c5ffbde: *stop=0x8c5ffbdeu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffbe0; /* mov.l @r13,r3 */
L8c5ffbe0: *stop=0x8c5ffbe0u;if(!valid_access(r[3]+28u,4u))return -3;wr(m,bus,&io_status,r[3]+0x1cu,r[12],4);if(io_status)return io_status;goto L8c5ffbe2; /* mov.l r12,@(0x1c,r3) */
L8c5ffbe2: *stop=0x8c5ffbe2u;r[0]=82u;goto L8c5ffbe4; /* mov #0x52,r0 */
L8c5ffbe4: *stop=0x8c5ffbe4u;if(!valid_access(r[0]+r[14],2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c5ffbe6; /* mov.w @(r0,r14),r0 */
L8c5ffbe6: *stop=0x8c5ffbe6u;*sr=(*sr&~1u)|(r[0]==0xffffffe3u);goto L8c5ffbe8; /* cmp/eq #-0x1d,r0 */
L8c5ffbe8: if(!(*sr&1u))goto L8c5ffc34;goto L8c5ffbea; /* bf 0x8c5ffc34 */
L8c5ffbea: *stop=0x8c5ffbeau;r[0]=68u;goto L8c5ffbec; /* mov #0x44,r0 */
L8c5ffbec: *stop=0x8c5ffbecu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c5ffbee; /* mov.l @(r0,r14),r0 */
L8c5ffbee: *stop=0x8c5ffbeeu;*sr=(*sr&~1u)|(r[0]==0x0000003au);goto L8c5ffbf0; /* cmp/eq #0x3a,r0 */
L8c5ffbf0: if(!(*sr&1u))goto L8c5ffc24;goto L8c5ffbf2; /* bf 0x8c5ffc24 */
L8c5ffbf2: target=0x8c600688u;*pr=0x8c5ffbf6u;*stop=0x8c5ffbf4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffbf6; /* bsr 0x8c600688 */
L8c5ffbf6: *stop=0x8c5ffbf6u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c5ffbf8; /* cmp/eq #0x6,r0 */
L8c5ffbf8: take=(*sr&1u);*stop=0x8c5ffbfau;r[4]=r[0];if(take)goto L8c5ffc00;goto L8c5ffbfc; /* bt/s 0x8c5ffc00 */
L8c5ffbfc: *stop=0x8c5ffbfcu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c5ffbfe; /* tst r4,r4 */
L8c5ffbfe: if(!(*sr&1u))goto L8c5ffc34;goto L8c5ffc00; /* bf 0x8c5ffc34 */
L8c5ffc00: *stop=0x8c5ffc00u;r[0]=82u;goto L8c5ffc02; /* mov #0x52,r0 */
L8c5ffc02: *stop=0x8c5ffc02u;r[3]=4294967273u;goto L8c5ffc04; /* mov #-0x17,r3 */
L8c5ffc04: *stop=0x8c5ffc06u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[3],2);if(io_status)return io_status;goto L8c5ffc34; /* bra 0x8c5ffc34 */
L8c5ffc24: *stop=0x8c5ffc24u;r[0]=68u;goto L8c5ffc26; /* mov #0x44,r0 */
L8c5ffc26: *stop=0x8c5ffc26u;r[1]=sx(m,bus,&io_status,0x8c5ffd14u,2);if(io_status)return io_status;goto L8c5ffc28; /* mov.w 0x8c5ffd14,r1 */
L8c5ffc28: *stop=0x8c5ffc28u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffc2a; /* mov.l @(r0,r14),r2 */
L8c5ffc2a: *stop=0x8c5ffc2au;*sr=(*sr&~1u)|(r[1]==r[2]);goto L8c5ffc2c; /* cmp/eq r1,r2 */
L8c5ffc2c: if(!(*sr&1u))goto L8c5ffc34;goto L8c5ffc2e; /* bf 0x8c5ffc34 */
L8c5ffc2e: *stop=0x8c5ffc2eu;r[0]=82u;goto L8c5ffc30; /* mov #0x52,r0 */
L8c5ffc30: *stop=0x8c5ffc30u;r[3]=4294967259u;goto L8c5ffc32; /* mov #-0x25,r3 */
L8c5ffc32: *stop=0x8c5ffc32u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[3],2);if(io_status)return io_status;goto L8c5ffc34; /* mov.w r3,@(r0,r14) */
L8c5ffc34: *stop=0x8c5ffc34u;r[0]=82u;goto L8c5ffc36; /* mov #0x52,r0 */
L8c5ffc36: *stop=0x8c5ffc36u;if(!valid_access(r[0]+r[14],2u))return -3;r[0]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c5ffc38; /* mov.w @(r0,r14),r0 */
L8c5ffc38: *stop=0x8c5ffc38u;*sr=(*sr&~1u)|(r[0]==0xffffffe9u);goto L8c5ffc3a; /* cmp/eq #-0x17,r0 */
L8c5ffc3a: take=(*sr&1u);*stop=0x8c5ffc3cu;r[4]=r[0];if(take)goto L8c5ffc44;goto L8c5ffc3e; /* bt/s 0x8c5ffc44 */
L8c5ffc3e: *stop=0x8c5ffc3eu;r[0]=r[4];goto L8c5ffc40; /* mov r4,r0 */
L8c5ffc40: *stop=0x8c5ffc40u;*sr=(*sr&~1u)|(r[0]==0xffffffdfu);goto L8c5ffc42; /* cmp/eq #-0x21,r0 */
L8c5ffc42: if(!(*sr&1u))goto L8c5ffc4a;goto L8c5ffc44; /* bf 0x8c5ffc4a */
L8c5ffc44: *stop=0x8c5ffc44u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffc46; /* mov.l @r13,r3 */
L8c5ffc46: *stop=0x8c5ffc46u;r[2]=1u;goto L8c5ffc48; /* mov #0x1,r2 */
L8c5ffc48: *stop=0x8c5ffc48u;if(!valid_access(r[3]+52u,4u))return -3;wr(m,bus,&io_status,r[3]+0x34u,r[2],4);if(io_status)return io_status;goto L8c5ffc4a; /* mov.l r2,@(0x34,r3) */
L8c5ffc4a: *stop=0x8c5ffc4au;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffc4c; /* mov.l @r13,r3 */
L8c5ffc4c: *stop=0x8c5ffc4cu;if(!valid_access(r[3]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffc4e; /* mov.l @(0x24,r3),r2 */
L8c5ffc4e: *stop=0x8c5ffc4eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c5ffc50; /* tst r2,r2 */
L8c5ffc50: if((*sr&1u))goto L8c5ffc60;goto L8c5ffc52; /* bt 0x8c5ffc60 */
L8c5ffc52: *stop=0x8c5ffc52u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[2]=loaded;}goto L8c5ffc54; /* mov.l @r13,r2 */
L8c5ffc54: *stop=0x8c5ffc54u;r[0]=82u;goto L8c5ffc56; /* mov #0x52,r0 */
L8c5ffc56: *stop=0x8c5ffc56u;if(!valid_access(r[0]+r[14],2u))return -3;r[5]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c5ffc58; /* mov.w @(r0,r14),r5 */
L8c5ffc58: *stop=0x8c5ffc58u;if(!valid_access(r[2]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2]+0x24u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffc5a; /* mov.l @(0x24,r2),r3 */
L8c5ffc5a: *stop=0x8c5ffc5au;r[4]=r[2];goto L8c5ffc5c; /* mov r2,r4 */
L8c5ffc5c: target=r[3];*pr=0x8c5ffc60u;*stop=0x8c5ffc5eu;if(!valid_access(r[4]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x28u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffc60; /* jsr @r3 */
L8c5ffc60: *stop=0x8c5ffc60u;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffc62; /* mov.l @(0x34,r14),r3 */
L8c5ffc62: *stop=0x8c5ffc62u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c5ffc64; /* tst r3,r3 */
L8c5ffc64: if((*sr&1u))goto L8c5ffc80;goto L8c5ffc66; /* bt 0x8c5ffc80 */
L8c5ffc66: *stop=0x8c5ffc66u;r[0]=82u;goto L8c5ffc68; /* mov #0x52,r0 */
L8c5ffc68: *stop=0x8c5ffc68u;if(!valid_access(r[14]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x34u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffc6a; /* mov.l @(0x34,r14),r3 */
L8c5ffc6a: *stop=0x8c5ffc6au;if(!valid_access(r[0]+r[14],2u))return -3;r[5]=sx(m,bus,&io_status,r[0]+r[14],2);if(io_status)return io_status;goto L8c5ffc6c; /* mov.w @(r0,r14),r5 */
L8c5ffc6c: target=r[3];*pr=0x8c5ffc70u;*stop=0x8c5ffc6eu;if(!valid_access(r[14]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x38u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c5ffc70; /* jsr @r3 */
L8c5ffc70: *stop=0x8c5ffc72u;goto L8c5ffc80; /* bra 0x8c5ffc80 */
L8c5ffc74: *stop=0x8c5ffc74u;r[0]=80u;goto L8c5ffc76; /* mov #0x50,r0 */
L8c5ffc76: *stop=0x8c5ffc76u;r[2]=r[11]&255u;goto L8c5ffc78; /* extu.b r11,r2 */
L8c5ffc78: *stop=0x8c5ffc78u;if(!valid_access(r[0]+r[14],2u))return -3;wr(m,bus,&io_status,r[0]+r[14],r[2],2);if(io_status)return io_status;goto L8c5ffc7a; /* mov.w r2,@(r0,r14) */
L8c5ffc7a: *stop=0x8c5ffc7au;if(!valid_access(r[14]+60u,4u))return -3;wr(m,bus,&io_status,r[14]+0x3cu,r[12],4);if(io_status)return io_status;goto L8c5ffc7c; /* mov.l r12,@(0x3c,r14) */
L8c5ffc7c: *stop=0x8c5ffc7cu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffc7e; /* mov.l @r13,r3 */
L8c5ffc7e: *stop=0x8c5ffc7eu;if(!valid_access(r[3]+28u,4u))return -3;wr(m,bus,&io_status,r[3]+0x1cu,r[12],4);if(io_status)return io_status;goto L8c5ffc80; /* mov.l r12,@(0x1c,r3) */
L8c5ffc80: *stop=0x8c5ffc80u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[3]=loaded;}goto L8c5ffc82; /* mov.l @r13,r3 */
L8c5ffc82: *stop=0x8c5ffc82u;if(!valid_access(r[3]+32u,4u))return -3;wr(m,bus,&io_status,r[3]+0x20u,r[12],4);if(io_status)return io_status;goto L8c5ffc84; /* mov.l r12,@(0x20,r3) */
L8c5ffc84: *stop=0x8c5ffc84u;r[0]=r[11];goto L8c5ffc86; /* mov r11,r0 */
L8c5ffc86: *stop=0x8c5ffc86u;r[15]+=40u;goto L8c5ffc88; /* add #0x28,r15 */
L8c5ffc88: *stop=0x8c5ffc88u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5ffc8a; /* lds.l @r15+,PR */
L8c5ffc8a: *stop=0x8c5ffc8au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c5ffc8c; /* mov.l @r15+,r9 */
L8c5ffc8c: *stop=0x8c5ffc8cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c5ffc8e; /* mov.l @r15+,r10 */
L8c5ffc8e: *stop=0x8c5ffc8eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c5ffc90; /* mov.l @r15+,r11 */
L8c5ffc90: *stop=0x8c5ffc90u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c5ffc92; /* mov.l @r15+,r12 */
L8c5ffc92: *stop=0x8c5ffc92u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c5ffc94; /* mov.l @r15+,r13 */
L8c5ffc94: *stop=0x8c5ffc94u;if(*pr!=entry_pr)return -3;*stop=0x8c5ffc96u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
L8c6005c2: *stop=0x8c6005c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6006b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6005c4; /* mov.l 0x8c6006b4,r2 */
L8c6005c4: *stop=0x8c6005c4u;r[0]=64u;goto L8c6005c6; /* mov #0x40,r0 */
L8c6005c6: *stop=0x8c6005c6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6005c8; /* sts.l PR,@-r15 */
L8c6005c8: *stop=0x8c6005c8u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6005ca; /* mov.l @r2,r3 */
L8c6005ca: *stop=0x8c6005cau;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6005cc; /* mov.l @(0xc,r3),r1 */
L8c6005cc: *stop=0x8c6005ccu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6005ce; /* mov.l @(r0,r1),r0 */
L8c6005ce: target=r[0];*pr=0x8c6005d2u;*stop=0x8c6005d0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6005d2; /* jsr @r0 */
L8c6005d2: *stop=0x8c6005d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6006b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6005d4; /* mov.l 0x8c6006b4,r2 */
L8c6005d4: *stop=0x8c6005d4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6005d6; /* mov.l @r2,r3 */
L8c6005d6: *stop=0x8c6005d6u;if(!valid_access(r[3]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x1cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6005d8; /* mov.l @(0x1c,r3),r1 */
L8c6005d8: *stop=0x8c6005d8u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c6005da; /* tst r1,r1 */
L8c6005da: if((*sr&1u))goto L8c6005e0;goto L8c6005dc; /* bt 0x8c6005e0 */
L8c6005dc: *stop=0x8c6005deu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c5ffa12; /* bra 0x8c5ffa12 */
L8c6005e0: *stop=0x8c6005e0u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6005e2; /* lds.l @r15+,PR */
L8c6005e2: *stop=0x8c6005e2u;if(*pr!=entry_pr)return -3;*stop=0x8c6005e4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_file_8c645348(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 int io_status=0;(void)io_status;(void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c645348;
L8c645348: *stop=0x8c645348u;r[3]=97u;goto L8c64534a; /* mov #0x61,r3 */
L8c64534a: *stop=0x8c64534au;r[5]=(uint32_t)(int32_t)(int8_t)r[4];goto L8c64534c; /* exts.b r4,r5 */
L8c64534c: *stop=0x8c64534cu;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[3]);goto L8c64534e; /* cmp/ge r3,r5 */
L8c64534e: if(!(*sr&1u))goto L8c645358;goto L8c645350; /* bf 0x8c645358 */
L8c645350: *stop=0x8c645350u;r[1]=122u;goto L8c645352; /* mov #0x7a,r1 */
L8c645352: *stop=0x8c645352u;*sr=(*sr&~1u)|((int32_t)r[5]>(int32_t)r[1]);goto L8c645354; /* cmp/gt r1,r5 */
L8c645354: if((*sr&1u))goto L8c645358;goto L8c645356; /* bt 0x8c645358 */
L8c645356: *stop=0x8c645356u;r[4]+=4294967264u;goto L8c645358; /* add #-0x20,r4 */
L8c645358: *stop=0x8c645358u;if(*pr!=entry_pr)return -3;*stop=0x8c64535au;r[0]=r[4];return 0; /* rts */
}
