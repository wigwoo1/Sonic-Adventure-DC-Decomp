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
int sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);
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
/* Static C recovery of UNVERIFIED retail audio stream helper 8c667ad4..8c667b16.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_stream_update_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c667ad4;
L8c667ad4: r[0]=sx(m,0x8c667b48u,2); /* mov.w 0x8c667b48,r0 */
 goto L8c667ad6;
L8c667ad6: r[2]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r2 */
 goto L8c667ad8;
L8c667ad8: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c667ada;
L8c667ada: if((*sr&1u))return call(ctx,0x8c667a5cu,m,f,sr,r,pr); /* bt 0x8c667a5c */
 goto L8c667adc;
L8c667adc: r[0]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r0 */
 goto L8c667ade;
L8c667ade: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c667ae0;
L8c667ae0: if(!(*sr&1u))goto L8c667ae8; /* bf 0x8c667ae8 */
 goto L8c667ae2;
L8c667ae2: r[3]=rd(m,0x8c667b5cu,4); /* mov.l 0x8c667b5c,r3 */
 goto L8c667ae4;
L8c667ae4: target=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
L8c667ae8: r[0]=sx(m,0x8c667b48u,2); /* mov.w 0x8c667b48,r0 */
 goto L8c667aea;
L8c667aea: r[0]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r0 */
 goto L8c667aec;
L8c667aec: *sr=(*sr&~1u)|(r[0]==0x00000003u); /* cmp/eq #0x3,r0 */
 goto L8c667aee;
L8c667aee: if(!(*sr&1u))goto L8c667af6; /* bf 0x8c667af6 */
 goto L8c667af0;
L8c667af0: r[3]=rd(m,0x8c667b60u,4); /* mov.l 0x8c667b60,r3 */
 goto L8c667af2;
L8c667af2: target=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
L8c667af6: r[0]=sx(m,0x8c667b48u,2); /* mov.w 0x8c667b48,r0 */
 goto L8c667af8;
L8c667af8: r[0]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r0 */
 goto L8c667afa;
L8c667afa: *sr=(*sr&~1u)|(r[0]==0x00000004u); /* cmp/eq #0x4,r0 */
 goto L8c667afc;
L8c667afc: if(!(*sr&1u))goto L8c667b04; /* bf 0x8c667b04 */
 goto L8c667afe;
L8c667afe: r[3]=rd(m,0x8c667b64u,4); /* mov.l 0x8c667b64,r3 */
 goto L8c667b00;
L8c667b00: target=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
L8c667b04: r[0]=sx(m,0x8c667b48u,2); /* mov.w 0x8c667b48,r0 */
 goto L8c667b06;
L8c667b06: r[0]=sx(m,r[0]+r[4],2); /* mov.w @(r0,r4),r0 */
 goto L8c667b08;
L8c667b08: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c667b0a;
L8c667b0a: if(!(*sr&1u))goto L8c667b12; /* bf 0x8c667b12 */
 goto L8c667b0c;
L8c667b0c: r[3]=rd(m,0x8c667b68u,4); /* mov.l 0x8c667b68,r3 */
 goto L8c667b0e;
L8c667b0e: target=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
L8c667b12: return 0; /* rts */
}
