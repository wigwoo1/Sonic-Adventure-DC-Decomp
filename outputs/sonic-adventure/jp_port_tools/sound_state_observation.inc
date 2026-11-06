/* Read-only observations of actual imported game globals. Test inputs are
 * supplied by the verifier; there is no replacement state provider here. */
__declspec(dllexport) int peer_sound_state_addresses(uintptr_t *out)
{
    if(!out)return 1;out[0]=(uintptr_t)&camera_work;out[1]=(uintptr_t)&event_skipping;
    out[2]=(uintptr_t)&frame_redrawing;out[3]=(uintptr_t)pad_records;return 0;
}
__declspec(dllexport) int peer_sound_state_values(uint64_t *out)
{
    if(!out)return 1;out[0]=(uintptr_t)camera_work;out[1]=(uint32_t)event_skipping;
    out[2]=(uint32_t)frame_redrawing;
    for(int i=0;i<8;i++){out[3+i*2]=pad_records[i].press;out[4+i*2]=pad_records[i].release;}
    return 0;
}
__declspec(dllexport) int peer_sound_state_layout(uint32_t *out)
{
    if(!out)return 1;out[0]=sizeof(struct task_work);out[1]=__builtin_offsetof(struct task_work,pos);
    out[2]=sizeof(PDS_PERIPHERAL);out[3]=__builtin_offsetof(PDS_PERIPHERAL,press);
    out[4]=__builtin_offsetof(PDS_PERIPHERAL,release);return 0;
}
__declspec(dllexport) int peer_sound_camera_position(void *out)
{
    if(!out)return 1;if(!camera_work)return 2;memcpy(out,&camera_work->pos,12);return 0;
}
