/* Bounded host ownership around the actual SDK allocation/free algorithms. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include "jp_native_sdk_heap.h"
#define JP_SDK_HEAP_HOST
#include "jp_native_sdk_heap_internal.h"
_Static_assert(sizeof(unsigned long)==4,"SDK scalar widths stay 32-bit");
enum {UNIT=32,MAX_SPAN=64u<<20};
static SRWLOCK lock=SRWLOCK_INIT;
static struct {
    void *arena,*reservation;uintptr_t base;
    struct jp_sdk_heap_record *records;
    uint32_t span,usable,units,live,requested,capacity;
    DWORD owner;int initialized,corrupt,status,exit_started;uint16_t mask;uint64_t exit_token;
} heap={.status=JP_SDK_HEAP_NOT_INITIALIZED};
static uint64_t next_exit_token=1;
uint16_t jp_sdk_heap_mask_get(void){return heap.mask;}
void jp_sdk_heap_mask_set(uint16_t value){heap.mask=value;}
/* The source's mask bookkeeping is private to this heap and always runs under
 * the SRW lock. It does not implement or export console interrupt primitives. */
static int result(int code){heap.status=code;return code;}
static int ready(void)
{
    if(!heap.initialized)return JP_SDK_HEAP_NOT_INITIALIZED;
    if(heap.corrupt)return JP_SDK_HEAP_CORRUPT;
    if(heap.mask || jp_sdk_source_validate(heap.base,heap.usable,heap.records)){
        heap.corrupt=1;return JP_SDK_HEAP_CORRUPT;
    }
    return 0;
}
static int overlap(uintptr_t a,size_t n,uintptr_t b,size_t m)
{return n && m && a<=UINTPTR_MAX-n && b<=UINTPTR_MAX-m && a<b+m && b<a+n;}
static int output_valid(const void *pointer,size_t bytes)
{
    uintptr_t at=(uintptr_t)pointer;
    return pointer && at<=UINTPTR_MAX-bytes &&
        (!heap.initialized || (!overlap(at,bytes,(uintptr_t)heap.arena,heap.span) &&
        !overlap(at,bytes,(uintptr_t)heap.records,(size_t)heap.units*sizeof *heap.records)));
}
static int initialize(void *arena,uint32_t span,void *reservation)
{
    if(heap.initialized || heap.exit_token)return JP_SDK_HEAP_STATE;
    uintptr_t start=(uintptr_t)arena;
    if(!arena || span<64 || span>MAX_SPAN || start>UINTPTR_MAX-span)return JP_SDK_HEAP_ARGUMENT;
    uintptr_t base=(start+UNIT)&~(uintptr_t)(UNIT-1);
    uint32_t usable=(span-(uint32_t)(base-start))&~(uint32_t)(UNIT-1),units=usable/UNIT;
    if(!units)return JP_SDK_HEAP_ARGUMENT;
    struct jp_sdk_heap_record *records=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(size_t)units*sizeof *records);
    if(!records)return JP_SDK_HEAP_MEMORY;
    heap.arena=arena;heap.base=base;heap.reservation=reservation;heap.records=records;
    heap.span=span;heap.usable=usable;heap.units=units;heap.live=heap.requested=heap.capacity=0;
    heap.owner=GetCurrentThreadId();heap.initialized=1;heap.corrupt=0;heap.mask=0;
    /* Original init skips alignment itself. Pass only the bounded usable size,
     * instead of its historical count which could extend beyond the span. */
    jp_sdk_source_init(arena,(unsigned long)usable);
    return ready();
}
int jp_sdk_heap_initialize(void *arena,uint32_t span)
{AcquireSRWLockExclusive(&lock);int rc=initialize(arena,span,NULL);result(rc);ReleaseSRWLockExclusive(&lock);return rc;}
int jp_sdk_heap_create(uint32_t span)
{
    AcquireSRWLockExclusive(&lock);int rc=0;
    if(heap.initialized || heap.exit_token){rc=JP_SDK_HEAP_STATE;goto done;}
    if(span<64 || span>MAX_SPAN){rc=JP_SDK_HEAP_ARGUMENT;goto done;}
    SYSTEM_INFO info;GetSystemInfo(&info);size_t page=info.dwPageSize;
    size_t committed=((size_t)span+page-1)/page*page,reserved=committed+2*page;
    void *area=VirtualAlloc(NULL,reserved,MEM_RESERVE,PAGE_NOACCESS);
    if(!area){rc=JP_SDK_HEAP_MEMORY;goto done;}
    unsigned char *memory=(unsigned char *)area+page;
    if(!VirtualAlloc(memory,committed,MEM_COMMIT,PAGE_READWRITE)){VirtualFree(area,0,MEM_RELEASE);rc=JP_SDK_HEAP_MEMORY;goto done;}
    /* The declared span ends exactly at the inaccessible trailing page. */
    rc=initialize(memory+committed-span,span,area);
    if(rc && !heap.initialized)VirtualFree(area,0,MEM_RELEASE);
done:result(rc);ReleaseSRWLockExclusive(&lock);return rc;
}
static int close_heap(void)
{
    if(!heap.initialized)return 0;
    if(heap.owner!=GetCurrentThreadId())return JP_SDK_HEAP_THREAD;
    if(heap.live)return JP_SDK_HEAP_ACTIVE;
    if(heap.reservation && !VirtualFree(heap.reservation,0,MEM_RELEASE))return JP_SDK_HEAP_MEMORY;
    jp_sdk_source_reset();HeapFree(GetProcessHeap(),0,heap.records);
    DWORD creator=heap.owner;uint64_t token=heap.exit_token;int started=heap.exit_started;
    memset(&heap,0,sizeof heap);
    if(token){heap.owner=creator;heap.exit_token=token;heap.exit_started=started;}
    return 0;
}
int jp_sdk_heap_shutdown(void)
{
    AcquireSRWLockExclusive(&lock);
    int rc=heap.exit_token?(heap.owner==GetCurrentThreadId()?JP_SDK_HEAP_STATE:JP_SDK_HEAP_THREAD):close_heap();
    result(rc);ReleaseSRWLockExclusive(&lock);return rc;
}
static int request(size_t bytes)
{
    if(bytes>UINT32_MAX-UNIT*2u)return JP_SDK_HEAP_OVERFLOW;
    if(bytes>heap.usable-UNIT)return JP_SDK_HEAP_EXHAUSTED;
    return 0;
}
static int index_of(void *pointer,uint32_t *index)
{
    uintptr_t p=(uintptr_t)pointer,end=heap.base+heap.usable;
    if(p<heap.base+UNIT || p>end || (p-heap.base)%UNIT)return JP_SDK_HEAP_POINTER;
    *index=(uint32_t)((p-heap.base)/UNIT-1);
    return heap.records[*index].units?0:JP_SDK_HEAP_POINTER;
}
static void *allocate(size_t count,size_t bytes,int zero)
{
    int rc=ready();if(rc){result(rc);return NULL;}
    if(count && bytes>SIZE_MAX/count){result(JP_SDK_HEAP_OVERFLOW);return NULL;}
    size_t requested=count*bytes;rc=request(requested);if(rc){result(rc);return NULL;}
    void *pointer=zero?jp_sdk_source_calloc(count,bytes):jp_sdk_source_syMalloc((unsigned long)requested);
    if(!pointer){result(JP_SDK_HEAP_EXHAUSTED);return NULL;}
    uintptr_t at=(uintptr_t)pointer;
    if(at<heap.base+UNIT || at>heap.base+heap.usable || (at-heap.base)%UNIT){heap.corrupt=1;result(JP_SDK_HEAP_CORRUPT);return NULL;}
    uint32_t index=(uint32_t)((at-heap.base)/UNIT-1),units=jp_sdk_source_units(pointer),capacity=(units-1)*UNIT;
    if(!units || units>heap.units-index || heap.records[index].units || requested>capacity){heap.corrupt=1;result(JP_SDK_HEAP_CORRUPT);return NULL;}
    heap.records[index]=(struct jp_sdk_heap_record){units,(uint32_t)requested};heap.live++;heap.requested+=(uint32_t)requested;heap.capacity+=capacity;
    rc=ready();result(rc);return rc?NULL:pointer;
}
void *jp_sdk_heap_allocate(size_t bytes)
{AcquireSRWLockExclusive(&lock);void *p=allocate(1,bytes,0);ReleaseSRWLockExclusive(&lock);return p;}
void *jp_sdk_heap_callocate(size_t count,size_t bytes)
{AcquireSRWLockExclusive(&lock);void *p=allocate(count,bytes,1);ReleaseSRWLockExclusive(&lock);return p;}
static int release(void *pointer)
{
    if(!pointer)return 0;int rc=ready();if(rc)return rc;uint32_t index;
    rc=index_of(pointer,&index);if(rc)return rc;
    struct jp_sdk_heap_record record=heap.records[index];
    jp_sdk_source_syFree(pointer);heap.records[index]=(struct jp_sdk_heap_record){0};
    heap.live--;heap.requested-=record.requested;heap.capacity-=(record.units-1)*UNIT;return ready();
}
int jp_sdk_heap_free(void *pointer)
{AcquireSRWLockExclusive(&lock);int rc=release(pointer);result(rc);ReleaseSRWLockExclusive(&lock);return rc;}
void *jp_sdk_heap_reallocate(void *pointer,size_t bytes)
{
    AcquireSRWLockExclusive(&lock);void *out=NULL;int rc=0;uint32_t index;
    if(!pointer){out=allocate(1,bytes,0);goto done;}
    rc=ready();if(rc)goto failed;rc=index_of(pointer,&index);if(rc)goto failed;
    if(!bytes){rc=release(pointer);goto failed;}
    rc=request(bytes);if(rc)goto failed;
    struct jp_sdk_heap_record old=heap.records[index];
    if(bytes<=(size_t)(old.units-1)*UNIT){
        heap.requested-=old.requested;heap.requested+=(uint32_t)bytes;heap.records[index].requested=(uint32_t)bytes;out=pointer;result(0);goto done;
    }
    out=allocate(1,bytes,0);if(!out)goto done;
    memmove(out,pointer,old.requested<bytes?old.requested:bytes);
    rc=release(pointer);if(rc){out=NULL;goto failed;}result(0);goto done;
failed:result(rc);
done:ReleaseSRWLockExclusive(&lock);return out;
}
int jp_sdk_heap_statistics(uint32_t *total,uint32_t *largest)
{
    AcquireSRWLockExclusive(&lock);int rc=ready();
    if(!rc && (!output_valid(total,4) || !output_valid(largest,4) || overlap((uintptr_t)total,4,(uintptr_t)largest,4)))rc=JP_SDK_HEAP_ARGUMENT;
    if(!rc){unsigned long whole,big;jp_sdk_source_stat(&whole,&big);*total=(uint32_t)whole;*largest=(uint32_t)big;}
    result(rc);ReleaseSRWLockExclusive(&lock);return rc;
}
int jp_sdk_heap_snapshot(struct jp_sdk_heap_info *out)
{
    AcquireSRWLockExclusive(&lock);int rc=0;
    if(!output_valid(out,sizeof *out))rc=JP_SDK_HEAP_ARGUMENT;
    if(!rc && heap.initialized)rc=ready();
    if(!rc){unsigned long whole=0,big=0;if(heap.initialized)jp_sdk_source_stat(&whole,&big);
        *out=(struct jp_sdk_heap_info){(uint64_t)(uintptr_t)heap.arena,(uint64_t)heap.base,(uint64_t)(uintptr_t)heap.reservation,
            heap.span,heap.usable,heap.units,heap.live,heap.requested,heap.capacity,(uint32_t)whole,(uint32_t)big,
            heap.reservation!=NULL,(uint32_t)heap.status,UNIT,jp_sdk_source_header_bytes(),heap.initialized!=0,0};}
    ReleaseSRWLockExclusive(&lock);return rc;
}
int jp_sdk_heap_status(void)
{AcquireSRWLockExclusive(&lock);int code=heap.status;ReleaseSRWLockExclusive(&lock);return code;}
int jp_sdk_heap_exit_claim(uint64_t *token)
{
    AcquireSRWLockExclusive(&lock);int rc=0;
    if(!output_valid(token,sizeof *token))rc=JP_SDK_HEAP_ARGUMENT;
    else if(heap.owner && heap.owner!=GetCurrentThreadId())rc=JP_SDK_HEAP_THREAD;
    else if(heap.exit_token || !next_exit_token)rc=JP_SDK_HEAP_STATE;
    else if((rc=ready())==0){heap.exit_token=next_exit_token++;heap.exit_started=0;*token=heap.exit_token;}
    result(rc);ReleaseSRWLockExclusive(&lock);return rc;
}
static int exit_access(uint64_t token)
{
    if(heap.owner && heap.owner!=GetCurrentThreadId())return JP_SDK_HEAP_THREAD;
    return !token || token!=heap.exit_token?JP_SDK_HEAP_STATE:0;
}
int jp_sdk_heap_exit_cancel(uint64_t token)
{
    AcquireSRWLockExclusive(&lock);int rc=exit_access(token);
    if(!rc && heap.exit_started)rc=JP_SDK_HEAP_STATE;
    if(!rc)heap.exit_token=0;
    result(rc);ReleaseSRWLockExclusive(&lock);return rc;
}
int jp_sdk_heap_exit_finish(uint64_t token)
{
    AcquireSRWLockExclusive(&lock);int rc=exit_access(token);
    if(!rc){heap.exit_started=1;rc=close_heap();}
    result(rc);ReleaseSRWLockExclusive(&lock);return rc;
}
/* Exact Windows SDK scalar ABI; these do not replace native CRT symbols. */
__declspec(dllexport) void syMallocInit(void *arena,unsigned long span){(void)jp_sdk_heap_initialize(arena,(uint32_t)span);}
__declspec(dllexport) void syMallocFinish(void){(void)jp_sdk_heap_shutdown();}
__declspec(dllexport) void *syMalloc(unsigned long bytes){return jp_sdk_heap_allocate((size_t)bytes);}
__declspec(dllexport) void syFree(void *pointer){(void)jp_sdk_heap_free(pointer);}
__declspec(dllexport) void syMallocStat(unsigned long *whole,unsigned long *largest)
{
    if(!whole || !largest){AcquireSRWLockExclusive(&lock);result(JP_SDK_HEAP_ARGUMENT);ReleaseSRWLockExclusive(&lock);return;}
    /* Same 32-bit representation, with values copied rather than aliasing
     * uint32_t objects through a different C scalar type. */
    AcquireSRWLockExclusive(&lock);int rc=ready();
    if(!rc && (!output_valid(whole,4) || !output_valid(largest,4) || overlap((uintptr_t)whole,4,(uintptr_t)largest,4)))rc=JP_SDK_HEAP_ARGUMENT;
    if(!rc)jp_sdk_source_stat(whole,largest);result(rc);ReleaseSRWLockExclusive(&lock);
}
