#ifndef SA_FPU_CLEANUP_TRANSFER_H
#define SA_FPU_CLEANUP_TRANSFER_H
#include "stage00_task.h"
struct sa_fpu_banks { uint32_t bank[2][16],fpscr; };
/* Support code for recovered cleanup, not a recovered game routine.
 * Execute FLDI0 FR3 then FMOV FR3,@destination using FPSCR FR/SZ.
 * Return 0 for unsupported PR=1, leaving state and output untouched.
 * Caller supplies 8 valid output bytes; alignment/exceptions are not modeled. */
SA_STAGE_EXPORT int sa_fpu_cleanup_transfer(struct sa_fpu_banks *,uint8_t *destination);
#endif
