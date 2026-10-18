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
/* Static C recovery of retail Tikal helper 8c6461f0..8c646294.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_input_service_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6461f0;
L8c6461f0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6461f2;
L8c6461f2: r[7]=0u; /* mov #0x0,r7 */
 goto L8c6461f4;
L8c6461f4: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6461f6;
L8c6461f6: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6461f8;
L8c6461f8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6461fa;
L8c6461fa: r[5]=rd(m,0x8c6462c4u,4); /* mov.l 0x8c6462c4,r5 */
 goto L8c6461fc;
L8c6461fc: r[4]=sx(m,0x8c646298u,2); /* mov.w 0x8c646298,r4 */
 goto L8c6461fe;
L8c6461fe: r[14]=rd(m,0x8c6462c0u,4); /* mov.l 0x8c6462c0,r14 */
 goto L8c646200;
L8c646200: r[12]=rd(m,0x8c6462b8u,4); /* mov.l 0x8c6462b8,r12 */
 goto L8c646202;
L8c646202: r[13]=rd(m,0x8c6462bcu,4); /* mov.l 0x8c6462bc,r13 */
 goto L8c646204;
L8c646204: target=r[13];*pr=0x8c646208u;r[6]=rd(m,r[12],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c646208;
L8c646208: r[5]=rd(m,0x8c6462c8u,4); /* mov.l 0x8c6462c8,r5 */
 goto L8c64620a;
L8c64620a: r[7]=0u; /* mov #0x0,r7 */
 goto L8c64620c;
L8c64620c: r[4]=sx(m,0x8c64629au,2); /* mov.w 0x8c64629a,r4 */
 goto L8c64620e;
L8c64620e: wr(m,r[14],r[0],4); /* mov.l r0,@r14 */
 goto L8c646210;
L8c646210: target=r[13];*pr=0x8c646214u;r[6]=rd(m,r[12]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c646214;
L8c646214: r[5]=rd(m,0x8c6462ccu,4); /* mov.l 0x8c6462cc,r5 */
 goto L8c646216;
L8c646216: r[7]=0u; /* mov #0x0,r7 */
 goto L8c646218;
L8c646218: r[4]=sx(m,0x8c64629au,2); /* mov.w 0x8c64629a,r4 */
 goto L8c64621a;
L8c64621a: wr(m,r[14]+0x4u,r[0],4); /* mov.l r0,@(0x4,r14) */
 goto L8c64621c;
L8c64621c: target=r[13];*pr=0x8c646220u;r[6]=rd(m,r[12]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c646220;
L8c646220: r[5]=rd(m,0x8c6462d0u,4); /* mov.l 0x8c6462d0,r5 */
 goto L8c646222;
L8c646222: r[7]=0u; /* mov #0x0,r7 */
 goto L8c646224;
L8c646224: r[4]=sx(m,0x8c64629cu,2); /* mov.w 0x8c64629c,r4 */
 goto L8c646226;
L8c646226: wr(m,r[14]+0x8u,r[0],4); /* mov.l r0,@(0x8,r14) */
 goto L8c646228;
L8c646228: target=r[13];*pr=0x8c64622cu;r[6]=rd(m,r[12]+0xcu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r13 */
 goto L8c64622c;
L8c64622c: r[2]=rd(m,0x8c6462d4u,4); /* mov.l 0x8c6462d4,r2 */
 goto L8c64622e;
L8c64622e: r[3]=0u; /* mov #0x0,r3 */
 goto L8c646230;
L8c646230: wr(m,r[14]+0xcu,r[0],4); /* mov.l r0,@(0xc,r14) */
 goto L8c646232;
L8c646232: r[1]=r[3]; /* mov r3,r1 */
 goto L8c646234;
L8c646234: r[0]=rd(m,0x8c6462acu,4); /* mov.l 0x8c6462ac,r0 */
 goto L8c646236;
L8c646236: r[6]=56u; /* mov #0x38,r6 */
 goto L8c646238;
L8c646238: r[1]=rd(m,0x8c6462d8u,4); /* mov.l 0x8c6462d8,r1 */
 goto L8c64623a;
L8c64623a: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c64623c;
L8c64623c: r[2]=r[3]; /* mov r3,r2 */
 goto L8c64623e;
L8c64623e: wr(m,r[0],r[3],4); /* mov.l r3,@r0 */
 goto L8c646240;
L8c646240: r[0]=0u; /* mov #0x0,r0 */
 goto L8c646242;
L8c646242: wr(m,r[1],r[3],4); /* mov.l r3,@r1 */
 goto L8c646244;
L8c646244: r[1]=r[0]; /* mov r0,r1 */
 goto L8c646246;
L8c646246: r[3]=rd(m,0x8c6462dcu,4); /* mov.l 0x8c6462dc,r3 */
 goto L8c646248;
L8c646248: r[14]=rd(m,0x8c6462f8u,4); /* mov.l 0x8c6462f8,r14 */
 goto L8c64624a;
L8c64624a: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c64624c;
L8c64624c: r[3]=4294967295u; /* mov #-0x1,r3 */
 goto L8c64624e;
L8c64624e: r[2]=rd(m,0x8c6462e0u,4); /* mov.l 0x8c6462e0,r2 */
 goto L8c646250;
L8c646250: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c646252;
L8c646252: r[2]=0u; /* mov #0x0,r2 */
 goto L8c646254;
L8c646254: r[0]=rd(m,0x8c6462e4u,4); /* mov.l 0x8c6462e4,r0 */
 goto L8c646256;
L8c646256: wr(m,r[0],r[1],4); /* mov.l r1,@r0 */
 goto L8c646258;
L8c646258: r[0]=r[2]; /* mov r2,r0 */
 goto L8c64625a;
L8c64625a: r[1]=rd(m,0x8c6462e8u,4); /* mov.l 0x8c6462e8,r1 */
 goto L8c64625c;
L8c64625c: wr(m,r[1],r[3],4); /* mov.l r3,@r1 */
 goto L8c64625e;
L8c64625e: r[1]=0u; /* mov #0x0,r1 */
 goto L8c646260;
L8c646260: r[3]=rd(m,0x8c6462ecu,4); /* mov.l 0x8c6462ec,r3 */
 goto L8c646262;
L8c646262: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c646264;
L8c646264: r[2]=rd(m,0x8c6462f0u,4); /* mov.l 0x8c6462f0,r2 */
 goto L8c646266;
L8c646266: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c646268;
L8c646268: r[0]=rd(m,0x8c6462f4u,4); /* mov.l 0x8c6462f4,r0 */
 goto L8c64626a;
L8c64626a: wr(m,r[0],r[1],4); /* mov.l r1,@r0 */
 goto L8c64626c;
L8c64626c: r[4]=rd(m,0x8c6462a8u,4); /* mov.l 0x8c6462a8,r4 */
 goto L8c64626e;
L8c64626e: target=r[14];*pr=0x8c646272u;r[5]=r[1];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c646272;
L8c646272: r[4]=rd(m,0x8c6462b0u,4); /* mov.l 0x8c6462b0,r4 */
 goto L8c646274;
L8c646274: r[6]=sx(m,0x8c64629eu,2); /* mov.w 0x8c64629e,r6 */
 goto L8c646276;
L8c646276: target=r[14];*pr=0x8c64627au;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c64627a;
L8c64627a: r[4]=rd(m,0x8c6462fcu,4); /* mov.l 0x8c6462fc,r4 */
 goto L8c64627c;
L8c64627c: r[6]=sx(m,0x8c64629eu,2); /* mov.w 0x8c64629e,r6 */
 goto L8c64627e;
L8c64627e: target=r[14];*pr=0x8c646282u;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c646282;
L8c646282: r[4]=rd(m,0x8c646300u,4); /* mov.l 0x8c646300,r4 */
 goto L8c646284;
L8c646284: r[6]=sx(m,0x8c6462a0u,2); /* mov.w 0x8c6462a0,r6 */
 goto L8c646286;
L8c646286: target=r[14];*pr=0x8c64628au;r[5]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r14 */
 goto L8c64628a;
L8c64628a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c64628c;
L8c64628c: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c64628e;
L8c64628e: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c646290;
L8c646290: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
