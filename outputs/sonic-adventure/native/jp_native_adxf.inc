/* Same sound lock, creator thread and lifetime; allocations only own storage. */
extern int jp_adxf_records_initialize(void),jp_adxf_records_close(void);
extern void jp_adxf_records_info(uint32_t *,uint32_t *,uint32_t *,int32_t *);
extern uint64_t jp_adxf_work_bytes(uint32_t,int);
extern int jp_adxf_partition_present(uint32_t);
extern int jp_adxf_partition_apply(uint32_t,char *,void *,uint32_t,int);
extern void jp_adxf_partition_detach(uint32_t);
static struct jp_adxf_info adxf_owner;
static struct adxf_allocation {struct adxf_allocation *next;void *reserve,*work;uint64_t bytes;uint32_t id;} *adxf_storage;
int jp_adxf_service_enabled(void){return adxf_owner.active&&!adxf_owner.stopping;}
int jp_adxf_blocks_lower_shutdown(void){return adxf_owner.active||adxf_storage!=NULL;}
void jp_adxf_service_observe(void){adxf_owner.service_calls++;}
void jp_adxf_host_yield(void){Sleep(1);}
static int jp_adxf_free_storage(uint32_t id,struct adxf_allocation *keep)
{
    struct adxf_allocation **at=&adxf_storage;int error=0;
    while(*at){struct adxf_allocation *node=*at;
        if(node!=keep&&(id==UINT32_MAX||node->id==id)){
            if(!VirtualFree(node->reserve,0,MEM_RELEASE)){error=JP_SOUND_MEMORY;at=&node->next;continue;}
            *at=node->next;adxf_owner.owned_bytes-=node->bytes;adxf_owner.owned_nodes--;
            HeapFree(GetProcessHeap(),0,node);
        }else at=&node->next;
    }
    return error;
}
static int jp_adxf_close_owned(void)
{
    uint32_t live,pending,roots;int32_t native;jp_adxf_records_info(&live,&pending,&roots,&native);
    if(!adxf_owner.active&&!live&&!roots&&!adxf_storage)return 0;
    if(jp_g2_gate_get()!=0)return JP_SOUND_G2;
    int error=jp_adxstm_dependency();if(error)return error;
    adxf_owner.stopping=1;
    if(jp_adxf_records_close()){adxf_owner.status=JP_SOUND_CLEANUP;return JP_SOUND_CLEANUP;}
    error=jp_adxf_free_storage(UINT32_MAX,NULL);if(error){adxf_owner.status=error;return error;}
    adxf_owner.active=adxf_owner.stopping=adxf_owner.status=0;adxf_owner.shutdowns++;return 0;
}
int jp_adxf_initialize(void)
{
    int error=begin(1);if(error)return error;
    if(adxf_owner.active||!adxstm_owner.active||adxstm_owner.stopping)return end(JP_SOUND_STATE);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    if(jp_adxf_records_initialize())return end(JP_SOUND_STATE);
    adxf_owner.active=1;adxf_owner.stopping=adxf_owner.status=0;adxf_owner.initializations++;return end(0);
}
int jp_adxf_service(void)
{
    int error=begin(1);if(error)return error;
    if(!adxf_owner.active||adxf_owner.stopping||!adxstm_owner.active)return end(JP_SOUND_STATE);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    error=jp_game_sdk_service();if(error){adxf_owner.status=JP_SOUND_FILE;return end(JP_SOUND_FILE);}return end(0);
}
int jp_adxf_shutdown(void)
{
    int error=begin(0);if(error)return error;
    if(!sound.owner)return end(JP_SOUND_STATE);return end(jp_adxf_close_owned());
}
int jp_adxf_snapshot(struct jp_adxf_info *out)
{
    AcquireSRWLockExclusive(&lock);int error=thread_error();if(error)return leave(error);
    if(!out||!outside_stage(out,sizeof *out))return leave(JP_SOUND_ARGUMENT);
    *out=adxf_owner;jp_adxf_records_info(&out->live_files,&out->pending_reads,&out->partition_roots,&out->native_error);return leave(0);
}
static int jp_adxf_partition_change(uint32_t id,const char *filename,uint32_t count,int added)
{
    int error=begin(1);if(error)return error;
    if(!adxf_owner.active||adxf_owner.stopping||!adxstm_owner.active)return end(JP_SOUND_STATE);
    if(jp_g2_gate_get()!=0)return end(JP_SOUND_G2);
    if(id>=256||!filename||!count||count>65535)return end(JP_SOUND_ARGUMENT);
    char name[32];uint32_t i=0;for(;i<sizeof name;i++){name[i]=filename[i];if(!name[i])break;}
    if(i==sizeof name||!i)return end(JP_SOUND_ARGUMENT);
    if(added&&!jp_adxf_partition_present(id))return end(JP_SOUND_STATE);
    uint64_t bytes=jp_adxf_work_bytes(count,added);SYSTEM_INFO system;GetSystemInfo(&system);
    SIZE_T rounded=((SIZE_T)bytes+system.dwPageSize-1)&~((SIZE_T)system.dwPageSize-1);
    struct adxf_allocation *node=HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,sizeof *node);
    if(!node)return end(JP_SOUND_MEMORY);
    node->reserve=VirtualAlloc(NULL,rounded+2*system.dwPageSize,MEM_RESERVE,PAGE_NOACCESS);
    if(!node->reserve){HeapFree(GetProcessHeap(),0,node);return end(JP_SOUND_MEMORY);}
    unsigned char *memory=(unsigned char *)node->reserve+system.dwPageSize;
    if(!VirtualAlloc(memory,rounded,MEM_COMMIT,PAGE_READWRITE)){
        VirtualFree(node->reserve,0,MEM_RELEASE);HeapFree(GetProcessHeap(),0,node);return end(JP_SOUND_MEMORY);}
    node->work=memory+rounded-(((SIZE_T)bytes+7)&~(SIZE_T)7);node->bytes=bytes;node->id=id;
    int result=jp_adxf_partition_apply(id,name,node->work,count,added);
    if(result<0){VirtualFree(node->reserve,0,MEM_RELEASE);HeapFree(GetProcessHeap(),0,node);return end(JP_SOUND_FILE);}
    node->next=adxf_storage;adxf_storage=node;adxf_owner.owned_nodes++;adxf_owner.owned_bytes+=bytes;
    error=added?0:jp_adxf_free_storage(id,node);return end(error);
}
int jp_adxf_partition_load(uint32_t id,const char *name,uint32_t count){return jp_adxf_partition_change(id,name,count,0);}
int jp_adxf_partition_add(uint32_t id,const char *name,uint32_t count){return jp_adxf_partition_change(id,name,count,1);}
int jp_adxf_partition_unload(uint32_t id)
{
    int error=begin(1);if(error)return error;
    if(!adxf_owner.active||adxf_owner.stopping)return end(JP_SOUND_STATE);
    if(id>=256)return end(JP_SOUND_ARGUMENT);
    jp_adxf_partition_detach(id);return end(jp_adxf_free_storage(id,NULL));
}
