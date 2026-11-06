/* Appended to the actual sound owner; uses its existing lock/thread/lifetime. */
extern int jp_adxstm_records_initialize(void);
extern int jp_adxstm_records_close(void);
extern void jp_adxstm_records_info(uint32_t *,uint32_t *,int32_t *);
static struct jp_adxstm_info adxstm_owner;
int jp_adxstm_service_enabled(void){return adxstm_owner.active&&!adxstm_owner.stopping;}
void jp_adxstm_service_observe(void){adxstm_owner.service_calls++;}
static int jp_adxstm_dependency(void)
{
    struct jp_gdfs_info info;
    return jp_gdfs_snapshot(&info)||info.phase!=JP_GDFS_READY?JP_SOUND_FILE:0;
}
static int jp_adxstm_close_owned(void)
{
    uint32_t live,pending;int32_t native;
    jp_adxstm_records_info(&live,&pending,&native);
    if(!adxstm_owner.active&&!live)return 0;
    if(jp_g2_gate_get()!=0)return JP_SOUND_G2;
    int error=jp_adxstm_dependency();if(error)return error;
    adxstm_owner.stopping=1;
    /* GDFS already performs bounded cancellation of accepted requests. A
     * rejection retains the record/chunk for explicit retry; do not repeat a
     * blocked lifecycle call or silently free its borrowed storage. */
    if(jp_adxstm_records_close()){
        adxstm_owner.status=JP_SOUND_CLEANUP;return JP_SOUND_CLEANUP;
    }
    adxstm_owner.active=adxstm_owner.stopping=adxstm_owner.status=0;
    adxstm_owner.shutdowns++;return 0;
}
int jp_adxstm_initialize(void)
{
    int error=begin(1);if(error)return error;
    if(adxstm_owner.active)return end(JP_SOUND_STATE);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    error=jp_adxstm_dependency();if(error)return end(error);
    if(jp_adxstm_records_initialize())return end(JP_SOUND_STATE);
    adxstm_owner.active=1;adxstm_owner.stopping=adxstm_owner.status=0;
    adxstm_owner.initializations++;return end(0);
}
int jp_adxstm_service(void)
{
    int error=begin(1);if(error)return error;
    if(!adxstm_owner.active||adxstm_owner.stopping)return end(JP_SOUND_STATE);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    error=jp_adxstm_dependency();if(error)return end(error);
    error=jp_game_sdk_service();
    if(error){adxstm_owner.status=JP_SOUND_FILE;return end(JP_SOUND_FILE);}
    return end(0);
}
int jp_adxstm_shutdown(void)
{
    int error=begin(0);if(error)return error;
    if(!sound.owner)return end(JP_SOUND_STATE);
    return end(jp_adxstm_close_owned());
}
int jp_adxstm_snapshot(struct jp_adxstm_info *out)
{
    AcquireSRWLockExclusive(&lock);int error=thread_error();if(error)return leave(error);
    if(!out||!outside_stage(out,sizeof *out))return leave(JP_SOUND_ARGUMENT);
    *out=adxstm_owner;
    jp_adxstm_records_info(&out->live_streams,&out->pending_reads,&out->native_error);
    return leave(0);
}
