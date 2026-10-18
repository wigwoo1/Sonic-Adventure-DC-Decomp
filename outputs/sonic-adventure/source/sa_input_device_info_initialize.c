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
/* Static C recovery of retail Tikal helper 8c6602d2..8c660322.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_info_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6602d2;
L8c6602d2: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6602d4;
L8c6602d4: r[6]=30u; /* mov #0x1e,r6 */
 goto L8c6602d6;
L8c6602d6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6602d8;
L8c6602d8: r[14]=r[4]; /* mov r4,r14 */
 goto L8c6602da;
L8c6602da: *pr=0x8c6602deu;r[5]=0u;status=call(ctx,0x8c6607a8u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6607a8 */
 goto L8c6602de;
L8c6602de: r[2]=r[14]; /* mov r14,r2 */
 goto L8c6602e0;
L8c6602e0: r[4]=110u; /* mov #0x6e,r4 */
 goto L8c6602e2;
L8c6602e2: r[0]=21u; /* mov #0x15,r0 */
 goto L8c6602e4;
L8c6602e4: r[2]+=20u; /* add #0x14,r2 */
 goto L8c6602e6;
L8c6602e6: r[3]=40u; /* mov #0x28,r3 */
 goto L8c6602e8;
L8c6602e8: wr(m,r[2],r[3],1); /* mov.b r3,@r2 */
 goto L8c6602ea;
L8c6602ea: r[3]=111u; /* mov #0x6f,r3 */
 goto L8c6602ec;
L8c6602ec: wr(m,r[14]+r[0],r[4],1); /* mov.b r4,@(r0,r14) */
 goto L8c6602ee;
L8c6602ee: r[0]=22u; /* mov #0x16,r0 */
 goto L8c6602f0;
L8c6602f0: wr(m,r[14]+r[0],r[3],1); /* mov.b r3,@(r0,r14) */
 goto L8c6602f2;
L8c6602f2: r[0]=23u; /* mov #0x17,r0 */
 goto L8c6602f4;
L8c6602f4: wr(m,r[14]+r[0],r[4],1); /* mov.b r4,@(r0,r14) */
 goto L8c6602f6;
L8c6602f6: r[0]=24u; /* mov #0x18,r0 */
 goto L8c6602f8;
L8c6602f8: r[3]=101u; /* mov #0x65,r3 */
 goto L8c6602fa;
L8c6602fa: r[4]=r[14]; /* mov r14,r4 */
 goto L8c6602fc;
L8c6602fc: wr(m,r[14]+r[0],r[3],1); /* mov.b r3,@(r0,r14) */
 goto L8c6602fe;
L8c6602fe: r[0]=25u; /* mov #0x19,r0 */
 goto L8c660300;
L8c660300: r[2]=41u; /* mov #0x29,r2 */
 goto L8c660302;
L8c660302: r[4]+=26u; /* add #0x1a,r4 */
 goto L8c660304;
L8c660304: wr(m,r[14]+r[0],r[2],1); /* mov.b r2,@(r0,r14) */
 goto L8c660306;
L8c660306: r[5]=32u; /* mov #0x20,r5 */
 goto L8c660308;
L8c660308: r[3]=r[14]; /* mov r14,r3 */
 goto L8c66030a;
L8c66030a: wr(m,r[4],r[5],1); /* mov.b r5,@r4 */
 goto L8c66030c;
L8c66030c: r[3]+=51u; /* add #0x33,r3 */
 goto L8c66030e;
L8c66030e: r[4]+=1u; /* add #0x1,r4 */
 goto L8c660310;
L8c660310: *sr=(*sr&~1u)|(r[4]>=r[3]); /* cmp/hs r3,r4 */
 goto L8c660312;
L8c660312: if(!(*sr&1u))goto L8c660308; /* bf 0x8c660308 */
 goto L8c660314;
L8c660314: r[0]=51u; /* mov #0x33,r0 */
 goto L8c660316;
L8c660316: r[3]=0u; /* mov #0x0,r3 */
 goto L8c660318;
L8c660318: wr(m,r[14]+r[0],r[3],1); /* mov.b r3,@(r0,r14) */
 goto L8c66031a;
L8c66031a: r[0]=r[3]; /* mov r3,r0 */
 goto L8c66031c;
L8c66031c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66031e;
L8c66031e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
