#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include "jp_native_game_init.h"
#define JP_MAIN_BOOT_API __declspec(dllimport)
#define JP_SOUND_CLOCK_API __declspec(dllimport)
#define JP_AICA_API __declspec(dllimport)
#define JP_DEVICE_API __declspec(dllimport)
#define JP_SYSTEM_API __declspec(dllimport)
#define JP_SYSTEM_GAME_INIT_API __declspec(dllimport)
#define JP_GAME_SOUND_API __declspec(dllimport)
#define JP_HISTORY_API __declspec(dllimport)
#define JP_READ_QUEUE_API __declspec(dllimport)
#define JP_RNA_API __declspec(dllimport)
#define JP_ADXSTM_API __declspec(dllimport)
#define JP_ADXF_API __declspec(dllimport)
#define JP_ADXT_API __declspec(dllimport)
#define JP_GDFS_API __declspec(dllimport)
#include "jp_native_main_boot.h"
#include "jp_native_system.h"
#include "jp_native_system_game_init.h"
#include "jp_native_history.h"
#include "jp_native_read_queue.h"
#include "jp_native_rna.h"
#include "jp_native_adxstm.h"
#include "jp_native_adxf.h"
#include "jp_native_adxt.h"
#include "jp_native_gdfs.h"
#include "jp_game_frame_wait_imports.h"
__declspec(dllimport) void frame_wait(void);
__declspec(dllimport) int jp_present_status(void);
__declspec(dllimport) void ADXCRS_Init(void),SJRBF_Init(void),SJMEM_Init(void),ADXSJD_Init(void),ADXERR_Init(void);
__declspec(dllimport) void gdFsEntryErrFuncAll(void (*)(void *,int32_t),void *);
extern uint32_t sygIntMode,mwsfdg_intStack[2048];extern void *sygIntStack;
extern int32_t game_mode,loader_error_flag;
void jp_original_game_init(void),jp_original_loader_note_error(void *,int32_t);
static struct {
    struct jp_game_init_info info;struct jp_history_options history;
    wchar_t directory[32768],track[32768];DWORD thread;int busy;
    uint64_t scope;
} game;
_Static_assert(sizeof(struct jp_game_init_info)==176,"game initialization snapshot ABI");
static int access_owner(void)
{return game.thread&&game.thread!=GetCurrentThreadId()?JP_GAME_INIT_THREAD:game.busy?JP_GAME_INIT_REENTRANT:0;}
static int result(int code,int native)
{game.info.status=(uint32_t)code;game.info.native_error=(uint32_t)native;return code;}
int jp_game_init_failed(void){return game.info.status!=0;}
static void problem(int code,int native)
{if(!game.info.status)result(code,native);}
static int output_valid(const void *p,size_t n)
{
    uintptr_t at=(uintptr_t)p,base=(uintptr_t)&game,stack=(uintptr_t)mwsfdg_intStack;
    return p&&n<=UINTPTR_MAX-at&&!(at<base+sizeof game&&base<at+n)&&!(at<stack+8192&&stack<at+n);
}
static int audio_state(uint32_t *stages)
{
    struct jp_rna_info r;struct jp_adxstm_info s;struct jp_adxf_info f;struct jp_adxt_info t;
    if(jp_rna_snapshot(&r)||jp_adxstm_snapshot(&s)||jp_adxf_snapshot(&f)||jp_adxt_snapshot(&t))return JP_GAME_INIT_AUDIO;
    *stages=(r.active?1u:0u)|(s.active?2u:0u)|(f.active?4u:0u)|(t.active?8u:0u);return 0;
}
int jp_game_init_snapshot(struct jp_game_init_info *out)
{
    if(game.thread&&game.thread!=GetCurrentThreadId())return JP_GAME_INIT_THREAD;
    if(!output_valid(out,sizeof *out))return JP_GAME_INIT_ARGUMENT;
    struct jp_game_init_info info=game.info;struct jp_system_info actual={0};uint32_t stages=0;
    if(jp_system_snapshot(&actual)||audio_state(&stages))return JP_GAME_INIT_STATE;
    info.parent_owned=game.info.parent_owned;info.scope_held=game.scope!=0;info.system_phase=actual.phase;
    info.game_mode=game_mode;info.loader_error_flag=loader_error_flag;info.audio_stages=stages;
    info.preinit_mode=sygIntMode;info.stack_base=(uintptr_t)mwsfdg_intStack;info.stack_end=(uintptr_t)sygIntStack;
    *out=info;return 0;
}
int jp_game_init_configure(const struct jp_history_options *options)
{
    int rc=access_owner();if(rc)return rc;
    if(game.info.parent_owned||game.scope||game.info.phase==2||game.info.phase==3)return JP_GAME_INIT_STATE;
    if(!options||options->bytes!=sizeof *options||!options->directory||!*options->directory||!options->track||!*options->track||
       options->create_if_missing>1||options->reserved||(options->raw_sector_bytes!=2048&&options->raw_sector_bytes!=2352))return JP_GAME_INIT_ARGUMENT;
    wchar_t directory[32768],track[32768];DWORD n=GetFullPathNameW(options->directory,32768,directory,NULL),m=GetFullPathNameW(options->track,32768,track,NULL);
    if(!n||n>=32768||!m||m>=32768)return JP_GAME_INIT_ARGUMENT;
    memcpy(game.directory,directory,(n+1)*sizeof *directory);memcpy(game.track,track,(m+1)*sizeof *track);game.history=*options;
    game.history.directory=game.directory;game.history.track=game.track;game.thread=GetCurrentThreadId();game.info.creator_thread=game.thread;game.info.phase=1;return result(0,0);
}
void jp_game_parent_start(void)
{
    if(!game.busy||game.info.stage!=1){problem(JP_GAME_INIT_STATE,0);return;}
    if(sygIntMode!=UINT32_C(0x80000000)||sygIntStack!=mwsfdg_intStack+2048){problem(JP_GAME_INIT_STATE,1);return;}
    game.info.preinit_calls++;game.info.stage=2;game.info.parent_calls++;game.info.parent_owned=1;
    int rc=jp_main_boot_initialize();if(rc){problem(JP_GAME_INIT_PARENT,rc);return;}
    rc=jp_system_game_init_claim(&game.scope);if(rc){problem(JP_GAME_INIT_SCOPE,rc);return;}
    struct jp_wait_snapshot wait;if(jp_wait_snapshot(&wait)){problem(JP_GAME_INIT_FRAME,JP_WAIT_NOT_INITIALIZED);return;}
    game.info.logical_ticks_before=wait.ticks;
}
void jp_game_frame_wait(void)
{
    if(!game.busy||!game.scope){problem(JP_GAME_INIT_STATE,0);return;}
    game.info.stage=3;game.info.frame_calls++;frame_wait();
    int rc=jp_wait_status();if(!rc)rc=jp_present_status();
    if(rc){problem(JP_GAME_INIT_FRAME,rc);return;}
    struct jp_wait_snapshot wait;if(jp_wait_snapshot(&wait)){problem(JP_GAME_INIT_FRAME,JP_WAIT_CLOCK);return;}
    game.info.logical_ticks_after=wait.ticks;game.info.frame_completed++;
}
void jp_game_adxt_start(void)
{
    if(!game.busy||!game.scope){problem(JP_GAME_INIT_STATE,0);return;}
    game.info.stage=4;game.info.audio_calls++;uint32_t stages=0;
    if(audio_state(&stages)||stages){problem(JP_GAME_INIT_AUDIO,JP_SOUND_STATE);return;}
    /* The original cold SDK pools, then existing checked native owners. The
     * same adapted original ADXT_Init runs exactly once inside its real owner.
     * Existing Sound cleanup owns partial and completed nested initialization. */
    ADXCRS_Init();SJRBF_Init();SJMEM_Init();ADXSJD_Init();ADXERR_Init();
    int rc=jp_rna_initialize();if(!rc)rc=jp_adxstm_initialize();if(!rc)rc=jp_adxf_initialize();if(!rc)rc=jp_adxt_initialize();
    if(rc)problem(JP_GAME_INIT_AUDIO,rc);
}
void jp_game_read_queue_start(void)
{
    if(!game.busy||!game.scope){problem(JP_GAME_INIT_STATE,0);return;}
    game.info.stage=5;game.info.queue_calls++;int rc=jp_system_game_init_queue(game.scope);if(rc)problem(JP_GAME_INIT_QUEUE,rc);
}
void jp_game_history_start(void)
{
    if(!game.busy||!game.scope){problem(JP_GAME_INIT_STATE,0);return;}
    game.info.stage=6;game.info.history_calls++;int rc=jp_system_game_init_history(game.scope,&game.history);if(rc)problem(JP_GAME_INIT_HISTORY,rc);
}
__declspec(dllexport) void loader_note_error(void *object,int32_t error)
{
    if(!game.thread||game.thread!=GetCurrentThreadId()||!game.info.callback_bound)return;
    game.info.loader_error_calls++;game.info.last_loader_error=error;jp_original_loader_note_error(object,error);
}
void jp_game_loader_error_bind(void (*callback)(void *,int32_t),void *object)
{
    if(!game.busy||!game.scope||callback!=loader_note_error||object){problem(JP_GAME_INIT_CALLBACK,0);return;}
    game.info.stage=7;struct jp_gdfs_info fs;
    if(jp_gdfs_snapshot(&fs)||fs.phase!=JP_GDFS_READY){problem(JP_GAME_INIT_CALLBACK,JP_GDFS_STATE);return;}
    gdFsEntryErrFuncAll(callback,object);game.info.callback_bound=1;
}
int jp_game_initialize(void)
{
    int rc=access_owner();if(rc)return rc;if(game.info.phase!=1||game.info.parent_owned||game.scope)return JP_GAME_INIT_STATE;
    struct jp_system_info system;struct jp_main_boot_info boot;struct jp_read_queue_info queue;struct jp_history_info history;struct jp_sound_info sound;
    if(jp_system_snapshot(&system)||system.phase!=JP_SYSTEM_CONFIGURED||system.resources||jp_main_boot_snapshot(&boot)||boot.phase!=1||boot.card_owned||
       jp_read_queue_snapshot(&queue)||queue.initialized||jp_history_snapshot(&history)||history.resources||jp_sound_snapshot(&sound)||sound.resources)return JP_GAME_INIT_STATE;
    game.busy=1;game.info.phase=2;game.info.stage=1;game.info.starts++;game.info.callback_bound=0;result(0,0);jp_original_game_init();
    if(!game.info.status){rc=jp_system_game_init_release(game.scope);if(rc)problem(JP_GAME_INIT_SCOPE,rc);else game.scope=0;}
    game.busy=0;if(game.info.status){game.info.phase=4;return (int)game.info.status;}
    game.info.phase=3;game.info.stage=8;game.info.completions++;return 0;
}
int jp_game_init_shutdown(uint32_t timeout)
{
    int rc=access_owner();if(rc)return rc;if(!timeout||timeout>60000)return JP_GAME_INIT_ARGUMENT;
    if(!game.info.parent_owned&&!game.scope)return 0;game.busy=1;
    if(game.scope){rc=jp_system_game_init_release(game.scope);if(rc){game.busy=0;return result(JP_GAME_INIT_CLEANUP,rc);}game.scope=0;}
    struct jp_system_info system;rc=jp_system_snapshot(&system);
    if(!rc){
        if(system.phase==JP_SYSTEM_DELEGATED)rc=jp_system_shutdown();
        else if(system.phase!=JP_SYSTEM_CLOSED||system.resources)rc=jp_main_boot_shutdown(timeout);
    }
    game.busy=0;if(rc){game.info.phase=4;return result(JP_GAME_INIT_CLEANUP,rc);}
    game.info.parent_owned=0;game.info.callback_bound=0;game.info.phase=5;game.info.shutdowns++;return result(0,0);
}
void game_init(void)
{int rc=jp_game_initialize();if(rc){(void)jp_game_init_shutdown(5000);ExitProcess(64u+(unsigned)rc);}}
