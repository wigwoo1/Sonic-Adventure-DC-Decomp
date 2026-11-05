/* Authored test consumer: original task destruction invokes this C callback,
 * which validates/frees a real SDK allocation before the heap's final close. */
#include <shinobi.h>
__declspec(dllimport) int jp_sdk_heap_status(void);
static void *payload;
static Uint32 bytes,calls,failures;
static void (*observer)(void *);
__declspec(dllexport) int jp_sdk_fixture_prepare(Uint32 size)
{
    if(payload || !size || size>4096)return 1;
    payload=syMalloc(size);if(!payload || jp_sdk_heap_status())return 2;
    bytes=size;for(Uint32 i=0;i<size;i++)((Uint8 *)payload)[i]=(Uint8)(i*37+11);
    return 0;
}
__declspec(dllexport) void jp_sdk_fixture_observe(void (*callback)(void *))
{observer=callback;}
__declspec(dllexport) void jp_sdk_fixture_destructor(void *task)
{
    calls++;
    if(!payload)failures++;
    else{
        Uint32 total,largest;syMallocStat(&total,&largest);
        if(jp_sdk_heap_status() || total<largest)failures++;
        for(Uint32 i=0;i<bytes;i++)if(((Uint8 *)payload)[i]!=(Uint8)(i*37+11)){failures++;break;}
        syFree(payload);if(jp_sdk_heap_status())failures++;payload=0;
    }
    if(observer)observer(task);
}
__declspec(dllexport) void jp_sdk_fixture_snapshot(Uint32 *out)
{out[0]=calls;out[1]=failures;out[2]=payload!=0;out[3]=bytes;}
