#ifndef JP_NATIVE_CHARACTER_EXIT_H
#define JP_NATIVE_CHARACTER_EXIT_H
#include <stdint.h>
#ifndef JP_CHARACTER_EXIT_API
#define JP_CHARACTER_EXIT_API __declspec(dllexport)
#endif
#define JP_EXIT_CHARACTER_TEXTURES UINT32_C(33554432)
enum jp_character_exit_error {JP_CHARACTER_EXIT_ARGUMENT=1,JP_CHARACTER_EXIT_STATE=2,
    JP_CHARACTER_EXIT_THREAD=3,JP_CHARACTER_EXIT_REENTRANT=4,JP_CHARACTER_EXIT_BUSY=8};
struct jp_character_exit_info {
    uint64_t next_token,token;
    uint32_t terminal,closing,finished,active,creator_thread,status,users,partial;
};
/* Same genuine character_users/lists and actual checked texture owner. Claim
 * and final handoff may retain live logical users. Retirement requires zero
 * users and no borrowed loader operation; owned partial loads recover through
 * the real existing asset/PVM/texture providers before shutdown. */
JP_CHARACTER_EXIT_API int jp_character_exit_activity(void);
JP_CHARACTER_EXIT_API int jp_character_exit_claim(uint64_t *);
JP_CHARACTER_EXIT_API int jp_character_exit_cancel(uint64_t);
JP_CHARACTER_EXIT_API int jp_character_exit_can_close(uint64_t);
JP_CHARACTER_EXIT_API int jp_character_exit_can_retire(uint64_t);
JP_CHARACTER_EXIT_API int jp_character_exit_close(uint64_t);
JP_CHARACTER_EXIT_API int jp_character_exit_finish(uint64_t);
JP_CHARACTER_EXIT_API int jp_character_exit_close_unclaimed(uint32_t);
JP_CHARACTER_EXIT_API int jp_character_exit_snapshot(struct jp_character_exit_info *);
#endif
