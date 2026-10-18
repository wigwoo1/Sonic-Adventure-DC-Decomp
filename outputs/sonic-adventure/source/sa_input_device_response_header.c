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
/* Static C recovery of retail Tikal helper 8c65ff5a..8c65ffb8.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_response_header(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65ff5a;
L8c65ff5a: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c65ff5c;
L8c65ff5c: r[2]=rd(m,0x8c660054u,4); /* mov.l 0x8c660054,r2 */
 goto L8c65ff5e;
L8c65ff5e: r[1]=sx(m,0x8c66004cu,2); /* mov.w 0x8c66004c,r1 */
 goto L8c65ff60;
L8c65ff60: r[3]=rd(m,r[5],4);r[5]+=4; /* mov.l @r5+,r3 */
 goto L8c65ff62;
L8c65ff62: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c65ff64;
L8c65ff64: r[0]=rd(m,r[2],4); /* mov.l @r2,r0 */
 goto L8c65ff66;
L8c65ff66: r[0]=rd(m,r[1]+r[0],4); /* mov.l @(r0,r1),r0 */
 goto L8c65ff68;
L8c65ff68: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c65ff6a;
L8c65ff6a: if(!(*sr&1u))goto L8c65ff80; /* bf 0x8c65ff80 */
 goto L8c65ff6c;
L8c65ff6c: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65ff6e;
L8c65ff6e: r[0]=sx(m,r[6],1);r[6]+=1; /* mov.b @r6+,r0 */
 goto L8c65ff70;
L8c65ff70: wr(m,r[4]+0xbu,r[0],1); /* mov.b r0,@(0xb,r4) */
 goto L8c65ff72;
L8c65ff72: r[0]=sx(m,r[6],1);r[6]+=1; /* mov.b @r6+,r0 */
 goto L8c65ff74;
L8c65ff74: wr(m,r[4]+0xau,r[0],1); /* mov.b r0,@(0xa,r4) */
 goto L8c65ff76;
L8c65ff76: r[0]=sx(m,r[6],1);r[6]+=1; /* mov.b @r6+,r0 */
 goto L8c65ff78;
L8c65ff78: wr(m,r[4]+0x9u,r[0],1); /* mov.b r0,@(0x9,r4) */
 goto L8c65ff7a;
L8c65ff7a: r[0]=sx(m,r[6],1); /* mov.b @r6,r0 */
 goto L8c65ff7c;
L8c65ff7c: wr(m,r[4]+0x8u,r[0],1);goto L8c65ff92; /* bra 0x8c65ff92 */
 goto L8c65ff80;
L8c65ff80: r[6]=r[15]; /* mov r15,r6 */
 goto L8c65ff82;
L8c65ff82: r[0]=sx(m,r[6],1);r[6]+=1; /* mov.b @r6+,r0 */
 goto L8c65ff84;
L8c65ff84: wr(m,r[4]+0x8u,r[0],1); /* mov.b r0,@(0x8,r4) */
 goto L8c65ff86;
L8c65ff86: r[0]=sx(m,r[6],1);r[6]+=1; /* mov.b @r6+,r0 */
 goto L8c65ff88;
L8c65ff88: wr(m,r[4]+0x9u,r[0],1); /* mov.b r0,@(0x9,r4) */
 goto L8c65ff8a;
L8c65ff8a: r[0]=sx(m,r[6],1);r[6]+=1; /* mov.b @r6+,r0 */
 goto L8c65ff8c;
L8c65ff8c: wr(m,r[4]+0xau,r[0],1); /* mov.b r0,@(0xa,r4) */
 goto L8c65ff8e;
L8c65ff8e: r[0]=sx(m,r[6],1); /* mov.b @r6,r0 */
 goto L8c65ff90;
L8c65ff90: wr(m,r[4]+0xbu,r[0],1); /* mov.b r0,@(0xb,r4) */
 goto L8c65ff92;
L8c65ff92: r[0]=sx(m,r[4]+0x8u,1); /* mov.b @(0x8,r4),r0 */
 goto L8c65ff94;
L8c65ff94: r[3]=sx(m,0x8c66004eu,2); /* mov.w 0x8c66004e,r3 */
 goto L8c65ff96;
L8c65ff96: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c65ff98;
L8c65ff98: *sr=(*sr&~1u)|(r[3]==r[0]); /* cmp/eq r3,r0 */
 goto L8c65ff9a;
L8c65ff9a: if(!(*sr&1u))goto L8c65ffa2; /* bf 0x8c65ffa2 */
 goto L8c65ff9c;
L8c65ff9c: r[1]=4294967295u; /* mov #-0x1,r1 */
 goto L8c65ff9e;
L8c65ff9e: wr(m,r[4]+0x14u,r[1],4);goto L8c65ffb2; /* bra 0x8c65ffb2 */
 goto L8c65ffa2;
L8c65ffa2: r[2]=sx(m,0x8c660050u,2); /* mov.w 0x8c660050,r2 */
 goto L8c65ffa4;
L8c65ffa4: *sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[2]); /* cmp/ge r2,r0 */
 goto L8c65ffa6;
L8c65ffa6: if((*sr&1u))goto L8c65ffae; /* bt 0x8c65ffae */
 goto L8c65ffa8;
L8c65ffa8: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65ffaa;
L8c65ffaa: wr(m,r[4]+0x14u,r[0],4);goto L8c65ffb2; /* bra 0x8c65ffb2 */
 goto L8c65ffae;
L8c65ffae: r[1]=rd(m,r[5],4); /* mov.l @r5,r1 */
 goto L8c65ffb0;
L8c65ffb0: wr(m,r[4]+0x14u,r[1],4); /* mov.l r1,@(0x14,r4) */
 goto L8c65ffb2;
L8c65ffb2: r[0]=0u; /* mov #0x0,r0 */
 goto L8c65ffb4;
L8c65ffb4: r[15]+=4u;return 0; /* rts */
}
