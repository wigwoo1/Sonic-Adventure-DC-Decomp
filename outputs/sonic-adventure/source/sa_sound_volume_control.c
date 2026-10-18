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
/* Retail indexed MIDI controller-7 volume wrapper. */
SA_STAGE_EXPORT int sa_sound_volume_control(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c093240;
L8c093240: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c093242;
L8c093242: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c093244;
L8c093244: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c093246;
L8c093246: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c093248;
L8c093248: r[14]=r[4]; /* mov r4,r14 */
 goto L8c09324a;
L8c09324a: r[4]=15u; /* mov #0xf,r4 */
 goto L8c09324c;
L8c09324c: *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[4]); /* cmp/gt r4,r14 */
 goto L8c09324e;
L8c09324e: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c093250;
L8c093250: take=!(*sr&1u);r[13]=0u;if(take)goto L8c093256;goto L8c093254; /* bf/s 0x8c093256 */
 goto L8c093254;
L8c093254: r[13]=1u; /* mov #0x1,r13 */
 goto L8c093256;
L8c093256: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c093258;
L8c093258: r[14]&=r[4]; /* and r4,r14 */
 goto L8c09325a;
L8c09325a: r[4]=r[13]; /* mov r13,r4 */
 goto L8c09325c;
L8c09325c: r[3]=7u; /* mov #0x7,r3 */
 goto L8c09325e;
L8c09325e: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c093260;
L8c093260: r[2]=4294967216u; /* mov #-0x50,r2 */
 goto L8c093262;
L8c093262: r[2]=r[2]&255u; /* extu.b r2,r2 */
 goto L8c093264;
L8c093264: r[1]=rd(m,0x8c093348u,4); /* mov.l 0x8c093348,r1 */
 goto L8c093266;
L8c093266: r[4]<<=2; /* shll2 r4 */
 goto L8c093268;
L8c093268: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c09326a;
L8c09326a: r[14]|=r[2]; /* or r2,r14 */
 goto L8c09326c;
L8c09326c: r[3]=rd(m,0x8c093340u,4); /* mov.l 0x8c093340,r3 */
 goto L8c09326e;
L8c09326e: r[4]+=r[1]; /* add r1,r4 */
 goto L8c093270;
L8c093270: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c093272;
L8c093272: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c093274;
L8c093274: wr(m,r[15]+0x10u,r[4],4); /* mov.l r4,@(0x10,r15) */
 goto L8c093276;
L8c093276: target=r[3];*pr=0x8c09327au;r[4]=rd(m,r[4],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c09327a;
L8c09327a: r[4]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r4 */
 goto L8c09327c;
L8c09327c: r[6]=0u; /* mov #0x0,r6 */
 goto L8c09327e;
L8c09327e: r[5]=rd(m,0x8c093344u,4); /* mov.l 0x8c093344,r5 */
 goto L8c093280;
L8c093280: r[4]=rd(m,r[4],4); /* mov.l @r4,r4 */
 goto L8c093282;
L8c093282: r[15]+=20u; /* add #0x14,r15 */
 goto L8c093284;
L8c093284: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c093286;
L8c093286: r[3]=rd(m,0x8c09334cu,4); /* mov.l 0x8c09334c,r3 */
 goto L8c093288;
L8c093288: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c09328a;
L8c09328a: target=r[3];r[14]=rd(m,r[15],4);r[15]+=4;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u];return 0; /* jmp @r3 */
}
