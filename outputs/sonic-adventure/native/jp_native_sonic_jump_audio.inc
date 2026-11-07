#define JP_EFFECTS_API __declspec(dllimport)
#include "jp_native_effects.h"
#undef JP_EFFECTS_API

static void sonic_jump_audio(int prior_mode, int mode)
{
    enum { JUMP_EFFECT = 17, LAND_EFFECT = 33 };
    int effect;
    if (prior_mode != JP_SONIC_JUMP && mode == JP_SONIC_JUMP)
        effect = JUMP_EFFECT;
    else if (prior_mode == JP_SONIC_JUMP && mode == JP_SONIC_STAND)
        effect = LAND_EFFECT;
    else
        return;
    const struct jp_effects_request request = {.bytes = sizeof request, .se = effect};
    int32_t slot;
    (void)jp_effects_request(&request, &slot);
}
