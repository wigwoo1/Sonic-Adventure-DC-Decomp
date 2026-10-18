#ifndef SA_TASK_WORK_RELEASE_H
#define SA_TASK_WORK_RELEASE_H
#include "task_create.h"
struct sa_task_work_release_services {
 void *context;
 void (*release_8c0344a0)(void *,void *);
 void (*release_8c04b468)(void *,void *);
 void (*free_8c098b42)(void *,void *);
};
/* Task slot 8 must point to at least 64 bytes, with 32-bit fields at 56/60. */
SA_STAGE_EXPORT void sa_task_work_release_8c098998(struct sa_created_task *,
 const struct sa_task_work_release_services *);
#endif
