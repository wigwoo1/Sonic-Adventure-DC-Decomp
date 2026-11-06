#ifndef JP_NATIVE_EFFECTS_H
#define JP_NATIVE_EFFECTS_H
#include <stdint.h>
#ifndef JP_EFFECTS_API
#define JP_EFFECTS_API __declspec(dllexport)
#endif
/* Same creator thread, clock, MIDI ports and output as jp_sound_*. Call the
 * queue once per logical game update; the real frame-wait dispatcher does so.
 * Loading borrows one filename for the call and owns aligned staging until
 * actual GDFS and G2 completion. No retail data is compiled into this module.
 */
struct jp_effects_request {
    uint32_t bytes,kind; /* once, at, held, for, at-for, around */
    int64_t id;
    /* IDs above 1274 follow the original voice-offset rule. They require a
     * ready game stream and must resolve inside its actual loaded archive. */
    int32_t se,priority,volume,updates;
    float x,y,z,radius;
};
struct jp_effects_info {
    uint64_t queue_calls,requests,loads,loaded_bytes,stage_address,stage_capacity;
    uint32_t live_slots,muted,retained_stage,status;
    int32_t sdk_error,native_error;
};
JP_EFFECTS_API int jp_effects_request(const struct jp_effects_request *,int32_t *slot);
/* kind: 0 MLT, 1 shot, 2 sequence, 3 program. Bank is ignored for MLT. */
JP_EFFECTS_API int jp_effects_load_file(const char *name,uint32_t kind,int32_t bank);
JP_EFFECTS_API int jp_effects_queue(void);
/* 0 stop-se, 1 stop-id, 2 volume, 3 pan, 4 pitch, 5 mute, 6 unmute,
 * 7 kill-all, 8 request-fx. IDs keep native pointer width. */
JP_EFFECTS_API int jp_effects_control(uint32_t action,int64_t id,int32_t value);
JP_EFFECTS_API int jp_effects_snapshot(struct jp_effects_info *);
#endif
