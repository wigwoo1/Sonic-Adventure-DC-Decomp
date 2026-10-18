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
/* Static C recovery of the USA Rev A main executable transfer service. External calls use the supplied dispatcher. */
SA_STAGE_EXPORT int sa_engine_transfer_descriptor_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10f9f4;
L8c10f9f4: r[3]=rd(m,0x8c10fa54u,4); /* mov.l 0x8c10fa54,r3 */
 goto L8c10f9f6;
L8c10f9f6: r[2]=rd(m,0x8c10fa58u,4); /* mov.l 0x8c10fa58,r2 */
 goto L8c10f9f8;
L8c10f9f8: r[6]&=r[3]; /* and r3,r6 */
 goto L8c10f9fa;
L8c10f9fa: r[1]=rd(m,0x8c10fa5cu,4); /* mov.l 0x8c10fa5c,r1 */
 goto L8c10f9fc;
L8c10f9fc: r[5]&=r[3]; /* and r3,r5 */
 goto L8c10f9fe;
L8c10f9fe: wr(m,r[4]+0x4u,r[5],4); /* mov.l r5,@(0x4,r4) */
 goto L8c10fa00;
L8c10fa00: r[5]=1u; /* mov #0x1,r5 */
 goto L8c10fa02;
L8c10fa02: r[7]&=r[2]; /* and r2,r7 */
 goto L8c10fa04;
L8c10fa04: wr(m,r[4]+0x8u,r[6],4); /* mov.l r6,@(0x8,r4) */
 goto L8c10fa06;
L8c10fa06: r[7]|=r[1]; /* or r1,r7 */
 goto L8c10fa08;
L8c10fa08: wr(m,r[4]+0xcu,r[7],4); /* mov.l r7,@(0xc,r4) */
 goto L8c10fa0a;
L8c10fa0a: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c10fa0c;
L8c10fa0c: r[3]&=r[5]; /* and r5,r3 */
 goto L8c10fa0e;
L8c10fa0e: wr(m,r[4]+0x10u,r[3],4); /* mov.l r3,@(0x10,r4) */
 goto L8c10fa10;
L8c10fa10: r[0]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c10fa12;
L8c10fa12: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10fa14;
L8c10fa14: if(!(*sr&1u))goto L8c10fa1e; /* bf 0x8c10fa1e */
 goto L8c10fa16;
L8c10fa16: r[3]=rd(m,r[4]+0x14u,4); /* mov.l @(0x14,r4),r3 */
 goto L8c10fa18;
L8c10fa18: r[3]|=r[5]; /* or r5,r3 */
 goto L8c10fa1a;
L8c10fa1a: wr(m,r[4]+0x14u,r[3],4);goto L8c10fa26; /* bra 0x8c10fa26 */
 goto L8c10fa1e;
L8c10fa1e: r[0]=rd(m,r[4]+0x14u,4); /* mov.l @(0x14,r4),r0 */
 goto L8c10fa20;
L8c10fa20: r[3]=4294967293u; /* mov #-0x3,r3 */
 goto L8c10fa22;
L8c10fa22: r[0]&=r[3]; /* and r3,r0 */
 goto L8c10fa24;
L8c10fa24: wr(m,r[4]+0x14u,r[0],4); /* mov.l r0,@(0x14,r4) */
 goto L8c10fa26;
L8c10fa26: r[0]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r0 */
 goto L8c10fa28;
L8c10fa28: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10fa2a;
L8c10fa2a: if(!(*sr&1u))goto L8c10fa34; /* bf 0x8c10fa34 */
 goto L8c10fa2c;
L8c10fa2c: r[0]=rd(m,r[4]+0x14u,4); /* mov.l @(0x14,r4),r0 */
 goto L8c10fa2e;
L8c10fa2e: r[0]|=0x4u; /* or #0x4,r0 */
 goto L8c10fa30;
L8c10fa30: wr(m,r[4]+0x14u,r[0],4);goto L8c10fa3c; /* bra 0x8c10fa3c */
 goto L8c10fa34;
L8c10fa34: r[2]=rd(m,r[4]+0x14u,4); /* mov.l @(0x14,r4),r2 */
 goto L8c10fa36;
L8c10fa36: r[3]=4294967294u; /* mov #-0x2,r3 */
 goto L8c10fa38;
L8c10fa38: r[2]&=r[3]; /* and r3,r2 */
 goto L8c10fa3a;
L8c10fa3a: wr(m,r[4]+0x14u,r[2],4); /* mov.l r2,@(0x14,r4) */
 goto L8c10fa3c;
L8c10fa3c: r[0]=rd(m,r[4]+0x14u,4); /* mov.l @(0x14,r4),r0 */
 goto L8c10fa3e;
L8c10fa3e: r[0]&=0x7u; /* and #0x7,r0 */
 goto L8c10fa40;
L8c10fa40: wr(m,r[4]+0x14u,r[0],4); /* mov.l r0,@(0x14,r4) */
 goto L8c10fa42;
L8c10fa42: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c10fa44;
L8c10fa44: wr(m,r[4]+0x1cu,r[3],4); /* mov.l r3,@(0x1c,r4) */
 goto L8c10fa46;
L8c10fa46: r[3]=0u; /* mov #0x0,r3 */
 goto L8c10fa48;
L8c10fa48: r[2]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r2 */
 goto L8c10fa4a;
L8c10fa4a: wr(m,r[4]+0x20u,r[2],4); /* mov.l r2,@(0x20,r4) */
 goto L8c10fa4c;
L8c10fa4c: wr(m,r[4]+0x30u,r[3],4); /* mov.l r3,@(0x30,r4) */
 goto L8c10fa4e;
L8c10fa4e: r[0]=r[3];return 0; /* rts */
}
