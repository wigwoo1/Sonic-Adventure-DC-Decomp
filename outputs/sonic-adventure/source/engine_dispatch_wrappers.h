#ifndef SA_ENGINE_DISPATCH_WRAPPERS_H
#define SA_ENGINE_DISPATCH_WRAPPERS_H
#include "fpu_cleanup_transfer.h"
#ifdef _WIN32
#define SA_ENGINE_DISPATCH_EXPORT __declspec(dllexport)
#else
#define SA_ENGINE_DISPATCH_EXPORT
#endif
typedef int (*sa_engine_dispatch_call)(void *, uint32_t, uint8_t *,
    struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *);

SA_ENGINE_DISPATCH_EXPORT int sa_engine_dispatch_40_8c652f56(
    uint8_t *, struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *,
    uint32_t *, uint32_t *, void *, sa_engine_dispatch_call);
SA_ENGINE_DISPATCH_EXPORT int sa_engine_dispatch_b0_8c652d6e(
    uint8_t *, struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *,
    uint32_t *, uint32_t *, void *, sa_engine_dispatch_call);
SA_ENGINE_DISPATCH_EXPORT int sa_engine_dispatch_b8_8c652e38(
    uint8_t *, struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *,
    uint32_t *, uint32_t *, void *, sa_engine_dispatch_call);
SA_ENGINE_DISPATCH_EXPORT int sa_engine_dispatch_pair_8c666968(
    uint8_t *, struct sa_fpu_banks *, uint32_t *, uint32_t *, uint32_t *,
    uint32_t *, uint32_t *, void *, sa_engine_dispatch_call);
#endif
