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
/* Static C recovery of retail Tikal helper 8c666ad0..8c666b14.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_command_complete(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c666ad0;
L8c666ad0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c666ad2;
L8c666ad2: r[5]=1u; /* mov #0x1,r5 */
 goto L8c666ad4;
L8c666ad4: r[14]=rd(m,0x8c666b20u,4); /* mov.l 0x8c666b20,r14 */
 goto L8c666ad6;
L8c666ad6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c666ad8;
L8c666ad8: r[0]=sx(m,0x8c666b1cu,2); /* mov.w 0x8c666b1c,r0 */
 goto L8c666ada;
L8c666ada: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c666adc;
L8c666adc: r[3]=rd(m,0x8c666b30u,4); /* mov.l 0x8c666b30,r3 */
 goto L8c666ade;
L8c666ade: target=r[3];*pr=0x8c666ae2u;r[4]=rd(m,r[4]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666ae2;
L8c666ae2: r[5]=0u; /* mov #0x0,r5 */
 goto L8c666ae4;
L8c666ae4: r[4]=r[5];goto L8c666afa; /* bra 0x8c666afa */
 goto L8c666ae8;
L8c666ae8: r[0]=sx(m,0x8c666b18u,2); /* mov.w 0x8c666b18,r0 */
 goto L8c666aea;
L8c666aea: r[2]=r[4]; /* mov r4,r2 */
 goto L8c666aec;
L8c666aec: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c666aee;
L8c666aee: r[2]<<=2; /* shll2 r2 */
 goto L8c666af0;
L8c666af0: r[2]<<=2; /* shll2 r2 */
 goto L8c666af2;
L8c666af2: r[3]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r3 */
 goto L8c666af4;
L8c666af4: r[4]+=1u; /* add #0x1,r4 */
 goto L8c666af6;
L8c666af6: r[2]+=r[3]; /* add r3,r2 */
 goto L8c666af8;
L8c666af8: wr(m,r[2],r[5],1); /* mov.b r5,@r2 */
 goto L8c666afa;
L8c666afa: r[0]=sx(m,0x8c666b14u,2); /* mov.w 0x8c666b14,r0 */
 goto L8c666afc;
L8c666afc: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c666afe;
L8c666afe: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c666b00;
L8c666b00: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]); /* cmp/ge r2,r4 */
 goto L8c666b02;
L8c666b02: if(!(*sr&1u))goto L8c666ae8; /* bf 0x8c666ae8 */
 goto L8c666b04;
L8c666b04: r[1]=rd(m,r[14],4); /* mov.l @r14,r1 */
 goto L8c666b06;
L8c666b06: wr(m,r[1]+r[0],r[5],4); /* mov.l r5,@(r0,r1) */
 goto L8c666b08;
L8c666b08: r[0]+=4u; /* add #0x4,r0 */
 goto L8c666b0a;
L8c666b0a: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c666b0c;
L8c666b0c: wr(m,r[3]+r[0],r[5],4); /* mov.l r5,@(r0,r3) */
 goto L8c666b0e;
L8c666b0e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c666b10;
L8c666b10: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
