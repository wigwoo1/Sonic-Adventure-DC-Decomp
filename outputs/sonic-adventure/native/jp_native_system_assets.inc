static int system_asset_error(int rc,uint32_t bit)
{
    int thread=bit==JP_EXIT_ASSETS?JP_ASSET_EXIT_THREAD:JP_CHARACTER_EXIT_THREAD;
    int reentrant=bit==JP_EXIT_ASSETS?JP_ASSET_EXIT_REENTRANT:JP_CHARACTER_EXIT_REENTRANT;
    return rc==thread?JP_SYSTEM_THREAD:rc==reentrant?JP_SYSTEM_REENTRANT:result(JP_SYSTEM_CLEANUP,bit,rc);
}
static int system_assets_activity(int retiring)
{
    int own_character=(system_owner.info.resources&JP_EXIT_CHARACTER_TEXTURES)||system_owner.character_token;
    if(own_character){
        int rc=system_owner.character_token?(retiring?jp_character_exit_can_retire(system_owner.character_token):jp_character_exit_can_close(system_owner.character_token)):jp_character_exit_activity();
        if(!rc&&retiring&&!system_owner.character_token){struct jp_character_exit_info child;rc=jp_character_exit_snapshot(&child);if(!rc&&(child.users||jp_asset_loader_activity()))rc=JP_CHARACTER_EXIT_BUSY;}
        if(rc)return system_asset_error(rc,JP_EXIT_CHARACTER_TEXTURES);
    }
    if((system_owner.info.resources&JP_EXIT_ASSETS)||system_owner.asset_token){
        int rc=system_owner.asset_token?(retiring?(own_character?jp_assets_exit_can_retire_after_character(system_owner.asset_token,system_owner.character_token):jp_assets_exit_can_retire(system_owner.asset_token)):jp_assets_exit_can_close(system_owner.asset_token)):jp_assets_exit_activity();
        if(!rc&&retiring&&!system_owner.asset_token){
            struct jp_asset_exit_info asset;struct jp_character_exit_info child;rc=jp_assets_exit_snapshot(&asset);
            if(!rc)rc=jp_character_exit_snapshot(&child);
            if(!rc&&(asset.decoded_bytes||jp_asset_loader_activity()||(child.active&&!own_character)))rc=JP_ASSET_EXIT_BUSY;
        }
        if(rc)return system_asset_error(rc,JP_EXIT_ASSETS);
    }
    return 0;
}
static int system_characters_close(void)
{return system_owner.character_token?(system_owner.terminal?jp_character_exit_finish(system_owner.character_token):jp_character_exit_close(system_owner.character_token)):jp_character_exit_close_unclaimed((uint32_t)system_owner.terminal);}
static int system_assets_close(void)
{return system_owner.asset_token?(system_owner.terminal?jp_assets_exit_finish(system_owner.asset_token):jp_assets_exit_close(system_owner.asset_token)):jp_assets_exit_close_unclaimed((uint32_t)system_owner.terminal);}
static int system_assets_release(void)
{
    if(system_owner.terminal)return 0;int rc=0;
    if(system_owner.character_token){rc=jp_character_exit_cancel(system_owner.character_token);if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_CHARACTER_TEXTURES,rc);system_owner.character_token=0;}
    if(system_owner.asset_token){rc=jp_assets_exit_cancel(system_owner.asset_token);if(rc)return result(JP_SYSTEM_CLEANUP,JP_EXIT_ASSETS,rc);system_owner.asset_token=0;}
    return 0;
}
static int system_adopt_asset_owner(int character)
{
    int rc;
    if(character){
        struct jp_character_exit_info info;rc=jp_character_exit_snapshot(&info);if(rc)return system_asset_error(rc,JP_EXIT_CHARACTER_TEXTURES);
        if(!info.active||info.token||info.terminal)return JP_SYSTEM_STATE;
        system_owner.info.resources|=JP_EXIT_CHARACTER_TEXTURES;rc=jp_character_exit_claim(&system_owner.character_token);
        int attached=jp_exit_attach_bootstrap_characters(bootstrap_cleanup,&system_owner,system_owner.character_token);
        if(attached)return result(JP_SYSTEM_EXIT,JP_EXIT_CHARACTER_TEXTURES,attached);
        return rc?system_asset_error(rc,JP_EXIT_CHARACTER_TEXTURES):0;
    }
    struct jp_asset_exit_info info;rc=jp_assets_exit_snapshot(&info);if(rc)return system_asset_error(rc,JP_EXIT_ASSETS);
    if(!info.initialized||info.token||info.terminal)return JP_SYSTEM_STATE;
    system_owner.info.resources|=JP_EXIT_ASSETS;rc=jp_assets_exit_claim(&system_owner.asset_token);
    int attached=jp_exit_attach_bootstrap_assets(bootstrap_cleanup,&system_owner,system_owner.asset_token);
    if(attached)return result(JP_SYSTEM_EXIT,JP_EXIT_ASSETS,attached);
    return rc?system_asset_error(rc,JP_EXIT_ASSETS):0;
}
static int system_asset_start_access(int character)
{
    int rc=access_owner();if(rc)return rc;
    if(system_owner.busy||(system_owner.game_init_token&&!system_owner.game_init_call))return JP_SYSTEM_REENTRANT;
    if(!system_owner.registered||system_owner.terminal||system_owner.info.phase!=JP_SYSTEM_READY)return JP_SYSTEM_STATE;
    if(character){
        if(!(system_owner.info.resources&JP_EXIT_ASSETS)||!system_owner.asset_token||system_owner.character_token||(system_owner.info.resources&JP_EXIT_CHARACTER_TEXTURES))return JP_SYSTEM_STATE;
        rc=jp_assets_exit_can_close(system_owner.asset_token);if(rc)return system_asset_error(rc,JP_EXIT_ASSETS);
    }else if(system_owner.asset_token||(system_owner.info.resources&JP_EXIT_ASSETS))return JP_SYSTEM_STATE;
    return 0;
}
int jp_system_adopt_assets(void)
{int rc=system_asset_start_access(0);if(rc)return rc;if(!system_owner.main_guard)return JP_SYSTEM_STATE;return system_adopt_asset_owner(0);}
int jp_system_adopt_character_textures(void)
{int rc=system_asset_start_access(1);if(rc)return rc;if(!system_owner.main_guard)return JP_SYSTEM_STATE;return system_adopt_asset_owner(1);}
static int system_asset_start_done(int character,int started,int own_guard)
{
    int active=0,rc=0;uint32_t bit=character?JP_EXIT_CHARACTER_TEXTURES:JP_EXIT_ASSETS;
    if(character){struct jp_character_exit_info info={0};rc=jp_character_exit_snapshot(&info);active=(int)info.active;}
    else{struct jp_asset_exit_info info={0};rc=jp_assets_exit_snapshot(&info);active=(int)info.initialized;}
    if(!rc&&active)rc=system_adopt_asset_owner(character);system_owner.busy=0;
    if(own_guard&&(!active||!rc)){int released=jp_system_main_initialization_guard(0);if(released)return released;}
    if(rc)return rc;
    return started?result(JP_SYSTEM_CLEANUP,bit,started):result(0,0,0);
}
int jp_system_start_assets(const wchar_t *root,const struct jp_asset_limits *limits)
{
    int rc=system_asset_start_access(0);if(rc)return rc;struct jp_asset_exit_info old;
    rc=jp_assets_exit_snapshot(&old);if(rc)return system_asset_error(rc,JP_EXIT_ASSETS);
    if(old.initialized||old.token||old.terminal)return JP_SYSTEM_STATE;
    int own_guard=!system_owner.main_guard;if(own_guard){rc=jp_system_main_initialization_guard(1);if(rc)return rc;}
    system_owner.busy=1;int started=jp_assets_initialize(root,limits);return system_asset_start_done(0,started,own_guard);
}
int jp_system_start_character_textures(void)
{
    int rc=system_asset_start_access(1);if(rc)return rc;struct jp_character_exit_info old;
    rc=jp_character_exit_snapshot(&old);if(rc)return system_asset_error(rc,JP_EXIT_CHARACTER_TEXTURES);
    if(old.active||old.token||old.terminal)return JP_SYSTEM_STATE;
    int own_guard=!system_owner.main_guard;if(own_guard){rc=jp_system_main_initialization_guard(1);if(rc)return rc;}
    system_owner.busy=1;int started=jp_character_textures_initialize();return system_asset_start_done(1,started,own_guard);
}
