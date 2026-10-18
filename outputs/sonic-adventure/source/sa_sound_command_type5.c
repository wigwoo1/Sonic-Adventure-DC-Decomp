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
/* Retail type-5 sound parameter command, byte input biased by 128. */
SA_STAGE_EXPORT int sa_sound_command_type5(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c643d88;
L8c643d88: r[2]=rd(m,0x8c643de4u,4); /* mov.l 0x8c643de4,r2 */
 goto L8c643d8a;
L8c643d8a: r[15]-=4;wr(m,r[15],r[14],4); /* mov.l r14,@-r15 */
 goto L8c643d8c;
L8c643d8c: r[15]-=4;wr(m,r[15],*pr,4); /* sts.l PR,@-r15 */
 goto L8c643d8e;
L8c643d8e: r[3]=rd(m,r[2],4); /* mov.l @r2,r3 */
 goto L8c643d90;
L8c643d90: *sr=(*sr&~1u)|!(r[3]&r[3]); /* tst r3,r3 */
 goto L8c643d92;
L8c643d92: if(!(*sr&1u))goto L8c643d9a; /* bf 0x8c643d9a */
 goto L8c643d94;
L8c643d94: r[14]=rd(m,0x8c643de8u,4); /* mov.l 0x8c643de8,r14 */
 goto L8c643d96;
L8c643d96: goto L8c643dce; /* bra 0x8c643dce */
 goto L8c643d9a;
L8c643d9a: *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c643d9c;
L8c643d9c: if(!(*sr&1u))goto L8c643da4; /* bf 0x8c643da4 */
 goto L8c643d9e;
L8c643d9e: r[14]=rd(m,0x8c643decu,4); /* mov.l 0x8c643dec,r14 */
 goto L8c643da0;
L8c643da0: goto L8c643dce; /* bra 0x8c643dce */
 goto L8c643da4;
L8c643da4: r[3]=sx(m,0x8c643de2u,2); /* mov.w 0x8c643de2,r3 */
 goto L8c643da6;
L8c643da6: r[5]=(uint32_t)(int32_t)(int8_t)r[5]; /* exts.b r5,r5 */
 goto L8c643da8;
L8c643da8: r[1]=rd(m,0x8c643df0u,4); /* mov.l 0x8c643df0,r1 */
 goto L8c643daa;
L8c643daa: r[2]=5u; /* mov #0x5,r2 */
 goto L8c643dac;
L8c643dac: r[5]+=r[3]; /* add r3,r5 */
 goto L8c643dae;
L8c643dae: r[3]=rd(m,0x8c643df4u,4); /* mov.l 0x8c643df4,r3 */
 goto L8c643db0;
L8c643db0: wr(m,r[1],r[2],4); /* mov.l r2,@r1 */
 goto L8c643db2;
L8c643db2: r[1]+=7u; /* add #0x7,r1 */
 goto L8c643db4;
L8c643db4: r[0]=sx(m,r[4]+0x1u,1); /* mov.b @(0x1,r4),r0 */
 goto L8c643db6;
L8c643db6: r[2]=rd(m,0x8c643df8u,4); /* mov.l 0x8c643df8,r2 */
 goto L8c643db8;
L8c643db8: wr(m,r[3],r[0],1); /* mov.b r0,@r3 */
 goto L8c643dba;
L8c643dba: r[3]+=1u; /* add #0x1,r3 */
 goto L8c643dbc;
L8c643dbc: wr(m,r[3],r[5],1); /* mov.b r5,@r3 */
 goto L8c643dbe;
L8c643dbe: r[3]=4294967288u; /* mov #-0x8,r3 */
 goto L8c643dc0;
L8c643dc0: wr(m,r[2],r[6],1); /* mov.b r6,@r2 */
 goto L8c643dc2;
L8c643dc2: {uint32_t shift=r[3];if(!(shift&0x80000000u))r[6]<<=shift&31u;else{unsigned n=(0u-shift)&31u;r[6]=n?(uint32_t)((int32_t)r[6]>>n):((r[6]&0x80000000u)?0xffffffffu:0u);}} /* shad r3,r6 */
 goto L8c643dc4;
L8c643dc4: r[3]=rd(m,0x8c643dfcu,4); /* mov.l 0x8c643dfc,r3 */
 goto L8c643dc6;
L8c643dc6: r[4]=rd(m,0x8c643df0u,4); /* mov.l 0x8c643df0,r4 */
 goto L8c643dc8;
L8c643dc8: target=r[3];*pr=0x8c643dccu;wr(m,r[1],r[6],1);status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643dcc;
L8c643dcc: r[14]=r[0]; /* mov r0,r14 */
 goto L8c643dce;
L8c643dce: *sr=(*sr&~1u)|!(r[14]&r[14]); /* tst r14,r14 */
 goto L8c643dd0;
L8c643dd0: if((*sr&1u))goto L8c643dda; /* bt 0x8c643dda */
 goto L8c643dd2;
L8c643dd2: r[3]=rd(m,0x8c643e04u,4); /* mov.l 0x8c643e04,r3 */
 goto L8c643dd4;
L8c643dd4: r[4]=rd(m,0x8c643e00u,4); /* mov.l 0x8c643e00,r4 */
 goto L8c643dd6;
L8c643dd6: target=r[3];*pr=0x8c643ddau;r[5]=r[14];status=call(ctx,target,m,f,sr,r,pr);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* jsr @r3 */
 goto L8c643dda;
L8c643dda: *pr=rd(m,r[15],4);r[15]+=4; /* lds.l @r15+,PR */
 goto L8c643ddc;
L8c643ddc: r[0]=r[14]; /* mov r14,r0 */
 goto L8c643dde;
L8c643dde: r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
