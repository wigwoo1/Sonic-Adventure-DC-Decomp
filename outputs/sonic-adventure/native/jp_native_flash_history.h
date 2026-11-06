#ifndef JP_NATIVE_FLASH_HISTORY_H
#define JP_NATIVE_FLASH_HISTORY_H
#include "jp_native_flash.h"
struct jp_flash_history_info {
    uint64_t next_token,token;
    uint32_t active,created_profile,transaction,reserved;
};
/* Explicit ownership over this actual profile and original FM provider.
 * Enter/leave surrounds individual FM calls, never a user RTC callback. */
JP_FLASH_API int jp_flash_history_claim(uint64_t *);
JP_FLASH_API int jp_flash_history_enter(uint64_t);
JP_FLASH_API int jp_flash_history_leave(uint64_t);
JP_FLASH_API int jp_flash_history_cancel(uint64_t);
JP_FLASH_API int jp_flash_history_snapshot(struct jp_flash_history_info *);
JP_FLASH_API int jp_flash_history_transaction_begin(uint64_t);
JP_FLASH_API int jp_flash_history_transaction_commit(uint64_t);
JP_FLASH_API int jp_flash_history_transaction_abort(uint64_t);
#endif
