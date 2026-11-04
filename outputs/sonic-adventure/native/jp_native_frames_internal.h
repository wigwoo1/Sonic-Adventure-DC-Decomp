#ifndef JP_NATIVE_FRAMES_INTERNAL_H
#define JP_NATIVE_FRAMES_INTERNAL_H
#include "jp_native_frames.h"
/* Only the real queue and texture owner translation units use this boundary. */
int jp_frame_queue_idle(void);
int jp_frame_queue_empty(void);
int jp_frames_report_error(int code);
int jp_frames_record_begin(int textured,uint32_t word);
void jp_frames_record_commit(void);
void jp_frames_record_abort(void);
void jp_frames_record_discard(void);
int jp_frames_capture_lists(const void *const data[5],const size_t sizes[5],uint64_t *id);
int jp_frames_defer_surface_free(void *pixels);
int jp_texture_live_surface_view(uint32_t word,const void **pixels,size_t *bytes);
#endif
