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
#include "sh4_compare_bits.h"
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){return sa_sh4_compare_bits(a,b,&f->fpscr,sr,1);}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){return sa_sh4_compare_bits(a,b,&f->fpscr,sr,0);}
/* Static C recovery of retail Tikal helper 8c10ef84..8c10f114.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_atan_core(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ef84;
L8c10ef84: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10ef86;
L8c10ef86: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c10ef88;
L8c10ef88: r[15]-=4;wr(m,r[15],fr[15],4); /* fmov.s fr15,@-r15 */
 goto L8c10ef8a;
L8c10ef8a: r[15]-=4;wr(m,r[15],fr[14],4); /* fmov.s fr14,@-r15 */
 goto L8c10ef8c;
L8c10ef8c: r[15]-=4;wr(m,r[15],fr[13],4); /* fmov.s fr13,@-r15 */
 goto L8c10ef8e;
L8c10ef8e: r[15]-=4;wr(m,r[15],fr[12],4); /* fmov.s fr12,@-r15 */
 goto L8c10ef90;
L8c10ef90: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10ef92;
L8c10ef92: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c10ef94;
L8c10ef94: r[0]=8u; /* mov #0x8,r0 */
 goto L8c10ef96;
L8c10ef96: fr[5]=fr[4]; /* fmov fr4,fr5 */
 goto L8c10ef98;
L8c10ef98: wr(m,r[0]+r[15],fr[5],4); /* fmov fr5,@(r0,r15) */
 goto L8c10ef9a;
L8c10ef9a: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c10ef9c;
L8c10ef9c: *sr=(*sr&~1u)|((int32_t)r[3]>=0); /* cmp/pz r3 */
 goto L8c10ef9e;
L8c10ef9e: take=(*sr&1u);r[4]=0u;if(take)goto L8c10efa4;goto L8c10efa2; /* bt/s 0x8c10efa4 */
 goto L8c10efa2;
L8c10efa2: r[4]=8u; /* mov #0x8,r4 */
 goto L8c10efa4;
L8c10efa4: fr[3]=0; /* fldi0 fr3 */
 goto L8c10efa6;
L8c10efa6: status=eq(f,sr,fr[5],fr[3]);if(status)return status; /* fcmp/eq fr3,fr5 */
 goto L8c10efa8;
L8c10efa8: r[7]=4u; /* mov #0x4,r7 */
 goto L8c10efaa;
L8c10efaa: take=!(*sr&1u);r[6]=1u;if(take)goto L8c10efb2;goto L8c10efae; /* bf/s 0x8c10efb2 */
 goto L8c10efae;
L8c10efae: r[4]|=r[7];goto L8c10efca; /* bra 0x8c10efca */
 goto L8c10efb2;
L8c10efb2: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c10efb4;
L8c10efb4: r[3]=rd(m,0x8c10f114u,4); /* mov.l 0x8c10f114,r3 */
 goto L8c10efb6;
L8c10efb6: r[1]=rd(m,0x8c10f118u,4); /* mov.l 0x8c10f118,r1 */
 goto L8c10efb8;
L8c10efb8: r[2]&=r[3]; /* and r3,r2 */
 goto L8c10efba;
L8c10efba: *sr=(*sr&~1u)|(r[1]==r[2]); /* cmp/eq r1,r2 */
 goto L8c10efbc;
L8c10efbc: if(!(*sr&1u))goto L8c10efc4; /* bf 0x8c10efc4 */
 goto L8c10efbe;
L8c10efbe: r[0]=2u; /* mov #0x2,r0 */
 goto L8c10efc0;
L8c10efc0: r[4]|=r[0];goto L8c10efca; /* bra 0x8c10efca */
 goto L8c10efc4;
L8c10efc4: status=eq(f,sr,fr[5],fr[5]);if(status)return status; /* fcmp/eq fr5,fr5 */
 goto L8c10efc6;
L8c10efc6: if((*sr&1u))goto L8c10efca; /* bt 0x8c10efca */
 goto L8c10efc8;
L8c10efc8: r[4]|=r[6]; /* or r6,r4 */
 goto L8c10efca;
L8c10efca: r[3]=r[15]; /* mov r15,r3 */
 goto L8c10efcc;
L8c10efcc: r[2]=rd(m,0x8c10f11cu,4); /* mov.l 0x8c10f11c,r2 */
 goto L8c10efce;
L8c10efce: wr(m,r[15],fr[4],4); /* fmov.s fr4,@r15 */
 goto L8c10efd0;
L8c10efd0: r[1]=rd(m,r[3],4); /* mov.l @r3,r1 */
 goto L8c10efd2;
L8c10efd2: *sr=(*sr&~1u)|!(r[2]&r[1]); /* tst r2,r1 */
 goto L8c10efd4;
L8c10efd4: take=(*sr&1u);r[5]=r[4];if(take)goto L8c10efde;goto L8c10efd8; /* bt/s 0x8c10efde */
 goto L8c10efd8;
L8c10efd8: fr[3]=rd(m,r[15],4); /* fmov.s @r15,fr3 */
 goto L8c10efda;
L8c10efda: fr[3]^=0x80000000u; /* fneg fr3 */
 goto L8c10efdc;
L8c10efdc: wr(m,r[15],fr[3],4); /* fmov.s fr3,@r15 */
 goto L8c10efde;
L8c10efde: *sr=(*sr&~1u)|!(r[4]&r[6]); /* tst r4,r6 */
 goto L8c10efe0;
L8c10efe0: if((*sr&1u))goto L8c10efe8; /* bt 0x8c10efe8 */
 goto L8c10efe2;
L8c10efe2: r[2]=rd(m,0x8c10f120u,4); /* mov.l 0x8c10f120,r2 */
 goto L8c10efe4;
L8c10efe4: fr[5]=rd(m,r[2],4);goto L8c10f100; /* bra 0x8c10f100 */
 goto L8c10efe8;
L8c10efe8: r[2]=r[15]; /* mov r15,r2 */
 goto L8c10efea;
L8c10efea: r[1]=rd(m,r[2],4); /* mov.l @r2,r1 */
 goto L8c10efec;
L8c10efec: r[3]=rd(m,0x8c10f118u,4); /* mov.l 0x8c10f118,r3 */
 goto L8c10efee;
L8c10efee: r[2]=4294967273u; /* mov #-0x17,r2 */
 goto L8c10eff0;
L8c10eff0: r[0]=32u; /* mov #0x20,r0 */
 goto L8c10eff2;
L8c10eff2: r[1]&=r[3]; /* and r3,r1 */
 goto L8c10eff4;
L8c10eff4: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[1]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[1]=n?(uint32_t)((int32_t)r[1]>>n):((r[1]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r1 */
 goto L8c10eff6;
L8c10eff6: r[1]+=4294967169u; /* add #-0x7f,r1 */
 goto L8c10eff8;
L8c10eff8: *sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[0]); /* cmp/ge r0,r1 */
 goto L8c10effa;
L8c10effa: r[13]=8u; /* mov #0x8,r13 */
 goto L8c10effc;
L8c10effc: take=!(*sr&1u);r[13]&=r[4];if(take)goto L8c10f010;goto L8c10f000; /* bf/s 0x8c10f010 */
 goto L8c10f000;
L8c10f000: r[1]=rd(m,0x8c10f124u,4); /* mov.l 0x8c10f124,r1 */
 goto L8c10f002;
L8c10f002: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c10f004;
L8c10f004: take=!(*sr&1u);fr[5]=rd(m,r[1],4);if(take)goto L8c10f00c;goto L8c10f008; /* bf/s 0x8c10f00c */
 goto L8c10f008;
L8c10f008: goto L8c10f100; /* bra 0x8c10f100 */
 goto L8c10f00c;
L8c10f00c: fr[5]^=0x80000000u;goto L8c10f100; /* bra 0x8c10f100 */
 goto L8c10f010;
L8c10f010: *sr=(*sr&~1u)|!(r[7]&r[4]); /* tst r7,r4 */
 goto L8c10f012;
L8c10f012: if((*sr&1u))goto L8c10f018; /* bt 0x8c10f018 */
 goto L8c10f014;
L8c10f014: fr[5]=fr[4];goto L8c10f100; /* bra 0x8c10f100 */
 goto L8c10f018;
L8c10f018: r[1]=rd(m,0x8c10f128u,4); /* mov.l 0x8c10f128,r1 */
 goto L8c10f01a;
L8c10f01a: fr[3]=rd(m,r[15],4); /* fmov.s @r15,fr3 */
 goto L8c10f01c;
L8c10f01c: fr[5]=rd(m,r[1],4); /* fmov.s @r1,fr5 */
 goto L8c10f01e;
L8c10f01e: r[3]=rd(m,0x8c10f12cu,4); /* mov.l 0x8c10f12c,r3 */
 goto L8c10f020;
L8c10f020: status=gt(f,sr,fr[5],fr[3]);if(status)return status; /* fcmp/gt fr3,fr5 */
 goto L8c10f022;
L8c10f022: take=!(*sr&1u);fr[4]=rd(m,r[3],4);if(take)goto L8c10f02e;goto L8c10f026; /* bf/s 0x8c10f02e */
 goto L8c10f026;
L8c10f026: r[2]=rd(m,0x8c10f130u,4); /* mov.l 0x8c10f130,r2 */
 goto L8c10f028;
L8c10f028: fr[14]=rd(m,r[2],4); /* fmov.s @r2,fr14 */
 goto L8c10f02a;
L8c10f02a: fr[15]=fr[3];goto L8c10f04c; /* bra 0x8c10f04c */
 goto L8c10f02e;
L8c10f02e: status=eq(f,sr,fr[5],fr[3]);if(status)return status; /* fcmp/eq fr3,fr5 */
 goto L8c10f030;
L8c10f030: if(!(*sr&1u))goto L8c10f03c; /* bf 0x8c10f03c */
 goto L8c10f032;
L8c10f032: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c10f034;
L8c10f034: if((*sr&1u))goto L8c10f038; /* bt 0x8c10f038 */
 goto L8c10f036;
L8c10f036: fr[4]^=0x80000000u; /* fneg fr4 */
 goto L8c10f038;
L8c10f038: fr[0]=fr[4];goto L8c10f102; /* bra 0x8c10f102 */
 goto L8c10f03c;
L8c10f03c: r[2]=rd(m,0x8c10f134u,4); /* mov.l 0x8c10f134,r2 */
 goto L8c10f03e;
L8c10f03e: fr[3]=rd(m,r[15],4); /* fmov.s @r15,fr3 */
 goto L8c10f040;
L8c10f040: fr[2]=rd(m,r[2],4); /* fmov.s @r2,fr2 */
 goto L8c10f042;
L8c10f042: status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],0);if(status)return status; /* fadd fr3,fr5 */
 goto L8c10f044;
L8c10f044: fr[14]=fr[4]; /* fmov fr4,fr14 */
 goto L8c10f046;
L8c10f046: status=sa_sh4_add_sub(&fr[2],&fr[3],&f->fpscr,&fr[2],0);if(status)return status; /* fadd fr3,fr2 */
 goto L8c10f048;
L8c10f048: fr[15]=fr[2]; /* fmov fr2,fr15 */
 goto L8c10f04a;
L8c10f04a: status=sa_sh4_divide(&fr[15],&fr[5],&f->fpscr,&fr[15]);if(status)return status; /* fdiv fr5,fr15 */
 goto L8c10f04c;
L8c10f04c: r[3]=rd(m,0x8c10f138u,4); /* mov.l 0x8c10f138,r3 */
 goto L8c10f04e;
L8c10f04e: r[4]=r[15]; /* mov r15,r4 */
 goto L8c10f050;
L8c10f050: r[2]=rd(m,0x8c10f13cu,4); /* mov.l 0x8c10f13c,r2 */
 goto L8c10f052;
L8c10f052: r[4]+=4u; /* add #0x4,r4 */
 goto L8c10f054;
L8c10f054: fr[5]=rd(m,r[3],4); /* fmov.s @r3,fr5 */
 goto L8c10f056;
L8c10f056: target=r[2];*pr=0x8c10f05au;fr[4]=fr[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c10f05a;
L8c10f05a: r[0]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c10f05c;
L8c10f05c: r[3]=rd(m,0x8c10f128u,4); /* mov.l 0x8c10f128,r3 */
 goto L8c10f05e;
L8c10f05e: r[14]=rd(m,0x8c10f140u,4); /* mov.l 0x8c10f140,r14 */
 goto L8c10f060;
L8c10f060: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c10f062;
L8c10f062: take=(*sr&1u);fr[13]=rd(m,r[3],4);if(take)goto L8c10f086;goto L8c10f066; /* bt/s 0x8c10f086 */
 goto L8c10f066;
L8c10f066: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10f068;
L8c10f068: if((*sr&1u))goto L8c10f08a; /* bt 0x8c10f08a */
 goto L8c10f06a;
L8c10f06a: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c10f06c;
L8c10f06c: if((*sr&1u))goto L8c10f08a; /* bt 0x8c10f08a */
 goto L8c10f06e;
L8c10f06e: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c10f070;
L8c10f070: if((*sr&1u))goto L8c10f0a8; /* bt 0x8c10f0a8 */
 goto L8c10f072;
L8c10f072: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c10f074;
L8c10f074: if((*sr&1u))goto L8c10f0a8; /* bt 0x8c10f0a8 */
 goto L8c10f076;
L8c10f076: *sr=(*sr&~1u)|(r[0]==0x5u); /* cmp/eq #0x5,r0 */
 goto L8c10f078;
L8c10f078: if((*sr&1u))goto L8c10f0c6; /* bt 0x8c10f0c6 */
 goto L8c10f07a;
L8c10f07a: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c10f07c;
L8c10f07c: if((*sr&1u))goto L8c10f0c6; /* bt 0x8c10f0c6 */
 goto L8c10f07e;
L8c10f07e: *sr=(*sr&~1u)|(r[0]==0x7u); /* cmp/eq #0x7,r0 */
 goto L8c10f080;
L8c10f080: if((*sr&1u))goto L8c10f0e0; /* bt 0x8c10f0e0 */
 goto L8c10f082;
L8c10f082: goto L8c10f0fa; /* bra 0x8c10f0fa */
 goto L8c10f086;
L8c10f086: fr[4]=fr[15];goto L8c10f0f2; /* bra 0x8c10f0f2 */
 goto L8c10f08a;
L8c10f08a: r[1]=rd(m,0x8c10f148u,4); /* mov.l 0x8c10f148,r1 */
 goto L8c10f08c;
L8c10f08c: r[4]=4294967294u; /* mov #-0x2,r4 */
 goto L8c10f08e;
L8c10f08e: r[2]=rd(m,0x8c10f144u,4); /* mov.l 0x8c10f144,r2 */
 goto L8c10f090;
L8c10f090: fr[2]=rd(m,r[1],4); /* fmov.s @r1,fr2 */
 goto L8c10f092;
L8c10f092: fr[3]=rd(m,r[2],4); /* fmov.s @r2,fr3 */
 goto L8c10f094;
L8c10f094: status=sa_sh4_add_sub(&fr[2],&fr[15],&f->fpscr,&fr[2],0);if(status)return status; /* fadd fr15,fr2 */
 goto L8c10f096;
L8c10f096: r[3]=rd(m,0x8c10f14cu,4); /* mov.l 0x8c10f14c,r3 */
 goto L8c10f098;
L8c10f098: status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status; /* fadd fr3,fr14 */
 goto L8c10f09a;
L8c10f09a: fr[12]=fr[2]; /* fmov fr2,fr12 */
 goto L8c10f09c;
L8c10f09c: target=r[3];*pr=0x8c10f0a0u;fr[4]=fr[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10f0a0;
L8c10f0a0: status=sa_sh4_add_sub(&fr[13],&fr[0],&f->fpscr,&fr[13],0);if(status)return status; /* fadd fr0,fr13 */
 goto L8c10f0a2;
L8c10f0a2: status=sa_sh4_divide(&fr[12],&fr[13],&f->fpscr,&fr[12]);if(status)return status; /* fdiv fr13,fr12 */
 goto L8c10f0a4;
L8c10f0a4: fr[4]=fr[12];goto L8c10f0f2; /* bra 0x8c10f0f2 */
 goto L8c10f0a8;
L8c10f0a8: r[1]=rd(m,0x8c10f154u,4); /* mov.l 0x8c10f154,r1 */
 goto L8c10f0aa;
L8c10f0aa: r[4]=4294967295u; /* mov #-0x1,r4 */
 goto L8c10f0ac;
L8c10f0ac: r[2]=rd(m,0x8c10f150u,4); /* mov.l 0x8c10f150,r2 */
 goto L8c10f0ae;
L8c10f0ae: fr[2]=rd(m,r[1],4); /* fmov.s @r1,fr2 */
 goto L8c10f0b0;
L8c10f0b0: fr[3]=rd(m,r[2],4); /* fmov.s @r2,fr3 */
 goto L8c10f0b2;
L8c10f0b2: status=sa_sh4_add_sub(&fr[2],&fr[15],&f->fpscr,&fr[2],0);if(status)return status; /* fadd fr15,fr2 */
 goto L8c10f0b4;
L8c10f0b4: r[3]=rd(m,0x8c10f14cu,4); /* mov.l 0x8c10f14c,r3 */
 goto L8c10f0b6;
L8c10f0b6: status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status; /* fadd fr3,fr14 */
 goto L8c10f0b8;
L8c10f0b8: fr[12]=fr[2]; /* fmov fr2,fr12 */
 goto L8c10f0ba;
L8c10f0ba: target=r[3];*pr=0x8c10f0beu;fr[4]=fr[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10f0be;
L8c10f0be: status=sa_sh4_add_sub(&fr[13],&fr[0],&f->fpscr,&fr[13],0);if(status)return status; /* fadd fr0,fr13 */
 goto L8c10f0c0;
L8c10f0c0: status=sa_sh4_divide(&fr[12],&fr[13],&f->fpscr,&fr[12]);if(status)return status; /* fdiv fr13,fr12 */
 goto L8c10f0c2;
L8c10f0c2: fr[4]=fr[12];goto L8c10f0f2; /* bra 0x8c10f0f2 */
 goto L8c10f0c6;
L8c10f0c6: r[1]=rd(m,0x8c10f15cu,4); /* mov.l 0x8c10f15c,r1 */
 goto L8c10f0c8;
L8c10f0c8: r[3]=rd(m,0x8c10f160u,4); /* mov.l 0x8c10f160,r3 */
 goto L8c10f0ca;
L8c10f0ca: r[2]=rd(m,0x8c10f158u,4); /* mov.l 0x8c10f158,r2 */
 goto L8c10f0cc;
L8c10f0cc: fr[2]=rd(m,r[1],4); /* fmov.s @r1,fr2 */
 goto L8c10f0ce;
L8c10f0ce: fr[0]=rd(m,r[3],4); /* fmov.s @r3,fr0 */
 goto L8c10f0d0;
L8c10f0d0: fr[1]=fr[15]; /* fmov fr15,fr1 */
 goto L8c10f0d2;
L8c10f0d2: status=sa_sh4_add_sub(&fr[1],&fr[2],&f->fpscr,&fr[1],0);if(status)return status; /* fadd fr2,fr1 */
 goto L8c10f0d4;
L8c10f0d4: fr[3]=rd(m,r[2],4); /* fmov.s @r2,fr3 */
 goto L8c10f0d6;
L8c10f0d6: status=sa_sh4_fmac(&fr[0],&fr[15],&fr[13],&f->fpscr,&fr[13]);if(status)return status; /* fmac fr0,fr15,fr13 */
 goto L8c10f0d8;
L8c10f0d8: status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status; /* fadd fr3,fr14 */
 goto L8c10f0da;
L8c10f0da: fr[4]=fr[1]; /* fmov fr1,fr4 */
 goto L8c10f0dc;
L8c10f0dc: status=sa_sh4_divide(&fr[4],&fr[13],&f->fpscr,&fr[4]);if(status)return status;goto L8c10f0f2; /* bra 0x8c10f0f2 */
 goto L8c10f0e0;
L8c10f0e0: r[1]=rd(m,0x8c10f134u,4); /* mov.l 0x8c10f134,r1 */
 goto L8c10f0e2;
L8c10f0e2: status=sa_sh4_add_sub(&fr[13],&fr[15],&f->fpscr,&fr[13],0);if(status)return status; /* fadd fr15,fr13 */
 goto L8c10f0e4;
L8c10f0e4: r[2]=rd(m,0x8c10f12cu,4); /* mov.l 0x8c10f12c,r2 */
 goto L8c10f0e6;
L8c10f0e6: fr[2]=rd(m,r[1],4); /* fmov.s @r1,fr2 */
 goto L8c10f0e8;
L8c10f0e8: fr[3]=rd(m,r[2],4); /* fmov.s @r2,fr3 */
 goto L8c10f0ea;
L8c10f0ea: status=sa_sh4_add_sub(&fr[2],&fr[15],&f->fpscr,&fr[2],0);if(status)return status; /* fadd fr15,fr2 */
 goto L8c10f0ec;
L8c10f0ec: status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],0);if(status)return status; /* fadd fr3,fr14 */
 goto L8c10f0ee;
L8c10f0ee: fr[4]=fr[2]; /* fmov fr2,fr4 */
 goto L8c10f0f0;
L8c10f0f0: status=sa_sh4_divide(&fr[4],&fr[13],&f->fpscr,&fr[4]);if(status)return status; /* fdiv fr13,fr4 */
 goto L8c10f0f2;
L8c10f0f2: target=r[14];*pr=0x8c10f0f6u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c10f0f6;
L8c10f0f6: fr[5]=fr[14]; /* fmov fr14,fr5 */
 goto L8c10f0f8;
L8c10f0f8: status=sa_sh4_add_sub(&fr[5],&fr[0],&f->fpscr,&fr[5],0);if(status)return status; /* fadd fr0,fr5 */
 goto L8c10f0fa;
L8c10f0fa: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c10f0fc;
L8c10f0fc: if((*sr&1u))goto L8c10f100; /* bt 0x8c10f100 */
 goto L8c10f0fe;
L8c10f0fe: fr[5]^=0x80000000u; /* fneg fr5 */
 goto L8c10f100;
L8c10f100: fr[0]=fr[5]; /* fmov fr5,fr0 */
 goto L8c10f102;
L8c10f102: r[15]+=12u; /* add #0xc,r15 */
 goto L8c10f104;
L8c10f104: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10f106;
L8c10f106: fr[12]=rd(m,r[15],4);r[15]+=4; /* fmov.s @r15+,fr12 */
 goto L8c10f108;
L8c10f108: fr[13]=rd(m,r[15],4);r[15]+=4; /* fmov.s @r15+,fr13 */
 goto L8c10f10a;
L8c10f10a: fr[14]=rd(m,r[15],4);r[15]+=4; /* fmov.s @r15+,fr14 */
 goto L8c10f10c;
L8c10f10c: fr[15]=rd(m,r[15],4);r[15]+=4; /* fmov.s @r15+,fr15 */
 goto L8c10f10e;
L8c10f10e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c10f110;
L8c10f110: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
