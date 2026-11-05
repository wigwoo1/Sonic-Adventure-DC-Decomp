/* Console video setup is represented by real native owners and the original
 * typed SDK frame calculations. Hardware MMIO/interrupt setup is not executed. */
#include <kamui.h>
#include <ninja.h>
#include "jp_native_device_internal.h"
extern int jp_video_sdk_initialize(unsigned,unsigned,uint32_t,int);
extern int jp_video_write(uint32_t,uint32_t);
extern int jp_present_status(void);
extern int jp_ninja_global_status(void);

int jp_device_sdk_validate(int32_t mode,int32_t frame,int32_t count)
{
    if(mode<0 || (mode&~0x3f) || (mode&3)>2 || count<1)return 1;
    return frame!=0 && frame!=1 && frame!=3 && frame!=4 && frame!=5;
}
KMSTATUS kmInitDevice(KMVIDEOMODE video)
{
    if(video<KM_NTSC || video>KM_PAL)return KMSTATUS_INVALID_VIDEO_MODE;
    if(!jp_device_phase_is(JP_DEVICE_PREPARED))return KMSTATUS_INVALID_PARAMETER;
    if(jp_device_create_gpu())return KMSTATUS_HARDWARE_NOT_PRESENTED;
    if(jp_video_sdk_initialize(640,480,0xff000000,TRUE)){
        jp_device_fail(JP_DEVICE_VIDEO);return KMSTATUS_INVALID_PARAMETER;
    }
    if(jp_device_sdk_video((unsigned)video))return KMSTATUS_INVALID_PARAMETER;
    kmiDeviceExtension.video_mode=video;return KMSTATUS_SUCCESS;
}
KMSTATUS kmSetDisplayMode(KMDISPLAYMODE display,KMBPPMODE bpp,KMBOOLEAN dither,KMBOOLEAN anti_alias)
{
    if(!jp_device_phase_is(JP_DEVICE_GPU_READY))return KMSTATUS_INVALID_PARAMETER;
    if(((unsigned)display&~0x3fu) || ((unsigned)display&3u)>2 || anti_alias ||
       (bpp!=KM_DSPBPP_RGB565 && bpp!=KM_DSPBPP_RGB555 && bpp!=KM_DSPBPP_ARGB1555 && bpp!=KM_DSPBPP_RGB888 && bpp!=KM_DSPBPP_ARGB8888))
        return KMSTATUS_INVALID_DISPLAY_MODE;
    unsigned width=display==KM_DSPMODE_VGA || (display&KM_DSPMODE_WIDTH_640)?640:320;
    unsigned height=display==KM_DSPMODE_VGA || (display&KM_DSPMODE_HEIGHT_480)?480:240;
    kmiDeviceExtension.display_mode=display;kmiDeviceExtension.video_mode=display&3;
    kmiDeviceExtension.bpp=bpp;kmiDeviceExtension.dither=dither!=0;kmiDeviceExtension.anti_alias=FALSE;
    kmiDeviceExtension.picture_lines=display==KM_DSPMODE_VGA?480:240;
    int interlace=(display&KM_VGA)!=KM_VGA && (display&(KM_DSPMODE_INTERLACE|KM_DSPMODE_PSEUDONONINTERLACE|KM_DSPMODE_HEIGHT_480));
    if(kmiFBControl((KMINT32)width,(KMINT32)height,bpp,interlace!=0,dither!=0,FALSE) ||
       jp_video_write(0x48,(uint32_t)kmiDeviceExtension.fb_write_control)){
        jp_device_fail(JP_DEVICE_VIDEO);return KMSTATUS_INVALID_PARAMETER;
    }
    if(jp_device_create_display(width,height,(void *)&kmiDeviceExtension))return KMSTATUS_INVALID_PARAMETER;
    return KMSTATUS_SUCCESS;
}
void waitVsync(void){(void)jp_device_wait_tick();}
void njSetVideoMode(Uint32 cable){(void)jp_device_set_cable(cable);}
void njInitSystem(Int mode,Int frame,Int count){(void)jp_device_initialize(mode,frame,count);}
void jp_device_sdk_snapshot(struct jp_device_info *out)
{
    out->video=(uint32_t)kmiDeviceExtension.video_mode;out->display=(uint32_t)kmiDeviceExtension.display_mode;
    out->bpp=(uint32_t)kmiDeviceExtension.bpp;
    out->logical_width=(uint32_t)kmiDeviceExtension.display_width;out->logical_height=(uint32_t)kmiDeviceExtension.display_height;
    out->wait_count=kmiDeviceExtension.vsync_wait+1;
}
