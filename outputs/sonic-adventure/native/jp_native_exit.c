#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include "jp_native_exit.h"
/* Explicit imports avoid exporting another copy of the shared native owners.
 * The build workflow independently checks these signatures against headers. */
#define IMPORT_INT(n) __declspec(dllimport) int n(void)
#define IMPORT_VOID(n) __declspec(dllimport) void n(void)
IMPORT_INT(jp_present_shutdown);IMPORT_INT(jp_input_shutdown);
IMPORT_INT(jp_recording_shutdown);IMPORT_INT(jp_wait_shutdown);
IMPORT_INT(jp_assets_release_data);IMPORT_INT(jp_assets_recover_pending);
IMPORT_INT(jp_assets_shutdown);IMPORT_INT(jp_queue_reset_frame);
IMPORT_INT(jp_frames_shutdown);IMPORT_INT(jp_memory_unbind_source);
IMPORT_INT(jp_memory_shutdown);IMPORT_INT(jp_pvm_unbind_destination);
IMPORT_VOID(task_destroy_all);IMPORT_VOID(jp_heap_reset);
IMPORT_VOID(jp_assets_clear_error);IMPORT_VOID(jp_memory_clear_error);
IMPORT_VOID(jp_textures_shutdown);IMPORT_VOID(jp_render_state_shutdown);
IMPORT_VOID(jp_queue_shutdown);IMPORT_VOID(jp_clock_shutdown);
__declspec(dllimport) void jp_window_close(struct jp_window *);
__declspec(dllimport) void jp_gpu_close(struct jp_gpu *);
__declspec(dllimport) void *jp_window_native_handle(struct jp_window *);
__declspec(dllimport) unsigned jp_gpu_jobs_live(struct jp_gpu *);
__declspec(dllimport) unsigned jp_textures_live_surfaces(void);
__declspec(dllimport) size_t jp_textures_allocated_bytes(void);
static struct {
    struct jp_exit_info info;struct jp_gpu *gpu;struct jp_window *window;
    DWORD owner;int bound,busy;
} state;
static int owner(void)
{return !state.bound?JP_EXIT_NOT_BOUND:state.owner!=GetCurrentThreadId()?JP_EXIT_THREAD:JP_EXIT_OK;}
int jp_exit_status(void){int e=owner();return e?e:(int)state.info.status;}
int jp_exit_snapshot(struct jp_exit_info *out)
{int e=owner();if(e)return e;if(!out)return JP_EXIT_ARGUMENT;*out=state.info;return JP_EXIT_OK;}
int jp_exit_bind(const struct jp_exit_config *cfg)
{
    if(state.bound)return state.owner==GetCurrentThreadId()?JP_EXIT_ALREADY_BOUND:JP_EXIT_THREAD;
    if(!cfg||cfg->bytes!=sizeof *cfg||!cfg->resources||(cfg->resources&~JP_EXIT_ALL)||
       (!!cfg->gpu!=!!(cfg->resources&JP_EXIT_GPU))||
       (!!cfg->window!=!!(cfg->resources&JP_EXIT_WINDOW)))return JP_EXIT_ARGUMENT;
    if(cfg->window && !jp_window_native_handle(cfg->window))return JP_EXIT_ARGUMENT;
    memset(&state,0,sizeof state);state.bound=1;state.owner=GetCurrentThreadId();
    state.gpu=cfg->gpu;state.window=cfg->window;state.info.remaining=cfg->resources;return JP_EXIT_OK;
}
static int failed(unsigned resource,int code)
{
    state.info.status=JP_EXIT_SERVICE;state.info.failed_resource=resource;
    state.info.native_error=(uint32_t)code;state.busy=0;return JP_EXIT_SERVICE;
}
static void complete(unsigned resource)
{state.info.remaining&=~resource;state.info.completed|=resource;}
int jp_exit_cleanup(void)
{
    int e=owner();if(e)return e;if(state.busy)return JP_EXIT_REENTRANT;
    state.busy=1;state.info.cleanup_calls++;state.info.status=state.info.failed_resource=state.info.native_error=0;
    /* Drain captured GPU work before releasing any producers/resources. This
     * service does not dispatch EOR callbacks while shutting down. */
#define CLOSE_INT(bit,call) do {if(state.info.remaining&(bit)){int rc=(call);if(rc)return failed((bit),rc);complete(bit);}} while(0)
    CLOSE_INT(JP_EXIT_PRESENT,jp_present_shutdown());
    CLOSE_INT(JP_EXIT_INPUT,jp_input_shutdown());
    CLOSE_INT(JP_EXIT_RECORDING,jp_recording_shutdown());
    CLOSE_INT(JP_EXIT_WAIT,jp_wait_shutdown());
    if(state.info.remaining&JP_EXIT_TASKS){task_destroy_all();jp_heap_reset();complete(JP_EXIT_TASKS);}
    if(state.info.remaining&JP_EXIT_ASSETS){
        jp_assets_clear_error();jp_memory_clear_error();
        int rc=jp_pvm_unbind_destination();if(rc)return failed(JP_EXIT_ASSETS,rc);
        rc=jp_memory_unbind_source();if(rc)return failed(JP_EXIT_ASSETS,rc);
        rc=jp_assets_release_data();if(rc)return failed(JP_EXIT_ASSETS,rc);
        rc=jp_assets_recover_pending();if(rc)return failed(JP_EXIT_ASSETS,rc);
        rc=jp_assets_shutdown();if(rc)return failed(JP_EXIT_ASSETS,rc);
        complete(JP_EXIT_ASSETS);
    }
    if(state.info.remaining&JP_EXIT_QUEUE){int rc=jp_queue_reset_frame();if(rc)return failed(JP_EXIT_QUEUE,rc);}
    CLOSE_INT(JP_EXIT_FRAMES,jp_frames_shutdown());
    /* A borrowed memory view must have ended before shutdown. Standalone
     * partial-bootstrap cleanup reports it and permits explicit recovery. */
    CLOSE_INT(JP_EXIT_MEMORY,jp_memory_shutdown());
    if(state.info.remaining&JP_EXIT_RENDER_STATE){jp_render_state_shutdown();complete(JP_EXIT_RENDER_STATE);}
    if(state.info.remaining&JP_EXIT_TEXTURES){
        jp_textures_shutdown();if(jp_textures_live_surfaces()||jp_textures_allocated_bytes())return failed(JP_EXIT_TEXTURES,1);
        complete(JP_EXIT_TEXTURES);
    }
    if(state.info.remaining&JP_EXIT_QUEUE){jp_queue_shutdown();complete(JP_EXIT_QUEUE);}
    if(state.info.remaining&JP_EXIT_CLOCK){jp_clock_shutdown();complete(JP_EXIT_CLOCK);}
    if(state.info.remaining&JP_EXIT_WINDOW){jp_window_close(state.window);state.window=NULL;complete(JP_EXIT_WINDOW);}
    if(state.info.remaining&JP_EXIT_GPU){
        if(jp_gpu_jobs_live(state.gpu))return failed(JP_EXIT_GPU,1);
        jp_gpu_close(state.gpu);state.gpu=NULL;complete(JP_EXIT_GPU);
    }
#undef CLOSE_INT
    state.busy=0;return JP_EXIT_OK;
}
void sbExitSystem(void){(void)jp_exit_cleanup();}
__declspec(noreturn) void syBtExit(void)
{int code=jp_exit_cleanup();ExitProcess(code?64u+(unsigned)code:0u);}
