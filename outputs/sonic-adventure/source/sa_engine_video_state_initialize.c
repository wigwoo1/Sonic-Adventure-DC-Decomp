#include "fpu_cleanup_transfer.h"
#include "input_io.h"
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
/* Static C recovery of retail video initialization helper 8c6656e2..8c6659ac.
 * Register access and video-memory writes remain external services;
 *  Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_video_state_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,const struct sa_input_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6656e2;
L8c6656e2: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6656e4;
L8c6656e4: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6656e6;
L8c6656e6: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6656e8;
L8c6656e8: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c6656ea;
L8c6656ea: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6656ec;
L8c6656ec: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c6656ee;
L8c6656ee: r[3]=rd(m,0x8c665748u,4); /* mov.l 0x8c665748,r3 */
 goto L8c6656f0;
L8c6656f0: r[12]=0u; /* mov #0x0,r12 */
 goto L8c6656f2;
L8c6656f2: r[11]=rd(m,0x8c665744u,4); /* mov.l 0x8c665744,r11 */
 goto L8c6656f4;
L8c6656f4: wr(m,r[3],r[12],4); /* mov.l r12,@r3 */
 goto L8c6656f6;
L8c6656f6: r[2]=rd(m,0x8c66574cu,4); /* mov.l 0x8c66574c,r2 */
 goto L8c6656f8;
L8c6656f8: target=r[2];*pr=0x8c6656fcu;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6656fc;
L8c6656fc: r[3]=rd(m,0x8c66574cu,4); /* mov.l 0x8c66574c,r3 */
 goto L8c6656fe;
L8c6656fe: wr(m,r[15]+0x4u,r[0],4); /* mov.l r0,@(0x4,r15) */
 goto L8c665700;
L8c665700: target=r[3];*pr=0x8c665704u;r[4]=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c665704;
L8c665704: r[2]=rd(m,0x8c665750u,4); /* mov.l 0x8c665750,r2 */
 goto L8c665706;
L8c665706: r[4]=5u; /* mov #0x5,r4 */
 goto L8c665708;
L8c665708: r[5]=r[0]; /* mov r0,r5 */
 goto L8c66570a;
L8c66570a: wr(m,r[2],r[4],4); /* mov.l r4,@r2 */
 goto L8c66570c;
L8c66570c: r[1]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r1 */
 goto L8c66570e;
L8c66570e: r[3]=rd(m,0x8c665754u,4); /* mov.l 0x8c665754,r3 */
 goto L8c665710;
L8c665710: *sr=(*sr&~1u)|(r[3]==r[1]); /* cmp/eq r3,r1 */
 goto L8c665712;
L8c665712: if((*sr&1u))goto L8c66571a; /* bt 0x8c66571a */
 goto L8c665714;
L8c665714: r[1]=rd(m,0x8c665750u,4); /* mov.l 0x8c665750,r1 */
 goto L8c665716;
L8c665716: wr(m,r[1],r[4],4);goto L8c665774; /* bra 0x8c665774 */
 goto L8c66571a;
L8c66571a: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66571c;
L8c66571c: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c66571e;
L8c66571e: if(!(*sr&1u))goto L8c665760; /* bf 0x8c665760 */
 goto L8c665720;
L8c665720: r[0]=rd(m,0x8c665750u,4); /* mov.l 0x8c665750,r0 */
 goto L8c665722;
L8c665722: wr(m,r[0],r[4],4);goto L8c665774; /* bra 0x8c665774 */
 goto L8c665760;
L8c665760: r[1]=17u; /* mov #0x11,r1 */
 goto L8c665762;
L8c665762: *sr=(*sr&~1u)|(r[5]>=r[1]); /* cmp/hs r1,r5 */
 goto L8c665764;
L8c665764: if(!(*sr&1u))goto L8c665774; /* bf 0x8c665774 */
 goto L8c665766;
L8c665766: r[2]=rd(m,0x8c6657c4u,4); /* mov.l 0x8c6657c4,r2 */
 goto L8c665768;
L8c665768: r[0]=6u; /* mov #0x6,r0 */
 goto L8c66576a;
L8c66576a: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c66576c;
L8c66576c: r[0]=rd(m,0x8c6657ccu,4); /* mov.l 0x8c6657cc,r0 */
 goto L8c66576e;
L8c66576e: r[3]=rd(m,0x8c6657c8u,4); /* mov.l 0x8c6657c8,r3 */
 goto L8c665770;
L8c665770: wr(m,r[0],r[3],4); /* mov.l r3,@r0 */
 goto L8c665772;
L8c665772: r[11]=rd(m,0x8c6657d0u,4); /* mov.l 0x8c6657d0,r11 */
 goto L8c665774;
L8c665774: r[3]=rd(m,0x8c6657d4u,4); /* mov.l 0x8c6657d4,r3 */
 goto L8c665776;
L8c665776: r[4]=sx(m,0x8c6657c0u,2); /* mov.w 0x8c6657c0,r4 */
 goto L8c665778;
L8c665778: r[5]=io->read(io->user,r[3],4); /* mov.l @r3,r5 */
 goto L8c66577a;
L8c66577a: r[0]=r[5]; /* mov r5,r0 */
 goto L8c66577c;
L8c66577c: *sr=(*sr&~1u)|(r[0]==0x0u); /* cmp/eq #0x0,r0 */
 goto L8c66577e;
L8c66577e: take=(*sr&1u);r[6]=15u;if(take)goto L8c6657a2;goto L8c665782; /* bt/s 0x8c6657a2 */
 goto L8c665782;
L8c665782: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c665784;
L8c665784: if((*sr&1u))goto L8c6657a8; /* bt 0x8c6657a8 */
 goto L8c665786;
L8c665786: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c665788;
L8c665788: if((*sr&1u))goto L8c6657e0; /* bt 0x8c6657e0 */
 goto L8c66578a;
L8c66578a: *sr=(*sr&~1u)|(r[0]==0x4u); /* cmp/eq #0x4,r0 */
 goto L8c66578c;
L8c66578c: if((*sr&1u))goto L8c6657ea; /* bt 0x8c6657ea */
 goto L8c66578e;
L8c66578e: *sr=(*sr&~1u)|(r[0]==0x5u); /* cmp/eq #0x5,r0 */
 goto L8c665790;
L8c665790: if((*sr&1u))goto L8c6657ea; /* bt 0x8c6657ea */
 goto L8c665792;
L8c665792: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c665794;
L8c665794: if((*sr&1u))goto L8c6657ea; /* bt 0x8c6657ea */
 goto L8c665796;
L8c665796: *sr=(*sr&~1u)|(r[0]==0x8u); /* cmp/eq #0x8,r0 */
 goto L8c665798;
L8c665798: if((*sr&1u))goto L8c6657f6; /* bt 0x8c6657f6 */
 goto L8c66579a;
L8c66579a: *sr=(*sr&~1u)|(r[0]==0x9u); /* cmp/eq #0x9,r0 */
 goto L8c66579c;
L8c66579c: if((*sr&1u))goto L8c665800; /* bt 0x8c665800 */
 goto L8c66579e;
L8c66579e: goto L8c665840; /* bra 0x8c665840 */
 goto L8c6657a2;
L8c6657a2: r[2]=rd(m,0x8c6657d8u,4); /* mov.l 0x8c6657d8,r2 */
 goto L8c6657a4;
L8c6657a4: wr(m,r[2],r[12],4);goto L8c6657ae; /* bra 0x8c6657ae */
 goto L8c6657a8;
L8c6657a8: r[1]=rd(m,0x8c6657d8u,4); /* mov.l 0x8c6657d8,r1 */
 goto L8c6657aa;
L8c6657aa: r[2]=1u; /* mov #0x1,r2 */
 goto L8c6657ac;
L8c6657ac: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c6657ae;
L8c6657ae: r[13]=r[12];goto L8c665846; /* bra 0x8c665846 */
 goto L8c6657e0;
L8c6657e0: r[2]=rd(m,0x8c665820u,4); /* mov.l 0x8c665820,r2 */
 goto L8c6657e2;
L8c6657e2: r[1]=3u; /* mov #0x3,r1 */
 goto L8c6657e4;
L8c6657e4: wr(m,r[2],r[1],4); /* mov.l r1,@r2 */
 goto L8c6657e6;
L8c6657e6: goto L8c665844; /* bra 0x8c665844 */
 goto L8c6657ea;
L8c6657ea: r[1]=rd(m,0x8c665820u,4); /* mov.l 0x8c665820,r1 */
 goto L8c6657ec;
L8c6657ec: r[2]=127u; /* mov #0x7f,r2 */
 goto L8c6657ee;
L8c6657ee: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c6657f0;
L8c6657f0: r[13]=rd(m,0x8c665824u,4); /* mov.l 0x8c665824,r13 */
 goto L8c6657f2;
L8c6657f2: goto L8c665846; /* bra 0x8c665846 */
 goto L8c6657f6;
L8c6657f6: r[2]=rd(m,0x8c665820u,4); /* mov.l 0x8c665820,r2 */
 goto L8c6657f8;
L8c6657f8: r[1]=31u; /* mov #0x1f,r1 */
 goto L8c6657fa;
L8c6657fa: wr(m,r[2],r[1],4); /* mov.l r1,@r2 */
 goto L8c6657fc;
L8c6657fc: goto L8c665844; /* bra 0x8c665844 */
 goto L8c665800;
L8c665800: r[2]=rd(m,0x8c665820u,4); /* mov.l 0x8c665820,r2 */
 goto L8c665802;
L8c665802: wr(m,r[2],r[6],4);goto L8c665844; /* bra 0x8c665844 */
 goto L8c665840;
L8c665840: r[2]=rd(m,0x8c6658ccu,4); /* mov.l 0x8c6658cc,r2 */
 goto L8c665842;
L8c665842: wr(m,r[2],r[6],4); /* mov.l r6,@r2 */
 goto L8c665844;
L8c665844: r[13]=r[4]; /* mov r4,r13 */
 goto L8c665846;
L8c665846: r[4]=sx(m,0x8c6658c0u,2); /* mov.w 0x8c6658c0,r4 */
 goto L8c665848;
L8c665848: r[14]=rd(m,0x8c6658d0u,4); /* mov.l 0x8c6658d0,r14 */
 goto L8c66584a;
L8c66584a: target=r[14];*pr=0x8c66584eu;r[5]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c66584e;
L8c66584e: r[4]=sx(m,0x8c6658c2u,2); /* mov.w 0x8c6658c2,r4 */
 goto L8c665850;
L8c665850: target=r[14];*pr=0x8c665854u;r[5]=32u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665854;
L8c665854: r[5]=0u; /* mov #0x0,r5 */
 goto L8c665856;
L8c665856: target=r[14];*pr=0x8c66585au;r[4]=8u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c66585a;
L8c66585a: r[4]=sx(m,0x8c6658c4u,2); /* mov.w 0x8c6658c4,r4 */
 goto L8c66585c;
L8c66585c: target=r[14];*pr=0x8c665860u;r[5]=3u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665860;
L8c665860: r[5]=9u; /* mov #0x9,r5 */
 goto L8c665862;
L8c665862: target=r[14];*pr=0x8c665866u;r[4]=72u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665866;
L8c665866: r[3]=rd(m,0x8c6658d4u,4); /* mov.l 0x8c6658d4,r3 */
 goto L8c665868;
L8c665868: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c66586a;
L8c66586a: *sr=(*sr&~1u)|!(r[0]&0x1u); /* tst #0x1,r0 */
 goto L8c66586c;
L8c66586c: if((*sr&1u))goto L8c66587a; /* bt 0x8c66587a */
 goto L8c66586e;
L8c66586e: r[1]=rd(m,0x8c6658dcu,4); /* mov.l 0x8c6658dc,r1 */
 goto L8c665870;
L8c665870: r[5]=rd(m,0x8c6658d8u,4); /* mov.l 0x8c6658d8,r5 */
 goto L8c665872;
L8c665872: target=r[1];*pr=0x8c665876u;r[4]=68u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c665876;
L8c665876: goto L8c665882; /* bra 0x8c665882 */
 goto L8c66587a;
L8c66587a: r[2]=rd(m,0x8c6658dcu,4); /* mov.l 0x8c6658dc,r2 */
 goto L8c66587c;
L8c66587c: r[5]=0u; /* mov #0x0,r5 */
 goto L8c66587e;
L8c66587e: target=r[2];*pr=0x8c665882u;r[4]=68u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c665882;
L8c665882: r[5]=0u; /* mov #0x0,r5 */
 goto L8c665884;
L8c665884: target=r[14];*pr=0x8c665888u;r[4]=92u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665888;
L8c665888: r[5]=rd(m,0x8c6658e0u,4); /* mov.l 0x8c6658e0,r5 */
 goto L8c66588a;
L8c66588a: target=r[14];*pr=0x8c66588eu;r[4]=104u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c66588e;
L8c66588e: r[5]=rd(m,0x8c6658e4u,4); /* mov.l 0x8c6658e4,r5 */
 goto L8c665890;
L8c665890: target=r[14];*pr=0x8c665894u;r[4]=108u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665894;
L8c665894: r[3]=rd(m,0x8c6658e8u,4); /* mov.l 0x8c6658e8,r3 */
 goto L8c665896;
L8c665896: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c665898;
L8c665898: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c66589a;
L8c66589a: if(!(*sr&1u))goto L8c665900; /* bf 0x8c665900 */
 goto L8c66589c;
L8c66589c: r[4]=sx(m,0x8c6658c6u,2); /* mov.w 0x8c6658c6,r4 */
 goto L8c66589e;
L8c66589e: r[5]=rd(m,0x8c6658ecu,4); /* mov.l 0x8c6658ec,r5 */
 goto L8c6658a0;
L8c6658a0: target=r[14];*pr=0x8c6658a4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c6658a4;
L8c6658a4: r[4]=sx(m,0x8c6658c8u,2); /* mov.w 0x8c6658c8,r4 */
 goto L8c6658a6;
L8c6658a6: r[3]=rd(m,0x8c6658f0u,4); /* mov.l 0x8c6658f0,r3 */
 goto L8c6658a8;
L8c6658a8: r[5]=rd(m,r[3],4);goto L8c665904; /* bra 0x8c665904 */
 goto L8c665900;
L8c665900: r[4]=sx(m,0x8c6659c0u,2); /* mov.w 0x8c6659c0,r4 */
 goto L8c665902;
L8c665902: r[5]=rd(m,0x8c6659d4u,4); /* mov.l 0x8c6659d4,r5 */
 goto L8c665904;
L8c665904: target=r[14];*pr=0x8c665908u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665908;
L8c665908: r[5]=sx(m,0x8c6659c2u,2); /* mov.w 0x8c6659c2,r5 */
 goto L8c66590a;
L8c66590a: target=r[14];*pr=0x8c66590eu;r[4]=76u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c66590e;
L8c66590e: r[5]=0u; /* mov #0x0,r5 */
 goto L8c665910;
L8c665910: target=r[14];*pr=0x8c665914u;r[4]=116u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665914;
L8c665914: r[5]=rd(m,0x8c6659d8u,4); /* mov.l 0x8c6659d8,r5 */
 goto L8c665916;
L8c665916: target=r[14];*pr=0x8c66591au;r[4]=120u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c66591a;
L8c66591a: r[4]=sx(m,0x8c6659c4u,2); /* mov.w 0x8c6659c4,r4 */
 goto L8c66591c;
L8c66591c: target=r[14];*pr=0x8c665920u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665920;
L8c665920: r[5]=r[13]; /* mov r13,r5 */
 goto L8c665922;
L8c665922: target=r[14];*pr=0x8c665926u;r[4]=48u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665926;
L8c665926: r[4]=sx(m,0x8c6659c6u,2); /* mov.w 0x8c6659c6,r4 */
 goto L8c665928;
L8c665928: r[5]=rd(m,0x8c6659dcu,4); /* mov.l 0x8c6659dc,r5 */
 goto L8c66592a;
L8c66592a: target=r[14];*pr=0x8c66592eu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c66592e;
L8c66592e: r[4]=sx(m,0x8c6659c8u,2); /* mov.w 0x8c6659c8,r4 */
 goto L8c665930;
L8c665930: r[5]=rd(m,0x8c6659dcu,4); /* mov.l 0x8c6659dc,r5 */
 goto L8c665932;
L8c665932: target=r[14];*pr=0x8c665936u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665936;
L8c665936: r[4]=sx(m,0x8c6659cau,2); /* mov.w 0x8c6659ca,r4 */
 goto L8c665938;
L8c665938: target=r[14];*pr=0x8c66593cu;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c66593c;
L8c66593c: r[4]=sx(m,0x8c6659ccu,2); /* mov.w 0x8c6659cc,r4 */
 goto L8c66593e;
L8c66593e: target=r[14];*pr=0x8c665942u;r[5]=4294967295u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665942;
L8c665942: r[5]=7u; /* mov #0x7,r5 */
 goto L8c665944;
L8c665944: r[4]=r[5]; /* mov r5,r4 */
 goto L8c665946;
L8c665946: target=r[14];*pr=0x8c66594au;r[4]+=121u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c66594a;
L8c66594a: r[4]=sx(m,0x8c6659ceu,2); /* mov.w 0x8c6659ce,r4 */
 goto L8c66594c;
L8c66594c: r[5]=rd(m,0x8c6659e0u,4); /* mov.l 0x8c6659e0,r5 */
 goto L8c66594e;
L8c66594e: target=r[14];*pr=0x8c665952u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665952;
L8c665952: r[4]=sx(m,0x8c6659d0u,2); /* mov.w 0x8c6659d0,r4 */
 goto L8c665954;
L8c665954: r[5]=rd(m,0x8c6659e4u,4); /* mov.l 0x8c6659e4,r5 */
 goto L8c665956;
L8c665956: target=r[14];*pr=0x8c66595au;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c66595a;
L8c66595a: r[5]=1u; /* mov #0x1,r5 */
 goto L8c66595c;
L8c66595c: target=r[14];*pr=0x8c665960u;r[4]=116u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665960;
L8c665960: r[3]=rd(m,0x8c6659e8u,4); /* mov.l 0x8c6659e8,r3 */
 goto L8c665962;
L8c665962: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c665964;
L8c665964: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c665966;
L8c665966: if((*sr&1u))goto L8c66596c; /* bt 0x8c66596c */
 goto L8c665968;
L8c665968: goto L8c665968; /* bra 0x8c665968 */
 goto L8c66596c;
L8c66596c: r[2]=rd(m,0x8c6659e8u,4); /* mov.l 0x8c6659e8,r2 */
 goto L8c66596e;
L8c66596e: r[0]=rd(m,r[2],4); /* mov.l @r2,r0 */
 goto L8c665970;
L8c665970: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c665972;
L8c665972: take=!(*sr&1u);r[4]=124u;if(take)goto L8c66597c;goto L8c665976; /* bf/s 0x8c66597c */
 goto L8c665976;
L8c665976: r[5]=rd(m,0x8c6659ecu,4); /* mov.l 0x8c6659ec,r5 */
 goto L8c665978;
L8c665978: goto L8c66597e; /* bra 0x8c66597e */
 goto L8c66597c;
L8c66597c: r[5]=rd(m,0x8c6659f0u,4); /* mov.l 0x8c6659f0,r5 */
 goto L8c66597e;
L8c66597e: target=r[14];*pr=0x8c665982u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c665982;
L8c665982: r[13]=r[12]; /* mov r12,r13 */
 goto L8c665984;
L8c665984: wr(m,r[15],r[12],4); /* mov.l r12,@r15 */
 goto L8c665986;
L8c665986: r[14]=rd(m,0x8c6659fcu,4); /* mov.l 0x8c6659fc,r14 */
 goto L8c665988;
L8c665988: r[12]=rd(m,0x8c6659f8u,4); /* mov.l 0x8c6659f8,r12 */
 goto L8c66598a;
L8c66598a: r[11]=rd(m,0x8c6659f4u,4); /* mov.l 0x8c6659f4,r11 */
 goto L8c66598c;
L8c66598c: r[5]=r[15]; /* mov r15,r5 */
 goto L8c66598e;
L8c66598e: r[6]=4u; /* mov #0x4,r6 */
 goto L8c665990;
L8c665990: target=r[11];*pr=0x8c665994u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r11 */
 goto L8c665994;
L8c665994: r[13]+=1u; /* add #0x1,r13 */
 goto L8c665996;
L8c665996: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[12]); /* cmp/ge r12,r13 */
 goto L8c665998;
L8c665998: take=!(*sr&1u);r[14]+=4u;if(take)goto L8c66598c;goto L8c66599c; /* bf/s 0x8c66598c */
 goto L8c66599c;
L8c66599c: r[0]=1u; /* mov #0x1,r0 */
 goto L8c66599e;
L8c66599e: r[15]+=8u; /* add #0x8,r15 */
 goto L8c6659a0;
L8c6659a0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6659a2;
L8c6659a2: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c6659a4;
L8c6659a4: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6659a6;
L8c6659a6: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6659a8;
L8c6659a8: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
