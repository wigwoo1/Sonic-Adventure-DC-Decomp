#include "sa_draw_motion_stop.h"
#include "fpu_cleanup_transfer.h"
typedef int (*instruction)(void *,uint32_t,struct sa_fpu_banks *,uint32_t);
static uint32_t rd(uint8_t *m,uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;i++)v|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return v;}
static void wr(uint8_t *m,uint32_t a,uint32_t v){for(unsigned i=0;i<4;i++)m[(a&0xffffffu)+i]=(uint8_t)(v>>(8*i));}
static void move(struct sa_fpu_banks *f,unsigned n,unsigned s){unsigned b=(f->fpscr>>21)&1u;
 if(!(f->fpscr&0x100000u)){f->bank[b][n]=f->bank[b][s];return;}
 unsigned sb=b^(s&1u),nb=b^(n&1u);s&=~1u;n&=~1u;uint32_t a=f->bank[sb][s],v=f->bank[sb][s+1];f->bank[nb][n]=a;f->bank[nb][n+1]=v;
}
/* Complete retail body. PR=SZ=0 at entry. FSCA/FTRV numerical service explicit. */
SA_STAGE_EXPORT int sa_matrix_rotate_z(uint8_t *m,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *fpul,void *ctx,instruction vector){
 if(f->fpscr&0x180000u||!vector)return -1;
 uint32_t *fr=f->bank[(f->fpscr>>21)&1u];int status,take;(void)take;
 goto L8c63a1cc;
L8c63a1cc: sa_motion_publish_stop(ctx,0x8c63a1ccu); *sr=(*sr&~1u)|!(r[4]&r[4]); /* tst r4,r4 */
 goto L8c63a1ce;
L8c63a1ce: sa_motion_publish_stop(ctx,0x8c63a1d0u); take=!(*sr&1u);*fpul=r[5];if(take)goto L8c63a1fa; /* bf/s 0x8c63a1fa */
 goto L8c63a1d2;
L8c63a1d2: sa_motion_publish_stop(ctx,0x8c63a1d2u); fr[3]=0; /* fldi0 fr3 */
 goto L8c63a1d4;
L8c63a1d4: sa_motion_publish_stop(ctx,0x8c63a1d4u); status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a1d6;
L8c63a1d6: sa_motion_publish_stop(ctx,0x8c63a1d6u); fr[6]=0; /* fldi0 fr6 */
 goto L8c63a1d8;
L8c63a1d8: sa_motion_publish_stop(ctx,0x8c63a1d8u); fr[7]=0; /* fldi0 fr7 */
 goto L8c63a1da;
L8c63a1da: sa_motion_publish_stop(ctx,0x8c63a1dau); move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a1dc;
L8c63a1dc: sa_motion_publish_stop(ctx,0x8c63a1dcu); move(f,0,1); /* fmov fr1,fr0 */
 goto L8c63a1de;
L8c63a1de: sa_motion_publish_stop(ctx,0x8c63a1deu); move(f,1,2); /* fmov fr2,fr1 */
 goto L8c63a1e0;
L8c63a1e0: sa_motion_publish_stop(ctx,0x8c63a1e0u); fr[2]=0; /* fldi0 fr2 */
 goto L8c63a1e2;
L8c63a1e2: sa_motion_publish_stop(ctx,0x8c63a1e2u); move(f,5,0); /* fmov fr0,fr5 */
 goto L8c63a1e4;
L8c63a1e4: sa_motion_publish_stop(ctx,0x8c63a1e4u); move(f,4,1); /* fmov fr1,fr4 */
 goto L8c63a1e6;
L8c63a1e6: sa_motion_publish_stop(ctx,0x8c63a1e6u); fr[4]^=0x80000000u; /* fneg fr4 */
 goto L8c63a1e8;
L8c63a1e8: sa_motion_publish_stop(ctx,0x8c63a1e8u); status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a1ea;
L8c63a1ea: sa_motion_publish_stop(ctx,0x8c63a1eau); status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a1ec;
L8c63a1ec: sa_motion_publish_stop(ctx,0x8c63a1ecu); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a1ee;
L8c63a1ee: sa_motion_publish_stop(ctx,0x8c63a1eeu); move(f,1,0); /* fmov fr0,fr1 */
 goto L8c63a1f0;
L8c63a1f0: sa_motion_publish_stop(ctx,0x8c63a1f0u); move(f,3,2); /* fmov fr2,fr3 */
 goto L8c63a1f2;
L8c63a1f2: sa_motion_publish_stop(ctx,0x8c63a1f2u); move(f,5,4); /* fmov fr4,fr5 */
 goto L8c63a1f4;
L8c63a1f4: sa_motion_publish_stop(ctx,0x8c63a1f4u); move(f,7,6); /* fmov fr6,fr7 */
 goto L8c63a1f6;
L8c63a1f6: sa_motion_publish_stop(ctx,0x8c63a1f8u); f->fpscr^=0x100000u;return 0; /* rts */
L8c63a1fa: sa_motion_publish_stop(ctx,0x8c63a1fau); fr[3]=0; /* fldi0 fr3 */
 goto L8c63a1fc;
L8c63a1fc: sa_motion_publish_stop(ctx,0x8c63a1fcu); status=vector(ctx,0xf0fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* fsca FPUL,dr0 */
 goto L8c63a1fe;
L8c63a1fe: sa_motion_publish_stop(ctx,0x8c63a1feu); fr[6]=0; /* fldi0 fr6 */
 goto L8c63a200;
L8c63a200: sa_motion_publish_stop(ctx,0x8c63a200u); fr[7]=0; /* fldi0 fr7 */
 goto L8c63a202;
L8c63a202: sa_motion_publish_stop(ctx,0x8c63a202u); move(f,2,0); /* fmov fr0,fr2 */
 goto L8c63a204;
L8c63a204: sa_motion_publish_stop(ctx,0x8c63a204u); move(f,0,1); /* fmov fr1,fr0 */
 goto L8c63a206;
L8c63a206: sa_motion_publish_stop(ctx,0x8c63a206u); move(f,1,2); /* fmov fr2,fr1 */
 goto L8c63a208;
L8c63a208: sa_motion_publish_stop(ctx,0x8c63a208u); fr[2]=0; /* fldi0 fr2 */
 goto L8c63a20a;
L8c63a20a: sa_motion_publish_stop(ctx,0x8c63a20au); move(f,5,0); /* fmov fr0,fr5 */
 goto L8c63a20c;
L8c63a20c: sa_motion_publish_stop(ctx,0x8c63a20cu); move(f,4,1); /* fmov fr1,fr4 */
 goto L8c63a20e;
L8c63a20e: sa_motion_publish_stop(ctx,0x8c63a20eu); fr[4]^=0x80000000u; /* fneg fr4 */
 goto L8c63a210;
L8c63a210: sa_motion_publish_stop(ctx,0x8c63a210u); r[15]-=4;wr(m,r[15],fr[12]); /* fmov.s fr12,@-r15 */
 goto L8c63a212;
L8c63a212: sa_motion_publish_stop(ctx,0x8c63a212u); r[15]-=4;wr(m,r[15],fr[13]); /* fmov.s fr13,@-r15 */
 goto L8c63a214;
L8c63a214: sa_motion_publish_stop(ctx,0x8c63a214u); r[15]-=4;wr(m,r[15],fr[14]); /* fmov.s fr14,@-r15 */
 goto L8c63a216;
L8c63a216: sa_motion_publish_stop(ctx,0x8c63a216u); r[15]-=4;wr(m,r[15],fr[15]); /* fmov.s fr15,@-r15 */
 goto L8c63a218;
L8c63a218: sa_motion_publish_stop(ctx,0x8c63a218u); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a21a;
L8c63a21a: sa_motion_publish_stop(ctx,0x8c63a21au); move(f,8,1); /* fmov fr1,fr8 */
 goto L8c63a21c;
L8c63a21c: sa_motion_publish_stop(ctx,0x8c63a21cu); move(f,10,3); /* fmov fr3,fr10 */
 goto L8c63a21e;
L8c63a21e: sa_motion_publish_stop(ctx,0x8c63a21eu); move(f,12,5); /* fmov fr5,fr12 */
 goto L8c63a220;
L8c63a220: sa_motion_publish_stop(ctx,0x8c63a220u); move(f,14,7); /* fmov fr7,fr14 */
 goto L8c63a222;
L8c63a222: sa_motion_publish_stop(ctx,0x8c63a222u); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a224;
L8c63a224: sa_motion_publish_stop(ctx,0x8c63a224u); f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a226;
L8c63a226: sa_motion_publish_stop(ctx,0x8c63a226u); fr[0]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr0 */
 goto L8c63a228;
L8c63a228: sa_motion_publish_stop(ctx,0x8c63a228u); fr[1]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr1 */
 goto L8c63a22a;
L8c63a22a: sa_motion_publish_stop(ctx,0x8c63a22au); fr[2]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr2 */
 goto L8c63a22c;
L8c63a22c: sa_motion_publish_stop(ctx,0x8c63a22cu); fr[3]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr3 */
 goto L8c63a22e;
L8c63a22e: sa_motion_publish_stop(ctx,0x8c63a22eu); fr[4]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr4 */
 goto L8c63a230;
L8c63a230: sa_motion_publish_stop(ctx,0x8c63a230u); fr[5]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr5 */
 goto L8c63a232;
L8c63a232: sa_motion_publish_stop(ctx,0x8c63a232u); fr[6]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr6 */
 goto L8c63a234;
L8c63a234: sa_motion_publish_stop(ctx,0x8c63a234u); fr[7]=rd(m,r[4]);r[4]+=4; /* fmov.s @r4+,fr7 */
 goto L8c63a236;
L8c63a236: sa_motion_publish_stop(ctx,0x8c63a236u); f->fpscr^=0x200000u;fr=f->bank[(f->fpscr>>21)&1u]; /* frchg */
 goto L8c63a238;
L8c63a238: sa_motion_publish_stop(ctx,0x8c63a238u); status=vector(ctx,0xf1fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv0 */
 goto L8c63a23a;
L8c63a23a: sa_motion_publish_stop(ctx,0x8c63a23au); status=vector(ctx,0xf5fdu,f,*fpul);if(status)return status;fr=f->bank[(f->fpscr>>21)&1u]; /* ftrv xmtrx,fv4 */
 goto L8c63a23c;
L8c63a23c: sa_motion_publish_stop(ctx,0x8c63a23cu); r[4]-=4;wr(m,r[4],fr[7]); /* fmov.s fr7,@-r4 */
 goto L8c63a23e;
L8c63a23e: sa_motion_publish_stop(ctx,0x8c63a23eu); r[4]-=4;wr(m,r[4],fr[6]); /* fmov.s fr6,@-r4 */
 goto L8c63a240;
L8c63a240: sa_motion_publish_stop(ctx,0x8c63a240u); r[4]-=4;wr(m,r[4],fr[5]); /* fmov.s fr5,@-r4 */
 goto L8c63a242;
L8c63a242: sa_motion_publish_stop(ctx,0x8c63a242u); r[4]-=4;wr(m,r[4],fr[4]); /* fmov.s fr4,@-r4 */
 goto L8c63a244;
L8c63a244: sa_motion_publish_stop(ctx,0x8c63a244u); r[4]-=4;wr(m,r[4],fr[3]); /* fmov.s fr3,@-r4 */
 goto L8c63a246;
L8c63a246: sa_motion_publish_stop(ctx,0x8c63a246u); r[4]-=4;wr(m,r[4],fr[2]); /* fmov.s fr2,@-r4 */
 goto L8c63a248;
L8c63a248: sa_motion_publish_stop(ctx,0x8c63a248u); r[4]-=4;wr(m,r[4],fr[1]); /* fmov.s fr1,@-r4 */
 goto L8c63a24a;
L8c63a24a: sa_motion_publish_stop(ctx,0x8c63a24au); r[4]-=4;wr(m,r[4],fr[0]); /* fmov.s fr0,@-r4 */
 goto L8c63a24c;
L8c63a24c: sa_motion_publish_stop(ctx,0x8c63a24cu); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a24e;
L8c63a24e: sa_motion_publish_stop(ctx,0x8c63a24eu); move(f,1,8); /* fmov fr8,fr1 */
 goto L8c63a250;
L8c63a250: sa_motion_publish_stop(ctx,0x8c63a250u); move(f,3,10); /* fmov fr10,fr3 */
 goto L8c63a252;
L8c63a252: sa_motion_publish_stop(ctx,0x8c63a252u); move(f,5,12); /* fmov fr12,fr5 */
 goto L8c63a254;
L8c63a254: sa_motion_publish_stop(ctx,0x8c63a254u); move(f,7,14); /* fmov fr14,fr7 */
 goto L8c63a256;
L8c63a256: sa_motion_publish_stop(ctx,0x8c63a256u); f->fpscr^=0x100000u; /* fschg */
 goto L8c63a258;
L8c63a258: sa_motion_publish_stop(ctx,0x8c63a258u); fr[15]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr15 */
 goto L8c63a25a;
L8c63a25a: sa_motion_publish_stop(ctx,0x8c63a25au); fr[14]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr14 */
 goto L8c63a25c;
L8c63a25c: sa_motion_publish_stop(ctx,0x8c63a25cu); fr[13]=rd(m,r[15]);r[15]+=4; /* fmov.s @r15+,fr13 */
 goto L8c63a25e;
L8c63a25e: sa_motion_publish_stop(ctx,0x8c63a260u); fr[12]=rd(m,r[15]);r[15]+=4;return 0; /* rts */
}
