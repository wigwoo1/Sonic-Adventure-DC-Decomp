#ifndef SA_MAIN_READY_FLAG_H
#define SA_MAIN_READY_FLAG_H
#include <stdint.h>
#ifdef _WIN32
#define SA_MAIN_READY_EXPORT __declspec(dllexport)
#else
#define SA_MAIN_READY_EXPORT
#endif

/* Native recovery of 1ST_READ.BIN routine 0x8c04c680. */
SA_MAIN_READY_EXPORT void sa_main_ready_flag_8c04c680(uint8_t *ram, uint32_t *r);

#endif
