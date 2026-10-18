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
/* Static C recovery of retail Tikal helper 8c07aef8..8c07af8c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_effect_primary_angles(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c07aef8;
L8c07aef8: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c07aefa;
L8c07aefa: r[14]=r[4]; /* mov r4,r14 */
 goto L8c07aefc;
L8c07aefc: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c07aefe;
L8c07aefe: r[5]=rd(m,r[14],4); /* mov.l @r14,r5 */
 goto L8c07af00;
L8c07af00: r[6]=16u; /* mov #0x10,r6 */
 goto L8c07af02;
L8c07af02: r[3]=rd(m,0x8c07af24u,4); /* mov.l 0x8c07af24,r3 */
 goto L8c07af04;
L8c07af04: r[6]<<=8; /* shll8 r6 */
 goto L8c07af06;
L8c07af06: target=r[3];*pr=0x8c07af0au;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c07af0a;
L8c07af0a: r[2]=rd(m,0x8c07af28u,4); /* mov.l 0x8c07af28,r2 */
 goto L8c07af0c;
L8c07af0c: r[4]=r[0]; /* mov r0,r4 */
 goto L8c07af0e;
L8c07af0e: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[2]); /* cmp/gt r2,r4 */
 goto L8c07af10;
L8c07af10: if(!(*sr&1u))goto L8c07af16; /* bf 0x8c07af16 */
 goto L8c07af12;
L8c07af12: r[1]=rd(m,0x8c07af2cu,4); /* mov.l 0x8c07af2c,r1 */
 goto L8c07af14;
L8c07af14: r[4]|=r[1]; /* or r1,r4 */
 goto L8c07af16;
L8c07af16: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c07af18;
L8c07af18: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[3]); /* cmp/gt r3,r4 */
 goto L8c07af1a;
L8c07af1a: if(!(*sr&1u))goto L8c07af40; /* bf 0x8c07af40 */
 goto L8c07af1c;
L8c07af1c: r[4]=rd(m,r[14]+0x28u,4);goto L8c07af48; /* bra 0x8c07af48 */
 goto L8c07af40;
L8c07af40: r[2]=rd(m,r[14]+0x2cu,4); /* mov.l @(0x2c,r14),r2 */
 goto L8c07af42;
L8c07af42: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]); /* cmp/ge r2,r4 */
 goto L8c07af44;
L8c07af44: if((*sr&1u))goto L8c07af48; /* bt 0x8c07af48 */
 goto L8c07af46;
L8c07af46: r[4]=rd(m,r[14]+0x2cu,4); /* mov.l @(0x2c,r14),r4 */
 goto L8c07af48;
L8c07af48: r[0]=88u; /* mov #0x58,r0 */
 goto L8c07af4a;
L8c07af4a: wr(m,r[14]+0x8u,r[4],4); /* mov.l r4,@(0x8,r14) */
 goto L8c07af4c;
L8c07af4c: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c07af4e;
L8c07af4e: r[3]=r[4]&65535u; /* extu.w r4,r3 */
 goto L8c07af50;
L8c07af50: r[6]=16u; /* mov #0x10,r6 */
 goto L8c07af52;
L8c07af52: wr(m,r[2]+0x18u,r[3],4); /* mov.l r3,@(0x18,r2) */
 goto L8c07af54;
L8c07af54: r[6]<<=8; /* shll8 r6 */
 goto L8c07af56;
L8c07af56: r[5]=rd(m,r[14]+0x4u,4); /* mov.l @(0x4,r14),r5 */
 goto L8c07af58;
L8c07af58: r[2]=rd(m,0x8c07b060u,4); /* mov.l 0x8c07b060,r2 */
 goto L8c07af5a;
L8c07af5a: target=r[2];*pr=0x8c07af5eu;r[4]=rd(m,r[14]+0xcu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c07af5e;
L8c07af5e: r[3]=rd(m,0x8c07b064u,4); /* mov.l 0x8c07b064,r3 */
 goto L8c07af60;
L8c07af60: r[4]=r[0]; /* mov r0,r4 */
 goto L8c07af62;
L8c07af62: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[3]); /* cmp/gt r3,r4 */
 goto L8c07af64;
L8c07af64: take=!(*sr&1u);r[3]=rd(m,r[14]+0x30u,4);if(take)goto L8c07af6c;goto L8c07af68; /* bf/s 0x8c07af6c */
 goto L8c07af68;
L8c07af68: r[1]=rd(m,0x8c07b068u,4); /* mov.l 0x8c07b068,r1 */
 goto L8c07af6a;
L8c07af6a: r[4]|=r[1]; /* or r1,r4 */
 goto L8c07af6c;
L8c07af6c: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[3]); /* cmp/gt r3,r4 */
 goto L8c07af6e;
L8c07af6e: if(!(*sr&1u))goto L8c07af74; /* bf 0x8c07af74 */
 goto L8c07af70;
L8c07af70: r[4]=rd(m,r[14]+0x30u,4);goto L8c07af7c; /* bra 0x8c07af7c */
 goto L8c07af74;
L8c07af74: r[2]=rd(m,r[14]+0x34u,4); /* mov.l @(0x34,r14),r2 */
 goto L8c07af76;
L8c07af76: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[2]); /* cmp/ge r2,r4 */
 goto L8c07af78;
L8c07af78: if((*sr&1u))goto L8c07af7c; /* bt 0x8c07af7c */
 goto L8c07af7a;
L8c07af7a: r[4]=rd(m,r[14]+0x34u,4); /* mov.l @(0x34,r14),r4 */
 goto L8c07af7c;
L8c07af7c: r[0]=88u; /* mov #0x58,r0 */
 goto L8c07af7e;
L8c07af7e: wr(m,r[14]+0xcu,r[4],4); /* mov.l r4,@(0xc,r14) */
 goto L8c07af80;
L8c07af80: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c07af82;
L8c07af82: r[3]=r[4]&65535u; /* extu.w r4,r3 */
 goto L8c07af84;
L8c07af84: wr(m,r[2]+0x1cu,r[3],4); /* mov.l r3,@(0x1c,r2) */
 goto L8c07af86;
L8c07af86: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c07af88;
L8c07af88: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
