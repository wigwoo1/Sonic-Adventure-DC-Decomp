/* Original USA world-render/resource dependencies; behavior proof separate. */
/* Renderer initialization, geometry and explicit GBR state; names inferred.
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
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)value:(uint32_t)(int32_t)(int16_t)value;
}
static void wr(uint8_t *m,uint32_t a,uint32_t value,unsigned n){
 for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(value>>(8*i));
}
static void fload(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  f->bank[bank][reg]=rd(m,a,4);f->bank[bank][reg+1]=rd(m,a+4u,4);
 }else f->bank[bank][reg]=rd(m,a,4);
}
static void fstore(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  wr(m,a,f->bank[bank][reg],4);wr(m,a+4u,f->bank[bank][reg+1],4);
 }else wr(m,a,f->bank[bank][reg],4);
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
SA_STAGE_EXPORT int sa_stage_world_matrix_restore_8c02e7e4(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 (void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c02e7e4;
L8c02e7e4: *stop=0x8c02e7e4u;r[4]=1u;goto L8c02e7e6; /* mov #0x1,r4 */
L8c02e7e6: *stop=0x8c02e7e6u;r[4]<<=2;goto L8c02e7e8; /* shll2 r4 */
L8c02e7e8: *stop=0x8c02e7e8u;r[4]<<=2;goto L8c02e7ea; /* shll2 r4 */
L8c02e7ea: *stop=0x8c02e7eau;r[5]=rd(m,0x8c02e80cu,4);goto L8c02e7ec; /* mov.l 0x8c02e80c,r5 */
L8c02e7ec: *stop=0x8c02e7ecu;r[4]<<=2;goto L8c02e7ee; /* shll2 r4 */
L8c02e7ee: *stop=0x8c02e7eeu;if(!valid_ram(r[5],4u))return -3;r[1]=rd(m,r[5],4);goto L8c02e7f0; /* mov.l @r5,r1 */
L8c02e7f0: *stop=0x8c02e7f0u;r[1]-=r[4];goto L8c02e7f2; /* sub r4,r1 */
L8c02e7f2: *stop=0x8c02e7f2u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c02e7f4; /* fschg */
L8c02e7f4: *stop=0x8c02e7f4u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[1]);r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02e7f6; /* fmov.s @r1+,fr1 */
L8c02e7f6: *stop=0x8c02e7f6u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[1]);r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02e7f8; /* fmov.s @r1+,fr3 */
L8c02e7f8: *stop=0x8c02e7f8u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[1]);r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02e7fa; /* fmov.s @r1+,fr5 */
L8c02e7fa: *stop=0x8c02e7fau;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[1]);r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02e7fc; /* fmov.s @r1+,fr7 */
L8c02e7fc: *stop=0x8c02e7fcu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[1]);r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02e7fe; /* fmov.s @r1+,fr9 */
L8c02e7fe: *stop=0x8c02e7feu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,11u,r[1]);r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02e800; /* fmov.s @r1+,fr11 */
L8c02e800: *stop=0x8c02e800u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[1]);r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02e802; /* fmov.s @r1+,fr13 */
L8c02e802: *stop=0x8c02e802u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[1]);r[1]+=(f->fpscr&0x100000u)?8u:4u;goto L8c02e804; /* fmov.s @r1+,fr15 */
L8c02e804: *stop=0x8c02e804u;if(f->fpscr&0x80000u)return -1;f->fpscr^=0x100000u;goto L8c02e806; /* fschg */
L8c02e806: *stop=0x8c02e806u;if(*pr!=entry_pr)return -3;*stop=0x8c02e808u;r[0]=1u;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_stage_world_model_draw_8c037108(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 (void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c037108;
L8c037108: *stop=0x8c037108u;r[0]=rd(m,0x8c037180u,4);goto L8c03710a; /* mov.l 0x8c037180,r0 */
L8c03710a: *stop=0x8c03710au;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4u;wr(m,r[0],*pr,4);goto L8c03710c; /* sts.l PR,@-r0 */
L8c03710c: *stop=0x8c03710cu;if(!valid_ram(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[0]);goto L8c03710e; /* fmov.s fr15,@-r0 */
L8c03710e: *stop=0x8c03710eu;if(!valid_ram(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[0]);goto L8c037110; /* fmov.s fr14,@-r0 */
L8c037110: *stop=0x8c037110u;if(!valid_ram(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[0]);goto L8c037112; /* fmov.s fr13,@-r0 */
L8c037112: *stop=0x8c037112u;if(!valid_ram(r[0]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[0]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[0]);goto L8c037114; /* fmov.s fr12,@-r0 */
L8c037114: *stop=0x8c037114u;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[14],4);goto L8c037116; /* mov.l r14,@-r0 */
L8c037116: *stop=0x8c037116u;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[13],4);goto L8c037118; /* mov.l r13,@-r0 */
L8c037118: *stop=0x8c037118u;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[12],4);goto L8c03711a; /* mov.l r12,@-r0 */
L8c03711a: *stop=0x8c03711au;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[11],4);goto L8c03711c; /* mov.l r11,@-r0 */
L8c03711c: *stop=0x8c03711cu;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[10],4);goto L8c03711e; /* mov.l r10,@-r0 */
L8c03711e: *stop=0x8c03711eu;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[9],4);goto L8c037120; /* mov.l r9,@-r0 */
L8c037120: *stop=0x8c037120u;if(!valid_ram(r[0]-4u,4u))return -3;r[0]-=4;wr(m,r[0],r[8],4);goto L8c037122; /* mov.l r8,@-r0 */
L8c037122: target=0x8c037294u;*pr=0x8c037126u;*stop=0x8c037124u;r[13]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c037126; /* bsr 0x8c037294 */
L8c037126: *stop=0x8c037126u;if(!valid_ram(r[4]+8u,4u))return -3;r[0]=rd(m,r[4]+0x8u,4);goto L8c037128; /* mov.l @(0x8,r4),r0 */
L8c037128: *stop=0x8c037128u;*sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[0]);goto L8c03712a; /* cmp/ge r0,r13 */
L8c03712a: if((*sr&1u))goto L8c03715e;goto L8c03712c; /* bt 0x8c03715e */
L8c03712c: *stop=0x8c03712cu;if(!valid_ram(r[4]+12u,4u))return -3;r[2]=rd(m,r[4]+0xcu,4);goto L8c03712e; /* mov.l @(0xc,r4),r2 */
L8c03712e: *stop=0x8c03712eu;if(!valid_ram(r[4]+16u,4u))return -3;r[3]=rd(m,r[4]+0x10u,4);goto L8c037130; /* mov.l @(0x10,r4),r3 */
L8c037130: *stop=0x8c037130u;if(!valid_ram(r[2]+0u,2u))return -3;r[0]=sx(m,r[2]+0x0u,2);goto L8c037132; /* mov.w @(0x0,r2),r0 */
L8c037132: *stop=0x8c037132u;r[6]=sx(m,0x8c03717cu,2);goto L8c037134; /* mov.w 0x8c03717c,r6 */
L8c037134: *stop=0x8c037134u;r[0]&=r[6];goto L8c037136; /* and r6,r0 */
L8c037136: *stop=0x8c037136u;r[5]=20u;goto L8c037138; /* mov #0x14,r5 */
L8c037138: *stop=0x8c037138u;*macl=(r[0]&65535u)*(r[5]&65535u);goto L8c03713a; /* mulu.w r0,r5 */
L8c03713a: *stop=0x8c03713au;r[5]=*macl;goto L8c03713c; /* sts MACL,r5 */
L8c03713c: *stop=0x8c03713cu;r[5]+=r[3];goto L8c03713e; /* add r3,r5 */
L8c03713e: *stop=0x8c03713eu;r[0]=r[5];goto L8c037140; /* mov r5,r0 */
L8c037140: *stop=0x8c037140u;if(!valid_ram(*gbr+0x34u,4u))return -3;wr(m,*gbr+0x34u,r[0],4);goto L8c037142; /* mov.l r0,@(0x34,GBR) */
L8c037142: *stop=0x8c037142u;if(!valid_ram(r[5]+16u,4u))return -3;r[0]=rd(m,r[5]+0x10u,4);goto L8c037144; /* mov.l @(0x10,r5),r0 */
L8c037144: *stop=0x8c037144u;r[1]=rd(m,0x8c037184u,4);goto L8c037146; /* mov.l 0x8c037184,r1 */
L8c037146: *stop=0x8c037146u;if(!valid_ram(r[1],4u))return -3;r[6]=rd(m,r[1],4);r[1]+=4;goto L8c037148; /* mov.l @r1+,r6 */
L8c037148: *stop=0x8c037148u;*sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c03714a; /* tst r6,r6 */
L8c03714a: if((*sr&1u))goto L8c037154;goto L8c03714c; /* bt 0x8c037154 */
L8c03714c: *stop=0x8c03714cu;if(!valid_ram(r[1],4u))return -3;r[6]=rd(m,r[1],4);r[1]+=4;goto L8c03714e; /* mov.l @r1+,r6 */
L8c03714e: *stop=0x8c03714eu;r[0]&=r[6];goto L8c037150; /* and r6,r0 */
L8c037150: *stop=0x8c037150u;if(!valid_ram(r[1],4u))return -3;r[6]=rd(m,r[1],4);r[1]+=4;goto L8c037152; /* mov.l @r1+,r6 */
L8c037152: *stop=0x8c037152u;r[0]|=r[6];goto L8c037154; /* or r6,r0 */
L8c037154: *stop=0x8c037154u;if(!valid_ram(*gbr+0x2cu,4u))return -3;wr(m,*gbr+0x2cu,r[0],4);goto L8c037156; /* mov.l r0,@(0x2c,GBR) */
L8c037156: target=0x8c037350u;*pr=0x8c03715au;*stop=0x8c037158u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c03715a; /* bsr 0x8c037350 */
L8c03715a: target=0x8c0376d0u;*pr=0x8c03715eu;*stop=0x8c03715cu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c03715e; /* bsr 0x8c0376d0 */
L8c03715e: *stop=0x8c03715eu;r[0]=rd(m,0x8c037188u,4);goto L8c037160; /* mov.l 0x8c037188,r0 */
L8c037160: *stop=0x8c037160u;if(!valid_ram(r[0],4u))return -3;r[8]=rd(m,r[0],4);r[0]+=4;goto L8c037162; /* mov.l @r0+,r8 */
L8c037162: *stop=0x8c037162u;if(!valid_ram(r[0],4u))return -3;r[9]=rd(m,r[0],4);r[0]+=4;goto L8c037164; /* mov.l @r0+,r9 */
L8c037164: *stop=0x8c037164u;if(!valid_ram(r[0],4u))return -3;r[10]=rd(m,r[0],4);r[0]+=4;goto L8c037166; /* mov.l @r0+,r10 */
L8c037166: *stop=0x8c037166u;if(!valid_ram(r[0],4u))return -3;r[11]=rd(m,r[0],4);r[0]+=4;goto L8c037168; /* mov.l @r0+,r11 */
L8c037168: *stop=0x8c037168u;if(!valid_ram(r[0],4u))return -3;r[12]=rd(m,r[0],4);r[0]+=4;goto L8c03716a; /* mov.l @r0+,r12 */
L8c03716a: *stop=0x8c03716au;if(!valid_ram(r[0],4u))return -3;r[13]=rd(m,r[0],4);r[0]+=4;goto L8c03716c; /* mov.l @r0+,r13 */
L8c03716c: *stop=0x8c03716cu;if(!valid_ram(r[0],4u))return -3;r[14]=rd(m,r[0],4);r[0]+=4;goto L8c03716e; /* mov.l @r0+,r14 */
L8c03716e: *stop=0x8c03716eu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037170; /* fmov.s @r0+,fr12 */
L8c037170: *stop=0x8c037170u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037172; /* fmov.s @r0+,fr13 */
L8c037172: *stop=0x8c037172u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037174; /* fmov.s @r0+,fr14 */
L8c037174: *stop=0x8c037174u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[0]);r[0]+=(f->fpscr&0x100000u)?8u:4u;goto L8c037176; /* fmov.s @r0+,fr15 */
L8c037176: *stop=0x8c037176u;if(!valid_ram(r[0],4u))return -3;*pr=rd(m,r[0],4);r[0]+=4u;goto L8c037178; /* lds.l @r0+,PR */
L8c037178: *stop=0x8c037178u;if(*pr!=entry_pr)return -3;*stop=0x8c03717au;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_stage_world_render_state_8c038da0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 (void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c038da0;
L8c038da0: *stop=0x8c038da0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c038da2; /* sts.l PR,@-r15 */
L8c038da2: *stop=0x8c038da2u;r[0]=0u;goto L8c038da4; /* mov #0x0,r0 */
L8c038da4: *stop=0x8c038da4u;r[2]=rd(m,0x8c038de0u,4);goto L8c038da6; /* mov.l 0x8c038de0,r2 */
L8c038da6: *stop=0x8c038da6u;if(!valid_ram(r[2],4u))return -3;r[6]=rd(m,r[2],4);goto L8c038da8; /* mov.l @r2,r6 */
L8c038da8: *stop=0x8c038da8u;r[3]=rd(m,0x8c038de4u,4);goto L8c038daa; /* mov.l 0x8c038de4,r3 */
L8c038daa: *stop=0x8c038daau;*sr=(*sr&~1u)|!(r[3]&r[6]);goto L8c038dac; /* tst r3,r6 */
L8c038dac: if((*sr&1u))goto L8c038db0;goto L8c038dae; /* bt 0x8c038db0 */
L8c038dae: *stop=0x8c038daeu;r[0]=1u;goto L8c038db0; /* mov #0x1,r0 */
L8c038db0: *stop=0x8c038db0u;if(!valid_ram(*gbr+0x1cu,4u))return -3;wr(m,*gbr+0x1cu,r[0],4);goto L8c038db2; /* mov.l r0,@(0x1c,GBR) */
L8c038db2: *stop=0x8c038db2u;r[0]=rd(m,0x8c038de8u,4);goto L8c038db4; /* mov.l 0x8c038de8,r0 */
L8c038db4: *stop=0x8c038db4u;if(!valid_ram(r[0],4u))return -3;r[0]=rd(m,r[0],4);goto L8c038db6; /* mov.l @r0,r0 */
L8c038db6: *stop=0x8c038db6u;if(!valid_ram(*gbr+0x20u,4u))return -3;wr(m,*gbr+0x20u,r[0],4);goto L8c038db8; /* mov.l r0,@(0x20,GBR) */
L8c038db8: *stop=0x8c038db8u;r[0]=rd(m,0x8c038decu,4);goto L8c038dba; /* mov.l 0x8c038dec,r0 */
L8c038dba: *stop=0x8c038dbau;if(!valid_ram(r[0],4u))return -3;r[0]=rd(m,r[0],4);goto L8c038dbc; /* mov.l @r0,r0 */
L8c038dbc: *stop=0x8c038dbcu;if(!valid_ram(*gbr+0x24u,4u))return -3;wr(m,*gbr+0x24u,r[0],4);goto L8c038dbe; /* mov.l r0,@(0x24,GBR) */
L8c038dbe: *stop=0x8c038dbeu;r[0]=0u;goto L8c038dc0; /* mov #0x0,r0 */
L8c038dc0: *stop=0x8c038dc0u;r[3]=rd(m,0x8c038df0u,4);goto L8c038dc2; /* mov.l 0x8c038df0,r3 */
L8c038dc2: *stop=0x8c038dc2u;*sr=(*sr&~1u)|!(r[3]&r[6]);goto L8c038dc4; /* tst r3,r6 */
L8c038dc4: if((*sr&1u))goto L8c038dc8;goto L8c038dc6; /* bt 0x8c038dc8 */
L8c038dc6: *stop=0x8c038dc6u;r[0]=4294967295u;goto L8c038dc8; /* mov #-0x1,r0 */
L8c038dc8: *stop=0x8c038dc8u;r[3]=rd(m,0x8c038df4u,4);goto L8c038dca; /* mov.l 0x8c038df4,r3 */
L8c038dca: *stop=0x8c038dcau;*sr=(*sr&~1u)|!(r[3]&r[6]);goto L8c038dcc; /* tst r3,r6 */
L8c038dcc: if((*sr&1u))goto L8c038dd0;goto L8c038dce; /* bt 0x8c038dd0 */
L8c038dce: *stop=0x8c038dceu;r[0]=1u;goto L8c038dd0; /* mov #0x1,r0 */
L8c038dd0: *stop=0x8c038dd0u;if(!valid_ram(*gbr+0x28u,4u))return -3;wr(m,*gbr+0x28u,r[0],4);goto L8c038dd2; /* mov.l r0,@(0x28,GBR) */
L8c038dd2: *stop=0x8c038dd2u;r[0]=rd(m,0x8c038df8u,4);goto L8c038dd4; /* mov.l 0x8c038df8,r0 */
L8c038dd4: target=r[0];*pr=0x8c038dd8u;*stop=0x8c038dd6u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c038dd8; /* jsr @r0 */
L8c038dd8: *stop=0x8c038dd8u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c038dda; /* lds.l @r15+,PR */
L8c038dda: *stop=0x8c038ddau;if(*pr!=entry_pr)return -3;*stop=0x8c038ddcu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_stage_world_motion_draw_8c040942(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 (void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c040942;
L8c040942: *stop=0x8c040942u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c040944; /* mov.l r14,@-r15 */
L8c040944: *stop=0x8c040944u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c040946; /* sts.l PR,@-r15 */
L8c040946: *stop=0x8c040946u;r[15]+=4294967292u;goto L8c040948; /* add #-0x4,r15 */
L8c040948: *stop=0x8c040948u;if(!valid_ram(r[4]+4u,4u))return -3;r[3]=rd(m,r[4]+0x4u,4);goto L8c04094a; /* mov.l @(0x4,r4),r3 */
L8c04094a: *stop=0x8c04094au;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto L8c04094c; /* mov.l r3,@r15 */
L8c04094c: *stop=0x8c04094cu;if(!valid_ram(r[4],4u))return -3;r[14]=rd(m,r[4],4);goto L8c04094e; /* mov.l @r4,r14 */
L8c04094e: *stop=0x8c04094eu;r[4]=r[3];goto L8c040950; /* mov r3,r4 */
L8c040950: target=0x8c03fe1au;*pr=0x8c040954u;*stop=0x8c040952u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040954; /* bsr 0x8c03fe1a */
L8c040954: *stop=0x8c040954u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c040956; /* cmp/eq #0x0,r0 */
L8c040956: if(!(*sr&1u))goto L8c040970;goto L8c040958; /* bf 0x8c040970 */
L8c040958: target=0x8c03fe60u;*pr=0x8c04095cu;*stop=0x8c04095au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04095c; /* bsr 0x8c03fe60 */
L8c04095c: *stop=0x8c04095cu;r[2]=rd(m,0x8c0409b0u,4);goto L8c04095e; /* mov.l 0x8c0409b0,r2 */
L8c04095e: target=r[2];*pr=0x8c040962u;*stop=0x8c040960u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040962; /* jsr @r2 */
L8c040962: target=0x8c040784u;*pr=0x8c040966u;*stop=0x8c040964u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c040966; /* bsr 0x8c040784 */
L8c040966: *stop=0x8c040966u;r[15]+=4u;goto L8c040968; /* add #0x4,r15 */
L8c040968: *stop=0x8c040968u;r[2]=rd(m,0x8c0409b4u,4);goto L8c04096a; /* mov.l 0x8c0409b4,r2 */
L8c04096a: *stop=0x8c04096au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c04096c; /* lds.l @r15+,PR */
L8c04096c: target=r[2];*stop=0x8c04096eu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r2 */
L8c040970: *stop=0x8c040970u;r[15]+=4u;goto L8c040972; /* add #0x4,r15 */
L8c040972: *stop=0x8c040972u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c040974; /* lds.l @r15+,PR */
L8c040974: *stop=0x8c040974u;if(*pr!=entry_pr)return -3;*stop=0x8c040976u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_stage_world_file_size_8c09da74(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 (void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c09da74;
L8c09da74: *stop=0x8c09da74u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c09da76; /* mov.l r14,@-r15 */
L8c09da76: *stop=0x8c09da76u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c09da78; /* sts.l PR,@-r15 */
L8c09da78: *stop=0x8c09da78u;r[15]+=4294967292u;goto L8c09da7a; /* add #-0x4,r15 */
L8c09da7a: *stop=0x8c09da7au;r[3]=rd(m,0x8c09db54u,4);goto L8c09da7c; /* mov.l 0x8c09db54,r3 */
L8c09da7c: *stop=0x8c09da7cu;r[14]=rd(m,0x8c09db50u,4);goto L8c09da7e; /* mov.l 0x8c09db50,r14 */
L8c09da7e: target=r[3];*pr=0x8c09da82u;*stop=0x8c09da80u;r[5]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09da82; /* jsr @r3 */
L8c09da82: *stop=0x8c09da82u;r[4]=r[0];goto L8c09da84; /* mov r0,r4 */
L8c09da84: *stop=0x8c09da84u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c09da86; /* tst r4,r4 */
L8c09da86: take=!(*sr&1u);*stop=0x8c09da88u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[0],4);if(take)goto L8c09da94;goto L8c09da8a; /* bf/s 0x8c09da94 */
L8c09da8a: *stop=0x8c09da8au;r[0]=4294967295u;goto L8c09da8c; /* mov #-0x1,r0 */
L8c09da8c: *stop=0x8c09da8cu;r[15]+=4u;goto L8c09da8e; /* add #0x4,r15 */
L8c09da8e: *stop=0x8c09da8eu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c09da90; /* lds.l @r15+,PR */
L8c09da90: *stop=0x8c09da90u;if(*pr!=entry_pr)return -3;*stop=0x8c09da92u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c09da94: *stop=0x8c09da94u;r[3]=rd(m,0x8c09db58u,4);goto L8c09da96; /* mov.l 0x8c09db58,r3 */
L8c09da96: target=r[3];*pr=0x8c09da9au;*stop=0x8c09da98u;r[5]=r[15];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09da9a; /* jsr @r3 */
L8c09da9a: *stop=0x8c09da9au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c09da9c; /* tst r0,r0 */
L8c09da9c: if(!(*sr&1u))goto L8c09daa2;goto L8c09da9e; /* bf 0x8c09daa2 */
L8c09da9e: *stop=0x8c09da9eu;r[3]=4294967295u;goto L8c09daa0; /* mov #-0x1,r3 */
L8c09daa0: *stop=0x8c09daa0u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto L8c09daa2; /* mov.l r3,@r15 */
L8c09daa2: *stop=0x8c09daa2u;r[3]=rd(m,0x8c09db5cu,4);goto L8c09daa4; /* mov.l 0x8c09db5c,r3 */
L8c09daa4: target=r[3];*pr=0x8c09daa8u;*stop=0x8c09daa6u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09daa8; /* jsr @r3 */
L8c09daa8: *stop=0x8c09daa8u;if(!valid_ram(r[15],4u))return -3;r[0]=rd(m,r[15],4);goto L8c09daaa; /* mov.l @r15,r0 */
L8c09daaa: *stop=0x8c09daaau;r[15]+=4u;goto L8c09daac; /* add #0x4,r15 */
L8c09daac: *stop=0x8c09daacu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c09daae; /* lds.l @r15+,PR */
L8c09daae: *stop=0x8c09daaeu;if(*pr!=entry_pr)return -3;*stop=0x8c09dab0u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_stage_world_chunk1_8c09f5d6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 (void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c09f5d6;
L8c09f5d6: *stop=0x8c09f5d6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c09f5d8; /* mov.l r14,@-r15 */
L8c09f5d8: *stop=0x8c09f5d8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c09f5da; /* mov.l r13,@-r15 */
L8c09f5da: *stop=0x8c09f5dau;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c09f5dc; /* mov.l r12,@-r15 */
L8c09f5dc: *stop=0x8c09f5dcu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c09f5de; /* mov.l r11,@-r15 */
L8c09f5de: *stop=0x8c09f5deu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c09f5e0; /* sts.l PR,@-r15 */
L8c09f5e0: *stop=0x8c09f5e0u;r[15]+=4294967292u;goto L8c09f5e2; /* add #-0x4,r15 */
L8c09f5e2: *stop=0x8c09f5e2u;r[14]=r[4];goto L8c09f5e4; /* mov r4,r14 */
L8c09f5e4: *stop=0x8c09f5e4u;r[11]=4294967292u;goto L8c09f5e6; /* mov #-0x4,r11 */
L8c09f5e6: *stop=0x8c09f5e6u;r[12]=r[14];goto L8c09f5e8; /* mov r14,r12 */
L8c09f5e8: *stop=0x8c09f5e8u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[5],4);goto L8c09f5ea; /* mov.l r5,@r15 */
L8c09f5ea: *stop=0x8c09f5eau;if(!valid_ram(r[14],2u))return -3;r[0]=sx(m,r[14],2);goto L8c09f5ec; /* mov.w @r14,r0 */
L8c09f5ec: *stop=0x8c09f5ecu;if(!valid_ram(r[14]+4u,4u))return -3;r[13]=rd(m,r[14]+0x4u,4);goto L8c09f5ee; /* mov.l @(0x4,r14),r13 */
L8c09f5ee: *stop=0x8c09f5eeu;r[0]=r[0]&65535u;goto L8c09f5f0; /* extu.w r0,r0 */
L8c09f5f0: *stop=0x8c09f5f0u;r[13]+=r[14];goto L8c09f5f2; /* add r14,r13 */
L8c09f5f2: *stop=0x8c09f5f2u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c09f5f4; /* cmp/eq #0x0,r0 */
L8c09f5f4: *stop=0x8c09f5f4u;r[13]+=11u;goto L8c09f5f6; /* add #0xb,r13 */
L8c09f5f6: take=(*sr&1u);*stop=0x8c09f5f8u;r[13]&=r[11];if(take)goto L8c09f666;goto L8c09f5fa; /* bt/s 0x8c09f666 */
L8c09f5fa: *stop=0x8c09f5fau;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c09f5fc; /* cmp/eq #0x1,r0 */
L8c09f5fc: if((*sr&1u))goto L8c09f666;goto L8c09f5fe; /* bt 0x8c09f666 */
L8c09f5fe: *stop=0x8c09f5feu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c09f600; /* cmp/eq #0x2,r0 */
L8c09f600: if((*sr&1u))goto L8c09f666;goto L8c09f602; /* bt 0x8c09f666 */
L8c09f602: *stop=0x8c09f602u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c09f604; /* cmp/eq #0x3,r0 */
L8c09f604: if((*sr&1u))goto L8c09f666;goto L8c09f606; /* bt 0x8c09f666 */
L8c09f606: *stop=0x8c09f606u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c09f608; /* cmp/eq #0x4,r0 */
L8c09f608: if((*sr&1u))goto L8c09f666;goto L8c09f60a; /* bt 0x8c09f666 */
L8c09f60a: *stop=0x8c09f60au;*sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c09f60c; /* cmp/eq #0x5,r0 */
L8c09f60c: if((*sr&1u))goto L8c09f640;goto L8c09f60e; /* bt 0x8c09f640 */
L8c09f60e: *stop=0x8c09f60eu;*sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c09f610; /* cmp/eq #0x6,r0 */
L8c09f610: if((*sr&1u))goto L8c09f64e;goto L8c09f612; /* bt 0x8c09f64e */
L8c09f612: *stop=0x8c09f612u;*sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c09f614; /* cmp/eq #0x7,r0 */
L8c09f614: if((*sr&1u))goto L8c09f65c;goto L8c09f616; /* bt 0x8c09f65c */
L8c09f616: *stop=0x8c09f618u;goto L8c09f666; /* bra 0x8c09f666 */
L8c09f640: *stop=0x8c09f640u;r[5]=r[14];goto L8c09f642; /* mov r14,r5 */
L8c09f642: *stop=0x8c09f642u;r[5]+=8u;goto L8c09f644; /* add #0x8,r5 */
L8c09f644: *stop=0x8c09f644u;r[6]=r[13];goto L8c09f646; /* mov r13,r6 */
L8c09f646: target=0x8c09f67cu;*pr=0x8c09f64au;*stop=0x8c09f648u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09f64a; /* bsr 0x8c09f67c */
L8c09f64a: *stop=0x8c09f64cu;goto L8c09f666; /* bra 0x8c09f666 */
L8c09f64e: *stop=0x8c09f64eu;r[5]=r[14];goto L8c09f650; /* mov r14,r5 */
L8c09f650: *stop=0x8c09f650u;r[5]+=8u;goto L8c09f652; /* add #0x8,r5 */
L8c09f652: *stop=0x8c09f652u;r[6]=r[13];goto L8c09f654; /* mov r13,r6 */
L8c09f654: target=0x8c09f6c8u;*pr=0x8c09f658u;*stop=0x8c09f656u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09f658; /* bsr 0x8c09f6c8 */
L8c09f658: *stop=0x8c09f65au;goto L8c09f666; /* bra 0x8c09f666 */
L8c09f65c: *stop=0x8c09f65cu;r[5]=r[14];goto L8c09f65e; /* mov r14,r5 */
L8c09f65e: *stop=0x8c09f65eu;r[5]+=8u;goto L8c09f660; /* add #0x8,r5 */
L8c09f660: *stop=0x8c09f660u;r[6]=r[13];goto L8c09f662; /* mov r13,r6 */
L8c09f662: target=0x8c09f708u;*pr=0x8c09f666u;*stop=0x8c09f664u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09f666; /* bsr 0x8c09f708 */
L8c09f666: *stop=0x8c09f666u;if(!valid_ram(r[15],4u))return -3;r[2]=rd(m,r[15],4);goto L8c09f668; /* mov.l @r15,r2 */
L8c09f668: *stop=0x8c09f668u;r[14]=r[13];goto L8c09f66a; /* mov r13,r14 */
L8c09f66a: *stop=0x8c09f66au;*sr=(*sr&~1u)|(r[14]>=r[2]);goto L8c09f66c; /* cmp/hs r2,r14 */
L8c09f66c: if(!(*sr&1u))goto L8c09f5ea;goto L8c09f66e; /* bf 0x8c09f5ea */
L8c09f66e: *stop=0x8c09f66eu;r[15]+=4u;goto L8c09f670; /* add #0x4,r15 */
L8c09f670: *stop=0x8c09f670u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c09f672; /* lds.l @r15+,PR */
L8c09f672: *stop=0x8c09f672u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c09f674; /* mov.l @r15+,r11 */
L8c09f674: *stop=0x8c09f674u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c09f676; /* mov.l @r15+,r12 */
L8c09f676: *stop=0x8c09f676u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c09f678; /* mov.l @r15+,r13 */
L8c09f678: *stop=0x8c09f678u;if(*pr!=entry_pr)return -3;*stop=0x8c09f67au;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_stage_world_chunk3_8c09f900(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 (void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c09f900;
L8c09f900: *stop=0x8c09f900u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c09f902; /* mov.l r14,@-r15 */
L8c09f902: *stop=0x8c09f902u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c09f904; /* mov.l r13,@-r15 */
L8c09f904: *stop=0x8c09f904u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c09f906; /* mov.l r12,@-r15 */
L8c09f906: *stop=0x8c09f906u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c09f908; /* mov.l r11,@-r15 */
L8c09f908: *stop=0x8c09f908u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c09f90a; /* sts.l PR,@-r15 */
L8c09f90a: *stop=0x8c09f90au;r[15]+=4294967292u;goto L8c09f90c; /* add #-0x4,r15 */
L8c09f90c: *stop=0x8c09f90cu;r[14]=r[4];goto L8c09f90e; /* mov r4,r14 */
L8c09f90e: *stop=0x8c09f90eu;r[11]=4294967292u;goto L8c09f910; /* mov #-0x4,r11 */
L8c09f910: *stop=0x8c09f910u;r[12]=r[14];goto L8c09f912; /* mov r14,r12 */
L8c09f912: *stop=0x8c09f912u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[5],4);goto L8c09f914; /* mov.l r5,@r15 */
L8c09f914: *stop=0x8c09f914u;if(!valid_ram(r[14],2u))return -3;r[0]=sx(m,r[14],2);goto L8c09f916; /* mov.w @r14,r0 */
L8c09f916: *stop=0x8c09f916u;if(!valid_ram(r[14]+4u,4u))return -3;r[13]=rd(m,r[14]+0x4u,4);goto L8c09f918; /* mov.l @(0x4,r14),r13 */
L8c09f918: *stop=0x8c09f918u;r[0]=r[0]&65535u;goto L8c09f91a; /* extu.w r0,r0 */
L8c09f91a: *stop=0x8c09f91au;r[13]+=r[14];goto L8c09f91c; /* add r14,r13 */
L8c09f91c: *stop=0x8c09f91cu;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c09f91e; /* cmp/eq #0x0,r0 */
L8c09f91e: *stop=0x8c09f91eu;r[13]+=11u;goto L8c09f920; /* add #0xb,r13 */
L8c09f920: take=(*sr&1u);*stop=0x8c09f922u;r[13]&=r[11];if(take)goto L8c09f950;goto L8c09f924; /* bt/s 0x8c09f950 */
L8c09f924: *stop=0x8c09f924u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c09f926; /* cmp/eq #0x1,r0 */
L8c09f926: if((*sr&1u))goto L8c09f950;goto L8c09f928; /* bt 0x8c09f950 */
L8c09f928: *stop=0x8c09f928u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c09f92a; /* cmp/eq #0x2,r0 */
L8c09f92a: if((*sr&1u))goto L8c09f950;goto L8c09f92c; /* bt 0x8c09f950 */
L8c09f92c: *stop=0x8c09f92cu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c09f92e; /* cmp/eq #0x3,r0 */
L8c09f92e: if((*sr&1u))goto L8c09f938;goto L8c09f930; /* bt 0x8c09f938 */
L8c09f930: *stop=0x8c09f930u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c09f932; /* cmp/eq #0x4,r0 */
L8c09f932: if((*sr&1u))goto L8c09f946;goto L8c09f934; /* bt 0x8c09f946 */
L8c09f934: *stop=0x8c09f936u;goto L8c09f950; /* bra 0x8c09f950 */
L8c09f938: *stop=0x8c09f938u;r[5]=r[14];goto L8c09f93a; /* mov r14,r5 */
L8c09f93a: *stop=0x8c09f93au;r[5]+=8u;goto L8c09f93c; /* add #0x8,r5 */
L8c09f93c: *stop=0x8c09f93cu;r[6]=r[13];goto L8c09f93e; /* mov r13,r6 */
L8c09f93e: target=0x8c09f884u;*pr=0x8c09f942u;*stop=0x8c09f940u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09f942; /* bsr 0x8c09f884 */
L8c09f942: *stop=0x8c09f944u;goto L8c09f950; /* bra 0x8c09f950 */
L8c09f946: *stop=0x8c09f946u;r[5]=r[14];goto L8c09f948; /* mov r14,r5 */
L8c09f948: *stop=0x8c09f948u;r[5]+=8u;goto L8c09f94a; /* add #0x8,r5 */
L8c09f94a: *stop=0x8c09f94au;r[6]=r[13];goto L8c09f94c; /* mov r13,r6 */
L8c09f94c: target=0x8c09f8e2u;*pr=0x8c09f950u;*stop=0x8c09f94eu;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09f950; /* bsr 0x8c09f8e2 */
L8c09f950: *stop=0x8c09f950u;if(!valid_ram(r[15],4u))return -3;r[2]=rd(m,r[15],4);goto L8c09f952; /* mov.l @r15,r2 */
L8c09f952: *stop=0x8c09f952u;r[14]=r[13];goto L8c09f954; /* mov r13,r14 */
L8c09f954: *stop=0x8c09f954u;*sr=(*sr&~1u)|(r[14]>=r[2]);goto L8c09f956; /* cmp/hs r2,r14 */
L8c09f956: if(!(*sr&1u))goto L8c09f914;goto L8c09f958; /* bf 0x8c09f914 */
L8c09f958: *stop=0x8c09f958u;r[15]+=4u;goto L8c09f95a; /* add #0x4,r15 */
L8c09f95a: *stop=0x8c09f95au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c09f95c; /* lds.l @r15+,PR */
L8c09f95c: *stop=0x8c09f95cu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c09f95e; /* mov.l @r15+,r11 */
L8c09f95e: *stop=0x8c09f95eu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c09f960; /* mov.l @r15+,r12 */
L8c09f960: *stop=0x8c09f960u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c09f962; /* mov.l @r15+,r13 */
L8c09f962: *stop=0x8c09f962u;if(*pr!=entry_pr)return -3;*stop=0x8c09f964u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_stage_world_chunk4_8c09f966(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop,uint32_t *gbr,const struct sa_vertex_bus *bus,struct sa_sh4_integer_extra *integer,const uint32_t *vbr){
 (void)bus;(void)integer;(void)vbr;
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sr;(void)sx;(void)m;(void)wr;(void)fload;(void)fstore;(void)gbr;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c09f966;
L8c09f966: *stop=0x8c09f966u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c09f968; /* mov.l r14,@-r15 */
L8c09f968: *stop=0x8c09f968u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c09f96a; /* mov.l r13,@-r15 */
L8c09f96a: *stop=0x8c09f96au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c09f96c; /* mov.l r12,@-r15 */
L8c09f96c: *stop=0x8c09f96cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c09f96e; /* mov.l r11,@-r15 */
L8c09f96e: *stop=0x8c09f96eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4u;wr(m,r[15],*pr,4);goto L8c09f970; /* sts.l PR,@-r15 */
L8c09f970: *stop=0x8c09f970u;r[15]+=4294967292u;goto L8c09f972; /* add #-0x4,r15 */
L8c09f972: *stop=0x8c09f972u;r[14]=r[4];goto L8c09f974; /* mov r4,r14 */
L8c09f974: *stop=0x8c09f974u;r[11]=4294967292u;goto L8c09f976; /* mov #-0x4,r11 */
L8c09f976: *stop=0x8c09f976u;r[12]=r[14];goto L8c09f978; /* mov r14,r12 */
L8c09f978: *stop=0x8c09f978u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[5],4);goto L8c09f97a; /* mov.l r5,@r15 */
L8c09f97a: *stop=0x8c09f97au;if(!valid_ram(r[14],2u))return -3;r[0]=sx(m,r[14],2);goto L8c09f97c; /* mov.w @r14,r0 */
L8c09f97c: *stop=0x8c09f97cu;if(!valid_ram(r[14]+4u,4u))return -3;r[13]=rd(m,r[14]+0x4u,4);goto L8c09f97e; /* mov.l @(0x4,r14),r13 */
L8c09f97e: *stop=0x8c09f97eu;r[0]=r[0]&65535u;goto L8c09f980; /* extu.w r0,r0 */
L8c09f980: *stop=0x8c09f980u;r[13]+=r[14];goto L8c09f982; /* add r14,r13 */
L8c09f982: *stop=0x8c09f982u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c09f984; /* cmp/eq #0x0,r0 */
L8c09f984: *stop=0x8c09f984u;r[13]+=11u;goto L8c09f986; /* add #0xb,r13 */
L8c09f986: take=(*sr&1u);*stop=0x8c09f988u;r[13]&=r[11];if(take)goto L8c09f9c4;goto L8c09f98a; /* bt/s 0x8c09f9c4 */
L8c09f98a: *stop=0x8c09f98au;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c09f98c; /* cmp/eq #0x1,r0 */
L8c09f98c: if((*sr&1u))goto L8c09f9c4;goto L8c09f98e; /* bt 0x8c09f9c4 */
L8c09f98e: *stop=0x8c09f98eu;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c09f990; /* cmp/eq #0x2,r0 */
L8c09f990: if((*sr&1u))goto L8c09f99e;goto L8c09f992; /* bt 0x8c09f99e */
L8c09f992: *stop=0x8c09f992u;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c09f994; /* cmp/eq #0x3,r0 */
L8c09f994: if((*sr&1u))goto L8c09f9ac;goto L8c09f996; /* bt 0x8c09f9ac */
L8c09f996: *stop=0x8c09f996u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c09f998; /* cmp/eq #0x4,r0 */
L8c09f998: if((*sr&1u))goto L8c09f9ba;goto L8c09f99a; /* bt 0x8c09f9ba */
L8c09f99a: *stop=0x8c09f99cu;goto L8c09f9c4; /* bra 0x8c09f9c4 */
L8c09f99e: *stop=0x8c09f99eu;r[5]=r[14];goto L8c09f9a0; /* mov r14,r5 */
L8c09f9a0: *stop=0x8c09f9a0u;r[5]+=8u;goto L8c09f9a2; /* add #0x8,r5 */
L8c09f9a2: *stop=0x8c09f9a2u;r[6]=r[13];goto L8c09f9a4; /* mov r13,r6 */
L8c09f9a4: target=0x8c09f9dau;*pr=0x8c09f9a8u;*stop=0x8c09f9a6u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09f9a8; /* bsr 0x8c09f9da */
L8c09f9a8: *stop=0x8c09f9aau;goto L8c09f9c4; /* bra 0x8c09f9c4 */
L8c09f9ac: *stop=0x8c09f9acu;r[5]=r[14];goto L8c09f9ae; /* mov r14,r5 */
L8c09f9ae: *stop=0x8c09f9aeu;r[5]+=8u;goto L8c09f9b0; /* add #0x8,r5 */
L8c09f9b0: *stop=0x8c09f9b0u;r[6]=r[13];goto L8c09f9b2; /* mov r13,r6 */
L8c09f9b2: target=0x8c09f884u;*pr=0x8c09f9b6u;*stop=0x8c09f9b4u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09f9b6; /* bsr 0x8c09f884 */
L8c09f9b6: *stop=0x8c09f9b8u;goto L8c09f9c4; /* bra 0x8c09f9c4 */
L8c09f9ba: *stop=0x8c09f9bau;r[5]=r[14];goto L8c09f9bc; /* mov r14,r5 */
L8c09f9bc: *stop=0x8c09f9bcu;r[5]+=8u;goto L8c09f9be; /* add #0x8,r5 */
L8c09f9be: *stop=0x8c09f9beu;r[6]=r[13];goto L8c09f9c0; /* mov r13,r6 */
L8c09f9c0: target=0x8c09f8e2u;*pr=0x8c09f9c4u;*stop=0x8c09f9c2u;r[4]=r[12];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09f9c4; /* bsr 0x8c09f8e2 */
L8c09f9c4: *stop=0x8c09f9c4u;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto L8c09f9c6; /* mov.l @r15,r3 */
L8c09f9c6: *stop=0x8c09f9c6u;r[14]=r[13];goto L8c09f9c8; /* mov r13,r14 */
L8c09f9c8: *stop=0x8c09f9c8u;*sr=(*sr&~1u)|(r[14]>=r[3]);goto L8c09f9ca; /* cmp/hs r3,r14 */
L8c09f9ca: if(!(*sr&1u))goto L8c09f97a;goto L8c09f9cc; /* bf 0x8c09f97a */
L8c09f9cc: *stop=0x8c09f9ccu;r[15]+=4u;goto L8c09f9ce; /* add #0x4,r15 */
L8c09f9ce: *stop=0x8c09f9ceu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4u;goto L8c09f9d0; /* lds.l @r15+,PR */
L8c09f9d0: *stop=0x8c09f9d0u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c09f9d2; /* mov.l @r15+,r11 */
L8c09f9d2: *stop=0x8c09f9d2u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c09f9d4; /* mov.l @r15+,r12 */
L8c09f9d4: *stop=0x8c09f9d4u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c09f9d6; /* mov.l @r15+,r13 */
L8c09f9d6: *stop=0x8c09f9d6u;if(*pr!=entry_pr)return -3;*stop=0x8c09f9d8u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
