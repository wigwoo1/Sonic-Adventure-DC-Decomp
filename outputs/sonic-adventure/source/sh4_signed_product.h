#ifndef SA_SH4_SIGNED_PRODUCT_H
#define SA_SH4_SIGNED_PRODUCT_H
#include "fpu_cleanup_transfer.h"
/* Guest input words and explicit, distinct low/high output words. No SR change. */
SA_STAGE_EXPORT void sa_sh4_dmuls(uint32_t left,uint32_t right,
 uint32_t *macl,uint32_t *mach);
#endif
