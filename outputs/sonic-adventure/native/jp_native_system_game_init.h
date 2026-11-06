#ifndef JP_NATIVE_SYSTEM_GAME_INIT_H
#define JP_NATIVE_SYSTEM_GAME_INIT_H
#include <stdint.h>
#ifndef JP_SYSTEM_GAME_INIT_API
#define JP_SYSTEM_GAME_INIT_API __declspec(dllexport)
#endif
struct jp_history_options;
/* One higher-constructor scope in the existing System instance. It holds the
 * actual bootstrap guard and blocks ordinary cleanup/binding while callbacks
 * may run inside the two frame waits. Sound-clock data service remains usable.
 * Only this token's checked queue/history adapters can start new dependencies.
 * Release the scope before explicit constructor-failure cleanup. */
JP_SYSTEM_GAME_INIT_API int jp_system_game_init_claim(uint64_t *);
JP_SYSTEM_GAME_INIT_API int jp_system_game_init_release(uint64_t);
/* Returns REENTRANT while the higher constructor owns its scope, otherwise 0.
 * Lower MainBoot checks this before it can retire the original card backend. */
JP_SYSTEM_GAME_INIT_API int jp_system_game_init_activity(void);
JP_SYSTEM_GAME_INIT_API int jp_system_game_init_queue(uint64_t);
JP_SYSTEM_GAME_INIT_API int jp_system_game_init_history(uint64_t,const struct jp_history_options *);
#endif
