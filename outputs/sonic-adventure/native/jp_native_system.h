#ifndef JP_NATIVE_SYSTEM_H
#define JP_NATIVE_SYSTEM_H
#include <stdint.h>
#include <stddef.h>
#include "jp_native_device.h"
#ifndef JP_SYSTEM_API
#define JP_SYSTEM_API __declspec(dllexport)
#endif
struct jp_input_sample;
typedef int (*jp_system_input_source)(void *,struct jp_input_sample *);
typedef int (*jp_system_rtc_reader)(void *,uint32_t *);
enum jp_system_error {JP_SYSTEM_OK,JP_SYSTEM_ARGUMENT,JP_SYSTEM_STATE,
    JP_SYSTEM_THREAD,JP_SYSTEM_REENTRANT,JP_SYSTEM_HEAP,JP_SYSTEM_DEVICE,
    JP_SYSTEM_INPUT,JP_SYSTEM_RTC,JP_SYSTEM_GDFS,JP_SYSTEM_DIRECTORY,
    JP_SYSTEM_EXIT,JP_SYSTEM_CLEANUP};
enum jp_system_phase {JP_SYSTEM_COLD,JP_SYSTEM_CONFIGURED,JP_SYSTEM_STARTING,
    JP_SYSTEM_READY,JP_SYSTEM_DELEGATED,JP_SYSTEM_DRAINING,JP_SYSTEM_CLOSED,JP_SYSTEM_FAILED};
struct jp_system_options {
    uint32_t bytes,input_flags;struct jp_device_options device;
    const wchar_t *track;uint32_t raw_sector_bytes,chunk_sectors,timeout_ms,reserved;
    jp_system_input_source input_source;void *input_context;
    jp_system_rtc_reader rtc_reader;void *rtc_context;
};
struct jp_system_info {
    uint32_t phase,status,resources,failed_resource,native_error;
    int32_t mode,frame,count;uint32_t attempts,reserved;
    uint64_t initializations,cleanups,window;
};
/* This compiled sbInitSystem owns the actual 1MiB SDK heap, native device,
 * configured keyboard/controller or explicit sample provider, RTC and original
 * GDFS. Borrow already initialized, empty queue/frame/texture owners; do not
 * replace those owners during system use. The options and full track path are
 * copied. Supplied callbacks/context remain borrowed until shutdown.
 * Native settings replace console memory/DMA/cache/interrupt assumptions.
 * No syHwInit, SH interrupt or _BSG_END success stub is supplied.
 * Failed initialization unwinds only acquired resources and retains failed
 * cleanup ownership for retry. A successful startup registers real early-exit
 * cleanup; final bind must also cover all owned core resources. Borrowed render
 * owners become exit-owned only when the caller explicitly includes them.
 */
JP_SYSTEM_API int jp_system_configure(const struct jp_system_options *);
JP_SYSTEM_API int jp_system_initialize(int32_t mode,int32_t frame,int32_t count);
JP_SYSTEM_API int jp_system_bind_exit(uint32_t complete_resource_mask);
JP_SYSTEM_API int jp_system_shutdown(void);
JP_SYSTEM_API int jp_system_status(void);
JP_SYSTEM_API int jp_system_snapshot(struct jp_system_info *);
/* Original ABI: successful calls return normally; initialization failure
 * cleans up and exits with a nonzero process status instead of falling through.
 */
JP_SYSTEM_API void sbInitSystem(int32_t mode,int32_t frame,int32_t count);
#endif
