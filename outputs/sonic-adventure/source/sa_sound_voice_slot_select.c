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
/* USA Rev A sound-work slot selector093f02. Provisional name; recovered integer control flow with native/reference verification. */
SA_STAGE_EXPORT int sa_sound_voice_slot_select(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c093f02;
L8c093f02: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c093f04;
L8c093f04: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c093f06;
L8c093f06: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c093f08;
L8c093f08: r[13]=32u; /* mov #0x20,r13 */
 goto L8c093f0a;
L8c093f0a: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c093f0c;
L8c093f0c: r[7]=4294967295u; /* mov #-0x1,r7 */
 goto L8c093f0e;
L8c093f0e: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c093f10;
L8c093f10: r[14]=rd(m,0x8c094078u,4); /* mov.l 0x8c094078,r14 */
 goto L8c093f12;
L8c093f12: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c093f14;
L8c093f14: r[9]=1u; /* mov #0x1,r9 */
 goto L8c093f16;
L8c093f16: take=(*sr&1u);r[6]=52u;if(take)goto L8c093fda;goto L8c093f1a; /* bt/s 0x8c093fda */
 goto L8c093f1a;
L8c093f1a: r[0]=r[5]; /* mov r5,r0 */
 goto L8c093f1c;
L8c093f1c: *sr=(*sr&~1u)|(r[0]==0x42u); /* cmp/eq #0x42,r0 */
 goto L8c093f1e;
L8c093f1e: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f20;
L8c093f20: r[0]=r[5]; /* mov r5,r0 */
 goto L8c093f22;
L8c093f22: *sr=(*sr&~1u)|(r[0]==0x43u); /* cmp/eq #0x43,r0 */
 goto L8c093f24;
L8c093f24: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f26;
L8c093f26: r[0]=r[5]; /* mov r5,r0 */
 goto L8c093f28;
L8c093f28: *sr=(*sr&~1u)|(r[0]==0x7cu); /* cmp/eq #0x7c,r0 */
 goto L8c093f2a;
L8c093f2a: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f2c;
L8c093f2c: r[3]=4294967201u; /* mov #-0x5f,r3 */
 goto L8c093f2e;
L8c093f2e: r[3]=r[3]&255u; /* extu.b r3,r3 */
 goto L8c093f30;
L8c093f30: *sr=(*sr&~1u)|(r[3]==r[5]); /* cmp/eq r3,r5 */
 goto L8c093f32;
L8c093f32: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f34;
L8c093f34: r[1]=4294967200u; /* mov #-0x60,r1 */
 goto L8c093f36;
L8c093f36: r[1]=r[1]&255u; /* extu.b r1,r1 */
 goto L8c093f38;
L8c093f38: *sr=(*sr&~1u)|(r[1]==r[5]); /* cmp/eq r1,r5 */
 goto L8c093f3a;
L8c093f3a: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f3c;
L8c093f3c: r[0]=4294967279u; /* mov #-0x11,r0 */
 goto L8c093f3e;
L8c093f3e: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c093f40;
L8c093f40: *sr=(*sr&~1u)|(r[0]==r[5]); /* cmp/eq r0,r5 */
 goto L8c093f42;
L8c093f42: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f44;
L8c093f44: r[2]=2u; /* mov #0x2,r2 */
 goto L8c093f46;
L8c093f46: r[2]<<=8; /* shll8 r2 */
 goto L8c093f48;
L8c093f48: r[2]+=41u; /* add #0x29,r2 */
 goto L8c093f4a;
L8c093f4a: *sr=(*sr&~1u)|(r[2]==r[5]); /* cmp/eq r2,r5 */
 goto L8c093f4c;
L8c093f4c: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f4e;
L8c093f4e: r[1]=2u; /* mov #0x2,r1 */
 goto L8c093f50;
L8c093f50: r[1]<<=8; /* shll8 r1 */
 goto L8c093f52;
L8c093f52: r[1]+=42u; /* add #0x2a,r1 */
 goto L8c093f54;
L8c093f54: *sr=(*sr&~1u)|(r[1]==r[5]); /* cmp/eq r1,r5 */
 goto L8c093f56;
L8c093f56: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f58;
L8c093f58: r[2]=4u; /* mov #0x4,r2 */
 goto L8c093f5a;
L8c093f5a: r[2]<<=8; /* shll8 r2 */
 goto L8c093f5c;
L8c093f5c: r[2]+=35u; /* add #0x23,r2 */
 goto L8c093f5e;
L8c093f5e: *sr=(*sr&~1u)|(r[2]==r[5]); /* cmp/eq r2,r5 */
 goto L8c093f60;
L8c093f60: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f62;
L8c093f62: r[1]=4u; /* mov #0x4,r1 */
 goto L8c093f64;
L8c093f64: r[1]<<=8; /* shll8 r1 */
 goto L8c093f66;
L8c093f66: r[1]+=15u; /* add #0xf,r1 */
 goto L8c093f68;
L8c093f68: *sr=(*sr&~1u)|(r[1]==r[5]); /* cmp/eq r1,r5 */
 goto L8c093f6a;
L8c093f6a: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f6c;
L8c093f6c: r[2]=4u; /* mov #0x4,r2 */
 goto L8c093f6e;
L8c093f6e: r[2]<<=8; /* shll8 r2 */
 goto L8c093f70;
L8c093f70: r[2]+=56u; /* add #0x38,r2 */
 goto L8c093f72;
L8c093f72: *sr=(*sr&~1u)|(r[2]==r[5]); /* cmp/eq r2,r5 */
 goto L8c093f74;
L8c093f74: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f76;
L8c093f76: r[1]=3u; /* mov #0x3,r1 */
 goto L8c093f78;
L8c093f78: r[1]<<=8; /* shll8 r1 */
 goto L8c093f7a;
L8c093f7a: r[1]+=4294967195u; /* add #-0x65,r1 */
 goto L8c093f7c;
L8c093f7c: *sr=(*sr&~1u)|(r[1]==r[5]); /* cmp/eq r1,r5 */
 goto L8c093f7e;
L8c093f7e: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f80;
L8c093f80: r[2]=4u; /* mov #0x4,r2 */
 goto L8c093f82;
L8c093f82: r[2]<<=8; /* shll8 r2 */
 goto L8c093f84;
L8c093f84: r[2]+=3u; /* add #0x3,r2 */
 goto L8c093f86;
L8c093f86: *sr=(*sr&~1u)|(r[2]==r[5]); /* cmp/eq r2,r5 */
 goto L8c093f88;
L8c093f88: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f8a;
L8c093f8a: r[1]=4u; /* mov #0x4,r1 */
 goto L8c093f8c;
L8c093f8c: r[1]<<=8; /* shll8 r1 */
 goto L8c093f8e;
L8c093f8e: r[1]+=5u; /* add #0x5,r1 */
 goto L8c093f90;
L8c093f90: *sr=(*sr&~1u)|(r[1]==r[5]); /* cmp/eq r1,r5 */
 goto L8c093f92;
L8c093f92: if((*sr&1u))goto L8c093fda; /* bt 0x8c093fda */
 goto L8c093f94;
L8c093f94: r[2]=3u; /* mov #0x3,r2 */
 goto L8c093f96;
L8c093f96: r[2]<<=8; /* shll8 r2 */
 goto L8c093f98;
L8c093f98: r[2]+=9u; /* add #0x9,r2 */
 goto L8c093f9a;
L8c093f9a: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[2]); /* cmp/ge r2,r5 */
 goto L8c093f9c;
L8c093f9c: if(!(*sr&1u))goto L8c093fa8; /* bf 0x8c093fa8 */
 goto L8c093f9e;
L8c093f9e: r[1]=3u; /* mov #0x3,r1 */
 goto L8c093fa0;
L8c093fa0: r[1]<<=8; /* shll8 r1 */
 goto L8c093fa2;
L8c093fa2: r[1]+=13u; /* add #0xd,r1 */
 goto L8c093fa4;
L8c093fa4: *sr=(*sr&~1u)|((int32_t)r[5]>(int32_t)r[1]); /* cmp/gt r1,r5 */
 goto L8c093fa6;
L8c093fa6: if(!(*sr&1u))goto L8c093fda; /* bf 0x8c093fda */
 goto L8c093fa8;
L8c093fa8: r[2]=5u; /* mov #0x5,r2 */
 goto L8c093faa;
L8c093faa: r[2]<<=8; /* shll8 r2 */
 goto L8c093fac;
L8c093fac: r[2]+=4294967231u; /* add #-0x41,r2 */
 goto L8c093fae;
L8c093fae: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[2]); /* cmp/ge r2,r5 */
 goto L8c093fb0;
L8c093fb0: if(!(*sr&1u))goto L8c093fbc; /* bf 0x8c093fbc */
 goto L8c093fb2;
L8c093fb2: r[1]=5u; /* mov #0x5,r1 */
 goto L8c093fb4;
L8c093fb4: r[1]<<=8; /* shll8 r1 */
 goto L8c093fb6;
L8c093fb6: r[1]+=4294967238u; /* add #-0x3a,r1 */
 goto L8c093fb8;
L8c093fb8: *sr=(*sr&~1u)|((int32_t)r[5]>(int32_t)r[1]); /* cmp/gt r1,r5 */
 goto L8c093fba;
L8c093fba: if(!(*sr&1u))goto L8c093fda; /* bf 0x8c093fda */
 goto L8c093fbc;
L8c093fbc: r[2]=5u; /* mov #0x5,r2 */
 goto L8c093fbe;
L8c093fbe: r[2]<<=8; /* shll8 r2 */
 goto L8c093fc0;
L8c093fc0: r[2]+=4294967280u; /* add #-0x10,r2 */
 goto L8c093fc2;
L8c093fc2: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[2]); /* cmp/ge r2,r5 */
 goto L8c093fc4;
L8c093fc4: if(!(*sr&1u))goto L8c093fd0; /* bf 0x8c093fd0 */
 goto L8c093fc6;
L8c093fc6: r[1]=5u; /* mov #0x5,r1 */
 goto L8c093fc8;
L8c093fc8: r[1]<<=8; /* shll8 r1 */
 goto L8c093fca;
L8c093fca: r[1]+=4294967287u; /* add #-0x9,r1 */
 goto L8c093fcc;
L8c093fcc: *sr=(*sr&~1u)|((int32_t)r[5]>(int32_t)r[1]); /* cmp/gt r1,r5 */
 goto L8c093fce;
L8c093fce: if(!(*sr&1u))goto L8c093fda; /* bf 0x8c093fda */
 goto L8c093fd0;
L8c093fd0: r[2]=5u; /* mov #0x5,r2 */
 goto L8c093fd2;
L8c093fd2: r[2]<<=8; /* shll8 r2 */
 goto L8c093fd4;
L8c093fd4: r[2]+=4294967289u; /* add #-0x7,r2 */
 goto L8c093fd6;
L8c093fd6: *sr=(*sr&~1u)|(r[2]==r[5]); /* cmp/eq r2,r5 */
 goto L8c093fd8;
L8c093fd8: if(!(*sr&1u))goto L8c094080; /* bf 0x8c094080 */
 goto L8c093fda;
L8c093fda: r[10]=rd(m,0x8c09407cu,4); /* mov.l 0x8c09407c,r10 */
 goto L8c093fdc;
L8c093fdc: *sr=(*sr&~1u)|!(r[9]&r[9]); /* tst r9,r9 */
 goto L8c093fde;
L8c093fde: r[12]=37u; /* mov #0x25,r12 */
 goto L8c093fe0;
L8c093fe0: r[5]=rd(m,r[10],4); /* mov.l @r10,r5 */
 goto L8c093fe2;
L8c093fe2: take=(*sr&1u);r[0]=r[12];if(take)goto L8c09401a;goto L8c093fe6; /* bt/s 0x8c09401a */
 goto L8c093fe6;
L8c093fe6: *sr=(*sr&~1u)|!(r[9]&r[9]); /* tst r9,r9 */
 goto L8c093fe8;
L8c093fe8: if((*sr&1u))goto L8c09401a; /* bt 0x8c09401a */
 goto L8c093fea;
L8c093fea: *macl=r[6]*r[5]; /* mul.l r6,r5 */
 goto L8c093fec;
L8c093fec: r[2]=*macl; /* sts MACL,r2 */
 goto L8c093fee;
L8c093fee: r[2]+=r[14]; /* add r14,r2 */
 goto L8c093ff0;
L8c093ff0: r[3]=rd(m,r[2]+0x4u,4); /* mov.l @(0x4,r2),r3 */
 goto L8c093ff2;
L8c093ff2: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c093ff4;
L8c093ff4: take=!(*sr&1u);*macl=r[6]*r[5];if(take)goto L8c093ffa;goto L8c093ff8; /* bf/s 0x8c093ffa */
 goto L8c093ff8;
L8c093ff8: r[7]=r[5]; /* mov r5,r7 */
 goto L8c093ffa;
L8c093ffa: r[3]=*macl; /* sts MACL,r3 */
 goto L8c093ffc;
L8c093ffc: r[3]+=r[14]; /* add r14,r3 */
 goto L8c093ffe;
L8c093ffe: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c094000;
L8c094000: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[4]); /* cmp/gt r4,r2 */
 goto L8c094002;
L8c094002: if((*sr&1u))goto L8c094006; /* bt 0x8c094006 */
 goto L8c094004;
L8c094004: r[7]=r[5]; /* mov r5,r7 */
 goto L8c094006;
L8c094006: r[5]+=1u; /* add #0x1,r5 */
 goto L8c094008;
L8c094008: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[12]); /* cmp/ge r12,r5 */
 goto L8c09400a;
L8c09400a: take=!(*sr&1u);r[0]+=4294967295u;if(take)goto L8c094010;goto L8c09400e; /* bf/s 0x8c094010 */
 goto L8c09400e;
L8c09400e: r[5]=r[13]; /* mov r13,r5 */
 goto L8c094010;
L8c094010: *sr=(*sr&~1u)|((int32_t)r[7]>=0); /* cmp/pz r7 */
 goto L8c094012;
L8c094012: if((*sr&1u))goto L8c09401a; /* bt 0x8c09401a */
 goto L8c094014;
L8c094014: r[2]=32u; /* mov #0x20,r2 */
 goto L8c094016;
L8c094016: *sr=(*sr&~1u)|((int32_t)r[0]>(int32_t)r[2]); /* cmp/gt r2,r0 */
 goto L8c094018;
L8c094018: if((*sr&1u))goto L8c093fea; /* bt 0x8c093fea */
 goto L8c09401a;
L8c09401a: r[1]=r[7]; /* mov r7,r1 */
 goto L8c09401c;
L8c09401c: r[1]+=1u; /* add #0x1,r1 */
 goto L8c09401e;
L8c09401e: r[3]=r[1]; /* mov r1,r3 */
 goto L8c094020;
L8c094020: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[12]); /* cmp/ge r12,r3 */
 goto L8c094022;
L8c094022: take=!(*sr&1u);wr(m,r[10],r[1],4);if(take)goto L8c0940cc;goto L8c094026; /* bf/s 0x8c0940cc */
 goto L8c094026;
L8c094026: wr(m,r[10],r[13],4);goto L8c0940cc; /* bra 0x8c0940cc */
 goto L8c094080;
L8c094080: r[10]=rd(m,0x8c094220u,4); /* mov.l 0x8c094220,r10 */
 goto L8c094082;
L8c094082: *sr=(*sr&~1u)|!(r[9]&r[9]); /* tst r9,r9 */
 goto L8c094084;
L8c094084: r[0]=r[13]; /* mov r13,r0 */
 goto L8c094086;
L8c094086: r[5]=rd(m,r[10],4); /* mov.l @r10,r5 */
 goto L8c094088;
L8c094088: take=(*sr&1u);r[12]=0u;if(take)goto L8c0940be;goto L8c09408c; /* bt/s 0x8c0940be */
 goto L8c09408c;
L8c09408c: *sr=(*sr&~1u)|!(r[9]&r[9]); /* tst r9,r9 */
 goto L8c09408e;
L8c09408e: if((*sr&1u))goto L8c0940be; /* bt 0x8c0940be */
 goto L8c094090;
L8c094090: *macl=r[6]*r[5]; /* mul.l r6,r5 */
 goto L8c094092;
L8c094092: r[2]=*macl; /* sts MACL,r2 */
 goto L8c094094;
L8c094094: r[2]+=r[14]; /* add r14,r2 */
 goto L8c094096;
L8c094096: r[3]=rd(m,r[2]+0x4u,4); /* mov.l @(0x4,r2),r3 */
 goto L8c094098;
L8c094098: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c09409a;
L8c09409a: take=!(*sr&1u);*macl=r[6]*r[5];if(take)goto L8c0940a0;goto L8c09409e; /* bf/s 0x8c0940a0 */
 goto L8c09409e;
L8c09409e: r[7]=r[5]; /* mov r5,r7 */
 goto L8c0940a0;
L8c0940a0: r[3]=*macl; /* sts MACL,r3 */
 goto L8c0940a2;
L8c0940a2: r[3]+=r[14]; /* add r14,r3 */
 goto L8c0940a4;
L8c0940a4: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c0940a6;
L8c0940a6: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[4]); /* cmp/gt r4,r2 */
 goto L8c0940a8;
L8c0940a8: if((*sr&1u))goto L8c0940ac; /* bt 0x8c0940ac */
 goto L8c0940aa;
L8c0940aa: r[7]=r[5]; /* mov r5,r7 */
 goto L8c0940ac;
L8c0940ac: r[5]+=1u; /* add #0x1,r5 */
 goto L8c0940ae;
L8c0940ae: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[13]); /* cmp/ge r13,r5 */
 goto L8c0940b0;
L8c0940b0: take=!(*sr&1u);r[0]+=4294967295u;if(take)goto L8c0940b6;goto L8c0940b4; /* bf/s 0x8c0940b6 */
 goto L8c0940b4;
L8c0940b4: r[5]=r[12]; /* mov r12,r5 */
 goto L8c0940b6;
L8c0940b6: *sr=(*sr&~1u)|((int32_t)r[7]>=0); /* cmp/pz r7 */
 goto L8c0940b8;
L8c0940b8: if((*sr&1u))goto L8c0940be; /* bt 0x8c0940be */
 goto L8c0940ba;
L8c0940ba: *sr=(*sr&~1u)|((int32_t)r[0]>0); /* cmp/pl r0 */
 goto L8c0940bc;
L8c0940bc: if((*sr&1u))goto L8c094090; /* bt 0x8c094090 */
 goto L8c0940be;
L8c0940be: r[3]=r[7]; /* mov r7,r3 */
 goto L8c0940c0;
L8c0940c0: r[3]+=1u; /* add #0x1,r3 */
 goto L8c0940c2;
L8c0940c2: r[2]=r[3]; /* mov r3,r2 */
 goto L8c0940c4;
L8c0940c4: *sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[13]); /* cmp/ge r13,r2 */
 goto L8c0940c6;
L8c0940c6: wr(m,r[10],r[3],4); /* mov.l r3,@r10 */
 goto L8c0940c8;
L8c0940c8: if(!(*sr&1u))goto L8c0940cc; /* bf 0x8c0940cc */
 goto L8c0940ca;
L8c0940ca: wr(m,r[10],r[12],4); /* mov.l r12,@r10 */
 goto L8c0940cc;
L8c0940cc: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c0940ce;
L8c0940ce: r[0]=r[7]; /* mov r7,r0 */
 goto L8c0940d0;
L8c0940d0: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c0940d2;
L8c0940d2: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c0940d4;
L8c0940d4: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c0940d6;
L8c0940d6: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
