int jp_exit_attach_bootstrap_assets(jp_assets_boot_cleanup cleanup,void *context,uint64_t token)
{
    int rc=owner();if(rc)return rc;if(state.busy)return JP_EXIT_REENTRANT;
    if(state.bound||bootstrap.started||bootstrap.cleanup!=cleanup||bootstrap.context!=context||(bootstrap.resources&JP_EXIT_ASSETS))return JP_EXIT_ALREADY_BOUND;
    struct jp_asset_exit_info actual;rc=jp_assets_exit_snapshot(&actual);
    if(rc||!actual.initialized||actual.token!=token||actual.terminal||actual.closing)return JP_EXIT_ARGUMENT;
    rc=token?jp_assets_exit_can_close(token):jp_assets_exit_activity();if(rc)return rc==JP_ASSET_EXIT_REENTRANT?JP_EXIT_REENTRANT:JP_EXIT_ARGUMENT;
    bootstrap.asset_token=token;bootstrap.resources|=JP_EXIT_ASSETS;return 0;
}
int jp_exit_attach_bootstrap_characters(jp_assets_boot_cleanup cleanup,void *context,uint64_t token)
{
    int rc=owner();if(rc)return rc;if(state.busy)return JP_EXIT_REENTRANT;
    if(state.bound||bootstrap.started||bootstrap.cleanup!=cleanup||bootstrap.context!=context||(bootstrap.resources&JP_EXIT_CHARACTER_TEXTURES)||!(bootstrap.resources&JP_EXIT_ASSETS))return JP_EXIT_ALREADY_BOUND;
    struct jp_character_exit_info actual;rc=jp_character_exit_snapshot(&actual);
    if(rc||!actual.active||actual.token!=token||actual.terminal||actual.closing)return JP_EXIT_ARGUMENT;
    rc=token?jp_character_exit_can_close(token):jp_character_exit_activity();if(rc)return rc==JP_CHARACTER_EXIT_REENTRANT?JP_EXIT_REENTRANT:JP_EXIT_ARGUMENT;
    bootstrap.character_token=token;bootstrap.resources|=JP_EXIT_CHARACTER_TEXTURES;return 0;
}
static int asset_character_retire_preflight(void)
{
    uint32_t mask=state.bound?state.info.remaining:bootstrap.resources;
    uint64_t character=state.bound?state.character_token:bootstrap.character_token;
    uint64_t asset=state.bound?state.asset_token:bootstrap.asset_token;
    if(mask&JP_EXIT_CHARACTER_TEXTURES){
        int rc=character?jp_character_exit_can_retire(character):jp_character_exit_activity();
        if(!rc&&!character){struct jp_character_exit_info info;rc=jp_character_exit_snapshot(&info);if(!rc&&(info.users||jp_asset_loader_activity()))rc=JP_CHARACTER_EXIT_BUSY;}
        if(rc)return rc==JP_CHARACTER_EXIT_THREAD?JP_EXIT_THREAD:rc==JP_CHARACTER_EXIT_REENTRANT?JP_EXIT_REENTRANT:failed(JP_EXIT_CHARACTER_TEXTURES,rc);
    }
    /* Classic unclaimed ASSETS selection keeps its original cleanup contract.
     * The claimed/bootstrap path performs strict complete preflight first. */
    if((mask&JP_EXIT_ASSETS)&&(asset||(!state.bound&&(bootstrap.resources&JP_EXIT_ASSETS)))){
        int rc=asset?((mask&JP_EXIT_CHARACTER_TEXTURES)?jp_assets_exit_can_retire_after_character(asset,character):jp_assets_exit_can_retire(asset)):jp_assets_exit_activity();
        if(!rc&&!asset){struct jp_asset_exit_info info;struct jp_character_exit_info child;rc=jp_assets_exit_snapshot(&info);if(!rc)rc=jp_character_exit_snapshot(&child);if(!rc&&(info.decoded_bytes||jp_asset_loader_activity()||(child.active&&!(mask&JP_EXIT_CHARACTER_TEXTURES))))rc=JP_ASSET_EXIT_BUSY;}
        if(rc)return rc==JP_ASSET_EXIT_THREAD?JP_EXIT_THREAD:rc==JP_ASSET_EXIT_REENTRANT?JP_EXIT_REENTRANT:failed(JP_EXIT_ASSETS,rc);
    }
    return 0;
}
