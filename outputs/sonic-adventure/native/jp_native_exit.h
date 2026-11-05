#ifndef JP_NATIVE_EXIT_H
#define JP_NATIVE_EXIT_H
#include <stdint.h>
#define JP_EXIT_API __declspec(dllexport)
struct jp_gpu;struct jp_window;
enum jp_exit_resource {
    JP_EXIT_PRESENT=1,JP_EXIT_INPUT=2,JP_EXIT_RECORDING=4,JP_EXIT_WAIT=8,
    JP_EXIT_TASKS=16,JP_EXIT_ASSETS=32,JP_EXIT_QUEUE=64,JP_EXIT_FRAMES=128,
    JP_EXIT_MEMORY=256,JP_EXIT_TEXTURES=512,JP_EXIT_RENDER_STATE=1024,
    JP_EXIT_CLOCK=2048,JP_EXIT_WINDOW=4096,JP_EXIT_GPU=8192,
    JP_EXIT_ALL=16383,JP_EXIT_DEVICE=16384,JP_EXIT_SDK_HEAP=32768,
    JP_EXIT_RTC=65536,JP_EXIT_SUPPORTED=131071,JP_EXIT_WITH_DEVICE=18422,
    JP_EXIT_WITH_DEVICE_SDK_HEAP=51190,JP_EXIT_WITH_DEVICE_SDK_HEAP_RTC=116726
};
enum jp_exit_error {JP_EXIT_OK,JP_EXIT_ARGUMENT,JP_EXIT_NOT_BOUND,
    JP_EXIT_THREAD,JP_EXIT_REENTRANT,JP_EXIT_ALREADY_BOUND,JP_EXIT_SERVICE,
    JP_EXIT_DEVICE_STATE,JP_EXIT_SDK_HEAP_STATE,JP_EXIT_RTC_STATE};
struct jp_exit_config {uint32_t bytes,resources;struct jp_gpu *gpu;struct jp_window *window;};
struct jp_exit_info {uint64_t cleanup_calls;uint32_t remaining,completed,status,failed_resource,native_error;};
/* Bind once, on the game thread, after the selected owners are initialized.
 * Each selected owner and handle transfers shutdown authority to this service.
 * Do not destroy/reinitialize them independently afterward. TASKS requires real
 * task_init, and ASSETS requires real asset initialization. Unselected owners
 * remain caller-owned, permitting cleanup after partial bootstrap. Valid opaque
 * handles must be supplied by their actual owners. A bound exit is terminal;
 * no same-process game restart is implied. */
/* DEVICE delegates one actual device owner; gpu/window must be NULL and
 * PRESENT/WAIT/CLOCK/WINDOW/GPU bits must be absent. WITH_DEVICE selects DEVICE
 * plus the other classic owners. ALL retains its existing classic meaning.
 * Claim occurs only after all other binding validation; rejection leaves the
 * device unclaimed and the exit service unbound. */
/* SDK_HEAP claims the real SDK arena and closes it after all other selected
 * consumers. A failed later device claim cancels the uncommitted heap claim.
 * WITH_DEVICE_SDK_HEAP selects both owners and all other classic resources. */
/* RTC claims the actual clock callback and closes it before device quiescence
 * or a separately selected WAIT. A later rejected claim cancels earlier
 * uncommitted claims. Existing masks retain their existing meanings. */
JP_EXIT_API int jp_exit_bind(const struct jp_exit_config *);
/* Safe to retry after an actual native service failure. Successfully closed
 * owners are never closed twice; the failing/remaining owners stay owned. */
JP_EXIT_API int jp_exit_cleanup(void);
JP_EXIT_API int jp_exit_status(void);
JP_EXIT_API int jp_exit_snapshot(struct jp_exit_info *);
/* Real native replacements for platform-specific system/boot-ROM exit. The
 * first drains owned resources. The second never returns and exits the process
 * with zero only after successful cleanup; failures produce a nonzero code. */
JP_EXIT_API void sbExitSystem(void);
JP_EXIT_API __declspec(noreturn) void syBtExit(void);
#endif
