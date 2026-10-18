#ifndef SA_OWNER_HELPER_PREPARE_H
#define SA_OWNER_HELPER_PREPARE_H
#include "stage00_task.h"
/* Verified front section of 8c079476, not the complete original routine. */
struct sa_owner_helper_selection {uint32_t state,descriptor_offset,target;};
SA_STAGE_EXPORT void sa_owner_helper_prepare(uint8_t *,uint32_t *,uint32_t,struct sa_owner_helper_selection *);
#endif
