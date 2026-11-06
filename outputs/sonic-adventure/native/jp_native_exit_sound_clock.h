#ifndef JP_NATIVE_EXIT_SOUND_CLOCK_H
#define JP_NATIVE_EXIT_SOUND_CLOCK_H
#include <stdint.h>
#ifndef JP_EXIT_SOUND_CLOCK_API
#define JP_EXIT_SOUND_CLOCK_API __declspec(dllexport)
#endif
/* Guard actual sound work against nested global cleanup before mutation, both
 * before and after final ownership transfer. Exact live sound token required.
 * This does not claim/close another owner or invoke a callback.
 */
JP_EXIT_SOUND_CLOCK_API int jp_exit_hold_sound_activity(uint64_t token,uint32_t held);
#endif
