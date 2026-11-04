#include "jp_native_video_state.h"
#include <string.h>
static struct jp_video_snapshot current;
static uint64_t next_generation=1;
static int initialized,status=JP_VIDEO_NOT_INITIALIZED;
size_t jp_video_snapshot_size(void){return sizeof(struct jp_video_snapshot);}
static jp_video_device_source device_source;
static void *device_context;
static int fail(int code){status=code;return code;}
static int ready(void){return initialized && status==JP_VIDEO_OK;}
static int changed(void)
{
    if(!next_generation)return fail(JP_VIDEO_OVERFLOW);
    current.generation=next_generation++;return JP_VIDEO_OK;
}
static uint32_t *field(uint32_t address)
{
    if(address>=0x1000 && address<0x2000 && !(address&3))return &current.palette[(address-0x1000)/4];
    if(address>=0x200 && address<0x400 && !(address&3))return &current.fog_table[(address-0x200)/4];
    switch(address){
    case 0x40:return &current.border_color;
    case 0x48:return &current.fb_write_control;
    case 0x78:return &current.cull_bits;
    case 0xb0:return &current.fog_color;
    case 0xb4:return &current.vertex_fog_color;
    case 0xb8:return &current.fog_density;
    case 0xbc:return &current.clamp_max;
    case 0xc0:return &current.clamp_min;
    case 0xe4:return &current.texture_control;
    case 0x108:return &current.palette_mode;
    default:return NULL;
    }
}
int jp_video_initialize(unsigned w,unsigned h,uint32_t clear,int presort)
{
    if(!w||!h||w>8192||h>8192||(presort!=0 && presort!=1))return fail(JP_VIDEO_ARGUMENT);
    if(!next_generation)return fail(JP_VIDEO_OVERFLOW);
    memset(&current,0,sizeof current);current.logical_width=w;current.logical_height=h;current.clear_argb=clear;current.presort=(uint32_t)presort;
    current.clamp_max=UINT32_MAX;current.clip_x=(w-1)<<16;current.clip_y=(h-1)<<16;
    device_source=NULL;device_context=NULL;initialized=1;status=JP_VIDEO_OK;return changed();
}
void jp_video_shutdown(void){memset(&current,0,sizeof current);device_source=NULL;device_context=NULL;initialized=0;status=JP_VIDEO_NOT_INITIALIZED;}
int jp_video_set_device_source(jp_video_device_source source,void *context)
{if(!ready())return status;if(!source && context)return fail(JP_VIDEO_ARGUMENT);device_source=source;device_context=context;return JP_VIDEO_OK;}
int jp_video_status(void){return status;}
void jp_video_clear_error(void){status=initialized?JP_VIDEO_OK:JP_VIDEO_NOT_INITIALIZED;}
int jp_video_write(uint32_t address,uint32_t value)
{
    if(!ready())return status;
    uint32_t *p=field(address);if(!p)return fail(JP_VIDEO_UNSUPPORTED);
    if(address==0x108 && value>3)return fail(JP_VIDEO_ARGUMENT);
    if(!next_generation)return fail(JP_VIDEO_OVERFLOW);
    if(*p==value)return JP_VIDEO_OK;
    *p=value;return changed();
}
int jp_video_read(uint32_t address,uint32_t *out)
{
    if(!ready())return status;if(!out)return fail(JP_VIDEO_ARGUMENT);
    uint32_t *p=field(address);if(!p)return fail(JP_VIDEO_UNSUPPORTED);
    *out=*p;return JP_VIDEO_OK;
}
int jp_video_set_canvas(unsigned w,unsigned h,uint32_t clear,int presort)
{
    if(!ready())return status;
    if(!w||!h||w>8192||h>8192||(presort!=0 && presort!=1))return fail(JP_VIDEO_ARGUMENT);
    if(!next_generation)return fail(JP_VIDEO_OVERFLOW);
    current.logical_width=w;current.logical_height=h;current.clear_argb=clear;current.presort=(uint32_t)presort;return changed();
}
int jp_video_set_clip(unsigned l,unsigned t,unsigned r,unsigned b)
{
    if(!ready())return status;
    if(l>r||t>b||r>=current.logical_width||b>=current.logical_height)return fail(JP_VIDEO_ARGUMENT);
    if(!next_generation)return fail(JP_VIDEO_OVERFLOW);
    current.clip_x=(r<<16)|l;current.clip_y=(b<<16)|t;return changed();
}
int jp_video_capture(struct jp_video_snapshot *out)
{
    if(!ready())return status;if(!out)return fail(JP_VIDEO_ARGUMENT);
    struct jp_video_snapshot next=current;
    if(device_source && device_source(device_context,&next))return fail(JP_VIDEO_UNSUPPORTED);
    next.generation=current.generation;
    if(memcmp(&next,&current,sizeof next)){
        if(!next_generation)return fail(JP_VIDEO_OVERFLOW);
        current=next;changed();
    }
    *out=current;return JP_VIDEO_OK;
}
static unsigned five(unsigned x){return (x<<3)|(x>>2);}
static unsigned six(unsigned x){return (x<<2)|(x>>4);}
int jp_video_palette_rgba(const struct jp_video_snapshot *s,uint32_t *out)
{
    if(!s||!out||s->palette_mode>3)return JP_VIDEO_ARGUMENT;
    for(unsigned i=0;i<1024;i++){
        uint32_t c=s->palette[i],r,g,b,a;
        if(s->palette_mode==0){r=five((c>>10)&31);g=five((c>>5)&31);b=five(c&31);a=c&0x8000?255:0;}
        else if(s->palette_mode==1){r=five((c>>11)&31);g=six((c>>5)&63);b=five(c&31);a=255;}
        else if(s->palette_mode==2){r=((c>>8)&15)*17;g=((c>>4)&15)*17;b=(c&15)*17;a=((c>>12)&15)*17;}
        else{out[i]=c;continue;}
        out[i]=(a<<24)|(r<<16)|(g<<8)|b;
    }
    return JP_VIDEO_OK;
}
