static int system_motion_activity(int retiring)
{
    if(!(system_owner.info.resources&JP_EXIT_MOTIONS)&&!system_owner.motion_token)return 0;
    int rc=system_owner.motion_token?(retiring?jp_motion_exit_can_retire(system_owner.motion_token):jp_motion_exit_can_close(system_owner.motion_token)):jp_motion_exit_activity();
    if(!rc&&retiring&&!system_owner.motion_token){struct jp_motion_info info;rc=jp_motions_snapshot(&info);if(!rc&&info.live_leases)rc=JP_MOTION_BUSY;}
    if(!rc)return 0;
    return rc==JP_MOTION_THREAD?JP_SYSTEM_THREAD:rc==JP_MOTION_REENTRANT?JP_SYSTEM_REENTRANT:result(JP_SYSTEM_CLEANUP,JP_EXIT_MOTIONS,rc);
}
int jp_system_motion_cleanup_activity(void)
{
    int rc=access_owner();if(rc)return rc;if(system_owner.busy)return JP_SYSTEM_REENTRANT;
    return system_motion_activity(1);
}
int jp_system_prepare_boot_shutdown(uint32_t *retired)
{
    int rc=access_owner();if(rc)return rc;
    if(system_owner.busy||(system_owner.game_init_token&&!system_owner.game_init_call))return JP_SYSTEM_REENTRANT;
    if(!retired)return JP_SYSTEM_ARGUMENT;
    if(!(system_owner.info.resources&JP_EXIT_MOTIONS)&&!system_owner.motion_token){*retired=0;return 0;}
    uint32_t owned=(system_owner.card_token?JP_EXIT_CARD:0)|(system_owner.print_token?JP_EXIT_PRINT:0);
    rc=jp_system_shutdown();if(rc)return rc;
    *retired=owned|JP_EXIT_MOTIONS;return 0;
}
static int system_motion_close(void)
{
    if(!system_owner.motion_token)return jp_motions_shutdown();
    return system_owner.terminal?jp_motion_exit_finish(system_owner.motion_token):jp_motion_exit_close(system_owner.motion_token);
}
static int system_motion_release(void)
{
    if(!system_owner.motion_token||system_owner.terminal)return 0;
    int rc=jp_motion_exit_cancel(system_owner.motion_token);if(!rc)system_owner.motion_token=0;return rc;
}
static int system_motion_adopt(void)
{
    struct jp_motion_info info;struct jp_motion_exit_info claim;
    int rc=jp_motions_snapshot(&info);if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_MOTIONS,rc);
    if(info.phase!=1)return JP_SYSTEM_STATE;
    rc=jp_motion_exit_snapshot(&claim);if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_MOTIONS,rc);
    if(claim.token||claim.terminal)return JP_SYSTEM_STATE;
    rc=jp_motion_exit_activity();if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_MOTIONS,rc);
    system_owner.info.resources|=JP_EXIT_MOTIONS;rc=jp_motion_exit_claim(&system_owner.motion_token);
    int attached=jp_exit_attach_bootstrap_motions(bootstrap_cleanup,&system_owner,system_owner.motion_token);
    if(attached)return result(JP_SYSTEM_EXIT,JP_EXIT_MOTIONS,attached);
    return rc?result(JP_SYSTEM_CLEANUP,JP_EXIT_MOTIONS,rc):0;
}
int jp_system_adopt_motions(void)
{
    int rc=access_owner();if(rc)return rc;
    if(system_owner.busy||(system_owner.game_init_token&&!system_owner.game_init_call))return JP_SYSTEM_REENTRANT;
    if(!system_owner.registered||system_owner.terminal||system_owner.info.phase!=JP_SYSTEM_READY||!system_owner.main_guard||
       system_owner.motion_token||(system_owner.info.resources&JP_EXIT_MOTIONS))return JP_SYSTEM_STATE;
    return system_motion_adopt();
}
int jp_system_start_motions(size_t budget,uint32_t capacity)
{
    int rc=access_owner();if(rc)return rc;
    if(system_owner.busy||(system_owner.game_init_token&&!system_owner.game_init_call))return JP_SYSTEM_REENTRANT;
    if(!system_owner.registered||system_owner.terminal||system_owner.info.phase!=JP_SYSTEM_READY||system_owner.motion_token||
       (system_owner.info.resources&JP_EXIT_MOTIONS))return JP_SYSTEM_STATE;
    struct jp_motion_info old;struct jp_motion_exit_info claim;
    if(jp_motion_exit_activity()||jp_motions_snapshot(&old)||old.phase==1||jp_motion_exit_snapshot(&claim)||claim.token||claim.terminal)return JP_SYSTEM_STATE;
    int own_guard=!system_owner.main_guard;if(own_guard){rc=jp_system_main_initialization_guard(1);if(rc)return rc;}
    system_owner.busy=1;int started=jp_motions_initialize(budget,capacity);struct jp_motion_info current={0};rc=jp_motions_snapshot(&current);
    if(!rc&&current.phase==1)rc=system_motion_adopt();system_owner.busy=0;
    if(own_guard&&(current.phase!=1||!rc)){int released=jp_system_main_initialization_guard(0);if(released)return released;}
    if(rc)return rc;
    return started?result(JP_SYSTEM_CLEANUP,JP_EXIT_MOTIONS,started):result(0,0,0);
}
