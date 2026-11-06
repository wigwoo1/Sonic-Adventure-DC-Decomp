/* Read the original records, not a shadow stream or file pool. */
void jp_adxstm_records_info(uint32_t *live,uint32_t *pending,int32_t *error)
{
    *live=*pending=0;*error=jp_adxstm_native_error;
    for(int i=0;i<ADXSTM_MAX_OBJ;i++)if(adxstmf_obj[i].used){
        (*live)++;*pending+=adxstmf_obj[i].rqst==1;
    }
}
int jp_adxstm_records_initialize(void)
{
    uint32_t live,pending;int32_t error;jp_adxstm_records_info(&live,&pending,&error);
    if(live)return 1;jp_adxstm_native_error=0;ADXSTM_Init();return 0;
}
int jp_adxstm_exec_native(void)
{
    jp_adxstm_native_error=0;gdFsExecServer();ADXSTM_ExecServer();
    if(jp_adxstm_native_error)return jp_adxstm_native_error;
    for(int i=0;i<ADXSTM_MAX_OBJ;i++)if(adxstmf_obj[i].used&&adxstmf_obj[i].stat==ADXSTM_STAT_ERROR)return -1;
    return 0;
}
int jp_adxstm_records_close(void)
{
    int32_t first_error=0;
    for(int i=0;i<ADXSTM_MAX_OBJ;i++)if(adxstmf_obj[i].used){
        ADXSTM_Close(&adxstmf_obj[i]);
        if(adxstmf_obj[i].used&&!first_error)first_error=jp_adxstm_native_error;
    }
    for(int i=0;i<ADXSTM_MAX_OBJ;i++)if(adxstmf_obj[i].used){jp_adxstm_native_error=first_error;return 1;}
    return 0;
}
__declspec(dllexport) int peer_adxstm_state(ADXSTM handle,uint64_t *out)
{
    if(!handle||!out)return 1;ADXSTM_OBJ *s=handle;
    out[0]=s->used;out[1]=s->stat;out[2]=s->rqst;out[3]=(uintptr_t)s->sj;out[4]=(uintptr_t)s->gdfs;
    int32_t fields[]={s->stsct,s->ofst,s->fsize,s->rdsize,s->bufsize,s->rdsct,s->ck.len,s->maxsct,s->eossct,s->rdbyte};
    for(unsigned i=0;i<sizeof fields/sizeof fields[0];i++)out[5+i]=(uint64_t)(int64_t)fields[i];
    out[15]=(uintptr_t)s->ck.data;out[16]=(uintptr_t)s->eosfunc;out[17]=(uintptr_t)s->eosobj;
    out[18]=sizeof *s;out[19]=sizeof(GDFS_HANDLE);return 0;
}
