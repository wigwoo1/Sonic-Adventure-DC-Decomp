#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
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
/* Static C recovery of retail Tikal helper 8c645468..8c6454fe.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_directory_record_parse(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c645468;
L8c645468: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c64546a;
L8c64546a: r[2]=34u; /* mov #0x22,r2 */
 goto L8c64546c;
L8c64546c: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c64546e;
L8c64546e: r[13]=r[4]; /* mov r4,r13 */
 goto L8c645470;
L8c645470: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c645472;
L8c645472: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c645474;
L8c645474: r[3]=sx(m,r[13],1); /* mov.b @r13,r3 */
 goto L8c645476;
L8c645476: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c645478;
L8c645478: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]); /* cmp/ge r2,r3 */
 goto L8c64547a;
L8c64547a: take=(*sr&1u);r[14]=r[5];if(take)goto L8c645482;goto L8c64547e; /* bt/s 0x8c645482 */
 goto L8c64547e;
L8c64547e: r[0]=0u;goto L8c6454f4; /* bra 0x8c6454f4 */
 goto L8c645482;
L8c645482: r[4]=r[13]; /* mov r13,r4 */
 goto L8c645484;
L8c645484: r[5]=r[14]; /* mov r14,r5 */
 goto L8c645486;
L8c645486: r[6]=4u; /* mov #0x4,r6 */
 goto L8c645488;
L8c645488: *pr=0x8c64548cu;r[4]+=2u;status=call(ctx,0x8c64535cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c64535c */
 goto L8c64548c;
L8c64548c: r[5]=r[14]; /* mov r14,r5 */
 goto L8c64548e;
L8c64548e: r[4]=r[13]; /* mov r13,r4 */
 goto L8c645490;
L8c645490: r[5]+=4u; /* add #0x4,r5 */
 goto L8c645492;
L8c645492: r[6]=4u; /* mov #0x4,r6 */
 goto L8c645494;
L8c645494: *pr=0x8c645498u;r[4]+=10u;status=call(ctx,0x8c64535cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c64535c */
 goto L8c645498;
L8c645498: r[0]=25u; /* mov #0x19,r0 */
 goto L8c64549a;
L8c64549a: r[0]=sx(m,r[13]+r[0],1); /* mov.b @(r0,r13),r0 */
 goto L8c64549c;
L8c64549c: wr(m,r[14]+0x8u,r[0],1); /* mov.b r0,@(0x8,r14) */
 goto L8c64549e;
L8c64549e: r[0]=32u; /* mov #0x20,r0 */
 goto L8c6454a0;
L8c6454a0: r[12]=sx(m,r[13]+r[0],1); /* mov.b @(r0,r13),r12 */
 goto L8c6454a2;
L8c6454a2: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto L8c6454a4;
L8c6454a4: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6454a6;
L8c6454a6: *sr=(*sr&~1u)|!(r[0]&0x2u); /* tst #0x2,r0 */
 goto L8c6454a8;
L8c6454a8: take=!(*sr&1u);r[12]=r[12]&255u;if(take)goto L8c6454ae;goto L8c6454ac; /* bf/s 0x8c6454ae */
 goto L8c6454ac;
L8c6454ac: r[12]+=4294967294u; /* add #-0x2,r12 */
 goto L8c6454ae;
L8c6454ae: r[5]=r[14]; /* mov r14,r5 */
 goto L8c6454b0;
L8c6454b0: r[4]=r[13]; /* mov r13,r4 */
 goto L8c6454b2;
L8c6454b2: r[5]+=10u; /* add #0xa,r5 */
 goto L8c6454b4;
L8c6454b4: r[6]=r[12]; /* mov r12,r6 */
 goto L8c6454b6;
L8c6454b6: *pr=0x8c6454bau;r[4]+=33u;status=call(ctx,0x8c64535cu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c64535c */
 goto L8c6454ba;
L8c6454ba: r[3]=r[14]; /* mov r14,r3 */
 goto L8c6454bc;
L8c6454bc: r[3]+=10u; /* add #0xa,r3 */
 goto L8c6454be;
L8c6454be: r[4]=0u; /* mov #0x0,r4 */
 goto L8c6454c0;
L8c6454c0: r[12]+=r[3]; /* add r3,r12 */
 goto L8c6454c2;
L8c6454c2: wr(m,r[12],r[4],1); /* mov.b r4,@r12 */
 goto L8c6454c4;
L8c6454c4: r[0]=sx(m,r[14]+0x8u,1); /* mov.b @(0x8,r14),r0 */
 goto L8c6454c6;
L8c6454c6: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6454c8;
L8c6454c8: *sr=(*sr&~1u)|!(r[0]&0x2u); /* tst #0x2,r0 */
 goto L8c6454ca;
L8c6454ca: if((*sr&1u))goto L8c6454e8; /* bt 0x8c6454e8 */
 goto L8c6454cc;
L8c6454cc: r[0]=sx(m,r[14]+0xau,1); /* mov.b @(0xa,r14),r0 */
 goto L8c6454ce;
L8c6454ce: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c6454d0;
L8c6454d0: take=!(*sr&1u);r[5]=46u;if(take)goto L8c6454da;goto L8c6454d4; /* bf/s 0x8c6454da */
 goto L8c6454d4;
L8c6454d4: r[0]=r[5]; /* mov r5,r0 */
 goto L8c6454d6;
L8c6454d6: wr(m,r[14]+0xau,r[0],1);goto L8c6454e8; /* bra 0x8c6454e8 */
 goto L8c6454da;
L8c6454da: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c6454dc;
L8c6454dc: if(!(*sr&1u))goto L8c6454e8; /* bf 0x8c6454e8 */
 goto L8c6454de;
L8c6454de: r[0]=r[5]; /* mov r5,r0 */
 goto L8c6454e0;
L8c6454e0: wr(m,r[14]+0xau,r[0],1); /* mov.b r0,@(0xa,r14) */
 goto L8c6454e2;
L8c6454e2: wr(m,r[14]+0xbu,r[0],1); /* mov.b r0,@(0xb,r14) */
 goto L8c6454e4;
L8c6454e4: r[0]=r[4]; /* mov r4,r0 */
 goto L8c6454e6;
L8c6454e6: wr(m,r[14]+0xcu,r[0],1); /* mov.b r0,@(0xc,r14) */
 goto L8c6454e8;
L8c6454e8: r[4]=r[14]; /* mov r14,r4 */
 goto L8c6454ea;
L8c6454ea: *pr=0x8c6454eeu;r[4]+=10u;status=call(ctx,0x8c645442u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c645442 */
 goto L8c6454ee;
L8c6454ee: wr(m,r[14]+0x9u,r[0],1); /* mov.b r0,@(0x9,r14) */
 goto L8c6454f0;
L8c6454f0: r[0]=sx(m,r[13],1); /* mov.b @r13,r0 */
 goto L8c6454f2;
L8c6454f2: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6454f4;
L8c6454f4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6454f6;
L8c6454f6: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6454f8;
L8c6454f8: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6454fa;
L8c6454fa: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
