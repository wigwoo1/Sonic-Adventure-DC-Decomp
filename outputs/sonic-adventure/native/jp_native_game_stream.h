#ifndef JP_NATIVE_GAME_STREAM_H
#define JP_NATIVE_GAME_STREAM_H
#include <stdint.h>
#ifndef JP_GAME_STREAM_API
#define JP_GAME_STREAM_API __declspec(dllexport)
#endif
struct jp_game_stream_info {
    uint64_t initializations,shutdowns,steps,errors,task,pending_task,music,voice;
    uint32_t active,stopping,ready,status,children,archive_owned;
    int32_t music_request,voice_request,music_current,music_saved,music_state,voice_state,native_error,reserved;
};
/* Borrows an initialized original game task pool and active native ADXT/ADXF
 * owners. Initialize schedules the actual original task; its normal game task
 * pass performs initialization. Never resets the game heap or task pool, and
 * never steps unrelated tasks from the audio service. Return jp_sound_error.
 * Partition0 is exclusively reserved while this owner holds the voice table.
 * Native requests require ready; -1 cancels a pending request as in the game.
 */
JP_GAME_STREAM_API int jp_game_stream_initialize(void);
JP_GAME_STREAM_API int jp_game_stream_shutdown(void);
JP_GAME_STREAM_API int jp_game_stream_snapshot(struct jp_game_stream_info *);
JP_GAME_STREAM_API int jp_game_stream_request_music(int32_t);
JP_GAME_STREAM_API int jp_game_stream_request_voice(int32_t);
JP_GAME_STREAM_API int jp_game_stream_request_interlude(int32_t);
enum jp_game_stream_action {JP_STREAM_STOP_MUSIC,JP_STREAM_STOP_VOICE,
    JP_STREAM_MUTE,JP_STREAM_UNMUTE,JP_STREAM_RESTART_MUSIC,JP_STREAM_RESTORE_MUSIC,
    JP_STREAM_RESET_MUSIC,JP_STREAM_RESET_VOICE,JP_STREAM_SAVE_MUSIC};
JP_GAME_STREAM_API int jp_game_stream_action(uint32_t action,int32_t value,int32_t *result);
#endif
