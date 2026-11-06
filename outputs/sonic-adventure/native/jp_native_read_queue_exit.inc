/* Appended beside the SAME queue owner; no System/Exit import. */
static uint64_t next_read_queue_exit_token=1;
int jp_read_queue_exit_activity(void)
{
    int rc=access_owner();if(rc)return rc;
    struct jp_boot_life_info card;rc=jp_boot_lifetime_snapshot(JP_BOOT_CARD,&card);
    if(!rc&&card.token)rc=jp_boot_lifetime_can_close(JP_BOOT_CARD,card.token);
    if(rc)return rc==JP_BOOT_LIFE_THREAD?JP_READ_QUEUE_THREAD:rc==JP_BOOT_LIFE_REENTRANT?JP_READ_QUEUE_REENTRANT:JP_READ_QUEUE_STATE;
    if(queue.info.initialized){
        rc=jp_gdfs_read_queue_enter(queue.info.lease);
        if(!rc)rc=jp_gdfs_read_queue_leave(queue.info.lease);
        if(rc)return rc==JP_GDFS_THREAD?JP_READ_QUEUE_THREAD:rc==JP_GDFS_REENTRANT?JP_READ_QUEUE_REENTRANT:JP_READ_QUEUE_GDFS;
    }
    return 0;
}
int jp_read_queue_exit_snapshot(struct jp_read_queue_exit_info *out)
{
    if(queue.thread&&queue.thread!=GetCurrentThreadId())return JP_READ_QUEUE_THREAD;
    if(!snapshot_output_valid(out,sizeof *out))return JP_READ_QUEUE_ARGUMENT;
    *out=(struct jp_read_queue_exit_info){next_read_queue_exit_token,queue.exit_token,
        queue.exit_terminal,queue.exit_closing,queue.exit_finished,0};return 0;
}
int jp_read_queue_exit_can_close(uint64_t token)
{
    int rc=jp_read_queue_exit_activity();if(rc)return rc;
    return !token||token!=queue.exit_token||queue.exit_call?JP_READ_QUEUE_STATE:0;
}
int jp_read_queue_exit_claim(uint64_t *out)
{
    int rc=jp_read_queue_exit_activity();if(rc)return rc;
    if(!snapshot_output_valid(out,sizeof *out))return JP_READ_QUEUE_ARGUMENT;
    if(queue.exit_token||queue.exit_terminal||!queue.info.initialized||!queue.info.lease||!next_read_queue_exit_token)return JP_READ_QUEUE_STATE;
    queue.exit_token=next_read_queue_exit_token++;queue.exit_closing=queue.exit_finished=0;*out=queue.exit_token;return 0;
}
int jp_read_queue_exit_cancel(uint64_t token)
{
    int rc=jp_read_queue_exit_can_close(token);if(rc)return rc;
    if(queue.exit_terminal||(queue.exit_closing&&!queue.exit_finished))return JP_READ_QUEUE_STATE;
    queue.exit_token=0;queue.exit_closing=queue.exit_finished=0;return 0;
}
static int read_queue_exit_close(uint64_t token,int terminal)
{
    int rc=jp_read_queue_exit_can_close(token);if(rc)return rc;
    if(!terminal&&queue.exit_terminal)return JP_READ_QUEUE_STATE;
    if(terminal)queue.exit_terminal=1;
    if(queue.exit_finished)return 0;
    queue.exit_closing=1;queue.exit_call=1;
    rc=jp_read_queue_shutdown();queue.exit_call=0;
    if(!rc)queue.exit_finished=1;return rc;
}
int jp_read_queue_exit_close(uint64_t token){return read_queue_exit_close(token,0);}
int jp_read_queue_exit_finish(uint64_t token){return read_queue_exit_close(token,1);}
