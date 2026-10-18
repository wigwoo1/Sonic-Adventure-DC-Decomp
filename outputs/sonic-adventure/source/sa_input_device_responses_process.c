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
/* Static C recovery of retail Tikal helper 8c65f202..8c65f284.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_responses_process(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65f202;
L8c65f202: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65f204;
L8c65f204: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65f206;
L8c65f206: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65f208;
L8c65f208: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c65f20a;
L8c65f20a: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c65f20c;
L8c65f20c: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c65f20e;
L8c65f20e: r[9]=6u; /* mov #0x6,r9 */
 goto L8c65f210;
L8c65f210: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c65f212;
L8c65f212: r[8]=0u; /* mov #0x0,r8 */
 goto L8c65f214;
L8c65f214: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65f216;
L8c65f216: r[10]=rd(m,0x8c65f290u,4); /* mov.l 0x8c65f290,r10 */
 goto L8c65f218;
L8c65f218: r[14]=r[8];goto L8c65f26a; /* bra 0x8c65f26a */
 goto L8c65f21c;
L8c65f21c: r[0]=rd(m,0x8c65f298u,4); /* mov.l 0x8c65f298,r0 */
 goto L8c65f21e;
L8c65f21e: r[3]=r[14]; /* mov r14,r3 */
 goto L8c65f220;
L8c65f220: r[3]<<=2; /* shll2 r3 */
 goto L8c65f222;
L8c65f222: r[1]=sx(m,0x8c65f284u,2); /* mov.w 0x8c65f284,r1 */
 goto L8c65f224;
L8c65f224: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c65f226;
L8c65f226: r[0]=rd(m,r[10],4); /* mov.l @r10,r0 */
 goto L8c65f228;
L8c65f228: r[1]+=r[0]; /* add r0,r1 */
 goto L8c65f22a;
L8c65f22a: r[2]+=r[1]; /* add r1,r2 */
 goto L8c65f22c;
L8c65f22c: r[3]=sx(m,r[2],1); /* mov.b @r2,r3 */
 goto L8c65f22e;
L8c65f22e: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65f230;
L8c65f230: if((*sr&1u))goto L8c65f268; /* bt 0x8c65f268 */
 goto L8c65f232;
L8c65f232: r[2]=rd(m,0x8c65f29cu,4); /* mov.l 0x8c65f29c,r2 */
 goto L8c65f234;
L8c65f234: target=r[2];*pr=0x8c65f238u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65f238;
L8c65f238: r[3]=r[14]; /* mov r14,r3 */
 goto L8c65f23a;
L8c65f23a: r[4]=r[14]; /* mov r14,r4 */
 goto L8c65f23c;
L8c65f23c: r[3]<<=2; /* shll2 r3 */
 goto L8c65f23e;
L8c65f23e: r[11]=r[8]; /* mov r8,r11 */
 goto L8c65f240;
L8c65f240: *sr=(*sr&~1u)|(r[4]>>31);r[4]<<=1; /* shll r4 */
 goto L8c65f242;
L8c65f242: r[4]+=r[3]; /* add r3,r4 */
 goto L8c65f244;
L8c65f244: r[12]=r[4]; /* mov r4,r12 */
 goto L8c65f246;
L8c65f246: r[13]=r[4]; /* mov r4,r13 */
 goto L8c65f248;
L8c65f248: r[3]=sx(m,0x8c65f284u,2); /* mov.w 0x8c65f284,r3 */
 goto L8c65f24a;
L8c65f24a: r[0]=rd(m,r[10],4); /* mov.l @r10,r0 */
 goto L8c65f24c;
L8c65f24c: r[3]+=r[0]; /* add r0,r3 */
 goto L8c65f24e;
L8c65f24e: r[3]+=r[12]; /* add r12,r3 */
 goto L8c65f250;
L8c65f250: r[2]=sx(m,r[3],1); /* mov.b @r3,r2 */
 goto L8c65f252;
L8c65f252: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c65f254;
L8c65f254: if((*sr&1u))goto L8c65f25e; /* bt 0x8c65f25e */
 goto L8c65f256;
L8c65f256: r[3]=rd(m,0x8c65f2a0u,4); /* mov.l 0x8c65f2a0,r3 */
 goto L8c65f258;
L8c65f258: r[5]=r[14]; /* mov r14,r5 */
 goto L8c65f25a;
L8c65f25a: target=r[3];*pr=0x8c65f25eu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65f25e;
L8c65f25e: r[11]+=1u; /* add #0x1,r11 */
 goto L8c65f260;
L8c65f260: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[9]); /* cmp/ge r9,r11 */
 goto L8c65f262;
L8c65f262: r[13]+=1u; /* add #0x1,r13 */
 goto L8c65f264;
L8c65f264: take=!(*sr&1u);r[12]+=1u;if(take)goto L8c65f248;goto L8c65f268; /* bf/s 0x8c65f248 */
 goto L8c65f268;
L8c65f268: r[14]+=1u; /* add #0x1,r14 */
 goto L8c65f26a;
L8c65f26a: r[3]=4u; /* mov #0x4,r3 */
 goto L8c65f26c;
L8c65f26c: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[3]); /* cmp/ge r3,r14 */
 goto L8c65f26e;
L8c65f26e: if(!(*sr&1u))goto L8c65f21c; /* bf 0x8c65f21c */
 goto L8c65f270;
L8c65f270: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65f272;
L8c65f272: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65f274;
L8c65f274: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c65f276;
L8c65f276: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c65f278;
L8c65f278: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c65f27a;
L8c65f27a: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c65f27c;
L8c65f27c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65f27e;
L8c65f27e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65f280;
L8c65f280: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
