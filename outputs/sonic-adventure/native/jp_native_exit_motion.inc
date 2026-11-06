int jp_exit_attach_bootstrap_motions(jp_motion_boot_cleanup cleanup,void *context,uint64_t token)
{
    int rc=owner();if(rc)return rc;if(state.busy)return JP_EXIT_REENTRANT;
    if(state.bound||bootstrap.started||bootstrap.cleanup!=cleanup||bootstrap.context!=context||(bootstrap.resources&JP_EXIT_MOTIONS))return JP_EXIT_ALREADY_BOUND;
    struct jp_motion_info info;struct jp_motion_exit_info claim;
    if(jp_motions_snapshot(&info)||info.phase!=1||jp_motion_exit_snapshot(&claim)||claim.token!=token||claim.terminal||claim.closing)return JP_EXIT_ARGUMENT;
    rc=token?jp_motion_exit_can_close(token):jp_motion_exit_activity();
    if(rc)return rc==JP_MOTION_REENTRANT?JP_EXIT_REENTRANT:JP_EXIT_ARGUMENT;
    bootstrap.motion_token=token;bootstrap.resources|=JP_EXIT_MOTIONS;return 0;
}
static int motion_retire_preflight(void)
{
    uint32_t mask=state.bound?state.info.remaining:bootstrap.resources;
    uint64_t token=state.bound?state.motion_token:bootstrap.motion_token;
    if(!(mask&JP_EXIT_MOTIONS))return 0;
    int rc=token?jp_motion_exit_can_retire(token):jp_motion_exit_activity();
    if(!rc&&!token){struct jp_motion_info info;rc=jp_motions_snapshot(&info);if(!rc&&info.live_leases)rc=JP_MOTION_BUSY;}
    if(!rc)return 0;
    if(rc==JP_MOTION_THREAD)return JP_EXIT_THREAD;if(rc==JP_MOTION_REENTRANT)return JP_EXIT_REENTRANT;
    return failed(JP_EXIT_MOTIONS,rc);
}
