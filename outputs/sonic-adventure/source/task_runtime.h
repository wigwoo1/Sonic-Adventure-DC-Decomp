#ifndef SA_TASK_RUNTIME_H
#define SA_TASK_RUNTIME_H
#include "task_dispose.h"
#include "task_children.h"
/* Adapter connecting recovered task routines. Unknown callback/resource bodies
 * remain explicit services. Callback identities use original address tokens. */
struct sa_task_runtime {
 struct sa_task_lists *lists;
 void *context;
 void (*invoke_external)(void *,uintptr_t,struct sa_created_task *);
 void (*release_work)(void *,struct sa_created_task *);
 void (*free_allocation)(void *,void *);
};
SA_STAGE_EXPORT void sa_task_runtime_dispose(struct sa_task_runtime *,struct sa_created_task *);
SA_STAGE_EXPORT void sa_task_runtime_children(struct sa_task_runtime *,struct sa_created_task *);
#endif
