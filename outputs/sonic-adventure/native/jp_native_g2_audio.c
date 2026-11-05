#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <string.h>
#define JP_AICA_API __declspec(dllimport)
#include "jp_native_aica.h"
#include "jp_native_g2_audio.h"

static SRWLOCK lock=SRWLOCK_INIT;
static CRITICAL_SECTION gate;
static struct jp_g2_info info;
static unsigned char *staging;
static uintptr_t core_low,core_high;
static struct {
    uint32_t bus,bytes,direction;
    void *host;
    jp_g2_callback callback;
    void *context;
} channel;

static int result(int error)
{ info.status=(uint32_t)error;if(error)info.failures++;return error; }
static int owner(void)
{
    if(!info.ready)return JP_G2_STATE;
    if(info.creator_thread!=GetCurrentThreadId())return JP_G2_THREAD;
    return JP_G2_OK;
}
static int core_ready(void)
{
    struct jp_aica_info core;
    int error=jp_aica_snapshot(&core);
    info.core_status=(uint32_t)error;
    return error||core.phase!=JP_AICA_READY||!(core.resources&1)?JP_G2_CORE:0;
}
/* Validate the complete traversal before reading or writing any caller data. */
static int span(uintptr_t address,uint32_t width,uint32_t count,uint32_t mode,
    uintptr_t *low,uintptr_t *high)
{
    uintptr_t distance=(uintptr_t)(count-1)*width;
    if(mode>2||!address)return 0;
    if(mode==2) { if(address<distance)return 0;*low=address-distance; }
    else *low=address;
    uintptr_t last=mode==1?address+distance:address;
    if(last<address||last>UINTPTR_MAX-width)return 0;
    *high=last+width;return 1;
}
static int overlap(uintptr_t a,uintptr_t b,uintptr_t c,uintptr_t d)
{ return a<d&&c<b; }
static int validate(uint32_t bus,const void *host,uint32_t width,int32_t count,
    uint32_t bus_mode,uint32_t host_mode,int *registers)
{
    uintptr_t low,high,h_low,h_high;
    if((width!=1&&width!=2&&width!=4)||count<=0||
       (uint32_t)count>JP_AICA_RAM_BYTES/width||
       !span(bus,width,(uint32_t)count,bus_mode,&low,&high)||
       !span((uintptr_t)host,width,(uint32_t)count,host_mode,&h_low,&h_high))return JP_G2_ARGUMENT;
    if(overlap(h_low,h_high,(uintptr_t)staging,(uintptr_t)staging+JP_AICA_RAM_BYTES)||
       overlap(h_low,h_high,core_low,core_high))return JP_G2_ARGUMENT;
    if(low>=JP_G2_SOUND_RAM&&high<=JP_G2_SOUND_RAM+JP_AICA_RAM_BYTES) {
        *registers=0;return 0;
    }
    if(low>=JP_G2_REGISTERS&&high<=JP_G2_REGISTERS+0x10000u) {
        if(width!=4||count!=1||(bus&3))return JP_G2_UNSUPPORTED;
        *registers=1;return 0;
    }
    return JP_G2_ARGUMENT;
}
static uintptr_t step(uintptr_t address,uint32_t width,uint32_t index,uint32_t mode)
{
    uintptr_t offset=(uintptr_t)width*index;
    return mode==1?address+offset:mode==2?address-offset:address;
}
static void reverse_units(uint32_t width,uint32_t count)
{
    unsigned char temporary[4];
    for(uint32_t i=0;i<count/2;i++) {
        unsigned char *first=staging+(size_t)i*width;
        unsigned char *last=staging+(size_t)(count-1-i)*width;
        memcpy(temporary,first,width);memcpy(first,last,width);memcpy(last,temporary,width);
    }
}
/* Caller holds lock. CPU execution cannot run concurrently on the borrowed
 * creator-thread core. Gather/scatter preserves ordered FIX and SUB semantics. */
static int copy_bus(uint32_t bus,void *host,uint32_t width,int32_t count,
    uint32_t source_mode,uint32_t destination_mode,int write)
{
    int registers=0,error=validate(bus,host,width,count,
        write?destination_mode:source_mode,write?source_mode:destination_mode,&registers);
    if(error)return error;
    if((error=core_ready())!=0)return error;
    uint32_t bytes=(uint32_t)count*width,value=0;
    if(registers) {
        if(write) {
            memcpy(&value,host,4);
            error=jp_aica_write_register(bus-JP_G2_REGISTERS,value);
        } else {
            error=jp_aica_read_register(bus-JP_G2_REGISTERS,&value);
            if(!error)memcpy(host,&value,4);
        }
    } else if(write) {
        if(source_mode==1)memcpy(staging,host,bytes);
        else for(uint32_t i=0;i<(uint32_t)count;i++)
            memcpy(staging+(size_t)i*width,(void *)step((uintptr_t)host,width,i,source_mode),width);
        uint32_t low=bus-JP_G2_SOUND_RAM;
        if(destination_mode==2) {low-=(uint32_t)(count-1)*width;reverse_units(width,(uint32_t)count);}
        error=destination_mode==0?
            jp_aica_write(low,staging+bytes-width,width):jp_aica_write(low,staging,bytes);
    } else {
        uint32_t low=bus-JP_G2_SOUND_RAM;
        if(source_mode==2)low-=(uint32_t)(count-1)*width;
        error=jp_aica_read(low,staging,source_mode==0?width:bytes);
        if(!error) {
            if(source_mode==0)for(uint32_t i=1;i<(uint32_t)count;i++)memcpy(staging+(size_t)i*width,staging,width);
            if(source_mode==2)reverse_units(width,(uint32_t)count);
            if(destination_mode==1)memcpy(host,staging,bytes);
            else if(destination_mode==0)memcpy(host,staging+bytes-width,width);
            else for(uint32_t i=0;i<(uint32_t)count;i++)
                memcpy((void *)step((uintptr_t)host,width,i,destination_mode),staging+(size_t)i*width,width);
        }
    }
    info.core_status=(uint32_t)error;
    if(error)return JP_G2_CORE;
    info.bytes_copied+=bytes;return 0;
}

int jp_g2_open(void)
{
    AcquireSRWLockExclusive(&lock);
    int error=0;
    if(info.ready)error=info.creator_thread==GetCurrentThreadId()?JP_G2_STATE:JP_G2_THREAD;
    else {
        struct jp_aica_info core;
        int core_error=jp_aica_snapshot(&core);
        if(core_error||core.phase!=JP_AICA_READY||!(core.resources&1))error=JP_G2_CORE;
        else if(!InitializeCriticalSectionEx(&gate,0,0))error=JP_G2_MEMORY;
        else {
            staging=VirtualAlloc(NULL,JP_AICA_RAM_BYTES,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
            if(!staging) {DeleteCriticalSection(&gate);error=JP_G2_MEMORY;}
            else {
                SYSTEM_INFO system;GetSystemInfo(&system);uintptr_t page=system.dwPageSize;
                core_low=(uintptr_t)core.state_address-page;
                core_high=((uintptr_t)core.state_address+(uintptr_t)core.state_bytes+page-1)/page*page+page;
                memset(&info,0,sizeof info);memset(&channel,0,sizeof channel);
                info.ready=1;info.creator_thread=GetCurrentThreadId();
            }
        }
    }
    if(error!=JP_G2_THREAD)result(error);
    ReleaseSRWLockExclusive(&lock);return error;
}
int jp_g2_close(void)
{
    AcquireSRWLockExclusive(&lock);int error=owner();
    if(!info.ready)error=0;
    else if(!error&&(info.busy||info.in_callback||info.gate_level))error=JP_G2_BUSY;
    else if(!error) {
        if(!VirtualFree(staging,0,MEM_RELEASE))error=JP_G2_MEMORY;
        else {
            staging=NULL;DeleteCriticalSection(&gate);
            info.ready=0;info.channel_open=0;info.configured=0;memset(&channel,0,sizeof channel);
        }
    }
    if(error!=JP_G2_THREAD)result(error);
    ReleaseSRWLockExclusive(&lock);return error;
}
static int cpu_copy(uint32_t bus,void *host,uint32_t width,int32_t count,
    uint32_t source_mode,uint32_t destination_mode,int write)
{
    AcquireSRWLockExclusive(&lock);int error=owner();
    if(!error)error=copy_bus(bus,host,width,count,source_mode,destination_mode,write);
    if(!error) {if(write)info.cpu_writes++;else info.cpu_reads++;}
    if(error!=JP_G2_THREAD)result(error);
    ReleaseSRWLockExclusive(&lock);return error;
}
int jp_g2_read(uint32_t b,void *h,uint32_t w,int32_t n,uint32_t s,uint32_t d)
{return cpu_copy(b,h,w,n,s,d,0);}
int jp_g2_write(const void *h,uint32_t b,uint32_t w,int32_t n,uint32_t s,uint32_t d)
{return cpu_copy(b,(void *)h,w,n,s,d,1);}
int jp_g2_dma_open(void **handle,uint32_t type)
{
    AcquireSRWLockExclusive(&lock);int error=owner();
    if(!error&&!handle)error=JP_G2_ARGUMENT;
    if(!error&&type)error=JP_G2_UNSUPPORTED;
    if(!error&&info.channel_open)error=JP_G2_STATE;
    if(!error) {info.channel_open=1;*handle=&channel;}
    if(error!=JP_G2_THREAD)result(error);
    ReleaseSRWLockExclusive(&lock);return error;
}
static int valid_channel(void *handle)
{return info.channel_open&&handle==&channel;}
int jp_g2_dma_prepare(void *handle,uint32_t bus,void *host,int32_t bytes,
    uint32_t direction,uint32_t transfer_control,uint32_t suspend_mode,
    jp_g2_callback callback,void *context)
{
    AcquireSRWLockExclusive(&lock);int error=owner(),registers=0;
    if(!error&&(!valid_channel(handle)||direction>1||transfer_control>1||suspend_mode>1||
        bytes<=0||((uintptr_t)host&31)||(bus&31)||(bytes&31)))error=JP_G2_ARGUMENT;
    if(!error&&info.busy)error=JP_G2_BUSY;
    if(!error)error=validate(bus,host,4,bytes/4,1,1,&registers);
    if(!error&&registers)error=JP_G2_UNSUPPORTED;
    if(!error)error=core_ready();
    if(!error) {
        channel.bus=bus;channel.host=host;channel.bytes=(uint32_t)bytes;
        channel.direction=direction;channel.callback=callback;channel.context=context;info.configured=1;
    }
    if(error!=JP_G2_THREAD)result(error);
    ReleaseSRWLockExclusive(&lock);return error;
}
int jp_g2_dma_trigger(void *handle,uint32_t trigger)
{
    AcquireSRWLockExclusive(&lock);int error=owner();
    if(!error&&(!valid_channel(handle)||!info.configured))error=JP_G2_ARGUMENT;
    if(!error&&trigger)error=JP_G2_UNSUPPORTED;
    if(!error&&info.busy)error=JP_G2_BUSY;
    if(!error)error=copy_bus(channel.bus,channel.host,4,(int32_t)(channel.bytes/4),1,1,!channel.direction);
    if(!error) {info.busy=1;info.dma_started++;}
    if(error!=JP_G2_THREAD)result(error);
    ReleaseSRWLockExclusive(&lock);return error;
}
int jp_g2_dma_busy(void *handle,uint32_t *busy)
{
    AcquireSRWLockExclusive(&lock);int error=owner();
    if(!error&&(!valid_channel(handle)||!busy))error=JP_G2_ARGUMENT;
    if(!error)*busy=info.busy;
    if(error!=JP_G2_THREAD)result(error);
    ReleaseSRWLockExclusive(&lock);return error;
}
int jp_g2_service(uint32_t budget,uint32_t *completed)
{
    AcquireSRWLockExclusive(&lock);int error=owner();uint32_t count=0;
    if(!error&&(!completed||!budget||budget>64))error=JP_G2_ARGUMENT;
    if(!error&&(info.in_callback||info.gate_level))error=JP_G2_BUSY;
    while(!error&&count<budget&&info.busy) {
        jp_g2_callback callback=channel.callback;void *context=channel.context;
        info.busy=0;info.dma_ended++;count++;
        if(callback) {
            info.in_callback=1;info.callbacks++;
            ReleaseSRWLockExclusive(&lock);callback(context);AcquireSRWLockExclusive(&lock);
            info.in_callback=0;
            if(info.gate_level)error=JP_G2_BUSY;
        }
    }
    if(!error)*completed=count;
    if(error!=JP_G2_THREAD)result(error);
    ReleaseSRWLockExclusive(&lock);return error;
}
int jp_g2_gate_get(void)
{
    AcquireSRWLockExclusive(&lock);int error=owner();
    int level=error?-error:(int)info.gate_level;
    ReleaseSRWLockExclusive(&lock);return level;
}
int jp_g2_gate_set(int level)
{
    AcquireSRWLockExclusive(&lock);int error=owner();
    if(!error&&(level<0||level>15))error=JP_G2_ARGUMENT;
    if(!error) {
        if(!info.gate_level&&level)EnterCriticalSection(&gate);
        if(info.gate_level&&!level)LeaveCriticalSection(&gate);
        info.gate_level=(uint32_t)level;
    }
    if(error!=JP_G2_THREAD)result(error);
    ReleaseSRWLockExclusive(&lock);return error;
}
int jp_g2_snapshot(struct jp_g2_info *out)
{
    AcquireSRWLockExclusive(&lock);
    int error=info.ready&&info.creator_thread!=GetCurrentThreadId()?JP_G2_THREAD:0;
    if(!error&&!out)error=JP_G2_ARGUMENT;
    if(!error)*out=info;
    ReleaseSRWLockExclusive(&lock);return error;
}
