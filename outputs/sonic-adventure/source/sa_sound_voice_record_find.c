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
/* USA Rev A sound-work lookup094280. Returns last identity/payload match across37 slots; provisional name. */
SA_STAGE_EXPORT int sa_sound_voice_record_find(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c094280;
L8c094280: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c094282;
L8c094282: r[7]=52u; /* mov #0x34,r7 */
 goto L8c094284;
L8c094284: r[0]=rd(m,0x8c0943c0u,4); /* mov.l 0x8c0943c0,r0 */
 goto L8c094286;
L8c094286: r[13]=4294967295u; /* mov #-0x1,r13 */
 goto L8c094288;
L8c094288: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c09428a;
L8c09428a: r[12]=33u; /* mov #0x21,r12 */
 goto L8c09428c;
L8c09428c: r[6]=0u; /* mov #0x0,r6 */
 goto L8c09428e;
L8c09428e: *macl=r[7]*r[6]; /* mul.l r7,r6 */
 goto L8c094290;
L8c094290: r[3]=*macl; /* sts MACL,r3 */
 goto L8c094292;
L8c094292: r[3]+=r[0]; /* add r0,r3 */
 goto L8c094294;
L8c094294: r[2]=rd(m,r[3]+0x8u,4); /* mov.l @(0x8,r3),r2 */
 goto L8c094296;
L8c094296: *sr=(*sr&~1u)|(r[5]==r[2]); /* cmp/eq r5,r2 */
 goto L8c094298;
L8c094298: if(!(*sr&1u))goto L8c0942a8; /* bf 0x8c0942a8 */
 goto L8c09429a;
L8c09429a: *macl=r[7]*r[6]; /* mul.l r7,r6 */
 goto L8c09429c;
L8c09429c: r[1]=*macl; /* sts MACL,r1 */
 goto L8c09429e;
L8c09429e: r[1]+=r[0]; /* add r0,r1 */
 goto L8c0942a0;
L8c0942a0: r[3]=rd(m,r[1]+0x10u,4); /* mov.l @(0x10,r1),r3 */
 goto L8c0942a2;
L8c0942a2: *sr=(*sr&~1u)|(r[4]==r[3]); /* cmp/eq r4,r3 */
 goto L8c0942a4;
L8c0942a4: if(!(*sr&1u))goto L8c0942a8; /* bf 0x8c0942a8 */
 goto L8c0942a6;
L8c0942a6: r[13]=r[6]; /* mov r6,r13 */
 goto L8c0942a8;
L8c0942a8: r[6]+=1u; /* add #0x1,r6 */
 goto L8c0942aa;
L8c0942aa: *macl=r[7]*r[6]; /* mul.l r7,r6 */
 goto L8c0942ac;
L8c0942ac: r[3]=*macl; /* sts MACL,r3 */
 goto L8c0942ae;
L8c0942ae: r[3]+=r[0]; /* add r0,r3 */
 goto L8c0942b0;
L8c0942b0: r[2]=rd(m,r[3]+0x8u,4); /* mov.l @(0x8,r3),r2 */
 goto L8c0942b2;
L8c0942b2: *sr=(*sr&~1u)|(r[5]==r[2]); /* cmp/eq r5,r2 */
 goto L8c0942b4;
L8c0942b4: if(!(*sr&1u))goto L8c0942c4; /* bf 0x8c0942c4 */
 goto L8c0942b6;
L8c0942b6: *macl=r[7]*r[6]; /* mul.l r7,r6 */
 goto L8c0942b8;
L8c0942b8: r[1]=*macl; /* sts MACL,r1 */
 goto L8c0942ba;
L8c0942ba: r[1]+=r[0]; /* add r0,r1 */
 goto L8c0942bc;
L8c0942bc: r[3]=rd(m,r[1]+0x10u,4); /* mov.l @(0x10,r1),r3 */
 goto L8c0942be;
L8c0942be: *sr=(*sr&~1u)|(r[4]==r[3]); /* cmp/eq r4,r3 */
 goto L8c0942c0;
L8c0942c0: if(!(*sr&1u))goto L8c0942c4; /* bf 0x8c0942c4 */
 goto L8c0942c2;
L8c0942c2: r[13]=r[6]; /* mov r6,r13 */
 goto L8c0942c4;
L8c0942c4: r[6]+=1u; /* add #0x1,r6 */
 goto L8c0942c6;
L8c0942c6: *macl=r[7]*r[6]; /* mul.l r7,r6 */
 goto L8c0942c8;
L8c0942c8: r[3]=*macl; /* sts MACL,r3 */
 goto L8c0942ca;
L8c0942ca: r[3]+=r[0]; /* add r0,r3 */
 goto L8c0942cc;
L8c0942cc: r[2]=rd(m,r[3]+0x8u,4); /* mov.l @(0x8,r3),r2 */
 goto L8c0942ce;
L8c0942ce: *sr=(*sr&~1u)|(r[5]==r[2]); /* cmp/eq r5,r2 */
 goto L8c0942d0;
L8c0942d0: if(!(*sr&1u))goto L8c0942e0; /* bf 0x8c0942e0 */
 goto L8c0942d2;
L8c0942d2: *macl=r[7]*r[6]; /* mul.l r7,r6 */
 goto L8c0942d4;
L8c0942d4: r[1]=*macl; /* sts MACL,r1 */
 goto L8c0942d6;
L8c0942d6: r[1]+=r[0]; /* add r0,r1 */
 goto L8c0942d8;
L8c0942d8: r[3]=rd(m,r[1]+0x10u,4); /* mov.l @(0x10,r1),r3 */
 goto L8c0942da;
L8c0942da: *sr=(*sr&~1u)|(r[4]==r[3]); /* cmp/eq r4,r3 */
 goto L8c0942dc;
L8c0942dc: if(!(*sr&1u))goto L8c0942e0; /* bf 0x8c0942e0 */
 goto L8c0942de;
L8c0942de: r[13]=r[6]; /* mov r6,r13 */
 goto L8c0942e0;
L8c0942e0: r[6]+=1u; /* add #0x1,r6 */
 goto L8c0942e2;
L8c0942e2: *macl=r[7]*r[6]; /* mul.l r7,r6 */
 goto L8c0942e4;
L8c0942e4: r[3]=*macl; /* sts MACL,r3 */
 goto L8c0942e6;
L8c0942e6: r[3]+=r[0]; /* add r0,r3 */
 goto L8c0942e8;
L8c0942e8: r[2]=rd(m,r[3]+0x8u,4); /* mov.l @(0x8,r3),r2 */
 goto L8c0942ea;
L8c0942ea: *sr=(*sr&~1u)|(r[5]==r[2]); /* cmp/eq r5,r2 */
 goto L8c0942ec;
L8c0942ec: if(!(*sr&1u))goto L8c0942fc; /* bf 0x8c0942fc */
 goto L8c0942ee;
L8c0942ee: *macl=r[7]*r[6]; /* mul.l r7,r6 */
 goto L8c0942f0;
L8c0942f0: r[1]=*macl; /* sts MACL,r1 */
 goto L8c0942f2;
L8c0942f2: r[1]+=r[0]; /* add r0,r1 */
 goto L8c0942f4;
L8c0942f4: r[3]=rd(m,r[1]+0x10u,4); /* mov.l @(0x10,r1),r3 */
 goto L8c0942f6;
L8c0942f6: *sr=(*sr&~1u)|(r[4]==r[3]); /* cmp/eq r4,r3 */
 goto L8c0942f8;
L8c0942f8: if(!(*sr&1u))goto L8c0942fc; /* bf 0x8c0942fc */
 goto L8c0942fa;
L8c0942fa: r[13]=r[6]; /* mov r6,r13 */
 goto L8c0942fc;
L8c0942fc: r[6]+=1u; /* add #0x1,r6 */
 goto L8c0942fe;
L8c0942fe: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[12]); /* cmp/ge r12,r6 */
 goto L8c094300;
L8c094300: if(!(*sr&1u))goto L8c09428e; /* bf 0x8c09428e */
 goto L8c094302;
L8c094302: r[12]=37u; /* mov #0x25,r12 */
 goto L8c094304;
L8c094304: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[12]); /* cmp/ge r12,r6 */
 goto L8c094306;
L8c094306: if((*sr&1u))goto L8c094328; /* bt 0x8c094328 */
 goto L8c094308;
L8c094308: *macl=r[7]*r[6]; /* mul.l r7,r6 */
 goto L8c09430a;
L8c09430a: r[3]=*macl; /* sts MACL,r3 */
 goto L8c09430c;
L8c09430c: r[3]+=r[0]; /* add r0,r3 */
 goto L8c09430e;
L8c09430e: r[2]=rd(m,r[3]+0x8u,4); /* mov.l @(0x8,r3),r2 */
 goto L8c094310;
L8c094310: *sr=(*sr&~1u)|(r[5]==r[2]); /* cmp/eq r5,r2 */
 goto L8c094312;
L8c094312: if(!(*sr&1u))goto L8c094322; /* bf 0x8c094322 */
 goto L8c094314;
L8c094314: *macl=r[7]*r[6]; /* mul.l r7,r6 */
 goto L8c094316;
L8c094316: r[1]=*macl; /* sts MACL,r1 */
 goto L8c094318;
L8c094318: r[1]+=r[0]; /* add r0,r1 */
 goto L8c09431a;
L8c09431a: r[3]=rd(m,r[1]+0x10u,4); /* mov.l @(0x10,r1),r3 */
 goto L8c09431c;
L8c09431c: *sr=(*sr&~1u)|(r[4]==r[3]); /* cmp/eq r4,r3 */
 goto L8c09431e;
L8c09431e: if(!(*sr&1u))goto L8c094322; /* bf 0x8c094322 */
 goto L8c094320;
L8c094320: r[13]=r[6]; /* mov r6,r13 */
 goto L8c094322;
L8c094322: r[6]+=1u; /* add #0x1,r6 */
 goto L8c094324;
L8c094324: *sr=(*sr&~1u)|((int32_t)r[6]>=(int32_t)r[12]); /* cmp/ge r12,r6 */
 goto L8c094326;
L8c094326: if(!(*sr&1u))goto L8c094308; /* bf 0x8c094308 */
 goto L8c094328;
L8c094328: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c09432a;
L8c09432a: r[0]=r[13]; /* mov r13,r0 */
 goto L8c09432c;
L8c09432c: r[13]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
