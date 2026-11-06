/* Native/guest layout and actual original expander state, no fake decoder. */
__declspec(dllexport) int peer_adx_error(void){return adxpd_internal_error;}
__declspec(dllexport) int peer_adx_layout(uint32_t *out)
{
    if(!out)return 1;out[0]=sizeof(ADXPD_OBJ);out[1]=__builtin_offsetof(ADXPD_OBJ,ibuf);
    out[2]=__builtin_offsetof(ADXPD_OBJ,req_nblk);out[3]=__builtin_offsetof(ADXPD_OBJ,obufl);
    out[4]=__builtin_offsetof(ADXPD_OBJ,obufr);out[5]=__builtin_offsetof(ADXPD_OBJ,delay);
    out[6]=__builtin_offsetof(ADXPD_OBJ,k);return 0;
}
__declspec(dllexport) int peer_adx_state(ADXPD x,uint64_t *out)
{
    if(!x||!out)return 1;out[0]=x->used;out[1]=x->no;out[2]=x->mode;out[3]=x->stat;
    out[4]=(uint64_t)(int64_t)x->nblk;out[5]=x->nch;out[6]=(uintptr_t)x->ibuf;
    out[7]=(uint64_t)(int64_t)x->req_nblk;out[8]=(uintptr_t)x->obufl;out[9]=(uintptr_t)x->obufr;
    for(int i=0;i<4;i++)out[10+i]=(uint64_t)(int64_t)((int16_t *)x->delay)[i];
    out[14]=(uint64_t)(int64_t)x->k[0];out[15]=(uint64_t)(int64_t)x->k[1];return 0;
}
