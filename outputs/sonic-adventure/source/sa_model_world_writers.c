/* Complete register-input matrix scale and translation; names inferred.
 * Static C from byte-checked SH-4 instructions, no runtime opcode decoding.
 * FMOV obeys FR/SZ; arithmetic guards occur only at reached instructions.
 * Host 0 completes; guest results remain in registers. Missing service -2
 * only at a reached boundary; propagated errors retain partial guest state.
 */
#include "fpu_cleanup_transfer.h"
#include "sh4_add_sub.h"
#include "sh4_vector_numeric.h"
#include "sh4_fmac.h"
#include "sh4_divide.h"
#include "sh4_multiply_extended.h"
#include "sh4_float_i32.h"
#include "sh4_compare_bits.h"
int sa_sh4_truncate_i32(uint32_t,uint32_t *,uint32_t *);
typedef int (*sa_shared_service)(void *,uint32_t,uint8_t *,
    struct sa_fpu_banks *,uint32_t *,uint32_t *,uint32_t *);
static uint32_t rd(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=0;for(unsigned i=0;i<n;i++)value|=(uint32_t)m[(a&0xffffffu)+i]<<(8*i);return value;
}
static uint32_t sx(uint8_t *m,uint32_t a,unsigned n){
 uint32_t value=rd(m,a,n);return n==1?(uint32_t)(int32_t)(int8_t)value:(uint32_t)(int32_t)(int16_t)value;
}
static void wr(uint8_t *m,uint32_t a,uint32_t value,unsigned n){
 for(unsigned i=0;i<n;i++)m[(a&0xffffffu)+i]=(uint8_t)(value>>(8*i));
}
static void fload(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  f->bank[bank][reg]=rd(m,a,4);f->bank[bank][reg+1]=rd(m,a+4u,4);
 }else f->bank[bank][reg]=rd(m,a,4);
}
static void fstore(uint8_t *m,struct sa_fpu_banks *f,unsigned reg,uint32_t a){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){bank^=reg&1u;reg&=~1u;
  wr(m,a,f->bank[bank][reg],4);wr(m,a+4u,f->bank[bank][reg+1],4);
 }else wr(m,a,f->bank[bank][reg],4);
}
static void fcopy(struct sa_fpu_banks *f,unsigned n,unsigned s){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){unsigned nb=bank^(n&1u),sb=bank^(s&1u);n&=~1u;s&=~1u;
  uint32_t a=f->bank[sb][s],b=f->bank[sb][s+1];f->bank[nb][n]=a;f->bank[nb][n+1]=b;
 }else f->bank[bank][n]=f->bank[bank][s];
}
static int valid_ram(uint32_t a,unsigned n){
 return (a&0x1f000000u)==0x0c000000u && (a&0xffffffu)<=0x1000000u-n;
}
SA_STAGE_EXPORT int sa_world_reset_8c051a16(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 goto L8c051a16;
L8c051a16: *stop=0x8c051a16u;r[3]=rd(m,0x8c051b18u,4);goto L8c051a18; /* mov.l 0x8c051b18,r3 */
L8c051a18: *stop=0x8c051a18u;r[4]=0u;goto L8c051a1a; /* mov #0x0,r4 */
L8c051a1a: *stop=0x8c051a1au;r[1]=rd(m,0x8c051b20u,4);goto L8c051a1c; /* mov.l 0x8c051b20,r1 */
L8c051a1c: *stop=0x8c051a1cu;r[5]=1u;goto L8c051a1e; /* mov #0x1,r5 */
L8c051a1e: *stop=0x8c051a1eu;r[2]=rd(m,0x8c051b1cu,4);goto L8c051a20; /* mov.l 0x8c051b1c,r2 */
L8c051a20: *stop=0x8c051a20u;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[4],2);goto L8c051a22; /* mov.w r4,@r3 */
L8c051a22: *stop=0x8c051a22u;r[3]=rd(m,0x8c051b24u,4);goto L8c051a24; /* mov.l 0x8c051b24,r3 */
L8c051a24: *stop=0x8c051a24u;if(!valid_ram(r[2],1u))return -3;wr(m,r[2],r[4],1);goto L8c051a26; /* mov.b r4,@r2 */
L8c051a26: *stop=0x8c051a26u;if(!valid_ram(r[1],1u))return -3;wr(m,r[1],r[4],1);goto L8c051a28; /* mov.b r4,@r1 */
L8c051a28: *stop=0x8c051a28u;r[1]=rd(m,0x8c051b28u,4);goto L8c051a2a; /* mov.l 0x8c051b28,r1 */
L8c051a2a: *stop=0x8c051a2au;r[2]=rd(m,0x8c051b00u,4);goto L8c051a2c; /* mov.l 0x8c051b00,r2 */
L8c051a2c: *stop=0x8c051a2cu;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[4],4);goto L8c051a2e; /* mov.l r4,@r3 */
L8c051a2e: *stop=0x8c051a2eu;r[3]=rd(m,0x8c051b2cu,4);goto L8c051a30; /* mov.l 0x8c051b2c,r3 */
L8c051a30: *stop=0x8c051a30u;if(f->fpscr&0x80000u)return -1;f->bank[(f->fpscr>>21)&1u][3]=0;goto L8c051a32; /* fldi0 fr3 */
L8c051a32: *stop=0x8c051a32u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[4],4);goto L8c051a34; /* mov.l r4,@r2 */
L8c051a34: *stop=0x8c051a34u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[4],4);goto L8c051a36; /* mov.l r4,@r1 */
L8c051a36: *stop=0x8c051a36u;r[2]=rd(m,0x8c051b30u,4);goto L8c051a38; /* mov.l 0x8c051b30,r2 */
L8c051a38: *stop=0x8c051a38u;r[1]=rd(m,0x8c051b34u,4);goto L8c051a3a; /* mov.l 0x8c051b34,r1 */
L8c051a3a: *stop=0x8c051a3au;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[3]);goto L8c051a3c; /* fmov.s fr3,@r3 */
L8c051a3c: *stop=0x8c051a3cu;r[3]=rd(m,0x8c051b38u,4);goto L8c051a3e; /* mov.l 0x8c051b38,r3 */
L8c051a3e: *stop=0x8c051a3eu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[5],4);goto L8c051a40; /* mov.l r5,@r2 */
L8c051a40: *stop=0x8c051a40u;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[5],4);goto L8c051a42; /* mov.l r5,@r1 */
L8c051a42: *stop=0x8c051a42u;r[1]=rd(m,0x8c051b40u,4);goto L8c051a44; /* mov.l 0x8c051b40,r1 */
L8c051a44: *stop=0x8c051a44u;r[2]=rd(m,0x8c051b3cu,4);goto L8c051a46; /* mov.l 0x8c051b3c,r2 */
L8c051a46: *stop=0x8c051a46u;if(!valid_ram(r[3],2u))return -3;wr(m,r[3],r[4],2);goto L8c051a48; /* mov.w r4,@r3 */
L8c051a48: *stop=0x8c051a48u;r[3]=rd(m,0x8c051b44u,4);goto L8c051a4a; /* mov.l 0x8c051b44,r3 */
L8c051a4a: *stop=0x8c051a4au;if(!valid_ram(r[2],2u))return -3;wr(m,r[2],r[4],2);goto L8c051a4c; /* mov.w r4,@r2 */
L8c051a4c: *stop=0x8c051a4cu;if(!valid_ram(r[1],4u))return -3;wr(m,r[1],r[4],4);goto L8c051a4e; /* mov.l r4,@r1 */
L8c051a4e: *stop=0x8c051a4eu;if(*pr!=entry_pr)return -3;*stop=0x8c051a50u;if(!valid_ram(r[3],4u))return -3;wr(m,r[3],r[4],4);return 0; /* rts */
}

SA_STAGE_EXPORT int sa_world_collect_near_8c052518(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c052518;
L8c052518: *stop=0x8c052518u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c05251a; /* mov.l r14,@-r15 */
L8c05251a: *stop=0x8c05251au;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c05251c; /* mov.l r13,@-r15 */
L8c05251c: *stop=0x8c05251cu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c05251e; /* mov.l r12,@-r15 */
L8c05251e: *stop=0x8c05251eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c052520; /* mov.l r11,@-r15 */
L8c052520: *stop=0x8c052520u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c052522; /* mov.l r10,@-r15 */
L8c052522: *stop=0x8c052522u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[9],4);goto L8c052524; /* mov.l r9,@-r15 */
L8c052524: *stop=0x8c052524u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[8],4);goto L8c052526; /* mov.l r8,@-r15 */
L8c052526: *stop=0x8c052526u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,15u,r[15]);goto L8c052528; /* fmov.s fr15,@-r15 */
L8c052528: *stop=0x8c052528u;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,14u,r[15]);goto L8c05252a; /* fmov.s fr14,@-r15 */
L8c05252a: *stop=0x8c05252au;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,13u,r[15]);goto L8c05252c; /* fmov.s fr13,@-r15 */
L8c05252c: *stop=0x8c05252cu;if(!valid_ram(r[15]-((f->fpscr&0x100000u)?8u:4u),((f->fpscr&0x100000u)?8u:4u)))return -3;r[15]-=(f->fpscr&0x100000u)?8u:4u;fstore(m,f,12u,r[15]);goto L8c05252e; /* fmov.s fr12,@-r15 */
L8c05252e: *stop=0x8c05252eu;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],*pr,4);goto L8c052530; /* sts.l PR,@-r15 */
L8c052530: *stop=0x8c052530u;r[15]+=4294967240u;goto L8c052532; /* add #-0x38,r15 */
L8c052532: *stop=0x8c052532u;r[2]=rd(m,0x8c05260cu,4);goto L8c052534; /* mov.l 0x8c05260c,r2 */
L8c052534: *stop=0x8c052534u;r[5]=r[15];goto L8c052536; /* mov r15,r5 */
L8c052536: *stop=0x8c052536u;r[5]+=32u;goto L8c052538; /* add #0x20,r5 */
L8c052538: *stop=0x8c052538u;r[4]=rd(m,0x8c052608u,4);goto L8c05253a; /* mov.l 0x8c052608,r4 */
L8c05253a: *stop=0x8c05253au;if(!valid_ram(r[2],4u))return -3;r[3]=rd(m,r[2],4);goto L8c05253c; /* mov.l @r2,r3 */
L8c05253c: *stop=0x8c05253cu;r[14]=rd(m,0x8c052604u,4);goto L8c05253e; /* mov.l 0x8c052604,r14 */
L8c05253e: *stop=0x8c05253eu;if(!valid_ram(r[3]+32u,4u))return -3;r[1]=rd(m,r[3]+0x20u,4);goto L8c052540; /* mov.l @(0x20,r3),r1 */
L8c052540: *stop=0x8c052540u;if(!valid_ram(r[3]+36u,4u))return -3;r[0]=rd(m,r[3]+0x24u,4);goto L8c052542; /* mov.l @(0x24,r3),r0 */
L8c052542: *stop=0x8c052542u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[1],4);goto L8c052544; /* mov.l r1,@r5 */
L8c052544: *stop=0x8c052544u;if(!valid_ram(r[5]+4u,4u))return -3;wr(m,r[5]+0x4u,r[0],4);goto L8c052546; /* mov.l r0,@(0x4,r5) */
L8c052546: *stop=0x8c052546u;if(!valid_ram(r[3]+40u,4u))return -3;r[1]=rd(m,r[3]+0x28u,4);goto L8c052548; /* mov.l @(0x28,r3),r1 */
L8c052548: *stop=0x8c052548u;r[3]=1u;goto L8c05254a; /* mov #0x1,r3 */
L8c05254a: *stop=0x8c05254au;if(!valid_ram(r[5]+8u,4u))return -3;wr(m,r[5]+0x8u,r[1],4);goto L8c05254c; /* mov.l r1,@(0x8,r5) */
L8c05254c: *stop=0x8c05254cu;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[3],4);goto L8c05254e; /* mov.l r3,@(0x8,r15) */
L8c05254e: *stop=0x8c05254eu;if(!valid_ram(r[4],4u))return -3;r[1]=rd(m,r[4],4);goto L8c052550; /* mov.l @r4,r1 */
L8c052550: *stop=0x8c052550u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c052552; /* tst r1,r1 */
L8c052552: if((*sr&1u))goto L8c052580;goto L8c052554; /* bt 0x8c052580 */
L8c052554: *stop=0x8c052554u;if(!valid_ram(r[4],4u))return -3;r[1]=rd(m,r[4],4);r[4]+=4;goto L8c052556; /* mov.l @r4+,r1 */
L8c052556: *stop=0x8c052556u;if(!valid_ram(r[1]+32u,4u))return -3;r[3]=rd(m,r[1]+0x20u,4);goto L8c052558; /* mov.l @(0x20,r1),r3 */
L8c052558: *stop=0x8c052558u;if(!valid_ram(r[1]+36u,4u))return -3;r[0]=rd(m,r[1]+0x24u,4);goto L8c05255a; /* mov.l @(0x24,r1),r0 */
L8c05255a: *stop=0x8c05255au;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[3],4);goto L8c05255c; /* mov.l r3,@r5 */
L8c05255c: *stop=0x8c05255cu;if(!valid_ram(r[5]+4u,4u))return -3;wr(m,r[5]+0x4u,r[0],4);goto L8c05255e; /* mov.l r0,@(0x4,r5) */
L8c05255e: *stop=0x8c05255eu;if(!valid_ram(r[1]+40u,4u))return -3;r[3]=rd(m,r[1]+0x28u,4);goto L8c052560; /* mov.l @(0x28,r1),r3 */
L8c052560: *stop=0x8c052560u;if(!valid_ram(r[5]+8u,4u))return -3;wr(m,r[5]+0x8u,r[3],4);goto L8c052562; /* mov.l r3,@(0x8,r5) */
L8c052562: *stop=0x8c052562u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c052564; /* mov.l @r4,r3 */
L8c052564: *stop=0x8c052564u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c052566; /* tst r3,r3 */
L8c052566: if((*sr&1u))goto L8c052596;goto L8c052568; /* bt 0x8c052596 */
L8c052568: *stop=0x8c052568u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c05256a; /* mov.l @r4,r3 */
L8c05256a: *stop=0x8c05256au;r[5]+=12u;goto L8c05256c; /* add #0xc,r5 */
L8c05256c: *stop=0x8c05256cu;if(!valid_ram(r[3]+32u,4u))return -3;r[1]=rd(m,r[3]+0x20u,4);goto L8c05256e; /* mov.l @(0x20,r3),r1 */
L8c05256e: *stop=0x8c05256eu;if(!valid_ram(r[3]+36u,4u))return -3;r[0]=rd(m,r[3]+0x24u,4);goto L8c052570; /* mov.l @(0x24,r3),r0 */
L8c052570: *stop=0x8c052570u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[1],4);goto L8c052572; /* mov.l r1,@r5 */
L8c052572: *stop=0x8c052572u;if(!valid_ram(r[5]+4u,4u))return -3;wr(m,r[5]+0x4u,r[0],4);goto L8c052574; /* mov.l r0,@(0x4,r5) */
L8c052574: *stop=0x8c052574u;if(!valid_ram(r[3]+40u,4u))return -3;r[1]=rd(m,r[3]+0x28u,4);goto L8c052576; /* mov.l @(0x28,r3),r1 */
L8c052576: *stop=0x8c052576u;if(!valid_ram(r[5]+8u,4u))return -3;wr(m,r[5]+0x8u,r[1],4);goto L8c052578; /* mov.l r1,@(0x8,r5) */
L8c052578: *stop=0x8c052578u;if(!valid_ram(r[15]+8u,4u))return -3;r[3]=rd(m,r[15]+0x8u,4);goto L8c05257a; /* mov.l @(0x8,r15),r3 */
L8c05257a: *stop=0x8c05257au;r[3]+=1u;goto L8c05257c; /* add #0x1,r3 */
L8c05257c: *stop=0x8c05257eu;if(!valid_ram(r[15]+8u,4u))return -3;wr(m,r[15]+0x8u,r[3],4);goto L8c052596; /* bra 0x8c052596 */
L8c052580: *stop=0x8c052580u;r[4]+=4u;goto L8c052582; /* add #0x4,r4 */
L8c052582: *stop=0x8c052582u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c052584; /* mov.l @r4,r3 */
L8c052584: *stop=0x8c052584u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c052586; /* tst r3,r3 */
L8c052586: if((*sr&1u))goto L8c052596;goto L8c052588; /* bt 0x8c052596 */
L8c052588: *stop=0x8c052588u;if(!valid_ram(r[4],4u))return -3;r[3]=rd(m,r[4],4);goto L8c05258a; /* mov.l @r4,r3 */
L8c05258a: *stop=0x8c05258au;if(!valid_ram(r[3]+32u,4u))return -3;r[1]=rd(m,r[3]+0x20u,4);goto L8c05258c; /* mov.l @(0x20,r3),r1 */
L8c05258c: *stop=0x8c05258cu;if(!valid_ram(r[3]+36u,4u))return -3;r[0]=rd(m,r[3]+0x24u,4);goto L8c05258e; /* mov.l @(0x24,r3),r0 */
L8c05258e: *stop=0x8c05258eu;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[1],4);goto L8c052590; /* mov.l r1,@r5 */
L8c052590: *stop=0x8c052590u;if(!valid_ram(r[5]+4u,4u))return -3;wr(m,r[5]+0x4u,r[0],4);goto L8c052592; /* mov.l r0,@(0x4,r5) */
L8c052592: *stop=0x8c052592u;if(!valid_ram(r[3]+40u,4u))return -3;r[1]=rd(m,r[3]+0x28u,4);goto L8c052594; /* mov.l @(0x28,r3),r1 */
L8c052594: *stop=0x8c052594u;if(!valid_ram(r[5]+8u,4u))return -3;wr(m,r[5]+0x8u,r[1],4);goto L8c052596; /* mov.l r1,@(0x8,r5) */
L8c052596: *stop=0x8c052596u;r[10]=rd(m,0x8c052614u,4);goto L8c052598; /* mov.l 0x8c052614,r10 */
L8c052598: *stop=0x8c052598u;r[3]=0u;goto L8c05259a; /* mov #0x0,r3 */
L8c05259a: *stop=0x8c05259au;r[13]=rd(m,0x8c052610u,4);goto L8c05259c; /* mov.l 0x8c052610,r13 */
L8c05259c: *stop=0x8c05259cu;r[2]=r[3];goto L8c05259e; /* mov r3,r2 */
L8c05259e: *stop=0x8c05259eu;if(!valid_ram(r[13],2u))return -3;wr(m,r[13],r[3],2);goto L8c0525a0; /* mov.w r3,@r13 */
L8c0525a0: *stop=0x8c0525a0u;if(!valid_ram(r[10],4u))return -3;wr(m,r[10],r[3],4);goto L8c0525a2; /* mov.l r3,@r10 */
L8c0525a2: *stop=0x8c0525a2u;r[3]=rd(m,0x8c052618u,4);goto L8c0525a4; /* mov.l 0x8c052618,r3 */
L8c0525a4: *stop=0x8c0525a4u;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[3],4);goto L8c0525a6; /* mov.l r3,@(0x4,r15) */
L8c0525a6: *stop=0x8c0525a6u;r[3]=rd(m,0x8c05261cu,4);goto L8c0525a8; /* mov.l 0x8c05261c,r3 */
L8c0525a8: target=r[3];*pr=0x8c0525acu;*stop=0x8c0525aau;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0525ac; /* jsr @r3 */
L8c0525ac: *stop=0x8c0525acu;r[3]=rd(m,0x8c052620u,4);goto L8c0525ae; /* mov.l 0x8c052620,r3 */
L8c0525ae: *stop=0x8c0525aeu;r[9]=4294967168u;goto L8c0525b0; /* mov #-0x80,r9 */
L8c0525b0: *stop=0x8c0525b0u;r[4]=rd(m,0x8c052624u,4);goto L8c0525b2; /* mov.l 0x8c052624,r4 */
L8c0525b2: *stop=0x8c0525b2u;r[8]=4u;goto L8c0525b4; /* mov #0x4,r8 */
L8c0525b4: *stop=0x8c0525b4u;if(!valid_ram(r[3],2u))return -3;r[11]=sx(m,r[3],2);goto L8c0525b6; /* mov.w @r3,r11 */
L8c0525b6: *stop=0x8c0525b6u;r[0]=0x8c052628u;goto L8c0525b8; /* mova 0x8c052628,r0 */
L8c0525b8: *stop=0x8c0525b8u;r[8]<<=8;goto L8c0525ba; /* shll8 r8 */
L8c0525ba: *stop=0x8c0525bau;r[11]+=4294967295u;goto L8c0525bc; /* add #-0x1,r11 */
L8c0525bc: *stop=0x8c0525bcu;r[2]=r[11];goto L8c0525be; /* mov r11,r2 */
L8c0525be: *stop=0x8c0525beu;*sr=(*sr&~1u)|(r[11]>>31);r[11]<<=1;goto L8c0525c0; /* shll r11 */
L8c0525c0: *stop=0x8c0525c0u;r[11]+=r[2];goto L8c0525c2; /* add r2,r11 */
L8c0525c2: *stop=0x8c0525c2u;r[11]<<=2;goto L8c0525c4; /* shll2 r11 */
L8c0525c4: *stop=0x8c0525c4u;r[11]+=r[4];goto L8c0525c6; /* add r4,r11 */
L8c0525c6: *stop=0x8c0525c6u;*sr=(*sr&~1u)|(r[11]>=r[4]);goto L8c0525c8; /* cmp/hs r4,r11 */
L8c0525c8: *stop=0x8c0525c8u;r[9]=r[9]&255u;goto L8c0525ca; /* extu.b r9,r9 */
L8c0525ca: take=(*sr&1u);*stop=0x8c0525ccu;if(!valid_ram(r[0],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[0]);if(take)goto L8c0525d2;goto L8c0525ce; /* bt/s 0x8c0525d2 */
L8c0525ce: *stop=0x8c0525d0u;goto L8c05272e; /* bra 0x8c05272e */
L8c0525d2: *stop=0x8c0525d2u;if(!valid_ram(r[11]+8u,4u))return -3;r[4]=rd(m,r[11]+0x8u,4);goto L8c0525d4; /* mov.l @(0x8,r11),r4 */
L8c0525d4: *stop=0x8c0525d4u;if(!valid_ram(r[4]+32u,4u))return -3;r[3]=rd(m,r[4]+0x20u,4);goto L8c0525d6; /* mov.l @(0x20,r4),r3 */
L8c0525d6: *stop=0x8c0525d6u;if(!valid_ram(r[3]+4u,2u))return -3;r[0]=sx(m,r[3]+0x4u,2);goto L8c0525d8; /* mov.w @(0x4,r3),r0 */
L8c0525d8: *stop=0x8c0525d8u;r[3]=1u;goto L8c0525da; /* mov #0x1,r3 */
L8c0525da: *stop=0x8c0525dau;r[3]<<=8;goto L8c0525dc; /* shll8 r3 */
L8c0525dc: *stop=0x8c0525dcu;*sr=(*sr&~1u)|!(r[3]&r[0]);goto L8c0525de; /* tst r3,r0 */
L8c0525de: take=!(*sr&1u);*stop=0x8c0525e0u;if(!valid_ram(r[11]+4u,4u))return -3;r[12]=rd(m,r[11]+0x4u,4);if(take)goto L8c0525e6;goto L8c0525e2; /* bf/s 0x8c0525e6 */
L8c0525e2: *stop=0x8c0525e4u;goto L8c052722; /* bra 0x8c052722 */
L8c0525e6: *stop=0x8c0525e6u;if(!valid_ram(r[11],4u))return -3;r[1]=rd(m,r[11],4);goto L8c0525e8; /* mov.l @r11,r1 */
L8c0525e8: *stop=0x8c0525e8u;r[2]=rd(m,0x8c05262cu,4);goto L8c0525ea; /* mov.l 0x8c05262c,r2 */
L8c0525ea: *stop=0x8c0525eau;*sr=(*sr&~1u)|!(r[2]&r[1]);goto L8c0525ec; /* tst r2,r1 */
L8c0525ec: if(!(*sr&1u))goto L8c052640;goto L8c0525ee; /* bf 0x8c052640 */
L8c0525ee: *stop=0x8c0525eeu;if(!valid_ram(r[12]+4u,4u))return -3;r[4]=rd(m,r[12]+0x4u,4);goto L8c0525f0; /* mov.l @(0x4,r12),r4 */
L8c0525f0: *stop=0x8c0525f0u;r[5]=r[12];goto L8c0525f2; /* mov r12,r5 */
L8c0525f2: *stop=0x8c0525f2u;r[5]+=8u;goto L8c0525f4; /* add #0x8,r5 */
L8c0525f4: *stop=0x8c0525f6u;r[4]+=24u;goto L8c05268c; /* bra 0x8c05268c */
L8c052640: *stop=0x8c052640u;r[1]=rd(m,0x8c052820u,4);goto L8c052642; /* mov.l 0x8c052820,r1 */
L8c052642: target=r[1];*pr=0x8c052646u;*stop=0x8c052644u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052646; /* jsr @r1 */
L8c052646: *stop=0x8c052646u;if(!valid_ram(r[12]+28u,4u))return -3;r[3]=rd(m,r[12]+0x1cu,4);goto L8c052648; /* mov.l @(0x1c,r12),r3 */
L8c052648: *stop=0x8c052648u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c05264a; /* tst r3,r3 */
L8c05264a: take=(*sr&1u);*stop=0x8c05264cu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);if(take)goto L8c052656;goto L8c05264e; /* bt/s 0x8c052656 */
L8c05264e: *stop=0x8c05264eu;r[3]=rd(m,0x8c052824u,4);goto L8c052650; /* mov.l 0x8c052824,r3 */
L8c052650: *stop=0x8c052650u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c052652; /* mov.l @r15,r5 */
L8c052652: target=r[3];*pr=0x8c052656u;*stop=0x8c052654u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052656; /* jsr @r3 */
L8c052656: *stop=0x8c052656u;if(!valid_ram(r[12]+20u,4u))return -3;r[3]=rd(m,r[12]+0x14u,4);goto L8c052658; /* mov.l @(0x14,r12),r3 */
L8c052658: *stop=0x8c052658u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c05265a; /* tst r3,r3 */
L8c05265a: take=(*sr&1u);*stop=0x8c05265cu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);if(take)goto L8c052666;goto L8c05265e; /* bt/s 0x8c052666 */
L8c05265e: *stop=0x8c05265eu;r[3]=rd(m,0x8c052828u,4);goto L8c052660; /* mov.l 0x8c052828,r3 */
L8c052660: *stop=0x8c052660u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c052662; /* mov.l @r15,r5 */
L8c052662: target=r[3];*pr=0x8c052666u;*stop=0x8c052664u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052666; /* jsr @r3 */
L8c052666: *stop=0x8c052666u;if(!valid_ram(r[12]+24u,4u))return -3;r[3]=rd(m,r[12]+0x18u,4);goto L8c052668; /* mov.l @(0x18,r12),r3 */
L8c052668: *stop=0x8c052668u;*sr=(*sr&~1u)|!(r[3]&r[3]);goto L8c05266a; /* tst r3,r3 */
L8c05266a: take=(*sr&1u);*stop=0x8c05266cu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);if(take)goto L8c052676;goto L8c05266e; /* bt/s 0x8c052676 */
L8c05266e: *stop=0x8c05266eu;r[3]=rd(m,0x8c05282cu,4);goto L8c052670; /* mov.l 0x8c05282c,r3 */
L8c052670: *stop=0x8c052670u;if(!valid_ram(r[15],4u))return -3;r[5]=rd(m,r[15],4);goto L8c052672; /* mov.l @r15,r5 */
L8c052672: target=r[3];*pr=0x8c052676u;*stop=0x8c052674u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052676; /* jsr @r3 */
L8c052676: *stop=0x8c052676u;if(!valid_ram(r[12]+4u,4u))return -3;r[5]=rd(m,r[12]+0x4u,4);goto L8c052678; /* mov.l @(0x4,r12),r5 */
L8c052678: *stop=0x8c052678u;r[6]=r[15];goto L8c05267a; /* mov r15,r6 */
L8c05267a: *stop=0x8c05267au;r[3]=rd(m,0x8c052830u,4);goto L8c05267c; /* mov.l 0x8c052830,r3 */
L8c05267c: *stop=0x8c05267cu;r[6]+=20u;goto L8c05267e; /* add #0x14,r6 */
L8c05267e: *stop=0x8c05267eu;r[5]+=24u;goto L8c052680; /* add #0x18,r5 */
L8c052680: target=r[3];*pr=0x8c052684u;*stop=0x8c052682u;r[4]=0u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052684; /* jsr @r3 */
L8c052684: *stop=0x8c052684u;r[4]=r[15];goto L8c052686; /* mov r15,r4 */
L8c052686: *stop=0x8c052686u;r[5]=r[12];goto L8c052688; /* mov r12,r5 */
L8c052688: *stop=0x8c052688u;r[4]+=20u;goto L8c05268a; /* add #0x14,r4 */
L8c05268a: *stop=0x8c05268au;r[5]+=8u;goto L8c05268c; /* add #0x8,r5 */
L8c05268c: *stop=0x8c05268cu;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,8u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c05268e; /* fmov.s @r5+,fr8 */
L8c05268e: *stop=0x8c05268eu;r[6]=r[15];goto L8c052690; /* mov r15,r6 */
L8c052690: *stop=0x8c052690u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c052692; /* fmov.s @r4+,fr3 */
L8c052692: *stop=0x8c052692u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,10u,r[5]);r[5]+=(f->fpscr&0x100000u)?8u:4u;goto L8c052694; /* fmov.s @r5+,fr10 */
L8c052694: *stop=0x8c052694u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[8],&fr[3],&f->fpscr,&fr[8],0);if(status)return status;}goto L8c052696; /* fadd fr3,fr8 */
L8c052696: *stop=0x8c052696u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c052698; /* fmov.s @r4+,fr3 */
L8c052698: *stop=0x8c052698u;if(!valid_ram(r[5],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,9u,r[5]);goto L8c05269a; /* fmov.s @r5,fr9 */
L8c05269a: *stop=0x8c05269au;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[10],&fr[3],&f->fpscr,&fr[10],0);if(status)return status;}goto L8c05269c; /* fadd fr3,fr10 */
L8c05269c: *stop=0x8c05269cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);goto L8c05269e; /* fmov.s @r4,fr3 */
L8c05269e: *stop=0x8c05269eu;r[3]=rd(m,0x8c052834u,4);goto L8c0526a0; /* mov.l 0x8c052834,r3 */
L8c0526a0: *stop=0x8c0526a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[9],&fr[3],&f->fpscr,&fr[9],0);if(status)return status;}goto L8c0526a2; /* fadd fr3,fr9 */
L8c0526a2: *stop=0x8c0526a2u;if(!valid_ram(r[15]+8u,4u))return -3;r[5]=rd(m,r[15]+0x8u,4);goto L8c0526a4; /* mov.l @(0x8,r15),r5 */
L8c0526a4: *stop=0x8c0526a4u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,7u,r[3]);goto L8c0526a6; /* fmov.s @r3,fr7 */
L8c0526a6: *stop=0x8c0526a6u;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c0526a8; /* cmp/pl r5 */
L8c0526a8: take=!(*sr&1u);*stop=0x8c0526aau;r[6]+=32u;if(take)goto L8c052722;goto L8c0526ac; /* bf/s 0x8c052722 */
L8c0526ac: *stop=0x8c0526acu;r[4]=r[6];goto L8c0526ae; /* mov r6,r4 */
L8c0526ae: *stop=0x8c0526aeu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0526b0; /* fmov.s @r4+,fr4 */
L8c0526b0: *stop=0x8c0526b0u;if(!valid_ram(r[12]+4u,4u))return -3;r[3]=rd(m,r[12]+0x4u,4);goto L8c0526b2; /* mov.l @(0x4,r12),r3 */
L8c0526b2: *stop=0x8c0526b2u;r[0]=36u;goto L8c0526b4; /* mov #0x24,r0 */
L8c0526b4: *stop=0x8c0526b4u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,5u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c0526b6; /* fmov.s @r4+,fr5 */
L8c0526b6: *stop=0x8c0526b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[8],&f->fpscr,&fr[4],1);if(status)return status;}goto L8c0526b8; /* fsub fr8,fr4 */
L8c0526b8: *stop=0x8c0526b8u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[10],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c0526ba; /* fsub fr10,fr5 */
L8c0526ba: *stop=0x8c0526bau;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,6u,r[4]);goto L8c0526bc; /* fmov.s @r4,fr6 */
L8c0526bc: *stop=0x8c0526bcu;fcopy(f,0u,4u);goto L8c0526be; /* fmov fr4,fr0 */
L8c0526be: *stop=0x8c0526beu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[6],&fr[9],&f->fpscr,&fr[6],1);if(status)return status;}goto L8c0526c0; /* fsub fr9,fr6 */
L8c0526c0: *stop=0x8c0526c0u;fcopy(f,3u,5u);goto L8c0526c2; /* fmov fr5,fr3 */
L8c0526c2: *stop=0x8c0526c2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[3],&fr[5],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0526c4; /* fmul fr5,fr3 */
L8c0526c4: *stop=0x8c0526c4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[4],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0526c6; /* fmac fr0,fr4,fr3 */
L8c0526c6: *stop=0x8c0526c6u;if(!valid_ram(r[0]+r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[3]);goto L8c0526c8; /* fmov.s @(r0,r3),fr4 */
L8c0526c8: *stop=0x8c0526c8u;fcopy(f,0u,6u);goto L8c0526ca; /* fmov fr6,fr0 */
L8c0526ca: *stop=0x8c0526cau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[7],&f->fpscr,&fr[4],0);if(status)return status;}goto L8c0526cc; /* fadd fr7,fr4 */
L8c0526cc: *stop=0x8c0526ccu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_fmac(&fr[0],&fr[6],&fr[3],&f->fpscr,&fr[3]);if(status)return status;}goto L8c0526ce; /* fmac fr0,fr6,fr3 */
L8c0526ce: *stop=0x8c0526ceu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[4],&fr[4],&f->fpscr,&fr[4]);if(status)return status;}goto L8c0526d0; /* fmul fr4,fr4 */
L8c0526d0: *stop=0x8c0526d0u;fcopy(f,5u,3u);goto L8c0526d2; /* fmov fr3,fr5 */
L8c0526d2: *stop=0x8c0526d2u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c0526d4; /* fcmp/gt fr5,fr4 */
L8c0526d4: if(!(*sr&1u))goto L8c052718;goto L8c0526d6; /* bf 0x8c052718 */
L8c0526d6: *stop=0x8c0526d6u;if(!valid_ram(r[11],4u))return -3;r[3]=rd(m,r[11],4);goto L8c0526d8; /* mov.l @r11,r3 */
L8c0526d8: *stop=0x8c0526d8u;if(!valid_ram(r[15]+4u,4u))return -3;r[2]=rd(m,r[15]+0x4u,4);goto L8c0526da; /* mov.l @(0x4,r15),r2 */
L8c0526da: *stop=0x8c0526dau;if(!valid_ram(r[11]+4u,4u))return -3;r[1]=rd(m,r[11]+0x4u,4);goto L8c0526dc; /* mov.l @(0x4,r11),r1 */
L8c0526dc: *stop=0x8c0526dcu;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[3],4);goto L8c0526de; /* mov.l r3,@r2 */
L8c0526de: *stop=0x8c0526deu;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[1],4);goto L8c0526e0; /* mov.l r1,@(0x4,r2) */
L8c0526e0: *stop=0x8c0526e0u;if(!valid_ram(r[11]+8u,4u))return -3;r[3]=rd(m,r[11]+0x8u,4);goto L8c0526e2; /* mov.l @(0x8,r11),r3 */
L8c0526e2: *stop=0x8c0526e2u;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[3],4);goto L8c0526e4; /* mov.l r3,@(0x8,r2) */
L8c0526e4: *stop=0x8c0526e4u;if(!valid_ram(r[15]+4u,4u))return -3;r[0]=rd(m,r[15]+0x4u,4);goto L8c0526e6; /* mov.l @(0x4,r15),r0 */
L8c0526e6: *stop=0x8c0526e6u;r[0]+=12u;goto L8c0526e8; /* add #0xc,r0 */
L8c0526e8: *stop=0x8c0526e8u;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[0],4);goto L8c0526ea; /* mov.l r0,@(0x4,r15) */
L8c0526ea: *stop=0x8c0526eau;if(!valid_ram(r[10],4u))return -3;r[3]=rd(m,r[10],4);goto L8c0526ec; /* mov.l @r10,r3 */
L8c0526ec: *stop=0x8c0526ecu;r[3]+=1u;goto L8c0526ee; /* add #0x1,r3 */
L8c0526ee: *stop=0x8c0526eeu;if(!valid_ram(r[10],4u))return -3;wr(m,r[10],r[3],4);goto L8c0526f0; /* mov.l r3,@r10 */
L8c0526f0: *stop=0x8c0526f0u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[12],4);goto L8c0526f2; /* mov.l r12,@(0x4,r14) */
L8c0526f2: *stop=0x8c0526f2u;if(!valid_ram(r[11]+8u,4u))return -3;r[2]=rd(m,r[11]+0x8u,4);goto L8c0526f4; /* mov.l @(0x8,r11),r2 */
L8c0526f4: *stop=0x8c0526f4u;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[2],4);goto L8c0526f6; /* mov.l r2,@(0x8,r14) */
L8c0526f6: *stop=0x8c0526f6u;if(!valid_ram(r[11],4u))return -3;r[3]=rd(m,r[11],4);goto L8c0526f8; /* mov.l @r11,r3 */
L8c0526f8: *stop=0x8c0526f8u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[3],4);goto L8c0526fa; /* mov.l r3,@r14 */
L8c0526fa: *stop=0x8c0526fau;if(!valid_ram(r[13],2u))return -3;r[2]=sx(m,r[13],2);goto L8c0526fc; /* mov.w @r13,r2 */
L8c0526fc: *stop=0x8c0526fcu;r[2]+=1u;goto L8c0526fe; /* add #0x1,r2 */
L8c0526fe: *stop=0x8c0526feu;if(!valid_ram(r[13],2u))return -3;wr(m,r[13],r[2],2);goto L8c052700; /* mov.w r2,@r13 */
L8c052700: *stop=0x8c052700u;if(!valid_ram(r[10],4u))return -3;r[3]=rd(m,r[10],4);goto L8c052702; /* mov.l @r10,r3 */
L8c052702: *stop=0x8c052702u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[9]);goto L8c052704; /* cmp/ge r9,r3 */
L8c052704: take=(*sr&1u);*stop=0x8c052706u;r[14]+=12u;if(take)goto L8c05272e;goto L8c052708; /* bt/s 0x8c05272e */
L8c052708: *stop=0x8c052708u;if(!valid_ram(r[13],2u))return -3;r[2]=sx(m,r[13],2);goto L8c05270a; /* mov.w @r13,r2 */
L8c05270a: *stop=0x8c05270au;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[8]);goto L8c05270c; /* cmp/ge r8,r2 */
L8c05270c: if(!(*sr&1u))goto L8c052722;goto L8c05270e; /* bf 0x8c052722 */
L8c05270e: *stop=0x8c05270eu;r[2]=rd(m,0x8c052838u,4);goto L8c052710; /* mov.l 0x8c052838,r2 */
L8c052710: target=r[2];*pr=0x8c052714u;*stop=0x8c052712u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052714; /* jsr @r2 */
L8c052714: *stop=0x8c052716u;goto L8c052882; /* bra 0x8c052882 */
L8c052718: *stop=0x8c052718u;r[5]+=4294967295u;goto L8c05271a; /* add #-0x1,r5 */
L8c05271a: *stop=0x8c05271au;fcopy(f,7u,12u);goto L8c05271c; /* fmov fr12,fr7 */
L8c05271c: *stop=0x8c05271cu;*sr=(*sr&~1u)|((int32_t)r[5]>0);goto L8c05271e; /* cmp/pl r5 */
L8c05271e: take=(*sr&1u);*stop=0x8c052720u;r[6]+=12u;if(take)goto L8c0526ac;goto L8c052722; /* bt/s 0x8c0526ac */
L8c052722: *stop=0x8c052722u;r[3]=rd(m,0x8c05283cu,4);goto L8c052724; /* mov.l 0x8c05283c,r3 */
L8c052724: *stop=0x8c052724u;r[11]+=4294967284u;goto L8c052726; /* add #-0xc,r11 */
L8c052726: *stop=0x8c052726u;*sr=(*sr&~1u)|(r[11]>=r[3]);goto L8c052728; /* cmp/hs r3,r11 */
L8c052728: if(!(*sr&1u))goto L8c05272e;goto L8c05272a; /* bf 0x8c05272e */
L8c05272a: *stop=0x8c05272cu;goto L8c0525d2; /* bra 0x8c0525d2 */
L8c05272e: *stop=0x8c05272eu;r[3]=rd(m,0x8c052838u,4);goto L8c052730; /* mov.l 0x8c052838,r3 */
L8c052730: target=r[3];*pr=0x8c052734u;*stop=0x8c052732u;r[4]=1u;if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c052734; /* jsr @r3 */
L8c052734: *stop=0x8c052734u;r[11]=rd(m,0x8c052840u,4);goto L8c052736; /* mov.l 0x8c052840,r11 */
L8c052736: *stop=0x8c052736u;r[2]=0u;goto L8c052738; /* mov #0x0,r2 */
L8c052738: *stop=0x8c052738u;r[3]=rd(m,0x8c052844u,4);goto L8c05273a; /* mov.l 0x8c052844,r3 */
L8c05273a: *stop=0x8c05273au;if(!valid_ram(r[11],4u))return -3;wr(m,r[11],r[2],4);goto L8c05273c; /* mov.l r2,@r11 */
L8c05273c: *stop=0x8c05273cu;r[2]=rd(m,0x8c052848u,4);goto L8c05273e; /* mov.l 0x8c052848,r2 */
L8c05273e: *stop=0x8c05273eu;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[3],4);goto L8c052740; /* mov.l r3,@r15 */
L8c052740: *stop=0x8c052740u;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);goto L8c052742; /* mov.l @r2,r0 */
L8c052742: *stop=0x8c052742u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c052744; /* cmp/eq #0x1,r0 */
L8c052744: if((*sr&1u))goto L8c05274a;goto L8c052746; /* bt 0x8c05274a */
L8c052746: *stop=0x8c052748u;goto L8c052882; /* bra 0x8c052882 */
L8c05274a: *stop=0x8c05274au;r[3]=rd(m,0x8c05284cu,4);goto L8c05274c; /* mov.l 0x8c05284c,r3 */
L8c05274c: *stop=0x8c05274cu;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c05274e; /* mov.l @r3,r1 */
L8c05274e: *stop=0x8c05274eu;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c052750; /* tst r1,r1 */
L8c052750: if(!(*sr&1u))goto L8c052756;goto L8c052752; /* bf 0x8c052756 */
L8c052752: *stop=0x8c052754u;goto L8c052882; /* bra 0x8c052882 */
L8c052756: *stop=0x8c052756u;r[1]=rd(m,0x8c05284cu,4);goto L8c052758; /* mov.l 0x8c05284c,r1 */
L8c052758: *stop=0x8c052758u;if(!valid_ram(r[1],4u))return -3;r[4]=rd(m,r[1],4);goto L8c05275a; /* mov.l @r1,r4 */
L8c05275a: *stop=0x8c05275au;if(!valid_ram(r[4],2u))return -3;r[0]=sx(m,r[4],2);goto L8c05275c; /* mov.w @r4,r0 */
L8c05275c: *stop=0x8c05275cu;if(!valid_ram(r[4]+12u,4u))return -3;r[12]=rd(m,r[4]+0xcu,4);goto L8c05275e; /* mov.l @(0xc,r4),r12 */
L8c05275e: *stop=0x8c052760u;if(!valid_ram(r[15]+12u,2u))return -3;wr(m,r[15]+0xcu,r[0],2);goto L8c052878; /* bra 0x8c052878 */
L8c052762: *stop=0x8c052762u;if(!valid_ram(r[12]+32u,4u))return -3;r[1]=rd(m,r[12]+0x20u,4);goto L8c052764; /* mov.l @(0x20,r12),r1 */
L8c052764: *stop=0x8c052764u;r[3]=rd(m,0x8c052850u,4);goto L8c052766; /* mov.l 0x8c052850,r3 */
L8c052766: *stop=0x8c052766u;*sr=(*sr&~1u)|!(r[3]&r[1]);goto L8c052768; /* tst r3,r1 */
L8c052768: if(!(*sr&1u))goto L8c05276e;goto L8c05276a; /* bf 0x8c05276e */
L8c05276a: *stop=0x8c05276cu;goto L8c052870; /* bra 0x8c052870 */
L8c05276e: *stop=0x8c05276eu;r[3]=rd(m,0x8c052834u,4);goto L8c052770; /* mov.l 0x8c052834,r3 */
L8c052770: *stop=0x8c052770u;r[2]=r[15];goto L8c052772; /* mov r15,r2 */
L8c052772: *stop=0x8c052772u;r[2]+=32u;goto L8c052774; /* add #0x20,r2 */
L8c052774: *stop=0x8c052774u;if(!valid_ram(r[3],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[3]);goto L8c052776; /* fmov.s @r3,fr3 */
L8c052776: *stop=0x8c052776u;r[0]=16u;goto L8c052778; /* mov #0x10,r0 */
L8c052778: *stop=0x8c052778u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,3u,r[0]+r[15]);goto L8c05277a; /* fmov fr3,@(r0,r15) */
L8c05277a: *stop=0x8c05277au;if(!valid_ram(r[15]+8u,4u))return -3;r[10]=rd(m,r[15]+0x8u,4);goto L8c05277c; /* mov.l @(0x8,r15),r10 */
L8c05277c: *stop=0x8c05277cu;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[2],4);goto L8c05277e; /* mov.l r2,@(0x4,r15) */
L8c05277e: *stop=0x8c05277eu;*sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8c052780; /* cmp/pl r10 */
L8c052780: if((*sr&1u))goto L8c052786;goto L8c052782; /* bt 0x8c052786 */
L8c052782: *stop=0x8c052784u;goto L8c052870; /* bra 0x8c052870 */
L8c052786: *stop=0x8c052786u;r[4]=r[12];goto L8c052788; /* mov r12,r4 */
L8c052788: *stop=0x8c052788u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c05278a; /* fmov.s @r4+,fr14 */
L8c05278a: *stop=0x8c05278au;r[3]=rd(m,0x8c052854u,4);goto L8c05278c; /* mov.l 0x8c052854,r3 */
L8c05278c: *stop=0x8c05278cu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c05278e; /* fmov.s @r4+,fr13 */
L8c05278e: *stop=0x8c05278eu;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[4]);goto L8c052790; /* fmov.s @r4,fr15 */
L8c052790: *stop=0x8c052790u;if(!valid_ram(r[15]+4u,4u))return -3;r[4]=rd(m,r[15]+0x4u,4);goto L8c052792; /* mov.l @(0x4,r15),r4 */
L8c052792: *stop=0x8c052792u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c052794; /* fmov.s @r4+,fr3 */
L8c052794: *stop=0x8c052794u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[3],&f->fpscr,&fr[14],1);if(status)return status;}goto L8c052796; /* fsub fr3,fr14 */
L8c052796: *stop=0x8c052796u;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);r[4]+=(f->fpscr&0x100000u)?8u:4u;goto L8c052798; /* fmov.s @r4+,fr3 */
L8c052798: *stop=0x8c052798u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[13],&fr[3],&f->fpscr,&fr[13],1);if(status)return status;}goto L8c05279a; /* fsub fr3,fr13 */
L8c05279a: *stop=0x8c05279au;if(!valid_ram(r[4],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[4]);goto L8c05279c; /* fmov.s @r4,fr3 */
L8c05279c: *stop=0x8c05279cu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[14],&fr[14],&f->fpscr,&fr[14]);if(status)return status;}goto L8c05279e; /* fmul fr14,fr14 */
L8c05279e: *stop=0x8c05279eu;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[15],&fr[3],&f->fpscr,&fr[15],1);if(status)return status;}goto L8c0527a0; /* fsub fr3,fr15 */
L8c0527a0: *stop=0x8c0527a0u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[13],&fr[13],&f->fpscr,&fr[13]);if(status)return status;}goto L8c0527a2; /* fmul fr13,fr13 */
L8c0527a2: *stop=0x8c0527a2u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_multiply_extended(&fr[15],&fr[15],&f->fpscr,&fr[15]);if(status)return status;}goto L8c0527a4; /* fmul fr15,fr15 */
L8c0527a4: *stop=0x8c0527a4u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[14],&fr[13],&f->fpscr,&fr[14],0);if(status)return status;}goto L8c0527a6; /* fadd fr13,fr14 */
L8c0527a6: *stop=0x8c0527a6u;fcopy(f,4u,14u);goto L8c0527a8; /* fmov fr14,fr4 */
L8c0527a8: target=r[3];*pr=0x8c0527acu;*stop=0x8c0527aau;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[4],&fr[15],&f->fpscr,&fr[4],0);if(status)return status;}if(!call)return -2;status=call(ctx,target,m,f,sr,r,pr);if(status)return status;goto L8c0527ac; /* jsr @r3 */
L8c0527ac: *stop=0x8c0527acu;r[0]=16u;goto L8c0527ae; /* mov #0x10,r0 */
L8c0527ae: *stop=0x8c0527aeu;fcopy(f,5u,0u);goto L8c0527b0; /* fmov fr0,fr5 */
L8c0527b0: *stop=0x8c0527b0u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,3u,r[0]+r[15]);goto L8c0527b2; /* fmov.s @(r0,r15),fr3 */
L8c0527b2: *stop=0x8c0527b2u;r[0]=12u;goto L8c0527b4; /* mov #0xc,r0 */
L8c0527b4: *stop=0x8c0527b4u;if(!valid_ram(r[0]+r[12],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,4u,r[0]+r[12]);goto L8c0527b6; /* fmov.s @(r0,r12),fr4 */
L8c0527b6: *stop=0x8c0527b6u;{uint32_t *fr=f->bank[(f->fpscr>>21)&1u];status=sa_sh4_add_sub(&fr[5],&fr[3],&f->fpscr,&fr[5],1);if(status)return status;}goto L8c0527b8; /* fsub fr3,fr5 */
L8c0527b8: *stop=0x8c0527b8u;status=sa_sh4_compare_bits(f->bank[(f->fpscr>>21)&1u][4],f->bank[(f->fpscr>>21)&1u][5],&f->fpscr,sr,1);if(status)return status;goto L8c0527ba; /* fcmp/gt fr5,fr4 */
L8c0527ba: if(!(*sr&1u))goto L8c052860;goto L8c0527bc; /* bf 0x8c052860 */
L8c0527bc: *stop=0x8c0527bcu;if(!valid_ram(r[12],4u))return -3;r[3]=rd(m,r[12],4);goto L8c0527be; /* mov.l @r12,r3 */
L8c0527be: *stop=0x8c0527beu;if(!valid_ram(r[15],4u))return -3;r[2]=rd(m,r[15],4);goto L8c0527c0; /* mov.l @r15,r2 */
L8c0527c0: *stop=0x8c0527c0u;if(!valid_ram(r[12]+4u,4u))return -3;r[1]=rd(m,r[12]+0x4u,4);goto L8c0527c2; /* mov.l @(0x4,r12),r1 */
L8c0527c2: *stop=0x8c0527c2u;if(!valid_ram(r[2],4u))return -3;wr(m,r[2],r[3],4);goto L8c0527c4; /* mov.l r3,@r2 */
L8c0527c4: *stop=0x8c0527c4u;if(!valid_ram(r[2]+4u,4u))return -3;wr(m,r[2]+0x4u,r[1],4);goto L8c0527c6; /* mov.l r1,@(0x4,r2) */
L8c0527c6: *stop=0x8c0527c6u;if(!valid_ram(r[12]+8u,4u))return -3;r[3]=rd(m,r[12]+0x8u,4);goto L8c0527c8; /* mov.l @(0x8,r12),r3 */
L8c0527c8: *stop=0x8c0527c8u;if(!valid_ram(r[12]+12u,4u))return -3;r[1]=rd(m,r[12]+0xcu,4);goto L8c0527ca; /* mov.l @(0xc,r12),r1 */
L8c0527ca: *stop=0x8c0527cau;if(!valid_ram(r[2]+8u,4u))return -3;wr(m,r[2]+0x8u,r[3],4);goto L8c0527cc; /* mov.l r3,@(0x8,r2) */
L8c0527cc: *stop=0x8c0527ccu;if(!valid_ram(r[2]+12u,4u))return -3;wr(m,r[2]+0xcu,r[1],4);goto L8c0527ce; /* mov.l r1,@(0xc,r2) */
L8c0527ce: *stop=0x8c0527ceu;if(!valid_ram(r[12]+16u,4u))return -3;r[3]=rd(m,r[12]+0x10u,4);goto L8c0527d0; /* mov.l @(0x10,r12),r3 */
L8c0527d0: *stop=0x8c0527d0u;if(!valid_ram(r[12]+20u,4u))return -3;r[1]=rd(m,r[12]+0x14u,4);goto L8c0527d2; /* mov.l @(0x14,r12),r1 */
L8c0527d2: *stop=0x8c0527d2u;if(!valid_ram(r[2]+16u,4u))return -3;wr(m,r[2]+0x10u,r[3],4);goto L8c0527d4; /* mov.l r3,@(0x10,r2) */
L8c0527d4: *stop=0x8c0527d4u;if(!valid_ram(r[2]+20u,4u))return -3;wr(m,r[2]+0x14u,r[1],4);goto L8c0527d6; /* mov.l r1,@(0x14,r2) */
L8c0527d6: *stop=0x8c0527d6u;if(!valid_ram(r[12]+24u,4u))return -3;r[3]=rd(m,r[12]+0x18u,4);goto L8c0527d8; /* mov.l @(0x18,r12),r3 */
L8c0527d8: *stop=0x8c0527d8u;if(!valid_ram(r[12]+28u,4u))return -3;r[1]=rd(m,r[12]+0x1cu,4);goto L8c0527da; /* mov.l @(0x1c,r12),r1 */
L8c0527da: *stop=0x8c0527dau;if(!valid_ram(r[2]+24u,4u))return -3;wr(m,r[2]+0x18u,r[3],4);goto L8c0527dc; /* mov.l r3,@(0x18,r2) */
L8c0527dc: *stop=0x8c0527dcu;if(!valid_ram(r[2]+28u,4u))return -3;wr(m,r[2]+0x1cu,r[1],4);goto L8c0527de; /* mov.l r1,@(0x1c,r2) */
L8c0527de: *stop=0x8c0527deu;if(!valid_ram(r[12]+32u,4u))return -3;r[3]=rd(m,r[12]+0x20u,4);goto L8c0527e0; /* mov.l @(0x20,r12),r3 */
L8c0527e0: *stop=0x8c0527e0u;if(!valid_ram(r[2]+32u,4u))return -3;wr(m,r[2]+0x20u,r[3],4);goto L8c0527e2; /* mov.l r3,@(0x20,r2) */
L8c0527e2: *stop=0x8c0527e2u;if(!valid_ram(r[15],4u))return -3;r[0]=rd(m,r[15],4);goto L8c0527e4; /* mov.l @r15,r0 */
L8c0527e4: *stop=0x8c0527e4u;r[0]+=36u;goto L8c0527e6; /* add #0x24,r0 */
L8c0527e6: *stop=0x8c0527e6u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[0],4);goto L8c0527e8; /* mov.l r0,@r15 */
L8c0527e8: *stop=0x8c0527e8u;if(!valid_ram(r[11],4u))return -3;r[3]=rd(m,r[11],4);goto L8c0527ea; /* mov.l @r11,r3 */
L8c0527ea: *stop=0x8c0527eau;r[3]+=1u;goto L8c0527ec; /* add #0x1,r3 */
L8c0527ec: *stop=0x8c0527ecu;if(!valid_ram(r[11],4u))return -3;wr(m,r[11],r[3],4);goto L8c0527ee; /* mov.l r3,@r11 */
L8c0527ee: *stop=0x8c0527eeu;r[3]=0u;goto L8c0527f0; /* mov #0x0,r3 */
L8c0527f0: *stop=0x8c0527f0u;if(!valid_ram(r[12]+32u,4u))return -3;r[2]=rd(m,r[12]+0x20u,4);goto L8c0527f2; /* mov.l @(0x20,r12),r2 */
L8c0527f2: *stop=0x8c0527f2u;if(!valid_ram(r[14],4u))return -3;wr(m,r[14],r[2],4);goto L8c0527f4; /* mov.l r2,@r14 */
L8c0527f4: *stop=0x8c0527f4u;if(!valid_ram(r[14]+8u,4u))return -3;wr(m,r[14]+0x8u,r[3],4);goto L8c0527f6; /* mov.l r3,@(0x8,r14) */
L8c0527f6: *stop=0x8c0527f6u;if(!valid_ram(r[12]+24u,4u))return -3;r[2]=rd(m,r[12]+0x18u,4);goto L8c0527f8; /* mov.l @(0x18,r12),r2 */
L8c0527f8: *stop=0x8c0527f8u;if(!valid_ram(r[14]+4u,4u))return -3;wr(m,r[14]+0x4u,r[2],4);goto L8c0527fa; /* mov.l r2,@(0x4,r14) */
L8c0527fa: *stop=0x8c0527fau;if(!valid_ram(r[13],2u))return -3;r[3]=sx(m,r[13],2);goto L8c0527fc; /* mov.w @r13,r3 */
L8c0527fc: *stop=0x8c0527fcu;r[3]+=1u;goto L8c0527fe; /* add #0x1,r3 */
L8c0527fe: *stop=0x8c0527feu;if(!valid_ram(r[13],2u))return -3;wr(m,r[13],r[3],2);goto L8c052800; /* mov.w r3,@r13 */
L8c052800: *stop=0x8c052800u;if(!valid_ram(r[11],4u))return -3;r[2]=rd(m,r[11],4);goto L8c052802; /* mov.l @r11,r2 */
L8c052802: *stop=0x8c052802u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[9]);goto L8c052804; /* cmp/ge r9,r2 */
L8c052804: take=(*sr&1u);*stop=0x8c052806u;r[14]+=12u;if(take)goto L8c052882;goto L8c052808; /* bt/s 0x8c052882 */
L8c052808: *stop=0x8c052808u;if(!valid_ram(r[13],2u))return -3;r[1]=sx(m,r[13],2);goto L8c05280a; /* mov.w @r13,r1 */
L8c05280a: *stop=0x8c05280au;*sr=(*sr&~1u)|((int32_t)r[1]>=(int32_t)r[8]);goto L8c05280c; /* cmp/ge r8,r1 */
L8c05280c: if((*sr&1u))goto L8c052882;goto L8c05280e; /* bt 0x8c052882 */
L8c05280e: *stop=0x8c052810u;goto L8c052870; /* bra 0x8c052870 */
L8c052860: *stop=0x8c052860u;r[10]+=4294967295u;goto L8c052862; /* add #-0x1,r10 */
L8c052862: *stop=0x8c052862u;r[0]=16u;goto L8c052864; /* mov #0x10,r0 */
L8c052864: *stop=0x8c052864u;*sr=(*sr&~1u)|((int32_t)r[10]>0);goto L8c052866; /* cmp/pl r10 */
L8c052866: *stop=0x8c052866u;if(!valid_ram(r[0]+r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fstore(m,f,12u,r[0]+r[15]);goto L8c052868; /* fmov fr12,@(r0,r15) */
L8c052868: *stop=0x8c052868u;if(!valid_ram(r[15]+4u,4u))return -3;r[3]=rd(m,r[15]+0x4u,4);goto L8c05286a; /* mov.l @(0x4,r15),r3 */
L8c05286a: *stop=0x8c05286au;r[3]+=12u;goto L8c05286c; /* add #0xc,r3 */
L8c05286c: take=(*sr&1u);*stop=0x8c05286eu;if(!valid_ram(r[15]+4u,4u))return -3;wr(m,r[15]+0x4u,r[3],4);if(take)goto L8c052786;goto L8c052870; /* bt/s 0x8c052786 */
L8c052870: *stop=0x8c052870u;if(!valid_ram(r[15]+12u,2u))return -3;r[0]=sx(m,r[15]+0xcu,2);goto L8c052872; /* mov.w @(0xc,r15),r0 */
L8c052872: *stop=0x8c052872u;r[12]+=36u;goto L8c052874; /* add #0x24,r12 */
L8c052874: *stop=0x8c052874u;r[0]+=4294967295u;goto L8c052876; /* add #-0x1,r0 */
L8c052876: *stop=0x8c052876u;if(!valid_ram(r[15]+12u,2u))return -3;wr(m,r[15]+0xcu,r[0],2);goto L8c052878; /* mov.w r0,@(0xc,r15) */
L8c052878: *stop=0x8c052878u;if(!valid_ram(r[15]+12u,2u))return -3;r[0]=sx(m,r[15]+0xcu,2);goto L8c05287a; /* mov.w @(0xc,r15),r0 */
L8c05287a: *stop=0x8c05287au;*sr=(*sr&~1u)|((int32_t)r[0]>0);goto L8c05287c; /* cmp/pl r0 */
L8c05287c: if(!(*sr&1u))goto L8c052882;goto L8c05287e; /* bf 0x8c052882 */
L8c05287e: *stop=0x8c052880u;goto L8c052762; /* bra 0x8c052762 */
L8c052882: *stop=0x8c052882u;r[15]+=56u;goto L8c052884; /* add #0x38,r15 */
L8c052884: *stop=0x8c052884u;if(!valid_ram(r[15],4u))return -3;*pr=rd(m,r[15],4);r[15]+=4;goto L8c052886; /* lds.l @r15+,PR */
L8c052886: *stop=0x8c052886u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,12u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c052888; /* fmov.s @r15+,fr12 */
L8c052888: *stop=0x8c052888u;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,13u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c05288a; /* fmov.s @r15+,fr13 */
L8c05288a: *stop=0x8c05288au;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,14u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c05288c; /* fmov.s @r15+,fr14 */
L8c05288c: *stop=0x8c05288cu;if(!valid_ram(r[15],((f->fpscr&0x100000u)?8u:4u)))return -3;fload(m,f,15u,r[15]);r[15]+=(f->fpscr&0x100000u)?8u:4u;goto L8c05288e; /* fmov.s @r15+,fr15 */
L8c05288e: *stop=0x8c05288eu;if(!valid_ram(r[15],4u))return -3;r[8]=rd(m,r[15],4);r[15]+=4;goto L8c052890; /* mov.l @r15+,r8 */
L8c052890: *stop=0x8c052890u;if(!valid_ram(r[15],4u))return -3;r[9]=rd(m,r[15],4);r[15]+=4;goto L8c052892; /* mov.l @r15+,r9 */
L8c052892: *stop=0x8c052892u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c052894; /* mov.l @r15+,r10 */
L8c052894: *stop=0x8c052894u;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c052896; /* mov.l @r15+,r11 */
L8c052896: *stop=0x8c052896u;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c052898; /* mov.l @r15+,r12 */
L8c052898: *stop=0x8c052898u;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c05289a; /* mov.l @r15+,r13 */
L8c05289a: *stop=0x8c05289au;if(*pr!=entry_pr)return -3;*stop=0x8c05289cu;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}

SA_STAGE_EXPORT int sa_world_collect_all_8c052e30(uint8_t *m,struct sa_fpu_banks *f,
 uint32_t *sr,uint32_t *r,uint32_t *pr,uint32_t *fpul,uint32_t *macl,
 void *ctx,sa_shared_service call,uint32_t *stop){
 uint32_t entry_pr=*pr;
 (void)entry_pr;(void)sx;(void)sr;(void)fload;(void)fstore;
 (void)fpul;(void)macl;(void)f;(void)pr;(void)ctx;(void)call;(void)fcopy;(void)sa_sh4_compare_bits;
 int status;uint32_t target;(void)sizeof(status);(void)sizeof(target);
 int take;
 goto L8c052e30;
L8c052e30: *stop=0x8c052e30u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[14],4);goto L8c052e32; /* mov.l r14,@-r15 */
L8c052e32: *stop=0x8c052e32u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[13],4);goto L8c052e34; /* mov.l r13,@-r15 */
L8c052e34: *stop=0x8c052e34u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[12],4);goto L8c052e36; /* mov.l r12,@-r15 */
L8c052e36: *stop=0x8c052e36u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[11],4);goto L8c052e38; /* mov.l r11,@-r15 */
L8c052e38: *stop=0x8c052e38u;if(!valid_ram(r[15]-4u,4u))return -3;r[15]-=4;wr(m,r[15],r[10],4);goto L8c052e3a; /* mov.l r10,@-r15 */
L8c052e3a: *stop=0x8c052e3au;r[15]+=4294967292u;goto L8c052e3c; /* add #-0x4,r15 */
L8c052e3c: *stop=0x8c052e3cu;r[3]=rd(m,0x8c052f74u,4);goto L8c052e3e; /* mov.l 0x8c052f74,r3 */
L8c052e3e: *stop=0x8c052e3eu;r[13]=4u;goto L8c052e40; /* mov #0x4,r13 */
L8c052e40: *stop=0x8c052e40u;r[10]=rd(m,0x8c052f70u,4);goto L8c052e42; /* mov.l 0x8c052f70,r10 */
L8c052e42: *stop=0x8c052e42u;r[14]=4294967168u;goto L8c052e44; /* mov #-0x80,r14 */
L8c052e44: *stop=0x8c052e44u;r[7]=rd(m,0x8c052f68u,4);goto L8c052e46; /* mov.l 0x8c052f68,r7 */
L8c052e46: *stop=0x8c052e46u;r[12]=0u;goto L8c052e48; /* mov #0x0,r12 */
L8c052e48: *stop=0x8c052e48u;r[4]=rd(m,0x8c052f64u,4);goto L8c052e4a; /* mov.l 0x8c052f64,r4 */
L8c052e4a: *stop=0x8c052e4au;r[14]=r[14]&255u;goto L8c052e4c; /* extu.b r14,r14 */
L8c052e4c: *stop=0x8c052e4cu;r[11]=rd(m,0x8c052f6cu,4);goto L8c052e4e; /* mov.l 0x8c052f6c,r11 */
L8c052e4e: *stop=0x8c052e4eu;r[13]<<=8;goto L8c052e50; /* shll8 r13 */
L8c052e50: *stop=0x8c052e50u;r[5]=rd(m,0x8c052f60u,4);goto L8c052e52; /* mov.l 0x8c052f60,r5 */
L8c052e52: *stop=0x8c052e52u;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[12],2);goto L8c052e54; /* mov.w r12,@r4 */
L8c052e54: *stop=0x8c052e54u;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[12],4);goto L8c052e56; /* mov.l r12,@r7 */
L8c052e56: *stop=0x8c052e56u;if(!valid_ram(r[3],2u))return -3;r[6]=sx(m,r[3],2);goto L8c052e58; /* mov.w @r3,r6 */
L8c052e58: *stop=0x8c052e58u;r[6]+=4294967295u;goto L8c052e5a; /* add #-0x1,r6 */
L8c052e5a: *stop=0x8c052e5au;r[2]=r[6];goto L8c052e5c; /* mov r6,r2 */
L8c052e5c: *stop=0x8c052e5cu;*sr=(*sr&~1u)|(r[6]>>31);r[6]<<=1;goto L8c052e5e; /* shll r6 */
L8c052e5e: *stop=0x8c052e5eu;r[6]+=r[2];goto L8c052e60; /* add r2,r6 */
L8c052e60: *stop=0x8c052e60u;r[2]=r[10];goto L8c052e62; /* mov r10,r2 */
L8c052e62: *stop=0x8c052e62u;r[6]<<=2;goto L8c052e64; /* shll2 r6 */
L8c052e64: *stop=0x8c052e64u;r[6]+=r[10];goto L8c052e66; /* add r10,r6 */
L8c052e66: *stop=0x8c052e66u;*sr=(*sr&~1u)|(r[6]>=r[2]);goto L8c052e68; /* cmp/hs r2,r6 */
L8c052e68: take=!(*sr&1u);*stop=0x8c052e6au;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[10],4);if(take)goto L8c052ea6;goto L8c052e6c; /* bf/s 0x8c052ea6 */
L8c052e6c: *stop=0x8c052e6cu;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c052e6e; /* mov.l @r6,r3 */
L8c052e6e: *stop=0x8c052e6eu;if(!valid_ram(r[6]+4u,4u))return -3;r[0]=rd(m,r[6]+0x4u,4);goto L8c052e70; /* mov.l @(0x4,r6),r0 */
L8c052e70: *stop=0x8c052e70u;if(!valid_ram(r[11],4u))return -3;wr(m,r[11],r[3],4);goto L8c052e72; /* mov.l r3,@r11 */
L8c052e72: *stop=0x8c052e72u;r[2]=r[0];goto L8c052e74; /* mov r0,r2 */
L8c052e74: *stop=0x8c052e74u;if(!valid_ram(r[11]+4u,4u))return -3;wr(m,r[11]+0x4u,r[2],4);goto L8c052e76; /* mov.l r2,@(0x4,r11) */
L8c052e76: *stop=0x8c052e76u;if(!valid_ram(r[6]+8u,4u))return -3;r[3]=rd(m,r[6]+0x8u,4);goto L8c052e78; /* mov.l @(0x8,r6),r3 */
L8c052e78: *stop=0x8c052e78u;if(!valid_ram(r[11]+8u,4u))return -3;wr(m,r[11]+0x8u,r[3],4);goto L8c052e7a; /* mov.l r3,@(0x8,r11) */
L8c052e7a: *stop=0x8c052e7au;r[11]+=12u;goto L8c052e7c; /* add #0xc,r11 */
L8c052e7c: *stop=0x8c052e7cu;if(!valid_ram(r[7],4u))return -3;r[3]=rd(m,r[7],4);goto L8c052e7e; /* mov.l @r7,r3 */
L8c052e7e: *stop=0x8c052e7eu;r[3]+=1u;goto L8c052e80; /* add #0x1,r3 */
L8c052e80: *stop=0x8c052e80u;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[3],4);goto L8c052e82; /* mov.l r3,@r7 */
L8c052e82: *stop=0x8c052e82u;if(!valid_ram(r[5]+4u,4u))return -3;wr(m,r[5]+0x4u,r[2],4);goto L8c052e84; /* mov.l r2,@(0x4,r5) */
L8c052e84: *stop=0x8c052e84u;if(!valid_ram(r[6]+8u,4u))return -3;r[2]=rd(m,r[6]+0x8u,4);goto L8c052e86; /* mov.l @(0x8,r6),r2 */
L8c052e86: *stop=0x8c052e86u;if(!valid_ram(r[5]+8u,4u))return -3;wr(m,r[5]+0x8u,r[2],4);goto L8c052e88; /* mov.l r2,@(0x8,r5) */
L8c052e88: *stop=0x8c052e88u;if(!valid_ram(r[6],4u))return -3;r[3]=rd(m,r[6],4);goto L8c052e8a; /* mov.l @r6,r3 */
L8c052e8a: *stop=0x8c052e8au;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[3],4);goto L8c052e8c; /* mov.l r3,@r5 */
L8c052e8c: *stop=0x8c052e8cu;if(!valid_ram(r[4],2u))return -3;r[2]=sx(m,r[4],2);goto L8c052e8e; /* mov.w @r4,r2 */
L8c052e8e: *stop=0x8c052e8eu;r[2]+=1u;goto L8c052e90; /* add #0x1,r2 */
L8c052e90: *stop=0x8c052e90u;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[2],2);goto L8c052e92; /* mov.w r2,@r4 */
L8c052e92: *stop=0x8c052e92u;if(!valid_ram(r[7],4u))return -3;r[3]=rd(m,r[7],4);goto L8c052e94; /* mov.l @r7,r3 */
L8c052e94: *stop=0x8c052e94u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[14]);goto L8c052e96; /* cmp/ge r14,r3 */
L8c052e96: take=(*sr&1u);*stop=0x8c052e98u;r[5]+=12u;if(take)goto L8c052ea6;goto L8c052e9a; /* bt/s 0x8c052ea6 */
L8c052e9a: *stop=0x8c052e9au;if(!valid_ram(r[4],2u))return -3;r[2]=sx(m,r[4],2);goto L8c052e9c; /* mov.w @r4,r2 */
L8c052e9c: *stop=0x8c052e9cu;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[13]);goto L8c052e9e; /* cmp/ge r13,r2 */
L8c052e9e: if((*sr&1u))goto L8c052ea6;goto L8c052ea0; /* bt 0x8c052ea6 */
L8c052ea0: *stop=0x8c052ea0u;r[6]+=4294967284u;goto L8c052ea2; /* add #-0xc,r6 */
L8c052ea2: *stop=0x8c052ea2u;*sr=(*sr&~1u)|(r[6]>=r[10]);goto L8c052ea4; /* cmp/hs r10,r6 */
L8c052ea4: if((*sr&1u))goto L8c052e6c;goto L8c052ea6; /* bt 0x8c052e6c */
L8c052ea6: *stop=0x8c052ea6u;r[2]=rd(m,0x8c052f80u,4);goto L8c052ea8; /* mov.l 0x8c052f80,r2 */
L8c052ea8: *stop=0x8c052ea8u;r[7]=rd(m,0x8c052f78u,4);goto L8c052eaa; /* mov.l 0x8c052f78,r7 */
L8c052eaa: *stop=0x8c052eaau;r[11]=rd(m,0x8c052f7cu,4);goto L8c052eac; /* mov.l 0x8c052f7c,r11 */
L8c052eac: *stop=0x8c052eacu;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[12],4);goto L8c052eae; /* mov.l r12,@r7 */
L8c052eae: *stop=0x8c052eaeu;if(!valid_ram(r[2],4u))return -3;r[0]=rd(m,r[2],4);goto L8c052eb0; /* mov.l @r2,r0 */
L8c052eb0: *stop=0x8c052eb0u;*sr=(*sr&~1u)|(r[0]==0x00000001u);goto L8c052eb2; /* cmp/eq #0x1,r0 */
L8c052eb2: if(!(*sr&1u))goto L8c052f26;goto L8c052eb4; /* bf 0x8c052f26 */
L8c052eb4: *stop=0x8c052eb4u;r[3]=rd(m,0x8c052f84u,4);goto L8c052eb6; /* mov.l 0x8c052f84,r3 */
L8c052eb6: *stop=0x8c052eb6u;if(!valid_ram(r[3],4u))return -3;r[1]=rd(m,r[3],4);goto L8c052eb8; /* mov.l @r3,r1 */
L8c052eb8: *stop=0x8c052eb8u;*sr=(*sr&~1u)|!(r[1]&r[1]);goto L8c052eba; /* tst r1,r1 */
L8c052eba: if((*sr&1u))goto L8c052f26;goto L8c052ebc; /* bt 0x8c052f26 */
L8c052ebc: *stop=0x8c052ebcu;r[3]=rd(m,0x8c052f84u,4);goto L8c052ebe; /* mov.l 0x8c052f84,r3 */
L8c052ebe: *stop=0x8c052ebeu;r[0]=rd(m,0x8c052f88u,4);goto L8c052ec0; /* mov.l 0x8c052f88,r0 */
L8c052ec0: *stop=0x8c052ec0u;if(!valid_ram(r[3],4u))return -3;r[6]=rd(m,r[3],4);goto L8c052ec2; /* mov.l @r3,r6 */
L8c052ec2: *stop=0x8c052ec2u;if(!valid_ram(r[15],4u))return -3;wr(m,r[15],r[6],4);goto L8c052ec4; /* mov.l r6,@r15 */
L8c052ec4: *stop=0x8c052ec4u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);goto L8c052ec6; /* mov.l @r15,r10 */
L8c052ec6: *stop=0x8c052ec6u;if(!valid_ram(r[6]+12u,4u))return -3;r[6]=rd(m,r[6]+0xcu,4);goto L8c052ec8; /* mov.l @(0xc,r6),r6 */
L8c052ec8: *stop=0x8c052ecau;if(!valid_ram(r[10],2u))return -3;r[10]=sx(m,r[10],2);goto L8c052f20; /* bra 0x8c052f20 */
L8c052ecc: *stop=0x8c052eccu;if(!valid_ram(r[6]+32u,4u))return -3;r[3]=rd(m,r[6]+0x20u,4);goto L8c052ece; /* mov.l @(0x20,r6),r3 */
L8c052ece: *stop=0x8c052eceu;*sr=(*sr&~1u)|!(r[0]&r[3]);goto L8c052ed0; /* tst r0,r3 */
L8c052ed0: if((*sr&1u))goto L8c052f1c;goto L8c052ed2; /* bt 0x8c052f1c */
L8c052ed2: *stop=0x8c052ed2u;if(!valid_ram(r[6],4u))return -3;r[2]=rd(m,r[6],4);goto L8c052ed4; /* mov.l @r6,r2 */
L8c052ed4: *stop=0x8c052ed4u;if(!valid_ram(r[6]+4u,4u))return -3;r[3]=rd(m,r[6]+0x4u,4);goto L8c052ed6; /* mov.l @(0x4,r6),r3 */
L8c052ed6: *stop=0x8c052ed6u;if(!valid_ram(r[11],4u))return -3;wr(m,r[11],r[2],4);goto L8c052ed8; /* mov.l r2,@r11 */
L8c052ed8: *stop=0x8c052ed8u;if(!valid_ram(r[11]+4u,4u))return -3;wr(m,r[11]+0x4u,r[3],4);goto L8c052eda; /* mov.l r3,@(0x4,r11) */
L8c052eda: *stop=0x8c052edau;if(!valid_ram(r[6]+8u,4u))return -3;r[2]=rd(m,r[6]+0x8u,4);goto L8c052edc; /* mov.l @(0x8,r6),r2 */
L8c052edc: *stop=0x8c052edcu;if(!valid_ram(r[6]+12u,4u))return -3;r[3]=rd(m,r[6]+0xcu,4);goto L8c052ede; /* mov.l @(0xc,r6),r3 */
L8c052ede: *stop=0x8c052edeu;if(!valid_ram(r[11]+8u,4u))return -3;wr(m,r[11]+0x8u,r[2],4);goto L8c052ee0; /* mov.l r2,@(0x8,r11) */
L8c052ee0: *stop=0x8c052ee0u;if(!valid_ram(r[11]+12u,4u))return -3;wr(m,r[11]+0xcu,r[3],4);goto L8c052ee2; /* mov.l r3,@(0xc,r11) */
L8c052ee2: *stop=0x8c052ee2u;if(!valid_ram(r[6]+16u,4u))return -3;r[2]=rd(m,r[6]+0x10u,4);goto L8c052ee4; /* mov.l @(0x10,r6),r2 */
L8c052ee4: *stop=0x8c052ee4u;if(!valid_ram(r[6]+20u,4u))return -3;r[3]=rd(m,r[6]+0x14u,4);goto L8c052ee6; /* mov.l @(0x14,r6),r3 */
L8c052ee6: *stop=0x8c052ee6u;if(!valid_ram(r[11]+16u,4u))return -3;wr(m,r[11]+0x10u,r[2],4);goto L8c052ee8; /* mov.l r2,@(0x10,r11) */
L8c052ee8: *stop=0x8c052ee8u;if(!valid_ram(r[11]+20u,4u))return -3;wr(m,r[11]+0x14u,r[3],4);goto L8c052eea; /* mov.l r3,@(0x14,r11) */
L8c052eea: *stop=0x8c052eeau;if(!valid_ram(r[6]+24u,4u))return -3;r[2]=rd(m,r[6]+0x18u,4);goto L8c052eec; /* mov.l @(0x18,r6),r2 */
L8c052eec: *stop=0x8c052eecu;if(!valid_ram(r[6]+28u,4u))return -3;r[3]=rd(m,r[6]+0x1cu,4);goto L8c052eee; /* mov.l @(0x1c,r6),r3 */
L8c052eee: *stop=0x8c052eeeu;if(!valid_ram(r[11]+24u,4u))return -3;wr(m,r[11]+0x18u,r[2],4);goto L8c052ef0; /* mov.l r2,@(0x18,r11) */
L8c052ef0: *stop=0x8c052ef0u;if(!valid_ram(r[11]+28u,4u))return -3;wr(m,r[11]+0x1cu,r[3],4);goto L8c052ef2; /* mov.l r3,@(0x1c,r11) */
L8c052ef2: *stop=0x8c052ef2u;if(!valid_ram(r[6]+32u,4u))return -3;r[2]=rd(m,r[6]+0x20u,4);goto L8c052ef4; /* mov.l @(0x20,r6),r2 */
L8c052ef4: *stop=0x8c052ef4u;if(!valid_ram(r[11]+32u,4u))return -3;wr(m,r[11]+0x20u,r[2],4);goto L8c052ef6; /* mov.l r2,@(0x20,r11) */
L8c052ef6: *stop=0x8c052ef6u;r[11]+=36u;goto L8c052ef8; /* add #0x24,r11 */
L8c052ef8: *stop=0x8c052ef8u;if(!valid_ram(r[7],4u))return -3;r[2]=rd(m,r[7],4);goto L8c052efa; /* mov.l @r7,r2 */
L8c052efa: *stop=0x8c052efau;r[2]+=1u;goto L8c052efc; /* add #0x1,r2 */
L8c052efc: *stop=0x8c052efcu;if(!valid_ram(r[7],4u))return -3;wr(m,r[7],r[2],4);goto L8c052efe; /* mov.l r2,@r7 */
L8c052efe: *stop=0x8c052efeu;if(!valid_ram(r[6]+32u,4u))return -3;r[3]=rd(m,r[6]+0x20u,4);goto L8c052f00; /* mov.l @(0x20,r6),r3 */
L8c052f00: *stop=0x8c052f00u;if(!valid_ram(r[5],4u))return -3;wr(m,r[5],r[3],4);goto L8c052f02; /* mov.l r3,@r5 */
L8c052f02: *stop=0x8c052f02u;if(!valid_ram(r[5]+8u,4u))return -3;wr(m,r[5]+0x8u,r[12],4);goto L8c052f04; /* mov.l r12,@(0x8,r5) */
L8c052f04: *stop=0x8c052f04u;if(!valid_ram(r[6]+24u,4u))return -3;r[2]=rd(m,r[6]+0x18u,4);goto L8c052f06; /* mov.l @(0x18,r6),r2 */
L8c052f06: *stop=0x8c052f06u;if(!valid_ram(r[5]+4u,4u))return -3;wr(m,r[5]+0x4u,r[2],4);goto L8c052f08; /* mov.l r2,@(0x4,r5) */
L8c052f08: *stop=0x8c052f08u;if(!valid_ram(r[4],2u))return -3;r[3]=sx(m,r[4],2);goto L8c052f0a; /* mov.w @r4,r3 */
L8c052f0a: *stop=0x8c052f0au;r[3]+=1u;goto L8c052f0c; /* add #0x1,r3 */
L8c052f0c: *stop=0x8c052f0cu;if(!valid_ram(r[4],2u))return -3;wr(m,r[4],r[3],2);goto L8c052f0e; /* mov.w r3,@r4 */
L8c052f0e: *stop=0x8c052f0eu;if(!valid_ram(r[7],4u))return -3;r[2]=rd(m,r[7],4);goto L8c052f10; /* mov.l @r7,r2 */
L8c052f10: *stop=0x8c052f10u;*sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[14]);goto L8c052f12; /* cmp/ge r14,r2 */
L8c052f12: take=(*sr&1u);*stop=0x8c052f14u;r[5]+=12u;if(take)goto L8c052f26;goto L8c052f16; /* bt/s 0x8c052f26 */
L8c052f16: *stop=0x8c052f16u;if(!valid_ram(r[4],2u))return -3;r[3]=sx(m,r[4],2);goto L8c052f18; /* mov.w @r4,r3 */
L8c052f18: *stop=0x8c052f18u;*sr=(*sr&~1u)|((int32_t)r[3]>=(int32_t)r[13]);goto L8c052f1a; /* cmp/ge r13,r3 */
L8c052f1a: if((*sr&1u))goto L8c052f26;goto L8c052f1c; /* bt 0x8c052f26 */
L8c052f1c: *stop=0x8c052f1cu;r[10]+=4294967295u;goto L8c052f1e; /* add #-0x1,r10 */
L8c052f1e: *stop=0x8c052f1eu;r[6]+=36u;goto L8c052f20; /* add #0x24,r6 */
L8c052f20: *stop=0x8c052f20u;r[2]=(uint32_t)(int32_t)(int16_t)r[10];goto L8c052f22; /* exts.w r10,r2 */
L8c052f22: *stop=0x8c052f22u;*sr=(*sr&~1u)|((int32_t)r[2]>0);goto L8c052f24; /* cmp/pl r2 */
L8c052f24: if((*sr&1u))goto L8c052ecc;goto L8c052f26; /* bt 0x8c052ecc */
L8c052f26: *stop=0x8c052f26u;r[15]+=4u;goto L8c052f28; /* add #0x4,r15 */
L8c052f28: *stop=0x8c052f28u;if(!valid_ram(r[15],4u))return -3;r[10]=rd(m,r[15],4);r[15]+=4;goto L8c052f2a; /* mov.l @r15+,r10 */
L8c052f2a: *stop=0x8c052f2au;if(!valid_ram(r[15],4u))return -3;r[11]=rd(m,r[15],4);r[15]+=4;goto L8c052f2c; /* mov.l @r15+,r11 */
L8c052f2c: *stop=0x8c052f2cu;if(!valid_ram(r[15],4u))return -3;r[12]=rd(m,r[15],4);r[15]+=4;goto L8c052f2e; /* mov.l @r15+,r12 */
L8c052f2e: *stop=0x8c052f2eu;if(!valid_ram(r[15],4u))return -3;r[13]=rd(m,r[15],4);r[15]+=4;goto L8c052f30; /* mov.l @r15+,r13 */
L8c052f30: *stop=0x8c052f30u;if(*pr!=entry_pr)return -3;*stop=0x8c052f32u;if(!valid_ram(r[15],4u))return -3;r[14]=rd(m,r[15],4);r[15]+=4;return 0; /* rts */
}
