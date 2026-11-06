/* Part of the existing sound owner. Original stream/task/ADXT records are the
 * authoritative handles. No second heap, task scheduler, clock or output. */
extern int jp_stream_records_open(void),jp_stream_records_close(void),jp_stream_records_ready(void);
extern void jp_stream_records_info(struct jp_game_stream_info *);
extern int jp_stream_request_native(int,int32_t),jp_stream_action_native(uint32_t,int32_t,int32_t *);
extern uint32_t jp_stream_voice_count(void);
static struct jp_game_stream_info game_stream_owner;
static unsigned game_stream_scope;
int jp_game_stream_blocks_parent(void){return game_stream_owner.active||game_stream_owner.archive_owned;}
int jp_game_stream_blocks_partition(uint32_t id){return id==0&&game_stream_owner.active;}
static int jp_game_stream_close_owned(void)
{
    struct jp_game_stream_info records={0};jp_stream_records_info(&records);
    if(!game_stream_owner.active&&!game_stream_owner.archive_owned&&!records.task&&!records.pending_task&&!records.music&&!records.voice)return 0;
    if(jp_g2_gate_get()!=0)return JP_SOUND_G2;
    game_stream_owner.stopping=1;
    if(jp_stream_records_close()){game_stream_owner.status=JP_SOUND_CLEANUP;return JP_SOUND_CLEANUP;}
    game_stream_owner.active=game_stream_owner.stopping=0;return 0;
}
int jp_game_stream_initialize(void)
{
    int error=begin(1);if(error)return error;
    if(game_stream_owner.active||!jp_adxt_service_enabled()||jp_adxf_partition_present(0))return end(JP_SOUND_STATE);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    game_stream_owner.active=1;game_stream_owner.stopping=game_stream_owner.status=0;game_stream_owner.native_error=0;
    if(jp_stream_records_open()){game_stream_owner.active=0;game_stream_owner.status=JP_SOUND_MEMORY;return end(JP_SOUND_MEMORY);}
    return end(0);
}
int jp_game_stream_shutdown(void)
{
    int error=begin(0);if(error)return error;
    if(!sound.owner)return end(JP_SOUND_STATE);return end(jp_game_stream_close_owned());
}
int jp_game_stream_snapshot(struct jp_game_stream_info *out)
{
    AcquireSRWLockExclusive(&lock);int error=thread_error();if(error)return leave(error);
    if(!out||!outside_stage(out,sizeof *out))return leave(JP_SOUND_ARGUMENT);
    *out=game_stream_owner;jp_stream_records_info(out);return leave(0);
}
int jp_game_stream_task_enter(void)
{
    int error=begin(1);if(error)return error;
    if(!game_stream_owner.active||game_stream_owner.stopping){end(0);return JP_SOUND_STATE;}
    game_stream_scope++;return 0;
}
void jp_game_stream_task_leave(void){game_stream_scope--;end(0);}
void jp_game_stream_step_observe(void){game_stream_owner.steps++;}
void jp_game_stream_ready_observe(void){game_stream_owner.initializations++;}
void jp_game_stream_error_observe(void){game_stream_owner.errors++;}
void jp_game_stream_fail(int error){game_stream_owner.status=error;game_stream_owner.native_error=error;}
/* -error rejects cleanup and retains audio storage after an externally deleted
 * task. 0 borrows an existing authorized scope; 1 owns a newly entered scope. */
int jp_game_stream_close_enter(void)
{
    int error=thread_error();if(error)return -error;
    if(jp_g2_gate_get()!=0){jp_game_stream_fail(JP_SOUND_G2);return -JP_SOUND_G2;}
    if(sound.busy){
        if(game_stream_scope||game_stream_owner.stopping)return 0;
        jp_game_stream_fail(JP_SOUND_REENTRANT);return -JP_SOUND_REENTRANT;
    }
    error=begin(0);return error?-error:1;
}
void jp_game_stream_close_leave(int scope,int complete)
{
    if(complete&&game_stream_owner.active){game_stream_owner.active=game_stream_owner.stopping=0;game_stream_owner.shutdowns++;}
    if(scope)end(0);
}
int jp_game_stream_archive_load(void)
{
    int error=jp_stream_adxf_partition_owned(0,"event_adx.afs",2148,0);
    if(!error)game_stream_owner.archive_owned=1;else jp_game_stream_fail(error);return error;
}
int jp_game_stream_archive_release(void)
{
    if(!game_stream_owner.archive_owned)return 0;
    jp_adxf_partition_detach(0);int error=jp_adxf_free_storage(0,NULL);
    if(!error)game_stream_owner.archive_owned=0;else jp_game_stream_fail(error);return error;
}
void *jp_game_stream_player_create(uint32_t channels,uint32_t bytes)
{
    void *h=NULL;int error=jp_stream_adxt_create_owned(channels,bytes,&h);
    if(error)jp_game_stream_fail(error);return h;
}
int jp_game_stream_player_destroy(void *h)
{int error=jp_stream_adxt_control_owned(h,1);if(error)jp_game_stream_fail(error);return error;}
static int jp_game_stream_request(int kind,int32_t value)
{
    int error=begin(1);if(error)return error;
    if(!game_stream_owner.active||game_stream_owner.stopping||!jp_stream_records_ready())return end(JP_SOUND_STATE);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    return end(jp_stream_request_native(kind,value));
}
int jp_game_stream_request_music(int32_t value){return jp_game_stream_request(0,value);}
int jp_game_stream_request_voice(int32_t value){return jp_game_stream_request(1,value);}
int jp_game_stream_request_interlude(int32_t value){return jp_game_stream_request(2,value);}
int jp_game_stream_action(uint32_t action,int32_t value,int32_t *result)
{
    int error=begin(1);if(error)return error;
    if(!game_stream_owner.active||game_stream_owner.stopping||!jp_stream_records_ready())return end(JP_SOUND_STATE);
    if(!result||!outside_stage(result,sizeof *result))return end(JP_SOUND_ARGUMENT);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    return end(jp_stream_action_native(action,value,result));
}
