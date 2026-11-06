#ifndef JP_NATIVE_MAIN_BOOT_H
#define JP_NATIVE_MAIN_BOOT_H
#include <stdint.h>
#include "jp_native_game_sound.h"
#include "jp_native_sound_clock.h"
#ifndef JP_MAIN_BOOT_API
#define JP_MAIN_BOOT_API __declspec(dllexport)
#endif
enum jp_main_boot_error {JP_MAIN_BOOT_OK,JP_MAIN_BOOT_ARGUMENT,JP_MAIN_BOOT_STATE,
    JP_MAIN_BOOT_THREAD,JP_MAIN_BOOT_REENTRANT,JP_MAIN_BOOT_SOUND,
    JP_MAIN_BOOT_RENDER,JP_MAIN_BOOT_CARD,JP_MAIN_BOOT_CLEANUP};
struct jp_main_boot_info {
    uint64_t starts,completions,shutdowns;
    uint32_t phase,status,native_error,creator_thread,sound_calls,card_owned;
};
/* Configure System with its existing API first. Borrow initialized empty native
 * queue/frame/texture/render owners, registered original texture_buffer, and a
 * real VMU media store. All original high-level initialization runs in C.
 * Copy settings/name; explicit clock callbacks/context remain borrowed.
 * Call shutdown before closing the borrowed store/LCD/render owners. This API
 * does not yet attach the new card backend to terminal global Exit ownership.
 * Do not bind terminal Exit for this full parent until that integration exists.
 */
JP_MAIN_BOOT_API int jp_main_boot_configure(const struct jp_sound_options *,const struct jp_sound_clock_options *);
JP_MAIN_BOOT_API int jp_main_boot_initialize(void);
JP_MAIN_BOOT_API int jp_main_boot_shutdown(uint32_t timeout_ms);
JP_MAIN_BOOT_API int jp_main_boot_snapshot(struct jp_main_boot_info *);
/* Original ABI. A failed whole initialization exits nonzero, never falling
 * through to game_init with a partially initialized system. */
JP_MAIN_BOOT_API void system_boot_init(void);
#endif
