/* Actual ADXF records/tables remain the only file and partition registry. */
void jp_adxf_records_info(uint32_t *live,uint32_t *pending,uint32_t *roots,int32_t *error)
{
    *live=*pending=*roots=0;*error=jp_adxf_native_error;
    for(int i=0;i<ADXF_OBJ_MAX;i++)if(adxf_obj[i].used){(*live)++;*pending+=jp_adxstm_handle_pending(adxf_obj[i].stm);}
    for(int i=0;i<ADXF_PART_MAX;i++)*roots+=adxf_ptinfo[i]!=NULL;
}
int jp_adxf_records_initialize(void)
{
    uint32_t live,pending,roots;int32_t error;jp_adxf_records_info(&live,&pending,&roots,&error);
    if(live||roots)return 1;jp_adxf_native_error=0;ADXF_Init();return 0;
}
int jp_adxf_records_close(void)
{
    int first=0;
    for(int i=0;i<ADXF_OBJ_MAX;i++)if(adxf_obj[i].used){ADXF_Close(&adxf_obj[i]);if(adxf_obj[i].used&&!first)first=jp_adxf_native_error;}
    for(int i=0;i<ADXF_OBJ_MAX;i++)if(adxf_obj[i].used){jp_adxf_native_error=first;return 1;}
    ADXF_Finish();return 0;
}
int jp_adxf_exec_native(void)
{
    jp_adxf_native_error=0;ADXF_ExecServer();return jp_adxf_native_error;
}
uint64_t jp_adxf_work_bytes(uint32_t n,int added)
{return added?sizeof(ADXF_PTINFO)+(uint64_t)n*sizeof(ADXF_ENTRY):(sizeof(ADXF_PTINFO)+((uint64_t)n+1)*2+3)&~UINT64_C(3);}
int jp_adxf_partition_present(uint32_t id){return id<ADXF_PART_MAX&&adxf_ptinfo[id]!=NULL;}
int jp_adxf_partition_apply(uint32_t id,char *name,void *work,uint32_t count,int added)
{
    ADXF_PTINFO *old=adxf_ptinfo[id];
    int result=added?ADXF_AddPartition(id,name,work,count):ADXF_LoadPartition(id,name,work,count);
    if(result<0)adxf_ptinfo[id]=old;
    jp_adxf_native_error=result;return result;
}
void jp_adxf_partition_detach(uint32_t id){adxf_ptinfo[id]=NULL;}
__declspec(dllexport) int peer_adxf_state(ADXF h,uint64_t *out)
{
    if(!h||!out)return 1;
    out[0]=h->used;out[1]=h->stat;out[2]=(uintptr_t)h->stm;out[3]=(uintptr_t)h->sj;
    int32_t fields[]={h->fad,h->fsize_sct,h->fsize_byte,h->skpos,h->rdstpos,h->rqnsct,h->rdnsct};
    for(unsigned i=0;i<sizeof fields/sizeof fields[0];i++)out[4+i]=(uint64_t)(int64_t)fields[i];
    out[11]=sizeof *h;return 0;
}
__declspec(dllexport) int peer_adxf_partition(uint32_t id,uint64_t *out)
{
    if(id>=ADXF_PART_MAX||!out)return 1;ADXF_PTINFO *pt=adxf_ptinfo[id];
    out[0]=(uintptr_t)pt;out[1]=sizeof *pt;out[2]=sizeof(ADXF_ENTRY);
    if(pt){out[3]=(uintptr_t)pt->next;out[4]=pt->size;out[5]=pt->top;out[6]=pt->nfile;out[7]=pt->nentry;out[8]=pt->type;}
    return 0;
}
