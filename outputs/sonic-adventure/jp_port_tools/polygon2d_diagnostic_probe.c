/* Test-only replay observation and real wide-owner stand-ins. This object is
 * excluded from the production link audit; it cannot complete a game frame. */
#include "jp_ninja_polygon2d.h"
#include <string.h>
float port_wide_anchor;
int32_t port_wide_cover;
static uint32_t calls,meta[5];
static jp_point2col saved;
static jp_point2 points[256];
static jp_color2 colors[256],texs[256];
void present_record(enum jp_present_kind kind,const void *what,int32_t n,int32_t a,float pri,uint32_t attr)
{
    const jp_point2col *p=what;
    ++calls;meta[0]=(uint32_t)kind;meta[1]=(uint32_t)n;meta[2]=(uint32_t)a;memcpy(meta+3,&pri,4);meta[4]=attr;
    saved=*p;
    if(n>=0 && n<=256) {
        memcpy(points,p->p,(size_t)n*sizeof(*points));memcpy(colors,p->col,(size_t)n*sizeof(*colors));
        if(attr&0x80000000u)memcpy(texs,p->tex,(size_t)n*sizeof(*texs));
    }
}
__declspec(dllexport) uint32_t jp_polygon_probe_calls(void) { return calls; }
__declspec(dllexport) const uint32_t *jp_polygon_probe_meta(void) { return meta; }
__declspec(dllexport) const jp_point2col *jp_polygon_probe_saved(void) { return &saved; }
__declspec(dllexport) const jp_point2 *jp_polygon_probe_points(void) { return points; }
__declspec(dllexport) const jp_color2 *jp_polygon_probe_colors(void) { return colors; }
__declspec(dllexport) const jp_color2 *jp_polygon_probe_texs(void) { return texs; }
__declspec(dllexport) int jp_polygon_probe_draw(jp_point2col *p,int32_t n,uint32_t pri,uint32_t attr)
{
    float value;memcpy(&value,&pri,4);return jp_polygon2d_draw(p,n,value,attr);
}
