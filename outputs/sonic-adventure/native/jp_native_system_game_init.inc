/* Same System owner; preserve normal sound-clock activity during frame waits. */
static uint64_t next_game_init_token=1;
static int game_init_access(uint64_t token)
{
    int rc=access_owner();if(rc)return rc;
    if(system_owner.busy||system_owner.game_init_call)return JP_SYSTEM_REENTRANT;
    return !token||token!=system_owner.game_init_token||!system_owner.main_guard||
        !system_owner.registered||system_owner.terminal||system_owner.info.phase!=JP_SYSTEM_READY?JP_SYSTEM_STATE:0;
}
int jp_system_game_init_claim(uint64_t *out)
{
    int rc=access_owner();if(rc)return rc;if(system_owner.busy||system_owner.game_init_call)return JP_SYSTEM_REENTRANT;
    if(!out)return JP_SYSTEM_ARGUMENT;
    if(system_owner.game_init_token||system_owner.main_guard||!next_game_init_token||
       system_owner.info.resources!=(CORE|JP_EXIT_SOUND|JP_EXIT_CARD|JP_EXIT_PRINT)||
       !system_owner.registered||system_owner.terminal||system_owner.info.phase!=JP_SYSTEM_READY)return JP_SYSTEM_STATE;
    rc=jp_system_main_initialization_guard(1);if(rc)return rc;
    system_owner.game_init_token=next_game_init_token++;*out=system_owner.game_init_token;return 0;
}
int jp_system_game_init_activity(void)
{
    int rc=access_owner();if(rc)return rc;
    return system_owner.game_init_token?JP_SYSTEM_REENTRANT:0;
}
int jp_system_game_init_release(uint64_t token)
{
    int rc=game_init_access(token);if(rc)return rc;
    system_owner.game_init_call=1;rc=jp_system_main_initialization_guard(0);system_owner.game_init_call=0;
    if(!rc)system_owner.game_init_token=0;return rc;
}
int jp_system_game_init_queue(uint64_t token)
{
    int rc=game_init_access(token);if(rc)return rc;
    system_owner.game_init_call=1;rc=jp_system_start_read_queue();system_owner.game_init_call=0;return rc;
}
int jp_system_game_init_history(uint64_t token,const struct jp_history_options *options)
{
    int rc=game_init_access(token);if(rc)return rc;
    system_owner.game_init_call=1;rc=jp_system_start_history(options);system_owner.game_init_call=0;return rc;
}
