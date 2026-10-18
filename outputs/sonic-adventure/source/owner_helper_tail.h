#ifndef SA_OWNER_HELPER_TAIL_H
#define SA_OWNER_HELPER_TAIL_H
#include "stage00_task.h"
/* Internal shared-tail source; 0 completed, -1 unsupported FPU execution. */
SA_STAGE_EXPORT int sa_owner_helper_tail(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t *);
#endif
