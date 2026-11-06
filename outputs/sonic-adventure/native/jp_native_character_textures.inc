/* Included after genuine data, tables and guarded original function bodies. */
static struct {struct jp_character_texture_info info;DWORD thread;int active,busy,partial_releasing;} character_owner={.info.partial_character=-1};
static int character_result(int rc,int native)
{character_owner.info.status=(uint32_t)rc;character_owner.info.native_error=(uint32_t)native;return rc;}
static int character_access(void)
{return character_owner.thread&&character_owner.thread!=GetCurrentThreadId()?JP_CHARACTER_TEXTURE_THREAD:character_owner.busy?JP_CHARACTER_TEXTURE_REENTRANT:!character_owner.active?JP_CHARACTER_TEXTURE_STATE:0;}
static int character_native_status(void)
{
    int rc=jp_assets_status();if(rc)return character_result(JP_CHARACTER_TEXTURE_ASSET,rc);
    rc=jp_memory_status();if(!rc)rc=jp_textures_status();
    return rc?character_result(JP_CHARACTER_TEXTURE_NATIVE,rc):0;
}
static uint32_t character_loaded_lists(void)
{
    uint32_t mask=0;for(unsigned i=0;i<17;i++)if(jp_pvm_loaded_count(character_list_definitions[i].list))mask|=UINT32_C(1)<<i;return mask;
}
static int character_validate(unsigned character,int loaded)
{
    for(struct texture_pvm *p=character_pvms[character];p->texlist;p++){
        uint32_t count=jp_pvm_loaded_count(p->texlist);
        if(loaded){if(count!=p->texlist->nbTexture||!jp_textures_validate_list(p->texlist))return character_result(JP_CHARACTER_TEXTURE_NATIVE,jp_textures_status());}
        else if(count)return character_result(JP_CHARACTER_TEXTURE_STATE,0);
    }return 0;
}
/* Runs after each actual original dependency call; returning from the guarded
 * original body stops a failed load before its logical users increment. */
static int character_dependency_failed(NJS_TEXLIST *list,int releasing)
{
    for(unsigned c=0;c<8;c++)for(struct texture_pvm *p=character_pvms[c];p->texlist;p++)if(p->texlist==list){character_owner.info.partial_character=(int32_t)c;character_owner.partial_releasing=releasing;}
    return character_native_status();
}
int jp_character_textures_initialize(void)
{
    if(character_owner.thread&&character_owner.thread!=GetCurrentThreadId())return JP_CHARACTER_TEXTURE_THREAD;
    if(character_owner.busy)return JP_CHARACTER_TEXTURE_REENTRANT;if(character_owner.active)return character_result(JP_CHARACTER_TEXTURE_STATE,0);
    int rc=character_native_status();if(rc)return rc;struct jp_asset_info info;
    if(jp_assets_snapshot(&info)||info.pending_refs||info.decoded_bytes)return character_result(JP_CHARACTER_TEXTURE_STATE,0);
    for(unsigned c=0;c<8;c++)if(character_users[c]||character_validate(c,0))return character_result(JP_CHARACTER_TEXTURE_STATE,0);
    character_owner.thread=GetCurrentThreadId();character_owner.active=1;character_owner.info.starts++;character_owner.info.phase=1;
    character_owner.info.creator_thread=character_owner.thread;character_owner.info.partial_character=-1;return character_result(0,0);
}
int jp_character_textures_acquire(uint32_t character)
{
    int rc=character_access();if(rc)return rc;if(character>=8)return character_result(JP_CHARACTER_TEXTURE_ARGUMENT,0);
    if(character_owner.info.phase!=1)return character_result(JP_CHARACTER_TEXTURE_STATE,0);
    if(character_users[character]<0||character_users[character]==127)return character_result(JP_CHARACTER_TEXTURE_LIMIT,0);
    rc=character_native_status();if(rc)return rc;rc=character_validate(character,character_users[character]!=0);if(rc)return rc;
    struct jp_asset_info assets;if(jp_assets_snapshot(&assets)||assets.pending_refs||assets.decoded_bytes)return character_result(JP_CHARACTER_TEXTURE_STATE,0);
    character_owner.busy=1;character_owner.info.partial_character=(int32_t)character;character_owner.partial_releasing=0;
    jp_original_texture_load_character((int)character);character_owner.busy=0;rc=character_native_status();
    if(rc){character_owner.info.phase=2;return rc;}
    character_owner.info.partial_character=-1;character_owner.info.acquires++;return character_result(0,0);
}
int jp_character_textures_release(uint32_t character)
{
    int rc=character_access();if(rc)return rc;if(character>=8)return character_result(JP_CHARACTER_TEXTURE_ARGUMENT,0);
    if(character_owner.info.phase!=1)return character_result(JP_CHARACTER_TEXTURE_STATE,0);
    rc=character_native_status();if(rc)return rc;if(character_users[character]){rc=character_validate(character,1);if(rc)return rc;}
    character_owner.busy=1;jp_original_texture_release_character((int)character);character_owner.busy=0;rc=character_native_status();
    if(rc){character_owner.info.phase=2;return rc;}
    character_owner.info.partial_character=-1;character_owner.info.releases++;return character_result(0,0);
}
int jp_character_textures_release_all(void)
{
    int rc=character_access();if(rc)return rc;if(character_owner.info.phase!=1)return character_result(JP_CHARACTER_TEXTURE_STATE,0);
    rc=character_native_status();if(rc)return rc;for(unsigned c=0;c<8;c++)if(character_users[c]){rc=character_validate(c,1);if(rc)return rc;}
    character_owner.busy=1;jp_original_texture_release_characters();character_owner.busy=0;rc=character_native_status();
    if(rc){character_owner.info.phase=2;return rc;}
    character_owner.info.partial_character=-1;character_owner.info.releases++;return character_result(0,0);
}
int jp_character_textures_recover(void)
{
    int rc=character_access();if(rc)return rc;if(character_owner.info.phase!=2||character_owner.info.partial_character<0)return character_result(JP_CHARACTER_TEXTURE_STATE,0);
    rc=character_native_status();if(rc)return rc;character_owner.busy=1;
    rc=jp_assets_recover_pending();
    if(!rc)for(struct texture_pvm *p=character_pvms[character_owner.info.partial_character];p->texlist;p++)if(jp_pvm_release_loaded(p->texlist)<0){rc=JP_CHARACTER_TEXTURE_NATIVE;break;}
    character_owner.busy=0;if(rc)return character_result(JP_CHARACTER_TEXTURE_NATIVE,rc);
    if(character_owner.partial_releasing)character_users[character_owner.info.partial_character]=0;
    character_owner.info.partial_character=-1;character_owner.info.phase=1;character_owner.info.recoveries++;return character_result(0,0);
}
int jp_character_textures_shutdown(void)
{
    int rc=character_access();if(rc)return rc;if(character_owner.info.phase!=1)return character_result(JP_CHARACTER_TEXTURE_STATE,0);
    for(unsigned c=0;c<8;c++)if(character_users[c])return character_result(JP_CHARACTER_TEXTURE_BUSY,0);
    if(character_loaded_lists())return character_result(JP_CHARACTER_TEXTURE_STATE,0);
    character_owner.active=0;character_owner.thread=0;character_owner.info.phase=3;return character_result(0,0);
}
int jp_character_textures_snapshot(struct jp_character_texture_info *out)
{
    if(character_owner.thread&&character_owner.thread!=GetCurrentThreadId())return JP_CHARACTER_TEXTURE_THREAD;
    if(!out)return JP_CHARACTER_TEXTURE_ARGUMENT;struct jp_character_texture_info info=character_owner.info;
    memcpy(info.users,character_users,sizeof info.users);info.loaded_lists=character_owner.active?character_loaded_lists():0;*out=info;return 0;
}
int jp_character_textures_list(uint32_t character,uint32_t index,NJS_TEXLIST **out)
{
    int rc=character_access();if(rc)return rc;if(!out||character>=8||index>=5)return JP_CHARACTER_TEXTURE_ARGUMENT;
    if(character_owner.info.phase!=1||!character_users[character])return JP_CHARACTER_TEXTURE_STATE;
    struct texture_pvm *p=character_pvms[character];for(unsigned i=0;i<index;i++){if(!p->texlist)return JP_CHARACTER_TEXTURE_ARGUMENT;p++;}
    if(!p->texlist)return JP_CHARACTER_TEXTURE_ARGUMENT;*out=p->texlist;return 0;
}
