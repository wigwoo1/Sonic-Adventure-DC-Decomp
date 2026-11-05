/* Authored owner for the pinned, privately compiled portable ARM/AICA core. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include <string.h>
#include "jp_native_aica.h"
#define JP_AUDIO_API __declspec(dllimport)
#include "jp_native_audio.h"
#include "dcsound.h"
#include "arm.h"
#include "yam.h"
extern void jp_aica_core_cpu_reset(void *);
extern void jp_aica_core_held_render(void *,int16_t *,uint32_t);
extern void jp_aica_core_flush(void *);
_Static_assert(sizeof(struct jp_aica_options)==32,"aica options ABI");
_Static_assert(sizeof(struct jp_aica_info)==104,"aica snapshot ABI");
_Static_assert(sizeof(uint32)==4 && sizeof(sint16)==2 && sizeof(void *)==8,"core ABI");
enum {OWN_CORE=1,OWN_OUTPUT=2};
static SRWLOCK lock=SRWLOCK_INIT;
static struct {
    struct jp_aica_options options;struct jp_aica_info info;
    void *allocation,*core;SIZE_T allocation_bytes;DWORD owner;
    int16_t pcm[JP_AICA_MAX_FRAMES*2];float output[JP_AICA_MAX_FRAMES*2];
} aica={.info={.phase=JP_AICA_COLD,.status=JP_AICA_STATE}};
static int owner(void)
{return aica.owner&&aica.owner!=GetCurrentThreadId()?JP_AICA_THREAD:0;}
static int finish(int rc){ReleaseSRWLockExclusive(&lock);return rc;}
static int fail(int rc,unsigned native)
{aica.info.status=(uint32_t)rc;aica.info.native_error=native;aica.info.phase=JP_AICA_FAILED;return rc;}
static int ready(void)
{
    int rc=owner();if(rc)return rc;
    if(aica.info.phase!=JP_AICA_READY)return aica.info.status?(int)aica.info.status:JP_AICA_STATE;
    if(aica.info.resources&OWN_OUTPUT){rc=jp_audio_status();if(rc)return fail(JP_AICA_OUTPUT,(unsigned)rc);}
    return 0;
}
static int outside_core(const void *pointer,uint64_t bytes)
{
    uintptr_t at=(uintptr_t)pointer,base=(uintptr_t)aica.allocation;
    if(!at || bytes>UINTPTR_MAX-at)return 0;
    return !aica.allocation || at+bytes<=base || at>=base+aica.allocation_bytes;
}
static int cleanup(uint32_t timeout)
{
    if(aica.info.resources&OWN_OUTPUT){
        int rc=jp_audio_close(timeout);
        if(rc)return rc==JP_AUDIO_TIMEOUT?JP_AICA_TIMEOUT:JP_AICA_OUTPUT;
        aica.info.resources&=~OWN_OUTPUT;
    }
    if(aica.info.resources&OWN_CORE){
        if(!VirtualFree(aica.allocation,0,MEM_RELEASE))return JP_AICA_MEMORY;
        aica.allocation=aica.core=NULL;aica.allocation_bytes=0;aica.info.resources&=~OWN_CORE;
        aica.info.state_address=aica.info.state_bytes=0;
    }
    aica.owner=0;return 0;
}
int jp_aica_open(const struct jp_aica_options *options)
{
    AcquireSRWLockExclusive(&lock);int rc=owner();if(rc)return finish(rc);
    if(aica.owner||aica.info.resources)return finish(JP_AICA_STATE);
    if(!options||options->bytes!=sizeof *options||options->flags&~JP_AICA_WITH_OUTPUT||
       options->capacity_frames<4096||options->capacity_frames>1048576||options->latency_ms<20||options->latency_ms>500||
       !options->timeout_ms||options->timeout_ms>60000||options->muted>1||!isfinite(options->gain)||options->gain<0||options->gain>1||options->reserved)return finish(JP_AICA_ARGUMENT);
    if(options->flags&JP_AICA_WITH_OUTPUT){
        struct jp_audio_info prior;rc=jp_audio_snapshot(&prior);
        if(rc||!(prior.state==JP_AUDIO_COLD||prior.state==JP_AUDIO_CLOSED))return finish(JP_AICA_STATE);
    }
    aica.owner=GetCurrentThreadId();aica.options=*options;
    aica.info=(struct jp_aica_info){.phase=JP_AICA_OPENING,.creator_thread=aica.owner,.arm_reset=1,.output_enabled=!!(options->flags&JP_AICA_WITH_OUTPUT)};
    const uint32 word=0x12345678;
    if(*(const uint8 *)&word!=0x78||arm_init()||yam_init()||dcsound_init()){rc=fail(JP_AICA_CORE,0);goto rollback;}
    uint32 bytes=dcsound_get_state_size();
    if(bytes<=JP_AICA_RAM_BYTES||bytes>16u<<20){rc=fail(JP_AICA_CORE,0);goto rollback;}
    SYSTEM_INFO system;GetSystemInfo(&system);SIZE_T page=system.dwPageSize;
    SIZE_T committed=((SIZE_T)bytes+page-1)/page*page;
    aica.allocation_bytes=committed+page*2;
    aica.allocation=VirtualAlloc(NULL,aica.allocation_bytes,MEM_RESERVE,PAGE_NOACCESS);
    if(!aica.allocation){rc=fail(JP_AICA_MEMORY,GetLastError());goto rollback;}
    aica.info.resources|=OWN_CORE;
    aica.core=VirtualAlloc((char *)aica.allocation+page,committed,MEM_COMMIT,PAGE_READWRITE);
    if(!aica.core){rc=fail(JP_AICA_MEMORY,GetLastError());goto rollback;}
    dcsound_clear_state(aica.core);jp_aica_core_cpu_reset(aica.core);
    aica.info.state_address=(uintptr_t)aica.core;aica.info.state_bytes=bytes;
    if(aica.info.output_enabled){
        struct jp_audio_config config={sizeof config,44100,options->capacity_frames,options->latency_ms,options->timeout_ms,options->muted,options->gain,0};
        int opened=jp_audio_open(&config);struct jp_audio_info current;
        if(!jp_audio_snapshot(&current)&&current.creator_thread==aica.owner&&current.state!=JP_AUDIO_COLD&&current.state!=JP_AUDIO_CLOSED)aica.info.resources|=OWN_OUTPUT;
        if(opened||!(aica.info.resources&OWN_OUTPUT)){rc=fail(JP_AICA_OUTPUT,(unsigned)opened);goto rollback;}
    }
    aica.info.phase=JP_AICA_READY;aica.info.status=0;return finish(0);
rollback:
    (void)cleanup(options->timeout_ms);return finish(rc);
}
int jp_aica_read(uint32_t address,void *out,uint32_t bytes)
{
    AcquireSRWLockExclusive(&lock);int rc=ready();if(rc)return finish(rc);
    if(!bytes||address>=JP_AICA_RAM_BYTES||bytes>JP_AICA_RAM_BYTES-address||!outside_core(out,bytes))return finish(JP_AICA_ARGUMENT);
    jp_aica_core_flush(aica.core);unsigned char *p=out;uint32_t at=address,remaining=bytes;
    while(remaining){uint32_t value=dcsound_getword(aica.core,at&~3u);uint32_t offset=at&3u,n=4-offset;if(n>remaining)n=remaining;memcpy(p,(const char *)&value+offset,n);p+=n;at+=n;remaining-=n;}
    aica.info.read_bytes+=bytes;return finish(0);
}
int jp_aica_write(uint32_t address,const void *data,uint32_t bytes)
{
    AcquireSRWLockExclusive(&lock);int rc=ready();if(rc)return finish(rc);
    if(!bytes||address>=JP_AICA_RAM_BYTES||bytes>JP_AICA_RAM_BYTES-address||!outside_core(data,bytes))return finish(JP_AICA_ARGUMENT);
    jp_aica_core_flush(aica.core);dcsound_upload_to_ram(aica.core,address,(void *)data,bytes);aica.info.write_bytes+=bytes;return finish(0);
}
int jp_aica_read_register(uint32_t offset,uint32_t *out)
{
    AcquireSRWLockExclusive(&lock);int rc=ready();if(rc)return finish(rc);
    if(offset>=0x10000||offset&3u||!outside_core(out,sizeof *out))return finish(JP_AICA_ARGUMENT);
    jp_aica_core_flush(aica.core);*out=offset==0x2c00?aica.info.arm_reset:yam_aica_load_reg(dcsound_get_yam_state(aica.core),offset,0xffff);return finish(0);
}
static void reset_arm(uint32_t held)
{
    if(held&&!aica.info.arm_reset){jp_aica_core_cpu_reset(aica.core);aica.info.resets++;}
    aica.info.arm_reset=held;
}
int jp_aica_write_register(uint32_t offset,uint32_t value)
{
    AcquireSRWLockExclusive(&lock);int rc=ready();if(rc)return finish(rc);
    if(offset>=0x10000||offset&3u)return finish(JP_AICA_ARGUMENT);
    if(offset==0x2c00){if(value&~1u)return finish(JP_AICA_ARGUMENT);reset_arm(value);}
    else{jp_aica_core_flush(aica.core);yam_aica_store_reg(dcsound_get_yam_state(aica.core),offset,value,0xffff,NULL);}
    return finish(0);
}
int jp_aica_set_arm_reset(uint32_t held)
{
    AcquireSRWLockExclusive(&lock);int rc=ready();if(rc)return finish(rc);
    if(held>1)return finish(JP_AICA_ARGUMENT);reset_arm(held);return finish(0);
}
static int render(uint32_t frames)
{
    if(aica.info.arm_reset)jp_aica_core_held_render(aica.core,aica.pcm,frames);
    else{
        uint32_t count=frames;int cycles=dcsound_execute(aica.core,(int)(frames*128),aica.pcm,&count);
        if(cycles<0||count!=frames)return fail(JP_AICA_CORE,(unsigned)cycles);
        aica.info.cycles+=(unsigned)cycles;
    }
    aica.info.frames+=frames;aica.info.render_calls++;return 0;
}
int jp_aica_render(uint32_t frames,int16_t *out,uint64_t bytes)
{
    AcquireSRWLockExclusive(&lock);int rc=ready();if(rc)return finish(rc);
    if(!frames||frames>JP_AICA_MAX_FRAMES||bytes<(uint64_t)frames*4||!outside_core(out,(uint64_t)frames*4))return finish(JP_AICA_ARGUMENT);
    rc=render(frames);if(!rc)memcpy(out,aica.pcm,(size_t)frames*4);return finish(rc);
}
int jp_aica_pump(uint32_t frames)
{
    AcquireSRWLockExclusive(&lock);int rc=ready();if(rc)return finish(rc);
    if(!frames||frames>JP_AICA_MAX_FRAMES)return finish(JP_AICA_ARGUMENT);
    if(!(aica.info.resources&OWN_OUTPUT))return finish(JP_AICA_STATE);
    struct jp_audio_info info;rc=jp_audio_snapshot(&info);if(rc)return finish(fail(JP_AICA_OUTPUT,(unsigned)rc));
    if(frames>info.capacity_frames-info.queued_frames)return finish(JP_AICA_QUEUE);
    rc=render(frames);if(rc)return finish(rc);
    for(uint32_t j=0;j<frames*2;j++)aica.output[j]=aica.pcm[j]/32768.0f;
    rc=jp_audio_submit(aica.output,frames);if(rc)return finish(fail(JP_AICA_OUTPUT,(unsigned)rc));
    aica.info.submitted+=frames;return finish(0);
}
int jp_aica_drain(uint32_t timeout)
{
    AcquireSRWLockExclusive(&lock);int rc=ready();if(rc)return finish(rc);
    if(timeout>60000)return finish(JP_AICA_ARGUMENT);
    if(!(aica.info.resources&OWN_OUTPUT))return finish(JP_AICA_STATE);
    rc=jp_audio_drain(timeout);
    return finish(rc==JP_AUDIO_TIMEOUT?JP_AICA_TIMEOUT:rc?fail(JP_AICA_OUTPUT,(unsigned)rc):0);
}
int jp_aica_close(uint32_t timeout)
{
    AcquireSRWLockExclusive(&lock);int rc=owner();if(rc)return finish(rc);
    if(timeout>60000)return finish(JP_AICA_ARGUMENT);
    if(!aica.owner)return finish(0);
    aica.info.phase=JP_AICA_CLOSING;rc=cleanup(timeout);
    if(!rc){aica.info.phase=JP_AICA_CLOSED;aica.info.arm_reset=0;}
    return finish(rc);
}
int jp_aica_status(void)
{AcquireSRWLockExclusive(&lock);return finish(ready());}
int jp_aica_snapshot(struct jp_aica_info *out)
{
    AcquireSRWLockExclusive(&lock);int rc=owner();if(rc)return finish(rc);
    if(!outside_core(out,sizeof *out))return finish(JP_AICA_ARGUMENT);*out=aica.info;return finish(0);
}
