#ifndef SA_SH4_FLOAT_I32_H
#define SA_SH4_FLOAT_I32_H
#include "stage00_task.h"
/* 0: converted; 1: inexact-enabled trap (destination preserved);
 * -1: unsupported PR or reserved RM (all outputs preserved).
 * raw contains signed int32 bits. Numerical conversion uses integer operations. */
SA_STAGE_EXPORT int sa_sh4_float_i32(uint32_t raw,uint32_t *fpscr,uint32_t *destination);
#endif
