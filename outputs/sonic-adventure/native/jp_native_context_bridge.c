#include "jp_native_context_bridge.h"
#include "jp_native_frames_internal.h"
#include "jp_native_video_state.h"
#include <stdint.h>
#include <string.h>
extern KMVERTEXCONTEXT _nj_context_[NJD_CONTEXT_MAX];
extern KMVERTEXCONTEXT *_nj_current_ctx_;
extern Uint32 _nj_ctx_color_mode_[NJD_CONTEXT_MAX];
/* The queue already owns regular contexts; this is the original SDK's
 * separate background context, not another queue or device owner. */
__declspec(dllexport) KMVERTEXCONTEXT _nj_background_ctx_;
extern void jp_context_make_defaults(KMVERTEXCONTEXT *,Uint32 *,KMVERTEXCONTEXT *);
extern void njSetCurrentContext(Int);
static int index_for(const KMVERTEXCONTEXT *context)
{
    uintptr_t p=(uintptr_t)context,start=(uintptr_t)_nj_context_;size_t bytes=sizeof _nj_context_;
    if(p<start||p-start>=bytes||(p-start)%sizeof *context)return -1;
    return (int)((p-start)/sizeof *context);
}
int jp_context_update_guard(KMVERTEXCONTEXT *context)
{
    if(!context)return 1;
    int index=index_for(context);
    if(index<0)return 0; /* Caller-owned metadata has no queue side effects. */
    /* This idempotent owner operation validates active/dirty/ready state. */
    int current=index_for(_nj_current_ctx_);
    if(current<0)return 1;
    return jp_queue_select_context((unsigned)current)!=JP_QUEUE_OK;
}
KMSTATUS kmSetVertexRenderState(PKMVERTEXCONTEXT context)
{
    if(!context)return KMSTATUS_INVALID_PARAMETER;
    int current=index_for(_nj_current_ctx_);
    if(current<0||jp_queue_select_context((unsigned)current))return KMSTATUS_INVALID_PARAMETER;
    uint32_t words[4]={context->GLOBALPARAMBUFFER,context->ISPPARAMBUFFER,context->TSPPARAMBUFFER,context->TexturePARAMBUFFER};
    return jp_queue_set_context((unsigned)current,words)?KMSTATUS_INVALID_PARAMETER:KMSTATUS_SUCCESS;
}
int jp_ninja_contexts_initialize(void)
{
    if(jp_queue_status()!=JP_QUEUE_OK||jp_video_status()!=JP_VIDEO_OK||!jp_frame_queue_empty())return JP_QUEUE_STATE;
    KMVERTEXCONTEXT contexts[NJD_CONTEXT_MAX],background;Uint32 colors[NJD_CONTEXT_MAX];
    memcpy(contexts,_nj_context_,sizeof contexts);memcpy(colors,_nj_ctx_color_mode_,sizeof colors);background=_nj_background_ctx_;
    jp_context_make_defaults(contexts,colors,&background);
    memcpy(_nj_context_,contexts,sizeof contexts);memcpy(_nj_ctx_color_mode_,colors,sizeof colors);_nj_background_ctx_=background;
    njSetCurrentContext(0);return jp_queue_status();
}
