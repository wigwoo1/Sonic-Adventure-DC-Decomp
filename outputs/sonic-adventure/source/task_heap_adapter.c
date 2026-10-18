/* Native connection adapter; not an additional recovered original routine. */
#include "task_heap.h"
#include "task_allocate.h"
static void *allocate_heap(void *context,uint32_t size) {
 return sa_task_heap_allocate_8c098a80(context,size);
}
SA_STAGE_EXPORT void *sa_task_allocate_from_heap(struct sa_task_heap *heap,
 uint32_t count,uint32_t size) {
 struct sa_task_allocator allocator={heap,allocate_heap};
 return sa_task_allocate_zero_8c098b0c(count,size,&allocator);
}
