#include "fpu_cleanup_transfer.h"
#include "input_io.h"
int sa_sh4_cache_writeback(void *,uint32_t);
int sa_sh4_float_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
int sa_sh4_multiply(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
int sa_tikal_transition(uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,const struct sa_input_io *io,uint32_t a,unsigned n){if((a&0x1fffffffu)<0x0c000000u||(a&0x1fffffffu)>=0x0d000000u)return io->read(io->user,a,n);uint32_t v=0;for(unsigned i=0;i<n;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(i*8);return v;}
static uint32_t sx(uint8_t *m,const struct sa_input_io *io,uint32_t a,unsigned n){uint32_t v=rd(m,io,a,n);return n==1?(uint32_t)(int32_t)(int8_t)v:(uint32_t)(int32_t)(int16_t)v;}
static void wr(uint8_t *m,const struct sa_input_io *io,uint32_t a,uint32_t v,unsigned n){if((a&0x1fffffffu)<0x0c000000u||(a&0x1fffffffu)>=0x0d000000u){io->write(io->user,a,v,n);return;}for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(i*8));}
static int gt(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;unsigned t;if(!(a&0x7fffffffu)&&!(b&0x7fffffffu))t=0;else if((a^b)&0x80000000u)t=!(a&0x80000000u);else t=(a&0x80000000u)?a<b:a>b;*sr=(*sr&~1u)|t;return 0;
}
static int eq(struct sa_fpu_banks *f,uint32_t *sr,uint32_t a,uint32_t b){
 for(unsigned i=0;i<2;i++){uint32_t v=i?a:b,e=v&0x7f800000u;if(e==0x7f800000u||(!e&&(v&0x7fffffu)))return -1;}
 f->fpscr&=~0x3f000u;*sr=(*sr&~1u)|((a==b)||(!(a&0x7fffffffu)&&!(b&0x7fffffffu)));return 0;
}
/* Retail buffer header 8c665f42..8c665fa8, including FPSCR.SZ transfers. */
SA_STAGE_EXPORT int sa_engine_render_buffer_header(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,const struct sa_input_io *io,void *ctx,int (*call)(void *,uint32_t,uint8_t *,struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *)){
 (void)m;(void)fpul;(void)macl;(void)gt;(void)eq;(void)sx;(void)wr;(void)sr;(void)pr;(void)ctx;(void)call;
 if(f->fpscr&0x80000u)return -1;uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status;uint32_t target;(void)fr;(void)sizeof(status);(void)sizeof(target);
 goto L8c665f42;
L8c665f42: r[3]=r[4]; /* mov r4,r3 */
 goto L8c665f44;
L8c665f44: r[3]-=r[5]; /* sub r5,r3 */
 goto L8c665f46;
L8c665f46: r[3]>>=2; /* shlr2 r3 */
 goto L8c665f48;
L8c665f48: r[1]=rd(m,io,0x8c665ff4u,4); /* mov.l 0x8c665ff4,r1 */
 goto L8c665f4a;
L8c665f4a: r[6]=rd(m,io,0x8c665ff0u,4); /* mov.l 0x8c665ff0,r6 */
 goto L8c665f4c;
L8c665f4c: r[3]+=2u; /* add #0x2,r3 */
 goto L8c665f4e;
L8c665f4e: r[2]=rd(m,io,0x8c665ff8u,4); /* mov.l 0x8c665ff8,r2 */
 goto L8c665f50;
L8c665f50: r[3]|=r[1]; /* or r1,r3 */
 goto L8c665f52;
L8c665f52: r[6]|=r[4]; /* or r4,r6 */
 goto L8c665f54;
L8c665f54: fr[4]=0; /* fldi0 fr4 */
 goto L8c665f56;
L8c665f56: wr(m,io,r[6],r[3],4); /* mov.l r3,@r6 */
 goto L8c665f58;
L8c665f58: r[0]=12u; /* mov #0xc,r0 */
 goto L8c665f5a;
L8c665f5a: wr(m,io,r[6]+0x4u,r[2],4); /* mov.l r2,@(0x4,r6) */
 goto L8c665f5c;
L8c665f5c: r[6]+=8u; /* add #0x8,r6 */
 goto L8c665f5e;
L8c665f5e: r[3]=rd(m,io,0x8c665ffcu,4); /* mov.l 0x8c665ffc,r3 */
 goto L8c665f60;
L8c665f60: r[2]=rd(m,io,0x8c666000u,4); /* mov.l 0x8c666000,r2 */
 goto L8c665f62;
L8c665f62: wr(m,io,r[6],r[3],4); /* mov.l r3,@r6 */
 goto L8c665f64;
L8c665f64: r[3]=0u; /* mov #0x0,r3 */
 goto L8c665f66;
L8c665f66: wr(m,io,r[6]+0x4u,r[2],4); /* mov.l r2,@(0x4,r6) */
 goto L8c665f68;
L8c665f68: wr(m,io,r[6]+0x8u,r[3],4); /* mov.l r3,@(0x8,r6) */
 goto L8c665f6a;
L8c665f6a: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(4u&1u);unsigned pair=4u&~1u;wr(m,io,r[0]+r[6],f->bank[bank][pair],4);wr(m,io,r[0]+r[6]+4,f->bank[bank][pair+1],4);}else wr(m,io,r[0]+r[6],fr[4],4); /* fmov fr4,@(r0,r6) */
 goto L8c665f6c;
L8c665f6c: r[0]=16u; /* mov #0x10,r0 */
 goto L8c665f6e;
L8c665f6e: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(4u&1u);unsigned pair=4u&~1u;wr(m,io,r[0]+r[6],f->bank[bank][pair],4);wr(m,io,r[0]+r[6]+4,f->bank[bank][pair+1],4);}else wr(m,io,r[0]+r[6],fr[4],4); /* fmov fr4,@(r0,r6) */
 goto L8c665f70;
L8c665f70: r[0]=0x8c666004u; /* mova 0x8c666004,r0 */
 goto L8c665f72;
L8c665f72: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^1u;f->bank[bank][4]=rd(m,io,r[0],4);f->bank[bank][5]=rd(m,io,r[0]+4,4);}else fr[5]=rd(m,io,r[0],4); /* fmov.s @r0,fr5 */
 goto L8c665f74;
L8c665f74: r[0]=20u; /* mov #0x14,r0 */
 goto L8c665f76;
L8c665f76: r[3]=rd(m,io,0x8c666008u,4); /* mov.l 0x8c666008,r3 */
 goto L8c665f78;
L8c665f78: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(5u&1u);unsigned pair=5u&~1u;wr(m,io,r[0]+r[6],f->bank[bank][pair],4);wr(m,io,r[0]+r[6]+4,f->bank[bank][pair+1],4);}else wr(m,io,r[0]+r[6],fr[5],4); /* fmov fr5,@(r0,r6) */
 goto L8c665f7a;
L8c665f7a: r[0]=28u; /* mov #0x1c,r0 */
 goto L8c665f7c;
L8c665f7c: wr(m,io,r[6]+0x18u,r[3],4); /* mov.l r3,@(0x18,r6) */
 goto L8c665f7e;
L8c665f7e: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(4u&1u);unsigned pair=4u&~1u;wr(m,io,r[0]+r[6],f->bank[bank][pair],4);wr(m,io,r[0]+r[6]+4,f->bank[bank][pair+1],4);}else wr(m,io,r[0]+r[6],fr[4],4); /* fmov fr4,@(r0,r6) */
 goto L8c665f80;
L8c665f80: r[0]=0x8c66600cu; /* mova 0x8c66600c,r0 */
 goto L8c665f82;
L8c665f82: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^1u;f->bank[bank][2]=rd(m,io,r[0],4);f->bank[bank][3]=rd(m,io,r[0]+4,4);}else fr[3]=rd(m,io,r[0],4); /* fmov.s @r0,fr3 */
 goto L8c665f84;
L8c665f84: r[0]=32u; /* mov #0x20,r0 */
 goto L8c665f86;
L8c665f86: r[3]=rd(m,io,0x8c666010u,4); /* mov.l 0x8c666010,r3 */
 goto L8c665f88;
L8c665f88: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(3u&1u);unsigned pair=3u&~1u;wr(m,io,r[0]+r[6],f->bank[bank][pair],4);wr(m,io,r[0]+r[6]+4,f->bank[bank][pair+1],4);}else wr(m,io,r[0]+r[6],fr[3],4); /* fmov fr3,@(r0,r6) */
 goto L8c665f8a;
L8c665f8a: r[0]=36u; /* mov #0x24,r0 */
 goto L8c665f8c;
L8c665f8c: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(5u&1u);unsigned pair=5u&~1u;wr(m,io,r[0]+r[6],f->bank[bank][pair],4);wr(m,io,r[0]+r[6]+4,f->bank[bank][pair+1],4);}else wr(m,io,r[0]+r[6],fr[5],4); /* fmov fr5,@(r0,r6) */
 goto L8c665f8e;
L8c665f8e: r[0]=0x8c666014u; /* mova 0x8c666014,r0 */
 goto L8c665f90;
L8c665f90: wr(m,io,r[6]+0x28u,r[3],4); /* mov.l r3,@(0x28,r6) */
 goto L8c665f92;
L8c665f92: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^1u;f->bank[bank][2]=rd(m,io,r[0],4);f->bank[bank][3]=rd(m,io,r[0]+4,4);}else fr[3]=rd(m,io,r[0],4); /* fmov.s @r0,fr3 */
 goto L8c665f94;
L8c665f94: r[0]=44u; /* mov #0x2c,r0 */
 goto L8c665f96;
L8c665f96: r[3]=rd(m,io,0x8c666018u,4); /* mov.l 0x8c666018,r3 */
 goto L8c665f98;
L8c665f98: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(3u&1u);unsigned pair=3u&~1u;wr(m,io,r[0]+r[6],f->bank[bank][pair],4);wr(m,io,r[0]+r[6]+4,f->bank[bank][pair+1],4);}else wr(m,io,r[0]+r[6],fr[3],4); /* fmov fr3,@(r0,r6) */
 goto L8c665f9a;
L8c665f9a: r[0]=48u; /* mov #0x30,r0 */
 goto L8c665f9c;
L8c665f9c: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(4u&1u);unsigned pair=4u&~1u;wr(m,io,r[0]+r[6],f->bank[bank][pair],4);wr(m,io,r[0]+r[6]+4,f->bank[bank][pair+1],4);}else wr(m,io,r[0]+r[6],fr[4],4); /* fmov fr4,@(r0,r6) */
 goto L8c665f9e;
L8c665f9e: r[0]=52u; /* mov #0x34,r0 */
 goto L8c665fa0;
L8c665fa0: if(f->fpscr&0x100000u){unsigned bank=((f->fpscr>>21)&1u)^(5u&1u);unsigned pair=5u&~1u;wr(m,io,r[0]+r[6],f->bank[bank][pair],4);wr(m,io,r[0]+r[6]+4,f->bank[bank][pair+1],4);}else wr(m,io,r[0]+r[6],fr[5],4); /* fmov fr5,@(r0,r6) */
 goto L8c665fa2;
L8c665fa2: wr(m,io,r[6]+0x38u,r[3],4); /* mov.l r3,@(0x38,r6) */
 goto L8c665fa4;
L8c665fa4: r[0]=r[4];return 0; /* rts */
}

