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
/* Static C recovery of retail Tikal helper 8c643754..8c6437c6.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_global_volume_request(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c643754;
L8c643754: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c643756;
L8c643756: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c643758;
L8c643758: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c64375a;
L8c64375a: r[3]=rd(m,0x8c6437ccu,4); /* mov.l 0x8c6437cc,r3 */
 goto L8c64375c;
L8c64375c: r[7]=rd(m,r[3],4); /* mov.l @r3,r7 */
 goto L8c64375e;
L8c64375e: *sr=(*sr&~1u)|!(r[7]&r[7]); /* tst r7,r7 */
 goto L8c643760;
L8c643760: if(!(*sr&1u))goto L8c643768; /* bf 0x8c643768 */
 goto L8c643762;
L8c643762: r[14]=rd(m,0x8c6437d0u,4); /* mov.l 0x8c6437d0,r14 */
 goto L8c643764;
L8c643764: goto L8c6437b0; /* bra 0x8c6437b0 */
 goto L8c643768;
L8c643768: r[2]=(uint32_t)(int32_t)(int8_t)r[4]; /* exts.b r4,r2 */
 goto L8c64376a;
L8c64376a: *sr=(*sr&~1u)|((int32_t)r[2]>=0); /* cmp/pz r2 */
 goto L8c64376c;
L8c64376c: take=!(*sr&1u);wr(m,r[15],r[2],4);if(take)goto L8c643776;goto L8c643770; /* bf/s 0x8c643776 */
 goto L8c643770;
L8c643770: r[1]=15u; /* mov #0xf,r1 */
 goto L8c643772;
L8c643772: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[1]); /* cmp/gt r1,r2 */
 goto L8c643774;
L8c643774: if(!(*sr&1u))goto L8c64377c; /* bf 0x8c64377c */
 goto L8c643776;
L8c643776: r[14]=rd(m,0x8c6437d4u,4); /* mov.l 0x8c6437d4,r14 */
 goto L8c643778;
L8c643778: goto L8c6437b0; /* bra 0x8c6437b0 */
 goto L8c64377c;
L8c64377c: r[0]=sx(m,0x8c6437c6u,2); /* mov.w 0x8c6437c6,r0 */
 goto L8c64377e;
L8c64377e: r[2]=39u; /* mov #0x27,r2 */
 goto L8c643780;
L8c643780: r[6]=(uint32_t)(int32_t)(int8_t)r[6]; /* exts.b r6,r6 */
 goto L8c643782;
L8c643782: r[3]=sx(m,r[7]+r[0],1); /* mov.b @(r0,r7),r3 */
 goto L8c643784;
L8c643784: *sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[2]); /* cmp/ge r2,r3 */
 goto L8c643786;
L8c643786: take=(*sr&1u);r[5]=(uint32_t)(int32_t)(int8_t)r[5];if(take)goto L8c643790;goto L8c64378a; /* bt/s 0x8c643790 */
 goto L8c64378a;
L8c64378a: *sr=(*sr&~1u)|(r[6]&1u);r[6]=(r[6]>>1)|(r[6]&0x80000000u); /* shar r6 */
 goto L8c64378c;
L8c64378c: *sr=(*sr&~1u)|(r[6]&1u);r[6]=(r[6]>>1)|(r[6]&0x80000000u); /* shar r6 */
 goto L8c64378e;
L8c64378e: *sr=(*sr&~1u)|(r[6]&1u);r[6]=(r[6]>>1)|(r[6]&0x80000000u); /* shar r6 */
 goto L8c643790;
L8c643790: r[1]=rd(m,0x8c6437dcu,4); /* mov.l 0x8c6437dc,r1 */
 goto L8c643792;
L8c643792: r[3]=sx(m,0x8c6437c8u,2); /* mov.w 0x8c6437c8,r3 */
 goto L8c643794;
L8c643794: r[2]=rd(m,0x8c6437d8u,4); /* mov.l 0x8c6437d8,r2 */
 goto L8c643796;
L8c643796: wr(m,r[2],r[3],4); /* mov.l r3,@r2 */
 goto L8c643798;
L8c643798: r[2]+=6u; /* add #0x6,r2 */
 goto L8c64379a;
L8c64379a: wr(m,r[1],r[4],1); /* mov.b r4,@r1 */
 goto L8c64379c;
L8c64379c: r[4]=sx(m,0x8c6437cau,2); /* mov.w 0x8c6437ca,r4 */
 goto L8c64379e;
L8c64379e: r[3]=rd(m,0x8c6437e0u,4); /* mov.l 0x8c6437e0,r3 */
 goto L8c6437a0;
L8c6437a0: r[5]+=r[4]; /* add r4,r5 */
 goto L8c6437a2;
L8c6437a2: r[1]=rd(m,0x8c6437e4u,4); /* mov.l 0x8c6437e4,r1 */
 goto L8c6437a4;
L8c6437a4: r[6]+=r[4]; /* add r4,r6 */
 goto L8c6437a6;
L8c6437a6: r[4]=rd(m,0x8c6437d8u,4); /* mov.l 0x8c6437d8,r4 */
 goto L8c6437a8;
L8c6437a8: wr(m,r[3],r[6],1); /* mov.b r6,@r3 */
 goto L8c6437aa;
L8c6437aa: target=r[1];*pr=0x8c6437aeu;wr(m,r[2],r[5],1);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c6437ae;
L8c6437ae: r[14]=r[0]; /* mov r0,r14 */
 goto L8c6437b0;
L8c6437b0: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c6437b2;
L8c6437b2: if((*sr&1u))goto L8c6437bc; /* bt 0x8c6437bc */
 goto L8c6437b4;
L8c6437b4: r[2]=rd(m,0x8c6437ecu,4); /* mov.l 0x8c6437ec,r2 */
 goto L8c6437b6;
L8c6437b6: r[4]=rd(m,0x8c6437e8u,4); /* mov.l 0x8c6437e8,r4 */
 goto L8c6437b8;
L8c6437b8: target=r[2];*pr=0x8c6437bcu;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6437bc;
L8c6437bc: r[0]=r[14]; /* mov r14,r0 */
 goto L8c6437be;
L8c6437be: r[15]+=4u; /* add #0x4,r15 */
 goto L8c6437c0;
L8c6437c0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6437c2;
L8c6437c2: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
