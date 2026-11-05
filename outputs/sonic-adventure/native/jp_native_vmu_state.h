#ifndef JP_NATIVE_VMU_STATE_H
#define JP_NATIVE_VMU_STATE_H
#include <stdint.h>
#include <stddef.h>
struct jp_vmu_state_info { int32_t game_mode,skip,updates,request;
    uint32_t screen_bytes,animation_frame_bytes; };
__declspec(dllexport) int jp_vmu_state_read(struct jp_vmu_state_info *out);
__declspec(dllexport) void *jp_vmu_screen(uint32_t port);
#endif
