/* UV-constant alternate strip emitters with portable queue interface; names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "vertex_bus.h"
#include "sh4_vector_numeric.h"
#include "sh4_fmac.h"
#include "sh4_divide.h"
#include "sh4_add_sub.h"
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
SA_STAGE_EXPORT int sa_uv_constant_alternate_strip_8c61430e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61430e;
L8c61430e: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c614310; /* mov.l r14,@-r15 */
L8c614310: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c614312; /* mov.l r13,@-r15 */
L8c614312: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c614314; /* mov.l r12,@-r15 */
L8c614314: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c614316; /* mov.l r11,@-r15 */
L8c614316: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c614318; /* mov.l r10,@-r15 */
L8c614318: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61431a; /* mov.l r9,@-r15 */
L8c61431a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61431c; /* fmov.s fr15,@-r15 */
L8c61431c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61431e; /* fmov.s fr14,@-r15 */
L8c61431e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c614320; /* fmov.s fr13,@-r15 */
L8c614320: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c614322; /* sts.l PR,@-r15 */
L8c614322: r[15]+=4294967276u;goto L8c614324; /* add #-0x14,r15 */
L8c614324: r[3]=rd(m,bus,&io_status,0x8c614358u,4);if(io_status)return io_status;goto L8c614326; /* mov.l 0x8c614358,r3 */
L8c614326: r[9]=rd(m,bus,&io_status,0x8c614354u,4);if(io_status)return io_status;goto L8c614328; /* mov.l 0x8c614354,r9 */
L8c614328: target=r[3];*pr=0x8c61432cu;r[12]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61432c; /* jsr @r3 */
L8c61432c: r[2]=rd(m,bus,&io_status,0x8c61435cu,4);if(io_status)return io_status;goto L8c61432e; /* mov.l 0x8c61435c,r2 */
L8c61432e: r[1]=0u;goto L8c614330; /* mov #0x0,r1 */
L8c614330: r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c614332; /* mov.w @(0x2,r12),r0 */
L8c614332: r[14]=rd(m,bus,&io_status,r[12]+0x14u,4);if(io_status)return io_status;goto L8c614334; /* mov.l @(0x14,r12),r14 */
L8c614334: r[13]=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;goto L8c614336; /* mov.l @(0x4,r12),r13 */
L8c614336: r[0]=r[0]&65535u;goto L8c614338; /* extu.w r0,r0 */
L8c614338: wr(m,bus,&io_status,r[15]+0x10u,r[0],4);if(io_status)return io_status;goto L8c61433a; /* mov.l r0,@(0x10,r15) */
L8c61433a: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61433c; /* mov.l @r2,r3 */
L8c61433c: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61433e; /* mov.l r3,@(0x4,r15) */
L8c61433e: wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c614340; /* mov.l r1,@(0x8,r15) */
L8c614340: wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c614342; /* mov.l r1,@(0xc,r15) */
L8c614342: goto L8c614360; /* bra 0x8c614360 */
L8c614360: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c614362; /* mov.w @r13+,r3 */
L8c614362: r[4]=rd(m,bus,&io_status,0x8c61444cu,4);if(io_status)return io_status;goto L8c614364; /* mov.l 0x8c61444c,r4 */
L8c614364: r[3]=r[3]&65535u;goto L8c614366; /* extu.w r3,r3 */
L8c614366: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c614368; /* fldi1 fr4 */
L8c614368: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c61436a; /* tst r4,r3 */
L8c61436a: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61436c; /* mov.l r3,@r15 */
L8c61436c: take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c61437e;goto L8c614370; /* bt/s 0x8c61437e */
L8c614370: r[4]=sx(m,bus,&io_status,0x8c61444au,2);if(io_status)return io_status;goto L8c614372; /* mov.w 0x8c61444a,r4 */
L8c614372: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614374; /* mov.l @r15,r3 */
L8c614374: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c614376; /* fldi1 fr4 */
L8c614376: r[3]&=r[4];goto L8c614378; /* and r4,r3 */
L8c614378: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c61437a; /* fneg fr4 */
L8c61437a: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61437c; /* mov.l r3,@r15 */
L8c61437c: fcopy(f,15u,4u);goto L8c61437e; /* fmov fr4,fr15 */
L8c61437e: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614380; /* mov.l @r15,r3 */
L8c614380: r[0]=16u;goto L8c614382; /* mov #0x10,r0 */
L8c614382: r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614384; /* mov.l @(0xc,r15),r2 */
L8c614384: r[10]=r[8];goto L8c614386; /* mov r8,r10 */
L8c614386: r[2]+=r[3];goto L8c614388; /* add r3,r2 */
L8c614388: wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c61438a; /* mov.l r2,@(0xc,r15) */
L8c61438a: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61438c; /* mov.l @r15,r1 */
L8c61438c: r[1]+=4294967294u;goto L8c61438e; /* add #-0x2,r1 */
L8c61438e: wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c614390; /* mov.l r1,@r15 */
L8c614390: r[4]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c614392; /* mov.w @r13+,r4 */
L8c614392: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614394; /* mov.l @(0x4,r15),r3 */
L8c614394: r[11]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c614396; /* mov.w @r13+,r11 */
L8c614396: r[4]=r[4]&65535u;goto L8c614398; /* extu.w r4,r4 */
L8c614398: r[4]<<=2;goto L8c61439a; /* shll2 r4 */
L8c61439a: r[11]=r[11]&65535u;goto L8c61439c; /* extu.w r11,r11 */
L8c61439c: r[11]<<=2;goto L8c61439e; /* shll2 r11 */
L8c61439e: r[4]<<=2;goto L8c6143a0; /* shll2 r4 */
L8c6143a0: r[11]<<=2;goto L8c6143a2; /* shll2 r11 */
L8c6143a2: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6143a4; /* shll r4 */
L8c6143a4: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c6143a6; /* shll r11 */
L8c6143a6: r[11]+=r[3];goto L8c6143a8; /* add r3,r11 */
L8c6143a8: r[4]+=r[3];goto L8c6143aa; /* add r3,r4 */
L8c6143aa: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c6143ac; /* fmov.s @(r0,r11),fr14 */
L8c6143ac: fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c6143ae; /* fmov.s @(r0,r4),fr4 */
L8c6143ae: r[0]=20u;goto L8c6143b0; /* mov #0x14,r0 */
L8c6143b0: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c6143b2; /* fmov.s @(r0,r11),fr13 */
L8c6143b2: fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c6143b4; /* fmov.s @(r0,r4),fr5 */
L8c6143b4: r[12]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6143b6; /* mov.w @r13+,r12 */
L8c6143b6: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6143b8; /* mov.l @(0x4,r15),r3 */
L8c6143b8: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c6143ba; /* mov.w @r13,r2 */
L8c6143ba: r[12]=r[12]&65535u;goto L8c6143bc; /* extu.w r12,r12 */
L8c6143bc: r[12]<<=2;goto L8c6143be; /* shll2 r12 */
L8c6143be: r[2]=r[2]&65535u;goto L8c6143c0; /* extu.w r2,r2 */
L8c6143c0: r[2]<<=2;goto L8c6143c2; /* shll2 r2 */
L8c6143c2: r[2]<<=2;goto L8c6143c4; /* shll2 r2 */
L8c6143c4: r[12]<<=2;goto L8c6143c6; /* shll2 r12 */
L8c6143c6: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c6143c8; /* shll r12 */
L8c6143c8: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6143ca; /* shll r2 */
L8c6143ca: r[2]+=r[3];goto L8c6143cc; /* add r3,r2 */
L8c6143cc: r[12]+=r[3];goto L8c6143ce; /* add r3,r12 */
L8c6143ce: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c6143d0; /* pref @r2 */
L8c6143d0: r[0]=20u;goto L8c6143d2; /* mov #0x14,r0 */
L8c6143d2: fcopy(f,3u,14u);goto L8c6143d4; /* fmov fr14,fr3 */
L8c6143d4: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6143d6; /* fmov.s @(r0,r12),fr2 */
L8c6143d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6143d8; /* fsub fr4,fr3 */
L8c6143d8: r[0]=16u;goto L8c6143da; /* mov #0x10,r0 */
L8c6143da: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c6143dc; /* cmp/hs r8,r10 */
L8c6143dc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6143de; /* fsub fr13,fr2 */
L8c6143de: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c6143e0; /* fmov.s @(r0,r12),fr1 */
L8c6143e0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6143e2; /* fsub fr14,fr1 */
L8c6143e2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6143e4; /* fmul fr2,fr3 */
L8c6143e4: fcopy(f,2u,13u);goto L8c6143e6; /* fmov fr13,fr2 */
L8c6143e6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6143e8; /* fsub fr5,fr2 */
L8c6143e8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6143ea; /* fmul fr1,fr2 */
L8c6143ea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6143ec; /* fsub fr2,fr3 */
L8c6143ec: fcopy(f,4u,3u);goto L8c6143ee; /* fmov fr3,fr4 */
L8c6143ee: take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c614450;goto L8c6143f2; /* bt/s 0x8c614450 */
L8c6143f2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6143f4; /* fldi0 fr3 */
L8c6143f4: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6143f6; /* fcmp/gt fr4,fr3 */
L8c6143f6: if((*sr&1u))goto L8c614494;goto L8c6143f8; /* bt 0x8c614494 */
L8c6143f8: r[6]=r[14];goto L8c6143fa; /* mov r14,r6 */
L8c6143fa: r[4]=r[11];goto L8c6143fc; /* mov r11,r4 */
L8c6143fc: r[5]=r[9];goto L8c6143fe; /* mov r9,r5 */
L8c6143fe: r[14]+=4u;goto L8c614400; /* add #0x4,r14 */
L8c614400: r[4]+=16u;goto L8c614402; /* add #0x10,r4 */
L8c614402: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614404; /* fschg */
L8c614404: r[8]+=16u;goto L8c614406; /* add #0x10,r8 */
L8c614406: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614408; /* fmov.s @r4+,fr4 */
L8c614408: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61440a; /* fmov.s @r4,fr6 */
L8c61440a: fcopy(f,0u,1u);goto L8c61440c; /* fmov fr1,fr0 */
L8c61440c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61440e; /* fschg */
L8c61440e: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c614410; /* mov.w @r6+,r2 */
L8c614410: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c614412; /* mov.w @r6,r3 */
L8c614412: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614414; /* fmov.s fr6,@-r8 */
L8c614414: *fpul=r[3];goto L8c614416; /* lds r3,FPUL */
L8c614416: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614418; /* fmov.s fr5,@-r8 */
L8c614418: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61441a; /* float FPUL,fr1 */
L8c61441a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61441c; /* fmov.s fr4,@-r8 */
L8c61441c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61441e; /* fmul fr0,fr1 */
L8c61441e: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614420; /* mov.l r5,@-r8 */
L8c614420: *fpul=r[2];goto L8c614422; /* lds r2,FPUL */
L8c614422: r[8]+=28u;goto L8c614424; /* add #0x1c,r8 */
L8c614424: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c614426; /* float FPUL,fr2 */
L8c614426: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c614428; /* fldi1 fr3 */
L8c614428: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61442a; /* fmul fr0,fr2 */
L8c61442a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61442c; /* fmov.s fr3,@-r8 */
L8c61442c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61442e; /* fmov.s fr1,@-r8 */
L8c61442e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614430; /* fmov.s fr2,@-r8 */
L8c614430: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614432; /* pref @r8 */
L8c614432: r[8]+=16u;goto L8c614434; /* add #0x10,r8 */
L8c614434: r[3]=r[8];goto L8c614436; /* mov r8,r3 */
L8c614436: r[3]-=r[10];goto L8c614438; /* sub r10,r3 */
L8c614438: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61443a; /* mov.l @(0x8,r15),r2 */
L8c61443a: r[3]+=32u;goto L8c61443c; /* add #0x20,r3 */
L8c61443c: r[14]+=4294967292u;goto L8c61443e; /* add #-0x4,r14 */
L8c61443e: r[2]+=r[3];goto L8c614440; /* add r3,r2 */
L8c614440: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614442; /* mov.l r2,@(0x8,r15) */
L8c614442: r[10]=r[8];goto L8c6144d0; /* bra 0x8c6144d0 */
L8c614450: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614452; /* fldi0 fr3 */
L8c614452: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c614454; /* fcmp/gt fr4,fr3 */
L8c614454: take=!(*sr&1u);r[14]+=4u;if(take)goto L8c6144d0;goto L8c614458; /* bf/s 0x8c6144d0 */
L8c614458: r[14]+=4294967292u;goto L8c61445a; /* add #-0x4,r14 */
L8c61445a: r[5]=rd(m,bus,&io_status,0x8c6145b8u,4);if(io_status)return io_status;goto L8c61445c; /* mov.l 0x8c6145b8,r5 */
L8c61445c: r[6]=r[14];goto L8c61445e; /* mov r14,r6 */
L8c61445e: r[14]+=4u;goto L8c614460; /* add #0x4,r14 */
L8c614460: r[4]+=16u;goto L8c614462; /* add #0x10,r4 */
L8c614462: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614464; /* fschg */
L8c614464: r[8]+=16u;goto L8c614466; /* add #0x10,r8 */
L8c614466: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614468; /* fmov.s @r4+,fr4 */
L8c614468: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61446a; /* fmov.s @r4,fr6 */
L8c61446a: fcopy(f,0u,1u);goto L8c61446c; /* fmov fr1,fr0 */
L8c61446c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61446e; /* fschg */
L8c61446e: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c614470; /* mov.w @r6+,r2 */
L8c614470: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c614472; /* mov.w @r6,r3 */
L8c614472: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614474; /* fmov.s fr6,@-r8 */
L8c614474: *fpul=r[3];goto L8c614476; /* lds r3,FPUL */
L8c614476: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614478; /* fmov.s fr5,@-r8 */
L8c614478: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61447a; /* float FPUL,fr1 */
L8c61447a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61447c; /* fmov.s fr4,@-r8 */
L8c61447c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61447e; /* fmul fr0,fr1 */
L8c61447e: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614480; /* mov.l r5,@-r8 */
L8c614480: *fpul=r[2];goto L8c614482; /* lds r2,FPUL */
L8c614482: r[8]+=28u;goto L8c614484; /* add #0x1c,r8 */
L8c614484: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c614486; /* float FPUL,fr2 */
L8c614486: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c614488; /* fldi1 fr3 */
L8c614488: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61448a; /* fmul fr0,fr2 */
L8c61448a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61448c; /* fmov.s fr3,@-r8 */
L8c61448c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61448e; /* fmov.s fr1,@-r8 */
L8c61448e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614490; /* fmov.s fr2,@-r8 */
L8c614490: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614492; /* pref @r8 */
L8c614492: r[8]+=16u;goto L8c614494; /* add #0x10,r8 */
L8c614494: r[5]=rd(m,bus,&io_status,0x8c6145b8u,4);if(io_status)return io_status;goto L8c614496; /* mov.l 0x8c6145b8,r5 */
L8c614496: r[6]=r[14];goto L8c614498; /* mov r14,r6 */
L8c614498: r[4]=r[11];goto L8c61449a; /* mov r11,r4 */
L8c61449a: r[14]+=4u;goto L8c61449c; /* add #0x4,r14 */
L8c61449c: r[4]+=16u;goto L8c61449e; /* add #0x10,r4 */
L8c61449e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6144a0; /* fschg */
L8c6144a0: r[8]+=16u;goto L8c6144a2; /* add #0x10,r8 */
L8c6144a2: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6144a4; /* fmov.s @r4+,fr4 */
L8c6144a4: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6144a6; /* fmov.s @r4,fr6 */
L8c6144a6: fcopy(f,0u,1u);goto L8c6144a8; /* fmov fr1,fr0 */
L8c6144a8: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6144aa; /* fschg */
L8c6144aa: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c6144ac; /* mov.w @r6+,r2 */
L8c6144ac: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c6144ae; /* mov.w @r6,r3 */
L8c6144ae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6144b0; /* fmov.s fr6,@-r8 */
L8c6144b0: *fpul=r[3];goto L8c6144b2; /* lds r3,FPUL */
L8c6144b2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6144b4; /* fmov.s fr5,@-r8 */
L8c6144b4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c6144b6; /* float FPUL,fr1 */
L8c6144b6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6144b8; /* fmov.s fr4,@-r8 */
L8c6144b8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6144ba; /* fmul fr0,fr1 */
L8c6144ba: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6144bc; /* mov.l r5,@-r8 */
L8c6144bc: *fpul=r[2];goto L8c6144be; /* lds r2,FPUL */
L8c6144be: r[8]+=28u;goto L8c6144c0; /* add #0x1c,r8 */
L8c6144c0: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c6144c2; /* float FPUL,fr2 */
L8c6144c2: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6144c4; /* fldi1 fr3 */
L8c6144c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6144c6; /* fmul fr0,fr2 */
L8c6144c6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6144c8; /* fmov.s fr3,@-r8 */
L8c6144c8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6144ca; /* fmov.s fr1,@-r8 */
L8c6144ca: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6144cc; /* fmov.s fr2,@-r8 */
L8c6144cc: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6144ce; /* pref @r8 */
L8c6144ce: r[8]+=16u;goto L8c6144d0; /* add #0x10,r8 */
L8c6144d0: fcopy(f,4u,14u);goto L8c6144d2; /* fmov fr14,fr4 */
L8c6144d2: r[0]=16u;goto L8c6144d4; /* mov #0x10,r0 */
L8c6144d4: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6144d6; /* mov.l @r15,r3 */
L8c6144d6: r[4]=r[11];goto L8c6144d8; /* mov r11,r4 */
L8c6144d8: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c6144da; /* fmov.s @(r0,r12),fr14 */
L8c6144da: r[0]=20u;goto L8c6144dc; /* mov #0x14,r0 */
L8c6144dc: fcopy(f,5u,13u);goto L8c6144de; /* fmov fr13,fr5 */
L8c6144de: r[3]+=4294967295u;goto L8c6144e0; /* add #-0x1,r3 */
L8c6144e0: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c6144e2; /* fmov.s @(r0,r12),fr13 */
L8c6144e2: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6144e4; /* tst r3,r3 */
L8c6144e4: r[11]=r[12];goto L8c6144e6; /* mov r12,r11 */
L8c6144e6: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c6144e8; /* fneg fr15 */
L8c6144e8: take=(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c6144f0;goto L8c6144ec; /* bt/s 0x8c6144f0 */
L8c6144ec: goto L8c6143b4; /* bra 0x8c6143b4 */
L8c6144f0: *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c6144f2; /* cmp/hs r8,r10 */
L8c6144f2: take=(*sr&1u);r[14]+=8u;if(take)goto L8c614540;goto L8c6144f6; /* bt/s 0x8c614540 */
L8c6144f6: r[14]+=4294967288u;goto L8c6144f8; /* add #-0x8,r14 */
L8c6144f8: r[4]=r[12];goto L8c6144fa; /* mov r12,r4 */
L8c6144fa: r[6]=r[14];goto L8c6144fc; /* mov r14,r6 */
L8c6144fc: r[5]=r[9];goto L8c6144fe; /* mov r9,r5 */
L8c6144fe: r[14]+=4u;goto L8c614500; /* add #0x4,r14 */
L8c614500: r[4]+=16u;goto L8c614502; /* add #0x10,r4 */
L8c614502: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614504; /* fschg */
L8c614504: r[8]+=16u;goto L8c614506; /* add #0x10,r8 */
L8c614506: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614508; /* fmov.s @r4+,fr4 */
L8c614508: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61450a; /* fmov.s @r4,fr6 */
L8c61450a: fcopy(f,0u,1u);goto L8c61450c; /* fmov fr1,fr0 */
L8c61450c: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61450e; /* fschg */
L8c61450e: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c614510; /* mov.w @r6+,r2 */
L8c614510: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c614512; /* mov.w @r6,r3 */
L8c614512: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614514; /* fmov.s fr6,@-r8 */
L8c614514: *fpul=r[3];goto L8c614516; /* lds r3,FPUL */
L8c614516: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614518; /* fmov.s fr5,@-r8 */
L8c614518: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61451a; /* float FPUL,fr1 */
L8c61451a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61451c; /* fmov.s fr4,@-r8 */
L8c61451c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61451e; /* fmul fr0,fr1 */
L8c61451e: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614520; /* mov.l r5,@-r8 */
L8c614520: *fpul=r[2];goto L8c614522; /* lds r2,FPUL */
L8c614522: r[8]+=28u;goto L8c614524; /* add #0x1c,r8 */
L8c614524: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c614526; /* float FPUL,fr2 */
L8c614526: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c614528; /* fldi1 fr3 */
L8c614528: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61452a; /* fmul fr0,fr2 */
L8c61452a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61452c; /* fmov.s fr3,@-r8 */
L8c61452c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61452e; /* fmov.s fr1,@-r8 */
L8c61452e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614530; /* fmov.s fr2,@-r8 */
L8c614530: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614532; /* pref @r8 */
L8c614532: r[8]+=16u;goto L8c614534; /* add #0x10,r8 */
L8c614534: r[3]=r[8];goto L8c614536; /* mov r8,r3 */
L8c614536: r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614538; /* mov.l @(0x8,r15),r2 */
L8c614538: r[3]-=r[10];goto L8c61453a; /* sub r10,r3 */
L8c61453a: r[3]+=32u;goto L8c61453c; /* add #0x20,r3 */
L8c61453c: r[2]+=r[3];goto L8c61453e; /* add r3,r2 */
L8c61453e: wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614540; /* mov.l r2,@(0x8,r15) */
L8c614540: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c614542; /* mov.l @(0x10,r15),r3 */
L8c614542: r[3]+=4294967295u;goto L8c614544; /* add #-0x1,r3 */
L8c614544: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614546; /* tst r3,r3 */
L8c614546: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;if(take)goto L8c61454e;goto L8c61454a; /* bt/s 0x8c61454e */
L8c61454a: goto L8c614360; /* bra 0x8c614360 */
L8c61454e: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614550; /* mov.l @(0x8,r15),r3 */
L8c614550: r[4]=rd(m,bus,&io_status,0x8c6145bcu,4);if(io_status)return io_status;goto L8c614552; /* mov.l 0x8c6145bc,r4 */
L8c614552: r[3]>>=2;goto L8c614554; /* shlr2 r3 */
L8c614554: r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614556; /* mov.l @r4,r1 */
L8c614556: r[3]>>=2;goto L8c614558; /* shlr2 r3 */
L8c614558: *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c61455a; /* shlr r3 */
L8c61455a: r[1]+=r[3];goto L8c61455c; /* add r3,r1 */
L8c61455c: wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c61455e; /* mov.l r1,@r4 */
L8c61455e: r[4]=rd(m,bus,&io_status,0x8c6145c0u,4);if(io_status)return io_status;goto L8c614560; /* mov.l 0x8c6145c0,r4 */
L8c614560: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614562; /* mov.l @(0xc,r15),r3 */
L8c614562: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614564; /* mov.l @r4,r2 */
L8c614564: r[2]+=r[3];goto L8c614566; /* add r3,r2 */
L8c614566: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c614568; /* mov.l r2,@r4 */
L8c614568: r[15]+=20u;goto L8c61456a; /* add #0x14,r15 */
L8c61456a: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61456c; /* lds.l @r15+,PR */
L8c61456c: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61456e; /* fmov.s @r15+,fr13 */
L8c61456e: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614570; /* fmov.s @r15+,fr14 */
L8c614570: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614572; /* fmov.s @r15+,fr15 */
L8c614572: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614574; /* mov.l @r15+,r9 */
L8c614574: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614576; /* mov.l @r15+,r10 */
L8c614576: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614578; /* mov.l @r15+,r11 */
L8c614578: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61457a; /* mov.l @r15+,r12 */
L8c61457a: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61457c; /* mov.l @r15+,r13 */
L8c61457c: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_uv_constant_alternate_strip_clipped_8c616068(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c616068;
L8c616068: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61606a; /* mov.l r14,@-r15 */
L8c61606a: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61606c; /* mov.l r13,@-r15 */
L8c61606c: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61606e; /* mov.l r12,@-r15 */
L8c61606e: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c616070; /* mov.l r11,@-r15 */
L8c616070: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c616072; /* mov.l r10,@-r15 */
L8c616072: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c616074; /* mov.l r9,@-r15 */
L8c616074: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c616076; /* fmov.s fr15,@-r15 */
L8c616076: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c616078; /* fmov.s fr14,@-r15 */
L8c616078: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61607a; /* fmov.s fr13,@-r15 */
L8c61607a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61607c; /* fmov.s fr12,@-r15 */
L8c61607c: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61607e; /* sts.l PR,@-r15 */
L8c61607e: r[15]+=4294967216u;goto L8c616080; /* add #-0x50,r15 */
L8c616080: r[3]=rd(m,bus,&io_status,0x8c6160a0u,4);if(io_status)return io_status;goto L8c616082; /* mov.l 0x8c6160a0,r3 */
L8c616082: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c616084; /* fldi1 fr12 */
L8c616084: target=r[3];*pr=0x8c616088u;r[12]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616088; /* jsr @r3 */
L8c616088: r[2]=rd(m,bus,&io_status,0x8c6160a4u,4);if(io_status)return io_status;goto L8c61608a; /* mov.l 0x8c6160a4,r2 */
L8c61608a: r[1]=0u;goto L8c61608c; /* mov #0x0,r1 */
L8c61608c: r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c61608e; /* mov.w @(0x2,r12),r0 */
L8c61608e: r[13]=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;goto L8c616090; /* mov.l @(0x4,r12),r13 */
L8c616090: r[14]=rd(m,bus,&io_status,r[12]+0x14u,4);if(io_status)return io_status;goto L8c616092; /* mov.l @(0x14,r12),r14 */
L8c616092: r[0]=r[0]&65535u;goto L8c616094; /* extu.w r0,r0 */
L8c616094: wr(m,bus,&io_status,r[15]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c616096; /* mov.l r0,@(0x1c,r15) */
L8c616096: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c616098; /* mov.l @r2,r3 */
L8c616098: wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c61609a; /* mov.l r3,@(0xc,r15) */
L8c61609a: wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c61609c; /* mov.l r1,@(0x10,r15) */
L8c61609c: goto L8c6160a8; /* bra 0x8c6160a8 */
L8c6160a8: r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6160aa; /* mov.w @r13+,r3 */
L8c6160aa: r[4]=rd(m,bus,&io_status,0x8c6161c8u,4);if(io_status)return io_status;goto L8c6160ac; /* mov.l 0x8c6161c8,r4 */
L8c6160ac: r[3]=r[3]&65535u;goto L8c6160ae; /* extu.w r3,r3 */
L8c6160ae: *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6160b0; /* tst r4,r3 */
L8c6160b0: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6160b2; /* mov.l r3,@(0x8,r15) */
L8c6160b2: take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c6160c4;goto L8c6160b6; /* bt/s 0x8c6160c4 */
L8c6160b6: r[4]=sx(m,bus,&io_status,0x8c6161c4u,2);if(io_status)return io_status;goto L8c6160b8; /* mov.w 0x8c6161c4,r4 */
L8c6160b8: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6160ba; /* mov.l @(0x8,r15),r3 */
L8c6160ba: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6160bc; /* fldi1 fr4 */
L8c6160bc: r[3]&=r[4];goto L8c6160be; /* and r4,r3 */
L8c6160be: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c6160c0; /* fneg fr4 */
L8c6160c0: wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6160c2; /* mov.l r3,@(0x8,r15) */
L8c6160c2: fcopy(f,15u,4u);goto L8c6160c4; /* fmov fr4,fr15 */
L8c6160c4: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6160c6; /* mov.l @(0x8,r15),r3 */
L8c6160c6: r[0]=16u;goto L8c6160c8; /* mov #0x10,r0 */
L8c6160c8: r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6160ca; /* mov.l @(0x10,r15),r2 */
L8c6160ca: r[9]=r[8];goto L8c6160cc; /* mov r8,r9 */
L8c6160cc: r[2]+=r[3];goto L8c6160ce; /* add r3,r2 */
L8c6160ce: wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6160d0; /* mov.l r2,@(0x10,r15) */
L8c6160d0: r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6160d2; /* mov.l @(0x8,r15),r1 */
L8c6160d2: r[1]+=4294967294u;goto L8c6160d4; /* add #-0x2,r1 */
L8c6160d4: wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c6160d6; /* mov.l r1,@(0x8,r15) */
L8c6160d6: r[10]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6160d8; /* mov.w @r13+,r10 */
L8c6160d8: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6160da; /* mov.l @(0xc,r15),r3 */
L8c6160da: r[11]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6160dc; /* mov.w @r13+,r11 */
L8c6160dc: r[10]=r[10]&65535u;goto L8c6160de; /* extu.w r10,r10 */
L8c6160de: r[10]<<=2;goto L8c6160e0; /* shll2 r10 */
L8c6160e0: r[11]=r[11]&65535u;goto L8c6160e2; /* extu.w r11,r11 */
L8c6160e2: r[11]<<=2;goto L8c6160e4; /* shll2 r11 */
L8c6160e4: r[10]<<=2;goto L8c6160e6; /* shll2 r10 */
L8c6160e6: r[11]<<=2;goto L8c6160e8; /* shll2 r11 */
L8c6160e8: *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c6160ea; /* shll r10 */
L8c6160ea: *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c6160ec; /* shll r11 */
L8c6160ec: r[11]+=r[3];goto L8c6160ee; /* add r3,r11 */
L8c6160ee: r[10]+=r[3];goto L8c6160f0; /* add r3,r10 */
L8c6160f0: fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c6160f2; /* fmov.s @(r0,r11),fr13 */
L8c6160f2: fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c6160f4; /* fmov.s @(r0,r10),fr5 */
L8c6160f4: r[0]=20u;goto L8c6160f6; /* mov #0x14,r0 */
L8c6160f6: fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c6160f8; /* fmov.s @(r0,r11),fr14 */
L8c6160f8: fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c6160fa; /* fmov.s @(r0,r10),fr6 */
L8c6160fa: r[12]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6160fc; /* mov.w @r13+,r12 */
L8c6160fc: r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6160fe; /* mov.l @(0xc,r15),r3 */
L8c6160fe: r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c616100; /* mov.w @r13,r2 */
L8c616100: r[12]=r[12]&65535u;goto L8c616102; /* extu.w r12,r12 */
L8c616102: r[12]<<=2;goto L8c616104; /* shll2 r12 */
L8c616104: r[2]=r[2]&65535u;goto L8c616106; /* extu.w r2,r2 */
L8c616106: r[2]<<=2;goto L8c616108; /* shll2 r2 */
L8c616108: r[2]<<=2;goto L8c61610a; /* shll2 r2 */
L8c61610a: r[12]<<=2;goto L8c61610c; /* shll2 r12 */
L8c61610c: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61610e; /* shll r12 */
L8c61610e: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c616110; /* shll r2 */
L8c616110: r[2]+=r[3];goto L8c616112; /* add r3,r2 */
L8c616112: r[12]+=r[3];goto L8c616114; /* add r3,r12 */
L8c616114: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c616116; /* pref @r2 */
L8c616116: r[3]=0u;goto L8c616118; /* mov #0x0,r3 */
L8c616118: wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c61611a; /* mov.l r3,@(0x18,r15) */
L8c61611a: r[4]=63u;goto L8c61611c; /* mov #0x3f,r4 */
L8c61611c: r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c61611e; /* mov.l @(0xc,r10),r2 */
L8c61611e: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c616120; /* mov.l @(0xc,r11),r3 */
L8c616120: r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c616122; /* mov.l @(0xc,r12),r1 */
L8c616122: r[2]&=r[3];goto L8c616124; /* and r3,r2 */
L8c616124: r[2]&=r[1];goto L8c616126; /* and r1,r2 */
L8c616126: r[4]&=r[2];goto L8c616128; /* and r2,r4 */
L8c616128: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c61612a; /* tst r4,r4 */
L8c61612a: take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c616132;goto L8c61612e; /* bt/s 0x8c616132 */
L8c61612e: goto L8c61628c; /* bra 0x8c61628c */
L8c616132: r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c616134; /* mov.l @(0xc,r10),r4 */
L8c616134: r[5]=1u;goto L8c616136; /* mov #0x1,r5 */
L8c616136: r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c616138; /* mov.l @(0xc,r11),r3 */
L8c616138: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61613a; /* mov.l @(0xc,r12),r2 */
L8c61613a: r[4]|=r[3];goto L8c61613c; /* or r3,r4 */
L8c61613c: r[4]|=r[2];goto L8c61613e; /* or r2,r4 */
L8c61613e: r[4]&=r[5];goto L8c616140; /* and r5,r4 */
L8c616140: *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c616142; /* tst r4,r4 */
L8c616142: if(!(*sr&1u))goto L8c616148;goto L8c616144; /* bf 0x8c616148 */
L8c616144: goto L8c61626e; /* bra 0x8c61626e */
L8c616148: r[7]=r[15];goto L8c61614a; /* mov r15,r7 */
L8c61614a: r[7]+=68u;goto L8c61614c; /* add #0x44,r7 */
L8c61614c: wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c61614e; /* mov.l r10,@r7 */
L8c61614e: wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c616150; /* mov.l r11,@(0x4,r7) */
L8c616150: wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c616152; /* mov.l r12,@(0x8,r7) */
L8c616152: r[6]=0u;goto L8c616200; /* bra 0x8c616200 */
L8c616156: r[2]=r[6];goto L8c616158; /* mov r6,r2 */
L8c616158: r[1]=r[6];goto L8c61615a; /* mov r6,r1 */
L8c61615a: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c61615c; /* shll r2 */
L8c61615c: r[3]=r[6];goto L8c61615e; /* mov r6,r3 */
L8c61615e: r[3]<<=2;goto L8c616160; /* shll2 r3 */
L8c616160: r[2]+=r[1];goto L8c616162; /* add r1,r2 */
L8c616162: r[2]<<=2;goto L8c616164; /* shll2 r2 */
L8c616164: r[3]+=r[7];goto L8c616166; /* add r7,r3 */
L8c616166: r[2]+=r[4];goto L8c616168; /* add r4,r2 */
L8c616168: wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61616a; /* mov.l r3,@(0x4,r15) */
L8c61616a: wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61616c; /* mov.l r2,@r15 */
L8c61616c: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61616e; /* mov.l @r3,r1 */
L8c61616e: r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c616170; /* mov.l @(0xc,r1),r2 */
L8c616170: *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c616172; /* tst r5,r2 */
L8c616172: take=!(*sr&1u);r[6]+=1u;if(take)goto L8c6161d8;goto L8c616176; /* bf/s 0x8c6161d8 */
L8c616176: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616178; /* mov.l @(0x4,r15),r1 */
L8c616178: r[0]=24u;goto L8c61617a; /* mov #0x18,r0 */
L8c61617a: fcopy(f,4u,12u);goto L8c61617c; /* fmov fr12,fr4 */
L8c61617c: r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61617e; /* mov.l @r1,r3 */
L8c61617e: r[4]=rd(m,bus,&io_status,0x8c6161ccu,4);if(io_status)return io_status;goto L8c616180; /* mov.l 0x8c6161cc,r4 */
L8c616180: wr(m,bus,&io_status,r[15]+0x14u,r[3],4);if(io_status)return io_status;goto L8c616182; /* mov.l r3,@(0x14,r15) */
L8c616182: fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616184; /* fmov.s @(r0,r3),fr3 */
L8c616184: r[0]=12u;goto L8c616186; /* mov #0xc,r0 */
L8c616186: r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c616188; /* mov.l @(0x14,r15),r2 */
L8c616188: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61618a; /* fdiv fr3,fr4 */
L8c61618a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61618c; /* fmov.s @(r0,r4),fr3 */
L8c61618c: r[0]=16u;goto L8c61618e; /* mov #0x10,r0 */
L8c61618e: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616190; /* mov.l @r15,r3 */
L8c616190: fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c616192; /* fmov.s @(r0,r2),fr2 */
L8c616192: r[0]=16u;goto L8c616194; /* mov #0x10,r0 */
L8c616194: r[2]=rd(m,bus,&io_status,0x8c6161d0u,4);if(io_status)return io_status;goto L8c616196; /* mov.l 0x8c6161d0,r2 */
L8c616196: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616198; /* fsub fr3,fr2 */
L8c616198: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61619a; /* fmov.s @r2,fr3 */
L8c61619a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61619c; /* fmul fr4,fr2 */
L8c61619c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61619e; /* fdiv fr3,fr2 */
L8c61619e: fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6161a0; /* fmov.s fr2,@r3 */
L8c6161a0: r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6161a2; /* mov.l @(0x4,r15),r1 */
L8c6161a2: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6161a4; /* fmov.s @(r0,r4),fr3 */
L8c6161a4: r[0]=20u;goto L8c6161a6; /* mov #0x14,r0 */
L8c6161a6: r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c6161a8; /* mov.l @r1,r1 */
L8c6161a8: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6161aa; /* mov.l @r15,r3 */
L8c6161aa: fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c6161ac; /* fmov.s @(r0,r1),fr2 */
L8c6161ac: r[1]=rd(m,bus,&io_status,0x8c6161d4u,4);if(io_status)return io_status;goto L8c6161ae; /* mov.l 0x8c6161d4,r1 */
L8c6161ae: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6161b0; /* fsub fr3,fr2 */
L8c6161b0: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6161b2; /* fmov.s @r1,fr3 */
L8c6161b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6161b4; /* fmul fr4,fr2 */
L8c6161b4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6161b6; /* fdiv fr3,fr2 */
L8c6161b6: r[0]=4u;goto L8c6161b8; /* mov #0x4,r0 */
L8c6161b8: fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c6161ba; /* fmov fr2,@(r0,r3) */
L8c6161ba: r[0]=8u;goto L8c6161bc; /* mov #0x8,r0 */
L8c6161bc: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6161be; /* mov.l @r15,r3 */
L8c6161be: fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c6161c0; /* fmov fr4,@(r0,r3) */
L8c6161c0: goto L8c616200; /* bra 0x8c616200 */
L8c6161d8: r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6161da; /* mov.l @(0x4,r15),r3 */
L8c6161da: r[0]=16u;goto L8c6161dc; /* mov #0x10,r0 */
L8c6161dc: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6161de; /* mov.l @r15,r2 */
L8c6161de: r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6161e0; /* mov.l @r3,r1 */
L8c6161e0: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6161e2; /* fmov.s @(r0,r1),fr3 */
L8c6161e2: r[0]=20u;goto L8c6161e4; /* mov #0x14,r0 */
L8c6161e4: fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6161e6; /* fmov.s fr3,@r2 */
L8c6161e6: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6161e8; /* mov.l @(0x4,r15),r2 */
L8c6161e8: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6161ea; /* mov.l @r15,r3 */
L8c6161ea: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6161ec; /* mov.l @r2,r1 */
L8c6161ec: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6161ee; /* fmov.s @(r0,r1),fr3 */
L8c6161ee: r[0]=4u;goto L8c6161f0; /* mov #0x4,r0 */
L8c6161f0: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6161f2; /* fmov fr3,@(r0,r3) */
L8c6161f2: r[0]=24u;goto L8c6161f4; /* mov #0x18,r0 */
L8c6161f4: r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6161f6; /* mov.l @(0x4,r15),r2 */
L8c6161f6: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6161f8; /* mov.l @r15,r3 */
L8c6161f8: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6161fa; /* mov.l @r2,r1 */
L8c6161fa: fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6161fc; /* fmov.s @(r0,r1),fr3 */
L8c6161fc: r[0]=8u;goto L8c6161fe; /* mov #0x8,r0 */
L8c6161fe: fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616200; /* fmov fr3,@(r0,r3) */
L8c616200: r[3]=3u;goto L8c616202; /* mov #0x3,r3 */
L8c616202: r[4]=r[15];goto L8c616204; /* mov r15,r4 */
L8c616204: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c616206; /* cmp/ge r3,r6 */
L8c616206: take=!(*sr&1u);r[4]+=32u;if(take)goto L8c616156;goto L8c61620a; /* bf/s 0x8c616156 */
L8c61620a: r[7]=r[4];goto L8c61620c; /* mov r4,r7 */
L8c61620c: r[0]=8u;goto L8c61620e; /* mov #0x8,r0 */
L8c61620e: r[3]=r[4];goto L8c616210; /* mov r4,r3 */
L8c616210: fload(m,bus,&io_status,f,8u,r[0]+r[7]);if(io_status)return io_status;goto L8c616212; /* fmov.s @(r0,r7),fr8 */
L8c616212: r[3]+=12u;goto L8c616214; /* add #0xc,r3 */
L8c616214: r[6]=r[4];goto L8c616216; /* mov r4,r6 */
L8c616216: r[0]=4u;goto L8c616218; /* mov #0x4,r0 */
L8c616218: fcopy(f,0u,8u);goto L8c61621a; /* fmov fr8,fr0 */
L8c61621a: r[6]+=24u;goto L8c61621c; /* add #0x18,r6 */
L8c61621c: wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61621e; /* mov.l r3,@r15 */
L8c61621e: fload(m,bus,&io_status,f,9u,r[0]+r[7]);if(io_status)return io_status;goto L8c616220; /* fmov.s @(r0,r7),fr9 */
L8c616220: fload(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c616222; /* fmov.s @(r0,r3),fr4 */
L8c616222: r[0]=8u;goto L8c616224; /* mov #0x8,r0 */
L8c616224: fload(m,bus,&io_status,f,6u,r[0]+r[3]);if(io_status)return io_status;goto L8c616226; /* fmov.s @(r0,r3),fr6 */
L8c616226: r[0]=4u;goto L8c616228; /* mov #0x4,r0 */
L8c616228: fcopy(f,3u,9u);goto L8c61622a; /* fmov fr9,fr3 */
L8c61622a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61622c; /* fmul fr4,fr0 */
L8c61622c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61622e; /* fmul fr6,fr3 */
L8c61622e: fload(m,bus,&io_status,f,7u,r[7]);if(io_status)return io_status;goto L8c616230; /* fmov.s @r7,fr7 */
L8c616230: fload(m,bus,&io_status,f,5u,r[3]);if(io_status)return io_status;goto L8c616232; /* fmov.s @r3,fr5 */
L8c616232: fcopy(f,2u,7u);goto L8c616234; /* fmov fr7,fr2 */
L8c616234: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616236; /* fmul fr6,fr2 */
L8c616236: fcopy(f,1u,9u);goto L8c616238; /* fmov fr9,fr1 */
L8c616238: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c61623a; /* fsub fr3,fr0 */
L8c61623a: fcopy(f,3u,8u);goto L8c61623c; /* fmov fr8,fr3 */
L8c61623c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61623e; /* fmul fr5,fr3 */
L8c61623e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616240; /* fsub fr3,fr2 */
L8c616240: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c616242; /* fmov.s @(r0,r6),fr3 */
L8c616242: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616244; /* fmul fr2,fr3 */
L8c616244: fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c616246; /* fmov.s @r6,fr2 */
L8c616246: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616248; /* fmac fr0,fr2,fr3 */
L8c616248: fcopy(f,2u,3u);goto L8c61624a; /* fmov fr3,fr2 */
L8c61624a: fcopy(f,3u,7u);goto L8c61624c; /* fmov fr7,fr3 */
L8c61624c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61624e; /* fmul fr4,fr3 */
L8c61624e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[5],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616250; /* fmul fr5,fr1 */
L8c616250: r[0]=8u;goto L8c616252; /* mov #0x8,r0 */
L8c616252: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c616254; /* fsub fr3,fr1 */
L8c616254: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c616256; /* fmov.s @(r0,r6),fr3 */
L8c616256: fcopy(f,0u,1u);goto L8c616258; /* fmov fr1,fr0 */
L8c616258: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61625a; /* fmac fr0,fr3,fr2 */
L8c61625a: fcopy(f,3u,15u);goto L8c61625c; /* fmov fr15,fr3 */
L8c61625c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c61625e; /* fneg fr3 */
L8c61625e: fcopy(f,4u,2u);goto L8c616260; /* fmov fr2,fr4 */
L8c616260: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616262; /* fmul fr3,fr4 */
L8c616262: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c616264; /* fldi0 fr2 */
L8c616264: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c616266; /* fcmp/gt fr2,fr4 */
L8c616266: if((*sr&1u))goto L8c61628c;goto L8c616268; /* bt 0x8c61628c */
L8c616268: wr(m,bus,&io_status,r[15]+0x18u,r[5],4);if(io_status)return io_status;goto L8c61626a; /* mov.l r5,@(0x18,r15) */
L8c61626a: fcopy(f,4u,12u);goto L8c61628c; /* bra 0x8c61628c */
L8c61626e: r[0]=20u;goto L8c616270; /* mov #0x14,r0 */
L8c616270: fcopy(f,3u,13u);goto L8c616272; /* fmov fr13,fr3 */
L8c616272: fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c616274; /* fmov.s @(r0,r12),fr2 */
L8c616274: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c616276; /* fsub fr5,fr3 */
L8c616276: r[0]=16u;goto L8c616278; /* mov #0x10,r0 */
L8c616278: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61627a; /* fsub fr14,fr2 */
L8c61627a: fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c61627c; /* fmov.s @(r0,r12),fr1 */
L8c61627c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61627e; /* fsub fr13,fr1 */
L8c61627e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616280; /* fmul fr2,fr3 */
L8c616280: fcopy(f,2u,14u);goto L8c616282; /* fmov fr14,fr2 */
L8c616282: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[6],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616284; /* fsub fr6,fr2 */
L8c616284: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616286; /* fmul fr1,fr2 */
L8c616286: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c616288; /* fsub fr2,fr3 */
L8c616288: fcopy(f,4u,3u);goto L8c61628a; /* fmov fr3,fr4 */
L8c61628a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61628c; /* fmul fr15,fr4 */
L8c61628c: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c61628e; /* cmp/hs r8,r9 */
L8c61628e: if((*sr&1u))goto L8c6162d8;goto L8c616290; /* bt 0x8c6162d8 */
L8c616290: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c616292; /* fldi0 fr3 */
L8c616292: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c616294; /* fcmp/gt fr4,fr3 */
L8c616294: if((*sr&1u))goto L8c61631e;goto L8c616296; /* bt 0x8c61631e */
L8c616296: r[5]=rd(m,bus,&io_status,0x8c6164acu,4);if(io_status)return io_status;goto L8c616298; /* mov.l 0x8c6164ac,r5 */
L8c616298: r[6]=r[14];goto L8c61629a; /* mov r14,r6 */
L8c61629a: r[4]=r[11];goto L8c61629c; /* mov r11,r4 */
L8c61629c: r[14]+=4u;goto L8c61629e; /* add #0x4,r14 */
L8c61629e: r[4]+=16u;goto L8c6162a0; /* add #0x10,r4 */
L8c6162a0: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6162a2; /* fschg */
L8c6162a2: r[8]+=16u;goto L8c6162a4; /* add #0x10,r8 */
L8c6162a4: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6162a6; /* fmov.s @r4+,fr4 */
L8c6162a6: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6162a8; /* fmov.s @r4,fr6 */
L8c6162a8: fcopy(f,0u,1u);goto L8c6162aa; /* fmov fr1,fr0 */
L8c6162aa: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6162ac; /* fschg */
L8c6162ac: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c6162ae; /* mov.w @r6+,r2 */
L8c6162ae: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c6162b0; /* mov.w @r6,r3 */
L8c6162b0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6162b2; /* fmov.s fr6,@-r8 */
L8c6162b2: *fpul=r[3];goto L8c6162b4; /* lds r3,FPUL */
L8c6162b4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6162b6; /* fmov.s fr5,@-r8 */
L8c6162b6: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c6162b8; /* float FPUL,fr1 */
L8c6162b8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6162ba; /* fmov.s fr4,@-r8 */
L8c6162ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6162bc; /* fmul fr0,fr1 */
L8c6162bc: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6162be; /* mov.l r5,@-r8 */
L8c6162be: *fpul=r[2];goto L8c6162c0; /* lds r2,FPUL */
L8c6162c0: r[8]+=28u;goto L8c6162c2; /* add #0x1c,r8 */
L8c6162c2: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c6162c4; /* float FPUL,fr2 */
L8c6162c4: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6162c6; /* fldi1 fr3 */
L8c6162c6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6162c8; /* fmul fr0,fr2 */
L8c6162c8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6162ca; /* fmov.s fr3,@-r8 */
L8c6162ca: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6162cc; /* fmov.s fr1,@-r8 */
L8c6162cc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6162ce; /* fmov.s fr2,@-r8 */
L8c6162ce: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6162d0; /* pref @r8 */
L8c6162d0: r[8]+=16u;goto L8c6162d2; /* add #0x10,r8 */
L8c6162d2: r[14]+=4294967292u;goto L8c6162d4; /* add #-0x4,r14 */
L8c6162d4: r[9]=r[8];goto L8c61635a; /* bra 0x8c61635a */
L8c6162d8: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6162da; /* fldi0 fr3 */
L8c6162da: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6162dc; /* fcmp/gt fr4,fr3 */
L8c6162dc: take=!(*sr&1u);r[14]+=4u;if(take)goto L8c61635a;goto L8c6162e0; /* bf/s 0x8c61635a */
L8c6162e0: r[14]+=4294967292u;goto L8c6162e2; /* add #-0x4,r14 */
L8c6162e2: r[5]=rd(m,bus,&io_status,0x8c6164b0u,4);if(io_status)return io_status;goto L8c6162e4; /* mov.l 0x8c6164b0,r5 */
L8c6162e4: r[6]=r[14];goto L8c6162e6; /* mov r14,r6 */
L8c6162e6: r[4]=r[10];goto L8c6162e8; /* mov r10,r4 */
L8c6162e8: r[14]+=4u;goto L8c6162ea; /* add #0x4,r14 */
L8c6162ea: r[4]+=16u;goto L8c6162ec; /* add #0x10,r4 */
L8c6162ec: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6162ee; /* fschg */
L8c6162ee: r[8]+=16u;goto L8c6162f0; /* add #0x10,r8 */
L8c6162f0: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6162f2; /* fmov.s @r4+,fr4 */
L8c6162f2: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6162f4; /* fmov.s @r4,fr6 */
L8c6162f4: fcopy(f,0u,1u);goto L8c6162f6; /* fmov fr1,fr0 */
L8c6162f6: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6162f8; /* fschg */
L8c6162f8: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c6162fa; /* mov.w @r6+,r2 */
L8c6162fa: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c6162fc; /* mov.w @r6,r3 */
L8c6162fc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6162fe; /* fmov.s fr6,@-r8 */
L8c6162fe: *fpul=r[3];goto L8c616300; /* lds r3,FPUL */
L8c616300: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616302; /* fmov.s fr5,@-r8 */
L8c616302: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c616304; /* float FPUL,fr1 */
L8c616304: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616306; /* fmov.s fr4,@-r8 */
L8c616306: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616308; /* fmul fr0,fr1 */
L8c616308: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61630a; /* mov.l r5,@-r8 */
L8c61630a: *fpul=r[2];goto L8c61630c; /* lds r2,FPUL */
L8c61630c: r[8]+=28u;goto L8c61630e; /* add #0x1c,r8 */
L8c61630e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c616310; /* float FPUL,fr2 */
L8c616310: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c616312; /* fldi1 fr3 */
L8c616312: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616314; /* fmul fr0,fr2 */
L8c616314: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c616316; /* fmov.s fr3,@-r8 */
L8c616316: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616318; /* fmov.s fr1,@-r8 */
L8c616318: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61631a; /* fmov.s fr2,@-r8 */
L8c61631a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61631c; /* pref @r8 */
L8c61631c: r[8]+=16u;goto L8c61631e; /* add #0x10,r8 */
L8c61631e: r[5]=rd(m,bus,&io_status,0x8c6164b0u,4);if(io_status)return io_status;goto L8c616320; /* mov.l 0x8c6164b0,r5 */
L8c616320: r[6]=r[14];goto L8c616322; /* mov r14,r6 */
L8c616322: r[4]=r[11];goto L8c616324; /* mov r11,r4 */
L8c616324: r[14]+=4u;goto L8c616326; /* add #0x4,r14 */
L8c616326: r[4]+=16u;goto L8c616328; /* add #0x10,r4 */
L8c616328: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61632a; /* fschg */
L8c61632a: r[8]+=16u;goto L8c61632c; /* add #0x10,r8 */
L8c61632c: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61632e; /* fmov.s @r4+,fr4 */
L8c61632e: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616330; /* fmov.s @r4,fr6 */
L8c616330: fcopy(f,0u,1u);goto L8c616332; /* fmov fr1,fr0 */
L8c616332: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616334; /* fschg */
L8c616334: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c616336; /* mov.w @r6+,r2 */
L8c616336: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c616338; /* mov.w @r6,r3 */
L8c616338: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c61633a; /* fmov.s fr6,@-r8 */
L8c61633a: *fpul=r[3];goto L8c61633c; /* lds r3,FPUL */
L8c61633c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61633e; /* fmov.s fr5,@-r8 */
L8c61633e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c616340; /* float FPUL,fr1 */
L8c616340: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616342; /* fmov.s fr4,@-r8 */
L8c616342: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616344; /* fmul fr0,fr1 */
L8c616344: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616346; /* mov.l r5,@-r8 */
L8c616346: *fpul=r[2];goto L8c616348; /* lds r2,FPUL */
L8c616348: r[8]+=28u;goto L8c61634a; /* add #0x1c,r8 */
L8c61634a: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c61634c; /* float FPUL,fr2 */
L8c61634c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c61634e; /* fldi1 fr3 */
L8c61634e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616350; /* fmul fr0,fr2 */
L8c616350: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c616352; /* fmov.s fr3,@-r8 */
L8c616352: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616354; /* fmov.s fr1,@-r8 */
L8c616354: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616356; /* fmov.s fr2,@-r8 */
L8c616356: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616358; /* pref @r8 */
L8c616358: r[8]+=16u;goto L8c61635a; /* add #0x10,r8 */
L8c61635a: r[2]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c61635c; /* mov.l @(0x18,r15),r2 */
L8c61635c: *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c61635e; /* tst r2,r2 */
L8c61635e: if((*sr&1u))goto L8c6163d4;goto L8c616360; /* bt 0x8c6163d4 */
L8c616360: r[4]=rd(m,bus,&io_status,0x8c6164b4u,4);if(io_status)return io_status;goto L8c616362; /* mov.l 0x8c6164b4,r4 */
L8c616362: r[3]=0u;goto L8c616364; /* mov #0x0,r3 */
L8c616364: r[14]+=4294967292u;goto L8c616366; /* add #-0x4,r14 */
L8c616366: r[9]=rd(m,bus,&io_status,0x8c6164b8u,4);if(io_status)return io_status;goto L8c616368; /* mov.l 0x8c6164b8,r9 */
L8c616368: wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c61636a; /* mov.l r3,@r4 */
L8c61636a: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c61636c; /* mov.w @r14,r3 */
L8c61636c: r[4]=r[9];goto L8c61636e; /* mov r9,r4 */
L8c61636e: *fpul=r[3];goto L8c616370; /* lds r3,FPUL */
L8c616370: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c616372; /* float FPUL,fr3 */
L8c616372: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c616374; /* ftrc fr3,FPUL */
L8c616374: r[2]=*fpul;goto L8c616376; /* sts FPUL,r2 */
L8c616376: wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c616378; /* mov.w r2,@r4 */
L8c616378: r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c61637a; /* mov.w @(0x2,r14),r0 */
L8c61637a: r[14]+=4u;goto L8c61637c; /* add #0x4,r14 */
L8c61637c: r[3]=r[0];goto L8c61637e; /* mov r0,r3 */
L8c61637e: *fpul=r[3];goto L8c616380; /* lds r3,FPUL */
L8c616380: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c616382; /* float FPUL,fr3 */
L8c616382: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c616384; /* ftrc fr3,FPUL */
L8c616384: r[0]=*fpul;goto L8c616386; /* sts FPUL,r0 */
L8c616386: wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c616388; /* mov.w r0,@(0x2,r4) */
L8c616388: r[4]+=4u;goto L8c61638a; /* add #0x4,r4 */
L8c61638a: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c61638c; /* mov.w @r14,r3 */
L8c61638c: *fpul=r[3];goto L8c61638e; /* lds r3,FPUL */
L8c61638e: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c616390; /* float FPUL,fr3 */
L8c616390: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c616392; /* ftrc fr3,FPUL */
L8c616392: r[2]=*fpul;goto L8c616394; /* sts FPUL,r2 */
L8c616394: wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c616396; /* mov.w r2,@r4 */
L8c616396: r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c616398; /* mov.w @(0x2,r14),r0 */
L8c616398: r[3]=r[0];goto L8c61639a; /* mov r0,r3 */
L8c61639a: *fpul=r[3];goto L8c61639c; /* lds r3,FPUL */
L8c61639c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61639e; /* float FPUL,fr3 */
L8c61639e: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6163a0; /* ftrc fr3,FPUL */
L8c6163a0: r[14]+=4u;goto L8c6163a2; /* add #0x4,r14 */
L8c6163a2: r[0]=*fpul;goto L8c6163a4; /* sts FPUL,r0 */
L8c6163a4: r[6]=r[12];goto L8c6163a6; /* mov r12,r6 */
L8c6163a6: r[5]=r[11];goto L8c6163a8; /* mov r11,r5 */
L8c6163a8: r[7]=r[9];goto L8c6163aa; /* mov r9,r7 */
L8c6163aa: wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6163ac; /* mov.w r0,@(0x2,r4) */
L8c6163ac: r[4]=r[9];goto L8c6163ae; /* mov r9,r4 */
L8c6163ae: r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c6163b0; /* mov.w @r14,r3 */
L8c6163b0: r[4]+=8u;goto L8c6163b2; /* add #0x8,r4 */
L8c6163b2: *fpul=r[3];goto L8c6163b4; /* lds r3,FPUL */
L8c6163b4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6163b6; /* float FPUL,fr3 */
L8c6163b6: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6163b8; /* ftrc fr3,FPUL */
L8c6163b8: r[2]=*fpul;goto L8c6163ba; /* sts FPUL,r2 */
L8c6163ba: wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c6163bc; /* mov.w r2,@r4 */
L8c6163bc: r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c6163be; /* mov.w @(0x2,r14),r0 */
L8c6163be: r[3]=r[0];goto L8c6163c0; /* mov r0,r3 */
L8c6163c0: *fpul=r[3];goto L8c6163c2; /* lds r3,FPUL */
L8c6163c2: r[3]=rd(m,bus,&io_status,0x8c6164bcu,4);if(io_status)return io_status;goto L8c6163c4; /* mov.l 0x8c6164bc,r3 */
L8c6163c4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6163c6; /* float FPUL,fr3 */
L8c6163c6: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6163c8; /* ftrc fr3,FPUL */
L8c6163c8: r[0]=*fpul;goto L8c6163ca; /* sts FPUL,r0 */
L8c6163ca: wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6163cc; /* mov.w r0,@(0x2,r4) */
L8c6163cc: target=r[3];*pr=0x8c6163d0u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6163d0; /* jsr @r3 */
L8c6163d0: r[9]=r[8];goto L8c6163d2; /* mov r8,r9 */
L8c6163d2: r[14]+=4294967292u;goto L8c6163d4; /* add #-0x4,r14 */
L8c6163d4: fcopy(f,5u,13u);goto L8c6163d6; /* fmov fr13,fr5 */
L8c6163d6: r[0]=16u;goto L8c6163d8; /* mov #0x10,r0 */
L8c6163d8: r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6163da; /* mov.l @(0x8,r15),r3 */
L8c6163da: r[10]=r[11];goto L8c6163dc; /* mov r11,r10 */
L8c6163dc: fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c6163de; /* fmov.s @(r0,r12),fr13 */
L8c6163de: r[0]=20u;goto L8c6163e0; /* mov #0x14,r0 */
L8c6163e0: fcopy(f,6u,14u);goto L8c6163e2; /* fmov fr14,fr6 */
L8c6163e2: r[3]+=4294967295u;goto L8c6163e4; /* add #-0x1,r3 */
L8c6163e4: fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c6163e6; /* fmov.s @(r0,r12),fr14 */
L8c6163e6: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6163e8; /* tst r3,r3 */
L8c6163e8: r[11]=r[12];goto L8c6163ea; /* mov r12,r11 */
L8c6163ea: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c6163ec; /* fneg fr15 */
L8c6163ec: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c6163f4;goto L8c6163f0; /* bt/s 0x8c6163f4 */
L8c6163f0: goto L8c6160fa; /* bra 0x8c6160fa */
L8c6163f4: *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c6163f6; /* cmp/hs r8,r9 */
L8c6163f6: take=(*sr&1u);r[14]+=8u;if(take)goto L8c616438;goto L8c6163fa; /* bt/s 0x8c616438 */
L8c6163fa: r[14]+=4294967288u;goto L8c6163fc; /* add #-0x8,r14 */
L8c6163fc: r[5]=rd(m,bus,&io_status,0x8c6164acu,4);if(io_status)return io_status;goto L8c6163fe; /* mov.l 0x8c6164ac,r5 */
L8c6163fe: r[6]=r[14];goto L8c616400; /* mov r14,r6 */
L8c616400: r[4]=r[12];goto L8c616402; /* mov r12,r4 */
L8c616402: r[14]+=4u;goto L8c616404; /* add #0x4,r14 */
L8c616404: r[4]+=16u;goto L8c616406; /* add #0x10,r4 */
L8c616406: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616408; /* fschg */
L8c616408: r[8]+=16u;goto L8c61640a; /* add #0x10,r8 */
L8c61640a: fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61640c; /* fmov.s @r4+,fr4 */
L8c61640c: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61640e; /* fmov.s @r4,fr6 */
L8c61640e: fcopy(f,0u,1u);goto L8c616410; /* fmov fr1,fr0 */
L8c616410: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616412; /* fschg */
L8c616412: r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c616414; /* mov.w @r6+,r2 */
L8c616414: r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c616416; /* mov.w @r6,r3 */
L8c616416: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616418; /* fmov.s fr6,@-r8 */
L8c616418: *fpul=r[3];goto L8c61641a; /* lds r3,FPUL */
L8c61641a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61641c; /* fmov.s fr5,@-r8 */
L8c61641c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61641e; /* float FPUL,fr1 */
L8c61641e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616420; /* fmov.s fr4,@-r8 */
L8c616420: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616422; /* fmul fr0,fr1 */
L8c616422: r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616424; /* mov.l r5,@-r8 */
L8c616424: *fpul=r[2];goto L8c616426; /* lds r2,FPUL */
L8c616426: r[8]+=28u;goto L8c616428; /* add #0x1c,r8 */
L8c616428: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c61642a; /* float FPUL,fr2 */
L8c61642a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c61642c; /* fldi1 fr3 */
L8c61642c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61642e; /* fmul fr0,fr2 */
L8c61642e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c616430; /* fmov.s fr3,@-r8 */
L8c616430: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616432; /* fmov.s fr1,@-r8 */
L8c616432: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616434; /* fmov.s fr2,@-r8 */
L8c616434: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616436; /* pref @r8 */
L8c616436: r[8]+=16u;goto L8c616438; /* add #0x10,r8 */
L8c616438: r[3]=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;goto L8c61643a; /* mov.l @(0x1c,r15),r3 */
L8c61643a: r[3]+=4294967295u;goto L8c61643c; /* add #-0x1,r3 */
L8c61643c: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c61643e; /* tst r3,r3 */
L8c61643e: take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x1cu,r[3],4);if(io_status)return io_status;if(take)goto L8c616446;goto L8c616442; /* bt/s 0x8c616446 */
L8c616442: goto L8c6160a8; /* bra 0x8c6160a8 */
L8c616446: r[4]=rd(m,bus,&io_status,0x8c6164c0u,4);if(io_status)return io_status;goto L8c616448; /* mov.l 0x8c6164c0,r4 */
L8c616448: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61644a; /* mov.l @(0x10,r15),r3 */
L8c61644a: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61644c; /* mov.l @r4,r2 */
L8c61644c: r[2]+=r[3];goto L8c61644e; /* add r3,r2 */
L8c61644e: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616450; /* mov.l r2,@r4 */
L8c616450: r[4]=rd(m,bus,&io_status,0x8c6164c4u,4);if(io_status)return io_status;goto L8c616452; /* mov.l 0x8c6164c4,r4 */
L8c616452: r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c616454; /* mov.l @(0x10,r15),r3 */
L8c616454: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616456; /* mov.l @r4,r2 */
L8c616456: r[2]+=r[3];goto L8c616458; /* add r3,r2 */
L8c616458: wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c61645a; /* mov.l r2,@r4 */
L8c61645a: r[15]+=80u;goto L8c61645c; /* add #0x50,r15 */
L8c61645c: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61645e; /* lds.l @r15+,PR */
L8c61645e: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616460; /* fmov.s @r15+,fr12 */
L8c616460: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616462; /* fmov.s @r15+,fr13 */
L8c616462: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616464; /* fmov.s @r15+,fr14 */
L8c616464: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616466; /* fmov.s @r15+,fr15 */
L8c616466: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616468; /* mov.l @r15+,r9 */
L8c616468: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61646a; /* mov.l @r15+,r10 */
L8c61646a: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61646c; /* mov.l @r15+,r11 */
L8c61646c: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61646e; /* mov.l @r15+,r12 */
L8c61646e: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616470; /* mov.l @r15+,r13 */
L8c616470: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
