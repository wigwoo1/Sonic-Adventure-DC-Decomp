#ifndef SA_TASK_ALLOCATE_H
#define SA_TASK_ALLOCATE_H
#include "stage00_task.h"
struct sa_task_allocator {
 void *context;
 void *(*allocate_8c098a80)(void *,uint32_t);
};
/* The original computes a wrapping 32-bit total from count * align4(size).
 * Successful storage must be four-byte aligned with at least total bytes. */
SA_STAGE_EXPORT void *sa_task_allocate_zero_8c098b0c(uint32_t count,uint32_t size,
 const struct sa_task_allocator *allocator);
#endif
