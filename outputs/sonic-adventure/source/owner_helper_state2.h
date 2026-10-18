#ifndef SA_OWNER_HELPER_STATE2_H
#define SA_OWNER_HELPER_STATE2_H
#include "owner_helper_state0.h"
/* 0: state returns; 1: shared tail required; -1: unsupported arithmetic.
 * Internal state body only, not the full original routine. */
SA_STAGE_EXPORT int sa_owner_helper_state2(uint8_t *,uint32_t *,uint32_t *,uint32_t,uint32_t,void *,sa_owner_state0_service,uint32_t *);
#endif
