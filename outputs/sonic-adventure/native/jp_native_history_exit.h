#ifndef JP_NATIVE_HISTORY_EXIT_H
#define JP_NATIVE_HISTORY_EXIT_H
#include "jp_native_history.h"
#ifndef JP_HISTORY_EXIT_API
#define JP_HISTORY_EXIT_API __declspec(dllexport)
#endif
#define JP_EXIT_HISTORY UINT32_C(4194304)
struct jp_history_exit_info {
    uint64_t next_token,token;
    uint32_t terminal,closing,finished,reserved;
};
/* Lifecycle authority over the existing UF/profile/work owner. Data operations
 * remain valid until close starts; recover remains valid after a failed close.
 * Terminal finish cannot be cancelled or restarted. A completed nonterminal
 * close may be cancelled for an ordinary same-process System restart. */
JP_HISTORY_EXIT_API int jp_history_exit_claim(uint64_t *);
JP_HISTORY_EXIT_API int jp_history_exit_cancel(uint64_t);
JP_HISTORY_EXIT_API int jp_history_exit_close(uint64_t);
JP_HISTORY_EXIT_API int jp_history_exit_finish(uint64_t);
JP_HISTORY_EXIT_API int jp_history_exit_can_close(uint64_t);
JP_HISTORY_EXIT_API int jp_history_exit_activity(void);
JP_HISTORY_EXIT_API int jp_history_exit_snapshot(struct jp_history_exit_info *);
#endif
