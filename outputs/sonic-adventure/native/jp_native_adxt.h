#ifndef JP_NATIVE_ADXT_H
#define JP_NATIVE_ADXT_H
#include <stdint.h>
#ifndef JP_ADXT_API
#define JP_ADXT_API __declspec(dllexport)
#endif
struct jp_adxt_info {
    uint64_t initializations,shutdowns,service_calls,owned_bytes,last_core_frame,next_core_frame;
    uint32_t active,stopping,live_players,owned_work,status;
    int32_t native_error;
};
/* Uses the existing sound/System owner, original ADXT records and actual AICA
 * frame count. One logical server step per 735 frames (60 Hz), independent of
 * presentation FPS. No extra output device, timer, worker or vblank handler.
 * Zero work_bytes chooses the original music/voice allocation size. Work is
 * owned until successful destruction; failed cancellation retains it.
 * Raw SDK entry points remain internal, with their original caller contracts.
 */
JP_ADXT_API int jp_adxt_initialize(void);
JP_ADXT_API int jp_adxt_shutdown(void);
JP_ADXT_API int jp_adxt_service(void);
JP_ADXT_API int jp_adxt_snapshot(struct jp_adxt_info *);
JP_ADXT_API int jp_adxt_create(uint32_t channels,uint32_t work_bytes,void **);
JP_ADXT_API int jp_adxt_destroy(void *);
JP_ADXT_API int jp_adxt_stop(void *);
JP_ADXT_API int jp_adxt_start_fname(void *,const char *);
JP_ADXT_API int jp_adxt_start_afs(void *,uint32_t partition,uint32_t file);
#endif
