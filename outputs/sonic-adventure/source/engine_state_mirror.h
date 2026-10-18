#ifndef SA_ENGINE_STATE_MIRROR_H
#define SA_ENGINE_STATE_MIRROR_H
#include <stdint.h>
#ifdef _WIN32
#define SA_ENGINE_STATE_EXPORT __declspec(dllexport)
#else
#define SA_ENGINE_STATE_EXPORT
#endif

/* Native recovery of 1ST_READ.BIN routine 0x8c107ac6. */
SA_ENGINE_STATE_EXPORT void sa_engine_state_mirror_8c107ac6(
    uint8_t *ram, uint32_t *sr, uint32_t *r);

#endif
