#include "fpu_cleanup_transfer.h"
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v,unsigned n){for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
/* Static C recovery of retail position task callback 8c07d560..8c07d5a8.
 * Updates payload timer and linked object flag; task removal remains a service.
 * Integer instructions preserve the FPU state. */
SA_STAGE_EXPORT int sa_position_task_callback(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c07d560;
L8c07d560: r[5]=rd(m,r[4]+0x20u,4); /* mov.l @(0x20,r4),r5 */
 goto L8c07d562;
L8c07d562: r[3]=120u; /* mov #0x78,r3 */
 goto L8c07d564;
L8c07d564: r[7]=rd(m,0x8c07d6a0u,4); /* mov.l 0x8c07d6a0,r7 */
 goto L8c07d566;
L8c07d566: r[0]=sx(m,r[5]+0x8u,1); /* mov.b @(0x8,r5),r0 */
 goto L8c07d568;
L8c07d568: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c07d56a;
L8c07d56a: r[0]<<=2; /* shll2 r0 */
 goto L8c07d56c;
L8c07d56c: r[6]=rd(m,r[7]+r[0],4); /* mov.l @(r0,r7),r6 */
 goto L8c07d56e;
L8c07d56e: r[0]=sx(m,r[5]+0x6u,2); /* mov.w @(0x6,r5),r0 */
 goto L8c07d570;
L8c07d570: r[0]+=1u; /* add #0x1,r0 */
 goto L8c07d572;
L8c07d572: wr(m,r[5]+0x6u,r[0],2); /* mov.w r0,@(0x6,r5) */
 goto L8c07d574;
L8c07d574: r[0]=sx(m,r[5]+0x6u,2); /* mov.w @(0x6,r5),r0 */
 goto L8c07d576;
L8c07d576: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c07d578;
L8c07d578: *sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[3]); /* cmp/gt r3,r0 */
 goto L8c07d57a;
L8c07d57a: if(!(*sr&1u))goto L8c07d592; /* bf 0x8c07d592 */
 goto L8c07d57c;
L8c07d57c: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c07d57e;
L8c07d57e: if((*sr&1u))goto L8c07d58c; /* bt 0x8c07d58c */
 goto L8c07d580;
L8c07d580: r[2]=4294967168u; /* mov #-0x80,r2 */
 goto L8c07d582;
L8c07d582: r[0]=sx(m,r[6]+0x6u,2); /* mov.w @(0x6,r6),r0 */
 goto L8c07d584;
L8c07d584: r[2]<<=8; /* shll8 r2 */
 goto L8c07d586;
L8c07d586: r[2]=~r[2]; /* not r2,r2 */
 goto L8c07d588;
L8c07d588: r[0]&=r[2]; /* and r2,r0 */
 goto L8c07d58a;
L8c07d58a: wr(m,r[6]+0x6u,r[0],2); /* mov.w r0,@(0x6,r6) */
 goto L8c07d58c;
L8c07d58c: r[3]=rd(m,0x8c07d6a4u,4); /* mov.l 0x8c07d6a4,r3 */
 goto L8c07d58e;
L8c07d58e: target=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
L8c07d592: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c07d594;
L8c07d594: if((*sr&1u))goto L8c07d5a6; /* bt 0x8c07d5a6 */
 goto L8c07d596;
L8c07d596: r[0]=sx(m,r[5]+0x8u,1); /* mov.b @(0x8,r5),r0 */
 goto L8c07d598;
L8c07d598: r[1]=rd(m,0x8c07d6a8u,4); /* mov.l 0x8c07d6a8,r1 */
 goto L8c07d59a;
L8c07d59a: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c07d59c;
L8c07d59c: r[0]<<=2; /* shll2 r0 */
 goto L8c07d59e;
L8c07d59e: r[2]=rd(m,r[7]+r[0],4); /* mov.l @(r0,r7),r2 */
 goto L8c07d5a0;
L8c07d5a0: r[0]=sx(m,r[2]+0x6u,2); /* mov.w @(0x6,r2),r0 */
 goto L8c07d5a2;
L8c07d5a2: r[0]|=r[1]; /* or r1,r0 */
 goto L8c07d5a4;
L8c07d5a4: wr(m,r[2]+0x6u,r[0],2); /* mov.w r0,@(0x6,r2) */
 goto L8c07d5a6;
L8c07d5a6: return 0; /* rts */
}
