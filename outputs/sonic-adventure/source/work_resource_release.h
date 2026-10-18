#ifndef SA_WORK_RESOURCE_RELEASE_H
#define SA_WORK_RESOURCE_RELEASE_H
#include "stage00_task.h"
/* Projected native fields: original flags are at +2, child at +12;
 * original owning work pointer is at +56. Full resource ABI is unrecovered. */
struct sa_work_resource { uint16_t flags; void *child; };
struct sa_resource_owner { struct sa_work_resource *resource; };
SA_STAGE_EXPORT void sa_work_resource_release_8c0344a0(struct sa_resource_owner *,
 void *context,void (*free_resource)(void *,void *));
#endif
