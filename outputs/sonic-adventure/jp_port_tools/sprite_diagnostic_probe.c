/* Diagnostic replay/frame observation only. Never link this into production. */
#include "jp_native_textures.h"
#include "gcc/src/system/port.h"
#include <string.h>
extern Int (*port_sprite_record)(enum present_kind,const void *,Int,Int,Float,Uint32);
extern int jp_polygon2d_draw(NJS_POINT2COL *,Int,Float,Uint32);
int frame_step,frame_later_draws;
static Uint32 calls,meta[5];
static int untimed,unbalanced,allowed=TRUE;
static unsigned char sprite_copy[sizeof(NJS_SPRITE)],frame_copy[sizeof(NJS_TEXANIM)];
static NJS_POINT2COL polygon;
static NJS_POINT2 points[256];static NJS_COLOR colors[256],texs[256];
void present_record(enum present_kind kind,const void *what,Int count,Int a,Float pri,Uint32 attr)
{
    ++calls;meta[0]=(Uint32)kind;meta[1]=(Uint32)count;meta[2]=(Uint32)a;memcpy(meta+3,&pri,4);meta[4]=attr;
    if(kind==PRESENT_POLYGON_2D){
        polygon=*(const NJS_POINT2COL *)what;
        if(count>=0 && count<=256){memcpy(points,polygon.p,(size_t)count*8);memcpy(colors,polygon.col,(size_t)count*4);if(attr&NJD_USE_TEXTURE)memcpy(texs,polygon.tex,(size_t)count*4);}
    }else if(kind==PRESENT_SPRITE || kind==PRESENT_NJ_SPRITE || kind==PRESENT_SPRITE_2D){
        const NJS_SPRITE *s=what;memcpy(sprite_copy,s,sizeof(*s));memcpy(frame_copy,&s->tanim[a],sizeof(NJS_TEXANIM));
    }
}
void present_untimed(int begin){untimed+=begin?1:-1;if(untimed<0)unbalanced=1;}
static Int sprite_observe(enum present_kind kind,const void *what,Int count,Int a,Float pri,Uint32 attr)
{present_record(kind,what,count,a,pri,attr);return allowed;}
__declspec(dllexport) void jp_sprite_probe_callback(Int enable,Int allow){allowed=allow;port_sprite_record=enable?sprite_observe:NULL;}
__declspec(dllexport) void jp_sprite_probe_frame(Int step,Int later){frame_step=step;frame_later_draws=later;}
__declspec(dllexport) int jp_sprite_probe_untimed(void){return unbalanced?-999:untimed;}
__declspec(dllexport) const void *jp_sprite_probe_saved_sprite(void){return sprite_copy;}
__declspec(dllexport) const void *jp_sprite_probe_saved_frame(void){return frame_copy;}
__declspec(dllexport) Uint32 jp_polygon_probe_calls(void){return calls;}
__declspec(dllexport) const Uint32 *jp_polygon_probe_meta(void){return meta;}
__declspec(dllexport) const NJS_POINT2COL *jp_polygon_probe_saved(void){return &polygon;}
__declspec(dllexport) const NJS_POINT2 *jp_polygon_probe_points(void){return points;}
__declspec(dllexport) const NJS_COLOR *jp_polygon_probe_colors(void){return colors;}
__declspec(dllexport) const NJS_COLOR *jp_polygon_probe_texs(void){return texs;}
__declspec(dllexport) int jp_polygon_probe_draw(NJS_POINT2COL *p,Int count,Uint32 pri,Uint32 attr)
{Float f;memcpy(&f,&pri,4);return jp_polygon2d_draw(p,count,f,attr);}
static NJS_TEXNAME texture_name;static NJS_TEXLIST texture_list;
__declspec(dllexport) NJS_TEXLIST *jp_sprite_probe_prepare_texture(void)
{
    unsigned char pixels[128]={0};NJS_TEXINFO info;memset(&info,0,sizeof(info));
    if(jp_textures_initialize(4,8,1<<20))return NULL;
    info.texsurface.Type=0x10000;info.texsurface.BitDepth=1;info.texsurface.PixelFormat=0x08000000;
    info.texsurface.nWidth=info.texsurface.nHeight=8;info.texsurface.TextureSize=128;info.texsurface.fSurfaceFlags=2;
    if(jp_texture_copy_surface(&info,pixels,sizeof(pixels)))return NULL;
    njSetTextureName(&texture_name,NULL,123,0);njEntryTexMemList(&texture_name.texaddr,&info,123,2,0xffffffffu);
    if(jp_textures_status())return NULL;
    texture_list.textures=&texture_name;texture_list.nbTexture=1;return &texture_list;
}
