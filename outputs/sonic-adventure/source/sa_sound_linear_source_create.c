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
/* Retail linear-source constructor: first free of32 slots,36-byte objects and native reset. */
SA_STAGE_EXPORT int sa_sound_linear_source_create(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e0ea;
L8c65e0ea: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65e0ec;
L8c65e0ec: r[14]=32u; /* mov #0x20,r14 */
 goto L8c65e0ee;
L8c65e0ee: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65e0f0;
L8c65e0f0: r[7]=rd(m,0x8c65e1bcu,4); /* mov.l 0x8c65e1bc,r7 */
 goto L8c65e0f2;
L8c65e0f2: r[6]=0u; /* mov #0x0,r6 */
 goto L8c65e0f4;
L8c65e0f4: r[3]=r[6]; /* mov r6,r3 */
 goto L8c65e0f6;
L8c65e0f6: r[3]<<=2; /* shll2 r3 */
 goto L8c65e0f8;
L8c65e0f8: *sr=(*sr&~1u)|(r[3]>>31);r[3]<<=1; /* shll r3 */
 goto L8c65e0fa;
L8c65e0fa: r[2]=r[6]; /* mov r6,r2 */
 goto L8c65e0fc;
L8c65e0fc: r[3]+=r[2]; /* add r2,r3 */
 goto L8c65e0fe;
L8c65e0fe: r[3]<<=2; /* shll2 r3 */
 goto L8c65e100;
L8c65e100: r[3]+=r[7]; /* add r7,r3 */
 goto L8c65e102;
L8c65e102: r[1]=rd(m,r[3]+0x4u,4); /* mov.l @(0x4,r3),r1 */
 goto L8c65e104;
L8c65e104: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c65e106;
L8c65e106: if((*sr&1u))goto L8c65e10e; /* bt 0x8c65e10e */
 goto L8c65e108;
L8c65e108: r[6]+=1u; /* add #0x1,r6 */
 goto L8c65e10a;
L8c65e10a: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[14]); /* cmp/ge r14,r6 */
 goto L8c65e10c;
L8c65e10c: if(!(*sr&1u))goto L8c65e0f4; /* bf 0x8c65e0f4 */
 goto L8c65e10e;
L8c65e10e: r[0]=r[6]; /* mov r6,r0 */
 goto L8c65e110;
L8c65e110:  /* nop */
 goto L8c65e112;
L8c65e112: *sr=(*sr&~1u)|(r[0]==0x00000020u); /* cmp/eq #0x20,r0 */
 goto L8c65e114;
L8c65e114: if(!(*sr&1u))goto L8c65e11e; /* bf 0x8c65e11e */
 goto L8c65e116;
L8c65e116: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e118;
L8c65e118: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65e11a;
L8c65e11a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c65e11e: r[14]=r[6]; /* mov r6,r14 */
 goto L8c65e120;
L8c65e120: r[14]<<=2; /* shll2 r14 */
 goto L8c65e122;
L8c65e122: *sr=(*sr&~1u)|(r[14]>>31);r[14]<<=1; /* shll r14 */
 goto L8c65e124;
L8c65e124: r[3]=r[6]; /* mov r6,r3 */
 goto L8c65e126;
L8c65e126: r[14]+=r[3]; /* add r3,r14 */
 goto L8c65e128;
L8c65e128: r[3]=rd(m,0x8c65e1c4u,4); /* mov.l 0x8c65e1c4,r3 */
 goto L8c65e12a;
L8c65e12a: r[14]<<=2; /* shll2 r14 */
 goto L8c65e12c;
L8c65e12c: r[2]=1u; /* mov #0x1,r2 */
 goto L8c65e12e;
L8c65e12e: r[14]+=r[7]; /* add r7,r14 */
 goto L8c65e130;
L8c65e130: wr(m,r[14]+0x4u,r[2],4); /* mov.l r2,@(0x4,r14) */
 goto L8c65e132;
L8c65e132: wr(m,r[14],r[3],4); /* mov.l r3,@r14 */
 goto L8c65e134;
L8c65e134: r[2]=rd(m,0x8c65e1c8u,4); /* mov.l 0x8c65e1c8,r2 */
 goto L8c65e136;
L8c65e136: r[3]=rd(m,0x8c65e1ccu,4); /* mov.l 0x8c65e1cc,r3 */
 goto L8c65e138;
L8c65e138: wr(m,r[14]+0x14u,r[4],4); /* mov.l r4,@(0x14,r14) */
 goto L8c65e13a;
L8c65e13a: wr(m,r[14]+0x18u,r[5],4); /* mov.l r5,@(0x18,r14) */
 goto L8c65e13c;
L8c65e13c: wr(m,r[14]+0x8u,r[2],4); /* mov.l r2,@(0x8,r14) */
 goto L8c65e13e;
L8c65e13e: wr(m,r[14]+0x1cu,r[3],4); /* mov.l r3,@(0x1c,r14) */
 goto L8c65e140;
L8c65e140: wr(m,r[14]+0x20u,r[14],4); /* mov.l r14,@(0x20,r14) */
 goto L8c65e142;
L8c65e142: *pr=0x8c65e146u;r[4]=r[14];status=call(ctx,0x8c65e178u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65e178 */
 goto L8c65e146;
L8c65e146: r[0]=r[14]; /* mov r14,r0 */
 goto L8c65e148;
L8c65e148:  /* nop */
 goto L8c65e14a;
L8c65e14a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e14c;
L8c65e14c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
