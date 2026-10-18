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
/* Static C recovery of retail Tikal helper 8c09878c..8c0987ce.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_task_update_list(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c09878c;
L8c09878c: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c09878e;
L8c09878e: r[0]=r[4]; /* mov r4,r0 */
 goto L8c098790;
L8c098790: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c098792;
L8c098792: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto L8c098794;
L8c098794: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c098796;
L8c098796: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c098798;
L8c098798: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c09879a;
L8c09879a: if(!(*sr&1u))goto L8c09879e; /* bf 0x8c09879e */
 goto L8c09879c;
L8c09879c: r[4]=7u; /* mov #0x7,r4 */
 goto L8c09879e;
L8c09879e: r[0]=rd(m,0x8c0987e0u,4); /* mov.l 0x8c0987e0,r0 */
 goto L8c0987a0;
L8c0987a0: r[4]<<=2; /* shll2 r4 */
 goto L8c0987a2;
L8c0987a2: r[4]=rd(m,r[4]+r[0],4); /* mov.l @(r0,r4),r4 */
 goto L8c0987a4;
L8c0987a4: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c0987a6;
L8c0987a6: if((*sr&1u))goto L8c0987c2; /* bt 0x8c0987c2 */
 goto L8c0987a8;
L8c0987a8: r[13]=rd(m,0x8c0987ecu,4); /* mov.l 0x8c0987ec,r13 */
 goto L8c0987aa;
L8c0987aa: r[11]=0u; /* mov #0x0,r11 */
 goto L8c0987ac;
L8c0987ac: r[14]=rd(m,r[4]+0x10u,4); /* mov.l @(0x10,r4),r14 */
 goto L8c0987ae;
L8c0987ae: r[12]=rd(m,r[4],4); /* mov.l @r4,r12 */
 goto L8c0987b0;
L8c0987b0: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c0987b2;
L8c0987b2: take=(*sr&1u);wr(m,r[13],r[14],4);if(take)goto L8c0987ba;goto L8c0987b6; /* bt/s 0x8c0987ba */
 goto L8c0987b6;
L8c0987b6: target=r[14];*pr=0x8c0987bau;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c0987ba;
L8c0987ba: r[4]=r[12]; /* mov r12,r4 */
 goto L8c0987bc;
L8c0987bc: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c0987be;
L8c0987be: take=!(*sr&1u);wr(m,r[13],r[11],4);if(take)goto L8c0987ac;goto L8c0987c2; /* bf/s 0x8c0987ac */
 goto L8c0987c2;
L8c0987c2: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c0987c4;
L8c0987c4: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c0987c6;
L8c0987c6: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c0987c8;
L8c0987c8: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c0987ca;
L8c0987ca: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
