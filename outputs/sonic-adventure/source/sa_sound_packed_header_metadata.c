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
/* Retail packed-header audio metadata, 8c66a378..8c66a440.
 * Final width16/stride2/type1 overrides branch-specific intermediate values.
 * Source format identity not established; no external calls. */
SA_STAGE_EXPORT int sa_sound_packed_header_metadata(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66a378;
L8c66a378: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66a37a;
L8c66a37a: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66a37c;
L8c66a37c: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66a37e;
L8c66a37e: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66a380;
L8c66a380: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c66a382;
L8c66a382: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c66a384;
L8c66a384: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c66a386;
L8c66a386: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c66a388;
L8c66a388: r[5]=r[4]; /* mov r4,r5 */
 goto L8c66a38a;
L8c66a38a: r[0]=sx(m,r[5]+0x7u,1); /* mov.b @(0x7,r5),r0 */
 goto L8c66a38c;
L8c66a38c: r[13]=rd(m,r[15]+0x28u,4); /* mov.l @(0x28,r15),r13 */
 goto L8c66a38e;
L8c66a38e: r[9]=2u; /* mov #0x2,r9 */
 goto L8c66a390;
L8c66a390: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66a392;
L8c66a392: r[12]=rd(m,r[15]+0x30u,4); /* mov.l @(0x30,r15),r12 */
 goto L8c66a394;
L8c66a394: r[0]<<=2; /* shll2 r0 */
 goto L8c66a396;
L8c66a396: r[11]=rd(m,r[15]+0x20u,4); /* mov.l @(0x20,r15),r11 */
 goto L8c66a398;
L8c66a398: r[0]<<=2; /* shll2 r0 */
 goto L8c66a39a;
L8c66a39a: r[10]=rd(m,r[15]+0x24u,4); /* mov.l @(0x24,r15),r10 */
 goto L8c66a39c;
L8c66a39c: r[8]=rd(m,r[15]+0x38u,4); /* mov.l @(0x38,r15),r8 */
 goto L8c66a39e;
L8c66a39e: r[14]=rd(m,r[15]+0x34u,4); /* mov.l @(0x34,r15),r14 */
 goto L8c66a3a0;
L8c66a3a0: wr(m,r[6],r[0],2); /* mov.w r0,@r6 */
 goto L8c66a3a2;
L8c66a3a2: r[6]=1u; /* mov #0x1,r6 */
 goto L8c66a3a4;
L8c66a3a4: r[0]=sx(m,r[5]+0x9u,1); /* mov.b @(0x9,r5),r0 */
 goto L8c66a3a6;
L8c66a3a6: r[0]+=1u; /* add #0x1,r0 */
 goto L8c66a3a8;
L8c66a3a8: wr(m,r[13],r[0],1); /* mov.b r0,@r13 */
 goto L8c66a3aa;
L8c66a3aa: r[0]=42u; /* mov #0x2a,r0 */
 goto L8c66a3ac;
L8c66a3ac: r[2]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r2 */
 goto L8c66a3ae;
L8c66a3ae: r[3]=rd(m,r[15]+0x2cu,4); /* mov.l @(0x2c,r15),r3 */
 goto L8c66a3b0;
L8c66a3b0: r[2]=r[2]&65535u; /* extu.w r2,r2 */
 goto L8c66a3b2;
L8c66a3b2: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c66a3b4;
L8c66a3b4: r[0]=sx(m,r[4]+0x8u,1); /* mov.b @(0x8,r4),r0 */
 goto L8c66a3b6;
L8c66a3b6: wr(m,r[15],r[0],1); /* mov.b r0,@r15 */
 goto L8c66a3b8;
L8c66a3b8: r[0]=sx(m,r[15],1); /* mov.b @r15,r0 */
 goto L8c66a3ba;
L8c66a3ba: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c66a3bc;
L8c66a3bc: *sr=(*sr&~1u)|(r[0]==0x00000000u); /* cmp/eq #0x0,r0 */
 goto L8c66a3be;
L8c66a3be: take=(*sr&1u);r[5]+=12u;if(take)goto L8c66a3d2;goto L8c66a3c2; /* bt/s 0x8c66a3d2 */
 goto L8c66a3c2;
L8c66a3c2: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c66a3c4;
L8c66a3c4: if((*sr&1u))goto L8c66a3f0; /* bt 0x8c66a3f0 */
 goto L8c66a3c6;
L8c66a3c6: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c66a3c8;
L8c66a3c8: if((*sr&1u))goto L8c66a402; /* bt 0x8c66a402 */
 goto L8c66a3ca;
L8c66a3ca: *sr=(*sr&~1u)|(r[0]==0x00000003u); /* cmp/eq #0x3,r0 */
 goto L8c66a3cc;
L8c66a3cc: if((*sr&1u))goto L8c66a402; /* bt 0x8c66a402 */
 goto L8c66a3ce;
L8c66a3ce: goto L8c66a414; /* bra 0x8c66a414 */
 goto L8c66a3d2;
L8c66a3d2: r[3]=16u; /* mov #0x10,r3 */
 goto L8c66a3d4;
L8c66a3d4: wr(m,r[11],r[3],1); /* mov.b r3,@r11 */
 goto L8c66a3d6;
L8c66a3d6: r[2]=sx(m,r[13],1); /* mov.b @r13,r2 */
 goto L8c66a3d8;
L8c66a3d8: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c66a3da;
L8c66a3da: wr(m,r[10],r[2],1); /* mov.b r2,@r10 */
 goto L8c66a3dc;
L8c66a3dc: r[2]=0u; /* mov #0x0,r2 */
 goto L8c66a3de;
L8c66a3de: r[1]=r[2]; /* mov r2,r1 */
 goto L8c66a3e0;
L8c66a3e0: wr(m,r[14],r[6],4); /* mov.l r6,@r14 */
 goto L8c66a3e2;
L8c66a3e2: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c66a3e4;
L8c66a3e4: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[3]); /* cmp/gt r3,r2 */
 goto L8c66a3e6;
L8c66a3e6: {uint64_t sum=(uint64_t)r[3]+r[2]+(*sr&1u);r[3]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);} /* addc r2,r3 */
 goto L8c66a3e8;
L8c66a3e8: *sr=(*sr&~1u)|(r[3]&1u);r[3]=(r[3]>>1)|(r[3]&0x80000000u); /* shar r3 */
 goto L8c66a3ea;
L8c66a3ea: wr(m,r[12],r[3],4); /* mov.l r3,@r12 */
 goto L8c66a3ec;
L8c66a3ec: wr(m,r[8],r[1],2);goto L8c66a414; /* bra 0x8c66a414 */
 goto L8c66a3f0;
L8c66a3f0: r[2]=8u; /* mov #0x8,r2 */
 goto L8c66a3f2;
L8c66a3f2: wr(m,r[11],r[2],1); /* mov.b r2,@r11 */
 goto L8c66a3f4;
L8c66a3f4: r[3]=sx(m,r[13],1); /* mov.b @r13,r3 */
 goto L8c66a3f6;
L8c66a3f6: wr(m,r[10],r[3],1); /* mov.b r3,@r10 */
 goto L8c66a3f8;
L8c66a3f8: wr(m,r[14],r[6],4); /* mov.l r6,@r14 */
 goto L8c66a3fa;
L8c66a3fa: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c66a3fc;
L8c66a3fc: wr(m,r[12],r[2],4); /* mov.l r2,@r12 */
 goto L8c66a3fe;
L8c66a3fe: wr(m,r[8],r[6],2);goto L8c66a414; /* bra 0x8c66a414 */
 goto L8c66a402;
L8c66a402: r[2]=4u; /* mov #0x4,r2 */
 goto L8c66a404;
L8c66a404: wr(m,r[11],r[2],1); /* mov.b r2,@r11 */
 goto L8c66a406;
L8c66a406: r[3]=sx(m,r[13],1); /* mov.b @r13,r3 */
 goto L8c66a408;
L8c66a408: wr(m,r[10],r[3],1); /* mov.b r3,@r10 */
 goto L8c66a40a;
L8c66a40a: wr(m,r[14],r[9],4); /* mov.l r9,@r14 */
 goto L8c66a40c;
L8c66a40c: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c66a40e;
L8c66a40e: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c66a410;
L8c66a410: wr(m,r[12],r[2],4); /* mov.l r2,@r12 */
 goto L8c66a412;
L8c66a412: wr(m,r[8],r[9],2); /* mov.w r9,@r8 */
 goto L8c66a414;
L8c66a414: r[3]=0u; /* mov #0x0,r3 */
 goto L8c66a416;
L8c66a416: wr(m,r[10],r[9],1); /* mov.b r9,@r10 */
 goto L8c66a418;
L8c66a418: r[1]=16u; /* mov #0x10,r1 */
 goto L8c66a41a;
L8c66a41a: wr(m,r[14],r[6],4); /* mov.l r6,@r14 */
 goto L8c66a41c;
L8c66a41c: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c66a41e;
L8c66a41e: r[0]=0u; /* mov #0x0,r0 */
 goto L8c66a420;
L8c66a420: *sr=(*sr&~1u)|((int32_t)r[3]>(int32_t)r[2]); /* cmp/gt r2,r3 */
 goto L8c66a422;
L8c66a422: {uint64_t sum=(uint64_t)r[2]+r[3]+(*sr&1u);r[2]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);} /* addc r3,r2 */
 goto L8c66a424;
L8c66a424: r[3]=4294967295u; /* mov #-0x1,r3 */
 goto L8c66a426;
L8c66a426: *sr=(*sr&~1u)|(r[2]&1u);r[2]=(r[2]>>1)|(r[2]&0x80000000u); /* shar r2 */
 goto L8c66a428;
L8c66a428: wr(m,r[12],r[2],4); /* mov.l r2,@r12 */
 goto L8c66a42a;
L8c66a42a: wr(m,r[11],r[1],1); /* mov.b r1,@r11 */
 goto L8c66a42c;
L8c66a42c: wr(m,r[7],r[3],1); /* mov.b r3,@r7 */
 goto L8c66a42e;
L8c66a42e: r[15]+=4u; /* add #0x4,r15 */
 goto L8c66a430;
L8c66a430: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c66a432;
L8c66a432: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c66a434;
L8c66a434: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c66a436;
L8c66a436: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c66a438;
L8c66a438: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c66a43a;
L8c66a43a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66a43c;
L8c66a43c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
