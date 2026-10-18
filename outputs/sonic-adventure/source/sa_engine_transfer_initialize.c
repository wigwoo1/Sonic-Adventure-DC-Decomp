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
/* Static C recovery of USA Rev A transfer initialization. Hardware accesses require explicit IO specialization. */
SA_STAGE_EXPORT int sa_engine_transfer_initialize(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10d5a8;
L8c10d5a8: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c10d5aa;
L8c10d5aa: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c10d5ac;
L8c10d5ac: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c10d5ae;
L8c10d5ae: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c10d5b0;
L8c10d5b0: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c10d5b2;
L8c10d5b2: r[15]-=4;wr(m,r[15],r[9],4); /* mov.l r9,@-r15 */
 goto L8c10d5b4;
L8c10d5b4: r[15]-=4;wr(m,r[15],r[8],4); /* mov.l r8,@-r15 */
 goto L8c10d5b6;
L8c10d5b6: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10d5b8;
L8c10d5b8: r[15]+=4294967292u; /* add #-0x4,r15 */
 goto L8c10d5ba;
L8c10d5ba: r[2]=rd(m,0x8c10d748u,4); /* mov.l 0x8c10d748,r2 */
 goto L8c10d5bc;
L8c10d5bc: r[3]=4294967264u; /* mov #-0x20,r3 */
 goto L8c10d5be;
L8c10d5be: r[14]=rd(m,0x8c10d744u,4); /* mov.l 0x8c10d744,r14 */
 goto L8c10d5c0;
L8c10d5c0: r[5]=0u; /* mov #0x0,r5 */
 goto L8c10d5c2;
L8c10d5c2: r[6]=sx(m,0x8c10d72au,2); /* mov.w 0x8c10d72a,r6 */
 goto L8c10d5c4;
L8c10d5c4: r[2]&=r[3]; /* and r3,r2 */
 goto L8c10d5c6;
L8c10d5c6: r[1]=rd(m,0x8c10d74cu,4); /* mov.l 0x8c10d74c,r1 */
 goto L8c10d5c8;
L8c10d5c8: wr(m,r[14],r[2],4); /* mov.l r2,@r14 */
 goto L8c10d5ca;
L8c10d5ca: target=r[1];*pr=0x8c10d5ceu;r[4]=r[2];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c10d5ce;
L8c10d5ce: r[2]=rd(m,0x8c10d750u,4); /* mov.l 0x8c10d750,r2 */
 goto L8c10d5d0;
L8c10d5d0: r[13]=0u; /* mov #0x0,r13 */
 goto L8c10d5d2;
L8c10d5d2: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c10d5d4;
L8c10d5d4: r[10]=3u; /* mov #0x3,r10 */
 goto L8c10d5d6;
L8c10d5d6: r[0]=sx(m,0x8c10d72cu,2); /* mov.w 0x8c10d72c,r0 */
 goto L8c10d5d8;
L8c10d5d8: r[11]=r[13]; /* mov r13,r11 */
 goto L8c10d5da;
L8c10d5da: wr(m,r[3],r[2],4); /* mov.l r2,@r3 */
 goto L8c10d5dc;
L8c10d5dc: r[2]=1u; /* mov #0x1,r2 */
 goto L8c10d5de;
L8c10d5de: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c10d5e0;
L8c10d5e0: r[9]=4u; /* mov #0x4,r9 */
 goto L8c10d5e2;
L8c10d5e2: r[8]=52u; /* mov #0x34,r8 */
 goto L8c10d5e4;
L8c10d5e4: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c10d5e6;
L8c10d5e6: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c10d5e8;
L8c10d5e8: r[0]=sx(m,0x8c10d72eu,2); /* mov.w 0x8c10d72e,r0 */
 goto L8c10d5ea;
L8c10d5ea: wr(m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c10d5ec;
L8c10d5ec: *macl=r[8]*r[11]; /* mul.l r8,r11 */
 goto L8c10d5ee;
L8c10d5ee: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d5f0;
L8c10d5f0: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10d5f2;
L8c10d5f2: r[12]=*macl; /* sts MACL,r12 */
 goto L8c10d5f4;
L8c10d5f4: r[2]+=r[12]; /* add r12,r2 */
 goto L8c10d5f6;
L8c10d5f6: wr(m,r[2],r[11],4); /* mov.l r11,@r2 */
 goto L8c10d5f8;
L8c10d5f8: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d5fa;
L8c10d5fa: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10d5fc;
L8c10d5fc: r[2]+=r[12]; /* add r12,r2 */
 goto L8c10d5fe;
L8c10d5fe: wr(m,r[2]+0x28u,r[13],4); /* mov.l r13,@(0x28,r2) */
 goto L8c10d600;
L8c10d600: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d602;
L8c10d602: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10d604;
L8c10d604: r[2]+=r[12]; /* add r12,r2 */
 goto L8c10d606;
L8c10d606: wr(m,r[2]+0x2cu,r[13],4); /* mov.l r13,@(0x2c,r2) */
 goto L8c10d608;
L8c10d608: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d60a;
L8c10d60a: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10d60c;
L8c10d60c: r[2]+=r[12]; /* add r12,r2 */
 goto L8c10d60e;
L8c10d60e: wr(m,r[2]+0x30u,r[13],4); /* mov.l r13,@(0x30,r2) */
 goto L8c10d610;
L8c10d610: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d612;
L8c10d612: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10d614;
L8c10d614: r[2]+=r[12]; /* add r12,r2 */
 goto L8c10d616;
L8c10d616: wr(m,r[2]+0x4u,r[13],4); /* mov.l r13,@(0x4,r2) */
 goto L8c10d618;
L8c10d618: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d61a;
L8c10d61a: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10d61c;
L8c10d61c: r[2]+=r[12]; /* add r12,r2 */
 goto L8c10d61e;
L8c10d61e: wr(m,r[2]+0x8u,r[13],4); /* mov.l r13,@(0x8,r2) */
 goto L8c10d620;
L8c10d620: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d622;
L8c10d622: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10d624;
L8c10d624: r[2]+=r[12]; /* add r12,r2 */
 goto L8c10d626;
L8c10d626: wr(m,r[2]+0xcu,r[13],4); /* mov.l r13,@(0xc,r2) */
 goto L8c10d628;
L8c10d628: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d62a;
L8c10d62a: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10d62c;
L8c10d62c: r[2]+=r[12]; /* add r12,r2 */
 goto L8c10d62e;
L8c10d62e: r[3]=rd(m,0x8c10d754u,4); /* mov.l 0x8c10d754,r3 */
 goto L8c10d630;
L8c10d630: wr(m,r[2]+0x10u,r[13],4); /* mov.l r13,@(0x10,r2) */
 goto L8c10d632;
L8c10d632: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d634;
L8c10d634: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10d636;
L8c10d636: r[2]+=r[12]; /* add r12,r2 */
 goto L8c10d638;
L8c10d638: wr(m,r[2]+0x14u,r[13],4); /* mov.l r13,@(0x14,r2) */
 goto L8c10d63a;
L8c10d63a: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d63c;
L8c10d63c: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10d63e;
L8c10d63e: r[2]+=r[12]; /* add r12,r2 */
 goto L8c10d640;
L8c10d640: wr(m,r[2]+0x18u,r[10],4); /* mov.l r10,@(0x18,r2) */
 goto L8c10d642;
L8c10d642: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d644;
L8c10d644: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10d646;
L8c10d646: r[2]+=r[12]; /* add r12,r2 */
 goto L8c10d648;
L8c10d648: wr(m,r[2]+0x1cu,r[13],4); /* mov.l r13,@(0x1c,r2) */
 goto L8c10d64a;
L8c10d64a: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d64c;
L8c10d64c: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10d64e;
L8c10d64e: r[2]+=r[12]; /* add r12,r2 */
 goto L8c10d650;
L8c10d650: wr(m,r[2]+0x20u,r[13],4); /* mov.l r13,@(0x20,r2) */
 goto L8c10d652;
L8c10d652: r[4]=rd(m,r[14],4); /* mov.l @r14,r4 */
 goto L8c10d654;
L8c10d654: r[4]+=4u; /* add #0x4,r4 */
 goto L8c10d656;
L8c10d656: target=r[3];*pr=0x8c10d65au;r[4]+=r[12];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10d65a;
L8c10d65a: r[3]=sx(m,0x8c10d730u,2); /* mov.w 0x8c10d730,r3 */
 goto L8c10d65c;
L8c10d65c: r[4]=r[11]; /* mov r11,r4 */
 goto L8c10d65e;
L8c10d65e: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c10d660;
L8c10d660: r[4]<<=2; /* shll2 r4 */
 goto L8c10d662;
L8c10d662: r[11]+=1u; /* add #0x1,r11 */
 goto L8c10d664;
L8c10d664: r[3]+=r[0]; /* add r0,r3 */
 goto L8c10d666;
L8c10d666: *sr=(*sr&~1u)|((int32_t)r[11]>=(int32_t)r[9]); /* cmp/ge r9,r11 */
 goto L8c10d668;
L8c10d668: r[3]+=r[4]; /* add r4,r3 */
 goto L8c10d66a;
L8c10d66a: wr(m,r[3],r[13],4); /* mov.l r13,@r3 */
 goto L8c10d66c;
L8c10d66c: r[3]=sx(m,0x8c10d732u,2); /* mov.w 0x8c10d732,r3 */
 goto L8c10d66e;
L8c10d66e: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c10d670;
L8c10d670: r[3]+=r[0]; /* add r0,r3 */
 goto L8c10d672;
L8c10d672: r[3]+=r[4]; /* add r4,r3 */
 goto L8c10d674;
L8c10d674: wr(m,r[3],r[13],4); /* mov.l r13,@r3 */
 goto L8c10d676;
L8c10d676: r[3]=sx(m,0x8c10d734u,2); /* mov.w 0x8c10d734,r3 */
 goto L8c10d678;
L8c10d678: r[0]=rd(m,r[14],4); /* mov.l @r14,r0 */
 goto L8c10d67a;
L8c10d67a: r[3]+=r[0]; /* add r0,r3 */
 goto L8c10d67c;
L8c10d67c: r[4]+=r[3]; /* add r3,r4 */
 goto L8c10d67e;
L8c10d67e: take=!(*sr&1u);wr(m,r[4],r[13],4);if(take)goto L8c10d5ec;goto L8c10d682; /* bf/s 0x8c10d5ec */
 goto L8c10d682;
L8c10d682: r[0]=sx(m,0x8c10d736u,2); /* mov.w 0x8c10d736,r0 */
 goto L8c10d684;
L8c10d684: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c10d686;
L8c10d686: r[4]=rd(m,0x8c10d75cu,4); /* mov.l 0x8c10d75c,r4 */
 goto L8c10d688;
L8c10d688: wr(m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c10d68a;
L8c10d68a: r[0]+=4u; /* add #0x4,r0 */
 goto L8c10d68c;
L8c10d68c: r[3]=rd(m,r[14],4); /* mov.l @r14,r3 */
 goto L8c10d68e;
L8c10d68e: r[5]=rd(m,0x8c10d758u,4); /* mov.l 0x8c10d758,r5 */
 goto L8c10d690;
L8c10d690: wr(m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c10d692;
L8c10d692: r[3]=rd(m,0x8c10d760u,4); /* mov.l 0x8c10d760,r3 */
 goto L8c10d694;
L8c10d694: target=r[3];*pr=0x8c10d698u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10d698;
L8c10d698: r[0]=sx(m,0x8c10d72cu,2); /* mov.w 0x8c10d72c,r0 */
 goto L8c10d69a;
L8c10d69a: r[3]=1u; /* mov #0x1,r3 */
 goto L8c10d69c;
L8c10d69c: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d69e;
L8c10d69e: r[6]=96u; /* mov #0x60,r6 */
 goto L8c10d6a0;
L8c10d6a0: r[4]=sx(m,0x8c10d738u,2); /* mov.w 0x8c10d738,r4 */
 goto L8c10d6a2;
L8c10d6a2: wr(m,r[2]+r[0],r[3],4); /* mov.l r3,@(r0,r2) */
 goto L8c10d6a4;
L8c10d6a4: r[3]=36u; /* mov #0x24,r3 */
 goto L8c10d6a6;
L8c10d6a6: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d6a8;
L8c10d6a8: r[12]=rd(m,0x8c10d768u,4); /* mov.l 0x8c10d768,r12 */
 goto L8c10d6aa;
L8c10d6aa: r[2]+=4u; /* add #0x4,r2 */
 goto L8c10d6ac;
L8c10d6ac: r[11]=rd(m,0x8c10d764u,4); /* mov.l 0x8c10d764,r11 */
 goto L8c10d6ae;
L8c10d6ae: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10d6b0;
L8c10d6b0: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c10d6b2;
L8c10d6b2: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c10d6b4;
L8c10d6b4: r[7]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r7 */
 goto L8c10d6b6;
L8c10d6b6: target=r[12];*pr=0x8c10d6bau;r[5]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c10d6ba;
L8c10d6ba: r[1]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r1 */
 goto L8c10d6bc;
L8c10d6bc: r[3]=36u; /* mov #0x24,r3 */
 goto L8c10d6be;
L8c10d6be: r[4]=sx(m,0x8c10d73au,2); /* mov.w 0x8c10d73a,r4 */
 goto L8c10d6c0;
L8c10d6c0: r[6]=96u; /* mov #0x60,r6 */
 goto L8c10d6c2;
L8c10d6c2: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c10d6c4;
L8c10d6c4: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d6c6;
L8c10d6c6: r[2]+=56u; /* add #0x38,r2 */
 goto L8c10d6c8;
L8c10d6c8: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10d6ca;
L8c10d6ca: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c10d6cc;
L8c10d6cc: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c10d6ce;
L8c10d6ce: r[7]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r7 */
 goto L8c10d6d0;
L8c10d6d0: target=r[12];*pr=0x8c10d6d4u;r[5]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c10d6d4;
L8c10d6d4: r[1]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r1 */
 goto L8c10d6d6;
L8c10d6d6: r[3]=36u; /* mov #0x24,r3 */
 goto L8c10d6d8;
L8c10d6d8: r[4]=sx(m,0x8c10d73cu,2); /* mov.w 0x8c10d73c,r4 */
 goto L8c10d6da;
L8c10d6da: r[6]=96u; /* mov #0x60,r6 */
 goto L8c10d6dc;
L8c10d6dc: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c10d6de;
L8c10d6de: r[2]=rd(m,r[14],4); /* mov.l @r14,r2 */
 goto L8c10d6e0;
L8c10d6e0: r[2]+=108u; /* add #0x6c,r2 */
 goto L8c10d6e2;
L8c10d6e2: r[3]+=r[2]; /* add r2,r3 */
 goto L8c10d6e4;
L8c10d6e4: wr(m,r[15],r[2],4); /* mov.l r2,@r15 */
 goto L8c10d6e6;
L8c10d6e6: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c10d6e8;
L8c10d6e8: r[7]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r7 */
 goto L8c10d6ea;
L8c10d6ea: target=r[12];*pr=0x8c10d6eeu;r[5]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c10d6ee;
L8c10d6ee: r[1]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r1 */
 goto L8c10d6f0;
L8c10d6f0: r[3]=36u; /* mov #0x24,r3 */
 goto L8c10d6f2;
L8c10d6f2: r[2]=sx(m,0x8c10d73eu,2); /* mov.w 0x8c10d73e,r2 */
 goto L8c10d6f4;
L8c10d6f4: r[6]=96u; /* mov #0x60,r6 */
 goto L8c10d6f6;
L8c10d6f6: wr(m,r[1],r[0],4); /* mov.l r0,@r1 */
 goto L8c10d6f8;
L8c10d6f8: r[1]=rd(m,r[14],4); /* mov.l @r14,r1 */
 goto L8c10d6fa;
L8c10d6fa: r[4]=sx(m,0x8c10d740u,2); /* mov.w 0x8c10d740,r4 */
 goto L8c10d6fc;
L8c10d6fc: r[1]+=4u; /* add #0x4,r1 */
 goto L8c10d6fe;
L8c10d6fe: r[1]+=r[2]; /* add r2,r1 */
 goto L8c10d700;
L8c10d700: r[3]+=r[1]; /* add r1,r3 */
 goto L8c10d702;
L8c10d702: wr(m,r[15],r[1],4); /* mov.l r1,@r15 */
 goto L8c10d704;
L8c10d704: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c10d706;
L8c10d706: r[7]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r7 */
 goto L8c10d708;
L8c10d708: target=r[12];*pr=0x8c10d70cu;r[5]=r[11];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r12 */
 goto L8c10d70c;
L8c10d70c: r[2]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r2 */
 goto L8c10d70e;
L8c10d70e: r[3]=rd(m,0x8c10d76cu,4); /* mov.l 0x8c10d76c,r3 */
 goto L8c10d710;
L8c10d710: wr(m,r[2],r[0],4); /* mov.l r0,@r2 */
 goto L8c10d712;
L8c10d712: r[0]=0u; /* mov #0x0,r0 */
 goto L8c10d714;
L8c10d714: wr(m,r[3],r[13],4); /* mov.l r13,@r3 */
 goto L8c10d716;
L8c10d716: r[15]+=4u; /* add #0x4,r15 */
 goto L8c10d718;
L8c10d718: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10d71a;
L8c10d71a: r[8]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r8 */
 goto L8c10d71c;
L8c10d71c: r[9]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r9 */
 goto L8c10d71e;
L8c10d71e: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c10d720;
L8c10d720: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c10d722;
L8c10d722: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c10d724;
L8c10d724: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c10d726;
L8c10d726: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
