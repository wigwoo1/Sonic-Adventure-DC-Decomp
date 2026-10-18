#include "fpu_cleanup_transfer.h"
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
/* Static C recovery of the USA Rev A main executable. External calls use the supplied dispatcher. */
SA_STAGE_EXPORT int sa_engine_queue_consume_context(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c666d10;
L8c666d10: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c666d12;
L8c666d12: r[15]-=4;wr(m,r[15],r[13],4); /* mov.l r13,@-r15 */
 goto L8c666d14;
L8c666d14: r[15]-=4;wr(m,r[15],r[12],4); /* mov.l r12,@-r15 */
 goto L8c666d16;
L8c666d16: r[15]-=4;wr(m,r[15],r[11],4); /* mov.l r11,@-r15 */
 goto L8c666d18;
L8c666d18: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c666d1a;
L8c666d1a: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c666d1c;
L8c666d1c: r[15]+=4294967284u; /* add #-0xc,r15 */
 goto L8c666d1e;
L8c666d1e: r[0]=*sr; /* stc SR,r0 */
 goto L8c666d20;
L8c666d20: r[3]=sx(m,0x8c666e10u,2); /* mov.w 0x8c666e10,r3 */
 goto L8c666d22;
L8c666d22: r[13]=r[4]; /* mov r4,r13 */
 goto L8c666d24;
L8c666d24: wr(m,r[15]+0x8u,r[5],4); /* mov.l r5,@(0x8,r15) */
 goto L8c666d26;
L8c666d26: r[0]>>=2; /* shlr2 r0 */
 goto L8c666d28;
L8c666d28: r[0]>>=2; /* shlr2 r0 */
 goto L8c666d2a;
L8c666d2a: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c666d2c;
L8c666d2c: wr(m,r[15]+0x4u,r[0],4); /* mov.l r0,@(0x4,r15) */
 goto L8c666d2e;
L8c666d2e: r[0]=*sr; /* stc SR,r0 */
 goto L8c666d30;
L8c666d30: r[0]&=r[3]; /* and r3,r0 */
 goto L8c666d32;
L8c666d32: r[0]|=0xf0u; /* or #0xf0,r0 */
 goto L8c666d34;
L8c666d34: *sr=r[0]; /* ldc r0,SR */
 goto L8c666d36;
L8c666d36: r[12]=rd(m,0x8c666e18u,4); /* mov.l 0x8c666e18,r12 */
 goto L8c666d38;
L8c666d38: r[2]=rd(m,r[12],4); /* mov.l @r12,r2 */
 goto L8c666d3a;
L8c666d3a: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c666d3c;
L8c666d3c: if(!(*sr&1u))goto L8c666d44; /* bf 0x8c666d44 */
 goto L8c666d3e;
L8c666d3e: r[14]=rd(m,0x8c666e1cu,4); /* mov.l 0x8c666e1c,r14 */
 goto L8c666d40;
L8c666d40: goto L8c666dd2; /* bra 0x8c666dd2 */
 goto L8c666d44;
L8c666d44: *sr=(*sr&~1u)|!(r[13]&r[13]); /* tst r13,r13 */
 goto L8c666d46;
L8c666d46: if(!(*sr&1u))goto L8c666d4e; /* bf 0x8c666d4e */
 goto L8c666d48;
L8c666d48: r[14]=rd(m,0x8c666e20u,4); /* mov.l 0x8c666e20,r14 */
 goto L8c666d4a;
L8c666d4a: goto L8c666dd2; /* bra 0x8c666dd2 */
 goto L8c666d4e;
L8c666d4e: r[3]=rd(m,0x8c666e24u,4); /* mov.l 0x8c666e24,r3 */
 goto L8c666d50;
L8c666d50: r[5]=r[15]; /* mov r15,r5 */
 goto L8c666d52;
L8c666d52: target=r[3];*pr=0x8c666d56u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666d56;
L8c666d56: r[0]=rd(m,r[15],4); /* mov.l @r15,r0 */
 goto L8c666d58;
L8c666d58: *sr=(*sr&~1u)|(r[0]==0x2u); /* cmp/eq #0x2,r0 */
 goto L8c666d5a;
L8c666d5a: if(!(*sr&1u))goto L8c666d62; /* bf 0x8c666d62 */
 goto L8c666d5c;
L8c666d5c: r[14]=rd(m,0x8c666e28u,4); /* mov.l 0x8c666e28,r14 */
 goto L8c666d5e;
L8c666d5e: goto L8c666dd2; /* bra 0x8c666dd2 */
 goto L8c666d62;
L8c666d62: r[14]=r[13]; /* mov r13,r14 */
 goto L8c666d64;
L8c666d64: r[3]=rd(m,0x8c666e30u,4); /* mov.l 0x8c666e30,r3 */
 goto L8c666d66;
L8c666d66: r[2]=1u; /* mov #0x1,r2 */
 goto L8c666d68;
L8c666d68: r[1]=rd(m,0x8c666e34u,4); /* mov.l 0x8c666e34,r1 */
 goto L8c666d6a;
L8c666d6a: r[10]=0u; /* mov #0x0,r10 */
 goto L8c666d6c;
L8c666d6c: r[11]=rd(m,0x8c666e2cu,4); /* mov.l 0x8c666e2c,r11 */
 goto L8c666d6e;
L8c666d6e: r[0]=sx(m,0x8c666e12u,2); /* mov.w 0x8c666e12,r0 */
 goto L8c666d70;
L8c666d70: wr(m,r[11],r[10],4); /* mov.l r10,@r11 */
 goto L8c666d72;
L8c666d72: wr(m,r[14]+0x18u,r[10],4); /* mov.l r10,@(0x18,r14) */
 goto L8c666d74;
L8c666d74: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c666d76;
L8c666d76: r[15]-=4;wr(m,r[15],r[3],4); /* mov.l r3,@-r15 */
 goto L8c666d78;
L8c666d78: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c666d7a;
L8c666d7a: r[15]-=4;wr(m,r[15],r[2],4); /* mov.l r2,@-r15 */
 goto L8c666d7c;
L8c666d7c: r[15]-=4;wr(m,r[15],r[10],4); /* mov.l r10,@-r15 */
 goto L8c666d7e;
L8c666d7e: r[5]=rd(m,r[15]+0x1cu,4); /* mov.l @(0x1c,r15),r5 */
 goto L8c666d80;
L8c666d80: r[4]=rd(m,r[12],4); /* mov.l @r12,r4 */
 goto L8c666d82;
L8c666d82: r[5]+=r[1]; /* add r1,r5 */
 goto L8c666d84;
L8c666d84: r[1]=rd(m,0x8c666e38u,4); /* mov.l 0x8c666e38,r1 */
 goto L8c666d86;
L8c666d86: r[7]=rd(m,r[14]+0xcu,4); /* mov.l @(0xc,r14),r7 */
 goto L8c666d88;
L8c666d88: r[6]=rd(m,r[14]+0x8u,4); /* mov.l @(0x8,r14),r6 */
 goto L8c666d8a;
L8c666d8a: target=r[1];*pr=0x8c666d8eu;r[4]=rd(m,r[4]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r1 */
 goto L8c666d8e;
L8c666d8e: r[15]+=20u; /* add #0x14,r15 */
 goto L8c666d90;
L8c666d90: r[3]=r[0]; /* mov r0,r3 */
 goto L8c666d92;
L8c666d92: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c666d94;
L8c666d94: take=(*sr&1u);wr(m,r[11],r[0],4);if(take)goto L8c666d9c;goto L8c666d98; /* bt/s 0x8c666d9c */
 goto L8c666d98;
L8c666d98: *pr=0x8c666d9cu;status=call(ctx,0x8c666e06u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c666e06 */
 goto L8c666d9c;
L8c666d9c: r[2]=rd(m,0x8c666e3cu,4); /* mov.l 0x8c666e3c,r2 */
 goto L8c666d9e;
L8c666d9e: r[5]=2u; /* mov #0x2,r5 */
 goto L8c666da0;
L8c666da0: target=r[2];*pr=0x8c666da4u;r[4]=r[13];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r2 */
 goto L8c666da4;
L8c666da4: r[14]=r[0]; /* mov r0,r14 */
 goto L8c666da6;
L8c666da6: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c666da8;
L8c666da8: if(!(*sr&1u))goto L8c666dd2; /* bf 0x8c666dd2 */
 goto L8c666daa;
L8c666daa: r[0]=sx(m,0x8c666e14u,2); /* mov.w 0x8c666e14,r0 */
 goto L8c666dac;
L8c666dac: r[5]=0u; /* mov #0x0,r5 */
 goto L8c666dae;
L8c666dae: r[3]=rd(m,r[12],4); /* mov.l @r12,r3 */
 goto L8c666db0;
L8c666db0: wr(m,r[3]+r[0],r[13],4); /* mov.l r13,@(r0,r3) */
 goto L8c666db2;
L8c666db2: r[0]+=4294967288u; /* add #-0x8,r0 */
 goto L8c666db4;
L8c666db4: r[3]=rd(m,r[12],4); /* mov.l @r12,r3 */
 goto L8c666db6;
L8c666db6: r[2]=rd(m,r[3]+r[0],4); /* mov.l @(r0,r3),r2 */
 goto L8c666db8;
L8c666db8: r[2]+=1u; /* add #0x1,r2 */
 goto L8c666dba;
L8c666dba: wr(m,r[3]+r[0],r[2],4); /* mov.l r2,@(r0,r3) */
 goto L8c666dbc;
L8c666dbc: r[0]+=36u; /* add #0x24,r0 */
 goto L8c666dbe;
L8c666dbe: r[4]=rd(m,r[12],4); /* mov.l @r12,r4 */
 goto L8c666dc0;
L8c666dc0: r[3]=rd(m,0x8c666e40u,4); /* mov.l 0x8c666e40,r3 */
 goto L8c666dc2;
L8c666dc2: target=r[3];*pr=0x8c666dc6u;r[4]=rd(m,r[4]+r[0],4);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666dc6;
L8c666dc6: r[2]=r[0]; /* mov r0,r2 */
 goto L8c666dc8;
L8c666dc8: *sr=(*sr&~1u)|!(r[2]&r[2]); /* tst r2,r2 */
 goto L8c666dca;
L8c666dca: take=(*sr&1u);wr(m,r[11],r[0],4);if(take)goto L8c666dd2;goto L8c666dce; /* bt/s 0x8c666dd2 */
 goto L8c666dce;
L8c666dce: *pr=0x8c666dd2u;status=call(ctx,0x8c666e06u,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* bsr 0x8c666e06 */
 goto L8c666dd2;
L8c666dd2: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c666dd4;
L8c666dd4: if((*sr&1u))goto L8c666dde; /* bt 0x8c666dde */
 goto L8c666dd6;
L8c666dd6: r[3]=rd(m,0x8c666e48u,4); /* mov.l 0x8c666e48,r3 */
 goto L8c666dd8;
L8c666dd8: r[4]=rd(m,0x8c666e44u,4); /* mov.l 0x8c666e44,r4 */
 goto L8c666dda;
L8c666dda: target=r[3];*pr=0x8c666ddeu;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c666dde;
L8c666dde: r[0]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r0 */
 goto L8c666de0;
L8c666de0: r[2]=*sr; /* stc SR,r2 */
 goto L8c666de2;
L8c666de2: r[3]=sx(m,0x8c666e10u,2); /* mov.w 0x8c666e10,r3 */
 goto L8c666de4;
L8c666de4: r[0]&=0xfu; /* and #0xf,r0 */
 goto L8c666de6;
L8c666de6: r[0]<<=2; /* shll2 r0 */
 goto L8c666de8;
L8c666de8: r[2]&=r[3]; /* and r3,r2 */
 goto L8c666dea;
L8c666dea: r[0]<<=2; /* shll2 r0 */
 goto L8c666dec;
L8c666dec: r[0]|=r[2]; /* or r2,r0 */
 goto L8c666dee;
L8c666dee: *sr=r[0]; /* ldc r0,SR */
 goto L8c666df0;
L8c666df0: r[0]=r[14]; /* mov r14,r0 */
 goto L8c666df2;
L8c666df2: r[15]+=12u; /* add #0xc,r15 */
 goto L8c666df4;
L8c666df4: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c666df6;
L8c666df6: r[10]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r10 */
 goto L8c666df8;
L8c666df8: r[11]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r11 */
 goto L8c666dfa;
L8c666dfa: r[12]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r12 */
 goto L8c666dfc;
L8c666dfc: r[13]=rd(m,r[15],4);r[15]+=4; /* mov.l @r15+,r13 */
 goto L8c666dfe;
L8c666dfe: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
