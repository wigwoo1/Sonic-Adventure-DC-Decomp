/* Appended to the compiled System owner, sharing its one bootstrap callback. */
static int system_sound_start_done(int rc)
{
    if(system_owner.registered){int guard=jp_exit_hold_bootstrap(bootstrap_cleanup,&system_owner,0);
        if(guard){system_owner.busy=0;return result(JP_SYSTEM_EXIT,0,guard);}}
    system_owner.busy=0;return rc;
}
static int system_sound_close(void)
{
    if(system_owner.sound_token){
        return system_owner.terminal?jp_sound_exit_finish(system_owner.sound_token,system_owner.sound_timeout):
            jp_sound_exit_close(system_owner.sound_token,system_owner.sound_timeout);
    }
    return jp_sound_shutdown(system_owner.sound_timeout);
}
static int system_sound_release(void)
{
    if(!system_owner.sound_token||system_owner.terminal)return 0;
    int rc=jp_sound_exit_cancel(system_owner.sound_token);if(rc)return rc;
    system_owner.sound_token=0;return 0;
}
int jp_system_start_sound(const struct jp_sound_options *options)
{
    int rc=access_owner();if(rc)return rc;if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    if(system_owner.terminal||!system_owner.registered||system_owner.info.phase!=JP_SYSTEM_READY||
       (system_owner.info.resources&JP_EXIT_SOUND)||system_owner.sound_token)return JP_SYSTEM_STATE;
    struct jp_sound_info prior;struct jp_sound_exit_info delegation;
    if(jp_sound_snapshot(&prior)||prior.resources||jp_sound_exit_snapshot(&delegation)||
       delegation.token||delegation.terminal)return JP_SYSTEM_STATE;
    rc=jp_exit_hold_bootstrap(bootstrap_cleanup,&system_owner,1);if(rc)return result(JP_SYSTEM_EXIT,0,rc);
    system_owner.busy=1;rc=jp_sound_configure(options);
    if(rc)return system_sound_start_done(result(JP_SYSTEM_SOUND,JP_EXIT_SOUND,rc));
    system_owner.sound_timeout=options->close_timeout_ms;
    rc=jp_exit_attach_bootstrap_sound(bootstrap_cleanup,&system_owner,0,system_owner.sound_timeout);
    if(rc)return system_sound_start_done(result(JP_SYSTEM_EXIT,JP_EXIT_SOUND,rc));
    system_owner.info.resources|=JP_EXIT_SOUND;
    rc=jp_sound_initialize();if(rc)goto failed;
    rc=jp_sound_exit_claim(&system_owner.sound_token);if(rc)goto failed;
    rc=jp_exit_attach_bootstrap_sound(bootstrap_cleanup,&system_owner,system_owner.sound_token,system_owner.sound_timeout);
    if(rc)goto failed;
    return system_sound_start_done(result(0,0,0));
failed:
    {int native=rc;int cleanup=shutdown_owned();
     if(!cleanup&&system_owner.registered){
         cleanup=jp_exit_cancel_bootstrap(bootstrap_cleanup,&system_owner);
         if(!cleanup)system_owner.registered=0;
     }
     if(!cleanup)cleanup=system_sound_release();
     if(cleanup)return system_sound_start_done(result(JP_SYSTEM_CLEANUP,JP_EXIT_SOUND,cleanup));
     system_owner.info.phase=JP_SYSTEM_FAILED;return system_sound_start_done(result(JP_SYSTEM_SOUND,JP_EXIT_SOUND,native));}
}
