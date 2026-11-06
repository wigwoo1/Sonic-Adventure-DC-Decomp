/* Read and operate the original pool; never maintain another player table. */
extern void (*adxerr_func)(void *,char *);
extern void *adxerr_obj;
static void (*adxt_previous_error)(void *,char *),(*adxt_claimed_error)(void *,char *);
static void *adxt_previous_error_obj,*adxt_claimed_error_obj;
static int adxt_error_owned;
static void jp_adxt_error_claim(void (*func)(void *,char *),void *obj)
{
    if(!adxt_error_owned){adxt_previous_error=adxerr_func;adxt_previous_error_obj=adxerr_obj;}
    adxt_claimed_error=func;adxt_claimed_error_obj=obj;adxt_error_owned=1;
}
static void jp_adxt_error_release(void)
{
    if(adxt_error_owned&&adxerr_func==adxt_claimed_error&&adxerr_obj==adxt_claimed_error_obj)
        ADXERR_EntryErrFunc(adxt_previous_error,adxt_previous_error_obj);
    adxt_error_owned=0;adxt_previous_error=adxt_claimed_error=NULL;
    adxt_previous_error_obj=adxt_claimed_error_obj=NULL;
}
uint32_t jp_adxt_records_live(void)
{uint32_t n=0;for(unsigned i=0;i<ADXT_MAX_OBJ;i++)n+=adxt_obj[i].used!=0;return n;}
int jp_adxt_record_valid(void *h)
{
    for(unsigned i=0;i<ADXT_MAX_OBJ;i++)if(h==&adxt_obj[i])return adxt_obj[i].used!=0;
    return 0;
}
int jp_adxt_record_error(void){return jp_adxt_native_error;}
int jp_adxt_work_live(void *work)
{
    for(unsigned i=0;i<ADXT_MAX_OBJ;i++)if(adxt_obj[i].used&&adxt_obj[i].obuf==work)return 1;
    return 0;
}
int jp_adxt_records_initialize(void)
{
    if(jp_adxt_records_live()||adxt_init_cnt)return 1;
    jp_adxt_native_error=0;ADXT_Init();return adxt_init_cnt!=1;
}
int jp_adxt_records_close(void)
{jp_adxt_native_error=0;ADXT_Finish();return jp_adxt_records_live()!=0||adxt_init_cnt!=0;}
void *jp_adxt_record_create(uint32_t channels,void *work,uint32_t bytes)
{jp_adxt_native_error=0;return ADXT_Create(channels,work,bytes);}
int jp_adxt_record_destroy(void *h)
{ADXT_Destroy(h);return jp_adxt_record_valid(h)||jp_adxt_native_error;}
int jp_adxt_record_stop(void *h)
{ADXT_Stop(h);return jp_adxt_native_error;}
int jp_adxt_record_fname(void *h,char *name)
{ADXT_StartFname(h,name);return jp_adxt_native_error||((ADXT)h)->stat!=ADXT_STAT_DECINFO;}
int jp_adxt_record_afs(void *h,uint32_t id,uint32_t file)
{ADXT_StartAfs(h,id,file);return jp_adxt_native_error||((ADXT)h)->stat!=ADXT_STAT_DECINFO;}
static void jp_adxt_pad_tail(void)
{
    for(unsigned i=0;i<ADXT_MAX_OBJ;i++){
        ADXT h=&adxt_obj[i];if(!h->used||!h->sjd||h->native_tail_padded||
            ADXSJD_GetStat(h->sjd)!=ADXSJD_STAT_DECEND)continue;
        int nch=ADXSJD_GetNumChan(h->sjd),bps=ADXSJD_GetOutBps(h->sjd);
        if(nch<1||nch>h->maxnch||(bps!=8&&bps!=16)){jp_adxt_native_error=-1;continue;}
        int unit=16*bps/8,bytes=SJ_GetNumData(h->sjo[0],SJ_LIN_DATA),pad=(unit-bytes%unit)%unit;
        int ready=1;for(int ch=0;ch<nch;ch++)if(SJ_GetNumData(h->sjo[ch],SJ_LIN_FREE)<pad)ready=0;
        if(!ready)continue;
        for(int ch=0;ch<nch;ch++){
            int remaining=pad;
            while(remaining){SJCK ck;h->sjo[ch]->vtbl->GetChunk(h->sjo[ch],SJ_LIN_FREE,remaining,&ck);
                if(ck.len<=0){jp_adxt_native_error=-1;break;}
                memset(ck.data,0,ck.len);remaining-=ck.len;h->sjo[ch]->vtbl->PutChunk(h->sjo[ch],SJ_LIN_DATA,&ck);}
        }
        if(!jp_adxt_native_error)h->native_tail_padded=1;
    }
}
int jp_adxt_exec_native(void)
{jp_adxt_native_error=0;gdFsExecServer();ADXT_ExecServer();return jp_adxt_native_error||jp_rna_clock_error();}
__declspec(dllexport) int peer_adxt_state(ADXT h,uint64_t *out)
{
    if(!jp_adxt_record_valid(h)||!out)return 1;
    out[0]=h->used;out[1]=h->stat;out[2]=h->pmode;out[3]=h->maxnch;
    out[4]=(uintptr_t)h->sjd;out[5]=(uintptr_t)h->stm;out[6]=(uintptr_t)h->rna;
    out[7]=(uintptr_t)h->sjf;out[8]=(uintptr_t)h->sji;
    out[9]=(uintptr_t)h->sjo[0];out[10]=(uintptr_t)h->sjo[1];out[11]=(uintptr_t)h->ibuf;
    out[12]=h->ibuflen;out[13]=h->ibufxlen;out[14]=(uintptr_t)h->obuf;
    int32_t values[]={h->obufsize,h->obufdist,h->svrfreq,h->maxsct,h->minsct,h->maxdecsmpl,
        h->lpcnt,h->lp_skiplen,h->ercode,h->edecpos,h->edeccnt,h->eshrtcnt,h->lpflg,h->autorcvr,
        h->pstwait_flag,h->pstready_flag,(int32_t)h->native_tail_padded,(int32_t)sizeof *h};
    for(unsigned i=0;i<sizeof values/sizeof values[0];i++)out[15+i]=(uint64_t)(int64_t)values[i];return 0;
}
