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
/* Static C recovery of retail Tikal helper 8c098800..8c098842.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_task_display_list(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c098800;
L8c098800: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c098802;
L8c098802: r[0]=r[4]; /* mov r4,r0 */
 goto L8c098804;
L8c098804: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c098806;
L8c098806: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto L8c098808;
L8c098808: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c09880a;
L8c09880a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c09880c;
L8c09880c: if(!(*sr&1u))goto L8c098810; /* bf 0x8c098810 */
 goto L8c09880e;
L8c09880e: r[4]=7u; /* mov #0x7,r4 */
 goto L8c098810;
L8c098810: r[0]=rd(m,0x8c098940u,4); /* mov.l 0x8c098940,r0 */
 goto L8c098812;
L8c098812: r[14]=r[4]; /* mov r4,r14 */
 goto L8c098814;
L8c098814: r[14]<<=2; /* shll2 r14 */
 goto L8c098816;
L8c098816: r[14]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r14 */
 goto L8c098818;
L8c098818: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c09881a;
L8c09881a: if((*sr&1u))goto L8c098838; /* bt 0x8c098838 */
 goto L8c09881c;
L8c09881c: r[13]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r13 */
 goto L8c09881e;
L8c09881e: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c098820;
L8c098820: take=(*sr&1u);r[12]=rd(m,r[14],4);if(take)goto L8c098828;goto L8c098824; /* bt/s 0x8c098828 */
 goto L8c098824;
L8c098824: target=r[13];*pr=0x8c098828u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c098828;
L8c098828: r[3]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r3 */
 goto L8c09882a;
L8c09882a: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c09882c;
L8c09882c: if((*sr&1u))goto L8c098832; /* bt 0x8c098832 */
 goto L8c09882e;
L8c09882e: *pr=0x8c098832u;r[4]=r[14];status=call(ctx,0x8c098842u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c098842 */
 goto L8c098832;
L8c098832: r[14]=r[12]; /* mov r12,r14 */
 goto L8c098834;
L8c098834: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c098836;
L8c098836: if(!(*sr&1u))goto L8c09881c; /* bf 0x8c09881c */
 goto L8c098838;
L8c098838: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c09883a;
L8c09883a: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c09883c;
L8c09883c: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c09883e;
L8c09883e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
