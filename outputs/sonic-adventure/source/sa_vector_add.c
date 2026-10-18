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
/* Static C recovery of retail Tikal helper 8c638eac..8c638ec6.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_vector_add(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c638eac;
L8c638eac: fr[4]=rd(m,r[4],4);r[4]+=4; /* fmov.s @r4+,fr4 */
 goto L8c638eae;
L8c638eae: fr[1]=rd(m,r[5],4);r[5]+=4; /* fmov.s @r5+,fr1 */
 goto L8c638eb0;
L8c638eb0: fr[5]=rd(m,r[4],4);r[4]+=4; /* fmov.s @r4+,fr5 */
 goto L8c638eb2;
L8c638eb2: status=sa_sh4_add_sub(&fr[1],&fr[4],&f->fpscr,&fr[1],0);if(status)return status; /* fadd fr4,fr1 */
 goto L8c638eb4;
L8c638eb4: fr[2]=rd(m,r[5],4);r[5]+=4; /* fmov.s @r5+,fr2 */
 goto L8c638eb6;
L8c638eb6: fr[6]=rd(m,r[4],4); /* fmov.s @r4,fr6 */
 goto L8c638eb8;
L8c638eb8: status=sa_sh4_add_sub(&fr[2],&fr[5],&f->fpscr,&fr[2],0);if(status)return status; /* fadd fr5,fr2 */
 goto L8c638eba;
L8c638eba: fr[3]=rd(m,r[5],4); /* fmov.s @r5,fr3 */
 goto L8c638ebc;
L8c638ebc: status=sa_sh4_add_sub(&fr[3],&fr[6],&f->fpscr,&fr[3],0);if(status)return status; /* fadd fr6,fr3 */
 goto L8c638ebe;
L8c638ebe: wr(m,r[4],fr[3],4); /* fmov.s fr3,@r4 */
 goto L8c638ec0;
L8c638ec0: r[4]-=4;wr(m,r[4],fr[2],4); /* fmov.s fr2,@-r4 */
 goto L8c638ec2;
L8c638ec2: r[4]-=4;wr(m,r[4],fr[1],4);return 0; /* rts */
}
