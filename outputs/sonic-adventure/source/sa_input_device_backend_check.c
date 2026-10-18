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
/* Static C recovery of retail Tikal helper 8c64b4e4..8c64b584.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_backend_check(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c64b4e4;
L8c64b4e4: r[5]=rd(m,r[4]+0x1cu,4); /* mov.l @(0x1c,r4),r5 */
 goto L8c64b4e6;
L8c64b4e6: r[6]=1u; /* mov #0x1,r6 */
 goto L8c64b4e8;
L8c64b4e8: r[7]=rd(m,r[5],4); /* mov.l @r5,r7 */
 goto L8c64b4ea;
L8c64b4ea: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[6]); /* cmp/ge r6,r7 */
 goto L8c64b4ec;
L8c64b4ec: if(!(*sr&1u))goto L8c64b57a; /* bf 0x8c64b57a */
 goto L8c64b4ee;
L8c64b4ee: r[2]=sx(m,0x8c64b5beu,2); /* mov.w 0x8c64b5be,r2 */
 goto L8c64b4f0;
L8c64b4f0: *sr=(*sr&~1u)|((int32_t)r[7]>(int32_t)r[2]); /* cmp/gt r2,r7 */
 goto L8c64b4f2;
L8c64b4f2: if((*sr&1u))goto L8c64b57a; /* bt 0x8c64b57a */
 goto L8c64b4f4;
L8c64b4f4: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c64b4f6;
L8c64b4f6: r[2]=rd(m,r[5]+0x8u,4); /* mov.l @(0x8,r5),r2 */
 goto L8c64b4f8;
L8c64b4f8: *sr=(*sr&~1u)|(r[2]==r[3]); /* cmp/eq r2,r3 */
 goto L8c64b4fa;
L8c64b4fa: if(!(*sr&1u))goto L8c64b57a; /* bf 0x8c64b57a */
 goto L8c64b4fc;
L8c64b4fc: r[7]=rd(m,r[5]+0xcu,4); /* mov.l @(0xc,r5),r7 */
 goto L8c64b4fe;
L8c64b4fe: *sr=(*sr&~1u)|((int32_t)r[7]>=0); /* cmp/pz r7 */
 goto L8c64b500;
L8c64b500: if(!(*sr&1u))goto L8c64b57a; /* bf 0x8c64b57a */
 goto L8c64b502;
L8c64b502: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c64b504;
L8c64b504: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[2]); /* cmp/ge r2,r7 */
 goto L8c64b506;
L8c64b506: if((*sr&1u))goto L8c64b57a; /* bt 0x8c64b57a */
 goto L8c64b508;
L8c64b508: r[7]=rd(m,r[5]+0x14u,4); /* mov.l @(0x14,r5),r7 */
 goto L8c64b50a;
L8c64b50a: *sr=(*sr&~1u)|((int32_t)r[7]>=0); /* cmp/pz r7 */
 goto L8c64b50c;
L8c64b50c: if(!(*sr&1u))goto L8c64b57a; /* bf 0x8c64b57a */
 goto L8c64b50e;
L8c64b50e: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c64b510;
L8c64b510: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[3]); /* cmp/ge r3,r7 */
 goto L8c64b512;
L8c64b512: if((*sr&1u))goto L8c64b57a; /* bt 0x8c64b57a */
 goto L8c64b514;
L8c64b514: r[7]=rd(m,r[5]+0x34u,4); /* mov.l @(0x34,r5),r7 */
 goto L8c64b516;
L8c64b516: *sr=(*sr&~1u)|((int32_t)r[7]>=0); /* cmp/pz r7 */
 goto L8c64b518;
L8c64b518: if(!(*sr&1u))goto L8c64b57a; /* bf 0x8c64b57a */
 goto L8c64b51a;
L8c64b51a: r[2]=rd(m,r[5],4); /* mov.l @r5,r2 */
 goto L8c64b51c;
L8c64b51c: *sr=(*sr&~1u)|((int32_t)r[7]>(int32_t)r[2]); /* cmp/gt r2,r7 */
 goto L8c64b51e;
L8c64b51e: if((*sr&1u))goto L8c64b57a; /* bt 0x8c64b57a */
 goto L8c64b520;
L8c64b520: r[7]=rd(m,r[5]+0x38u,4); /* mov.l @(0x38,r5),r7 */
 goto L8c64b522;
L8c64b522: *sr=(*sr&~1u)|((int32_t)r[7]>=0); /* cmp/pz r7 */
 goto L8c64b524;
L8c64b524: if(!(*sr&1u))goto L8c64b57a; /* bf 0x8c64b57a */
 goto L8c64b526;
L8c64b526: r[3]=rd(m,r[5],4); /* mov.l @r5,r3 */
 goto L8c64b528;
L8c64b528: *sr=(*sr&~1u)|((int32_t)r[7]>(int32_t)r[3]); /* cmp/gt r3,r7 */
 goto L8c64b52a;
L8c64b52a: if((*sr&1u))goto L8c64b57a; /* bt 0x8c64b57a */
 goto L8c64b52c;
L8c64b52c: r[2]=rd(m,0x8c64b5ccu,4); /* mov.l 0x8c64b5cc,r2 */
 goto L8c64b52e;
L8c64b52e: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c64b530;
L8c64b530: r[1]=rd(m,r[3]+0x8u,4); /* mov.l @(0x8,r3),r1 */
 goto L8c64b532;
L8c64b532: *sr=(*sr&~1u)|!(r[1]&r[1]); /* tst r1,r1 */
 goto L8c64b534;
L8c64b534: if(!(*sr&1u))goto L8c64b546; /* bf 0x8c64b546 */
 goto L8c64b536;
L8c64b536: r[3]=rd(m,r[5]+0x10u,4); /* mov.l @(0x10,r5),r3 */
 goto L8c64b538;
L8c64b538: r[0]=sx(m,r[4]+0x10u,2); /* mov.w @(0x10,r4),r0 */
 goto L8c64b53a;
L8c64b53a: *sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[3]); /* cmp/ge r3,r0 */
 goto L8c64b53c;
L8c64b53c: if(!(*sr&1u))goto L8c64b57a; /* bf 0x8c64b57a */
 goto L8c64b53e;
L8c64b53e: r[3]=rd(m,r[5]+0x18u,4); /* mov.l @(0x18,r5),r3 */
 goto L8c64b540;
L8c64b540: r[0]=sx(m,r[4]+0x12u,2); /* mov.w @(0x12,r4),r0 */
 goto L8c64b542;
L8c64b542: *sr=(*sr&~1u)|((int32_t)r[0]>=(int32_t)r[3]); /* cmp/ge r3,r0 */
 goto L8c64b544;
L8c64b544: if(!(*sr&1u))goto L8c64b57a; /* bf 0x8c64b57a */
 goto L8c64b546;
L8c64b546: r[1]=sx(m,0x8c64b5c0u,2); /* mov.w 0x8c64b5c0,r1 */
 goto L8c64b548;
L8c64b548: r[0]=rd(m,r[5],4); /* mov.l @r5,r0 */
 goto L8c64b54a;
L8c64b54a: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c64b54c;
L8c64b54c: if((*sr&1u))goto L8c64b564; /* bt 0x8c64b564 */
 goto L8c64b54e;
L8c64b54e: r[1]=sx(m,0x8c64b5c2u,2); /* mov.w 0x8c64b5c2,r1 */
 goto L8c64b550;
L8c64b550: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c64b552;
L8c64b552: if((*sr&1u))goto L8c64b56a; /* bt 0x8c64b56a */
 goto L8c64b554;
L8c64b554: r[1]=sx(m,0x8c64b5c4u,2); /* mov.w 0x8c64b5c4,r1 */
 goto L8c64b556;
L8c64b556: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c64b558;
L8c64b558: if((*sr&1u))goto L8c64b56e; /* bt 0x8c64b56e */
 goto L8c64b55a;
L8c64b55a: r[1]=sx(m,0x8c64b5beu,2); /* mov.w 0x8c64b5be,r1 */
 goto L8c64b55c;
L8c64b55c: *sr=(*sr&~1u)|(r[1]==r[0]); /* cmp/eq r1,r0 */
 goto L8c64b55e;
L8c64b55e: if((*sr&1u))goto L8c64b574; /* bt 0x8c64b574 */
 goto L8c64b560;
L8c64b560: goto L8c64b57a; /* bra 0x8c64b57a */
 goto L8c64b564;
L8c64b564: r[3]=0u; /* mov #0x0,r3 */
 goto L8c64b566;
L8c64b566: wr(m,r[4]+0x4u,r[3],4);goto L8c64b57e; /* bra 0x8c64b57e */
 goto L8c64b56a;
L8c64b56a: wr(m,r[4]+0x4u,r[6],4);goto L8c64b57e; /* bra 0x8c64b57e */
 goto L8c64b56e;
L8c64b56e: r[2]=2u; /* mov #0x2,r2 */
 goto L8c64b570;
L8c64b570: wr(m,r[4]+0x4u,r[2],4);goto L8c64b57e; /* bra 0x8c64b57e */
 goto L8c64b574;
L8c64b574: r[1]=3u; /* mov #0x3,r1 */
 goto L8c64b576;
L8c64b576: wr(m,r[4]+0x4u,r[1],4);goto L8c64b57e; /* bra 0x8c64b57e */
 goto L8c64b57a;
L8c64b57a: r[0]=4294967295u;return 0; /* rts */
L8c64b57e: r[0]=0u; /* mov #0x0,r0 */
 goto L8c64b580;
L8c64b580: return 0; /* rts */
}
