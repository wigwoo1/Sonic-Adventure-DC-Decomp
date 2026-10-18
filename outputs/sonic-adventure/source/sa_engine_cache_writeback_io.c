#include "cache_io.h"
#include "fpu_cleanup_transfer.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(const struct sa_input_io *io,uint8_t *m,uint32_t a,unsigned n){uint32_t physical=a&0x1fffffffu;if(physical<0x0c000000u||physical>=0x0d000000u)return io->read(io->user,a,n);uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(const struct sa_input_io *io,uint8_t *m,uint32_t a,unsigned n){uint32_t v=rd(io,m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(const struct sa_input_io *io,uint8_t *m,uint32_t a,uint32_t v,unsigned n){uint32_t physical=a&0x1fffffffu;if(physical<0x0c000000u||physical>=0x0d000000u){io->write(io->user,a,v,n);return;}for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
/* Static C recovery of USA Rev A cache writeback. OCBWB is an explicit host service; hardware-address reads require IO specialization before runtime integration. */
SA_STAGE_EXPORT int sa_engine_cache_writeback_io(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,const struct sa_cache_io *cache,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c5fc58a;
L8c5fc58a: r[15]-=4;wr(&cache->registers,m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c5fc58c;
L8c5fc58c: r[15]-=4;wr(&cache->registers,m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c5fc58e;
L8c5fc58e: r[15]-=4;wr(&cache->registers,m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c5fc590;
L8c5fc590: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c5fc592;
L8c5fc592: r[13]=r[5]; /* mov r5,r13 */
 goto L8c5fc594;
L8c5fc594: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c5fc596;
L8c5fc596: take=!(*sr&1u);r[12]=r[4];if(take)goto L8c5fc5a0;goto L8c5fc59a; /* bf/s 0x8c5fc5a0 */
 goto L8c5fc59a;
L8c5fc59a: r[3]=rd(&cache->registers,m,0x8c5fc5c0u,4); /* mov.l 0x8c5fc5c0,r3 */
 goto L8c5fc59c;
L8c5fc59c: target=r[3];switch(target&0x1fffffffu){case 0xc5fc7d4u:goto L8c5fc7d4;default:return -2;} /* jmp @r3 */
L8c5fc5a0: r[2]=rd(&cache->registers,m,0x8c5fc5c8u,4); /* mov.l 0x8c5fc5c8,r2 */
 goto L8c5fc5a2;
L8c5fc5a2: r[14]=rd(&cache->registers,m,0x8c5fc5c4u,4); /* mov.l 0x8c5fc5c4,r14 */
 goto L8c5fc5a4;
L8c5fc5a4: *sr=(*sr&~1u)|(r[13]>=r[2]); /* cmp/hs r2,r13 */
 goto L8c5fc5a6;
L8c5fc5a6: take=!(*sr&1u);r[14]&=r[12];if(take)goto L8c5fc5b0;goto L8c5fc5aa; /* bf/s 0x8c5fc5b0 */
 goto L8c5fc5aa;
L8c5fc5aa: r[3]=rd(&cache->registers,m,0x8c5fc5ccu,4); /* mov.l 0x8c5fc5cc,r3 */
 goto L8c5fc5ac;
L8c5fc5ac: target=r[3];switch(target&0x1fffffffu){case 0xc5fc5e6u:goto L8c5fc5e6;default:return -2;} /* jmp @r3 */
L8c5fc5b0: r[5]=31u; /* mov #0x1f,r5 */
 goto L8c5fc5b2;
L8c5fc5b2: r[4]=r[14]; /* mov r14,r4 */
 goto L8c5fc5b4;
L8c5fc5b4: r[5]&=r[12]; /* and r12,r5 */
 goto L8c5fc5b6;
L8c5fc5b6: r[5]+=r[13]; /* add r13,r5 */
 goto L8c5fc5b8;
L8c5fc5b8: goto L8c5fc5d0; /* bra 0x8c5fc5d0 */
 goto L8c5fc5d0;
L8c5fc5d0: r[5]+=31u; /* add #0x1f,r5 */
 goto L8c5fc5d2;
L8c5fc5d2: r[5]>>=2; /* shlr2 r5 */
 goto L8c5fc5d4;
L8c5fc5d4: r[5]>>=2; /* shlr2 r5 */
 goto L8c5fc5d6;
L8c5fc5d6: *sr=(*sr&~1u)|(r[5]&1u);r[5]>>=1; /* shlr r5 */
 goto L8c5fc5d8;
L8c5fc5d8: status=cache->writeback(cache->registers.user,r[4]);if(status)return status; /* ocbwb @r4 */
 goto L8c5fc5da;
L8c5fc5da: r[5]--;*sr=(*sr&~1u)|(!r[5]); /* dt r5 */
 goto L8c5fc5dc;
L8c5fc5dc: take=!(*sr&1u);r[4]+=32u;if(take)goto L8c5fc5d8;goto L8c5fc5e0; /* bf/s 0x8c5fc5d8 */
 goto L8c5fc5e0;
L8c5fc5e0: r[2]=rd(&cache->registers,m,0x8c5fc600u,4); /* mov.l 0x8c5fc600,r2 */
 goto L8c5fc5e2;
L8c5fc5e2: target=r[2];switch(target&0x1fffffffu){case 0xc5fc7d4u:goto L8c5fc7d4;default:return -2;} /* jmp @r2 */
L8c5fc5e6: r[0]=*sr; /* stc SR,r0 */
 goto L8c5fc5e8;
L8c5fc5e8: r[3]=sx(&cache->registers,m,0x8c5fc5feu,2); /* mov.w 0x8c5fc5fe,r3 */
 goto L8c5fc5ea;
L8c5fc5ea: r[0]>>=2; /* shlr2 r0 */
 goto L8c5fc5ec;
L8c5fc5ec: r[0]>>=2; /* shlr2 r0 */
 goto L8c5fc5ee;
L8c5fc5ee: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c5fc5f0;
L8c5fc5f0: wr(&cache->registers,m,r[15],r[0],2); /* mov.w r0,@r15 */
 goto L8c5fc5f2;
L8c5fc5f2: r[0]=*sr; /* stc SR,r0 */
 goto L8c5fc5f4;
L8c5fc5f4: r[0]&=r[3]; /* and r3,r0 */
 goto L8c5fc5f6;
L8c5fc5f6: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c5fc5f8;
L8c5fc5f8: *sr=r[0]; /* ldc r0,SR */
 goto L8c5fc5fa;
L8c5fc5fa: goto L8c5fc604; /* bra 0x8c5fc604 */
 goto L8c5fc604;
L8c5fc604: r[0]=0x8c5fc614u; /* mova 0x8c5fc614,r0 */
 goto L8c5fc606;
L8c5fc606: r[1]=rd(&cache->registers,m,0x8c5fc610u,4); /* mov.l 0x8c5fc610,r1 */
 goto L8c5fc608;
L8c5fc608: r[1]|=r[0]; /* or r0,r1 */
 goto L8c5fc60a;
L8c5fc60a: target=r[1];switch(target&0x1fffffffu){case 0xc5fc614u:goto L8c5fc614;default:return -2;} /* jmp @r1 */
L8c5fc614: r[2]=rd(&cache->registers,m,0x8c5fc644u,4); /* mov.l 0x8c5fc644,r2 */
 goto L8c5fc616;
L8c5fc616: r[12]+=r[13]; /* add r13,r12 */
 goto L8c5fc618;
L8c5fc618: r[1]=4294967289u; /* mov #-0x7,r1 */
 goto L8c5fc61a;
L8c5fc61a: r[3]=rd(&cache->registers,m,0x8c5fc640u,4); /* mov.l 0x8c5fc640,r3 */
 goto L8c5fc61c;
L8c5fc61c: r[4]=rd(&cache->registers,m,r[2],4); /* mov.l @r2,r4 */
 goto L8c5fc61e;
L8c5fc61e: r[5]=1u; /* mov #0x1,r5 */
 goto L8c5fc620;
L8c5fc620: r[12]+=4294967295u; /* add #-0x1,r12 */
 goto L8c5fc622;
L8c5fc622: r[13]=r[4]; /* mov r4,r13 */
 goto L8c5fc624;
L8c5fc624: r[13]>>=2; /* shlr2 r13 */
 goto L8c5fc626;
L8c5fc626: r[4]=(r[1]&0x80000000u)?((r[1]&31u)?r[4]>>((0u-r[1])&31u):0u):(r[4]<<(r[1]&31u)); /* shld r1,r4 */
 goto L8c5fc628;
L8c5fc628: r[13]>>=2; /* shlr2 r13 */
 goto L8c5fc62a;
L8c5fc62a: r[4]&=r[5]; /* and r5,r4 */
 goto L8c5fc62c;
L8c5fc62c: *sr=(*sr&~1u)|(r[13]&1u);r[13]>>=1; /* shlr r13 */
 goto L8c5fc62e;
L8c5fc62e: wr(&cache->registers,m,r[15]+0x4u,r[4],4); /* mov.l r4,@(0x4,r15) */
 goto L8c5fc630;
L8c5fc630: r[12]&=r[3]; /* and r3,r12 */
 goto L8c5fc632;
L8c5fc632: r[4]=r[14]; /* mov r14,r4 */
 goto L8c5fc634;
L8c5fc634: r[13]&=r[5]; /* and r5,r13 */
 goto L8c5fc636;
L8c5fc636: r[5]=r[12]; /* mov r12,r5 */
 goto L8c5fc638;
L8c5fc638: r[6]=r[13]; /* mov r13,r6 */
 goto L8c5fc63a;
L8c5fc63a: goto L8c5fc648; /* bra 0x8c5fc648 */
 goto L8c5fc648;
L8c5fc648: r[15]-=4;wr(&cache->registers,m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c5fc64a;
L8c5fc64a: r[15]-=4;wr(&cache->registers,m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c5fc64c;
L8c5fc64c: r[15]-=4;wr(&cache->registers,m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c5fc64e;
L8c5fc64e: r[0]=rd(&cache->registers,m,0x8c5fc668u,4); /* mov.l 0x8c5fc668,r0 */
 goto L8c5fc650;
L8c5fc650: r[8]=rd(&cache->registers,m,0x8c5fc66cu,4); /* mov.l 0x8c5fc66c,r8 */
 goto L8c5fc652;
L8c5fc652: r[9]=rd(&cache->registers,m,0x8c5fc670u,4); /* mov.l 0x8c5fc670,r9 */
 goto L8c5fc654;
L8c5fc654: r[10]=rd(&cache->registers,m,0x8c5fc674u,4); /* mov.l 0x8c5fc674,r10 */
 goto L8c5fc656;
L8c5fc656: r[2]=rd(&cache->registers,m,0x8c5fc678u,4); /* mov.l 0x8c5fc678,r2 */
 goto L8c5fc658;
L8c5fc658: r[3]=rd(&cache->registers,m,0x8c5fc67cu,4); /* mov.l 0x8c5fc67c,r3 */
 goto L8c5fc65a;
L8c5fc65a: r[7]=rd(&cache->registers,m,0x8c5fc680u,4); /* mov.l 0x8c5fc680,r7 */
 goto L8c5fc65c;
L8c5fc65c: r[7]&=r[4]; /* and r4,r7 */
 goto L8c5fc65e;
L8c5fc65e: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc660;
L8c5fc660: r[4]&=r[7]; /* and r7,r4 */
 goto L8c5fc662;
L8c5fc662: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc664;
L8c5fc664: goto L8c5fc6a4; /* bra 0x8c5fc6a4 */
 goto L8c5fc684;
L8c5fc684: r[1]=r[0]; /* mov r0,r1 */
 goto L8c5fc686;
L8c5fc686: r[1]|=r[8]; /* or r8,r1 */
 goto L8c5fc688;
L8c5fc688: r[1]=rd(&cache->registers,m,r[1],4); /* mov.l @r1,r1 */
 goto L8c5fc68a;
L8c5fc68a: r[1]&=r[9]; /* and r9,r1 */
 goto L8c5fc68c;
L8c5fc68c: r[1]|=r[0]; /* or r0,r1 */
 goto L8c5fc68e;
L8c5fc68e: *sr=(*sr&~1u)|(r[4]>r[1]); /* cmp/hi r1,r4 */
 goto L8c5fc690;
L8c5fc690: if((*sr&1u))goto L8c5fc69a; /* bt 0x8c5fc69a */
 goto L8c5fc692;
L8c5fc692: *sr=(*sr&~1u)|(r[1]>r[5]); /* cmp/hi r5,r1 */
 goto L8c5fc694;
L8c5fc694: if((*sr&1u))goto L8c5fc69a; /* bt 0x8c5fc69a */
 goto L8c5fc696;
L8c5fc696: r[1]|=r[7]; /* or r7,r1 */
 goto L8c5fc698;
L8c5fc698: status=cache->writeback(cache->registers.user,r[1]);if(status)return status; /* ocbwb @r1 */
 goto L8c5fc69a;
L8c5fc69a: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c5fc69c;
L8c5fc69c: if((*sr&1u))goto L8c5fc6a2; /* bt 0x8c5fc6a2 */
 goto L8c5fc69e;
L8c5fc69e: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c5fc6a0;
L8c5fc6a0: if((*sr&1u))goto L8c5fc6a8; /* bt 0x8c5fc6a8 */
 goto L8c5fc6a2;
L8c5fc6a2: r[0]+=32u; /* add #0x20,r0 */
 goto L8c5fc6a4;
L8c5fc6a4: *sr=(*sr&~1u)|(r[2]>r[0]); /* cmp/hi r0,r2 */
 goto L8c5fc6a6;
L8c5fc6a6: if((*sr&1u))goto L8c5fc684; /* bt 0x8c5fc684 */
 goto L8c5fc6a8;
L8c5fc6a8: r[8]=rd(&cache->registers,m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c5fc6aa;
L8c5fc6aa: r[9]=rd(&cache->registers,m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c5fc6ac;
L8c5fc6ac: r[10]=rd(&cache->registers,m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c5fc6ae;
L8c5fc6ae:  /* nop */
 goto L8c5fc6b0;
L8c5fc6b0: r[3]=rd(&cache->registers,m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c5fc6b2;
L8c5fc6b2: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c5fc6b4;
L8c5fc6b4: if(!(*sr&1u))goto L8c5fc6bc; /* bf 0x8c5fc6bc */
 goto L8c5fc6b6;
L8c5fc6b6: r[3]=rd(&cache->registers,m,0x8c5fc6c8u,4); /* mov.l 0x8c5fc6c8,r3 */
 goto L8c5fc6b8;
L8c5fc6b8: target=r[3];switch(target&0x1fffffffu){case 0xc5fc746u:goto L8c5fc746;default:return -2;} /* jmp @r3 */
L8c5fc6bc: r[4]=r[14]; /* mov r14,r4 */
 goto L8c5fc6be;
L8c5fc6be: r[5]=r[12]; /* mov r12,r5 */
 goto L8c5fc6c0;
L8c5fc6c0: r[6]=r[13]; /* mov r13,r6 */
 goto L8c5fc6c2;
L8c5fc6c2: goto L8c5fc6cc; /* bra 0x8c5fc6cc */
 goto L8c5fc6cc;
L8c5fc6cc: r[15]-=4;wr(&cache->registers,m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c5fc6ce;
L8c5fc6ce: r[15]-=4;wr(&cache->registers,m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c5fc6d0;
L8c5fc6d0: r[15]-=4;wr(&cache->registers,m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c5fc6d2;
L8c5fc6d2: r[15]-=4;wr(&cache->registers,m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c5fc6d4;
L8c5fc6d4: r[0]=rd(&cache->registers,m,0x8c5fc6f0u,4); /* mov.l 0x8c5fc6f0,r0 */
 goto L8c5fc6d6;
L8c5fc6d6: r[8]=rd(&cache->registers,m,0x8c5fc6f4u,4); /* mov.l 0x8c5fc6f4,r8 */
 goto L8c5fc6d8;
L8c5fc6d8: r[9]=rd(&cache->registers,m,0x8c5fc6f8u,4); /* mov.l 0x8c5fc6f8,r9 */
 goto L8c5fc6da;
L8c5fc6da: r[10]=rd(&cache->registers,m,0x8c5fc6fcu,4); /* mov.l 0x8c5fc6fc,r10 */
 goto L8c5fc6dc;
L8c5fc6dc: r[2]=rd(&cache->registers,m,0x8c5fc700u,4); /* mov.l 0x8c5fc700,r2 */
 goto L8c5fc6de;
L8c5fc6de: r[3]=rd(&cache->registers,m,0x8c5fc704u,4); /* mov.l 0x8c5fc704,r3 */
 goto L8c5fc6e0;
L8c5fc6e0: r[7]=rd(&cache->registers,m,0x8c5fc708u,4); /* mov.l 0x8c5fc708,r7 */
 goto L8c5fc6e2;
L8c5fc6e2: r[7]&=r[4]; /* and r4,r7 */
 goto L8c5fc6e4;
L8c5fc6e4: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc6e6;
L8c5fc6e6: r[4]&=r[7]; /* and r7,r4 */
 goto L8c5fc6e8;
L8c5fc6e8: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc6ea;
L8c5fc6ea: goto L8c5fc734; /* bra 0x8c5fc734 */
 goto L8c5fc70c;
L8c5fc70c: r[1]=r[0]; /* mov r0,r1 */
 goto L8c5fc70e;
L8c5fc70e: r[1]|=r[8]; /* or r8,r1 */
 goto L8c5fc710;
L8c5fc710: r[1]=rd(&cache->registers,m,r[1],4); /* mov.l @r1,r1 */
 goto L8c5fc712;
L8c5fc712: r[1]&=r[9]; /* and r9,r1 */
 goto L8c5fc714;
L8c5fc714: r[1]|=r[0]; /* or r0,r1 */
 goto L8c5fc716;
L8c5fc716: r[11]=r[0]; /* mov r0,r11 */
 goto L8c5fc718;
L8c5fc718: r[11]&=r[10]; /* and r10,r11 */
 goto L8c5fc71a;
L8c5fc71a: r[1]|=r[11]; /* or r11,r1 */
 goto L8c5fc71c;
L8c5fc71c: r[1]|=r[12]; /* or r12,r1 */
 goto L8c5fc71e;
L8c5fc71e: *sr=(*sr&~1u)|(r[4]>r[1]); /* cmp/hi r1,r4 */
 goto L8c5fc720;
L8c5fc720: if((*sr&1u))goto L8c5fc72a; /* bt 0x8c5fc72a */
 goto L8c5fc722;
L8c5fc722: *sr=(*sr&~1u)|(r[1]>r[5]); /* cmp/hi r5,r1 */
 goto L8c5fc724;
L8c5fc724: if((*sr&1u))goto L8c5fc72a; /* bt 0x8c5fc72a */
 goto L8c5fc726;
L8c5fc726: r[1]|=r[7]; /* or r7,r1 */
 goto L8c5fc728;
L8c5fc728: status=cache->writeback(cache->registers.user,r[1]);if(status)return status; /* ocbwb @r1 */
 goto L8c5fc72a;
L8c5fc72a: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c5fc72c;
L8c5fc72c: if((*sr&1u))goto L8c5fc732; /* bt 0x8c5fc732 */
 goto L8c5fc72e;
L8c5fc72e: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c5fc730;
L8c5fc730: if((*sr&1u))goto L8c5fc738; /* bt 0x8c5fc738 */
 goto L8c5fc732;
L8c5fc732: r[0]+=32u; /* add #0x20,r0 */
 goto L8c5fc734;
L8c5fc734: *sr=(*sr&~1u)|(r[2]>r[0]); /* cmp/hi r0,r2 */
 goto L8c5fc736;
L8c5fc736: if((*sr&1u))goto L8c5fc70c; /* bt 0x8c5fc70c */
 goto L8c5fc738;
L8c5fc738: r[8]=rd(&cache->registers,m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c5fc73a;
L8c5fc73a: r[9]=rd(&cache->registers,m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c5fc73c;
L8c5fc73c: r[10]=rd(&cache->registers,m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c5fc73e;
L8c5fc73e: r[11]=rd(&cache->registers,m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c5fc740;
L8c5fc740: r[2]=rd(&cache->registers,m,0x8c5fc750u,4); /* mov.l 0x8c5fc750,r2 */
 goto L8c5fc742;
L8c5fc742: target=r[2];switch(target&0x1fffffffu){case 0xc5fc7c0u:goto L8c5fc7c0;default:return -2;} /* jmp @r2 */
L8c5fc746: r[4]=r[14]; /* mov r14,r4 */
 goto L8c5fc748;
L8c5fc748: r[5]=r[12]; /* mov r12,r5 */
 goto L8c5fc74a;
L8c5fc74a: r[6]=r[13]; /* mov r13,r6 */
 goto L8c5fc74c;
L8c5fc74c: goto L8c5fc754; /* bra 0x8c5fc754 */
 goto L8c5fc754;
L8c5fc754: r[15]-=4;wr(&cache->registers,m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c5fc756;
L8c5fc756: r[15]-=4;wr(&cache->registers,m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c5fc758;
L8c5fc758: r[15]-=4;wr(&cache->registers,m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c5fc75a;
L8c5fc75a: r[15]-=4;wr(&cache->registers,m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c5fc75c;
L8c5fc75c: r[0]=rd(&cache->registers,m,0x8c5fc778u,4); /* mov.l 0x8c5fc778,r0 */
 goto L8c5fc75e;
L8c5fc75e: r[8]=rd(&cache->registers,m,0x8c5fc77cu,4); /* mov.l 0x8c5fc77c,r8 */
 goto L8c5fc760;
L8c5fc760: r[9]=rd(&cache->registers,m,0x8c5fc780u,4); /* mov.l 0x8c5fc780,r9 */
 goto L8c5fc762;
L8c5fc762: r[10]=rd(&cache->registers,m,0x8c5fc784u,4); /* mov.l 0x8c5fc784,r10 */
 goto L8c5fc764;
L8c5fc764: r[2]=rd(&cache->registers,m,0x8c5fc788u,4); /* mov.l 0x8c5fc788,r2 */
 goto L8c5fc766;
L8c5fc766: r[3]=rd(&cache->registers,m,0x8c5fc78cu,4); /* mov.l 0x8c5fc78c,r3 */
 goto L8c5fc768;
L8c5fc768: r[7]=rd(&cache->registers,m,0x8c5fc790u,4); /* mov.l 0x8c5fc790,r7 */
 goto L8c5fc76a;
L8c5fc76a: r[7]&=r[4]; /* and r4,r7 */
 goto L8c5fc76c;
L8c5fc76c: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc76e;
L8c5fc76e: r[4]&=r[7]; /* and r7,r4 */
 goto L8c5fc770;
L8c5fc770: r[7]=~r[7]; /* not r7,r7 */
 goto L8c5fc772;
L8c5fc772: goto L8c5fc7b4; /* bra 0x8c5fc7b4 */
 goto L8c5fc794;
L8c5fc794: r[1]=r[0]; /* mov r0,r1 */
 goto L8c5fc796;
L8c5fc796: r[1]|=r[8]; /* or r8,r1 */
 goto L8c5fc798;
L8c5fc798: r[1]=rd(&cache->registers,m,r[1],4); /* mov.l @r1,r1 */
 goto L8c5fc79a;
L8c5fc79a: r[1]&=r[9]; /* and r9,r1 */
 goto L8c5fc79c;
L8c5fc79c: r[1]|=r[0]; /* or r0,r1 */
 goto L8c5fc79e;
L8c5fc79e: *sr=(*sr&~1u)|(r[4]>r[1]); /* cmp/hi r1,r4 */
 goto L8c5fc7a0;
L8c5fc7a0: if((*sr&1u))goto L8c5fc7aa; /* bt 0x8c5fc7aa */
 goto L8c5fc7a2;
L8c5fc7a2: *sr=(*sr&~1u)|(r[1]>r[5]); /* cmp/hi r5,r1 */
 goto L8c5fc7a4;
L8c5fc7a4: if((*sr&1u))goto L8c5fc7aa; /* bt 0x8c5fc7aa */
 goto L8c5fc7a6;
L8c5fc7a6: r[1]|=r[7]; /* or r7,r1 */
 goto L8c5fc7a8;
L8c5fc7a8: status=cache->writeback(cache->registers.user,r[1]);if(status)return status; /* ocbwb @r1 */
 goto L8c5fc7aa;
L8c5fc7aa: *sr=(*sr&~1u)|!(r[6]&r[6]); /* tst r6,r6 */
 goto L8c5fc7ac;
L8c5fc7ac: if((*sr&1u))goto L8c5fc7b2; /* bt 0x8c5fc7b2 */
 goto L8c5fc7ae;
L8c5fc7ae: *sr=(*sr&~1u)|(r[0]==r[3]); /* cmp/eq r0,r3 */
 goto L8c5fc7b0;
L8c5fc7b0: if((*sr&1u))goto L8c5fc7b8; /* bt 0x8c5fc7b8 */
 goto L8c5fc7b2;
L8c5fc7b2: r[0]+=32u; /* add #0x20,r0 */
 goto L8c5fc7b4;
L8c5fc7b4: *sr=(*sr&~1u)|(r[2]>r[0]); /* cmp/hi r0,r2 */
 goto L8c5fc7b6;
L8c5fc7b6: if((*sr&1u))goto L8c5fc794; /* bt 0x8c5fc794 */
 goto L8c5fc7b8;
L8c5fc7b8: r[8]=rd(&cache->registers,m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c5fc7ba;
L8c5fc7ba: r[9]=rd(&cache->registers,m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c5fc7bc;
L8c5fc7bc: r[10]=rd(&cache->registers,m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c5fc7be;
L8c5fc7be: r[11]=rd(&cache->registers,m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c5fc7c0;
L8c5fc7c0: r[0]=sx(&cache->registers,m,r[15],2); /* mov.w @r15,r0 */
 goto L8c5fc7c2;
L8c5fc7c2: r[3]=*sr; /* stc SR,r3 */
 goto L8c5fc7c4;
L8c5fc7c4: r[0]=r[0]&65535u; /* extu.w r0,r0 */
 goto L8c5fc7c6;
L8c5fc7c6: r[2]=sx(&cache->registers,m,0x8c5fc7f4u,2); /* mov.w 0x8c5fc7f4,r2 */
 goto L8c5fc7c8;
L8c5fc7c8: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c5fc7ca;
L8c5fc7ca: r[0]<<=2; /* shll2 r0 */
 goto L8c5fc7cc;
L8c5fc7cc: r[3]&=r[2]; /* and r2,r3 */
 goto L8c5fc7ce;
L8c5fc7ce: r[0]<<=2; /* shll2 r0 */
 goto L8c5fc7d0;
L8c5fc7d0: r[0]|=r[3]; /* or r3,r0 */
 goto L8c5fc7d2;
L8c5fc7d2: *sr=r[0]; /* ldc r0,SR */
 goto L8c5fc7d4;
L8c5fc7d4: r[15]+=8u; /* add #0x8,r15 */
 goto L8c5fc7d6;
L8c5fc7d6: r[12]=rd(&cache->registers,m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c5fc7d8;
L8c5fc7d8: r[13]=rd(&cache->registers,m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c5fc7da;
L8c5fc7da: r[14]=rd(&cache->registers,m,r[15],4);r[15]+=4;return 0; /* rts */
}
