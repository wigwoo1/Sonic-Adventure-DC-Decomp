#include "sa_draw_motion_stop.h"
/* UV/constant-factor triangle near-plane clipper with portable queue interface; name inferred.
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
SA_STAGE_EXPORT int sa_uv_constant_triangle_near_clip_8c6124c0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c6124c0;
L8c6124c0: sa_motion_publish_stop(ctx,0x8c6124c0u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c6124c2; /* mov.l r14,@-r15 */
L8c6124c2: sa_motion_publish_stop(ctx,0x8c6124c2u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c6124c4; /* mov.l r13,@-r15 */
L8c6124c4: sa_motion_publish_stop(ctx,0x8c6124c4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c6124c6; /* mov.l r12,@-r15 */
L8c6124c6: sa_motion_publish_stop(ctx,0x8c6124c6u); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c6124c8; /* mov.l r11,@-r15 */
L8c6124c8: sa_motion_publish_stop(ctx,0x8c6124c8u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c6124ca; /* mov.l r10,@-r15 */
L8c6124ca: sa_motion_publish_stop(ctx,0x8c6124cau); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c6124cc; /* mov.l r9,@-r15 */
L8c6124cc: sa_motion_publish_stop(ctx,0x8c6124ccu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c6124ce; /* fmov.s fr12,@-r15 */
L8c6124ce: sa_motion_publish_stop(ctx,0x8c6124ceu); r[15]+=4294967228u;goto L8c6124d0; /* add #-0x44,r15 */
L8c6124d0: sa_motion_publish_stop(ctx,0x8c6124d0u); r[3]=rd(m,bus,&io_status,0x8c612558u,4);if(io_status)return io_status;goto L8c6124d2; /* mov.l 0x8c612558,r3 */
L8c6124d2: sa_motion_publish_stop(ctx,0x8c6124d2u); r[14]=r[15];goto L8c6124d4; /* mov r15,r14 */
L8c6124d4: sa_motion_publish_stop(ctx,0x8c6124d4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][5]=0x3f800000u;goto L8c6124d6; /* fldi1 fr5 */
L8c6124d6: sa_motion_publish_stop(ctx,0x8c6124d6u); r[14]+=56u;goto L8c6124d8; /* add #0x38,r14 */
L8c6124d8: sa_motion_publish_stop(ctx,0x8c6124d8u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6124da; /* fmov.s @r3,fr3 */
L8c6124da: sa_motion_publish_stop(ctx,0x8c6124dau); r[9]=r[15];goto L8c6124dc; /* mov r15,r9 */
L8c6124dc: sa_motion_publish_stop(ctx,0x8c6124dcu); fcopy(f,6u,5u);goto L8c6124de; /* fmov fr5,fr6 */
L8c6124de: sa_motion_publish_stop(ctx,0x8c6124deu); r[2]=r[14];goto L8c6124e0; /* mov r14,r2 */
L8c6124e0: sa_motion_publish_stop(ctx,0x8c6124e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[6],&fr[3],&f->fpscr,&fr[6]);if(status)return status;}goto L8c6124e2; /* fdiv fr3,fr6 */
L8c6124e2: sa_motion_publish_stop(ctx,0x8c6124e2u); r[12]=rd(m,bus,&io_status,0x8c61255cu,4);if(io_status)return io_status;goto L8c6124e4; /* mov.l 0x8c61255c,r12 */
L8c6124e4: sa_motion_publish_stop(ctx,0x8c6124e4u); r[9]+=20u;goto L8c6124e6; /* add #0x14,r9 */
L8c6124e6: sa_motion_publish_stop(ctx,0x8c6124e6u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c6124e8; /* mov.l r2,@r15 */
L8c6124e8: sa_motion_publish_stop(ctx,0x8c6124e8u); wr(m,bus,&io_status,r[2],r[4],4);if(io_status)return io_status;goto L8c6124ea; /* mov.l r4,@r2 */
L8c6124ea: sa_motion_publish_stop(ctx,0x8c6124eau); r[4]=1u;goto L8c6124ec; /* mov #0x1,r4 */
L8c6124ec: sa_motion_publish_stop(ctx,0x8c6124ecu); wr(m,bus,&io_status,r[14]+0x4u,r[5],4);if(io_status)return io_status;goto L8c6124ee; /* mov.l r5,@(0x4,r14) */
L8c6124ee: sa_motion_publish_stop(ctx,0x8c6124eeu); r[11]=0u;goto L8c6124f0; /* mov #0x0,r11 */
L8c6124f0: sa_motion_publish_stop(ctx,0x8c6124f0u); wr(m,bus,&io_status,r[14]+0x8u,r[6],4);if(io_status)return io_status;goto L8c6124f2; /* mov.l r6,@(0x8,r14) */
L8c6124f2: sa_motion_publish_stop(ctx,0x8c6124f2u); r[6]=r[9];goto L8c6124f4; /* mov r9,r6 */
L8c6124f4: sa_motion_publish_stop(ctx,0x8c6124f4u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6124f6; /* mov.l @r15,r13 */
L8c6124f6: sa_motion_publish_stop(ctx,0x8c6124f6u); r[10]=r[9];goto L8c6124f8; /* mov r9,r10 */
L8c6124f8: sa_motion_publish_stop(ctx,0x8c6124f8u); r[5]=r[9];goto L8c6124fa; /* mov r9,r5 */
L8c6124fa: sa_motion_publish_stop(ctx,0x8c6124fau); r[0]=r[11];goto L8c6124fc; /* mov r11,r0 */
L8c6124fc: sa_motion_publish_stop(ctx,0x8c6124fcu); r[0]<<=2;goto L8c6124fe; /* shll2 r0 */
L8c6124fe: sa_motion_publish_stop(ctx,0x8c6124feu); r[3]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c612500; /* mov.l @(r0,r14),r3 */
L8c612500: sa_motion_publish_stop(ctx,0x8c612500u); r[2]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c612502; /* mov.l @(0xc,r3),r2 */
L8c612502: sa_motion_publish_stop(ctx,0x8c612502u); *sr=(*sr&~1u)|!(r[4]&r[2]);goto L8c612504; /* tst r4,r2 */
L8c612504: sa_motion_publish_stop(ctx,0x8c612504u); if(!(*sr&1u))goto L8c612568;goto L8c612506; /* bf 0x8c612568 */
L8c612506: sa_motion_publish_stop(ctx,0x8c612506u); r[1]=r[11];goto L8c612508; /* mov r11,r1 */
L8c612508: sa_motion_publish_stop(ctx,0x8c612508u); r[1]<<=2;goto L8c61250a; /* shll2 r1 */
L8c61250a: sa_motion_publish_stop(ctx,0x8c61250au); r[1]+=r[14];goto L8c61250c; /* add r14,r1 */
L8c61250c: sa_motion_publish_stop(ctx,0x8c61250cu); fcopy(f,4u,5u);goto L8c61250e; /* fmov fr5,fr4 */
L8c61250e: sa_motion_publish_stop(ctx,0x8c61250eu); r[0]=24u;goto L8c612510; /* mov #0x18,r0 */
L8c612510: sa_motion_publish_stop(ctx,0x8c612510u); wr(m,bus,&io_status,r[15],r[1],4);if(io_status)return io_status;goto L8c612512; /* mov.l r1,@r15 */
L8c612512: sa_motion_publish_stop(ctx,0x8c612512u); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c612514; /* mov.l @r1,r3 */
L8c612514: sa_motion_publish_stop(ctx,0x8c612514u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612516; /* mov.l r3,@(0x4,r15) */
L8c612516: sa_motion_publish_stop(ctx,0x8c612516u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612518; /* fmov.s @(r0,r3),fr3 */
L8c612518: sa_motion_publish_stop(ctx,0x8c612518u); r[0]=12u;goto L8c61251a; /* mov #0xc,r0 */
L8c61251a: sa_motion_publish_stop(ctx,0x8c61251au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61251c; /* fdiv fr3,fr4 */
L8c61251c: sa_motion_publish_stop(ctx,0x8c61251cu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61251e; /* fmov.s @(r0,r12),fr3 */
L8c61251e: sa_motion_publish_stop(ctx,0x8c61251eu); r[0]=16u;goto L8c612520; /* mov #0x10,r0 */
L8c612520: sa_motion_publish_stop(ctx,0x8c612520u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c612522; /* fmov.s @(r0,r3),fr2 */
L8c612522: sa_motion_publish_stop(ctx,0x8c612522u); r[0]=16u;goto L8c612524; /* mov #0x10,r0 */
L8c612524: sa_motion_publish_stop(ctx,0x8c612524u); r[3]=rd(m,bus,&io_status,0x8c612560u,4);if(io_status)return io_status;goto L8c612526; /* mov.l 0x8c612560,r3 */
L8c612526: sa_motion_publish_stop(ctx,0x8c612526u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612528; /* fsub fr3,fr2 */
L8c612528: sa_motion_publish_stop(ctx,0x8c612528u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61252a; /* fmov.s @r3,fr3 */
L8c61252a: sa_motion_publish_stop(ctx,0x8c61252au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61252c; /* fmul fr4,fr2 */
L8c61252c: sa_motion_publish_stop(ctx,0x8c61252cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61252e; /* fdiv fr3,fr2 */
L8c61252e: sa_motion_publish_stop(ctx,0x8c61252eu); fstore(m,bus,&io_status,f,2u,r[6]);if(io_status)return io_status;goto L8c612530; /* fmov.s fr2,@r6 */
L8c612530: sa_motion_publish_stop(ctx,0x8c612530u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612532; /* mov.l @r15,r2 */
L8c612532: sa_motion_publish_stop(ctx,0x8c612532u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c612534; /* fmov.s @(r0,r12),fr3 */
L8c612534: sa_motion_publish_stop(ctx,0x8c612534u); r[0]=20u;goto L8c612536; /* mov #0x14,r0 */
L8c612536: sa_motion_publish_stop(ctx,0x8c612536u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c612538; /* mov.l @r2,r1 */
L8c612538: sa_motion_publish_stop(ctx,0x8c612538u); r[2]=rd(m,bus,&io_status,0x8c612564u,4);if(io_status)return io_status;goto L8c61253a; /* mov.l 0x8c612564,r2 */
L8c61253a: sa_motion_publish_stop(ctx,0x8c61253au); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61253c; /* fmov.s @(r0,r1),fr2 */
L8c61253c: sa_motion_publish_stop(ctx,0x8c61253cu); r[0]=4u;goto L8c61253e; /* mov #0x4,r0 */
L8c61253e: sa_motion_publish_stop(ctx,0x8c61253eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c612540; /* fsub fr3,fr2 */
L8c612540: sa_motion_publish_stop(ctx,0x8c612540u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c612542; /* fmov.s @r2,fr3 */
L8c612542: sa_motion_publish_stop(ctx,0x8c612542u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[4],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612544; /* fmul fr4,fr2 */
L8c612544: sa_motion_publish_stop(ctx,0x8c612544u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612546; /* fdiv fr3,fr2 */
L8c612546: sa_motion_publish_stop(ctx,0x8c612546u); fstore(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c612548; /* fmov fr2,@(r0,r6) */
L8c612548: sa_motion_publish_stop(ctx,0x8c612548u); r[0]=8u;goto L8c61254a; /* mov #0x8,r0 */
L8c61254a: sa_motion_publish_stop(ctx,0x8c61254cu); fstore(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c612584; /* bra 0x8c612584 */
L8c612568: sa_motion_publish_stop(ctx,0x8c612568u); r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61256a; /* mov.l @r13,r3 */
L8c61256a: sa_motion_publish_stop(ctx,0x8c61256au); r[0]=16u;goto L8c61256c; /* mov #0x10,r0 */
L8c61256c: sa_motion_publish_stop(ctx,0x8c61256cu); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c61256e; /* fmov.s @(r0,r3),fr3 */
L8c61256e: sa_motion_publish_stop(ctx,0x8c61256eu); r[0]=20u;goto L8c612570; /* mov #0x14,r0 */
L8c612570: sa_motion_publish_stop(ctx,0x8c612570u); fstore(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c612572; /* fmov.s fr3,@r5 */
L8c612572: sa_motion_publish_stop(ctx,0x8c612572u); r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c612574; /* mov.l @r13,r3 */
L8c612574: sa_motion_publish_stop(ctx,0x8c612574u); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612576; /* fmov.s @(r0,r3),fr3 */
L8c612576: sa_motion_publish_stop(ctx,0x8c612576u); r[0]=4u;goto L8c612578; /* mov #0x4,r0 */
L8c612578: sa_motion_publish_stop(ctx,0x8c612578u); fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61257a; /* fmov fr3,@(r0,r5) */
L8c61257a: sa_motion_publish_stop(ctx,0x8c61257au); r[0]=24u;goto L8c61257c; /* mov #0x18,r0 */
L8c61257c: sa_motion_publish_stop(ctx,0x8c61257cu); r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61257e; /* mov.l @r13,r3 */
L8c61257e: sa_motion_publish_stop(ctx,0x8c61257eu); fload(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612580; /* fmov.s @(r0,r3),fr3 */
L8c612580: sa_motion_publish_stop(ctx,0x8c612580u); r[0]=8u;goto L8c612582; /* mov #0x8,r0 */
L8c612582: sa_motion_publish_stop(ctx,0x8c612582u); fstore(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c612584; /* fmov fr3,@(r0,r5) */
L8c612584: sa_motion_publish_stop(ctx,0x8c612584u); r[3]=3u;goto L8c612586; /* mov #0x3,r3 */
L8c612586: sa_motion_publish_stop(ctx,0x8c612586u); r[11]+=1u;goto L8c612588; /* add #0x1,r11 */
L8c612588: sa_motion_publish_stop(ctx,0x8c612588u); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[3]);goto L8c61258a; /* cmp/ge r3,r11 */
L8c61258a: sa_motion_publish_stop(ctx,0x8c61258au); r[13]+=4u;goto L8c61258c; /* add #0x4,r13 */
L8c61258c: sa_motion_publish_stop(ctx,0x8c61258cu); r[5]+=12u;goto L8c61258e; /* add #0xc,r5 */
L8c61258e: sa_motion_publish_stop(ctx,0x8c612590u); take=!(*sr&1u);r[6]+=12u;if(take)goto L8c6124fa;goto L8c612592; /* bf/s 0x8c6124fa */
L8c612592: sa_motion_publish_stop(ctx,0x8c612592u); r[0]=8u;goto L8c612594; /* mov #0x8,r0 */
L8c612594: sa_motion_publish_stop(ctx,0x8c612594u); r[6]=r[9];goto L8c612596; /* mov r9,r6 */
L8c612596: sa_motion_publish_stop(ctx,0x8c612596u); fload(m,bus,&io_status,f,8u,r[0]+r[10]);if(io_status)return io_status;goto L8c612598; /* fmov.s @(r0,r10),fr8 */
L8c612598: sa_motion_publish_stop(ctx,0x8c612598u); r[6]+=12u;goto L8c61259a; /* add #0xc,r6 */
L8c61259a: sa_motion_publish_stop(ctx,0x8c61259au); r[0]=4u;goto L8c61259c; /* mov #0x4,r0 */
L8c61259c: sa_motion_publish_stop(ctx,0x8c61259cu); fload(m,bus,&io_status,f,11u,r[10]);if(io_status)return io_status;goto L8c61259e; /* fmov.s @r10,fr11 */
L8c61259e: sa_motion_publish_stop(ctx,0x8c61259eu); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c6125a0; /* fmov.s @(r0,r6),fr4 */
L8c6125a0: sa_motion_publish_stop(ctx,0x8c6125a0u); r[5]=r[9];goto L8c6125a2; /* mov r9,r5 */
L8c6125a2: sa_motion_publish_stop(ctx,0x8c6125a2u); fload(m,bus,&io_status,f,10u,r[0]+r[10]);if(io_status)return io_status;goto L8c6125a4; /* fmov.s @(r0,r10),fr10 */
L8c6125a4: sa_motion_publish_stop(ctx,0x8c6125a4u); r[0]=8u;goto L8c6125a6; /* mov #0x8,r0 */
L8c6125a6: sa_motion_publish_stop(ctx,0x8c6125a6u); fload(m,bus,&io_status,f,7u,r[0]+r[6]);if(io_status)return io_status;goto L8c6125a8; /* fmov.s @(r0,r6),fr7 */
L8c6125a8: sa_motion_publish_stop(ctx,0x8c6125a8u); r[0]=4u;goto L8c6125aa; /* mov #0x4,r0 */
L8c6125aa: sa_motion_publish_stop(ctx,0x8c6125aau); fcopy(f,3u,10u);goto L8c6125ac; /* fmov fr10,fr3 */
L8c6125ac: sa_motion_publish_stop(ctx,0x8c6125acu); r[5]+=24u;goto L8c6125ae; /* add #0x18,r5 */
L8c6125ae: sa_motion_publish_stop(ctx,0x8c6125aeu); fcopy(f,0u,8u);goto L8c6125b0; /* fmov fr8,fr0 */
L8c6125b0: sa_motion_publish_stop(ctx,0x8c6125b0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[4],&f->fpscr,&fr[0]);if(status)return status;}goto L8c6125b2; /* fmul fr4,fr0 */
L8c6125b2: sa_motion_publish_stop(ctx,0x8c6125b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6125b4; /* fmul fr7,fr3 */
L8c6125b4: sa_motion_publish_stop(ctx,0x8c6125b4u); fload(m,bus,&io_status,f,9u,r[6]);if(io_status)return io_status;goto L8c6125b6; /* fmov.s @r6,fr9 */
L8c6125b6: sa_motion_publish_stop(ctx,0x8c6125b6u); fcopy(f,2u,11u);goto L8c6125b8; /* fmov fr11,fr2 */
L8c6125b8: sa_motion_publish_stop(ctx,0x8c6125b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[7],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6125ba; /* fmul fr7,fr2 */
L8c6125ba: sa_motion_publish_stop(ctx,0x8c6125bau); fcopy(f,12u,11u);goto L8c6125bc; /* fmov fr11,fr12 */
L8c6125bc: sa_motion_publish_stop(ctx,0x8c6125bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[12],&fr[4],&f->fpscr,&fr[12]);if(status)return status;}goto L8c6125be; /* fmul fr4,fr12 */
L8c6125be: sa_motion_publish_stop(ctx,0x8c6125beu); fcopy(f,1u,9u);goto L8c6125c0; /* fmov fr9,fr1 */
L8c6125c0: sa_motion_publish_stop(ctx,0x8c6125c0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[10],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6125c2; /* fmul fr10,fr1 */
L8c6125c2: sa_motion_publish_stop(ctx,0x8c6125c2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status;}goto L8c6125c4; /* fsub fr3,fr0 */
L8c6125c4: sa_motion_publish_stop(ctx,0x8c6125c4u); fcopy(f,3u,8u);goto L8c6125c6; /* fmov fr8,fr3 */
L8c6125c6: sa_motion_publish_stop(ctx,0x8c6125c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[9],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6125c8; /* fmul fr9,fr3 */
L8c6125c8: sa_motion_publish_stop(ctx,0x8c6125c8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6125ca; /* fsub fr3,fr2 */
L8c6125ca: sa_motion_publish_stop(ctx,0x8c6125cau); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6125cc; /* fmov.s @(r0,r5),fr3 */
L8c6125cc: sa_motion_publish_stop(ctx,0x8c6125ccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[2],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6125ce; /* fmul fr2,fr3 */
L8c6125ce: sa_motion_publish_stop(ctx,0x8c6125ceu); fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c6125d0; /* fmov.s @r5,fr2 */
L8c6125d0: sa_motion_publish_stop(ctx,0x8c6125d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6125d2; /* fmac fr0,fr2,fr3 */
L8c6125d2: sa_motion_publish_stop(ctx,0x8c6125d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[12],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6125d4; /* fsub fr12,fr1 */
L8c6125d4: sa_motion_publish_stop(ctx,0x8c6125d4u); r[0]=8u;goto L8c6125d6; /* mov #0x8,r0 */
L8c6125d6: sa_motion_publish_stop(ctx,0x8c6125d6u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c6125d8; /* fmov.s @(r0,r5),fr2 */
L8c6125d8: sa_motion_publish_stop(ctx,0x8c6125d8u); r[5]=rd(m,bus,&io_status,0x8c612830u,4);if(io_status)return io_status;goto L8c6125da; /* mov.l 0x8c612830,r5 */
L8c6125da: sa_motion_publish_stop(ctx,0x8c6125dau); fcopy(f,0u,1u);goto L8c6125dc; /* fmov fr1,fr0 */
L8c6125dc: sa_motion_publish_stop(ctx,0x8c6125dcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6125de; /* fmac fr0,fr2,fr3 */
L8c6125de: sa_motion_publish_stop(ctx,0x8c6125deu); r[3]=rd(m,bus,&io_status,r[5],4);if(io_status)return io_status;goto L8c6125e0; /* mov.l @r5,r3 */
L8c6125e0: sa_motion_publish_stop(ctx,0x8c6125e0u); fcopy(f,4u,3u);goto L8c6125e2; /* fmov fr3,fr4 */
L8c6125e2: sa_motion_publish_stop(ctx,0x8c6125e2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6125e4; /* fldi0 fr3 */
L8c6125e4: sa_motion_publish_stop(ctx,0x8c6125e4u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][4],&f->fpscr,sr,1);if(status)return status;goto L8c6125e6; /* fcmp/gt fr4,fr3 */
L8c6125e6: sa_motion_publish_stop(ctx,0x8c6125e6u); r[0]=*sr&1u;goto L8c6125e8; /* movt r0 */
L8c6125e8: sa_motion_publish_stop(ctx,0x8c6125e8u); *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c6125ea; /* tst r3,r0 */
L8c6125ea: sa_motion_publish_stop(ctx,0x8c6125eau); if((*sr&1u))goto L8c6125f0;goto L8c6125ec; /* bt 0x8c6125f0 */
L8c6125ec: sa_motion_publish_stop(ctx,0x8c6125eeu); goto L8c612948; /* bra 0x8c612948 */
L8c6125f0: sa_motion_publish_stop(ctx,0x8c6125f0u); r[1]=0u;goto L8c6125f2; /* mov #0x0,r1 */
L8c6125f2: sa_motion_publish_stop(ctx,0x8c6125f2u); r[6]=rd(m,bus,&io_status,0x8c612834u,4);if(io_status)return io_status;goto L8c6125f4; /* mov.l 0x8c612834,r6 */
L8c6125f4: sa_motion_publish_stop(ctx,0x8c6125f4u); wr(m,bus,&io_status,r[5],r[1],4);if(io_status)return io_status;goto L8c6125f6; /* mov.l r1,@r5 */
L8c6125f6: sa_motion_publish_stop(ctx,0x8c6125f6u); r[3]=2u;goto L8c6125f8; /* mov #0x2,r3 */
L8c6125f8: sa_motion_publish_stop(ctx,0x8c6125f8u); r[5]=rd(m,bus,&io_status,0x8c612838u,4);if(io_status)return io_status;goto L8c6125fa; /* mov.l 0x8c612838,r5 */
L8c6125fa: sa_motion_publish_stop(ctx,0x8c6125fau); r[13]=r[1];goto L8c6125fc; /* mov r1,r13 */
L8c6125fc: sa_motion_publish_stop(ctx,0x8c6125fcu); wr(m,bus,&io_status,r[15]+0x8u,r[3],4);if(io_status)return io_status;goto L8c6125fe; /* mov.l r3,@(0x8,r15) */
L8c6125fe: sa_motion_publish_stop(ctx,0x8c6125feu); r[10]=r[13];goto L8c612600; /* mov r13,r10 */
L8c612600: sa_motion_publish_stop(ctx,0x8c612600u); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612602; /* mov.l @(0x8,r15),r3 */
L8c612602: sa_motion_publish_stop(ctx,0x8c612602u); r[0]=r[14];goto L8c612604; /* mov r14,r0 */
L8c612604: sa_motion_publish_stop(ctx,0x8c612604u); r[11]=r[10];goto L8c612606; /* mov r10,r11 */
L8c612606: sa_motion_publish_stop(ctx,0x8c612606u); r[11]<<=2;goto L8c612608; /* shll2 r11 */
L8c612608: sa_motion_publish_stop(ctx,0x8c612608u); r[3]<<=2;goto L8c61260a; /* shll2 r3 */
L8c61260a: sa_motion_publish_stop(ctx,0x8c61260au); wr(m,bus,&io_status,r[15]+0xcu,r[3],4);if(io_status)return io_status;goto L8c61260c; /* mov.l r3,@(0xc,r15) */
L8c61260c: sa_motion_publish_stop(ctx,0x8c61260cu); r[2]=rd(m,bus,&io_status,r[3]+r[0],4);if(io_status)return io_status;goto L8c61260e; /* mov.l @(r0,r3),r2 */
L8c61260e: sa_motion_publish_stop(ctx,0x8c61260eu); r[3]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612610; /* mov.l @(0xc,r2),r3 */
L8c612610: sa_motion_publish_stop(ctx,0x8c612610u); r[2]=rd(m,bus,&io_status,r[11]+r[0],4);if(io_status)return io_status;goto L8c612612; /* mov.l @(r0,r11),r2 */
L8c612612: sa_motion_publish_stop(ctx,0x8c612612u); r[3]&=r[4];goto L8c612614; /* and r4,r3 */
L8c612614: sa_motion_publish_stop(ctx,0x8c612614u); if(((r[2]+12u)&0x1f000000u)!=0x0c000000u)return -3;r[2]=rd(m,bus,&io_status,r[2]+0xcu,4);if(io_status)return io_status;goto L8c612616; /* mov.l @(0xc,r2),r2 */
L8c612616: sa_motion_publish_stop(ctx,0x8c612616u); r[2]&=r[4];goto L8c612618; /* and r4,r2 */
L8c612618: sa_motion_publish_stop(ctx,0x8c612618u); r[3]^=r[2];goto L8c61261a; /* xor r2,r3 */
L8c61261a: sa_motion_publish_stop(ctx,0x8c61261au); r[0]=r[3];goto L8c61261c; /* mov r3,r0 */
L8c61261c: sa_motion_publish_stop(ctx,0x8c61261cu); *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c61261e; /* cmp/eq #0x1,r0 */
L8c61261e: sa_motion_publish_stop(ctx,0x8c61261eu); if(!(*sr&1u))goto L8c612702;goto L8c612620; /* bf 0x8c612702 */
L8c612620: sa_motion_publish_stop(ctx,0x8c612620u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612622; /* mov.l @(0x8,r15),r2 */
L8c612622: sa_motion_publish_stop(ctx,0x8c612622u); r[0]=8u;goto L8c612624; /* mov #0x8,r0 */
L8c612624: sa_motion_publish_stop(ctx,0x8c612624u); r[1]=r[10];goto L8c612626; /* mov r10,r1 */
L8c612626: sa_motion_publish_stop(ctx,0x8c612626u); r[3]=r[2];goto L8c612628; /* mov r2,r3 */
L8c612628: sa_motion_publish_stop(ctx,0x8c612628u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c61262a; /* shll r2 */
L8c61262a: sa_motion_publish_stop(ctx,0x8c61262au); r[2]+=r[3];goto L8c61262c; /* add r3,r2 */
L8c61262c: sa_motion_publish_stop(ctx,0x8c61262cu); r[3]=rd(m,bus,&io_status,0x8c61283cu,4);if(io_status)return io_status;goto L8c61262e; /* mov.l 0x8c61283c,r3 */
L8c61262e: sa_motion_publish_stop(ctx,0x8c61262eu); r[2]<<=2;goto L8c612630; /* shll2 r2 */
L8c612630: sa_motion_publish_stop(ctx,0x8c612630u); r[2]+=r[9];goto L8c612632; /* add r9,r2 */
L8c612632: sa_motion_publish_stop(ctx,0x8c612632u); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c612634; /* mov.l r2,@r15 */
L8c612634: sa_motion_publish_stop(ctx,0x8c612634u); fload(m,bus,&io_status,f,4u,r[0]+r[2]);if(io_status)return io_status;goto L8c612636; /* fmov.s @(r0,r2),fr4 */
L8c612636: sa_motion_publish_stop(ctx,0x8c612636u); r[2]=r[10];goto L8c612638; /* mov r10,r2 */
L8c612638: sa_motion_publish_stop(ctx,0x8c612638u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c61263a; /* shll r2 */
L8c61263a: sa_motion_publish_stop(ctx,0x8c61263au); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c61263c; /* fmov.s @r3,fr3 */
L8c61263c: sa_motion_publish_stop(ctx,0x8c61263cu); r[2]+=r[1];goto L8c61263e; /* add r1,r2 */
L8c61263e: sa_motion_publish_stop(ctx,0x8c61263eu); r[2]<<=2;goto L8c612640; /* shll2 r2 */
L8c612640: sa_motion_publish_stop(ctx,0x8c612640u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612642; /* fsub fr4,fr3 */
L8c612642: sa_motion_publish_stop(ctx,0x8c612642u); r[0]=8u;goto L8c612644; /* mov #0x8,r0 */
L8c612644: sa_motion_publish_stop(ctx,0x8c612644u); r[2]+=r[9];goto L8c612646; /* add r9,r2 */
L8c612646: sa_motion_publish_stop(ctx,0x8c612646u); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c612648; /* mov.l r2,@(0x4,r15) */
L8c612648: sa_motion_publish_stop(ctx,0x8c612648u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61264a; /* fmov.s @(r0,r2),fr2 */
L8c61264a: sa_motion_publish_stop(ctx,0x8c61264au); r[1]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61264c; /* mov.l @(0x4,r15),r1 */
L8c61264c: sa_motion_publish_stop(ctx,0x8c61264cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[4],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61264e; /* fsub fr4,fr2 */
L8c61264e: sa_motion_publish_stop(ctx,0x8c61264eu); fcopy(f,4u,3u);goto L8c612650; /* fmov fr3,fr4 */
L8c612650: sa_motion_publish_stop(ctx,0x8c612650u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612652; /* mov.l @r15,r2 */
L8c612652: sa_motion_publish_stop(ctx,0x8c612652u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c612654; /* fmov.s @r1,fr3 */
L8c612654: sa_motion_publish_stop(ctx,0x8c612654u); fload(m,bus,&io_status,f,7u,r[2]);if(io_status)return io_status;goto L8c612656; /* fmov.s @r2,fr7 */
L8c612656: sa_motion_publish_stop(ctx,0x8c612656u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_divide(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status;}goto L8c612658; /* fdiv fr2,fr4 */
L8c612658: sa_motion_publish_stop(ctx,0x8c612658u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61265a; /* fsub fr7,fr3 */
L8c61265a: sa_motion_publish_stop(ctx,0x8c61265au); fcopy(f,2u,7u);goto L8c61265c; /* fmov fr7,fr2 */
L8c61265c: sa_motion_publish_stop(ctx,0x8c61265cu); fcopy(f,0u,4u);goto L8c61265e; /* fmov fr4,fr0 */
L8c61265e: sa_motion_publish_stop(ctx,0x8c61265eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612660; /* fmac fr0,fr3,fr2 */
L8c612660: sa_motion_publish_stop(ctx,0x8c612660u); r[1]=rd(m,bus,&io_status,0x8c612840u,4);if(io_status)return io_status;goto L8c612662; /* mov.l 0x8c612840,r1 */
L8c612662: sa_motion_publish_stop(ctx,0x8c612662u); r[0]=12u;goto L8c612664; /* mov #0xc,r0 */
L8c612664: sa_motion_publish_stop(ctx,0x8c612664u); fcopy(f,7u,2u);goto L8c612666; /* fmov fr2,fr7 */
L8c612666: sa_motion_publish_stop(ctx,0x8c612666u); r[2]=r[13];goto L8c612668; /* mov r13,r2 */
L8c612668: sa_motion_publish_stop(ctx,0x8c612668u); fcopy(f,0u,6u);goto L8c61266a; /* fmov fr6,fr0 */
L8c61266a: sa_motion_publish_stop(ctx,0x8c61266au); r[2]<<=2;goto L8c61266c; /* shll2 r2 */
L8c61266c: sa_motion_publish_stop(ctx,0x8c61266cu); r[2]<<=2;goto L8c61266e; /* shll2 r2 */
L8c61266e: sa_motion_publish_stop(ctx,0x8c61266eu); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c612670; /* shll r2 */
L8c612670: sa_motion_publish_stop(ctx,0x8c612670u); r[2]+=r[5];goto L8c612672; /* add r5,r2 */
L8c612672: sa_motion_publish_stop(ctx,0x8c612672u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c612674; /* mov.l r2,@(0x8,r15) */
L8c612674: sa_motion_publish_stop(ctx,0x8c612674u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c612676; /* fmov.s @r1,fr3 */
L8c612676: sa_motion_publish_stop(ctx,0x8c612676u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c612678; /* fmov.s @(r0,r12),fr2 */
L8c612678: sa_motion_publish_stop(ctx,0x8c612678u); r[0]=16u;goto L8c61267a; /* mov #0x10,r0 */
L8c61267a: sa_motion_publish_stop(ctx,0x8c61267au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61267c; /* fmul fr7,fr3 */
L8c61267c: sa_motion_publish_stop(ctx,0x8c61267cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61267e; /* fmac fr0,fr3,fr2 */
L8c61267e: sa_motion_publish_stop(ctx,0x8c61267eu); fcopy(f,0u,4u);goto L8c612680; /* fmov fr4,fr0 */
L8c612680: sa_motion_publish_stop(ctx,0x8c612680u); fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c612682; /* fmov fr2,@(r0,r2) */
L8c612682: sa_motion_publish_stop(ctx,0x8c612682u); r[0]=4u;goto L8c612684; /* mov #0x4,r0 */
L8c612684: sa_motion_publish_stop(ctx,0x8c612684u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612686; /* mov.l @r15,r2 */
L8c612686: sa_motion_publish_stop(ctx,0x8c612686u); fload(m,bus,&io_status,f,7u,r[0]+r[2]);if(io_status)return io_status;goto L8c612688; /* fmov.s @(r0,r2),fr7 */
L8c612688: sa_motion_publish_stop(ctx,0x8c612688u); r[0]=4u;goto L8c61268a; /* mov #0x4,r0 */
L8c61268a: sa_motion_publish_stop(ctx,0x8c61268au); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61268c; /* mov.l @(0x4,r15),r2 */
L8c61268c: sa_motion_publish_stop(ctx,0x8c61268cu); fcopy(f,2u,7u);goto L8c61268e; /* fmov fr7,fr2 */
L8c61268e: sa_motion_publish_stop(ctx,0x8c61268eu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612690; /* fmov.s @(r0,r2),fr3 */
L8c612690: sa_motion_publish_stop(ctx,0x8c612690u); r[0]=rd(m,bus,&io_status,0x8c612844u,4);if(io_status)return io_status;goto L8c612692; /* mov.l 0x8c612844,r0 */
L8c612692: sa_motion_publish_stop(ctx,0x8c612692u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[7],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c612694; /* fsub fr7,fr3 */
L8c612694: sa_motion_publish_stop(ctx,0x8c612694u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c612696; /* mov.l @(0x8,r15),r2 */
L8c612696: sa_motion_publish_stop(ctx,0x8c612696u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c612698; /* fmac fr0,fr3,fr2 */
L8c612698: sa_motion_publish_stop(ctx,0x8c612698u); fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c61269a; /* fmov.s @r0,fr3 */
L8c61269a: sa_motion_publish_stop(ctx,0x8c61269au); r[0]=16u;goto L8c61269c; /* mov #0x10,r0 */
L8c61269c: sa_motion_publish_stop(ctx,0x8c61269cu); fcopy(f,7u,2u);goto L8c61269e; /* fmov fr2,fr7 */
L8c61269e: sa_motion_publish_stop(ctx,0x8c61269eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[7],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6126a0; /* fmul fr7,fr3 */
L8c6126a0: sa_motion_publish_stop(ctx,0x8c6126a0u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c6126a2; /* fmov.s @(r0,r12),fr2 */
L8c6126a2: sa_motion_publish_stop(ctx,0x8c6126a2u); r[0]=20u;goto L8c6126a4; /* mov #0x14,r0 */
L8c6126a4: sa_motion_publish_stop(ctx,0x8c6126a4u); fcopy(f,0u,6u);goto L8c6126a6; /* fmov fr6,fr0 */
L8c6126a6: sa_motion_publish_stop(ctx,0x8c6126a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6126a8; /* fmac fr0,fr3,fr2 */
L8c6126a8: sa_motion_publish_stop(ctx,0x8c6126a8u); fstore(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c6126aa; /* fmov fr2,@(r0,r2) */
L8c6126aa: sa_motion_publish_stop(ctx,0x8c6126aau); r[0]=24u;goto L8c6126ac; /* mov #0x18,r0 */
L8c6126ac: sa_motion_publish_stop(ctx,0x8c6126acu); r[3]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6126ae; /* mov.l @(0x8,r15),r3 */
L8c6126ae: sa_motion_publish_stop(ctx,0x8c6126aeu); fstore(m,bus,&io_status,f,0u,r[0]+r[3]);if(io_status)return io_status;goto L8c6126b0; /* fmov fr0,@(r0,r3) */
L8c6126b0: sa_motion_publish_stop(ctx,0x8c6126b0u); r[0]=r[11];goto L8c6126b2; /* mov r11,r0 */
L8c6126b2: sa_motion_publish_stop(ctx,0x8c6126b2u); r[0]+=r[7];goto L8c6126b4; /* add r7,r0 */
L8c6126b4: sa_motion_publish_stop(ctx,0x8c6126b4u); fcopy(f,0u,4u);goto L8c6126b6; /* fmov fr4,fr0 */
L8c6126b6: sa_motion_publish_stop(ctx,0x8c6126b6u); r[3]=r[13];goto L8c6126b8; /* mov r13,r3 */
L8c6126b8: sa_motion_publish_stop(ctx,0x8c6126b8u); r[3]<<=2;goto L8c6126ba; /* shll2 r3 */
L8c6126ba: sa_motion_publish_stop(ctx,0x8c6126bau); r[3]+=r[6];goto L8c6126bc; /* add r6,r3 */
L8c6126bc: sa_motion_publish_stop(ctx,0x8c6126bcu); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c6126be; /* mov.l r3,@(0x4,r15) */
L8c6126be: sa_motion_publish_stop(ctx,0x8c6126beu); r[2]=rd(m,bus,&io_status,r[15]+0xcu,4);if(io_status)return io_status;goto L8c6126c0; /* mov.l @(0xc,r15),r2 */
L8c6126c0: sa_motion_publish_stop(ctx,0x8c6126c0u); r[2]+=r[7];goto L8c6126c2; /* add r7,r2 */
L8c6126c2: sa_motion_publish_stop(ctx,0x8c6126c2u); wr(m,bus,&io_status,r[15]+0x8u,r[2],4);if(io_status)return io_status;goto L8c6126c4; /* mov.l r2,@(0x8,r15) */
L8c6126c4: sa_motion_publish_stop(ctx,0x8c6126c4u); r[2]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c6126c6; /* mov.w @r2,r2 */
L8c6126c6: sa_motion_publish_stop(ctx,0x8c6126c6u); wr(m,bus,&io_status,r[15],r[2],2);if(io_status)return io_status;goto L8c6126c8; /* mov.w r2,@r15 */
L8c6126c8: sa_motion_publish_stop(ctx,0x8c6126c8u); *fpul=r[2];goto L8c6126ca; /* lds r2,FPUL */
L8c6126ca: sa_motion_publish_stop(ctx,0x8c6126cau); wr(m,bus,&io_status,r[15]+0x10u,r[0],4);if(io_status)return io_status;goto L8c6126cc; /* mov.l r0,@(0x10,r15) */
L8c6126cc: sa_motion_publish_stop(ctx,0x8c6126ccu); r[2]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c6126ce; /* mov.w @r0,r2 */
L8c6126ce: sa_motion_publish_stop(ctx,0x8c6126ceu); r[0]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c6126d0; /* mov.w @r15,r0 */
L8c6126d0: sa_motion_publish_stop(ctx,0x8c6126d0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6126d2; /* float FPUL,fr3 */
L8c6126d2: sa_motion_publish_stop(ctx,0x8c6126d2u); r[2]-=r[0];goto L8c6126d4; /* sub r0,r2 */
L8c6126d4: sa_motion_publish_stop(ctx,0x8c6126d4u); *fpul=r[2];goto L8c6126d6; /* lds r2,FPUL */
L8c6126d6: sa_motion_publish_stop(ctx,0x8c6126d6u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c6126d8; /* float FPUL,fr2 */
L8c6126d8: sa_motion_publish_stop(ctx,0x8c6126d8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6126da; /* fmac fr0,fr2,fr3 */
L8c6126da: sa_motion_publish_stop(ctx,0x8c6126dau); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6126dc; /* ftrc fr3,FPUL */
L8c6126dc: sa_motion_publish_stop(ctx,0x8c6126dcu); r[2]=*fpul;goto L8c6126de; /* sts FPUL,r2 */
L8c6126de: sa_motion_publish_stop(ctx,0x8c6126deu); wr(m,bus,&io_status,r[3],r[2],2);if(io_status)return io_status;goto L8c6126e0; /* mov.w r2,@r3 */
L8c6126e0: sa_motion_publish_stop(ctx,0x8c6126e0u); r[2]=rd(m,bus,&io_status,r[15]+0x8u,4);if(io_status)return io_status;goto L8c6126e2; /* mov.l @(0x8,r15),r2 */
L8c6126e2: sa_motion_publish_stop(ctx,0x8c6126e2u); r[13]+=1u;goto L8c6126e4; /* add #0x1,r13 */
L8c6126e4: sa_motion_publish_stop(ctx,0x8c6126e4u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c6126e6; /* mov.l @(0x4,r15),r3 */
L8c6126e6: sa_motion_publish_stop(ctx,0x8c6126e6u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c6126e8; /* mov.w @(0x2,r2),r0 */
L8c6126e8: sa_motion_publish_stop(ctx,0x8c6126e8u); *fpul=r[0];goto L8c6126ea; /* lds r0,FPUL */
L8c6126ea: sa_motion_publish_stop(ctx,0x8c6126eau); wr(m,bus,&io_status,r[15],r[0],2);if(io_status)return io_status;goto L8c6126ec; /* mov.w r0,@r15 */
L8c6126ec: sa_motion_publish_stop(ctx,0x8c6126ecu); r[2]=rd(m,bus,&io_status,r[15]+0x10u,4);if(io_status)return io_status;goto L8c6126ee; /* mov.l @(0x10,r15),r2 */
L8c6126ee: sa_motion_publish_stop(ctx,0x8c6126eeu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6126f0; /* float FPUL,fr3 */
L8c6126f0: sa_motion_publish_stop(ctx,0x8c6126f0u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c6126f2; /* mov.w @(0x2,r2),r0 */
L8c6126f2: sa_motion_publish_stop(ctx,0x8c6126f2u); r[2]=sx(m,bus,&io_status,r[15],2);if(io_status)return io_status;goto L8c6126f4; /* mov.w @r15,r2 */
L8c6126f4: sa_motion_publish_stop(ctx,0x8c6126f4u); r[0]-=r[2];goto L8c6126f6; /* sub r2,r0 */
L8c6126f6: sa_motion_publish_stop(ctx,0x8c6126f6u); *fpul=r[0];goto L8c6126f8; /* lds r0,FPUL */
L8c6126f8: sa_motion_publish_stop(ctx,0x8c6126f8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][2]);if(status)return status;goto L8c6126fa; /* float FPUL,fr2 */
L8c6126fa: sa_motion_publish_stop(ctx,0x8c6126fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6126fc; /* fmac fr0,fr2,fr3 */
L8c6126fc: sa_motion_publish_stop(ctx,0x8c6126fcu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c6126fe; /* ftrc fr3,FPUL */
L8c6126fe: sa_motion_publish_stop(ctx,0x8c6126feu); r[0]=*fpul;goto L8c612700; /* sts FPUL,r0 */
L8c612700: sa_motion_publish_stop(ctx,0x8c612700u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c612702; /* mov.w r0,@(0x2,r3) */
L8c612702: sa_motion_publish_stop(ctx,0x8c612702u); r[0]=r[11];goto L8c612704; /* mov r11,r0 */
L8c612704: sa_motion_publish_stop(ctx,0x8c612704u); r[3]=rd(m,bus,&io_status,r[14]+r[0],4);if(io_status)return io_status;goto L8c612706; /* mov.l @(r0,r14),r3 */
L8c612706: sa_motion_publish_stop(ctx,0x8c612706u); r[3]=rd(m,bus,&io_status,r[3]+0xcu,4);if(io_status)return io_status;goto L8c612708; /* mov.l @(0xc,r3),r3 */
L8c612708: sa_motion_publish_stop(ctx,0x8c612708u); *sr=(*sr&~1u)|!(r[4]&r[3]);goto L8c61270a; /* tst r4,r3 */
L8c61270a: sa_motion_publish_stop(ctx,0x8c61270au); if(!(*sr&1u))goto L8c61275a;goto L8c61270c; /* bf 0x8c61275a */
L8c61270c: sa_motion_publish_stop(ctx,0x8c61270cu); r[3]=r[11];goto L8c61270e; /* mov r11,r3 */
L8c61270e: sa_motion_publish_stop(ctx,0x8c61270eu); r[2]=r[13];goto L8c612710; /* mov r13,r2 */
L8c612710: sa_motion_publish_stop(ctx,0x8c612710u); r[3]+=r[14];goto L8c612712; /* add r14,r3 */
L8c612712: sa_motion_publish_stop(ctx,0x8c612712u); r[2]<<=2;goto L8c612714; /* shll2 r2 */
L8c612714: sa_motion_publish_stop(ctx,0x8c612714u); r[0]=16u;goto L8c612716; /* mov #0x10,r0 */
L8c612716: sa_motion_publish_stop(ctx,0x8c612716u); r[2]<<=2;goto L8c612718; /* shll2 r2 */
L8c612718: sa_motion_publish_stop(ctx,0x8c612718u); *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1;goto L8c61271a; /* shll r2 */
L8c61271a: sa_motion_publish_stop(ctx,0x8c61271au); r[2]+=r[5];goto L8c61271c; /* add r5,r2 */
L8c61271c: sa_motion_publish_stop(ctx,0x8c61271cu); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61271e; /* mov.l r2,@r15 */
L8c61271e: sa_motion_publish_stop(ctx,0x8c61271eu); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c612720; /* mov.l r3,@(0x4,r15) */
L8c612720: sa_motion_publish_stop(ctx,0x8c612720u); r[1]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c612722; /* mov.l @r3,r1 */
L8c612722: sa_motion_publish_stop(ctx,0x8c612722u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612724; /* fmov.s @(r0,r1),fr3 */
L8c612724: sa_motion_publish_stop(ctx,0x8c612724u); fstore(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c612726; /* fmov fr3,@(r0,r2) */
L8c612726: sa_motion_publish_stop(ctx,0x8c612726u); r[0]=20u;goto L8c612728; /* mov #0x14,r0 */
L8c612728: sa_motion_publish_stop(ctx,0x8c612728u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61272a; /* mov.l @(0x4,r15),r2 */
L8c61272a: sa_motion_publish_stop(ctx,0x8c61272au); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61272c; /* mov.l @r15,r3 */
L8c61272c: sa_motion_publish_stop(ctx,0x8c61272cu); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61272e; /* mov.l @r2,r1 */
L8c61272e: sa_motion_publish_stop(ctx,0x8c61272eu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612730; /* fmov.s @(r0,r1),fr3 */
L8c612730: sa_motion_publish_stop(ctx,0x8c612730u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612732; /* fmov fr3,@(r0,r3) */
L8c612732: sa_motion_publish_stop(ctx,0x8c612732u); r[0]=24u;goto L8c612734; /* mov #0x18,r0 */
L8c612734: sa_motion_publish_stop(ctx,0x8c612734u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612736; /* mov.l @(0x4,r15),r2 */
L8c612736: sa_motion_publish_stop(ctx,0x8c612736u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612738; /* mov.l @r15,r3 */
L8c612738: sa_motion_publish_stop(ctx,0x8c612738u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61273a; /* mov.l @r2,r1 */
L8c61273a: sa_motion_publish_stop(ctx,0x8c61273au); r[2]=r[11];goto L8c61273c; /* mov r11,r2 */
L8c61273c: sa_motion_publish_stop(ctx,0x8c61273cu); r[2]+=r[7];goto L8c61273e; /* add r7,r2 */
L8c61273e: sa_motion_publish_stop(ctx,0x8c61273eu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c612740; /* fmov.s @(r0,r1),fr3 */
L8c612740: sa_motion_publish_stop(ctx,0x8c612740u); fstore(m,bus,&io_status,f,3u,r[0]+r[3]);if(io_status)return io_status;goto L8c612742; /* fmov fr3,@(r0,r3) */
L8c612742: sa_motion_publish_stop(ctx,0x8c612742u); r[3]=r[13];goto L8c612744; /* mov r13,r3 */
L8c612744: sa_motion_publish_stop(ctx,0x8c612744u); r[3]<<=2;goto L8c612746; /* shll2 r3 */
L8c612746: sa_motion_publish_stop(ctx,0x8c612746u); r[3]+=r[6];goto L8c612748; /* add r6,r3 */
L8c612748: sa_motion_publish_stop(ctx,0x8c612748u); wr(m,bus,&io_status,r[15]+0x4u,r[3],4);if(io_status)return io_status;goto L8c61274a; /* mov.l r3,@(0x4,r15) */
L8c61274a: sa_motion_publish_stop(ctx,0x8c61274au); wr(m,bus,&io_status,r[15],r[2],4);if(io_status)return io_status;goto L8c61274c; /* mov.l r2,@r15 */
L8c61274c: sa_motion_publish_stop(ctx,0x8c61274cu); r[1]=sx(m,bus,&io_status,r[2],2);if(io_status)return io_status;goto L8c61274e; /* mov.w @r2,r1 */
L8c61274e: sa_motion_publish_stop(ctx,0x8c61274eu); r[13]+=1u;goto L8c612750; /* add #0x1,r13 */
L8c612750: sa_motion_publish_stop(ctx,0x8c612750u); wr(m,bus,&io_status,r[3],r[1],2);if(io_status)return io_status;goto L8c612752; /* mov.w r1,@r3 */
L8c612752: sa_motion_publish_stop(ctx,0x8c612752u); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612754; /* mov.l @r15,r0 */
L8c612754: sa_motion_publish_stop(ctx,0x8c612754u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c612756; /* mov.l @(0x4,r15),r3 */
L8c612756: sa_motion_publish_stop(ctx,0x8c612756u); r[0]=sx(m,bus,&io_status,r[0]+0x2u,2);if(io_status)return io_status;goto L8c612758; /* mov.w @(0x2,r0),r0 */
L8c612758: sa_motion_publish_stop(ctx,0x8c612758u); wr(m,bus,&io_status,r[3]+0x2u,r[0],2);if(io_status)return io_status;goto L8c61275a; /* mov.w r0,@(0x2,r3) */
L8c61275a: sa_motion_publish_stop(ctx,0x8c61275au); wr(m,bus,&io_status,r[15]+0x8u,r[10],4);if(io_status)return io_status;goto L8c61275c; /* mov.l r10,@(0x8,r15) */
L8c61275c: sa_motion_publish_stop(ctx,0x8c61275cu); r[3]=3u;goto L8c61275e; /* mov #0x3,r3 */
L8c61275e: sa_motion_publish_stop(ctx,0x8c61275eu); r[10]+=1u;goto L8c612760; /* add #0x1,r10 */
L8c612760: sa_motion_publish_stop(ctx,0x8c612760u); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[3]);goto L8c612762; /* cmp/ge r3,r10 */
L8c612762: sa_motion_publish_stop(ctx,0x8c612762u); if((*sr&1u))goto L8c612768;goto L8c612764; /* bt 0x8c612768 */
L8c612764: sa_motion_publish_stop(ctx,0x8c612766u); goto L8c612600; /* bra 0x8c612600 */
L8c612768: sa_motion_publish_stop(ctx,0x8c612768u); r[0]=0x8c612850u;goto L8c61276a; /* mova 0x8c612850,r0 */
L8c61276a: sa_motion_publish_stop(ctx,0x8c61276au); r[4]=rd(m,bus,&io_status,0x8c61284cu,4);if(io_status)return io_status;goto L8c61276c; /* mov.l 0x8c61284c,r4 */
L8c61276c: sa_motion_publish_stop(ctx,0x8c61276cu); fload(m,bus,&io_status,f,4u,r[0]);if(io_status)return io_status;goto L8c61276e; /* fmov.s @r0,fr4 */
L8c61276e: sa_motion_publish_stop(ctx,0x8c61276eu); r[0]=r[13];goto L8c612770; /* mov r13,r0 */
L8c612770: sa_motion_publish_stop(ctx,0x8c612770u); r[7]=rd(m,bus,&io_status,0x8c612848u,4);if(io_status)return io_status;goto L8c612772; /* mov.l 0x8c612848,r7 */
L8c612772: sa_motion_publish_stop(ctx,0x8c612772u); *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c612774; /* cmp/eq #0x3,r0 */
L8c612774: sa_motion_publish_stop(ctx,0x8c612774u); if(!(*sr&1u))goto L8c612858;goto L8c612776; /* bf 0x8c612858 */
L8c612776: sa_motion_publish_stop(ctx,0x8c612776u); r[3]=r[6];goto L8c612778; /* mov r6,r3 */
L8c612778: sa_motion_publish_stop(ctx,0x8c612778u); r[8]+=28u;goto L8c61277a; /* add #0x1c,r8 */
L8c61277a: sa_motion_publish_stop(ctx,0x8c61277au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61277c; /* fmov.s fr5,@-r8 */
L8c61277c: sa_motion_publish_stop(ctx,0x8c61277cu); r[14]=r[5];goto L8c61277e; /* mov r5,r14 */
L8c61277e: sa_motion_publish_stop(ctx,0x8c61277eu); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612780; /* mov.l r3,@r15 */
L8c612780: sa_motion_publish_stop(ctx,0x8c612780u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c612782; /* mov.w @(0x2,r3),r0 */
L8c612782: sa_motion_publish_stop(ctx,0x8c612782u); r[3]=r[0];goto L8c612784; /* mov r0,r3 */
L8c612784: sa_motion_publish_stop(ctx,0x8c612784u); *fpul=r[3];goto L8c612786; /* lds r3,FPUL */
L8c612786: sa_motion_publish_stop(ctx,0x8c612786u); r[0]=24u;goto L8c612788; /* mov #0x18,r0 */
L8c612788: sa_motion_publish_stop(ctx,0x8c612788u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61278a; /* float FPUL,fr3 */
L8c61278a: sa_motion_publish_stop(ctx,0x8c61278au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61278c; /* fmul fr4,fr3 */
L8c61278c: sa_motion_publish_stop(ctx,0x8c61278cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61278e; /* fmov.s fr3,@-r8 */
L8c61278e: sa_motion_publish_stop(ctx,0x8c61278eu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612790; /* mov.l @r15,r3 */
L8c612790: sa_motion_publish_stop(ctx,0x8c612790u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c612792; /* mov.w @r3,r3 */
L8c612792: sa_motion_publish_stop(ctx,0x8c612792u); *fpul=r[3];goto L8c612794; /* lds r3,FPUL */
L8c612794: sa_motion_publish_stop(ctx,0x8c612794u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612796; /* float FPUL,fr3 */
L8c612796: sa_motion_publish_stop(ctx,0x8c612796u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c612798; /* fmul fr4,fr3 */
L8c612798: sa_motion_publish_stop(ctx,0x8c612798u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61279a; /* fmov.s fr3,@-r8 */
L8c61279a: sa_motion_publish_stop(ctx,0x8c61279au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61279c; /* fmov.s @(r0,r14),fr3 */
L8c61279c: sa_motion_publish_stop(ctx,0x8c61279cu); r[0]=20u;goto L8c61279e; /* mov #0x14,r0 */
L8c61279e: sa_motion_publish_stop(ctx,0x8c61279eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127a0; /* fmov.s fr3,@-r8 */
L8c6127a0: sa_motion_publish_stop(ctx,0x8c6127a0u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6127a2; /* fmov.s @(r0,r14),fr3 */
L8c6127a2: sa_motion_publish_stop(ctx,0x8c6127a2u); r[0]=16u;goto L8c6127a4; /* mov #0x10,r0 */
L8c6127a4: sa_motion_publish_stop(ctx,0x8c6127a4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127a6; /* fmov.s fr3,@-r8 */
L8c6127a6: sa_motion_publish_stop(ctx,0x8c6127a6u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6127a8; /* fmov.s @(r0,r14),fr3 */
L8c6127a8: sa_motion_publish_stop(ctx,0x8c6127a8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127aa; /* fmov.s fr3,@-r8 */
L8c6127aa: sa_motion_publish_stop(ctx,0x8c6127aau); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c6127ac; /* mov.l r4,@-r8 */
L8c6127ac: sa_motion_publish_stop(ctx,0x8c6127acu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6127ae; /* pref @r8 */
L8c6127ae: sa_motion_publish_stop(ctx,0x8c6127aeu); r[3]=r[6];goto L8c6127b0; /* mov r6,r3 */
L8c6127b0: sa_motion_publish_stop(ctx,0x8c6127b0u); r[3]+=4u;goto L8c6127b2; /* add #0x4,r3 */
L8c6127b2: sa_motion_publish_stop(ctx,0x8c6127b2u); r[8]+=60u;goto L8c6127b4; /* add #0x3c,r8 */
L8c6127b4: sa_motion_publish_stop(ctx,0x8c6127b4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6127b6; /* fmov.s fr5,@-r8 */
L8c6127b6: sa_motion_publish_stop(ctx,0x8c6127b6u); r[14]+=32u;goto L8c6127b8; /* add #0x20,r14 */
L8c6127b8: sa_motion_publish_stop(ctx,0x8c6127b8u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6127ba; /* mov.l r3,@r15 */
L8c6127ba: sa_motion_publish_stop(ctx,0x8c6127bau); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6127bc; /* mov.w @(0x2,r3),r0 */
L8c6127bc: sa_motion_publish_stop(ctx,0x8c6127bcu); r[3]=r[0];goto L8c6127be; /* mov r0,r3 */
L8c6127be: sa_motion_publish_stop(ctx,0x8c6127beu); *fpul=r[3];goto L8c6127c0; /* lds r3,FPUL */
L8c6127c0: sa_motion_publish_stop(ctx,0x8c6127c0u); r[0]=24u;goto L8c6127c2; /* mov #0x18,r0 */
L8c6127c2: sa_motion_publish_stop(ctx,0x8c6127c2u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6127c4; /* float FPUL,fr3 */
L8c6127c4: sa_motion_publish_stop(ctx,0x8c6127c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6127c6; /* fmul fr4,fr3 */
L8c6127c6: sa_motion_publish_stop(ctx,0x8c6127c6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127c8; /* fmov.s fr3,@-r8 */
L8c6127c8: sa_motion_publish_stop(ctx,0x8c6127c8u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6127ca; /* mov.l @r15,r3 */
L8c6127ca: sa_motion_publish_stop(ctx,0x8c6127cau); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6127cc; /* mov.w @r3,r3 */
L8c6127cc: sa_motion_publish_stop(ctx,0x8c6127ccu); *fpul=r[3];goto L8c6127ce; /* lds r3,FPUL */
L8c6127ce: sa_motion_publish_stop(ctx,0x8c6127ceu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6127d0; /* float FPUL,fr3 */
L8c6127d0: sa_motion_publish_stop(ctx,0x8c6127d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6127d2; /* fmul fr4,fr3 */
L8c6127d2: sa_motion_publish_stop(ctx,0x8c6127d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127d4; /* fmov.s fr3,@-r8 */
L8c6127d4: sa_motion_publish_stop(ctx,0x8c6127d4u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6127d6; /* fmov.s @(r0,r14),fr3 */
L8c6127d6: sa_motion_publish_stop(ctx,0x8c6127d6u); r[0]=20u;goto L8c6127d8; /* mov #0x14,r0 */
L8c6127d8: sa_motion_publish_stop(ctx,0x8c6127d8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127da; /* fmov.s fr3,@-r8 */
L8c6127da: sa_motion_publish_stop(ctx,0x8c6127dau); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6127dc; /* fmov.s @(r0,r14),fr3 */
L8c6127dc: sa_motion_publish_stop(ctx,0x8c6127dcu); r[0]=16u;goto L8c6127de; /* mov #0x10,r0 */
L8c6127de: sa_motion_publish_stop(ctx,0x8c6127deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127e0; /* fmov.s fr3,@-r8 */
L8c6127e0: sa_motion_publish_stop(ctx,0x8c6127e0u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6127e2; /* fmov.s @(r0,r14),fr3 */
L8c6127e2: sa_motion_publish_stop(ctx,0x8c6127e2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127e4; /* fmov.s fr3,@-r8 */
L8c6127e4: sa_motion_publish_stop(ctx,0x8c6127e4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c6127e6; /* mov.l r4,@-r8 */
L8c6127e6: sa_motion_publish_stop(ctx,0x8c6127e6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6127e8; /* pref @r8 */
L8c6127e8: sa_motion_publish_stop(ctx,0x8c6127e8u); r[4]=r[6];goto L8c6127ea; /* mov r6,r4 */
L8c6127ea: sa_motion_publish_stop(ctx,0x8c6127eau); r[4]+=8u;goto L8c6127ec; /* add #0x8,r4 */
L8c6127ec: sa_motion_publish_stop(ctx,0x8c6127ecu); r[8]+=60u;goto L8c6127ee; /* add #0x3c,r8 */
L8c6127ee: sa_motion_publish_stop(ctx,0x8c6127eeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6127f0; /* fmov.s fr5,@-r8 */
L8c6127f0: sa_motion_publish_stop(ctx,0x8c6127f0u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c6127f2; /* mov.w @(0x2,r4),r0 */
L8c6127f2: sa_motion_publish_stop(ctx,0x8c6127f2u); r[3]=r[0];goto L8c6127f4; /* mov r0,r3 */
L8c6127f4: sa_motion_publish_stop(ctx,0x8c6127f4u); *fpul=r[3];goto L8c6127f6; /* lds r3,FPUL */
L8c6127f6: sa_motion_publish_stop(ctx,0x8c6127f6u); r[0]=24u;goto L8c6127f8; /* mov #0x18,r0 */
L8c6127f8: sa_motion_publish_stop(ctx,0x8c6127f8u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6127fa; /* float FPUL,fr3 */
L8c6127fa: sa_motion_publish_stop(ctx,0x8c6127fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6127fc; /* fmul fr4,fr3 */
L8c6127fc: sa_motion_publish_stop(ctx,0x8c6127fcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6127fe; /* fmov.s fr3,@-r8 */
L8c6127fe: sa_motion_publish_stop(ctx,0x8c6127feu); r[3]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c612800; /* mov.w @r4,r3 */
L8c612800: sa_motion_publish_stop(ctx,0x8c612800u); r[4]=r[5];goto L8c612802; /* mov r5,r4 */
L8c612802: sa_motion_publish_stop(ctx,0x8c612802u); r[4]+=64u;goto L8c612804; /* add #0x40,r4 */
L8c612804: sa_motion_publish_stop(ctx,0x8c612804u); *fpul=r[3];goto L8c612806; /* lds r3,FPUL */
L8c612806: sa_motion_publish_stop(ctx,0x8c612806u); r[3]=rd(m,bus,&io_status,0x8c612854u,4);if(io_status)return io_status;goto L8c612808; /* mov.l 0x8c612854,r3 */
L8c612808: sa_motion_publish_stop(ctx,0x8c612808u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61280a; /* float FPUL,fr3 */
L8c61280a: sa_motion_publish_stop(ctx,0x8c61280au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61280c; /* fmul fr4,fr3 */
L8c61280c: sa_motion_publish_stop(ctx,0x8c61280cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61280e; /* fmov.s fr3,@-r8 */
L8c61280e: sa_motion_publish_stop(ctx,0x8c61280eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612810; /* fmov.s @(r0,r4),fr3 */
L8c612810: sa_motion_publish_stop(ctx,0x8c612810u); r[0]=20u;goto L8c612812; /* mov #0x14,r0 */
L8c612812: sa_motion_publish_stop(ctx,0x8c612812u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612814; /* fmov.s fr3,@-r8 */
L8c612814: sa_motion_publish_stop(ctx,0x8c612814u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612816; /* fmov.s @(r0,r4),fr3 */
L8c612816: sa_motion_publish_stop(ctx,0x8c612816u); r[0]=16u;goto L8c612818; /* mov #0x10,r0 */
L8c612818: sa_motion_publish_stop(ctx,0x8c612818u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61281a; /* fmov.s fr3,@-r8 */
L8c61281a: sa_motion_publish_stop(ctx,0x8c61281au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61281c; /* fmov.s @(r0,r4),fr3 */
L8c61281c: sa_motion_publish_stop(ctx,0x8c61281cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61281e; /* fmov.s fr3,@-r8 */
L8c61281e: sa_motion_publish_stop(ctx,0x8c61281eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c612820; /* mov.l r3,@-r8 */
L8c612820: sa_motion_publish_stop(ctx,0x8c612820u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612822; /* pref @r8 */
L8c612822: sa_motion_publish_stop(ctx,0x8c612822u); r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c612824; /* mov.l @r7,r2 */
L8c612824: sa_motion_publish_stop(ctx,0x8c612824u); r[8]+=32u;goto L8c612826; /* add #0x20,r8 */
L8c612826: sa_motion_publish_stop(ctx,0x8c612828u); r[2]+=3u;goto L8c612946; /* bra 0x8c612946 */
L8c612858: sa_motion_publish_stop(ctx,0x8c612858u); r[3]=r[6];goto L8c61285a; /* mov r6,r3 */
L8c61285a: sa_motion_publish_stop(ctx,0x8c61285au); r[8]+=28u;goto L8c61285c; /* add #0x1c,r8 */
L8c61285c: sa_motion_publish_stop(ctx,0x8c61285cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61285e; /* fmov.s fr5,@-r8 */
L8c61285e: sa_motion_publish_stop(ctx,0x8c61285eu); r[14]=r[5];goto L8c612860; /* mov r5,r14 */
L8c612860: sa_motion_publish_stop(ctx,0x8c612860u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c612862; /* mov.l r3,@r15 */
L8c612862: sa_motion_publish_stop(ctx,0x8c612862u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c612864; /* mov.w @(0x2,r3),r0 */
L8c612864: sa_motion_publish_stop(ctx,0x8c612864u); r[3]=r[0];goto L8c612866; /* mov r0,r3 */
L8c612866: sa_motion_publish_stop(ctx,0x8c612866u); *fpul=r[3];goto L8c612868; /* lds r3,FPUL */
L8c612868: sa_motion_publish_stop(ctx,0x8c612868u); r[0]=24u;goto L8c61286a; /* mov #0x18,r0 */
L8c61286a: sa_motion_publish_stop(ctx,0x8c61286au); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c61286c; /* float FPUL,fr3 */
L8c61286c: sa_motion_publish_stop(ctx,0x8c61286cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61286e; /* fmul fr4,fr3 */
L8c61286e: sa_motion_publish_stop(ctx,0x8c61286eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612870; /* fmov.s fr3,@-r8 */
L8c612870: sa_motion_publish_stop(ctx,0x8c612870u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c612872; /* mov.l @r15,r3 */
L8c612872: sa_motion_publish_stop(ctx,0x8c612872u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c612874; /* mov.w @r3,r3 */
L8c612874: sa_motion_publish_stop(ctx,0x8c612874u); *fpul=r[3];goto L8c612876; /* lds r3,FPUL */
L8c612876: sa_motion_publish_stop(ctx,0x8c612876u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612878; /* float FPUL,fr3 */
L8c612878: sa_motion_publish_stop(ctx,0x8c612878u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61287a; /* fmul fr4,fr3 */
L8c61287a: sa_motion_publish_stop(ctx,0x8c61287au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61287c; /* fmov.s fr3,@-r8 */
L8c61287c: sa_motion_publish_stop(ctx,0x8c61287cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61287e; /* fmov.s @(r0,r14),fr3 */
L8c61287e: sa_motion_publish_stop(ctx,0x8c61287eu); r[0]=20u;goto L8c612880; /* mov #0x14,r0 */
L8c612880: sa_motion_publish_stop(ctx,0x8c612880u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612882; /* fmov.s fr3,@-r8 */
L8c612882: sa_motion_publish_stop(ctx,0x8c612882u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c612884; /* fmov.s @(r0,r14),fr3 */
L8c612884: sa_motion_publish_stop(ctx,0x8c612884u); r[0]=16u;goto L8c612886; /* mov #0x10,r0 */
L8c612886: sa_motion_publish_stop(ctx,0x8c612886u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612888; /* fmov.s fr3,@-r8 */
L8c612888: sa_motion_publish_stop(ctx,0x8c612888u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61288a; /* fmov.s @(r0,r14),fr3 */
L8c61288a: sa_motion_publish_stop(ctx,0x8c61288au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61288c; /* fmov.s fr3,@-r8 */
L8c61288c: sa_motion_publish_stop(ctx,0x8c61288cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c61288e; /* mov.l r4,@-r8 */
L8c61288e: sa_motion_publish_stop(ctx,0x8c61288eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612890; /* pref @r8 */
L8c612890: sa_motion_publish_stop(ctx,0x8c612890u); r[3]=r[6];goto L8c612892; /* mov r6,r3 */
L8c612892: sa_motion_publish_stop(ctx,0x8c612892u); r[3]+=4u;goto L8c612894; /* add #0x4,r3 */
L8c612894: sa_motion_publish_stop(ctx,0x8c612894u); r[8]+=60u;goto L8c612896; /* add #0x3c,r8 */
L8c612896: sa_motion_publish_stop(ctx,0x8c612896u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c612898; /* fmov.s fr5,@-r8 */
L8c612898: sa_motion_publish_stop(ctx,0x8c612898u); r[14]+=32u;goto L8c61289a; /* add #0x20,r14 */
L8c61289a: sa_motion_publish_stop(ctx,0x8c61289au); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c61289c; /* mov.l r3,@r15 */
L8c61289c: sa_motion_publish_stop(ctx,0x8c61289cu); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61289e; /* mov.w @(0x2,r3),r0 */
L8c61289e: sa_motion_publish_stop(ctx,0x8c61289eu); r[3]=r[0];goto L8c6128a0; /* mov r0,r3 */
L8c6128a0: sa_motion_publish_stop(ctx,0x8c6128a0u); *fpul=r[3];goto L8c6128a2; /* lds r3,FPUL */
L8c6128a2: sa_motion_publish_stop(ctx,0x8c6128a2u); r[0]=24u;goto L8c6128a4; /* mov #0x18,r0 */
L8c6128a4: sa_motion_publish_stop(ctx,0x8c6128a4u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6128a6; /* float FPUL,fr3 */
L8c6128a6: sa_motion_publish_stop(ctx,0x8c6128a6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6128a8; /* fmul fr4,fr3 */
L8c6128a8: sa_motion_publish_stop(ctx,0x8c6128a8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128aa; /* fmov.s fr3,@-r8 */
L8c6128aa: sa_motion_publish_stop(ctx,0x8c6128aau); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6128ac; /* mov.l @r15,r3 */
L8c6128ac: sa_motion_publish_stop(ctx,0x8c6128acu); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6128ae; /* mov.w @r3,r3 */
L8c6128ae: sa_motion_publish_stop(ctx,0x8c6128aeu); *fpul=r[3];goto L8c6128b0; /* lds r3,FPUL */
L8c6128b0: sa_motion_publish_stop(ctx,0x8c6128b0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6128b2; /* float FPUL,fr3 */
L8c6128b2: sa_motion_publish_stop(ctx,0x8c6128b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6128b4; /* fmul fr4,fr3 */
L8c6128b4: sa_motion_publish_stop(ctx,0x8c6128b4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128b6; /* fmov.s fr3,@-r8 */
L8c6128b6: sa_motion_publish_stop(ctx,0x8c6128b6u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6128b8; /* fmov.s @(r0,r14),fr3 */
L8c6128b8: sa_motion_publish_stop(ctx,0x8c6128b8u); r[0]=20u;goto L8c6128ba; /* mov #0x14,r0 */
L8c6128ba: sa_motion_publish_stop(ctx,0x8c6128bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128bc; /* fmov.s fr3,@-r8 */
L8c6128bc: sa_motion_publish_stop(ctx,0x8c6128bcu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6128be; /* fmov.s @(r0,r14),fr3 */
L8c6128be: sa_motion_publish_stop(ctx,0x8c6128beu); r[0]=16u;goto L8c6128c0; /* mov #0x10,r0 */
L8c6128c0: sa_motion_publish_stop(ctx,0x8c6128c0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128c2; /* fmov.s fr3,@-r8 */
L8c6128c2: sa_motion_publish_stop(ctx,0x8c6128c2u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6128c4; /* fmov.s @(r0,r14),fr3 */
L8c6128c4: sa_motion_publish_stop(ctx,0x8c6128c4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128c6; /* fmov.s fr3,@-r8 */
L8c6128c6: sa_motion_publish_stop(ctx,0x8c6128c6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c6128c8; /* mov.l r4,@-r8 */
L8c6128c8: sa_motion_publish_stop(ctx,0x8c6128c8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6128ca; /* pref @r8 */
L8c6128ca: sa_motion_publish_stop(ctx,0x8c6128cau); r[3]=r[6];goto L8c6128cc; /* mov r6,r3 */
L8c6128cc: sa_motion_publish_stop(ctx,0x8c6128ccu); r[3]+=12u;goto L8c6128ce; /* add #0xc,r3 */
L8c6128ce: sa_motion_publish_stop(ctx,0x8c6128ceu); r[8]+=60u;goto L8c6128d0; /* add #0x3c,r8 */
L8c6128d0: sa_motion_publish_stop(ctx,0x8c6128d0u); r[14]=r[5];goto L8c6128d2; /* mov r5,r14 */
L8c6128d2: sa_motion_publish_stop(ctx,0x8c6128d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c6128d4; /* fmov.s fr5,@-r8 */
L8c6128d4: sa_motion_publish_stop(ctx,0x8c6128d4u); r[14]+=96u;goto L8c6128d6; /* add #0x60,r14 */
L8c6128d6: sa_motion_publish_stop(ctx,0x8c6128d6u); wr(m,bus,&io_status,r[15],r[3],4);if(io_status)return io_status;goto L8c6128d8; /* mov.l r3,@r15 */
L8c6128d8: sa_motion_publish_stop(ctx,0x8c6128d8u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c6128da; /* mov.w @(0x2,r3),r0 */
L8c6128da: sa_motion_publish_stop(ctx,0x8c6128dau); r[3]=r[0];goto L8c6128dc; /* mov r0,r3 */
L8c6128dc: sa_motion_publish_stop(ctx,0x8c6128dcu); *fpul=r[3];goto L8c6128de; /* lds r3,FPUL */
L8c6128de: sa_motion_publish_stop(ctx,0x8c6128deu); r[0]=24u;goto L8c6128e0; /* mov #0x18,r0 */
L8c6128e0: sa_motion_publish_stop(ctx,0x8c6128e0u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6128e2; /* float FPUL,fr3 */
L8c6128e2: sa_motion_publish_stop(ctx,0x8c6128e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6128e4; /* fmul fr4,fr3 */
L8c6128e4: sa_motion_publish_stop(ctx,0x8c6128e4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128e6; /* fmov.s fr3,@-r8 */
L8c6128e6: sa_motion_publish_stop(ctx,0x8c6128e6u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6128e8; /* mov.l @r15,r3 */
L8c6128e8: sa_motion_publish_stop(ctx,0x8c6128e8u); r[3]=sx(m,bus,&io_status,r[3],2);if(io_status)return io_status;goto L8c6128ea; /* mov.w @r3,r3 */
L8c6128ea: sa_motion_publish_stop(ctx,0x8c6128eau); *fpul=r[3];goto L8c6128ec; /* lds r3,FPUL */
L8c6128ec: sa_motion_publish_stop(ctx,0x8c6128ecu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c6128ee; /* float FPUL,fr3 */
L8c6128ee: sa_motion_publish_stop(ctx,0x8c6128eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c6128f0; /* fmul fr4,fr3 */
L8c6128f0: sa_motion_publish_stop(ctx,0x8c6128f0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128f2; /* fmov.s fr3,@-r8 */
L8c6128f2: sa_motion_publish_stop(ctx,0x8c6128f2u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6128f4; /* fmov.s @(r0,r14),fr3 */
L8c6128f4: sa_motion_publish_stop(ctx,0x8c6128f4u); r[0]=20u;goto L8c6128f6; /* mov #0x14,r0 */
L8c6128f6: sa_motion_publish_stop(ctx,0x8c6128f6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128f8; /* fmov.s fr3,@-r8 */
L8c6128f8: sa_motion_publish_stop(ctx,0x8c6128f8u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6128fa; /* fmov.s @(r0,r14),fr3 */
L8c6128fa: sa_motion_publish_stop(ctx,0x8c6128fau); r[0]=16u;goto L8c6128fc; /* mov #0x10,r0 */
L8c6128fc: sa_motion_publish_stop(ctx,0x8c6128fcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6128fe; /* fmov.s fr3,@-r8 */
L8c6128fe: sa_motion_publish_stop(ctx,0x8c6128feu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c612900; /* fmov.s @(r0,r14),fr3 */
L8c612900: sa_motion_publish_stop(ctx,0x8c612900u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612902; /* fmov.s fr3,@-r8 */
L8c612902: sa_motion_publish_stop(ctx,0x8c612902u); r[8]-=4;wr(m,bus,&io_status,r[8],r[4],4);if(io_status)return io_status;goto L8c612904; /* mov.l r4,@-r8 */
L8c612904: sa_motion_publish_stop(ctx,0x8c612904u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612906; /* pref @r8 */
L8c612906: sa_motion_publish_stop(ctx,0x8c612906u); r[4]=r[6];goto L8c612908; /* mov r6,r4 */
L8c612908: sa_motion_publish_stop(ctx,0x8c612908u); r[4]+=8u;goto L8c61290a; /* add #0x8,r4 */
L8c61290a: sa_motion_publish_stop(ctx,0x8c61290au); r[8]+=60u;goto L8c61290c; /* add #0x3c,r8 */
L8c61290c: sa_motion_publish_stop(ctx,0x8c61290cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[8]);if(io_status)return io_status;goto L8c61290e; /* fmov.s fr5,@-r8 */
L8c61290e: sa_motion_publish_stop(ctx,0x8c61290eu); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c612910; /* mov.w @(0x2,r4),r0 */
L8c612910: sa_motion_publish_stop(ctx,0x8c612910u); r[3]=r[0];goto L8c612912; /* mov r0,r3 */
L8c612912: sa_motion_publish_stop(ctx,0x8c612912u); *fpul=r[3];goto L8c612914; /* lds r3,FPUL */
L8c612914: sa_motion_publish_stop(ctx,0x8c612914u); r[0]=24u;goto L8c612916; /* mov #0x18,r0 */
L8c612916: sa_motion_publish_stop(ctx,0x8c612916u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612918; /* float FPUL,fr3 */
L8c612918: sa_motion_publish_stop(ctx,0x8c612918u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61291a; /* fmul fr4,fr3 */
L8c61291a: sa_motion_publish_stop(ctx,0x8c61291au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61291c; /* fmov.s fr3,@-r8 */
L8c61291c: sa_motion_publish_stop(ctx,0x8c61291cu); r[3]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61291e; /* mov.w @r4,r3 */
L8c61291e: sa_motion_publish_stop(ctx,0x8c61291eu); r[4]=r[5];goto L8c612920; /* mov r5,r4 */
L8c612920: sa_motion_publish_stop(ctx,0x8c612920u); r[4]+=64u;goto L8c612922; /* add #0x40,r4 */
L8c612922: sa_motion_publish_stop(ctx,0x8c612922u); *fpul=r[3];goto L8c612924; /* lds r3,FPUL */
L8c612924: sa_motion_publish_stop(ctx,0x8c612924u); r[3]=rd(m,bus,&io_status,0x8c612a60u,4);if(io_status)return io_status;goto L8c612926; /* mov.l 0x8c612a60,r3 */
L8c612926: sa_motion_publish_stop(ctx,0x8c612926u); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c612928; /* float FPUL,fr3 */
L8c612928: sa_motion_publish_stop(ctx,0x8c612928u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[4],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61292a; /* fmul fr4,fr3 */
L8c61292a: sa_motion_publish_stop(ctx,0x8c61292au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61292c; /* fmov.s fr3,@-r8 */
L8c61292c: sa_motion_publish_stop(ctx,0x8c61292cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61292e; /* fmov.s @(r0,r4),fr3 */
L8c61292e: sa_motion_publish_stop(ctx,0x8c61292eu); r[0]=20u;goto L8c612930; /* mov #0x14,r0 */
L8c612930: sa_motion_publish_stop(ctx,0x8c612930u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612932; /* fmov.s fr3,@-r8 */
L8c612932: sa_motion_publish_stop(ctx,0x8c612932u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c612934; /* fmov.s @(r0,r4),fr3 */
L8c612934: sa_motion_publish_stop(ctx,0x8c612934u); r[0]=16u;goto L8c612936; /* mov #0x10,r0 */
L8c612936: sa_motion_publish_stop(ctx,0x8c612936u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c612938; /* fmov.s fr3,@-r8 */
L8c612938: sa_motion_publish_stop(ctx,0x8c612938u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61293a; /* fmov.s @(r0,r4),fr3 */
L8c61293a: sa_motion_publish_stop(ctx,0x8c61293au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61293c; /* fmov.s fr3,@-r8 */
L8c61293c: sa_motion_publish_stop(ctx,0x8c61293cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61293e; /* mov.l r3,@-r8 */
L8c61293e: sa_motion_publish_stop(ctx,0x8c61293eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c612940; /* pref @r8 */
L8c612940: sa_motion_publish_stop(ctx,0x8c612940u); r[2]=rd(m,bus,&io_status,r[7],4);if(io_status)return io_status;goto L8c612942; /* mov.l @r7,r2 */
L8c612942: sa_motion_publish_stop(ctx,0x8c612942u); r[8]+=32u;goto L8c612944; /* add #0x20,r8 */
L8c612944: sa_motion_publish_stop(ctx,0x8c612944u); r[2]+=4u;goto L8c612946; /* add #0x4,r2 */
L8c612946: sa_motion_publish_stop(ctx,0x8c612946u); wr(m,bus,&io_status,r[7],r[2],4);if(io_status)return io_status;goto L8c612948; /* mov.l r2,@r7 */
L8c612948: sa_motion_publish_stop(ctx,0x8c612948u); r[15]+=68u;goto L8c61294a; /* add #0x44,r15 */
L8c61294a: sa_motion_publish_stop(ctx,0x8c61294au); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61294c; /* fmov.s @r15+,fr12 */
L8c61294c: sa_motion_publish_stop(ctx,0x8c61294cu); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61294e; /* mov.l @r15+,r9 */
L8c61294e: sa_motion_publish_stop(ctx,0x8c61294eu); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612950; /* mov.l @r15+,r10 */
L8c612950: sa_motion_publish_stop(ctx,0x8c612950u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612952; /* mov.l @r15+,r11 */
L8c612952: sa_motion_publish_stop(ctx,0x8c612952u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612954; /* mov.l @r15+,r12 */
L8c612954: sa_motion_publish_stop(ctx,0x8c612954u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c612956; /* mov.l @r15+,r13 */
L8c612956: sa_motion_publish_stop(ctx,0x8c612958u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
