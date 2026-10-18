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
/* Static C recovery of retail Tikal helper 8c66120c..8c66126c.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_context_parameters_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66120c;
L8c66120c: r[2]=rd(m,0x8c6612a0u,4); /* mov.l 0x8c6612a0,r2 */
 goto L8c66120e;
L8c66120e: r[3]=rd(m,0x8c661298u,4); /* mov.l 0x8c661298,r3 */
 goto L8c661210;
L8c661210: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c661212;
L8c661212: r[2]=rd(m,0x8c6612a8u,4); /* mov.l 0x8c6612a8,r2 */
 goto L8c661214;
L8c661214: r[1]=rd(m,0x8c6612a4u,4); /* mov.l 0x8c6612a4,r1 */
 goto L8c661216;
L8c661216: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c661218;
L8c661218: r[0]=rd(m,0x8c6612acu,4); /* mov.l 0x8c6612ac,r0 */
 goto L8c66121a;
L8c66121a: r[3]=0u; /* mov #0x0,r3 */
 goto L8c66121c;
L8c66121c: wr(m,r[0],r[3],4); /* mov.l r3,@r0 */
 goto L8c66121e;
L8c66121e: r[2]=rd(m,0x8c6612b4u,4); /* mov.l 0x8c6612b4,r2 */
 goto L8c661220;
L8c661220: r[7]=rd(m,0x8c6612b0u,4); /* mov.l 0x8c6612b0,r7 */
 goto L8c661222;
L8c661222: wr(m,r[2],r[7],4); /* mov.l r7,@r2 */
 goto L8c661224;
L8c661224: r[3]=rd(m,0x8c6612bcu,4); /* mov.l 0x8c6612bc,r3 */
 goto L8c661226;
L8c661226: r[6]=rd(m,0x8c6612b8u,4); /* mov.l 0x8c6612b8,r6 */
 goto L8c661228;
L8c661228: wr(m,r[3],r[6],4); /* mov.l r6,@r3 */
 goto L8c66122a;
L8c66122a: r[5]=rd(m,0x8c6612c0u,4); /* mov.l 0x8c6612c0,r5 */
 goto L8c66122c;
L8c66122c: r[1]+=16u; /* add #0x10,r1 */
 goto L8c66122e;
L8c66122e: wr(m,r[1],r[5],4); /* mov.l r5,@r1 */
 goto L8c661230;
L8c661230: r[4]=rd(m,0x8c6612c4u,4); /* mov.l 0x8c6612c4,r4 */
 goto L8c661232;
L8c661232: r[2]+=12u; /* add #0xc,r2 */
 goto L8c661234;
L8c661234: wr(m,r[2],r[4],4); /* mov.l r4,@r2 */
 goto L8c661236;
L8c661236: r[1]=rd(m,0x8c6612c8u,4); /* mov.l 0x8c6612c8,r1 */
 goto L8c661238;
L8c661238: r[3]+=12u; /* add #0xc,r3 */
 goto L8c66123a;
L8c66123a: wr(m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto L8c66123c;
L8c66123c: r[0]=rd(m,0x8c6612ccu,4); /* mov.l 0x8c6612cc,r0 */
 goto L8c66123e;
L8c66123e: wr(m,r[0],r[7],4); /* mov.l r7,@r0 */
 goto L8c661240;
L8c661240: r[2]+=12u; /* add #0xc,r2 */
 goto L8c661242;
L8c661242: wr(m,r[2],r[5],4); /* mov.l r5,@r2 */
 goto L8c661244;
L8c661244: r[3]+=12u; /* add #0xc,r3 */
 goto L8c661246;
L8c661246: wr(m,r[3],r[4],4); /* mov.l r4,@r3 */
 goto L8c661248;
L8c661248: r[2]=rd(m,0x8c6612d4u,4); /* mov.l 0x8c6612d4,r2 */
 goto L8c66124a;
L8c66124a: r[1]=rd(m,0x8c6612d0u,4); /* mov.l 0x8c6612d0,r1 */
 goto L8c66124c;
L8c66124c: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c66124e;
L8c66124e: r[0]=rd(m,0x8c6612d8u,4); /* mov.l 0x8c6612d8,r0 */
 goto L8c661250;
L8c661250: wr(m,r[0],r[6],4); /* mov.l r6,@r0 */
 goto L8c661252;
L8c661252: r[3]+=12u; /* add #0xc,r3 */
 goto L8c661254;
L8c661254: wr(m,r[3],r[5],4); /* mov.l r5,@r3 */
 goto L8c661256;
L8c661256: r[2]=rd(m,0x8c6612dcu,4); /* mov.l 0x8c6612dc,r2 */
 goto L8c661258;
L8c661258: wr(m,r[2],r[4],4); /* mov.l r4,@r2 */
 goto L8c66125a;
L8c66125a: r[1]+=4294967272u; /* add #-0x18,r1 */
 goto L8c66125c;
L8c66125c: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c66125e;
L8c66125e: r[0]=rd(m,0x8c6612e0u,4); /* mov.l 0x8c6612e0,r0 */
 goto L8c661260;
L8c661260: wr(m,r[0],r[3],4); /* mov.l r3,@r0 */
 goto L8c661262;
L8c661262: r[3]=rd(m,0x8c661280u,4); /* mov.l 0x8c661280,r3 */
 goto L8c661264;
L8c661264: r[2]=rd(m,0x8c661290u,4); /* mov.l 0x8c661290,r2 */
 goto L8c661266;
L8c661266: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c661268;
L8c661268: r[0]=0u;return 0; /* rts */
}
