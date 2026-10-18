#ifndef SA_SH4_VECTOR_NUMERIC_H
#define SA_SH4_VECTOR_NUMERIC_H
#include "fpu_cleanup_transfer.h"
/* Native numerical FSCA/FTRV/FIPR/FSRRA using the pinned MIT-licensed core.
 * 0 complete, 1 enabled FPU trap (FPSCR effects retained, destinations intact),
 * -1 invalid opcode/reserved RM (state untouched). No hardware trap delivery.
 * FPUL is an input bit pattern; guest integer registers/SR/PR are not changed.
 * FR selects the active bank and XMTRX is the opposite bank; SZ is ignored.
 * PR=1 follows the dependency's measured no-op/trap semantics.
 */
SA_STAGE_EXPORT int sa_sh4_vector_numeric(uint32_t,struct sa_fpu_banks *,uint32_t);
#endif
