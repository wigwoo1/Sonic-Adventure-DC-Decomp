/* Native storage/file boundary for the original demo recorder. One game-thread
 * owner; initialize before use. These services do not run recording_update,
 * alter game globals, or implement general debug-host/stage services. */
#ifndef JP_NATIVE_RECORDING_H
#define JP_NATIVE_RECORDING_H
#include <stddef.h>
#include <stdint.h>
#define JP_RECORDING_API __declspec(dllexport)
#define JP_RECORDING_RECORD_LAST_OFFSET 0x1ffe8u
#define JP_RECORDING_PLAY_LAST_OFFSET 0x180000u
#define JP_RECORDING_MAX_RECORD_BYTES 131064u
#define JP_RECORDING_MAX_PLAY_BYTES 1572876u
#define JP_RECORDING_STORAGE_SAMPLES 131074u
enum jp_recording_error {JP_RECORDING_OK,JP_RECORDING_ARGUMENT,
    JP_RECORDING_STATE,JP_RECORDING_THREAD,JP_RECORDING_IO,
    JP_RECORDING_FORMAT,JP_RECORDING_BOUNDS};
struct jp_recording_info {
    uint64_t save_count,debug_count;
    uint32_t status,os_error,loaded_bytes,saved_bytes,legacy_cursor;
};
JP_RECORDING_API int jp_recording_initialize(const wchar_t *existing_save_root);
JP_RECORDING_API int jp_recording_shutdown(void);
JP_RECORDING_API int jp_recording_status(void);
JP_RECORDING_API void jp_recording_clear_error(void);
JP_RECORDING_API int jp_recording_snapshot(struct jp_recording_info *);
JP_RECORDING_API int jp_recording_begin_record(void);
/* A readable caller-owned byte range containing whole 12-byte little-endian
 * samples and an aligned end marker. Trailing whole samples are preserved;
 * playback stops at the first marker, just as the original does. */
JP_RECORDING_API int jp_recording_load_memory(const void *,size_t);
JP_RECORDING_API int jp_recording_load_file(const wchar_t *);
JP_RECORDING_API void *jp_recording_buffer(void);
JP_RECORDING_API size_t jp_recording_storage_bytes(void);
/* mode uses the original enum: record=0, play=1, done=2. Validates without
 * dereferencing the cursor. A failed guard must stop the caller's update. */
JP_RECORDING_API int jp_recording_check_cursor(const void *,int mode);
/* Restricted replacement for recorder.c's six host_save_file calls. The
 * original ignores the return; the host must check the sticky status. */
JP_RECORDING_API int jp_recording_save(const char *,const void *,int);
/* Actual native debugger output; reports the original guest cursor rather
 * than truncating a 64-bit host address. */
JP_RECORDING_API int jp_recording_debug_cursor(const void *);
#endif
