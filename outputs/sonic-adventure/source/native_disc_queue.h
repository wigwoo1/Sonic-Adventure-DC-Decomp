#ifndef SA_NATIVE_DISC_QUEUE_H
#define SA_NATIVE_DISC_QUEUE_H
#include <stdint.h>
#ifdef _WIN32
#define SA_DISC_EXPORT __declspec(dllexport)
#else
#define SA_DISC_EXPORT
#endif
typedef int (*sa_disc_read_at)(void *,uint64_t,uint8_t *,uint32_t);
enum { SA_DISC_FREE,SA_DISC_QUEUED,SA_DISC_COMPLETE,SA_DISC_CANCELLED,SA_DISC_FAILED };
struct sa_disc_job { uint32_t handle,state,lba,count,completed,kind;int32_t error;uint8_t *output;uint64_t capacity; };
struct sa_disc_queue {
    sa_disc_read_at read;void *user;
    uint64_t file_offset,track_sectors;
    uint32_t track_lba,stride,next_handle,cursor,busy;
    struct sa_disc_job jobs[8];
};
/* New host platform API, not a firmware ABI. Single-threaded, non-reentrant.
 * Output storage is borrowed until release; completed sectors survive cancel.
 * Each step reads at most one sector, round-robin across live requests.
 * Terminal slots stay owned until release. Handles never wrap within a session.
 * All functions return 0 on host success, negative on host error. */
SA_DISC_EXPORT uint32_t sa_disc_queue_size(void);
SA_DISC_EXPORT int sa_disc_queue_init(struct sa_disc_queue *,sa_disc_read_at,void *,uint64_t,uint64_t,uint32_t,uint32_t);
SA_DISC_EXPORT int sa_disc_queue_submit(struct sa_disc_queue *,uint32_t,uint32_t,uint8_t *,uint64_t,uint32_t *);
SA_DISC_EXPORT int sa_disc_queue_step(struct sa_disc_queue *);
SA_DISC_EXPORT int sa_disc_queue_status(struct sa_disc_queue *,uint32_t,uint32_t *,uint32_t *,int32_t *);
SA_DISC_EXPORT int sa_disc_queue_cancel(struct sa_disc_queue *,uint32_t);
SA_DISC_EXPORT int sa_disc_queue_release(struct sa_disc_queue *,uint32_t);
SA_DISC_EXPORT int sa_disc_submit_read_command(struct sa_disc_queue *,uint8_t *,uint32_t,uint32_t,uint32_t *);
SA_DISC_EXPORT int sa_disc_service(struct sa_disc_queue *,uint8_t *,uint32_t,uint32_t,uint32_t,uint32_t *);
SA_DISC_EXPORT int sa_disc_read_guest_sectors(struct sa_disc_queue *,uint8_t *,uint32_t,uint32_t,uint32_t);
SA_DISC_EXPORT int sa_disc_queue_submit_version(struct sa_disc_queue *,uint8_t *,uint64_t,uint32_t *);
SA_DISC_EXPORT int sa_disc_queue_transfer_bytes(struct sa_disc_queue *,uint32_t,uint32_t *);
#endif
