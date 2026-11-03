#include "jp_native_render_state.h"
#include <malloc.h>
#include <string.h>
_Static_assert(sizeof(Float)==4 && sizeof(void *)==8,"native renderer ABI");
NJS_SCREEN _nj_screen_;
NJS_POINT2 _nj_2d_clip_[2];
Float _nj_near_clip_,_nj_far_clip_,_nj_scr_aspect_x_,_nj_scr_aspect_y_;
Uint32 _nj_control_3d_flag_,_nj_constant_attr_and_,_nj_constant_attr_or_;
NJS_ARGB _nj_constant_material_;
NJS_VERTEX_BUF *_nj_vertex_buf_;
Int _nj_vertex_num_,_nj_calc_vertex_,_nj_calc_polygon_,_nj_draw_polygon_;
Float _nj_depth_near_,_nj_depth_far_,_nj_depth_range_;
static unsigned char *allocation,*scratch;
static uint32_t capacity;
static size_t allocated;
static int status=JP_RENDER_NOT_INITIALIZED;
static int fail(int value) {status=value;return value;}
static int ready(void)
{
    if(!allocation) {fail(JP_RENDER_NOT_INITIALIZED);return 0;}
    return status==JP_RENDER_OK;
}
int jp_render_state_status(void) {return status;}
void jp_render_state_clear_error(void) {status=allocation?JP_RENDER_OK:JP_RENDER_NOT_INITIALIZED;}
void *jp_render_state_scratch(void) {return scratch;}
size_t jp_render_state_allocated_bytes(void) {return allocated;}
uint32_t jp_render_state_capacity(void) {return capacity;}
void jp_render_state_shutdown(void)
{
    _aligned_free(allocation);allocation=scratch=NULL;capacity=0;allocated=0;
    _nj_vertex_buf_=NULL;_nj_vertex_num_=0;status=JP_RENDER_NOT_INITIALIZED;
}
int jp_render_state_initialize(uint32_t count,size_t budget)
{
    if(!count || count>2048)return fail(JP_RENDER_ARGUMENT);
    size_t cost=(size_t)count*16+64;
    if(allocated>budget || cost>budget-allocated)return fail(JP_RENDER_BUDGET);
    unsigned char *next=_aligned_malloc(cost,32);
    if(!next)return fail(JP_RENDER_ALLOCATION);
    memset(next,0xa5,32);memset(next+32,0,(size_t)count*16);
    memset(next+32+(size_t)count*16,0x5a,32);
    jp_render_state_shutdown();allocation=next;scratch=next+32;
    capacity=count;allocated=cost;status=JP_RENDER_OK;
    njInit3D((NJS_VERTEX_BUF *)scratch,(Int)count);
    _nj_calc_vertex_=_nj_calc_polygon_=_nj_draw_polygon_=0;
    _nj_depth_near_=_nj_depth_far_=_nj_depth_range_=0.0F;
    return jp_render_state_configure(640,480,500.0F,1.0F,60000.0F,1.0F,1.0F);
}
int jp_render_state_accept_buffer(NJS_VERTEX_BUF *buffer,Int count)
{
    if(!ready())return 0;
    if(buffer!=(NJS_VERTEX_BUF *)scratch || count!=(Int)capacity) {fail(JP_RENDER_STATE);return 0;}
    return 1;
}
/* Native expression of the original FCMP/GT/FNEG clamp contract, including
 * comparisons that leave a NaN unchanged before its sign is negated. */
void njClipZ(Float near_value,Float far_value)
{
    if(near_value>-1.0F)near_value=-1.0F;
    else if(near_value<-65535.0F)near_value=-65535.0F;
    if(far_value>-1.0F)far_value=-1.0F;
    else if(far_value<-65535.0F)far_value=-65535.0F;
    _nj_near_clip_=-near_value;_nj_far_clip_=-far_value;
}
int jp_render_state_configure(uint32_t width,uint32_t height,float distance,
    float near_distance,float far_distance,float aspect_x,float aspect_y)
{
    if(!ready())return status;
    if(!width || !height || width>8192 || height>8192 ||
       !__builtin_isfinite(distance) || distance<=0 || !__builtin_isfinite(near_distance) ||
       !__builtin_isfinite(far_distance) || near_distance<1 || far_distance>65535 || far_distance<=near_distance ||
       !__builtin_isfinite(aspect_x) || !__builtin_isfinite(aspect_y) || aspect_x<=0 || aspect_y<=0)return fail(JP_RENDER_ARGUMENT);
    _nj_screen_.w=(Float)width;_nj_screen_.h=(Float)height;
    NJS_SCREEN screen={distance,(Float)width,(Float)height,(Float)width*0.5F,(Float)height*0.5F};
    njSetScreen(&screen);njClipZ(-near_distance,-far_distance);
    _nj_scr_aspect_x_=aspect_x;_nj_scr_aspect_y_=aspect_y;
    _nj_2d_clip_[0].x=_nj_2d_clip_[0].y=0.0F;
    _nj_2d_clip_[1].x=(Float)(width-1);_nj_2d_clip_[1].y=(Float)(height-1);
    return status;
}
int jp_render_state_check_guards(void)
{
    if(!ready())return status;
    for(size_t i=0;i<32;i++)if(allocation[i]!=0xa5 || scratch[(size_t)capacity*16+i]!=0x5a)return fail(JP_RENDER_GUARD);
    return status;
}
int jp_render_state_reset_scratch(void)
{
    if(jp_render_state_check_guards()!=JP_RENDER_OK)return status;
    memset(scratch,0,(size_t)capacity*16);return status;
}
