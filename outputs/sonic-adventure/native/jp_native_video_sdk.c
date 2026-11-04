/* Real SDK global setters write the real typed native device record and the
 * authored register owner. This does not implement Kamui render/flip or IRQs.
 */
#include <kamui.h>
#include <string.h>
#include "jp_native_video_state.h"
volatile struct kmi_device_extension kmiDeviceExtension;
static int device_source(void *context,struct jp_video_snapshot *s)
{
    (void)context;
    s->logical_width=kmiDeviceExtension.display_width;s->logical_height=kmiDeviceExtension.display_height;
    s->clip_x=kmiDeviceExtension.fb_clip_x;s->clip_y=kmiDeviceExtension.fb_clip_y;s->presort=kmiDeviceExtension.presort;
    s->clear_argb=kmiDeviceExtension.background_base_color;
    s->background_isp=kmiDeviceExtension.background_isp;s->background_tsp=kmiDeviceExtension.background_tsp;
    s->background_texture=kmiDeviceExtension.background_texture;s->background_bytes=kmiDeviceExtension.background_size;
    for(unsigned i=0;i<62;i++)s->background_packet[i]=kmiDeviceExtension.background_packet[i];
    s->global_clip=kmiDeviceExtension.global_clip;s->global_clip_width=kmiDeviceExtension.global_clip_width;s->global_clip_height=kmiDeviceExtension.global_clip_height;
    if(!s->logical_width||!s->logical_height||s->logical_width>8192||s->logical_height>8192||s->presort>1)return 1;
    return 0;
}
__declspec(dllexport) int jp_video_sdk_initialize(unsigned width,unsigned height,uint32_t clear,int presort)
{
    int result=jp_video_initialize(width,height,clear,presort);if(result)return result;
    memset((void *)&kmiDeviceExtension,0,sizeof kmiDeviceExtension);
    kmiDeviceExtension.display_width=width;kmiDeviceExtension.display_height=height;kmiDeviceExtension.presort=presort;
    kmiDeviceExtension.background_base_color=clear;kmiDeviceExtension.fb_clip_x=(width-1)<<16;kmiDeviceExtension.fb_clip_y=(height-1)<<16;
    return jp_video_set_device_source(device_source,NULL);
}
KMINT32 kmiWriteRegister(KMDWORD address,KMDWORD value)
{return jp_video_write(address,value)==JP_VIDEO_OK;}
KMDWORD kmiReadRegister(KMDWORD address)
{uint32_t value=0;jp_video_read(address,&value);return value;}
