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
/* Retail indexed sound dispatch. Lower-index callee remains an external dependency. */
SA_STAGE_EXPORT int sa_sound_index_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c093b70;
L8c093b70: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c093b72;
L8c093b72: r[3]=32u; /* mov #0x20,r3 */
 goto L8c093b74;
L8c093b74: r[14]=r[4]; /* mov r4,r14 */
 goto L8c093b76;
L8c093b76: *sr=(*sr&~1u)|((int32_t)r[14]>=(int32_t)r[3]); /* cmp/ge r3,r14 */
 goto L8c093b78;
L8c093b78: if(!(*sr&1u))goto L8c093b8a; /* bf 0x8c093b8a */
 goto L8c093b7a;
L8c093b7a: r[4]=r[14]; /* mov r14,r4 */
 goto L8c093b7c;
L8c093b7c: r[0]=rd(m,0x8c093bb4u,4); /* mov.l 0x8c093bb4,r0 */
 goto L8c093b7e;
L8c093b7e: r[4]+=4294967266u; /* add #-0x1e,r4 */
 goto L8c093b80;
L8c093b80: r[2]=rd(m,0x8c093bc0u,4); /* mov.l 0x8c093bc0,r2 */
 goto L8c093b82;
L8c093b82: r[4]<<=2; /* shll2 r4 */
 goto L8c093b84;
L8c093b84: r[4]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r4 */
 goto L8c093b86;
L8c093b86: target=r[2];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r2 */
L8c093b8a: r[4]=r[14]; /* mov r14,r4 */
 goto L8c093b8c;
L8c093b8c: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c0933bau,m,f,sr,r,pr); /* bra 0x8c0933ba */
}
