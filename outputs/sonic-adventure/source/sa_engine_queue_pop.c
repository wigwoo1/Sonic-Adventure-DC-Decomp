#include "fpu_cleanup_transfer.h"
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
/* Static C recovery of the USA Rev A main executable ring queue pop. External calls use the supplied dispatcher. */
SA_STAGE_EXPORT int sa_engine_queue_pop(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c666f98;
L8c666f98: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c666f9a;
L8c666f9a: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c666f9c;
L8c666f9c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c666f9e;
L8c666f9e: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c666fa0;
L8c666fa0: r[0]=*sr; /* stc SR,r0 */
 goto L8c666fa2;
L8c666fa2: r[3]=sx(m,0x8c66706au,2); /* mov.w 0x8c66706a,r3 */
 goto L8c666fa4;
L8c666fa4: r[13]=0u; /* mov #0x0,r13 */
 goto L8c666fa6;
L8c666fa6: r[14]=r[13]; /* mov r13,r14 */
 goto L8c666fa8;
L8c666fa8: r[0]>>=2; /* shlr2 r0 */
 goto L8c666faa;
L8c666faa: r[0]>>=2; /* shlr2 r0 */
 goto L8c666fac;
L8c666fac: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c666fae;
L8c666fae: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c666fb0;
L8c666fb0: r[0]=*sr; /* stc SR,r0 */
 goto L8c666fb2;
L8c666fb2: r[0]&=r[3]; /* and r3,r0 */
 goto L8c666fb4;
L8c666fb4: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c666fb6;
L8c666fb6: *sr=r[0]; /* ldc r0,SR */
 goto L8c666fb8;
L8c666fb8: r[6]=rd(m,0x8c667070u,4); /* mov.l 0x8c667070,r6 */
 goto L8c666fba;
L8c666fba: r[7]=rd(m,r[6],4); /* mov.l @r6,r7 */
 goto L8c666fbc;
L8c666fbc: *sr=(*sr&~1u)|!(r[7]&r[7]); /* tst r7,r7 */
 goto L8c666fbe;
L8c666fbe: if(!(*sr&1u))goto L8c666fc6; /* bf 0x8c666fc6 */
 goto L8c666fc0;
L8c666fc0: r[14]=rd(m,0x8c667074u,4); /* mov.l 0x8c667074,r14 */
 goto L8c666fc2;
L8c666fc2: goto L8c667040; /* bra 0x8c667040 */
 goto L8c666fc6;
L8c666fc6: r[0]=sx(m,0x8c66706cu,2); /* mov.w 0x8c66706c,r0 */
 goto L8c666fc8;
L8c666fc8: r[2]=rd(m,r[7]+r[0],4); /* mov.l @(r0,r7),r2 */
 goto L8c666fca;
L8c666fca: r[0]+=4294967284u; /* add #-0xc,r0 */
 goto L8c666fcc;
L8c666fcc: r[3]=rd(m,r[7]+r[0],4); /* mov.l @(r0,r7),r3 */
 goto L8c666fce;
L8c666fce: r[0]+=12u; /* add #0xc,r0 */
 goto L8c666fd0;
L8c666fd0: r[2]<<=2; /* shll2 r2 */
 goto L8c666fd2;
L8c666fd2: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c666fd4;
L8c666fd4: r[2]+=r[3]; /* add r3,r2 */
 goto L8c666fd6;
L8c666fd6: r[1]=rd(m,r[2],4); /* mov.l @r2,r1 */
 goto L8c666fd8;
L8c666fd8: wr(m,r[4],r[1],4); /* mov.l r1,@r4 */
 goto L8c666fda;
L8c666fda: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c666fdc;
L8c666fdc: r[2]=r[3]; /* mov r3,r2 */
 goto L8c666fde;
L8c666fde: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c666fe0;
L8c666fe0: r[0]+=4294967284u; /* add #-0xc,r0 */
 goto L8c666fe2;
L8c666fe2: r[3]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r3 */
 goto L8c666fe4;
L8c666fe4: r[1]<<=2; /* shll2 r1 */
 goto L8c666fe6;
L8c666fe6: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c666fe8;
L8c666fe8: r[1]+=r[3]; /* add r3,r1 */
 goto L8c666fea;
L8c666fea: r[2]=rd(m,r[1]+0x4u,4); /* mov.l @(0x4,r1),r2 */
 goto L8c666fec;
L8c666fec: r[0]+=12u; /* add #0xc,r0 */
 goto L8c666fee;
L8c666fee: wr(m,r[5],r[2],4); /* mov.l r2,@r5 */
 goto L8c666ff0;
L8c666ff0: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c666ff2;
L8c666ff2: r[2]=r[3]; /* mov r3,r2 */
 goto L8c666ff4;
L8c666ff4: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c666ff6;
L8c666ff6: r[0]+=4294967284u; /* add #-0xc,r0 */
 goto L8c666ff8;
L8c666ff8: r[3]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r3 */
 goto L8c666ffa;
L8c666ffa: r[1]<<=2; /* shll2 r1 */
 goto L8c666ffc;
L8c666ffc: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c666ffe;
L8c666ffe: r[1]+=r[3]; /* add r3,r1 */
 goto L8c667000;
L8c667000: wr(m,r[1],r[13],4); /* mov.l r13,@r1 */
 goto L8c667002;
L8c667002: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c667004;
L8c667004: r[2]=r[3]; /* mov r3,r2 */
 goto L8c667006;
L8c667006: r[0]+=12u; /* add #0xc,r0 */
 goto L8c667008;
L8c667008: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c66700a;
L8c66700a: r[0]+=4294967284u; /* add #-0xc,r0 */
 goto L8c66700c;
L8c66700c: r[3]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r3 */
 goto L8c66700e;
L8c66700e: r[0]+=12u; /* add #0xc,r0 */
 goto L8c667010;
L8c667010: r[1]<<=2; /* shll2 r1 */
 goto L8c667012;
L8c667012: *sr=(*sr&~1u)|(r[1]>>31);r[1]<<=1; /* shll r1 */
 goto L8c667014;
L8c667014: r[1]+=r[3]; /* add r3,r1 */
 goto L8c667016;
L8c667016: wr(m,r[1]+0x4u,r[13],4); /* mov.l r13,@(0x4,r1) */
 goto L8c667018;
L8c667018: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c66701a;
L8c66701a: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c66701c;
L8c66701c: r[2]+=1u; /* add #0x1,r2 */
 goto L8c66701e;
L8c66701e: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c667020;
L8c667020: r[0]+=4294967288u; /* add #-0x8,r0 */
 goto L8c667022;
L8c667022: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c667024;
L8c667024: r[2]=r[3]; /* mov r3,r2 */
 goto L8c667026;
L8c667026: r[1]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r1 */
 goto L8c667028;
L8c667028: r[0]+=8u; /* add #0x8,r0 */
 goto L8c66702a;
L8c66702a: r[3]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r3 */
 goto L8c66702c;
L8c66702c: *sr=(*sr&~1u)|(r[1]==r[3]); /* cmp/eq r1,r3 */
 goto L8c66702e;
L8c66702e: if(!(*sr&1u))goto L8c667036; /* bf 0x8c667036 */
 goto L8c667030;
L8c667030: r[0]=sx(m,0x8c66706cu,2); /* mov.w 0x8c66706c,r0 */
 goto L8c667032;
L8c667032: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c667034;
L8c667034: wr(m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c667036;
L8c667036: r[0]=sx(m,0x8c66706eu,2); /* mov.w 0x8c66706e,r0 */
 goto L8c667038;
L8c667038: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c66703a;
L8c66703a: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c66703c;
L8c66703c: r[2]+=4294967295u; /* add #-0x1,r2 */
 goto L8c66703e;
L8c66703e: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c667040;
L8c667040: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c667042;
L8c667042: if((*sr&1u))goto L8c66704c; /* bt 0x8c66704c */
 goto L8c667044;
L8c667044: r[2]=rd(m,0x8c66707cu,4); /* mov.l 0x8c66707c,r2 */
 goto L8c667046;
L8c667046: r[4]=rd(m,0x8c667078u,4); /* mov.l 0x8c667078,r4 */
 goto L8c667048;
L8c667048: target=r[2];*pr=0x8c66704cu;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66704c;
L8c66704c: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c66704e;
L8c66704e: r[3]=*sr; /* stc SR,r3 */
 goto L8c667050;
L8c667050: r[2]=sx(m,0x8c66706au,2); /* mov.w 0x8c66706a,r2 */
 goto L8c667052;
L8c667052: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c667054;
L8c667054: r[0]<<=2; /* shll2 r0 */
 goto L8c667056;
L8c667056: r[3]&=r[2]; /* and r2,r3 */
 goto L8c667058;
L8c667058: r[0]<<=2; /* shll2 r0 */
 goto L8c66705a;
L8c66705a: r[0]|=r[3]; /* or r3,r0 */
 goto L8c66705c;
L8c66705c: *sr=r[0]; /* ldc r0,SR */
 goto L8c66705e;
L8c66705e: r[0]=r[14]; /* mov r14,r0 */
 goto L8c667060;
L8c667060: r[15]+=4u; /* add #0x4,r15 */
 goto L8c667062;
L8c667062: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c667064;
L8c667064: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c667066;
L8c667066: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
