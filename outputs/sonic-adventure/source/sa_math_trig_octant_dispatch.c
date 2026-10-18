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
/* Retail finite trigonometric octant dispatcher10fcbc..10fd8c.
 * Absolute input, external reduction, odd-index adjustment, helper/sign selection.
 * Lower10fbb8/110218/110238 remain external; finite PR/SZ/RM0 scope. */
SA_STAGE_EXPORT int sa_math_trig_octant_dispatch(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c10fcbc;
L8c10fcbc: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c10fcbe;
L8c10fcbe: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c10fcc0;
L8c10fcc0: r[0]=4u; /* mov #0x4,r0 */
 goto L8c10fcc2;
L8c10fcc2: fr[5]=fr[4]; /* fmov fr4,fr5 */
 goto L8c10fcc4;
L8c10fcc4: r[4]=0u; /* mov #0x0,r4 */
 goto L8c10fcc6;
L8c10fcc6: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(5u&1u);unsigned pair=5u&~1u;wr(m,r[0]+r[15],f->bank[bank][pair],4);wr(m,r[0]+r[15]+4,f->bank[bank][pair+1],4);}else wr(m,r[0]+r[15],fr[5],4); /* fmov fr5,@(r0,r15) */
 goto L8c10fcc8;
L8c10fcc8: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c10fcca;
L8c10fcca: *sr=(*sr&~1u)|((int32_t)r[3]>=0); /* cmp/pz r3 */
 goto L8c10fccc;
L8c10fccc: take=(*sr&1u);r[5]=8u;if(take)goto L8c10fcd2;goto L8c10fcd0; /* bt/s 0x8c10fcd2 */
 goto L8c10fcd0;
L8c10fcd0: r[4]=r[5]; /* mov r5,r4 */
 goto L8c10fcd2;
L8c10fcd2: fr[3]=0; /* fldi0 fr3 */
 goto L8c10fcd4;
L8c10fcd4: status=eq(f,sr,fr[5],fr[3]);if(status)return status; /* fcmp/eq fr3,fr5 */
 goto L8c10fcd6;
L8c10fcd6: if(!(*sr&1u))goto L8c10fcde; /* bf 0x8c10fcde */
 goto L8c10fcd8;
L8c10fcd8: r[2]=4u; /* mov #0x4,r2 */
 goto L8c10fcda;
L8c10fcda: r[4]|=r[2];goto L8c10fcf8; /* bra 0x8c10fcf8 */
 goto L8c10fcde;
L8c10fcde: r[1]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r1 */
 goto L8c10fce0;
L8c10fce0: r[3]=rd(m,0x8c10fd8cu,4); /* mov.l 0x8c10fd8c,r3 */
 goto L8c10fce2;
L8c10fce2: r[2]=rd(m,0x8c10fd90u,4); /* mov.l 0x8c10fd90,r2 */
 goto L8c10fce4;
L8c10fce4: r[1]&=r[3]; /* and r3,r1 */
 goto L8c10fce6;
L8c10fce6: *sr=(*sr&~1u)|(r[2]==r[1]); /* cmp/eq r2,r1 */
 goto L8c10fce8;
L8c10fce8: if(!(*sr&1u))goto L8c10fcf0; /* bf 0x8c10fcf0 */
 goto L8c10fcea;
L8c10fcea: r[0]=2u; /* mov #0x2,r0 */
 goto L8c10fcec;
L8c10fcec: r[4]|=r[0];goto L8c10fcf8; /* bra 0x8c10fcf8 */
 goto L8c10fcf0;
L8c10fcf0: status=eq(f,sr,fr[5],fr[5]);if(status)return status; /* fcmp/eq fr5,fr5 */
 goto L8c10fcf2;
L8c10fcf2: if((*sr&1u))goto L8c10fcf8; /* bt 0x8c10fcf8 */
 goto L8c10fcf4;
L8c10fcf4: r[0]=1u; /* mov #0x1,r0 */
 goto L8c10fcf6;
L8c10fcf6: r[4]|=r[0]; /* or r0,r4 */
 goto L8c10fcf8;
L8c10fcf8: r[2]=3u; /* mov #0x3,r2 */
 goto L8c10fcfa;
L8c10fcfa: r[0]=r[4]; /* mov r4,r0 */
 goto L8c10fcfc;
L8c10fcfc: *sr=(*sr&~1u)|!(r[0]&r[2]); /* tst r0,r2 */
 goto L8c10fcfe;
L8c10fcfe: if((*sr&1u))goto L8c10fd0c; /* bt 0x8c10fd0c */
 goto L8c10fd00;
L8c10fd00: r[1]=rd(m,0x8c10fd94u,4); /* mov.l 0x8c10fd94,r1 */
 goto L8c10fd02;
L8c10fd02: fr[0]=rd(m,r[1],4); /* fmov.s @r1,fr0 */
 goto L8c10fd04;
L8c10fd04: r[15]+=8u; /* add #0x8,r15 */
 goto L8c10fd06;
L8c10fd06: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10fd08;
L8c10fd08: return 0; /* rts */
L8c10fd0c: *sr=(*sr&~1u)|!(r[0]&r[5]); /* tst r0,r5 */
 goto L8c10fd0e;
L8c10fd0e: if((*sr&1u))goto L8c10fd12; /* bt 0x8c10fd12 */
 goto L8c10fd10;
L8c10fd10: fr[4]^=0x80000000u; /* fneg fr4 */
 goto L8c10fd12;
L8c10fd12: r[3]=rd(m,0x8c10fd9cu,4); /* mov.l 0x8c10fd9c,r3 */
 goto L8c10fd14;
L8c10fd14: r[4]=r[15]; /* mov r15,r4 */
 goto L8c10fd16;
L8c10fd16: r[2]=rd(m,0x8c10fd98u,4); /* mov.l 0x8c10fd98,r2 */
 goto L8c10fd18;
L8c10fd18: target=r[3];*pr=0x8c10fd1cu;fr[5]=rd(m,r[2],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10fd1c;
L8c10fd1c: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c10fd1e;
L8c10fd1e: fr[4]=fr[0]; /* fmov fr0,fr4 */
 goto L8c10fd20;
L8c10fd20: r[0]&=0x7u; /* and #0x7,r0 */
 goto L8c10fd22;
L8c10fd22: *sr=(*sr&~1u)|!(r[0]&0x1u); /* tst #0x1,r0 */
 goto L8c10fd24;
L8c10fd24: take=(*sr&1u);wr(m,r[15],r[0],4);if(take)goto L8c10fd30;goto L8c10fd28; /* bt/s 0x8c10fd30 */
 goto L8c10fd28;
L8c10fd28: r[3]=rd(m,0x8c10fda0u,4); /* mov.l 0x8c10fda0,r3 */
 goto L8c10fd2a;
L8c10fd2a: fr[3]=fr[4]; /* fmov fr4,fr3 */
 goto L8c10fd2c;
L8c10fd2c: fr[4]=rd(m,r[3],4); /* fmov.s @r3,fr4 */
 goto L8c10fd2e;
L8c10fd2e: status=sa_sh4_add_sub(&fr[4],&fr[3],&f->fpscr,&fr[4],0);if(status)return status; /* fadd fr3,fr4 */
 goto L8c10fd30;
L8c10fd30: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c10fd32;
L8c10fd32: *sr=(*sr&~1u)|(r[0]==0x00000000u); /* cmp/eq #0x0,r0 */
 goto L8c10fd34;
L8c10fd34: if((*sr&1u))goto L8c10fd56; /* bt 0x8c10fd56 */
 goto L8c10fd36;
L8c10fd36: *sr=(*sr&~1u)|(r[0]==0x00000007u); /* cmp/eq #0x7,r0 */
 goto L8c10fd38;
L8c10fd38: if((*sr&1u))goto L8c10fd56; /* bt 0x8c10fd56 */
 goto L8c10fd3a;
L8c10fd3a: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c10fd3c;
L8c10fd3c: if((*sr&1u))goto L8c10fd60; /* bt 0x8c10fd60 */
 goto L8c10fd3e;
L8c10fd3e: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c10fd40;
L8c10fd40: if((*sr&1u))goto L8c10fd60; /* bt 0x8c10fd60 */
 goto L8c10fd42;
L8c10fd42: *sr=(*sr&~1u)|(r[0]==0x00000003u); /* cmp/eq #0x3,r0 */
 goto L8c10fd44;
L8c10fd44: if((*sr&1u))goto L8c10fd6c; /* bt 0x8c10fd6c */
 goto L8c10fd46;
L8c10fd46: *sr=(*sr&~1u)|(r[0]==0x00000004u); /* cmp/eq #0x4,r0 */
 goto L8c10fd48;
L8c10fd48: if((*sr&1u))goto L8c10fd6c; /* bt 0x8c10fd6c */
 goto L8c10fd4a;
L8c10fd4a: *sr=(*sr&~1u)|(r[0]==0x00000005u); /* cmp/eq #0x5,r0 */
 goto L8c10fd4c;
L8c10fd4c: if((*sr&1u))goto L8c10fd7a; /* bt 0x8c10fd7a */
 goto L8c10fd4e;
L8c10fd4e: *sr=(*sr&~1u)|(r[0]==0x00000006u); /* cmp/eq #0x6,r0 */
 goto L8c10fd50;
L8c10fd50: if((*sr&1u))goto L8c10fd7a; /* bt 0x8c10fd7a */
 goto L8c10fd52;
L8c10fd52: goto L8c10fd82; /* bra 0x8c10fd82 */
 goto L8c10fd56;
L8c10fd56: r[3]=rd(m,0x8c10fda4u,4); /* mov.l 0x8c10fda4,r3 */
 goto L8c10fd58;
L8c10fd58: target=r[3];*pr=0x8c10fd5cu;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10fd5c;
L8c10fd5c: goto L8c10fd80; /* bra 0x8c10fd80 */
 goto L8c10fd60;
L8c10fd60: r[3]=rd(m,0x8c10fda8u,4); /* mov.l 0x8c10fda8,r3 */
 goto L8c10fd62;
L8c10fd62: target=r[3];*pr=0x8c10fd66u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10fd66;
L8c10fd66: fr[4]=fr[0]; /* fmov fr0,fr4 */
 goto L8c10fd68;
L8c10fd68: fr[5]=fr[0];goto L8c10fd76; /* bra 0x8c10fd76 */
 goto L8c10fd6c;
L8c10fd6c: r[3]=rd(m,0x8c10fda4u,4); /* mov.l 0x8c10fda4,r3 */
 goto L8c10fd6e;
L8c10fd6e: target=r[3];*pr=0x8c10fd72u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10fd72;
L8c10fd72: fr[5]=fr[0]; /* fmov fr0,fr5 */
 goto L8c10fd74;
L8c10fd74: fr[4]=fr[0]; /* fmov fr0,fr4 */
 goto L8c10fd76;
L8c10fd76: fr[5]^=0x80000000u;goto L8c10fd82; /* bra 0x8c10fd82 */
 goto L8c10fd7a;
L8c10fd7a: r[3]=rd(m,0x8c10fda8u,4); /* mov.l 0x8c10fda8,r3 */
 goto L8c10fd7c;
L8c10fd7c: target=r[3];*pr=0x8c10fd80u;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c10fd80;
L8c10fd80: fr[5]=fr[0]; /* fmov fr0,fr5 */
 goto L8c10fd82;
L8c10fd82: fr[0]=fr[5]; /* fmov fr5,fr0 */
 goto L8c10fd84;
L8c10fd84: r[15]+=8u; /* add #0x8,r15 */
 goto L8c10fd86;
L8c10fd86: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c10fd88;
L8c10fd88: return 0; /* rts */
}
