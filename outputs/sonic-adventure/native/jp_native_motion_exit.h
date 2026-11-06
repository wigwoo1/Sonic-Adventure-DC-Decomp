#ifndef JP_NATIVE_MOTION_EXIT_H
#define JP_NATIVE_MOTION_EXIT_H
#include "jp_native_motion_store.h"
#ifndef JP_MOTION_EXIT_API
#define JP_MOTION_EXIT_API __declspec(dllexport)
#endif
#define JP_EXIT_MOTIONS UINT32_C(16777216)
struct jp_motion_exit_info {
    uint64_t next_token,token;
    uint32_t terminal,closing,finished,reserved;
};
/* Transfer authority over the SAME motion owner. Identity/activity checks allow
 * live views during adoption/final handoff; can_retire additionally requires no
 * views and must precede any parent cleanup. Only delegated close/finish may
 * shut down a claimed owner. Failed retirement leaves views/data/parents usable.
 * Ordinary completed close may cancel for restart; terminal finish cannot. */
JP_MOTION_EXIT_API int jp_motion_exit_activity(void);
JP_MOTION_EXIT_API int jp_motion_exit_claim(uint64_t *);
JP_MOTION_EXIT_API int jp_motion_exit_cancel(uint64_t);
JP_MOTION_EXIT_API int jp_motion_exit_can_close(uint64_t);
JP_MOTION_EXIT_API int jp_motion_exit_can_retire(uint64_t);
JP_MOTION_EXIT_API int jp_motion_exit_close(uint64_t);
JP_MOTION_EXIT_API int jp_motion_exit_finish(uint64_t);
JP_MOTION_EXIT_API int jp_motion_exit_snapshot(struct jp_motion_exit_info *);
#endif
