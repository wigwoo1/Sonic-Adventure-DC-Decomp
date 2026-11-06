/* Same native owner, thread, real game wrapper and source-local sound SDK. */
extern int32_t jp_game_qsound_set(const int8_t *);
int jp_sound_set_qsound(const int8_t *positions,uint32_t bytes)
{
    int rc=begin(1);if(rc)return rc;
    if(bytes<7||!outside_stage(positions,7))return end(JP_SOUND_ARGUMENT);
    int32_t sdk=jp_game_qsound_set(positions);
    if((uint32_t)sdk==UINT32_C(0x0a000001))return end(JP_SOUND_QUEUE);
    if(sdk){sound.info.sdk_error=sdk;fail(JP_SOUND_SDK,JP_SOUND_OWNS_SDK,(uint32_t)sdk);return end(JP_SOUND_SDK);}
    return end(0);
}
