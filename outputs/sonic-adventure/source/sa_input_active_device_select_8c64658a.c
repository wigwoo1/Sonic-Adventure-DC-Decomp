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
/* Static C recovery of retail Tikal helper 8c64658a..8c646654.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_active_device_select_8c64658a(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c64658a;
L8c64658a: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c64658c;
L8c64658c: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c64658e;
L8c64658e: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c646590;
L8c646590: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c646592;
L8c646592: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c646594;
L8c646594: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c646596;
L8c646596: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c646598;
L8c646598: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c64659a;
L8c64659a: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c64659c;
L8c64659c: r[2]=rd(m,0x8c646628u,4); /* mov.l 0x8c646628,r2 */
 goto L8c64659e;
L8c64659e: r[8]=4u; /* mov #0x4,r8 */
 goto L8c6465a0;
L8c6465a0: r[0]=sx(m,0x8c646616u,2); /* mov.w 0x8c646616,r0 */
 goto L8c6465a2;
L8c6465a2: r[10]=r[8]; /* mov r8,r10 */
 goto L8c6465a4;
L8c6465a4: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c6465a6;
L8c6465a6: r[9]=1u; /* mov #0x1,r9 */
 goto L8c6465a8;
L8c6465a8: r[11]=rd(m,0x8c64662cu,4); /* mov.l 0x8c64662c,r11 */
 goto L8c6465aa;
L8c6465aa: r[10]+=124u; /* add #0x7c,r10 */
 goto L8c6465ac;
L8c6465ac: r[1]=sx(m,r[3]+r[0],1); /* mov.b @(r0,r3),r1 */
 goto L8c6465ae;
L8c6465ae: r[12]=rd(m,0x8c646630u,4); /* mov.l 0x8c646630,r12 */
 goto L8c6465b0;
L8c6465b0: wr(m,r[15],r[1],1); /* mov.b r1,@r15 */
 goto L8c6465b2;
L8c6465b2: r[14]=0u;goto L8c64663a; /* bra 0x8c64663a */
 goto L8c6465b6;
L8c6465b6: r[3]=sx(m,r[15],1); /* mov.b @r15,r3 */
 goto L8c6465b8;
L8c6465b8: r[2]=r[9]; /* mov r9,r2 */
 goto L8c6465ba;
L8c6465ba: {uint32_t shift=r[14];if(!(shift&0x80000000u))r[2]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[2]=n?(uint32_t)((int32_t)r[2]>>n):((r[2]&0x80000000u)?0xffffffffu:0u);}} /* shad r14,r2 */
 goto L8c6465bc;
L8c6465bc: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c6465be;
L8c6465be: *sr=(*sr&~1u)|!(r[3]&r[2]); /* tst r3,r2 */
 goto L8c6465c0;
L8c6465c0: if((*sr&1u))goto L8c646638; /* bt 0x8c646638 */
 goto L8c6465c2;
L8c6465c2: r[4]=rd(m,0x8c646634u,4); /* mov.l 0x8c646634,r4 */
 goto L8c6465c4;
L8c6465c4: r[4]+=r[14]; /* add r14,r4 */
 goto L8c6465c6;
L8c6465c6: wr(m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c6465c8;
L8c6465c8: r[4]=sx(m,r[4],1); /* mov.b @r4,r4 */
 goto L8c6465ca;
L8c6465ca: *pr=0x8c6465ceu;r[4]=r[4]&255u;status=call(ctx,0x8c646856u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c646856 */
 goto L8c6465ce;
L8c6465ce: r[4]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r4 */
 goto L8c6465d0;
L8c6465d0: r[13]=r[0]; /* mov r0,r13 */
 goto L8c6465d2;
L8c6465d2: r[4]=sx(m,r[4],1); /* mov.b @r4,r4 */
 goto L8c6465d4;
L8c6465d4: *pr=0x8c6465d8u;r[4]=r[4]&255u;status=call(ctx,0x8c646868u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c646868 */
 goto L8c6465d8;
L8c6465d8: r[4]=r[0]; /* mov r0,r4 */
 goto L8c6465da;
L8c6465da: r[0]=r[14]; /* mov r14,r0 */
 goto L8c6465dc;
L8c6465dc: r[3]=sx(m,r[11]+r[0],1); /* mov.b @(r0,r11),r3 */
 goto L8c6465de;
L8c6465de: wr(m,r[12]+r[0],r[3],1); /* mov.b r3,@(r0,r12) */
 goto L8c6465e0;
L8c6465e0: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c6465e2;
L8c6465e2: *sr=(*sr&~1u)|!(r[10]&r[2]); /* tst r10,r2 */
 goto L8c6465e4;
L8c6465e4: if((*sr&1u))goto L8c646638; /* bt 0x8c646638 */
 goto L8c6465e6;
L8c6465e6: r[0]=sx(m,r[13]+0x2u,1); /* mov.b @(0x2,r13),r0 */
 goto L8c6465e8;
L8c6465e8: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c6465ea;
L8c6465ea: if(!(*sr&1u))goto L8c646638; /* bf 0x8c646638 */
 goto L8c6465ec;
L8c6465ec: r[0]=sx(m,r[13]+0x4u,1); /* mov.b @(0x4,r13),r0 */
 goto L8c6465ee;
L8c6465ee: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6465f0;
L8c6465f0: *sr=(*sr&~1u)|(r[0]==0x00000008u); /* cmp/eq #0x8,r0 */
 goto L8c6465f2;
L8c6465f2: if(!(*sr&1u))goto L8c646638; /* bf 0x8c646638 */
 goto L8c6465f4;
L8c6465f4: r[4]=rd(m,r[13]+0x8u,4); /* mov.l @(0x8,r13),r4 */
 goto L8c6465f6;
L8c6465f6: r[0]=sx(m,r[4]+0x8u,1); /* mov.b @(0x8,r4),r0 */
 goto L8c6465f8;
L8c6465f8: r[0]&=0x4u; /* and #0x4,r0 */
 goto L8c6465fa;
L8c6465fa: r[4]=r[0]; /* mov r0,r4 */
 goto L8c6465fc;
L8c6465fc: r[0]=r[14]; /* mov r14,r0 */
 goto L8c6465fe;
L8c6465fe: wr(m,r[11]+r[0],r[4],1); /* mov.b r4,@(r0,r11) */
 goto L8c646600;
L8c646600: r[5]=sx(m,r[12]+r[0],1); /* mov.b @(r0,r12),r5 */
 goto L8c646602;
L8c646602: r[5]^=r[4]; /* xor r4,r5 */
 goto L8c646604;
L8c646604: r[4]=r[4]&255u; /* extu.b r4,r4 */
 goto L8c646606;
L8c646606: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c646608;
L8c646608: if(!(*sr&1u))goto L8c646638; /* bf 0x8c646638 */
 goto L8c64660a;
L8c64660a: r[5]=r[5]&255u; /* extu.b r5,r5 */
 goto L8c64660c;
L8c64660c: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c64660e;
L8c64660e: if((*sr&1u))goto L8c646638; /* bt 0x8c646638 */
 goto L8c646610;
L8c646610: r[0]=r[14]; /* mov r14,r0 */
 goto L8c646612;
L8c646612: r[0]+=1u;goto L8c646640; /* bra 0x8c646640 */
 goto L8c646638;
L8c646638: r[14]+=1u; /* add #0x1,r14 */
 goto L8c64663a;
L8c64663a: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[8]); /* cmp/ge r8,r14 */
 goto L8c64663c;
L8c64663c: if(!(*sr&1u))goto L8c6465b6; /* bf 0x8c6465b6 */
 goto L8c64663e;
L8c64663e: r[0]=0u; /* mov #0x0,r0 */
 goto L8c646640;
L8c646640: r[15]+=8u; /* add #0x8,r15 */
 goto L8c646642;
L8c646642: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c646644;
L8c646644: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c646646;
L8c646646: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c646648;
L8c646648: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c64664a;
L8c64664a: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c64664c;
L8c64664c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c64664e;
L8c64664e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c646650;
L8c646650: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
