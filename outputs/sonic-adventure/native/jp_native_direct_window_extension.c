/* Appended to the checked authored GPU/window translation unit. */
#include "jp_native_direct_window.h"
static int window_render_target(struct jp_window *w,uint64_t frame,
    const struct jp_gpu_config *settings,struct jp_gpu_report *out,struct jp_gpu_failure *failure,int bound)
{
    if(!valid_window(w)||!settings||!out)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    ID3D11Texture2D *back=NULL;
    HRESULT hr=IDXGISwapChain_GetBuffer(w->swap,0,&IID_ID3D11Texture2D,(void **)&back);
    if(FAILED(hr))return fail(failure,JP_GPU_API,0,hr);
    struct jp_gpu_config config=*settings;config.width=w->width;config.height=w->height;
    int code=bound?gpu_draw_bound_target(w->gpu,frame,&config,NULL,0,out,failure,back):
        gpu_render_frame(w->gpu,frame,&config,NULL,0,out,failure,back);
    ID3D11Texture2D_Release(back);return code;
}
int jp_window_render_frame(struct jp_window *w,uint64_t frame,
    const struct jp_gpu_config *settings,struct jp_gpu_report *out,struct jp_gpu_failure *failure)
{return window_render_target(w,frame,settings,out,failure,1);}
int jp_window_render_configured_frame(struct jp_window *w,uint64_t frame,
    const struct jp_gpu_config *settings,struct jp_gpu_report *out,struct jp_gpu_failure *failure)
{return window_render_target(w,frame,settings,out,failure,0);}
int jp_window_draw_direct_frame(struct jp_window *w,uint64_t frame,
    const struct jp_gpu_config *settings,unsigned interval,
    struct jp_gpu_report *out,struct jp_gpu_failure *failure)
{
    if(!valid_window(w)||!settings||!out||interval>1)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    struct jp_gpu_report report;
    int code=jp_window_render_frame(w,frame,settings,&report,failure);
    if(!code)code=jp_window_present(w,interval,failure);
    if(!code)*out=report;
    return code;
}
