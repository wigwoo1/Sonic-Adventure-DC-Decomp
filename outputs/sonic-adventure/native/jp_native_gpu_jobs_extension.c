/* Appended to checked authored renderer; original game bodies stay external. */
#include "jp_native_gpu_jobs.h"
struct jp_gpu_job {
    ID3D11Texture2D *target;
    ID3D11Query *begin,*end,*disjoint,*complete;
    struct jp_gpu_timing timing;
    unsigned ready;HRESULT error;
};
static volatile LONG64 job_sequence;
static uint64_t new_job_id(void)
{
    LONG64 old=InterlockedCompareExchange64(&job_sequence,0,0);
    for(;;){if(old==INT64_MAX)return 0;LONG64 found=InterlockedCompareExchange64(&job_sequence,old+1,old);if(found==old)return (uint64_t)old+1;old=found;}
}
static int job_thread(struct jp_gpu *g){return g && g->owner_thread==GetCurrentThreadId();}
static struct jp_gpu_job *find_job(struct jp_gpu *g,uint64_t id)
{for(unsigned i=0;i<4;i++)if(g->jobs[i] && g->jobs[i]->timing.id==id)return g->jobs[i];return NULL;}
static void free_job(struct jp_gpu_job *j)
{
    if(j->target)ID3D11Texture2D_Release(j->target);
    if(j->begin)ID3D11Query_Release(j->begin);if(j->end)ID3D11Query_Release(j->end);
    if(j->disjoint)ID3D11Query_Release(j->disjoint);if(j->complete)ID3D11Query_Release(j->complete);free(j);
}
size_t jp_gpu_timing_size(void){return sizeof(struct jp_gpu_timing);}
int jp_gpu_elapsed_counts(uint64_t begin,uint64_t end,uint64_t frequency,uint64_t *out)
{
    const uint64_t rate=781250;
    if(!out||end<begin||!frequency||frequency>UINT64_MAX/(rate+1))return JP_GPU_ARGUMENT;
    uint64_t delta=end-begin,whole=delta/frequency,part=delta%frequency;
    if(whole>UINT64_MAX/rate)return JP_GPU_ARGUMENT;
    whole*=rate;part=part*rate/frequency;if(whole>UINT64_MAX-part)return JP_GPU_ARGUMENT;
    *out=whole+part;return JP_GPU_OK;
}
static int submit_gpu_frame(struct jp_gpu *g,uint64_t frame,const struct jp_gpu_config *settings,
    uint64_t *out,struct jp_gpu_report *report,struct jp_gpu_failure *failure,int bound)
{
    if(!job_thread(g)||g->closing||!settings||!out||!report||!settings->width||!settings->height||settings->width>8192||settings->height>8192)
        return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    unsigned slot=0;while(slot<4 && g->jobs[slot])++slot;
    if(slot==4)return fail(failure,JP_GPU_JOB_FULL,0,E_PENDING);
    if(settings->budget<sizeof(struct jp_gpu_job))return fail(failure,JP_GPU_BUDGET,0,E_OUTOFMEMORY);
    LARGE_INTEGER frequency,begin,submitted;
    if(!QueryPerformanceFrequency(&frequency)||frequency.QuadPart<=0||!QueryPerformanceCounter(&begin)||begin.QuadPart<0)
        return fail(failure,JP_GPU_API,0,E_FAIL);
    struct jp_gpu_job *j=calloc(1,sizeof *j);if(!j)return fail(failure,JP_GPU_ALLOCATION,0,E_OUTOFMEMORY);
    D3D11_TEXTURE2D_DESC desc={0};desc.Width=settings->width;desc.Height=settings->height;desc.MipLevels=desc.ArraySize=1;
    desc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;desc.SampleDesc.Count=1;desc.Usage=D3D11_USAGE_DEFAULT;desc.BindFlags=D3D11_BIND_RENDER_TARGET;
    HRESULT hr=ID3D11Device_CreateTexture2D(g->device,&desc,NULL,&j->target);int code=JP_GPU_API;
    if(FAILED(hr))goto error;
    D3D11_QUERY_DESC query={D3D11_QUERY_TIMESTAMP,0};
    hr=ID3D11Device_CreateQuery(g->device,&query,&j->begin);if(FAILED(hr))goto error;
    hr=ID3D11Device_CreateQuery(g->device,&query,&j->end);if(FAILED(hr))goto error;
    query.Query=D3D11_QUERY_TIMESTAMP_DISJOINT;hr=ID3D11Device_CreateQuery(g->device,&query,&j->disjoint);if(FAILED(hr))goto error;
    query.Query=D3D11_QUERY_EVENT;hr=ID3D11Device_CreateQuery(g->device,&query,&j->complete);if(FAILED(hr))goto error;
    uint64_t id=new_job_id();if(!id){code=JP_GPU_STATE;hr=E_FAIL;goto error;}
    struct jp_gpu_config config=*settings;config.budget-=sizeof *j;struct jp_gpu_report result;
    g->measure_begin=j->begin;g->measure_end=j->end;g->measure_disjoint=j->disjoint;g->measure_prepared.QuadPart=-1;
    code=bound?gpu_draw_bound_target(g,frame,&config,NULL,0,&result,failure,j->target):
        gpu_render_frame(g,frame,&config,NULL,0,&result,failure,j->target);
    g->measure_begin=g->measure_end=g->measure_disjoint=NULL;
    if(code){free_job(j);return code;}
    code=JP_GPU_API;
    ID3D11DeviceContext_End(g->context,(ID3D11Asynchronous *)j->complete);
    ID3D11DeviceContext_Flush(g->context);
    hr=ID3D11Device_GetDeviceRemovedReason(g->device);if(FAILED(hr))goto error;
    if(!QueryPerformanceCounter(&submitted)||g->measure_prepared.QuadPart<begin.QuadPart||submitted.QuadPart<g->measure_prepared.QuadPart){hr=E_FAIL;goto error;}
    j->timing.id=id;j->timing.frame=frame;j->timing.cpu_frequency=(uint64_t)frequency.QuadPart;
    j->timing.cpu_begin=(uint64_t)begin.QuadPart;j->timing.cpu_prepared=(uint64_t)g->measure_prepared.QuadPart;j->timing.cpu_submitted=(uint64_t)submitted.QuadPart;
    j->timing.width=settings->width;j->timing.height=settings->height;g->jobs[slot]=j;++g->job_refs;
    result.accounted_payload_bytes+=sizeof *j;*report=result;*out=id;return fail(failure,JP_GPU_OK,0,S_OK);
error:
    free_job(j);return fail(failure,code,0,hr);
}
int jp_gpu_submit_frame(struct jp_gpu *g,uint64_t frame,const struct jp_gpu_config *settings,
    uint64_t *out,struct jp_gpu_report *report,struct jp_gpu_failure *failure)
{return submit_gpu_frame(g,frame,settings,out,report,failure,1);}
int jp_gpu_submit_configured_frame(struct jp_gpu *g,uint64_t frame,const struct jp_gpu_config *settings,
    uint64_t *out,struct jp_gpu_report *report,struct jp_gpu_failure *failure)
{return submit_gpu_frame(g,frame,settings,out,report,failure,0);}
int jp_gpu_job_poll(struct jp_gpu *g,uint64_t id,struct jp_gpu_timing *out,struct jp_gpu_failure *failure)
{
    if(!job_thread(g)||!out)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    struct jp_gpu_job *j=find_job(g,id);if(!j)return fail(failure,JP_GPU_JOB_STALE,0,E_INVALIDARG);
    if(FAILED(j->error))return fail(failure,JP_GPU_API,0,j->error);
    if(j->ready){*out=j->timing;return fail(failure,JP_GPU_OK,0,S_OK);}
    BOOL complete=FALSE;D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjoint={0};uint64_t begin=0,end=0;
    HRESULT hr=ID3D11DeviceContext_GetData(g->context,(ID3D11Asynchronous *)j->complete,&complete,sizeof complete,D3D11_ASYNC_GETDATA_DONOTFLUSH);
    if(FAILED(hr))goto error;
    if(hr==S_FALSE||!complete)goto pending;
    hr=ID3D11DeviceContext_GetData(g->context,(ID3D11Asynchronous *)j->disjoint,&disjoint,sizeof disjoint,D3D11_ASYNC_GETDATA_DONOTFLUSH);if(FAILED(hr))goto error;if(hr==S_FALSE)goto pending;
    hr=ID3D11DeviceContext_GetData(g->context,(ID3D11Asynchronous *)j->begin,&begin,sizeof begin,D3D11_ASYNC_GETDATA_DONOTFLUSH);if(FAILED(hr))goto error;if(hr==S_FALSE)goto pending;
    hr=ID3D11DeviceContext_GetData(g->context,(ID3D11Asynchronous *)j->end,&end,sizeof end,D3D11_ASYNC_GETDATA_DONOTFLUSH);if(FAILED(hr))goto error;if(hr==S_FALSE)goto pending;
    LARGE_INTEGER observed;if(!QueryPerformanceCounter(&observed)||observed.QuadPart<0||(uint64_t)observed.QuadPart<j->timing.cpu_submitted){hr=E_FAIL;goto error;}
    j->timing.cpu_completed_observed=(uint64_t)observed.QuadPart;j->timing.gpu_begin=begin;j->timing.gpu_end=end;j->timing.gpu_frequency=disjoint.Frequency;
    j->timing.gpu_timing_valid=!disjoint.Disjoint && jp_gpu_elapsed_counts(begin,end,disjoint.Frequency,&j->timing.gpu_elapsed_counts)==JP_GPU_OK;
    j->ready=1;*out=j->timing;return fail(failure,JP_GPU_OK,0,S_OK);
pending:
    hr=ID3D11Device_GetDeviceRemovedReason(g->device);if(FAILED(hr))goto error;
    return fail(failure,JP_GPU_JOB_PENDING,0,S_FALSE);
error:
    j->error=hr;return fail(failure,JP_GPU_API,0,hr);
}
int jp_gpu_job_release(struct jp_gpu *g,uint64_t id,struct jp_gpu_failure *failure)
{
    if(!job_thread(g))return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    unsigned slot=0;while(slot<4 && (!g->jobs[slot]||g->jobs[slot]->timing.id!=id))++slot;
    if(slot==4)return fail(failure,JP_GPU_JOB_STALE,0,E_INVALIDARG);
    struct jp_gpu_job *j=g->jobs[slot];struct jp_gpu_timing t;
    int code=jp_gpu_job_poll(g,id,&t,failure);if(code==JP_GPU_JOB_PENDING)return code;
    if(code && !FAILED(j->error))return code;
    g->jobs[slot]=NULL;--g->job_refs;free_job(j);
    if(g->closing && !g->job_refs && !g->window_refs)jp_gpu_close(g);
    return fail(failure,JP_GPU_OK,0,S_OK);
}
unsigned jp_gpu_jobs_live(struct jp_gpu *g){return job_thread(g)?g->job_refs:0;}
int jp_window_copy_job(struct jp_window *w,uint64_t id,struct jp_gpu_failure *failure)
{
    if(!valid_window(w)||!job_thread(w->gpu))return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    struct jp_gpu_timing t;int code=jp_gpu_job_poll(w->gpu,id,&t,failure);if(code)return code;
    if(t.width!=w->width||t.height!=w->height)return fail(failure,JP_GPU_STATE,0,E_INVALIDARG);
    ID3D11Texture2D *back=NULL;HRESULT hr=IDXGISwapChain_GetBuffer(w->swap,0,&IID_ID3D11Texture2D,(void **)&back);if(FAILED(hr))return fail(failure,JP_GPU_API,0,hr);
    struct jp_gpu_job *j=find_job(w->gpu,id);ID3D11DeviceContext_CopyResource(w->gpu->context,(ID3D11Resource *)back,(ID3D11Resource *)j->target);
    ID3D11Texture2D_Release(back);hr=ID3D11Device_GetDeviceRemovedReason(w->gpu->device);return fail(failure,FAILED(hr)?JP_GPU_API:JP_GPU_OK,0,hr);
}
int jp_window_present_job(struct jp_window *w,uint64_t id,unsigned interval,struct jp_gpu_failure *failure)
{
    if(interval>1)return fail(failure,JP_GPU_ARGUMENT,0,E_INVALIDARG);
    int code=jp_window_copy_job(w,id,failure);if(!code)code=jp_window_present(w,interval,failure);return code;
}
