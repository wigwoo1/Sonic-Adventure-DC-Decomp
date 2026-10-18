/* Sonic Adventure USA Rev A recovered frame startup helper. */
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
/* Static C recovery of retail Tikal helper 8c6434e0..8c643588.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. Integer routine; FPU mode bits do not affect its instructions. */
SA_STAGE_EXPORT int sa_engine_frame_object_allocate(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c6434e0;
L8c6434e0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c6434e2;
L8c6434e2: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c6434e4;
L8c6434e4: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c6434e6;
L8c6434e6: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c6434e8;
L8c6434e8: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c6434ea;
L8c6434ea: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c6434ec;
L8c6434ec: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c6434ee;
L8c6434ee: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c6434f0;
L8c6434f0: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c6434f2;
L8c6434f2: r[3]=rd(m,0x8c64358cu,4); /* mov.l 0x8c64358c,r3 */
 goto L8c6434f4;
L8c6434f4: r[9]=0u; /* mov #0x0,r9 */
 goto L8c6434f6;
L8c6434f6: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c6434f8;
L8c6434f8: r[11]=r[9]; /* mov r9,r11 */
 goto L8c6434fa;
L8c6434fa: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c6434fc;
L8c6434fc: r[0]=sx(m,0x8c643588u,2); /* mov.w 0x8c643588,r0 */
 goto L8c6434fe;
L8c6434fe: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c643500;
L8c643500: take=!(*sr&1u);r[14]=rd(m,r[4]+r[0],4);if(take)goto L8c64350a;goto L8c643504; /* bf/s 0x8c64350a */
 goto L8c643504;
L8c643504: r[11]=rd(m,0x8c643590u,4); /* mov.l 0x8c643590,r11 */
 goto L8c643506;
L8c643506: goto L8c643566; /* bra 0x8c643566 */
 goto L8c64350a;
L8c64350a: r[2]=rd(m,0x8c643598u,4); /* mov.l 0x8c643598,r2 */
 goto L8c64350c;
L8c64350c: r[12]=rd(m,0x8c643594u,4); /* mov.l 0x8c643594,r12 */
 goto L8c64350e;
L8c64350e: target=r[12];*pr=0x8c643512u;r[15]-=4;wr(m,r[15],r[2],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c643512;
L8c643512: r[15]+=4u; /* add #0x4,r15 */
 goto L8c643514;
L8c643514: r[8]=rd(m,0x8c64359cu,4); /* mov.l 0x8c64359c,r8 */
 goto L8c643516;
L8c643516: r[10]=64u; /* mov #0x40,r10 */
 goto L8c643518;
L8c643518: r[13]=r[9]; /* mov r9,r13 */
 goto L8c64351a;
L8c64351a: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c64351c;
L8c64351c: target=r[12];*pr=0x8c643520u;r[15]-=4;wr(m,r[15],r[8],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c643520;
L8c643520: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c643522;
L8c643522: *sr=(*sr&~1u)|(r[0]==0x1u); /* cmp/eq #0x1,r0 */
 goto L8c643524;
L8c643524: take=!(*sr&1u);r[15]+=8u;if(take)goto L8c643542;goto L8c643528; /* bf/s 0x8c643542 */
 goto L8c643528;
L8c643528: r[13]+=1u; /* add #0x1,r13 */
 goto L8c64352a;
L8c64352a: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[10]); /* cmp/ge r10,r13 */
 goto L8c64352c;
L8c64352c: take=!(*sr&1u);r[14]+=32u;if(take)goto L8c64351a;goto L8c643530; /* bf/s 0x8c64351a */
 goto L8c643530;
L8c643530: r[2]=rd(m,0x8c6435a0u,4); /* mov.l 0x8c6435a0,r2 */
 goto L8c643532;
L8c643532: target=r[12];*pr=0x8c643536u;r[15]-=4;wr(m,r[15],r[2],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c643536;
L8c643536: r[15]+=4u; /* add #0x4,r15 */
 goto L8c643538;
L8c643538: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c64353a;
L8c64353a: r[11]=rd(m,0x8c6435a4u,4); /* mov.l 0x8c6435a4,r11 */
 goto L8c64353c;
L8c64353c: wr(m,r[3],r[9],4); /* mov.l r9,@r3 */
 goto L8c64353e;
L8c64353e: goto L8c643566; /* bra 0x8c643566 */
 goto L8c643542;
L8c643542: r[2]=rd(m,0x8c6435a8u,4); /* mov.l 0x8c6435a8,r2 */
 goto L8c643544;
L8c643544: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c643546;
L8c643546: target=r[12];*pr=0x8c64354au;r[15]-=4;wr(m,r[15],r[2],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c64354a;
L8c64354a: r[15]+=8u; /* add #0x8,r15 */
 goto L8c64354c;
L8c64354c: r[3]=rd(m,0x8c6435b0u,4); /* mov.l 0x8c6435b0,r3 */
 goto L8c64354e;
L8c64354e: r[1]=r[14]; /* mov r14,r1 */
 goto L8c643550;
L8c643550: r[2]=rd(m,0x8c6435acu,4); /* mov.l 0x8c6435ac,r2 */
 goto L8c643552;
L8c643552: target=r[3];*pr=0x8c643556u;r[0]=32u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643556;
L8c643556: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c643558;
L8c643558: r[0]=sx(m,0x8c64358au,2); /* mov.w 0x8c64358a,r0 */
 goto L8c64355a;
L8c64355a: wr(m,r[1],r[14],4); /* mov.l r14,@r1 */
 goto L8c64355c;
L8c64355c: r[1]=rd(m,0x8c64358cu,4); /* mov.l 0x8c64358c,r1 */
 goto L8c64355e;
L8c64355e: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c643560;
L8c643560: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c643562;
L8c643562: r[2]+=1u; /* add #0x1,r2 */
 goto L8c643564;
L8c643564: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c643566;
L8c643566: *sr=(*sr&~1u)|!(r[11]&r[11]); /* tst r11,r11 */
 goto L8c643568;
L8c643568: if((*sr&1u))goto L8c643572; /* bt 0x8c643572 */
 goto L8c64356a;
L8c64356a: r[2]=rd(m,0x8c6435b8u,4); /* mov.l 0x8c6435b8,r2 */
 goto L8c64356c;
L8c64356c: r[4]=rd(m,0x8c6435b4u,4); /* mov.l 0x8c6435b4,r4 */
 goto L8c64356e;
L8c64356e: target=r[2];*pr=0x8c643572u;r[5]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c643572;
L8c643572: r[0]=r[11]; /* mov r11,r0 */
 goto L8c643574;
L8c643574: r[15]+=4u; /* add #0x4,r15 */
 goto L8c643576;
L8c643576: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c643578;
L8c643578: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c64357a;
L8c64357a: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c64357c;
L8c64357c: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c64357e;
L8c64357e: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c643580;
L8c643580: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c643582;
L8c643582: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c643584;
L8c643584: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
