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
/* Static C recovery of retail Tikal helper 8c093e60..8c093f02.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_load_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c093e60;
L8c093e60: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c093e62;
L8c093e62: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c093e64;
L8c093e64: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c093e66;
L8c093e66: r[3]=0u; /* mov #0x0,r3 */
 goto L8c093e68;
L8c093e68: r[4]=rd(m,0x8c094040u,4); /* mov.l 0x8c094040,r4 */
 goto L8c093e6a;
L8c093e6a: r[5]=r[3]; /* mov r3,r5 */
 goto L8c093e6c;
L8c093e6c: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c093e6e;
L8c093e6e: r[3]=rd(m,0x8c094044u,4); /* mov.l 0x8c094044,r3 */
 goto L8c093e70;
L8c093e70: target=r[3];*pr=0x8c093e74u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c093e74;
L8c093e74: r[2]=rd(m,0x8c094048u,4); /* mov.l 0x8c094048,r2 */
 goto L8c093e76;
L8c093e76: r[5]=r[15]; /* mov r15,r5 */
 goto L8c093e78;
L8c093e78: r[14]=r[0]; /* mov r0,r14 */
 goto L8c093e7a;
L8c093e7a: r[5]+=4u; /* add #0x4,r5 */
 goto L8c093e7c;
L8c093e7c: target=r[2];*pr=0x8c093e80u;r[4]=r[0];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c093e80;
L8c093e80: r[3]=8u; /* mov #0x8,r3 */
 goto L8c093e82;
L8c093e82: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c093e84;
L8c093e84: r[3]<<=8; /* shll8 r3 */
 goto L8c093e86;
L8c093e86: r[6]=rd(m,0x8c09404cu,4); /* mov.l 0x8c09404c,r6 */
 goto L8c093e88;
L8c093e88: r[3]+=4294967295u; /* add #-0x1,r3 */
 goto L8c093e8a;
L8c093e8a: r[1]=rd(m,0x8c094050u,4); /* mov.l 0x8c094050,r1 */
 goto L8c093e8c;
L8c093e8c: r[5]+=r[3]; /* add r3,r5 */
 goto L8c093e8e;
L8c093e8e: r[2]=4294967285u; /* mov #-0xb,r2 */
 goto L8c093e90;
L8c093e90: {uint32_t shift=r[2];if(!(shift&0x80000000u))r[5]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[5]=n?(uint32_t)((int32_t)r[5]>>n):((r[5]&0x80000000u)?0xffffffffu:0u);}} /* shad r2,r5 */
 goto L8c093e92;
L8c093e92: target=r[1];*pr=0x8c093e96u;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c093e96;
L8c093e96: r[3]=rd(m,0x8c094054u,4); /* mov.l 0x8c094054,r3 */
 goto L8c093e98;
L8c093e98: target=r[3];*pr=0x8c093e9cu;r[4]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c093e9c;
L8c093e9c: r[2]=rd(m,0x8c094058u,4); /* mov.l 0x8c094058,r2 */
 goto L8c093e9e;
L8c093e9e: r[6]=0u; /* mov #0x0,r6 */
 goto L8c093ea0;
L8c093ea0: r[5]=r[6]; /* mov r6,r5 */
 goto L8c093ea2;
L8c093ea2: target=r[2];*pr=0x8c093ea6u;r[4]=r[6];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c093ea6;
L8c093ea6: r[4]=r[0]; /* mov r0,r4 */
 goto L8c093ea8;
L8c093ea8: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c093eaa;
L8c093eaa: if(!(*sr&1u))goto L8c093ee0; /* bf 0x8c093ee0 */
 goto L8c093eac;
L8c093eac: r[3]=rd(m,0x8c09405cu,4); /* mov.l 0x8c09405c,r3 */
 goto L8c093eae;
L8c093eae: target=r[3];*pr=0x8c093eb2u;r[4]=1u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c093eb2;
L8c093eb2: r[2]=rd(m,0x8c094060u,4); /* mov.l 0x8c094060,r2 */
 goto L8c093eb4;
L8c093eb4: target=r[2];*pr=0x8c093eb8u;r[4]=r[15];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c093eb8;
L8c093eb8: r[4]=r[0]; /* mov r0,r4 */
 goto L8c093eba;
L8c093eba: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c093ebc;
L8c093ebc: if(!(*sr&1u))goto L8c093ee0; /* bf 0x8c093ee0 */
 goto L8c093ebe;
L8c093ebe: r[5]=rd(m,0x8c09404cu,4); /* mov.l 0x8c09404c,r5 */
 goto L8c093ec0;
L8c093ec0: r[3]=0u; /* mov #0x0,r3 */
 goto L8c093ec2;
L8c093ec2: r[2]=rd(m,0x8c094064u,4); /* mov.l 0x8c094064,r2 */
 goto L8c093ec4;
L8c093ec4: r[6]=r[3]; /* mov r3,r6 */
 goto L8c093ec6;
L8c093ec6: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c093ec8;
L8c093ec8: r[7]=4294967295u; /* mov #-0x1,r7 */
 goto L8c093eca;
L8c093eca: target=r[2];*pr=0x8c093eceu;r[4]=rd(m,r[15]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c093ece;
L8c093ece: r[4]=r[0]; /* mov r0,r4 */
 goto L8c093ed0;
L8c093ed0: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c093ed2;
L8c093ed2: take=!(*sr&1u);r[15]+=4u;if(take)goto L8c093ee0;goto L8c093ed6; /* bf/s 0x8c093ee0 */
 goto L8c093ed6;
L8c093ed6: r[3]=rd(m,0x8c094068u,4); /* mov.l 0x8c094068,r3 */
 goto L8c093ed8;
L8c093ed8: target=r[3];*pr=0x8c093edcu;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c093edc;
L8c093edc: r[4]=r[0]; /* mov r0,r4 */
 goto L8c093ede;
L8c093ede: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c093ee0;
L8c093ee0: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c093ee2;
L8c093ee2: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c093ee4;
L8c093ee4: if((*sr&1u))goto L8c093eec; /* bt 0x8c093eec */
 goto L8c093ee6;
L8c093ee6: r[3]=rd(m,0x8c09406cu,4); /* mov.l 0x8c09406c,r3 */
 goto L8c093ee8;
L8c093ee8: target=r[3];*pr=0x8c093eecu;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c093eec;
L8c093eec: r[3]=rd(m,0x8c094068u,4); /* mov.l 0x8c094068,r3 */
 goto L8c093eee;
L8c093eee: target=r[3];*pr=0x8c093ef2u;r[4]=rd(m,r[15],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c093ef2;
L8c093ef2: r[2]=rd(m,0x8c094074u,4); /* mov.l 0x8c094074,r2 */
 goto L8c093ef4;
L8c093ef4: r[4]=rd(m,0x8c094070u,4); /* mov.l 0x8c094070,r4 */
 goto L8c093ef6;
L8c093ef6: target=r[2];*pr=0x8c093efau;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c093efa;
L8c093efa: r[15]+=8u; /* add #0x8,r15 */
 goto L8c093efc;
L8c093efc: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c093efe;
L8c093efe: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
