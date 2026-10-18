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
/* Retail game-facing play dispatch: MIDI setup/note or typed sound command. */
SA_STAGE_EXPORT int sa_sound_play_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c093b2a;
L8c093b2a: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c093b2c;
L8c093b2c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c093b2e;
L8c093b2e: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c093b30;
L8c093b30: r[14]=r[7]; /* mov r7,r14 */
 goto L8c093b32;
L8c093b32: r[3]=32u; /* mov #0x20,r3 */
 goto L8c093b34;
L8c093b34: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[3]); /* cmp/ge r3,r14 */
 goto L8c093b36;
L8c093b36: wr(m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c093b38;
L8c093b38: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c093b3a;
L8c093b3a: take=!(*sr&1u);wr(m,r[15]+0x8u,r[6],4);if(take)goto L8c093b58;goto L8c093b3e; /* bf/s 0x8c093b58 */
 goto L8c093b3e;
L8c093b3e: r[4]=r[14]; /* mov r14,r4 */
 goto L8c093b40;
L8c093b40: r[4]+=4294967266u; /* add #-0x1e,r4 */
 goto L8c093b42;
L8c093b42: r[0]=rd(m,0x8c093bb4u,4); /* mov.l 0x8c093bb4,r0 */
 goto L8c093b44;
L8c093b44: r[4]<<=2; /* shll2 r4 */
 goto L8c093b46;
L8c093b46: r[6]=rd(m,r[15],4); /* mov.l @r15,r6 */
 goto L8c093b48;
L8c093b48: r[4]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r4 */
 goto L8c093b4a;
L8c093b4a: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c093b4c;
L8c093b4c: r[7]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r7 */
 goto L8c093b4e;
L8c093b4e: r[15]+=12u; /* add #0xc,r15 */
 goto L8c093b50;
L8c093b50: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c093b52;
L8c093b52: r[2]=rd(m,0x8c093bb8u,4); /* mov.l 0x8c093bb8,r2 */
 goto L8c093b54;
L8c093b54: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c093b58: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c093b5a;
L8c093b5a: r[6]=rd(m,r[15],4); /* mov.l @r15,r6 */
 goto L8c093b5c;
L8c093b5c: *pr=0x8c093b60u;r[4]=r[14];status=call(ctx,0x8c093140u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093140 */
 goto L8c093b60;
L8c093b60: r[3]=rd(m,0x8c093bbcu,4); /* mov.l 0x8c093bbc,r3 */
 goto L8c093b62;
L8c093b62: r[4]=r[14]; /* mov r14,r4 */
 goto L8c093b64;
L8c093b64: r[6]=127u; /* mov #0x7f,r6 */
 goto L8c093b66;
L8c093b66: r[5]=rd(m,r[3],4); /* mov.l @r3,r5 */
 goto L8c093b68;
L8c093b68: r[15]+=12u; /* add #0xc,r15 */
 goto L8c093b6a;
L8c093b6a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c093b6c;
L8c093b6c: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c093360u,m,f,sr,r,pr); /* bra 0x8c093360 */
}
