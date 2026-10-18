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
/* Static C recovery of retail Tikal helper 8c6455ce..8c64563e.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_file_filename_lookup(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6455ce;
L8c6455ce: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6455d0;
L8c6455d0: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6455d2;
L8c6455d2: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6455d4;
L8c6455d4: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c6455d6;
L8c6455d6: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c6455d8;
L8c6455d8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6455da;
L8c6455da: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c6455dc;
L8c6455dc: r[14]=r[5]; /* mov r5,r14 */
 goto L8c6455de;
L8c6455de: r[14]+=16u; /* add #0x10,r14 */
 goto L8c6455e0;
L8c6455e0: r[12]=r[4]; /* mov r4,r12 */
 goto L8c6455e2;
L8c6455e2: wr(m,r[15],r[6],4); /* mov.l r6,@r15 */
 goto L8c6455e4;
L8c6455e4: r[11]=rd(m,r[5],4); /* mov.l @r5,r11 */
 goto L8c6455e6;
L8c6455e6: *pr=0x8c6455eau;r[4]=r[12];status=call(ctx,0x8c645442u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c645442 */
 goto L8c6455ea;
L8c6455ea: *sr=(*sr&~1u)|((int32_t)r[11]>0); /* cmp/pl r11 */
 goto L8c6455ec;
L8c6455ec: r[10]=r[0]; /* mov r0,r10 */
 goto L8c6455ee;
L8c6455ee: take=!(*sr&1u);r[13]=0u;if(take)goto L8c64562c;goto L8c6455f2; /* bf/s 0x8c64562c */
 goto L8c6455f2;
L8c6455f2: r[0]=sx(m,r[14]+0x9u,1); /* mov.b @(0x9,r14),r0 */
 goto L8c6455f4;
L8c6455f4: r[3]=r[10]&255u; /* extu.b r10,r3 */
 goto L8c6455f6;
L8c6455f6: r[0]=r[0]&255u; /* extu.b r0,r0 */
 goto L8c6455f8;
L8c6455f8: *sr=(*sr&~1u)|(r[3]==r[0]); /* cmp/eq r3,r0 */
 goto L8c6455fa;
L8c6455fa: if(!(*sr&1u))goto L8c645624; /* bf 0x8c645624 */
 goto L8c6455fc;
L8c6455fc: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c6455fe;
L8c6455fe: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c645600;
L8c645600: if(!(*sr&1u))goto L8c645612; /* bf 0x8c645612 */
 goto L8c645602;
L8c645602: r[5]=r[14]; /* mov r14,r5 */
 goto L8c645604;
L8c645604: r[5]+=10u; /* add #0xa,r5 */
 goto L8c645606;
L8c645606: *pr=0x8c64560au;r[4]=r[12];status=call(ctx,0x8c6453c0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6453c0 */
 goto L8c64560a;
L8c64560a: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c64560c;
L8c64560c: if((*sr&1u))goto L8c64561e; /* bt 0x8c64561e */
 goto L8c64560e;
L8c64560e: goto L8c645624; /* bra 0x8c645624 */
 goto L8c645612;
L8c645612: r[5]=r[14]; /* mov r14,r5 */
 goto L8c645614;
L8c645614: r[5]+=10u; /* add #0xa,r5 */
 goto L8c645616;
L8c645616: *pr=0x8c64561au;r[4]=r[12];status=call(ctx,0x8c6453e2u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6453e2 */
 goto L8c64561a;
L8c64561a: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c64561c;
L8c64561c: if(!(*sr&1u))goto L8c645624; /* bf 0x8c645624 */
 goto L8c64561e;
L8c64561e: r[0]=r[13]; /* mov r13,r0 */
 goto L8c645620;
L8c645620: r[0]+=1u;goto L8c64562e; /* bra 0x8c64562e */
 goto L8c645624;
L8c645624: r[13]+=1u; /* add #0x1,r13 */
 goto L8c645626;
L8c645626: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[11]); /* cmp/ge r11,r13 */
 goto L8c645628;
L8c645628: take=!(*sr&1u);r[14]+=44u;if(take)goto L8c6455f2;goto L8c64562c; /* bf/s 0x8c6455f2 */
 goto L8c64562c;
L8c64562c: r[0]=0u; /* mov #0x0,r0 */
 goto L8c64562e;
L8c64562e: r[15]+=4u; /* add #0x4,r15 */
 goto L8c645630;
L8c645630: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c645632;
L8c645632: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c645634;
L8c645634: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c645636;
L8c645636: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c645638;
L8c645638: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c64563a;
L8c64563a: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
