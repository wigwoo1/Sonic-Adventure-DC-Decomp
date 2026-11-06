#ifndef JP_NATIVE_READ_QUEUE_H
#define JP_NATIVE_READ_QUEUE_H
#include <stdint.h>
#include <stddef.h>
#ifndef JP_READ_QUEUE_API
#define JP_READ_QUEUE_API __declspec(dllexport)
#endif
enum jp_read_queue_error {JP_READ_QUEUE_OK,JP_READ_QUEUE_ARGUMENT,JP_READ_QUEUE_STATE,
    JP_READ_QUEUE_THREAD,JP_READ_QUEUE_REENTRANT,JP_READ_QUEUE_GDFS,JP_READ_QUEUE_SOURCE,
    JP_READ_QUEUE_FULL,JP_READ_QUEUE_BUSY,JP_READ_QUEUE_BUFFER,JP_READ_QUEUE_IDENTITY};
struct jp_read_queue_info {
    uint64_t starts,adds,completed,cancels,lease,active_handle;
    uint32_t phase,status;int32_t source_error;
    uint32_t queued,current,creator_thread,initialized,reserved;
};
struct jp_read_queue_slot {
    char name[32];uint64_t destination,capacity,file_bytes;
    int32_t state;uint32_t index,fad,reserved;
};
/* Actual original 16-slot loader queue over already initialized native GDFS.
 * The caller keeps each 32-byte-aligned destination and its sector-rounded
 * capacity alive until its slot becomes FREE or cancellation succeeds.
 * Overlapping live destinations are rejected. A failed read is explicit and
 * retains its slot/handle until successful cancel/shutdown. This service holds
 * GDFS against independent destruction, but does not own RTC/System/Exit.
 * Close this queue before closing those owners. No user callback is installed
 * or replaced by this component. All calls use the creating game thread.
 */
JP_READ_QUEUE_API int jp_read_queue_initialize(void);
JP_READ_QUEUE_API int jp_read_queue_add(const char *,void *,size_t);
JP_READ_QUEUE_API int jp_read_queue_serve(void);
JP_READ_QUEUE_API int jp_read_queue_cancel(void);
JP_READ_QUEUE_API int jp_read_queue_shutdown(void);
JP_READ_QUEUE_API int jp_read_queue_snapshot(struct jp_read_queue_info *);
JP_READ_QUEUE_API int jp_read_queue_slot_snapshot(uint32_t,struct jp_read_queue_slot *);
#endif
