/* Generated from retail bytes by recover_main_dispatch_shared_check.py.
 * Inferred symbol, original-PC annotations and static native C control flow;
 * no opcode interpreter. 195 integer instructions, no external calls/FPU.
 * Valid 16 MiB RAM and original pointer preconditions are required. Memory
 * exceptions/MMIO are excluded. SR and stack writes remain explicit.
 * Host 0 means completion, with retail flag result in R0. The callback can
 * be null because this routine never calls it. PR/FPU/FPUL/MACL are untouched.
 */
#include "fpu_cleanup_transfer.h"
typedef int (*sa_check_service)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
SA_STAGE_EXPORT int sa_main_dispatch_shared_check_8c06c880(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,sa_check_service call){
 (void)f;(void)pr;(void)fpul;(void)macl;(void)ctx;(void)call;
 int take;
 goto L8c06c880;
L8c06c880: r[15]-=4;wr(m,r[15],r[14],4);goto L8c06c882; /* mov.l r14,@-r15 */
L8c06c882: r[15]-=4;wr(m,r[15],r[13],4);goto L8c06c884; /* mov.l r13,@-r15 */
L8c06c884: r[15]-=4;wr(m,r[15],r[12],4);goto L8c06c886; /* mov.l r12,@-r15 */
L8c06c886: r[15]+=4294967288u;goto L8c06c888; /* add #-0x8,r15 */
L8c06c888: r[0]=sx(m,r[4]+0x4u,2);goto L8c06c88a; /* mov.w @(0x4,r4),r0 */
L8c06c88a: r[7]=8u;goto L8c06c88c; /* mov #0x8,r7 */
L8c06c88c: r[7]<<=8;goto L8c06c88e; /* shll8 r7 */
L8c06c88e: r[14]=2u;goto L8c06c890; /* mov #0x2,r14 */
L8c06c890: r[13]=6u;goto L8c06c892; /* mov #0x6,r13 */
L8c06c892: *sr=(*sr&~1u)|!(r[7]&r[0]);goto L8c06c894; /* tst r7,r0 */
L8c06c894: r[12]=0u;goto L8c06c896; /* mov #0x0,r12 */
L8c06c896: r[13]<<=8;goto L8c06c898; /* shll8 r13 */
L8c06c898: r[14]<<=8;goto L8c06c89a; /* shll8 r14 */
L8c06c89a: take=(*sr&1u);r[5]=r[12];if(take)goto L8c06c944;goto L8c06c89e; /* bt/s 0x8c06c944 */
L8c06c89e: r[0]=sx(m,r[4]+0x8u,1);goto L8c06c8a0; /* mov.b @(0x8,r4),r0 */
L8c06c8a0: r[6]=r[0]&255u;goto L8c06c8a2; /* extu.b r0,r6 */
L8c06c8a2: r[0]=rd(m,0x8c06c9c0u,4);goto L8c06c8a4; /* mov.l 0x8c06c9c0,r0 */
L8c06c8a4: r[6]<<=2;goto L8c06c8a6; /* shll2 r6 */
L8c06c8a6: r[6]=rd(m,r[6]+r[0],4);goto L8c06c8a8; /* mov.l @(r0,r6),r6 */
L8c06c8a8: r[0]=104u;goto L8c06c8aa; /* mov #0x68,r0 */
L8c06c8aa: wr(m,r[15]+0x4u,r[6],4);goto L8c06c8ac; /* mov.l r6,@(0x4,r15) */
L8c06c8ac: r[6]=rd(m,r[6]+r[0],4);goto L8c06c8ae; /* mov.l @(r0,r6),r6 */
L8c06c8ae: wr(m,r[15],r[6],4);goto L8c06c8b0; /* mov.l r6,@r15 */
L8c06c8b0: r[3]=rd(m,r[15],4);goto L8c06c8b2; /* mov.l @r15,r3 */
L8c06c8b2: r[6]=rd(m,r[6]+0x20u,4);goto L8c06c8b4; /* mov.l @(0x20,r6),r6 */
L8c06c8b4: r[2]=rd(m,r[3]+0x24u,4);goto L8c06c8b6; /* mov.l @(0x24,r3),r2 */
L8c06c8b6: r[3]=r[2];goto L8c06c8b8; /* mov r2,r3 */
L8c06c8b8: *sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c06c8ba; /* tst r3,r3 */
L8c06c8ba: take=(*sr&1u);wr(m,r[15],r[2],4);if(take)goto L8c06c98e;goto L8c06c8be; /* bt/s 0x8c06c98e */
L8c06c8be: *sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c06c8c0; /* tst r6,r6 */
L8c06c8c0: if((*sr&1u))goto L8c06c98e;goto L8c06c8c2; /* bt 0x8c06c98e */
L8c06c8c2: r[2]=rd(m,r[6]+0x38u,4);goto L8c06c8c4; /* mov.l @(0x38,r6),r2 */
L8c06c8c4: *sr=(*sr&~1u)|!(r[2]&r[2]);goto L8c06c8c6; /* tst r2,r2 */
L8c06c8c6: if((*sr&1u))goto L8c06c98e;goto L8c06c8c8; /* bt 0x8c06c98e */
L8c06c8c8: r[0]=sx(m,r[6]+0x2u,1);goto L8c06c8ca; /* mov.b @(0x2,r6),r0 */
L8c06c8ca: wr(m,r[15],r[0],1);goto L8c06c8cc; /* mov.b r0,@r15 */
L8c06c8cc: r[0]=sx(m,r[4]+0x9u,1);goto L8c06c8ce; /* mov.b @(0x9,r4),r0 */
L8c06c8ce: r[6]=sx(m,r[15],1);goto L8c06c8d0; /* mov.b @r15,r6 */
L8c06c8d0: r[0]=r[0]&255u;goto L8c06c8d2; /* extu.b r0,r0 */
L8c06c8d2: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c06c8d4; /* cmp/eq #0x0,r0 */
L8c06c8d4: take=(*sr&1u);r[6]=r[6]&255u;if(take)goto L8c06c8f0;goto L8c06c8d8; /* bt/s 0x8c06c8f0 */
L8c06c8d8: *sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c06c8da; /* cmp/eq #0x2,r0 */
L8c06c8da: if((*sr&1u))goto L8c06c8f0;goto L8c06c8dc; /* bt 0x8c06c8f0 */
L8c06c8dc: *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c06c8de; /* cmp/eq #0x3,r0 */
L8c06c8de: if((*sr&1u))goto L8c06c8f0;goto L8c06c8e0; /* bt 0x8c06c8f0 */
L8c06c8e0: *sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c06c8e2; /* cmp/eq #0x5,r0 */
L8c06c8e2: if((*sr&1u))goto L8c06c8f0;goto L8c06c8e4; /* bt 0x8c06c8f0 */
L8c06c8e4: *sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c06c8e6; /* cmp/eq #0x6,r0 */
L8c06c8e6: if((*sr&1u))goto L8c06c8f0;goto L8c06c8e8; /* bt 0x8c06c8f0 */
L8c06c8e8: *sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c06c8ea; /* cmp/eq #0x7,r0 */
L8c06c8ea: if((*sr&1u))goto L8c06c91c;goto L8c06c8ec; /* bt 0x8c06c91c */
L8c06c8ec: goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c8f0: r[0]=r[6];goto L8c06c8f2; /* mov r6,r0 */
L8c06c8f2: *sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c06c8f4; /* cmp/eq #0x4,r0 */
L8c06c8f4: if(!(*sr&1u))goto L8c06c8fa;goto L8c06c8f6; /* bf 0x8c06c8fa */
L8c06c8f6: goto L8c06ca0c; /* bra 0x8c06ca0c */
L8c06c8fa: r[0]=r[6];goto L8c06c8fc; /* mov r6,r0 */
L8c06c8fc: *sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c06c8fe; /* cmp/eq #0x5,r0 */
L8c06c8fe: if(!(*sr&1u))goto L8c06c904;goto L8c06c900; /* bf 0x8c06c904 */
L8c06c900: goto L8c06c9ec; /* bra 0x8c06c9ec */
L8c06c904: r[0]=r[6];goto L8c06c906; /* mov r6,r0 */
L8c06c906: *sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c06c908; /* cmp/eq #0x6,r0 */
L8c06c908: if(!(*sr&1u))goto L8c06c90e;goto L8c06c90a; /* bf 0x8c06c90e */
L8c06c90a: goto L8c06c9f8; /* bra 0x8c06c9f8 */
L8c06c90e: r[0]=r[6];goto L8c06c910; /* mov r6,r0 */
L8c06c910: *sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c06c912; /* cmp/eq #0x7,r0 */
L8c06c912: if((*sr&1u))goto L8c06c918;goto L8c06c914; /* bt 0x8c06c918 */
L8c06c914: goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c918: r[5]=r[7];goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c91c: r[0]=r[6];goto L8c06c91e; /* mov r6,r0 */
L8c06c91e: *sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c06c920; /* cmp/eq #0x4,r0 */
L8c06c920: if(!(*sr&1u))goto L8c06c926;goto L8c06c922; /* bf 0x8c06c926 */
L8c06c922: goto L8c06ca0c; /* bra 0x8c06ca0c */
L8c06c926: *sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c06c928; /* cmp/eq #0x5,r0 */
L8c06c928: if(!(*sr&1u))goto L8c06c92e;goto L8c06c92a; /* bf 0x8c06c92e */
L8c06c92a: goto L8c06ca1a; /* bra 0x8c06ca1a */
L8c06c92e: r[0]=r[6];goto L8c06c930; /* mov r6,r0 */
L8c06c930: *sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c06c932; /* cmp/eq #0x6,r0 */
L8c06c932: if(!(*sr&1u))goto L8c06c938;goto L8c06c934; /* bf 0x8c06c938 */
L8c06c934: goto L8c06ca1a; /* bra 0x8c06ca1a */
L8c06c938: *sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c06c93a; /* cmp/eq #0x7,r0 */
L8c06c93a: if((*sr&1u))goto L8c06c940;goto L8c06c93c; /* bt 0x8c06c940 */
L8c06c93c: goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c940: r[5]=r[7];goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c944: r[6]=rd(m,r[4]+0x38u,4);goto L8c06c946; /* mov.l @(0x38,r4),r6 */
L8c06c946: r[3]=2u;goto L8c06c948; /* mov #0x2,r3 */
L8c06c948: r[0]=sx(m,r[6]+0x2u,2);goto L8c06c94a; /* mov.w @(0x2,r6),r0 */
L8c06c94a: r[6]=r[0]&65535u;goto L8c06c94c; /* extu.w r0,r6 */
L8c06c94c: *sr=(*sr&~1u)|!(r[6]&r[3]);goto L8c06c94e; /* tst r6,r3 */
L8c06c94e: if(!(*sr&1u))goto L8c06c954;goto L8c06c950; /* bf 0x8c06c954 */
L8c06c950: goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c954: r[0]=sx(m,r[4]+0x4u,2);goto L8c06c956; /* mov.w @(0x4,r4),r0 */
L8c06c956: *sr=(*sr&~1u)|!(r[0]&0x4u);goto L8c06c958; /* tst #0x4,r0 */
L8c06c958: if((*sr&1u))goto L8c06c95e;goto L8c06c95a; /* bt 0x8c06c95e */
L8c06c95a: goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c95e: r[2]=8u;goto L8c06c960; /* mov #0x8,r2 */
L8c06c960: *sr=(*sr&~1u)|!(r[6]&r[2]);goto L8c06c962; /* tst r6,r2 */
L8c06c962: if(!(*sr&1u))goto L8c06c968;goto L8c06c964; /* bf 0x8c06c968 */
L8c06c964: goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c968: r[6]=rd(m,r[4]+0x38u,4);goto L8c06c96a; /* mov.l @(0x38,r4),r6 */
L8c06c96a: r[6]=rd(m,r[6]+0x18u,4);goto L8c06c96c; /* mov.l @(0x18,r6),r6 */
L8c06c96c: *sr=(*sr&~1u)|!(r[6]&r[6]);goto L8c06c96e; /* tst r6,r6 */
L8c06c96e: if((*sr&1u))goto L8c06ca24;goto L8c06c970; /* bt 0x8c06ca24 */
L8c06c970: r[0]=rd(m,r[6]+0x10u,4);goto L8c06c972; /* mov.l @(0x10,r6),r0 */
L8c06c972: r[0]=rd(m,r[0]+0x20u,4);goto L8c06c974; /* mov.l @(0x20,r0),r0 */
L8c06c974: r[0]=sx(m,r[0]+0x4u,2);goto L8c06c976; /* mov.w @(0x4,r0),r0 */
L8c06c976: *sr=(*sr&~1u)|!(r[0]&0x40u);goto L8c06c978; /* tst #0x40,r0 */
L8c06c978: if((*sr&1u))goto L8c06ca24;goto L8c06c97a; /* bt 0x8c06ca24 */
L8c06c97a: r[2]=rd(m,r[6]+0x10u,4);goto L8c06c97c; /* mov.l @(0x10,r6),r2 */
L8c06c97c: r[3]=16u;goto L8c06c97e; /* mov #0x10,r3 */
L8c06c97e: r[3]<<=8;goto L8c06c980; /* shll8 r3 */
L8c06c980: r[6]=rd(m,r[2]+0x20u,4);goto L8c06c982; /* mov.l @(0x20,r2),r6 */
L8c06c982: r[0]=sx(m,r[6]+0x2u,1);goto L8c06c984; /* mov.b @(0x2,r6),r0 */
L8c06c984: wr(m,r[15],r[0],1);goto L8c06c986; /* mov.b r0,@r15 */
L8c06c986: r[0]=sx(m,r[6]+0x4u,2);goto L8c06c988; /* mov.w @(0x4,r6),r0 */
L8c06c988: r[6]=(uint32_t)(int32_t)(int16_t)r[0];goto L8c06c98a; /* exts.w r0,r6 */
L8c06c98a: *sr=(*sr&~1u)|!(r[3]&r[6]);goto L8c06c98c; /* tst r3,r6 */
L8c06c98c: if((*sr&1u))goto L8c06c992;goto L8c06c98e; /* bt 0x8c06c992 */
L8c06c98e: r[0]=0u;goto L8c06ca26; /* bra 0x8c06ca26 */
L8c06c992: r[0]=sx(m,r[4]+0x9u,1);goto L8c06c994; /* mov.b @(0x9,r4),r0 */
L8c06c994: r[6]=sx(m,r[15],1);goto L8c06c996; /* mov.b @r15,r6 */
L8c06c996: r[0]=r[0]&255u;goto L8c06c998; /* extu.b r0,r0 */
L8c06c998: *sr=(*sr&~1u)|(r[0]==0x00000000u);goto L8c06c99a; /* cmp/eq #0x0,r0 */
L8c06c99a: take=(*sr&1u);r[6]=r[6]&255u;if(take)goto L8c06c9e0;goto L8c06c99e; /* bt/s 0x8c06c9e0 */
L8c06c99e: *sr=(*sr&~1u)|(r[0]==0x00000002u);goto L8c06c9a0; /* cmp/eq #0x2,r0 */
L8c06c9a0: if((*sr&1u))goto L8c06c9e0;goto L8c06c9a2; /* bt 0x8c06c9e0 */
L8c06c9a2: *sr=(*sr&~1u)|(r[0]==0x00000003u);goto L8c06c9a4; /* cmp/eq #0x3,r0 */
L8c06c9a4: if((*sr&1u))goto L8c06c9e0;goto L8c06c9a6; /* bt 0x8c06c9e0 */
L8c06c9a6: *sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c06c9a8; /* cmp/eq #0x5,r0 */
L8c06c9a8: if((*sr&1u))goto L8c06c9e0;goto L8c06c9aa; /* bt 0x8c06c9e0 */
L8c06c9aa: *sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c06c9ac; /* cmp/eq #0x6,r0 */
L8c06c9ac: if((*sr&1u))goto L8c06c9e0;goto L8c06c9ae; /* bt 0x8c06c9e0 */
L8c06c9ae: *sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c06c9b0; /* cmp/eq #0x7,r0 */
L8c06c9b0: if((*sr&1u))goto L8c06ca06;goto L8c06c9b2; /* bt 0x8c06ca06 */
L8c06c9b2: goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c9e0: r[0]=r[6];goto L8c06c9e2; /* mov r6,r0 */
L8c06c9e2: *sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c06c9e4; /* cmp/eq #0x4,r0 */
L8c06c9e4: if((*sr&1u))goto L8c06ca0c;goto L8c06c9e6; /* bt 0x8c06ca0c */
L8c06c9e6: r[0]=r[6];goto L8c06c9e8; /* mov r6,r0 */
L8c06c9e8: *sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c06c9ea; /* cmp/eq #0x5,r0 */
L8c06c9ea: if(!(*sr&1u))goto L8c06c9f4;goto L8c06c9ec; /* bf 0x8c06c9f4 */
L8c06c9ec: r[5]=4u;goto L8c06c9ee; /* mov #0x4,r5 */
L8c06c9ee: r[5]<<=8;goto L8c06c9f0; /* shll8 r5 */
L8c06c9f0: goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c9f4: *sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c06c9f6; /* cmp/eq #0x6,r0 */
L8c06c9f6: if(!(*sr&1u))goto L8c06c9fc;goto L8c06c9f8; /* bf 0x8c06c9fc */
L8c06c9f8: r[5]=r[12];goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06c9fc: r[0]=r[6];goto L8c06c9fe; /* mov r6,r0 */
L8c06c9fe: *sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c06ca00; /* cmp/eq #0x7,r0 */
L8c06ca00: if(!(*sr&1u))goto L8c06ca24;goto L8c06ca02; /* bf 0x8c06ca24 */
L8c06ca02: r[5]=r[7];goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06ca06: r[0]=r[6];goto L8c06ca08; /* mov r6,r0 */
L8c06ca08: *sr=(*sr&~1u)|(r[0]==0x00000004u);goto L8c06ca0a; /* cmp/eq #0x4,r0 */
L8c06ca0a: if(!(*sr&1u))goto L8c06ca10;goto L8c06ca0c; /* bf 0x8c06ca10 */
L8c06ca0c: r[5]=r[14];goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06ca10: *sr=(*sr&~1u)|(r[0]==0x00000005u);goto L8c06ca12; /* cmp/eq #0x5,r0 */
L8c06ca12: if((*sr&1u))goto L8c06ca1a;goto L8c06ca14; /* bt 0x8c06ca1a */
L8c06ca14: r[0]=r[6];goto L8c06ca16; /* mov r6,r0 */
L8c06ca16: *sr=(*sr&~1u)|(r[0]==0x00000006u);goto L8c06ca18; /* cmp/eq #0x6,r0 */
L8c06ca18: if(!(*sr&1u))goto L8c06ca1e;goto L8c06ca1a; /* bf 0x8c06ca1e */
L8c06ca1a: r[5]=r[13];goto L8c06ca24; /* bra 0x8c06ca24 */
L8c06ca1e: *sr=(*sr&~1u)|(r[0]==0x00000007u);goto L8c06ca20; /* cmp/eq #0x7,r0 */
L8c06ca20: if(!(*sr&1u))goto L8c06ca24;goto L8c06ca22; /* bf 0x8c06ca24 */
L8c06ca22: r[5]=r[7];goto L8c06ca24; /* mov r7,r5 */
L8c06ca24: r[0]=r[5];goto L8c06ca26; /* mov r5,r0 */
L8c06ca26: r[15]+=8u;goto L8c06ca28; /* add #0x8,r15 */
L8c06ca28: r[12]=rd(m,r[15],4);r[15]+=4;goto L8c06ca2a; /* mov.l @r15+,r12 */
L8c06ca2a: r[13]=rd(m,r[15],4);r[15]+=4;goto L8c06ca2c; /* mov.l @r15+,r13 */
L8c06ca2c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
