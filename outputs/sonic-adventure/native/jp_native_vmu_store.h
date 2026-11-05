#ifndef JP_NATIVE_VMU_STORE_H
#define JP_NATIVE_VMU_STORE_H
#include <stdint.h>
#include <stddef.h>
#define JP_VMU_API __declspec(dllexport)
enum jp_vmu_error {JP_VMU_OK,JP_VMU_ARGUMENT,JP_VMU_NOT_INITIALIZED,
    JP_VMU_THREAD,JP_VMU_STATE,JP_VMU_IO,JP_VMU_FORMAT,JP_VMU_BUSY,
    JP_VMU_ALLOCATION,JP_VMU_EXISTS};
enum {JP_VMU_DRIVES=8,JP_VMU_BLOCK_BYTES=512,JP_VMU_BLOCKS=256};
struct jp_vmu_store_info {uint64_t block_reads,block_writes,created;
    uint32_t attached_mask,status,os_error,reserved;};
/* Existing directory; one game/emulator thread. Files VMU_A1.VMU through
 * VMU_D2.VMU are exact 128KB card images, not flat SONICADV_JP.SAV records.
 * Create explicitly makes an unformatted zero-filled image and never replaces
 * an existing file. Open validates size and takes an exclusive .lock sidecar
 * understood by the future companion/widget. Close releases that ownership.
 */
JP_VMU_API int jp_vmu_store_initialize(const wchar_t *existing_directory);
JP_VMU_API int jp_vmu_store_shutdown(void);
JP_VMU_API int jp_vmu_store_status(void);
JP_VMU_API void jp_vmu_store_clear_error(void);
JP_VMU_API int jp_vmu_store_snapshot(struct jp_vmu_store_info *);
JP_VMU_API int jp_vmu_store_create(uint32_t drive);
JP_VMU_API int jp_vmu_store_open(uint32_t drive);
JP_VMU_API int jp_vmu_store_close(uint32_t drive);
JP_VMU_API int jp_vmu_store_read(uint32_t drive,uint32_t block,void *out512);
/* Stages the full image, flushes and atomically replaces it. Failed writes
 * preserve memory and the previous image. Detects a pre-existing external edit
 * before replacing it; uncooperative third-party rename races are not guarded.
 * This is block persistence, not an atomic multi-block filesystem transaction.
 */
JP_VMU_API int jp_vmu_store_write(uint32_t drive,uint32_t block,const void *in512);
#endif
