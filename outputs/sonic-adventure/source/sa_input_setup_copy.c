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
/* Static C recovery of retail Tikal helper 8c10fb5c..8c10fb94.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_setup_copy(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10fb5c;
L8c10fb5c: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c10fb5e;
L8c10fb5e: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c10fb60;
L8c10fb60: r[15]-=4;wr(m,r[15],r[4],4); /* mov.l r4,@-r15 */
 goto L8c10fb62;
L8c10fb62: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c10fb64;
L8c10fb64: if((*sr&1u))goto L8c10fb8c; /* bt 0x8c10fb8c */
 goto L8c10fb66;
L8c10fb66: r[4]=r[2]; /* mov r2,r4 */
 goto L8c10fb68;
L8c10fb68: r[4]+=r[0]; /* add r0,r4 */
 goto L8c10fb6a;
L8c10fb6a: r[0]=sx(m,r[2],1);r[2]+=1; /* mov.b @r2+,r0 */
 goto L8c10fb6c;
L8c10fb6c: wr(m,r[1],r[0],1); /* mov.b r0,@r1 */
 goto L8c10fb6e;
L8c10fb6e: *sr=(*sr&~1u)|(r[4]>r[2]); /* cmp/hi r2,r4 */
 goto L8c10fb70;
L8c10fb70: if(!(*sr&1u))goto L8c10fb8c; /* bf 0x8c10fb8c */
 goto L8c10fb72;
L8c10fb72: r[0]=sx(m,r[2],1);r[2]+=1; /* mov.b @r2+,r0 */
 goto L8c10fb74;
L8c10fb74: wr(m,r[1]+0x1u,r[0],1); /* mov.b r0,@(0x1,r1) */
 goto L8c10fb76;
L8c10fb76: *sr=(*sr&~1u)|(r[4]>r[2]); /* cmp/hi r2,r4 */
 goto L8c10fb78;
L8c10fb78: if(!(*sr&1u))goto L8c10fb8c; /* bf 0x8c10fb8c */
 goto L8c10fb7a;
L8c10fb7a: r[0]=sx(m,r[2],1);r[2]+=1; /* mov.b @r2+,r0 */
 goto L8c10fb7c;
L8c10fb7c: wr(m,r[1]+0x2u,r[0],1); /* mov.b r0,@(0x2,r1) */
 goto L8c10fb7e;
L8c10fb7e: *sr=(*sr&~1u)|(r[4]>r[2]); /* cmp/hi r2,r4 */
 goto L8c10fb80;
L8c10fb80: if(!(*sr&1u))goto L8c10fb8c; /* bf 0x8c10fb8c */
 goto L8c10fb82;
L8c10fb82: r[0]=sx(m,r[2],1);r[2]+=1; /* mov.b @r2+,r0 */
 goto L8c10fb84;
L8c10fb84: wr(m,r[1]+0x3u,r[0],1); /* mov.b r0,@(0x3,r1) */
 goto L8c10fb86;
L8c10fb86: *sr=(*sr&~1u)|(r[4]>r[2]); /* cmp/hi r2,r4 */
 goto L8c10fb88;
L8c10fb88: r[1]+=4u; /* add #0x4,r1 */
 goto L8c10fb8a;
L8c10fb8a: if((*sr&1u))goto L8c10fb6a; /* bt 0x8c10fb6a */
 goto L8c10fb8c;
L8c10fb8c: r[4]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r4 */
 goto L8c10fb8e;
L8c10fb8e: r[3]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r3 */
 goto L8c10fb90;
L8c10fb90: r[2]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
