/* Actual original pool readback, kept private in the combined SDK build. */
int jp_mwrna_live(void)
{
    int count=0;for(int i=0;i<MWRNA_MAX_OBJ;i++)count+=mwg_rna_obj[i].used==1;
    return count;
}
__declspec(dllexport) int peer_rna_layout(uint32_t *out)
{
    if(!out)return 1;out[0]=sizeof(MWRNA_OBJ);out[1]=offsetof(MWRNA_OBJ,sj);
    out[2]=offsetof(MWRNA_OBJ,ck);out[3]=offsetof(MWRNA_OBJ,bufptr);
    out[4]=offsetof(MWRNA_OBJ,snd);out[5]=offsetof(MWRNA_OBJ,trnsnsmpl);
    return 0;
}
__declspec(dllexport) int peer_rna_state(MWRNA rna,uint64_t *out)
{
    if(!rna||!out)return 1;
    out[0]=rna->used;out[1]=rna->stat;out[2]=rna->maxnch;out[3]=rna->nch;
    out[4]=rna->ovrflw;out[5]=rna->copying;out[6]=rna->flush;out[7]=rna->pause;
    out[8]=rna->bps;out[9]=rna->sfreq;
    for(int i=0;i<2;i++){out[10+i]=(uintptr_t)rna->sj[i];out[12+i]=(uintptr_t)rna->ck[i].data;
        out[14+i]=(uint64_t)(int64_t)rna->ck[i].len;out[16+i]=(uint32_t)rna->bufptr[i];}
    out[18]=rna->bufsize;out[19]=rna->wrpos;out[20]=rna->rdpos;out[21]=rna->ndata;
    out[22]=(uintptr_t)rna->snd;
    int32_t values[]={rna->trnsnsmpl,rna->playndata,rna->startsmpl,rna->nplay,rna->flshpos,rna->nflsh,rna->flshnsmpl,
        rna->outvol,rna->outpan[0],rna->outpan[1],rna->fxch,rna->fxlev,rna->semi,rna->fine,rna->coff,rna->q,rna->ntotal,rna->swsave};
    for(unsigned i=0;i<sizeof values/sizeof values[0];i++)out[23+i]=(uint64_t)(int64_t)values[i];
    return 0;
}
