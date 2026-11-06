#ifndef JP_NATIVE_CHARACTER_SWITCH_H
#define JP_NATIVE_CHARACTER_SWITCH_H
#include <stdint.h>
#ifndef JP_CHARACTER_SWITCH_API
#define JP_CHARACTER_SWITCH_API __declspec(dllexport)
#endif
enum jp_character_switch_error {JP_SWITCH_OK,JP_SWITCH_ARGUMENT,JP_SWITCH_STATE,
    JP_SWITCH_THREAD,JP_SWITCH_REENTRANT,JP_SWITCH_BUSY,JP_SWITCH_TEXTURE,
    JP_SWITCH_MOTION,JP_SWITCH_IDENTITY};
struct jp_character_switch_info {
    uint64_t starts,completions,retirements;
    uint32_t phase,status,native_error,stage;
    int32_t requested,current,owned_character;
    uint32_t creator_thread,save_slot,reserved;
};
/* Borrow the same initialized Character/Assets and Motion owners. Only the six
 * playable IDs are valid. Retire task/draw users before switching. One texture
 * reference belongs to the game until the next switch or explicit retire().
 * Stage1 releases old textures;2 selects motions;3 loads target textures;
 * stage4 completed the exact original lives/Chao tail. Failure retains truthful
 * partial state. Clear reported provider errors before recover(); it resumes
 * that operation rather than saving the old lives into the new character slot.
 * Direct shared-state writes during a partial operation are forbidden. */
JP_CHARACTER_SWITCH_API int jp_character_switch(int32_t);
JP_CHARACTER_SWITCH_API int jp_character_switch_recover(void);
JP_CHARACTER_SWITCH_API int jp_character_switch_retire(void);
JP_CHARACTER_SWITCH_API int jp_character_switch_snapshot(struct jp_character_switch_info *);
#endif
