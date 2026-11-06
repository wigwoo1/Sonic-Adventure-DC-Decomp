/* Readback of the actual SDK-owned records, not another port or command pool. */
_Static_assert(sizeof(struct sd_stream)==44,"stream scalars retain original layout");
_Static_assert(sizeof(struct sd_ch_status)==32,"driver channel status wire layout");
_Static_assert(sizeof(SDS_PSTM_STAT)==28,"public stream status wire layout");
__declspec(dllexport) int peer_pstm_layout(uint64_t *out)
{
    if(!out)return 1;
    out[0]=sizeof(struct sd_stream);out[1]=offsetof(struct sd_work,streams);
    out[2]=offsetof(struct sd_work,host);out[3]=offsetof(struct sd_work,sound_status);
    out[4]=offsetof(struct sd_work,ports.kind.pstm);out[5]=SDD_PSTM_PORT_MAX;
    out[6]=sizeof(struct sd_sound_status);out[7]=offsetof(struct sd_sound_status,pstm);
    return 0;
}
__declspec(dllexport) int peer_pstm_record(uint32_t index,void *out)
{
    if(!sdgWrkPtr||!out||index>=SDD_PSTM_PORT_MAX)return 1;
    memcpy(out,&sdgWrkPtr->streams[index],sizeof(struct sd_stream));return 0;
}
__declspec(dllexport) void *peer_pstm_port(uint32_t index)
{
    return sdgWrkPtr&&index<SDD_PSTM_PORT_MAX?&sdgWrkPtr->ports.kind.pstm[index]:NULL;
}
