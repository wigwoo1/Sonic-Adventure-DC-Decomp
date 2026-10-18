#ifndef SA_ENGINE_RENDER_BUFFER_READY_QUERY_H
#define SA_ENGINE_RENDER_BUFFER_READY_QUERY_H
#include "fpu_cleanup_transfer.h"
#include "sa_engine_render_buffer_dispatch.h"

SA_STAGE_EXPORT int sa_engine_render_buffer_ready_query(
    uint8_t *, struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *,
    uint32_t *, uint32_t *, void *, sa_render_dispatch_call_fn);
#endif
