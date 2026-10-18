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
/* Static C recovery of retail File backend open helper 8c6004d4..8c60054c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_backend_open(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6004d4;
L8c6004d4: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6004d6;
L8c6004d6: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6004d8;
L8c6004d8: r[13]=r[5]; /* mov r5,r13 */
 goto L8c6004da;
L8c6004da: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6004dc;
L8c6004dc: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c6004de;
L8c6004de: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6004e0;
L8c6004e0: take=!(*sr&1u);r[6]=0u;if(take)goto L8c6004ea;goto L8c6004e4; /* bf/s 0x8c6004ea */
 goto L8c6004e4;
L8c6004e4: r[3]=rd(m,0x8c60059cu,4); /* mov.l 0x8c60059c,r3 */
 goto L8c6004e6;
L8c6004e6: r[13]=rd(m,r[3],4); /* mov.l @r3,r13 */
 goto L8c6004e8;
L8c6004e8: r[13]=rd(m,r[13]+0x10u,4); /* mov.l @(0x10,r13),r13 */
 goto L8c6004ea;
L8c6004ea: r[2]=rd(m,0x8c6005a0u,4); /* mov.l 0x8c6005a0,r2 */
 goto L8c6004ec;
L8c6004ec: target=r[2];*pr=0x8c6004f0u;r[5]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6004f0;
L8c6004f0: r[5]=r[0]; /* mov r0,r5 */
 goto L8c6004f2;
L8c6004f2: *sr=(*sr&~1u)|!(r[5]&r[5]); /* tst r5,r5 */
 goto L8c6004f4;
L8c6004f4: if(!(*sr&1u))goto L8c6004fe; /* bf 0x8c6004fe */
 goto L8c6004f6;
L8c6004f6: *pr=0x8c6004fau;r[4]=4294967288u;status=call(ctx,0x8c5ffa08u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c5ffa08 */
 goto L8c6004fa;
L8c6004fa: goto L8c600540; /* bra 0x8c600540 */
 goto L8c6004fe;
L8c6004fe: r[7]=r[5]; /* mov r5,r7 */
 goto L8c600500;
L8c600500: r[2]=rd(m,0x8c60059cu,4); /* mov.l 0x8c60059c,r2 */
 goto L8c600502;
L8c600502: r[7]+=4294967295u; /* add #-0x1,r7 */
 goto L8c600504;
L8c600504: r[12]=sx(m,0x8c600596u,2); /* mov.w 0x8c600596,r12 */
 goto L8c600506;
L8c600506: r[3]=44u; /* mov #0x2c,r3 */
 goto L8c600508;
L8c600508: r[6]=rd(m,r[2],4); /* mov.l @r2,r6 */
 goto L8c60050a;
L8c60050a: *macl=r[3]*r[7]; /* mul.l r3,r7 */
 goto L8c60050c;
L8c60050c: r[14]=rd(m,r[6]+0x18u,4); /* mov.l @(0x18,r6),r14 */
 goto L8c60050e;
L8c60050e: r[4]=0u; /* mov #0x0,r4 */
 goto L8c600510;
L8c600510: r[7]=*macl;goto L8c600536; /* bra 0x8c600536 */
 goto L8c600514;
L8c600514: r[0]=76u; /* mov #0x4c,r0 */
 goto L8c600516;
L8c600516: r[3]=sx(m,r[14]+r[0],2); /* mov.w @(r0,r14),r3 */
 goto L8c600518;
L8c600518: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c60051a;
L8c60051a: if(!(*sr&1u))goto L8c600532; /* bf 0x8c600532 */
 goto L8c60051c;
L8c60051c: r[13]+=16u; /* add #0x10,r13 */
 goto L8c60051e;
L8c60051e: r[13]+=r[7]; /* add r7,r13 */
 goto L8c600520;
L8c600520: r[6]=rd(m,r[13],4); /* mov.l @r13,r6 */
 goto L8c600522;
L8c600522: r[7]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r7 */
 goto L8c600524;
L8c600524: r[6]+=r[12]; /* add r12,r6 */
 goto L8c600526;
L8c600526: *pr=0x8c60052au;r[4]=r[14];status=call(ctx,0x8c600464u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c600464 */
 goto L8c60052a;
L8c60052a: *pr=0x8c60052eu;r[4]=0u;status=call(ctx,0x8c5ffa08u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c5ffa08 */
 goto L8c60052e;
L8c60052e: r[0]=r[14];goto L8c600542; /* bra 0x8c600542 */
 goto L8c600532;
L8c600532: r[14]+=84u; /* add #0x54,r14 */
 goto L8c600534;
L8c600534: r[4]+=1u; /* add #0x1,r4 */
 goto L8c600536;
L8c600536: r[3]=rd(m,r[6],4); /* mov.l @r6,r3 */
 goto L8c600538;
L8c600538: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]); /* cmp/ge r3,r4 */
 goto L8c60053a;
L8c60053a: if(!(*sr&1u))goto L8c600514; /* bf 0x8c600514 */
 goto L8c60053c;
L8c60053c: *pr=0x8c600540u;r[4]=4294967287u;status=call(ctx,0x8c5ffa08u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c5ffa08 */
 goto L8c600540;
L8c600540: r[0]=0u; /* mov #0x0,r0 */
 goto L8c600542;
L8c600542: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c600544;
L8c600544: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c600546;
L8c600546: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c600548;
L8c600548: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
