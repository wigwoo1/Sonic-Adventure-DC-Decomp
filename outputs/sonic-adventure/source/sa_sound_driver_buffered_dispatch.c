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
/* Static C recovery of retail Tikal helper 8c668916..8c6689b6.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_sound_driver_buffered_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c668916;
L8c668916: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c668918;
L8c668918: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66891a;
L8c66891a: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66891c;
L8c66891c: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66891e;
L8c66891e: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c668920;
L8c668920: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c668922;
L8c668922: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c668924;
L8c668924: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c668926;
L8c668926: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c668928;
L8c668928: r[3]=rd(m,0x8c668960u,4); /* mov.l 0x8c668960,r3 */
 goto L8c66892a;
L8c66892a: r[13]=r[4]; /* mov r4,r13 */
 goto L8c66892c;
L8c66892c: r[4]=r[15]; /* mov r15,r4 */
 goto L8c66892e;
L8c66892e: r[10]=r[5]; /* mov r5,r10 */
 goto L8c668930;
L8c668930: wr(m,r[15]+0x8u,r[6],4); /* mov.l r6,@(0x8,r15) */
 goto L8c668932;
L8c668932: target=r[3];*pr=0x8c668936u;r[4]+=4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668936;
L8c668936: r[2]=rd(m,0x8c668964u,4); /* mov.l 0x8c668964,r2 */
 goto L8c668938;
L8c668938: target=r[2];*pr=0x8c66893cu;r[4]=0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c66893c;
L8c66893c: r[3]=rd(m,0x8c668968u,4); /* mov.l 0x8c668968,r3 */
 goto L8c66893e;
L8c66893e: r[12]=0u; /* mov #0x0,r12 */
 goto L8c668940;
L8c668940: r[11]=sx(m,0x8c668958u,2); /* mov.w 0x8c668958,r11 */
 goto L8c668942;
L8c668942: r[1]=r[10]; /* mov r10,r1 */
 goto L8c668944;
L8c668944: target=r[3];*pr=0x8c668948u;r[0]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c668948;
L8c668948: r[2]=rd(m,0x8c66896cu,4); /* mov.l 0x8c66896c,r2 */
 goto L8c66894a;
L8c66894a: r[1]=r[10]; /* mov r10,r1 */
 goto L8c66894c;
L8c66894c: r[8]=r[0]; /* mov r0,r8 */
 goto L8c66894e;
L8c66894e: target=r[2];*pr=0x8c668952u;r[0]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c668952;
L8c668952: r[9]=r[11]; /* mov r11,r9 */
 goto L8c668954;
L8c668954: r[9]-=r[0];goto L8c668996; /* bra 0x8c668996 */
 goto L8c668970;
L8c668970: r[2]=r[9]; /* mov r9,r2 */
 goto L8c668972;
L8c668972: *sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[11]); /* cmp/ge r11,r2 */
 goto L8c668974;
L8c668974: r[14]=r[8]; /* mov r8,r14 */
 goto L8c668976;
L8c668976: take=(*sr&1u);wr(m,r[13]+0x3cu,r[9],4);if(take)goto L8c66897c;goto L8c66897a; /* bt/s 0x8c66897c */
 goto L8c66897a;
L8c66897a: r[14]+=1u; /* add #0x1,r14 */
 goto L8c66897c;
L8c66897c: r[5]=r[14]; /* mov r14,r5 */
 goto L8c66897e;
L8c66897e: r[6]=r[15]; /* mov r15,r6 */
 goto L8c668980;
L8c668980: *pr=0x8c668984u;r[4]=r[10];status=call(ctx,0x8c6687bcu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6687bc */
 goto L8c668984;
L8c668984: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c668986;
L8c668986: r[5]=r[12]; /* mov r12,r5 */
 goto L8c668988;
L8c668988: r[6]=r[14]; /* mov r14,r6 */
 goto L8c66898a;
L8c66898a: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c66898c;
L8c66898c: r[7]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r7 */
 goto L8c66898e;
L8c66898e: *pr=0x8c668992u;r[4]=r[13];status=call(ctx,0x8c6687f4u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c6687f4 */
 goto L8c668992;
L8c668992: r[15]+=4u; /* add #0x4,r15 */
 goto L8c668994;
L8c668994: r[12]+=1u; /* add #0x1,r12 */
 goto L8c668996;
L8c668996: r[2]=rd(m,r[13]+0x8u,4); /* mov.l @(0x8,r13),r2 */
 goto L8c668998;
L8c668998: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[2]); /* cmp/ge r2,r12 */
 goto L8c66899a;
L8c66899a: if(!(*sr&1u))goto L8c668970; /* bf 0x8c668970 */
 goto L8c66899c;
L8c66899c: r[3]=rd(m,0x8c668aa4u,4); /* mov.l 0x8c668aa4,r3 */
 goto L8c66899e;
L8c66899e: target=r[3];*pr=0x8c6689a2u;r[4]=rd(m,r[15]+0x4u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6689a2;
L8c6689a2: r[15]+=12u; /* add #0xc,r15 */
 goto L8c6689a4;
L8c6689a4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6689a6;
L8c6689a6: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c6689a8;
L8c6689a8: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c6689aa;
L8c6689aa: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c6689ac;
L8c6689ac: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c6689ae;
L8c6689ae: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6689b0;
L8c6689b0: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6689b2;
L8c6689b2: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
