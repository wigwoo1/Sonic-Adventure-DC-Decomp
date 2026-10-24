#ifndef SA_SH4_VECTOR_BASE_H
#define SA_SH4_VECTOR_BASE_H
#include "sh4_integer_state.h"
/* Only the reached STC VBR operation: no exception delivery or VBR writes. */
SA_STAGE_EXPORT int sa_sh4_stc_vbr(uint32_t vbr,uint32_t sr,uint32_t *out,unsigned delay_slot);
#endif
