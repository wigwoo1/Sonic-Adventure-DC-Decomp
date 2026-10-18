#ifndef SA_TASK_CREATE_H
#define SA_TASK_CREATE_H
#include "stage00_task.h"
/* Native slots represent the original 12 four-byte task fields as pointers.
 * Complete scheduler semantics and layout are still being recovered. */
struct sa_created_task { void *slots[12]; };
struct sa_task_create_services {
 void *context;
 struct sa_created_task *(*create_8c0985ec)(void *,uint32_t,uintptr_t);
 void *(*allocate_8c098b0c)(void *,uint32_t,uint32_t);
 void (*dispose_8c09865c)(void *,struct sa_created_task *);
};
SA_STAGE_EXPORT struct sa_created_task *sa_task_create_8c09852e(
 uint32_t flags,uint32_t level,uintptr_t callback,const struct sa_task_create_services *services);
#endif
