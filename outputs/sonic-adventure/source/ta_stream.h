#ifndef SA_TA_STREAM_H
#define SA_TA_STREAM_H
#include "frame_command_bus.h"
#include <stddef.h>
/* Portable owned storage. A page is a bounded 64-packet chunk, not a frame limit.
 * The allocator must return suitably aligned, non-overlapping owned allocations.
 * No callback executes game code or produces game/device results. */
struct sa_ta_stream;
typedef void *(*sa_ta_allocate)(void *,size_t);
typedef void (*sa_ta_release)(void *,void *);
SA_STAGE_EXPORT struct sa_ta_stream *sa_ta_stream_create(sa_ta_allocate,sa_ta_release,void *);
SA_STAGE_EXPORT struct sa_ta_stream *sa_ta_stream_create_win32(void);
SA_STAGE_EXPORT void sa_ta_stream_destroy(struct sa_ta_stream *);
SA_STAGE_EXPORT int sa_ta_stream_bind(struct sa_ta_stream *,struct sa_vertex_memory *,const struct sa_vertex_bus *,struct sa_vertex_bus *,uint32_t *,const uint32_t *,const uint64_t *);
SA_STAGE_EXPORT int sa_ta_stream_append(struct sa_ta_stream *,const struct sa_frame_command_packet *);
/* Snapshot is an independent deep copy. Allocation failure preserves its old
 * contents and cursor. Retiring a list never changes a submitted snapshot. */
SA_STAGE_EXPORT int sa_ta_stream_snapshot(struct sa_ta_stream *);
SA_STAGE_EXPORT uint32_t sa_ta_stream_count(const struct sa_ta_stream *,unsigned);
SA_STAGE_EXPORT uint32_t sa_ta_stream_pages(const struct sa_ta_stream *,unsigned);
SA_STAGE_EXPORT int sa_ta_stream_packet(const struct sa_ta_stream *,unsigned,uint32_t,struct sa_frame_command_packet *);
SA_STAGE_EXPORT int sa_ta_stream_events(const struct sa_ta_stream *,uint32_t *);
SA_STAGE_EXPORT int sa_ta_stream_lifecycle(const struct sa_ta_stream *,uint64_t *);
#endif
