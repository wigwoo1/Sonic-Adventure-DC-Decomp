#ifndef SA_SH4_MULTIPLY_EXTENDED_H
#define SA_SH4_MULTIPLY_EXTENDED_H
#include "stage00_task.h"
/* Binary32 FMUL extension for infinity classes. Existing finite/NaN helper
 * remains the delegate. 0 complete; 1 FPU trap; -1 PR/reserved RM unsupported.
 * New exceptional path classifies DN inputs without rewriting source operands.
 * Destination aliases either operand; FPSCR must be separate valid storage.
 * No actual CPU exception dispatch. Reference: SH-4 manual rev6 section 9.39.
 */
SA_STAGE_EXPORT int sa_sh4_multiply_extended(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
#endif
