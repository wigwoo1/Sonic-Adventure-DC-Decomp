static int system_clock_activity(void *ctx,uint32_t entering)
{
    (void)ctx;int rc=access_owner();if(rc)return rc;
    if(entering){
        if(system_owner.busy)return JP_SYSTEM_REENTRANT;
        if(!system_owner.sound_token||(system_owner.info.phase!=JP_SYSTEM_READY&&system_owner.info.phase!=JP_SYSTEM_DELEGATED))return JP_SYSTEM_STATE;
        rc=jp_exit_hold_sound_activity(system_owner.sound_token,1);if(rc)return rc;
        system_owner.busy=1;return 0;
    }
    rc=jp_exit_hold_sound_activity(system_owner.sound_token,0);system_owner.busy=0;return rc;
}
int jp_system_start_sound_clock(const struct jp_sound_options *options,const struct jp_sound_clock_options *clock_options)
{
    int rc=access_owner();if(rc)return rc;if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    struct jp_wait_service_info prior;
    if(jp_wait_service_snapshot(&prior)||prior.bound)return JP_SYSTEM_STATE;
    struct jp_sound_clock_options copied;
    if(clock_options){if(clock_options->bytes!=sizeof copied||clock_options->activity||clock_options->activity_context)return JP_SYSTEM_ARGUMENT;
        copied=*clock_options;copied.activity=system_clock_activity;copied.activity_context=&system_owner;}
    rc=jp_system_start_sound(options);if(rc)return rc;
    rc=jp_exit_hold_bootstrap(bootstrap_cleanup,&system_owner,1);if(rc)return result(JP_SYSTEM_EXIT,0,rc);
    system_owner.busy=1;rc=jp_sound_clock_start(clock_options?&copied:NULL);rc=system_sound_start_done(rc);
    if(!rc)return 0;
    int native=rc;rc=jp_system_shutdown();
    if(rc)return rc;return result(JP_SYSTEM_SOUND,JP_EXIT_SOUND,native);
}
