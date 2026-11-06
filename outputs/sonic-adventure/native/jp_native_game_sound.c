/* Host ownership is isolated from original console headers and memory maps. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include <string.h>
#define JP_AICA_API __declspec(dllimport)
#include "jp_native_game_sound.h"
#define JP_G2_API __declspec(dllimport)
#define JP_GDFS_API __declspec(dllimport)
#define JP_SOUND_DISPATCH_API __declspec(dllimport)
#include "jp_native_g2_audio.h"
#include "jp_native_gdfs.h"
#include "jp_native_sound_dispatch.h"

extern void jp_game_sound_system_init(void);
extern int32_t jp_game_sdk_service(void);
extern int jp_game_sdk_active(void);
extern void jp_game_sdk_retire(void);
extern void gdFsClose(void *);
extern __declspec(dllimport) int jp_wait_status(void);
_Static_assert(sizeof(struct jp_sound_options)==56,"sound options ABI");
_Static_assert(sizeof(struct jp_sound_info)==104,"sound snapshot ABI");
_Static_assert(sizeof(long)==4,"SDK Sint32 uses Windows long32");
static SRWLOCK lock=SRWLOCK_INIT;
static struct {
    struct jp_sound_options options;struct jp_sound_info info;
    DWORD owner;int busy;char name[256];void *allocation,*stage,*file;
    void *config_work;uint32_t config_generation;
} sound={.info={.phase=JP_SOUND_COLD,.status=JP_SOUND_STATE}};
static int thread_error(void)
{return sound.owner&&sound.owner!=GetCurrentThreadId()?JP_SOUND_THREAD:0;}
static int leave(int rc){ReleaseSRWLockExclusive(&lock);return rc;}
static int fail(int rc,uint32_t resource,uint32_t native)
{
    if(!sound.info.status){sound.info.status=(uint32_t)rc;
        sound.info.failed_resource=resource;sound.info.native_error=native;}
    if(sound.info.phase==JP_SOUND_READY)sound.info.phase=JP_SOUND_FAILED;
    return rc;
}
static int begin(int ready)
{
    AcquireSRWLockExclusive(&lock);int rc=thread_error();
    if(!rc&&sound.busy)rc=JP_SOUND_REENTRANT;
    if(!rc&&ready&&(sound.info.phase!=JP_SOUND_READY||sound.info.status))rc=JP_SOUND_STATE;
    if(!rc)sound.busy=1;
    return leave(rc);
}
static int end(int rc)
{
    AcquireSRWLockExclusive(&lock);sound.busy=0;
    return leave(rc);
}
static int32_t frame_service(void *context)
{(void)context;return jp_sound_service();}
static int outside_stage(const void *pointer,uint64_t bytes)
{
    uintptr_t p=(uintptr_t)pointer,a=(uintptr_t)sound.allocation;
    return p&&bytes<=UINTPTR_MAX-p&&(!a||p+bytes<=a||p>=a+sound.info.stage_reservation_bytes);
}
int jp_sound_configure(const struct jp_sound_options *options)
{
    AcquireSRWLockExclusive(&lock);int rc=thread_error();if(rc)return leave(rc);
    if(sound.busy)return leave(JP_SOUND_REENTRANT);
    if(sound.info.resources||sound.config_work)return leave(JP_SOUND_STATE);
    if(!options||options->bytes!=sizeof *options||options->sound_mode>1||
       options->max_driver_bytes<32||options->max_driver_bytes>2u<<20||
       !options->close_timeout_ms||options->close_timeout_ms>60000)return leave(JP_SOUND_ARGUMENT);
    const struct jp_aica_options *a=&options->aica;
    if(a->bytes!=sizeof *a||a->flags&~JP_AICA_WITH_OUTPUT||a->capacity_frames<4096||
       a->capacity_frames>1048576||a->latency_ms<20||a->latency_ms>500||
       !a->timeout_ms||a->timeout_ms>60000||a->muted>1||!isfinite(a->gain)||
       a->gain<0||a->gain>1||a->reserved)return leave(JP_SOUND_ARGUMENT);
    const char *name=options->driver_name?options->driver_name:"masaru.drv";
    size_t n=strnlen(name,sizeof sound.name);
    if(!n||n==sizeof sound.name)return leave(JP_SOUND_ARGUMENT);
    /* A single GDFS filename; callers cannot redirect the current directory. */
    for(size_t i=0;i<n;i++)if((unsigned char)name[i]<32||strchr("/\\:",name[i]))return leave(JP_SOUND_ARGUMENT);
    struct jp_sound_options copy=*options;memmove(sound.name,name,n+1);
    sound.options=copy;sound.options.driver_name=sound.name;sound.owner=GetCurrentThreadId();
    uint64_t starts=sound.info.initializations,stops=sound.info.shutdowns,sessions=sound.info.config_sessions;
    sound.info=(struct jp_sound_info){.phase=JP_SOUND_CONFIGURED,.creator_thread=sound.owner,
        .sound_mode=copy.sound_mode,.initializations=starts,.shutdowns=stops,.config_sessions=sessions};
    return leave(0);
}
/* Private bridge, callable only by original code while this owner is starting. */
int jp_sound_failed(void){return (int)sound.info.status;}
const char *jp_sound_driver_name(void){return sound.name;}
void jp_sound_problem(int rc,uint32_t resource,uint32_t native)
{AcquireSRWLockExclusive(&lock);fail(rc,resource,native);ReleaseSRWLockExclusive(&lock);}
int jp_sound_record_sdk(long error)
{
    if(error){AcquireSRWLockExclusive(&lock);sound.info.sdk_error=(int32_t)error;
        fail(JP_SOUND_SDK,JP_SOUND_OWNS_SDK,(uint32_t)error);ReleaseSRWLockExclusive(&lock);}
    return error!=0;
}
void jp_sound_owns_sdk(void){sound.info.resources|=JP_SOUND_OWNS_SDK;}
void jp_sound_adopt_file(void *file){sound.file=file;sound.info.resources|=JP_SOUND_OWNS_FILE;}
void jp_sound_release_file(void){sound.file=NULL;sound.info.resources&=~JP_SOUND_OWNS_FILE;}
void *jp_sound_stage_data(void){return sound.stage;}
int jp_sound_stage_create(long size)
{
    if(size<32||(uint32_t)size>sound.options.max_driver_bytes||sound.allocation){
        jp_sound_problem(JP_SOUND_DRIVER,JP_SOUND_OWNS_STAGE,0);return 1;}
    SYSTEM_INFO system;GetSystemInfo(&system);SIZE_T page=system.dwPageSize;
    SIZE_T sectors=((SIZE_T)size+2047)/2048*2048,commit=(sectors+page-1)/page*page;
    sound.allocation=VirtualAlloc(NULL,commit+2*page,MEM_RESERVE,PAGE_NOACCESS);
    if(!sound.allocation){jp_sound_problem(JP_SOUND_MEMORY,JP_SOUND_OWNS_STAGE,GetLastError());return 1;}
    sound.info.resources|=JP_SOUND_OWNS_STAGE;sound.info.stage_reservation_bytes=commit+2*page;
    sound.stage=VirtualAlloc((char *)sound.allocation+page,commit,MEM_COMMIT,PAGE_READWRITE);
    if(!sound.stage){jp_sound_problem(JP_SOUND_MEMORY,JP_SOUND_OWNS_STAGE,GetLastError());return 1;}
    sound.info.stage_address=(uintptr_t)sound.stage;sound.info.stage_capacity=sectors;
    sound.info.driver_bytes=(uint32_t)size;return 0;
}
int jp_sound_validate_driver(long size)
{
    const unsigned char *p=sound.stage;uint32_t payload=0;
    if(p&&size>=32)memcpy(&payload,p+8,4);
    if(!p||size<32||memcmp(p,"SDRV",4)||p[4]!=1||p[5]>1||!payload||
       (payload&31)||payload>(uint32_t)size-32||payload>JP_AICA_RAM_BYTES){
        jp_sound_problem(JP_SOUND_DRIVER,JP_SOUND_OWNS_STAGE,0);return 1;}
    return 0;
}
/* Real copied native configuration session; no console flash/low RAM access.
 * The original API borrows a 16 KiB work buffer until syCfgExit. memcpy avoids
 * assuming that the original char stack buffer is aligned. Settings persistence
 * belongs to the future native settings store, not a pretend firmware write. */
struct config_record {uint32_t magic,generation,mode,reserved;};
static int config_valid(void)
{
    if(!sound.config_work)return 0;struct config_record r;memcpy(&r,sound.config_work,sizeof r);
    return r.magic==UINT32_C(0x47464353)&&r.generation==sound.config_generation&&r.mode==sound.options.sound_mode;
}
long syCfgInit(void *work)
{
    AcquireSRWLockExclusive(&lock);
    if(thread_error()||!sound.owner||!work||sound.config_work||
       (sound.info.phase!=JP_SOUND_CONFIGURED&&sound.info.phase!=JP_SOUND_STARTING))return leave(-1);
    if(++sound.config_generation==0)++sound.config_generation;
    struct config_record r={UINT32_C(0x47464353),sound.config_generation,sound.options.sound_mode,0};
    memcpy(work,&r,sizeof r);sound.config_work=work;sound.info.config_active=1;sound.info.config_sessions++;
    return leave(0);
}
long syCfgGetSoundMode(long *out)
{
    AcquireSRWLockExclusive(&lock);if(thread_error()||!out||!config_valid())return leave(-1);
    *out=(long)sound.options.sound_mode;return leave(0);
}
long syCfgSetSoundMode(long mode)
{
    AcquireSRWLockExclusive(&lock);if(thread_error()||mode<0||mode>1||!config_valid())return leave(-1);
    struct config_record r;memcpy(&r,sound.config_work,sizeof r);r.mode=(uint32_t)mode;
    memcpy(sound.config_work,&r,sizeof r);sound.options.sound_mode=sound.info.sound_mode=(uint32_t)mode;
    return leave(0);
}
long syCfgExit(void)
{
    AcquireSRWLockExclusive(&lock);if(thread_error()||!sound.config_work)return leave(-1);
    sound.config_work=NULL;sound.info.config_active=0;return leave(0);
}
static int cleanup(uint32_t timeout)
{
    if(sound.info.resources&JP_SOUND_OWNS_DISPATCH){
        int rc=jp_sound_dispatch_unbind(sound.info.dispatch_token);if(rc)return JP_SOUND_DISPATCH;
        sound.info.resources&=~JP_SOUND_OWNS_DISPATCH;sound.info.dispatch_token=0;
    }
    if(sound.file){gdFsClose(sound.file);jp_sound_release_file();}
    sound.config_work=NULL;sound.info.config_active=0;
    if(sound.info.resources&JP_SOUND_OWNS_G2){
        uint32_t completed=0;int rc=jp_g2_service(64,&completed);
        if(rc||jp_g2_close())return JP_SOUND_G2;
        sound.info.resources&=~JP_SOUND_OWNS_G2;
    }
    if(sound.info.resources&JP_SOUND_OWNS_SDK){jp_game_sdk_retire();sound.info.resources&=~JP_SOUND_OWNS_SDK;}
    if(sound.info.resources&JP_SOUND_OWNS_AICA){
        int rc=jp_aica_close(timeout);if(rc)return JP_SOUND_AICA;
        sound.info.resources&=~JP_SOUND_OWNS_AICA;
    }
    if(sound.info.resources&JP_SOUND_OWNS_STAGE){
        if(!VirtualFree(sound.allocation,0,MEM_RELEASE))return JP_SOUND_MEMORY;
        sound.allocation=sound.stage=NULL;sound.info.resources&=~JP_SOUND_OWNS_STAGE;
        sound.info.stage_address=sound.info.stage_capacity=sound.info.stage_reservation_bytes=0;
    }
    return 0;
}
int jp_sound_initialize(void)
{
    int rc=begin(0);if(rc)return rc;
    if(sound.info.phase!=JP_SOUND_CONFIGURED||sound.info.resources||sound.config_work)return end(JP_SOUND_STATE);
    struct jp_gdfs_info fs;struct jp_aica_info ai;struct jp_g2_info g2;
    struct jp_sound_dispatch_info dispatch;
    if(jp_gdfs_snapshot(&fs)||fs.phase!=JP_GDFS_READY||jp_wait_status()||
       jp_aica_snapshot(&ai)||ai.resources||jp_g2_snapshot(&g2)||g2.ready||
       jp_sound_dispatch_snapshot(&dispatch)||dispatch.bound||jp_game_sdk_active())return end(JP_SOUND_DEPENDENCY);
    sound.info.phase=JP_SOUND_STARTING;sound.info.status=0;
    rc=jp_aica_open(&sound.options.aica);
    if(!jp_aica_snapshot(&ai)&&ai.creator_thread==sound.owner&&ai.resources)sound.info.resources|=JP_SOUND_OWNS_AICA;
    if(rc){fail(JP_SOUND_AICA,JP_SOUND_OWNS_AICA,(uint32_t)rc);goto rollback;}
    rc=jp_g2_open();if(!rc)sound.info.resources|=JP_SOUND_OWNS_G2;
    if(rc){fail(JP_SOUND_G2,JP_SOUND_OWNS_G2,(uint32_t)rc);goto rollback;}
    jp_game_sound_system_init();if(sound.info.status)goto rollback;
    rc=jp_sound_dispatch_bind(frame_service,NULL,&sound,&sound.info.dispatch_token);
    if(rc){fail(JP_SOUND_DISPATCH,JP_SOUND_OWNS_DISPATCH,(uint32_t)rc);goto rollback;}
    sound.info.resources|=JP_SOUND_OWNS_DISPATCH;sound.info.phase=JP_SOUND_READY;
    sound.info.initializations++;return end(0);
rollback:
    rc=(int)sound.info.status;sound.info.phase=JP_SOUND_FAILED;
    sound.info.cleanup_error=(uint32_t)cleanup(sound.options.close_timeout_ms);return end(rc);
}
int jp_sound_shutdown(uint32_t timeout)
{
    if(timeout>60000)return JP_SOUND_ARGUMENT;
    int rc=begin(0);if(rc)return rc;
    if(!sound.owner)return end(JP_SOUND_STATE);
    sound.info.phase=JP_SOUND_CLOSING;rc=cleanup(timeout);
    sound.info.cleanup_error=(uint32_t)rc;
    if(rc){fail(rc,0,0);sound.info.phase=JP_SOUND_FAILED;return end(rc);}
    sound.info.phase=JP_SOUND_CLOSED;sound.info.status=0;sound.info.shutdowns++;
    return end(0);
}
int jp_sound_service(void)
{
    int rc=begin(1);if(rc)return rc;int32_t sdk=jp_game_sdk_service();
    if(sdk){sound.info.sdk_error=sdk;fail(JP_SOUND_SDK,JP_SOUND_OWNS_SDK,(uint32_t)sdk);rc=JP_SOUND_SDK;}
    return end(rc);
}
int jp_sound_render(uint32_t frames,int16_t *out,uint64_t bytes)
{
    int rc=begin(1);if(rc)return rc;
    if(!outside_stage(out,bytes)||!frames||frames>JP_AICA_MAX_FRAMES||bytes<(uint64_t)frames*4)return end(JP_SOUND_ARGUMENT);
    rc=jp_aica_render(frames,out,bytes);if(rc)fail(JP_SOUND_AICA,JP_SOUND_OWNS_AICA,(uint32_t)rc);
    return end(rc?JP_SOUND_AICA:0);
}
int jp_sound_pump(uint32_t frames)
{
    int rc=begin(1);if(rc)return rc;
    if(!frames||frames>JP_AICA_MAX_FRAMES||!(sound.options.aica.flags&JP_AICA_WITH_OUTPUT))return end(JP_SOUND_ARGUMENT);
    rc=jp_aica_pump(frames);
    /* Queue-full is temporary and does not quarantine a healthy owner. */
    if(rc==JP_AICA_QUEUE)return end(JP_SOUND_QUEUE);
    if(rc)fail(JP_SOUND_AICA,JP_SOUND_OWNS_AICA,(uint32_t)rc);
    return end(rc?JP_SOUND_AICA:0);
}
int jp_sound_snapshot(struct jp_sound_info *out)
{
    AcquireSRWLockExclusive(&lock);int rc=thread_error();if(rc)return leave(rc);
    if(!out||!outside_stage(out,sizeof *out))return leave(JP_SOUND_ARGUMENT);
    *out=sound.info;return leave(0);
}
