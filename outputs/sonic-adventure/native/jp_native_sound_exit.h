#ifndef JP_NATIVE_SOUND_EXIT_H
#define JP_NATIVE_SOUND_EXIT_H
#include <stdint.h>
#ifndef JP_SOUND_EXIT_API
#define JP_SOUND_EXIT_API __declspec(dllexport)
#endif
struct jp_sound_exit_info {
    uint64_t next_token,token;
    uint32_t terminal,closing,finished,reserved;
};
/* Transfer lifecycle authority, without interrupting actual frame/audio work.
 * Ordinary configure/init/shutdown reject while claimed. Service/render/pump
 * remain available until delegated close begins. Tokens never recycle.
 * Cancel an untouched claim, or a successfully closed nonterminal claim.
 * A failed close retains the claim and all unfinished original resources.
 * close is nonterminal for ordinary System shutdown/restart; finish commits a
 * terminal exit. Both are idempotent and creator-thread owned. A live callback
 * cannot start cleanup. Never cancel a terminal or partially closed claim.
 */
JP_SOUND_EXIT_API int jp_sound_exit_claim(uint64_t *);
JP_SOUND_EXIT_API int jp_sound_exit_cancel(uint64_t);
JP_SOUND_EXIT_API int jp_sound_exit_close(uint64_t,uint32_t timeout_ms);
JP_SOUND_EXIT_API int jp_sound_exit_finish(uint64_t,uint32_t timeout_ms);
JP_SOUND_EXIT_API int jp_sound_exit_snapshot(struct jp_sound_exit_info *);
#endif
