/* Original bitmap queue is separately compiled from pinned game source. */
#include "jp_native_texture_upload.h"
#include "system/bitmap.h"
int jp_bitmap_source_begin(struct bitmap *bitmap)
{
    if(!bitmap || !bitmap->pixels)return jp_memory_bind_source(NULL,0);
    size_t bytes;int rc=jp_memory_raw_size(0x900,bitmap->texture_width,bitmap->texture_height,&bytes);
    if(rc)return rc;
    return jp_memory_bind_source(bitmap->pixels,bytes);
}
