/* Original scene direction/state owner and its stage setup dependencies; names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
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
SA_STAGE_EXPORT int sa_camera_scene_8c012240(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c012240;
L8c012240: *stop=0x8c012240u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c012242; /* mov.l r14,@-r15 */
L8c012242: *stop=0x8c012242u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c012244; /* mov.l r13,@-r15 */
L8c012244: *stop=0x8c012244u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c012246; /* mov.l r12,@-r15 */
L8c012246: *stop=0x8c012246u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c012248; /* mov.l r11,@-r15 */
L8c012248: *stop=0x8c012248u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c01224a; /* mov.l r10,@-r15 */
L8c01224a: *stop=0x8c01224au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c01224c; /* mov.l r9,@-r15 */
L8c01224c: *stop=0x8c01224cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01224e; /* sts.l PR,@-r15 */
L8c01224e: *stop=0x8c01224eu;r[15]+=4294967284u;goto L8c012250; /* add #-0xc,r15 */
L8c012250: *stop=0x8c012250u;r[2]=rd(m,0x8c012464u,4);goto L8c012252; /* mov.l 0x8c012464,r2 */
L8c012252: *stop=0x8c012252u;r[14]=0u;goto L8c012254; /* mov #0x0,r14 */
L8c012254: *stop=0x8c012254u;r[3]=rd(m,0x8c012460u,4);goto L8c012256; /* mov.l 0x8c012460,r3 */
L8c012256: target=r[2];*pr=0x8c01225au;*stop=0x8c012258u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01225a; /* jsr @r2 */
L8c01225a: *stop=0x8c01225au;r[3]=rd(m,0x8c012468u,4);goto L8c01225c; /* mov.l 0x8c012468,r3 */
L8c01225c: *stop=0x8c01225cu;r[4]=r[0];goto L8c01225e; /* mov r0,r4 */
L8c01225e: *stop=0x8c01225eu;r[10]=r[4]&65535u;goto L8c012260; /* extu.w r4,r10 */
L8c012260: *stop=0x8c012260u;r[1]=rd(m,0x8c01246cu,4);goto L8c012262; /* mov.l 0x8c01246c,r1 */
L8c012262: *stop=0x8c012262u;r[12]=4294967295u;goto L8c012264; /* mov #-0x1,r12 */
L8c012264: *stop=0x8c012264u;r[2]=4294967288u;goto L8c012266; /* mov #-0x8,r2 */
L8c012266: *stop=0x8c012266u;r[13]=r[12];goto L8c012268; /* mov r12,r13 */
L8c012268: *stop=0x8c012268u;r[10]&=r[3];goto L8c01226a; /* and r3,r10 */
L8c01226a: *stop=0x8c01226au;r[11]=r[12];goto L8c01226c; /* mov r12,r11 */
L8c01226c: *stop=0x8c01226cu;r[9]=r[4]&255u;goto L8c01226e; /* extu.b r4,r9 */
L8c01226e: *stop=0x8c01226eu;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[10]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[10]=n?(uint32_t)((int32_t)r[10]>>n):((r[10]&0x80000000u)?0xffffffffu:0u);}}goto L8c012270; /* shad r2,r10 */
L8c012270: target=r[1];*pr=0x8c012274u;*stop=0x8c012272u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c012274; /* jsr @r1 */
L8c012274: *stop=0x8c012274u;r[2]=rd(m,0x8c012474u,4);goto L8c012276; /* mov.l 0x8c012474,r2 */
L8c012276: *stop=0x8c012276u;r[3]=rd(m,0x8c012470u,4);goto L8c012278; /* mov.l 0x8c012470,r3 */
L8c012278: target=r[2];*pr=0x8c01227cu;*stop=0x8c01227au;if(!valid_ram(r[3],4u))return -3;r[4]=rd(m,r[3],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01227c; /* jsr @r2 */
L8c01227c: *stop=0x8c01227cu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c01227e; /* tst r0,r0 */
L8c01227e: if((*sr&1u))goto L8c012290;goto L8c012280; /* bt 0x8c012290 */
L8c012280: *stop=0x8c012280u;r[5]=r[9];goto L8c012282; /* mov r9,r5 */
L8c012282: target=0x8c012014u;*pr=0x8c012286u;*stop=0x8c012284u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c012286; /* bsr 0x8c012014 */
L8c012286: *stop=0x8c012286u;r[5]=r[9];goto L8c012288; /* mov r9,r5 */
L8c012288: *stop=0x8c012288u;r[13]=r[0];goto L8c01228a; /* mov r0,r13 */
L8c01228a: target=0x8c011d46u;*pr=0x8c01228eu;*stop=0x8c01228cu;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01228e; /* bsr 0x8c011d46 */
L8c01228e: *stop=0x8c01228eu;r[11]=r[0];goto L8c012290; /* mov r0,r11 */
L8c012290: *stop=0x8c012290u;*sr=(*sr&~1u)|!(r[13]&r[13]);goto L8c012292; /* tst r13,r13 */
L8c012292: if(!(*sr&1u))goto L8c012298;goto L8c012294; /* bf 0x8c012298 */
L8c012294: *stop=0x8c012294u;*sr=(*sr&~1u)|!(r[11]&r[11]);goto L8c012296; /* tst r11,r11 */
L8c012296: if((*sr&1u))goto L8c01234c;goto L8c012298; /* bt 0x8c01234c */
L8c012298: *stop=0x8c012298u;r[5]=6u;goto L8c01229a; /* mov #0x6,r5 */
L8c01229a: *stop=0x8c01229au;r[6]=rd(m,0x8c012478u,4);goto L8c01229c; /* mov.l 0x8c012478,r6 */
L8c01229c: *stop=0x8c01229cu;r[5]<<=8;goto L8c01229e; /* shll8 r5 */
L8c01229e: *stop=0x8c01229eu;r[5]+=4294967200u;goto L8c0122a0; /* add #-0x60,r5 */
L8c0122a0: *stop=0x8c0122a0u;r[4]=r[6];goto L8c0122a2; /* mov r6,r4 */
L8c0122a2: *stop=0x8c0122a2u;r[5]+=r[6];goto L8c0122a4; /* add r6,r5 */
L8c0122a4: *stop=0x8c0122a4u;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c0122a6; /* cmp/hs r5,r4 */
L8c0122a6: if((*sr&1u))goto L8c0122b8;goto L8c0122a8; /* bt 0x8c0122b8 */
L8c0122a8: *stop=0x8c0122a8u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[14],4);goto L8c0122aa; /* mov.l r14,@r4 */
L8c0122aa: *stop=0x8c0122aau;r[4]+=48u;goto L8c0122ac; /* add #0x30,r4 */
L8c0122ac: *stop=0x8c0122acu;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[14],4);goto L8c0122ae; /* mov.l r14,@r4 */
L8c0122ae: *stop=0x8c0122aeu;r[4]+=48u;goto L8c0122b0; /* add #0x30,r4 */
L8c0122b0: *stop=0x8c0122b0u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[14],4);goto L8c0122b2; /* mov.l r14,@r4 */
L8c0122b2: *stop=0x8c0122b2u;r[4]+=48u;goto L8c0122b4; /* add #0x30,r4 */
L8c0122b4: *stop=0x8c0122b4u;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c0122b6; /* cmp/hs r5,r4 */
L8c0122b6: if(!(*sr&1u))goto L8c0122a8;goto L8c0122b8; /* bf 0x8c0122a8 */
L8c0122b8: *stop=0x8c0122b8u;r[0]=0x8c012480u;goto L8c0122ba; /* mova 0x8c012480,r0 */
L8c0122ba: *stop=0x8c0122bau;r[2]=rd(m,0x8c012484u,4);goto L8c0122bc; /* mov.l 0x8c012484,r2 */
L8c0122bc: *stop=0x8c0122bcu;r[3]=rd(m,0x8c01247cu,4);goto L8c0122be; /* mov.l 0x8c01247c,r3 */
L8c0122be: *stop=0x8c0122beu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0x3f800000u;goto L8c0122c0; /* fldi1 fr4 */
L8c0122c0: *stop=0x8c0122c0u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[14],4);goto L8c0122c2; /* mov.l r14,@r3 */
L8c0122c2: *stop=0x8c0122c2u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c0122c4; /* fmov.s @r0,fr3 */
L8c0122c4: *stop=0x8c0122c4u;r[0]=0x8c0124acu;goto L8c0122c6; /* mova 0x8c0124ac,r0 */
L8c0122c6: *stop=0x8c0122c6u;r[3]=rd(m,0x8c01248cu,4);goto L8c0122c8; /* mov.l 0x8c01248c,r3 */
L8c0122c8: *stop=0x8c0122c8u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[2]);goto L8c0122ca; /* fmov.s fr3,@r2 */
L8c0122ca: *stop=0x8c0122cau;r[2]=rd(m,0x8c012490u,4);goto L8c0122cc; /* mov.l 0x8c012490,r2 */
L8c0122cc: *stop=0x8c0122ccu;r[1]=rd(m,0x8c012488u,4);goto L8c0122ce; /* mov.l 0x8c012488,r1 */
L8c0122ce: *stop=0x8c0122ceu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[1]);goto L8c0122d0; /* fmov.s fr4,@r1 */
L8c0122d0: *stop=0x8c0122d0u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[3]);goto L8c0122d2; /* fmov.s fr4,@r3 */
L8c0122d2: *stop=0x8c0122d2u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[2]);goto L8c0122d4; /* fmov.s fr4,@r2 */
L8c0122d4: *stop=0x8c0122d4u;r[3]=rd(m,0x8c012498u,4);goto L8c0122d6; /* mov.l 0x8c012498,r3 */
L8c0122d6: *stop=0x8c0122d6u;r[2]=rd(m,0x8c01249cu,4);goto L8c0122d8; /* mov.l 0x8c01249c,r2 */
L8c0122d8: *stop=0x8c0122d8u;r[1]=rd(m,0x8c012494u,4);goto L8c0122da; /* mov.l 0x8c012494,r1 */
L8c0122da: *stop=0x8c0122dau;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[1]);goto L8c0122dc; /* fmov.s fr4,@r1 */
L8c0122dc: *stop=0x8c0122dcu;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[14],4);goto L8c0122de; /* mov.l r14,@r3 */
L8c0122de: *stop=0x8c0122deu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[14],4);goto L8c0122e0; /* mov.l r14,@r2 */
L8c0122e0: *stop=0x8c0122e0u;r[3]=rd(m,0x8c0124a4u,4);goto L8c0122e2; /* mov.l 0x8c0124a4,r3 */
L8c0122e2: *stop=0x8c0122e2u;r[2]=rd(m,0x8c0124a8u,4);goto L8c0122e4; /* mov.l 0x8c0124a8,r2 */
L8c0122e4: *stop=0x8c0122e4u;r[1]=rd(m,0x8c0124a0u,4);goto L8c0122e6; /* mov.l 0x8c0124a0,r1 */
L8c0122e6: *stop=0x8c0122e6u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[1]);goto L8c0122e8; /* fmov.s fr4,@r1 */
L8c0122e8: *stop=0x8c0122e8u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[3]);goto L8c0122ea; /* fmov.s fr4,@r3 */
L8c0122ea: *stop=0x8c0122eau;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[2]);goto L8c0122ec; /* fmov.s fr4,@r2 */
L8c0122ec: *stop=0x8c0122ecu;r[3]=rd(m,0x8c0124b0u,4);goto L8c0122ee; /* mov.l 0x8c0124b0,r3 */
L8c0122ee: *stop=0x8c0122eeu;r[2]=rd(m,0x8c0124b4u,4);goto L8c0122f0; /* mov.l 0x8c0124b4,r2 */
L8c0122f0: *stop=0x8c0122f0u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c0122f2; /* fmov.s @r0,fr3 */
L8c0122f2: *stop=0x8c0122f2u;r[0]=0x8c0124b8u;goto L8c0122f4; /* mova 0x8c0124b8,r0 */
L8c0122f4: *stop=0x8c0122f4u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[3]);goto L8c0122f6; /* fmov.s fr3,@r3 */
L8c0122f6: *stop=0x8c0122f6u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[2]);goto L8c0122f8; /* fmov.s fr4,@r2 */
L8c0122f8: *stop=0x8c0122f8u;r[1]=rd(m,0x8c0124bcu,4);goto L8c0122fa; /* mov.l 0x8c0124bc,r1 */
L8c0122fa: *stop=0x8c0122fau;r[11]=8u;goto L8c0122fc; /* mov #0x8,r11 */
L8c0122fc: *stop=0x8c0122fcu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c0122fe; /* fmov.s @r0,fr3 */
L8c0122fe: *stop=0x8c0122feu;r[11]<<=8;goto L8c012300; /* shll8 r11 */
L8c012300: *stop=0x8c012300u;r[6]=rd(m,0x8c0124c0u,4);goto L8c012302; /* mov.l 0x8c0124c0,r6 */
L8c012302: *stop=0x8c012302u;r[7]=r[14];goto L8c012304; /* mov r14,r7 */
L8c012304: *stop=0x8c012304u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[1]);goto L8c012306; /* fmov.s fr3,@r1 */
L8c012306: *stop=0x8c012306u;r[1]=4u;goto L8c012308; /* mov #0x4,r1 */
L8c012308: *stop=0x8c012308u;r[13]=r[7];goto L8c01230a; /* mov r7,r13 */
L8c01230a: *stop=0x8c01230au;r[13]<<=8;goto L8c01230c; /* shll8 r13 */
L8c01230c: *stop=0x8c01230cu;r[13]<<=2;goto L8c01230e; /* shll2 r13 */
L8c01230e: *stop=0x8c01230eu;r[3]=rd(m,0x8c0124c4u,4);goto L8c012310; /* mov.l 0x8c0124c4,r3 */
L8c012310: *stop=0x8c012310u;*sr=(*sr&~1u)|(r[13]>>31);r[13]<<=1;goto L8c012312; /* shll r13 */
L8c012312: *stop=0x8c012312u;r[4]=r[14];goto L8c012314; /* mov r14,r4 */
L8c012314: *stop=0x8c012314u;r[13]+=r[3];goto L8c012316; /* add r3,r13 */
L8c012316: *stop=0x8c012316u;r[5]=r[1];goto L8c012318; /* mov r1,r5 */
L8c012318: *stop=0x8c012318u;r[0]=r[13];goto L8c01231a; /* mov r13,r0 */
L8c01231a: *stop=0x8c01231au;if(!valid_ram(r[0]+r[4],4u))return -3;wr(m,r[4]+r[0],r[12],4);goto L8c01231c; /* mov.l r12,@(r0,r4) */
L8c01231c: *stop=0x8c01231cu;r[0]=r[5];goto L8c01231e; /* mov r5,r0 */
L8c01231e: *stop=0x8c01231eu;if(!valid_ram(r[0]+r[13],4u))return -3;wr(m,r[13]+r[0],r[6],4);goto L8c012320; /* mov.l r6,@(r0,r13) */
L8c012320: *stop=0x8c012320u;r[0]=r[13];goto L8c012322; /* mov r13,r0 */
L8c012322: *stop=0x8c012322u;r[4]+=8u;goto L8c012324; /* add #0x8,r4 */
L8c012324: *stop=0x8c012324u;if(!valid_ram(r[0]+r[4],4u))return -3;wr(m,r[4]+r[0],r[12],4);goto L8c012326; /* mov.l r12,@(r0,r4) */
L8c012326: *stop=0x8c012326u;r[5]+=8u;goto L8c012328; /* add #0x8,r5 */
L8c012328: *stop=0x8c012328u;r[4]+=8u;goto L8c01232a; /* add #0x8,r4 */
L8c01232a: *stop=0x8c01232au;r[0]=r[5];goto L8c01232c; /* mov r5,r0 */
L8c01232c: *stop=0x8c01232cu;*sr=(*sr&~1u)|(r[4]>=r[11]);goto L8c01232e; /* cmp/hs r11,r4 */
L8c01232e: *stop=0x8c01232eu;if(!valid_ram(r[0]+r[13],4u))return -3;wr(m,r[13]+r[0],r[6],4);goto L8c012330; /* mov.l r6,@(r0,r13) */
L8c012330: take=!(*sr&1u);*stop=0x8c012332u;r[5]+=8u;if(take)goto L8c012318;goto L8c012334; /* bf/s 0x8c012318 */
L8c012334: *stop=0x8c012334u;r[3]=6u;goto L8c012336; /* mov #0x6,r3 */
L8c012336: *stop=0x8c012336u;r[7]+=1u;goto L8c012338; /* add #0x1,r7 */
L8c012338: *stop=0x8c012338u;*sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[3]);goto L8c01233a; /* cmp/ge r3,r7 */
L8c01233a: if(!(*sr&1u))goto L8c012308;goto L8c01233c; /* bf 0x8c012308 */
L8c01233c: *stop=0x8c01233cu;r[3]=rd(m,0x8c0124c8u,4);goto L8c01233e; /* mov.l 0x8c0124c8,r3 */
L8c01233e: *stop=0x8c01233eu;r[5]=r[9];goto L8c012340; /* mov r9,r5 */
L8c012340: target=r[3];*pr=0x8c012344u;*stop=0x8c012342u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c012344; /* jsr @r3 */
L8c012344: *stop=0x8c012344u;r[2]=rd(m,0x8c0124ccu,4);goto L8c012346; /* mov.l 0x8c0124cc,r2 */
L8c012346: *stop=0x8c012346u;r[5]=r[9];goto L8c012348; /* mov r9,r5 */
L8c012348: target=r[2];*pr=0x8c01234cu;*stop=0x8c01234au;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01234c; /* jsr @r2 */
L8c01234c: *stop=0x8c01234cu;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0x3f800000u;goto L8c01234e; /* fldi1 fr3 */
L8c01234e: *stop=0x8c01234eu;r[0]=4u;goto L8c012350; /* mov #0x4,r0 */
L8c012350: *stop=0x8c012350u;r[4]=rd(m,0x8c0124d0u,4);goto L8c012352; /* mov.l 0x8c0124d0,r4 */
L8c012352: *stop=0x8c012352u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]^=0x80000000u;goto L8c012354; /* fneg fr3 */
L8c012354: *stop=0x8c012354u;r[3]=rd(m,0x8c0124d4u,4);goto L8c012356; /* mov.l 0x8c0124d4,r3 */
L8c012356: *stop=0x8c012356u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][4]=0;goto L8c012358; /* fldi0 fr4 */
L8c012358: *stop=0x8c012358u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[4]);goto L8c01235a; /* fmov.s fr4,@r4 */
L8c01235a: *stop=0x8c01235au;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[4]);goto L8c01235c; /* fmov fr3,@(r0,r4) */
L8c01235c: *stop=0x8c01235cu;r[0]=8u;goto L8c01235e; /* mov #0x8,r0 */
L8c01235e: *stop=0x8c01235eu;if(!valid_ram(r[0]+r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,4u,r[0]+r[4]);goto L8c012360; /* fmov fr4,@(r0,r4) */
L8c012360: target=r[3];*pr=0x8c012364u;*stop=0x8c012362u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c012364; /* jsr @r3 */
L8c012364: *stop=0x8c012364u;r[2]=rd(m,0x8c0124d8u,4);goto L8c012366; /* mov.l 0x8c0124d8,r2 */
L8c012366: target=r[2];*pr=0x8c01236au;*stop=0x8c012368u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01236a; /* jsr @r2 */
L8c01236a: *stop=0x8c01236au;r[2]=rd(m,0x8c012498u,4);goto L8c01236c; /* mov.l 0x8c012498,r2 */
L8c01236c: *stop=0x8c01236cu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c01236e; /* mov.l @r2,r3 */
L8c01236e: *stop=0x8c01236eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c012370; /* tst r3,r3 */
L8c012370: if((*sr&1u))goto L8c01237a;goto L8c012372; /* bt 0x8c01237a */
L8c012372: *stop=0x8c012372u;r[3]=rd(m,0x8c0124dcu,4);goto L8c012374; /* mov.l 0x8c0124dc,r3 */
L8c012374: *stop=0x8c012374u;if(!valid_ram(r[2],4u))return -3;r[5]=rd(m,r[2],4);goto L8c012376; /* mov.l @r2,r5 */
L8c012376: target=r[3];*pr=0x8c01237au;*stop=0x8c012378u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01237a; /* jsr @r3 */
L8c01237a: *stop=0x8c01237au;r[2]=rd(m,0x8c01249cu,4);goto L8c01237c; /* mov.l 0x8c01249c,r2 */
L8c01237c: *stop=0x8c01237cu;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c01237e; /* mov.l @r2,r3 */
L8c01237e: *stop=0x8c01237eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c012380; /* tst r3,r3 */
L8c012380: if((*sr&1u))goto L8c01238a;goto L8c012382; /* bt 0x8c01238a */
L8c012382: *stop=0x8c012382u;r[3]=rd(m,0x8c0124e0u,4);goto L8c012384; /* mov.l 0x8c0124e0,r3 */
L8c012384: *stop=0x8c012384u;if(!valid_ram(r[2],4u))return -3;r[5]=rd(m,r[2],4);goto L8c012386; /* mov.l @r2,r5 */
L8c012386: target=r[3];*pr=0x8c01238au;*stop=0x8c012388u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01238a; /* jsr @r3 */
L8c01238a: *stop=0x8c01238au;r[3]=rd(m,0x8c0124e8u,4);goto L8c01238c; /* mov.l 0x8c0124e8,r3 */
L8c01238c: *stop=0x8c01238cu;r[5]=rd(m,0x8c0124d0u,4);goto L8c01238e; /* mov.l 0x8c0124d0,r5 */
L8c01238e: *stop=0x8c01238eu;r[6]=rd(m,0x8c0124e4u,4);goto L8c012390; /* mov.l 0x8c0124e4,r6 */
L8c012390: target=r[3];*pr=0x8c012394u;*stop=0x8c012392u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c012394; /* jsr @r3 */
L8c012394: *stop=0x8c012394u;r[2]=rd(m,0x8c0124ecu,4);goto L8c012396; /* mov.l 0x8c0124ec,r2 */
L8c012396: target=r[2];*pr=0x8c01239au;*stop=0x8c012398u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01239a; /* jsr @r2 */
L8c01239a: *stop=0x8c01239au;r[3]=rd(m,0x8c0124d4u,4);goto L8c01239c; /* mov.l 0x8c0124d4,r3 */
L8c01239c: target=r[3];*pr=0x8c0123a0u;*stop=0x8c01239eu;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0123a0; /* jsr @r3 */
L8c0123a0: *stop=0x8c0123a0u;r[2]=rd(m,0x8c0124e8u,4);goto L8c0123a2; /* mov.l 0x8c0124e8,r2 */
L8c0123a2: *stop=0x8c0123a2u;r[6]=r[15];goto L8c0123a4; /* mov r15,r6 */
L8c0123a4: *stop=0x8c0123a4u;r[5]=rd(m,0x8c0124e4u,4);goto L8c0123a6; /* mov.l 0x8c0124e4,r5 */
L8c0123a6: target=r[2];*pr=0x8c0123aau;*stop=0x8c0123a8u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0123aa; /* jsr @r2 */
L8c0123aa: *stop=0x8c0123aau;r[3]=rd(m,0x8c0124ecu,4);goto L8c0123ac; /* mov.l 0x8c0124ec,r3 */
L8c0123ac: target=r[3];*pr=0x8c0123b0u;*stop=0x8c0123aeu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0123b0; /* jsr @r3 */
L8c0123b0: *stop=0x8c0123b0u;r[0]=8u;goto L8c0123b2; /* mov #0x8,r0 */
L8c0123b2: *stop=0x8c0123b2u;r[3]=rd(m,0x8c0124f0u,4);goto L8c0123b4; /* mov.l 0x8c0124f0,r3 */
L8c0123b4: *stop=0x8c0123b4u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[15]);goto L8c0123b6; /* fmov.s @(r0,r15),fr6 */
L8c0123b6: *stop=0x8c0123b6u;r[0]=4u;goto L8c0123b8; /* mov #0x4,r0 */
L8c0123b8: *stop=0x8c0123b8u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[15]);goto L8c0123ba; /* fmov.s @(r0,r15),fr5 */
L8c0123ba: target=r[3];*pr=0x8c0123beu;*stop=0x8c0123bcu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[15]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0123be; /* jsr @r3 */
L8c0123be: *stop=0x8c0123beu;r[0]=8u;goto L8c0123c0; /* mov #0x8,r0 */
L8c0123c0: *stop=0x8c0123c0u;r[3]=rd(m,0x8c0124f4u,4);goto L8c0123c2; /* mov.l 0x8c0124f4,r3 */
L8c0123c2: *stop=0x8c0123c2u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[15]);goto L8c0123c4; /* fmov.s @(r0,r15),fr6 */
L8c0123c4: *stop=0x8c0123c4u;r[0]=4u;goto L8c0123c6; /* mov #0x4,r0 */
L8c0123c6: *stop=0x8c0123c6u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[15]);goto L8c0123c8; /* fmov.s @(r0,r15),fr5 */
L8c0123c8: target=r[3];*pr=0x8c0123ccu;*stop=0x8c0123cau;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[15]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0123cc; /* jsr @r3 */
L8c0123cc: *stop=0x8c0123ccu;r[3]=rd(m,0x8c0124bcu,4);goto L8c0123ce; /* mov.l 0x8c0124bc,r3 */
L8c0123ce: *stop=0x8c0123ceu;r[0]=0x8c0124f8u;goto L8c0123d0; /* mova 0x8c0124f8,r0 */
L8c0123d0: *stop=0x8c0123d0u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]);goto L8c0123d2; /* fmov.s @r0,fr3 */
L8c0123d2: *stop=0x8c0123d2u;r[0]=0x8c0124fcu;goto L8c0123d4; /* mova 0x8c0124fc,r0 */
L8c0123d4: *stop=0x8c0123d4u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,2u,r[3]);goto L8c0123d6; /* fmov.s @r3,fr2 */
L8c0123d6: *stop=0x8c0123d6u;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,1u,r[0]);goto L8c0123d8; /* fmov.s @r0,fr1 */
L8c0123d8: *stop=0x8c0123d8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c0123da; /* fadd fr3,fr2 */
L8c0123da: *stop=0x8c0123dau;r[2]=rd(m,0x8c012500u,4);goto L8c0123dc; /* mov.l 0x8c012500,r2 */
L8c0123dc: *stop=0x8c0123dcu;r[3]=rd(m,0x8c0124b4u,4);goto L8c0123de; /* mov.l 0x8c0124b4,r3 */
L8c0123de: *stop=0x8c0123deu;fcopy(f,5u,2u);goto L8c0123e0; /* fmov fr2,fr5 */
L8c0123e0: *stop=0x8c0123e0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[5],&fr[1],&f->fpscr,&fr[5]);if(status)return status;}goto L8c0123e2; /* fmul fr1,fr5 */
L8c0123e2: target=r[2];*pr=0x8c0123e6u;*stop=0x8c0123e4u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[3]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0123e6; /* jsr @r2 */
L8c0123e6: *stop=0x8c0123e6u;r[3]=rd(m,0x8c0124a4u,4);goto L8c0123e8; /* mov.l 0x8c0124a4,r3 */
L8c0123e8: *stop=0x8c0123e8u;r[1]=rd(m,0x8c0124a0u,4);goto L8c0123ea; /* mov.l 0x8c0124a0,r1 */
L8c0123ea: *stop=0x8c0123eau;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[3]);goto L8c0123ec; /* fmov.s @r3,fr5 */
L8c0123ec: *stop=0x8c0123ecu;r[3]=rd(m,0x8c012504u,4);goto L8c0123ee; /* mov.l 0x8c012504,r3 */
L8c0123ee: *stop=0x8c0123eeu;r[2]=rd(m,0x8c0124a8u,4);goto L8c0123f0; /* mov.l 0x8c0124a8,r2 */
L8c0123f0: *stop=0x8c0123f0u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[1]);goto L8c0123f2; /* fmov.s @r1,fr6 */
L8c0123f2: target=r[3];*pr=0x8c0123f6u;*stop=0x8c0123f4u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[2]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0123f6; /* jsr @r3 */
L8c0123f6: *stop=0x8c0123f6u;r[2]=rd(m,0x8c01250cu,4);goto L8c0123f8; /* mov.l 0x8c01250c,r2 */
L8c0123f8: *stop=0x8c0123f8u;r[4]=rd(m,0x8c012508u,4);goto L8c0123fa; /* mov.l 0x8c012508,r4 */
L8c0123fa: target=r[2];*pr=0x8c0123feu;*stop=0x8c0123fcu;r[5]=19u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0123fe; /* jsr @r2 */
L8c0123fe: *stop=0x8c0123feu;r[3]=rd(m,0x8c0124bcu,4);goto L8c012400; /* mov.l 0x8c0124bc,r3 */
L8c012400: *stop=0x8c012400u;r[2]=rd(m,0x8c0124b4u,4);goto L8c012402; /* mov.l 0x8c0124b4,r2 */
L8c012402: *stop=0x8c012402u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[3]);goto L8c012404; /* fmov.s @r3,fr6 */
L8c012404: *stop=0x8c012404u;r[4]=rd(m,0x8c012508u,4);goto L8c012406; /* mov.l 0x8c012508,r4 */
L8c012406: *stop=0x8c012406u;r[3]=rd(m,0x8c012510u,4);goto L8c012408; /* mov.l 0x8c012510,r3 */
L8c012408: *stop=0x8c012408u;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[2]);goto L8c01240a; /* fmov.s @r2,fr5 */
L8c01240a: *stop=0x8c01240au;r[1]=rd(m,0x8c0124b0u,4);goto L8c01240c; /* mov.l 0x8c0124b0,r1 */
L8c01240c: target=r[3];*pr=0x8c012410u;*stop=0x8c01240eu;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[1]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c012410; /* jsr @r3 */
L8c012410: *stop=0x8c012410u;r[3]=rd(m,0x8c0124a4u,4);goto L8c012412; /* mov.l 0x8c0124a4,r3 */
L8c012412: *stop=0x8c012412u;r[2]=rd(m,0x8c0124a0u,4);goto L8c012414; /* mov.l 0x8c0124a0,r2 */
L8c012414: *stop=0x8c012414u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[3]);goto L8c012416; /* fmov.s @r3,fr5 */
L8c012416: *stop=0x8c012416u;r[4]=rd(m,0x8c012508u,4);goto L8c012418; /* mov.l 0x8c012508,r4 */
L8c012418: *stop=0x8c012418u;r[3]=rd(m,0x8c012514u,4);goto L8c01241a; /* mov.l 0x8c012514,r3 */
L8c01241a: *stop=0x8c01241au;if(!valid_ram(r[2],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[2]);goto L8c01241c; /* fmov.s @r2,fr6 */
L8c01241c: *stop=0x8c01241cu;r[1]=rd(m,0x8c0124a8u,4);goto L8c01241e; /* mov.l 0x8c0124a8,r1 */
L8c01241e: target=r[3];*pr=0x8c012422u;*stop=0x8c012420u;if(!valid_ram(r[1],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[1]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c012422; /* jsr @r3 */
L8c012422: *stop=0x8c012422u;r[5]=11u;goto L8c012424; /* mov #0xb,r5 */
L8c012424: *stop=0x8c012424u;r[4]=rd(m,0x8c012508u,4);goto L8c012426; /* mov.l 0x8c012508,r4 */
L8c012426: *stop=0x8c012426u;r[6]=50u;goto L8c012428; /* mov #0x32,r6 */
L8c012428: *stop=0x8c012428u;r[2]=rd(m,0x8c012518u,4);goto L8c01242a; /* mov.l 0x8c012518,r2 */
L8c01242a: *stop=0x8c01242au;r[5]<<=8;goto L8c01242c; /* shll8 r5 */
L8c01242c: *stop=0x8c01242cu;r[6]<<=8;goto L8c01242e; /* shll8 r6 */
L8c01242e: *stop=0x8c01242eu;r[6]+=4294967239u;goto L8c012430; /* add #-0x39,r6 */
L8c012430: *stop=0x8c012430u;r[5]+=4294967210u;goto L8c012432; /* add #-0x56,r5 */
L8c012432: target=r[2];*pr=0x8c012436u;*stop=0x8c012434u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c012436; /* jsr @r2 */
L8c012436: *stop=0x8c012436u;r[14]=rd(m,0x8c0124e4u,4);goto L8c012438; /* mov.l 0x8c0124e4,r14 */
L8c012438: *stop=0x8c012438u;r[0]=8u;goto L8c01243a; /* mov #0x8,r0 */
L8c01243a: *stop=0x8c01243au;r[3]=rd(m,0x8c01251cu,4);goto L8c01243c; /* mov.l 0x8c01251c,r3 */
L8c01243c: *stop=0x8c01243cu;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[0]+r[14]);goto L8c01243e; /* fmov.s @(r0,r14),fr6 */
L8c01243e: *stop=0x8c01243eu;r[0]=4u;goto L8c012440; /* mov #0x4,r0 */
L8c012440: *stop=0x8c012440u;r[4]=rd(m,0x8c012508u,4);goto L8c012442; /* mov.l 0x8c012508,r4 */
L8c012442: *stop=0x8c012442u;if(!valid_ram(r[0]+r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[0]+r[14]);goto L8c012444; /* fmov.s @(r0,r14),fr5 */
L8c012444: target=r[3];*pr=0x8c012448u;*stop=0x8c012446u;if(!valid_ram(r[14],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[14]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c012448; /* jsr @r3 */
L8c012448: *stop=0x8c012448u;r[15]+=12u;goto L8c01244a; /* add #0xc,r15 */
L8c01244a: *stop=0x8c01244au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01244c; /* lds.l @r15+,PR */
L8c01244c: *stop=0x8c01244cu;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c01244e; /* mov.l @r15+,r9 */
L8c01244e: *stop=0x8c01244eu;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c012450; /* mov.l @r15+,r10 */
L8c012450: *stop=0x8c012450u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c012452; /* mov.l @r15+,r11 */
L8c012452: *stop=0x8c012452u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c012454; /* mov.l @r15+,r12 */
L8c012454: *stop=0x8c012454u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c012456; /* mov.l @r15+,r13 */
L8c012456: *stop=0x8c012456u;if(*pr!=entry_pr)return -3;*stop=0x8c012458u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_scene_8c602df2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c602df2;
L8c602df2: *stop=0x8c602df2u;r[3]=rd(m,0x8c602f3cu,4);goto L8c602df4; /* mov.l 0x8c602f3c,r3 */
L8c602df4: target=r[3];*stop=0x8c602df6u;if(!call)return -2;return call(ctx,target,m,f,sr,r,pr); /* jmp @r3 */
}

SA_STAGE_EXPORT int sa_camera_scene_8c64826e(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c64826e;
L8c64826e: *stop=0x8c64826eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c648270; /* sts.l PR,@-r15 */
L8c648270: *stop=0x8c648270u;r[15]+=4294967292u;goto L8c648272; /* add #-0x4,r15 */
L8c648272: *stop=0x8c648272u;r[3]=sx(m,0x8c648360u,2);goto L8c648274; /* mov.w 0x8c648360,r3 */
L8c648274: *stop=0x8c648274u;r[2]=rd(m,0x8c648368u,4);goto L8c648276; /* mov.l 0x8c648368,r2 */
L8c648276: *stop=0x8c648276u;*macl=r[3]*r[4];goto L8c648278; /* mul.l r3,r4 */
L8c648278: *stop=0x8c648278u;r[1]=rd(m,0x8c64836cu,4);goto L8c64827a; /* mov.l 0x8c64836c,r1 */
L8c64827a: *stop=0x8c64827au;r[3]=*macl;goto L8c64827c; /* sts MACL,r3 */
L8c64827c: *stop=0x8c64827cu;r[3]+=r[2];goto L8c64827e; /* add r2,r3 */
L8c64827e: target=r[1];*pr=0x8c648282u;*stop=0x8c648280u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c648282; /* jsr @r1 */
L8c648282: *stop=0x8c648282u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c648284; /* tst r0,r0 */
L8c648284: if((*sr&1u))goto L8c648296;goto L8c648286; /* bt 0x8c648296 */
L8c648286: *stop=0x8c648286u;if(!valid_ram(r[15],4u))return -3;r[2]=rd(m,r[15],4);goto L8c648288; /* mov.l @r15,r2 */
L8c648288: *stop=0x8c648288u;if(!valid_ram(r[2]+8u,1u))return -3;r[0]=sx(m,r[2]+0x8u,1);goto L8c64828a; /* mov.b @(0x8,r2),r0 */
L8c64828a: *stop=0x8c64828au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c64828c; /* tst r0,r0 */
L8c64828c: if((*sr&1u))goto L8c648296;goto L8c64828e; /* bt 0x8c648296 */
L8c64828e: *stop=0x8c64828eu;r[15]+=4u;goto L8c648290; /* add #0x4,r15 */
L8c648290: *stop=0x8c648290u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c648292; /* lds.l @r15+,PR */
L8c648292: *stop=0x8c648292u;if(*pr!=entry_pr)return -3;*stop=0x8c648294u;r[0]=1u;return 0; /* rts */
L8c648296: *stop=0x8c648296u;r[0]=0u;goto L8c648298; /* mov #0x0,r0 */
L8c648298: *stop=0x8c648298u;r[15]+=4u;goto L8c64829a; /* add #0x4,r15 */
L8c64829a: *stop=0x8c64829au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c64829c; /* lds.l @r15+,PR */
L8c64829c: *stop=0x8c64829cu;if(*pr!=entry_pr)return -3;*stop=0x8c64829eu;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_scene_8c014242(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c014242;
L8c014242: *stop=0x8c014242u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c014244; /* mov.l r14,@-r15 */
L8c014244: *stop=0x8c014244u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c014246; /* mov.l r13,@-r15 */
L8c014246: *stop=0x8c014246u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c014248; /* sts.l PR,@-r15 */
L8c014248: *stop=0x8c014248u;r[15]+=4294967276u;goto L8c01424a; /* add #-0x14,r15 */
L8c01424a: *stop=0x8c01424au;r[3]=rd(m,0x8c014398u,4);goto L8c01424c; /* mov.l 0x8c014398,r3 */
L8c01424c: *stop=0x8c01424cu;r[2]=r[15];goto L8c01424e; /* mov r15,r2 */
L8c01424e: *stop=0x8c01424eu;r[2]+=4u;goto L8c014250; /* add #0x4,r2 */
L8c014250: *stop=0x8c014250u;r[14]=r[15];goto L8c014252; /* mov r15,r14 */
L8c014252: *stop=0x8c014252u;if(!valid_ram(r[3],1u))return -3;r[0]=sx(m,r[3],1);goto L8c014254; /* mov.b @r3,r0 */
L8c014254: *stop=0x8c014254u;r[14]+=4u;goto L8c014256; /* add #0x4,r14 */
L8c014256: *stop=0x8c014256u;if(!valid_ram(r[2],1u))return -3;wr(m,r[2],r[0],1);goto L8c014258; /* mov.b r0,@r2 */
L8c014258: *stop=0x8c014258u;if(!valid_ram(r[3]+1u,1u))return -3;r[0]=sx(m,r[3]+0x1u,1);goto L8c01425a; /* mov.b @(0x1,r3),r0 */
L8c01425a: *stop=0x8c01425au;if(!valid_ram(r[2]+1u,1u))return -3;wr(m,r[2]+0x1u,r[0],1);goto L8c01425c; /* mov.b r0,@(0x1,r2) */
L8c01425c: *stop=0x8c01425cu;if(!valid_ram(r[3]+2u,1u))return -3;r[0]=sx(m,r[3]+0x2u,1);goto L8c01425e; /* mov.b @(0x2,r3),r0 */
L8c01425e: *stop=0x8c01425eu;if(!valid_ram(r[2]+2u,1u))return -3;wr(m,r[2]+0x2u,r[0],1);goto L8c014260; /* mov.b r0,@(0x2,r2) */
L8c014260: *stop=0x8c014260u;if(!valid_ram(r[3]+3u,1u))return -3;r[0]=sx(m,r[3]+0x3u,1);goto L8c014262; /* mov.b @(0x3,r3),r0 */
L8c014262: *stop=0x8c014262u;if(!valid_ram(r[2]+3u,1u))return -3;wr(m,r[2]+0x3u,r[0],1);goto L8c014264; /* mov.b r0,@(0x3,r2) */
L8c014264: *stop=0x8c014264u;if(!valid_ram(r[3]+4u,1u))return -3;r[0]=sx(m,r[3]+0x4u,1);goto L8c014266; /* mov.b @(0x4,r3),r0 */
L8c014266: *stop=0x8c014266u;if(!valid_ram(r[2]+4u,1u))return -3;wr(m,r[2]+0x4u,r[0],1);goto L8c014268; /* mov.b r0,@(0x4,r2) */
L8c014268: *stop=0x8c014268u;if(!valid_ram(r[3]+5u,1u))return -3;r[0]=sx(m,r[3]+0x5u,1);goto L8c01426a; /* mov.b @(0x5,r3),r0 */
L8c01426a: *stop=0x8c01426au;if(!valid_ram(r[2]+5u,1u))return -3;wr(m,r[2]+0x5u,r[0],1);goto L8c01426c; /* mov.b r0,@(0x5,r2) */
L8c01426c: *stop=0x8c01426cu;if(!valid_ram(r[3]+6u,1u))return -3;r[0]=sx(m,r[3]+0x6u,1);goto L8c01426e; /* mov.b @(0x6,r3),r0 */
L8c01426e: *stop=0x8c01426eu;if(!valid_ram(r[2]+6u,1u))return -3;wr(m,r[2]+0x6u,r[0],1);goto L8c014270; /* mov.b r0,@(0x6,r2) */
L8c014270: *stop=0x8c014270u;if(!valid_ram(r[3]+7u,1u))return -3;r[0]=sx(m,r[3]+0x7u,1);goto L8c014272; /* mov.b @(0x7,r3),r0 */
L8c014272: *stop=0x8c014272u;if(!valid_ram(r[2]+7u,1u))return -3;wr(m,r[2]+0x7u,r[0],1);goto L8c014274; /* mov.b r0,@(0x7,r2) */
L8c014274: *stop=0x8c014274u;if(!valid_ram(r[3]+8u,1u))return -3;r[0]=sx(m,r[3]+0x8u,1);goto L8c014276; /* mov.b @(0x8,r3),r0 */
L8c014276: *stop=0x8c014276u;if(!valid_ram(r[2]+8u,1u))return -3;wr(m,r[2]+0x8u,r[0],1);goto L8c014278; /* mov.b r0,@(0x8,r2) */
L8c014278: *stop=0x8c014278u;if(!valid_ram(r[3]+9u,1u))return -3;r[0]=sx(m,r[3]+0x9u,1);goto L8c01427a; /* mov.b @(0x9,r3),r0 */
L8c01427a: *stop=0x8c01427au;if(!valid_ram(r[2]+9u,1u))return -3;wr(m,r[2]+0x9u,r[0],1);goto L8c01427c; /* mov.b r0,@(0x9,r2) */
L8c01427c: *stop=0x8c01427cu;if(!valid_ram(r[3]+10u,1u))return -3;r[0]=sx(m,r[3]+0xau,1);goto L8c01427e; /* mov.b @(0xa,r3),r0 */
L8c01427e: *stop=0x8c01427eu;if(!valid_ram(r[2]+10u,1u))return -3;wr(m,r[2]+0xau,r[0],1);goto L8c014280; /* mov.b r0,@(0xa,r2) */
L8c014280: *stop=0x8c014280u;if(!valid_ram(r[3]+11u,1u))return -3;r[0]=sx(m,r[3]+0xbu,1);goto L8c014282; /* mov.b @(0xb,r3),r0 */
L8c014282: *stop=0x8c014282u;if(!valid_ram(r[2]+11u,1u))return -3;wr(m,r[2]+0xbu,r[0],1);goto L8c014284; /* mov.b r0,@(0xb,r2) */
L8c014284: *stop=0x8c014284u;if(!valid_ram(r[3]+12u,1u))return -3;r[0]=sx(m,r[3]+0xcu,1);goto L8c014286; /* mov.b @(0xc,r3),r0 */
L8c014286: *stop=0x8c014286u;if(!valid_ram(r[2]+12u,1u))return -3;wr(m,r[2]+0xcu,r[0],1);goto L8c014288; /* mov.b r0,@(0xc,r2) */
L8c014288: *stop=0x8c014288u;if(!valid_ram(r[3]+13u,1u))return -3;r[0]=sx(m,r[3]+0xdu,1);goto L8c01428a; /* mov.b @(0xd,r3),r0 */
L8c01428a: *stop=0x8c01428au;if(!valid_ram(r[2]+13u,1u))return -3;wr(m,r[2]+0xdu,r[0],1);goto L8c01428c; /* mov.b r0,@(0xd,r2) */
L8c01428c: *stop=0x8c01428cu;r[1]=r[4]&65535u;goto L8c01428e; /* extu.w r4,r1 */
L8c01428e: *stop=0x8c01428eu;if(!valid_ram(r[3]+14u,1u))return -3;r[0]=sx(m,r[3]+0xeu,1);goto L8c014290; /* mov.b @(0xe,r3),r0 */
L8c014290: *stop=0x8c014290u;r[6]=r[14];goto L8c014292; /* mov r14,r6 */
L8c014292: *stop=0x8c014292u;r[13]=rd(m,0x8c01439cu,4);goto L8c014294; /* mov.l 0x8c01439c,r13 */
L8c014294: *stop=0x8c014294u;if(!valid_ram(r[2]+14u,1u))return -3;wr(m,r[2]+0xeu,r[0],1);goto L8c014296; /* mov.b r0,@(0xe,r2) */
L8c014296: *stop=0x8c014296u;if(!valid_ram(r[3]+15u,1u))return -3;r[0]=sx(m,r[3]+0xfu,1);goto L8c014298; /* mov.b @(0xf,r3),r0 */
L8c014298: *stop=0x8c014298u;if(!valid_ram(r[2]+15u,1u))return -3;wr(m,r[2]+0xfu,r[0],1);goto L8c01429a; /* mov.b r0,@(0xf,r2) */
L8c01429a: *stop=0x8c01429au;r[2]=9u;goto L8c01429c; /* mov #0x9,r2 */
L8c01429c: *stop=0x8c01429cu;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[2]);goto L8c01429e; /* cmp/gt r2,r1 */
L8c01429e: take=(*sr&1u);*stop=0x8c0142a0u;r[6]+=3u;if(take)goto L8c0142a6;goto L8c0142a2; /* bt/s 0x8c0142a6 */
L8c0142a2: *stop=0x8c0142a4u;r[4]+=48u;goto L8c0142a8; /* bra 0x8c0142a8 */
L8c0142a6: *stop=0x8c0142a6u;r[4]+=55u;goto L8c0142a8; /* add #0x37,r4 */
L8c0142a8: *stop=0x8c0142a8u;if(!valid_ram(r[6],1u))return -3;wr(m,r[6],r[4],1);goto L8c0142aa; /* mov.b r4,@r6 */
L8c0142aa: *stop=0x8c0142aau;r[4]=r[14];goto L8c0142ac; /* mov r14,r4 */
L8c0142ac: *stop=0x8c0142acu;r[4]+=3u;goto L8c0142ae; /* add #0x3,r4 */
L8c0142ae: *stop=0x8c0142aeu;if(!valid_ram(r[4],1u))return -3;r[3]=sx(m,r[4],1);goto L8c0142b0; /* mov.b @r4,r3 */
L8c0142b0: *stop=0x8c0142b0u;r[2]=90u;goto L8c0142b2; /* mov #0x5a,r2 */
L8c0142b2: *stop=0x8c0142b2u;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[2]);goto L8c0142b4; /* cmp/gt r2,r3 */
L8c0142b4: if(!(*sr&1u))goto L8c0142c0;goto L8c0142b6; /* bf 0x8c0142c0 */
L8c0142b6: *stop=0x8c0142b6u;if(!valid_ram(r[4],1u))return -3;r[1]=sx(m,r[4],1);goto L8c0142b8; /* mov.b @r4,r1 */
L8c0142b8: *stop=0x8c0142b8u;r[0]=49u;goto L8c0142ba; /* mov #0x31,r0 */
L8c0142ba: *stop=0x8c0142bau;r[1]+=4294967270u;goto L8c0142bc; /* add #-0x1a,r1 */
L8c0142bc: *stop=0x8c0142bcu;if(!valid_ram(r[4],1u))return -3;wr(m,r[4],r[1],1);goto L8c0142be; /* mov.b r1,@r4 */
L8c0142be: *stop=0x8c0142beu;if(!valid_ram(r[14]+2u,1u))return -3;wr(m,r[14]+0x2u,r[0],1);goto L8c0142c0; /* mov.b r0,@(0x2,r14) */
L8c0142c0: *stop=0x8c0142c0u;r[0]=66u;goto L8c0142c2; /* mov #0x42,r0 */
L8c0142c2: *stop=0x8c0142c2u;r[3]=r[14];goto L8c0142c4; /* mov r14,r3 */
L8c0142c4: *stop=0x8c0142c4u;r[3]+=4u;goto L8c0142c6; /* add #0x4,r3 */
L8c0142c6: *stop=0x8c0142c6u;r[5]+=48u;goto L8c0142c8; /* add #0x30,r5 */
L8c0142c8: *stop=0x8c0142c8u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto L8c0142ca; /* mov.l r3,@r15 */
L8c0142ca: *stop=0x8c0142cau;if(!valid_ram(r[3],1u))return -3;wr(m,r[3],r[5],1);goto L8c0142cc; /* mov.b r5,@r3 */
L8c0142cc: *stop=0x8c0142ccu;r[5]=r[13];goto L8c0142ce; /* mov r13,r5 */
L8c0142ce: *stop=0x8c0142ceu;if(!valid_ram(r[14]+5u,1u))return -3;wr(m,r[14]+0x5u,r[0],1);goto L8c0142d0; /* mov.b r0,@(0x5,r14) */
L8c0142d0: *stop=0x8c0142d0u;r[0]=46u;goto L8c0142d2; /* mov #0x2e,r0 */
L8c0142d2: *stop=0x8c0142d2u;if(!valid_ram(r[14]+6u,1u))return -3;wr(m,r[14]+0x6u,r[0],1);goto L8c0142d4; /* mov.b r0,@(0x6,r14) */
L8c0142d4: *stop=0x8c0142d4u;r[0]=98u;goto L8c0142d6; /* mov #0x62,r0 */
L8c0142d6: *stop=0x8c0142d6u;if(!valid_ram(r[14]+7u,1u))return -3;wr(m,r[14]+0x7u,r[0],1);goto L8c0142d8; /* mov.b r0,@(0x7,r14) */
L8c0142d8: *stop=0x8c0142d8u;r[0]=105u;goto L8c0142da; /* mov #0x69,r0 */
L8c0142da: *stop=0x8c0142dau;if(!valid_ram(r[14]+8u,1u))return -3;wr(m,r[14]+0x8u,r[0],1);goto L8c0142dc; /* mov.b r0,@(0x8,r14) */
L8c0142dc: *stop=0x8c0142dcu;r[0]=110u;goto L8c0142de; /* mov #0x6e,r0 */
L8c0142de: *stop=0x8c0142deu;if(!valid_ram(r[14]+9u,1u))return -3;wr(m,r[14]+0x9u,r[0],1);goto L8c0142e0; /* mov.b r0,@(0x9,r14) */
L8c0142e0: *stop=0x8c0142e0u;r[0]=0u;goto L8c0142e2; /* mov #0x0,r0 */
L8c0142e2: *stop=0x8c0142e2u;if(!valid_ram(r[14]+10u,1u))return -3;wr(m,r[14]+0xau,r[0],1);goto L8c0142e4; /* mov.b r0,@(0xa,r14) */
L8c0142e4: target=0x8c0141e0u;*pr=0x8c0142e8u;*stop=0x8c0142e6u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0142e8; /* bsr 0x8c0141e0 */
L8c0142e8: *stop=0x8c0142e8u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c0142ea; /* cmp/eq #0x1,r0 */
L8c0142ea: take=(*sr&1u);*stop=0x8c0142ecu;r[4]=r[0];if(take)goto L8c0142fc;goto L8c0142ee; /* bt/s 0x8c0142fc */
L8c0142ee: *stop=0x8c0142eeu;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto L8c0142f0; /* mov.l @r15,r3 */
L8c0142f0: *stop=0x8c0142f0u;r[2]=48u;goto L8c0142f2; /* mov #0x30,r2 */
L8c0142f2: *stop=0x8c0142f2u;r[5]=r[13];goto L8c0142f4; /* mov r13,r5 */
L8c0142f4: *stop=0x8c0142f4u;if(!valid_ram(r[3],1u))return -3;wr(m,r[3],r[2],1);goto L8c0142f6; /* mov.b r2,@r3 */
L8c0142f6: target=0x8c0141e0u;*pr=0x8c0142fau;*stop=0x8c0142f8u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0142fa; /* bsr 0x8c0141e0 */
L8c0142fa: *stop=0x8c0142fau;r[4]=r[0];goto L8c0142fc; /* mov r0,r4 */
L8c0142fc: *stop=0x8c0142fcu;r[0]=r[4];goto L8c0142fe; /* mov r4,r0 */
L8c0142fe: *stop=0x8c0142feu;r[15]+=20u;goto L8c014300; /* add #0x14,r15 */
L8c014300: *stop=0x8c014300u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c014302; /* lds.l @r15+,PR */
L8c014302: *stop=0x8c014302u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c014304; /* mov.l @r15+,r13 */
L8c014304: *stop=0x8c014304u;if(*pr!=entry_pr)return -3;*stop=0x8c014306u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_scene_8c014308(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c014308;
L8c014308: *stop=0x8c014308u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c01430a; /* mov.l r14,@-r15 */
L8c01430a: *stop=0x8c01430au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c01430c; /* mov.l r13,@-r15 */
L8c01430c: *stop=0x8c01430cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01430e; /* sts.l PR,@-r15 */
L8c01430e: *stop=0x8c01430eu;r[15]+=4294967276u;goto L8c014310; /* add #-0x14,r15 */
L8c014310: *stop=0x8c014310u;r[3]=rd(m,0x8c0143a0u,4);goto L8c014312; /* mov.l 0x8c0143a0,r3 */
L8c014312: *stop=0x8c014312u;r[2]=r[15];goto L8c014314; /* mov r15,r2 */
L8c014314: *stop=0x8c014314u;r[2]+=4u;goto L8c014316; /* add #0x4,r2 */
L8c014316: *stop=0x8c014316u;r[14]=r[15];goto L8c014318; /* mov r15,r14 */
L8c014318: *stop=0x8c014318u;if(!valid_ram(r[3],1u))return -3;r[0]=sx(m,r[3],1);goto L8c01431a; /* mov.b @r3,r0 */
L8c01431a: *stop=0x8c01431au;r[14]+=4u;goto L8c01431c; /* add #0x4,r14 */
L8c01431c: *stop=0x8c01431cu;if(!valid_ram(r[2],1u))return -3;wr(m,r[2],r[0],1);goto L8c01431e; /* mov.b r0,@r2 */
L8c01431e: *stop=0x8c01431eu;if(!valid_ram(r[3]+1u,1u))return -3;r[0]=sx(m,r[3]+0x1u,1);goto L8c014320; /* mov.b @(0x1,r3),r0 */
L8c014320: *stop=0x8c014320u;if(!valid_ram(r[2]+1u,1u))return -3;wr(m,r[2]+0x1u,r[0],1);goto L8c014322; /* mov.b r0,@(0x1,r2) */
L8c014322: *stop=0x8c014322u;if(!valid_ram(r[3]+2u,1u))return -3;r[0]=sx(m,r[3]+0x2u,1);goto L8c014324; /* mov.b @(0x2,r3),r0 */
L8c014324: *stop=0x8c014324u;if(!valid_ram(r[2]+2u,1u))return -3;wr(m,r[2]+0x2u,r[0],1);goto L8c014326; /* mov.b r0,@(0x2,r2) */
L8c014326: *stop=0x8c014326u;if(!valid_ram(r[3]+3u,1u))return -3;r[0]=sx(m,r[3]+0x3u,1);goto L8c014328; /* mov.b @(0x3,r3),r0 */
L8c014328: *stop=0x8c014328u;if(!valid_ram(r[2]+3u,1u))return -3;wr(m,r[2]+0x3u,r[0],1);goto L8c01432a; /* mov.b r0,@(0x3,r2) */
L8c01432a: *stop=0x8c01432au;if(!valid_ram(r[3]+4u,1u))return -3;r[0]=sx(m,r[3]+0x4u,1);goto L8c01432c; /* mov.b @(0x4,r3),r0 */
L8c01432c: *stop=0x8c01432cu;if(!valid_ram(r[2]+4u,1u))return -3;wr(m,r[2]+0x4u,r[0],1);goto L8c01432e; /* mov.b r0,@(0x4,r2) */
L8c01432e: *stop=0x8c01432eu;if(!valid_ram(r[3]+5u,1u))return -3;r[0]=sx(m,r[3]+0x5u,1);goto L8c014330; /* mov.b @(0x5,r3),r0 */
L8c014330: *stop=0x8c014330u;if(!valid_ram(r[2]+5u,1u))return -3;wr(m,r[2]+0x5u,r[0],1);goto L8c014332; /* mov.b r0,@(0x5,r2) */
L8c014332: *stop=0x8c014332u;if(!valid_ram(r[3]+6u,1u))return -3;r[0]=sx(m,r[3]+0x6u,1);goto L8c014334; /* mov.b @(0x6,r3),r0 */
L8c014334: *stop=0x8c014334u;if(!valid_ram(r[2]+6u,1u))return -3;wr(m,r[2]+0x6u,r[0],1);goto L8c014336; /* mov.b r0,@(0x6,r2) */
L8c014336: *stop=0x8c014336u;if(!valid_ram(r[3]+7u,1u))return -3;r[0]=sx(m,r[3]+0x7u,1);goto L8c014338; /* mov.b @(0x7,r3),r0 */
L8c014338: *stop=0x8c014338u;if(!valid_ram(r[2]+7u,1u))return -3;wr(m,r[2]+0x7u,r[0],1);goto L8c01433a; /* mov.b r0,@(0x7,r2) */
L8c01433a: *stop=0x8c01433au;if(!valid_ram(r[3]+8u,1u))return -3;r[0]=sx(m,r[3]+0x8u,1);goto L8c01433c; /* mov.b @(0x8,r3),r0 */
L8c01433c: *stop=0x8c01433cu;if(!valid_ram(r[2]+8u,1u))return -3;wr(m,r[2]+0x8u,r[0],1);goto L8c01433e; /* mov.b r0,@(0x8,r2) */
L8c01433e: *stop=0x8c01433eu;if(!valid_ram(r[3]+9u,1u))return -3;r[0]=sx(m,r[3]+0x9u,1);goto L8c014340; /* mov.b @(0x9,r3),r0 */
L8c014340: *stop=0x8c014340u;if(!valid_ram(r[2]+9u,1u))return -3;wr(m,r[2]+0x9u,r[0],1);goto L8c014342; /* mov.b r0,@(0x9,r2) */
L8c014342: *stop=0x8c014342u;if(!valid_ram(r[3]+10u,1u))return -3;r[0]=sx(m,r[3]+0xau,1);goto L8c014344; /* mov.b @(0xa,r3),r0 */
L8c014344: *stop=0x8c014344u;if(!valid_ram(r[2]+10u,1u))return -3;wr(m,r[2]+0xau,r[0],1);goto L8c014346; /* mov.b r0,@(0xa,r2) */
L8c014346: *stop=0x8c014346u;if(!valid_ram(r[3]+11u,1u))return -3;r[0]=sx(m,r[3]+0xbu,1);goto L8c014348; /* mov.b @(0xb,r3),r0 */
L8c014348: *stop=0x8c014348u;if(!valid_ram(r[2]+11u,1u))return -3;wr(m,r[2]+0xbu,r[0],1);goto L8c01434a; /* mov.b r0,@(0xb,r2) */
L8c01434a: *stop=0x8c01434au;if(!valid_ram(r[3]+12u,1u))return -3;r[0]=sx(m,r[3]+0xcu,1);goto L8c01434c; /* mov.b @(0xc,r3),r0 */
L8c01434c: *stop=0x8c01434cu;if(!valid_ram(r[2]+12u,1u))return -3;wr(m,r[2]+0xcu,r[0],1);goto L8c01434e; /* mov.b r0,@(0xc,r2) */
L8c01434e: *stop=0x8c01434eu;if(!valid_ram(r[3]+13u,1u))return -3;r[0]=sx(m,r[3]+0xdu,1);goto L8c014350; /* mov.b @(0xd,r3),r0 */
L8c014350: *stop=0x8c014350u;if(!valid_ram(r[2]+13u,1u))return -3;wr(m,r[2]+0xdu,r[0],1);goto L8c014352; /* mov.b r0,@(0xd,r2) */
L8c014352: *stop=0x8c014352u;r[1]=r[4]&65535u;goto L8c014354; /* extu.w r4,r1 */
L8c014354: *stop=0x8c014354u;if(!valid_ram(r[3]+14u,1u))return -3;r[0]=sx(m,r[3]+0xeu,1);goto L8c014356; /* mov.b @(0xe,r3),r0 */
L8c014356: *stop=0x8c014356u;r[6]=r[14];goto L8c014358; /* mov r14,r6 */
L8c014358: *stop=0x8c014358u;r[13]=rd(m,0x8c0143a4u,4);goto L8c01435a; /* mov.l 0x8c0143a4,r13 */
L8c01435a: *stop=0x8c01435au;if(!valid_ram(r[2]+14u,1u))return -3;wr(m,r[2]+0xeu,r[0],1);goto L8c01435c; /* mov.b r0,@(0xe,r2) */
L8c01435c: *stop=0x8c01435cu;if(!valid_ram(r[3]+15u,1u))return -3;r[0]=sx(m,r[3]+0xfu,1);goto L8c01435e; /* mov.b @(0xf,r3),r0 */
L8c01435e: *stop=0x8c01435eu;if(!valid_ram(r[2]+15u,1u))return -3;wr(m,r[2]+0xfu,r[0],1);goto L8c014360; /* mov.b r0,@(0xf,r2) */
L8c014360: *stop=0x8c014360u;r[2]=9u;goto L8c014362; /* mov #0x9,r2 */
L8c014362: *stop=0x8c014362u;*sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[2]);goto L8c014364; /* cmp/gt r2,r1 */
L8c014364: take=(*sr&1u);*stop=0x8c014366u;r[6]+=3u;if(take)goto L8c0143c0;goto L8c014368; /* bt/s 0x8c0143c0 */
L8c014368: *stop=0x8c01436au;r[4]+=48u;goto L8c0143c2; /* bra 0x8c0143c2 */
L8c0143c0: *stop=0x8c0143c0u;r[4]+=55u;goto L8c0143c2; /* add #0x37,r4 */
L8c0143c2: *stop=0x8c0143c2u;if(!valid_ram(r[6],1u))return -3;wr(m,r[6],r[4],1);goto L8c0143c4; /* mov.b r4,@r6 */
L8c0143c4: *stop=0x8c0143c4u;r[4]=r[14];goto L8c0143c6; /* mov r14,r4 */
L8c0143c6: *stop=0x8c0143c6u;r[4]+=3u;goto L8c0143c8; /* add #0x3,r4 */
L8c0143c8: *stop=0x8c0143c8u;if(!valid_ram(r[4],1u))return -3;r[3]=sx(m,r[4],1);goto L8c0143ca; /* mov.b @r4,r3 */
L8c0143ca: *stop=0x8c0143cau;r[2]=90u;goto L8c0143cc; /* mov #0x5a,r2 */
L8c0143cc: *stop=0x8c0143ccu;*sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[2]);goto L8c0143ce; /* cmp/gt r2,r3 */
L8c0143ce: if(!(*sr&1u))goto L8c0143da;goto L8c0143d0; /* bf 0x8c0143da */
L8c0143d0: *stop=0x8c0143d0u;if(!valid_ram(r[4],1u))return -3;r[1]=sx(m,r[4],1);goto L8c0143d2; /* mov.b @r4,r1 */
L8c0143d2: *stop=0x8c0143d2u;r[0]=49u;goto L8c0143d4; /* mov #0x31,r0 */
L8c0143d4: *stop=0x8c0143d4u;r[1]+=4294967270u;goto L8c0143d6; /* add #-0x1a,r1 */
L8c0143d6: *stop=0x8c0143d6u;if(!valid_ram(r[4],1u))return -3;wr(m,r[4],r[1],1);goto L8c0143d8; /* mov.b r1,@r4 */
L8c0143d8: *stop=0x8c0143d8u;if(!valid_ram(r[14]+2u,1u))return -3;wr(m,r[14]+0x2u,r[0],1);goto L8c0143da; /* mov.b r0,@(0x2,r14) */
L8c0143da: *stop=0x8c0143dau;r[0]=66u;goto L8c0143dc; /* mov #0x42,r0 */
L8c0143dc: *stop=0x8c0143dcu;r[3]=r[14];goto L8c0143de; /* mov r14,r3 */
L8c0143de: *stop=0x8c0143deu;r[3]+=4u;goto L8c0143e0; /* add #0x4,r3 */
L8c0143e0: *stop=0x8c0143e0u;r[5]+=48u;goto L8c0143e2; /* add #0x30,r5 */
L8c0143e2: *stop=0x8c0143e2u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto L8c0143e4; /* mov.l r3,@r15 */
L8c0143e4: *stop=0x8c0143e4u;if(!valid_ram(r[3],1u))return -3;wr(m,r[3],r[5],1);goto L8c0143e6; /* mov.b r5,@r3 */
L8c0143e6: *stop=0x8c0143e6u;r[5]=r[13];goto L8c0143e8; /* mov r13,r5 */
L8c0143e8: *stop=0x8c0143e8u;if(!valid_ram(r[14]+5u,1u))return -3;wr(m,r[14]+0x5u,r[0],1);goto L8c0143ea; /* mov.b r0,@(0x5,r14) */
L8c0143ea: *stop=0x8c0143eau;r[0]=46u;goto L8c0143ec; /* mov #0x2e,r0 */
L8c0143ec: *stop=0x8c0143ecu;if(!valid_ram(r[14]+6u,1u))return -3;wr(m,r[14]+0x6u,r[0],1);goto L8c0143ee; /* mov.b r0,@(0x6,r14) */
L8c0143ee: *stop=0x8c0143eeu;r[0]=98u;goto L8c0143f0; /* mov #0x62,r0 */
L8c0143f0: *stop=0x8c0143f0u;if(!valid_ram(r[14]+7u,1u))return -3;wr(m,r[14]+0x7u,r[0],1);goto L8c0143f2; /* mov.b r0,@(0x7,r14) */
L8c0143f2: *stop=0x8c0143f2u;r[0]=105u;goto L8c0143f4; /* mov #0x69,r0 */
L8c0143f4: *stop=0x8c0143f4u;if(!valid_ram(r[14]+8u,1u))return -3;wr(m,r[14]+0x8u,r[0],1);goto L8c0143f6; /* mov.b r0,@(0x8,r14) */
L8c0143f6: *stop=0x8c0143f6u;r[0]=110u;goto L8c0143f8; /* mov #0x6e,r0 */
L8c0143f8: *stop=0x8c0143f8u;if(!valid_ram(r[14]+9u,1u))return -3;wr(m,r[14]+0x9u,r[0],1);goto L8c0143fa; /* mov.b r0,@(0x9,r14) */
L8c0143fa: *stop=0x8c0143fau;r[0]=0u;goto L8c0143fc; /* mov #0x0,r0 */
L8c0143fc: *stop=0x8c0143fcu;if(!valid_ram(r[14]+10u,1u))return -3;wr(m,r[14]+0xau,r[0],1);goto L8c0143fe; /* mov.b r0,@(0xa,r14) */
L8c0143fe: target=0x8c0141e0u;*pr=0x8c014402u;*stop=0x8c014400u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c014402; /* bsr 0x8c0141e0 */
L8c014402: *stop=0x8c014402u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c014404; /* cmp/eq #0x1,r0 */
L8c014404: take=(*sr&1u);*stop=0x8c014406u;r[4]=r[0];if(take)goto L8c014416;goto L8c014408; /* bt/s 0x8c014416 */
L8c014408: *stop=0x8c014408u;if(!valid_ram(r[15],4u))return -3;r[3]=rd(m,r[15],4);goto L8c01440a; /* mov.l @r15,r3 */
L8c01440a: *stop=0x8c01440au;r[2]=48u;goto L8c01440c; /* mov #0x30,r2 */
L8c01440c: *stop=0x8c01440cu;r[5]=r[13];goto L8c01440e; /* mov r13,r5 */
L8c01440e: *stop=0x8c01440eu;if(!valid_ram(r[3],1u))return -3;wr(m,r[3],r[2],1);goto L8c014410; /* mov.b r2,@r3 */
L8c014410: target=0x8c0141e0u;*pr=0x8c014414u;*stop=0x8c014412u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c014414; /* bsr 0x8c0141e0 */
L8c014414: *stop=0x8c014414u;r[4]=r[0];goto L8c014416; /* mov r0,r4 */
L8c014416: *stop=0x8c014416u;r[0]=r[4];goto L8c014418; /* mov r4,r0 */
L8c014418: *stop=0x8c014418u;r[15]+=20u;goto L8c01441a; /* add #0x14,r15 */
L8c01441a: *stop=0x8c01441au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c01441c; /* lds.l @r15+,PR */
L8c01441c: *stop=0x8c01441cu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c01441e; /* mov.l @r15+,r13 */
L8c01441e: *stop=0x8c01441eu;if(*pr!=entry_pr)return -3;*stop=0x8c014420u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_scene_8c012014(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c012014;
L8c012014: *stop=0x8c012014u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c012016; /* mov.l r14,@-r15 */
L8c012016: *stop=0x8c012016u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c012018; /* mov.l r13,@-r15 */
L8c012018: *stop=0x8c012018u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c01201a; /* mov.l r12,@-r15 */
L8c01201a: *stop=0x8c01201au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c01201c; /* mov.l r11,@-r15 */
L8c01201c: *stop=0x8c01201cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c01201e; /* sts.l PR,@-r15 */
L8c01201e: *stop=0x8c01201eu;r[15]+=4294967176u;goto L8c012020; /* add #-0x78,r15 */
L8c012020: *stop=0x8c012020u;r[3]=rd(m,0x8c01210cu,4);goto L8c012022; /* mov.l 0x8c01210c,r3 */
L8c012022: *stop=0x8c012022u;r[2]=r[15];goto L8c012024; /* mov r15,r2 */
L8c012024: *stop=0x8c012024u;r[2]+=96u;goto L8c012026; /* add #0x60,r2 */
L8c012026: *stop=0x8c012026u;r[12]=r[15];goto L8c012028; /* mov r15,r12 */
L8c012028: *stop=0x8c012028u;if(!valid_ram(r[3],1u))return -3;r[0]=sx(m,r[3],1);goto L8c01202a; /* mov.b @r3,r0 */
L8c01202a: *stop=0x8c01202au;r[12]+=96u;goto L8c01202c; /* add #0x60,r12 */
L8c01202c: *stop=0x8c01202cu;if(!valid_ram(r[2],1u))return -3;wr(m,r[2],r[0],1);goto L8c01202e; /* mov.b r0,@r2 */
L8c01202e: *stop=0x8c01202eu;if(!valid_ram(r[3]+1u,1u))return -3;r[0]=sx(m,r[3]+0x1u,1);goto L8c012030; /* mov.b @(0x1,r3),r0 */
L8c012030: *stop=0x8c012030u;if(!valid_ram(r[2]+1u,1u))return -3;wr(m,r[2]+0x1u,r[0],1);goto L8c012032; /* mov.b r0,@(0x1,r2) */
L8c012032: *stop=0x8c012032u;if(!valid_ram(r[3]+2u,1u))return -3;r[0]=sx(m,r[3]+0x2u,1);goto L8c012034; /* mov.b @(0x2,r3),r0 */
L8c012034: *stop=0x8c012034u;if(!valid_ram(r[2]+2u,1u))return -3;wr(m,r[2]+0x2u,r[0],1);goto L8c012036; /* mov.b r0,@(0x2,r2) */
L8c012036: *stop=0x8c012036u;if(!valid_ram(r[3]+3u,1u))return -3;r[0]=sx(m,r[3]+0x3u,1);goto L8c012038; /* mov.b @(0x3,r3),r0 */
L8c012038: *stop=0x8c012038u;if(!valid_ram(r[2]+3u,1u))return -3;wr(m,r[2]+0x3u,r[0],1);goto L8c01203a; /* mov.b r0,@(0x3,r2) */
L8c01203a: *stop=0x8c01203au;if(!valid_ram(r[3]+4u,1u))return -3;r[0]=sx(m,r[3]+0x4u,1);goto L8c01203c; /* mov.b @(0x4,r3),r0 */
L8c01203c: *stop=0x8c01203cu;if(!valid_ram(r[2]+4u,1u))return -3;wr(m,r[2]+0x4u,r[0],1);goto L8c01203e; /* mov.b r0,@(0x4,r2) */
L8c01203e: *stop=0x8c01203eu;if(!valid_ram(r[3]+5u,1u))return -3;r[0]=sx(m,r[3]+0x5u,1);goto L8c012040; /* mov.b @(0x5,r3),r0 */
L8c012040: *stop=0x8c012040u;if(!valid_ram(r[2]+5u,1u))return -3;wr(m,r[2]+0x5u,r[0],1);goto L8c012042; /* mov.b r0,@(0x5,r2) */
L8c012042: *stop=0x8c012042u;if(!valid_ram(r[3]+6u,1u))return -3;r[0]=sx(m,r[3]+0x6u,1);goto L8c012044; /* mov.b @(0x6,r3),r0 */
L8c012044: *stop=0x8c012044u;if(!valid_ram(r[2]+6u,1u))return -3;wr(m,r[2]+0x6u,r[0],1);goto L8c012046; /* mov.b r0,@(0x6,r2) */
L8c012046: *stop=0x8c012046u;if(!valid_ram(r[3]+7u,1u))return -3;r[0]=sx(m,r[3]+0x7u,1);goto L8c012048; /* mov.b @(0x7,r3),r0 */
L8c012048: *stop=0x8c012048u;if(!valid_ram(r[2]+7u,1u))return -3;wr(m,r[2]+0x7u,r[0],1);goto L8c01204a; /* mov.b r0,@(0x7,r2) */
L8c01204a: *stop=0x8c01204au;if(!valid_ram(r[3]+8u,1u))return -3;r[0]=sx(m,r[3]+0x8u,1);goto L8c01204c; /* mov.b @(0x8,r3),r0 */
L8c01204c: *stop=0x8c01204cu;if(!valid_ram(r[2]+8u,1u))return -3;wr(m,r[2]+0x8u,r[0],1);goto L8c01204e; /* mov.b r0,@(0x8,r2) */
L8c01204e: *stop=0x8c01204eu;if(!valid_ram(r[3]+9u,1u))return -3;r[0]=sx(m,r[3]+0x9u,1);goto L8c012050; /* mov.b @(0x9,r3),r0 */
L8c012050: *stop=0x8c012050u;if(!valid_ram(r[2]+9u,1u))return -3;wr(m,r[2]+0x9u,r[0],1);goto L8c012052; /* mov.b r0,@(0x9,r2) */
L8c012052: *stop=0x8c012052u;if(!valid_ram(r[3]+10u,1u))return -3;r[0]=sx(m,r[3]+0xau,1);goto L8c012054; /* mov.b @(0xa,r3),r0 */
L8c012054: *stop=0x8c012054u;if(!valid_ram(r[2]+10u,1u))return -3;wr(m,r[2]+0xau,r[0],1);goto L8c012056; /* mov.b r0,@(0xa,r2) */
L8c012056: *stop=0x8c012056u;if(!valid_ram(r[3]+11u,1u))return -3;r[0]=sx(m,r[3]+0xbu,1);goto L8c012058; /* mov.b @(0xb,r3),r0 */
L8c012058: *stop=0x8c012058u;if(!valid_ram(r[2]+11u,1u))return -3;wr(m,r[2]+0xbu,r[0],1);goto L8c01205a; /* mov.b r0,@(0xb,r2) */
L8c01205a: *stop=0x8c01205au;if(!valid_ram(r[3]+12u,1u))return -3;r[0]=sx(m,r[3]+0xcu,1);goto L8c01205c; /* mov.b @(0xc,r3),r0 */
L8c01205c: *stop=0x8c01205cu;if(!valid_ram(r[2]+12u,1u))return -3;wr(m,r[2]+0xcu,r[0],1);goto L8c01205e; /* mov.b r0,@(0xc,r2) */
L8c01205e: *stop=0x8c01205eu;if(!valid_ram(r[3]+13u,1u))return -3;r[0]=sx(m,r[3]+0xdu,1);goto L8c012060; /* mov.b @(0xd,r3),r0 */
L8c012060: *stop=0x8c012060u;if(!valid_ram(r[2]+13u,1u))return -3;wr(m,r[2]+0xdu,r[0],1);goto L8c012062; /* mov.b r0,@(0xd,r2) */
L8c012062: *stop=0x8c012062u;r[6]=r[4]&65535u;goto L8c012064; /* extu.w r4,r6 */
L8c012064: *stop=0x8c012064u;if(!valid_ram(r[3]+14u,1u))return -3;r[0]=sx(m,r[3]+0xeu,1);goto L8c012066; /* mov.b @(0xe,r3),r0 */
L8c012066: *stop=0x8c012066u;r[6]<<=2;goto L8c012068; /* shll2 r6 */
L8c012068: *stop=0x8c012068u;r[5]=r[5]&65535u;goto L8c01206a; /* extu.w r5,r5 */
L8c01206a: *stop=0x8c01206au;r[14]=rd(m,0x8c012110u,4);goto L8c01206c; /* mov.l 0x8c012110,r14 */
L8c01206c: *stop=0x8c01206cu;if(!valid_ram(r[2]+14u,1u))return -3;wr(m,r[2]+0xeu,r[0],1);goto L8c01206e; /* mov.b r0,@(0xe,r2) */
L8c01206e: *stop=0x8c01206eu;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c012070; /* shll r6 */
L8c012070: *stop=0x8c012070u;if(!valid_ram(r[3]+15u,1u))return -3;r[0]=sx(m,r[3]+0xfu,1);goto L8c012072; /* mov.b @(0xf,r3),r0 */
L8c012072: *stop=0x8c012072u;r[6]|=r[5];goto L8c012074; /* or r5,r6 */
L8c012074: *stop=0x8c012074u;r[4]=r[6];goto L8c012076; /* mov r6,r4 */
L8c012076: *stop=0x8c012076u;r[7]=31u;goto L8c012078; /* mov #0x1f,r7 */
L8c012078: *stop=0x8c012078u;if(!valid_ram(r[2]+15u,1u))return -3;wr(m,r[2]+0xfu,r[0],1);goto L8c01207a; /* mov.b r0,@(0xf,r2) */
L8c01207a: *stop=0x8c01207au;r[2]=4294967291u;goto L8c01207c; /* mov #-0x5,r2 */
L8c01207c: *stop=0x8c01207cu;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[4]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[4]=n?(uint32_t)((int32_t)r[4]>>n):((r[4]&0x80000000u)?0xffffffffu:0u);}}goto L8c01207e; /* shad r2,r4 */
L8c01207e: *stop=0x8c01207eu;r[13]=rd(m,0x8c012114u,4);goto L8c012080; /* mov.l 0x8c012114,r13 */
L8c012080: *stop=0x8c012080u;r[4]&=r[7];goto L8c012082; /* and r7,r4 */
L8c012082: *stop=0x8c012082u;r[5]=r[12];goto L8c012084; /* mov r12,r5 */
L8c012084: *stop=0x8c012084u;r[1]=9u;goto L8c012086; /* mov #0x9,r1 */
L8c012086: *stop=0x8c012086u;*sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[1]);goto L8c012088; /* cmp/gt r1,r4 */
L8c012088: take=(*sr&1u);*stop=0x8c01208au;r[5]+=10u;if(take)goto L8c012090;goto L8c01208c; /* bt/s 0x8c012090 */
L8c01208c: *stop=0x8c01208eu;r[4]+=48u;goto L8c012092; /* bra 0x8c012092 */
L8c012090: *stop=0x8c012090u;r[4]+=55u;goto L8c012092; /* add #0x37,r4 */
L8c012092: *stop=0x8c012092u;if(!valid_ram(r[5],1u))return -3;wr(m,r[5],r[4],1);goto L8c012094; /* mov.b r4,@r5 */
L8c012094: *stop=0x8c012094u;r[4]=r[6];goto L8c012096; /* mov r6,r4 */
L8c012096: *stop=0x8c012096u;r[3]=9u;goto L8c012098; /* mov #0x9,r3 */
L8c012098: *stop=0x8c012098u;r[5]=r[12];goto L8c01209a; /* mov r12,r5 */
L8c01209a: *stop=0x8c01209au;r[4]&=r[7];goto L8c01209c; /* and r7,r4 */
L8c01209c: *stop=0x8c01209cu;*sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[3]);goto L8c01209e; /* cmp/gt r3,r4 */
L8c01209e: take=(*sr&1u);*stop=0x8c0120a0u;r[5]+=11u;if(take)goto L8c0120a6;goto L8c0120a2; /* bt/s 0x8c0120a6 */
L8c0120a2: *stop=0x8c0120a4u;r[4]+=48u;goto L8c0120a8; /* bra 0x8c0120a8 */
L8c0120a6: *stop=0x8c0120a6u;r[4]+=55u;goto L8c0120a8; /* add #0x37,r4 */
L8c0120a8: *stop=0x8c0120a8u;r[3]=rd(m,0x8c012118u,4);goto L8c0120aa; /* mov.l 0x8c012118,r3 */
L8c0120aa: *stop=0x8c0120aau;r[11]=rd(m,0x8c0120fcu,4);goto L8c0120ac; /* mov.l 0x8c0120fc,r11 */
L8c0120ac: *stop=0x8c0120acu;if(!valid_ram(r[5],1u))return -3;wr(m,r[5],r[4],1);goto L8c0120ae; /* mov.b r4,@r5 */
L8c0120ae: *stop=0x8c0120aeu;r[5]=r[12];goto L8c0120b0; /* mov r12,r5 */
L8c0120b0: target=r[3];*pr=0x8c0120b4u;*stop=0x8c0120b2u;if(!valid_ram(r[11],4u))return -3;r[4]=rd(m,r[11],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0120b4; /* jsr @r3 */
L8c0120b4: *stop=0x8c0120b4u;r[4]=r[0];goto L8c0120b6; /* mov r0,r4 */
L8c0120b6: *stop=0x8c0120b6u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c0120b8; /* tst r4,r4 */
L8c0120b8: if((*sr&1u))goto L8c012120;goto L8c0120ba; /* bt 0x8c012120 */
L8c0120ba: *stop=0x8c0120bau;r[3]=rd(m,0x8c012118u,4);goto L8c0120bc; /* mov.l 0x8c012118,r3 */
L8c0120bc: *stop=0x8c0120bcu;r[0]=48u;goto L8c0120be; /* mov #0x30,r0 */
L8c0120be: *stop=0x8c0120beu;r[5]=r[12];goto L8c0120c0; /* mov r12,r5 */
L8c0120c0: *stop=0x8c0120c0u;if(!valid_ram(r[12]+4u,1u))return -3;wr(m,r[12]+0x4u,r[0],1);goto L8c0120c2; /* mov.b r0,@(0x4,r12) */
L8c0120c2: target=r[3];*pr=0x8c0120c6u;*stop=0x8c0120c4u;if(!valid_ram(r[11],4u))return -3;r[4]=rd(m,r[11],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0120c6; /* jsr @r3 */
L8c0120c6: *stop=0x8c0120c6u;r[4]=r[0];goto L8c0120c8; /* mov r0,r4 */
L8c0120c8: *stop=0x8c0120c8u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c0120ca; /* tst r4,r4 */
L8c0120ca: if((*sr&1u))goto L8c012120;goto L8c0120cc; /* bt 0x8c012120 */
L8c0120cc: *stop=0x8c0120ceu;r[0]=4294967295u;goto L8c01215c; /* bra 0x8c01215c */
L8c012120: *stop=0x8c012120u;r[3]=rd(m,0x8c012220u,4);goto L8c012122; /* mov.l 0x8c012220,r3 */
L8c012122: *stop=0x8c012122u;r[5]=r[12];goto L8c012124; /* mov r12,r5 */
L8c012124: *stop=0x8c012124u;r[7]=5u;goto L8c012126; /* mov #0x5,r7 */
L8c012126: *stop=0x8c012126u;r[6]=r[14];goto L8c012128; /* mov r14,r6 */
L8c012128: target=r[3];*pr=0x8c01212cu;*stop=0x8c01212au;if(!valid_ram(r[11],4u))return -3;r[4]=rd(m,r[11],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c01212c; /* jsr @r3 */
L8c01212c: *stop=0x8c01212cu;r[12]=rd(m,0x8c012224u,4);goto L8c01212e; /* mov.l 0x8c012224,r12 */
L8c01212e: target=r[12];*pr=0x8c012132u;*stop=0x8c012130u;if(!valid_ram(r[11],4u))return -3;r[4]=rd(m,r[11],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c012132; /* jsr @r12 */
L8c012132: *stop=0x8c012132u;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c012134; /* cmp/eq #-0x1,r0 */
L8c012134: if((*sr&1u))goto L8c01212e;goto L8c012136; /* bt 0x8c01212e */
L8c012136: *stop=0x8c012136u;r[2]=3u;goto L8c012138; /* mov #0x3,r2 */
L8c012138: *stop=0x8c012138u;r[2]<<=8;goto L8c01213a; /* shll8 r2 */
L8c01213a: *stop=0x8c01213au;r[4]=6u;goto L8c01213c; /* mov #0x6,r4 */
L8c01213c: *stop=0x8c01213cu;r[2]+=4294967168u;goto L8c01213e; /* add #-0x80,r2 */
L8c01213e: *stop=0x8c01213eu;r[4]<<=8;goto L8c012140; /* shll8 r4 */
L8c012140: *stop=0x8c012140u;r[14]+=r[2];goto L8c012142; /* add r2,r14 */
L8c012142: *stop=0x8c012142u;if(!valid_ram(r[14],1u))return -3;r[3]=sx(m,r[14],1);r[14]+=1;goto L8c012144; /* mov.b @r14+,r3 */
L8c012144: *stop=0x8c012144u;r[4]+=4294967293u;goto L8c012146; /* add #-0x3,r4 */
L8c012146: *stop=0x8c012146u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c012148; /* tst r4,r4 */
L8c012148: *stop=0x8c012148u;if(!valid_ram(r[13],1u))return -3;wr(m,r[13],r[3],1);goto L8c01214a; /* mov.b r3,@r13 */
L8c01214a: *stop=0x8c01214au;r[13]+=1u;goto L8c01214c; /* add #0x1,r13 */
L8c01214c: *stop=0x8c01214cu;if(!valid_ram(r[14],1u))return -3;r[2]=sx(m,r[14],1);r[14]+=1;goto L8c01214e; /* mov.b @r14+,r2 */
L8c01214e: *stop=0x8c01214eu;if(!valid_ram(r[13],1u))return -3;wr(m,r[13],r[2],1);goto L8c012150; /* mov.b r2,@r13 */
L8c012150: *stop=0x8c012150u;r[13]+=1u;goto L8c012152; /* add #0x1,r13 */
L8c012152: *stop=0x8c012152u;if(!valid_ram(r[14],1u))return -3;r[3]=sx(m,r[14],1);r[14]+=1;goto L8c012154; /* mov.b @r14+,r3 */
L8c012154: *stop=0x8c012154u;if(!valid_ram(r[13],1u))return -3;wr(m,r[13],r[3],1);goto L8c012156; /* mov.b r3,@r13 */
L8c012156: take=!(*sr&1u);*stop=0x8c012158u;r[13]+=1u;if(take)goto L8c012142;goto L8c01215a; /* bf/s 0x8c012142 */
L8c01215a: *stop=0x8c01215au;r[0]=0u;goto L8c01215c; /* mov #0x0,r0 */
L8c01215c: *stop=0x8c01215cu;r[15]+=120u;goto L8c01215e; /* add #0x78,r15 */
L8c01215e: *stop=0x8c01215eu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c012160; /* lds.l @r15+,PR */
L8c012160: *stop=0x8c012160u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c012162; /* mov.l @r15+,r11 */
L8c012162: *stop=0x8c012162u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c012164; /* mov.l @r15+,r12 */
L8c012164: *stop=0x8c012164u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c012166; /* mov.l @r15+,r13 */
L8c012166: *stop=0x8c012166u;if(*pr!=entry_pr)return -3;*stop=0x8c012168u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_camera_scene_8c011d46(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c011d46;
L8c011d46: *stop=0x8c011d46u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c011d48; /* mov.l r14,@-r15 */
L8c011d48: *stop=0x8c011d48u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c011d4a; /* mov.l r13,@-r15 */
L8c011d4a: *stop=0x8c011d4au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c011d4c; /* mov.l r12,@-r15 */
L8c011d4c: *stop=0x8c011d4cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c011d4e; /* mov.l r11,@-r15 */
L8c011d4e: *stop=0x8c011d4eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c011d50; /* sts.l PR,@-r15 */
L8c011d50: *stop=0x8c011d50u;r[15]+=4294967176u;goto L8c011d52; /* add #-0x78,r15 */
L8c011d52: *stop=0x8c011d52u;r[3]=rd(m,0x8c011e18u,4);goto L8c011d54; /* mov.l 0x8c011e18,r3 */
L8c011d54: *stop=0x8c011d54u;r[2]=r[15];goto L8c011d56; /* mov r15,r2 */
L8c011d56: *stop=0x8c011d56u;r[2]+=96u;goto L8c011d58; /* add #0x60,r2 */
L8c011d58: *stop=0x8c011d58u;r[12]=r[15];goto L8c011d5a; /* mov r15,r12 */
L8c011d5a: *stop=0x8c011d5au;if(!valid_ram(r[3],1u))return -3;r[0]=sx(m,r[3],1);goto L8c011d5c; /* mov.b @r3,r0 */
L8c011d5c: *stop=0x8c011d5cu;r[12]+=96u;goto L8c011d5e; /* add #0x60,r12 */
L8c011d5e: *stop=0x8c011d5eu;if(!valid_ram(r[2],1u))return -3;wr(m,r[2],r[0],1);goto L8c011d60; /* mov.b r0,@r2 */
L8c011d60: *stop=0x8c011d60u;if(!valid_ram(r[3]+1u,1u))return -3;r[0]=sx(m,r[3]+0x1u,1);goto L8c011d62; /* mov.b @(0x1,r3),r0 */
L8c011d62: *stop=0x8c011d62u;if(!valid_ram(r[2]+1u,1u))return -3;wr(m,r[2]+0x1u,r[0],1);goto L8c011d64; /* mov.b r0,@(0x1,r2) */
L8c011d64: *stop=0x8c011d64u;if(!valid_ram(r[3]+2u,1u))return -3;r[0]=sx(m,r[3]+0x2u,1);goto L8c011d66; /* mov.b @(0x2,r3),r0 */
L8c011d66: *stop=0x8c011d66u;if(!valid_ram(r[2]+2u,1u))return -3;wr(m,r[2]+0x2u,r[0],1);goto L8c011d68; /* mov.b r0,@(0x2,r2) */
L8c011d68: *stop=0x8c011d68u;if(!valid_ram(r[3]+3u,1u))return -3;r[0]=sx(m,r[3]+0x3u,1);goto L8c011d6a; /* mov.b @(0x3,r3),r0 */
L8c011d6a: *stop=0x8c011d6au;if(!valid_ram(r[2]+3u,1u))return -3;wr(m,r[2]+0x3u,r[0],1);goto L8c011d6c; /* mov.b r0,@(0x3,r2) */
L8c011d6c: *stop=0x8c011d6cu;if(!valid_ram(r[3]+4u,1u))return -3;r[0]=sx(m,r[3]+0x4u,1);goto L8c011d6e; /* mov.b @(0x4,r3),r0 */
L8c011d6e: *stop=0x8c011d6eu;if(!valid_ram(r[2]+4u,1u))return -3;wr(m,r[2]+0x4u,r[0],1);goto L8c011d70; /* mov.b r0,@(0x4,r2) */
L8c011d70: *stop=0x8c011d70u;if(!valid_ram(r[3]+5u,1u))return -3;r[0]=sx(m,r[3]+0x5u,1);goto L8c011d72; /* mov.b @(0x5,r3),r0 */
L8c011d72: *stop=0x8c011d72u;if(!valid_ram(r[2]+5u,1u))return -3;wr(m,r[2]+0x5u,r[0],1);goto L8c011d74; /* mov.b r0,@(0x5,r2) */
L8c011d74: *stop=0x8c011d74u;if(!valid_ram(r[3]+6u,1u))return -3;r[0]=sx(m,r[3]+0x6u,1);goto L8c011d76; /* mov.b @(0x6,r3),r0 */
L8c011d76: *stop=0x8c011d76u;if(!valid_ram(r[2]+6u,1u))return -3;wr(m,r[2]+0x6u,r[0],1);goto L8c011d78; /* mov.b r0,@(0x6,r2) */
L8c011d78: *stop=0x8c011d78u;if(!valid_ram(r[3]+7u,1u))return -3;r[0]=sx(m,r[3]+0x7u,1);goto L8c011d7a; /* mov.b @(0x7,r3),r0 */
L8c011d7a: *stop=0x8c011d7au;if(!valid_ram(r[2]+7u,1u))return -3;wr(m,r[2]+0x7u,r[0],1);goto L8c011d7c; /* mov.b r0,@(0x7,r2) */
L8c011d7c: *stop=0x8c011d7cu;if(!valid_ram(r[3]+8u,1u))return -3;r[0]=sx(m,r[3]+0x8u,1);goto L8c011d7e; /* mov.b @(0x8,r3),r0 */
L8c011d7e: *stop=0x8c011d7eu;if(!valid_ram(r[2]+8u,1u))return -3;wr(m,r[2]+0x8u,r[0],1);goto L8c011d80; /* mov.b r0,@(0x8,r2) */
L8c011d80: *stop=0x8c011d80u;if(!valid_ram(r[3]+9u,1u))return -3;r[0]=sx(m,r[3]+0x9u,1);goto L8c011d82; /* mov.b @(0x9,r3),r0 */
L8c011d82: *stop=0x8c011d82u;if(!valid_ram(r[2]+9u,1u))return -3;wr(m,r[2]+0x9u,r[0],1);goto L8c011d84; /* mov.b r0,@(0x9,r2) */
L8c011d84: *stop=0x8c011d84u;if(!valid_ram(r[3]+10u,1u))return -3;r[0]=sx(m,r[3]+0xau,1);goto L8c011d86; /* mov.b @(0xa,r3),r0 */
L8c011d86: *stop=0x8c011d86u;if(!valid_ram(r[2]+10u,1u))return -3;wr(m,r[2]+0xau,r[0],1);goto L8c011d88; /* mov.b r0,@(0xa,r2) */
L8c011d88: *stop=0x8c011d88u;if(!valid_ram(r[3]+11u,1u))return -3;r[0]=sx(m,r[3]+0xbu,1);goto L8c011d8a; /* mov.b @(0xb,r3),r0 */
L8c011d8a: *stop=0x8c011d8au;if(!valid_ram(r[2]+11u,1u))return -3;wr(m,r[2]+0xbu,r[0],1);goto L8c011d8c; /* mov.b r0,@(0xb,r2) */
L8c011d8c: *stop=0x8c011d8cu;if(!valid_ram(r[3]+12u,1u))return -3;r[0]=sx(m,r[3]+0xcu,1);goto L8c011d8e; /* mov.b @(0xc,r3),r0 */
L8c011d8e: *stop=0x8c011d8eu;if(!valid_ram(r[2]+12u,1u))return -3;wr(m,r[2]+0xcu,r[0],1);goto L8c011d90; /* mov.b r0,@(0xc,r2) */
L8c011d90: *stop=0x8c011d90u;if(!valid_ram(r[3]+13u,1u))return -3;r[0]=sx(m,r[3]+0xdu,1);goto L8c011d92; /* mov.b @(0xd,r3),r0 */
L8c011d92: *stop=0x8c011d92u;if(!valid_ram(r[2]+13u,1u))return -3;wr(m,r[2]+0xdu,r[0],1);goto L8c011d94; /* mov.b r0,@(0xd,r2) */
L8c011d94: *stop=0x8c011d94u;r[6]=r[4]&65535u;goto L8c011d96; /* extu.w r4,r6 */
L8c011d96: *stop=0x8c011d96u;if(!valid_ram(r[3]+14u,1u))return -3;r[0]=sx(m,r[3]+0xeu,1);goto L8c011d98; /* mov.b @(0xe,r3),r0 */
L8c011d98: *stop=0x8c011d98u;r[6]<<=2;goto L8c011d9a; /* shll2 r6 */
L8c011d9a: *stop=0x8c011d9au;r[5]=r[5]&65535u;goto L8c011d9c; /* extu.w r5,r5 */
L8c011d9c: *stop=0x8c011d9cu;r[14]=rd(m,0x8c011de4u,4);goto L8c011d9e; /* mov.l 0x8c011de4,r14 */
L8c011d9e: *stop=0x8c011d9eu;if(!valid_ram(r[2]+14u,1u))return -3;wr(m,r[2]+0xeu,r[0],1);goto L8c011da0; /* mov.b r0,@(0xe,r2) */
L8c011da0: *stop=0x8c011da0u;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c011da2; /* shll r6 */
L8c011da2: *stop=0x8c011da2u;if(!valid_ram(r[3]+15u,1u))return -3;r[0]=sx(m,r[3]+0xfu,1);goto L8c011da4; /* mov.b @(0xf,r3),r0 */
L8c011da4: *stop=0x8c011da4u;r[6]|=r[5];goto L8c011da6; /* or r5,r6 */
L8c011da6: *stop=0x8c011da6u;r[4]=r[6];goto L8c011da8; /* mov r6,r4 */
L8c011da8: *stop=0x8c011da8u;r[7]=31u;goto L8c011daa; /* mov #0x1f,r7 */
L8c011daa: *stop=0x8c011daau;if(!valid_ram(r[2]+15u,1u))return -3;wr(m,r[2]+0xfu,r[0],1);goto L8c011dac; /* mov.b r0,@(0xf,r2) */
L8c011dac: *stop=0x8c011dacu;r[2]=4294967291u;goto L8c011dae; /* mov #-0x5,r2 */
L8c011dae: *stop=0x8c011daeu;{uint32_t shift=r[2];if(!(shift&0x80000000u))r[4]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[4]=n?(uint32_t)((int32_t)r[4]>>n):((r[4]&0x80000000u)?0xffffffffu:0u);}}goto L8c011db0; /* shad r2,r4 */
L8c011db0: *stop=0x8c011db0u;r[13]=rd(m,0x8c011de8u,4);goto L8c011db2; /* mov.l 0x8c011de8,r13 */
L8c011db2: *stop=0x8c011db2u;r[4]&=r[7];goto L8c011db4; /* and r7,r4 */
L8c011db4: *stop=0x8c011db4u;r[5]=r[12];goto L8c011db6; /* mov r12,r5 */
L8c011db6: *stop=0x8c011db6u;r[1]=9u;goto L8c011db8; /* mov #0x9,r1 */
L8c011db8: *stop=0x8c011db8u;*sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[1]);goto L8c011dba; /* cmp/gt r1,r4 */
L8c011dba: take=(*sr&1u);*stop=0x8c011dbcu;r[5]+=10u;if(take)goto L8c011e20;goto L8c011dbe; /* bt/s 0x8c011e20 */
L8c011dbe: *stop=0x8c011dc0u;r[4]+=48u;goto L8c011e22; /* bra 0x8c011e22 */
L8c011e20: *stop=0x8c011e20u;r[4]+=55u;goto L8c011e22; /* add #0x37,r4 */
L8c011e22: *stop=0x8c011e22u;if(!valid_ram(r[5],1u))return -3;wr(m,r[5],r[4],1);goto L8c011e24; /* mov.b r4,@r5 */
L8c011e24: *stop=0x8c011e24u;r[4]=r[6];goto L8c011e26; /* mov r6,r4 */
L8c011e26: *stop=0x8c011e26u;r[3]=9u;goto L8c011e28; /* mov #0x9,r3 */
L8c011e28: *stop=0x8c011e28u;r[5]=r[12];goto L8c011e2a; /* mov r12,r5 */
L8c011e2a: *stop=0x8c011e2au;r[4]&=r[7];goto L8c011e2c; /* and r7,r4 */
L8c011e2c: *stop=0x8c011e2cu;*sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[3]);goto L8c011e2e; /* cmp/gt r3,r4 */
L8c011e2e: take=(*sr&1u);*stop=0x8c011e30u;r[5]+=11u;if(take)goto L8c011e36;goto L8c011e32; /* bt/s 0x8c011e36 */
L8c011e32: *stop=0x8c011e34u;r[4]+=48u;goto L8c011e38; /* bra 0x8c011e38 */
L8c011e36: *stop=0x8c011e36u;r[4]+=55u;goto L8c011e38; /* add #0x37,r4 */
L8c011e38: *stop=0x8c011e38u;r[11]=rd(m,0x8c011f40u,4);goto L8c011e3a; /* mov.l 0x8c011f40,r11 */
L8c011e3a: *stop=0x8c011e3au;if(!valid_ram(r[5],1u))return -3;wr(m,r[5],r[4],1);goto L8c011e3c; /* mov.b r4,@r5 */
L8c011e3c: *stop=0x8c011e3cu;r[5]=r[12];goto L8c011e3e; /* mov r12,r5 */
L8c011e3e: target=0x8c010fccu;*pr=0x8c011e42u;*stop=0x8c011e40u;if(!valid_ram(r[11],4u))return -3;r[4]=rd(m,r[11],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c011e42; /* bsr 0x8c010fcc */
L8c011e42: *stop=0x8c011e42u;r[4]=r[0];goto L8c011e44; /* mov r0,r4 */
L8c011e44: *stop=0x8c011e44u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c011e46; /* tst r4,r4 */
L8c011e46: if((*sr&1u))goto L8c011e5c;goto L8c011e48; /* bt 0x8c011e5c */
L8c011e48: *stop=0x8c011e48u;r[0]=48u;goto L8c011e4a; /* mov #0x30,r0 */
L8c011e4a: *stop=0x8c011e4au;r[5]=r[12];goto L8c011e4c; /* mov r12,r5 */
L8c011e4c: *stop=0x8c011e4cu;if(!valid_ram(r[12]+4u,1u))return -3;wr(m,r[12]+0x4u,r[0],1);goto L8c011e4e; /* mov.b r0,@(0x4,r12) */
L8c011e4e: target=0x8c010fccu;*pr=0x8c011e52u;*stop=0x8c011e50u;if(!valid_ram(r[11],4u))return -3;r[4]=rd(m,r[11],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c011e52; /* bsr 0x8c010fcc */
L8c011e52: *stop=0x8c011e52u;r[4]=r[0];goto L8c011e54; /* mov r0,r4 */
L8c011e54: *stop=0x8c011e54u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c011e56; /* tst r4,r4 */
L8c011e56: if((*sr&1u))goto L8c011e5c;goto L8c011e58; /* bt 0x8c011e5c */
L8c011e58: *stop=0x8c011e5au;r[0]=4294967295u;goto L8c011e98; /* bra 0x8c011e98 */
L8c011e5c: *stop=0x8c011e5cu;r[3]=rd(m,0x8c011f44u,4);goto L8c011e5e; /* mov.l 0x8c011f44,r3 */
L8c011e5e: *stop=0x8c011e5eu;r[5]=r[12];goto L8c011e60; /* mov r12,r5 */
L8c011e60: *stop=0x8c011e60u;r[7]=18u;goto L8c011e62; /* mov #0x12,r7 */
L8c011e62: *stop=0x8c011e62u;r[6]=r[14];goto L8c011e64; /* mov r14,r6 */
L8c011e64: target=r[3];*pr=0x8c011e68u;*stop=0x8c011e66u;if(!valid_ram(r[11],4u))return -3;r[4]=rd(m,r[11],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c011e68; /* jsr @r3 */
L8c011e68: *stop=0x8c011e68u;r[12]=rd(m,0x8c011f48u,4);goto L8c011e6a; /* mov.l 0x8c011f48,r12 */
L8c011e6a: target=r[12];*pr=0x8c011e6eu;*stop=0x8c011e6cu;if(!valid_ram(r[11],4u))return -3;r[4]=rd(m,r[11],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c011e6e; /* jsr @r12 */
L8c011e6e: *stop=0x8c011e6eu;*sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c011e70; /* cmp/eq #-0x1,r0 */
L8c011e70: if((*sr&1u))goto L8c011e6a;goto L8c011e72; /* bt 0x8c011e6a */
L8c011e72: *stop=0x8c011e72u;r[2]=3u;goto L8c011e74; /* mov #0x3,r2 */
L8c011e74: *stop=0x8c011e74u;r[2]<<=8;goto L8c011e76; /* shll8 r2 */
L8c011e76: *stop=0x8c011e76u;r[4]=48u;goto L8c011e78; /* mov #0x30,r4 */
L8c011e78: *stop=0x8c011e78u;r[2]+=4294967168u;goto L8c011e7a; /* add #-0x80,r2 */
L8c011e7a: *stop=0x8c011e7au;r[4]<<=8;goto L8c011e7c; /* shll8 r4 */
L8c011e7c: *stop=0x8c011e7cu;r[14]+=r[2];goto L8c011e7e; /* add r2,r14 */
L8c011e7e: *stop=0x8c011e7eu;if(!valid_ram(r[14],1u))return -3;r[3]=sx(m,r[14],1);r[14]+=1;goto L8c011e80; /* mov.b @r14+,r3 */
L8c011e80: *stop=0x8c011e80u;r[4]+=4294967293u;goto L8c011e82; /* add #-0x3,r4 */
L8c011e82: *stop=0x8c011e82u;*sr=(*sr&~1u)|!(r[4]&r[4]);goto L8c011e84; /* tst r4,r4 */
L8c011e84: *stop=0x8c011e84u;if(!valid_ram(r[13],1u))return -3;wr(m,r[13],r[3],1);goto L8c011e86; /* mov.b r3,@r13 */
L8c011e86: *stop=0x8c011e86u;r[13]+=1u;goto L8c011e88; /* add #0x1,r13 */
L8c011e88: *stop=0x8c011e88u;if(!valid_ram(r[14],1u))return -3;r[2]=sx(m,r[14],1);r[14]+=1;goto L8c011e8a; /* mov.b @r14+,r2 */
L8c011e8a: *stop=0x8c011e8au;if(!valid_ram(r[13],1u))return -3;wr(m,r[13],r[2],1);goto L8c011e8c; /* mov.b r2,@r13 */
L8c011e8c: *stop=0x8c011e8cu;r[13]+=1u;goto L8c011e8e; /* add #0x1,r13 */
L8c011e8e: *stop=0x8c011e8eu;if(!valid_ram(r[14],1u))return -3;r[3]=sx(m,r[14],1);r[14]+=1;goto L8c011e90; /* mov.b @r14+,r3 */
L8c011e90: *stop=0x8c011e90u;if(!valid_ram(r[13],1u))return -3;wr(m,r[13],r[3],1);goto L8c011e92; /* mov.b r3,@r13 */
L8c011e92: take=!(*sr&1u);*stop=0x8c011e94u;r[13]+=1u;if(take)goto L8c011e7e;goto L8c011e96; /* bf/s 0x8c011e7e */
L8c011e96: *stop=0x8c011e96u;r[0]=0u;goto L8c011e98; /* mov #0x0,r0 */
L8c011e98: *stop=0x8c011e98u;r[15]+=120u;goto L8c011e9a; /* add #0x78,r15 */
L8c011e9a: *stop=0x8c011e9au;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c011e9c; /* lds.l @r15+,PR */
L8c011e9c: *stop=0x8c011e9cu;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c011e9e; /* mov.l @r15+,r11 */
L8c011e9e: *stop=0x8c011e9eu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c011ea0; /* mov.l @r15+,r12 */
L8c011ea0: *stop=0x8c011ea0u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c011ea2; /* mov.l @r15+,r13 */
L8c011ea2: *stop=0x8c011ea2u;if(*pr!=entry_pr)return -3;*stop=0x8c011ea4u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
