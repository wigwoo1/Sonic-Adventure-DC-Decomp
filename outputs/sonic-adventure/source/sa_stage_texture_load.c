/* Original USA world/land producer entries; names are source-informed.
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
SA_STAGE_EXPORT int sa_world_stage_textures_8c099220(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c099220;
L8c099220: *stop=0x8c099220u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c099222; /* mov.l r14,@-r15 */
L8c099222: *stop=0x8c099222u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c099224; /* mov.l r13,@-r15 */
L8c099224: *stop=0x8c099224u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c099226; /* mov.l r12,@-r15 */
L8c099226: *stop=0x8c099226u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c099228; /* mov.l r11,@-r15 */
L8c099228: *stop=0x8c099228u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c09922a; /* mov.l r10,@-r15 */
L8c09922a: *stop=0x8c09922au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c09922c; /* mov.l r9,@-r15 */
L8c09922c: *stop=0x8c09922cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c09922e; /* mov.l r8,@-r15 */
L8c09922e: *stop=0x8c09922eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c099230; /* sts.l PR,@-r15 */
L8c099230: *stop=0x8c099230u;r[15]+=4294967292u;goto L8c099232; /* add #-0x4,r15 */
L8c099232: *stop=0x8c099232u;r[3]=rd(m,0x8c0992ecu,4);goto L8c099234; /* mov.l 0x8c0992ec,r3 */
L8c099234: target=r[3];*pr=0x8c099238u;*stop=0x8c099236u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099238; /* jsr @r3 */
L8c099238: *stop=0x8c099238u;r[2]=rd(m,0x8c0992f0u,4);goto L8c09923a; /* mov.l 0x8c0992f0,r2 */
L8c09923a: *stop=0x8c09923au;r[0]=r[0]&65535u;goto L8c09923c; /* extu.w r0,r0 */
L8c09923c: *stop=0x8c09923cu;r[3]=4294967288u;goto L8c09923e; /* mov #-0x8,r3 */
L8c09923e: *stop=0x8c09923eu;r[0]&=r[2];goto L8c099240; /* and r2,r0 */
L8c099240: *stop=0x8c099240u;{uint32_t shift=r[3];if(!(shift&0x80000000u))r[0]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[0]=n?(uint32_t)((int32_t)r[0]>>n):((r[0]&0x80000000u)?0xffffffffu:0u);}}goto L8c099242; /* shad r3,r0 */
L8c099242: *stop=0x8c099242u;r[4]=r[0];goto L8c099244; /* mov r0,r4 */
L8c099244: *stop=0x8c099244u;r[0]=rd(m,0x8c0992f4u,4);goto L8c099246; /* mov.l 0x8c0992f4,r0 */
L8c099246: *stop=0x8c099246u;r[14]=r[4]&65535u;goto L8c099248; /* extu.w r4,r14 */
L8c099248: *stop=0x8c099248u;r[14]<<=2;goto L8c09924a; /* shll2 r14 */
L8c09924a: *stop=0x8c09924au;if(!valid_ram(r[0]+r[14],4u))return -3;r[14]=rd(m,r[14]+r[0],4);goto L8c09924c; /* mov.l @(r0,r14),r14 */
L8c09924c: *stop=0x8c09924cu;r[0]=r[4]&65535u;goto L8c09924e; /* extu.w r4,r0 */
L8c09924e: *stop=0x8c09924eu;*sr=(*sr&~1u)|(r[0]==0x0000001du);goto L8c099250; /* cmp/eq #0x1d,r0 */
L8c099250: if(!(*sr&1u))goto L8c09925e;goto L8c099252; /* bf 0x8c09925e */
L8c099252: *stop=0x8c099252u;r[1]=rd(m,0x8c0992f8u,4);goto L8c099254; /* mov.l 0x8c0992f8,r1 */
L8c099254: target=r[1];*pr=0x8c099258u;*stop=0x8c099256u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099258; /* jsr @r1 */
L8c099258: *stop=0x8c099258u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c09925a; /* tst r0,r0 */
L8c09925a: if((*sr&1u))goto L8c09925e;goto L8c09925c; /* bt 0x8c09925e */
L8c09925c: *stop=0x8c09925cu;r[14]=rd(m,0x8c0992fcu,4);goto L8c09925e; /* mov.l 0x8c0992fc,r14 */
L8c09925e: *stop=0x8c09925eu;r[6]=rd(m,0x8c0992e4u,4);goto L8c099260; /* mov.l 0x8c0992e4,r6 */
L8c099260: *stop=0x8c099260u;*sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c099262; /* tst r14,r14 */
L8c099262: *stop=0x8c099262u;r[9]=rd(m,0x8c099300u,4);goto L8c099264; /* mov.l 0x8c099300,r9 */
L8c099264: take=!(*sr&1u);*stop=0x8c099266u;r[10]=0u;if(take)goto L8c099284;goto L8c099268; /* bf/s 0x8c099284 */
L8c099268: *stop=0x8c099268u;r[5]=r[6];goto L8c09926a; /* mov r6,r5 */
L8c09926a: *stop=0x8c09926au;r[4]=r[6];goto L8c09926c; /* mov r6,r4 */
L8c09926c: *stop=0x8c09926cu;r[5]+=20u;goto L8c09926e; /* add #0x14,r5 */
L8c09926e: *stop=0x8c09926eu;r[4]+=4u;goto L8c099270; /* add #0x4,r4 */
L8c099270: *stop=0x8c099270u;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c099272; /* cmp/hs r5,r4 */
L8c099272: if((*sr&1u))goto L8c099280;goto L8c099274; /* bt 0x8c099280 */
L8c099274: *stop=0x8c099274u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[10],4);goto L8c099276; /* mov.l r10,@r4 */
L8c099276: *stop=0x8c099276u;r[4]+=4u;goto L8c099278; /* add #0x4,r4 */
L8c099278: *stop=0x8c099278u;if(!valid_ram(r[4],4u))return -3;wr(m,r[4],r[10],4);goto L8c09927a; /* mov.l r10,@r4 */
L8c09927a: *stop=0x8c09927au;r[4]+=4u;goto L8c09927c; /* add #0x4,r4 */
L8c09927c: *stop=0x8c09927cu;*sr=(*sr&~1u)|(r[4]>=r[5]);goto L8c09927e; /* cmp/hs r5,r4 */
L8c09927e: if(!(*sr&1u))goto L8c099274;goto L8c099280; /* bf 0x8c099274 */
L8c099280: *stop=0x8c099282u;if(!valid_ram(r[9],4u))return -3;wr(m,r[9],r[10],4);goto L8c0994ba; /* bra 0x8c0994ba */
L8c099284: *stop=0x8c099284u;r[13]=r[6];goto L8c099286; /* mov r6,r13 */
L8c099286: *stop=0x8c099286u;r[6]+=20u;goto L8c099288; /* add #0x14,r6 */
L8c099288: *stop=0x8c099288u;r[12]=rd(m,0x8c09930cu,4);goto L8c09928a; /* mov.l 0x8c09930c,r12 */
L8c09928a: *stop=0x8c09928au;r[3]=r[6];goto L8c09928c; /* mov r6,r3 */
L8c09928c: *stop=0x8c09928cu;r[11]=rd(m,0x8c099308u,4);goto L8c09928e; /* mov.l 0x8c099308,r11 */
L8c09928e: *stop=0x8c09928eu;r[13]+=4u;goto L8c099290; /* add #0x4,r13 */
L8c099290: *stop=0x8c099290u;r[8]=rd(m,0x8c099304u,4);goto L8c099292; /* mov.l 0x8c099304,r8 */
L8c099292: *stop=0x8c099292u;*sr=(*sr&~1u)|(r[13]>=r[3]);goto L8c099294; /* cmp/hs r3,r13 */
L8c099294: take=!(*sr&1u);*stop=0x8c099296u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[6],4);if(take)goto L8c09929c;goto L8c099298; /* bf/s 0x8c09929c */
L8c099298: *stop=0x8c09929au;goto L8c09943c; /* bra 0x8c09943c */
L8c09929c: *stop=0x8c09929cu;if(!valid_ram(r[14]+4u,4u))return -3;r[1]=rd(m,r[14]+0x4u,4);goto L8c09929e; /* mov.l @(0x4,r14),r1 */
L8c09929e: *stop=0x8c09929eu;if(!valid_ram(r[13],4u))return -3;wr(m,r[13],r[1],4);goto L8c0992a0; /* mov.l r1,@r13 */
L8c0992a0: *stop=0x8c0992a0u;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c0992a2; /* mov.l @(0x4,r14),r3 */
L8c0992a2: *stop=0x8c0992a2u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0992a4; /* tst r3,r3 */
L8c0992a4: if((*sr&1u))goto L8c099324;goto L8c0992a6; /* bt 0x8c099324 */
L8c0992a6: *stop=0x8c0992a6u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c0992a8; /* mov.l @r14,r3 */
L8c0992a8: *stop=0x8c0992a8u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0992aa; /* tst r3,r3 */
L8c0992aa: if((*sr&1u))goto L8c099320;goto L8c0992ac; /* bt 0x8c099320 */
L8c0992ac: *stop=0x8c0992acu;r[5]=rd(m,0x8c099310u,4);goto L8c0992ae; /* mov.l 0x8c099310,r5 */
L8c0992ae: target=r[11];*pr=0x8c0992b2u;*stop=0x8c0992b0u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0992b2; /* jsr @r11 */
L8c0992b2: *stop=0x8c0992b2u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0992b4; /* tst r0,r0 */
L8c0992b4: if(!(*sr&1u))goto L8c0992ce;goto L8c0992b6; /* bf 0x8c0992ce */
L8c0992b6: *stop=0x8c0992b6u;if(!valid_ram(r[12],4u))return -3;r[2]=rd(m,r[12],4);goto L8c0992b8; /* mov.l @r12,r2 */
L8c0992b8: *stop=0x8c0992b8u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c0992ba; /* tst r2,r2 */
L8c0992ba: if(!(*sr&1u))goto L8c0992c2;goto L8c0992bc; /* bf 0x8c0992c2 */
L8c0992bc: *stop=0x8c0992bcu;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c0992be; /* mov.l @(0x4,r14),r5 */
L8c0992be: *stop=0x8c0992c0u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);goto L8c0992c6; /* bra 0x8c0992c6 */
L8c0992c2: *stop=0x8c0992c2u;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c0992c4; /* mov.l @(0x4,r14),r5 */
L8c0992c4: *stop=0x8c0992c4u;r[4]=r[8];goto L8c0992c6; /* mov r8,r4 */
L8c0992c6: target=0x8c099750u;*pr=0x8c0992cau;*stop=0x8c0992c8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0992ca; /* bsr 0x8c099750 */
L8c0992ca: *stop=0x8c0992ccu;goto L8c099324; /* bra 0x8c099324 */
L8c0992ce: *stop=0x8c0992ceu;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c0992d0; /* mov.l @(0x4,r14),r5 */
L8c0992d0: target=0x8c099750u;*pr=0x8c0992d4u;*stop=0x8c0992d2u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0992d4; /* bsr 0x8c099750 */
L8c0992d4: *stop=0x8c0992d6u;goto L8c099324; /* bra 0x8c099324 */
L8c099320: target=0x8c098ee0u;*pr=0x8c099324u;*stop=0x8c099322u;if(!valid_ram(r[14]+4u,4u))return -3;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099324; /* bsr 0x8c098ee0 */
L8c099324: *stop=0x8c099324u;r[14]+=8u;goto L8c099326; /* add #0x8,r14 */
L8c099326: *stop=0x8c099326u;if(!valid_ram(r[14]+4u,4u))return -3;r[2]=rd(m,r[14]+0x4u,4);goto L8c099328; /* mov.l @(0x4,r14),r2 */
L8c099328: *stop=0x8c099328u;r[13]+=4u;goto L8c09932a; /* add #0x4,r13 */
L8c09932a: *stop=0x8c09932au;if(!valid_ram(r[13],4u))return -3;wr(m,r[13],r[2],4);goto L8c09932c; /* mov.l r2,@r13 */
L8c09932c: *stop=0x8c09932cu;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c09932e; /* mov.l @(0x4,r14),r3 */
L8c09932e: *stop=0x8c09932eu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c099330; /* tst r3,r3 */
L8c099330: if((*sr&1u))goto L8c099368;goto L8c099332; /* bt 0x8c099368 */
L8c099332: *stop=0x8c099332u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c099334; /* mov.l @r14,r3 */
L8c099334: *stop=0x8c099334u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c099336; /* tst r3,r3 */
L8c099336: if((*sr&1u))goto L8c099364;goto L8c099338; /* bt 0x8c099364 */
L8c099338: *stop=0x8c099338u;r[5]=rd(m,0x8c099400u,4);goto L8c09933a; /* mov.l 0x8c099400,r5 */
L8c09933a: target=r[11];*pr=0x8c09933eu;*stop=0x8c09933cu;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09933e; /* jsr @r11 */
L8c09933e: *stop=0x8c09933eu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c099340; /* tst r0,r0 */
L8c099340: if(!(*sr&1u))goto L8c09935a;goto L8c099342; /* bf 0x8c09935a */
L8c099342: *stop=0x8c099342u;if(!valid_ram(r[12],4u))return -3;r[2]=rd(m,r[12],4);goto L8c099344; /* mov.l @r12,r2 */
L8c099344: *stop=0x8c099344u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c099346; /* tst r2,r2 */
L8c099346: if(!(*sr&1u))goto L8c09934e;goto L8c099348; /* bf 0x8c09934e */
L8c099348: *stop=0x8c099348u;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c09934a; /* mov.l @(0x4,r14),r5 */
L8c09934a: *stop=0x8c09934cu;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);goto L8c099352; /* bra 0x8c099352 */
L8c09934e: *stop=0x8c09934eu;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c099350; /* mov.l @(0x4,r14),r5 */
L8c099350: *stop=0x8c099350u;r[4]=r[8];goto L8c099352; /* mov r8,r4 */
L8c099352: target=0x8c099750u;*pr=0x8c099356u;*stop=0x8c099354u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099356; /* bsr 0x8c099750 */
L8c099356: *stop=0x8c099358u;goto L8c099368; /* bra 0x8c099368 */
L8c09935a: *stop=0x8c09935au;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c09935c; /* mov.l @(0x4,r14),r5 */
L8c09935c: target=0x8c099750u;*pr=0x8c099360u;*stop=0x8c09935eu;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099360; /* bsr 0x8c099750 */
L8c099360: *stop=0x8c099362u;goto L8c099368; /* bra 0x8c099368 */
L8c099364: target=0x8c098ee0u;*pr=0x8c099368u;*stop=0x8c099366u;if(!valid_ram(r[14]+4u,4u))return -3;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099368; /* bsr 0x8c098ee0 */
L8c099368: *stop=0x8c099368u;r[14]+=8u;goto L8c09936a; /* add #0x8,r14 */
L8c09936a: *stop=0x8c09936au;if(!valid_ram(r[14]+4u,4u))return -3;r[2]=rd(m,r[14]+0x4u,4);goto L8c09936c; /* mov.l @(0x4,r14),r2 */
L8c09936c: *stop=0x8c09936cu;r[13]+=4u;goto L8c09936e; /* add #0x4,r13 */
L8c09936e: *stop=0x8c09936eu;if(!valid_ram(r[13],4u))return -3;wr(m,r[13],r[2],4);goto L8c099370; /* mov.l r2,@r13 */
L8c099370: *stop=0x8c099370u;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c099372; /* mov.l @(0x4,r14),r3 */
L8c099372: *stop=0x8c099372u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c099374; /* tst r3,r3 */
L8c099374: if((*sr&1u))goto L8c0993ac;goto L8c099376; /* bt 0x8c0993ac */
L8c099376: *stop=0x8c099376u;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c099378; /* mov.l @r14,r3 */
L8c099378: *stop=0x8c099378u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c09937a; /* tst r3,r3 */
L8c09937a: if((*sr&1u))goto L8c0993a8;goto L8c09937c; /* bt 0x8c0993a8 */
L8c09937c: *stop=0x8c09937cu;r[5]=rd(m,0x8c099400u,4);goto L8c09937e; /* mov.l 0x8c099400,r5 */
L8c09937e: target=r[11];*pr=0x8c099382u;*stop=0x8c099380u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099382; /* jsr @r11 */
L8c099382: *stop=0x8c099382u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c099384; /* tst r0,r0 */
L8c099384: if(!(*sr&1u))goto L8c09939e;goto L8c099386; /* bf 0x8c09939e */
L8c099386: *stop=0x8c099386u;if(!valid_ram(r[12],4u))return -3;r[2]=rd(m,r[12],4);goto L8c099388; /* mov.l @r12,r2 */
L8c099388: *stop=0x8c099388u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c09938a; /* tst r2,r2 */
L8c09938a: if(!(*sr&1u))goto L8c099392;goto L8c09938c; /* bf 0x8c099392 */
L8c09938c: *stop=0x8c09938cu;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c09938e; /* mov.l @(0x4,r14),r5 */
L8c09938e: *stop=0x8c099390u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);goto L8c099396; /* bra 0x8c099396 */
L8c099392: *stop=0x8c099392u;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c099394; /* mov.l @(0x4,r14),r5 */
L8c099394: *stop=0x8c099394u;r[4]=r[8];goto L8c099396; /* mov r8,r4 */
L8c099396: target=0x8c099750u;*pr=0x8c09939au;*stop=0x8c099398u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09939a; /* bsr 0x8c099750 */
L8c09939a: *stop=0x8c09939cu;goto L8c0993ac; /* bra 0x8c0993ac */
L8c09939e: *stop=0x8c09939eu;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c0993a0; /* mov.l @(0x4,r14),r5 */
L8c0993a0: target=0x8c099750u;*pr=0x8c0993a4u;*stop=0x8c0993a2u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0993a4; /* bsr 0x8c099750 */
L8c0993a4: *stop=0x8c0993a6u;goto L8c0993ac; /* bra 0x8c0993ac */
L8c0993a8: target=0x8c098ee0u;*pr=0x8c0993acu;*stop=0x8c0993aau;if(!valid_ram(r[14]+4u,4u))return -3;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0993ac; /* bsr 0x8c098ee0 */
L8c0993ac: *stop=0x8c0993acu;r[14]+=8u;goto L8c0993ae; /* add #0x8,r14 */
L8c0993ae: *stop=0x8c0993aeu;if(!valid_ram(r[14]+4u,4u))return -3;r[2]=rd(m,r[14]+0x4u,4);goto L8c0993b0; /* mov.l @(0x4,r14),r2 */
L8c0993b0: *stop=0x8c0993b0u;r[13]+=4u;goto L8c0993b2; /* add #0x4,r13 */
L8c0993b2: *stop=0x8c0993b2u;if(!valid_ram(r[13],4u))return -3;wr(m,r[13],r[2],4);goto L8c0993b4; /* mov.l r2,@r13 */
L8c0993b4: *stop=0x8c0993b4u;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c0993b6; /* mov.l @(0x4,r14),r3 */
L8c0993b6: *stop=0x8c0993b6u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0993b8; /* tst r3,r3 */
L8c0993b8: if((*sr&1u))goto L8c09942e;goto L8c0993ba; /* bt 0x8c09942e */
L8c0993ba: *stop=0x8c0993bau;if(!valid_ram(r[14],4u))return -3;r[3]=rd(m,r[14],4);goto L8c0993bc; /* mov.l @r14,r3 */
L8c0993bc: *stop=0x8c0993bcu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0993be; /* tst r3,r3 */
L8c0993be: if((*sr&1u))goto L8c09942a;goto L8c0993c0; /* bt 0x8c09942a */
L8c0993c0: *stop=0x8c0993c0u;r[5]=rd(m,0x8c099400u,4);goto L8c0993c2; /* mov.l 0x8c099400,r5 */
L8c0993c2: target=r[11];*pr=0x8c0993c6u;*stop=0x8c0993c4u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0993c6; /* jsr @r11 */
L8c0993c6: *stop=0x8c0993c6u;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c0993c8; /* tst r0,r0 */
L8c0993c8: if(!(*sr&1u))goto L8c099420;goto L8c0993ca; /* bf 0x8c099420 */
L8c0993ca: *stop=0x8c0993cau;if(!valid_ram(r[12],4u))return -3;r[2]=rd(m,r[12],4);goto L8c0993cc; /* mov.l @r12,r2 */
L8c0993cc: *stop=0x8c0993ccu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c0993ce; /* tst r2,r2 */
L8c0993ce: if(!(*sr&1u))goto L8c0993d6;goto L8c0993d0; /* bf 0x8c0993d6 */
L8c0993d0: *stop=0x8c0993d0u;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c0993d2; /* mov.l @(0x4,r14),r5 */
L8c0993d2: *stop=0x8c0993d4u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);goto L8c0993da; /* bra 0x8c0993da */
L8c0993d6: *stop=0x8c0993d6u;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c0993d8; /* mov.l @(0x4,r14),r5 */
L8c0993d8: *stop=0x8c0993d8u;r[4]=r[8];goto L8c0993da; /* mov r8,r4 */
L8c0993da: target=0x8c099750u;*pr=0x8c0993deu;*stop=0x8c0993dcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0993de; /* bsr 0x8c099750 */
L8c0993de: *stop=0x8c0993e0u;goto L8c09942e; /* bra 0x8c09942e */
L8c099420: *stop=0x8c099420u;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c099422; /* mov.l @(0x4,r14),r5 */
L8c099422: target=0x8c099750u;*pr=0x8c099426u;*stop=0x8c099424u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099426; /* bsr 0x8c099750 */
L8c099426: *stop=0x8c099428u;goto L8c09942e; /* bra 0x8c09942e */
L8c09942a: target=0x8c098ee0u;*pr=0x8c09942eu;*stop=0x8c09942cu;if(!valid_ram(r[14]+4u,4u))return -3;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09942e; /* bsr 0x8c098ee0 */
L8c09942e: *stop=0x8c09942eu;if(!valid_ram(r[15],4u))return -3;r[2]=rd(m,r[15],4);goto L8c099430; /* mov.l @r15,r2 */
L8c099430: *stop=0x8c099430u;r[13]+=4u;goto L8c099432; /* add #0x4,r13 */
L8c099432: *stop=0x8c099432u;*sr=(*sr&~1u)|(r[13]>=r[2]);goto L8c099434; /* cmp/hs r2,r13 */
L8c099434: take=(*sr&1u);*stop=0x8c099436u;r[14]+=8u;if(take)goto L8c09943c;goto L8c099438; /* bt/s 0x8c09943c */
L8c099438: *stop=0x8c09943au;goto L8c09929c; /* bra 0x8c09929c */
L8c09943c: *stop=0x8c09943cu;if(!valid_ram(r[14]+4u,4u))return -3;r[2]=rd(m,r[14]+0x4u,4);goto L8c09943e; /* mov.l @(0x4,r14),r2 */
L8c09943e: *stop=0x8c09943eu;r[8]=rd(m,0x8c0994e0u,4);goto L8c099440; /* mov.l 0x8c0994e0,r8 */
L8c099440: *stop=0x8c099440u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c099442; /* tst r2,r2 */
L8c099442: take=(*sr&1u);*stop=0x8c099444u;r[13]=r[10];if(take)goto L8c0994b4;goto L8c099446; /* bt/s 0x8c0994b4 */
L8c099446: *stop=0x8c099446u;if(!valid_ram(r[14]+4u,4u))return -3;r[3]=rd(m,r[14]+0x4u,4);goto L8c099448; /* mov.l @(0x4,r14),r3 */
L8c099448: *stop=0x8c099448u;r[0]=r[13];goto L8c09944a; /* mov r13,r0 */
L8c09944a: *stop=0x8c09944au;r[0]<<=2;goto L8c09944c; /* shll2 r0 */
L8c09944c: *stop=0x8c09944cu;if(!valid_ram(r[0]+r[9],4u))return -3;wr(m,r[9]+r[0],r[3],4);goto L8c09944e; /* mov.l r3,@(r0,r9) */
L8c09944e: *stop=0x8c09944eu;if(!valid_ram(r[14],4u))return -3;r[2]=rd(m,r[14],4);goto L8c099450; /* mov.l @r14,r2 */
L8c099450: *stop=0x8c099450u;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c099452; /* tst r2,r2 */
L8c099452: if((*sr&1u))goto L8c0994a6;goto L8c099454; /* bt 0x8c0994a6 */
L8c099454: *stop=0x8c099454u;r[5]=r[8];goto L8c099456; /* mov r8,r5 */
L8c099456: target=r[11];*pr=0x8c09945au;*stop=0x8c099458u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c09945a; /* jsr @r11 */
L8c09945a: *stop=0x8c09945au;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c09945c; /* tst r0,r0 */
L8c09945c: if(!(*sr&1u))goto L8c09949c;goto L8c09945e; /* bf 0x8c09949c */
L8c09945e: *stop=0x8c09945eu;if(!valid_ram(r[12],4u))return -3;r[0]=rd(m,r[12],4);goto L8c099460; /* mov.l @r12,r0 */
L8c099460: *stop=0x8c099460u;*sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c099462; /* cmp/eq #0x0,r0 */
L8c099462: if((*sr&1u))goto L8c099478;goto L8c099464; /* bt 0x8c099478 */
L8c099464: *stop=0x8c099464u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c099466; /* cmp/eq #0x1,r0 */
L8c099466: if((*sr&1u))goto L8c09947e;goto L8c099468; /* bt 0x8c09947e */
L8c099468: *stop=0x8c099468u;*sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c09946a; /* cmp/eq #0x2,r0 */
L8c09946a: if((*sr&1u))goto L8c099484;goto L8c09946c; /* bt 0x8c099484 */
L8c09946c: *stop=0x8c09946cu;*sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c09946e; /* cmp/eq #0x3,r0 */
L8c09946e: if((*sr&1u))goto L8c09948a;goto L8c099470; /* bt 0x8c09948a */
L8c099470: *stop=0x8c099470u;*sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c099472; /* cmp/eq #0x4,r0 */
L8c099472: if((*sr&1u))goto L8c099490;goto L8c099474; /* bt 0x8c099490 */
L8c099474: *stop=0x8c099476u;goto L8c0994aa; /* bra 0x8c0994aa */
L8c099478: *stop=0x8c099478u;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c09947a; /* mov.l @(0x4,r14),r5 */
L8c09947a: *stop=0x8c09947cu;r[4]=r[8];goto L8c099494; /* bra 0x8c099494 */
L8c09947e: *stop=0x8c09947eu;r[4]=rd(m,0x8c0994e4u,4);goto L8c099480; /* mov.l 0x8c0994e4,r4 */
L8c099480: *stop=0x8c099482u;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c099494; /* bra 0x8c099494 */
L8c099484: *stop=0x8c099484u;r[4]=rd(m,0x8c0994e8u,4);goto L8c099486; /* mov.l 0x8c0994e8,r4 */
L8c099486: *stop=0x8c099488u;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c099494; /* bra 0x8c099494 */
L8c09948a: *stop=0x8c09948au;r[4]=rd(m,0x8c0994ecu,4);goto L8c09948c; /* mov.l 0x8c0994ec,r4 */
L8c09948c: *stop=0x8c09948eu;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c099494; /* bra 0x8c099494 */
L8c099490: *stop=0x8c099490u;r[4]=rd(m,0x8c0994f0u,4);goto L8c099492; /* mov.l 0x8c0994f0,r4 */
L8c099492: *stop=0x8c099492u;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c099494; /* mov.l @(0x4,r14),r5 */
L8c099494: target=0x8c099750u;*pr=0x8c099498u;*stop=0x8c099496u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099498; /* bsr 0x8c099750 */
L8c099498: *stop=0x8c09949au;goto L8c0994aa; /* bra 0x8c0994aa */
L8c09949c: *stop=0x8c09949cu;if(!valid_ram(r[14]+4u,4u))return -3;r[5]=rd(m,r[14]+0x4u,4);goto L8c09949e; /* mov.l @(0x4,r14),r5 */
L8c09949e: target=0x8c099750u;*pr=0x8c0994a2u;*stop=0x8c0994a0u;if(!valid_ram(r[14],4u))return -3;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0994a2; /* bsr 0x8c099750 */
L8c0994a2: *stop=0x8c0994a4u;goto L8c0994aa; /* bra 0x8c0994aa */
L8c0994a6: target=0x8c098ee0u;*pr=0x8c0994aau;*stop=0x8c0994a8u;if(!valid_ram(r[14]+4u,4u))return -3;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0994aa; /* bsr 0x8c098ee0 */
L8c0994aa: *stop=0x8c0994aau;r[14]+=8u;goto L8c0994ac; /* add #0x8,r14 */
L8c0994ac: *stop=0x8c0994acu;if(!valid_ram(r[14]+4u,4u))return -3;r[2]=rd(m,r[14]+0x4u,4);goto L8c0994ae; /* mov.l @(0x4,r14),r2 */
L8c0994ae: *stop=0x8c0994aeu;*sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c0994b0; /* tst r2,r2 */
L8c0994b0: take=!(*sr&1u);*stop=0x8c0994b2u;r[13]+=1u;if(take)goto L8c099446;goto L8c0994b4; /* bf/s 0x8c099446 */
L8c0994b4: *stop=0x8c0994b4u;r[0]=r[13];goto L8c0994b6; /* mov r13,r0 */
L8c0994b6: *stop=0x8c0994b6u;r[0]<<=2;goto L8c0994b8; /* shll2 r0 */
L8c0994b8: *stop=0x8c0994b8u;if(!valid_ram(r[0]+r[9],4u))return -3;wr(m,r[9]+r[0],r[10],4);goto L8c0994ba; /* mov.l r10,@(r0,r9) */
L8c0994ba: *stop=0x8c0994bau;r[15]+=4u;goto L8c0994bc; /* add #0x4,r15 */
L8c0994bc: *stop=0x8c0994bcu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c0994be; /* lds.l @r15+,PR */
L8c0994be: *stop=0x8c0994beu;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c0994c0; /* mov.l @r15+,r8 */
L8c0994c0: *stop=0x8c0994c0u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c0994c2; /* mov.l @r15+,r9 */
L8c0994c2: *stop=0x8c0994c2u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c0994c4; /* mov.l @r15+,r10 */
L8c0994c4: *stop=0x8c0994c4u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c0994c6; /* mov.l @r15+,r11 */
L8c0994c6: *stop=0x8c0994c6u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c0994c8; /* mov.l @r15+,r12 */
L8c0994c8: *stop=0x8c0994c8u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c0994ca; /* mov.l @r15+,r13 */
L8c0994ca: *stop=0x8c0994cau;if(*pr!=entry_pr)return -3;*stop=0x8c0994ccu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_world_act_textures_8c0995e0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c0995e0;
L8c0995e0: *stop=0x8c0995e0u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c0995e2; /* mov.l r14,@-r15 */
L8c0995e2: *stop=0x8c0995e2u;r[5]=rd(m,0x8c0996e0u,4);goto L8c0995e4; /* mov.l 0x8c0996e0,r5 */
L8c0995e4: *stop=0x8c0995e4u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c0995e6; /* mov.l r13,@-r15 */
L8c0995e6: *stop=0x8c0995e6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c0995e8; /* mov.l r12,@-r15 */
L8c0995e8: *stop=0x8c0995e8u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c0995ea; /* sts.l PR,@-r15 */
L8c0995ea: *stop=0x8c0995eau;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c0995ec; /* mov.l @r5,r3 */
L8c0995ec: *stop=0x8c0995ecu;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0995ee; /* tst r3,r3 */
L8c0995ee: if((*sr&1u))goto L8c09962c;goto L8c0995f0; /* bt 0x8c09962c */
L8c0995f0: *stop=0x8c0995f0u;if(!valid_ram(r[5],4u))return -3;r[14]=rd(m,r[5],4);goto L8c0995f2; /* mov.l @r5,r14 */
L8c0995f2: *stop=0x8c0995f2u;r[2]=r[4]&65535u;goto L8c0995f4; /* extu.w r4,r2 */
L8c0995f4: *stop=0x8c0995f4u;if(!valid_ram(r[14],2u))return -3;r[3]=sx(m,r[14],2);goto L8c0995f6; /* mov.w @r14,r3 */
L8c0995f6: *stop=0x8c0995f6u;r[3]=r[3]&65535u;goto L8c0995f8; /* extu.w r3,r3 */
L8c0995f8: *stop=0x8c0995f8u;*sr=(*sr&~1u)|(r[2]==r[3]);goto L8c0995fa; /* cmp/eq r2,r3 */
L8c0995fa: if(!(*sr&1u))goto L8c099624;goto L8c0995fc; /* bf 0x8c099624 */
L8c0995fc: *stop=0x8c0995fcu;if(!valid_ram(r[14]+3u,1u))return -3;r[0]=sx(m,r[14]+0x3u,1);goto L8c0995fe; /* mov.b @(0x3,r14),r0 */
L8c0995fe: *stop=0x8c0995feu;*sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c099600; /* tst r0,r0 */
L8c099600: if(!(*sr&1u))goto L8c09961a;goto L8c099602; /* bf 0x8c09961a */
L8c099602: *stop=0x8c099602u;if(!valid_ram(r[14]+2u,1u))return -3;r[0]=sx(m,r[14]+0x2u,1);goto L8c099604; /* mov.b @(0x2,r14),r0 */
L8c099604: *stop=0x8c099604u;r[12]=r[0];goto L8c099606; /* mov r0,r12 */
L8c099606: *stop=0x8c099606u;*sr=(*sr&~1u)|((int32_t)r[12]>0);goto L8c099608; /* cmp/pl r12 */
L8c099608: take=!(*sr&1u);*stop=0x8c09960au;if(!valid_ram(r[14]+4u,4u))return -3;r[13]=rd(m,r[14]+0x4u,4);if(take)goto L8c09961a;goto L8c09960c; /* bf/s 0x8c09961a */
L8c09960c: *stop=0x8c09960cu;if(!valid_ram(r[13]+4u,4u))return -3;r[5]=rd(m,r[13]+0x4u,4);goto L8c09960e; /* mov.l @(0x4,r13),r5 */
L8c09960e: target=0x8c099750u;*pr=0x8c099612u;*stop=0x8c099610u;if(!valid_ram(r[13],4u))return -3;r[4]=rd(m,r[13],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c099612; /* bsr 0x8c099750 */
L8c099612: *stop=0x8c099612u;r[12]+=4294967295u;goto L8c099614; /* add #-0x1,r12 */
L8c099614: *stop=0x8c099614u;*sr=(*sr&~1u)|((int32_t)r[12]>0);goto L8c099616; /* cmp/pl r12 */
L8c099616: take=(*sr&1u);*stop=0x8c099618u;r[13]+=8u;if(take)goto L8c09960c;goto L8c09961a; /* bt/s 0x8c09960c */
L8c09961a: *stop=0x8c09961au;if(!valid_ram(r[14]+3u,1u))return -3;r[0]=sx(m,r[14]+0x3u,1);goto L8c09961c; /* mov.b @(0x3,r14),r0 */
L8c09961c: *stop=0x8c09961cu;r[0]+=1u;goto L8c09961e; /* add #0x1,r0 */
L8c09961e: *stop=0x8c09961eu;if(!valid_ram(r[14]+3u,1u))return -3;wr(m,r[14]+0x3u,r[0],1);goto L8c099620; /* mov.b r0,@(0x3,r14) */
L8c099620: *stop=0x8c099622u;r[0]=1u;goto L8c09962e; /* bra 0x8c09962e */
L8c099624: *stop=0x8c099624u;r[5]+=4u;goto L8c099626; /* add #0x4,r5 */
L8c099626: *stop=0x8c099626u;if(!valid_ram(r[5],4u))return -3;r[3]=rd(m,r[5],4);goto L8c099628; /* mov.l @r5,r3 */
L8c099628: *stop=0x8c099628u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c09962a; /* tst r3,r3 */
L8c09962a: if(!(*sr&1u))goto L8c0995f0;goto L8c09962c; /* bf 0x8c0995f0 */
L8c09962c: *stop=0x8c09962cu;r[0]=0u;goto L8c09962e; /* mov #0x0,r0 */
L8c09962e: *stop=0x8c09962eu;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c099630; /* lds.l @r15+,PR */
L8c099630: *stop=0x8c099630u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c099632; /* mov.l @r15+,r12 */
L8c099632: *stop=0x8c099632u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c099634; /* mov.l @r15+,r13 */
L8c099634: *stop=0x8c099634u;if(*pr!=entry_pr)return -3;*stop=0x8c099636u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_world_special_stage_query_8c0908b6(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)m;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c0908b6;
L8c0908b6: *stop=0x8c0908b6u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c0908b8; /* sts.l PR,@-r15 */
L8c0908b8: *stop=0x8c0908b8u;r[3]=rd(m,0x8c090950u,4);goto L8c0908ba; /* mov.l 0x8c090950,r3 */
L8c0908ba: target=r[3];*pr=0x8c0908beu;*stop=0x8c0908bcu;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0908be; /* jsr @r3 */
L8c0908be: *stop=0x8c0908beu;r[1]=rd(m,0x8c09095cu,4);goto L8c0908c0; /* mov.l 0x8c09095c,r1 */
L8c0908c0: *stop=0x8c0908c0u;*sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto L8c0908c2; /* shll r0 */
L8c0908c2: *stop=0x8c0908c2u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c0908c4; /* lds.l @r15+,PR */
L8c0908c4: *stop=0x8c0908c4u;if(!valid_ram(r[0]+r[1],2u))return -3;r[4]=sx(m,r[0]+r[1],2);goto L8c0908c6; /* mov.w @(r0,r1),r4 */
L8c0908c6: *stop=0x8c0908c6u;r[0]=rd(m,0x8c090930u,4);goto L8c0908c8; /* mov.l 0x8c090930,r0 */
L8c0908c8: *stop=0x8c0908c8u;if(!valid_ram(r[0]+r[4],1u))return -3;r[5]=sx(m,r[4]+r[0],1);goto L8c0908ca; /* mov.b @(r0,r4),r5 */
L8c0908ca: *stop=0x8c0908cau;r[5]=r[5]&255u;goto L8c0908cc; /* extu.b r5,r5 */
L8c0908cc: *stop=0x8c0908ccu;*sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c0908ce; /* tst r5,r5 */
L8c0908ce: *stop=0x8c0908ceu;r[5]=*sr&1u;goto L8c0908d0; /* movt r5 */
L8c0908d0: *stop=0x8c0908d0u;r[5]+=4294967295u;goto L8c0908d2; /* add #-0x1,r5 */
L8c0908d2: *stop=0x8c0908d2u;r[5]=0u-r[5];goto L8c0908d4; /* neg r5,r5 */
L8c0908d4: *stop=0x8c0908d4u;if(*pr!=entry_pr)return -3;*stop=0x8c0908d6u;r[0]=r[5];return 0; /* rts */
}
