/* Authored SDK header-bit service bound to existing queue/render-state owners. */
#include <kamui.h>
#include <ninja.h>
#include "sdk/ninja/njkamui.h"
void njSetkmColorMode(Uint32 mode)
{
    Uint32 global=(_nj_parameterkm_.GLOBALPARAMBUFFER & ~0x020000ffu) | (mode & 0x020000ffu);
    Uint32 tsp=(_nj_parameterkm_.TSPPARAMBUFFER & ~0x00180000u) | (mode & 0x00180000u);
    Uint32 flags=_nj_control_3d_flag_;
    if((flags & 0x400u) || ((flags & 0x1000u) && !(mode & 0x02100000u)))global|=0x80u;
    else global&=~0x80u;
    _nj_parameterkm_.GLOBALPARAMBUFFER=global;
    _nj_parameterkm_.TSPPARAMBUFFER=tsp;
    *_nj_color_mode_=mode;
}
