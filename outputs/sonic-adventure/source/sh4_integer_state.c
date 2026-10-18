#include "sh4_integer_state.h"
unsigned sa_sh4_integer_bank(uint32_t sr){return (unsigned)((sr>>30)&(sr>>29)&1u);}
static int privileged(uint32_t sr,unsigned delay){
 return (sr&0x40000000u)?0:(delay?SA_SH4_SLOT_FAULT:SA_SH4_PRIVILEGE_FAULT);
}
int sa_sh4_ldc_sr(uint32_t value,uint32_t *sr,uint32_t r[16],struct sa_sh4_integer_extra *extra,unsigned delay){
 if(delay)return SA_SH4_SLOT_FAULT;
 int status=privileged(*sr,0);if(status)return status;
 uint32_t next=value&0x700083f3u;
 if(sa_sh4_integer_bank(*sr)!=sa_sh4_integer_bank(next)){
  for(unsigned i=0;i<8;i++){uint32_t temporary=r[i];r[i]=extra->other[i];extra->other[i]=temporary;}
 }
 *sr=next;return 0;
}
int sa_sh4_stc_sr(uint32_t sr,uint32_t *out,unsigned delay){
 int status=privileged(sr,delay);if(status)return status;*out=sr;return 0;
}
int sa_sh4_ldc_bank(uint32_t value,unsigned n,uint32_t sr,struct sa_sh4_integer_extra *extra,unsigned delay){
 if(n>=8)return SA_SH4_BANK_ARGUMENT;
 int status=privileged(sr,delay);if(status)return status;extra->other[n]=value;return 0;
}
int sa_sh4_stc_bank(unsigned n,uint32_t sr,const struct sa_sh4_integer_extra *extra,uint32_t *out,unsigned delay){
 if(n>=8)return SA_SH4_BANK_ARGUMENT;
 int status=privileged(sr,delay);if(status)return status;*out=extra->other[n];return 0;
}
void sa_sh4_dmuls_state(uint32_t left,uint32_t right,uint32_t *macl,struct sa_sh4_integer_extra *extra){
 sa_sh4_dmuls(left,right,macl,&extra->mach);
}
