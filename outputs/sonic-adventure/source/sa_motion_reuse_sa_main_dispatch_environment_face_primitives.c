#include "sa_draw_motion_stop.h"
/* Environment UV/face-factor primitive family with portable queue interface; names inferred.
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
SA_STAGE_EXPORT int sa_environment_face_primitive_emit_8c61db98(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 uint32_t entry_pr=*pr;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61db98;
L8c61db98: sa_motion_publish_stop(ctx,0x8c61db98u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61db9a; /* mov.l r14,@-r15 */
L8c61db9a: sa_motion_publish_stop(ctx,0x8c61db9au); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61db9c; /* mov.l r13,@-r15 */
L8c61db9c: sa_motion_publish_stop(ctx,0x8c61db9cu); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61db9e; /* mov.l r12,@-r15 */
L8c61db9e: sa_motion_publish_stop(ctx,0x8c61db9eu); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61dba0; /* mov.l r11,@-r15 */
L8c61dba0: sa_motion_publish_stop(ctx,0x8c61dba0u); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61dba2; /* mov.l r10,@-r15 */
L8c61dba2: sa_motion_publish_stop(ctx,0x8c61dba2u); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61dba4; /* mov.l r9,@-r15 */
L8c61dba4: sa_motion_publish_stop(ctx,0x8c61dba4u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61dba6; /* fmov.s fr15,@-r15 */
L8c61dba6: sa_motion_publish_stop(ctx,0x8c61dba6u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61dba8; /* sts.l PR,@-r15 */
L8c61dba8: sa_motion_publish_stop(ctx,0x8c61dba8u); r[15]+=4294967280u;goto L8c61dbaa; /* add #-0x10,r15 */
L8c61dbaa: sa_motion_publish_stop(ctx,0x8c61dbaau); r[0]=0x8c61dc0cu;goto L8c61dbac; /* mova 0x8c61dc0c,r0 */
L8c61dbac: sa_motion_publish_stop(ctx,0x8c61dbacu); r[3]=rd(m,bus,&io_status,0x8c61dc14u,4);if(io_status)return io_status;goto L8c61dbae; /* mov.l 0x8c61dc14,r3 */
L8c61dbae: sa_motion_publish_stop(ctx,0x8c61dbaeu); r[10]=r[4];goto L8c61dbb0; /* mov r4,r10 */
L8c61dbb0: sa_motion_publish_stop(ctx,0x8c61dbb0u); r[13]=rd(m,bus,&io_status,0x8c61dc10u,4);if(io_status)return io_status;goto L8c61dbb2; /* mov.l 0x8c61dc10,r13 */
L8c61dbb2: sa_motion_publish_stop(ctx,0x8c61dbb2u); wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61dbb4; /* mov.l r4,@r15 */
L8c61dbb4: sa_motion_publish_stop(ctx,0x8c61dbb4u); fload(m,bus,&io_status,f,15u,r[0]);if(io_status)return io_status;goto L8c61dbb6; /* fmov.s @r0,fr15 */
L8c61dbb6: sa_motion_publish_stop(ctx,0x8c61dbb6u); r[0]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61dbb8; /* mov.l @r15,r0 */
L8c61dbb8: sa_motion_publish_stop(ctx,0x8c61dbb8u); r[10]=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;goto L8c61dbba; /* mov.l @(0x4,r10),r10 */
L8c61dbba: sa_motion_publish_stop(ctx,0x8c61dbbau); r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61dbbc; /* mov.w @r0,r0 */
L8c61dbbc: sa_motion_publish_stop(ctx,0x8c61dbbcu); r[0]=r[0]&65535u;goto L8c61dbbe; /* extu.w r0,r0 */
L8c61dbbe: sa_motion_publish_stop(ctx,0x8c61dbbeu); r[0]&=r[3];goto L8c61dbc0; /* and r3,r0 */
L8c61dbc0: sa_motion_publish_stop(ctx,0x8c61dbc0u); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61dbc2; /* cmp/eq #0x0,r0 */
L8c61dbc2: sa_motion_publish_stop(ctx,0x8c61dbc4u); take=(*sr&1u);r[4]=0u;if(take)goto L8c61dbe8;goto L8c61dbc6; /* bt/s 0x8c61dbe8 */
L8c61dbc6: sa_motion_publish_stop(ctx,0x8c61dbc6u); r[1]=sx(m,bus,&io_status,0x8c61dc00u,2);if(io_status)return io_status;goto L8c61dbc8; /* mov.w 0x8c61dc00,r1 */
L8c61dbc8: sa_motion_publish_stop(ctx,0x8c61dbc8u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61dbca; /* cmp/eq r1,r0 */
L8c61dbca: sa_motion_publish_stop(ctx,0x8c61dbcau); if(!(*sr&1u))goto L8c61dbd0;goto L8c61dbcc; /* bf 0x8c61dbd0 */
L8c61dbcc: sa_motion_publish_stop(ctx,0x8c61dbceu); goto L8c61de40; /* bra 0x8c61de40 */
L8c61dbd0: sa_motion_publish_stop(ctx,0x8c61dbd0u); r[1]=rd(m,bus,&io_status,0x8c61dc18u,4);if(io_status)return io_status;goto L8c61dbd2; /* mov.l 0x8c61dc18,r1 */
L8c61dbd2: sa_motion_publish_stop(ctx,0x8c61dbd2u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61dbd4; /* cmp/eq r1,r0 */
L8c61dbd4: sa_motion_publish_stop(ctx,0x8c61dbd4u); if(!(*sr&1u))goto L8c61dbda;goto L8c61dbd6; /* bf 0x8c61dbda */
L8c61dbd6: sa_motion_publish_stop(ctx,0x8c61dbd8u); goto L8c61e122; /* bra 0x8c61e122 */
L8c61dbda: sa_motion_publish_stop(ctx,0x8c61dbdau); r[1]=rd(m,bus,&io_status,0x8c61dc14u,4);if(io_status)return io_status;goto L8c61dbdc; /* mov.l 0x8c61dc14,r1 */
L8c61dbdc: sa_motion_publish_stop(ctx,0x8c61dbdcu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61dbde; /* cmp/eq r1,r0 */
L8c61dbde: sa_motion_publish_stop(ctx,0x8c61dbdeu); if(!(*sr&1u))goto L8c61dbe4;goto L8c61dbe0; /* bf 0x8c61dbe4 */
L8c61dbe0: sa_motion_publish_stop(ctx,0x8c61dbe2u); goto L8c61e42e; /* bra 0x8c61e42e */
L8c61dbe4: sa_motion_publish_stop(ctx,0x8c61dbe6u); goto L8c61e4a8; /* bra 0x8c61e4a8 */
L8c61dbe8: sa_motion_publish_stop(ctx,0x8c61dbe8u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61dbea; /* mov.l @r15,r2 */
L8c61dbea: sa_motion_publish_stop(ctx,0x8c61dbeau); r[1]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61dbec; /* mov.l @r13,r1 */
L8c61dbec: sa_motion_publish_stop(ctx,0x8c61dbecu); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61dbee; /* mov.w @(0x2,r2),r0 */
L8c61dbee: sa_motion_publish_stop(ctx,0x8c61dbeeu); r[0]=r[0]&65535u;goto L8c61dbf0; /* extu.w r0,r0 */
L8c61dbf0: sa_motion_publish_stop(ctx,0x8c61dbf0u); r[2]=r[0];goto L8c61dbf2; /* mov r0,r2 */
L8c61dbf2: sa_motion_publish_stop(ctx,0x8c61dbf2u); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61dbf4; /* shll r0 */
L8c61dbf4: sa_motion_publish_stop(ctx,0x8c61dbf4u); r[0]+=r[2];goto L8c61dbf6; /* add r2,r0 */
L8c61dbf6: sa_motion_publish_stop(ctx,0x8c61dbf6u); r[1]+=r[0];goto L8c61dbf8; /* add r0,r1 */
L8c61dbf8: sa_motion_publish_stop(ctx,0x8c61dbf8u); wr(m,bus,&io_status,r[13],r[1],4);if(io_status)return io_status;goto L8c61dbfa; /* mov.l r1,@r13 */
L8c61dbfa: sa_motion_publish_stop(ctx,0x8c61dbfcu); r[11]=r[4];goto L8c61de2e; /* bra 0x8c61de2e */
L8c61dc1c: sa_motion_publish_stop(ctx,0x8c61dc1cu); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61dc1e; /* mov.w @r10+,r12 */
L8c61dc1e: sa_motion_publish_stop(ctx,0x8c61dc1eu); r[0]=16u;goto L8c61dc20; /* mov #0x10,r0 */
L8c61dc20: sa_motion_publish_stop(ctx,0x8c61dc20u); r[3]=rd(m,bus,&io_status,0x8c61de58u,4);if(io_status)return io_status;goto L8c61dc22; /* mov.l 0x8c61de58,r3 */
L8c61dc22: sa_motion_publish_stop(ctx,0x8c61dc22u); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61dc24; /* mov.w @r10+,r13 */
L8c61dc24: sa_motion_publish_stop(ctx,0x8c61dc24u); r[12]=r[12]&65535u;goto L8c61dc26; /* extu.w r12,r12 */
L8c61dc26: sa_motion_publish_stop(ctx,0x8c61dc26u); r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61dc28; /* mov.l @r3,r4 */
L8c61dc28: sa_motion_publish_stop(ctx,0x8c61dc28u); r[12]<<=2;goto L8c61dc2a; /* shll2 r12 */
L8c61dc2a: sa_motion_publish_stop(ctx,0x8c61dc2au); r[13]=r[13]&65535u;goto L8c61dc2c; /* extu.w r13,r13 */
L8c61dc2c: sa_motion_publish_stop(ctx,0x8c61dc2cu); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61dc2e; /* mov.w @r10+,r14 */
L8c61dc2e: sa_motion_publish_stop(ctx,0x8c61dc2eu); r[13]<<=2;goto L8c61dc30; /* shll2 r13 */
L8c61dc30: sa_motion_publish_stop(ctx,0x8c61dc30u); r[13]<<=2;goto L8c61dc32; /* shll2 r13 */
L8c61dc32: sa_motion_publish_stop(ctx,0x8c61dc32u); r[12]<<=2;goto L8c61dc34; /* shll2 r12 */
L8c61dc34: sa_motion_publish_stop(ctx,0x8c61dc34u); r[14]=r[14]&65535u;goto L8c61dc36; /* extu.w r14,r14 */
L8c61dc36: sa_motion_publish_stop(ctx,0x8c61dc36u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61dc38; /* shll r12 */
L8c61dc38: sa_motion_publish_stop(ctx,0x8c61dc38u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61dc3a; /* shll r13 */
L8c61dc3a: sa_motion_publish_stop(ctx,0x8c61dc3au); r[13]+=r[4];goto L8c61dc3c; /* add r4,r13 */
L8c61dc3c: sa_motion_publish_stop(ctx,0x8c61dc3cu); fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dc3e; /* fmov.s @(r0,r13),fr5 */
L8c61dc3e: sa_motion_publish_stop(ctx,0x8c61dc3eu); r[12]+=r[4];goto L8c61dc40; /* add r4,r12 */
L8c61dc40: sa_motion_publish_stop(ctx,0x8c61dc40u); r[14]<<=2;goto L8c61dc42; /* shll2 r14 */
L8c61dc42: sa_motion_publish_stop(ctx,0x8c61dc42u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dc44; /* fmov.s @(r0,r12),fr3 */
L8c61dc44: sa_motion_publish_stop(ctx,0x8c61dc44u); r[14]<<=2;goto L8c61dc46; /* shll2 r14 */
L8c61dc46: sa_motion_publish_stop(ctx,0x8c61dc46u); fcopy(f,2u,5u);goto L8c61dc48; /* fmov fr5,fr2 */
L8c61dc48: sa_motion_publish_stop(ctx,0x8c61dc48u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61dc4a; /* shll r14 */
L8c61dc4a: sa_motion_publish_stop(ctx,0x8c61dc4au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61dc4c; /* fsub fr3,fr2 */
L8c61dc4c: sa_motion_publish_stop(ctx,0x8c61dc4cu); r[0]=20u;goto L8c61dc4e; /* mov #0x14,r0 */
L8c61dc4e: sa_motion_publish_stop(ctx,0x8c61dc4eu); r[14]+=r[4];goto L8c61dc50; /* add r4,r14 */
L8c61dc50: sa_motion_publish_stop(ctx,0x8c61dc50u); fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dc52; /* fmov.s @(r0,r13),fr4 */
L8c61dc52: sa_motion_publish_stop(ctx,0x8c61dc52u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dc54; /* fmov.s @(r0,r14),fr3 */
L8c61dc54: sa_motion_publish_stop(ctx,0x8c61dc54u); fcopy(f,1u,4u);goto L8c61dc56; /* fmov fr4,fr1 */
L8c61dc56: sa_motion_publish_stop(ctx,0x8c61dc56u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61dc58; /* fsub fr4,fr3 */
L8c61dc58: sa_motion_publish_stop(ctx,0x8c61dc58u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dc5a; /* fmul fr3,fr2 */
L8c61dc5a: sa_motion_publish_stop(ctx,0x8c61dc5au); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dc5c; /* fmov.s @(r0,r12),fr3 */
L8c61dc5c: sa_motion_publish_stop(ctx,0x8c61dc5cu); r[0]=16u;goto L8c61dc5e; /* mov #0x10,r0 */
L8c61dc5e: sa_motion_publish_stop(ctx,0x8c61dc5eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61dc60; /* fsub fr3,fr1 */
L8c61dc60: sa_motion_publish_stop(ctx,0x8c61dc60u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dc62; /* fmov.s @(r0,r14),fr3 */
L8c61dc62: sa_motion_publish_stop(ctx,0x8c61dc62u); fcopy(f,4u,2u);goto L8c61dc64; /* fmov fr2,fr4 */
L8c61dc64: sa_motion_publish_stop(ctx,0x8c61dc64u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61dc66; /* fsub fr5,fr3 */
L8c61dc66: sa_motion_publish_stop(ctx,0x8c61dc66u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61dc68; /* fmul fr3,fr1 */
L8c61dc68: sa_motion_publish_stop(ctx,0x8c61dc68u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61dc6a; /* fldi0 fr3 */
L8c61dc6a: sa_motion_publish_stop(ctx,0x8c61dc6au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61dc6c; /* fsub fr1,fr4 */
L8c61dc6c: sa_motion_publish_stop(ctx,0x8c61dc6cu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61dc6e; /* fcmp/gt fr3,fr4 */
L8c61dc6e: sa_motion_publish_stop(ctx,0x8c61dc6eu); if(!(*sr&1u))goto L8c61dc74;goto L8c61dc70; /* bf 0x8c61dc74 */
L8c61dc70: sa_motion_publish_stop(ctx,0x8c61dc72u); goto L8c61de2c; /* bra 0x8c61de2c */
L8c61dc74: sa_motion_publish_stop(ctx,0x8c61dc74u); fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61dc76; /* fmov.s @r12,fr2 */
L8c61dc76: sa_motion_publish_stop(ctx,0x8c61dc76u); r[0]=4u;goto L8c61dc78; /* mov #0x4,r0 */
L8c61dc78: sa_motion_publish_stop(ctx,0x8c61dc78u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61dc7a; /* fmov.s @r13,fr3 */
L8c61dc7a: sa_motion_publish_stop(ctx,0x8c61dc7au); r[4]=r[15];goto L8c61dc7c; /* mov r15,r4 */
L8c61dc7c: sa_motion_publish_stop(ctx,0x8c61dc7cu); r[3]=rd(m,bus,&io_status,0x8c61de5cu,4);if(io_status)return io_status;goto L8c61dc7e; /* mov.l 0x8c61de5c,r3 */
L8c61dc7e: sa_motion_publish_stop(ctx,0x8c61dc7eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61dc80; /* fadd fr3,fr2 */
L8c61dc80: sa_motion_publish_stop(ctx,0x8c61dc80u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61dc82; /* fmov.s @r14,fr3 */
L8c61dc82: sa_motion_publish_stop(ctx,0x8c61dc82u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61dc84; /* fadd fr3,fr2 */
L8c61dc84: sa_motion_publish_stop(ctx,0x8c61dc84u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61dc86; /* fmov fr2,@(r0,r15) */
L8c61dc86: sa_motion_publish_stop(ctx,0x8c61dc86u); r[0]=4u;goto L8c61dc88; /* mov #0x4,r0 */
L8c61dc88: sa_motion_publish_stop(ctx,0x8c61dc88u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dc8a; /* fmov.s @(r0,r12),fr2 */
L8c61dc8a: sa_motion_publish_stop(ctx,0x8c61dc8au); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dc8c; /* fmov.s @(r0,r13),fr3 */
L8c61dc8c: sa_motion_publish_stop(ctx,0x8c61dc8cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61dc8e; /* fadd fr3,fr2 */
L8c61dc8e: sa_motion_publish_stop(ctx,0x8c61dc8eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dc90; /* fmov.s @(r0,r14),fr3 */
L8c61dc90: sa_motion_publish_stop(ctx,0x8c61dc90u); r[0]=8u;goto L8c61dc92; /* mov #0x8,r0 */
L8c61dc92: sa_motion_publish_stop(ctx,0x8c61dc92u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61dc94; /* fadd fr3,fr2 */
L8c61dc94: sa_motion_publish_stop(ctx,0x8c61dc94u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61dc96; /* fmov fr2,@(r0,r15) */
L8c61dc96: sa_motion_publish_stop(ctx,0x8c61dc96u); r[0]=8u;goto L8c61dc98; /* mov #0x8,r0 */
L8c61dc98: sa_motion_publish_stop(ctx,0x8c61dc98u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dc9a; /* fmov.s @(r0,r12),fr2 */
L8c61dc9a: sa_motion_publish_stop(ctx,0x8c61dc9au); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dc9c; /* fmov.s @(r0,r13),fr3 */
L8c61dc9c: sa_motion_publish_stop(ctx,0x8c61dc9cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61dc9e; /* fadd fr3,fr2 */
L8c61dc9e: sa_motion_publish_stop(ctx,0x8c61dc9eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dca0; /* fmov.s @(r0,r14),fr3 */
L8c61dca0: sa_motion_publish_stop(ctx,0x8c61dca0u); r[0]=12u;goto L8c61dca2; /* mov #0xc,r0 */
L8c61dca2: sa_motion_publish_stop(ctx,0x8c61dca2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61dca4; /* fadd fr3,fr2 */
L8c61dca4: sa_motion_publish_stop(ctx,0x8c61dca4u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61dca6; /* fmov fr2,@(r0,r15) */
L8c61dca6: sa_motion_publish_stop(ctx,0x8c61dca8u); target=r[3];*pr=0x8c61dcaau;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61dcaa; /* jsr @r3 */
L8c61dcaa: sa_motion_publish_stop(ctx,0x8c61dcaau); r[3]=rd(m,bus,&io_status,0x8c61de60u,4);if(io_status)return io_status;goto L8c61dcac; /* mov.l 0x8c61de60,r3 */
L8c61dcac: sa_motion_publish_stop(ctx,0x8c61dcacu); r[0]=4u;goto L8c61dcae; /* mov #0x4,r0 */
L8c61dcae: sa_motion_publish_stop(ctx,0x8c61dcaeu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61dcb0; /* fmov.s @(r0,r15),fr3 */
L8c61dcb0: sa_motion_publish_stop(ctx,0x8c61dcb0u); r[0]=8u;goto L8c61dcb2; /* mov #0x8,r0 */
L8c61dcb2: sa_motion_publish_stop(ctx,0x8c61dcb2u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61dcb4; /* fmov.s @r3,fr2 */
L8c61dcb4: sa_motion_publish_stop(ctx,0x8c61dcb4u); r[3]+=8u;goto L8c61dcb6; /* add #0x8,r3 */
L8c61dcb6: sa_motion_publish_stop(ctx,0x8c61dcb6u); r[2]=rd(m,bus,&io_status,0x8c61de64u,4);if(io_status)return io_status;goto L8c61dcb8; /* mov.l 0x8c61de64,r2 */
L8c61dcb8: sa_motion_publish_stop(ctx,0x8c61dcb8u); r[8]+=24u;goto L8c61dcba; /* add #0x18,r8 */
L8c61dcba: sa_motion_publish_stop(ctx,0x8c61dcbau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dcbc; /* fmul fr3,fr2 */
L8c61dcbc: sa_motion_publish_stop(ctx,0x8c61dcbcu); r[1]=rd(m,bus,&io_status,0x8c61de68u,4);if(io_status)return io_status;goto L8c61dcbe; /* mov.l 0x8c61de68,r1 */
L8c61dcbe: sa_motion_publish_stop(ctx,0x8c61dcbeu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61dcc0; /* fmov.s @r2,fr3 */
L8c61dcc0: sa_motion_publish_stop(ctx,0x8c61dcc0u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61dcc2; /* fmov.s @r1,fr1 */
L8c61dcc2: sa_motion_publish_stop(ctx,0x8c61dcc2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61dcc4; /* fsub fr2,fr3 */
L8c61dcc4: sa_motion_publish_stop(ctx,0x8c61dcc4u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61dcc6; /* fmov.s @(r0,r15),fr2 */
L8c61dcc6: sa_motion_publish_stop(ctx,0x8c61dcc6u); r[0]=12u;goto L8c61dcc8; /* mov #0xc,r0 */
L8c61dcc8: sa_motion_publish_stop(ctx,0x8c61dcc8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61dcca; /* fmul fr2,fr1 */
L8c61dcca: sa_motion_publish_stop(ctx,0x8c61dccau); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61dccc; /* fmov.s @(r0,r15),fr2 */
L8c61dccc: sa_motion_publish_stop(ctx,0x8c61dcccu); r[0]=rd(m,bus,&io_status,0x8c61de6cu,4);if(io_status)return io_status;goto L8c61dcce; /* mov.l 0x8c61de6c,r0 */
L8c61dcce: sa_motion_publish_stop(ctx,0x8c61dcceu); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61dcd0; /* mov.l @r0,r2 */
L8c61dcd0: sa_motion_publish_stop(ctx,0x8c61dcd0u); r[0]=4u;goto L8c61dcd2; /* mov #0x4,r0 */
L8c61dcd2: sa_motion_publish_stop(ctx,0x8c61dcd2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61dcd4; /* fsub fr1,fr3 */
L8c61dcd4: sa_motion_publish_stop(ctx,0x8c61dcd4u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61dcd6; /* fmov.s @r3,fr1 */
L8c61dcd6: sa_motion_publish_stop(ctx,0x8c61dcd6u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dcd8; /* fmov.s @(r0,r12),fr0 */
L8c61dcd8: sa_motion_publish_stop(ctx,0x8c61dcd8u); r[1]=r[2];goto L8c61dcda; /* mov r2,r1 */
L8c61dcda: sa_motion_publish_stop(ctx,0x8c61dcdau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61dcdc; /* fmul fr2,fr1 */
L8c61dcdc: sa_motion_publish_stop(ctx,0x8c61dcdcu); r[0]=20u;goto L8c61dcde; /* mov #0x14,r0 */
L8c61dcde: sa_motion_publish_stop(ctx,0x8c61dcdeu); r[3]=r[2];goto L8c61dce0; /* mov r2,r3 */
L8c61dce0: sa_motion_publish_stop(ctx,0x8c61dce0u); fcopy(f,4u,3u);goto L8c61dce2; /* fmov fr3,fr4 */
L8c61dce2: sa_motion_publish_stop(ctx,0x8c61dce2u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61dce4; /* fmov.s @(r0,r1),fr3 */
L8c61dce4: sa_motion_publish_stop(ctx,0x8c61dce4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61dce6; /* fsub fr1,fr4 */
L8c61dce6: sa_motion_publish_stop(ctx,0x8c61dce6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dce8; /* fmul fr0,fr3 */
L8c61dce8: sa_motion_publish_stop(ctx,0x8c61dce8u); fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61dcea; /* fmov.s @r12,fr0 */
L8c61dcea: sa_motion_publish_stop(ctx,0x8c61dceau); r[0]=4u;goto L8c61dcec; /* mov #0x4,r0 */
L8c61dcec: sa_motion_publish_stop(ctx,0x8c61dcecu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61dcee; /* fmov.s @(r0,r3),fr2 */
L8c61dcee: sa_motion_publish_stop(ctx,0x8c61dceeu); r[0]=8u;goto L8c61dcf0; /* mov #0x8,r0 */
L8c61dcf0: sa_motion_publish_stop(ctx,0x8c61dcf0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dcf2; /* fmac fr0,fr2,fr3 */
L8c61dcf2: sa_motion_publish_stop(ctx,0x8c61dcf2u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dcf4; /* fmov.s @(r0,r12),fr0 */
L8c61dcf4: sa_motion_publish_stop(ctx,0x8c61dcf4u); r[0]=36u;goto L8c61dcf6; /* mov #0x24,r0 */
L8c61dcf6: sa_motion_publish_stop(ctx,0x8c61dcf6u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61dcf8; /* fmov.s @(r0,r2),fr2 */
L8c61dcf8: sa_motion_publish_stop(ctx,0x8c61dcf8u); r[0]=4u;goto L8c61dcfa; /* mov #0x4,r0 */
L8c61dcfa: sa_motion_publish_stop(ctx,0x8c61dcfau); r[2]=rd(m,bus,&io_status,0x8c61de6cu,4);if(io_status)return io_status;goto L8c61dcfc; /* mov.l 0x8c61de6c,r2 */
L8c61dcfc: sa_motion_publish_stop(ctx,0x8c61dcfcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dcfe; /* fmac fr0,fr2,fr3 */
L8c61dcfe: sa_motion_publish_stop(ctx,0x8c61dcfeu); fcopy(f,0u,15u);goto L8c61dd00; /* fmov fr15,fr0 */
L8c61dd00: sa_motion_publish_stop(ctx,0x8c61dd00u); fcopy(f,2u,15u);goto L8c61dd02; /* fmov fr15,fr2 */
L8c61dd02: sa_motion_publish_stop(ctx,0x8c61dd02u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dd04; /* fmac fr0,fr3,fr2 */
L8c61dd04: sa_motion_publish_stop(ctx,0x8c61dd04u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61dd06; /* fmov.s fr2,@-r8 */
L8c61dd06: sa_motion_publish_stop(ctx,0x8c61dd06u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61dd08; /* mov.l @r2,r3 */
L8c61dd08: sa_motion_publish_stop(ctx,0x8c61dd08u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dd0a; /* fmov.s @(r0,r12),fr0 */
L8c61dd0a: sa_motion_publish_stop(ctx,0x8c61dd0au); r[0]=16u;goto L8c61dd0c; /* mov #0x10,r0 */
L8c61dd0c: sa_motion_publish_stop(ctx,0x8c61dd0cu); r[2]=r[3];goto L8c61dd0e; /* mov r3,r2 */
L8c61dd0e: sa_motion_publish_stop(ctx,0x8c61dd0eu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61dd10; /* fmov.s @(r0,r2),fr3 */
L8c61dd10: sa_motion_publish_stop(ctx,0x8c61dd10u); r[1]=r[3];goto L8c61dd12; /* mov r3,r1 */
L8c61dd12: sa_motion_publish_stop(ctx,0x8c61dd12u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61dd14; /* fmov.s @r1,fr2 */
L8c61dd14: sa_motion_publish_stop(ctx,0x8c61dd14u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd16; /* fmul fr0,fr3 */
L8c61dd16: sa_motion_publish_stop(ctx,0x8c61dd16u); fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61dd18; /* fmov.s @r12,fr0 */
L8c61dd18: sa_motion_publish_stop(ctx,0x8c61dd18u); r[0]=8u;goto L8c61dd1a; /* mov #0x8,r0 */
L8c61dd1a: sa_motion_publish_stop(ctx,0x8c61dd1au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd1c; /* fmac fr0,fr2,fr3 */
L8c61dd1c: sa_motion_publish_stop(ctx,0x8c61dd1cu); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dd1e; /* fmov.s @(r0,r12),fr0 */
L8c61dd1e: sa_motion_publish_stop(ctx,0x8c61dd1eu); r[0]=32u;goto L8c61dd20; /* mov #0x20,r0 */
L8c61dd20: sa_motion_publish_stop(ctx,0x8c61dd20u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61dd22; /* fmov.s @(r0,r3),fr2 */
L8c61dd22: sa_motion_publish_stop(ctx,0x8c61dd22u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd24; /* fmac fr0,fr2,fr3 */
L8c61dd24: sa_motion_publish_stop(ctx,0x8c61dd24u); fcopy(f,0u,15u);goto L8c61dd26; /* fmov fr15,fr0 */
L8c61dd26: sa_motion_publish_stop(ctx,0x8c61dd26u); fcopy(f,2u,15u);goto L8c61dd28; /* fmov fr15,fr2 */
L8c61dd28: sa_motion_publish_stop(ctx,0x8c61dd28u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dd2a; /* fmac fr0,fr3,fr2 */
L8c61dd2a: sa_motion_publish_stop(ctx,0x8c61dd2au); r[0]=24u;goto L8c61dd2c; /* mov #0x18,r0 */
L8c61dd2c: sa_motion_publish_stop(ctx,0x8c61dd2cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61dd2e; /* fmov.s fr2,@-r8 */
L8c61dd2e: sa_motion_publish_stop(ctx,0x8c61dd2eu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dd30; /* fmov.s @(r0,r12),fr3 */
L8c61dd30: sa_motion_publish_stop(ctx,0x8c61dd30u); r[0]=20u;goto L8c61dd32; /* mov #0x14,r0 */
L8c61dd32: sa_motion_publish_stop(ctx,0x8c61dd32u); r[3]=rd(m,bus,&io_status,0x8c61de70u,4);if(io_status)return io_status;goto L8c61dd34; /* mov.l 0x8c61de70,r3 */
L8c61dd34: sa_motion_publish_stop(ctx,0x8c61dd34u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dd36; /* fmov.s fr3,@-r8 */
L8c61dd36: sa_motion_publish_stop(ctx,0x8c61dd36u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dd38; /* fmov.s @(r0,r12),fr3 */
L8c61dd38: sa_motion_publish_stop(ctx,0x8c61dd38u); r[0]=16u;goto L8c61dd3a; /* mov #0x10,r0 */
L8c61dd3a: sa_motion_publish_stop(ctx,0x8c61dd3au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dd3c; /* fmov.s fr3,@-r8 */
L8c61dd3c: sa_motion_publish_stop(ctx,0x8c61dd3cu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61dd3e; /* fmov.s @(r0,r12),fr3 */
L8c61dd3e: sa_motion_publish_stop(ctx,0x8c61dd3eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dd40; /* fmov.s fr3,@-r8 */
L8c61dd40: sa_motion_publish_stop(ctx,0x8c61dd40u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61dd42; /* mov.l r3,@-r8 */
L8c61dd42: sa_motion_publish_stop(ctx,0x8c61dd42u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61dd44; /* pref @r8 */
L8c61dd44: sa_motion_publish_stop(ctx,0x8c61dd44u); r[2]=rd(m,bus,&io_status,0x8c61de6cu,4);if(io_status)return io_status;goto L8c61dd46; /* mov.l 0x8c61de6c,r2 */
L8c61dd46: sa_motion_publish_stop(ctx,0x8c61dd46u); r[0]=4u;goto L8c61dd48; /* mov #0x4,r0 */
L8c61dd48: sa_motion_publish_stop(ctx,0x8c61dd48u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dd4a; /* fmov.s @(r0,r13),fr0 */
L8c61dd4a: sa_motion_publish_stop(ctx,0x8c61dd4au); r[0]=20u;goto L8c61dd4c; /* mov #0x14,r0 */
L8c61dd4c: sa_motion_publish_stop(ctx,0x8c61dd4cu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61dd4e; /* mov.l @r2,r3 */
L8c61dd4e: sa_motion_publish_stop(ctx,0x8c61dd4eu); r[8]+=56u;goto L8c61dd50; /* add #0x38,r8 */
L8c61dd50: sa_motion_publish_stop(ctx,0x8c61dd50u); r[2]=r[3];goto L8c61dd52; /* mov r3,r2 */
L8c61dd52: sa_motion_publish_stop(ctx,0x8c61dd52u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61dd54; /* fmov.s @(r0,r2),fr3 */
L8c61dd54: sa_motion_publish_stop(ctx,0x8c61dd54u); r[0]=4u;goto L8c61dd56; /* mov #0x4,r0 */
L8c61dd56: sa_motion_publish_stop(ctx,0x8c61dd56u); r[1]=r[3];goto L8c61dd58; /* mov r3,r1 */
L8c61dd58: sa_motion_publish_stop(ctx,0x8c61dd58u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd5a; /* fmul fr0,fr3 */
L8c61dd5a: sa_motion_publish_stop(ctx,0x8c61dd5au); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61dd5c; /* fmov.s @(r0,r1),fr2 */
L8c61dd5c: sa_motion_publish_stop(ctx,0x8c61dd5cu); fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61dd5e; /* fmov.s @r13,fr0 */
L8c61dd5e: sa_motion_publish_stop(ctx,0x8c61dd5eu); r[0]=8u;goto L8c61dd60; /* mov #0x8,r0 */
L8c61dd60: sa_motion_publish_stop(ctx,0x8c61dd60u); r[2]=rd(m,bus,&io_status,0x8c61de6cu,4);if(io_status)return io_status;goto L8c61dd62; /* mov.l 0x8c61de6c,r2 */
L8c61dd62: sa_motion_publish_stop(ctx,0x8c61dd62u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd64; /* fmac fr0,fr2,fr3 */
L8c61dd64: sa_motion_publish_stop(ctx,0x8c61dd64u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dd66; /* fmov.s @(r0,r13),fr0 */
L8c61dd66: sa_motion_publish_stop(ctx,0x8c61dd66u); r[0]=36u;goto L8c61dd68; /* mov #0x24,r0 */
L8c61dd68: sa_motion_publish_stop(ctx,0x8c61dd68u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61dd6a; /* fmov.s @(r0,r3),fr2 */
L8c61dd6a: sa_motion_publish_stop(ctx,0x8c61dd6au); r[0]=4u;goto L8c61dd6c; /* mov #0x4,r0 */
L8c61dd6c: sa_motion_publish_stop(ctx,0x8c61dd6cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd6e; /* fmac fr0,fr2,fr3 */
L8c61dd6e: sa_motion_publish_stop(ctx,0x8c61dd6eu); fcopy(f,0u,15u);goto L8c61dd70; /* fmov fr15,fr0 */
L8c61dd70: sa_motion_publish_stop(ctx,0x8c61dd70u); fcopy(f,2u,15u);goto L8c61dd72; /* fmov fr15,fr2 */
L8c61dd72: sa_motion_publish_stop(ctx,0x8c61dd72u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dd74; /* fmac fr0,fr3,fr2 */
L8c61dd74: sa_motion_publish_stop(ctx,0x8c61dd74u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61dd76; /* fmov.s fr2,@-r8 */
L8c61dd76: sa_motion_publish_stop(ctx,0x8c61dd76u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61dd78; /* mov.l @r2,r3 */
L8c61dd78: sa_motion_publish_stop(ctx,0x8c61dd78u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dd7a; /* fmov.s @(r0,r13),fr0 */
L8c61dd7a: sa_motion_publish_stop(ctx,0x8c61dd7au); r[0]=16u;goto L8c61dd7c; /* mov #0x10,r0 */
L8c61dd7c: sa_motion_publish_stop(ctx,0x8c61dd7cu); r[2]=r[3];goto L8c61dd7e; /* mov r3,r2 */
L8c61dd7e: sa_motion_publish_stop(ctx,0x8c61dd7eu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61dd80; /* fmov.s @(r0,r2),fr3 */
L8c61dd80: sa_motion_publish_stop(ctx,0x8c61dd80u); r[1]=r[3];goto L8c61dd82; /* mov r3,r1 */
L8c61dd82: sa_motion_publish_stop(ctx,0x8c61dd82u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd84; /* fmul fr0,fr3 */
L8c61dd84: sa_motion_publish_stop(ctx,0x8c61dd84u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61dd86; /* fmov.s @r1,fr2 */
L8c61dd86: sa_motion_publish_stop(ctx,0x8c61dd86u); r[0]=8u;goto L8c61dd88; /* mov #0x8,r0 */
L8c61dd88: sa_motion_publish_stop(ctx,0x8c61dd88u); fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61dd8a; /* fmov.s @r13,fr0 */
L8c61dd8a: sa_motion_publish_stop(ctx,0x8c61dd8au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd8c; /* fmac fr0,fr2,fr3 */
L8c61dd8c: sa_motion_publish_stop(ctx,0x8c61dd8cu); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dd8e; /* fmov.s @(r0,r13),fr0 */
L8c61dd8e: sa_motion_publish_stop(ctx,0x8c61dd8eu); r[0]=32u;goto L8c61dd90; /* mov #0x20,r0 */
L8c61dd90: sa_motion_publish_stop(ctx,0x8c61dd90u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61dd92; /* fmov.s @(r0,r3),fr2 */
L8c61dd92: sa_motion_publish_stop(ctx,0x8c61dd92u); r[0]=24u;goto L8c61dd94; /* mov #0x18,r0 */
L8c61dd94: sa_motion_publish_stop(ctx,0x8c61dd94u); r[3]=rd(m,bus,&io_status,0x8c61de70u,4);if(io_status)return io_status;goto L8c61dd96; /* mov.l 0x8c61de70,r3 */
L8c61dd96: sa_motion_publish_stop(ctx,0x8c61dd96u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dd98; /* fmac fr0,fr2,fr3 */
L8c61dd98: sa_motion_publish_stop(ctx,0x8c61dd98u); fcopy(f,0u,15u);goto L8c61dd9a; /* fmov fr15,fr0 */
L8c61dd9a: sa_motion_publish_stop(ctx,0x8c61dd9au); fcopy(f,2u,15u);goto L8c61dd9c; /* fmov fr15,fr2 */
L8c61dd9c: sa_motion_publish_stop(ctx,0x8c61dd9cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dd9e; /* fmac fr0,fr3,fr2 */
L8c61dd9e: sa_motion_publish_stop(ctx,0x8c61dd9eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61dda0; /* fmov.s fr2,@-r8 */
L8c61dda0: sa_motion_publish_stop(ctx,0x8c61dda0u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dda2; /* fmov.s @(r0,r13),fr3 */
L8c61dda2: sa_motion_publish_stop(ctx,0x8c61dda2u); r[0]=20u;goto L8c61dda4; /* mov #0x14,r0 */
L8c61dda4: sa_motion_publish_stop(ctx,0x8c61dda4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dda6; /* fmov.s fr3,@-r8 */
L8c61dda6: sa_motion_publish_stop(ctx,0x8c61dda6u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dda8; /* fmov.s @(r0,r13),fr3 */
L8c61dda8: sa_motion_publish_stop(ctx,0x8c61dda8u); r[0]=16u;goto L8c61ddaa; /* mov #0x10,r0 */
L8c61ddaa: sa_motion_publish_stop(ctx,0x8c61ddaau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ddac; /* fmov.s fr3,@-r8 */
L8c61ddac: sa_motion_publish_stop(ctx,0x8c61ddacu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ddae; /* fmov.s @(r0,r13),fr3 */
L8c61ddae: sa_motion_publish_stop(ctx,0x8c61ddaeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ddb0; /* fmov.s fr3,@-r8 */
L8c61ddb0: sa_motion_publish_stop(ctx,0x8c61ddb0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61ddb2; /* mov.l r3,@-r8 */
L8c61ddb2: sa_motion_publish_stop(ctx,0x8c61ddb2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ddb4; /* pref @r8 */
L8c61ddb4: sa_motion_publish_stop(ctx,0x8c61ddb4u); r[2]=rd(m,bus,&io_status,0x8c61de6cu,4);if(io_status)return io_status;goto L8c61ddb6; /* mov.l 0x8c61de6c,r2 */
L8c61ddb6: sa_motion_publish_stop(ctx,0x8c61ddb6u); r[0]=4u;goto L8c61ddb8; /* mov #0x4,r0 */
L8c61ddb8: sa_motion_publish_stop(ctx,0x8c61ddb8u); r[8]+=60u;goto L8c61ddba; /* add #0x3c,r8 */
L8c61ddba: sa_motion_publish_stop(ctx,0x8c61ddbau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61ddbc; /* fmov.s fr4,@-r8 */
L8c61ddbc: sa_motion_publish_stop(ctx,0x8c61ddbcu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ddbe; /* mov.l @r2,r3 */
L8c61ddbe: sa_motion_publish_stop(ctx,0x8c61ddbeu); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ddc0; /* fmov.s @(r0,r14),fr0 */
L8c61ddc0: sa_motion_publish_stop(ctx,0x8c61ddc0u); r[0]=20u;goto L8c61ddc2; /* mov #0x14,r0 */
L8c61ddc2: sa_motion_publish_stop(ctx,0x8c61ddc2u); r[2]=r[3];goto L8c61ddc4; /* mov r3,r2 */
L8c61ddc4: sa_motion_publish_stop(ctx,0x8c61ddc4u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61ddc6; /* fmov.s @(r0,r2),fr3 */
L8c61ddc6: sa_motion_publish_stop(ctx,0x8c61ddc6u); r[1]=r[3];goto L8c61ddc8; /* mov r3,r1 */
L8c61ddc8: sa_motion_publish_stop(ctx,0x8c61ddc8u); r[0]=4u;goto L8c61ddca; /* mov #0x4,r0 */
L8c61ddca: sa_motion_publish_stop(ctx,0x8c61ddcau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ddcc; /* fmul fr0,fr3 */
L8c61ddcc: sa_motion_publish_stop(ctx,0x8c61ddccu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61ddce; /* fmov.s @(r0,r1),fr2 */
L8c61ddce: sa_motion_publish_stop(ctx,0x8c61ddceu); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61ddd0; /* fmov.s @r14,fr0 */
L8c61ddd0: sa_motion_publish_stop(ctx,0x8c61ddd0u); r[0]=8u;goto L8c61ddd2; /* mov #0x8,r0 */
L8c61ddd2: sa_motion_publish_stop(ctx,0x8c61ddd2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ddd4; /* fmac fr0,fr2,fr3 */
L8c61ddd4: sa_motion_publish_stop(ctx,0x8c61ddd4u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ddd6; /* fmov.s @(r0,r14),fr0 */
L8c61ddd6: sa_motion_publish_stop(ctx,0x8c61ddd6u); r[0]=36u;goto L8c61ddd8; /* mov #0x24,r0 */
L8c61ddd8: sa_motion_publish_stop(ctx,0x8c61ddd8u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61ddda; /* fmov.s @(r0,r3),fr2 */
L8c61ddda: sa_motion_publish_stop(ctx,0x8c61dddau); r[0]=4u;goto L8c61dddc; /* mov #0x4,r0 */
L8c61dddc: sa_motion_publish_stop(ctx,0x8c61dddcu); r[3]=rd(m,bus,&io_status,0x8c61de6cu,4);if(io_status)return io_status;goto L8c61ddde; /* mov.l 0x8c61de6c,r3 */
L8c61ddde: sa_motion_publish_stop(ctx,0x8c61dddeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dde0; /* fmac fr0,fr2,fr3 */
L8c61dde0: sa_motion_publish_stop(ctx,0x8c61dde0u); fcopy(f,2u,15u);goto L8c61dde2; /* fmov fr15,fr2 */
L8c61dde2: sa_motion_publish_stop(ctx,0x8c61dde2u); fcopy(f,0u,15u);goto L8c61dde4; /* fmov fr15,fr0 */
L8c61dde4: sa_motion_publish_stop(ctx,0x8c61dde4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dde6; /* fmac fr0,fr3,fr2 */
L8c61dde6: sa_motion_publish_stop(ctx,0x8c61dde6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61dde8; /* fmov.s fr2,@-r8 */
L8c61dde8: sa_motion_publish_stop(ctx,0x8c61dde8u); r[9]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ddea; /* mov.l @r3,r9 */
L8c61ddea: sa_motion_publish_stop(ctx,0x8c61ddeau); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ddec; /* fmov.s @(r0,r14),fr0 */
L8c61ddec: sa_motion_publish_stop(ctx,0x8c61ddecu); r[0]=16u;goto L8c61ddee; /* mov #0x10,r0 */
L8c61ddee: sa_motion_publish_stop(ctx,0x8c61ddeeu); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61ddf0; /* fmov.s @(r0,r9),fr3 */
L8c61ddf0: sa_motion_publish_stop(ctx,0x8c61ddf0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ddf2; /* fmul fr0,fr3 */
L8c61ddf2: sa_motion_publish_stop(ctx,0x8c61ddf2u); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61ddf4; /* fmov.s @r14,fr0 */
L8c61ddf4: sa_motion_publish_stop(ctx,0x8c61ddf4u); fload(m,bus,&io_status,f,2u,r[9]);if(io_status)return io_status;goto L8c61ddf6; /* fmov.s @r9,fr2 */
L8c61ddf6: sa_motion_publish_stop(ctx,0x8c61ddf6u); r[0]=8u;goto L8c61ddf8; /* mov #0x8,r0 */
L8c61ddf8: sa_motion_publish_stop(ctx,0x8c61ddf8u); r[2]=rd(m,bus,&io_status,0x8c61de74u,4);if(io_status)return io_status;goto L8c61ddfa; /* mov.l 0x8c61de74,r2 */
L8c61ddfa: sa_motion_publish_stop(ctx,0x8c61ddfau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ddfc; /* fmac fr0,fr2,fr3 */
L8c61ddfc: sa_motion_publish_stop(ctx,0x8c61ddfcu); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ddfe; /* fmov.s @(r0,r14),fr0 */
L8c61ddfe: sa_motion_publish_stop(ctx,0x8c61ddfeu); r[0]=32u;goto L8c61de00; /* mov #0x20,r0 */
L8c61de00: sa_motion_publish_stop(ctx,0x8c61de00u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61de02; /* fmov.s @(r0,r9),fr2 */
L8c61de02: sa_motion_publish_stop(ctx,0x8c61de02u); r[0]=24u;goto L8c61de04; /* mov #0x18,r0 */
L8c61de04: sa_motion_publish_stop(ctx,0x8c61de04u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61de06; /* fmac fr0,fr2,fr3 */
L8c61de06: sa_motion_publish_stop(ctx,0x8c61de06u); fcopy(f,0u,15u);goto L8c61de08; /* fmov fr15,fr0 */
L8c61de08: sa_motion_publish_stop(ctx,0x8c61de08u); fcopy(f,2u,15u);goto L8c61de0a; /* fmov fr15,fr2 */
L8c61de0a: sa_motion_publish_stop(ctx,0x8c61de0au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61de0c; /* fmac fr0,fr3,fr2 */
L8c61de0c: sa_motion_publish_stop(ctx,0x8c61de0cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61de0e; /* fmov.s fr2,@-r8 */
L8c61de0e: sa_motion_publish_stop(ctx,0x8c61de0eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61de10; /* fmov.s @(r0,r14),fr3 */
L8c61de10: sa_motion_publish_stop(ctx,0x8c61de10u); r[0]=20u;goto L8c61de12; /* mov #0x14,r0 */
L8c61de12: sa_motion_publish_stop(ctx,0x8c61de12u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61de14; /* fmov.s fr3,@-r8 */
L8c61de14: sa_motion_publish_stop(ctx,0x8c61de14u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61de16; /* fmov.s @(r0,r14),fr3 */
L8c61de16: sa_motion_publish_stop(ctx,0x8c61de16u); r[0]=16u;goto L8c61de18; /* mov #0x10,r0 */
L8c61de18: sa_motion_publish_stop(ctx,0x8c61de18u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61de1a; /* fmov.s fr3,@-r8 */
L8c61de1a: sa_motion_publish_stop(ctx,0x8c61de1au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61de1c; /* fmov.s @(r0,r14),fr3 */
L8c61de1c: sa_motion_publish_stop(ctx,0x8c61de1cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61de1e; /* fmov.s fr3,@-r8 */
L8c61de1e: sa_motion_publish_stop(ctx,0x8c61de1eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61de20; /* mov.l r2,@-r8 */
L8c61de20: sa_motion_publish_stop(ctx,0x8c61de20u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61de22; /* pref @r8 */
L8c61de22: sa_motion_publish_stop(ctx,0x8c61de22u); r[1]=rd(m,bus,&io_status,0x8c61de78u,4);if(io_status)return io_status;goto L8c61de24; /* mov.l 0x8c61de78,r1 */
L8c61de24: sa_motion_publish_stop(ctx,0x8c61de24u); r[8]+=32u;goto L8c61de26; /* add #0x20,r8 */
L8c61de26: sa_motion_publish_stop(ctx,0x8c61de26u); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61de28; /* mov.l @r1,r2 */
L8c61de28: sa_motion_publish_stop(ctx,0x8c61de28u); r[2]+=3u;goto L8c61de2a; /* add #0x3,r2 */
L8c61de2a: sa_motion_publish_stop(ctx,0x8c61de2au); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61de2c; /* mov.l r2,@r1 */
L8c61de2c: sa_motion_publish_stop(ctx,0x8c61de2cu); r[11]+=1u;goto L8c61de2e; /* add #0x1,r11 */
L8c61de2e: sa_motion_publish_stop(ctx,0x8c61de2eu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61de30; /* mov.l @r15,r3 */
L8c61de30: sa_motion_publish_stop(ctx,0x8c61de30u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61de32; /* mov.w @(0x2,r3),r0 */
L8c61de32: sa_motion_publish_stop(ctx,0x8c61de32u); r[0]=r[0]&65535u;goto L8c61de34; /* extu.w r0,r0 */
L8c61de34: sa_motion_publish_stop(ctx,0x8c61de34u); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61de36; /* cmp/ge r0,r11 */
L8c61de36: sa_motion_publish_stop(ctx,0x8c61de36u); if((*sr&1u))goto L8c61de3c;goto L8c61de38; /* bt 0x8c61de3c */
L8c61de38: sa_motion_publish_stop(ctx,0x8c61de3au); goto L8c61dc1c; /* bra 0x8c61dc1c */
L8c61de3c: sa_motion_publish_stop(ctx,0x8c61de3eu); goto L8c61e4a8; /* bra 0x8c61e4a8 */
L8c61de40: sa_motion_publish_stop(ctx,0x8c61de40u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61de42; /* mov.l @r15,r1 */
L8c61de42: sa_motion_publish_stop(ctx,0x8c61de42u); r[2]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61de44; /* mov.l @r13,r2 */
L8c61de44: sa_motion_publish_stop(ctx,0x8c61de44u); r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61de46; /* mov.w @(0x2,r1),r0 */
L8c61de46: sa_motion_publish_stop(ctx,0x8c61de46u); r[0]=r[0]&65535u;goto L8c61de48; /* extu.w r0,r0 */
L8c61de48: sa_motion_publish_stop(ctx,0x8c61de48u); r[0]<<=2;goto L8c61de4a; /* shll2 r0 */
L8c61de4a: sa_motion_publish_stop(ctx,0x8c61de4au); r[2]+=r[0];goto L8c61de4c; /* add r0,r2 */
L8c61de4c: sa_motion_publish_stop(ctx,0x8c61de4cu); wr(m,bus,&io_status,r[13],r[2],4);if(io_status)return io_status;goto L8c61de4e; /* mov.l r2,@r13 */
L8c61de4e: sa_motion_publish_stop(ctx,0x8c61de50u); r[9]=r[4];goto L8c61e110; /* bra 0x8c61e110 */
L8c61de7c: sa_motion_publish_stop(ctx,0x8c61de7cu); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61de7e; /* mov.w @r10+,r14 */
L8c61de7e: sa_motion_publish_stop(ctx,0x8c61de7eu); r[0]=16u;goto L8c61de80; /* mov #0x10,r0 */
L8c61de80: sa_motion_publish_stop(ctx,0x8c61de80u); r[3]=rd(m,bus,&io_status,0x8c61e18cu,4);if(io_status)return io_status;goto L8c61de82; /* mov.l 0x8c61e18c,r3 */
L8c61de82: sa_motion_publish_stop(ctx,0x8c61de82u); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61de84; /* mov.w @r10+,r13 */
L8c61de84: sa_motion_publish_stop(ctx,0x8c61de84u); r[14]=r[14]&65535u;goto L8c61de86; /* extu.w r14,r14 */
L8c61de86: sa_motion_publish_stop(ctx,0x8c61de86u); r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61de88; /* mov.l @r3,r4 */
L8c61de88: sa_motion_publish_stop(ctx,0x8c61de88u); r[14]<<=2;goto L8c61de8a; /* shll2 r14 */
L8c61de8a: sa_motion_publish_stop(ctx,0x8c61de8au); r[13]=r[13]&65535u;goto L8c61de8c; /* extu.w r13,r13 */
L8c61de8c: sa_motion_publish_stop(ctx,0x8c61de8cu); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61de8e; /* mov.w @r10+,r12 */
L8c61de8e: sa_motion_publish_stop(ctx,0x8c61de8eu); r[13]<<=2;goto L8c61de90; /* shll2 r13 */
L8c61de90: sa_motion_publish_stop(ctx,0x8c61de90u); r[13]<<=2;goto L8c61de92; /* shll2 r13 */
L8c61de92: sa_motion_publish_stop(ctx,0x8c61de92u); r[11]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61de94; /* mov.w @r10+,r11 */
L8c61de94: sa_motion_publish_stop(ctx,0x8c61de94u); r[14]<<=2;goto L8c61de96; /* shll2 r14 */
L8c61de96: sa_motion_publish_stop(ctx,0x8c61de96u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61de98; /* shll r14 */
L8c61de98: sa_motion_publish_stop(ctx,0x8c61de98u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61de9a; /* shll r13 */
L8c61de9a: sa_motion_publish_stop(ctx,0x8c61de9au); r[13]+=r[4];goto L8c61de9c; /* add r4,r13 */
L8c61de9c: sa_motion_publish_stop(ctx,0x8c61de9cu); r[14]+=r[4];goto L8c61de9e; /* add r4,r14 */
L8c61de9e: sa_motion_publish_stop(ctx,0x8c61de9eu); fload(m,bus,&io_status,f,5u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dea0; /* fmov.s @(r0,r13),fr5 */
L8c61dea0: sa_motion_publish_stop(ctx,0x8c61dea0u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dea2; /* fmov.s @(r0,r14),fr3 */
L8c61dea2: sa_motion_publish_stop(ctx,0x8c61dea2u); r[12]=r[12]&65535u;goto L8c61dea4; /* extu.w r12,r12 */
L8c61dea4: sa_motion_publish_stop(ctx,0x8c61dea4u); fcopy(f,2u,5u);goto L8c61dea6; /* fmov fr5,fr2 */
L8c61dea6: sa_motion_publish_stop(ctx,0x8c61dea6u); r[12]<<=2;goto L8c61dea8; /* shll2 r12 */
L8c61dea8: sa_motion_publish_stop(ctx,0x8c61dea8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61deaa; /* fsub fr3,fr2 */
L8c61deaa: sa_motion_publish_stop(ctx,0x8c61deaau); r[11]=r[11]&65535u;goto L8c61deac; /* extu.w r11,r11 */
L8c61deac: sa_motion_publish_stop(ctx,0x8c61deacu); r[12]<<=2;goto L8c61deae; /* shll2 r12 */
L8c61deae: sa_motion_publish_stop(ctx,0x8c61deaeu); r[11]<<=2;goto L8c61deb0; /* shll2 r11 */
L8c61deb0: sa_motion_publish_stop(ctx,0x8c61deb0u); r[0]=20u;goto L8c61deb2; /* mov #0x14,r0 */
L8c61deb2: sa_motion_publish_stop(ctx,0x8c61deb2u); fload(m,bus,&io_status,f,4u,r[0]+r[13]);if(io_status)return io_status;goto L8c61deb4; /* fmov.s @(r0,r13),fr4 */
L8c61deb4: sa_motion_publish_stop(ctx,0x8c61deb4u); r[11]<<=2;goto L8c61deb6; /* shll2 r11 */
L8c61deb6: sa_motion_publish_stop(ctx,0x8c61deb6u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61deb8; /* shll r12 */
L8c61deb8: sa_motion_publish_stop(ctx,0x8c61deb8u); r[12]+=r[4];goto L8c61deba; /* add r4,r12 */
L8c61deba: sa_motion_publish_stop(ctx,0x8c61debau); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61debc; /* shll r11 */
L8c61debc: sa_motion_publish_stop(ctx,0x8c61debcu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61debe; /* fmov.s @(r0,r12),fr3 */
L8c61debe: sa_motion_publish_stop(ctx,0x8c61debeu); fcopy(f,1u,4u);goto L8c61dec0; /* fmov fr4,fr1 */
L8c61dec0: sa_motion_publish_stop(ctx,0x8c61dec0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61dec2; /* fsub fr4,fr3 */
L8c61dec2: sa_motion_publish_stop(ctx,0x8c61dec2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dec4; /* fmul fr3,fr2 */
L8c61dec4: sa_motion_publish_stop(ctx,0x8c61dec4u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dec6; /* fmov.s @(r0,r14),fr3 */
L8c61dec6: sa_motion_publish_stop(ctx,0x8c61dec6u); r[0]=16u;goto L8c61dec8; /* mov #0x10,r0 */
L8c61dec8: sa_motion_publish_stop(ctx,0x8c61dec8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61deca; /* fsub fr3,fr1 */
L8c61deca: sa_motion_publish_stop(ctx,0x8c61decau); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61decc; /* fmov.s @(r0,r12),fr3 */
L8c61decc: sa_motion_publish_stop(ctx,0x8c61deccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61dece; /* fsub fr5,fr3 */
L8c61dece: sa_motion_publish_stop(ctx,0x8c61deceu); fcopy(f,4u,2u);goto L8c61ded0; /* fmov fr2,fr4 */
L8c61ded0: sa_motion_publish_stop(ctx,0x8c61ded0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ded2; /* fmul fr3,fr1 */
L8c61ded2: sa_motion_publish_stop(ctx,0x8c61ded2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61ded4; /* fldi0 fr3 */
L8c61ded4: sa_motion_publish_stop(ctx,0x8c61ded4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61ded6; /* fsub fr1,fr4 */
L8c61ded6: sa_motion_publish_stop(ctx,0x8c61ded6u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61ded8; /* fcmp/gt fr3,fr4 */
L8c61ded8: sa_motion_publish_stop(ctx,0x8c61dedau); take=!(*sr&1u);r[11]+=r[4];if(take)goto L8c61dee0;goto L8c61dedc; /* bf/s 0x8c61dee0 */
L8c61dedc: sa_motion_publish_stop(ctx,0x8c61dedeu); goto L8c61e10e; /* bra 0x8c61e10e */
L8c61dee0: sa_motion_publish_stop(ctx,0x8c61dee0u); fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61dee2; /* fmov.s @r14,fr2 */
L8c61dee2: sa_motion_publish_stop(ctx,0x8c61dee2u); r[0]=4u;goto L8c61dee4; /* mov #0x4,r0 */
L8c61dee4: sa_motion_publish_stop(ctx,0x8c61dee4u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61dee6; /* fmov.s @r13,fr3 */
L8c61dee6: sa_motion_publish_stop(ctx,0x8c61dee6u); r[4]=r[15];goto L8c61dee8; /* mov r15,r4 */
L8c61dee8: sa_motion_publish_stop(ctx,0x8c61dee8u); r[3]=rd(m,bus,&io_status,0x8c61e190u,4);if(io_status)return io_status;goto L8c61deea; /* mov.l 0x8c61e190,r3 */
L8c61deea: sa_motion_publish_stop(ctx,0x8c61deeau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61deec; /* fadd fr3,fr2 */
L8c61deec: sa_motion_publish_stop(ctx,0x8c61deecu); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61deee; /* fmov.s @r12,fr3 */
L8c61deee: sa_motion_publish_stop(ctx,0x8c61deeeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61def0; /* fadd fr3,fr2 */
L8c61def0: sa_motion_publish_stop(ctx,0x8c61def0u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61def2; /* fmov fr2,@(r0,r15) */
L8c61def2: sa_motion_publish_stop(ctx,0x8c61def2u); r[0]=4u;goto L8c61def4; /* mov #0x4,r0 */
L8c61def4: sa_motion_publish_stop(ctx,0x8c61def4u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61def6; /* fmov.s @(r0,r14),fr2 */
L8c61def6: sa_motion_publish_stop(ctx,0x8c61def6u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61def8; /* fmov.s @(r0,r13),fr3 */
L8c61def8: sa_motion_publish_stop(ctx,0x8c61def8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61defa; /* fadd fr3,fr2 */
L8c61defa: sa_motion_publish_stop(ctx,0x8c61defau); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61defc; /* fmov.s @(r0,r12),fr3 */
L8c61defc: sa_motion_publish_stop(ctx,0x8c61defcu); r[0]=8u;goto L8c61defe; /* mov #0x8,r0 */
L8c61defe: sa_motion_publish_stop(ctx,0x8c61defeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61df00; /* fadd fr3,fr2 */
L8c61df00: sa_motion_publish_stop(ctx,0x8c61df00u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61df02; /* fmov fr2,@(r0,r15) */
L8c61df02: sa_motion_publish_stop(ctx,0x8c61df02u); r[0]=8u;goto L8c61df04; /* mov #0x8,r0 */
L8c61df04: sa_motion_publish_stop(ctx,0x8c61df04u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61df06; /* fmov.s @(r0,r14),fr2 */
L8c61df06: sa_motion_publish_stop(ctx,0x8c61df06u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61df08; /* fmov.s @(r0,r13),fr3 */
L8c61df08: sa_motion_publish_stop(ctx,0x8c61df08u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61df0a; /* fadd fr3,fr2 */
L8c61df0a: sa_motion_publish_stop(ctx,0x8c61df0au); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61df0c; /* fmov.s @(r0,r12),fr3 */
L8c61df0c: sa_motion_publish_stop(ctx,0x8c61df0cu); r[0]=12u;goto L8c61df0e; /* mov #0xc,r0 */
L8c61df0e: sa_motion_publish_stop(ctx,0x8c61df0eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61df10; /* fadd fr3,fr2 */
L8c61df10: sa_motion_publish_stop(ctx,0x8c61df10u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61df12; /* fmov fr2,@(r0,r15) */
L8c61df12: sa_motion_publish_stop(ctx,0x8c61df14u); target=r[3];*pr=0x8c61df16u;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61df16; /* jsr @r3 */
L8c61df16: sa_motion_publish_stop(ctx,0x8c61df16u); r[3]=rd(m,bus,&io_status,0x8c61e194u,4);if(io_status)return io_status;goto L8c61df18; /* mov.l 0x8c61e194,r3 */
L8c61df18: sa_motion_publish_stop(ctx,0x8c61df18u); r[0]=4u;goto L8c61df1a; /* mov #0x4,r0 */
L8c61df1a: sa_motion_publish_stop(ctx,0x8c61df1au); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61df1c; /* fmov.s @(r0,r15),fr3 */
L8c61df1c: sa_motion_publish_stop(ctx,0x8c61df1cu); r[0]=8u;goto L8c61df1e; /* mov #0x8,r0 */
L8c61df1e: sa_motion_publish_stop(ctx,0x8c61df1eu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61df20; /* fmov.s @r3,fr2 */
L8c61df20: sa_motion_publish_stop(ctx,0x8c61df20u); r[3]+=8u;goto L8c61df22; /* add #0x8,r3 */
L8c61df22: sa_motion_publish_stop(ctx,0x8c61df22u); r[2]=rd(m,bus,&io_status,0x8c61e198u,4);if(io_status)return io_status;goto L8c61df24; /* mov.l 0x8c61e198,r2 */
L8c61df24: sa_motion_publish_stop(ctx,0x8c61df24u); r[8]+=24u;goto L8c61df26; /* add #0x18,r8 */
L8c61df26: sa_motion_publish_stop(ctx,0x8c61df26u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61df28; /* fmul fr3,fr2 */
L8c61df28: sa_motion_publish_stop(ctx,0x8c61df28u); r[1]=rd(m,bus,&io_status,0x8c61e19cu,4);if(io_status)return io_status;goto L8c61df2a; /* mov.l 0x8c61e19c,r1 */
L8c61df2a: sa_motion_publish_stop(ctx,0x8c61df2au); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61df2c; /* fmov.s @r2,fr3 */
L8c61df2c: sa_motion_publish_stop(ctx,0x8c61df2cu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61df2e; /* fmov.s @r1,fr1 */
L8c61df2e: sa_motion_publish_stop(ctx,0x8c61df2eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61df30; /* fsub fr2,fr3 */
L8c61df30: sa_motion_publish_stop(ctx,0x8c61df30u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61df32; /* fmov.s @(r0,r15),fr2 */
L8c61df32: sa_motion_publish_stop(ctx,0x8c61df32u); r[0]=12u;goto L8c61df34; /* mov #0xc,r0 */
L8c61df34: sa_motion_publish_stop(ctx,0x8c61df34u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61df36; /* fmul fr2,fr1 */
L8c61df36: sa_motion_publish_stop(ctx,0x8c61df36u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61df38; /* fmov.s @(r0,r15),fr2 */
L8c61df38: sa_motion_publish_stop(ctx,0x8c61df38u); r[0]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61df3a; /* mov.l 0x8c61e1a0,r0 */
L8c61df3a: sa_motion_publish_stop(ctx,0x8c61df3au); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61df3c; /* mov.l @r0,r2 */
L8c61df3c: sa_motion_publish_stop(ctx,0x8c61df3cu); r[0]=4u;goto L8c61df3e; /* mov #0x4,r0 */
L8c61df3e: sa_motion_publish_stop(ctx,0x8c61df3eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61df40; /* fsub fr1,fr3 */
L8c61df40: sa_motion_publish_stop(ctx,0x8c61df40u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61df42; /* fmov.s @r3,fr1 */
L8c61df42: sa_motion_publish_stop(ctx,0x8c61df42u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61df44; /* fmov.s @(r0,r14),fr0 */
L8c61df44: sa_motion_publish_stop(ctx,0x8c61df44u); r[1]=r[2];goto L8c61df46; /* mov r2,r1 */
L8c61df46: sa_motion_publish_stop(ctx,0x8c61df46u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61df48; /* fmul fr2,fr1 */
L8c61df48: sa_motion_publish_stop(ctx,0x8c61df48u); r[0]=20u;goto L8c61df4a; /* mov #0x14,r0 */
L8c61df4a: sa_motion_publish_stop(ctx,0x8c61df4au); r[3]=r[2];goto L8c61df4c; /* mov r2,r3 */
L8c61df4c: sa_motion_publish_stop(ctx,0x8c61df4cu); fcopy(f,4u,3u);goto L8c61df4e; /* fmov fr3,fr4 */
L8c61df4e: sa_motion_publish_stop(ctx,0x8c61df4eu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61df50; /* fmov.s @(r0,r1),fr3 */
L8c61df50: sa_motion_publish_stop(ctx,0x8c61df50u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61df52; /* fsub fr1,fr4 */
L8c61df52: sa_motion_publish_stop(ctx,0x8c61df52u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61df54; /* fmul fr0,fr3 */
L8c61df54: sa_motion_publish_stop(ctx,0x8c61df54u); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61df56; /* fmov.s @r14,fr0 */
L8c61df56: sa_motion_publish_stop(ctx,0x8c61df56u); r[0]=4u;goto L8c61df58; /* mov #0x4,r0 */
L8c61df58: sa_motion_publish_stop(ctx,0x8c61df58u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61df5a; /* fmov.s @(r0,r3),fr2 */
L8c61df5a: sa_motion_publish_stop(ctx,0x8c61df5au); r[0]=8u;goto L8c61df5c; /* mov #0x8,r0 */
L8c61df5c: sa_motion_publish_stop(ctx,0x8c61df5cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61df5e; /* fmac fr0,fr2,fr3 */
L8c61df5e: sa_motion_publish_stop(ctx,0x8c61df5eu); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61df60; /* fmov.s @(r0,r14),fr0 */
L8c61df60: sa_motion_publish_stop(ctx,0x8c61df60u); r[0]=36u;goto L8c61df62; /* mov #0x24,r0 */
L8c61df62: sa_motion_publish_stop(ctx,0x8c61df62u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61df64; /* fmov.s @(r0,r2),fr2 */
L8c61df64: sa_motion_publish_stop(ctx,0x8c61df64u); r[0]=4u;goto L8c61df66; /* mov #0x4,r0 */
L8c61df66: sa_motion_publish_stop(ctx,0x8c61df66u); r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61df68; /* mov.l 0x8c61e1a0,r2 */
L8c61df68: sa_motion_publish_stop(ctx,0x8c61df68u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61df6a; /* fmac fr0,fr2,fr3 */
L8c61df6a: sa_motion_publish_stop(ctx,0x8c61df6au); fcopy(f,0u,15u);goto L8c61df6c; /* fmov fr15,fr0 */
L8c61df6c: sa_motion_publish_stop(ctx,0x8c61df6cu); fcopy(f,2u,15u);goto L8c61df6e; /* fmov fr15,fr2 */
L8c61df6e: sa_motion_publish_stop(ctx,0x8c61df6eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61df70; /* fmac fr0,fr3,fr2 */
L8c61df70: sa_motion_publish_stop(ctx,0x8c61df70u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61df72; /* fmov.s fr2,@-r8 */
L8c61df72: sa_motion_publish_stop(ctx,0x8c61df72u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61df74; /* mov.l @r2,r3 */
L8c61df74: sa_motion_publish_stop(ctx,0x8c61df74u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61df76; /* fmov.s @(r0,r14),fr0 */
L8c61df76: sa_motion_publish_stop(ctx,0x8c61df76u); r[0]=16u;goto L8c61df78; /* mov #0x10,r0 */
L8c61df78: sa_motion_publish_stop(ctx,0x8c61df78u); r[2]=r[3];goto L8c61df7a; /* mov r3,r2 */
L8c61df7a: sa_motion_publish_stop(ctx,0x8c61df7au); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61df7c; /* fmov.s @(r0,r2),fr3 */
L8c61df7c: sa_motion_publish_stop(ctx,0x8c61df7cu); r[1]=r[3];goto L8c61df7e; /* mov r3,r1 */
L8c61df7e: sa_motion_publish_stop(ctx,0x8c61df7eu); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61df80; /* fmov.s @r1,fr2 */
L8c61df80: sa_motion_publish_stop(ctx,0x8c61df80u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61df82; /* fmul fr0,fr3 */
L8c61df82: sa_motion_publish_stop(ctx,0x8c61df82u); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61df84; /* fmov.s @r14,fr0 */
L8c61df84: sa_motion_publish_stop(ctx,0x8c61df84u); r[0]=8u;goto L8c61df86; /* mov #0x8,r0 */
L8c61df86: sa_motion_publish_stop(ctx,0x8c61df86u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61df88; /* fmac fr0,fr2,fr3 */
L8c61df88: sa_motion_publish_stop(ctx,0x8c61df88u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61df8a; /* fmov.s @(r0,r14),fr0 */
L8c61df8a: sa_motion_publish_stop(ctx,0x8c61df8au); r[0]=32u;goto L8c61df8c; /* mov #0x20,r0 */
L8c61df8c: sa_motion_publish_stop(ctx,0x8c61df8cu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61df8e; /* fmov.s @(r0,r3),fr2 */
L8c61df8e: sa_motion_publish_stop(ctx,0x8c61df8eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61df90; /* fmac fr0,fr2,fr3 */
L8c61df90: sa_motion_publish_stop(ctx,0x8c61df90u); fcopy(f,0u,15u);goto L8c61df92; /* fmov fr15,fr0 */
L8c61df92: sa_motion_publish_stop(ctx,0x8c61df92u); fcopy(f,2u,15u);goto L8c61df94; /* fmov fr15,fr2 */
L8c61df94: sa_motion_publish_stop(ctx,0x8c61df94u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61df96; /* fmac fr0,fr3,fr2 */
L8c61df96: sa_motion_publish_stop(ctx,0x8c61df96u); r[0]=24u;goto L8c61df98; /* mov #0x18,r0 */
L8c61df98: sa_motion_publish_stop(ctx,0x8c61df98u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61df9a; /* fmov.s fr2,@-r8 */
L8c61df9a: sa_motion_publish_stop(ctx,0x8c61df9au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61df9c; /* fmov.s @(r0,r14),fr3 */
L8c61df9c: sa_motion_publish_stop(ctx,0x8c61df9cu); r[0]=20u;goto L8c61df9e; /* mov #0x14,r0 */
L8c61df9e: sa_motion_publish_stop(ctx,0x8c61df9eu); r[3]=rd(m,bus,&io_status,0x8c61e1a4u,4);if(io_status)return io_status;goto L8c61dfa0; /* mov.l 0x8c61e1a4,r3 */
L8c61dfa0: sa_motion_publish_stop(ctx,0x8c61dfa0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dfa2; /* fmov.s fr3,@-r8 */
L8c61dfa2: sa_motion_publish_stop(ctx,0x8c61dfa2u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dfa4; /* fmov.s @(r0,r14),fr3 */
L8c61dfa4: sa_motion_publish_stop(ctx,0x8c61dfa4u); r[0]=16u;goto L8c61dfa6; /* mov #0x10,r0 */
L8c61dfa6: sa_motion_publish_stop(ctx,0x8c61dfa6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dfa8; /* fmov.s fr3,@-r8 */
L8c61dfa8: sa_motion_publish_stop(ctx,0x8c61dfa8u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61dfaa; /* fmov.s @(r0,r14),fr3 */
L8c61dfaa: sa_motion_publish_stop(ctx,0x8c61dfaau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61dfac; /* fmov.s fr3,@-r8 */
L8c61dfac: sa_motion_publish_stop(ctx,0x8c61dfacu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61dfae; /* mov.l r3,@-r8 */
L8c61dfae: sa_motion_publish_stop(ctx,0x8c61dfaeu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61dfb0; /* pref @r8 */
L8c61dfb0: sa_motion_publish_stop(ctx,0x8c61dfb0u); r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61dfb2; /* mov.l 0x8c61e1a0,r2 */
L8c61dfb2: sa_motion_publish_stop(ctx,0x8c61dfb2u); r[0]=4u;goto L8c61dfb4; /* mov #0x4,r0 */
L8c61dfb4: sa_motion_publish_stop(ctx,0x8c61dfb4u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dfb6; /* fmov.s @(r0,r13),fr0 */
L8c61dfb6: sa_motion_publish_stop(ctx,0x8c61dfb6u); r[0]=20u;goto L8c61dfb8; /* mov #0x14,r0 */
L8c61dfb8: sa_motion_publish_stop(ctx,0x8c61dfb8u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61dfba; /* mov.l @r2,r3 */
L8c61dfba: sa_motion_publish_stop(ctx,0x8c61dfbau); r[8]+=56u;goto L8c61dfbc; /* add #0x38,r8 */
L8c61dfbc: sa_motion_publish_stop(ctx,0x8c61dfbcu); r[2]=r[3];goto L8c61dfbe; /* mov r3,r2 */
L8c61dfbe: sa_motion_publish_stop(ctx,0x8c61dfbeu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61dfc0; /* fmov.s @(r0,r2),fr3 */
L8c61dfc0: sa_motion_publish_stop(ctx,0x8c61dfc0u); r[0]=4u;goto L8c61dfc2; /* mov #0x4,r0 */
L8c61dfc2: sa_motion_publish_stop(ctx,0x8c61dfc2u); r[1]=r[3];goto L8c61dfc4; /* mov r3,r1 */
L8c61dfc4: sa_motion_publish_stop(ctx,0x8c61dfc4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dfc6; /* fmul fr0,fr3 */
L8c61dfc6: sa_motion_publish_stop(ctx,0x8c61dfc6u); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61dfc8; /* fmov.s @(r0,r1),fr2 */
L8c61dfc8: sa_motion_publish_stop(ctx,0x8c61dfc8u); fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61dfca; /* fmov.s @r13,fr0 */
L8c61dfca: sa_motion_publish_stop(ctx,0x8c61dfcau); r[0]=8u;goto L8c61dfcc; /* mov #0x8,r0 */
L8c61dfcc: sa_motion_publish_stop(ctx,0x8c61dfccu); r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61dfce; /* mov.l 0x8c61e1a0,r2 */
L8c61dfce: sa_motion_publish_stop(ctx,0x8c61dfceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dfd0; /* fmac fr0,fr2,fr3 */
L8c61dfd0: sa_motion_publish_stop(ctx,0x8c61dfd0u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dfd2; /* fmov.s @(r0,r13),fr0 */
L8c61dfd2: sa_motion_publish_stop(ctx,0x8c61dfd2u); r[0]=36u;goto L8c61dfd4; /* mov #0x24,r0 */
L8c61dfd4: sa_motion_publish_stop(ctx,0x8c61dfd4u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61dfd6; /* fmov.s @(r0,r3),fr2 */
L8c61dfd6: sa_motion_publish_stop(ctx,0x8c61dfd6u); r[0]=4u;goto L8c61dfd8; /* mov #0x4,r0 */
L8c61dfd8: sa_motion_publish_stop(ctx,0x8c61dfd8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dfda; /* fmac fr0,fr2,fr3 */
L8c61dfda: sa_motion_publish_stop(ctx,0x8c61dfdau); fcopy(f,0u,15u);goto L8c61dfdc; /* fmov fr15,fr0 */
L8c61dfdc: sa_motion_publish_stop(ctx,0x8c61dfdcu); fcopy(f,2u,15u);goto L8c61dfde; /* fmov fr15,fr2 */
L8c61dfde: sa_motion_publish_stop(ctx,0x8c61dfdeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61dfe0; /* fmac fr0,fr3,fr2 */
L8c61dfe0: sa_motion_publish_stop(ctx,0x8c61dfe0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61dfe2; /* fmov.s fr2,@-r8 */
L8c61dfe2: sa_motion_publish_stop(ctx,0x8c61dfe2u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61dfe4; /* mov.l @r2,r3 */
L8c61dfe4: sa_motion_publish_stop(ctx,0x8c61dfe4u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dfe6; /* fmov.s @(r0,r13),fr0 */
L8c61dfe6: sa_motion_publish_stop(ctx,0x8c61dfe6u); r[0]=16u;goto L8c61dfe8; /* mov #0x10,r0 */
L8c61dfe8: sa_motion_publish_stop(ctx,0x8c61dfe8u); r[2]=r[3];goto L8c61dfea; /* mov r3,r2 */
L8c61dfea: sa_motion_publish_stop(ctx,0x8c61dfeau); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61dfec; /* fmov.s @(r0,r2),fr3 */
L8c61dfec: sa_motion_publish_stop(ctx,0x8c61dfecu); r[1]=r[3];goto L8c61dfee; /* mov r3,r1 */
L8c61dfee: sa_motion_publish_stop(ctx,0x8c61dfeeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dff0; /* fmul fr0,fr3 */
L8c61dff0: sa_motion_publish_stop(ctx,0x8c61dff0u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61dff2; /* fmov.s @r1,fr2 */
L8c61dff2: sa_motion_publish_stop(ctx,0x8c61dff2u); r[0]=8u;goto L8c61dff4; /* mov #0x8,r0 */
L8c61dff4: sa_motion_publish_stop(ctx,0x8c61dff4u); fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61dff6; /* fmov.s @r13,fr0 */
L8c61dff6: sa_motion_publish_stop(ctx,0x8c61dff6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61dff8; /* fmac fr0,fr2,fr3 */
L8c61dff8: sa_motion_publish_stop(ctx,0x8c61dff8u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61dffa; /* fmov.s @(r0,r13),fr0 */
L8c61dffa: sa_motion_publish_stop(ctx,0x8c61dffau); r[0]=32u;goto L8c61dffc; /* mov #0x20,r0 */
L8c61dffc: sa_motion_publish_stop(ctx,0x8c61dffcu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61dffe; /* fmov.s @(r0,r3),fr2 */
L8c61dffe: sa_motion_publish_stop(ctx,0x8c61dffeu); r[0]=24u;goto L8c61e000; /* mov #0x18,r0 */
L8c61e000: sa_motion_publish_stop(ctx,0x8c61e000u); r[3]=rd(m,bus,&io_status,0x8c61e1a4u,4);if(io_status)return io_status;goto L8c61e002; /* mov.l 0x8c61e1a4,r3 */
L8c61e002: sa_motion_publish_stop(ctx,0x8c61e002u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e004; /* fmac fr0,fr2,fr3 */
L8c61e004: sa_motion_publish_stop(ctx,0x8c61e004u); fcopy(f,0u,15u);goto L8c61e006; /* fmov fr15,fr0 */
L8c61e006: sa_motion_publish_stop(ctx,0x8c61e006u); fcopy(f,2u,15u);goto L8c61e008; /* fmov fr15,fr2 */
L8c61e008: sa_motion_publish_stop(ctx,0x8c61e008u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e00a; /* fmac fr0,fr3,fr2 */
L8c61e00a: sa_motion_publish_stop(ctx,0x8c61e00au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e00c; /* fmov.s fr2,@-r8 */
L8c61e00c: sa_motion_publish_stop(ctx,0x8c61e00cu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e00e; /* fmov.s @(r0,r13),fr3 */
L8c61e00e: sa_motion_publish_stop(ctx,0x8c61e00eu); r[0]=20u;goto L8c61e010; /* mov #0x14,r0 */
L8c61e010: sa_motion_publish_stop(ctx,0x8c61e010u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e012; /* fmov.s fr3,@-r8 */
L8c61e012: sa_motion_publish_stop(ctx,0x8c61e012u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e014; /* fmov.s @(r0,r13),fr3 */
L8c61e014: sa_motion_publish_stop(ctx,0x8c61e014u); r[0]=16u;goto L8c61e016; /* mov #0x10,r0 */
L8c61e016: sa_motion_publish_stop(ctx,0x8c61e016u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e018; /* fmov.s fr3,@-r8 */
L8c61e018: sa_motion_publish_stop(ctx,0x8c61e018u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e01a; /* fmov.s @(r0,r13),fr3 */
L8c61e01a: sa_motion_publish_stop(ctx,0x8c61e01au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e01c; /* fmov.s fr3,@-r8 */
L8c61e01c: sa_motion_publish_stop(ctx,0x8c61e01cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e01e; /* mov.l r3,@-r8 */
L8c61e01e: sa_motion_publish_stop(ctx,0x8c61e01eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e020; /* pref @r8 */
L8c61e020: sa_motion_publish_stop(ctx,0x8c61e020u); r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61e022; /* mov.l 0x8c61e1a0,r2 */
L8c61e022: sa_motion_publish_stop(ctx,0x8c61e022u); r[0]=4u;goto L8c61e024; /* mov #0x4,r0 */
L8c61e024: sa_motion_publish_stop(ctx,0x8c61e024u); r[8]+=60u;goto L8c61e026; /* add #0x3c,r8 */
L8c61e026: sa_motion_publish_stop(ctx,0x8c61e026u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61e028; /* fmov.s fr4,@-r8 */
L8c61e028: sa_motion_publish_stop(ctx,0x8c61e028u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e02a; /* mov.l @r2,r3 */
L8c61e02a: sa_motion_publish_stop(ctx,0x8c61e02au); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e02c; /* fmov.s @(r0,r12),fr0 */
L8c61e02c: sa_motion_publish_stop(ctx,0x8c61e02cu); r[0]=20u;goto L8c61e02e; /* mov #0x14,r0 */
L8c61e02e: sa_motion_publish_stop(ctx,0x8c61e02eu); r[2]=r[3];goto L8c61e030; /* mov r3,r2 */
L8c61e030: sa_motion_publish_stop(ctx,0x8c61e030u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e032; /* fmov.s @(r0,r2),fr3 */
L8c61e032: sa_motion_publish_stop(ctx,0x8c61e032u); r[1]=r[3];goto L8c61e034; /* mov r3,r1 */
L8c61e034: sa_motion_publish_stop(ctx,0x8c61e034u); r[0]=4u;goto L8c61e036; /* mov #0x4,r0 */
L8c61e036: sa_motion_publish_stop(ctx,0x8c61e036u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e038; /* fmul fr0,fr3 */
L8c61e038: sa_motion_publish_stop(ctx,0x8c61e038u); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e03a; /* fmov.s @(r0,r1),fr2 */
L8c61e03a: sa_motion_publish_stop(ctx,0x8c61e03au); fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61e03c; /* fmov.s @r12,fr0 */
L8c61e03c: sa_motion_publish_stop(ctx,0x8c61e03cu); r[0]=8u;goto L8c61e03e; /* mov #0x8,r0 */
L8c61e03e: sa_motion_publish_stop(ctx,0x8c61e03eu); r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61e040; /* mov.l 0x8c61e1a0,r2 */
L8c61e040: sa_motion_publish_stop(ctx,0x8c61e040u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e042; /* fmac fr0,fr2,fr3 */
L8c61e042: sa_motion_publish_stop(ctx,0x8c61e042u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e044; /* fmov.s @(r0,r12),fr0 */
L8c61e044: sa_motion_publish_stop(ctx,0x8c61e044u); r[0]=36u;goto L8c61e046; /* mov #0x24,r0 */
L8c61e046: sa_motion_publish_stop(ctx,0x8c61e046u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e048; /* fmov.s @(r0,r3),fr2 */
L8c61e048: sa_motion_publish_stop(ctx,0x8c61e048u); r[0]=4u;goto L8c61e04a; /* mov #0x4,r0 */
L8c61e04a: sa_motion_publish_stop(ctx,0x8c61e04au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e04c; /* fmac fr0,fr2,fr3 */
L8c61e04c: sa_motion_publish_stop(ctx,0x8c61e04cu); fcopy(f,0u,15u);goto L8c61e04e; /* fmov fr15,fr0 */
L8c61e04e: sa_motion_publish_stop(ctx,0x8c61e04eu); fcopy(f,2u,15u);goto L8c61e050; /* fmov fr15,fr2 */
L8c61e050: sa_motion_publish_stop(ctx,0x8c61e050u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e052; /* fmac fr0,fr3,fr2 */
L8c61e052: sa_motion_publish_stop(ctx,0x8c61e052u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e054; /* fmov.s fr2,@-r8 */
L8c61e054: sa_motion_publish_stop(ctx,0x8c61e054u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e056; /* mov.l @r2,r3 */
L8c61e056: sa_motion_publish_stop(ctx,0x8c61e056u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e058; /* fmov.s @(r0,r12),fr0 */
L8c61e058: sa_motion_publish_stop(ctx,0x8c61e058u); r[0]=16u;goto L8c61e05a; /* mov #0x10,r0 */
L8c61e05a: sa_motion_publish_stop(ctx,0x8c61e05au); r[2]=r[3];goto L8c61e05c; /* mov r3,r2 */
L8c61e05c: sa_motion_publish_stop(ctx,0x8c61e05cu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e05e; /* fmov.s @(r0,r2),fr3 */
L8c61e05e: sa_motion_publish_stop(ctx,0x8c61e05eu); r[1]=r[3];goto L8c61e060; /* mov r3,r1 */
L8c61e060: sa_motion_publish_stop(ctx,0x8c61e060u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e062; /* fmul fr0,fr3 */
L8c61e062: sa_motion_publish_stop(ctx,0x8c61e062u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61e064; /* fmov.s @r1,fr2 */
L8c61e064: sa_motion_publish_stop(ctx,0x8c61e064u); fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61e066; /* fmov.s @r12,fr0 */
L8c61e066: sa_motion_publish_stop(ctx,0x8c61e066u); r[0]=8u;goto L8c61e068; /* mov #0x8,r0 */
L8c61e068: sa_motion_publish_stop(ctx,0x8c61e068u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e06a; /* fmac fr0,fr2,fr3 */
L8c61e06a: sa_motion_publish_stop(ctx,0x8c61e06au); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e06c; /* fmov.s @(r0,r12),fr0 */
L8c61e06c: sa_motion_publish_stop(ctx,0x8c61e06cu); r[0]=32u;goto L8c61e06e; /* mov #0x20,r0 */
L8c61e06e: sa_motion_publish_stop(ctx,0x8c61e06eu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e070; /* fmov.s @(r0,r3),fr2 */
L8c61e070: sa_motion_publish_stop(ctx,0x8c61e070u); r[0]=24u;goto L8c61e072; /* mov #0x18,r0 */
L8c61e072: sa_motion_publish_stop(ctx,0x8c61e072u); r[3]=rd(m,bus,&io_status,0x8c61e1a4u,4);if(io_status)return io_status;goto L8c61e074; /* mov.l 0x8c61e1a4,r3 */
L8c61e074: sa_motion_publish_stop(ctx,0x8c61e074u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e076; /* fmac fr0,fr2,fr3 */
L8c61e076: sa_motion_publish_stop(ctx,0x8c61e076u); fcopy(f,0u,15u);goto L8c61e078; /* fmov fr15,fr0 */
L8c61e078: sa_motion_publish_stop(ctx,0x8c61e078u); fcopy(f,2u,15u);goto L8c61e07a; /* fmov fr15,fr2 */
L8c61e07a: sa_motion_publish_stop(ctx,0x8c61e07au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e07c; /* fmac fr0,fr3,fr2 */
L8c61e07c: sa_motion_publish_stop(ctx,0x8c61e07cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e07e; /* fmov.s fr2,@-r8 */
L8c61e07e: sa_motion_publish_stop(ctx,0x8c61e07eu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e080; /* fmov.s @(r0,r12),fr3 */
L8c61e080: sa_motion_publish_stop(ctx,0x8c61e080u); r[0]=20u;goto L8c61e082; /* mov #0x14,r0 */
L8c61e082: sa_motion_publish_stop(ctx,0x8c61e082u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e084; /* fmov.s fr3,@-r8 */
L8c61e084: sa_motion_publish_stop(ctx,0x8c61e084u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e086; /* fmov.s @(r0,r12),fr3 */
L8c61e086: sa_motion_publish_stop(ctx,0x8c61e086u); r[0]=16u;goto L8c61e088; /* mov #0x10,r0 */
L8c61e088: sa_motion_publish_stop(ctx,0x8c61e088u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e08a; /* fmov.s fr3,@-r8 */
L8c61e08a: sa_motion_publish_stop(ctx,0x8c61e08au); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e08c; /* fmov.s @(r0,r12),fr3 */
L8c61e08c: sa_motion_publish_stop(ctx,0x8c61e08cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e08e; /* fmov.s fr3,@-r8 */
L8c61e08e: sa_motion_publish_stop(ctx,0x8c61e08eu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e090; /* mov.l r3,@-r8 */
L8c61e090: sa_motion_publish_stop(ctx,0x8c61e090u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e092; /* pref @r8 */
L8c61e092: sa_motion_publish_stop(ctx,0x8c61e092u); r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61e094; /* mov.l 0x8c61e1a0,r2 */
L8c61e094: sa_motion_publish_stop(ctx,0x8c61e094u); r[0]=4u;goto L8c61e096; /* mov #0x4,r0 */
L8c61e096: sa_motion_publish_stop(ctx,0x8c61e096u); r[8]+=60u;goto L8c61e098; /* add #0x3c,r8 */
L8c61e098: sa_motion_publish_stop(ctx,0x8c61e098u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61e09a; /* fmov.s fr4,@-r8 */
L8c61e09a: sa_motion_publish_stop(ctx,0x8c61e09au); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e09c; /* mov.l @r2,r3 */
L8c61e09c: sa_motion_publish_stop(ctx,0x8c61e09cu); fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e09e; /* fmov.s @(r0,r11),fr0 */
L8c61e09e: sa_motion_publish_stop(ctx,0x8c61e09eu); r[0]=20u;goto L8c61e0a0; /* mov #0x14,r0 */
L8c61e0a0: sa_motion_publish_stop(ctx,0x8c61e0a0u); r[2]=r[3];goto L8c61e0a2; /* mov r3,r2 */
L8c61e0a2: sa_motion_publish_stop(ctx,0x8c61e0a2u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e0a4; /* fmov.s @(r0,r2),fr3 */
L8c61e0a4: sa_motion_publish_stop(ctx,0x8c61e0a4u); r[1]=r[3];goto L8c61e0a6; /* mov r3,r1 */
L8c61e0a6: sa_motion_publish_stop(ctx,0x8c61e0a6u); r[0]=4u;goto L8c61e0a8; /* mov #0x4,r0 */
L8c61e0a8: sa_motion_publish_stop(ctx,0x8c61e0a8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e0aa; /* fmul fr0,fr3 */
L8c61e0aa: sa_motion_publish_stop(ctx,0x8c61e0aau); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e0ac; /* fmov.s @(r0,r1),fr2 */
L8c61e0ac: sa_motion_publish_stop(ctx,0x8c61e0acu); fload(m,bus,&io_status,f,0u,r[11]);if(io_status)return io_status;goto L8c61e0ae; /* fmov.s @r11,fr0 */
L8c61e0ae: sa_motion_publish_stop(ctx,0x8c61e0aeu); r[0]=8u;goto L8c61e0b0; /* mov #0x8,r0 */
L8c61e0b0: sa_motion_publish_stop(ctx,0x8c61e0b0u); r[2]=rd(m,bus,&io_status,0x8c61e1a0u,4);if(io_status)return io_status;goto L8c61e0b2; /* mov.l 0x8c61e1a0,r2 */
L8c61e0b2: sa_motion_publish_stop(ctx,0x8c61e0b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e0b4; /* fmac fr0,fr2,fr3 */
L8c61e0b4: sa_motion_publish_stop(ctx,0x8c61e0b4u); fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e0b6; /* fmov.s @(r0,r11),fr0 */
L8c61e0b6: sa_motion_publish_stop(ctx,0x8c61e0b6u); r[0]=36u;goto L8c61e0b8; /* mov #0x24,r0 */
L8c61e0b8: sa_motion_publish_stop(ctx,0x8c61e0b8u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e0ba; /* fmov.s @(r0,r3),fr2 */
L8c61e0ba: sa_motion_publish_stop(ctx,0x8c61e0bau); r[0]=4u;goto L8c61e0bc; /* mov #0x4,r0 */
L8c61e0bc: sa_motion_publish_stop(ctx,0x8c61e0bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e0be; /* fmac fr0,fr2,fr3 */
L8c61e0be: sa_motion_publish_stop(ctx,0x8c61e0beu); fcopy(f,0u,15u);goto L8c61e0c0; /* fmov fr15,fr0 */
L8c61e0c0: sa_motion_publish_stop(ctx,0x8c61e0c0u); fcopy(f,2u,15u);goto L8c61e0c2; /* fmov fr15,fr2 */
L8c61e0c2: sa_motion_publish_stop(ctx,0x8c61e0c2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e0c4; /* fmac fr0,fr3,fr2 */
L8c61e0c4: sa_motion_publish_stop(ctx,0x8c61e0c4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e0c6; /* fmov.s fr2,@-r8 */
L8c61e0c6: sa_motion_publish_stop(ctx,0x8c61e0c6u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e0c8; /* mov.l @r2,r3 */
L8c61e0c8: sa_motion_publish_stop(ctx,0x8c61e0c8u); fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e0ca; /* fmov.s @(r0,r11),fr0 */
L8c61e0ca: sa_motion_publish_stop(ctx,0x8c61e0cau); r[0]=16u;goto L8c61e0cc; /* mov #0x10,r0 */
L8c61e0cc: sa_motion_publish_stop(ctx,0x8c61e0ccu); r[2]=r[3];goto L8c61e0ce; /* mov r3,r2 */
L8c61e0ce: sa_motion_publish_stop(ctx,0x8c61e0ceu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e0d0; /* fmov.s @(r0,r2),fr3 */
L8c61e0d0: sa_motion_publish_stop(ctx,0x8c61e0d0u); r[1]=r[3];goto L8c61e0d2; /* mov r3,r1 */
L8c61e0d2: sa_motion_publish_stop(ctx,0x8c61e0d2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e0d4; /* fmul fr0,fr3 */
L8c61e0d4: sa_motion_publish_stop(ctx,0x8c61e0d4u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61e0d6; /* fmov.s @r1,fr2 */
L8c61e0d6: sa_motion_publish_stop(ctx,0x8c61e0d6u); fload(m,bus,&io_status,f,0u,r[11]);if(io_status)return io_status;goto L8c61e0d8; /* fmov.s @r11,fr0 */
L8c61e0d8: sa_motion_publish_stop(ctx,0x8c61e0d8u); r[0]=8u;goto L8c61e0da; /* mov #0x8,r0 */
L8c61e0da: sa_motion_publish_stop(ctx,0x8c61e0dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e0dc; /* fmac fr0,fr2,fr3 */
L8c61e0dc: sa_motion_publish_stop(ctx,0x8c61e0dcu); fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e0de; /* fmov.s @(r0,r11),fr0 */
L8c61e0de: sa_motion_publish_stop(ctx,0x8c61e0deu); r[0]=32u;goto L8c61e0e0; /* mov #0x20,r0 */
L8c61e0e0: sa_motion_publish_stop(ctx,0x8c61e0e0u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e0e2; /* fmov.s @(r0,r3),fr2 */
L8c61e0e2: sa_motion_publish_stop(ctx,0x8c61e0e2u); r[0]=24u;goto L8c61e0e4; /* mov #0x18,r0 */
L8c61e0e4: sa_motion_publish_stop(ctx,0x8c61e0e4u); r[3]=rd(m,bus,&io_status,0x8c61e1a8u,4);if(io_status)return io_status;goto L8c61e0e6; /* mov.l 0x8c61e1a8,r3 */
L8c61e0e6: sa_motion_publish_stop(ctx,0x8c61e0e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e0e8; /* fmac fr0,fr2,fr3 */
L8c61e0e8: sa_motion_publish_stop(ctx,0x8c61e0e8u); fcopy(f,0u,15u);goto L8c61e0ea; /* fmov fr15,fr0 */
L8c61e0ea: sa_motion_publish_stop(ctx,0x8c61e0eau); fcopy(f,2u,15u);goto L8c61e0ec; /* fmov fr15,fr2 */
L8c61e0ec: sa_motion_publish_stop(ctx,0x8c61e0ecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e0ee; /* fmac fr0,fr3,fr2 */
L8c61e0ee: sa_motion_publish_stop(ctx,0x8c61e0eeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e0f0; /* fmov.s fr2,@-r8 */
L8c61e0f0: sa_motion_publish_stop(ctx,0x8c61e0f0u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e0f2; /* fmov.s @(r0,r11),fr3 */
L8c61e0f2: sa_motion_publish_stop(ctx,0x8c61e0f2u); r[0]=20u;goto L8c61e0f4; /* mov #0x14,r0 */
L8c61e0f4: sa_motion_publish_stop(ctx,0x8c61e0f4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e0f6; /* fmov.s fr3,@-r8 */
L8c61e0f6: sa_motion_publish_stop(ctx,0x8c61e0f6u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e0f8; /* fmov.s @(r0,r11),fr3 */
L8c61e0f8: sa_motion_publish_stop(ctx,0x8c61e0f8u); r[0]=16u;goto L8c61e0fa; /* mov #0x10,r0 */
L8c61e0fa: sa_motion_publish_stop(ctx,0x8c61e0fau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e0fc; /* fmov.s fr3,@-r8 */
L8c61e0fc: sa_motion_publish_stop(ctx,0x8c61e0fcu); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e0fe; /* fmov.s @(r0,r11),fr3 */
L8c61e0fe: sa_motion_publish_stop(ctx,0x8c61e0feu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e100; /* fmov.s fr3,@-r8 */
L8c61e100: sa_motion_publish_stop(ctx,0x8c61e100u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e102; /* mov.l r3,@-r8 */
L8c61e102: sa_motion_publish_stop(ctx,0x8c61e102u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e104; /* pref @r8 */
L8c61e104: sa_motion_publish_stop(ctx,0x8c61e104u); r[2]=rd(m,bus,&io_status,0x8c61e1acu,4);if(io_status)return io_status;goto L8c61e106; /* mov.l 0x8c61e1ac,r2 */
L8c61e106: sa_motion_publish_stop(ctx,0x8c61e106u); r[8]+=32u;goto L8c61e108; /* add #0x20,r8 */
L8c61e108: sa_motion_publish_stop(ctx,0x8c61e108u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e10a; /* mov.l @r2,r3 */
L8c61e10a: sa_motion_publish_stop(ctx,0x8c61e10au); r[3]+=4u;goto L8c61e10c; /* add #0x4,r3 */
L8c61e10c: sa_motion_publish_stop(ctx,0x8c61e10cu); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61e10e; /* mov.l r3,@r2 */
L8c61e10e: sa_motion_publish_stop(ctx,0x8c61e10eu); r[9]+=1u;goto L8c61e110; /* add #0x1,r9 */
L8c61e110: sa_motion_publish_stop(ctx,0x8c61e110u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61e112; /* mov.l @r15,r2 */
L8c61e112: sa_motion_publish_stop(ctx,0x8c61e112u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61e114; /* mov.w @(0x2,r2),r0 */
L8c61e114: sa_motion_publish_stop(ctx,0x8c61e114u); r[0]=r[0]&65535u;goto L8c61e116; /* extu.w r0,r0 */
L8c61e116: sa_motion_publish_stop(ctx,0x8c61e116u); *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61e118; /* cmp/ge r0,r9 */
L8c61e118: sa_motion_publish_stop(ctx,0x8c61e118u); if((*sr&1u))goto L8c61e11e;goto L8c61e11a; /* bt 0x8c61e11e */
L8c61e11a: sa_motion_publish_stop(ctx,0x8c61e11cu); goto L8c61de7c; /* bra 0x8c61de7c */
L8c61e11e: sa_motion_publish_stop(ctx,0x8c61e120u); goto L8c61e4a8; /* bra 0x8c61e4a8 */
L8c61e122: sa_motion_publish_stop(ctx,0x8c61e122u); r[11]=2u;goto L8c61e124; /* mov #0x2,r11 */
L8c61e124: sa_motion_publish_stop(ctx,0x8c61e126u); r[12]=r[4];goto L8c61e41c; /* bra 0x8c61e41c */
L8c61e128: sa_motion_publish_stop(ctx,0x8c61e128u); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e12a; /* mov.w @r10+,r14 */
L8c61e12a: sa_motion_publish_stop(ctx,0x8c61e12au); r[3]=rd(m,bus,&io_status,r[13],4);if(io_status)return io_status;goto L8c61e12c; /* mov.l @r13,r3 */
L8c61e12c: sa_motion_publish_stop(ctx,0x8c61e12cu); r[14]=r[14]&65535u;goto L8c61e12e; /* extu.w r14,r14 */
L8c61e12e: sa_motion_publish_stop(ctx,0x8c61e12eu); r[3]+=r[14];goto L8c61e130; /* add r14,r3 */
L8c61e130: sa_motion_publish_stop(ctx,0x8c61e130u); wr(m,bus,&io_status,r[13],r[3],4);if(io_status)return io_status;goto L8c61e132; /* mov.l r3,@r13 */
L8c61e132: sa_motion_publish_stop(ctx,0x8c61e132u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61e134; /* mov.w @(0x2,r10),r0 */
L8c61e134: sa_motion_publish_stop(ctx,0x8c61e134u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61e136; /* mov.w @r10,r4 */
L8c61e136: sa_motion_publish_stop(ctx,0x8c61e136u); r[3]=rd(m,bus,&io_status,0x8c61e18cu,4);if(io_status)return io_status;goto L8c61e138; /* mov.l 0x8c61e18c,r3 */
L8c61e138: sa_motion_publish_stop(ctx,0x8c61e138u); r[6]=r[0]&65535u;goto L8c61e13a; /* extu.w r0,r6 */
L8c61e13a: sa_motion_publish_stop(ctx,0x8c61e13au); r[0]=sx(m,bus,&io_status,r[10]+0x4u,2);if(io_status)return io_status;goto L8c61e13c; /* mov.w @(0x4,r10),r0 */
L8c61e13c: sa_motion_publish_stop(ctx,0x8c61e13cu); r[6]<<=2;goto L8c61e13e; /* shll2 r6 */
L8c61e13e: sa_motion_publish_stop(ctx,0x8c61e13eu); r[4]=r[4]&65535u;goto L8c61e140; /* extu.w r4,r4 */
L8c61e140: sa_motion_publish_stop(ctx,0x8c61e140u); r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e142; /* mov.l @r3,r7 */
L8c61e142: sa_motion_publish_stop(ctx,0x8c61e142u); r[6]<<=2;goto L8c61e144; /* shll2 r6 */
L8c61e144: sa_motion_publish_stop(ctx,0x8c61e144u); r[4]<<=2;goto L8c61e146; /* shll2 r4 */
L8c61e146: sa_motion_publish_stop(ctx,0x8c61e146u); r[5]=r[0]&65535u;goto L8c61e148; /* extu.w r0,r5 */
L8c61e148: sa_motion_publish_stop(ctx,0x8c61e148u); r[4]<<=2;goto L8c61e14a; /* shll2 r4 */
L8c61e14a: sa_motion_publish_stop(ctx,0x8c61e14au); r[0]=16u;goto L8c61e14c; /* mov #0x10,r0 */
L8c61e14c: sa_motion_publish_stop(ctx,0x8c61e14cu); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61e14e; /* shll r4 */
L8c61e14e: sa_motion_publish_stop(ctx,0x8c61e14eu); *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c61e150; /* shll r6 */
L8c61e150: sa_motion_publish_stop(ctx,0x8c61e150u); r[6]+=r[7];goto L8c61e152; /* add r7,r6 */
L8c61e152: sa_motion_publish_stop(ctx,0x8c61e152u); fload(m,bus,&io_status,f,5u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e154; /* fmov.s @(r0,r6),fr5 */
L8c61e154: sa_motion_publish_stop(ctx,0x8c61e154u); r[4]+=r[7];goto L8c61e156; /* add r7,r4 */
L8c61e156: sa_motion_publish_stop(ctx,0x8c61e156u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e158; /* fmov.s @(r0,r4),fr3 */
L8c61e158: sa_motion_publish_stop(ctx,0x8c61e158u); r[5]<<=2;goto L8c61e15a; /* shll2 r5 */
L8c61e15a: sa_motion_publish_stop(ctx,0x8c61e15au); fcopy(f,2u,5u);goto L8c61e15c; /* fmov fr5,fr2 */
L8c61e15c: sa_motion_publish_stop(ctx,0x8c61e15cu); r[0]=20u;goto L8c61e15e; /* mov #0x14,r0 */
L8c61e15e: sa_motion_publish_stop(ctx,0x8c61e15eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61e160; /* fsub fr3,fr2 */
L8c61e160: sa_motion_publish_stop(ctx,0x8c61e160u); r[5]<<=2;goto L8c61e162; /* shll2 r5 */
L8c61e162: sa_motion_publish_stop(ctx,0x8c61e162u); fload(m,bus,&io_status,f,4u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e164; /* fmov.s @(r0,r6),fr4 */
L8c61e164: sa_motion_publish_stop(ctx,0x8c61e164u); *sr=(*sr&~1u)|(r[5]>>31);r[5]<<=1;goto L8c61e166; /* shll r5 */
L8c61e166: sa_motion_publish_stop(ctx,0x8c61e166u); r[5]+=r[7];goto L8c61e168; /* add r7,r5 */
L8c61e168: sa_motion_publish_stop(ctx,0x8c61e168u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61e16a; /* fmov.s @(r0,r5),fr3 */
L8c61e16a: sa_motion_publish_stop(ctx,0x8c61e16au); fcopy(f,1u,4u);goto L8c61e16c; /* fmov fr4,fr1 */
L8c61e16c: sa_motion_publish_stop(ctx,0x8c61e16cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e16e; /* fsub fr4,fr3 */
L8c61e16e: sa_motion_publish_stop(ctx,0x8c61e16eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e170; /* fmul fr3,fr2 */
L8c61e170: sa_motion_publish_stop(ctx,0x8c61e170u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e172; /* fmov.s @(r0,r4),fr3 */
L8c61e172: sa_motion_publish_stop(ctx,0x8c61e172u); r[0]=16u;goto L8c61e174; /* mov #0x10,r0 */
L8c61e174: sa_motion_publish_stop(ctx,0x8c61e174u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61e176; /* fsub fr3,fr1 */
L8c61e176: sa_motion_publish_stop(ctx,0x8c61e176u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61e178; /* fmov.s @(r0,r5),fr3 */
L8c61e178: sa_motion_publish_stop(ctx,0x8c61e178u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e17a; /* fsub fr5,fr3 */
L8c61e17a: sa_motion_publish_stop(ctx,0x8c61e17au); fcopy(f,4u,2u);goto L8c61e17c; /* fmov fr2,fr4 */
L8c61e17c: sa_motion_publish_stop(ctx,0x8c61e17cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e17e; /* fmul fr3,fr1 */
L8c61e17e: sa_motion_publish_stop(ctx,0x8c61e17eu); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61e180; /* fldi0 fr3 */
L8c61e180: sa_motion_publish_stop(ctx,0x8c61e180u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e182; /* fsub fr1,fr4 */
L8c61e182: sa_motion_publish_stop(ctx,0x8c61e182u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61e184; /* fcmp/gt fr3,fr4 */
L8c61e184: sa_motion_publish_stop(ctx,0x8c61e184u); if(!(*sr&1u))goto L8c61e1b0;goto L8c61e186; /* bf 0x8c61e1b0 */
L8c61e186: sa_motion_publish_stop(ctx,0x8c61e186u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61e188; /* shll r14 */
L8c61e188: sa_motion_publish_stop(ctx,0x8c61e18au); r[10]+=r[14];goto L8c61e41a; /* bra 0x8c61e41a */
L8c61e1b0: sa_motion_publish_stop(ctx,0x8c61e1b0u); r[0]=4u;goto L8c61e1b2; /* mov #0x4,r0 */
L8c61e1b2: sa_motion_publish_stop(ctx,0x8c61e1b2u); r[3]=rd(m,bus,&io_status,0x8c61e4c8u,4);if(io_status)return io_status;goto L8c61e1b4; /* mov.l 0x8c61e4c8,r3 */
L8c61e1b4: sa_motion_publish_stop(ctx,0x8c61e1b4u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e1b6; /* mov.l @r3,r2 */
L8c61e1b6: sa_motion_publish_stop(ctx,0x8c61e1b6u); r[2]+=r[14];goto L8c61e1b8; /* add r14,r2 */
L8c61e1b8: sa_motion_publish_stop(ctx,0x8c61e1b8u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61e1ba; /* mov.l r2,@r3 */
L8c61e1ba: sa_motion_publish_stop(ctx,0x8c61e1bau); fload(m,bus,&io_status,f,3u,r[6]);if(io_status)return io_status;goto L8c61e1bc; /* fmov.s @r6,fr3 */
L8c61e1bc: sa_motion_publish_stop(ctx,0x8c61e1bcu); fload(m,bus,&io_status,f,2u,r[4]);if(io_status)return io_status;goto L8c61e1be; /* fmov.s @r4,fr2 */
L8c61e1be: sa_motion_publish_stop(ctx,0x8c61e1beu); r[3]=rd(m,bus,&io_status,0x8c61e4ccu,4);if(io_status)return io_status;goto L8c61e1c0; /* mov.l 0x8c61e4cc,r3 */
L8c61e1c0: sa_motion_publish_stop(ctx,0x8c61e1c0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e1c2; /* fadd fr3,fr2 */
L8c61e1c2: sa_motion_publish_stop(ctx,0x8c61e1c2u); fload(m,bus,&io_status,f,3u,r[5]);if(io_status)return io_status;goto L8c61e1c4; /* fmov.s @r5,fr3 */
L8c61e1c4: sa_motion_publish_stop(ctx,0x8c61e1c4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e1c6; /* fadd fr3,fr2 */
L8c61e1c6: sa_motion_publish_stop(ctx,0x8c61e1c6u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e1c8; /* fmov fr2,@(r0,r15) */
L8c61e1c8: sa_motion_publish_stop(ctx,0x8c61e1c8u); r[0]=4u;goto L8c61e1ca; /* mov #0x4,r0 */
L8c61e1ca: sa_motion_publish_stop(ctx,0x8c61e1cau); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e1cc; /* fmov.s @(r0,r4),fr2 */
L8c61e1cc: sa_motion_publish_stop(ctx,0x8c61e1ccu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e1ce; /* fmov.s @(r0,r6),fr3 */
L8c61e1ce: sa_motion_publish_stop(ctx,0x8c61e1ceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e1d0; /* fadd fr3,fr2 */
L8c61e1d0: sa_motion_publish_stop(ctx,0x8c61e1d0u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61e1d2; /* fmov.s @(r0,r5),fr3 */
L8c61e1d2: sa_motion_publish_stop(ctx,0x8c61e1d2u); r[0]=8u;goto L8c61e1d4; /* mov #0x8,r0 */
L8c61e1d4: sa_motion_publish_stop(ctx,0x8c61e1d4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e1d6; /* fadd fr3,fr2 */
L8c61e1d6: sa_motion_publish_stop(ctx,0x8c61e1d6u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e1d8; /* fmov fr2,@(r0,r15) */
L8c61e1d8: sa_motion_publish_stop(ctx,0x8c61e1d8u); r[0]=8u;goto L8c61e1da; /* mov #0x8,r0 */
L8c61e1da: sa_motion_publish_stop(ctx,0x8c61e1dau); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e1dc; /* fmov.s @(r0,r6),fr3 */
L8c61e1dc: sa_motion_publish_stop(ctx,0x8c61e1dcu); fload(m,bus,&io_status,f,2u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e1de; /* fmov.s @(r0,r4),fr2 */
L8c61e1de: sa_motion_publish_stop(ctx,0x8c61e1deu); r[4]=r[15];goto L8c61e1e0; /* mov r15,r4 */
L8c61e1e0: sa_motion_publish_stop(ctx,0x8c61e1e0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e1e2; /* fadd fr3,fr2 */
L8c61e1e2: sa_motion_publish_stop(ctx,0x8c61e1e2u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61e1e4; /* fmov.s @(r0,r5),fr3 */
L8c61e1e4: sa_motion_publish_stop(ctx,0x8c61e1e4u); r[0]=12u;goto L8c61e1e6; /* mov #0xc,r0 */
L8c61e1e6: sa_motion_publish_stop(ctx,0x8c61e1e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e1e8; /* fadd fr3,fr2 */
L8c61e1e8: sa_motion_publish_stop(ctx,0x8c61e1e8u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e1ea; /* fmov fr2,@(r0,r15) */
L8c61e1ea: sa_motion_publish_stop(ctx,0x8c61e1ecu); target=r[3];*pr=0x8c61e1eeu;r[4]+=4u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61e1ee; /* jsr @r3 */
L8c61e1ee: sa_motion_publish_stop(ctx,0x8c61e1eeu); r[3]=rd(m,bus,&io_status,0x8c61e4d0u,4);if(io_status)return io_status;goto L8c61e1f0; /* mov.l 0x8c61e4d0,r3 */
L8c61e1f0: sa_motion_publish_stop(ctx,0x8c61e1f0u); r[0]=4u;goto L8c61e1f2; /* mov #0x4,r0 */
L8c61e1f2: sa_motion_publish_stop(ctx,0x8c61e1f2u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e1f4; /* fmov.s @(r0,r15),fr3 */
L8c61e1f4: sa_motion_publish_stop(ctx,0x8c61e1f4u); r[0]=8u;goto L8c61e1f6; /* mov #0x8,r0 */
L8c61e1f6: sa_motion_publish_stop(ctx,0x8c61e1f6u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61e1f8; /* fmov.s @r3,fr2 */
L8c61e1f8: sa_motion_publish_stop(ctx,0x8c61e1f8u); r[3]+=8u;goto L8c61e1fa; /* add #0x8,r3 */
L8c61e1fa: sa_motion_publish_stop(ctx,0x8c61e1fau); r[2]=rd(m,bus,&io_status,0x8c61e4d4u,4);if(io_status)return io_status;goto L8c61e1fc; /* mov.l 0x8c61e4d4,r2 */
L8c61e1fc: sa_motion_publish_stop(ctx,0x8c61e1fcu); r[8]+=24u;goto L8c61e1fe; /* add #0x18,r8 */
L8c61e1fe: sa_motion_publish_stop(ctx,0x8c61e1feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e200; /* fmul fr3,fr2 */
L8c61e200: sa_motion_publish_stop(ctx,0x8c61e200u); r[1]=rd(m,bus,&io_status,0x8c61e4d8u,4);if(io_status)return io_status;goto L8c61e202; /* mov.l 0x8c61e4d8,r1 */
L8c61e202: sa_motion_publish_stop(ctx,0x8c61e202u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61e204; /* fmov.s @r2,fr3 */
L8c61e204: sa_motion_publish_stop(ctx,0x8c61e204u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61e206; /* fmov.s @r1,fr1 */
L8c61e206: sa_motion_publish_stop(ctx,0x8c61e206u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e208; /* mov.w @r10+,r4 */
L8c61e208: sa_motion_publish_stop(ctx,0x8c61e208u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e20a; /* fsub fr2,fr3 */
L8c61e20a: sa_motion_publish_stop(ctx,0x8c61e20au); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e20c; /* fmov.s @(r0,r15),fr2 */
L8c61e20c: sa_motion_publish_stop(ctx,0x8c61e20cu); r[0]=12u;goto L8c61e20e; /* mov #0xc,r0 */
L8c61e20e: sa_motion_publish_stop(ctx,0x8c61e20eu); r[1]=rd(m,bus,&io_status,0x8c61e4e0u,4);if(io_status)return io_status;goto L8c61e210; /* mov.l 0x8c61e4e0,r1 */
L8c61e210: sa_motion_publish_stop(ctx,0x8c61e210u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e212; /* fmul fr2,fr1 */
L8c61e212: sa_motion_publish_stop(ctx,0x8c61e212u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e214; /* fmov.s @(r0,r15),fr2 */
L8c61e214: sa_motion_publish_stop(ctx,0x8c61e214u); r[4]=r[4]&65535u;goto L8c61e216; /* extu.w r4,r4 */
L8c61e216: sa_motion_publish_stop(ctx,0x8c61e216u); r[0]=rd(m,bus,&io_status,0x8c61e4dcu,4);if(io_status)return io_status;goto L8c61e218; /* mov.l 0x8c61e4dc,r0 */
L8c61e218: sa_motion_publish_stop(ctx,0x8c61e218u); r[4]<<=2;goto L8c61e21a; /* shll2 r4 */
L8c61e21a: sa_motion_publish_stop(ctx,0x8c61e21au); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61e21c; /* mov.l @r0,r2 */
L8c61e21c: sa_motion_publish_stop(ctx,0x8c61e21cu); r[4]<<=2;goto L8c61e21e; /* shll2 r4 */
L8c61e21e: sa_motion_publish_stop(ctx,0x8c61e21eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e220; /* fsub fr1,fr3 */
L8c61e220: sa_motion_publish_stop(ctx,0x8c61e220u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61e222; /* fmov.s @r3,fr1 */
L8c61e222: sa_motion_publish_stop(ctx,0x8c61e222u); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61e224; /* mov.l @r1,r3 */
L8c61e224: sa_motion_publish_stop(ctx,0x8c61e224u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61e226; /* shll r4 */
L8c61e226: sa_motion_publish_stop(ctx,0x8c61e226u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e228; /* fmul fr2,fr1 */
L8c61e228: sa_motion_publish_stop(ctx,0x8c61e228u); r[4]+=r[2];goto L8c61e22a; /* add r2,r4 */
L8c61e22a: sa_motion_publish_stop(ctx,0x8c61e22au); fcopy(f,4u,3u);goto L8c61e22c; /* fmov fr3,fr4 */
L8c61e22c: sa_motion_publish_stop(ctx,0x8c61e22cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e22e; /* fsub fr1,fr4 */
L8c61e22e: sa_motion_publish_stop(ctx,0x8c61e22eu); r[0]=4u;goto L8c61e230; /* mov #0x4,r0 */
L8c61e230: sa_motion_publish_stop(ctx,0x8c61e230u); r[1]=r[3];goto L8c61e232; /* mov r3,r1 */
L8c61e232: sa_motion_publish_stop(ctx,0x8c61e232u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e234; /* fmov.s @(r0,r4),fr0 */
L8c61e234: sa_motion_publish_stop(ctx,0x8c61e234u); r[0]=20u;goto L8c61e236; /* mov #0x14,r0 */
L8c61e236: sa_motion_publish_stop(ctx,0x8c61e236u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e238; /* fmov.s @(r0,r1),fr3 */
L8c61e238: sa_motion_publish_stop(ctx,0x8c61e238u); r[0]=4u;goto L8c61e23a; /* mov #0x4,r0 */
L8c61e23a: sa_motion_publish_stop(ctx,0x8c61e23au); r[2]=r[3];goto L8c61e23c; /* mov r3,r2 */
L8c61e23c: sa_motion_publish_stop(ctx,0x8c61e23cu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e23e; /* fmov.s @(r0,r2),fr2 */
L8c61e23e: sa_motion_publish_stop(ctx,0x8c61e23eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e240; /* fmul fr0,fr3 */
L8c61e240: sa_motion_publish_stop(ctx,0x8c61e240u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e242; /* fmov.s @r4,fr0 */
L8c61e242: sa_motion_publish_stop(ctx,0x8c61e242u); r[0]=8u;goto L8c61e244; /* mov #0x8,r0 */
L8c61e244: sa_motion_publish_stop(ctx,0x8c61e244u); r[2]=rd(m,bus,&io_status,0x8c61e4e0u,4);if(io_status)return io_status;goto L8c61e246; /* mov.l 0x8c61e4e0,r2 */
L8c61e246: sa_motion_publish_stop(ctx,0x8c61e246u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e248; /* fmac fr0,fr2,fr3 */
L8c61e248: sa_motion_publish_stop(ctx,0x8c61e248u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e24a; /* fmov.s @(r0,r4),fr0 */
L8c61e24a: sa_motion_publish_stop(ctx,0x8c61e24au); r[0]=36u;goto L8c61e24c; /* mov #0x24,r0 */
L8c61e24c: sa_motion_publish_stop(ctx,0x8c61e24cu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e24e; /* fmov.s @(r0,r3),fr2 */
L8c61e24e: sa_motion_publish_stop(ctx,0x8c61e24eu); r[0]=4u;goto L8c61e250; /* mov #0x4,r0 */
L8c61e250: sa_motion_publish_stop(ctx,0x8c61e250u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e252; /* fmac fr0,fr2,fr3 */
L8c61e252: sa_motion_publish_stop(ctx,0x8c61e252u); fcopy(f,0u,15u);goto L8c61e254; /* fmov fr15,fr0 */
L8c61e254: sa_motion_publish_stop(ctx,0x8c61e254u); fcopy(f,2u,15u);goto L8c61e256; /* fmov fr15,fr2 */
L8c61e256: sa_motion_publish_stop(ctx,0x8c61e256u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e258; /* fmac fr0,fr3,fr2 */
L8c61e258: sa_motion_publish_stop(ctx,0x8c61e258u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e25a; /* fmov.s fr2,@-r8 */
L8c61e25a: sa_motion_publish_stop(ctx,0x8c61e25au); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e25c; /* mov.l @r2,r3 */
L8c61e25c: sa_motion_publish_stop(ctx,0x8c61e25cu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e25e; /* fmov.s @(r0,r4),fr0 */
L8c61e25e: sa_motion_publish_stop(ctx,0x8c61e25eu); r[0]=16u;goto L8c61e260; /* mov #0x10,r0 */
L8c61e260: sa_motion_publish_stop(ctx,0x8c61e260u); r[2]=r[3];goto L8c61e262; /* mov r3,r2 */
L8c61e262: sa_motion_publish_stop(ctx,0x8c61e262u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e264; /* fmov.s @(r0,r2),fr3 */
L8c61e264: sa_motion_publish_stop(ctx,0x8c61e264u); r[1]=r[3];goto L8c61e266; /* mov r3,r1 */
L8c61e266: sa_motion_publish_stop(ctx,0x8c61e266u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61e268; /* fmov.s @r1,fr2 */
L8c61e268: sa_motion_publish_stop(ctx,0x8c61e268u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e26a; /* fmul fr0,fr3 */
L8c61e26a: sa_motion_publish_stop(ctx,0x8c61e26au); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e26c; /* fmov.s @r4,fr0 */
L8c61e26c: sa_motion_publish_stop(ctx,0x8c61e26cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e26e; /* fmac fr0,fr2,fr3 */
L8c61e26e: sa_motion_publish_stop(ctx,0x8c61e26eu); r[0]=8u;goto L8c61e270; /* mov #0x8,r0 */
L8c61e270: sa_motion_publish_stop(ctx,0x8c61e270u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e272; /* fmov.s @(r0,r4),fr0 */
L8c61e272: sa_motion_publish_stop(ctx,0x8c61e272u); r[0]=32u;goto L8c61e274; /* mov #0x20,r0 */
L8c61e274: sa_motion_publish_stop(ctx,0x8c61e274u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e276; /* fmov.s @(r0,r3),fr2 */
L8c61e276: sa_motion_publish_stop(ctx,0x8c61e276u); r[0]=24u;goto L8c61e278; /* mov #0x18,r0 */
L8c61e278: sa_motion_publish_stop(ctx,0x8c61e278u); r[3]=rd(m,bus,&io_status,0x8c61e4e4u,4);if(io_status)return io_status;goto L8c61e27a; /* mov.l 0x8c61e4e4,r3 */
L8c61e27a: sa_motion_publish_stop(ctx,0x8c61e27au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e27c; /* fmac fr0,fr2,fr3 */
L8c61e27c: sa_motion_publish_stop(ctx,0x8c61e27cu); fcopy(f,0u,15u);goto L8c61e27e; /* fmov fr15,fr0 */
L8c61e27e: sa_motion_publish_stop(ctx,0x8c61e27eu); fcopy(f,2u,15u);goto L8c61e280; /* fmov fr15,fr2 */
L8c61e280: sa_motion_publish_stop(ctx,0x8c61e280u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e282; /* fmac fr0,fr3,fr2 */
L8c61e282: sa_motion_publish_stop(ctx,0x8c61e282u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e284; /* fmov.s fr2,@-r8 */
L8c61e284: sa_motion_publish_stop(ctx,0x8c61e284u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e286; /* fmov.s @(r0,r4),fr3 */
L8c61e286: sa_motion_publish_stop(ctx,0x8c61e286u); r[0]=20u;goto L8c61e288; /* mov #0x14,r0 */
L8c61e288: sa_motion_publish_stop(ctx,0x8c61e288u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e28a; /* fmov.s fr3,@-r8 */
L8c61e28a: sa_motion_publish_stop(ctx,0x8c61e28au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e28c; /* fmov.s @(r0,r4),fr3 */
L8c61e28c: sa_motion_publish_stop(ctx,0x8c61e28cu); r[0]=16u;goto L8c61e28e; /* mov #0x10,r0 */
L8c61e28e: sa_motion_publish_stop(ctx,0x8c61e28eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e290; /* fmov.s fr3,@-r8 */
L8c61e290: sa_motion_publish_stop(ctx,0x8c61e290u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e292; /* fmov.s @(r0,r4),fr3 */
L8c61e292: sa_motion_publish_stop(ctx,0x8c61e292u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e294; /* fmov.s fr3,@-r8 */
L8c61e294: sa_motion_publish_stop(ctx,0x8c61e294u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e296; /* mov.l r3,@-r8 */
L8c61e296: sa_motion_publish_stop(ctx,0x8c61e296u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e298; /* pref @r8 */
L8c61e298: sa_motion_publish_stop(ctx,0x8c61e298u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e29a; /* mov.w @r10+,r4 */
L8c61e29a: sa_motion_publish_stop(ctx,0x8c61e29au); r[8]+=56u;goto L8c61e29c; /* add #0x38,r8 */
L8c61e29c: sa_motion_publish_stop(ctx,0x8c61e29cu); r[2]=rd(m,bus,&io_status,0x8c61e4dcu,4);if(io_status)return io_status;goto L8c61e29e; /* mov.l 0x8c61e4dc,r2 */
L8c61e29e: sa_motion_publish_stop(ctx,0x8c61e29eu); r[4]=r[4]&65535u;goto L8c61e2a0; /* extu.w r4,r4 */
L8c61e2a0: sa_motion_publish_stop(ctx,0x8c61e2a0u); r[0]=rd(m,bus,&io_status,0x8c61e4e0u,4);if(io_status)return io_status;goto L8c61e2a2; /* mov.l 0x8c61e4e0,r0 */
L8c61e2a2: sa_motion_publish_stop(ctx,0x8c61e2a2u); r[4]<<=2;goto L8c61e2a4; /* shll2 r4 */
L8c61e2a4: sa_motion_publish_stop(ctx,0x8c61e2a4u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e2a6; /* mov.l @r2,r3 */
L8c61e2a6: sa_motion_publish_stop(ctx,0x8c61e2a6u); r[4]<<=2;goto L8c61e2a8; /* shll2 r4 */
L8c61e2a8: sa_motion_publish_stop(ctx,0x8c61e2a8u); r[1]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61e2aa; /* mov.l @r0,r1 */
L8c61e2aa: sa_motion_publish_stop(ctx,0x8c61e2aau); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61e2ac; /* shll r4 */
L8c61e2ac: sa_motion_publish_stop(ctx,0x8c61e2acu); r[0]=4u;goto L8c61e2ae; /* mov #0x4,r0 */
L8c61e2ae: sa_motion_publish_stop(ctx,0x8c61e2aeu); r[2]=r[1];goto L8c61e2b0; /* mov r1,r2 */
L8c61e2b0: sa_motion_publish_stop(ctx,0x8c61e2b0u); r[4]+=r[3];goto L8c61e2b2; /* add r3,r4 */
L8c61e2b2: sa_motion_publish_stop(ctx,0x8c61e2b2u); r[3]=r[1];goto L8c61e2b4; /* mov r1,r3 */
L8c61e2b4: sa_motion_publish_stop(ctx,0x8c61e2b4u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e2b6; /* fmov.s @(r0,r4),fr0 */
L8c61e2b6: sa_motion_publish_stop(ctx,0x8c61e2b6u); r[0]=20u;goto L8c61e2b8; /* mov #0x14,r0 */
L8c61e2b8: sa_motion_publish_stop(ctx,0x8c61e2b8u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e2ba; /* fmov.s @(r0,r2),fr3 */
L8c61e2ba: sa_motion_publish_stop(ctx,0x8c61e2bau); r[0]=4u;goto L8c61e2bc; /* mov #0x4,r0 */
L8c61e2bc: sa_motion_publish_stop(ctx,0x8c61e2bcu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e2be; /* fmov.s @(r0,r3),fr2 */
L8c61e2be: sa_motion_publish_stop(ctx,0x8c61e2beu); r[0]=8u;goto L8c61e2c0; /* mov #0x8,r0 */
L8c61e2c0: sa_motion_publish_stop(ctx,0x8c61e2c0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e2c2; /* fmul fr0,fr3 */
L8c61e2c2: sa_motion_publish_stop(ctx,0x8c61e2c2u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e2c4; /* fmov.s @r4,fr0 */
L8c61e2c4: sa_motion_publish_stop(ctx,0x8c61e2c4u); r[3]=rd(m,bus,&io_status,0x8c61e4e0u,4);if(io_status)return io_status;goto L8c61e2c6; /* mov.l 0x8c61e4e0,r3 */
L8c61e2c6: sa_motion_publish_stop(ctx,0x8c61e2c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e2c8; /* fmac fr0,fr2,fr3 */
L8c61e2c8: sa_motion_publish_stop(ctx,0x8c61e2c8u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e2ca; /* fmov.s @(r0,r4),fr0 */
L8c61e2ca: sa_motion_publish_stop(ctx,0x8c61e2cau); r[0]=36u;goto L8c61e2cc; /* mov #0x24,r0 */
L8c61e2cc: sa_motion_publish_stop(ctx,0x8c61e2ccu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e2ce; /* fmov.s @(r0,r1),fr2 */
L8c61e2ce: sa_motion_publish_stop(ctx,0x8c61e2ceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e2d0; /* fmac fr0,fr2,fr3 */
L8c61e2d0: sa_motion_publish_stop(ctx,0x8c61e2d0u); fcopy(f,0u,15u);goto L8c61e2d2; /* fmov fr15,fr0 */
L8c61e2d2: sa_motion_publish_stop(ctx,0x8c61e2d2u); fcopy(f,2u,15u);goto L8c61e2d4; /* fmov fr15,fr2 */
L8c61e2d4: sa_motion_publish_stop(ctx,0x8c61e2d4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e2d6; /* fmac fr0,fr3,fr2 */
L8c61e2d6: sa_motion_publish_stop(ctx,0x8c61e2d6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e2d8; /* fmov.s fr2,@-r8 */
L8c61e2d8: sa_motion_publish_stop(ctx,0x8c61e2d8u); r[0]=4u;goto L8c61e2da; /* mov #0x4,r0 */
L8c61e2da: sa_motion_publish_stop(ctx,0x8c61e2dau); r[9]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e2dc; /* mov.l @r3,r9 */
L8c61e2dc: sa_motion_publish_stop(ctx,0x8c61e2dcu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e2de; /* fmov.s @(r0,r4),fr0 */
L8c61e2de: sa_motion_publish_stop(ctx,0x8c61e2deu); r[0]=16u;goto L8c61e2e0; /* mov #0x10,r0 */
L8c61e2e0: sa_motion_publish_stop(ctx,0x8c61e2e0u); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61e2e2; /* fmov.s @(r0,r9),fr3 */
L8c61e2e2: sa_motion_publish_stop(ctx,0x8c61e2e2u); r[0]=8u;goto L8c61e2e4; /* mov #0x8,r0 */
L8c61e2e4: sa_motion_publish_stop(ctx,0x8c61e2e4u); fload(m,bus,&io_status,f,2u,r[9]);if(io_status)return io_status;goto L8c61e2e6; /* fmov.s @r9,fr2 */
L8c61e2e6: sa_motion_publish_stop(ctx,0x8c61e2e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e2e8; /* fmul fr0,fr3 */
L8c61e2e8: sa_motion_publish_stop(ctx,0x8c61e2e8u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e2ea; /* fmov.s @r4,fr0 */
L8c61e2ea: sa_motion_publish_stop(ctx,0x8c61e2eau); r[2]=rd(m,bus,&io_status,0x8c61e4e4u,4);if(io_status)return io_status;goto L8c61e2ec; /* mov.l 0x8c61e4e4,r2 */
L8c61e2ec: sa_motion_publish_stop(ctx,0x8c61e2ecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e2ee; /* fmac fr0,fr2,fr3 */
L8c61e2ee: sa_motion_publish_stop(ctx,0x8c61e2eeu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e2f0; /* fmov.s @(r0,r4),fr0 */
L8c61e2f0: sa_motion_publish_stop(ctx,0x8c61e2f0u); r[0]=32u;goto L8c61e2f2; /* mov #0x20,r0 */
L8c61e2f2: sa_motion_publish_stop(ctx,0x8c61e2f2u); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61e2f4; /* fmov.s @(r0,r9),fr2 */
L8c61e2f4: sa_motion_publish_stop(ctx,0x8c61e2f4u); r[0]=24u;goto L8c61e2f6; /* mov #0x18,r0 */
L8c61e2f6: sa_motion_publish_stop(ctx,0x8c61e2f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e2f8; /* fmac fr0,fr2,fr3 */
L8c61e2f8: sa_motion_publish_stop(ctx,0x8c61e2f8u); fcopy(f,0u,15u);goto L8c61e2fa; /* fmov fr15,fr0 */
L8c61e2fa: sa_motion_publish_stop(ctx,0x8c61e2fau); fcopy(f,2u,15u);goto L8c61e2fc; /* fmov fr15,fr2 */
L8c61e2fc: sa_motion_publish_stop(ctx,0x8c61e2fcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e2fe; /* fmac fr0,fr3,fr2 */
L8c61e2fe: sa_motion_publish_stop(ctx,0x8c61e2feu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e300; /* fmov.s fr2,@-r8 */
L8c61e300: sa_motion_publish_stop(ctx,0x8c61e300u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e302; /* fmov.s @(r0,r4),fr3 */
L8c61e302: sa_motion_publish_stop(ctx,0x8c61e302u); r[0]=20u;goto L8c61e304; /* mov #0x14,r0 */
L8c61e304: sa_motion_publish_stop(ctx,0x8c61e304u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e306; /* fmov.s fr3,@-r8 */
L8c61e306: sa_motion_publish_stop(ctx,0x8c61e306u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e308; /* fmov.s @(r0,r4),fr3 */
L8c61e308: sa_motion_publish_stop(ctx,0x8c61e308u); r[0]=16u;goto L8c61e30a; /* mov #0x10,r0 */
L8c61e30a: sa_motion_publish_stop(ctx,0x8c61e30au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e30c; /* fmov.s fr3,@-r8 */
L8c61e30c: sa_motion_publish_stop(ctx,0x8c61e30cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e30e; /* fmov.s @(r0,r4),fr3 */
L8c61e30e: sa_motion_publish_stop(ctx,0x8c61e30eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e310; /* fmov.s fr3,@-r8 */
L8c61e310: sa_motion_publish_stop(ctx,0x8c61e310u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61e312; /* mov.l r2,@-r8 */
L8c61e312: sa_motion_publish_stop(ctx,0x8c61e312u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e314; /* pref @r8 */
L8c61e314: sa_motion_publish_stop(ctx,0x8c61e314u); r[6]=r[14];goto L8c61e316; /* mov r14,r6 */
L8c61e316: sa_motion_publish_stop(ctx,0x8c61e316u); r[6]+=4294967295u;goto L8c61e318; /* add #-0x1,r6 */
L8c61e318: sa_motion_publish_stop(ctx,0x8c61e318u); r[5]=r[11];goto L8c61e31a; /* mov r11,r5 */
L8c61e31a: sa_motion_publish_stop(ctx,0x8c61e31au); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61e31c; /* cmp/ge r6,r5 */
L8c61e31c: sa_motion_publish_stop(ctx,0x8c61e31eu); take=(*sr&1u);r[8]+=32u;if(take)goto L8c61e3a0;goto L8c61e320; /* bt/s 0x8c61e3a0 */
L8c61e320: sa_motion_publish_stop(ctx,0x8c61e320u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e322; /* mov.w @r10+,r4 */
L8c61e322: sa_motion_publish_stop(ctx,0x8c61e322u); r[0]=4u;goto L8c61e324; /* mov #0x4,r0 */
L8c61e324: sa_motion_publish_stop(ctx,0x8c61e324u); r[2]=rd(m,bus,&io_status,0x8c61e4dcu,4);if(io_status)return io_status;goto L8c61e326; /* mov.l 0x8c61e4dc,r2 */
L8c61e326: sa_motion_publish_stop(ctx,0x8c61e326u); r[8]+=28u;goto L8c61e328; /* add #0x1c,r8 */
L8c61e328: sa_motion_publish_stop(ctx,0x8c61e328u); r[4]=r[4]&65535u;goto L8c61e32a; /* extu.w r4,r4 */
L8c61e32a: sa_motion_publish_stop(ctx,0x8c61e32au); r[4]<<=2;goto L8c61e32c; /* shll2 r4 */
L8c61e32c: sa_motion_publish_stop(ctx,0x8c61e32cu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e32e; /* mov.l @r2,r3 */
L8c61e32e: sa_motion_publish_stop(ctx,0x8c61e32eu); r[4]<<=2;goto L8c61e330; /* shll2 r4 */
L8c61e330: sa_motion_publish_stop(ctx,0x8c61e330u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61e332; /* fmov.s fr4,@-r8 */
L8c61e332: sa_motion_publish_stop(ctx,0x8c61e332u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61e334; /* shll r4 */
L8c61e334: sa_motion_publish_stop(ctx,0x8c61e334u); r[4]+=r[3];goto L8c61e336; /* add r3,r4 */
L8c61e336: sa_motion_publish_stop(ctx,0x8c61e336u); r[3]=rd(m,bus,&io_status,0x8c61e4e0u,4);if(io_status)return io_status;goto L8c61e338; /* mov.l 0x8c61e4e0,r3 */
L8c61e338: sa_motion_publish_stop(ctx,0x8c61e338u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e33a; /* fmov.s @(r0,r4),fr0 */
L8c61e33a: sa_motion_publish_stop(ctx,0x8c61e33au); r[0]=20u;goto L8c61e33c; /* mov #0x14,r0 */
L8c61e33c: sa_motion_publish_stop(ctx,0x8c61e33cu); r[14]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e33e; /* mov.l @r3,r14 */
L8c61e33e: sa_motion_publish_stop(ctx,0x8c61e33eu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e340; /* fmov.s @(r0,r14),fr3 */
L8c61e340: sa_motion_publish_stop(ctx,0x8c61e340u); r[0]=4u;goto L8c61e342; /* mov #0x4,r0 */
L8c61e342: sa_motion_publish_stop(ctx,0x8c61e342u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e344; /* fmov.s @(r0,r14),fr2 */
L8c61e344: sa_motion_publish_stop(ctx,0x8c61e344u); r[0]=8u;goto L8c61e346; /* mov #0x8,r0 */
L8c61e346: sa_motion_publish_stop(ctx,0x8c61e346u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e348; /* fmul fr0,fr3 */
L8c61e348: sa_motion_publish_stop(ctx,0x8c61e348u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e34a; /* fmov.s @r4,fr0 */
L8c61e34a: sa_motion_publish_stop(ctx,0x8c61e34au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e34c; /* fmac fr0,fr2,fr3 */
L8c61e34c: sa_motion_publish_stop(ctx,0x8c61e34cu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e34e; /* fmov.s @(r0,r4),fr0 */
L8c61e34e: sa_motion_publish_stop(ctx,0x8c61e34eu); r[0]=36u;goto L8c61e350; /* mov #0x24,r0 */
L8c61e350: sa_motion_publish_stop(ctx,0x8c61e350u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e352; /* fmov.s @(r0,r14),fr2 */
L8c61e352: sa_motion_publish_stop(ctx,0x8c61e352u); r[0]=4u;goto L8c61e354; /* mov #0x4,r0 */
L8c61e354: sa_motion_publish_stop(ctx,0x8c61e354u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e356; /* fmac fr0,fr2,fr3 */
L8c61e356: sa_motion_publish_stop(ctx,0x8c61e356u); fcopy(f,2u,15u);goto L8c61e358; /* fmov fr15,fr2 */
L8c61e358: sa_motion_publish_stop(ctx,0x8c61e358u); fcopy(f,0u,15u);goto L8c61e35a; /* fmov fr15,fr0 */
L8c61e35a: sa_motion_publish_stop(ctx,0x8c61e35au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e35c; /* fmac fr0,fr3,fr2 */
L8c61e35c: sa_motion_publish_stop(ctx,0x8c61e35cu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e35e; /* fmov.s fr2,@-r8 */
L8c61e35e: sa_motion_publish_stop(ctx,0x8c61e35eu); r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e360; /* mov.l @r3,r7 */
L8c61e360: sa_motion_publish_stop(ctx,0x8c61e360u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e362; /* fmov.s @(r0,r4),fr0 */
L8c61e362: sa_motion_publish_stop(ctx,0x8c61e362u); r[0]=16u;goto L8c61e364; /* mov #0x10,r0 */
L8c61e364: sa_motion_publish_stop(ctx,0x8c61e364u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61e366; /* fmov.s @(r0,r7),fr3 */
L8c61e366: sa_motion_publish_stop(ctx,0x8c61e366u); r[0]=8u;goto L8c61e368; /* mov #0x8,r0 */
L8c61e368: sa_motion_publish_stop(ctx,0x8c61e368u); fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61e36a; /* fmov.s @r7,fr2 */
L8c61e36a: sa_motion_publish_stop(ctx,0x8c61e36au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e36c; /* fmul fr0,fr3 */
L8c61e36c: sa_motion_publish_stop(ctx,0x8c61e36cu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e36e; /* fmov.s @r4,fr0 */
L8c61e36e: sa_motion_publish_stop(ctx,0x8c61e36eu); r[1]=rd(m,bus,&io_status,0x8c61e4e4u,4);if(io_status)return io_status;goto L8c61e370; /* mov.l 0x8c61e4e4,r1 */
L8c61e370: sa_motion_publish_stop(ctx,0x8c61e370u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e372; /* fmac fr0,fr2,fr3 */
L8c61e372: sa_motion_publish_stop(ctx,0x8c61e372u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e374; /* fmov.s @(r0,r4),fr0 */
L8c61e374: sa_motion_publish_stop(ctx,0x8c61e374u); r[0]=32u;goto L8c61e376; /* mov #0x20,r0 */
L8c61e376: sa_motion_publish_stop(ctx,0x8c61e376u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61e378; /* fmov.s @(r0,r7),fr2 */
L8c61e378: sa_motion_publish_stop(ctx,0x8c61e378u); r[0]=24u;goto L8c61e37a; /* mov #0x18,r0 */
L8c61e37a: sa_motion_publish_stop(ctx,0x8c61e37au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e37c; /* fmac fr0,fr2,fr3 */
L8c61e37c: sa_motion_publish_stop(ctx,0x8c61e37cu); fcopy(f,0u,15u);goto L8c61e37e; /* fmov fr15,fr0 */
L8c61e37e: sa_motion_publish_stop(ctx,0x8c61e37eu); fcopy(f,2u,15u);goto L8c61e380; /* fmov fr15,fr2 */
L8c61e380: sa_motion_publish_stop(ctx,0x8c61e380u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e382; /* fmac fr0,fr3,fr2 */
L8c61e382: sa_motion_publish_stop(ctx,0x8c61e382u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e384; /* fmov.s fr2,@-r8 */
L8c61e384: sa_motion_publish_stop(ctx,0x8c61e384u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e386; /* fmov.s @(r0,r4),fr3 */
L8c61e386: sa_motion_publish_stop(ctx,0x8c61e386u); r[0]=20u;goto L8c61e388; /* mov #0x14,r0 */
L8c61e388: sa_motion_publish_stop(ctx,0x8c61e388u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e38a; /* fmov.s fr3,@-r8 */
L8c61e38a: sa_motion_publish_stop(ctx,0x8c61e38au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e38c; /* fmov.s @(r0,r4),fr3 */
L8c61e38c: sa_motion_publish_stop(ctx,0x8c61e38cu); r[0]=16u;goto L8c61e38e; /* mov #0x10,r0 */
L8c61e38e: sa_motion_publish_stop(ctx,0x8c61e38eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e390; /* fmov.s fr3,@-r8 */
L8c61e390: sa_motion_publish_stop(ctx,0x8c61e390u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e392; /* fmov.s @(r0,r4),fr3 */
L8c61e392: sa_motion_publish_stop(ctx,0x8c61e392u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e394; /* fmov.s fr3,@-r8 */
L8c61e394: sa_motion_publish_stop(ctx,0x8c61e394u); r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c61e396; /* mov.l r1,@-r8 */
L8c61e396: sa_motion_publish_stop(ctx,0x8c61e396u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e398; /* pref @r8 */
L8c61e398: sa_motion_publish_stop(ctx,0x8c61e398u); r[5]+=1u;goto L8c61e39a; /* add #0x1,r5 */
L8c61e39a: sa_motion_publish_stop(ctx,0x8c61e39au); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61e39c; /* cmp/ge r6,r5 */
L8c61e39c: sa_motion_publish_stop(ctx,0x8c61e39eu); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61e320;goto L8c61e3a0; /* bf/s 0x8c61e320 */
L8c61e3a0: sa_motion_publish_stop(ctx,0x8c61e3a0u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e3a2; /* mov.w @r10+,r4 */
L8c61e3a2: sa_motion_publish_stop(ctx,0x8c61e3a2u); r[0]=4u;goto L8c61e3a4; /* mov #0x4,r0 */
L8c61e3a4: sa_motion_publish_stop(ctx,0x8c61e3a4u); r[2]=rd(m,bus,&io_status,0x8c61e4dcu,4);if(io_status)return io_status;goto L8c61e3a6; /* mov.l 0x8c61e4dc,r2 */
L8c61e3a6: sa_motion_publish_stop(ctx,0x8c61e3a6u); r[8]+=28u;goto L8c61e3a8; /* add #0x1c,r8 */
L8c61e3a8: sa_motion_publish_stop(ctx,0x8c61e3a8u); r[4]=r[4]&65535u;goto L8c61e3aa; /* extu.w r4,r4 */
L8c61e3aa: sa_motion_publish_stop(ctx,0x8c61e3aau); r[4]<<=2;goto L8c61e3ac; /* shll2 r4 */
L8c61e3ac: sa_motion_publish_stop(ctx,0x8c61e3acu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e3ae; /* mov.l @r2,r3 */
L8c61e3ae: sa_motion_publish_stop(ctx,0x8c61e3aeu); r[4]<<=2;goto L8c61e3b0; /* shll2 r4 */
L8c61e3b0: sa_motion_publish_stop(ctx,0x8c61e3b0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61e3b2; /* fmov.s fr4,@-r8 */
L8c61e3b2: sa_motion_publish_stop(ctx,0x8c61e3b2u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61e3b4; /* shll r4 */
L8c61e3b4: sa_motion_publish_stop(ctx,0x8c61e3b4u); r[4]+=r[3];goto L8c61e3b6; /* add r3,r4 */
L8c61e3b6: sa_motion_publish_stop(ctx,0x8c61e3b6u); r[3]=rd(m,bus,&io_status,0x8c61e4e0u,4);if(io_status)return io_status;goto L8c61e3b8; /* mov.l 0x8c61e4e0,r3 */
L8c61e3b8: sa_motion_publish_stop(ctx,0x8c61e3b8u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e3ba; /* fmov.s @(r0,r4),fr0 */
L8c61e3ba: sa_motion_publish_stop(ctx,0x8c61e3bau); r[0]=20u;goto L8c61e3bc; /* mov #0x14,r0 */
L8c61e3bc: sa_motion_publish_stop(ctx,0x8c61e3bcu); r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e3be; /* mov.l @r3,r6 */
L8c61e3be: sa_motion_publish_stop(ctx,0x8c61e3beu); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e3c0; /* fmov.s @(r0,r6),fr3 */
L8c61e3c0: sa_motion_publish_stop(ctx,0x8c61e3c0u); r[0]=4u;goto L8c61e3c2; /* mov #0x4,r0 */
L8c61e3c2: sa_motion_publish_stop(ctx,0x8c61e3c2u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e3c4; /* fmov.s @(r0,r6),fr2 */
L8c61e3c4: sa_motion_publish_stop(ctx,0x8c61e3c4u); r[0]=8u;goto L8c61e3c6; /* mov #0x8,r0 */
L8c61e3c6: sa_motion_publish_stop(ctx,0x8c61e3c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e3c8; /* fmul fr0,fr3 */
L8c61e3c8: sa_motion_publish_stop(ctx,0x8c61e3c8u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e3ca; /* fmov.s @r4,fr0 */
L8c61e3ca: sa_motion_publish_stop(ctx,0x8c61e3cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e3cc; /* fmac fr0,fr2,fr3 */
L8c61e3cc: sa_motion_publish_stop(ctx,0x8c61e3ccu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e3ce; /* fmov.s @(r0,r4),fr0 */
L8c61e3ce: sa_motion_publish_stop(ctx,0x8c61e3ceu); r[0]=36u;goto L8c61e3d0; /* mov #0x24,r0 */
L8c61e3d0: sa_motion_publish_stop(ctx,0x8c61e3d0u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61e3d2; /* fmov.s @(r0,r6),fr2 */
L8c61e3d2: sa_motion_publish_stop(ctx,0x8c61e3d2u); r[0]=4u;goto L8c61e3d4; /* mov #0x4,r0 */
L8c61e3d4: sa_motion_publish_stop(ctx,0x8c61e3d4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e3d6; /* fmac fr0,fr2,fr3 */
L8c61e3d6: sa_motion_publish_stop(ctx,0x8c61e3d6u); fcopy(f,2u,15u);goto L8c61e3d8; /* fmov fr15,fr2 */
L8c61e3d8: sa_motion_publish_stop(ctx,0x8c61e3d8u); fcopy(f,0u,15u);goto L8c61e3da; /* fmov fr15,fr0 */
L8c61e3da: sa_motion_publish_stop(ctx,0x8c61e3dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e3dc; /* fmac fr0,fr3,fr2 */
L8c61e3dc: sa_motion_publish_stop(ctx,0x8c61e3dcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e3de; /* fmov.s fr2,@-r8 */
L8c61e3de: sa_motion_publish_stop(ctx,0x8c61e3deu); r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e3e0; /* mov.l @r3,r5 */
L8c61e3e0: sa_motion_publish_stop(ctx,0x8c61e3e0u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e3e2; /* fmov.s @(r0,r4),fr0 */
L8c61e3e2: sa_motion_publish_stop(ctx,0x8c61e3e2u); r[0]=16u;goto L8c61e3e4; /* mov #0x10,r0 */
L8c61e3e4: sa_motion_publish_stop(ctx,0x8c61e3e4u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61e3e6; /* fmov.s @(r0,r5),fr3 */
L8c61e3e6: sa_motion_publish_stop(ctx,0x8c61e3e6u); r[0]=8u;goto L8c61e3e8; /* mov #0x8,r0 */
L8c61e3e8: sa_motion_publish_stop(ctx,0x8c61e3e8u); fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61e3ea; /* fmov.s @r5,fr2 */
L8c61e3ea: sa_motion_publish_stop(ctx,0x8c61e3eau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e3ec; /* fmul fr0,fr3 */
L8c61e3ec: sa_motion_publish_stop(ctx,0x8c61e3ecu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61e3ee; /* fmov.s @r4,fr0 */
L8c61e3ee: sa_motion_publish_stop(ctx,0x8c61e3eeu); r[1]=rd(m,bus,&io_status,0x8c61e4e8u,4);if(io_status)return io_status;goto L8c61e3f0; /* mov.l 0x8c61e4e8,r1 */
L8c61e3f0: sa_motion_publish_stop(ctx,0x8c61e3f0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e3f2; /* fmac fr0,fr2,fr3 */
L8c61e3f2: sa_motion_publish_stop(ctx,0x8c61e3f2u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e3f4; /* fmov.s @(r0,r4),fr0 */
L8c61e3f4: sa_motion_publish_stop(ctx,0x8c61e3f4u); r[0]=32u;goto L8c61e3f6; /* mov #0x20,r0 */
L8c61e3f6: sa_motion_publish_stop(ctx,0x8c61e3f6u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61e3f8; /* fmov.s @(r0,r5),fr2 */
L8c61e3f8: sa_motion_publish_stop(ctx,0x8c61e3f8u); r[0]=24u;goto L8c61e3fa; /* mov #0x18,r0 */
L8c61e3fa: sa_motion_publish_stop(ctx,0x8c61e3fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e3fc; /* fmac fr0,fr2,fr3 */
L8c61e3fc: sa_motion_publish_stop(ctx,0x8c61e3fcu); fcopy(f,0u,15u);goto L8c61e3fe; /* fmov fr15,fr0 */
L8c61e3fe: sa_motion_publish_stop(ctx,0x8c61e3feu); fcopy(f,2u,15u);goto L8c61e400; /* fmov fr15,fr2 */
L8c61e400: sa_motion_publish_stop(ctx,0x8c61e400u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e402; /* fmac fr0,fr3,fr2 */
L8c61e402: sa_motion_publish_stop(ctx,0x8c61e402u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e404; /* fmov.s fr2,@-r8 */
L8c61e404: sa_motion_publish_stop(ctx,0x8c61e404u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e406; /* fmov.s @(r0,r4),fr3 */
L8c61e406: sa_motion_publish_stop(ctx,0x8c61e406u); r[0]=20u;goto L8c61e408; /* mov #0x14,r0 */
L8c61e408: sa_motion_publish_stop(ctx,0x8c61e408u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e40a; /* fmov.s fr3,@-r8 */
L8c61e40a: sa_motion_publish_stop(ctx,0x8c61e40au); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e40c; /* fmov.s @(r0,r4),fr3 */
L8c61e40c: sa_motion_publish_stop(ctx,0x8c61e40cu); r[0]=16u;goto L8c61e40e; /* mov #0x10,r0 */
L8c61e40e: sa_motion_publish_stop(ctx,0x8c61e40eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e410; /* fmov.s fr3,@-r8 */
L8c61e410: sa_motion_publish_stop(ctx,0x8c61e410u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61e412; /* fmov.s @(r0,r4),fr3 */
L8c61e412: sa_motion_publish_stop(ctx,0x8c61e412u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e414; /* fmov.s fr3,@-r8 */
L8c61e414: sa_motion_publish_stop(ctx,0x8c61e414u); r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c61e416; /* mov.l r1,@-r8 */
L8c61e416: sa_motion_publish_stop(ctx,0x8c61e416u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e418; /* pref @r8 */
L8c61e418: sa_motion_publish_stop(ctx,0x8c61e418u); r[8]+=32u;goto L8c61e41a; /* add #0x20,r8 */
L8c61e41a: sa_motion_publish_stop(ctx,0x8c61e41au); r[12]+=1u;goto L8c61e41c; /* add #0x1,r12 */
L8c61e41c: sa_motion_publish_stop(ctx,0x8c61e41cu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61e41e; /* mov.l @r15,r3 */
L8c61e41e: sa_motion_publish_stop(ctx,0x8c61e41eu); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61e420; /* mov.w @(0x2,r3),r0 */
L8c61e420: sa_motion_publish_stop(ctx,0x8c61e420u); r[0]=r[0]&65535u;goto L8c61e422; /* extu.w r0,r0 */
L8c61e422: sa_motion_publish_stop(ctx,0x8c61e422u); *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[0]);goto L8c61e424; /* cmp/ge r0,r12 */
L8c61e424: sa_motion_publish_stop(ctx,0x8c61e424u); if((*sr&1u))goto L8c61e42a;goto L8c61e426; /* bt 0x8c61e42a */
L8c61e426: sa_motion_publish_stop(ctx,0x8c61e428u); goto L8c61e128; /* bra 0x8c61e128 */
L8c61e42a: sa_motion_publish_stop(ctx,0x8c61e42cu); goto L8c61e4a8; /* bra 0x8c61e4a8 */
L8c61e42e: sa_motion_publish_stop(ctx,0x8c61e42eu); r[2]=rd(m,bus,&io_status,0x8c61e4ecu,4);if(io_status)return io_status;goto L8c61e430; /* mov.l 0x8c61e4ec,r2 */
L8c61e430: sa_motion_publish_stop(ctx,0x8c61e430u); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61e432; /* mov.l @r15,r1 */
L8c61e432: sa_motion_publish_stop(ctx,0x8c61e432u); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61e434; /* mov.l r1,@r2 */
L8c61e434: sa_motion_publish_stop(ctx,0x8c61e434u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61e436; /* frchg */
L8c61e436: sa_motion_publish_stop(ctx,0x8c61e436u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61e438; /* fmov.s fr0,@-r15 */
L8c61e438: sa_motion_publish_stop(ctx,0x8c61e438u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61e43a; /* fmov.s fr1,@-r15 */
L8c61e43a: sa_motion_publish_stop(ctx,0x8c61e43au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61e43c; /* fmov.s fr2,@-r15 */
L8c61e43c: sa_motion_publish_stop(ctx,0x8c61e43cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61e43e; /* fmov.s fr3,@-r15 */
L8c61e43e: sa_motion_publish_stop(ctx,0x8c61e43eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61e440; /* fmov.s fr4,@-r15 */
L8c61e440: sa_motion_publish_stop(ctx,0x8c61e440u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61e442; /* fmov.s fr5,@-r15 */
L8c61e442: sa_motion_publish_stop(ctx,0x8c61e442u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61e444; /* fmov.s fr6,@-r15 */
L8c61e444: sa_motion_publish_stop(ctx,0x8c61e444u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61e446; /* fmov.s fr7,@-r15 */
L8c61e446: sa_motion_publish_stop(ctx,0x8c61e446u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61e448; /* fmov.s fr8,@-r15 */
L8c61e448: sa_motion_publish_stop(ctx,0x8c61e448u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61e44a; /* fmov.s fr9,@-r15 */
L8c61e44a: sa_motion_publish_stop(ctx,0x8c61e44au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c61e44c; /* fmov.s fr10,@-r15 */
L8c61e44c: sa_motion_publish_stop(ctx,0x8c61e44cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c61e44e; /* fmov.s fr11,@-r15 */
L8c61e44e: sa_motion_publish_stop(ctx,0x8c61e44eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61e450; /* fmov.s fr12,@-r15 */
L8c61e450: sa_motion_publish_stop(ctx,0x8c61e450u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61e452; /* fmov.s fr13,@-r15 */
L8c61e452: sa_motion_publish_stop(ctx,0x8c61e452u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61e454; /* fmov.s fr14,@-r15 */
L8c61e454: sa_motion_publish_stop(ctx,0x8c61e454u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61e456; /* fmov.s fr15,@-r15 */
L8c61e456: sa_motion_publish_stop(ctx,0x8c61e456u); r[0]=0x8c61e4bcu;goto L8c61e458; /* mova 0x8c61e4bc,r0 */
L8c61e458: sa_motion_publish_stop(ctx,0x8c61e458u); fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c61e45a; /* fmov.s @r0,fr12 */
L8c61e45a: sa_motion_publish_stop(ctx,0x8c61e45au); fcopy(f,13u,12u);goto L8c61e45c; /* fmov fr12,fr13 */
L8c61e45c: sa_motion_publish_stop(ctx,0x8c61e45cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61e45e; /* fmul fr12,fr0 */
L8c61e45e: sa_motion_publish_stop(ctx,0x8c61e45eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61e460; /* fmul fr12,fr4 */
L8c61e460: sa_motion_publish_stop(ctx,0x8c61e460u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[12],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61e462; /* fmul fr12,fr8 */
L8c61e462: sa_motion_publish_stop(ctx,0x8c61e462u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[12],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e464; /* fmul fr12,fr1 */
L8c61e464: sa_motion_publish_stop(ctx,0x8c61e464u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[12],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61e466; /* fmul fr12,fr5 */
L8c61e466: sa_motion_publish_stop(ctx,0x8c61e466u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[12],&f->fpscr,&fr[9]);if(status)return status;}goto L8c61e468; /* fmul fr12,fr9 */
L8c61e468: sa_motion_publish_stop(ctx,0x8c61e468u); r[0]=rd(m,bus,&io_status,0x8c61e4c0u,4);if(io_status)return io_status;goto L8c61e46a; /* mov.l 0x8c61e4c0,r0 */
L8c61e46a: sa_motion_publish_stop(ctx,0x8c61e46au); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e46c; /* fmov.s @r0+,fr2 */
L8c61e46c: sa_motion_publish_stop(ctx,0x8c61e46cu); fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e46e; /* fmov.s @r0+,fr6 */
L8c61e46e: sa_motion_publish_stop(ctx,0x8c61e46eu); fload(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;goto L8c61e470; /* fmov.s @r0,fr10 */
L8c61e470: sa_motion_publish_stop(ctx,0x8c61e470u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c61e472; /* fneg fr2 */
L8c61e472: sa_motion_publish_stop(ctx,0x8c61e472u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]^=0x80000000u;goto L8c61e474; /* fneg fr6 */
L8c61e474: sa_motion_publish_stop(ctx,0x8c61e474u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]^=0x80000000u;goto L8c61e476; /* fneg fr10 */
L8c61e476: sa_motion_publish_stop(ctx,0x8c61e476u); r[0]=rd(m,bus,&io_status,0x8c61e4c4u,4);if(io_status)return io_status;goto L8c61e478; /* mov.l 0x8c61e4c4,r0 */
L8c61e478: sa_motion_publish_stop(ctx,0x8c61e478u); fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c61e47a; /* fmov.s @r0,fr14 */
L8c61e47a: sa_motion_publish_stop(ctx,0x8c61e47au); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61e47c; /* frchg */
L8c61e47c: sa_motion_publish_stop(ctx,0x8c61e47cu); r[2]=rd(m,bus,&io_status,0x8c61e4f0u,4);if(io_status)return io_status;goto L8c61e47e; /* mov.l 0x8c61e4f0,r2 */
L8c61e47e: sa_motion_publish_stop(ctx,0x8c61e47eu); r[3]=rd(m,bus,&io_status,0x8c61e4ecu,4);if(io_status)return io_status;goto L8c61e480; /* mov.l 0x8c61e4ec,r3 */
L8c61e480: sa_motion_publish_stop(ctx,0x8c61e482u); target=r[2];*pr=0x8c61e484u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61e484; /* jsr @r2 */
L8c61e484: sa_motion_publish_stop(ctx,0x8c61e484u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61e486; /* frchg */
L8c61e486: sa_motion_publish_stop(ctx,0x8c61e486u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e488; /* fmov.s @r15+,fr15 */
L8c61e488: sa_motion_publish_stop(ctx,0x8c61e488u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e48a; /* fmov.s @r15+,fr14 */
L8c61e48a: sa_motion_publish_stop(ctx,0x8c61e48au); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e48c; /* fmov.s @r15+,fr13 */
L8c61e48c: sa_motion_publish_stop(ctx,0x8c61e48cu); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e48e; /* fmov.s @r15+,fr12 */
L8c61e48e: sa_motion_publish_stop(ctx,0x8c61e48eu); fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e490; /* fmov.s @r15+,fr11 */
L8c61e490: sa_motion_publish_stop(ctx,0x8c61e490u); fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e492; /* fmov.s @r15+,fr10 */
L8c61e492: sa_motion_publish_stop(ctx,0x8c61e492u); fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e494; /* fmov.s @r15+,fr9 */
L8c61e494: sa_motion_publish_stop(ctx,0x8c61e494u); fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e496; /* fmov.s @r15+,fr8 */
L8c61e496: sa_motion_publish_stop(ctx,0x8c61e496u); fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e498; /* fmov.s @r15+,fr7 */
L8c61e498: sa_motion_publish_stop(ctx,0x8c61e498u); fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e49a; /* fmov.s @r15+,fr6 */
L8c61e49a: sa_motion_publish_stop(ctx,0x8c61e49au); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e49c; /* fmov.s @r15+,fr5 */
L8c61e49c: sa_motion_publish_stop(ctx,0x8c61e49cu); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e49e; /* fmov.s @r15+,fr4 */
L8c61e49e: sa_motion_publish_stop(ctx,0x8c61e49eu); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e4a0; /* fmov.s @r15+,fr3 */
L8c61e4a0: sa_motion_publish_stop(ctx,0x8c61e4a0u); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e4a2; /* fmov.s @r15+,fr2 */
L8c61e4a2: sa_motion_publish_stop(ctx,0x8c61e4a2u); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e4a4; /* fmov.s @r15+,fr1 */
L8c61e4a4: sa_motion_publish_stop(ctx,0x8c61e4a4u); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e4a6; /* fmov.s @r15+,fr0 */
L8c61e4a6: sa_motion_publish_stop(ctx,0x8c61e4a6u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61e4a8; /* frchg */
L8c61e4a8: sa_motion_publish_stop(ctx,0x8c61e4a8u); r[15]+=16u;goto L8c61e4aa; /* add #0x10,r15 */
L8c61e4aa: sa_motion_publish_stop(ctx,0x8c61e4aau); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61e4ac; /* lds.l @r15+,PR */
L8c61e4ac: sa_motion_publish_stop(ctx,0x8c61e4acu); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61e4ae; /* fmov.s @r15+,fr15 */
L8c61e4ae: sa_motion_publish_stop(ctx,0x8c61e4aeu); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61e4b0; /* mov.l @r15+,r9 */
L8c61e4b0: sa_motion_publish_stop(ctx,0x8c61e4b0u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61e4b2; /* mov.l @r15+,r10 */
L8c61e4b2: sa_motion_publish_stop(ctx,0x8c61e4b2u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61e4b4; /* mov.l @r15+,r11 */
L8c61e4b4: sa_motion_publish_stop(ctx,0x8c61e4b4u); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61e4b6; /* mov.l @r15+,r12 */
L8c61e4b6: sa_motion_publish_stop(ctx,0x8c61e4b6u); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61e4b8; /* mov.l @r15+,r13 */
L8c61e4b8: sa_motion_publish_stop(ctx,0x8c61e4bau); if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_environment_face_primitive_clipped_8c61e4f4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 const struct sa_vertex_bus *bus,void *ctx,sa_shared_service call){
 int io_status=0;
 uint32_t entry_pr=*pr;
 (void)fpul;(void)macl;(void)sx;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c61e4f4;
L8c61e4f4: sa_motion_publish_stop(ctx,0x8c61e4f4u); r[15]-=4;wr(m,bus,&io_status,r[15],r[14],4);if(io_status)return io_status;goto L8c61e4f6; /* mov.l r14,@-r15 */
L8c61e4f6: sa_motion_publish_stop(ctx,0x8c61e4f6u); r[15]-=4;wr(m,bus,&io_status,r[15],r[13],4);if(io_status)return io_status;goto L8c61e4f8; /* mov.l r13,@-r15 */
L8c61e4f8: sa_motion_publish_stop(ctx,0x8c61e4f8u); r[15]-=4;wr(m,bus,&io_status,r[15],r[12],4);if(io_status)return io_status;goto L8c61e4fa; /* mov.l r12,@-r15 */
L8c61e4fa: sa_motion_publish_stop(ctx,0x8c61e4fau); r[15]-=4;wr(m,bus,&io_status,r[15],r[11],4);if(io_status)return io_status;goto L8c61e4fc; /* mov.l r11,@-r15 */
L8c61e4fc: sa_motion_publish_stop(ctx,0x8c61e4fcu); r[15]-=4;wr(m,bus,&io_status,r[15],r[10],4);if(io_status)return io_status;goto L8c61e4fe; /* mov.l r10,@-r15 */
L8c61e4fe: sa_motion_publish_stop(ctx,0x8c61e4feu); r[15]-=4;wr(m,bus,&io_status,r[15],r[9],4);if(io_status)return io_status;goto L8c61e500; /* mov.l r9,@-r15 */
L8c61e500: sa_motion_publish_stop(ctx,0x8c61e500u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61e502; /* fmov.s fr15,@-r15 */
L8c61e502: sa_motion_publish_stop(ctx,0x8c61e502u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61e504; /* fmov.s fr14,@-r15 */
L8c61e504: sa_motion_publish_stop(ctx,0x8c61e504u); r[15]-=4;wr(m,bus,&io_status,r[15],*pr,4);if(io_status)return io_status;goto L8c61e506; /* sts.l PR,@-r15 */
L8c61e506: sa_motion_publish_stop(ctx,0x8c61e506u); r[15]+=4294967272u;goto L8c61e508; /* add #-0x18,r15 */
L8c61e508: sa_motion_publish_stop(ctx,0x8c61e508u); r[0]=0x8c61e560u;goto L8c61e50a; /* mova 0x8c61e560,r0 */
L8c61e50a: sa_motion_publish_stop(ctx,0x8c61e50au); r[3]=rd(m,bus,&io_status,0x8c61e564u,4);if(io_status)return io_status;goto L8c61e50c; /* mov.l 0x8c61e564,r3 */
L8c61e50c: sa_motion_publish_stop(ctx,0x8c61e50cu); fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c61e50e; /* fmov.s @r0,fr14 */
L8c61e50e: sa_motion_publish_stop(ctx,0x8c61e50eu); r[0]=r[4];goto L8c61e510; /* mov r4,r0 */
L8c61e510: sa_motion_publish_stop(ctx,0x8c61e510u); wr(m,bus,&io_status,r[15],r[4],4);if(io_status)return io_status;goto L8c61e512; /* mov.l r4,@r15 */
L8c61e512: sa_motion_publish_stop(ctx,0x8c61e512u); r[10]=r[4];goto L8c61e514; /* mov r4,r10 */
L8c61e514: sa_motion_publish_stop(ctx,0x8c61e514u); r[0]=sx(m,bus,&io_status,r[0],2);if(io_status)return io_status;goto L8c61e516; /* mov.w @r0,r0 */
L8c61e516: sa_motion_publish_stop(ctx,0x8c61e516u); r[0]=r[0]&65535u;goto L8c61e518; /* extu.w r0,r0 */
L8c61e518: sa_motion_publish_stop(ctx,0x8c61e518u); r[0]&=r[3];goto L8c61e51a; /* and r3,r0 */
L8c61e51a: sa_motion_publish_stop(ctx,0x8c61e51au); *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c61e51c; /* cmp/eq #0x0,r0 */
L8c61e51c: sa_motion_publish_stop(ctx,0x8c61e51eu); take=(*sr&1u);r[10]=rd(m,bus,&io_status,r[10]+0x4u,4);if(io_status)return io_status;if(take)goto L8c61e542;goto L8c61e520; /* bt/s 0x8c61e542 */
L8c61e520: sa_motion_publish_stop(ctx,0x8c61e520u); r[1]=sx(m,bus,&io_status,0x8c61e55eu,2);if(io_status)return io_status;goto L8c61e522; /* mov.w 0x8c61e55e,r1 */
L8c61e522: sa_motion_publish_stop(ctx,0x8c61e522u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61e524; /* cmp/eq r1,r0 */
L8c61e524: sa_motion_publish_stop(ctx,0x8c61e524u); if(!(*sr&1u))goto L8c61e52a;goto L8c61e526; /* bf 0x8c61e52a */
L8c61e526: sa_motion_publish_stop(ctx,0x8c61e528u); goto L8c61e82c; /* bra 0x8c61e82c */
L8c61e52a: sa_motion_publish_stop(ctx,0x8c61e52au); r[1]=rd(m,bus,&io_status,0x8c61e568u,4);if(io_status)return io_status;goto L8c61e52c; /* mov.l 0x8c61e568,r1 */
L8c61e52c: sa_motion_publish_stop(ctx,0x8c61e52cu); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61e52e; /* cmp/eq r1,r0 */
L8c61e52e: sa_motion_publish_stop(ctx,0x8c61e52eu); if(!(*sr&1u))goto L8c61e534;goto L8c61e530; /* bf 0x8c61e534 */
L8c61e530: sa_motion_publish_stop(ctx,0x8c61e532u); goto L8c61ec08; /* bra 0x8c61ec08 */
L8c61e534: sa_motion_publish_stop(ctx,0x8c61e534u); r[1]=rd(m,bus,&io_status,0x8c61e564u,4);if(io_status)return io_status;goto L8c61e536; /* mov.l 0x8c61e564,r1 */
L8c61e536: sa_motion_publish_stop(ctx,0x8c61e536u); *sr=(*sr&~1u)|(r[1]==r[0]);goto L8c61e538; /* cmp/eq r1,r0 */
L8c61e538: sa_motion_publish_stop(ctx,0x8c61e538u); if(!(*sr&1u))goto L8c61e53e;goto L8c61e53a; /* bf 0x8c61e53e */
L8c61e53a: sa_motion_publish_stop(ctx,0x8c61e53cu); goto L8c61f042; /* bra 0x8c61f042 */
L8c61e53e: sa_motion_publish_stop(ctx,0x8c61e540u); goto L8c61f0bc; /* bra 0x8c61f0bc */
L8c61e542: sa_motion_publish_stop(ctx,0x8c61e542u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61e544; /* mov.l @r15,r2 */
L8c61e544: sa_motion_publish_stop(ctx,0x8c61e544u); r[1]=rd(m,bus,&io_status,0x8c61e56cu,4);if(io_status)return io_status;goto L8c61e546; /* mov.l 0x8c61e56c,r1 */
L8c61e546: sa_motion_publish_stop(ctx,0x8c61e546u); r[0]=sx(m,bus,&io_status,r[2]+0x2u,2);if(io_status)return io_status;goto L8c61e548; /* mov.w @(0x2,r2),r0 */
L8c61e548: sa_motion_publish_stop(ctx,0x8c61e548u); r[0]=r[0]&65535u;goto L8c61e54a; /* extu.w r0,r0 */
L8c61e54a: sa_motion_publish_stop(ctx,0x8c61e54au); r[2]=r[0];goto L8c61e54c; /* mov r0,r2 */
L8c61e54c: sa_motion_publish_stop(ctx,0x8c61e54cu); *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c61e54e; /* shll r0 */
L8c61e54e: sa_motion_publish_stop(ctx,0x8c61e54eu); r[0]+=r[2];goto L8c61e550; /* add r2,r0 */
L8c61e550: sa_motion_publish_stop(ctx,0x8c61e550u); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61e552; /* mov.l @r1,r2 */
L8c61e552: sa_motion_publish_stop(ctx,0x8c61e552u); r[2]+=r[0];goto L8c61e554; /* add r0,r2 */
L8c61e554: sa_motion_publish_stop(ctx,0x8c61e554u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61e556; /* mov.l r2,@r1 */
L8c61e556: sa_motion_publish_stop(ctx,0x8c61e558u); r[11]=0u;goto L8c61e81a; /* bra 0x8c61e81a */
L8c61e570: sa_motion_publish_stop(ctx,0x8c61e570u); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e572; /* mov.w @r10+,r14 */
L8c61e572: sa_motion_publish_stop(ctx,0x8c61e572u); r[3]=rd(m,bus,&io_status,0x8c61e840u,4);if(io_status)return io_status;goto L8c61e574; /* mov.l 0x8c61e840,r3 */
L8c61e574: sa_motion_publish_stop(ctx,0x8c61e574u); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e576; /* mov.w @r10+,r12 */
L8c61e576: sa_motion_publish_stop(ctx,0x8c61e576u); r[14]=r[14]&65535u;goto L8c61e578; /* extu.w r14,r14 */
L8c61e578: sa_motion_publish_stop(ctx,0x8c61e578u); r[14]<<=2;goto L8c61e57a; /* shll2 r14 */
L8c61e57a: sa_motion_publish_stop(ctx,0x8c61e57au); r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e57c; /* mov.l @r3,r5 */
L8c61e57c: sa_motion_publish_stop(ctx,0x8c61e57cu); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e57e; /* mov.w @r10+,r13 */
L8c61e57e: sa_motion_publish_stop(ctx,0x8c61e57eu); r[12]=r[12]&65535u;goto L8c61e580; /* extu.w r12,r12 */
L8c61e580: sa_motion_publish_stop(ctx,0x8c61e580u); r[12]<<=2;goto L8c61e582; /* shll2 r12 */
L8c61e582: sa_motion_publish_stop(ctx,0x8c61e582u); r[13]=r[13]&65535u;goto L8c61e584; /* extu.w r13,r13 */
L8c61e584: sa_motion_publish_stop(ctx,0x8c61e584u); r[12]<<=2;goto L8c61e586; /* shll2 r12 */
L8c61e586: sa_motion_publish_stop(ctx,0x8c61e586u); r[13]<<=2;goto L8c61e588; /* shll2 r13 */
L8c61e588: sa_motion_publish_stop(ctx,0x8c61e588u); r[14]<<=2;goto L8c61e58a; /* shll2 r14 */
L8c61e58a: sa_motion_publish_stop(ctx,0x8c61e58au); r[13]<<=2;goto L8c61e58c; /* shll2 r13 */
L8c61e58c: sa_motion_publish_stop(ctx,0x8c61e58cu); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61e58e; /* shll r14 */
L8c61e58e: sa_motion_publish_stop(ctx,0x8c61e58eu); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61e590; /* shll r12 */
L8c61e590: sa_motion_publish_stop(ctx,0x8c61e590u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61e592; /* shll r13 */
L8c61e592: sa_motion_publish_stop(ctx,0x8c61e592u); r[14]+=r[5];goto L8c61e594; /* add r5,r14 */
L8c61e594: sa_motion_publish_stop(ctx,0x8c61e594u); r[12]+=r[5];goto L8c61e596; /* add r5,r12 */
L8c61e596: sa_motion_publish_stop(ctx,0x8c61e596u); r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61e598; /* mov.l @(0xc,r14),r4 */
L8c61e598: sa_motion_publish_stop(ctx,0x8c61e598u); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61e59a; /* mov.l @(0xc,r12),r2 */
L8c61e59a: sa_motion_publish_stop(ctx,0x8c61e59au); r[13]+=r[5];goto L8c61e59c; /* add r5,r13 */
L8c61e59c: sa_motion_publish_stop(ctx,0x8c61e59cu); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61e59e; /* mov.l @(0xc,r13),r1 */
L8c61e59e: sa_motion_publish_stop(ctx,0x8c61e59eu); r[4]&=r[2];goto L8c61e5a0; /* and r2,r4 */
L8c61e5a0: sa_motion_publish_stop(ctx,0x8c61e5a0u); r[2]=63u;goto L8c61e5a2; /* mov #0x3f,r2 */
L8c61e5a2: sa_motion_publish_stop(ctx,0x8c61e5a2u); r[4]&=r[1];goto L8c61e5a4; /* and r1,r4 */
L8c61e5a4: sa_motion_publish_stop(ctx,0x8c61e5a4u); *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61e5a6; /* tst r2,r4 */
L8c61e5a6: sa_motion_publish_stop(ctx,0x8c61e5a6u); if((*sr&1u))goto L8c61e5ac;goto L8c61e5a8; /* bt 0x8c61e5ac */
L8c61e5a8: sa_motion_publish_stop(ctx,0x8c61e5aau); goto L8c61e818; /* bra 0x8c61e818 */
L8c61e5ac: sa_motion_publish_stop(ctx,0x8c61e5acu); r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61e5ae; /* mov.l @(0xc,r14),r4 */
L8c61e5ae: sa_motion_publish_stop(ctx,0x8c61e5aeu); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61e5b0; /* mov.l @(0xc,r12),r3 */
L8c61e5b0: sa_motion_publish_stop(ctx,0x8c61e5b0u); r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61e5b2; /* mov.l @(0xc,r13),r2 */
L8c61e5b2: sa_motion_publish_stop(ctx,0x8c61e5b2u); r[4]|=r[3];goto L8c61e5b4; /* or r3,r4 */
L8c61e5b4: sa_motion_publish_stop(ctx,0x8c61e5b4u); r[3]=1u;goto L8c61e5b6; /* mov #0x1,r3 */
L8c61e5b6: sa_motion_publish_stop(ctx,0x8c61e5b6u); r[4]|=r[2];goto L8c61e5b8; /* or r2,r4 */
L8c61e5b8: sa_motion_publish_stop(ctx,0x8c61e5b8u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61e5ba; /* tst r3,r4 */
L8c61e5ba: sa_motion_publish_stop(ctx,0x8c61e5bau); if((*sr&1u))goto L8c61e5c0;goto L8c61e5bc; /* bt 0x8c61e5c0 */
L8c61e5bc: sa_motion_publish_stop(ctx,0x8c61e5beu); goto L8c61e7aa; /* bra 0x8c61e7aa */
L8c61e5c0: sa_motion_publish_stop(ctx,0x8c61e5c0u); r[0]=16u;goto L8c61e5c2; /* mov #0x10,r0 */
L8c61e5c2: sa_motion_publish_stop(ctx,0x8c61e5c2u); fload(m,bus,&io_status,f,4u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e5c4; /* fmov.s @(r0,r12),fr4 */
L8c61e5c4: sa_motion_publish_stop(ctx,0x8c61e5c4u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e5c6; /* fmov.s @(r0,r14),fr3 */
L8c61e5c6: sa_motion_publish_stop(ctx,0x8c61e5c6u); r[0]=20u;goto L8c61e5c8; /* mov #0x14,r0 */
L8c61e5c8: sa_motion_publish_stop(ctx,0x8c61e5c8u); fcopy(f,2u,4u);goto L8c61e5ca; /* fmov fr4,fr2 */
L8c61e5ca: sa_motion_publish_stop(ctx,0x8c61e5cau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61e5cc; /* fsub fr3,fr2 */
L8c61e5cc: sa_motion_publish_stop(ctx,0x8c61e5ccu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e5ce; /* fmov.s @(r0,r13),fr3 */
L8c61e5ce: sa_motion_publish_stop(ctx,0x8c61e5ceu); fload(m,bus,&io_status,f,5u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e5d0; /* fmov.s @(r0,r12),fr5 */
L8c61e5d0: sa_motion_publish_stop(ctx,0x8c61e5d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e5d2; /* fsub fr5,fr3 */
L8c61e5d2: sa_motion_publish_stop(ctx,0x8c61e5d2u); fcopy(f,1u,5u);goto L8c61e5d4; /* fmov fr5,fr1 */
L8c61e5d4: sa_motion_publish_stop(ctx,0x8c61e5d4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e5d6; /* fmul fr3,fr2 */
L8c61e5d6: sa_motion_publish_stop(ctx,0x8c61e5d6u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e5d8; /* fmov.s @(r0,r14),fr3 */
L8c61e5d8: sa_motion_publish_stop(ctx,0x8c61e5d8u); r[0]=16u;goto L8c61e5da; /* mov #0x10,r0 */
L8c61e5da: sa_motion_publish_stop(ctx,0x8c61e5dau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61e5dc; /* fsub fr3,fr1 */
L8c61e5dc: sa_motion_publish_stop(ctx,0x8c61e5dcu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e5de; /* fmov.s @(r0,r13),fr3 */
L8c61e5de: sa_motion_publish_stop(ctx,0x8c61e5deu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e5e0; /* fsub fr4,fr3 */
L8c61e5e0: sa_motion_publish_stop(ctx,0x8c61e5e0u); fcopy(f,4u,2u);goto L8c61e5e2; /* fmov fr2,fr4 */
L8c61e5e2: sa_motion_publish_stop(ctx,0x8c61e5e2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e5e4; /* fmul fr3,fr1 */
L8c61e5e4: sa_motion_publish_stop(ctx,0x8c61e5e4u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61e5e6; /* fldi0 fr3 */
L8c61e5e6: sa_motion_publish_stop(ctx,0x8c61e5e6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e5e8; /* fsub fr1,fr4 */
L8c61e5e8: sa_motion_publish_stop(ctx,0x8c61e5e8u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61e5ea; /* fcmp/gt fr3,fr4 */
L8c61e5ea: sa_motion_publish_stop(ctx,0x8c61e5eau); if(!(*sr&1u))goto L8c61e5f0;goto L8c61e5ec; /* bf 0x8c61e5f0 */
L8c61e5ec: sa_motion_publish_stop(ctx,0x8c61e5eeu); goto L8c61e818; /* bra 0x8c61e818 */
L8c61e5f0: sa_motion_publish_stop(ctx,0x8c61e5f0u); fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61e5f2; /* fmov.s @r14,fr2 */
L8c61e5f2: sa_motion_publish_stop(ctx,0x8c61e5f2u); r[0]=12u;goto L8c61e5f4; /* mov #0xc,r0 */
L8c61e5f4: sa_motion_publish_stop(ctx,0x8c61e5f4u); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61e5f6; /* fmov.s @r12,fr3 */
L8c61e5f6: sa_motion_publish_stop(ctx,0x8c61e5f6u); r[4]=r[15];goto L8c61e5f8; /* mov r15,r4 */
L8c61e5f8: sa_motion_publish_stop(ctx,0x8c61e5f8u); r[3]=rd(m,bus,&io_status,0x8c61e844u,4);if(io_status)return io_status;goto L8c61e5fa; /* mov.l 0x8c61e844,r3 */
L8c61e5fa: sa_motion_publish_stop(ctx,0x8c61e5fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e5fc; /* fadd fr3,fr2 */
L8c61e5fc: sa_motion_publish_stop(ctx,0x8c61e5fcu); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61e5fe; /* fmov.s @r13,fr3 */
L8c61e5fe: sa_motion_publish_stop(ctx,0x8c61e5feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e600; /* fadd fr3,fr2 */
L8c61e600: sa_motion_publish_stop(ctx,0x8c61e600u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e602; /* fmov fr2,@(r0,r15) */
L8c61e602: sa_motion_publish_stop(ctx,0x8c61e602u); r[0]=4u;goto L8c61e604; /* mov #0x4,r0 */
L8c61e604: sa_motion_publish_stop(ctx,0x8c61e604u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e606; /* fmov.s @(r0,r14),fr2 */
L8c61e606: sa_motion_publish_stop(ctx,0x8c61e606u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e608; /* fmov.s @(r0,r12),fr3 */
L8c61e608: sa_motion_publish_stop(ctx,0x8c61e608u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e60a; /* fadd fr3,fr2 */
L8c61e60a: sa_motion_publish_stop(ctx,0x8c61e60au); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e60c; /* fmov.s @(r0,r13),fr3 */
L8c61e60c: sa_motion_publish_stop(ctx,0x8c61e60cu); r[0]=16u;goto L8c61e60e; /* mov #0x10,r0 */
L8c61e60e: sa_motion_publish_stop(ctx,0x8c61e60eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e610; /* fadd fr3,fr2 */
L8c61e610: sa_motion_publish_stop(ctx,0x8c61e610u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e612; /* fmov fr2,@(r0,r15) */
L8c61e612: sa_motion_publish_stop(ctx,0x8c61e612u); r[0]=8u;goto L8c61e614; /* mov #0x8,r0 */
L8c61e614: sa_motion_publish_stop(ctx,0x8c61e614u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e616; /* fmov.s @(r0,r14),fr2 */
L8c61e616: sa_motion_publish_stop(ctx,0x8c61e616u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e618; /* fmov.s @(r0,r12),fr3 */
L8c61e618: sa_motion_publish_stop(ctx,0x8c61e618u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e61a; /* fadd fr3,fr2 */
L8c61e61a: sa_motion_publish_stop(ctx,0x8c61e61au); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e61c; /* fmov.s @(r0,r13),fr3 */
L8c61e61c: sa_motion_publish_stop(ctx,0x8c61e61cu); r[0]=20u;goto L8c61e61e; /* mov #0x14,r0 */
L8c61e61e: sa_motion_publish_stop(ctx,0x8c61e61eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e620; /* fadd fr3,fr2 */
L8c61e620: sa_motion_publish_stop(ctx,0x8c61e620u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e622; /* fmov fr2,@(r0,r15) */
L8c61e622: sa_motion_publish_stop(ctx,0x8c61e624u); target=r[3];*pr=0x8c61e626u;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61e626; /* jsr @r3 */
L8c61e626: sa_motion_publish_stop(ctx,0x8c61e626u); r[3]=rd(m,bus,&io_status,0x8c61e848u,4);if(io_status)return io_status;goto L8c61e628; /* mov.l 0x8c61e848,r3 */
L8c61e628: sa_motion_publish_stop(ctx,0x8c61e628u); r[0]=12u;goto L8c61e62a; /* mov #0xc,r0 */
L8c61e62a: sa_motion_publish_stop(ctx,0x8c61e62au); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e62c; /* fmov.s @(r0,r15),fr3 */
L8c61e62c: sa_motion_publish_stop(ctx,0x8c61e62cu); r[0]=16u;goto L8c61e62e; /* mov #0x10,r0 */
L8c61e62e: sa_motion_publish_stop(ctx,0x8c61e62eu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61e630; /* fmov.s @r3,fr2 */
L8c61e630: sa_motion_publish_stop(ctx,0x8c61e630u); r[3]+=8u;goto L8c61e632; /* add #0x8,r3 */
L8c61e632: sa_motion_publish_stop(ctx,0x8c61e632u); r[2]=rd(m,bus,&io_status,0x8c61e84cu,4);if(io_status)return io_status;goto L8c61e634; /* mov.l 0x8c61e84c,r2 */
L8c61e634: sa_motion_publish_stop(ctx,0x8c61e634u); r[8]+=24u;goto L8c61e636; /* add #0x18,r8 */
L8c61e636: sa_motion_publish_stop(ctx,0x8c61e636u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e638; /* fmul fr3,fr2 */
L8c61e638: sa_motion_publish_stop(ctx,0x8c61e638u); r[1]=rd(m,bus,&io_status,0x8c61e850u,4);if(io_status)return io_status;goto L8c61e63a; /* mov.l 0x8c61e850,r1 */
L8c61e63a: sa_motion_publish_stop(ctx,0x8c61e63au); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61e63c; /* fmov.s @r2,fr3 */
L8c61e63c: sa_motion_publish_stop(ctx,0x8c61e63cu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61e63e; /* fmov.s @r1,fr1 */
L8c61e63e: sa_motion_publish_stop(ctx,0x8c61e63eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e640; /* fsub fr2,fr3 */
L8c61e640: sa_motion_publish_stop(ctx,0x8c61e640u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e642; /* fmov.s @(r0,r15),fr2 */
L8c61e642: sa_motion_publish_stop(ctx,0x8c61e642u); r[0]=20u;goto L8c61e644; /* mov #0x14,r0 */
L8c61e644: sa_motion_publish_stop(ctx,0x8c61e644u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e646; /* fmul fr2,fr1 */
L8c61e646: sa_motion_publish_stop(ctx,0x8c61e646u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e648; /* fmov.s @(r0,r15),fr2 */
L8c61e648: sa_motion_publish_stop(ctx,0x8c61e648u); r[0]=rd(m,bus,&io_status,0x8c61e854u,4);if(io_status)return io_status;goto L8c61e64a; /* mov.l 0x8c61e854,r0 */
L8c61e64a: sa_motion_publish_stop(ctx,0x8c61e64au); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61e64c; /* mov.l @r0,r2 */
L8c61e64c: sa_motion_publish_stop(ctx,0x8c61e64cu); r[0]=4u;goto L8c61e64e; /* mov #0x4,r0 */
L8c61e64e: sa_motion_publish_stop(ctx,0x8c61e64eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e650; /* fsub fr1,fr3 */
L8c61e650: sa_motion_publish_stop(ctx,0x8c61e650u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61e652; /* fmov.s @r3,fr1 */
L8c61e652: sa_motion_publish_stop(ctx,0x8c61e652u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e654; /* fmov.s @(r0,r14),fr0 */
L8c61e654: sa_motion_publish_stop(ctx,0x8c61e654u); r[1]=r[2];goto L8c61e656; /* mov r2,r1 */
L8c61e656: sa_motion_publish_stop(ctx,0x8c61e656u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e658; /* fmul fr2,fr1 */
L8c61e658: sa_motion_publish_stop(ctx,0x8c61e658u); r[0]=20u;goto L8c61e65a; /* mov #0x14,r0 */
L8c61e65a: sa_motion_publish_stop(ctx,0x8c61e65au); r[3]=r[2];goto L8c61e65c; /* mov r2,r3 */
L8c61e65c: sa_motion_publish_stop(ctx,0x8c61e65cu); fcopy(f,4u,3u);goto L8c61e65e; /* fmov fr3,fr4 */
L8c61e65e: sa_motion_publish_stop(ctx,0x8c61e65eu); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e660; /* fmov.s @(r0,r1),fr3 */
L8c61e660: sa_motion_publish_stop(ctx,0x8c61e660u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e662; /* fsub fr1,fr4 */
L8c61e662: sa_motion_publish_stop(ctx,0x8c61e662u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e664; /* fmul fr0,fr3 */
L8c61e664: sa_motion_publish_stop(ctx,0x8c61e664u); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61e666; /* fmov.s @r14,fr0 */
L8c61e666: sa_motion_publish_stop(ctx,0x8c61e666u); r[0]=4u;goto L8c61e668; /* mov #0x4,r0 */
L8c61e668: sa_motion_publish_stop(ctx,0x8c61e668u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e66a; /* fmov.s @(r0,r3),fr2 */
L8c61e66a: sa_motion_publish_stop(ctx,0x8c61e66au); r[0]=8u;goto L8c61e66c; /* mov #0x8,r0 */
L8c61e66c: sa_motion_publish_stop(ctx,0x8c61e66cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e66e; /* fmac fr0,fr2,fr3 */
L8c61e66e: sa_motion_publish_stop(ctx,0x8c61e66eu); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e670; /* fmov.s @(r0,r14),fr0 */
L8c61e670: sa_motion_publish_stop(ctx,0x8c61e670u); r[0]=36u;goto L8c61e672; /* mov #0x24,r0 */
L8c61e672: sa_motion_publish_stop(ctx,0x8c61e672u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e674; /* fmov.s @(r0,r2),fr2 */
L8c61e674: sa_motion_publish_stop(ctx,0x8c61e674u); r[0]=4u;goto L8c61e676; /* mov #0x4,r0 */
L8c61e676: sa_motion_publish_stop(ctx,0x8c61e676u); r[2]=rd(m,bus,&io_status,0x8c61e854u,4);if(io_status)return io_status;goto L8c61e678; /* mov.l 0x8c61e854,r2 */
L8c61e678: sa_motion_publish_stop(ctx,0x8c61e678u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e67a; /* fmac fr0,fr2,fr3 */
L8c61e67a: sa_motion_publish_stop(ctx,0x8c61e67au); fcopy(f,0u,14u);goto L8c61e67c; /* fmov fr14,fr0 */
L8c61e67c: sa_motion_publish_stop(ctx,0x8c61e67cu); fcopy(f,2u,14u);goto L8c61e67e; /* fmov fr14,fr2 */
L8c61e67e: sa_motion_publish_stop(ctx,0x8c61e67eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e680; /* fmac fr0,fr3,fr2 */
L8c61e680: sa_motion_publish_stop(ctx,0x8c61e680u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e682; /* fmov.s fr2,@-r8 */
L8c61e682: sa_motion_publish_stop(ctx,0x8c61e682u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e684; /* mov.l @r2,r3 */
L8c61e684: sa_motion_publish_stop(ctx,0x8c61e684u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e686; /* fmov.s @(r0,r14),fr0 */
L8c61e686: sa_motion_publish_stop(ctx,0x8c61e686u); r[0]=16u;goto L8c61e688; /* mov #0x10,r0 */
L8c61e688: sa_motion_publish_stop(ctx,0x8c61e688u); r[2]=r[3];goto L8c61e68a; /* mov r3,r2 */
L8c61e68a: sa_motion_publish_stop(ctx,0x8c61e68au); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e68c; /* fmov.s @(r0,r2),fr3 */
L8c61e68c: sa_motion_publish_stop(ctx,0x8c61e68cu); r[1]=r[3];goto L8c61e68e; /* mov r3,r1 */
L8c61e68e: sa_motion_publish_stop(ctx,0x8c61e68eu); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61e690; /* fmov.s @r1,fr2 */
L8c61e690: sa_motion_publish_stop(ctx,0x8c61e690u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e692; /* fmul fr0,fr3 */
L8c61e692: sa_motion_publish_stop(ctx,0x8c61e692u); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61e694; /* fmov.s @r14,fr0 */
L8c61e694: sa_motion_publish_stop(ctx,0x8c61e694u); r[0]=8u;goto L8c61e696; /* mov #0x8,r0 */
L8c61e696: sa_motion_publish_stop(ctx,0x8c61e696u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e698; /* fmac fr0,fr2,fr3 */
L8c61e698: sa_motion_publish_stop(ctx,0x8c61e698u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e69a; /* fmov.s @(r0,r14),fr0 */
L8c61e69a: sa_motion_publish_stop(ctx,0x8c61e69au); r[0]=32u;goto L8c61e69c; /* mov #0x20,r0 */
L8c61e69c: sa_motion_publish_stop(ctx,0x8c61e69cu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e69e; /* fmov.s @(r0,r3),fr2 */
L8c61e69e: sa_motion_publish_stop(ctx,0x8c61e69eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e6a0; /* fmac fr0,fr2,fr3 */
L8c61e6a0: sa_motion_publish_stop(ctx,0x8c61e6a0u); fcopy(f,0u,14u);goto L8c61e6a2; /* fmov fr14,fr0 */
L8c61e6a2: sa_motion_publish_stop(ctx,0x8c61e6a2u); fcopy(f,2u,14u);goto L8c61e6a4; /* fmov fr14,fr2 */
L8c61e6a4: sa_motion_publish_stop(ctx,0x8c61e6a4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e6a6; /* fmac fr0,fr3,fr2 */
L8c61e6a6: sa_motion_publish_stop(ctx,0x8c61e6a6u); r[0]=24u;goto L8c61e6a8; /* mov #0x18,r0 */
L8c61e6a8: sa_motion_publish_stop(ctx,0x8c61e6a8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e6aa; /* fmov.s fr2,@-r8 */
L8c61e6aa: sa_motion_publish_stop(ctx,0x8c61e6aau); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e6ac; /* fmov.s @(r0,r14),fr3 */
L8c61e6ac: sa_motion_publish_stop(ctx,0x8c61e6acu); r[0]=20u;goto L8c61e6ae; /* mov #0x14,r0 */
L8c61e6ae: sa_motion_publish_stop(ctx,0x8c61e6aeu); r[3]=rd(m,bus,&io_status,0x8c61e858u,4);if(io_status)return io_status;goto L8c61e6b0; /* mov.l 0x8c61e858,r3 */
L8c61e6b0: sa_motion_publish_stop(ctx,0x8c61e6b0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e6b2; /* fmov.s fr3,@-r8 */
L8c61e6b2: sa_motion_publish_stop(ctx,0x8c61e6b2u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e6b4; /* fmov.s @(r0,r14),fr3 */
L8c61e6b4: sa_motion_publish_stop(ctx,0x8c61e6b4u); r[0]=16u;goto L8c61e6b6; /* mov #0x10,r0 */
L8c61e6b6: sa_motion_publish_stop(ctx,0x8c61e6b6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e6b8; /* fmov.s fr3,@-r8 */
L8c61e6b8: sa_motion_publish_stop(ctx,0x8c61e6b8u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e6ba; /* fmov.s @(r0,r14),fr3 */
L8c61e6ba: sa_motion_publish_stop(ctx,0x8c61e6bau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e6bc; /* fmov.s fr3,@-r8 */
L8c61e6bc: sa_motion_publish_stop(ctx,0x8c61e6bcu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e6be; /* mov.l r3,@-r8 */
L8c61e6be: sa_motion_publish_stop(ctx,0x8c61e6beu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e6c0; /* pref @r8 */
L8c61e6c0: sa_motion_publish_stop(ctx,0x8c61e6c0u); r[2]=rd(m,bus,&io_status,0x8c61e854u,4);if(io_status)return io_status;goto L8c61e6c2; /* mov.l 0x8c61e854,r2 */
L8c61e6c2: sa_motion_publish_stop(ctx,0x8c61e6c2u); r[0]=4u;goto L8c61e6c4; /* mov #0x4,r0 */
L8c61e6c4: sa_motion_publish_stop(ctx,0x8c61e6c4u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e6c6; /* fmov.s @(r0,r12),fr0 */
L8c61e6c6: sa_motion_publish_stop(ctx,0x8c61e6c6u); r[0]=20u;goto L8c61e6c8; /* mov #0x14,r0 */
L8c61e6c8: sa_motion_publish_stop(ctx,0x8c61e6c8u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e6ca; /* mov.l @r2,r3 */
L8c61e6ca: sa_motion_publish_stop(ctx,0x8c61e6cau); r[8]+=56u;goto L8c61e6cc; /* add #0x38,r8 */
L8c61e6cc: sa_motion_publish_stop(ctx,0x8c61e6ccu); r[2]=r[3];goto L8c61e6ce; /* mov r3,r2 */
L8c61e6ce: sa_motion_publish_stop(ctx,0x8c61e6ceu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e6d0; /* fmov.s @(r0,r2),fr3 */
L8c61e6d0: sa_motion_publish_stop(ctx,0x8c61e6d0u); r[0]=4u;goto L8c61e6d2; /* mov #0x4,r0 */
L8c61e6d2: sa_motion_publish_stop(ctx,0x8c61e6d2u); r[1]=r[3];goto L8c61e6d4; /* mov r3,r1 */
L8c61e6d4: sa_motion_publish_stop(ctx,0x8c61e6d4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e6d6; /* fmul fr0,fr3 */
L8c61e6d6: sa_motion_publish_stop(ctx,0x8c61e6d6u); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e6d8; /* fmov.s @(r0,r1),fr2 */
L8c61e6d8: sa_motion_publish_stop(ctx,0x8c61e6d8u); fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61e6da; /* fmov.s @r12,fr0 */
L8c61e6da: sa_motion_publish_stop(ctx,0x8c61e6dau); r[0]=8u;goto L8c61e6dc; /* mov #0x8,r0 */
L8c61e6dc: sa_motion_publish_stop(ctx,0x8c61e6dcu); r[2]=rd(m,bus,&io_status,0x8c61e854u,4);if(io_status)return io_status;goto L8c61e6de; /* mov.l 0x8c61e854,r2 */
L8c61e6de: sa_motion_publish_stop(ctx,0x8c61e6deu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e6e0; /* fmac fr0,fr2,fr3 */
L8c61e6e0: sa_motion_publish_stop(ctx,0x8c61e6e0u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e6e2; /* fmov.s @(r0,r12),fr0 */
L8c61e6e2: sa_motion_publish_stop(ctx,0x8c61e6e2u); r[0]=36u;goto L8c61e6e4; /* mov #0x24,r0 */
L8c61e6e4: sa_motion_publish_stop(ctx,0x8c61e6e4u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e6e6; /* fmov.s @(r0,r3),fr2 */
L8c61e6e6: sa_motion_publish_stop(ctx,0x8c61e6e6u); r[0]=4u;goto L8c61e6e8; /* mov #0x4,r0 */
L8c61e6e8: sa_motion_publish_stop(ctx,0x8c61e6e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e6ea; /* fmac fr0,fr2,fr3 */
L8c61e6ea: sa_motion_publish_stop(ctx,0x8c61e6eau); fcopy(f,0u,14u);goto L8c61e6ec; /* fmov fr14,fr0 */
L8c61e6ec: sa_motion_publish_stop(ctx,0x8c61e6ecu); fcopy(f,2u,14u);goto L8c61e6ee; /* fmov fr14,fr2 */
L8c61e6ee: sa_motion_publish_stop(ctx,0x8c61e6eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e6f0; /* fmac fr0,fr3,fr2 */
L8c61e6f0: sa_motion_publish_stop(ctx,0x8c61e6f0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e6f2; /* fmov.s fr2,@-r8 */
L8c61e6f2: sa_motion_publish_stop(ctx,0x8c61e6f2u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e6f4; /* mov.l @r2,r3 */
L8c61e6f4: sa_motion_publish_stop(ctx,0x8c61e6f4u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e6f6; /* fmov.s @(r0,r12),fr0 */
L8c61e6f6: sa_motion_publish_stop(ctx,0x8c61e6f6u); r[0]=16u;goto L8c61e6f8; /* mov #0x10,r0 */
L8c61e6f8: sa_motion_publish_stop(ctx,0x8c61e6f8u); r[2]=r[3];goto L8c61e6fa; /* mov r3,r2 */
L8c61e6fa: sa_motion_publish_stop(ctx,0x8c61e6fau); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e6fc; /* fmov.s @(r0,r2),fr3 */
L8c61e6fc: sa_motion_publish_stop(ctx,0x8c61e6fcu); r[1]=r[3];goto L8c61e6fe; /* mov r3,r1 */
L8c61e6fe: sa_motion_publish_stop(ctx,0x8c61e6feu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e700; /* fmul fr0,fr3 */
L8c61e700: sa_motion_publish_stop(ctx,0x8c61e700u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61e702; /* fmov.s @r1,fr2 */
L8c61e702: sa_motion_publish_stop(ctx,0x8c61e702u); r[0]=8u;goto L8c61e704; /* mov #0x8,r0 */
L8c61e704: sa_motion_publish_stop(ctx,0x8c61e704u); fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61e706; /* fmov.s @r12,fr0 */
L8c61e706: sa_motion_publish_stop(ctx,0x8c61e706u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e708; /* fmac fr0,fr2,fr3 */
L8c61e708: sa_motion_publish_stop(ctx,0x8c61e708u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e70a; /* fmov.s @(r0,r12),fr0 */
L8c61e70a: sa_motion_publish_stop(ctx,0x8c61e70au); r[0]=32u;goto L8c61e70c; /* mov #0x20,r0 */
L8c61e70c: sa_motion_publish_stop(ctx,0x8c61e70cu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e70e; /* fmov.s @(r0,r3),fr2 */
L8c61e70e: sa_motion_publish_stop(ctx,0x8c61e70eu); r[0]=24u;goto L8c61e710; /* mov #0x18,r0 */
L8c61e710: sa_motion_publish_stop(ctx,0x8c61e710u); r[3]=rd(m,bus,&io_status,0x8c61e858u,4);if(io_status)return io_status;goto L8c61e712; /* mov.l 0x8c61e858,r3 */
L8c61e712: sa_motion_publish_stop(ctx,0x8c61e712u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e714; /* fmac fr0,fr2,fr3 */
L8c61e714: sa_motion_publish_stop(ctx,0x8c61e714u); fcopy(f,0u,14u);goto L8c61e716; /* fmov fr14,fr0 */
L8c61e716: sa_motion_publish_stop(ctx,0x8c61e716u); fcopy(f,2u,14u);goto L8c61e718; /* fmov fr14,fr2 */
L8c61e718: sa_motion_publish_stop(ctx,0x8c61e718u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e71a; /* fmac fr0,fr3,fr2 */
L8c61e71a: sa_motion_publish_stop(ctx,0x8c61e71au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e71c; /* fmov.s fr2,@-r8 */
L8c61e71c: sa_motion_publish_stop(ctx,0x8c61e71cu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e71e; /* fmov.s @(r0,r12),fr3 */
L8c61e71e: sa_motion_publish_stop(ctx,0x8c61e71eu); r[0]=20u;goto L8c61e720; /* mov #0x14,r0 */
L8c61e720: sa_motion_publish_stop(ctx,0x8c61e720u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e722; /* fmov.s fr3,@-r8 */
L8c61e722: sa_motion_publish_stop(ctx,0x8c61e722u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e724; /* fmov.s @(r0,r12),fr3 */
L8c61e724: sa_motion_publish_stop(ctx,0x8c61e724u); r[0]=16u;goto L8c61e726; /* mov #0x10,r0 */
L8c61e726: sa_motion_publish_stop(ctx,0x8c61e726u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e728; /* fmov.s fr3,@-r8 */
L8c61e728: sa_motion_publish_stop(ctx,0x8c61e728u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e72a; /* fmov.s @(r0,r12),fr3 */
L8c61e72a: sa_motion_publish_stop(ctx,0x8c61e72au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e72c; /* fmov.s fr3,@-r8 */
L8c61e72c: sa_motion_publish_stop(ctx,0x8c61e72cu); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e72e; /* mov.l r3,@-r8 */
L8c61e72e: sa_motion_publish_stop(ctx,0x8c61e72eu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e730; /* pref @r8 */
L8c61e730: sa_motion_publish_stop(ctx,0x8c61e730u); r[2]=rd(m,bus,&io_status,0x8c61e854u,4);if(io_status)return io_status;goto L8c61e732; /* mov.l 0x8c61e854,r2 */
L8c61e732: sa_motion_publish_stop(ctx,0x8c61e732u); r[0]=4u;goto L8c61e734; /* mov #0x4,r0 */
L8c61e734: sa_motion_publish_stop(ctx,0x8c61e734u); r[8]+=60u;goto L8c61e736; /* add #0x3c,r8 */
L8c61e736: sa_motion_publish_stop(ctx,0x8c61e736u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61e738; /* fmov.s fr4,@-r8 */
L8c61e738: sa_motion_publish_stop(ctx,0x8c61e738u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e73a; /* mov.l @r2,r3 */
L8c61e73a: sa_motion_publish_stop(ctx,0x8c61e73au); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e73c; /* fmov.s @(r0,r13),fr0 */
L8c61e73c: sa_motion_publish_stop(ctx,0x8c61e73cu); r[0]=20u;goto L8c61e73e; /* mov #0x14,r0 */
L8c61e73e: sa_motion_publish_stop(ctx,0x8c61e73eu); r[2]=r[3];goto L8c61e740; /* mov r3,r2 */
L8c61e740: sa_motion_publish_stop(ctx,0x8c61e740u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e742; /* fmov.s @(r0,r2),fr3 */
L8c61e742: sa_motion_publish_stop(ctx,0x8c61e742u); r[1]=r[3];goto L8c61e744; /* mov r3,r1 */
L8c61e744: sa_motion_publish_stop(ctx,0x8c61e744u); r[0]=4u;goto L8c61e746; /* mov #0x4,r0 */
L8c61e746: sa_motion_publish_stop(ctx,0x8c61e746u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e748; /* fmul fr0,fr3 */
L8c61e748: sa_motion_publish_stop(ctx,0x8c61e748u); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e74a; /* fmov.s @(r0,r1),fr2 */
L8c61e74a: sa_motion_publish_stop(ctx,0x8c61e74au); fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61e74c; /* fmov.s @r13,fr0 */
L8c61e74c: sa_motion_publish_stop(ctx,0x8c61e74cu); r[0]=8u;goto L8c61e74e; /* mov #0x8,r0 */
L8c61e74e: sa_motion_publish_stop(ctx,0x8c61e74eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e750; /* fmac fr0,fr2,fr3 */
L8c61e750: sa_motion_publish_stop(ctx,0x8c61e750u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e752; /* fmov.s @(r0,r13),fr0 */
L8c61e752: sa_motion_publish_stop(ctx,0x8c61e752u); r[0]=36u;goto L8c61e754; /* mov #0x24,r0 */
L8c61e754: sa_motion_publish_stop(ctx,0x8c61e754u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e756; /* fmov.s @(r0,r3),fr2 */
L8c61e756: sa_motion_publish_stop(ctx,0x8c61e756u); r[0]=4u;goto L8c61e758; /* mov #0x4,r0 */
L8c61e758: sa_motion_publish_stop(ctx,0x8c61e758u); r[3]=rd(m,bus,&io_status,0x8c61e854u,4);if(io_status)return io_status;goto L8c61e75a; /* mov.l 0x8c61e854,r3 */
L8c61e75a: sa_motion_publish_stop(ctx,0x8c61e75au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e75c; /* fmac fr0,fr2,fr3 */
L8c61e75c: sa_motion_publish_stop(ctx,0x8c61e75cu); fcopy(f,2u,14u);goto L8c61e75e; /* fmov fr14,fr2 */
L8c61e75e: sa_motion_publish_stop(ctx,0x8c61e75eu); fcopy(f,0u,14u);goto L8c61e760; /* fmov fr14,fr0 */
L8c61e760: sa_motion_publish_stop(ctx,0x8c61e760u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e762; /* fmac fr0,fr3,fr2 */
L8c61e762: sa_motion_publish_stop(ctx,0x8c61e762u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e764; /* fmov.s fr2,@-r8 */
L8c61e764: sa_motion_publish_stop(ctx,0x8c61e764u); r[9]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e766; /* mov.l @r3,r9 */
L8c61e766: sa_motion_publish_stop(ctx,0x8c61e766u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e768; /* fmov.s @(r0,r13),fr0 */
L8c61e768: sa_motion_publish_stop(ctx,0x8c61e768u); r[0]=16u;goto L8c61e76a; /* mov #0x10,r0 */
L8c61e76a: sa_motion_publish_stop(ctx,0x8c61e76au); fload(m,bus,&io_status,f,3u,r[0]+r[9]);if(io_status)return io_status;goto L8c61e76c; /* fmov.s @(r0,r9),fr3 */
L8c61e76c: sa_motion_publish_stop(ctx,0x8c61e76cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e76e; /* fmul fr0,fr3 */
L8c61e76e: sa_motion_publish_stop(ctx,0x8c61e76eu); fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61e770; /* fmov.s @r13,fr0 */
L8c61e770: sa_motion_publish_stop(ctx,0x8c61e770u); fload(m,bus,&io_status,f,2u,r[9]);if(io_status)return io_status;goto L8c61e772; /* fmov.s @r9,fr2 */
L8c61e772: sa_motion_publish_stop(ctx,0x8c61e772u); r[0]=8u;goto L8c61e774; /* mov #0x8,r0 */
L8c61e774: sa_motion_publish_stop(ctx,0x8c61e774u); r[2]=rd(m,bus,&io_status,0x8c61e85cu,4);if(io_status)return io_status;goto L8c61e776; /* mov.l 0x8c61e85c,r2 */
L8c61e776: sa_motion_publish_stop(ctx,0x8c61e776u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e778; /* fmac fr0,fr2,fr3 */
L8c61e778: sa_motion_publish_stop(ctx,0x8c61e778u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e77a; /* fmov.s @(r0,r13),fr0 */
L8c61e77a: sa_motion_publish_stop(ctx,0x8c61e77au); r[0]=32u;goto L8c61e77c; /* mov #0x20,r0 */
L8c61e77c: sa_motion_publish_stop(ctx,0x8c61e77cu); fload(m,bus,&io_status,f,2u,r[0]+r[9]);if(io_status)return io_status;goto L8c61e77e; /* fmov.s @(r0,r9),fr2 */
L8c61e77e: sa_motion_publish_stop(ctx,0x8c61e77eu); r[0]=24u;goto L8c61e780; /* mov #0x18,r0 */
L8c61e780: sa_motion_publish_stop(ctx,0x8c61e780u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e782; /* fmac fr0,fr2,fr3 */
L8c61e782: sa_motion_publish_stop(ctx,0x8c61e782u); fcopy(f,0u,14u);goto L8c61e784; /* fmov fr14,fr0 */
L8c61e784: sa_motion_publish_stop(ctx,0x8c61e784u); fcopy(f,2u,14u);goto L8c61e786; /* fmov fr14,fr2 */
L8c61e786: sa_motion_publish_stop(ctx,0x8c61e786u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e788; /* fmac fr0,fr3,fr2 */
L8c61e788: sa_motion_publish_stop(ctx,0x8c61e788u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e78a; /* fmov.s fr2,@-r8 */
L8c61e78a: sa_motion_publish_stop(ctx,0x8c61e78au); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e78c; /* fmov.s @(r0,r13),fr3 */
L8c61e78c: sa_motion_publish_stop(ctx,0x8c61e78cu); r[0]=20u;goto L8c61e78e; /* mov #0x14,r0 */
L8c61e78e: sa_motion_publish_stop(ctx,0x8c61e78eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e790; /* fmov.s fr3,@-r8 */
L8c61e790: sa_motion_publish_stop(ctx,0x8c61e790u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e792; /* fmov.s @(r0,r13),fr3 */
L8c61e792: sa_motion_publish_stop(ctx,0x8c61e792u); r[0]=16u;goto L8c61e794; /* mov #0x10,r0 */
L8c61e794: sa_motion_publish_stop(ctx,0x8c61e794u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e796; /* fmov.s fr3,@-r8 */
L8c61e796: sa_motion_publish_stop(ctx,0x8c61e796u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e798; /* fmov.s @(r0,r13),fr3 */
L8c61e798: sa_motion_publish_stop(ctx,0x8c61e798u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e79a; /* fmov.s fr3,@-r8 */
L8c61e79a: sa_motion_publish_stop(ctx,0x8c61e79au); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61e79c; /* mov.l r2,@-r8 */
L8c61e79c: sa_motion_publish_stop(ctx,0x8c61e79cu); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e79e; /* pref @r8 */
L8c61e79e: sa_motion_publish_stop(ctx,0x8c61e79eu); r[1]=rd(m,bus,&io_status,0x8c61e860u,4);if(io_status)return io_status;goto L8c61e7a0; /* mov.l 0x8c61e860,r1 */
L8c61e7a0: sa_motion_publish_stop(ctx,0x8c61e7a0u); r[2]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61e7a2; /* mov.l @r1,r2 */
L8c61e7a2: sa_motion_publish_stop(ctx,0x8c61e7a2u); r[2]+=3u;goto L8c61e7a4; /* add #0x3,r2 */
L8c61e7a4: sa_motion_publish_stop(ctx,0x8c61e7a4u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61e7a6; /* mov.l r2,@r1 */
L8c61e7a6: sa_motion_publish_stop(ctx,0x8c61e7a8u); r[8]+=32u;goto L8c61e818; /* bra 0x8c61e818 */
L8c61e7aa: sa_motion_publish_stop(ctx,0x8c61e7aau); r[0]=rd(m,bus,&io_status,0x8c61e864u,4);if(io_status)return io_status;goto L8c61e7ac; /* mov.l 0x8c61e864,r0 */
L8c61e7ac: sa_motion_publish_stop(ctx,0x8c61e7acu); r[4]=r[15];goto L8c61e7ae; /* mov r15,r4 */
L8c61e7ae: sa_motion_publish_stop(ctx,0x8c61e7aeu); wr(m,bus,&io_status,r[0],r[3],4);if(io_status)return io_status;goto L8c61e7b0; /* mov.l r3,@r0 */
L8c61e7b0: sa_motion_publish_stop(ctx,0x8c61e7b0u); r[0]=12u;goto L8c61e7b2; /* mov #0xc,r0 */
L8c61e7b2: sa_motion_publish_stop(ctx,0x8c61e7b2u); fload(m,bus,&io_status,f,3u,r[12]);if(io_status)return io_status;goto L8c61e7b4; /* fmov.s @r12,fr3 */
L8c61e7b4: sa_motion_publish_stop(ctx,0x8c61e7b4u); fload(m,bus,&io_status,f,2u,r[14]);if(io_status)return io_status;goto L8c61e7b6; /* fmov.s @r14,fr2 */
L8c61e7b6: sa_motion_publish_stop(ctx,0x8c61e7b6u); r[3]=rd(m,bus,&io_status,0x8c61e844u,4);if(io_status)return io_status;goto L8c61e7b8; /* mov.l 0x8c61e844,r3 */
L8c61e7b8: sa_motion_publish_stop(ctx,0x8c61e7b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e7ba; /* fadd fr3,fr2 */
L8c61e7ba: sa_motion_publish_stop(ctx,0x8c61e7bau); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61e7bc; /* fmov.s @r13,fr3 */
L8c61e7bc: sa_motion_publish_stop(ctx,0x8c61e7bcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e7be; /* fadd fr3,fr2 */
L8c61e7be: sa_motion_publish_stop(ctx,0x8c61e7beu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e7c0; /* fmov fr2,@(r0,r15) */
L8c61e7c0: sa_motion_publish_stop(ctx,0x8c61e7c0u); r[0]=4u;goto L8c61e7c2; /* mov #0x4,r0 */
L8c61e7c2: sa_motion_publish_stop(ctx,0x8c61e7c2u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e7c4; /* fmov.s @(r0,r14),fr2 */
L8c61e7c4: sa_motion_publish_stop(ctx,0x8c61e7c4u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e7c6; /* fmov.s @(r0,r12),fr3 */
L8c61e7c6: sa_motion_publish_stop(ctx,0x8c61e7c6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e7c8; /* fadd fr3,fr2 */
L8c61e7c8: sa_motion_publish_stop(ctx,0x8c61e7c8u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e7ca; /* fmov.s @(r0,r13),fr3 */
L8c61e7ca: sa_motion_publish_stop(ctx,0x8c61e7cau); r[0]=16u;goto L8c61e7cc; /* mov #0x10,r0 */
L8c61e7cc: sa_motion_publish_stop(ctx,0x8c61e7ccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e7ce; /* fadd fr3,fr2 */
L8c61e7ce: sa_motion_publish_stop(ctx,0x8c61e7ceu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e7d0; /* fmov fr2,@(r0,r15) */
L8c61e7d0: sa_motion_publish_stop(ctx,0x8c61e7d0u); r[0]=8u;goto L8c61e7d2; /* mov #0x8,r0 */
L8c61e7d2: sa_motion_publish_stop(ctx,0x8c61e7d2u); fload(m,bus,&io_status,f,2u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e7d4; /* fmov.s @(r0,r14),fr2 */
L8c61e7d4: sa_motion_publish_stop(ctx,0x8c61e7d4u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61e7d6; /* fmov.s @(r0,r12),fr3 */
L8c61e7d6: sa_motion_publish_stop(ctx,0x8c61e7d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e7d8; /* fadd fr3,fr2 */
L8c61e7d8: sa_motion_publish_stop(ctx,0x8c61e7d8u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e7da; /* fmov.s @(r0,r13),fr3 */
L8c61e7da: sa_motion_publish_stop(ctx,0x8c61e7dau); r[0]=20u;goto L8c61e7dc; /* mov #0x14,r0 */
L8c61e7dc: sa_motion_publish_stop(ctx,0x8c61e7dcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e7de; /* fadd fr3,fr2 */
L8c61e7de: sa_motion_publish_stop(ctx,0x8c61e7deu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e7e0; /* fmov fr2,@(r0,r15) */
L8c61e7e0: sa_motion_publish_stop(ctx,0x8c61e7e2u); target=r[3];*pr=0x8c61e7e4u;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61e7e4; /* jsr @r3 */
L8c61e7e4: sa_motion_publish_stop(ctx,0x8c61e7e4u); r[3]=rd(m,bus,&io_status,0x8c61e848u,4);if(io_status)return io_status;goto L8c61e7e6; /* mov.l 0x8c61e848,r3 */
L8c61e7e6: sa_motion_publish_stop(ctx,0x8c61e7e6u); r[0]=12u;goto L8c61e7e8; /* mov #0xc,r0 */
L8c61e7e8: sa_motion_publish_stop(ctx,0x8c61e7e8u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e7ea; /* fmov.s @(r0,r15),fr3 */
L8c61e7ea: sa_motion_publish_stop(ctx,0x8c61e7eau); r[0]=16u;goto L8c61e7ec; /* mov #0x10,r0 */
L8c61e7ec: sa_motion_publish_stop(ctx,0x8c61e7ecu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61e7ee; /* fmov.s @r3,fr2 */
L8c61e7ee: sa_motion_publish_stop(ctx,0x8c61e7eeu); r[3]+=8u;goto L8c61e7f0; /* add #0x8,r3 */
L8c61e7f0: sa_motion_publish_stop(ctx,0x8c61e7f0u); r[2]=rd(m,bus,&io_status,0x8c61e84cu,4);if(io_status)return io_status;goto L8c61e7f2; /* mov.l 0x8c61e84c,r2 */
L8c61e7f2: sa_motion_publish_stop(ctx,0x8c61e7f2u); r[5]=r[12];goto L8c61e7f4; /* mov r12,r5 */
L8c61e7f4: sa_motion_publish_stop(ctx,0x8c61e7f4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e7f6; /* fmul fr3,fr2 */
L8c61e7f6: sa_motion_publish_stop(ctx,0x8c61e7f6u); r[1]=rd(m,bus,&io_status,0x8c61e850u,4);if(io_status)return io_status;goto L8c61e7f8; /* mov.l 0x8c61e850,r1 */
L8c61e7f8: sa_motion_publish_stop(ctx,0x8c61e7f8u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61e7fa; /* fmov.s @r2,fr3 */
L8c61e7fa: sa_motion_publish_stop(ctx,0x8c61e7fau); r[6]=r[13];goto L8c61e7fc; /* mov r13,r6 */
L8c61e7fc: sa_motion_publish_stop(ctx,0x8c61e7fcu); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61e7fe; /* fmov.s @r1,fr1 */
L8c61e7fe: sa_motion_publish_stop(ctx,0x8c61e7feu); r[2]=rd(m,bus,&io_status,0x8c61e868u,4);if(io_status)return io_status;goto L8c61e800; /* mov.l 0x8c61e868,r2 */
L8c61e800: sa_motion_publish_stop(ctx,0x8c61e800u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e802; /* fsub fr2,fr3 */
L8c61e802: sa_motion_publish_stop(ctx,0x8c61e802u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e804; /* fmov.s @(r0,r15),fr2 */
L8c61e804: sa_motion_publish_stop(ctx,0x8c61e804u); r[0]=20u;goto L8c61e806; /* mov #0x14,r0 */
L8c61e806: sa_motion_publish_stop(ctx,0x8c61e806u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e808; /* fmul fr2,fr1 */
L8c61e808: sa_motion_publish_stop(ctx,0x8c61e808u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e80a; /* fmov.s @(r0,r15),fr2 */
L8c61e80a: sa_motion_publish_stop(ctx,0x8c61e80au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e80c; /* fsub fr1,fr3 */
L8c61e80c: sa_motion_publish_stop(ctx,0x8c61e80cu); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61e80e; /* fmov.s @r3,fr1 */
L8c61e80e: sa_motion_publish_stop(ctx,0x8c61e80eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e810; /* fmul fr2,fr1 */
L8c61e810: sa_motion_publish_stop(ctx,0x8c61e810u); fcopy(f,4u,3u);goto L8c61e812; /* fmov fr3,fr4 */
L8c61e812: sa_motion_publish_stop(ctx,0x8c61e812u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e814; /* fsub fr1,fr4 */
L8c61e814: sa_motion_publish_stop(ctx,0x8c61e816u); target=r[2];*pr=0x8c61e818u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61e818; /* jsr @r2 */
L8c61e818: sa_motion_publish_stop(ctx,0x8c61e818u); r[11]+=1u;goto L8c61e81a; /* add #0x1,r11 */
L8c61e81a: sa_motion_publish_stop(ctx,0x8c61e81au); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61e81c; /* mov.l @r15,r3 */
L8c61e81c: sa_motion_publish_stop(ctx,0x8c61e81cu); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61e81e; /* mov.w @(0x2,r3),r0 */
L8c61e81e: sa_motion_publish_stop(ctx,0x8c61e81eu); r[0]=r[0]&65535u;goto L8c61e820; /* extu.w r0,r0 */
L8c61e820: sa_motion_publish_stop(ctx,0x8c61e820u); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[0]);goto L8c61e822; /* cmp/ge r0,r11 */
L8c61e822: sa_motion_publish_stop(ctx,0x8c61e822u); if((*sr&1u))goto L8c61e828;goto L8c61e824; /* bt 0x8c61e828 */
L8c61e824: sa_motion_publish_stop(ctx,0x8c61e826u); goto L8c61e570; /* bra 0x8c61e570 */
L8c61e828: sa_motion_publish_stop(ctx,0x8c61e82au); goto L8c61f0bc; /* bra 0x8c61f0bc */
L8c61e82c: sa_motion_publish_stop(ctx,0x8c61e82cu); r[1]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61e82e; /* mov.l @r15,r1 */
L8c61e82e: sa_motion_publish_stop(ctx,0x8c61e82eu); r[2]=rd(m,bus,&io_status,0x8c61e86cu,4);if(io_status)return io_status;goto L8c61e830; /* mov.l 0x8c61e86c,r2 */
L8c61e830: sa_motion_publish_stop(ctx,0x8c61e830u); r[0]=sx(m,bus,&io_status,r[1]+0x2u,2);if(io_status)return io_status;goto L8c61e832; /* mov.w @(0x2,r1),r0 */
L8c61e832: sa_motion_publish_stop(ctx,0x8c61e832u); r[1]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e834; /* mov.l @r2,r1 */
L8c61e834: sa_motion_publish_stop(ctx,0x8c61e834u); r[0]=r[0]&65535u;goto L8c61e836; /* extu.w r0,r0 */
L8c61e836: sa_motion_publish_stop(ctx,0x8c61e836u); r[0]<<=2;goto L8c61e838; /* shll2 r0 */
L8c61e838: sa_motion_publish_stop(ctx,0x8c61e838u); r[1]+=r[0];goto L8c61e83a; /* add r0,r1 */
L8c61e83a: sa_motion_publish_stop(ctx,0x8c61e83au); wr(m,bus,&io_status,r[2],r[1],4);if(io_status)return io_status;goto L8c61e83c; /* mov.l r1,@r2 */
L8c61e83c: sa_motion_publish_stop(ctx,0x8c61e83eu); r[9]=0u;goto L8c61ebf6; /* bra 0x8c61ebf6 */
L8c61e870: sa_motion_publish_stop(ctx,0x8c61e870u); r[11]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e872; /* mov.w @r10+,r11 */
L8c61e872: sa_motion_publish_stop(ctx,0x8c61e872u); r[3]=rd(m,bus,&io_status,0x8c61eb34u,4);if(io_status)return io_status;goto L8c61e874; /* mov.l 0x8c61eb34,r3 */
L8c61e874: sa_motion_publish_stop(ctx,0x8c61e874u); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e876; /* mov.w @r10+,r14 */
L8c61e876: sa_motion_publish_stop(ctx,0x8c61e876u); r[11]=r[11]&65535u;goto L8c61e878; /* extu.w r11,r11 */
L8c61e878: sa_motion_publish_stop(ctx,0x8c61e878u); r[11]<<=2;goto L8c61e87a; /* shll2 r11 */
L8c61e87a: sa_motion_publish_stop(ctx,0x8c61e87au); r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61e87c; /* mov.l @r3,r5 */
L8c61e87c: sa_motion_publish_stop(ctx,0x8c61e87cu); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e87e; /* mov.w @r10+,r13 */
L8c61e87e: sa_motion_publish_stop(ctx,0x8c61e87eu); r[14]=r[14]&65535u;goto L8c61e880; /* extu.w r14,r14 */
L8c61e880: sa_motion_publish_stop(ctx,0x8c61e880u); r[14]<<=2;goto L8c61e882; /* shll2 r14 */
L8c61e882: sa_motion_publish_stop(ctx,0x8c61e882u); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61e884; /* mov.w @r10+,r12 */
L8c61e884: sa_motion_publish_stop(ctx,0x8c61e884u); r[13]=r[13]&65535u;goto L8c61e886; /* extu.w r13,r13 */
L8c61e886: sa_motion_publish_stop(ctx,0x8c61e886u); r[14]<<=2;goto L8c61e888; /* shll2 r14 */
L8c61e888: sa_motion_publish_stop(ctx,0x8c61e888u); r[11]<<=2;goto L8c61e88a; /* shll2 r11 */
L8c61e88a: sa_motion_publish_stop(ctx,0x8c61e88au); r[12]=r[12]&65535u;goto L8c61e88c; /* extu.w r12,r12 */
L8c61e88c: sa_motion_publish_stop(ctx,0x8c61e88cu); r[13]<<=2;goto L8c61e88e; /* shll2 r13 */
L8c61e88e: sa_motion_publish_stop(ctx,0x8c61e88eu); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61e890; /* shll r11 */
L8c61e890: sa_motion_publish_stop(ctx,0x8c61e890u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61e892; /* shll r14 */
L8c61e892: sa_motion_publish_stop(ctx,0x8c61e892u); r[11]+=r[5];goto L8c61e894; /* add r5,r11 */
L8c61e894: sa_motion_publish_stop(ctx,0x8c61e894u); r[12]<<=2;goto L8c61e896; /* shll2 r12 */
L8c61e896: sa_motion_publish_stop(ctx,0x8c61e896u); r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61e898; /* mov.l @(0xc,r11),r4 */
L8c61e898: sa_motion_publish_stop(ctx,0x8c61e898u); r[14]+=r[5];goto L8c61e89a; /* add r5,r14 */
L8c61e89a: sa_motion_publish_stop(ctx,0x8c61e89au); r[13]<<=2;goto L8c61e89c; /* shll2 r13 */
L8c61e89c: sa_motion_publish_stop(ctx,0x8c61e89cu); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61e89e; /* mov.l @(0xc,r14),r2 */
L8c61e89e: sa_motion_publish_stop(ctx,0x8c61e89eu); r[12]<<=2;goto L8c61e8a0; /* shll2 r12 */
L8c61e8a0: sa_motion_publish_stop(ctx,0x8c61e8a0u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61e8a2; /* shll r13 */
L8c61e8a2: sa_motion_publish_stop(ctx,0x8c61e8a2u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61e8a4; /* shll r12 */
L8c61e8a4: sa_motion_publish_stop(ctx,0x8c61e8a4u); r[13]+=r[5];goto L8c61e8a6; /* add r5,r13 */
L8c61e8a6: sa_motion_publish_stop(ctx,0x8c61e8a6u); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61e8a8; /* mov.l @(0xc,r13),r1 */
L8c61e8a8: sa_motion_publish_stop(ctx,0x8c61e8a8u); r[4]&=r[2];goto L8c61e8aa; /* and r2,r4 */
L8c61e8aa: sa_motion_publish_stop(ctx,0x8c61e8aau); r[12]+=r[5];goto L8c61e8ac; /* add r5,r12 */
L8c61e8ac: sa_motion_publish_stop(ctx,0x8c61e8acu); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61e8ae; /* mov.l @(0xc,r12),r2 */
L8c61e8ae: sa_motion_publish_stop(ctx,0x8c61e8aeu); r[4]&=r[1];goto L8c61e8b0; /* and r1,r4 */
L8c61e8b0: sa_motion_publish_stop(ctx,0x8c61e8b0u); r[1]=63u;goto L8c61e8b2; /* mov #0x3f,r1 */
L8c61e8b2: sa_motion_publish_stop(ctx,0x8c61e8b2u); r[4]&=r[2];goto L8c61e8b4; /* and r2,r4 */
L8c61e8b4: sa_motion_publish_stop(ctx,0x8c61e8b4u); *sr=(*sr&~1u)|!(r[1]&r[4]);goto L8c61e8b6; /* tst r1,r4 */
L8c61e8b6: sa_motion_publish_stop(ctx,0x8c61e8b6u); if((*sr&1u))goto L8c61e8bc;goto L8c61e8b8; /* bt 0x8c61e8bc */
L8c61e8b8: sa_motion_publish_stop(ctx,0x8c61e8bau); goto L8c61ebf4; /* bra 0x8c61ebf4 */
L8c61e8bc: sa_motion_publish_stop(ctx,0x8c61e8bcu); r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61e8be; /* mov.l @(0xc,r11),r4 */
L8c61e8be: sa_motion_publish_stop(ctx,0x8c61e8beu); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61e8c0; /* mov.l @(0xc,r14),r3 */
L8c61e8c0: sa_motion_publish_stop(ctx,0x8c61e8c0u); r[2]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61e8c2; /* mov.l @(0xc,r13),r2 */
L8c61e8c2: sa_motion_publish_stop(ctx,0x8c61e8c2u); r[4]|=r[3];goto L8c61e8c4; /* or r3,r4 */
L8c61e8c4: sa_motion_publish_stop(ctx,0x8c61e8c4u); r[3]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61e8c6; /* mov.l @(0xc,r12),r3 */
L8c61e8c6: sa_motion_publish_stop(ctx,0x8c61e8c6u); r[4]|=r[2];goto L8c61e8c8; /* or r2,r4 */
L8c61e8c8: sa_motion_publish_stop(ctx,0x8c61e8c8u); r[2]=1u;goto L8c61e8ca; /* mov #0x1,r2 */
L8c61e8ca: sa_motion_publish_stop(ctx,0x8c61e8cau); r[4]|=r[3];goto L8c61e8cc; /* or r3,r4 */
L8c61e8cc: sa_motion_publish_stop(ctx,0x8c61e8ccu); *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61e8ce; /* tst r2,r4 */
L8c61e8ce: sa_motion_publish_stop(ctx,0x8c61e8ceu); if((*sr&1u))goto L8c61e8d4;goto L8c61e8d0; /* bt 0x8c61e8d4 */
L8c61e8d0: sa_motion_publish_stop(ctx,0x8c61e8d2u); goto L8c61eb58; /* bra 0x8c61eb58 */
L8c61e8d4: sa_motion_publish_stop(ctx,0x8c61e8d4u); r[0]=16u;goto L8c61e8d6; /* mov #0x10,r0 */
L8c61e8d6: sa_motion_publish_stop(ctx,0x8c61e8d6u); fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e8d8; /* fmov.s @(r0,r14),fr5 */
L8c61e8d8: sa_motion_publish_stop(ctx,0x8c61e8d8u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e8da; /* fmov.s @(r0,r11),fr3 */
L8c61e8da: sa_motion_publish_stop(ctx,0x8c61e8dau); r[0]=20u;goto L8c61e8dc; /* mov #0x14,r0 */
L8c61e8dc: sa_motion_publish_stop(ctx,0x8c61e8dcu); fcopy(f,2u,5u);goto L8c61e8de; /* fmov fr5,fr2 */
L8c61e8de: sa_motion_publish_stop(ctx,0x8c61e8deu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61e8e0; /* fsub fr3,fr2 */
L8c61e8e0: sa_motion_publish_stop(ctx,0x8c61e8e0u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e8e2; /* fmov.s @(r0,r13),fr3 */
L8c61e8e2: sa_motion_publish_stop(ctx,0x8c61e8e2u); fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e8e4; /* fmov.s @(r0,r14),fr4 */
L8c61e8e4: sa_motion_publish_stop(ctx,0x8c61e8e4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e8e6; /* fsub fr4,fr3 */
L8c61e8e6: sa_motion_publish_stop(ctx,0x8c61e8e6u); fcopy(f,1u,4u);goto L8c61e8e8; /* fmov fr4,fr1 */
L8c61e8e8: sa_motion_publish_stop(ctx,0x8c61e8e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e8ea; /* fmul fr3,fr2 */
L8c61e8ea: sa_motion_publish_stop(ctx,0x8c61e8eau); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e8ec; /* fmov.s @(r0,r11),fr3 */
L8c61e8ec: sa_motion_publish_stop(ctx,0x8c61e8ecu); r[0]=16u;goto L8c61e8ee; /* mov #0x10,r0 */
L8c61e8ee: sa_motion_publish_stop(ctx,0x8c61e8eeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61e8f0; /* fsub fr3,fr1 */
L8c61e8f0: sa_motion_publish_stop(ctx,0x8c61e8f0u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e8f2; /* fmov.s @(r0,r13),fr3 */
L8c61e8f2: sa_motion_publish_stop(ctx,0x8c61e8f2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e8f4; /* fsub fr5,fr3 */
L8c61e8f4: sa_motion_publish_stop(ctx,0x8c61e8f4u); fcopy(f,4u,2u);goto L8c61e8f6; /* fmov fr2,fr4 */
L8c61e8f6: sa_motion_publish_stop(ctx,0x8c61e8f6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e8f8; /* fmul fr3,fr1 */
L8c61e8f8: sa_motion_publish_stop(ctx,0x8c61e8f8u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61e8fa; /* fldi0 fr3 */
L8c61e8fa: sa_motion_publish_stop(ctx,0x8c61e8fau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e8fc; /* fsub fr1,fr4 */
L8c61e8fc: sa_motion_publish_stop(ctx,0x8c61e8fcu); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61e8fe; /* fcmp/gt fr3,fr4 */
L8c61e8fe: sa_motion_publish_stop(ctx,0x8c61e8feu); if(!(*sr&1u))goto L8c61e904;goto L8c61e900; /* bf 0x8c61e904 */
L8c61e900: sa_motion_publish_stop(ctx,0x8c61e902u); goto L8c61ebf4; /* bra 0x8c61ebf4 */
L8c61e904: sa_motion_publish_stop(ctx,0x8c61e904u); fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61e906; /* fmov.s @r11,fr2 */
L8c61e906: sa_motion_publish_stop(ctx,0x8c61e906u); r[0]=12u;goto L8c61e908; /* mov #0xc,r0 */
L8c61e908: sa_motion_publish_stop(ctx,0x8c61e908u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61e90a; /* fmov.s @r14,fr3 */
L8c61e90a: sa_motion_publish_stop(ctx,0x8c61e90au); r[4]=r[15];goto L8c61e90c; /* mov r15,r4 */
L8c61e90c: sa_motion_publish_stop(ctx,0x8c61e90cu); r[3]=rd(m,bus,&io_status,0x8c61eb38u,4);if(io_status)return io_status;goto L8c61e90e; /* mov.l 0x8c61eb38,r3 */
L8c61e90e: sa_motion_publish_stop(ctx,0x8c61e90eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e910; /* fadd fr3,fr2 */
L8c61e910: sa_motion_publish_stop(ctx,0x8c61e910u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61e912; /* fmov.s @r13,fr3 */
L8c61e912: sa_motion_publish_stop(ctx,0x8c61e912u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e914; /* fadd fr3,fr2 */
L8c61e914: sa_motion_publish_stop(ctx,0x8c61e914u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e916; /* fmov fr2,@(r0,r15) */
L8c61e916: sa_motion_publish_stop(ctx,0x8c61e916u); r[0]=4u;goto L8c61e918; /* mov #0x4,r0 */
L8c61e918: sa_motion_publish_stop(ctx,0x8c61e918u); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e91a; /* fmov.s @(r0,r11),fr2 */
L8c61e91a: sa_motion_publish_stop(ctx,0x8c61e91au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e91c; /* fmov.s @(r0,r14),fr3 */
L8c61e91c: sa_motion_publish_stop(ctx,0x8c61e91cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e91e; /* fadd fr3,fr2 */
L8c61e91e: sa_motion_publish_stop(ctx,0x8c61e91eu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e920; /* fmov.s @(r0,r13),fr3 */
L8c61e920: sa_motion_publish_stop(ctx,0x8c61e920u); r[0]=16u;goto L8c61e922; /* mov #0x10,r0 */
L8c61e922: sa_motion_publish_stop(ctx,0x8c61e922u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e924; /* fadd fr3,fr2 */
L8c61e924: sa_motion_publish_stop(ctx,0x8c61e924u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e926; /* fmov fr2,@(r0,r15) */
L8c61e926: sa_motion_publish_stop(ctx,0x8c61e926u); r[0]=8u;goto L8c61e928; /* mov #0x8,r0 */
L8c61e928: sa_motion_publish_stop(ctx,0x8c61e928u); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e92a; /* fmov.s @(r0,r11),fr2 */
L8c61e92a: sa_motion_publish_stop(ctx,0x8c61e92au); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e92c; /* fmov.s @(r0,r14),fr3 */
L8c61e92c: sa_motion_publish_stop(ctx,0x8c61e92cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e92e; /* fadd fr3,fr2 */
L8c61e92e: sa_motion_publish_stop(ctx,0x8c61e92eu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61e930; /* fmov.s @(r0,r13),fr3 */
L8c61e930: sa_motion_publish_stop(ctx,0x8c61e930u); r[0]=20u;goto L8c61e932; /* mov #0x14,r0 */
L8c61e932: sa_motion_publish_stop(ctx,0x8c61e932u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61e934; /* fadd fr3,fr2 */
L8c61e934: sa_motion_publish_stop(ctx,0x8c61e934u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e936; /* fmov fr2,@(r0,r15) */
L8c61e936: sa_motion_publish_stop(ctx,0x8c61e938u); target=r[3];*pr=0x8c61e93au;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61e93a; /* jsr @r3 */
L8c61e93a: sa_motion_publish_stop(ctx,0x8c61e93au); r[3]=rd(m,bus,&io_status,0x8c61eb3cu,4);if(io_status)return io_status;goto L8c61e93c; /* mov.l 0x8c61eb3c,r3 */
L8c61e93c: sa_motion_publish_stop(ctx,0x8c61e93cu); r[0]=12u;goto L8c61e93e; /* mov #0xc,r0 */
L8c61e93e: sa_motion_publish_stop(ctx,0x8c61e93eu); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e940; /* fmov.s @(r0,r15),fr3 */
L8c61e940: sa_motion_publish_stop(ctx,0x8c61e940u); r[0]=16u;goto L8c61e942; /* mov #0x10,r0 */
L8c61e942: sa_motion_publish_stop(ctx,0x8c61e942u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61e944; /* fmov.s @r3,fr2 */
L8c61e944: sa_motion_publish_stop(ctx,0x8c61e944u); r[3]+=8u;goto L8c61e946; /* add #0x8,r3 */
L8c61e946: sa_motion_publish_stop(ctx,0x8c61e946u); r[2]=rd(m,bus,&io_status,0x8c61eb40u,4);if(io_status)return io_status;goto L8c61e948; /* mov.l 0x8c61eb40,r2 */
L8c61e948: sa_motion_publish_stop(ctx,0x8c61e948u); r[8]+=24u;goto L8c61e94a; /* add #0x18,r8 */
L8c61e94a: sa_motion_publish_stop(ctx,0x8c61e94au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e94c; /* fmul fr3,fr2 */
L8c61e94c: sa_motion_publish_stop(ctx,0x8c61e94cu); r[1]=rd(m,bus,&io_status,0x8c61eb44u,4);if(io_status)return io_status;goto L8c61e94e; /* mov.l 0x8c61eb44,r1 */
L8c61e94e: sa_motion_publish_stop(ctx,0x8c61e94eu); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61e950; /* fmov.s @r2,fr3 */
L8c61e950: sa_motion_publish_stop(ctx,0x8c61e950u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61e952; /* fmov.s @r1,fr1 */
L8c61e952: sa_motion_publish_stop(ctx,0x8c61e952u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e954; /* fsub fr2,fr3 */
L8c61e954: sa_motion_publish_stop(ctx,0x8c61e954u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e956; /* fmov.s @(r0,r15),fr2 */
L8c61e956: sa_motion_publish_stop(ctx,0x8c61e956u); r[0]=20u;goto L8c61e958; /* mov #0x14,r0 */
L8c61e958: sa_motion_publish_stop(ctx,0x8c61e958u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e95a; /* fmul fr2,fr1 */
L8c61e95a: sa_motion_publish_stop(ctx,0x8c61e95au); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61e95c; /* fmov.s @(r0,r15),fr2 */
L8c61e95c: sa_motion_publish_stop(ctx,0x8c61e95cu); r[0]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61e95e; /* mov.l 0x8c61eb48,r0 */
L8c61e95e: sa_motion_publish_stop(ctx,0x8c61e95eu); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61e960; /* mov.l @r0,r2 */
L8c61e960: sa_motion_publish_stop(ctx,0x8c61e960u); r[0]=4u;goto L8c61e962; /* mov #0x4,r0 */
L8c61e962: sa_motion_publish_stop(ctx,0x8c61e962u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61e964; /* fsub fr1,fr3 */
L8c61e964: sa_motion_publish_stop(ctx,0x8c61e964u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61e966; /* fmov.s @r3,fr1 */
L8c61e966: sa_motion_publish_stop(ctx,0x8c61e966u); fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e968; /* fmov.s @(r0,r11),fr0 */
L8c61e968: sa_motion_publish_stop(ctx,0x8c61e968u); r[1]=r[2];goto L8c61e96a; /* mov r2,r1 */
L8c61e96a: sa_motion_publish_stop(ctx,0x8c61e96au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61e96c; /* fmul fr2,fr1 */
L8c61e96c: sa_motion_publish_stop(ctx,0x8c61e96cu); r[0]=20u;goto L8c61e96e; /* mov #0x14,r0 */
L8c61e96e: sa_motion_publish_stop(ctx,0x8c61e96eu); r[3]=r[2];goto L8c61e970; /* mov r2,r3 */
L8c61e970: sa_motion_publish_stop(ctx,0x8c61e970u); fcopy(f,4u,3u);goto L8c61e972; /* fmov fr3,fr4 */
L8c61e972: sa_motion_publish_stop(ctx,0x8c61e972u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e974; /* fmov.s @(r0,r1),fr3 */
L8c61e974: sa_motion_publish_stop(ctx,0x8c61e974u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61e976; /* fsub fr1,fr4 */
L8c61e976: sa_motion_publish_stop(ctx,0x8c61e976u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e978; /* fmul fr0,fr3 */
L8c61e978: sa_motion_publish_stop(ctx,0x8c61e978u); fload(m,bus,&io_status,f,0u,r[11]);if(io_status)return io_status;goto L8c61e97a; /* fmov.s @r11,fr0 */
L8c61e97a: sa_motion_publish_stop(ctx,0x8c61e97au); r[0]=4u;goto L8c61e97c; /* mov #0x4,r0 */
L8c61e97c: sa_motion_publish_stop(ctx,0x8c61e97cu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e97e; /* fmov.s @(r0,r3),fr2 */
L8c61e97e: sa_motion_publish_stop(ctx,0x8c61e97eu); r[0]=8u;goto L8c61e980; /* mov #0x8,r0 */
L8c61e980: sa_motion_publish_stop(ctx,0x8c61e980u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e982; /* fmac fr0,fr2,fr3 */
L8c61e982: sa_motion_publish_stop(ctx,0x8c61e982u); fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e984; /* fmov.s @(r0,r11),fr0 */
L8c61e984: sa_motion_publish_stop(ctx,0x8c61e984u); r[0]=36u;goto L8c61e986; /* mov #0x24,r0 */
L8c61e986: sa_motion_publish_stop(ctx,0x8c61e986u); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e988; /* fmov.s @(r0,r2),fr2 */
L8c61e988: sa_motion_publish_stop(ctx,0x8c61e988u); r[0]=4u;goto L8c61e98a; /* mov #0x4,r0 */
L8c61e98a: sa_motion_publish_stop(ctx,0x8c61e98au); r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61e98c; /* mov.l 0x8c61eb48,r2 */
L8c61e98c: sa_motion_publish_stop(ctx,0x8c61e98cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e98e; /* fmac fr0,fr2,fr3 */
L8c61e98e: sa_motion_publish_stop(ctx,0x8c61e98eu); fcopy(f,0u,14u);goto L8c61e990; /* fmov fr14,fr0 */
L8c61e990: sa_motion_publish_stop(ctx,0x8c61e990u); fcopy(f,2u,14u);goto L8c61e992; /* fmov fr14,fr2 */
L8c61e992: sa_motion_publish_stop(ctx,0x8c61e992u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e994; /* fmac fr0,fr3,fr2 */
L8c61e994: sa_motion_publish_stop(ctx,0x8c61e994u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e996; /* fmov.s fr2,@-r8 */
L8c61e996: sa_motion_publish_stop(ctx,0x8c61e996u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e998; /* mov.l @r2,r3 */
L8c61e998: sa_motion_publish_stop(ctx,0x8c61e998u); fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e99a; /* fmov.s @(r0,r11),fr0 */
L8c61e99a: sa_motion_publish_stop(ctx,0x8c61e99au); r[0]=16u;goto L8c61e99c; /* mov #0x10,r0 */
L8c61e99c: sa_motion_publish_stop(ctx,0x8c61e99cu); r[2]=r[3];goto L8c61e99e; /* mov r3,r2 */
L8c61e99e: sa_motion_publish_stop(ctx,0x8c61e99eu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e9a0; /* fmov.s @(r0,r2),fr3 */
L8c61e9a0: sa_motion_publish_stop(ctx,0x8c61e9a0u); r[1]=r[3];goto L8c61e9a2; /* mov r3,r1 */
L8c61e9a2: sa_motion_publish_stop(ctx,0x8c61e9a2u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61e9a4; /* fmov.s @r1,fr2 */
L8c61e9a4: sa_motion_publish_stop(ctx,0x8c61e9a4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e9a6; /* fmul fr0,fr3 */
L8c61e9a6: sa_motion_publish_stop(ctx,0x8c61e9a6u); fload(m,bus,&io_status,f,0u,r[11]);if(io_status)return io_status;goto L8c61e9a8; /* fmov.s @r11,fr0 */
L8c61e9a8: sa_motion_publish_stop(ctx,0x8c61e9a8u); r[0]=8u;goto L8c61e9aa; /* mov #0x8,r0 */
L8c61e9aa: sa_motion_publish_stop(ctx,0x8c61e9aau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e9ac; /* fmac fr0,fr2,fr3 */
L8c61e9ac: sa_motion_publish_stop(ctx,0x8c61e9acu); fload(m,bus,&io_status,f,0u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e9ae; /* fmov.s @(r0,r11),fr0 */
L8c61e9ae: sa_motion_publish_stop(ctx,0x8c61e9aeu); r[0]=32u;goto L8c61e9b0; /* mov #0x20,r0 */
L8c61e9b0: sa_motion_publish_stop(ctx,0x8c61e9b0u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e9b2; /* fmov.s @(r0,r3),fr2 */
L8c61e9b2: sa_motion_publish_stop(ctx,0x8c61e9b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e9b4; /* fmac fr0,fr2,fr3 */
L8c61e9b4: sa_motion_publish_stop(ctx,0x8c61e9b4u); fcopy(f,0u,14u);goto L8c61e9b6; /* fmov fr14,fr0 */
L8c61e9b6: sa_motion_publish_stop(ctx,0x8c61e9b6u); fcopy(f,2u,14u);goto L8c61e9b8; /* fmov fr14,fr2 */
L8c61e9b8: sa_motion_publish_stop(ctx,0x8c61e9b8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61e9ba; /* fmac fr0,fr3,fr2 */
L8c61e9ba: sa_motion_publish_stop(ctx,0x8c61e9bau); r[0]=24u;goto L8c61e9bc; /* mov #0x18,r0 */
L8c61e9bc: sa_motion_publish_stop(ctx,0x8c61e9bcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61e9be; /* fmov.s fr2,@-r8 */
L8c61e9be: sa_motion_publish_stop(ctx,0x8c61e9beu); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e9c0; /* fmov.s @(r0,r11),fr3 */
L8c61e9c0: sa_motion_publish_stop(ctx,0x8c61e9c0u); r[0]=20u;goto L8c61e9c2; /* mov #0x14,r0 */
L8c61e9c2: sa_motion_publish_stop(ctx,0x8c61e9c2u); r[3]=rd(m,bus,&io_status,0x8c61eb4cu,4);if(io_status)return io_status;goto L8c61e9c4; /* mov.l 0x8c61eb4c,r3 */
L8c61e9c4: sa_motion_publish_stop(ctx,0x8c61e9c4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e9c6; /* fmov.s fr3,@-r8 */
L8c61e9c6: sa_motion_publish_stop(ctx,0x8c61e9c6u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e9c8; /* fmov.s @(r0,r11),fr3 */
L8c61e9c8: sa_motion_publish_stop(ctx,0x8c61e9c8u); r[0]=16u;goto L8c61e9ca; /* mov #0x10,r0 */
L8c61e9ca: sa_motion_publish_stop(ctx,0x8c61e9cau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e9cc; /* fmov.s fr3,@-r8 */
L8c61e9cc: sa_motion_publish_stop(ctx,0x8c61e9ccu); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61e9ce; /* fmov.s @(r0,r11),fr3 */
L8c61e9ce: sa_motion_publish_stop(ctx,0x8c61e9ceu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61e9d0; /* fmov.s fr3,@-r8 */
L8c61e9d0: sa_motion_publish_stop(ctx,0x8c61e9d0u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61e9d2; /* mov.l r3,@-r8 */
L8c61e9d2: sa_motion_publish_stop(ctx,0x8c61e9d2u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61e9d4; /* pref @r8 */
L8c61e9d4: sa_motion_publish_stop(ctx,0x8c61e9d4u); r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61e9d6; /* mov.l 0x8c61eb48,r2 */
L8c61e9d6: sa_motion_publish_stop(ctx,0x8c61e9d6u); r[0]=4u;goto L8c61e9d8; /* mov #0x4,r0 */
L8c61e9d8: sa_motion_publish_stop(ctx,0x8c61e9d8u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e9da; /* fmov.s @(r0,r14),fr0 */
L8c61e9da: sa_motion_publish_stop(ctx,0x8c61e9dau); r[0]=20u;goto L8c61e9dc; /* mov #0x14,r0 */
L8c61e9dc: sa_motion_publish_stop(ctx,0x8c61e9dcu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61e9de; /* mov.l @r2,r3 */
L8c61e9de: sa_motion_publish_stop(ctx,0x8c61e9deu); r[8]+=56u;goto L8c61e9e0; /* add #0x38,r8 */
L8c61e9e0: sa_motion_publish_stop(ctx,0x8c61e9e0u); r[2]=r[3];goto L8c61e9e2; /* mov r3,r2 */
L8c61e9e2: sa_motion_publish_stop(ctx,0x8c61e9e2u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61e9e4; /* fmov.s @(r0,r2),fr3 */
L8c61e9e4: sa_motion_publish_stop(ctx,0x8c61e9e4u); r[0]=4u;goto L8c61e9e6; /* mov #0x4,r0 */
L8c61e9e6: sa_motion_publish_stop(ctx,0x8c61e9e6u); r[1]=r[3];goto L8c61e9e8; /* mov r3,r1 */
L8c61e9e8: sa_motion_publish_stop(ctx,0x8c61e9e8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e9ea; /* fmul fr0,fr3 */
L8c61e9ea: sa_motion_publish_stop(ctx,0x8c61e9eau); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61e9ec; /* fmov.s @(r0,r1),fr2 */
L8c61e9ec: sa_motion_publish_stop(ctx,0x8c61e9ecu); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61e9ee; /* fmov.s @r14,fr0 */
L8c61e9ee: sa_motion_publish_stop(ctx,0x8c61e9eeu); r[0]=8u;goto L8c61e9f0; /* mov #0x8,r0 */
L8c61e9f0: sa_motion_publish_stop(ctx,0x8c61e9f0u); r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61e9f2; /* mov.l 0x8c61eb48,r2 */
L8c61e9f2: sa_motion_publish_stop(ctx,0x8c61e9f2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e9f4; /* fmac fr0,fr2,fr3 */
L8c61e9f4: sa_motion_publish_stop(ctx,0x8c61e9f4u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61e9f6; /* fmov.s @(r0,r14),fr0 */
L8c61e9f6: sa_motion_publish_stop(ctx,0x8c61e9f6u); r[0]=36u;goto L8c61e9f8; /* mov #0x24,r0 */
L8c61e9f8: sa_motion_publish_stop(ctx,0x8c61e9f8u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61e9fa; /* fmov.s @(r0,r3),fr2 */
L8c61e9fa: sa_motion_publish_stop(ctx,0x8c61e9fau); r[0]=4u;goto L8c61e9fc; /* mov #0x4,r0 */
L8c61e9fc: sa_motion_publish_stop(ctx,0x8c61e9fcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61e9fe; /* fmac fr0,fr2,fr3 */
L8c61e9fe: sa_motion_publish_stop(ctx,0x8c61e9feu); fcopy(f,0u,14u);goto L8c61ea00; /* fmov fr14,fr0 */
L8c61ea00: sa_motion_publish_stop(ctx,0x8c61ea00u); fcopy(f,2u,14u);goto L8c61ea02; /* fmov fr14,fr2 */
L8c61ea02: sa_motion_publish_stop(ctx,0x8c61ea02u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ea04; /* fmac fr0,fr3,fr2 */
L8c61ea04: sa_motion_publish_stop(ctx,0x8c61ea04u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ea06; /* fmov.s fr2,@-r8 */
L8c61ea06: sa_motion_publish_stop(ctx,0x8c61ea06u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ea08; /* mov.l @r2,r3 */
L8c61ea08: sa_motion_publish_stop(ctx,0x8c61ea08u); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ea0a; /* fmov.s @(r0,r14),fr0 */
L8c61ea0a: sa_motion_publish_stop(ctx,0x8c61ea0au); r[0]=16u;goto L8c61ea0c; /* mov #0x10,r0 */
L8c61ea0c: sa_motion_publish_stop(ctx,0x8c61ea0cu); r[2]=r[3];goto L8c61ea0e; /* mov r3,r2 */
L8c61ea0e: sa_motion_publish_stop(ctx,0x8c61ea0eu); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61ea10; /* fmov.s @(r0,r2),fr3 */
L8c61ea10: sa_motion_publish_stop(ctx,0x8c61ea10u); r[1]=r[3];goto L8c61ea12; /* mov r3,r1 */
L8c61ea12: sa_motion_publish_stop(ctx,0x8c61ea12u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea14; /* fmul fr0,fr3 */
L8c61ea14: sa_motion_publish_stop(ctx,0x8c61ea14u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61ea16; /* fmov.s @r1,fr2 */
L8c61ea16: sa_motion_publish_stop(ctx,0x8c61ea16u); r[0]=8u;goto L8c61ea18; /* mov #0x8,r0 */
L8c61ea18: sa_motion_publish_stop(ctx,0x8c61ea18u); fload(m,bus,&io_status,f,0u,r[14]);if(io_status)return io_status;goto L8c61ea1a; /* fmov.s @r14,fr0 */
L8c61ea1a: sa_motion_publish_stop(ctx,0x8c61ea1au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea1c; /* fmac fr0,fr2,fr3 */
L8c61ea1c: sa_motion_publish_stop(ctx,0x8c61ea1cu); fload(m,bus,&io_status,f,0u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ea1e; /* fmov.s @(r0,r14),fr0 */
L8c61ea1e: sa_motion_publish_stop(ctx,0x8c61ea1eu); r[0]=32u;goto L8c61ea20; /* mov #0x20,r0 */
L8c61ea20: sa_motion_publish_stop(ctx,0x8c61ea20u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61ea22; /* fmov.s @(r0,r3),fr2 */
L8c61ea22: sa_motion_publish_stop(ctx,0x8c61ea22u); r[0]=24u;goto L8c61ea24; /* mov #0x18,r0 */
L8c61ea24: sa_motion_publish_stop(ctx,0x8c61ea24u); r[3]=rd(m,bus,&io_status,0x8c61eb4cu,4);if(io_status)return io_status;goto L8c61ea26; /* mov.l 0x8c61eb4c,r3 */
L8c61ea26: sa_motion_publish_stop(ctx,0x8c61ea26u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea28; /* fmac fr0,fr2,fr3 */
L8c61ea28: sa_motion_publish_stop(ctx,0x8c61ea28u); fcopy(f,0u,14u);goto L8c61ea2a; /* fmov fr14,fr0 */
L8c61ea2a: sa_motion_publish_stop(ctx,0x8c61ea2au); fcopy(f,2u,14u);goto L8c61ea2c; /* fmov fr14,fr2 */
L8c61ea2c: sa_motion_publish_stop(ctx,0x8c61ea2cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ea2e; /* fmac fr0,fr3,fr2 */
L8c61ea2e: sa_motion_publish_stop(ctx,0x8c61ea2eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ea30; /* fmov.s fr2,@-r8 */
L8c61ea30: sa_motion_publish_stop(ctx,0x8c61ea30u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ea32; /* fmov.s @(r0,r14),fr3 */
L8c61ea32: sa_motion_publish_stop(ctx,0x8c61ea32u); r[0]=20u;goto L8c61ea34; /* mov #0x14,r0 */
L8c61ea34: sa_motion_publish_stop(ctx,0x8c61ea34u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ea36; /* fmov.s fr3,@-r8 */
L8c61ea36: sa_motion_publish_stop(ctx,0x8c61ea36u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ea38; /* fmov.s @(r0,r14),fr3 */
L8c61ea38: sa_motion_publish_stop(ctx,0x8c61ea38u); r[0]=16u;goto L8c61ea3a; /* mov #0x10,r0 */
L8c61ea3a: sa_motion_publish_stop(ctx,0x8c61ea3au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ea3c; /* fmov.s fr3,@-r8 */
L8c61ea3c: sa_motion_publish_stop(ctx,0x8c61ea3cu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ea3e; /* fmov.s @(r0,r14),fr3 */
L8c61ea3e: sa_motion_publish_stop(ctx,0x8c61ea3eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ea40; /* fmov.s fr3,@-r8 */
L8c61ea40: sa_motion_publish_stop(ctx,0x8c61ea40u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61ea42; /* mov.l r3,@-r8 */
L8c61ea42: sa_motion_publish_stop(ctx,0x8c61ea42u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ea44; /* pref @r8 */
L8c61ea44: sa_motion_publish_stop(ctx,0x8c61ea44u); r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61ea46; /* mov.l 0x8c61eb48,r2 */
L8c61ea46: sa_motion_publish_stop(ctx,0x8c61ea46u); r[0]=4u;goto L8c61ea48; /* mov #0x4,r0 */
L8c61ea48: sa_motion_publish_stop(ctx,0x8c61ea48u); r[8]+=60u;goto L8c61ea4a; /* add #0x3c,r8 */
L8c61ea4a: sa_motion_publish_stop(ctx,0x8c61ea4au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61ea4c; /* fmov.s fr4,@-r8 */
L8c61ea4c: sa_motion_publish_stop(ctx,0x8c61ea4cu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ea4e; /* mov.l @r2,r3 */
L8c61ea4e: sa_motion_publish_stop(ctx,0x8c61ea4eu); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ea50; /* fmov.s @(r0,r13),fr0 */
L8c61ea50: sa_motion_publish_stop(ctx,0x8c61ea50u); r[0]=20u;goto L8c61ea52; /* mov #0x14,r0 */
L8c61ea52: sa_motion_publish_stop(ctx,0x8c61ea52u); r[2]=r[3];goto L8c61ea54; /* mov r3,r2 */
L8c61ea54: sa_motion_publish_stop(ctx,0x8c61ea54u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61ea56; /* fmov.s @(r0,r2),fr3 */
L8c61ea56: sa_motion_publish_stop(ctx,0x8c61ea56u); r[1]=r[3];goto L8c61ea58; /* mov r3,r1 */
L8c61ea58: sa_motion_publish_stop(ctx,0x8c61ea58u); r[0]=4u;goto L8c61ea5a; /* mov #0x4,r0 */
L8c61ea5a: sa_motion_publish_stop(ctx,0x8c61ea5au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea5c; /* fmul fr0,fr3 */
L8c61ea5c: sa_motion_publish_stop(ctx,0x8c61ea5cu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61ea5e; /* fmov.s @(r0,r1),fr2 */
L8c61ea5e: sa_motion_publish_stop(ctx,0x8c61ea5eu); fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61ea60; /* fmov.s @r13,fr0 */
L8c61ea60: sa_motion_publish_stop(ctx,0x8c61ea60u); r[0]=8u;goto L8c61ea62; /* mov #0x8,r0 */
L8c61ea62: sa_motion_publish_stop(ctx,0x8c61ea62u); r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61ea64; /* mov.l 0x8c61eb48,r2 */
L8c61ea64: sa_motion_publish_stop(ctx,0x8c61ea64u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea66; /* fmac fr0,fr2,fr3 */
L8c61ea66: sa_motion_publish_stop(ctx,0x8c61ea66u); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ea68; /* fmov.s @(r0,r13),fr0 */
L8c61ea68: sa_motion_publish_stop(ctx,0x8c61ea68u); r[0]=36u;goto L8c61ea6a; /* mov #0x24,r0 */
L8c61ea6a: sa_motion_publish_stop(ctx,0x8c61ea6au); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61ea6c; /* fmov.s @(r0,r3),fr2 */
L8c61ea6c: sa_motion_publish_stop(ctx,0x8c61ea6cu); r[0]=4u;goto L8c61ea6e; /* mov #0x4,r0 */
L8c61ea6e: sa_motion_publish_stop(ctx,0x8c61ea6eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea70; /* fmac fr0,fr2,fr3 */
L8c61ea70: sa_motion_publish_stop(ctx,0x8c61ea70u); fcopy(f,0u,14u);goto L8c61ea72; /* fmov fr14,fr0 */
L8c61ea72: sa_motion_publish_stop(ctx,0x8c61ea72u); fcopy(f,2u,14u);goto L8c61ea74; /* fmov fr14,fr2 */
L8c61ea74: sa_motion_publish_stop(ctx,0x8c61ea74u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ea76; /* fmac fr0,fr3,fr2 */
L8c61ea76: sa_motion_publish_stop(ctx,0x8c61ea76u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ea78; /* fmov.s fr2,@-r8 */
L8c61ea78: sa_motion_publish_stop(ctx,0x8c61ea78u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ea7a; /* mov.l @r2,r3 */
L8c61ea7a: sa_motion_publish_stop(ctx,0x8c61ea7au); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ea7c; /* fmov.s @(r0,r13),fr0 */
L8c61ea7c: sa_motion_publish_stop(ctx,0x8c61ea7cu); r[0]=16u;goto L8c61ea7e; /* mov #0x10,r0 */
L8c61ea7e: sa_motion_publish_stop(ctx,0x8c61ea7eu); r[2]=r[3];goto L8c61ea80; /* mov r3,r2 */
L8c61ea80: sa_motion_publish_stop(ctx,0x8c61ea80u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61ea82; /* fmov.s @(r0,r2),fr3 */
L8c61ea82: sa_motion_publish_stop(ctx,0x8c61ea82u); r[1]=r[3];goto L8c61ea84; /* mov r3,r1 */
L8c61ea84: sa_motion_publish_stop(ctx,0x8c61ea84u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea86; /* fmul fr0,fr3 */
L8c61ea86: sa_motion_publish_stop(ctx,0x8c61ea86u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61ea88; /* fmov.s @r1,fr2 */
L8c61ea88: sa_motion_publish_stop(ctx,0x8c61ea88u); fload(m,bus,&io_status,f,0u,r[13]);if(io_status)return io_status;goto L8c61ea8a; /* fmov.s @r13,fr0 */
L8c61ea8a: sa_motion_publish_stop(ctx,0x8c61ea8au); r[0]=8u;goto L8c61ea8c; /* mov #0x8,r0 */
L8c61ea8c: sa_motion_publish_stop(ctx,0x8c61ea8cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea8e; /* fmac fr0,fr2,fr3 */
L8c61ea8e: sa_motion_publish_stop(ctx,0x8c61ea8eu); fload(m,bus,&io_status,f,0u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ea90; /* fmov.s @(r0,r13),fr0 */
L8c61ea90: sa_motion_publish_stop(ctx,0x8c61ea90u); r[0]=32u;goto L8c61ea92; /* mov #0x20,r0 */
L8c61ea92: sa_motion_publish_stop(ctx,0x8c61ea92u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61ea94; /* fmov.s @(r0,r3),fr2 */
L8c61ea94: sa_motion_publish_stop(ctx,0x8c61ea94u); r[0]=24u;goto L8c61ea96; /* mov #0x18,r0 */
L8c61ea96: sa_motion_publish_stop(ctx,0x8c61ea96u); r[3]=rd(m,bus,&io_status,0x8c61eb4cu,4);if(io_status)return io_status;goto L8c61ea98; /* mov.l 0x8c61eb4c,r3 */
L8c61ea98: sa_motion_publish_stop(ctx,0x8c61ea98u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ea9a; /* fmac fr0,fr2,fr3 */
L8c61ea9a: sa_motion_publish_stop(ctx,0x8c61ea9au); fcopy(f,0u,14u);goto L8c61ea9c; /* fmov fr14,fr0 */
L8c61ea9c: sa_motion_publish_stop(ctx,0x8c61ea9cu); fcopy(f,2u,14u);goto L8c61ea9e; /* fmov fr14,fr2 */
L8c61ea9e: sa_motion_publish_stop(ctx,0x8c61ea9eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eaa0; /* fmac fr0,fr3,fr2 */
L8c61eaa0: sa_motion_publish_stop(ctx,0x8c61eaa0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61eaa2; /* fmov.s fr2,@-r8 */
L8c61eaa2: sa_motion_publish_stop(ctx,0x8c61eaa2u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61eaa4; /* fmov.s @(r0,r13),fr3 */
L8c61eaa4: sa_motion_publish_stop(ctx,0x8c61eaa4u); r[0]=20u;goto L8c61eaa6; /* mov #0x14,r0 */
L8c61eaa6: sa_motion_publish_stop(ctx,0x8c61eaa6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eaa8; /* fmov.s fr3,@-r8 */
L8c61eaa8: sa_motion_publish_stop(ctx,0x8c61eaa8u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61eaaa; /* fmov.s @(r0,r13),fr3 */
L8c61eaaa: sa_motion_publish_stop(ctx,0x8c61eaaau); r[0]=16u;goto L8c61eaac; /* mov #0x10,r0 */
L8c61eaac: sa_motion_publish_stop(ctx,0x8c61eaacu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eaae; /* fmov.s fr3,@-r8 */
L8c61eaae: sa_motion_publish_stop(ctx,0x8c61eaaeu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61eab0; /* fmov.s @(r0,r13),fr3 */
L8c61eab0: sa_motion_publish_stop(ctx,0x8c61eab0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eab2; /* fmov.s fr3,@-r8 */
L8c61eab2: sa_motion_publish_stop(ctx,0x8c61eab2u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61eab4; /* mov.l r3,@-r8 */
L8c61eab4: sa_motion_publish_stop(ctx,0x8c61eab4u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61eab6; /* pref @r8 */
L8c61eab6: sa_motion_publish_stop(ctx,0x8c61eab6u); r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61eab8; /* mov.l 0x8c61eb48,r2 */
L8c61eab8: sa_motion_publish_stop(ctx,0x8c61eab8u); r[0]=4u;goto L8c61eaba; /* mov #0x4,r0 */
L8c61eaba: sa_motion_publish_stop(ctx,0x8c61eabau); r[8]+=60u;goto L8c61eabc; /* add #0x3c,r8 */
L8c61eabc: sa_motion_publish_stop(ctx,0x8c61eabcu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61eabe; /* fmov.s fr4,@-r8 */
L8c61eabe: sa_motion_publish_stop(ctx,0x8c61eabeu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61eac0; /* mov.l @r2,r3 */
L8c61eac0: sa_motion_publish_stop(ctx,0x8c61eac0u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eac2; /* fmov.s @(r0,r12),fr0 */
L8c61eac2: sa_motion_publish_stop(ctx,0x8c61eac2u); r[0]=20u;goto L8c61eac4; /* mov #0x14,r0 */
L8c61eac4: sa_motion_publish_stop(ctx,0x8c61eac4u); r[2]=r[3];goto L8c61eac6; /* mov r3,r2 */
L8c61eac6: sa_motion_publish_stop(ctx,0x8c61eac6u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61eac8; /* fmov.s @(r0,r2),fr3 */
L8c61eac8: sa_motion_publish_stop(ctx,0x8c61eac8u); r[1]=r[3];goto L8c61eaca; /* mov r3,r1 */
L8c61eaca: sa_motion_publish_stop(ctx,0x8c61eacau); r[0]=4u;goto L8c61eacc; /* mov #0x4,r0 */
L8c61eacc: sa_motion_publish_stop(ctx,0x8c61eaccu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eace; /* fmul fr0,fr3 */
L8c61eace: sa_motion_publish_stop(ctx,0x8c61eaceu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61ead0; /* fmov.s @(r0,r1),fr2 */
L8c61ead0: sa_motion_publish_stop(ctx,0x8c61ead0u); fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61ead2; /* fmov.s @r12,fr0 */
L8c61ead2: sa_motion_publish_stop(ctx,0x8c61ead2u); r[0]=8u;goto L8c61ead4; /* mov #0x8,r0 */
L8c61ead4: sa_motion_publish_stop(ctx,0x8c61ead4u); r[2]=rd(m,bus,&io_status,0x8c61eb48u,4);if(io_status)return io_status;goto L8c61ead6; /* mov.l 0x8c61eb48,r2 */
L8c61ead6: sa_motion_publish_stop(ctx,0x8c61ead6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ead8; /* fmac fr0,fr2,fr3 */
L8c61ead8: sa_motion_publish_stop(ctx,0x8c61ead8u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eada; /* fmov.s @(r0,r12),fr0 */
L8c61eada: sa_motion_publish_stop(ctx,0x8c61eadau); r[0]=36u;goto L8c61eadc; /* mov #0x24,r0 */
L8c61eadc: sa_motion_publish_stop(ctx,0x8c61eadcu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61eade; /* fmov.s @(r0,r3),fr2 */
L8c61eade: sa_motion_publish_stop(ctx,0x8c61eadeu); r[0]=4u;goto L8c61eae0; /* mov #0x4,r0 */
L8c61eae0: sa_motion_publish_stop(ctx,0x8c61eae0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eae2; /* fmac fr0,fr2,fr3 */
L8c61eae2: sa_motion_publish_stop(ctx,0x8c61eae2u); fcopy(f,0u,14u);goto L8c61eae4; /* fmov fr14,fr0 */
L8c61eae4: sa_motion_publish_stop(ctx,0x8c61eae4u); fcopy(f,2u,14u);goto L8c61eae6; /* fmov fr14,fr2 */
L8c61eae6: sa_motion_publish_stop(ctx,0x8c61eae6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eae8; /* fmac fr0,fr3,fr2 */
L8c61eae8: sa_motion_publish_stop(ctx,0x8c61eae8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61eaea; /* fmov.s fr2,@-r8 */
L8c61eaea: sa_motion_publish_stop(ctx,0x8c61eaeau); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61eaec; /* mov.l @r2,r3 */
L8c61eaec: sa_motion_publish_stop(ctx,0x8c61eaecu); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eaee; /* fmov.s @(r0,r12),fr0 */
L8c61eaee: sa_motion_publish_stop(ctx,0x8c61eaeeu); r[0]=16u;goto L8c61eaf0; /* mov #0x10,r0 */
L8c61eaf0: sa_motion_publish_stop(ctx,0x8c61eaf0u); r[2]=r[3];goto L8c61eaf2; /* mov r3,r2 */
L8c61eaf2: sa_motion_publish_stop(ctx,0x8c61eaf2u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61eaf4; /* fmov.s @(r0,r2),fr3 */
L8c61eaf4: sa_motion_publish_stop(ctx,0x8c61eaf4u); r[1]=r[3];goto L8c61eaf6; /* mov r3,r1 */
L8c61eaf6: sa_motion_publish_stop(ctx,0x8c61eaf6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eaf8; /* fmul fr0,fr3 */
L8c61eaf8: sa_motion_publish_stop(ctx,0x8c61eaf8u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61eafa; /* fmov.s @r1,fr2 */
L8c61eafa: sa_motion_publish_stop(ctx,0x8c61eafau); fload(m,bus,&io_status,f,0u,r[12]);if(io_status)return io_status;goto L8c61eafc; /* fmov.s @r12,fr0 */
L8c61eafc: sa_motion_publish_stop(ctx,0x8c61eafcu); r[0]=8u;goto L8c61eafe; /* mov #0x8,r0 */
L8c61eafe: sa_motion_publish_stop(ctx,0x8c61eafeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eb00; /* fmac fr0,fr2,fr3 */
L8c61eb00: sa_motion_publish_stop(ctx,0x8c61eb00u); fload(m,bus,&io_status,f,0u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eb02; /* fmov.s @(r0,r12),fr0 */
L8c61eb02: sa_motion_publish_stop(ctx,0x8c61eb02u); r[0]=32u;goto L8c61eb04; /* mov #0x20,r0 */
L8c61eb04: sa_motion_publish_stop(ctx,0x8c61eb04u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61eb06; /* fmov.s @(r0,r3),fr2 */
L8c61eb06: sa_motion_publish_stop(ctx,0x8c61eb06u); r[0]=24u;goto L8c61eb08; /* mov #0x18,r0 */
L8c61eb08: sa_motion_publish_stop(ctx,0x8c61eb08u); r[3]=rd(m,bus,&io_status,0x8c61eb50u,4);if(io_status)return io_status;goto L8c61eb0a; /* mov.l 0x8c61eb50,r3 */
L8c61eb0a: sa_motion_publish_stop(ctx,0x8c61eb0au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eb0c; /* fmac fr0,fr2,fr3 */
L8c61eb0c: sa_motion_publish_stop(ctx,0x8c61eb0cu); fcopy(f,0u,14u);goto L8c61eb0e; /* fmov fr14,fr0 */
L8c61eb0e: sa_motion_publish_stop(ctx,0x8c61eb0eu); fcopy(f,2u,14u);goto L8c61eb10; /* fmov fr14,fr2 */
L8c61eb10: sa_motion_publish_stop(ctx,0x8c61eb10u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eb12; /* fmac fr0,fr3,fr2 */
L8c61eb12: sa_motion_publish_stop(ctx,0x8c61eb12u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61eb14; /* fmov.s fr2,@-r8 */
L8c61eb14: sa_motion_publish_stop(ctx,0x8c61eb14u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eb16; /* fmov.s @(r0,r12),fr3 */
L8c61eb16: sa_motion_publish_stop(ctx,0x8c61eb16u); r[0]=20u;goto L8c61eb18; /* mov #0x14,r0 */
L8c61eb18: sa_motion_publish_stop(ctx,0x8c61eb18u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eb1a; /* fmov.s fr3,@-r8 */
L8c61eb1a: sa_motion_publish_stop(ctx,0x8c61eb1au); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eb1c; /* fmov.s @(r0,r12),fr3 */
L8c61eb1c: sa_motion_publish_stop(ctx,0x8c61eb1cu); r[0]=16u;goto L8c61eb1e; /* mov #0x10,r0 */
L8c61eb1e: sa_motion_publish_stop(ctx,0x8c61eb1eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eb20; /* fmov.s fr3,@-r8 */
L8c61eb20: sa_motion_publish_stop(ctx,0x8c61eb20u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61eb22; /* fmov.s @(r0,r12),fr3 */
L8c61eb22: sa_motion_publish_stop(ctx,0x8c61eb22u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eb24; /* fmov.s fr3,@-r8 */
L8c61eb24: sa_motion_publish_stop(ctx,0x8c61eb24u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61eb26; /* mov.l r3,@-r8 */
L8c61eb26: sa_motion_publish_stop(ctx,0x8c61eb26u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61eb28; /* pref @r8 */
L8c61eb28: sa_motion_publish_stop(ctx,0x8c61eb28u); r[2]=rd(m,bus,&io_status,0x8c61eb54u,4);if(io_status)return io_status;goto L8c61eb2a; /* mov.l 0x8c61eb54,r2 */
L8c61eb2a: sa_motion_publish_stop(ctx,0x8c61eb2au); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61eb2c; /* mov.l @r2,r3 */
L8c61eb2c: sa_motion_publish_stop(ctx,0x8c61eb2cu); r[3]+=4u;goto L8c61eb2e; /* add #0x4,r3 */
L8c61eb2e: sa_motion_publish_stop(ctx,0x8c61eb2eu); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61eb30; /* mov.l r3,@r2 */
L8c61eb30: sa_motion_publish_stop(ctx,0x8c61eb32u); r[8]+=32u;goto L8c61ebf4; /* bra 0x8c61ebf4 */
L8c61eb58: sa_motion_publish_stop(ctx,0x8c61eb58u); r[0]=12u;goto L8c61eb5a; /* mov #0xc,r0 */
L8c61eb5a: sa_motion_publish_stop(ctx,0x8c61eb5au); r[1]=rd(m,bus,&io_status,0x8c61ecb0u,4);if(io_status)return io_status;goto L8c61eb5c; /* mov.l 0x8c61ecb0,r1 */
L8c61eb5c: sa_motion_publish_stop(ctx,0x8c61eb5cu); r[3]=rd(m,bus,&io_status,0x8c61ecb4u,4);if(io_status)return io_status;goto L8c61eb5e; /* mov.l 0x8c61ecb4,r3 */
L8c61eb5e: sa_motion_publish_stop(ctx,0x8c61eb5eu); r[4]=r[15];goto L8c61eb60; /* mov r15,r4 */
L8c61eb60: sa_motion_publish_stop(ctx,0x8c61eb60u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61eb62; /* mov.l r2,@r1 */
L8c61eb62: sa_motion_publish_stop(ctx,0x8c61eb62u); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61eb64; /* fmov.s @r14,fr3 */
L8c61eb64: sa_motion_publish_stop(ctx,0x8c61eb64u); fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61eb66; /* fmov.s @r11,fr2 */
L8c61eb66: sa_motion_publish_stop(ctx,0x8c61eb66u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61eb68; /* fadd fr3,fr2 */
L8c61eb68: sa_motion_publish_stop(ctx,0x8c61eb68u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61eb6a; /* fmov.s @r13,fr3 */
L8c61eb6a: sa_motion_publish_stop(ctx,0x8c61eb6au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61eb6c; /* fadd fr3,fr2 */
L8c61eb6c: sa_motion_publish_stop(ctx,0x8c61eb6cu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61eb6e; /* fmov fr2,@(r0,r15) */
L8c61eb6e: sa_motion_publish_stop(ctx,0x8c61eb6eu); r[0]=4u;goto L8c61eb70; /* mov #0x4,r0 */
L8c61eb70: sa_motion_publish_stop(ctx,0x8c61eb70u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61eb72; /* fmov.s @(r0,r14),fr3 */
L8c61eb72: sa_motion_publish_stop(ctx,0x8c61eb72u); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61eb74; /* fmov.s @(r0,r11),fr2 */
L8c61eb74: sa_motion_publish_stop(ctx,0x8c61eb74u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61eb76; /* fadd fr3,fr2 */
L8c61eb76: sa_motion_publish_stop(ctx,0x8c61eb76u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61eb78; /* fmov.s @(r0,r13),fr3 */
L8c61eb78: sa_motion_publish_stop(ctx,0x8c61eb78u); r[0]=16u;goto L8c61eb7a; /* mov #0x10,r0 */
L8c61eb7a: sa_motion_publish_stop(ctx,0x8c61eb7au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61eb7c; /* fadd fr3,fr2 */
L8c61eb7c: sa_motion_publish_stop(ctx,0x8c61eb7cu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61eb7e; /* fmov fr2,@(r0,r15) */
L8c61eb7e: sa_motion_publish_stop(ctx,0x8c61eb7eu); r[0]=8u;goto L8c61eb80; /* mov #0x8,r0 */
L8c61eb80: sa_motion_publish_stop(ctx,0x8c61eb80u); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61eb82; /* fmov.s @(r0,r11),fr2 */
L8c61eb82: sa_motion_publish_stop(ctx,0x8c61eb82u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61eb84; /* fmov.s @(r0,r14),fr3 */
L8c61eb84: sa_motion_publish_stop(ctx,0x8c61eb84u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61eb86; /* fadd fr3,fr2 */
L8c61eb86: sa_motion_publish_stop(ctx,0x8c61eb86u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61eb88; /* fmov.s @(r0,r13),fr3 */
L8c61eb88: sa_motion_publish_stop(ctx,0x8c61eb88u); r[0]=20u;goto L8c61eb8a; /* mov #0x14,r0 */
L8c61eb8a: sa_motion_publish_stop(ctx,0x8c61eb8au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61eb8c; /* fadd fr3,fr2 */
L8c61eb8c: sa_motion_publish_stop(ctx,0x8c61eb8cu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61eb8e; /* fmov fr2,@(r0,r15) */
L8c61eb8e: sa_motion_publish_stop(ctx,0x8c61eb90u); target=r[3];*pr=0x8c61eb92u;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61eb92; /* jsr @r3 */
L8c61eb92: sa_motion_publish_stop(ctx,0x8c61eb92u); r[3]=rd(m,bus,&io_status,0x8c61ecb8u,4);if(io_status)return io_status;goto L8c61eb94; /* mov.l 0x8c61ecb8,r3 */
L8c61eb94: sa_motion_publish_stop(ctx,0x8c61eb94u); r[0]=12u;goto L8c61eb96; /* mov #0xc,r0 */
L8c61eb96: sa_motion_publish_stop(ctx,0x8c61eb96u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61eb98; /* fmov.s @(r0,r15),fr3 */
L8c61eb98: sa_motion_publish_stop(ctx,0x8c61eb98u); r[0]=16u;goto L8c61eb9a; /* mov #0x10,r0 */
L8c61eb9a: sa_motion_publish_stop(ctx,0x8c61eb9au); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61eb9c; /* fmov.s @r3,fr2 */
L8c61eb9c: sa_motion_publish_stop(ctx,0x8c61eb9cu); r[3]+=8u;goto L8c61eb9e; /* add #0x8,r3 */
L8c61eb9e: sa_motion_publish_stop(ctx,0x8c61eb9eu); r[2]=rd(m,bus,&io_status,0x8c61ecbcu,4);if(io_status)return io_status;goto L8c61eba0; /* mov.l 0x8c61ecbc,r2 */
L8c61eba0: sa_motion_publish_stop(ctx,0x8c61eba0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eba2; /* fmul fr3,fr2 */
L8c61eba2: sa_motion_publish_stop(ctx,0x8c61eba2u); r[1]=rd(m,bus,&io_status,0x8c61ecc0u,4);if(io_status)return io_status;goto L8c61eba4; /* mov.l 0x8c61ecc0,r1 */
L8c61eba4: sa_motion_publish_stop(ctx,0x8c61eba4u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61eba6; /* fmov.s @r2,fr3 */
L8c61eba6: sa_motion_publish_stop(ctx,0x8c61eba6u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61eba8; /* fmov.s @r1,fr1 */
L8c61eba8: sa_motion_publish_stop(ctx,0x8c61eba8u); r[2]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61ebaa; /* mov.l @(0xc,r14),r2 */
L8c61ebaa: sa_motion_publish_stop(ctx,0x8c61ebaau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ebac; /* fsub fr2,fr3 */
L8c61ebac: sa_motion_publish_stop(ctx,0x8c61ebacu); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ebae; /* fmov.s @(r0,r15),fr2 */
L8c61ebae: sa_motion_publish_stop(ctx,0x8c61ebaeu); r[0]=20u;goto L8c61ebb0; /* mov #0x14,r0 */
L8c61ebb0: sa_motion_publish_stop(ctx,0x8c61ebb0u); r[4]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61ebb2; /* mov.l @(0xc,r11),r4 */
L8c61ebb2: sa_motion_publish_stop(ctx,0x8c61ebb2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ebb4; /* fmul fr2,fr1 */
L8c61ebb4: sa_motion_publish_stop(ctx,0x8c61ebb4u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ebb6; /* fmov.s @(r0,r15),fr2 */
L8c61ebb6: sa_motion_publish_stop(ctx,0x8c61ebb6u); r[0]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61ebb8; /* mov.l @(0xc,r13),r0 */
L8c61ebb8: sa_motion_publish_stop(ctx,0x8c61ebb8u); r[4]&=r[2];goto L8c61ebba; /* and r2,r4 */
L8c61ebba: sa_motion_publish_stop(ctx,0x8c61ebbau); r[2]=1u;goto L8c61ebbc; /* mov #0x1,r2 */
L8c61ebbc: sa_motion_publish_stop(ctx,0x8c61ebbcu); r[4]&=r[0];goto L8c61ebbe; /* and r0,r4 */
L8c61ebbe: sa_motion_publish_stop(ctx,0x8c61ebbeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ebc0; /* fsub fr1,fr3 */
L8c61ebc0: sa_motion_publish_stop(ctx,0x8c61ebc0u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61ebc2; /* fmov.s @r3,fr1 */
L8c61ebc2: sa_motion_publish_stop(ctx,0x8c61ebc2u); *sr=(*sr&~1u)|!(r[2]&r[4]);goto L8c61ebc4; /* tst r2,r4 */
L8c61ebc4: sa_motion_publish_stop(ctx,0x8c61ebc4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ebc6; /* fmul fr2,fr1 */
L8c61ebc6: sa_motion_publish_stop(ctx,0x8c61ebc6u); fcopy(f,15u,3u);goto L8c61ebc8; /* fmov fr3,fr15 */
L8c61ebc8: sa_motion_publish_stop(ctx,0x8c61ebcau); take=!(*sr&1u);{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[1],&f->fpscr,&fr[15],1);if(status)return status;}if(take)goto L8c61ebd8;goto L8c61ebcc; /* bf/s 0x8c61ebd8 */
L8c61ebcc: sa_motion_publish_stop(ctx,0x8c61ebccu); r[3]=rd(m,bus,&io_status,0x8c61ecc4u,4);if(io_status)return io_status;goto L8c61ebce; /* mov.l 0x8c61ecc4,r3 */
L8c61ebce: sa_motion_publish_stop(ctx,0x8c61ebceu); r[5]=r[14];goto L8c61ebd0; /* mov r14,r5 */
L8c61ebd0: sa_motion_publish_stop(ctx,0x8c61ebd0u); fcopy(f,4u,15u);goto L8c61ebd2; /* fmov fr15,fr4 */
L8c61ebd2: sa_motion_publish_stop(ctx,0x8c61ebd2u); r[6]=r[13];goto L8c61ebd4; /* mov r13,r6 */
L8c61ebd4: sa_motion_publish_stop(ctx,0x8c61ebd6u); target=r[3];*pr=0x8c61ebd8u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ebd8; /* jsr @r3 */
L8c61ebd8: sa_motion_publish_stop(ctx,0x8c61ebd8u); r[4]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61ebda; /* mov.l @(0xc,r14),r4 */
L8c61ebda: sa_motion_publish_stop(ctx,0x8c61ebdau); r[3]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61ebdc; /* mov.l @(0xc,r13),r3 */
L8c61ebdc: sa_motion_publish_stop(ctx,0x8c61ebdcu); r[2]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61ebde; /* mov.l @(0xc,r12),r2 */
L8c61ebde: sa_motion_publish_stop(ctx,0x8c61ebdeu); r[4]&=r[3];goto L8c61ebe0; /* and r3,r4 */
L8c61ebe0: sa_motion_publish_stop(ctx,0x8c61ebe0u); r[3]=1u;goto L8c61ebe2; /* mov #0x1,r3 */
L8c61ebe2: sa_motion_publish_stop(ctx,0x8c61ebe2u); r[4]&=r[2];goto L8c61ebe4; /* and r2,r4 */
L8c61ebe4: sa_motion_publish_stop(ctx,0x8c61ebe4u); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61ebe6; /* tst r3,r4 */
L8c61ebe6: sa_motion_publish_stop(ctx,0x8c61ebe6u); if(!(*sr&1u))goto L8c61ebf4;goto L8c61ebe8; /* bf 0x8c61ebf4 */
L8c61ebe8: sa_motion_publish_stop(ctx,0x8c61ebe8u); r[1]=rd(m,bus,&io_status,0x8c61ecc4u,4);if(io_status)return io_status;goto L8c61ebea; /* mov.l 0x8c61ecc4,r1 */
L8c61ebea: sa_motion_publish_stop(ctx,0x8c61ebeau); r[5]=r[13];goto L8c61ebec; /* mov r13,r5 */
L8c61ebec: sa_motion_publish_stop(ctx,0x8c61ebecu); fcopy(f,4u,15u);goto L8c61ebee; /* fmov fr15,fr4 */
L8c61ebee: sa_motion_publish_stop(ctx,0x8c61ebeeu); r[6]=r[12];goto L8c61ebf0; /* mov r12,r6 */
L8c61ebf0: sa_motion_publish_stop(ctx,0x8c61ebf2u); target=r[1];*pr=0x8c61ebf4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ebf4; /* jsr @r1 */
L8c61ebf4: sa_motion_publish_stop(ctx,0x8c61ebf4u); r[9]+=1u;goto L8c61ebf6; /* add #0x1,r9 */
L8c61ebf6: sa_motion_publish_stop(ctx,0x8c61ebf6u); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61ebf8; /* mov.l @r15,r3 */
L8c61ebf8: sa_motion_publish_stop(ctx,0x8c61ebf8u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61ebfa; /* mov.w @(0x2,r3),r0 */
L8c61ebfa: sa_motion_publish_stop(ctx,0x8c61ebfau); r[0]=r[0]&65535u;goto L8c61ebfc; /* extu.w r0,r0 */
L8c61ebfc: sa_motion_publish_stop(ctx,0x8c61ebfcu); *sr=(*sr&~1u)|((int32_t)r[9]>=(int32_t)r[0]);goto L8c61ebfe; /* cmp/ge r0,r9 */
L8c61ebfe: sa_motion_publish_stop(ctx,0x8c61ebfeu); if((*sr&1u))goto L8c61ec04;goto L8c61ec00; /* bt 0x8c61ec04 */
L8c61ec00: sa_motion_publish_stop(ctx,0x8c61ec02u); goto L8c61e870; /* bra 0x8c61e870 */
L8c61ec04: sa_motion_publish_stop(ctx,0x8c61ec06u); goto L8c61f0bc; /* bra 0x8c61f0bc */
L8c61ec08: sa_motion_publish_stop(ctx,0x8c61ec08u); r[1]=0u;goto L8c61ec0a; /* mov #0x0,r1 */
L8c61ec0a: sa_motion_publish_stop(ctx,0x8c61ec0cu); wr(m,bus,&io_status,r[15]+0x4u,r[1],4);if(io_status)return io_status;goto L8c61f02e; /* bra 0x8c61f02e */
L8c61ec0e: sa_motion_publish_stop(ctx,0x8c61ec0eu); r[3]=rd(m,bus,&io_status,0x8c61ecc8u,4);if(io_status)return io_status;goto L8c61ec10; /* mov.l 0x8c61ecc8,r3 */
L8c61ec10: sa_motion_publish_stop(ctx,0x8c61ec10u); r[7]=0u;goto L8c61ec12; /* mov #0x0,r7 */
L8c61ec12: sa_motion_publish_stop(ctx,0x8c61ec12u); r[9]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61ec14; /* mov.w @r10+,r9 */
L8c61ec14: sa_motion_publish_stop(ctx,0x8c61ec14u); r[4]=0u;goto L8c61ec16; /* mov #0x0,r4 */
L8c61ec16: sa_motion_publish_stop(ctx,0x8c61ec16u); r[2]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ec18; /* mov.l @r3,r2 */
L8c61ec18: sa_motion_publish_stop(ctx,0x8c61ec18u); r[5]=r[7];goto L8c61ec1a; /* mov r7,r5 */
L8c61ec1a: sa_motion_publish_stop(ctx,0x8c61ec1au); r[9]=r[9]&65535u;goto L8c61ec1c; /* extu.w r9,r9 */
L8c61ec1c: sa_motion_publish_stop(ctx,0x8c61ec1cu); r[2]+=r[9];goto L8c61ec1e; /* add r9,r2 */
L8c61ec1e: sa_motion_publish_stop(ctx,0x8c61ec1eu); *sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c61ec20; /* cmp/pl r9 */
L8c61ec20: sa_motion_publish_stop(ctx,0x8c61ec20u); wr(m,bus,&io_status,r[3],r[2],4);if(io_status)return io_status;goto L8c61ec22; /* mov.l r2,@r3 */
L8c61ec22: sa_motion_publish_stop(ctx,0x8c61ec22u); r[6]=4294967295u;goto L8c61ec24; /* mov #-0x1,r6 */
L8c61ec24: sa_motion_publish_stop(ctx,0x8c61ec26u); take=!(*sr&1u);r[4]+=r[10];if(take)goto L8c61ec46;goto L8c61ec28; /* bf/s 0x8c61ec46 */
L8c61ec28: sa_motion_publish_stop(ctx,0x8c61ec28u); r[11]=sx(m,bus,&io_status,r[4],2);if(io_status)return io_status;r[4]+=2;goto L8c61ec2a; /* mov.w @r4+,r11 */
L8c61ec2a: sa_motion_publish_stop(ctx,0x8c61ec2au); r[5]+=1u;goto L8c61ec2c; /* add #0x1,r5 */
L8c61ec2c: sa_motion_publish_stop(ctx,0x8c61ec2cu); r[2]=rd(m,bus,&io_status,0x8c61ecccu,4);if(io_status)return io_status;goto L8c61ec2e; /* mov.l 0x8c61eccc,r2 */
L8c61ec2e: sa_motion_publish_stop(ctx,0x8c61ec2eu); r[11]=r[11]&65535u;goto L8c61ec30; /* extu.w r11,r11 */
L8c61ec30: sa_motion_publish_stop(ctx,0x8c61ec30u); r[11]<<=2;goto L8c61ec32; /* shll2 r11 */
L8c61ec32: sa_motion_publish_stop(ctx,0x8c61ec32u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ec34; /* mov.l @r2,r3 */
L8c61ec34: sa_motion_publish_stop(ctx,0x8c61ec34u); r[11]<<=2;goto L8c61ec36; /* shll2 r11 */
L8c61ec36: sa_motion_publish_stop(ctx,0x8c61ec36u); *sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c61ec38; /* shll r11 */
L8c61ec38: sa_motion_publish_stop(ctx,0x8c61ec38u); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[9]);goto L8c61ec3a; /* cmp/ge r9,r5 */
L8c61ec3a: sa_motion_publish_stop(ctx,0x8c61ec3au); r[11]+=r[3];goto L8c61ec3c; /* add r3,r11 */
L8c61ec3c: sa_motion_publish_stop(ctx,0x8c61ec3cu); wr(m,bus,&io_status,r[15]+0x8u,r[11],4);if(io_status)return io_status;goto L8c61ec3e; /* mov.l r11,@(0x8,r15) */
L8c61ec3e: sa_motion_publish_stop(ctx,0x8c61ec3eu); r[11]=rd(m,bus,&io_status,r[11]+0xcu,4);if(io_status)return io_status;goto L8c61ec40; /* mov.l @(0xc,r11),r11 */
L8c61ec40: sa_motion_publish_stop(ctx,0x8c61ec40u); r[6]&=r[11];goto L8c61ec42; /* and r11,r6 */
L8c61ec42: sa_motion_publish_stop(ctx,0x8c61ec44u); take=!(*sr&1u);r[7]|=r[11];if(take)goto L8c61ec28;goto L8c61ec46; /* bf/s 0x8c61ec28 */
L8c61ec46: sa_motion_publish_stop(ctx,0x8c61ec46u); r[2]=63u;goto L8c61ec48; /* mov #0x3f,r2 */
L8c61ec48: sa_motion_publish_stop(ctx,0x8c61ec48u); *sr=(*sr&~1u)|!(r[2]&r[6]);goto L8c61ec4a; /* tst r2,r6 */
L8c61ec4a: sa_motion_publish_stop(ctx,0x8c61ec4au); if(!(*sr&1u))goto L8c61ecaa;goto L8c61ec4c; /* bf 0x8c61ecaa */
L8c61ec4c: sa_motion_publish_stop(ctx,0x8c61ec4cu); r[1]=1u;goto L8c61ec4e; /* mov #0x1,r1 */
L8c61ec4e: sa_motion_publish_stop(ctx,0x8c61ec4eu); *sr=(*sr&~1u)|!(r[1]&r[7]);goto L8c61ec50; /* tst r1,r7 */
L8c61ec50: sa_motion_publish_stop(ctx,0x8c61ec50u); if((*sr&1u))goto L8c61ec56;goto L8c61ec52; /* bt 0x8c61ec56 */
L8c61ec52: sa_motion_publish_stop(ctx,0x8c61ec54u); goto L8c61ef68; /* bra 0x8c61ef68 */
L8c61ec56: sa_motion_publish_stop(ctx,0x8c61ec56u); r[0]=sx(m,bus,&io_status,r[10]+0x2u,2);if(io_status)return io_status;goto L8c61ec58; /* mov.w @(0x2,r10),r0 */
L8c61ec58: sa_motion_publish_stop(ctx,0x8c61ec58u); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;goto L8c61ec5a; /* mov.w @r10,r12 */
L8c61ec5a: sa_motion_publish_stop(ctx,0x8c61ec5au); r[3]=rd(m,bus,&io_status,0x8c61ecccu,4);if(io_status)return io_status;goto L8c61ec5c; /* mov.l 0x8c61eccc,r3 */
L8c61ec5c: sa_motion_publish_stop(ctx,0x8c61ec5cu); r[14]=r[0]&65535u;goto L8c61ec5e; /* extu.w r0,r14 */
L8c61ec5e: sa_motion_publish_stop(ctx,0x8c61ec5eu); r[14]<<=2;goto L8c61ec60; /* shll2 r14 */
L8c61ec60: sa_motion_publish_stop(ctx,0x8c61ec60u); r[0]=sx(m,bus,&io_status,r[10]+0x4u,2);if(io_status)return io_status;goto L8c61ec62; /* mov.w @(0x4,r10),r0 */
L8c61ec62: sa_motion_publish_stop(ctx,0x8c61ec62u); r[12]=r[12]&65535u;goto L8c61ec64; /* extu.w r12,r12 */
L8c61ec64: sa_motion_publish_stop(ctx,0x8c61ec64u); r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ec66; /* mov.l @r3,r4 */
L8c61ec66: sa_motion_publish_stop(ctx,0x8c61ec66u); r[12]<<=2;goto L8c61ec68; /* shll2 r12 */
L8c61ec68: sa_motion_publish_stop(ctx,0x8c61ec68u); r[14]<<=2;goto L8c61ec6a; /* shll2 r14 */
L8c61ec6a: sa_motion_publish_stop(ctx,0x8c61ec6au); r[13]=r[0]&65535u;goto L8c61ec6c; /* extu.w r0,r13 */
L8c61ec6c: sa_motion_publish_stop(ctx,0x8c61ec6cu); r[12]<<=2;goto L8c61ec6e; /* shll2 r12 */
L8c61ec6e: sa_motion_publish_stop(ctx,0x8c61ec6eu); r[0]=16u;goto L8c61ec70; /* mov #0x10,r0 */
L8c61ec70: sa_motion_publish_stop(ctx,0x8c61ec70u); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61ec72; /* shll r12 */
L8c61ec72: sa_motion_publish_stop(ctx,0x8c61ec72u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61ec74; /* shll r14 */
L8c61ec74: sa_motion_publish_stop(ctx,0x8c61ec74u); r[14]+=r[4];goto L8c61ec76; /* add r4,r14 */
L8c61ec76: sa_motion_publish_stop(ctx,0x8c61ec76u); fload(m,bus,&io_status,f,5u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ec78; /* fmov.s @(r0,r14),fr5 */
L8c61ec78: sa_motion_publish_stop(ctx,0x8c61ec78u); r[13]<<=2;goto L8c61ec7a; /* shll2 r13 */
L8c61ec7a: sa_motion_publish_stop(ctx,0x8c61ec7au); r[12]+=r[4];goto L8c61ec7c; /* add r4,r12 */
L8c61ec7c: sa_motion_publish_stop(ctx,0x8c61ec7cu); r[13]<<=2;goto L8c61ec7e; /* shll2 r13 */
L8c61ec7e: sa_motion_publish_stop(ctx,0x8c61ec7eu); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61ec80; /* fmov.s @(r0,r12),fr3 */
L8c61ec80: sa_motion_publish_stop(ctx,0x8c61ec80u); fcopy(f,2u,5u);goto L8c61ec82; /* fmov fr5,fr2 */
L8c61ec82: sa_motion_publish_stop(ctx,0x8c61ec82u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61ec84; /* shll r13 */
L8c61ec84: sa_motion_publish_stop(ctx,0x8c61ec84u); r[0]=20u;goto L8c61ec86; /* mov #0x14,r0 */
L8c61ec86: sa_motion_publish_stop(ctx,0x8c61ec86u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c61ec88; /* fsub fr3,fr2 */
L8c61ec88: sa_motion_publish_stop(ctx,0x8c61ec88u); r[13]+=r[4];goto L8c61ec8a; /* add r4,r13 */
L8c61ec8a: sa_motion_publish_stop(ctx,0x8c61ec8au); fload(m,bus,&io_status,f,4u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ec8c; /* fmov.s @(r0,r14),fr4 */
L8c61ec8c: sa_motion_publish_stop(ctx,0x8c61ec8cu); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ec8e; /* fmov.s @(r0,r13),fr3 */
L8c61ec8e: sa_motion_publish_stop(ctx,0x8c61ec8eu); fcopy(f,1u,4u);goto L8c61ec90; /* fmov fr4,fr1 */
L8c61ec90: sa_motion_publish_stop(ctx,0x8c61ec90u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ec92; /* fsub fr4,fr3 */
L8c61ec92: sa_motion_publish_stop(ctx,0x8c61ec92u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ec94; /* fmul fr3,fr2 */
L8c61ec94: sa_motion_publish_stop(ctx,0x8c61ec94u); fload(m,bus,&io_status,f,3u,r[0]+r[12]);if(io_status)return io_status;goto L8c61ec96; /* fmov.s @(r0,r12),fr3 */
L8c61ec96: sa_motion_publish_stop(ctx,0x8c61ec96u); r[0]=16u;goto L8c61ec98; /* mov #0x10,r0 */
L8c61ec98: sa_motion_publish_stop(ctx,0x8c61ec98u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[1],&fr[3],&f->fpscr,&fr[1],1);if(status)return status;}goto L8c61ec9a; /* fsub fr3,fr1 */
L8c61ec9a: sa_motion_publish_stop(ctx,0x8c61ec9au); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ec9c; /* fmov.s @(r0,r13),fr3 */
L8c61ec9c: sa_motion_publish_stop(ctx,0x8c61ec9cu); fcopy(f,4u,2u);goto L8c61ec9e; /* fmov fr2,fr4 */
L8c61ec9e: sa_motion_publish_stop(ctx,0x8c61ec9eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[5],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61eca0; /* fsub fr5,fr3 */
L8c61eca0: sa_motion_publish_stop(ctx,0x8c61eca0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61eca2; /* fmul fr3,fr1 */
L8c61eca2: sa_motion_publish_stop(ctx,0x8c61eca2u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c61eca4; /* fldi0 fr3 */
L8c61eca4: sa_motion_publish_stop(ctx,0x8c61eca4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61eca6; /* fsub fr1,fr4 */
L8c61eca6: sa_motion_publish_stop(ctx,0x8c61eca6u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c61eca8; /* fcmp/gt fr3,fr4 */
L8c61eca8: sa_motion_publish_stop(ctx,0x8c61eca8u); if(!(*sr&1u))goto L8c61ecd0;goto L8c61ecaa; /* bf 0x8c61ecd0 */
L8c61ecaa: sa_motion_publish_stop(ctx,0x8c61ecaau); *sr=(*sr&~1u)|(r[9]>>31);r[9]<<=1;goto L8c61ecac; /* shll r9 */
L8c61ecac: sa_motion_publish_stop(ctx,0x8c61ecaeu); r[10]+=r[9];goto L8c61f028; /* bra 0x8c61f028 */
L8c61ecd0: sa_motion_publish_stop(ctx,0x8c61ecd0u); r[0]=12u;goto L8c61ecd2; /* mov #0xc,r0 */
L8c61ecd2: sa_motion_publish_stop(ctx,0x8c61ecd2u); r[2]=rd(m,bus,&io_status,0x8c61ef44u,4);if(io_status)return io_status;goto L8c61ecd4; /* mov.l 0x8c61ef44,r2 */
L8c61ecd4: sa_motion_publish_stop(ctx,0x8c61ecd4u); r[4]=r[15];goto L8c61ecd6; /* mov r15,r4 */
L8c61ecd6: sa_motion_publish_stop(ctx,0x8c61ecd6u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ecd8; /* mov.l @r2,r3 */
L8c61ecd8: sa_motion_publish_stop(ctx,0x8c61ecd8u); r[3]+=r[9];goto L8c61ecda; /* add r9,r3 */
L8c61ecda: sa_motion_publish_stop(ctx,0x8c61ecdau); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61ecdc; /* mov.l r3,@r2 */
L8c61ecdc: sa_motion_publish_stop(ctx,0x8c61ecdcu); fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61ecde; /* fmov.s @r14,fr3 */
L8c61ecde: sa_motion_publish_stop(ctx,0x8c61ecdeu); fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61ece0; /* fmov.s @r12,fr2 */
L8c61ece0: sa_motion_publish_stop(ctx,0x8c61ece0u); r[3]=rd(m,bus,&io_status,0x8c61ef48u,4);if(io_status)return io_status;goto L8c61ece2; /* mov.l 0x8c61ef48,r3 */
L8c61ece2: sa_motion_publish_stop(ctx,0x8c61ece2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ece4; /* fadd fr3,fr2 */
L8c61ece4: sa_motion_publish_stop(ctx,0x8c61ece4u); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61ece6; /* fmov.s @r13,fr3 */
L8c61ece6: sa_motion_publish_stop(ctx,0x8c61ece6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ece8; /* fadd fr3,fr2 */
L8c61ece8: sa_motion_publish_stop(ctx,0x8c61ece8u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ecea; /* fmov fr2,@(r0,r15) */
L8c61ecea: sa_motion_publish_stop(ctx,0x8c61eceau); r[0]=4u;goto L8c61ecec; /* mov #0x4,r0 */
L8c61ecec: sa_motion_publish_stop(ctx,0x8c61ececu); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61ecee; /* fmov.s @(r0,r12),fr2 */
L8c61ecee: sa_motion_publish_stop(ctx,0x8c61eceeu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ecf0; /* fmov.s @(r0,r14),fr3 */
L8c61ecf0: sa_motion_publish_stop(ctx,0x8c61ecf0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ecf2; /* fadd fr3,fr2 */
L8c61ecf2: sa_motion_publish_stop(ctx,0x8c61ecf2u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ecf4; /* fmov.s @(r0,r13),fr3 */
L8c61ecf4: sa_motion_publish_stop(ctx,0x8c61ecf4u); r[0]=16u;goto L8c61ecf6; /* mov #0x10,r0 */
L8c61ecf6: sa_motion_publish_stop(ctx,0x8c61ecf6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ecf8; /* fadd fr3,fr2 */
L8c61ecf8: sa_motion_publish_stop(ctx,0x8c61ecf8u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ecfa; /* fmov fr2,@(r0,r15) */
L8c61ecfa: sa_motion_publish_stop(ctx,0x8c61ecfau); r[0]=8u;goto L8c61ecfc; /* mov #0x8,r0 */
L8c61ecfc: sa_motion_publish_stop(ctx,0x8c61ecfcu); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ecfe; /* fmov.s @(r0,r14),fr3 */
L8c61ecfe: sa_motion_publish_stop(ctx,0x8c61ecfeu); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61ed00; /* fmov.s @(r0,r12),fr2 */
L8c61ed00: sa_motion_publish_stop(ctx,0x8c61ed00u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ed02; /* fadd fr3,fr2 */
L8c61ed02: sa_motion_publish_stop(ctx,0x8c61ed02u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ed04; /* fmov.s @(r0,r13),fr3 */
L8c61ed04: sa_motion_publish_stop(ctx,0x8c61ed04u); r[0]=20u;goto L8c61ed06; /* mov #0x14,r0 */
L8c61ed06: sa_motion_publish_stop(ctx,0x8c61ed06u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ed08; /* fadd fr3,fr2 */
L8c61ed08: sa_motion_publish_stop(ctx,0x8c61ed08u); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ed0a; /* fmov fr2,@(r0,r15) */
L8c61ed0a: sa_motion_publish_stop(ctx,0x8c61ed0cu); target=r[3];*pr=0x8c61ed0eu;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61ed0e; /* jsr @r3 */
L8c61ed0e: sa_motion_publish_stop(ctx,0x8c61ed0eu); r[3]=rd(m,bus,&io_status,0x8c61ef4cu,4);if(io_status)return io_status;goto L8c61ed10; /* mov.l 0x8c61ef4c,r3 */
L8c61ed10: sa_motion_publish_stop(ctx,0x8c61ed10u); r[0]=12u;goto L8c61ed12; /* mov #0xc,r0 */
L8c61ed12: sa_motion_publish_stop(ctx,0x8c61ed12u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ed14; /* fmov.s @(r0,r15),fr3 */
L8c61ed14: sa_motion_publish_stop(ctx,0x8c61ed14u); r[0]=16u;goto L8c61ed16; /* mov #0x10,r0 */
L8c61ed16: sa_motion_publish_stop(ctx,0x8c61ed16u); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61ed18; /* fmov.s @r3,fr2 */
L8c61ed18: sa_motion_publish_stop(ctx,0x8c61ed18u); r[3]+=8u;goto L8c61ed1a; /* add #0x8,r3 */
L8c61ed1a: sa_motion_publish_stop(ctx,0x8c61ed1au); r[2]=rd(m,bus,&io_status,0x8c61ef50u,4);if(io_status)return io_status;goto L8c61ed1c; /* mov.l 0x8c61ef50,r2 */
L8c61ed1c: sa_motion_publish_stop(ctx,0x8c61ed1cu); r[8]+=24u;goto L8c61ed1e; /* add #0x18,r8 */
L8c61ed1e: sa_motion_publish_stop(ctx,0x8c61ed1eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ed20; /* fmul fr3,fr2 */
L8c61ed20: sa_motion_publish_stop(ctx,0x8c61ed20u); r[1]=rd(m,bus,&io_status,0x8c61ef54u,4);if(io_status)return io_status;goto L8c61ed22; /* mov.l 0x8c61ef54,r1 */
L8c61ed22: sa_motion_publish_stop(ctx,0x8c61ed22u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61ed24; /* fmov.s @r2,fr3 */
L8c61ed24: sa_motion_publish_stop(ctx,0x8c61ed24u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61ed26; /* fmov.s @r1,fr1 */
L8c61ed26: sa_motion_publish_stop(ctx,0x8c61ed26u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61ed28; /* mov.w @r10+,r4 */
L8c61ed28: sa_motion_publish_stop(ctx,0x8c61ed28u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ed2a; /* fsub fr2,fr3 */
L8c61ed2a: sa_motion_publish_stop(ctx,0x8c61ed2au); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ed2c; /* fmov.s @(r0,r15),fr2 */
L8c61ed2c: sa_motion_publish_stop(ctx,0x8c61ed2cu); r[0]=20u;goto L8c61ed2e; /* mov #0x14,r0 */
L8c61ed2e: sa_motion_publish_stop(ctx,0x8c61ed2eu); r[1]=rd(m,bus,&io_status,0x8c61ef5cu,4);if(io_status)return io_status;goto L8c61ed30; /* mov.l 0x8c61ef5c,r1 */
L8c61ed30: sa_motion_publish_stop(ctx,0x8c61ed30u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ed32; /* fmul fr2,fr1 */
L8c61ed32: sa_motion_publish_stop(ctx,0x8c61ed32u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ed34; /* fmov.s @(r0,r15),fr2 */
L8c61ed34: sa_motion_publish_stop(ctx,0x8c61ed34u); r[4]=r[4]&65535u;goto L8c61ed36; /* extu.w r4,r4 */
L8c61ed36: sa_motion_publish_stop(ctx,0x8c61ed36u); r[0]=rd(m,bus,&io_status,0x8c61ef58u,4);if(io_status)return io_status;goto L8c61ed38; /* mov.l 0x8c61ef58,r0 */
L8c61ed38: sa_motion_publish_stop(ctx,0x8c61ed38u); r[4]<<=2;goto L8c61ed3a; /* shll2 r4 */
L8c61ed3a: sa_motion_publish_stop(ctx,0x8c61ed3au); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61ed3c; /* mov.l @r0,r2 */
L8c61ed3c: sa_motion_publish_stop(ctx,0x8c61ed3cu); r[4]<<=2;goto L8c61ed3e; /* shll2 r4 */
L8c61ed3e: sa_motion_publish_stop(ctx,0x8c61ed3eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61ed40; /* fsub fr1,fr3 */
L8c61ed40: sa_motion_publish_stop(ctx,0x8c61ed40u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61ed42; /* fmov.s @r3,fr1 */
L8c61ed42: sa_motion_publish_stop(ctx,0x8c61ed42u); r[3]=rd(m,bus,&io_status,r[1],4);if(io_status)return io_status;goto L8c61ed44; /* mov.l @r1,r3 */
L8c61ed44: sa_motion_publish_stop(ctx,0x8c61ed44u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61ed46; /* shll r4 */
L8c61ed46: sa_motion_publish_stop(ctx,0x8c61ed46u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61ed48; /* fmul fr2,fr1 */
L8c61ed48: sa_motion_publish_stop(ctx,0x8c61ed48u); r[4]+=r[2];goto L8c61ed4a; /* add r2,r4 */
L8c61ed4a: sa_motion_publish_stop(ctx,0x8c61ed4au); fcopy(f,4u,3u);goto L8c61ed4c; /* fmov fr3,fr4 */
L8c61ed4c: sa_motion_publish_stop(ctx,0x8c61ed4cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c61ed4e; /* fsub fr1,fr4 */
L8c61ed4e: sa_motion_publish_stop(ctx,0x8c61ed4eu); r[0]=4u;goto L8c61ed50; /* mov #0x4,r0 */
L8c61ed50: sa_motion_publish_stop(ctx,0x8c61ed50u); r[1]=r[3];goto L8c61ed52; /* mov r3,r1 */
L8c61ed52: sa_motion_publish_stop(ctx,0x8c61ed52u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ed54; /* fmov.s @(r0,r4),fr0 */
L8c61ed54: sa_motion_publish_stop(ctx,0x8c61ed54u); r[0]=20u;goto L8c61ed56; /* mov #0x14,r0 */
L8c61ed56: sa_motion_publish_stop(ctx,0x8c61ed56u); fload(m,bus,&io_status,f,3u,r[0]+r[1]);if(io_status)return io_status;goto L8c61ed58; /* fmov.s @(r0,r1),fr3 */
L8c61ed58: sa_motion_publish_stop(ctx,0x8c61ed58u); r[0]=4u;goto L8c61ed5a; /* mov #0x4,r0 */
L8c61ed5a: sa_motion_publish_stop(ctx,0x8c61ed5au); r[2]=r[3];goto L8c61ed5c; /* mov r3,r2 */
L8c61ed5c: sa_motion_publish_stop(ctx,0x8c61ed5cu); fload(m,bus,&io_status,f,2u,r[0]+r[2]);if(io_status)return io_status;goto L8c61ed5e; /* fmov.s @(r0,r2),fr2 */
L8c61ed5e: sa_motion_publish_stop(ctx,0x8c61ed5eu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ed60; /* fmul fr0,fr3 */
L8c61ed60: sa_motion_publish_stop(ctx,0x8c61ed60u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ed62; /* fmov.s @r4,fr0 */
L8c61ed62: sa_motion_publish_stop(ctx,0x8c61ed62u); r[0]=8u;goto L8c61ed64; /* mov #0x8,r0 */
L8c61ed64: sa_motion_publish_stop(ctx,0x8c61ed64u); r[2]=rd(m,bus,&io_status,0x8c61ef5cu,4);if(io_status)return io_status;goto L8c61ed66; /* mov.l 0x8c61ef5c,r2 */
L8c61ed66: sa_motion_publish_stop(ctx,0x8c61ed66u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ed68; /* fmac fr0,fr2,fr3 */
L8c61ed68: sa_motion_publish_stop(ctx,0x8c61ed68u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ed6a; /* fmov.s @(r0,r4),fr0 */
L8c61ed6a: sa_motion_publish_stop(ctx,0x8c61ed6au); r[0]=36u;goto L8c61ed6c; /* mov #0x24,r0 */
L8c61ed6c: sa_motion_publish_stop(ctx,0x8c61ed6cu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61ed6e; /* fmov.s @(r0,r3),fr2 */
L8c61ed6e: sa_motion_publish_stop(ctx,0x8c61ed6eu); r[0]=4u;goto L8c61ed70; /* mov #0x4,r0 */
L8c61ed70: sa_motion_publish_stop(ctx,0x8c61ed70u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ed72; /* fmac fr0,fr2,fr3 */
L8c61ed72: sa_motion_publish_stop(ctx,0x8c61ed72u); fcopy(f,0u,14u);goto L8c61ed74; /* fmov fr14,fr0 */
L8c61ed74: sa_motion_publish_stop(ctx,0x8c61ed74u); fcopy(f,2u,14u);goto L8c61ed76; /* fmov fr14,fr2 */
L8c61ed76: sa_motion_publish_stop(ctx,0x8c61ed76u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ed78; /* fmac fr0,fr3,fr2 */
L8c61ed78: sa_motion_publish_stop(ctx,0x8c61ed78u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ed7a; /* fmov.s fr2,@-r8 */
L8c61ed7a: sa_motion_publish_stop(ctx,0x8c61ed7au); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ed7c; /* mov.l @r2,r3 */
L8c61ed7c: sa_motion_publish_stop(ctx,0x8c61ed7cu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ed7e; /* fmov.s @(r0,r4),fr0 */
L8c61ed7e: sa_motion_publish_stop(ctx,0x8c61ed7eu); r[0]=16u;goto L8c61ed80; /* mov #0x10,r0 */
L8c61ed80: sa_motion_publish_stop(ctx,0x8c61ed80u); r[2]=r[3];goto L8c61ed82; /* mov r3,r2 */
L8c61ed82: sa_motion_publish_stop(ctx,0x8c61ed82u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61ed84; /* fmov.s @(r0,r2),fr3 */
L8c61ed84: sa_motion_publish_stop(ctx,0x8c61ed84u); r[1]=r[3];goto L8c61ed86; /* mov r3,r1 */
L8c61ed86: sa_motion_publish_stop(ctx,0x8c61ed86u); fload(m,bus,&io_status,f,2u,r[1]);if(io_status)return io_status;goto L8c61ed88; /* fmov.s @r1,fr2 */
L8c61ed88: sa_motion_publish_stop(ctx,0x8c61ed88u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ed8a; /* fmul fr0,fr3 */
L8c61ed8a: sa_motion_publish_stop(ctx,0x8c61ed8au); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ed8c; /* fmov.s @r4,fr0 */
L8c61ed8c: sa_motion_publish_stop(ctx,0x8c61ed8cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ed8e; /* fmac fr0,fr2,fr3 */
L8c61ed8e: sa_motion_publish_stop(ctx,0x8c61ed8eu); r[0]=8u;goto L8c61ed90; /* mov #0x8,r0 */
L8c61ed90: sa_motion_publish_stop(ctx,0x8c61ed90u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ed92; /* fmov.s @(r0,r4),fr0 */
L8c61ed92: sa_motion_publish_stop(ctx,0x8c61ed92u); r[0]=32u;goto L8c61ed94; /* mov #0x20,r0 */
L8c61ed94: sa_motion_publish_stop(ctx,0x8c61ed94u); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61ed96; /* fmov.s @(r0,r3),fr2 */
L8c61ed96: sa_motion_publish_stop(ctx,0x8c61ed96u); r[0]=24u;goto L8c61ed98; /* mov #0x18,r0 */
L8c61ed98: sa_motion_publish_stop(ctx,0x8c61ed98u); r[3]=rd(m,bus,&io_status,0x8c61ef60u,4);if(io_status)return io_status;goto L8c61ed9a; /* mov.l 0x8c61ef60,r3 */
L8c61ed9a: sa_motion_publish_stop(ctx,0x8c61ed9au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ed9c; /* fmac fr0,fr2,fr3 */
L8c61ed9c: sa_motion_publish_stop(ctx,0x8c61ed9cu); fcopy(f,0u,14u);goto L8c61ed9e; /* fmov fr14,fr0 */
L8c61ed9e: sa_motion_publish_stop(ctx,0x8c61ed9eu); fcopy(f,2u,14u);goto L8c61eda0; /* fmov fr14,fr2 */
L8c61eda0: sa_motion_publish_stop(ctx,0x8c61eda0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eda2; /* fmac fr0,fr3,fr2 */
L8c61eda2: sa_motion_publish_stop(ctx,0x8c61eda2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61eda4; /* fmov.s fr2,@-r8 */
L8c61eda4: sa_motion_publish_stop(ctx,0x8c61eda4u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61eda6; /* fmov.s @(r0,r4),fr3 */
L8c61eda6: sa_motion_publish_stop(ctx,0x8c61eda6u); r[0]=20u;goto L8c61eda8; /* mov #0x14,r0 */
L8c61eda8: sa_motion_publish_stop(ctx,0x8c61eda8u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61edaa; /* fmov.s fr3,@-r8 */
L8c61edaa: sa_motion_publish_stop(ctx,0x8c61edaau); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61edac; /* fmov.s @(r0,r4),fr3 */
L8c61edac: sa_motion_publish_stop(ctx,0x8c61edacu); r[0]=16u;goto L8c61edae; /* mov #0x10,r0 */
L8c61edae: sa_motion_publish_stop(ctx,0x8c61edaeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61edb0; /* fmov.s fr3,@-r8 */
L8c61edb0: sa_motion_publish_stop(ctx,0x8c61edb0u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61edb2; /* fmov.s @(r0,r4),fr3 */
L8c61edb2: sa_motion_publish_stop(ctx,0x8c61edb2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61edb4; /* fmov.s fr3,@-r8 */
L8c61edb4: sa_motion_publish_stop(ctx,0x8c61edb4u); r[8]-=4;wr(m,bus,&io_status,r[8],r[3],4);if(io_status)return io_status;goto L8c61edb6; /* mov.l r3,@-r8 */
L8c61edb6: sa_motion_publish_stop(ctx,0x8c61edb6u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61edb8; /* pref @r8 */
L8c61edb8: sa_motion_publish_stop(ctx,0x8c61edb8u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61edba; /* mov.w @r10+,r4 */
L8c61edba: sa_motion_publish_stop(ctx,0x8c61edbau); r[8]+=56u;goto L8c61edbc; /* add #0x38,r8 */
L8c61edbc: sa_motion_publish_stop(ctx,0x8c61edbcu); r[2]=rd(m,bus,&io_status,0x8c61ef58u,4);if(io_status)return io_status;goto L8c61edbe; /* mov.l 0x8c61ef58,r2 */
L8c61edbe: sa_motion_publish_stop(ctx,0x8c61edbeu); r[4]=r[4]&65535u;goto L8c61edc0; /* extu.w r4,r4 */
L8c61edc0: sa_motion_publish_stop(ctx,0x8c61edc0u); r[0]=rd(m,bus,&io_status,0x8c61ef5cu,4);if(io_status)return io_status;goto L8c61edc2; /* mov.l 0x8c61ef5c,r0 */
L8c61edc2: sa_motion_publish_stop(ctx,0x8c61edc2u); r[4]<<=2;goto L8c61edc4; /* shll2 r4 */
L8c61edc4: sa_motion_publish_stop(ctx,0x8c61edc4u); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61edc6; /* mov.l @r2,r3 */
L8c61edc6: sa_motion_publish_stop(ctx,0x8c61edc6u); r[4]<<=2;goto L8c61edc8; /* shll2 r4 */
L8c61edc8: sa_motion_publish_stop(ctx,0x8c61edc8u); r[1]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61edca; /* mov.l @r0,r1 */
L8c61edca: sa_motion_publish_stop(ctx,0x8c61edcau); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61edcc; /* shll r4 */
L8c61edcc: sa_motion_publish_stop(ctx,0x8c61edccu); r[0]=4u;goto L8c61edce; /* mov #0x4,r0 */
L8c61edce: sa_motion_publish_stop(ctx,0x8c61edceu); r[2]=r[1];goto L8c61edd0; /* mov r1,r2 */
L8c61edd0: sa_motion_publish_stop(ctx,0x8c61edd0u); r[4]+=r[3];goto L8c61edd2; /* add r3,r4 */
L8c61edd2: sa_motion_publish_stop(ctx,0x8c61edd2u); r[3]=r[1];goto L8c61edd4; /* mov r1,r3 */
L8c61edd4: sa_motion_publish_stop(ctx,0x8c61edd4u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61edd6; /* fmov.s @(r0,r4),fr0 */
L8c61edd6: sa_motion_publish_stop(ctx,0x8c61edd6u); r[0]=20u;goto L8c61edd8; /* mov #0x14,r0 */
L8c61edd8: sa_motion_publish_stop(ctx,0x8c61edd8u); fload(m,bus,&io_status,f,3u,r[0]+r[2]);if(io_status)return io_status;goto L8c61edda; /* fmov.s @(r0,r2),fr3 */
L8c61edda: sa_motion_publish_stop(ctx,0x8c61eddau); r[0]=4u;goto L8c61eddc; /* mov #0x4,r0 */
L8c61eddc: sa_motion_publish_stop(ctx,0x8c61eddcu); fload(m,bus,&io_status,f,2u,r[0]+r[3]);if(io_status)return io_status;goto L8c61edde; /* fmov.s @(r0,r3),fr2 */
L8c61edde: sa_motion_publish_stop(ctx,0x8c61eddeu); r[0]=8u;goto L8c61ede0; /* mov #0x8,r0 */
L8c61ede0: sa_motion_publish_stop(ctx,0x8c61ede0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ede2; /* fmul fr0,fr3 */
L8c61ede2: sa_motion_publish_stop(ctx,0x8c61ede2u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ede4; /* fmov.s @r4,fr0 */
L8c61ede4: sa_motion_publish_stop(ctx,0x8c61ede4u); r[3]=rd(m,bus,&io_status,0x8c61ef5cu,4);if(io_status)return io_status;goto L8c61ede6; /* mov.l 0x8c61ef5c,r3 */
L8c61ede6: sa_motion_publish_stop(ctx,0x8c61ede6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ede8; /* fmac fr0,fr2,fr3 */
L8c61ede8: sa_motion_publish_stop(ctx,0x8c61ede8u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61edea; /* fmov.s @(r0,r4),fr0 */
L8c61edea: sa_motion_publish_stop(ctx,0x8c61edeau); r[0]=36u;goto L8c61edec; /* mov #0x24,r0 */
L8c61edec: sa_motion_publish_stop(ctx,0x8c61edecu); fload(m,bus,&io_status,f,2u,r[0]+r[1]);if(io_status)return io_status;goto L8c61edee; /* fmov.s @(r0,r1),fr2 */
L8c61edee: sa_motion_publish_stop(ctx,0x8c61edeeu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61edf0; /* fmac fr0,fr2,fr3 */
L8c61edf0: sa_motion_publish_stop(ctx,0x8c61edf0u); fcopy(f,0u,14u);goto L8c61edf2; /* fmov fr14,fr0 */
L8c61edf2: sa_motion_publish_stop(ctx,0x8c61edf2u); fcopy(f,2u,14u);goto L8c61edf4; /* fmov fr14,fr2 */
L8c61edf4: sa_motion_publish_stop(ctx,0x8c61edf4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61edf6; /* fmac fr0,fr3,fr2 */
L8c61edf6: sa_motion_publish_stop(ctx,0x8c61edf6u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61edf8; /* fmov.s fr2,@-r8 */
L8c61edf8: sa_motion_publish_stop(ctx,0x8c61edf8u); r[0]=4u;goto L8c61edfa; /* mov #0x4,r0 */
L8c61edfa: sa_motion_publish_stop(ctx,0x8c61edfau); r[11]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61edfc; /* mov.l @r3,r11 */
L8c61edfc: sa_motion_publish_stop(ctx,0x8c61edfcu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61edfe; /* fmov.s @(r0,r4),fr0 */
L8c61edfe: sa_motion_publish_stop(ctx,0x8c61edfeu); r[0]=16u;goto L8c61ee00; /* mov #0x10,r0 */
L8c61ee00: sa_motion_publish_stop(ctx,0x8c61ee00u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61ee02; /* fmov.s @(r0,r11),fr3 */
L8c61ee02: sa_motion_publish_stop(ctx,0x8c61ee02u); r[0]=8u;goto L8c61ee04; /* mov #0x8,r0 */
L8c61ee04: sa_motion_publish_stop(ctx,0x8c61ee04u); fload(m,bus,&io_status,f,2u,r[11]);if(io_status)return io_status;goto L8c61ee06; /* fmov.s @r11,fr2 */
L8c61ee06: sa_motion_publish_stop(ctx,0x8c61ee06u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee08; /* fmul fr0,fr3 */
L8c61ee08: sa_motion_publish_stop(ctx,0x8c61ee08u); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ee0a; /* fmov.s @r4,fr0 */
L8c61ee0a: sa_motion_publish_stop(ctx,0x8c61ee0au); r[2]=rd(m,bus,&io_status,0x8c61ef60u,4);if(io_status)return io_status;goto L8c61ee0c; /* mov.l 0x8c61ef60,r2 */
L8c61ee0c: sa_motion_publish_stop(ctx,0x8c61ee0cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee0e; /* fmac fr0,fr2,fr3 */
L8c61ee0e: sa_motion_publish_stop(ctx,0x8c61ee0eu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee10; /* fmov.s @(r0,r4),fr0 */
L8c61ee10: sa_motion_publish_stop(ctx,0x8c61ee10u); r[0]=32u;goto L8c61ee12; /* mov #0x20,r0 */
L8c61ee12: sa_motion_publish_stop(ctx,0x8c61ee12u); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61ee14; /* fmov.s @(r0,r11),fr2 */
L8c61ee14: sa_motion_publish_stop(ctx,0x8c61ee14u); r[0]=24u;goto L8c61ee16; /* mov #0x18,r0 */
L8c61ee16: sa_motion_publish_stop(ctx,0x8c61ee16u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee18; /* fmac fr0,fr2,fr3 */
L8c61ee18: sa_motion_publish_stop(ctx,0x8c61ee18u); fcopy(f,0u,14u);goto L8c61ee1a; /* fmov fr14,fr0 */
L8c61ee1a: sa_motion_publish_stop(ctx,0x8c61ee1au); fcopy(f,2u,14u);goto L8c61ee1c; /* fmov fr14,fr2 */
L8c61ee1c: sa_motion_publish_stop(ctx,0x8c61ee1cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ee1e; /* fmac fr0,fr3,fr2 */
L8c61ee1e: sa_motion_publish_stop(ctx,0x8c61ee1eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ee20; /* fmov.s fr2,@-r8 */
L8c61ee20: sa_motion_publish_stop(ctx,0x8c61ee20u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee22; /* fmov.s @(r0,r4),fr3 */
L8c61ee22: sa_motion_publish_stop(ctx,0x8c61ee22u); r[0]=20u;goto L8c61ee24; /* mov #0x14,r0 */
L8c61ee24: sa_motion_publish_stop(ctx,0x8c61ee24u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ee26; /* fmov.s fr3,@-r8 */
L8c61ee26: sa_motion_publish_stop(ctx,0x8c61ee26u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee28; /* fmov.s @(r0,r4),fr3 */
L8c61ee28: sa_motion_publish_stop(ctx,0x8c61ee28u); r[0]=16u;goto L8c61ee2a; /* mov #0x10,r0 */
L8c61ee2a: sa_motion_publish_stop(ctx,0x8c61ee2au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ee2c; /* fmov.s fr3,@-r8 */
L8c61ee2c: sa_motion_publish_stop(ctx,0x8c61ee2cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee2e; /* fmov.s @(r0,r4),fr3 */
L8c61ee2e: sa_motion_publish_stop(ctx,0x8c61ee2eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ee30; /* fmov.s fr3,@-r8 */
L8c61ee30: sa_motion_publish_stop(ctx,0x8c61ee30u); r[8]-=4;wr(m,bus,&io_status,r[8],r[2],4);if(io_status)return io_status;goto L8c61ee32; /* mov.l r2,@-r8 */
L8c61ee32: sa_motion_publish_stop(ctx,0x8c61ee32u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ee34; /* pref @r8 */
L8c61ee34: sa_motion_publish_stop(ctx,0x8c61ee34u); r[6]=r[9];goto L8c61ee36; /* mov r9,r6 */
L8c61ee36: sa_motion_publish_stop(ctx,0x8c61ee36u); r[4]=2u;goto L8c61ee38; /* mov #0x2,r4 */
L8c61ee38: sa_motion_publish_stop(ctx,0x8c61ee38u); r[6]+=4294967295u;goto L8c61ee3a; /* add #-0x1,r6 */
L8c61ee3a: sa_motion_publish_stop(ctx,0x8c61ee3au); r[5]=r[4];goto L8c61ee3c; /* mov r4,r5 */
L8c61ee3c: sa_motion_publish_stop(ctx,0x8c61ee3cu); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61ee3e; /* cmp/ge r6,r5 */
L8c61ee3e: sa_motion_publish_stop(ctx,0x8c61ee40u); take=(*sr&1u);r[8]+=32u;if(take)goto L8c61eec2;goto L8c61ee42; /* bt/s 0x8c61eec2 */
L8c61ee42: sa_motion_publish_stop(ctx,0x8c61ee42u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61ee44; /* mov.w @r10+,r4 */
L8c61ee44: sa_motion_publish_stop(ctx,0x8c61ee44u); r[0]=4u;goto L8c61ee46; /* mov #0x4,r0 */
L8c61ee46: sa_motion_publish_stop(ctx,0x8c61ee46u); r[2]=rd(m,bus,&io_status,0x8c61ef58u,4);if(io_status)return io_status;goto L8c61ee48; /* mov.l 0x8c61ef58,r2 */
L8c61ee48: sa_motion_publish_stop(ctx,0x8c61ee48u); r[8]+=28u;goto L8c61ee4a; /* add #0x1c,r8 */
L8c61ee4a: sa_motion_publish_stop(ctx,0x8c61ee4au); r[4]=r[4]&65535u;goto L8c61ee4c; /* extu.w r4,r4 */
L8c61ee4c: sa_motion_publish_stop(ctx,0x8c61ee4cu); r[4]<<=2;goto L8c61ee4e; /* shll2 r4 */
L8c61ee4e: sa_motion_publish_stop(ctx,0x8c61ee4eu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61ee50; /* mov.l @r2,r3 */
L8c61ee50: sa_motion_publish_stop(ctx,0x8c61ee50u); r[4]<<=2;goto L8c61ee52; /* shll2 r4 */
L8c61ee52: sa_motion_publish_stop(ctx,0x8c61ee52u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61ee54; /* fmov.s fr4,@-r8 */
L8c61ee54: sa_motion_publish_stop(ctx,0x8c61ee54u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61ee56; /* shll r4 */
L8c61ee56: sa_motion_publish_stop(ctx,0x8c61ee56u); r[4]+=r[3];goto L8c61ee58; /* add r3,r4 */
L8c61ee58: sa_motion_publish_stop(ctx,0x8c61ee58u); r[3]=rd(m,bus,&io_status,0x8c61ef5cu,4);if(io_status)return io_status;goto L8c61ee5a; /* mov.l 0x8c61ef5c,r3 */
L8c61ee5a: sa_motion_publish_stop(ctx,0x8c61ee5au); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee5c; /* fmov.s @(r0,r4),fr0 */
L8c61ee5c: sa_motion_publish_stop(ctx,0x8c61ee5cu); r[0]=20u;goto L8c61ee5e; /* mov #0x14,r0 */
L8c61ee5e: sa_motion_publish_stop(ctx,0x8c61ee5eu); r[11]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ee60; /* mov.l @r3,r11 */
L8c61ee60: sa_motion_publish_stop(ctx,0x8c61ee60u); fload(m,bus,&io_status,f,3u,r[0]+r[11]);if(io_status)return io_status;goto L8c61ee62; /* fmov.s @(r0,r11),fr3 */
L8c61ee62: sa_motion_publish_stop(ctx,0x8c61ee62u); r[0]=4u;goto L8c61ee64; /* mov #0x4,r0 */
L8c61ee64: sa_motion_publish_stop(ctx,0x8c61ee64u); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61ee66; /* fmov.s @(r0,r11),fr2 */
L8c61ee66: sa_motion_publish_stop(ctx,0x8c61ee66u); r[0]=8u;goto L8c61ee68; /* mov #0x8,r0 */
L8c61ee68: sa_motion_publish_stop(ctx,0x8c61ee68u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee6a; /* fmul fr0,fr3 */
L8c61ee6a: sa_motion_publish_stop(ctx,0x8c61ee6au); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ee6c; /* fmov.s @r4,fr0 */
L8c61ee6c: sa_motion_publish_stop(ctx,0x8c61ee6cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee6e; /* fmac fr0,fr2,fr3 */
L8c61ee6e: sa_motion_publish_stop(ctx,0x8c61ee6eu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee70; /* fmov.s @(r0,r4),fr0 */
L8c61ee70: sa_motion_publish_stop(ctx,0x8c61ee70u); r[0]=36u;goto L8c61ee72; /* mov #0x24,r0 */
L8c61ee72: sa_motion_publish_stop(ctx,0x8c61ee72u); fload(m,bus,&io_status,f,2u,r[0]+r[11]);if(io_status)return io_status;goto L8c61ee74; /* fmov.s @(r0,r11),fr2 */
L8c61ee74: sa_motion_publish_stop(ctx,0x8c61ee74u); r[0]=4u;goto L8c61ee76; /* mov #0x4,r0 */
L8c61ee76: sa_motion_publish_stop(ctx,0x8c61ee76u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee78; /* fmac fr0,fr2,fr3 */
L8c61ee78: sa_motion_publish_stop(ctx,0x8c61ee78u); fcopy(f,2u,14u);goto L8c61ee7a; /* fmov fr14,fr2 */
L8c61ee7a: sa_motion_publish_stop(ctx,0x8c61ee7au); fcopy(f,0u,14u);goto L8c61ee7c; /* fmov fr14,fr0 */
L8c61ee7c: sa_motion_publish_stop(ctx,0x8c61ee7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ee7e; /* fmac fr0,fr3,fr2 */
L8c61ee7e: sa_motion_publish_stop(ctx,0x8c61ee7eu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ee80; /* fmov.s fr2,@-r8 */
L8c61ee80: sa_motion_publish_stop(ctx,0x8c61ee80u); r[7]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ee82; /* mov.l @r3,r7 */
L8c61ee82: sa_motion_publish_stop(ctx,0x8c61ee82u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee84; /* fmov.s @(r0,r4),fr0 */
L8c61ee84: sa_motion_publish_stop(ctx,0x8c61ee84u); r[0]=16u;goto L8c61ee86; /* mov #0x10,r0 */
L8c61ee86: sa_motion_publish_stop(ctx,0x8c61ee86u); fload(m,bus,&io_status,f,3u,r[0]+r[7]);if(io_status)return io_status;goto L8c61ee88; /* fmov.s @(r0,r7),fr3 */
L8c61ee88: sa_motion_publish_stop(ctx,0x8c61ee88u); r[0]=8u;goto L8c61ee8a; /* mov #0x8,r0 */
L8c61ee8a: sa_motion_publish_stop(ctx,0x8c61ee8au); fload(m,bus,&io_status,f,2u,r[7]);if(io_status)return io_status;goto L8c61ee8c; /* fmov.s @r7,fr2 */
L8c61ee8c: sa_motion_publish_stop(ctx,0x8c61ee8cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee8e; /* fmul fr0,fr3 */
L8c61ee8e: sa_motion_publish_stop(ctx,0x8c61ee8eu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ee90; /* fmov.s @r4,fr0 */
L8c61ee90: sa_motion_publish_stop(ctx,0x8c61ee90u); r[1]=rd(m,bus,&io_status,0x8c61ef60u,4);if(io_status)return io_status;goto L8c61ee92; /* mov.l 0x8c61ef60,r1 */
L8c61ee92: sa_motion_publish_stop(ctx,0x8c61ee92u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee94; /* fmac fr0,fr2,fr3 */
L8c61ee94: sa_motion_publish_stop(ctx,0x8c61ee94u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ee96; /* fmov.s @(r0,r4),fr0 */
L8c61ee96: sa_motion_publish_stop(ctx,0x8c61ee96u); r[0]=32u;goto L8c61ee98; /* mov #0x20,r0 */
L8c61ee98: sa_motion_publish_stop(ctx,0x8c61ee98u); fload(m,bus,&io_status,f,2u,r[0]+r[7]);if(io_status)return io_status;goto L8c61ee9a; /* fmov.s @(r0,r7),fr2 */
L8c61ee9a: sa_motion_publish_stop(ctx,0x8c61ee9au); r[0]=24u;goto L8c61ee9c; /* mov #0x18,r0 */
L8c61ee9c: sa_motion_publish_stop(ctx,0x8c61ee9cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ee9e; /* fmac fr0,fr2,fr3 */
L8c61ee9e: sa_motion_publish_stop(ctx,0x8c61ee9eu); fcopy(f,0u,14u);goto L8c61eea0; /* fmov fr14,fr0 */
L8c61eea0: sa_motion_publish_stop(ctx,0x8c61eea0u); fcopy(f,2u,14u);goto L8c61eea2; /* fmov fr14,fr2 */
L8c61eea2: sa_motion_publish_stop(ctx,0x8c61eea2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eea4; /* fmac fr0,fr3,fr2 */
L8c61eea4: sa_motion_publish_stop(ctx,0x8c61eea4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61eea6; /* fmov.s fr2,@-r8 */
L8c61eea6: sa_motion_publish_stop(ctx,0x8c61eea6u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61eea8; /* fmov.s @(r0,r4),fr3 */
L8c61eea8: sa_motion_publish_stop(ctx,0x8c61eea8u); r[0]=20u;goto L8c61eeaa; /* mov #0x14,r0 */
L8c61eeaa: sa_motion_publish_stop(ctx,0x8c61eeaau); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eeac; /* fmov.s fr3,@-r8 */
L8c61eeac: sa_motion_publish_stop(ctx,0x8c61eeacu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61eeae; /* fmov.s @(r0,r4),fr3 */
L8c61eeae: sa_motion_publish_stop(ctx,0x8c61eeaeu); r[0]=16u;goto L8c61eeb0; /* mov #0x10,r0 */
L8c61eeb0: sa_motion_publish_stop(ctx,0x8c61eeb0u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eeb2; /* fmov.s fr3,@-r8 */
L8c61eeb2: sa_motion_publish_stop(ctx,0x8c61eeb2u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61eeb4; /* fmov.s @(r0,r4),fr3 */
L8c61eeb4: sa_motion_publish_stop(ctx,0x8c61eeb4u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61eeb6; /* fmov.s fr3,@-r8 */
L8c61eeb6: sa_motion_publish_stop(ctx,0x8c61eeb6u); r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c61eeb8; /* mov.l r1,@-r8 */
L8c61eeb8: sa_motion_publish_stop(ctx,0x8c61eeb8u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61eeba; /* pref @r8 */
L8c61eeba: sa_motion_publish_stop(ctx,0x8c61eebau); r[5]+=1u;goto L8c61eebc; /* add #0x1,r5 */
L8c61eebc: sa_motion_publish_stop(ctx,0x8c61eebcu); *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[6]);goto L8c61eebe; /* cmp/ge r6,r5 */
L8c61eebe: sa_motion_publish_stop(ctx,0x8c61eec0u); take=!(*sr&1u);r[8]+=32u;if(take)goto L8c61ee42;goto L8c61eec2; /* bf/s 0x8c61ee42 */
L8c61eec2: sa_motion_publish_stop(ctx,0x8c61eec2u); r[4]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61eec4; /* mov.w @r10+,r4 */
L8c61eec4: sa_motion_publish_stop(ctx,0x8c61eec4u); r[0]=4u;goto L8c61eec6; /* mov #0x4,r0 */
L8c61eec6: sa_motion_publish_stop(ctx,0x8c61eec6u); r[2]=rd(m,bus,&io_status,0x8c61ef58u,4);if(io_status)return io_status;goto L8c61eec8; /* mov.l 0x8c61ef58,r2 */
L8c61eec8: sa_motion_publish_stop(ctx,0x8c61eec8u); r[8]+=28u;goto L8c61eeca; /* add #0x1c,r8 */
L8c61eeca: sa_motion_publish_stop(ctx,0x8c61eecau); r[4]=r[4]&65535u;goto L8c61eecc; /* extu.w r4,r4 */
L8c61eecc: sa_motion_publish_stop(ctx,0x8c61eeccu); r[4]<<=2;goto L8c61eece; /* shll2 r4 */
L8c61eece: sa_motion_publish_stop(ctx,0x8c61eeceu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61eed0; /* mov.l @r2,r3 */
L8c61eed0: sa_motion_publish_stop(ctx,0x8c61eed0u); r[4]<<=2;goto L8c61eed2; /* shll2 r4 */
L8c61eed2: sa_motion_publish_stop(ctx,0x8c61eed2u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[8]);if(io_status)return io_status;goto L8c61eed4; /* fmov.s fr4,@-r8 */
L8c61eed4: sa_motion_publish_stop(ctx,0x8c61eed4u); *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto L8c61eed6; /* shll r4 */
L8c61eed6: sa_motion_publish_stop(ctx,0x8c61eed6u); r[4]+=r[3];goto L8c61eed8; /* add r3,r4 */
L8c61eed8: sa_motion_publish_stop(ctx,0x8c61eed8u); r[3]=rd(m,bus,&io_status,0x8c61ef5cu,4);if(io_status)return io_status;goto L8c61eeda; /* mov.l 0x8c61ef5c,r3 */
L8c61eeda: sa_motion_publish_stop(ctx,0x8c61eedau); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61eedc; /* fmov.s @(r0,r4),fr0 */
L8c61eedc: sa_motion_publish_stop(ctx,0x8c61eedcu); r[0]=20u;goto L8c61eede; /* mov #0x14,r0 */
L8c61eede: sa_motion_publish_stop(ctx,0x8c61eedeu); r[6]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61eee0; /* mov.l @r3,r6 */
L8c61eee0: sa_motion_publish_stop(ctx,0x8c61eee0u); fload(m,bus,&io_status,f,3u,r[0]+r[6]);if(io_status)return io_status;goto L8c61eee2; /* fmov.s @(r0,r6),fr3 */
L8c61eee2: sa_motion_publish_stop(ctx,0x8c61eee2u); r[0]=4u;goto L8c61eee4; /* mov #0x4,r0 */
L8c61eee4: sa_motion_publish_stop(ctx,0x8c61eee4u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61eee6; /* fmov.s @(r0,r6),fr2 */
L8c61eee6: sa_motion_publish_stop(ctx,0x8c61eee6u); r[0]=8u;goto L8c61eee8; /* mov #0x8,r0 */
L8c61eee8: sa_motion_publish_stop(ctx,0x8c61eee8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eeea; /* fmul fr0,fr3 */
L8c61eeea: sa_motion_publish_stop(ctx,0x8c61eeeau); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61eeec; /* fmov.s @r4,fr0 */
L8c61eeec: sa_motion_publish_stop(ctx,0x8c61eeecu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eeee; /* fmac fr0,fr2,fr3 */
L8c61eeee: sa_motion_publish_stop(ctx,0x8c61eeeeu); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61eef0; /* fmov.s @(r0,r4),fr0 */
L8c61eef0: sa_motion_publish_stop(ctx,0x8c61eef0u); r[0]=36u;goto L8c61eef2; /* mov #0x24,r0 */
L8c61eef2: sa_motion_publish_stop(ctx,0x8c61eef2u); fload(m,bus,&io_status,f,2u,r[0]+r[6]);if(io_status)return io_status;goto L8c61eef4; /* fmov.s @(r0,r6),fr2 */
L8c61eef4: sa_motion_publish_stop(ctx,0x8c61eef4u); r[0]=4u;goto L8c61eef6; /* mov #0x4,r0 */
L8c61eef6: sa_motion_publish_stop(ctx,0x8c61eef6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61eef8; /* fmac fr0,fr2,fr3 */
L8c61eef8: sa_motion_publish_stop(ctx,0x8c61eef8u); fcopy(f,2u,14u);goto L8c61eefa; /* fmov fr14,fr2 */
L8c61eefa: sa_motion_publish_stop(ctx,0x8c61eefau); fcopy(f,0u,14u);goto L8c61eefc; /* fmov fr14,fr0 */
L8c61eefc: sa_motion_publish_stop(ctx,0x8c61eefcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61eefe; /* fmac fr0,fr3,fr2 */
L8c61eefe: sa_motion_publish_stop(ctx,0x8c61eefeu); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ef00; /* fmov.s fr2,@-r8 */
L8c61ef00: sa_motion_publish_stop(ctx,0x8c61ef00u); r[5]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;goto L8c61ef02; /* mov.l @r3,r5 */
L8c61ef02: sa_motion_publish_stop(ctx,0x8c61ef02u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ef04; /* fmov.s @(r0,r4),fr0 */
L8c61ef04: sa_motion_publish_stop(ctx,0x8c61ef04u); r[0]=16u;goto L8c61ef06; /* mov #0x10,r0 */
L8c61ef06: sa_motion_publish_stop(ctx,0x8c61ef06u); fload(m,bus,&io_status,f,3u,r[0]+r[5]);if(io_status)return io_status;goto L8c61ef08; /* fmov.s @(r0,r5),fr3 */
L8c61ef08: sa_motion_publish_stop(ctx,0x8c61ef08u); r[0]=8u;goto L8c61ef0a; /* mov #0x8,r0 */
L8c61ef0a: sa_motion_publish_stop(ctx,0x8c61ef0au); fload(m,bus,&io_status,f,2u,r[5]);if(io_status)return io_status;goto L8c61ef0c; /* fmov.s @r5,fr2 */
L8c61ef0c: sa_motion_publish_stop(ctx,0x8c61ef0cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[0],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ef0e; /* fmul fr0,fr3 */
L8c61ef0e: sa_motion_publish_stop(ctx,0x8c61ef0eu); fload(m,bus,&io_status,f,0u,r[4]);if(io_status)return io_status;goto L8c61ef10; /* fmov.s @r4,fr0 */
L8c61ef10: sa_motion_publish_stop(ctx,0x8c61ef10u); r[1]=rd(m,bus,&io_status,0x8c61ef64u,4);if(io_status)return io_status;goto L8c61ef12; /* mov.l 0x8c61ef64,r1 */
L8c61ef12: sa_motion_publish_stop(ctx,0x8c61ef12u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ef14; /* fmac fr0,fr2,fr3 */
L8c61ef14: sa_motion_publish_stop(ctx,0x8c61ef14u); fload(m,bus,&io_status,f,0u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ef16; /* fmov.s @(r0,r4),fr0 */
L8c61ef16: sa_motion_publish_stop(ctx,0x8c61ef16u); r[0]=32u;goto L8c61ef18; /* mov #0x20,r0 */
L8c61ef18: sa_motion_publish_stop(ctx,0x8c61ef18u); fload(m,bus,&io_status,f,2u,r[0]+r[5]);if(io_status)return io_status;goto L8c61ef1a; /* fmov.s @(r0,r5),fr2 */
L8c61ef1a: sa_motion_publish_stop(ctx,0x8c61ef1au); r[0]=24u;goto L8c61ef1c; /* mov #0x18,r0 */
L8c61ef1c: sa_motion_publish_stop(ctx,0x8c61ef1cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[2],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c61ef1e; /* fmac fr0,fr2,fr3 */
L8c61ef1e: sa_motion_publish_stop(ctx,0x8c61ef1eu); fcopy(f,0u,14u);goto L8c61ef20; /* fmov fr14,fr0 */
L8c61ef20: sa_motion_publish_stop(ctx,0x8c61ef20u); fcopy(f,2u,14u);goto L8c61ef22; /* fmov fr14,fr2 */
L8c61ef22: sa_motion_publish_stop(ctx,0x8c61ef22u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[3],&fr[2],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61ef24; /* fmac fr0,fr3,fr2 */
L8c61ef24: sa_motion_publish_stop(ctx,0x8c61ef24u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[8]);if(io_status)return io_status;goto L8c61ef26; /* fmov.s fr2,@-r8 */
L8c61ef26: sa_motion_publish_stop(ctx,0x8c61ef26u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ef28; /* fmov.s @(r0,r4),fr3 */
L8c61ef28: sa_motion_publish_stop(ctx,0x8c61ef28u); r[0]=20u;goto L8c61ef2a; /* mov #0x14,r0 */
L8c61ef2a: sa_motion_publish_stop(ctx,0x8c61ef2au); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ef2c; /* fmov.s fr3,@-r8 */
L8c61ef2c: sa_motion_publish_stop(ctx,0x8c61ef2cu); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ef2e; /* fmov.s @(r0,r4),fr3 */
L8c61ef2e: sa_motion_publish_stop(ctx,0x8c61ef2eu); r[0]=16u;goto L8c61ef30; /* mov #0x10,r0 */
L8c61ef30: sa_motion_publish_stop(ctx,0x8c61ef30u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ef32; /* fmov.s fr3,@-r8 */
L8c61ef32: sa_motion_publish_stop(ctx,0x8c61ef32u); fload(m,bus,&io_status,f,3u,r[0]+r[4]);if(io_status)return io_status;goto L8c61ef34; /* fmov.s @(r0,r4),fr3 */
L8c61ef34: sa_motion_publish_stop(ctx,0x8c61ef34u); r[8]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[8]);if(io_status)return io_status;goto L8c61ef36; /* fmov.s fr3,@-r8 */
L8c61ef36: sa_motion_publish_stop(ctx,0x8c61ef36u); r[8]-=4;wr(m,bus,&io_status,r[8],r[1],4);if(io_status)return io_status;goto L8c61ef38; /* mov.l r1,@-r8 */
L8c61ef38: sa_motion_publish_stop(ctx,0x8c61ef38u); if(!bus||!bus->cache)return -2;status=bus->cache(bus->context,r[8],1u);if(status)return status;goto L8c61ef3a; /* pref @r8 */
L8c61ef3a: sa_motion_publish_stop(ctx,0x8c61ef3cu); r[8]+=32u;goto L8c61f028; /* bra 0x8c61f028 */
L8c61ef68: sa_motion_publish_stop(ctx,0x8c61ef68u); r[0]=12u;goto L8c61ef6a; /* mov #0xc,r0 */
L8c61ef6a: sa_motion_publish_stop(ctx,0x8c61ef6au); r[2]=rd(m,bus,&io_status,0x8c61f0e4u,4);if(io_status)return io_status;goto L8c61ef6c; /* mov.l 0x8c61f0e4,r2 */
L8c61ef6c: sa_motion_publish_stop(ctx,0x8c61ef6cu); r[3]=0u;goto L8c61ef6e; /* mov #0x0,r3 */
L8c61ef6e: sa_motion_publish_stop(ctx,0x8c61ef6eu); r[4]=r[15];goto L8c61ef70; /* mov r15,r4 */
L8c61ef70: sa_motion_publish_stop(ctx,0x8c61ef70u); wr(m,bus,&io_status,r[2],r[3],4);if(io_status)return io_status;goto L8c61ef72; /* mov.l r3,@r2 */
L8c61ef72: sa_motion_publish_stop(ctx,0x8c61ef72u); if((r[14]&0x1f000000u)!=0x0c000000u)return -3;fload(m,bus,&io_status,f,3u,r[14]);if(io_status)return io_status;goto L8c61ef74; /* fmov.s @r14,fr3 */
L8c61ef74: sa_motion_publish_stop(ctx,0x8c61ef74u); fload(m,bus,&io_status,f,2u,r[12]);if(io_status)return io_status;goto L8c61ef76; /* fmov.s @r12,fr2 */
L8c61ef76: sa_motion_publish_stop(ctx,0x8c61ef76u); r[3]=rd(m,bus,&io_status,0x8c61f0e8u,4);if(io_status)return io_status;goto L8c61ef78; /* mov.l 0x8c61f0e8,r3 */
L8c61ef78: sa_motion_publish_stop(ctx,0x8c61ef78u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ef7a; /* fadd fr3,fr2 */
L8c61ef7a: sa_motion_publish_stop(ctx,0x8c61ef7au); fload(m,bus,&io_status,f,3u,r[13]);if(io_status)return io_status;goto L8c61ef7c; /* fmov.s @r13,fr3 */
L8c61ef7c: sa_motion_publish_stop(ctx,0x8c61ef7cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ef7e; /* fadd fr3,fr2 */
L8c61ef7e: sa_motion_publish_stop(ctx,0x8c61ef7eu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ef80; /* fmov fr2,@(r0,r15) */
L8c61ef80: sa_motion_publish_stop(ctx,0x8c61ef80u); r[0]=4u;goto L8c61ef82; /* mov #0x4,r0 */
L8c61ef82: sa_motion_publish_stop(ctx,0x8c61ef82u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ef84; /* fmov.s @(r0,r14),fr3 */
L8c61ef84: sa_motion_publish_stop(ctx,0x8c61ef84u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61ef86; /* fmov.s @(r0,r12),fr2 */
L8c61ef86: sa_motion_publish_stop(ctx,0x8c61ef86u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ef88; /* fadd fr3,fr2 */
L8c61ef88: sa_motion_publish_stop(ctx,0x8c61ef88u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ef8a; /* fmov.s @(r0,r13),fr3 */
L8c61ef8a: sa_motion_publish_stop(ctx,0x8c61ef8au); r[0]=16u;goto L8c61ef8c; /* mov #0x10,r0 */
L8c61ef8c: sa_motion_publish_stop(ctx,0x8c61ef8cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ef8e; /* fadd fr3,fr2 */
L8c61ef8e: sa_motion_publish_stop(ctx,0x8c61ef8eu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61ef90; /* fmov fr2,@(r0,r15) */
L8c61ef90: sa_motion_publish_stop(ctx,0x8c61ef90u); r[0]=8u;goto L8c61ef92; /* mov #0x8,r0 */
L8c61ef92: sa_motion_publish_stop(ctx,0x8c61ef92u); fload(m,bus,&io_status,f,3u,r[0]+r[14]);if(io_status)return io_status;goto L8c61ef94; /* fmov.s @(r0,r14),fr3 */
L8c61ef94: sa_motion_publish_stop(ctx,0x8c61ef94u); fload(m,bus,&io_status,f,2u,r[0]+r[12]);if(io_status)return io_status;goto L8c61ef96; /* fmov.s @(r0,r12),fr2 */
L8c61ef96: sa_motion_publish_stop(ctx,0x8c61ef96u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ef98; /* fadd fr3,fr2 */
L8c61ef98: sa_motion_publish_stop(ctx,0x8c61ef98u); fload(m,bus,&io_status,f,3u,r[0]+r[13]);if(io_status)return io_status;goto L8c61ef9a; /* fmov.s @(r0,r13),fr3 */
L8c61ef9a: sa_motion_publish_stop(ctx,0x8c61ef9au); r[0]=20u;goto L8c61ef9c; /* mov #0x14,r0 */
L8c61ef9c: sa_motion_publish_stop(ctx,0x8c61ef9cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c61ef9e; /* fadd fr3,fr2 */
L8c61ef9e: sa_motion_publish_stop(ctx,0x8c61ef9eu); fstore(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61efa0; /* fmov fr2,@(r0,r15) */
L8c61efa0: sa_motion_publish_stop(ctx,0x8c61efa2u); target=r[3];*pr=0x8c61efa4u;r[4]+=12u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61efa4; /* jsr @r3 */
L8c61efa4: sa_motion_publish_stop(ctx,0x8c61efa4u); r[3]=rd(m,bus,&io_status,0x8c61f0ecu,4);if(io_status)return io_status;goto L8c61efa6; /* mov.l 0x8c61f0ec,r3 */
L8c61efa6: sa_motion_publish_stop(ctx,0x8c61efa6u); r[0]=12u;goto L8c61efa8; /* mov #0xc,r0 */
L8c61efa8: sa_motion_publish_stop(ctx,0x8c61efa8u); fload(m,bus,&io_status,f,3u,r[0]+r[15]);if(io_status)return io_status;goto L8c61efaa; /* fmov.s @(r0,r15),fr3 */
L8c61efaa: sa_motion_publish_stop(ctx,0x8c61efaau); r[0]=16u;goto L8c61efac; /* mov #0x10,r0 */
L8c61efac: sa_motion_publish_stop(ctx,0x8c61efacu); fload(m,bus,&io_status,f,2u,r[3]);if(io_status)return io_status;goto L8c61efae; /* fmov.s @r3,fr2 */
L8c61efae: sa_motion_publish_stop(ctx,0x8c61efaeu); r[3]+=8u;goto L8c61efb0; /* add #0x8,r3 */
L8c61efb0: sa_motion_publish_stop(ctx,0x8c61efb0u); r[2]=rd(m,bus,&io_status,0x8c61f0f0u,4);if(io_status)return io_status;goto L8c61efb2; /* mov.l 0x8c61f0f0,r2 */
L8c61efb2: sa_motion_publish_stop(ctx,0x8c61efb2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[3],&f->fpscr,&fr[2]);if(status)return status;}goto L8c61efb4; /* fmul fr3,fr2 */
L8c61efb4: sa_motion_publish_stop(ctx,0x8c61efb4u); r[1]=rd(m,bus,&io_status,0x8c61f0f4u,4);if(io_status)return io_status;goto L8c61efb6; /* mov.l 0x8c61f0f4,r1 */
L8c61efb6: sa_motion_publish_stop(ctx,0x8c61efb6u); fload(m,bus,&io_status,f,3u,r[2]);if(io_status)return io_status;goto L8c61efb8; /* fmov.s @r2,fr3 */
L8c61efb8: sa_motion_publish_stop(ctx,0x8c61efb8u); fload(m,bus,&io_status,f,1u,r[1]);if(io_status)return io_status;goto L8c61efba; /* fmov.s @r1,fr1 */
L8c61efba: sa_motion_publish_stop(ctx,0x8c61efbau); r[12]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61efbc; /* mov.w @r10+,r12 */
L8c61efbc: sa_motion_publish_stop(ctx,0x8c61efbcu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[2],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61efbe; /* fsub fr2,fr3 */
L8c61efbe: sa_motion_publish_stop(ctx,0x8c61efbeu); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61efc0; /* fmov.s @(r0,r15),fr2 */
L8c61efc0: sa_motion_publish_stop(ctx,0x8c61efc0u); r[0]=20u;goto L8c61efc2; /* mov #0x14,r0 */
L8c61efc2: sa_motion_publish_stop(ctx,0x8c61efc2u); r[14]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61efc4; /* mov.w @r10+,r14 */
L8c61efc4: sa_motion_publish_stop(ctx,0x8c61efc4u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61efc6; /* fmul fr2,fr1 */
L8c61efc6: sa_motion_publish_stop(ctx,0x8c61efc6u); fload(m,bus,&io_status,f,2u,r[0]+r[15]);if(io_status)return io_status;goto L8c61efc8; /* fmov.s @(r0,r15),fr2 */
L8c61efc8: sa_motion_publish_stop(ctx,0x8c61efc8u); r[12]=r[12]&65535u;goto L8c61efca; /* extu.w r12,r12 */
L8c61efca: sa_motion_publish_stop(ctx,0x8c61efcau); r[0]=rd(m,bus,&io_status,0x8c61f0f8u,4);if(io_status)return io_status;goto L8c61efcc; /* mov.l 0x8c61f0f8,r0 */
L8c61efcc: sa_motion_publish_stop(ctx,0x8c61efccu); r[12]<<=2;goto L8c61efce; /* shll2 r12 */
L8c61efce: sa_motion_publish_stop(ctx,0x8c61efceu); r[2]=rd(m,bus,&io_status,r[0],4);if(io_status)return io_status;goto L8c61efd0; /* mov.l @r0,r2 */
L8c61efd0: sa_motion_publish_stop(ctx,0x8c61efd0u); r[12]<<=2;goto L8c61efd2; /* shll2 r12 */
L8c61efd2: sa_motion_publish_stop(ctx,0x8c61efd2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[1],&f->fpscr,&fr[3],1);if(status)return status;}goto L8c61efd4; /* fsub fr1,fr3 */
L8c61efd4: sa_motion_publish_stop(ctx,0x8c61efd4u); fload(m,bus,&io_status,f,1u,r[3]);if(io_status)return io_status;goto L8c61efd6; /* fmov.s @r3,fr1 */
L8c61efd6: sa_motion_publish_stop(ctx,0x8c61efd6u); r[14]=r[14]&65535u;goto L8c61efd8; /* extu.w r14,r14 */
L8c61efd8: sa_motion_publish_stop(ctx,0x8c61efd8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[2],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61efda; /* fmul fr2,fr1 */
L8c61efda: sa_motion_publish_stop(ctx,0x8c61efdau); *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto L8c61efdc; /* shll r12 */
L8c61efdc: sa_motion_publish_stop(ctx,0x8c61efdcu); r[14]<<=2;goto L8c61efde; /* shll2 r14 */
L8c61efde: sa_motion_publish_stop(ctx,0x8c61efdeu); fcopy(f,15u,3u);goto L8c61efe0; /* fmov fr3,fr15 */
L8c61efe0: sa_motion_publish_stop(ctx,0x8c61efe0u); r[12]+=r[2];goto L8c61efe2; /* add r2,r12 */
L8c61efe2: sa_motion_publish_stop(ctx,0x8c61efe2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[1],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c61efe4; /* fsub fr1,fr15 */
L8c61efe4: sa_motion_publish_stop(ctx,0x8c61efe4u); r[14]<<=2;goto L8c61efe6; /* shll2 r14 */
L8c61efe6: sa_motion_publish_stop(ctx,0x8c61efe6u); r[9]+=4294967294u;goto L8c61efe8; /* add #-0x2,r9 */
L8c61efe8: sa_motion_publish_stop(ctx,0x8c61efe8u); *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1;goto L8c61efea; /* shll r14 */
L8c61efea: sa_motion_publish_stop(ctx,0x8c61efeau); *sr=(*sr&~1u)|((int32_t)r[9]>0);goto L8c61efec; /* cmp/pl r9 */
L8c61efec: sa_motion_publish_stop(ctx,0x8c61efecu); r[14]+=r[2];goto L8c61efee; /* add r2,r14 */
L8c61efee: sa_motion_publish_stop(ctx,0x8c61eff0u); take=!(*sr&1u);r[11]=0u;if(take)goto L8c61f028;goto L8c61eff2; /* bf/s 0x8c61f028 */
L8c61eff2: sa_motion_publish_stop(ctx,0x8c61eff2u); r[13]=sx(m,bus,&io_status,r[10],2);if(io_status)return io_status;r[10]+=2;goto L8c61eff4; /* mov.w @r10+,r13 */
L8c61eff4: sa_motion_publish_stop(ctx,0x8c61eff4u); r[2]=rd(m,bus,&io_status,0x8c61f0f8u,4);if(io_status)return io_status;goto L8c61eff6; /* mov.l 0x8c61f0f8,r2 */
L8c61eff6: sa_motion_publish_stop(ctx,0x8c61eff6u); r[13]=r[13]&65535u;goto L8c61eff8; /* extu.w r13,r13 */
L8c61eff8: sa_motion_publish_stop(ctx,0x8c61eff8u); r[4]=rd(m,bus,&io_status,r[12]+0xcu,4);if(io_status)return io_status;goto L8c61effa; /* mov.l @(0xc,r12),r4 */
L8c61effa: sa_motion_publish_stop(ctx,0x8c61effau); r[13]<<=2;goto L8c61effc; /* shll2 r13 */
L8c61effc: sa_motion_publish_stop(ctx,0x8c61effcu); r[3]=rd(m,bus,&io_status,r[2],4);if(io_status)return io_status;goto L8c61effe; /* mov.l @r2,r3 */
L8c61effe: sa_motion_publish_stop(ctx,0x8c61effeu); r[13]<<=2;goto L8c61f000; /* shll2 r13 */
L8c61f000: sa_motion_publish_stop(ctx,0x8c61f000u); *sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c61f002; /* shll r13 */
L8c61f002: sa_motion_publish_stop(ctx,0x8c61f002u); r[13]+=r[3];goto L8c61f004; /* add r3,r13 */
L8c61f004: sa_motion_publish_stop(ctx,0x8c61f004u); r[3]=rd(m,bus,&io_status,r[14]+0xcu,4);if(io_status)return io_status;goto L8c61f006; /* mov.l @(0xc,r14),r3 */
L8c61f006: sa_motion_publish_stop(ctx,0x8c61f006u); r[1]=rd(m,bus,&io_status,r[13]+0xcu,4);if(io_status)return io_status;goto L8c61f008; /* mov.l @(0xc,r13),r1 */
L8c61f008: sa_motion_publish_stop(ctx,0x8c61f008u); r[4]&=r[3];goto L8c61f00a; /* and r3,r4 */
L8c61f00a: sa_motion_publish_stop(ctx,0x8c61f00au); r[3]=1u;goto L8c61f00c; /* mov #0x1,r3 */
L8c61f00c: sa_motion_publish_stop(ctx,0x8c61f00cu); r[4]&=r[1];goto L8c61f00e; /* and r1,r4 */
L8c61f00e: sa_motion_publish_stop(ctx,0x8c61f00eu); *sr=(*sr&~1u)|!(r[3]&r[4]);goto L8c61f010; /* tst r3,r4 */
L8c61f010: sa_motion_publish_stop(ctx,0x8c61f010u); if(!(*sr&1u))goto L8c61f01e;goto L8c61f012; /* bf 0x8c61f01e */
L8c61f012: sa_motion_publish_stop(ctx,0x8c61f012u); r[1]=rd(m,bus,&io_status,0x8c61f0fcu,4);if(io_status)return io_status;goto L8c61f014; /* mov.l 0x8c61f0fc,r1 */
L8c61f014: sa_motion_publish_stop(ctx,0x8c61f014u); r[5]=r[14];goto L8c61f016; /* mov r14,r5 */
L8c61f016: sa_motion_publish_stop(ctx,0x8c61f016u); fcopy(f,4u,15u);goto L8c61f018; /* fmov fr15,fr4 */
L8c61f018: sa_motion_publish_stop(ctx,0x8c61f018u); r[6]=r[13];goto L8c61f01a; /* mov r13,r6 */
L8c61f01a: sa_motion_publish_stop(ctx,0x8c61f01cu); target=r[1];*pr=0x8c61f01eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61f01e; /* jsr @r1 */
L8c61f01e: sa_motion_publish_stop(ctx,0x8c61f01eu); r[11]+=1u;goto L8c61f020; /* add #0x1,r11 */
L8c61f020: sa_motion_publish_stop(ctx,0x8c61f020u); r[12]=r[14];goto L8c61f022; /* mov r14,r12 */
L8c61f022: sa_motion_publish_stop(ctx,0x8c61f022u); *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[9]);goto L8c61f024; /* cmp/ge r9,r11 */
L8c61f024: sa_motion_publish_stop(ctx,0x8c61f026u); take=!(*sr&1u);r[14]=r[13];if(take)goto L8c61eff2;goto L8c61f028; /* bf/s 0x8c61eff2 */
L8c61f028: sa_motion_publish_stop(ctx,0x8c61f028u); r[2]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61f02a; /* mov.l @(0x4,r15),r2 */
L8c61f02a: sa_motion_publish_stop(ctx,0x8c61f02au); r[2]+=1u;goto L8c61f02c; /* add #0x1,r2 */
L8c61f02c: sa_motion_publish_stop(ctx,0x8c61f02cu); wr(m,bus,&io_status,r[15]+0x4u,r[2],4);if(io_status)return io_status;goto L8c61f02e; /* mov.l r2,@(0x4,r15) */
L8c61f02e: sa_motion_publish_stop(ctx,0x8c61f02eu); r[3]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61f030; /* mov.l @r15,r3 */
L8c61f030: sa_motion_publish_stop(ctx,0x8c61f030u); r[0]=sx(m,bus,&io_status,r[3]+0x2u,2);if(io_status)return io_status;goto L8c61f032; /* mov.w @(0x2,r3),r0 */
L8c61f032: sa_motion_publish_stop(ctx,0x8c61f032u); r[3]=rd(m,bus,&io_status,r[15]+0x4u,4);if(io_status)return io_status;goto L8c61f034; /* mov.l @(0x4,r15),r3 */
L8c61f034: sa_motion_publish_stop(ctx,0x8c61f034u); r[0]=r[0]&65535u;goto L8c61f036; /* extu.w r0,r0 */
L8c61f036: sa_motion_publish_stop(ctx,0x8c61f036u); *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[0]);goto L8c61f038; /* cmp/ge r0,r3 */
L8c61f038: sa_motion_publish_stop(ctx,0x8c61f038u); if((*sr&1u))goto L8c61f03e;goto L8c61f03a; /* bt 0x8c61f03e */
L8c61f03a: sa_motion_publish_stop(ctx,0x8c61f03cu); goto L8c61ec0e; /* bra 0x8c61ec0e */
L8c61f03e: sa_motion_publish_stop(ctx,0x8c61f040u); goto L8c61f0bc; /* bra 0x8c61f0bc */
L8c61f042: sa_motion_publish_stop(ctx,0x8c61f042u); r[1]=rd(m,bus,&io_status,0x8c61f100u,4);if(io_status)return io_status;goto L8c61f044; /* mov.l 0x8c61f100,r1 */
L8c61f044: sa_motion_publish_stop(ctx,0x8c61f044u); r[2]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;goto L8c61f046; /* mov.l @r15,r2 */
L8c61f046: sa_motion_publish_stop(ctx,0x8c61f046u); wr(m,bus,&io_status,r[1],r[2],4);if(io_status)return io_status;goto L8c61f048; /* mov.l r2,@r1 */
L8c61f048: sa_motion_publish_stop(ctx,0x8c61f048u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f04a; /* frchg */
L8c61f04a: sa_motion_publish_stop(ctx,0x8c61f04au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;goto L8c61f04c; /* fmov.s fr0,@-r15 */
L8c61f04c: sa_motion_publish_stop(ctx,0x8c61f04cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;goto L8c61f04e; /* fmov.s fr1,@-r15 */
L8c61f04e: sa_motion_publish_stop(ctx,0x8c61f04eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;goto L8c61f050; /* fmov.s fr2,@-r15 */
L8c61f050: sa_motion_publish_stop(ctx,0x8c61f050u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;goto L8c61f052; /* fmov.s fr3,@-r15 */
L8c61f052: sa_motion_publish_stop(ctx,0x8c61f052u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;goto L8c61f054; /* fmov.s fr4,@-r15 */
L8c61f054: sa_motion_publish_stop(ctx,0x8c61f054u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;goto L8c61f056; /* fmov.s fr5,@-r15 */
L8c61f056: sa_motion_publish_stop(ctx,0x8c61f056u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;goto L8c61f058; /* fmov.s fr6,@-r15 */
L8c61f058: sa_motion_publish_stop(ctx,0x8c61f058u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;goto L8c61f05a; /* fmov.s fr7,@-r15 */
L8c61f05a: sa_motion_publish_stop(ctx,0x8c61f05au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;goto L8c61f05c; /* fmov.s fr8,@-r15 */
L8c61f05c: sa_motion_publish_stop(ctx,0x8c61f05cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;goto L8c61f05e; /* fmov.s fr9,@-r15 */
L8c61f05e: sa_motion_publish_stop(ctx,0x8c61f05eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;goto L8c61f060; /* fmov.s fr10,@-r15 */
L8c61f060: sa_motion_publish_stop(ctx,0x8c61f060u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;goto L8c61f062; /* fmov.s fr11,@-r15 */
L8c61f062: sa_motion_publish_stop(ctx,0x8c61f062u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;goto L8c61f064; /* fmov.s fr12,@-r15 */
L8c61f064: sa_motion_publish_stop(ctx,0x8c61f064u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;goto L8c61f066; /* fmov.s fr13,@-r15 */
L8c61f066: sa_motion_publish_stop(ctx,0x8c61f066u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;goto L8c61f068; /* fmov.s fr14,@-r15 */
L8c61f068: sa_motion_publish_stop(ctx,0x8c61f068u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;goto L8c61f06a; /* fmov.s fr15,@-r15 */
L8c61f06a: sa_motion_publish_stop(ctx,0x8c61f06au); r[0]=0x8c61f0d4u;goto L8c61f06c; /* mova 0x8c61f0d4,r0 */
L8c61f06c: sa_motion_publish_stop(ctx,0x8c61f06cu); fload(m,bus,&io_status,f,12u,r[0]);if(io_status)return io_status;goto L8c61f06e; /* fmov.s @r0,fr12 */
L8c61f06e: sa_motion_publish_stop(ctx,0x8c61f06eu); fcopy(f,13u,12u);goto L8c61f070; /* fmov fr12,fr13 */
L8c61f070: sa_motion_publish_stop(ctx,0x8c61f070u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[0],&fr[12],&f->fpscr,&fr[0]);if(status)return status;}goto L8c61f072; /* fmul fr12,fr0 */
L8c61f072: sa_motion_publish_stop(ctx,0x8c61f072u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c61f074; /* fmul fr12,fr4 */
L8c61f074: sa_motion_publish_stop(ctx,0x8c61f074u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[8],&fr[12],&f->fpscr,&fr[8]);if(status)return status;}goto L8c61f076; /* fmul fr12,fr8 */
L8c61f076: sa_motion_publish_stop(ctx,0x8c61f076u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[1],&fr[12],&f->fpscr,&fr[1]);if(status)return status;}goto L8c61f078; /* fmul fr12,fr1 */
L8c61f078: sa_motion_publish_stop(ctx,0x8c61f078u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[12],&f->fpscr,&fr[5]);if(status)return status;}goto L8c61f07a; /* fmul fr12,fr5 */
L8c61f07a: sa_motion_publish_stop(ctx,0x8c61f07au); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[9],&fr[12],&f->fpscr,&fr[9]);if(status)return status;}goto L8c61f07c; /* fmul fr12,fr9 */
L8c61f07c: sa_motion_publish_stop(ctx,0x8c61f07cu); r[0]=rd(m,bus,&io_status,0x8c61f0d8u,4);if(io_status)return io_status;goto L8c61f07e; /* mov.l 0x8c61f0d8,r0 */
L8c61f07e: sa_motion_publish_stop(ctx,0x8c61f07eu); fload(m,bus,&io_status,f,2u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f080; /* fmov.s @r0+,fr2 */
L8c61f080: sa_motion_publish_stop(ctx,0x8c61f080u); fload(m,bus,&io_status,f,6u,r[0]);if(io_status)return io_status;r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f082; /* fmov.s @r0+,fr6 */
L8c61f082: sa_motion_publish_stop(ctx,0x8c61f082u); fload(m,bus,&io_status,f,10u,r[0]);if(io_status)return io_status;goto L8c61f084; /* fmov.s @r0,fr10 */
L8c61f084: sa_motion_publish_stop(ctx,0x8c61f084u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][2]^=0x80000000u;goto L8c61f086; /* fneg fr2 */
L8c61f086: sa_motion_publish_stop(ctx,0x8c61f086u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][6]^=0x80000000u;goto L8c61f088; /* fneg fr6 */
L8c61f088: sa_motion_publish_stop(ctx,0x8c61f088u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][10]^=0x80000000u;goto L8c61f08a; /* fneg fr10 */
L8c61f08a: sa_motion_publish_stop(ctx,0x8c61f08au); r[0]=rd(m,bus,&io_status,0x8c61f0dcu,4);if(io_status)return io_status;goto L8c61f08c; /* mov.l 0x8c61f0dc,r0 */
L8c61f08c: sa_motion_publish_stop(ctx,0x8c61f08cu); fload(m,bus,&io_status,f,14u,r[0]);if(io_status)return io_status;goto L8c61f08e; /* fmov.s @r0,fr14 */
L8c61f08e: sa_motion_publish_stop(ctx,0x8c61f08eu); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f090; /* frchg */
L8c61f090: sa_motion_publish_stop(ctx,0x8c61f090u); r[2]=rd(m,bus,&io_status,0x8c61f104u,4);if(io_status)return io_status;goto L8c61f092; /* mov.l 0x8c61f104,r2 */
L8c61f092: sa_motion_publish_stop(ctx,0x8c61f092u); r[3]=rd(m,bus,&io_status,0x8c61f100u,4);if(io_status)return io_status;goto L8c61f094; /* mov.l 0x8c61f100,r3 */
L8c61f094: sa_motion_publish_stop(ctx,0x8c61f096u); target=r[2];*pr=0x8c61f098u;r[4]=rd(m,bus,&io_status,r[3],4);if(io_status)return io_status;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c61f098; /* jsr @r2 */
L8c61f098: sa_motion_publish_stop(ctx,0x8c61f098u); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f09a; /* frchg */
L8c61f09a: sa_motion_publish_stop(ctx,0x8c61f09au); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f09c; /* fmov.s @r15+,fr15 */
L8c61f09c: sa_motion_publish_stop(ctx,0x8c61f09cu); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f09e; /* fmov.s @r15+,fr14 */
L8c61f09e: sa_motion_publish_stop(ctx,0x8c61f09eu); fload(m,bus,&io_status,f,13u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0a0; /* fmov.s @r15+,fr13 */
L8c61f0a0: sa_motion_publish_stop(ctx,0x8c61f0a0u); fload(m,bus,&io_status,f,12u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0a2; /* fmov.s @r15+,fr12 */
L8c61f0a2: sa_motion_publish_stop(ctx,0x8c61f0a2u); fload(m,bus,&io_status,f,11u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0a4; /* fmov.s @r15+,fr11 */
L8c61f0a4: sa_motion_publish_stop(ctx,0x8c61f0a4u); fload(m,bus,&io_status,f,10u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0a6; /* fmov.s @r15+,fr10 */
L8c61f0a6: sa_motion_publish_stop(ctx,0x8c61f0a6u); fload(m,bus,&io_status,f,9u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0a8; /* fmov.s @r15+,fr9 */
L8c61f0a8: sa_motion_publish_stop(ctx,0x8c61f0a8u); fload(m,bus,&io_status,f,8u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0aa; /* fmov.s @r15+,fr8 */
L8c61f0aa: sa_motion_publish_stop(ctx,0x8c61f0aau); fload(m,bus,&io_status,f,7u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0ac; /* fmov.s @r15+,fr7 */
L8c61f0ac: sa_motion_publish_stop(ctx,0x8c61f0acu); fload(m,bus,&io_status,f,6u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0ae; /* fmov.s @r15+,fr6 */
L8c61f0ae: sa_motion_publish_stop(ctx,0x8c61f0aeu); fload(m,bus,&io_status,f,5u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0b0; /* fmov.s @r15+,fr5 */
L8c61f0b0: sa_motion_publish_stop(ctx,0x8c61f0b0u); fload(m,bus,&io_status,f,4u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0b2; /* fmov.s @r15+,fr4 */
L8c61f0b2: sa_motion_publish_stop(ctx,0x8c61f0b2u); fload(m,bus,&io_status,f,3u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0b4; /* fmov.s @r15+,fr3 */
L8c61f0b4: sa_motion_publish_stop(ctx,0x8c61f0b4u); fload(m,bus,&io_status,f,2u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0b6; /* fmov.s @r15+,fr2 */
L8c61f0b6: sa_motion_publish_stop(ctx,0x8c61f0b6u); fload(m,bus,&io_status,f,1u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0b8; /* fmov.s @r15+,fr1 */
L8c61f0b8: sa_motion_publish_stop(ctx,0x8c61f0b8u); fload(m,bus,&io_status,f,0u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0ba; /* fmov.s @r15+,fr0 */
L8c61f0ba: sa_motion_publish_stop(ctx,0x8c61f0bau); if(f->fpscr&0x80000u)return -1;f->fpscr^=0x200000u;goto L8c61f0bc; /* frchg */
L8c61f0bc: sa_motion_publish_stop(ctx,0x8c61f0bcu); r[15]+=24u;goto L8c61f0be; /* add #0x18,r15 */
L8c61f0be: sa_motion_publish_stop(ctx,0x8c61f0beu); *pr=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f0c0; /* lds.l @r15+,PR */
L8c61f0c0: sa_motion_publish_stop(ctx,0x8c61f0c0u); fload(m,bus,&io_status,f,14u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0c2; /* fmov.s @r15+,fr14 */
L8c61f0c2: sa_motion_publish_stop(ctx,0x8c61f0c2u); fload(m,bus,&io_status,f,15u,r[15]);if(io_status)return io_status;r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c61f0c4; /* fmov.s @r15+,fr15 */
L8c61f0c4: sa_motion_publish_stop(ctx,0x8c61f0c4u); r[9]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f0c6; /* mov.l @r15+,r9 */
L8c61f0c6: sa_motion_publish_stop(ctx,0x8c61f0c6u); r[10]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f0c8; /* mov.l @r15+,r10 */
L8c61f0c8: sa_motion_publish_stop(ctx,0x8c61f0c8u); r[11]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f0ca; /* mov.l @r15+,r11 */
L8c61f0ca: sa_motion_publish_stop(ctx,0x8c61f0cau); r[12]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f0cc; /* mov.l @r15+,r12 */
L8c61f0cc: sa_motion_publish_stop(ctx,0x8c61f0ccu); r[13]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;goto L8c61f0ce; /* mov.l @r15+,r13 */
L8c61f0ce: sa_motion_publish_stop(ctx,0x8c61f0d0u); if(*pr!=entry_pr)return -3;r[14]=rd(m,bus,&io_status,r[15],4);if(io_status)return io_status;r[15]+=4;return 0; /* rts */
}
