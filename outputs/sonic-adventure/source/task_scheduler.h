#ifndef SA_TASK_SCHEDULER_H
#define SA_TASK_SCHEDULER_H
#include "task_lists.h"
struct sa_task_scheduler {
 struct sa_task_lists *lists;
 uintptr_t active_callback;
 void *context;
 void (*invoke)(void *,uintptr_t,struct sa_created_task *);
};
/* Valid retail levels 0..8; level 8 aliases level 7. */
SA_STAGE_EXPORT void sa_task_dispatch_8c09878c(struct sa_task_scheduler *,uint32_t);
SA_STAGE_EXPORT void sa_task_secondary_children_8c098842(struct sa_task_scheduler *,struct sa_created_task *);
SA_STAGE_EXPORT void sa_task_secondary_dispatch_8c098800(struct sa_task_scheduler *,uint32_t);
#endif
