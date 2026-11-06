/* Included in the existing sound owner, after its stream owner. */
extern int jp_effects_request_owned(const struct jp_effects_request *,int32_t *);
extern int jp_effects_load_owned(const char *,uint32_t,int32_t);
extern int jp_effects_control_owned(uint32_t,intptr_t,int32_t);
extern void jp_effects_queue_owned(void),jp_effects_state_owned(struct jp_effects_info *);
static struct jp_effects_info effects;
static void *effects_allocation,*effects_stage;
static uint64_t effects_reservation;
static int jp_effects_outside(const void *pointer,uint64_t bytes)
{
    uintptr_t p=(uintptr_t)pointer,a=(uintptr_t)effects_allocation;
    return p&&bytes<=UINTPTR_MAX-p&&(!a||p+bytes<=a||p>=a+effects_reservation);
}
int jp_effects_error(long error)
{if(error){effects.sdk_error=(int32_t)error;effects.status=JP_SOUND_SDK;}return error!=0;}
int jp_effects_failed(void){return (int)effects.status;}
void *jp_effects_stage_allocate(long bytes)
{
    if(effects_allocation||bytes<32||bytes>2*1024*1024){effects.status=JP_SOUND_ARGUMENT;return NULL;}
    SYSTEM_INFO si;GetSystemInfo(&si);SIZE_T rounded=((SIZE_T)bytes+2047)/2048*2048;
    SIZE_T commit=(rounded+si.dwPageSize-1)/si.dwPageSize*si.dwPageSize;
    effects_allocation=VirtualAlloc(NULL,commit+2*si.dwPageSize,MEM_RESERVE,PAGE_NOACCESS);
    if(effects_allocation)effects_reservation=commit+2*si.dwPageSize;
    if(effects_allocation)effects_stage=VirtualAlloc((char *)effects_allocation+si.dwPageSize,commit,MEM_COMMIT,PAGE_READWRITE);
    if(!effects_stage){effects.status=JP_SOUND_MEMORY;effects.native_error=(int32_t)GetLastError();return NULL;}
    effects.stage_address=(uintptr_t)effects_stage;effects.stage_capacity=rounded;effects.retained_stage=1;
    return effects_stage;
}
static int jp_effects_stage_free(void)
{
    if(effects_allocation&&!VirtualFree(effects_allocation,0,MEM_RELEASE))return JP_SOUND_MEMORY;
    effects_stage=effects_allocation=NULL;effects_reservation=0;effects.stage_address=effects.stage_capacity=effects.retained_stage=0;return 0;
}
int jp_effects_stage_complete(int pending,uint32_t bytes)
{
    if(pending){jp_sound_problem(JP_SOUND_CLEANUP,JP_SOUND_OWNS_SDK,0);effects.status=JP_SOUND_CLEANUP;return (int)effects.status;}
    int error=jp_effects_stage_free();if(error)effects.status=(uint32_t)error;
    if(!effects.status){effects.loads++;effects.loaded_bytes+=bytes;}return (int)effects.status;
}
void jp_effects_file_error(void){effects.status=JP_SOUND_FILE;}
void jp_effects_pending_file(void)
{effects.status=JP_SOUND_CLEANUP;jp_sound_problem(JP_SOUND_CLEANUP,JP_SOUND_OWNS_FILE,0);}
static int effects_enter(void)
{
    int error=begin(1);if(error)return error;
    if(effects_allocation){end(0);return JP_SOUND_CLEANUP;}
    if(jp_g2_gate_get()!=0){end(0);return JP_SOUND_G2;}
    effects.status=0;effects.sdk_error=effects.native_error=0;return 0;
}
int jp_effects_queue(void)
{
    int error=effects_enter();if(error)return error;
    effects.queue_calls++;jp_effects_queue_owned();return end(effects.status?(int)effects.status:(int)sound.info.status);
}
static int32_t frame_effects(void *context){(void)context;return jp_effects_queue();}
int jp_effects_request(const struct jp_effects_request *r,int32_t *slot)
{
    int error=effects_enter();if(error)return error;
    if(!r||!slot||!outside_stage(r,sizeof *r)||!outside_stage(slot,sizeof *slot)||
       r->bytes!=sizeof *r||r->kind>5||r->se<0||r->volume < -127||r->volume>127||
       r->updates<0||r->updates==INT32_MAX||!isfinite(r->x)||!isfinite(r->y)||!isfinite(r->z)||
       !isfinite(r->radius)||fabsf(r->x)>1000000||fabsf(r->y)>1000000||fabsf(r->z)>1000000||
       r->radius<0||r->radius>1000000||r->id==-1)return end(JP_SOUND_ARGUMENT);
    if(r->se>1274){
        if(!game_stream_owner.active||game_stream_owner.stopping||!jp_stream_records_ready())return end(JP_SOUND_STATE);
        int64_t voice=(int64_t)r->se-1274-1+594;
        if(voice>=jp_stream_voice_count())return end(JP_SOUND_ARGUMENT);
    }
    error=jp_effects_request_owned(r,slot);if(!error)effects.requests++;return end(error);
}
int jp_effects_load_file(const char *name,uint32_t kind,int32_t bank)
{
    int error=effects_enter();if(error)return error;
    if(!name||!outside_stage(name,1)||kind>3||(kind&&(bank<0||bank>15)))return end(JP_SOUND_ARGUMENT);
    size_t n=strnlen(name,256);if(!n||n==256)return end(JP_SOUND_ARGUMENT);
    for(size_t i=0;i<n;i++)if((unsigned char)name[i]<32||strchr("/\\:",name[i]))return end(JP_SOUND_ARGUMENT);
    error=jp_effects_load_owned(name,kind,bank);return end(error?error:(int)effects.status);
}
int jp_effects_control(uint32_t action,int64_t id,int32_t value)
{
    int error=effects_enter();if(error)return error;
    if(action>8||(action==0&&value<0)||(action==2&&(value < -127||value>127))||
       (action==3&&(value < -128||value>127))||(action==4&&(value < -8192||value>8191))||
       (action==8&&(value < -6||value>15)))return end(JP_SOUND_ARGUMENT);
    error=jp_effects_control_owned(action,(intptr_t)id,value);return end(error?error:(int)effects.status);
}
int jp_effects_snapshot(struct jp_effects_info *out)
{
    AcquireSRWLockExclusive(&lock);int error=thread_error();if(error)return leave(error);
    if(!out||!outside_stage(out,sizeof *out))return leave(JP_SOUND_ARGUMENT);
    *out=effects;jp_effects_state_owned(out);return leave(0);
}
