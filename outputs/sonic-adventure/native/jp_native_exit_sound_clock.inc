int jp_exit_hold_sound_activity(uint64_t token,uint32_t held)
{
    int rc=owner();if(rc)return rc;if(state.busy)return JP_EXIT_REENTRANT;
    if(held>1)return JP_EXIT_ARGUMENT;
    uint64_t actual=state.bound?state.sound_token:bootstrap.sound_token;
    if(!token||actual!=token)return JP_EXIT_SOUND_STATE;
    if(state.sound_activity==held)return JP_EXIT_REENTRANT;
    state.sound_activity=held;return 0;
}
