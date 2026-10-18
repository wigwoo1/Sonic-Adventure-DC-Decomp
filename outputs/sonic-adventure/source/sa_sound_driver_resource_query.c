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
/* Static C recovery of retail Tikal helper 8c66863c..8c66869a.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_resource_query(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66863c;
L8c66863c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66863e;
L8c66863e: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c668640;
L8c668640: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c668642;
L8c668642: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c668644;
L8c668644: r[14]=r[4]; /* mov r4,r14 */
 goto L8c668646;
L8c668646: r[3]=r[14]; /* mov r14,r3 */
 goto L8c668648;
L8c668648: r[3]+=16u; /* add #0x10,r3 */
 goto L8c66864a;
L8c66864a: r[2]=rd(m,0x8c6686f8u,4); /* mov.l 0x8c6686f8,r2 */
 goto L8c66864c;
L8c66864c: wr(m,r[15],r[5],1); /* mov.b r5,@r15 */
 goto L8c66864e;
L8c66864e: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c668650;
L8c668650: wr(m,r[15]+0x8u,r[7],4); /* mov.l r7,@(0x8,r15) */
 goto L8c668652;
L8c668652: r[4]=sx(m,r[15],1); /* mov.b @r15,r4 */
 goto L8c668654;
L8c668654: r[4]<<=2; /* shll2 r4 */
 goto L8c668656;
L8c668656: wr(m,r[15]+0xcu,r[4],4); /* mov.l r4,@(0xc,r15) */
 goto L8c668658;
L8c668658: r[4]+=r[3]; /* add r3,r4 */
 goto L8c66865a;
L8c66865a: target=r[2];*pr=0x8c66865eu;r[4]=rd(m,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66865e;
L8c66865e: r[4]=rd(m,0x8c6686fcu,4); /* mov.l 0x8c6686fc,r4 */
 goto L8c668660;
L8c668660: r[3]=rd(m,0x8c668700u,4); /* mov.l 0x8c668700,r3 */
 goto L8c668662;
L8c668662: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c668664;
L8c668664: r[6]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r6 */
 goto L8c668666;
L8c668666: r[7]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r7 */
 goto L8c668668;
L8c668668: target=r[3];*pr=0x8c66866cu;r[5]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66866c;
L8c66866c: r[2]=r[14]; /* mov r14,r2 */
 goto L8c66866e;
L8c66866e: r[1]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r1 */
 goto L8c668670;
L8c668670: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c668672;
L8c668672: r[2]+=48u; /* add #0x30,r2 */
 goto L8c668674;
L8c668674: r[1]=rd(m,r[1],4); /* mov.l @r1,r1 */
 goto L8c668676;
L8c668676: r[13]=r[0]; /* mov r0,r13 */
 goto L8c668678;
L8c668678: r[2]+=r[3]; /* add r3,r2 */
 goto L8c66867a;
L8c66867a: wr(m,r[2],r[1],4); /* mov.l r1,@r2 */
 goto L8c66867c;
L8c66867c: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c66867e;
L8c66867e: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c668680;
L8c668680: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c668682;
L8c668682: take=(*sr&1u);wr(m,r[14]+0x38u,r[2],4);if(take)goto L8c66868e;goto L8c668686; /* bt/s 0x8c66868e */
 goto L8c668686;
L8c668686: r[2]=rd(m,0x8c6686c0u,4); /* mov.l 0x8c6686c0,r2 */
 goto L8c668688;
L8c668688: r[4]=rd(m,0x8c668704u,4); /* mov.l 0x8c668704,r4 */
 goto L8c66868a;
L8c66868a: target=r[2];*pr=0x8c66868eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66868e;
L8c66868e: r[0]=r[13]; /* mov r13,r0 */
 goto L8c668690;
L8c668690: r[15]+=16u; /* add #0x10,r15 */
 goto L8c668692;
L8c668692: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c668694;
L8c668694: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c668696;
L8c668696: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
