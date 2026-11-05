#ifndef JP_NATIVE_GDFS_H
#define JP_NATIVE_GDFS_H
#include <stdint.h>
#include <stddef.h>
#ifndef JP_GDFS_API
#define JP_GDFS_API __declspec(dllexport)
#endif
enum jp_gdfs_error {JP_GDFS_OK,JP_GDFS_ARGUMENT,JP_GDFS_STATE,JP_GDFS_THREAD,
    JP_GDFS_NOT_INITIALIZED,JP_GDFS_MEMORY,JP_GDFS_MEDIA,JP_GDFS_SOURCE,
    JP_GDFS_CHAIN,JP_GDFS_REENTRANT,JP_GDFS_TIMEOUT,JP_GDFS_ACTIVE,
    JP_GDFS_UNSUPPORTED};
enum jp_gdfs_phase {JP_GDFS_COLD,JP_GDFS_CONFIGURED,JP_GDFS_STARTING,
    JP_GDFS_READY,JP_GDFS_FAILED};
struct jp_gdfs_info {
    uint32_t phase,status;int32_t source_error;
    uint32_t live_handles,max_handles,max_entries,dir_entries,vblank;
    uint32_t handle_bytes,work_bytes,dir_bytes,pending;
    uint64_t work,dir,read_bytes,requests,callbacks,vblanks;
};
/* One creating game thread. Configure a read-only GD data track, then start
 * the already owned real wait/vblank service before initializing GDFS.
 * The original library expects first_fad45150. CPU/DMA modes both use real
 * native I/O/copies; no console DMA interrupt or uncached pointer is faked.
 * Reader buffers/work/handles are private owned native allocations. The SDK
 * gdFsInit's legacy console work arguments are not dereferenced/retained.
 * Do not independently close/request the underlying media while GDFS owns it.
 * CD-DA/TOC/subcode services fail explicitly until real audio/media support.
 * SDK read buffers need sector-rounded capacity; streaming/DMA entry points
 * preserve original 32-byte alignment rules. Large read counts clamp at EOF
 * before the original 32-bit sector arithmetic. Caller buffers stay live until
 * completion or successful stop/close/reinit/shutdown.
 * Callbacks run on this owner thread after actual I/O/copy progress. Transfer
 * callbacks are deferred until original state updates, not physical console
 * interrupts. Queries and nonblocking requeues are allowed; recursive server
 * calls do nothing and blocking I/O/lifecycle calls reject during callbacks.
 * A failed cleanup retains unfinished ownership and must be retried before
 * closing the shared RTC/device/wait or final SDK heap.
 */
JP_GDFS_API int jp_gdfs_configure(const wchar_t *,uint32_t first_fad,
    uint32_t raw_sector_bytes,uint32_t chunk_sectors,uint32_t timeout_ms);
JP_GDFS_API int jp_gdfs_initialize(uint32_t handles,uint32_t directory_entries);
JP_GDFS_API int jp_gdfs_shutdown(void);
JP_GDFS_API int jp_gdfs_status(void);
JP_GDFS_API int jp_gdfs_snapshot(struct jp_gdfs_info *);
/* Delegate terminal lifecycle authority to the native exit owner. Ordinary
 * configure/init/reinit/shutdown reject while delegated; file I/O remains
 * usable until final close begins. Cancel only an uncommitted claim. Finish
 * retains the same token across failure/retry and cannot permit a restart.
 * Tokens never recycle and these APIs never dispatch a user callback.
 */
JP_GDFS_API int jp_gdfs_exit_claim(uint64_t *);
JP_GDFS_API int jp_gdfs_exit_cancel(uint64_t);
JP_GDFS_API int jp_gdfs_exit_finish(uint64_t);
#endif
