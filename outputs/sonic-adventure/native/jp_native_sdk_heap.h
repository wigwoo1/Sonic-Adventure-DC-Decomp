#ifndef JP_NATIVE_SDK_HEAP_H
#define JP_NATIVE_SDK_HEAP_H
#include <stdint.h>
#include <stddef.h>
#ifndef JP_SDK_HEAP_API
#define JP_SDK_HEAP_API __declspec(dllexport)
#endif
enum jp_sdk_heap_error {JP_SDK_HEAP_OK,JP_SDK_HEAP_ARGUMENT,
    JP_SDK_HEAP_NOT_INITIALIZED,JP_SDK_HEAP_STATE,JP_SDK_HEAP_ACTIVE,
    JP_SDK_HEAP_MEMORY,JP_SDK_HEAP_EXHAUSTED,JP_SDK_HEAP_OVERFLOW,
    JP_SDK_HEAP_POINTER,JP_SDK_HEAP_CORRUPT,JP_SDK_HEAP_THREAD};
struct jp_sdk_heap_info {
    uint64_t arena,base,reservation;
    uint32_t span,usable,units,live,requested,capacity,free_bytes,largest_free;
    uint32_t owned,status,unit_bytes,header_bytes,initialized,reserved;
};
/* One actual SDK arena; unrelated native CRT/task/texture allocators remain
 * separate. Borrowed arena must be writable for span bytes and stay alive
 * through shutdown. Both modes use the first 32-byte boundary strictly after
 * the supplied start, subtracting that prefix from the usable extent. Bounds:
 * 64 bytes through 64MiB. Owned mode adds inaccessible guard pages.
 * Lifecycle belongs to its creating thread; allocation/free/stat are protected
 * by a real native lock and may be used by other threads. */
JP_SDK_HEAP_API int jp_sdk_heap_initialize(void *arena,uint32_t span);
JP_SDK_HEAP_API int jp_sdk_heap_create(uint32_t span);
/* Refuses live allocations, preserving the arena for a retry after free. */
JP_SDK_HEAP_API int jp_sdk_heap_shutdown(void);
JP_SDK_HEAP_API void *jp_sdk_heap_allocate(size_t bytes);
JP_SDK_HEAP_API void *jp_sdk_heap_callocate(size_t count,size_t bytes);
/* Safe native repair: NULL acts as allocate; non-NULL size 0 frees. A failed
 * resize preserves the old pointer/data. In-place shrink keeps its capacity.
 * The unsafe original SDK realloc body is not compiled/executed. */
JP_SDK_HEAP_API void *jp_sdk_heap_reallocate(void *pointer,size_t bytes);
JP_SDK_HEAP_API int jp_sdk_heap_free(void *pointer);
JP_SDK_HEAP_API int jp_sdk_heap_statistics(uint32_t *total,uint32_t *largest);
/* Snapshot preserves its output on error. Status reports the last operation;
 * snapshots do not clear it. Header/link corruption is terminal until teardown
 * without live allocations. Output records must be outside the arena. */
JP_SDK_HEAP_API int jp_sdk_heap_snapshot(struct jp_sdk_heap_info *);
JP_SDK_HEAP_API int jp_sdk_heap_status(void);
/* One terminal exit owner, on the creator thread. Allocation/free remain
 * available until all consumers retire. Ordinary close/reinitialize cannot
 * discard a delegated arena. Cancel is only for an uncommitted bind before
 * the first finish attempt; cancelled tokens are never recycled. */
JP_SDK_HEAP_API int jp_sdk_heap_exit_claim(uint64_t *token);
JP_SDK_HEAP_API int jp_sdk_heap_exit_cancel(uint64_t token);
JP_SDK_HEAP_API int jp_sdk_heap_exit_finish(uint64_t token);
#endif
