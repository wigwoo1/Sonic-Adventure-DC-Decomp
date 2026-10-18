#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_cache_invalidate(void *,uint32_t);
int sa_sh4_cache_purge(void *,uint32_t);
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
/* Static C recovery of retail Tikal helper 8c66b304..8c66b374.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_control_level_request(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c66b304;
L8c66b304: r[2]=rd(m,0x8c66b354u,4); /* mov.l 0x8c66b354,r2 */
 goto L8c66b306;
L8c66b306: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66b308;
L8c66b308: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c66b30a;
L8c66b30a: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c66b30c;
L8c66b30c: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c66b30e;
L8c66b30e: if(!(*sr&1u))goto L8c66b316; /* bf 0x8c66b316 */
 goto L8c66b310;
L8c66b310: r[14]=rd(m,0x8c66b358u,4); /* mov.l 0x8c66b358,r14 */
 goto L8c66b312;
L8c66b312: goto L8c66b33e; /* bra 0x8c66b33e */
 goto L8c66b316;
L8c66b316: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c66b318;
L8c66b318: if(!(*sr&1u))goto L8c66b320; /* bf 0x8c66b320 */
 goto L8c66b31a;
L8c66b31a: r[14]=rd(m,0x8c66b35cu,4); /* mov.l 0x8c66b35c,r14 */
 goto L8c66b31c;
L8c66b31c: goto L8c66b33e; /* bra 0x8c66b33e */
 goto L8c66b320;
L8c66b320: r[3]=sx(m,0x8c66b352u,2); /* mov.w 0x8c66b352,r3 */
 goto L8c66b322;
L8c66b322: r[5]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r5 */
 goto L8c66b324;
L8c66b324: r[1]=rd(m,0x8c66b360u,4); /* mov.l 0x8c66b360,r1 */
 goto L8c66b326;
L8c66b326: r[2]=42u; /* mov #0x2a,r2 */
 goto L8c66b328;
L8c66b328: r[5]+=r[3]; /* add r3,r5 */
 goto L8c66b32a;
L8c66b32a: r[3]=rd(m,0x8c66b364u,4); /* mov.l 0x8c66b364,r3 */
 goto L8c66b32c;
L8c66b32c: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c66b32e;
L8c66b32e: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c66b330;
L8c66b330: r[2]=rd(m,0x8c66b368u,4); /* mov.l 0x8c66b368,r2 */
 goto L8c66b332;
L8c66b332: wr(m,r[3],r[0],1); /* mov.b r0,@r3 */
 goto L8c66b334;
L8c66b334: r[3]+=1u; /* add #0x1,r3 */
 goto L8c66b336;
L8c66b336: wr(m,r[3],r[5],1); /* mov.b r5,@r3 */
 goto L8c66b338;
L8c66b338: target=r[2];*pr=0x8c66b33cu;r[4]=r[1];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66b33c;
L8c66b33c: r[14]=r[0]; /* mov r0,r14 */
 goto L8c66b33e;
L8c66b33e: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c66b340;
L8c66b340: if((*sr&1u))goto L8c66b34a; /* bt 0x8c66b34a */
 goto L8c66b342;
L8c66b342: r[2]=rd(m,0x8c66b370u,4); /* mov.l 0x8c66b370,r2 */
 goto L8c66b344;
L8c66b344: r[4]=rd(m,0x8c66b36cu,4); /* mov.l 0x8c66b36c,r4 */
 goto L8c66b346;
L8c66b346: target=r[2];*pr=0x8c66b34au;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66b34a;
L8c66b34a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66b34c;
L8c66b34c: r[0]=r[14]; /* mov r14,r0 */
 goto L8c66b34e;
L8c66b34e: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
