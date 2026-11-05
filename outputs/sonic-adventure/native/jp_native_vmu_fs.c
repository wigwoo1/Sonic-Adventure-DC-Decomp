/* Hardware boundary for the original SDK filesystem. This module deliberately
 * does not implement FAT, directories, file allocation or the backup codec. */
#include <sg_xpt.h>
#include <shinobi.h>
#include <vmsfs.h>
#include <string.h>
#include "jp_native_fibers.h"
#include "jp_native_vmu_store.h"

extern Uint32 buInitializeFlag;
extern void jp_vms_reset_api_state(void *work);
extern void jp_vms_exec_original(void);
extern Sint32 jp_vms_stat_original(Sint32 drive);
static Uint32 allowed_drives, recognized_drives;
static int serving;
static Uint32 attached(void) {
    struct jp_vmu_store_info info;
    if (jp_vmu_store_snapshot(&info)) return 0;
    recognized_drives &= info.attached_mask;
    return info.attached_mask & allowed_drives;
}
static int valid_drive(Sint32 drive) { return drive>=0 && drive<VMSFS_DRIVES; }

Sint32 buInit(Sint32 capacity,Sint32 drives,void *work,BU_INITCALLBACK callback) {
    struct jp_vmu_store_info info;
    if (buInitializeFlag) return BUD_ERR_BUSY;
    /* This owner implements an actual 128KB device, not imaginary capacities.
     * Work must satisfy the original BUM_WORK_SIZE(capacity,drive_count). */
    if (capacity!=BUD_CAPACITY_128KB || drives<=0 || (drives & ~255))
        return BUD_ERR_INVALID_PARAM;
    if (jp_vmu_store_snapshot(&info) || info.status) return BUD_ERR_NO_DISK;
    rfInitSystem(0);
    if (jp_rf_status()) return BUD_ERR_GENERIC;
    allowed_drives=(Uint32)drives; recognized_drives=0; serving=0;
    jp_vms_reset_api_state(work);
    Sint32 result=vmsfs_c_init(drives);
    if (result || jp_rf_status()) { rfExitSystem(); return BUD_ERR_GENERIC; }
    result=vmsfs_init(capacity,drives,work);
    if (result) { vmsfb_exit(); rfExitSystem(); return result; }
    buInitializeFlag=1;
    if (callback) callback();
    return BUD_ERR_OK;
}
void buDisableServer(void) {
    /* No Maple IRQ exists on the host. Service is already driven explicitly
     * on the game thread by buExecServer and busy buStat polling. */
}
void buExecServer(void) {
    if (!buInitializeFlag || serving || jp_rf_status()==JP_RF_THREAD) return;
    serving=1; jp_vms_exec_original(); serving=0;
}
Sint32 buStat(Sint32 drive) {
    if (!valid_drive(drive) || jp_rf_status()==JP_RF_THREAD) return BUD_STAT_BUSY;
    /* Original game save paths spin here while Maple IRQs deliver work. On
     * Windows perform one real server pass; never manufacture completion. */
    if (buInitializeFlag && !serving && vmsfs_c_getstate(drive)) buExecServer();
    return jp_vms_stat_original(drive);
}
Sint32 buExit(void) {
    if (!buInitializeFlag) return BUD_ERR_OK;
    if (serving || jp_rf_status()==JP_RF_THREAD) return BUD_ERR_BUSY;
    for (Sint32 drive=0;drive<VMSFS_DRIVES;drive++)
        if (vmsfs_c_getstate(drive)) return BUD_ERR_BUSY;
    /* Unlike the SH-4 path, a failed busy exit leaves service enabled. */
    buInitializeFlag=0; vmsfs_c_exit(); vmsfb_exit();
    Sint32 result=vmsfs_exit(); rfExitSystem();
    allowed_drives=recognized_drives=0;
    return result;
}
void vmsio_init(void) { recognized_drives=0; }
void vmsio_exit(void) { recognized_drives=0; }
Sint32 vmsio_isready(Sint32 drive) {
    return valid_drive(drive) && (attached() & (1UL<<drive)) ? 1 : 0;
}
Sint32 vmsio_ismount(Sint32 drive) {
    return vmsio_isready(drive) && (recognized_drives & (1UL<<drive)) ? 1 : 0;
}
Sint32 vmsio_get_media_info(Sint32 drive,struct vmsfs_media *media) {
    vmsfb_sleep();
    if (!media || !vmsio_isready(drive)) return BUD_ERR_NO_DISK;
    /* Fixed standard 128KB device profile. User blocks are assigned by the
     * original vmsphys caller from spare_block. Do not widen disk fields. */
    media->last_block=255; media->partition=0; media->system_block=255;
    media->fat_block=254; media->fat_size=1;
    media->dir_block=253; media->dir_size=13;
    media->spare_block=200; media->spare_size=41;
    media->icon=0; media->unk_28=0; media->last_byte=511;
    media->exec_block=0; media->exec_size=128;
    media->read_phases=media->write_phases=1;
    media->read_bytes=media->write_bytes=512;
    recognized_drives |= 1UL<<drive;
    return BUD_ERR_OK;
}
Sint32 vmsio_block_read(Sint32 drive,Sint32 block,void *buffer) {
    vmsfb_sleep();
    if (!vmsio_ismount(drive)) return BUD_ERR_NO_DISK;
    jp_vmu_store_clear_error();
    int result=jp_vmu_store_read((uint32_t)drive,(uint32_t)block,buffer);
    return result ? BUD_ERR_GENERIC : BUD_ERR_OK;
}
Sint32 vmsio_block_write(Sint32 drive,Sint32 block,const void *buffer) {
    vmsfb_sleep();
    if (!vmsio_ismount(drive)) return BUD_ERR_NO_DISK;
    jp_vmu_store_clear_error();
    int result=jp_vmu_store_write((uint32_t)drive,(uint32_t)block,buffer);
    return result ? BUD_ERR_WRITE_ERROR : BUD_ERR_OK;
}
Uint16 vmsfs_getword_little(void *buffer,Sint32 offset) {
    const Uint8 *p=(const Uint8 *)buffer+offset;
    return (Uint16)(p[0]|((Uint16)p[1]<<8));
}
Uint16 vmsfs_getword_big(void *buffer,Sint32 offset) {
    const Uint8 *p=(const Uint8 *)buffer+offset;
    return (Uint16)(((Uint16)p[0]<<8)|p[1]);
}
void vmsfs_setword_little(void *buffer,Sint32 offset,Uint16 value) {
    Uint8 *p=(Uint8 *)buffer+offset; p[0]=(Uint8)value; p[1]=(Uint8)(value>>8);
}
void vmsfs_setword_big(void *buffer,Sint32 offset,Uint16 value) {
    Uint8 *p=(Uint8 *)buffer+offset; p[0]=(Uint8)(value>>8); p[1]=(Uint8)value;
}
Sint32 vmsfs_memcmp(const void *a,const void *b) { return memcmp(a,b,512) ? -1 : 0; }
