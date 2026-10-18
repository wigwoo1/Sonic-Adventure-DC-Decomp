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
/* Static C recovery of retail Tikal helper 8c66854e..8c6685bc.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_fixed_block_transfer(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66854e;
L8c66854e: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c668550;
L8c668550: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c668552;
L8c668552: r[15]+=4294967276u; /* add #-0x14,r15 */
 goto L8c668554;
L8c668554: r[0]=r[5]; /* mov r5,r0 */
 goto L8c668556;
L8c668556: r[3]=0u; /* mov #0x0,r3 */
 goto L8c668558;
L8c668558: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c66855a;
L8c66855a: wr(m,r[15]+0xcu,r[0],1); /* mov.b r0,@(0xc,r15) */
 goto L8c66855c;
L8c66855c: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c66855e;
L8c66855e: r[6]=32u; /* mov #0x20,r6 */
 goto L8c668560;
L8c668560: wr(m,r[15]+0x10u,r[7],4); /* mov.l r7,@(0x10,r15) */
 goto L8c668562;
L8c668562: r[7]=r[3]; /* mov r3,r7 */
 goto L8c668564;
L8c668564: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c668566;
L8c668566: r[0]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c668568;
L8c668568: r[1]=rd(m,0x8c6686e0u,4); /* mov.l 0x8c6686e0,r1 */
 goto L8c66856a;
L8c66856a: r[2]=r[0]; /* mov r0,r2 */
 goto L8c66856c;
L8c66856c: r[0]=16u; /* mov #0x10,r0 */
 goto L8c66856e;
L8c66856e: r[0]=sx(m,r[15]+r[0],1); /* mov.b @(r0,r15),r0 */
 goto L8c668570;
L8c668570: r[2]+=32u; /* add #0x20,r2 */
 goto L8c668572;
L8c668572: r[5]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r5 */
 goto L8c668574;
L8c668574: r[4]=r[0]; /* mov r0,r4 */
 goto L8c668576;
L8c668576: r[4]<<=2; /* shll2 r4 */
 goto L8c668578;
L8c668578: wr(m,r[15]+0xcu,r[4],4); /* mov.l r4,@(0xc,r15) */
 goto L8c66857a;
L8c66857a: r[4]+=r[2]; /* add r2,r4 */
 goto L8c66857c;
L8c66857c: target=r[1];*pr=0x8c668580u;r[4]=rd(m,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c668580;
L8c668580: r[14]=r[0]; /* mov r0,r14 */
 goto L8c668582;
L8c668582: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c668584;
L8c668584: take=(*sr&1u);r[15]+=4u;if(take)goto L8c668594;goto L8c668588; /* bt/s 0x8c668594 */
 goto L8c668588;
L8c668588: r[2]=rd(m,0x8c6686c0u,4); /* mov.l 0x8c6686c0,r2 */
 goto L8c66858a;
L8c66858a: r[4]=rd(m,0x8c6686e4u,4); /* mov.l 0x8c6686e4,r4 */
 goto L8c66858c;
L8c66858c: target=r[2];*pr=0x8c668590u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c668590;
L8c668590: goto L8c6685b2; /* bra 0x8c6685b2 */
 goto L8c668594;
L8c668594: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c668596;
L8c668596: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c668598;
L8c668598: r[2]=rd(m,0x8c6686e8u,4); /* mov.l 0x8c6686e8,r2 */
 goto L8c66859a;
L8c66859a: r[4]+=32u; /* add #0x20,r4 */
 goto L8c66859c;
L8c66859c: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c66859e;
L8c66859e: r[4]+=r[3]; /* add r3,r4 */
 goto L8c6685a0;
L8c6685a0: target=r[2];*pr=0x8c6685a4u;r[4]=rd(m,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6685a4;
L8c6685a4: r[14]=r[0]; /* mov r0,r14 */
 goto L8c6685a6;
L8c6685a6: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c6685a8;
L8c6685a8: if((*sr&1u))goto L8c6685b2; /* bt 0x8c6685b2 */
 goto L8c6685aa;
L8c6685aa: r[2]=rd(m,0x8c6686c0u,4); /* mov.l 0x8c6686c0,r2 */
 goto L8c6685ac;
L8c6685ac: r[4]=rd(m,0x8c6686ecu,4); /* mov.l 0x8c6686ec,r4 */
 goto L8c6685ae;
L8c6685ae: target=r[2];*pr=0x8c6685b2u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6685b2;
L8c6685b2: r[0]=r[14]; /* mov r14,r0 */
 goto L8c6685b4;
L8c6685b4: r[15]+=20u; /* add #0x14,r15 */
 goto L8c6685b6;
L8c6685b6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6685b8;
L8c6685b8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
