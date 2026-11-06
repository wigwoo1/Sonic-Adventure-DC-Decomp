/* Read actual original stream handles/requests and actual child task links. */
static struct task *stream_pending_task;
extern void (*adxerr_func)(void *,char *);
extern void *adxerr_obj;
static void (*stream_previous_error)(void *,char *);
static void *stream_previous_error_obj;
static int stream_error_owned;
static void jp_stream_pending_close(struct task *task)
{if(stream_pending_task==task)stream_pending_task=NULL;}
static void jp_stream_error_claim(void)
{
    if(!stream_error_owned){stream_previous_error=adxerr_func;stream_previous_error_obj=adxerr_obj;stream_error_owned=1;}
}
static void jp_stream_error_release(void)
{
    if(stream_error_owned&&adxerr_func==stream_ignore_error&&adxerr_obj==NULL)
        ADXERR_EntryErrFunc(stream_previous_error,stream_previous_error_obj);
    stream_error_owned=0;stream_previous_error=NULL;stream_previous_error_obj=NULL;
}
uint32_t jp_stream_voice_count(void){return jp_stream_adxf_file_count(0);}
int jp_stream_records_ready(void){return stream_task&&music_stream&&voice_stream;}
int jp_stream_records_open(void)
{
    if(stream_task||stream_pending_task||music_stream||voice_stream)return 1;
    stream_reset();return stream_pending_task==NULL;
}
int jp_stream_records_close(void)
{
    if(stream_pending_task)task_destroy(stream_pending_task);
    stream_destroy();
    if(music_stream||voice_stream)stream_close(NULL);
    if(stream_task||stream_pending_task||music_stream||voice_stream)return 1;
    int error=jp_game_stream_archive_release();if(error)return error;
    jp_stream_error_release();return 0;
}
void jp_stream_records_info(struct jp_game_stream_info *out)
{
    out->task=(uintptr_t)stream_task;out->pending_task=(uintptr_t)stream_pending_task;
    out->music=(uintptr_t)music_stream;out->voice=(uintptr_t)voice_stream;out->ready=jp_stream_records_ready();
    out->children=0;if(stream_task)for(struct task *at=stream_task->child;at&&out->children<4096;at=at->next)out->children++;
    out->music_request=music_request;out->voice_request=voice_request;out->music_current=music_current;out->music_saved=music_saved;
    out->music_state=music_state;out->voice_state=voice_state;
}
int jp_stream_request_native(int kind,int32_t value)
{
    if(kind==1){if(value< -1||(value>=0&&(uint32_t)value>=jp_stream_voice_count()))return 1;stream_request_voice(value);}
    else{
        if(value< -1||value>=MUSIC_COUNT)return 1;
        if(kind==0)stream_request_music(value);else if(kind==2){
            unsigned before=0;for(struct task *at=stream_task->child;at;at=at->next)before++;
            stream_request_interlude(value);
            unsigned after=0;for(struct task *at=stream_task->child;at;at=at->next)after++;
            if(after==before)return 9;
        }else return 1;
    }
    return 0;
}
int jp_stream_action_native(uint32_t action,int32_t value,int32_t *result)
{
    *result=0;
    switch(action){
    case JP_STREAM_STOP_MUSIC:stream_stop_music();break;
    case JP_STREAM_STOP_VOICE:stream_stop_voice();break;
    case JP_STREAM_MUTE:stream_mute();break;
    case JP_STREAM_UNMUTE:stream_unmute();break;
    case JP_STREAM_RESTART_MUSIC:*result=stream_restart_music();break;
    case JP_STREAM_RESTORE_MUSIC:*result=stream_restore_music();break;
    case JP_STREAM_RESET_MUSIC:stream_reset_music();break;
    case JP_STREAM_RESET_VOICE:stream_reset_voice();break;
    case JP_STREAM_SAVE_MUSIC:
        if(value< -1||value>=MUSIC_COUNT)return 1;stream_request_and_save_music(value);break;
    default:return 1;
    }
    if((action==JP_STREAM_STOP_MUSIC||action==JP_STREAM_STOP_VOICE)&&jp_adxt_record_error())return 13;
    return 0;
}
__declspec(dllexport) int peer_game_stream_music_file(uint32_t index,char *out,uint32_t bytes)
{
    if(index>=MUSIC_COUNT||!out||bytes<32)return 1;strcpy(out,music_files[index]);return 0;
}
/* Address of the existing decoder flag for private cross-module identity
 * qualification. No replacement flag is declared or maintained here. */
__declspec(dllexport) void *peer_game_stream_error_address(void)
{return &adxpd_internal_error;}
