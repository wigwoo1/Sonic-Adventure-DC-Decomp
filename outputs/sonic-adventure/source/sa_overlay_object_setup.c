/* Static C generated from retail instruction exports by
 * recover_overlay_object_setup.py. Descriptive symbols are inferred.
 * No runtime opcode decoding. Guest RAM is 16 MiB; invalid pointers, hardware
 * exceptions and MMIO are not modeled. Native selector integration and other explicit callees use the bridge.
 * Host 0 means complete; the retail result is in R0. Missing call service
 * returns -2 atomically. A service error returns its status with partial state.
 * These routines use integer instructions, guarded FLDI0 and raw FMOV;
 * PR=1 stops at the original FLDI0 before its effects.
 */
#include "fpu_cleanup_transfer.h"
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
SA_STAGE_EXPORT int sa_overlay_object_preorder_walk_0c9044a0(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)sr;(void)fload;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc9044a0;
Lc9044a0: r[15]-=4;wr(m,r[15],r[14],4);goto Lc9044a2; /* mov.l r14,@-r15 */
Lc9044a2: r[0]=r[6];goto Lc9044a4; /* mov r6,r0 */
Lc9044a4: r[15]-=4;wr(m,r[15],r[13],4);goto Lc9044a6; /* mov.l r13,@-r15 */
Lc9044a6: r[13]=r[4];goto Lc9044a8; /* mov r4,r13 */
Lc9044a8: r[15]-=4;wr(m,r[15],r[12],4);goto Lc9044aa; /* mov.l r12,@-r15 */
Lc9044aa: r[0]<<=2;goto Lc9044ac; /* shll2 r0 */
Lc9044ac: r[15]-=4;wr(m,r[15],*pr,4);goto Lc9044ae; /* sts.l PR,@-r15 */
Lc9044ae: r[12]=r[5];goto Lc9044b0; /* mov r5,r12 */
Lc9044b0: r[14]=r[6];goto Lc9044b2; /* mov r6,r14 */
Lc9044b2: wr(m,r[12]+r[0],r[4],4);goto Lc9044b4; /* mov.l r4,@(r0,r12) */
Lc9044b4: if((r[13]&0x1f000000u)!=0x0c000000u)return -3;r[3]=rd(m,r[13]+0x2cu,4);goto Lc9044b6; /* mov.l @(0x2c,r13),r3 */
Lc9044b6: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc9044b8; /* tst r3,r3 */
Lc9044b8: take=(*sr&1u);r[14]+=1u;if(take)goto Lc9044c6;goto Lc9044bc; /* bt/s 0x0c9044c6 */
Lc9044bc: r[5]=r[12];goto Lc9044be; /* mov r12,r5 */
Lc9044be: r[6]=r[14];goto Lc9044c0; /* mov r14,r6 */
Lc9044c0: target=0x0c9044a0u;*pr=0x0c9044c4u;r[4]=rd(m,r[13]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9044c4; /* bsr 0x0c9044a0 */
Lc9044c4: r[14]=r[0];goto Lc9044c6; /* mov r0,r14 */
Lc9044c6: r[2]=rd(m,r[13]+0x30u,4);goto Lc9044c8; /* mov.l @(0x30,r13),r2 */
Lc9044c8: *sr=(*sr&~1u)|!(r[2]&r[2]);goto Lc9044ca; /* tst r2,r2 */
Lc9044ca: if((*sr&1u))goto Lc9044d6;goto Lc9044cc; /* bt 0x0c9044d6 */
Lc9044cc: r[5]=r[12];goto Lc9044ce; /* mov r12,r5 */
Lc9044ce: r[6]=r[14];goto Lc9044d0; /* mov r14,r6 */
Lc9044d0: target=0x0c9044a0u;*pr=0x0c9044d4u;r[4]=rd(m,r[13]+0x30u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9044d4; /* bsr 0x0c9044a0 */
Lc9044d4: r[14]=r[0];goto Lc9044d6; /* mov r0,r14 */
Lc9044d6: *pr=rd(m,r[15],4);r[15]+=4;goto Lc9044d8; /* lds.l @r15+,PR */
Lc9044d8: r[0]=r[14];goto Lc9044da; /* mov r14,r0 */
Lc9044da: r[12]=rd(m,r[15],4);r[15]+=4;goto Lc9044dc; /* mov.l @r15+,r12 */
Lc9044dc: r[13]=rd(m,r[15],4);r[15]+=4;goto Lc9044de; /* mov.l @r15+,r13 */
Lc9044de: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_object_root_walk_0c9044e2(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)sr;(void)fload;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc9044e2;
Lc9044e2: r[15]-=4;wr(m,r[15],r[14],4);goto Lc9044e4; /* mov.l r14,@-r15 */
Lc9044e4: r[14]=0u;goto Lc9044e6; /* mov #0x0,r14 */
Lc9044e6: r[15]-=4;wr(m,r[15],r[13],4);goto Lc9044e8; /* mov.l r13,@-r15 */
Lc9044e8: r[13]=r[4];goto Lc9044ea; /* mov r4,r13 */
Lc9044ea: r[15]-=4;wr(m,r[15],r[12],4);goto Lc9044ec; /* mov.l r12,@-r15 */
Lc9044ec: r[12]=r[5];goto Lc9044ee; /* mov r5,r12 */
Lc9044ee: r[15]-=4;wr(m,r[15],*pr,4);goto Lc9044f0; /* sts.l PR,@-r15 */
Lc9044f0: wr(m,r[12],r[4],4);goto Lc9044f2; /* mov.l r4,@r12 */
Lc9044f2: if((r[13]&0x1f000000u)!=0x0c000000u)return -3;r[3]=rd(m,r[13]+0x2cu,4);goto Lc9044f4; /* mov.l @(0x2c,r13),r3 */
Lc9044f4: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc9044f6; /* tst r3,r3 */
Lc9044f6: take=(*sr&1u);r[14]+=1u;if(take)goto Lc904504;goto Lc9044fa; /* bt/s 0x0c904504 */
Lc9044fa: r[5]=r[12];goto Lc9044fc; /* mov r12,r5 */
Lc9044fc: r[6]=r[14];goto Lc9044fe; /* mov r14,r6 */
Lc9044fe: target=0x0c9044a0u;*pr=0x0c904502u;r[4]=rd(m,r[13]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904502; /* bsr 0x0c9044a0 */
Lc904502: r[14]=r[0];goto Lc904504; /* mov r0,r14 */
Lc904504: r[2]=rd(m,r[13]+0x30u,4);goto Lc904506; /* mov.l @(0x30,r13),r2 */
Lc904506: *sr=(*sr&~1u)|!(r[2]&r[2]);goto Lc904508; /* tst r2,r2 */
Lc904508: if((*sr&1u))goto Lc904512;goto Lc90450a; /* bt 0x0c904512 */
Lc90450a: r[5]=r[12];goto Lc90450c; /* mov r12,r5 */
Lc90450c: r[6]=r[14];goto Lc90450e; /* mov r14,r6 */
Lc90450e: target=0x0c9044a0u;*pr=0x0c904512u;r[4]=rd(m,r[13]+0x30u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904512; /* bsr 0x0c9044a0 */
Lc904512: *pr=rd(m,r[15],4);r[15]+=4;goto Lc904514; /* lds.l @r15+,PR */
Lc904514: r[12]=rd(m,r[15],4);r[15]+=4;goto Lc904516; /* mov.l @r15+,r12 */
Lc904516: r[13]=rd(m,r[15],4);r[15]+=4;goto Lc904518; /* mov.l @r15+,r13 */
Lc904518: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_object_clone_0c90451c(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)sr;(void)fload;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc90451c;
Lc90451c: r[15]-=4;wr(m,r[15],r[14],4);goto Lc90451e; /* mov.l r14,@-r15 */
Lc90451e: r[15]-=4;wr(m,r[15],r[13],4);goto Lc904520; /* mov.l r13,@-r15 */
Lc904520: r[15]-=4;wr(m,r[15],r[12],4);goto Lc904522; /* mov.l r12,@-r15 */
Lc904522: r[15]-=4;wr(m,r[15],r[11],4);goto Lc904524; /* mov.l r11,@-r15 */
Lc904524: r[15]-=4;wr(m,r[15],r[10],4);goto Lc904526; /* mov.l r10,@-r15 */
Lc904526: r[15]-=4;wr(m,r[15],r[9],4);goto Lc904528; /* mov.l r9,@-r15 */
Lc904528: r[15]-=4;wr(m,r[15],r[8],4);goto Lc90452a; /* mov.l r8,@-r15 */
Lc90452a: r[15]-=4;wr(m,r[15],*pr,4);goto Lc90452c; /* sts.l PR,@-r15 */
Lc90452c: r[15]+=4294967292u;goto Lc90452e; /* add #-0x4,r15 */
Lc90452e: r[10]=r[4];goto Lc904530; /* mov r4,r10 */
Lc904530: *sr=(*sr&~1u)|!(r[10]&r[10]);goto Lc904532; /* tst r10,r10 */
Lc904532: take=!(*sr&1u);if((r[10]&0x1f000000u)!=0x0c000000u)return -3;r[12]=rd(m,r[10]+0x4u,4);if(take)goto Lc90453a;goto Lc904536; /* bf/s 0x0c90453a */
Lc904536: goto Lc904748; /* bra 0x0c904748 */
Lc90453a: r[8]=rd(m,0x0c904640u,4);goto Lc90453c; /* mov.l 0x0c904640,r8 */
Lc90453c: target=r[8];*pr=0x0c904540u;r[4]=52u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904540; /* jsr @r8 */
Lc904540: *sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc904542; /* tst r0,r0 */
Lc904542: take=!(*sr&1u);r[11]=r[0];if(take)goto Lc90454a;goto Lc904546; /* bf/s 0x0c90454a */
Lc904546: goto Lc90472e; /* bra 0x0c90472e */
Lc90454a: r[9]=rd(m,0x0c904644u,4);goto Lc90454c; /* mov.l 0x0c904644,r9 */
Lc90454c: r[5]=r[10];goto Lc90454e; /* mov r10,r5 */
Lc90454e: r[6]=52u;goto Lc904550; /* mov #0x34,r6 */
Lc904550: target=r[9];*pr=0x0c904554u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904554; /* jsr @r9 */
Lc904554: *sr=(*sr&~1u)|!(r[12]&r[12]);goto Lc904556; /* tst r12,r12 */
Lc904556: if(!(*sr&1u))goto Lc90455c;goto Lc904558; /* bf 0x0c90455c */
Lc904558: goto Lc904748; /* bra 0x0c904748 */
Lc90455c: r[13]=rd(m,0x0c904648u,4);goto Lc90455e; /* mov.l 0x0c904648,r13 */
Lc90455e: target=r[8];*pr=0x0c904562u;r[4]=40u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904562; /* jsr @r8 */
Lc904562: *sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc904564; /* tst r0,r0 */
Lc904564: take=!(*sr&1u);r[14]=r[0];if(take)goto Lc904570;goto Lc904568; /* bf/s 0x0c904570 */
Lc904568: target=r[13];*pr=0x0c90456cu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90456c; /* jsr @r13 */
Lc90456c: goto Lc90472e; /* bra 0x0c90472e */
Lc904570: r[5]=r[12];goto Lc904572; /* mov r12,r5 */
Lc904572: r[6]=40u;goto Lc904574; /* mov #0x28,r6 */
Lc904574: target=r[9];*pr=0x0c904578u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904578; /* jsr @r9 */
Lc904578: wr(m,r[11]+0x4u,r[14],4);goto Lc90457a; /* mov.l r14,@(0x4,r11) */
Lc90457a: r[3]=rd(m,r[12],4);goto Lc90457c; /* mov.l @r12,r3 */
Lc90457c: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc90457e; /* tst r3,r3 */
Lc90457e: if((*sr&1u))goto Lc9045ae;goto Lc904580; /* bt 0x0c9045ae */
Lc904580: r[4]=rd(m,r[12]+0x8u,4);goto Lc904582; /* mov.l @(0x8,r12),r4 */
Lc904582: r[3]=r[4];goto Lc904584; /* mov r4,r3 */
Lc904584: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto Lc904586; /* shll r4 */
Lc904586: r[4]+=r[3];goto Lc904588; /* add r3,r4 */
Lc904588: target=r[8];*pr=0x0c90458cu;r[4]<<=2;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90458c; /* jsr @r8 */
Lc90458c: *sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc90458e; /* tst r0,r0 */
Lc90458e: take=!(*sr&1u);wr(m,r[14],r[0],4);if(take)goto Lc90459e;goto Lc904592; /* bf/s 0x0c90459e */
Lc904592: target=r[13];*pr=0x0c904596u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904596; /* jsr @r13 */
Lc904596: target=r[13];*pr=0x0c90459au;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90459a; /* jsr @r13 */
Lc90459a: goto Lc90472e; /* bra 0x0c90472e */
Lc90459e: r[6]=rd(m,r[12]+0x8u,4);goto Lc9045a0; /* mov.l @(0x8,r12),r6 */
Lc9045a0: r[5]=rd(m,r[12],4);goto Lc9045a2; /* mov.l @r12,r5 */
Lc9045a2: r[3]=r[6];goto Lc9045a4; /* mov r6,r3 */
Lc9045a4: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto Lc9045a6; /* shll r6 */
Lc9045a6: r[6]+=r[3];goto Lc9045a8; /* add r3,r6 */
Lc9045a8: r[6]<<=2;goto Lc9045aa; /* shll2 r6 */
Lc9045aa: target=r[9];*pr=0x0c9045aeu;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9045ae; /* jsr @r9 */
Lc9045ae: r[2]=rd(m,r[12]+0x4u,4);goto Lc9045b0; /* mov.l @(0x4,r12),r2 */
Lc9045b0: *sr=(*sr&~1u)|!(r[2]&r[2]);goto Lc9045b2; /* tst r2,r2 */
Lc9045b2: if((*sr&1u))goto Lc9045ec;goto Lc9045b4; /* bt 0x0c9045ec */
Lc9045b4: r[4]=rd(m,r[12]+0x8u,4);goto Lc9045b6; /* mov.l @(0x8,r12),r4 */
Lc9045b6: r[3]=r[4];goto Lc9045b8; /* mov r4,r3 */
Lc9045b8: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1;goto Lc9045ba; /* shll r4 */
Lc9045ba: r[4]+=r[3];goto Lc9045bc; /* add r3,r4 */
Lc9045bc: target=r[8];*pr=0x0c9045c0u;r[4]<<=2;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9045c0; /* jsr @r8 */
Lc9045c0: *sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc9045c2; /* tst r0,r0 */
Lc9045c2: take=!(*sr&1u);wr(m,r[14]+0x4u,r[0],4);if(take)goto Lc9045dc;goto Lc9045c6; /* bf/s 0x0c9045dc */
Lc9045c6: r[3]=rd(m,r[14],4);goto Lc9045c8; /* mov.l @r14,r3 */
Lc9045c8: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc9045ca; /* tst r3,r3 */
Lc9045ca: if((*sr&1u))goto Lc9045d0;goto Lc9045cc; /* bt 0x0c9045d0 */
Lc9045cc: target=r[13];*pr=0x0c9045d0u;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9045d0; /* jsr @r13 */
Lc9045d0: target=r[13];*pr=0x0c9045d4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9045d4; /* jsr @r13 */
Lc9045d4: target=r[13];*pr=0x0c9045d8u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9045d8; /* jsr @r13 */
Lc9045d8: goto Lc90472e; /* bra 0x0c90472e */
Lc9045dc: r[6]=rd(m,r[12]+0x8u,4);goto Lc9045de; /* mov.l @(0x8,r12),r6 */
Lc9045de: r[5]=rd(m,r[12]+0x4u,4);goto Lc9045e0; /* mov.l @(0x4,r12),r5 */
Lc9045e0: r[3]=r[6];goto Lc9045e2; /* mov r6,r3 */
Lc9045e2: *sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto Lc9045e4; /* shll r6 */
Lc9045e4: r[6]+=r[3];goto Lc9045e6; /* add r3,r6 */
Lc9045e6: r[6]<<=2;goto Lc9045e8; /* shll2 r6 */
Lc9045e8: target=r[9];*pr=0x0c9045ecu;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9045ec; /* jsr @r9 */
Lc9045ec: r[2]=rd(m,r[12]+0x10u,4);goto Lc9045ee; /* mov.l @(0x10,r12),r2 */
Lc9045ee: *sr=(*sr&~1u)|!(r[2]&r[2]);goto Lc9045f0; /* tst r2,r2 */
Lc9045f0: if((*sr&1u))goto Lc904674;goto Lc9045f2; /* bt 0x0c904674 */
Lc9045f2: r[0]=sx(m,r[12]+0x16u,2);goto Lc9045f4; /* mov.w @(0x16,r12),r0 */
Lc9045f4: r[0]=r[0]&65535u;goto Lc9045f6; /* extu.w r0,r0 */
Lc9045f6: r[3]=r[0];goto Lc9045f8; /* mov r0,r3 */
Lc9045f8: r[0]<<=2;goto Lc9045fa; /* shll2 r0 */
Lc9045fa: r[0]+=r[3];goto Lc9045fc; /* add r3,r0 */
Lc9045fc: r[0]<<=2;goto Lc9045fe; /* shll2 r0 */
Lc9045fe: target=r[8];*pr=0x0c904602u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904602; /* jsr @r8 */
Lc904602: *sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc904604; /* tst r0,r0 */
Lc904604: take=!(*sr&1u);wr(m,r[14]+0x10u,r[0],4);if(take)goto Lc904660;goto Lc904608; /* bf/s 0x0c904660 */
Lc904608: r[3]=rd(m,r[14],4);goto Lc90460a; /* mov.l @r14,r3 */
Lc90460a: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc90460c; /* tst r3,r3 */
Lc90460c: if((*sr&1u))goto Lc904612;goto Lc90460e; /* bt 0x0c904612 */
Lc90460e: target=r[13];*pr=0x0c904612u;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904612; /* jsr @r13 */
Lc904612: r[3]=rd(m,r[14]+0x4u,4);goto Lc904614; /* mov.l @(0x4,r14),r3 */
Lc904614: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc904616; /* tst r3,r3 */
Lc904616: if((*sr&1u))goto Lc90461c;goto Lc904618; /* bt 0x0c90461c */
Lc904618: target=r[13];*pr=0x0c90461cu;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90461c; /* jsr @r13 */
Lc90461c: target=r[13];*pr=0x0c904620u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904620; /* jsr @r13 */
Lc904620: target=r[13];*pr=0x0c904624u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904624; /* jsr @r13 */
Lc904624: goto Lc90472e; /* bra 0x0c90472e */
Lc904660: r[0]=sx(m,r[12]+0x16u,2);goto Lc904662; /* mov.w @(0x16,r12),r0 */
Lc904662: r[5]=rd(m,r[12]+0x10u,4);goto Lc904664; /* mov.l @(0x10,r12),r5 */
Lc904664: r[0]=r[0]&65535u;goto Lc904666; /* extu.w r0,r0 */
Lc904666: r[3]=r[0];goto Lc904668; /* mov r0,r3 */
Lc904668: r[0]<<=2;goto Lc90466a; /* shll2 r0 */
Lc90466a: r[0]+=r[3];goto Lc90466c; /* add r3,r0 */
Lc90466c: r[0]<<=2;goto Lc90466e; /* shll2 r0 */
Lc90466e: r[6]=r[0];goto Lc904670; /* mov r0,r6 */
Lc904670: target=r[9];*pr=0x0c904674u;r[4]=rd(m,r[14]+0x10u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904674; /* jsr @r9 */
Lc904674: r[2]=rd(m,r[12]+0xcu,4);goto Lc904676; /* mov.l @(0xc,r12),r2 */
Lc904676: *sr=(*sr&~1u)|!(r[2]&r[2]);goto Lc904678; /* tst r2,r2 */
Lc904678: if((*sr&1u))goto Lc904748;goto Lc90467a; /* bt 0x0c904748 */
Lc90467a: r[0]=sx(m,r[12]+0x14u,2);goto Lc90467c; /* mov.w @(0x14,r12),r0 */
Lc90467c: r[0]=r[0]&65535u;goto Lc90467e; /* extu.w r0,r0 */
Lc90467e: r[3]=r[0];goto Lc904680; /* mov r0,r3 */
Lc904680: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc904682; /* shll r0 */
Lc904682: r[0]+=r[3];goto Lc904684; /* add r3,r0 */
Lc904684: r[0]<<=2;goto Lc904686; /* shll2 r0 */
Lc904686: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc904688; /* shll r0 */
Lc904688: target=r[8];*pr=0x0c90468cu;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90468c; /* jsr @r8 */
Lc90468c: *sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc90468e; /* tst r0,r0 */
Lc90468e: take=!(*sr&1u);wr(m,r[14]+0xcu,r[0],4);if(take)goto Lc9046bc;goto Lc904692; /* bf/s 0x0c9046bc */
Lc904692: r[3]=rd(m,r[14],4);goto Lc904694; /* mov.l @r14,r3 */
Lc904694: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc904696; /* tst r3,r3 */
Lc904696: if((*sr&1u))goto Lc90469c;goto Lc904698; /* bt 0x0c90469c */
Lc904698: target=r[13];*pr=0x0c90469cu;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90469c; /* jsr @r13 */
Lc90469c: r[3]=rd(m,r[14]+0x4u,4);goto Lc90469e; /* mov.l @(0x4,r14),r3 */
Lc90469e: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc9046a0; /* tst r3,r3 */
Lc9046a0: if((*sr&1u))goto Lc9046a6;goto Lc9046a2; /* bt 0x0c9046a6 */
Lc9046a2: target=r[13];*pr=0x0c9046a6u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9046a6; /* jsr @r13 */
Lc9046a6: r[3]=rd(m,r[14]+0x10u,4);goto Lc9046a8; /* mov.l @(0x10,r14),r3 */
Lc9046a8: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc9046aa; /* tst r3,r3 */
Lc9046aa: if((*sr&1u))goto Lc9046b0;goto Lc9046ac; /* bt 0x0c9046b0 */
Lc9046ac: target=r[13];*pr=0x0c9046b0u;r[4]=rd(m,r[14]+0x10u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9046b0; /* jsr @r13 */
Lc9046b0: target=r[13];*pr=0x0c9046b4u;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9046b4; /* jsr @r13 */
Lc9046b4: target=r[13];*pr=0x0c9046b8u;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9046b8; /* jsr @r13 */
Lc9046b8: goto Lc90472e; /* bra 0x0c90472e */
Lc9046bc: r[0]=sx(m,r[12]+0x14u,2);goto Lc9046be; /* mov.w @(0x14,r12),r0 */
Lc9046be: r[5]=rd(m,r[12]+0xcu,4);goto Lc9046c0; /* mov.l @(0xc,r12),r5 */
Lc9046c0: r[0]=r[0]&65535u;goto Lc9046c2; /* extu.w r0,r0 */
Lc9046c2: r[3]=r[0];goto Lc9046c4; /* mov r0,r3 */
Lc9046c4: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc9046c6; /* shll r0 */
Lc9046c6: r[0]+=r[3];goto Lc9046c8; /* add r3,r0 */
Lc9046c8: r[0]<<=2;goto Lc9046ca; /* shll2 r0 */
Lc9046ca: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc9046cc; /* shll r0 */
Lc9046cc: r[6]=r[0];goto Lc9046ce; /* mov r0,r6 */
Lc9046ce: target=r[9];*pr=0x0c9046d2u;r[4]=rd(m,r[14]+0xcu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9046d2; /* jsr @r9 */
Lc9046d2: r[2]=rd(m,r[14]+0xcu,4);goto Lc9046d4; /* mov.l @(0xc,r14),r2 */
Lc9046d4: wr(m,r[15],r[2],4);goto Lc9046d6; /* mov.l r2,@r15 */
Lc9046d6: r[12]=rd(m,r[12]+0xcu,4);goto Lc9046d8; /* mov.l @(0xc,r12),r12 */
Lc9046d8: r[3]=rd(m,r[12]+0xcu,4);goto Lc9046da; /* mov.l @(0xc,r12),r3 */
Lc9046da: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc9046dc; /* tst r3,r3 */
Lc9046dc: if((*sr&1u))goto Lc904748;goto Lc9046de; /* bt 0x0c904748 */
Lc9046de: r[3]=rd(m,r[15],4);goto Lc9046e0; /* mov.l @r15,r3 */
Lc9046e0: r[2]=12u;goto Lc9046e2; /* mov #0xc,r2 */
Lc9046e2: r[2]+=r[3];goto Lc9046e4; /* add r3,r2 */
Lc9046e4: r[15]-=4;wr(m,r[15],r[2],4);goto Lc9046e6; /* mov.l r2,@-r15 */
Lc9046e6: r[0]=sx(m,r[12]+0x2u,2);goto Lc9046e8; /* mov.w @(0x2,r12),r0 */
Lc9046e8: r[0]=r[0]&65535u;goto Lc9046ea; /* extu.w r0,r0 */
Lc9046ea: r[3]=r[0];goto Lc9046ec; /* mov r0,r3 */
Lc9046ec: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc9046ee; /* shll r0 */
Lc9046ee: r[0]+=r[3];goto Lc9046f0; /* add r3,r0 */
Lc9046f0: r[0]<<=2;goto Lc9046f2; /* shll2 r0 */
Lc9046f2: target=r[8];*pr=0x0c9046f6u;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc9046f6; /* jsr @r8 */
Lc9046f6: r[1]=rd(m,r[15],4);r[15]+=4;goto Lc9046f8; /* mov.l @r15+,r1 */
Lc9046f8: *sr=(*sr&~1u)|!(r[0]&r[0]);goto Lc9046fa; /* tst r0,r0 */
Lc9046fa: take=!(*sr&1u);wr(m,r[1],r[0],4);if(take)goto Lc904732;goto Lc9046fe; /* bf/s 0x0c904732 */
Lc9046fe: r[2]=rd(m,r[14],4);goto Lc904700; /* mov.l @r14,r2 */
Lc904700: *sr=(*sr&~1u)|!(r[2]&r[2]);goto Lc904702; /* tst r2,r2 */
Lc904702: if((*sr&1u))goto Lc904708;goto Lc904704; /* bt 0x0c904708 */
Lc904704: target=r[13];*pr=0x0c904708u;r[4]=rd(m,r[14],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904708; /* jsr @r13 */
Lc904708: r[3]=rd(m,r[14]+0x4u,4);goto Lc90470a; /* mov.l @(0x4,r14),r3 */
Lc90470a: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc90470c; /* tst r3,r3 */
Lc90470c: if((*sr&1u))goto Lc904712;goto Lc90470e; /* bt 0x0c904712 */
Lc90470e: target=r[13];*pr=0x0c904712u;r[4]=rd(m,r[14]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904712; /* jsr @r13 */
Lc904712: r[3]=rd(m,r[14]+0x10u,4);goto Lc904714; /* mov.l @(0x10,r14),r3 */
Lc904714: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc904716; /* tst r3,r3 */
Lc904716: if((*sr&1u))goto Lc904726;goto Lc904718; /* bt 0x0c904726 */
Lc904718: target=r[13];*pr=0x0c90471cu;r[4]=rd(m,r[14]+0x10u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90471c; /* jsr @r13 */
Lc90471c: r[3]=rd(m,r[14]+0x10u,4);goto Lc90471e; /* mov.l @(0x10,r14),r3 */
Lc90471e: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc904720; /* tst r3,r3 */
Lc904720: if((*sr&1u))goto Lc904726;goto Lc904722; /* bt 0x0c904726 */
Lc904722: target=r[13];*pr=0x0c904726u;r[4]=rd(m,r[14]+0xcu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904726; /* jsr @r13 */
Lc904726: target=r[13];*pr=0x0c90472au;r[4]=r[14];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90472a; /* jsr @r13 */
Lc90472a: target=r[13];*pr=0x0c90472eu;r[4]=r[11];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90472e; /* jsr @r13 */
Lc90472e: r[0]=0u;goto Lc904762; /* bra 0x0c904762 */
Lc904732: r[0]=sx(m,r[12]+0x2u,2);goto Lc904734; /* mov.w @(0x2,r12),r0 */
Lc904734: r[5]=rd(m,r[12]+0xcu,4);goto Lc904736; /* mov.l @(0xc,r12),r5 */
Lc904736: r[0]=r[0]&65535u;goto Lc904738; /* extu.w r0,r0 */
Lc904738: r[4]=rd(m,r[15],4);goto Lc90473a; /* mov.l @r15,r4 */
Lc90473a: r[3]=r[0];goto Lc90473c; /* mov r0,r3 */
Lc90473c: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1;goto Lc90473e; /* shll r0 */
Lc90473e: r[0]+=r[3];goto Lc904740; /* add r3,r0 */
Lc904740: r[0]<<=2;goto Lc904742; /* shll2 r0 */
Lc904742: r[6]=r[0];goto Lc904744; /* mov r0,r6 */
Lc904744: target=r[9];*pr=0x0c904748u;r[4]=rd(m,r[4]+0xcu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904748; /* jsr @r9 */
Lc904748: r[3]=rd(m,r[10]+0x2cu,4);goto Lc90474a; /* mov.l @(0x2c,r10),r3 */
Lc90474a: *sr=(*sr&~1u)|!(r[3]&r[3]);goto Lc90474c; /* tst r3,r3 */
Lc90474c: if((*sr&1u))goto Lc904754;goto Lc90474e; /* bt 0x0c904754 */
Lc90474e: target=0x0c90451cu;*pr=0x0c904752u;r[4]=rd(m,r[10]+0x2cu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904752; /* bsr 0x0c90451c */
Lc904752: wr(m,r[11]+0x2cu,r[0],4);goto Lc904754; /* mov.l r0,@(0x2c,r11) */
Lc904754: r[2]=rd(m,r[10]+0x30u,4);goto Lc904756; /* mov.l @(0x30,r10),r2 */
Lc904756: *sr=(*sr&~1u)|!(r[2]&r[2]);goto Lc904758; /* tst r2,r2 */
Lc904758: if((*sr&1u))goto Lc904760;goto Lc90475a; /* bt 0x0c904760 */
Lc90475a: target=0x0c90451cu;*pr=0x0c90475eu;r[4]=rd(m,r[10]+0x30u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc90475e; /* bsr 0x0c90451c */
Lc90475e: wr(m,r[11]+0x30u,r[0],4);goto Lc904760; /* mov.l r0,@(0x30,r11) */
Lc904760: r[0]=r[11];goto Lc904762; /* mov r11,r0 */
Lc904762: r[15]+=4u;goto Lc904764; /* add #0x4,r15 */
Lc904764: *pr=rd(m,r[15],4);r[15]+=4;goto Lc904766; /* lds.l @r15+,PR */
Lc904766: r[8]=rd(m,r[15],4);r[15]+=4;goto Lc904768; /* mov.l @r15+,r8 */
Lc904768: r[9]=rd(m,r[15],4);r[15]+=4;goto Lc90476a; /* mov.l @r15+,r9 */
Lc90476a: r[10]=rd(m,r[15],4);r[15]+=4;goto Lc90476c; /* mov.l @r15+,r10 */
Lc90476c: r[11]=rd(m,r[15],4);r[15]+=4;goto Lc90476e; /* mov.l @r15+,r11 */
Lc90476e: r[12]=rd(m,r[15],4);r[15]+=4;goto Lc904770; /* mov.l @r15+,r12 */
Lc904770: r[13]=rd(m,r[15],4);r[15]+=4;goto Lc904772; /* mov.l @r15+,r13 */
Lc904772: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_overlay_object_setup_0c904d22(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)sr;(void)fload;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto Lc904d22;
Lc904d22: r[0]=8u;goto Lc904d24; /* mov #0x8,r0 */
Lc904d24: r[3]=rd(m,0x0c904e68u,4);goto Lc904d26; /* mov.l 0x0c904e68,r3 */
Lc904d26: r[0]<<=8;goto Lc904d28; /* shll8 r0 */
Lc904d28: r[15]-=4;wr(m,r[15],r[14],4);goto Lc904d2a; /* mov.l r14,@-r15 */
Lc904d2a: r[14]=r[5];goto Lc904d2c; /* mov r5,r14 */
Lc904d2c: r[0]+=4294967236u;goto Lc904d2e; /* add #-0x3c,r0 */
Lc904d2e: r[15]-=4;wr(m,r[15],r[13],4);goto Lc904d30; /* mov.l r13,@-r15 */
Lc904d30: r[13]=2u;goto Lc904d32; /* mov #0x2,r13 */
Lc904d32: r[15]-=4;wr(m,r[15],r[12],4);goto Lc904d34; /* mov.l r12,@-r15 */
Lc904d34: r[13]<<=8;goto Lc904d36; /* shll8 r13 */
Lc904d36: r[15]-=4;wr(m,r[15],r[11],4);goto Lc904d38; /* mov.l r11,@-r15 */
Lc904d38: r[13]+=72u;goto Lc904d3a; /* add #0x48,r13 */
Lc904d3a: r[15]-=4;wr(m,r[15],r[10],4);goto Lc904d3c; /* mov.l r10,@-r15 */
Lc904d3c: r[10]=r[4];goto Lc904d3e; /* mov r4,r10 */
Lc904d3e: r[15]-=4;wr(m,r[15],*pr,4);goto Lc904d40; /* sts.l PR,@-r15 */
Lc904d40: wr(m,r[14]+r[0],r[3],4);goto Lc904d42; /* mov.l r3,@(r0,r14) */
Lc904d42: r[0]=4294967276u;goto Lc904d44; /* mov #-0x14,r0 */
Lc904d44: r[0]=r[0]&255u;goto Lc904d46; /* extu.b r0,r0 */
Lc904d46: r[11]=rd(m,r[14]+r[0],4);goto Lc904d48; /* mov.l @(r0,r14),r11 */
Lc904d48: r[11]=sx(m,r[11],1);goto Lc904d4a; /* mov.b @r11,r11 */
Lc904d4a: r[0]=(uint32_t)(int32_t)(int8_t)r[11];goto Lc904d4c; /* exts.b r11,r0 */
Lc904d4c: *sr=(*sr&~1u)|(r[0]==0x00000007u);goto Lc904d4e; /* cmp/eq #0x7,r0 */
Lc904d4e: take=!(*sr&1u);r[13]+=r[14];if(take)goto Lc904d54;goto Lc904d52; /* bf/s 0x0c904d54 */
Lc904d52: r[11]=0u;goto Lc904d54; /* mov #0x0,r11 */
Lc904d54: r[12]=(uint32_t)(int32_t)(int8_t)r[11];goto Lc904d56; /* exts.b r11,r12 */
Lc904d56: r[2]=rd(m,0x0c904e6cu,4);goto Lc904d58; /* mov.l 0x0c904e6c,r2 */
Lc904d58: r[3]=r[12];goto Lc904d5a; /* mov r12,r3 */
Lc904d5a: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto Lc904d5c; /* shll r12 */
Lc904d5c: r[12]+=r[3];goto Lc904d5e; /* add r3,r12 */
Lc904d5e: if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto Lc904d60; /* fldi0 fr3 */
Lc904d60: r[12]<<=2;goto Lc904d62; /* shll2 r12 */
Lc904d62: r[0]=4294967252u;goto Lc904d64; /* mov #-0x2c,r0 */
Lc904d64: *sr=(*sr&~1u)|(r[12]>>31);r[12]<<=1;goto Lc904d66; /* shll r12 */
Lc904d66: r[0]=r[0]&255u;goto Lc904d68; /* extu.b r0,r0 */
Lc904d68: r[12]+=r[2];goto Lc904d6a; /* add r2,r12 */
Lc904d6a: fstore(m,f,3u,r[0]+r[14]);goto Lc904d6c; /* fmov fr3,@(r0,r14) */
Lc904d6c: target=0x0c90451cu;*pr=0x0c904d70u;r[4]=rd(m,r[12],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904d70; /* bsr 0x0c90451c */
Lc904d70: r[5]=r[13];goto Lc904d72; /* mov r13,r5 */
Lc904d72: r[5]+=4u;goto Lc904d74; /* add #0x4,r5 */
Lc904d74: wr(m,r[13],r[0],4);goto Lc904d76; /* mov.l r0,@r13 */
Lc904d76: target=0x0c9044e2u;*pr=0x0c904d7au;r[4]=r[0];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904d7a; /* bsr 0x0c9044e2 */
Lc904d7a: r[5]=4294967244u;goto Lc904d7c; /* mov #-0x34,r5 */
Lc904d7c: r[5]=r[5]&255u;goto Lc904d7e; /* extu.b r5,r5 */
Lc904d7e: r[5]+=r[13];goto Lc904d80; /* add r13,r5 */
Lc904d80: target=0x0c9044e2u;*pr=0x0c904d84u;r[4]=rd(m,r[12],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904d84; /* bsr 0x0c9044e2 */
Lc904d84: r[5]=101u;goto Lc904d86; /* mov #0x65,r5 */
Lc904d86: r[5]<<=2;goto Lc904d88; /* shll2 r5 */
Lc904d88: r[5]+=r[13];goto Lc904d8a; /* add r13,r5 */
Lc904d8a: target=0x0c9044e2u;*pr=0x0c904d8eu;r[4]=rd(m,r[12]+0x4u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904d8e; /* bsr 0x0c9044e2 */
Lc904d8e: r[5]=5u;goto Lc904d90; /* mov #0x5,r5 */
Lc904d90: r[5]<<=8;goto Lc904d92; /* shll8 r5 */
Lc904d92: r[5]+=4294967220u;goto Lc904d94; /* add #-0x4c,r5 */
Lc904d94: r[5]+=r[13];goto Lc904d96; /* add r13,r5 */
Lc904d96: target=0x0c9044e2u;*pr=0x0c904d9au;r[4]=rd(m,r[12]+0x8u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904d9a; /* bsr 0x0c9044e2 */
Lc904d9a: r[5]=2u;goto Lc904d9c; /* mov #0x2,r5 */
Lc904d9c: r[5]<<=8;goto Lc904d9e; /* shll8 r5 */
Lc904d9e: r[5]+=92u;goto Lc904da0; /* add #0x5c,r5 */
Lc904da0: r[5]+=r[13];goto Lc904da2; /* add r13,r5 */
Lc904da2: target=0x0c9044e2u;*pr=0x0c904da6u;r[4]=rd(m,r[12]+0xcu,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904da6; /* bsr 0x0c9044e2 */
Lc904da6: r[5]=3u;goto Lc904da8; /* mov #0x3,r5 */
Lc904da8: r[5]<<=8;goto Lc904daa; /* shll8 r5 */
Lc904daa: r[5]+=36u;goto Lc904dac; /* add #0x24,r5 */
Lc904dac: r[5]+=r[13];goto Lc904dae; /* add r13,r5 */
Lc904dae: target=0x0c9044e2u;*pr=0x0c904db2u;r[4]=rd(m,r[12]+0x10u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904db2; /* bsr 0x0c9044e2 */
Lc904db2: r[5]=4u;goto Lc904db4; /* mov #0x4,r5 */
Lc904db4: r[5]<<=8;goto Lc904db6; /* shll8 r5 */
Lc904db6: r[5]+=4294967276u;goto Lc904db8; /* add #-0x14,r5 */
Lc904db8: r[5]+=r[13];goto Lc904dba; /* add r13,r5 */
Lc904dba: target=0x0c9044e2u;*pr=0x0c904dbeu;r[4]=rd(m,r[12]+0x14u,4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904dbe; /* bsr 0x0c9044e2 */
Lc904dbe: r[0]=4294967276u;goto Lc904dc0; /* mov #-0x14,r0 */
Lc904dc0: r[0]=r[0]&255u;goto Lc904dc2; /* extu.b r0,r0 */
Lc904dc2: r[0]=rd(m,r[14]+r[0],4);goto Lc904dc4; /* mov.l @(r0,r14),r0 */
Lc904dc4: r[1]=54u;goto Lc904dc6; /* mov #0x36,r1 */
Lc904dc6: r[0]=sx(m,r[0]+r[1],2);goto Lc904dc8; /* mov.w @(r0,r1),r0 */
Lc904dc8: r[0]=r[0]&65535u;goto Lc904dca; /* extu.w r0,r0 */
Lc904dca: *sr=(*sr&~1u)|!(r[0]&0x40u);goto Lc904dcc; /* tst #0x40,r0 */
Lc904dcc: if((*sr&1u))goto Lc904dd4;goto Lc904dce; /* bt 0x0c904dd4 */
Lc904dce: r[5]=r[14];goto Lc904dd0; /* mov r14,r5 */
Lc904dd0: target=0x0c904aeau;*pr=0x0c904dd4u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto Lc904dd4; /* bsr 0x0c904aea */
Lc904dd4: r[0]=4294967276u;goto Lc904dd6; /* mov #-0x14,r0 */
Lc904dd6: r[0]=r[0]&255u;goto Lc904dd8; /* extu.b r0,r0 */
Lc904dd8: r[0]=rd(m,r[14]+r[0],4);goto Lc904dda; /* mov.l @(r0,r14),r0 */
Lc904dda: r[1]=54u;goto Lc904ddc; /* mov #0x36,r1 */
Lc904ddc: r[0]=sx(m,r[0]+r[1],2);goto Lc904dde; /* mov.w @(r0,r1),r0 */
Lc904dde: r[0]=r[0]&65535u;goto Lc904de0; /* extu.w r0,r0 */
Lc904de0: *sr=(*sr&~1u)|!(r[0]&0x4u);goto Lc904de2; /* tst #0x4,r0 */
Lc904de2: if((*sr&1u))goto Lc904df6;goto Lc904de4; /* bt 0x0c904df6 */
Lc904de4: *pr=rd(m,r[15],4);r[15]+=4;goto Lc904de6; /* lds.l @r15+,PR */
Lc904de6: r[4]=r[10];goto Lc904de8; /* mov r10,r4 */
Lc904de8: r[5]=r[14];goto Lc904dea; /* mov r14,r5 */
Lc904dea: r[10]=rd(m,r[15],4);r[15]+=4;goto Lc904dec; /* mov.l @r15+,r10 */
Lc904dec: r[11]=rd(m,r[15],4);r[15]+=4;goto Lc904dee; /* mov.l @r15+,r11 */
Lc904dee: r[12]=rd(m,r[15],4);r[15]+=4;goto Lc904df0; /* mov.l @r15+,r12 */
Lc904df0: r[13]=rd(m,r[15],4);r[15]+=4;goto Lc904df2; /* mov.l @r15+,r13 */
Lc904df2: r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0xc904bbcu,m,f,sr,r,pr); /* bra 0x0c904bbc */
Lc904df6: r[0]=4294967276u;goto Lc904df8; /* mov #-0x14,r0 */
Lc904df8: r[0]=r[0]&255u;goto Lc904dfa; /* extu.b r0,r0 */
Lc904dfa: r[0]=rd(m,r[14]+r[0],4);goto Lc904dfc; /* mov.l @(r0,r14),r0 */
Lc904dfc: r[2]=54u;goto Lc904dfe; /* mov #0x36,r2 */
Lc904dfe: r[0]=sx(m,r[0]+r[2],2);goto Lc904e00; /* mov.w @(r0,r2),r0 */
Lc904e00: r[0]=r[0]&65535u;goto Lc904e02; /* extu.w r0,r0 */
Lc904e02: *sr=(*sr&~1u)|!(r[0]&0x8u);goto Lc904e04; /* tst #0x8,r0 */
Lc904e04: if((*sr&1u))goto Lc904e18;goto Lc904e06; /* bt 0x0c904e18 */
Lc904e06: *pr=rd(m,r[15],4);r[15]+=4;goto Lc904e08; /* lds.l @r15+,PR */
Lc904e08: r[4]=r[10];goto Lc904e0a; /* mov r10,r4 */
Lc904e0a: r[5]=r[14];goto Lc904e0c; /* mov r14,r5 */
Lc904e0c: r[10]=rd(m,r[15],4);r[15]+=4;goto Lc904e0e; /* mov.l @r15+,r10 */
Lc904e0e: r[11]=rd(m,r[15],4);r[15]+=4;goto Lc904e10; /* mov.l @r15+,r11 */
Lc904e10: r[12]=rd(m,r[15],4);r[15]+=4;goto Lc904e12; /* mov.l @r15+,r12 */
Lc904e12: r[13]=rd(m,r[15],4);r[15]+=4;goto Lc904e14; /* mov.l @r15+,r13 */
Lc904e14: r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;return call(ctx,0xc904c80u,m,f,sr,r,pr); /* bra 0x0c904c80 */
Lc904e18: *pr=rd(m,r[15],4);r[15]+=4;goto Lc904e1a; /* lds.l @r15+,PR */
Lc904e1a: r[10]=rd(m,r[15],4);r[15]+=4;goto Lc904e1c; /* mov.l @r15+,r10 */
Lc904e1c: r[11]=rd(m,r[15],4);r[15]+=4;goto Lc904e1e; /* mov.l @r15+,r11 */
Lc904e1e: r[12]=rd(m,r[15],4);r[15]+=4;goto Lc904e20; /* mov.l @r15+,r12 */
Lc904e20: r[13]=rd(m,r[15],4);r[15]+=4;goto Lc904e22; /* mov.l @r15+,r13 */
Lc904e22: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
