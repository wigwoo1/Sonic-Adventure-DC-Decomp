#ifndef JP_NATIVE_WAIT_SERVICE_H
#define JP_NATIVE_WAIT_SERVICE_H
#include <stdint.h>
#ifndef JP_WAIT_SERVICE_API
#define JP_WAIT_SERVICE_API __declspec(dllexport)
#endif
typedef int32_t (*jp_wait_service_callback)(void *);
enum jp_wait_service_error {JP_WAIT_SERVICE_OK,JP_WAIT_SERVICE_ARGUMENT,
    JP_WAIT_SERVICE_STATE,JP_WAIT_SERVICE_THREAD,JP_WAIT_SERVICE_REENTRANT,
    JP_WAIT_SERVICE_CALLBACK};
struct jp_wait_service_info {
    uint64_t token,calls,failures,next_token;
    uint32_t bound,in_callback,creator_thread,period_ms,status;
    int32_t native_result;
};
/* Optional creator-thread service around real native message waits. The actual
 * initialized wait owner must authorize bind. No worker calls this callback.
 * Borrow its context until exact-token unbind; active callbacks cannot retire
 * or replace it. An unbound run is an optional-service no-op, not audio success.
 */
JP_WAIT_SERVICE_API int jp_wait_service_bind(jp_wait_service_callback,void *,uint32_t period_ms,uint64_t *);
JP_WAIT_SERVICE_API int jp_wait_service_unbind(uint64_t);
JP_WAIT_SERVICE_API int jp_wait_service_run(void);
JP_WAIT_SERVICE_API uint32_t jp_wait_service_limit(uint32_t requested_ms);
JP_WAIT_SERVICE_API int jp_wait_service_snapshot(struct jp_wait_service_info *);
#endif
