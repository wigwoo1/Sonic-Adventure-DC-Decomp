#ifndef SA_SH4_DIVIDE_H
#define SA_SH4_DIVIDE_H
#include "stage00_task.h"
/* Finite single precision, nonzero divisor. 0 completes; 1 guarded FPU trap
 * preserves destination; -1 unsupported precision/mode/class preserves outputs. */
SA_STAGE_EXPORT int sa_sh4_divide(uint32_t *,uint32_t *,uint32_t *,uint32_t *);
#endif
