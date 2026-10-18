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
/* Static C recovery of retail Tikal helper 8c63a7e4..8c63a7fe.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_vector_subtract(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c63a7e4;
L8c63a7e4: fr[1]=rd(m,r[5],4);r[5]+=4; /* fmov.s @r5+,fr1 */
 goto L8c63a7e6;
L8c63a7e6: fr[4]=rd(m,r[4],4);r[4]+=4; /* fmov.s @r4+,fr4 */
 goto L8c63a7e8;
L8c63a7e8: fr[2]=rd(m,r[5],4);r[5]+=4; /* fmov.s @r5+,fr2 */
 goto L8c63a7ea;
L8c63a7ea: status=sa_sh4_add_sub(&fr[4],&fr[1],&f->fpscr,&fr[4],1);if(status)return status; /* fsub fr1,fr4 */
 goto L8c63a7ec;
L8c63a7ec: fr[5]=rd(m,r[4],4);r[4]+=4; /* fmov.s @r4+,fr5 */
 goto L8c63a7ee;
L8c63a7ee: fr[3]=rd(m,r[5],4); /* fmov.s @r5,fr3 */
 goto L8c63a7f0;
L8c63a7f0: status=sa_sh4_add_sub(&fr[5],&fr[2],&f->fpscr,&fr[5],1);if(status)return status; /* fsub fr2,fr5 */
 goto L8c63a7f2;
L8c63a7f2: fr[6]=rd(m,r[4],4); /* fmov.s @r4,fr6 */
 goto L8c63a7f4;
L8c63a7f4: status=sa_sh4_add_sub(&fr[6],&fr[3],&f->fpscr,&fr[6],1);if(status)return status; /* fsub fr3,fr6 */
 goto L8c63a7f6;
L8c63a7f6: wr(m,r[4],fr[6],4); /* fmov.s fr6,@r4 */
 goto L8c63a7f8;
L8c63a7f8: r[4]-=4;wr(m,r[4],fr[5],4); /* fmov.s fr5,@-r4 */
 goto L8c63a7fa;
L8c63a7fa: r[4]-=4;wr(m,r[4],fr[4],4);return 0; /* rts */
}
