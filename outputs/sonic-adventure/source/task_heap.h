#ifndef SA_TASK_HEAP_H
#define SA_TASK_HEAP_H
#include "stage00_task.h"
/* Native arena uses relative offsets to preserve the retail 16-byte headers.
 * Offset zero is null. All linked offsets must designate valid arena storage.
 * Header fields: free-next, free-previous, physical-next, physical-previous. */
struct sa_task_heap { uint8_t *arena; uint32_t free_head, field_8c79f040; };
/* Retail heap has 0x90000 bytes and a terminal header. Native offset 16
 * reserves zero for null; caller supplies at least 0x90020 aligned bytes. */
SA_STAGE_EXPORT void sa_task_heap_initialize_8c098a10(struct sa_task_heap *);
SA_STAGE_EXPORT void *sa_task_heap_allocate_8c098a80(struct sa_task_heap *,uint32_t size);
/* Null is ignored; otherwise pointer must belong to this heap. The original
 * spins forever when both free-link fields of the released block are nonzero. */
SA_STAGE_EXPORT void sa_task_heap_free_8c098b42(struct sa_task_heap *,void *pointer);
SA_STAGE_EXPORT void *sa_task_allocate_from_heap(struct sa_task_heap *,uint32_t count,uint32_t size);
#endif
