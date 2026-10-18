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
/* Static C recovery of retail Tikal helper 8c6698d6..8c6699d4.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_aiff_state_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6698d6;
L8c6698d6: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6698d8;
L8c6698d8: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6698da;
L8c6698da: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c6698dc;
L8c6698dc: r[14]=r[4]; /* mov r4,r14 */
 goto L8c6698de;
L8c6698de: r[3]=r[14]; /* mov r14,r3 */
 goto L8c6698e0;
L8c6698e0: r[3]+=16u; /* add #0x10,r3 */
 goto L8c6698e2;
L8c6698e2: r[2]=r[14]; /* mov r14,r2 */
 goto L8c6698e4;
L8c6698e4: wr(m,r[15]+0x4u,r[5],4); /* mov.l r5,@(0x4,r15) */
 goto L8c6698e6;
L8c6698e6: r[2]+=24u; /* add #0x18,r2 */
 goto L8c6698e8;
L8c6698e8: wr(m,r[15]+0x8u,r[6],4); /* mov.l r6,@(0x8,r15) */
 goto L8c6698ea;
L8c6698ea: r[7]=r[14]; /* mov r14,r7 */
 goto L8c6698ec;
L8c6698ec: r[0]=1u; /* mov #0x1,r0 */
 goto L8c6698ee;
L8c6698ee: wr(m,r[14]+0x2u,r[0],2); /* mov.w r0,@(0x2,r14) */
 goto L8c6698f0;
L8c6698f0: r[7]+=12u; /* add #0xc,r7 */
 goto L8c6698f2;
L8c6698f2: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c6698f4;
L8c6698f4: r[3]=r[14]; /* mov r14,r3 */
 goto L8c6698f6;
L8c6698f6: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c6698f8;
L8c6698f8: r[3]+=20u; /* add #0x14,r3 */
 goto L8c6698fa;
L8c6698fa: r[2]=r[14]; /* mov r14,r2 */
 goto L8c6698fc;
L8c6698fc: r[2]+=14u; /* add #0xe,r2 */
 goto L8c6698fe;
L8c6698fe: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c669900;
L8c669900: r[3]=r[14]; /* mov r14,r3 */
 goto L8c669902;
L8c669902: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c669904;
L8c669904: r[2]=r[14]; /* mov r14,r2 */
 goto L8c669906;
L8c669906: r[3]+=15u; /* add #0xf,r3 */
 goto L8c669908;
L8c669908: r[2]+=13u; /* add #0xd,r2 */
 goto L8c66990a;
L8c66990a: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c66990c;
L8c66990c: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c66990e;
L8c66990e: r[6]=r[15]; /* mov r15,r6 */
 goto L8c669910;
L8c669910: r[5]=rd(m,r[15]+0x20u,4); /* mov.l @(0x20,r15),r5 */
 goto L8c669912;
L8c669912: r[6]+=24u; /* add #0x18,r6 */
 goto L8c669914;
L8c669914: *pr=0x8c669918u;r[4]=rd(m,r[15]+0x1cu,4);status=call(ctx,0x8c66983eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c66983e */
 goto L8c669918;
L8c669918: r[4]=r[0]; /* mov r0,r4 */
 goto L8c66991a;
L8c66991a: *sr=(*sr&~1u)|((int32_t)r[4]>=0); /* cmp/pz r4 */
 goto L8c66991c;
L8c66991c: take=(*sr&1u);r[15]+=24u;if(take)goto L8c669938;goto L8c669920; /* bt/s 0x8c669938 */
 goto L8c669920;
L8c669920: r[0]=0u; /* mov #0x0,r0 */
 goto L8c669922;
L8c669922: r[15]+=12u; /* add #0xc,r15 */
 goto L8c669924;
L8c669924: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c669926;
L8c669926: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
L8c669938: r[4]=0u; /* mov #0x0,r4 */
 goto L8c66993a;
L8c66993a: r[0]=r[4]; /* mov r4,r0 */
 goto L8c66993c;
L8c66993c: wr(m,r[14]+0x1cu,r[0],2); /* mov.w r0,@(0x1c,r14) */
 goto L8c66993e;
L8c66993e: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c669940;
L8c669940: wr(m,r[0]+r[14],r[4],2); /* mov.w r4,@(r0,r14) */
 goto L8c669942;
L8c669942: r[0]=72u; /* mov #0x48,r0 */
 goto L8c669944;
L8c669944: wr(m,r[0]+r[14],r[4],2); /* mov.w r4,@(r0,r14) */
 goto L8c669946;
L8c669946: r[0]=38u; /* mov #0x26,r0 */
 goto L8c669948;
L8c669948: wr(m,r[0]+r[14],r[4],2); /* mov.w r4,@(r0,r14) */
 goto L8c66994a;
L8c66994a: r[0]=36u; /* mov #0x24,r0 */
 goto L8c66994c;
L8c66994c: wr(m,r[0]+r[14],r[4],2); /* mov.w r4,@(r0,r14) */
 goto L8c66994e;
L8c66994e: r[1]=88u; /* mov #0x58,r1 */
 goto L8c669950;
L8c669950: r[1]+=r[14]; /* add r14,r1 */
 goto L8c669952;
L8c669952: wr(m,r[14]+0x34u,r[4],4); /* mov.l r4,@(0x34,r14) */
 goto L8c669954;
L8c669954: wr(m,r[14]+0x30u,r[4],4); /* mov.l r4,@(0x30,r14) */
 goto L8c669956;
L8c669956: wr(m,r[14]+0x2cu,r[4],4); /* mov.l r4,@(0x2c,r14) */
 goto L8c669958;
L8c669958: wr(m,r[14]+0x28u,r[4],4); /* mov.l r4,@(0x28,r14) */
 goto L8c66995a;
L8c66995a: wr(m,r[14]+0x20u,r[4],4); /* mov.l r4,@(0x20,r14) */
 goto L8c66995c;
L8c66995c: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66995e;
L8c66995e: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c669960;
L8c669960: r[1]=92u; /* mov #0x5c,r1 */
 goto L8c669962;
L8c669962: r[0]=sx(m,r[14]+0xfu,1); /* mov.b @(0xf,r14),r0 */
 goto L8c669964;
L8c669964: r[1]+=r[14]; /* add r14,r1 */
 goto L8c669966;
L8c669966: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c669968;
L8c669968: r[0]=96u; /* mov #0x60,r0 */
 goto L8c66996a;
L8c66996a: r[3]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r3 */
 goto L8c66996c;
L8c66996c: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c66996e;
L8c66996e: r[0]=100u; /* mov #0x64,r0 */
 goto L8c669970;
L8c669970: r[2]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r2 */
 goto L8c669972;
L8c669972: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c669974;
L8c669974: r[0]=64u; /* mov #0x40,r0 */
 goto L8c669976;
L8c669976: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c669978;
L8c669978: r[0]=104u; /* mov #0x68,r0 */
 goto L8c66997a;
L8c66997a: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c66997c;
L8c66997c: r[0]=68u; /* mov #0x44,r0 */
 goto L8c66997e;
L8c66997e: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c669980;
L8c669980: r[0]=108u; /* mov #0x6c,r0 */
 goto L8c669982;
L8c669982: r[3]=r[14]; /* mov r14,r3 */
 goto L8c669984;
L8c669984: r[3]+=124u; /* add #0x7c,r3 */
 goto L8c669986;
L8c669986: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c669988;
L8c669988: r[0]=72u; /* mov #0x48,r0 */
 goto L8c66998a;
L8c66998a: r[2]=sx(m,r[0]+r[14],2); /* mov.w @(r0,r14),r2 */
 goto L8c66998c;
L8c66998c: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c66998e;
L8c66998e: wr(m,r[3],r[2],2); /* mov.w r2,@r3 */
 goto L8c669990;
L8c669990: r[2]=r[14]; /* mov r14,r2 */
 goto L8c669992;
L8c669992: r[3]=sx(m,r[0]+r[14],2); /* mov.w @(r0,r14),r3 */
 goto L8c669994;
L8c669994: r[0]=126u; /* mov #0x7e,r0 */
 goto L8c669996;
L8c669996: r[2]+=76u; /* add #0x4c,r2 */
 goto L8c669998;
L8c669998: wr(m,r[0]+r[14],r[3],2); /* mov.w r3,@(r0,r14) */
 goto L8c66999a;
L8c66999a: r[0]+=2u; /* add #0x2,r0 */
 goto L8c66999c;
L8c66999c: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c66999e;
L8c66999e: r[0]+=28u; /* add #0x1c,r0 */
 goto L8c6699a0;
L8c6699a0: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c6699a2;
L8c6699a2: r[0]+=4294967292u; /* add #-0x4,r0 */
 goto L8c6699a4;
L8c6699a4: wr(m,r[14]+r[0],r[4],4); /* mov.l r4,@(r0,r14) */
 goto L8c6699a6;
L8c6699a6: r[0]+=16u; /* add #0x10,r0 */
 goto L8c6699a8;
L8c6699a8: r[3]=3u; /* mov #0x3,r3 */
 goto L8c6699aa;
L8c6699aa: wr(m,r[0]+r[14],r[3],2); /* mov.w r3,@(r0,r14) */
 goto L8c6699ac;
L8c6699ac: r[0]=sx(m,r[14]+0xdu,1); /* mov.b @(0xd,r14),r0 */
 goto L8c6699ae;
L8c6699ae: *sr=(*sr&~1u)|(r[0]==0x00000008u); /* cmp/eq #0x8,r0 */
 goto L8c6699b0;
L8c6699b0: take=!(*sr&1u);r[6]=4u;if(take)goto L8c6699bc;goto L8c6699b4; /* bf/s 0x8c6699bc */
 goto L8c6699b4;
L8c6699b4: r[0]=sx(m,0x8c669a84u,2); /* mov.w 0x8c669a84,r0 */
 goto L8c6699b6;
L8c6699b6: r[2]=1u; /* mov #0x1,r2 */
 goto L8c6699b8;
L8c6699b8: wr(m,r[0]+r[14],r[2],2);goto L8c6699c0; /* bra 0x8c6699c0 */
 goto L8c6699bc;
L8c6699bc: r[0]=sx(m,0x8c669a84u,2); /* mov.w 0x8c669a84,r0 */
 goto L8c6699be;
L8c6699be: wr(m,r[0]+r[14],r[4],2); /* mov.w r4,@(r0,r14) */
 goto L8c6699c0;
L8c6699c0: r[3]=rd(m,0x8c669a88u,4); /* mov.l 0x8c669a88,r3 */
 goto L8c6699c2;
L8c6699c2: r[4]=r[14]; /* mov r14,r4 */
 goto L8c6699c4;
L8c6699c4: r[5]=0u; /* mov #0x0,r5 */
 goto L8c6699c6;
L8c6699c6: target=r[3];*pr=0x8c6699cau;r[4]+=76u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6699ca;
L8c6699ca: r[0]=sx(m,r[15],2); /* mov.w @r15,r0 */
 goto L8c6699cc;
L8c6699cc: r[15]+=12u; /* add #0xc,r15 */
 goto L8c6699ce;
L8c6699ce: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6699d0;
L8c6699d0: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
