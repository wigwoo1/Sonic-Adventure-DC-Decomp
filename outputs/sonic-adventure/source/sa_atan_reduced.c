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
/* Static C recovery of retail Tikal helper 8c10fb94..8c10fbb4.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_atan_reduced(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10fb94;
L8c10fb94: fr[6]=fr[4]; /* fmov fr4,fr6 */
 goto L8c10fb96;
L8c10fb96: status=sa_sh4_multiply(&fr[6],&fr[4],&f->fpscr,&fr[6]);if(status)return status; /* fmul fr4,fr6 */
 goto L8c10fb98;
L8c10fb98: r[5]=rd(m,0x8c10fbb4u,4); /* mov.l 0x8c10fbb4,r5 */
 goto L8c10fb9a;
L8c10fb9a: r[4]=5u; /* mov #0x5,r4 */
 goto L8c10fb9c;
L8c10fb9c: fr[5]=rd(m,r[5],4);r[5]+=4; /* fmov.s @r5+,fr5 */
 goto L8c10fb9e;
L8c10fb9e: fr[3]=rd(m,r[5],4);r[5]+=4; /* fmov.s @r5+,fr3 */
 goto L8c10fba0;
L8c10fba0: r[4]+=4294967295u; /* add #-0x1,r4 */
 goto L8c10fba2;
L8c10fba2: fr[0]=fr[6]; /* fmov fr6,fr0 */
 goto L8c10fba4;
L8c10fba4: *sr=(*sr&~1u)|((int32_t)r[4]>0); /* cmp/pl r4 */
 goto L8c10fba6;
L8c10fba6: status=sa_sh4_fmac(&fr[0],&fr[5],&fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fmac fr0,fr5,fr3 */
 goto L8c10fba8;
L8c10fba8: take=(*sr&1u);fr[5]=fr[3];if(take)goto L8c10fb9e;goto L8c10fbac; /* bt/s 0x8c10fb9e */
 goto L8c10fbac;
L8c10fbac: fr[0]=fr[4]; /* fmov fr4,fr0 */
 goto L8c10fbae;
L8c10fbae: fr[6]=fr[5]; /* fmov fr5,fr6 */
 goto L8c10fbb0;
L8c10fbb0: status=sa_sh4_multiply(&fr[0],&fr[6],&f->fpscr,&fr[0]);if(status)return status;return 0; /* rts */
}
