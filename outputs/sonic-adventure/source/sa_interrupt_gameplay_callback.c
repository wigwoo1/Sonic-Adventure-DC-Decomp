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
/* Static C recovery of retail interrupt gameplay callback 8c051810..8c05187e.
 * Lower gameplay and device routines use the supplied dispatcher. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_interrupt_gameplay_callback(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c051810;
L8c051810: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c051812;
L8c051812: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c051814;
L8c051814: r[4]=rd(m,0x8c051884u,4); /* mov.l 0x8c051884,r4 */
 goto L8c051816;
L8c051816: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c051818;
L8c051818: r[3]+=4294967295u; /* add #-0x1,r3 */
 goto L8c05181a;
L8c05181a: r[2]=r[3]; /* mov r3,r2 */
 goto L8c05181c;
L8c05181c: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c05181e;
L8c05181e: take=!(*sr&1u);wr(m,r[4],r[3],4);if(take)goto L8c051870;goto L8c051822; /* bf/s 0x8c051870 */
 goto L8c051822;
L8c051822: r[2]=rd(m,0x8c0518d4u,4); /* mov.l 0x8c0518d4,r2 */
 goto L8c051824;
L8c051824: target=r[2];*pr=0x8c051828u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c051828;
L8c051828: r[4]=r[0]; /* mov r0,r4 */
 goto L8c05182a;
L8c05182a: r[3]=rd(m,0x8c0518d8u,4); /* mov.l 0x8c0518d8,r3 */
 goto L8c05182c;
L8c05182c: r[0]=r[4]&255u; /* extu.b r4,r0 */
 goto L8c05182e;
L8c05182e: r[4]=r[4]&65535u; /* extu.w r4,r4 */
 goto L8c051830;
L8c051830: wr(m,r[15]+0x4u,r[0],2); /* mov.w r0,@(0x4,r15) */
 goto L8c051832;
L8c051832: r[2]=4294967288u; /* mov #-0x8,r2 */
 goto L8c051834;
L8c051834: r[4]&=r[3]; /* and r3,r4 */
 goto L8c051836;
L8c051836: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[4]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[4]=n?(uint32_t)((int32_t)r[4]>>n):((r[4]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r4 */
 goto L8c051838;
L8c051838: r[2]=rd(m,0x8c0518dcu,4); /* mov.l 0x8c0518dc,r2 */
 goto L8c05183a;
L8c05183a: wr(m,r[15],r[4],2); /* mov.w r4,@r15 */
 goto L8c05183c;
L8c05183c: r[0]=rd(m,r[2],4); /* mov.l @r2,r0 */
 goto L8c05183e;
L8c05183e: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c051840;
L8c051840: take=(*sr&1u);r[4]=r[0];if(take)goto L8c051850;goto L8c051844; /* bt/s 0x8c051850 */
 goto L8c051844;
L8c051844: r[0]=r[4]; /* mov r4,r0 */
 goto L8c051846;
L8c051846: *sr=(*sr&~1u)|(r[0]==0x5u); /* cmp/eq #0x5,r0 */
 goto L8c051848;
L8c051848: if((*sr&1u))goto L8c051850; /* bt 0x8c051850 */
 goto L8c05184a;
L8c05184a: r[0]=r[4]; /* mov r4,r0 */
 goto L8c05184c;
L8c05184c: *sr=(*sr&~1u)|(r[0]==0x9u); /* cmp/eq #0x9,r0 */
 goto L8c05184e;
L8c05184e: if(!(*sr&1u))goto L8c05186a; /* bf 0x8c05186a */
 goto L8c051850;
L8c051850: r[0]=sx(m,r[15],2); /* mov.w @r15,r0 */
 goto L8c051852;
L8c051852: r[3]=rd(m,0x8c0518e0u,4); /* mov.l 0x8c0518e0,r3 */
 goto L8c051854;
L8c051854: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c051856;
L8c051856: r[0]<<=2; /* shll2 r0 */
 goto L8c051858;
L8c051858: r[0]<<=2; /* shll2 r0 */
 goto L8c05185a;
L8c05185a: *sr=(*sr&~1u)|(r[0]>>31);r[0]<<=1; /* shll r0 */
 goto L8c05185c;
L8c05185c: r[3]+=r[0]; /* add r0,r3 */
 goto L8c05185e;
L8c05185e: r[0]=sx(m,r[15]+0x4u,2); /* mov.w @(0x4,r15),r0 */
 goto L8c051860;
L8c051860: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c051862;
L8c051862: r[0]<<=2; /* shll2 r0 */
 goto L8c051864;
L8c051864: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c051866;
L8c051866: target=r[2];*pr=0x8c05186au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c05186a;
L8c05186a: r[1]=rd(m,0x8c0518e4u,4); /* mov.l 0x8c0518e4,r1 */
 goto L8c05186c;
L8c05186c: target=r[1];*pr=0x8c051870u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c051870;
L8c051870: r[3]=rd(m,0x8c0518e8u,4); /* mov.l 0x8c0518e8,r3 */
 goto L8c051872;
L8c051872: target=r[3];*pr=0x8c051876u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c051876;
L8c051876: r[15]+=8u; /* add #0x8,r15 */
 goto L8c051878;
L8c051878: r[1]=rd(m,0x8c0518ecu,4); /* mov.l 0x8c0518ec,r1 */
 goto L8c05187a;
L8c05187a: target=r[1];*pr=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r1 */
}
