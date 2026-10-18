#ifndef SA_SH4_FMAC_H
#define SA_SH4_FMAC_H
#include "stage00_task.h"
/* Finite operands only. 0 complete; 1 trap preserves destination; -1 unsupported.
 * Operand pointers represent FR0/FRm/FRn and can alias. DN may flush them. */
SA_STAGE_EXPORT int sa_sh4_fmac(uint32_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t *);
#endif
