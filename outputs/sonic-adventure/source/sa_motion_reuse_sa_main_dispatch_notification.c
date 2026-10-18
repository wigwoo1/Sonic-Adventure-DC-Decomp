#include "sa_draw_motion_stop.h"
/* Notification-related routines, names inferred; generated checked static C.
 * Integer instructions only: all FPU modes preserved, including across calls.
 * Valid original RAM/pointer preconditions. No MMIO or memory exceptions.
 * Host 0 completes (retail results in registers); -2 at reached absent service.
 * Tail disposal preserves PR; service errors retain exact reached partial state.
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
SA_STAGE_EXPORT int sa_main_dispatch_notification_8c109fde(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;
 int status;uint32_t target;
 int take;
 goto L8c109fde;
L8c109fde: sa_motion_publish_stop(ctx,0x8c109fdeu); r[15]-=4;wr(m,r[15],r[14],4);goto L8c109fe0; /* mov.l r14,@-r15 */
L8c109fe0: sa_motion_publish_stop(ctx,0x8c109fe0u); r[15]-=4;wr(m,r[15],r[13],4);goto L8c109fe2; /* mov.l r13,@-r15 */
L8c109fe2: sa_motion_publish_stop(ctx,0x8c109fe2u); r[15]-=4;wr(m,r[15],r[12],4);goto L8c109fe4; /* mov.l r12,@-r15 */
L8c109fe4: sa_motion_publish_stop(ctx,0x8c109fe4u); r[15]-=4;wr(m,r[15],r[11],4);goto L8c109fe6; /* mov.l r11,@-r15 */
L8c109fe6: sa_motion_publish_stop(ctx,0x8c109fe6u); r[15]-=4;wr(m,r[15],r[10],4);goto L8c109fe8; /* mov.l r10,@-r15 */
L8c109fe8: sa_motion_publish_stop(ctx,0x8c109fe8u); r[15]-=4;wr(m,r[15],r[9],4);goto L8c109fea; /* mov.l r9,@-r15 */
L8c109fea: sa_motion_publish_stop(ctx,0x8c109feau); r[15]-=4;wr(m,r[15],r[8],4);goto L8c109fec; /* mov.l r8,@-r15 */
L8c109fec: sa_motion_publish_stop(ctx,0x8c109fecu); r[15]-=4;wr(m,r[15],*pr,4);goto L8c109fee; /* sts.l PR,@-r15 */
L8c109fee: sa_motion_publish_stop(ctx,0x8c109feeu); r[15]+=4294967284u;goto L8c109ff0; /* add #-0xc,r15 */
L8c109ff0: sa_motion_publish_stop(ctx,0x8c109ff0u); r[2]=rd(m,0x8c10a084u,4);goto L8c109ff2; /* mov.l 0x8c10a084,r2 */
L8c109ff2: sa_motion_publish_stop(ctx,0x8c109ff2u); r[14]=r[5];goto L8c109ff4; /* mov r5,r14 */
L8c109ff4: sa_motion_publish_stop(ctx,0x8c109ff4u); wr(m,r[15],r[7],4);goto L8c109ff6; /* mov.l r7,@r15 */
L8c109ff6: sa_motion_publish_stop(ctx,0x8c109ff6u); r[3]=rd(m,r[2],4);goto L8c109ff8; /* mov.l @r2,r3 */
L8c109ff8: sa_motion_publish_stop(ctx,0x8c109ff8u); *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c109ffa; /* tst r3,r3 */
L8c109ffa: sa_motion_publish_stop(ctx,0x8c109ffcu); take=(*sr&1u);r[12]=r[6];if(take)goto L8c10a002;goto L8c109ffe; /* bt/s 0x8c10a002 */
L8c109ffe: sa_motion_publish_stop(ctx,0x8c10a000u); goto L8c10a0d8; /* bra 0x8c10a0d8 */
L8c10a002: sa_motion_publish_stop(ctx,0x8c10a002u); r[0]=rd(m,0x8c10a088u,4);goto L8c10a004; /* mov.l 0x8c10a088,r0 */
L8c10a004: sa_motion_publish_stop(ctx,0x8c10a004u); r[13]=r[4];goto L8c10a006; /* mov r4,r13 */
L8c10a006: sa_motion_publish_stop(ctx,0x8c10a006u); r[13]<<=2;goto L8c10a008; /* shll2 r13 */
L8c10a008: sa_motion_publish_stop(ctx,0x8c10a008u); r[13]=rd(m,r[13]+r[0],4);goto L8c10a00a; /* mov.l @(r0,r13),r13 */
L8c10a00a: sa_motion_publish_stop(ctx,0x8c10a00au); r[10]=4294967295u;goto L8c10a00c; /* mov #-0x1,r10 */
L8c10a00c: sa_motion_publish_stop(ctx,0x8c10a00cu); r[0]=r[13];goto L8c10a00e; /* mov r13,r0 */
L8c10a00e: sa_motion_publish_stop(ctx,0x8c10a00eu); *sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c10a010; /* cmp/eq #-0x1,r0 */
L8c10a010: sa_motion_publish_stop(ctx,0x8c10a012u); take=(*sr&1u);r[11]=2u;if(take)goto L8c10a038;goto L8c10a014; /* bt/s 0x8c10a038 */
L8c10a014: sa_motion_publish_stop(ctx,0x8c10a014u); r[9]=rd(m,0x8c10a08cu,4);goto L8c10a016; /* mov.l 0x8c10a08c,r9 */
L8c10a016: sa_motion_publish_stop(ctx,0x8c10a016u); r[13]+=r[13];goto L8c10a018; /* add r13,r13 */
L8c10a018: sa_motion_publish_stop(ctx,0x8c10a018u); r[8]=0u;goto L8c10a01a; /* mov #0x0,r8 */
L8c10a01a: sa_motion_publish_stop(ctx,0x8c10a01au); r[3]=rd(m,0x8c10a090u,4);goto L8c10a01c; /* mov.l 0x8c10a090,r3 */
L8c10a01c: sa_motion_publish_stop(ctx,0x8c10a01cu); r[0]=r[13];goto L8c10a01e; /* mov r13,r0 */
L8c10a01e: sa_motion_publish_stop(ctx,0x8c10a01eu); r[0]<<=2;goto L8c10a020; /* shll2 r0 */
L8c10a020: sa_motion_publish_stop(ctx,0x8c10a022u); target=r[3];*pr=0x8c10a024u;r[4]=rd(m,r[9]+r[0],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10a024; /* jsr @r3 */
L8c10a024: sa_motion_publish_stop(ctx,0x8c10a024u); *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c10a026; /* tst r0,r0 */
L8c10a026: sa_motion_publish_stop(ctx,0x8c10a026u); if((*sr&1u))goto L8c10a030;goto L8c10a028; /* bt 0x8c10a030 */
L8c10a028: sa_motion_publish_stop(ctx,0x8c10a028u); r[0]=r[13];goto L8c10a02a; /* mov r13,r0 */
L8c10a02a: sa_motion_publish_stop(ctx,0x8c10a02au); r[0]<<=2;goto L8c10a02c; /* shll2 r0 */
L8c10a02c: sa_motion_publish_stop(ctx,0x8c10a02eu); r[10]=rd(m,r[9]+r[0],4);goto L8c10a038; /* bra 0x8c10a038 */
L8c10a030: sa_motion_publish_stop(ctx,0x8c10a030u); r[8]+=1u;goto L8c10a032; /* add #0x1,r8 */
L8c10a032: sa_motion_publish_stop(ctx,0x8c10a032u); *sr=(*sr&~1u)|((int32_t)r[8]>=(int32_t)r[11]);goto L8c10a034; /* cmp/ge r11,r8 */
L8c10a034: sa_motion_publish_stop(ctx,0x8c10a036u); take=!(*sr&1u);r[13]+=1u;if(take)goto L8c10a01a;goto L8c10a038; /* bf/s 0x8c10a01a */
L8c10a038: sa_motion_publish_stop(ctx,0x8c10a038u); r[0]=r[10];goto L8c10a03a; /* mov r10,r0 */
L8c10a03a: sa_motion_publish_stop(ctx,0x8c10a03au); *sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c10a03c; /* cmp/eq #-0x1,r0 */
L8c10a03c: sa_motion_publish_stop(ctx,0x8c10a03cu); if((*sr&1u))goto L8c10a0d8;goto L8c10a03e; /* bt 0x8c10a0d8 */
L8c10a03e: sa_motion_publish_stop(ctx,0x8c10a03eu); r[4]=4u;goto L8c10a040; /* mov #0x4,r4 */
L8c10a040: sa_motion_publish_stop(ctx,0x8c10a040u); *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[4]);goto L8c10a042; /* cmp/gt r4,r14 */
L8c10a042: sa_motion_publish_stop(ctx,0x8c10a042u); if(!(*sr&1u))goto L8c10a048;goto L8c10a044; /* bf 0x8c10a048 */
L8c10a044: sa_motion_publish_stop(ctx,0x8c10a046u); r[14]=r[4];goto L8c10a050; /* bra 0x8c10a050 */
L8c10a048: sa_motion_publish_stop(ctx,0x8c10a048u); r[4]=4294967292u;goto L8c10a04a; /* mov #-0x4,r4 */
L8c10a04a: sa_motion_publish_stop(ctx,0x8c10a04au); *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[4]);goto L8c10a04c; /* cmp/ge r4,r14 */
L8c10a04c: sa_motion_publish_stop(ctx,0x8c10a04cu); if((*sr&1u))goto L8c10a050;goto L8c10a04e; /* bt 0x8c10a050 */
L8c10a04e: sa_motion_publish_stop(ctx,0x8c10a04eu); r[14]=r[4];goto L8c10a050; /* mov r4,r14 */
L8c10a050: sa_motion_publish_stop(ctx,0x8c10a050u); r[0]=r[14];goto L8c10a052; /* mov r14,r0 */
L8c10a052: sa_motion_publish_stop(ctx,0x8c10a052u); *sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c10a054; /* cmp/eq #0x1,r0 */
L8c10a054: sa_motion_publish_stop(ctx,0x8c10a054u); if(!(*sr&1u))goto L8c10a05a;goto L8c10a056; /* bf 0x8c10a05a */
L8c10a056: sa_motion_publish_stop(ctx,0x8c10a058u); r[14]=r[11];goto L8c10a060; /* bra 0x8c10a060 */
L8c10a05a: sa_motion_publish_stop(ctx,0x8c10a05au); *sr=(*sr&~1u)|(r[0]==0xffffffffu);goto L8c10a05c; /* cmp/eq #-0x1,r0 */
L8c10a05c: sa_motion_publish_stop(ctx,0x8c10a05cu); if(!(*sr&1u))goto L8c10a060;goto L8c10a05e; /* bf 0x8c10a060 */
L8c10a05e: sa_motion_publish_stop(ctx,0x8c10a05eu); r[14]=4294967294u;goto L8c10a060; /* mov #-0x2,r14 */
L8c10a060: sa_motion_publish_stop(ctx,0x8c10a060u); r[4]=59u;goto L8c10a062; /* mov #0x3b,r4 */
L8c10a062: sa_motion_publish_stop(ctx,0x8c10a062u); *sr=(*sr&~1u)|((int32_t)r[12]>(int32_t)r[4]);goto L8c10a064; /* cmp/gt r4,r12 */
L8c10a064: sa_motion_publish_stop(ctx,0x8c10a064u); if(!(*sr&1u))goto L8c10a0a0;goto L8c10a066; /* bf 0x8c10a0a0 */
L8c10a066: sa_motion_publish_stop(ctx,0x8c10a068u); r[12]=r[4];goto L8c10a0a8; /* bra 0x8c10a0a8 */
L8c10a0a0: sa_motion_publish_stop(ctx,0x8c10a0a0u); r[4]=7u;goto L8c10a0a2; /* mov #0x7,r4 */
L8c10a0a2: sa_motion_publish_stop(ctx,0x8c10a0a2u); *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[4]);goto L8c10a0a4; /* cmp/ge r4,r12 */
L8c10a0a4: sa_motion_publish_stop(ctx,0x8c10a0a4u); if((*sr&1u))goto L8c10a0a8;goto L8c10a0a6; /* bt 0x8c10a0a8 */
L8c10a0a6: sa_motion_publish_stop(ctx,0x8c10a0a6u); r[12]=r[4];goto L8c10a0a8; /* mov r4,r12 */
L8c10a0a8: sa_motion_publish_stop(ctx,0x8c10a0a8u); r[1]=rd(m,r[15],4);goto L8c10a0aa; /* mov.l @r15,r1 */
L8c10a0aa: sa_motion_publish_stop(ctx,0x8c10a0aau); r[5]=4294967168u;goto L8c10a0ac; /* mov #-0x80,r5 */
L8c10a0ac: sa_motion_publish_stop(ctx,0x8c10a0acu); r[3]=rd(m,0x8c10a100u,4);goto L8c10a0ae; /* mov.l 0x8c10a100,r3 */
L8c10a0ae: sa_motion_publish_stop(ctx,0x8c10a0aeu); r[0]=r[14];goto L8c10a0b0; /* mov r14,r0 */
L8c10a0b0: sa_motion_publish_stop(ctx,0x8c10a0b0u); *macl=r[1]*r[12];goto L8c10a0b2; /* mul.l r1,r12 */
L8c10a0b2: sa_motion_publish_stop(ctx,0x8c10a0b2u); r[5]=r[5]&255u;goto L8c10a0b4; /* extu.b r5,r5 */
L8c10a0b4: sa_motion_publish_stop(ctx,0x8c10a0b4u); r[1]=*macl;goto L8c10a0b6; /* sts MACL,r1 */
L8c10a0b6: sa_motion_publish_stop(ctx,0x8c10a0b8u); target=r[3];*pr=0x8c10a0bau;r[0]<<=2;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10a0ba; /* jsr @r3 */
L8c10a0ba: sa_motion_publish_stop(ctx,0x8c10a0bau); r[13]=r[0];goto L8c10a0bc; /* mov r0,r13 */
L8c10a0bc: sa_motion_publish_stop(ctx,0x8c10a0bcu); *sr=(*sr&~1u)|((int32_t)r[13]>0);goto L8c10a0be; /* cmp/pl r13 */
L8c10a0be: sa_motion_publish_stop(ctx,0x8c10a0beu); if((*sr&1u))goto L8c10a0c2;goto L8c10a0c0; /* bt 0x8c10a0c2 */
L8c10a0c0: sa_motion_publish_stop(ctx,0x8c10a0c0u); r[13]=1u;goto L8c10a0c2; /* mov #0x1,r13 */
L8c10a0c2: sa_motion_publish_stop(ctx,0x8c10a0c2u); r[3]=rd(m,0x8c10a100u,4);goto L8c10a0c4; /* mov.l 0x8c10a100,r3 */
L8c10a0c4: sa_motion_publish_stop(ctx,0x8c10a0c4u); r[1]=4294967295u;goto L8c10a0c6; /* mov #-0x1,r1 */
L8c10a0c6: sa_motion_publish_stop(ctx,0x8c10a0c6u); r[1]=r[1]&255u;goto L8c10a0c8; /* extu.b r1,r1 */
L8c10a0c8: sa_motion_publish_stop(ctx,0x8c10a0cau); target=r[3];*pr=0x8c10a0ccu;r[0]=r[13];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10a0cc; /* jsr @r3 */
L8c10a0cc: sa_motion_publish_stop(ctx,0x8c10a0ccu); r[6]=r[14];goto L8c10a0ce; /* mov r14,r6 */
L8c10a0ce: sa_motion_publish_stop(ctx,0x8c10a0ceu); r[7]=r[12];goto L8c10a0d0; /* mov r12,r7 */
L8c10a0d0: sa_motion_publish_stop(ctx,0x8c10a0d0u); r[15]-=4;wr(m,r[15],r[0],4);goto L8c10a0d2; /* mov.l r0,@-r15 */
L8c10a0d2: sa_motion_publish_stop(ctx,0x8c10a0d4u); target=0x8c109e50u;*pr=0x8c10a0d6u;r[4]=r[10];if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c10a0d6; /* bsr 0x8c109e50 */
L8c10a0d6: sa_motion_publish_stop(ctx,0x8c10a0d6u); r[15]+=4u;goto L8c10a0d8; /* add #0x4,r15 */
L8c10a0d8: sa_motion_publish_stop(ctx,0x8c10a0d8u); r[15]+=12u;goto L8c10a0da; /* add #0xc,r15 */
L8c10a0da: sa_motion_publish_stop(ctx,0x8c10a0dau); *pr=rd(m,r[15],4);r[15]+=4;goto L8c10a0dc; /* lds.l @r15+,PR */
L8c10a0dc: sa_motion_publish_stop(ctx,0x8c10a0dcu); r[8]=rd(m,r[15],4);r[15]+=4;goto L8c10a0de; /* mov.l @r15+,r8 */
L8c10a0de: sa_motion_publish_stop(ctx,0x8c10a0deu); r[9]=rd(m,r[15],4);r[15]+=4;goto L8c10a0e0; /* mov.l @r15+,r9 */
L8c10a0e0: sa_motion_publish_stop(ctx,0x8c10a0e0u); r[10]=rd(m,r[15],4);r[15]+=4;goto L8c10a0e2; /* mov.l @r15+,r10 */
L8c10a0e2: sa_motion_publish_stop(ctx,0x8c10a0e2u); r[11]=rd(m,r[15],4);r[15]+=4;goto L8c10a0e4; /* mov.l @r15+,r11 */
L8c10a0e4: sa_motion_publish_stop(ctx,0x8c10a0e4u); r[12]=rd(m,r[15],4);r[15]+=4;goto L8c10a0e6; /* mov.l @r15+,r12 */
L8c10a0e6: sa_motion_publish_stop(ctx,0x8c10a0e6u); r[13]=rd(m,r[15],4);r[15]+=4;goto L8c10a0e8; /* mov.l @r15+,r13 */
L8c10a0e8: sa_motion_publish_stop(ctx,0x8c10a0eau); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_notification_create_8c109e50(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;
 int status;uint32_t target;
 int take;
 goto L8c109e50;
L8c109e50: sa_motion_publish_stop(ctx,0x8c109e50u); r[15]-=4;wr(m,r[15],r[14],4);goto L8c109e52; /* mov.l r14,@-r15 */
L8c109e52: sa_motion_publish_stop(ctx,0x8c109e52u); r[15]-=4;wr(m,r[15],*pr,4);goto L8c109e54; /* sts.l PR,@-r15 */
L8c109e54: sa_motion_publish_stop(ctx,0x8c109e54u); r[15]+=4294967280u;goto L8c109e56; /* add #-0x10,r15 */
L8c109e56: sa_motion_publish_stop(ctx,0x8c109e56u); r[3]=rd(m,0x8c109f4cu,4);goto L8c109e58; /* mov.l 0x8c109f4c,r3 */
L8c109e58: sa_motion_publish_stop(ctx,0x8c109e58u); wr(m,r[15]+0xcu,r[4],4);goto L8c109e5a; /* mov.l r4,@(0xc,r15) */
L8c109e5a: sa_motion_publish_stop(ctx,0x8c109e5au); wr(m,r[15],r[5],4);goto L8c109e5c; /* mov.l r5,@r15 */
L8c109e5c: sa_motion_publish_stop(ctx,0x8c109e5cu); r[5]=0u;goto L8c109e5e; /* mov #0x0,r5 */
L8c109e5e: sa_motion_publish_stop(ctx,0x8c109e5eu); wr(m,r[15]+0x8u,r[6],4);goto L8c109e60; /* mov.l r6,@(0x8,r15) */
L8c109e60: sa_motion_publish_stop(ctx,0x8c109e60u); r[6]=rd(m,0x8c109f48u,4);goto L8c109e62; /* mov.l 0x8c109f48,r6 */
L8c109e62: sa_motion_publish_stop(ctx,0x8c109e62u); wr(m,r[15]+0x4u,r[7],4);goto L8c109e64; /* mov.l r7,@(0x4,r15) */
L8c109e64: sa_motion_publish_stop(ctx,0x8c109e66u); target=r[3];*pr=0x8c109e68u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c109e68; /* jsr @r3 */
L8c109e68: sa_motion_publish_stop(ctx,0x8c109e68u); *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c109e6a; /* tst r0,r0 */
L8c109e6a: sa_motion_publish_stop(ctx,0x8c109e6cu); take=(*sr&1u);r[14]=r[0];if(take)goto L8c109ea4;goto L8c109e6e; /* bt/s 0x8c109ea4 */
L8c109e6e: sa_motion_publish_stop(ctx,0x8c109e6eu); r[3]=rd(m,0x8c109f50u,4);goto L8c109e70; /* mov.l 0x8c109f50,r3 */
L8c109e70: sa_motion_publish_stop(ctx,0x8c109e72u); target=r[3];*pr=0x8c109e74u;r[4]=8u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c109e74; /* jsr @r3 */
L8c109e74: sa_motion_publish_stop(ctx,0x8c109e74u); *sr=(*sr&~1u)|!(r[0]&r[0]);goto L8c109e76; /* tst r0,r0 */
L8c109e76: sa_motion_publish_stop(ctx,0x8c109e78u); take=(*sr&1u);r[4]=r[0];if(take)goto L8c109e98;goto L8c109e7a; /* bt/s 0x8c109e98 */
L8c109e7a: sa_motion_publish_stop(ctx,0x8c109e7au); wr(m,r[14]+0x28u,r[4],4);goto L8c109e7c; /* mov.l r4,@(0x28,r14) */
L8c109e7c: sa_motion_publish_stop(ctx,0x8c109e7cu); r[2]=rd(m,r[15]+0xcu,4);goto L8c109e7e; /* mov.l @(0xc,r15),r2 */
L8c109e7e: sa_motion_publish_stop(ctx,0x8c109e7eu); r[3]=rd(m,r[14]+0x2cu,4);goto L8c109e80; /* mov.l @(0x2c,r14),r3 */
L8c109e80: sa_motion_publish_stop(ctx,0x8c109e80u); wr(m,r[3],r[2],4);goto L8c109e82; /* mov.l r2,@r3 */
L8c109e82: sa_motion_publish_stop(ctx,0x8c109e82u); r[3]=1u;goto L8c109e84; /* mov #0x1,r3 */
L8c109e84: sa_motion_publish_stop(ctx,0x8c109e84u); wr(m,r[4],r[3],1);goto L8c109e86; /* mov.b r3,@r4 */
L8c109e86: sa_motion_publish_stop(ctx,0x8c109e86u); r[0]=rd(m,r[15],4);goto L8c109e88; /* mov.l @r15,r0 */
L8c109e88: sa_motion_publish_stop(ctx,0x8c109e88u); wr(m,r[4]+0x1u,r[0],1);goto L8c109e8a; /* mov.b r0,@(0x1,r4) */
L8c109e8a: sa_motion_publish_stop(ctx,0x8c109e8au); r[0]=sx(m,r[15]+0x8u,1);goto L8c109e8c; /* mov.b @(0x8,r15),r0 */
L8c109e8c: sa_motion_publish_stop(ctx,0x8c109e8cu); wr(m,r[4]+0x2u,r[0],1);goto L8c109e8e; /* mov.b r0,@(0x2,r4) */
L8c109e8e: sa_motion_publish_stop(ctx,0x8c109e8eu); r[0]=sx(m,r[15]+0x4u,1);goto L8c109e90; /* mov.b @(0x4,r15),r0 */
L8c109e90: sa_motion_publish_stop(ctx,0x8c109e90u); wr(m,r[4]+0x3u,r[0],1);goto L8c109e92; /* mov.b r0,@(0x3,r4) */
L8c109e92: sa_motion_publish_stop(ctx,0x8c109e92u); r[0]=rd(m,r[15]+0x18u,4);goto L8c109e94; /* mov.l @(0x18,r15),r0 */
L8c109e94: sa_motion_publish_stop(ctx,0x8c109e96u); wr(m,r[4]+0x4u,r[0],1);goto L8c109ea4; /* bra 0x8c109ea4 */
L8c109e98: sa_motion_publish_stop(ctx,0x8c109e98u); r[4]=r[14];goto L8c109e9a; /* mov r14,r4 */
L8c109e9a: sa_motion_publish_stop(ctx,0x8c109e9au); r[15]+=16u;goto L8c109e9c; /* add #0x10,r15 */
L8c109e9c: sa_motion_publish_stop(ctx,0x8c109e9cu); *pr=rd(m,r[15],4);r[15]+=4;goto L8c109e9e; /* lds.l @r15+,PR */
L8c109e9e: sa_motion_publish_stop(ctx,0x8c109e9eu); r[2]=rd(m,0x8c109f44u,4);goto L8c109ea0; /* mov.l 0x8c109f44,r2 */
L8c109ea0: sa_motion_publish_stop(ctx,0x8c109ea2u); target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;return 0; /* jmp @r2 */
L8c109ea4: sa_motion_publish_stop(ctx,0x8c109ea4u); r[15]+=16u;goto L8c109ea6; /* add #0x10,r15 */
L8c109ea6: sa_motion_publish_stop(ctx,0x8c109ea6u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c109ea8; /* lds.l @r15+,PR */
L8c109ea8: sa_motion_publish_stop(ctx,0x8c109eaau); r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_notification_candidate_ready_8c64bed8(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call){
 (void)fpul;(void)macl;(void)f;
 int status;uint32_t target;
 goto L8c64bed8;
L8c64bed8: sa_motion_publish_stop(ctx,0x8c64bed8u); r[15]-=4;wr(m,r[15],*pr,4);goto L8c64beda; /* sts.l PR,@-r15 */
L8c64beda: sa_motion_publish_stop(ctx,0x8c64bedau); r[15]+=4294967292u;goto L8c64bedc; /* add #-0x4,r15 */
L8c64bedc: sa_motion_publish_stop(ctx,0x8c64bedcu); r[3]=rd(m,0x8c64bfd8u,4);goto L8c64bede; /* mov.l 0x8c64bfd8,r3 */
L8c64bede: sa_motion_publish_stop(ctx,0x8c64bee0u); target=r[3];*pr=0x8c64bee2u;wr(m,r[15],r[4],4);if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c64bee2; /* jsr @r3 */
L8c64bee2: sa_motion_publish_stop(ctx,0x8c64bee2u); r[4]=r[0];goto L8c64bee4; /* mov r0,r4 */
L8c64bee4: sa_motion_publish_stop(ctx,0x8c64bee4u); r[0]=rd(m,r[4],4);goto L8c64bee6; /* mov.l @r4,r0 */
L8c64bee6: sa_motion_publish_stop(ctx,0x8c64bee6u); r[3]=sx(m,0x8c64bfd4u,2);goto L8c64bee8; /* mov.w 0x8c64bfd4,r3 */
L8c64bee8: sa_motion_publish_stop(ctx,0x8c64bee8u); *sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c64beea; /* tst r3,r0 */
L8c64beea: sa_motion_publish_stop(ctx,0x8c64beeau); if((*sr&1u))goto L8c64bef4;goto L8c64beec; /* bt 0x8c64bef4 */
L8c64beec: sa_motion_publish_stop(ctx,0x8c64beecu); r[15]+=4u;goto L8c64beee; /* add #0x4,r15 */
L8c64beee: sa_motion_publish_stop(ctx,0x8c64beeeu); *pr=rd(m,r[15],4);r[15]+=4;goto L8c64bef0; /* lds.l @r15+,PR */
L8c64bef0: sa_motion_publish_stop(ctx,0x8c64bef2u); r[0]=1u;return 0; /* rts */
L8c64bef4: sa_motion_publish_stop(ctx,0x8c64bef4u); r[0]=0u;goto L8c64bef6; /* mov #0x0,r0 */
L8c64bef6: sa_motion_publish_stop(ctx,0x8c64bef6u); r[15]+=4u;goto L8c64bef8; /* add #0x4,r15 */
L8c64bef8: sa_motion_publish_stop(ctx,0x8c64bef8u); *pr=rd(m,r[15],4);r[15]+=4;goto L8c64befa; /* lds.l @r15+,PR */
L8c64befa: sa_motion_publish_stop(ctx,0x8c64befcu); return 0; /* rts */
}
