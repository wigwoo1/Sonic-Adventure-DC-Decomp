#include "ta_stream.h"
/* Host allocation only. The core stream and its allocator interface are portable. */
__declspec(dllimport) void *__stdcall VirtualAlloc(void *,size_t,unsigned long,unsigned long);
__declspec(dllimport) int __stdcall VirtualFree(void *,size_t,unsigned long);
static void *allocate(void *context,size_t bytes){(void)context;return VirtualAlloc(0,bytes,0x3000u,4u);}
static void release(void *context,void *p){(void)context;if(p)(void)VirtualFree(p,0,0x8000u);}
SA_STAGE_EXPORT struct sa_ta_stream *sa_ta_stream_create_win32(void){return sa_ta_stream_create(allocate,release,0);}
