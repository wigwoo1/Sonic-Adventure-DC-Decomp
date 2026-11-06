/* Same System owner and exactly the existing bootstrap callback/context. */
static int system_history_activity(void)
{
    if(!(system_owner.info.resources&JP_EXIT_HISTORY))return 0;
    int rc=system_owner.history_token?jp_history_exit_can_close(system_owner.history_token):jp_history_exit_activity();
    return !rc?0:rc==JP_HISTORY_THREAD?JP_SYSTEM_THREAD:rc==JP_HISTORY_REENTRANT?JP_SYSTEM_REENTRANT:JP_SYSTEM_CLEANUP;
}
static int system_history_close(void)
{
    if(!system_owner.history_token)return jp_history_shutdown();
    return system_owner.terminal?jp_history_exit_finish(system_owner.history_token):jp_history_exit_close(system_owner.history_token);
}
static int system_history_release(void)
{
    if(!system_owner.history_token||system_owner.terminal)return 0;
    int rc=jp_history_exit_cancel(system_owner.history_token);if(rc)return rc;
    system_owner.history_token=0;return 0;
}
static int system_history_adopt(void)
{
    struct jp_history_info info;int rc=jp_history_snapshot(&info);
    if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_HISTORY,rc);
    if(!info.resources)return JP_SYSTEM_STATE;
    rc=jp_history_exit_activity();if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_HISTORY,rc);
    /* Retain cleanup responsibility even if a lower claim cannot be acquired.
     * Final binding requires a real token; early cleanup can retire a partial
     * constructor through its actual existing ordinary owner. */
    system_owner.info.resources|=JP_EXIT_HISTORY;
    rc=jp_history_exit_claim(&system_owner.history_token);
    int attached=jp_exit_attach_bootstrap_history(bootstrap_cleanup,&system_owner,system_owner.history_token);
    if(attached)return result(JP_SYSTEM_EXIT,JP_EXIT_HISTORY,attached);
    return rc?result(JP_SYSTEM_CLEANUP,JP_EXIT_HISTORY,rc):0;
}
int jp_system_adopt_history(void)
{
    int rc=access_owner();if(rc)return rc;if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    if(!system_owner.registered||system_owner.terminal||system_owner.info.phase!=JP_SYSTEM_READY||
       !system_owner.main_guard||system_owner.history_token||(system_owner.info.resources&JP_EXIT_HISTORY))return JP_SYSTEM_STATE;
    return system_history_adopt();
}
int jp_system_start_history(const struct jp_history_options *options)
{
    int rc=access_owner();if(rc)return rc;if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    if(!system_owner.registered||system_owner.terminal||system_owner.info.phase!=JP_SYSTEM_READY||
       system_owner.history_token||(system_owner.info.resources&JP_EXIT_HISTORY))return JP_SYSTEM_STATE;
    int active=jp_history_exit_activity();if(active)return active==JP_HISTORY_REENTRANT?JP_SYSTEM_REENTRANT:JP_SYSTEM_STATE;
    struct jp_history_info old;struct jp_history_exit_info claim;
    if(jp_history_snapshot(&old)||old.resources||jp_history_exit_snapshot(&claim)||claim.token||claim.terminal)return JP_SYSTEM_STATE;
    int own_guard=!system_owner.main_guard;
    if(own_guard){rc=jp_system_main_initialization_guard(1);if(rc)return rc;}
    system_owner.busy=1;
    int started=jp_history_configure(options);if(!started)started=jp_history_initialize();
    struct jp_history_info current={0};rc=jp_history_snapshot(&current);
    if(!rc&&current.resources)rc=system_history_adopt();
    system_owner.busy=0;
    /* An attachment failure retains the constructor guard until explicit
     * System cleanup; no unregistered resource can escape via an early exit. */
    if(own_guard&&(!current.resources||!rc)){
        int released=jp_system_main_initialization_guard(0);if(released)return released;
    }
    if(rc)return rc;
    return started?result(JP_SYSTEM_CLEANUP,JP_EXIT_HISTORY,started):result(0,0,0);
}
