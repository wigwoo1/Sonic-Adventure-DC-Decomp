/* Private extension beside the real media owner. No History DLL import. */
static uint64_t next_history_token=1;
static int history_blocked(void)
{return flash.history_token&&!flash.history_active;}
int jp_flash_history_claim(uint64_t *out)
{
    int rc=jp_flash_owner_access();if(rc)return rc;
    if(!out)return JP_FLASH_ARGUMENT;
    if(flash.history_token||flash.fm_call||jp_flash_fm_is_open()||!next_history_token)return JP_FLASH_STATE;
    flash.history_token=next_history_token++;*out=flash.history_token;return 0;
}
int jp_flash_history_enter(uint64_t token)
{
    int rc=jp_flash_owner_access();if(rc)return rc;
    if(!token||flash.history_token!=token)return JP_FLASH_STATE;
    if(flash.history_active||flash.fm_call)return JP_FLASH_BUSY;
    flash.history_active=1;return 0;
}
int jp_flash_history_leave(uint64_t token)
{
    int rc=jp_flash_owner_access();if(rc)return rc;
    if(!token||flash.history_token!=token||!flash.history_active||flash.fm_call)return JP_FLASH_STATE;
    flash.history_active=0;return 0;
}
int jp_flash_history_cancel(uint64_t token)
{
    int rc=jp_flash_owner_access();if(rc)return rc;
    if(!token||flash.history_token!=token)return JP_FLASH_STATE;
    if(flash.history_active||flash.fm_call||flash.history_backup||jp_flash_fm_is_open())return JP_FLASH_BUSY;
    flash.history_token=0;return 0;
}
int jp_flash_history_snapshot(struct jp_flash_history_info *out)
{
    int rc=jp_flash_owner_access();if(rc)return rc;if(!out)return JP_FLASH_ARGUMENT;
    *out=(struct jp_flash_history_info){next_history_token,flash.history_token,
        flash.history_active,flash.created_profile,flash.history_backup!=NULL,0};return 0;
}
static int transaction_access(uint64_t token)
{
    int rc=jp_flash_owner_access();if(rc)return rc;
    if(!token||flash.history_token!=token)return JP_FLASH_STATE;
    return flash.history_active||flash.fm_call?JP_FLASH_BUSY:0;
}
int jp_flash_history_transaction_begin(uint64_t token)
{
    int rc=transaction_access(token);if(rc)return rc;
    if(flash.history_backup)return JP_FLASH_STATE;
    if(flash.info.status)return (int)flash.info.status;
    unsigned char *next=malloc(IMAGE_BYTES);if(!next)return fail(JP_FLASH_ALLOCATION,0);
    rc=load(next);if(!rc&&memcmp(next,flash.image,IMAGE_BYTES))rc=fail(JP_FLASH_CHANGED,0);
    if(rc){free(next);return rc;}
    flash.history_backup=flash.image;flash.image=next;
    flash.history_programs=flash.history_erases=0;return 0;
}
int jp_flash_history_transaction_commit(uint64_t token)
{
    int rc=transaction_access(token);if(rc)return rc;
    if(!flash.history_backup)return JP_FLASH_STATE;
    unsigned char *disk=malloc(IMAGE_BYTES);if(!disk)return fail(JP_FLASH_ALLOCATION,0);
    rc=load(disk);if(!rc&&memcmp(disk,flash.history_backup,IMAGE_BYTES))rc=fail(JP_FLASH_CHANGED,0);
    free(disk);if(!rc&&flash.info.status)rc=(int)flash.info.status;
    int changed=flash.history_programs||flash.history_erases;
    if(!rc&&changed)rc=persist(flash.image,1);
    if(rc)return rc;
    free(flash.history_backup);flash.history_backup=NULL;if(changed)flash.info.commits++;
    flash.info.programs+=flash.history_programs;flash.info.erases+=flash.history_erases;
    flash.history_programs=flash.history_erases=0;return 0;
}
int jp_flash_history_transaction_abort(uint64_t token)
{
    int rc=transaction_access(token);if(rc)return rc;
    if(!flash.history_backup)return JP_FLASH_STATE;
    rc=jp_flash_fm_close();if(rc)return rc;
    free(flash.image);flash.image=flash.history_backup;flash.history_backup=NULL;
    flash.history_programs=flash.history_erases=0;return 0;
}
