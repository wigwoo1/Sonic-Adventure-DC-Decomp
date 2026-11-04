/* Actual SDK selection contract over the existing native texture-handle owner. */
#include "jp_native_textures.h"
#include <kamui.h>
#include "sdk/ninja/njkamui.h"
Sint32 njSetkmTextureNum(Uint32 index)
{
    const NJS_TEXMEMLIST *t=jp_renderer_texture_lookup(_nj_current_texlist,index);
    if(!t)return -1;
    if(t->globalIndex!=_nj_current_texture || t->bank!=_nj_current_texture_bank) {
        _nj_current_texture=t->globalIndex;_nj_current_texture_bank=t->bank;
        _nj_parameterkm_.TSPPARAMBUFFER=(_nj_parameterkm_.TSPPARAMBUFFER&~0x3fu)|(t->tspparambuffer&0x3fu);
        if((t->texparambuffer&0x001fffffu)>0x00100000u)return -1;
        _nj_parameterkm_.TexturePARAMBUFFER=t->texparambuffer;
    }
    return 1;
}
