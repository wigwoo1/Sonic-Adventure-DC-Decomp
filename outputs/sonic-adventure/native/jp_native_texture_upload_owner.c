/* Replace textures_retained.obj with this object, never link both. */
#include "jp_native_textures.h"
#include "jp_native_texture_upload.h"
#include "jp_native_textures_retained.c"
int jp_memory_owner_can_bind(Uint32 *field)
{
    if(!ready() || !field)return 0;
    struct binding *b=binding_for(field,1);
    return b && !b->field;
}
NJS_TEXMEMLIST *jp_memory_owner_bound(Uint32 *field)
{
    if(!initialized || !field)return NULL;
    struct binding *b=binding_for(field,0);
    if(!b || b->handle!=*field)return NULL;
    for(uint32_t i=0;i<_nj_texmemsize;i++)
        if(tokens[i]==b->handle && _nj_texmemlist[i].globalIndex!=UINT32_MAX)
            return &_nj_texmemlist[i];
    return NULL;
}
