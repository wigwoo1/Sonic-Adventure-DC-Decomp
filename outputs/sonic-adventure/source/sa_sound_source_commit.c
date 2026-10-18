#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_cache_invalidate(void *,uint32_t);
int sa_sh4_cache_purge(void *,uint32_t);
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);
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
/* Retail source commit65e58a..65e644, explicit SH4 ABI.
 * Ring mirror/wrap copies and per-mode counters. Scope: sound_source_commit_verification.json. */
SA_STAGE_EXPORT int sa_sound_source_commit(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e58a;
L8c65e58a: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65e58c;
L8c65e58c: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65e58e;
L8c65e58e: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65e590;
L8c65e590: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65e592;
L8c65e592: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c65e594;
L8c65e594: r[13]=r[6]; /* mov r6,r13 */
 goto L8c65e596;
L8c65e596: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c65e598;
L8c65e598: *pr=0x8c65e59cu;r[14]=r[4];status=call(ctx,0x8c65e368u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65e368 */
 goto L8c65e59c;
L8c65e59c: wr(m,r[15]+0x4u,r[0],4); /* mov.l r0,@(0x4,r15) */
 goto L8c65e59e;
L8c65e59e: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c65e5a0;
L8c65e5a0: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c65e5a2;
L8c65e5a2: if(!(*sr&1u))goto L8c65e610; /* bf 0x8c65e610 */
 goto L8c65e5a4;
L8c65e5a4: r[2]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r2 */
 goto L8c65e5a6;
L8c65e5a6: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e5a8;
L8c65e5a8: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65e5aa;
L8c65e5aa: wr(m,r[14]+0xcu,r[2],4); /* mov.l r2,@(0xc,r14) */
 goto L8c65e5ac;
L8c65e5ac: r[12]=rd(m,r[13],4); /* mov.l @r13,r12 */
 goto L8c65e5ae;
L8c65e5ae: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e5b0;
L8c65e5b0: r[2]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r2 */
 goto L8c65e5b2;
L8c65e5b2: r[12]-=r[3]; /* sub r3,r12 */
 goto L8c65e5b4;
L8c65e5b4: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[2]); /* cmp/ge r2,r12 */
 goto L8c65e5b6;
L8c65e5b6: if((*sr&1u))goto L8c65e5da; /* bt 0x8c65e5da */
 goto L8c65e5b8;
L8c65e5b8: r[5]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r5 */
 goto L8c65e5ba;
L8c65e5ba: r[4]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r4 */
 goto L8c65e5bc;
L8c65e5bc: r[5]-=r[12]; /* sub r12,r5 */
 goto L8c65e5be;
L8c65e5be: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]); /* cmp/ge r5,r4 */
 goto L8c65e5c0;
L8c65e5c0: if((*sr&1u))goto L8c65e5c6; /* bt 0x8c65e5c6 */
 goto L8c65e5c2;
L8c65e5c2: r[1]=r[4];goto L8c65e5c8; /* bra 0x8c65e5c8 */
 goto L8c65e5c6;
L8c65e5c6: r[1]=r[5]; /* mov r5,r1 */
 goto L8c65e5c8;
L8c65e5c8: r[2]=rd(m,0x8c65e6ecu,4); /* mov.l 0x8c65e6ec,r2 */
 goto L8c65e5ca;
L8c65e5ca: r[6]=r[1]; /* mov r1,r6 */
 goto L8c65e5cc;
L8c65e5cc: wr(m,r[15],r[1],4); /* mov.l r1,@r15 */
 goto L8c65e5ce;
L8c65e5ce: r[4]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r4 */
 goto L8c65e5d0;
L8c65e5d0: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e5d2;
L8c65e5d2: r[5]=rd(m,r[13],4); /* mov.l @r13,r5 */
 goto L8c65e5d4;
L8c65e5d4: r[4]+=r[12]; /* add r12,r4 */
 goto L8c65e5d6;
L8c65e5d6: target=r[2];*pr=0x8c65e5dau;r[4]+=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65e5da;
L8c65e5da: r[12]=rd(m,r[13],4); /* mov.l @r13,r12 */
 goto L8c65e5dc;
L8c65e5dc: r[3]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r3 */
 goto L8c65e5de;
L8c65e5de: r[2]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r2 */
 goto L8c65e5e0;
L8c65e5e0: r[12]-=r[3]; /* sub r3,r12 */
 goto L8c65e5e2;
L8c65e5e2: r[3]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r3 */
 goto L8c65e5e4;
L8c65e5e4: r[12]+=r[2]; /* add r2,r12 */
 goto L8c65e5e6;
L8c65e5e6: *sr=(*sr&~1u)|((int32_t)r[12]>(int32_t)r[3]); /* cmp/gt r3,r12 */
 goto L8c65e5e8;
L8c65e5e8: if(!(*sr&1u))goto L8c65e634; /* bf 0x8c65e634 */
 goto L8c65e5ea;
L8c65e5ea: r[3]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r3 */
 goto L8c65e5ec;
L8c65e5ec: r[5]=r[12]; /* mov r12,r5 */
 goto L8c65e5ee;
L8c65e5ee: r[4]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r4 */
 goto L8c65e5f0;
L8c65e5f0: r[5]-=r[3]; /* sub r3,r5 */
 goto L8c65e5f2;
L8c65e5f2: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]); /* cmp/ge r5,r4 */
 goto L8c65e5f4;
L8c65e5f4: if((*sr&1u))goto L8c65e5fa; /* bt 0x8c65e5fa */
 goto L8c65e5f6;
L8c65e5f6: r[13]=r[4];goto L8c65e5fc; /* bra 0x8c65e5fc */
 goto L8c65e5fa;
L8c65e5fa: r[13]=r[5]; /* mov r5,r13 */
 goto L8c65e5fc;
L8c65e5fc: r[5]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r5 */
 goto L8c65e5fe;
L8c65e5fe: r[12]-=r[13]; /* sub r13,r12 */
 goto L8c65e600;
L8c65e600: r[3]=rd(m,0x8c65e6ecu,4); /* mov.l 0x8c65e6ec,r3 */
 goto L8c65e602;
L8c65e602: r[6]=r[13]; /* mov r13,r6 */
 goto L8c65e604;
L8c65e604: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c65e606;
L8c65e606: r[5]+=r[12]; /* add r12,r5 */
 goto L8c65e608;
L8c65e608: target=r[3];*pr=0x8c65e60cu;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65e60c;
L8c65e60c: goto L8c65e634; /* bra 0x8c65e634 */
 goto L8c65e610;
L8c65e610: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65e612;
L8c65e612: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e614;
L8c65e614: if(!(*sr&1u))goto L8c65e620; /* bf 0x8c65e620 */
 goto L8c65e616;
L8c65e616: r[2]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r2 */
 goto L8c65e618;
L8c65e618: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e61a;
L8c65e61a: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65e61c;
L8c65e61c: wr(m,r[14]+0x10u,r[2],4);goto L8c65e634; /* bra 0x8c65e634 */
 goto L8c65e620;
L8c65e620: r[4]=0u; /* mov #0x0,r4 */
 goto L8c65e622;
L8c65e622: wr(m,r[13]+0x4u,r[4],4); /* mov.l r4,@(0x4,r13) */
 goto L8c65e624;
L8c65e624: wr(m,r[13],r[4],4); /* mov.l r4,@r13 */
 goto L8c65e626;
L8c65e626: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c65e628;
L8c65e628: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e62a;
L8c65e62a: if((*sr&1u))goto L8c65e634; /* bt 0x8c65e634 */
 goto L8c65e62c;
L8c65e62c: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c65e62e;
L8c65e62e: r[5]=4294967293u; /* mov #-0x3,r5 */
 goto L8c65e630;
L8c65e630: target=r[3];*pr=0x8c65e634u;r[4]=rd(m,r[14]+0x2cu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65e634;
L8c65e634: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c65e636;
L8c65e636: r[4]=r[14]; /* mov r14,r4 */
 goto L8c65e638;
L8c65e638: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65e63a;
L8c65e63a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e63c;
L8c65e63c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65e63e;
L8c65e63e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65e640;
L8c65e640: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c65e380u,m,f,sr,r,pr); /* bra 0x8c65e380 */
}
