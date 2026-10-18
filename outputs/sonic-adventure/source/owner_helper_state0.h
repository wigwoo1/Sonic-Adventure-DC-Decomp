#ifndef SA_OWNER_HELPER_STATE0_H
#define SA_OWNER_HELPER_STATE0_H
#include "stage00_task.h"
/* Internal state body, not a complete original routine. argc identifies
 * meaningful service args; callback also receives incoming r0 and SR. */
typedef uint32_t (*sa_owner_state0_service)(void *,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t *);
SA_STAGE_EXPORT uint32_t sa_owner_helper_state0(uint8_t *,uint32_t *,uint32_t,uint32_t,void *,sa_owner_state0_service);
#endif
