#ifndef SA_SH4_WIDEN_COMPARE_H
#define SA_SH4_WIDEN_COMPARE_H
#include "fpu_cleanup_transfer.h"
/* Manual-based software instruction model; no physical trap delivery.
 * DRn high word is active FR[n], low word FR[n+1]. SZ does not select a bank.
 * Return 0 completed, 1 FPU exception (FPSCR retained, result/T unchanged),
 * -1 invalid register/precision contract (no state change).
 */
SA_STAGE_EXPORT int sa_sh4_fcnvsd(struct sa_fpu_banks *,uint32_t,unsigned);
SA_STAGE_EXPORT int sa_sh4_compare_pair(struct sa_fpu_banks *,uint32_t *,unsigned,unsigned,int);
#endif
