#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
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
/* Static C recovery of retail Aligned file read helper 8c601084..8c60116e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_read_aligned(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c601084;
L8c601084: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c601086;
L8c601086: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c601088;
L8c601088: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c60108a;
L8c60108a: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c60108c;
L8c60108c: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c60108e;
L8c60108e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c601090;
L8c601090: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c601092;
L8c601092: r[14]=r[4]; /* mov r4,r14 */
 goto L8c601094;
L8c601094: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c601096;
L8c601096: take=(*sr&1u);r[13]=r[5];if(take)goto L8c6010a2;goto L8c60109a; /* bt/s 0x8c6010a2 */
 goto L8c60109a;
L8c60109a: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c60109c;
L8c60109c: r[3]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r3 */
 goto L8c60109e;
L8c60109e: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c6010a0;
L8c6010a0: if(!(*sr&1u))goto L8c6010a6; /* bf 0x8c6010a6 */
 goto L8c6010a2;
L8c6010a2: r[0]=4294967286u;goto L8c60115e; /* bra 0x8c60115e */
 goto L8c6010a6;
L8c6010a6: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c6010a8;
L8c6010a8: r[2]=rd(m,r[3]+0x1cu,4); /* mov.l @(0x1c,r3),r2 */
 goto L8c6010aa;
L8c6010aa: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c6010ac;
L8c6010ac: if(!(*sr&1u))goto L8c60115c; /* bf 0x8c60115c */
 goto L8c6010ae;
L8c6010ae: r[3]=31u; /* mov #0x1f,r3 */
 goto L8c6010b0;
L8c6010b0: *sr=(*sr&~1u)|!(r[6]&r[3]); /* tst r6,r3 */
 goto L8c6010b2;
L8c6010b2: if((*sr&1u))goto L8c6010b8; /* bt 0x8c6010b8 */
 goto L8c6010b4;
L8c6010b4: r[0]=4294967282u;goto L8c60115e; /* bra 0x8c60115e */
 goto L8c6010b8;
L8c6010b8: r[12]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r12 */
 goto L8c6010ba;
L8c6010ba: r[10]=0u; /* mov #0x0,r10 */
 goto L8c6010bc;
L8c6010bc: r[4]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r4 */
 goto L8c6010be;
L8c6010be: r[3]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r3 */
 goto L8c6010c0;
L8c6010c0: r[12]+=r[4]; /* add r4,r12 */
 goto L8c6010c2;
L8c6010c2: r[4]+=r[3]; /* add r3,r4 */
 goto L8c6010c4;
L8c6010c4: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[4]); /* cmp/ge r4,r12 */
 goto L8c6010c6;
L8c6010c6: take=!(*sr&1u);r[11]=1u;if(take)goto L8c6010d2;goto L8c6010ca; /* bf/s 0x8c6010d2 */
 goto L8c6010ca;
L8c6010ca: r[0]=80u; /* mov #0x50,r0 */
 goto L8c6010cc;
L8c6010cc: wr(m,r[14]+0x18u,r[10],4); /* mov.l r10,@(0x18,r14) */
 goto L8c6010ce;
L8c6010ce: wr(m,r[14]+r[0],r[11],2);goto L8c60115c; /* bra 0x8c60115c */
 goto L8c6010d2;
L8c6010d2: r[2]=r[13]; /* mov r13,r2 */
 goto L8c6010d4;
L8c6010d4: r[2]+=r[12]; /* add r12,r2 */
 goto L8c6010d6;
L8c6010d6: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[4]); /* cmp/gt r4,r2 */
 goto L8c6010d8;
L8c6010d8: if(!(*sr&1u))goto L8c6010de; /* bf 0x8c6010de */
 goto L8c6010da;
L8c6010da: r[13]=r[4]; /* mov r4,r13 */
 goto L8c6010dc;
L8c6010dc: r[13]-=r[12]; /* sub r12,r13 */
 goto L8c6010de;
L8c6010de: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c6010e0;
L8c6010e0: if(!(*sr&1u))goto L8c6010ec; /* bf 0x8c6010ec */
 goto L8c6010e2;
L8c6010e2: r[0]=80u; /* mov #0x50,r0 */
 goto L8c6010e4;
L8c6010e4: wr(m,r[14]+0x18u,r[10],4); /* mov.l r10,@(0x18,r14) */
 goto L8c6010e6;
L8c6010e6: wr(m,r[14]+r[0],r[11],2);goto L8c601158; /* bra 0x8c601158 */
 goto L8c6010ec;
L8c6010ec: r[0]=*sr; /* stc SR,r0 */
 goto L8c6010ee;
L8c6010ee: r[3]=sx(m,0x8c60121au,2); /* mov.w 0x8c60121a,r3 */
 goto L8c6010f0;
L8c6010f0: r[0]>>=2; /* shlr2 r0 */
 goto L8c6010f2;
L8c6010f2: r[0]>>=2; /* shlr2 r0 */
 goto L8c6010f4;
L8c6010f4: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c6010f6;
L8c6010f6: wr(m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c6010f8;
L8c6010f8: r[0]=*sr; /* stc SR,r0 */
 goto L8c6010fa;
L8c6010fa: r[0]&=r[3]; /* and r3,r0 */
 goto L8c6010fc;
L8c6010fc: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c6010fe;
L8c6010fe: *sr=r[0]; /* ldc r0,SR */
 goto L8c601100;
L8c601100: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c601102;
L8c601102: r[0]=78u; /* mov #0x4e,r0 */
 goto L8c601104;
L8c601104: r[7]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r7 */
 goto L8c601106;
L8c601106: r[5]=r[13]; /* mov r13,r5 */
 goto L8c601108;
L8c601108: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c60110a;
L8c60110a: r[1]=rd(m,r[3]+0x4u,4); /* mov.l @(0x4,r3),r1 */
 goto L8c60110c;
L8c60110c: target=r[1];*pr=0x8c601110u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c601110;
L8c601110: r[12]=r[0]; /* mov r0,r12 */
 goto L8c601112;
L8c601112: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c601114;
L8c601114: if((*sr&1u))goto L8c60113e; /* bt 0x8c60113e */
 goto L8c601116;
L8c601116: r[2]=2u; /* mov #0x2,r2 */
 goto L8c601118;
L8c601118: wr(m,r[14]+0x3cu,r[12],4); /* mov.l r12,@(0x3c,r14) */
 goto L8c60111a;
L8c60111a: r[0]=80u; /* mov #0x50,r0 */
 goto L8c60111c;
L8c60111c: wr(m,r[14]+0x1cu,r[13],4); /* mov.l r13,@(0x1c,r14) */
 goto L8c60111e;
L8c60111e: wr(m,r[14]+r[0],r[2],2); /* mov.w r2,@(r0,r14) */
 goto L8c601120;
L8c601120: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c601122;
L8c601122: wr(m,r[14]+r[0],r[11],2); /* mov.w r11,@(r0,r14) */
 goto L8c601124;
L8c601124: r[0]=72u; /* mov #0x48,r0 */
 goto L8c601126;
L8c601126: wr(m,r[14]+r[0],r[11],2); /* mov.w r11,@(r0,r14) */
 goto L8c601128;
L8c601128: wr(m,r[14]+0x18u,r[10],4); /* mov.l r10,@(0x18,r14) */
 goto L8c60112a;
L8c60112a: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c60112c;
L8c60112c: wr(m,r[3]+0x1cu,r[14],4); /* mov.l r14,@(0x1c,r3) */
 goto L8c60112e;
L8c60112e: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c601130;
L8c601130: r[0]=64u; /* mov #0x40,r0 */
 goto L8c601132;
L8c601132: r[3]=rd(m,r[2]+0xcu,4); /* mov.l @(0xc,r2),r3 */
 goto L8c601134;
L8c601134: r[1]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r1 */
 goto L8c601136;
L8c601136: target=r[1];*pr=0x8c60113au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c60113a;
L8c60113a: goto L8c601142; /* bra 0x8c601142 */
 goto L8c60113e;
L8c60113e: r[0]=80u; /* mov #0x50,r0 */
 goto L8c601140;
L8c601140: wr(m,r[14]+r[0],r[11],2); /* mov.w r11,@(r0,r14) */
 goto L8c601142;
L8c601142: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c601144;
L8c601144: r[3]=*sr; /* stc SR,r3 */
 goto L8c601146;
L8c601146: r[2]=sx(m,0x8c60121au,2); /* mov.w 0x8c60121a,r2 */
 goto L8c601148;
L8c601148: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c60114a;
L8c60114a: r[0]<<=2; /* shll2 r0 */
 goto L8c60114c;
L8c60114c: r[3]&=r[2]; /* and r2,r3 */
 goto L8c60114e;
L8c60114e: r[0]<<=2; /* shll2 r0 */
 goto L8c601150;
L8c601150: r[0]|=r[3]; /* or r3,r0 */
 goto L8c601152;
L8c601152: *sr=r[0]; /* ldc r0,SR */
 goto L8c601154;
L8c601154: *sr=(*sr&~1u)|!(r[12]&r[12]); /* tst r12,r12 */
 goto L8c601156;
L8c601156: if((*sr&1u))goto L8c60115c; /* bt 0x8c60115c */
 goto L8c601158;
L8c601158: r[0]=r[13];goto L8c60115e; /* bra 0x8c60115e */
 goto L8c60115c;
L8c60115c: r[0]=0u; /* mov #0x0,r0 */
 goto L8c60115e;
L8c60115e: r[15]+=4u; /* add #0x4,r15 */
 goto L8c601160;
L8c601160: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c601162;
L8c601162: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c601164;
L8c601164: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c601166;
L8c601166: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c601168;
L8c601168: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c60116a;
L8c60116a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
