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
int sa_sh4_sqrt(uint32_t,uint32_t *,uint32_t *);
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
/* Retail sound source query 8c65e4b8..8c65e58a, recovered with explicit SH4 ABI.
 * Mode 0 consumes available data; mode 1 reserves free ring space.
 * Lock enter/exit, signed remainder and the optional error callback are services.
 * Verified scopes: sound_source_query_verification.json and
 * input_provider_native_query_verification.json. Continuous playback unverified. */
SA_STAGE_EXPORT int sa_sound_source_query(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65e4b8;
L8c65e4b8: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c65e4ba;
L8c65e4ba: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c65e4bc;
L8c65e4bc: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c65e4be;
L8c65e4be: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65e4c0;
L8c65e4c0: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c65e4c2;
L8c65e4c2: r[13]=r[7]; /* mov r7,r13 */
 goto L8c65e4c4;
L8c65e4c4: r[12]=r[6]; /* mov r6,r12 */
 goto L8c65e4c6;
L8c65e4c6: wr(m,r[15],r[5],4); /* mov.l r5,@r15 */
 goto L8c65e4c8;
L8c65e4c8: *pr=0x8c65e4ccu;r[14]=r[4];status=call(ctx,0x8c65e368u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c65e368 */
 goto L8c65e4cc;
L8c65e4cc: wr(m,r[15]+0x4u,r[0],4); /* mov.l r0,@(0x4,r15) */
 goto L8c65e4ce;
L8c65e4ce: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65e4d0;
L8c65e4d0: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e4d2;
L8c65e4d2: if(!(*sr&1u))goto L8c65e51a; /* bf 0x8c65e51a */
 goto L8c65e4d4;
L8c65e4d4: r[4]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r4 */
 goto L8c65e4d6;
L8c65e4d6: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c65e4d8;
L8c65e4d8: r[2]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r2 */
 goto L8c65e4da;
L8c65e4da: r[4]-=r[3]; /* sub r3,r4 */
 goto L8c65e4dc;
L8c65e4dc: r[5]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r5 */
 goto L8c65e4de;
L8c65e4de: r[4]+=r[2]; /* add r2,r4 */
 goto L8c65e4e0;
L8c65e4e0: *sr=(*sr&~1u)|((int32_t)r[5]>=(int32_t)r[4]); /* cmp/ge r4,r5 */
 goto L8c65e4e2;
L8c65e4e2: if((*sr&1u))goto L8c65e4e8; /* bt 0x8c65e4e8 */
 goto L8c65e4e4;
L8c65e4e4: r[3]=r[5];goto L8c65e4ea; /* bra 0x8c65e4ea */
 goto L8c65e4e8;
L8c65e4e8: r[3]=r[4]; /* mov r4,r3 */
 goto L8c65e4ea;
L8c65e4ea: r[4]=r[3]; /* mov r3,r4 */
 goto L8c65e4ec;
L8c65e4ec: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[12]); /* cmp/ge r12,r4 */
 goto L8c65e4ee;
L8c65e4ee: wr(m,r[13]+0x4u,r[3],4); /* mov.l r3,@(0x4,r13) */
 goto L8c65e4f0;
L8c65e4f0: if((*sr&1u))goto L8c65e4f6; /* bt 0x8c65e4f6 */
 goto L8c65e4f2;
L8c65e4f2: r[1]=r[4];goto L8c65e4f8; /* bra 0x8c65e4f8 */
 goto L8c65e4f6;
L8c65e4f6: r[1]=r[12]; /* mov r12,r1 */
 goto L8c65e4f8;
L8c65e4f8: wr(m,r[13]+0x4u,r[1],4); /* mov.l r1,@(0x4,r13) */
 goto L8c65e4fa;
L8c65e4fa: r[2]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r2 */
 goto L8c65e4fc;
L8c65e4fc: r[3]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r3 */
 goto L8c65e4fe;
L8c65e4fe: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65e500;
L8c65e500: wr(m,r[13],r[2],4); /* mov.l r2,@r13 */
 goto L8c65e502;
L8c65e502: r[1]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r1 */
 goto L8c65e504;
L8c65e504: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e506;
L8c65e506: r[2]=rd(m,0x8c65e6e8u,4); /* mov.l 0x8c65e6e8,r2 */
 goto L8c65e508;
L8c65e508: r[1]+=r[3]; /* add r3,r1 */
 goto L8c65e50a;
L8c65e50a: target=r[2];*pr=0x8c65e50eu;r[0]=rd(m,r[14]+0x20u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65e50e;
L8c65e50e: wr(m,r[14]+0x14u,r[0],4); /* mov.l r0,@(0x14,r14) */
 goto L8c65e510;
L8c65e510: r[2]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r2 */
 goto L8c65e512;
L8c65e512: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e514;
L8c65e514: r[2]-=r[3]; /* sub r3,r2 */
 goto L8c65e516;
L8c65e516: wr(m,r[14]+0x10u,r[2],4);goto L8c65e57a; /* bra 0x8c65e57a */
 goto L8c65e51a;
L8c65e51a: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c65e51c;
L8c65e51c: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c65e51e;
L8c65e51e: if(!(*sr&1u))goto L8c65e566; /* bf 0x8c65e566 */
 goto L8c65e520;
L8c65e520: r[5]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r5 */
 goto L8c65e522;
L8c65e522: r[3]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r3 */
 goto L8c65e524;
L8c65e524: r[2]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r2 */
 goto L8c65e526;
L8c65e526: r[5]-=r[3]; /* sub r3,r5 */
 goto L8c65e528;
L8c65e528: r[4]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r4 */
 goto L8c65e52a;
L8c65e52a: r[5]+=r[2]; /* add r2,r5 */
 goto L8c65e52c;
L8c65e52c: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[5]); /* cmp/ge r5,r4 */
 goto L8c65e52e;
L8c65e52e: if((*sr&1u))goto L8c65e534; /* bt 0x8c65e534 */
 goto L8c65e530;
L8c65e530: r[2]=r[4];goto L8c65e536; /* bra 0x8c65e536 */
 goto L8c65e534;
L8c65e534: r[2]=r[5]; /* mov r5,r2 */
 goto L8c65e536;
L8c65e536: r[4]=r[2]; /* mov r2,r4 */
 goto L8c65e538;
L8c65e538: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[12]); /* cmp/ge r12,r4 */
 goto L8c65e53a;
L8c65e53a: wr(m,r[13]+0x4u,r[2],4); /* mov.l r2,@(0x4,r13) */
 goto L8c65e53c;
L8c65e53c: if((*sr&1u))goto L8c65e542; /* bt 0x8c65e542 */
 goto L8c65e53e;
L8c65e53e: r[3]=r[4];goto L8c65e544; /* bra 0x8c65e544 */
 goto L8c65e542;
L8c65e542: r[3]=r[12]; /* mov r12,r3 */
 goto L8c65e544;
L8c65e544: wr(m,r[13]+0x4u,r[3],4); /* mov.l r3,@(0x4,r13) */
 goto L8c65e546;
L8c65e546: r[2]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r2 */
 goto L8c65e548;
L8c65e548: r[3]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r3 */
 goto L8c65e54a;
L8c65e54a: r[2]+=r[3]; /* add r3,r2 */
 goto L8c65e54c;
L8c65e54c: wr(m,r[13],r[2],4); /* mov.l r2,@r13 */
 goto L8c65e54e;
L8c65e54e: r[1]=rd(m,r[14]+0x18u,4); /* mov.l @(0x18,r14),r1 */
 goto L8c65e550;
L8c65e550: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e552;
L8c65e552: r[2]=rd(m,0x8c65e6e8u,4); /* mov.l 0x8c65e6e8,r2 */
 goto L8c65e554;
L8c65e554: r[1]+=r[3]; /* add r3,r1 */
 goto L8c65e556;
L8c65e556: target=r[2];*pr=0x8c65e55au;r[0]=rd(m,r[14]+0x20u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c65e55a;
L8c65e55a: wr(m,r[14]+0x18u,r[0],4); /* mov.l r0,@(0x18,r14) */
 goto L8c65e55c;
L8c65e55c: r[2]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r2 */
 goto L8c65e55e;
L8c65e55e: r[3]=rd(m,r[13]+0x4u,4); /* mov.l @(0x4,r13),r3 */
 goto L8c65e560;
L8c65e560: r[2]-=r[3]; /* sub r3,r2 */
 goto L8c65e562;
L8c65e562: wr(m,r[14]+0xcu,r[2],4);goto L8c65e57a; /* bra 0x8c65e57a */
 goto L8c65e566;
L8c65e566: r[4]=0u; /* mov #0x0,r4 */
 goto L8c65e568;
L8c65e568: wr(m,r[13]+0x4u,r[4],4); /* mov.l r4,@(0x4,r13) */
 goto L8c65e56a;
L8c65e56a: wr(m,r[13],r[4],4); /* mov.l r4,@r13 */
 goto L8c65e56c;
L8c65e56c: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c65e56e;
L8c65e56e: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c65e570;
L8c65e570: if((*sr&1u))goto L8c65e57a; /* bt 0x8c65e57a */
 goto L8c65e572;
L8c65e572: r[3]=rd(m,r[14]+0x28u,4); /* mov.l @(0x28,r14),r3 */
 goto L8c65e574;
L8c65e574: r[5]=4294967293u; /* mov #-0x3,r5 */
 goto L8c65e576;
L8c65e576: target=r[3];*pr=0x8c65e57au;r[4]=rd(m,r[14]+0x2cu,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65e57a;
L8c65e57a: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c65e57c;
L8c65e57c: r[4]=r[14]; /* mov r14,r4 */
 goto L8c65e57e;
L8c65e57e: r[15]+=8u; /* add #0x8,r15 */
 goto L8c65e580;
L8c65e580: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65e582;
L8c65e582: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c65e584;
L8c65e584: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c65e586;
L8c65e586: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c65e380u,m,f,sr,r,pr); /* bra 0x8c65e380 */
}
