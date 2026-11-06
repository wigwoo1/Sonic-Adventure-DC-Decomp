static uint64_t next_character_exit_token=1;
static DWORD character_exit_thread(void)
{return character_owner.thread?character_owner.thread:(character_owner.exit_token||character_owner.exit_terminal)?character_owner.info.creator_thread:0;}
static unsigned character_exit_users(void)
{unsigned n=0;for(unsigned i=0;i<8;i++)n+=(unsigned)(uint8_t)character_users[i];return n;}
int jp_character_exit_activity(void)
{DWORD thread=character_exit_thread();return thread&&thread!=GetCurrentThreadId()?JP_CHARACTER_EXIT_THREAD:character_owner.busy?JP_CHARACTER_EXIT_REENTRANT:0;}
int jp_character_exit_snapshot(struct jp_character_exit_info *out)
{
    int rc=jp_character_exit_activity();if(rc)return rc;if(!out)return JP_CHARACTER_EXIT_ARGUMENT;
    *out=(struct jp_character_exit_info){next_character_exit_token,character_owner.exit_token,character_owner.exit_terminal,character_owner.exit_closing,character_owner.exit_finished,
        (uint32_t)character_owner.active,character_exit_thread(),character_owner.info.status,character_exit_users(),character_owner.info.phase==2};return 0;
}
int jp_character_exit_can_close(uint64_t token)
{
    int rc=jp_character_exit_activity();if(rc)return rc;
    return !token||token!=character_owner.exit_token||character_owner.exit_call?JP_CHARACTER_EXIT_STATE:0;
}
int jp_character_exit_can_retire(uint64_t token)
{
    int rc=jp_character_exit_can_close(token);if(rc)return rc;
    if(character_owner.exit_finished)return 0;
    return character_exit_users()||jp_asset_loader_activity()?JP_CHARACTER_EXIT_BUSY:0;
}
int jp_character_exit_claim(uint64_t *out)
{
    int rc=jp_character_exit_activity();if(rc)return rc;if(!out)return JP_CHARACTER_EXIT_ARGUMENT;
    if(!character_owner.active||character_owner.exit_token||character_owner.exit_terminal||!next_character_exit_token)return JP_CHARACTER_EXIT_STATE;
    character_owner.exit_token=next_character_exit_token++;character_owner.exit_closing=character_owner.exit_finished=0;*out=character_owner.exit_token;return 0;
}
int jp_character_exit_cancel(uint64_t token)
{
    int rc=jp_character_exit_can_close(token);if(rc)return rc;
    if(character_owner.exit_terminal||(character_owner.exit_closing&&!character_owner.exit_finished))return JP_CHARACTER_EXIT_STATE;
    character_owner.exit_token=0;character_owner.exit_closing=character_owner.exit_finished=0;return 0;
}
static int character_close_actual(void)
{
    if(character_owner.info.phase==2){
        jp_assets_clear_error();jp_memory_clear_error();jp_textures_clear_error();
        int rc=jp_character_textures_recover();if(rc)return rc;
    }
    return jp_character_textures_shutdown();
}
static int character_close_claim(uint64_t token,int terminal)
{
    int rc=jp_character_exit_can_retire(token);if(rc)return rc;
    if(!terminal&&character_owner.exit_terminal)return JP_CHARACTER_EXIT_STATE;
    if(terminal)character_owner.exit_terminal=1;if(character_owner.exit_finished)return 0;
    character_owner.exit_closing=character_owner.exit_call=1;rc=character_close_actual();character_owner.exit_call=0;
    if(!rc)character_owner.exit_finished=1;return rc;
}
int jp_character_exit_close(uint64_t token){return character_close_claim(token,0);}
int jp_character_exit_finish(uint64_t token){return character_close_claim(token,1);}
int jp_character_exit_close_unclaimed(uint32_t terminal)
{
    int rc=jp_character_exit_activity();if(rc)return rc;
    if(terminal>1||character_owner.exit_token||(character_owner.exit_terminal&&!terminal))return JP_CHARACTER_EXIT_STATE;
    if(character_owner.exit_finished){if(terminal)character_owner.exit_terminal=1;return 0;}
    if(character_exit_users()||jp_asset_loader_activity())return JP_CHARACTER_EXIT_BUSY;
    if(!character_owner.active)return 0;
    character_owner.exit_terminal=terminal;character_owner.exit_closing=character_owner.exit_call=1;
    rc=character_close_actual();character_owner.exit_call=0;if(!rc){character_owner.exit_finished=1;if(!terminal)character_owner.exit_closing=character_owner.exit_finished=0;}return rc;
}
