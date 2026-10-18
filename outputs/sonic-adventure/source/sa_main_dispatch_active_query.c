/* Generated from retail instruction exports by recover_main_dispatch_active_query.py.
 * Inferred descriptive names; static C, not an opcode interpreter. All game
 * callees in this three-routine group are native; observer is optional host
 * instrumentation. RAM is a valid 16 MiB guest image. No MMIO/memory exceptions.
 * Host 0 = complete, retail return in R0. Floating support can return -1 for
 * unsupported PR/RM or 1 for an enabled exception, preserving partial state.
 * Integer-only paths retain their original behavior for all FPU modes.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_float_i32.h"
#include "sh4_compare_bits.h"
typedef void (*sa_active_observer)(void *,uint32_t,const uint8_t *,const struct sa_fpu_banks *,const uint32_t *,const uint32_t *,const uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static void fload(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;f->bank[bank][reg]=rd(m,a,4);f->bank[bank][reg+1]=rd(m,a+4u,4);}
 else f->bank[bank][reg]=rd(m,a,4);
}
SA_STAGE_EXPORT int sa_main_dispatch_clear_table_8c05e69e(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_active_observer);
SA_STAGE_EXPORT int sa_main_dispatch_active_predicate_8c05e6c4(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_active_observer);
SA_STAGE_EXPORT int sa_main_dispatch_active_query_8c05e6e8(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *,void *,sa_active_observer);
SA_STAGE_EXPORT int sa_main_dispatch_clear_table_8c05e69e(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,sa_active_observer observe){
 (void)f;(void)pr;(void)fpul;(void)macl;(void)ctx;(void)observe;
 goto L8c05e69e;
L8c05e69e: r[7]=rd(m,0x8c05e760u,4);goto L8c05e6a0; /* mov.l 0x8c05e760,r7 */
L8c05e6a0: r[6]=r[7];goto L8c05e6a2; /* mov r7,r6 */
L8c05e6a2: r[6]+=40u;goto L8c05e6a4; /* add #0x28,r6 */
L8c05e6a4: r[4]=r[7];goto L8c05e6a6; /* mov r7,r4 */
L8c05e6a6: r[5]=r[7];goto L8c05e6a8; /* mov r7,r5 */
L8c05e6a8: r[7]=0u;goto L8c05e6bc; /* bra 0x8c05e6bc */
L8c05e6ac: r[2]=rd(m,r[4],4);goto L8c05e6ae; /* mov.l @r4,r2 */
L8c05e6ae: *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c05e6b0; /* tst r2,r2 */
L8c05e6b0: if((*sr&1u))goto L8c05e6b8;goto L8c05e6b2; /* bt 0x8c05e6b8 */
L8c05e6b2: r[2]=rd(m,r[5],4);goto L8c05e6b4; /* mov.l @r5,r2 */
L8c05e6b4: r[3]=rd(m,r[2]+0x20u,4);goto L8c05e6b6; /* mov.l @(0x20,r2),r3 */
L8c05e6b6: wr(m,r[3],r[7],1);goto L8c05e6b8; /* mov.b r7,@r3 */
L8c05e6b8: r[4]+=4u;goto L8c05e6ba; /* add #0x4,r4 */
L8c05e6ba: r[5]+=4u;goto L8c05e6bc; /* add #0x4,r5 */
L8c05e6bc: *sr=(*sr&~1u)|(r[4]>=r[6]);goto L8c05e6be; /* cmp/hs r6,r4 */
L8c05e6be: if(!(*sr&1u))goto L8c05e6ac;goto L8c05e6c0; /* bf 0x8c05e6ac */
L8c05e6c0: return 0; /* rts */
}

SA_STAGE_EXPORT int sa_main_dispatch_active_predicate_8c05e6c4(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,sa_active_observer observe){
 (void)f;(void)pr;(void)fpul;(void)macl;(void)ctx;(void)observe;
 goto L8c05e6c4;
L8c05e6c4: r[5]=rd(m,r[4]+0x38u,4);goto L8c05e6c6; /* mov.l @(0x38,r4),r5 */
L8c05e6c6: r[5]=rd(m,r[5]+0x18u,4);goto L8c05e6c8; /* mov.l @(0x18,r5),r5 */
L8c05e6c8: *sr=(*sr&~1u)|!(r[5]&r[5]);goto L8c05e6ca; /* tst r5,r5 */
L8c05e6ca: if((*sr&1u))goto L8c05e6de;goto L8c05e6cc; /* bt 0x8c05e6de */
L8c05e6cc: r[0]=sx(m,r[4]+0x8u,1);goto L8c05e6ce; /* mov.b @(0x8,r4),r0 */
L8c05e6ce: *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c05e6d0; /* tst r0,r0 */
L8c05e6d0: if(!(*sr&1u))goto L8c05e6de;goto L8c05e6d2; /* bf 0x8c05e6de */
L8c05e6d2: r[4]=rd(m,r[5]+0x10u,4);goto L8c05e6d4; /* mov.l @(0x10,r5),r4 */
L8c05e6d4: r[4]=rd(m,r[4]+0x20u,4);goto L8c05e6d6; /* mov.l @(0x20,r4),r4 */
L8c05e6d6: r[0]=sx(m,r[4]+0x2u,1);goto L8c05e6d8; /* mov.b @(0x2,r4),r0 */
L8c05e6d8: r[0]=r[0]&255u;goto L8c05e6da; /* extu.b r0,r0 */
L8c05e6da: *sr=(*sr&~1u)|(r[0]==0x00000008u);goto L8c05e6dc; /* cmp/eq #0x8,r0 */
L8c05e6dc: if((*sr&1u))goto L8c05e6e2;goto L8c05e6de; /* bt 0x8c05e6e2 */
L8c05e6de: r[0]=0u;return 0; /* rts */
L8c05e6e2: r[0]=1u;goto L8c05e6e4; /* mov #0x1,r0 */
L8c05e6e4: return 0; /* rts */
}

SA_STAGE_EXPORT int sa_main_dispatch_active_query_8c05e6e8(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,sa_active_observer observe){
 (void)f;(void)pr;(void)fpul;(void)macl;(void)ctx;(void)observe;
 int status;
 goto L8c05e6e8;
L8c05e6e8: r[15]-=4;wr(m,r[15],r[14],4);goto L8c05e6ea; /* mov.l r14,@-r15 */
L8c05e6ea: r[15]-=4;wr(m,r[15],*pr,4);goto L8c05e6ec; /* sts.l PR,@-r15 */
L8c05e6ec: *pr=0x8c05e6f0u;r[14]=r[4];if(observe)observe(ctx,0x8c05e6c4u,m,f,sr,r,pr);status=sa_main_dispatch_active_predicate_8c05e6c4(m,f,sr,r,pr,fpul,macl,ctx,observe);if(status)return status;goto L8c05e6f0; /* bsr 0x8c05e6c4 */
L8c05e6f0: *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c05e6f2; /* tst r0,r0 */
L8c05e6f2: if(!(*sr&1u))goto L8c05e702;goto L8c05e6f4; /* bf 0x8c05e702 */
L8c05e6f4: r[0]=sx(m,r[14]+0x8u,1);goto L8c05e6f6; /* mov.b @(0x8,r14),r0 */
L8c05e6f6: *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c05e6f8; /* tst r0,r0 */
L8c05e6f8: if(!(*sr&1u))goto L8c05e73e;goto L8c05e6fa; /* bf 0x8c05e73e */
L8c05e6fa: *pr=0x8c05e6feu;if(observe)observe(ctx,0x8c05e69eu,m,f,sr,r,pr);status=sa_main_dispatch_clear_table_8c05e69e(m,f,sr,r,pr,fpul,macl,ctx,observe);if(status)return status;goto L8c05e6fe; /* bsr 0x8c05e69e */
L8c05e6fe: goto L8c05e73e; /* bra 0x8c05e73e */
L8c05e702: r[4]=rd(m,r[14]+0x38u,4);goto L8c05e704; /* mov.l @(0x38,r14),r4 */
L8c05e704: r[4]=rd(m,r[4]+0x18u,4);goto L8c05e706; /* mov.l @(0x18,r4),r4 */
L8c05e706: r[3]=rd(m,r[4]+0x10u,4);goto L8c05e708; /* mov.l @(0x10,r4),r3 */
L8c05e708: r[4]=rd(m,r[3]+0x20u,4);goto L8c05e70a; /* mov.l @(0x20,r3),r4 */
L8c05e70a: r[0]=sx(m,r[4]+0x6u,2);goto L8c05e70c; /* mov.w @(0x6,r4),r0 */
L8c05e70c: *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c05e70e; /* tst r0,r0 */
L8c05e70e: if(!(*sr&1u))goto L8c05e71c;goto L8c05e710; /* bf 0x8c05e71c */
L8c05e710: *pr=0x8c05e714u;if(observe)observe(ctx,0x8c05e69eu,m,f,sr,r,pr);status=sa_main_dispatch_clear_table_8c05e69e(m,f,sr,r,pr,fpul,macl,ctx,observe);if(status)return status;goto L8c05e714; /* bsr 0x8c05e69e */
L8c05e714: *pr=rd(m,r[15],4);r[15]+=4;goto L8c05e716; /* lds.l @r15+,PR */
L8c05e716: r[0]=4294967295u;goto L8c05e718; /* mov #-0x1,r0 */
L8c05e718: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c05e71c: r[0]=rd(m,r[14]+0x38u,4);goto L8c05e71e; /* mov.l @(0x38,r14),r0 */
L8c05e71e: r[0]=sx(m,r[0]+0x16u,2);goto L8c05e720; /* mov.w @(0x16,r0),r0 */
L8c05e720: *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c05e722; /* cmp/eq #0x1,r0 */
L8c05e722: if((*sr&1u))goto L8c05e72c;goto L8c05e724; /* bt 0x8c05e72c */
L8c05e724: *pr=0x8c05e728u;if(observe)observe(ctx,0x8c05e69eu,m,f,sr,r,pr);status=sa_main_dispatch_clear_table_8c05e69e(m,f,sr,r,pr,fpul,macl,ctx,observe);if(status)return status;goto L8c05e728; /* bsr 0x8c05e69e */
L8c05e728: goto L8c05e73e; /* bra 0x8c05e73e */
L8c05e72c: r[3]=rd(m,r[4]+0xcu,4);goto L8c05e72e; /* mov.l @(0xc,r4),r3 */
L8c05e72e: r[0]=0x8c05e764u;goto L8c05e730; /* mova 0x8c05e764,r0 */
L8c05e730: fload(m,f,2u,r[0]);goto L8c05e732; /* fmov.s @r0,fr2 */
L8c05e732: *fpul=r[3];goto L8c05e734; /* lds r3,FPUL */
L8c05e734: status=sa_sh4_float_i32(*fpul,&f->fpscr,&f->bank[(f->fpscr>>21)&1u][3]);if(status)return status;goto L8c05e736; /* float FPUL,fr3 */
L8c05e736: status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][2],f->bank[(f->fpscr>>21)&1u][3],&f->fpscr,sr,1);if(status)return status;goto L8c05e738; /* fcmp/gt fr3,fr2 */
L8c05e738: if((*sr&1u))goto L8c05e780;goto L8c05e73a; /* bt 0x8c05e780 */
L8c05e73a: *pr=0x8c05e73eu;if(observe)observe(ctx,0x8c05e69eu,m,f,sr,r,pr);status=sa_main_dispatch_clear_table_8c05e69e(m,f,sr,r,pr,fpul,macl,ctx,observe);if(status)return status;goto L8c05e73e; /* bsr 0x8c05e69e */
L8c05e73e: *pr=rd(m,r[15],4);r[15]+=4;goto L8c05e740; /* lds.l @r15+,PR */
L8c05e740: r[0]=0u;goto L8c05e742; /* mov #0x0,r0 */
L8c05e742: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c05e780: r[0]=1u;goto L8c05e782; /* mov #0x1,r0 */
L8c05e782: *pr=rd(m,r[15],4);r[15]+=4;goto L8c05e784; /* lds.l @r15+,PR */
L8c05e784: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
