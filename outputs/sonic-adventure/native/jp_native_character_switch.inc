/* Same character_owner, original users/lists, and actual shared game globals. */
static int switch_result(int rc,int native)
{character_owner.switching.status=(uint32_t)rc;character_owner.switching.native_error=(uint32_t)native;return rc;}
static int switch_access(void)
{
    int rc=character_access();if(rc)return rc==JP_CHARACTER_TEXTURE_THREAD?JP_SWITCH_THREAD:rc==JP_CHARACTER_TEXTURE_REENTRANT?JP_SWITCH_REENTRANT:JP_SWITCH_STATE;
    return character_owner.switch_held?JP_SWITCH_REENTRANT:0;
}
static int switch_playable(int32_t c)
{return c==0||c==2||c==3||c==5||c==6||c==7;}
static int switch_consumers(void)
{
    int owned=character_owner.switching.owned_character;
    for(unsigned c=0;c<8;c++)if(character_users[c]!=(owned==(int)c?1:0))return JP_SWITCH_BUSY;
    if(jp_asset_loader_activity())return JP_SWITCH_BUSY;
    struct jp_asset_info a;if(jp_assets_snapshot(&a)||a.pending_refs||a.decoded_bytes)return JP_SWITCH_BUSY;
    return 0;
}
static int switch_release(int character)
{
    character_owner.switching.stage=1;character_owner.switch_call=1;
    int rc=jp_character_textures_release((uint32_t)character);character_owner.switch_call=0;
    if(rc)return switch_result(JP_SWITCH_TEXTURE,rc);
    character_owner.switching.owned_character=-1;return 0;
}
static int switch_motion(int character)
{
    character_owner.switching.stage=2;int rc=jp_motions_select(character);
    return rc?switch_result(JP_SWITCH_MOTION,rc):0;
}
static int switch_load(int character)
{
    character_owner.switching.stage=3;character_owner.switch_call=1;
    int rc=jp_character_textures_acquire((uint32_t)character);character_owner.switch_call=0;
    if(rc)return switch_result(JP_SWITCH_TEXTURE,rc);
    character_owner.switching.owned_character=character;return 0;
}
static void switch_complete(void)
{
    character_owner.switch_held=0;
    if(!character_owner.switching.status){character_owner.switching.phase=1;character_owner.switching.stage=4;character_owner.switching.completions++;}
    else character_owner.switching.phase=2;
}
int jp_character_switch(int32_t character)
{
    int rc=switch_access();if(rc)return rc;
    if(!switch_playable(character)||!switch_playable(character_current)||(unsigned)save_slot>=SAVE_COUNT)return switch_result(JP_SWITCH_ARGUMENT,0);
    if(character_owner.info.phase!=1||character_owner.switching.phase==2)return switch_result(JP_SWITCH_STATE,0);
    rc=switch_consumers();if(rc)return switch_result(rc,0);
    rc=character_native_status();if(rc)return switch_result(JP_SWITCH_TEXTURE,rc);
    rc=jp_motions_can_select(character);if(rc)return switch_result(JP_SWITCH_MOTION,rc);
    for(unsigned c=0;c<8;c++){rc=character_validate(c,character_users[c]!=0);if(rc)return switch_result(JP_SWITCH_TEXTURE,rc);}
    character_owner.switching.starts++;character_owner.switching.requested=character;character_owner.switching.save_slot=save_slot;
    character_owner.switch_held=1;switch_result(0,0);jp_original_set_character((Sint16)character);switch_complete();
    return (int)character_owner.switching.status;
}
int jp_character_switch_recover(void)
{
    int rc=switch_access();if(rc)return rc;
    if(character_owner.switching.phase!=2||(uint32_t)save_slot!=character_owner.switching.save_slot)return switch_result(JP_SWITCH_STATE,0);
    int target=character_owner.switching.requested;uint32_t stage=character_owner.switching.stage;
    if(!switch_playable(target)||stage<1||stage>3||(stage>1&&character_current!=target))return switch_result(JP_SWITCH_IDENTITY,0);
    if(character_owner.info.phase==2){rc=jp_character_textures_recover();if(rc)return switch_result(JP_SWITCH_TEXTURE,rc);}
    if(character_owner.info.phase!=1)return switch_result(JP_SWITCH_STATE,0);
    rc=switch_consumers();if(rc)return switch_result(rc,0);rc=character_native_status();if(rc)return switch_result(JP_SWITCH_TEXTURE,rc);
    if(stage==1){character_owner.switching.phase=0;rc=jp_character_switch(target);if(rc)character_owner.switching.phase=2;return rc;}
    if(stage==2){rc=jp_motions_can_select(target);if(rc)return switch_result(JP_SWITCH_MOTION,rc);}
    else {struct jp_motion_info m;rc=jp_motions_snapshot(&m);if(rc||m.phase!=1||m.character!=target)return switch_result(JP_SWITCH_IDENTITY,rc);}
    character_owner.switch_held=1;switch_result(0,0);
    if((stage==2&&switch_motion(target))||switch_load(target)){switch_complete();return (int)character_owner.switching.status;}
    jp_original_character_finish();switch_complete();return (int)character_owner.switching.status;
}
int jp_character_switch_retire(void)
{
    int rc=switch_access();if(rc)return rc;if(character_owner.info.phase!=1)return JP_SWITCH_STATE;
    int owned=character_owner.switching.owned_character;if(owned<0)return 0;
    character_owner.switch_call=1;rc=jp_character_textures_release((uint32_t)owned);character_owner.switch_call=0;
    if(rc)return switch_result(JP_SWITCH_TEXTURE,rc);
    character_owner.switching.owned_character=-1;character_owner.switching.retirements++;
    if(character_owner.switching.phase!=2)character_owner.switching.phase=3;return switch_result(0,0);
}
int jp_character_switch_snapshot(struct jp_character_switch_info *out)
{
    DWORD thread=character_exit_thread();if(thread&&thread!=GetCurrentThreadId())return JP_SWITCH_THREAD;
    if(!out)return JP_SWITCH_ARGUMENT;struct jp_character_switch_info v=character_owner.switching;
    v.current=character_current;v.creator_thread=thread;*out=v;return 0;
}
