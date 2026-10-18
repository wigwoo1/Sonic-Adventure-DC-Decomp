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
/* Retail stream scheduler 667844..667900. Verified within the scope of
 * analysis/sound_stream_decode_schedule_verification.json; downstream request
 * wrappers remain services in that comparison. */
SA_STAGE_EXPORT int sa_sound_stream_decode_schedule(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c667844;
L8c667844: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c667846;
L8c667846: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c667848;
L8c667848: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66784a;
L8c66784a: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c66784c;
L8c66784c: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c66784e;
L8c66784e: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c667850;
L8c667850: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c667852;
L8c667852: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c667854;
L8c667854: r[5]=r[4]; /* mov r4,r5 */
 goto L8c667856;
L8c667856: r[5]+=80u; /* add #0x50,r5 */
 goto L8c667858;
L8c667858: r[3]=rd(m,0x8c667924u,4); /* mov.l 0x8c667924,r3 */
 goto L8c66785a;
L8c66785a: r[1]=rd(m,r[5]+0x4u,4); /* mov.l @(0x4,r5),r1 */
 goto L8c66785c;
L8c66785c: target=r[3];*pr=0x8c667860u;r[0]=rd(m,r[5]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c667860;
L8c667860: r[10]=rd(m,r[5]+0x28u,4); /* mov.l @(0x28,r5),r10 */
 goto L8c667862;
L8c667862: r[14]=r[0]; /* mov r0,r14 */
 goto L8c667864;
L8c667864: r[6]=rd(m,r[5]+0x10u,4); /* mov.l @(0x10,r5),r6 */
 goto L8c667866;
L8c667866: r[1]=r[10]; /* mov r10,r1 */
 goto L8c667868;
L8c667868: r[3]=rd(m,0x8c667924u,4); /* mov.l 0x8c667924,r3 */
 goto L8c66786a;
L8c66786a: r[1]+=r[6]; /* add r6,r1 */
 goto L8c66786c;
L8c66786c: r[11]=rd(m,r[5]+0x20u,4); /* mov.l @(0x20,r5),r11 */
 goto L8c66786e;
L8c66786e: r[1]+=4294967295u; /* add #-0x1,r1 */
 goto L8c667870;
L8c667870: r[13]=rd(m,r[5]+0x24u,4); /* mov.l @(0x24,r5),r13 */
 goto L8c667872;
L8c667872: r[7]=rd(m,r[5]+0x18u,4); /* mov.l @(0x18,r5),r7 */
 goto L8c667874;
L8c667874: wr(m,r[15],r[1],4); /* mov.l r1,@r15 */
 goto L8c667876;
L8c667876: target=r[3];*pr=0x8c66787au;r[0]=r[6];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c66787a;
L8c66787a: r[2]=rd(m,0x8c667928u,4); /* mov.l 0x8c667928,r2 */
 goto L8c66787c;
L8c66787c: r[12]=r[0]; /* mov r0,r12 */
 goto L8c66787e;
L8c66787e: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c667880;
L8c667880: target=r[2];*pr=0x8c667884u;r[0]=r[6];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c667884;
L8c667884: r[7]-=r[11]; /* sub r11,r7 */
 goto L8c667886;
L8c667886: r[9]=r[6]; /* mov r6,r9 */
 goto L8c667888;
L8c667888: r[1]=r[7]; /* mov r7,r1 */
 goto L8c66788a;
L8c66788a: r[1]+=r[6]; /* add r6,r1 */
 goto L8c66788c;
L8c66788c: r[3]=rd(m,0x8c667924u,4); /* mov.l 0x8c667924,r3 */
 goto L8c66788e;
L8c66788e: r[9]-=r[0]; /* sub r0,r9 */
 goto L8c667890;
L8c667890: r[9]+=4294967295u; /* add #-0x1,r9 */
 goto L8c667892;
L8c667892: r[1]+=4294967295u; /* add #-0x1,r1 */
 goto L8c667894;
L8c667894: target=r[3];*pr=0x8c667898u;r[0]=r[6];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c667898;
L8c667898: r[7]=r[0]; /* mov r0,r7 */
 goto L8c66789a;
L8c66789a: *sr=(*sr&~1u)|((int32_t)r[12]>=(int32_t)r[7]); /* cmp/ge r7,r12 */
 goto L8c66789c;
L8c66789c: *macl=r[6]*r[7]; /* mul.l r6,r7 */
 goto L8c66789e;
L8c66789e: r[2]=*macl; /* sts MACL,r2 */
 goto L8c6678a0;
L8c6678a0: take=(*sr&1u);wr(m,r[15],r[2],4);if(take)goto L8c6678b0;goto L8c6678a4; /* bt/s 0x8c6678b0 */
 goto L8c6678a4;
L8c6678a4: r[1]=rd(m,r[5]+0x18u,4); /* mov.l @(0x18,r5),r1 */
 goto L8c6678a6;
L8c6678a6: r[2]-=r[9]; /* sub r9,r2 */
 goto L8c6678a8;
L8c6678a8: r[2]+=r[11]; /* add r11,r2 */
 goto L8c6678aa;
L8c6678aa: *sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[1]); /* cmp/ge r1,r2 */
 goto L8c6678ac;
L8c6678ac: if((*sr&1u))goto L8c6678b0; /* bt 0x8c6678b0 */
 goto L8c6678ae;
L8c6678ae: r[7]+=1u; /* add #0x1,r7 */
 goto L8c6678b0;
L8c6678b0: *sr=(*sr&~1u)|((int32_t)r[10]>=(int32_t)r[13]); /* cmp/ge r13,r10 */
 goto L8c6678b2;
L8c6678b2: if((*sr&1u))goto L8c6678b6; /* bt 0x8c6678b6 */
 goto L8c6678b4;
L8c6678b4: r[13]+=r[9]; /* add r9,r13 */
 goto L8c6678b6;
L8c6678b6: r[3]=rd(m,0x8c667924u,4); /* mov.l 0x8c667924,r3 */
 goto L8c6678b8;
L8c6678b8: r[1]=r[13]; /* mov r13,r1 */
 goto L8c6678ba;
L8c6678ba: target=r[3];*pr=0x8c6678beu;r[0]=r[6];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6678be;
L8c6678be: r[6]=r[0]; /* mov r0,r6 */
 goto L8c6678c0;
L8c6678c0: *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[6]); /* cmp/gt r6,r14 */
 goto L8c6678c2;
L8c6678c2: if(!(*sr&1u))goto L8c6678c6; /* bf 0x8c6678c6 */
 goto L8c6678c4;
L8c6678c4: r[14]=r[6]; /* mov r6,r14 */
 goto L8c6678c6;
L8c6678c6: *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[12]); /* cmp/gt r12,r14 */
 goto L8c6678c8;
L8c6678c8: if(!(*sr&1u))goto L8c6678cc; /* bf 0x8c6678cc */
 goto L8c6678ca;
L8c6678ca: r[14]=r[12]; /* mov r12,r14 */
 goto L8c6678cc;
L8c6678cc: *sr=(*sr&~1u)|((int32_t)r[14]>(int32_t)r[7]); /* cmp/gt r7,r14 */
 goto L8c6678ce;
L8c6678ce: take=!(*sr&1u);r[0]=rd(m,r[5]+0x8u,4);if(take)goto L8c6678d4;goto L8c6678d2; /* bf/s 0x8c6678d4 */
 goto L8c6678d2;
L8c6678d2: r[14]=r[7]; /* mov r7,r14 */
 goto L8c6678d4;
L8c6678d4: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c6678d6;
L8c6678d6: if(!(*sr&1u))goto L8c6678ec; /* bf 0x8c6678ec */
 goto L8c6678d8;
L8c6678d8: r[5]=r[14]; /* mov r14,r5 */
 goto L8c6678da;
L8c6678da: r[15]+=4u; /* add #0x4,r15 */
 goto L8c6678dc;
L8c6678dc: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6678de;
L8c6678de: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c6678e0;
L8c6678e0: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c6678e2;
L8c6678e2: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c6678e4;
L8c6678e4: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6678e6;
L8c6678e6: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6678e8;
L8c6678e8: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c66780eu,m,f,sr,r,pr); /* bra 0x8c66780e */
 goto L8c6678ec;
L8c6678ec: r[5]=r[14]; /* mov r14,r5 */
 goto L8c6678ee;
L8c6678ee: r[15]+=4u; /* add #0x4,r15 */
 goto L8c6678f0;
L8c6678f0: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c6678f2;
L8c6678f2: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c6678f4;
L8c6678f4: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c6678f6;
L8c6678f6: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c6678f8;
L8c6678f8: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c6678fa;
L8c6678fa: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c6678fc;
L8c6678fc: r[14]=rd(m,r[15],4);r[15]+=4;return call(ctx,0x8c6677e2u,m,f,sr,r,pr); /* bra 0x8c6677e2 */
}
