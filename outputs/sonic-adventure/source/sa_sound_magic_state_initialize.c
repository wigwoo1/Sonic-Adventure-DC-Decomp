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
/* Retail magic8000 stream-state initializer667488..667596.
 * Metadata/channel failures return0 with partial state updates retained.
 * Configure/parameter lower returns ignored; initializes format0 on success. */
SA_STAGE_EXPORT int sa_sound_magic_state_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c667488;
L8c667488: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c66748a;
L8c66748a: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c66748c;
L8c66748c: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c66748e;
L8c66748e: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c667490;
L8c667490: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c667492;
L8c667492: r[14]=r[4]; /* mov r4,r14 */
 goto L8c667494;
L8c667494: r[3]=r[14]; /* mov r14,r3 */
 goto L8c667496;
L8c667496: r[3]+=16u; /* add #0x10,r3 */
 goto L8c667498;
L8c667498: r[2]=r[14]; /* mov r14,r2 */
 goto L8c66749a;
L8c66749a: r[2]+=24u; /* add #0x18,r2 */
 goto L8c66749c;
L8c66749c: r[13]=r[6]; /* mov r6,r13 */
 goto L8c66749e;
L8c66749e: r[7]=r[14]; /* mov r14,r7 */
 goto L8c6674a0;
L8c6674a0: r[0]=1u; /* mov #0x1,r0 */
 goto L8c6674a2;
L8c6674a2: r[12]=r[5]; /* mov r5,r12 */
 goto L8c6674a4;
L8c6674a4: r[5]=r[13]; /* mov r13,r5 */
 goto L8c6674a6;
L8c6674a6: wr(m,r[14]+0x2u,r[0],2); /* mov.w r0,@(0x2,r14) */
 goto L8c6674a8;
L8c6674a8: r[7]+=12u; /* add #0xc,r7 */
 goto L8c6674aa;
L8c6674aa: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c6674ac;
L8c6674ac: r[3]=r[14]; /* mov r14,r3 */
 goto L8c6674ae;
L8c6674ae: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c6674b0;
L8c6674b0: r[2]=r[14]; /* mov r14,r2 */
 goto L8c6674b2;
L8c6674b2: r[3]+=20u; /* add #0x14,r3 */
 goto L8c6674b4;
L8c6674b4: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c6674b6;
L8c6674b6: r[2]+=14u; /* add #0xe,r2 */
 goto L8c6674b8;
L8c6674b8: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c6674ba;
L8c6674ba: r[3]=r[14]; /* mov r14,r3 */
 goto L8c6674bc;
L8c6674bc: r[3]+=15u; /* add #0xf,r3 */
 goto L8c6674be;
L8c6674be: r[2]=r[14]; /* mov r14,r2 */
 goto L8c6674c0;
L8c6674c0: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c6674c2;
L8c6674c2: r[2]+=13u; /* add #0xd,r2 */
 goto L8c6674c4;
L8c6674c4: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c6674c6;
L8c6674c6: r[3]=rd(m,0x8c667650u,4); /* mov.l 0x8c667650,r3 */
 goto L8c6674c8;
L8c6674c8: r[6]=r[15]; /* mov r15,r6 */
 goto L8c6674ca;
L8c6674ca: r[6]+=24u; /* add #0x18,r6 */
 goto L8c6674cc;
L8c6674cc: target=r[3];*pr=0x8c6674d0u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6674d0;
L8c6674d0: r[4]=r[0]; /* mov r0,r4 */
 goto L8c6674d2;
L8c6674d2: *sr=(*sr&~1u)|((int32_t)r[4]>=0); /* cmp/pz r4 */
 goto L8c6674d4;
L8c6674d4: take=!(*sr&1u);r[15]+=24u;if(take)goto L8c6674ea;goto L8c6674d8; /* bf/s 0x8c6674ea */
 goto L8c6674d8;
L8c6674d8: r[2]=rd(m,0x8c667654u,4); /* mov.l 0x8c667654,r2 */
 goto L8c6674da;
L8c6674da: r[6]=r[14]; /* mov r14,r6 */
 goto L8c6674dc;
L8c6674dc: r[5]=r[13]; /* mov r13,r5 */
 goto L8c6674de;
L8c6674de: r[6]+=28u; /* add #0x1c,r6 */
 goto L8c6674e0;
L8c6674e0: target=r[2];*pr=0x8c6674e4u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c6674e4;
L8c6674e4: r[4]=r[0]; /* mov r0,r4 */
 goto L8c6674e6;
L8c6674e6: *sr=(*sr&~1u)|((int32_t)r[4]>=0); /* cmp/pz r4 */
 goto L8c6674e8;
L8c6674e8: if((*sr&1u))goto L8c6674ee; /* bt 0x8c6674ee */
 goto L8c6674ea;
L8c6674ea: r[0]=0u;goto L8c66758a; /* bra 0x8c66758a */
 goto L8c6674ee;
L8c6674ee: r[0]=sx(m,r[14]+0x1cu,2); /* mov.w @(0x1c,r14),r0 */
 goto L8c6674f0;
L8c6674f0: r[5]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r5 */
 goto L8c6674f2;
L8c6674f2: r[3]=rd(m,0x8c667658u,4); /* mov.l 0x8c667658,r3 */
 goto L8c6674f4;
L8c6674f4: r[6]=r[0]; /* mov r0,r6 */
 goto L8c6674f6;
L8c6674f6: target=r[3];*pr=0x8c6674fau;r[4]=rd(m,r[14]+0x8u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c6674fa;
L8c6674fa: r[2]=r[14]; /* mov r14,r2 */
 goto L8c6674fc;
L8c6674fc: r[3]=r[14]; /* mov r14,r3 */
 goto L8c6674fe;
L8c6674fe: r[2]+=52u; /* add #0x34,r2 */
 goto L8c667500;
L8c667500: r[6]=r[14]; /* mov r14,r6 */
 goto L8c667502;
L8c667502: r[3]+=48u; /* add #0x30,r3 */
 goto L8c667504;
L8c667504: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c667506;
L8c667506: r[2]=r[14]; /* mov r14,r2 */
 goto L8c667508;
L8c667508: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c66750a;
L8c66750a: r[3]=r[14]; /* mov r14,r3 */
 goto L8c66750c;
L8c66750c: r[2]+=44u; /* add #0x2c,r2 */
 goto L8c66750e;
L8c66750e: r[3]+=40u; /* add #0x28,r3 */
 goto L8c667510;
L8c667510: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c667512;
L8c667512: r[2]=r[14]; /* mov r14,r2 */
 goto L8c667514;
L8c667514: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c667516;
L8c667516: r[3]=rd(m,0x8c66765cu,4); /* mov.l 0x8c66765c,r3 */
 goto L8c667518;
L8c667518: r[7]=r[14]; /* mov r14,r7 */
 goto L8c66751a;
L8c66751a: r[2]+=38u; /* add #0x26,r2 */
 goto L8c66751c;
L8c66751c: r[5]=r[13]; /* mov r13,r5 */
 goto L8c66751e;
L8c66751e: r[6]+=32u; /* add #0x20,r6 */
 goto L8c667520;
L8c667520: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c667522;
L8c667522: r[7]+=36u; /* add #0x24,r7 */
 goto L8c667524;
L8c667524: target=r[3];*pr=0x8c667528u;r[4]=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c667528;
L8c667528: r[15]+=20u; /* add #0x14,r15 */
 goto L8c66752a;
L8c66752a: r[1]=88u; /* mov #0x58,r1 */
 goto L8c66752c;
L8c66752c: r[0]=sx(m,r[14]+0xeu,1); /* mov.b @(0xe,r14),r0 */
 goto L8c66752e;
L8c66752e: r[1]+=r[14]; /* add r14,r1 */
 goto L8c667530;
L8c667530: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c667532;
L8c667532: r[1]=92u; /* mov #0x5c,r1 */
 goto L8c667534;
L8c667534: r[0]=sx(m,r[14]+0xfu,1); /* mov.b @(0xf,r14),r0 */
 goto L8c667536;
L8c667536: r[1]+=r[14]; /* add r14,r1 */
 goto L8c667538;
L8c667538: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c66753a;
L8c66753a: r[0]=96u; /* mov #0x60,r0 */
 goto L8c66753c;
L8c66753c: r[3]=rd(m,r[14]+0x10u,4); /* mov.l @(0x10,r14),r3 */
 goto L8c66753e;
L8c66753e: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c667540;
L8c667540: r[0]=100u; /* mov #0x64,r0 */
 goto L8c667542;
L8c667542: r[2]=rd(m,r[14]+0x3cu,4); /* mov.l @(0x3c,r14),r2 */
 goto L8c667544;
L8c667544: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c667546;
L8c667546: r[0]=64u; /* mov #0x40,r0 */
 goto L8c667548;
L8c667548: r[3]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r3 */
 goto L8c66754a;
L8c66754a: r[0]=104u; /* mov #0x68,r0 */
 goto L8c66754c;
L8c66754c: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c66754e;
L8c66754e: r[0]=68u; /* mov #0x44,r0 */
 goto L8c667550;
L8c667550: r[2]=rd(m,r[14]+r[0],4); /* mov.l @(r0,r14),r2 */
 goto L8c667552;
L8c667552: r[0]=108u; /* mov #0x6c,r0 */
 goto L8c667554;
L8c667554: r[3]=r[14]; /* mov r14,r3 */
 goto L8c667556;
L8c667556: r[3]+=124u; /* add #0x7c,r3 */
 goto L8c667558;
L8c667558: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c66755a;
L8c66755a: r[0]=72u; /* mov #0x48,r0 */
 goto L8c66755c;
L8c66755c: r[2]=sx(m,r[0]+r[14],2); /* mov.w @(r0,r14),r2 */
 goto L8c66755e;
L8c66755e: r[0]=74u; /* mov #0x4a,r0 */
 goto L8c667560;
L8c667560: wr(m,r[3],r[2],2); /* mov.w r2,@r3 */
 goto L8c667562;
L8c667562: r[2]=r[14]; /* mov r14,r2 */
 goto L8c667564;
L8c667564: r[3]=sx(m,r[0]+r[14],2); /* mov.w @(r0,r14),r3 */
 goto L8c667566;
L8c667566: r[0]=126u; /* mov #0x7e,r0 */
 goto L8c667568;
L8c667568: wr(m,r[0]+r[14],r[3],2); /* mov.w r3,@(r0,r14) */
 goto L8c66756a;
L8c66756a: r[0]+=2u; /* add #0x2,r0 */
 goto L8c66756c;
L8c66756c: r[4]=r[14]; /* mov r14,r4 */
 goto L8c66756e;
L8c66756e: r[2]+=76u; /* add #0x4c,r2 */
 goto L8c667570;
L8c667570: wr(m,r[14]+r[0],r[2],4); /* mov.l r2,@(r0,r14) */
 goto L8c667572;
L8c667572: r[0]+=28u; /* add #0x1c,r0 */
 goto L8c667574;
L8c667574: r[3]=0u; /* mov #0x0,r3 */
 goto L8c667576;
L8c667576: r[2]=r[3]; /* mov r3,r2 */
 goto L8c667578;
L8c667578: wr(m,r[14]+r[0],r[3],4); /* mov.l r3,@(r0,r14) */
 goto L8c66757a;
L8c66757a: r[3]=rd(m,0x8c667660u,4); /* mov.l 0x8c667660,r3 */
 goto L8c66757c;
L8c66757c: r[0]+=12u; /* add #0xc,r0 */
 goto L8c66757e;
L8c66757e: r[5]=r[2]; /* mov r2,r5 */
 goto L8c667580;
L8c667580: wr(m,r[0]+r[14],r[2],2); /* mov.w r2,@(r0,r14) */
 goto L8c667582;
L8c667582: r[6]=4u; /* mov #0x4,r6 */
 goto L8c667584;
L8c667584: target=r[3];*pr=0x8c667588u;r[4]+=76u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c667588;
L8c667588: r[0]=sx(m,r[15],2); /* mov.w @r15,r0 */
 goto L8c66758a;
L8c66758a: r[15]+=4u; /* add #0x4,r15 */
 goto L8c66758c;
L8c66758c: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c66758e;
L8c66758e: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c667590;
L8c667590: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c667592;
L8c667592: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
