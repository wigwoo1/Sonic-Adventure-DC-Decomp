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
/* Static C recovery of retail Tikal helper 8c65b7a8..8c65b88c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_stream_work_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65b7a8;
L8c65b7a8: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65b7aa;
L8c65b7aa: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65b7ac;
L8c65b7ac: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65b7ae;
L8c65b7ae: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65b7b0;
L8c65b7b0: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65b7b2;
L8c65b7b2: r[15]+=4294967276u; /* add #-0x14,r15 */
 goto L8c65b7b4;
L8c65b7b4: r[13]=r[4]; /* mov r4,r13 */
 goto L8c65b7b6;
L8c65b7b6: r[14]=rd(m,r[13]+0x8u,4); /* mov.l @(0x8,r13),r14 */
 goto L8c65b7b8;
L8c65b7b8: r[7]=r[15]; /* mov r15,r7 */
 goto L8c65b7ba;
L8c65b7ba: r[6]=sx(m,0x8c65b820u,2); /* mov.w 0x8c65b820,r6 */
 goto L8c65b7bc;
L8c65b7bc: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65b7be;
L8c65b7be: r[5]=1u; /* mov #0x1,r5 */
 goto L8c65b7c0;
L8c65b7c0: r[11]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r11 */
 goto L8c65b7c2;
L8c65b7c2: r[7]+=12u; /* add #0xc,r7 */
 goto L8c65b7c4;
L8c65b7c4: r[2]=rd(m,r[3]+0x18u,4); /* mov.l @(0x18,r3),r2 */
 goto L8c65b7c6;
L8c65b7c6: target=r[2];*pr=0x8c65b7cau;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65b7ca;
L8c65b7ca: r[6]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r6 */
 goto L8c65b7cc;
L8c65b7cc: r[3]=16u; /* mov #0x10,r3 */
 goto L8c65b7ce;
L8c65b7ce: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]); /* cmp/ge r3,r6 */
 goto L8c65b7d0;
L8c65b7d0: if(!(*sr&1u))goto L8c65b7e6; /* bf 0x8c65b7e6 */
 goto L8c65b7d2;
L8c65b7d2: r[3]=rd(m,0x8c65b838u,4); /* mov.l 0x8c65b838,r3 */
 goto L8c65b7d4;
L8c65b7d4: r[5]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r5 */
 goto L8c65b7d6;
L8c65b7d6: target=r[3];*pr=0x8c65b7dau;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65b7da;
L8c65b7da: r[12]=r[0]; /* mov r0,r12 */
 goto L8c65b7dc;
L8c65b7dc: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c65b7de;
L8c65b7de: if((*sr&1u))goto L8c65b7e6; /* bt 0x8c65b7e6 */
 goto L8c65b7e0;
L8c65b7e0: r[3]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r3 */
 goto L8c65b7e2;
L8c65b7e2: *sr=(*sr&~1u)|((int32_t)r[12]>(int32_t)r[3]); /* cmp/gt r3,r12 */
 goto L8c65b7e4;
L8c65b7e4: if(!(*sr&1u))goto L8c65b7f8; /* bf 0x8c65b7f8 */
 goto L8c65b7e6;
L8c65b7e6: r[1]=rd(m,r[14],4); /* mov.l @r14,r1 */
 goto L8c65b7e8;
L8c65b7e8: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65b7ea;
L8c65b7ea: r[5]=1u; /* mov #0x1,r5 */
 goto L8c65b7ec;
L8c65b7ec: r[3]=rd(m,r[1]+0x1cu,4); /* mov.l @(0x1c,r1),r3 */
 goto L8c65b7ee;
L8c65b7ee: r[6]+=12u; /* add #0xc,r6 */
 goto L8c65b7f0;
L8c65b7f0: target=r[3];*pr=0x8c65b7f4u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65b7f4;
L8c65b7f4: goto L8c65b87e; /* bra 0x8c65b87e */
 goto L8c65b7f8;
L8c65b7f8: r[2]=rd(m,0x8c65b83cu,4); /* mov.l 0x8c65b83c,r2 */
 goto L8c65b7fa;
L8c65b7fa: target=r[2];*pr=0x8c65b7feu;r[4]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65b7fe;
L8c65b7fe: *sr=(*sr&~1u)|(r[0]==0x00000004u); /* cmp/eq #0x4,r0 */
 goto L8c65b800;
L8c65b800: take=!(*sr&1u);r[4]=r[11];if(take)goto L8c65b808;goto L8c65b804; /* bf/s 0x8c65b808 */
 goto L8c65b804;
L8c65b804: r[0]=1u; /* mov #0x1,r0 */
 goto L8c65b806;
L8c65b806: wr(m,r[13]+0x3u,r[0],1); /* mov.b r0,@(0x3,r13) */
 goto L8c65b808;
L8c65b808: r[3]=rd(m,0x8c65b83cu,4); /* mov.l 0x8c65b83c,r3 */
 goto L8c65b80a;
L8c65b80a: target=r[3];*pr=0x8c65b80eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65b80e;
L8c65b80e: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c65b810;
L8c65b810: if(!(*sr&1u))goto L8c65b84c; /* bf 0x8c65b84c */
 goto L8c65b812;
L8c65b812: r[6]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r6 */
 goto L8c65b814;
L8c65b814: r[3]=64u; /* mov #0x40,r3 */
 goto L8c65b816;
L8c65b816: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[3]); /* cmp/ge r3,r6 */
 goto L8c65b818;
L8c65b818: wr(m,r[15],r[6],4); /* mov.l r6,@r15 */
 goto L8c65b81a;
L8c65b81a: if((*sr&1u))goto L8c65b840; /* bt 0x8c65b840 */
 goto L8c65b81c;
L8c65b81c: r[6]=rd(m,r[15],4);goto L8c65b842; /* bra 0x8c65b842 */
 goto L8c65b840;
L8c65b840: r[6]=r[3]; /* mov r3,r6 */
 goto L8c65b842;
L8c65b842: r[3]=rd(m,0x8c65b95cu,4); /* mov.l 0x8c65b95c,r3 */
 goto L8c65b844;
L8c65b844: r[4]=r[13]; /* mov r13,r4 */
 goto L8c65b846;
L8c65b846: r[5]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r5 */
 goto L8c65b848;
L8c65b848: target=r[3];*pr=0x8c65b84cu;r[4]+=80u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65b84c;
L8c65b84c: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65b84e;
L8c65b84e: r[2]=rd(m,0x8c65b960u,4); /* mov.l 0x8c65b960,r2 */
 goto L8c65b850;
L8c65b850: r[7]=r[15]; /* mov r15,r7 */
 goto L8c65b852;
L8c65b852: r[4]=r[15]; /* mov r15,r4 */
 goto L8c65b854;
L8c65b854: r[5]=r[12]; /* mov r12,r5 */
 goto L8c65b856;
L8c65b856: r[7]+=4u; /* add #0x4,r7 */
 goto L8c65b858;
L8c65b858: r[6]+=12u; /* add #0xc,r6 */
 goto L8c65b85a;
L8c65b85a: target=r[2];*pr=0x8c65b85eu;r[4]+=12u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65b85e;
L8c65b85e: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65b860;
L8c65b860: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65b862;
L8c65b862: r[5]=0u; /* mov #0x0,r5 */
 goto L8c65b864;
L8c65b864: r[2]=rd(m,r[3]+0x20u,4); /* mov.l @(0x20,r3),r2 */
 goto L8c65b866;
L8c65b866: r[6]+=12u; /* add #0xc,r6 */
 goto L8c65b868;
L8c65b868: target=r[2];*pr=0x8c65b86cu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65b86c;
L8c65b86c: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c65b86e;
L8c65b86e: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65b870;
L8c65b870: r[5]=1u; /* mov #0x1,r5 */
 goto L8c65b872;
L8c65b872: r[2]=rd(m,r[3]+0x1cu,4); /* mov.l @(0x1c,r3),r2 */
 goto L8c65b874;
L8c65b874: r[6]+=4u; /* add #0x4,r6 */
 goto L8c65b876;
L8c65b876: target=r[2];*pr=0x8c65b87au;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65b87a;
L8c65b87a: r[0]=2u; /* mov #0x2,r0 */
 goto L8c65b87c;
L8c65b87c: wr(m,r[13]+0x1u,r[0],1); /* mov.b r0,@(0x1,r13) */
 goto L8c65b87e;
L8c65b87e: r[15]+=20u; /* add #0x14,r15 */
 goto L8c65b880;
L8c65b880: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65b882;
L8c65b882: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65b884;
L8c65b884: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65b886;
L8c65b886: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65b888;
L8c65b888: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
