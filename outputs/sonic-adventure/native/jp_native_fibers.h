#ifndef JP_NATIVE_FIBERS_H
#define JP_NATIVE_FIBERS_H
#include <stdint.h>
#define JP_RF_API __declspec(dllexport)
struct rf_server;
enum jp_rf_error { JP_RF_OK, JP_RF_ARGUMENT, JP_RF_THREAD, JP_RF_STATE,
                   JP_RF_SYSTEM, JP_RF_CAPACITY };
struct jp_rf_info { uint64_t switches, passes; uint32_t servers, fibers,
    status, os_error; };
/* One owning thread; cooperative execution, never worker-thread callbacks.
 * Original 4KB SH-4 stack arrays remain caller metadata. Windows owns real
 * native stacks and preserves floating-point context. No copied jmp_buf. */
JP_RF_API int jp_rf_status(void);
JP_RF_API int jp_rf_snapshot(struct jp_rf_info *out);
JP_RF_API void jp_rf_clear_error(void);
JP_RF_API void rfInitSystem(long unused);
JP_RF_API void rfExitSystem(void);
JP_RF_API struct rf_server *rfCreateServer(long capacity);
JP_RF_API void rfDestroyServer(struct rf_server *server);
JP_RF_API long rfCreateFiber(struct rf_server *,void (*entry)(void),void *,unsigned long);
JP_RF_API long rfKillFiber(struct rf_server *,long slot);
JP_RF_API void rfKillFiberLevel(struct rf_server *,long level);
JP_RF_API void rfWakeupFiber(struct rf_server *,long slot);
JP_RF_API void rfExecFiberServer(struct rf_server *);
JP_RF_API void rfEndFiber(void);
JP_RF_API void rfSleepFiber(long ticks);
JP_RF_API void rfSleepVFiber(void);
#endif
