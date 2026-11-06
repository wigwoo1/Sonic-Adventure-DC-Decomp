#ifndef JP_NATIVE_HISTORY_H
#define JP_NATIVE_HISTORY_H
#include <stdint.h>
#include <stddef.h>
#ifndef JP_HISTORY_API
#define JP_HISTORY_API __declspec(dllexport)
#endif
enum jp_history_error {JP_HISTORY_OK,JP_HISTORY_ARGUMENT,JP_HISTORY_STATE,
    JP_HISTORY_THREAD,JP_HISTORY_REENTRANT,JP_HISTORY_FLASH,JP_HISTORY_RTC,
    JP_HISTORY_INPUT,JP_HISTORY_IDENTITY,JP_HISTORY_MEMORY,JP_HISTORY_SOURCE};
enum jp_history_phase {JP_HISTORY_COLD,JP_HISTORY_CONFIGURED,JP_HISTORY_STARTING,
    JP_HISTORY_READY,JP_HISTORY_DRAINING,JP_HISTORY_FAILED};
struct jp_history_options {
    uint32_t bytes,create_if_missing;
    const wchar_t *directory,*track;
    uint32_t raw_sector_bytes,reserved;
};
struct jp_history_info {
    uint64_t starts,saves,closes,flash_token,work3,work4;
    uint32_t phase,status,native_error,creator_thread,resources,dirty;
};
/* RTC/input remain borrowed existing owners and must outlive this service.
 * Configure copies paths. Only an actually newly created native profile may
 * be seeded; an existing corrupt/unknown profile is never reformatted.
 * Failed saves/shutdown retain the real UF/work/profile ownership. Recover
 * clears corrected media errors and rebuilds its FM cache before retry.
 */
JP_HISTORY_API int jp_history_configure(const struct jp_history_options *);
JP_HISTORY_API int jp_history_initialize(void);
JP_HISTORY_API int jp_history_save(void);
JP_HISTORY_API int jp_history_read(uint16_t times[24],uint32_t starts[3]);
JP_HISTORY_API int jp_history_count_play(int32_t);
JP_HISTORY_API int jp_history_count_clear(int32_t);
JP_HISTORY_API int jp_history_recover(void);
JP_HISTORY_API int jp_history_refresh(void);
JP_HISTORY_API int jp_history_shutdown(void);
JP_HISTORY_API int jp_history_snapshot(struct jp_history_info *);
/* Internal ABI shared with the privately compiled real SDK/game source. */
int jp_history_source_begin_flash(void);
void jp_history_source_end_flash(void);
void jp_history_source_problem(uint32_t,uint32_t);
const char *jp_history_boot_header(void);
void *jp_history_work(uint32_t);
int jp_history_peer_begin(int);
int jp_history_peer_end(void);
#endif
