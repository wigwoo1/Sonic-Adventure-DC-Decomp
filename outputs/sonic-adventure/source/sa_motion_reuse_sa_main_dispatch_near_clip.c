#include "sa_draw_motion_stop.h"
/* Triangle near-plane clipper with portable queue interface; name inferred.
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
SA_STAGE_EXPORT int sa_triangle_near_clip_8c613312(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c613312;
L8c613312: sa_motion_publish_stop(ctx,0x8c613312u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c613314; /* mov.l r14,@-r15 */
L8c613314: sa_motion_publish_stop(ctx,0x8c613314u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c613316; /* mov.l r13,@-r15 */
L8c613316: sa_motion_publish_stop(ctx,0x8c613316u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c613318; /* mov.l r12,@-r15 */
L8c613318: sa_motion_publish_stop(ctx,0x8c613318u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61331a; /* mov.l r11,@-r15 */
L8c61331a: sa_motion_publish_stop(ctx,0x8c61331au); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61331c; /* mov.l r10,@-r15 */
L8c61331c: sa_motion_publish_stop(ctx,0x8c61331cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61331e; /* mov.l r9,@-r15 */
L8c61331e: sa_motion_publish_stop(ctx,0x8c61331eu); r[15]+=4294967236u;goto L8c613320; /* add #-0x3c,r15 */
L8c613320: sa_motion_publish_stop(ctx,0x8c613320u); r[3]=rd(m,bus,&io_status,0x8c6133a4u,4);if(io_status)return io_status;goto L8c613322; /* mov.l 0x8c6133a4,r3 */
L8c613322: sa_motion_publish_stop(ctx,0x8c613322u); r[7]=r[15];goto L8c613324; /* mov r15,r7 */
L8c613324: sa_motion_publish_stop(ctx,0x8c613324u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]=0x3f800000u;goto L8c613326; /* fldi1 fr6 */
L8c613326: sa_motion_publish_stop(ctx,0x8c613326u); r[12]=r[15];goto L8c613328; /* mov r15,r12 */
L8c613328: sa_motion_publish_stop(ctx,0x8c613328u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61332a; /* fmov.s @r3,fr3 */
L8c61332a: sa_motion_publish_stop(ctx,0x8c61332au); r[7]+=48u;goto L8c61332c; /* add #0x30,r7 */
L8c61332c: sa_motion_publish_stop(ctx,0x8c61332cu); fcopy(f,4u,6u);goto L8c61332e; /* fmov fr6,fr4 */
L8c61332e: sa_motion_publish_stop(ctx,0x8c61332eu); r[12]+=12u;goto L8c613330; /* add #0xc,r12 */
L8c613330: sa_motion_publish_stop(ctx,0x8c613330u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c613332; /* fdiv fr3,fr4 */
L8c613332: sa_motion_publish_stop(ctx,0x8c613332u); r[2]=r[7];goto L8c613334; /* mov r7,r2 */
L8c613334: sa_motion_publish_stop(ctx,0x8c613334u); r[14]=rd(m,bus,&io_status,0x8c6133acu,4);if(io_status)return io_status;goto L8c613336; /* mov.l 0x8c6133ac,r14 */
L8c613336: sa_motion_publish_stop(ctx,0x8c613336u); r[10]=3u;goto L8c613338; /* mov #0x3,r10 */
L8c613338: sa_motion_publish_stop(ctx,0x8c613338u); r[9]=rd(m,bus,&io_status,0x8c6133a8u,4);if(io_status)return io_status;goto L8c61333a; /* mov.l 0x8c6133a8,r9 */
L8c61333a: sa_motion_publish_stop(ctx,0x8c61333au); r[11]=0u;goto L8c61333c; /* mov #0x0,r11 */
L8c61333c: sa_motion_publish_stop(ctx,0x8c61333cu); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61333e; /* mov.l r2,@r15 */
L8c61333e: sa_motion_publish_stop(ctx,0x8c61333eu); wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c613340; /* mov.l r4,@r2 */
L8c613340: sa_motion_publish_stop(ctx,0x8c613340u); r[4]=1u;goto L8c613342; /* mov #0x1,r4 */
L8c613342: sa_motion_publish_stop(ctx,0x8c613342u); wr(m,bus,&io_status,r[7]+0x4u,r[5],4);if(io_status)return io_status;goto L8c613344; /* mov.l r5,@(0x4,r7) */
L8c613344: sa_motion_publish_stop(ctx,0x8c613344u); r[5]=r[12];goto L8c613346; /* mov r12,r5 */
L8c613346: sa_motion_publish_stop(ctx,0x8c613346u); wr(m,bus,&io_status,r[7]+0x8u,r[6],4);if(io_status)return io_status;goto L8c613348; /* mov.l r6,@(0x8,r7) */
L8c613348: sa_motion_publish_stop(ctx,0x8c613348u); r[13]=r[5];goto L8c61334a; /* mov r5,r13 */
L8c61334a: sa_motion_publish_stop(ctx,0x8c61334au); r[6]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61334c; /* mov.l @r15,r6 */
L8c61334c: sa_motion_publish_stop(ctx,0x8c61334cu); wr(m,bus,&io_status,r[15],r[5],4);if(io_status)return io_status;goto L8c61334e; /* mov.l r5,@r15 */
L8c61334e: sa_motion_publish_stop(ctx,0x8c61334eu); r[0]=r[11];goto L8c613350; /* mov r11,r0 */
L8c613350: sa_motion_publish_stop(ctx,0x8c613350u); r[0]<<=2;goto L8c613352; /* shll2 r0 */
L8c613352: sa_motion_publish_stop(ctx,0x8c613352u); r[3]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c613354; /* mov.l @(r0,r7),r3 */
L8c613354: sa_motion_publish_stop(ctx,0x8c613354u); r[2]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c613356; /* mov.l @(0xc,r3),r2 */
L8c613356: sa_motion_publish_stop(ctx,0x8c613356u); *sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c613358; /* tst r4,r2 */
L8c613358: sa_motion_publish_stop(ctx,0x8c613358u); if(!(*sr&1u))goto L8c6133b4;goto L8c61335a; /* bf 0x8c6133b4 */
L8c61335a: sa_motion_publish_stop(ctx,0x8c61335au); r[1]=r[11];goto L8c61335c; /* mov r11,r1 */
L8c61335c: sa_motion_publish_stop(ctx,0x8c61335cu); r[1]<<=2;goto L8c61335e; /* shll2 r1 */
L8c61335e: sa_motion_publish_stop(ctx,0x8c61335eu); r[1]+=r[7];goto L8c613360; /* add r7,r1 */
L8c613360: sa_motion_publish_stop(ctx,0x8c613360u); fcopy(f,5u,6u);goto L8c613362; /* fmov fr6,fr5 */
L8c613362: sa_motion_publish_stop(ctx,0x8c613362u); r[0]=24u;goto L8c613364; /* mov #0x18,r0 */
L8c613364: sa_motion_publish_stop(ctx,0x8c613364u); wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c613366; /* mov.l r1,@(0x4,r15) */
L8c613366: sa_motion_publish_stop(ctx,0x8c613366u); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c613368; /* mov.l @r1,r3 */
L8c613368: sa_motion_publish_stop(ctx,0x8c613368u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c61336a; /* mov.l r3,@(0x8,r15) */
L8c61336a: sa_motion_publish_stop(ctx,0x8c61336au); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61336c; /* fmov.s @(r0,r3),fr3 */
L8c61336c: sa_motion_publish_stop(ctx,0x8c61336cu); r[0]=12u;goto L8c61336e; /* mov #0xc,r0 */
L8c61336e: sa_motion_publish_stop(ctx,0x8c61336eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[3],&f->fpscr,&fr[5]);if(status)return status;}goto L8c613370; /* fdiv fr3,fr5 */
L8c613370: sa_motion_publish_stop(ctx,0x8c613370u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613372; /* fmov.s @(r0,r14),fr3 */
L8c613372: sa_motion_publish_stop(ctx,0x8c613372u); r[0]=16u;goto L8c613374; /* mov #0x10,r0 */
L8c613374: sa_motion_publish_stop(ctx,0x8c613374u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c613376; /* fmov.s @(r0,r3),fr2 */
L8c613376: sa_motion_publish_stop(ctx,0x8c613376u); r[0]=16u;goto L8c613378; /* mov #0x10,r0 */
L8c613378: sa_motion_publish_stop(ctx,0x8c613378u); r[3]=rd(m,bus,&io_status,0x8c6133b0u,4);if(io_status)return io_status;goto L8c61337a; /* mov.l 0x8c6133b0,r3 */
L8c61337a: sa_motion_publish_stop(ctx,0x8c61337au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61337c; /* fsub fr3,fr2 */
L8c61337c: sa_motion_publish_stop(ctx,0x8c61337cu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61337e; /* fmov.s @r3,fr3 */
L8c61337e: sa_motion_publish_stop(ctx,0x8c61337eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613380; /* fmul fr5,fr2 */
L8c613380: sa_motion_publish_stop(ctx,0x8c613380u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613382; /* fdiv fr3,fr2 */
L8c613382: sa_motion_publish_stop(ctx,0x8c613382u); fstore(m,bus,&io_status,f,2u,r[13]);if(io_status)return io_status;goto L8c613384; /* fmov.s fr2,@r13 */
L8c613384: sa_motion_publish_stop(ctx,0x8c613384u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613386; /* mov.l @(0x4,r15),r2 */
L8c613386: sa_motion_publish_stop(ctx,0x8c613386u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613388; /* fmov.s @(r0,r14),fr3 */
L8c613388: sa_motion_publish_stop(ctx,0x8c613388u); r[0]=20u;goto L8c61338a; /* mov #0x14,r0 */
L8c61338a: sa_motion_publish_stop(ctx,0x8c61338au); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61338c; /* mov.l @r2,r1 */
L8c61338c: sa_motion_publish_stop(ctx,0x8c61338cu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61338e; /* fmov.s @(r0,r1),fr2 */
L8c61338e: sa_motion_publish_stop(ctx,0x8c61338eu); r[0]=4u;goto L8c613390; /* mov #0x4,r0 */
L8c613390: sa_motion_publish_stop(ctx,0x8c613390u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613392; /* fsub fr3,fr2 */
L8c613392: sa_motion_publish_stop(ctx,0x8c613392u); fload(m,bus,&io_status,f,3u,r[9]);if(io_status)return io_status;goto L8c613394; /* fmov.s @r9,fr3 */
L8c613394: sa_motion_publish_stop(ctx,0x8c613394u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[5],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613396; /* fmul fr5,fr2 */
L8c613396: sa_motion_publish_stop(ctx,0x8c613396u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613398; /* fdiv fr3,fr2 */
L8c613398: sa_motion_publish_stop(ctx,0x8c613398u); fstore(m,bus,&io_status,f,2u,r[0]+r[13]);if(io_status)return io_status;goto L8c61339a; /* fmov fr2,@(r0,r13) */
L8c61339a: sa_motion_publish_stop(ctx,0x8c61339au); r[0]=8u;goto L8c61339c; /* mov #0x8,r0 */
L8c61339c: sa_motion_publish_stop(ctx,0x8c61339eu); fstore(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c6133d0; /* bra 0x8c6133d0 */
L8c6133b4: sa_motion_publish_stop(ctx,0x8c6133b4u); r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6133b6; /* mov.l @r6,r3 */
L8c6133b6: sa_motion_publish_stop(ctx,0x8c6133b6u); r[0]=16u;goto L8c6133b8; /* mov #0x10,r0 */
L8c6133b8: sa_motion_publish_stop(ctx,0x8c6133b8u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6133ba; /* fmov.s @(r0,r3),fr3 */
L8c6133ba: sa_motion_publish_stop(ctx,0x8c6133bau); r[0]=20u;goto L8c6133bc; /* mov #0x14,r0 */
L8c6133bc: sa_motion_publish_stop(ctx,0x8c6133bcu); fstore(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c6133be; /* fmov.s fr3,@r5 */
L8c6133be: sa_motion_publish_stop(ctx,0x8c6133beu); r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6133c0; /* mov.l @r6,r3 */
L8c6133c0: sa_motion_publish_stop(ctx,0x8c6133c0u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6133c2; /* fmov.s @(r0,r3),fr3 */
L8c6133c2: sa_motion_publish_stop(ctx,0x8c6133c2u); r[0]=4u;goto L8c6133c4; /* mov #0x4,r0 */
L8c6133c4: sa_motion_publish_stop(ctx,0x8c6133c4u); fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6133c6; /* fmov fr3,@(r0,r5) */
L8c6133c6: sa_motion_publish_stop(ctx,0x8c6133c6u); r[0]=24u;goto L8c6133c8; /* mov #0x18,r0 */
L8c6133c8: sa_motion_publish_stop(ctx,0x8c6133c8u); r[3]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6133ca; /* mov.l @r6,r3 */
L8c6133ca: sa_motion_publish_stop(ctx,0x8c6133cau); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6133cc; /* fmov.s @(r0,r3),fr3 */
L8c6133cc: sa_motion_publish_stop(ctx,0x8c6133ccu); r[0]=8u;goto L8c6133ce; /* mov #0x8,r0 */
L8c6133ce: sa_motion_publish_stop(ctx,0x8c6133ceu); fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6133d0; /* fmov fr3,@(r0,r5) */
L8c6133d0: sa_motion_publish_stop(ctx,0x8c6133d0u); r[11]+=1u;goto L8c6133d2; /* add #0x1,r11 */
L8c6133d2: sa_motion_publish_stop(ctx,0x8c6133d2u); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[10]);goto L8c6133d4; /* cmp/ge r10,r11 */
L8c6133d4: sa_motion_publish_stop(ctx,0x8c6133d4u); r[5]+=12u;goto L8c6133d6; /* add #0xc,r5 */
L8c6133d6: sa_motion_publish_stop(ctx,0x8c6133d6u); r[6]+=4u;goto L8c6133d8; /* add #0x4,r6 */
L8c6133d8: sa_motion_publish_stop(ctx,0x8c6133dau); take=!(*sr&1u);r[13]+=12u;if(take)goto L8c61334e;goto L8c6133dc; /* bf/s 0x8c61334e */
L8c6133dc: sa_motion_publish_stop(ctx,0x8c6133dcu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6133de; /* mov.l @r15,r2 */
L8c6133de: sa_motion_publish_stop(ctx,0x8c6133deu); r[0]=8u;goto L8c6133e0; /* mov #0x8,r0 */
L8c6133e0: sa_motion_publish_stop(ctx,0x8c6133e0u); r[5]=r[12];goto L8c6133e2; /* mov r12,r5 */
L8c6133e2: sa_motion_publish_stop(ctx,0x8c6133e2u); r[5]+=12u;goto L8c6133e4; /* add #0xc,r5 */
L8c6133e4: sa_motion_publish_stop(ctx,0x8c6133e4u); fload(m,bus,&io_status,f,7u,r[0]+r[2]);if(io_status)return io_status;goto L8c6133e6; /* fmov.s @(r0,r2),fr7 */
L8c6133e6: sa_motion_publish_stop(ctx,0x8c6133e6u); r[0]=4u;goto L8c6133e8; /* mov #0x4,r0 */
L8c6133e8: sa_motion_publish_stop(ctx,0x8c6133e8u); r[3]=r[2];goto L8c6133ea; /* mov r2,r3 */
L8c6133ea: sa_motion_publish_stop(ctx,0x8c6133eau); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6133ec; /* fmov.s @(r0,r5),fr5 */
L8c6133ec: sa_motion_publish_stop(ctx,0x8c6133ecu); fload(m,bus,&io_status,f,9u,r[0]+r[3]);if(io_status)return io_status;goto L8c6133ee; /* fmov.s @(r0,r3),fr9 */
L8c6133ee: sa_motion_publish_stop(ctx,0x8c6133eeu); r[0]=8u;goto L8c6133f0; /* mov #0x8,r0 */
L8c6133f0: sa_motion_publish_stop(ctx,0x8c6133f0u); fload(m,bus,&io_status,f,6u,r[0]+r[5]);if(io_status)return io_status;goto L8c6133f2; /* fmov.s @(r0,r5),fr6 */
L8c6133f2: sa_motion_publish_stop(ctx,0x8c6133f2u); r[6]=r[12];goto L8c6133f4; /* mov r12,r6 */
L8c6133f4: sa_motion_publish_stop(ctx,0x8c6133f4u); fcopy(f,3u,9u);goto L8c6133f6; /* fmov fr9,fr3 */
L8c6133f6: sa_motion_publish_stop(ctx,0x8c6133f6u); r[0]=4u;goto L8c6133f8; /* mov #0x4,r0 */
L8c6133f8: sa_motion_publish_stop(ctx,0x8c6133f8u); fcopy(f,0u,7u);goto L8c6133fa; /* fmov fr7,fr0 */
L8c6133fa: sa_motion_publish_stop(ctx,0x8c6133fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6133fc; /* fmul fr6,fr3 */
L8c6133fc: sa_motion_publish_stop(ctx,0x8c6133fcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[5],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6133fe; /* fmul fr5,fr0 */
L8c6133fe: sa_motion_publish_stop(ctx,0x8c6133feu); fload(m,bus,&io_status,f,10u,r[3]);if(io_status)return io_status;goto L8c613400; /* fmov.s @r3,fr10 */
L8c613400: sa_motion_publish_stop(ctx,0x8c613400u); fload(m,bus,&io_status,f,8u,r[5]);if(io_status)return io_status;goto L8c613402; /* fmov.s @r5,fr8 */
L8c613402: sa_motion_publish_stop(ctx,0x8c613402u); r[6]+=24u;goto L8c613404; /* add #0x18,r6 */
L8c613404: sa_motion_publish_stop(ctx,0x8c613404u); fcopy(f,2u,7u);goto L8c613406; /* fmov fr7,fr2 */
L8c613406: sa_motion_publish_stop(ctx,0x8c613406u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[8],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613408; /* fmul fr8,fr2 */
L8c613408: sa_motion_publish_stop(ctx,0x8c613408u); fcopy(f,1u,8u);goto L8c61340a; /* fmov fr8,fr1 */
L8c61340a: sa_motion_publish_stop(ctx,0x8c61340au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c61340c; /* fsub fr3,fr0 */
L8c61340c: sa_motion_publish_stop(ctx,0x8c61340cu); fcopy(f,3u,10u);goto L8c61340e; /* fmov fr10,fr3 */
L8c61340e: sa_motion_publish_stop(ctx,0x8c61340eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c613410; /* fmul fr6,fr3 */
L8c613410: sa_motion_publish_stop(ctx,0x8c613410u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[9],&f->fpscr,&fr[1]);if(status)return status;}goto L8c613412; /* fmul fr9,fr1 */
L8c613412: sa_motion_publish_stop(ctx,0x8c613412u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c613414; /* fsub fr2,fr3 */
L8c613414: sa_motion_publish_stop(ctx,0x8c613414u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c613416; /* fmov.s @(r0,r6),fr2 */
L8c613416: sa_motion_publish_stop(ctx,0x8c613416u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c613418; /* fmul fr3,fr2 */
L8c613418: sa_motion_publish_stop(ctx,0x8c613418u); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c61341a; /* fmov.s @r6,fr3 */
L8c61341a: sa_motion_publish_stop(ctx,0x8c61341au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61341c; /* fmac fr0,fr3,fr2 */
L8c61341c: sa_motion_publish_stop(ctx,0x8c61341cu); fcopy(f,11u,10u);goto L8c61341e; /* fmov fr10,fr11 */
L8c61341e: sa_motion_publish_stop(ctx,0x8c61341eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[11],&fr[5],&f->fpscr,&fr[11]);if(status)return status;}goto L8c613420; /* fmul fr5,fr11 */
L8c613420: sa_motion_publish_stop(ctx,0x8c613420u); r[0]=8u;goto L8c613422; /* mov #0x8,r0 */
L8c613422: sa_motion_publish_stop(ctx,0x8c613422u); r[5]=rd(m,bus,&io_status,0x8c6136f8u,4);if(io_status)return io_status;goto L8c613424; /* mov.l 0x8c6136f8,r5 */
L8c613424: sa_motion_publish_stop(ctx,0x8c613424u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c613426; /* fmov.s @(r0,r6),fr3 */
L8c613426: sa_motion_publish_stop(ctx,0x8c613426u); r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c613428; /* mov.l @r5,r3 */
L8c613428: sa_motion_publish_stop(ctx,0x8c613428u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[11],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61342a; /* fsub fr11,fr1 */
L8c61342a: sa_motion_publish_stop(ctx,0x8c61342au); fcopy(f,0u,1u);goto L8c61342c; /* fmov fr1,fr0 */
L8c61342c: sa_motion_publish_stop(ctx,0x8c61342cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61342e; /* fmac fr0,fr3,fr2 */
L8c61342e: sa_motion_publish_stop(ctx,0x8c61342eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c613430; /* fldi0 fr3 */
L8c613430: sa_motion_publish_stop(ctx,0x8c613430u); fcopy(f,5u,2u);goto L8c613432; /* fmov fr2,fr5 */
L8c613432: sa_motion_publish_stop(ctx,0x8c613432u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c613434; /* fcmp/gt fr5,fr3 */
L8c613434: sa_motion_publish_stop(ctx,0x8c613434u); r[0]=*sr&1u;goto L8c613436; /* movt r0 */
L8c613436: sa_motion_publish_stop(ctx,0x8c613436u); *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c613438; /* tst r3,r0 */
L8c613438: sa_motion_publish_stop(ctx,0x8c613438u); if((*sr&1u))goto L8c61343e;goto L8c61343a; /* bt 0x8c61343e */
L8c61343a: sa_motion_publish_stop(ctx,0x8c61343cu); goto L8c6136b0; /* bra 0x8c6136b0 */
L8c61343e: sa_motion_publish_stop(ctx,0x8c61343eu); r[1]=0u;goto L8c613440; /* mov #0x0,r1 */
L8c613440: sa_motion_publish_stop(ctx,0x8c613440u); wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c613442; /* mov.l r1,@r5 */
L8c613442: sa_motion_publish_stop(ctx,0x8c613442u); r[6]=r[1];goto L8c613444; /* mov r1,r6 */
L8c613444: sa_motion_publish_stop(ctx,0x8c613444u); r[5]=rd(m,bus,&io_status,0x8c6136fcu,4);if(io_status)return io_status;goto L8c613446; /* mov.l 0x8c6136fc,r5 */
L8c613446: sa_motion_publish_stop(ctx,0x8c613446u); r[13]=r[1];goto L8c613448; /* mov r1,r13 */
L8c613448: sa_motion_publish_stop(ctx,0x8c613448u); r[11]=2u;goto L8c61344a; /* mov #0x2,r11 */
L8c61344a: sa_motion_publish_stop(ctx,0x8c61344au); r[3]=r[6];goto L8c61344c; /* mov r6,r3 */
L8c61344c: sa_motion_publish_stop(ctx,0x8c61344cu); r[3]<<=2;goto L8c61344e; /* shll2 r3 */
L8c61344e: sa_motion_publish_stop(ctx,0x8c61344eu); r[0]=r[11];goto L8c613450; /* mov r11,r0 */
L8c613450: sa_motion_publish_stop(ctx,0x8c613450u); r[0]<<=2;goto L8c613452; /* shll2 r0 */
L8c613452: sa_motion_publish_stop(ctx,0x8c613452u); r[3]+=r[7];goto L8c613454; /* add r7,r3 */
L8c613454: sa_motion_publish_stop(ctx,0x8c613454u); r[0]=rd(m,bus,&io_status,r[7]+r[0],4);if(io_status)return io_status;goto L8c613456; /* mov.l @(r0,r7),r0 */
L8c613456: sa_motion_publish_stop(ctx,0x8c613456u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c613458; /* mov.l @r3,r2 */
L8c613458: sa_motion_publish_stop(ctx,0x8c613458u); r[0]=rd(m,bus,&io_status,r[0]+0xcu,4);if(io_status)return io_status;goto L8c61345a; /* mov.l @(0xc,r0),r0 */
L8c61345a: sa_motion_publish_stop(ctx,0x8c61345au); if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[1]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c61345c; /* mov.l @(0xc,r2),r1 */
L8c61345c: sa_motion_publish_stop(ctx,0x8c61345cu); r[0]&=r[4];goto L8c61345e; /* and r4,r0 */
L8c61345e: sa_motion_publish_stop(ctx,0x8c61345eu); r[1]&=r[4];goto L8c613460; /* and r4,r1 */
L8c613460: sa_motion_publish_stop(ctx,0x8c613460u); r[0]^=r[1];goto L8c613462; /* xor r1,r0 */
L8c613462: sa_motion_publish_stop(ctx,0x8c613462u); *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c613464; /* cmp/eq #0x1,r0 */
L8c613464: sa_motion_publish_stop(ctx,0x8c613464u); if(!(*sr&1u))goto L8c6134ec;goto L8c613466; /* bf 0x8c6134ec */
L8c613466: sa_motion_publish_stop(ctx,0x8c613466u); r[3]=r[11];goto L8c613468; /* mov r11,r3 */
L8c613468: sa_motion_publish_stop(ctx,0x8c613468u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61346a; /* shll r11 */
L8c61346a: sa_motion_publish_stop(ctx,0x8c61346au); r[2]=r[6];goto L8c61346c; /* mov r6,r2 */
L8c61346c: sa_motion_publish_stop(ctx,0x8c61346cu); r[11]+=r[3];goto L8c61346e; /* add r3,r11 */
L8c61346e: sa_motion_publish_stop(ctx,0x8c61346eu); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c613470; /* shll r2 */
L8c613470: sa_motion_publish_stop(ctx,0x8c613470u); r[1]=r[6];goto L8c613472; /* mov r6,r1 */
L8c613472: sa_motion_publish_stop(ctx,0x8c613472u); r[11]<<=2;goto L8c613474; /* shll2 r11 */
L8c613474: sa_motion_publish_stop(ctx,0x8c613474u); r[3]=rd(m,bus,&io_status,0x8c613700u,4);if(io_status)return io_status;goto L8c613476; /* mov.l 0x8c613700,r3 */
L8c613476: sa_motion_publish_stop(ctx,0x8c613476u); r[11]+=r[12];goto L8c613478; /* add r12,r11 */
L8c613478: sa_motion_publish_stop(ctx,0x8c613478u); r[0]=8u;goto L8c61347a; /* mov #0x8,r0 */
L8c61347a: sa_motion_publish_stop(ctx,0x8c61347au); wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61347c; /* mov.l r11,@r15 */
L8c61347c: sa_motion_publish_stop(ctx,0x8c61347cu); r[2]+=r[1];goto L8c61347e; /* add r1,r2 */
L8c61347e: sa_motion_publish_stop(ctx,0x8c61347eu); fload(m,bus,&io_status,f,5u,r[0]+r[11]);if(io_status)return io_status;goto L8c613480; /* fmov.s @(r0,r11),fr5 */
L8c613480: sa_motion_publish_stop(ctx,0x8c613480u); r[2]<<=2;goto L8c613482; /* shll2 r2 */
L8c613482: sa_motion_publish_stop(ctx,0x8c613482u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c613484; /* fmov.s @r3,fr3 */
L8c613484: sa_motion_publish_stop(ctx,0x8c613484u); r[2]+=r[12];goto L8c613486; /* add r12,r2 */
L8c613486: sa_motion_publish_stop(ctx,0x8c613486u); r[0]=8u;goto L8c613488; /* mov #0x8,r0 */
L8c613488: sa_motion_publish_stop(ctx,0x8c613488u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61348a; /* fsub fr5,fr3 */
L8c61348a: sa_motion_publish_stop(ctx,0x8c61348au); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c61348c; /* mov.l r2,@(0x4,r15) */
L8c61348c: sa_motion_publish_stop(ctx,0x8c61348cu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61348e; /* fmov.s @(r0,r2),fr2 */
L8c61348e: sa_motion_publish_stop(ctx,0x8c61348eu); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c613490; /* mov.l @(0x4,r15),r1 */
L8c613490: sa_motion_publish_stop(ctx,0x8c613490u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c613492; /* fsub fr5,fr2 */
L8c613492: sa_motion_publish_stop(ctx,0x8c613492u); fcopy(f,5u,3u);goto L8c613494; /* fmov fr3,fr5 */
L8c613494: sa_motion_publish_stop(ctx,0x8c613494u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613496; /* mov.l @r15,r2 */
L8c613496: sa_motion_publish_stop(ctx,0x8c613496u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c613498; /* fmov.s @r1,fr3 */
L8c613498: sa_motion_publish_stop(ctx,0x8c613498u); fload(m,bus,&io_status,f,6u,r[2]);if(io_status)return io_status;goto L8c61349a; /* fmov.s @r2,fr6 */
L8c61349a: sa_motion_publish_stop(ctx,0x8c61349au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[5],&fr[2],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61349c; /* fdiv fr2,fr5 */
L8c61349c: sa_motion_publish_stop(ctx,0x8c61349cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61349e; /* fsub fr6,fr3 */
L8c61349e: sa_motion_publish_stop(ctx,0x8c61349eu); fcopy(f,2u,6u);goto L8c6134a0; /* fmov fr6,fr2 */
L8c6134a0: sa_motion_publish_stop(ctx,0x8c6134a0u); fcopy(f,0u,5u);goto L8c6134a2; /* fmov fr5,fr0 */
L8c6134a2: sa_motion_publish_stop(ctx,0x8c6134a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6134a4; /* fmac fr0,fr3,fr2 */
L8c6134a4: sa_motion_publish_stop(ctx,0x8c6134a4u); fcopy(f,6u,2u);goto L8c6134a6; /* fmov fr2,fr6 */
L8c6134a6: sa_motion_publish_stop(ctx,0x8c6134a6u); r[2]=rd(m,bus,&io_status,0x8c613704u,4);if(io_status)return io_status;goto L8c6134a8; /* mov.l 0x8c613704,r2 */
L8c6134a8: sa_motion_publish_stop(ctx,0x8c6134a8u); r[0]=12u;goto L8c6134aa; /* mov #0xc,r0 */
L8c6134aa: sa_motion_publish_stop(ctx,0x8c6134aau); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c6134ac; /* fmov.s @(r0,r14),fr2 */
L8c6134ac: sa_motion_publish_stop(ctx,0x8c6134acu); r[11]=r[13];goto L8c6134ae; /* mov r13,r11 */
L8c6134ae: sa_motion_publish_stop(ctx,0x8c6134aeu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6134b0; /* fmov.s @r2,fr3 */
L8c6134b0: sa_motion_publish_stop(ctx,0x8c6134b0u); r[11]<<=2;goto L8c6134b2; /* shll2 r11 */
L8c6134b2: sa_motion_publish_stop(ctx,0x8c6134b2u); fcopy(f,0u,4u);goto L8c6134b4; /* fmov fr4,fr0 */
L8c6134b4: sa_motion_publish_stop(ctx,0x8c6134b4u); r[11]<<=2;goto L8c6134b6; /* shll2 r11 */
L8c6134b6: sa_motion_publish_stop(ctx,0x8c6134b6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6134b8; /* fmul fr6,fr3 */
L8c6134b8: sa_motion_publish_stop(ctx,0x8c6134b8u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c6134ba; /* shll r11 */
L8c6134ba: sa_motion_publish_stop(ctx,0x8c6134bau); r[0]=16u;goto L8c6134bc; /* mov #0x10,r0 */
L8c6134bc: sa_motion_publish_stop(ctx,0x8c6134bcu); r[11]+=r[5];goto L8c6134be; /* add r5,r11 */
L8c6134be: sa_motion_publish_stop(ctx,0x8c6134beu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6134c0; /* fmac fr0,fr3,fr2 */
L8c6134c0: sa_motion_publish_stop(ctx,0x8c6134c0u); fcopy(f,0u,5u);goto L8c6134c2; /* fmov fr5,fr0 */
L8c6134c2: sa_motion_publish_stop(ctx,0x8c6134c2u); fstore(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c6134c4; /* fmov fr2,@(r0,r11) */
L8c6134c4: sa_motion_publish_stop(ctx,0x8c6134c4u); r[0]=4u;goto L8c6134c6; /* mov #0x4,r0 */
L8c6134c6: sa_motion_publish_stop(ctx,0x8c6134c6u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6134c8; /* mov.l @r15,r1 */
L8c6134c8: sa_motion_publish_stop(ctx,0x8c6134c8u); fload(m,bus,&io_status,f,6u,r[0]+r[1]);if(io_status)return io_status;goto L8c6134ca; /* fmov.s @(r0,r1),fr6 */
L8c6134ca: sa_motion_publish_stop(ctx,0x8c6134cau); r[0]=4u;goto L8c6134cc; /* mov #0x4,r0 */
L8c6134cc: sa_motion_publish_stop(ctx,0x8c6134ccu); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6134ce; /* mov.l @(0x4,r15),r1 */
L8c6134ce: sa_motion_publish_stop(ctx,0x8c6134ceu); fcopy(f,5u,6u);goto L8c6134d0; /* fmov fr6,fr5 */
L8c6134d0: sa_motion_publish_stop(ctx,0x8c6134d0u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6134d2; /* fmov.s @(r0,r1),fr3 */
L8c6134d2: sa_motion_publish_stop(ctx,0x8c6134d2u); r[0]=16u;goto L8c6134d4; /* mov #0x10,r0 */
L8c6134d4: sa_motion_publish_stop(ctx,0x8c6134d4u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c6134d6; /* fmov.s @(r0,r14),fr2 */
L8c6134d6: sa_motion_publish_stop(ctx,0x8c6134d6u); r[0]=20u;goto L8c6134d8; /* mov #0x14,r0 */
L8c6134d8: sa_motion_publish_stop(ctx,0x8c6134d8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6134da; /* fsub fr6,fr3 */
L8c6134da: sa_motion_publish_stop(ctx,0x8c6134dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[5],&f->fpscr,&fr[5]);if(status)return status;}goto L8c6134dc; /* fmac fr0,fr3,fr5 */
L8c6134dc: sa_motion_publish_stop(ctx,0x8c6134dcu); fload(m,bus,&io_status,f,3u,r[9]);if(io_status)return io_status;goto L8c6134de; /* fmov.s @r9,fr3 */
L8c6134de: sa_motion_publish_stop(ctx,0x8c6134deu); fcopy(f,0u,4u);goto L8c6134e0; /* fmov fr4,fr0 */
L8c6134e0: sa_motion_publish_stop(ctx,0x8c6134e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6134e2; /* fmul fr5,fr3 */
L8c6134e2: sa_motion_publish_stop(ctx,0x8c6134e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6134e4; /* fmac fr0,fr3,fr2 */
L8c6134e4: sa_motion_publish_stop(ctx,0x8c6134e4u); fstore(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c6134e6; /* fmov fr2,@(r0,r11) */
L8c6134e6: sa_motion_publish_stop(ctx,0x8c6134e6u); r[0]=24u;goto L8c6134e8; /* mov #0x18,r0 */
L8c6134e8: sa_motion_publish_stop(ctx,0x8c6134e8u); r[13]+=1u;goto L8c6134ea; /* add #0x1,r13 */
L8c6134ea: sa_motion_publish_stop(ctx,0x8c6134eau); fstore(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c6134ec; /* fmov fr0,@(r0,r11) */
L8c6134ec: sa_motion_publish_stop(ctx,0x8c6134ecu); r[11]=r[6];goto L8c6134ee; /* mov r6,r11 */
L8c6134ee: sa_motion_publish_stop(ctx,0x8c6134eeu); r[11]<<=2;goto L8c6134f0; /* shll2 r11 */
L8c6134f0: sa_motion_publish_stop(ctx,0x8c6134f0u); r[11]+=r[7];goto L8c6134f2; /* add r7,r11 */
L8c6134f2: sa_motion_publish_stop(ctx,0x8c6134f2u); r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c6134f4; /* mov.l @r11,r3 */
L8c6134f4: sa_motion_publish_stop(ctx,0x8c6134f4u); r[2]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c6134f6; /* mov.l @(0xc,r3),r2 */
L8c6134f6: sa_motion_publish_stop(ctx,0x8c6134f6u); *sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c6134f8; /* tst r4,r2 */
L8c6134f8: sa_motion_publish_stop(ctx,0x8c6134f8u); if(!(*sr&1u))goto L8c613524;goto L8c6134fa; /* bf 0x8c613524 */
L8c6134fa: sa_motion_publish_stop(ctx,0x8c6134fau); r[1]=r[13];goto L8c6134fc; /* mov r13,r1 */
L8c6134fc: sa_motion_publish_stop(ctx,0x8c6134fcu); r[1]<<=2;goto L8c6134fe; /* shll2 r1 */
L8c6134fe: sa_motion_publish_stop(ctx,0x8c6134feu); r[0]=16u;goto L8c613500; /* mov #0x10,r0 */
L8c613500: sa_motion_publish_stop(ctx,0x8c613500u); r[1]<<=2;goto L8c613502; /* shll2 r1 */
L8c613502: sa_motion_publish_stop(ctx,0x8c613502u); *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1;goto L8c613504; /* shll r1 */
L8c613504: sa_motion_publish_stop(ctx,0x8c613504u); r[1]+=r[5];goto L8c613506; /* add r5,r1 */
L8c613506: sa_motion_publish_stop(ctx,0x8c613506u); r[13]+=1u;goto L8c613508; /* add #0x1,r13 */
L8c613508: sa_motion_publish_stop(ctx,0x8c613508u); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c61350a; /* mov.l r1,@r15 */
L8c61350a: sa_motion_publish_stop(ctx,0x8c61350au); r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61350c; /* mov.l @r11,r3 */
L8c61350c: sa_motion_publish_stop(ctx,0x8c61350cu); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61350e; /* fmov.s @(r0,r3),fr3 */
L8c61350e: sa_motion_publish_stop(ctx,0x8c61350eu); fstore(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c613510; /* fmov fr3,@(r0,r1) */
L8c613510: sa_motion_publish_stop(ctx,0x8c613510u); r[0]=20u;goto L8c613512; /* mov #0x14,r0 */
L8c613512: sa_motion_publish_stop(ctx,0x8c613512u); r[2]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c613514; /* mov.l @r11,r2 */
L8c613514: sa_motion_publish_stop(ctx,0x8c613514u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613516; /* mov.l @r15,r3 */
L8c613516: sa_motion_publish_stop(ctx,0x8c613516u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c613518; /* fmov.s @(r0,r2),fr3 */
L8c613518: sa_motion_publish_stop(ctx,0x8c613518u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61351a; /* fmov fr3,@(r0,r3) */
L8c61351a: sa_motion_publish_stop(ctx,0x8c61351au); r[0]=24u;goto L8c61351c; /* mov #0x18,r0 */
L8c61351c: sa_motion_publish_stop(ctx,0x8c61351cu); r[2]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c61351e; /* mov.l @r11,r2 */
L8c61351e: sa_motion_publish_stop(ctx,0x8c61351eu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c613520; /* mov.l @r15,r3 */
L8c613520: sa_motion_publish_stop(ctx,0x8c613520u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c613522; /* fmov.s @(r0,r2),fr3 */
L8c613522: sa_motion_publish_stop(ctx,0x8c613522u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c613524; /* fmov fr3,@(r0,r3) */
L8c613524: sa_motion_publish_stop(ctx,0x8c613524u); r[11]=r[6];goto L8c613526; /* mov r6,r11 */
L8c613526: sa_motion_publish_stop(ctx,0x8c613526u); r[6]+=1u;goto L8c613528; /* add #0x1,r6 */
L8c613528: sa_motion_publish_stop(ctx,0x8c613528u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[10]);goto L8c61352a; /* cmp/ge r10,r6 */
L8c61352a: sa_motion_publish_stop(ctx,0x8c61352au); if(!(*sr&1u))goto L8c61344a;goto L8c61352c; /* bf 0x8c61344a */
L8c61352c: sa_motion_publish_stop(ctx,0x8c61352cu); r[4]=rd(m,bus,&io_status,0x8c613710u,4);if(io_status)return io_status;goto L8c61352e; /* mov.l 0x8c613710,r4 */
L8c61352e: sa_motion_publish_stop(ctx,0x8c61352eu); r[0]=r[13];goto L8c613530; /* mov r13,r0 */
L8c613530: sa_motion_publish_stop(ctx,0x8c613530u); r[7]=rd(m,bus,&io_status,0x8c613708u,4);if(io_status)return io_status;goto L8c613532; /* mov.l 0x8c613708,r7 */
L8c613532: sa_motion_publish_stop(ctx,0x8c613532u); *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c613534; /* cmp/eq #0x3,r0 */
L8c613534: sa_motion_publish_stop(ctx,0x8c613534u); r[6]=rd(m,bus,&io_status,0x8c61370cu,4);if(io_status)return io_status;goto L8c613536; /* mov.l 0x8c61370c,r6 */
L8c613536: sa_motion_publish_stop(ctx,0x8c613536u); if(!(*sr&1u))goto L8c6135da;goto L8c613538; /* bf 0x8c6135da */
L8c613538: sa_motion_publish_stop(ctx,0x8c613538u); r[0]=28u;goto L8c61353a; /* mov #0x1c,r0 */
L8c61353a: sa_motion_publish_stop(ctx,0x8c61353au); r[14]=r[5];goto L8c61353c; /* mov r5,r14 */
L8c61353c: sa_motion_publish_stop(ctx,0x8c61353cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61353e; /* fmov.s @(r0,r4),fr3 */
L8c61353e: sa_motion_publish_stop(ctx,0x8c61353eu); r[8]+=32u;goto L8c613540; /* add #0x20,r8 */
L8c613540: sa_motion_publish_stop(ctx,0x8c613540u); r[0]=24u;goto L8c613542; /* mov #0x18,r0 */
L8c613542: sa_motion_publish_stop(ctx,0x8c613542u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613544; /* fmov.s fr3,@-r8 */
L8c613544: sa_motion_publish_stop(ctx,0x8c613544u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613546; /* fmov.s @(r0,r4),fr3 */
L8c613546: sa_motion_publish_stop(ctx,0x8c613546u); r[0]=20u;goto L8c613548; /* mov #0x14,r0 */
L8c613548: sa_motion_publish_stop(ctx,0x8c613548u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61354a; /* fmov.s fr3,@-r8 */
L8c61354a: sa_motion_publish_stop(ctx,0x8c61354au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61354c; /* fmov.s @(r0,r4),fr3 */
L8c61354c: sa_motion_publish_stop(ctx,0x8c61354cu); r[0]=16u;goto L8c61354e; /* mov #0x10,r0 */
L8c61354e: sa_motion_publish_stop(ctx,0x8c61354eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613550; /* fmov.s fr3,@-r8 */
L8c613550: sa_motion_publish_stop(ctx,0x8c613550u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613552; /* fmov.s @(r0,r4),fr3 */
L8c613552: sa_motion_publish_stop(ctx,0x8c613552u); r[0]=24u;goto L8c613554; /* mov #0x18,r0 */
L8c613554: sa_motion_publish_stop(ctx,0x8c613554u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613556; /* fmov.s fr3,@-r8 */
L8c613556: sa_motion_publish_stop(ctx,0x8c613556u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613558; /* fmov.s @(r0,r14),fr3 */
L8c613558: sa_motion_publish_stop(ctx,0x8c613558u); r[0]=20u;goto L8c61355a; /* mov #0x14,r0 */
L8c61355a: sa_motion_publish_stop(ctx,0x8c61355au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61355c; /* fmov.s fr3,@-r8 */
L8c61355c: sa_motion_publish_stop(ctx,0x8c61355cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61355e; /* fmov.s @(r0,r14),fr3 */
L8c61355e: sa_motion_publish_stop(ctx,0x8c61355eu); r[0]=16u;goto L8c613560; /* mov #0x10,r0 */
L8c613560: sa_motion_publish_stop(ctx,0x8c613560u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613562; /* fmov.s fr3,@-r8 */
L8c613562: sa_motion_publish_stop(ctx,0x8c613562u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613564; /* fmov.s @(r0,r14),fr3 */
L8c613564: sa_motion_publish_stop(ctx,0x8c613564u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613566; /* fmov.s fr3,@-r8 */
L8c613566: sa_motion_publish_stop(ctx,0x8c613566u); r[8]-=4;wr(m,bus,&io_status,r[8],r[7],4);if(io_status)return io_status;goto L8c613568; /* mov.l r7,@-r8 */
L8c613568: sa_motion_publish_stop(ctx,0x8c613568u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61356a; /* pref @r8 */
L8c61356a: sa_motion_publish_stop(ctx,0x8c61356au); r[0]=28u;goto L8c61356c; /* mov #0x1c,r0 */
L8c61356c: sa_motion_publish_stop(ctx,0x8c61356cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61356e; /* fmov.s @(r0,r4),fr3 */
L8c61356e: sa_motion_publish_stop(ctx,0x8c61356eu); r[8]+=64u;goto L8c613570; /* add #0x40,r8 */
L8c613570: sa_motion_publish_stop(ctx,0x8c613570u); r[0]=24u;goto L8c613572; /* mov #0x18,r0 */
L8c613572: sa_motion_publish_stop(ctx,0x8c613572u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613574; /* fmov.s fr3,@-r8 */
L8c613574: sa_motion_publish_stop(ctx,0x8c613574u); r[14]+=32u;goto L8c613576; /* add #0x20,r14 */
L8c613576: sa_motion_publish_stop(ctx,0x8c613576u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613578; /* fmov.s @(r0,r4),fr3 */
L8c613578: sa_motion_publish_stop(ctx,0x8c613578u); r[0]=20u;goto L8c61357a; /* mov #0x14,r0 */
L8c61357a: sa_motion_publish_stop(ctx,0x8c61357au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61357c; /* fmov.s fr3,@-r8 */
L8c61357c: sa_motion_publish_stop(ctx,0x8c61357cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61357e; /* fmov.s @(r0,r4),fr3 */
L8c61357e: sa_motion_publish_stop(ctx,0x8c61357eu); r[0]=16u;goto L8c613580; /* mov #0x10,r0 */
L8c613580: sa_motion_publish_stop(ctx,0x8c613580u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613582; /* fmov.s fr3,@-r8 */
L8c613582: sa_motion_publish_stop(ctx,0x8c613582u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613584; /* fmov.s @(r0,r4),fr3 */
L8c613584: sa_motion_publish_stop(ctx,0x8c613584u); r[0]=24u;goto L8c613586; /* mov #0x18,r0 */
L8c613586: sa_motion_publish_stop(ctx,0x8c613586u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613588; /* fmov.s fr3,@-r8 */
L8c613588: sa_motion_publish_stop(ctx,0x8c613588u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61358a; /* fmov.s @(r0,r14),fr3 */
L8c61358a: sa_motion_publish_stop(ctx,0x8c61358au); r[0]=20u;goto L8c61358c; /* mov #0x14,r0 */
L8c61358c: sa_motion_publish_stop(ctx,0x8c61358cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61358e; /* fmov.s fr3,@-r8 */
L8c61358e: sa_motion_publish_stop(ctx,0x8c61358eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613590; /* fmov.s @(r0,r14),fr3 */
L8c613590: sa_motion_publish_stop(ctx,0x8c613590u); r[0]=16u;goto L8c613592; /* mov #0x10,r0 */
L8c613592: sa_motion_publish_stop(ctx,0x8c613592u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613594; /* fmov.s fr3,@-r8 */
L8c613594: sa_motion_publish_stop(ctx,0x8c613594u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613596; /* fmov.s @(r0,r14),fr3 */
L8c613596: sa_motion_publish_stop(ctx,0x8c613596u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613598; /* fmov.s fr3,@-r8 */
L8c613598: sa_motion_publish_stop(ctx,0x8c613598u); r[8]-=4;wr(m,bus,&io_status,r[8],r[7],4);if(io_status)return io_status;goto L8c61359a; /* mov.l r7,@-r8 */
L8c61359a: sa_motion_publish_stop(ctx,0x8c61359au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61359c; /* pref @r8 */
L8c61359c: sa_motion_publish_stop(ctx,0x8c61359cu); r[0]=28u;goto L8c61359e; /* mov #0x1c,r0 */
L8c61359e: sa_motion_publish_stop(ctx,0x8c61359eu); r[3]=rd(m,bus,&io_status,0x8c613714u,4);if(io_status)return io_status;goto L8c6135a0; /* mov.l 0x8c613714,r3 */
L8c6135a0: sa_motion_publish_stop(ctx,0x8c6135a0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135a2; /* fmov.s @(r0,r4),fr3 */
L8c6135a2: sa_motion_publish_stop(ctx,0x8c6135a2u); r[8]+=64u;goto L8c6135a4; /* add #0x40,r8 */
L8c6135a4: sa_motion_publish_stop(ctx,0x8c6135a4u); r[0]=24u;goto L8c6135a6; /* mov #0x18,r0 */
L8c6135a6: sa_motion_publish_stop(ctx,0x8c6135a6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135a8; /* fmov.s fr3,@-r8 */
L8c6135a8: sa_motion_publish_stop(ctx,0x8c6135a8u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135aa; /* fmov.s @(r0,r4),fr3 */
L8c6135aa: sa_motion_publish_stop(ctx,0x8c6135aau); r[0]=20u;goto L8c6135ac; /* mov #0x14,r0 */
L8c6135ac: sa_motion_publish_stop(ctx,0x8c6135acu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135ae; /* fmov.s fr3,@-r8 */
L8c6135ae: sa_motion_publish_stop(ctx,0x8c6135aeu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135b0; /* fmov.s @(r0,r4),fr3 */
L8c6135b0: sa_motion_publish_stop(ctx,0x8c6135b0u); r[0]=16u;goto L8c6135b2; /* mov #0x10,r0 */
L8c6135b2: sa_motion_publish_stop(ctx,0x8c6135b2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135b4; /* fmov.s fr3,@-r8 */
L8c6135b4: sa_motion_publish_stop(ctx,0x8c6135b4u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135b6; /* fmov.s @(r0,r4),fr3 */
L8c6135b6: sa_motion_publish_stop(ctx,0x8c6135b6u); r[4]=r[5];goto L8c6135b8; /* mov r5,r4 */
L8c6135b8: sa_motion_publish_stop(ctx,0x8c6135b8u); r[0]=24u;goto L8c6135ba; /* mov #0x18,r0 */
L8c6135ba: sa_motion_publish_stop(ctx,0x8c6135bau); r[4]+=64u;goto L8c6135bc; /* add #0x40,r4 */
L8c6135bc: sa_motion_publish_stop(ctx,0x8c6135bcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135be; /* fmov.s fr3,@-r8 */
L8c6135be: sa_motion_publish_stop(ctx,0x8c6135beu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135c0; /* fmov.s @(r0,r4),fr3 */
L8c6135c0: sa_motion_publish_stop(ctx,0x8c6135c0u); r[0]=20u;goto L8c6135c2; /* mov #0x14,r0 */
L8c6135c2: sa_motion_publish_stop(ctx,0x8c6135c2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135c4; /* fmov.s fr3,@-r8 */
L8c6135c4: sa_motion_publish_stop(ctx,0x8c6135c4u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135c6; /* fmov.s @(r0,r4),fr3 */
L8c6135c6: sa_motion_publish_stop(ctx,0x8c6135c6u); r[0]=16u;goto L8c6135c8; /* mov #0x10,r0 */
L8c6135c8: sa_motion_publish_stop(ctx,0x8c6135c8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135ca; /* fmov.s fr3,@-r8 */
L8c6135ca: sa_motion_publish_stop(ctx,0x8c6135cau); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135cc; /* fmov.s @(r0,r4),fr3 */
L8c6135cc: sa_motion_publish_stop(ctx,0x8c6135ccu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135ce; /* fmov.s fr3,@-r8 */
L8c6135ce: sa_motion_publish_stop(ctx,0x8c6135ceu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6135d0; /* mov.l r3,@-r8 */
L8c6135d0: sa_motion_publish_stop(ctx,0x8c6135d0u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6135d2; /* pref @r8 */
L8c6135d2: sa_motion_publish_stop(ctx,0x8c6135d2u); r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6135d4; /* mov.l @r6,r2 */
L8c6135d4: sa_motion_publish_stop(ctx,0x8c6135d4u); r[8]+=32u;goto L8c6135d6; /* add #0x20,r8 */
L8c6135d6: sa_motion_publish_stop(ctx,0x8c6135d8u); r[2]+=3u;goto L8c6136ae; /* bra 0x8c6136ae */
L8c6135da: sa_motion_publish_stop(ctx,0x8c6135dau); r[0]=28u;goto L8c6135dc; /* mov #0x1c,r0 */
L8c6135dc: sa_motion_publish_stop(ctx,0x8c6135dcu); r[14]=r[5];goto L8c6135de; /* mov r5,r14 */
L8c6135de: sa_motion_publish_stop(ctx,0x8c6135deu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135e0; /* fmov.s @(r0,r4),fr3 */
L8c6135e0: sa_motion_publish_stop(ctx,0x8c6135e0u); r[8]+=32u;goto L8c6135e2; /* add #0x20,r8 */
L8c6135e2: sa_motion_publish_stop(ctx,0x8c6135e2u); r[0]=24u;goto L8c6135e4; /* mov #0x18,r0 */
L8c6135e4: sa_motion_publish_stop(ctx,0x8c6135e4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135e6; /* fmov.s fr3,@-r8 */
L8c6135e6: sa_motion_publish_stop(ctx,0x8c6135e6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135e8; /* fmov.s @(r0,r4),fr3 */
L8c6135e8: sa_motion_publish_stop(ctx,0x8c6135e8u); r[0]=20u;goto L8c6135ea; /* mov #0x14,r0 */
L8c6135ea: sa_motion_publish_stop(ctx,0x8c6135eau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135ec; /* fmov.s fr3,@-r8 */
L8c6135ec: sa_motion_publish_stop(ctx,0x8c6135ecu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135ee; /* fmov.s @(r0,r4),fr3 */
L8c6135ee: sa_motion_publish_stop(ctx,0x8c6135eeu); r[0]=16u;goto L8c6135f0; /* mov #0x10,r0 */
L8c6135f0: sa_motion_publish_stop(ctx,0x8c6135f0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135f2; /* fmov.s fr3,@-r8 */
L8c6135f2: sa_motion_publish_stop(ctx,0x8c6135f2u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6135f4; /* fmov.s @(r0,r4),fr3 */
L8c6135f4: sa_motion_publish_stop(ctx,0x8c6135f4u); r[0]=24u;goto L8c6135f6; /* mov #0x18,r0 */
L8c6135f6: sa_motion_publish_stop(ctx,0x8c6135f6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135f8; /* fmov.s fr3,@-r8 */
L8c6135f8: sa_motion_publish_stop(ctx,0x8c6135f8u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6135fa; /* fmov.s @(r0,r14),fr3 */
L8c6135fa: sa_motion_publish_stop(ctx,0x8c6135fau); r[0]=20u;goto L8c6135fc; /* mov #0x14,r0 */
L8c6135fc: sa_motion_publish_stop(ctx,0x8c6135fcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6135fe; /* fmov.s fr3,@-r8 */
L8c6135fe: sa_motion_publish_stop(ctx,0x8c6135feu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613600; /* fmov.s @(r0,r14),fr3 */
L8c613600: sa_motion_publish_stop(ctx,0x8c613600u); r[0]=16u;goto L8c613602; /* mov #0x10,r0 */
L8c613602: sa_motion_publish_stop(ctx,0x8c613602u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613604; /* fmov.s fr3,@-r8 */
L8c613604: sa_motion_publish_stop(ctx,0x8c613604u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613606; /* fmov.s @(r0,r14),fr3 */
L8c613606: sa_motion_publish_stop(ctx,0x8c613606u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613608; /* fmov.s fr3,@-r8 */
L8c613608: sa_motion_publish_stop(ctx,0x8c613608u); r[8]-=4;wr(m,bus,&io_status,r[8],r[7],4);if(io_status)return io_status;goto L8c61360a; /* mov.l r7,@-r8 */
L8c61360a: sa_motion_publish_stop(ctx,0x8c61360au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61360c; /* pref @r8 */
L8c61360c: sa_motion_publish_stop(ctx,0x8c61360cu); r[0]=28u;goto L8c61360e; /* mov #0x1c,r0 */
L8c61360e: sa_motion_publish_stop(ctx,0x8c61360eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613610; /* fmov.s @(r0,r4),fr3 */
L8c613610: sa_motion_publish_stop(ctx,0x8c613610u); r[8]+=64u;goto L8c613612; /* add #0x40,r8 */
L8c613612: sa_motion_publish_stop(ctx,0x8c613612u); r[0]=24u;goto L8c613614; /* mov #0x18,r0 */
L8c613614: sa_motion_publish_stop(ctx,0x8c613614u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613616; /* fmov.s fr3,@-r8 */
L8c613616: sa_motion_publish_stop(ctx,0x8c613616u); r[14]+=32u;goto L8c613618; /* add #0x20,r14 */
L8c613618: sa_motion_publish_stop(ctx,0x8c613618u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61361a; /* fmov.s @(r0,r4),fr3 */
L8c61361a: sa_motion_publish_stop(ctx,0x8c61361au); r[0]=20u;goto L8c61361c; /* mov #0x14,r0 */
L8c61361c: sa_motion_publish_stop(ctx,0x8c61361cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61361e; /* fmov.s fr3,@-r8 */
L8c61361e: sa_motion_publish_stop(ctx,0x8c61361eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613620; /* fmov.s @(r0,r4),fr3 */
L8c613620: sa_motion_publish_stop(ctx,0x8c613620u); r[0]=16u;goto L8c613622; /* mov #0x10,r0 */
L8c613622: sa_motion_publish_stop(ctx,0x8c613622u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613624; /* fmov.s fr3,@-r8 */
L8c613624: sa_motion_publish_stop(ctx,0x8c613624u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613626; /* fmov.s @(r0,r4),fr3 */
L8c613626: sa_motion_publish_stop(ctx,0x8c613626u); r[0]=24u;goto L8c613628; /* mov #0x18,r0 */
L8c613628: sa_motion_publish_stop(ctx,0x8c613628u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61362a; /* fmov.s fr3,@-r8 */
L8c61362a: sa_motion_publish_stop(ctx,0x8c61362au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61362c; /* fmov.s @(r0,r14),fr3 */
L8c61362c: sa_motion_publish_stop(ctx,0x8c61362cu); r[0]=20u;goto L8c61362e; /* mov #0x14,r0 */
L8c61362e: sa_motion_publish_stop(ctx,0x8c61362eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613630; /* fmov.s fr3,@-r8 */
L8c613630: sa_motion_publish_stop(ctx,0x8c613630u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613632; /* fmov.s @(r0,r14),fr3 */
L8c613632: sa_motion_publish_stop(ctx,0x8c613632u); r[0]=16u;goto L8c613634; /* mov #0x10,r0 */
L8c613634: sa_motion_publish_stop(ctx,0x8c613634u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613636; /* fmov.s fr3,@-r8 */
L8c613636: sa_motion_publish_stop(ctx,0x8c613636u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613638; /* fmov.s @(r0,r14),fr3 */
L8c613638: sa_motion_publish_stop(ctx,0x8c613638u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61363a; /* fmov.s fr3,@-r8 */
L8c61363a: sa_motion_publish_stop(ctx,0x8c61363au); r[8]-=4;wr(m,bus,&io_status,r[8],r[7],4);if(io_status)return io_status;goto L8c61363c; /* mov.l r7,@-r8 */
L8c61363c: sa_motion_publish_stop(ctx,0x8c61363cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61363e; /* pref @r8 */
L8c61363e: sa_motion_publish_stop(ctx,0x8c61363eu); r[0]=28u;goto L8c613640; /* mov #0x1c,r0 */
L8c613640: sa_motion_publish_stop(ctx,0x8c613640u); r[14]=r[5];goto L8c613642; /* mov r5,r14 */
L8c613642: sa_motion_publish_stop(ctx,0x8c613642u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613644; /* fmov.s @(r0,r4),fr3 */
L8c613644: sa_motion_publish_stop(ctx,0x8c613644u); r[8]+=64u;goto L8c613646; /* add #0x40,r8 */
L8c613646: sa_motion_publish_stop(ctx,0x8c613646u); r[0]=24u;goto L8c613648; /* mov #0x18,r0 */
L8c613648: sa_motion_publish_stop(ctx,0x8c613648u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61364a; /* fmov.s fr3,@-r8 */
L8c61364a: sa_motion_publish_stop(ctx,0x8c61364au); r[14]+=96u;goto L8c61364c; /* add #0x60,r14 */
L8c61364c: sa_motion_publish_stop(ctx,0x8c61364cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61364e; /* fmov.s @(r0,r4),fr3 */
L8c61364e: sa_motion_publish_stop(ctx,0x8c61364eu); r[0]=20u;goto L8c613650; /* mov #0x14,r0 */
L8c613650: sa_motion_publish_stop(ctx,0x8c613650u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613652; /* fmov.s fr3,@-r8 */
L8c613652: sa_motion_publish_stop(ctx,0x8c613652u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613654; /* fmov.s @(r0,r4),fr3 */
L8c613654: sa_motion_publish_stop(ctx,0x8c613654u); r[0]=16u;goto L8c613656; /* mov #0x10,r0 */
L8c613656: sa_motion_publish_stop(ctx,0x8c613656u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613658; /* fmov.s fr3,@-r8 */
L8c613658: sa_motion_publish_stop(ctx,0x8c613658u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61365a; /* fmov.s @(r0,r4),fr3 */
L8c61365a: sa_motion_publish_stop(ctx,0x8c61365au); r[0]=24u;goto L8c61365c; /* mov #0x18,r0 */
L8c61365c: sa_motion_publish_stop(ctx,0x8c61365cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61365e; /* fmov.s fr3,@-r8 */
L8c61365e: sa_motion_publish_stop(ctx,0x8c61365eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613660; /* fmov.s @(r0,r14),fr3 */
L8c613660: sa_motion_publish_stop(ctx,0x8c613660u); r[0]=20u;goto L8c613662; /* mov #0x14,r0 */
L8c613662: sa_motion_publish_stop(ctx,0x8c613662u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613664; /* fmov.s fr3,@-r8 */
L8c613664: sa_motion_publish_stop(ctx,0x8c613664u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c613666; /* fmov.s @(r0,r14),fr3 */
L8c613666: sa_motion_publish_stop(ctx,0x8c613666u); r[0]=16u;goto L8c613668; /* mov #0x10,r0 */
L8c613668: sa_motion_publish_stop(ctx,0x8c613668u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61366a; /* fmov.s fr3,@-r8 */
L8c61366a: sa_motion_publish_stop(ctx,0x8c61366au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61366c; /* fmov.s @(r0,r14),fr3 */
L8c61366c: sa_motion_publish_stop(ctx,0x8c61366cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61366e; /* fmov.s fr3,@-r8 */
L8c61366e: sa_motion_publish_stop(ctx,0x8c61366eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[7],4);if(io_status)return io_status;goto L8c613670; /* mov.l r7,@-r8 */
L8c613670: sa_motion_publish_stop(ctx,0x8c613670u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c613672; /* pref @r8 */
L8c613672: sa_motion_publish_stop(ctx,0x8c613672u); r[0]=28u;goto L8c613674; /* mov #0x1c,r0 */
L8c613674: sa_motion_publish_stop(ctx,0x8c613674u); r[3]=rd(m,bus,&io_status,0x8c613714u,4);if(io_status)return io_status;goto L8c613676; /* mov.l 0x8c613714,r3 */
L8c613676: sa_motion_publish_stop(ctx,0x8c613676u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613678; /* fmov.s @(r0,r4),fr3 */
L8c613678: sa_motion_publish_stop(ctx,0x8c613678u); r[8]+=64u;goto L8c61367a; /* add #0x40,r8 */
L8c61367a: sa_motion_publish_stop(ctx,0x8c61367au); r[0]=24u;goto L8c61367c; /* mov #0x18,r0 */
L8c61367c: sa_motion_publish_stop(ctx,0x8c61367cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61367e; /* fmov.s fr3,@-r8 */
L8c61367e: sa_motion_publish_stop(ctx,0x8c61367eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613680; /* fmov.s @(r0,r4),fr3 */
L8c613680: sa_motion_publish_stop(ctx,0x8c613680u); r[0]=20u;goto L8c613682; /* mov #0x14,r0 */
L8c613682: sa_motion_publish_stop(ctx,0x8c613682u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613684; /* fmov.s fr3,@-r8 */
L8c613684: sa_motion_publish_stop(ctx,0x8c613684u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613686; /* fmov.s @(r0,r4),fr3 */
L8c613686: sa_motion_publish_stop(ctx,0x8c613686u); r[0]=16u;goto L8c613688; /* mov #0x10,r0 */
L8c613688: sa_motion_publish_stop(ctx,0x8c613688u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61368a; /* fmov.s fr3,@-r8 */
L8c61368a: sa_motion_publish_stop(ctx,0x8c61368au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61368c; /* fmov.s @(r0,r4),fr3 */
L8c61368c: sa_motion_publish_stop(ctx,0x8c61368cu); r[4]=r[5];goto L8c61368e; /* mov r5,r4 */
L8c61368e: sa_motion_publish_stop(ctx,0x8c61368eu); r[0]=24u;goto L8c613690; /* mov #0x18,r0 */
L8c613690: sa_motion_publish_stop(ctx,0x8c613690u); r[4]+=64u;goto L8c613692; /* add #0x40,r4 */
L8c613692: sa_motion_publish_stop(ctx,0x8c613692u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c613694; /* fmov.s fr3,@-r8 */
L8c613694: sa_motion_publish_stop(ctx,0x8c613694u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c613696; /* fmov.s @(r0,r4),fr3 */
L8c613696: sa_motion_publish_stop(ctx,0x8c613696u); r[0]=20u;goto L8c613698; /* mov #0x14,r0 */
L8c613698: sa_motion_publish_stop(ctx,0x8c613698u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61369a; /* fmov.s fr3,@-r8 */
L8c61369a: sa_motion_publish_stop(ctx,0x8c61369au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61369c; /* fmov.s @(r0,r4),fr3 */
L8c61369c: sa_motion_publish_stop(ctx,0x8c61369cu); r[0]=16u;goto L8c61369e; /* mov #0x10,r0 */
L8c61369e: sa_motion_publish_stop(ctx,0x8c61369eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6136a0; /* fmov.s fr3,@-r8 */
L8c6136a0: sa_motion_publish_stop(ctx,0x8c6136a0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6136a2; /* fmov.s @(r0,r4),fr3 */
L8c6136a2: sa_motion_publish_stop(ctx,0x8c6136a2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6136a4; /* fmov.s fr3,@-r8 */
L8c6136a4: sa_motion_publish_stop(ctx,0x8c6136a4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6136a6; /* mov.l r3,@-r8 */
L8c6136a6: sa_motion_publish_stop(ctx,0x8c6136a6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6136a8; /* pref @r8 */
L8c6136a8: sa_motion_publish_stop(ctx,0x8c6136a8u); r[2]=rd(m,bus,&io_status,r[6],4);if(io_status)return io_status;goto L8c6136aa; /* mov.l @r6,r2 */
L8c6136aa: sa_motion_publish_stop(ctx,0x8c6136aau); r[8]+=32u;goto L8c6136ac; /* add #0x20,r8 */
L8c6136ac: sa_motion_publish_stop(ctx,0x8c6136acu); r[2]+=4u;goto L8c6136ae; /* add #0x4,r2 */
L8c6136ae: sa_motion_publish_stop(ctx,0x8c6136aeu); wr(m,bus,&io_status,r[6],r[2],4);if(io_status)return io_status;goto L8c6136b0; /* mov.l r2,@r6 */
L8c6136b0: sa_motion_publish_stop(ctx,0x8c6136b0u); r[15]+=60u;goto L8c6136b2; /* add #0x3c,r15 */
L8c6136b2: sa_motion_publish_stop(ctx,0x8c6136b2u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6136b4; /* mov.l @r15+,r9 */
L8c6136b4: sa_motion_publish_stop(ctx,0x8c6136b4u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6136b6; /* mov.l @r15+,r10 */
L8c6136b6: sa_motion_publish_stop(ctx,0x8c6136b6u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6136b8; /* mov.l @r15+,r11 */
L8c6136b8: sa_motion_publish_stop(ctx,0x8c6136b8u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6136ba; /* mov.l @r15+,r12 */
L8c6136ba: sa_motion_publish_stop(ctx,0x8c6136bau); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6136bc; /* mov.l @r15+,r13 */
L8c6136bc: sa_motion_publish_stop(ctx,0x8c6136beu); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
