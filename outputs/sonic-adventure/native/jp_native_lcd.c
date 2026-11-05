/* Native LCD endpoints, backed by the actual virtual-card attachment owner.
 * Only the Kaede LCD request used here is supported; other commands fail. */
#include <sg_xpt.h>
#include <kaede.h>
#include <string.h>
#include "jp_native_lcd.h"
#include "jp_native_vmu_store.h"
struct endpoint { uint8_t pixels[JP_LCD_BYTES],queued[JP_LCD_BYTES];
    struct jp_lcd_info info; };
static struct endpoint endpoints[8];
static KDS_DEVICEINFO devices[24],absent;
static uint32_t enabled,attached;
static int initialized;
static int owner(struct jp_vmu_store_info *info) {
    if (!initialized) return JP_LCD_NOT_INITIALIZED;
    int result=jp_vmu_store_snapshot(info);
    return result==JP_VMU_THREAD ? JP_LCD_THREAD : result ? JP_LCD_NOT_INITIALIZED : 0;
}
static int drive_of(long port) {
    if (port<0 || port>=24 || (port%6!=1 && port%6!=2)) return -1;
    return (int)(port/6*2+port%6-1);
}
static void refresh(uint32_t mask) {
    mask &= enabled;
    for (uint32_t drive=0;drive<8;drive++) {
        uint32_t bit=1U<<drive;
        struct endpoint *e=endpoints+drive;
        if ((mask^attached)&bit) {
            memset(e->pixels,0,sizeof(e->pixels));
            memset(e->queued,0,sizeof(e->queued));
            e->info.connected=!!(mask&bit);e->info.pending=0;
        }
        KDS_DEVICEINFO *device=devices+(drive/2*6+drive%2+1);
        memset(device,0,sizeof(*device));
        if (mask&bit) {
            device->function=KDD_FUNC_STORAGE|KDD_FUNC_LCD|KDD_FUNC_TIMER;
            /* Ordered by descending function bit. The timer descriptor
             * advertises no optional timer operations; those are unsupported. */
            device->fd[1][1]=5;device->fd[1][2]=0x10;
            device->fd[2][1]=15;device->fd[2][2]=0x11;device->fd[2][3]=0x80;
            device->connector_dir[0]=(Uint8)e->info.device_direction;
        }
        KDS_DEVICEINFO *root=devices+(drive/2*6);
        root->function=KDD_FUNC_CONTROLLER;root->is_root=1;
        root->connector_dir[drive%2]=(Uint8)e->info.root_direction;
    }
    attached=mask;
}
int jp_lcd_initialize(uint32_t mask) {
    struct jp_vmu_store_info info;
    if (initialized) { int error=owner(&info);return error ? error : JP_LCD_STATE; }
    if (mask&~255U) return JP_LCD_ARGUMENT;
    int result=jp_vmu_store_snapshot(&info);
    if (result) return result==JP_VMU_THREAD ? JP_LCD_THREAD : JP_LCD_NOT_INITIALIZED;
    memset(endpoints,0,sizeof(endpoints));memset(devices,0,sizeof(devices));
    for (int i=0;i<8;i++) endpoints[i].info.device_direction=1;
    initialized=1;enabled=mask;attached=0;refresh(info.attached_mask);return 0;
}
int jp_lcd_connectors(uint32_t drive,uint32_t root,uint32_t device) {
    struct jp_vmu_store_info info;int error=owner(&info);if(error)return error;
    if (drive>=8 || root>=4 || device>=4) return JP_LCD_ARGUMENT;
    endpoints[drive].info.root_direction=root;
    endpoints[drive].info.device_direction=device;
    refresh(info.attached_mask);return 0;
}
int jp_lcd_service(void) {
    struct jp_vmu_store_info info;int error=owner(&info);if(error)return error;
    refresh(info.attached_mask);
    for (int i=0;i<8;i++) {
        struct endpoint *e=endpoints+i;
        if (e->info.connected && e->info.pending) {
            memcpy(e->pixels,e->queued,JP_LCD_BYTES);
            e->info.pending=0;e->info.presented++;
        }
    }
    return 0;
}
int jp_lcd_read(uint32_t drive,void *out,size_t size,struct jp_lcd_info *result) {
    struct jp_vmu_store_info info;int error=owner(&info);if(error)return error;
    if (drive>=8 || !out || size<JP_LCD_BYTES || !result) return JP_LCD_ARGUMENT;
    refresh(info.attached_mask);memcpy(out,endpoints[drive].pixels,JP_LCD_BYTES);
    *result=endpoints[drive].info;return 0;
}
int jp_lcd_shutdown(void) {
    struct jp_vmu_store_info info;int error=owner(&info);if(error)return error;
    refresh(info.attached_mask);
    for (int i=0;i<8;i++) if (endpoints[i].info.pending) return JP_LCD_STATE;
    memset(endpoints,0,sizeof(endpoints));memset(devices,0,sizeof(devices));
    initialized=0;attached=enabled=0;return 0;
}
const KDS_DEVICEINFO *kdGetDeviceInfo(Sint32 port) {
    struct jp_vmu_store_info info;
    if (port<0 || port>=24 || owner(&info)) return &absent;
    refresh(info.attached_mask);return devices+port;
}
Sint32 kdSetBlockWrite(Sint32 port,Sint32 function,Sint32 partition,
    Sint32 phase,Sint32 block,const void *data,Sint32 words) {
    struct jp_vmu_store_info info;int drive=drive_of(port);
    if (owner(&info) || drive<0 || function!=KDD_FUNC_LCD || partition || phase ||
        block || !data || words!=JP_LCD_BYTES/4) return -1;
    refresh(info.attached_mask);struct endpoint *e=endpoints+drive;
    if (!e->info.connected || e->info.pending) return -1;
    memcpy(e->queued,data,JP_LCD_BYTES);e->info.pending=1;e->info.accepted++;
    return (Sint32)(e->info.accepted&0x7fffffff);
}
/* C replacements for the four original lcdasm entry points. Their source
 * pixels use bit 3, unlike the game's vmu_pack, which tests any nonzero byte. */
static void pack(void *output,const void *input,int horizontal,int vertical) {
    uint8_t *out=output;const uint8_t *src=input;memset(out,0,JP_LCD_BYTES);
    for (int y=0;y<32;y++) for (int x=0;x<48;x++) if (src[y*48+x]&8) {
        int dx=horizontal?47-x:x,dy=vertical?31-y:y;
        out[dy*6+dx/8] |= (uint8_t)(0x80U>>(dx%8));
    }
}
void pdVmsLcdBltAsm(void *out,void *in) { pack(out,in,0,0); }
void pdVmsLcdBltAsmFlipH(void *out,void *in) { pack(out,in,1,0); }
void pdVmsLcdBltAsmFlipV(void *out,void *in) { pack(out,in,0,1); }
void pdVmsLcdBltAsmFlipHV(void *out,void *in) { pack(out,in,1,1); }
