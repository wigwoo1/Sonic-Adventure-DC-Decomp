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

SA_STAGE_EXPORT int sa_boot_input_8c601e2a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c601e2a;
L8c601e2a: *stop=0x8c601e2au;{uint32_t loaded=rd(m,bus,&io_status,0x8c601e58u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601e2c; /* mov.l 0x8c601e58,r2 */
L8c601e2c: *stop=0x8c601e2cu;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c601e2e; /* mov.l @r2,r3 */
L8c601e2e: *stop=0x8c601e2eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c601e30; /* tst r3,r3 */
L8c601e30: if(!(*sr&1u))goto L8c601e5c;goto L8c601e32; /* bf 0x8c601e5c */
L8c601e32: *stop=0x8c601e32u;if(*pr!=entry_pr)return -3;*stop=0x8c601e34u;return 0; /* rts */
L8c601e5c: *stop=0x8c601e5cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c601e5e; /* mov.l r14,@-r15 */
L8c601e5e: *stop=0x8c601e5eu;r[4]=0u;goto L8c601e60; /* mov #0x0,r4 */
L8c601e60: *stop=0x8c601e60u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c601e62; /* mov.l r13,@-r15 */
L8c601e62: *stop=0x8c601e62u;r[13]=r[4];goto L8c601e64; /* mov r4,r13 */
L8c601e64: *stop=0x8c601e64u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c601e66; /* mov.l r12,@-r15 */
L8c601e66: *stop=0x8c601e66u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c601e68; /* mov.l r11,@-r15 */
L8c601e68: *stop=0x8c601e68u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c601e6a; /* mov.l r10,@-r15 */
L8c601e6a: *stop=0x8c601e6au;r[10]=r[4];goto L8c601e6c; /* mov r4,r10 */
L8c601e6c: *stop=0x8c601e6cu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c601e6e; /* mov.l r9,@-r15 */
L8c601e6e: *stop=0x8c601e6eu;r[9]=24u;goto L8c601e70; /* mov #0x18,r9 */
L8c601e70: *stop=0x8c601e70u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c601e72; /* mov.l r8,@-r15 */
L8c601e72: *stop=0x8c601e72u;r[8]=6u;goto L8c601e74; /* mov #0x6,r8 */
L8c601e74: *stop=0x8c601e74u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c601e76; /* sts.l PR,@-r15 */
L8c601e76: *stop=0x8c601e76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601f68u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601e78; /* mov.l 0x8c601f68,r3 */
L8c601e78: target=r[3];*pr=0x8c601e7cu;*stop=0x8c601e7au;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601e7c; /* jsr @r3 */
L8c601e7c: *stop=0x8c601e7cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601f6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601e7e; /* mov.l 0x8c601f6c,r2 */
L8c601e7e: *stop=0x8c601e7eu;r[11]=r[0];goto L8c601e80; /* mov r0,r11 */
L8c601e80: target=r[2];*pr=0x8c601e84u;*stop=0x8c601e82u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601e84; /* jsr @r2 */
L8c601e84: *stop=0x8c601e84u;r[14]=52u;goto L8c601e86; /* mov #0x34,r14 */
L8c601e86: *stop=0x8c601e86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601f70u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601e88; /* mov.l 0x8c601f70,r3 */
L8c601e88: *stop=0x8c601e88u;*macl=r[14]*r[13];goto L8c601e8a; /* mul.l r14,r13 */
L8c601e8a: *stop=0x8c601e8au;r[1]=r[13];goto L8c601e8c; /* mov r13,r1 */
L8c601e8c: *stop=0x8c601e8cu;r[1]<<=2;goto L8c601e8e; /* shll2 r1 */
L8c601e8e: *stop=0x8c601e8eu;r[12]=r[0];goto L8c601e90; /* mov r0,r12 */
L8c601e90: *stop=0x8c601e90u;r[6]=r[11];goto L8c601e92; /* mov r11,r6 */
L8c601e92: *stop=0x8c601e92u;r[5]=r[12];goto L8c601e94; /* mov r12,r5 */
L8c601e94: *stop=0x8c601e94u;r[14]=*macl;goto L8c601e96; /* sts MACL,r14 */
L8c601e96: *stop=0x8c601e96u;r[14]+=r[3];goto L8c601e98; /* add r3,r14 */
L8c601e98: *stop=0x8c601e98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601f74u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601e9a; /* mov.l 0x8c601f74,r3 */
L8c601e9a: *stop=0x8c601e9au;if(!valid_access(r[14]+48u,4u))return -3;wr(m,bus,&io_status,r[14]+0x30u,r[0],4);if(io_status)return io_status;goto L8c601e9c; /* mov.l r0,@(0x30,r14) */
L8c601e9c: *stop=0x8c601e9cu;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601e9e; /* mov.l @(0x8,r14),r2 */
L8c601e9e: *stop=0x8c601e9eu;r[3]+=r[1];goto L8c601ea0; /* add r1,r3 */
L8c601ea0: *stop=0x8c601ea0u;if(!valid_access(r[14]+44u,4u))return -3;wr(m,bus,&io_status,r[14]+0x2cu,r[2],4);if(io_status)return io_status;goto L8c601ea2; /* mov.l r2,@(0x2c,r14) */
L8c601ea2: *stop=0x8c601ea2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c601ea4; /* mov.l r3,@-r15 */
L8c601ea4: target=0x8c601df6u;*pr=0x8c601ea8u;*stop=0x8c601ea6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601ea8; /* bsr 0x8c601df6 */
L8c601ea8: *stop=0x8c601ea8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;goto L8c601eaa; /* mov.l @r15+,r2 */
L8c601eaa: *stop=0x8c601eaau;r[1]=r[13];goto L8c601eac; /* mov r13,r1 */
L8c601eac: *stop=0x8c601eacu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601f78u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601eae; /* mov.l 0x8c601f78,r3 */
L8c601eae: *stop=0x8c601eaeu;if(!valid_access(r[2],4u))return -3;wr(m,bus,&io_status,r[2],r[0],4);if(io_status)return io_status;goto L8c601eb0; /* mov.l r0,@r2 */
L8c601eb0: target=r[3];*pr=0x8c601eb4u;*stop=0x8c601eb2u;r[0]=r[8];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601eb4; /* jsr @r3 */
L8c601eb4: *stop=0x8c601eb4u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c601eb6; /* tst r0,r0 */
L8c601eb6: if(!(*sr&1u))goto L8c601f3c;goto L8c601eb8; /* bf 0x8c601f3c */
L8c601eb8: *stop=0x8c601eb8u;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[0]=loaded;}goto L8c601eba; /* mov.l @r12,r0 */
L8c601eba: *stop=0x8c601ebau;*sr=(*sr&~1u)|!(r[0]&0x21u);goto L8c601ebc; /* tst #0x21,r0 */
L8c601ebc: if((*sr&1u))goto L8c601ede;goto L8c601ebe; /* bt 0x8c601ede */
L8c601ebe: *stop=0x8c601ebeu;if(!valid_access(r[11]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[11]+0x2u,1);if(io_status)return io_status;goto L8c601ec0; /* mov.b @(0x2,r11),r0 */
L8c601ec0: *stop=0x8c601ec0u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c601ec2; /* tst r0,r0 */
L8c601ec2: if(!(*sr&1u))goto L8c601ede;goto L8c601ec4; /* bf 0x8c601ede */
L8c601ec4: *stop=0x8c601ec4u;if(!valid_access(r[11]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[11]+0x4u,1);if(io_status)return io_status;goto L8c601ec6; /* mov.b @(0x4,r11),r0 */
L8c601ec6: *stop=0x8c601ec6u;r[0]=r[0]&255u;goto L8c601ec8; /* extu.b r0,r0 */
L8c601ec8: *stop=0x8c601ec8u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c601eca; /* cmp/eq #0x8,r0 */
L8c601eca: if(!(*sr&1u))goto L8c601ede;goto L8c601ecc; /* bf 0x8c601ede */
L8c601ecc: *stop=0x8c601eccu;r[5]=r[12];goto L8c601ece; /* mov r12,r5 */
L8c601ece: target=0x8c601da6u;*pr=0x8c601ed2u;*stop=0x8c601ed0u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601ed2; /* bsr 0x8c601da6 */
L8c601ed2: *stop=0x8c601ed2u;r[5]=r[11];goto L8c601ed4; /* mov r11,r5 */
L8c601ed4: *stop=0x8c601ed4u;r[6]=r[13];goto L8c601ed6; /* mov r13,r6 */
L8c601ed6: target=0x8c601bf0u;*pr=0x8c601edau;*stop=0x8c601ed8u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601eda; /* bsr 0x8c601bf0 */
L8c601eda: *stop=0x8c601edcu;goto L8c601f06; /* bra 0x8c601f06 */
L8c601ede: *stop=0x8c601edeu;if(!valid_access(r[12],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[12],4);if(io_status)return io_status;r[0]=loaded;}goto L8c601ee0; /* mov.l @r12,r0 */
L8c601ee0: *stop=0x8c601ee0u;*sr=(*sr&~1u)|!(r[0]&0x40u);goto L8c601ee2; /* tst #0x40,r0 */
L8c601ee2: if((*sr&1u))goto L8c601f02;goto L8c601ee4; /* bt 0x8c601f02 */
L8c601ee4: *stop=0x8c601ee4u;if(!valid_access(r[11]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[11]+0x2u,1);if(io_status)return io_status;goto L8c601ee6; /* mov.b @(0x2,r11),r0 */
L8c601ee6: *stop=0x8c601ee6u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c601ee8; /* tst r0,r0 */
L8c601ee8: if(!(*sr&1u))goto L8c601f02;goto L8c601eea; /* bf 0x8c601f02 */
L8c601eea: *stop=0x8c601eeau;if(!valid_access(r[11]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[11]+0x4u,1);if(io_status)return io_status;goto L8c601eec; /* mov.b @(0x4,r11),r0 */
L8c601eec: *stop=0x8c601eecu;r[0]=r[0]&255u;goto L8c601eee; /* extu.b r0,r0 */
L8c601eee: *stop=0x8c601eeeu;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c601ef0; /* cmp/eq #0x8,r0 */
L8c601ef0: if(!(*sr&1u))goto L8c601f02;goto L8c601ef2; /* bf 0x8c601f02 */
L8c601ef2: *stop=0x8c601ef2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601f7cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601ef4; /* mov.l 0x8c601f7c,r2 */
L8c601ef4: *stop=0x8c601ef4u;r[5]=r[14];goto L8c601ef6; /* mov r14,r5 */
L8c601ef6: *stop=0x8c601ef6u;r[7]=r[12];goto L8c601ef8; /* mov r12,r7 */
L8c601ef8: *stop=0x8c601ef8u;r[6]=r[11];goto L8c601efa; /* mov r11,r6 */
L8c601efa: target=r[2];*pr=0x8c601efeu;*stop=0x8c601efcu;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601efe; /* jsr @r2 */
L8c601efe: *stop=0x8c601f00u;goto L8c601f06; /* bra 0x8c601f06 */
L8c601f02: target=0x8c601bb2u;*pr=0x8c601f06u;*stop=0x8c601f04u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601f06; /* bsr 0x8c601bb2 */
L8c601f06: *stop=0x8c601f06u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601f80u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c601f08; /* mov.l 0x8c601f80,r1 */
L8c601f08: *stop=0x8c601f08u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601f0a; /* mov.l @(0x8,r14),r3 */
L8c601f0a: *stop=0x8c601f0au;r[3]=~r[3];goto L8c601f0c; /* not r3,r3 */
L8c601f0c: *stop=0x8c601f0cu;if(!valid_access(r[14]+12u,4u))return -3;wr(m,bus,&io_status,r[14]+0xcu,r[3],4);if(io_status)return io_status;goto L8c601f0e; /* mov.l r3,@(0xc,r14) */
L8c601f0e: *stop=0x8c601f0eu;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[2]=loaded;}goto L8c601f10; /* mov.l @r1,r2 */
L8c601f10: *stop=0x8c601f10u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c601f12; /* tst r2,r2 */
L8c601f12: if(!(*sr&1u))goto L8c601f24;goto L8c601f14; /* bf 0x8c601f24 */
L8c601f14: *stop=0x8c601f14u;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c601f16; /* mov.l @(0x2c,r14),r4 */
L8c601f16: *stop=0x8c601f16u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c601f18; /* mov.l @(0x8,r14),r5 */
L8c601f18: *stop=0x8c601f18u;r[4]^=r[5];goto L8c601f1a; /* xor r5,r4 */
L8c601f1a: *stop=0x8c601f1au;r[5]&=r[4];goto L8c601f1c; /* and r4,r5 */
L8c601f1c: *stop=0x8c601f1cu;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[5],4);if(io_status)return io_status;goto L8c601f1e; /* mov.l r5,@(0x10,r14) */
L8c601f1e: *stop=0x8c601f1eu;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601f20; /* mov.l @(0xc,r14),r3 */
L8c601f20: *stop=0x8c601f22u;r[4]&=r[3];goto L8c601f36; /* bra 0x8c601f36 */
L8c601f24: *stop=0x8c601f24u;if(!valid_access(r[14]+44u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x2cu,4);if(io_status)return io_status;r[4]=loaded;}goto L8c601f26; /* mov.l @(0x2c,r14),r4 */
L8c601f26: *stop=0x8c601f26u;r[3]=4294967295u;goto L8c601f28; /* mov #-0x1,r3 */
L8c601f28: *stop=0x8c601f28u;if(!valid_access(r[14]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c601f2a; /* mov.l @(0x8,r14),r5 */
L8c601f2a: *stop=0x8c601f2au;r[4]^=r[5];goto L8c601f2c; /* xor r5,r4 */
L8c601f2c: *stop=0x8c601f2cu;r[4]^=r[3];goto L8c601f2e; /* xor r3,r4 */
L8c601f2e: *stop=0x8c601f2eu;r[5]|=r[4];goto L8c601f30; /* or r4,r5 */
L8c601f30: *stop=0x8c601f30u;if(!valid_access(r[14]+16u,4u))return -3;wr(m,bus,&io_status,r[14]+0x10u,r[5],4);if(io_status)return io_status;goto L8c601f32; /* mov.l r5,@(0x10,r14) */
L8c601f32: *stop=0x8c601f32u;if(!valid_access(r[14]+12u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601f34; /* mov.l @(0xc,r14),r2 */
L8c601f34: *stop=0x8c601f34u;r[4]|=r[2];goto L8c601f36; /* or r2,r4 */
L8c601f36: *stop=0x8c601f36u;if(!valid_access(r[14]+20u,4u))return -3;wr(m,bus,&io_status,r[14]+0x14u,r[4],4);if(io_status)return io_status;goto L8c601f38; /* mov.l r4,@(0x14,r14) */
L8c601f38: *stop=0x8c601f3au;r[10]+=1u;goto L8c601f42; /* bra 0x8c601f42 */
L8c601f3c: *stop=0x8c601f3cu;r[2]=r[12];goto L8c601f3e; /* mov r12,r2 */
L8c601f3e: *stop=0x8c601f3eu;r[2]+=20u;goto L8c601f40; /* add #0x14,r2 */
L8c601f40: *stop=0x8c601f40u;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[2],4);if(io_status)return io_status;goto L8c601f42; /* mov.l r2,@(0x24,r14) */
L8c601f42: *stop=0x8c601f42u;r[13]+=1u;goto L8c601f44; /* add #0x1,r13 */
L8c601f44: *stop=0x8c601f44u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[9]);goto L8c601f46; /* cmp/ge r9,r13 */
L8c601f46: if(!(*sr&1u))goto L8c601e76;goto L8c601f48; /* bf 0x8c601e76 */
L8c601f48: *stop=0x8c601f48u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c601f4a; /* lds.l @r15+,PR */
L8c601f4a: *stop=0x8c601f4au;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[8]=loaded;}r[15]+=4;goto L8c601f4c; /* mov.l @r15+,r8 */
L8c601f4c: *stop=0x8c601f4cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[9]=loaded;}r[15]+=4;goto L8c601f4e; /* mov.l @r15+,r9 */
L8c601f4e: *stop=0x8c601f4eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[10]=loaded;}r[15]+=4;goto L8c601f50; /* mov.l @r15+,r10 */
L8c601f50: *stop=0x8c601f50u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[11]=loaded;}r[15]+=4;goto L8c601f52; /* mov.l @r15+,r11 */
L8c601f52: *stop=0x8c601f52u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c601f54; /* mov.l @r15+,r12 */
L8c601f54: *stop=0x8c601f54u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c601f56; /* mov.l @r15+,r13 */
L8c601f56: *stop=0x8c601f56u;if(*pr!=entry_pr)return -3;*stop=0x8c601f58u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c601df6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c601df6;
L8c601df6: *stop=0x8c601df6u;if(!valid_access(r[6]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[6]+0x4u,1);if(io_status)return io_status;goto L8c601df8; /* mov.b @(0x4,r6),r0 */
L8c601df8: *stop=0x8c601df8u;r[1]=sx(m,bus,&io_status,0x8c601e38u,2);if(io_status)return io_status;goto L8c601dfa; /* mov.w 0x8c601e38,r1 */
L8c601dfa: *stop=0x8c601dfau;r[0]=r[0]&255u;goto L8c601dfc; /* extu.b r0,r0 */
L8c601dfc: *stop=0x8c601dfcu;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c601dfe; /* cmp/eq r1,r0 */
L8c601dfe: if((*sr&1u))goto L8c601e1a;goto L8c601e00; /* bt 0x8c601e1a */
L8c601e00: *stop=0x8c601e00u;r[1]=sx(m,bus,&io_status,0x8c601e3au,2);if(io_status)return io_status;goto L8c601e02; /* mov.w 0x8c601e3a,r1 */
L8c601e02: *stop=0x8c601e02u;*sr=(*sr&~1u)|(r[1]==r[0]);goto L8c601e04; /* cmp/eq r1,r0 */
L8c601e04: if((*sr&1u))goto L8c601e20;goto L8c601e06; /* bt 0x8c601e20 */
L8c601e06: *stop=0x8c601e06u;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c601e08; /* cmp/eq #0x8,r0 */
L8c601e08: if((*sr&1u))goto L8c601e20;goto L8c601e0a; /* bt 0x8c601e20 */
L8c601e0a: *stop=0x8c601e0au;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c601e0c; /* cmp/eq #0x5,r0 */
L8c601e0c: if((*sr&1u))goto L8c601e20;goto L8c601e0e; /* bt 0x8c601e20 */
L8c601e0e: *stop=0x8c601e0eu;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c601e10; /* cmp/eq #0x6,r0 */
L8c601e10: if((*sr&1u))goto L8c601e20;goto L8c601e12; /* bt 0x8c601e20 */
L8c601e12: *stop=0x8c601e12u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c601e14; /* cmp/eq #0x0,r0 */
L8c601e14: if((*sr&1u))goto L8c601e20;goto L8c601e16; /* bt 0x8c601e20 */
L8c601e16: *stop=0x8c601e18u;goto L8c601e24; /* bra 0x8c601e24 */
L8c601e1a: *stop=0x8c601e1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c601e50u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c601e1c; /* mov.l 0x8c601e50,r0 */
L8c601e1c: *stop=0x8c601e1cu;if(*pr!=entry_pr)return -3;*stop=0x8c601e1eu;return 0; /* rts */
L8c601e20: *stop=0x8c601e20u;if(*pr!=entry_pr)return -3;*stop=0x8c601e22u;r[0]=0u;return 0; /* rts */
L8c601e24: *stop=0x8c601e24u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601e54u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c601e26; /* mov.l 0x8c601e54,r0 */
L8c601e26: *stop=0x8c601e26u;if(*pr!=entry_pr)return -3;*stop=0x8c601e28u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c601bb2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c601bb2;
L8c601bb2: *stop=0x8c601bb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601c2cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601bb4; /* mov.l 0x8c601c2c,r2 */
L8c601bb4: *stop=0x8c601bb4u;r[5]=0u;goto L8c601bb6; /* mov #0x0,r5 */
L8c601bb6: *stop=0x8c601bb6u;if(!valid_access(r[4],4u))return -3;wr(m,bus,&io_status,r[4],r[5],4);if(io_status)return io_status;goto L8c601bb8; /* mov.l r5,@r4 */
L8c601bb8: *stop=0x8c601bb8u;if(!valid_access(r[4]+4u,4u))return -3;wr(m,bus,&io_status,r[4]+0x4u,r[5],4);if(io_status)return io_status;goto L8c601bba; /* mov.l r5,@(0x4,r4) */
L8c601bba: *stop=0x8c601bbau;if(!valid_access(r[2],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;r[3]=loaded;}goto L8c601bbc; /* mov.l @r2,r3 */
L8c601bbc: *stop=0x8c601bbcu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c601bbe; /* tst r3,r3 */
L8c601bbe: take=!(*sr&1u);*stop=0x8c601bc0u;r[6]=4294967295u;if(take)goto L8c601bcc;goto L8c601bc2; /* bf/s 0x8c601bcc */
L8c601bc2: *stop=0x8c601bc2u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[5],4);if(io_status)return io_status;goto L8c601bc4; /* mov.l r5,@(0x8,r4) */
L8c601bc4: *stop=0x8c601bc4u;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[6],4);if(io_status)return io_status;goto L8c601bc6; /* mov.l r6,@(0xc,r4) */
L8c601bc6: *stop=0x8c601bc6u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[5],4);if(io_status)return io_status;goto L8c601bc8; /* mov.l r5,@(0x10,r4) */
L8c601bc8: *stop=0x8c601bcau;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[5],4);if(io_status)return io_status;goto L8c601bd4; /* bra 0x8c601bd4 */
L8c601bcc: *stop=0x8c601bccu;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[6],4);if(io_status)return io_status;goto L8c601bce; /* mov.l r6,@(0x8,r4) */
L8c601bce: *stop=0x8c601bceu;if(!valid_access(r[4]+12u,4u))return -3;wr(m,bus,&io_status,r[4]+0xcu,r[5],4);if(io_status)return io_status;goto L8c601bd0; /* mov.l r5,@(0xc,r4) */
L8c601bd0: *stop=0x8c601bd0u;if(!valid_access(r[4]+16u,4u))return -3;wr(m,bus,&io_status,r[4]+0x10u,r[6],4);if(io_status)return io_status;goto L8c601bd2; /* mov.l r6,@(0x10,r4) */
L8c601bd2: *stop=0x8c601bd2u;if(!valid_access(r[4]+20u,4u))return -3;wr(m,bus,&io_status,r[4]+0x14u,r[6],4);if(io_status)return io_status;goto L8c601bd4; /* mov.l r6,@(0x14,r4) */
L8c601bd4: *stop=0x8c601bd4u;r[0]=r[5];goto L8c601bd6; /* mov r5,r0 */
L8c601bd6: *stop=0x8c601bd6u;if(!valid_access(r[4]+24u,2u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[0],2);if(io_status)return io_status;goto L8c601bd8; /* mov.w r0,@(0x18,r4) */
L8c601bd8: *stop=0x8c601bd8u;if(!valid_access(r[4]+26u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1au,r[0],2);if(io_status)return io_status;goto L8c601bda; /* mov.w r0,@(0x1a,r4) */
L8c601bda: *stop=0x8c601bdau;r[0]=34u;goto L8c601bdc; /* mov #0x22,r0 */
L8c601bdc: *stop=0x8c601bdcu;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[5],2);if(io_status)return io_status;goto L8c601bde; /* mov.w r5,@(r0,r4) */
L8c601bde: *stop=0x8c601bdeu;r[0]=32u;goto L8c601be0; /* mov #0x20,r0 */
L8c601be0: *stop=0x8c601be0u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[5],2);if(io_status)return io_status;goto L8c601be2; /* mov.w r5,@(r0,r4) */
L8c601be2: *stop=0x8c601be2u;r[0]=r[5];goto L8c601be4; /* mov r5,r0 */
L8c601be4: *stop=0x8c601be4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601c60u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601be6; /* mov.l 0x8c601c60,r3 */
L8c601be6: *stop=0x8c601be6u;if(!valid_access(r[4]+30u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1eu,r[0],2);if(io_status)return io_status;goto L8c601be8; /* mov.w r0,@(0x1e,r4) */
L8c601be8: *stop=0x8c601be8u;if(!valid_access(r[4]+28u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[0],2);if(io_status)return io_status;goto L8c601bea; /* mov.w r0,@(0x1c,r4) */
L8c601bea: *stop=0x8c601beau;if(!valid_access(r[4]+40u,4u))return -3;wr(m,bus,&io_status,r[4]+0x28u,r[0],4);if(io_status)return io_status;goto L8c601bec; /* mov.l r0,@(0x28,r4) */
L8c601bec: *stop=0x8c601becu;if(*pr!=entry_pr)return -3;*stop=0x8c601beeu;if(!valid_access(r[4]+36u,4u))return -3;wr(m,bus,&io_status,r[4]+0x24u,r[3],4);if(io_status)return io_status;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c601da6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c601da6;
L8c601da6: *stop=0x8c601da6u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c601da8; /* mov.l r14,@-r15 */
L8c601da8: *stop=0x8c601da8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c601daa; /* mov.l r13,@-r15 */
L8c601daa: *stop=0x8c601daau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c601dac; /* sts.l PR,@-r15 */
L8c601dac: *stop=0x8c601dacu;r[15]+=4294967292u;goto L8c601dae; /* add #-0x4,r15 */
L8c601dae: *stop=0x8c601daeu;r[13]=r[5];goto L8c601db0; /* mov r5,r13 */
L8c601db0: *stop=0x8c601db0u;if(!valid_access(r[13],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;r[0]=loaded;}goto L8c601db2; /* mov.l @r13,r0 */
L8c601db2: *stop=0x8c601db2u;*sr=(*sr&~1u)|!(r[0]&0x1u);goto L8c601db4; /* tst #0x1,r0 */
L8c601db4: take=(*sr&1u);*stop=0x8c601db6u;r[14]=r[4];if(take)goto L8c601dcc;goto L8c601db8; /* bt/s 0x8c601dcc */
L8c601db8: *stop=0x8c601db8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601e48u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601dba; /* mov.l 0x8c601e48,r2 */
L8c601dba: *stop=0x8c601dbau;r[5]=r[13];goto L8c601dbc; /* mov r13,r5 */
L8c601dbc: target=r[2];*pr=0x8c601dc0u;*stop=0x8c601dbeu;r[4]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601dc0; /* jsr @r2 */
L8c601dc0: *stop=0x8c601dc0u;*sr=(*sr&~1u)|((int32_t)r[0]>=0);goto L8c601dc2; /* cmp/pz r0 */
L8c601dc2: if((*sr&1u))goto L8c601dcc;goto L8c601dc4; /* bt 0x8c601dcc */
L8c601dc4: *stop=0x8c601dc4u;r[4]=0u;goto L8c601dc6; /* mov #0x0,r4 */
L8c601dc6: *stop=0x8c601dc6u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[4],4);if(io_status)return io_status;goto L8c601dc8; /* mov.l r4,@(0x4,r14) */
L8c601dc8: *stop=0x8c601dcau;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[4],4);if(io_status)return io_status;goto L8c601dec; /* bra 0x8c601dec */
L8c601dcc: *stop=0x8c601dccu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601e4cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601dce; /* mov.l 0x8c601e4c,r2 */
L8c601dce: target=r[2];*pr=0x8c601dd2u;*stop=0x8c601dd0u;r[4]=5u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601dd2; /* jsr @r2 */
L8c601dd2: *stop=0x8c601dd2u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c601dd4; /* cmp/eq #0x1,r0 */
L8c601dd4: if(!(*sr&1u))goto L8c601dde;goto L8c601dd6; /* bf 0x8c601dde */
L8c601dd6: target=0x8c601d84u;*pr=0x8c601ddau;*stop=0x8c601dd8u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c601dda; /* bsr 0x8c601d84 */
L8c601dda: *stop=0x8c601ddcu;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[0],4);if(io_status)return io_status;goto L8c601de2; /* bra 0x8c601de2 */
L8c601dde: *stop=0x8c601ddeu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c601de0; /* mov.l @r15,r3 */
L8c601de0: *stop=0x8c601de0u;if(!valid_access(r[14]+4u,4u))return -3;wr(m,bus,&io_status,r[14]+0x4u,r[3],4);if(io_status)return io_status;goto L8c601de2; /* mov.l r3,@(0x4,r14) */
L8c601de2: *stop=0x8c601de2u;r[3]=r[13];goto L8c601de4; /* mov r13,r3 */
L8c601de4: *stop=0x8c601de4u;if(!valid_access(r[14]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[14]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601de6; /* mov.l @(0x4,r14),r2 */
L8c601de6: *stop=0x8c601de6u;r[3]+=20u;goto L8c601de8; /* add #0x14,r3 */
L8c601de8: *stop=0x8c601de8u;if(!valid_access(r[14],4u))return -3;wr(m,bus,&io_status,r[14],r[2],4);if(io_status)return io_status;goto L8c601dea; /* mov.l r2,@r14 */
L8c601dea: *stop=0x8c601deau;if(!valid_access(r[14]+36u,4u))return -3;wr(m,bus,&io_status,r[14]+0x24u,r[3],4);if(io_status)return io_status;goto L8c601dec; /* mov.l r3,@(0x24,r14) */
L8c601dec: *stop=0x8c601decu;r[15]+=4u;goto L8c601dee; /* add #0x4,r15 */
L8c601dee: *stop=0x8c601deeu;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c601df0; /* lds.l @r15+,PR */
L8c601df0: *stop=0x8c601df0u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c601df2; /* mov.l @r15+,r13 */
L8c601df2: *stop=0x8c601df2u;if(*pr!=entry_pr)return -3;*stop=0x8c601df4u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c601bf0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c601bf0;
L8c601bf0: *stop=0x8c601bf0u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c601bf2; /* mov.l r14,@-r15 */
L8c601bf2: *stop=0x8c601bf2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c601bf4; /* mov.l r13,@-r15 */
L8c601bf4: *stop=0x8c601bf4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601c64u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601bf6; /* mov.l 0x8c601c64,r3 */
L8c601bf6: *stop=0x8c601bf6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601c48u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c601bf8; /* mov.l 0x8c601c48,r14 */
L8c601bf8: *stop=0x8c601bf8u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c601bfa; /* mov.l r12,@-r15 */
L8c601bfa: *stop=0x8c601bfau;if(!valid_access(r[5]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[5]+0x8u,4);if(io_status)return io_status;r[5]=loaded;}goto L8c601bfc; /* mov.l @(0x8,r5),r5 */
L8c601bfc: *stop=0x8c601bfcu;r[5]+=8u;goto L8c601bfe; /* add #0x8,r5 */
L8c601bfe: *stop=0x8c601bfeu;if(!valid_access(r[5]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x1u,1);if(io_status)return io_status;goto L8c601c00; /* mov.b @(0x1,r5),r0 */
L8c601c00: *stop=0x8c601c00u;r[13]=r[5];goto L8c601c02; /* mov r5,r13 */
L8c601c02: *stop=0x8c601c02u;if(!valid_access(r[5],1u))return -3;r[7]=sx(m,bus,&io_status,r[5],1);if(io_status)return io_status;goto L8c601c04; /* mov.b @r5,r7 */
L8c601c04: *stop=0x8c601c04u;r[13]+=3u;goto L8c601c06; /* add #0x3,r13 */
L8c601c06: *stop=0x8c601c06u;r[0]=r[0]&255u;goto L8c601c08; /* extu.b r0,r0 */
L8c601c08: *stop=0x8c601c08u;if(!valid_access(r[13],1u))return -3;r[2]=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;goto L8c601c0a; /* mov.b @r13,r2 */
L8c601c0a: *stop=0x8c601c0au;r[0]<<=8;goto L8c601c0c; /* shll8 r0 */
L8c601c0c: *stop=0x8c601c0cu;r[7]=r[7]&255u;goto L8c601c0e; /* extu.b r7,r7 */
L8c601c0e: *stop=0x8c601c0eu;r[7]|=r[0];goto L8c601c10; /* or r0,r7 */
L8c601c10: *stop=0x8c601c10u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601c58u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c601c12; /* mov.l 0x8c601c58,r0 */
L8c601c12: *stop=0x8c601c12u;r[7]|=r[3];goto L8c601c14; /* or r3,r7 */
L8c601c14: *stop=0x8c601c14u;if(!valid_access(r[0],1u))return -3;r[1]=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;goto L8c601c16; /* mov.b @r0,r1 */
L8c601c16: *stop=0x8c601c16u;*sr=(*sr&~1u)|(r[2]>=r[1]);goto L8c601c18; /* cmp/hs r1,r2 */
L8c601c18: take=!(*sr&1u);*stop=0x8c601c1au;r[14]+=r[6];if(take)goto L8c601c6c;goto L8c601c1c; /* bf/s 0x8c601c6c */
L8c601c1c: *stop=0x8c601c1cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601c68u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c601c1e; /* mov.l 0x8c601c68,r1 */
L8c601c1e: *stop=0x8c601c1eu;r[3]=1u;goto L8c601c20; /* mov #0x1,r3 */
L8c601c20: *stop=0x8c601c20u;r[7]&=r[1];goto L8c601c22; /* and r1,r7 */
L8c601c22: *stop=0x8c601c24u;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[3],1);if(io_status)return io_status;goto L8c601c8a; /* bra 0x8c601c8a */
L8c601c6c: *stop=0x8c601c6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d54u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601c6e; /* mov.l 0x8c601d54,r3 */
L8c601c6e: *stop=0x8c601c6eu;if(!valid_access(r[13],1u))return -3;r[1]=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;goto L8c601c70; /* mov.b @r13,r1 */
L8c601c70: *stop=0x8c601c70u;if(!valid_access(r[3],1u))return -3;r[2]=sx(m,bus,&io_status,r[3],1);if(io_status)return io_status;goto L8c601c72; /* mov.b @r3,r2 */
L8c601c72: *stop=0x8c601c72u;*sr=(*sr&~1u)|(r[1]>r[2]);goto L8c601c74; /* cmp/hi r2,r1 */
L8c601c74: if((*sr&1u))goto L8c601c80;goto L8c601c76; /* bt 0x8c601c80 */
L8c601c76: *stop=0x8c601c76u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d58u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c601c78; /* mov.l 0x8c601d58,r1 */
L8c601c78: *stop=0x8c601c78u;r[2]=0u;goto L8c601c7a; /* mov #0x0,r2 */
L8c601c7a: *stop=0x8c601c7au;r[7]|=r[1];goto L8c601c7c; /* or r1,r7 */
L8c601c7c: *stop=0x8c601c7eu;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[2],1);if(io_status)return io_status;goto L8c601c8a; /* bra 0x8c601c8a */
L8c601c80: *stop=0x8c601c80u;if(!valid_access(r[14],1u))return -3;r[1]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c601c82; /* mov.b @r14,r1 */
L8c601c82: *stop=0x8c601c82u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c601c84; /* tst r1,r1 */
L8c601c84: if((*sr&1u))goto L8c601c8a;goto L8c601c86; /* bt 0x8c601c8a */
L8c601c86: *stop=0x8c601c86u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d5cu,4);if(io_status)return io_status;r[1]=loaded;}goto L8c601c88; /* mov.l 0x8c601d5c,r1 */
L8c601c88: *stop=0x8c601c88u;r[7]&=r[1];goto L8c601c8a; /* and r1,r7 */
L8c601c8a: *stop=0x8c601c8au;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d64u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c601c8c; /* mov.l 0x8c601d64,r1 */
L8c601c8c: *stop=0x8c601c8cu;r[12]=r[5];goto L8c601c8e; /* mov r5,r12 */
L8c601c8e: *stop=0x8c601c8eu;r[12]+=2u;goto L8c601c90; /* add #0x2,r12 */
L8c601c90: *stop=0x8c601c90u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d60u,4);if(io_status)return io_status;r[14]=loaded;}goto L8c601c92; /* mov.l 0x8c601d60,r14 */
L8c601c92: *stop=0x8c601c92u;if(!valid_access(r[1],1u))return -3;r[2]=sx(m,bus,&io_status,r[1],1);if(io_status)return io_status;goto L8c601c94; /* mov.b @r1,r2 */
L8c601c94: *stop=0x8c601c94u;if(!valid_access(r[12],1u))return -3;r[3]=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;goto L8c601c96; /* mov.b @r12,r3 */
L8c601c96: *stop=0x8c601c96u;*sr=(*sr&~1u)|(r[3]>=r[2]);goto L8c601c98; /* cmp/hs r2,r3 */
L8c601c98: take=!(*sr&1u);*stop=0x8c601c9au;r[14]+=r[6];if(take)goto L8c601ca4;goto L8c601c9c; /* bf/s 0x8c601ca4 */
L8c601c9c: *stop=0x8c601c9cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d68u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601c9e; /* mov.l 0x8c601d68,r2 */
L8c601c9e: *stop=0x8c601c9eu;r[7]&=r[2];goto L8c601ca0; /* and r2,r7 */
L8c601ca0: *stop=0x8c601ca2u;r[3]=1u;goto L8c601cb4; /* bra 0x8c601cb4 */
L8c601ca4: *stop=0x8c601ca4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d54u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c601ca6; /* mov.l 0x8c601d54,r0 */
L8c601ca6: *stop=0x8c601ca6u;if(!valid_access(r[12],1u))return -3;r[2]=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;goto L8c601ca8; /* mov.b @r12,r2 */
L8c601ca8: *stop=0x8c601ca8u;if(!valid_access(r[0],1u))return -3;r[3]=sx(m,bus,&io_status,r[0],1);if(io_status)return io_status;goto L8c601caa; /* mov.b @r0,r3 */
L8c601caa: *stop=0x8c601caau;*sr=(*sr&~1u)|(r[2]>r[3]);goto L8c601cac; /* cmp/hi r3,r2 */
L8c601cac: if((*sr&1u))goto L8c601cb8;goto L8c601cae; /* bt 0x8c601cb8 */
L8c601cae: *stop=0x8c601caeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d6cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601cb0; /* mov.l 0x8c601d6c,r2 */
L8c601cb0: *stop=0x8c601cb0u;r[3]=0u;goto L8c601cb2; /* mov #0x0,r3 */
L8c601cb2: *stop=0x8c601cb2u;r[7]|=r[2];goto L8c601cb4; /* or r2,r7 */
L8c601cb4: *stop=0x8c601cb6u;if(!valid_access(r[14],1u))return -3;wr(m,bus,&io_status,r[14],r[3],1);if(io_status)return io_status;goto L8c601cc2; /* bra 0x8c601cc2 */
L8c601cb8: *stop=0x8c601cb8u;if(!valid_access(r[14],1u))return -3;r[2]=sx(m,bus,&io_status,r[14],1);if(io_status)return io_status;goto L8c601cba; /* mov.b @r14,r2 */
L8c601cba: *stop=0x8c601cbau;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c601cbc; /* tst r2,r2 */
L8c601cbc: if((*sr&1u))goto L8c601cc2;goto L8c601cbe; /* bt 0x8c601cc2 */
L8c601cbe: *stop=0x8c601cbeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d68u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601cc0; /* mov.l 0x8c601d68,r2 */
L8c601cc0: *stop=0x8c601cc0u;r[7]&=r[2];goto L8c601cc2; /* and r2,r7 */
L8c601cc2: *stop=0x8c601cc2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d70u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c601cc4; /* mov.l 0x8c601d70,r1 */
L8c601cc4: *stop=0x8c601cc4u;if(!valid_access(r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;r[3]=loaded;}goto L8c601cc6; /* mov.l @r1,r3 */
L8c601cc6: *stop=0x8c601cc6u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c601cc8; /* tst r3,r3 */
L8c601cc8: if(!(*sr&1u))goto L8c601cce;goto L8c601cca; /* bf 0x8c601cce */
L8c601cca: *stop=0x8c601ccau;r[3]=4294967295u;goto L8c601ccc; /* mov #-0x1,r3 */
L8c601ccc: *stop=0x8c601cccu;r[7]^=r[3];goto L8c601cce; /* xor r3,r7 */
L8c601cce: *stop=0x8c601cceu;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d6cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601cd0; /* mov.l 0x8c601d6c,r3 */
L8c601cd0: *stop=0x8c601cd0u;if(!valid_access(r[4]+8u,4u))return -3;wr(m,bus,&io_status,r[4]+0x8u,r[7],4);if(io_status)return io_status;goto L8c601cd2; /* mov.l r7,@(0x8,r4) */
L8c601cd2: *stop=0x8c601cd2u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c601cd4; /* mov.l @(0x4,r4),r0 */
L8c601cd4: *stop=0x8c601cd4u;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c601cd6; /* tst r3,r0 */
L8c601cd6: if((*sr&1u))goto L8c601cde;goto L8c601cd8; /* bt 0x8c601cde */
L8c601cd8: *stop=0x8c601cd8u;if(!valid_access(r[12],1u))return -3;r[0]=sx(m,bus,&io_status,r[12],1);if(io_status)return io_status;goto L8c601cda; /* mov.b @r12,r0 */
L8c601cda: *stop=0x8c601cdcu;r[0]=r[0]&255u;goto L8c601ce0; /* bra 0x8c601ce0 */
L8c601cde: *stop=0x8c601cdeu;r[0]=0u;goto L8c601ce0; /* mov #0x0,r0 */
L8c601ce0: *stop=0x8c601ce0u;if(!valid_access(r[4]+24u,2u))return -3;wr(m,bus,&io_status,r[4]+0x18u,r[0],2);if(io_status)return io_status;goto L8c601ce2; /* mov.w r0,@(0x18,r4) */
L8c601ce2: *stop=0x8c601ce2u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c601ce4; /* mov.l @(0x4,r4),r0 */
L8c601ce4: *stop=0x8c601ce4u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d58u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601ce6; /* mov.l 0x8c601d58,r3 */
L8c601ce6: *stop=0x8c601ce6u;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c601ce8; /* tst r3,r0 */
L8c601ce8: if((*sr&1u))goto L8c601cf0;goto L8c601cea; /* bt 0x8c601cf0 */
L8c601cea: *stop=0x8c601ceau;if(!valid_access(r[13],1u))return -3;r[0]=sx(m,bus,&io_status,r[13],1);if(io_status)return io_status;goto L8c601cec; /* mov.b @r13,r0 */
L8c601cec: *stop=0x8c601ceeu;r[0]=r[0]&255u;goto L8c601cf2; /* bra 0x8c601cf2 */
L8c601cf0: *stop=0x8c601cf0u;r[0]=0u;goto L8c601cf2; /* mov #0x0,r0 */
L8c601cf2: *stop=0x8c601cf2u;if(!valid_access(r[4]+26u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1au,r[0],2);if(io_status)return io_status;goto L8c601cf4; /* mov.w r0,@(0x1a,r4) */
L8c601cf4: *stop=0x8c601cf4u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c601cf6; /* mov.l @(0x4,r4),r0 */
L8c601cf6: *stop=0x8c601cf6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d74u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601cf8; /* mov.l 0x8c601d74,r3 */
L8c601cf8: *stop=0x8c601cf8u;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c601cfa; /* tst r3,r0 */
L8c601cfa: if((*sr&1u))goto L8c601d04;goto L8c601cfc; /* bt 0x8c601d04 */
L8c601cfc: *stop=0x8c601cfcu;if(!valid_access(r[5]+4u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x4u,1);if(io_status)return io_status;goto L8c601cfe; /* mov.b @(0x4,r5),r0 */
L8c601cfe: *stop=0x8c601cfeu;r[0]=r[0]&255u;goto L8c601d00; /* extu.b r0,r0 */
L8c601d00: *stop=0x8c601d02u;r[0]+=4294967168u;goto L8c601d06; /* bra 0x8c601d06 */
L8c601d04: *stop=0x8c601d04u;r[0]=0u;goto L8c601d06; /* mov #0x0,r0 */
L8c601d06: *stop=0x8c601d06u;if(!valid_access(r[4]+28u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1cu,r[0],2);if(io_status)return io_status;goto L8c601d08; /* mov.w r0,@(0x1c,r4) */
L8c601d08: *stop=0x8c601d08u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[0]=loaded;}goto L8c601d0a; /* mov.l @(0x4,r4),r0 */
L8c601d0a: *stop=0x8c601d0au;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d78u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601d0c; /* mov.l 0x8c601d78,r3 */
L8c601d0c: *stop=0x8c601d0cu;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c601d0e; /* tst r3,r0 */
L8c601d0e: if((*sr&1u))goto L8c601d18;goto L8c601d10; /* bt 0x8c601d18 */
L8c601d10: *stop=0x8c601d10u;if(!valid_access(r[5]+5u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x5u,1);if(io_status)return io_status;goto L8c601d12; /* mov.b @(0x5,r5),r0 */
L8c601d12: *stop=0x8c601d12u;r[0]=r[0]&255u;goto L8c601d14; /* extu.b r0,r0 */
L8c601d14: *stop=0x8c601d16u;r[0]+=4294967168u;goto L8c601d1a; /* bra 0x8c601d1a */
L8c601d18: *stop=0x8c601d18u;r[0]=0u;goto L8c601d1a; /* mov #0x0,r0 */
L8c601d1a: *stop=0x8c601d1au;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d7cu,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601d1c; /* mov.l 0x8c601d7c,r3 */
L8c601d1c: *stop=0x8c601d1cu;if(!valid_access(r[4]+30u,2u))return -3;wr(m,bus,&io_status,r[4]+0x1eu,r[0],2);if(io_status)return io_status;goto L8c601d1e; /* mov.w r0,@(0x1e,r4) */
L8c601d1e: *stop=0x8c601d1eu;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601d20; /* mov.l @(0x4,r4),r2 */
L8c601d20: *stop=0x8c601d20u;*sr=(*sr&~1u)|!(r[3]&r[2]);goto L8c601d22; /* tst r3,r2 */
L8c601d22: if((*sr&1u))goto L8c601d2e;goto L8c601d24; /* bt 0x8c601d2e */
L8c601d24: *stop=0x8c601d24u;if(!valid_access(r[5]+6u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x6u,1);if(io_status)return io_status;goto L8c601d26; /* mov.b @(0x6,r5),r0 */
L8c601d26: *stop=0x8c601d26u;r[0]=r[0]&255u;goto L8c601d28; /* extu.b r0,r0 */
L8c601d28: *stop=0x8c601d28u;r[2]=r[0];goto L8c601d2a; /* mov r0,r2 */
L8c601d2a: *stop=0x8c601d2cu;r[2]+=4294967168u;goto L8c601d30; /* bra 0x8c601d30 */
L8c601d2e: *stop=0x8c601d2eu;r[2]=0u;goto L8c601d30; /* mov #0x0,r2 */
L8c601d30: *stop=0x8c601d30u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601d80u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601d32; /* mov.l 0x8c601d80,r3 */
L8c601d32: *stop=0x8c601d32u;r[0]=32u;goto L8c601d34; /* mov #0x20,r0 */
L8c601d34: *stop=0x8c601d34u;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[2],2);if(io_status)return io_status;goto L8c601d36; /* mov.w r2,@(r0,r4) */
L8c601d36: *stop=0x8c601d36u;if(!valid_access(r[4]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c601d38; /* mov.l @(0x4,r4),r1 */
L8c601d38: *stop=0x8c601d38u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c601d3a; /* tst r3,r1 */
L8c601d3a: if((*sr&1u))goto L8c601d46;goto L8c601d3c; /* bt 0x8c601d46 */
L8c601d3c: *stop=0x8c601d3cu;if(!valid_access(r[5]+7u,1u))return -3;r[0]=sx(m,bus,&io_status,r[5]+0x7u,1);if(io_status)return io_status;goto L8c601d3e; /* mov.b @(0x7,r5),r0 */
L8c601d3e: *stop=0x8c601d3eu;r[0]=r[0]&255u;goto L8c601d40; /* extu.b r0,r0 */
L8c601d40: *stop=0x8c601d40u;r[3]=r[0];goto L8c601d42; /* mov r0,r3 */
L8c601d42: *stop=0x8c601d44u;r[3]+=4294967168u;goto L8c601d48; /* bra 0x8c601d48 */
L8c601d46: *stop=0x8c601d46u;r[3]=0u;goto L8c601d48; /* mov #0x0,r3 */
L8c601d48: *stop=0x8c601d48u;r[0]=34u;goto L8c601d4a; /* mov #0x22,r0 */
L8c601d4a: *stop=0x8c601d4au;if(!valid_access(r[0]+r[4],2u))return -3;wr(m,bus,&io_status,r[0]+r[4],r[3],2);if(io_status)return io_status;goto L8c601d4c; /* mov.w r3,@(r0,r4) */
L8c601d4c: *stop=0x8c601d4cu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;goto L8c601d4e; /* mov.l @r15+,r12 */
L8c601d4e: *stop=0x8c601d4eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[13]=loaded;}r[15]+=4;goto L8c601d50; /* mov.l @r15+,r13 */
L8c601d50: *stop=0x8c601d50u;if(*pr!=entry_pr)return -3;*stop=0x8c601d52u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[14]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c646856(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c646856;
L8c646856: *stop=0x8c646856u;r[0]=r[4];goto L8c646858; /* mov r4,r0 */
L8c646858: *stop=0x8c646858u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c64685a; /* shll r0 */
L8c64685a: *stop=0x8c64685au;r[3]=r[4];goto L8c64685c; /* mov r4,r3 */
L8c64685c: *stop=0x8c64685cu;r[0]+=r[3];goto L8c64685e; /* add r3,r0 */
L8c64685e: *stop=0x8c64685eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c6468b0u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646860; /* mov.l 0x8c6468b0,r2 */
L8c646860: *stop=0x8c646860u;r[0]<<=2;goto L8c646862; /* shll2 r0 */
L8c646862: *stop=0x8c646862u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c646864; /* shll r0 */
L8c646864: *stop=0x8c646864u;if(*pr!=entry_pr)return -3;*stop=0x8c646866u;r[0]+=r[2];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c646868(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c646868;
L8c646868: *stop=0x8c646868u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c64686a; /* sts.l PR,@-r15 */
L8c64686a: *stop=0x8c64686au;{uint32_t loaded=rd(m,bus,&io_status,0x8c6468b8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c64686c; /* mov.l 0x8c6468b8,r3 */
L8c64686c: target=r[3];*pr=0x8c646870u;*stop=0x8c64686eu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646870; /* jsr @r3 */
L8c646870: *stop=0x8c646870u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c646872; /* lds.l @r15+,PR */
L8c646872: *stop=0x8c646872u;if(*pr!=entry_pr)return -3;*stop=0x8c646874u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c645898(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c645898;
L8c645898: *stop=0x8c645898u;r[3]=0u;goto L8c64589a; /* mov #0x0,r3 */
L8c64589a: *stop=0x8c64589au;if(!valid_access(r[6]+8u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[6]+0x8u,4);if(io_status)return io_status;r[6]=loaded;}goto L8c64589c; /* mov.l @(0x8,r6),r6 */
L8c64589c: *stop=0x8c64589cu;r[2]=r[3];goto L8c64589e; /* mov r3,r2 */
L8c64589e: *stop=0x8c64589eu;if(!valid_access(r[5]+4u,4u))return -3;wr(m,bus,&io_status,r[5]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6458a0; /* mov.l r3,@(0x4,r5) */
L8c6458a0: *stop=0x8c6458a0u;if(!valid_access(r[5],4u))return -3;wr(m,bus,&io_status,r[5],r[3],4);if(io_status)return io_status;goto L8c6458a2; /* mov.l r3,@r5 */
L8c6458a2: *stop=0x8c6458a2u;r[3]=r[7];goto L8c6458a4; /* mov r7,r3 */
L8c6458a4: *stop=0x8c6458a4u;r[3]+=20u;goto L8c6458a6; /* add #0x14,r3 */
L8c6458a6: *stop=0x8c6458a6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c6458f4u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c6458a8; /* mov.l 0x8c6458f4,r2 */
L8c6458a8: *stop=0x8c6458a8u;if(!valid_access(r[5]+36u,4u))return -3;wr(m,bus,&io_status,r[5]+0x24u,r[3],4);if(io_status)return io_status;goto L8c6458aa; /* mov.l r3,@(0x24,r5) */
L8c6458aa: *stop=0x8c6458aau;r[6]+=8u;goto L8c6458ac; /* add #0x8,r6 */
L8c6458ac: *stop=0x8c6458acu;r[4]<<=2;goto L8c6458ae; /* shll2 r4 */
L8c6458ae: *stop=0x8c6458aeu;*sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6458b0; /* shll r4 */
L8c6458b0: *stop=0x8c6458b0u;r[4]+=r[2];goto L8c6458b2; /* add r2,r4 */
L8c6458b2: *stop=0x8c6458b2u;if(!valid_access(r[5]+40u,4u))return -3;wr(m,bus,&io_status,r[5]+0x28u,r[4],4);if(io_status)return io_status;goto L8c6458b4; /* mov.l r4,@(0x28,r5) */
L8c6458b4: *stop=0x8c6458b4u;r[5]=r[4];goto L8c6458b6; /* mov r4,r5 */
L8c6458b6: *stop=0x8c6458b6u;if(!valid_access(r[6],1u))return -3;r[3]=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[6]+=1;goto L8c6458b8; /* mov.b @r6+,r3 */
L8c6458b8: *stop=0x8c6458b8u;r[5]+=2u;goto L8c6458ba; /* add #0x2,r5 */
L8c6458ba: *stop=0x8c6458bau;if(!valid_access(r[4],1u))return -3;wr(m,bus,&io_status,r[4],r[3],1);if(io_status)return io_status;goto L8c6458bc; /* mov.b r3,@r4 */
L8c6458bc: *stop=0x8c6458bcu;if(!valid_access(r[6],1u))return -3;r[0]=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[6]+=1;goto L8c6458be; /* mov.b @r6+,r0 */
L8c6458be: *stop=0x8c6458beu;if(!valid_access(r[4]+1u,1u))return -3;wr(m,bus,&io_status,r[4]+0x1u,r[0],1);if(io_status)return io_status;goto L8c6458c0; /* mov.b r0,@(0x1,r4) */
L8c6458c0: *stop=0x8c6458c0u;r[4]=6u;goto L8c6458c2; /* mov #0x6,r4 */
L8c6458c2: *stop=0x8c6458c2u;if(!valid_access(r[6],1u))return -3;r[2]=sx(m,bus,&io_status,r[6],1);if(io_status)return io_status;r[6]+=1;goto L8c6458c4; /* mov.b @r6+,r2 */
L8c6458c4: *stop=0x8c6458c4u;r[4]+=4294967295u;goto L8c6458c6; /* add #-0x1,r4 */
L8c6458c6: *stop=0x8c6458c6u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c6458c8; /* tst r4,r4 */
L8c6458c8: *stop=0x8c6458c8u;if(!valid_access(r[5],1u))return -3;wr(m,bus,&io_status,r[5],r[2],1);if(io_status)return io_status;goto L8c6458ca; /* mov.b r2,@r5 */
L8c6458ca: take=!(*sr&1u);*stop=0x8c6458ccu;r[5]+=1u;if(take)goto L8c6458c2;goto L8c6458ce; /* bf/s 0x8c6458c2 */
L8c6458ce: *stop=0x8c6458ceu;if(*pr!=entry_pr)return -3;*stop=0x8c6458d0u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c601d84(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c601d84;
L8c601d84: *stop=0x8c601d84u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601e40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601d86; /* mov.l 0x8c601e40,r3 */
L8c601d86: *stop=0x8c601d86u;r[5]=r[4];goto L8c601d88; /* mov r4,r5 */
L8c601d88: *stop=0x8c601d88u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601e3cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c601d8a; /* mov.l 0x8c601e3c,r2 */
L8c601d8a: *stop=0x8c601d8au;r[5]>>=8;goto L8c601d8c; /* shlr8 r5 */
L8c601d8c: *stop=0x8c601d8cu;*sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c601d8e; /* tst r4,r3 */
L8c601d8e: *stop=0x8c601d8eu;r[5]>>=2;goto L8c601d90; /* shlr2 r5 */
L8c601d90: take=(*sr&1u);*stop=0x8c601d92u;r[5]&=r[2];if(take)goto L8c601d98;goto L8c601d94; /* bt/s 0x8c601d98 */
L8c601d94: *stop=0x8c601d94u;r[0]=sx(m,bus,&io_status,0x8c601e36u,2);if(io_status)return io_status;goto L8c601d96; /* mov.w 0x8c601e36,r0 */
L8c601d96: *stop=0x8c601d96u;r[5]|=r[0];goto L8c601d98; /* or r0,r5 */
L8c601d98: *stop=0x8c601d98u;{uint32_t loaded=rd(m,bus,&io_status,0x8c601e44u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c601d9a; /* mov.l 0x8c601e44,r3 */
L8c601d9a: *stop=0x8c601d9au;*sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c601d9c; /* tst r3,r4 */
L8c601d9c: if((*sr&1u))goto L8c601da2;goto L8c601d9e; /* bt 0x8c601da2 */
L8c601d9e: *stop=0x8c601d9eu;r[1]=15u;goto L8c601da0; /* mov #0xf,r1 */
L8c601da0: *stop=0x8c601da0u;r[5]|=r[1];goto L8c601da2; /* or r1,r5 */
L8c601da2: *stop=0x8c601da2u;if(*pr!=entry_pr)return -3;*stop=0x8c601da4u;r[0]=r[5];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c65f3a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f3a0;
L8c65f3a0: *stop=0x8c65f3a0u;r[2]=120u;goto L8c65f3a2; /* mov #0x78,r2 */
L8c65f3a2: *stop=0x8c65f3a2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f3d8u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f3a4; /* mov.l 0x8c65f3d8,r3 */
L8c65f3a4: *stop=0x8c65f3a4u;*macl=r[2]*r[4];goto L8c65f3a6; /* mul.l r2,r4 */
L8c65f3a6: *stop=0x8c65f3a6u;r[1]=sx(m,bus,&io_status,0x8c65f3cau,2);if(io_status)return io_status;goto L8c65f3a8; /* mov.w 0x8c65f3ca,r1 */
L8c65f3a8: *stop=0x8c65f3a8u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f3aa; /* mov.l @r3,r0 */
L8c65f3aa: *stop=0x8c65f3aau;r[4]=*macl;goto L8c65f3ac; /* sts MACL,r4 */
L8c65f3ac: *stop=0x8c65f3acu;r[0]+=r[1];goto L8c65f3ae; /* add r1,r0 */
L8c65f3ae: *stop=0x8c65f3aeu;if(*pr!=entry_pr)return -3;*stop=0x8c65f3b0u;r[0]+=r[4];return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c10cbb8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c10cbb8;
L8c10cbb8: *stop=0x8c10cbb8u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c10cbba; /* tst r0,r0 */
L8c10cbba: *stop=0x8c10cbbau;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c10cbbc; /* mov.l r2,@-r15 */
L8c10cbbc: if((*sr&1u))goto L8c10cc6a;goto L8c10cbbe; /* bt 0x8c10cc6a */
L8c10cbbe: *stop=0x8c10cbbeu;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c10cbc0; /* mov.l r3,@-r15 */
L8c10cbc0: *stop=0x8c10cbc0u;r[2]=0u;goto L8c10cbc2; /* mov #0x0,r2 */
L8c10cbc2: *stop=0x8c10cbc2u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c10cbc4; /* mov.l r4,@-r15 */
L8c10cbc4: *stop=0x8c10cbc4u;*sr=(*sr&~0x301u)|((r[1]>>31)<<8)|((r[2]>>31)<<9)|((r[1]^r[2])>>31);goto L8c10cbc6; /* div0s r2,r1 */
L8c10cbc6: *stop=0x8c10cbc6u;r[4]=*sr&1u;goto L8c10cbc8; /* movt r4 */
L8c10cbc8: *stop=0x8c10cbc8u;{uint64_t lhs=r[3],rhs=(uint64_t)r[3]+(*sr&1u);r[3]=(uint32_t)(lhs-rhs);*sr=(*sr&~1u)|(lhs<rhs);}goto L8c10cbca; /* subc r3,r3 */
L8c10cbca: *stop=0x8c10cbcau;{uint64_t lhs=r[1],rhs=(uint64_t)r[2]+(*sr&1u);r[1]=(uint32_t)(lhs-rhs);*sr=(*sr&~1u)|(lhs<rhs);}goto L8c10cbcc; /* subc r2,r1 */
L8c10cbcc: *stop=0x8c10cbccu;*sr=(*sr&~0x301u)|((r[3]>>31)<<8)|((r[0]>>31)<<9)|((r[3]^r[0])>>31);goto L8c10cbce; /* div0s r0,r3 */
L8c10cbce: *stop=0x8c10cbceu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cbd0; /* rotcl r1 */
L8c10cbd0: *stop=0x8c10cbd0u;div_step(r[0],&r[3],sr);goto L8c10cbd2; /* div1 r0,r3 */
L8c10cbd2: *stop=0x8c10cbd2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cbd4; /* rotcl r1 */
L8c10cbd4: *stop=0x8c10cbd4u;div_step(r[0],&r[3],sr);goto L8c10cbd6; /* div1 r0,r3 */
L8c10cbd6: *stop=0x8c10cbd6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cbd8; /* rotcl r1 */
L8c10cbd8: *stop=0x8c10cbd8u;div_step(r[0],&r[3],sr);goto L8c10cbda; /* div1 r0,r3 */
L8c10cbda: *stop=0x8c10cbdau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cbdc; /* rotcl r1 */
L8c10cbdc: *stop=0x8c10cbdcu;div_step(r[0],&r[3],sr);goto L8c10cbde; /* div1 r0,r3 */
L8c10cbde: *stop=0x8c10cbdeu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cbe0; /* rotcl r1 */
L8c10cbe0: *stop=0x8c10cbe0u;div_step(r[0],&r[3],sr);goto L8c10cbe2; /* div1 r0,r3 */
L8c10cbe2: *stop=0x8c10cbe2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cbe4; /* rotcl r1 */
L8c10cbe4: *stop=0x8c10cbe4u;div_step(r[0],&r[3],sr);goto L8c10cbe6; /* div1 r0,r3 */
L8c10cbe6: *stop=0x8c10cbe6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cbe8; /* rotcl r1 */
L8c10cbe8: *stop=0x8c10cbe8u;div_step(r[0],&r[3],sr);goto L8c10cbea; /* div1 r0,r3 */
L8c10cbea: *stop=0x8c10cbeau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cbec; /* rotcl r1 */
L8c10cbec: *stop=0x8c10cbecu;div_step(r[0],&r[3],sr);goto L8c10cbee; /* div1 r0,r3 */
L8c10cbee: *stop=0x8c10cbeeu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cbf0; /* rotcl r1 */
L8c10cbf0: *stop=0x8c10cbf0u;div_step(r[0],&r[3],sr);goto L8c10cbf2; /* div1 r0,r3 */
L8c10cbf2: *stop=0x8c10cbf2u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cbf4; /* rotcl r1 */
L8c10cbf4: *stop=0x8c10cbf4u;div_step(r[0],&r[3],sr);goto L8c10cbf6; /* div1 r0,r3 */
L8c10cbf6: *stop=0x8c10cbf6u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cbf8; /* rotcl r1 */
L8c10cbf8: *stop=0x8c10cbf8u;div_step(r[0],&r[3],sr);goto L8c10cbfa; /* div1 r0,r3 */
L8c10cbfa: *stop=0x8c10cbfau;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cbfc; /* rotcl r1 */
L8c10cbfc: *stop=0x8c10cbfcu;div_step(r[0],&r[3],sr);goto L8c10cbfe; /* div1 r0,r3 */
L8c10cbfe: *stop=0x8c10cbfeu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc00; /* rotcl r1 */
L8c10cc00: *stop=0x8c10cc00u;div_step(r[0],&r[3],sr);goto L8c10cc02; /* div1 r0,r3 */
L8c10cc02: *stop=0x8c10cc02u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc04; /* rotcl r1 */
L8c10cc04: *stop=0x8c10cc04u;div_step(r[0],&r[3],sr);goto L8c10cc06; /* div1 r0,r3 */
L8c10cc06: *stop=0x8c10cc06u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc08; /* rotcl r1 */
L8c10cc08: *stop=0x8c10cc08u;div_step(r[0],&r[3],sr);goto L8c10cc0a; /* div1 r0,r3 */
L8c10cc0a: *stop=0x8c10cc0au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc0c; /* rotcl r1 */
L8c10cc0c: *stop=0x8c10cc0cu;div_step(r[0],&r[3],sr);goto L8c10cc0e; /* div1 r0,r3 */
L8c10cc0e: *stop=0x8c10cc0eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc10; /* rotcl r1 */
L8c10cc10: *stop=0x8c10cc10u;div_step(r[0],&r[3],sr);goto L8c10cc12; /* div1 r0,r3 */
L8c10cc12: *stop=0x8c10cc12u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc14; /* rotcl r1 */
L8c10cc14: *stop=0x8c10cc14u;div_step(r[0],&r[3],sr);goto L8c10cc16; /* div1 r0,r3 */
L8c10cc16: *stop=0x8c10cc16u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc18; /* rotcl r1 */
L8c10cc18: *stop=0x8c10cc18u;div_step(r[0],&r[3],sr);goto L8c10cc1a; /* div1 r0,r3 */
L8c10cc1a: *stop=0x8c10cc1au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc1c; /* rotcl r1 */
L8c10cc1c: *stop=0x8c10cc1cu;div_step(r[0],&r[3],sr);goto L8c10cc1e; /* div1 r0,r3 */
L8c10cc1e: *stop=0x8c10cc1eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc20; /* rotcl r1 */
L8c10cc20: *stop=0x8c10cc20u;div_step(r[0],&r[3],sr);goto L8c10cc22; /* div1 r0,r3 */
L8c10cc22: *stop=0x8c10cc22u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc24; /* rotcl r1 */
L8c10cc24: *stop=0x8c10cc24u;div_step(r[0],&r[3],sr);goto L8c10cc26; /* div1 r0,r3 */
L8c10cc26: *stop=0x8c10cc26u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc28; /* rotcl r1 */
L8c10cc28: *stop=0x8c10cc28u;div_step(r[0],&r[3],sr);goto L8c10cc2a; /* div1 r0,r3 */
L8c10cc2a: *stop=0x8c10cc2au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc2c; /* rotcl r1 */
L8c10cc2c: *stop=0x8c10cc2cu;div_step(r[0],&r[3],sr);goto L8c10cc2e; /* div1 r0,r3 */
L8c10cc2e: *stop=0x8c10cc2eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc30; /* rotcl r1 */
L8c10cc30: *stop=0x8c10cc30u;div_step(r[0],&r[3],sr);goto L8c10cc32; /* div1 r0,r3 */
L8c10cc32: *stop=0x8c10cc32u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc34; /* rotcl r1 */
L8c10cc34: *stop=0x8c10cc34u;div_step(r[0],&r[3],sr);goto L8c10cc36; /* div1 r0,r3 */
L8c10cc36: *stop=0x8c10cc36u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc38; /* rotcl r1 */
L8c10cc38: *stop=0x8c10cc38u;div_step(r[0],&r[3],sr);goto L8c10cc3a; /* div1 r0,r3 */
L8c10cc3a: *stop=0x8c10cc3au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc3c; /* rotcl r1 */
L8c10cc3c: *stop=0x8c10cc3cu;div_step(r[0],&r[3],sr);goto L8c10cc3e; /* div1 r0,r3 */
L8c10cc3e: *stop=0x8c10cc3eu;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc40; /* rotcl r1 */
L8c10cc40: *stop=0x8c10cc40u;div_step(r[0],&r[3],sr);goto L8c10cc42; /* div1 r0,r3 */
L8c10cc42: *stop=0x8c10cc42u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc44; /* rotcl r1 */
L8c10cc44: *stop=0x8c10cc44u;div_step(r[0],&r[3],sr);goto L8c10cc46; /* div1 r0,r3 */
L8c10cc46: *stop=0x8c10cc46u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc48; /* rotcl r1 */
L8c10cc48: *stop=0x8c10cc48u;div_step(r[0],&r[3],sr);goto L8c10cc4a; /* div1 r0,r3 */
L8c10cc4a: *stop=0x8c10cc4au;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c10cc4c; /* rotcl r1 */
L8c10cc4c: *stop=0x8c10cc4cu;div_step(r[0],&r[3],sr);goto L8c10cc4e; /* div1 r0,r3 */
L8c10cc4e: *stop=0x8c10cc4eu;*sr=(*sr&~0x301u)|((r[3]>>31)<<8)|((r[2]>>31)<<9)|((r[3]^r[2])>>31);goto L8c10cc50; /* div0s r2,r3 */
L8c10cc50: *stop=0x8c10cc50u;r[2]=*sr&1u;goto L8c10cc52; /* movt r2 */
L8c10cc52: *stop=0x8c10cc52u;r[2]^=r[4];goto L8c10cc54; /* xor r4,r2 */
L8c10cc54: *stop=0x8c10cc54u;{uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(incoming<<31);}goto L8c10cc56; /* rotcr r2 */
L8c10cc56: if(!(*sr&1u))goto L8c10cc5e;goto L8c10cc58; /* bf 0x8c10cc5e */
L8c10cc58: *stop=0x8c10cc58u;*sr=(*sr&~0x301u)|((r[3]>>31)<<8)|((r[0]>>31)<<9)|((r[3]^r[0])>>31);goto L8c10cc5a; /* div0s r0,r3 */
L8c10cc5a: *stop=0x8c10cc5au;*sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u);goto L8c10cc5c; /* shar r3 */
L8c10cc5c: *stop=0x8c10cc5cu;div_step(r[0],&r[3],sr);goto L8c10cc5e; /* div1 r0,r3 */
L8c10cc5e: *stop=0x8c10cc5eu;r[3]+=r[4];goto L8c10cc60; /* add r4,r3 */
L8c10cc60: *stop=0x8c10cc60u;r[0]=r[3];goto L8c10cc62; /* mov r3,r0 */
L8c10cc62: *stop=0x8c10cc62u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[4]=loaded;}r[15]+=4;goto L8c10cc64; /* mov.l @r15+,r4 */
L8c10cc64: *stop=0x8c10cc64u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}r[15]+=4;goto L8c10cc66; /* mov.l @r15+,r3 */
L8c10cc66: *stop=0x8c10cc66u;if(*pr!=entry_pr)return -3;*stop=0x8c10cc68u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;return 0; /* rts */
L8c10cc6a: *stop=0x8c10cc6au;{uint32_t loaded=rd(m,bus,&io_status,0x8c10cc78u,4);if(io_status)return io_status;r[1]=loaded;}goto L8c10cc6c; /* mov.l 0x8c10cc78,r1 */
L8c10cc6c: *stop=0x8c10cc6cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c10cc7cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c10cc6e; /* mov.l 0x8c10cc7c,r2 */
L8c10cc6e: *stop=0x8c10cc6eu;r[0]=0u;goto L8c10cc70; /* mov #0x0,r0 */
L8c10cc70: *stop=0x8c10cc70u;if(!valid_access(r[1],4u))return -3;wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c10cc72; /* mov.l r2,@r1 */
L8c10cc72: *stop=0x8c10cc72u;if(*pr!=entry_pr)return -3;*stop=0x8c10cc74u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[2]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c646b72(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c646b72;
L8c646b72: *stop=0x8c646b72u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c646b74; /* sts.l PR,@-r15 */
L8c646b74: *stop=0x8c646b74u;r[15]+=4294967288u;goto L8c646b76; /* add #-0x8,r15 */
L8c646b76: *stop=0x8c646b76u;if(!valid_access(r[15],4u))return -3;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c646b78; /* mov.l r4,@r15 */
L8c646b78: *stop=0x8c646b78u;if(!valid_access(r[15]+4u,4u))return -3;wr(m,bus,&io_status,r[15]+0x4u,r[5],4);if(io_status)return io_status;goto L8c646b7a; /* mov.l r5,@(0x4,r15) */
L8c646b7a: *stop=0x8c646b7au;r[5]=1u;goto L8c646b7c; /* mov #0x1,r5 */
L8c646b7c: target=0x8c646b42u;*pr=0x8c646b80u;*stop=0x8c646b7eu;if(!valid_access(r[15]+4u,4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;r[4]=loaded;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c646b80; /* bsr 0x8c646b42 */
L8c646b80: *stop=0x8c646b80u;r[4]=r[0];goto L8c646b82; /* mov r0,r4 */
L8c646b82: *stop=0x8c646b82u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c646b84; /* tst r4,r4 */
L8c646b84: if(!(*sr&1u))goto L8c646b8e;goto L8c646b86; /* bf 0x8c646b8e */
L8c646b86: *stop=0x8c646b86u;r[15]+=8u;goto L8c646b88; /* add #0x8,r15 */
L8c646b88: *stop=0x8c646b88u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c646b8a; /* lds.l @r15+,PR */
L8c646b8a: *stop=0x8c646b8au;if(*pr!=entry_pr)return -3;*stop=0x8c646b8cu;r[0]=4294967295u;return 0; /* rts */
L8c646b8e: *stop=0x8c646b8eu;if(!valid_access(r[4]+1u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x1u,1);if(io_status)return io_status;goto L8c646b90; /* mov.b @(0x1,r4),r0 */
L8c646b90: *stop=0x8c646b90u;if(!valid_access(r[4],1u))return -3;r[1]=sx(m,bus,&io_status,r[4],1);if(io_status)return io_status;goto L8c646b92; /* mov.b @r4,r1 */
L8c646b92: *stop=0x8c646b92u;r[0]=r[0]&255u;goto L8c646b94; /* extu.b r0,r0 */
L8c646b94: *stop=0x8c646b94u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[3]=loaded;}goto L8c646b96; /* mov.l @r15,r3 */
L8c646b96: *stop=0x8c646b96u;r[0]<<=16;goto L8c646b98; /* shll16 r0 */
L8c646b98: *stop=0x8c646b98u;r[2]=r[0];goto L8c646b9a; /* mov r0,r2 */
L8c646b9a: *stop=0x8c646b9au;if(!valid_access(r[4]+2u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x2u,1);if(io_status)return io_status;goto L8c646b9c; /* mov.b @(0x2,r4),r0 */
L8c646b9c: *stop=0x8c646b9cu;r[1]=r[1]&255u;goto L8c646b9e; /* extu.b r1,r1 */
L8c646b9e: *stop=0x8c646b9eu;r[0]=r[0]&255u;goto L8c646ba0; /* extu.b r0,r0 */
L8c646ba0: *stop=0x8c646ba0u;r[0]<<=8;goto L8c646ba2; /* shll8 r0 */
L8c646ba2: *stop=0x8c646ba2u;r[2]|=r[0];goto L8c646ba4; /* or r0,r2 */
L8c646ba4: *stop=0x8c646ba4u;if(!valid_access(r[4]+3u,1u))return -3;r[0]=sx(m,bus,&io_status,r[4]+0x3u,1);if(io_status)return io_status;goto L8c646ba6; /* mov.b @(0x3,r4),r0 */
L8c646ba6: *stop=0x8c646ba6u;r[1]<<=16;goto L8c646ba8; /* shll16 r1 */
L8c646ba8: *stop=0x8c646ba8u;r[1]<<=8;goto L8c646baa; /* shll8 r1 */
L8c646baa: *stop=0x8c646baau;r[0]=r[0]&255u;goto L8c646bac; /* extu.b r0,r0 */
L8c646bac: *stop=0x8c646bacu;r[2]|=r[1];goto L8c646bae; /* or r1,r2 */
L8c646bae: *stop=0x8c646baeu;r[2]|=r[0];goto L8c646bb0; /* or r0,r2 */
L8c646bb0: *stop=0x8c646bb0u;r[0]=0u;goto L8c646bb2; /* mov #0x0,r0 */
L8c646bb2: *stop=0x8c646bb2u;if(!valid_access(r[3],4u))return -3;wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c646bb4; /* mov.l r2,@r3 */
L8c646bb4: *stop=0x8c646bb4u;r[15]+=8u;goto L8c646bb6; /* add #0x8,r15 */
L8c646bb6: *stop=0x8c646bb6u;if(!valid_access(r[15],4u))return -3;*pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4u;goto L8c646bb8; /* lds.l @r15+,PR */
L8c646bb8: *stop=0x8c646bb8u;if(*pr!=entry_pr)return -3;*stop=0x8c646bbau;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c646b42(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c646b42;
L8c646b42: *stop=0x8c646b42u;r[7]=r[4];goto L8c646b44; /* mov r4,r7 */
L8c646b44: *stop=0x8c646b44u;if(!valid_access(r[15]-4u,4u))return -3;r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c646b46; /* mov.l r12,@-r15 */
L8c646b46: *stop=0x8c646b46u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646c3cu,4);if(io_status)return io_status;r[6]=loaded;}goto L8c646b48; /* mov.l 0x8c646c3c,r6 */
L8c646b48: *stop=0x8c646b48u;r[0]=32u;goto L8c646b4a; /* mov #0x20,r0 */
L8c646b4a: *stop=0x8c646b4au;if(!valid_access(r[4],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[12]=loaded;}r[4]+=4;goto L8c646b4c; /* mov.l @r4+,r12 */
L8c646b4c: *stop=0x8c646b4cu;r[7]+=4u;goto L8c646b4e; /* add #0x4,r7 */
L8c646b4e: *stop=0x8c646b4eu;r[1]=0u;goto L8c646b50; /* mov #0x0,r1 */
L8c646b50: *stop=0x8c646b50u;r[3]=r[6];goto L8c646b52; /* mov r6,r3 */
L8c646b52: *stop=0x8c646b52u;*sr=(*sr&~1u)|!(r[12]&r[3]);goto L8c646b54; /* tst r12,r3 */
L8c646b54: if((*sr&1u))goto L8c646b64;goto L8c646b56; /* bt 0x8c646b64 */
L8c646b56: *stop=0x8c646b56u;*sr=(*sr&~1u)|(r[6]==r[5]);goto L8c646b58; /* cmp/eq r6,r5 */
L8c646b58: if(!(*sr&1u))goto L8c646b60;goto L8c646b5a; /* bf 0x8c646b60 */
L8c646b5a: *stop=0x8c646b5au;r[0]=r[7];goto L8c646b5c; /* mov r7,r0 */
L8c646b5c: *stop=0x8c646b5cu;if(*pr!=entry_pr)return -3;*stop=0x8c646b5eu;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;return 0; /* rts */
L8c646b60: *stop=0x8c646b60u;r[4]+=4u;goto L8c646b62; /* add #0x4,r4 */
L8c646b62: *stop=0x8c646b62u;r[7]=r[4];goto L8c646b64; /* mov r4,r7 */
L8c646b64: *stop=0x8c646b64u;r[1]+=1u;goto L8c646b66; /* add #0x1,r1 */
L8c646b66: *stop=0x8c646b66u;*sr=(*sr&~1u)|(r[6]&1u);r[6]>>=1;goto L8c646b68; /* shlr r6 */
L8c646b68: *stop=0x8c646b68u;*sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[0]);goto L8c646b6a; /* cmp/ge r0,r1 */
L8c646b6a: if(!(*sr&1u))goto L8c646b50;goto L8c646b6c; /* bf 0x8c646b50 */
L8c646b6c: *stop=0x8c646b6cu;r[0]=0u;goto L8c646b6e; /* mov #0x0,r0 */
L8c646b6e: *stop=0x8c646b6eu;if(*pr!=entry_pr)return -3;*stop=0x8c646b70u;if(!valid_access(r[15],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[12]=loaded;}r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c646e5a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c646e5a;
L8c646e5a: *stop=0x8c646e5au;r[0]=r[4];goto L8c646e5c; /* mov r4,r0 */
L8c646e5c: *stop=0x8c646e5cu;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c646e5e; /* cmp/eq #0x1,r0 */
L8c646e5e: if((*sr&1u))goto L8c646e88;goto L8c646e60; /* bt 0x8c646e88 */
L8c646e60: *stop=0x8c646e60u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c646e62; /* cmp/eq #0x2,r0 */
L8c646e62: if((*sr&1u))goto L8c646e8e;goto L8c646e64; /* bt 0x8c646e8e */
L8c646e64: *stop=0x8c646e64u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c646e66; /* cmp/eq #0x4,r0 */
L8c646e66: if((*sr&1u))goto L8c646e94;goto L8c646e68; /* bt 0x8c646e94 */
L8c646e68: *stop=0x8c646e68u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c646e6a; /* cmp/eq #0x3,r0 */
L8c646e6a: if((*sr&1u))goto L8c646e9a;goto L8c646e6c; /* bt 0x8c646e9a */
L8c646e6c: *stop=0x8c646e6cu;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c646e6e; /* cmp/eq #0x6,r0 */
L8c646e6e: if((*sr&1u))goto L8c646ea0;goto L8c646e70; /* bt 0x8c646ea0 */
L8c646e70: *stop=0x8c646e70u;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c646e72; /* cmp/eq #0x5,r0 */
L8c646e72: if((*sr&1u))goto L8c646ea6;goto L8c646e74; /* bt 0x8c646ea6 */
L8c646e74: *stop=0x8c646e74u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c646e76; /* cmp/eq #0x0,r0 */
L8c646e76: if((*sr&1u))goto L8c646eac;goto L8c646e78; /* bt 0x8c646eac */
L8c646e78: *stop=0x8c646e78u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c646e7a; /* cmp/eq #0x7,r0 */
L8c646e7a: if((*sr&1u))goto L8c646eb2;goto L8c646e7c; /* bt 0x8c646eb2 */
L8c646e7c: *stop=0x8c646e7cu;*sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c646e7e; /* cmp/eq #0x8,r0 */
L8c646e7e: if((*sr&1u))goto L8c646eb8;goto L8c646e80; /* bt 0x8c646eb8 */
L8c646e80: *stop=0x8c646e80u;*sr=(*sr&~1u)|(r[0]==0x00000009u);goto L8c646e82; /* cmp/eq #0x9,r0 */
L8c646e82: if((*sr&1u))goto L8c646ebe;goto L8c646e84; /* bt 0x8c646ebe */
L8c646e84: *stop=0x8c646e86u;goto L8c646ec4; /* bra 0x8c646ec4 */
L8c646e88: *stop=0x8c646e88u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646f28u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646e8a; /* mov.l 0x8c646f28,r3 */
L8c646e8a: target=r[3];*stop=0x8c646e8cu;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c646e8e: *stop=0x8c646e8eu;{uint32_t loaded=rd(m,bus,&io_status,0x8c646f2cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646e90; /* mov.l 0x8c646f2c,r2 */
L8c646e90: target=r[2];*stop=0x8c646e92u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c646e94: *stop=0x8c646e94u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646f30u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646e96; /* mov.l 0x8c646f30,r3 */
L8c646e96: target=r[3];*stop=0x8c646e98u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c646e9a: *stop=0x8c646e9au;{uint32_t loaded=rd(m,bus,&io_status,0x8c646f34u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646e9c; /* mov.l 0x8c646f34,r2 */
L8c646e9c: target=r[2];*stop=0x8c646e9eu;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c646ea0: *stop=0x8c646ea0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646f38u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646ea2; /* mov.l 0x8c646f38,r3 */
L8c646ea2: target=r[3];*stop=0x8c646ea4u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c646ea6: *stop=0x8c646ea6u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646f3cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646ea8; /* mov.l 0x8c646f3c,r2 */
L8c646ea8: target=r[2];*stop=0x8c646eaau;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c646eac: *stop=0x8c646eacu;{uint32_t loaded=rd(m,bus,&io_status,0x8c646f40u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646eae; /* mov.l 0x8c646f40,r3 */
L8c646eae: target=r[3];*stop=0x8c646eb0u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c646eb2: *stop=0x8c646eb2u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646f44u,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646eb4; /* mov.l 0x8c646f44,r2 */
L8c646eb4: target=r[2];*stop=0x8c646eb6u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c646eb8: *stop=0x8c646eb8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c646f48u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c646eba; /* mov.l 0x8c646f48,r3 */
L8c646eba: target=r[3];*stop=0x8c646ebcu;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
L8c646ebe: *stop=0x8c646ebeu;{uint32_t loaded=rd(m,bus,&io_status,0x8c646f4cu,4);if(io_status)return io_status;r[2]=loaded;}goto L8c646ec0; /* mov.l 0x8c646f4c,r2 */
L8c646ec0: target=r[2];*stop=0x8c646ec2u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c646ec4: *stop=0x8c646ec4u;if(*pr!=entry_pr)return -3;*stop=0x8c646ec6u;r[0]=4294967295u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c65f458(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f458;
L8c65f458: *stop=0x8c65f458u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f530u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f45a; /* mov.l 0x8c65f530,r3 */
L8c65f45a: *stop=0x8c65f45au;r[1]=sx(m,bus,&io_status,0x8c65f518u,2);if(io_status)return io_status;goto L8c65f45c; /* mov.w 0x8c65f518,r1 */
L8c65f45c: *stop=0x8c65f45cu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f45e; /* mov.l @r3,r0 */
L8c65f45e: *stop=0x8c65f45eu;if(*pr!=entry_pr)return -3;*stop=0x8c65f460u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c65f4da(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f4da;
L8c65f4da: *stop=0x8c65f4dau;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f530u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f4dc; /* mov.l 0x8c65f530,r3 */
L8c65f4dc: *stop=0x8c65f4dcu;r[1]=sx(m,bus,&io_status,0x8c65f524u,2);if(io_status)return io_status;goto L8c65f4de; /* mov.w 0x8c65f524,r1 */
L8c65f4de: *stop=0x8c65f4deu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f4e0; /* mov.l @r3,r0 */
L8c65f4e0: *stop=0x8c65f4e0u;if(*pr!=entry_pr)return -3;*stop=0x8c65f4e2u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c65f46c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f46c;
L8c65f46c: *stop=0x8c65f46cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f530u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f46e; /* mov.l 0x8c65f530,r3 */
L8c65f46e: *stop=0x8c65f46eu;r[1]=sx(m,bus,&io_status,0x8c65f51au,2);if(io_status)return io_status;goto L8c65f470; /* mov.w 0x8c65f51a,r1 */
L8c65f470: *stop=0x8c65f470u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f472; /* mov.l @r3,r0 */
L8c65f472: *stop=0x8c65f472u;if(*pr!=entry_pr)return -3;*stop=0x8c65f474u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c65f43a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f43a;
L8c65f43a: *stop=0x8c65f43au;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f530u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f43c; /* mov.l 0x8c65f530,r3 */
L8c65f43c: *stop=0x8c65f43cu;r[1]=sx(m,bus,&io_status,0x8c65f516u,2);if(io_status)return io_status;goto L8c65f43e; /* mov.w 0x8c65f516,r1 */
L8c65f43e: *stop=0x8c65f43eu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f440; /* mov.l @r3,r0 */
L8c65f440: *stop=0x8c65f440u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f442; /* mov.l @(r0,r1),r0 */
L8c65f442: *stop=0x8c65f442u;r[0]>>=16;goto L8c65f444; /* shlr16 r0 */
L8c65f444: *stop=0x8c65f444u;r[0]=(uint32_t)(int32_t)(int16_t)r[0];goto L8c65f446; /* exts.w r0,r0 */
L8c65f446: *stop=0x8c65f446u;if(*pr!=entry_pr)return -3;*stop=0x8c65f448u;r[0]=r[0]&65535u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c65f480(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f480;
L8c65f480: *stop=0x8c65f480u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f530u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f482; /* mov.l 0x8c65f530,r3 */
L8c65f482: *stop=0x8c65f482u;r[1]=sx(m,bus,&io_status,0x8c65f51cu,2);if(io_status)return io_status;goto L8c65f484; /* mov.w 0x8c65f51c,r1 */
L8c65f484: *stop=0x8c65f484u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f486; /* mov.l @r3,r0 */
L8c65f486: *stop=0x8c65f486u;if(*pr!=entry_pr)return -3;*stop=0x8c65f488u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c65f4c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f4c0;
L8c65f4c0: *stop=0x8c65f4c0u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f530u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f4c2; /* mov.l 0x8c65f530,r3 */
L8c65f4c2: *stop=0x8c65f4c2u;r[1]=sx(m,bus,&io_status,0x8c65f520u,2);if(io_status)return io_status;goto L8c65f4c4; /* mov.w 0x8c65f520,r1 */
L8c65f4c4: *stop=0x8c65f4c4u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f4c6; /* mov.l @r3,r0 */
L8c65f4c6: *stop=0x8c65f4c6u;if(*pr!=entry_pr)return -3;*stop=0x8c65f4c8u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c65f4ac(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f4ac;
L8c65f4ac: *stop=0x8c65f4acu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f530u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f4ae; /* mov.l 0x8c65f530,r3 */
L8c65f4ae: *stop=0x8c65f4aeu;r[1]=sx(m,bus,&io_status,0x8c65f51eu,2);if(io_status)return io_status;goto L8c65f4b0; /* mov.w 0x8c65f51e,r1 */
L8c65f4b0: *stop=0x8c65f4b0u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f4b2; /* mov.l @r3,r0 */
L8c65f4b2: *stop=0x8c65f4b2u;if(*pr!=entry_pr)return -3;*stop=0x8c65f4b4u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c65f4f8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f4f8;
L8c65f4f8: *stop=0x8c65f4f8u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f530u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f4fa; /* mov.l 0x8c65f530,r3 */
L8c65f4fa: *stop=0x8c65f4fau;r[1]=sx(m,bus,&io_status,0x8c65f528u,2);if(io_status)return io_status;goto L8c65f4fc; /* mov.w 0x8c65f528,r1 */
L8c65f4fc: *stop=0x8c65f4fcu;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f4fe; /* mov.l @r3,r0 */
L8c65f4fe: *stop=0x8c65f4feu;if(*pr!=entry_pr)return -3;*stop=0x8c65f500u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c65f50c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f50c;
L8c65f50c: *stop=0x8c65f50cu;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f530u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f50e; /* mov.l 0x8c65f530,r3 */
L8c65f50e: *stop=0x8c65f50eu;r[1]=sx(m,bus,&io_status,0x8c65f52cu,2);if(io_status)return io_status;goto L8c65f510; /* mov.w 0x8c65f52c,r1 */
L8c65f510: *stop=0x8c65f510u;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f512; /* mov.l @r3,r0 */
L8c65f512: *stop=0x8c65f512u;if(*pr!=entry_pr)return -3;*stop=0x8c65f514u;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}return 0; /* rts */
}

SA_STAGE_EXPORT int sa_boot_input_8c65f546(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer){
 int io_status=0;(void)io_status;(void)bus;(void)integer;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f546;
L8c65f546: *stop=0x8c65f546u;{uint32_t loaded=rd(m,bus,&io_status,0x8c65f554u,4);if(io_status)return io_status;r[3]=loaded;}goto L8c65f548; /* mov.l 0x8c65f554,r3 */
L8c65f548: *stop=0x8c65f548u;r[1]=sx(m,bus,&io_status,0x8c65f550u,2);if(io_status)return io_status;goto L8c65f54a; /* mov.w 0x8c65f550,r1 */
L8c65f54a: *stop=0x8c65f54au;if(!valid_access(r[3],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;r[0]=loaded;}goto L8c65f54c; /* mov.l @r3,r0 */
L8c65f54c: *stop=0x8c65f54cu;if(*pr!=entry_pr)return -3;*stop=0x8c65f54eu;if(!valid_access(r[0]+r[1],4u))return -3;{uint32_t loaded=rd(m,bus,&io_status,r[1]+r[0],4);if(io_status)return io_status;r[0]=loaded;}return 0; /* rts */
}
