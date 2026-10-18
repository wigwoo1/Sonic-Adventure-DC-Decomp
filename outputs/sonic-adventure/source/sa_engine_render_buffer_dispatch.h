#ifndef SA_ENGINE_RENDER_BUFFER_DISPATCH_H
#define SA_ENGINE_RENDER_BUFFER_DISPATCH_H
#include "fpu_cleanup_transfer.h"

typedef int (*sa_render_dispatch_call_fn)(
    void *, uint32_t, uint8_t *, struct sa_fpu_banks *, uint32_t *,
    uint32_t *, uint32_t *);

SA_STAGE_EXPORT int sa_engine_render_buffer_dispatch(
    uint8_t *, struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *,
    uint32_t *, uint32_t *, void *, sa_render_dispatch_call_fn);
#endif
