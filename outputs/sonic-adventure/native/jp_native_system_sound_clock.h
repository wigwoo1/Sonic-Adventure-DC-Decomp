#ifndef JP_NATIVE_SYSTEM_SOUND_CLOCK_H
#define JP_NATIVE_SYSTEM_SOUND_CLOCK_H
#ifndef JP_SYSTEM_SOUND_CLOCK_API
#define JP_SYSTEM_SOUND_CLOCK_API __declspec(dllexport)
#endif
struct jp_sound_options;struct jp_sound_clock_options;
/* Production timed sound startup through the qualified System/exit owner.
 * The older explicit-progress entry remains available for offline/research
 * clients. A failed clock start rolls back the acquired sound/core owners.
 */
JP_SYSTEM_SOUND_CLOCK_API int jp_system_start_sound_clock(const struct jp_sound_options *,const struct jp_sound_clock_options *);
#endif
