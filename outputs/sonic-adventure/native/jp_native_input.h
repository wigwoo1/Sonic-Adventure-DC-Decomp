/* One game-thread peripheral owner. Callbacks/context must outlive initialization.
 * The stock source polls focused Windows keyboard/XInput. A supplied source is
 * an explicit alternate provider, useful for deterministic input/logic tests. */
#ifndef JP_NATIVE_INPUT_H
#define JP_NATIVE_INPUT_H
#include <stdint.h>
#include <stddef.h>
#define JP_INPUT_API __declspec(dllexport)
enum jp_input_error {JP_INPUT_OK,JP_INPUT_ARGUMENT,JP_INPUT_STATE,JP_INPUT_THREAD,
    JP_INPUT_NOT_INITIALIZED,JP_INPUT_SOURCE,JP_INPUT_UNAVAILABLE,JP_INPUT_REENTRANT};
enum jp_input_flags {JP_INPUT_KEYBOARD=1,JP_INPUT_CONTROLLERS=2};
/* Axes use original Dreamcast units [-128,127], triggers [0,255]. Button
 * values are PDD_DGT_* masks from the pinned external SDK. */
struct jp_input_sample {uint32_t connected,buttons;int32_t x1,y1,x2,y2;uint32_t left,right;};
struct jp_input_xinput {uint16_t buttons;uint8_t left,right;int16_t lx,ly,rx,ry;};
typedef int (*jp_input_source)(void *,struct jp_input_sample samples[4]);
struct jp_input_info {
    uint64_t polls;
    uint32_t status,source_kind,focused,connected_mask,xinput_available,os_error,record_bytes;
};
JP_INPUT_API int jp_input_initialize(void *window,uint32_t flags,jp_input_source source,void *context);
JP_INPUT_API int jp_input_shutdown(void);
JP_INPUT_API int jp_input_poll(void);
JP_INPUT_API int jp_input_status(void);
JP_INPUT_API void jp_input_clear_error(void);
JP_INPUT_API int jp_input_snapshot(struct jp_input_info *out);
JP_INPUT_API size_t jp_input_info_size(void);
JP_INPUT_API size_t jp_input_record_size(void);
JP_INPUT_API const void *jp_input_record(uint32_t slot);
/* Pure conversions used by the real Windows collector, independently testable
 * without sending keyboard events or requiring a physical controller. */
JP_INPUT_API int jp_input_map_xinput(const struct jp_input_xinput *,struct jp_input_sample *);
JP_INPUT_API int jp_input_apply_keys(const uint8_t keys[256],struct jp_input_sample *);
#endif
