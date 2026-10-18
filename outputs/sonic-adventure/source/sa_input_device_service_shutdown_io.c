#include "input_io.h"
#include "fpu_cleanup_transfer.h"
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(const struct sa_input_io *io,uint8_t *m,uint32_t a,unsigned n){uint32_t physical=a&0x1fffffffu;if(physical<0x0c000000u||physical>=0x0d000000u)return io->read(io->user,a,n);uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(const struct sa_input_io *io,uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(io,m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(const struct sa_input_io *io,uint8_t *m,uint32_t a,uint32_t v,unsigned n){uint32_t physical=a&0x1fffffffu;if(physical<0x0c000000u||physical>=0x0d000000u){io->write(io->user,a,v,n);return;}for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
/* Static C recovery of retail Tikal helper 8c6467f0..8c646856.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_device_service_shutdown_io(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,const struct sa_input_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6467f0;
L8c6467f0: r[15]-=4;wr(io,m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6467f2;
L8c6467f2: r[15]-=4;wr(io,m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6467f4;
L8c6467f4: r[15]-=4;wr(io,m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6467f6;
L8c6467f6: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c6467f8;
L8c6467f8: r[0]=*sr; /* stc SR,r0 */
 goto L8c6467fa;
L8c6467fa: r[3]=sx(io,m,0x8c64688cu,2); /* mov.w 0x8c64688c,r3 */
 goto L8c6467fc;
L8c6467fc: r[0]>>=2; /* shlr2 r0 */
 goto L8c6467fe;
L8c6467fe: r[0]>>=2; /* shlr2 r0 */
 goto L8c646800;
L8c646800: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c646802;
L8c646802: wr(io,m,r[15],r[0],4); /* mov.l r0,@r15 */
 goto L8c646804;
L8c646804: r[0]=*sr; /* stc SR,r0 */
 goto L8c646806;
L8c646806: r[0]&=r[3]; /* and r3,r0 */
 goto L8c646808;
L8c646808: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c64680a;
L8c64680a: *sr=r[0]; /* ldc r0,SR */
 goto L8c64680c;
L8c64680c: r[2]=sx(io,m,0x8c64688eu,2); /* mov.w 0x8c64688e,r2 */
 goto L8c64680e;
L8c64680e: r[3]=rd(io,m,0x8c6468a0u,4); /* mov.l 0x8c6468a0,r3 */
 goto L8c646810;
L8c646810: r[1]=rd(io,m,r[3],4); /* mov.l @r3,r1 */
 goto L8c646812;
L8c646812: r[1]&=r[2]; /* and r2,r1 */
 goto L8c646814;
L8c646814: wr(io,m,r[3],r[1],4); /* mov.l r1,@r3 */
 goto L8c646816;
L8c646816: r[14]=rd(io,m,0x8c6468a8u,4); /* mov.l 0x8c6468a8,r14 */
 goto L8c646818;
L8c646818: r[13]=rd(io,m,0x8c6468a4u,4); /* mov.l 0x8c6468a4,r13 */
 goto L8c64681a;
L8c64681a: target=r[13];*pr=0x8c64681eu;r[4]=rd(io,m,r[14]+0xcu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c64681e;
L8c64681e: target=r[13];*pr=0x8c646822u;r[4]=rd(io,m,r[14]+0x10u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c646822;
L8c646822: target=r[13];*pr=0x8c646826u;r[4]=rd(io,m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c646826;
L8c646826: target=r[13];*pr=0x8c64682au;r[4]=rd(io,m,r[14]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c64682a;
L8c64682a: r[3]=rd(io,m,0x8c6468acu,4); /* mov.l 0x8c6468ac,r3 */
 goto L8c64682c;
L8c64682c: target=r[3];*pr=0x8c646830u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c646830;
L8c646830: r[2]=rd(io,m,0x8c6468b4u,4); /* mov.l 0x8c6468b4,r2 */
 goto L8c646832;
L8c646832: r[4]=rd(io,m,0x8c6468b0u,4); /* mov.l 0x8c6468b0,r4 */
 goto L8c646834;
L8c646834: r[6]=sx(io,m,0x8c646890u,2); /* mov.w 0x8c646890,r6 */
 goto L8c646836;
L8c646836: target=r[2];*pr=0x8c64683au;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c64683a;
L8c64683a: r[0]=rd(io,m,r[15],4); /* mov.l @r15,r0 */
 goto L8c64683c;
L8c64683c: r[3]=*sr; /* stc SR,r3 */
 goto L8c64683e;
L8c64683e: r[2]=sx(io,m,0x8c64688cu,2); /* mov.w 0x8c64688c,r2 */
 goto L8c646840;
L8c646840: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c646842;
L8c646842: r[0]<<=2; /* shll2 r0 */
 goto L8c646844;
L8c646844: r[3]&=r[2]; /* and r2,r3 */
 goto L8c646846;
L8c646846: r[0]<<=2; /* shll2 r0 */
 goto L8c646848;
L8c646848: r[0]|=r[3]; /* or r3,r0 */
 goto L8c64684a;
L8c64684a: *sr=r[0]; /* ldc r0,SR */
 goto L8c64684c;
L8c64684c: r[15]+=4u; /* add #0x4,r15 */
 goto L8c64684e;
L8c64684e: *pr=rd(io,m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c646850;
L8c646850: r[13]=rd(io,m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c646852;
L8c646852: r[14]=rd(io,m,r[15],4);r[15]+=4;return 0; /* rts */
}
