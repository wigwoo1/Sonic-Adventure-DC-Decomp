/* Authored native host bridge. Implementation and record types are external.
 * This version qualifies Windows x64's LLP64 ABI only. On-disc record decoding
 * must be separate; widening a host pointer is not a file-format conversion. */
#include "system/task.h"
#include "event/event.h"
#include "collision/collision.h"
#include "jp_task_runtime_layout.h"
#define JP_EXPORT __declspec(dllexport)
#define OFFSET(record, field) ((unsigned)__builtin_offsetof(struct record, field))
_Static_assert(sizeof(void *) == 8 && sizeof(Uint32) == 4, "Windows x64 ABI");
_Static_assert(sizeof(unsigned) == 4 && sizeof(Float) == 4, "native scalar ABI");
_Static_assert(4096 * sizeof(struct task) + 4 * sizeof(void *) < 0x90000,
               "task pool must fit the native heap arena");
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
JP_EXPORT void jp_task_runtime_get_layout(struct jp_task_runtime_layout *out)
{
    const struct jp_task_runtime_layout layout = {
        1, sizeof(void *), sizeof(Uint32),
        sizeof(struct task), sizeof(struct task_work),
        sizeof(struct task_motion), sizeof(struct task_force),
        sizeof(struct event_work), sizeof(struct collision_work),
        sizeof(struct event_motion), sizeof(struct event_path_entry),
        sizeof(struct task_spec), 4 * sizeof(void *),
        OFFSET(task_work, vars), OFFSET(task_work, ang), OFFSET(task_work, pos),
        OFFSET(task_work, collision), OFFSET(task_work, event),
        OFFSET(event_work, motions), OFFSET(event_work, previous),
        OFFSET(event_work, paths), OFFSET(event_motion, next),
        OFFSET(event_path_entry, next), OFFSET(collision_work, flags),
        OFFSET(collision_work, shapes)
    };
    *out = layout;
}
