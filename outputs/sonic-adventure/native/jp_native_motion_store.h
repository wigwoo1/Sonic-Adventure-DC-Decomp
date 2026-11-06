#ifndef JP_NATIVE_MOTION_STORE_H
#define JP_NATIVE_MOTION_STORE_H
#include <stddef.h>
#include <stdint.h>
#ifndef JP_MOTION_API
#define JP_MOTION_API __declspec(dllexport)
#endif
enum jp_motion_error {JP_MOTION_OK,JP_MOTION_ARGUMENT,JP_MOTION_STATE,JP_MOTION_THREAD,
    JP_MOTION_REENTRANT,JP_MOTION_LIMIT,JP_MOTION_ALLOCATION,JP_MOTION_BUSY,
    JP_MOTION_NOT_FOUND,JP_MOTION_CHARACTER,JP_MOTION_IDENTITY};
enum jp_motion_bank {JP_MOTION_SONIC,JP_MOTION_AMY,JP_MOTION_BIG,JP_MOTION_E102,
    JP_MOTION_KNUCKLES,JP_MOTION_SONIC_BOARD,JP_MOTION_TAILS_BOARD,JP_MOTION_MODELS,
    JP_MOTION_BANK_COUNT};
#define JP_MOTION_NO_BANK UINT32_MAX
struct jp_motion_info {
    uint64_t starts,selections,board_resets,acquires,releases,generation,resident_bytes,allocated_bytes;
    uint32_t phase,status,creator_thread,selected_bank;
    int32_t character;
    uint32_t live_leases,ready_mask,reserved;
};
struct jp_motion_view {
    uint64_t token,generation;
    void *data;size_t bytes;
    uint32_t bank,index,kind,reserved;
};
/* One game thread owns the verified compiled data pack. Initialize before any
 * direct data mutation; keep its DLL loaded until all views and this owner close.
 * budget covers metadata/default backups/leases, beyond the resident module data.
 * Primary selection executes the original valid character/file switch and
 * restores the target bank's defaults, including same-bank reloads. Existing
 * views of the old/target primary bank prevent selection. Shared model/independent
 * board views remain usable. Board reset protects only that board's views.
 * Acquire by the original case-sensitive symbol name; its kind is the qualified
 * registry kind. Keep a view alive through every task/retained draw using it.
 * Released views expire logically even while their static address still exists.
 * Shutdown rejects all live views and restores every record before freeing its
 * own memory. This owner is not yet attached to System/Exit or renderer jobs.
 */
JP_MOTION_API int jp_motions_initialize(size_t budget,uint32_t lease_capacity);
JP_MOTION_API int jp_motions_select(int32_t character);
JP_MOTION_API int jp_motions_reset_board(uint32_t bank);
JP_MOTION_API int jp_motions_acquire(uint32_t bank,const char *name,struct jp_motion_view *);
JP_MOTION_API int jp_motions_release(uint64_t token);
JP_MOTION_API int jp_motions_shutdown(void);
JP_MOTION_API int jp_motions_snapshot(struct jp_motion_info *);
#endif
