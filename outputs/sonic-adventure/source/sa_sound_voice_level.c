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
/* Static C recovery of retail Tikal helper 8c0947ba..8c0948a0.
 * External gameplay/rendering and local subordinate routines remain services;
 * transition 0cb00994 is native. PR=SZ=0, finite normal/zero FCMP. */
SA_STAGE_EXPORT int sa_sound_voice_level(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c0947ba;
L8c0947ba: r[15]-=4;wr(m,r[15],fr[15],4); /* fmov.s fr15,@-r15 */
 goto L8c0947bc;
L8c0947bc: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c0947be;
L8c0947be: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c0947c0;
L8c0947c0: r[3]=rd(m,0x8c094870u,4); /* mov.l 0x8c094870,r3 */
 goto L8c0947c2;
L8c0947c2: wr(m,r[15],r[4],4); /* mov.l r4,@r15 */
 goto L8c0947c4;
L8c0947c4: r[4]=rd(m,r[3],4); /* mov.l @r3,r4 */
 goto L8c0947c6;
L8c0947c6: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c0947c8;
L8c0947c8: take=(*sr&1u);fr[15]=0;if(take)goto L8c094800;goto L8c0947cc; /* bt/s 0x8c094800 */
 goto L8c0947cc;
L8c0947cc: r[1]=rd(m,r[15],4); /* mov.l @r15,r1 */
 goto L8c0947ce;
L8c0947ce: r[2]=52u; /* mov #0x34,r2 */
 goto L8c0947d0;
L8c0947d0: r[0]=32u; /* mov #0x20,r0 */
 goto L8c0947d2;
L8c0947d2: *macl=r[2]*r[1]; /* mul.l r2,r1 */
 goto L8c0947d4;
L8c0947d4: r[1]=rd(m,0x8c09486cu,4); /* mov.l 0x8c09486c,r1 */
 goto L8c0947d6;
L8c0947d6: fr[3]=rd(m,r[0]+r[4],4); /* fmov.s @(r0,r4),fr3 */
 goto L8c0947d8;
L8c0947d8: r[0]=36u; /* mov #0x24,r0 */
 goto L8c0947da;
L8c0947da: r[5]=*macl; /* sts MACL,r5 */
 goto L8c0947dc;
L8c0947dc: r[5]+=r[1]; /* add r1,r5 */
 goto L8c0947de;
L8c0947de: fr[5]=rd(m,r[0]+r[5],4); /* fmov.s @(r0,r5),fr5 */
 goto L8c0947e0;
L8c0947e0: status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],1);if(status)return status; /* fsub fr3,fr5 */
 goto L8c0947e2;
L8c0947e2: fr[3]=rd(m,r[0]+r[4],4); /* fmov.s @(r0,r4),fr3 */
 goto L8c0947e4;
L8c0947e4: r[0]=40u; /* mov #0x28,r0 */
 goto L8c0947e6;
L8c0947e6: fr[6]=rd(m,r[0]+r[5],4); /* fmov.s @(r0,r5),fr6 */
 goto L8c0947e8;
L8c0947e8: status=sa_sh4_add_sub(&fr[6],&fr[3],&f->fpscr,&fr[6],1);if(status)return status; /* fsub fr3,fr6 */
 goto L8c0947ea;
L8c0947ea: fr[3]=rd(m,r[0]+r[4],4); /* fmov.s @(r0,r4),fr3 */
 goto L8c0947ec;
L8c0947ec: r[0]=44u; /* mov #0x2c,r0 */
 goto L8c0947ee;
L8c0947ee: status=sa_sh4_multiply(&fr[5],&fr[5],&f->fpscr,&fr[5]);if(status)return status; /* fmul fr5,fr5 */
 goto L8c0947f0;
L8c0947f0: fr[7]=rd(m,r[0]+r[5],4); /* fmov.s @(r0,r5),fr7 */
 goto L8c0947f2;
L8c0947f2: status=sa_sh4_multiply(&fr[6],&fr[6],&f->fpscr,&fr[6]);if(status)return status; /* fmul fr6,fr6 */
 goto L8c0947f4;
L8c0947f4: status=sa_sh4_add_sub(&fr[7],&fr[3],&f->fpscr,&fr[7],1);if(status)return status; /* fsub fr3,fr7 */
 goto L8c0947f6;
L8c0947f6: status=sa_sh4_add_sub(&fr[5],&fr[6],&f->fpscr,&fr[5],0);if(status)return status; /* fadd fr6,fr5 */
 goto L8c0947f8;
L8c0947f8: status=sa_sh4_multiply(&fr[7],&fr[7],&f->fpscr,&fr[7]);if(status)return status; /* fmul fr7,fr7 */
 goto L8c0947fa;
L8c0947fa: fr[4]=fr[5]; /* fmov fr5,fr4 */
 goto L8c0947fc;
L8c0947fc: status=sa_sh4_add_sub(&fr[4],&fr[7],&f->fpscr,&fr[4],0);if(status)return status;goto L8c094802; /* bra 0x8c094802 */
 goto L8c094800;
L8c094800: fr[4]=fr[15]; /* fmov fr15,fr4 */
 goto L8c094802;
L8c094802: r[0]=0x8c094874u; /* mova 0x8c094874,r0 */
 goto L8c094804;
L8c094804: fr[3]=rd(m,r[0],4); /* fmov.s @r0,fr3 */
 goto L8c094806;
L8c094806: status=gt(f,sr,fr[3],fr[4]);if(status)return status; /* fcmp/gt fr4,fr3 */
 goto L8c094808;
L8c094808: if(!(*sr&1u))goto L8c09480e; /* bf 0x8c09480e */
 goto L8c09480a;
L8c09480a: r[4]=0u;goto L8c094836; /* bra 0x8c094836 */
 goto L8c09480e;
L8c09480e: r[3]=rd(m,0x8c094878u,4); /* mov.l 0x8c094878,r3 */
 goto L8c094810;
L8c094810: target=r[3];*pr=0x8c094814u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c094814;
L8c094814: r[0]=0x8c09487cu; /* mova 0x8c09487c,r0 */
 goto L8c094816;
L8c094816: fr[4]=fr[0]; /* fmov fr0,fr4 */
 goto L8c094818;
L8c094818: fr[3]=rd(m,r[0],4); /* fmov.s @r0,fr3 */
 goto L8c09481a;
L8c09481a: r[0]=0x8c094880u; /* mova 0x8c094880,r0 */
 goto L8c09481c;
L8c09481c: fr[2]=rd(m,r[0],4); /* fmov.s @r0,fr2 */
 goto L8c09481e;
L8c09481e: status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status; /* fadd fr3,fr4 */
 goto L8c094820;
L8c094820: fr[1]=0; /* fldi0 fr1 */
 goto L8c094822;
L8c094822: status=sa_sh4_multiply(&fr[4],&fr[2],&f->fpscr,&fr[4]);if(status)return status; /* fmul fr2,fr4 */
 goto L8c094824;
L8c094824: status=gt(f,sr,fr[1],fr[4]);if(status)return status; /* fcmp/gt fr4,fr1 */
 goto L8c094826;
L8c094826: if(!(*sr&1u))goto L8c09482a; /* bf 0x8c09482a */
 goto L8c094828;
L8c094828: fr[4]=fr[15]; /* fmov fr15,fr4 */
 goto L8c09482a;
L8c09482a: r[0]=0x8c094884u; /* mova 0x8c094884,r0 */
 goto L8c09482c;
L8c09482c: fr[3]=rd(m,r[0],4); /* fmov.s @r0,fr3 */
 goto L8c09482e;
L8c09482e: status=sa_sh4_multiply(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status; /* fmul fr3,fr4 */
 goto L8c094830;
L8c094830: status=sa_sh4_add_sub(&fr[15],&fr[4],&f->fpscr,&fr[15],1);if(status)return status; /* fsub fr4,fr15 */
 goto L8c094832;
L8c094832: status=sa_sh4_truncate_i32(fr[15],&f->fpscr,fpul);if(status)return status; /* ftrc fr15,FPUL */
 goto L8c094834;
L8c094834: r[4]=*fpul; /* sts FPUL,r4 */
 goto L8c094836;
L8c094836: r[2]=rd(m,r[15],4); /* mov.l @r15,r2 */
 goto L8c094838;
L8c094838: r[3]=52u; /* mov #0x34,r3 */
 goto L8c09483a;
L8c09483a: r[1]=rd(m,0x8c09486cu,4); /* mov.l 0x8c09486c,r1 */
 goto L8c09483c;
L8c09483c: r[6]=4294967169u; /* mov #-0x7f,r6 */
 goto L8c09483e;
L8c09483e: *macl=r[3]*r[2]; /* mul.l r3,r2 */
 goto L8c094840;
L8c094840: r[5]=*macl; /* sts MACL,r5 */
 goto L8c094842;
L8c094842: r[5]+=r[1]; /* add r1,r5 */
 goto L8c094844;
L8c094844: r[2]=rd(m,r[5]+0x18u,4); /* mov.l @(0x18,r5),r2 */
 goto L8c094846;
L8c094846: r[4]+=r[2]; /* add r2,r4 */
 goto L8c094848;
L8c094848: *sr=(*sr&~1u)|((int32_t)r[4]>=(int32_t)r[6]); /* cmp/ge r6,r4 */
 goto L8c09484a;
L8c09484a: take=(*sr&1u);r[3]=rd(m,r[5]+0x1cu,4);if(take)goto L8c094850;goto L8c09484e; /* bt/s 0x8c094850 */
 goto L8c09484e;
L8c09484e: r[4]=r[6]; /* mov r6,r4 */
 goto L8c094850;
L8c094850: *sr=(*sr&~1u)|((int32_t)r[4]>(int32_t)r[3]); /* cmp/gt r3,r4 */
 goto L8c094852;
L8c094852: if(!(*sr&1u))goto L8c094856; /* bf 0x8c094856 */
 goto L8c094854;
L8c094854: r[4]=rd(m,r[5]+0x1cu,4); /* mov.l @(0x1c,r5),r4 */
 goto L8c094856;
L8c094856: r[0]=r[4]; /* mov r4,r0 */
 goto L8c094858;
L8c094858: r[15]+=4u; /* add #0x4,r15 */
 goto L8c09485a;
L8c09485a: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c09485c;
L8c09485c: fr[15]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
