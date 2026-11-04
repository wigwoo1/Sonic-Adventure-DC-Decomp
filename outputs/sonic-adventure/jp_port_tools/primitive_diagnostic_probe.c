/* Extend the preserved diagnostic observer; never a production service. */
#define present_record jp_preserved_sprite_record
#include "sprite_diagnostic_probe.c"
#undef present_record
static unsigned char flat_copy[256*24];
void present_record(enum present_kind kind,const void *what,Int count,Int a,Float pri,Uint32 attr)
{
    if((kind==PRESENT_POLYGON || kind==PRESENT_TEXTURE || kind==PRESENT_TEXTURE_MEMLIST) && count>=0 && count<=256)
        memcpy(flat_copy,what,(size_t)count*(kind==PRESENT_POLYGON?sizeof(NJS_POLYGON_VTX):sizeof(NJS_TEXTURE_VTX)));
    jp_preserved_sprite_record(kind,what,count,a,pri,attr);
}
__declspec(dllexport) const void *jp_primitive_probe_saved_flat(void){return flat_copy;}
