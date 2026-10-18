#include "fpu_cleanup_transfer.h"
/* Original 8c638f94..8c638fc0; FRCHG pair selects and restores XF bank.
 * Single-width mode only; double-width stores require separate verification. */
SA_STAGE_EXPORT int sa_model_matrix_bank_save_8c638f94(uint8_t *ram,struct sa_fpu_banks *fpu,uint32_t destination){
 if(fpu->fpscr&0x180000u)return -1;
 fpu->fpscr^=0x200000u;uint32_t bank=(fpu->fpscr>>21)&1u,address=destination+64u;
 for(unsigned i=16;i>0;--i){address-=4u;uint8_t *p=ram+((address&0x1fffffffu)-0x0c000000u);uint32_t value=fpu->bank[bank][i-1u];for(unsigned b=0;b<4;b++)p[b]=(uint8_t)(value>>(b*8));}
 fpu->fpscr^=0x200000u;return 0;
}
