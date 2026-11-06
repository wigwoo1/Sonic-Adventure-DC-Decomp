/* Beside the existing GDFS owner; original handle pool remains authoritative. */
static uint64_t next_read_queue_token=1;
static int read_queue_handle_access(GDFS h)
{
    if(!handle_valid(h,1))return 0;
    uint64_t token=fs.records[h-gd_wk->hndlist].read_queue_token;
    return !token||(token==fs.read_queue_token&&fs.read_queue_active&&!fs.callback_depth);
}
static int read_queue_access(uint64_t token)
{
    int rc=access(1);if(rc)return rc;
    if(fs.api_depth||fs.callback_depth||fs.in_directory)return JP_GDFS_REENTRANT;
    return !token||token!=fs.read_queue_token||fs.closing?JP_GDFS_STATE:0;
}
int jp_gdfs_read_queue_claim(uint64_t *out)
{
    int rc=access(1);if(rc)return rc;
    if(fs.api_depth||fs.callback_depth||fs.in_directory)return JP_GDFS_REENTRANT;
    if(!out||!output_valid(out,sizeof *out))return JP_GDFS_ARGUMENT;
    if(fs.read_queue_token||fs.closing||fs.exit_started||!next_read_queue_token||!linked())return JP_GDFS_STATE;
    fs.read_queue_token=next_read_queue_token++;*out=fs.read_queue_token;return 0;
}
int jp_gdfs_read_queue_release(uint64_t token)
{
    int rc=read_queue_access(token);if(rc)return rc;
    if(fs.read_queue_active)return JP_GDFS_REENTRANT;
    for(uint32_t n=0;n<=fs.info.max_handles;n++)if(fs.records[n].read_queue_token==token)return JP_GDFS_ACTIVE;
    fs.read_queue_token=0;return 0;
}
int jp_gdfs_read_queue_enter(uint64_t token)
{
    int rc=read_queue_access(token);if(rc)return rc;
    if(fs.read_queue_active)return JP_GDFS_REENTRANT;
    fs.read_queue_active=1;return 0;
}
int jp_gdfs_read_queue_leave(uint64_t token)
{
    int rc=read_queue_access(token);if(rc)return rc;
    if(!fs.read_queue_active)return JP_GDFS_STATE;
    fs.read_queue_active=0;return 0;
}
int jp_gdfs_read_queue_close(uint64_t token,void *handle)
{
    int rc=read_queue_access(token);if(rc)return rc;GDFS h=handle;
    if(!fs.read_queue_active||!handle_valid(h,1)||fs.records[h-gd_wk->hndlist].read_queue_token!=token)return JP_GDFS_ARGUMENT;
    long stopped=gdFsStopRd(h);if(stopped)return result(JP_GDFS_SOURCE,stopped);
    gdFsClose(h);return h->used?result(JP_GDFS_ACTIVE,GDD_ERR_BUSY):0;
}
