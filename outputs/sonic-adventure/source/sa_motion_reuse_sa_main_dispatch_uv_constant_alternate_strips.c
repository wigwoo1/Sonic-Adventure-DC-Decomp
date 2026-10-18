#include "sa_draw_motion_stop.h"
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
L8c61430e: sa_motion_publish_stop(ctx,0x8c61430eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c614310; /* mov.l r14,@-r15 */
L8c614310: sa_motion_publish_stop(ctx,0x8c614310u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c614312; /* mov.l r13,@-r15 */
L8c614312: sa_motion_publish_stop(ctx,0x8c614312u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c614314; /* mov.l r12,@-r15 */
L8c614314: sa_motion_publish_stop(ctx,0x8c614314u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c614316; /* mov.l r11,@-r15 */
L8c614316: sa_motion_publish_stop(ctx,0x8c614316u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c614318; /* mov.l r10,@-r15 */
L8c614318: sa_motion_publish_stop(ctx,0x8c614318u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61431a; /* mov.l r9,@-r15 */
L8c61431a: sa_motion_publish_stop(ctx,0x8c61431au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61431c; /* fmov.s fr15,@-r15 */
L8c61431c: sa_motion_publish_stop(ctx,0x8c61431cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61431e; /* fmov.s fr14,@-r15 */
L8c61431e: sa_motion_publish_stop(ctx,0x8c61431eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c614320; /* fmov.s fr13,@-r15 */
L8c614320: sa_motion_publish_stop(ctx,0x8c614320u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c614322; /* sts.l PR,@-r15 */
L8c614322: sa_motion_publish_stop(ctx,0x8c614322u); r[15]+=4294967276u;goto L8c614324; /* add #-0x14,r15 */
L8c614324: sa_motion_publish_stop(ctx,0x8c614324u); r[3]=rd(m,bus,&io_status,0x8c614358u,4);if(io_status)return io_status;goto L8c614326; /* mov.l 0x8c614358,r3 */
L8c614326: sa_motion_publish_stop(ctx,0x8c614326u); r[9]=rd(m,bus,&io_status,0x8c614354u,4);if(io_status)return io_status;goto L8c614328; /* mov.l 0x8c614354,r9 */
L8c614328: sa_motion_publish_stop(ctx,0x8c61432au); target=r[3];*pr=0x8c61432cu;r[12]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61432c; /* jsr @r3 */
L8c61432c: sa_motion_publish_stop(ctx,0x8c61432cu); r[2]=rd(m,bus,&io_status,0x8c61435cu,4);if(io_status)return io_status;goto L8c61432e; /* mov.l 0x8c61435c,r2 */
L8c61432e: sa_motion_publish_stop(ctx,0x8c61432eu); r[1]=0u;goto L8c614330; /* mov #0x0,r1 */
L8c614330: sa_motion_publish_stop(ctx,0x8c614330u); r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c614332; /* mov.w @(0x2,r12),r0 */
L8c614332: sa_motion_publish_stop(ctx,0x8c614332u); r[14]=rd(m,bus,&io_status,r[12]+0x14u,4);if(io_status)return io_status;goto L8c614334; /* mov.l @(0x14,r12),r14 */
L8c614334: sa_motion_publish_stop(ctx,0x8c614334u); r[13]=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;goto L8c614336; /* mov.l @(0x4,r12),r13 */
L8c614336: sa_motion_publish_stop(ctx,0x8c614336u); r[0]=r[0]&65535u;goto L8c614338; /* extu.w r0,r0 */
L8c614338: sa_motion_publish_stop(ctx,0x8c614338u); wr(m,bus,&io_status,r[15]+0x10u,r[0],4);if(io_status)return io_status;goto L8c61433a; /* mov.l r0,@(0x10,r15) */
L8c61433a: sa_motion_publish_stop(ctx,0x8c61433au); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61433c; /* mov.l @r2,r3 */
L8c61433c: sa_motion_publish_stop(ctx,0x8c61433cu); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61433e; /* mov.l r3,@(0x4,r15) */
L8c61433e: sa_motion_publish_stop(ctx,0x8c61433eu); wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c614340; /* mov.l r1,@(0x8,r15) */
L8c614340: sa_motion_publish_stop(ctx,0x8c614340u); wr(m,bus,&io_status,r[15]+0xcu,r[1],4);if(io_status)return io_status;goto L8c614342; /* mov.l r1,@(0xc,r15) */
L8c614342: sa_motion_publish_stop(ctx,0x8c614344u); goto L8c614360; /* bra 0x8c614360 */
L8c614360: sa_motion_publish_stop(ctx,0x8c614360u); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c614362; /* mov.w @r13+,r3 */
L8c614362: sa_motion_publish_stop(ctx,0x8c614362u); r[4]=rd(m,bus,&io_status,0x8c61444cu,4);if(io_status)return io_status;goto L8c614364; /* mov.l 0x8c61444c,r4 */
L8c614364: sa_motion_publish_stop(ctx,0x8c614364u); r[3]=r[3]&65535u;goto L8c614366; /* extu.w r3,r3 */
L8c614366: sa_motion_publish_stop(ctx,0x8c614366u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c614368; /* fldi1 fr4 */
L8c614368: sa_motion_publish_stop(ctx,0x8c614368u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c61436a; /* tst r4,r3 */
L8c61436a: sa_motion_publish_stop(ctx,0x8c61436au); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61436c; /* mov.l r3,@r15 */
L8c61436c: sa_motion_publish_stop(ctx,0x8c61436eu); take=(*sr&1u);fcopy(f,15u,4u);if(take)goto L8c61437e;goto L8c614370; /* bt/s 0x8c61437e */
L8c614370: sa_motion_publish_stop(ctx,0x8c614370u); r[4]=sx(m,bus,&io_status,0x8c61444au,2);if(io_status)return io_status;goto L8c614372; /* mov.w 0x8c61444a,r4 */
L8c614372: sa_motion_publish_stop(ctx,0x8c614372u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614374; /* mov.l @r15,r3 */
L8c614374: sa_motion_publish_stop(ctx,0x8c614374u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c614376; /* fldi1 fr4 */
L8c614376: sa_motion_publish_stop(ctx,0x8c614376u); r[3]&=r[4];goto L8c614378; /* and r4,r3 */
L8c614378: sa_motion_publish_stop(ctx,0x8c614378u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c61437a; /* fneg fr4 */
L8c61437a: sa_motion_publish_stop(ctx,0x8c61437au); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61437c; /* mov.l r3,@r15 */
L8c61437c: sa_motion_publish_stop(ctx,0x8c61437cu); fcopy(f,15u,4u);goto L8c61437e; /* fmov fr4,fr15 */
L8c61437e: sa_motion_publish_stop(ctx,0x8c61437eu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c614380; /* mov.l @r15,r3 */
L8c614380: sa_motion_publish_stop(ctx,0x8c614380u); r[0]=16u;goto L8c614382; /* mov #0x10,r0 */
L8c614382: sa_motion_publish_stop(ctx,0x8c614382u); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614384; /* mov.l @(0xc,r15),r2 */
L8c614384: sa_motion_publish_stop(ctx,0x8c614384u); r[10]=r[8];goto L8c614386; /* mov r8,r10 */
L8c614386: sa_motion_publish_stop(ctx,0x8c614386u); r[2]+=r[3];goto L8c614388; /* add r3,r2 */
L8c614388: sa_motion_publish_stop(ctx,0x8c614388u); wr(m,bus,&io_status,r[15]+0xcu,r[2],4);if(io_status)return io_status;goto L8c61438a; /* mov.l r2,@(0xc,r15) */
L8c61438a: sa_motion_publish_stop(ctx,0x8c61438au); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61438c; /* mov.l @r15,r1 */
L8c61438c: sa_motion_publish_stop(ctx,0x8c61438cu); r[1]+=4294967294u;goto L8c61438e; /* add #-0x2,r1 */
L8c61438e: sa_motion_publish_stop(ctx,0x8c61438eu); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c614390; /* mov.l r1,@r15 */
L8c614390: sa_motion_publish_stop(ctx,0x8c614390u); r[4]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c614392; /* mov.w @r13+,r4 */
L8c614392: sa_motion_publish_stop(ctx,0x8c614392u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c614394; /* mov.l @(0x4,r15),r3 */
L8c614394: sa_motion_publish_stop(ctx,0x8c614394u); r[11]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c614396; /* mov.w @r13+,r11 */
L8c614396: sa_motion_publish_stop(ctx,0x8c614396u); r[4]=r[4]&65535u;goto L8c614398; /* extu.w r4,r4 */
L8c614398: sa_motion_publish_stop(ctx,0x8c614398u); r[4]<<=2;goto L8c61439a; /* shll2 r4 */
L8c61439a: sa_motion_publish_stop(ctx,0x8c61439au); r[11]=r[11]&65535u;goto L8c61439c; /* extu.w r11,r11 */
L8c61439c: sa_motion_publish_stop(ctx,0x8c61439cu); r[11]<<=2;goto L8c61439e; /* shll2 r11 */
L8c61439e: sa_motion_publish_stop(ctx,0x8c61439eu); r[4]<<=2;goto L8c6143a0; /* shll2 r4 */
L8c6143a0: sa_motion_publish_stop(ctx,0x8c6143a0u); r[11]<<=2;goto L8c6143a2; /* shll2 r11 */
L8c6143a2: sa_motion_publish_stop(ctx,0x8c6143a2u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6143a4; /* shll r4 */
L8c6143a4: sa_motion_publish_stop(ctx,0x8c6143a4u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c6143a6; /* shll r11 */
L8c6143a6: sa_motion_publish_stop(ctx,0x8c6143a6u); r[11]+=r[3];goto L8c6143a8; /* add r3,r11 */
L8c6143a8: sa_motion_publish_stop(ctx,0x8c6143a8u); r[4]+=r[3];goto L8c6143aa; /* add r3,r4 */
L8c6143aa: sa_motion_publish_stop(ctx,0x8c6143aau); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c6143ac; /* fmov.s @(r0,r11),fr14 */
L8c6143ac: sa_motion_publish_stop(ctx,0x8c6143acu); fload(m,bus,&io_status,f,4u,r[0]+r[4]);if(io_status)return io_status;goto L8c6143ae; /* fmov.s @(r0,r4),fr4 */
L8c6143ae: sa_motion_publish_stop(ctx,0x8c6143aeu); r[0]=20u;goto L8c6143b0; /* mov #0x14,r0 */
L8c6143b0: sa_motion_publish_stop(ctx,0x8c6143b0u); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c6143b2; /* fmov.s @(r0,r11),fr13 */
L8c6143b2: sa_motion_publish_stop(ctx,0x8c6143b2u); fload(m,bus,&io_status,f,5u,r[0]+r[4]);if(io_status)return io_status;goto L8c6143b4; /* fmov.s @(r0,r4),fr5 */
L8c6143b4: sa_motion_publish_stop(ctx,0x8c6143b4u); r[12]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6143b6; /* mov.w @r13+,r12 */
L8c6143b6: sa_motion_publish_stop(ctx,0x8c6143b6u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6143b8; /* mov.l @(0x4,r15),r3 */
L8c6143b8: sa_motion_publish_stop(ctx,0x8c6143b8u); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c6143ba; /* mov.w @r13,r2 */
L8c6143ba: sa_motion_publish_stop(ctx,0x8c6143bau); r[12]=r[12]&65535u;goto L8c6143bc; /* extu.w r12,r12 */
L8c6143bc: sa_motion_publish_stop(ctx,0x8c6143bcu); r[12]<<=2;goto L8c6143be; /* shll2 r12 */
L8c6143be: sa_motion_publish_stop(ctx,0x8c6143beu); r[2]=r[2]&65535u;goto L8c6143c0; /* extu.w r2,r2 */
L8c6143c0: sa_motion_publish_stop(ctx,0x8c6143c0u); r[2]<<=2;goto L8c6143c2; /* shll2 r2 */
L8c6143c2: sa_motion_publish_stop(ctx,0x8c6143c2u); r[2]<<=2;goto L8c6143c4; /* shll2 r2 */
L8c6143c4: sa_motion_publish_stop(ctx,0x8c6143c4u); r[12]<<=2;goto L8c6143c6; /* shll2 r12 */
L8c6143c6: sa_motion_publish_stop(ctx,0x8c6143c6u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c6143c8; /* shll r12 */
L8c6143c8: sa_motion_publish_stop(ctx,0x8c6143c8u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c6143ca; /* shll r2 */
L8c6143ca: sa_motion_publish_stop(ctx,0x8c6143cau); r[2]+=r[3];goto L8c6143cc; /* add r3,r2 */
L8c6143cc: sa_motion_publish_stop(ctx,0x8c6143ccu); r[12]+=r[3];goto L8c6143ce; /* add r3,r12 */
L8c6143ce: sa_motion_publish_stop(ctx,0x8c6143ceu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c6143d0; /* pref @r2 */
L8c6143d0: sa_motion_publish_stop(ctx,0x8c6143d0u); r[0]=20u;goto L8c6143d2; /* mov #0x14,r0 */
L8c6143d2: sa_motion_publish_stop(ctx,0x8c6143d2u); fcopy(f,3u,14u);goto L8c6143d4; /* fmov fr14,fr3 */
L8c6143d4: sa_motion_publish_stop(ctx,0x8c6143d4u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6143d6; /* fmov.s @(r0,r12),fr2 */
L8c6143d6: sa_motion_publish_stop(ctx,0x8c6143d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6143d8; /* fsub fr4,fr3 */
L8c6143d8: sa_motion_publish_stop(ctx,0x8c6143d8u); r[0]=16u;goto L8c6143da; /* mov #0x10,r0 */
L8c6143da: sa_motion_publish_stop(ctx,0x8c6143dau); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c6143dc; /* cmp/hs r8,r10 */
L8c6143dc: sa_motion_publish_stop(ctx,0x8c6143dcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[13],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6143de; /* fsub fr13,fr2 */
L8c6143de: sa_motion_publish_stop(ctx,0x8c6143deu); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c6143e0; /* fmov.s @(r0,r12),fr1 */
L8c6143e0: sa_motion_publish_stop(ctx,0x8c6143e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[14],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6143e2; /* fsub fr14,fr1 */
L8c6143e2: sa_motion_publish_stop(ctx,0x8c6143e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6143e4; /* fmul fr2,fr3 */
L8c6143e4: sa_motion_publish_stop(ctx,0x8c6143e4u); fcopy(f,2u,13u);goto L8c6143e6; /* fmov fr13,fr2 */
L8c6143e6: sa_motion_publish_stop(ctx,0x8c6143e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6143e8; /* fsub fr5,fr2 */
L8c6143e8: sa_motion_publish_stop(ctx,0x8c6143e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6143ea; /* fmul fr1,fr2 */
L8c6143ea: sa_motion_publish_stop(ctx,0x8c6143eau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6143ec; /* fsub fr2,fr3 */
L8c6143ec: sa_motion_publish_stop(ctx,0x8c6143ecu); fcopy(f,4u,3u);goto L8c6143ee; /* fmov fr3,fr4 */
L8c6143ee: sa_motion_publish_stop(ctx,0x8c6143f0u); take=(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}if(take)goto L8c614450;goto L8c6143f2; /* bt/s 0x8c614450 */
L8c6143f2: sa_motion_publish_stop(ctx,0x8c6143f2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6143f4; /* fldi0 fr3 */
L8c6143f4: sa_motion_publish_stop(ctx,0x8c6143f4u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6143f6; /* fcmp/gt fr4,fr3 */
L8c6143f6: sa_motion_publish_stop(ctx,0x8c6143f6u); if((*sr&1u))goto L8c614494;goto L8c6143f8; /* bt 0x8c614494 */
L8c6143f8: sa_motion_publish_stop(ctx,0x8c6143f8u); r[6]=r[14];goto L8c6143fa; /* mov r14,r6 */
L8c6143fa: sa_motion_publish_stop(ctx,0x8c6143fau); r[4]=r[11];goto L8c6143fc; /* mov r11,r4 */
L8c6143fc: sa_motion_publish_stop(ctx,0x8c6143fcu); r[5]=r[9];goto L8c6143fe; /* mov r9,r5 */
L8c6143fe: sa_motion_publish_stop(ctx,0x8c6143feu); r[14]+=4u;goto L8c614400; /* add #0x4,r14 */
L8c614400: sa_motion_publish_stop(ctx,0x8c614400u); r[4]+=16u;goto L8c614402; /* add #0x10,r4 */
L8c614402: sa_motion_publish_stop(ctx,0x8c614402u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614404; /* fschg */
L8c614404: sa_motion_publish_stop(ctx,0x8c614404u); r[8]+=16u;goto L8c614406; /* add #0x10,r8 */
L8c614406: sa_motion_publish_stop(ctx,0x8c614406u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614408; /* fmov.s @r4+,fr4 */
L8c614408: sa_motion_publish_stop(ctx,0x8c614408u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61440a; /* fmov.s @r4,fr6 */
L8c61440a: sa_motion_publish_stop(ctx,0x8c61440au); fcopy(f,0u,1u);goto L8c61440c; /* fmov fr1,fr0 */
L8c61440c: sa_motion_publish_stop(ctx,0x8c61440cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61440e; /* fschg */
L8c61440e: sa_motion_publish_stop(ctx,0x8c61440eu); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c614410; /* mov.w @r6+,r2 */
L8c614410: sa_motion_publish_stop(ctx,0x8c614410u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c614412; /* mov.w @r6,r3 */
L8c614412: sa_motion_publish_stop(ctx,0x8c614412u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614414; /* fmov.s fr6,@-r8 */
L8c614414: sa_motion_publish_stop(ctx,0x8c614414u); *fpul=r[3];goto L8c614416; /* lds r3,FPUL */
L8c614416: sa_motion_publish_stop(ctx,0x8c614416u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614418; /* fmov.s fr5,@-r8 */
L8c614418: sa_motion_publish_stop(ctx,0x8c614418u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61441a; /* float FPUL,fr1 */
L8c61441a: sa_motion_publish_stop(ctx,0x8c61441au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61441c; /* fmov.s fr4,@-r8 */
L8c61441c: sa_motion_publish_stop(ctx,0x8c61441cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61441e; /* fmul fr0,fr1 */
L8c61441e: sa_motion_publish_stop(ctx,0x8c61441eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614420; /* mov.l r5,@-r8 */
L8c614420: sa_motion_publish_stop(ctx,0x8c614420u); *fpul=r[2];goto L8c614422; /* lds r2,FPUL */
L8c614422: sa_motion_publish_stop(ctx,0x8c614422u); r[8]+=28u;goto L8c614424; /* add #0x1c,r8 */
L8c614424: sa_motion_publish_stop(ctx,0x8c614424u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c614426; /* float FPUL,fr2 */
L8c614426: sa_motion_publish_stop(ctx,0x8c614426u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c614428; /* fldi1 fr3 */
L8c614428: sa_motion_publish_stop(ctx,0x8c614428u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61442a; /* fmul fr0,fr2 */
L8c61442a: sa_motion_publish_stop(ctx,0x8c61442au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61442c; /* fmov.s fr3,@-r8 */
L8c61442c: sa_motion_publish_stop(ctx,0x8c61442cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61442e; /* fmov.s fr1,@-r8 */
L8c61442e: sa_motion_publish_stop(ctx,0x8c61442eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614430; /* fmov.s fr2,@-r8 */
L8c614430: sa_motion_publish_stop(ctx,0x8c614430u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614432; /* pref @r8 */
L8c614432: sa_motion_publish_stop(ctx,0x8c614432u); r[8]+=16u;goto L8c614434; /* add #0x10,r8 */
L8c614434: sa_motion_publish_stop(ctx,0x8c614434u); r[3]=r[8];goto L8c614436; /* mov r8,r3 */
L8c614436: sa_motion_publish_stop(ctx,0x8c614436u); r[3]-=r[10];goto L8c614438; /* sub r10,r3 */
L8c614438: sa_motion_publish_stop(ctx,0x8c614438u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c61443a; /* mov.l @(0x8,r15),r2 */
L8c61443a: sa_motion_publish_stop(ctx,0x8c61443au); r[3]+=32u;goto L8c61443c; /* add #0x20,r3 */
L8c61443c: sa_motion_publish_stop(ctx,0x8c61443cu); r[14]+=4294967292u;goto L8c61443e; /* add #-0x4,r14 */
L8c61443e: sa_motion_publish_stop(ctx,0x8c61443eu); r[2]+=r[3];goto L8c614440; /* add r3,r2 */
L8c614440: sa_motion_publish_stop(ctx,0x8c614440u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614442; /* mov.l r2,@(0x8,r15) */
L8c614442: sa_motion_publish_stop(ctx,0x8c614444u); r[10]=r[8];goto L8c6144d0; /* bra 0x8c6144d0 */
L8c614450: sa_motion_publish_stop(ctx,0x8c614450u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c614452; /* fldi0 fr3 */
L8c614452: sa_motion_publish_stop(ctx,0x8c614452u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c614454; /* fcmp/gt fr4,fr3 */
L8c614454: sa_motion_publish_stop(ctx,0x8c614456u); take=!(*sr&1u);r[14]+=4u;if(take)goto L8c6144d0;goto L8c614458; /* bf/s 0x8c6144d0 */
L8c614458: sa_motion_publish_stop(ctx,0x8c614458u); r[14]+=4294967292u;goto L8c61445a; /* add #-0x4,r14 */
L8c61445a: sa_motion_publish_stop(ctx,0x8c61445au); r[5]=rd(m,bus,&io_status,0x8c6145b8u,4);if(io_status)return io_status;goto L8c61445c; /* mov.l 0x8c6145b8,r5 */
L8c61445c: sa_motion_publish_stop(ctx,0x8c61445cu); r[6]=r[14];goto L8c61445e; /* mov r14,r6 */
L8c61445e: sa_motion_publish_stop(ctx,0x8c61445eu); r[14]+=4u;goto L8c614460; /* add #0x4,r14 */
L8c614460: sa_motion_publish_stop(ctx,0x8c614460u); r[4]+=16u;goto L8c614462; /* add #0x10,r4 */
L8c614462: sa_motion_publish_stop(ctx,0x8c614462u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614464; /* fschg */
L8c614464: sa_motion_publish_stop(ctx,0x8c614464u); r[8]+=16u;goto L8c614466; /* add #0x10,r8 */
L8c614466: sa_motion_publish_stop(ctx,0x8c614466u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614468; /* fmov.s @r4+,fr4 */
L8c614468: sa_motion_publish_stop(ctx,0x8c614468u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61446a; /* fmov.s @r4,fr6 */
L8c61446a: sa_motion_publish_stop(ctx,0x8c61446au); fcopy(f,0u,1u);goto L8c61446c; /* fmov fr1,fr0 */
L8c61446c: sa_motion_publish_stop(ctx,0x8c61446cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61446e; /* fschg */
L8c61446e: sa_motion_publish_stop(ctx,0x8c61446eu); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c614470; /* mov.w @r6+,r2 */
L8c614470: sa_motion_publish_stop(ctx,0x8c614470u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c614472; /* mov.w @r6,r3 */
L8c614472: sa_motion_publish_stop(ctx,0x8c614472u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614474; /* fmov.s fr6,@-r8 */
L8c614474: sa_motion_publish_stop(ctx,0x8c614474u); *fpul=r[3];goto L8c614476; /* lds r3,FPUL */
L8c614476: sa_motion_publish_stop(ctx,0x8c614476u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614478; /* fmov.s fr5,@-r8 */
L8c614478: sa_motion_publish_stop(ctx,0x8c614478u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61447a; /* float FPUL,fr1 */
L8c61447a: sa_motion_publish_stop(ctx,0x8c61447au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61447c; /* fmov.s fr4,@-r8 */
L8c61447c: sa_motion_publish_stop(ctx,0x8c61447cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61447e; /* fmul fr0,fr1 */
L8c61447e: sa_motion_publish_stop(ctx,0x8c61447eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614480; /* mov.l r5,@-r8 */
L8c614480: sa_motion_publish_stop(ctx,0x8c614480u); *fpul=r[2];goto L8c614482; /* lds r2,FPUL */
L8c614482: sa_motion_publish_stop(ctx,0x8c614482u); r[8]+=28u;goto L8c614484; /* add #0x1c,r8 */
L8c614484: sa_motion_publish_stop(ctx,0x8c614484u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c614486; /* float FPUL,fr2 */
L8c614486: sa_motion_publish_stop(ctx,0x8c614486u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c614488; /* fldi1 fr3 */
L8c614488: sa_motion_publish_stop(ctx,0x8c614488u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61448a; /* fmul fr0,fr2 */
L8c61448a: sa_motion_publish_stop(ctx,0x8c61448au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61448c; /* fmov.s fr3,@-r8 */
L8c61448c: sa_motion_publish_stop(ctx,0x8c61448cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61448e; /* fmov.s fr1,@-r8 */
L8c61448e: sa_motion_publish_stop(ctx,0x8c61448eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614490; /* fmov.s fr2,@-r8 */
L8c614490: sa_motion_publish_stop(ctx,0x8c614490u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614492; /* pref @r8 */
L8c614492: sa_motion_publish_stop(ctx,0x8c614492u); r[8]+=16u;goto L8c614494; /* add #0x10,r8 */
L8c614494: sa_motion_publish_stop(ctx,0x8c614494u); r[5]=rd(m,bus,&io_status,0x8c6145b8u,4);if(io_status)return io_status;goto L8c614496; /* mov.l 0x8c6145b8,r5 */
L8c614496: sa_motion_publish_stop(ctx,0x8c614496u); r[6]=r[14];goto L8c614498; /* mov r14,r6 */
L8c614498: sa_motion_publish_stop(ctx,0x8c614498u); r[4]=r[11];goto L8c61449a; /* mov r11,r4 */
L8c61449a: sa_motion_publish_stop(ctx,0x8c61449au); r[14]+=4u;goto L8c61449c; /* add #0x4,r14 */
L8c61449c: sa_motion_publish_stop(ctx,0x8c61449cu); r[4]+=16u;goto L8c61449e; /* add #0x10,r4 */
L8c61449e: sa_motion_publish_stop(ctx,0x8c61449eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6144a0; /* fschg */
L8c6144a0: sa_motion_publish_stop(ctx,0x8c6144a0u); r[8]+=16u;goto L8c6144a2; /* add #0x10,r8 */
L8c6144a2: sa_motion_publish_stop(ctx,0x8c6144a2u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6144a4; /* fmov.s @r4+,fr4 */
L8c6144a4: sa_motion_publish_stop(ctx,0x8c6144a4u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6144a6; /* fmov.s @r4,fr6 */
L8c6144a6: sa_motion_publish_stop(ctx,0x8c6144a6u); fcopy(f,0u,1u);goto L8c6144a8; /* fmov fr1,fr0 */
L8c6144a8: sa_motion_publish_stop(ctx,0x8c6144a8u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6144aa; /* fschg */
L8c6144aa: sa_motion_publish_stop(ctx,0x8c6144aau); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c6144ac; /* mov.w @r6+,r2 */
L8c6144ac: sa_motion_publish_stop(ctx,0x8c6144acu); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c6144ae; /* mov.w @r6,r3 */
L8c6144ae: sa_motion_publish_stop(ctx,0x8c6144aeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6144b0; /* fmov.s fr6,@-r8 */
L8c6144b0: sa_motion_publish_stop(ctx,0x8c6144b0u); *fpul=r[3];goto L8c6144b2; /* lds r3,FPUL */
L8c6144b2: sa_motion_publish_stop(ctx,0x8c6144b2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6144b4; /* fmov.s fr5,@-r8 */
L8c6144b4: sa_motion_publish_stop(ctx,0x8c6144b4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c6144b6; /* float FPUL,fr1 */
L8c6144b6: sa_motion_publish_stop(ctx,0x8c6144b6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6144b8; /* fmov.s fr4,@-r8 */
L8c6144b8: sa_motion_publish_stop(ctx,0x8c6144b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6144ba; /* fmul fr0,fr1 */
L8c6144ba: sa_motion_publish_stop(ctx,0x8c6144bau); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6144bc; /* mov.l r5,@-r8 */
L8c6144bc: sa_motion_publish_stop(ctx,0x8c6144bcu); *fpul=r[2];goto L8c6144be; /* lds r2,FPUL */
L8c6144be: sa_motion_publish_stop(ctx,0x8c6144beu); r[8]+=28u;goto L8c6144c0; /* add #0x1c,r8 */
L8c6144c0: sa_motion_publish_stop(ctx,0x8c6144c0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c6144c2; /* float FPUL,fr2 */
L8c6144c2: sa_motion_publish_stop(ctx,0x8c6144c2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6144c4; /* fldi1 fr3 */
L8c6144c4: sa_motion_publish_stop(ctx,0x8c6144c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6144c6; /* fmul fr0,fr2 */
L8c6144c6: sa_motion_publish_stop(ctx,0x8c6144c6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6144c8; /* fmov.s fr3,@-r8 */
L8c6144c8: sa_motion_publish_stop(ctx,0x8c6144c8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6144ca; /* fmov.s fr1,@-r8 */
L8c6144ca: sa_motion_publish_stop(ctx,0x8c6144cau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6144cc; /* fmov.s fr2,@-r8 */
L8c6144cc: sa_motion_publish_stop(ctx,0x8c6144ccu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6144ce; /* pref @r8 */
L8c6144ce: sa_motion_publish_stop(ctx,0x8c6144ceu); r[8]+=16u;goto L8c6144d0; /* add #0x10,r8 */
L8c6144d0: sa_motion_publish_stop(ctx,0x8c6144d0u); fcopy(f,4u,14u);goto L8c6144d2; /* fmov fr14,fr4 */
L8c6144d2: sa_motion_publish_stop(ctx,0x8c6144d2u); r[0]=16u;goto L8c6144d4; /* mov #0x10,r0 */
L8c6144d4: sa_motion_publish_stop(ctx,0x8c6144d4u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6144d6; /* mov.l @r15,r3 */
L8c6144d6: sa_motion_publish_stop(ctx,0x8c6144d6u); r[4]=r[11];goto L8c6144d8; /* mov r11,r4 */
L8c6144d8: sa_motion_publish_stop(ctx,0x8c6144d8u); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c6144da; /* fmov.s @(r0,r12),fr14 */
L8c6144da: sa_motion_publish_stop(ctx,0x8c6144dau); r[0]=20u;goto L8c6144dc; /* mov #0x14,r0 */
L8c6144dc: sa_motion_publish_stop(ctx,0x8c6144dcu); fcopy(f,5u,13u);goto L8c6144de; /* fmov fr13,fr5 */
L8c6144de: sa_motion_publish_stop(ctx,0x8c6144deu); r[3]+=4294967295u;goto L8c6144e0; /* add #-0x1,r3 */
L8c6144e0: sa_motion_publish_stop(ctx,0x8c6144e0u); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c6144e2; /* fmov.s @(r0,r12),fr13 */
L8c6144e2: sa_motion_publish_stop(ctx,0x8c6144e2u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6144e4; /* tst r3,r3 */
L8c6144e4: sa_motion_publish_stop(ctx,0x8c6144e4u); r[11]=r[12];goto L8c6144e6; /* mov r12,r11 */
L8c6144e6: sa_motion_publish_stop(ctx,0x8c6144e6u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c6144e8; /* fneg fr15 */
L8c6144e8: sa_motion_publish_stop(ctx,0x8c6144eau); take=(*sr&1u);wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;if(take)goto L8c6144f0;goto L8c6144ec; /* bt/s 0x8c6144f0 */
L8c6144ec: sa_motion_publish_stop(ctx,0x8c6144eeu); goto L8c6143b4; /* bra 0x8c6143b4 */
L8c6144f0: sa_motion_publish_stop(ctx,0x8c6144f0u); *sr=(*sr&~1u)|(r[10]>=r[8]);goto L8c6144f2; /* cmp/hs r8,r10 */
L8c6144f2: sa_motion_publish_stop(ctx,0x8c6144f4u); take=(*sr&1u);r[14]+=8u;if(take)goto L8c614540;goto L8c6144f6; /* bt/s 0x8c614540 */
L8c6144f6: sa_motion_publish_stop(ctx,0x8c6144f6u); r[14]+=4294967288u;goto L8c6144f8; /* add #-0x8,r14 */
L8c6144f8: sa_motion_publish_stop(ctx,0x8c6144f8u); r[4]=r[12];goto L8c6144fa; /* mov r12,r4 */
L8c6144fa: sa_motion_publish_stop(ctx,0x8c6144fau); r[6]=r[14];goto L8c6144fc; /* mov r14,r6 */
L8c6144fc: sa_motion_publish_stop(ctx,0x8c6144fcu); r[5]=r[9];goto L8c6144fe; /* mov r9,r5 */
L8c6144fe: sa_motion_publish_stop(ctx,0x8c6144feu); r[14]+=4u;goto L8c614500; /* add #0x4,r14 */
L8c614500: sa_motion_publish_stop(ctx,0x8c614500u); r[4]+=16u;goto L8c614502; /* add #0x10,r4 */
L8c614502: sa_motion_publish_stop(ctx,0x8c614502u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c614504; /* fschg */
L8c614504: sa_motion_publish_stop(ctx,0x8c614504u); r[8]+=16u;goto L8c614506; /* add #0x10,r8 */
L8c614506: sa_motion_publish_stop(ctx,0x8c614506u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614508; /* fmov.s @r4+,fr4 */
L8c614508: sa_motion_publish_stop(ctx,0x8c614508u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61450a; /* fmov.s @r4,fr6 */
L8c61450a: sa_motion_publish_stop(ctx,0x8c61450au); fcopy(f,0u,1u);goto L8c61450c; /* fmov fr1,fr0 */
L8c61450c: sa_motion_publish_stop(ctx,0x8c61450cu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61450e; /* fschg */
L8c61450e: sa_motion_publish_stop(ctx,0x8c61450eu); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c614510; /* mov.w @r6+,r2 */
L8c614510: sa_motion_publish_stop(ctx,0x8c614510u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c614512; /* mov.w @r6,r3 */
L8c614512: sa_motion_publish_stop(ctx,0x8c614512u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c614514; /* fmov.s fr6,@-r8 */
L8c614514: sa_motion_publish_stop(ctx,0x8c614514u); *fpul=r[3];goto L8c614516; /* lds r3,FPUL */
L8c614516: sa_motion_publish_stop(ctx,0x8c614516u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c614518; /* fmov.s fr5,@-r8 */
L8c614518: sa_motion_publish_stop(ctx,0x8c614518u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61451a; /* float FPUL,fr1 */
L8c61451a: sa_motion_publish_stop(ctx,0x8c61451au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61451c; /* fmov.s fr4,@-r8 */
L8c61451c: sa_motion_publish_stop(ctx,0x8c61451cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61451e; /* fmul fr0,fr1 */
L8c61451e: sa_motion_publish_stop(ctx,0x8c61451eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c614520; /* mov.l r5,@-r8 */
L8c614520: sa_motion_publish_stop(ctx,0x8c614520u); *fpul=r[2];goto L8c614522; /* lds r2,FPUL */
L8c614522: sa_motion_publish_stop(ctx,0x8c614522u); r[8]+=28u;goto L8c614524; /* add #0x1c,r8 */
L8c614524: sa_motion_publish_stop(ctx,0x8c614524u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c614526; /* float FPUL,fr2 */
L8c614526: sa_motion_publish_stop(ctx,0x8c614526u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c614528; /* fldi1 fr3 */
L8c614528: sa_motion_publish_stop(ctx,0x8c614528u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61452a; /* fmul fr0,fr2 */
L8c61452a: sa_motion_publish_stop(ctx,0x8c61452au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61452c; /* fmov.s fr3,@-r8 */
L8c61452c: sa_motion_publish_stop(ctx,0x8c61452cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c61452e; /* fmov.s fr1,@-r8 */
L8c61452e: sa_motion_publish_stop(ctx,0x8c61452eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c614530; /* fmov.s fr2,@-r8 */
L8c614530: sa_motion_publish_stop(ctx,0x8c614530u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c614532; /* pref @r8 */
L8c614532: sa_motion_publish_stop(ctx,0x8c614532u); r[8]+=16u;goto L8c614534; /* add #0x10,r8 */
L8c614534: sa_motion_publish_stop(ctx,0x8c614534u); r[3]=r[8];goto L8c614536; /* mov r8,r3 */
L8c614536: sa_motion_publish_stop(ctx,0x8c614536u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614538; /* mov.l @(0x8,r15),r2 */
L8c614538: sa_motion_publish_stop(ctx,0x8c614538u); r[3]-=r[10];goto L8c61453a; /* sub r10,r3 */
L8c61453a: sa_motion_publish_stop(ctx,0x8c61453au); r[3]+=32u;goto L8c61453c; /* add #0x20,r3 */
L8c61453c: sa_motion_publish_stop(ctx,0x8c61453cu); r[2]+=r[3];goto L8c61453e; /* add r3,r2 */
L8c61453e: sa_motion_publish_stop(ctx,0x8c61453eu); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c614540; /* mov.l r2,@(0x8,r15) */
L8c614540: sa_motion_publish_stop(ctx,0x8c614540u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c614542; /* mov.l @(0x10,r15),r3 */
L8c614542: sa_motion_publish_stop(ctx,0x8c614542u); r[3]+=4294967295u;goto L8c614544; /* add #-0x1,r3 */
L8c614544: sa_motion_publish_stop(ctx,0x8c614544u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c614546; /* tst r3,r3 */
L8c614546: sa_motion_publish_stop(ctx,0x8c614548u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x10u,r[3],4);if(io_status)return io_status;if(take)goto L8c61454e;goto L8c61454a; /* bt/s 0x8c61454e */
L8c61454a: sa_motion_publish_stop(ctx,0x8c61454cu); goto L8c614360; /* bra 0x8c614360 */
L8c61454e: sa_motion_publish_stop(ctx,0x8c61454eu); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c614550; /* mov.l @(0x8,r15),r3 */
L8c614550: sa_motion_publish_stop(ctx,0x8c614550u); r[4]=rd(m,bus,&io_status,0x8c6145bcu,4);if(io_status)return io_status;goto L8c614552; /* mov.l 0x8c6145bc,r4 */
L8c614552: sa_motion_publish_stop(ctx,0x8c614552u); r[3]>>=2;goto L8c614554; /* shlr2 r3 */
L8c614554: sa_motion_publish_stop(ctx,0x8c614554u); r[1]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614556; /* mov.l @r4,r1 */
L8c614556: sa_motion_publish_stop(ctx,0x8c614556u); r[3]>>=2;goto L8c614558; /* shlr2 r3 */
L8c614558: sa_motion_publish_stop(ctx,0x8c614558u); *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1;goto L8c61455a; /* shlr r3 */
L8c61455a: sa_motion_publish_stop(ctx,0x8c61455au); r[1]+=r[3];goto L8c61455c; /* add r3,r1 */
L8c61455c: sa_motion_publish_stop(ctx,0x8c61455cu); wr(m,bus,&io_status,r[4],r[1],4);if(io_status)return io_status;goto L8c61455e; /* mov.l r1,@r4 */
L8c61455e: sa_motion_publish_stop(ctx,0x8c61455eu); r[4]=rd(m,bus,&io_status,0x8c6145c0u,4);if(io_status)return io_status;goto L8c614560; /* mov.l 0x8c6145c0,r4 */
L8c614560: sa_motion_publish_stop(ctx,0x8c614560u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c614562; /* mov.l @(0xc,r15),r3 */
L8c614562: sa_motion_publish_stop(ctx,0x8c614562u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c614564; /* mov.l @r4,r2 */
L8c614564: sa_motion_publish_stop(ctx,0x8c614564u); r[2]+=r[3];goto L8c614566; /* add r3,r2 */
L8c614566: sa_motion_publish_stop(ctx,0x8c614566u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c614568; /* mov.l r2,@r4 */
L8c614568: sa_motion_publish_stop(ctx,0x8c614568u); r[15]+=20u;goto L8c61456a; /* add #0x14,r15 */
L8c61456a: sa_motion_publish_stop(ctx,0x8c61456au); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61456c; /* lds.l @r15+,PR */
L8c61456c: sa_motion_publish_stop(ctx,0x8c61456cu); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61456e; /* fmov.s @r15+,fr13 */
L8c61456e: sa_motion_publish_stop(ctx,0x8c61456eu); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614570; /* fmov.s @r15+,fr14 */
L8c614570: sa_motion_publish_stop(ctx,0x8c614570u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c614572; /* fmov.s @r15+,fr15 */
L8c614572: sa_motion_publish_stop(ctx,0x8c614572u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614574; /* mov.l @r15+,r9 */
L8c614574: sa_motion_publish_stop(ctx,0x8c614574u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614576; /* mov.l @r15+,r10 */
L8c614576: sa_motion_publish_stop(ctx,0x8c614576u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c614578; /* mov.l @r15+,r11 */
L8c614578: sa_motion_publish_stop(ctx,0x8c614578u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61457a; /* mov.l @r15+,r12 */
L8c61457a: sa_motion_publish_stop(ctx,0x8c61457au); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61457c; /* mov.l @r15+,r13 */
L8c61457c: sa_motion_publish_stop(ctx,0x8c61457eu); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_uv_constant_alternate_strip_clipped_8c616068(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c616068;
L8c616068: sa_motion_publish_stop(ctx,0x8c616068u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61606a; /* mov.l r14,@-r15 */
L8c61606a: sa_motion_publish_stop(ctx,0x8c61606au); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61606c; /* mov.l r13,@-r15 */
L8c61606c: sa_motion_publish_stop(ctx,0x8c61606cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61606e; /* mov.l r12,@-r15 */
L8c61606e: sa_motion_publish_stop(ctx,0x8c61606eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c616070; /* mov.l r11,@-r15 */
L8c616070: sa_motion_publish_stop(ctx,0x8c616070u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c616072; /* mov.l r10,@-r15 */
L8c616072: sa_motion_publish_stop(ctx,0x8c616072u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c616074; /* mov.l r9,@-r15 */
L8c616074: sa_motion_publish_stop(ctx,0x8c616074u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c616076; /* fmov.s fr15,@-r15 */
L8c616076: sa_motion_publish_stop(ctx,0x8c616076u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c616078; /* fmov.s fr14,@-r15 */
L8c616078: sa_motion_publish_stop(ctx,0x8c616078u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61607a; /* fmov.s fr13,@-r15 */
L8c61607a: sa_motion_publish_stop(ctx,0x8c61607au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61607c; /* fmov.s fr12,@-r15 */
L8c61607c: sa_motion_publish_stop(ctx,0x8c61607cu); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61607e; /* sts.l PR,@-r15 */
L8c61607e: sa_motion_publish_stop(ctx,0x8c61607eu); r[15]+=4294967216u;goto L8c616080; /* add #-0x50,r15 */
L8c616080: sa_motion_publish_stop(ctx,0x8c616080u); r[3]=rd(m,bus,&io_status,0x8c6160a0u,4);if(io_status)return io_status;goto L8c616082; /* mov.l 0x8c6160a0,r3 */
L8c616082: sa_motion_publish_stop(ctx,0x8c616082u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][12]=0x3f800000u;goto L8c616084; /* fldi1 fr12 */
L8c616084: sa_motion_publish_stop(ctx,0x8c616086u); target=r[3];*pr=0x8c616088u;r[12]=r[4];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c616088; /* jsr @r3 */
L8c616088: sa_motion_publish_stop(ctx,0x8c616088u); r[2]=rd(m,bus,&io_status,0x8c6160a4u,4);if(io_status)return io_status;goto L8c61608a; /* mov.l 0x8c6160a4,r2 */
L8c61608a: sa_motion_publish_stop(ctx,0x8c61608au); r[1]=0u;goto L8c61608c; /* mov #0x0,r1 */
L8c61608c: sa_motion_publish_stop(ctx,0x8c61608cu); r[0]=sx(m,bus,&io_status,r[12]+0x2u,2);if(io_status)return io_status;goto L8c61608e; /* mov.w @(0x2,r12),r0 */
L8c61608e: sa_motion_publish_stop(ctx,0x8c61608eu); r[13]=rd(m,bus,&io_status,r[12]+0x4u,4);if(io_status)return io_status;goto L8c616090; /* mov.l @(0x4,r12),r13 */
L8c616090: sa_motion_publish_stop(ctx,0x8c616090u); r[14]=rd(m,bus,&io_status,r[12]+0x14u,4);if(io_status)return io_status;goto L8c616092; /* mov.l @(0x14,r12),r14 */
L8c616092: sa_motion_publish_stop(ctx,0x8c616092u); r[0]=r[0]&65535u;goto L8c616094; /* extu.w r0,r0 */
L8c616094: sa_motion_publish_stop(ctx,0x8c616094u); wr(m,bus,&io_status,r[15]+0x1cu,r[0],4);if(io_status)return io_status;goto L8c616096; /* mov.l r0,@(0x1c,r15) */
L8c616096: sa_motion_publish_stop(ctx,0x8c616096u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c616098; /* mov.l @r2,r3 */
L8c616098: sa_motion_publish_stop(ctx,0x8c616098u); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c61609a; /* mov.l r3,@(0xc,r15) */
L8c61609a: sa_motion_publish_stop(ctx,0x8c61609au); wr(m,bus,&io_status,r[15]+0x10u,r[1],4);if(io_status)return io_status;goto L8c61609c; /* mov.l r1,@(0x10,r15) */
L8c61609c: sa_motion_publish_stop(ctx,0x8c61609eu); goto L8c6160a8; /* bra 0x8c6160a8 */
L8c6160a8: sa_motion_publish_stop(ctx,0x8c6160a8u); r[3]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6160aa; /* mov.w @r13+,r3 */
L8c6160aa: sa_motion_publish_stop(ctx,0x8c6160aau); r[4]=rd(m,bus,&io_status,0x8c6161c8u,4);if(io_status)return io_status;goto L8c6160ac; /* mov.l 0x8c6161c8,r4 */
L8c6160ac: sa_motion_publish_stop(ctx,0x8c6160acu); r[3]=r[3]&65535u;goto L8c6160ae; /* extu.w r3,r3 */
L8c6160ae: sa_motion_publish_stop(ctx,0x8c6160aeu); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c6160b0; /* tst r4,r3 */
L8c6160b0: sa_motion_publish_stop(ctx,0x8c6160b0u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6160b2; /* mov.l r3,@(0x8,r15) */
L8c6160b2: sa_motion_publish_stop(ctx,0x8c6160b4u); take=(*sr&1u);fcopy(f,15u,12u);if(take)goto L8c6160c4;goto L8c6160b6; /* bt/s 0x8c6160c4 */
L8c6160b6: sa_motion_publish_stop(ctx,0x8c6160b6u); r[4]=sx(m,bus,&io_status,0x8c6161c4u,2);if(io_status)return io_status;goto L8c6160b8; /* mov.w 0x8c6161c4,r4 */
L8c6160b8: sa_motion_publish_stop(ctx,0x8c6160b8u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6160ba; /* mov.l @(0x8,r15),r3 */
L8c6160ba: sa_motion_publish_stop(ctx,0x8c6160bau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c6160bc; /* fldi1 fr4 */
L8c6160bc: sa_motion_publish_stop(ctx,0x8c6160bcu); r[3]&=r[4];goto L8c6160be; /* and r4,r3 */
L8c6160be: sa_motion_publish_stop(ctx,0x8c6160beu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]^=0x80000000u;goto L8c6160c0; /* fneg fr4 */
L8c6160c0: sa_motion_publish_stop(ctx,0x8c6160c0u); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6160c2; /* mov.l r3,@(0x8,r15) */
L8c6160c2: sa_motion_publish_stop(ctx,0x8c6160c2u); fcopy(f,15u,4u);goto L8c6160c4; /* fmov fr4,fr15 */
L8c6160c4: sa_motion_publish_stop(ctx,0x8c6160c4u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6160c6; /* mov.l @(0x8,r15),r3 */
L8c6160c6: sa_motion_publish_stop(ctx,0x8c6160c6u); r[0]=16u;goto L8c6160c8; /* mov #0x10,r0 */
L8c6160c8: sa_motion_publish_stop(ctx,0x8c6160c8u); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6160ca; /* mov.l @(0x10,r15),r2 */
L8c6160ca: sa_motion_publish_stop(ctx,0x8c6160cau); r[9]=r[8];goto L8c6160cc; /* mov r8,r9 */
L8c6160cc: sa_motion_publish_stop(ctx,0x8c6160ccu); r[2]+=r[3];goto L8c6160ce; /* add r3,r2 */
L8c6160ce: sa_motion_publish_stop(ctx,0x8c6160ceu); wr(m,bus,&io_status,r[15]+0x10u,r[2],4);if(io_status)return io_status;goto L8c6160d0; /* mov.l r2,@(0x10,r15) */
L8c6160d0: sa_motion_publish_stop(ctx,0x8c6160d0u); r[1]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6160d2; /* mov.l @(0x8,r15),r1 */
L8c6160d2: sa_motion_publish_stop(ctx,0x8c6160d2u); r[1]+=4294967294u;goto L8c6160d4; /* add #-0x2,r1 */
L8c6160d4: sa_motion_publish_stop(ctx,0x8c6160d4u); wr(m,bus,&io_status,r[15]+0x8u,r[1],4);if(io_status)return io_status;goto L8c6160d6; /* mov.l r1,@(0x8,r15) */
L8c6160d6: sa_motion_publish_stop(ctx,0x8c6160d6u); r[10]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6160d8; /* mov.w @r13+,r10 */
L8c6160d8: sa_motion_publish_stop(ctx,0x8c6160d8u); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6160da; /* mov.l @(0xc,r15),r3 */
L8c6160da: sa_motion_publish_stop(ctx,0x8c6160dau); r[11]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6160dc; /* mov.w @r13+,r11 */
L8c6160dc: sa_motion_publish_stop(ctx,0x8c6160dcu); r[10]=r[10]&65535u;goto L8c6160de; /* extu.w r10,r10 */
L8c6160de: sa_motion_publish_stop(ctx,0x8c6160deu); r[10]<<=2;goto L8c6160e0; /* shll2 r10 */
L8c6160e0: sa_motion_publish_stop(ctx,0x8c6160e0u); r[11]=r[11]&65535u;goto L8c6160e2; /* extu.w r11,r11 */
L8c6160e2: sa_motion_publish_stop(ctx,0x8c6160e2u); r[11]<<=2;goto L8c6160e4; /* shll2 r11 */
L8c6160e4: sa_motion_publish_stop(ctx,0x8c6160e4u); r[10]<<=2;goto L8c6160e6; /* shll2 r10 */
L8c6160e6: sa_motion_publish_stop(ctx,0x8c6160e6u); r[11]<<=2;goto L8c6160e8; /* shll2 r11 */
L8c6160e8: sa_motion_publish_stop(ctx,0x8c6160e8u); *sr=(*sr&~1u)|(r[10]>>31);r[10]<<=1;goto L8c6160ea; /* shll r10 */
L8c6160ea: sa_motion_publish_stop(ctx,0x8c6160eau); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c6160ec; /* shll r11 */
L8c6160ec: sa_motion_publish_stop(ctx,0x8c6160ecu); r[11]+=r[3];goto L8c6160ee; /* add r3,r11 */
L8c6160ee: sa_motion_publish_stop(ctx,0x8c6160eeu); r[10]+=r[3];goto L8c6160f0; /* add r3,r10 */
L8c6160f0: sa_motion_publish_stop(ctx,0x8c6160f0u); fload(m,bus,&io_status,f,13u,r[0]+r[11]);if(io_status)return io_status;goto L8c6160f2; /* fmov.s @(r0,r11),fr13 */
L8c6160f2: sa_motion_publish_stop(ctx,0x8c6160f2u); fload(m,bus,&io_status,f,5u,r[0]+r[10]);if(io_status)return io_status;goto L8c6160f4; /* fmov.s @(r0,r10),fr5 */
L8c6160f4: sa_motion_publish_stop(ctx,0x8c6160f4u); r[0]=20u;goto L8c6160f6; /* mov #0x14,r0 */
L8c6160f6: sa_motion_publish_stop(ctx,0x8c6160f6u); fload(m,bus,&io_status,f,14u,r[0]+r[11]);if(io_status)return io_status;goto L8c6160f8; /* fmov.s @(r0,r11),fr14 */
L8c6160f8: sa_motion_publish_stop(ctx,0x8c6160f8u); fload(m,bus,&io_status,f,6u,r[0]+r[10]);if(io_status)return io_status;goto L8c6160fa; /* fmov.s @(r0,r10),fr6 */
L8c6160fa: sa_motion_publish_stop(ctx,0x8c6160fau); r[12]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6160fc; /* mov.w @r13+,r12 */
L8c6160fc: sa_motion_publish_stop(ctx,0x8c6160fcu); r[3]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6160fe; /* mov.l @(0xc,r15),r3 */
L8c6160fe: sa_motion_publish_stop(ctx,0x8c6160feu); r[2]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c616100; /* mov.w @r13,r2 */
L8c616100: sa_motion_publish_stop(ctx,0x8c616100u); r[12]=r[12]&65535u;goto L8c616102; /* extu.w r12,r12 */
L8c616102: sa_motion_publish_stop(ctx,0x8c616102u); r[12]<<=2;goto L8c616104; /* shll2 r12 */
L8c616104: sa_motion_publish_stop(ctx,0x8c616104u); r[2]=r[2]&65535u;goto L8c616106; /* extu.w r2,r2 */
L8c616106: sa_motion_publish_stop(ctx,0x8c616106u); r[2]<<=2;goto L8c616108; /* shll2 r2 */
L8c616108: sa_motion_publish_stop(ctx,0x8c616108u); r[2]<<=2;goto L8c61610a; /* shll2 r2 */
L8c61610a: sa_motion_publish_stop(ctx,0x8c61610au); r[12]<<=2;goto L8c61610c; /* shll2 r12 */
L8c61610c: sa_motion_publish_stop(ctx,0x8c61610cu); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61610e; /* shll r12 */
L8c61610e: sa_motion_publish_stop(ctx,0x8c61610eu); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c616110; /* shll r2 */
L8c616110: sa_motion_publish_stop(ctx,0x8c616110u); r[2]+=r[3];goto L8c616112; /* add r3,r2 */
L8c616112: sa_motion_publish_stop(ctx,0x8c616112u); r[12]+=r[3];goto L8c616114; /* add r3,r12 */
L8c616114: sa_motion_publish_stop(ctx,0x8c616114u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[2],1u);if(status)return status;goto L8c616116; /* pref @r2 */
L8c616116: sa_motion_publish_stop(ctx,0x8c616116u); r[3]=0u;goto L8c616118; /* mov #0x0,r3 */
L8c616118: sa_motion_publish_stop(ctx,0x8c616118u); wr(m,bus,&io_status,r[15]+0x18u,r[3],4);if(io_status)return io_status;goto L8c61611a; /* mov.l r3,@(0x18,r15) */
L8c61611a: sa_motion_publish_stop(ctx,0x8c61611au); r[4]=63u;goto L8c61611c; /* mov #0x3f,r4 */
L8c61611c: sa_motion_publish_stop(ctx,0x8c61611cu); r[2]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c61611e; /* mov.l @(0xc,r10),r2 */
L8c61611e: sa_motion_publish_stop(ctx,0x8c61611eu); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c616120; /* mov.l @(0xc,r11),r3 */
L8c616120: sa_motion_publish_stop(ctx,0x8c616120u); r[1]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c616122; /* mov.l @(0xc,r12),r1 */
L8c616122: sa_motion_publish_stop(ctx,0x8c616122u); r[2]&=r[3];goto L8c616124; /* and r3,r2 */
L8c616124: sa_motion_publish_stop(ctx,0x8c616124u); r[2]&=r[1];goto L8c616126; /* and r1,r2 */
L8c616126: sa_motion_publish_stop(ctx,0x8c616126u); r[4]&=r[2];goto L8c616128; /* and r2,r4 */
L8c616128: sa_motion_publish_stop(ctx,0x8c616128u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c61612a; /* tst r4,r4 */
L8c61612a: sa_motion_publish_stop(ctx,0x8c61612cu); take=(*sr&1u);fcopy(f,4u,12u);if(take)goto L8c616132;goto L8c61612e; /* bt/s 0x8c616132 */
L8c61612e: sa_motion_publish_stop(ctx,0x8c616130u); goto L8c61628c; /* bra 0x8c61628c */
L8c616132: sa_motion_publish_stop(ctx,0x8c616132u); r[4]=rd(m,bus,&io_status,r[10]+0xcu,4);if(io_status)return io_status;goto L8c616134; /* mov.l @(0xc,r10),r4 */
L8c616134: sa_motion_publish_stop(ctx,0x8c616134u); r[5]=1u;goto L8c616136; /* mov #0x1,r5 */
L8c616136: sa_motion_publish_stop(ctx,0x8c616136u); r[3]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c616138; /* mov.l @(0xc,r11),r3 */
L8c616138: sa_motion_publish_stop(ctx,0x8c616138u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61613a; /* mov.l @(0xc,r12),r2 */
L8c61613a: sa_motion_publish_stop(ctx,0x8c61613au); r[4]|=r[3];goto L8c61613c; /* or r3,r4 */
L8c61613c: sa_motion_publish_stop(ctx,0x8c61613cu); r[4]|=r[2];goto L8c61613e; /* or r2,r4 */
L8c61613e: sa_motion_publish_stop(ctx,0x8c61613eu); r[4]&=r[5];goto L8c616140; /* and r5,r4 */
L8c616140: sa_motion_publish_stop(ctx,0x8c616140u); *sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c616142; /* tst r4,r4 */
L8c616142: sa_motion_publish_stop(ctx,0x8c616142u); if(!(*sr&1u))goto L8c616148;goto L8c616144; /* bf 0x8c616148 */
L8c616144: sa_motion_publish_stop(ctx,0x8c616146u); goto L8c61626e; /* bra 0x8c61626e */
L8c616148: sa_motion_publish_stop(ctx,0x8c616148u); r[7]=r[15];goto L8c61614a; /* mov r15,r7 */
L8c61614a: sa_motion_publish_stop(ctx,0x8c61614au); r[7]+=68u;goto L8c61614c; /* add #0x44,r7 */
L8c61614c: sa_motion_publish_stop(ctx,0x8c61614cu); wr(m,bus,&io_status,r[7],r[10],4);if(io_status)return io_status;goto L8c61614e; /* mov.l r10,@r7 */
L8c61614e: sa_motion_publish_stop(ctx,0x8c61614eu); wr(m,bus,&io_status,r[7]+0x4u,r[11],4);if(io_status)return io_status;goto L8c616150; /* mov.l r11,@(0x4,r7) */
L8c616150: sa_motion_publish_stop(ctx,0x8c616150u); wr(m,bus,&io_status,r[7]+0x8u,r[12],4);if(io_status)return io_status;goto L8c616152; /* mov.l r12,@(0x8,r7) */
L8c616152: sa_motion_publish_stop(ctx,0x8c616154u); r[6]=0u;goto L8c616200; /* bra 0x8c616200 */
L8c616156: sa_motion_publish_stop(ctx,0x8c616156u); r[2]=r[6];goto L8c616158; /* mov r6,r2 */
L8c616158: sa_motion_publish_stop(ctx,0x8c616158u); r[1]=r[6];goto L8c61615a; /* mov r6,r1 */
L8c61615a: sa_motion_publish_stop(ctx,0x8c61615au); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c61615c; /* shll r2 */
L8c61615c: sa_motion_publish_stop(ctx,0x8c61615cu); r[3]=r[6];goto L8c61615e; /* mov r6,r3 */
L8c61615e: sa_motion_publish_stop(ctx,0x8c61615eu); r[3]<<=2;goto L8c616160; /* shll2 r3 */
L8c616160: sa_motion_publish_stop(ctx,0x8c616160u); r[2]+=r[1];goto L8c616162; /* add r1,r2 */
L8c616162: sa_motion_publish_stop(ctx,0x8c616162u); r[2]<<=2;goto L8c616164; /* shll2 r2 */
L8c616164: sa_motion_publish_stop(ctx,0x8c616164u); r[3]+=r[7];goto L8c616166; /* add r7,r3 */
L8c616166: sa_motion_publish_stop(ctx,0x8c616166u); r[2]+=r[4];goto L8c616168; /* add r4,r2 */
L8c616168: sa_motion_publish_stop(ctx,0x8c616168u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61616a; /* mov.l r3,@(0x4,r15) */
L8c61616a: sa_motion_publish_stop(ctx,0x8c61616au); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61616c; /* mov.l r2,@r15 */
L8c61616c: sa_motion_publish_stop(ctx,0x8c61616cu); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61616e; /* mov.l @r3,r1 */
L8c61616e: sa_motion_publish_stop(ctx,0x8c61616eu); r[2]=rd(m,bus,&io_status,r[1]+0xcu,4);if(io_status)return io_status;goto L8c616170; /* mov.l @(0xc,r1),r2 */
L8c616170: sa_motion_publish_stop(ctx,0x8c616170u); *sr=(*sr&~1u)|!(r[5]&r[2]);goto L8c616172; /* tst r5,r2 */
L8c616172: sa_motion_publish_stop(ctx,0x8c616174u); take=!(*sr&1u);r[6]+=1u;if(take)goto L8c6161d8;goto L8c616176; /* bf/s 0x8c6161d8 */
L8c616176: sa_motion_publish_stop(ctx,0x8c616176u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c616178; /* mov.l @(0x4,r15),r1 */
L8c616178: sa_motion_publish_stop(ctx,0x8c616178u); r[0]=24u;goto L8c61617a; /* mov #0x18,r0 */
L8c61617a: sa_motion_publish_stop(ctx,0x8c61617au); fcopy(f,4u,12u);goto L8c61617c; /* fmov fr12,fr4 */
L8c61617c: sa_motion_publish_stop(ctx,0x8c61617cu); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61617e; /* mov.l @r1,r3 */
L8c61617e: sa_motion_publish_stop(ctx,0x8c61617eu); r[4]=rd(m,bus,&io_status,0x8c6161ccu,4);if(io_status)return io_status;goto L8c616180; /* mov.l 0x8c6161cc,r4 */
L8c616180: sa_motion_publish_stop(ctx,0x8c616180u); wr(m,bus,&io_status,r[15]+0x14u,r[3],4);if(io_status)return io_status;goto L8c616182; /* mov.l r3,@(0x14,r15) */
L8c616182: sa_motion_publish_stop(ctx,0x8c616182u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616184; /* fmov.s @(r0,r3),fr3 */
L8c616184: sa_motion_publish_stop(ctx,0x8c616184u); r[0]=12u;goto L8c616186; /* mov #0xc,r0 */
L8c616186: sa_motion_publish_stop(ctx,0x8c616186u); r[2]=rd(m,bus,&io_status,r[15]+0x14u,4);if(io_status)return io_status;goto L8c616188; /* mov.l @(0x14,r15),r2 */
L8c616188: sa_motion_publish_stop(ctx,0x8c616188u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61618a; /* fdiv fr3,fr4 */
L8c61618a: sa_motion_publish_stop(ctx,0x8c61618au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61618c; /* fmov.s @(r0,r4),fr3 */
L8c61618c: sa_motion_publish_stop(ctx,0x8c61618cu); r[0]=16u;goto L8c61618e; /* mov #0x10,r0 */
L8c61618e: sa_motion_publish_stop(ctx,0x8c61618eu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c616190; /* mov.l @r15,r3 */
L8c616190: sa_motion_publish_stop(ctx,0x8c616190u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c616192; /* fmov.s @(r0,r2),fr2 */
L8c616192: sa_motion_publish_stop(ctx,0x8c616192u); r[0]=16u;goto L8c616194; /* mov #0x10,r0 */
L8c616194: sa_motion_publish_stop(ctx,0x8c616194u); r[2]=rd(m,bus,&io_status,0x8c6161d0u,4);if(io_status)return io_status;goto L8c616196; /* mov.l 0x8c6161d0,r2 */
L8c616196: sa_motion_publish_stop(ctx,0x8c616196u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616198; /* fsub fr3,fr2 */
L8c616198: sa_motion_publish_stop(ctx,0x8c616198u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61619a; /* fmov.s @r2,fr3 */
L8c61619a: sa_motion_publish_stop(ctx,0x8c61619au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61619c; /* fmul fr4,fr2 */
L8c61619c: sa_motion_publish_stop(ctx,0x8c61619cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61619e; /* fdiv fr3,fr2 */
L8c61619e: sa_motion_publish_stop(ctx,0x8c61619eu); fstore(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c6161a0; /* fmov.s fr2,@r3 */
L8c6161a0: sa_motion_publish_stop(ctx,0x8c6161a0u); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6161a2; /* mov.l @(0x4,r15),r1 */
L8c6161a2: sa_motion_publish_stop(ctx,0x8c6161a2u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6161a4; /* fmov.s @(r0,r4),fr3 */
L8c6161a4: sa_motion_publish_stop(ctx,0x8c6161a4u); r[0]=20u;goto L8c6161a6; /* mov #0x14,r0 */
L8c6161a6: sa_motion_publish_stop(ctx,0x8c6161a6u); r[1]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c6161a8; /* mov.l @r1,r1 */
L8c6161a8: sa_motion_publish_stop(ctx,0x8c6161a8u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6161aa; /* mov.l @r15,r3 */
L8c6161aa: sa_motion_publish_stop(ctx,0x8c6161aau); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c6161ac; /* fmov.s @(r0,r1),fr2 */
L8c6161ac: sa_motion_publish_stop(ctx,0x8c6161acu); r[1]=rd(m,bus,&io_status,0x8c6161d4u,4);if(io_status)return io_status;goto L8c6161ae; /* mov.l 0x8c6161d4,r1 */
L8c6161ae: sa_motion_publish_stop(ctx,0x8c6161aeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6161b0; /* fsub fr3,fr2 */
L8c6161b0: sa_motion_publish_stop(ctx,0x8c6161b0u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6161b2; /* fmov.s @r1,fr3 */
L8c6161b2: sa_motion_publish_stop(ctx,0x8c6161b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6161b4; /* fmul fr4,fr2 */
L8c6161b4: sa_motion_publish_stop(ctx,0x8c6161b4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6161b6; /* fdiv fr3,fr2 */
L8c6161b6: sa_motion_publish_stop(ctx,0x8c6161b6u); r[0]=4u;goto L8c6161b8; /* mov #0x4,r0 */
L8c6161b8: sa_motion_publish_stop(ctx,0x8c6161b8u); fstore(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c6161ba; /* fmov fr2,@(r0,r3) */
L8c6161ba: sa_motion_publish_stop(ctx,0x8c6161bau); r[0]=8u;goto L8c6161bc; /* mov #0x8,r0 */
L8c6161bc: sa_motion_publish_stop(ctx,0x8c6161bcu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6161be; /* mov.l @r15,r3 */
L8c6161be: sa_motion_publish_stop(ctx,0x8c6161beu); fstore(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c6161c0; /* fmov fr4,@(r0,r3) */
L8c6161c0: sa_motion_publish_stop(ctx,0x8c6161c2u); goto L8c616200; /* bra 0x8c616200 */
L8c6161d8: sa_motion_publish_stop(ctx,0x8c6161d8u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6161da; /* mov.l @(0x4,r15),r3 */
L8c6161da: sa_motion_publish_stop(ctx,0x8c6161dau); r[0]=16u;goto L8c6161dc; /* mov #0x10,r0 */
L8c6161dc: sa_motion_publish_stop(ctx,0x8c6161dcu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6161de; /* mov.l @r15,r2 */
L8c6161de: sa_motion_publish_stop(ctx,0x8c6161deu); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6161e0; /* mov.l @r3,r1 */
L8c6161e0: sa_motion_publish_stop(ctx,0x8c6161e0u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6161e2; /* fmov.s @(r0,r1),fr3 */
L8c6161e2: sa_motion_publish_stop(ctx,0x8c6161e2u); r[0]=20u;goto L8c6161e4; /* mov #0x14,r0 */
L8c6161e4: sa_motion_publish_stop(ctx,0x8c6161e4u); fstore(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6161e6; /* fmov.s fr3,@r2 */
L8c6161e6: sa_motion_publish_stop(ctx,0x8c6161e6u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6161e8; /* mov.l @(0x4,r15),r2 */
L8c6161e8: sa_motion_publish_stop(ctx,0x8c6161e8u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6161ea; /* mov.l @r15,r3 */
L8c6161ea: sa_motion_publish_stop(ctx,0x8c6161eau); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6161ec; /* mov.l @r2,r1 */
L8c6161ec: sa_motion_publish_stop(ctx,0x8c6161ecu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6161ee; /* fmov.s @(r0,r1),fr3 */
L8c6161ee: sa_motion_publish_stop(ctx,0x8c6161eeu); r[0]=4u;goto L8c6161f0; /* mov #0x4,r0 */
L8c6161f0: sa_motion_publish_stop(ctx,0x8c6161f0u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c6161f2; /* fmov fr3,@(r0,r3) */
L8c6161f2: sa_motion_publish_stop(ctx,0x8c6161f2u); r[0]=24u;goto L8c6161f4; /* mov #0x18,r0 */
L8c6161f4: sa_motion_publish_stop(ctx,0x8c6161f4u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6161f6; /* mov.l @(0x4,r15),r2 */
L8c6161f6: sa_motion_publish_stop(ctx,0x8c6161f6u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6161f8; /* mov.l @r15,r3 */
L8c6161f8: sa_motion_publish_stop(ctx,0x8c6161f8u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6161fa; /* mov.l @r2,r1 */
L8c6161fa: sa_motion_publish_stop(ctx,0x8c6161fau); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c6161fc; /* fmov.s @(r0,r1),fr3 */
L8c6161fc: sa_motion_publish_stop(ctx,0x8c6161fcu); r[0]=8u;goto L8c6161fe; /* mov #0x8,r0 */
L8c6161fe: sa_motion_publish_stop(ctx,0x8c6161feu); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c616200; /* fmov fr3,@(r0,r3) */
L8c616200: sa_motion_publish_stop(ctx,0x8c616200u); r[3]=3u;goto L8c616202; /* mov #0x3,r3 */
L8c616202: sa_motion_publish_stop(ctx,0x8c616202u); r[4]=r[15];goto L8c616204; /* mov r15,r4 */
L8c616204: sa_motion_publish_stop(ctx,0x8c616204u); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]);goto L8c616206; /* cmp/ge r3,r6 */
L8c616206: sa_motion_publish_stop(ctx,0x8c616208u); take=!(*sr&1u);r[4]+=32u;if(take)goto L8c616156;goto L8c61620a; /* bf/s 0x8c616156 */
L8c61620a: sa_motion_publish_stop(ctx,0x8c61620au); r[7]=r[4];goto L8c61620c; /* mov r4,r7 */
L8c61620c: sa_motion_publish_stop(ctx,0x8c61620cu); r[0]=8u;goto L8c61620e; /* mov #0x8,r0 */
L8c61620e: sa_motion_publish_stop(ctx,0x8c61620eu); r[3]=r[4];goto L8c616210; /* mov r4,r3 */
L8c616210: sa_motion_publish_stop(ctx,0x8c616210u); fload(m,bus,&io_status,f,8u,r[0]+r[7]);if(io_status)return io_status;goto L8c616212; /* fmov.s @(r0,r7),fr8 */
L8c616212: sa_motion_publish_stop(ctx,0x8c616212u); r[3]+=12u;goto L8c616214; /* add #0xc,r3 */
L8c616214: sa_motion_publish_stop(ctx,0x8c616214u); r[6]=r[4];goto L8c616216; /* mov r4,r6 */
L8c616216: sa_motion_publish_stop(ctx,0x8c616216u); r[0]=4u;goto L8c616218; /* mov #0x4,r0 */
L8c616218: sa_motion_publish_stop(ctx,0x8c616218u); fcopy(f,0u,8u);goto L8c61621a; /* fmov fr8,fr0 */
L8c61621a: sa_motion_publish_stop(ctx,0x8c61621au); r[6]+=24u;goto L8c61621c; /* add #0x18,r6 */
L8c61621c: sa_motion_publish_stop(ctx,0x8c61621cu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61621e; /* mov.l r3,@r15 */
L8c61621e: sa_motion_publish_stop(ctx,0x8c61621eu); fload(m,bus,&io_status,f,9u,r[0]+r[7]);if(io_status)return io_status;goto L8c616220; /* fmov.s @(r0,r7),fr9 */
L8c616220: sa_motion_publish_stop(ctx,0x8c616220u); fload(m,bus,&io_status,f,4u,r[0]+r[3]);if(io_status)return io_status;goto L8c616222; /* fmov.s @(r0,r3),fr4 */
L8c616222: sa_motion_publish_stop(ctx,0x8c616222u); r[0]=8u;goto L8c616224; /* mov #0x8,r0 */
L8c616224: sa_motion_publish_stop(ctx,0x8c616224u); fload(m,bus,&io_status,f,6u,r[0]+r[3]);if(io_status)return io_status;goto L8c616226; /* fmov.s @(r0,r3),fr6 */
L8c616226: sa_motion_publish_stop(ctx,0x8c616226u); r[0]=4u;goto L8c616228; /* mov #0x4,r0 */
L8c616228: sa_motion_publish_stop(ctx,0x8c616228u); fcopy(f,3u,9u);goto L8c61622a; /* fmov fr9,fr3 */
L8c61622a: sa_motion_publish_stop(ctx,0x8c61622au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61622c; /* fmul fr4,fr0 */
L8c61622c: sa_motion_publish_stop(ctx,0x8c61622cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[6],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61622e; /* fmul fr6,fr3 */
L8c61622e: sa_motion_publish_stop(ctx,0x8c61622eu); fload(m,bus,&io_status,f,7u,r[7]);if(io_status)return io_status;goto L8c616230; /* fmov.s @r7,fr7 */
L8c616230: sa_motion_publish_stop(ctx,0x8c616230u); fload(m,bus,&io_status,f,5u,r[3]);if(io_status)return io_status;goto L8c616232; /* fmov.s @r3,fr5 */
L8c616232: sa_motion_publish_stop(ctx,0x8c616232u); fcopy(f,2u,7u);goto L8c616234; /* fmov fr7,fr2 */
L8c616234: sa_motion_publish_stop(ctx,0x8c616234u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[6],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616236; /* fmul fr6,fr2 */
L8c616236: sa_motion_publish_stop(ctx,0x8c616236u); fcopy(f,1u,9u);goto L8c616238; /* fmov fr9,fr1 */
L8c616238: sa_motion_publish_stop(ctx,0x8c616238u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c61623a; /* fsub fr3,fr0 */
L8c61623a: sa_motion_publish_stop(ctx,0x8c61623au); fcopy(f,3u,8u);goto L8c61623c; /* fmov fr8,fr3 */
L8c61623c: sa_motion_publish_stop(ctx,0x8c61623cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61623e; /* fmul fr5,fr3 */
L8c61623e: sa_motion_publish_stop(ctx,0x8c61623eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616240; /* fsub fr3,fr2 */
L8c616240: sa_motion_publish_stop(ctx,0x8c616240u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c616242; /* fmov.s @(r0,r6),fr3 */
L8c616242: sa_motion_publish_stop(ctx,0x8c616242u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616244; /* fmul fr2,fr3 */
L8c616244: sa_motion_publish_stop(ctx,0x8c616244u); fload(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c616246; /* fmov.s @r6,fr2 */
L8c616246: sa_motion_publish_stop(ctx,0x8c616246u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616248; /* fmac fr0,fr2,fr3 */
L8c616248: sa_motion_publish_stop(ctx,0x8c616248u); fcopy(f,2u,3u);goto L8c61624a; /* fmov fr3,fr2 */
L8c61624a: sa_motion_publish_stop(ctx,0x8c61624au); fcopy(f,3u,7u);goto L8c61624c; /* fmov fr7,fr3 */
L8c61624c: sa_motion_publish_stop(ctx,0x8c61624cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61624e; /* fmul fr4,fr3 */
L8c61624e: sa_motion_publish_stop(ctx,0x8c61624eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[5],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616250; /* fmul fr5,fr1 */
L8c616250: sa_motion_publish_stop(ctx,0x8c616250u); r[0]=8u;goto L8c616252; /* mov #0x8,r0 */
L8c616252: sa_motion_publish_stop(ctx,0x8c616252u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c616254; /* fsub fr3,fr1 */
L8c616254: sa_motion_publish_stop(ctx,0x8c616254u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c616256; /* fmov.s @(r0,r6),fr3 */
L8c616256: sa_motion_publish_stop(ctx,0x8c616256u); fcopy(f,0u,1u);goto L8c616258; /* fmov fr1,fr0 */
L8c616258: sa_motion_publish_stop(ctx,0x8c616258u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61625a; /* fmac fr0,fr3,fr2 */
L8c61625a: sa_motion_publish_stop(ctx,0x8c61625au); fcopy(f,3u,15u);goto L8c61625c; /* fmov fr15,fr3 */
L8c61625c: sa_motion_publish_stop(ctx,0x8c61625cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c61625e; /* fneg fr3 */
L8c61625e: sa_motion_publish_stop(ctx,0x8c61625eu); fcopy(f,4u,2u);goto L8c616260; /* fmov fr2,fr4 */
L8c616260: sa_motion_publish_stop(ctx,0x8c616260u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c616262; /* fmul fr3,fr4 */
L8c616262: sa_motion_publish_stop(ctx,0x8c616262u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]=0;goto L8c616264; /* fldi0 fr2 */
L8c616264: sa_motion_publish_stop(ctx,0x8c616264u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c616266; /* fcmp/gt fr2,fr4 */
L8c616266: sa_motion_publish_stop(ctx,0x8c616266u); if((*sr&1u))goto L8c61628c;goto L8c616268; /* bt 0x8c61628c */
L8c616268: sa_motion_publish_stop(ctx,0x8c616268u); wr(m,bus,&io_status,r[15]+0x18u,r[5],4);if(io_status)return io_status;goto L8c61626a; /* mov.l r5,@(0x18,r15) */
L8c61626a: sa_motion_publish_stop(ctx,0x8c61626cu); fcopy(f,4u,12u);goto L8c61628c; /* bra 0x8c61628c */
L8c61626e: sa_motion_publish_stop(ctx,0x8c61626eu); r[0]=20u;goto L8c616270; /* mov #0x14,r0 */
L8c616270: sa_motion_publish_stop(ctx,0x8c616270u); fcopy(f,3u,13u);goto L8c616272; /* fmov fr13,fr3 */
L8c616272: sa_motion_publish_stop(ctx,0x8c616272u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c616274; /* fmov.s @(r0,r12),fr2 */
L8c616274: sa_motion_publish_stop(ctx,0x8c616274u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c616276; /* fsub fr5,fr3 */
L8c616276: sa_motion_publish_stop(ctx,0x8c616276u); r[0]=16u;goto L8c616278; /* mov #0x10,r0 */
L8c616278: sa_motion_publish_stop(ctx,0x8c616278u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[14],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61627a; /* fsub fr14,fr2 */
L8c61627a: sa_motion_publish_stop(ctx,0x8c61627au); fload(m,bus,&io_status,f,1u,r[0]+r[12]);if(io_status)return io_status;goto L8c61627c; /* fmov.s @(r0,r12),fr1 */
L8c61627c: sa_motion_publish_stop(ctx,0x8c61627cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[13],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61627e; /* fsub fr13,fr1 */
L8c61627e: sa_motion_publish_stop(ctx,0x8c61627eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c616280; /* fmul fr2,fr3 */
L8c616280: sa_motion_publish_stop(ctx,0x8c616280u); fcopy(f,2u,14u);goto L8c616282; /* fmov fr14,fr2 */
L8c616282: sa_motion_publish_stop(ctx,0x8c616282u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[6],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c616284; /* fsub fr6,fr2 */
L8c616284: sa_motion_publish_stop(ctx,0x8c616284u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616286; /* fmul fr1,fr2 */
L8c616286: sa_motion_publish_stop(ctx,0x8c616286u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c616288; /* fsub fr2,fr3 */
L8c616288: sa_motion_publish_stop(ctx,0x8c616288u); fcopy(f,4u,3u);goto L8c61628a; /* fmov fr3,fr4 */
L8c61628a: sa_motion_publish_stop(ctx,0x8c61628au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[15],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61628c; /* fmul fr15,fr4 */
L8c61628c: sa_motion_publish_stop(ctx,0x8c61628cu); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c61628e; /* cmp/hs r8,r9 */
L8c61628e: sa_motion_publish_stop(ctx,0x8c61628eu); if((*sr&1u))goto L8c6162d8;goto L8c616290; /* bt 0x8c6162d8 */
L8c616290: sa_motion_publish_stop(ctx,0x8c616290u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c616292; /* fldi0 fr3 */
L8c616292: sa_motion_publish_stop(ctx,0x8c616292u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c616294; /* fcmp/gt fr4,fr3 */
L8c616294: sa_motion_publish_stop(ctx,0x8c616294u); if((*sr&1u))goto L8c61631e;goto L8c616296; /* bt 0x8c61631e */
L8c616296: sa_motion_publish_stop(ctx,0x8c616296u); r[5]=rd(m,bus,&io_status,0x8c6164acu,4);if(io_status)return io_status;goto L8c616298; /* mov.l 0x8c6164ac,r5 */
L8c616298: sa_motion_publish_stop(ctx,0x8c616298u); r[6]=r[14];goto L8c61629a; /* mov r14,r6 */
L8c61629a: sa_motion_publish_stop(ctx,0x8c61629au); r[4]=r[11];goto L8c61629c; /* mov r11,r4 */
L8c61629c: sa_motion_publish_stop(ctx,0x8c61629cu); r[14]+=4u;goto L8c61629e; /* add #0x4,r14 */
L8c61629e: sa_motion_publish_stop(ctx,0x8c61629eu); r[4]+=16u;goto L8c6162a0; /* add #0x10,r4 */
L8c6162a0: sa_motion_publish_stop(ctx,0x8c6162a0u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6162a2; /* fschg */
L8c6162a2: sa_motion_publish_stop(ctx,0x8c6162a2u); r[8]+=16u;goto L8c6162a4; /* add #0x10,r8 */
L8c6162a4: sa_motion_publish_stop(ctx,0x8c6162a4u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6162a6; /* fmov.s @r4+,fr4 */
L8c6162a6: sa_motion_publish_stop(ctx,0x8c6162a6u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6162a8; /* fmov.s @r4,fr6 */
L8c6162a8: sa_motion_publish_stop(ctx,0x8c6162a8u); fcopy(f,0u,1u);goto L8c6162aa; /* fmov fr1,fr0 */
L8c6162aa: sa_motion_publish_stop(ctx,0x8c6162aau); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6162ac; /* fschg */
L8c6162ac: sa_motion_publish_stop(ctx,0x8c6162acu); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c6162ae; /* mov.w @r6+,r2 */
L8c6162ae: sa_motion_publish_stop(ctx,0x8c6162aeu); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c6162b0; /* mov.w @r6,r3 */
L8c6162b0: sa_motion_publish_stop(ctx,0x8c6162b0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6162b2; /* fmov.s fr6,@-r8 */
L8c6162b2: sa_motion_publish_stop(ctx,0x8c6162b2u); *fpul=r[3];goto L8c6162b4; /* lds r3,FPUL */
L8c6162b4: sa_motion_publish_stop(ctx,0x8c6162b4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6162b6; /* fmov.s fr5,@-r8 */
L8c6162b6: sa_motion_publish_stop(ctx,0x8c6162b6u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c6162b8; /* float FPUL,fr1 */
L8c6162b8: sa_motion_publish_stop(ctx,0x8c6162b8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c6162ba; /* fmov.s fr4,@-r8 */
L8c6162ba: sa_motion_publish_stop(ctx,0x8c6162bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6162bc; /* fmul fr0,fr1 */
L8c6162bc: sa_motion_publish_stop(ctx,0x8c6162bcu); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c6162be; /* mov.l r5,@-r8 */
L8c6162be: sa_motion_publish_stop(ctx,0x8c6162beu); *fpul=r[2];goto L8c6162c0; /* lds r2,FPUL */
L8c6162c0: sa_motion_publish_stop(ctx,0x8c6162c0u); r[8]+=28u;goto L8c6162c2; /* add #0x1c,r8 */
L8c6162c2: sa_motion_publish_stop(ctx,0x8c6162c2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c6162c4; /* float FPUL,fr2 */
L8c6162c4: sa_motion_publish_stop(ctx,0x8c6162c4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c6162c6; /* fldi1 fr3 */
L8c6162c6: sa_motion_publish_stop(ctx,0x8c6162c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6162c8; /* fmul fr0,fr2 */
L8c6162c8: sa_motion_publish_stop(ctx,0x8c6162c8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6162ca; /* fmov.s fr3,@-r8 */
L8c6162ca: sa_motion_publish_stop(ctx,0x8c6162cau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c6162cc; /* fmov.s fr1,@-r8 */
L8c6162cc: sa_motion_publish_stop(ctx,0x8c6162ccu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c6162ce; /* fmov.s fr2,@-r8 */
L8c6162ce: sa_motion_publish_stop(ctx,0x8c6162ceu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6162d0; /* pref @r8 */
L8c6162d0: sa_motion_publish_stop(ctx,0x8c6162d0u); r[8]+=16u;goto L8c6162d2; /* add #0x10,r8 */
L8c6162d2: sa_motion_publish_stop(ctx,0x8c6162d2u); r[14]+=4294967292u;goto L8c6162d4; /* add #-0x4,r14 */
L8c6162d4: sa_motion_publish_stop(ctx,0x8c6162d6u); r[9]=r[8];goto L8c61635a; /* bra 0x8c61635a */
L8c6162d8: sa_motion_publish_stop(ctx,0x8c6162d8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6162da; /* fldi0 fr3 */
L8c6162da: sa_motion_publish_stop(ctx,0x8c6162dau); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6162dc; /* fcmp/gt fr4,fr3 */
L8c6162dc: sa_motion_publish_stop(ctx,0x8c6162deu); take=!(*sr&1u);r[14]+=4u;if(take)goto L8c61635a;goto L8c6162e0; /* bf/s 0x8c61635a */
L8c6162e0: sa_motion_publish_stop(ctx,0x8c6162e0u); r[14]+=4294967292u;goto L8c6162e2; /* add #-0x4,r14 */
L8c6162e2: sa_motion_publish_stop(ctx,0x8c6162e2u); r[5]=rd(m,bus,&io_status,0x8c6164b0u,4);if(io_status)return io_status;goto L8c6162e4; /* mov.l 0x8c6164b0,r5 */
L8c6162e4: sa_motion_publish_stop(ctx,0x8c6162e4u); r[6]=r[14];goto L8c6162e6; /* mov r14,r6 */
L8c6162e6: sa_motion_publish_stop(ctx,0x8c6162e6u); r[4]=r[10];goto L8c6162e8; /* mov r10,r4 */
L8c6162e8: sa_motion_publish_stop(ctx,0x8c6162e8u); r[14]+=4u;goto L8c6162ea; /* add #0x4,r14 */
L8c6162ea: sa_motion_publish_stop(ctx,0x8c6162eau); r[4]+=16u;goto L8c6162ec; /* add #0x10,r4 */
L8c6162ec: sa_motion_publish_stop(ctx,0x8c6162ecu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6162ee; /* fschg */
L8c6162ee: sa_motion_publish_stop(ctx,0x8c6162eeu); r[8]+=16u;goto L8c6162f0; /* add #0x10,r8 */
L8c6162f0: sa_motion_publish_stop(ctx,0x8c6162f0u); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6162f2; /* fmov.s @r4+,fr4 */
L8c6162f2: sa_motion_publish_stop(ctx,0x8c6162f2u); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c6162f4; /* fmov.s @r4,fr6 */
L8c6162f4: sa_motion_publish_stop(ctx,0x8c6162f4u); fcopy(f,0u,1u);goto L8c6162f6; /* fmov fr1,fr0 */
L8c6162f6: sa_motion_publish_stop(ctx,0x8c6162f6u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c6162f8; /* fschg */
L8c6162f8: sa_motion_publish_stop(ctx,0x8c6162f8u); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c6162fa; /* mov.w @r6+,r2 */
L8c6162fa: sa_motion_publish_stop(ctx,0x8c6162fau); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c6162fc; /* mov.w @r6,r3 */
L8c6162fc: sa_motion_publish_stop(ctx,0x8c6162fcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c6162fe; /* fmov.s fr6,@-r8 */
L8c6162fe: sa_motion_publish_stop(ctx,0x8c6162feu); *fpul=r[3];goto L8c616300; /* lds r3,FPUL */
L8c616300: sa_motion_publish_stop(ctx,0x8c616300u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c616302; /* fmov.s fr5,@-r8 */
L8c616302: sa_motion_publish_stop(ctx,0x8c616302u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c616304; /* float FPUL,fr1 */
L8c616304: sa_motion_publish_stop(ctx,0x8c616304u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616306; /* fmov.s fr4,@-r8 */
L8c616306: sa_motion_publish_stop(ctx,0x8c616306u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616308; /* fmul fr0,fr1 */
L8c616308: sa_motion_publish_stop(ctx,0x8c616308u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c61630a; /* mov.l r5,@-r8 */
L8c61630a: sa_motion_publish_stop(ctx,0x8c61630au); *fpul=r[2];goto L8c61630c; /* lds r2,FPUL */
L8c61630c: sa_motion_publish_stop(ctx,0x8c61630cu); r[8]+=28u;goto L8c61630e; /* add #0x1c,r8 */
L8c61630e: sa_motion_publish_stop(ctx,0x8c61630eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c616310; /* float FPUL,fr2 */
L8c616310: sa_motion_publish_stop(ctx,0x8c616310u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c616312; /* fldi1 fr3 */
L8c616312: sa_motion_publish_stop(ctx,0x8c616312u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616314; /* fmul fr0,fr2 */
L8c616314: sa_motion_publish_stop(ctx,0x8c616314u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c616316; /* fmov.s fr3,@-r8 */
L8c616316: sa_motion_publish_stop(ctx,0x8c616316u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616318; /* fmov.s fr1,@-r8 */
L8c616318: sa_motion_publish_stop(ctx,0x8c616318u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61631a; /* fmov.s fr2,@-r8 */
L8c61631a: sa_motion_publish_stop(ctx,0x8c61631au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61631c; /* pref @r8 */
L8c61631c: sa_motion_publish_stop(ctx,0x8c61631cu); r[8]+=16u;goto L8c61631e; /* add #0x10,r8 */
L8c61631e: sa_motion_publish_stop(ctx,0x8c61631eu); r[5]=rd(m,bus,&io_status,0x8c6164b0u,4);if(io_status)return io_status;goto L8c616320; /* mov.l 0x8c6164b0,r5 */
L8c616320: sa_motion_publish_stop(ctx,0x8c616320u); r[6]=r[14];goto L8c616322; /* mov r14,r6 */
L8c616322: sa_motion_publish_stop(ctx,0x8c616322u); r[4]=r[11];goto L8c616324; /* mov r11,r4 */
L8c616324: sa_motion_publish_stop(ctx,0x8c616324u); r[14]+=4u;goto L8c616326; /* add #0x4,r14 */
L8c616326: sa_motion_publish_stop(ctx,0x8c616326u); r[4]+=16u;goto L8c616328; /* add #0x10,r4 */
L8c616328: sa_motion_publish_stop(ctx,0x8c616328u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c61632a; /* fschg */
L8c61632a: sa_motion_publish_stop(ctx,0x8c61632au); r[8]+=16u;goto L8c61632c; /* add #0x10,r8 */
L8c61632c: sa_motion_publish_stop(ctx,0x8c61632cu); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61632e; /* fmov.s @r4+,fr4 */
L8c61632e: sa_motion_publish_stop(ctx,0x8c61632eu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c616330; /* fmov.s @r4,fr6 */
L8c616330: sa_motion_publish_stop(ctx,0x8c616330u); fcopy(f,0u,1u);goto L8c616332; /* fmov fr1,fr0 */
L8c616332: sa_motion_publish_stop(ctx,0x8c616332u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616334; /* fschg */
L8c616334: sa_motion_publish_stop(ctx,0x8c616334u); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c616336; /* mov.w @r6+,r2 */
L8c616336: sa_motion_publish_stop(ctx,0x8c616336u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c616338; /* mov.w @r6,r3 */
L8c616338: sa_motion_publish_stop(ctx,0x8c616338u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c61633a; /* fmov.s fr6,@-r8 */
L8c61633a: sa_motion_publish_stop(ctx,0x8c61633au); *fpul=r[3];goto L8c61633c; /* lds r3,FPUL */
L8c61633c: sa_motion_publish_stop(ctx,0x8c61633cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61633e; /* fmov.s fr5,@-r8 */
L8c61633e: sa_motion_publish_stop(ctx,0x8c61633eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c616340; /* float FPUL,fr1 */
L8c616340: sa_motion_publish_stop(ctx,0x8c616340u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616342; /* fmov.s fr4,@-r8 */
L8c616342: sa_motion_publish_stop(ctx,0x8c616342u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616344; /* fmul fr0,fr1 */
L8c616344: sa_motion_publish_stop(ctx,0x8c616344u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616346; /* mov.l r5,@-r8 */
L8c616346: sa_motion_publish_stop(ctx,0x8c616346u); *fpul=r[2];goto L8c616348; /* lds r2,FPUL */
L8c616348: sa_motion_publish_stop(ctx,0x8c616348u); r[8]+=28u;goto L8c61634a; /* add #0x1c,r8 */
L8c61634a: sa_motion_publish_stop(ctx,0x8c61634au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c61634c; /* float FPUL,fr2 */
L8c61634c: sa_motion_publish_stop(ctx,0x8c61634cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c61634e; /* fldi1 fr3 */
L8c61634e: sa_motion_publish_stop(ctx,0x8c61634eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c616350; /* fmul fr0,fr2 */
L8c616350: sa_motion_publish_stop(ctx,0x8c616350u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c616352; /* fmov.s fr3,@-r8 */
L8c616352: sa_motion_publish_stop(ctx,0x8c616352u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616354; /* fmov.s fr1,@-r8 */
L8c616354: sa_motion_publish_stop(ctx,0x8c616354u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616356; /* fmov.s fr2,@-r8 */
L8c616356: sa_motion_publish_stop(ctx,0x8c616356u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616358; /* pref @r8 */
L8c616358: sa_motion_publish_stop(ctx,0x8c616358u); r[8]+=16u;goto L8c61635a; /* add #0x10,r8 */
L8c61635a: sa_motion_publish_stop(ctx,0x8c61635au); r[2]=rd(m,bus,&io_status,r[15]+0x18u,4);if(io_status)return io_status;goto L8c61635c; /* mov.l @(0x18,r15),r2 */
L8c61635c: sa_motion_publish_stop(ctx,0x8c61635cu); *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c61635e; /* tst r2,r2 */
L8c61635e: sa_motion_publish_stop(ctx,0x8c61635eu); if((*sr&1u))goto L8c6163d4;goto L8c616360; /* bt 0x8c6163d4 */
L8c616360: sa_motion_publish_stop(ctx,0x8c616360u); r[4]=rd(m,bus,&io_status,0x8c6164b4u,4);if(io_status)return io_status;goto L8c616362; /* mov.l 0x8c6164b4,r4 */
L8c616362: sa_motion_publish_stop(ctx,0x8c616362u); r[3]=0u;goto L8c616364; /* mov #0x0,r3 */
L8c616364: sa_motion_publish_stop(ctx,0x8c616364u); r[14]+=4294967292u;goto L8c616366; /* add #-0x4,r14 */
L8c616366: sa_motion_publish_stop(ctx,0x8c616366u); r[9]=rd(m,bus,&io_status,0x8c6164b8u,4);if(io_status)return io_status;goto L8c616368; /* mov.l 0x8c6164b8,r9 */
L8c616368: sa_motion_publish_stop(ctx,0x8c616368u); wr(m,bus,&io_status,r[4],r[3],4);if(io_status)return io_status;goto L8c61636a; /* mov.l r3,@r4 */
L8c61636a: sa_motion_publish_stop(ctx,0x8c61636au); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c61636c; /* mov.w @r14,r3 */
L8c61636c: sa_motion_publish_stop(ctx,0x8c61636cu); r[4]=r[9];goto L8c61636e; /* mov r9,r4 */
L8c61636e: sa_motion_publish_stop(ctx,0x8c61636eu); *fpul=r[3];goto L8c616370; /* lds r3,FPUL */
L8c616370: sa_motion_publish_stop(ctx,0x8c616370u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c616372; /* float FPUL,fr3 */
L8c616372: sa_motion_publish_stop(ctx,0x8c616372u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c616374; /* ftrc fr3,FPUL */
L8c616374: sa_motion_publish_stop(ctx,0x8c616374u); r[2]=*fpul;goto L8c616376; /* sts FPUL,r2 */
L8c616376: sa_motion_publish_stop(ctx,0x8c616376u); wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c616378; /* mov.w r2,@r4 */
L8c616378: sa_motion_publish_stop(ctx,0x8c616378u); r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c61637a; /* mov.w @(0x2,r14),r0 */
L8c61637a: sa_motion_publish_stop(ctx,0x8c61637au); r[14]+=4u;goto L8c61637c; /* add #0x4,r14 */
L8c61637c: sa_motion_publish_stop(ctx,0x8c61637cu); r[3]=r[0];goto L8c61637e; /* mov r0,r3 */
L8c61637e: sa_motion_publish_stop(ctx,0x8c61637eu); *fpul=r[3];goto L8c616380; /* lds r3,FPUL */
L8c616380: sa_motion_publish_stop(ctx,0x8c616380u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c616382; /* float FPUL,fr3 */
L8c616382: sa_motion_publish_stop(ctx,0x8c616382u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c616384; /* ftrc fr3,FPUL */
L8c616384: sa_motion_publish_stop(ctx,0x8c616384u); r[0]=*fpul;goto L8c616386; /* sts FPUL,r0 */
L8c616386: sa_motion_publish_stop(ctx,0x8c616386u); wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c616388; /* mov.w r0,@(0x2,r4) */
L8c616388: sa_motion_publish_stop(ctx,0x8c616388u); r[4]+=4u;goto L8c61638a; /* add #0x4,r4 */
L8c61638a: sa_motion_publish_stop(ctx,0x8c61638au); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c61638c; /* mov.w @r14,r3 */
L8c61638c: sa_motion_publish_stop(ctx,0x8c61638cu); *fpul=r[3];goto L8c61638e; /* lds r3,FPUL */
L8c61638e: sa_motion_publish_stop(ctx,0x8c61638eu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c616390; /* float FPUL,fr3 */
L8c616390: sa_motion_publish_stop(ctx,0x8c616390u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c616392; /* ftrc fr3,FPUL */
L8c616392: sa_motion_publish_stop(ctx,0x8c616392u); r[2]=*fpul;goto L8c616394; /* sts FPUL,r2 */
L8c616394: sa_motion_publish_stop(ctx,0x8c616394u); wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c616396; /* mov.w r2,@r4 */
L8c616396: sa_motion_publish_stop(ctx,0x8c616396u); r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c616398; /* mov.w @(0x2,r14),r0 */
L8c616398: sa_motion_publish_stop(ctx,0x8c616398u); r[3]=r[0];goto L8c61639a; /* mov r0,r3 */
L8c61639a: sa_motion_publish_stop(ctx,0x8c61639au); *fpul=r[3];goto L8c61639c; /* lds r3,FPUL */
L8c61639c: sa_motion_publish_stop(ctx,0x8c61639cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61639e; /* float FPUL,fr3 */
L8c61639e: sa_motion_publish_stop(ctx,0x8c61639eu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6163a0; /* ftrc fr3,FPUL */
L8c6163a0: sa_motion_publish_stop(ctx,0x8c6163a0u); r[14]+=4u;goto L8c6163a2; /* add #0x4,r14 */
L8c6163a2: sa_motion_publish_stop(ctx,0x8c6163a2u); r[0]=*fpul;goto L8c6163a4; /* sts FPUL,r0 */
L8c6163a4: sa_motion_publish_stop(ctx,0x8c6163a4u); r[6]=r[12];goto L8c6163a6; /* mov r12,r6 */
L8c6163a6: sa_motion_publish_stop(ctx,0x8c6163a6u); r[5]=r[11];goto L8c6163a8; /* mov r11,r5 */
L8c6163a8: sa_motion_publish_stop(ctx,0x8c6163a8u); r[7]=r[9];goto L8c6163aa; /* mov r9,r7 */
L8c6163aa: sa_motion_publish_stop(ctx,0x8c6163aau); wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6163ac; /* mov.w r0,@(0x2,r4) */
L8c6163ac: sa_motion_publish_stop(ctx,0x8c6163acu); r[4]=r[9];goto L8c6163ae; /* mov r9,r4 */
L8c6163ae: sa_motion_publish_stop(ctx,0x8c6163aeu); r[3]=sx(m,bus,&io_status,r[14],2);if(io_status)return io_status;goto L8c6163b0; /* mov.w @r14,r3 */
L8c6163b0: sa_motion_publish_stop(ctx,0x8c6163b0u); r[4]+=8u;goto L8c6163b2; /* add #0x8,r4 */
L8c6163b2: sa_motion_publish_stop(ctx,0x8c6163b2u); *fpul=r[3];goto L8c6163b4; /* lds r3,FPUL */
L8c6163b4: sa_motion_publish_stop(ctx,0x8c6163b4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6163b6; /* float FPUL,fr3 */
L8c6163b6: sa_motion_publish_stop(ctx,0x8c6163b6u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6163b8; /* ftrc fr3,FPUL */
L8c6163b8: sa_motion_publish_stop(ctx,0x8c6163b8u); r[2]=*fpul;goto L8c6163ba; /* sts FPUL,r2 */
L8c6163ba: sa_motion_publish_stop(ctx,0x8c6163bau); wr(m,bus,&io_status,r[4],r[2],2);if(io_status)return io_status;goto L8c6163bc; /* mov.w r2,@r4 */
L8c6163bc: sa_motion_publish_stop(ctx,0x8c6163bcu); r[0]=sx(m,bus,&io_status,r[14]+0x2u,2);if(io_status)return io_status;goto L8c6163be; /* mov.w @(0x2,r14),r0 */
L8c6163be: sa_motion_publish_stop(ctx,0x8c6163beu); r[3]=r[0];goto L8c6163c0; /* mov r0,r3 */
L8c6163c0: sa_motion_publish_stop(ctx,0x8c6163c0u); *fpul=r[3];goto L8c6163c2; /* lds r3,FPUL */
L8c6163c2: sa_motion_publish_stop(ctx,0x8c6163c2u); r[3]=rd(m,bus,&io_status,0x8c6164bcu,4);if(io_status)return io_status;goto L8c6163c4; /* mov.l 0x8c6164bc,r3 */
L8c6163c4: sa_motion_publish_stop(ctx,0x8c6163c4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6163c6; /* float FPUL,fr3 */
L8c6163c6: sa_motion_publish_stop(ctx,0x8c6163c6u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6163c8; /* ftrc fr3,FPUL */
L8c6163c8: sa_motion_publish_stop(ctx,0x8c6163c8u); r[0]=*fpul;goto L8c6163ca; /* sts FPUL,r0 */
L8c6163ca: sa_motion_publish_stop(ctx,0x8c6163cau); wr(m,bus,&io_status,r[4]+0x2u,r[0],2);if(io_status)return io_status;goto L8c6163cc; /* mov.w r0,@(0x2,r4) */
L8c6163cc: sa_motion_publish_stop(ctx,0x8c6163ceu); target=r[3];*pr=0x8c6163d0u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6163d0; /* jsr @r3 */
L8c6163d0: sa_motion_publish_stop(ctx,0x8c6163d0u); r[9]=r[8];goto L8c6163d2; /* mov r8,r9 */
L8c6163d2: sa_motion_publish_stop(ctx,0x8c6163d2u); r[14]+=4294967292u;goto L8c6163d4; /* add #-0x4,r14 */
L8c6163d4: sa_motion_publish_stop(ctx,0x8c6163d4u); fcopy(f,5u,13u);goto L8c6163d6; /* fmov fr13,fr5 */
L8c6163d6: sa_motion_publish_stop(ctx,0x8c6163d6u); r[0]=16u;goto L8c6163d8; /* mov #0x10,r0 */
L8c6163d8: sa_motion_publish_stop(ctx,0x8c6163d8u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6163da; /* mov.l @(0x8,r15),r3 */
L8c6163da: sa_motion_publish_stop(ctx,0x8c6163dau); r[10]=r[11];goto L8c6163dc; /* mov r11,r10 */
L8c6163dc: sa_motion_publish_stop(ctx,0x8c6163dcu); fload(m,bus,&io_status,f,13u,r[0]+r[12]);if(io_status)return io_status;goto L8c6163de; /* fmov.s @(r0,r12),fr13 */
L8c6163de: sa_motion_publish_stop(ctx,0x8c6163deu); r[0]=20u;goto L8c6163e0; /* mov #0x14,r0 */
L8c6163e0: sa_motion_publish_stop(ctx,0x8c6163e0u); fcopy(f,6u,14u);goto L8c6163e2; /* fmov fr14,fr6 */
L8c6163e2: sa_motion_publish_stop(ctx,0x8c6163e2u); r[3]+=4294967295u;goto L8c6163e4; /* add #-0x1,r3 */
L8c6163e4: sa_motion_publish_stop(ctx,0x8c6163e4u); fload(m,bus,&io_status,f,14u,r[0]+r[12]);if(io_status)return io_status;goto L8c6163e6; /* fmov.s @(r0,r12),fr14 */
L8c6163e6: sa_motion_publish_stop(ctx,0x8c6163e6u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c6163e8; /* tst r3,r3 */
L8c6163e8: sa_motion_publish_stop(ctx,0x8c6163e8u); r[11]=r[12];goto L8c6163ea; /* mov r12,r11 */
L8c6163ea: sa_motion_publish_stop(ctx,0x8c6163eau); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][15]^=0x80000000u;goto L8c6163ec; /* fneg fr15 */
L8c6163ec: sa_motion_publish_stop(ctx,0x8c6163eeu); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;if(take)goto L8c6163f4;goto L8c6163f0; /* bt/s 0x8c6163f4 */
L8c6163f0: sa_motion_publish_stop(ctx,0x8c6163f2u); goto L8c6160fa; /* bra 0x8c6160fa */
L8c6163f4: sa_motion_publish_stop(ctx,0x8c6163f4u); *sr=(*sr&~1u)|(r[9]>=r[8]);goto L8c6163f6; /* cmp/hs r8,r9 */
L8c6163f6: sa_motion_publish_stop(ctx,0x8c6163f8u); take=(*sr&1u);r[14]+=8u;if(take)goto L8c616438;goto L8c6163fa; /* bt/s 0x8c616438 */
L8c6163fa: sa_motion_publish_stop(ctx,0x8c6163fau); r[14]+=4294967288u;goto L8c6163fc; /* add #-0x8,r14 */
L8c6163fc: sa_motion_publish_stop(ctx,0x8c6163fcu); r[5]=rd(m,bus,&io_status,0x8c6164acu,4);if(io_status)return io_status;goto L8c6163fe; /* mov.l 0x8c6164ac,r5 */
L8c6163fe: sa_motion_publish_stop(ctx,0x8c6163feu); r[6]=r[14];goto L8c616400; /* mov r14,r6 */
L8c616400: sa_motion_publish_stop(ctx,0x8c616400u); r[4]=r[12];goto L8c616402; /* mov r12,r4 */
L8c616402: sa_motion_publish_stop(ctx,0x8c616402u); r[14]+=4u;goto L8c616404; /* add #0x4,r14 */
L8c616404: sa_motion_publish_stop(ctx,0x8c616404u); r[4]+=16u;goto L8c616406; /* add #0x10,r4 */
L8c616406: sa_motion_publish_stop(ctx,0x8c616406u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616408; /* fschg */
L8c616408: sa_motion_publish_stop(ctx,0x8c616408u); r[8]+=16u;goto L8c61640a; /* add #0x10,r8 */
L8c61640a: sa_motion_publish_stop(ctx,0x8c61640au); fload(m,bus,&io_status,f,4u,r[4]);if(io_status)return io_status;r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61640c; /* fmov.s @r4+,fr4 */
L8c61640c: sa_motion_publish_stop(ctx,0x8c61640cu); fload(m,bus,&io_status,f,6u,r[4]);if(io_status)return io_status;goto L8c61640e; /* fmov.s @r4,fr6 */
L8c61640e: sa_motion_publish_stop(ctx,0x8c61640eu); fcopy(f,0u,1u);goto L8c616410; /* fmov fr1,fr0 */
L8c616410: sa_motion_publish_stop(ctx,0x8c616410u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c616412; /* fschg */
L8c616412: sa_motion_publish_stop(ctx,0x8c616412u); r[2]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;r[6]+=2;goto L8c616414; /* mov.w @r6+,r2 */
L8c616414: sa_motion_publish_stop(ctx,0x8c616414u); r[3]=sx(m,bus,&io_status,r[6],2);if(io_status)return io_status;goto L8c616416; /* mov.w @r6,r3 */
L8c616416: sa_motion_publish_stop(ctx,0x8c616416u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[8]);if(io_status)return io_status;goto L8c616418; /* fmov.s fr6,@-r8 */
L8c616418: sa_motion_publish_stop(ctx,0x8c616418u); *fpul=r[3];goto L8c61641a; /* lds r3,FPUL */
L8c61641a: sa_motion_publish_stop(ctx,0x8c61641au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61641c; /* fmov.s fr5,@-r8 */
L8c61641c: sa_motion_publish_stop(ctx,0x8c61641cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][1]);if(status)return status;goto L8c61641e; /* float FPUL,fr1 */
L8c61641e: sa_motion_publish_stop(ctx,0x8c61641eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c616420; /* fmov.s fr4,@-r8 */
L8c616420: sa_motion_publish_stop(ctx,0x8c616420u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status;}goto L8c616422; /* fmul fr0,fr1 */
L8c616422: sa_motion_publish_stop(ctx,0x8c616422u); r[8]-=4;wr(m,bus,&io_status,r[8],r[5],4);if(io_status)return io_status;goto L8c616424; /* mov.l r5,@-r8 */
L8c616424: sa_motion_publish_stop(ctx,0x8c616424u); *fpul=r[2];goto L8c616426; /* lds r2,FPUL */
L8c616426: sa_motion_publish_stop(ctx,0x8c616426u); r[8]+=28u;goto L8c616428; /* add #0x1c,r8 */
L8c616428: sa_motion_publish_stop(ctx,0x8c616428u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c61642a; /* float FPUL,fr2 */
L8c61642a: sa_motion_publish_stop(ctx,0x8c61642au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c61642c; /* fldi1 fr3 */
L8c61642c: sa_motion_publish_stop(ctx,0x8c61642cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[0],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61642e; /* fmul fr0,fr2 */
L8c61642e: sa_motion_publish_stop(ctx,0x8c61642eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c616430; /* fmov.s fr3,@-r8 */
L8c616430: sa_motion_publish_stop(ctx,0x8c616430u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[8]);if(io_status)return io_status;goto L8c616432; /* fmov.s fr1,@-r8 */
L8c616432: sa_motion_publish_stop(ctx,0x8c616432u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c616434; /* fmov.s fr2,@-r8 */
L8c616434: sa_motion_publish_stop(ctx,0x8c616434u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c616436; /* pref @r8 */
L8c616436: sa_motion_publish_stop(ctx,0x8c616436u); r[8]+=16u;goto L8c616438; /* add #0x10,r8 */
L8c616438: sa_motion_publish_stop(ctx,0x8c616438u); r[3]=rd(m,bus,&io_status,r[15]+0x1cu,4);if(io_status)return io_status;goto L8c61643a; /* mov.l @(0x1c,r15),r3 */
L8c61643a: sa_motion_publish_stop(ctx,0x8c61643au); r[3]+=4294967295u;goto L8c61643c; /* add #-0x1,r3 */
L8c61643c: sa_motion_publish_stop(ctx,0x8c61643cu); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c61643e; /* tst r3,r3 */
L8c61643e: sa_motion_publish_stop(ctx,0x8c616440u); take=(*sr&1u);wr(m,bus,&io_status,r[15]+0x1cu,r[3],4);if(io_status)return io_status;if(take)goto L8c616446;goto L8c616442; /* bt/s 0x8c616446 */
L8c616442: sa_motion_publish_stop(ctx,0x8c616444u); goto L8c6160a8; /* bra 0x8c6160a8 */
L8c616446: sa_motion_publish_stop(ctx,0x8c616446u); r[4]=rd(m,bus,&io_status,0x8c6164c0u,4);if(io_status)return io_status;goto L8c616448; /* mov.l 0x8c6164c0,r4 */
L8c616448: sa_motion_publish_stop(ctx,0x8c616448u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c61644a; /* mov.l @(0x10,r15),r3 */
L8c61644a: sa_motion_publish_stop(ctx,0x8c61644au); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c61644c; /* mov.l @r4,r2 */
L8c61644c: sa_motion_publish_stop(ctx,0x8c61644cu); r[2]+=r[3];goto L8c61644e; /* add r3,r2 */
L8c61644e: sa_motion_publish_stop(ctx,0x8c61644eu); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c616450; /* mov.l r2,@r4 */
L8c616450: sa_motion_publish_stop(ctx,0x8c616450u); r[4]=rd(m,bus,&io_status,0x8c6164c4u,4);if(io_status)return io_status;goto L8c616452; /* mov.l 0x8c6164c4,r4 */
L8c616452: sa_motion_publish_stop(ctx,0x8c616452u); r[3]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c616454; /* mov.l @(0x10,r15),r3 */
L8c616454: sa_motion_publish_stop(ctx,0x8c616454u); r[2]=rd(m,bus,&io_status,r[4],4);if(io_status)return io_status;goto L8c616456; /* mov.l @r4,r2 */
L8c616456: sa_motion_publish_stop(ctx,0x8c616456u); r[2]+=r[3];goto L8c616458; /* add r3,r2 */
L8c616458: sa_motion_publish_stop(ctx,0x8c616458u); wr(m,bus,&io_status,r[4],r[2],4);if(io_status)return io_status;goto L8c61645a; /* mov.l r2,@r4 */
L8c61645a: sa_motion_publish_stop(ctx,0x8c61645au); r[15]+=80u;goto L8c61645c; /* add #0x50,r15 */
L8c61645c: sa_motion_publish_stop(ctx,0x8c61645cu); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61645e; /* lds.l @r15+,PR */
L8c61645e: sa_motion_publish_stop(ctx,0x8c61645eu); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616460; /* fmov.s @r15+,fr12 */
L8c616460: sa_motion_publish_stop(ctx,0x8c616460u); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616462; /* fmov.s @r15+,fr13 */
L8c616462: sa_motion_publish_stop(ctx,0x8c616462u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616464; /* fmov.s @r15+,fr14 */
L8c616464: sa_motion_publish_stop(ctx,0x8c616464u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c616466; /* fmov.s @r15+,fr15 */
L8c616466: sa_motion_publish_stop(ctx,0x8c616466u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616468; /* mov.l @r15+,r9 */
L8c616468: sa_motion_publish_stop(ctx,0x8c616468u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61646a; /* mov.l @r15+,r10 */
L8c61646a: sa_motion_publish_stop(ctx,0x8c61646au); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61646c; /* mov.l @r15+,r11 */
L8c61646c: sa_motion_publish_stop(ctx,0x8c61646cu); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61646e; /* mov.l @r15+,r12 */
L8c61646e: sa_motion_publish_stop(ctx,0x8c61646eu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c616470; /* mov.l @r15+,r13 */
L8c616470: sa_motion_publish_stop(ctx,0x8c616472u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
