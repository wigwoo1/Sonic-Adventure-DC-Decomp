#ifndef JP_NATIVE_GAME_INIT_H
#define JP_NATIVE_GAME_INIT_H
#include <stdint.h>
#ifndef JP_GAME_INIT_API
#define JP_GAME_INIT_API __declspec(dllexport)
#endif
struct jp_history_options;
enum jp_game_init_error {JP_GAME_INIT_OK,JP_GAME_INIT_ARGUMENT,JP_GAME_INIT_STATE,
    JP_GAME_INIT_THREAD,JP_GAME_INIT_REENTRANT,JP_GAME_INIT_PARENT,JP_GAME_INIT_FRAME,
    JP_GAME_INIT_AUDIO,JP_GAME_INIT_QUEUE,JP_GAME_INIT_HISTORY,JP_GAME_INIT_CALLBACK,
    JP_GAME_INIT_CLEANUP,JP_GAME_INIT_SCOPE};
enum jp_game_init_phase {JP_GAME_COLD,JP_GAME_CONFIGURED,JP_GAME_STARTING,
    JP_GAME_READY,JP_GAME_FAILED,JP_GAME_CLOSED};
enum jp_game_init_stage {JP_GAME_STAGE_NONE,JP_GAME_STAGE_PREINIT,
    JP_GAME_STAGE_PARENT,JP_GAME_STAGE_FRAMES,JP_GAME_STAGE_AUDIO,
    JP_GAME_STAGE_QUEUE,JP_GAME_STAGE_HISTORY,JP_GAME_STAGE_CALLBACK,
    JP_GAME_STAGE_READY};
struct jp_game_init_info {
    uint64_t starts,completions,shutdowns,preinit_calls,parent_calls,frame_calls,
        frame_completed,audio_calls,queue_calls,history_calls,loader_error_calls,
        logical_ticks_before,logical_ticks_after,stack_base,stack_end;
    uint32_t phase,status,native_error,creator_thread,parent_owned,scope_held,stage;
    int32_t game_mode,loader_error_flag,last_loader_error;
    uint32_t audio_stages,preinit_mode,callback_bound,system_phase;
};
/* Configure System/MainBoot and their borrowed render/VMU prerequisites first,
 * as documented in their guides. This copies the history paths/settings and
 * runs the complete original game_init successful order with checked native
 * adapters. Failed initialization retains actual parent/scope ownership for
 * explicit shutdown/retry. Legacy fatal System failures still exit nonzero.
 * The Sofdec stack is real retained configuration storage, not an active
 * Windows interrupt stack. Native OS interrupt handling remains OS-owned.
 */
JP_GAME_INIT_API int jp_game_init_configure(const struct jp_history_options *);
JP_GAME_INIT_API int jp_game_initialize(void);
JP_GAME_INIT_API int jp_game_init_shutdown(uint32_t);
JP_GAME_INIT_API int jp_game_init_snapshot(struct jp_game_init_info *);
JP_GAME_INIT_API void game_init(void);
#endif
