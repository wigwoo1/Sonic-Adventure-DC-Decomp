/* Vertex workers with portable MMIO/store-queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_vertex_project_normals_8c611060(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sr;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c611060;
L8c611060: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c611062; /* fmov.s fr15,@-r15 */
L8c611062: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c611064; /* fmov.s fr14,@-r15 */
L8c611064: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c611066; /* fmov.s fr13,@-r15 */
L8c611066: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c611068; /* fmov.s fr12,@-r15 */
L8c611068: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61106a; /* mov.l r11,@-r15 */
L8c61106a: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61106c; /* mov.l r10,@-r15 */
L8c61106c: r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c61106e; /* mov.l r8,@-r15 */
L8c61106e: r[0]=r[4];goto L8c611070; /* mov r4,r0 */
L8c611070: r[8]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]+=4;goto L8c611072; /* mov.l @r0+,r8 */
L8c611072: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]+=4;goto L8c611074; /* mov.l @r0+,r2 */
L8c611074: r[4]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]+=4;goto L8c611076; /* mov.l @r0+,r4 */
L8c611076: r[5]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]+=4;goto L8c611078; /* mov.l @r0+,r5 */
L8c611078: r[0]+=8u;goto L8c61107a; /* add #0x8,r0 */
L8c61107a: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61107c; /* fmov.s @r0+,fr6 */
L8c61107c: fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61107e; /* fmov.s @r0+,fr7 */
L8c61107e: r[0]=rd(m,bus,&io_status,0x8c6110b8u,4);if(io_status)return io_status;goto L8c611080; /* mov.l 0x8c6110b8,r0 */
L8c611080: fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611082; /* fmov.s @r0+,fr14 */
L8c611082: fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611084; /* fmov.s @r0+,fr15 */
L8c611084: r[10]=r[2];goto L8c611086; /* mov r2,r10 */
L8c611086: r[0]=rd(m,bus,&io_status,0x8c6110bcu,4);if(io_status)return io_status;goto L8c611088; /* mov.l 0x8c6110bc,r0 */
L8c611088: r[1]=rd(m,bus,&io_status,0x8c6110c0u,4);if(io_status)return io_status;goto L8c61108a; /* mov.l 0x8c6110c0,r1 */
L8c61108a: r[0]&=r[2];goto L8c61108c; /* and r2,r0 */
L8c61108c: r[0]>>=16;goto L8c61108e; /* shlr16 r0 */
L8c61108e: r[0]>>=8;goto L8c611090; /* shlr8 r0 */
L8c611090: wr(m,bus,&io_status,r[1]+0x0u,r[0],4);if(io_status)return io_status;goto L8c611092; /* mov.l r0,@(0x0,r1) */
L8c611092: wr(m,bus,&io_status,r[1]+0x4u,r[0],4);if(io_status)return io_status;goto L8c611094; /* mov.l r0,@(0x4,r1) */
L8c611094: r[0]=rd(m,bus,&io_status,0x8c6110c4u,4);if(io_status)return io_status;goto L8c611096; /* mov.l 0x8c6110c4,r0 */
L8c611096: r[1]=rd(m,bus,&io_status,0x8c6110c8u,4);if(io_status)return io_status;goto L8c611098; /* mov.l 0x8c6110c8,r1 */
L8c611098: r[2]&=r[0];goto L8c61109a; /* and r0,r2 */
L8c61109a: r[2]|=r[1];goto L8c61109c; /* or r1,r2 */
L8c61109c: r[3]=r[2];goto L8c61109e; /* mov r2,r3 */
L8c61109e: r[11]=r[10];goto L8c6110a0; /* mov r10,r11 */
L8c6110a0: r[3]+=32u;goto L8c6110a2; /* add #0x20,r3 */
L8c6110a2: r[11]+=32u;goto L8c6110a4; /* add #0x20,r11 */
L8c6110a4: *sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c6110a6; /* tst r8,r8 */
L8c6110a6: if((*sr&1u))goto L8c61110e;goto L8c6110a8; /* bt 0x8c61110e */
L8c6110a8: r[1]=0u;goto L8c6110aa; /* mov #0x0,r1 */
L8c6110aa: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6110ac; /* fldi1 fr3 */
L8c6110ac: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6110ae; /* fmov.s @r4+,fr0 */
L8c6110ae: fload(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6110b0; /* fmov.s @r4+,fr1 */
L8c6110b0: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6110b2; /* fmov.s @r4+,fr2 */
L8c6110b2: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c611102; /* bra 0x8c611102 */
L8c6110cc: fload(m,bus,&io_status,f,10u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6110ce; /* fmov.s @r5+,fr10 */
L8c6110ce: fload(m,bus,&io_status,f,11u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6110d0; /* fmov.s @r5+,fr11 */
L8c6110d0: fload(m,bus,&io_status,f,12u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6110d2; /* fmov.s @r5+,fr12 */
L8c6110d2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6110d4; /* fmul fr2,fr1 */
L8c6110d4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[2],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6110d6; /* fmul fr2,fr0 */
L8c6110d6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[10],0u);if(status)return status;goto L8c6110d8; /* ocbi @r10 */
L8c6110d8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[15],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c6110da; /* fadd fr15,fr1 */
L8c6110da: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[14],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c6110dc; /* fadd fr14,fr0 */
L8c6110dc: r[2]+=28u;goto L8c6110de; /* add #0x1c,r2 */
L8c6110de: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c6110e0; /* fmov.s fr2,@-r2 */
L8c6110e0: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c6110e2; /* fmov.s fr1,@-r2 */
L8c6110e2: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[2]);if(io_status)return io_status;goto L8c6110e4; /* fmov.s fr0,@-r2 */
L8c6110e4: r[2]-=4;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c6110e6; /* mov.l r1,@-r2 */
L8c6110e6: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[2]);if(io_status)return io_status;goto L8c6110e8; /* fmov.s fr12,@-r2 */
L8c6110e8: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[2]);if(io_status)return io_status;goto L8c6110ea; /* fmov.s fr11,@-r2 */
L8c6110ea: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[2]);if(io_status)return io_status;goto L8c6110ec; /* fmov.s fr10,@-r2 */
L8c6110ec: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6110ee; /* fldi1 fr3 */
L8c6110ee: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6110f0; /* fmov.s @r4+,fr0 */
L8c6110f0: fload(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6110f2; /* fmov.s @r4+,fr1 */
L8c6110f2: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6110f4; /* fmov.s @r4+,fr2 */
L8c6110f4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c6110f6; /* pref @r2 */
L8c6110f6: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6110f8; /* ftrv xmtrx,fv0 */
L8c6110f8: r[8]--;*sr=(*sr&~1u)|(!r[8]);goto L8c6110fa; /* dt r8 */
L8c6110fa: r[2]+=32u;goto L8c6110fc; /* add #0x20,r2 */
L8c6110fc: take=(*sr&1u);r[10]+=32u;if(take)goto L8c61110e;goto L8c611100; /* bt/s 0x8c61110e */
L8c611100: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[5],1u);if(status)return status;goto L8c611102; /* pref @r5 */
L8c611102: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c611104; /* fmul fr2,fr2 */
L8c611104: status=sa_sh4_vector_numeric(0xf27du,f,*fpul);if(status)return status;goto L8c611106; /* fsrra fr2 */
L8c611106: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c611108; /* fmul fr7,fr1 */
L8c611108: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61110a; /* fmul fr6,fr0 */
L8c61110a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c6110cc; /* bra 0x8c6110cc */
L8c61110e: r[8]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c611110; /* mov.l @r15+,r8 */
L8c611110: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c611112; /* mov.l @r15+,r10 */
L8c611112: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c611114; /* mov.l @r15+,r11 */
L8c611114: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611116; /* fmov.s @r15+,fr12 */
L8c611116: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611118; /* fmov.s @r15+,fr13 */
L8c611118: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61111a; /* fmov.s @r15+,fr14 */
L8c61111a: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_vertex_project_8c611122(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sr;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c611122;
L8c611122: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c611124; /* fmov.s fr15,@-r15 */
L8c611124: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c611126; /* fmov.s fr14,@-r15 */
L8c611126: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c611128; /* fmov.s fr13,@-r15 */
L8c611128: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61112a; /* fmov.s fr12,@-r15 */
L8c61112a: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61112c; /* mov.l r11,@-r15 */
L8c61112c: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61112e; /* mov.l r10,@-r15 */
L8c61112e: r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c611130; /* mov.l r8,@-r15 */
L8c611130: r[0]=r[4];goto L8c611132; /* mov r4,r0 */
L8c611132: r[8]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]+=4;goto L8c611134; /* mov.l @r0+,r8 */
L8c611134: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]+=4;goto L8c611136; /* mov.l @r0+,r2 */
L8c611136: r[4]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]+=4;goto L8c611138; /* mov.l @r0+,r4 */
L8c611138: r[5]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;r[0]+=4;goto L8c61113a; /* mov.l @r0+,r5 */
L8c61113a: r[0]+=8u;goto L8c61113c; /* add #0x8,r0 */
L8c61113c: fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61113e; /* fmov.s @r0+,fr6 */
L8c61113e: fload(m,bus,&io_status,f,7u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611140; /* fmov.s @r0+,fr7 */
L8c611140: r[0]=rd(m,bus,&io_status,0x8c611178u,4);if(io_status)return io_status;goto L8c611142; /* mov.l 0x8c611178,r0 */
L8c611142: fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611144; /* fmov.s @r0+,fr14 */
L8c611144: fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611146; /* fmov.s @r0+,fr15 */
L8c611146: r[10]=r[2];goto L8c611148; /* mov r2,r10 */
L8c611148: r[0]=rd(m,bus,&io_status,0x8c61117cu,4);if(io_status)return io_status;goto L8c61114a; /* mov.l 0x8c61117c,r0 */
L8c61114a: r[1]=rd(m,bus,&io_status,0x8c611180u,4);if(io_status)return io_status;goto L8c61114c; /* mov.l 0x8c611180,r1 */
L8c61114c: r[0]&=r[2];goto L8c61114e; /* and r2,r0 */
L8c61114e: r[0]>>=16;goto L8c611150; /* shlr16 r0 */
L8c611150: r[0]>>=8;goto L8c611152; /* shlr8 r0 */
L8c611152: wr(m,bus,&io_status,r[1]+0x0u,r[0],4);if(io_status)return io_status;goto L8c611154; /* mov.l r0,@(0x0,r1) */
L8c611154: wr(m,bus,&io_status,r[1]+0x4u,r[0],4);if(io_status)return io_status;goto L8c611156; /* mov.l r0,@(0x4,r1) */
L8c611156: r[0]=rd(m,bus,&io_status,0x8c611184u,4);if(io_status)return io_status;goto L8c611158; /* mov.l 0x8c611184,r0 */
L8c611158: r[1]=rd(m,bus,&io_status,0x8c611188u,4);if(io_status)return io_status;goto L8c61115a; /* mov.l 0x8c611188,r1 */
L8c61115a: r[2]&=r[0];goto L8c61115c; /* and r0,r2 */
L8c61115c: r[2]|=r[1];goto L8c61115e; /* or r1,r2 */
L8c61115e: r[3]=r[2];goto L8c611160; /* mov r2,r3 */
L8c611160: r[11]=r[10];goto L8c611162; /* mov r10,r11 */
L8c611162: r[3]+=32u;goto L8c611164; /* add #0x20,r3 */
L8c611164: r[11]+=32u;goto L8c611166; /* add #0x20,r11 */
L8c611166: *sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c611168; /* tst r8,r8 */
L8c611168: if((*sr&1u))goto L8c6111c2;goto L8c61116a; /* bt 0x8c6111c2 */
L8c61116a: r[1]=0u;goto L8c61116c; /* mov #0x0,r1 */
L8c61116c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c61116e; /* fldi1 fr3 */
L8c61116e: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611170; /* fmov.s @r4+,fr0 */
L8c611170: fload(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611172; /* fmov.s @r4+,fr1 */
L8c611172: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611174; /* fmov.s @r4+,fr2 */
L8c611174: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6111b6; /* bra 0x8c6111b6 */
L8c61118c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61118e; /* fmul fr2,fr1 */
L8c61118e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[2],&f->fpscr,&fr[0]);if(status)return status;}goto L8c611190; /* fmul fr2,fr0 */
L8c611190: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[10],0u);if(status)return status;goto L8c611192; /* ocbi @r10 */
L8c611192: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[15],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c611194; /* fadd fr15,fr1 */
L8c611194: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[14],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c611196; /* fadd fr14,fr0 */
L8c611196: r[2]+=28u;goto L8c611198; /* add #0x1c,r2 */
L8c611198: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c61119a; /* fmov.s fr2,@-r2 */
L8c61119a: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c61119c; /* fmov.s fr1,@-r2 */
L8c61119c: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[2]);if(io_status)return io_status;goto L8c61119e; /* fmov.s fr0,@-r2 */
L8c61119e: r[2]-=4;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c6111a0; /* mov.l r1,@-r2 */
L8c6111a0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6111a2; /* fldi1 fr3 */
L8c6111a2: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6111a4; /* fmov.s @r4+,fr0 */
L8c6111a4: fload(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6111a6; /* fmov.s @r4+,fr1 */
L8c6111a6: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6111a8; /* fmov.s @r4+,fr2 */
L8c6111a8: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c6111aa; /* pref @r2 */
L8c6111aa: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6111ac; /* ftrv xmtrx,fv0 */
L8c6111ac: r[8]--;*sr=(*sr&~1u)|(!r[8]);goto L8c6111ae; /* dt r8 */
L8c6111ae: r[2]+=20u;goto L8c6111b0; /* add #0x14,r2 */
L8c6111b0: take=(*sr&1u);r[10]+=32u;if(take)goto L8c6111c2;goto L8c6111b4; /* bt/s 0x8c6111c2 */
L8c6111b4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[5],1u);if(status)return status;goto L8c6111b6; /* pref @r5 */
L8c6111b6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6111b8; /* fmul fr2,fr2 */
L8c6111b8: status=sa_sh4_vector_numeric(0xf27du,f,*fpul);if(status)return status;goto L8c6111ba; /* fsrra fr2 */
L8c6111ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6111bc; /* fmul fr7,fr1 */
L8c6111bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6111be; /* fmul fr6,fr0 */
L8c6111be: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c61118c; /* bra 0x8c61118c */
L8c6111c2: r[8]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6111c4; /* mov.l @r15+,r8 */
L8c6111c4: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6111c6; /* mov.l @r15+,r10 */
L8c6111c6: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6111c8; /* mov.l @r15+,r11 */
L8c6111c8: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6111ca; /* fmov.s @r15+,fr12 */
L8c6111ca: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6111cc; /* fmov.s @r15+,fr13 */
L8c6111cc: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6111ce; /* fmov.s @r15+,fr14 */
L8c6111ce: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_vertex_classify_normals_8c6111d6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sr;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6111d6;
L8c6111d6: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c6111d8; /* fmov.s fr15,@-r15 */
L8c6111d8: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c6111da; /* fmov.s fr14,@-r15 */
L8c6111da: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c6111dc; /* fmov.s fr13,@-r15 */
L8c6111dc: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c6111de; /* fmov.s fr12,@-r15 */
L8c6111de: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6111e0; /* mov.l r11,@-r15 */
L8c6111e0: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6111e2; /* mov.l r10,@-r15 */
L8c6111e2: r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6111e4; /* mov.l r8,@-r15 */
L8c6111e4: r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c6111e6; /* mov.l r4,@-r15 */
L8c6111e6: r[15]+=4294967280u;goto L8c6111e8; /* add #-0x10,r15 */
L8c6111e8: r[8]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]+=4;goto L8c6111ea; /* mov.l @r4+,r8 */
L8c6111ea: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]+=4;goto L8c6111ec; /* mov.l @r4+,r2 */
L8c6111ec: r[0]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]+=4;goto L8c6111ee; /* mov.l @r4+,r0 */
L8c6111ee: r[5]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]+=4;goto L8c6111f0; /* mov.l @r4+,r5 */
L8c6111f0: r[6]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]+=4;goto L8c6111f2; /* mov.l @r4+,r6 */
L8c6111f2: r[7]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]+=4;goto L8c6111f4; /* mov.l @r4+,r7 */
L8c6111f4: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6111f6; /* fmov.s @r4+,fr6 */
L8c6111f6: fload(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6111f8; /* fmov.s @r4+,fr7 */
L8c6111f8: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6111fa; /* fmov.s @r4+,fr10 */
L8c6111fa: fload(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6111fc; /* fmov.s @r4+,fr11 */
L8c6111fc: fload(m,bus,&io_status,f,12u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6111fe; /* fmov.s @r4+,fr12 */
L8c6111fe: fload(m,bus,&io_status,f,13u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611200; /* fmov.s @r4+,fr13 */
L8c611200: r[4]=rd(m,bus,&io_status,0x8c611240u,4);if(io_status)return io_status;goto L8c611202; /* mov.l 0x8c611240,r4 */
L8c611202: fload(m,bus,&io_status,f,14u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611204; /* fmov.s @r4+,fr14 */
L8c611204: fload(m,bus,&io_status,f,15u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611206; /* fmov.s @r4+,fr15 */
L8c611206: r[4]=rd(m,bus,&io_status,0x8c611244u,4);if(io_status)return io_status;goto L8c611208; /* mov.l 0x8c611244,r4 */
L8c611208: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;goto L8c61120a; /* fmov.s @r4,fr8 */
L8c61120a: r[4]=rd(m,bus,&io_status,0x8c611248u,4);if(io_status)return io_status;goto L8c61120c; /* mov.l 0x8c611248,r4 */
L8c61120c: fload(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c61120e; /* fmov.s @r4,fr9 */
L8c61120e: r[4]=r[0];goto L8c611210; /* mov r0,r4 */
L8c611210: r[10]=r[2];goto L8c611212; /* mov r2,r10 */
L8c611212: r[0]=rd(m,bus,&io_status,0x8c61124cu,4);if(io_status)return io_status;goto L8c611214; /* mov.l 0x8c61124c,r0 */
L8c611214: r[1]=rd(m,bus,&io_status,0x8c611250u,4);if(io_status)return io_status;goto L8c611216; /* mov.l 0x8c611250,r1 */
L8c611216: r[0]&=r[2];goto L8c611218; /* and r2,r0 */
L8c611218: r[0]>>=16;goto L8c61121a; /* shlr16 r0 */
L8c61121a: r[0]>>=8;goto L8c61121c; /* shlr8 r0 */
L8c61121c: wr(m,bus,&io_status,r[1]+0x0u,r[0],4);if(io_status)return io_status;goto L8c61121e; /* mov.l r0,@(0x0,r1) */
L8c61121e: wr(m,bus,&io_status,r[1]+0x4u,r[0],4);if(io_status)return io_status;goto L8c611220; /* mov.l r0,@(0x4,r1) */
L8c611220: r[0]=rd(m,bus,&io_status,0x8c611254u,4);if(io_status)return io_status;goto L8c611222; /* mov.l 0x8c611254,r0 */
L8c611222: r[1]=rd(m,bus,&io_status,0x8c611258u,4);if(io_status)return io_status;goto L8c611224; /* mov.l 0x8c611258,r1 */
L8c611224: r[2]&=r[0];goto L8c611226; /* and r0,r2 */
L8c611226: r[2]|=r[1];goto L8c611228; /* or r1,r2 */
L8c611228: r[3]=r[2];goto L8c61122a; /* mov r2,r3 */
L8c61122a: r[11]=r[10];goto L8c61122c; /* mov r10,r11 */
L8c61122c: r[3]+=32u;goto L8c61122e; /* add #0x20,r3 */
L8c61122e: r[11]+=32u;goto L8c611230; /* add #0x20,r11 */
L8c611230: *sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c611232; /* tst r8,r8 */
L8c611232: if((*sr&1u))goto L8c6112c6;goto L8c611234; /* bt 0x8c6112c6 */
L8c611234: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c611236; /* fldi1 fr3 */
L8c611236: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611238; /* fmov.s @r4+,fr0 */
L8c611238: fload(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61123a; /* fmov.s @r4+,fr1 */
L8c61123a: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61123c; /* fmov.s @r4+,fr2 */
L8c61123c: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6112b0; /* bra 0x8c6112b0 */
L8c61125c: fcopy(f,3u,4u);goto L8c61125e; /* fmov fr4,fr3 */
L8c61125e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c611260; /* fmul fr4,fr1 */
L8c611260: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c611262; /* fmul fr4,fr0 */
L8c611262: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[15],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c611264; /* fadd fr15,fr1 */
L8c611264: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[14],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c611266; /* fadd fr14,fr0 */
L8c611266: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][13],&f->fpscr,sr,1);if(status)return status;goto L8c611268; /* fcmp/gt fr13,fr1 */
L8c611268: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c61126a; /* rotcl r1 */
L8c61126a: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c61126c; /* fcmp/gt fr1,fr12 */
L8c61126c: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c61126e; /* rotcl r1 */
L8c61126e: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,sr,1);if(status)return status;goto L8c611270; /* fcmp/gt fr11,fr0 */
L8c611270: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c611272; /* rotcl r1 */
L8c611272: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][10],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c611274; /* fcmp/gt fr0,fr10 */
L8c611274: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c611276; /* rotcl r1 */
L8c611276: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[10],0u);if(status)return status;goto L8c611278; /* ocbi @r10 */
L8c611278: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][9],&f->fpscr,sr,1);if(status)return status;goto L8c61127a; /* fcmp/gt fr9,fr2 */
L8c61127a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c61127c; /* rotcl r1 */
L8c61127c: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][8],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c61127e; /* fcmp/gt fr2,fr8 */
L8c61127e: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c611280; /* rotcl r1 */
L8c611280: r[2]+=28u;goto L8c611282; /* add #0x1c,r2 */
L8c611282: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c611284; /* fmov.s fr3,@-r2 */
L8c611284: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c611286; /* fmov.s fr1,@-r2 */
L8c611286: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[2]);if(io_status)return io_status;goto L8c611288; /* fmov.s fr0,@-r2 */
L8c611288: fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61128a; /* fmov.s @r5+,fr2 */
L8c61128a: fload(m,bus,&io_status,f,1u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61128c; /* fmov.s @r5+,fr1 */
L8c61128c: fload(m,bus,&io_status,f,0u,r[5]);if(io_status)return io_status;r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61128e; /* fmov.s @r5+,fr0 */
L8c61128e: r[2]-=4;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c611290; /* mov.l r1,@-r2 */
L8c611290: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[2]);if(io_status)return io_status;goto L8c611292; /* fmov.s fr0,@-r2 */
L8c611292: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c611294; /* fmov.s fr1,@-r2 */
L8c611294: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[2]);if(io_status)return io_status;goto L8c611296; /* fmov.s fr2,@-r2 */
L8c611296: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c611298; /* fldi1 fr3 */
L8c611298: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61129a; /* fmov.s @r4+,fr0 */
L8c61129a: fload(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61129c; /* fmov.s @r4+,fr1 */
L8c61129c: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61129e; /* fmov.s @r4+,fr2 */
L8c61129e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c6112a0; /* pref @r2 */
L8c6112a0: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6112a2; /* ftrv xmtrx,fv0 */
L8c6112a2: r[6]&=r[1];goto L8c6112a4; /* and r1,r6 */
L8c6112a4: r[7]|=r[1];goto L8c6112a6; /* or r1,r7 */
L8c6112a6: r[8]--;*sr=(*sr&~1u)|(!r[8]);goto L8c6112a8; /* dt r8 */
L8c6112a8: r[2]+=32u;goto L8c6112aa; /* add #0x20,r2 */
L8c6112aa: take=(*sr&1u);r[10]+=32u;if(take)goto L8c6112c6;goto L8c6112ae; /* bt/s 0x8c6112c6 */
L8c6112ae: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[5],1u);if(status)return status;goto L8c6112b0; /* pref @r5 */
L8c6112b0: r[1]=0u;goto L8c6112b2; /* mov #0x0,r1 */
L8c6112b2: fcopy(f,4u,2u);goto L8c6112b4; /* fmov fr2,fr4 */
L8c6112b4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6112b6; /* fmul fr2,fr4 */
L8c6112b6: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][8],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c6112b8; /* fcmp/gt fr2,fr8 */
L8c6112b8: take=(*sr&1u);fcopy(f,3u,2u);if(take)goto L8c611276;goto L8c6112bc; /* bt/s 0x8c611276 */
L8c6112bc: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c6112be; /* pref @r4 */
L8c6112be: status=sa_sh4_vector_numeric(0xf47du,f,*fpul);if(status)return status;goto L8c6112c0; /* fsrra fr4 */
L8c6112c0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6112c2; /* fmul fr7,fr1 */
L8c6112c2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61125c; /* bra 0x8c61125c */
L8c6112c6: r[15]+=16u;goto L8c6112c8; /* add #0x10,r15 */
L8c6112c8: r[4]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6112ca; /* mov.l @r15+,r4 */
L8c6112ca: wr(m,bus,&io_status,r[4]+0x10u,r[6],4);if(io_status)return io_status;goto L8c6112cc; /* mov.l r6,@(0x10,r4) */
L8c6112cc: wr(m,bus,&io_status,r[4]+0x14u,r[7],4);if(io_status)return io_status;goto L8c6112ce; /* mov.l r7,@(0x14,r4) */
L8c6112ce: r[8]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6112d0; /* mov.l @r15+,r8 */
L8c6112d0: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6112d2; /* mov.l @r15+,r10 */
L8c6112d2: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6112d4; /* mov.l @r15+,r11 */
L8c6112d4: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6112d6; /* fmov.s @r15+,fr12 */
L8c6112d6: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6112d8; /* fmov.s @r15+,fr13 */
L8c6112d8: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6112da; /* fmov.s @r15+,fr14 */
L8c6112da: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_vertex_classify_8c6112e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sr;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6112e2;
L8c6112e2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c6112e4; /* fmov.s fr15,@-r15 */
L8c6112e4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c6112e6; /* fmov.s fr14,@-r15 */
L8c6112e6: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c6112e8; /* fmov.s fr13,@-r15 */
L8c6112e8: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c6112ea; /* fmov.s fr12,@-r15 */
L8c6112ea: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6112ec; /* mov.l r11,@-r15 */
L8c6112ec: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6112ee; /* mov.l r10,@-r15 */
L8c6112ee: r[15]-=4;wr(m,bus,&io_status,r[15],r[8],4);if(io_status)return io_status;goto L8c6112f0; /* mov.l r8,@-r15 */
L8c6112f0: r[15]-=4;wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c6112f2; /* mov.l r4,@-r15 */
L8c6112f2: r[15]+=4294967280u;goto L8c6112f4; /* add #-0x10,r15 */
L8c6112f4: r[8]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]+=4;goto L8c6112f6; /* mov.l @r4+,r8 */
L8c6112f6: r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]+=4;goto L8c6112f8; /* mov.l @r4+,r2 */
L8c6112f8: r[0]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]+=4;goto L8c6112fa; /* mov.l @r4+,r0 */
L8c6112fa: r[4]+=4u;goto L8c6112fc; /* add #0x4,r4 */
L8c6112fc: r[6]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]+=4;goto L8c6112fe; /* mov.l @r4+,r6 */
L8c6112fe: r[7]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;r[4]+=4;goto L8c611300; /* mov.l @r4+,r7 */
L8c611300: fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611302; /* fmov.s @r4+,fr6 */
L8c611302: fload(m,bus,&io_status,f,7u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611304; /* fmov.s @r4+,fr7 */
L8c611304: fload(m,bus,&io_status,f,10u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611306; /* fmov.s @r4+,fr10 */
L8c611306: fload(m,bus,&io_status,f,11u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611308; /* fmov.s @r4+,fr11 */
L8c611308: fload(m,bus,&io_status,f,12u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61130a; /* fmov.s @r4+,fr12 */
L8c61130a: fload(m,bus,&io_status,f,13u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61130c; /* fmov.s @r4+,fr13 */
L8c61130c: r[4]=rd(m,bus,&io_status,0x8c61134cu,4);if(io_status)return io_status;goto L8c61130e; /* mov.l 0x8c61134c,r4 */
L8c61130e: fload(m,bus,&io_status,f,14u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611310; /* fmov.s @r4+,fr14 */
L8c611310: fload(m,bus,&io_status,f,15u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611312; /* fmov.s @r4+,fr15 */
L8c611312: r[4]=rd(m,bus,&io_status,0x8c611350u,4);if(io_status)return io_status;goto L8c611314; /* mov.l 0x8c611350,r4 */
L8c611314: fload(m,bus,&io_status,f,8u,r[4]);if(io_status)return io_status;goto L8c611316; /* fmov.s @r4,fr8 */
L8c611316: r[4]=rd(m,bus,&io_status,0x8c611354u,4);if(io_status)return io_status;goto L8c611318; /* mov.l 0x8c611354,r4 */
L8c611318: fload(m,bus,&io_status,f,9u,r[4]);if(io_status)return io_status;goto L8c61131a; /* fmov.s @r4,fr9 */
L8c61131a: r[4]=r[0];goto L8c61131c; /* mov r0,r4 */
L8c61131c: r[10]=r[2];goto L8c61131e; /* mov r2,r10 */
L8c61131e: r[0]=rd(m,bus,&io_status,0x8c611358u,4);if(io_status)return io_status;goto L8c611320; /* mov.l 0x8c611358,r0 */
L8c611320: r[1]=rd(m,bus,&io_status,0x8c61135cu,4);if(io_status)return io_status;goto L8c611322; /* mov.l 0x8c61135c,r1 */
L8c611322: r[0]&=r[2];goto L8c611324; /* and r2,r0 */
L8c611324: r[0]>>=16;goto L8c611326; /* shlr16 r0 */
L8c611326: r[0]>>=8;goto L8c611328; /* shlr8 r0 */
L8c611328: wr(m,bus,&io_status,r[1]+0x0u,r[0],4);if(io_status)return io_status;goto L8c61132a; /* mov.l r0,@(0x0,r1) */
L8c61132a: wr(m,bus,&io_status,r[1]+0x4u,r[0],4);if(io_status)return io_status;goto L8c61132c; /* mov.l r0,@(0x4,r1) */
L8c61132c: r[0]=rd(m,bus,&io_status,0x8c611360u,4);if(io_status)return io_status;goto L8c61132e; /* mov.l 0x8c611360,r0 */
L8c61132e: r[1]=rd(m,bus,&io_status,0x8c611364u,4);if(io_status)return io_status;goto L8c611330; /* mov.l 0x8c611364,r1 */
L8c611330: r[2]&=r[0];goto L8c611332; /* and r0,r2 */
L8c611332: r[2]|=r[1];goto L8c611334; /* or r1,r2 */
L8c611334: r[3]=r[2];goto L8c611336; /* mov r2,r3 */
L8c611336: r[11]=r[10];goto L8c611338; /* mov r10,r11 */
L8c611338: r[3]+=32u;goto L8c61133a; /* add #0x20,r3 */
L8c61133a: r[11]+=32u;goto L8c61133c; /* add #0x20,r11 */
L8c61133c: *sr=(*sr&~1u)|!(r[8]&r[8]);goto L8c61133e; /* tst r8,r8 */
L8c61133e: if((*sr&1u))goto L8c6113c6;goto L8c611340; /* bt 0x8c6113c6 */
L8c611340: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c611342; /* fldi1 fr3 */
L8c611342: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611344; /* fmov.s @r4+,fr0 */
L8c611344: fload(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611346; /* fmov.s @r4+,fr1 */
L8c611346: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c611348; /* fmov.s @r4+,fr2 */
L8c611348: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6113b0; /* bra 0x8c6113b0 */
L8c611368: fcopy(f,3u,4u);goto L8c61136a; /* fmov fr4,fr3 */
L8c61136a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61136c; /* fmul fr4,fr1 */
L8c61136c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61136e; /* fmul fr4,fr0 */
L8c61136e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[15],&f->fpscr,&fr[1],0);if(status)return status;}goto L8c611370; /* fadd fr15,fr1 */
L8c611370: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[14],&f->fpscr,&fr[0],0);if(status)return status;}goto L8c611372; /* fadd fr14,fr0 */
L8c611372: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][1],f->bank[(f->fpscr>>21)&1u][13],&f->fpscr,sr,1);if(status)return status;goto L8c611374; /* fcmp/gt fr13,fr1 */
L8c611374: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c611376; /* rotcl r1 */
L8c611376: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][12],f->bank[(f->fpscr>>21)&1u][1],&f->fpscr,sr,1);if(status)return status;goto L8c611378; /* fcmp/gt fr1,fr12 */
L8c611378: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c61137a; /* rotcl r1 */
L8c61137a: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][0],f->bank[(f->fpscr>>21)&1u][11],&f->fpscr,sr,1);if(status)return status;goto L8c61137c; /* fcmp/gt fr11,fr0 */
L8c61137c: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c61137e; /* rotcl r1 */
L8c61137e: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][10],f->bank[(f->fpscr>>21)&1u][0],&f->fpscr,sr,1);if(status)return status;goto L8c611380; /* fcmp/gt fr0,fr10 */
L8c611380: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c611382; /* rotcl r1 */
L8c611382: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[10],0u);if(status)return status;goto L8c611384; /* ocbi @r10 */
L8c611384: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][9],&f->fpscr,sr,1);if(status)return status;goto L8c611386; /* fcmp/gt fr9,fr2 */
L8c611386: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c611388; /* rotcl r1 */
L8c611388: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][8],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c61138a; /* fcmp/gt fr2,fr8 */
L8c61138a: {uint32_t incoming=*sr&1u;*sr=(*sr&~1u)|(r[1]>>31);r[1]=(r[1]<<1)|incoming;}goto L8c61138c; /* rotcl r1 */
L8c61138c: r[2]+=28u;goto L8c61138e; /* add #0x1c,r2 */
L8c61138e: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c611390; /* fmov.s fr3,@-r2 */
L8c611390: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[2]);if(io_status)return io_status;goto L8c611392; /* fmov.s fr1,@-r2 */
L8c611392: r[2]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[2]);if(io_status)return io_status;goto L8c611394; /* fmov.s fr0,@-r2 */
L8c611394: r[2]-=4;wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c611396; /* mov.l r1,@-r2 */
L8c611396: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c611398; /* fldi1 fr3 */
L8c611398: fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61139a; /* fmov.s @r4+,fr0 */
L8c61139a: fload(m,bus,&io_status,f,1u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61139c; /* fmov.s @r4+,fr1 */
L8c61139c: fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61139e; /* fmov.s @r4+,fr2 */
L8c61139e: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c6113a0; /* pref @r2 */
L8c6113a0: status=sa_sh4_vector_numeric(0xf1fdu,f,*fpul);if(status)return status;goto L8c6113a2; /* ftrv xmtrx,fv0 */
L8c6113a2: r[6]&=r[1];goto L8c6113a4; /* and r1,r6 */
L8c6113a4: r[7]|=r[1];goto L8c6113a6; /* or r1,r7 */
L8c6113a6: r[8]--;*sr=(*sr&~1u)|(!r[8]);goto L8c6113a8; /* dt r8 */
L8c6113a8: r[2]+=20u;goto L8c6113aa; /* add #0x14,r2 */
L8c6113aa: take=(*sr&1u);r[10]+=32u;if(take)goto L8c6113c6;goto L8c6113ae; /* bt/s 0x8c6113c6 */
L8c6113ae: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[5],1u);if(status)return status;goto L8c6113b0; /* pref @r5 */
L8c6113b0: r[1]=0u;goto L8c6113b2; /* mov #0x0,r1 */
L8c6113b2: fcopy(f,4u,2u);goto L8c6113b4; /* fmov fr2,fr4 */
L8c6113b4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c6113b6; /* fmul fr2,fr4 */
L8c6113b6: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][8],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c6113b8; /* fcmp/gt fr2,fr8 */
L8c6113b8: take=(*sr&1u);fcopy(f,3u,2u);if(take)goto L8c611382;goto L8c6113bc; /* bt/s 0x8c611382 */
L8c6113bc: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[4],1u);if(status)return status;goto L8c6113be; /* pref @r4 */
L8c6113be: status=sa_sh4_vector_numeric(0xf47du,f,*fpul);if(status)return status;goto L8c6113c0; /* fsrra fr4 */
L8c6113c0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[7],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6113c2; /* fmul fr7,fr1 */
L8c6113c2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;}goto L8c611368; /* bra 0x8c611368 */
L8c6113c6: r[15]+=16u;goto L8c6113c8; /* add #0x10,r15 */
L8c6113c8: r[4]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6113ca; /* mov.l @r15+,r4 */
L8c6113ca: wr(m,bus,&io_status,r[4]+0x10u,r[6],4);if(io_status)return io_status;goto L8c6113cc; /* mov.l r6,@(0x10,r4) */
L8c6113cc: wr(m,bus,&io_status,r[4]+0x14u,r[7],4);if(io_status)return io_status;goto L8c6113ce; /* mov.l r7,@(0x14,r4) */
L8c6113ce: r[8]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6113d0; /* mov.l @r15+,r8 */
L8c6113d0: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6113d2; /* mov.l @r15+,r10 */
L8c6113d2: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6113d4; /* mov.l @r15+,r11 */
L8c6113d4: fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6113d6; /* fmov.s @r15+,fr12 */
L8c6113d6: fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6113d8; /* fmov.s @r15+,fr13 */
L8c6113d8: fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6113da; /* fmov.s @r15+,fr14 */
L8c6113da: fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;return 0; /* rts */
}
