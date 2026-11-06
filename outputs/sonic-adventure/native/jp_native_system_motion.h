#ifndef JP_NATIVE_SYSTEM_MOTION_H
#define JP_NATIVE_SYSTEM_MOTION_H
#include "jp_native_system.h"
#include "jp_native_motion_exit.h"
/* Start after real System/MainBoot readiness. Explicit adoption requires the
 * existing initialization guard and an active unclaimed owner; live views may
 * transfer with it. The same bootstrap registration retains actual authority. */
JP_SYSTEM_API int jp_system_start_motions(size_t,uint32_t);
JP_SYSTEM_API int jp_system_adopt_motions(void);
/* Lower MainBoot calls this before card retirement. Delegated System state
 * still checks the retained motion token even after resource-mask transfer. */
JP_SYSTEM_API int jp_system_motion_cleanup_activity(void);
/* When motions are System-owned, execute the same ordered System shutdown
 * before MainBoot's older card-first path. Return retired ownership bits so
 * MainBoot clears only flags actually delegated to that System instance.
 * Zero means the ordinary pre-motion MainBoot path still owns its cleanup. */
JP_SYSTEM_API int jp_system_prepare_boot_shutdown(uint32_t *retired_resources);
#endif
