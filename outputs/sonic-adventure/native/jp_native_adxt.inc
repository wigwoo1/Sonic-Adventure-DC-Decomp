/* Appended to the existing sound owner; this list owns allocations, not an
 * independent player registry. Actual adxt_obj records determine liveness. */
extern int jp_adxt_records_initialize(void),jp_adxt_records_close(void);
extern uint32_t jp_adxt_records_live(void);
extern int jp_adxt_record_valid(void *),jp_adxt_record_error(void);
extern int jp_adxt_work_live(void *);
extern void *jp_adxt_record_create(uint32_t,void *,uint32_t);
extern int jp_adxt_record_destroy(void *),jp_adxt_record_stop(void *);
extern int jp_adxt_record_fname(void *,char *),jp_adxt_record_afs(void *,uint32_t,uint32_t);
extern int jp_adxt_exec_native(void),jp_adxt_sdk_drain(void);
static struct jp_adxt_info adxt_owner;
static struct adxt_allocation {struct adxt_allocation *next;void *reserve,*work;uint64_t bytes;} *adxt_storage;
int jp_adxt_service_enabled(void){return adxt_owner.active&&!adxt_owner.stopping;}
int jp_adxt_blocks_lower_shutdown(void){return adxt_owner.active||adxt_storage!=NULL;}
static int jp_adxt_free_retired(void)
{
    struct adxt_allocation **at=&adxt_storage;int error=0;
    while(*at){struct adxt_allocation *node=*at;
        if(jp_adxt_work_live(node->work)){at=&node->next;continue;}
        if(!VirtualFree(node->reserve,0,MEM_RELEASE)){error=JP_SOUND_MEMORY;at=&node->next;continue;}
        *at=node->next;adxt_owner.owned_bytes-=node->bytes;adxt_owner.owned_work--;
        HeapFree(GetProcessHeap(),0,node);
    }
    return error;
}
static int jp_adxt_close_owned(void)
{
    if(!adxt_owner.active&&!jp_adxt_records_live()&&!adxt_storage)return 0;
    if(jp_g2_gate_get()!=0)return JP_SOUND_G2;
    adxt_owner.stopping=1;
    if(jp_adxt_records_close()){adxt_owner.status=JP_SOUND_CLEANUP;return JP_SOUND_CLEANUP;}
    int error=jp_adxt_free_retired();if(error){adxt_owner.status=error;return error;}
    if(adxt_storage)return JP_SOUND_CLEANUP;
    adxt_owner.active=adxt_owner.stopping=adxt_owner.status=0;adxt_owner.shutdowns++;return 0;
}
int jp_adxt_initialize(void)
{
    int error=begin(1);if(error)return error;
    if(adxt_owner.active||adxt_storage||!rna_owner.active||rna_owner.stopping||
       !adxstm_owner.active||adxstm_owner.stopping||!adxf_owner.active||adxf_owner.stopping)return end(JP_SOUND_STATE);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    struct jp_aica_info core;if(jp_aica_snapshot(&core)||core.phase!=JP_AICA_READY)return end(JP_SOUND_AICA);
    if(jp_adxt_records_initialize())return end(JP_SOUND_STATE);
    adxt_owner.active=1;adxt_owner.stopping=adxt_owner.status=0;adxt_owner.initializations++;
    adxt_owner.last_core_frame=adxt_owner.next_core_frame=core.frames;return end(0);
}
int jp_adxt_exec_due(void)
{
    struct jp_aica_info core;if(jp_aica_snapshot(&core)||core.phase!=JP_AICA_READY)return JP_SOUND_AICA;
    if(core.frames<adxt_owner.last_core_frame)return JP_SOUND_STATE;
    adxt_owner.last_core_frame=core.frames;
    for(unsigned budget=0;budget<8&&core.frames>=adxt_owner.next_core_frame;budget++){
        if(adxt_owner.next_core_frame>UINT64_MAX-735||adxt_owner.service_calls==UINT64_MAX)return JP_SOUND_STATE;
        int error=jp_adxt_exec_native();if(error){adxt_owner.status=JP_SOUND_SDK;return JP_SOUND_SDK;}
        adxt_owner.service_calls++;adxt_owner.next_core_frame+=735;
    }
    return 0;
}
int jp_adxt_service(void)
{
    int error=begin(1);if(error)return error;
    if(!jp_adxt_service_enabled())return end(JP_SOUND_STATE);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    return end(jp_game_sdk_service()?JP_SOUND_SDK:0);
}
int jp_adxt_shutdown(void)
{
    int error=begin(0);if(error)return error;
    if(!sound.owner)return end(JP_SOUND_STATE);return end(jp_adxt_close_owned());
}
int jp_adxt_snapshot(struct jp_adxt_info *out)
{
    AcquireSRWLockExclusive(&lock);int error=thread_error();if(error)return leave(error);
    if(!out||!outside_stage(out,sizeof *out))return leave(JP_SOUND_ARGUMENT);
    *out=adxt_owner;out->live_players=jp_adxt_records_live();out->native_error=jp_adxt_record_error();return leave(0);
}
int jp_adxt_create(uint32_t channels,uint32_t bytes,void **out)
{
    int error=begin(1);if(error)return error;
    if(!jp_adxt_service_enabled())return end(JP_SOUND_STATE);
    if(!out||!outside_stage(out,sizeof *out)||channels<1||channels>2)return end(JP_SOUND_ARGUMENT);
    *out=NULL;if(!bytes)bytes=channels==2?184516:55428;
    if(bytes<channels*16448+36+2048+31||bytes>16*1024*1024)return end(JP_SOUND_ARGUMENT);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    SYSTEM_INFO system;GetSystemInfo(&system);
    SIZE_T rounded=((SIZE_T)bytes+system.dwPageSize-1)&~((SIZE_T)system.dwPageSize-1);
    struct adxt_allocation *node=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof *node);
    if(!node)return end(JP_SOUND_MEMORY);
    node->reserve=VirtualAlloc(NULL,rounded+2*system.dwPageSize,MEM_RESERVE,PAGE_NOACCESS);
    if(!node->reserve){HeapFree(GetProcessHeap(),0,node);return end(JP_SOUND_MEMORY);}
    void *memory=(unsigned char *)node->reserve+system.dwPageSize;
    if(!VirtualAlloc(memory,rounded,MEM_COMMIT,PAGE_READWRITE)){
        VirtualFree(node->reserve,0,MEM_RELEASE);HeapFree(GetProcessHeap(),0,node);return end(JP_SOUND_MEMORY);}
    node->work=(unsigned char *)memory+rounded-((bytes+31)&~31u);node->bytes=bytes;
    node->next=adxt_storage;adxt_storage=node;adxt_owner.owned_bytes+=bytes;adxt_owner.owned_work++;
    *out=jp_adxt_record_create(channels,node->work,bytes);
    if(!*out){error=jp_adxt_free_retired();return end(error?error:JP_SOUND_MEMORY);}return end(0);
}
static int jp_adxt_control(void *h,int destroy)
{
    int error=begin(1);if(error)return error;
    if(!jp_adxt_service_enabled()||!jp_adxt_record_valid(h))return end(JP_SOUND_STATE);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    error=destroy?jp_adxt_record_destroy(h):jp_adxt_record_stop(h);
    if(error)return end(JP_SOUND_CLEANUP);
    return end(destroy?jp_adxt_free_retired():0);
}
int jp_adxt_destroy(void *h){return jp_adxt_control(h,1);}
int jp_adxt_stop(void *h){return jp_adxt_control(h,0);}
int jp_adxt_start_fname(void *h,const char *filename)
{
    int error=begin(1);if(error)return error;
    if(!jp_adxt_service_enabled()||!jp_adxt_record_valid(h))return end(JP_SOUND_STATE);
    if(!filename)return end(JP_SOUND_ARGUMENT);
    char name[32];unsigned i=0;for(;i<sizeof name;i++){name[i]=filename[i];if(!name[i])break;}
    if(!i||i==sizeof name)return end(JP_SOUND_ARGUMENT);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    return end(jp_adxt_record_fname(h,name)?JP_SOUND_FILE:0);
}
int jp_adxt_start_afs(void *h,uint32_t partition,uint32_t file)
{
    int error=begin(1);if(error)return error;
    if(!jp_adxt_service_enabled()||!jp_adxt_record_valid(h))return end(JP_SOUND_STATE);
    if(partition>=256||file>INT32_MAX)return end(JP_SOUND_ARGUMENT);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    return end(jp_adxt_record_afs(h,partition,file)?JP_SOUND_FILE:0);
}
