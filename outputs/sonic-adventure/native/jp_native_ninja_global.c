#include <ninja.h>
#include <kamui.h>
#include "jp_native_ninja_global.h"
#include "jp_native_context_bridge.h"
#include "jp_native_frames_internal.h"
#include "jp_native_video_state.h"
extern Float _nj_cnk_easy_multilight_buffer_dst_[32];
extern KMVERTEXCONTEXT _nj_context_[NJD_CONTEXT_MAX];
extern void jp_original_ninja_global(void);
static int result=JP_NINJA_GLOBAL_NOT_READY;
int jp_ninja_global_status(void) {return result;}
int jp_ninja_global_snapshot(struct jp_ninja_global_info *out) {
    if(!out)return JP_NINJA_GLOBAL_ARGUMENT;
    *out=(struct jp_ninja_global_info){NJD_LIGHT_MAX,sizeof(_nj_light_material_),
        sizeof(_nj_light_control_),sizeof(_nj_light_queue_),
        sizeof(_nj_cnk_easy_multilight_buffer_),sizeof(_nj_cnk_easy_multilight_buffer_dst_),
        sizeof(void *),(uint32_t)result};return 0;
}
int jp_ninja_global_initialize(void) {
    struct jp_video_snapshot video;
    if(jp_queue_status() || jp_video_status() || jp_frames_status())
        return result=JP_NINJA_GLOBAL_NOT_READY;
    if(!jp_frame_queue_empty() || jp_frames_live_count())
        return result=JP_NINJA_GLOBAL_ACTIVE;
    if(jp_video_capture(&video) || !video.logical_width || !video.logical_height)
        return result=JP_NINJA_GLOBAL_PROVIDER;
    if(jp_queue_select_context(0))return result=JP_NINJA_GLOBAL_PROVIDER;
    /* Existing native context guards need an actual current slot before
     * initializing the owned context array. No header/vertices are emitted.
     * The complete original initializer then sets all contexts and slot 0. */
    _nj_current_ctx_=&_nj_context_[0];
    _nj_screen_.w=(Float)video.logical_width;
    _nj_screen_.h=(Float)video.logical_height;
    jp_original_ninja_global();
    return result=(jp_queue_status() || jp_video_status()) ?
        JP_NINJA_GLOBAL_PROVIDER : JP_NINJA_GLOBAL_OK;
}
void njInitGlobal(void) {(void)jp_ninja_global_initialize();}
