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
/* Retail source routine 8c65e3c6..8c65e42c. Allocates first free of64 source slots, installs bindings and calls reset. */
SA_STAGE_EXPORT int sa_sound_source_create(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e3c6;
L8c65e3c6: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65e3c8;
L8c65e3c8: r[0]=64u; /* mov #0x40,r0 */
 goto L8c65e3ca;
L8c65e3ca: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65e3cc;
L8c65e3cc: r[14]=rd(m,0x8c65e4a4u,4); /* mov.l 0x8c65e4a4,r14 */
 goto L8c65e3ce;
L8c65e3ce: r[7]=0u; /* mov #0x0,r7 */
 goto L8c65e3d0;
L8c65e3d0: r[3]=r[7]; /* mov r7,r3 */
 goto L8c65e3d2;
L8c65e3d2: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c65e3d4;
L8c65e3d4: r[2]=r[7]; /* mov r7,r2 */
 goto L8c65e3d6;
L8c65e3d6: r[3]+=r[2]; /* add r2,r3 */
 goto L8c65e3d8;
L8c65e3d8: r[3]<<=2; /* shll2 r3 */
 goto L8c65e3da;
L8c65e3da: r[3]<<=2; /* shll2 r3 */
 goto L8c65e3dc;
L8c65e3dc: r[3]+=r[14]; /* add r14,r3 */
 goto L8c65e3de;
L8c65e3de: r[1]=rd(m,r[3]+0x4u,4); /* mov.l @(0x4,r3),r1 */
 goto L8c65e3e0;
L8c65e3e0: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c65e3e2;
L8c65e3e2: if((*sr&1u))goto L8c65e3ea; /* bt 0x8c65e3ea */
 goto L8c65e3e4;
L8c65e3e4: r[7]+=1u; /* add #0x1,r7 */
 goto L8c65e3e6;
L8c65e3e6: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[0]); /* cmp/ge r0,r7 */
 goto L8c65e3e8;
L8c65e3e8: if(!(*sr&1u))goto L8c65e3d0; /* bf 0x8c65e3d0 */
 goto L8c65e3ea;
L8c65e3ea: r[0]=r[7]; /* mov r7,r0 */
 goto L8c65e3ec;
L8c65e3ec:  /* nop */
 goto L8c65e3ee;
L8c65e3ee: *sr=(*sr&~1u)|(r[0]==0x00000040u); /* cmp/eq #0x40,r0 */
 goto L8c65e3f0;
L8c65e3f0: if(!(*sr&1u))goto L8c65e3fa; /* bf 0x8c65e3fa */
 goto L8c65e3f2;
L8c65e3f2: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e3f4;
L8c65e3f4: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65e3f6;
L8c65e3f6: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c65e3fa: r[3]=r[7]; /* mov r7,r3 */
 goto L8c65e3fc;
L8c65e3fc: *sr=(*sr&~1u)|(r[7]>>31);r[7]<<=1; /* shll r7 */
 goto L8c65e3fe;
L8c65e3fe: r[7]+=r[3]; /* add r3,r7 */
 goto L8c65e400;
L8c65e400: r[3]=rd(m,0x8c65e4acu,4); /* mov.l 0x8c65e4ac,r3 */
 goto L8c65e402;
L8c65e402: r[7]<<=2; /* shll2 r7 */
 goto L8c65e404;
L8c65e404: r[7]<<=2; /* shll2 r7 */
 goto L8c65e406;
L8c65e406: r[2]=1u; /* mov #0x1,r2 */
 goto L8c65e408;
L8c65e408: r[14]+=r[7]; /* add r7,r14 */
 goto L8c65e40a;
L8c65e40a: wr(m,r[14]+0x4u,r[2],4); /* mov.l r2,@(0x4,r14) */
 goto L8c65e40c;
L8c65e40c: wr(m,r[14],r[3],4); /* mov.l r3,@r14 */
 goto L8c65e40e;
L8c65e40e: r[3]=rd(m,0x8c65e4b4u,4); /* mov.l 0x8c65e4b4,r3 */
 goto L8c65e410;
L8c65e410: r[2]=rd(m,0x8c65e4b0u,4); /* mov.l 0x8c65e4b0,r2 */
 goto L8c65e412;
L8c65e412: wr(m,r[14]+0x1cu,r[4],4); /* mov.l r4,@(0x1c,r14) */
 goto L8c65e414;
L8c65e414: wr(m,r[14]+0x20u,r[5],4); /* mov.l r5,@(0x20,r14) */
 goto L8c65e416;
L8c65e416: wr(m,r[14]+0x24u,r[6],4); /* mov.l r6,@(0x24,r14) */
 goto L8c65e418;
L8c65e418: wr(m,r[14]+0x8u,r[2],4); /* mov.l r2,@(0x8,r14) */
 goto L8c65e41a;
L8c65e41a: wr(m,r[14]+0x28u,r[3],4); /* mov.l r3,@(0x28,r14) */
 goto L8c65e41c;
L8c65e41c: wr(m,r[14]+0x2cu,r[14],4); /* mov.l r14,@(0x2c,r14) */
 goto L8c65e41e;
L8c65e41e: *pr=0x8c65e422u;r[4]=r[14];status=call(ctx,0x8c65e454u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65e454 */
 goto L8c65e422;
L8c65e422: r[0]=r[14]; /* mov r14,r0 */
 goto L8c65e424;
L8c65e424:  /* nop */
 goto L8c65e426;
L8c65e426: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e428;
L8c65e428: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
