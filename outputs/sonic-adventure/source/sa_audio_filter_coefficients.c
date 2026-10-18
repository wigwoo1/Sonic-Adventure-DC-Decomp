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
/* Retail coefficient routine65adc0..65ae44.
 * Native binary32 arithmetic; trigonometric callee10f90c external.
 * PR/SZ/RM0 supported within verification scope. */
SA_STAGE_EXPORT int sa_audio_filter_coefficients(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x180000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c65adc0;
L8c65adc0: r[15]-=4;wr(m,r[15],fr[12],4); /* fmov.s fr12,@-r15 */
 goto L8c65adc2;
L8c65adc2: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c65adc4;
L8c65adc4: r[15]+=4294967280u; /* add #-0x10,r15 */
 goto L8c65adc6;
L8c65adc6: fr[3]=0x3f800000u; /* fldi1 fr3 */
 goto L8c65adc8;
L8c65adc8: status=sa_sh4_add_sub(&fr[3],&fr[3],&f->fpscr,&fr[3],0);if(status)return status; /* fadd fr3,fr3 */
 goto L8c65adca;
L8c65adca: r[0]=0x8c65af44u; /* mova 0x8c65af44,r0 */
 goto L8c65adcc;
L8c65adcc: wr(m,r[15]+0xcu,r[4],4); /* mov.l r4,@(0xc,r15) */
 goto L8c65adce;
L8c65adce: wr(m,r[15]+0x8u,r[5],4); /* mov.l r5,@(0x8,r15) */
 goto L8c65add0;
L8c65add0: wr(m,r[15]+0x4u,r[6],4); /* mov.l r6,@(0x4,r15) */
 goto L8c65add2;
L8c65add2: status=sa_sh4_sqrt(fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fsqrt fr3 */
 goto L8c65add4;
L8c65add4: wr(m,r[15],r[7],4); /* mov.l r7,@r15 */
 goto L8c65add6;
L8c65add6: r[3]=rd(m,r[15]+0xcu,4); /* mov.l @(0xc,r15),r3 */
 goto L8c65add8;
L8c65add8: fr[1]=rd(m,r[0],4); /* fmov.s @r0,fr1 */
 goto L8c65adda;
L8c65adda: *fpul=r[3]; /* lds r3,FPUL */
 goto L8c65addc;
L8c65addc: r[3]=rd(m,r[15]+0x8u,4); /* mov.l @(0x8,r15),r3 */
 goto L8c65adde;
L8c65adde: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[2]);if(status)return status; /* float FPUL,fr2 */
 goto L8c65ade0;
L8c65ade0: *fpul=r[3]; /* lds r3,FPUL */
 goto L8c65ade2;
L8c65ade2: r[3]=rd(m,0x8c65af48u,4); /* mov.l 0x8c65af48,r3 */
 goto L8c65ade4;
L8c65ade4: status=sa_sh4_float_i32(*fpul,&f->fpscr,&fr[0]);if(status)return status; /* float FPUL,fr0 */
 goto L8c65ade6;
L8c65ade6: status=sa_sh4_multiply(&fr[2],&fr[1],&f->fpscr,&fr[2]);if(status)return status; /* fmul fr1,fr2 */
 goto L8c65ade8;
L8c65ade8: fr[12]=fr[3]; /* fmov fr3,fr12 */
 goto L8c65adea;
L8c65adea: fr[4]=fr[2]; /* fmov fr2,fr4 */
 goto L8c65adec;
L8c65adec: target=r[3];*pr=0x8c65adf0u;status=sa_sh4_divide(&fr[4],&fr[0],&f->fpscr,&fr[4]);if(status)return status;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;if(f->fpscr&0x180000u)return -1;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c65adf0;
L8c65adf0: fr[3]=0x3f800000u; /* fldi1 fr3 */
 goto L8c65adf2;
L8c65adf2: status=sa_sh4_add_sub(&fr[3],&fr[3],&f->fpscr,&fr[3],0);if(status)return status; /* fadd fr3,fr3 */
 goto L8c65adf4;
L8c65adf4: fr[2]=0x3f800000u; /* fldi1 fr2 */
 goto L8c65adf6;
L8c65adf6: status=sa_sh4_add_sub(&fr[12],&fr[0],&f->fpscr,&fr[12],1);if(status)return status; /* fsub fr0,fr12 */
 goto L8c65adf8;
L8c65adf8: fr[2]^=0x80000000u; /* fneg fr2 */
 goto L8c65adfa;
L8c65adfa: r[0]=0x8c65af4cu; /* mova 0x8c65af4c,r0 */
 goto L8c65adfc;
L8c65adfc: r[3]=rd(m,r[15]+0x4u,4); /* mov.l @(0x4,r15),r3 */
 goto L8c65adfe;
L8c65adfe: status=sa_sh4_sqrt(fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fsqrt fr3 */
 goto L8c65ae00;
L8c65ae00: fr[1]=fr[12]; /* fmov fr12,fr1 */
 goto L8c65ae02;
L8c65ae02: fr[0]=fr[12]; /* fmov fr12,fr0 */
 goto L8c65ae04;
L8c65ae04: fr[5]=fr[12]; /* fmov fr12,fr5 */
 goto L8c65ae06;
L8c65ae06: fr[4]=fr[3]; /* fmov fr3,fr4 */
 goto L8c65ae08;
L8c65ae08: status=sa_sh4_add_sub(&fr[4],&fr[2],&f->fpscr,&fr[4],0);if(status)return status; /* fadd fr2,fr4 */
 goto L8c65ae0a;
L8c65ae0a: fr[3]=fr[12]; /* fmov fr12,fr3 */
 goto L8c65ae0c;
L8c65ae0c: status=sa_sh4_add_sub(&fr[1],&fr[4],&f->fpscr,&fr[1],1);if(status)return status; /* fsub fr4,fr1 */
 goto L8c65ae0e;
L8c65ae0e: status=sa_sh4_add_sub(&fr[3],&fr[4],&f->fpscr,&fr[3],0);if(status)return status; /* fadd fr4,fr3 */
 goto L8c65ae10;
L8c65ae10: status=sa_sh4_multiply(&fr[3],&fr[1],&f->fpscr,&fr[3]);if(status)return status; /* fmul fr1,fr3 */
 goto L8c65ae12;
L8c65ae12: status=sa_sh4_sqrt(fr[3],&f->fpscr,&fr[3]);if(status)return status; /* fsqrt fr3 */
 goto L8c65ae14;
L8c65ae14: status=sa_sh4_add_sub(&fr[0],&fr[3],&f->fpscr,&fr[0],1);if(status)return status; /* fsub fr3,fr0 */
 goto L8c65ae16;
L8c65ae16: fr[3]=fr[4]; /* fmov fr4,fr3 */
 goto L8c65ae18;
L8c65ae18: fr[4]=fr[0]; /* fmov fr0,fr4 */
 goto L8c65ae1a;
L8c65ae1a: status=sa_sh4_divide(&fr[4],&fr[3],&f->fpscr,&fr[4]);if(status)return status; /* fdiv fr3,fr4 */
 goto L8c65ae1c;
L8c65ae1c: fr[3]=rd(m,r[0],4); /* fmov.s @r0,fr3 */
 goto L8c65ae1e;
L8c65ae1e: r[0]=0x8c65af50u; /* mova 0x8c65af50,r0 */
 goto L8c65ae20;
L8c65ae20: fr[1]=fr[4]; /* fmov fr4,fr1 */
 goto L8c65ae22;
L8c65ae22: status=sa_sh4_multiply(&fr[1],&fr[3],&f->fpscr,&fr[1]);if(status)return status; /* fmul fr3,fr1 */
 goto L8c65ae24;
L8c65ae24: status=sa_sh4_truncate_i32(fr[1],&f->fpscr,fpul);if(status)return status; /* ftrc fr1,FPUL */
 goto L8c65ae26;
L8c65ae26: fr[1]=fr[4]; /* fmov fr4,fr1 */
 goto L8c65ae28;
L8c65ae28: status=sa_sh4_multiply(&fr[1],&fr[4],&f->fpscr,&fr[1]);if(status)return status; /* fmul fr4,fr1 */
 goto L8c65ae2a;
L8c65ae2a: r[2]=*fpul; /* sts FPUL,r2 */
 goto L8c65ae2c;
L8c65ae2c: wr(m,r[3],r[2],2); /* mov.w r2,@r3 */
 goto L8c65ae2e;
L8c65ae2e: r[3]=rd(m,r[15],4); /* mov.l @r15,r3 */
 goto L8c65ae30;
L8c65ae30: fr[0]=rd(m,r[0],4); /* fmov.s @r0,fr0 */
 goto L8c65ae32;
L8c65ae32: status=sa_sh4_multiply(&fr[1],&fr[0],&f->fpscr,&fr[1]);if(status)return status; /* fmul fr0,fr1 */
 goto L8c65ae34;
L8c65ae34: fr[1]^=0x80000000u; /* fneg fr1 */
 goto L8c65ae36;
L8c65ae36: status=sa_sh4_truncate_i32(fr[1],&f->fpscr,fpul);if(status)return status; /* ftrc fr1,FPUL */
 goto L8c65ae38;
L8c65ae38: r[2]=*fpul; /* sts FPUL,r2 */
 goto L8c65ae3a;
L8c65ae3a: wr(m,r[3],r[2],2); /* mov.w r2,@r3 */
 goto L8c65ae3c;
L8c65ae3c: r[15]+=16u; /* add #0x10,r15 */
 goto L8c65ae3e;
L8c65ae3e: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c65ae40;
L8c65ae40: fr[12]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
