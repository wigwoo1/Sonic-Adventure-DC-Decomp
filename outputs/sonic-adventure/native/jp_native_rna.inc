/* Appended to the existing sound owner, sharing its actual lock and lifetime. */
extern void mwRnaInit(void);
extern void mwRnaFinish(void);
extern void mwRnaErrEntryFunc(void (*)(void *,char *),void *);
extern int jp_mwrna_live(void);
extern int jp_rna_sdk_pending(void);
extern int jp_rna_clock_begin(void);
extern int jp_rna_clock_error(void);
static struct jp_rna_info rna_owner;

int jp_rna_service_enabled(void)
{return rna_owner.active&&!rna_owner.stopping;}
void jp_rna_service_observe(void){rna_owner.service_calls++;}
static int jp_rna_drain_owned(void)
{
    /* Drain real SDK/DMA callbacks without asking RNA for another input chunk.
     * In particular, never spin inside its interrupt-masked Stop while a host
     * completion is still deferred. There is no synthetic transfer/ARM ACK. */
    for(uint32_t pass=0;pass<128;pass++){
        int pending=jp_rna_sdk_pending();
        if(pending<0){rna_owner.status=JP_SOUND_SDK;return JP_SOUND_SDK;}
        if(!pending)break;
        int error=jp_game_sdk_service();
        if(error){rna_owner.status=JP_SOUND_SDK;return JP_SOUND_SDK;}
    }
    if(jp_rna_sdk_pending()){rna_owner.status=JP_SOUND_CLEANUP;return JP_SOUND_CLEANUP;}
    return 0;
}
static int jp_rna_close_owned(void)
{
    if(!rna_owner.active&&!jp_mwrna_live())return 0;
    if(jp_g2_gate_get()!=0)return JP_SOUND_G2;
    rna_owner.stopping=1;
    int error=jp_rna_drain_owned();if(error)return error;
    mwRnaFinish();
    /* Stop/reset can themselves post a final real command-list DMA. */
    error=jp_rna_drain_owned();if(error)return error;
    if(jp_mwrna_live()||jp_rna_sdk_pending()){
        rna_owner.status=JP_SOUND_CLEANUP;return JP_SOUND_CLEANUP;}
    mwRnaErrEntryFunc(NULL,NULL);
    rna_owner.active=rna_owner.stopping=rna_owner.status=0;
    rna_owner.shutdowns++;return 0;
}
int jp_rna_initialize(void)
{
    int error=begin(1);if(error)return error;
    if(rna_owner.active||jp_mwrna_live())return end(JP_SOUND_STATE);
    if(jp_g2_gate_get()!=0||jp_rna_clock_begin())return end(JP_SOUND_DEPENDENCY);
    mwRnaInit();rna_owner.active=1;rna_owner.stopping=rna_owner.status=0;
    rna_owner.initializations++;return end(0);
}
int jp_rna_service(void)
{
    int error=begin(1);if(error)return error;
    if(!rna_owner.active||rna_owner.stopping)return end(JP_SOUND_STATE);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    error=jp_game_sdk_service();
    if(error){rna_owner.status=JP_SOUND_SDK;return end(JP_SOUND_SDK);}
    return end(0);
}
int jp_rna_shutdown(void)
{
    int error=begin(0);if(error)return error;
    if(!sound.owner)return end(JP_SOUND_STATE);
    return end(jp_rna_close_owned());
}
int jp_rna_snapshot(struct jp_rna_info *out)
{
    AcquireSRWLockExclusive(&lock);int error=thread_error();if(error)return leave(error);
    if(!out||!outside_stage(out,sizeof *out))return leave(JP_SOUND_ARGUMENT);
    *out=rna_owner;out->live_renderers=(uint32_t)jp_mwrna_live();
    int pending=jp_rna_sdk_pending();out->pending_transfers=pending<0?0:(uint32_t)pending;
    out->clock_error=(uint32_t)jp_rna_clock_error();return leave(0);
}
