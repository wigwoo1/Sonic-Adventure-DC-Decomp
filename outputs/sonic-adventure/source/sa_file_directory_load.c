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
/* Static C recovery of retail Tikal helper 8c6454fe..8c6455ce.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_directory_load(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6454fe;
L8c6454fe: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c645500;
L8c645500: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c645502;
L8c645502: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c645504;
L8c645504: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c645506;
L8c645506: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c645508;
L8c645508: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c64550a;
L8c64550a: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c64550c;
L8c64550c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c64550e;
L8c64550e: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c645510;
L8c645510: r[9]=r[6]; /* mov r6,r9 */
 goto L8c645512;
L8c645512: r[3]=r[5]; /* mov r5,r3 */
 goto L8c645514;
L8c645514: r[12]=r[6]; /* mov r6,r12 */
 goto L8c645516;
L8c645516: wr(m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c645518;
L8c645518: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c64551a;
L8c64551a: r[11]=r[7]; /* mov r7,r11 */
 goto L8c64551c;
L8c64551c: r[2]=rd(m,r[9]+0x8u,4); /* mov.l @(0x8,r9),r2 */
 goto L8c64551e;
L8c64551e: *sr=(*sr&~1u)|(r[3]==r[2]); /* cmp/eq r3,r2 */
 goto L8c645520;
L8c645520: take=(*sr&1u);r[12]+=16u;if(take)goto L8c6455b8;goto L8c645524; /* bt/s 0x8c6455b8 */
 goto L8c645524;
L8c645524: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c645526;
L8c645526: r[14]=0u; /* mov #0x0,r14 */
 goto L8c645528;
L8c645528: r[8]=sx(m,0x8c645550u,2); /* mov.w 0x8c645550,r8 */
 goto L8c64552a;
L8c64552a: wr(m,r[9]+0x8u,r[3],4); /* mov.l r3,@(0x8,r9) */
 goto L8c64552c;
L8c64552c: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c64552e;
L8c64552e: r[10]=rd(m,r[9]+0x4u,4); /* mov.l @(0x4,r9),r10 */
 goto L8c645530;
L8c645530: r[3]+=r[8]; /* add r8,r3 */
 goto L8c645532;
L8c645532: wr(m,r[15]+0x8u,r[3],4);goto L8c645592; /* bra 0x8c645592 */
 goto L8c645536;
L8c645536: r[3]=rd(m,0x8c645554u,4); /* mov.l 0x8c645554,r3 */
 goto L8c645538;
L8c645538: r[5]=1u; /* mov #0x1,r5 */
 goto L8c64553a;
L8c64553a: r[6]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r6 */
 goto L8c64553c;
L8c64553c: target=r[3];*pr=0x8c645540u;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c645540;
L8c645540: r[4]=r[0]; /* mov r0,r4 */
 goto L8c645542;
L8c645542: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c645544;
L8c645544: if((*sr&1u))goto L8c645558; /* bt 0x8c645558 */
 goto L8c645546;
L8c645546: r[3]=0u; /* mov #0x0,r3 */
 goto L8c645548;
L8c645548: wr(m,r[9]+0x8u,r[3],4); /* mov.l r3,@(0x8,r9) */
 goto L8c64554a;
L8c64554a: r[0]=r[4];goto L8c6455ba; /* bra 0x8c6455ba */
 goto L8c645558;
L8c645558: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]); /* cmp/ge r10,r14 */
 goto L8c64555a;
L8c64555a: take=(*sr&1u);r[13]=rd(m,r[15]+0x4u,4);if(take)goto L8c645596;goto L8c64555e; /* bt/s 0x8c645596 */
 goto L8c64555e;
L8c64555e: r[5]=r[12]; /* mov r12,r5 */
 goto L8c645560;
L8c645560: *pr=0x8c645564u;r[4]=r[13];status=call(ctx,0x8c645468u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c645468 */
 goto L8c645564;
L8c645564: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c645566;
L8c645566: take=(*sr&1u);r[4]=r[0];if(take)goto L8c645586;goto L8c64556a; /* bt/s 0x8c645586 */
 goto L8c64556a;
L8c64556a: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c64556c;
L8c64556c: if(!(*sr&1u))goto L8c645576; /* bf 0x8c645576 */
 goto L8c64556e;
L8c64556e: r[2]=rd(m,r[12]+0x4u,4); /* mov.l @(0x4,r12),r2 */
 goto L8c645570;
L8c645570: *sr=(*sr&~1u)|(r[11]==r[2]); /* cmp/eq r11,r2 */
 goto L8c645572;
L8c645572: if((*sr&1u))goto L8c645576; /* bt 0x8c645576 */
 goto L8c645574;
L8c645574: r[11]=rd(m,r[12]+0x4u,4); /* mov.l @(0x4,r12),r11 */
 goto L8c645576;
L8c645576: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c645578;
L8c645578: r[13]+=r[4]; /* add r4,r13 */
 goto L8c64557a;
L8c64557a: r[12]+=44u; /* add #0x2c,r12 */
 goto L8c64557c;
L8c64557c: *sr=(*sr&~1u)|(r[13]>=r[2]); /* cmp/hs r2,r13 */
 goto L8c64557e;
L8c64557e: take=(*sr&1u);r[14]+=1u;if(take)goto L8c645586;goto L8c645582; /* bt/s 0x8c645586 */
 goto L8c645582;
L8c645582: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]); /* cmp/ge r10,r14 */
 goto L8c645584;
L8c645584: if(!(*sr&1u))goto L8c64555e; /* bf 0x8c64555e */
 goto L8c645586;
L8c645586: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[10]); /* cmp/ge r10,r14 */
 goto L8c645588;
L8c645588: if((*sr&1u))goto L8c645596; /* bt 0x8c645596 */
 goto L8c64558a;
L8c64558a: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c64558c;
L8c64558c: r[11]-=r[8]; /* sub r8,r11 */
 goto L8c64558e;
L8c64558e: r[2]+=1u; /* add #0x1,r2 */
 goto L8c645590;
L8c645590: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c645592;
L8c645592: *sr=(*sr&~1u)|((int32_t)r[11]>0); /* cmp/pl r11 */
 goto L8c645594;
L8c645594: if((*sr&1u))goto L8c645536; /* bt 0x8c645536 */
 goto L8c645596;
L8c645596: *sr=(*sr&~1u)|(r[10]==r[14]); /* cmp/eq r10,r14 */
 goto L8c645598;
L8c645598: take=!(*sr&1u);wr(m,r[9],r[14],4);if(take)goto L8c6455b8;goto L8c64559c; /* bf/s 0x8c6455b8 */
 goto L8c64559c;
L8c64559c: *sr=(*sr&~1u)|!(r[11]&r[11]); /* tst r11,r11 */
 goto L8c64559e;
L8c64559e: if((*sr&1u))goto L8c6455b8; /* bt 0x8c6455b8 */
 goto L8c6455a0;
L8c6455a0: *sr=(*sr&~1u)|(r[8]==r[11]); /* cmp/eq r8,r11 */
 goto L8c6455a2;
L8c6455a2: if(!(*sr&1u))goto L8c6455b0; /* bf 0x8c6455b0 */
 goto L8c6455a4;
L8c6455a4: r[2]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r2 */
 goto L8c6455a6;
L8c6455a6: *sr=(*sr&~1u)|(r[13]>=r[2]); /* cmp/hs r2,r13 */
 goto L8c6455a8;
L8c6455a8: if((*sr&1u))goto L8c6455b8; /* bt 0x8c6455b8 */
 goto L8c6455aa;
L8c6455aa: r[1]=sx(m,r[13],1); /* mov.b @r13,r1 */
 goto L8c6455ac;
L8c6455ac: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c6455ae;
L8c6455ae: if((*sr&1u))goto L8c6455b8; /* bt 0x8c6455b8 */
 goto L8c6455b0;
L8c6455b0: r[2]=0u; /* mov #0x0,r2 */
 goto L8c6455b2;
L8c6455b2: wr(m,r[9]+0x8u,r[2],4); /* mov.l r2,@(0x8,r9) */
 goto L8c6455b4;
L8c6455b4: r[0]=4294967284u;goto L8c6455ba; /* bra 0x8c6455ba */
 goto L8c6455b8;
L8c6455b8: r[0]=0u; /* mov #0x0,r0 */
 goto L8c6455ba;
L8c6455ba: r[15]+=12u; /* add #0xc,r15 */
 goto L8c6455bc;
L8c6455bc: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6455be;
L8c6455be: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c6455c0;
L8c6455c0: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c6455c2;
L8c6455c2: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c6455c4;
L8c6455c4: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c6455c6;
L8c6455c6: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6455c8;
L8c6455c8: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6455ca;
L8c6455ca: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
