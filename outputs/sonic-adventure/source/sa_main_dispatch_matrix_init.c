#include "fpu_cleanup_transfer.h"
typedef void (*sa_matrix_init_observer)(void *,uint32_t,const uint8_t *,const struct sa_fpu_banks *,const uint32_t *,const uint32_t *,const uint32_t *);
static uint32_t read_word(const uint8_t *ram,uint32_t address){
 const uint32_t a=address&0xffffffu;
 return ram[a]|((uint32_t)ram[a+1]<<8)|((uint32_t)ram[a+2]<<16)|((uint32_t)ram[a+3]<<24);
}
static void write_word(uint8_t *ram,uint32_t address,uint32_t value){
 const uint32_t a=address&0xffffffu;
 for(unsigned i=0;i<4;++i)ram[a+i]=(uint8_t)(value>>(8u*i));
}
static void store_predecrement(uint8_t *ram,const struct sa_fpu_banks *f,uint32_t *r,unsigned address_reg,unsigned source){
 unsigned bank=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){
  bank^=source&1u;source&=~1u;
  r[address_reg]-=8u;
  write_word(ram,r[address_reg],f->bank[bank][source]);
  write_word(ram,r[address_reg]+4u,f->bank[bank][source+1u]);
 }else{
  r[address_reg]-=4u;write_word(ram,r[address_reg],f->bank[bank][source]);
 }
}
static void move_register(struct sa_fpu_banks *f,unsigned source,unsigned destination){
 const unsigned active=(f->fpscr>>21)&1u;
 if(f->fpscr&0x100000u){
  const unsigned from=active^(source&1u),to=active^(destination&1u);
  source&=~1u;destination&=~1u;
  const uint32_t low=f->bank[from][source],high=f->bank[from][source+1u];
  f->bank[to][destination]=low;f->bank[to][destination+1u]=high;
 }else f->bank[active][destination]=f->bank[active][source];
}
/* Retail 8c057b40..8c057bac: 50 instructions plus two literal words.
 * Inferred name. Structured native C; loops represent fixed store sequences.
 * Host 0 = complete, retail R0 = 0 (limit reached) or 1 (pushed/initialized).
 * Reached FLDI1 with PR=1 returns -1 before execution, after the original
 * global/GPR changes. The limit-reached path works for all FPU modes.
 * Raw SZ=1 behavior is retained, including toggled 32-bit stores and partial
 * matrix writes; only the ordinary initial SZ=0 path creates a full identity.
 * Requires mapped 16 MiB RAM and valid original stack-global pointers.
 */
SA_STAGE_EXPORT int sa_main_dispatch_matrix_init_8c057b40(
 uint8_t *ram,struct sa_fpu_banks *f,uint32_t *sr,uint32_t *r,uint32_t *pr,
 uint32_t *fpul,uint32_t *macl,void *context,sa_matrix_init_observer observe){
 (void)pr;(void)fpul;(void)macl;(void)context;(void)observe;
 r[1]=read_word(ram,0x8c057b50u);
 r[6]=read_word(ram,0x8c057b54u);
 r[3]=read_word(ram,r[1]);r[2]=read_word(ram,r[6]);
 *sr=(*sr&~1u)|((int32_t)r[2]>=(int32_t)r[3]);
 if(*sr&1u){r[0]=0u;return 0;}
 r[2]+=1u;r[1]=read_word(ram,0x8c057bacu);
 write_word(ram,r[6],r[2]);
 r[5]=read_word(ram,r[1]);r[5]+=0x40u;
 write_word(ram,r[1],r[5]);r[4]=r[5];
 if(f->fpscr&0x80000u)return -1; /* 8c057b66 FLDI1 */
 const unsigned bank=(f->fpscr>>21)&1u;
 f->bank[bank][0]=0x3f800000u;f->bank[bank][1]=0u;
 f->bank[bank][2]=0u;f->bank[bank][3]=0x3f800000u;
 f->bank[bank][4]=0u;f->bank[bank][5]=0u;
 f->fpscr^=0x100000u;
 for(unsigned i=16;i>0;i-=2)store_predecrement(ram,f,r,5,i-1u);
 r[4]+=0x40u;
 move_register(f,0,1);move_register(f,4,3);move_register(f,2,5);move_register(f,4,7);
 move_register(f,4,9);move_register(f,0,11);move_register(f,4,13);move_register(f,2,15);
 for(unsigned i=16;i>0;i-=2)store_predecrement(ram,f,r,4,i-1u);
 f->fpscr^=0x100000u;
 r[0]=1u;return 0;
}
