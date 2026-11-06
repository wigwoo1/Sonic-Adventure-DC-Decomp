/* Appended beside the actual History owner. No System/Exit import. */
static uint64_t next_history_exit_token=1;
static int history_exit_card_activity(void)
{
    struct jp_boot_life_info card;int rc=jp_boot_lifetime_snapshot(JP_BOOT_CARD,&card);
    if(rc)return rc==JP_BOOT_LIFE_THREAD?JP_HISTORY_THREAD:JP_HISTORY_STATE;
    if(!card.token)return 0;
    rc=jp_boot_lifetime_can_close(JP_BOOT_CARD,card.token);
    return !rc?0:rc==JP_BOOT_LIFE_REENTRANT?JP_HISTORY_REENTRANT:rc==JP_BOOT_LIFE_THREAD?JP_HISTORY_THREAD:JP_HISTORY_STATE;
}
int jp_history_exit_activity(void)
{int rc=access();return rc?rc:history_exit_card_activity();}
int jp_history_exit_snapshot(struct jp_history_exit_info *out)
{
    if(history.thread&&history.thread!=GetCurrentThreadId())return JP_HISTORY_THREAD;
    if(!out)return JP_HISTORY_ARGUMENT;
    *out=(struct jp_history_exit_info){next_history_exit_token,history.exit_token,
        history.exit_terminal,history.exit_closing,history.exit_finished,0};return 0;
}
int jp_history_exit_can_close(uint64_t token)
{
    int rc=jp_history_exit_activity();if(rc)return rc;
    return !token||history.exit_token!=token||history.exit_call?JP_HISTORY_STATE:0;
}
int jp_history_exit_claim(uint64_t *out)
{
    int rc=jp_history_exit_activity();if(rc)return rc;if(!out)return JP_HISTORY_ARGUMENT;
    if(history.exit_token||history.exit_terminal||!history.info.resources||!next_history_exit_token||
       (history.info.phase!=JP_HISTORY_READY&&history.info.phase!=JP_HISTORY_FAILED&&history.info.phase!=JP_HISTORY_DRAINING))return JP_HISTORY_STATE;
    history.exit_token=next_history_exit_token++;history.exit_closing=history.exit_finished=0;
    *out=history.exit_token;return 0;
}
int jp_history_exit_cancel(uint64_t token)
{
    int rc=jp_history_exit_can_close(token);if(rc)return rc;
    if(history.exit_terminal||(history.exit_closing&&!history.exit_finished))return JP_HISTORY_STATE;
    history.exit_token=0;history.exit_closing=history.exit_finished=0;return 0;
}
static int history_exit_close(uint64_t token,int terminal)
{
    int rc=jp_history_exit_can_close(token);if(rc)return rc;
    if(!terminal&&history.exit_terminal)return JP_HISTORY_STATE;
    if(terminal)history.exit_terminal=1;
    if(history.exit_finished)return 0;
    history.exit_closing=1;history.exit_call=1;
    rc=jp_history_shutdown();history.exit_call=0;
    if(!rc)history.exit_finished=1;
    return rc;
}
int jp_history_exit_close(uint64_t token){return history_exit_close(token,0);}
int jp_history_exit_finish(uint64_t token){return history_exit_close(token,1);}
