/* Authored file-backed data-track transport for the original GDFS software.
 * This executes native I/O, not game instructions or a boot-ROM replacement. */
#ifndef JP_NATIVE_GD_MEDIA_H
#define JP_NATIVE_GD_MEDIA_H
#include <stdint.h>
#include <stddef.h>
#ifndef JP_GD_API
#define JP_GD_API __declspec(dllexport)
#endif
enum jp_gd_error {JP_GD_OK,JP_GD_ARGUMENT,JP_GD_NOT_INITIALIZED,JP_GD_THREAD,
    JP_GD_STATE,JP_GD_RANGE,JP_GD_MEMORY,JP_GD_IO,JP_GD_FORMAT,JP_GD_BUSY,
    JP_GD_TIMEOUT};
enum jp_gd_request_state {JP_GD_IDLE,JP_GD_READING,JP_GD_COMPLETE,
    JP_GD_FAILED,JP_GD_CANCELLING,JP_GD_CANCELLED};
struct jp_gd_media_info {
    uint64_t file_bytes,requests,completed,cancelled,raw_bytes_read,payload_bytes;
    uint32_t first_fad,sectors,raw_sector_bytes,maximum_request_sectors;
    uint32_t request_fad,request_sectors,state,status,os_error,initialized;
};
/* One creator-thread owner, one pending request. The input is read-only and
 * held against concurrent writers. first_fad is track LBA+150, e.g.45150.
 * Supported data is cooked2048 or Mode1 raw2352 (validated16byte headers).
 * No Mode2, ECC correction, audio tracks, disc timing or CD-DA is emulated.
 * Maximum request count is explicit (1..1048576), bounded again by the actual
 * track and DWORD-sized raw read. Buffers are owned until release/close.
 */
JP_GD_API int jp_gd_media_open(const wchar_t *,uint32_t first_fad,
    uint32_t raw_sector_bytes,uint32_t maximum_request_sectors);
JP_GD_API int jp_gd_media_request(uint32_t fad,uint32_t sectors);
/* A healthy poll returns0 even while READING; inspect state in snapshot.
 * File bytes enter private staging; output becomes visible only after the
 * complete request and every raw sector header have been validated. */
JP_GD_API int jp_gd_media_poll(void);
JP_GD_API int jp_gd_media_wait(uint32_t timeout_ms);
JP_GD_API int jp_gd_media_snapshot(struct jp_gd_media_info *);
JP_GD_API int jp_gd_media_status(void);
/* Borrowed contiguous2048-byte-sector view; lifetime ends at release/close.
 * Native consumers retain no OS write pointer and must finish using the view
 * before releasing it. Output arguments cannot overlap. */
JP_GD_API int jp_gd_media_view(const void **,size_t *);
JP_GD_API int jp_gd_media_release(void);
/* Cancel/drain waits are bounded to0..60000ms. A timeout retains every file,
 * event and buffer; retry/poll the same owner. Never free pending OS I/O.
 * Close cancels first, then closes only after actual completion/drain. */
JP_GD_API int jp_gd_media_cancel(uint32_t timeout_ms);
JP_GD_API int jp_gd_media_close(uint32_t timeout_ms);
#endif
