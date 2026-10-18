#include "fpu_cleanup_transfer.h"
#include "sh4_compare_bits.h"
SA_STAGE_EXPORT int sa_sh4_compare(uint32_t a,uint32_t b,uint32_t *fpscr,uint32_t *sr,int greater){return sa_sh4_compare_bits(a,b,fpscr,sr,greater);}
