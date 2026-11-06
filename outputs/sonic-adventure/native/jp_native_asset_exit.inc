static uint64_t next_asset_exit_token=1;
static DWORD asset_exit_thread(void)
{return assets.owner?assets.owner:(assets.exit_token||assets.exit_terminal)?assets.claim_thread:0;}
static void asset_clear_retaining_claim(void)
{
    uint64_t token=assets.exit_token;DWORD thread=assets.claim_thread;
    uint32_t terminal=assets.exit_terminal,closing=assets.exit_closing,finished=assets.exit_finished,call=assets.exit_call;
    memset(&assets,0,sizeof assets);assets.exit_token=token;assets.claim_thread=thread;
    assets.exit_terminal=terminal;assets.exit_closing=closing;assets.exit_finished=finished;assets.exit_call=call;
}
int jp_assets_exit_activity(void)
{DWORD thread=asset_exit_thread();return thread&&thread!=GetCurrentThreadId()?JP_ASSET_EXIT_THREAD:assets.busy?JP_ASSET_EXIT_REENTRANT:0;}
int jp_assets_exit_snapshot(struct jp_asset_exit_info *out)
{
    int rc=jp_assets_exit_activity();if(rc)return rc;if(!out)return JP_ASSET_EXIT_ARGUMENT;
    *out=(struct jp_asset_exit_info){next_asset_exit_token,assets.exit_token,assets.info.decoded_bytes,assets.info.owned_bytes,
        assets.exit_terminal,assets.exit_closing,assets.exit_finished,(uint32_t)assets.initialized,asset_exit_thread(),assets.info.status,
        assets.initialized?jp_pvm_loaded_count(&assets.pending):0,assets.initialized?resident_count():0};return 0;
}
int jp_assets_exit_can_close(uint64_t token)
{
    int rc=jp_assets_exit_activity();if(rc)return rc;
    return !token||token!=assets.exit_token||assets.exit_call?JP_ASSET_EXIT_STATE:0;
}
static int asset_retire_activity(uint64_t character_token,int after_character)
{
    if(assets.data||jp_asset_loader_activity())return JP_ASSET_EXIT_BUSY;
    struct jp_character_exit_info child;int rc=jp_character_exit_snapshot(&child);if(rc)return rc==JP_CHARACTER_EXIT_THREAD?JP_ASSET_EXIT_THREAD:JP_ASSET_EXIT_REENTRANT;
    if(child.active){
        if(!after_character||child.token!=character_token)return JP_ASSET_EXIT_BUSY;
        if(character_token){rc=jp_character_exit_can_retire(character_token);if(rc)return rc==JP_CHARACTER_EXIT_THREAD?JP_ASSET_EXIT_THREAD:rc==JP_CHARACTER_EXIT_REENTRANT?JP_ASSET_EXIT_REENTRANT:JP_ASSET_EXIT_BUSY;}
        else if(child.users)return JP_ASSET_EXIT_BUSY;
    }
    return 0;
}
int jp_assets_exit_can_retire_after_character(uint64_t token,uint64_t character_token)
{int rc=jp_assets_exit_can_close(token);return rc?rc:assets.exit_finished?0:asset_retire_activity(character_token,1);}
int jp_assets_exit_can_retire(uint64_t token)
{int rc=jp_assets_exit_can_close(token);return rc?rc:assets.exit_finished?0:asset_retire_activity(0,0);}
int jp_assets_exit_claim(uint64_t *out)
{
    int rc=jp_assets_exit_activity();if(rc)return rc;if(!out)return JP_ASSET_EXIT_ARGUMENT;
    if(!assets.initialized||assets.exit_token||assets.exit_terminal||!next_asset_exit_token)return JP_ASSET_EXIT_STATE;
    assets.exit_token=next_asset_exit_token++;assets.claim_thread=assets.owner;assets.exit_closing=assets.exit_finished=0;*out=assets.exit_token;return 0;
}
int jp_assets_exit_cancel(uint64_t token)
{
    int rc=jp_assets_exit_can_close(token);if(rc)return rc;
    if(assets.exit_terminal||(assets.exit_closing&&!assets.exit_finished))return JP_ASSET_EXIT_STATE;
    assets.exit_token=0;assets.claim_thread=0;assets.exit_closing=assets.exit_finished=0;return 0;
}
static int asset_close_actual(void)
{
    jp_assets_clear_error();jp_memory_clear_error();jp_textures_clear_error();
    int rc=jp_assets_recover_pending();if(rc)return rc;
    return jp_assets_shutdown();
}
static int asset_close_claim(uint64_t token,int terminal)
{
    int rc=jp_assets_exit_can_retire(token);if(rc)return rc;
    if(!terminal&&assets.exit_terminal)return JP_ASSET_EXIT_STATE;
    if(terminal)assets.exit_terminal=1;if(assets.exit_finished)return 0;
    assets.exit_closing=1;assets.exit_call=1;rc=asset_close_actual();assets.exit_call=0;
    if(!rc)assets.exit_finished=1;return rc;
}
int jp_assets_exit_close(uint64_t token){return asset_close_claim(token,0);}
int jp_assets_exit_finish(uint64_t token){return asset_close_claim(token,1);}
int jp_assets_exit_close_unclaimed(uint32_t terminal)
{
    int rc=jp_assets_exit_activity();if(rc)return rc;
    if(terminal>1||assets.exit_token||(assets.exit_terminal&&!terminal))return JP_ASSET_EXIT_STATE;
    if(assets.exit_finished){if(terminal)assets.exit_terminal=1;return 0;}
    rc=asset_retire_activity(0,0);if(rc)return rc;if(!assets.initialized)return 0;
    assets.claim_thread=assets.owner;assets.exit_terminal=terminal;assets.exit_closing=assets.exit_call=1;
    rc=asset_close_actual();assets.exit_call=0;if(!rc){assets.exit_finished=1;if(!terminal){assets.claim_thread=0;assets.exit_closing=assets.exit_finished=0;}}return rc;
}
