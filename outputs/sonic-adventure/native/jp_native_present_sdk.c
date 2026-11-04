#include <kamui.h>
#include "jp_native_present.h"
int jp_present_initialize_sdk(struct jp_gpu *gpu,struct jp_window *window,
    const struct jp_gpu_config *settings,uint32_t timeout_ms,unsigned interval)
{return jp_present_initialize(gpu,window,&kmiDeviceExtension,settings,timeout_ms,interval);}
