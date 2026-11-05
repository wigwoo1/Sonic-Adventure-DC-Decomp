#ifndef JP_NATIVE_AICA_H
#define JP_NATIVE_AICA_H
#include <stdint.h>
#ifndef JP_AICA_API
#define JP_AICA_API __declspec(dllexport)
#endif
enum jp_aica_error {JP_AICA_OK,JP_AICA_ARGUMENT,JP_AICA_STATE,JP_AICA_THREAD,
    JP_AICA_MEMORY,JP_AICA_CORE,JP_AICA_OUTPUT,JP_AICA_TIMEOUT,JP_AICA_QUEUE};
enum jp_aica_phase {JP_AICA_COLD,JP_AICA_OPENING,JP_AICA_READY,
    JP_AICA_CLOSING,JP_AICA_CLOSED,JP_AICA_FAILED};
enum {JP_AICA_WITH_OUTPUT=1,JP_AICA_RAM_BYTES=2<<20,JP_AICA_MAX_FRAMES=8192};
struct jp_aica_options {
    uint32_t bytes,flags,capacity_frames,latency_ms,timeout_ms,muted;
    float gain;uint32_t reserved;
};
struct jp_aica_info {
    uint64_t state_address,state_bytes,frames,cycles,read_bytes,write_bytes,
        submitted,render_calls,resets;
    uint32_t phase,status,native_error,resources,arm_reset,creator_thread,
        output_enabled,reserved;
};
/* Single creator-thread core owner. RAM is exactly 2 MiB; host pointers are
 * native width. Opens with the ARM held in reset. CPU reset preserves RAM and
 * AICA/DSP/timer state; full close/open creates a fresh chip/session.
 * WITH_OUTPUT owns an actual 44100 Hz Windows PCM worker; flags=0 explicitly
 * selects offline rendering. A pre-existing audio worker cannot be adopted.
 * Failure/timeout may retain resources: inspect and retry close on this owner.
 * This interface does not pretend to initialize the game's original sound SDK.
 */
JP_AICA_API int jp_aica_open(const struct jp_aica_options *);
JP_AICA_API int jp_aica_read(uint32_t address,void *out,uint32_t bytes);
JP_AICA_API int jp_aica_write(uint32_t address,const void *data,uint32_t bytes);
JP_AICA_API int jp_aica_read_register(uint32_t offset,uint32_t *out);
JP_AICA_API int jp_aica_write_register(uint32_t offset,uint32_t value);
JP_AICA_API int jp_aica_set_arm_reset(uint32_t held);
/* Render returns copied interleaved signed16 stereo PCM. Pump instead sends
 * generated PCM to the owned real audio stream. Queue-full pump never advances
 * the emulated core. Device/core failure is explicit and stops further work.
 * ARM-held rendering advances the sound chip/timers without executing the CPU.
 * Render/pump limits are 1..8192 frames; output bytes must cover frames*4.
 */
JP_AICA_API int jp_aica_render(uint32_t frames,int16_t *out,uint64_t out_bytes);
JP_AICA_API int jp_aica_pump(uint32_t frames);
JP_AICA_API int jp_aica_drain(uint32_t timeout_ms);
/* Close cancels queued audio using its actual bounded join; explicit drain is
 * separate. A timeout retains the core/audio until the same close is retried.
 */
JP_AICA_API int jp_aica_close(uint32_t timeout_ms);
JP_AICA_API int jp_aica_status(void);
JP_AICA_API int jp_aica_snapshot(struct jp_aica_info *);
#endif
