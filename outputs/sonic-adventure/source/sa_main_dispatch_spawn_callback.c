/* Spawn callback and transform/particle helpers; names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_add_sub.h"
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
SA_STAGE_EXPORT int sa_main_dispatch_spawn_callback_8c044680(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c044680;
L8c044680: r[15]-=4;wr(m,r[15],r[14],4);goto L8c044682; /* mov.l r14,@-r15 */
L8c044682: r[15]-=4;wr(m,r[15],*pr,4);goto L8c044684; /* sts.l PR,@-r15 */
L8c044684: r[15]+=4294967292u;goto L8c044686; /* add #-0x4,r15 */
L8c044686: r[14]=r[4];goto L8c044688; /* mov r4,r14 */
L8c044688: wr(m,r[15],r[4],4);goto L8c04468a; /* mov.l r4,@r15 */
L8c04468a: r[14]=rd(m,r[14]+0x20u,4);goto L8c04468c; /* mov.l @(0x20,r14),r14 */
L8c04468c: target=0x8c0446a2u;*pr=0x8c044690u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044690; /* bsr 0x8c0446a2 */
L8c044690: target=0x8c04474au;*pr=0x8c044694u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044694; /* bsr 0x8c04474a */
L8c044694: r[3]=rd(m,0x8c044840u,4);goto L8c044696; /* mov.l 0x8c044840,r3 */
L8c044696: target=r[3];*pr=0x8c04469au;r[4]=rd(m,r[15],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04469a; /* jsr @r3 */
L8c04469a: r[15]+=4u;goto L8c04469c; /* add #0x4,r15 */
L8c04469c: *pr=rd(m,r[15],4);r[15]+=4;goto L8c04469e; /* lds.l @r15+,PR */
L8c04469e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_spawn_work_transform_8c0446a2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c0446a2;
L8c0446a2: r[15]-=4;wr(m,r[15],r[14],4);goto L8c0446a4; /* mov.l r14,@-r15 */
L8c0446a4: r[15]-=4;wr(m,r[15],r[13],4);goto L8c0446a6; /* mov.l r13,@-r15 */
L8c0446a6: r[15]-=4;wr(m,r[15],*pr,4);goto L8c0446a8; /* sts.l PR,@-r15 */
L8c0446a8: r[15]+=4294967268u;goto L8c0446aa; /* add #-0x1c,r15 */
L8c0446aa: r[13]=r[4];goto L8c0446ac; /* mov r4,r13 */
L8c0446ac: r[3]=1u;goto L8c0446ae; /* mov #0x1,r3 */
L8c0446ae: wr(m,r[13],r[3],1);goto L8c0446b0; /* mov.b r3,@r13 */
L8c0446b0: r[0]=sx(m,r[13]+0x8u,1);goto L8c0446b2; /* mov.b @(0x8,r13),r0 */
L8c0446b2: r[14]=r[0]&255u;goto L8c0446b4; /* extu.b r0,r14 */
L8c0446b4: r[0]=rd(m,0x8c044844u,4);goto L8c0446b6; /* mov.l 0x8c044844,r0 */
L8c0446b6: r[14]<<=2;goto L8c0446b8; /* shll2 r14 */
L8c0446b8: r[14]=rd(m,r[14]+r[0],4);goto L8c0446ba; /* mov.l @(r0,r14),r14 */
L8c0446ba: *sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c0446bc; /* tst r14,r14 */
L8c0446bc: if((*sr&1u))goto L8c044740;goto L8c0446be; /* bt 0x8c044740 */
L8c0446be: r[0]=24u;goto L8c0446c0; /* mov #0x18,r0 */
L8c0446c0: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0446c2; /* fldi0 fr3 */
L8c0446c2: fstore(m,f,3u,r[0]+r[15]);goto L8c0446c4; /* fmov fr3,@(r0,r15) */
L8c0446c4: r[0]=16u;goto L8c0446c6; /* mov #0x10,r0 */
L8c0446c6: fstore(m,f,3u,r[0]+r[15]);goto L8c0446c8; /* fmov fr3,@(r0,r15) */
L8c0446c8: r[0]=0x8c044848u;goto L8c0446ca; /* mova 0x8c044848,r0 */
L8c0446ca: r[4]=rd(m,0x8c04484cu,4);goto L8c0446cc; /* mov.l 0x8c04484c,r4 */
L8c0446cc: r[3]=rd(m,0x8c044850u,4);goto L8c0446ce; /* mov.l 0x8c044850,r3 */
L8c0446ce: fload(m,f,3u,r[0]);goto L8c0446d0; /* fmov.s @r0,fr3 */
L8c0446d0: r[0]=20u;goto L8c0446d2; /* mov #0x14,r0 */
L8c0446d2: target=r[3];*pr=0x8c0446d6u;fstore(m,f,3u,r[0]+r[15]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0446d6; /* jsr @r3 */
L8c0446d6: r[2]=rd(m,r[14]+0x1cu,4);goto L8c0446d8; /* mov.l @(0x1c,r14),r2 */
L8c0446d8: *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c0446da; /* tst r2,r2 */
L8c0446da: take=(*sr&1u);wr(m,r[15],r[2],4);if(take)goto L8c0446e6;goto L8c0446de; /* bt/s 0x8c0446e6 */
L8c0446de: r[3]=rd(m,0x8c044854u,4);goto L8c0446e0; /* mov.l 0x8c044854,r3 */
L8c0446e0: r[5]=rd(m,r[15],4);goto L8c0446e2; /* mov.l @r15,r5 */
L8c0446e2: target=r[3];*pr=0x8c0446e6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0446e6; /* jsr @r3 */
L8c0446e6: r[3]=rd(m,r[14]+0x14u,4);goto L8c0446e8; /* mov.l @(0x14,r14),r3 */
L8c0446e8: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0446ea; /* tst r3,r3 */
L8c0446ea: take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto L8c0446f6;goto L8c0446ee; /* bt/s 0x8c0446f6 */
L8c0446ee: r[3]=rd(m,0x8c044858u,4);goto L8c0446f0; /* mov.l 0x8c044858,r3 */
L8c0446f0: r[5]=rd(m,r[15],4);goto L8c0446f2; /* mov.l @r15,r5 */
L8c0446f2: target=r[3];*pr=0x8c0446f6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0446f6; /* jsr @r3 */
L8c0446f6: r[3]=rd(m,r[14]+0x18u,4);goto L8c0446f8; /* mov.l @(0x18,r14),r3 */
L8c0446f8: r[3]=0u-r[3];goto L8c0446fa; /* neg r3,r3 */
L8c0446fa: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0446fc; /* tst r3,r3 */
L8c0446fc: take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto L8c044708;goto L8c044700; /* bt/s 0x8c044708 */
L8c044700: r[1]=rd(m,0x8c04485cu,4);goto L8c044702; /* mov.l 0x8c04485c,r1 */
L8c044702: r[5]=r[3];goto L8c044704; /* mov r3,r5 */
L8c044704: target=r[1];*pr=0x8c044708u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044708; /* jsr @r1 */
L8c044708: r[5]=r[15];goto L8c04470a; /* mov r15,r5 */
L8c04470a: r[2]=rd(m,0x8c044860u,4);goto L8c04470c; /* mov.l 0x8c044860,r2 */
L8c04470c: r[6]=r[15];goto L8c04470e; /* mov r15,r6 */
L8c04470e: r[5]+=16u;goto L8c044710; /* add #0x10,r5 */
L8c044710: r[6]+=4u;goto L8c044712; /* add #0x4,r6 */
L8c044712: target=r[2];*pr=0x8c044716u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044716; /* jsr @r2 */
L8c044716: r[3]=rd(m,0x8c044864u,4);goto L8c044718; /* mov.l 0x8c044864,r3 */
L8c044718: target=r[3];*pr=0x8c04471cu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04471c; /* jsr @r3 */
L8c04471c: r[0]=4u;goto L8c04471e; /* mov #0x4,r0 */
L8c04471e: fload(m,f,3u,r[0]+r[15]);goto L8c044720; /* fmov.s @(r0,r15),fr3 */
L8c044720: r[0]=32u;goto L8c044722; /* mov #0x20,r0 */
L8c044722: fload(m,f,2u,r[0]+r[14]);goto L8c044724; /* fmov.s @(r0,r14),fr2 */
L8c044724: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c044726; /* fadd fr3,fr2 */
L8c044726: fstore(m,f,2u,r[0]+r[13]);goto L8c044728; /* fmov fr2,@(r0,r13) */
L8c044728: r[0]=8u;goto L8c04472a; /* mov #0x8,r0 */
L8c04472a: fload(m,f,3u,r[0]+r[15]);goto L8c04472c; /* fmov.s @(r0,r15),fr3 */
L8c04472c: r[0]=36u;goto L8c04472e; /* mov #0x24,r0 */
L8c04472e: fload(m,f,2u,r[0]+r[14]);goto L8c044730; /* fmov.s @(r0,r14),fr2 */
L8c044730: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c044732; /* fadd fr3,fr2 */
L8c044732: fstore(m,f,2u,r[0]+r[13]);goto L8c044734; /* fmov fr2,@(r0,r13) */
L8c044734: r[0]=12u;goto L8c044736; /* mov #0xc,r0 */
L8c044736: fload(m,f,3u,r[0]+r[15]);goto L8c044738; /* fmov.s @(r0,r15),fr3 */
L8c044738: r[0]=40u;goto L8c04473a; /* mov #0x28,r0 */
L8c04473a: fload(m,f,2u,r[0]+r[14]);goto L8c04473c; /* fmov.s @(r0,r14),fr2 */
L8c04473c: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c04473e; /* fadd fr3,fr2 */
L8c04473e: fstore(m,f,2u,r[0]+r[13]);goto L8c044740; /* fmov fr2,@(r0,r13) */
L8c044740: r[15]+=28u;goto L8c044742; /* add #0x1c,r15 */
L8c044742: *pr=rd(m,r[15],4);r[15]+=4;goto L8c044744; /* lds.l @r15+,PR */
L8c044744: r[13]=rd(m,r[15],4);r[15]+=4;goto L8c044746; /* mov.l @r15+,r13 */
L8c044746: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_spawn_particle_emit_8c04474a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c04474a;
L8c04474a: r[15]-=4;wr(m,r[15],r[14],4);goto L8c04474c; /* mov.l r14,@-r15 */
L8c04474c: r[15]-=4;wr(m,r[15],r[13],4);goto L8c04474e; /* mov.l r13,@-r15 */
L8c04474e: r[15]-=4;wr(m,r[15],r[12],4);goto L8c044750; /* mov.l r12,@-r15 */
L8c044750: r[15]-=4;wr(m,r[15],r[11],4);goto L8c044752; /* mov.l r11,@-r15 */
L8c044752: r[15]-=4;wr(m,r[15],r[10],4);goto L8c044754; /* mov.l r10,@-r15 */
L8c044754: r[15]-=4;wr(m,r[15],r[9],4);goto L8c044756; /* mov.l r9,@-r15 */
L8c044756: r[15]-=4;wr(m,r[15],r[8],4);goto L8c044758; /* mov.l r8,@-r15 */
L8c044758: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c04475a; /* fmov.s fr15,@-r15 */
L8c04475a: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[15]);goto L8c04475c; /* fmov.s fr14,@-r15 */
L8c04475c: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[15]);goto L8c04475e; /* fmov.s fr13,@-r15 */
L8c04475e: r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[15]);goto L8c044760; /* fmov.s fr12,@-r15 */
L8c044760: r[15]-=4;wr(m,r[15],*pr,4);goto L8c044762; /* sts.l PR,@-r15 */
L8c044762: r[15]+=4294967292u;goto L8c044764; /* add #-0x4,r15 */
L8c044764: r[0]=0x8c044870u;goto L8c044766; /* mova 0x8c044870,r0 */
L8c044766: r[11]=rd(m,0x8c04487cu,4);goto L8c044768; /* mov.l 0x8c04487c,r11 */
L8c044768: r[12]=64u;goto L8c04476a; /* mov #0x40,r12 */
L8c04476a: r[8]=rd(m,0x8c044868u,4);goto L8c04476c; /* mov.l 0x8c044868,r8 */
L8c04476c: r[12]<<=8;goto L8c04476e; /* shll8 r12 */
L8c04476e: r[9]=rd(m,0x8c04486cu,4);goto L8c044770; /* mov.l 0x8c04486c,r9 */
L8c044770: r[12]+=4294967295u;goto L8c044772; /* add #-0x1,r12 */
L8c044772: wr(m,r[15],r[4],4);goto L8c044774; /* mov.l r4,@r15 */
L8c044774: fload(m,f,12u,r[0]);goto L8c044776; /* fmov.s @r0,fr12 */
L8c044776: r[0]=0x8c044874u;goto L8c044778; /* mova 0x8c044874,r0 */
L8c044778: fload(m,f,13u,r[0]);goto L8c04477a; /* fmov.s @r0,fr13 */
L8c04477a: r[0]=0x8c044878u;goto L8c04477c; /* mova 0x8c044878,r0 */
L8c04477c: fload(m,f,14u,r[0]);goto L8c04477e; /* fmov.s @r0,fr14 */
L8c04477e: r[0]=0x8c044880u;goto L8c044780; /* mova 0x8c044880,r0 */
L8c044780: fload(m,f,15u,r[0]);goto L8c044782; /* fmov.s @r0,fr15 */
L8c044782: r[13]=0u;goto L8c044784; /* mov #0x0,r13 */
L8c044784: r[10]=6u;goto L8c044786; /* mov #0x6,r10 */
L8c044786: r[6]=rd(m,0x8c044884u,4);goto L8c044788; /* mov.l 0x8c044884,r6 */
L8c044788: r[5]=5u;goto L8c04478a; /* mov #0x5,r5 */
L8c04478a: target=r[9];*pr=0x8c04478eu;r[4]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04478e; /* jsr @r9 */
L8c04478e: r[14]=r[0];goto L8c044790; /* mov r0,r14 */
L8c044790: *sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c044792; /* tst r14,r14 */
L8c044792: if((*sr&1u))goto L8c0447fe;goto L8c044794; /* bt 0x8c0447fe */
L8c044794: target=r[8];*pr=0x8c044798u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044798; /* jsr @r8 */
L8c044798: *fpul=r[0];goto L8c04479a; /* lds r0,FPUL */
L8c04479a: r[2]=rd(m,0x8c044888u,4);goto L8c04479c; /* mov.l 0x8c044888,r2 */
L8c04479c: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c04479e; /* float FPUL,fr3 */
L8c04479e: *fpul=r[2];goto L8c0447a0; /* lds r2,FPUL */
L8c0447a0: fcopy(f,4u,3u);goto L8c0447a2; /* fmov fr3,fr4 */
L8c0447a2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c0447a4; /* fmul fr12,fr4 */
L8c0447a4: f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto L8c0447a6; /* fsts FPUL,fr3 */
L8c0447a6: fcopy(f,2u,4u);goto L8c0447a8; /* fmov fr4,fr2 */
L8c0447a8: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[13],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0447aa; /* fmul fr13,fr2 */
L8c0447aa: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c0447ac; /* fcmp/gt fr2,fr3 */
L8c0447ac: take=(*sr&1u);r[4]=rd(m,r[14]+0x20u,4);if(take)goto L8c0447b4;goto L8c0447b0; /* bt/s 0x8c0447b4 */
L8c0447b0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[3],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c0447b2; /* fadd fr3,fr3 */
L8c0447b2: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0447b4; /* fsub fr3,fr2 */
L8c0447b4: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c0447b6; /* ftrc fr2,FPUL */
L8c0447b6: r[2]=rd(m,0x8c044888u,4);goto L8c0447b8; /* mov.l 0x8c044888,r2 */
L8c0447b8: fcopy(f,2u,4u);goto L8c0447ba; /* fmov fr4,fr2 */
L8c0447ba: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[14],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0447bc; /* fmul fr14,fr2 */
L8c0447bc: r[3]=*fpul;goto L8c0447be; /* sts FPUL,r3 */
L8c0447be: *fpul=r[2];goto L8c0447c0; /* lds r2,FPUL */
L8c0447c0: r[3]<<=8;goto L8c0447c2; /* shll8 r3 */
L8c0447c2: f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto L8c0447c4; /* fsts FPUL,fr3 */
L8c0447c4: r[3]<<=2;goto L8c0447c6; /* shll2 r3 */
L8c0447c6: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c0447c8; /* shll r3 */
L8c0447c8: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c0447ca; /* fcmp/gt fr2,fr3 */
L8c0447ca: wr(m,r[4]+0x18u,r[3],4);goto L8c0447cc; /* mov.l r3,@(0x18,r4) */
L8c0447cc: if((*sr&1u))goto L8c0447d2;goto L8c0447ce; /* bt 0x8c0447d2 */
L8c0447ce: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[3],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c0447d0; /* fadd fr3,fr3 */
L8c0447d0: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0447d2; /* fsub fr3,fr2 */
L8c0447d2: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c0447d4; /* ftrc fr2,FPUL */
L8c0447d4: fcopy(f,3u,4u);goto L8c0447d6; /* fmov fr4,fr3 */
L8c0447d6: {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0447d8; /* fmul fr15,fr3 */
L8c0447d8: r[13]+=1u;goto L8c0447da; /* add #0x1,r13 */
L8c0447da: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[10]);goto L8c0447dc; /* cmp/ge r10,r13 */
L8c0447dc: r[3]=*fpul;goto L8c0447de; /* sts FPUL,r3 */
L8c0447de: status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c0447e0; /* ftrc fr3,FPUL */
L8c0447e0: r[3]&=r[12];goto L8c0447e2; /* and r12,r3 */
L8c0447e2: r[2]=0u-r[3];goto L8c0447e4; /* neg r3,r2 */
L8c0447e4: r[2]+=r[11];goto L8c0447e6; /* add r11,r2 */
L8c0447e6: r[0]=*fpul;goto L8c0447e8; /* sts FPUL,r0 */
L8c0447e8: wr(m,r[4]+0x14u,r[2],4);goto L8c0447ea; /* mov.l r2,@(0x14,r4) */
L8c0447ea: r[0]&=0x1u;goto L8c0447ec; /* and #0x1,r0 */
L8c0447ec: wr(m,r[4]+0x3u,r[0],1);goto L8c0447ee; /* mov.b r0,@(0x3,r4) */
L8c0447ee: r[3]=rd(m,r[15],4);goto L8c0447f0; /* mov.l @r15,r3 */
L8c0447f0: r[2]=rd(m,r[3]+0x20u,4);goto L8c0447f2; /* mov.l @(0x20,r3),r2 */
L8c0447f2: r[1]=rd(m,r[3]+0x24u,4);goto L8c0447f4; /* mov.l @(0x24,r3),r1 */
L8c0447f4: wr(m,r[4]+0x20u,r[2],4);goto L8c0447f6; /* mov.l r2,@(0x20,r4) */
L8c0447f6: wr(m,r[4]+0x24u,r[1],4);goto L8c0447f8; /* mov.l r1,@(0x24,r4) */
L8c0447f8: r[2]=rd(m,r[3]+0x28u,4);goto L8c0447fa; /* mov.l @(0x28,r3),r2 */
L8c0447fa: take=!(*sr&1u);wr(m,r[4]+0x28u,r[2],4);if(take)goto L8c044786;goto L8c0447fe; /* bf/s 0x8c044786 */
L8c0447fe: r[15]+=4u;goto L8c044800; /* add #0x4,r15 */
L8c044800: *pr=rd(m,r[15],4);r[15]+=4;goto L8c044802; /* lds.l @r15+,PR */
L8c044802: fload(m,f,12u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c044804; /* fmov.s @r15+,fr12 */
L8c044804: fload(m,f,13u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c044806; /* fmov.s @r15+,fr13 */
L8c044806: fload(m,f,14u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c044808; /* fmov.s @r15+,fr14 */
L8c044808: fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c04480a; /* fmov.s @r15+,fr15 */
L8c04480a: r[8]=rd(m,r[15],4);r[15]+=4;goto L8c04480c; /* mov.l @r15+,r8 */
L8c04480c: r[9]=rd(m,r[15],4);r[15]+=4;goto L8c04480e; /* mov.l @r15+,r9 */
L8c04480e: r[10]=rd(m,r[15],4);r[15]+=4;goto L8c044810; /* mov.l @r15+,r10 */
L8c044810: r[11]=rd(m,r[15],4);r[15]+=4;goto L8c044812; /* mov.l @r15+,r11 */
L8c044812: r[12]=rd(m,r[15],4);r[15]+=4;goto L8c044814; /* mov.l @r15+,r12 */
L8c044814: r[13]=rd(m,r[15],4);r[15]+=4;goto L8c044816; /* mov.l @r15+,r13 */
L8c044816: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
