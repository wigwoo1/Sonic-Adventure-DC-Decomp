#include <stdint.h>
#ifdef _WIN32
#define SA_EXPORT __declspec(dllexport)
#else
#define SA_EXPORT
#endif
typedef int (*sa_disc_read_at)(void *,uint64_t,uint8_t *,uint32_t);
/* Native platform support, not recovered Sega code. MODE1 payloads only.
 * The read-at service must fill all requested bytes or return a nonzero error.
 * Complete input spans are checked before I/O. On I/O failure earlier sectors
 * remain delivered; completed counts those sectors, excluding a partial sector.
 * No global state, filesystem parser, BIOS timing or command queue is implied. */
SA_EXPORT int sa_disc_read_sectors(sa_disc_read_at read,void *user,
    uint64_t file_offset,uint64_t track_sectors,uint32_t track_lba,uint32_t stride,
    uint32_t first,uint32_t count,uint8_t *output,uint64_t capacity,uint32_t *completed)
{
    if(!completed)return -2;
    *completed=0;
    if(!read || (!output && count) || (stride!=2048 && stride!=2352))return -2;
    if(first<track_lba)return -3;
    uint64_t relative=(uint64_t)first-track_lba;
    if(relative>track_sectors || (uint64_t)count>track_sectors-relative || (uint64_t)count*2048>capacity)return -3;
    uint64_t payload=stride==2352?16u:0u;
    if(relative>UINT64_MAX/stride)return -3;
    uint64_t start=relative*stride;
    if(file_offset>UINT64_MAX-start || file_offset+start>UINT64_MAX-payload)return -3;
    start+=file_offset+payload;
    if(count && ((uint64_t)(count-1)>(UINT64_MAX-start)/stride ||
        start+(uint64_t)(count-1)*stride>UINT64_MAX-2048))return -3;
    for(uint32_t sector=0;sector<count;sector++) {
        int result=read(user,start+(uint64_t)sector*stride,output+(uint64_t)sector*2048,2048);
        if(result)return result;
        *completed=sector+1;
    }
    return 0;
}
