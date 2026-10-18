#include "sa_draw_motion_stop.h"
/* Particle primitive emitters with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_primitive_emit_8c61ffe4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61ffe4;
L8c61ffe4: sa_motion_publish_stop(ctx,0x8c61ffe4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61ffe6; /* mov.l r14,@-r15 */
L8c61ffe6: sa_motion_publish_stop(ctx,0x8c61ffe6u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61ffe8; /* mov.l r13,@-r15 */
L8c61ffe8: sa_motion_publish_stop(ctx,0x8c61ffe8u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61ffea; /* mov.l r12,@-r15 */
L8c61ffea: sa_motion_publish_stop(ctx,0x8c61ffeau); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61ffec; /* mov.l r11,@-r15 */
L8c61ffec: sa_motion_publish_stop(ctx,0x8c61ffecu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61ffee; /* mov.l r10,@-r15 */
L8c61ffee: sa_motion_publish_stop(ctx,0x8c61ffeeu); r[3]=rd(m,bus,&io_status,0x8c620148u,4);if(io_status)return io_status;goto L8c61fff0; /* mov.l 0x8c620148,r3 */
L8c61fff0: sa_motion_publish_stop(ctx,0x8c61fff0u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61fff2; /* mov.l r9,@-r15 */
L8c61fff2: sa_motion_publish_stop(ctx,0x8c61fff2u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61fff4; /* sts.l PR,@-r15 */
L8c61fff4: sa_motion_publish_stop(ctx,0x8c61fff4u); r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61fff6; /* mov.w @r4,r0 */
L8c61fff6: sa_motion_publish_stop(ctx,0x8c61fff6u); r[13]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c61fff8; /* mov.l @(0x4,r4),r13 */
L8c61fff8: sa_motion_publish_stop(ctx,0x8c61fff8u); r[0]=r[0]&65535u;goto L8c61fffa; /* extu.w r0,r0 */
L8c61fffa: sa_motion_publish_stop(ctx,0x8c61fffau); r[9]=rd(m,bus,&io_status,0x8c620140u,4);if(io_status)return io_status;goto L8c61fffc; /* mov.l 0x8c620140,r9 */
L8c61fffc: sa_motion_publish_stop(ctx,0x8c61fffcu); r[0]&=r[3];goto L8c61fffe; /* and r3,r0 */
L8c61fffe: sa_motion_publish_stop(ctx,0x8c61fffeu); r[11]=rd(m,bus,&io_status,0x8c620144u,4);if(io_status)return io_status;goto L8c620000; /* mov.l 0x8c620144,r11 */
L8c620000: sa_motion_publish_stop(ctx,0x8c620000u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c620002; /* cmp/eq #0x0,r0 */
L8c620002: sa_motion_publish_stop(ctx,0x8c620004u); take=(*sr&1u);r[12]=0u;if(take)goto L8c620028;goto L8c620006; /* bt/s 0x8c620028 */
L8c620006: sa_motion_publish_stop(ctx,0x8c620006u); r[1]=sx(m,bus,&io_status,0x8c62013cu,2);if(io_status)return io_status;goto L8c620008; /* mov.w 0x8c62013c,r1 */
L8c620008: sa_motion_publish_stop(ctx,0x8c620008u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c62000a; /* cmp/eq r1,r0 */
L8c62000a: sa_motion_publish_stop(ctx,0x8c62000au); if(!(*sr&1u))goto L8c620010;goto L8c62000c; /* bf 0x8c620010 */
L8c62000c: sa_motion_publish_stop(ctx,0x8c62000eu); goto L8c620160; /* bra 0x8c620160 */
L8c620010: sa_motion_publish_stop(ctx,0x8c620010u); r[1]=rd(m,bus,&io_status,0x8c62014cu,4);if(io_status)return io_status;goto L8c620012; /* mov.l 0x8c62014c,r1 */
L8c620012: sa_motion_publish_stop(ctx,0x8c620012u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620014; /* cmp/eq r1,r0 */
L8c620014: sa_motion_publish_stop(ctx,0x8c620014u); if(!(*sr&1u))goto L8c62001a;goto L8c620016; /* bf 0x8c62001a */
L8c620016: sa_motion_publish_stop(ctx,0x8c620018u); goto L8c6202b0; /* bra 0x8c6202b0 */
L8c62001a: sa_motion_publish_stop(ctx,0x8c62001au); r[1]=rd(m,bus,&io_status,0x8c620148u,4);if(io_status)return io_status;goto L8c62001c; /* mov.l 0x8c620148,r1 */
L8c62001c: sa_motion_publish_stop(ctx,0x8c62001cu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c62001e; /* cmp/eq r1,r0 */
L8c62001e: sa_motion_publish_stop(ctx,0x8c62001eu); if(!(*sr&1u))goto L8c620024;goto L8c620020; /* bf 0x8c620024 */
L8c620020: sa_motion_publish_stop(ctx,0x8c620022u); goto L8c6203ca; /* bra 0x8c6203ca */
L8c620024: sa_motion_publish_stop(ctx,0x8c620026u); goto L8c6203fa; /* bra 0x8c6203fa */
L8c620028: sa_motion_publish_stop(ctx,0x8c620028u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c62002a; /* mov.w @(0x2,r4),r0 */
L8c62002a: sa_motion_publish_stop(ctx,0x8c62002au); r[1]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c62002c; /* mov.l @r11,r1 */
L8c62002c: sa_motion_publish_stop(ctx,0x8c62002cu); r[0]=r[0]&65535u;goto L8c62002e; /* extu.w r0,r0 */
L8c62002e: sa_motion_publish_stop(ctx,0x8c62002eu); r[2]=r[0];goto L8c620030; /* mov r0,r2 */
L8c620030: sa_motion_publish_stop(ctx,0x8c620030u); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c620032; /* shll r0 */
L8c620032: sa_motion_publish_stop(ctx,0x8c620032u); r[0]+=r[2];goto L8c620034; /* add r2,r0 */
L8c620034: sa_motion_publish_stop(ctx,0x8c620034u); r[1]+=r[0];goto L8c620036; /* add r0,r1 */
L8c620036: sa_motion_publish_stop(ctx,0x8c620036u); wr(m,bus,&io_status,r[11],r[1],4);if(io_status)return io_status;goto L8c620038; /* mov.l r1,@r11 */
L8c620038: sa_motion_publish_stop(ctx,0x8c62003au); r[14]=r[12];goto L8c620130; /* bra 0x8c620130 */
L8c62003c: sa_motion_publish_stop(ctx,0x8c62003cu); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c62003e; /* mov.w @r13+,r5 */
L8c62003e: sa_motion_publish_stop(ctx,0x8c62003eu); r[0]=16u;goto L8c620040; /* mov #0x10,r0 */
L8c620040: sa_motion_publish_stop(ctx,0x8c620040u); r[3]=rd(m,bus,&io_status,0x8c620150u,4);if(io_status)return io_status;goto L8c620042; /* mov.l 0x8c620150,r3 */
L8c620042: sa_motion_publish_stop(ctx,0x8c620042u); r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c620044; /* mov.w @r13+,r6 */
L8c620044: sa_motion_publish_stop(ctx,0x8c620044u); r[5]=r[5]&65535u;goto L8c620046; /* extu.w r5,r5 */
L8c620046: sa_motion_publish_stop(ctx,0x8c620046u); r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c620048; /* mov.l @r3,r12 */
L8c620048: sa_motion_publish_stop(ctx,0x8c620048u); r[5]<<=2;goto L8c62004a; /* shll2 r5 */
L8c62004a: sa_motion_publish_stop(ctx,0x8c62004au); r[6]=r[6]&65535u;goto L8c62004c; /* extu.w r6,r6 */
L8c62004c: sa_motion_publish_stop(ctx,0x8c62004cu); r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c62004e; /* mov.w @r13+,r7 */
L8c62004e: sa_motion_publish_stop(ctx,0x8c62004eu); r[6]<<=2;goto L8c620050; /* shll2 r6 */
L8c620050: sa_motion_publish_stop(ctx,0x8c620050u); r[6]<<=2;goto L8c620052; /* shll2 r6 */
L8c620052: sa_motion_publish_stop(ctx,0x8c620052u); r[5]<<=2;goto L8c620054; /* shll2 r5 */
L8c620054: sa_motion_publish_stop(ctx,0x8c620054u); r[7]=r[7]&65535u;goto L8c620056; /* extu.w r7,r7 */
L8c620056: sa_motion_publish_stop(ctx,0x8c620056u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c620058; /* shll r5 */
L8c620058: sa_motion_publish_stop(ctx,0x8c620058u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c62005a; /* shll r6 */
L8c62005a: sa_motion_publish_stop(ctx,0x8c62005au); r[6]+=r[12];goto L8c62005c; /* add r12,r6 */
L8c62005c: sa_motion_publish_stop(ctx,0x8c62005cu); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c62005e; /* fmov.s @(r0,r6),fr4 */
L8c62005e: sa_motion_publish_stop(ctx,0x8c62005eu); r[5]+=r[12];goto L8c620060; /* add r12,r5 */
L8c620060: sa_motion_publish_stop(ctx,0x8c620060u); r[7]<<=2;goto L8c620062; /* shll2 r7 */
L8c620062: sa_motion_publish_stop(ctx,0x8c620062u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620064; /* fmov.s @(r0,r5),fr3 */
L8c620064: sa_motion_publish_stop(ctx,0x8c620064u); r[7]<<=2;goto L8c620066; /* shll2 r7 */
L8c620066: sa_motion_publish_stop(ctx,0x8c620066u); fcopy(f,2u,4u);goto L8c620068; /* fmov fr4,fr2 */
L8c620068: sa_motion_publish_stop(ctx,0x8c620068u); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c62006a; /* shll r7 */
L8c62006a: sa_motion_publish_stop(ctx,0x8c62006au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c62006c; /* fsub fr3,fr2 */
L8c62006c: sa_motion_publish_stop(ctx,0x8c62006cu); r[0]=20u;goto L8c62006e; /* mov #0x14,r0 */
L8c62006e: sa_motion_publish_stop(ctx,0x8c62006eu); r[7]+=r[12];goto L8c620070; /* add r12,r7 */
L8c620070: sa_motion_publish_stop(ctx,0x8c620070u); fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c620072; /* fmov.s @(r0,r6),fr5 */
L8c620072: sa_motion_publish_stop(ctx,0x8c620072u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c620074; /* fmov.s @(r0,r7),fr3 */
L8c620074: sa_motion_publish_stop(ctx,0x8c620074u); fcopy(f,1u,5u);goto L8c620076; /* fmov fr5,fr1 */
L8c620076: sa_motion_publish_stop(ctx,0x8c620076u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c620078; /* fsub fr5,fr3 */
L8c620078: sa_motion_publish_stop(ctx,0x8c620078u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c62007a; /* fmul fr3,fr2 */
L8c62007a: sa_motion_publish_stop(ctx,0x8c62007au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c62007c; /* fmov.s @(r0,r5),fr3 */
L8c62007c: sa_motion_publish_stop(ctx,0x8c62007cu); r[0]=16u;goto L8c62007e; /* mov #0x10,r0 */
L8c62007e: sa_motion_publish_stop(ctx,0x8c62007eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c620080; /* fsub fr3,fr1 */
L8c620080: sa_motion_publish_stop(ctx,0x8c620080u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c620082; /* fmov.s @(r0,r7),fr3 */
L8c620082: sa_motion_publish_stop(ctx,0x8c620082u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c620084; /* fsub fr4,fr3 */
L8c620084: sa_motion_publish_stop(ctx,0x8c620084u); fcopy(f,4u,2u);goto L8c620086; /* fmov fr2,fr4 */
L8c620086: sa_motion_publish_stop(ctx,0x8c620086u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c620088; /* fmul fr3,fr1 */
L8c620088: sa_motion_publish_stop(ctx,0x8c620088u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c62008a; /* fldi0 fr3 */
L8c62008a: sa_motion_publish_stop(ctx,0x8c62008au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c62008c; /* fsub fr1,fr4 */
L8c62008c: sa_motion_publish_stop(ctx,0x8c62008cu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c62008e; /* fcmp/gt fr3,fr4 */
L8c62008e: sa_motion_publish_stop(ctx,0x8c620090u); take=(*sr&1u);r[14]+=1u;if(take)goto L8c620130;goto L8c620092; /* bt/s 0x8c620130 */
L8c620092: sa_motion_publish_stop(ctx,0x8c620092u); r[0]=28u;goto L8c620094; /* mov #0x1c,r0 */
L8c620094: sa_motion_publish_stop(ctx,0x8c620094u); r[3]=rd(m,bus,&io_status,0x8c620154u,4);if(io_status)return io_status;goto L8c620096; /* mov.l 0x8c620154,r3 */
L8c620096: sa_motion_publish_stop(ctx,0x8c620096u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620098; /* fmov.s @(r0,r9),fr3 */
L8c620098: sa_motion_publish_stop(ctx,0x8c620098u); r[8]+=32u;goto L8c62009a; /* add #0x20,r8 */
L8c62009a: sa_motion_publish_stop(ctx,0x8c62009au); r[0]=24u;goto L8c62009c; /* mov #0x18,r0 */
L8c62009c: sa_motion_publish_stop(ctx,0x8c62009cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62009e; /* fmov.s fr3,@-r8 */
L8c62009e: sa_motion_publish_stop(ctx,0x8c62009eu); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200a0; /* fmov.s @(r0,r9),fr3 */
L8c6200a0: sa_motion_publish_stop(ctx,0x8c6200a0u); r[0]=20u;goto L8c6200a2; /* mov #0x14,r0 */
L8c6200a2: sa_motion_publish_stop(ctx,0x8c6200a2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200a4; /* fmov.s fr3,@-r8 */
L8c6200a4: sa_motion_publish_stop(ctx,0x8c6200a4u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200a6; /* fmov.s @(r0,r9),fr3 */
L8c6200a6: sa_motion_publish_stop(ctx,0x8c6200a6u); r[0]=16u;goto L8c6200a8; /* mov #0x10,r0 */
L8c6200a8: sa_motion_publish_stop(ctx,0x8c6200a8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200aa; /* fmov.s fr3,@-r8 */
L8c6200aa: sa_motion_publish_stop(ctx,0x8c6200aau); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200ac; /* fmov.s @(r0,r9),fr3 */
L8c6200ac: sa_motion_publish_stop(ctx,0x8c6200acu); r[0]=24u;goto L8c6200ae; /* mov #0x18,r0 */
L8c6200ae: sa_motion_publish_stop(ctx,0x8c6200aeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200b0; /* fmov.s fr3,@-r8 */
L8c6200b0: sa_motion_publish_stop(ctx,0x8c6200b0u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6200b2; /* fmov.s @(r0,r5),fr3 */
L8c6200b2: sa_motion_publish_stop(ctx,0x8c6200b2u); r[0]=20u;goto L8c6200b4; /* mov #0x14,r0 */
L8c6200b4: sa_motion_publish_stop(ctx,0x8c6200b4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200b6; /* fmov.s fr3,@-r8 */
L8c6200b6: sa_motion_publish_stop(ctx,0x8c6200b6u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6200b8; /* fmov.s @(r0,r5),fr3 */
L8c6200b8: sa_motion_publish_stop(ctx,0x8c6200b8u); r[0]=16u;goto L8c6200ba; /* mov #0x10,r0 */
L8c6200ba: sa_motion_publish_stop(ctx,0x8c6200bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200bc; /* fmov.s fr3,@-r8 */
L8c6200bc: sa_motion_publish_stop(ctx,0x8c6200bcu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6200be; /* fmov.s @(r0,r5),fr3 */
L8c6200be: sa_motion_publish_stop(ctx,0x8c6200beu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200c0; /* fmov.s fr3,@-r8 */
L8c6200c0: sa_motion_publish_stop(ctx,0x8c6200c0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6200c2; /* mov.l r3,@-r8 */
L8c6200c2: sa_motion_publish_stop(ctx,0x8c6200c2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6200c4; /* pref @r8 */
L8c6200c4: sa_motion_publish_stop(ctx,0x8c6200c4u); r[0]=28u;goto L8c6200c6; /* mov #0x1c,r0 */
L8c6200c6: sa_motion_publish_stop(ctx,0x8c6200c6u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200c8; /* fmov.s @(r0,r9),fr3 */
L8c6200c8: sa_motion_publish_stop(ctx,0x8c6200c8u); r[8]+=64u;goto L8c6200ca; /* add #0x40,r8 */
L8c6200ca: sa_motion_publish_stop(ctx,0x8c6200cau); r[0]=24u;goto L8c6200cc; /* mov #0x18,r0 */
L8c6200cc: sa_motion_publish_stop(ctx,0x8c6200ccu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200ce; /* fmov.s fr3,@-r8 */
L8c6200ce: sa_motion_publish_stop(ctx,0x8c6200ceu); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200d0; /* fmov.s @(r0,r9),fr3 */
L8c6200d0: sa_motion_publish_stop(ctx,0x8c6200d0u); r[0]=20u;goto L8c6200d2; /* mov #0x14,r0 */
L8c6200d2: sa_motion_publish_stop(ctx,0x8c6200d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200d4; /* fmov.s fr3,@-r8 */
L8c6200d4: sa_motion_publish_stop(ctx,0x8c6200d4u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200d6; /* fmov.s @(r0,r9),fr3 */
L8c6200d6: sa_motion_publish_stop(ctx,0x8c6200d6u); r[0]=16u;goto L8c6200d8; /* mov #0x10,r0 */
L8c6200d8: sa_motion_publish_stop(ctx,0x8c6200d8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200da; /* fmov.s fr3,@-r8 */
L8c6200da: sa_motion_publish_stop(ctx,0x8c6200dau); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200dc; /* fmov.s @(r0,r9),fr3 */
L8c6200dc: sa_motion_publish_stop(ctx,0x8c6200dcu); r[0]=24u;goto L8c6200de; /* mov #0x18,r0 */
L8c6200de: sa_motion_publish_stop(ctx,0x8c6200deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200e0; /* fmov.s fr3,@-r8 */
L8c6200e0: sa_motion_publish_stop(ctx,0x8c6200e0u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6200e2; /* fmov.s @(r0,r6),fr3 */
L8c6200e2: sa_motion_publish_stop(ctx,0x8c6200e2u); r[0]=20u;goto L8c6200e4; /* mov #0x14,r0 */
L8c6200e4: sa_motion_publish_stop(ctx,0x8c6200e4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200e6; /* fmov.s fr3,@-r8 */
L8c6200e6: sa_motion_publish_stop(ctx,0x8c6200e6u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6200e8; /* fmov.s @(r0,r6),fr3 */
L8c6200e8: sa_motion_publish_stop(ctx,0x8c6200e8u); r[0]=16u;goto L8c6200ea; /* mov #0x10,r0 */
L8c6200ea: sa_motion_publish_stop(ctx,0x8c6200eau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200ec; /* fmov.s fr3,@-r8 */
L8c6200ec: sa_motion_publish_stop(ctx,0x8c6200ecu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6200ee; /* fmov.s @(r0,r6),fr3 */
L8c6200ee: sa_motion_publish_stop(ctx,0x8c6200eeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200f0; /* fmov.s fr3,@-r8 */
L8c6200f0: sa_motion_publish_stop(ctx,0x8c6200f0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6200f2; /* mov.l r3,@-r8 */
L8c6200f2: sa_motion_publish_stop(ctx,0x8c6200f2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6200f4; /* pref @r8 */
L8c6200f4: sa_motion_publish_stop(ctx,0x8c6200f4u); r[0]=28u;goto L8c6200f6; /* mov #0x1c,r0 */
L8c6200f6: sa_motion_publish_stop(ctx,0x8c6200f6u); r[3]=rd(m,bus,&io_status,0x8c620158u,4);if(io_status)return io_status;goto L8c6200f8; /* mov.l 0x8c620158,r3 */
L8c6200f8: sa_motion_publish_stop(ctx,0x8c6200f8u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200fa; /* fmov.s @(r0,r9),fr3 */
L8c6200fa: sa_motion_publish_stop(ctx,0x8c6200fau); r[8]+=64u;goto L8c6200fc; /* add #0x40,r8 */
L8c6200fc: sa_motion_publish_stop(ctx,0x8c6200fcu); r[0]=24u;goto L8c6200fe; /* mov #0x18,r0 */
L8c6200fe: sa_motion_publish_stop(ctx,0x8c6200feu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620100; /* fmov.s fr3,@-r8 */
L8c620100: sa_motion_publish_stop(ctx,0x8c620100u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620102; /* fmov.s @(r0,r9),fr3 */
L8c620102: sa_motion_publish_stop(ctx,0x8c620102u); r[0]=20u;goto L8c620104; /* mov #0x14,r0 */
L8c620104: sa_motion_publish_stop(ctx,0x8c620104u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620106; /* fmov.s fr3,@-r8 */
L8c620106: sa_motion_publish_stop(ctx,0x8c620106u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620108; /* fmov.s @(r0,r9),fr3 */
L8c620108: sa_motion_publish_stop(ctx,0x8c620108u); r[0]=16u;goto L8c62010a; /* mov #0x10,r0 */
L8c62010a: sa_motion_publish_stop(ctx,0x8c62010au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62010c; /* fmov.s fr3,@-r8 */
L8c62010c: sa_motion_publish_stop(ctx,0x8c62010cu); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62010e; /* fmov.s @(r0,r9),fr3 */
L8c62010e: sa_motion_publish_stop(ctx,0x8c62010eu); r[0]=24u;goto L8c620110; /* mov #0x18,r0 */
L8c620110: sa_motion_publish_stop(ctx,0x8c620110u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620112; /* fmov.s fr3,@-r8 */
L8c620112: sa_motion_publish_stop(ctx,0x8c620112u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c620114; /* fmov.s @(r0,r7),fr3 */
L8c620114: sa_motion_publish_stop(ctx,0x8c620114u); r[0]=20u;goto L8c620116; /* mov #0x14,r0 */
L8c620116: sa_motion_publish_stop(ctx,0x8c620116u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620118; /* fmov.s fr3,@-r8 */
L8c620118: sa_motion_publish_stop(ctx,0x8c620118u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c62011a; /* fmov.s @(r0,r7),fr3 */
L8c62011a: sa_motion_publish_stop(ctx,0x8c62011au); r[0]=16u;goto L8c62011c; /* mov #0x10,r0 */
L8c62011c: sa_motion_publish_stop(ctx,0x8c62011cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62011e; /* fmov.s fr3,@-r8 */
L8c62011e: sa_motion_publish_stop(ctx,0x8c62011eu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c620120; /* fmov.s @(r0,r7),fr3 */
L8c620120: sa_motion_publish_stop(ctx,0x8c620120u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620122; /* fmov.s fr3,@-r8 */
L8c620122: sa_motion_publish_stop(ctx,0x8c620122u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c620124; /* mov.l r3,@-r8 */
L8c620124: sa_motion_publish_stop(ctx,0x8c620124u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c620126; /* pref @r8 */
L8c620126: sa_motion_publish_stop(ctx,0x8c620126u); r[2]=rd(m,bus,&io_status,0x8c62015cu,4);if(io_status)return io_status;goto L8c620128; /* mov.l 0x8c62015c,r2 */
L8c620128: sa_motion_publish_stop(ctx,0x8c620128u); r[8]+=32u;goto L8c62012a; /* add #0x20,r8 */
L8c62012a: sa_motion_publish_stop(ctx,0x8c62012au); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c62012c; /* mov.l @r2,r3 */
L8c62012c: sa_motion_publish_stop(ctx,0x8c62012cu); r[3]+=3u;goto L8c62012e; /* add #0x3,r3 */
L8c62012e: sa_motion_publish_stop(ctx,0x8c62012eu); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c620130; /* mov.l r3,@r2 */
L8c620130: sa_motion_publish_stop(ctx,0x8c620130u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c620132; /* mov.w @(0x2,r4),r0 */
L8c620132: sa_motion_publish_stop(ctx,0x8c620132u); r[0]=r[0]&65535u;goto L8c620134; /* extu.w r0,r0 */
L8c620134: sa_motion_publish_stop(ctx,0x8c620134u); *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c620136; /* cmp/ge r0,r14 */
L8c620136: sa_motion_publish_stop(ctx,0x8c620136u); if(!(*sr&1u))goto L8c62003c;goto L8c620138; /* bf 0x8c62003c */
L8c620138: sa_motion_publish_stop(ctx,0x8c62013au); goto L8c6203fa; /* bra 0x8c6203fa */
L8c620160: sa_motion_publish_stop(ctx,0x8c620160u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c620162; /* mov.w @(0x2,r4),r0 */
L8c620162: sa_motion_publish_stop(ctx,0x8c620162u); r[2]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c620164; /* mov.l @r11,r2 */
L8c620164: sa_motion_publish_stop(ctx,0x8c620164u); r[0]=r[0]&65535u;goto L8c620166; /* extu.w r0,r0 */
L8c620166: sa_motion_publish_stop(ctx,0x8c620166u); r[0]<<=2;goto L8c620168; /* shll2 r0 */
L8c620168: sa_motion_publish_stop(ctx,0x8c620168u); r[2]+=r[0];goto L8c62016a; /* add r0,r2 */
L8c62016a: sa_motion_publish_stop(ctx,0x8c62016au); wr(m,bus,&io_status,r[11],r[2],4);if(io_status)return io_status;goto L8c62016c; /* mov.l r2,@r11 */
L8c62016c: sa_motion_publish_stop(ctx,0x8c62016eu); r[11]=r[12];goto L8c6202a0; /* bra 0x8c6202a0 */
L8c620170: sa_motion_publish_stop(ctx,0x8c620170u); r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c620172; /* mov.w @r13+,r7 */
L8c620172: sa_motion_publish_stop(ctx,0x8c620172u); r[0]=16u;goto L8c620174; /* mov #0x10,r0 */
L8c620174: sa_motion_publish_stop(ctx,0x8c620174u); r[3]=rd(m,bus,&io_status,0x8c620410u,4);if(io_status)return io_status;goto L8c620176; /* mov.l 0x8c620410,r3 */
L8c620176: sa_motion_publish_stop(ctx,0x8c620176u); r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c620178; /* mov.w @r13+,r6 */
L8c620178: sa_motion_publish_stop(ctx,0x8c620178u); r[7]=r[7]&65535u;goto L8c62017a; /* extu.w r7,r7 */
L8c62017a: sa_motion_publish_stop(ctx,0x8c62017au); r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c62017c; /* mov.l @r3,r12 */
L8c62017c: sa_motion_publish_stop(ctx,0x8c62017cu); r[7]<<=2;goto L8c62017e; /* shll2 r7 */
L8c62017e: sa_motion_publish_stop(ctx,0x8c62017eu); r[6]=r[6]&65535u;goto L8c620180; /* extu.w r6,r6 */
L8c620180: sa_motion_publish_stop(ctx,0x8c620180u); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c620182; /* mov.w @r13+,r5 */
L8c620182: sa_motion_publish_stop(ctx,0x8c620182u); r[6]<<=2;goto L8c620184; /* shll2 r6 */
L8c620184: sa_motion_publish_stop(ctx,0x8c620184u); r[6]<<=2;goto L8c620186; /* shll2 r6 */
L8c620186: sa_motion_publish_stop(ctx,0x8c620186u); r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c620188; /* mov.w @r13+,r14 */
L8c620188: sa_motion_publish_stop(ctx,0x8c620188u); r[7]<<=2;goto L8c62018a; /* shll2 r7 */
L8c62018a: sa_motion_publish_stop(ctx,0x8c62018au); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c62018c; /* shll r7 */
L8c62018c: sa_motion_publish_stop(ctx,0x8c62018cu); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c62018e; /* shll r6 */
L8c62018e: sa_motion_publish_stop(ctx,0x8c62018eu); r[6]+=r[12];goto L8c620190; /* add r12,r6 */
L8c620190: sa_motion_publish_stop(ctx,0x8c620190u); r[7]+=r[12];goto L8c620192; /* add r12,r7 */
L8c620192: sa_motion_publish_stop(ctx,0x8c620192u); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c620194; /* fmov.s @(r0,r6),fr4 */
L8c620194: sa_motion_publish_stop(ctx,0x8c620194u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c620196; /* fmov.s @(r0,r7),fr3 */
L8c620196: sa_motion_publish_stop(ctx,0x8c620196u); r[5]=r[5]&65535u;goto L8c620198; /* extu.w r5,r5 */
L8c620198: sa_motion_publish_stop(ctx,0x8c620198u); fcopy(f,2u,4u);goto L8c62019a; /* fmov fr4,fr2 */
L8c62019a: sa_motion_publish_stop(ctx,0x8c62019au); r[5]<<=2;goto L8c62019c; /* shll2 r5 */
L8c62019c: sa_motion_publish_stop(ctx,0x8c62019cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c62019e; /* fsub fr3,fr2 */
L8c62019e: sa_motion_publish_stop(ctx,0x8c62019eu); r[14]=r[14]&65535u;goto L8c6201a0; /* extu.w r14,r14 */
L8c6201a0: sa_motion_publish_stop(ctx,0x8c6201a0u); r[5]<<=2;goto L8c6201a2; /* shll2 r5 */
L8c6201a2: sa_motion_publish_stop(ctx,0x8c6201a2u); r[14]<<=2;goto L8c6201a4; /* shll2 r14 */
L8c6201a4: sa_motion_publish_stop(ctx,0x8c6201a4u); r[0]=20u;goto L8c6201a6; /* mov #0x14,r0 */
L8c6201a6: sa_motion_publish_stop(ctx,0x8c6201a6u); fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c6201a8; /* fmov.s @(r0,r6),fr5 */
L8c6201a8: sa_motion_publish_stop(ctx,0x8c6201a8u); r[14]<<=2;goto L8c6201aa; /* shll2 r14 */
L8c6201aa: sa_motion_publish_stop(ctx,0x8c6201aau); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6201ac; /* shll r5 */
L8c6201ac: sa_motion_publish_stop(ctx,0x8c6201acu); r[5]+=r[12];goto L8c6201ae; /* add r12,r5 */
L8c6201ae: sa_motion_publish_stop(ctx,0x8c6201aeu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c6201b0; /* shll r14 */
L8c6201b0: sa_motion_publish_stop(ctx,0x8c6201b0u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6201b2; /* fmov.s @(r0,r5),fr3 */
L8c6201b2: sa_motion_publish_stop(ctx,0x8c6201b2u); fcopy(f,1u,5u);goto L8c6201b4; /* fmov fr5,fr1 */
L8c6201b4: sa_motion_publish_stop(ctx,0x8c6201b4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6201b6; /* fsub fr5,fr3 */
L8c6201b6: sa_motion_publish_stop(ctx,0x8c6201b6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6201b8; /* fmul fr3,fr2 */
L8c6201b8: sa_motion_publish_stop(ctx,0x8c6201b8u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6201ba; /* fmov.s @(r0,r7),fr3 */
L8c6201ba: sa_motion_publish_stop(ctx,0x8c6201bau); r[0]=16u;goto L8c6201bc; /* mov #0x10,r0 */
L8c6201bc: sa_motion_publish_stop(ctx,0x8c6201bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6201be; /* fsub fr3,fr1 */
L8c6201be: sa_motion_publish_stop(ctx,0x8c6201beu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6201c0; /* fmov.s @(r0,r5),fr3 */
L8c6201c0: sa_motion_publish_stop(ctx,0x8c6201c0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6201c2; /* fsub fr4,fr3 */
L8c6201c2: sa_motion_publish_stop(ctx,0x8c6201c2u); fcopy(f,4u,2u);goto L8c6201c4; /* fmov fr2,fr4 */
L8c6201c4: sa_motion_publish_stop(ctx,0x8c6201c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6201c6; /* fmul fr3,fr1 */
L8c6201c6: sa_motion_publish_stop(ctx,0x8c6201c6u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6201c8; /* fldi0 fr3 */
L8c6201c8: sa_motion_publish_stop(ctx,0x8c6201c8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c6201ca; /* fsub fr1,fr4 */
L8c6201ca: sa_motion_publish_stop(ctx,0x8c6201cau); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c6201cc; /* fcmp/gt fr3,fr4 */
L8c6201cc: sa_motion_publish_stop(ctx,0x8c6201ceu); take=(*sr&1u);r[14]+=r[12];if(take)goto L8c62029e;goto L8c6201d0; /* bt/s 0x8c62029e */
L8c6201d0: sa_motion_publish_stop(ctx,0x8c6201d0u); r[0]=28u;goto L8c6201d2; /* mov #0x1c,r0 */
L8c6201d2: sa_motion_publish_stop(ctx,0x8c6201d2u); r[3]=rd(m,bus,&io_status,0x8c620414u,4);if(io_status)return io_status;goto L8c6201d4; /* mov.l 0x8c620414,r3 */
L8c6201d4: sa_motion_publish_stop(ctx,0x8c6201d4u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6201d6; /* fmov.s @(r0,r9),fr3 */
L8c6201d6: sa_motion_publish_stop(ctx,0x8c6201d6u); r[8]+=32u;goto L8c6201d8; /* add #0x20,r8 */
L8c6201d8: sa_motion_publish_stop(ctx,0x8c6201d8u); r[0]=24u;goto L8c6201da; /* mov #0x18,r0 */
L8c6201da: sa_motion_publish_stop(ctx,0x8c6201dau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201dc; /* fmov.s fr3,@-r8 */
L8c6201dc: sa_motion_publish_stop(ctx,0x8c6201dcu); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6201de; /* fmov.s @(r0,r9),fr3 */
L8c6201de: sa_motion_publish_stop(ctx,0x8c6201deu); r[0]=20u;goto L8c6201e0; /* mov #0x14,r0 */
L8c6201e0: sa_motion_publish_stop(ctx,0x8c6201e0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201e2; /* fmov.s fr3,@-r8 */
L8c6201e2: sa_motion_publish_stop(ctx,0x8c6201e2u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6201e4; /* fmov.s @(r0,r9),fr3 */
L8c6201e4: sa_motion_publish_stop(ctx,0x8c6201e4u); r[0]=16u;goto L8c6201e6; /* mov #0x10,r0 */
L8c6201e6: sa_motion_publish_stop(ctx,0x8c6201e6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201e8; /* fmov.s fr3,@-r8 */
L8c6201e8: sa_motion_publish_stop(ctx,0x8c6201e8u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6201ea; /* fmov.s @(r0,r9),fr3 */
L8c6201ea: sa_motion_publish_stop(ctx,0x8c6201eau); r[0]=24u;goto L8c6201ec; /* mov #0x18,r0 */
L8c6201ec: sa_motion_publish_stop(ctx,0x8c6201ecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201ee; /* fmov.s fr3,@-r8 */
L8c6201ee: sa_motion_publish_stop(ctx,0x8c6201eeu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6201f0; /* fmov.s @(r0,r7),fr3 */
L8c6201f0: sa_motion_publish_stop(ctx,0x8c6201f0u); r[0]=20u;goto L8c6201f2; /* mov #0x14,r0 */
L8c6201f2: sa_motion_publish_stop(ctx,0x8c6201f2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201f4; /* fmov.s fr3,@-r8 */
L8c6201f4: sa_motion_publish_stop(ctx,0x8c6201f4u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6201f6; /* fmov.s @(r0,r7),fr3 */
L8c6201f6: sa_motion_publish_stop(ctx,0x8c6201f6u); r[0]=16u;goto L8c6201f8; /* mov #0x10,r0 */
L8c6201f8: sa_motion_publish_stop(ctx,0x8c6201f8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201fa; /* fmov.s fr3,@-r8 */
L8c6201fa: sa_motion_publish_stop(ctx,0x8c6201fau); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6201fc; /* fmov.s @(r0,r7),fr3 */
L8c6201fc: sa_motion_publish_stop(ctx,0x8c6201fcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201fe; /* fmov.s fr3,@-r8 */
L8c6201fe: sa_motion_publish_stop(ctx,0x8c6201feu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c620200; /* mov.l r3,@-r8 */
L8c620200: sa_motion_publish_stop(ctx,0x8c620200u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c620202; /* pref @r8 */
L8c620202: sa_motion_publish_stop(ctx,0x8c620202u); r[0]=28u;goto L8c620204; /* mov #0x1c,r0 */
L8c620204: sa_motion_publish_stop(ctx,0x8c620204u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620206; /* fmov.s @(r0,r9),fr3 */
L8c620206: sa_motion_publish_stop(ctx,0x8c620206u); r[8]+=64u;goto L8c620208; /* add #0x40,r8 */
L8c620208: sa_motion_publish_stop(ctx,0x8c620208u); r[0]=24u;goto L8c62020a; /* mov #0x18,r0 */
L8c62020a: sa_motion_publish_stop(ctx,0x8c62020au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62020c; /* fmov.s fr3,@-r8 */
L8c62020c: sa_motion_publish_stop(ctx,0x8c62020cu); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62020e; /* fmov.s @(r0,r9),fr3 */
L8c62020e: sa_motion_publish_stop(ctx,0x8c62020eu); r[0]=20u;goto L8c620210; /* mov #0x14,r0 */
L8c620210: sa_motion_publish_stop(ctx,0x8c620210u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620212; /* fmov.s fr3,@-r8 */
L8c620212: sa_motion_publish_stop(ctx,0x8c620212u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620214; /* fmov.s @(r0,r9),fr3 */
L8c620214: sa_motion_publish_stop(ctx,0x8c620214u); r[0]=16u;goto L8c620216; /* mov #0x10,r0 */
L8c620216: sa_motion_publish_stop(ctx,0x8c620216u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620218; /* fmov.s fr3,@-r8 */
L8c620218: sa_motion_publish_stop(ctx,0x8c620218u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62021a; /* fmov.s @(r0,r9),fr3 */
L8c62021a: sa_motion_publish_stop(ctx,0x8c62021au); r[0]=24u;goto L8c62021c; /* mov #0x18,r0 */
L8c62021c: sa_motion_publish_stop(ctx,0x8c62021cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62021e; /* fmov.s fr3,@-r8 */
L8c62021e: sa_motion_publish_stop(ctx,0x8c62021eu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620220; /* fmov.s @(r0,r6),fr3 */
L8c620220: sa_motion_publish_stop(ctx,0x8c620220u); r[0]=20u;goto L8c620222; /* mov #0x14,r0 */
L8c620222: sa_motion_publish_stop(ctx,0x8c620222u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620224; /* fmov.s fr3,@-r8 */
L8c620224: sa_motion_publish_stop(ctx,0x8c620224u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620226; /* fmov.s @(r0,r6),fr3 */
L8c620226: sa_motion_publish_stop(ctx,0x8c620226u); r[0]=16u;goto L8c620228; /* mov #0x10,r0 */
L8c620228: sa_motion_publish_stop(ctx,0x8c620228u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62022a; /* fmov.s fr3,@-r8 */
L8c62022a: sa_motion_publish_stop(ctx,0x8c62022au); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c62022c; /* fmov.s @(r0,r6),fr3 */
L8c62022c: sa_motion_publish_stop(ctx,0x8c62022cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62022e; /* fmov.s fr3,@-r8 */
L8c62022e: sa_motion_publish_stop(ctx,0x8c62022eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c620230; /* mov.l r3,@-r8 */
L8c620230: sa_motion_publish_stop(ctx,0x8c620230u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c620232; /* pref @r8 */
L8c620232: sa_motion_publish_stop(ctx,0x8c620232u); r[0]=28u;goto L8c620234; /* mov #0x1c,r0 */
L8c620234: sa_motion_publish_stop(ctx,0x8c620234u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620236; /* fmov.s @(r0,r9),fr3 */
L8c620236: sa_motion_publish_stop(ctx,0x8c620236u); r[8]+=64u;goto L8c620238; /* add #0x40,r8 */
L8c620238: sa_motion_publish_stop(ctx,0x8c620238u); r[0]=24u;goto L8c62023a; /* mov #0x18,r0 */
L8c62023a: sa_motion_publish_stop(ctx,0x8c62023au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62023c; /* fmov.s fr3,@-r8 */
L8c62023c: sa_motion_publish_stop(ctx,0x8c62023cu); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62023e; /* fmov.s @(r0,r9),fr3 */
L8c62023e: sa_motion_publish_stop(ctx,0x8c62023eu); r[0]=20u;goto L8c620240; /* mov #0x14,r0 */
L8c620240: sa_motion_publish_stop(ctx,0x8c620240u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620242; /* fmov.s fr3,@-r8 */
L8c620242: sa_motion_publish_stop(ctx,0x8c620242u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620244; /* fmov.s @(r0,r9),fr3 */
L8c620244: sa_motion_publish_stop(ctx,0x8c620244u); r[0]=16u;goto L8c620246; /* mov #0x10,r0 */
L8c620246: sa_motion_publish_stop(ctx,0x8c620246u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620248; /* fmov.s fr3,@-r8 */
L8c620248: sa_motion_publish_stop(ctx,0x8c620248u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62024a; /* fmov.s @(r0,r9),fr3 */
L8c62024a: sa_motion_publish_stop(ctx,0x8c62024au); r[0]=24u;goto L8c62024c; /* mov #0x18,r0 */
L8c62024c: sa_motion_publish_stop(ctx,0x8c62024cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62024e; /* fmov.s fr3,@-r8 */
L8c62024e: sa_motion_publish_stop(ctx,0x8c62024eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620250; /* fmov.s @(r0,r5),fr3 */
L8c620250: sa_motion_publish_stop(ctx,0x8c620250u); r[0]=20u;goto L8c620252; /* mov #0x14,r0 */
L8c620252: sa_motion_publish_stop(ctx,0x8c620252u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620254; /* fmov.s fr3,@-r8 */
L8c620254: sa_motion_publish_stop(ctx,0x8c620254u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620256; /* fmov.s @(r0,r5),fr3 */
L8c620256: sa_motion_publish_stop(ctx,0x8c620256u); r[0]=16u;goto L8c620258; /* mov #0x10,r0 */
L8c620258: sa_motion_publish_stop(ctx,0x8c620258u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62025a; /* fmov.s fr3,@-r8 */
L8c62025a: sa_motion_publish_stop(ctx,0x8c62025au); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c62025c; /* fmov.s @(r0,r5),fr3 */
L8c62025c: sa_motion_publish_stop(ctx,0x8c62025cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62025e; /* fmov.s fr3,@-r8 */
L8c62025e: sa_motion_publish_stop(ctx,0x8c62025eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c620260; /* mov.l r3,@-r8 */
L8c620260: sa_motion_publish_stop(ctx,0x8c620260u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c620262; /* pref @r8 */
L8c620262: sa_motion_publish_stop(ctx,0x8c620262u); r[0]=28u;goto L8c620264; /* mov #0x1c,r0 */
L8c620264: sa_motion_publish_stop(ctx,0x8c620264u); r[3]=rd(m,bus,&io_status,0x8c620418u,4);if(io_status)return io_status;goto L8c620266; /* mov.l 0x8c620418,r3 */
L8c620266: sa_motion_publish_stop(ctx,0x8c620266u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620268; /* fmov.s @(r0,r9),fr3 */
L8c620268: sa_motion_publish_stop(ctx,0x8c620268u); r[8]+=64u;goto L8c62026a; /* add #0x40,r8 */
L8c62026a: sa_motion_publish_stop(ctx,0x8c62026au); r[0]=24u;goto L8c62026c; /* mov #0x18,r0 */
L8c62026c: sa_motion_publish_stop(ctx,0x8c62026cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62026e; /* fmov.s fr3,@-r8 */
L8c62026e: sa_motion_publish_stop(ctx,0x8c62026eu); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620270; /* fmov.s @(r0,r9),fr3 */
L8c620270: sa_motion_publish_stop(ctx,0x8c620270u); r[0]=20u;goto L8c620272; /* mov #0x14,r0 */
L8c620272: sa_motion_publish_stop(ctx,0x8c620272u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620274; /* fmov.s fr3,@-r8 */
L8c620274: sa_motion_publish_stop(ctx,0x8c620274u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620276; /* fmov.s @(r0,r9),fr3 */
L8c620276: sa_motion_publish_stop(ctx,0x8c620276u); r[0]=16u;goto L8c620278; /* mov #0x10,r0 */
L8c620278: sa_motion_publish_stop(ctx,0x8c620278u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62027a; /* fmov.s fr3,@-r8 */
L8c62027a: sa_motion_publish_stop(ctx,0x8c62027au); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62027c; /* fmov.s @(r0,r9),fr3 */
L8c62027c: sa_motion_publish_stop(ctx,0x8c62027cu); r[0]=24u;goto L8c62027e; /* mov #0x18,r0 */
L8c62027e: sa_motion_publish_stop(ctx,0x8c62027eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620280; /* fmov.s fr3,@-r8 */
L8c620280: sa_motion_publish_stop(ctx,0x8c620280u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c620282; /* fmov.s @(r0,r14),fr3 */
L8c620282: sa_motion_publish_stop(ctx,0x8c620282u); r[0]=20u;goto L8c620284; /* mov #0x14,r0 */
L8c620284: sa_motion_publish_stop(ctx,0x8c620284u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620286; /* fmov.s fr3,@-r8 */
L8c620286: sa_motion_publish_stop(ctx,0x8c620286u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c620288; /* fmov.s @(r0,r14),fr3 */
L8c620288: sa_motion_publish_stop(ctx,0x8c620288u); r[0]=16u;goto L8c62028a; /* mov #0x10,r0 */
L8c62028a: sa_motion_publish_stop(ctx,0x8c62028au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62028c; /* fmov.s fr3,@-r8 */
L8c62028c: sa_motion_publish_stop(ctx,0x8c62028cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c62028e; /* fmov.s @(r0,r14),fr3 */
L8c62028e: sa_motion_publish_stop(ctx,0x8c62028eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620290; /* fmov.s fr3,@-r8 */
L8c620290: sa_motion_publish_stop(ctx,0x8c620290u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c620292; /* mov.l r3,@-r8 */
L8c620292: sa_motion_publish_stop(ctx,0x8c620292u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c620294; /* pref @r8 */
L8c620294: sa_motion_publish_stop(ctx,0x8c620294u); r[2]=rd(m,bus,&io_status,0x8c62041cu,4);if(io_status)return io_status;goto L8c620296; /* mov.l 0x8c62041c,r2 */
L8c620296: sa_motion_publish_stop(ctx,0x8c620296u); r[8]+=32u;goto L8c620298; /* add #0x20,r8 */
L8c620298: sa_motion_publish_stop(ctx,0x8c620298u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c62029a; /* mov.l @r2,r3 */
L8c62029a: sa_motion_publish_stop(ctx,0x8c62029au); r[3]+=4u;goto L8c62029c; /* add #0x4,r3 */
L8c62029c: sa_motion_publish_stop(ctx,0x8c62029cu); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c62029e; /* mov.l r3,@r2 */
L8c62029e: sa_motion_publish_stop(ctx,0x8c62029eu); r[11]+=1u;goto L8c6202a0; /* add #0x1,r11 */
L8c6202a0: sa_motion_publish_stop(ctx,0x8c6202a0u); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c6202a2; /* mov.w @(0x2,r4),r0 */
L8c6202a2: sa_motion_publish_stop(ctx,0x8c6202a2u); r[0]=r[0]&65535u;goto L8c6202a4; /* extu.w r0,r0 */
L8c6202a4: sa_motion_publish_stop(ctx,0x8c6202a4u); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c6202a6; /* cmp/ge r0,r11 */
L8c6202a6: sa_motion_publish_stop(ctx,0x8c6202a6u); if((*sr&1u))goto L8c6202ac;goto L8c6202a8; /* bt 0x8c6202ac */
L8c6202a8: sa_motion_publish_stop(ctx,0x8c6202aau); goto L8c620170; /* bra 0x8c620170 */
L8c6202ac: sa_motion_publish_stop(ctx,0x8c6202aeu); goto L8c6203fa; /* bra 0x8c6203fa */
L8c6202b0: sa_motion_publish_stop(ctx,0x8c6202b2u); r[10]=r[12];goto L8c6203ba; /* bra 0x8c6203ba */
L8c6202b4: sa_motion_publish_stop(ctx,0x8c6202b4u); r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6202b6; /* mov.w @r13+,r14 */
L8c6202b6: sa_motion_publish_stop(ctx,0x8c6202b6u); r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c6202b8; /* mov.l @r11,r3 */
L8c6202b8: sa_motion_publish_stop(ctx,0x8c6202b8u); r[14]=r[14]&65535u;goto L8c6202ba; /* extu.w r14,r14 */
L8c6202ba: sa_motion_publish_stop(ctx,0x8c6202bau); r[2]=rd(m,bus,&io_status,0x8c620410u,4);if(io_status)return io_status;goto L8c6202bc; /* mov.l 0x8c620410,r2 */
L8c6202bc: sa_motion_publish_stop(ctx,0x8c6202bcu); r[3]+=r[14];goto L8c6202be; /* add r14,r3 */
L8c6202be: sa_motion_publish_stop(ctx,0x8c6202beu); wr(m,bus,&io_status,r[11],r[3],4);if(io_status)return io_status;goto L8c6202c0; /* mov.l r3,@r11 */
L8c6202c0: sa_motion_publish_stop(ctx,0x8c6202c0u); r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c6202c2; /* mov.w @(0x2,r13),r0 */
L8c6202c2: sa_motion_publish_stop(ctx,0x8c6202c2u); r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c6202c4; /* mov.w @r13,r7 */
L8c6202c4: sa_motion_publish_stop(ctx,0x8c6202c4u); r[5]=r[0]&65535u;goto L8c6202c6; /* extu.w r0,r5 */
L8c6202c6: sa_motion_publish_stop(ctx,0x8c6202c6u); r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c6202c8; /* mov.w @(0x4,r13),r0 */
L8c6202c8: sa_motion_publish_stop(ctx,0x8c6202c8u); r[5]<<=2;goto L8c6202ca; /* shll2 r5 */
L8c6202ca: sa_motion_publish_stop(ctx,0x8c6202cau); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6202cc; /* mov.l @r2,r3 */
L8c6202cc: sa_motion_publish_stop(ctx,0x8c6202ccu); r[7]=r[7]&65535u;goto L8c6202ce; /* extu.w r7,r7 */
L8c6202ce: sa_motion_publish_stop(ctx,0x8c6202ceu); r[7]<<=2;goto L8c6202d0; /* shll2 r7 */
L8c6202d0: sa_motion_publish_stop(ctx,0x8c6202d0u); r[5]<<=2;goto L8c6202d2; /* shll2 r5 */
L8c6202d2: sa_motion_publish_stop(ctx,0x8c6202d2u); r[6]=r[0]&65535u;goto L8c6202d4; /* extu.w r0,r6 */
L8c6202d4: sa_motion_publish_stop(ctx,0x8c6202d4u); r[7]<<=2;goto L8c6202d6; /* shll2 r7 */
L8c6202d6: sa_motion_publish_stop(ctx,0x8c6202d6u); *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c6202d8; /* shll r7 */
L8c6202d8: sa_motion_publish_stop(ctx,0x8c6202d8u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6202da; /* shll r5 */
L8c6202da: sa_motion_publish_stop(ctx,0x8c6202dau); r[0]=16u;goto L8c6202dc; /* mov #0x10,r0 */
L8c6202dc: sa_motion_publish_stop(ctx,0x8c6202dcu); r[5]+=r[3];goto L8c6202de; /* add r3,r5 */
L8c6202de: sa_motion_publish_stop(ctx,0x8c6202deu); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6202e0; /* fmov.s @(r0,r5),fr5 */
L8c6202e0: sa_motion_publish_stop(ctx,0x8c6202e0u); r[7]+=r[3];goto L8c6202e2; /* add r3,r7 */
L8c6202e2: sa_motion_publish_stop(ctx,0x8c6202e2u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6202e4; /* fmov.s @(r0,r7),fr3 */
L8c6202e4: sa_motion_publish_stop(ctx,0x8c6202e4u); r[6]<<=2;goto L8c6202e6; /* shll2 r6 */
L8c6202e6: sa_motion_publish_stop(ctx,0x8c6202e6u); fcopy(f,2u,5u);goto L8c6202e8; /* fmov fr5,fr2 */
L8c6202e8: sa_motion_publish_stop(ctx,0x8c6202e8u); r[6]<<=2;goto L8c6202ea; /* shll2 r6 */
L8c6202ea: sa_motion_publish_stop(ctx,0x8c6202eau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6202ec; /* fsub fr3,fr2 */
L8c6202ec: sa_motion_publish_stop(ctx,0x8c6202ecu); r[0]=20u;goto L8c6202ee; /* mov #0x14,r0 */
L8c6202ee: sa_motion_publish_stop(ctx,0x8c6202eeu); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c6202f0; /* shll r6 */
L8c6202f0: sa_motion_publish_stop(ctx,0x8c6202f0u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c6202f2; /* fmov.s @(r0,r5),fr4 */
L8c6202f2: sa_motion_publish_stop(ctx,0x8c6202f2u); r[6]+=r[3];goto L8c6202f4; /* add r3,r6 */
L8c6202f4: sa_motion_publish_stop(ctx,0x8c6202f4u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6202f6; /* fmov.s @(r0,r6),fr3 */
L8c6202f6: sa_motion_publish_stop(ctx,0x8c6202f6u); fcopy(f,1u,4u);goto L8c6202f8; /* fmov fr4,fr1 */
L8c6202f8: sa_motion_publish_stop(ctx,0x8c6202f8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6202fa; /* fsub fr4,fr3 */
L8c6202fa: sa_motion_publish_stop(ctx,0x8c6202fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6202fc; /* fmul fr3,fr2 */
L8c6202fc: sa_motion_publish_stop(ctx,0x8c6202fcu); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6202fe; /* fmov.s @(r0,r7),fr3 */
L8c6202fe: sa_motion_publish_stop(ctx,0x8c6202feu); r[0]=16u;goto L8c620300; /* mov #0x10,r0 */
L8c620300: sa_motion_publish_stop(ctx,0x8c620300u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c620302; /* fsub fr3,fr1 */
L8c620302: sa_motion_publish_stop(ctx,0x8c620302u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620304; /* fmov.s @(r0,r6),fr3 */
L8c620304: sa_motion_publish_stop(ctx,0x8c620304u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c620306; /* fsub fr5,fr3 */
L8c620306: sa_motion_publish_stop(ctx,0x8c620306u); fcopy(f,4u,2u);goto L8c620308; /* fmov fr2,fr4 */
L8c620308: sa_motion_publish_stop(ctx,0x8c620308u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c62030a; /* fmul fr3,fr1 */
L8c62030a: sa_motion_publish_stop(ctx,0x8c62030au); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c62030c; /* fldi0 fr3 */
L8c62030c: sa_motion_publish_stop(ctx,0x8c62030cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c62030e; /* fsub fr1,fr4 */
L8c62030e: sa_motion_publish_stop(ctx,0x8c62030eu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c620310; /* fcmp/gt fr3,fr4 */
L8c620310: sa_motion_publish_stop(ctx,0x8c620310u); if(!(*sr&1u))goto L8c620318;goto L8c620312; /* bf 0x8c620318 */
L8c620312: sa_motion_publish_stop(ctx,0x8c620312u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c620314; /* shll r14 */
L8c620314: sa_motion_publish_stop(ctx,0x8c620316u); r[13]+=r[14];goto L8c6203b8; /* bra 0x8c6203b8 */
L8c620318: sa_motion_publish_stop(ctx,0x8c620318u); r[3]=rd(m,bus,&io_status,0x8c62041cu,4);if(io_status)return io_status;goto L8c62031a; /* mov.l 0x8c62041c,r3 */
L8c62031a: sa_motion_publish_stop(ctx,0x8c62031au); r[7]=r[14];goto L8c62031c; /* mov r14,r7 */
L8c62031c: sa_motion_publish_stop(ctx,0x8c62031cu); r[7]+=4294967295u;goto L8c62031e; /* add #-0x1,r7 */
L8c62031e: sa_motion_publish_stop(ctx,0x8c62031eu); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c620320; /* mov.l @r3,r2 */
L8c620320: sa_motion_publish_stop(ctx,0x8c620320u); *sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c620322; /* cmp/pl r7 */
L8c620322: sa_motion_publish_stop(ctx,0x8c620322u); r[2]+=r[14];goto L8c620324; /* add r14,r2 */
L8c620324: sa_motion_publish_stop(ctx,0x8c620324u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c620326; /* mov.l r2,@r3 */
L8c620326: sa_motion_publish_stop(ctx,0x8c620328u); take=!(*sr&1u);r[6]=r[12];if(take)goto L8c620374;goto L8c62032a; /* bf/s 0x8c620374 */
L8c62032a: sa_motion_publish_stop(ctx,0x8c62032au); r[0]=28u;goto L8c62032c; /* mov #0x1c,r0 */
L8c62032c: sa_motion_publish_stop(ctx,0x8c62032cu); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c62032e; /* mov.w @r13+,r5 */
L8c62032e: sa_motion_publish_stop(ctx,0x8c62032eu); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620330; /* fmov.s @(r0,r9),fr3 */
L8c620330: sa_motion_publish_stop(ctx,0x8c620330u); r[8]+=32u;goto L8c620332; /* add #0x20,r8 */
L8c620332: sa_motion_publish_stop(ctx,0x8c620332u); r[0]=24u;goto L8c620334; /* mov #0x18,r0 */
L8c620334: sa_motion_publish_stop(ctx,0x8c620334u); r[2]=rd(m,bus,&io_status,0x8c620410u,4);if(io_status)return io_status;goto L8c620336; /* mov.l 0x8c620410,r2 */
L8c620336: sa_motion_publish_stop(ctx,0x8c620336u); r[5]=r[5]&65535u;goto L8c620338; /* extu.w r5,r5 */
L8c620338: sa_motion_publish_stop(ctx,0x8c620338u); r[5]<<=2;goto L8c62033a; /* shll2 r5 */
L8c62033a: sa_motion_publish_stop(ctx,0x8c62033au); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c62033c; /* mov.l @r2,r3 */
L8c62033c: sa_motion_publish_stop(ctx,0x8c62033cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62033e; /* fmov.s fr3,@-r8 */
L8c62033e: sa_motion_publish_stop(ctx,0x8c62033eu); r[5]<<=2;goto L8c620340; /* shll2 r5 */
L8c620340: sa_motion_publish_stop(ctx,0x8c620340u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620342; /* fmov.s @(r0,r9),fr3 */
L8c620342: sa_motion_publish_stop(ctx,0x8c620342u); r[0]=20u;goto L8c620344; /* mov #0x14,r0 */
L8c620344: sa_motion_publish_stop(ctx,0x8c620344u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c620346; /* shll r5 */
L8c620346: sa_motion_publish_stop(ctx,0x8c620346u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620348; /* fmov.s fr3,@-r8 */
L8c620348: sa_motion_publish_stop(ctx,0x8c620348u); r[5]+=r[3];goto L8c62034a; /* add r3,r5 */
L8c62034a: sa_motion_publish_stop(ctx,0x8c62034au); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62034c; /* fmov.s @(r0,r9),fr3 */
L8c62034c: sa_motion_publish_stop(ctx,0x8c62034cu); r[0]=16u;goto L8c62034e; /* mov #0x10,r0 */
L8c62034e: sa_motion_publish_stop(ctx,0x8c62034eu); r[3]=rd(m,bus,&io_status,0x8c620414u,4);if(io_status)return io_status;goto L8c620350; /* mov.l 0x8c620414,r3 */
L8c620350: sa_motion_publish_stop(ctx,0x8c620350u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620352; /* fmov.s fr3,@-r8 */
L8c620352: sa_motion_publish_stop(ctx,0x8c620352u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620354; /* fmov.s @(r0,r9),fr3 */
L8c620354: sa_motion_publish_stop(ctx,0x8c620354u); r[0]=24u;goto L8c620356; /* mov #0x18,r0 */
L8c620356: sa_motion_publish_stop(ctx,0x8c620356u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620358; /* fmov.s fr3,@-r8 */
L8c620358: sa_motion_publish_stop(ctx,0x8c620358u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c62035a; /* fmov.s @(r0,r5),fr3 */
L8c62035a: sa_motion_publish_stop(ctx,0x8c62035au); r[0]=20u;goto L8c62035c; /* mov #0x14,r0 */
L8c62035c: sa_motion_publish_stop(ctx,0x8c62035cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62035e; /* fmov.s fr3,@-r8 */
L8c62035e: sa_motion_publish_stop(ctx,0x8c62035eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620360; /* fmov.s @(r0,r5),fr3 */
L8c620360: sa_motion_publish_stop(ctx,0x8c620360u); r[0]=16u;goto L8c620362; /* mov #0x10,r0 */
L8c620362: sa_motion_publish_stop(ctx,0x8c620362u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620364; /* fmov.s fr3,@-r8 */
L8c620364: sa_motion_publish_stop(ctx,0x8c620364u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620366; /* fmov.s @(r0,r5),fr3 */
L8c620366: sa_motion_publish_stop(ctx,0x8c620366u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620368; /* fmov.s fr3,@-r8 */
L8c620368: sa_motion_publish_stop(ctx,0x8c620368u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c62036a; /* mov.l r3,@-r8 */
L8c62036a: sa_motion_publish_stop(ctx,0x8c62036au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c62036c; /* pref @r8 */
L8c62036c: sa_motion_publish_stop(ctx,0x8c62036cu); r[6]+=1u;goto L8c62036e; /* add #0x1,r6 */
L8c62036e: sa_motion_publish_stop(ctx,0x8c62036eu); *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c620370; /* cmp/ge r7,r6 */
L8c620370: sa_motion_publish_stop(ctx,0x8c620372u); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c62032a;goto L8c620374; /* bf/s 0x8c62032a */
L8c620374: sa_motion_publish_stop(ctx,0x8c620374u); r[0]=28u;goto L8c620376; /* mov #0x1c,r0 */
L8c620376: sa_motion_publish_stop(ctx,0x8c620376u); r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c620378; /* mov.w @r13+,r5 */
L8c620378: sa_motion_publish_stop(ctx,0x8c620378u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62037a; /* fmov.s @(r0,r9),fr3 */
L8c62037a: sa_motion_publish_stop(ctx,0x8c62037au); r[8]+=32u;goto L8c62037c; /* add #0x20,r8 */
L8c62037c: sa_motion_publish_stop(ctx,0x8c62037cu); r[0]=24u;goto L8c62037e; /* mov #0x18,r0 */
L8c62037e: sa_motion_publish_stop(ctx,0x8c62037eu); r[2]=rd(m,bus,&io_status,0x8c620410u,4);if(io_status)return io_status;goto L8c620380; /* mov.l 0x8c620410,r2 */
L8c620380: sa_motion_publish_stop(ctx,0x8c620380u); r[5]=r[5]&65535u;goto L8c620382; /* extu.w r5,r5 */
L8c620382: sa_motion_publish_stop(ctx,0x8c620382u); r[5]<<=2;goto L8c620384; /* shll2 r5 */
L8c620384: sa_motion_publish_stop(ctx,0x8c620384u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c620386; /* mov.l @r2,r3 */
L8c620386: sa_motion_publish_stop(ctx,0x8c620386u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620388; /* fmov.s fr3,@-r8 */
L8c620388: sa_motion_publish_stop(ctx,0x8c620388u); r[5]<<=2;goto L8c62038a; /* shll2 r5 */
L8c62038a: sa_motion_publish_stop(ctx,0x8c62038au); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62038c; /* fmov.s @(r0,r9),fr3 */
L8c62038c: sa_motion_publish_stop(ctx,0x8c62038cu); r[0]=20u;goto L8c62038e; /* mov #0x14,r0 */
L8c62038e: sa_motion_publish_stop(ctx,0x8c62038eu); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c620390; /* shll r5 */
L8c620390: sa_motion_publish_stop(ctx,0x8c620390u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620392; /* fmov.s fr3,@-r8 */
L8c620392: sa_motion_publish_stop(ctx,0x8c620392u); r[5]+=r[3];goto L8c620394; /* add r3,r5 */
L8c620394: sa_motion_publish_stop(ctx,0x8c620394u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620396; /* fmov.s @(r0,r9),fr3 */
L8c620396: sa_motion_publish_stop(ctx,0x8c620396u); r[0]=16u;goto L8c620398; /* mov #0x10,r0 */
L8c620398: sa_motion_publish_stop(ctx,0x8c620398u); r[3]=rd(m,bus,&io_status,0x8c620418u,4);if(io_status)return io_status;goto L8c62039a; /* mov.l 0x8c620418,r3 */
L8c62039a: sa_motion_publish_stop(ctx,0x8c62039au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62039c; /* fmov.s fr3,@-r8 */
L8c62039c: sa_motion_publish_stop(ctx,0x8c62039cu); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62039e; /* fmov.s @(r0,r9),fr3 */
L8c62039e: sa_motion_publish_stop(ctx,0x8c62039eu); r[0]=24u;goto L8c6203a0; /* mov #0x18,r0 */
L8c6203a0: sa_motion_publish_stop(ctx,0x8c6203a0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6203a2; /* fmov.s fr3,@-r8 */
L8c6203a2: sa_motion_publish_stop(ctx,0x8c6203a2u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6203a4; /* fmov.s @(r0,r5),fr3 */
L8c6203a4: sa_motion_publish_stop(ctx,0x8c6203a4u); r[0]=20u;goto L8c6203a6; /* mov #0x14,r0 */
L8c6203a6: sa_motion_publish_stop(ctx,0x8c6203a6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6203a8; /* fmov.s fr3,@-r8 */
L8c6203a8: sa_motion_publish_stop(ctx,0x8c6203a8u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6203aa; /* fmov.s @(r0,r5),fr3 */
L8c6203aa: sa_motion_publish_stop(ctx,0x8c6203aau); r[0]=16u;goto L8c6203ac; /* mov #0x10,r0 */
L8c6203ac: sa_motion_publish_stop(ctx,0x8c6203acu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6203ae; /* fmov.s fr3,@-r8 */
L8c6203ae: sa_motion_publish_stop(ctx,0x8c6203aeu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6203b0; /* fmov.s @(r0,r5),fr3 */
L8c6203b0: sa_motion_publish_stop(ctx,0x8c6203b0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6203b2; /* fmov.s fr3,@-r8 */
L8c6203b2: sa_motion_publish_stop(ctx,0x8c6203b2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6203b4; /* mov.l r3,@-r8 */
L8c6203b4: sa_motion_publish_stop(ctx,0x8c6203b4u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6203b6; /* pref @r8 */
L8c6203b6: sa_motion_publish_stop(ctx,0x8c6203b6u); r[8]+=32u;goto L8c6203b8; /* add #0x20,r8 */
L8c6203b8: sa_motion_publish_stop(ctx,0x8c6203b8u); r[10]+=1u;goto L8c6203ba; /* add #0x1,r10 */
L8c6203ba: sa_motion_publish_stop(ctx,0x8c6203bau); r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c6203bc; /* mov.w @(0x2,r4),r0 */
L8c6203bc: sa_motion_publish_stop(ctx,0x8c6203bcu); r[0]=r[0]&65535u;goto L8c6203be; /* extu.w r0,r0 */
L8c6203be: sa_motion_publish_stop(ctx,0x8c6203beu); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c6203c0; /* cmp/ge r0,r10 */
L8c6203c0: sa_motion_publish_stop(ctx,0x8c6203c0u); if((*sr&1u))goto L8c6203c6;goto L8c6203c2; /* bt 0x8c6203c6 */
L8c6203c2: sa_motion_publish_stop(ctx,0x8c6203c4u); goto L8c6202b4; /* bra 0x8c6202b4 */
L8c6203c6: sa_motion_publish_stop(ctx,0x8c6203c8u); goto L8c6203fa; /* bra 0x8c6203fa */
L8c6203ca: sa_motion_publish_stop(ctx,0x8c6203cau); r[1]=rd(m,bus,&io_status,0x8c620420u,4);if(io_status)return io_status;goto L8c6203cc; /* mov.l 0x8c620420,r1 */
L8c6203cc: sa_motion_publish_stop(ctx,0x8c6203ccu); wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c6203ce; /* mov.l r4,@r1 */
L8c6203ce: sa_motion_publish_stop(ctx,0x8c6203ceu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6203d0; /* frchg */
L8c6203d0: sa_motion_publish_stop(ctx,0x8c6203d0u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c6203d2; /* fmov.s fr0,@-r15 */
L8c6203d2: sa_motion_publish_stop(ctx,0x8c6203d2u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c6203d4; /* fmov.s fr1,@-r15 */
L8c6203d4: sa_motion_publish_stop(ctx,0x8c6203d4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c6203d6; /* fmov.s fr2,@-r15 */
L8c6203d6: sa_motion_publish_stop(ctx,0x8c6203d6u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c6203d8; /* fmov.s fr3,@-r15 */
L8c6203d8: sa_motion_publish_stop(ctx,0x8c6203d8u); r[0]=rd(m,bus,&io_status,0x8c62040cu,4);if(io_status)return io_status;goto L8c6203da; /* mov.l 0x8c62040c,r0 */
L8c6203da: sa_motion_publish_stop(ctx,0x8c6203dau); r[0]+=16u;goto L8c6203dc; /* add #0x10,r0 */
L8c6203dc: sa_motion_publish_stop(ctx,0x8c6203dcu); fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203de; /* fmov.s @r0+,fr0 */
L8c6203de: sa_motion_publish_stop(ctx,0x8c6203deu); fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203e0; /* fmov.s @r0+,fr1 */
L8c6203e0: sa_motion_publish_stop(ctx,0x8c6203e0u); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203e2; /* fmov.s @r0+,fr2 */
L8c6203e2: sa_motion_publish_stop(ctx,0x8c6203e2u); fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c6203e4; /* fmov.s @r0,fr3 */
L8c6203e4: sa_motion_publish_stop(ctx,0x8c6203e4u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6203e6; /* frchg */
L8c6203e6: sa_motion_publish_stop(ctx,0x8c6203e6u); r[2]=rd(m,bus,&io_status,0x8c620424u,4);if(io_status)return io_status;goto L8c6203e8; /* mov.l 0x8c620424,r2 */
L8c6203e8: sa_motion_publish_stop(ctx,0x8c6203e8u); r[3]=rd(m,bus,&io_status,0x8c620420u,4);if(io_status)return io_status;goto L8c6203ea; /* mov.l 0x8c620420,r3 */
L8c6203ea: sa_motion_publish_stop(ctx,0x8c6203ecu); target=r[2];*pr=0x8c6203eeu;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6203ee; /* jsr @r2 */
L8c6203ee: sa_motion_publish_stop(ctx,0x8c6203eeu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6203f0; /* frchg */
L8c6203f0: sa_motion_publish_stop(ctx,0x8c6203f0u); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203f2; /* fmov.s @r15+,fr3 */
L8c6203f2: sa_motion_publish_stop(ctx,0x8c6203f2u); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203f4; /* fmov.s @r15+,fr2 */
L8c6203f4: sa_motion_publish_stop(ctx,0x8c6203f4u); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203f6; /* fmov.s @r15+,fr1 */
L8c6203f6: sa_motion_publish_stop(ctx,0x8c6203f6u); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203f8; /* fmov.s @r15+,fr0 */
L8c6203f8: sa_motion_publish_stop(ctx,0x8c6203f8u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6203fa; /* frchg */
L8c6203fa: sa_motion_publish_stop(ctx,0x8c6203fau); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6203fc; /* lds.l @r15+,PR */
L8c6203fc: sa_motion_publish_stop(ctx,0x8c6203fcu); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6203fe; /* mov.l @r15+,r9 */
L8c6203fe: sa_motion_publish_stop(ctx,0x8c6203feu); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620400; /* mov.l @r15+,r10 */
L8c620400: sa_motion_publish_stop(ctx,0x8c620400u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620402; /* mov.l @r15+,r11 */
L8c620402: sa_motion_publish_stop(ctx,0x8c620402u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620404; /* mov.l @r15+,r12 */
L8c620404: sa_motion_publish_stop(ctx,0x8c620404u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620406; /* mov.l @r15+,r13 */
L8c620406: sa_motion_publish_stop(ctx,0x8c620408u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_primitive_emit_clipped_8c620428(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c620428;
L8c620428: sa_motion_publish_stop(ctx,0x8c620428u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c62042a; /* mov.l r14,@-r15 */
L8c62042a: sa_motion_publish_stop(ctx,0x8c62042au); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c62042c; /* mov.l r13,@-r15 */
L8c62042c: sa_motion_publish_stop(ctx,0x8c62042cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c62042e; /* mov.l r12,@-r15 */
L8c62042e: sa_motion_publish_stop(ctx,0x8c62042eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c620430; /* mov.l r11,@-r15 */
L8c620430: sa_motion_publish_stop(ctx,0x8c620430u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c620432; /* mov.l r10,@-r15 */
L8c620432: sa_motion_publish_stop(ctx,0x8c620432u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c620434; /* mov.l r9,@-r15 */
L8c620434: sa_motion_publish_stop(ctx,0x8c620434u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c620436; /* sts.l PR,@-r15 */
L8c620436: sa_motion_publish_stop(ctx,0x8c620436u); r[15]+=4294967288u;goto L8c620438; /* add #-0x8,r15 */
L8c620438: sa_motion_publish_stop(ctx,0x8c620438u); r[0]=r[4];goto L8c62043a; /* mov r4,r0 */
L8c62043a: sa_motion_publish_stop(ctx,0x8c62043au); r[3]=rd(m,bus,&io_status,0x8c62048cu,4);if(io_status)return io_status;goto L8c62043c; /* mov.l 0x8c62048c,r3 */
L8c62043c: sa_motion_publish_stop(ctx,0x8c62043cu); wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c62043e; /* mov.l r4,@r15 */
L8c62043e: sa_motion_publish_stop(ctx,0x8c62043eu); r[11]=r[4];goto L8c620440; /* mov r4,r11 */
L8c620440: sa_motion_publish_stop(ctx,0x8c620440u); r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c620442; /* mov.w @r0,r0 */
L8c620442: sa_motion_publish_stop(ctx,0x8c620442u); r[0]=r[0]&65535u;goto L8c620444; /* extu.w r0,r0 */
L8c620444: sa_motion_publish_stop(ctx,0x8c620444u); r[0]&=r[3];goto L8c620446; /* and r3,r0 */
L8c620446: sa_motion_publish_stop(ctx,0x8c620446u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c620448; /* cmp/eq #0x0,r0 */
L8c620448: sa_motion_publish_stop(ctx,0x8c62044au); take=(*sr&1u);r[11]=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;if(take)goto L8c62046e;goto L8c62044c; /* bt/s 0x8c62046e */
L8c62044c: sa_motion_publish_stop(ctx,0x8c62044cu); r[1]=sx(m,bus,&io_status,0x8c62048au,2);if(io_status)return io_status;goto L8c62044e; /* mov.w 0x8c62048a,r1 */
L8c62044e: sa_motion_publish_stop(ctx,0x8c62044eu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620450; /* cmp/eq r1,r0 */
L8c620450: sa_motion_publish_stop(ctx,0x8c620450u); if(!(*sr&1u))goto L8c620456;goto L8c620452; /* bf 0x8c620456 */
L8c620452: sa_motion_publish_stop(ctx,0x8c620454u); goto L8c6205c4; /* bra 0x8c6205c4 */
L8c620456: sa_motion_publish_stop(ctx,0x8c620456u); r[1]=rd(m,bus,&io_status,0x8c620490u,4);if(io_status)return io_status;goto L8c620458; /* mov.l 0x8c620490,r1 */
L8c620458: sa_motion_publish_stop(ctx,0x8c620458u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c62045a; /* cmp/eq r1,r0 */
L8c62045a: sa_motion_publish_stop(ctx,0x8c62045au); if(!(*sr&1u))goto L8c620460;goto L8c62045c; /* bf 0x8c620460 */
L8c62045c: sa_motion_publish_stop(ctx,0x8c62045eu); goto L8c62077a; /* bra 0x8c62077a */
L8c620460: sa_motion_publish_stop(ctx,0x8c620460u); r[1]=rd(m,bus,&io_status,0x8c62048cu,4);if(io_status)return io_status;goto L8c620462; /* mov.l 0x8c62048c,r1 */
L8c620462: sa_motion_publish_stop(ctx,0x8c620462u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620464; /* cmp/eq r1,r0 */
L8c620464: sa_motion_publish_stop(ctx,0x8c620464u); if(!(*sr&1u))goto L8c62046a;goto L8c620466; /* bf 0x8c62046a */
L8c620466: sa_motion_publish_stop(ctx,0x8c620468u); goto L8c62095e; /* bra 0x8c62095e */
L8c62046a: sa_motion_publish_stop(ctx,0x8c62046cu); goto L8c620990; /* bra 0x8c620990 */
L8c62046e: sa_motion_publish_stop(ctx,0x8c62046eu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c620470; /* mov.l @r15,r2 */
L8c620470: sa_motion_publish_stop(ctx,0x8c620470u); r[1]=rd(m,bus,&io_status,0x8c620494u,4);if(io_status)return io_status;goto L8c620472; /* mov.l 0x8c620494,r1 */
L8c620472: sa_motion_publish_stop(ctx,0x8c620472u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c620474; /* mov.w @(0x2,r2),r0 */
L8c620474: sa_motion_publish_stop(ctx,0x8c620474u); r[0]=r[0]&65535u;goto L8c620476; /* extu.w r0,r0 */
L8c620476: sa_motion_publish_stop(ctx,0x8c620476u); r[2]=r[0];goto L8c620478; /* mov r0,r2 */
L8c620478: sa_motion_publish_stop(ctx,0x8c620478u); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c62047a; /* shll r0 */
L8c62047a: sa_motion_publish_stop(ctx,0x8c62047au); r[0]+=r[2];goto L8c62047c; /* add r2,r0 */
L8c62047c: sa_motion_publish_stop(ctx,0x8c62047cu); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c62047e; /* mov.l @r1,r2 */
L8c62047e: sa_motion_publish_stop(ctx,0x8c62047eu); r[2]+=r[0];goto L8c620480; /* add r0,r2 */
L8c620480: sa_motion_publish_stop(ctx,0x8c620480u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c620482; /* mov.l r2,@r1 */
L8c620482: sa_motion_publish_stop(ctx,0x8c620484u); r[14]=0u;goto L8c6205b2; /* bra 0x8c6205b2 */
L8c620498: sa_motion_publish_stop(ctx,0x8c620498u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c62049a; /* mov.w @r11+,r4 */
L8c62049a: sa_motion_publish_stop(ctx,0x8c62049au); r[3]=rd(m,bus,&io_status,0x8c620784u,4);if(io_status)return io_status;goto L8c62049c; /* mov.l 0x8c620784,r3 */
L8c62049c: sa_motion_publish_stop(ctx,0x8c62049cu); r[5]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c62049e; /* mov.w @r11+,r5 */
L8c62049e: sa_motion_publish_stop(ctx,0x8c62049eu); r[4]=r[4]&65535u;goto L8c6204a0; /* extu.w r4,r4 */
L8c6204a0: sa_motion_publish_stop(ctx,0x8c6204a0u); r[4]<<=2;goto L8c6204a2; /* shll2 r4 */
L8c6204a2: sa_motion_publish_stop(ctx,0x8c6204a2u); r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6204a4; /* mov.l @r3,r13 */
L8c6204a4: sa_motion_publish_stop(ctx,0x8c6204a4u); r[6]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6204a6; /* mov.w @r11+,r6 */
L8c6204a6: sa_motion_publish_stop(ctx,0x8c6204a6u); r[5]=r[5]&65535u;goto L8c6204a8; /* extu.w r5,r5 */
L8c6204a8: sa_motion_publish_stop(ctx,0x8c6204a8u); r[5]<<=2;goto L8c6204aa; /* shll2 r5 */
L8c6204aa: sa_motion_publish_stop(ctx,0x8c6204aau); r[6]=r[6]&65535u;goto L8c6204ac; /* extu.w r6,r6 */
L8c6204ac: sa_motion_publish_stop(ctx,0x8c6204acu); r[5]<<=2;goto L8c6204ae; /* shll2 r5 */
L8c6204ae: sa_motion_publish_stop(ctx,0x8c6204aeu); r[6]<<=2;goto L8c6204b0; /* shll2 r6 */
L8c6204b0: sa_motion_publish_stop(ctx,0x8c6204b0u); r[4]<<=2;goto L8c6204b2; /* shll2 r4 */
L8c6204b2: sa_motion_publish_stop(ctx,0x8c6204b2u); r[6]<<=2;goto L8c6204b4; /* shll2 r6 */
L8c6204b4: sa_motion_publish_stop(ctx,0x8c6204b4u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6204b6; /* shll r4 */
L8c6204b6: sa_motion_publish_stop(ctx,0x8c6204b6u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6204b8; /* shll r5 */
L8c6204b8: sa_motion_publish_stop(ctx,0x8c6204b8u); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c6204ba; /* shll r6 */
L8c6204ba: sa_motion_publish_stop(ctx,0x8c6204bau); r[4]+=r[13];goto L8c6204bc; /* add r13,r4 */
L8c6204bc: sa_motion_publish_stop(ctx,0x8c6204bcu); r[5]+=r[13];goto L8c6204be; /* add r13,r5 */
L8c6204be: sa_motion_publish_stop(ctx,0x8c6204beu); r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6204c0; /* mov.l @(0xc,r4),r7 */
L8c6204c0: sa_motion_publish_stop(ctx,0x8c6204c0u); r[2]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c6204c2; /* mov.l @(0xc,r5),r2 */
L8c6204c2: sa_motion_publish_stop(ctx,0x8c6204c2u); r[6]+=r[13];goto L8c6204c4; /* add r13,r6 */
L8c6204c4: sa_motion_publish_stop(ctx,0x8c6204c4u); r[1]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c6204c6; /* mov.l @(0xc,r6),r1 */
L8c6204c6: sa_motion_publish_stop(ctx,0x8c6204c6u); r[7]&=r[2];goto L8c6204c8; /* and r2,r7 */
L8c6204c8: sa_motion_publish_stop(ctx,0x8c6204c8u); r[2]=63u;goto L8c6204ca; /* mov #0x3f,r2 */
L8c6204ca: sa_motion_publish_stop(ctx,0x8c6204cau); r[7]&=r[1];goto L8c6204cc; /* and r1,r7 */
L8c6204cc: sa_motion_publish_stop(ctx,0x8c6204ccu); *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c6204ce; /* tst r2,r7 */
L8c6204ce: sa_motion_publish_stop(ctx,0x8c6204ceu); if(!(*sr&1u))goto L8c6205b0;goto L8c6204d0; /* bf 0x8c6205b0 */
L8c6204d0: sa_motion_publish_stop(ctx,0x8c6204d0u); r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6204d2; /* mov.l @(0xc,r4),r7 */
L8c6204d2: sa_motion_publish_stop(ctx,0x8c6204d2u); r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c6204d4; /* mov.l @(0xc,r5),r3 */
L8c6204d4: sa_motion_publish_stop(ctx,0x8c6204d4u); r[2]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c6204d6; /* mov.l @(0xc,r6),r2 */
L8c6204d6: sa_motion_publish_stop(ctx,0x8c6204d6u); r[7]|=r[3];goto L8c6204d8; /* or r3,r7 */
L8c6204d8: sa_motion_publish_stop(ctx,0x8c6204d8u); r[3]=1u;goto L8c6204da; /* mov #0x1,r3 */
L8c6204da: sa_motion_publish_stop(ctx,0x8c6204dau); r[7]|=r[2];goto L8c6204dc; /* or r2,r7 */
L8c6204dc: sa_motion_publish_stop(ctx,0x8c6204dcu); *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c6204de; /* tst r3,r7 */
L8c6204de: sa_motion_publish_stop(ctx,0x8c6204deu); if(!(*sr&1u))goto L8c6205a6;goto L8c6204e0; /* bf 0x8c6205a6 */
L8c6204e0: sa_motion_publish_stop(ctx,0x8c6204e0u); r[0]=16u;goto L8c6204e2; /* mov #0x10,r0 */
L8c6204e2: sa_motion_publish_stop(ctx,0x8c6204e2u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c6204e4; /* fmov.s @(r0,r5),fr4 */
L8c6204e4: sa_motion_publish_stop(ctx,0x8c6204e4u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6204e6; /* fmov.s @(r0,r4),fr3 */
L8c6204e6: sa_motion_publish_stop(ctx,0x8c6204e6u); r[0]=20u;goto L8c6204e8; /* mov #0x14,r0 */
L8c6204e8: sa_motion_publish_stop(ctx,0x8c6204e8u); fcopy(f,2u,4u);goto L8c6204ea; /* fmov fr4,fr2 */
L8c6204ea: sa_motion_publish_stop(ctx,0x8c6204eau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6204ec; /* fsub fr3,fr2 */
L8c6204ec: sa_motion_publish_stop(ctx,0x8c6204ecu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6204ee; /* fmov.s @(r0,r6),fr3 */
L8c6204ee: sa_motion_publish_stop(ctx,0x8c6204eeu); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6204f0; /* fmov.s @(r0,r5),fr5 */
L8c6204f0: sa_motion_publish_stop(ctx,0x8c6204f0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6204f2; /* fsub fr5,fr3 */
L8c6204f2: sa_motion_publish_stop(ctx,0x8c6204f2u); fcopy(f,1u,5u);goto L8c6204f4; /* fmov fr5,fr1 */
L8c6204f4: sa_motion_publish_stop(ctx,0x8c6204f4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6204f6; /* fmul fr3,fr2 */
L8c6204f6: sa_motion_publish_stop(ctx,0x8c6204f6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6204f8; /* fmov.s @(r0,r4),fr3 */
L8c6204f8: sa_motion_publish_stop(ctx,0x8c6204f8u); r[0]=16u;goto L8c6204fa; /* mov #0x10,r0 */
L8c6204fa: sa_motion_publish_stop(ctx,0x8c6204fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6204fc; /* fsub fr3,fr1 */
L8c6204fc: sa_motion_publish_stop(ctx,0x8c6204fcu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6204fe; /* fmov.s @(r0,r6),fr3 */
L8c6204fe: sa_motion_publish_stop(ctx,0x8c6204feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c620500; /* fsub fr4,fr3 */
L8c620500: sa_motion_publish_stop(ctx,0x8c620500u); fcopy(f,4u,2u);goto L8c620502; /* fmov fr2,fr4 */
L8c620502: sa_motion_publish_stop(ctx,0x8c620502u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c620504; /* fmul fr3,fr1 */
L8c620504: sa_motion_publish_stop(ctx,0x8c620504u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c620506; /* fldi0 fr3 */
L8c620506: sa_motion_publish_stop(ctx,0x8c620506u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c620508; /* fsub fr1,fr4 */
L8c620508: sa_motion_publish_stop(ctx,0x8c620508u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c62050a; /* fcmp/gt fr3,fr4 */
L8c62050a: sa_motion_publish_stop(ctx,0x8c62050au); if((*sr&1u))goto L8c6205b0;goto L8c62050c; /* bt 0x8c6205b0 */
L8c62050c: sa_motion_publish_stop(ctx,0x8c62050cu); r[3]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c62050e; /* mov.l 0x8c620788,r3 */
L8c62050e: sa_motion_publish_stop(ctx,0x8c62050eu); r[8]+=32u;goto L8c620510; /* add #0x20,r8 */
L8c620510: sa_motion_publish_stop(ctx,0x8c620510u); r[2]=rd(m,bus,&io_status,0x8c62078cu,4);if(io_status)return io_status;goto L8c620512; /* mov.l 0x8c62078c,r2 */
L8c620512: sa_motion_publish_stop(ctx,0x8c620512u); r[0]=24u;goto L8c620514; /* mov #0x18,r0 */
L8c620514: sa_motion_publish_stop(ctx,0x8c620514u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c620516; /* fmov.s @r3,fr3 */
L8c620516: sa_motion_publish_stop(ctx,0x8c620516u); r[3]+=4294967284u;goto L8c620518; /* add #-0xc,r3 */
L8c620518: sa_motion_publish_stop(ctx,0x8c620518u); r[1]=rd(m,bus,&io_status,0x8c620790u,4);if(io_status)return io_status;goto L8c62051a; /* mov.l 0x8c620790,r1 */
L8c62051a: sa_motion_publish_stop(ctx,0x8c62051au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62051c; /* fmov.s fr3,@-r8 */
L8c62051c: sa_motion_publish_stop(ctx,0x8c62051cu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c62051e; /* fmov.s @r2,fr3 */
L8c62051e: sa_motion_publish_stop(ctx,0x8c62051eu); r[2]=rd(m,bus,&io_status,0x8c620794u,4);if(io_status)return io_status;goto L8c620520; /* mov.l 0x8c620794,r2 */
L8c620520: sa_motion_publish_stop(ctx,0x8c620520u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620522; /* fmov.s fr3,@-r8 */
L8c620522: sa_motion_publish_stop(ctx,0x8c620522u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620524; /* fmov.s @r1+,fr3 */
L8c620524: sa_motion_publish_stop(ctx,0x8c620524u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620526; /* fmov.s fr3,@-r8 */
L8c620526: sa_motion_publish_stop(ctx,0x8c620526u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620528; /* fmov.s @r3+,fr3 */
L8c620528: sa_motion_publish_stop(ctx,0x8c620528u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62052a; /* fmov.s fr3,@-r8 */
L8c62052a: sa_motion_publish_stop(ctx,0x8c62052au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c62052c; /* fmov.s @(r0,r4),fr3 */
L8c62052c: sa_motion_publish_stop(ctx,0x8c62052cu); r[0]=20u;goto L8c62052e; /* mov #0x14,r0 */
L8c62052e: sa_motion_publish_stop(ctx,0x8c62052eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620530; /* fmov.s fr3,@-r8 */
L8c620530: sa_motion_publish_stop(ctx,0x8c620530u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620532; /* fmov.s @(r0,r4),fr3 */
L8c620532: sa_motion_publish_stop(ctx,0x8c620532u); r[0]=16u;goto L8c620534; /* mov #0x10,r0 */
L8c620534: sa_motion_publish_stop(ctx,0x8c620534u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620536; /* fmov.s fr3,@-r8 */
L8c620536: sa_motion_publish_stop(ctx,0x8c620536u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620538; /* fmov.s @(r0,r4),fr3 */
L8c620538: sa_motion_publish_stop(ctx,0x8c620538u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62053a; /* fmov.s fr3,@-r8 */
L8c62053a: sa_motion_publish_stop(ctx,0x8c62053au); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c62053c; /* mov.l r2,@-r8 */
L8c62053c: sa_motion_publish_stop(ctx,0x8c62053cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c62053e; /* pref @r8 */
L8c62053e: sa_motion_publish_stop(ctx,0x8c62053eu); r[2]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c620540; /* mov.l 0x8c620788,r2 */
L8c620540: sa_motion_publish_stop(ctx,0x8c620540u); r[8]+=64u;goto L8c620542; /* add #0x40,r8 */
L8c620542: sa_motion_publish_stop(ctx,0x8c620542u); r[0]=24u;goto L8c620544; /* mov #0x18,r0 */
L8c620544: sa_motion_publish_stop(ctx,0x8c620544u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c620546; /* fmov.s @r2,fr3 */
L8c620546: sa_motion_publish_stop(ctx,0x8c620546u); r[2]+=4294967284u;goto L8c620548; /* add #-0xc,r2 */
L8c620548: sa_motion_publish_stop(ctx,0x8c620548u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62054a; /* fmov.s fr3,@-r8 */
L8c62054a: sa_motion_publish_stop(ctx,0x8c62054au); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c62054c; /* fmov.s @r1,fr3 */
L8c62054c: sa_motion_publish_stop(ctx,0x8c62054cu); r[1]=rd(m,bus,&io_status,0x8c620794u,4);if(io_status)return io_status;goto L8c62054e; /* mov.l 0x8c620794,r1 */
L8c62054e: sa_motion_publish_stop(ctx,0x8c62054eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620550; /* fmov.s fr3,@-r8 */
L8c620550: sa_motion_publish_stop(ctx,0x8c620550u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620552; /* fmov.s @r3+,fr3 */
L8c620552: sa_motion_publish_stop(ctx,0x8c620552u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620554; /* fmov.s fr3,@-r8 */
L8c620554: sa_motion_publish_stop(ctx,0x8c620554u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620556; /* fmov.s @r2+,fr3 */
L8c620556: sa_motion_publish_stop(ctx,0x8c620556u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620558; /* fmov.s fr3,@-r8 */
L8c620558: sa_motion_publish_stop(ctx,0x8c620558u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c62055a; /* fmov.s @(r0,r5),fr3 */
L8c62055a: sa_motion_publish_stop(ctx,0x8c62055au); r[0]=20u;goto L8c62055c; /* mov #0x14,r0 */
L8c62055c: sa_motion_publish_stop(ctx,0x8c62055cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62055e; /* fmov.s fr3,@-r8 */
L8c62055e: sa_motion_publish_stop(ctx,0x8c62055eu); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620560; /* fmov.s @(r0,r5),fr3 */
L8c620560: sa_motion_publish_stop(ctx,0x8c620560u); r[0]=16u;goto L8c620562; /* mov #0x10,r0 */
L8c620562: sa_motion_publish_stop(ctx,0x8c620562u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620564; /* fmov.s fr3,@-r8 */
L8c620564: sa_motion_publish_stop(ctx,0x8c620564u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620566; /* fmov.s @(r0,r5),fr3 */
L8c620566: sa_motion_publish_stop(ctx,0x8c620566u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620568; /* fmov.s fr3,@-r8 */
L8c620568: sa_motion_publish_stop(ctx,0x8c620568u); r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c62056a; /* mov.l r1,@-r8 */
L8c62056a: sa_motion_publish_stop(ctx,0x8c62056au); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c62056c; /* pref @r8 */
L8c62056c: sa_motion_publish_stop(ctx,0x8c62056cu); r[1]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c62056e; /* mov.l 0x8c620788,r1 */
L8c62056e: sa_motion_publish_stop(ctx,0x8c62056eu); r[8]+=64u;goto L8c620570; /* add #0x40,r8 */
L8c620570: sa_motion_publish_stop(ctx,0x8c620570u); r[0]=24u;goto L8c620572; /* mov #0x18,r0 */
L8c620572: sa_motion_publish_stop(ctx,0x8c620572u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c620574; /* fmov.s @r1,fr3 */
L8c620574: sa_motion_publish_stop(ctx,0x8c620574u); r[1]+=4294967284u;goto L8c620576; /* add #-0xc,r1 */
L8c620576: sa_motion_publish_stop(ctx,0x8c620576u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620578; /* fmov.s fr3,@-r8 */
L8c620578: sa_motion_publish_stop(ctx,0x8c620578u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c62057a; /* fmov.s @r3,fr3 */
L8c62057a: sa_motion_publish_stop(ctx,0x8c62057au); r[3]=rd(m,bus,&io_status,0x8c620798u,4);if(io_status)return io_status;goto L8c62057c; /* mov.l 0x8c620798,r3 */
L8c62057c: sa_motion_publish_stop(ctx,0x8c62057cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62057e; /* fmov.s fr3,@-r8 */
L8c62057e: sa_motion_publish_stop(ctx,0x8c62057eu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c620580; /* fmov.s @r2,fr3 */
L8c620580: sa_motion_publish_stop(ctx,0x8c620580u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620582; /* fmov.s fr3,@-r8 */
L8c620582: sa_motion_publish_stop(ctx,0x8c620582u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c620584; /* fmov.s @r1,fr3 */
L8c620584: sa_motion_publish_stop(ctx,0x8c620584u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620586; /* fmov.s fr3,@-r8 */
L8c620586: sa_motion_publish_stop(ctx,0x8c620586u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620588; /* fmov.s @(r0,r6),fr3 */
L8c620588: sa_motion_publish_stop(ctx,0x8c620588u); r[0]=20u;goto L8c62058a; /* mov #0x14,r0 */
L8c62058a: sa_motion_publish_stop(ctx,0x8c62058au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62058c; /* fmov.s fr3,@-r8 */
L8c62058c: sa_motion_publish_stop(ctx,0x8c62058cu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c62058e; /* fmov.s @(r0,r6),fr3 */
L8c62058e: sa_motion_publish_stop(ctx,0x8c62058eu); r[0]=16u;goto L8c620590; /* mov #0x10,r0 */
L8c620590: sa_motion_publish_stop(ctx,0x8c620590u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620592; /* fmov.s fr3,@-r8 */
L8c620592: sa_motion_publish_stop(ctx,0x8c620592u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620594; /* fmov.s @(r0,r6),fr3 */
L8c620594: sa_motion_publish_stop(ctx,0x8c620594u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620596; /* fmov.s fr3,@-r8 */
L8c620596: sa_motion_publish_stop(ctx,0x8c620596u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c620598; /* mov.l r3,@-r8 */
L8c620598: sa_motion_publish_stop(ctx,0x8c620598u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c62059a; /* pref @r8 */
L8c62059a: sa_motion_publish_stop(ctx,0x8c62059au); r[0]=rd(m,bus,&io_status,0x8c62079cu,4);if(io_status)return io_status;goto L8c62059c; /* mov.l 0x8c62079c,r0 */
L8c62059c: sa_motion_publish_stop(ctx,0x8c62059cu); r[8]+=32u;goto L8c62059e; /* add #0x20,r8 */
L8c62059e: sa_motion_publish_stop(ctx,0x8c62059eu); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c6205a0; /* mov.l @r0,r3 */
L8c6205a0: sa_motion_publish_stop(ctx,0x8c6205a0u); r[3]+=3u;goto L8c6205a2; /* add #0x3,r3 */
L8c6205a2: sa_motion_publish_stop(ctx,0x8c6205a4u); wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c6205b0; /* bra 0x8c6205b0 */
L8c6205a6: sa_motion_publish_stop(ctx,0x8c6205a6u); r[1]=rd(m,bus,&io_status,0x8c6207a0u,4);if(io_status)return io_status;goto L8c6205a8; /* mov.l 0x8c6207a0,r1 */
L8c6205a8: sa_motion_publish_stop(ctx,0x8c6205a8u); wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c6205aa; /* mov.l r3,@r1 */
L8c6205aa: sa_motion_publish_stop(ctx,0x8c6205aau); r[3]=rd(m,bus,&io_status,0x8c6207a4u,4);if(io_status)return io_status;goto L8c6205ac; /* mov.l 0x8c6207a4,r3 */
L8c6205ac: sa_motion_publish_stop(ctx,0x8c6205aeu); target=r[3];*pr=0x8c6205b0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6205b0; /* jsr @r3 */
L8c6205b0: sa_motion_publish_stop(ctx,0x8c6205b0u); r[14]+=1u;goto L8c6205b2; /* add #0x1,r14 */
L8c6205b2: sa_motion_publish_stop(ctx,0x8c6205b2u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6205b4; /* mov.l @r15,r2 */
L8c6205b4: sa_motion_publish_stop(ctx,0x8c6205b4u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c6205b6; /* mov.w @(0x2,r2),r0 */
L8c6205b6: sa_motion_publish_stop(ctx,0x8c6205b6u); r[0]=r[0]&65535u;goto L8c6205b8; /* extu.w r0,r0 */
L8c6205b8: sa_motion_publish_stop(ctx,0x8c6205b8u); *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c6205ba; /* cmp/ge r0,r14 */
L8c6205ba: sa_motion_publish_stop(ctx,0x8c6205bau); if((*sr&1u))goto L8c6205c0;goto L8c6205bc; /* bt 0x8c6205c0 */
L8c6205bc: sa_motion_publish_stop(ctx,0x8c6205beu); goto L8c620498; /* bra 0x8c620498 */
L8c6205c0: sa_motion_publish_stop(ctx,0x8c6205c2u); goto L8c620990; /* bra 0x8c620990 */
L8c6205c4: sa_motion_publish_stop(ctx,0x8c6205c4u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6205c6; /* mov.l @r15,r1 */
L8c6205c6: sa_motion_publish_stop(ctx,0x8c6205c6u); r[2]=rd(m,bus,&io_status,0x8c6207a8u,4);if(io_status)return io_status;goto L8c6205c8; /* mov.l 0x8c6207a8,r2 */
L8c6205c8: sa_motion_publish_stop(ctx,0x8c6205c8u); r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c6205ca; /* mov.w @(0x2,r1),r0 */
L8c6205ca: sa_motion_publish_stop(ctx,0x8c6205cau); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6205cc; /* mov.l @r2,r1 */
L8c6205cc: sa_motion_publish_stop(ctx,0x8c6205ccu); r[0]=r[0]&65535u;goto L8c6205ce; /* extu.w r0,r0 */
L8c6205ce: sa_motion_publish_stop(ctx,0x8c6205ceu); r[0]<<=2;goto L8c6205d0; /* shll2 r0 */
L8c6205d0: sa_motion_publish_stop(ctx,0x8c6205d0u); r[1]+=r[0];goto L8c6205d2; /* add r0,r1 */
L8c6205d2: sa_motion_publish_stop(ctx,0x8c6205d2u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c6205d4; /* mov.l r1,@r2 */
L8c6205d4: sa_motion_publish_stop(ctx,0x8c6205d6u); r[10]=0u;goto L8c620768; /* bra 0x8c620768 */
L8c6205d8: sa_motion_publish_stop(ctx,0x8c6205d8u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6205da; /* mov.w @r11+,r4 */
L8c6205da: sa_motion_publish_stop(ctx,0x8c6205dau); r[3]=rd(m,bus,&io_status,0x8c620784u,4);if(io_status)return io_status;goto L8c6205dc; /* mov.l 0x8c620784,r3 */
L8c6205dc: sa_motion_publish_stop(ctx,0x8c6205dcu); r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6205de; /* mov.w @r11+,r13 */
L8c6205de: sa_motion_publish_stop(ctx,0x8c6205deu); r[4]=r[4]&65535u;goto L8c6205e0; /* extu.w r4,r4 */
L8c6205e0: sa_motion_publish_stop(ctx,0x8c6205e0u); r[4]<<=2;goto L8c6205e2; /* shll2 r4 */
L8c6205e2: sa_motion_publish_stop(ctx,0x8c6205e2u); r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6205e4; /* mov.l @r3,r6 */
L8c6205e4: sa_motion_publish_stop(ctx,0x8c6205e4u); r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6205e6; /* mov.w @r11+,r14 */
L8c6205e6: sa_motion_publish_stop(ctx,0x8c6205e6u); r[13]=r[13]&65535u;goto L8c6205e8; /* extu.w r13,r13 */
L8c6205e8: sa_motion_publish_stop(ctx,0x8c6205e8u); r[13]<<=2;goto L8c6205ea; /* shll2 r13 */
L8c6205ea: sa_motion_publish_stop(ctx,0x8c6205eau); r[12]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6205ec; /* mov.w @r11+,r12 */
L8c6205ec: sa_motion_publish_stop(ctx,0x8c6205ecu); r[14]=r[14]&65535u;goto L8c6205ee; /* extu.w r14,r14 */
L8c6205ee: sa_motion_publish_stop(ctx,0x8c6205eeu); r[13]<<=2;goto L8c6205f0; /* shll2 r13 */
L8c6205f0: sa_motion_publish_stop(ctx,0x8c6205f0u); r[4]<<=2;goto L8c6205f2; /* shll2 r4 */
L8c6205f2: sa_motion_publish_stop(ctx,0x8c6205f2u); r[12]=r[12]&65535u;goto L8c6205f4; /* extu.w r12,r12 */
L8c6205f4: sa_motion_publish_stop(ctx,0x8c6205f4u); r[14]<<=2;goto L8c6205f6; /* shll2 r14 */
L8c6205f6: sa_motion_publish_stop(ctx,0x8c6205f6u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6205f8; /* shll r4 */
L8c6205f8: sa_motion_publish_stop(ctx,0x8c6205f8u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c6205fa; /* shll r13 */
L8c6205fa: sa_motion_publish_stop(ctx,0x8c6205fau); r[4]+=r[6];goto L8c6205fc; /* add r6,r4 */
L8c6205fc: sa_motion_publish_stop(ctx,0x8c6205fcu); r[12]<<=2;goto L8c6205fe; /* shll2 r12 */
L8c6205fe: sa_motion_publish_stop(ctx,0x8c6205feu); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c620600; /* mov.l @(0xc,r4),r5 */
L8c620600: sa_motion_publish_stop(ctx,0x8c620600u); r[13]+=r[6];goto L8c620602; /* add r6,r13 */
L8c620602: sa_motion_publish_stop(ctx,0x8c620602u); r[14]<<=2;goto L8c620604; /* shll2 r14 */
L8c620604: sa_motion_publish_stop(ctx,0x8c620604u); r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c620606; /* mov.l @(0xc,r13),r2 */
L8c620606: sa_motion_publish_stop(ctx,0x8c620606u); r[12]<<=2;goto L8c620608; /* shll2 r12 */
L8c620608: sa_motion_publish_stop(ctx,0x8c620608u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c62060a; /* shll r14 */
L8c62060a: sa_motion_publish_stop(ctx,0x8c62060au); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c62060c; /* shll r12 */
L8c62060c: sa_motion_publish_stop(ctx,0x8c62060cu); r[14]+=r[6];goto L8c62060e; /* add r6,r14 */
L8c62060e: sa_motion_publish_stop(ctx,0x8c62060eu); r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c620610; /* mov.l @(0xc,r14),r1 */
L8c620610: sa_motion_publish_stop(ctx,0x8c620610u); r[5]&=r[2];goto L8c620612; /* and r2,r5 */
L8c620612: sa_motion_publish_stop(ctx,0x8c620612u); r[12]+=r[6];goto L8c620614; /* add r6,r12 */
L8c620614: sa_motion_publish_stop(ctx,0x8c620614u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c620616; /* mov.l @(0xc,r12),r2 */
L8c620616: sa_motion_publish_stop(ctx,0x8c620616u); r[5]&=r[1];goto L8c620618; /* and r1,r5 */
L8c620618: sa_motion_publish_stop(ctx,0x8c620618u); r[1]=63u;goto L8c62061a; /* mov #0x3f,r1 */
L8c62061a: sa_motion_publish_stop(ctx,0x8c62061au); r[5]&=r[2];goto L8c62061c; /* and r2,r5 */
L8c62061c: sa_motion_publish_stop(ctx,0x8c62061cu); *sr=(*sr&~1u)|!(r[1]&r[5]);goto L8c62061e; /* tst r1,r5 */
L8c62061e: sa_motion_publish_stop(ctx,0x8c62061eu); if((*sr&1u))goto L8c620624;goto L8c620620; /* bt 0x8c620624 */
L8c620620: sa_motion_publish_stop(ctx,0x8c620622u); goto L8c620766; /* bra 0x8c620766 */
L8c620624: sa_motion_publish_stop(ctx,0x8c620624u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c620626; /* mov.l @(0xc,r4),r5 */
L8c620626: sa_motion_publish_stop(ctx,0x8c620626u); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c620628; /* mov.l @(0xc,r13),r3 */
L8c620628: sa_motion_publish_stop(ctx,0x8c620628u); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c62062a; /* mov.l @(0xc,r14),r2 */
L8c62062a: sa_motion_publish_stop(ctx,0x8c62062au); r[5]|=r[3];goto L8c62062c; /* or r3,r5 */
L8c62062c: sa_motion_publish_stop(ctx,0x8c62062cu); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c62062e; /* mov.l @(0xc,r12),r3 */
L8c62062e: sa_motion_publish_stop(ctx,0x8c62062eu); r[5]|=r[2];goto L8c620630; /* or r2,r5 */
L8c620630: sa_motion_publish_stop(ctx,0x8c620630u); r[2]=1u;goto L8c620632; /* mov #0x1,r2 */
L8c620632: sa_motion_publish_stop(ctx,0x8c620632u); r[5]|=r[3];goto L8c620634; /* or r3,r5 */
L8c620634: sa_motion_publish_stop(ctx,0x8c620634u); *sr=(*sr&~1u)|!(r[2]&r[5]);goto L8c620636; /* tst r2,r5 */
L8c620636: sa_motion_publish_stop(ctx,0x8c620636u); if(!(*sr&1u))goto L8c620730;goto L8c620638; /* bf 0x8c620730 */
L8c620638: sa_motion_publish_stop(ctx,0x8c620638u); r[0]=16u;goto L8c62063a; /* mov #0x10,r0 */
L8c62063a: sa_motion_publish_stop(ctx,0x8c62063au); fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c62063c; /* fmov.s @(r0,r13),fr4 */
L8c62063c: sa_motion_publish_stop(ctx,0x8c62063cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c62063e; /* fmov.s @(r0,r4),fr3 */
L8c62063e: sa_motion_publish_stop(ctx,0x8c62063eu); r[0]=20u;goto L8c620640; /* mov #0x14,r0 */
L8c620640: sa_motion_publish_stop(ctx,0x8c620640u); fcopy(f,2u,4u);goto L8c620642; /* fmov fr4,fr2 */
L8c620642: sa_motion_publish_stop(ctx,0x8c620642u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c620644; /* fsub fr3,fr2 */
L8c620644: sa_motion_publish_stop(ctx,0x8c620644u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c620646; /* fmov.s @(r0,r14),fr3 */
L8c620646: sa_motion_publish_stop(ctx,0x8c620646u); fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c620648; /* fmov.s @(r0,r13),fr5 */
L8c620648: sa_motion_publish_stop(ctx,0x8c620648u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c62064a; /* fsub fr5,fr3 */
L8c62064a: sa_motion_publish_stop(ctx,0x8c62064au); fcopy(f,1u,5u);goto L8c62064c; /* fmov fr5,fr1 */
L8c62064c: sa_motion_publish_stop(ctx,0x8c62064cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c62064e; /* fmul fr3,fr2 */
L8c62064e: sa_motion_publish_stop(ctx,0x8c62064eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620650; /* fmov.s @(r0,r4),fr3 */
L8c620650: sa_motion_publish_stop(ctx,0x8c620650u); r[0]=16u;goto L8c620652; /* mov #0x10,r0 */
L8c620652: sa_motion_publish_stop(ctx,0x8c620652u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c620654; /* fsub fr3,fr1 */
L8c620654: sa_motion_publish_stop(ctx,0x8c620654u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c620656; /* fmov.s @(r0,r14),fr3 */
L8c620656: sa_motion_publish_stop(ctx,0x8c620656u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c620658; /* fsub fr4,fr3 */
L8c620658: sa_motion_publish_stop(ctx,0x8c620658u); fcopy(f,4u,2u);goto L8c62065a; /* fmov fr2,fr4 */
L8c62065a: sa_motion_publish_stop(ctx,0x8c62065au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c62065c; /* fmul fr3,fr1 */
L8c62065c: sa_motion_publish_stop(ctx,0x8c62065cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c62065e; /* fldi0 fr3 */
L8c62065e: sa_motion_publish_stop(ctx,0x8c62065eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c620660; /* fsub fr1,fr4 */
L8c620660: sa_motion_publish_stop(ctx,0x8c620660u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c620662; /* fcmp/gt fr3,fr4 */
L8c620662: sa_motion_publish_stop(ctx,0x8c620662u); if(!(*sr&1u))goto L8c620668;goto L8c620664; /* bf 0x8c620668 */
L8c620664: sa_motion_publish_stop(ctx,0x8c620666u); goto L8c620766; /* bra 0x8c620766 */
L8c620668: sa_motion_publish_stop(ctx,0x8c620668u); r[3]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c62066a; /* mov.l 0x8c620788,r3 */
L8c62066a: sa_motion_publish_stop(ctx,0x8c62066au); r[8]+=32u;goto L8c62066c; /* add #0x20,r8 */
L8c62066c: sa_motion_publish_stop(ctx,0x8c62066cu); r[2]=rd(m,bus,&io_status,0x8c62078cu,4);if(io_status)return io_status;goto L8c62066e; /* mov.l 0x8c62078c,r2 */
L8c62066e: sa_motion_publish_stop(ctx,0x8c62066eu); r[0]=24u;goto L8c620670; /* mov #0x18,r0 */
L8c620670: sa_motion_publish_stop(ctx,0x8c620670u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c620672; /* fmov.s @r3,fr3 */
L8c620672: sa_motion_publish_stop(ctx,0x8c620672u); r[3]+=4294967284u;goto L8c620674; /* add #-0xc,r3 */
L8c620674: sa_motion_publish_stop(ctx,0x8c620674u); r[1]=rd(m,bus,&io_status,0x8c620790u,4);if(io_status)return io_status;goto L8c620676; /* mov.l 0x8c620790,r1 */
L8c620676: sa_motion_publish_stop(ctx,0x8c620676u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620678; /* fmov.s fr3,@-r8 */
L8c620678: sa_motion_publish_stop(ctx,0x8c620678u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c62067a; /* fmov.s @r2,fr3 */
L8c62067a: sa_motion_publish_stop(ctx,0x8c62067au); r[2]=rd(m,bus,&io_status,0x8c620794u,4);if(io_status)return io_status;goto L8c62067c; /* mov.l 0x8c620794,r2 */
L8c62067c: sa_motion_publish_stop(ctx,0x8c62067cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62067e; /* fmov.s fr3,@-r8 */
L8c62067e: sa_motion_publish_stop(ctx,0x8c62067eu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620680; /* fmov.s @r1+,fr3 */
L8c620680: sa_motion_publish_stop(ctx,0x8c620680u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620682; /* fmov.s fr3,@-r8 */
L8c620682: sa_motion_publish_stop(ctx,0x8c620682u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620684; /* fmov.s @r3+,fr3 */
L8c620684: sa_motion_publish_stop(ctx,0x8c620684u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620686; /* fmov.s fr3,@-r8 */
L8c620686: sa_motion_publish_stop(ctx,0x8c620686u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620688; /* fmov.s @(r0,r4),fr3 */
L8c620688: sa_motion_publish_stop(ctx,0x8c620688u); r[0]=20u;goto L8c62068a; /* mov #0x14,r0 */
L8c62068a: sa_motion_publish_stop(ctx,0x8c62068au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62068c; /* fmov.s fr3,@-r8 */
L8c62068c: sa_motion_publish_stop(ctx,0x8c62068cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c62068e; /* fmov.s @(r0,r4),fr3 */
L8c62068e: sa_motion_publish_stop(ctx,0x8c62068eu); r[0]=16u;goto L8c620690; /* mov #0x10,r0 */
L8c620690: sa_motion_publish_stop(ctx,0x8c620690u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620692; /* fmov.s fr3,@-r8 */
L8c620692: sa_motion_publish_stop(ctx,0x8c620692u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620694; /* fmov.s @(r0,r4),fr3 */
L8c620694: sa_motion_publish_stop(ctx,0x8c620694u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620696; /* fmov.s fr3,@-r8 */
L8c620696: sa_motion_publish_stop(ctx,0x8c620696u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c620698; /* mov.l r2,@-r8 */
L8c620698: sa_motion_publish_stop(ctx,0x8c620698u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c62069a; /* pref @r8 */
L8c62069a: sa_motion_publish_stop(ctx,0x8c62069au); r[2]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c62069c; /* mov.l 0x8c620788,r2 */
L8c62069c: sa_motion_publish_stop(ctx,0x8c62069cu); r[8]+=64u;goto L8c62069e; /* add #0x40,r8 */
L8c62069e: sa_motion_publish_stop(ctx,0x8c62069eu); r[0]=24u;goto L8c6206a0; /* mov #0x18,r0 */
L8c6206a0: sa_motion_publish_stop(ctx,0x8c6206a0u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6206a2; /* fmov.s @r2,fr3 */
L8c6206a2: sa_motion_publish_stop(ctx,0x8c6206a2u); r[2]+=4294967284u;goto L8c6206a4; /* add #-0xc,r2 */
L8c6206a4: sa_motion_publish_stop(ctx,0x8c6206a4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206a6; /* fmov.s fr3,@-r8 */
L8c6206a6: sa_motion_publish_stop(ctx,0x8c6206a6u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6206a8; /* fmov.s @r1,fr3 */
L8c6206a8: sa_motion_publish_stop(ctx,0x8c6206a8u); r[1]=rd(m,bus,&io_status,0x8c620794u,4);if(io_status)return io_status;goto L8c6206aa; /* mov.l 0x8c620794,r1 */
L8c6206aa: sa_motion_publish_stop(ctx,0x8c6206aau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206ac; /* fmov.s fr3,@-r8 */
L8c6206ac: sa_motion_publish_stop(ctx,0x8c6206acu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6206ae; /* fmov.s @r3+,fr3 */
L8c6206ae: sa_motion_publish_stop(ctx,0x8c6206aeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206b0; /* fmov.s fr3,@-r8 */
L8c6206b0: sa_motion_publish_stop(ctx,0x8c6206b0u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6206b2; /* fmov.s @r2+,fr3 */
L8c6206b2: sa_motion_publish_stop(ctx,0x8c6206b2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206b4; /* fmov.s fr3,@-r8 */
L8c6206b4: sa_motion_publish_stop(ctx,0x8c6206b4u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6206b6; /* fmov.s @(r0,r13),fr3 */
L8c6206b6: sa_motion_publish_stop(ctx,0x8c6206b6u); r[0]=20u;goto L8c6206b8; /* mov #0x14,r0 */
L8c6206b8: sa_motion_publish_stop(ctx,0x8c6206b8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206ba; /* fmov.s fr3,@-r8 */
L8c6206ba: sa_motion_publish_stop(ctx,0x8c6206bau); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6206bc; /* fmov.s @(r0,r13),fr3 */
L8c6206bc: sa_motion_publish_stop(ctx,0x8c6206bcu); r[0]=16u;goto L8c6206be; /* mov #0x10,r0 */
L8c6206be: sa_motion_publish_stop(ctx,0x8c6206beu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206c0; /* fmov.s fr3,@-r8 */
L8c6206c0: sa_motion_publish_stop(ctx,0x8c6206c0u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6206c2; /* fmov.s @(r0,r13),fr3 */
L8c6206c2: sa_motion_publish_stop(ctx,0x8c6206c2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206c4; /* fmov.s fr3,@-r8 */
L8c6206c4: sa_motion_publish_stop(ctx,0x8c6206c4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c6206c6; /* mov.l r1,@-r8 */
L8c6206c6: sa_motion_publish_stop(ctx,0x8c6206c6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6206c8; /* pref @r8 */
L8c6206c8: sa_motion_publish_stop(ctx,0x8c6206c8u); r[1]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c6206ca; /* mov.l 0x8c620788,r1 */
L8c6206ca: sa_motion_publish_stop(ctx,0x8c6206cau); r[8]+=64u;goto L8c6206cc; /* add #0x40,r8 */
L8c6206cc: sa_motion_publish_stop(ctx,0x8c6206ccu); r[0]=24u;goto L8c6206ce; /* mov #0x18,r0 */
L8c6206ce: sa_motion_publish_stop(ctx,0x8c6206ceu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6206d0; /* fmov.s @r1,fr3 */
L8c6206d0: sa_motion_publish_stop(ctx,0x8c6206d0u); r[1]+=4294967284u;goto L8c6206d2; /* add #-0xc,r1 */
L8c6206d2: sa_motion_publish_stop(ctx,0x8c6206d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206d4; /* fmov.s fr3,@-r8 */
L8c6206d4: sa_motion_publish_stop(ctx,0x8c6206d4u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6206d6; /* fmov.s @r3,fr3 */
L8c6206d6: sa_motion_publish_stop(ctx,0x8c6206d6u); r[3]=rd(m,bus,&io_status,0x8c620794u,4);if(io_status)return io_status;goto L8c6206d8; /* mov.l 0x8c620794,r3 */
L8c6206d8: sa_motion_publish_stop(ctx,0x8c6206d8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206da; /* fmov.s fr3,@-r8 */
L8c6206da: sa_motion_publish_stop(ctx,0x8c6206dau); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6206dc; /* fmov.s @r2+,fr3 */
L8c6206dc: sa_motion_publish_stop(ctx,0x8c6206dcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206de; /* fmov.s fr3,@-r8 */
L8c6206de: sa_motion_publish_stop(ctx,0x8c6206deu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6206e0; /* fmov.s @r1+,fr3 */
L8c6206e0: sa_motion_publish_stop(ctx,0x8c6206e0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206e2; /* fmov.s fr3,@-r8 */
L8c6206e2: sa_motion_publish_stop(ctx,0x8c6206e2u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6206e4; /* fmov.s @(r0,r14),fr3 */
L8c6206e4: sa_motion_publish_stop(ctx,0x8c6206e4u); r[0]=20u;goto L8c6206e6; /* mov #0x14,r0 */
L8c6206e6: sa_motion_publish_stop(ctx,0x8c6206e6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206e8; /* fmov.s fr3,@-r8 */
L8c6206e8: sa_motion_publish_stop(ctx,0x8c6206e8u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6206ea; /* fmov.s @(r0,r14),fr3 */
L8c6206ea: sa_motion_publish_stop(ctx,0x8c6206eau); r[0]=16u;goto L8c6206ec; /* mov #0x10,r0 */
L8c6206ec: sa_motion_publish_stop(ctx,0x8c6206ecu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206ee; /* fmov.s fr3,@-r8 */
L8c6206ee: sa_motion_publish_stop(ctx,0x8c6206eeu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6206f0; /* fmov.s @(r0,r14),fr3 */
L8c6206f0: sa_motion_publish_stop(ctx,0x8c6206f0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206f2; /* fmov.s fr3,@-r8 */
L8c6206f2: sa_motion_publish_stop(ctx,0x8c6206f2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6206f4; /* mov.l r3,@-r8 */
L8c6206f4: sa_motion_publish_stop(ctx,0x8c6206f4u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6206f6; /* pref @r8 */
L8c6206f6: sa_motion_publish_stop(ctx,0x8c6206f6u); r[3]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c6206f8; /* mov.l 0x8c620788,r3 */
L8c6206f8: sa_motion_publish_stop(ctx,0x8c6206f8u); r[8]+=64u;goto L8c6206fa; /* add #0x40,r8 */
L8c6206fa: sa_motion_publish_stop(ctx,0x8c6206fau); r[0]=24u;goto L8c6206fc; /* mov #0x18,r0 */
L8c6206fc: sa_motion_publish_stop(ctx,0x8c6206fcu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6206fe; /* fmov.s @r3,fr3 */
L8c6206fe: sa_motion_publish_stop(ctx,0x8c6206feu); r[3]+=4294967284u;goto L8c620700; /* add #-0xc,r3 */
L8c620700: sa_motion_publish_stop(ctx,0x8c620700u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620702; /* fmov.s fr3,@-r8 */
L8c620702: sa_motion_publish_stop(ctx,0x8c620702u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c620704; /* fmov.s @r2,fr3 */
L8c620704: sa_motion_publish_stop(ctx,0x8c620704u); r[2]=rd(m,bus,&io_status,0x8c620798u,4);if(io_status)return io_status;goto L8c620706; /* mov.l 0x8c620798,r2 */
L8c620706: sa_motion_publish_stop(ctx,0x8c620706u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620708; /* fmov.s fr3,@-r8 */
L8c620708: sa_motion_publish_stop(ctx,0x8c620708u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c62070a; /* fmov.s @r1,fr3 */
L8c62070a: sa_motion_publish_stop(ctx,0x8c62070au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62070c; /* fmov.s fr3,@-r8 */
L8c62070c: sa_motion_publish_stop(ctx,0x8c62070cu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c62070e; /* fmov.s @r3,fr3 */
L8c62070e: sa_motion_publish_stop(ctx,0x8c62070eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620710; /* fmov.s fr3,@-r8 */
L8c620710: sa_motion_publish_stop(ctx,0x8c620710u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c620712; /* fmov.s @(r0,r12),fr3 */
L8c620712: sa_motion_publish_stop(ctx,0x8c620712u); r[0]=20u;goto L8c620714; /* mov #0x14,r0 */
L8c620714: sa_motion_publish_stop(ctx,0x8c620714u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620716; /* fmov.s fr3,@-r8 */
L8c620716: sa_motion_publish_stop(ctx,0x8c620716u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c620718; /* fmov.s @(r0,r12),fr3 */
L8c620718: sa_motion_publish_stop(ctx,0x8c620718u); r[0]=16u;goto L8c62071a; /* mov #0x10,r0 */
L8c62071a: sa_motion_publish_stop(ctx,0x8c62071au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62071c; /* fmov.s fr3,@-r8 */
L8c62071c: sa_motion_publish_stop(ctx,0x8c62071cu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c62071e; /* fmov.s @(r0,r12),fr3 */
L8c62071e: sa_motion_publish_stop(ctx,0x8c62071eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620720; /* fmov.s fr3,@-r8 */
L8c620720: sa_motion_publish_stop(ctx,0x8c620720u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c620722; /* mov.l r2,@-r8 */
L8c620722: sa_motion_publish_stop(ctx,0x8c620722u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c620724; /* pref @r8 */
L8c620724: sa_motion_publish_stop(ctx,0x8c620724u); r[0]=rd(m,bus,&io_status,0x8c62079cu,4);if(io_status)return io_status;goto L8c620726; /* mov.l 0x8c62079c,r0 */
L8c620726: sa_motion_publish_stop(ctx,0x8c620726u); r[8]+=32u;goto L8c620728; /* add #0x20,r8 */
L8c620728: sa_motion_publish_stop(ctx,0x8c620728u); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c62072a; /* mov.l @r0,r2 */
L8c62072a: sa_motion_publish_stop(ctx,0x8c62072au); r[2]+=4u;goto L8c62072c; /* add #0x4,r2 */
L8c62072c: sa_motion_publish_stop(ctx,0x8c62072eu); wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c620766; /* bra 0x8c620766 */
L8c620730: sa_motion_publish_stop(ctx,0x8c620730u); r[3]=rd(m,bus,&io_status,0x8c6207a0u,4);if(io_status)return io_status;goto L8c620732; /* mov.l 0x8c6207a0,r3 */
L8c620732: sa_motion_publish_stop(ctx,0x8c620732u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c620734; /* mov.l r2,@r3 */
L8c620734: sa_motion_publish_stop(ctx,0x8c620734u); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c620736; /* mov.l @(0xc,r4),r5 */
L8c620736: sa_motion_publish_stop(ctx,0x8c620736u); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c620738; /* mov.l @(0xc,r13),r3 */
L8c620738: sa_motion_publish_stop(ctx,0x8c620738u); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c62073a; /* mov.l @(0xc,r14),r2 */
L8c62073a: sa_motion_publish_stop(ctx,0x8c62073au); r[5]&=r[3];goto L8c62073c; /* and r3,r5 */
L8c62073c: sa_motion_publish_stop(ctx,0x8c62073cu); r[3]=1u;goto L8c62073e; /* mov #0x1,r3 */
L8c62073e: sa_motion_publish_stop(ctx,0x8c62073eu); r[5]&=r[2];goto L8c620740; /* and r2,r5 */
L8c620740: sa_motion_publish_stop(ctx,0x8c620740u); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c620742; /* tst r3,r5 */
L8c620742: sa_motion_publish_stop(ctx,0x8c620742u); if(!(*sr&1u))goto L8c62074c;goto L8c620744; /* bf 0x8c62074c */
L8c620744: sa_motion_publish_stop(ctx,0x8c620744u); r[1]=rd(m,bus,&io_status,0x8c6207a4u,4);if(io_status)return io_status;goto L8c620746; /* mov.l 0x8c6207a4,r1 */
L8c620746: sa_motion_publish_stop(ctx,0x8c620746u); r[6]=r[14];goto L8c620748; /* mov r14,r6 */
L8c620748: sa_motion_publish_stop(ctx,0x8c62074au); target=r[1];*pr=0x8c62074cu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62074c; /* jsr @r1 */
L8c62074c: sa_motion_publish_stop(ctx,0x8c62074cu); r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c62074e; /* mov.l @(0xc,r13),r4 */
L8c62074e: sa_motion_publish_stop(ctx,0x8c62074eu); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c620750; /* mov.l @(0xc,r14),r3 */
L8c620750: sa_motion_publish_stop(ctx,0x8c620750u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c620752; /* mov.l @(0xc,r12),r2 */
L8c620752: sa_motion_publish_stop(ctx,0x8c620752u); r[4]&=r[3];goto L8c620754; /* and r3,r4 */
L8c620754: sa_motion_publish_stop(ctx,0x8c620754u); r[3]=1u;goto L8c620756; /* mov #0x1,r3 */
L8c620756: sa_motion_publish_stop(ctx,0x8c620756u); r[4]&=r[2];goto L8c620758; /* and r2,r4 */
L8c620758: sa_motion_publish_stop(ctx,0x8c620758u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c62075a; /* tst r3,r4 */
L8c62075a: sa_motion_publish_stop(ctx,0x8c62075au); if(!(*sr&1u))goto L8c620766;goto L8c62075c; /* bf 0x8c620766 */
L8c62075c: sa_motion_publish_stop(ctx,0x8c62075cu); r[1]=rd(m,bus,&io_status,0x8c6207a4u,4);if(io_status)return io_status;goto L8c62075e; /* mov.l 0x8c6207a4,r1 */
L8c62075e: sa_motion_publish_stop(ctx,0x8c62075eu); r[5]=r[14];goto L8c620760; /* mov r14,r5 */
L8c620760: sa_motion_publish_stop(ctx,0x8c620760u); r[6]=r[12];goto L8c620762; /* mov r12,r6 */
L8c620762: sa_motion_publish_stop(ctx,0x8c620764u); target=r[1];*pr=0x8c620766u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620766; /* jsr @r1 */
L8c620766: sa_motion_publish_stop(ctx,0x8c620766u); r[10]+=1u;goto L8c620768; /* add #0x1,r10 */
L8c620768: sa_motion_publish_stop(ctx,0x8c620768u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c62076a; /* mov.l @r15,r3 */
L8c62076a: sa_motion_publish_stop(ctx,0x8c62076au); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c62076c; /* mov.w @(0x2,r3),r0 */
L8c62076c: sa_motion_publish_stop(ctx,0x8c62076cu); r[0]=r[0]&65535u;goto L8c62076e; /* extu.w r0,r0 */
L8c62076e: sa_motion_publish_stop(ctx,0x8c62076eu); *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c620770; /* cmp/ge r0,r10 */
L8c620770: sa_motion_publish_stop(ctx,0x8c620770u); if((*sr&1u))goto L8c620776;goto L8c620772; /* bt 0x8c620776 */
L8c620772: sa_motion_publish_stop(ctx,0x8c620774u); goto L8c6205d8; /* bra 0x8c6205d8 */
L8c620776: sa_motion_publish_stop(ctx,0x8c620778u); goto L8c620990; /* bra 0x8c620990 */
L8c62077a: sa_motion_publish_stop(ctx,0x8c62077cu); r[9]=0u;goto L8c62094c; /* bra 0x8c62094c */
L8c6207ac: sa_motion_publish_stop(ctx,0x8c6207acu); r[3]=rd(m,bus,&io_status,0x8c6209ecu,4);if(io_status)return io_status;goto L8c6207ae; /* mov.l 0x8c6209ec,r3 */
L8c6207ae: sa_motion_publish_stop(ctx,0x8c6207aeu); r[7]=0u;goto L8c6207b0; /* mov #0x0,r7 */
L8c6207b0: sa_motion_publish_stop(ctx,0x8c6207b0u); r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6207b2; /* mov.w @r11+,r13 */
L8c6207b2: sa_motion_publish_stop(ctx,0x8c6207b2u); r[4]=0u;goto L8c6207b4; /* mov #0x0,r4 */
L8c6207b4: sa_motion_publish_stop(ctx,0x8c6207b4u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6207b6; /* mov.l @r3,r2 */
L8c6207b6: sa_motion_publish_stop(ctx,0x8c6207b6u); r[5]=r[7];goto L8c6207b8; /* mov r7,r5 */
L8c6207b8: sa_motion_publish_stop(ctx,0x8c6207b8u); r[13]=r[13]&65535u;goto L8c6207ba; /* extu.w r13,r13 */
L8c6207ba: sa_motion_publish_stop(ctx,0x8c6207bau); r[2]+=r[13];goto L8c6207bc; /* add r13,r2 */
L8c6207bc: sa_motion_publish_stop(ctx,0x8c6207bcu); *sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c6207be; /* cmp/pl r13 */
L8c6207be: sa_motion_publish_stop(ctx,0x8c6207beu); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6207c0; /* mov.l r2,@r3 */
L8c6207c0: sa_motion_publish_stop(ctx,0x8c6207c0u); r[6]=4294967295u;goto L8c6207c2; /* mov #-0x1,r6 */
L8c6207c2: sa_motion_publish_stop(ctx,0x8c6207c4u); take=!(*sr&1u);r[4]+=r[11];if(take)goto L8c6207e4;goto L8c6207c6; /* bf/s 0x8c6207e4 */
L8c6207c6: sa_motion_publish_stop(ctx,0x8c6207c6u); r[14]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c6207c8; /* mov.w @r4+,r14 */
L8c6207c8: sa_motion_publish_stop(ctx,0x8c6207c8u); r[5]+=1u;goto L8c6207ca; /* add #0x1,r5 */
L8c6207ca: sa_motion_publish_stop(ctx,0x8c6207cau); r[2]=rd(m,bus,&io_status,0x8c6209f0u,4);if(io_status)return io_status;goto L8c6207cc; /* mov.l 0x8c6209f0,r2 */
L8c6207cc: sa_motion_publish_stop(ctx,0x8c6207ccu); r[14]=r[14]&65535u;goto L8c6207ce; /* extu.w r14,r14 */
L8c6207ce: sa_motion_publish_stop(ctx,0x8c6207ceu); r[14]<<=2;goto L8c6207d0; /* shll2 r14 */
L8c6207d0: sa_motion_publish_stop(ctx,0x8c6207d0u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6207d2; /* mov.l @r2,r3 */
L8c6207d2: sa_motion_publish_stop(ctx,0x8c6207d2u); r[14]<<=2;goto L8c6207d4; /* shll2 r14 */
L8c6207d4: sa_motion_publish_stop(ctx,0x8c6207d4u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c6207d6; /* shll r14 */
L8c6207d6: sa_motion_publish_stop(ctx,0x8c6207d6u); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[13]);goto L8c6207d8; /* cmp/ge r13,r5 */
L8c6207d8: sa_motion_publish_stop(ctx,0x8c6207d8u); r[14]+=r[3];goto L8c6207da; /* add r3,r14 */
L8c6207da: sa_motion_publish_stop(ctx,0x8c6207dau); wr(m,bus,&io_status,r[15]+0x4u,r[14],4);if(io_status)return io_status;goto L8c6207dc; /* mov.l r14,@(0x4,r15) */
L8c6207dc: sa_motion_publish_stop(ctx,0x8c6207dcu); r[14]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c6207de; /* mov.l @(0xc,r14),r14 */
L8c6207de: sa_motion_publish_stop(ctx,0x8c6207deu); r[6]&=r[14];goto L8c6207e0; /* and r14,r6 */
L8c6207e0: sa_motion_publish_stop(ctx,0x8c6207e2u); take=!(*sr&1u);r[7]|=r[14];if(take)goto L8c6207c6;goto L8c6207e4; /* bf/s 0x8c6207c6 */
L8c6207e4: sa_motion_publish_stop(ctx,0x8c6207e4u); r[2]=63u;goto L8c6207e6; /* mov #0x3f,r2 */
L8c6207e6: sa_motion_publish_stop(ctx,0x8c6207e6u); *sr=(*sr&~1u)|!(r[2]&r[6]);goto L8c6207e8; /* tst r2,r6 */
L8c6207e8: sa_motion_publish_stop(ctx,0x8c6207e8u); if(!(*sr&1u))goto L8c620844;goto L8c6207ea; /* bf 0x8c620844 */
L8c6207ea: sa_motion_publish_stop(ctx,0x8c6207eau); r[1]=1u;goto L8c6207ec; /* mov #0x1,r1 */
L8c6207ec: sa_motion_publish_stop(ctx,0x8c6207ecu); *sr=(*sr&~1u)|!(r[1]&r[7]);goto L8c6207ee; /* tst r1,r7 */
L8c6207ee: sa_motion_publish_stop(ctx,0x8c6207eeu); if(!(*sr&1u))goto L8c6208ec;goto L8c6207f0; /* bf 0x8c6208ec */
L8c6207f0: sa_motion_publish_stop(ctx,0x8c6207f0u); r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c6207f2; /* mov.w @(0x2,r11),r0 */
L8c6207f2: sa_motion_publish_stop(ctx,0x8c6207f2u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c6207f4; /* mov.w @r11,r4 */
L8c6207f4: sa_motion_publish_stop(ctx,0x8c6207f4u); r[3]=rd(m,bus,&io_status,0x8c6209f0u,4);if(io_status)return io_status;goto L8c6207f6; /* mov.l 0x8c6209f0,r3 */
L8c6207f6: sa_motion_publish_stop(ctx,0x8c6207f6u); r[5]=r[0]&65535u;goto L8c6207f8; /* extu.w r0,r5 */
L8c6207f8: sa_motion_publish_stop(ctx,0x8c6207f8u); r[5]<<=2;goto L8c6207fa; /* shll2 r5 */
L8c6207fa: sa_motion_publish_stop(ctx,0x8c6207fau); r[0]=sx(m,bus,&io_status,r[11]+0x4u,2);if(io_status)return io_status;goto L8c6207fc; /* mov.w @(0x4,r11),r0 */
L8c6207fc: sa_motion_publish_stop(ctx,0x8c6207fcu); r[4]=r[4]&65535u;goto L8c6207fe; /* extu.w r4,r4 */
L8c6207fe: sa_motion_publish_stop(ctx,0x8c6207feu); r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c620800; /* mov.l @r3,r7 */
L8c620800: sa_motion_publish_stop(ctx,0x8c620800u); r[4]<<=2;goto L8c620802; /* shll2 r4 */
L8c620802: sa_motion_publish_stop(ctx,0x8c620802u); r[5]<<=2;goto L8c620804; /* shll2 r5 */
L8c620804: sa_motion_publish_stop(ctx,0x8c620804u); r[6]=r[0]&65535u;goto L8c620806; /* extu.w r0,r6 */
L8c620806: sa_motion_publish_stop(ctx,0x8c620806u); r[4]<<=2;goto L8c620808; /* shll2 r4 */
L8c620808: sa_motion_publish_stop(ctx,0x8c620808u); r[0]=16u;goto L8c62080a; /* mov #0x10,r0 */
L8c62080a: sa_motion_publish_stop(ctx,0x8c62080au); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c62080c; /* shll r4 */
L8c62080c: sa_motion_publish_stop(ctx,0x8c62080cu); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c62080e; /* shll r5 */
L8c62080e: sa_motion_publish_stop(ctx,0x8c62080eu); r[5]+=r[7];goto L8c620810; /* add r7,r5 */
L8c620810: sa_motion_publish_stop(ctx,0x8c620810u); fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c620812; /* fmov.s @(r0,r5),fr5 */
L8c620812: sa_motion_publish_stop(ctx,0x8c620812u); r[6]<<=2;goto L8c620814; /* shll2 r6 */
L8c620814: sa_motion_publish_stop(ctx,0x8c620814u); r[4]+=r[7];goto L8c620816; /* add r7,r4 */
L8c620816: sa_motion_publish_stop(ctx,0x8c620816u); r[6]<<=2;goto L8c620818; /* shll2 r6 */
L8c620818: sa_motion_publish_stop(ctx,0x8c620818u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c62081a; /* fmov.s @(r0,r4),fr3 */
L8c62081a: sa_motion_publish_stop(ctx,0x8c62081au); fcopy(f,2u,5u);goto L8c62081c; /* fmov fr5,fr2 */
L8c62081c: sa_motion_publish_stop(ctx,0x8c62081cu); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c62081e; /* shll r6 */
L8c62081e: sa_motion_publish_stop(ctx,0x8c62081eu); r[0]=20u;goto L8c620820; /* mov #0x14,r0 */
L8c620820: sa_motion_publish_stop(ctx,0x8c620820u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c620822; /* fsub fr3,fr2 */
L8c620822: sa_motion_publish_stop(ctx,0x8c620822u); r[6]+=r[7];goto L8c620824; /* add r7,r6 */
L8c620824: sa_motion_publish_stop(ctx,0x8c620824u); fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c620826; /* fmov.s @(r0,r5),fr4 */
L8c620826: sa_motion_publish_stop(ctx,0x8c620826u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620828; /* fmov.s @(r0,r6),fr3 */
L8c620828: sa_motion_publish_stop(ctx,0x8c620828u); fcopy(f,1u,4u);goto L8c62082a; /* fmov fr4,fr1 */
L8c62082a: sa_motion_publish_stop(ctx,0x8c62082au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c62082c; /* fsub fr4,fr3 */
L8c62082c: sa_motion_publish_stop(ctx,0x8c62082cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c62082e; /* fmul fr3,fr2 */
L8c62082e: sa_motion_publish_stop(ctx,0x8c62082eu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620830; /* fmov.s @(r0,r4),fr3 */
L8c620830: sa_motion_publish_stop(ctx,0x8c620830u); r[0]=16u;goto L8c620832; /* mov #0x10,r0 */
L8c620832: sa_motion_publish_stop(ctx,0x8c620832u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c620834; /* fsub fr3,fr1 */
L8c620834: sa_motion_publish_stop(ctx,0x8c620834u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620836; /* fmov.s @(r0,r6),fr3 */
L8c620836: sa_motion_publish_stop(ctx,0x8c620836u); fcopy(f,4u,2u);goto L8c620838; /* fmov fr2,fr4 */
L8c620838: sa_motion_publish_stop(ctx,0x8c620838u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c62083a; /* fsub fr5,fr3 */
L8c62083a: sa_motion_publish_stop(ctx,0x8c62083au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c62083c; /* fmul fr3,fr1 */
L8c62083c: sa_motion_publish_stop(ctx,0x8c62083cu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c62083e; /* fldi0 fr3 */
L8c62083e: sa_motion_publish_stop(ctx,0x8c62083eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c620840; /* fsub fr1,fr4 */
L8c620840: sa_motion_publish_stop(ctx,0x8c620840u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c620842; /* fcmp/gt fr3,fr4 */
L8c620842: sa_motion_publish_stop(ctx,0x8c620842u); if(!(*sr&1u))goto L8c62084a;goto L8c620844; /* bf 0x8c62084a */
L8c620844: sa_motion_publish_stop(ctx,0x8c620844u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c620846; /* shll r13 */
L8c620846: sa_motion_publish_stop(ctx,0x8c620848u); r[11]+=r[13];goto L8c62094a; /* bra 0x8c62094a */
L8c62084a: sa_motion_publish_stop(ctx,0x8c62084au); r[2]=rd(m,bus,&io_status,0x8c6209f4u,4);if(io_status)return io_status;goto L8c62084c; /* mov.l 0x8c6209f4,r2 */
L8c62084c: sa_motion_publish_stop(ctx,0x8c62084cu); r[6]=r[13];goto L8c62084e; /* mov r13,r6 */
L8c62084e: sa_motion_publish_stop(ctx,0x8c62084eu); r[6]+=4294967295u;goto L8c620850; /* add #-0x1,r6 */
L8c620850: sa_motion_publish_stop(ctx,0x8c620850u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c620852; /* mov.l @r2,r3 */
L8c620852: sa_motion_publish_stop(ctx,0x8c620852u); *sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c620854; /* cmp/pl r6 */
L8c620854: sa_motion_publish_stop(ctx,0x8c620854u); r[3]+=r[13];goto L8c620856; /* add r13,r3 */
L8c620856: sa_motion_publish_stop(ctx,0x8c620856u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c620858; /* mov.l r3,@r2 */
L8c620858: sa_motion_publish_stop(ctx,0x8c62085au); take=!(*sr&1u);r[5]=0u;if(take)goto L8c6208a6;goto L8c62085c; /* bf/s 0x8c6208a6 */
L8c62085c: sa_motion_publish_stop(ctx,0x8c62085cu); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c62085e; /* mov.w @r11+,r4 */
L8c62085e: sa_motion_publish_stop(ctx,0x8c62085eu); r[8]+=32u;goto L8c620860; /* add #0x20,r8 */
L8c620860: sa_motion_publish_stop(ctx,0x8c620860u); r[2]=rd(m,bus,&io_status,0x8c6209f0u,4);if(io_status)return io_status;goto L8c620862; /* mov.l 0x8c6209f0,r2 */
L8c620862: sa_motion_publish_stop(ctx,0x8c620862u); r[0]=24u;goto L8c620864; /* mov #0x18,r0 */
L8c620864: sa_motion_publish_stop(ctx,0x8c620864u); r[4]=r[4]&65535u;goto L8c620866; /* extu.w r4,r4 */
L8c620866: sa_motion_publish_stop(ctx,0x8c620866u); r[1]=rd(m,bus,&io_status,0x8c6209f8u,4);if(io_status)return io_status;goto L8c620868; /* mov.l 0x8c6209f8,r1 */
L8c620868: sa_motion_publish_stop(ctx,0x8c620868u); r[4]<<=2;goto L8c62086a; /* shll2 r4 */
L8c62086a: sa_motion_publish_stop(ctx,0x8c62086au); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c62086c; /* mov.l @r2,r3 */
L8c62086c: sa_motion_publish_stop(ctx,0x8c62086cu); r[4]<<=2;goto L8c62086e; /* shll2 r4 */
L8c62086e: sa_motion_publish_stop(ctx,0x8c62086eu); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c620870; /* fmov.s @r1,fr3 */
L8c620870: sa_motion_publish_stop(ctx,0x8c620870u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c620872; /* shll r4 */
L8c620872: sa_motion_publish_stop(ctx,0x8c620872u); r[2]=rd(m,bus,&io_status,0x8c620a00u,4);if(io_status)return io_status;goto L8c620874; /* mov.l 0x8c620a00,r2 */
L8c620874: sa_motion_publish_stop(ctx,0x8c620874u); r[4]+=r[3];goto L8c620876; /* add r3,r4 */
L8c620876: sa_motion_publish_stop(ctx,0x8c620876u); r[3]=rd(m,bus,&io_status,0x8c6209fcu,4);if(io_status)return io_status;goto L8c620878; /* mov.l 0x8c6209fc,r3 */
L8c620878: sa_motion_publish_stop(ctx,0x8c620878u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62087a; /* fmov.s fr3,@-r8 */
L8c62087a: sa_motion_publish_stop(ctx,0x8c62087au); r[1]+=4294967284u;goto L8c62087c; /* add #-0xc,r1 */
L8c62087c: sa_motion_publish_stop(ctx,0x8c62087cu); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c62087e; /* fmov.s @r3,fr3 */
L8c62087e: sa_motion_publish_stop(ctx,0x8c62087eu); r[3]=rd(m,bus,&io_status,0x8c620a04u,4);if(io_status)return io_status;goto L8c620880; /* mov.l 0x8c620a04,r3 */
L8c620880: sa_motion_publish_stop(ctx,0x8c620880u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620882; /* fmov.s fr3,@-r8 */
L8c620882: sa_motion_publish_stop(ctx,0x8c620882u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c620884; /* fmov.s @r2,fr3 */
L8c620884: sa_motion_publish_stop(ctx,0x8c620884u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620886; /* fmov.s fr3,@-r8 */
L8c620886: sa_motion_publish_stop(ctx,0x8c620886u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c620888; /* fmov.s @r1,fr3 */
L8c620888: sa_motion_publish_stop(ctx,0x8c620888u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62088a; /* fmov.s fr3,@-r8 */
L8c62088a: sa_motion_publish_stop(ctx,0x8c62088au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c62088c; /* fmov.s @(r0,r4),fr3 */
L8c62088c: sa_motion_publish_stop(ctx,0x8c62088cu); r[0]=20u;goto L8c62088e; /* mov #0x14,r0 */
L8c62088e: sa_motion_publish_stop(ctx,0x8c62088eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620890; /* fmov.s fr3,@-r8 */
L8c620890: sa_motion_publish_stop(ctx,0x8c620890u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620892; /* fmov.s @(r0,r4),fr3 */
L8c620892: sa_motion_publish_stop(ctx,0x8c620892u); r[0]=16u;goto L8c620894; /* mov #0x10,r0 */
L8c620894: sa_motion_publish_stop(ctx,0x8c620894u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620896; /* fmov.s fr3,@-r8 */
L8c620896: sa_motion_publish_stop(ctx,0x8c620896u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620898; /* fmov.s @(r0,r4),fr3 */
L8c620898: sa_motion_publish_stop(ctx,0x8c620898u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62089a; /* fmov.s fr3,@-r8 */
L8c62089a: sa_motion_publish_stop(ctx,0x8c62089au); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c62089c; /* mov.l r3,@-r8 */
L8c62089c: sa_motion_publish_stop(ctx,0x8c62089cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c62089e; /* pref @r8 */
L8c62089e: sa_motion_publish_stop(ctx,0x8c62089eu); r[5]+=1u;goto L8c6208a0; /* add #0x1,r5 */
L8c6208a0: sa_motion_publish_stop(ctx,0x8c6208a0u); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c6208a2; /* cmp/ge r6,r5 */
L8c6208a2: sa_motion_publish_stop(ctx,0x8c6208a4u); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c62085c;goto L8c6208a6; /* bf/s 0x8c62085c */
L8c6208a6: sa_motion_publish_stop(ctx,0x8c6208a6u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6208a8; /* mov.w @r11+,r4 */
L8c6208a8: sa_motion_publish_stop(ctx,0x8c6208a8u); r[8]+=32u;goto L8c6208aa; /* add #0x20,r8 */
L8c6208aa: sa_motion_publish_stop(ctx,0x8c6208aau); r[2]=rd(m,bus,&io_status,0x8c6209f0u,4);if(io_status)return io_status;goto L8c6208ac; /* mov.l 0x8c6209f0,r2 */
L8c6208ac: sa_motion_publish_stop(ctx,0x8c6208acu); r[0]=24u;goto L8c6208ae; /* mov #0x18,r0 */
L8c6208ae: sa_motion_publish_stop(ctx,0x8c6208aeu); r[4]=r[4]&65535u;goto L8c6208b0; /* extu.w r4,r4 */
L8c6208b0: sa_motion_publish_stop(ctx,0x8c6208b0u); r[1]=rd(m,bus,&io_status,0x8c6209f8u,4);if(io_status)return io_status;goto L8c6208b2; /* mov.l 0x8c6209f8,r1 */
L8c6208b2: sa_motion_publish_stop(ctx,0x8c6208b2u); r[4]<<=2;goto L8c6208b4; /* shll2 r4 */
L8c6208b4: sa_motion_publish_stop(ctx,0x8c6208b4u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6208b6; /* mov.l @r2,r3 */
L8c6208b6: sa_motion_publish_stop(ctx,0x8c6208b6u); r[4]<<=2;goto L8c6208b8; /* shll2 r4 */
L8c6208b8: sa_motion_publish_stop(ctx,0x8c6208b8u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6208ba; /* fmov.s @r1,fr3 */
L8c6208ba: sa_motion_publish_stop(ctx,0x8c6208bau); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6208bc; /* shll r4 */
L8c6208bc: sa_motion_publish_stop(ctx,0x8c6208bcu); r[2]=rd(m,bus,&io_status,0x8c620a00u,4);if(io_status)return io_status;goto L8c6208be; /* mov.l 0x8c620a00,r2 */
L8c6208be: sa_motion_publish_stop(ctx,0x8c6208beu); r[4]+=r[3];goto L8c6208c0; /* add r3,r4 */
L8c6208c0: sa_motion_publish_stop(ctx,0x8c6208c0u); r[3]=rd(m,bus,&io_status,0x8c6209fcu,4);if(io_status)return io_status;goto L8c6208c2; /* mov.l 0x8c6209fc,r3 */
L8c6208c2: sa_motion_publish_stop(ctx,0x8c6208c2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208c4; /* fmov.s fr3,@-r8 */
L8c6208c4: sa_motion_publish_stop(ctx,0x8c6208c4u); r[1]+=4294967284u;goto L8c6208c6; /* add #-0xc,r1 */
L8c6208c6: sa_motion_publish_stop(ctx,0x8c6208c6u); fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6208c8; /* fmov.s @r3,fr3 */
L8c6208c8: sa_motion_publish_stop(ctx,0x8c6208c8u); r[3]=rd(m,bus,&io_status,0x8c620a08u,4);if(io_status)return io_status;goto L8c6208ca; /* mov.l 0x8c620a08,r3 */
L8c6208ca: sa_motion_publish_stop(ctx,0x8c6208cau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208cc; /* fmov.s fr3,@-r8 */
L8c6208cc: sa_motion_publish_stop(ctx,0x8c6208ccu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6208ce; /* fmov.s @r2,fr3 */
L8c6208ce: sa_motion_publish_stop(ctx,0x8c6208ceu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208d0; /* fmov.s fr3,@-r8 */
L8c6208d0: sa_motion_publish_stop(ctx,0x8c6208d0u); fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6208d2; /* fmov.s @r1,fr3 */
L8c6208d2: sa_motion_publish_stop(ctx,0x8c6208d2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208d4; /* fmov.s fr3,@-r8 */
L8c6208d4: sa_motion_publish_stop(ctx,0x8c6208d4u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6208d6; /* fmov.s @(r0,r4),fr3 */
L8c6208d6: sa_motion_publish_stop(ctx,0x8c6208d6u); r[0]=20u;goto L8c6208d8; /* mov #0x14,r0 */
L8c6208d8: sa_motion_publish_stop(ctx,0x8c6208d8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208da; /* fmov.s fr3,@-r8 */
L8c6208da: sa_motion_publish_stop(ctx,0x8c6208dau); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6208dc; /* fmov.s @(r0,r4),fr3 */
L8c6208dc: sa_motion_publish_stop(ctx,0x8c6208dcu); r[0]=16u;goto L8c6208de; /* mov #0x10,r0 */
L8c6208de: sa_motion_publish_stop(ctx,0x8c6208deu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208e0; /* fmov.s fr3,@-r8 */
L8c6208e0: sa_motion_publish_stop(ctx,0x8c6208e0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6208e2; /* fmov.s @(r0,r4),fr3 */
L8c6208e2: sa_motion_publish_stop(ctx,0x8c6208e2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208e4; /* fmov.s fr3,@-r8 */
L8c6208e4: sa_motion_publish_stop(ctx,0x8c6208e4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6208e6; /* mov.l r3,@-r8 */
L8c6208e6: sa_motion_publish_stop(ctx,0x8c6208e6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6208e8; /* pref @r8 */
L8c6208e8: sa_motion_publish_stop(ctx,0x8c6208eau); r[8]+=32u;goto L8c62094a; /* bra 0x8c62094a */
L8c6208ec: sa_motion_publish_stop(ctx,0x8c6208ecu); r[2]=rd(m,bus,&io_status,0x8c620a0cu,4);if(io_status)return io_status;goto L8c6208ee; /* mov.l 0x8c620a0c,r2 */
L8c6208ee: sa_motion_publish_stop(ctx,0x8c6208eeu); r[3]=0u;goto L8c6208f0; /* mov #0x0,r3 */
L8c6208f0: sa_motion_publish_stop(ctx,0x8c6208f0u); r[0]=rd(m,bus,&io_status,0x8c6209f0u,4);if(io_status)return io_status;goto L8c6208f2; /* mov.l 0x8c6209f0,r0 */
L8c6208f2: sa_motion_publish_stop(ctx,0x8c6208f2u); r[10]=r[13];goto L8c6208f4; /* mov r13,r10 */
L8c6208f4: sa_motion_publish_stop(ctx,0x8c6208f4u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6208f6; /* mov.l r3,@r2 */
L8c6208f6: sa_motion_publish_stop(ctx,0x8c6208f6u); r[10]+=4294967294u;goto L8c6208f8; /* add #-0x2,r10 */
L8c6208f8: sa_motion_publish_stop(ctx,0x8c6208f8u); r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6208fa; /* mov.w @r11+,r4 */
L8c6208fa: sa_motion_publish_stop(ctx,0x8c6208fau); r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c6208fc; /* mov.l @r0,r3 */
L8c6208fc: sa_motion_publish_stop(ctx,0x8c6208fcu); r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6208fe; /* mov.w @r11+,r14 */
L8c6208fe: sa_motion_publish_stop(ctx,0x8c6208feu); r[4]=r[4]&65535u;goto L8c620900; /* extu.w r4,r4 */
L8c620900: sa_motion_publish_stop(ctx,0x8c620900u); r[4]<<=2;goto L8c620902; /* shll2 r4 */
L8c620902: sa_motion_publish_stop(ctx,0x8c620902u); r[14]=r[14]&65535u;goto L8c620904; /* extu.w r14,r14 */
L8c620904: sa_motion_publish_stop(ctx,0x8c620904u); r[14]<<=2;goto L8c620906; /* shll2 r14 */
L8c620906: sa_motion_publish_stop(ctx,0x8c620906u); r[14]<<=2;goto L8c620908; /* shll2 r14 */
L8c620908: sa_motion_publish_stop(ctx,0x8c620908u); r[4]<<=2;goto L8c62090a; /* shll2 r4 */
L8c62090a: sa_motion_publish_stop(ctx,0x8c62090au); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c62090c; /* shll r4 */
L8c62090c: sa_motion_publish_stop(ctx,0x8c62090cu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c62090e; /* shll r14 */
L8c62090e: sa_motion_publish_stop(ctx,0x8c62090eu); *sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8c620910; /* cmp/pl r10 */
L8c620910: sa_motion_publish_stop(ctx,0x8c620910u); r[4]+=r[3];goto L8c620912; /* add r3,r4 */
L8c620912: sa_motion_publish_stop(ctx,0x8c620912u); r[14]+=r[3];goto L8c620914; /* add r3,r14 */
L8c620914: sa_motion_publish_stop(ctx,0x8c620916u); take=!(*sr&1u);r[12]=0u;if(take)goto L8c62094a;goto L8c620918; /* bf/s 0x8c62094a */
L8c620918: sa_motion_publish_stop(ctx,0x8c620918u); r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c62091a; /* mov.w @r11+,r13 */
L8c62091a: sa_motion_publish_stop(ctx,0x8c62091au); r[2]=rd(m,bus,&io_status,0x8c6209f0u,4);if(io_status)return io_status;goto L8c62091c; /* mov.l 0x8c6209f0,r2 */
L8c62091c: sa_motion_publish_stop(ctx,0x8c62091cu); r[13]=r[13]&65535u;goto L8c62091e; /* extu.w r13,r13 */
L8c62091e: sa_motion_publish_stop(ctx,0x8c62091eu); r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c620920; /* mov.l @(0xc,r4),r5 */
L8c620920: sa_motion_publish_stop(ctx,0x8c620920u); r[13]<<=2;goto L8c620922; /* shll2 r13 */
L8c620922: sa_motion_publish_stop(ctx,0x8c620922u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c620924; /* mov.l @r2,r3 */
L8c620924: sa_motion_publish_stop(ctx,0x8c620924u); r[13]<<=2;goto L8c620926; /* shll2 r13 */
L8c620926: sa_motion_publish_stop(ctx,0x8c620926u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c620928; /* shll r13 */
L8c620928: sa_motion_publish_stop(ctx,0x8c620928u); r[13]+=r[3];goto L8c62092a; /* add r3,r13 */
L8c62092a: sa_motion_publish_stop(ctx,0x8c62092au); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c62092c; /* mov.l @(0xc,r14),r3 */
L8c62092c: sa_motion_publish_stop(ctx,0x8c62092cu); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c62092e; /* mov.l @(0xc,r13),r1 */
L8c62092e: sa_motion_publish_stop(ctx,0x8c62092eu); r[5]&=r[3];goto L8c620930; /* and r3,r5 */
L8c620930: sa_motion_publish_stop(ctx,0x8c620930u); r[3]=1u;goto L8c620932; /* mov #0x1,r3 */
L8c620932: sa_motion_publish_stop(ctx,0x8c620932u); r[5]&=r[1];goto L8c620934; /* and r1,r5 */
L8c620934: sa_motion_publish_stop(ctx,0x8c620934u); *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c620936; /* tst r3,r5 */
L8c620936: sa_motion_publish_stop(ctx,0x8c620936u); if(!(*sr&1u))goto L8c620940;goto L8c620938; /* bf 0x8c620940 */
L8c620938: sa_motion_publish_stop(ctx,0x8c620938u); r[1]=rd(m,bus,&io_status,0x8c620a10u,4);if(io_status)return io_status;goto L8c62093a; /* mov.l 0x8c620a10,r1 */
L8c62093a: sa_motion_publish_stop(ctx,0x8c62093au); r[6]=r[13];goto L8c62093c; /* mov r13,r6 */
L8c62093c: sa_motion_publish_stop(ctx,0x8c62093eu); target=r[1];*pr=0x8c620940u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620940; /* jsr @r1 */
L8c620940: sa_motion_publish_stop(ctx,0x8c620940u); r[12]+=1u;goto L8c620942; /* add #0x1,r12 */
L8c620942: sa_motion_publish_stop(ctx,0x8c620942u); r[4]=r[14];goto L8c620944; /* mov r14,r4 */
L8c620944: sa_motion_publish_stop(ctx,0x8c620944u); *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[10]);goto L8c620946; /* cmp/ge r10,r12 */
L8c620946: sa_motion_publish_stop(ctx,0x8c620948u); take=!(*sr&1u);r[14]=r[13];if(take)goto L8c620918;goto L8c62094a; /* bf/s 0x8c620918 */
L8c62094a: sa_motion_publish_stop(ctx,0x8c62094au); r[9]+=1u;goto L8c62094c; /* add #0x1,r9 */
L8c62094c: sa_motion_publish_stop(ctx,0x8c62094cu); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c62094e; /* mov.l @r15,r2 */
L8c62094e: sa_motion_publish_stop(ctx,0x8c62094eu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c620950; /* mov.w @(0x2,r2),r0 */
L8c620950: sa_motion_publish_stop(ctx,0x8c620950u); r[0]=r[0]&65535u;goto L8c620952; /* extu.w r0,r0 */
L8c620952: sa_motion_publish_stop(ctx,0x8c620952u); *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c620954; /* cmp/ge r0,r9 */
L8c620954: sa_motion_publish_stop(ctx,0x8c620954u); if((*sr&1u))goto L8c62095a;goto L8c620956; /* bt 0x8c62095a */
L8c620956: sa_motion_publish_stop(ctx,0x8c620958u); goto L8c6207ac; /* bra 0x8c6207ac */
L8c62095a: sa_motion_publish_stop(ctx,0x8c62095cu); goto L8c620990; /* bra 0x8c620990 */
L8c62095e: sa_motion_publish_stop(ctx,0x8c62095eu); r[2]=rd(m,bus,&io_status,0x8c620a14u,4);if(io_status)return io_status;goto L8c620960; /* mov.l 0x8c620a14,r2 */
L8c620960: sa_motion_publish_stop(ctx,0x8c620960u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c620962; /* mov.l @r15,r1 */
L8c620962: sa_motion_publish_stop(ctx,0x8c620962u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c620964; /* mov.l r1,@r2 */
L8c620964: sa_motion_publish_stop(ctx,0x8c620964u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c620966; /* frchg */
L8c620966: sa_motion_publish_stop(ctx,0x8c620966u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c620968; /* fmov.s fr0,@-r15 */
L8c620968: sa_motion_publish_stop(ctx,0x8c620968u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c62096a; /* fmov.s fr1,@-r15 */
L8c62096a: sa_motion_publish_stop(ctx,0x8c62096au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c62096c; /* fmov.s fr2,@-r15 */
L8c62096c: sa_motion_publish_stop(ctx,0x8c62096cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c62096e; /* fmov.s fr3,@-r15 */
L8c62096e: sa_motion_publish_stop(ctx,0x8c62096eu); r[0]=rd(m,bus,&io_status,0x8c6209a4u,4);if(io_status)return io_status;goto L8c620970; /* mov.l 0x8c6209a4,r0 */
L8c620970: sa_motion_publish_stop(ctx,0x8c620970u); r[0]+=16u;goto L8c620972; /* add #0x10,r0 */
L8c620972: sa_motion_publish_stop(ctx,0x8c620972u); fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620974; /* fmov.s @r0+,fr0 */
L8c620974: sa_motion_publish_stop(ctx,0x8c620974u); fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620976; /* fmov.s @r0+,fr1 */
L8c620976: sa_motion_publish_stop(ctx,0x8c620976u); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620978; /* fmov.s @r0+,fr2 */
L8c620978: sa_motion_publish_stop(ctx,0x8c620978u); fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c62097a; /* fmov.s @r0,fr3 */
L8c62097a: sa_motion_publish_stop(ctx,0x8c62097au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c62097c; /* frchg */
L8c62097c: sa_motion_publish_stop(ctx,0x8c62097cu); r[2]=rd(m,bus,&io_status,0x8c620a18u,4);if(io_status)return io_status;goto L8c62097e; /* mov.l 0x8c620a18,r2 */
L8c62097e: sa_motion_publish_stop(ctx,0x8c62097eu); r[3]=rd(m,bus,&io_status,0x8c620a14u,4);if(io_status)return io_status;goto L8c620980; /* mov.l 0x8c620a14,r3 */
L8c620980: sa_motion_publish_stop(ctx,0x8c620982u); target=r[2];*pr=0x8c620984u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620984; /* jsr @r2 */
L8c620984: sa_motion_publish_stop(ctx,0x8c620984u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c620986; /* frchg */
L8c620986: sa_motion_publish_stop(ctx,0x8c620986u); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620988; /* fmov.s @r15+,fr3 */
L8c620988: sa_motion_publish_stop(ctx,0x8c620988u); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c62098a; /* fmov.s @r15+,fr2 */
L8c62098a: sa_motion_publish_stop(ctx,0x8c62098au); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c62098c; /* fmov.s @r15+,fr1 */
L8c62098c: sa_motion_publish_stop(ctx,0x8c62098cu); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c62098e; /* fmov.s @r15+,fr0 */
L8c62098e: sa_motion_publish_stop(ctx,0x8c62098eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c620990; /* frchg */
L8c620990: sa_motion_publish_stop(ctx,0x8c620990u); r[15]+=8u;goto L8c620992; /* add #0x8,r15 */
L8c620992: sa_motion_publish_stop(ctx,0x8c620992u); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620994; /* lds.l @r15+,PR */
L8c620994: sa_motion_publish_stop(ctx,0x8c620994u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620996; /* mov.l @r15+,r9 */
L8c620996: sa_motion_publish_stop(ctx,0x8c620996u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620998; /* mov.l @r15+,r10 */
L8c620998: sa_motion_publish_stop(ctx,0x8c620998u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c62099a; /* mov.l @r15+,r11 */
L8c62099a: sa_motion_publish_stop(ctx,0x8c62099au); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c62099c; /* mov.l @r15+,r12 */
L8c62099c: sa_motion_publish_stop(ctx,0x8c62099cu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c62099e; /* mov.l @r15+,r13 */
L8c62099e: sa_motion_publish_stop(ctx,0x8c6209a0u); r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
