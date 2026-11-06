/* Appended to the real global exit owner; never installs another callback. */
int jp_exit_hold_bootstrap(jp_exit_bootstrap_cleanup cleanup,void *context,uint32_t held)
{
    int rc=owner();if(rc)return rc;if(state.busy)return JP_EXIT_REENTRANT;
    if(held>1)return JP_EXIT_ARGUMENT;
    if(state.bound||bootstrap.cleanup!=cleanup||bootstrap.context!=context)return JP_EXIT_ALREADY_BOUND;
    if(bootstrap.held==held)return JP_EXIT_REENTRANT;
    bootstrap.held=held;return 0;
}
int jp_exit_attach_bootstrap_sound(jp_exit_bootstrap_cleanup cleanup,void *context,
    uint64_t token,uint32_t timeout)
{
    int rc=owner();if(rc)return rc;if(state.busy)return JP_EXIT_REENTRANT;
    if(state.bound||bootstrap.started||bootstrap.cleanup!=cleanup||bootstrap.context!=context||
       bootstrap.sound_token)return JP_EXIT_ALREADY_BOUND;
    if(!timeout||timeout>60000)return JP_EXIT_ARGUMENT;
    if(!token){
        if(bootstrap.resources&JP_EXIT_SOUND)return JP_EXIT_ALREADY_BOUND;
        bootstrap.sound_timeout=timeout;bootstrap.resources|=JP_EXIT_SOUND;return 0;
    }
    struct jp_sound_exit_info claim;struct jp_sound_info sound;
    if(jp_sound_exit_snapshot(&claim)||jp_sound_snapshot(&sound)||claim.token!=token||
       claim.terminal||claim.closing||sound.phase!=JP_SOUND_READY||sound.status)return JP_EXIT_SOUND_STATE;
    bootstrap.sound_token=token;bootstrap.sound_timeout=timeout;bootstrap.resources|=JP_EXIT_SOUND;
    return 0;
}
