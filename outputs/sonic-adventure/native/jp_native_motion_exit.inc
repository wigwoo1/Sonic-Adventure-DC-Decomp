/* Same real data owner; deliberately no System/Exit/Frames dependency. */
static uint64_t next_motion_exit_token=1;
int jp_motion_exit_activity(void)
{
    DWORD thread=motion_owner_thread();
    return thread&&thread!=GetCurrentThreadId()?JP_MOTION_THREAD:store.busy?JP_MOTION_REENTRANT:0;
}
int jp_motion_exit_snapshot(struct jp_motion_exit_info *out)
{
    DWORD thread=motion_owner_thread();if(thread&&thread!=GetCurrentThreadId())return JP_MOTION_THREAD;
    if(!output_valid(out,sizeof *out))return JP_MOTION_ARGUMENT;
    *out=(struct jp_motion_exit_info){next_motion_exit_token,store.exit_token,store.exit_terminal,store.exit_closing,store.exit_finished,0};return 0;
}
int jp_motion_exit_can_close(uint64_t token)
{
    int rc=jp_motion_exit_activity();if(rc)return rc;
    return !token||token!=store.exit_token||store.exit_call?JP_MOTION_STATE:0;
}
int jp_motion_exit_can_retire(uint64_t token)
{
    int rc=jp_motion_exit_can_close(token);if(rc)return rc;
    return store.info.live_leases?JP_MOTION_BUSY:0;
}
int jp_motion_exit_claim(uint64_t *out)
{
    int rc=jp_motion_exit_activity();if(rc)return rc;
    if(!output_valid(out,sizeof *out))return JP_MOTION_ARGUMENT;
    if(store.exit_token||store.exit_terminal||!store.active||!next_motion_exit_token)return JP_MOTION_STATE;
    store.exit_token=next_motion_exit_token++;store.exit_closing=store.exit_finished=0;*out=store.exit_token;return 0;
}
int jp_motion_exit_cancel(uint64_t token)
{
    int rc=jp_motion_exit_can_close(token);if(rc)return rc;
    if(store.exit_terminal||(store.exit_closing&&!store.exit_finished))return JP_MOTION_STATE;
    store.exit_token=0;store.exit_closing=store.exit_finished=0;return 0;
}
static int motion_exit_close(uint64_t token,int terminal)
{
    int rc=jp_motion_exit_can_retire(token);if(rc)return rc;
    if(!terminal&&store.exit_terminal)return JP_MOTION_STATE;
    if(terminal)store.exit_terminal=1;
    if(store.exit_finished)return 0;
    store.exit_closing=1;store.exit_call=1;rc=jp_motions_shutdown();store.exit_call=0;
    if(!rc)store.exit_finished=1;return rc;
}
int jp_motion_exit_close(uint64_t token){return motion_exit_close(token,0);}
int jp_motion_exit_finish(uint64_t token){return motion_exit_close(token,1);}
