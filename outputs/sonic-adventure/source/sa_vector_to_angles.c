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
/* Static C recovery of retail Tikal helper 8c056860..8c056926.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_vector_to_angles(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c056860;
L8c056860: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c056862;
L8c056862: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c056864;
L8c056864: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c056866;
L8c056866: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c056868;
L8c056868: r[15]-=4;wr(m,r[15],fr[15],4); /* fmov.s fr15,@-r15 */
 goto L8c05686a;
L8c05686a: r[15]-=4;wr(m,r[15],fr[14],4); /* fmov.s fr14,@-r15 */
 goto L8c05686c;
L8c05686c: r[15]-=4;wr(m,r[15],fr[13],4); /* fmov.s fr13,@-r15 */
 goto L8c05686e;
L8c05686e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c056870;
L8c056870: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c056872;
L8c056872: r[0]=8u; /* mov #0x8,r0 */
 goto L8c056874;
L8c056874: r[13]=r[4]; /* mov r4,r13 */
 goto L8c056876;
L8c056876: r[3]=rd(m,0x8c0569a0u,4); /* mov.l 0x8c0569a0,r3 */
 goto L8c056878;
L8c056878: r[14]=r[5]; /* mov r5,r14 */
 goto L8c05687a;
L8c05687a: wr(m,r[15],r[6],4); /* mov.l r6,@r15 */
 goto L8c05687c;
L8c05687c: fr[15]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr15 */
 goto L8c05687e;
L8c05687e: fr[14]=rd(m,r[13],4); /* fmov.s @r13,fr14 */
 goto L8c056880;
L8c056880: fr[3]=fr[15]; /* fmov fr15,fr3 */
 goto L8c056882;
L8c056882: status=sa_sh4_multiply(&fr[3],&fr[15],&f->fpscr,&fr[3]);if(status)return status; /* fmul fr15,fr3 */
 goto L8c056884;
L8c056884: fr[0]=fr[14]; /* fmov fr14,fr0 */
 goto L8c056886;
L8c056886: status=sa_sh4_fmac(&fr[0],&fr[14],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr14,fr3 */
 goto L8c056888;
L8c056888: target=r[3];*pr=0x8c05688cu;fr[4]=fr[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c05688c;
L8c05688c: r[0]=0x8c0569a8u; /* mova 0x8c0569a8,r0 */
 goto L8c05688e;
L8c05688e: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c056890;
L8c056890: fr[15]=rd(m,r[0],4); /* fmov.s @r0,fr15 */
 goto L8c056892;
L8c056892: r[0]=8u; /* mov #0x8,r0 */
 goto L8c056894;
L8c056894: r[3]+=4u; /* add #0x4,r3 */
 goto L8c056896;
L8c056896: r[12]=rd(m,0x8c0569a4u,4); /* mov.l 0x8c0569a4,r12 */
 goto L8c056898;
L8c056898: fr[14]=fr[0]; /* fmov fr0,fr14 */
 goto L8c05689a;
L8c05689a: r[8]=r[3]; /* mov r3,r8 */
 goto L8c05689c;
L8c05689c: wr(m,r[15]+0x8u,r[3],4); /* mov.l r3,@(0x8,r15) */
 goto L8c05689e;
L8c05689e: fr[5]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr5 */
 goto L8c0568a0;
L8c0568a0: fr[5]^=0x80000000u; /* fneg fr5 */
 goto L8c0568a2;
L8c0568a2: target=r[12];*pr=0x8c0568a6u;fr[4]=rd(m,r[13],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c0568a6;
L8c0568a6: status=sa_sh4_multiply(&fr[0],&fr[15],&f->fpscr,&fr[0]);if(status)return status; /* fmul fr15,fr0 */
 goto L8c0568a8;
L8c0568a8: r[0]=4u; /* mov #0x4,r0 */
 goto L8c0568aa;
L8c0568aa: fr[5]=fr[14]; /* fmov fr14,fr5 */
 goto L8c0568ac;
L8c0568ac: status=sa_sh4_truncate_i32(fr[0],&f->fpscr,fpul);if(status)return status; /* ftrc fr0,FPUL */
 goto L8c0568ae;
L8c0568ae: r[3]=*fpul; /* sts FPUL,r3 */
 goto L8c0568b0;
L8c0568b0: wr(m,r[8],r[3],4); /* mov.l r3,@r8 */
 goto L8c0568b2;
L8c0568b2: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c0568b4;
L8c0568b4: r[2]+=8u; /* add #0x8,r2 */
 goto L8c0568b6;
L8c0568b6: wr(m,r[15]+0x4u,r[2],4); /* mov.l r2,@(0x4,r15) */
 goto L8c0568b8;
L8c0568b8: r[8]=r[2]; /* mov r2,r8 */
 goto L8c0568ba;
L8c0568ba: fr[4]=rd(m,r[0]+r[13],4); /* fmov.s @(r0,r13),fr4 */
 goto L8c0568bc;
L8c0568bc: target=r[12];*pr=0x8c0568c0u;fr[4]^=0x80000000u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c0568c0;
L8c0568c0: status=sa_sh4_multiply(&fr[0],&fr[15],&f->fpscr,&fr[0]);if(status)return status; /* fmul fr15,fr0 */
 goto L8c0568c2;
L8c0568c2: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c0568c4;
L8c0568c4: status=sa_sh4_truncate_i32(fr[0],&f->fpscr,fpul);if(status)return status; /* ftrc fr0,FPUL */
 goto L8c0568c6;
L8c0568c6: r[3]=*fpul; /* sts FPUL,r3 */
 goto L8c0568c8;
L8c0568c8: take=(*sr&1u);wr(m,r[8],r[3],4);if(take)goto L8c056912;goto L8c0568cc; /* bt/s 0x8c056912 */
 goto L8c0568cc;
L8c0568cc: r[0]=8u; /* mov #0x8,r0 */
 goto L8c0568ce;
L8c0568ce: fr[14]=rd(m,r[14],4); /* fmov.s @r14,fr14 */
 goto L8c0568d0;
L8c0568d0: fr[13]=rd(m,r[0]+r[14],4); /* fmov.s @(r0,r14),fr13 */
 goto L8c0568d2;
L8c0568d2: fr[0]=fr[14]; /* fmov fr14,fr0 */
 goto L8c0568d4;
L8c0568d4: fr[3]=fr[13]; /* fmov fr13,fr3 */
 goto L8c0568d6;
L8c0568d6: status=sa_sh4_multiply(&fr[3],&fr[13],&f->fpscr,&fr[3]);if(status)return status; /* fmul fr13,fr3 */
 goto L8c0568d8;
L8c0568d8: r[3]=rd(m,0x8c0569a0u,4); /* mov.l 0x8c0569a0,r3 */
 goto L8c0568da;
L8c0568da: status=sa_sh4_fmac(&fr[0],&fr[14],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr14,fr3 */
 goto L8c0568dc;
L8c0568dc: target=r[3];*pr=0x8c0568e0u;fr[4]=fr[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c0568e0;
L8c0568e0: r[0]=8u; /* mov #0x8,r0 */
 goto L8c0568e2;
L8c0568e2: r[8]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r8 */
 goto L8c0568e4;
L8c0568e4: fr[5]=rd(m,r[0]+r[14],4); /* fmov.s @(r0,r14),fr5 */
 goto L8c0568e6;
L8c0568e6: fr[14]=fr[0]; /* fmov fr0,fr14 */
 goto L8c0568e8;
L8c0568e8: fr[5]^=0x80000000u; /* fneg fr5 */
 goto L8c0568ea;
L8c0568ea: target=r[12];*pr=0x8c0568eeu;fr[4]=rd(m,r[14],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c0568ee;
L8c0568ee: status=sa_sh4_multiply(&fr[0],&fr[15],&f->fpscr,&fr[0]);if(status)return status; /* fmul fr15,fr0 */
 goto L8c0568f0;
L8c0568f0: r[1]=rd(m,r[8],4); /* mov.l @r8,r1 */
 goto L8c0568f2;
L8c0568f2: r[0]=4u; /* mov #0x4,r0 */
 goto L8c0568f4;
L8c0568f4: fr[5]=fr[14]; /* fmov fr14,fr5 */
 goto L8c0568f6;
L8c0568f6: status=sa_sh4_truncate_i32(fr[0],&f->fpscr,fpul);if(status)return status; /* ftrc fr0,FPUL */
 goto L8c0568f8;
L8c0568f8: r[3]=*fpul; /* sts FPUL,r3 */
 goto L8c0568fa;
L8c0568fa: r[1]-=r[3]; /* sub r3,r1 */
 goto L8c0568fc;
L8c0568fc: wr(m,r[8],r[1],4); /* mov.l r1,@r8 */
 goto L8c0568fe;
L8c0568fe: fr[4]=rd(m,r[0]+r[14],4); /* fmov.s @(r0,r14),fr4 */
 goto L8c056900;
L8c056900: r[8]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r8 */
 goto L8c056902;
L8c056902: target=r[12];*pr=0x8c056906u;fr[4]^=0x80000000u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c056906;
L8c056906: status=sa_sh4_multiply(&fr[0],&fr[15],&f->fpscr,&fr[0]);if(status)return status; /* fmul fr15,fr0 */
 goto L8c056908;
L8c056908: r[2]=rd(m,r[8],4); /* mov.l @r8,r2 */
 goto L8c05690a;
L8c05690a: status=sa_sh4_truncate_i32(fr[0],&f->fpscr,fpul);if(status)return status; /* ftrc fr0,FPUL */
 goto L8c05690c;
L8c05690c: r[3]=*fpul; /* sts FPUL,r3 */
 goto L8c05690e;
L8c05690e: r[2]-=r[3]; /* sub r3,r2 */
 goto L8c056910;
L8c056910: wr(m,r[8],r[2],4); /* mov.l r2,@r8 */
 goto L8c056912;
L8c056912: r[15]+=12u; /* add #0xc,r15 */
 goto L8c056914;
L8c056914: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c056916;
L8c056916: fr[13]=rd(m,r[15],4);r[15]+=4; /* fmov.s @r15+,fr13 */
 goto L8c056918;
L8c056918: fr[14]=rd(m,r[15],4);r[15]+=4; /* fmov.s @r15+,fr14 */
 goto L8c05691a;
L8c05691a: fr[15]=rd(m,r[15],4);r[15]+=4; /* fmov.s @r15+,fr15 */
 goto L8c05691c;
L8c05691c: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c05691e;
L8c05691e: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c056920;
L8c056920: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c056922;
L8c056922: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
