/* REPLACE textures_upload.obj. The existing owner stays byte-for-byte included;
 * its actual binding entries are moved, with no duplicate texture registry. */
#include "jp_native_texture_upload_owner.c"
#include "jp_native_assets.h"
static int move_check(NJS_TEXNAME *from,NJS_TEXNAME *to,uint32_t count)
{
    if(jp_memory_status()==JP_MEMORY_THREAD)return JP_TEXTURE_STATE;
    if(!ready())return status;
    if(!count)return JP_TEXTURE_OK;
    if(!from||!to||count>binding_limit)return fail(JP_TEXTURE_ARGUMENT);
    size_t bytes=(size_t)count*sizeof *from;uintptr_t a=(uintptr_t)from,b=(uintptr_t)to;
    if(bytes>UINTPTR_MAX-a || bytes>UINTPTR_MAX-b || (from!=to && a<b+bytes && b<a+bytes))return fail(JP_TEXTURE_ARGUMENT);
    for(uint32_t i=0;i<count;i++){
        struct binding *bound=binding_for(&from[i].texaddr,0);
        NJS_TEXMEMLIST *record=jp_memory_owner_bound(&from[i].texaddr);
        if(!bound||!record)return fail(JP_TEXTURE_STALE);
        if(!(record->texaddr&TEXMEM_LOADED)||!record->count||!surface_at(record->texinfo.texsurface.pSurface))return fail(JP_TEXTURE_STATE);
        if(from!=to && binding_for(&to[i].texaddr,0))return fail(JP_TEXTURE_STATE);
    }
    return JP_TEXTURE_OK;
}
int jp_texture_validate_move(NJS_TEXNAME *from,NJS_TEXNAME *to,uint32_t count)
{return move_check(from,to,count);}
int jp_texture_move_names(NJS_TEXNAME *from,NJS_TEXNAME *to,uint32_t count)
{
    int result=move_check(from,to,count);if(result || from==to)return result;
    for(uint32_t i=0;i<count;i++){
        struct binding *bound=binding_for(&from[i].texaddr,0);
        NJS_TEXMEMLIST *record=jp_memory_owner_bound(&from[i].texaddr);
        Uint32 index=record->globalIndex;
        if(from[i].attr&NJD_TEXATTR_PALGLOBALINDEX)index|=(record->bank&63)<<NJD_TEXBANK_SHIFT;
        to[i].attr=from[i].attr;to[i].texaddr=bound->handle;
        bound->field=&to[i].texaddr;from[i].texaddr=index;
    }
    return JP_TEXTURE_OK;
}
