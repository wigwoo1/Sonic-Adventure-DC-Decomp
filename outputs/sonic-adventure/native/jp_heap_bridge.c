/* Authored native test interface. Game implementation is an external input. */
#include <sg_xpt.h>
#include "system/task.h"

#if defined(_WIN32)
#define JP_EXPORT __declspec(dllexport)
/* The Windows floating-point ABI marker; no replacement game behavior. */
int _fltused = 0;
#else
#define JP_EXPORT __attribute__((visibility("default")))
#endif

/* task.c's declared 0x90000-byte arena and its four-pointer end header.
 * Match the host's pointer width for this in-memory component experiment.
 * This is not a declaration that disc structures have become 64-bit safe. */
unsigned char heap_arena[0x90000u + 4u * sizeof(void *)]
    __attribute__((aligned(32)));

JP_EXPORT void jp_heap_reset(void) { heap_init(); }
JP_EXPORT void *jp_heap_allocate(int bytes) { return heap_alloc(bytes); }
JP_EXPORT void *jp_heap_allocate_zeroed(int count, int bytes)
{
    return heap_calloc(count, bytes);
}
JP_EXPORT void jp_heap_release(void *pointer) { heap_free(pointer); }
JP_EXPORT void jp_heap_statistics(int *total, int *largest)
{
    heap_stats(total, largest);
}
JP_EXPORT unsigned jp_heap_pointer_bytes(void) { return sizeof(void *); }
JP_EXPORT unsigned jp_heap_task_bytes(void) { return sizeof(struct task); }
