#ifndef JP_NATIVE_ASSET_EXIT_H
#define JP_NATIVE_ASSET_EXIT_H
#include <stdint.h>
#ifndef JP_ASSET_EXIT_API
#define JP_ASSET_EXIT_API __declspec(dllexport)
#endif
enum jp_asset_exit_error {JP_ASSET_EXIT_ARGUMENT=1,JP_ASSET_EXIT_STATE=2,
    JP_ASSET_EXIT_THREAD=3,JP_ASSET_EXIT_REENTRANT=11,JP_ASSET_EXIT_BUSY=12};
struct jp_asset_exit_info {
    uint64_t next_token,token,decoded_bytes,owned_bytes;
    uint32_t terminal,closing,finished,initialized,creator_thread,status,pending_refs,resident_refs;
};
/* Actual existing asset provider only. Handoff permits live decoded/child
 * views; retirement rejects them before closing flags or parent resources.
 * after_character preflights a dependent SAME-module character capability;
 * the caller must then close that exact child BEFORE closing assets.
 * close_unclaimed is partial-start rollback only: it rejects another claim.
 * Ordinary completed claims cancel for restart; terminal finish cannot cancel. */
JP_ASSET_EXIT_API int jp_assets_exit_activity(void);
JP_ASSET_EXIT_API int jp_assets_exit_claim(uint64_t *);
JP_ASSET_EXIT_API int jp_assets_exit_cancel(uint64_t);
JP_ASSET_EXIT_API int jp_assets_exit_can_close(uint64_t);
JP_ASSET_EXIT_API int jp_assets_exit_can_retire(uint64_t);
JP_ASSET_EXIT_API int jp_assets_exit_can_retire_after_character(uint64_t,uint64_t);
JP_ASSET_EXIT_API int jp_assets_exit_close(uint64_t);
JP_ASSET_EXIT_API int jp_assets_exit_finish(uint64_t);
JP_ASSET_EXIT_API int jp_assets_exit_close_unclaimed(uint32_t);
JP_ASSET_EXIT_API int jp_assets_exit_snapshot(struct jp_asset_exit_info *);
/* Inspect the SAME native memory/PVM source and destination, without unbinding
 * caller-owned work. This is used only after the creating-thread check. */
JP_ASSET_EXIT_API int jp_asset_loader_activity(void);
#endif
