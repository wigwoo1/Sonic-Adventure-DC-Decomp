/* Attach real queue authority to exactly the existing callback/context. */
int jp_exit_attach_bootstrap_read_queue(jp_read_queue_boot_cleanup cleanup,void *context,uint64_t token)
{
    int rc=owner();if(rc)return rc;if(state.busy)return JP_EXIT_REENTRANT;
    if(state.bound||bootstrap.started||bootstrap.cleanup!=cleanup||bootstrap.context!=context||
       (bootstrap.resources&JP_EXIT_READ_QUEUE))return JP_EXIT_ALREADY_BOUND;
    struct jp_read_queue_info actual;struct jp_read_queue_exit_info claim;
    if(jp_read_queue_snapshot(&actual)||!actual.initialized||jp_read_queue_exit_snapshot(&claim)||
       claim.token!=token||claim.terminal||claim.closing)return JP_EXIT_ARGUMENT;
    rc=token?jp_read_queue_exit_can_close(token):jp_read_queue_exit_activity();
    if(rc)return rc==JP_READ_QUEUE_REENTRANT?JP_EXIT_REENTRANT:JP_EXIT_ARGUMENT;
    bootstrap.read_queue_token=token;bootstrap.resources|=JP_EXIT_READ_QUEUE;return 0;
}
