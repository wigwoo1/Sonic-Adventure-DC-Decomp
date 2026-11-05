#ifndef JP_NATIVE_SAVE_DISK_H
#define JP_NATIVE_SAVE_DISK_H
#include <stdint.h>
#include <stddef.h>
#define JP_SAVE_DISK_API __declspec(dllexport)
enum jp_save_disk_error {JP_SAVE_DISK_OK,JP_SAVE_DISK_ARGUMENT,
    JP_SAVE_DISK_NOT_INITIALIZED,JP_SAVE_DISK_STATE,JP_SAVE_DISK_THREAD,
    JP_SAVE_DISK_IO,JP_SAVE_DISK_FORMAT};
struct jp_save_disk_info {uint64_t reads,writes,applies;uint32_t status,os_error,record_bytes,record_count;};
/* One game thread, caller-owned initialized game/save state and an existing
 * save directory. No menu/card callback is substituted. The sole file is
 * SONICADV_JP.SAV: three exact original Japanese serialized save records.
 * Call genuine save_store/save_update explicitly before writing if desired.
 * These APIs do not run the game or initialize its progress themselves. */
JP_SAVE_DISK_API int jp_save_disk_initialize(const wchar_t *existing_directory);
JP_SAVE_DISK_API int jp_save_disk_shutdown(void);
JP_SAVE_DISK_API int jp_save_disk_status(void);
JP_SAVE_DISK_API void jp_save_disk_clear_error(void);
JP_SAVE_DISK_API int jp_save_disk_snapshot(struct jp_save_disk_info *);
/* Write validates current snapshots, flushes a same-directory temporary and
 * atomically replaces the file. A failed write preserves the prior file.
 * Read validates exact length, all original CRCs and character-index bounds
 * before touching live snapshots. Other game fields retain original semantics;
 * this is not a complete semantic validator for every future scene/script. */
JP_SAVE_DISK_API int jp_save_disk_write(void);
JP_SAVE_DISK_API int jp_save_disk_read(void);
/* Validate current snapshots, choose slot 0..2 and execute genuine save_load.
 * Read does not apply a slot or change live game progress by itself. */
JP_SAVE_DISK_API int jp_save_disk_apply(uint32_t slot);
#endif
