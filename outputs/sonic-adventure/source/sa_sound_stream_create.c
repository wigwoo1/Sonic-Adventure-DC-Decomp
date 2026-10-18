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
/* Retail sound stream create with explicit SH4 register/RAM ABI.
 * File handle services remain external. Verification: stream_allocation_verification.json. */
SA_STAGE_EXPORT int sa_sound_stream_create(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6673b4;
L8c6673b4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6673b6;
L8c6673b6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6673b8;
L8c6673b8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6673ba;
L8c6673ba: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c6673bc;
L8c6673bc: r[13]=sx(m,0x8c667468u,2); /* mov.w 0x8c667468,r13 */
 goto L8c6673be;
L8c6673be: wr(m,r[15]+0xcu,r[4],4); /* mov.l r4,@(0xc,r15) */
 goto L8c6673c0;
L8c6673c0: r[4]=0u; /* mov #0x0,r4 */
 goto L8c6673c2;
L8c6673c2: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c6673c4;
L8c6673c4: r[5]=rd(m,0x8c667470u,4); /* mov.l 0x8c667470,r5 */
 goto L8c6673c6;
L8c6673c6: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c6673c8;
L8c6673c8: r[6]=8u; /* mov #0x8,r6 */
 goto L8c6673ca;
L8c6673ca: wr(m,r[15]+0x8u,r[7],4); /* mov.l r7,@(0x8,r15) */
 goto L8c6673cc;
L8c6673cc: *macl=r[13]*r[4]; /* mul.l r13,r4 */
 goto L8c6673ce;
L8c6673ce: r[3]=*macl; /* sts MACL,r3 */
 goto L8c6673d0;
L8c6673d0: r[3]+=r[5]; /* add r5,r3 */
 goto L8c6673d2;
L8c6673d2: r[2]=sx(m,r[3],2); /* mov.w @r3,r2 */
 goto L8c6673d4;
L8c6673d4: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c6673d6;
L8c6673d6: if((*sr&1u))goto L8c6673de; /* bt 0x8c6673de */
 goto L8c6673d8;
L8c6673d8: r[4]+=1u; /* add #0x1,r4 */
 goto L8c6673da;
L8c6673da: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[6]); /* cmp/ge r6,r4 */
 goto L8c6673dc;
L8c6673dc: if(!(*sr&1u))goto L8c6673cc; /* bf 0x8c6673cc */
 goto L8c6673de;
L8c6673de: r[0]=r[4]; /* mov r4,r0 */
 goto L8c6673e0;
L8c6673e0: *sr=(*sr&~1u)|(r[0]==0x00000008u); /* cmp/eq #0x8,r0 */
 goto L8c6673e2;
L8c6673e2: if((*sr&1u))goto L8c667408; /* bt 0x8c667408 */
 goto L8c6673e4;
L8c6673e4: *macl=r[13]*r[4]; /* mul.l r13,r4 */
 goto L8c6673e6;
L8c6673e6: r[3]=rd(m,0x8c667474u,4); /* mov.l 0x8c667474,r3 */
 goto L8c6673e8;
L8c6673e8: r[6]=r[13]; /* mov r13,r6 */
 goto L8c6673ea;
L8c6673ea: r[14]=*macl; /* sts MACL,r14 */
 goto L8c6673ec;
L8c6673ec: r[14]+=r[5]; /* add r5,r14 */
 goto L8c6673ee;
L8c6673ee: r[5]=0u; /* mov #0x0,r5 */
 goto L8c6673f0;
L8c6673f0: target=r[3];*pr=0x8c6673f4u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6673f4;
L8c6673f4: r[3]=rd(m,0x8c667478u,4); /* mov.l 0x8c667478,r3 */
 goto L8c6673f6;
L8c6673f6: r[2]=1u; /* mov #0x1,r2 */
 goto L8c6673f8;
L8c6673f8: target=r[3];*pr=0x8c6673fcu;wr(m,r[14],r[2],2);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6673fc;
L8c6673fc: r[2]=r[0]; /* mov r0,r2 */
 goto L8c6673fe;
L8c6673fe: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c667400;
L8c667400: take=!(*sr&1u);wr(m,r[14]+0x8u,r[0],4);if(take)goto L8c66740c;goto L8c667404; /* bf/s 0x8c66740c */
 goto L8c667404;
L8c667404: *pr=0x8c667408u;r[4]=r[14];status=call(ctx,0x8c667440u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c667440 */
 goto L8c667408;
L8c667408: r[0]=0u;goto L8c667436; /* bra 0x8c667436 */
 goto L8c66740c;
L8c66740c: r[2]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r2 */
 goto L8c66740e;
L8c66740e: r[0]=64u; /* mov #0x40,r0 */
 goto L8c667410;
L8c667410: wr(m,r[14]+0x38u,r[2],4); /* mov.l r2,@(0x38,r14) */
 goto L8c667412;
L8c667412: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c667414;
L8c667414: wr(m,r[14]+0x3cu,r[3],4); /* mov.l r3,@(0x3c,r14) */
 goto L8c667416;
L8c667416: r[2]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r2 */
 goto L8c667418;
L8c667418: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c66741a;
L8c66741a: r[0]=68u; /* mov #0x44,r0 */
 goto L8c66741c;
L8c66741c: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c66741e;
L8c66741e: r[2]=rd(m,0x8c66747cu,4); /* mov.l 0x8c66747c,r2 */
 goto L8c667420;
L8c667420: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c667422;
L8c667422: r[0]+=68u; /* add #0x44,r0 */
 goto L8c667424;
L8c667424: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c667426;
L8c667426: r[0]+=4u; /* add #0x4,r0 */
 goto L8c667428;
L8c667428: r[3]=rd(m,0x8c667480u,4); /* mov.l 0x8c667480,r3 */
 goto L8c66742a;
L8c66742a: wr(m,r[14]+r[0],r[14],4); /* mov.l r14,@(r0,r14) */
 goto L8c66742c;
L8c66742c: r[0]+=4u; /* add #0x4,r0 */
 goto L8c66742e;
L8c66742e: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c667430;
L8c667430: r[0]+=4u; /* add #0x4,r0 */
 goto L8c667432;
L8c667432: wr(m,r[14]+r[0],r[14],4); /* mov.l r14,@(r0,r14) */
 goto L8c667434;
L8c667434: r[0]=r[14]; /* mov r14,r0 */
 goto L8c667436;
L8c667436: r[15]+=16u; /* add #0x10,r15 */
 goto L8c667438;
L8c667438: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66743a;
L8c66743a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c66743c;
L8c66743c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
