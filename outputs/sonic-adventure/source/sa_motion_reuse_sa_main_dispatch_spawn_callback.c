#include "sa_draw_motion_stop.h"
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
L8c044680: sa_motion_publish_stop(ctx,0x8c044680u); r[15]-=4;wr(m,r[15],r[14],4);goto L8c044682; /* mov.l r14,@-r15 */
L8c044682: sa_motion_publish_stop(ctx,0x8c044682u); r[15]-=4;wr(m,r[15],*pr,4);goto L8c044684; /* sts.l PR,@-r15 */
L8c044684: sa_motion_publish_stop(ctx,0x8c044684u); r[15]+=4294967292u;goto L8c044686; /* add #-0x4,r15 */
L8c044686: sa_motion_publish_stop(ctx,0x8c044686u); r[14]=r[4];goto L8c044688; /* mov r4,r14 */
L8c044688: sa_motion_publish_stop(ctx,0x8c044688u); wr(m,r[15],r[4],4);goto L8c04468a; /* mov.l r4,@r15 */
L8c04468a: sa_motion_publish_stop(ctx,0x8c04468au); r[14]=rd(m,r[14]+0x20u,4);goto L8c04468c; /* mov.l @(0x20,r14),r14 */
L8c04468c: sa_motion_publish_stop(ctx,0x8c04468eu); target=0x8c0446a2u;*pr=0x8c044690u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044690; /* bsr 0x8c0446a2 */
L8c044690: sa_motion_publish_stop(ctx,0x8c044692u); target=0x8c04474au;*pr=0x8c044694u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044694; /* bsr 0x8c04474a */
L8c044694: sa_motion_publish_stop(ctx,0x8c044694u); r[3]=rd(m,0x8c044840u,4);goto L8c044696; /* mov.l 0x8c044840,r3 */
L8c044696: sa_motion_publish_stop(ctx,0x8c044698u); target=r[3];*pr=0x8c04469au;r[4]=rd(m,r[15],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04469a; /* jsr @r3 */
L8c04469a: sa_motion_publish_stop(ctx,0x8c04469au); r[15]+=4u;goto L8c04469c; /* add #0x4,r15 */
L8c04469c: sa_motion_publish_stop(ctx,0x8c04469cu); *pr=rd(m,r[15],4);r[15]+=4;goto L8c04469e; /* lds.l @r15+,PR */
L8c04469e: sa_motion_publish_stop(ctx,0x8c0446a0u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_spawn_work_transform_8c0446a2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c0446a2;
L8c0446a2: sa_motion_publish_stop(ctx,0x8c0446a2u); r[15]-=4;wr(m,r[15],r[14],4);goto L8c0446a4; /* mov.l r14,@-r15 */
L8c0446a4: sa_motion_publish_stop(ctx,0x8c0446a4u); r[15]-=4;wr(m,r[15],r[13],4);goto L8c0446a6; /* mov.l r13,@-r15 */
L8c0446a6: sa_motion_publish_stop(ctx,0x8c0446a6u); r[15]-=4;wr(m,r[15],*pr,4);goto L8c0446a8; /* sts.l PR,@-r15 */
L8c0446a8: sa_motion_publish_stop(ctx,0x8c0446a8u); r[15]+=4294967268u;goto L8c0446aa; /* add #-0x1c,r15 */
L8c0446aa: sa_motion_publish_stop(ctx,0x8c0446aau); r[13]=r[4];goto L8c0446ac; /* mov r4,r13 */
L8c0446ac: sa_motion_publish_stop(ctx,0x8c0446acu); r[3]=1u;goto L8c0446ae; /* mov #0x1,r3 */
L8c0446ae: sa_motion_publish_stop(ctx,0x8c0446aeu); wr(m,r[13],r[3],1);goto L8c0446b0; /* mov.b r3,@r13 */
L8c0446b0: sa_motion_publish_stop(ctx,0x8c0446b0u); r[0]=sx(m,r[13]+0x8u,1);goto L8c0446b2; /* mov.b @(0x8,r13),r0 */
L8c0446b2: sa_motion_publish_stop(ctx,0x8c0446b2u); r[14]=r[0]&255u;goto L8c0446b4; /* extu.b r0,r14 */
L8c0446b4: sa_motion_publish_stop(ctx,0x8c0446b4u); r[0]=rd(m,0x8c044844u,4);goto L8c0446b6; /* mov.l 0x8c044844,r0 */
L8c0446b6: sa_motion_publish_stop(ctx,0x8c0446b6u); r[14]<<=2;goto L8c0446b8; /* shll2 r14 */
L8c0446b8: sa_motion_publish_stop(ctx,0x8c0446b8u); r[14]=rd(m,r[14]+r[0],4);goto L8c0446ba; /* mov.l @(r0,r14),r14 */
L8c0446ba: sa_motion_publish_stop(ctx,0x8c0446bau); *sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c0446bc; /* tst r14,r14 */
L8c0446bc: sa_motion_publish_stop(ctx,0x8c0446bcu); if((*sr&1u))goto L8c044740;goto L8c0446be; /* bt 0x8c044740 */
L8c0446be: sa_motion_publish_stop(ctx,0x8c0446beu); r[0]=24u;goto L8c0446c0; /* mov #0x18,r0 */
L8c0446c0: sa_motion_publish_stop(ctx,0x8c0446c0u); if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c0446c2; /* fldi0 fr3 */
L8c0446c2: sa_motion_publish_stop(ctx,0x8c0446c2u); fstore(m,f,3u,r[0]+r[15]);goto L8c0446c4; /* fmov fr3,@(r0,r15) */
L8c0446c4: sa_motion_publish_stop(ctx,0x8c0446c4u); r[0]=16u;goto L8c0446c6; /* mov #0x10,r0 */
L8c0446c6: sa_motion_publish_stop(ctx,0x8c0446c6u); fstore(m,f,3u,r[0]+r[15]);goto L8c0446c8; /* fmov fr3,@(r0,r15) */
L8c0446c8: sa_motion_publish_stop(ctx,0x8c0446c8u); r[0]=0x8c044848u;goto L8c0446ca; /* mova 0x8c044848,r0 */
L8c0446ca: sa_motion_publish_stop(ctx,0x8c0446cau); r[4]=rd(m,0x8c04484cu,4);goto L8c0446cc; /* mov.l 0x8c04484c,r4 */
L8c0446cc: sa_motion_publish_stop(ctx,0x8c0446ccu); r[3]=rd(m,0x8c044850u,4);goto L8c0446ce; /* mov.l 0x8c044850,r3 */
L8c0446ce: sa_motion_publish_stop(ctx,0x8c0446ceu); fload(m,f,3u,r[0]);goto L8c0446d0; /* fmov.s @r0,fr3 */
L8c0446d0: sa_motion_publish_stop(ctx,0x8c0446d0u); r[0]=20u;goto L8c0446d2; /* mov #0x14,r0 */
L8c0446d2: sa_motion_publish_stop(ctx,0x8c0446d4u); target=r[3];*pr=0x8c0446d6u;fstore(m,f,3u,r[0]+r[15]);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0446d6; /* jsr @r3 */
L8c0446d6: sa_motion_publish_stop(ctx,0x8c0446d6u); r[2]=rd(m,r[14]+0x1cu,4);goto L8c0446d8; /* mov.l @(0x1c,r14),r2 */
L8c0446d8: sa_motion_publish_stop(ctx,0x8c0446d8u); *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c0446da; /* tst r2,r2 */
L8c0446da: sa_motion_publish_stop(ctx,0x8c0446dcu); take=(*sr&1u);wr(m,r[15],r[2],4);if(take)goto L8c0446e6;goto L8c0446de; /* bt/s 0x8c0446e6 */
L8c0446de: sa_motion_publish_stop(ctx,0x8c0446deu); r[3]=rd(m,0x8c044854u,4);goto L8c0446e0; /* mov.l 0x8c044854,r3 */
L8c0446e0: sa_motion_publish_stop(ctx,0x8c0446e0u); r[5]=rd(m,r[15],4);goto L8c0446e2; /* mov.l @r15,r5 */
L8c0446e2: sa_motion_publish_stop(ctx,0x8c0446e4u); target=r[3];*pr=0x8c0446e6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0446e6; /* jsr @r3 */
L8c0446e6: sa_motion_publish_stop(ctx,0x8c0446e6u); r[3]=rd(m,r[14]+0x14u,4);goto L8c0446e8; /* mov.l @(0x14,r14),r3 */
L8c0446e8: sa_motion_publish_stop(ctx,0x8c0446e8u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0446ea; /* tst r3,r3 */
L8c0446ea: sa_motion_publish_stop(ctx,0x8c0446ecu); take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto L8c0446f6;goto L8c0446ee; /* bt/s 0x8c0446f6 */
L8c0446ee: sa_motion_publish_stop(ctx,0x8c0446eeu); r[3]=rd(m,0x8c044858u,4);goto L8c0446f0; /* mov.l 0x8c044858,r3 */
L8c0446f0: sa_motion_publish_stop(ctx,0x8c0446f0u); r[5]=rd(m,r[15],4);goto L8c0446f2; /* mov.l @r15,r5 */
L8c0446f2: sa_motion_publish_stop(ctx,0x8c0446f4u); target=r[3];*pr=0x8c0446f6u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0446f6; /* jsr @r3 */
L8c0446f6: sa_motion_publish_stop(ctx,0x8c0446f6u); r[3]=rd(m,r[14]+0x18u,4);goto L8c0446f8; /* mov.l @(0x18,r14),r3 */
L8c0446f8: sa_motion_publish_stop(ctx,0x8c0446f8u); r[3]=0u-r[3];goto L8c0446fa; /* neg r3,r3 */
L8c0446fa: sa_motion_publish_stop(ctx,0x8c0446fau); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c0446fc; /* tst r3,r3 */
L8c0446fc: sa_motion_publish_stop(ctx,0x8c0446feu); take=(*sr&1u);wr(m,r[15],r[3],4);if(take)goto L8c044708;goto L8c044700; /* bt/s 0x8c044708 */
L8c044700: sa_motion_publish_stop(ctx,0x8c044700u); r[1]=rd(m,0x8c04485cu,4);goto L8c044702; /* mov.l 0x8c04485c,r1 */
L8c044702: sa_motion_publish_stop(ctx,0x8c044702u); r[5]=r[3];goto L8c044704; /* mov r3,r5 */
L8c044704: sa_motion_publish_stop(ctx,0x8c044706u); target=r[1];*pr=0x8c044708u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044708; /* jsr @r1 */
L8c044708: sa_motion_publish_stop(ctx,0x8c044708u); r[5]=r[15];goto L8c04470a; /* mov r15,r5 */
L8c04470a: sa_motion_publish_stop(ctx,0x8c04470au); r[2]=rd(m,0x8c044860u,4);goto L8c04470c; /* mov.l 0x8c044860,r2 */
L8c04470c: sa_motion_publish_stop(ctx,0x8c04470cu); r[6]=r[15];goto L8c04470e; /* mov r15,r6 */
L8c04470e: sa_motion_publish_stop(ctx,0x8c04470eu); r[5]+=16u;goto L8c044710; /* add #0x10,r5 */
L8c044710: sa_motion_publish_stop(ctx,0x8c044710u); r[6]+=4u;goto L8c044712; /* add #0x4,r6 */
L8c044712: sa_motion_publish_stop(ctx,0x8c044714u); target=r[2];*pr=0x8c044716u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044716; /* jsr @r2 */
L8c044716: sa_motion_publish_stop(ctx,0x8c044716u); r[3]=rd(m,0x8c044864u,4);goto L8c044718; /* mov.l 0x8c044864,r3 */
L8c044718: sa_motion_publish_stop(ctx,0x8c04471au); target=r[3];*pr=0x8c04471cu;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04471c; /* jsr @r3 */
L8c04471c: sa_motion_publish_stop(ctx,0x8c04471cu); r[0]=4u;goto L8c04471e; /* mov #0x4,r0 */
L8c04471e: sa_motion_publish_stop(ctx,0x8c04471eu); fload(m,f,3u,r[0]+r[15]);goto L8c044720; /* fmov.s @(r0,r15),fr3 */
L8c044720: sa_motion_publish_stop(ctx,0x8c044720u); r[0]=32u;goto L8c044722; /* mov #0x20,r0 */
L8c044722: sa_motion_publish_stop(ctx,0x8c044722u); fload(m,f,2u,r[0]+r[14]);goto L8c044724; /* fmov.s @(r0,r14),fr2 */
L8c044724: sa_motion_publish_stop(ctx,0x8c044724u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c044726; /* fadd fr3,fr2 */
L8c044726: sa_motion_publish_stop(ctx,0x8c044726u); fstore(m,f,2u,r[0]+r[13]);goto L8c044728; /* fmov fr2,@(r0,r13) */
L8c044728: sa_motion_publish_stop(ctx,0x8c044728u); r[0]=8u;goto L8c04472a; /* mov #0x8,r0 */
L8c04472a: sa_motion_publish_stop(ctx,0x8c04472au); fload(m,f,3u,r[0]+r[15]);goto L8c04472c; /* fmov.s @(r0,r15),fr3 */
L8c04472c: sa_motion_publish_stop(ctx,0x8c04472cu); r[0]=36u;goto L8c04472e; /* mov #0x24,r0 */
L8c04472e: sa_motion_publish_stop(ctx,0x8c04472eu); fload(m,f,2u,r[0]+r[14]);goto L8c044730; /* fmov.s @(r0,r14),fr2 */
L8c044730: sa_motion_publish_stop(ctx,0x8c044730u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c044732; /* fadd fr3,fr2 */
L8c044732: sa_motion_publish_stop(ctx,0x8c044732u); fstore(m,f,2u,r[0]+r[13]);goto L8c044734; /* fmov fr2,@(r0,r13) */
L8c044734: sa_motion_publish_stop(ctx,0x8c044734u); r[0]=12u;goto L8c044736; /* mov #0xc,r0 */
L8c044736: sa_motion_publish_stop(ctx,0x8c044736u); fload(m,f,3u,r[0]+r[15]);goto L8c044738; /* fmov.s @(r0,r15),fr3 */
L8c044738: sa_motion_publish_stop(ctx,0x8c044738u); r[0]=40u;goto L8c04473a; /* mov #0x28,r0 */
L8c04473a: sa_motion_publish_stop(ctx,0x8c04473au); fload(m,f,2u,r[0]+r[14]);goto L8c04473c; /* fmov.s @(r0,r14),fr2 */
L8c04473c: sa_motion_publish_stop(ctx,0x8c04473cu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status;}goto L8c04473e; /* fadd fr3,fr2 */
L8c04473e: sa_motion_publish_stop(ctx,0x8c04473eu); fstore(m,f,2u,r[0]+r[13]);goto L8c044740; /* fmov fr2,@(r0,r13) */
L8c044740: sa_motion_publish_stop(ctx,0x8c044740u); r[15]+=28u;goto L8c044742; /* add #0x1c,r15 */
L8c044742: sa_motion_publish_stop(ctx,0x8c044742u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c044744; /* lds.l @r15+,PR */
L8c044744: sa_motion_publish_stop(ctx,0x8c044744u); r[13]=rd(m,r[15],4);r[15]+=4;goto L8c044746; /* mov.l @r15+,r13 */
L8c044746: sa_motion_publish_stop(ctx,0x8c044748u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_spawn_particle_emit_8c04474a(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c04474a;
L8c04474a: sa_motion_publish_stop(ctx,0x8c04474au); r[15]-=4;wr(m,r[15],r[14],4);goto L8c04474c; /* mov.l r14,@-r15 */
L8c04474c: sa_motion_publish_stop(ctx,0x8c04474cu); r[15]-=4;wr(m,r[15],r[13],4);goto L8c04474e; /* mov.l r13,@-r15 */
L8c04474e: sa_motion_publish_stop(ctx,0x8c04474eu); r[15]-=4;wr(m,r[15],r[12],4);goto L8c044750; /* mov.l r12,@-r15 */
L8c044750: sa_motion_publish_stop(ctx,0x8c044750u); r[15]-=4;wr(m,r[15],r[11],4);goto L8c044752; /* mov.l r11,@-r15 */
L8c044752: sa_motion_publish_stop(ctx,0x8c044752u); r[15]-=4;wr(m,r[15],r[10],4);goto L8c044754; /* mov.l r10,@-r15 */
L8c044754: sa_motion_publish_stop(ctx,0x8c044754u); r[15]-=4;wr(m,r[15],r[9],4);goto L8c044756; /* mov.l r9,@-r15 */
L8c044756: sa_motion_publish_stop(ctx,0x8c044756u); r[15]-=4;wr(m,r[15],r[8],4);goto L8c044758; /* mov.l r8,@-r15 */
L8c044758: sa_motion_publish_stop(ctx,0x8c044758u); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c04475a; /* fmov.s fr15,@-r15 */
L8c04475a: sa_motion_publish_stop(ctx,0x8c04475au); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[15]);goto L8c04475c; /* fmov.s fr14,@-r15 */
L8c04475c: sa_motion_publish_stop(ctx,0x8c04475cu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[15]);goto L8c04475e; /* fmov.s fr13,@-r15 */
L8c04475e: sa_motion_publish_stop(ctx,0x8c04475eu); r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[15]);goto L8c044760; /* fmov.s fr12,@-r15 */
L8c044760: sa_motion_publish_stop(ctx,0x8c044760u); r[15]-=4;wr(m,r[15],*pr,4);goto L8c044762; /* sts.l PR,@-r15 */
L8c044762: sa_motion_publish_stop(ctx,0x8c044762u); r[15]+=4294967292u;goto L8c044764; /* add #-0x4,r15 */
L8c044764: sa_motion_publish_stop(ctx,0x8c044764u); r[0]=0x8c044870u;goto L8c044766; /* mova 0x8c044870,r0 */
L8c044766: sa_motion_publish_stop(ctx,0x8c044766u); r[11]=rd(m,0x8c04487cu,4);goto L8c044768; /* mov.l 0x8c04487c,r11 */
L8c044768: sa_motion_publish_stop(ctx,0x8c044768u); r[12]=64u;goto L8c04476a; /* mov #0x40,r12 */
L8c04476a: sa_motion_publish_stop(ctx,0x8c04476au); r[8]=rd(m,0x8c044868u,4);goto L8c04476c; /* mov.l 0x8c044868,r8 */
L8c04476c: sa_motion_publish_stop(ctx,0x8c04476cu); r[12]<<=8;goto L8c04476e; /* shll8 r12 */
L8c04476e: sa_motion_publish_stop(ctx,0x8c04476eu); r[9]=rd(m,0x8c04486cu,4);goto L8c044770; /* mov.l 0x8c04486c,r9 */
L8c044770: sa_motion_publish_stop(ctx,0x8c044770u); r[12]+=4294967295u;goto L8c044772; /* add #-0x1,r12 */
L8c044772: sa_motion_publish_stop(ctx,0x8c044772u); wr(m,r[15],r[4],4);goto L8c044774; /* mov.l r4,@r15 */
L8c044774: sa_motion_publish_stop(ctx,0x8c044774u); fload(m,f,12u,r[0]);goto L8c044776; /* fmov.s @r0,fr12 */
L8c044776: sa_motion_publish_stop(ctx,0x8c044776u); r[0]=0x8c044874u;goto L8c044778; /* mova 0x8c044874,r0 */
L8c044778: sa_motion_publish_stop(ctx,0x8c044778u); fload(m,f,13u,r[0]);goto L8c04477a; /* fmov.s @r0,fr13 */
L8c04477a: sa_motion_publish_stop(ctx,0x8c04477au); r[0]=0x8c044878u;goto L8c04477c; /* mova 0x8c044878,r0 */
L8c04477c: sa_motion_publish_stop(ctx,0x8c04477cu); fload(m,f,14u,r[0]);goto L8c04477e; /* fmov.s @r0,fr14 */
L8c04477e: sa_motion_publish_stop(ctx,0x8c04477eu); r[0]=0x8c044880u;goto L8c044780; /* mova 0x8c044880,r0 */
L8c044780: sa_motion_publish_stop(ctx,0x8c044780u); fload(m,f,15u,r[0]);goto L8c044782; /* fmov.s @r0,fr15 */
L8c044782: sa_motion_publish_stop(ctx,0x8c044782u); r[13]=0u;goto L8c044784; /* mov #0x0,r13 */
L8c044784: sa_motion_publish_stop(ctx,0x8c044784u); r[10]=6u;goto L8c044786; /* mov #0x6,r10 */
L8c044786: sa_motion_publish_stop(ctx,0x8c044786u); r[6]=rd(m,0x8c044884u,4);goto L8c044788; /* mov.l 0x8c044884,r6 */
L8c044788: sa_motion_publish_stop(ctx,0x8c044788u); r[5]=5u;goto L8c04478a; /* mov #0x5,r5 */
L8c04478a: sa_motion_publish_stop(ctx,0x8c04478cu); target=r[9];*pr=0x8c04478eu;r[4]=2u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c04478e; /* jsr @r9 */
L8c04478e: sa_motion_publish_stop(ctx,0x8c04478eu); r[14]=r[0];goto L8c044790; /* mov r0,r14 */
L8c044790: sa_motion_publish_stop(ctx,0x8c044790u); *sr=(*sr&~1u)|!(r[14]&r[14]);goto L8c044792; /* tst r14,r14 */
L8c044792: sa_motion_publish_stop(ctx,0x8c044792u); if((*sr&1u))goto L8c0447fe;goto L8c044794; /* bt 0x8c0447fe */
L8c044794: sa_motion_publish_stop(ctx,0x8c044796u); target=r[8];*pr=0x8c044798u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c044798; /* jsr @r8 */
L8c044798: sa_motion_publish_stop(ctx,0x8c044798u); *fpul=r[0];goto L8c04479a; /* lds r0,FPUL */
L8c04479a: sa_motion_publish_stop(ctx,0x8c04479au); r[2]=rd(m,0x8c044888u,4);goto L8c04479c; /* mov.l 0x8c044888,r2 */
L8c04479c: sa_motion_publish_stop(ctx,0x8c04479cu); status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c04479e; /* float FPUL,fr3 */
L8c04479e: sa_motion_publish_stop(ctx,0x8c04479eu); *fpul=r[2];goto L8c0447a0; /* lds r2,FPUL */
L8c0447a0: sa_motion_publish_stop(ctx,0x8c0447a0u); fcopy(f,4u,3u);goto L8c0447a2; /* fmov fr3,fr4 */
L8c0447a2: sa_motion_publish_stop(ctx,0x8c0447a2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[12],&f->fpscr,&fr[4]);if(status)return status;}goto L8c0447a4; /* fmul fr12,fr4 */
L8c0447a4: sa_motion_publish_stop(ctx,0x8c0447a4u); f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto L8c0447a6; /* fsts FPUL,fr3 */
L8c0447a6: sa_motion_publish_stop(ctx,0x8c0447a6u); fcopy(f,2u,4u);goto L8c0447a8; /* fmov fr4,fr2 */
L8c0447a8: sa_motion_publish_stop(ctx,0x8c0447a8u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[13],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0447aa; /* fmul fr13,fr2 */
L8c0447aa: sa_motion_publish_stop(ctx,0x8c0447aau); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c0447ac; /* fcmp/gt fr2,fr3 */
L8c0447ac: sa_motion_publish_stop(ctx,0x8c0447aeu); take=(*sr&1u);r[4]=rd(m,r[14]+0x20u,4);if(take)goto L8c0447b4;goto L8c0447b0; /* bt/s 0x8c0447b4 */
L8c0447b0: sa_motion_publish_stop(ctx,0x8c0447b0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[3],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c0447b2; /* fadd fr3,fr3 */
L8c0447b2: sa_motion_publish_stop(ctx,0x8c0447b2u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0447b4; /* fsub fr3,fr2 */
L8c0447b4: sa_motion_publish_stop(ctx,0x8c0447b4u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c0447b6; /* ftrc fr2,FPUL */
L8c0447b6: sa_motion_publish_stop(ctx,0x8c0447b6u); r[2]=rd(m,0x8c044888u,4);goto L8c0447b8; /* mov.l 0x8c044888,r2 */
L8c0447b8: sa_motion_publish_stop(ctx,0x8c0447b8u); fcopy(f,2u,4u);goto L8c0447ba; /* fmov fr4,fr2 */
L8c0447ba: sa_motion_publish_stop(ctx,0x8c0447bau); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[2],&fr[14],&f->fpscr,&fr[2]);if(status)return status;}goto L8c0447bc; /* fmul fr14,fr2 */
L8c0447bc: sa_motion_publish_stop(ctx,0x8c0447bcu); r[3]=*fpul;goto L8c0447be; /* sts FPUL,r3 */
L8c0447be: sa_motion_publish_stop(ctx,0x8c0447beu); *fpul=r[2];goto L8c0447c0; /* lds r2,FPUL */
L8c0447c0: sa_motion_publish_stop(ctx,0x8c0447c0u); r[3]<<=8;goto L8c0447c2; /* shll8 r3 */
L8c0447c2: sa_motion_publish_stop(ctx,0x8c0447c2u); f->bank[(f->fpscr>>21)&1u][3]=*fpul;goto L8c0447c4; /* fsts FPUL,fr3 */
L8c0447c4: sa_motion_publish_stop(ctx,0x8c0447c4u); r[3]<<=2;goto L8c0447c6; /* shll2 r3 */
L8c0447c6: sa_motion_publish_stop(ctx,0x8c0447c6u); *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1;goto L8c0447c8; /* shll r3 */
L8c0447c8: sa_motion_publish_stop(ctx,0x8c0447c8u); status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][3],f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,sr,1);if(status)return status;goto L8c0447ca; /* fcmp/gt fr2,fr3 */
L8c0447ca: sa_motion_publish_stop(ctx,0x8c0447cau); wr(m,r[4]+0x18u,r[3],4);goto L8c0447cc; /* mov.l r3,@(0x18,r4) */
L8c0447cc: sa_motion_publish_stop(ctx,0x8c0447ccu); if((*sr&1u))goto L8c0447d2;goto L8c0447ce; /* bt 0x8c0447d2 */
L8c0447ce: sa_motion_publish_stop(ctx,0x8c0447ceu); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[3],&fr[3],&f->fpscr,&fr[3],0);if(status)return status;}goto L8c0447d0; /* fadd fr3,fr3 */
L8c0447d0: sa_motion_publish_stop(ctx,0x8c0447d0u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],1);if(status)return status;}goto L8c0447d2; /* fsub fr3,fr2 */
L8c0447d2: sa_motion_publish_stop(ctx,0x8c0447d2u); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][2],&f->fpscr,fpul);if(status)return status;goto L8c0447d4; /* ftrc fr2,FPUL */
L8c0447d4: sa_motion_publish_stop(ctx,0x8c0447d4u); fcopy(f,3u,4u);goto L8c0447d6; /* fmov fr4,fr3 */
L8c0447d6: sa_motion_publish_stop(ctx,0x8c0447d6u); {uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0447d8; /* fmul fr15,fr3 */
L8c0447d8: sa_motion_publish_stop(ctx,0x8c0447d8u); r[13]+=1u;goto L8c0447da; /* add #0x1,r13 */
L8c0447da: sa_motion_publish_stop(ctx,0x8c0447dau); *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[10]);goto L8c0447dc; /* cmp/ge r10,r13 */
L8c0447dc: sa_motion_publish_stop(ctx,0x8c0447dcu); r[3]=*fpul;goto L8c0447de; /* sts FPUL,r3 */
L8c0447de: sa_motion_publish_stop(ctx,0x8c0447deu); status=sa_sh4_truncate_i32(f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,fpul);if(status)return status;goto L8c0447e0; /* ftrc fr3,FPUL */
L8c0447e0: sa_motion_publish_stop(ctx,0x8c0447e0u); r[3]&=r[12];goto L8c0447e2; /* and r12,r3 */
L8c0447e2: sa_motion_publish_stop(ctx,0x8c0447e2u); r[2]=0u-r[3];goto L8c0447e4; /* neg r3,r2 */
L8c0447e4: sa_motion_publish_stop(ctx,0x8c0447e4u); r[2]+=r[11];goto L8c0447e6; /* add r11,r2 */
L8c0447e6: sa_motion_publish_stop(ctx,0x8c0447e6u); r[0]=*fpul;goto L8c0447e8; /* sts FPUL,r0 */
L8c0447e8: sa_motion_publish_stop(ctx,0x8c0447e8u); wr(m,r[4]+0x14u,r[2],4);goto L8c0447ea; /* mov.l r2,@(0x14,r4) */
L8c0447ea: sa_motion_publish_stop(ctx,0x8c0447eau); r[0]&=0x1u;goto L8c0447ec; /* and #0x1,r0 */
L8c0447ec: sa_motion_publish_stop(ctx,0x8c0447ecu); wr(m,r[4]+0x3u,r[0],1);goto L8c0447ee; /* mov.b r0,@(0x3,r4) */
L8c0447ee: sa_motion_publish_stop(ctx,0x8c0447eeu); r[3]=rd(m,r[15],4);goto L8c0447f0; /* mov.l @r15,r3 */
L8c0447f0: sa_motion_publish_stop(ctx,0x8c0447f0u); r[2]=rd(m,r[3]+0x20u,4);goto L8c0447f2; /* mov.l @(0x20,r3),r2 */
L8c0447f2: sa_motion_publish_stop(ctx,0x8c0447f2u); r[1]=rd(m,r[3]+0x24u,4);goto L8c0447f4; /* mov.l @(0x24,r3),r1 */
L8c0447f4: sa_motion_publish_stop(ctx,0x8c0447f4u); wr(m,r[4]+0x20u,r[2],4);goto L8c0447f6; /* mov.l r2,@(0x20,r4) */
L8c0447f6: sa_motion_publish_stop(ctx,0x8c0447f6u); wr(m,r[4]+0x24u,r[1],4);goto L8c0447f8; /* mov.l r1,@(0x24,r4) */
L8c0447f8: sa_motion_publish_stop(ctx,0x8c0447f8u); r[2]=rd(m,r[3]+0x28u,4);goto L8c0447fa; /* mov.l @(0x28,r3),r2 */
L8c0447fa: sa_motion_publish_stop(ctx,0x8c0447fcu); take=!(*sr&1u);wr(m,r[4]+0x28u,r[2],4);if(take)goto L8c044786;goto L8c0447fe; /* bf/s 0x8c044786 */
L8c0447fe: sa_motion_publish_stop(ctx,0x8c0447feu); r[15]+=4u;goto L8c044800; /* add #0x4,r15 */
L8c044800: sa_motion_publish_stop(ctx,0x8c044800u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c044802; /* lds.l @r15+,PR */
L8c044802: sa_motion_publish_stop(ctx,0x8c044802u); fload(m,f,12u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c044804; /* fmov.s @r15+,fr12 */
L8c044804: sa_motion_publish_stop(ctx,0x8c044804u); fload(m,f,13u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c044806; /* fmov.s @r15+,fr13 */
L8c044806: sa_motion_publish_stop(ctx,0x8c044806u); fload(m,f,14u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c044808; /* fmov.s @r15+,fr14 */
L8c044808: sa_motion_publish_stop(ctx,0x8c044808u); fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c04480a; /* fmov.s @r15+,fr15 */
L8c04480a: sa_motion_publish_stop(ctx,0x8c04480au); r[8]=rd(m,r[15],4);r[15]+=4;goto L8c04480c; /* mov.l @r15+,r8 */
L8c04480c: sa_motion_publish_stop(ctx,0x8c04480cu); r[9]=rd(m,r[15],4);r[15]+=4;goto L8c04480e; /* mov.l @r15+,r9 */
L8c04480e: sa_motion_publish_stop(ctx,0x8c04480eu); r[10]=rd(m,r[15],4);r[15]+=4;goto L8c044810; /* mov.l @r15+,r10 */
L8c044810: sa_motion_publish_stop(ctx,0x8c044810u); r[11]=rd(m,r[15],4);r[15]+=4;goto L8c044812; /* mov.l @r15+,r11 */
L8c044812: sa_motion_publish_stop(ctx,0x8c044812u); r[12]=rd(m,r[15],4);r[15]+=4;goto L8c044814; /* mov.l @r15+,r12 */
L8c044814: sa_motion_publish_stop(ctx,0x8c044814u); r[13]=rd(m,r[15],4);r[15]+=4;goto L8c044816; /* mov.l @r15+,r13 */
L8c044816: sa_motion_publish_stop(ctx,0x8c044818u); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
