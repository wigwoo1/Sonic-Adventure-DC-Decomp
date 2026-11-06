/* Same System instance and the unchanged bootstrap cleanup callback. */
static int system_read_queue_activity(void)
{
    if(!(system_owner.info.resources&JP_EXIT_READ_QUEUE))return 0;
    int rc=system_owner.read_queue_token?jp_read_queue_exit_can_close(system_owner.read_queue_token):jp_read_queue_exit_activity();
    return !rc?0:rc==JP_READ_QUEUE_THREAD?JP_SYSTEM_THREAD:rc==JP_READ_QUEUE_REENTRANT?JP_SYSTEM_REENTRANT:JP_SYSTEM_CLEANUP;
}
static int system_read_queue_close(void)
{
    if(!system_owner.read_queue_token)return jp_read_queue_shutdown();
    return system_owner.terminal?jp_read_queue_exit_finish(system_owner.read_queue_token):jp_read_queue_exit_close(system_owner.read_queue_token);
}
static int system_read_queue_release(void)
{
    if(!system_owner.read_queue_token||system_owner.terminal)return 0;
    int rc=jp_read_queue_exit_cancel(system_owner.read_queue_token);if(rc)return rc;system_owner.read_queue_token=0;return 0;
}
static int system_read_queue_adopt(void)
{
    struct jp_read_queue_info info;struct jp_read_queue_exit_info claim;
    int rc=jp_read_queue_snapshot(&info);if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_READ_QUEUE,rc);
    if(!info.initialized)return JP_SYSTEM_STATE;
    rc=jp_read_queue_exit_snapshot(&claim);if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_READ_QUEUE,rc);
    if(claim.token||claim.terminal)return JP_SYSTEM_STATE;
    rc=jp_read_queue_exit_activity();if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_READ_QUEUE,rc);
    system_owner.info.resources|=JP_EXIT_READ_QUEUE;
    rc=jp_read_queue_exit_claim(&system_owner.read_queue_token);
    int attached=jp_exit_attach_bootstrap_read_queue(bootstrap_cleanup,&system_owner,system_owner.read_queue_token);
    if(attached)return result(JP_SYSTEM_EXIT,JP_EXIT_READ_QUEUE,attached);
    return rc?result(JP_SYSTEM_CLEANUP,JP_EXIT_READ_QUEUE,rc):0;
}
int jp_system_adopt_read_queue(void)
{
    int rc=access_owner();if(rc)return rc;if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    if(!system_owner.registered||system_owner.terminal||system_owner.info.phase!=JP_SYSTEM_READY||!system_owner.main_guard||
       system_owner.read_queue_token||(system_owner.info.resources&JP_EXIT_READ_QUEUE))return JP_SYSTEM_STATE;
    return system_read_queue_adopt();
}
int jp_system_start_read_queue(void)
{
    int rc=access_owner();if(rc)return rc;if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    if(!system_owner.registered||system_owner.terminal||system_owner.info.phase!=JP_SYSTEM_READY||
       system_owner.read_queue_token||(system_owner.info.resources&JP_EXIT_READ_QUEUE))return JP_SYSTEM_STATE;
    int active=jp_read_queue_exit_activity();if(active)return active==JP_READ_QUEUE_REENTRANT?JP_SYSTEM_REENTRANT:JP_SYSTEM_STATE;
    struct jp_read_queue_info old;struct jp_read_queue_exit_info claim;
    if(jp_read_queue_snapshot(&old)||old.initialized||jp_read_queue_exit_snapshot(&claim)||claim.token||claim.terminal)return JP_SYSTEM_STATE;
    int own_guard=!system_owner.main_guard;
    if(own_guard){rc=jp_system_main_initialization_guard(1);if(rc)return rc;}
    system_owner.busy=1;int started=jp_read_queue_initialize();struct jp_read_queue_info current={0};rc=jp_read_queue_snapshot(&current);
    if(!rc&&current.initialized)rc=system_read_queue_adopt();system_owner.busy=0;
    /* Attachment failure retains the guard and actual cleanup responsibility. */
    if(own_guard&&(!current.initialized||!rc)){int released=jp_system_main_initialization_guard(0);if(released)return released;}
    if(rc)return rc;if(started==JP_READ_QUEUE_REENTRANT)return JP_SYSTEM_REENTRANT;
    return started?result(JP_SYSTEM_CLEANUP,JP_EXIT_READ_QUEUE,started):result(0,0,0);
}
