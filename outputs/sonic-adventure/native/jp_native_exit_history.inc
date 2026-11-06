/* Extend the existing bootstrap registration rather than replacing it. */
int jp_exit_attach_bootstrap_history(jp_history_boot_cleanup cleanup,void *context,uint64_t token)
{
    int rc=owner();if(rc)return rc;if(state.busy)return JP_EXIT_REENTRANT;
    if(state.bound||bootstrap.started||bootstrap.cleanup!=cleanup||bootstrap.context!=context||
       (bootstrap.resources&JP_EXIT_HISTORY))return JP_EXIT_ALREADY_BOUND;
    struct jp_history_info actual;struct jp_history_exit_info claim;
    if(jp_history_snapshot(&actual)||!actual.resources||jp_history_exit_snapshot(&claim)||
       claim.token!=token||claim.terminal||claim.closing)return JP_EXIT_ARGUMENT;
    rc=token?jp_history_exit_can_close(token):jp_history_exit_activity();
    if(rc)return rc==JP_HISTORY_REENTRANT?JP_EXIT_REENTRANT:JP_EXIT_ARGUMENT;
    bootstrap.history_token=token;bootstrap.resources|=JP_EXIT_HISTORY;return 0;
}
