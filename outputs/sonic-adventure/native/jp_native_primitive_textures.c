/* Complete global-ID and record-index SDK selectors on the real native owner. */
#include "jp_native_textures.h"
#include <kamui.h>
#include "sdk/ninja/njkamui.h"
static const NJS_TEXMEMLIST *loaded_record(Uint32 index)
{
    const NJS_TEXMEMLIST *r=jp_textures_record(index);
    if(!r || r->globalIndex==0xffffffffu || !(r->texaddr&TEXMEM_LOADED) ||
       !jp_texture_surface_owned(&r->texinfo.texsurface))return NULL;
    return r;
}
static Sint32 select_record(const NJS_TEXMEMLIST *r)
{
    _nj_current_texture=r->globalIndex;_nj_current_texture_bank=r->bank;
    _nj_parameterkm_.TSPPARAMBUFFER=(_nj_parameterkm_.TSPPARAMBUFFER&~0x3fu)|(r->tspparambuffer&0x3fu);
    if((r->texparambuffer&0x001fffffu)>0x00100000u)return -1;
    _nj_parameterkm_.TexturePARAMBUFFER=r->texparambuffer;return 1;
}
Sint32 njSetkmTextureNumG(Uint32 global_id)
{
    if(jp_textures_status()!=JP_TEXTURE_OK)return -1;
    Sint32 index=njSearchTexList(_nj_texmemlist,_nj_texmemsize,global_id);
    if(index<0)return -1;
    const NJS_TEXMEMLIST *r=loaded_record((Uint32)index);if(!r)return -1;
    if(global_id!=_nj_current_texture || r->bank!=_nj_current_texture_bank)return select_record(r);
    return 1;
}
Sint32 njSetkmTextureMemList(Uint32 index)
{
    const NJS_TEXMEMLIST *r=loaded_record(index);return r?select_record(r):-1;
}
