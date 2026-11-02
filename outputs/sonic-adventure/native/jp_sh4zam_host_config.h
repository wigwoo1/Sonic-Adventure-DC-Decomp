/* Authored configuration for the pinned SH4ZAM dependency on Clang/Windows C17.
 * The game thread owns matrix state. Audio/IO workers must not call this API.
 * Use the dependency checkout directly; no third-party routines are vendored.
 */
#ifndef JP_SH4ZAM_HOST_CONFIG_H
#define JP_SH4ZAM_HOST_CONFIG_H
#if !defined(__clang__) || !defined(_WIN32) || defined(__cplusplus)
#error "This configuration is qualified for Windows Clang C only"
#endif
/* SH4ZAM's documented disabled-TLS model gives one shared game-thread state.
 * It does not require pthreads, C11 TLS runtime support or false SH-4 defines. */
#define SHZ_TLS_MODEL 0
#include <sh4zam/shz_cdefs.h>
/* The pinned MSVC branch spells these as C++ constexpr. Clang accepts the
 * corresponding function attributes in C17, as its GNU branch does. */
#undef SHZ_CONST
#define SHZ_CONST __attribute__((const))
#undef SHZ_PURE
#define SHZ_PURE __attribute__((pure))
_Static_assert(SHZ_TLS_MODEL == SHZ_TLS_DISABLED, "single game-thread state");
_Static_assert(SHZ_BACKEND == SHZ_SW, "native portable backend, never SH-4 asm");
#endif
