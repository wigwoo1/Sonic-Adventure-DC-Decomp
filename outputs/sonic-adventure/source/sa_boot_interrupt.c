/* Geometry projection, sampling and view proposals; explicit GBR/queue state.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
#include "sh4_integer_state.h"
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
 if((a&0x1fffffffu)==0x005f6900u && n==4u)return 1;
 if((n==4u||n==8u) && !(a&3u) && (a&0x1fffffffu)>=0x05000000u && (a&0x1fffffffu)<=0x05800000u-n)return 1;
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

SA_STAGE_EXPORT int sa_boot_interrupt_8c651bc0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c651bc0;
L8c651bc0: *stop=0x8c651bc0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c651bc2; /* mov.l r14,@-r15 */
L8c651bc2: *stop=0x8c651bc2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c651bc4; /* mov.l r13,@-r15 */
L8c651bc4: *stop=0x8c651bc4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c651bc6; /* mov.l r12,@-r15 */
L8c651bc6: *stop=0x8c651bc6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c651bc8; /* mov.l r11,@-r15 */
L8c651bc8: *stop=0x8c651bc8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c651bca; /* mov.l r10,@-r15 */
L8c651bca: *stop=0x8c651bcau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c651bcc; /* mov.l r9,@-r15 */
L8c651bcc: *stop=0x8c651bccu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c651bce; /* mov.l r8,@-r15 */
L8c651bce: *stop=0x8c651bceu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c651bd0; /* sts.l PR,@-r15 */
L8c651bd0: *stop=0x8c651bd0u;r[15]+=4294967276u;goto L8c651bd2; /* add #-0x14,r15 */
L8c651bd2: *stop=0x8c651bd2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651c64u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651bd4; /* mov.l 0x8c651c64,r2 */
L8c651bd4: *stop=0x8c651bd4u;r[3]=8u;goto L8c651bd6; /* mov #0x8,r3 */
L8c651bd6: *stop=0x8c651bd6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c651bd8; /* mov.l r3,@r2 */
L8c651bd8: *stop=0x8c651bd8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651c68u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651bda; /* mov.l 0x8c651c68,r3 */
L8c651bda: *stop=0x8c651bdau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651bdc; /* mov.l @r3,r0 */
L8c651bdc: *stop=0x8c651bdcu;r[0]|=0x2u;goto L8c651bde; /* or #0x2,r0 */
L8c651bde: *stop=0x8c651bdeu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c651be0; /* mov.l r0,@r3 */
L8c651be0: *stop=0x8c651be0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651c6cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651be2; /* mov.l 0x8c651c6c,r1 */
L8c651be2: *stop=0x8c651be2u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651be4; /* mov.l @r1,r3 */
L8c651be4: *stop=0x8c651be4u;r[3]+=1u;goto L8c651be6; /* add #0x1,r3 */
L8c651be6: *stop=0x8c651be6u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c651be8; /* mov.l r3,@r1 */
L8c651be8: *stop=0x8c651be8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651c74u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651bea; /* mov.l 0x8c651c74,r1 */
L8c651bea: *stop=0x8c651beau;{uint32_t loaded=rd(m,bus,&io_status,0x8c651c70u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c651bec; /* mov.l 0x8c651c70,r5 */
L8c651bec: *stop=0x8c651becu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651bee; /* mov.l @r1,r0 */
L8c651bee: *stop=0x8c651beeu;*sr=(*sr&~1u)|!(r[0]&0x2u);goto L8c651bf0; /* tst #0x2,r0 */
L8c651bf0: if((*sr&1u))goto L8c651c00;goto L8c651bf2; /* bt 0x8c651c00 */
L8c651bf2: *stop=0x8c651bf2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651c78u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651bf4; /* mov.l 0x8c651c78,r2 */
L8c651bf4: *stop=0x8c651bf4u;r[3]=sx(m,bus,&io_status,0x8c651c60u,2);if(io_status)return io_status;goto L8c651bf6; /* mov.w 0x8c651c60,r3 */
L8c651bf6: *stop=0x8c651bf6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651bf8; /* mov.l @r2,r0 */
L8c651bf8: *stop=0x8c651bf8u;r[0]&=r[3];goto L8c651bfa; /* and r3,r0 */
L8c651bfa: *stop=0x8c651bfau;r[0]>>=8;goto L8c651bfc; /* shlr8 r0 */
L8c651bfc: *stop=0x8c651bfeu;r[0]>>=2;goto L8c651c0c; /* bra 0x8c651c0c */
L8c651c00: *stop=0x8c651c00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651c7cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c651c02; /* mov.l 0x8c651c7c,r4 */
L8c651c02: *stop=0x8c651c02u;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651c04; /* mov.l @r4,r0 */
L8c651c04: *stop=0x8c651c04u;r[0]+=1u;goto L8c651c06; /* add #0x1,r0 */
L8c651c06: *stop=0x8c651c06u;r[0]&=0x1u;goto L8c651c08; /* and #0x1,r0 */
L8c651c08: *stop=0x8c651c08u;r[3]=r[0];goto L8c651c0a; /* mov r0,r3 */
L8c651c0a: *stop=0x8c651c0au;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c651c0c; /* mov.l r0,@r4 */
L8c651c0c: *stop=0x8c651c0cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651c84u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651c0e; /* mov.l 0x8c651c84,r2 */
L8c651c0e: *stop=0x8c651c0eu;r[12]=1u;goto L8c651c10; /* mov #0x1,r12 */
L8c651c10: *stop=0x8c651c10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651c80u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c651c12; /* mov.l 0x8c651c80,r9 */
L8c651c12: *stop=0x8c651c12u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[0],4);if(io_status)return io_status;goto L8c651c14; /* mov.l r0,@r5 */
L8c651c14: *stop=0x8c651c14u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651c16; /* mov.l @r2,r0 */
L8c651c16: *stop=0x8c651c16u;r[0]&=0x4u;goto L8c651c18; /* and #0x4,r0 */
L8c651c18: *stop=0x8c651c18u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c651c1a; /* cmp/eq #0x4,r0 */
L8c651c1a: take=!(*sr&1u);*stop=0x8c651c1cu;r[14]=0u;if(take)goto L8c651c2a;goto L8c651c1e; /* bf/s 0x8c651c2a */
L8c651c1e: *stop=0x8c651c1eu;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651c20; /* mov.l @r5,r0 */
L8c651c20: *stop=0x8c651c20u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c651c22; /* cmp/eq #0x1,r0 */
L8c651c22: if(!(*sr&1u))goto L8c651c2a;goto L8c651c24; /* bf 0x8c651c2a */
L8c651c24: *stop=0x8c651c24u;r[5]=r[12];goto L8c651c26; /* mov r12,r5 */
L8c651c26: *stop=0x8c651c28u;r[4]=r[12];goto L8c651cb0; /* bra 0x8c651cb0 */
L8c651c2a: *stop=0x8c651c2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651c88u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651c2c; /* mov.l 0x8c651c88,r2 */
L8c651c2c: *stop=0x8c651c2cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651c2e; /* mov.l @r2,r3 */
L8c651c2e: *stop=0x8c651c2eu;r[3]+=4294967295u;goto L8c651c30; /* add #-0x1,r3 */
L8c651c30: *stop=0x8c651c30u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c651c32; /* mov.l r3,@r2 */
L8c651c32: *stop=0x8c651c32u;r[1]=r[2];goto L8c651c34; /* mov r2,r1 */
L8c651c34: *stop=0x8c651c34u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651c36; /* mov.l @r1,r2 */
L8c651c36: *stop=0x8c651c36u;*sr=(*sr&~1u)|((int32_t)r[2]>=0);goto L8c651c38; /* cmp/pz r2 */
L8c651c38: if(!(*sr&1u))goto L8c651ca4;goto L8c651c3a; /* bf 0x8c651ca4 */
L8c651c3a: *stop=0x8c651c3au;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651c3c; /* mov.l @r9,r0 */
L8c651c3c: *stop=0x8c651c3cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c651c3e; /* tst r0,r0 */
L8c651c3e: if((*sr&1u))goto L8c651ca0;goto L8c651c40; /* bt 0x8c651ca0 */
L8c651c40: *stop=0x8c651c40u;r[5]=r[14];goto L8c651c42; /* mov r14,r5 */
L8c651c42: *stop=0x8c651c44u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[14],4);if(io_status)return io_status;goto L8c651cae; /* bra 0x8c651cae */
L8c651ca0: *stop=0x8c651ca2u;r[5]=r[12];goto L8c651cae; /* bra 0x8c651cae */
L8c651ca4: *stop=0x8c651ca4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651da0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651ca6; /* mov.l 0x8c651da0,r2 */
L8c651ca6: *stop=0x8c651ca6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651ca8; /* mov.l @r2,r3 */
L8c651ca8: *stop=0x8c651ca8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651da4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651caa; /* mov.l 0x8c651da4,r0 */
L8c651caa: *stop=0x8c651caau;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c651cac; /* mov.l r3,@r0 */
L8c651cac: *stop=0x8c651cacu;r[5]=r[14];goto L8c651cae; /* mov r14,r5 */
L8c651cae: *stop=0x8c651caeu;r[4]=r[5];goto L8c651cb0; /* mov r5,r4 */
L8c651cb0: *stop=0x8c651cb0u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c651cb2; /* tst r4,r4 */
L8c651cb2: if((*sr&1u))goto L8c651cb8;goto L8c651cb4; /* bt 0x8c651cb8 */
L8c651cb4: *stop=0x8c651cb6u;goto L8c651f08; /* bra 0x8c651f08 */
L8c651cb8: *stop=0x8c651cb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651da8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651cba; /* mov.l 0x8c651da8,r2 */
L8c651cba: *stop=0x8c651cbau;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[14],4);if(io_status)return io_status;goto L8c651cbc; /* mov.l r14,@(0x10,r15) */
L8c651cbc: *stop=0x8c651cbcu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651cbe; /* mov.l @r2,r3 */
L8c651cbe: *stop=0x8c651cbeu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c651cc0; /* tst r3,r3 */
L8c651cc0: if((*sr&1u))goto L8c651cc6;goto L8c651cc2; /* bt 0x8c651cc6 */
L8c651cc2: *stop=0x8c651cc4u;r[4]=r[12];goto L8c651eec; /* bra 0x8c651eec */
L8c651cc6: *stop=0x8c651cc6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651db0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651cc8; /* mov.l 0x8c651db0,r3 */
L8c651cc8: *stop=0x8c651cc8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dacu,4);if(io_status)return io_status;r[13]=loaded;}goto L8c651cca; /* mov.l 0x8c651dac,r13 */
L8c651cca: *stop=0x8c651ccau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651ccc; /* mov.l @r3,r2 */
L8c651ccc: *stop=0x8c651cccu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c651cce; /* tst r2,r2 */
L8c651cce: if((*sr&1u))goto L8c651d0a;goto L8c651cd0; /* bt 0x8c651d0a */
L8c651cd0: *stop=0x8c651cd0u;if(!valid_access(r[13]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x18u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651cd2; /* mov.l @(0x18,r13),r2 */
L8c651cd2: *stop=0x8c651cd2u;r[1]=16u;goto L8c651cd4; /* mov #0x10,r1 */
L8c651cd4: *stop=0x8c651cd4u;r[2]+=1u;goto L8c651cd6; /* add #0x1,r2 */
L8c651cd6: *stop=0x8c651cd6u;r[0]=r[2];goto L8c651cd8; /* mov r2,r0 */
L8c651cd8: *stop=0x8c651cd8u;*sr=(*sr&~1u)|(r[0]>r[1]);goto L8c651cda; /* cmp/hi r1,r0 */
L8c651cda: *stop=0x8c651cdau;if(!valid_access(r[13]+24u,4u))return -3;wr(m,bus,&io_status,r[13]+0x18u,r[2],4);if(io_status)return io_status;goto L8c651cdc; /* mov.l r2,@(0x18,r13) */
L8c651cdc: take=!(*sr&1u);*stop=0x8c651cdeu;r[5]=r[12];if(take)goto L8c651d06;goto L8c651ce0; /* bf/s 0x8c651d06 */
L8c651ce0: *stop=0x8c651ce0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651db4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651ce2; /* mov.l 0x8c651db4,r0 */
L8c651ce2: *stop=0x8c651ce2u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651ce4; /* mov.l @r0,r2 */
L8c651ce4: *stop=0x8c651ce4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651db8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651ce6; /* mov.l 0x8c651db8,r3 */
L8c651ce6: *stop=0x8c651ce6u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c651ce8; /* mov.l r2,@r3 */
L8c651ce8: *stop=0x8c651ce8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dbcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651cea; /* mov.l 0x8c651dbc,r2 */
L8c651cea: *stop=0x8c651ceau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c651cec; /* mov.l @r2,r1 */
L8c651cec: *stop=0x8c651cecu;r[3]+=4u;goto L8c651cee; /* add #0x4,r3 */
L8c651cee: *stop=0x8c651ceeu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c651cf0; /* mov.l r1,@r3 */
L8c651cf0: *stop=0x8c651cf0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dc0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651cf2; /* mov.l 0x8c651dc0,r1 */
L8c651cf2: *stop=0x8c651cf2u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651cf4; /* mov.l @r1,r0 */
L8c651cf4: *stop=0x8c651cf4u;r[3]+=28u;goto L8c651cf6; /* add #0x1c,r3 */
L8c651cf6: *stop=0x8c651cf6u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c651cf8; /* mov.l r0,@r3 */
L8c651cf8: *stop=0x8c651cf8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dc4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651cfa; /* mov.l 0x8c651dc4,r0 */
L8c651cfa: *stop=0x8c651cfau;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651cfc; /* mov.l @r0,r2 */
L8c651cfc: *stop=0x8c651cfcu;r[3]+=4u;goto L8c651cfe; /* add #0x4,r3 */
L8c651cfe: *stop=0x8c651cfeu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c651d00; /* mov.l r2,@r3 */
L8c651d00: *stop=0x8c651d00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dc8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651d02; /* mov.l 0x8c651dc8,r1 */
L8c651d02: *stop=0x8c651d02u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c651d04; /* mov.l r14,@r1 */
L8c651d04: *stop=0x8c651d04u;if(!valid_access(r[13]+24u,4u))return -3;wr(m,bus,&io_status,r[13]+0x18u,r[14],4);if(io_status)return io_status;goto L8c651d06; /* mov.l r14,@(0x18,r13) */
L8c651d06: *stop=0x8c651d08u;r[4]=r[12];goto L8c651eec; /* bra 0x8c651eec */
L8c651d0a: *stop=0x8c651d0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dccu,4);if(io_status)return io_status;r[8]=loaded;}goto L8c651d0c; /* mov.l 0x8c651dcc,r8 */
L8c651d0c: *stop=0x8c651d0cu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c651d0e; /* mov.l r14,@r15 */
L8c651d0e: *stop=0x8c651d0eu;r[11]=r[8];goto L8c651d10; /* mov r8,r11 */
L8c651d10: *stop=0x8c651d10u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[14],4);if(io_status)return io_status;goto L8c651d12; /* mov.l r14,@(0xc,r15) */
L8c651d12: *stop=0x8c651d12u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[14],4);if(io_status)return io_status;goto L8c651d14; /* mov.l r14,@(0x4,r15) */
L8c651d14: *stop=0x8c651d14u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[14],4);if(io_status)return io_status;goto L8c651d16; /* mov.l r14,@(0x8,r15) */
L8c651d16: *stop=0x8c651d18u;r[11]+=4294967236u;goto L8c651ed4; /* bra 0x8c651ed4 */
L8c651d1a: *stop=0x8c651d1au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}goto L8c651d1c; /* mov.l @r15,r10 */
L8c651d1c: *stop=0x8c651d1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dd0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651d1e; /* mov.l 0x8c651dd0,r0 */
L8c651d1e: *stop=0x8c651d1eu;r[10]<<=2;goto L8c651d20; /* shll2 r10 */
L8c651d20: *stop=0x8c651d20u;if(!valid_access(r[0]+r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651d22; /* mov.l @(r0,r10),r3 */
L8c651d22: *stop=0x8c651d22u;*sr=(*sr&~1u)|((int32_t)r[3]>=0);goto L8c651d24; /* cmp/pz r3 */
L8c651d24: if((*sr&1u))goto L8c651d2a;goto L8c651d26; /* bt 0x8c651d2a */
L8c651d26: *stop=0x8c651d28u;goto L8c651ebc; /* bra 0x8c651ebc */
L8c651d2a: *stop=0x8c651d2au;if(!valid_access(r[0]+r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651d2c; /* mov.l @(r0,r10),r3 */
L8c651d2c: *stop=0x8c651d2cu;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c651d2e; /* tst r12,r3 */
L8c651d2e: if((*sr&1u))goto L8c651d34;goto L8c651d30; /* bt 0x8c651d34 */
L8c651d30: *stop=0x8c651d32u;goto L8c651ebc; /* bra 0x8c651ebc */
L8c651d34: *stop=0x8c651d34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dd4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651d36; /* mov.l 0x8c651dd4,r2 */
L8c651d36: *stop=0x8c651d36u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c651d38; /* mov.l @r2,r1 */
L8c651d38: *stop=0x8c651d38u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c651d3a; /* tst r1,r1 */
L8c651d3a: if(!(*sr&1u))goto L8c651e00;goto L8c651d3c; /* bf 0x8c651e00 */
L8c651d3c: *stop=0x8c651d3cu;if(!valid_access(r[0]+r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651d3e; /* mov.l @(r0,r10),r0 */
L8c651d3e: *stop=0x8c651d3eu;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c651d40; /* cmp/eq #0x4,r0 */
L8c651d40: if(!(*sr&1u))goto L8c651d62;goto L8c651d42; /* bf 0x8c651d62 */
L8c651d42: *stop=0x8c651d42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dd0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651d44; /* mov.l 0x8c651dd0,r0 */
L8c651d44: *stop=0x8c651d44u;r[1]=5u;goto L8c651d46; /* mov #0x5,r1 */
L8c651d46: *stop=0x8c651d46u;if(!valid_access(r[0]+r[10],4u))return -3;wr(m,bus,&io_status,r[10]+r[0],r[1],4);if(io_status)return io_status;goto L8c651d48; /* mov.l r1,@(r0,r10) */
L8c651d48: *stop=0x8c651d48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dc8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651d4a; /* mov.l 0x8c651dc8,r1 */
L8c651d4a: *stop=0x8c651d4au;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651d4c; /* mov.l @r1,r3 */
L8c651d4c: *stop=0x8c651d4cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c651d4e; /* tst r3,r3 */
L8c651d4e: if((*sr&1u))goto L8c651d56;goto L8c651d50; /* bt 0x8c651d56 */
L8c651d50: *stop=0x8c651d50u;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651d52; /* mov.l @(0x8,r13),r3 */
L8c651d52: *stop=0x8c651d52u;r[3]+=1u;goto L8c651d54; /* add #0x1,r3 */
L8c651d54: *stop=0x8c651d54u;if(!valid_access(r[13]+8u,4u))return -3;wr(m,bus,&io_status,r[13]+0x8u,r[3],4);if(io_status)return io_status;goto L8c651d56; /* mov.l r3,@(0x8,r13) */
L8c651d56: *stop=0x8c651d56u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dc8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651d58; /* mov.l 0x8c651dc8,r2 */
L8c651d58: *stop=0x8c651d58u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c651d5a; /* mov.l r14,@r2 */
L8c651d5a: *stop=0x8c651d5au;r[3]=2u;goto L8c651d5c; /* mov #0x2,r3 */
L8c651d5c: *stop=0x8c651d5cu;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[3],4);if(io_status)return io_status;goto L8c651d5e; /* mov.l r3,@r11 */
L8c651d5e: *stop=0x8c651d60u;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[14],4);if(io_status)return io_status;goto L8c651ebc; /* bra 0x8c651ebc */
L8c651d62: *stop=0x8c651d62u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dd0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651d64; /* mov.l 0x8c651dd0,r0 */
L8c651d64: *stop=0x8c651d64u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[12],4);if(io_status)return io_status;goto L8c651d66; /* mov.l r12,@r9 */
L8c651d66: *stop=0x8c651d66u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[12],4);if(io_status)return io_status;goto L8c651d68; /* mov.l r12,@(0x10,r15) */
L8c651d68: *stop=0x8c651d68u;if(!valid_access(r[0]+r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651d6a; /* mov.l @(r0,r10),r0 */
L8c651d6a: *stop=0x8c651d6au;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c651d6c; /* cmp/eq #0x2,r0 */
L8c651d6c: if((*sr&1u))goto L8c651d72;goto L8c651d6e; /* bt 0x8c651d72 */
L8c651d6e: *stop=0x8c651d70u;goto L8c651ebc; /* bra 0x8c651ebc */
L8c651d72: *stop=0x8c651d72u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651d74; /* mov.l @(0x4,r13),r1 */
L8c651d74: *stop=0x8c651d74u;r[1]+=1u;goto L8c651d76; /* add #0x1,r1 */
L8c651d76: *stop=0x8c651d76u;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[1],4);if(io_status)return io_status;goto L8c651d78; /* mov.l r1,@(0x4,r13) */
L8c651d78: *stop=0x8c651d78u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dd8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651d7a; /* mov.l 0x8c651dd8,r1 */
L8c651d7a: *stop=0x8c651d7au;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651d7c; /* mov.l @(0x4,r13),r0 */
L8c651d7c: *stop=0x8c651d7cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651d7e; /* mov.l @r1,r3 */
L8c651d7e: *stop=0x8c651d7eu;*sr=(*sr&~1u)|(r[0]>r[3]);goto L8c651d80; /* cmp/hi r3,r0 */
L8c651d80: if((*sr&1u))goto L8c651d86;goto L8c651d82; /* bt 0x8c651d86 */
L8c651d82: *stop=0x8c651d84u;goto L8c651ebc; /* bra 0x8c651ebc */
L8c651d86: *stop=0x8c651d86u;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651d88; /* mov.l @(0xc,r13),r3 */
L8c651d88: *stop=0x8c651d88u;r[3]+=1u;goto L8c651d8a; /* add #0x1,r3 */
L8c651d8a: *stop=0x8c651d8au;if(!valid_access(r[13]+12u,4u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[3],4);if(io_status)return io_status;goto L8c651d8c; /* mov.l r3,@(0xc,r13) */
L8c651d8c: *stop=0x8c651d8cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651ddcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651d8e; /* mov.l 0x8c651ddc,r3 */
L8c651d8e: target=r[3];*pr=0x8c651d92u;*stop=0x8c651d90u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651d92; /* jsr @r3 */
L8c651d92: *stop=0x8c651d92u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651dd0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651d94; /* mov.l 0x8c651dd0,r0 */
L8c651d94: *stop=0x8c651d94u;r[2]=4294967295u;goto L8c651d96; /* mov #-0x1,r2 */
L8c651d96: *stop=0x8c651d96u;if(!valid_access(r[0]+r[10],4u))return -3;wr(m,bus,&io_status,r[10]+r[0],r[2],4);if(io_status)return io_status;goto L8c651d98; /* mov.l r2,@(r0,r10) */
L8c651d98: *stop=0x8c651d98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651de0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651d9a; /* mov.l 0x8c651de0,r3 */
L8c651d9a: *stop=0x8c651d9au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c651d9c; /* mov.l r14,@r3 */
L8c651d9c: *stop=0x8c651d9eu;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[14],4);if(io_status)return io_status;goto L8c651ebc; /* bra 0x8c651ebc */
L8c651e00: *stop=0x8c651e00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f60u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651e02; /* mov.l 0x8c651f60,r0 */
L8c651e02: *stop=0x8c651e02u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[12],4);if(io_status)return io_status;goto L8c651e04; /* mov.l r12,@r9 */
L8c651e04: *stop=0x8c651e04u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[12],4);if(io_status)return io_status;goto L8c651e06; /* mov.l r12,@(0x10,r15) */
L8c651e06: *stop=0x8c651e06u;if(!valid_access(r[0]+r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651e08; /* mov.l @(r0,r10),r0 */
L8c651e08: *stop=0x8c651e08u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c651e0a; /* cmp/eq #0x4,r0 */
L8c651e0a: if(!(*sr&1u))goto L8c651e8a;goto L8c651e0c; /* bf 0x8c651e8a */
L8c651e0c: *stop=0x8c651e0cu;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[1]=loaded;}goto L8c651e0e; /* mov.l @r13,r1 */
L8c651e0e: *stop=0x8c651e0eu;r[1]+=1u;goto L8c651e10; /* add #0x1,r1 */
L8c651e10: *stop=0x8c651e10u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[1],4);if(io_status)return io_status;goto L8c651e12; /* mov.l r1,@r13 */
L8c651e12: *stop=0x8c651e12u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f64u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651e14; /* mov.l 0x8c651f64,r1 */
L8c651e14: *stop=0x8c651e14u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651e16; /* mov.l @r13,r0 */
L8c651e16: *stop=0x8c651e16u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651e18; /* mov.l @r1,r3 */
L8c651e18: *stop=0x8c651e18u;*sr=(*sr&~1u)|(r[0]>r[3]);goto L8c651e1a; /* cmp/hi r3,r0 */
L8c651e1a: if(!(*sr&1u))goto L8c651e8a;goto L8c651e1c; /* bf 0x8c651e8a */
L8c651e1c: *stop=0x8c651e1cu;if(!valid_access(r[13]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651e1e; /* mov.l @(0x8,r13),r3 */
L8c651e1e: *stop=0x8c651e1eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f68u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651e20; /* mov.l 0x8c651f68,r2 */
L8c651e20: *stop=0x8c651e20u;r[3]+=1u;goto L8c651e22; /* add #0x1,r3 */
L8c651e22: *stop=0x8c651e22u;if(!valid_access(r[13]+8u,4u))return -3;wr(m,bus,&io_status,r[13]+0x8u,r[3],4);if(io_status)return io_status;goto L8c651e24; /* mov.l r3,@(0x8,r13) */
L8c651e24: *stop=0x8c651e24u;r[3]=4294967294u;goto L8c651e26; /* mov #-0x2,r3 */
L8c651e26: *stop=0x8c651e26u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651e28; /* mov.l @r2,r0 */
L8c651e28: *stop=0x8c651e28u;r[0]&=r[3];goto L8c651e2a; /* and r3,r0 */
L8c651e2a: *stop=0x8c651e2au;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[0],4);if(io_status)return io_status;goto L8c651e2c; /* mov.l r0,@r8 */
L8c651e2c: *stop=0x8c651e2cu;r[1]=6u;goto L8c651e2e; /* mov #0x6,r1 */
L8c651e2e: *stop=0x8c651e2eu;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[1],4);if(io_status)return io_status;goto L8c651e30; /* mov.l r1,@r11 */
L8c651e30: *stop=0x8c651e30u;if(!valid_access(r[11],4u))return -3;wr(m,bus,&io_status,r[11],r[14],4);if(io_status)return io_status;goto L8c651e32; /* mov.l r14,@r11 */
L8c651e32: *stop=0x8c651e32u;r[1]=r[2];goto L8c651e34; /* mov r2,r1 */
L8c651e34: *stop=0x8c651e34u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651e36; /* mov.l @r1,r2 */
L8c651e36: *stop=0x8c651e36u;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c651e38; /* mov.l r2,@r8 */
L8c651e38: *stop=0x8c651e38u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651e3a; /* mov.l 0x8c651f6c,r2 */
L8c651e3a: *stop=0x8c651e3au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651e3c; /* mov.l @r2,r0 */
L8c651e3c: *stop=0x8c651e3cu;r[0]=~r[0];goto L8c651e3e; /* not r0,r0 */
L8c651e3e: *stop=0x8c651e3eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f70u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651e40; /* mov.l 0x8c651f70,r3 */
L8c651e40: *stop=0x8c651e40u;r[0]&=r[12];goto L8c651e42; /* and r12,r0 */
L8c651e42: *stop=0x8c651e42u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[0],4);if(io_status)return io_status;goto L8c651e44; /* mov.l r0,@-r15 */
L8c651e44: *stop=0x8c651e44u;r[6]=0u;goto L8c651e46; /* mov #0x0,r6 */
L8c651e46: *stop=0x8c651e46u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c651e48; /* mov.l r14,@-r15 */
L8c651e48: *stop=0x8c651e48u;r[7]=1u;goto L8c651e4a; /* mov #0x1,r7 */
L8c651e4a: *stop=0x8c651e4au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c651e4c; /* mov.l @r3,r5 */
L8c651e4c: *stop=0x8c651e4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f78u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651e4e; /* mov.l 0x8c651f78,r3 */
L8c651e4e: *stop=0x8c651e4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f74u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651e50; /* mov.l 0x8c651f74,r1 */
L8c651e50: target=r[3];*pr=0x8c651e54u;*stop=0x8c651e52u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651e54; /* jsr @r3 */
L8c651e54: *stop=0x8c651e54u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f6cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651e56; /* mov.l 0x8c651f6c,r3 */
L8c651e56: *stop=0x8c651e56u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651e58; /* mov.l @r3,r2 */
L8c651e58: *stop=0x8c651e58u;r[2]=~r[2];goto L8c651e5a; /* not r2,r2 */
L8c651e5a: *stop=0x8c651e5au;r[2]&=r[12];goto L8c651e5c; /* and r12,r2 */
L8c651e5c: *stop=0x8c651e5cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c651e5e; /* mov.l r2,@-r15 */
L8c651e5e: *stop=0x8c651e5eu;r[1]=1u;goto L8c651e60; /* mov #0x1,r1 */
L8c651e60: *stop=0x8c651e60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f70u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651e62; /* mov.l 0x8c651f70,r2 */
L8c651e62: *stop=0x8c651e62u;r[6]=0u;goto L8c651e64; /* mov #0x0,r6 */
L8c651e64: *stop=0x8c651e64u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c651e66; /* mov.l r1,@-r15 */
L8c651e66: *stop=0x8c651e66u;r[7]=r[1];goto L8c651e68; /* mov r1,r7 */
L8c651e68: *stop=0x8c651e68u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[5]=loaded;}goto L8c651e6a; /* mov.l @r2,r5 */
L8c651e6a: *stop=0x8c651e6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f78u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651e6c; /* mov.l 0x8c651f78,r2 */
L8c651e6c: *stop=0x8c651e6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f74u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651e6e; /* mov.l 0x8c651f74,r0 */
L8c651e6e: target=r[2];*pr=0x8c651e72u;*stop=0x8c651e70u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651e72; /* jsr @r2 */
L8c651e72: *stop=0x8c651e72u;r[15]+=16u;goto L8c651e74; /* add #0x10,r15 */
L8c651e74: *stop=0x8c651e74u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f60u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651e76; /* mov.l 0x8c651f60,r0 */
L8c651e76: *stop=0x8c651e76u;r[2]=5u;goto L8c651e78; /* mov #0x5,r2 */
L8c651e78: *stop=0x8c651e78u;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651e7a; /* mov.l @(0x8,r15),r3 */
L8c651e7a: *stop=0x8c651e7au;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[2],4);if(io_status)return io_status;goto L8c651e7c; /* mov.l r2,@(r0,r3) */
L8c651e7c: *stop=0x8c651e7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f7cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651e7e; /* mov.l 0x8c651f7c,r3 */
L8c651e7e: *stop=0x8c651e7eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c651e80; /* mov.l r14,@r3 */
L8c651e80: *stop=0x8c651e80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f80u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651e82; /* mov.l 0x8c651f80,r2 */
L8c651e82: *stop=0x8c651e82u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c651e84; /* mov.l @r2,r1 */
L8c651e84: *stop=0x8c651e84u;r[1]+=1u;goto L8c651e86; /* add #0x1,r1 */
L8c651e86: *stop=0x8c651e86u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c651e88; /* mov.l r1,@r2 */
L8c651e88: *stop=0x8c651e88u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[14],4);if(io_status)return io_status;goto L8c651e8a; /* mov.l r14,@r13 */
L8c651e8a: *stop=0x8c651e8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f60u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651e8c; /* mov.l 0x8c651f60,r0 */
L8c651e8c: *stop=0x8c651e8cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651e8e; /* mov.l @(0x4,r15),r3 */
L8c651e8e: *stop=0x8c651e8eu;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651e90; /* mov.l @(r0,r3),r0 */
L8c651e90: *stop=0x8c651e90u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c651e92; /* cmp/eq #0x2,r0 */
L8c651e92: if(!(*sr&1u))goto L8c651ebc;goto L8c651e94; /* bf 0x8c651ebc */
L8c651e94: *stop=0x8c651e94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f84u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651e96; /* mov.l 0x8c651f84,r0 */
L8c651e96: *stop=0x8c651e96u;if(!valid_access(r[13]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651e98; /* mov.l @(0x4,r13),r3 */
L8c651e98: *stop=0x8c651e98u;r[3]+=1u;goto L8c651e9a; /* add #0x1,r3 */
L8c651e9a: *stop=0x8c651e9au;r[1]=r[3];goto L8c651e9c; /* mov r3,r1 */
L8c651e9c: *stop=0x8c651e9cu;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[3],4);if(io_status)return io_status;goto L8c651e9e; /* mov.l r3,@(0x4,r13) */
L8c651e9e: *stop=0x8c651e9eu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651ea0; /* mov.l @r0,r2 */
L8c651ea0: *stop=0x8c651ea0u;*sr=(*sr&~1u)|(r[1]>r[2]);goto L8c651ea2; /* cmp/hi r2,r1 */
L8c651ea2: if(!(*sr&1u))goto L8c651ebc;goto L8c651ea4; /* bf 0x8c651ebc */
L8c651ea4: *stop=0x8c651ea4u;if(!valid_access(r[13]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651ea6; /* mov.l @(0xc,r13),r3 */
L8c651ea6: *stop=0x8c651ea6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f88u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651ea8; /* mov.l 0x8c651f88,r2 */
L8c651ea8: *stop=0x8c651ea8u;r[3]+=1u;goto L8c651eaa; /* add #0x1,r3 */
L8c651eaa: target=r[2];*pr=0x8c651eaeu;*stop=0x8c651eacu;if(!valid_access(r[13]+12u,4u))return -3;wr(m,bus,&io_status,r[13]+0xcu,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651eae; /* jsr @r2 */
L8c651eae: *stop=0x8c651eaeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f60u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651eb0; /* mov.l 0x8c651f60,r0 */
L8c651eb0: *stop=0x8c651eb0u;r[3]=4294967295u;goto L8c651eb2; /* mov #-0x1,r3 */
L8c651eb2: *stop=0x8c651eb2u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651eb4; /* mov.l @(0xc,r15),r1 */
L8c651eb4: *stop=0x8c651eb4u;if(!valid_access(r[0]+r[1],4u))return -3;wr(m,bus,&io_status,r[1]+r[0],r[3],4);if(io_status)return io_status;goto L8c651eb6; /* mov.l r3,@(r0,r1) */
L8c651eb6: *stop=0x8c651eb6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f8cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651eb8; /* mov.l 0x8c651f8c,r2 */
L8c651eb8: *stop=0x8c651eb8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c651eba; /* mov.l r14,@r2 */
L8c651eba: *stop=0x8c651ebau;if(!valid_access(r[13]+4u,4u))return -3;wr(m,bus,&io_status,r[13]+0x4u,r[14],4);if(io_status)return io_status;goto L8c651ebc; /* mov.l r14,@(0x4,r13) */
L8c651ebc: *stop=0x8c651ebcu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651ebe; /* mov.l @r15,r3 */
L8c651ebe: *stop=0x8c651ebeu;r[3]+=1u;goto L8c651ec0; /* add #0x1,r3 */
L8c651ec0: *stop=0x8c651ec0u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c651ec2; /* mov.l r3,@r15 */
L8c651ec2: *stop=0x8c651ec2u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651ec4; /* mov.l @(0xc,r15),r2 */
L8c651ec4: *stop=0x8c651ec4u;r[2]+=4u;goto L8c651ec6; /* add #0x4,r2 */
L8c651ec6: *stop=0x8c651ec6u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c651ec8; /* mov.l r2,@(0xc,r15) */
L8c651ec8: *stop=0x8c651ec8u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651eca; /* mov.l @(0x4,r15),r1 */
L8c651eca: *stop=0x8c651ecau;r[1]+=4u;goto L8c651ecc; /* add #0x4,r1 */
L8c651ecc: *stop=0x8c651eccu;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c651ece; /* mov.l r1,@(0x4,r15) */
L8c651ece: *stop=0x8c651eceu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651ed0; /* mov.l @(0x8,r15),r3 */
L8c651ed0: *stop=0x8c651ed0u;r[3]+=4u;goto L8c651ed2; /* add #0x4,r3 */
L8c651ed2: *stop=0x8c651ed2u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c651ed4; /* mov.l r3,@(0x8,r15) */
L8c651ed4: *stop=0x8c651ed4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[1]=loaded;}goto L8c651ed6; /* mov.l @r15,r1 */
L8c651ed6: *stop=0x8c651ed6u;r[2]=4u;goto L8c651ed8; /* mov #0x4,r2 */
L8c651ed8: *stop=0x8c651ed8u;*sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[2]);goto L8c651eda; /* cmp/ge r2,r1 */
L8c651eda: if((*sr&1u))goto L8c651ee0;goto L8c651edc; /* bt 0x8c651ee0 */
L8c651edc: *stop=0x8c651edeu;goto L8c651d1a; /* bra 0x8c651d1a */
L8c651ee0: *stop=0x8c651ee0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f7cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651ee2; /* mov.l 0x8c651f7c,r2 */
L8c651ee2: *stop=0x8c651ee2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651ee4; /* mov.l @r2,r3 */
L8c651ee4: *stop=0x8c651ee4u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651ee6; /* mov.l @(0x10,r15),r2 */
L8c651ee6: *stop=0x8c651ee6u;r[2]|=r[3];goto L8c651ee8; /* or r3,r2 */
L8c651ee8: *stop=0x8c651ee8u;r[4]=r[2];goto L8c651eea; /* mov r2,r4 */
L8c651eea: *stop=0x8c651eeau;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c651eec; /* mov.l r2,@(0x10,r15) */
L8c651eec: *stop=0x8c651eecu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c651eee; /* tst r4,r4 */
L8c651eee: if(!(*sr&1u))goto L8c651f08;goto L8c651ef0; /* bf 0x8c651f08 */
L8c651ef0: target=0x8c651570u;*pr=0x8c651ef4u;*stop=0x8c651ef2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651ef4; /* bsr 0x8c651570 */
L8c651ef4: target=0x8c6516a0u;*pr=0x8c651ef8u;*stop=0x8c651ef6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651ef8; /* bsr 0x8c6516a0 */
L8c651ef8: *stop=0x8c651ef8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c651efa; /* tst r0,r0 */
L8c651efa: if(!(*sr&1u))goto L8c651f00;goto L8c651efc; /* bf 0x8c651f00 */
L8c651efc: *stop=0x8c651efeu;r[0]=1u;goto L8c651f0a; /* bra 0x8c651f0a */
L8c651f00: target=0x8c65194au;*pr=0x8c651f04u;*stop=0x8c651f02u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651f04; /* bsr 0x8c65194a */
L8c651f04: *stop=0x8c651f04u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651f90u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651f06; /* mov.l 0x8c651f90,r3 */
L8c651f06: *stop=0x8c651f06u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c651f08; /* mov.l r14,@r3 */
L8c651f08: *stop=0x8c651f08u;r[0]=0u;goto L8c651f0a; /* mov #0x0,r0 */
L8c651f0a: *stop=0x8c651f0au;r[15]+=20u;goto L8c651f0c; /* add #0x14,r15 */
L8c651f0c: *stop=0x8c651f0cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c651f0e; /* lds.l @r15+,PR */
L8c651f0e: *stop=0x8c651f0eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c651f10; /* mov.l @r15+,r8 */
L8c651f10: *stop=0x8c651f10u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c651f12; /* mov.l @r15+,r9 */
L8c651f12: *stop=0x8c651f12u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c651f14; /* mov.l @r15+,r10 */
L8c651f14: *stop=0x8c651f14u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c651f16; /* mov.l @r15+,r11 */
L8c651f16: *stop=0x8c651f16u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c651f18; /* mov.l @r15+,r12 */
L8c651f18: *stop=0x8c651f18u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c651f1a; /* mov.l @r15+,r13 */
L8c651f1a: *stop=0x8c651f1au;if(*pr!=entry_pr)return -3;*stop=0x8c651f1cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_interrupt_8c650b60(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 unsigned wait_visits[1]={0};(void)wait_visits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c650b60;
L8c650b60: *stop=0x8c650b60u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c650b62; /* mov.l r14,@-r15 */
L8c650b62: *stop=0x8c650b62u;r[15]+=4294967292u;goto L8c650b64; /* add #-0x4,r15 */
L8c650b64: *stop=0x8c650b64u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650c28u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c650b66; /* mov.l 0x8c650c28,r6 */
L8c650b66: *stop=0x8c650b66u;r[4]=0u;goto L8c650b68; /* mov #0x0,r4 */
L8c650b68: *stop=0x8c650b68u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[4],4);if(io_status)return io_status;goto L8c650b6a; /* mov.l r4,@r6 */
L8c650b6a: *stop=0x8c650b6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c650c2cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c650b6c; /* mov.l 0x8c650c2c,r5 */
L8c650b6c: *stop=0x8c650b6cu;r[7]=1u;goto L8c650b6e; /* mov #0x1,r7 */
L8c650b6e: *stop=0x8c650b6eu;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[7],4);if(io_status)return io_status;goto L8c650b70; /* mov.l r7,@r5 */
L8c650b70: *stop=0x8c650b70u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c650b72; /* mov.l r4,@r5 */
L8c650b72: *stop=0x8c650b72u;if(!valid_access(r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;r[2]=loaded;}goto L8c650b74; /* mov.l @r6,r2 */
L8c650b74: *stop=0x8c650b74u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c650b76; /* tst r2,r2 */
L8c650b76:
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c650b76u;if(!(*sr&1u) && ++wait_visits[0]>=2u)return -4;
#endif
 if(!(*sr&1u))goto L8c650b72;goto L8c650b78; /* bf 0x8c650b72 */
L8c650b78: *stop=0x8c650b78u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650c30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650b7a; /* mov.l 0x8c650c30,r3 */
L8c650b7a: *stop=0x8c650b7au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[6]=loaded;}goto L8c650b7c; /* mov.l @r3,r6 */
L8c650b7c: *stop=0x8c650b7cu;r[2]=4294967294u;goto L8c650b7e; /* mov #-0x2,r2 */
L8c650b7e: *stop=0x8c650b7eu;r[6]&=r[2];goto L8c650b80; /* and r2,r6 */
L8c650b80: *stop=0x8c650b80u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[6],4);if(io_status)return io_status;goto L8c650b82; /* mov.l r6,@r3 */
L8c650b82: *stop=0x8c650b82u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[7],4);if(io_status)return io_status;goto L8c650b84; /* mov.l r7,@r5 */
L8c650b84: *stop=0x8c650b84u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c650b86; /* mov.l r4,@r5 */
L8c650b86: *stop=0x8c650b86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650c34u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c650b88; /* mov.l 0x8c650c34,r5 */
L8c650b88: *stop=0x8c650b88u;r[7]=8u;goto L8c650b8a; /* mov #0x8,r7 */
L8c650b8a: *stop=0x8c650b8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c650c3cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c650b8c; /* mov.l 0x8c650c3c,r1 */
L8c650b8c: *stop=0x8c650b8cu;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c650b8e; /* mov.l r4,@r5 */
L8c650b8e: *stop=0x8c650b8eu;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[4],4);if(io_status)return io_status;goto L8c650b90; /* mov.l r4,@(0x4,r5) */
L8c650b90: *stop=0x8c650b90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650c38u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c650b92; /* mov.l 0x8c650c38,r5 */
L8c650b92: *stop=0x8c650b92u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650c40u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c650b94; /* mov.l 0x8c650c40,r2 */
L8c650b94: *stop=0x8c650b94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650c48u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c650b96; /* mov.l 0x8c650c48,r14 */
L8c650b96: *stop=0x8c650b96u;{uint32_t loaded=rd(m,bus,&io_status,0x8c650c44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c650b98; /* mov.l 0x8c650c44,r3 */
L8c650b98: *stop=0x8c650b98u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[4],4);if(io_status)return io_status;goto L8c650b9a; /* mov.l r4,@r5 */
L8c650b9a: *stop=0x8c650b9au;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[4],4);if(io_status)return io_status;goto L8c650b9c; /* mov.l r4,@(0x4,r5) */
L8c650b9c: *stop=0x8c650b9cu;r[5]=7u;goto L8c650b9e; /* mov #0x7,r5 */
L8c650b9e: *stop=0x8c650b9eu;r[6]=sx(m,bus,&io_status,0x8c650c20u,2);if(io_status)return io_status;goto L8c650ba0; /* mov.w 0x8c650c20,r6 */
L8c650ba0: *stop=0x8c650ba0u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c650ba2; /* mov.l r4,@r1 */
L8c650ba2: *stop=0x8c650ba2u;r[1]=r[14];goto L8c650ba4; /* mov r14,r1 */
L8c650ba4: *stop=0x8c650ba4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[5],4);if(io_status)return io_status;goto L8c650ba6; /* mov.l r5,@r2 */
L8c650ba6: *stop=0x8c650ba6u;r[1]+=r[6];goto L8c650ba8; /* add r6,r1 */
L8c650ba8: *stop=0x8c650ba8u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c650baa; /* mov.l r5,@r3 */
L8c650baa: *stop=0x8c650baau;r[5]=r[4];goto L8c650bac; /* mov r4,r5 */
L8c650bac: *stop=0x8c650bacu;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c650bae; /* mov.l r14,@r15 */
L8c650bae: *stop=0x8c650baeu;r[2]=r[1];goto L8c650bb0; /* mov r1,r2 */
L8c650bb0: *stop=0x8c650bb0u;r[3]=r[14];goto L8c650bb2; /* mov r14,r3 */
L8c650bb2: *stop=0x8c650bb2u;r[2]+=r[5];goto L8c650bb4; /* add r5,r2 */
L8c650bb4: *stop=0x8c650bb4u;r[3]+=r[5];goto L8c650bb6; /* add r5,r3 */
L8c650bb6: *stop=0x8c650bb6u;r[5]+=32u;goto L8c650bb8; /* add #0x20,r5 */
L8c650bb8: *stop=0x8c650bb8u;if(!valid_access(r[3]+24u,4u))return -3;wr(m,bus,&io_status,r[3]+0x18u,r[4],4);if(io_status)return io_status;goto L8c650bba; /* mov.l r4,@(0x18,r3) */
L8c650bba: *stop=0x8c650bbau;*sr=(*sr&~1u)|(r[5]>=r[6]);goto L8c650bbc; /* cmp/hs r6,r5 */
L8c650bbc: take=!(*sr&1u);*stop=0x8c650bbeu;if(!valid_access(r[2]+24u,4u))return -3;wr(m,bus,&io_status,r[2]+0x18u,r[7],4);if(io_status)return io_status;if(take)goto L8c650bae;goto L8c650bc0; /* bf/s 0x8c650bae */
L8c650bc0: *stop=0x8c650bc0u;r[0]=0u;goto L8c650bc2; /* mov #0x0,r0 */
L8c650bc2: *stop=0x8c650bc2u;r[15]+=4u;goto L8c650bc4; /* add #0x4,r15 */
L8c650bc4: *stop=0x8c650bc4u;if(*pr!=entry_pr)return -3;*stop=0x8c650bc6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_interrupt_8c665fa8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c665fa8;
L8c665fa8: *stop=0x8c665fa8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c665faa; /* mov.l r14,@-r15 */
L8c665faa: *stop=0x8c665faau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c665fac; /* mov.l r13,@-r15 */
L8c665fac: *stop=0x8c665facu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c665fae; /* mov.l r12,@-r15 */
L8c665fae: *stop=0x8c665faeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c665fb0; /* mov.l r11,@-r15 */
L8c665fb0: *stop=0x8c665fb0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c665fb2; /* mov.l r10,@-r15 */
L8c665fb2: *stop=0x8c665fb2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c665fb4; /* mov.l r9,@-r15 */
L8c665fb4: *stop=0x8c665fb4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c665fb6; /* mov.l r8,@-r15 */
L8c665fb6: *stop=0x8c665fb6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c665fb8; /* sts.l PR,@-r15 */
L8c665fb8: *stop=0x8c665fb8u;r[0]=sx(m,bus,&io_status,0x8c665fe4u,2);if(io_status)return io_status;goto L8c665fba; /* mov.w 0x8c665fe4,r0 */
L8c665fba: *stop=0x8c665fbau;r[15]+=r[0];goto L8c665fbc; /* add r0,r15 */
L8c665fbc: *stop=0x8c665fbcu;r[8]=0u;goto L8c665fbe; /* mov #0x0,r8 */
L8c665fbe: *stop=0x8c665fbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66601cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665fc0; /* mov.l 0x8c66601c,r3 */
L8c665fc0: *stop=0x8c665fc0u;r[0]=80u;goto L8c665fc2; /* mov #0x50,r0 */
L8c665fc2: *stop=0x8c665fc2u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c665fc4; /* mov.l r4,@r15 */
L8c665fc4: *stop=0x8c665fc4u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c665fc6; /* mov.l r5,@(0x4,r15) */
L8c665fc6: *stop=0x8c665fc6u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c665fc8; /* mov.l r6,@(0x8,r15) */
L8c665fc8: *stop=0x8c665fc8u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[8],4);if(io_status)return io_status;goto L8c665fca; /* mov.l r8,@(r0,r15) */
L8c665fca: *stop=0x8c665fcau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665fcc; /* mov.l @r3,r0 */
L8c665fcc: *stop=0x8c665fccu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c665fce; /* cmp/eq #0x1,r0 */
L8c665fce: if(!(*sr&1u))goto L8c666020;goto L8c665fd0; /* bf 0x8c666020 */
L8c665fd0: *stop=0x8c665fd2u;r[12]=1u;goto L8c666022; /* bra 0x8c666022 */
L8c666020: *stop=0x8c666020u;r[12]=r[8];goto L8c666022; /* mov r8,r12 */
L8c666022: *stop=0x8c666022u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6660c4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666024; /* mov.l 0x8c6660c4,r2 */
L8c666024: *stop=0x8c666024u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666026; /* mov.l @r2,r3 */
L8c666026: *stop=0x8c666026u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c666028; /* tst r3,r3 */
L8c666028: if(!(*sr&1u))goto L8c666034;goto L8c66602a; /* bf 0x8c666034 */
L8c66602a: *stop=0x8c66602au;r[0]=sx(m,bus,&io_status,0x8c6660c0u,2);if(io_status)return io_status;goto L8c66602c; /* mov.w 0x8c6660c0,r0 */
L8c66602c: *stop=0x8c66602cu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c66602e; /* mov.l @(0x4,r15),r5 */
L8c66602e: *stop=0x8c66602eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c666030; /* mov.l @(r0,r15),r6 */
L8c666030: target=0x8c665e3cu;*pr=0x8c666034u;*stop=0x8c666032u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666034; /* bsr 0x8c665e3c */
L8c666034: *stop=0x8c666034u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}goto L8c666036; /* mov.l @r15,r2 */
L8c666036: *stop=0x8c666036u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666038; /* mov.l @(0x4,r15),r3 */
L8c666038: *stop=0x8c666038u;r[0]=sx(m,bus,&io_status,0x8c6660c0u,2);if(io_status)return io_status;goto L8c66603a; /* mov.w 0x8c6660c0,r0 */
L8c66603a: *stop=0x8c66603au;*macl=r[3]*r[2];goto L8c66603c; /* mul.l r3,r2 */
L8c66603c: *stop=0x8c66603cu;r[3]=*macl;goto L8c66603e; /* sts MACL,r3 */
L8c66603e: *stop=0x8c66603eu;r[14]=r[3];goto L8c666040; /* mov r3,r14 */
L8c666040: *stop=0x8c666040u;*sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c666042; /* shll r14 */
L8c666042: *stop=0x8c666042u;r[1]=r[3];goto L8c666044; /* mov r3,r1 */
L8c666044: *stop=0x8c666044u;r[14]+=r[1];goto L8c666046; /* add r1,r14 */
L8c666046: *stop=0x8c666046u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c666048; /* mov.l r3,@(0x4,r15) */
L8c666048: *stop=0x8c666048u;r[14]<<=2;goto L8c66604a; /* shll2 r14 */
L8c66604a: *stop=0x8c66604au;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66604c; /* mov.l @(0x8,r15),r3 */
L8c66604c: *stop=0x8c66604cu;*sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c66604e; /* shll r14 */
L8c66604e: *stop=0x8c66604eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[13]=loaded;}goto L8c666050; /* mov.l @(r0,r15),r13 */
L8c666050: *stop=0x8c666050u;*macl=r[14]*r[3];goto L8c666052; /* mul.l r14,r3 */
L8c666052: *stop=0x8c666052u;r[13]<<=2;goto L8c666054; /* shll2 r13 */
L8c666054: *stop=0x8c666054u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6660c8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c666056; /* mov.l 0x8c6660c8,r0 */
L8c666056: *stop=0x8c666056u;r[13]<<=2;goto L8c666058; /* shll2 r13 */
L8c666058: *stop=0x8c666058u;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c66605a; /* shll r13 */
L8c66605a: *stop=0x8c66605au;r[3]=*macl;goto L8c66605c; /* sts MACL,r3 */
L8c66605c: *stop=0x8c66605cu;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66605e; /* mov.l @(r0,r13),r2 */
L8c66605e: *stop=0x8c66605eu;r[3]+=r[2];goto L8c666060; /* add r2,r3 */
L8c666060: *stop=0x8c666060u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6660ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666062; /* mov.l 0x8c6660cc,r2 */
L8c666062: *stop=0x8c666062u;if(!valid_access(r[15]+12u,4u))return -3;wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c666064; /* mov.l r3,@(0xc,r15) */
L8c666064: *stop=0x8c666064u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c666066; /* mov.l @r2,r0 */
L8c666066: *stop=0x8c666066u;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c666068; /* tst #0x20,r0 */
L8c666068: if((*sr&1u))goto L8c6660a2;goto L8c66606a; /* bt 0x8c6660a2 */
L8c66606a: *stop=0x8c66606au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6660d0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c66606c; /* mov.l 0x8c6660d0,r0 */
L8c66606c: *stop=0x8c66606cu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c66606e; /* mov.l @r0,r1 */
L8c66606e: *stop=0x8c66606eu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c666070; /* tst r1,r1 */
L8c666070: if((*sr&1u))goto L8c6660a2;goto L8c666072; /* bt 0x8c6660a2 */
L8c666072: *stop=0x8c666072u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6660d4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c666074; /* mov.l 0x8c6660d4,r0 */
L8c666074: *stop=0x8c666074u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c666076; /* mov.l @(r0,r13),r5 */
L8c666076: *stop=0x8c666076u;r[4]=r[0];goto L8c666078; /* mov r0,r4 */
L8c666078: *stop=0x8c666078u;r[14]<<=2;goto L8c66607a; /* shll2 r14 */
L8c66607a: *stop=0x8c66607au;r[4]+=r[13];goto L8c66607c; /* add r13,r4 */
L8c66607c: *stop=0x8c66607cu;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c66607e; /* mov.l @(0x4,r4),r4 */
L8c66607e: target=0x8c665f42u;*pr=0x8c666082u;*stop=0x8c666080u;r[4]+=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666082; /* bsr 0x8c665f42 */
L8c666082: *stop=0x8c666082u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6660d8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666084; /* mov.l 0x8c6660d8,r2 */
L8c666084: *stop=0x8c666084u;if(!valid_access(r[15]+20u,4u))return -3;wr(m,bus,&io_status,r[15]+0x14u,r[0],4);if(io_status)return io_status;goto L8c666086; /* mov.l r0,@(0x14,r15) */
L8c666086: *stop=0x8c666086u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666088; /* mov.l @r2,r3 */
L8c666088: *stop=0x8c666088u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c66608a; /* tst r3,r3 */
L8c66608a: if(!(*sr&1u))goto L8c66609c;goto L8c66608c; /* bf 0x8c66609c */
L8c66608c: *stop=0x8c66608cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6660dcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c66608e; /* mov.l 0x8c6660dc,r1 */
L8c66608e: *stop=0x8c66608eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666090; /* mov.l @r1,r3 */
L8c666090: *stop=0x8c666090u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c666092; /* tst r3,r3 */
L8c666092: if((*sr&1u))goto L8c66609c;goto L8c666094; /* bt 0x8c66609c */
L8c666094: *stop=0x8c666094u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c666096; /* tst r12,r12 */
L8c666096: if((*sr&1u))goto L8c66609c;goto L8c666098; /* bt 0x8c66609c */
L8c666098: *stop=0x8c66609au;r[2]=1u;goto L8c66609e; /* bra 0x8c66609e */
L8c66609c: *stop=0x8c66609cu;r[2]=0u;goto L8c66609e; /* mov #0x0,r2 */
L8c66609e: *stop=0x8c66609eu;r[0]=80u;goto L8c6660a0; /* mov #0x50,r0 */
L8c6660a0: *stop=0x8c6660a0u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c6660a2; /* mov.l r2,@(r0,r15) */
L8c6660a2: *stop=0x8c6660a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6660d8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6660a4; /* mov.l 0x8c6660d8,r2 */
L8c6660a4: *stop=0x8c6660a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6660e0u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c6660a6; /* mov.l 0x8c6660e0,r14 */
L8c6660a6: *stop=0x8c6660a6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6660a8; /* mov.l @r2,r3 */
L8c6660a8: *stop=0x8c6660a8u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6660aa; /* tst r3,r3 */
L8c6660aa: if(!(*sr&1u))goto L8c666100;goto L8c6660ac; /* bf 0x8c666100 */
L8c6660ac: *stop=0x8c6660acu;r[0]=76u;goto L8c6660ae; /* mov #0x4c,r0 */
L8c6660ae: *stop=0x8c6660b0u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666104; /* bra 0x8c666104 */
L8c666100: *stop=0x8c666100u;r[0]=76u;goto L8c666102; /* mov #0x4c,r0 */
L8c666102: *stop=0x8c666102u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[8],4);if(io_status)return io_status;goto L8c666104; /* mov.l r8,@(r0,r15) */
L8c666104: *stop=0x8c666104u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666160u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666106; /* mov.l 0x8c666160,r2 */
L8c666106: *stop=0x8c666106u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666108; /* mov.l @r2,r3 */
L8c666108: *stop=0x8c666108u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c66610a; /* tst r3,r3 */
L8c66610a: if(!(*sr&1u))goto L8c666112;goto L8c66610c; /* bf 0x8c666112 */
L8c66610c: *stop=0x8c66610cu;r[0]=92u;goto L8c66610e; /* mov #0x5c,r0 */
L8c66610e: *stop=0x8c666110u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666116; /* bra 0x8c666116 */
L8c666112: *stop=0x8c666112u;r[0]=92u;goto L8c666114; /* mov #0x5c,r0 */
L8c666114: *stop=0x8c666114u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[8],4);if(io_status)return io_status;goto L8c666116; /* mov.l r8,@(r0,r15) */
L8c666116: *stop=0x8c666116u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666164u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666118; /* mov.l 0x8c666164,r2 */
L8c666118: *stop=0x8c666118u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66611a; /* mov.l @r2,r3 */
L8c66611a: *stop=0x8c66611au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c66611c; /* tst r3,r3 */
L8c66611c: if(!(*sr&1u))goto L8c666124;goto L8c66611e; /* bf 0x8c666124 */
L8c66611e: *stop=0x8c66611eu;r[0]=72u;goto L8c666120; /* mov #0x48,r0 */
L8c666120: *stop=0x8c666122u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666128; /* bra 0x8c666128 */
L8c666124: *stop=0x8c666124u;r[0]=72u;goto L8c666126; /* mov #0x48,r0 */
L8c666126: *stop=0x8c666126u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[8],4);if(io_status)return io_status;goto L8c666128; /* mov.l r8,@(r0,r15) */
L8c666128: *stop=0x8c666128u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666168u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66612a; /* mov.l 0x8c666168,r2 */
L8c66612a: *stop=0x8c66612au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66612c; /* mov.l @r2,r3 */
L8c66612c: *stop=0x8c66612cu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c66612e; /* tst r3,r3 */
L8c66612e: if(!(*sr&1u))goto L8c666136;goto L8c666130; /* bf 0x8c666136 */
L8c666130: *stop=0x8c666130u;r[0]=84u;goto L8c666132; /* mov #0x54,r0 */
L8c666132: *stop=0x8c666134u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c66613a; /* bra 0x8c66613a */
L8c666136: *stop=0x8c666136u;r[0]=84u;goto L8c666138; /* mov #0x54,r0 */
L8c666138: *stop=0x8c666138u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[8],4);if(io_status)return io_status;goto L8c66613a; /* mov.l r8,@(r0,r15) */
L8c66613a: *stop=0x8c66613au;{uint32_t loaded=rd(m,bus,&io_status,0x8c66616cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66613c; /* mov.l 0x8c66616c,r2 */
L8c66613c: *stop=0x8c66613cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66613e; /* mov.l @r2,r3 */
L8c66613e: *stop=0x8c66613eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c666140; /* tst r3,r3 */
L8c666140: if(!(*sr&1u))goto L8c666180;goto L8c666142; /* bf 0x8c666180 */
L8c666142: *stop=0x8c666142u;r[0]=88u;goto L8c666144; /* mov #0x58,r0 */
L8c666144: *stop=0x8c666146u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666184; /* bra 0x8c666184 */
L8c666180: *stop=0x8c666180u;r[0]=88u;goto L8c666182; /* mov #0x58,r0 */
L8c666182: *stop=0x8c666182u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[8],4);if(io_status)return io_status;goto L8c666184; /* mov.l r8,@(r0,r15) */
L8c666184: *stop=0x8c666184u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666288u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666186; /* mov.l 0x8c666288,r3 */
L8c666186: *stop=0x8c666186u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666284u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c666188; /* mov.l 0x8c666284,r4 */
L8c666188: *stop=0x8c666188u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66618a; /* mov.l @r3,r0 */
L8c66618a: *stop=0x8c66618au;r[0]<<=2;goto L8c66618c; /* shll2 r0 */
L8c66618c: *stop=0x8c66618cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c66628cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c66618e; /* mov.l 0x8c66628c,r1 */
L8c66618e: *stop=0x8c66618eu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c666190; /* mov.l @(r0,r4),r2 */
L8c666190: *stop=0x8c666190u;r[2]<<=2;goto L8c666192; /* shll2 r2 */
L8c666192: *stop=0x8c666192u;if(!valid_access(r[15]+56u,4u))return -3;wr(m,bus,&io_status,r[15]+0x38u,r[2],4);if(io_status)return io_status;goto L8c666194; /* mov.l r2,@(0x38,r15) */
L8c666194: *stop=0x8c666194u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c666196; /* mov.l @r1,r0 */
L8c666196: *stop=0x8c666196u;r[0]<<=2;goto L8c666198; /* shll2 r0 */
L8c666198: *stop=0x8c666198u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666290u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66619a; /* mov.l 0x8c666290,r2 */
L8c66619a: *stop=0x8c66619au;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66619c; /* mov.l @(r0,r4),r3 */
L8c66619c: *stop=0x8c66619cu;r[0]=64u;goto L8c66619e; /* mov #0x40,r0 */
L8c66619e: *stop=0x8c66619eu;r[3]<<=2;goto L8c6661a0; /* shll2 r3 */
L8c6661a0: *stop=0x8c6661a0u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c6661a2; /* mov.l r3,@(r0,r15) */
L8c6661a2: *stop=0x8c6661a2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6661a4; /* mov.l @r2,r0 */
L8c6661a4: *stop=0x8c6661a4u;r[0]<<=2;goto L8c6661a6; /* shll2 r0 */
L8c6661a6: *stop=0x8c6661a6u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6661a8; /* mov.l @(r0,r4),r3 */
L8c6661a8: *stop=0x8c6661a8u;r[3]<<=2;goto L8c6661aa; /* shll2 r3 */
L8c6661aa: *stop=0x8c6661aau;if(!valid_access(r[15]+60u,4u))return -3;wr(m,bus,&io_status,r[15]+0x3cu,r[3],4);if(io_status)return io_status;goto L8c6661ac; /* mov.l r3,@(0x3c,r15) */
L8c6661ac: *stop=0x8c6661acu;if(!valid_access(r[1]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+0x8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6661ae; /* mov.l @(0x8,r1),r0 */
L8c6661ae: *stop=0x8c6661aeu;r[0]<<=2;goto L8c6661b0; /* shll2 r0 */
L8c6661b0: *stop=0x8c6661b0u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6661b2; /* mov.l @(r0,r4),r3 */
L8c6661b2: *stop=0x8c6661b2u;r[2]+=8u;goto L8c6661b4; /* add #0x8,r2 */
L8c6661b4: *stop=0x8c6661b4u;r[3]<<=2;goto L8c6661b6; /* shll2 r3 */
L8c6661b6: *stop=0x8c6661b6u;if(!valid_access(r[15]+48u,4u))return -3;wr(m,bus,&io_status,r[15]+0x30u,r[3],4);if(io_status)return io_status;goto L8c6661b8; /* mov.l r3,@(0x30,r15) */
L8c6661b8: *stop=0x8c6661b8u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6661ba; /* mov.l @r2,r0 */
L8c6661ba: *stop=0x8c6661bau;r[0]<<=2;goto L8c6661bc; /* shll2 r0 */
L8c6661bc: *stop=0x8c6661bcu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6661be; /* mov.l @(r0,r4),r3 */
L8c6661be: *stop=0x8c6661beu;r[0]=68u;goto L8c6661c0; /* mov #0x44,r0 */
L8c6661c0: *stop=0x8c6661c0u;r[3]<<=2;goto L8c6661c2; /* shll2 r3 */
L8c6661c2: *stop=0x8c6661c2u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c6661c4; /* mov.l r3,@(r0,r15) */
L8c6661c4: *stop=0x8c6661c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666294u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6661c6; /* mov.l 0x8c666294,r0 */
L8c6661c6: *stop=0x8c6661c6u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6661c8; /* mov.l @(r0,r13),r1 */
L8c6661c8: *stop=0x8c6661c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666288u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6661ca; /* mov.l 0x8c666288,r3 */
L8c6661ca: *stop=0x8c6661cau;if(!valid_access(r[15]+28u,4u))return -3;wr(m,bus,&io_status,r[15]+0x1cu,r[1],4);if(io_status)return io_status;goto L8c6661cc; /* mov.l r1,@(0x1c,r15) */
L8c6661cc: *stop=0x8c6661ccu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6661ce; /* mov.l @r3,r0 */
L8c6661ce: *stop=0x8c6661ceu;r[0]<<=2;goto L8c6661d0; /* shll2 r0 */
L8c6661d0: *stop=0x8c6661d0u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c6661d2; /* mov.l @(0x4,r15),r5 */
L8c6661d2: *stop=0x8c6661d2u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6661d4; /* mov.l @(r0,r4),r1 */
L8c6661d4: *stop=0x8c6661d4u;r[2]+=4294967284u;goto L8c6661d6; /* add #-0xc,r2 */
L8c6661d6: *stop=0x8c6661d6u;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6661d8; /* mov.l @(0x1c,r15),r0 */
L8c6661d8: *stop=0x8c6661d8u;*macl=r[5]*r[1];goto L8c6661da; /* mul.l r5,r1 */
L8c6661da: *stop=0x8c6661dau;r[1]=*macl;goto L8c6661dc; /* sts MACL,r1 */
L8c6661dc: *stop=0x8c6661dcu;r[1]<<=2;goto L8c6661de; /* shll2 r1 */
L8c6661de: *stop=0x8c6661deu;r[1]+=r[0];goto L8c6661e0; /* add r0,r1 */
L8c6661e0: *stop=0x8c6661e0u;if(!valid_access(r[15]+24u,4u))return -3;wr(m,bus,&io_status,r[15]+0x18u,r[1],4);if(io_status)return io_status;goto L8c6661e2; /* mov.l r1,@(0x18,r15) */
L8c6661e2: *stop=0x8c6661e2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6661e4; /* mov.l @r2,r0 */
L8c6661e4: *stop=0x8c6661e4u;r[0]<<=2;goto L8c6661e6; /* shll2 r0 */
L8c6661e6: *stop=0x8c6661e6u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6661e8; /* mov.l @(r0,r4),r2 */
L8c6661e8: *stop=0x8c6661e8u;*macl=r[5]*r[2];goto L8c6661ea; /* mul.l r5,r2 */
L8c6661ea: *stop=0x8c6661eau;r[2]=*macl;goto L8c6661ec; /* sts MACL,r2 */
L8c6661ec: *stop=0x8c6661ecu;r[2]<<=2;goto L8c6661ee; /* shll2 r2 */
L8c6661ee: *stop=0x8c6661eeu;r[1]+=r[2];goto L8c6661f0; /* add r2,r1 */
L8c6661f0: *stop=0x8c6661f0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666290u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6661f2; /* mov.l 0x8c666290,r2 */
L8c6661f2: *stop=0x8c6661f2u;if(!valid_access(r[15]+36u,4u))return -3;wr(m,bus,&io_status,r[15]+0x24u,r[1],4);if(io_status)return io_status;goto L8c6661f4; /* mov.l r1,@(0x24,r15) */
L8c6661f4: *stop=0x8c6661f4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6661f6; /* mov.l @r2,r0 */
L8c6661f6: *stop=0x8c6661f6u;r[0]<<=2;goto L8c6661f8; /* shll2 r0 */
L8c6661f8: *stop=0x8c6661f8u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6661fa; /* mov.l @(r0,r4),r1 */
L8c6661fa: *stop=0x8c6661fau;if(!valid_access(r[15]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x24u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6661fc; /* mov.l @(0x24,r15),r0 */
L8c6661fc: *stop=0x8c6661fcu;*macl=r[5]*r[1];goto L8c6661fe; /* mul.l r5,r1 */
L8c6661fe: *stop=0x8c6661feu;r[1]=*macl;goto L8c666200; /* sts MACL,r1 */
L8c666200: *stop=0x8c666200u;r[1]<<=2;goto L8c666202; /* shll2 r1 */
L8c666202: *stop=0x8c666202u;r[1]+=r[0];goto L8c666204; /* add r0,r1 */
L8c666204: *stop=0x8c666204u;if(!valid_access(r[15]+32u,4u))return -3;wr(m,bus,&io_status,r[15]+0x20u,r[1],4);if(io_status)return io_status;goto L8c666206; /* mov.l r1,@(0x20,r15) */
L8c666206: *stop=0x8c666206u;if(!valid_access(r[3]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c666208; /* mov.l @(0xc,r3),r0 */
L8c666208: *stop=0x8c666208u;r[0]<<=2;goto L8c66620a; /* shll2 r0 */
L8c66620a: *stop=0x8c66620au;r[13]=r[8];goto L8c66620c; /* mov r8,r13 */
L8c66620c: *stop=0x8c66620cu;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c66620e; /* mov.l @(r0,r4),r3 */
L8c66620e: *stop=0x8c66620eu;r[2]=29u;goto L8c666210; /* mov #0x1d,r2 */
L8c666210: *stop=0x8c666210u;*macl=r[5]*r[3];goto L8c666212; /* mul.l r5,r3 */
L8c666212: *stop=0x8c666212u;r[3]=*macl;goto L8c666214; /* sts MACL,r3 */
L8c666214: *stop=0x8c666214u;r[3]<<=2;goto L8c666216; /* shll2 r3 */
L8c666216: *stop=0x8c666216u;r[1]+=r[3];goto L8c666218; /* add r3,r1 */
L8c666218: *stop=0x8c666218u;r[3]=r[5];goto L8c66621a; /* mov r5,r3 */
L8c66621a: *stop=0x8c66621au;r[3]+=4294967295u;goto L8c66621c; /* add #-0x1,r3 */
L8c66621c: *stop=0x8c66621cu;if(!valid_access(r[15]+40u,4u))return -3;wr(m,bus,&io_status,r[15]+0x28u,r[1],4);if(io_status)return io_status;goto L8c66621e; /* mov.l r1,@(0x28,r15) */
L8c66621e: *stop=0x8c66621eu;r[1]=1u;goto L8c666220; /* mov #0x1,r1 */
L8c666220: *stop=0x8c666220u;if(!valid_access(r[15]+44u,4u))return -3;wr(m,bus,&io_status,r[15]+0x2cu,r[3],4);if(io_status)return io_status;goto L8c666222; /* mov.l r3,@(0x2c,r15) */
L8c666222: *stop=0x8c666222u;r[3]=0u;goto L8c666224; /* mov #0x0,r3 */
L8c666224: *stop=0x8c666224u;r[12]&=r[1];goto L8c666226; /* and r1,r12 */
L8c666226: *stop=0x8c666226u;*sr=(*sr&~1u)|(r[3]>=r[5]);goto L8c666228; /* cmp/hs r5,r3 */
L8c666228: *stop=0x8c666228u;r[12]=(r[2]&0x80000000u)?((r[2]&31u)?r[12]>>((0u-r[2])&31u):0u):(r[12]<<(r[2]&31u));goto L8c66622a; /* shld r2,r12 */
L8c66622a: take=!(*sr&1u);*stop=0x8c66622cu;if(!valid_access(r[15]+52u,4u))return -3;wr(m,bus,&io_status,r[15]+0x34u,r[12],4);if(io_status)return io_status;if(take)goto L8c666232;goto L8c66622e; /* bf/s 0x8c666232 */
L8c66622e: *stop=0x8c666230u;goto L8c66667c; /* bra 0x8c66667c */
L8c666232: *stop=0x8c666232u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6662a0u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c666234; /* mov.l 0x8c6662a0,r11 */
L8c666234: *stop=0x8c666234u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6662a4u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c666236; /* mov.l 0x8c6662a4,r12 */
L8c666236: *stop=0x8c666236u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666298u,4);if(io_status)return io_status;r[9]=loaded;}goto L8c666238; /* mov.l 0x8c666298,r9 */
L8c666238: *stop=0x8c666238u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66629cu,4);if(io_status)return io_status;r[10]=loaded;}goto L8c66623a; /* mov.l 0x8c66629c,r10 */
L8c66623a: *stop=0x8c66623au;r[0]=sx(m,bus,&io_status,0x8c666280u,2);if(io_status)return io_status;goto L8c66623c; /* mov.w 0x8c666280,r0 */
L8c66623c: *stop=0x8c66623cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66623e; /* mov.l @(r0,r15),r0 */
L8c66623e: *stop=0x8c66623eu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c666240; /* cmp/eq #0x0,r0 */
L8c666240: take=(*sr&1u);*stop=0x8c666242u;r[4]=r[8];if(take)goto L8c66624c;goto L8c666244; /* bt/s 0x8c66624c */
L8c666244: *stop=0x8c666244u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c666246; /* cmp/eq #0x1,r0 */
L8c666246: if((*sr&1u))goto L8c6662c0;goto L8c666248; /* bt 0x8c6662c0 */
L8c666248: *stop=0x8c66624au;goto L8c6662d8; /* bra 0x8c6662d8 */
L8c66624c: *stop=0x8c66624cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6662acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66624e; /* mov.l 0x8c6662ac,r3 */
L8c66624e: *stop=0x8c66624eu;r[1]=r[13];goto L8c666250; /* mov r13,r1 */
L8c666250: *stop=0x8c666250u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6662a8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666252; /* mov.l 0x8c6662a8,r2 */
L8c666252: target=r[3];*pr=0x8c666256u;*stop=0x8c666254u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666256; /* jsr @r3 */
L8c666256: *stop=0x8c666256u;r[3]=r[2];goto L8c666258; /* mov r2,r3 */
L8c666258: *stop=0x8c666258u;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[0],4);if(io_status)return io_status;goto L8c66625a; /* mov.l r0,@(0x10,r15) */
L8c66625a: *stop=0x8c66625au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66625c; /* mov.l @r3,r0 */
L8c66625c: *stop=0x8c66625cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6662b0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66625e; /* mov.l 0x8c6662b0,r3 */
L8c66625e: target=r[3];*pr=0x8c666262u;*stop=0x8c666260u;r[1]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666262; /* jsr @r3 */
L8c666262: *stop=0x8c666264u;goto L8c6662d6; /* bra 0x8c6662d6 */
L8c6662c0: *stop=0x8c6662c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6663b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6662c2; /* mov.l 0x8c6663b0,r2 */
L8c6662c2: *stop=0x8c6662c2u;r[1]=r[13];goto L8c6662c4; /* mov r13,r1 */
L8c6662c4: *stop=0x8c6662c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6663acu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6662c6; /* mov.l 0x8c6663ac,r3 */
L8c6662c6: target=r[2];*pr=0x8c6662cau;*stop=0x8c6662c8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6662ca; /* jsr @r2 */
L8c6662ca: *stop=0x8c6662cau;r[2]=r[3];goto L8c6662cc; /* mov r3,r2 */
L8c6662cc: *stop=0x8c6662ccu;if(!valid_access(r[15]+16u,4u))return -3;wr(m,bus,&io_status,r[15]+0x10u,r[0],4);if(io_status)return io_status;goto L8c6662ce; /* mov.l r0,@(0x10,r15) */
L8c6662ce: *stop=0x8c6662ceu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6662d0; /* mov.l @r2,r0 */
L8c6662d0: *stop=0x8c6662d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6663b4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6662d2; /* mov.l 0x8c6663b4,r2 */
L8c6662d2: target=r[2];*pr=0x8c6662d6u;*stop=0x8c6662d4u;r[1]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6662d6; /* jsr @r2 */
L8c6662d6: *stop=0x8c6662d6u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[0],4);if(io_status)return io_status;goto L8c6662d8; /* mov.l r0,@(0x8,r15) */
L8c6662d8: *stop=0x8c6662d8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6663b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6662da; /* mov.l 0x8c6663b8,r3 */
L8c6662da: *stop=0x8c6662dau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6662dc; /* mov.l @r3,r0 */
L8c6662dc: *stop=0x8c6662dcu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6662de; /* cmp/eq #0x1,r0 */
L8c6662de: if(!(*sr&1u))goto L8c6662f6;goto L8c6662e0; /* bf 0x8c6662f6 */
L8c6662e0: *stop=0x8c6662e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6663bcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6662e2; /* mov.l 0x8c6663bc,r0 */
L8c6662e2: *stop=0x8c6662e2u;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6662e4; /* mov.l @(0x10,r15),r1 */
L8c6662e4: *stop=0x8c6662e4u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6662e6; /* mov.l @r0,r2 */
L8c6662e6: *stop=0x8c6662e6u;*sr=(*sr&~1u)|(r[2]>=r[1]);goto L8c6662e8; /* cmp/hs r1,r2 */
L8c6662e8: if(!(*sr&1u))goto L8c6662f4;goto L8c6662ea; /* bf 0x8c6662f4 */
L8c6662ea: *stop=0x8c6662eau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6663c0u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6662ec; /* mov.l 0x8c6663c0,r0 */
L8c6662ec: *stop=0x8c6662ecu;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6662ee; /* mov.l @(0x8,r15),r2 */
L8c6662ee: *stop=0x8c6662eeu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6662f0; /* mov.l @r0,r1 */
L8c6662f0: *stop=0x8c6662f0u;*sr=(*sr&~1u)|(r[1]>=r[2]);goto L8c6662f2; /* cmp/hs r2,r1 */
L8c6662f2: if((*sr&1u))goto L8c6662f6;goto L8c6662f4; /* bt 0x8c6662f6 */
L8c6662f4: *stop=0x8c6662f4u;r[4]=r[14];goto L8c6662f6; /* mov r14,r4 */
L8c6662f6: *stop=0x8c6662f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6663acu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6662f8; /* mov.l 0x8c6663ac,r2 */
L8c6662f8: *stop=0x8c6662f8u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6662fa; /* mov.l @r2,r3 */
L8c6662fa: *stop=0x8c6662fau;if(!valid_access(r[15]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6662fc; /* mov.l @(0x8,r15),r2 */
L8c6662fc: *stop=0x8c6662fcu;if(!valid_access(r[15]+16u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6662fe; /* mov.l @(0x10,r15),r1 */
L8c6662fe: *stop=0x8c6662feu;*macl=r[3]*r[2];goto L8c666300; /* mul.l r3,r2 */
L8c666300: *stop=0x8c666300u;if(!valid_access(r[15]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x2cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666302; /* mov.l @(0x2c,r15),r3 */
L8c666302: *stop=0x8c666302u;*sr=(*sr&~1u)|(r[3]==r[13]);goto L8c666304; /* cmp/eq r3,r13 */
L8c666304: *stop=0x8c666304u;r[0]=*sr&1u;goto L8c666306; /* movt r0 */
L8c666306: *stop=0x8c666306u;r[0]&=0x1u;goto L8c666308; /* and #0x1,r0 */
L8c666308: *stop=0x8c666308u;*sr=(*sr&~1u)|(r[0]&1u);r[0]=(r[0]>>1)|(r[0]<<31);goto L8c66630a; /* rotr r0 */
L8c66630a: *stop=0x8c66630au;r[3]=r[0];goto L8c66630c; /* mov r0,r3 */
L8c66630c: *stop=0x8c66630cu;r[0]=r[2];goto L8c66630e; /* mov r2,r0 */
L8c66630e: *stop=0x8c66630eu;r[0]&=0x3fu;goto L8c666310; /* and #0x3f,r0 */
L8c666310: *stop=0x8c666310u;if(!valid_access(r[15]+52u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x34u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666312; /* mov.l @(0x34,r15),r2 */
L8c666312: *stop=0x8c666312u;r[5]=*macl;goto L8c666314; /* sts MACL,r5 */
L8c666314: *stop=0x8c666314u;r[0]<<=8;goto L8c666316; /* shll8 r0 */
L8c666316: *stop=0x8c666316u;r[3]|=r[0];goto L8c666318; /* or r0,r3 */
L8c666318: *stop=0x8c666318u;r[0]=r[1];goto L8c66631a; /* mov r1,r0 */
L8c66631a: *stop=0x8c66631au;r[5]+=r[1];goto L8c66631c; /* add r1,r5 */
L8c66631c: *stop=0x8c66631cu;r[0]&=0x3fu;goto L8c66631e; /* and #0x3f,r0 */
L8c66631e: *stop=0x8c66631eu;r[0]<<=2;goto L8c666320; /* shll2 r0 */
L8c666320: *stop=0x8c666320u;r[3]|=r[0];goto L8c666322; /* or r0,r3 */
L8c666322: *stop=0x8c666322u;r[0]=sx(m,bus,&io_status,0x8c6663a0u,2);if(io_status)return io_status;goto L8c666324; /* mov.w 0x8c6663a0,r0 */
L8c666324: *stop=0x8c666324u;r[3]|=r[2];goto L8c666326; /* or r2,r3 */
L8c666326: *stop=0x8c666326u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c666328; /* mov.l r3,@(r0,r15) */
L8c666328: *stop=0x8c666328u;r[0]=76u;goto L8c66632a; /* mov #0x4c,r0 */
L8c66632a: *stop=0x8c66632au;if(!valid_access(r[15]+56u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x38u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c66632c; /* mov.l @(0x38,r15),r1 */
L8c66632c: *stop=0x8c66632cu;if(!valid_access(r[15]+28u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66632e; /* mov.l @(0x1c,r15),r3 */
L8c66632e: *stop=0x8c66632eu;*macl=r[5]*r[1];goto L8c666330; /* mul.l r5,r1 */
L8c666330: *stop=0x8c666330u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c666332; /* mov.l @(r0,r15),r0 */
L8c666332: *stop=0x8c666332u;r[1]=*macl;goto L8c666334; /* sts MACL,r1 */
L8c666334: *stop=0x8c666334u;r[1]+=r[3];goto L8c666336; /* add r3,r1 */
L8c666336: *stop=0x8c666336u;r[1]|=r[0];goto L8c666338; /* or r0,r1 */
L8c666338: *stop=0x8c666338u;r[1]|=r[4];goto L8c66633a; /* or r4,r1 */
L8c66633a: *stop=0x8c66633au;r[0]=sx(m,bus,&io_status,0x8c6663a2u,2);if(io_status)return io_status;goto L8c66633c; /* mov.w 0x8c6663a2,r0 */
L8c66633c: *stop=0x8c66633cu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;goto L8c66633e; /* mov.l r1,@(r0,r15) */
L8c66633e: *stop=0x8c66633eu;r[0]=64u;goto L8c666340; /* mov #0x40,r0 */
L8c666340: *stop=0x8c666340u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c666342; /* mov.l @(r0,r15),r2 */
L8c666342: *stop=0x8c666342u;r[0]=92u;goto L8c666344; /* mov #0x5c,r0 */
L8c666344: *stop=0x8c666344u;if(!valid_access(r[15]+24u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c666346; /* mov.l @(0x18,r15),r1 */
L8c666346: *stop=0x8c666346u;*macl=r[5]*r[2];goto L8c666348; /* mul.l r5,r2 */
L8c666348: *stop=0x8c666348u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66634a; /* mov.l @(r0,r15),r0 */
L8c66634a: *stop=0x8c66634au;r[2]=*macl;goto L8c66634c; /* sts MACL,r2 */
L8c66634c: *stop=0x8c66634cu;r[2]+=r[1];goto L8c66634e; /* add r1,r2 */
L8c66634e: *stop=0x8c66634eu;r[2]|=r[0];goto L8c666350; /* or r0,r2 */
L8c666350: *stop=0x8c666350u;r[0]=sx(m,bus,&io_status,0x8c6663a4u,2);if(io_status)return io_status;goto L8c666352; /* mov.w 0x8c6663a4,r0 */
L8c666352: *stop=0x8c666352u;r[2]|=r[4];goto L8c666354; /* or r4,r2 */
L8c666354: *stop=0x8c666354u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c666356; /* mov.l r2,@(r0,r15) */
L8c666356: *stop=0x8c666356u;r[0]=72u;goto L8c666358; /* mov #0x48,r0 */
L8c666358: *stop=0x8c666358u;if(!valid_access(r[15]+60u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x3cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66635a; /* mov.l @(0x3c,r15),r3 */
L8c66635a: *stop=0x8c66635au;if(!valid_access(r[15]+36u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x24u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c66635c; /* mov.l @(0x24,r15),r2 */
L8c66635c: *stop=0x8c66635cu;*macl=r[5]*r[3];goto L8c66635e; /* mul.l r5,r3 */
L8c66635e: *stop=0x8c66635eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c666360; /* mov.l @(r0,r15),r0 */
L8c666360: *stop=0x8c666360u;r[3]=*macl;goto L8c666362; /* sts MACL,r3 */
L8c666362: *stop=0x8c666362u;r[3]+=r[2];goto L8c666364; /* add r2,r3 */
L8c666364: *stop=0x8c666364u;r[3]|=r[0];goto L8c666366; /* or r0,r3 */
L8c666366: *stop=0x8c666366u;r[0]=sx(m,bus,&io_status,0x8c6663a6u,2);if(io_status)return io_status;goto L8c666368; /* mov.w 0x8c6663a6,r0 */
L8c666368: *stop=0x8c666368u;r[3]|=r[4];goto L8c66636a; /* or r4,r3 */
L8c66636a: *stop=0x8c66636au;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c66636c; /* mov.l r3,@(r0,r15) */
L8c66636c: *stop=0x8c66636cu;r[0]=84u;goto L8c66636e; /* mov #0x54,r0 */
L8c66636e: *stop=0x8c66636eu;if(!valid_access(r[15]+48u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x30u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c666370; /* mov.l @(0x30,r15),r1 */
L8c666370: *stop=0x8c666370u;if(!valid_access(r[15]+32u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x20u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666372; /* mov.l @(0x20,r15),r3 */
L8c666372: *stop=0x8c666372u;*macl=r[5]*r[1];goto L8c666374; /* mul.l r5,r1 */
L8c666374: *stop=0x8c666374u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c666376; /* mov.l @(r0,r15),r0 */
L8c666376: *stop=0x8c666376u;r[1]=*macl;goto L8c666378; /* sts MACL,r1 */
L8c666378: *stop=0x8c666378u;r[1]+=r[3];goto L8c66637a; /* add r3,r1 */
L8c66637a: *stop=0x8c66637au;r[1]|=r[0];goto L8c66637c; /* or r0,r1 */
L8c66637c: *stop=0x8c66637cu;r[0]=sx(m,bus,&io_status,0x8c6663a8u,2);if(io_status)return io_status;goto L8c66637e; /* mov.w 0x8c6663a8,r0 */
L8c66637e: *stop=0x8c66637eu;r[1]|=r[4];goto L8c666380; /* or r4,r1 */
L8c666380: *stop=0x8c666380u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;goto L8c666382; /* mov.l r1,@(r0,r15) */
L8c666382: *stop=0x8c666382u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6663c4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c666384; /* mov.l 0x8c6663c4,r1 */
L8c666384: *stop=0x8c666384u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c666386; /* mov.l @r1,r2 */
L8c666386: *stop=0x8c666386u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c666388; /* tst r2,r2 */
L8c666388: if((*sr&1u))goto L8c6663e0;goto L8c66638a; /* bt 0x8c6663e0 */
L8c66638a: *stop=0x8c66638au;r[0]=sx(m,bus,&io_status,0x8c6663a6u,2);if(io_status)return io_status;goto L8c66638c; /* mov.w 0x8c6663a6,r0 */
L8c66638c: *stop=0x8c66638cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66638e; /* mov.l @(r0,r15),r2 */
L8c66638e: *stop=0x8c66638eu;r[0]=sx(m,bus,&io_status,0x8c6663aau,2);if(io_status)return io_status;goto L8c666390; /* mov.w 0x8c6663aa,r0 */
L8c666390: *stop=0x8c666390u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c666392; /* mov.l r2,@(r0,r15) */
L8c666392: *stop=0x8c666394u;goto L8c6663f8; /* bra 0x8c6663f8 */
L8c6663e0: *stop=0x8c6663e0u;r[0]=68u;goto L8c6663e2; /* mov #0x44,r0 */
L8c6663e2: *stop=0x8c6663e2u;if(!valid_access(r[15]+40u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6663e4; /* mov.l @(0x28,r15),r3 */
L8c6663e4: *stop=0x8c6663e4u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6663e6; /* mov.l @(r0,r15),r2 */
L8c6663e6: *stop=0x8c6663e6u;r[0]=88u;goto L8c6663e8; /* mov #0x58,r0 */
L8c6663e8: *stop=0x8c6663e8u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6663ea; /* mov.l @(r0,r15),r1 */
L8c6663ea: *stop=0x8c6663eau;*macl=r[5]*r[2];goto L8c6663ec; /* mul.l r5,r2 */
L8c6663ec: *stop=0x8c6663ecu;r[0]=sx(m,bus,&io_status,0x8c666420u,2);if(io_status)return io_status;goto L8c6663ee; /* mov.w 0x8c666420,r0 */
L8c6663ee: *stop=0x8c6663eeu;r[5]=*macl;goto L8c6663f0; /* sts MACL,r5 */
L8c6663f0: *stop=0x8c6663f0u;r[5]+=r[3];goto L8c6663f2; /* add r3,r5 */
L8c6663f2: *stop=0x8c6663f2u;r[5]|=r[1];goto L8c6663f4; /* or r1,r5 */
L8c6663f4: *stop=0x8c6663f4u;r[5]|=r[4];goto L8c6663f6; /* or r4,r5 */
L8c6663f6: *stop=0x8c6663f6u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[5],4);if(io_status)return io_status;goto L8c6663f8; /* mov.l r5,@(r0,r15) */
L8c6663f8: *stop=0x8c6663f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666428u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6663fa; /* mov.l 0x8c666428,r2 */
L8c6663fa: *stop=0x8c6663fau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6663fc; /* mov.l @r2,r3 */
L8c6663fc: *stop=0x8c6663fcu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6663fe; /* tst r3,r3 */
L8c6663fe: if((*sr&1u))goto L8c666448;goto L8c666400; /* bt 0x8c666448 */
L8c666400: *stop=0x8c666400u;{uint32_t loaded=rd(m,bus,&io_status,0x8c66642cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666402; /* mov.l 0x8c66642c,r3 */
L8c666402: *stop=0x8c666402u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c666404; /* mov.l @r3,r0 */
L8c666404: *stop=0x8c666404u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c666406; /* cmp/eq #0x1,r0 */
L8c666406: if(!(*sr&1u))goto L8c666440;goto L8c666408; /* bf 0x8c666440 */
L8c666408: *stop=0x8c666408u;r[0]=sx(m,bus,&io_status,0x8c666422u,2);if(io_status)return io_status;goto L8c66640a; /* mov.w 0x8c666422,r0 */
L8c66640a: *stop=0x8c66640au;r[1]=sx(m,bus,&io_status,0x8c666424u,2);if(io_status)return io_status;goto L8c66640c; /* mov.w 0x8c666424,r1 */
L8c66640c: *stop=0x8c66640cu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66640e; /* mov.l @(r0,r15),r0 */
L8c66640e: *stop=0x8c66640eu;r[1]+=r[15];goto L8c666410; /* add r15,r1 */
L8c666410: *stop=0x8c666410u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c666412; /* mov.l r0,@r1 */
L8c666412: *stop=0x8c666414u;goto L8c666448; /* bra 0x8c666448 */
L8c666440: *stop=0x8c666440u;r[0]=sx(m,bus,&io_status,0x8c6665a0u,2);if(io_status)return io_status;goto L8c666442; /* mov.w 0x8c6665a0,r0 */
L8c666442: *stop=0x8c666442u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c666444; /* mov.l @(r0,r15),r1 */
L8c666444: *stop=0x8c666444u;r[0]=sx(m,bus,&io_status,0x8c6665a2u,2);if(io_status)return io_status;goto L8c666446; /* mov.w 0x8c6665a2,r0 */
L8c666446: *stop=0x8c666446u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[1],4);if(io_status)return io_status;goto L8c666448; /* mov.l r1,@(r0,r15) */
L8c666448: *stop=0x8c666448u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6665c0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66644a; /* mov.l 0x8c6665c0,r3 */
L8c66644a: *stop=0x8c66644au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c66644c; /* mov.l @r3,r0 */
L8c66644c: *stop=0x8c66644cu;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c66644e; /* tst #0x20,r0 */
L8c66644e: if(!(*sr&1u))goto L8c666454;goto L8c666450; /* bf 0x8c666454 */
L8c666450: *stop=0x8c666452u;goto L8c666658; /* bra 0x8c666658 */
L8c666454: *stop=0x8c666454u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6665c4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c666456; /* mov.l 0x8c6665c4,r1 */
L8c666456: *stop=0x8c666456u;r[0]=sx(m,bus,&io_status,0x8c6665a4u,2);if(io_status)return io_status;goto L8c666458; /* mov.w 0x8c6665a4,r0 */
L8c666458: *stop=0x8c666458u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66645a; /* mov.l @r1,r2 */
L8c66645a: *stop=0x8c66645au;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c66645c; /* tst r2,r2 */
L8c66645c: take=!(*sr&1u);*stop=0x8c66645eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[4]=loaded;}if(take)goto L8c666464;goto L8c666460; /* bf/s 0x8c666464 */
L8c666460: *stop=0x8c666462u;goto L8c6665e0; /* bra 0x8c6665e0 */
L8c666464: *stop=0x8c666464u;r[0]=sx(m,bus,&io_status,0x8c6665a6u,2);if(io_status)return io_status;goto L8c666466; /* mov.w 0x8c6665a6,r0 */
L8c666466: *stop=0x8c666466u;r[3]=sx(m,bus,&io_status,0x8c6665a4u,2);if(io_status)return io_status;goto L8c666468; /* mov.w 0x8c6665a4,r3 */
L8c666468: *stop=0x8c666468u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[4],4);if(io_status)return io_status;goto L8c66646a; /* mov.l r4,@(r0,r15) */
L8c66646a: *stop=0x8c66646au;r[0]=120u;goto L8c66646c; /* mov #0x78,r0 */
L8c66646c: *stop=0x8c66646cu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[4],4);if(io_status)return io_status;goto L8c66646e; /* mov.l r4,@(r0,r15) */
L8c66646e: *stop=0x8c66646eu;r[0]=96u;goto L8c666470; /* mov #0x60,r0 */
L8c666470: *stop=0x8c666470u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[4],4);if(io_status)return io_status;goto L8c666472; /* mov.l r4,@(r0,r15) */
L8c666472: *stop=0x8c666472u;r[3]+=r[15];goto L8c666474; /* add r15,r3 */
L8c666474: *stop=0x8c666474u;r[0]=sx(m,bus,&io_status,0x8c6665a4u,2);if(io_status)return io_status;goto L8c666476; /* mov.w 0x8c6665a4,r0 */
L8c666476: *stop=0x8c666476u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c666478; /* mov.l @(r0,r15),r2 */
L8c666478: *stop=0x8c666478u;r[0]=sx(m,bus,&io_status,0x8c6665a4u,2);if(io_status)return io_status;goto L8c66647a; /* mov.w 0x8c6665a4,r0 */
L8c66647a: *stop=0x8c66647au;r[2]&=r[11];goto L8c66647c; /* and r11,r2 */
L8c66647c: *stop=0x8c66647cu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c66647e; /* mov.l r2,@(r0,r15) */
L8c66647e: *stop=0x8c66647eu;r[0]=sx(m,bus,&io_status,0x8c6665a6u,2);if(io_status)return io_status;goto L8c666480; /* mov.w 0x8c6665a6,r0 */
L8c666480: *stop=0x8c666480u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c666482; /* mov.l @(r0,r15),r2 */
L8c666482: *stop=0x8c666482u;r[0]=sx(m,bus,&io_status,0x8c6665a6u,2);if(io_status)return io_status;goto L8c666484; /* mov.w 0x8c6665a6,r0 */
L8c666484: *stop=0x8c666484u;r[2]&=r[11];goto L8c666486; /* and r11,r2 */
L8c666486: *stop=0x8c666486u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c666488; /* mov.l r2,@(r0,r15) */
L8c666488: *stop=0x8c666488u;r[0]=120u;goto L8c66648a; /* mov #0x78,r0 */
L8c66648a: *stop=0x8c66648au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66648c; /* mov.l @(r0,r15),r2 */
L8c66648c: *stop=0x8c66648cu;r[0]=120u;goto L8c66648e; /* mov #0x78,r0 */
L8c66648e: *stop=0x8c66648eu;r[2]&=r[11];goto L8c666490; /* and r11,r2 */
L8c666490: *stop=0x8c666490u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c666492; /* mov.l r2,@(r0,r15) */
L8c666492: *stop=0x8c666492u;r[0]=sx(m,bus,&io_status,0x8c6665a4u,2);if(io_status)return io_status;goto L8c666494; /* mov.w 0x8c6665a4,r0 */
L8c666494: *stop=0x8c666494u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6665c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666496; /* mov.l 0x8c6665c8,r2 */
L8c666496: *stop=0x8c666496u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c666498; /* mov.l @(r0,r15),r0 */
L8c666498: *stop=0x8c666498u;r[0]&=r[2];goto L8c66649a; /* and r2,r0 */
L8c66649a: *stop=0x8c66649au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c66649c; /* mov.l r0,@r3 */
L8c66649c: *stop=0x8c66649cu;r[0]=sx(m,bus,&io_status,0x8c6665a6u,2);if(io_status)return io_status;goto L8c66649e; /* mov.w 0x8c6665a6,r0 */
L8c66649e: *stop=0x8c66649eu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6664a0; /* mov.l @(r0,r15),r3 */
L8c6664a0: *stop=0x8c6664a0u;r[0]=sx(m,bus,&io_status,0x8c6665a6u,2);if(io_status)return io_status;goto L8c6664a2; /* mov.w 0x8c6665a6,r0 */
L8c6664a2: *stop=0x8c6664a2u;r[3]|=r[10];goto L8c6664a4; /* or r10,r3 */
L8c6664a4: *stop=0x8c6664a4u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c6664a6; /* mov.l r3,@(r0,r15) */
L8c6664a6: *stop=0x8c6664a6u;r[0]=120u;goto L8c6664a8; /* mov #0x78,r0 */
L8c6664a8: *stop=0x8c6664a8u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6664aa; /* mov.l @(r0,r15),r3 */
L8c6664aa: *stop=0x8c6664aau;r[0]=120u;goto L8c6664ac; /* mov #0x78,r0 */
L8c6664ac: *stop=0x8c6664acu;r[1]=r[15];goto L8c6664ae; /* mov r15,r1 */
L8c6664ae: *stop=0x8c6664aeu;r[1]+=96u;goto L8c6664b0; /* add #0x60,r1 */
L8c6664b0: *stop=0x8c6664b0u;r[3]|=r[10];goto L8c6664b2; /* or r10,r3 */
L8c6664b2: *stop=0x8c6664b2u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c6664b4; /* mov.l r3,@(r0,r15) */
L8c6664b4: *stop=0x8c6664b4u;r[0]=96u;goto L8c6664b6; /* mov #0x60,r0 */
L8c6664b6: *stop=0x8c6664b6u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6664b8; /* mov.l @(r0,r15),r3 */
L8c6664b8: *stop=0x8c6664b8u;r[0]=96u;goto L8c6664ba; /* mov #0x60,r0 */
L8c6664ba: *stop=0x8c6664bau;r[3]|=r[10];goto L8c6664bc; /* or r10,r3 */
L8c6664bc: *stop=0x8c6664bcu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c6664be; /* mov.l r3,@(r0,r15) */
L8c6664be: *stop=0x8c6664beu;r[0]=sx(m,bus,&io_status,0x8c6665a4u,2);if(io_status)return io_status;goto L8c6664c0; /* mov.w 0x8c6665a4,r0 */
L8c6664c0: *stop=0x8c6664c0u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6664c2; /* mov.l @(r0,r15),r3 */
L8c6664c2: *stop=0x8c6664c2u;r[0]=sx(m,bus,&io_status,0x8c6665a4u,2);if(io_status)return io_status;goto L8c6664c4; /* mov.w 0x8c6665a4,r0 */
L8c6664c4: *stop=0x8c6664c4u;r[3]|=r[9];goto L8c6664c6; /* or r9,r3 */
L8c6664c6: *stop=0x8c6664c6u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c6664c8; /* mov.l r3,@(r0,r15) */
L8c6664c8: *stop=0x8c6664c8u;r[0]=sx(m,bus,&io_status,0x8c6665a6u,2);if(io_status)return io_status;goto L8c6664ca; /* mov.w 0x8c6665a6,r0 */
L8c6664ca: *stop=0x8c6664cau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6664cc; /* mov.l @(r0,r15),r3 */
L8c6664cc: *stop=0x8c6664ccu;r[0]=sx(m,bus,&io_status,0x8c6665a6u,2);if(io_status)return io_status;goto L8c6664ce; /* mov.w 0x8c6665a6,r0 */
L8c6664ce: *stop=0x8c6664ceu;r[3]|=r[9];goto L8c6664d0; /* or r9,r3 */
L8c6664d0: *stop=0x8c6664d0u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c6664d2; /* mov.l r3,@(r0,r15) */
L8c6664d2: *stop=0x8c6664d2u;r[0]=120u;goto L8c6664d4; /* mov #0x78,r0 */
L8c6664d4: *stop=0x8c6664d4u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6664d6; /* mov.l @(r0,r15),r3 */
L8c6664d6: *stop=0x8c6664d6u;r[0]=120u;goto L8c6664d8; /* mov #0x78,r0 */
L8c6664d8: *stop=0x8c6664d8u;r[3]|=r[9];goto L8c6664da; /* or r9,r3 */
L8c6664da: *stop=0x8c6664dau;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c6664dc; /* mov.l r3,@(r0,r15) */
L8c6664dc: *stop=0x8c6664dcu;r[0]=96u;goto L8c6664de; /* mov #0x60,r0 */
L8c6664de: *stop=0x8c6664deu;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6664e0; /* mov.l @(r0,r15),r0 */
L8c6664e0: *stop=0x8c6664e0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6665ccu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6664e2; /* mov.l 0x8c6665cc,r3 */
L8c6664e2: *stop=0x8c6664e2u;r[0]&=r[3];goto L8c6664e4; /* and r3,r0 */
L8c6664e4: *stop=0x8c6664e4u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c6664e6; /* mov.l r0,@r1 */
L8c6664e6: *stop=0x8c6664e6u;r[0]=sx(m,bus,&io_status,0x8c6665a6u,2);if(io_status)return io_status;goto L8c6664e8; /* mov.w 0x8c6665a6,r0 */
L8c6664e8: *stop=0x8c6664e8u;r[2]=sx(m,bus,&io_status,0x8c6665a6u,2);if(io_status)return io_status;goto L8c6664ea; /* mov.w 0x8c6665a6,r2 */
L8c6664ea: *stop=0x8c6664eau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6664ec; /* mov.l @(r0,r15),r0 */
L8c6664ec: *stop=0x8c6664ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6665d0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6664ee; /* mov.l 0x8c6665d0,r1 */
L8c6664ee: *stop=0x8c6664eeu;r[2]+=r[15];goto L8c6664f0; /* add r15,r2 */
L8c6664f0: *stop=0x8c6664f0u;r[0]&=r[1];goto L8c6664f2; /* and r1,r0 */
L8c6664f2: *stop=0x8c6664f2u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c6664f4; /* mov.l r0,@r2 */
L8c6664f4: *stop=0x8c6664f4u;r[0]=80u;goto L8c6664f6; /* mov #0x50,r0 */
L8c6664f6: *stop=0x8c6664f6u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6664f8; /* mov.l @(r0,r15),r2 */
L8c6664f8: *stop=0x8c6664f8u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6664fa; /* tst r2,r2 */
L8c6664fa: if((*sr&1u))goto L8c666502;goto L8c6664fc; /* bt 0x8c666502 */
L8c6664fc: *stop=0x8c6664fcu;r[0]=sx(m,bus,&io_status,0x8c6665a8u,2);if(io_status)return io_status;goto L8c6664fe; /* mov.w 0x8c6665a8,r0 */
L8c6664fe: *stop=0x8c6664feu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c666500; /* mov.l @(0x14,r15),r2 */
L8c666500: *stop=0x8c666500u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c666502; /* mov.l r2,@(r0,r15) */
L8c666502: *stop=0x8c666502u;r[0]=sx(m,bus,&io_status,0x8c6665aau,2);if(io_status)return io_status;goto L8c666504; /* mov.w 0x8c6665aa,r0 */
L8c666504: *stop=0x8c666504u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666506; /* mov.l r14,@(r0,r15) */
L8c666506: *stop=0x8c666506u;r[0]=sx(m,bus,&io_status,0x8c6665acu,2);if(io_status)return io_status;goto L8c666508; /* mov.w 0x8c6665ac,r0 */
L8c666508: *stop=0x8c666508u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c66650a; /* mov.l r14,@(r0,r15) */
L8c66650a: *stop=0x8c66650au;r[0]=sx(m,bus,&io_status,0x8c6665aeu,2);if(io_status)return io_status;goto L8c66650c; /* mov.w 0x8c6665ae,r0 */
L8c66650c: *stop=0x8c66650cu;if(!valid_access(r[15]+20u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c66650e; /* mov.l @(0x14,r15),r3 */
L8c66650e: *stop=0x8c66650eu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c666510; /* mov.l r3,@(r0,r15) */
L8c666510: *stop=0x8c666510u;r[0]=sx(m,bus,&io_status,0x8c6665b0u,2);if(io_status)return io_status;goto L8c666512; /* mov.w 0x8c6665b0,r0 */
L8c666512: *stop=0x8c666512u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666514; /* mov.l r14,@(r0,r15) */
L8c666514: *stop=0x8c666514u;r[0]=sx(m,bus,&io_status,0x8c6665b2u,2);if(io_status)return io_status;goto L8c666516; /* mov.w 0x8c6665b2,r0 */
L8c666516: *stop=0x8c666516u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666518; /* mov.l r14,@(r0,r15) */
L8c666518: *stop=0x8c666518u;r[0]=124u;goto L8c66651a; /* mov #0x7c,r0 */
L8c66651a: *stop=0x8c66651au;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c66651c; /* mov.l r14,@(r0,r15) */
L8c66651c: *stop=0x8c66651cu;r[0]=sx(m,bus,&io_status,0x8c6665b4u,2);if(io_status)return io_status;goto L8c66651e; /* mov.w 0x8c6665b4,r0 */
L8c66651e: *stop=0x8c66651eu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666520; /* mov.l r14,@(r0,r15) */
L8c666520: *stop=0x8c666520u;r[0]=sx(m,bus,&io_status,0x8c6665b6u,2);if(io_status)return io_status;goto L8c666522; /* mov.w 0x8c6665b6,r0 */
L8c666522: *stop=0x8c666522u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666524; /* mov.l @(r0,r15),r3 */
L8c666524: *stop=0x8c666524u;r[0]=sx(m,bus,&io_status,0x8c6665b8u,2);if(io_status)return io_status;goto L8c666526; /* mov.w 0x8c6665b8,r0 */
L8c666526: *stop=0x8c666526u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c666528; /* mov.l r3,@(r0,r15) */
L8c666528: *stop=0x8c666528u;r[0]=sx(m,bus,&io_status,0x8c6665a0u,2);if(io_status)return io_status;goto L8c66652a; /* mov.w 0x8c6665a0,r0 */
L8c66652a: *stop=0x8c66652au;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c66652c; /* mov.l @(r0,r15),r2 */
L8c66652c: *stop=0x8c66652cu;r[0]=sx(m,bus,&io_status,0x8c6665bau,2);if(io_status)return io_status;goto L8c66652e; /* mov.w 0x8c6665ba,r0 */
L8c66652e: *stop=0x8c66652eu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c666530; /* mov.l r2,@(r0,r15) */
L8c666530: *stop=0x8c666530u;r[0]=sx(m,bus,&io_status,0x8c6665bcu,2);if(io_status)return io_status;goto L8c666532; /* mov.w 0x8c6665bc,r0 */
L8c666532: *stop=0x8c666532u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666534; /* mov.l r14,@(r0,r15) */
L8c666534: *stop=0x8c666534u;r[0]=sx(m,bus,&io_status,0x8c6665b6u,2);if(io_status)return io_status;goto L8c666536; /* mov.w 0x8c6665b6,r0 */
L8c666536: *stop=0x8c666536u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666538; /* mov.l r14,@(r0,r15) */
L8c666538: *stop=0x8c666538u;r[0]=sx(m,bus,&io_status,0x8c6665a0u,2);if(io_status)return io_status;goto L8c66653a; /* mov.w 0x8c6665a0,r0 */
L8c66653a: *stop=0x8c66653au;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c66653c; /* mov.l r14,@(r0,r15) */
L8c66653c: *stop=0x8c66653cu;r[0]=100u;goto L8c66653e; /* mov #0x64,r0 */
L8c66653e: *stop=0x8c66653eu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666540; /* mov.l r14,@(r0,r15) */
L8c666540: *stop=0x8c666540u;r[0]=104u;goto L8c666542; /* mov #0x68,r0 */
L8c666542: *stop=0x8c666542u;r[3]=r[13];goto L8c666544; /* mov r13,r3 */
L8c666544: *stop=0x8c666544u;r[2]=r[13];goto L8c666546; /* mov r13,r2 */
L8c666546: *stop=0x8c666546u;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c666548; /* shll r3 */
L8c666548: *stop=0x8c666548u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c66654a; /* mov.l r14,@(r0,r15) */
L8c66654a: *stop=0x8c66654au;r[0]=108u;goto L8c66654c; /* mov #0x6c,r0 */
L8c66654c: *stop=0x8c66654cu;r[5]=sx(m,bus,&io_status,0x8c6665a4u,2);if(io_status)return io_status;goto L8c66654e; /* mov.w 0x8c6665a4,r5 */
L8c66654e: *stop=0x8c66654eu;r[3]+=r[2];goto L8c666550; /* add r2,r3 */
L8c666550: *stop=0x8c666550u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666552; /* mov.l r14,@(r0,r15) */
L8c666552: *stop=0x8c666552u;r[3]<<=2;goto L8c666554; /* shll2 r3 */
L8c666554: *stop=0x8c666554u;r[0]=112u;goto L8c666556; /* mov #0x70,r0 */
L8c666556: *stop=0x8c666556u;r[3]<<=2;goto L8c666558; /* shll2 r3 */
L8c666558: *stop=0x8c666558u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c66655a; /* mov.l r14,@(r0,r15) */
L8c66655a: *stop=0x8c66655au;*sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c66655c; /* shll r3 */
L8c66655c: *stop=0x8c66655cu;r[0]=116u;goto L8c66655e; /* mov #0x74,r0 */
L8c66655e: *stop=0x8c66655eu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666560; /* mov.l r14,@(r0,r15) */
L8c666560: *stop=0x8c666560u;r[5]+=r[15];goto L8c666562; /* add r15,r5 */
L8c666562: *stop=0x8c666562u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c666564; /* mov.l @(0xc,r15),r1 */
L8c666564: *stop=0x8c666564u;r[6]=24u;goto L8c666566; /* mov #0x18,r6 */
L8c666566: *stop=0x8c666566u;r[3]+=r[1];goto L8c666568; /* add r1,r3 */
L8c666568: *stop=0x8c666568u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c66656a; /* mov.l r3,@r15 */
L8c66656a: target=r[12];*pr=0x8c66656eu;*stop=0x8c66656cu;r[4]=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66656e; /* jsr @r12 */
L8c66656e: *stop=0x8c66656eu;r[5]=sx(m,bus,&io_status,0x8c6665a6u,2);if(io_status)return io_status;goto L8c666570; /* mov.w 0x8c6665a6,r5 */
L8c666570: *stop=0x8c666570u;r[6]=24u;goto L8c666572; /* mov #0x18,r6 */
L8c666572: *stop=0x8c666572u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c666574; /* mov.l @r15,r4 */
L8c666574: *stop=0x8c666574u;r[5]+=r[15];goto L8c666576; /* add r15,r5 */
L8c666576: target=r[12];*pr=0x8c66657au;*stop=0x8c666578u;r[4]+=24u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c66657a; /* jsr @r12 */
L8c66657a: *stop=0x8c66657au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c66657c; /* mov.l @r15,r4 */
L8c66657c: *stop=0x8c66657cu;r[5]=r[15];goto L8c66657e; /* mov r15,r5 */
L8c66657e: *stop=0x8c66657eu;r[6]=24u;goto L8c666580; /* mov #0x18,r6 */
L8c666580: *stop=0x8c666580u;r[5]+=120u;goto L8c666582; /* add #0x78,r5 */
L8c666582: target=r[12];*pr=0x8c666586u;*stop=0x8c666584u;r[4]+=48u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666586; /* jsr @r12 */
L8c666586: *stop=0x8c666586u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c666588; /* mov.l @r15,r4 */
L8c666588: *stop=0x8c666588u;r[5]=r[15];goto L8c66658a; /* mov r15,r5 */
L8c66658a: *stop=0x8c66658au;r[6]=24u;goto L8c66658c; /* mov #0x18,r6 */
L8c66658c: *stop=0x8c66658cu;r[5]+=96u;goto L8c66658e; /* add #0x60,r5 */
L8c66658e: *stop=0x8c666590u;r[4]+=72u;goto L8c666650; /* bra 0x8c666650 */
L8c6665e0: *stop=0x8c6665e0u;r[0]=sx(m,bus,&io_status,0x8c6666a0u,2);if(io_status)return io_status;goto L8c6665e2; /* mov.w 0x8c6666a0,r0 */
L8c6665e2: *stop=0x8c6665e2u;r[1]=sx(m,bus,&io_status,0x8c6666a0u,2);if(io_status)return io_status;goto L8c6665e4; /* mov.w 0x8c6666a0,r1 */
L8c6665e4: *stop=0x8c6665e4u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[4],4);if(io_status)return io_status;goto L8c6665e6; /* mov.l r4,@(r0,r15) */
L8c6665e6: *stop=0x8c6665e6u;r[0]=sx(m,bus,&io_status,0x8c6666a2u,2);if(io_status)return io_status;goto L8c6665e8; /* mov.w 0x8c6666a2,r0 */
L8c6665e8: *stop=0x8c6665e8u;r[1]+=r[15];goto L8c6665ea; /* add r15,r1 */
L8c6665ea: *stop=0x8c6665eau;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6665ec; /* mov.l @(r0,r15),r2 */
L8c6665ec: *stop=0x8c6665ecu;r[0]=sx(m,bus,&io_status,0x8c6666a2u,2);if(io_status)return io_status;goto L8c6665ee; /* mov.w 0x8c6666a2,r0 */
L8c6665ee: *stop=0x8c6665eeu;r[2]&=r[11];goto L8c6665f0; /* and r11,r2 */
L8c6665f0: *stop=0x8c6665f0u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[2],4);if(io_status)return io_status;goto L8c6665f2; /* mov.l r2,@(r0,r15) */
L8c6665f2: *stop=0x8c6665f2u;r[0]=sx(m,bus,&io_status,0x8c6666a2u,2);if(io_status)return io_status;goto L8c6665f4; /* mov.w 0x8c6666a2,r0 */
L8c6665f4: *stop=0x8c6665f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6666b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6665f6; /* mov.l 0x8c6666b0,r2 */
L8c6665f6: *stop=0x8c6665f6u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6665f8; /* mov.l @(r0,r15),r0 */
L8c6665f8: *stop=0x8c6665f8u;r[0]&=r[2];goto L8c6665fa; /* and r2,r0 */
L8c6665fa: *stop=0x8c6665fau;r[3]=r[0];goto L8c6665fc; /* mov r0,r3 */
L8c6665fc: *stop=0x8c6665fcu;r[0]=sx(m,bus,&io_status,0x8c6666a2u,2);if(io_status)return io_status;goto L8c6665fe; /* mov.w 0x8c6666a2,r0 */
L8c6665fe: *stop=0x8c6665feu;r[3]|=r[9];goto L8c666600; /* or r9,r3 */
L8c666600: *stop=0x8c666600u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c666602; /* mov.l r3,@(r0,r15) */
L8c666602: *stop=0x8c666602u;r[0]=sx(m,bus,&io_status,0x8c6666a0u,2);if(io_status)return io_status;goto L8c666604; /* mov.w 0x8c6666a0,r0 */
L8c666604: *stop=0x8c666604u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c666606; /* mov.l @(r0,r15),r3 */
L8c666606: *stop=0x8c666606u;r[0]=sx(m,bus,&io_status,0x8c6666a0u,2);if(io_status)return io_status;goto L8c666608; /* mov.w 0x8c6666a0,r0 */
L8c666608: *stop=0x8c666608u;r[3]|=r[10];goto L8c66660a; /* or r10,r3 */
L8c66660a: *stop=0x8c66660au;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[3],4);if(io_status)return io_status;goto L8c66660c; /* mov.l r3,@(r0,r15) */
L8c66660c: *stop=0x8c66660cu;r[0]=sx(m,bus,&io_status,0x8c6666a0u,2);if(io_status)return io_status;goto L8c66660e; /* mov.w 0x8c6666a0,r0 */
L8c66660e: *stop=0x8c66660eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6666b4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666610; /* mov.l 0x8c6666b4,r3 */
L8c666610: *stop=0x8c666610u;if(!valid_access(r[0]+r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c666612; /* mov.l @(r0,r15),r0 */
L8c666612: *stop=0x8c666612u;r[0]&=r[3];goto L8c666614; /* and r3,r0 */
L8c666614: *stop=0x8c666614u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c666616; /* mov.l r0,@r1 */
L8c666616: *stop=0x8c666616u;r[0]=sx(m,bus,&io_status,0x8c6666a4u,2);if(io_status)return io_status;goto L8c666618; /* mov.w 0x8c6666a4,r0 */
L8c666618: *stop=0x8c666618u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c66661a; /* mov.l r14,@(r0,r15) */
L8c66661a: *stop=0x8c66661au;r[0]=sx(m,bus,&io_status,0x8c6666a6u,2);if(io_status)return io_status;goto L8c66661c; /* mov.w 0x8c6666a6,r0 */
L8c66661c: *stop=0x8c66661cu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c66661e; /* mov.l r14,@(r0,r15) */
L8c66661e: *stop=0x8c66661eu;r[0]=sx(m,bus,&io_status,0x8c6666a8u,2);if(io_status)return io_status;goto L8c666620; /* mov.w 0x8c6666a8,r0 */
L8c666620: *stop=0x8c666620u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666622; /* mov.l r14,@(r0,r15) */
L8c666622: *stop=0x8c666622u;r[1]=r[13];goto L8c666624; /* mov r13,r1 */
L8c666624: *stop=0x8c666624u;r[0]=sx(m,bus,&io_status,0x8c6666aau,2);if(io_status)return io_status;goto L8c666626; /* mov.w 0x8c6666aa,r0 */
L8c666626: *stop=0x8c666626u;*sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c666628; /* shll r1 */
L8c666628: *stop=0x8c666628u;r[5]=sx(m,bus,&io_status,0x8c6666a2u,2);if(io_status)return io_status;goto L8c66662a; /* mov.w 0x8c6666a2,r5 */
L8c66662a: *stop=0x8c66662au;r[6]=24u;goto L8c66662c; /* mov #0x18,r6 */
L8c66662c: *stop=0x8c66662cu;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c66662e; /* mov.l r14,@(r0,r15) */
L8c66662e: *stop=0x8c66662eu;r[0]=sx(m,bus,&io_status,0x8c6666acu,2);if(io_status)return io_status;goto L8c666630; /* mov.w 0x8c6666ac,r0 */
L8c666630: *stop=0x8c666630u;r[5]+=r[15];goto L8c666632; /* add r15,r5 */
L8c666632: *stop=0x8c666632u;if(!valid_access(r[0]+r[15],4u))return -3;wr(m,bus,&io_status,r[15]+r[0],r[14],4);if(io_status)return io_status;goto L8c666634; /* mov.l r14,@(r0,r15) */
L8c666634: *stop=0x8c666634u;r[0]=r[13];goto L8c666636; /* mov r13,r0 */
L8c666636: *stop=0x8c666636u;r[1]+=r[0];goto L8c666638; /* add r0,r1 */
L8c666638: *stop=0x8c666638u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c66663a; /* mov.l @(0xc,r15),r0 */
L8c66663a: *stop=0x8c66663au;r[1]<<=2;goto L8c66663c; /* shll2 r1 */
L8c66663c: *stop=0x8c66663cu;r[1]<<=2;goto L8c66663e; /* shll2 r1 */
L8c66663e: *stop=0x8c66663eu;r[1]+=r[0];goto L8c666640; /* add r0,r1 */
L8c666640: *stop=0x8c666640u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c666642; /* mov.l r1,@r15 */
L8c666642: target=r[12];*pr=0x8c666646u;*stop=0x8c666644u;r[4]=r[1];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666646; /* jsr @r12 */
L8c666646: *stop=0x8c666646u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}goto L8c666648; /* mov.l @r15,r4 */
L8c666648: *stop=0x8c666648u;r[6]=24u;goto L8c66664a; /* mov #0x18,r6 */
L8c66664a: *stop=0x8c66664au;r[5]=sx(m,bus,&io_status,0x8c6666a0u,2);if(io_status)return io_status;goto L8c66664c; /* mov.w 0x8c6666a0,r5 */
L8c66664c: *stop=0x8c66664cu;r[4]+=24u;goto L8c66664e; /* add #0x18,r4 */
L8c66664e: *stop=0x8c66664eu;r[5]+=r[15];goto L8c666650; /* add r15,r5 */
L8c666650: target=r[12];*pr=0x8c666654u;*stop=0x8c666652u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666654; /* jsr @r12 */
L8c666654: *stop=0x8c666656u;goto L8c666670; /* bra 0x8c666670 */
L8c666658: *stop=0x8c666658u;r[4]=r[13];goto L8c66665a; /* mov r13,r4 */
L8c66665a: *stop=0x8c66665au;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c66665c; /* shll r4 */
L8c66665c: *stop=0x8c66665cu;r[2]=r[13];goto L8c66665e; /* mov r13,r2 */
L8c66665e: *stop=0x8c66665eu;r[4]+=r[2];goto L8c666660; /* add r2,r4 */
L8c666660: *stop=0x8c666660u;if(!valid_access(r[15]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c666662; /* mov.l @(0xc,r15),r1 */
L8c666662: *stop=0x8c666662u;r[4]<<=2;goto L8c666664; /* shll2 r4 */
L8c666664: *stop=0x8c666664u;r[5]=sx(m,bus,&io_status,0x8c6666a2u,2);if(io_status)return io_status;goto L8c666666; /* mov.w 0x8c6666a2,r5 */
L8c666666: *stop=0x8c666666u;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c666668; /* shll r4 */
L8c666668: *stop=0x8c666668u;r[6]=24u;goto L8c66666a; /* mov #0x18,r6 */
L8c66666a: *stop=0x8c66666au;r[4]+=r[1];goto L8c66666c; /* add r1,r4 */
L8c66666c: target=r[12];*pr=0x8c666670u;*stop=0x8c66666eu;r[5]+=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c666670; /* jsr @r12 */
L8c666670: *stop=0x8c666670u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c666672; /* mov.l @(0x4,r15),r3 */
L8c666672: *stop=0x8c666672u;r[13]+=1u;goto L8c666674; /* add #0x1,r13 */
L8c666674: *stop=0x8c666674u;*sr=(*sr&~1u)|(r[13]>=r[3]);goto L8c666676; /* cmp/hs r3,r13 */
L8c666676: if((*sr&1u))goto L8c66667c;goto L8c666678; /* bt 0x8c66667c */
L8c666678: *stop=0x8c66667au;goto L8c66623a; /* bra 0x8c66623a */
L8c66667c: *stop=0x8c66667cu;r[1]=sx(m,bus,&io_status,0x8c6666aeu,2);if(io_status)return io_status;goto L8c66667e; /* mov.w 0x8c6666ae,r1 */
L8c66667e: *stop=0x8c66667eu;r[15]+=r[1];goto L8c666680; /* add r1,r15 */
L8c666680: *stop=0x8c666680u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c666682; /* lds.l @r15+,PR */
L8c666682: *stop=0x8c666682u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c666684; /* mov.l @r15+,r8 */
L8c666684: *stop=0x8c666684u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c666686; /* mov.l @r15+,r9 */
L8c666686: *stop=0x8c666686u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c666688; /* mov.l @r15+,r10 */
L8c666688: *stop=0x8c666688u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c66668a; /* mov.l @r15+,r11 */
L8c66668a: *stop=0x8c66668au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c66668c; /* mov.l @r15+,r12 */
L8c66668c: *stop=0x8c66668cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c66668e; /* mov.l @r15+,r13 */
L8c66668e: *stop=0x8c66668eu;if(*pr!=entry_pr)return -3;*stop=0x8c666690u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_interrupt_8c651570(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c651570;
L8c651570: *stop=0x8c651570u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c651572; /* mov.l r14,@-r15 */
L8c651572: *stop=0x8c651572u;r[14]=0u;goto L8c651574; /* mov #0x0,r14 */
L8c651574: *stop=0x8c651574u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c651576; /* mov.l r13,@-r15 */
L8c651576: *stop=0x8c651576u;r[4]=1u;goto L8c651578; /* mov #0x1,r4 */
L8c651578: *stop=0x8c651578u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c65157a; /* mov.l r12,@-r15 */
L8c65157a: *stop=0x8c65157au;r[12]=4u;goto L8c65157c; /* mov #0x4,r12 */
L8c65157c: *stop=0x8c65157cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65166cu,4);if(io_status)return io_status;r[7]=loaded;}goto L8c65157e; /* mov.l 0x8c65166c,r7 */
L8c65157e: *stop=0x8c65157eu;r[5]=r[14];goto L8c651580; /* mov r14,r5 */
L8c651580: *stop=0x8c651580u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c651582; /* mov.l r11,@-r15 */
L8c651582: *stop=0x8c651582u;r[11]=7u;goto L8c651584; /* mov #0x7,r11 */
L8c651584: *stop=0x8c651584u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c651586; /* mov.l r10,@-r15 */
L8c651586: *stop=0x8c651586u;r[6]=r[7];goto L8c651588; /* mov r7,r6 */
L8c651588: *stop=0x8c651588u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651668u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c65158a; /* mov.l 0x8c651668,r13 */
L8c65158a: *stop=0x8c65158au;r[6]+=4294967292u;goto L8c65158c; /* add #-0x4,r6 */
L8c65158c: *stop=0x8c65158cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651664u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c65158e; /* mov.l 0x8c651664,r10 */
L8c65158e: *stop=0x8c65158eu;r[1]=r[5];goto L8c651590; /* mov r5,r1 */
L8c651590: *stop=0x8c651590u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651670u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651592; /* mov.l 0x8c651670,r0 */
L8c651592: *stop=0x8c651592u;r[1]<<=2;goto L8c651594; /* shll2 r1 */
L8c651594: *stop=0x8c651594u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651596; /* mov.l @(r0,r1),r0 */
L8c651596: *stop=0x8c651596u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c651598; /* cmp/eq #0x5,r0 */
L8c651598: if((*sr&1u))goto L8c6515a2;goto L8c65159a; /* bt 0x8c6515a2 */
L8c65159a: *stop=0x8c65159au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651670u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65159c; /* mov.l 0x8c651670,r0 */
L8c65159c: *stop=0x8c65159cu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65159e; /* mov.l @(r0,r1),r0 */
L8c65159e: *stop=0x8c65159eu;*sr=(*sr&~1u)|(r[0]==0xfffffffdu);goto L8c6515a0; /* cmp/eq #-0x3,r0 */
L8c6515a0: if(!(*sr&1u))goto L8c651622;goto L8c6515a2; /* bf 0x8c651622 */
L8c6515a2: *stop=0x8c6515a2u;r[12]=r[5];goto L8c6515a4; /* mov r5,r12 */
L8c6515a4: *stop=0x8c6515a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651670u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6515a6; /* mov.l 0x8c651670,r0 */
L8c6515a6: *stop=0x8c6515a6u;r[12]<<=2;goto L8c6515a8; /* shll2 r12 */
L8c6515a8: *stop=0x8c6515a8u;if(!valid_access(r[0]+r[12],4u))return -3;wr(m,bus,&io_status,r[12]+r[0],r[11],4);if(io_status)return io_status;goto L8c6515aa; /* mov.l r11,@(r0,r12) */
L8c6515aa: *stop=0x8c6515aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c651674u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6515ac; /* mov.l 0x8c651674,r3 */
L8c6515ac: *stop=0x8c6515acu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6515ae; /* mov.l @r3,r0 */
L8c6515ae: *stop=0x8c6515aeu;r[0]&=r[4];goto L8c6515b0; /* and r4,r0 */
L8c6515b0: *stop=0x8c6515b0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6515b2; /* cmp/eq #0x1,r0 */
L8c6515b2: if((*sr&1u))goto L8c6515dc;goto L8c6515b4; /* bt 0x8c6515dc */
L8c6515b4: *stop=0x8c6515b4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651674u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6515b6; /* mov.l 0x8c651674,r2 */
L8c6515b6: *stop=0x8c6515b6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6515b8; /* mov.l @r2,r0 */
L8c6515b8: *stop=0x8c6515b8u;r[0]&=0x2cu;goto L8c6515ba; /* and #0x2c,r0 */
L8c6515ba: *stop=0x8c6515bau;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c6515bc; /* cmp/eq #0x20,r0 */
L8c6515bc: if(!(*sr&1u))goto L8c6515dc;goto L8c6515be; /* bf 0x8c6515dc */
L8c6515be: *stop=0x8c6515beu;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6515c0; /* tst r4,r4 */
L8c6515c0: if((*sr&1u))goto L8c6515dc;goto L8c6515c2; /* bt 0x8c6515dc */
L8c6515c2: *stop=0x8c6515c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65167cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6515c4; /* mov.l 0x8c65167c,r2 */
L8c6515c4: *stop=0x8c6515c4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651678u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6515c6; /* mov.l 0x8c651678,r3 */
L8c6515c6: *stop=0x8c6515c6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6515c8; /* mov.l @r2,r1 */
L8c6515c8: *stop=0x8c6515c8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6515ca; /* mov.l @r3,r0 */
L8c6515ca: *stop=0x8c6515cau;r[0]+=r[1];goto L8c6515cc; /* add r1,r0 */
L8c6515cc: *stop=0x8c6515ccu;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[0],4);if(io_status)return io_status;goto L8c6515ce; /* mov.l r0,@r6 */
L8c6515ce: *stop=0x8c6515ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651684u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6515d0; /* mov.l 0x8c651684,r3 */
L8c6515d0: *stop=0x8c6515d0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651680u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6515d2; /* mov.l 0x8c651680,r1 */
L8c6515d2: *stop=0x8c6515d2u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6515d4; /* mov.l @r3,r2 */
L8c6515d4: *stop=0x8c6515d4u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6515d6; /* mov.l @r1,r0 */
L8c6515d6: *stop=0x8c6515d6u;r[0]+=r[2];goto L8c6515d8; /* add r2,r0 */
L8c6515d8: *stop=0x8c6515dau;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[0],4);if(io_status)return io_status;goto L8c651600; /* bra 0x8c651600 */
L8c6515dc: *stop=0x8c6515dcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651688u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6515de; /* mov.l 0x8c651688,r3 */
L8c6515de: *stop=0x8c6515deu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6515e0; /* mov.l @r3,r0 */
L8c6515e0: *stop=0x8c6515e0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6515e2; /* cmp/eq #0x1,r0 */
L8c6515e2: if((*sr&1u))goto L8c651600;goto L8c6515e4; /* bt 0x8c651600 */
L8c6515e4: *stop=0x8c6515e4u;r[11]=r[5];goto L8c6515e6; /* mov r5,r11 */
L8c6515e6: *stop=0x8c6515e6u;r[11]&=r[4];goto L8c6515e8; /* and r4,r11 */
L8c6515e8: *stop=0x8c6515e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651678u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6515ea; /* mov.l 0x8c651678,r0 */
L8c6515ea: *stop=0x8c6515eau;r[11]<<=2;goto L8c6515ec; /* shll2 r11 */
L8c6515ec: *stop=0x8c6515ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65167cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6515ee; /* mov.l 0x8c65167c,r1 */
L8c6515ee: *stop=0x8c6515eeu;if(!valid_access(r[0]+r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6515f0; /* mov.l @(r0,r11),r2 */
L8c6515f0: *stop=0x8c6515f0u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6515f2; /* mov.l @r1,r3 */
L8c6515f2: *stop=0x8c6515f2u;r[2]+=r[3];goto L8c6515f4; /* add r3,r2 */
L8c6515f4: *stop=0x8c6515f4u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c6515f6; /* mov.l r2,@r6 */
L8c6515f6: *stop=0x8c6515f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651684u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6515f8; /* mov.l 0x8c651684,r2 */
L8c6515f8: *stop=0x8c6515f8u;if(!valid_access(r[0]+r[11],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6515fa; /* mov.l @(r0,r11),r3 */
L8c6515fa: *stop=0x8c6515fau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6515fc; /* mov.l @r2,r2 */
L8c6515fc: *stop=0x8c6515fcu;r[3]+=r[2];goto L8c6515fe; /* add r2,r3 */
L8c6515fe: *stop=0x8c6515feu;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[3],4);if(io_status)return io_status;goto L8c651600; /* mov.l r3,@r7 */
L8c651600: *stop=0x8c651600u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651670u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651602; /* mov.l 0x8c651670,r0 */
L8c651602: *stop=0x8c651602u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651604; /* mov.l @(r0,r12),r0 */
L8c651604: *stop=0x8c651604u;*sr=(*sr&~1u)|(r[0]==0xfffffffdu);goto L8c651606; /* cmp/eq #-0x3,r0 */
L8c651606: if((*sr&1u))goto L8c651628;goto L8c651608; /* bt 0x8c651628 */
L8c651608: *stop=0x8c651608u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65168cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65160a; /* mov.l 0x8c65168c,r2 */
L8c65160a: *stop=0x8c65160au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65160c; /* mov.l @r2,r0 */
L8c65160c: *stop=0x8c65160cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65160e; /* cmp/eq #0x1,r0 */
L8c65160e: if(!(*sr&1u))goto L8c651628;goto L8c651610; /* bf 0x8c651628 */
L8c651610: *stop=0x8c651610u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651690u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651612; /* mov.l 0x8c651690,r3 */
L8c651612: *stop=0x8c651612u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c651614; /* mov.l @r3,r1 */
L8c651614: *stop=0x8c651614u;if(!valid_access(r[13],4u))return -3;wr(m,bus,&io_status,r[13],r[1],4);if(io_status)return io_status;goto L8c651616; /* mov.l r1,@r13 */
L8c651616: *stop=0x8c651616u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651694u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651618; /* mov.l 0x8c651694,r0 */
L8c651618: *stop=0x8c651618u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65161a; /* mov.l @r0,r2 */
L8c65161a: *stop=0x8c65161au;if(!valid_access(r[10],4u))return -3;wr(m,bus,&io_status,r[10],r[2],4);if(io_status)return io_status;goto L8c65161c; /* mov.l r2,@r10 */
L8c65161c: *stop=0x8c65161cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65168cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65161e; /* mov.l 0x8c65168c,r1 */
L8c65161e: *stop=0x8c651620u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c651628; /* bra 0x8c651628 */
L8c651622: *stop=0x8c651622u;r[5]+=1u;goto L8c651624; /* add #0x1,r5 */
L8c651624: *stop=0x8c651624u;*sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[12]);goto L8c651626; /* cmp/ge r12,r5 */
L8c651626: if(!(*sr&1u))goto L8c65158e;goto L8c651628; /* bf 0x8c65158e */
L8c651628: *stop=0x8c651628u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c65162a; /* mov.l @r15+,r10 */
L8c65162a: *stop=0x8c65162au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c65162c; /* mov.l @r15+,r11 */
L8c65162c: *stop=0x8c65162cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c65162e; /* mov.l @r15+,r12 */
L8c65162e: *stop=0x8c65162eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c651630; /* mov.l @r15+,r13 */
L8c651630: *stop=0x8c651630u;if(*pr!=entry_pr)return -3;*stop=0x8c651632u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_interrupt_8c6516a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6516a0;
L8c6516a0: *stop=0x8c6516a0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6516a2; /* mov.l r14,@-r15 */
L8c6516a2: *stop=0x8c6516a2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6516a4; /* mov.l r13,@-r15 */
L8c6516a4: *stop=0x8c6516a4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6516a6; /* mov.l r12,@-r15 */
L8c6516a6: *stop=0x8c6516a6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6516a8; /* mov.l r11,@-r15 */
L8c6516a8: *stop=0x8c6516a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6516aa; /* mov.l r10,@-r15 */
L8c6516aa: *stop=0x8c6516aau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6516ac; /* mov.l r9,@-r15 */
L8c6516ac: *stop=0x8c6516acu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6516ae; /* mov.l r8,@-r15 */
L8c6516ae: *stop=0x8c6516aeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c6516b0; /* sts.l PR,@-r15 */
L8c6516b0: *stop=0x8c6516b0u;r[15]+=4294967288u;goto L8c6516b2; /* add #-0x8,r15 */
L8c6516b2: *stop=0x8c6516b2u;r[12]=sx(m,bus,&io_status,0x8c6517e0u,2);if(io_status)return io_status;goto L8c6516b4; /* mov.w 0x8c6517e0,r12 */
L8c6516b4: *stop=0x8c6516b4u;r[4]=0u;goto L8c6516b6; /* mov #0x0,r4 */
L8c6516b6: *stop=0x8c6516b6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6517ecu,4);if(io_status)return io_status;r[11]=loaded;}goto L8c6516b8; /* mov.l 0x8c6517ec,r11 */
L8c6516b8: *stop=0x8c6516b8u;r[5]=1u;goto L8c6516ba; /* mov #0x1,r5 */
L8c6516ba: *stop=0x8c6516bau;{uint32_t loaded=rd(m,bus,&io_status,0x8c6517e8u,4);if(io_status)return io_status;r[10]=loaded;}goto L8c6516bc; /* mov.l 0x8c6517e8,r10 */
L8c6516bc: *stop=0x8c6516bcu;r[6]=r[4];goto L8c6516be; /* mov r4,r6 */
L8c6516be: *stop=0x8c6516beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6517f0u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c6516c0; /* mov.l 0x8c6517f0,r13 */
L8c6516c0: *stop=0x8c6516c0u;r[8]=2u;goto L8c6516c2; /* mov #0x2,r8 */
L8c6516c2: *stop=0x8c6516c2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6517e4u,4);if(io_status)return io_status;r[7]=loaded;}goto L8c6516c4; /* mov.l 0x8c6517e4,r7 */
L8c6516c4: *stop=0x8c6516c4u;r[9]=4u;goto L8c6516c6; /* mov #0x4,r9 */
L8c6516c6: *stop=0x8c6516c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6517f4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6516c8; /* mov.l 0x8c6517f4,r1 */
L8c6516c8: *stop=0x8c6516c8u;r[0]=r[6];goto L8c6516ca; /* mov r6,r0 */
L8c6516ca: *stop=0x8c6516cau;r[0]<<=2;goto L8c6516cc; /* shll2 r0 */
L8c6516cc: *stop=0x8c6516ccu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6516ce; /* mov.l @(r0,r1),r0 */
L8c6516ce: *stop=0x8c6516ceu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6516d0; /* cmp/eq #0x1,r0 */
L8c6516d0: if((*sr&1u))goto L8c6516d6;goto L8c6516d2; /* bt 0x8c6516d6 */
L8c6516d2: *stop=0x8c6516d4u;goto L8c65192a; /* bra 0x8c65192a */
L8c6516d6: *stop=0x8c6516d6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6517f8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6516d8; /* mov.l 0x8c6517f8,r3 */
L8c6516d8: *stop=0x8c6516d8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6516da; /* mov.l @r3,r2 */
L8c6516da: *stop=0x8c6516dau;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6516dc; /* tst r2,r2 */
L8c6516dc: if((*sr&1u))goto L8c6516e2;goto L8c6516de; /* bt 0x8c6516e2 */
L8c6516de: *stop=0x8c6516e0u;goto L8c65192a; /* bra 0x8c65192a */
L8c6516e2: *stop=0x8c6516e2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6517f8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6516e4; /* mov.l 0x8c6517f8,r2 */
L8c6516e4: *stop=0x8c6516e4u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[5],4);if(io_status)return io_status;goto L8c6516e6; /* mov.l r5,@r2 */
L8c6516e6: *stop=0x8c6516e6u;r[14]=r[6];goto L8c6516e8; /* mov r6,r14 */
L8c6516e8: *stop=0x8c6516e8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6517f4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6516ea; /* mov.l 0x8c6517f4,r0 */
L8c6516ea: *stop=0x8c6516eau;r[14]<<=2;goto L8c6516ec; /* shll2 r14 */
L8c6516ec: *stop=0x8c6516ecu;if(!valid_access(r[0]+r[14],4u))return -3;wr(m,bus,&io_status,r[14]+r[0],r[8],4);if(io_status)return io_status;goto L8c6516ee; /* mov.l r8,@(r0,r14) */
L8c6516ee: *stop=0x8c6516eeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6517fcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6516f0; /* mov.l 0x8c6517fc,r3 */
L8c6516f0: *stop=0x8c6516f0u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[6],4);if(io_status)return io_status;goto L8c6516f2; /* mov.l r6,@r3 */
L8c6516f2: *stop=0x8c6516f2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651800u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6516f4; /* mov.l 0x8c651800,r1 */
L8c6516f4: *stop=0x8c6516f4u;r[6]&=r[5];goto L8c6516f6; /* and r5,r6 */
L8c6516f6: *stop=0x8c6516f6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651804u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6516f8; /* mov.l 0x8c651804,r0 */
L8c6516f8: *stop=0x8c6516f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651808u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6516fa; /* mov.l 0x8c651808,r3 */
L8c6516fa: *stop=0x8c6516fau;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[6],4);if(io_status)return io_status;goto L8c6516fc; /* mov.l r6,@(0x4,r15) */
L8c6516fc: *stop=0x8c6516fcu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6516fe; /* mov.l @r1,r2 */
L8c6516fe: *stop=0x8c6516feu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c651700; /* mov.l r2,@r0 */
L8c651700: *stop=0x8c651700u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[5],4);if(io_status)return io_status;goto L8c651702; /* mov.l r5,@r3 */
L8c651702: *stop=0x8c651702u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c651704; /* mov.l r4,@r3 */
L8c651704: *stop=0x8c651704u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c651706; /* mov.l @(0x4,r15),r6 */
L8c651706: *stop=0x8c651706u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65180cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651708; /* mov.l 0x8c65180c,r0 */
L8c651708: *stop=0x8c651708u;r[6]<<=2;goto L8c65170a; /* shll2 r6 */
L8c65170a: *stop=0x8c65170au;r[6]<<=2;goto L8c65170c; /* shll2 r6 */
L8c65170c: *stop=0x8c65170cu;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c65170e; /* shll r6 */
L8c65170e: *stop=0x8c65170eu;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651710; /* mov.l @(r0,r6),r2 */
L8c651710: *stop=0x8c651710u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651810u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651712; /* mov.l 0x8c651810,r1 */
L8c651712: *stop=0x8c651712u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c651714; /* mov.l r2,@r1 */
L8c651714: *stop=0x8c651714u;r[0]+=4u;goto L8c651716; /* add #0x4,r0 */
L8c651716: *stop=0x8c651716u;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651718; /* mov.l @(r0,r6),r3 */
L8c651718: *stop=0x8c651718u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651814u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c65171a; /* mov.l 0x8c651814,r2 */
L8c65171a: *stop=0x8c65171au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c65171c; /* mov.l r3,@r2 */
L8c65171c: *stop=0x8c65171cu;r[0]+=8u;goto L8c65171e; /* add #0x8,r0 */
L8c65171e: *stop=0x8c65171eu;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c651720; /* mov.l @(r0,r6),r1 */
L8c651720: *stop=0x8c651720u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651818u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651722; /* mov.l 0x8c651818,r3 */
L8c651722: *stop=0x8c651722u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c651724; /* mov.l r1,@r3 */
L8c651724: *stop=0x8c651724u;r[0]+=4u;goto L8c651726; /* add #0x4,r0 */
L8c651726: *stop=0x8c651726u;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651728; /* mov.l @(r0,r6),r2 */
L8c651728: *stop=0x8c651728u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65181cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65172a; /* mov.l 0x8c65181c,r1 */
L8c65172a: *stop=0x8c65172au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c65172c; /* mov.l r2,@r1 */
L8c65172c: *stop=0x8c65172cu;r[0]+=4294967288u;goto L8c65172e; /* add #-0x8,r0 */
L8c65172e: *stop=0x8c65172eu;if(!valid_access(r[0]+r[6],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651730; /* mov.l @(r0,r6),r3 */
L8c651730: *stop=0x8c651730u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651820u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651732; /* mov.l 0x8c651820,r2 */
L8c651732: *stop=0x8c651732u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c651734; /* mov.l r3,@r2 */
L8c651734: *stop=0x8c651734u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651824u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651736; /* mov.l 0x8c651824,r1 */
L8c651736: *stop=0x8c651736u;if(!valid_access(r[7],4u))return -3;wr(m,bus,&io_status,r[7],r[1],4);if(io_status)return io_status;goto L8c651738; /* mov.l r1,@r7 */
L8c651738: *stop=0x8c651738u;if(!valid_access(r[7],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65173a; /* mov.l @r7,r3 */
L8c65173a: *stop=0x8c65173au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651828u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65173c; /* mov.l 0x8c651828,r3 */
L8c65173c: *stop=0x8c65173cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c65173e; /* mov.l r4,@r3 */
L8c65173e: *stop=0x8c65173eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65182cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651740; /* mov.l 0x8c65182c,r2 */
L8c651740: *stop=0x8c651740u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c651742; /* mov.l r4,@r2 */
L8c651742: *stop=0x8c651742u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651834u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651744; /* mov.l 0x8c651834,r0 */
L8c651744: *stop=0x8c651744u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651830u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651746; /* mov.l 0x8c651830,r1 */
L8c651746: *stop=0x8c651746u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c651748; /* mov.l r4,@r1 */
L8c651748: *stop=0x8c651748u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65174a; /* mov.l @(r0,r14),r3 */
L8c65174a: *stop=0x8c65174au;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c65174c; /* tst r3,r3 */
L8c65174c: if((*sr&1u))goto L8c651774;goto L8c65174e; /* bt 0x8c651774 */
L8c65174e: *stop=0x8c65174eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651828u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651750; /* mov.l 0x8c651828,r3 */
L8c651750: *stop=0x8c651750u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651752; /* mov.l @r3,r2 */
L8c651752: *stop=0x8c651752u;r[2]|=r[5];goto L8c651754; /* or r5,r2 */
L8c651754: *stop=0x8c651754u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c651756; /* mov.l r2,@r3 */
L8c651756: *stop=0x8c651756u;r[3]=r[15];goto L8c651758; /* mov r15,r3 */
L8c651758: *stop=0x8c651758u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c65175a; /* mov.l r3,@-r15 */
L8c65175a: *stop=0x8c65175au;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c65175c; /* mov.l @(r0,r14),r6 */
L8c65175c: *stop=0x8c65175cu;r[7]=16u;goto L8c65175e; /* mov #0x10,r7 */
L8c65175e: *stop=0x8c65175eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651838u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651760; /* mov.l 0x8c651838,r0 */
L8c651760: *stop=0x8c651760u;r[6]+=32u;goto L8c651762; /* add #0x20,r6 */
L8c651762: *stop=0x8c651762u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c651764; /* mov.l @(r0,r14),r5 */
L8c651764: target=r[11];*pr=0x8c651768u;*stop=0x8c651766u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651768; /* jsr @r11 */
L8c651768: *stop=0x8c651768u;r[4]=r[0];goto L8c65176a; /* mov r0,r4 */
L8c65176a: *stop=0x8c65176au;*sr=(*sr&~1u)|(r[12]==r[4]);goto L8c65176c; /* cmp/eq r12,r4 */
L8c65176c: take=!(*sr&1u);*stop=0x8c65176eu;r[15]+=4u;if(take)goto L8c651774;goto L8c651770; /* bf/s 0x8c651774 */
L8c651770: *stop=0x8c651772u;goto L8c651926; /* bra 0x8c651926 */
L8c651774: *stop=0x8c651774u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65183cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651776; /* mov.l 0x8c65183c,r0 */
L8c651776: *stop=0x8c651776u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651778; /* mov.l @(r0,r14),r2 */
L8c651778: *stop=0x8c651778u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c65177a; /* tst r2,r2 */
L8c65177a: if(!(*sr&1u))goto L8c651780;goto L8c65177c; /* bf 0x8c651780 */
L8c65177c: *stop=0x8c65177eu;goto L8c651870; /* bra 0x8c651870 */
L8c651780: *stop=0x8c651780u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651828u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651782; /* mov.l 0x8c651828,r3 */
L8c651782: *stop=0x8c651782u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c651784; /* mov.l @r3,r1 */
L8c651784: *stop=0x8c651784u;r[1]|=r[8];goto L8c651786; /* or r8,r1 */
L8c651786: *stop=0x8c651786u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c651788; /* mov.l r1,@r3 */
L8c651788: *stop=0x8c651788u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651840u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65178a; /* mov.l 0x8c651840,r3 */
L8c65178a: *stop=0x8c65178au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65178c; /* mov.l @r3,r0 */
L8c65178c: *stop=0x8c65178cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c65178e; /* cmp/eq #0x1,r0 */
L8c65178e: if(!(*sr&1u))goto L8c6517c0;goto L8c651790; /* bf 0x8c6517c0 */
L8c651790: *stop=0x8c651790u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651844u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651792; /* mov.l 0x8c651844,r2 */
L8c651792: *stop=0x8c651792u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c651794; /* mov.l @r2,r1 */
L8c651794: *stop=0x8c651794u;r[3]=r[2];goto L8c651796; /* mov r2,r3 */
L8c651796: *stop=0x8c651796u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651798; /* mov.l @r3,r0 */
L8c651798: *stop=0x8c651798u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65179a; /* mov.l @r0,r0 */
L8c65179a: *stop=0x8c65179au;r[7]=16u;goto L8c65179c; /* mov #0x10,r7 */
L8c65179c: *stop=0x8c65179cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651848u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65179e; /* mov.l 0x8c651848,r3 */
L8c65179e: *stop=0x8c65179eu;r[0]&=r[10];goto L8c6517a0; /* and r10,r0 */
L8c6517a0: *stop=0x8c6517a0u;r[0]|=r[3];goto L8c6517a2; /* or r3,r0 */
L8c6517a2: *stop=0x8c6517a2u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c6517a4; /* mov.l r0,@r1 */
L8c6517a4: *stop=0x8c6517a4u;r[1]=r[15];goto L8c6517a6; /* mov r15,r1 */
L8c6517a6: *stop=0x8c6517a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65184cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6517a8; /* mov.l 0x8c65184c,r0 */
L8c6517a8: *stop=0x8c6517a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c6517aa; /* mov.l r1,@-r15 */
L8c6517aa: *stop=0x8c6517aau;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6517ac; /* mov.l @r0,r6 */
L8c6517ac: *stop=0x8c6517acu;r[3]=r[2];goto L8c6517ae; /* mov r2,r3 */
L8c6517ae: *stop=0x8c6517aeu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6517b0; /* mov.l @r3,r5 */
L8c6517b0: target=r[11];*pr=0x8c6517b4u;*stop=0x8c6517b2u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6517b4; /* jsr @r11 */
L8c6517b4: *stop=0x8c6517b4u;r[4]=r[0];goto L8c6517b6; /* mov r0,r4 */
L8c6517b6: *stop=0x8c6517b6u;*sr=(*sr&~1u)|(r[12]==r[4]);goto L8c6517b8; /* cmp/eq r12,r4 */
L8c6517b8: take=!(*sr&1u);*stop=0x8c6517bau;r[15]+=4u;if(take)goto L8c6517c0;goto L8c6517bc; /* bf/s 0x8c6517c0 */
L8c6517bc: *stop=0x8c6517beu;goto L8c651926; /* bra 0x8c651926 */
L8c6517c0: *stop=0x8c6517c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65183cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6517c2; /* mov.l 0x8c65183c,r0 */
L8c6517c2: *stop=0x8c6517c2u;r[3]=r[15];goto L8c6517c4; /* mov r15,r3 */
L8c6517c4: *stop=0x8c6517c4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6517c6; /* mov.l r3,@-r15 */
L8c6517c6: *stop=0x8c6517c6u;r[7]=16u;goto L8c6517c8; /* mov #0x10,r7 */
L8c6517c8: *stop=0x8c6517c8u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6517ca; /* mov.l @(r0,r14),r6 */
L8c6517ca: *stop=0x8c6517cau;r[6]+=32u;goto L8c6517cc; /* add #0x20,r6 */
L8c6517cc: *stop=0x8c6517ceu;goto L8c651860; /* bra 0x8c651860 */
L8c651860: *stop=0x8c651860u;r[0]+=4294967216u;goto L8c651862; /* add #-0x50,r0 */
L8c651862: *stop=0x8c651862u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c651864; /* mov.l @(r0,r14),r5 */
L8c651864: target=r[11];*pr=0x8c651868u;*stop=0x8c651866u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651868; /* jsr @r11 */
L8c651868: *stop=0x8c651868u;r[4]=r[0];goto L8c65186a; /* mov r0,r4 */
L8c65186a: *stop=0x8c65186au;*sr=(*sr&~1u)|(r[12]==r[4]);goto L8c65186c; /* cmp/eq r12,r4 */
L8c65186c: take=(*sr&1u);*stop=0x8c65186eu;r[15]+=4u;if(take)goto L8c651926;goto L8c651870; /* bt/s 0x8c651926 */
L8c651870: *stop=0x8c651870u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a40u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651872; /* mov.l 0x8c651a40,r0 */
L8c651872: *stop=0x8c651872u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651874; /* mov.l @(r0,r14),r2 */
L8c651874: *stop=0x8c651874u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c651876; /* tst r2,r2 */
L8c651876: if((*sr&1u))goto L8c65189a;goto L8c651878; /* bt 0x8c65189a */
L8c651878: *stop=0x8c651878u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65187a; /* mov.l 0x8c651a44,r3 */
L8c65187a: *stop=0x8c65187au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c65187c; /* mov.l @r3,r1 */
L8c65187c: *stop=0x8c65187cu;r[1]|=r[9];goto L8c65187e; /* or r9,r1 */
L8c65187e: *stop=0x8c65187eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c651880; /* mov.l r1,@r3 */
L8c651880: *stop=0x8c651880u;r[3]=r[15];goto L8c651882; /* mov r15,r3 */
L8c651882: *stop=0x8c651882u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c651884; /* mov.l r3,@-r15 */
L8c651884: *stop=0x8c651884u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c651886; /* mov.l @(r0,r14),r6 */
L8c651886: *stop=0x8c651886u;r[7]=16u;goto L8c651888; /* mov #0x10,r7 */
L8c651888: *stop=0x8c651888u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a48u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65188a; /* mov.l 0x8c651a48,r0 */
L8c65188a: *stop=0x8c65188au;r[6]+=32u;goto L8c65188c; /* add #0x20,r6 */
L8c65188c: *stop=0x8c65188cu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c65188e; /* mov.l @(r0,r14),r5 */
L8c65188e: target=r[11];*pr=0x8c651892u;*stop=0x8c651890u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c651892; /* jsr @r11 */
L8c651892: *stop=0x8c651892u;r[4]=r[0];goto L8c651894; /* mov r0,r4 */
L8c651894: *stop=0x8c651894u;*sr=(*sr&~1u)|(r[12]==r[4]);goto L8c651896; /* cmp/eq r12,r4 */
L8c651896: take=(*sr&1u);*stop=0x8c651898u;r[15]+=4u;if(take)goto L8c651926;goto L8c65189a; /* bt/s 0x8c651926 */
L8c65189a: *stop=0x8c65189au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a4cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65189c; /* mov.l 0x8c651a4c,r0 */
L8c65189c: *stop=0x8c65189cu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65189e; /* mov.l @(r0,r14),r2 */
L8c65189e: *stop=0x8c65189eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c6518a0; /* tst r2,r2 */
L8c6518a0: if((*sr&1u))goto L8c6518fa;goto L8c6518a2; /* bt 0x8c6518fa */
L8c6518a2: *stop=0x8c6518a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6518a4; /* mov.l 0x8c651a44,r3 */
L8c6518a4: *stop=0x8c6518a4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6518a6; /* mov.l @r3,r0 */
L8c6518a6: *stop=0x8c6518a6u;r[0]|=0x8u;goto L8c6518a8; /* or #0x8,r0 */
L8c6518a8: *stop=0x8c6518a8u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c6518aa; /* mov.l r0,@r3 */
L8c6518aa: *stop=0x8c6518aau;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a50u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6518ac; /* mov.l 0x8c651a50,r2 */
L8c6518ac: *stop=0x8c6518acu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6518ae; /* mov.l @r2,r0 */
L8c6518ae: *stop=0x8c6518aeu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6518b0; /* cmp/eq #0x1,r0 */
L8c6518b0: if(!(*sr&1u))goto L8c6518de;goto L8c6518b2; /* bf 0x8c6518de */
L8c6518b2: *stop=0x8c6518b2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a54u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6518b4; /* mov.l 0x8c651a54,r3 */
L8c6518b4: *stop=0x8c6518b4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6518b6; /* mov.l @r3,r1 */
L8c6518b6: *stop=0x8c6518b6u;r[2]=r[3];goto L8c6518b8; /* mov r3,r2 */
L8c6518b8: *stop=0x8c6518b8u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6518ba; /* mov.l @r2,r0 */
L8c6518ba: *stop=0x8c6518bau;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6518bc; /* mov.l @r0,r0 */
L8c6518bc: *stop=0x8c6518bcu;r[7]=16u;goto L8c6518be; /* mov #0x10,r7 */
L8c6518be: *stop=0x8c6518beu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a58u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6518c0; /* mov.l 0x8c651a58,r3 */
L8c6518c0: *stop=0x8c6518c0u;r[0]&=r[10];goto L8c6518c2; /* and r10,r0 */
L8c6518c2: *stop=0x8c6518c2u;r[0]|=r[3];goto L8c6518c4; /* or r3,r0 */
L8c6518c4: *stop=0x8c6518c4u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[0],4);if(io_status)return io_status;goto L8c6518c6; /* mov.l r0,@r1 */
L8c6518c6: *stop=0x8c6518c6u;r[1]=r[15];goto L8c6518c8; /* mov r15,r1 */
L8c6518c8: *stop=0x8c6518c8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a5cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6518ca; /* mov.l 0x8c651a5c,r0 */
L8c6518ca: *stop=0x8c6518cau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c6518cc; /* mov.l r1,@-r15 */
L8c6518cc: *stop=0x8c6518ccu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6518ce; /* mov.l @r0,r6 */
L8c6518ce: *stop=0x8c6518ceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a54u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6518d0; /* mov.l 0x8c651a54,r3 */
L8c6518d0: *stop=0x8c6518d0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6518d2; /* mov.l @r3,r5 */
L8c6518d2: target=r[11];*pr=0x8c6518d6u;*stop=0x8c6518d4u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6518d6; /* jsr @r11 */
L8c6518d6: *stop=0x8c6518d6u;r[4]=r[0];goto L8c6518d8; /* mov r0,r4 */
L8c6518d8: *stop=0x8c6518d8u;*sr=(*sr&~1u)|(r[12]==r[4]);goto L8c6518da; /* cmp/eq r12,r4 */
L8c6518da: take=(*sr&1u);*stop=0x8c6518dcu;r[15]+=4u;if(take)goto L8c651926;goto L8c6518de; /* bt/s 0x8c651926 */
L8c6518de: *stop=0x8c6518deu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a4cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6518e0; /* mov.l 0x8c651a4c,r0 */
L8c6518e0: *stop=0x8c6518e0u;r[3]=r[15];goto L8c6518e2; /* mov r15,r3 */
L8c6518e2: *stop=0x8c6518e2u;r[7]=16u;goto L8c6518e4; /* mov #0x10,r7 */
L8c6518e4: *stop=0x8c6518e4u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6518e6; /* mov.l r3,@-r15 */
L8c6518e6: *stop=0x8c6518e6u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c6518e8; /* mov.l @(r0,r14),r6 */
L8c6518e8: *stop=0x8c6518e8u;r[0]+=4294967216u;goto L8c6518ea; /* add #-0x50,r0 */
L8c6518ea: *stop=0x8c6518eau;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6518ec; /* mov.l @(r0,r14),r5 */
L8c6518ec: *stop=0x8c6518ecu;r[6]+=32u;goto L8c6518ee; /* add #0x20,r6 */
L8c6518ee: target=r[11];*pr=0x8c6518f2u;*stop=0x8c6518f0u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6518f2; /* jsr @r11 */
L8c6518f2: *stop=0x8c6518f2u;r[4]=r[0];goto L8c6518f4; /* mov r0,r4 */
L8c6518f4: *stop=0x8c6518f4u;*sr=(*sr&~1u)|(r[12]==r[4]);goto L8c6518f6; /* cmp/eq r12,r4 */
L8c6518f6: take=(*sr&1u);*stop=0x8c6518f8u;r[15]+=4u;if(take)goto L8c651926;goto L8c6518fa; /* bt/s 0x8c651926 */
L8c6518fa: *stop=0x8c6518fau;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a60u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6518fc; /* mov.l 0x8c651a60,r0 */
L8c6518fc: *stop=0x8c6518fcu;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6518fe; /* mov.l @(r0,r14),r2 */
L8c6518fe: *stop=0x8c6518feu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c651900; /* tst r2,r2 */
L8c651900: if((*sr&1u))goto L8c651934;goto L8c651902; /* bt 0x8c651934 */
L8c651902: *stop=0x8c651902u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651904; /* mov.l 0x8c651a44,r3 */
L8c651904: *stop=0x8c651904u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651906; /* mov.l @r3,r0 */
L8c651906: *stop=0x8c651906u;r[0]|=0x10u;goto L8c651908; /* or #0x10,r0 */
L8c651908: *stop=0x8c651908u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c65190a; /* mov.l r0,@r3 */
L8c65190a: *stop=0x8c65190au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a60u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65190c; /* mov.l 0x8c651a60,r0 */
L8c65190c: *stop=0x8c65190cu;r[2]=r[15];goto L8c65190e; /* mov r15,r2 */
L8c65190e: *stop=0x8c65190eu;r[7]=16u;goto L8c651910; /* mov #0x10,r7 */
L8c651910: *stop=0x8c651910u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c651912; /* mov.l r2,@-r15 */
L8c651912: *stop=0x8c651912u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[6]=loaded;}goto L8c651914; /* mov.l @(r0,r14),r6 */
L8c651914: *stop=0x8c651914u;r[0]+=4294967216u;goto L8c651916; /* add #-0x50,r0 */
L8c651916: *stop=0x8c651916u;if(!valid_access(r[0]+r[14],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;r[5]=loaded;}goto L8c651918; /* mov.l @(r0,r14),r5 */
L8c651918: *stop=0x8c651918u;r[6]+=32u;goto L8c65191a; /* add #0x20,r6 */
L8c65191a: target=r[11];*pr=0x8c65191eu;*stop=0x8c65191cu;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65191e; /* jsr @r11 */
L8c65191e: *stop=0x8c65191eu;r[4]=r[0];goto L8c651920; /* mov r0,r4 */
L8c651920: *stop=0x8c651920u;*sr=(*sr&~1u)|(r[12]==r[4]);goto L8c651922; /* cmp/eq r12,r4 */
L8c651922: take=!(*sr&1u);*stop=0x8c651924u;r[15]+=4u;if(take)goto L8c651934;goto L8c651926; /* bf/s 0x8c651934 */
L8c651926: *stop=0x8c651928u;r[0]=0u;goto L8c651936; /* bra 0x8c651936 */
L8c65192a: *stop=0x8c65192au;r[6]+=1u;goto L8c65192c; /* add #0x1,r6 */
L8c65192c: *stop=0x8c65192cu;*sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[9]);goto L8c65192e; /* cmp/ge r9,r6 */
L8c65192e: if((*sr&1u))goto L8c651934;goto L8c651930; /* bt 0x8c651934 */
L8c651930: *stop=0x8c651932u;goto L8c6516c6; /* bra 0x8c6516c6 */
L8c651934: *stop=0x8c651934u;r[0]=1u;goto L8c651936; /* mov #0x1,r0 */
L8c651936: *stop=0x8c651936u;r[15]+=8u;goto L8c651938; /* add #0x8,r15 */
L8c651938: *stop=0x8c651938u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65193a; /* lds.l @r15+,PR */
L8c65193a: *stop=0x8c65193au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c65193c; /* mov.l @r15+,r8 */
L8c65193c: *stop=0x8c65193cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c65193e; /* mov.l @r15+,r9 */
L8c65193e: *stop=0x8c65193eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c651940; /* mov.l @r15+,r10 */
L8c651940: *stop=0x8c651940u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c651942; /* mov.l @r15+,r11 */
L8c651942: *stop=0x8c651942u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c651944; /* mov.l @r15+,r12 */
L8c651944: *stop=0x8c651944u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c651946; /* mov.l @r15+,r13 */
L8c651946: *stop=0x8c651946u;if(*pr!=entry_pr)return -3;*stop=0x8c651948u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_interrupt_8c65194a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65194a;
L8c65194a: *stop=0x8c65194au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c65194c; /* mov.l r14,@-r15 */
L8c65194c: *stop=0x8c65194cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c65194e; /* mov.l r13,@-r15 */
L8c65194e: *stop=0x8c65194eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c651950; /* mov.l r12,@-r15 */
L8c651950: *stop=0x8c651950u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c651952; /* mov.l r11,@-r15 */
L8c651952: *stop=0x8c651952u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c651954; /* mov.l r10,@-r15 */
L8c651954: *stop=0x8c651954u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c651956; /* mov.l r9,@-r15 */
L8c651956: *stop=0x8c651956u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c651958; /* mov.l r8,@-r15 */
L8c651958: *stop=0x8c651958u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65195a; /* sts.l PR,@-r15 */
L8c65195a: *stop=0x8c65195au;r[15]+=4294967292u;goto L8c65195c; /* add #-0x4,r15 */
L8c65195c: *stop=0x8c65195cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a64u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c65195e; /* mov.l 0x8c651a64,r8 */
L8c65195e: *stop=0x8c65195eu;r[11]=0u;goto L8c651960; /* mov #0x0,r11 */
L8c651960: *stop=0x8c651960u;r[10]=4u;goto L8c651962; /* mov #0x4,r10 */
L8c651962: *stop=0x8c651962u;r[13]=r[11];goto L8c651964; /* mov r11,r13 */
L8c651964: *stop=0x8c651964u;r[9]=r[8];goto L8c651966; /* mov r8,r9 */
L8c651966: *stop=0x8c651966u;r[14]=1u;goto L8c651968; /* mov #0x1,r14 */
L8c651968: *stop=0x8c651968u;r[9]+=88u;goto L8c65196a; /* add #0x58,r9 */
L8c65196a: *stop=0x8c65196au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a68u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65196c; /* mov.l 0x8c651a68,r1 */
L8c65196c: *stop=0x8c65196cu;r[0]=r[13];goto L8c65196e; /* mov r13,r0 */
L8c65196e: *stop=0x8c65196eu;r[0]<<=2;goto L8c651970; /* shll2 r0 */
L8c651970: *stop=0x8c651970u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651972; /* mov.l @(r0,r1),r0 */
L8c651972: *stop=0x8c651972u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c651974; /* cmp/eq #0x3,r0 */
L8c651974: if((*sr&1u))goto L8c65197a;goto L8c651976; /* bt 0x8c65197a */
L8c651976: *stop=0x8c651978u;goto L8c651b42; /* bra 0x8c651b42 */
L8c65197a: *stop=0x8c65197au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a6cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65197c; /* mov.l 0x8c651a6c,r3 */
L8c65197c: *stop=0x8c65197cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c65197e; /* mov.l @r3,r2 */
L8c65197e: *stop=0x8c65197eu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c651980; /* tst r2,r2 */
L8c651980: if((*sr&1u))goto L8c651986;goto L8c651982; /* bt 0x8c651986 */
L8c651982: *stop=0x8c651984u;goto L8c651b42; /* bra 0x8c651b42 */
L8c651986: *stop=0x8c651986u;r[12]=r[13];goto L8c651988; /* mov r13,r12 */
L8c651988: *stop=0x8c651988u;r[12]&=r[14];goto L8c65198a; /* and r14,r12 */
L8c65198a: *stop=0x8c65198au;r[2]=r[12];goto L8c65198c; /* mov r12,r2 */
L8c65198c: *stop=0x8c65198cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a70u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c65198e; /* mov.l 0x8c651a70,r1 */
L8c65198e: *stop=0x8c65198eu;r[2]+=1u;goto L8c651990; /* add #0x1,r2 */
L8c651990: *stop=0x8c651990u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c651992; /* mov.l r2,@r15 */
L8c651992: *stop=0x8c651992u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651994; /* mov.l @r1,r3 */
L8c651994: *stop=0x8c651994u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c651996; /* tst r3,r2 */
L8c651996: if((*sr&1u))goto L8c6519c6;goto L8c651998; /* bt 0x8c6519c6 */
L8c651998: *stop=0x8c651998u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a74u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c65199a; /* mov.l 0x8c651a74,r0 */
L8c65199a: *stop=0x8c65199au;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c65199c; /* mov.l @r0,r3 */
L8c65199c: *stop=0x8c65199cu;r[3]=~r[3];goto L8c65199e; /* not r3,r3 */
L8c65199e: *stop=0x8c65199eu;r[3]&=r[14];goto L8c6519a0; /* and r14,r3 */
L8c6519a0: *stop=0x8c6519a0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6519a2; /* mov.l r3,@-r15 */
L8c6519a2: *stop=0x8c6519a2u;r[6]=0u;goto L8c6519a4; /* mov #0x0,r6 */
L8c6519a4: *stop=0x8c6519a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a78u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6519a6; /* mov.l 0x8c651a78,r3 */
L8c6519a6: *stop=0x8c6519a6u;r[7]=1u;goto L8c6519a8; /* mov #0x1,r7 */
L8c6519a8: *stop=0x8c6519a8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6519aa; /* mov.l r12,@-r15 */
L8c6519aa: *stop=0x8c6519aau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c6519ac; /* mov.l @r3,r5 */
L8c6519ac: *stop=0x8c6519acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a80u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6519ae; /* mov.l 0x8c651a80,r3 */
L8c6519ae: *stop=0x8c6519aeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a7cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6519b0; /* mov.l 0x8c651a7c,r2 */
L8c6519b0: target=r[3];*pr=0x8c6519b4u;*stop=0x8c6519b2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6519b4; /* jsr @r3 */
L8c6519b4: *stop=0x8c6519b4u;r[15]+=8u;goto L8c6519b6; /* add #0x8,r15 */
L8c6519b6: *stop=0x8c6519b6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6519b8; /* mov.l @r15,r0 */
L8c6519b8: *stop=0x8c6519b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a70u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6519ba; /* mov.l 0x8c651a70,r2 */
L8c6519ba: *stop=0x8c6519bau;r[0]=~r[0];goto L8c6519bc; /* not r0,r0 */
L8c6519bc: *stop=0x8c6519bcu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6519be; /* mov.l @r2,r3 */
L8c6519be: *stop=0x8c6519beu;r[0]&=r[3];goto L8c6519c0; /* and r3,r0 */
L8c6519c0: *stop=0x8c6519c0u;r[3]=r[2];goto L8c6519c2; /* mov r2,r3 */
L8c6519c2: *stop=0x8c6519c2u;r[0]&=0x3u;goto L8c6519c4; /* and #0x3,r0 */
L8c6519c4: *stop=0x8c6519c4u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c6519c6; /* mov.l r0,@r3 */
L8c6519c6: *stop=0x8c6519c6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6519c8; /* mov.l 0x8c651a6c,r2 */
L8c6519c8: *stop=0x8c6519c8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c6519ca; /* mov.l r14,@r2 */
L8c6519ca: *stop=0x8c6519cau;r[3]=r[13];goto L8c6519cc; /* mov r13,r3 */
L8c6519cc: *stop=0x8c6519ccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a68u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6519ce; /* mov.l 0x8c651a68,r0 */
L8c6519ce: *stop=0x8c6519ceu;r[3]<<=2;goto L8c6519d0; /* shll2 r3 */
L8c6519d0: *stop=0x8c6519d0u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[10],4);if(io_status)return io_status;goto L8c6519d2; /* mov.l r10,@(r0,r3) */
L8c6519d2: *stop=0x8c6519d2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a84u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6519d4; /* mov.l 0x8c651a84,r1 */
L8c6519d4: *stop=0x8c6519d4u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6519d6; /* mov.l @r1,r0 */
L8c6519d6: *stop=0x8c6519d6u;*sr=(*sr&~1u)|!(r[0]&0x10u);goto L8c6519d8; /* tst #0x10,r0 */
L8c6519d8: take=(*sr&1u);*stop=0x8c6519dau;r[4]=r[12];if(take)goto L8c6519e2;goto L8c6519dc; /* bt/s 0x8c6519e2 */
L8c6519dc: *stop=0x8c6519dcu;r[3]=2u;goto L8c6519de; /* mov #0x2,r3 */
L8c6519de: *stop=0x8c6519deu;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6519e0; /* mov.l r3,@r8 */
L8c6519e0: *stop=0x8c6519e0u;if(!valid_access(r[8],4u))return -3;wr(m,bus,&io_status,r[8],r[11],4);if(io_status)return io_status;goto L8c6519e2; /* mov.l r11,@r8 */
L8c6519e2: *stop=0x8c6519e2u;r[4]<<=2;goto L8c6519e4; /* shll2 r4 */
L8c6519e4: *stop=0x8c6519e4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a88u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6519e6; /* mov.l 0x8c651a88,r0 */
L8c6519e6: *stop=0x8c6519e6u;r[4]<<=2;goto L8c6519e8; /* shll2 r4 */
L8c6519e8: *stop=0x8c6519e8u;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6519ea; /* shll r4 */
L8c6519ea: *stop=0x8c6519eau;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c6519ec; /* mov.l @(r0,r4),r2 */
L8c6519ec: *stop=0x8c6519ecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a8cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6519ee; /* mov.l 0x8c651a8c,r3 */
L8c6519ee: *stop=0x8c6519eeu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6519f0; /* mov.l r2,@r3 */
L8c6519f0: *stop=0x8c6519f0u;r[0]+=20u;goto L8c6519f2; /* add #0x14,r0 */
L8c6519f2: *stop=0x8c6519f2u;if(!valid_access(r[0]+r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6519f4; /* mov.l @(r0,r4),r1 */
L8c6519f4: *stop=0x8c6519f4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a90u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6519f6; /* mov.l 0x8c651a90,r2 */
L8c6519f6: *stop=0x8c6519f6u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c6519f8; /* mov.l r1,@r2 */
L8c6519f8: *stop=0x8c6519f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a94u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c6519fa; /* mov.l 0x8c651a94,r3 */
L8c6519fa: *stop=0x8c6519fau;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c6519fc; /* mov.l @r3,r0 */
L8c6519fc: *stop=0x8c6519fcu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c6519fe; /* cmp/eq #0x1,r0 */
L8c6519fe: if((*sr&1u))goto L8c651ac8;goto L8c651a00; /* bt 0x8c651ac8 */
L8c651a00: *stop=0x8c651a00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a98u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651a02; /* mov.l 0x8c651a98,r1 */
L8c651a02: *stop=0x8c651a02u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651a04; /* mov.l @r1,r0 */
L8c651a04: *stop=0x8c651a04u;r[0]&=r[14];goto L8c651a06; /* and r14,r0 */
L8c651a06: *stop=0x8c651a06u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c651a08; /* cmp/eq #0x1,r0 */
L8c651a08: if((*sr&1u))goto L8c651ac0;goto L8c651a0a; /* bt 0x8c651ac0 */
L8c651a0a: *stop=0x8c651a0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a98u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651a0c; /* mov.l 0x8c651a98,r2 */
L8c651a0c: *stop=0x8c651a0cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651a0e; /* mov.l @r2,r0 */
L8c651a0e: *stop=0x8c651a0eu;r[0]&=0x2cu;goto L8c651a10; /* and #0x2c,r0 */
L8c651a10: *stop=0x8c651a10u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c651a12; /* cmp/eq #0x20,r0 */
L8c651a12: if(!(*sr&1u))goto L8c651ac0;goto L8c651a14; /* bf 0x8c651ac0 */
L8c651a14: *stop=0x8c651a14u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c651a16; /* tst r14,r14 */
L8c651a16: if((*sr&1u))goto L8c651ac0;goto L8c651a18; /* bt 0x8c651ac0 */
L8c651a18: *stop=0x8c651a18u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651aa0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651a1a; /* mov.l 0x8c651aa0,r2 */
L8c651a1a: *stop=0x8c651a1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651a9cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651a1c; /* mov.l 0x8c651a9c,r0 */
L8c651a1c: *stop=0x8c651a1cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651a1e; /* mov.l @r2,r3 */
L8c651a1e: *stop=0x8c651a1eu;r[3]<<=2;goto L8c651a20; /* shll2 r3 */
L8c651a20: *stop=0x8c651a22u;if(!valid_access(r[0]+r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651ac6; /* bra 0x8c651ac6 */
L8c651ac0: *stop=0x8c651ac0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b60u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651ac2; /* mov.l 0x8c651b60,r0 */
L8c651ac2: *stop=0x8c651ac2u;r[12]<<=2;goto L8c651ac4; /* shll2 r12 */
L8c651ac4: *stop=0x8c651ac4u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651ac6; /* mov.l @(r0,r12),r3 */
L8c651ac6: *stop=0x8c651ac6u;if(!valid_access(r[9],4u))return -3;wr(m,bus,&io_status,r[9],r[3],4);if(io_status)return io_status;goto L8c651ac8; /* mov.l r3,@r9 */
L8c651ac8: *stop=0x8c651ac8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b64u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651aca; /* mov.l 0x8c651b64,r1 */
L8c651aca: *stop=0x8c651acau;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651acc; /* mov.l @r1,r2 */
L8c651acc: *stop=0x8c651accu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b68u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651ace; /* mov.l 0x8c651b68,r0 */
L8c651ace: *stop=0x8c651aceu;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c651ad0; /* mov.l r2,@r0 */
L8c651ad0: *stop=0x8c651ad0u;r[1]+=4294967292u;goto L8c651ad2; /* add #-0x4,r1 */
L8c651ad2: *stop=0x8c651ad2u;r[3]=4294967280u;goto L8c651ad4; /* mov #-0x10,r3 */
L8c651ad4: *stop=0x8c651ad4u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651ad6; /* mov.l @r1,r2 */
L8c651ad6: *stop=0x8c651ad6u;r[0]+=4294967292u;goto L8c651ad8; /* add #-0x4,r0 */
L8c651ad8: *stop=0x8c651ad8u;r[2]&=r[3];goto L8c651ada; /* and r3,r2 */
L8c651ada: *stop=0x8c651adau;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c651adc; /* mov.l r2,@r0 */
L8c651adc: *stop=0x8c651adcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651ade; /* mov.l 0x8c651b6c,r2 */
L8c651ade: *stop=0x8c651adeu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c651ae0; /* mov.l @r2,r1 */
L8c651ae0: *stop=0x8c651ae0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b70u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651ae2; /* mov.l 0x8c651b70,r3 */
L8c651ae2: *stop=0x8c651ae2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c651ae4; /* mov.l r1,@r3 */
L8c651ae4: *stop=0x8c651ae4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b74u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651ae6; /* mov.l 0x8c651b74,r1 */
L8c651ae6: *stop=0x8c651ae6u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651ae8; /* mov.l @r1,r0 */
L8c651ae8: *stop=0x8c651ae8u;r[3]+=4u;goto L8c651aea; /* add #0x4,r3 */
L8c651aea: *stop=0x8c651aeau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c651aec; /* mov.l r0,@r3 */
L8c651aec: *stop=0x8c651aecu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b78u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651aee; /* mov.l 0x8c651b78,r0 */
L8c651aee: *stop=0x8c651aeeu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c651af0; /* mov.l @r0,r2 */
L8c651af0: *stop=0x8c651af0u;r[3]+=4294967264u;goto L8c651af2; /* add #-0x20,r3 */
L8c651af2: *stop=0x8c651af2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c651af4; /* mov.l r2,@r3 */
L8c651af4: *stop=0x8c651af4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b7cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651af6; /* mov.l 0x8c651b7c,r2 */
L8c651af6: *stop=0x8c651af6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c651af8; /* mov.l @r2,r1 */
L8c651af8: *stop=0x8c651af8u;r[3]+=4294967292u;goto L8c651afa; /* add #-0x4,r3 */
L8c651afa: *stop=0x8c651afau;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[1],4);if(io_status)return io_status;goto L8c651afc; /* mov.l r1,@r3 */
L8c651afc: *stop=0x8c651afcu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b80u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651afe; /* mov.l 0x8c651b80,r0 */
L8c651afe: *stop=0x8c651afeu;r[3]=4294967277u;goto L8c651b00; /* mov #-0x13,r3 */
L8c651b00: *stop=0x8c651b00u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b84u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651b02; /* mov.l 0x8c651b84,r1 */
L8c651b02: *stop=0x8c651b02u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[11],4);if(io_status)return io_status;goto L8c651b04; /* mov.l r11,@r0 */
L8c651b04: *stop=0x8c651b04u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651b06; /* mov.l @r1,r0 */
L8c651b06: *stop=0x8c651b06u;r[0]&=r[3];goto L8c651b08; /* and r3,r0 */
L8c651b08: *stop=0x8c651b08u;*sr=(*sr&~1u)|(r[0]==0x00000020u);goto L8c651b0a; /* cmp/eq #0x20,r0 */
L8c651b0a: if(!(*sr&1u))goto L8c651b2a;goto L8c651b0c; /* bf 0x8c651b2a */
L8c651b0c: *stop=0x8c651b0cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b88u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651b0e; /* mov.l 0x8c651b88,r2 */
L8c651b0e: *stop=0x8c651b0eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c651b10; /* mov.l @r2,r1 */
L8c651b10: *stop=0x8c651b10u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c651b12; /* tst r1,r1 */
L8c651b12: if(!(*sr&1u))goto L8c651b1e;goto L8c651b14; /* bf 0x8c651b1e */
L8c651b14: *stop=0x8c651b14u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b90u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651b16; /* mov.l 0x8c651b90,r3 */
L8c651b16: *stop=0x8c651b16u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b8cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651b18; /* mov.l 0x8c651b8c,r1 */
L8c651b18: *stop=0x8c651b18u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651b1a; /* mov.l @r3,r0 */
L8c651b1a: *stop=0x8c651b1cu;goto L8c651b24; /* bra 0x8c651b24 */
L8c651b1e: *stop=0x8c651b1eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b90u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651b20; /* mov.l 0x8c651b90,r3 */
L8c651b20: *stop=0x8c651b20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b94u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651b22; /* mov.l 0x8c651b94,r1 */
L8c651b22: *stop=0x8c651b22u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c651b24; /* mov.l @r3,r0 */
L8c651b24: *stop=0x8c651b24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b98u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651b26; /* mov.l 0x8c651b98,r2 */
L8c651b26: *stop=0x8c651b26u;r[0]|=r[1];goto L8c651b28; /* or r1,r0 */
L8c651b28: *stop=0x8c651b28u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c651b2a; /* mov.l r0,@r2 */
L8c651b2a: *stop=0x8c651b2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651b9cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651b2c; /* mov.l 0x8c651b9c,r1 */
L8c651b2c: *stop=0x8c651b2cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c651ba0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c651b2e; /* mov.l 0x8c651ba0,r2 */
L8c651b2e: *stop=0x8c651b2eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c651b30; /* mov.l @r1,r3 */
L8c651b30: *stop=0x8c651b30u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651ba4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c651b32; /* mov.l 0x8c651ba4,r0 */
L8c651b32: *stop=0x8c651b32u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c651b34; /* mov.l r3,@r2 */
L8c651b34: *stop=0x8c651b34u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[13],4);if(io_status)return io_status;goto L8c651b36; /* mov.l r13,@r0 */
L8c651b36: *stop=0x8c651b36u;{uint32_t loaded=rd(m,bus,&io_status,0x8c651ba8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c651b38; /* mov.l 0x8c651ba8,r3 */
L8c651b38: *stop=0x8c651b38u;r[2]=4294967295u;goto L8c651b3a; /* mov #-0x1,r2 */
L8c651b3a: *stop=0x8c651b3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c651bacu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c651b3c; /* mov.l 0x8c651bac,r1 */
L8c651b3c: *stop=0x8c651b3cu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c651b3e; /* mov.l r14,@r3 */
L8c651b3e: *stop=0x8c651b40u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c651b4c; /* bra 0x8c651b4c */
L8c651b42: *stop=0x8c651b42u;r[13]+=1u;goto L8c651b44; /* add #0x1,r13 */
L8c651b44: *stop=0x8c651b44u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[10]);goto L8c651b46; /* cmp/ge r10,r13 */
L8c651b46: if((*sr&1u))goto L8c651b4c;goto L8c651b48; /* bt 0x8c651b4c */
L8c651b48: *stop=0x8c651b4au;goto L8c65196a; /* bra 0x8c65196a */
L8c651b4c: *stop=0x8c651b4cu;r[15]+=4u;goto L8c651b4e; /* add #0x4,r15 */
L8c651b4e: *stop=0x8c651b4eu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c651b50; /* lds.l @r15+,PR */
L8c651b50: *stop=0x8c651b50u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c651b52; /* mov.l @r15+,r8 */
L8c651b52: *stop=0x8c651b52u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c651b54; /* mov.l @r15+,r9 */
L8c651b54: *stop=0x8c651b54u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c651b56; /* mov.l @r15+,r10 */
L8c651b56: *stop=0x8c651b56u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c651b58; /* mov.l @r15+,r11 */
L8c651b58: *stop=0x8c651b58u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c651b5a; /* mov.l @r15+,r12 */
L8c651b5a: *stop=0x8c651b5au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c651b5c; /* mov.l @r15+,r13 */
L8c651b5c: *stop=0x8c651b5cu;if(*pr!=entry_pr)return -3;*stop=0x8c651b5eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_interrupt_8c665e3c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c665e3c;
L8c665e3c: *stop=0x8c665e3cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c665e3e; /* mov.l r14,@-r15 */
L8c665e3e: *stop=0x8c665e3eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c665e40; /* mov.l r13,@-r15 */
L8c665e40: *stop=0x8c665e40u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c665e42; /* sts.l PR,@-r15 */
L8c665e42: *stop=0x8c665e42u;r[15]+=4294967284u;goto L8c665e44; /* add #-0xc,r15 */
L8c665e44: *stop=0x8c665e44u;r[13]=r[6];goto L8c665e46; /* mov r6,r13 */
L8c665e46: *stop=0x8c665e46u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c665e48; /* mov.l r4,@r15 */
L8c665e48: *stop=0x8c665e48u;r[13]<<=2;goto L8c665e4a; /* shll2 r13 */
L8c665e4a: *stop=0x8c665e4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c665efcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e4c; /* mov.l 0x8c665efc,r0 */
L8c665e4c: *stop=0x8c665e4cu;r[13]<<=2;goto L8c665e4e; /* shll2 r13 */
L8c665e4e: *stop=0x8c665e4eu;r[4]=sx(m,bus,&io_status,0x8c665ee0u,2);if(io_status)return io_status;goto L8c665e50; /* mov.w 0x8c665ee0,r4 */
L8c665e50: *stop=0x8c665e50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ef8u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c665e52; /* mov.l 0x8c665ef8,r14 */
L8c665e52: *stop=0x8c665e52u;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c665e54; /* shll r13 */
L8c665e54: *stop=0x8c665e54u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c665e56; /* mov.l r5,@(0x4,r15) */
L8c665e56: *stop=0x8c665e56u;if(!valid_access(r[15]+8u,4u))return -3;wr(m,bus,&io_status,r[15]+0x8u,r[6],4);if(io_status)return io_status;goto L8c665e58; /* mov.l r6,@(0x8,r15) */
L8c665e58: target=r[14];*pr=0x8c665e5cu;*stop=0x8c665e5au;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665e5c; /* jsr @r14 */
L8c665e5c: *stop=0x8c665e5cu;r[4]=sx(m,bus,&io_status,0x8c665ee2u,2);if(io_status)return io_status;goto L8c665e5e; /* mov.w 0x8c665ee2,r4 */
L8c665e5e: *stop=0x8c665e5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c665f00u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e60; /* mov.l 0x8c665f00,r0 */
L8c665e60: target=r[14];*pr=0x8c665e64u;*stop=0x8c665e62u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665e64; /* jsr @r14 */
L8c665e64: *stop=0x8c665e64u;r[4]=sx(m,bus,&io_status,0x8c665ee4u,2);if(io_status)return io_status;goto L8c665e66; /* mov.w 0x8c665ee4,r4 */
L8c665e66: *stop=0x8c665e66u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665f04u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e68; /* mov.l 0x8c665f04,r0 */
L8c665e68: target=r[14];*pr=0x8c665e6cu;*stop=0x8c665e6au;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665e6c; /* jsr @r14 */
L8c665e6c: *stop=0x8c665e6cu;r[4]=sx(m,bus,&io_status,0x8c665ee6u,2);if(io_status)return io_status;goto L8c665e6e; /* mov.w 0x8c665ee6,r4 */
L8c665e6e: *stop=0x8c665e6eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c665f08u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e70; /* mov.l 0x8c665f08,r0 */
L8c665e70: target=r[14];*pr=0x8c665e74u;*stop=0x8c665e72u;if(!valid_access(r[0]+r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665e74; /* jsr @r14 */
L8c665e74: *stop=0x8c665e74u;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665e76; /* mov.l @(0x4,r15),r5 */
L8c665e76: *stop=0x8c665e76u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c665e78; /* mov.l @r15,r3 */
L8c665e78: *stop=0x8c665e78u;r[5]+=4294967295u;goto L8c665e7a; /* add #-0x1,r5 */
L8c665e7a: *stop=0x8c665e7au;r[4]=sx(m,bus,&io_status,0x8c665ee8u,2);if(io_status)return io_status;goto L8c665e7c; /* mov.w 0x8c665ee8,r4 */
L8c665e7c: *stop=0x8c665e7cu;r[5]<<=16;goto L8c665e7e; /* shll16 r5 */
L8c665e7e: *stop=0x8c665e7eu;r[5]+=r[3];goto L8c665e80; /* add r3,r5 */
L8c665e80: target=r[14];*pr=0x8c665e84u;*stop=0x8c665e82u;r[5]+=4294967295u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665e84; /* jsr @r14 */
L8c665e84: *stop=0x8c665e84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ef0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665e86; /* mov.l 0x8c665ef0,r3 */
L8c665e86: *stop=0x8c665e86u;r[4]=3u;goto L8c665e88; /* mov #0x3,r4 */
L8c665e88: *stop=0x8c665e88u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[5]=loaded;}goto L8c665e8a; /* mov.l @r3,r5 */
L8c665e8a: *stop=0x8c665e8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ef4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665e8c; /* mov.l 0x8c665ef4,r1 */
L8c665e8c: *stop=0x8c665e8cu;r[5]&=r[4];goto L8c665e8e; /* and r4,r5 */
L8c665e8e: *stop=0x8c665e8eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665e90; /* mov.l @r1,r2 */
L8c665e90: *stop=0x8c665e90u;r[2]&=r[4];goto L8c665e92; /* and r4,r2 */
L8c665e92: *stop=0x8c665e92u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665f0cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665e94; /* mov.l 0x8c665f0c,r0 */
L8c665e94: *stop=0x8c665e94u;r[2]<<=2;goto L8c665e96; /* shll2 r2 */
L8c665e96: *stop=0x8c665e96u;r[2]<<=2;goto L8c665e98; /* shll2 r2 */
L8c665e98: *stop=0x8c665e98u;r[5]|=r[2];goto L8c665e9a; /* or r2,r5 */
L8c665e9a: *stop=0x8c665e9au;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665e9c; /* mov.l @r0,r2 */
L8c665e9c: *stop=0x8c665e9cu;r[2]&=r[4];goto L8c665e9e; /* and r4,r2 */
L8c665e9e: *stop=0x8c665e9eu;r[2]<<=8;goto L8c665ea0; /* shll8 r2 */
L8c665ea0: *stop=0x8c665ea0u;r[5]|=r[2];goto L8c665ea2; /* or r2,r5 */
L8c665ea2: *stop=0x8c665ea2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665f10u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665ea4; /* mov.l 0x8c665f10,r2 */
L8c665ea4: *stop=0x8c665ea4u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c665ea6; /* mov.l @r2,r3 */
L8c665ea6: *stop=0x8c665ea6u;r[3]&=r[4];goto L8c665ea8; /* and r4,r3 */
L8c665ea8: *stop=0x8c665ea8u;r[3]<<=8;goto L8c665eaa; /* shll8 r3 */
L8c665eaa: *stop=0x8c665eaau;r[3]<<=2;goto L8c665eac; /* shll2 r3 */
L8c665eac: *stop=0x8c665eacu;r[3]<<=2;goto L8c665eae; /* shll2 r3 */
L8c665eae: *stop=0x8c665eaeu;r[5]|=r[3];goto L8c665eb0; /* or r3,r5 */
L8c665eb0: *stop=0x8c665eb0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665f14u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665eb2; /* mov.l 0x8c665f14,r3 */
L8c665eb2: *stop=0x8c665eb2u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665eb4; /* mov.l @r3,r0 */
L8c665eb4: *stop=0x8c665eb4u;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c665eb6; /* cmp/eq #0x6,r0 */
L8c665eb6: take=!(*sr&1u);*stop=0x8c665eb8u;r[6]=1u;if(take)goto L8c665f20;goto L8c665eba; /* bf/s 0x8c665f20 */
L8c665eba: *stop=0x8c665ebau;{uint32_t loaded=rd(m,bus,&io_status,0x8c665f18u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665ebc; /* mov.l 0x8c665f18,r1 */
L8c665ebc: *stop=0x8c665ebcu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c665ebe; /* mov.l @r1,r0 */
L8c665ebe: *stop=0x8c665ebeu;r[6]&=r[0];goto L8c665ec0; /* and r0,r6 */
L8c665ec0: *stop=0x8c665ec0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665f1cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665ec2; /* mov.l 0x8c665f1c,r0 */
L8c665ec2: *stop=0x8c665ec2u;r[6]<<=16;goto L8c665ec4; /* shll16 r6 */
L8c665ec4: *stop=0x8c665ec4u;r[6]<<=2;goto L8c665ec6; /* shll2 r6 */
L8c665ec6: *stop=0x8c665ec6u;r[6]<<=2;goto L8c665ec8; /* shll2 r6 */
L8c665ec8: *stop=0x8c665ec8u;r[5]|=r[6];goto L8c665eca; /* or r6,r5 */
L8c665eca: *stop=0x8c665ecau;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c665ecc; /* mov.l @r0,r2 */
L8c665ecc: *stop=0x8c665eccu;r[4]&=r[2];goto L8c665ece; /* and r2,r4 */
L8c665ece: *stop=0x8c665eceu;r[4]<<=16;goto L8c665ed0; /* shll16 r4 */
L8c665ed0: *stop=0x8c665ed2u;r[5]|=r[4];goto L8c665f2a; /* bra 0x8c665f2a */
L8c665f20: *stop=0x8c665f20u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665fe8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c665f22; /* mov.l 0x8c665fe8,r0 */
L8c665f22: *stop=0x8c665f22u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c665f24; /* mov.l @r0,r1 */
L8c665f24: *stop=0x8c665f24u;r[6]&=r[1];goto L8c665f26; /* and r1,r6 */
L8c665f26: *stop=0x8c665f26u;r[6]<<=16;goto L8c665f28; /* shll16 r6 */
L8c665f28: *stop=0x8c665f28u;r[5]|=r[6];goto L8c665f2a; /* or r6,r5 */
L8c665f2a: *stop=0x8c665f2au;r[4]=sx(m,bus,&io_status,0x8c665fe0u,2);if(io_status)return io_status;goto L8c665f2c; /* mov.w 0x8c665fe0,r4 */
L8c665f2c: target=r[14];*pr=0x8c665f30u;*stop=0x8c665f2eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665f30; /* jsr @r14 */
L8c665f30: *stop=0x8c665f30u;r[4]=sx(m,bus,&io_status,0x8c665fe2u,2);if(io_status)return io_status;goto L8c665f32; /* mov.w 0x8c665fe2,r4 */
L8c665f32: *stop=0x8c665f32u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665fecu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c665f34; /* mov.l 0x8c665fec,r5 */
L8c665f34: target=r[14];*pr=0x8c665f38u;*stop=0x8c665f36u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c665f38; /* jsr @r14 */
L8c665f38: *stop=0x8c665f38u;r[15]+=12u;goto L8c665f3a; /* add #0xc,r15 */
L8c665f3a: *stop=0x8c665f3au;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c665f3c; /* lds.l @r15+,PR */
L8c665f3c: *stop=0x8c665f3cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c665f3e; /* mov.l @r15+,r13 */
L8c665f3e: *stop=0x8c665f3eu;if(*pr!=entry_pr)return -3;*stop=0x8c665f40u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_interrupt_8c665f42(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c665f42;
L8c665f42: *stop=0x8c665f42u;r[3]=r[4];goto L8c665f44; /* mov r4,r3 */
L8c665f44: *stop=0x8c665f44u;r[3]-=r[5];goto L8c665f46; /* sub r5,r3 */
L8c665f46: *stop=0x8c665f46u;r[3]>>=2;goto L8c665f48; /* shlr2 r3 */
L8c665f48: *stop=0x8c665f48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ff4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c665f4a; /* mov.l 0x8c665ff4,r1 */
L8c665f4a: *stop=0x8c665f4au;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ff0u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c665f4c; /* mov.l 0x8c665ff0,r6 */
L8c665f4c: *stop=0x8c665f4cu;r[3]+=2u;goto L8c665f4e; /* add #0x2,r3 */
L8c665f4e: *stop=0x8c665f4eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ff8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665f50; /* mov.l 0x8c665ff8,r2 */
L8c665f50: *stop=0x8c665f50u;r[3]|=r[1];goto L8c665f52; /* or r1,r3 */
L8c665f52: *stop=0x8c665f52u;r[6]|=r[4];goto L8c665f54; /* or r4,r6 */
L8c665f54: *stop=0x8c665f54u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c665f56; /* fldi0 fr4 */
L8c665f56: *stop=0x8c665f56u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c665f58; /* mov.l r3,@r6 */
L8c665f58: *stop=0x8c665f58u;r[0]=12u;goto L8c665f5a; /* mov #0xc,r0 */
L8c665f5a: *stop=0x8c665f5au;if(!valid_access(r[6]+4u,4u))return -3;wr(m,bus,&io_status,r[6]+0x4u,r[2],4);if(io_status)return io_status;goto L8c665f5c; /* mov.l r2,@(0x4,r6) */
L8c665f5c: *stop=0x8c665f5cu;r[6]+=8u;goto L8c665f5e; /* add #0x8,r6 */
L8c665f5e: *stop=0x8c665f5eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c665ffcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665f60; /* mov.l 0x8c665ffc,r3 */
L8c665f60: *stop=0x8c665f60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666000u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c665f62; /* mov.l 0x8c666000,r2 */
L8c665f62: *stop=0x8c665f62u;if(!valid_access(r[6],4u))return -3;wr(m,bus,&io_status,r[6],r[3],4);if(io_status)return io_status;goto L8c665f64; /* mov.l r3,@r6 */
L8c665f64: *stop=0x8c665f64u;r[3]=0u;goto L8c665f66; /* mov #0x0,r3 */
L8c665f66: *stop=0x8c665f66u;if(!valid_access(r[6]+4u,4u))return -3;wr(m,bus,&io_status,r[6]+0x4u,r[2],4);if(io_status)return io_status;goto L8c665f68; /* mov.l r2,@(0x4,r6) */
L8c665f68: *stop=0x8c665f68u;if(!valid_access(r[6]+8u,4u))return -3;wr(m,bus,&io_status,r[6]+0x8u,r[3],4);if(io_status)return io_status;goto L8c665f6a; /* mov.l r3,@(0x8,r6) */
L8c665f6a: *stop=0x8c665f6au;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c665f6c; /* fmov fr4,@(r0,r6) */
L8c665f6c: *stop=0x8c665f6cu;r[0]=16u;goto L8c665f6e; /* mov #0x10,r0 */
L8c665f6e: *stop=0x8c665f6eu;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c665f70; /* fmov fr4,@(r0,r6) */
L8c665f70: *stop=0x8c665f70u;r[0]=0x8c666004u;goto L8c665f72; /* mova 0x8c666004,r0 */
L8c665f72: *stop=0x8c665f72u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,5u,r[0]);if(io_status)return io_status;goto L8c665f74; /* fmov.s @r0,fr5 */
L8c665f74: *stop=0x8c665f74u;r[0]=20u;goto L8c665f76; /* mov #0x14,r0 */
L8c665f76: *stop=0x8c665f76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666008u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665f78; /* mov.l 0x8c666008,r3 */
L8c665f78: *stop=0x8c665f78u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c665f7a; /* fmov fr5,@(r0,r6) */
L8c665f7a: *stop=0x8c665f7au;r[0]=28u;goto L8c665f7c; /* mov #0x1c,r0 */
L8c665f7c: *stop=0x8c665f7cu;if(!valid_access(r[6]+24u,4u))return -3;wr(m,bus,&io_status,r[6]+0x18u,r[3],4);if(io_status)return io_status;goto L8c665f7e; /* mov.l r3,@(0x18,r6) */
L8c665f7e: *stop=0x8c665f7eu;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c665f80; /* fmov fr4,@(r0,r6) */
L8c665f80: *stop=0x8c665f80u;r[0]=0x8c66600cu;goto L8c665f82; /* mova 0x8c66600c,r0 */
L8c665f82: *stop=0x8c665f82u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c665f84; /* fmov.s @r0,fr3 */
L8c665f84: *stop=0x8c665f84u;r[0]=32u;goto L8c665f86; /* mov #0x20,r0 */
L8c665f86: *stop=0x8c665f86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666010u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665f88; /* mov.l 0x8c666010,r3 */
L8c665f88: *stop=0x8c665f88u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c665f8a; /* fmov fr3,@(r0,r6) */
L8c665f8a: *stop=0x8c665f8au;r[0]=36u;goto L8c665f8c; /* mov #0x24,r0 */
L8c665f8c: *stop=0x8c665f8cu;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c665f8e; /* fmov fr5,@(r0,r6) */
L8c665f8e: *stop=0x8c665f8eu;r[0]=0x8c666014u;goto L8c665f90; /* mova 0x8c666014,r0 */
L8c665f90: *stop=0x8c665f90u;if(!valid_access(r[6]+40u,4u))return -3;wr(m,bus,&io_status,r[6]+0x28u,r[3],4);if(io_status)return io_status;goto L8c665f92; /* mov.l r3,@(0x28,r6) */
L8c665f92: *stop=0x8c665f92u;if(!valid_access(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c665f94; /* fmov.s @r0,fr3 */
L8c665f94: *stop=0x8c665f94u;r[0]=44u;goto L8c665f96; /* mov #0x2c,r0 */
L8c665f96: *stop=0x8c665f96u;{uint32_t loaded=rd(m,bus,&io_status,0x8c666018u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c665f98; /* mov.l 0x8c666018,r3 */
L8c665f98: *stop=0x8c665f98u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c665f9a; /* fmov fr3,@(r0,r6) */
L8c665f9a: *stop=0x8c665f9au;r[0]=48u;goto L8c665f9c; /* mov #0x30,r0 */
L8c665f9c: *stop=0x8c665f9cu;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c665f9e; /* fmov fr4,@(r0,r6) */
L8c665f9e: *stop=0x8c665f9eu;r[0]=52u;goto L8c665fa0; /* mov #0x34,r0 */
L8c665fa0: *stop=0x8c665fa0u;if(!valid_access(r[0]+r[6],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c665fa2; /* fmov fr5,@(r0,r6) */
L8c665fa2: *stop=0x8c665fa2u;if(!valid_access(r[6]+56u,4u))return -3;wr(m,bus,&io_status,r[6]+0x38u,r[3],4);if(io_status)return io_status;goto L8c665fa4; /* mov.l r3,@(0x38,r6) */
L8c665fa4: *stop=0x8c665fa4u;if(*pr!=entry_pr)return -3;*stop=0x8c665fa6u;r[0]=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_interrupt_8c10cb10(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10cb10;
L8c10cb10: *stop=0x8c10cb10u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c10cb12; /* tst r0,r0 */
L8c10cb12: *stop=0x8c10cb12u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c10cb14; /* mov.l r2,@-r15 */
L8c10cb14: if((*sr&1u))goto L8c10cba2;goto L8c10cb16; /* bt 0x8c10cba2 */
L8c10cb16: *stop=0x8c10cb16u;r[2]=0u;goto L8c10cb18; /* mov #0x0,r2 */
L8c10cb18: *stop=0x8c10cb18u;*sr&=~0x301u;goto L8c10cb1a; /* div0u */
L8c10cb1a: *stop=0x8c10cb1au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb1c; /* rotcl r1 */
L8c10cb1c: *stop=0x8c10cb1cu;div_step(r[0],&r[2],sr);goto L8c10cb1e; /* div1 r0,r2 */
L8c10cb1e: *stop=0x8c10cb1eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb20; /* rotcl r1 */
L8c10cb20: *stop=0x8c10cb20u;div_step(r[0],&r[2],sr);goto L8c10cb22; /* div1 r0,r2 */
L8c10cb22: *stop=0x8c10cb22u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb24; /* rotcl r1 */
L8c10cb24: *stop=0x8c10cb24u;div_step(r[0],&r[2],sr);goto L8c10cb26; /* div1 r0,r2 */
L8c10cb26: *stop=0x8c10cb26u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb28; /* rotcl r1 */
L8c10cb28: *stop=0x8c10cb28u;div_step(r[0],&r[2],sr);goto L8c10cb2a; /* div1 r0,r2 */
L8c10cb2a: *stop=0x8c10cb2au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb2c; /* rotcl r1 */
L8c10cb2c: *stop=0x8c10cb2cu;div_step(r[0],&r[2],sr);goto L8c10cb2e; /* div1 r0,r2 */
L8c10cb2e: *stop=0x8c10cb2eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb30; /* rotcl r1 */
L8c10cb30: *stop=0x8c10cb30u;div_step(r[0],&r[2],sr);goto L8c10cb32; /* div1 r0,r2 */
L8c10cb32: *stop=0x8c10cb32u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb34; /* rotcl r1 */
L8c10cb34: *stop=0x8c10cb34u;div_step(r[0],&r[2],sr);goto L8c10cb36; /* div1 r0,r2 */
L8c10cb36: *stop=0x8c10cb36u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb38; /* rotcl r1 */
L8c10cb38: *stop=0x8c10cb38u;div_step(r[0],&r[2],sr);goto L8c10cb3a; /* div1 r0,r2 */
L8c10cb3a: *stop=0x8c10cb3au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb3c; /* rotcl r1 */
L8c10cb3c: *stop=0x8c10cb3cu;div_step(r[0],&r[2],sr);goto L8c10cb3e; /* div1 r0,r2 */
L8c10cb3e: *stop=0x8c10cb3eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb40; /* rotcl r1 */
L8c10cb40: *stop=0x8c10cb40u;div_step(r[0],&r[2],sr);goto L8c10cb42; /* div1 r0,r2 */
L8c10cb42: *stop=0x8c10cb42u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb44; /* rotcl r1 */
L8c10cb44: *stop=0x8c10cb44u;div_step(r[0],&r[2],sr);goto L8c10cb46; /* div1 r0,r2 */
L8c10cb46: *stop=0x8c10cb46u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb48; /* rotcl r1 */
L8c10cb48: *stop=0x8c10cb48u;div_step(r[0],&r[2],sr);goto L8c10cb4a; /* div1 r0,r2 */
L8c10cb4a: *stop=0x8c10cb4au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb4c; /* rotcl r1 */
L8c10cb4c: *stop=0x8c10cb4cu;div_step(r[0],&r[2],sr);goto L8c10cb4e; /* div1 r0,r2 */
L8c10cb4e: *stop=0x8c10cb4eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb50; /* rotcl r1 */
L8c10cb50: *stop=0x8c10cb50u;div_step(r[0],&r[2],sr);goto L8c10cb52; /* div1 r0,r2 */
L8c10cb52: *stop=0x8c10cb52u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb54; /* rotcl r1 */
L8c10cb54: *stop=0x8c10cb54u;div_step(r[0],&r[2],sr);goto L8c10cb56; /* div1 r0,r2 */
L8c10cb56: *stop=0x8c10cb56u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb58; /* rotcl r1 */
L8c10cb58: *stop=0x8c10cb58u;div_step(r[0],&r[2],sr);goto L8c10cb5a; /* div1 r0,r2 */
L8c10cb5a: *stop=0x8c10cb5au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb5c; /* rotcl r1 */
L8c10cb5c: *stop=0x8c10cb5cu;div_step(r[0],&r[2],sr);goto L8c10cb5e; /* div1 r0,r2 */
L8c10cb5e: *stop=0x8c10cb5eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb60; /* rotcl r1 */
L8c10cb60: *stop=0x8c10cb60u;div_step(r[0],&r[2],sr);goto L8c10cb62; /* div1 r0,r2 */
L8c10cb62: *stop=0x8c10cb62u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb64; /* rotcl r1 */
L8c10cb64: *stop=0x8c10cb64u;div_step(r[0],&r[2],sr);goto L8c10cb66; /* div1 r0,r2 */
L8c10cb66: *stop=0x8c10cb66u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb68; /* rotcl r1 */
L8c10cb68: *stop=0x8c10cb68u;div_step(r[0],&r[2],sr);goto L8c10cb6a; /* div1 r0,r2 */
L8c10cb6a: *stop=0x8c10cb6au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb6c; /* rotcl r1 */
L8c10cb6c: *stop=0x8c10cb6cu;div_step(r[0],&r[2],sr);goto L8c10cb6e; /* div1 r0,r2 */
L8c10cb6e: *stop=0x8c10cb6eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb70; /* rotcl r1 */
L8c10cb70: *stop=0x8c10cb70u;div_step(r[0],&r[2],sr);goto L8c10cb72; /* div1 r0,r2 */
L8c10cb72: *stop=0x8c10cb72u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb74; /* rotcl r1 */
L8c10cb74: *stop=0x8c10cb74u;div_step(r[0],&r[2],sr);goto L8c10cb76; /* div1 r0,r2 */
L8c10cb76: *stop=0x8c10cb76u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb78; /* rotcl r1 */
L8c10cb78: *stop=0x8c10cb78u;div_step(r[0],&r[2],sr);goto L8c10cb7a; /* div1 r0,r2 */
L8c10cb7a: *stop=0x8c10cb7au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb7c; /* rotcl r1 */
L8c10cb7c: *stop=0x8c10cb7cu;div_step(r[0],&r[2],sr);goto L8c10cb7e; /* div1 r0,r2 */
L8c10cb7e: *stop=0x8c10cb7eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb80; /* rotcl r1 */
L8c10cb80: *stop=0x8c10cb80u;div_step(r[0],&r[2],sr);goto L8c10cb82; /* div1 r0,r2 */
L8c10cb82: *stop=0x8c10cb82u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb84; /* rotcl r1 */
L8c10cb84: *stop=0x8c10cb84u;div_step(r[0],&r[2],sr);goto L8c10cb86; /* div1 r0,r2 */
L8c10cb86: *stop=0x8c10cb86u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb88; /* rotcl r1 */
L8c10cb88: *stop=0x8c10cb88u;div_step(r[0],&r[2],sr);goto L8c10cb8a; /* div1 r0,r2 */
L8c10cb8a: *stop=0x8c10cb8au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb8c; /* rotcl r1 */
L8c10cb8c: *stop=0x8c10cb8cu;div_step(r[0],&r[2],sr);goto L8c10cb8e; /* div1 r0,r2 */
L8c10cb8e: *stop=0x8c10cb8eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb90; /* rotcl r1 */
L8c10cb90: *stop=0x8c10cb90u;div_step(r[0],&r[2],sr);goto L8c10cb92; /* div1 r0,r2 */
L8c10cb92: *stop=0x8c10cb92u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb94; /* rotcl r1 */
L8c10cb94: *stop=0x8c10cb94u;div_step(r[0],&r[2],sr);goto L8c10cb96; /* div1 r0,r2 */
L8c10cb96: *stop=0x8c10cb96u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb98; /* rotcl r1 */
L8c10cb98: *stop=0x8c10cb98u;div_step(r[0],&r[2],sr);goto L8c10cb9a; /* div1 r0,r2 */
L8c10cb9a: *stop=0x8c10cb9au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cb9c; /* rotcl r1 */
L8c10cb9c: *stop=0x8c10cb9cu;r[0]=r[1];goto L8c10cb9e; /* mov r1,r0 */
L8c10cb9e: *stop=0x8c10cb9eu;if(*pr!=entry_pr)return -3;*stop=0x8c10cba0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;return 0; /* rts */
L8c10cba2: *stop=0x8c10cba2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10cbb0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10cba4; /* mov.l 0x8c10cbb0,r2 */
L8c10cba4: *stop=0x8c10cba4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10cbb4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10cba6; /* mov.l 0x8c10cbb4,r1 */
L8c10cba6: *stop=0x8c10cba6u;r[0]=0u;goto L8c10cba8; /* mov #0x0,r0 */
L8c10cba8: *stop=0x8c10cba8u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c10cbaa; /* mov.l r1,@r2 */
L8c10cbaa: *stop=0x8c10cbaau;if(*pr!=entry_pr)return -3;*stop=0x8c10cbacu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_interrupt_8c10cc80(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10cc80;
L8c10cc80: *stop=0x8c10cc80u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c10cc82; /* tst r0,r0 */
L8c10cc82: if((*sr&1u))goto L8c10cd20;goto L8c10cc84; /* bt 0x8c10cd20 */
L8c10cc84: *stop=0x8c10cc84u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10cc86; /* mov.l r3,@-r15 */
L8c10cc86: *stop=0x8c10cc86u;r[3]=0u;goto L8c10cc88; /* mov #0x0,r3 */
L8c10cc88: *stop=0x8c10cc88u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c10cc8a; /* mov.l r4,@-r15 */
L8c10cc8a: *stop=0x8c10cc8au;r[4]=r[0];goto L8c10cc8c; /* mov r0,r4 */
L8c10cc8c: *stop=0x8c10cc8cu;*sr&=~0x301u;goto L8c10cc8e; /* div0u */
L8c10cc8e: *stop=0x8c10cc8eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc90; /* rotcl r1 */
L8c10cc90: *stop=0x8c10cc90u;div_step(r[4],&r[3],sr);goto L8c10cc92; /* div1 r4,r3 */
L8c10cc92: *stop=0x8c10cc92u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc94; /* rotcl r1 */
L8c10cc94: *stop=0x8c10cc94u;div_step(r[4],&r[3],sr);goto L8c10cc96; /* div1 r4,r3 */
L8c10cc96: *stop=0x8c10cc96u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc98; /* rotcl r1 */
L8c10cc98: *stop=0x8c10cc98u;div_step(r[4],&r[3],sr);goto L8c10cc9a; /* div1 r4,r3 */
L8c10cc9a: *stop=0x8c10cc9au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc9c; /* rotcl r1 */
L8c10cc9c: *stop=0x8c10cc9cu;div_step(r[4],&r[3],sr);goto L8c10cc9e; /* div1 r4,r3 */
L8c10cc9e: *stop=0x8c10cc9eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cca0; /* rotcl r1 */
L8c10cca0: *stop=0x8c10cca0u;div_step(r[4],&r[3],sr);goto L8c10cca2; /* div1 r4,r3 */
L8c10cca2: *stop=0x8c10cca2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cca4; /* rotcl r1 */
L8c10cca4: *stop=0x8c10cca4u;div_step(r[4],&r[3],sr);goto L8c10cca6; /* div1 r4,r3 */
L8c10cca6: *stop=0x8c10cca6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cca8; /* rotcl r1 */
L8c10cca8: *stop=0x8c10cca8u;div_step(r[4],&r[3],sr);goto L8c10ccaa; /* div1 r4,r3 */
L8c10ccaa: *stop=0x8c10ccaau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccac; /* rotcl r1 */
L8c10ccac: *stop=0x8c10ccacu;div_step(r[4],&r[3],sr);goto L8c10ccae; /* div1 r4,r3 */
L8c10ccae: *stop=0x8c10ccaeu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccb0; /* rotcl r1 */
L8c10ccb0: *stop=0x8c10ccb0u;div_step(r[4],&r[3],sr);goto L8c10ccb2; /* div1 r4,r3 */
L8c10ccb2: *stop=0x8c10ccb2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccb4; /* rotcl r1 */
L8c10ccb4: *stop=0x8c10ccb4u;div_step(r[4],&r[3],sr);goto L8c10ccb6; /* div1 r4,r3 */
L8c10ccb6: *stop=0x8c10ccb6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccb8; /* rotcl r1 */
L8c10ccb8: *stop=0x8c10ccb8u;div_step(r[4],&r[3],sr);goto L8c10ccba; /* div1 r4,r3 */
L8c10ccba: *stop=0x8c10ccbau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccbc; /* rotcl r1 */
L8c10ccbc: *stop=0x8c10ccbcu;div_step(r[4],&r[3],sr);goto L8c10ccbe; /* div1 r4,r3 */
L8c10ccbe: *stop=0x8c10ccbeu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccc0; /* rotcl r1 */
L8c10ccc0: *stop=0x8c10ccc0u;div_step(r[4],&r[3],sr);goto L8c10ccc2; /* div1 r4,r3 */
L8c10ccc2: *stop=0x8c10ccc2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccc4; /* rotcl r1 */
L8c10ccc4: *stop=0x8c10ccc4u;div_step(r[4],&r[3],sr);goto L8c10ccc6; /* div1 r4,r3 */
L8c10ccc6: *stop=0x8c10ccc6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccc8; /* rotcl r1 */
L8c10ccc8: *stop=0x8c10ccc8u;div_step(r[4],&r[3],sr);goto L8c10ccca; /* div1 r4,r3 */
L8c10ccca: *stop=0x8c10cccau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cccc; /* rotcl r1 */
L8c10cccc: *stop=0x8c10ccccu;div_step(r[4],&r[3],sr);goto L8c10ccce; /* div1 r4,r3 */
L8c10ccce: *stop=0x8c10ccceu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccd0; /* rotcl r1 */
L8c10ccd0: *stop=0x8c10ccd0u;div_step(r[4],&r[3],sr);goto L8c10ccd2; /* div1 r4,r3 */
L8c10ccd2: *stop=0x8c10ccd2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccd4; /* rotcl r1 */
L8c10ccd4: *stop=0x8c10ccd4u;div_step(r[4],&r[3],sr);goto L8c10ccd6; /* div1 r4,r3 */
L8c10ccd6: *stop=0x8c10ccd6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccd8; /* rotcl r1 */
L8c10ccd8: *stop=0x8c10ccd8u;div_step(r[4],&r[3],sr);goto L8c10ccda; /* div1 r4,r3 */
L8c10ccda: *stop=0x8c10ccdau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccdc; /* rotcl r1 */
L8c10ccdc: *stop=0x8c10ccdcu;div_step(r[4],&r[3],sr);goto L8c10ccde; /* div1 r4,r3 */
L8c10ccde: *stop=0x8c10ccdeu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cce0; /* rotcl r1 */
L8c10cce0: *stop=0x8c10cce0u;div_step(r[4],&r[3],sr);goto L8c10cce2; /* div1 r4,r3 */
L8c10cce2: *stop=0x8c10cce2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cce4; /* rotcl r1 */
L8c10cce4: *stop=0x8c10cce4u;div_step(r[4],&r[3],sr);goto L8c10cce6; /* div1 r4,r3 */
L8c10cce6: *stop=0x8c10cce6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cce8; /* rotcl r1 */
L8c10cce8: *stop=0x8c10cce8u;div_step(r[4],&r[3],sr);goto L8c10ccea; /* div1 r4,r3 */
L8c10ccea: *stop=0x8c10cceau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccec; /* rotcl r1 */
L8c10ccec: *stop=0x8c10ccecu;div_step(r[4],&r[3],sr);goto L8c10ccee; /* div1 r4,r3 */
L8c10ccee: *stop=0x8c10cceeu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccf0; /* rotcl r1 */
L8c10ccf0: *stop=0x8c10ccf0u;div_step(r[4],&r[3],sr);goto L8c10ccf2; /* div1 r4,r3 */
L8c10ccf2: *stop=0x8c10ccf2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccf4; /* rotcl r1 */
L8c10ccf4: *stop=0x8c10ccf4u;div_step(r[4],&r[3],sr);goto L8c10ccf6; /* div1 r4,r3 */
L8c10ccf6: *stop=0x8c10ccf6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccf8; /* rotcl r1 */
L8c10ccf8: *stop=0x8c10ccf8u;div_step(r[4],&r[3],sr);goto L8c10ccfa; /* div1 r4,r3 */
L8c10ccfa: *stop=0x8c10ccfau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10ccfc; /* rotcl r1 */
L8c10ccfc: *stop=0x8c10ccfcu;div_step(r[4],&r[3],sr);goto L8c10ccfe; /* div1 r4,r3 */
L8c10ccfe: *stop=0x8c10ccfeu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cd00; /* rotcl r1 */
L8c10cd00: *stop=0x8c10cd00u;div_step(r[4],&r[3],sr);goto L8c10cd02; /* div1 r4,r3 */
L8c10cd02: *stop=0x8c10cd02u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cd04; /* rotcl r1 */
L8c10cd04: *stop=0x8c10cd04u;div_step(r[4],&r[3],sr);goto L8c10cd06; /* div1 r4,r3 */
L8c10cd06: *stop=0x8c10cd06u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cd08; /* rotcl r1 */
L8c10cd08: *stop=0x8c10cd08u;div_step(r[4],&r[3],sr);goto L8c10cd0a; /* div1 r4,r3 */
L8c10cd0a: *stop=0x8c10cd0au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cd0c; /* rotcl r1 */
L8c10cd0c: *stop=0x8c10cd0cu;div_step(r[4],&r[3],sr);goto L8c10cd0e; /* div1 r4,r3 */
L8c10cd0e: if(!(*sr&1u))goto L8c10cd18;goto L8c10cd10; /* bf 0x8c10cd18 */
L8c10cd10: *stop=0x8c10cd10u;r[0]=r[3];goto L8c10cd12; /* mov r3,r0 */
L8c10cd12: *stop=0x8c10cd12u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}r[15]+=4;goto L8c10cd14; /* mov.l @r15+,r4 */
L8c10cd14: *stop=0x8c10cd14u;if(*pr!=entry_pr)return -3;*stop=0x8c10cd16u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;return 0; /* rts */
L8c10cd18: *stop=0x8c10cd18u;r[0]+=r[3];goto L8c10cd1a; /* add r3,r0 */
L8c10cd1a: *stop=0x8c10cd1au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}r[15]+=4;goto L8c10cd1c; /* mov.l @r15+,r4 */
L8c10cd1c: *stop=0x8c10cd1cu;if(*pr!=entry_pr)return -3;*stop=0x8c10cd1eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;return 0; /* rts */
L8c10cd20: *stop=0x8c10cd20u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c10cd22; /* mov.l r2,@-r15 */
L8c10cd22: *stop=0x8c10cd22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10cd30u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10cd24; /* mov.l 0x8c10cd30,r1 */
L8c10cd24: *stop=0x8c10cd24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c10cd34u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10cd26; /* mov.l 0x8c10cd34,r2 */
L8c10cd26: *stop=0x8c10cd26u;r[0]=0u;goto L8c10cd28; /* mov #0x0,r0 */
L8c10cd28: *stop=0x8c10cd28u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c10cd2a; /* mov.l r2,@r1 */
L8c10cd2a: *stop=0x8c10cd2au;if(*pr!=entry_pr)return -3;*stop=0x8c10cd2cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_interrupt_8c65347c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65347c;
L8c65347c: *stop=0x8c65347cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c65347e; /* sts.l PR,@-r15 */
L8c65347e: *stop=0x8c65347eu;r[15]+=4294967292u;goto L8c653480; /* add #-0x4,r15 */
L8c653480: *stop=0x8c653480u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6534ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c653482; /* mov.l 0x8c6534cc,r2 */
L8c653482: *stop=0x8c653482u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6534c8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c653484; /* mov.l 0x8c6534c8,r3 */
L8c653484: *stop=0x8c653484u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c653486; /* mov.l r4,@r15 */
L8c653486: target=r[2];*pr=0x8c65348au;*stop=0x8c653488u;r[4]+=r[3];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c65348a; /* jsr @r2 */
L8c65348a: *stop=0x8c65348au;r[15]+=4u;goto L8c65348c; /* add #0x4,r15 */
L8c65348c: *stop=0x8c65348cu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c65348e; /* lds.l @r15+,PR */
L8c65348e: *stop=0x8c65348eu;if(*pr!=entry_pr)return -3;*stop=0x8c653490u;r[0]=1u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_interrupt_8c65253e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c65253e;
L8c65253e: *stop=0x8c65253eu;r[6]>>=2;goto L8c652540; /* shlr2 r6 */
L8c652540: *stop=0x8c652540u;if(!valid_access(r[5],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;r[0]=loaded;}r[5]+=4;goto L8c652542; /* mov.l @r5+,r0 */
L8c652542: *stop=0x8c652542u;r[6]--;*sr=(*sr&~1u)|(!r[6]);goto L8c652544; /* dt r6 */
L8c652544: *stop=0x8c652544u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[0],4);if(io_status)return io_status;goto L8c652546; /* mov.l r0,@r4 */
L8c652546: take=!(*sr&1u);*stop=0x8c652548u;r[4]+=4u;if(take)goto L8c652540;goto L8c65254a; /* bf/s 0x8c652540 */
L8c65254a: *stop=0x8c65254au;if(*pr!=entry_pr)return -3;*stop=0x8c65254cu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_interrupt_8c641f00(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 unsigned wait_visits[3]={0};(void)wait_visits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c641f00;
L8c641f00: *stop=0x8c641f00u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c641f02; /* mov.l r14,@-r15 */
L8c641f02: *stop=0x8c641f02u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c641f04; /* mov.l r13,@-r15 */
L8c641f04: *stop=0x8c641f04u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c641f06; /* mov.l r12,@-r15 */
L8c641f06: *stop=0x8c641f06u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c641f08; /* mov.l r11,@-r15 */
L8c641f08: *stop=0x8c641f08u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c641f0a; /* mov.l r10,@-r15 */
L8c641f0a: *stop=0x8c641f0au;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c641f0c; /* mov.l r9,@-r15 */
L8c641f0c: *stop=0x8c641f0cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c641f0e; /* mov.l r8,@-r15 */
L8c641f0e: *stop=0x8c641f0eu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c641f10; /* sts.l PR,@-r15 */
L8c641f10: *stop=0x8c641f10u;r[15]+=4294967288u;goto L8c641f12; /* add #-0x8,r15 */
L8c641f12: *stop=0x8c641f12u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fa0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641f14; /* mov.l 0x8c641fa0,r2 */
L8c641f14: *stop=0x8c641f14u;r[13]=0u;goto L8c641f16; /* mov #0x0,r13 */
L8c641f16: *stop=0x8c641f16u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f18; /* mov.l @r2,r3 */
L8c641f18: *stop=0x8c641f18u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c641f1a; /* tst r3,r3 */
L8c641f1a: take=(*sr&1u);*stop=0x8c641f1cu;r[14]=1u;if(take)goto L8c641f22;goto L8c641f1e; /* bt/s 0x8c641f22 */
L8c641f1e: *stop=0x8c641f20u;goto L8c64204a; /* bra 0x8c64204a */
L8c641f22: *stop=0x8c641f22u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fa4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641f24; /* mov.l 0x8c641fa4,r1 */
L8c641f24: *stop=0x8c641f24u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f26; /* mov.l @r1,r3 */
L8c641f26: *stop=0x8c641f26u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c641f28; /* tst r3,r3 */
L8c641f28: if(!(*sr&1u))goto L8c641f3a;goto L8c641f2a; /* bf 0x8c641f3a */
L8c641f2a: *stop=0x8c641f2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fa8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c641f2c; /* mov.l 0x8c641fa8,r0 */
L8c641f2c: *stop=0x8c641f2cu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f2e; /* mov.l @r0,r3 */
L8c641f2e: *stop=0x8c641f2eu;r[0]+=4294967280u;goto L8c641f30; /* add #-0x10,r0 */
L8c641f30: *stop=0x8c641f30u;r[3]<<=2;goto L8c641f32; /* shll2 r3 */
L8c641f32: *stop=0x8c641f32u;if(!valid_access(r[0]+r[3],4u))return -3;wr(m,bus,&io_status,r[3]+r[0],r[14],4);if(io_status)return io_status;goto L8c641f34; /* mov.l r14,@(r0,r3) */
L8c641f34: *stop=0x8c641f34u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641facu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f36; /* mov.l 0x8c641fac,r3 */
L8c641f36: *stop=0x8c641f38u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c64204a; /* bra 0x8c64204a */
L8c641f3a: *stop=0x8c641f3au;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fb4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c641f3c; /* mov.l 0x8c641fb4,r0 */
L8c641f3c: *stop=0x8c641f3cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fb0u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c641f3e; /* mov.l 0x8c641fb0,r5 */
L8c641f3e: *stop=0x8c641f3eu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f40; /* mov.l @r0,r3 */
L8c641f40: *stop=0x8c641f40u;r[0]=72u;goto L8c641f42; /* mov #0x48,r0 */
L8c641f42: *stop=0x8c641f42u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fa8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641f44; /* mov.l 0x8c641fa8,r2 */
L8c641f44: *stop=0x8c641f44u;if(!valid_access(r[0]+r[5],4u))return -3;wr(m,bus,&io_status,r[5]+r[0],r[3],4);if(io_status)return io_status;goto L8c641f46; /* mov.l r3,@(r0,r5) */
L8c641f46: *stop=0x8c641f46u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}goto L8c641f48; /* mov.l @r2,r4 */
L8c641f48: *stop=0x8c641f48u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fb8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f4a; /* mov.l 0x8c641fb8,r3 */
L8c641f4a: *stop=0x8c641f4au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c641f4c; /* mov.l r4,@r3 */
L8c641f4c: *stop=0x8c641f4cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fbcu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641f4e; /* mov.l 0x8c641fbc,r1 */
L8c641f4e: *stop=0x8c641f4eu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c641f50; /* mov.l r14,@r1 */
L8c641f50: *stop=0x8c641f50u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fc0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641f52; /* mov.l 0x8c641fc0,r2 */
L8c641f52: *stop=0x8c641f52u;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c641f54; /* mov.l r13,@r2 */
L8c641f54: *stop=0x8c641f54u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fc4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c641f56; /* mov.l 0x8c641fc4,r0 */
L8c641f56: *stop=0x8c641f56u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f58; /* mov.l @r0,r3 */
L8c641f58: *stop=0x8c641f58u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fc8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641f5a; /* mov.l 0x8c641fc8,r1 */
L8c641f5a: *stop=0x8c641f5au;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c641f5c; /* mov.l r3,@r1 */
L8c641f5c: *stop=0x8c641f5cu;r[2]+=8u;goto L8c641f5e; /* add #0x8,r2 */
L8c641f5e: *stop=0x8c641f5eu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[14],4);if(io_status)return io_status;goto L8c641f60; /* mov.l r14,@r2 */
L8c641f60: *stop=0x8c641f60u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fccu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c641f62; /* mov.l 0x8c641fcc,r0 */
L8c641f62: *stop=0x8c641f62u;r[3]=2u;goto L8c641f64; /* mov #0x2,r3 */
L8c641f64: *stop=0x8c641f64u;r[4]<<=2;goto L8c641f66; /* shll2 r4 */
L8c641f66: *stop=0x8c641f66u;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[13],4);if(io_status)return io_status;goto L8c641f68; /* mov.l r13,@(0x4,r5) */
L8c641f68: *stop=0x8c641f68u;if(!valid_access(r[0]+r[4],4u))return -3;wr(m,bus,&io_status,r[4]+r[0],r[3],4);if(io_status)return io_status;goto L8c641f6a; /* mov.l r3,@(r0,r4) */
L8c641f6a: *stop=0x8c641f6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fd0u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c641f6c; /* mov.l 0x8c641fd0,r1 */
L8c641f6c: *stop=0x8c641f6cu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c641f6e; /* mov.l @r1,r0 */
L8c641f6e: *stop=0x8c641f6eu;*sr=(*sr&~1u)|!(r[0]&0x20u);goto L8c641f70; /* tst #0x20,r0 */
L8c641f70: if((*sr&1u))goto L8c642000;goto L8c641f72; /* bt 0x8c642000 */
L8c641f72: *stop=0x8c641f72u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fd4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c641f74; /* mov.l 0x8c641fd4,r0 */
L8c641f74: *stop=0x8c641f74u;r[4]=16u;goto L8c641f76; /* mov #0x10,r4 */
L8c641f76: *stop=0x8c641f76u;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[4],4);if(io_status)return io_status;goto L8c641f78; /* mov.l r4,@r0 */
L8c641f78: *stop=0x8c641f78u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fd8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f7a; /* mov.l 0x8c641fd8,r3 */
L8c641f7a: *stop=0x8c641f7au;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[4],4);if(io_status)return io_status;goto L8c641f7c; /* mov.l r4,@r3 */
L8c641f7c: *stop=0x8c641f7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fe4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641f7e; /* mov.l 0x8c641fe4,r2 */
L8c641f7e: *stop=0x8c641f7eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fe0u,4);if(io_status)return io_status;r[4]=loaded;}goto L8c641f80; /* mov.l 0x8c641fe0,r4 */
L8c641f80: *stop=0x8c641f80u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fdcu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c641f82; /* mov.l 0x8c641fdc,r5 */
L8c641f82: target=r[2];*pr=0x8c641f86u;*stop=0x8c641f84u;r[6]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641f86; /* jsr @r2 */
L8c641f86: *stop=0x8c641f86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fe0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c641f88; /* mov.l 0x8c641fe0,r3 */
L8c641f88: *stop=0x8c641f88u;if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[3],1u);if(status)return status;goto L8c641f8a; /* pref @r3 */
L8c641f8a: *stop=0x8c641f8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c641fe8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c641f8c; /* mov.l 0x8c641fe8,r2 */
L8c641f8c: target=0x8c641e00u;*pr=0x8c641f90u;*stop=0x8c641f8eu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c641f90; /* bsr 0x8c641e00 */
L8c641f90: *stop=0x8c641f92u;goto L8c64204a; /* bra 0x8c64204a */
L8c642000: *stop=0x8c642000u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420c4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642002; /* mov.l 0x8c6420c4,r3 */
L8c642002: *stop=0x8c642002u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420c8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642004; /* mov.l 0x8c6420c8,r2 */
L8c642004: *stop=0x8c642004u;r[1]=r[3];goto L8c642006; /* mov r3,r1 */
L8c642006: *stop=0x8c642006u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[10]=loaded;}goto L8c642008; /* mov.l @r3,r10 */
L8c642008: *stop=0x8c642008u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[9]=loaded;}goto L8c64200a; /* mov.l @r1,r9 */
L8c64200a: *stop=0x8c64200au;r[3]+=116u;goto L8c64200c; /* add #0x74,r3 */
L8c64200c: *stop=0x8c64200cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420ccu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64200e; /* mov.l 0x8c6420cc,r0 */
L8c64200e: *stop=0x8c64200eu;r[10]<<=2;goto L8c642010; /* shll2 r10 */
L8c642010: *stop=0x8c642010u;r[9]<<=2;goto L8c642012; /* shll2 r9 */
L8c642012: *stop=0x8c642012u;r[10]+=r[2];goto L8c642014; /* add r2,r10 */
L8c642014: *stop=0x8c642014u;r[9]+=r[3];goto L8c642016; /* add r3,r9 */
L8c642016: *stop=0x8c642016u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642018; /* mov.l @r0,r2 */
L8c642018: *stop=0x8c642018u;r[2]<<=2;goto L8c64201a; /* shll2 r2 */
L8c64201a: *stop=0x8c64201au;r[2]<<=2;goto L8c64201c; /* shll2 r2 */
L8c64201c: *stop=0x8c64201cu;r[9]+=r[2];goto L8c64201e; /* add r2,r9 */
L8c64201e: *stop=0x8c64201eu;r[2]=r[0];goto L8c642020; /* mov r0,r2 */
L8c642020: *stop=0x8c642020u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642022; /* mov.l @r2,r3 */
L8c642022: *stop=0x8c642022u;r[3]<<=2;goto L8c642024; /* shll2 r3 */
L8c642024: *stop=0x8c642024u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420d0u,4);if(io_status)return io_status;r[8]=loaded;}goto L8c642026; /* mov.l 0x8c6420d0,r8 */
L8c642026: *stop=0x8c642026u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420d4u,4);if(io_status)return io_status;r[11]=loaded;}goto L8c642028; /* mov.l 0x8c6420d4,r11 */
L8c642028: *stop=0x8c642028u;r[3]<<=2;goto L8c64202a; /* shll2 r3 */
L8c64202a: *stop=0x8c64202au;r[12]=sx(m,bus,&io_status,0x8c6420c0u,2);if(io_status)return io_status;goto L8c64202c; /* mov.w 0x8c6420c0,r12 */
L8c64202c: *stop=0x8c64202cu;r[10]+=r[3];goto L8c64202e; /* add r3,r10 */
L8c64202e: *stop=0x8c64202eu;r[2]=r[15];goto L8c642030; /* mov r15,r2 */
L8c642030: *stop=0x8c642030u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c642032; /* mov.l r2,@-r15 */
L8c642032: *stop=0x8c642032u;if(!valid_access(r[9],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[9],4);if(io_status)return io_status;r[6]=loaded;}goto L8c642034; /* mov.l @r9,r6 */
L8c642034: *stop=0x8c642034u;r[7]=16u;goto L8c642036; /* mov #0x10,r7 */
L8c642036: *stop=0x8c642036u;if(!valid_access(r[10],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[10],4);if(io_status)return io_status;r[5]=loaded;}goto L8c642038; /* mov.l @r10,r5 */
L8c642038: *stop=0x8c642038u;r[6]+=32u;goto L8c64203a; /* add #0x20,r6 */
L8c64203a: target=r[8];*pr=0x8c64203eu;*stop=0x8c64203cu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64203e; /* jsr @r8 */
L8c64203e: *stop=0x8c64203eu;*sr=(*sr&~1u)|(r[12]==r[0]);goto L8c642040; /* cmp/eq r12,r0 */
L8c642040:
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c642040u;if((*sr&1u) && ++wait_visits[0]>=2u)return -4;
#endif
 take=(*sr&1u);*stop=0x8c642042u;r[15]+=4u;if(take)goto L8c64202e;goto L8c642044; /* bt/s 0x8c64202e */
L8c642044: *stop=0x8c642044u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420ccu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642046; /* mov.l 0x8c6420cc,r2 */
L8c642046: target=0x8c641e00u;*pr=0x8c64204au;*stop=0x8c642048u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64204a; /* bsr 0x8c641e00 */
L8c64204a: *stop=0x8c64204au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420d8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64204c; /* mov.l 0x8c6420d8,r1 */
L8c64204c: *stop=0x8c64204cu;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[13],4);if(io_status)return io_status;goto L8c64204e; /* mov.l r13,@r1 */
L8c64204e: *stop=0x8c64204eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420dcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642050; /* mov.l 0x8c6420dc,r2 */
L8c642050: *stop=0x8c642050u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642052; /* mov.l @r2,r3 */
L8c642052: *stop=0x8c642052u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c642054; /* tst r3,r3 */
L8c642054: take=(*sr&1u);*stop=0x8c642056u;r[12]=r[13];if(take)goto L8c642080;goto L8c642058; /* bt/s 0x8c642080 */
L8c642058: *stop=0x8c642058u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64205a; /* mov.l 0x8c6420e0,r3 */
L8c64205a: *stop=0x8c64205au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64205c; /* mov.l @r3,r2 */
L8c64205c: target=r[2];*pr=0x8c642060u;*stop=0x8c64205eu;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642060; /* jsr @r2 */
L8c642060: *stop=0x8c642060u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420e4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c642062; /* mov.l 0x8c6420e4,r1 */
L8c642062: *stop=0x8c642062u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642064; /* mov.l @r1,r3 */
L8c642064: *stop=0x8c642064u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420e8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642066; /* mov.l 0x8c6420e8,r0 */
L8c642066: *stop=0x8c642066u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642068; /* mov.l @r0,r2 */
L8c642068: *stop=0x8c642068u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420d8u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64206a; /* mov.l 0x8c6420d8,r1 */
L8c64206a: *stop=0x8c64206au;r[2]+=r[3];goto L8c64206c; /* add r3,r2 */
L8c64206c: *stop=0x8c64206cu;r[2]+=1u;goto L8c64206e; /* add #0x1,r2 */
L8c64206e: *stop=0x8c64206eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642070; /* mov.l @r1,r3 */
L8c642070: *stop=0x8c642070u;*sr=(*sr&~1u)|(r[3]>r[2]);goto L8c642072; /* cmp/hi r2,r3 */
L8c642072: if(!(*sr&1u))goto L8c642078;goto L8c642074; /* bf 0x8c642078 */
L8c642074: *stop=0x8c642076u;r[12]=r[14];goto L8c642080; /* bra 0x8c642080 */
L8c642078: *stop=0x8c642078u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420dcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64207a; /* mov.l 0x8c6420dc,r3 */
L8c64207a: *stop=0x8c64207au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64207c; /* mov.l @r3,r2 */
L8c64207c: *stop=0x8c64207cu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c64207e; /* tst r2,r2 */
L8c64207e:
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c64207eu;if(!(*sr&1u) && ++wait_visits[1]>=64u)return -4;
#endif
 if(!(*sr&1u))goto L8c642058;goto L8c642080; /* bf 0x8c642058 */
L8c642080: *stop=0x8c642080u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c642082; /* tst r12,r12 */
L8c642082: if((*sr&1u))goto L8c642112;goto L8c642084; /* bt 0x8c642112 */
L8c642084: *stop=0x8c642084u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420ecu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642086; /* mov.l 0x8c6420ec,r3 */
L8c642086: *stop=0x8c642086u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[13],4);if(io_status)return io_status;goto L8c642088; /* mov.l r13,@r3 */
L8c642088: *stop=0x8c642088u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420d8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64208a; /* mov.l 0x8c6420d8,r2 */
L8c64208a: *stop=0x8c64208au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[13],4);if(io_status)return io_status;goto L8c64208c; /* mov.l r13,@r2 */
L8c64208c: *stop=0x8c64208cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420dcu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64208e; /* mov.l 0x8c6420dc,r0 */
L8c64208e: *stop=0x8c64208eu;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c642090; /* mov.l @r0,r1 */
L8c642090: *stop=0x8c642090u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c642092; /* tst r1,r1 */
L8c642092: take=(*sr&1u);*stop=0x8c642094u;r[12]=r[13];if(take)goto L8c642108;goto L8c642096; /* bt/s 0x8c642108 */
L8c642096: *stop=0x8c642096u;r[11]=20u;goto L8c642098; /* mov #0x14,r11 */
L8c642098: *stop=0x8c642098u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420e0u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64209a; /* mov.l 0x8c6420e0,r3 */
L8c64209a: *stop=0x8c64209au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64209c; /* mov.l @r3,r2 */
L8c64209c: target=r[2];*pr=0x8c6420a0u;*stop=0x8c64209eu;if(!valid_access(r[3]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6420a0; /* jsr @r2 */
L8c6420a0: *stop=0x8c6420a0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6420d8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6420a2; /* mov.l 0x8c6420d8,r2 */
L8c6420a2: *stop=0x8c6420a2u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6420a4; /* mov.l @r2,r1 */
L8c6420a4: *stop=0x8c6420a4u;*sr=(*sr&~1u)|(r[1]>r[11]);goto L8c6420a6; /* cmp/hi r11,r1 */
L8c6420a6: if(!(*sr&1u))goto L8c642100;goto L8c6420a8; /* bf 0x8c642100 */
L8c6420a8: *stop=0x8c6420aau;r[12]=r[14];goto L8c642108; /* bra 0x8c642108 */
L8c642100: *stop=0x8c642100u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421ecu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c642102; /* mov.l 0x8c6421ec,r2 */
L8c642102: *stop=0x8c642102u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c642104; /* mov.l @r2,r3 */
L8c642104: *stop=0x8c642104u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c642106; /* tst r3,r3 */
L8c642106:
#ifdef SA_BOUND_RETAIL_STALLS
 *stop=0x8c642106u;if(!(*sr&1u) && ++wait_visits[2]>=64u)return -4;
#endif
 if(!(*sr&1u))goto L8c642098;goto L8c642108; /* bf 0x8c642098 */
L8c642108: *stop=0x8c642108u;*sr=(*sr&~1u)|!(r[12]&r[12]);goto L8c64210a; /* tst r12,r12 */
L8c64210a: if((*sr&1u))goto L8c642112;goto L8c64210c; /* bt 0x8c642112 */
L8c64210c: *stop=0x8c64210cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421f0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64210e; /* mov.l 0x8c6421f0,r2 */
L8c64210e: target=r[2];*pr=0x8c642112u;*stop=0x8c642110u;r[4]=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642112; /* jsr @r2 */
L8c642112: *stop=0x8c642112u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642114; /* mov.l 0x8c6421f4,r3 */
L8c642114: *stop=0x8c642114u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642116; /* mov.l @r3,r2 */
L8c642116: *stop=0x8c642116u;r[2]+=1u;goto L8c642118; /* add #0x1,r2 */
L8c642118: *stop=0x8c642118u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c64211a; /* mov.l r2,@r3 */
L8c64211a: *stop=0x8c64211au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64211c; /* mov.l @r3,r0 */
L8c64211c: *stop=0x8c64211cu;r[0]&=0x3u;goto L8c64211e; /* and #0x3,r0 */
L8c64211e: *stop=0x8c64211eu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[0],4);if(io_status)return io_status;goto L8c642120; /* mov.l r0,@r3 */
L8c642120: *stop=0x8c642120u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c642122; /* mov.l @r3,r2 */
L8c642122: *stop=0x8c642122u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421f8u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642124; /* mov.l 0x8c6421f8,r0 */
L8c642124: *stop=0x8c642124u;r[2]<<=2;goto L8c642126; /* shll2 r2 */
L8c642126: *stop=0x8c642126u;if(!valid_access(r[0]+r[2],4u))return -3;wr(m,bus,&io_status,r[2]+r[0],r[13],4);if(io_status)return io_status;goto L8c642128; /* mov.l r13,@(r0,r2) */
L8c642128: *stop=0x8c642128u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421fcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64212a; /* mov.l 0x8c6421fc,r2 */
L8c64212a: *stop=0x8c64212au;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[0]=loaded;}goto L8c64212c; /* mov.l @r2,r0 */
L8c64212c: *stop=0x8c64212cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c64212e; /* cmp/eq #0x1,r0 */
L8c64212e: if((*sr&1u))goto L8c642138;goto L8c642130; /* bt 0x8c642138 */
L8c642130: *stop=0x8c642130u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642200u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c642132; /* mov.l 0x8c642200,r1 */
L8c642132: *stop=0x8c642132u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[0]=loaded;}goto L8c642134; /* mov.l @r1,r0 */
L8c642134: *stop=0x8c642134u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c642136; /* cmp/eq #0x1,r0 */
L8c642136: if(!(*sr&1u))goto L8c6421c0;goto L8c642138; /* bf 0x8c6421c0 */
L8c642138: *stop=0x8c642138u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421f4u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64213a; /* mov.l 0x8c6421f4,r3 */
L8c64213a: *stop=0x8c64213au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[2]=loaded;}goto L8c64213c; /* mov.l @r3,r2 */
L8c64213c: *stop=0x8c64213cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642204u,4);if(io_status)return io_status;r[13]=loaded;}goto L8c64213e; /* mov.l 0x8c642204,r13 */
L8c64213e: *stop=0x8c64213eu;r[2]&=r[14];goto L8c642140; /* and r14,r2 */
L8c642140: *stop=0x8c642140u;r[5]=1u;goto L8c642142; /* mov #0x1,r5 */
L8c642142: *stop=0x8c642142u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c642144; /* mov.l r2,@(0x4,r15) */
L8c642144: target=r[13];*pr=0x8c642148u;*stop=0x8c642146u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642148; /* jsr @r13 */
L8c642148: *stop=0x8c642148u;r[5]=0u;goto L8c64214a; /* mov #0x0,r5 */
L8c64214a: target=r[13];*pr=0x8c64214eu;*stop=0x8c64214cu;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64214e; /* jsr @r13 */
L8c64214e: *stop=0x8c64214eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[12]=loaded;}goto L8c642150; /* mov.l @(0x4,r15),r12 */
L8c642150: *stop=0x8c642150u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642208u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642152; /* mov.l 0x8c642208,r0 */
L8c642152: *stop=0x8c642152u;r[12]<<=2;goto L8c642154; /* shll2 r12 */
L8c642154: *stop=0x8c642154u;r[4]=sx(m,bus,&io_status,0x8c6421e0u,2);if(io_status)return io_status;goto L8c642156; /* mov.w 0x8c6421e0,r4 */
L8c642156: *stop=0x8c642156u;r[12]<<=2;goto L8c642158; /* shll2 r12 */
L8c642158: *stop=0x8c642158u;*sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c64215a; /* shll r12 */
L8c64215a: target=r[13];*pr=0x8c64215eu;*stop=0x8c64215cu;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64215e; /* jsr @r13 */
L8c64215e: *stop=0x8c64215eu;r[4]=sx(m,bus,&io_status,0x8c6421e2u,2);if(io_status)return io_status;goto L8c642160; /* mov.w 0x8c6421e2,r4 */
L8c642160: *stop=0x8c642160u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64220cu,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642162; /* mov.l 0x8c64220c,r0 */
L8c642162: target=r[13];*pr=0x8c642166u;*stop=0x8c642164u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642166; /* jsr @r13 */
L8c642166: *stop=0x8c642166u;r[4]=sx(m,bus,&io_status,0x8c6421e4u,2);if(io_status)return io_status;goto L8c642168; /* mov.w 0x8c6421e4,r4 */
L8c642168: *stop=0x8c642168u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642210u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64216a; /* mov.l 0x8c642210,r0 */
L8c64216a: target=r[13];*pr=0x8c64216eu;*stop=0x8c64216cu;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64216e; /* jsr @r13 */
L8c64216e: *stop=0x8c64216eu;r[4]=sx(m,bus,&io_status,0x8c6421e6u,2);if(io_status)return io_status;goto L8c642170; /* mov.w 0x8c6421e6,r4 */
L8c642170: *stop=0x8c642170u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642214u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642172; /* mov.l 0x8c642214,r0 */
L8c642172: target=r[13];*pr=0x8c642176u;*stop=0x8c642174u;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642176; /* jsr @r13 */
L8c642176: *stop=0x8c642176u;r[4]=sx(m,bus,&io_status,0x8c6421e8u,2);if(io_status)return io_status;goto L8c642178; /* mov.w 0x8c6421e8,r4 */
L8c642178: *stop=0x8c642178u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642218u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c64217a; /* mov.l 0x8c642218,r0 */
L8c64217a: target=r[13];*pr=0x8c64217eu;*stop=0x8c64217cu;if(!valid_access(r[0]+r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12]+r[0],4);if(io_status)return io_status;r[5]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64217e; /* jsr @r13 */
L8c64217e: *stop=0x8c64217eu;r[4]=sx(m,bus,&io_status,0x8c6421eau,2);if(io_status)return io_status;goto L8c642180; /* mov.w 0x8c6421ea,r4 */
L8c642180: *stop=0x8c642180u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64221cu,4);if(io_status)return io_status;r[5]=loaded;}goto L8c642182; /* mov.l 0x8c64221c,r5 */
L8c642182: target=r[13];*pr=0x8c642186u;*stop=0x8c642184u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c642186; /* jsr @r13 */
L8c642186: *stop=0x8c642186u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642220u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642188; /* mov.l 0x8c642220,r3 */
L8c642188: *stop=0x8c642188u;r[4]=sx(m,bus,&io_status,0x8c6421eau,2);if(io_status)return io_status;goto L8c64218a; /* mov.w 0x8c6421ea,r4 */
L8c64218a: target=r[3];*pr=0x8c64218eu;*stop=0x8c64218cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64218e; /* jsr @r3 */
L8c64218e: *stop=0x8c64218eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642224u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c642190; /* mov.l 0x8c642224,r3 */
L8c642190: *stop=0x8c642190u;r[2]=0u;goto L8c642192; /* mov #0x0,r2 */
L8c642192: *stop=0x8c642192u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c642194; /* mov.l r2,@r3 */
L8c642194: *stop=0x8c642194u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642228u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c642196; /* mov.l 0x8c642228,r0 */
L8c642196: *stop=0x8c642196u;if(!valid_access(r[0],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[1]=loaded;}goto L8c642198; /* mov.l @r0,r1 */
L8c642198: *stop=0x8c642198u;{uint32_t loaded=rd(m,bus,&io_status,0x8c64222cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c64219a; /* mov.l 0x8c64222c,r2 */
L8c64219a: *stop=0x8c64219au;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c64219c; /* mov.l r1,@r2 */
L8c64219c: *stop=0x8c64219cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c642200u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c64219e; /* mov.l 0x8c642200,r1 */
L8c64219e: *stop=0x8c64219eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6421a0; /* mov.l @r1,r3 */
L8c6421a0: *stop=0x8c6421a0u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6421a2; /* tst r3,r3 */
L8c6421a2: if((*sr&1u))goto L8c6421c0;goto L8c6421a4; /* bt 0x8c6421c0 */
L8c6421a4: *stop=0x8c6421a4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6421f4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6421a6; /* mov.l 0x8c6421f4,r2 */
L8c6421a6: *stop=0x8c6421a6u;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c6421a8; /* mov.l @r2,r3 */
L8c6421a8: *stop=0x8c6421a8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642230u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c6421aa; /* mov.l 0x8c642230,r0 */
L8c6421aa: *stop=0x8c6421aau;if(!valid_access(r[0],4u))return -3;wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c6421ac; /* mov.l r3,@r0 */
L8c6421ac: *stop=0x8c6421acu;r[3]=r[2];goto L8c6421ae; /* mov r2,r3 */
L8c6421ae: *stop=0x8c6421aeu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[1]=loaded;}goto L8c6421b0; /* mov.l @r3,r1 */
L8c6421b0: *stop=0x8c6421b0u;r[2]=2u;goto L8c6421b2; /* mov #0x2,r2 */
L8c6421b2: *stop=0x8c6421b2u;r[1]<<=2;goto L8c6421b4; /* shll2 r1 */
L8c6421b4: *stop=0x8c6421b4u;r[0]+=4294967272u;goto L8c6421b6; /* add #-0x18,r0 */
L8c6421b6: *stop=0x8c6421b6u;if(!valid_access(r[0]+r[1],4u))return -3;wr(m,bus,&io_status,r[1]+r[0],r[2],4);if(io_status)return io_status;goto L8c6421b8; /* mov.l r2,@(r0,r1) */
L8c6421b8: *stop=0x8c6421b8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c642234u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c6421ba; /* mov.l 0x8c642234,r1 */
L8c6421ba: *stop=0x8c6421bau;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[14],4);if(io_status)return io_status;goto L8c6421bc; /* mov.l r14,@r1 */
L8c6421bc: *stop=0x8c6421bcu;r[3]+=32u;goto L8c6421be; /* add #0x20,r3 */
L8c6421be: *stop=0x8c6421beu;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[14],4);if(io_status)return io_status;goto L8c6421c0; /* mov.l r14,@r3 */
L8c6421c0: *stop=0x8c6421c0u;r[0]=0u;goto L8c6421c2; /* mov #0x0,r0 */
L8c6421c2: *stop=0x8c6421c2u;r[15]+=8u;goto L8c6421c4; /* add #0x8,r15 */
L8c6421c4: *stop=0x8c6421c4u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c6421c6; /* lds.l @r15+,PR */
L8c6421c6: *stop=0x8c6421c6u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c6421c8; /* mov.l @r15+,r8 */
L8c6421c8: *stop=0x8c6421c8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c6421ca; /* mov.l @r15+,r9 */
L8c6421ca: *stop=0x8c6421cau;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c6421cc; /* mov.l @r15+,r10 */
L8c6421cc: *stop=0x8c6421ccu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c6421ce; /* mov.l @r15+,r11 */
L8c6421ce: *stop=0x8c6421ceu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c6421d0; /* mov.l @r15+,r12 */
L8c6421d0: *stop=0x8c6421d0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c6421d2; /* mov.l @r15+,r13 */
L8c6421d2: *stop=0x8c6421d2u;if(*pr!=entry_pr)return -3;*stop=0x8c6421d4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}
