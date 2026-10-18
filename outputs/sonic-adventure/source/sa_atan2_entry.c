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
/* Static C recovery of retail Tikal helper 8c10d0f8..8c10d132.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_atan2_entry(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d0f8;
L8c10d0f8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10d0fa;
L8c10d0fa: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c10d0fc;
L8c10d0fc: r[3]=rd(m,0x8c10d138u,4); /* mov.l 0x8c10d138,r3 */
 goto L8c10d0fe;
L8c10d0fe: target=r[3];*pr=0x8c10d102u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10d102;
L8c10d102: r[3]=r[15]; /* mov r15,r3 */
 goto L8c10d104;
L8c10d104: r[4]=rd(m,0x8c10d13cu,4); /* mov.l 0x8c10d13c,r4 */
 goto L8c10d106;
L8c10d106: wr(m,r[15],fr[0],4); /* fmov.s fr0,@r15 */
 goto L8c10d108;
L8c10d108: r[2]=rd(m,r[3],4); /* mov.l @r3,r2 */
 goto L8c10d10a;
L8c10d10a: r[2]&=r[4]; /* and r4,r2 */
 goto L8c10d10c;
L8c10d10c: *sr=(*sr&~1u)|(r[4]==r[2]); /* cmp/eq r4,r2 */
 goto L8c10d10e;
L8c10d10e: if(!(*sr&1u))goto L8c10d128; /* bf 0x8c10d128 */
 goto L8c10d110;
L8c10d110: r[2]=r[15]; /* mov r15,r2 */
 goto L8c10d112;
L8c10d112: r[1]=rd(m,r[2],4); /* mov.l @r2,r1 */
 goto L8c10d114;
L8c10d114: r[3]=rd(m,0x8c10d140u,4); /* mov.l 0x8c10d140,r3 */
 goto L8c10d116;
L8c10d116: *sr=(*sr&~1u)|!(r[3]&r[1]); /* tst r3,r1 */
 goto L8c10d118;
L8c10d118: if((*sr&1u))goto L8c10d122; /* bt 0x8c10d122 */
 goto L8c10d11a;
L8c10d11a: r[2]=rd(m,0x8c10d144u,4); /* mov.l 0x8c10d144,r2 */
 goto L8c10d11c;
L8c10d11c: r[0]=sx(m,0x8c10d132u,2); /* mov.w 0x8c10d132,r0 */
 goto L8c10d11e;
L8c10d11e: wr(m,r[2],r[0],4);goto L8c10d128; /* bra 0x8c10d128 */
 goto L8c10d122;
L8c10d122: r[1]=rd(m,0x8c10d144u,4); /* mov.l 0x8c10d144,r1 */
 goto L8c10d124;
L8c10d124: r[2]=sx(m,0x8c10d134u,2); /* mov.w 0x8c10d134,r2 */
 goto L8c10d126;
L8c10d126: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c10d128;
L8c10d128: fr[0]=rd(m,r[15],4); /* fmov.s @r15,fr0 */
 goto L8c10d12a;
L8c10d12a: r[15]+=4u; /* add #0x4,r15 */
 goto L8c10d12c;
L8c10d12c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10d12e;
L8c10d12e: return 0; /* rts */
}
