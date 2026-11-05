#define WIN32_LEAN_AND_MEAN
#define COBJMACROS
#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <math.h>
#include <string.h>
#include "jp_native_audio.h"
#define FNV_OFFSET UINT64_C(14695981039346656037)
#define FNV_PRIME UINT64_C(1099511628211)
/* C declarations in the SDK do not instantiate these MIDL UUIDs. Values are
 * the interface identities in Windows SDK mmdeviceapi.h/audioclient.h. */
static const GUID enumerator_class={0xbcde0395,0xe52f,0x467c,{0x8e,0x3d,0xc4,0x57,0x92,0x91,0x69,0x2e}};
static const GUID enumerator_iid={0xa95664d2,0x9614,0x4f35,{0xa7,0x46,0xde,0x8d,0xb6,0x36,0x17,0xe6}};
static const GUID client_iid={0x1cb9ad4c,0xdbfa,0x4c32,{0xb1,0x78,0xc2,0xf5,0x68,0xa7,0x03,0xb2}};
static const GUID render_iid={0xf294acfc,0x3146,0x4483,{0xa7,0xbf,0xad,0xdc,0xa7,0xc2,0x60,0xe2}};
static const GUID clock_iid={0xcd63314f,0x3fba,0x4a1b,{0x81,0x2c,0xef,0x96,0x35,0x87,0x28,0xe7}};
static const GUID volume_iid={0x87ce5498,0x68d6,0x44e5,{0x92,0x15,0x6d,0xa4,0x7e,0xf8,0x83,0xd8}};
static SRWLOCK lock=SRWLOCK_INIT;
static struct {
    struct jp_audio_config config;struct jp_audio_info info;
    HANDLE worker,stop,render,ready,progress;DWORD owner;float *ring;
    uint32_t read,used;
} audio={.info={.state=JP_AUDIO_COLD,.status=JP_AUDIO_STATE}};
static int access_owner(void)
{AcquireSRWLockShared(&lock);int rc=audio.owner&&audio.owner!=GetCurrentThreadId()?JP_AUDIO_THREAD:0;ReleaseSRWLockShared(&lock);return rc;}
static int access_locked(void)
{return audio.owner&&audio.owner!=GetCurrentThreadId()?JP_AUDIO_THREAD:0;}
static void failed(int code,HRESULT hr)
{
    AcquireSRWLockExclusive(&lock);audio.info.status=(uint32_t)code;
    audio.info.os_error=(uint32_t)hr;audio.info.state=JP_AUDIO_FAILED;ReleaseSRWLockExclusive(&lock);
}
static uint64_t sample_clock(uint64_t position,uint64_t frequency,uint32_t rate)
{return position/frequency*rate+(position%frequency)*rate/frequency;}
static DWORD WINAPI worker(void *unused)
{
    (void)unused;IMMDeviceEnumerator *enumerator=NULL;IMMDevice *device=NULL;
    IAudioClient *client=NULL;IAudioRenderClient *render=NULL;IAudioClock *clock=NULL;
    ISimpleAudioVolume *volume=NULL;HRESULT hr=CoInitializeEx(NULL,COINIT_MULTITHREADED);
    int initialized=SUCCEEDED(hr),started=0,error=JP_AUDIO_COM;UINT32 frames=0;UINT64 frequency=0;REFERENCE_TIME latency=0;
    AcquireSRWLockExclusive(&lock);audio.info.worker_thread=GetCurrentThreadId();ReleaseSRWLockExclusive(&lock);
    if(FAILED(hr))goto done;
    hr=CoCreateInstance(&enumerator_class,NULL,CLSCTX_ALL,&enumerator_iid,(void **)&enumerator);if(FAILED(hr))goto done;
    error=JP_AUDIO_DEVICE;hr=IMMDeviceEnumerator_GetDefaultAudioEndpoint(enumerator,eRender,eConsole,&device);if(FAILED(hr))goto done;
    hr=IMMDevice_Activate(device,&client_iid,CLSCTX_ALL,NULL,(void **)&client);if(FAILED(hr))goto done;
    GUID session;error=JP_AUDIO_COM;hr=CoCreateGuid(&session);if(FAILED(hr))goto done;
    WAVEFORMATEX format={0};format.wFormatTag=WAVE_FORMAT_IEEE_FLOAT;format.nChannels=2;
    format.nSamplesPerSec=audio.config.rate;format.wBitsPerSample=32;format.nBlockAlign=8;format.nAvgBytesPerSec=audio.config.rate*8;
    error=JP_AUDIO_FORMAT;
    hr=IAudioClient_Initialize(client,AUDCLNT_SHAREMODE_SHARED,
        AUDCLNT_STREAMFLAGS_EVENTCALLBACK|AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM|AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY|AUDCLNT_STREAMFLAGS_NOPERSIST,
        (REFERENCE_TIME)audio.config.latency_ms*10000,0,&format,&session);if(FAILED(hr))goto done;
    error=JP_AUDIO_IO;hr=IAudioClient_GetBufferSize(client,&frames);if(FAILED(hr))goto done;if(!frames){hr=E_UNEXPECTED;goto done;}
    hr=IAudioClient_GetStreamLatency(client,&latency);if(FAILED(hr))goto done;if(latency<0){hr=E_UNEXPECTED;goto done;}
    hr=IAudioClient_SetEventHandle(client,audio.render);if(FAILED(hr))goto done;
    hr=IAudioClient_GetService(client,&render_iid,(void **)&render);if(FAILED(hr))goto done;
    hr=IAudioClient_GetService(client,&clock_iid,(void **)&clock);if(FAILED(hr))goto done;
    hr=IAudioClock_GetFrequency(clock,&frequency);if(FAILED(hr))goto done;if(!frequency){hr=E_UNEXPECTED;goto done;}
    hr=IAudioClient_GetService(client,&volume_iid,(void **)&volume);if(FAILED(hr))goto done;
    hr=ISimpleAudioVolume_SetMute(volume,(BOOL)audio.config.muted,NULL);if(FAILED(hr))goto done;
    BOOL muted=FALSE;hr=ISimpleAudioVolume_GetMute(volume,&muted);if(FAILED(hr))goto done;
    if((unsigned)muted!=audio.config.muted){hr=E_UNEXPECTED;goto done;}
    BYTE *packet=NULL;hr=IAudioRenderClient_GetBuffer(render,frames,&packet);if(FAILED(hr))goto done;
    hr=IAudioRenderClient_ReleaseBuffer(render,frames,AUDCLNT_BUFFERFLAGS_SILENT);if(FAILED(hr))goto done;
    AcquireSRWLockExclusive(&lock);audio.info.device_buffer_frames=frames;audio.info.clock_frequency=frequency;
    audio.info.silence=audio.info.client_frames=frames;audio.info.session_muted=(uint32_t)muted;audio.info.stream_latency_hns=(uint64_t)latency;ReleaseSRWLockExclusive(&lock);
    if(WaitForSingleObject(audio.stop,0)==WAIT_OBJECT_0)goto done;
    hr=IAudioClient_Start(client);if(FAILED(hr))goto done;started=1;
    AcquireSRWLockExclusive(&lock);
    if(audio.info.state!=JP_AUDIO_OPENING){ReleaseSRWLockExclusive(&lock);goto done;}
    audio.info.state=JP_AUDIO_READY;audio.info.status=0;ReleaseSRWLockExclusive(&lock);SetEvent(audio.ready);
    HANDLE events[2]={audio.stop,audio.render};
    for(;;){
        DWORD wait=WaitForMultipleObjects(2,events,FALSE,250);
        if(wait==WAIT_OBJECT_0){hr=S_OK;break;}
        if(wait==WAIT_FAILED){hr=HRESULT_FROM_WIN32(GetLastError());goto done;}
        UINT32 padding=0;UINT64 position=0;
        hr=IAudioClient_GetCurrentPadding(client,&padding);if(FAILED(hr))goto done;if(padding>frames){hr=E_UNEXPECTED;goto done;}
        hr=IAudioClock_GetPosition(clock,&position,NULL);if(FAILED(hr))goto done;
        AcquireSRWLockExclusive(&lock);
        audio.info.clock_position=position;if(wait==WAIT_OBJECT_0+1)audio.info.events++;
        UINT32 available=frames-padding,n=audio.used;if(n>available)n=available;
        if(available){
            hr=IAudioRenderClient_GetBuffer(render,available,&packet);
            if(FAILED(hr)){ReleaseSRWLockExclusive(&lock);goto done;}
            float *out=(float *)packet;uint64_t hash=audio.info.source_hash,clipped=0;
            for(UINT32 j=0;j<n;j++)for(unsigned c=0;c<2;c++){
                float value=audio.ring[((audio.read+j)%audio.config.capacity_frames)*2+c]*audio.config.gain;
                if(value>1){value=1;clipped++;}else if(value< -1){value=-1;clipped++;}
                out[j*2+c]=value;
                const unsigned char *bytes=(const unsigned char *)&out[j*2+c];
                for(unsigned k=0;k<sizeof(float);k++)hash=(hash^bytes[k])*FNV_PRIME;
            }
            if(available>n)memset(out+n*2,0,(size_t)(available-n)*8);
            hr=IAudioRenderClient_ReleaseBuffer(render,available,n?0:AUDCLNT_BUFFERFLAGS_SILENT);
            if(FAILED(hr)){ReleaseSRWLockExclusive(&lock);goto done;}
            audio.info.client_frames+=available;audio.info.source_hash=hash;audio.info.clipped_samples+=clipped;
            audio.read=(audio.read+n)%audio.config.capacity_frames;audio.used-=n;audio.info.consumed+=n;audio.info.silence+=available-n;
            audio.info.queued_frames=audio.used;
            if(n){
                /* Cumulative submitted frames cannot locate a packet after an
                 * engine underrun. Use a fresh device clock plus the full
                 * buffer and reported maximum downstream stream latency. */
                hr=IAudioClock_GetPosition(clock,&position,NULL);
                if(FAILED(hr)){ReleaseSRWLockExclusive(&lock);goto done;}
                uint64_t now=sample_clock(position,frequency,audio.config.rate);
                uint64_t lag=(uint64_t)latency/10000000*audio.config.rate+
                    (((uint64_t)latency%10000000)*audio.config.rate+9999999)/10000000;
                if(now>UINT64_MAX-frames-lag){hr=E_UNEXPECTED;ReleaseSRWLockExclusive(&lock);goto done;}
                audio.info.clock_position=position;audio.info.last_user_frame=now+frames+lag;
            }
        }
        audio.info.queued_frames=audio.used;ReleaseSRWLockExclusive(&lock);SetEvent(audio.progress);
    }
done:
    if(FAILED(hr))failed(error,hr);
    if(started){HRESULT stopped=IAudioClient_Stop(client);if(FAILED(stopped))failed(JP_AUDIO_IO,stopped);}
    if(volume)ISimpleAudioVolume_Release(volume);
    if(clock)IAudioClock_Release(clock);
    if(render)IAudioRenderClient_Release(render);
    if(client)IAudioClient_Release(client);
    if(device)IMMDevice_Release(device);
    if(enumerator)IMMDeviceEnumerator_Release(enumerator);
    if(initialized)CoUninitialize();
    AcquireSRWLockExclusive(&lock);audio.info.worker_live=0;ReleaseSRWLockExclusive(&lock);
    SetEvent(audio.ready);SetEvent(audio.progress);return 0;
}
int jp_audio_open(const struct jp_audio_config *config)
{
    int rc=access_owner();if(rc)return rc;
    if(!config||config->bytes!=sizeof *config||(config->rate!=44100&&config->rate!=48000)||
       config->capacity_frames<4096||config->capacity_frames>1048576||config->latency_ms<20||config->latency_ms>500||
       !config->timeout_ms||config->timeout_ms>60000||config->muted>1||!isfinite(config->gain)||config->gain<0||config->gain>1||config->reserved)return JP_AUDIO_ARGUMENT;
    AcquireSRWLockExclusive(&lock);
    if(audio.owner||audio.worker||audio.ring){rc=audio.owner!=GetCurrentThreadId()?JP_AUDIO_THREAD:JP_AUDIO_STATE;ReleaseSRWLockExclusive(&lock);return rc;}
    audio.config=*config;audio.owner=GetCurrentThreadId();audio.read=audio.used=0;
    audio.info=(struct jp_audio_info){.state=JP_AUDIO_OPENING,.rate=config->rate,.capacity_frames=config->capacity_frames,.channels=2,.creator_thread=audio.owner,.source_hash=FNV_OFFSET};
    ReleaseSRWLockExclusive(&lock);
    audio.ring=VirtualAlloc(NULL,(size_t)config->capacity_frames*8,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
    if(!audio.ring){failed(JP_AUDIO_MEMORY,HRESULT_FROM_WIN32(GetLastError()));return JP_AUDIO_MEMORY;}
    audio.stop=CreateEventW(NULL,TRUE,FALSE,NULL);audio.render=CreateEventW(NULL,FALSE,FALSE,NULL);
    audio.ready=CreateEventW(NULL,TRUE,FALSE,NULL);audio.progress=CreateEventW(NULL,FALSE,FALSE,NULL);
    if(!audio.stop||!audio.render||!audio.ready||!audio.progress){failed(JP_AUDIO_WORKER,HRESULT_FROM_WIN32(GetLastError()));return JP_AUDIO_WORKER;}
    audio.info.worker_live=1;audio.worker=CreateThread(NULL,0,worker,NULL,0,NULL);
    if(!audio.worker){audio.info.worker_live=0;failed(JP_AUDIO_WORKER,HRESULT_FROM_WIN32(GetLastError()));return JP_AUDIO_WORKER;}
    DWORD wait=WaitForSingleObject(audio.ready,config->timeout_ms);
    if(wait==WAIT_TIMEOUT)return JP_AUDIO_TIMEOUT;
    if(wait!=WAIT_OBJECT_0)return JP_AUDIO_WORKER;
    AcquireSRWLockShared(&lock);rc=audio.info.state==JP_AUDIO_READY?0:audio.info.status?(int)audio.info.status:JP_AUDIO_STATE;ReleaseSRWLockShared(&lock);return rc;
}
int jp_audio_submit(const float *samples,uint32_t frames)
{
    int rc=access_owner();if(rc)return rc;
    AcquireSRWLockExclusive(&lock);
    rc=access_locked();if(rc)goto done;
    if(!samples||!frames||frames>audio.config.capacity_frames){rc=JP_AUDIO_ARGUMENT;goto done;}
    for(size_t j=0;j<(size_t)frames*2;j++)if(!isfinite(samples[j])){rc=JP_AUDIO_ARGUMENT;goto done;}
    if(audio.info.state!=JP_AUDIO_READY){rc=audio.info.status?(int)audio.info.status:JP_AUDIO_STATE;goto done;}
    if(frames>audio.config.capacity_frames-audio.used){rc=JP_AUDIO_QUEUE;goto done;}
    if(audio.info.submitted>UINT64_MAX-frames){rc=JP_AUDIO_STATE;goto done;}
    uint32_t at=(audio.read+audio.used)%audio.config.capacity_frames,n=audio.config.capacity_frames-at;if(n>frames)n=frames;
    memcpy(audio.ring+(size_t)at*2,samples,(size_t)n*8);
    if(n<frames)memcpy(audio.ring,samples+(size_t)n*2,(size_t)(frames-n)*8);
    audio.used+=frames;audio.info.submitted+=frames;audio.info.queued_frames=audio.used;rc=0;
done:ReleaseSRWLockExclusive(&lock);return rc;
}
int jp_audio_drain(uint32_t timeout)
{
    int rc=access_owner();if(rc)return rc;if(timeout>60000)return JP_AUDIO_ARGUMENT;
    uint64_t begin=GetTickCount64();
    for(;;){
        AcquireSRWLockShared(&lock);rc=access_locked();struct jp_audio_info info=audio.info;ReleaseSRWLockShared(&lock);if(rc)return rc;
        if(info.state!=JP_AUDIO_READY)return info.status?(int)info.status:JP_AUDIO_STATE;
        if(!info.queued_frames&&info.clock_frequency&&sample_clock(info.clock_position,info.clock_frequency,info.rate)>=info.last_user_frame)return 0;
        uint64_t elapsed=GetTickCount64()-begin;if(elapsed>=timeout)return JP_AUDIO_TIMEOUT;
        DWORD wait=WaitForSingleObject(audio.progress,(DWORD)(timeout-elapsed));
        if(wait==WAIT_FAILED)return JP_AUDIO_WORKER;
    }
}
int jp_audio_discard_queued(void)
{
    int rc=access_owner();if(rc)return rc;AcquireSRWLockExclusive(&lock);
    rc=access_locked();if(rc){ReleaseSRWLockExclusive(&lock);return rc;}
    if(audio.info.state!=JP_AUDIO_READY)rc=audio.info.status?(int)audio.info.status:JP_AUDIO_STATE;
    else{audio.info.discarded+=audio.used;audio.used=0;audio.info.queued_frames=0;rc=0;}
    ReleaseSRWLockExclusive(&lock);return rc;
}
int jp_audio_close(uint32_t timeout)
{
    int rc=access_owner();if(rc)return rc;if(timeout>60000)return JP_AUDIO_ARGUMENT;
    AcquireSRWLockExclusive(&lock);rc=access_locked();if(rc||!audio.owner){ReleaseSRWLockExclusive(&lock);return rc;}
    audio.info.state=JP_AUDIO_CLOSING;ReleaseSRWLockExclusive(&lock);
    if(audio.stop)SetEvent(audio.stop);
    if(audio.worker){DWORD wait=WaitForSingleObject(audio.worker,timeout);if(wait==WAIT_TIMEOUT)return JP_AUDIO_TIMEOUT;if(wait!=WAIT_OBJECT_0)return JP_AUDIO_WORKER;}
    HANDLE *handles[]={&audio.worker,&audio.stop,&audio.render,&audio.ready,&audio.progress};
    for(unsigned j=0;j<5;j++)if(*handles[j]){if(!CloseHandle(*handles[j]))return JP_AUDIO_WORKER;*handles[j]=NULL;}
    if(audio.ring){if(!VirtualFree(audio.ring,0,MEM_RELEASE))return JP_AUDIO_MEMORY;audio.ring=NULL;}
    AcquireSRWLockExclusive(&lock);audio.info.discarded+=audio.used;audio.used=0;audio.info.queued_frames=0;
    audio.info.state=JP_AUDIO_CLOSED;audio.info.worker_live=0;audio.owner=0;ReleaseSRWLockExclusive(&lock);return 0;
}
int jp_audio_status(void)
{int rc=access_owner();if(rc)return rc;AcquireSRWLockShared(&lock);rc=access_locked();if(!rc)rc=audio.info.state==JP_AUDIO_READY?0:audio.info.status?(int)audio.info.status:JP_AUDIO_STATE;ReleaseSRWLockShared(&lock);return rc;}
int jp_audio_snapshot(struct jp_audio_info *out)
{int rc=access_owner();if(rc)return rc;if(!out)return JP_AUDIO_ARGUMENT;AcquireSRWLockShared(&lock);rc=access_locked();if(!rc)*out=audio.info;ReleaseSRWLockShared(&lock);return rc;}
