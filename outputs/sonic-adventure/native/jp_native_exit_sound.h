#ifndef JP_NATIVE_EXIT_SOUND_H
#define JP_NATIVE_EXIT_SOUND_H
#include "jp_native_exit.h"
#ifndef JP_EXIT_SOUND_API
#define JP_EXIT_SOUND_API __declspec(dllexport)
#endif
/* Preserve every existing mask and the observation-only bootstrap bit. */
#define JP_EXIT_SOUND UINT32_C(524288)
#define JP_EXIT_AUDIO_SUPPORTED (JP_EXIT_SUPPORTED|JP_EXIT_SOUND)
#define JP_EXIT_WITH_DEVICE_SDK_HEAP_RTC_GDFS_SOUND (JP_EXIT_WITH_DEVICE_SDK_HEAP_RTC_GDFS|JP_EXIT_SOUND)
#define JP_EXIT_SOUND_STATE 11
/* Extend only the exact existing bootstrap callback/context with an already
 * claimed live sound token. Final bind may adopt that same authority; failed
 * validation/other claims must leave it with the bootstrap. No callback runs
 * here. The token remains owned by the registering System until final bind.
 * Token0 reserves the required sound resource before its startup; a later
 * call attaches its actual claimed token. Final binding can never omit it,
 * including after a partial startup/cleanup failure.
 */
JP_EXIT_SOUND_API int jp_exit_attach_bootstrap_sound(jp_exit_bootstrap_cleanup,
    void *context,uint64_t sound_token,uint32_t timeout_ms);
/* Temporarily guard the exact registration while its System performs local
 * startup/ordinary cleanup. Nested global bind/cleanup rejects before marking
 * terminal exit started. Cancel of that same registration clears the guard.
 */
JP_EXIT_SOUND_API int jp_exit_hold_bootstrap(jp_exit_bootstrap_cleanup,
    void *context,uint32_t held);
#endif
