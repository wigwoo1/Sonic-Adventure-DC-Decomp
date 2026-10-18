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
/* USA Rev A sound-work update0948a0; provisional name. Static530-instruction recovery. Playback/volume services require connection and execution verification. */
SA_STAGE_EXPORT int sa_sound_voice_records_update(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c0948a0;
L8c0948a0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c0948a2;
L8c0948a2: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c0948a4;
L8c0948a4: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c0948a6;
L8c0948a6: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c0948a8;
L8c0948a8: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c0948aa;
L8c0948aa: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c0948ac;
L8c0948ac: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c0948ae;
L8c0948ae: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c0948b0;
L8c0948b0: r[15]+=4294967264u; /* add #-0x20,r15 */
 goto L8c0948b2;
L8c0948b2: r[2]=rd(m,0x8c094a00u,4); /* mov.l 0x8c094a00,r2 */
 goto L8c0948b4;
L8c0948b4: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c0948b6;
L8c0948b6: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c0948b8;
L8c0948b8: if((*sr&1u))goto L8c0948be; /* bt 0x8c0948be */
 goto L8c0948ba;
L8c0948ba: goto L8c094d3c; /* bra 0x8c094d3c */
 goto L8c0948be;
L8c0948be: r[3]=rd(m,0x8c094a04u,4); /* mov.l 0x8c094a04,r3 */
 goto L8c0948c0;
L8c0948c0: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c0948c2;
L8c0948c2: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c0948c4;
L8c0948c4: if(!(*sr&1u))goto L8c0948d0; /* bf 0x8c0948d0 */
 goto L8c0948c6;
L8c0948c6: r[1]=rd(m,0x8c094a10u,4); /* mov.l 0x8c094a10,r1 */
 goto L8c0948c8;
L8c0948c8: r[4]=rd(m,0x8c094a0cu,4); /* mov.l 0x8c094a0c,r4 */
 goto L8c0948ca;
L8c0948ca: r[5]=rd(m,0x8c094a08u,4); /* mov.l 0x8c094a08,r5 */
 goto L8c0948cc;
L8c0948cc: target=r[1];*pr=0x8c0948d0u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c0948d0;
L8c0948d0: r[3]=rd(m,0x8c094a14u,4); /* mov.l 0x8c094a14,r3 */
 goto L8c0948d2;
L8c0948d2: r[11]=0u; /* mov #0x0,r11 */
 goto L8c0948d4;
L8c0948d4: r[14]=rd(m,0x8c094a18u,4); /* mov.l 0x8c094a18,r14 */
 goto L8c0948d6;
L8c0948d6: r[8]=1u; /* mov #0x1,r8 */
 goto L8c0948d8;
L8c0948d8: r[13]=r[11]; /* mov r11,r13 */
 goto L8c0948da;
L8c0948da: wr(m,r[15]+0x1cu,r[3],4); /* mov.l r3,@(0x1c,r15) */
 goto L8c0948dc;
L8c0948dc: r[10]=52u; /* mov #0x34,r10 */
 goto L8c0948de;
L8c0948de: r[3]=r[13]; /* mov r13,r3 */
 goto L8c0948e0;
L8c0948e0: *macl=r[10]*r[13]; /* mul.l r10,r13 */
 goto L8c0948e2;
L8c0948e2: r[2]=1u; /* mov #0x1,r2 */
 goto L8c0948e4;
L8c0948e4: r[1]=r[13]; /* mov r13,r1 */
 goto L8c0948e6;
L8c0948e6: r[3]+=7u; /* add #0x7,r3 */
 goto L8c0948e8;
L8c0948e8: r[2]<<=8; /* shll8 r2 */
 goto L8c0948ea;
L8c0948ea: r[10]=*macl; /* sts MACL,r10 */
 goto L8c0948ec;
L8c0948ec: r[1]<<=2; /* shll2 r1 */
 goto L8c0948ee;
L8c0948ee: r[12]=r[10]; /* mov r10,r12 */
 goto L8c0948f0;
L8c0948f0: r[12]+=r[14]; /* add r14,r12 */
 goto L8c0948f2;
L8c0948f2: r[12]=rd(m,r[12]+0xcu,4); /* mov.l @(0xc,r12),r12 */
 goto L8c0948f4;
L8c0948f4: wr(m,r[15]+0x8u,r[3],4); /* mov.l r3,@(0x8,r15) */
 goto L8c0948f6;
L8c0948f6: r[3]=r[12]; /* mov r12,r3 */
 goto L8c0948f8;
L8c0948f8: r[2]&=r[12]; /* and r12,r2 */
 goto L8c0948fa;
L8c0948fa: *sr=(*sr&~1u)|!(r[8]&r[3]); /* tst r8,r3 */
 goto L8c0948fc;
L8c0948fc: wr(m,r[15]+0x14u,r[2],4); /* mov.l r2,@(0x14,r15) */
 goto L8c0948fe;
L8c0948fe: wr(m,r[15]+0xcu,r[1],4); /* mov.l r1,@(0xc,r15) */
 goto L8c094900;
L8c094900: take=!(*sr&1u);r[9]=r[11];if(take)goto L8c094908;goto L8c094904; /* bf/s 0x8c094908 */
 goto L8c094904;
L8c094904: goto L8c094b00; /* bra 0x8c094b00 */
 goto L8c094908;
L8c094908: r[3]=rd(m,0x8c094a04u,4); /* mov.l 0x8c094a04,r3 */
 goto L8c09490a;
L8c09490a: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c09490c;
L8c09490c: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c09490e;
L8c09490e: if(!(*sr&1u))goto L8c09491c; /* bf 0x8c09491c */
 goto L8c094910;
L8c094910: r[4]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r4 */
 goto L8c094912;
L8c094912: r[1]=rd(m,0x8c094a10u,4); /* mov.l 0x8c094a10,r1 */
 goto L8c094914;
L8c094914: r[5]=rd(m,0x8c094a1cu,4); /* mov.l 0x8c094a1c,r5 */
 goto L8c094916;
L8c094916: r[2]=rd(m,0x8c094a20u,4); /* mov.l 0x8c094a20,r2 */
 goto L8c094918;
L8c094918: target=r[1];*pr=0x8c09491cu;r[4]|=r[2];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c09491c;
L8c09491c: r[3]=r[10]; /* mov r10,r3 */
 goto L8c09491e;
L8c09491e: r[3]+=r[14]; /* add r14,r3 */
 goto L8c094920;
L8c094920: r[2]=rd(m,r[3]+0x10u,4); /* mov.l @(0x10,r3),r2 */
 goto L8c094922;
L8c094922: r[3]=5u; /* mov #0x5,r3 */
 goto L8c094924;
L8c094924: r[3]<<=8; /* shll8 r3 */
 goto L8c094926;
L8c094926: r[1]=r[2]; /* mov r2,r1 */
 goto L8c094928;
L8c094928: r[3]+=4294967290u; /* add #-0x6,r3 */
 goto L8c09492a;
L8c09492a: *sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[3]); /* cmp/gt r3,r1 */
 goto L8c09492c;
L8c09492c: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c09492e;
L8c09492e: if(!(*sr&1u))goto L8c09494a; /* bf 0x8c09494a */
 goto L8c094930;
L8c094930: r[2]=2u; /* mov #0x2,r2 */
 goto L8c094932;
L8c094932: r[4]=rd(m,r[15],4); /* mov.l @r15,r4 */
 goto L8c094934;
L8c094934: r[1]=rd(m,0x8c094a24u,4); /* mov.l 0x8c094a24,r1 */
 goto L8c094936;
L8c094936: r[2]<<=8; /* shll8 r2 */
 goto L8c094938;
L8c094938: r[2]+=4294967239u; /* add #-0x39,r2 */
 goto L8c09493a;
L8c09493a: target=r[1];*pr=0x8c09493eu;r[4]+=r[2];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c09493e;
L8c09493e: r[2]=r[10]; /* mov r10,r2 */
 goto L8c094940;
L8c094940: r[3]=4294967279u; /* mov #-0x11,r3 */
 goto L8c094942;
L8c094942: r[2]+=r[14]; /* add r14,r2 */
 goto L8c094944;
L8c094944: r[12]&=r[3]; /* and r3,r12 */
 goto L8c094946;
L8c094946: wr(m,r[2]+0x4u,r[11],4);goto L8c094acc; /* bra 0x8c094acc */
 goto L8c09494a;
L8c09494a: wr(m,r[15]+0x4u,r[11],4); /* mov.l r11,@(0x4,r15) */
 goto L8c09494c;
L8c09494c: r[4]=rd(m,r[15]+0x1cu,4); /* mov.l @(0x1c,r15),r4 */
 goto L8c09494e;
L8c09494e: r[2]=rd(m,r[4],4); /* mov.l @r4,r2 */
 goto L8c094950;
L8c094950: *sr=(*sr&~1u)|((int32_t)r[2]>=0); /* cmp/pz r2 */
 goto L8c094952;
L8c094952: take=(*sr&1u);r[9]=r[11];if(take)goto L8c09495a;goto L8c094956; /* bt/s 0x8c09495a */
 goto L8c094956;
L8c094956: *sr=(*sr&~1u)|!(r[9]&r[9]); /* tst r9,r9 */
 goto L8c094958;
L8c094958: if((*sr&1u))goto L8c094976; /* bt 0x8c094976 */
 goto L8c09495a;
L8c09495a: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c09495c;
L8c09495c: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c09495e;
L8c09495e: *sr=(*sr&~1u)|((int32_t)r[1]>(int32_t)r[3]); /* cmp/gt r3,r1 */
 goto L8c094960;
L8c094960: if(!(*sr&1u))goto L8c09496a; /* bf 0x8c09496a */
 goto L8c094962;
L8c094962: r[9]=rd(m,r[4],4);r[4]+=4; /* mov.l @r4+,r9 */
 goto L8c094964;
L8c094964: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c094966;
L8c094966: wr(m,r[15]+0x4u,r[3],4);goto L8c094976; /* bra 0x8c094976 */
 goto L8c09496a;
L8c09496a: r[4]+=8u; /* add #0x8,r4 */
 goto L8c09496c;
L8c09496c: r[3]=rd(m,r[4],4); /* mov.l @r4,r3 */
 goto L8c09496e;
L8c09496e: *sr=(*sr&~1u)|((int32_t)r[3]>=0); /* cmp/pz r3 */
 goto L8c094970;
L8c094970: if((*sr&1u))goto L8c09495a; /* bt 0x8c09495a */
 goto L8c094972;
L8c094972: *sr=(*sr&~1u)|!(r[9]&r[9]); /* tst r9,r9 */
 goto L8c094974;
L8c094974: if(!(*sr&1u))goto L8c09495a; /* bf 0x8c09495a */
 goto L8c094976;
L8c094976: *pr=0x8c09497au;r[4]=rd(m,r[15],4);status=call(ctx,0x8c095612u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c095612 */
 goto L8c09497a;
L8c09497a: *sr=(*sr&~1u)|!(r[0]&r[0]); /* tst r0,r0 */
 goto L8c09497c;
L8c09497c: if((*sr&1u))goto L8c094990; /* bt 0x8c094990 */
 goto L8c09497e;
L8c09497e: r[0]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c094980;
L8c094980: *sr=(*sr&~1u)|(r[0]==0x6u); /* cmp/eq #0x6,r0 */
 goto L8c094982;
L8c094982: if(!(*sr&1u))goto L8c094986; /* bf 0x8c094986 */
 goto L8c094984;
L8c094984: wr(m,r[15]+0x4u,r[8],4); /* mov.l r8,@(0x4,r15) */
 goto L8c094986;
L8c094986: r[0]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c094988;
L8c094988: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c09498a;
L8c09498a: if(!(*sr&1u))goto L8c094990; /* bf 0x8c094990 */
 goto L8c09498c;
L8c09498c: r[2]=4u; /* mov #0x4,r2 */
 goto L8c09498e;
L8c09498e: wr(m,r[15]+0x4u,r[2],4); /* mov.l r2,@(0x4,r15) */
 goto L8c094990;
L8c094990: r[3]=16u; /* mov #0x10,r3 */
 goto L8c094992;
L8c094992: r[3]<<=8; /* shll8 r3 */
 goto L8c094994;
L8c094994: *sr=(*sr&~1u)|!(r[12]&r[3]); /* tst r12,r3 */
 goto L8c094996;
L8c094996: if((*sr&1u))goto L8c094a40; /* bt 0x8c094a40 */
 goto L8c094998;
L8c094998: *pr=0x8c09499cu;r[4]=r[13];status=call(ctx,0x8c095402u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c095402 */
 goto L8c09499c;
L8c09499c: *sr=(*sr&~1u)|((int32_t)r[0]>=0); /* cmp/pz r0 */
 goto L8c09499e;
L8c09499e: take=!(*sr&1u);wr(m,r[15]+0x18u,r[0],4);if(take)goto L8c0949b4;goto L8c0949a2; /* bf/s 0x8c0949b4 */
 goto L8c0949a2;
L8c0949a2: *pr=0x8c0949a6u;r[4]=r[13];status=call(ctx,0x8c0955c0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c0955c0 */
 goto L8c0949a6;
L8c0949a6: r[3]=r[10]; /* mov r10,r3 */
 goto L8c0949a8;
L8c0949a8: r[3]+=r[14]; /* add r14,r3 */
 goto L8c0949aa;
L8c0949aa: r[4]=r[0]; /* mov r0,r4 */
 goto L8c0949ac;
L8c0949ac: r[0]=rd(m,0x8c094a28u,4); /* mov.l 0x8c094a28,r0 */
 goto L8c0949ae;
L8c0949ae: r[2]=rd(m,r[3]+0x30u,4); /* mov.l @(0x30,r3),r2 */
 goto L8c0949b0;
L8c0949b0: wr(m,r[2]+r[0],r[4],1);goto L8c0949b6; /* bra 0x8c0949b6 */
 goto L8c0949b4;
L8c0949b4: wr(m,r[15]+0x18u,r[11],4); /* mov.l r11,@(0x18,r15) */
 goto L8c0949b6;
L8c0949b6: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c0949b8;
L8c0949b8: *sr=(*sr&~1u)|!(r[9]&r[9]); /* tst r9,r9 */
 goto L8c0949ba;
L8c0949ba: r[3]=rd(m,r[15]+0x18u,4); /* mov.l @(0x18,r15),r3 */
 goto L8c0949bc;
L8c0949bc: r[2]+=r[3]; /* add r3,r2 */
 goto L8c0949be;
L8c0949be: take=(*sr&1u);wr(m,r[15]+0x10u,r[2],4);if(take)goto L8c0949d4;goto L8c0949c2; /* bt/s 0x8c0949d4 */
 goto L8c0949c2;
L8c0949c2: r[2]-=r[9]; /* sub r9,r2 */
 goto L8c0949c4;
L8c0949c4: r[7]=r[13]; /* mov r13,r7 */
 goto L8c0949c6;
L8c0949c6: r[5]=r[2]; /* mov r2,r5 */
 goto L8c0949c8;
L8c0949c8: r[5]+=4294967295u; /* add #-0x1,r5 */
 goto L8c0949ca;
L8c0949ca: r[6]=0u; /* mov #0x0,r6 */
 goto L8c0949cc;
L8c0949cc: *pr=0x8c0949d0u;r[4]=rd(m,r[15]+0x4u,4);status=call(ctx,0x8c093b2au,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093b2a */
 goto L8c0949d0;
L8c0949d0: goto L8c094a5e; /* bra 0x8c094a5e */
 goto L8c0949d4;
L8c0949d4: r[5]=r[2]; /* mov r2,r5 */
 goto L8c0949d6;
L8c0949d6: r[6]=0u; /* mov #0x0,r6 */
 goto L8c0949d8;
L8c0949d8: r[7]=r[13]; /* mov r13,r7 */
 goto L8c0949da;
L8c0949da: *pr=0x8c0949deu;r[4]=r[6];status=call(ctx,0x8c093b2au,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093b2a */
 goto L8c0949de;
L8c0949de: goto L8c094a5e; /* bra 0x8c094a5e */
 goto L8c094a40;
L8c094a40: *sr=(*sr&~1u)|!(r[9]&r[9]); /* tst r9,r9 */
 goto L8c094a42;
L8c094a42: if((*sr&1u))goto L8c094a52; /* bt 0x8c094a52 */
 goto L8c094a44;
L8c094a44: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c094a46;
L8c094a46: r[6]=0u; /* mov #0x0,r6 */
 goto L8c094a48;
L8c094a48: r[7]=r[13]; /* mov r13,r7 */
 goto L8c094a4a;
L8c094a4a: r[5]-=r[9]; /* sub r9,r5 */
 goto L8c094a4c;
L8c094a4c: r[5]+=4294967295u; /* add #-0x1,r5 */
 goto L8c094a4e;
L8c094a4e: r[4]=rd(m,r[15]+0x4u,4);goto L8c094a5a; /* bra 0x8c094a5a */
 goto L8c094a52;
L8c094a52: r[5]=rd(m,r[15],4); /* mov.l @r15,r5 */
 goto L8c094a54;
L8c094a54: r[6]=0u; /* mov #0x0,r6 */
 goto L8c094a56;
L8c094a56: r[4]=r[6]; /* mov r6,r4 */
 goto L8c094a58;
L8c094a58: r[7]=r[13]; /* mov r13,r7 */
 goto L8c094a5a;
L8c094a5a: *pr=0x8c094a5eu;status=call(ctx,0x8c093b2au,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093b2a */
 goto L8c094a5e;
L8c094a5e: r[2]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r2 */
 goto L8c094a60;
L8c094a60: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c094a62;
L8c094a62: if((*sr&1u))goto L8c094a6c; /* bt 0x8c094a6c */
 goto L8c094a64;
L8c094a64: *pr=0x8c094a68u;r[4]=r[13];status=call(ctx,0x8c0947bau,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c0947ba */
 goto L8c094a68;
L8c094a68: r[9]=r[0];goto L8c094a7e; /* bra 0x8c094a7e */
 goto L8c094a6c;
L8c094a6c: r[3]=2u; /* mov #0x2,r3 */
 goto L8c094a6e;
L8c094a6e: r[3]<<=8; /* shll8 r3 */
 goto L8c094a70;
L8c094a70: *sr=(*sr&~1u)|!(r[12]&r[3]); /* tst r12,r3 */
 goto L8c094a72;
L8c094a72: if((*sr&1u))goto L8c094a7c; /* bt 0x8c094a7c */
 goto L8c094a74;
L8c094a74: r[9]=r[10]; /* mov r10,r9 */
 goto L8c094a76;
L8c094a76: r[9]+=r[14]; /* add r14,r9 */
 goto L8c094a78;
L8c094a78: r[9]=rd(m,r[9]+0x18u,4);goto L8c094a7e; /* bra 0x8c094a7e */
 goto L8c094a7c;
L8c094a7c: r[9]=r[11]; /* mov r11,r9 */
 goto L8c094a7e;
L8c094a7e: r[5]=r[13]; /* mov r13,r5 */
 goto L8c094a80;
L8c094a80: *pr=0x8c094a84u;r[4]=r[9];status=call(ctx,0x8c093be0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093be0 */
 goto L8c094a84;
L8c094a84: r[0]=rd(m,0x8c094ae0u,4); /* mov.l 0x8c094ae0,r0 */
 goto L8c094a86;
L8c094a86: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c094a88;
L8c094a88: wr(m,r[3]+r[0],r[9],4); /* mov.l r9,@(r0,r3) */
 goto L8c094a8a;
L8c094a8a: r[3]=8u; /* mov #0x8,r3 */
 goto L8c094a8c;
L8c094a8c: r[3]<<=8; /* shll8 r3 */
 goto L8c094a8e;
L8c094a8e: *sr=(*sr&~1u)|!(r[12]&r[3]); /* tst r12,r3 */
 goto L8c094a90;
L8c094a90: if((*sr&1u))goto L8c094ab0; /* bt 0x8c094ab0 */
 goto L8c094a92;
L8c094a92: r[3]=r[10]; /* mov r10,r3 */
 goto L8c094a94;
L8c094a94: r[3]+=r[14]; /* add r14,r3 */
 goto L8c094a96;
L8c094a96: r[2]=rd(m,r[3]+0x14u,4); /* mov.l @(0x14,r3),r2 */
 goto L8c094a98;
L8c094a98: r[3]=32u; /* mov #0x20,r3 */
 goto L8c094a9a;
L8c094a9a: r[4]=r[13]; /* mov r13,r4 */
 goto L8c094a9c;
L8c094a9c: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]); /* cmp/ge r3,r4 */
 goto L8c094a9e;
L8c094a9e: wr(m,r[15]+0x4u,r[2],4); /* mov.l r2,@(0x4,r15) */
 goto L8c094aa0;
L8c094aa0: if((*sr&1u))goto L8c094ab0; /* bt 0x8c094ab0 */
 goto L8c094aa2;
L8c094aa2: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c094aa4;
L8c094aa4: r[2]=0u; /* mov #0x0,r2 */
 goto L8c094aa6;
L8c094aa6: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[5]); /* cmp/gt r5,r2 */
 goto L8c094aa8;
L8c094aa8: {uint64_t sum=(uint64_t)r[5]+r[2]+(*sr&1u);r[5]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);} /* addc r2,r5 */
 goto L8c094aaa;
L8c094aaa: r[2]=rd(m,0x8c094ae4u,4); /* mov.l 0x8c094ae4,r2 */
 goto L8c094aac;
L8c094aac: target=r[2];*pr=0x8c094ab0u;*sr=(*sr&~1u)|(r[5]&1u);r[5]=(r[5]>>1)|(r[5]&0x80000000u);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c094ab0;
L8c094ab0: r[3]=32u; /* mov #0x20,r3 */
 goto L8c094ab2;
L8c094ab2: r[3]<<=8; /* shll8 r3 */
 goto L8c094ab4;
L8c094ab4: *sr=(*sr&~1u)|!(r[12]&r[3]); /* tst r12,r3 */
 goto L8c094ab6;
L8c094ab6: if((*sr&1u))goto L8c094acc; /* bt 0x8c094acc */
 goto L8c094ab8;
L8c094ab8: r[3]=32u; /* mov #0x20,r3 */
 goto L8c094aba;
L8c094aba: r[5]=r[10]; /* mov r10,r5 */
 goto L8c094abc;
L8c094abc: r[4]=r[13]; /* mov r13,r4 */
 goto L8c094abe;
L8c094abe: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]); /* cmp/ge r3,r4 */
 goto L8c094ac0;
L8c094ac0: r[5]+=r[14]; /* add r14,r5 */
 goto L8c094ac2;
L8c094ac2: take=(*sr&1u);r[5]=rd(m,r[5]+0x20u,4);if(take)goto L8c094acc;goto L8c094ac6; /* bt/s 0x8c094acc */
 goto L8c094ac6;
L8c094ac6: r[2]=rd(m,0x8c094ae8u,4); /* mov.l 0x8c094ae8,r2 */
 goto L8c094ac8;
L8c094ac8: target=r[2];*pr=0x8c094accu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c094acc;
L8c094acc: r[3]=4294967294u; /* mov #-0x2,r3 */
 goto L8c094ace;
L8c094ace: r[12]&=r[3];goto L8c094c0e; /* bra 0x8c094c0e */
 goto L8c094b00;
L8c094b00: r[0]=r[10]; /* mov r10,r0 */
 goto L8c094b02;
L8c094b02: r[0]+=r[14]; /* add r14,r0 */
 goto L8c094b04;
L8c094b04: r[3]=rd(m,r[0]+0x4u,4); /* mov.l @(0x4,r0),r3 */
 goto L8c094b06;
L8c094b06: *sr=(*sr&~1u)|((int32_t)r[3]>0); /* cmp/pl r3 */
 goto L8c094b08;
L8c094b08: if(!(*sr&1u))goto L8c094bd8; /* bf 0x8c094bd8 */
 goto L8c094b0a;
L8c094b0a: r[1]=34u; /* mov #0x22,r1 */
 goto L8c094b0c;
L8c094b0c: *sr=(*sr&~1u)|!(r[12]&r[1]); /* tst r12,r1 */
 goto L8c094b0e;
L8c094b0e: if((*sr&1u))goto L8c094b36; /* bt 0x8c094b36 */
 goto L8c094b10;
L8c094b10: r[2]=rd(m,r[15]+0x14u,4); /* mov.l @(0x14,r15),r2 */
 goto L8c094b12;
L8c094b12: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c094b14;
L8c094b14: if((*sr&1u))goto L8c094b32; /* bt 0x8c094b32 */
 goto L8c094b16;
L8c094b16: *pr=0x8c094b1au;r[4]=r[13];status=call(ctx,0x8c0947bau,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c0947ba */
 goto L8c094b1a;
L8c094b1a: r[9]=r[0]; /* mov r0,r9 */
 goto L8c094b1c;
L8c094b1c: r[0]=rd(m,0x8c094d60u,4); /* mov.l 0x8c094d60,r0 */
 goto L8c094b1e;
L8c094b1e: r[2]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r2 */
 goto L8c094b20;
L8c094b20: r[3]=rd(m,r[2]+r[0],4); /* mov.l @(r0,r2),r3 */
 goto L8c094b22;
L8c094b22: *sr=(*sr&~1u)|(r[9]==r[3]); /* cmp/eq r9,r3 */
 goto L8c094b24;
L8c094b24: if((*sr&1u))goto L8c094b32; /* bt 0x8c094b32 */
 goto L8c094b26;
L8c094b26: r[5]=r[13]; /* mov r13,r5 */
 goto L8c094b28;
L8c094b28: *pr=0x8c094b2cu;r[4]=r[9];status=call(ctx,0x8c093be0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093be0 */
 goto L8c094b2c;
L8c094b2c: r[0]=rd(m,0x8c094d60u,4); /* mov.l 0x8c094d60,r0 */
 goto L8c094b2e;
L8c094b2e: r[2]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r2 */
 goto L8c094b30;
L8c094b30: wr(m,r[2]+r[0],r[9],4); /* mov.l r9,@(r0,r2) */
 goto L8c094b32;
L8c094b32: r[3]=4294967293u; /* mov #-0x3,r3 */
 goto L8c094b34;
L8c094b34: r[12]&=r[3]; /* and r3,r12 */
 goto L8c094b36;
L8c094b36: r[2]=2u; /* mov #0x2,r2 */
 goto L8c094b38;
L8c094b38: r[2]<<=8; /* shll8 r2 */
 goto L8c094b3a;
L8c094b3a: *sr=(*sr&~1u)|!(r[12]&r[2]); /* tst r12,r2 */
 goto L8c094b3c;
L8c094b3c: if((*sr&1u))goto L8c094b5a; /* bt 0x8c094b5a */
 goto L8c094b3e;
L8c094b3e: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c094b40;
L8c094b40: r[9]=r[10]; /* mov r10,r9 */
 goto L8c094b42;
L8c094b42: r[0]=rd(m,0x8c094d60u,4); /* mov.l 0x8c094d60,r0 */
 goto L8c094b44;
L8c094b44: r[9]+=r[14]; /* add r14,r9 */
 goto L8c094b46;
L8c094b46: r[9]=rd(m,r[9]+0x18u,4); /* mov.l @(0x18,r9),r9 */
 goto L8c094b48;
L8c094b48: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c094b4a;
L8c094b4a: *sr=(*sr&~1u)|(r[9]==r[2]); /* cmp/eq r9,r2 */
 goto L8c094b4c;
L8c094b4c: if((*sr&1u))goto L8c094b5a; /* bt 0x8c094b5a */
 goto L8c094b4e;
L8c094b4e: r[5]=r[13]; /* mov r13,r5 */
 goto L8c094b50;
L8c094b50: *pr=0x8c094b54u;r[4]=r[9];status=call(ctx,0x8c093be0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c093be0 */
 goto L8c094b54;
L8c094b54: r[0]=rd(m,0x8c094d60u,4); /* mov.l 0x8c094d60,r0 */
 goto L8c094b56;
L8c094b56: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c094b58;
L8c094b58: wr(m,r[3]+r[0],r[9],4); /* mov.l r9,@(r0,r3) */
 goto L8c094b5a;
L8c094b5a: r[3]=8u; /* mov #0x8,r3 */
 goto L8c094b5c;
L8c094b5c: r[3]<<=8; /* shll8 r3 */
 goto L8c094b5e;
L8c094b5e: *sr=(*sr&~1u)|!(r[12]&r[3]); /* tst r12,r3 */
 goto L8c094b60;
L8c094b60: if((*sr&1u))goto L8c094b88; /* bt 0x8c094b88 */
 goto L8c094b62;
L8c094b62: r[3]=r[10]; /* mov r10,r3 */
 goto L8c094b64;
L8c094b64: r[3]+=r[14]; /* add r14,r3 */
 goto L8c094b66;
L8c094b66: r[2]=rd(m,r[3]+0x14u,4); /* mov.l @(0x14,r3),r2 */
 goto L8c094b68;
L8c094b68: r[3]=32u; /* mov #0x20,r3 */
 goto L8c094b6a;
L8c094b6a: r[4]=r[13]; /* mov r13,r4 */
 goto L8c094b6c;
L8c094b6c: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]); /* cmp/ge r3,r4 */
 goto L8c094b6e;
L8c094b6e: wr(m,r[15]+0x4u,r[2],4); /* mov.l r2,@(0x4,r15) */
 goto L8c094b70;
L8c094b70: if((*sr&1u))goto L8c094b80; /* bt 0x8c094b80 */
 goto L8c094b72;
L8c094b72: r[5]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r5 */
 goto L8c094b74;
L8c094b74: r[2]=0u; /* mov #0x0,r2 */
 goto L8c094b76;
L8c094b76: *sr=(*sr&~1u)|((int32_t)r[2]>(int32_t)r[5]); /* cmp/gt r5,r2 */
 goto L8c094b78;
L8c094b78: {uint64_t sum=(uint64_t)r[5]+r[2]+(*sr&1u);r[5]=(uint32_t)sum;*sr=(*sr&~1u)|(uint32_t)(sum>>32);} /* addc r2,r5 */
 goto L8c094b7a;
L8c094b7a: r[2]=rd(m,0x8c094d64u,4); /* mov.l 0x8c094d64,r2 */
 goto L8c094b7c;
L8c094b7c: target=r[2];*pr=0x8c094b80u;*sr=(*sr&~1u)|(r[5]&1u);r[5]=(r[5]>>1)|(r[5]&0x80000000u);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c094b80;
L8c094b80: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c094b82;
L8c094b82: r[3]<<=8; /* shll8 r3 */
 goto L8c094b84;
L8c094b84: r[3]+=4294967295u; /* add #-0x1,r3 */
 goto L8c094b86;
L8c094b86: r[12]&=r[3]; /* and r3,r12 */
 goto L8c094b88;
L8c094b88: r[2]=32u; /* mov #0x20,r2 */
 goto L8c094b8a;
L8c094b8a: r[2]<<=8; /* shll8 r2 */
 goto L8c094b8c;
L8c094b8c: *sr=(*sr&~1u)|!(r[12]&r[2]); /* tst r12,r2 */
 goto L8c094b8e;
L8c094b8e: if((*sr&1u))goto L8c094bac; /* bt 0x8c094bac */
 goto L8c094b90;
L8c094b90: r[3]=32u; /* mov #0x20,r3 */
 goto L8c094b92;
L8c094b92: r[5]=r[10]; /* mov r10,r5 */
 goto L8c094b94;
L8c094b94: r[4]=r[13]; /* mov r13,r4 */
 goto L8c094b96;
L8c094b96: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[3]); /* cmp/ge r3,r4 */
 goto L8c094b98;
L8c094b98: r[5]+=r[14]; /* add r14,r5 */
 goto L8c094b9a;
L8c094b9a: take=(*sr&1u);r[5]=rd(m,r[5]+0x20u,4);if(take)goto L8c094ba4;goto L8c094b9e; /* bt/s 0x8c094ba4 */
 goto L8c094b9e;
L8c094b9e: r[2]=rd(m,0x8c094d68u,4); /* mov.l 0x8c094d68,r2 */
 goto L8c094ba0;
L8c094ba0: target=r[2];*pr=0x8c094ba4u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c094ba4;
L8c094ba4: r[3]=4294967264u; /* mov #-0x20,r3 */
 goto L8c094ba6;
L8c094ba6: r[3]<<=8; /* shll8 r3 */
 goto L8c094ba8;
L8c094ba8: r[3]+=4294967295u; /* add #-0x1,r3 */
 goto L8c094baa;
L8c094baa: r[12]&=r[3]; /* and r3,r12 */
 goto L8c094bac;
L8c094bac: r[2]=16u; /* mov #0x10,r2 */
 goto L8c094bae;
L8c094bae: r[2]<<=8; /* shll8 r2 */
 goto L8c094bb0;
L8c094bb0: *sr=(*sr&~1u)|!(r[12]&r[2]); /* tst r12,r2 */
 goto L8c094bb2;
L8c094bb2: if((*sr&1u))goto L8c094bce; /* bt 0x8c094bce */
 goto L8c094bb4;
L8c094bb4: r[1]=r[10]; /* mov r10,r1 */
 goto L8c094bb6;
L8c094bb6: r[1]+=r[14]; /* add r14,r1 */
 goto L8c094bb8;
L8c094bb8: r[3]=rd(m,r[1]+0x30u,4); /* mov.l @(0x30,r1),r3 */
 goto L8c094bba;
L8c094bba: *sr=(*sr&~1u)|((int32_t)r[3]>=0); /* cmp/pz r3 */
 goto L8c094bbc;
L8c094bbc: if(!(*sr&1u))goto L8c094bce; /* bf 0x8c094bce */
 goto L8c094bbe;
L8c094bbe: *pr=0x8c094bc2u;r[4]=r[13];status=call(ctx,0x8c0955c0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c0955c0 */
 goto L8c094bc2;
L8c094bc2: r[2]=r[10]; /* mov r10,r2 */
 goto L8c094bc4;
L8c094bc4: r[2]+=r[14]; /* add r14,r2 */
 goto L8c094bc6;
L8c094bc6: r[4]=r[0]; /* mov r0,r4 */
 goto L8c094bc8;
L8c094bc8: r[0]=rd(m,0x8c094d6cu,4); /* mov.l 0x8c094d6c,r0 */
 goto L8c094bca;
L8c094bca: r[3]=rd(m,r[2]+0x30u,4); /* mov.l @(0x30,r2),r3 */
 goto L8c094bcc;
L8c094bcc: wr(m,r[3]+r[0],r[4],1); /* mov.b r4,@(r0,r3) */
 goto L8c094bce;
L8c094bce: r[3]=r[10]; /* mov r10,r3 */
 goto L8c094bd0;
L8c094bd0: r[3]+=r[14]; /* add r14,r3 */
 goto L8c094bd2;
L8c094bd2: r[2]=rd(m,r[3]+0x4u,4); /* mov.l @(0x4,r3),r2 */
 goto L8c094bd4;
L8c094bd4: r[2]+=4294967295u; /* add #-0x1,r2 */
 goto L8c094bd6;
L8c094bd6: wr(m,r[3]+0x4u,r[2],4); /* mov.l r2,@(0x4,r3) */
 goto L8c094bd8;
L8c094bd8: r[3]=r[10]; /* mov r10,r3 */
 goto L8c094bda;
L8c094bda: r[3]+=r[14]; /* add r14,r3 */
 goto L8c094bdc;
L8c094bdc: r[2]=rd(m,r[3]+0x4u,4); /* mov.l @(0x4,r3),r2 */
 goto L8c094bde;
L8c094bde: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c094be0;
L8c094be0: if(!(*sr&1u))goto L8c094c0e; /* bf 0x8c094c0e */
 goto L8c094be2;
L8c094be2: r[2]=16u; /* mov #0x10,r2 */
 goto L8c094be4;
L8c094be4: *sr=(*sr&~1u)|!(r[2]&r[12]); /* tst r2,r12 */
 goto L8c094be6;
L8c094be6: if((*sr&1u))goto L8c094bee; /* bt 0x8c094bee */
 goto L8c094be8;
L8c094be8: r[1]=rd(m,0x8c094d70u,4); /* mov.l 0x8c094d70,r1 */
 goto L8c094bea;
L8c094bea: target=r[1];*pr=0x8c094beeu;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c094bee;
L8c094bee: r[3]=r[10]; /* mov r10,r3 */
 goto L8c094bf0;
L8c094bf0: r[3]+=r[14]; /* add r14,r3 */
 goto L8c094bf2;
L8c094bf2: r[1]=rd(m,r[3]+0xcu,4); /* mov.l @(0xc,r3),r1 */
 goto L8c094bf4;
L8c094bf4: r[2]=16u; /* mov #0x10,r2 */
 goto L8c094bf6;
L8c094bf6: r[2]<<=8; /* shll8 r2 */
 goto L8c094bf8;
L8c094bf8: *sr=(*sr&~1u)|!(r[2]&r[1]); /* tst r2,r1 */
 goto L8c094bfa;
L8c094bfa: if((*sr&1u))goto L8c094c00; /* bt 0x8c094c00 */
 goto L8c094bfc;
L8c094bfc: *pr=0x8c094c00u;r[4]=r[13];status=call(ctx,0x8c09550eu,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c09550e */
 goto L8c094c00;
L8c094c00: r[4]=r[10]; /* mov r10,r4 */
 goto L8c094c02;
L8c094c02: r[3]=4294967295u; /* mov #-0x1,r3 */
 goto L8c094c04;
L8c094c04: r[4]+=r[14]; /* add r14,r4 */
 goto L8c094c06;
L8c094c06: r[12]=r[11]; /* mov r11,r12 */
 goto L8c094c08;
L8c094c08: wr(m,r[4]+0x8u,r[11],4); /* mov.l r11,@(0x8,r4) */
 goto L8c094c0a;
L8c094c0a: wr(m,r[4]+0x10u,r[3],4); /* mov.l r3,@(0x10,r4) */
 goto L8c094c0c;
L8c094c0c: wr(m,r[4]+0x4u,r[12],4); /* mov.l r12,@(0x4,r4) */
 goto L8c094c0e;
L8c094c0e: r[3]=rd(m,0x8c094d74u,4); /* mov.l 0x8c094d74,r3 */
 goto L8c094c10;
L8c094c10: r[2]=r[10]; /* mov r10,r2 */
 goto L8c094c12;
L8c094c12: r[2]+=r[14]; /* add r14,r2 */
 goto L8c094c14;
L8c094c14: wr(m,r[2]+0xcu,r[12],4); /* mov.l r12,@(0xc,r2) */
 goto L8c094c16;
L8c094c16: r[0]=rd(m,r[3],4); /* mov.l @r3,r0 */
 goto L8c094c18;
L8c094c18: *sr=(*sr&~1u)|(r[0]==0x3u); /* cmp/eq #0x3,r0 */
 goto L8c094c1a;
L8c094c1a: if(!(*sr&1u))goto L8c094c66; /* bf 0x8c094c66 */
 goto L8c094c1c;
L8c094c1c: r[12]=r[10]; /* mov r10,r12 */
 goto L8c094c1e;
L8c094c1e: r[12]+=r[14]; /* add r14,r12 */
 goto L8c094c20;
L8c094c20: r[4]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r4 */
 goto L8c094c22;
L8c094c22: r[6]=1u; /* mov #0x1,r6 */
 goto L8c094c24;
L8c094c24: r[1]=rd(m,0x8c094d7cu,4); /* mov.l 0x8c094d7c,r1 */
 goto L8c094c26;
L8c094c26: r[2]=rd(m,0x8c094d78u,4); /* mov.l 0x8c094d78,r2 */
 goto L8c094c28;
L8c094c28: r[5]=rd(m,r[12],4); /* mov.l @r12,r5 */
 goto L8c094c2a;
L8c094c2a: target=r[1];*pr=0x8c094c2eu;r[4]|=r[2];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c094c2e;
L8c094c2e: r[4]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r4 */
 goto L8c094c30;
L8c094c30: r[6]=3u; /* mov #0x3,r6 */
 goto L8c094c32;
L8c094c32: r[2]=rd(m,0x8c094d7cu,4); /* mov.l 0x8c094d7c,r2 */
 goto L8c094c34;
L8c094c34: r[3]=rd(m,0x8c094d80u,4); /* mov.l 0x8c094d80,r3 */
 goto L8c094c36;
L8c094c36: r[5]=rd(m,r[12]+0x4u,4); /* mov.l @(0x4,r12),r5 */
 goto L8c094c38;
L8c094c38: target=r[2];*pr=0x8c094c3cu;r[4]|=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c094c3c;
L8c094c3c: r[4]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r4 */
 goto L8c094c3e;
L8c094c3e: r[6]=2u; /* mov #0x2,r6 */
 goto L8c094c40;
L8c094c40: r[2]=rd(m,0x8c094d7cu,4); /* mov.l 0x8c094d7c,r2 */
 goto L8c094c42;
L8c094c42: r[3]=rd(m,0x8c094d84u,4); /* mov.l 0x8c094d84,r3 */
 goto L8c094c44;
L8c094c44: r[5]=rd(m,r[12]+0x10u,4); /* mov.l @(0x10,r12),r5 */
 goto L8c094c46;
L8c094c46: target=r[2];*pr=0x8c094c4au;r[4]|=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c094c4a;
L8c094c4a: r[4]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r4 */
 goto L8c094c4c;
L8c094c4c: r[6]=2u; /* mov #0x2,r6 */
 goto L8c094c4e;
L8c094c4e: r[2]=rd(m,0x8c094d8cu,4); /* mov.l 0x8c094d8c,r2 */
 goto L8c094c50;
L8c094c50: r[5]=r[9]; /* mov r9,r5 */
 goto L8c094c52;
L8c094c52: r[3]=rd(m,0x8c094d88u,4); /* mov.l 0x8c094d88,r3 */
 goto L8c094c54;
L8c094c54: target=r[2];*pr=0x8c094c58u;r[4]|=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c094c58;
L8c094c58: r[4]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r4 */
 goto L8c094c5a;
L8c094c5a: r[6]=6u; /* mov #0x6,r6 */
 goto L8c094c5c;
L8c094c5c: r[2]=rd(m,0x8c094d8cu,4); /* mov.l 0x8c094d8c,r2 */
 goto L8c094c5e;
L8c094c5e: r[3]=rd(m,0x8c094d90u,4); /* mov.l 0x8c094d90,r3 */
 goto L8c094c60;
L8c094c60: r[5]=rd(m,r[12]+0x8u,4); /* mov.l @(0x8,r12),r5 */
 goto L8c094c62;
L8c094c62: target=r[2];*pr=0x8c094c66u;r[4]|=r[3];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c094c66;
L8c094c66: r[3]=38u; /* mov #0x26,r3 */
 goto L8c094c68;
L8c094c68: r[13]+=1u; /* add #0x1,r13 */
 goto L8c094c6a;
L8c094c6a: *sr=(*sr&~1u)|((int32_t)r[13]>=(int32_t)r[3]); /* cmp/ge r3,r13 */
 goto L8c094c6c;
L8c094c6c: if((*sr&1u))goto L8c094c72; /* bt 0x8c094c72 */
 goto L8c094c6e;
L8c094c6e: goto L8c0948dc; /* bra 0x8c0948dc */
 goto L8c094c72;
L8c094c72: r[6]=rd(m,0x8c094d94u,4); /* mov.l 0x8c094d94,r6 */
 goto L8c094c74;
L8c094c74: r[14]=4294967168u; /* mov #-0x80,r14 */
 goto L8c094c76;
L8c094c76: r[4]=rd(m,0x8c094d6cu,4); /* mov.l 0x8c094d6c,r4 */
 goto L8c094c78;
L8c094c78: r[14]=r[14]&255u; /* extu.b r14,r14 */
 goto L8c094c7a;
L8c094c7a: r[5]=r[6]; /* mov r6,r5 */
 goto L8c094c7c;
L8c094c7c: r[7]=r[11]; /* mov r11,r7 */
 goto L8c094c7e;
L8c094c7e: r[13]=r[11]; /* mov r11,r13 */
 goto L8c094c80;
L8c094c80: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c094c82;
L8c094c82: r[0]=rd(m,0x8c094d6cu,4); /* mov.l 0x8c094d6c,r0 */
 goto L8c094c84;
L8c094c84: r[2]=sx(m,r[5],1); /* mov.b @r5,r2 */
 goto L8c094c86;
L8c094c86: r[3]=sx(m,r[7]+r[0],1); /* mov.b @(r0,r7),r3 */
 goto L8c094c88;
L8c094c88: *sr=(*sr&~1u)|(r[2]==r[3]); /* cmp/eq r2,r3 */
 goto L8c094c8a;
L8c094c8a: take=(*sr&1u);r[7]+=1u;if(take)goto L8c094c9a;goto L8c094c8e; /* bt/s 0x8c094c9a */
 goto L8c094c8e;
L8c094c8e: r[2]=sx(m,r[4],1); /* mov.b @r4,r2 */
 goto L8c094c90;
L8c094c90: r[13]=r[8]; /* mov r8,r13 */
 goto L8c094c92;
L8c094c92: wr(m,r[6],r[2],1); /* mov.b r2,@r6 */
 goto L8c094c94;
L8c094c94: r[3]=sx(m,r[4],1); /* mov.b @r4,r3 */
 goto L8c094c96;
L8c094c96: r[3]|=r[14]; /* or r14,r3 */
 goto L8c094c98;
L8c094c98: wr(m,r[4],r[3],1); /* mov.b r3,@r4 */
 goto L8c094c9a;
L8c094c9a: r[5]+=1u; /* add #0x1,r5 */
 goto L8c094c9c;
L8c094c9c: r[3]=sx(m,r[7]+r[0],1); /* mov.b @(r0,r7),r3 */
 goto L8c094c9e;
L8c094c9e: r[2]=sx(m,r[5],1); /* mov.b @r5,r2 */
 goto L8c094ca0;
L8c094ca0: r[4]+=1u; /* add #0x1,r4 */
 goto L8c094ca2;
L8c094ca2: *sr=(*sr&~1u)|(r[2]==r[3]); /* cmp/eq r2,r3 */
 goto L8c094ca4;
L8c094ca4: take=(*sr&1u);r[6]+=1u;if(take)goto L8c094cb4;goto L8c094ca8; /* bt/s 0x8c094cb4 */
 goto L8c094ca8;
L8c094ca8: r[2]=sx(m,r[4],1); /* mov.b @r4,r2 */
 goto L8c094caa;
L8c094caa: r[13]=r[8]; /* mov r8,r13 */
 goto L8c094cac;
L8c094cac: wr(m,r[6],r[2],1); /* mov.b r2,@r6 */
 goto L8c094cae;
L8c094cae: r[3]=sx(m,r[4],1); /* mov.b @r4,r3 */
 goto L8c094cb0;
L8c094cb0: r[3]|=r[14]; /* or r14,r3 */
 goto L8c094cb2;
L8c094cb2: wr(m,r[4],r[3],1); /* mov.b r3,@r4 */
 goto L8c094cb4;
L8c094cb4: r[5]+=1u; /* add #0x1,r5 */
 goto L8c094cb6;
L8c094cb6: r[7]+=1u; /* add #0x1,r7 */
 goto L8c094cb8;
L8c094cb8: r[2]=sx(m,r[5],1); /* mov.b @r5,r2 */
 goto L8c094cba;
L8c094cba: r[3]=sx(m,r[7]+r[0],1); /* mov.b @(r0,r7),r3 */
 goto L8c094cbc;
L8c094cbc: r[4]+=1u; /* add #0x1,r4 */
 goto L8c094cbe;
L8c094cbe: *sr=(*sr&~1u)|(r[2]==r[3]); /* cmp/eq r2,r3 */
 goto L8c094cc0;
L8c094cc0: take=(*sr&1u);r[6]+=1u;if(take)goto L8c094cd0;goto L8c094cc4; /* bt/s 0x8c094cd0 */
 goto L8c094cc4;
L8c094cc4: r[2]=sx(m,r[4],1); /* mov.b @r4,r2 */
 goto L8c094cc6;
L8c094cc6: r[13]=r[8]; /* mov r8,r13 */
 goto L8c094cc8;
L8c094cc8: wr(m,r[6],r[2],1); /* mov.b r2,@r6 */
 goto L8c094cca;
L8c094cca: r[3]=sx(m,r[4],1); /* mov.b @r4,r3 */
 goto L8c094ccc;
L8c094ccc: r[3]|=r[14]; /* or r14,r3 */
 goto L8c094cce;
L8c094cce: wr(m,r[4],r[3],1); /* mov.b r3,@r4 */
 goto L8c094cd0;
L8c094cd0: r[5]+=1u; /* add #0x1,r5 */
 goto L8c094cd2;
L8c094cd2: r[7]+=1u; /* add #0x1,r7 */
 goto L8c094cd4;
L8c094cd4: r[2]=sx(m,r[5],1); /* mov.b @r5,r2 */
 goto L8c094cd6;
L8c094cd6: r[3]=sx(m,r[7]+r[0],1); /* mov.b @(r0,r7),r3 */
 goto L8c094cd8;
L8c094cd8: r[4]+=1u; /* add #0x1,r4 */
 goto L8c094cda;
L8c094cda: *sr=(*sr&~1u)|(r[2]==r[3]); /* cmp/eq r2,r3 */
 goto L8c094cdc;
L8c094cdc: take=(*sr&1u);r[6]+=1u;if(take)goto L8c094cec;goto L8c094ce0; /* bt/s 0x8c094cec */
 goto L8c094ce0;
L8c094ce0: r[2]=sx(m,r[4],1); /* mov.b @r4,r2 */
 goto L8c094ce2;
L8c094ce2: r[13]=r[8]; /* mov r8,r13 */
 goto L8c094ce4;
L8c094ce4: wr(m,r[6],r[2],1); /* mov.b r2,@r6 */
 goto L8c094ce6;
L8c094ce6: r[3]=sx(m,r[4],1); /* mov.b @r4,r3 */
 goto L8c094ce8;
L8c094ce8: r[3]|=r[14]; /* or r14,r3 */
 goto L8c094cea;
L8c094cea: wr(m,r[4],r[3],1); /* mov.b r3,@r4 */
 goto L8c094cec;
L8c094cec: r[2]=4u; /* mov #0x4,r2 */
 goto L8c094cee;
L8c094cee: r[7]+=1u; /* add #0x1,r7 */
 goto L8c094cf0;
L8c094cf0: *sr=(*sr&~1u)|((int32_t)r[7]>=(int32_t)r[2]); /* cmp/ge r2,r7 */
 goto L8c094cf2;
L8c094cf2: r[4]+=1u; /* add #0x1,r4 */
 goto L8c094cf4;
L8c094cf4: r[6]+=1u; /* add #0x1,r6 */
 goto L8c094cf6;
L8c094cf6: take=!(*sr&1u);r[5]+=1u;if(take)goto L8c094c82;goto L8c094cfa; /* bf/s 0x8c094c82 */
 goto L8c094cfa;
L8c094cfa: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c094cfc;
L8c094cfc: if((*sr&1u))goto L8c094d06; /* bt 0x8c094d06 */
 goto L8c094cfe;
L8c094cfe: r[3]=rd(m,0x8c094d98u,4); /* mov.l 0x8c094d98,r3 */
 goto L8c094d00;
L8c094d00: r[4]=rd(m,0x8c094d6cu,4); /* mov.l 0x8c094d6c,r4 */
 goto L8c094d02;
L8c094d02: target=r[3];*pr=0x8c094d06u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c094d06;
L8c094d06: r[6]=rd(m,0x8c094d6cu,4); /* mov.l 0x8c094d6c,r6 */
 goto L8c094d08;
L8c094d08: r[5]=r[6]; /* mov r6,r5 */
 goto L8c094d0a;
L8c094d0a: r[5]+=4u; /* add #0x4,r5 */
 goto L8c094d0c;
L8c094d0c: r[4]=r[6]; /* mov r6,r4 */
 goto L8c094d0e;
L8c094d0e: *sr=(*sr&~1u)|(r[4]>=r[5]); /* cmp/hs r5,r4 */
 goto L8c094d10;
L8c094d10: if((*sr&1u))goto L8c094d26; /* bt 0x8c094d26 */
 goto L8c094d12;
L8c094d12: r[0]=sx(m,r[4],1); /* mov.b @r4,r0 */
 goto L8c094d14;
L8c094d14: r[0]&=0x7fu; /* and #0x7f,r0 */
 goto L8c094d16;
L8c094d16: wr(m,r[4],r[0],1); /* mov.b r0,@r4 */
 goto L8c094d18;
L8c094d18: r[4]+=1u; /* add #0x1,r4 */
 goto L8c094d1a;
L8c094d1a: r[0]=sx(m,r[4],1); /* mov.b @r4,r0 */
 goto L8c094d1c;
L8c094d1c: r[0]&=0x7fu; /* and #0x7f,r0 */
 goto L8c094d1e;
L8c094d1e: wr(m,r[4],r[0],1); /* mov.b r0,@r4 */
 goto L8c094d20;
L8c094d20: r[4]+=1u; /* add #0x1,r4 */
 goto L8c094d22;
L8c094d22: *sr=(*sr&~1u)|(r[4]>=r[5]); /* cmp/hs r5,r4 */
 goto L8c094d24;
L8c094d24: if(!(*sr&1u))goto L8c094d12; /* bf 0x8c094d12 */
 goto L8c094d26;
L8c094d26: r[1]=rd(m,0x8c094da0u,4); /* mov.l 0x8c094da0,r1 */
 goto L8c094d28;
L8c094d28: r[2]=rd(m,0x8c094d9cu,4); /* mov.l 0x8c094d9c,r2 */
 goto L8c094d2a;
L8c094d2a: r[3]=rd(m,r[1],4); /* mov.l @r1,r3 */
 goto L8c094d2c;
L8c094d2c: r[4]=rd(m,r[2],4); /* mov.l @r2,r4 */
 goto L8c094d2e;
L8c094d2e: *sr=(*sr&~1u)|(r[3]==r[4]); /* cmp/eq r3,r4 */
 goto L8c094d30;
L8c094d30: if((*sr&1u))goto L8c094d3c; /* bt 0x8c094d3c */
 goto L8c094d32;
L8c094d32: *pr=0x8c094d36u;status=call(ctx,0x8c094dd0u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c094dd0 */
 goto L8c094d36;
L8c094d36: r[3]=rd(m,0x8c094d9cu,4); /* mov.l 0x8c094d9c,r3 */
 goto L8c094d38;
L8c094d38: r[2]=4294967290u; /* mov #-0x6,r2 */
 goto L8c094d3a;
L8c094d3a: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c094d3c;
L8c094d3c: r[15]+=32u; /* add #0x20,r15 */
 goto L8c094d3e;
L8c094d3e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c094d40;
L8c094d40: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c094d42;
L8c094d42: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c094d44;
L8c094d44: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c094d46;
L8c094d46: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c094d48;
L8c094d48: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c094d4a;
L8c094d4a: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c094d4c;
L8c094d4c: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
