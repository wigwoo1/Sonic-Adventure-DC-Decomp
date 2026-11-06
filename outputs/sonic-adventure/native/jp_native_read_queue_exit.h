#ifndef JP_NATIVE_READ_QUEUE_EXIT_H
#define JP_NATIVE_READ_QUEUE_EXIT_H
#include "jp_native_read_queue.h"
#ifndef JP_READ_QUEUE_EXIT_API
#define JP_READ_QUEUE_EXIT_API __declspec(dllexport)
#endif
#define JP_EXIT_READ_QUEUE UINT32_C(8388608)
struct jp_read_queue_exit_info {
    uint64_t next_token,token;
    uint32_t terminal,closing,finished,reserved;
};
/* Authority over the actual original queue and its existing GDFS client lease.
 * Data operations remain available until close starts. A failed close retains
 * the token, file/buffer ownership and lease for retry. Terminal finish cannot
 * be cancelled/restarted. Completed ordinary close may cancel for restart. */
JP_READ_QUEUE_EXIT_API int jp_read_queue_exit_activity(void);
JP_READ_QUEUE_EXIT_API int jp_read_queue_exit_claim(uint64_t *);
JP_READ_QUEUE_EXIT_API int jp_read_queue_exit_cancel(uint64_t);
JP_READ_QUEUE_EXIT_API int jp_read_queue_exit_can_close(uint64_t);
JP_READ_QUEUE_EXIT_API int jp_read_queue_exit_close(uint64_t);
JP_READ_QUEUE_EXIT_API int jp_read_queue_exit_finish(uint64_t);
JP_READ_QUEUE_EXIT_API int jp_read_queue_exit_snapshot(struct jp_read_queue_exit_info *);
#endif
