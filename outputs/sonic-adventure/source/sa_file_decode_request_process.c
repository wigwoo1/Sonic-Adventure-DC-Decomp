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
/* Retail file decode request dispatcher644cb0..644d4c.
 * Forwards stored buffers,histories,coefficients to mono/stereo kernel.
 * Promotes pending phase to complete; positive odd stereo result sets flag. */
SA_STAGE_EXPORT int sa_file_decode_request_process(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c644cb0;
L8c644cb0: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c644cb2;
L8c644cb2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c644cb4;
L8c644cb4: r[15]+=4294967288u; /* add #-0x8,r15 */
 goto L8c644cb6;
L8c644cb6: r[14]=r[4]; /* mov r4,r14 */
 goto L8c644cb8;
L8c644cb8: r[0]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r0 */
 goto L8c644cba;
L8c644cba: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c644cbc;
L8c644cbc: if(!(*sr&1u))goto L8c644cc2; /* bf 0x8c644cc2 */
 goto L8c644cbe;
L8c644cbe: r[2]=2u; /* mov #0x2,r2 */
 goto L8c644cc0;
L8c644cc0: wr(m,r[14]+0xcu,r[2],4); /* mov.l r2,@(0xc,r14) */
 goto L8c644cc2;
L8c644cc2: r[0]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r0 */
 goto L8c644cc4;
L8c644cc4: *sr=(*sr&~1u)|(r[0]==0x00000002u); /* cmp/eq #0x2,r0 */
 goto L8c644cc6;
L8c644cc6: if(!(*sr&1u))goto L8c644d44; /* bf 0x8c644d44 */
 goto L8c644cc8;
L8c644cc8: r[0]=rd(m,r[14]+0x14u,4); /* mov.l @(0x14,r14),r0 */
 goto L8c644cca;
L8c644cca: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c644ccc;
L8c644ccc: if(!(*sr&1u))goto L8c644cf4; /* bf 0x8c644cf4 */
 goto L8c644cce;
L8c644cce: r[3]=r[14]; /* mov r14,r3 */
 goto L8c644cd0;
L8c644cd0: r[3]+=48u; /* add #0x30,r3 */
 goto L8c644cd2;
L8c644cd2: wr(m,r[15],r[3],4); /* mov.l r3,@r15 */
 goto L8c644cd4;
L8c644cd4: r[7]=r[14]; /* mov r14,r7 */
 goto L8c644cd6;
L8c644cd6: r[0]=sx(m,r[3]+0x2u,2); /* mov.w @(0x2,r3),r0 */
 goto L8c644cd8;
L8c644cd8: r[7]+=40u; /* add #0x28,r7 */
 goto L8c644cda;
L8c644cda: r[15]-=4;wr(m,r[15],r[0],4); /* mov.l r0,@-r15 */
 goto L8c644cdc;
L8c644cdc: r[2]=r[0]; /* mov r0,r2 */
 goto L8c644cde;
L8c644cde: r[1]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r1 */
 goto L8c644ce0;
L8c644ce0: r[2]=rd(m,0x8c644d78u,4); /* mov.l 0x8c644d78,r2 */
 goto L8c644ce2;
L8c644ce2: r[3]=sx(m,r[1],2); /* mov.w @r1,r3 */
 goto L8c644ce4;
L8c644ce4: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c644ce6;
L8c644ce6: r[6]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r6 */
 goto L8c644ce8;
L8c644ce8: r[5]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r5 */
 goto L8c644cea;
L8c644cea: target=r[2];*pr=0x8c644ceeu;r[4]=rd(m,r[14]+0x18u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c644cee;
L8c644cee: r[15]+=8u; /* add #0x8,r15 */
 goto L8c644cf0;
L8c644cf0: wr(m,r[14]+0x10u,r[0],4);goto L8c644d40; /* bra 0x8c644d40 */
 goto L8c644cf4;
L8c644cf4: r[2]=r[14]; /* mov r14,r2 */
 goto L8c644cf6;
L8c644cf6: r[2]+=48u; /* add #0x30,r2 */
 goto L8c644cf8;
L8c644cf8: wr(m,r[15]+0x4u,r[2],4); /* mov.l r2,@(0x4,r15) */
 goto L8c644cfa;
L8c644cfa: r[0]=sx(m,r[2]+0x2u,2); /* mov.w @(0x2,r2),r0 */
 goto L8c644cfc;
L8c644cfc: r[2]=r[14]; /* mov r14,r2 */
 goto L8c644cfe;
L8c644cfe: r[2]+=40u; /* add #0x28,r2 */
 goto L8c644d00;
L8c644d00: r[15]-=4;wr(m,r[15],r[0],4); /* mov.l r0,@-r15 */
 goto L8c644d02;
L8c644d02: r[3]=r[0]; /* mov r0,r3 */
 goto L8c644d04;
L8c644d04: r[1]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r1 */
 goto L8c644d06;
L8c644d06: r[3]=sx(m,r[1],2); /* mov.w @r1,r3 */
 goto L8c644d08;
L8c644d08: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c644d0a;
L8c644d0a: r[3]=4u; /* mov #0x4,r3 */
 goto L8c644d0c;
L8c644d0c: r[3]+=r[2]; /* add r2,r3 */
 goto L8c644d0e;
L8c644d0e: wr(m,r[15]+0x8u,r[2],4); /* mov.l r2,@(0x8,r15) */
 goto L8c644d10;
L8c644d10: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c644d12;
L8c644d12: r[1]=rd(m,r[14]+0x24u,4); /* mov.l @(0x24,r14),r1 */
 goto L8c644d14;
L8c644d14: r[3]=rd(m,0x8c644d7cu,4); /* mov.l 0x8c644d7c,r3 */
 goto L8c644d16;
L8c644d16: r[15]-=4;wr(m,r[15],r[1],4); /* mov.l r1,@-r15 */
 goto L8c644d18;
L8c644d18: r[5]=rd(m,r[14]+0x1cu,4); /* mov.l @(0x1c,r14),r5 */
 goto L8c644d1a;
L8c644d1a: r[6]=rd(m,r[14]+0x20u,4); /* mov.l @(0x20,r14),r6 */
 goto L8c644d1c;
L8c644d1c: r[7]=rd(m,r[15]+0x10u,4); /* mov.l @(0x10,r15),r7 */
 goto L8c644d1e;
L8c644d1e: target=r[3];*pr=0x8c644d22u;r[4]=rd(m,r[14]+0x18u,4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c644d22;
L8c644d22: r[15]+=16u; /* add #0x10,r15 */
 goto L8c644d24;
L8c644d24: *sr=(*sr&~1u)|((int32_t)r[0]>=0); /* cmp/pz r0 */
 goto L8c644d26;
L8c644d26: take=!(*sr&1u);wr(m,r[14]+0x10u,r[0],4);if(take)goto L8c644d2e;goto L8c644d2a; /* bf/s 0x8c644d2e */
 goto L8c644d2a;
L8c644d2a: r[0]&=0x1u;goto L8c644d38; /* bra 0x8c644d38 */
 goto L8c644d2e;
L8c644d2e: r[0]=~r[0]; /* not r0,r0 */
 goto L8c644d30;
L8c644d30: r[0]+=1u; /* add #0x1,r0 */
 goto L8c644d32;
L8c644d32: r[0]&=0x1u; /* and #0x1,r0 */
 goto L8c644d34;
L8c644d34: r[0]=~r[0]; /* not r0,r0 */
 goto L8c644d36;
L8c644d36: r[0]+=1u; /* add #0x1,r0 */
 goto L8c644d38;
L8c644d38: *sr=(*sr&~1u)|(r[0]==0x00000001u); /* cmp/eq #0x1,r0 */
 goto L8c644d3a;
L8c644d3a: if(!(*sr&1u))goto L8c644d40; /* bf 0x8c644d40 */
 goto L8c644d3c;
L8c644d3c: *pr=0x8c644d40u;status=call(ctx,0x8c644c90u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c644c90 */
 goto L8c644d40;
L8c644d40: r[3]=3u; /* mov #0x3,r3 */
 goto L8c644d42;
L8c644d42: wr(m,r[14]+0xcu,r[3],4); /* mov.l r3,@(0xc,r14) */
 goto L8c644d44;
L8c644d44: r[15]+=8u; /* add #0x8,r15 */
 goto L8c644d46;
L8c644d46: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c644d48;
L8c644d48: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
