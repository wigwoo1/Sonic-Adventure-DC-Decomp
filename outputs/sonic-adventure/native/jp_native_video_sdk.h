/* Typed SDK bridge initialization; SDK setting functions use pinned kamui.h. */
#ifndef JP_NATIVE_VIDEO_SDK_H
#define JP_NATIVE_VIDEO_SDK_H
#include "jp_native_video_state.h"
JP_PACKET_API int jp_video_sdk_initialize(unsigned width,unsigned height,
    uint32_t clear_argb,int presort);
#endif
