#ifndef SA_ENGINE_CALLBACK_FLAG_RESET_H
#define SA_ENGINE_CALLBACK_FLAG_RESET_H
#include <stdint.h>
#ifdef _WIN32
#define SA_ENGINE_FLAG_EXPORT __declspec(dllexport)
#else
#define SA_ENGINE_FLAG_EXPORT
#endif

/* Native recovery of 1ST_READ.BIN routine 0x8c651368. */
SA_ENGINE_FLAG_EXPORT void sa_engine_callback_flag_reset_8c651368(
    uint8_t *ram, uint32_t *sr, uint32_t *r, uint32_t *pr);

#endif
