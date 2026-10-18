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
/* Static C recovery of retail engine memory-layout initializer 8c6666c0..8c666706.
 * Register configuration653444 and service650aa0 remain external;
 *  Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_memory_layout_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6666c0;
L8c6666c0: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6666c2;
L8c6666c2: r[3]=rd(m,0x8c666880u,4); /* mov.l 0x8c666880,r3 */
 goto L8c6666c4;
L8c6666c4: r[5]=1u; /* mov #0x1,r5 */
 goto L8c6666c6;
L8c6666c6: target=r[3];*pr=0x8c6666cau;r[4]=8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6666ca;
L8c6666ca: r[2]=rd(m,0x8c666880u,4); /* mov.l 0x8c666880,r2 */
 goto L8c6666cc;
L8c6666cc: r[5]=0u; /* mov #0x0,r5 */
 goto L8c6666ce;
L8c6666ce: target=r[2];*pr=0x8c6666d2u;r[4]=8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6666d2;
L8c6666d2: r[3]=rd(m,0x8c666884u,4); /* mov.l 0x8c666884,r3 */
 goto L8c6666d4;
L8c6666d4: target=r[3];*pr=0x8c6666d8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6666d8;
L8c6666d8: r[3]=rd(m,0x8c66688cu,4); /* mov.l 0x8c66688c,r3 */
 goto L8c6666da;
L8c6666da: r[2]=rd(m,0x8c666888u,4); /* mov.l 0x8c666888,r2 */
 goto L8c6666dc;
L8c6666dc: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c6666de;
L8c6666de: r[0]=rd(m,0x8c666894u,4); /* mov.l 0x8c666894,r0 */
 goto L8c6666e0;
L8c6666e0: r[1]=rd(m,0x8c666890u,4); /* mov.l 0x8c666890,r1 */
 goto L8c6666e2;
L8c6666e2: wr(m,r[0],r[1],4); /* mov.l r1,@r0 */
 goto L8c6666e4;
L8c6666e4: r[1]=r[0]; /* mov r0,r1 */
 goto L8c6666e6;
L8c6666e6: r[2]=rd(m,r[1],4); /* mov.l @r1,r2 */
 goto L8c6666e8;
L8c6666e8: r[3]+=12u; /* add #0xc,r3 */
 goto L8c6666ea;
L8c6666ea: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c6666ec;
L8c6666ec: r[2]=rd(m,0x8c66688cu,4); /* mov.l 0x8c66688c,r2 */
 goto L8c6666ee;
L8c6666ee: r[0]+=28u; /* add #0x1c,r0 */
 goto L8c6666f0;
L8c6666f0: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c6666f2;
L8c6666f2: *sr=(*sr&~1u)|(r[3]&1u);r[3]>>=1; /* shlr r3 */
 goto L8c6666f4;
L8c6666f4: wr(m,r[0],r[3],4); /* mov.l r3,@r0 */
 goto L8c6666f6;
L8c6666f6: r[3]=0u; /* mov #0x0,r3 */
 goto L8c6666f8;
L8c6666f8: r[1]+=60u; /* add #0x3c,r1 */
 goto L8c6666fa;
L8c6666fa: wr(m,r[1],r[3],4); /* mov.l r3,@r1 */
 goto L8c6666fc;
L8c6666fc: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6666fe;
L8c6666fe: r[3]=rd(m,0x8c666898u,4); /* mov.l 0x8c666898,r3 */
 goto L8c666700;
L8c666700: r[2]=1u; /* mov #0x1,r2 */
 goto L8c666702;
L8c666702: wr(m,r[3],r[2],4);return 0; /* rts */
}
