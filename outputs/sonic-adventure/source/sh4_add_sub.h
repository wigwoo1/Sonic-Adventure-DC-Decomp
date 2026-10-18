#ifndef SA_SH4_ADD_SUB_H
#define SA_SH4_ADD_SUB_H
#include "stage00_task.h"
/* 0: completed, 1: trap with destination preserved, -1: unsupported.
 * DN classification may flush operand words in place. */
SA_STAGE_EXPORT int sa_sh4_add_sub(uint32_t *,uint32_t *,uint32_t *,uint32_t *,int);
#endif
