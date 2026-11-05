#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>
#include "jp_native_gdfs_internal.h"
static wchar_t path[32768];
uint32_t jp_gdfs_host_thread(void){return GetCurrentThreadId();}
uint64_t jp_gdfs_host_time(void){return GetTickCount64();}
void jp_gdfs_host_yield(void){Sleep(1);}
int jp_gdfs_host_path(const wchar_t *value)
{
    if(!value||!*value)return JP_GDFS_ARGUMENT;
    wchar_t result[32768];DWORD n=GetFullPathNameW(value,32768,result,NULL);
    if(!n||n>=32768)return JP_GDFS_ARGUMENT;
    memcpy(path,result,((size_t)n+1)*sizeof(wchar_t));return 0;
}
const wchar_t *jp_gdfs_host_path_get(void){return path;}
int jp_gdfs_host_allocate(size_t size,struct jp_gdfs_allocation *out)
{
    if(!size||!out||size>SIZE_MAX-131072)return JP_GDFS_ARGUMENT;
    SYSTEM_INFO info;GetSystemInfo(&info);size_t page=info.dwPageSize;
    size_t used=(size+31)&~(size_t)31,rounded=(used+page-1)&~(page-1),reserved=rounded+page*2;
    unsigned char *base=VirtualAlloc(NULL,reserved,MEM_RESERVE,PAGE_NOACCESS);
    if(!base)return JP_GDFS_MEMORY;
    if(!VirtualAlloc(base+page,rounded,MEM_COMMIT,PAGE_READWRITE)){
        VirtualFree(base,0,MEM_RELEASE);return JP_GDFS_MEMORY;
    }
    *out=(struct jp_gdfs_allocation){base,base+page+rounded-used,size,reserved};return 0;
}
int jp_gdfs_host_free(struct jp_gdfs_allocation *memory)
{
    if(!memory)return JP_GDFS_ARGUMENT;if(!memory->base)return 0;
    if(!VirtualFree(memory->base,0,MEM_RELEASE))return JP_GDFS_MEMORY;
    memset(memory,0,sizeof *memory);return 0;
}
