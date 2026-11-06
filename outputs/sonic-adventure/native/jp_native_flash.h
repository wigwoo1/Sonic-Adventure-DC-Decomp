#ifndef JP_NATIVE_FLASH_H
#define JP_NATIVE_FLASH_H
#include <stdint.h>
#include <stddef.h>
#ifndef JP_FLASH_API
#define JP_FLASH_API __declspec(dllexport)
#endif
enum jp_flash_error {JP_FLASH_OK,JP_FLASH_ARGUMENT,JP_FLASH_NOT_OPEN,
    JP_FLASH_THREAD,JP_FLASH_STATE,JP_FLASH_IO,JP_FLASH_FORMAT,JP_FLASH_BUSY,
    JP_FLASH_ALLOCATION,JP_FLASH_CHANGED,JP_FLASH_PROTECTED};
struct jp_flash_info {
    uint64_t reads,programs,erases,commits;
    uint32_t status,os_error,creator_thread,opened;
};
struct jp_flash_fm_info {
    int32_t part,type,address,bytes,capacity,used,request,status,phase,error;
    uint64_t borrowed_work;
};
/* A fresh profile may create an erased 128KiB SONIC_FLASH.DAT. Existing files
 * are never reformatted implicitly. The directory must exist. No retail BIOS,
 * factory data or user saves are imported. Creator thread owns all operations.
 * Each primitive persists via flush/atomic replacement before RAM commits.
 * The original FM cache/work and request buffers stay borrowed until finish.
 * Close rejects a pending original FM request; fmMngFinish explicitly drops
 * one, with its original consequences (including an already erased sector).
 */
JP_FLASH_API int jp_flash_open(const wchar_t *directory,uint32_t create_if_missing);
JP_FLASH_API int jp_flash_close(void);
JP_FLASH_API int jp_flash_status(void);
JP_FLASH_API void jp_flash_clear_error(void);
JP_FLASH_API int jp_flash_snapshot(struct jp_flash_info *);
JP_FLASH_API int jp_flash_fm_snapshot(struct jp_flash_fm_info *);
/* Private same-DLL boundaries for the genuine FM provider. */
int jp_flash_owner_access(void);
int jp_flash_fm_begin(void);
void jp_flash_fm_end(void);
int jp_flash_fm_is_open(void);
int jp_flash_fm_close(void);
int jp_flash_argument_error(void);
#endif
