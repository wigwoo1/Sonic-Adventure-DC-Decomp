#ifndef JP_NATIVE_GAME_SOUND_H
#define JP_NATIVE_GAME_SOUND_H
#include <stdint.h>
#include "jp_native_aica.h"
#ifndef JP_GAME_SOUND_API
#define JP_GAME_SOUND_API __declspec(dllexport)
#endif
enum jp_sound_error {JP_SOUND_OK,JP_SOUND_ARGUMENT,JP_SOUND_STATE,JP_SOUND_THREAD,
    JP_SOUND_REENTRANT,JP_SOUND_DEPENDENCY,JP_SOUND_AICA,JP_SOUND_G2,
    JP_SOUND_FILE,JP_SOUND_MEMORY,JP_SOUND_DRIVER,JP_SOUND_SDK,
    JP_SOUND_DISPATCH,JP_SOUND_CLEANUP,JP_SOUND_CONFIG,JP_SOUND_QUEUE};
enum jp_sound_phase {JP_SOUND_COLD,JP_SOUND_CONFIGURED,JP_SOUND_STARTING,
    JP_SOUND_READY,JP_SOUND_CLOSING,JP_SOUND_CLOSED,JP_SOUND_FAILED};
enum jp_sound_resource {JP_SOUND_OWNS_AICA=1,JP_SOUND_OWNS_G2=2,
    JP_SOUND_OWNS_FILE=4,JP_SOUND_OWNS_STAGE=8,JP_SOUND_OWNS_SDK=16,
    JP_SOUND_OWNS_DISPATCH=32};
struct jp_sound_options {
    uint32_t bytes,sound_mode,max_driver_bytes,close_timeout_ms;
    struct jp_aica_options aica;
    const char *driver_name;
};
struct jp_sound_info {
    uint64_t stage_address,stage_capacity,stage_reservation_bytes,dispatch_token,
        initializations,shutdowns,driver_bytes,config_sessions;
    uint32_t phase,status,resources,failed_resource,native_error,creator_thread,
        sound_mode,config_active;
    int32_t sdk_error;uint32_t cleanup_error;
};
/* Creator-thread owner for the actual original game's initialization closure.
 * Borrows an initialized GDFS and native wait; owns SDK, guarded sector-rounded
 * driver storage, G2, AICA/output and its real frame-service binding. It cannot
 * adopt existing owners. Names and settings are copied at configure time.
 * Mode 0=stereo,1=mono. The driver cap must be 32..2097152 bytes. NULL name uses
 * masaru.drv. AICA flags=0 is explicit offline mode, WITH_OUTPUT uses real PCM.
 * Close this owner before borrowed System/GDFS/wait shutdown. Failed cleanup
 * retains its unfinished resources for another shutdown on the same thread.
 * This does not yet own the global early/final exit or the full game queue.
 */
JP_GAME_SOUND_API int jp_sound_configure(const struct jp_sound_options *);
JP_GAME_SOUND_API int jp_sound_initialize(void);
JP_GAME_SOUND_API int jp_sound_shutdown(uint32_t timeout_ms);
JP_GAME_SOUND_API int jp_sound_snapshot(struct jp_sound_info *);
/* Explicit sound progression until a sustained native scheduler is qualified.
 * Service runs the genuine SDK server. Render/pump advances the real firmware;
 * service alone does not advance emulated time. No fake game queue is bound.
 * Pump's JP_SOUND_QUEUE is temporary backpressure; no firmware time advances
 * and the owner stays healthy. Retry after the real output worker frees space.
 */
JP_GAME_SOUND_API int jp_sound_service(void);
JP_GAME_SOUND_API int jp_sound_render(uint32_t,int16_t *,uint64_t);
JP_GAME_SOUND_API int jp_sound_pump(uint32_t);
#endif
