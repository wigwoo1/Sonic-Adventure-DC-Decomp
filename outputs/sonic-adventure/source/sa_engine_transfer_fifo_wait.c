/* Sonic Adventure USA Rev A main executable recovered transfer routine. */
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
/* Static C recovery of retail Tikal helper 8c10ebac..8c10ebde.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_transfer_fifo_wait(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10ebac;
L8c10ebac: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10ebae;
L8c10ebae: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c10ebb0;
L8c10ebb0: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10ebb2;
L8c10ebb2: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c10ebb4;
L8c10ebb4: r[14]=rd(m,0x8c10ebe4u,4); /* mov.l 0x8c10ebe4,r14 */
 goto L8c10ebb6;
L8c10ebb6: r[13]=rd(m,0x8c10ebe0u,4); /* mov.l 0x8c10ebe0,r13 */
 goto L8c10ebb8;
L8c10ebb8: target=r[14];*pr=0x8c10ebbcu;r[4]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c10ebbc;
L8c10ebbc: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c10ebbe;
L8c10ebbe: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10ebc0;
L8c10ebc0: if(!(*sr&1u))goto L8c10ebb8; /* bf 0x8c10ebb8 */
 goto L8c10ebc2;
L8c10ebc2: r[0]=sx(m,0x8c10ebdeu,2); /* mov.w 0x8c10ebde,r0 */
 goto L8c10ebc4;
L8c10ebc4: r[2]=rd(m,r[13],4); /* mov.l @r13,r2 */
 goto L8c10ebc6;
L8c10ebc6: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c10ebc8;
L8c10ebc8: r[3]+=1u; /* add #0x1,r3 */
 goto L8c10ebca;
L8c10ebca: wr(m,r[2]+r[0],r[3],4); /* mov.l r3,@(r0,r2) */
 goto L8c10ebcc;
L8c10ebcc: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c10ebce;
L8c10ebce: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c10ebd0;
L8c10ebd0: if(!(*sr&1u))goto L8c10ebb8; /* bf 0x8c10ebb8 */
 goto L8c10ebd2;
L8c10ebd2: r[0]=0u; /* mov #0x0,r0 */
 goto L8c10ebd4;
L8c10ebd4: r[15]+=4u; /* add #0x4,r15 */
 goto L8c10ebd6;
L8c10ebd6: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10ebd8;
L8c10ebd8: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c10ebda;
L8c10ebda: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
