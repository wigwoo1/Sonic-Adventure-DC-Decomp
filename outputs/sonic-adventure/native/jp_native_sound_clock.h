#ifndef JP_NATIVE_SOUND_CLOCK_H
#define JP_NATIVE_SOUND_CLOCK_H
#include <stdint.h>
#ifndef JP_SOUND_CLOCK_API
#define JP_SOUND_CLOCK_API __declspec(dllexport)
#endif
typedef int (*jp_sound_clock_source)(void *,uint64_t *);
typedef int (*jp_sound_clock_activity)(void *,uint32_t entering);
enum jp_sound_clock_error {JP_SOUND_CLOCK_OK,JP_SOUND_CLOCK_ARGUMENT,
    JP_SOUND_CLOCK_STATE,JP_SOUND_CLOCK_THREAD,JP_SOUND_CLOCK_REENTRANT,
    JP_SOUND_CLOCK_SOURCE,JP_SOUND_CLOCK_BACKWARDS,JP_SOUND_CLOCK_OVERFLOW,
    JP_SOUND_CLOCK_OUTPUT,JP_SOUND_CLOCK_SDK,JP_SOUND_CLOCK_WAIT,JP_SOUND_CLOCK_PENDING};
enum jp_sound_clock_phase {JP_SOUND_CLOCK_COLD,JP_SOUND_CLOCK_ACTIVE,
    JP_SOUND_CLOCK_STOPPED,JP_SOUND_CLOCK_FAILED};
struct jp_sound_clock_options {
    uint32_t bytes,quantum_frames,max_frames_per_tick,period_ms;
    jp_sound_clock_source source;void *context;uint64_t frequency;
    jp_sound_clock_activity activity;void *activity_context;
};
struct jp_sound_clock_info {
    uint64_t anchor,last_tick,frequency,fraction,due_frames,rendered_frames,
        backlog,service_calls,render_calls,queue_full,wait_token,initial_core_frames,
        cancelled_frames;
    uint32_t phase,status,native_error,creator_thread,quantum_frames,
        max_frames_per_tick,period_ms,in_tick;
};
/* Requires actual ready sound with Windows output. NULL source/context/frequency
 * borrows the existing native wall clock at781250Hz. An explicit source is a
 * borrowed deterministic clock provider, not a replacement SDK/firmware/audio.
 * Its frequency must be nonzero and <=UINT64_MAX/44101. Copy all options.
 * Quantum1..2048, per-tick budget quantum..8192, service period1..10ms.
 * Integer fractional accumulation targets44100Hz independently of render FPS;
 * bounded work and actual output-full preserve debt. Tick success can retain
 * backlog: query info. Driver boot frames precede the new real-time anchor.
 * Explicit sound render/pump reject while the clock owns firmware progression.
 * Optional activity/context are borrowed guards around ticks and explicit stop;
 * the caller guards initial start. System's timed entry supplies its actual
 * lifecycle guard; callers of that entry must leave these two fields NULL.
 */
JP_SOUND_CLOCK_API int jp_sound_clock_start(const struct jp_sound_clock_options *);
JP_SOUND_CLOCK_API int jp_sound_clock_tick(void);
/* cancel_pending0 rejects outstanding debt. 1 explicitly cancels it without
 * advancing firmware or pretending it played. Sound cleanup uses this mode
 * before retiring callback/SDK/core memory. Failures retain actual ownership.
 */
JP_SOUND_CLOCK_API int jp_sound_clock_stop(uint32_t cancel_pending);
JP_SOUND_CLOCK_API int jp_sound_clock_snapshot(struct jp_sound_clock_info *);
#endif
