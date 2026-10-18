/* SH-4 Software Manual Rev6: sections 2.5, 6, 9.32, 9.37, 9.38.
 * Little-endian FMOV transfers preserve pair register order, rather than
 * serializing a host 64-bit numeric value. */
#include "fpu_cleanup_transfer.h"
static void word(uint8_t *destination,uint32_t value) {
 for(unsigned byte=0;byte<4;byte++)destination[byte]=(uint8_t)(value>>(byte*8));
}
SA_STAGE_EXPORT int sa_fpu_cleanup_transfer(struct sa_fpu_banks *fpu,uint8_t *destination) {
 if(fpu->fpscr & 0x80000u)return 0;
 unsigned active=(fpu->fpscr>>21)&1u;
 fpu->bank[active][3]=0;
 if(fpu->fpscr & 0x100000u) {
  unsigned alternate=active^1u;
  word(destination,fpu->bank[alternate][2]);
  word(destination+4,fpu->bank[alternate][3]);
 } else word(destination,fpu->bank[active][3]);
 return 1;
}
