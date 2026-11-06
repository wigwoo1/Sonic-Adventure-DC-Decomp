#ifndef JP_NATIVE_SYSTEM_SOUND_H
#define JP_NATIVE_SYSTEM_SOUND_H
#include <stdint.h>
#ifndef JP_SYSTEM_SOUND_API
#define JP_SYSTEM_SOUND_API __declspec(dllexport)
#endif
struct jp_sound_options;
#define JP_SYSTEM_SOUND 13
/* After real System initialization, configure/start the actual original game
 * sound owner and extend that System's existing early-exit registration. A
 * startup failure closes acquired audio before the borrowed core services.
 * Existing independently owned sound is rejected before adoption. Ordinary
 * shutdown releases the closed claim for restart; early/final exit is terminal.
 * The options/name are copied by the sound owner. This does not start the full
 * game queue or sustained firmware scheduler or export a fake parent init.
 */
JP_SYSTEM_SOUND_API int jp_system_start_sound(const struct jp_sound_options *);
#endif
