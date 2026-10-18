#ifndef SA_OWNER_HELPER_REPEAT_H
#define SA_OWNER_HELPER_REPEAT_H
#include "stage00_task.h"
/* Internal bodies 3/9/10. Requires a positive period and terminating finite
 * normalization; original zero-period/nonprogressing loops are not repaired. */
SA_STAGE_EXPORT int sa_owner_helper_repeat(uint8_t *,uint32_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,uint32_t,uint32_t *);
#endif
