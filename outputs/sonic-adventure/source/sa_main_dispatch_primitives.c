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
L8c61ffe4: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61ffe6; /* mov.l r14,@-r15 */
L8c61ffe6: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61ffe8; /* mov.l r13,@-r15 */
L8c61ffe8: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61ffea; /* mov.l r12,@-r15 */
L8c61ffea: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61ffec; /* mov.l r11,@-r15 */
L8c61ffec: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61ffee; /* mov.l r10,@-r15 */
L8c61ffee: r[3]=rd(m,bus,&io_status,0x8c620148u,4);if(io_status)return io_status;goto L8c61fff0; /* mov.l 0x8c620148,r3 */
L8c61fff0: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61fff2; /* mov.l r9,@-r15 */
L8c61fff2: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61fff4; /* sts.l PR,@-r15 */
L8c61fff4: r[0]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;goto L8c61fff6; /* mov.w @r4,r0 */
L8c61fff6: r[13]=rd(m,bus,&io_status,r[4]+0x4u,4);if(io_status)return io_status;goto L8c61fff8; /* mov.l @(0x4,r4),r13 */
L8c61fff8: r[0]=r[0]&65535u;goto L8c61fffa; /* extu.w r0,r0 */
L8c61fffa: r[9]=rd(m,bus,&io_status,0x8c620140u,4);if(io_status)return io_status;goto L8c61fffc; /* mov.l 0x8c620140,r9 */
L8c61fffc: r[0]&=r[3];goto L8c61fffe; /* and r3,r0 */
L8c61fffe: r[11]=rd(m,bus,&io_status,0x8c620144u,4);if(io_status)return io_status;goto L8c620000; /* mov.l 0x8c620144,r11 */
L8c620000: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c620002; /* cmp/eq #0x0,r0 */
L8c620002: take=(*sr&1u);r[12]=0u;if(take)goto L8c620028;goto L8c620006; /* bt/s 0x8c620028 */
L8c620006: r[1]=sx(m,bus,&io_status,0x8c62013cu,2);if(io_status)return io_status;goto L8c620008; /* mov.w 0x8c62013c,r1 */
L8c620008: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c62000a; /* cmp/eq r1,r0 */
L8c62000a: if(!(*sr&1u))goto L8c620010;goto L8c62000c; /* bf 0x8c620010 */
L8c62000c: goto L8c620160; /* bra 0x8c620160 */
L8c620010: r[1]=rd(m,bus,&io_status,0x8c62014cu,4);if(io_status)return io_status;goto L8c620012; /* mov.l 0x8c62014c,r1 */
L8c620012: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620014; /* cmp/eq r1,r0 */
L8c620014: if(!(*sr&1u))goto L8c62001a;goto L8c620016; /* bf 0x8c62001a */
L8c620016: goto L8c6202b0; /* bra 0x8c6202b0 */
L8c62001a: r[1]=rd(m,bus,&io_status,0x8c620148u,4);if(io_status)return io_status;goto L8c62001c; /* mov.l 0x8c620148,r1 */
L8c62001c: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c62001e; /* cmp/eq r1,r0 */
L8c62001e: if(!(*sr&1u))goto L8c620024;goto L8c620020; /* bf 0x8c620024 */
L8c620020: goto L8c6203ca; /* bra 0x8c6203ca */
L8c620024: goto L8c6203fa; /* bra 0x8c6203fa */
L8c620028: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c62002a; /* mov.w @(0x2,r4),r0 */
L8c62002a: r[1]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c62002c; /* mov.l @r11,r1 */
L8c62002c: r[0]=r[0]&65535u;goto L8c62002e; /* extu.w r0,r0 */
L8c62002e: r[2]=r[0];goto L8c620030; /* mov r0,r2 */
L8c620030: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c620032; /* shll r0 */
L8c620032: r[0]+=r[2];goto L8c620034; /* add r2,r0 */
L8c620034: r[1]+=r[0];goto L8c620036; /* add r0,r1 */
L8c620036: wr(m,bus,&io_status,r[11],r[1],4);if(io_status)return io_status;goto L8c620038; /* mov.l r1,@r11 */
L8c620038: r[14]=r[12];goto L8c620130; /* bra 0x8c620130 */
L8c62003c: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c62003e; /* mov.w @r13+,r5 */
L8c62003e: r[0]=16u;goto L8c620040; /* mov #0x10,r0 */
L8c620040: r[3]=rd(m,bus,&io_status,0x8c620150u,4);if(io_status)return io_status;goto L8c620042; /* mov.l 0x8c620150,r3 */
L8c620042: r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c620044; /* mov.w @r13+,r6 */
L8c620044: r[5]=r[5]&65535u;goto L8c620046; /* extu.w r5,r5 */
L8c620046: r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c620048; /* mov.l @r3,r12 */
L8c620048: r[5]<<=2;goto L8c62004a; /* shll2 r5 */
L8c62004a: r[6]=r[6]&65535u;goto L8c62004c; /* extu.w r6,r6 */
L8c62004c: r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c62004e; /* mov.w @r13+,r7 */
L8c62004e: r[6]<<=2;goto L8c620050; /* shll2 r6 */
L8c620050: r[6]<<=2;goto L8c620052; /* shll2 r6 */
L8c620052: r[5]<<=2;goto L8c620054; /* shll2 r5 */
L8c620054: r[7]=r[7]&65535u;goto L8c620056; /* extu.w r7,r7 */
L8c620056: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c620058; /* shll r5 */
L8c620058: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c62005a; /* shll r6 */
L8c62005a: r[6]+=r[12];goto L8c62005c; /* add r12,r6 */
L8c62005c: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c62005e; /* fmov.s @(r0,r6),fr4 */
L8c62005e: r[5]+=r[12];goto L8c620060; /* add r12,r5 */
L8c620060: r[7]<<=2;goto L8c620062; /* shll2 r7 */
L8c620062: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620064; /* fmov.s @(r0,r5),fr3 */
L8c620064: r[7]<<=2;goto L8c620066; /* shll2 r7 */
L8c620066: fcopy(f,2u,4u);goto L8c620068; /* fmov fr4,fr2 */
L8c620068: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c62006a; /* shll r7 */
L8c62006a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c62006c; /* fsub fr3,fr2 */
L8c62006c: r[0]=20u;goto L8c62006e; /* mov #0x14,r0 */
L8c62006e: r[7]+=r[12];goto L8c620070; /* add r12,r7 */
L8c620070: fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c620072; /* fmov.s @(r0,r6),fr5 */
L8c620072: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c620074; /* fmov.s @(r0,r7),fr3 */
L8c620074: fcopy(f,1u,5u);goto L8c620076; /* fmov fr5,fr1 */
L8c620076: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c620078; /* fsub fr5,fr3 */
L8c620078: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c62007a; /* fmul fr3,fr2 */
L8c62007a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c62007c; /* fmov.s @(r0,r5),fr3 */
L8c62007c: r[0]=16u;goto L8c62007e; /* mov #0x10,r0 */
L8c62007e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c620080; /* fsub fr3,fr1 */
L8c620080: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c620082; /* fmov.s @(r0,r7),fr3 */
L8c620082: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c620084; /* fsub fr4,fr3 */
L8c620084: fcopy(f,4u,2u);goto L8c620086; /* fmov fr2,fr4 */
L8c620086: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c620088; /* fmul fr3,fr1 */
L8c620088: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c62008a; /* fldi0 fr3 */
L8c62008a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c62008c; /* fsub fr1,fr4 */
L8c62008c: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c62008e; /* fcmp/gt fr3,fr4 */
L8c62008e: take=(*sr&1u);r[14]+=1u;if(take)goto L8c620130;goto L8c620092; /* bt/s 0x8c620130 */
L8c620092: r[0]=28u;goto L8c620094; /* mov #0x1c,r0 */
L8c620094: r[3]=rd(m,bus,&io_status,0x8c620154u,4);if(io_status)return io_status;goto L8c620096; /* mov.l 0x8c620154,r3 */
L8c620096: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620098; /* fmov.s @(r0,r9),fr3 */
L8c620098: r[8]+=32u;goto L8c62009a; /* add #0x20,r8 */
L8c62009a: r[0]=24u;goto L8c62009c; /* mov #0x18,r0 */
L8c62009c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62009e; /* fmov.s fr3,@-r8 */
L8c62009e: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200a0; /* fmov.s @(r0,r9),fr3 */
L8c6200a0: r[0]=20u;goto L8c6200a2; /* mov #0x14,r0 */
L8c6200a2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200a4; /* fmov.s fr3,@-r8 */
L8c6200a4: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200a6; /* fmov.s @(r0,r9),fr3 */
L8c6200a6: r[0]=16u;goto L8c6200a8; /* mov #0x10,r0 */
L8c6200a8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200aa; /* fmov.s fr3,@-r8 */
L8c6200aa: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200ac; /* fmov.s @(r0,r9),fr3 */
L8c6200ac: r[0]=24u;goto L8c6200ae; /* mov #0x18,r0 */
L8c6200ae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200b0; /* fmov.s fr3,@-r8 */
L8c6200b0: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6200b2; /* fmov.s @(r0,r5),fr3 */
L8c6200b2: r[0]=20u;goto L8c6200b4; /* mov #0x14,r0 */
L8c6200b4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200b6; /* fmov.s fr3,@-r8 */
L8c6200b6: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6200b8; /* fmov.s @(r0,r5),fr3 */
L8c6200b8: r[0]=16u;goto L8c6200ba; /* mov #0x10,r0 */
L8c6200ba: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200bc; /* fmov.s fr3,@-r8 */
L8c6200bc: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6200be; /* fmov.s @(r0,r5),fr3 */
L8c6200be: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200c0; /* fmov.s fr3,@-r8 */
L8c6200c0: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6200c2; /* mov.l r3,@-r8 */
L8c6200c2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6200c4; /* pref @r8 */
L8c6200c4: r[0]=28u;goto L8c6200c6; /* mov #0x1c,r0 */
L8c6200c6: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200c8; /* fmov.s @(r0,r9),fr3 */
L8c6200c8: r[8]+=64u;goto L8c6200ca; /* add #0x40,r8 */
L8c6200ca: r[0]=24u;goto L8c6200cc; /* mov #0x18,r0 */
L8c6200cc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200ce; /* fmov.s fr3,@-r8 */
L8c6200ce: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200d0; /* fmov.s @(r0,r9),fr3 */
L8c6200d0: r[0]=20u;goto L8c6200d2; /* mov #0x14,r0 */
L8c6200d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200d4; /* fmov.s fr3,@-r8 */
L8c6200d4: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200d6; /* fmov.s @(r0,r9),fr3 */
L8c6200d6: r[0]=16u;goto L8c6200d8; /* mov #0x10,r0 */
L8c6200d8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200da; /* fmov.s fr3,@-r8 */
L8c6200da: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200dc; /* fmov.s @(r0,r9),fr3 */
L8c6200dc: r[0]=24u;goto L8c6200de; /* mov #0x18,r0 */
L8c6200de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200e0; /* fmov.s fr3,@-r8 */
L8c6200e0: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6200e2; /* fmov.s @(r0,r6),fr3 */
L8c6200e2: r[0]=20u;goto L8c6200e4; /* mov #0x14,r0 */
L8c6200e4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200e6; /* fmov.s fr3,@-r8 */
L8c6200e6: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6200e8; /* fmov.s @(r0,r6),fr3 */
L8c6200e8: r[0]=16u;goto L8c6200ea; /* mov #0x10,r0 */
L8c6200ea: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200ec; /* fmov.s fr3,@-r8 */
L8c6200ec: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6200ee; /* fmov.s @(r0,r6),fr3 */
L8c6200ee: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6200f0; /* fmov.s fr3,@-r8 */
L8c6200f0: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6200f2; /* mov.l r3,@-r8 */
L8c6200f2: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6200f4; /* pref @r8 */
L8c6200f4: r[0]=28u;goto L8c6200f6; /* mov #0x1c,r0 */
L8c6200f6: r[3]=rd(m,bus,&io_status,0x8c620158u,4);if(io_status)return io_status;goto L8c6200f8; /* mov.l 0x8c620158,r3 */
L8c6200f8: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6200fa; /* fmov.s @(r0,r9),fr3 */
L8c6200fa: r[8]+=64u;goto L8c6200fc; /* add #0x40,r8 */
L8c6200fc: r[0]=24u;goto L8c6200fe; /* mov #0x18,r0 */
L8c6200fe: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620100; /* fmov.s fr3,@-r8 */
L8c620100: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620102; /* fmov.s @(r0,r9),fr3 */
L8c620102: r[0]=20u;goto L8c620104; /* mov #0x14,r0 */
L8c620104: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620106; /* fmov.s fr3,@-r8 */
L8c620106: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620108; /* fmov.s @(r0,r9),fr3 */
L8c620108: r[0]=16u;goto L8c62010a; /* mov #0x10,r0 */
L8c62010a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62010c; /* fmov.s fr3,@-r8 */
L8c62010c: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62010e; /* fmov.s @(r0,r9),fr3 */
L8c62010e: r[0]=24u;goto L8c620110; /* mov #0x18,r0 */
L8c620110: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620112; /* fmov.s fr3,@-r8 */
L8c620112: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c620114; /* fmov.s @(r0,r7),fr3 */
L8c620114: r[0]=20u;goto L8c620116; /* mov #0x14,r0 */
L8c620116: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620118; /* fmov.s fr3,@-r8 */
L8c620118: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c62011a; /* fmov.s @(r0,r7),fr3 */
L8c62011a: r[0]=16u;goto L8c62011c; /* mov #0x10,r0 */
L8c62011c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62011e; /* fmov.s fr3,@-r8 */
L8c62011e: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c620120; /* fmov.s @(r0,r7),fr3 */
L8c620120: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620122; /* fmov.s fr3,@-r8 */
L8c620122: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c620124; /* mov.l r3,@-r8 */
L8c620124: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c620126; /* pref @r8 */
L8c620126: r[2]=rd(m,bus,&io_status,0x8c62015cu,4);if(io_status)return io_status;goto L8c620128; /* mov.l 0x8c62015c,r2 */
L8c620128: r[8]+=32u;goto L8c62012a; /* add #0x20,r8 */
L8c62012a: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c62012c; /* mov.l @r2,r3 */
L8c62012c: r[3]+=3u;goto L8c62012e; /* add #0x3,r3 */
L8c62012e: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c620130; /* mov.l r3,@r2 */
L8c620130: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c620132; /* mov.w @(0x2,r4),r0 */
L8c620132: r[0]=r[0]&65535u;goto L8c620134; /* extu.w r0,r0 */
L8c620134: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c620136; /* cmp/ge r0,r14 */
L8c620136: if(!(*sr&1u))goto L8c62003c;goto L8c620138; /* bf 0x8c62003c */
L8c620138: goto L8c6203fa; /* bra 0x8c6203fa */
L8c620160: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c620162; /* mov.w @(0x2,r4),r0 */
L8c620162: r[2]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c620164; /* mov.l @r11,r2 */
L8c620164: r[0]=r[0]&65535u;goto L8c620166; /* extu.w r0,r0 */
L8c620166: r[0]<<=2;goto L8c620168; /* shll2 r0 */
L8c620168: r[2]+=r[0];goto L8c62016a; /* add r0,r2 */
L8c62016a: wr(m,bus,&io_status,r[11],r[2],4);if(io_status)return io_status;goto L8c62016c; /* mov.l r2,@r11 */
L8c62016c: r[11]=r[12];goto L8c6202a0; /* bra 0x8c6202a0 */
L8c620170: r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c620172; /* mov.w @r13+,r7 */
L8c620172: r[0]=16u;goto L8c620174; /* mov #0x10,r0 */
L8c620174: r[3]=rd(m,bus,&io_status,0x8c620410u,4);if(io_status)return io_status;goto L8c620176; /* mov.l 0x8c620410,r3 */
L8c620176: r[6]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c620178; /* mov.w @r13+,r6 */
L8c620178: r[7]=r[7]&65535u;goto L8c62017a; /* extu.w r7,r7 */
L8c62017a: r[12]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c62017c; /* mov.l @r3,r12 */
L8c62017c: r[7]<<=2;goto L8c62017e; /* shll2 r7 */
L8c62017e: r[6]=r[6]&65535u;goto L8c620180; /* extu.w r6,r6 */
L8c620180: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c620182; /* mov.w @r13+,r5 */
L8c620182: r[6]<<=2;goto L8c620184; /* shll2 r6 */
L8c620184: r[6]<<=2;goto L8c620186; /* shll2 r6 */
L8c620186: r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c620188; /* mov.w @r13+,r14 */
L8c620188: r[7]<<=2;goto L8c62018a; /* shll2 r7 */
L8c62018a: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c62018c; /* shll r7 */
L8c62018c: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c62018e; /* shll r6 */
L8c62018e: r[6]+=r[12];goto L8c620190; /* add r12,r6 */
L8c620190: r[7]+=r[12];goto L8c620192; /* add r12,r7 */
L8c620192: fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c620194; /* fmov.s @(r0,r6),fr4 */
L8c620194: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c620196; /* fmov.s @(r0,r7),fr3 */
L8c620196: r[5]=r[5]&65535u;goto L8c620198; /* extu.w r5,r5 */
L8c620198: fcopy(f,2u,4u);goto L8c62019a; /* fmov fr4,fr2 */
L8c62019a: r[5]<<=2;goto L8c62019c; /* shll2 r5 */
L8c62019c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c62019e; /* fsub fr3,fr2 */
L8c62019e: r[14]=r[14]&65535u;goto L8c6201a0; /* extu.w r14,r14 */
L8c6201a0: r[5]<<=2;goto L8c6201a2; /* shll2 r5 */
L8c6201a2: r[14]<<=2;goto L8c6201a4; /* shll2 r14 */
L8c6201a4: r[0]=20u;goto L8c6201a6; /* mov #0x14,r0 */
L8c6201a6: fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c6201a8; /* fmov.s @(r0,r6),fr5 */
L8c6201a8: r[14]<<=2;goto L8c6201aa; /* shll2 r14 */
L8c6201aa: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6201ac; /* shll r5 */
L8c6201ac: r[5]+=r[12];goto L8c6201ae; /* add r12,r5 */
L8c6201ae: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c6201b0; /* shll r14 */
L8c6201b0: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6201b2; /* fmov.s @(r0,r5),fr3 */
L8c6201b2: fcopy(f,1u,5u);goto L8c6201b4; /* fmov fr5,fr1 */
L8c6201b4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6201b6; /* fsub fr5,fr3 */
L8c6201b6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6201b8; /* fmul fr3,fr2 */
L8c6201b8: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6201ba; /* fmov.s @(r0,r7),fr3 */
L8c6201ba: r[0]=16u;goto L8c6201bc; /* mov #0x10,r0 */
L8c6201bc: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6201be; /* fsub fr3,fr1 */
L8c6201be: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6201c0; /* fmov.s @(r0,r5),fr3 */
L8c6201c0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6201c2; /* fsub fr4,fr3 */
L8c6201c2: fcopy(f,4u,2u);goto L8c6201c4; /* fmov fr2,fr4 */
L8c6201c4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c6201c6; /* fmul fr3,fr1 */
L8c6201c6: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c6201c8; /* fldi0 fr3 */
L8c6201c8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c6201ca; /* fsub fr1,fr4 */
L8c6201ca: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c6201cc; /* fcmp/gt fr3,fr4 */
L8c6201cc: take=(*sr&1u);r[14]+=r[12];if(take)goto L8c62029e;goto L8c6201d0; /* bt/s 0x8c62029e */
L8c6201d0: r[0]=28u;goto L8c6201d2; /* mov #0x1c,r0 */
L8c6201d2: r[3]=rd(m,bus,&io_status,0x8c620414u,4);if(io_status)return io_status;goto L8c6201d4; /* mov.l 0x8c620414,r3 */
L8c6201d4: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6201d6; /* fmov.s @(r0,r9),fr3 */
L8c6201d6: r[8]+=32u;goto L8c6201d8; /* add #0x20,r8 */
L8c6201d8: r[0]=24u;goto L8c6201da; /* mov #0x18,r0 */
L8c6201da: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201dc; /* fmov.s fr3,@-r8 */
L8c6201dc: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6201de; /* fmov.s @(r0,r9),fr3 */
L8c6201de: r[0]=20u;goto L8c6201e0; /* mov #0x14,r0 */
L8c6201e0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201e2; /* fmov.s fr3,@-r8 */
L8c6201e2: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6201e4; /* fmov.s @(r0,r9),fr3 */
L8c6201e4: r[0]=16u;goto L8c6201e6; /* mov #0x10,r0 */
L8c6201e6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201e8; /* fmov.s fr3,@-r8 */
L8c6201e8: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c6201ea; /* fmov.s @(r0,r9),fr3 */
L8c6201ea: r[0]=24u;goto L8c6201ec; /* mov #0x18,r0 */
L8c6201ec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201ee; /* fmov.s fr3,@-r8 */
L8c6201ee: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6201f0; /* fmov.s @(r0,r7),fr3 */
L8c6201f0: r[0]=20u;goto L8c6201f2; /* mov #0x14,r0 */
L8c6201f2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201f4; /* fmov.s fr3,@-r8 */
L8c6201f4: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6201f6; /* fmov.s @(r0,r7),fr3 */
L8c6201f6: r[0]=16u;goto L8c6201f8; /* mov #0x10,r0 */
L8c6201f8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201fa; /* fmov.s fr3,@-r8 */
L8c6201fa: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6201fc; /* fmov.s @(r0,r7),fr3 */
L8c6201fc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6201fe; /* fmov.s fr3,@-r8 */
L8c6201fe: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c620200; /* mov.l r3,@-r8 */
L8c620200: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c620202; /* pref @r8 */
L8c620202: r[0]=28u;goto L8c620204; /* mov #0x1c,r0 */
L8c620204: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620206; /* fmov.s @(r0,r9),fr3 */
L8c620206: r[8]+=64u;goto L8c620208; /* add #0x40,r8 */
L8c620208: r[0]=24u;goto L8c62020a; /* mov #0x18,r0 */
L8c62020a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62020c; /* fmov.s fr3,@-r8 */
L8c62020c: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62020e; /* fmov.s @(r0,r9),fr3 */
L8c62020e: r[0]=20u;goto L8c620210; /* mov #0x14,r0 */
L8c620210: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620212; /* fmov.s fr3,@-r8 */
L8c620212: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620214; /* fmov.s @(r0,r9),fr3 */
L8c620214: r[0]=16u;goto L8c620216; /* mov #0x10,r0 */
L8c620216: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620218; /* fmov.s fr3,@-r8 */
L8c620218: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62021a; /* fmov.s @(r0,r9),fr3 */
L8c62021a: r[0]=24u;goto L8c62021c; /* mov #0x18,r0 */
L8c62021c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62021e; /* fmov.s fr3,@-r8 */
L8c62021e: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620220; /* fmov.s @(r0,r6),fr3 */
L8c620220: r[0]=20u;goto L8c620222; /* mov #0x14,r0 */
L8c620222: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620224; /* fmov.s fr3,@-r8 */
L8c620224: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620226; /* fmov.s @(r0,r6),fr3 */
L8c620226: r[0]=16u;goto L8c620228; /* mov #0x10,r0 */
L8c620228: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62022a; /* fmov.s fr3,@-r8 */
L8c62022a: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c62022c; /* fmov.s @(r0,r6),fr3 */
L8c62022c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62022e; /* fmov.s fr3,@-r8 */
L8c62022e: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c620230; /* mov.l r3,@-r8 */
L8c620230: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c620232; /* pref @r8 */
L8c620232: r[0]=28u;goto L8c620234; /* mov #0x1c,r0 */
L8c620234: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620236; /* fmov.s @(r0,r9),fr3 */
L8c620236: r[8]+=64u;goto L8c620238; /* add #0x40,r8 */
L8c620238: r[0]=24u;goto L8c62023a; /* mov #0x18,r0 */
L8c62023a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62023c; /* fmov.s fr3,@-r8 */
L8c62023c: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62023e; /* fmov.s @(r0,r9),fr3 */
L8c62023e: r[0]=20u;goto L8c620240; /* mov #0x14,r0 */
L8c620240: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620242; /* fmov.s fr3,@-r8 */
L8c620242: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620244; /* fmov.s @(r0,r9),fr3 */
L8c620244: r[0]=16u;goto L8c620246; /* mov #0x10,r0 */
L8c620246: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620248; /* fmov.s fr3,@-r8 */
L8c620248: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62024a; /* fmov.s @(r0,r9),fr3 */
L8c62024a: r[0]=24u;goto L8c62024c; /* mov #0x18,r0 */
L8c62024c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62024e; /* fmov.s fr3,@-r8 */
L8c62024e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620250; /* fmov.s @(r0,r5),fr3 */
L8c620250: r[0]=20u;goto L8c620252; /* mov #0x14,r0 */
L8c620252: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620254; /* fmov.s fr3,@-r8 */
L8c620254: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620256; /* fmov.s @(r0,r5),fr3 */
L8c620256: r[0]=16u;goto L8c620258; /* mov #0x10,r0 */
L8c620258: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62025a; /* fmov.s fr3,@-r8 */
L8c62025a: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c62025c; /* fmov.s @(r0,r5),fr3 */
L8c62025c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62025e; /* fmov.s fr3,@-r8 */
L8c62025e: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c620260; /* mov.l r3,@-r8 */
L8c620260: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c620262; /* pref @r8 */
L8c620262: r[0]=28u;goto L8c620264; /* mov #0x1c,r0 */
L8c620264: r[3]=rd(m,bus,&io_status,0x8c620418u,4);if(io_status)return io_status;goto L8c620266; /* mov.l 0x8c620418,r3 */
L8c620266: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620268; /* fmov.s @(r0,r9),fr3 */
L8c620268: r[8]+=64u;goto L8c62026a; /* add #0x40,r8 */
L8c62026a: r[0]=24u;goto L8c62026c; /* mov #0x18,r0 */
L8c62026c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62026e; /* fmov.s fr3,@-r8 */
L8c62026e: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620270; /* fmov.s @(r0,r9),fr3 */
L8c620270: r[0]=20u;goto L8c620272; /* mov #0x14,r0 */
L8c620272: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620274; /* fmov.s fr3,@-r8 */
L8c620274: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620276; /* fmov.s @(r0,r9),fr3 */
L8c620276: r[0]=16u;goto L8c620278; /* mov #0x10,r0 */
L8c620278: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62027a; /* fmov.s fr3,@-r8 */
L8c62027a: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62027c; /* fmov.s @(r0,r9),fr3 */
L8c62027c: r[0]=24u;goto L8c62027e; /* mov #0x18,r0 */
L8c62027e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620280; /* fmov.s fr3,@-r8 */
L8c620280: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c620282; /* fmov.s @(r0,r14),fr3 */
L8c620282: r[0]=20u;goto L8c620284; /* mov #0x14,r0 */
L8c620284: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620286; /* fmov.s fr3,@-r8 */
L8c620286: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c620288; /* fmov.s @(r0,r14),fr3 */
L8c620288: r[0]=16u;goto L8c62028a; /* mov #0x10,r0 */
L8c62028a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62028c; /* fmov.s fr3,@-r8 */
L8c62028c: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c62028e; /* fmov.s @(r0,r14),fr3 */
L8c62028e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620290; /* fmov.s fr3,@-r8 */
L8c620290: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c620292; /* mov.l r3,@-r8 */
L8c620292: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c620294; /* pref @r8 */
L8c620294: r[2]=rd(m,bus,&io_status,0x8c62041cu,4);if(io_status)return io_status;goto L8c620296; /* mov.l 0x8c62041c,r2 */
L8c620296: r[8]+=32u;goto L8c620298; /* add #0x20,r8 */
L8c620298: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c62029a; /* mov.l @r2,r3 */
L8c62029a: r[3]+=4u;goto L8c62029c; /* add #0x4,r3 */
L8c62029c: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c62029e; /* mov.l r3,@r2 */
L8c62029e: r[11]+=1u;goto L8c6202a0; /* add #0x1,r11 */
L8c6202a0: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c6202a2; /* mov.w @(0x2,r4),r0 */
L8c6202a2: r[0]=r[0]&65535u;goto L8c6202a4; /* extu.w r0,r0 */
L8c6202a4: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c6202a6; /* cmp/ge r0,r11 */
L8c6202a6: if((*sr&1u))goto L8c6202ac;goto L8c6202a8; /* bt 0x8c6202ac */
L8c6202a8: goto L8c620170; /* bra 0x8c620170 */
L8c6202ac: goto L8c6203fa; /* bra 0x8c6203fa */
L8c6202b0: r[10]=r[12];goto L8c6203ba; /* bra 0x8c6203ba */
L8c6202b4: r[14]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c6202b6; /* mov.w @r13+,r14 */
L8c6202b6: r[3]=rd(m,bus,&io_status,r[11],4);if(io_status)return io_status;goto L8c6202b8; /* mov.l @r11,r3 */
L8c6202b8: r[14]=r[14]&65535u;goto L8c6202ba; /* extu.w r14,r14 */
L8c6202ba: r[2]=rd(m,bus,&io_status,0x8c620410u,4);if(io_status)return io_status;goto L8c6202bc; /* mov.l 0x8c620410,r2 */
L8c6202bc: r[3]+=r[14];goto L8c6202be; /* add r14,r3 */
L8c6202be: wr(m,bus,&io_status,r[11],r[3],4);if(io_status)return io_status;goto L8c6202c0; /* mov.l r3,@r11 */
L8c6202c0: r[0]=sx(m,bus,&io_status,r[13]+0x2u,2);if(io_status)return io_status;goto L8c6202c2; /* mov.w @(0x2,r13),r0 */
L8c6202c2: r[7]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;goto L8c6202c4; /* mov.w @r13,r7 */
L8c6202c4: r[5]=r[0]&65535u;goto L8c6202c6; /* extu.w r0,r5 */
L8c6202c6: r[0]=sx(m,bus,&io_status,r[13]+0x4u,2);if(io_status)return io_status;goto L8c6202c8; /* mov.w @(0x4,r13),r0 */
L8c6202c8: r[5]<<=2;goto L8c6202ca; /* shll2 r5 */
L8c6202ca: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6202cc; /* mov.l @r2,r3 */
L8c6202cc: r[7]=r[7]&65535u;goto L8c6202ce; /* extu.w r7,r7 */
L8c6202ce: r[7]<<=2;goto L8c6202d0; /* shll2 r7 */
L8c6202d0: r[5]<<=2;goto L8c6202d2; /* shll2 r5 */
L8c6202d2: r[6]=r[0]&65535u;goto L8c6202d4; /* extu.w r0,r6 */
L8c6202d4: r[7]<<=2;goto L8c6202d6; /* shll2 r7 */
L8c6202d6: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1;goto L8c6202d8; /* shll r7 */
L8c6202d8: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6202da; /* shll r5 */
L8c6202da: r[0]=16u;goto L8c6202dc; /* mov #0x10,r0 */
L8c6202dc: r[5]+=r[3];goto L8c6202de; /* add r3,r5 */
L8c6202de: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6202e0; /* fmov.s @(r0,r5),fr5 */
L8c6202e0: r[7]+=r[3];goto L8c6202e2; /* add r3,r7 */
L8c6202e2: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6202e4; /* fmov.s @(r0,r7),fr3 */
L8c6202e4: r[6]<<=2;goto L8c6202e6; /* shll2 r6 */
L8c6202e6: fcopy(f,2u,5u);goto L8c6202e8; /* fmov fr5,fr2 */
L8c6202e8: r[6]<<=2;goto L8c6202ea; /* shll2 r6 */
L8c6202ea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6202ec; /* fsub fr3,fr2 */
L8c6202ec: r[0]=20u;goto L8c6202ee; /* mov #0x14,r0 */
L8c6202ee: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c6202f0; /* shll r6 */
L8c6202f0: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c6202f2; /* fmov.s @(r0,r5),fr4 */
L8c6202f2: r[6]+=r[3];goto L8c6202f4; /* add r3,r6 */
L8c6202f4: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6202f6; /* fmov.s @(r0,r6),fr3 */
L8c6202f6: fcopy(f,1u,4u);goto L8c6202f8; /* fmov fr4,fr1 */
L8c6202f8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6202fa; /* fsub fr4,fr3 */
L8c6202fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6202fc; /* fmul fr3,fr2 */
L8c6202fc: fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c6202fe; /* fmov.s @(r0,r7),fr3 */
L8c6202fe: r[0]=16u;goto L8c620300; /* mov #0x10,r0 */
L8c620300: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c620302; /* fsub fr3,fr1 */
L8c620302: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620304; /* fmov.s @(r0,r6),fr3 */
L8c620304: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c620306; /* fsub fr5,fr3 */
L8c620306: fcopy(f,4u,2u);goto L8c620308; /* fmov fr2,fr4 */
L8c620308: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c62030a; /* fmul fr3,fr1 */
L8c62030a: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c62030c; /* fldi0 fr3 */
L8c62030c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c62030e; /* fsub fr1,fr4 */
L8c62030e: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c620310; /* fcmp/gt fr3,fr4 */
L8c620310: if(!(*sr&1u))goto L8c620318;goto L8c620312; /* bf 0x8c620318 */
L8c620312: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c620314; /* shll r14 */
L8c620314: r[13]+=r[14];goto L8c6203b8; /* bra 0x8c6203b8 */
L8c620318: r[3]=rd(m,bus,&io_status,0x8c62041cu,4);if(io_status)return io_status;goto L8c62031a; /* mov.l 0x8c62041c,r3 */
L8c62031a: r[7]=r[14];goto L8c62031c; /* mov r14,r7 */
L8c62031c: r[7]+=4294967295u;goto L8c62031e; /* add #-0x1,r7 */
L8c62031e: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c620320; /* mov.l @r3,r2 */
L8c620320: *sr=(*sr&~1u)|((int32_t)r[7]>0);goto L8c620322; /* cmp/pl r7 */
L8c620322: r[2]+=r[14];goto L8c620324; /* add r14,r2 */
L8c620324: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c620326; /* mov.l r2,@r3 */
L8c620326: take=!(*sr&1u);r[6]=r[12];if(take)goto L8c620374;goto L8c62032a; /* bf/s 0x8c620374 */
L8c62032a: r[0]=28u;goto L8c62032c; /* mov #0x1c,r0 */
L8c62032c: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c62032e; /* mov.w @r13+,r5 */
L8c62032e: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620330; /* fmov.s @(r0,r9),fr3 */
L8c620330: r[8]+=32u;goto L8c620332; /* add #0x20,r8 */
L8c620332: r[0]=24u;goto L8c620334; /* mov #0x18,r0 */
L8c620334: r[2]=rd(m,bus,&io_status,0x8c620410u,4);if(io_status)return io_status;goto L8c620336; /* mov.l 0x8c620410,r2 */
L8c620336: r[5]=r[5]&65535u;goto L8c620338; /* extu.w r5,r5 */
L8c620338: r[5]<<=2;goto L8c62033a; /* shll2 r5 */
L8c62033a: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c62033c; /* mov.l @r2,r3 */
L8c62033c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62033e; /* fmov.s fr3,@-r8 */
L8c62033e: r[5]<<=2;goto L8c620340; /* shll2 r5 */
L8c620340: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620342; /* fmov.s @(r0,r9),fr3 */
L8c620342: r[0]=20u;goto L8c620344; /* mov #0x14,r0 */
L8c620344: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c620346; /* shll r5 */
L8c620346: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620348; /* fmov.s fr3,@-r8 */
L8c620348: r[5]+=r[3];goto L8c62034a; /* add r3,r5 */
L8c62034a: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62034c; /* fmov.s @(r0,r9),fr3 */
L8c62034c: r[0]=16u;goto L8c62034e; /* mov #0x10,r0 */
L8c62034e: r[3]=rd(m,bus,&io_status,0x8c620414u,4);if(io_status)return io_status;goto L8c620350; /* mov.l 0x8c620414,r3 */
L8c620350: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620352; /* fmov.s fr3,@-r8 */
L8c620352: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620354; /* fmov.s @(r0,r9),fr3 */
L8c620354: r[0]=24u;goto L8c620356; /* mov #0x18,r0 */
L8c620356: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620358; /* fmov.s fr3,@-r8 */
L8c620358: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c62035a; /* fmov.s @(r0,r5),fr3 */
L8c62035a: r[0]=20u;goto L8c62035c; /* mov #0x14,r0 */
L8c62035c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62035e; /* fmov.s fr3,@-r8 */
L8c62035e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620360; /* fmov.s @(r0,r5),fr3 */
L8c620360: r[0]=16u;goto L8c620362; /* mov #0x10,r0 */
L8c620362: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620364; /* fmov.s fr3,@-r8 */
L8c620364: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620366; /* fmov.s @(r0,r5),fr3 */
L8c620366: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620368; /* fmov.s fr3,@-r8 */
L8c620368: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c62036a; /* mov.l r3,@-r8 */
L8c62036a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c62036c; /* pref @r8 */
L8c62036c: r[6]+=1u;goto L8c62036e; /* add #0x1,r6 */
L8c62036e: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[7]);goto L8c620370; /* cmp/ge r7,r6 */
L8c620370: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c62032a;goto L8c620374; /* bf/s 0x8c62032a */
L8c620374: r[0]=28u;goto L8c620376; /* mov #0x1c,r0 */
L8c620376: r[5]=sx(m,bus,&io_status,r[13],2);if(io_status)return io_status;r[13]+=2;goto L8c620378; /* mov.w @r13+,r5 */
L8c620378: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62037a; /* fmov.s @(r0,r9),fr3 */
L8c62037a: r[8]+=32u;goto L8c62037c; /* add #0x20,r8 */
L8c62037c: r[0]=24u;goto L8c62037e; /* mov #0x18,r0 */
L8c62037e: r[2]=rd(m,bus,&io_status,0x8c620410u,4);if(io_status)return io_status;goto L8c620380; /* mov.l 0x8c620410,r2 */
L8c620380: r[5]=r[5]&65535u;goto L8c620382; /* extu.w r5,r5 */
L8c620382: r[5]<<=2;goto L8c620384; /* shll2 r5 */
L8c620384: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c620386; /* mov.l @r2,r3 */
L8c620386: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620388; /* fmov.s fr3,@-r8 */
L8c620388: r[5]<<=2;goto L8c62038a; /* shll2 r5 */
L8c62038a: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62038c; /* fmov.s @(r0,r9),fr3 */
L8c62038c: r[0]=20u;goto L8c62038e; /* mov #0x14,r0 */
L8c62038e: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c620390; /* shll r5 */
L8c620390: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620392; /* fmov.s fr3,@-r8 */
L8c620392: r[5]+=r[3];goto L8c620394; /* add r3,r5 */
L8c620394: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c620396; /* fmov.s @(r0,r9),fr3 */
L8c620396: r[0]=16u;goto L8c620398; /* mov #0x10,r0 */
L8c620398: r[3]=rd(m,bus,&io_status,0x8c620418u,4);if(io_status)return io_status;goto L8c62039a; /* mov.l 0x8c620418,r3 */
L8c62039a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62039c; /* fmov.s fr3,@-r8 */
L8c62039c: fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c62039e; /* fmov.s @(r0,r9),fr3 */
L8c62039e: r[0]=24u;goto L8c6203a0; /* mov #0x18,r0 */
L8c6203a0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6203a2; /* fmov.s fr3,@-r8 */
L8c6203a2: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6203a4; /* fmov.s @(r0,r5),fr3 */
L8c6203a4: r[0]=20u;goto L8c6203a6; /* mov #0x14,r0 */
L8c6203a6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6203a8; /* fmov.s fr3,@-r8 */
L8c6203a8: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6203aa; /* fmov.s @(r0,r5),fr3 */
L8c6203aa: r[0]=16u;goto L8c6203ac; /* mov #0x10,r0 */
L8c6203ac: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6203ae; /* fmov.s fr3,@-r8 */
L8c6203ae: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c6203b0; /* fmov.s @(r0,r5),fr3 */
L8c6203b0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6203b2; /* fmov.s fr3,@-r8 */
L8c6203b2: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6203b4; /* mov.l r3,@-r8 */
L8c6203b4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6203b6; /* pref @r8 */
L8c6203b6: r[8]+=32u;goto L8c6203b8; /* add #0x20,r8 */
L8c6203b8: r[10]+=1u;goto L8c6203ba; /* add #0x1,r10 */
L8c6203ba: r[0]=sx(m,bus,&io_status,r[4]+0x2u,2);if(io_status)return io_status;goto L8c6203bc; /* mov.w @(0x2,r4),r0 */
L8c6203bc: r[0]=r[0]&65535u;goto L8c6203be; /* extu.w r0,r0 */
L8c6203be: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c6203c0; /* cmp/ge r0,r10 */
L8c6203c0: if((*sr&1u))goto L8c6203c6;goto L8c6203c2; /* bt 0x8c6203c6 */
L8c6203c2: goto L8c6202b4; /* bra 0x8c6202b4 */
L8c6203c6: goto L8c6203fa; /* bra 0x8c6203fa */
L8c6203ca: r[1]=rd(m,bus,&io_status,0x8c620420u,4);if(io_status)return io_status;goto L8c6203cc; /* mov.l 0x8c620420,r1 */
L8c6203cc: wr(m,bus,&io_status,r[1],r[4],4);if(io_status)return io_status;goto L8c6203ce; /* mov.l r4,@r1 */
L8c6203ce: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6203d0; /* frchg */
L8c6203d0: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c6203d2; /* fmov.s fr0,@-r15 */
L8c6203d2: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c6203d4; /* fmov.s fr1,@-r15 */
L8c6203d4: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c6203d6; /* fmov.s fr2,@-r15 */
L8c6203d6: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c6203d8; /* fmov.s fr3,@-r15 */
L8c6203d8: r[0]=rd(m,bus,&io_status,0x8c62040cu,4);if(io_status)return io_status;goto L8c6203da; /* mov.l 0x8c62040c,r0 */
L8c6203da: r[0]+=16u;goto L8c6203dc; /* add #0x10,r0 */
L8c6203dc: fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203de; /* fmov.s @r0+,fr0 */
L8c6203de: fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203e0; /* fmov.s @r0+,fr1 */
L8c6203e0: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203e2; /* fmov.s @r0+,fr2 */
L8c6203e2: fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c6203e4; /* fmov.s @r0,fr3 */
L8c6203e4: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6203e6; /* frchg */
L8c6203e6: r[2]=rd(m,bus,&io_status,0x8c620424u,4);if(io_status)return io_status;goto L8c6203e8; /* mov.l 0x8c620424,r2 */
L8c6203e8: r[3]=rd(m,bus,&io_status,0x8c620420u,4);if(io_status)return io_status;goto L8c6203ea; /* mov.l 0x8c620420,r3 */
L8c6203ea: target=r[2];*pr=0x8c6203eeu;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6203ee; /* jsr @r2 */
L8c6203ee: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6203f0; /* frchg */
L8c6203f0: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203f2; /* fmov.s @r15+,fr3 */
L8c6203f2: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203f4; /* fmov.s @r15+,fr2 */
L8c6203f4: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203f6; /* fmov.s @r15+,fr1 */
L8c6203f6: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6203f8; /* fmov.s @r15+,fr0 */
L8c6203f8: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c6203fa; /* frchg */
L8c6203fa: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6203fc; /* lds.l @r15+,PR */
L8c6203fc: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c6203fe; /* mov.l @r15+,r9 */
L8c6203fe: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620400; /* mov.l @r15+,r10 */
L8c620400: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620402; /* mov.l @r15+,r11 */
L8c620402: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620404; /* mov.l @r15+,r12 */
L8c620404: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620406; /* mov.l @r15+,r13 */
L8c620406: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_primitive_emit_clipped_8c620428(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c620428;
L8c620428: r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c62042a; /* mov.l r14,@-r15 */
L8c62042a: r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c62042c; /* mov.l r13,@-r15 */
L8c62042c: r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c62042e; /* mov.l r12,@-r15 */
L8c62042e: r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c620430; /* mov.l r11,@-r15 */
L8c620430: r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c620432; /* mov.l r10,@-r15 */
L8c620432: r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c620434; /* mov.l r9,@-r15 */
L8c620434: r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c620436; /* sts.l PR,@-r15 */
L8c620436: r[15]+=4294967288u;goto L8c620438; /* add #-0x8,r15 */
L8c620438: r[0]=r[4];goto L8c62043a; /* mov r4,r0 */
L8c62043a: r[3]=rd(m,bus,&io_status,0x8c62048cu,4);if(io_status)return io_status;goto L8c62043c; /* mov.l 0x8c62048c,r3 */
L8c62043c: wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c62043e; /* mov.l r4,@r15 */
L8c62043e: r[11]=r[4];goto L8c620440; /* mov r4,r11 */
L8c620440: r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c620442; /* mov.w @r0,r0 */
L8c620442: r[0]=r[0]&65535u;goto L8c620444; /* extu.w r0,r0 */
L8c620444: r[0]&=r[3];goto L8c620446; /* and r3,r0 */
L8c620446: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c620448; /* cmp/eq #0x0,r0 */
L8c620448: take=(*sr&1u);r[11]=rd(m,bus,&io_status,r[11]+0x4u,4);if(io_status)return io_status;if(take)goto L8c62046e;goto L8c62044c; /* bt/s 0x8c62046e */
L8c62044c: r[1]=sx(m,bus,&io_status,0x8c62048au,2);if(io_status)return io_status;goto L8c62044e; /* mov.w 0x8c62048a,r1 */
L8c62044e: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620450; /* cmp/eq r1,r0 */
L8c620450: if(!(*sr&1u))goto L8c620456;goto L8c620452; /* bf 0x8c620456 */
L8c620452: goto L8c6205c4; /* bra 0x8c6205c4 */
L8c620456: r[1]=rd(m,bus,&io_status,0x8c620490u,4);if(io_status)return io_status;goto L8c620458; /* mov.l 0x8c620490,r1 */
L8c620458: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c62045a; /* cmp/eq r1,r0 */
L8c62045a: if(!(*sr&1u))goto L8c620460;goto L8c62045c; /* bf 0x8c620460 */
L8c62045c: goto L8c62077a; /* bra 0x8c62077a */
L8c620460: r[1]=rd(m,bus,&io_status,0x8c62048cu,4);if(io_status)return io_status;goto L8c620462; /* mov.l 0x8c62048c,r1 */
L8c620462: *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c620464; /* cmp/eq r1,r0 */
L8c620464: if(!(*sr&1u))goto L8c62046a;goto L8c620466; /* bf 0x8c62046a */
L8c620466: goto L8c62095e; /* bra 0x8c62095e */
L8c62046a: goto L8c620990; /* bra 0x8c620990 */
L8c62046e: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c620470; /* mov.l @r15,r2 */
L8c620470: r[1]=rd(m,bus,&io_status,0x8c620494u,4);if(io_status)return io_status;goto L8c620472; /* mov.l 0x8c620494,r1 */
L8c620472: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c620474; /* mov.w @(0x2,r2),r0 */
L8c620474: r[0]=r[0]&65535u;goto L8c620476; /* extu.w r0,r0 */
L8c620476: r[2]=r[0];goto L8c620478; /* mov r0,r2 */
L8c620478: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c62047a; /* shll r0 */
L8c62047a: r[0]+=r[2];goto L8c62047c; /* add r2,r0 */
L8c62047c: r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c62047e; /* mov.l @r1,r2 */
L8c62047e: r[2]+=r[0];goto L8c620480; /* add r0,r2 */
L8c620480: wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c620482; /* mov.l r2,@r1 */
L8c620482: r[14]=0u;goto L8c6205b2; /* bra 0x8c6205b2 */
L8c620498: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c62049a; /* mov.w @r11+,r4 */
L8c62049a: r[3]=rd(m,bus,&io_status,0x8c620784u,4);if(io_status)return io_status;goto L8c62049c; /* mov.l 0x8c620784,r3 */
L8c62049c: r[5]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c62049e; /* mov.w @r11+,r5 */
L8c62049e: r[4]=r[4]&65535u;goto L8c6204a0; /* extu.w r4,r4 */
L8c6204a0: r[4]<<=2;goto L8c6204a2; /* shll2 r4 */
L8c6204a2: r[13]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6204a4; /* mov.l @r3,r13 */
L8c6204a4: r[6]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6204a6; /* mov.w @r11+,r6 */
L8c6204a6: r[5]=r[5]&65535u;goto L8c6204a8; /* extu.w r5,r5 */
L8c6204a8: r[5]<<=2;goto L8c6204aa; /* shll2 r5 */
L8c6204aa: r[6]=r[6]&65535u;goto L8c6204ac; /* extu.w r6,r6 */
L8c6204ac: r[5]<<=2;goto L8c6204ae; /* shll2 r5 */
L8c6204ae: r[6]<<=2;goto L8c6204b0; /* shll2 r6 */
L8c6204b0: r[4]<<=2;goto L8c6204b2; /* shll2 r4 */
L8c6204b2: r[6]<<=2;goto L8c6204b4; /* shll2 r6 */
L8c6204b4: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6204b6; /* shll r4 */
L8c6204b6: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c6204b8; /* shll r5 */
L8c6204b8: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c6204ba; /* shll r6 */
L8c6204ba: r[4]+=r[13];goto L8c6204bc; /* add r13,r4 */
L8c6204bc: r[5]+=r[13];goto L8c6204be; /* add r13,r5 */
L8c6204be: r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6204c0; /* mov.l @(0xc,r4),r7 */
L8c6204c0: r[2]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c6204c2; /* mov.l @(0xc,r5),r2 */
L8c6204c2: r[6]+=r[13];goto L8c6204c4; /* add r13,r6 */
L8c6204c4: r[1]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c6204c6; /* mov.l @(0xc,r6),r1 */
L8c6204c6: r[7]&=r[2];goto L8c6204c8; /* and r2,r7 */
L8c6204c8: r[2]=63u;goto L8c6204ca; /* mov #0x3f,r2 */
L8c6204ca: r[7]&=r[1];goto L8c6204cc; /* and r1,r7 */
L8c6204cc: *sr=(*sr&~1u)|!(r[2]&r[7]);goto L8c6204ce; /* tst r2,r7 */
L8c6204ce: if(!(*sr&1u))goto L8c6205b0;goto L8c6204d0; /* bf 0x8c6205b0 */
L8c6204d0: r[7]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c6204d2; /* mov.l @(0xc,r4),r7 */
L8c6204d2: r[3]=rd(m,bus,&io_status,r[5]+0xcu,4);if(io_status)return io_status;goto L8c6204d4; /* mov.l @(0xc,r5),r3 */
L8c6204d4: r[2]=rd(m,bus,&io_status,r[6]+0xcu,4);if(io_status)return io_status;goto L8c6204d6; /* mov.l @(0xc,r6),r2 */
L8c6204d6: r[7]|=r[3];goto L8c6204d8; /* or r3,r7 */
L8c6204d8: r[3]=1u;goto L8c6204da; /* mov #0x1,r3 */
L8c6204da: r[7]|=r[2];goto L8c6204dc; /* or r2,r7 */
L8c6204dc: *sr=(*sr&~1u)|!(r[3]&r[7]);goto L8c6204de; /* tst r3,r7 */
L8c6204de: if(!(*sr&1u))goto L8c6205a6;goto L8c6204e0; /* bf 0x8c6205a6 */
L8c6204e0: r[0]=16u;goto L8c6204e2; /* mov #0x10,r0 */
L8c6204e2: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c6204e4; /* fmov.s @(r0,r5),fr4 */
L8c6204e4: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6204e6; /* fmov.s @(r0,r4),fr3 */
L8c6204e6: r[0]=20u;goto L8c6204e8; /* mov #0x14,r0 */
L8c6204e8: fcopy(f,2u,4u);goto L8c6204ea; /* fmov fr4,fr2 */
L8c6204ea: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c6204ec; /* fsub fr3,fr2 */
L8c6204ec: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6204ee; /* fmov.s @(r0,r6),fr3 */
L8c6204ee: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c6204f0; /* fmov.s @(r0,r5),fr5 */
L8c6204f0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c6204f2; /* fsub fr5,fr3 */
L8c6204f2: fcopy(f,1u,5u);goto L8c6204f4; /* fmov fr5,fr1 */
L8c6204f4: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c6204f6; /* fmul fr3,fr2 */
L8c6204f6: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6204f8; /* fmov.s @(r0,r4),fr3 */
L8c6204f8: r[0]=16u;goto L8c6204fa; /* mov #0x10,r0 */
L8c6204fa: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c6204fc; /* fsub fr3,fr1 */
L8c6204fc: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c6204fe; /* fmov.s @(r0,r6),fr3 */
L8c6204fe: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c620500; /* fsub fr4,fr3 */
L8c620500: fcopy(f,4u,2u);goto L8c620502; /* fmov fr2,fr4 */
L8c620502: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c620504; /* fmul fr3,fr1 */
L8c620504: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c620506; /* fldi0 fr3 */
L8c620506: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c620508; /* fsub fr1,fr4 */
L8c620508: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c62050a; /* fcmp/gt fr3,fr4 */
L8c62050a: if((*sr&1u))goto L8c6205b0;goto L8c62050c; /* bt 0x8c6205b0 */
L8c62050c: r[3]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c62050e; /* mov.l 0x8c620788,r3 */
L8c62050e: r[8]+=32u;goto L8c620510; /* add #0x20,r8 */
L8c620510: r[2]=rd(m,bus,&io_status,0x8c62078cu,4);if(io_status)return io_status;goto L8c620512; /* mov.l 0x8c62078c,r2 */
L8c620512: r[0]=24u;goto L8c620514; /* mov #0x18,r0 */
L8c620514: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c620516; /* fmov.s @r3,fr3 */
L8c620516: r[3]+=4294967284u;goto L8c620518; /* add #-0xc,r3 */
L8c620518: r[1]=rd(m,bus,&io_status,0x8c620790u,4);if(io_status)return io_status;goto L8c62051a; /* mov.l 0x8c620790,r1 */
L8c62051a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62051c; /* fmov.s fr3,@-r8 */
L8c62051c: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c62051e; /* fmov.s @r2,fr3 */
L8c62051e: r[2]=rd(m,bus,&io_status,0x8c620794u,4);if(io_status)return io_status;goto L8c620520; /* mov.l 0x8c620794,r2 */
L8c620520: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620522; /* fmov.s fr3,@-r8 */
L8c620522: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620524; /* fmov.s @r1+,fr3 */
L8c620524: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620526; /* fmov.s fr3,@-r8 */
L8c620526: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620528; /* fmov.s @r3+,fr3 */
L8c620528: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62052a; /* fmov.s fr3,@-r8 */
L8c62052a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c62052c; /* fmov.s @(r0,r4),fr3 */
L8c62052c: r[0]=20u;goto L8c62052e; /* mov #0x14,r0 */
L8c62052e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620530; /* fmov.s fr3,@-r8 */
L8c620530: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620532; /* fmov.s @(r0,r4),fr3 */
L8c620532: r[0]=16u;goto L8c620534; /* mov #0x10,r0 */
L8c620534: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620536; /* fmov.s fr3,@-r8 */
L8c620536: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620538; /* fmov.s @(r0,r4),fr3 */
L8c620538: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62053a; /* fmov.s fr3,@-r8 */
L8c62053a: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c62053c; /* mov.l r2,@-r8 */
L8c62053c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c62053e; /* pref @r8 */
L8c62053e: r[2]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c620540; /* mov.l 0x8c620788,r2 */
L8c620540: r[8]+=64u;goto L8c620542; /* add #0x40,r8 */
L8c620542: r[0]=24u;goto L8c620544; /* mov #0x18,r0 */
L8c620544: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c620546; /* fmov.s @r2,fr3 */
L8c620546: r[2]+=4294967284u;goto L8c620548; /* add #-0xc,r2 */
L8c620548: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62054a; /* fmov.s fr3,@-r8 */
L8c62054a: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c62054c; /* fmov.s @r1,fr3 */
L8c62054c: r[1]=rd(m,bus,&io_status,0x8c620794u,4);if(io_status)return io_status;goto L8c62054e; /* mov.l 0x8c620794,r1 */
L8c62054e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620550; /* fmov.s fr3,@-r8 */
L8c620550: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620552; /* fmov.s @r3+,fr3 */
L8c620552: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620554; /* fmov.s fr3,@-r8 */
L8c620554: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620556; /* fmov.s @r2+,fr3 */
L8c620556: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620558; /* fmov.s fr3,@-r8 */
L8c620558: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c62055a; /* fmov.s @(r0,r5),fr3 */
L8c62055a: r[0]=20u;goto L8c62055c; /* mov #0x14,r0 */
L8c62055c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62055e; /* fmov.s fr3,@-r8 */
L8c62055e: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620560; /* fmov.s @(r0,r5),fr3 */
L8c620560: r[0]=16u;goto L8c620562; /* mov #0x10,r0 */
L8c620562: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620564; /* fmov.s fr3,@-r8 */
L8c620564: fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c620566; /* fmov.s @(r0,r5),fr3 */
L8c620566: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620568; /* fmov.s fr3,@-r8 */
L8c620568: r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c62056a; /* mov.l r1,@-r8 */
L8c62056a: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c62056c; /* pref @r8 */
L8c62056c: r[1]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c62056e; /* mov.l 0x8c620788,r1 */
L8c62056e: r[8]+=64u;goto L8c620570; /* add #0x40,r8 */
L8c620570: r[0]=24u;goto L8c620572; /* mov #0x18,r0 */
L8c620572: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c620574; /* fmov.s @r1,fr3 */
L8c620574: r[1]+=4294967284u;goto L8c620576; /* add #-0xc,r1 */
L8c620576: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620578; /* fmov.s fr3,@-r8 */
L8c620578: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c62057a; /* fmov.s @r3,fr3 */
L8c62057a: r[3]=rd(m,bus,&io_status,0x8c620798u,4);if(io_status)return io_status;goto L8c62057c; /* mov.l 0x8c620798,r3 */
L8c62057c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62057e; /* fmov.s fr3,@-r8 */
L8c62057e: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c620580; /* fmov.s @r2,fr3 */
L8c620580: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620582; /* fmov.s fr3,@-r8 */
L8c620582: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c620584; /* fmov.s @r1,fr3 */
L8c620584: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620586; /* fmov.s fr3,@-r8 */
L8c620586: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620588; /* fmov.s @(r0,r6),fr3 */
L8c620588: r[0]=20u;goto L8c62058a; /* mov #0x14,r0 */
L8c62058a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62058c; /* fmov.s fr3,@-r8 */
L8c62058c: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c62058e; /* fmov.s @(r0,r6),fr3 */
L8c62058e: r[0]=16u;goto L8c620590; /* mov #0x10,r0 */
L8c620590: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620592; /* fmov.s fr3,@-r8 */
L8c620592: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620594; /* fmov.s @(r0,r6),fr3 */
L8c620594: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620596; /* fmov.s fr3,@-r8 */
L8c620596: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c620598; /* mov.l r3,@-r8 */
L8c620598: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c62059a; /* pref @r8 */
L8c62059a: r[0]=rd(m,bus,&io_status,0x8c62079cu,4);if(io_status)return io_status;goto L8c62059c; /* mov.l 0x8c62079c,r0 */
L8c62059c: r[8]+=32u;goto L8c62059e; /* add #0x20,r8 */
L8c62059e: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c6205a0; /* mov.l @r0,r3 */
L8c6205a0: r[3]+=3u;goto L8c6205a2; /* add #0x3,r3 */
L8c6205a2: wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c6205b0; /* bra 0x8c6205b0 */
L8c6205a6: r[1]=rd(m,bus,&io_status,0x8c6207a0u,4);if(io_status)return io_status;goto L8c6205a8; /* mov.l 0x8c6207a0,r1 */
L8c6205a8: wr(m,bus,&io_status,r[1],r[3],4);if(io_status)return io_status;goto L8c6205aa; /* mov.l r3,@r1 */
L8c6205aa: r[3]=rd(m,bus,&io_status,0x8c6207a4u,4);if(io_status)return io_status;goto L8c6205ac; /* mov.l 0x8c6207a4,r3 */
L8c6205ac: target=r[3];*pr=0x8c6205b0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c6205b0; /* jsr @r3 */
L8c6205b0: r[14]+=1u;goto L8c6205b2; /* add #0x1,r14 */
L8c6205b2: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6205b4; /* mov.l @r15,r2 */
L8c6205b4: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c6205b6; /* mov.w @(0x2,r2),r0 */
L8c6205b6: r[0]=r[0]&65535u;goto L8c6205b8; /* extu.w r0,r0 */
L8c6205b8: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[0]);goto L8c6205ba; /* cmp/ge r0,r14 */
L8c6205ba: if((*sr&1u))goto L8c6205c0;goto L8c6205bc; /* bt 0x8c6205c0 */
L8c6205bc: goto L8c620498; /* bra 0x8c620498 */
L8c6205c0: goto L8c620990; /* bra 0x8c620990 */
L8c6205c4: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c6205c6; /* mov.l @r15,r1 */
L8c6205c6: r[2]=rd(m,bus,&io_status,0x8c6207a8u,4);if(io_status)return io_status;goto L8c6205c8; /* mov.l 0x8c6207a8,r2 */
L8c6205c8: r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c6205ca; /* mov.w @(0x2,r1),r0 */
L8c6205ca: r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6205cc; /* mov.l @r2,r1 */
L8c6205cc: r[0]=r[0]&65535u;goto L8c6205ce; /* extu.w r0,r0 */
L8c6205ce: r[0]<<=2;goto L8c6205d0; /* shll2 r0 */
L8c6205d0: r[1]+=r[0];goto L8c6205d2; /* add r0,r1 */
L8c6205d2: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c6205d4; /* mov.l r1,@r2 */
L8c6205d4: r[10]=0u;goto L8c620768; /* bra 0x8c620768 */
L8c6205d8: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6205da; /* mov.w @r11+,r4 */
L8c6205da: r[3]=rd(m,bus,&io_status,0x8c620784u,4);if(io_status)return io_status;goto L8c6205dc; /* mov.l 0x8c620784,r3 */
L8c6205dc: r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6205de; /* mov.w @r11+,r13 */
L8c6205de: r[4]=r[4]&65535u;goto L8c6205e0; /* extu.w r4,r4 */
L8c6205e0: r[4]<<=2;goto L8c6205e2; /* shll2 r4 */
L8c6205e2: r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6205e4; /* mov.l @r3,r6 */
L8c6205e4: r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6205e6; /* mov.w @r11+,r14 */
L8c6205e6: r[13]=r[13]&65535u;goto L8c6205e8; /* extu.w r13,r13 */
L8c6205e8: r[13]<<=2;goto L8c6205ea; /* shll2 r13 */
L8c6205ea: r[12]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6205ec; /* mov.w @r11+,r12 */
L8c6205ec: r[14]=r[14]&65535u;goto L8c6205ee; /* extu.w r14,r14 */
L8c6205ee: r[13]<<=2;goto L8c6205f0; /* shll2 r13 */
L8c6205f0: r[4]<<=2;goto L8c6205f2; /* shll2 r4 */
L8c6205f2: r[12]=r[12]&65535u;goto L8c6205f4; /* extu.w r12,r12 */
L8c6205f4: r[14]<<=2;goto L8c6205f6; /* shll2 r14 */
L8c6205f6: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6205f8; /* shll r4 */
L8c6205f8: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c6205fa; /* shll r13 */
L8c6205fa: r[4]+=r[6];goto L8c6205fc; /* add r6,r4 */
L8c6205fc: r[12]<<=2;goto L8c6205fe; /* shll2 r12 */
L8c6205fe: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c620600; /* mov.l @(0xc,r4),r5 */
L8c620600: r[13]+=r[6];goto L8c620602; /* add r6,r13 */
L8c620602: r[14]<<=2;goto L8c620604; /* shll2 r14 */
L8c620604: r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c620606; /* mov.l @(0xc,r13),r2 */
L8c620606: r[12]<<=2;goto L8c620608; /* shll2 r12 */
L8c620608: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c62060a; /* shll r14 */
L8c62060a: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c62060c; /* shll r12 */
L8c62060c: r[14]+=r[6];goto L8c62060e; /* add r6,r14 */
L8c62060e: r[1]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c620610; /* mov.l @(0xc,r14),r1 */
L8c620610: r[5]&=r[2];goto L8c620612; /* and r2,r5 */
L8c620612: r[12]+=r[6];goto L8c620614; /* add r6,r12 */
L8c620614: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c620616; /* mov.l @(0xc,r12),r2 */
L8c620616: r[5]&=r[1];goto L8c620618; /* and r1,r5 */
L8c620618: r[1]=63u;goto L8c62061a; /* mov #0x3f,r1 */
L8c62061a: r[5]&=r[2];goto L8c62061c; /* and r2,r5 */
L8c62061c: *sr=(*sr&~1u)|!(r[1]&r[5]);goto L8c62061e; /* tst r1,r5 */
L8c62061e: if((*sr&1u))goto L8c620624;goto L8c620620; /* bt 0x8c620624 */
L8c620620: goto L8c620766; /* bra 0x8c620766 */
L8c620624: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c620626; /* mov.l @(0xc,r4),r5 */
L8c620626: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c620628; /* mov.l @(0xc,r13),r3 */
L8c620628: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c62062a; /* mov.l @(0xc,r14),r2 */
L8c62062a: r[5]|=r[3];goto L8c62062c; /* or r3,r5 */
L8c62062c: r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c62062e; /* mov.l @(0xc,r12),r3 */
L8c62062e: r[5]|=r[2];goto L8c620630; /* or r2,r5 */
L8c620630: r[2]=1u;goto L8c620632; /* mov #0x1,r2 */
L8c620632: r[5]|=r[3];goto L8c620634; /* or r3,r5 */
L8c620634: *sr=(*sr&~1u)|!(r[2]&r[5]);goto L8c620636; /* tst r2,r5 */
L8c620636: if(!(*sr&1u))goto L8c620730;goto L8c620638; /* bf 0x8c620730 */
L8c620638: r[0]=16u;goto L8c62063a; /* mov #0x10,r0 */
L8c62063a: fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c62063c; /* fmov.s @(r0,r13),fr4 */
L8c62063c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c62063e; /* fmov.s @(r0,r4),fr3 */
L8c62063e: r[0]=20u;goto L8c620640; /* mov #0x14,r0 */
L8c620640: fcopy(f,2u,4u);goto L8c620642; /* fmov fr4,fr2 */
L8c620642: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c620644; /* fsub fr3,fr2 */
L8c620644: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c620646; /* fmov.s @(r0,r14),fr3 */
L8c620646: fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c620648; /* fmov.s @(r0,r13),fr5 */
L8c620648: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c62064a; /* fsub fr5,fr3 */
L8c62064a: fcopy(f,1u,5u);goto L8c62064c; /* fmov fr5,fr1 */
L8c62064c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c62064e; /* fmul fr3,fr2 */
L8c62064e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620650; /* fmov.s @(r0,r4),fr3 */
L8c620650: r[0]=16u;goto L8c620652; /* mov #0x10,r0 */
L8c620652: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c620654; /* fsub fr3,fr1 */
L8c620654: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c620656; /* fmov.s @(r0,r14),fr3 */
L8c620656: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c620658; /* fsub fr4,fr3 */
L8c620658: fcopy(f,4u,2u);goto L8c62065a; /* fmov fr2,fr4 */
L8c62065a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c62065c; /* fmul fr3,fr1 */
L8c62065c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c62065e; /* fldi0 fr3 */
L8c62065e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c620660; /* fsub fr1,fr4 */
L8c620660: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c620662; /* fcmp/gt fr3,fr4 */
L8c620662: if(!(*sr&1u))goto L8c620668;goto L8c620664; /* bf 0x8c620668 */
L8c620664: goto L8c620766; /* bra 0x8c620766 */
L8c620668: r[3]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c62066a; /* mov.l 0x8c620788,r3 */
L8c62066a: r[8]+=32u;goto L8c62066c; /* add #0x20,r8 */
L8c62066c: r[2]=rd(m,bus,&io_status,0x8c62078cu,4);if(io_status)return io_status;goto L8c62066e; /* mov.l 0x8c62078c,r2 */
L8c62066e: r[0]=24u;goto L8c620670; /* mov #0x18,r0 */
L8c620670: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c620672; /* fmov.s @r3,fr3 */
L8c620672: r[3]+=4294967284u;goto L8c620674; /* add #-0xc,r3 */
L8c620674: r[1]=rd(m,bus,&io_status,0x8c620790u,4);if(io_status)return io_status;goto L8c620676; /* mov.l 0x8c620790,r1 */
L8c620676: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620678; /* fmov.s fr3,@-r8 */
L8c620678: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c62067a; /* fmov.s @r2,fr3 */
L8c62067a: r[2]=rd(m,bus,&io_status,0x8c620794u,4);if(io_status)return io_status;goto L8c62067c; /* mov.l 0x8c620794,r2 */
L8c62067c: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62067e; /* fmov.s fr3,@-r8 */
L8c62067e: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620680; /* fmov.s @r1+,fr3 */
L8c620680: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620682; /* fmov.s fr3,@-r8 */
L8c620682: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620684; /* fmov.s @r3+,fr3 */
L8c620684: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620686; /* fmov.s fr3,@-r8 */
L8c620686: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620688; /* fmov.s @(r0,r4),fr3 */
L8c620688: r[0]=20u;goto L8c62068a; /* mov #0x14,r0 */
L8c62068a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62068c; /* fmov.s fr3,@-r8 */
L8c62068c: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c62068e; /* fmov.s @(r0,r4),fr3 */
L8c62068e: r[0]=16u;goto L8c620690; /* mov #0x10,r0 */
L8c620690: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620692; /* fmov.s fr3,@-r8 */
L8c620692: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620694; /* fmov.s @(r0,r4),fr3 */
L8c620694: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620696; /* fmov.s fr3,@-r8 */
L8c620696: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c620698; /* mov.l r2,@-r8 */
L8c620698: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c62069a; /* pref @r8 */
L8c62069a: r[2]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c62069c; /* mov.l 0x8c620788,r2 */
L8c62069c: r[8]+=64u;goto L8c62069e; /* add #0x40,r8 */
L8c62069e: r[0]=24u;goto L8c6206a0; /* mov #0x18,r0 */
L8c6206a0: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6206a2; /* fmov.s @r2,fr3 */
L8c6206a2: r[2]+=4294967284u;goto L8c6206a4; /* add #-0xc,r2 */
L8c6206a4: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206a6; /* fmov.s fr3,@-r8 */
L8c6206a6: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6206a8; /* fmov.s @r1,fr3 */
L8c6206a8: r[1]=rd(m,bus,&io_status,0x8c620794u,4);if(io_status)return io_status;goto L8c6206aa; /* mov.l 0x8c620794,r1 */
L8c6206aa: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206ac; /* fmov.s fr3,@-r8 */
L8c6206ac: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;r[3]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6206ae; /* fmov.s @r3+,fr3 */
L8c6206ae: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206b0; /* fmov.s fr3,@-r8 */
L8c6206b0: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6206b2; /* fmov.s @r2+,fr3 */
L8c6206b2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206b4; /* fmov.s fr3,@-r8 */
L8c6206b4: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6206b6; /* fmov.s @(r0,r13),fr3 */
L8c6206b6: r[0]=20u;goto L8c6206b8; /* mov #0x14,r0 */
L8c6206b8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206ba; /* fmov.s fr3,@-r8 */
L8c6206ba: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6206bc; /* fmov.s @(r0,r13),fr3 */
L8c6206bc: r[0]=16u;goto L8c6206be; /* mov #0x10,r0 */
L8c6206be: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206c0; /* fmov.s fr3,@-r8 */
L8c6206c0: fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c6206c2; /* fmov.s @(r0,r13),fr3 */
L8c6206c2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206c4; /* fmov.s fr3,@-r8 */
L8c6206c4: r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c6206c6; /* mov.l r1,@-r8 */
L8c6206c6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6206c8; /* pref @r8 */
L8c6206c8: r[1]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c6206ca; /* mov.l 0x8c620788,r1 */
L8c6206ca: r[8]+=64u;goto L8c6206cc; /* add #0x40,r8 */
L8c6206cc: r[0]=24u;goto L8c6206ce; /* mov #0x18,r0 */
L8c6206ce: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6206d0; /* fmov.s @r1,fr3 */
L8c6206d0: r[1]+=4294967284u;goto L8c6206d2; /* add #-0xc,r1 */
L8c6206d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206d4; /* fmov.s fr3,@-r8 */
L8c6206d4: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6206d6; /* fmov.s @r3,fr3 */
L8c6206d6: r[3]=rd(m,bus,&io_status,0x8c620794u,4);if(io_status)return io_status;goto L8c6206d8; /* mov.l 0x8c620794,r3 */
L8c6206d8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206da; /* fmov.s fr3,@-r8 */
L8c6206da: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;r[2]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6206dc; /* fmov.s @r2+,fr3 */
L8c6206dc: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206de; /* fmov.s fr3,@-r8 */
L8c6206de: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c6206e0; /* fmov.s @r1+,fr3 */
L8c6206e0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206e2; /* fmov.s fr3,@-r8 */
L8c6206e2: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6206e4; /* fmov.s @(r0,r14),fr3 */
L8c6206e4: r[0]=20u;goto L8c6206e6; /* mov #0x14,r0 */
L8c6206e6: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206e8; /* fmov.s fr3,@-r8 */
L8c6206e8: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6206ea; /* fmov.s @(r0,r14),fr3 */
L8c6206ea: r[0]=16u;goto L8c6206ec; /* mov #0x10,r0 */
L8c6206ec: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206ee; /* fmov.s fr3,@-r8 */
L8c6206ee: fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c6206f0; /* fmov.s @(r0,r14),fr3 */
L8c6206f0: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6206f2; /* fmov.s fr3,@-r8 */
L8c6206f2: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6206f4; /* mov.l r3,@-r8 */
L8c6206f4: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6206f6; /* pref @r8 */
L8c6206f6: r[3]=rd(m,bus,&io_status,0x8c620788u,4);if(io_status)return io_status;goto L8c6206f8; /* mov.l 0x8c620788,r3 */
L8c6206f8: r[8]+=64u;goto L8c6206fa; /* add #0x40,r8 */
L8c6206fa: r[0]=24u;goto L8c6206fc; /* mov #0x18,r0 */
L8c6206fc: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6206fe; /* fmov.s @r3,fr3 */
L8c6206fe: r[3]+=4294967284u;goto L8c620700; /* add #-0xc,r3 */
L8c620700: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620702; /* fmov.s fr3,@-r8 */
L8c620702: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c620704; /* fmov.s @r2,fr3 */
L8c620704: r[2]=rd(m,bus,&io_status,0x8c620798u,4);if(io_status)return io_status;goto L8c620706; /* mov.l 0x8c620798,r2 */
L8c620706: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620708; /* fmov.s fr3,@-r8 */
L8c620708: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c62070a; /* fmov.s @r1,fr3 */
L8c62070a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62070c; /* fmov.s fr3,@-r8 */
L8c62070c: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c62070e; /* fmov.s @r3,fr3 */
L8c62070e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620710; /* fmov.s fr3,@-r8 */
L8c620710: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c620712; /* fmov.s @(r0,r12),fr3 */
L8c620712: r[0]=20u;goto L8c620714; /* mov #0x14,r0 */
L8c620714: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620716; /* fmov.s fr3,@-r8 */
L8c620716: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c620718; /* fmov.s @(r0,r12),fr3 */
L8c620718: r[0]=16u;goto L8c62071a; /* mov #0x10,r0 */
L8c62071a: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62071c; /* fmov.s fr3,@-r8 */
L8c62071c: fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c62071e; /* fmov.s @(r0,r12),fr3 */
L8c62071e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620720; /* fmov.s fr3,@-r8 */
L8c620720: r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c620722; /* mov.l r2,@-r8 */
L8c620722: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c620724; /* pref @r8 */
L8c620724: r[0]=rd(m,bus,&io_status,0x8c62079cu,4);if(io_status)return io_status;goto L8c620726; /* mov.l 0x8c62079c,r0 */
L8c620726: r[8]+=32u;goto L8c620728; /* add #0x20,r8 */
L8c620728: r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c62072a; /* mov.l @r0,r2 */
L8c62072a: r[2]+=4u;goto L8c62072c; /* add #0x4,r2 */
L8c62072c: wr(m,bus,&io_status,r[0],r[2],4);if(io_status)return io_status;goto L8c620766; /* bra 0x8c620766 */
L8c620730: r[3]=rd(m,bus,&io_status,0x8c6207a0u,4);if(io_status)return io_status;goto L8c620732; /* mov.l 0x8c6207a0,r3 */
L8c620732: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c620734; /* mov.l r2,@r3 */
L8c620734: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c620736; /* mov.l @(0xc,r4),r5 */
L8c620736: r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c620738; /* mov.l @(0xc,r13),r3 */
L8c620738: r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c62073a; /* mov.l @(0xc,r14),r2 */
L8c62073a: r[5]&=r[3];goto L8c62073c; /* and r3,r5 */
L8c62073c: r[3]=1u;goto L8c62073e; /* mov #0x1,r3 */
L8c62073e: r[5]&=r[2];goto L8c620740; /* and r2,r5 */
L8c620740: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c620742; /* tst r3,r5 */
L8c620742: if(!(*sr&1u))goto L8c62074c;goto L8c620744; /* bf 0x8c62074c */
L8c620744: r[1]=rd(m,bus,&io_status,0x8c6207a4u,4);if(io_status)return io_status;goto L8c620746; /* mov.l 0x8c6207a4,r1 */
L8c620746: r[6]=r[14];goto L8c620748; /* mov r14,r6 */
L8c620748: target=r[1];*pr=0x8c62074cu;r[5]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c62074c; /* jsr @r1 */
L8c62074c: r[4]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c62074e; /* mov.l @(0xc,r13),r4 */
L8c62074e: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c620750; /* mov.l @(0xc,r14),r3 */
L8c620750: r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c620752; /* mov.l @(0xc,r12),r2 */
L8c620752: r[4]&=r[3];goto L8c620754; /* and r3,r4 */
L8c620754: r[3]=1u;goto L8c620756; /* mov #0x1,r3 */
L8c620756: r[4]&=r[2];goto L8c620758; /* and r2,r4 */
L8c620758: *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c62075a; /* tst r3,r4 */
L8c62075a: if(!(*sr&1u))goto L8c620766;goto L8c62075c; /* bf 0x8c620766 */
L8c62075c: r[1]=rd(m,bus,&io_status,0x8c6207a4u,4);if(io_status)return io_status;goto L8c62075e; /* mov.l 0x8c6207a4,r1 */
L8c62075e: r[5]=r[14];goto L8c620760; /* mov r14,r5 */
L8c620760: r[6]=r[12];goto L8c620762; /* mov r12,r6 */
L8c620762: target=r[1];*pr=0x8c620766u;r[4]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620766; /* jsr @r1 */
L8c620766: r[10]+=1u;goto L8c620768; /* add #0x1,r10 */
L8c620768: r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c62076a; /* mov.l @r15,r3 */
L8c62076a: r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c62076c; /* mov.w @(0x2,r3),r0 */
L8c62076c: r[0]=r[0]&65535u;goto L8c62076e; /* extu.w r0,r0 */
L8c62076e: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[0]);goto L8c620770; /* cmp/ge r0,r10 */
L8c620770: if((*sr&1u))goto L8c620776;goto L8c620772; /* bt 0x8c620776 */
L8c620772: goto L8c6205d8; /* bra 0x8c6205d8 */
L8c620776: goto L8c620990; /* bra 0x8c620990 */
L8c62077a: r[9]=0u;goto L8c62094c; /* bra 0x8c62094c */
L8c6207ac: r[3]=rd(m,bus,&io_status,0x8c6209ecu,4);if(io_status)return io_status;goto L8c6207ae; /* mov.l 0x8c6209ec,r3 */
L8c6207ae: r[7]=0u;goto L8c6207b0; /* mov #0x0,r7 */
L8c6207b0: r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6207b2; /* mov.w @r11+,r13 */
L8c6207b2: r[4]=0u;goto L8c6207b4; /* mov #0x0,r4 */
L8c6207b4: r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c6207b6; /* mov.l @r3,r2 */
L8c6207b6: r[5]=r[7];goto L8c6207b8; /* mov r7,r5 */
L8c6207b8: r[13]=r[13]&65535u;goto L8c6207ba; /* extu.w r13,r13 */
L8c6207ba: r[2]+=r[13];goto L8c6207bc; /* add r13,r2 */
L8c6207bc: *sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c6207be; /* cmp/pl r13 */
L8c6207be: wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c6207c0; /* mov.l r2,@r3 */
L8c6207c0: r[6]=4294967295u;goto L8c6207c2; /* mov #-0x1,r6 */
L8c6207c2: take=!(*sr&1u);r[4]+=r[11];if(take)goto L8c6207e4;goto L8c6207c6; /* bf/s 0x8c6207e4 */
L8c6207c6: r[14]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c6207c8; /* mov.w @r4+,r14 */
L8c6207c8: r[5]+=1u;goto L8c6207ca; /* add #0x1,r5 */
L8c6207ca: r[2]=rd(m,bus,&io_status,0x8c6209f0u,4);if(io_status)return io_status;goto L8c6207cc; /* mov.l 0x8c6209f0,r2 */
L8c6207cc: r[14]=r[14]&65535u;goto L8c6207ce; /* extu.w r14,r14 */
L8c6207ce: r[14]<<=2;goto L8c6207d0; /* shll2 r14 */
L8c6207d0: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6207d2; /* mov.l @r2,r3 */
L8c6207d2: r[14]<<=2;goto L8c6207d4; /* shll2 r14 */
L8c6207d4: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c6207d6; /* shll r14 */
L8c6207d6: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[13]);goto L8c6207d8; /* cmp/ge r13,r5 */
L8c6207d8: r[14]+=r[3];goto L8c6207da; /* add r3,r14 */
L8c6207da: wr(m,bus,&io_status,r[15]+0x4u,r[14],4);if(io_status)return io_status;goto L8c6207dc; /* mov.l r14,@(0x4,r15) */
L8c6207dc: r[14]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c6207de; /* mov.l @(0xc,r14),r14 */
L8c6207de: r[6]&=r[14];goto L8c6207e0; /* and r14,r6 */
L8c6207e0: take=!(*sr&1u);r[7]|=r[14];if(take)goto L8c6207c6;goto L8c6207e4; /* bf/s 0x8c6207c6 */
L8c6207e4: r[2]=63u;goto L8c6207e6; /* mov #0x3f,r2 */
L8c6207e6: *sr=(*sr&~1u)|!(r[2]&r[6]);goto L8c6207e8; /* tst r2,r6 */
L8c6207e8: if(!(*sr&1u))goto L8c620844;goto L8c6207ea; /* bf 0x8c620844 */
L8c6207ea: r[1]=1u;goto L8c6207ec; /* mov #0x1,r1 */
L8c6207ec: *sr=(*sr&~1u)|!(r[1]&r[7]);goto L8c6207ee; /* tst r1,r7 */
L8c6207ee: if(!(*sr&1u))goto L8c6208ec;goto L8c6207f0; /* bf 0x8c6208ec */
L8c6207f0: r[0]=sx(m,bus,&io_status,r[11]+0x2u,2);if(io_status)return io_status;goto L8c6207f2; /* mov.w @(0x2,r11),r0 */
L8c6207f2: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;goto L8c6207f4; /* mov.w @r11,r4 */
L8c6207f4: r[3]=rd(m,bus,&io_status,0x8c6209f0u,4);if(io_status)return io_status;goto L8c6207f6; /* mov.l 0x8c6209f0,r3 */
L8c6207f6: r[5]=r[0]&65535u;goto L8c6207f8; /* extu.w r0,r5 */
L8c6207f8: r[5]<<=2;goto L8c6207fa; /* shll2 r5 */
L8c6207fa: r[0]=sx(m,bus,&io_status,r[11]+0x4u,2);if(io_status)return io_status;goto L8c6207fc; /* mov.w @(0x4,r11),r0 */
L8c6207fc: r[4]=r[4]&65535u;goto L8c6207fe; /* extu.w r4,r4 */
L8c6207fe: r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c620800; /* mov.l @r3,r7 */
L8c620800: r[4]<<=2;goto L8c620802; /* shll2 r4 */
L8c620802: r[5]<<=2;goto L8c620804; /* shll2 r5 */
L8c620804: r[6]=r[0]&65535u;goto L8c620806; /* extu.w r0,r6 */
L8c620806: r[4]<<=2;goto L8c620808; /* shll2 r4 */
L8c620808: r[0]=16u;goto L8c62080a; /* mov #0x10,r0 */
L8c62080a: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c62080c; /* shll r4 */
L8c62080c: *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c62080e; /* shll r5 */
L8c62080e: r[5]+=r[7];goto L8c620810; /* add r7,r5 */
L8c620810: fload(m,bus,&io_status,f,5u,r[0]+r[5]);if(io_status)return io_status;goto L8c620812; /* fmov.s @(r0,r5),fr5 */
L8c620812: r[6]<<=2;goto L8c620814; /* shll2 r6 */
L8c620814: r[4]+=r[7];goto L8c620816; /* add r7,r4 */
L8c620816: r[6]<<=2;goto L8c620818; /* shll2 r6 */
L8c620818: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c62081a; /* fmov.s @(r0,r4),fr3 */
L8c62081a: fcopy(f,2u,5u);goto L8c62081c; /* fmov fr5,fr2 */
L8c62081c: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c62081e; /* shll r6 */
L8c62081e: r[0]=20u;goto L8c620820; /* mov #0x14,r0 */
L8c620820: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c620822; /* fsub fr3,fr2 */
L8c620822: r[6]+=r[7];goto L8c620824; /* add r7,r6 */
L8c620824: fload(m,bus,&io_status,f,4u,r[0]+r[5]);if(io_status)return io_status;goto L8c620826; /* fmov.s @(r0,r5),fr4 */
L8c620826: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620828; /* fmov.s @(r0,r6),fr3 */
L8c620828: fcopy(f,1u,4u);goto L8c62082a; /* fmov fr4,fr1 */
L8c62082a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c62082c; /* fsub fr4,fr3 */
L8c62082c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c62082e; /* fmul fr3,fr2 */
L8c62082e: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620830; /* fmov.s @(r0,r4),fr3 */
L8c620830: r[0]=16u;goto L8c620832; /* mov #0x10,r0 */
L8c620832: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c620834; /* fsub fr3,fr1 */
L8c620834: fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c620836; /* fmov.s @(r0,r6),fr3 */
L8c620836: fcopy(f,4u,2u);goto L8c620838; /* fmov fr2,fr4 */
L8c620838: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c62083a; /* fsub fr5,fr3 */
L8c62083a: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c62083c; /* fmul fr3,fr1 */
L8c62083c: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c62083e; /* fldi0 fr3 */
L8c62083e: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c620840; /* fsub fr1,fr4 */
L8c620840: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c620842; /* fcmp/gt fr3,fr4 */
L8c620842: if(!(*sr&1u))goto L8c62084a;goto L8c620844; /* bf 0x8c62084a */
L8c620844: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c620846; /* shll r13 */
L8c620846: r[11]+=r[13];goto L8c62094a; /* bra 0x8c62094a */
L8c62084a: r[2]=rd(m,bus,&io_status,0x8c6209f4u,4);if(io_status)return io_status;goto L8c62084c; /* mov.l 0x8c6209f4,r2 */
L8c62084c: r[6]=r[13];goto L8c62084e; /* mov r13,r6 */
L8c62084e: r[6]+=4294967295u;goto L8c620850; /* add #-0x1,r6 */
L8c620850: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c620852; /* mov.l @r2,r3 */
L8c620852: *sr=(*sr&~1u)|((int32_t)r[6]>0);goto L8c620854; /* cmp/pl r6 */
L8c620854: r[3]+=r[13];goto L8c620856; /* add r13,r3 */
L8c620856: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c620858; /* mov.l r3,@r2 */
L8c620858: take=!(*sr&1u);r[5]=0u;if(take)goto L8c6208a6;goto L8c62085c; /* bf/s 0x8c6208a6 */
L8c62085c: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c62085e; /* mov.w @r11+,r4 */
L8c62085e: r[8]+=32u;goto L8c620860; /* add #0x20,r8 */
L8c620860: r[2]=rd(m,bus,&io_status,0x8c6209f0u,4);if(io_status)return io_status;goto L8c620862; /* mov.l 0x8c6209f0,r2 */
L8c620862: r[0]=24u;goto L8c620864; /* mov #0x18,r0 */
L8c620864: r[4]=r[4]&65535u;goto L8c620866; /* extu.w r4,r4 */
L8c620866: r[1]=rd(m,bus,&io_status,0x8c6209f8u,4);if(io_status)return io_status;goto L8c620868; /* mov.l 0x8c6209f8,r1 */
L8c620868: r[4]<<=2;goto L8c62086a; /* shll2 r4 */
L8c62086a: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c62086c; /* mov.l @r2,r3 */
L8c62086c: r[4]<<=2;goto L8c62086e; /* shll2 r4 */
L8c62086e: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c620870; /* fmov.s @r1,fr3 */
L8c620870: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c620872; /* shll r4 */
L8c620872: r[2]=rd(m,bus,&io_status,0x8c620a00u,4);if(io_status)return io_status;goto L8c620874; /* mov.l 0x8c620a00,r2 */
L8c620874: r[4]+=r[3];goto L8c620876; /* add r3,r4 */
L8c620876: r[3]=rd(m,bus,&io_status,0x8c6209fcu,4);if(io_status)return io_status;goto L8c620878; /* mov.l 0x8c6209fc,r3 */
L8c620878: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62087a; /* fmov.s fr3,@-r8 */
L8c62087a: r[1]+=4294967284u;goto L8c62087c; /* add #-0xc,r1 */
L8c62087c: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c62087e; /* fmov.s @r3,fr3 */
L8c62087e: r[3]=rd(m,bus,&io_status,0x8c620a04u,4);if(io_status)return io_status;goto L8c620880; /* mov.l 0x8c620a04,r3 */
L8c620880: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620882; /* fmov.s fr3,@-r8 */
L8c620882: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c620884; /* fmov.s @r2,fr3 */
L8c620884: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620886; /* fmov.s fr3,@-r8 */
L8c620886: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c620888; /* fmov.s @r1,fr3 */
L8c620888: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62088a; /* fmov.s fr3,@-r8 */
L8c62088a: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c62088c; /* fmov.s @(r0,r4),fr3 */
L8c62088c: r[0]=20u;goto L8c62088e; /* mov #0x14,r0 */
L8c62088e: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620890; /* fmov.s fr3,@-r8 */
L8c620890: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620892; /* fmov.s @(r0,r4),fr3 */
L8c620892: r[0]=16u;goto L8c620894; /* mov #0x10,r0 */
L8c620894: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c620896; /* fmov.s fr3,@-r8 */
L8c620896: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c620898; /* fmov.s @(r0,r4),fr3 */
L8c620898: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c62089a; /* fmov.s fr3,@-r8 */
L8c62089a: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c62089c; /* mov.l r3,@-r8 */
L8c62089c: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c62089e; /* pref @r8 */
L8c62089e: r[5]+=1u;goto L8c6208a0; /* add #0x1,r5 */
L8c6208a0: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c6208a2; /* cmp/ge r6,r5 */
L8c6208a2: take=!(*sr&1u);r[8]+=32u;if(take)goto L8c62085c;goto L8c6208a6; /* bf/s 0x8c62085c */
L8c6208a6: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6208a8; /* mov.w @r11+,r4 */
L8c6208a8: r[8]+=32u;goto L8c6208aa; /* add #0x20,r8 */
L8c6208aa: r[2]=rd(m,bus,&io_status,0x8c6209f0u,4);if(io_status)return io_status;goto L8c6208ac; /* mov.l 0x8c6209f0,r2 */
L8c6208ac: r[0]=24u;goto L8c6208ae; /* mov #0x18,r0 */
L8c6208ae: r[4]=r[4]&65535u;goto L8c6208b0; /* extu.w r4,r4 */
L8c6208b0: r[1]=rd(m,bus,&io_status,0x8c6209f8u,4);if(io_status)return io_status;goto L8c6208b2; /* mov.l 0x8c6209f8,r1 */
L8c6208b2: r[4]<<=2;goto L8c6208b4; /* shll2 r4 */
L8c6208b4: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c6208b6; /* mov.l @r2,r3 */
L8c6208b6: r[4]<<=2;goto L8c6208b8; /* shll2 r4 */
L8c6208b8: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6208ba; /* fmov.s @r1,fr3 */
L8c6208ba: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c6208bc; /* shll r4 */
L8c6208bc: r[2]=rd(m,bus,&io_status,0x8c620a00u,4);if(io_status)return io_status;goto L8c6208be; /* mov.l 0x8c620a00,r2 */
L8c6208be: r[4]+=r[3];goto L8c6208c0; /* add r3,r4 */
L8c6208c0: r[3]=rd(m,bus,&io_status,0x8c6209fcu,4);if(io_status)return io_status;goto L8c6208c2; /* mov.l 0x8c6209fc,r3 */
L8c6208c2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208c4; /* fmov.s fr3,@-r8 */
L8c6208c4: r[1]+=4294967284u;goto L8c6208c6; /* add #-0xc,r1 */
L8c6208c6: fload(m,bus,&io_status,f,3u,r[3]);if(io_status)return io_status;goto L8c6208c8; /* fmov.s @r3,fr3 */
L8c6208c8: r[3]=rd(m,bus,&io_status,0x8c620a08u,4);if(io_status)return io_status;goto L8c6208ca; /* mov.l 0x8c620a08,r3 */
L8c6208ca: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208cc; /* fmov.s fr3,@-r8 */
L8c6208cc: fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c6208ce; /* fmov.s @r2,fr3 */
L8c6208ce: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208d0; /* fmov.s fr3,@-r8 */
L8c6208d0: fload(m,bus,&io_status,f,3u,r[1]);if(io_status)return io_status;goto L8c6208d2; /* fmov.s @r1,fr3 */
L8c6208d2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208d4; /* fmov.s fr3,@-r8 */
L8c6208d4: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6208d6; /* fmov.s @(r0,r4),fr3 */
L8c6208d6: r[0]=20u;goto L8c6208d8; /* mov #0x14,r0 */
L8c6208d8: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208da; /* fmov.s fr3,@-r8 */
L8c6208da: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6208dc; /* fmov.s @(r0,r4),fr3 */
L8c6208dc: r[0]=16u;goto L8c6208de; /* mov #0x10,r0 */
L8c6208de: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208e0; /* fmov.s fr3,@-r8 */
L8c6208e0: fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c6208e2; /* fmov.s @(r0,r4),fr3 */
L8c6208e2: r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c6208e4; /* fmov.s fr3,@-r8 */
L8c6208e4: r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c6208e6; /* mov.l r3,@-r8 */
L8c6208e6: if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c6208e8; /* pref @r8 */
L8c6208e8: r[8]+=32u;goto L8c62094a; /* bra 0x8c62094a */
L8c6208ec: r[2]=rd(m,bus,&io_status,0x8c620a0cu,4);if(io_status)return io_status;goto L8c6208ee; /* mov.l 0x8c620a0c,r2 */
L8c6208ee: r[3]=0u;goto L8c6208f0; /* mov #0x0,r3 */
L8c6208f0: r[0]=rd(m,bus,&io_status,0x8c6209f0u,4);if(io_status)return io_status;goto L8c6208f2; /* mov.l 0x8c6209f0,r0 */
L8c6208f2: r[10]=r[13];goto L8c6208f4; /* mov r13,r10 */
L8c6208f4: wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c6208f6; /* mov.l r3,@r2 */
L8c6208f6: r[10]+=4294967294u;goto L8c6208f8; /* add #-0x2,r10 */
L8c6208f8: r[4]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6208fa; /* mov.w @r11+,r4 */
L8c6208fa: r[3]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c6208fc; /* mov.l @r0,r3 */
L8c6208fc: r[14]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c6208fe; /* mov.w @r11+,r14 */
L8c6208fe: r[4]=r[4]&65535u;goto L8c620900; /* extu.w r4,r4 */
L8c620900: r[4]<<=2;goto L8c620902; /* shll2 r4 */
L8c620902: r[14]=r[14]&65535u;goto L8c620904; /* extu.w r14,r14 */
L8c620904: r[14]<<=2;goto L8c620906; /* shll2 r14 */
L8c620906: r[14]<<=2;goto L8c620908; /* shll2 r14 */
L8c620908: r[4]<<=2;goto L8c62090a; /* shll2 r4 */
L8c62090a: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c62090c; /* shll r4 */
L8c62090c: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c62090e; /* shll r14 */
L8c62090e: *sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8c620910; /* cmp/pl r10 */
L8c620910: r[4]+=r[3];goto L8c620912; /* add r3,r4 */
L8c620912: r[14]+=r[3];goto L8c620914; /* add r3,r14 */
L8c620914: take=!(*sr&1u);r[12]=0u;if(take)goto L8c62094a;goto L8c620918; /* bf/s 0x8c62094a */
L8c620918: r[13]=sx(m,bus,&io_status,r[11],2);if(io_status)return io_status;r[11]+=2;goto L8c62091a; /* mov.w @r11+,r13 */
L8c62091a: r[2]=rd(m,bus,&io_status,0x8c6209f0u,4);if(io_status)return io_status;goto L8c62091c; /* mov.l 0x8c6209f0,r2 */
L8c62091c: r[13]=r[13]&65535u;goto L8c62091e; /* extu.w r13,r13 */
L8c62091e: r[5]=rd(m,bus,&io_status,r[4]+0xcu,4);if(io_status)return io_status;goto L8c620920; /* mov.l @(0xc,r4),r5 */
L8c620920: r[13]<<=2;goto L8c620922; /* shll2 r13 */
L8c620922: r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c620924; /* mov.l @r2,r3 */
L8c620924: r[13]<<=2;goto L8c620926; /* shll2 r13 */
L8c620926: *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c620928; /* shll r13 */
L8c620928: r[13]+=r[3];goto L8c62092a; /* add r3,r13 */
L8c62092a: r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c62092c; /* mov.l @(0xc,r14),r3 */
L8c62092c: r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c62092e; /* mov.l @(0xc,r13),r1 */
L8c62092e: r[5]&=r[3];goto L8c620930; /* and r3,r5 */
L8c620930: r[3]=1u;goto L8c620932; /* mov #0x1,r3 */
L8c620932: r[5]&=r[1];goto L8c620934; /* and r1,r5 */
L8c620934: *sr=(*sr&~1u)|!(r[3]&r[5]);goto L8c620936; /* tst r3,r5 */
L8c620936: if(!(*sr&1u))goto L8c620940;goto L8c620938; /* bf 0x8c620940 */
L8c620938: r[1]=rd(m,bus,&io_status,0x8c620a10u,4);if(io_status)return io_status;goto L8c62093a; /* mov.l 0x8c620a10,r1 */
L8c62093a: r[6]=r[13];goto L8c62093c; /* mov r13,r6 */
L8c62093c: target=r[1];*pr=0x8c620940u;r[5]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620940; /* jsr @r1 */
L8c620940: r[12]+=1u;goto L8c620942; /* add #0x1,r12 */
L8c620942: r[4]=r[14];goto L8c620944; /* mov r14,r4 */
L8c620944: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[10]);goto L8c620946; /* cmp/ge r10,r12 */
L8c620946: take=!(*sr&1u);r[14]=r[13];if(take)goto L8c620918;goto L8c62094a; /* bf/s 0x8c620918 */
L8c62094a: r[9]+=1u;goto L8c62094c; /* add #0x1,r9 */
L8c62094c: r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c62094e; /* mov.l @r15,r2 */
L8c62094e: r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c620950; /* mov.w @(0x2,r2),r0 */
L8c620950: r[0]=r[0]&65535u;goto L8c620952; /* extu.w r0,r0 */
L8c620952: *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c620954; /* cmp/ge r0,r9 */
L8c620954: if((*sr&1u))goto L8c62095a;goto L8c620956; /* bt 0x8c62095a */
L8c620956: goto L8c6207ac; /* bra 0x8c6207ac */
L8c62095a: goto L8c620990; /* bra 0x8c620990 */
L8c62095e: r[2]=rd(m,bus,&io_status,0x8c620a14u,4);if(io_status)return io_status;goto L8c620960; /* mov.l 0x8c620a14,r2 */
L8c620960: r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c620962; /* mov.l @r15,r1 */
L8c620962: wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c620964; /* mov.l r1,@r2 */
L8c620964: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c620966; /* frchg */
L8c620966: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c620968; /* fmov.s fr0,@-r15 */
L8c620968: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c62096a; /* fmov.s fr1,@-r15 */
L8c62096a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c62096c; /* fmov.s fr2,@-r15 */
L8c62096c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c62096e; /* fmov.s fr3,@-r15 */
L8c62096e: r[0]=rd(m,bus,&io_status,0x8c6209a4u,4);if(io_status)return io_status;goto L8c620970; /* mov.l 0x8c6209a4,r0 */
L8c620970: r[0]+=16u;goto L8c620972; /* add #0x10,r0 */
L8c620972: fload(m,bus,&io_status,f,0u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620974; /* fmov.s @r0+,fr0 */
L8c620974: fload(m,bus,&io_status,f,1u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620976; /* fmov.s @r0+,fr1 */
L8c620976: fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620978; /* fmov.s @r0+,fr2 */
L8c620978: fload(m,bus,&io_status,f,3u,r[0]);if(io_status)return io_status;goto L8c62097a; /* fmov.s @r0,fr3 */
L8c62097a: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c62097c; /* frchg */
L8c62097c: r[2]=rd(m,bus,&io_status,0x8c620a18u,4);if(io_status)return io_status;goto L8c62097e; /* mov.l 0x8c620a18,r2 */
L8c62097e: r[3]=rd(m,bus,&io_status,0x8c620a14u,4);if(io_status)return io_status;goto L8c620980; /* mov.l 0x8c620a14,r3 */
L8c620980: target=r[2];*pr=0x8c620984u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c620984; /* jsr @r2 */
L8c620984: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c620986; /* frchg */
L8c620986: fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c620988; /* fmov.s @r15+,fr3 */
L8c620988: fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c62098a; /* fmov.s @r15+,fr2 */
L8c62098a: fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c62098c; /* fmov.s @r15+,fr1 */
L8c62098c: fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c62098e; /* fmov.s @r15+,fr0 */
L8c62098e: if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c620990; /* frchg */
L8c620990: r[15]+=8u;goto L8c620992; /* add #0x8,r15 */
L8c620992: *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620994; /* lds.l @r15+,PR */
L8c620994: r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620996; /* mov.l @r15+,r9 */
L8c620996: r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c620998; /* mov.l @r15+,r10 */
L8c620998: r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c62099a; /* mov.l @r15+,r11 */
L8c62099a: r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c62099c; /* mov.l @r15+,r12 */
L8c62099c: r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c62099e; /* mov.l @r15+,r13 */
L8c62099e: r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
