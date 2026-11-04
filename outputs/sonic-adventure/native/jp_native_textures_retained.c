/* Compile IN PLACE OF jp_native_textures.c. Keep its owner and all SDK
 * contracts; defer only payload frees while real queued/submitted frames
 * retain them. Record release and accounting remain the original owner's.
 */
#include <malloc.h>
#include "jp_native_frames_internal.h"
static void jp_texture_retained_free(void *pixels)
{if(!jp_frames_defer_surface_free(pixels))_aligned_free(pixels);}
#define _aligned_free jp_texture_retained_free
#include "jp_native_textures.c"
#undef _aligned_free
int jp_texture_live_surface_view(uint32_t word,const void **pixels,size_t *bytes)
{
    if(!initialized || !word || !pixels || !bytes)return 0;
    for(uint32_t i=0;i<_nj_texmemsize;i++)if(surfaces[i].pixels && surfaces[i].word==word){
        *pixels=surfaces[i].pixels;*bytes=surfaces[i].bytes;return 1;
    }
    return 0;
}
