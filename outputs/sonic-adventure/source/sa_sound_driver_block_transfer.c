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
/* Static C recovery of retail Tikal helper 8c6684c4..8c66854e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_block_transfer(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6684c4;
L8c6684c4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6684c6;
L8c6684c6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6684c8;
L8c6684c8: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6684ca;
L8c6684ca: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6684cc;
L8c6684cc: r[15]+=4294967276u; /* add #-0x14,r15 */
 goto L8c6684ce;
L8c6684ce: r[12]=rd(m,0x8c6686d8u,4); /* mov.l 0x8c6686d8,r12 */
 goto L8c6684d0;
L8c6684d0: r[0]=r[5]; /* mov r5,r0 */
 goto L8c6684d2;
L8c6684d2: r[13]=rd(m,0x8c6686dcu,4); /* mov.l 0x8c6686dc,r13 */
 goto L8c6684d4;
L8c6684d4: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c6684d6;
L8c6684d6: wr(m,r[15]+0xcu,r[0],1); /* mov.b r0,@(0xc,r15) */
 goto L8c6684d8;
L8c6684d8: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c6684da;
L8c6684da: target=r[13];*pr=0x8c6684deu;wr(m,r[15]+0x8u,r[7],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c6684de;
L8c6684de: r[3]=0u; /* mov #0x0,r3 */
 goto L8c6684e0;
L8c6684e0: wr(m,r[12],r[0],4); /* mov.l r0,@r12 */
 goto L8c6684e2;
L8c6684e2: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c6684e4;
L8c6684e4: r[7]=r[3]; /* mov r3,r7 */
 goto L8c6684e6;
L8c6684e6: r[0]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c6684e8;
L8c6684e8: r[1]=rd(m,0x8c6686e0u,4); /* mov.l 0x8c6686e0,r1 */
 goto L8c6684ea;
L8c6684ea: r[2]=r[0]; /* mov r0,r2 */
 goto L8c6684ec;
L8c6684ec: r[0]=16u; /* mov #0x10,r0 */
 goto L8c6684ee;
L8c6684ee: r[0]=sx(m,r[15]+r[0],1); /* mov.b @(r0,r15),r0 */
 goto L8c6684f0;
L8c6684f0: r[2]+=24u; /* add #0x18,r2 */
 goto L8c6684f2;
L8c6684f2: r[6]=rd(m,r[15]+0x28u,4); /* mov.l @(0x28,r15),r6 */
 goto L8c6684f4;
L8c6684f4: r[4]=r[0]; /* mov r0,r4 */
 goto L8c6684f6;
L8c6684f6: r[4]<<=2; /* shll2 r4 */
 goto L8c6684f8;
L8c6684f8: r[5]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r5 */
 goto L8c6684fa;
L8c6684fa: wr(m,r[15]+0x14u,r[4],4); /* mov.l r4,@(0x14,r15) */
 goto L8c6684fc;
L8c6684fc: r[4]+=r[2]; /* add r2,r4 */
 goto L8c6684fe;
L8c6684fe: target=r[1];*pr=0x8c668502u;r[4]=rd(m,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c668502;
L8c668502: r[14]=r[0]; /* mov r0,r14 */
 goto L8c668504;
L8c668504: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c668506;
L8c668506: take=(*sr&1u);r[15]+=4u;if(take)goto L8c668516;goto L8c66850a; /* bt/s 0x8c668516 */
 goto L8c66850a;
L8c66850a: r[2]=rd(m,0x8c6686c0u,4); /* mov.l 0x8c6686c0,r2 */
 goto L8c66850c;
L8c66850c: r[4]=rd(m,0x8c6686e4u,4); /* mov.l 0x8c6686e4,r4 */
 goto L8c66850e;
L8c66850e: target=r[2];*pr=0x8c668512u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c668512;
L8c668512: goto L8c668540; /* bra 0x8c668540 */
 goto L8c668516;
L8c668516: target=r[13];*pr=0x8c66851au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c66851a;
L8c66851a: r[2]=rd(m,0x8c6686e8u,4); /* mov.l 0x8c6686e8,r2 */
 goto L8c66851c;
L8c66851c: wr(m,r[12]+0x4u,r[0],4); /* mov.l r0,@(0x4,r12) */
 goto L8c66851e;
L8c66851e: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c668520;
L8c668520: r[3]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r3 */
 goto L8c668522;
L8c668522: r[4]+=24u; /* add #0x18,r4 */
 goto L8c668524;
L8c668524: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c668526;
L8c668526: r[4]+=r[3]; /* add r3,r4 */
 goto L8c668528;
L8c668528: target=r[2];*pr=0x8c66852cu;r[4]=rd(m,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66852c;
L8c66852c: r[14]=r[0]; /* mov r0,r14 */
 goto L8c66852e;
L8c66852e: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c668530;
L8c668530: if((*sr&1u))goto L8c66853a; /* bt 0x8c66853a */
 goto L8c668532;
L8c668532: r[2]=rd(m,0x8c6686c0u,4); /* mov.l 0x8c6686c0,r2 */
 goto L8c668534;
L8c668534: r[4]=rd(m,0x8c6686ecu,4); /* mov.l 0x8c6686ec,r4 */
 goto L8c668536;
L8c668536: target=r[2];*pr=0x8c66853au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66853a;
L8c66853a: target=r[13];*pr=0x8c66853eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c66853e;
L8c66853e: wr(m,r[12]+0x8u,r[0],4); /* mov.l r0,@(0x8,r12) */
 goto L8c668540;
L8c668540: r[0]=r[14]; /* mov r14,r0 */
 goto L8c668542;
L8c668542: r[15]+=20u; /* add #0x14,r15 */
 goto L8c668544;
L8c668544: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c668546;
L8c668546: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c668548;
L8c668548: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66854a;
L8c66854a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
