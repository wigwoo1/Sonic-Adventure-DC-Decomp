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
/* Static C recovery of retail Tikal helper 8c64753c..8c6475ec.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
static int sa_input_device_request_core(uint32_t entry,uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 switch(entry){case 0:goto L8c64753c;case 0x8c64756eu:goto L8c64756e;case 0x8c6475bcu:goto L8c6475bc;default:return -4;}
L8c64753c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c64753e;
L8c64753e: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c647540;
L8c647540: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c647542;
L8c647542: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c647544;
L8c647544: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c647546;
L8c647546: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c647548;
L8c647548: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c64754a;
L8c64754a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c64754c;
L8c64754c: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c64754e;
L8c64754e: r[12]=sx(m,0x8c6475ecu,2); /* mov.w 0x8c6475ec,r12 */
 goto L8c647550;
L8c647550: r[13]=r[4]; /* mov r4,r13 */
 goto L8c647552;
L8c647552: r[3]=rd(m,0x8c6475f4u,4); /* mov.l 0x8c6475f4,r3 */
 goto L8c647554;
L8c647554: *macl=r[12]*r[13]; /* mul.l r12,r13 */
 goto L8c647556;
L8c647556: r[2]=rd(m,0x8c6475f8u,4); /* mov.l 0x8c6475f8,r2 */
 goto L8c647558;
L8c647558: r[10]=rd(m,0x8c647600u,4); /* mov.l 0x8c647600,r10 */
 goto L8c64755a;
L8c64755a: r[8]=rd(m,0x8c6475fcu,4); /* mov.l 0x8c6475fc,r8 */
 goto L8c64755c;
L8c64755c: r[12]=*macl; /* sts MACL,r12 */
 goto L8c64755e;
L8c64755e: r[11]=sx(m,0x8c6475eeu,2); /* mov.w 0x8c6475ee,r11 */
 goto L8c647560;
L8c647560: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c647562;
L8c647562: r[9]=rd(m,r[3],4); /* mov.l @r3,r9 */
 goto L8c647564;
L8c647564: wr(m,r[15]+0x4u,r[12],4); /* mov.l r12,@(0x4,r15) */
 goto L8c647566;
L8c647566: r[12]+=r[2]; /* add r2,r12 */
 goto L8c647568;
L8c647568: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c64756a;
L8c64756a: target=r[10];*pr=0x8c64756eu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r10 */
 goto L8c64756e;
L8c64756e: r[14]=r[0]; /* mov r0,r14 */
 goto L8c647570;
L8c647570: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c647572;
L8c647572: if((*sr&1u))goto L8c647592; /* bt 0x8c647592 */
 goto L8c647574;
L8c647574: r[3]=rd(m,r[8],4); /* mov.l @r8,r3 */
 goto L8c647576;
L8c647576: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c647578;
L8c647578: if((*sr&1u))goto L8c64757e; /* bt 0x8c64757e */
 goto L8c64757a;
L8c64757a: *sr=(*sr&~1u)|(r[11]==r[14]); /* cmp/eq r11,r14 */
 goto L8c64757c;
L8c64757c: if((*sr&1u))goto L8c6475d0; /* bt 0x8c6475d0 */
 goto L8c64757e;
L8c64757e: r[3]=rd(m,r[12]+0x34u,4); /* mov.l @(0x34,r12),r3 */
 goto L8c647580;
L8c647580: *sr=(*sr&~1u)|!(r[9]&r[9]); /* tst r9,r9 */
 goto L8c647582;
L8c647582: r[3]+=1u; /* add #0x1,r3 */
 goto L8c647584;
L8c647584: wr(m,r[12]+0x34u,r[3],4); /* mov.l r3,@(0x34,r12) */
 goto L8c647586;
L8c647586: take=!(*sr&1u);r[9]+=4294967295u;if(take)goto L8c647568;goto L8c64758a; /* bf/s 0x8c647568 */
 goto L8c64758a;
L8c64758a: r[1]=rd(m,0x8c6475f4u,4); /* mov.l 0x8c6475f4,r1 */
 goto L8c64758c;
L8c64758c: r[0]=rd(m,r[1],4); /* mov.l @r1,r0 */
 goto L8c64758e;
L8c64758e: *sr=(*sr&~1u)|(r[0]==0xffffffffu); /* cmp/eq #-0x1,r0 */
 goto L8c647590;
L8c647590: if((*sr&1u))goto L8c647568; /* bt 0x8c647568 */
 goto L8c647592;
L8c647592: r[12]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r12 */
 goto L8c647594;
L8c647594: r[2]=r[13]; /* mov r13,r2 */
 goto L8c647596;
L8c647596: r[3]=rd(m,0x8c6475f8u,4); /* mov.l 0x8c6475f8,r3 */
 goto L8c647598;
L8c647598: r[2]<<=2; /* shll2 r2 */
 goto L8c64759a;
L8c64759a: r[2]<<=2; /* shll2 r2 */
 goto L8c64759c;
L8c64759c: r[1]=rd(m,0x8c647604u,4); /* mov.l 0x8c647604,r1 */
 goto L8c64759e;
L8c64759e: r[12]+=r[3]; /* add r3,r12 */
 goto L8c6475a0;
L8c6475a0: r[3]=rd(m,0x8c647608u,4); /* mov.l 0x8c647608,r3 */
 goto L8c6475a2;
L8c6475a2: r[2]<<=2; /* shll2 r2 */
 goto L8c6475a4;
L8c6475a4: *sr=(*sr&~1u)|(r[2]>>31);r[2]<<=1; /* shll r2 */
 goto L8c6475a6;
L8c6475a6: r[2]+=r[1]; /* add r1,r2 */
 goto L8c6475a8;
L8c6475a8: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c6475aa;
L8c6475aa: r[6]=rd(m,r[12]+0x30u,4); /* mov.l @(0x30,r12),r6 */
 goto L8c6475ac;
L8c6475ac: r[7]=rd(m,r[12]+0x34u,4); /* mov.l @(0x34,r12),r7 */
 goto L8c6475ae;
L8c6475ae: r[6]+=1u; /* add #0x1,r6 */
 goto L8c6475b0;
L8c6475b0: wr(m,r[12]+0x30u,r[6],4); /* mov.l r6,@(0x30,r12) */
 goto L8c6475b2;
L8c6475b2: r[6]+=4294967295u; /* add #-0x1,r6 */
 goto L8c6475b4;
L8c6475b4: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c6475b6;
L8c6475b6: r[5]=rd(m,r[5]+0x8u,4); /* mov.l @(0x8,r5),r5 */
 goto L8c6475b8;
L8c6475b8: target=r[3];*pr=0x8c6475bcu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6475bc;
L8c6475bc: r[0]=r[14]; /* mov r14,r0 */
 goto L8c6475be;
L8c6475be: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c6475c0;
L8c6475c0: if((*sr&1u))goto L8c6475cc; /* bt 0x8c6475cc */
 goto L8c6475c2;
L8c6475c2: r[1]=sx(m,0x8c6475f0u,2); /* mov.w 0x8c6475f0,r1 */
 goto L8c6475c4;
L8c6475c4: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c6475c6;
L8c6475c6: if((*sr&1u))goto L8c6475d0; /* bt 0x8c6475d0 */
 goto L8c6475c8;
L8c6475c8: goto L8c6475d6; /* bra 0x8c6475d6 */
 goto L8c6475cc;
L8c6475cc: r[0]=0u;goto L8c6475d8; /* bra 0x8c6475d8 */
 goto L8c6475d0;
L8c6475d0: r[0]=sx(m,0x8c6475f0u,2); /* mov.w 0x8c6475f0,r0 */
 goto L8c6475d2;
L8c6475d2: goto L8c6475d8; /* bra 0x8c6475d8 */
 goto L8c6475d6;
L8c6475d6: r[0]=rd(m,0x8c64760cu,4); /* mov.l 0x8c64760c,r0 */
 goto L8c6475d8;
L8c6475d8: r[15]+=8u; /* add #0x8,r15 */
 goto L8c6475da;
L8c6475da: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6475dc;
L8c6475dc: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c6475de;
L8c6475de: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c6475e0;
L8c6475e0: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c6475e2;
L8c6475e2: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c6475e4;
L8c6475e4: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6475e6;
L8c6475e6: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6475e8;
L8c6475e8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_input_device_request(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_device_request_core(0,m,f,sr,r,pr,fpul,macl,ctx,call); }

SA_STAGE_EXPORT int sa_input_device_request_resume(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){return sa_input_device_request_core(*pr,m,f,sr,r,pr,fpul,macl,ctx,call); }
