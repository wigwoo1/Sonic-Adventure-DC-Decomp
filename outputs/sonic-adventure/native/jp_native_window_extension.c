/* Appended to the frozen authored GPU source by build_native_window.py.
 * Shares its one device/context. Does not replace a game producer or emulate
 * Kamui render/flip scheduling. Requires the real stateful frame owner.
 */
#include <dxgi.h>
#include "jp_native_window.h"
struct jp_window {
    struct jp_gpu *gpu;HWND handle;IDXGISwapChain *swap;
    unsigned width,height,closed;DWORD thread;uint64_t presented;
};
int jp_gpu_draw_bound_frame(struct jp_gpu *g,uint64_t frame,const struct jp_gpu_config *settings,
    void *rgba,size_t bytes,struct jp_gpu_report *report,struct jp_gpu_failure *failure)
{
    if(!g||g->closing||!settings||!rgba||!report)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    struct jp_video_snapshot s;
    if(jp_frame_video(frame,&s))return fail(failure,JP_GPU_PACKETS,0,E_INVALIDARG);
    if(!s.presort||s.clamp_min||s.clamp_max!=UINT32_MAX||s.background_texture||s.background_bytes||
       !s.logical_width||!s.logical_height || s.logical_width>8192 || s.logical_height>8192 ||
       (s.global_clip && (!s.global_clip_width||!s.global_clip_height)))return fail(failure,JP_GPU_STATE,0,E_INVALIDARG);
    uint32_t palette[1024];if(jp_video_palette_rgba(&s,palette))return fail(failure,JP_GPU_STATE,0,E_INVALIDARG);
    size_t overhead=sizeof s+sizeof palette;
    if(overhead>settings->budget)return fail(failure,JP_GPU_BUDGET,0,E_OUTOFMEMORY);
    struct jp_gpu_config config=*settings;config.budget-=overhead;config.logical_width=(float)s.logical_width;config.logical_height=(float)s.logical_height;
    config.palette=palette;config.palette_entries=1024;config.stride_pixels=(s.texture_control&31)*32;
    uint32_t c=s.clear_argb;config.clear_rgba[0]=(float)((c>>16)&255)/255;config.clear_rgba[1]=(float)((c>>8)&255)/255;config.clear_rgba[2]=(float)(c&255)/255;config.clear_rgba[3]=(float)(c>>24)/255;
    config.punch_alpha=(float)((s.fb_write_control>>16)&255)/255;memcpy(&config.cull_small_area,&s.cull_bits,4);
    g->snapshot=&s;int result=jp_gpu_draw_frame(g,frame,&config,rgba,bytes,report,failure);g->snapshot=NULL;
    if(!result)report->accounted_payload_bytes+=overhead;return result;
}
static LRESULT CALLBACK window_proc(HWND hwnd,UINT message,WPARAM wp,LPARAM lp)
{
    struct jp_window *w=(struct jp_window *)GetWindowLongPtrW(hwnd,GWLP_USERDATA);
    if(message==WM_NCCREATE){CREATESTRUCTW *create=(CREATESTRUCTW *)lp;w=create->lpCreateParams;SetWindowLongPtrW(hwnd,GWLP_USERDATA,(LONG_PTR)w);}
    if(w && message==WM_CLOSE){w->closed=1;DestroyWindow(hwnd);return 0;}
    if(w && message==WM_NCDESTROY){w->closed=1;w->handle=NULL;SetWindowLongPtrW(hwnd,GWLP_USERDATA,0);}
    return DefWindowProcW(hwnd,message,wp,lp);
}
static int valid_window(struct jp_window *w)
{return w && w->gpu && !w->gpu->closing && !w->closed && w->handle && w->thread==GetCurrentThreadId();}
int jp_window_open(struct jp_gpu *g,unsigned width,unsigned height,int visible,struct jp_window **out,struct jp_gpu_failure *failure)
{
    if(!g||g->closing||!out||!width||!height||width>8192||height>8192||(visible!=0 && visible!=1))return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    struct jp_window *w=calloc(1,sizeof *w);if(!w)return fail(failure,JP_GPU_ALLOCATION,0,E_OUTOFMEMORY);
    static const wchar_t class_name[]=L"SA_Native_Renderer_Research_Window";
    WNDCLASSEXW wc={0};wc.cbSize=sizeof wc;wc.lpfnWndProc=window_proc;wc.hInstance=GetModuleHandleW(NULL);wc.lpszClassName=class_name;
    if(!RegisterClassExW(&wc) && GetLastError()!=ERROR_CLASS_ALREADY_EXISTS){free(w);return fail(failure,JP_GPU_API,0,HRESULT_FROM_WIN32(GetLastError()));}
    RECT rect={0,0,(LONG)width,(LONG)height};AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,FALSE);w->gpu=g;w->thread=GetCurrentThreadId();
    w->handle=CreateWindowExW(0,class_name,L"Sonic Adventure native renderer research - not playable",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,rect.right-rect.left,rect.bottom-rect.top,NULL,NULL,wc.hInstance,w);
    if(!w->handle){free(w);return fail(failure,JP_GPU_API,0,HRESULT_FROM_WIN32(GetLastError()));}
    IDXGIDevice *device=NULL;IDXGIAdapter *adapter=NULL;IDXGIFactory *factory=NULL;HRESULT hr;
    hr=ID3D11Device_QueryInterface(g->device,&IID_IDXGIDevice,(void **)&device);if(FAILED(hr))goto done;
    hr=IDXGIDevice_GetAdapter(device,&adapter);if(FAILED(hr))goto done;
    hr=IDXGIAdapter_GetParent(adapter,&IID_IDXGIFactory,(void **)&factory);if(FAILED(hr))goto done;
    DXGI_SWAP_CHAIN_DESC desc={0};desc.BufferDesc.Width=width;desc.BufferDesc.Height=height;desc.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count=1;desc.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;desc.BufferCount=2;desc.OutputWindow=w->handle;desc.Windowed=TRUE;desc.SwapEffect=DXGI_SWAP_EFFECT_FLIP_DISCARD;
    hr=IDXGIFactory_CreateSwapChain(factory,(IUnknown *)g->device,&desc,&w->swap);if(FAILED(hr))goto done;
    hr=IDXGIFactory_MakeWindowAssociation(factory,w->handle,DXGI_MWA_NO_ALT_ENTER);if(FAILED(hr))goto done;
    w->width=width;w->height=height;++g->window_refs;
    if(visible)ShowWindow(w->handle,SW_SHOWNOACTIVATE);
done:
    if(factory)IDXGIFactory_Release(factory);if(adapter)IDXGIAdapter_Release(adapter);if(device)IDXGIDevice_Release(device);
    if(FAILED(hr)){if(w->swap)IDXGISwapChain_Release(w->swap);if(w->handle)DestroyWindow(w->handle);free(w);return fail(failure,JP_GPU_API,0,hr);}
    *out=w;return fail(failure,JP_GPU_OK,0,S_OK);
}
void jp_window_close(struct jp_window *w)
{
    if(!w || w->thread!=GetCurrentThreadId())return;
    struct jp_gpu *g=w->gpu;if(w->swap)IDXGISwapChain_Release(w->swap);if(w->handle)DestroyWindow(w->handle);free(w);
    if(g && --g->window_refs==0 && g->closing)jp_gpu_close(g);
}
int jp_window_pump(struct jp_window *w)
{
    if(!w || w->thread!=GetCurrentThreadId())return 0;
    MSG message;while(PeekMessageW(&message,NULL,0,0,PM_REMOVE)){TranslateMessage(&message);DispatchMessageW(&message);}
    return valid_window(w);
}
int jp_window_resize(struct jp_window *w,unsigned width,unsigned height,struct jp_gpu_failure *failure)
{
    if(!valid_window(w)||!width||!height||width>8192||height>8192)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    RECT r={0,0,(LONG)width,(LONG)height};AdjustWindowRect(&r,WS_OVERLAPPEDWINDOW,FALSE);
    if(!SetWindowPos(w->handle,NULL,0,0,r.right-r.left,r.bottom-r.top,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE))return fail(failure,JP_GPU_API,0,HRESULT_FROM_WIN32(GetLastError()));
    HRESULT hr=IDXGISwapChain_ResizeBuffers(w->swap,2,width,height,DXGI_FORMAT_R8G8B8A8_UNORM,0);
    if(FAILED(hr))return fail(failure,JP_GPU_API,0,hr);
    w->width=width;w->height=height;return fail(failure,JP_GPU_OK,0,S_OK);
}
int jp_window_size(struct jp_window *w,unsigned *width,unsigned *height)
{if(!valid_window(w)||!width||!height)return JP_GPU_ARGUMENT;*width=w->width;*height=w->height;return JP_GPU_OK;}
int jp_window_upload(struct jp_window *w,const void *rgba,size_t bytes,struct jp_gpu_failure *failure)
{
    if(!valid_window(w)||!rgba||bytes<(size_t)w->width*w->height*4)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    ID3D11Texture2D *back=NULL;HRESULT hr=IDXGISwapChain_GetBuffer(w->swap,0,&IID_ID3D11Texture2D,(void **)&back);if(FAILED(hr))return fail(failure,JP_GPU_API,0,hr);
    ID3D11DeviceContext_UpdateSubresource(w->gpu->context,(ID3D11Resource *)back,0,NULL,rgba,w->width*4,0);ID3D11Texture2D_Release(back);
    hr=ID3D11Device_GetDeviceRemovedReason(w->gpu->device);return fail(failure,FAILED(hr)?JP_GPU_API:JP_GPU_OK,0,hr);
}
int jp_window_readback(struct jp_window *w,void *rgba,size_t bytes,struct jp_gpu_failure *failure)
{
    if(!valid_window(w)||!rgba||bytes<(size_t)w->width*w->height*4)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    ID3D11Texture2D *back=NULL,*staging=NULL;HRESULT hr=IDXGISwapChain_GetBuffer(w->swap,0,&IID_ID3D11Texture2D,(void **)&back);if(FAILED(hr))return fail(failure,JP_GPU_API,0,hr);
    D3D11_TEXTURE2D_DESC desc;ID3D11Texture2D_GetDesc(back,&desc);desc.Usage=D3D11_USAGE_STAGING;desc.BindFlags=desc.MiscFlags=0;desc.CPUAccessFlags=D3D11_CPU_ACCESS_READ;
    hr=ID3D11Device_CreateTexture2D(w->gpu->device,&desc,NULL,&staging);if(FAILED(hr))goto done;
    ID3D11DeviceContext_CopyResource(w->gpu->context,(ID3D11Resource *)staging,(ID3D11Resource *)back);D3D11_MAPPED_SUBRESOURCE mapped;
    hr=ID3D11DeviceContext_Map(w->gpu->context,(ID3D11Resource *)staging,0,D3D11_MAP_READ,0,&mapped);if(FAILED(hr))goto done;
    for(unsigned y=0;y<w->height;y++)memcpy((unsigned char *)rgba+(size_t)y*w->width*4,(unsigned char *)mapped.pData+(size_t)y*mapped.RowPitch,(size_t)w->width*4);
    ID3D11DeviceContext_Unmap(w->gpu->context,(ID3D11Resource *)staging,0);
done:
    if(staging)ID3D11Texture2D_Release(staging);ID3D11Texture2D_Release(back);return fail(failure,FAILED(hr)?JP_GPU_API:JP_GPU_OK,0,hr);
}
int jp_window_present(struct jp_window *w,unsigned interval,struct jp_gpu_failure *failure)
{
    if(!valid_window(w)||interval>1)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    HRESULT hr=IDXGISwapChain_Present(w->swap,interval,0);
    if(hr==DXGI_STATUS_OCCLUDED)return fail(failure,JP_GPU_STATE,0,hr);
    if(FAILED(hr))return fail(failure,JP_GPU_API,0,hr);
    ++w->presented;return fail(failure,JP_GPU_OK,0,S_OK);
}
uint64_t jp_window_presented(struct jp_window *w){return w?w->presented:0;}
int jp_window_draw_frame(struct jp_window *w,uint64_t frame,const struct jp_gpu_config *settings,unsigned interval,struct jp_gpu_report *out,struct jp_gpu_failure *failure)
{
    if(!valid_window(w)||!settings||!out||interval>1)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    size_t bytes=(size_t)w->width*w->height*4;
    if(bytes>settings->budget)return fail(failure,JP_GPU_BUDGET,0,E_OUTOFMEMORY);
    void *pixels=malloc(bytes);if(!pixels)return fail(failure,JP_GPU_ALLOCATION,0,E_OUTOFMEMORY);
    struct jp_gpu_config config=*settings;config.width=w->width;config.height=w->height;config.budget-=bytes;struct jp_gpu_report report;
    int code=jp_gpu_draw_bound_frame(w->gpu,frame,&config,pixels,bytes,&report,failure);
    if(!code)code=jp_window_upload(w,pixels,bytes,failure);
    if(!code)code=jp_window_present(w,interval,failure);
    if(!code){report.accounted_payload_bytes+=bytes;*out=report;}
    free(pixels);return code;
}
